/*****************************************************************************/
/*                                                                           */
/* File: egts_rx_fd.c                                                        */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Transport Layer                                           */
/*                                                                           */
/* Status: Version 1.1                                                       */
/*                                                                           */
/* Language: C                                                               */
/*                                                                           */
/* (c) Copyright JSC «Navigation-information systems», 2011                  */
/*                                                                           */
/* Address:                                                                  */
/*     24, Mishina Str., bld.1                                               */
/*     Moscow, Russia                                                        */
/*                                                                           */
/* Description: Main EGTS packet service layer decomposer.                   */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: public :                                                       */
/*                                                                           */
/*****************************************************************************/


#include "../include/egts_config.h"

#include "../include/egts.h"
#include "../include/egts_impl.h"
#include "egts_guts.h"

#include "egts_signup.h"
#include "../services/egts_services.h"

/*****************************************************************************/
/*                                                                           */
/* egts_proceed_rx_record()                                                  */
/*                                                                           */
/* Description: Record parser                                                */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            epacket - packet state mashine                                 */
/*            precord - resulting record                                     */
/*            ppbuf - raw buffer to parse                                    */
/*            pbuf_sz - raw buffer length                                    */
/*                                                                           */
/* Return:    zero on OK, nonzero value otherwise                            */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int  egts_proceed_rx_record(
  egts_state_t*         estate ,
  egts_packet_state_t*  epacket ,
  egts_record_t*        precord ,
  u8**  ppbuf ,
  u16*  pbuf_sz )
{

  EGTS_READ(precord->record.RL,*ppbuf,*pbuf_sz)
  EGTS_READ(precord->record.RN,*ppbuf,*pbuf_sz)
  
  if ( *pbuf_sz > 0U )  {
    egts_set_record_bits( &precord->record , **ppbuf );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }

  if ( precord->record.OBFE != 0U )
  {
    EGTS_READ(precord->record.OID,*ppbuf,*pbuf_sz)
  }
  if ( precord->record.EVFE != 0U )
  {
    EGTS_READ(precord->record.EVID,*ppbuf,*pbuf_sz)
  }
  if ( precord->record.TMFE != 0U )
  {
    EGTS_READ(precord->record.TM,*ppbuf,*pbuf_sz)
  }

  EGTS_READ(precord->record.SST,*ppbuf,*pbuf_sz)
  EGTS_READ(precord->record.RST,*ppbuf,*pbuf_sz)

  if ( precord->record.RL != 0U )
  {
    if ( precord->record.RL > *pbuf_sz )
    {
      egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid record RL");
      return -1;
    }
  }

  while ( *pbuf_sz != 0U )
  {

    if ( 0 != egts_proceed_rx_subrecord( estate , epacket , precord ,  ppbuf , pbuf_sz ) ) {
      return -1;
    }

    epacket->nsubrecords_used++;

  }

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_proceed_rx_subrecord()                                               */
/*                                                                           */
/* Description: Subrecord parser                                             */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            epacket - packet state mashine                                 */
/*            precord - record for contained subrecords                      */
/*            ppbuf - raw buffer to parse                                    */
/*            pbuf_sz - raw buffer length                                    */
/*                                                                           */
/* Return:    zero on OK, nonzero value otherwise                            */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int  egts_proceed_rx_subrecord(
  egts_state_t*         estate ,
  egts_packet_state_t*  epacket ,
  egts_record_t*        precord ,
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_subrecord_t*     psubrecord;
  const egts_subrecord_handler_t* chandler;

  if ( epacket->nsubrecords_used >= EGTS_MAX_PACKET_SUBRECORDS )
  {
    egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "too many subrecords");
    return -1;
  }

  if ( !precord->psubrecords )
  {
    precord->psubrecords = epacket->subrecords + epacket->nsubrecords_used;
    precord->nsubrecords = 0U;
  }
  psubrecord = precord->psubrecords + precord->nsubrecords;

  EGTS_READ(psubrecord->subrecord.SRT,*ppbuf,*pbuf_sz)
  EGTS_READ(psubrecord->subrecord.SRL,*ppbuf,*pbuf_sz)

  if ( psubrecord->subrecord.SRL > *pbuf_sz )
  {
    egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "too big subrecord SRL");
    return -1;
  }

  chandler = egts_find_subrecord_handler( precord->record.RST , psubrecord->subrecord.SRT );
  if ( !chandler )
  {
    egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "unsupported subrecord" );
    return -1;
  }

  psubrecord->SRD = egts_acquire_subrecord_buffer( epacket , chandler->subrecord_size );
  if ( !psubrecord->SRD ) {
    egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "insuffition memory" );
    return -1;
  }

  if ( 0 != chandler->fn_read( psubrecord->SRD , psubrecord->subrecord.SRL , ppbuf , pbuf_sz ) ) {
    egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid subrecord data" );
    return -1;
  }
  
  precord->nsubrecords++;

  return 0;

}


/*****************************************************************************/
/*                                                                           */
/* egts_proceed_rx_packet()                                                  */
/*                                                                           */
/* Description: Function for proceed received packet with raw frame data.    */
/*   Decomposes frame data to records/subrecords                             */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            epacket - packet state mashine                                 */
/*            pheader - packet transport header                              */
/*            pFD - packet frame data                                        */
/*            FDL - packet frame data length                                 */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int  egts_proceed_rx_packet( 
   egts_state_t*            estate ,
   egts_packet_state_t*     epacket ,
   egts_header_t*           pheader ,
   void* pFD ,
   u16   FDL )
{

  u8*  pbuf = (u8*)pFD;
  u16  buf_sz = FDL;
  egts_record_t* precord;

  epacket->have_responce   = (u8)0;
  epacket->have_signature  = (u8)0;

  if (epacket->header.PT == (u8)EGTS_PT_SIGNED_APPDATA )
  {
    /* parse and check signature */
    if ( FDL < (u16)sizeof(u16) )
    {
      egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid signed appdata length (1)" );
      return -1;
    }
    (void)egts_read_from_buf((u8*)&(epacket->signature_header.SIGL),(u16)sizeof(epacket->signature_header.SIGL),&pbuf,&buf_sz);
    if ( ( epacket->signature_header.SIGL == 0U ) ||
         ( epacket->signature_header.SIGL > EGTS_MAX_SIGN_LEN ) )
    {
      egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid signed appdata length (2)" );
      return -1;
    }
    if ( epacket->signature_header.SIGL >= buf_sz )
    {
      egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid signed appdata length (3)");
      return -1;
    }
    (void)egts_read_from_buf(epacket->signature_header.SIGD,epacket->signature_header.SIGL,&pbuf,&buf_sz);
    if ( buf_sz == 0U )
    {
      egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid signed appdata length (4)");
      return -1;
    }
    if ( 0 != egts_check_signup_data ( pbuf , buf_sz , &epacket->signature_header ) )
    {
      egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "sign-up check failed");
      return -1;
    }
    epacket->have_signature   = 1U;
  }
  else
    if ( epacket->header.PT == (u8)EGTS_PT_RESPONSE )
    {
      /* parse responce */
      if ( FDL < (u16)( sizeof(u16) + sizeof(u8) ) )
      {
        egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid responce length" );
        return -1;
      }
      (void)egts_read_from_buf((u8*)&(epacket->responce_header.RPID),(u16)sizeof(epacket->responce_header.RPID),&pbuf,&buf_sz);
      (void)egts_read_from_buf((u8*)&(epacket->responce_header.PR),(u16)sizeof(epacket->responce_header.PR),&pbuf,&buf_sz);
      epacket->have_responce   = (u8)1;
    }
    else
      if ( epacket->header.PT != (u8)EGTS_PT_APPDATA )
      {
        egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid packet type" );
        return -1;
      }

  /* */

  precord = epacket->records;
  epacket->nrecords = 0U;
  
  epacket->nsubrecords_used = 0U;

  while ( buf_sz != 0U )
  {
    precord->nsubrecords = 0U;
    precord->psubrecords = NULL;
    if ( 0 != egts_proceed_rx_record( estate , epacket , precord, &pbuf , &buf_sz ) ) {
      return -1;
    }
    precord++;
    epacket->nrecords++;
  }

  (void)estate->fn_rx_packet( estate->ctx , 
    pheader , 
    epacket->have_signature ,
    ( epacket->have_responce != (u8)0 ) ? &epacket->responce_header : (egts_responce_header_t*)NULL ,
    epacket->records ,
    epacket->nrecords ,
    epacket->SFRD_len );

  return 0;
}

/******************************************************************************
*
*/

