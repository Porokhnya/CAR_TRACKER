/*****************************************************************************/
/*                                                                           */
/* File: egts_tx.c                                                           */
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
/* Description: Main EGTS packet composer.                                   */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: public : egts_tx_packet.                                       */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"

#include "../include/egts.h"
#include "../include/egts_impl.h"
#include "egts_guts.h"

#include "egts_crc.h"
#include "egts_header_encode.h"
#include "egts_encode.h"
#include "egts_compress.h"
#include "egts_signup.h"
#include "../services/egts_services.h"

/*****************************************************************************/
/*                                                                           */
/* egts_tx_byte()                                                            */
/*                                                                           */
/* Description: Internal wrapper for transmit                                */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            pbuf - buffer to transmit                                      */
/*            sz - buffer size                                               */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_tx_buffer( egts_state_t* estate , void* pbuf , u32 sz )
{
  return estate->fn_tx_buffer( estate->ctx , pbuf , sz );
}

/*****************************************************************************/
/*                                                                           */
/* egts_tx_byte()                                                            */
/*                                                                           */
/* Description: Internal wrapper for transmit                                */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            uc - byte to transmit                                          */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_tx_byte( egts_state_t* estate , u8 uc )
{
  return egts_tx_buffer( estate , &uc , 1U  );
}

/*****************************************************************************/
/*                                                                           */
/* egts_tx_raw_packet()                                                      */
/*                                                                           */
/* Description: Internal function for transmit packet.                       */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            pprofile - transmit settings                                   */
/*            PR - packet priority                                           */
/*            proute - packet routing settings                               */
/*            PID - packet identifier                                        */
/*            presponce - optional response data                             */
/*            pFD - packet frame data                                        */
/*            FDL - packet frame data length                                 */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_tx_raw_packet( 
  egts_state_t*           estate ,
  egts_profile_t*         pprofile ,
  u8                      PR , 
  egts_route_t*           proute ,
  u16                     PID, 
  egts_responce_header_t* presponce ,
  void*                   pFD,
  u16                     FDL
  )
{
  int res;
  u16 sz;
  u8* puc;

  if ( FDL > EGTS_MAX_SFDR_LEN )
  {
    egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "frame data too long" );
    return 1;
  }

  if ( PR > (u8)EGTS_PRIORITY_MAX )
  {
    egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid priority, corrected" );
    PR = (u8)EGTS_PRIORITY_MAX;
  }

  estate->cur_tx_packet->header.PRV  =  (u8)EGTS_VERSION;

  estate->cur_tx_packet->header.SKID = pprofile->SKID;

  estate->cur_tx_packet->header.PRF  = (u8)EGTS_PRF;
  estate->cur_tx_packet->header.RTE  = ( proute ) ? (u8)1 : (u8)0;
  estate->cur_tx_packet->header.ENA  = pprofile->ENA;
  estate->cur_tx_packet->header.CMP  = pprofile->CMP;
  estate->cur_tx_packet->header.PR   = PR;

  estate->cur_tx_packet->header.HL   = (u8)EGTS_HEADER_LEN_FIXED;
  estate->cur_tx_packet->header.HE   = pprofile->HE;

  /* FDL setup below*/

  estate->cur_tx_packet->header.PID  = PID;
  if ( pprofile->SGN != (u8)0 )                     
  {
    if ( presponce )
    {
      egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "responce + sign-up conflict" );
      return -1;
    }
    estate->cur_tx_packet->header.PT = (u8)EGTS_PT_SIGNED_APPDATA;  
  }
  else
  {
    if ( presponce ) {
      estate->cur_tx_packet->header.PT = (u8)EGTS_PT_RESPONSE;
    } else {
      estate->cur_tx_packet->header.PT = (u8)EGTS_PT_APPDATA;
    }
  }

  if ( proute )
  {
    estate->cur_tx_packet->header.PRA = proute->PRA;
    estate->cur_tx_packet->header.RCA = proute->RCA;
    estate->cur_tx_packet->header.TTL = proute->TTL;
  }

  /* sign-up data */
  if ( pprofile->SGN != (u8)0 )
  {
    if ( 0 != ( res = egts_signup_data( pFD, FDL , &estate->cur_tx_packet->signature_header ) ) )
    {
      egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "data sign-up failure" );
      return res;
    }
    if ( ( estate->cur_tx_packet->signature_header.SIGL == 0U ) ||
         ( estate->cur_tx_packet->signature_header.SIGL > EGTS_MAX_SIGN_LEN ) )
    {
      egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "data sign-up invalid length" );
      return res;
    }
    puc = estate->cur_tx_packet->SFRD_encoded;
    sz = EGTS_SFDR_BUF_SIZE;
    (void)egts_write_to_buf( (u8*)&estate->cur_tx_packet->signature_header.SIGL, 
      (u16)sizeof(estate->cur_tx_packet->signature_header.SIGL), &puc, &sz );
    (void)egts_write_to_buf( estate->cur_tx_packet->signature_header.SIGD, 
      estate->cur_tx_packet->signature_header.SIGL, &puc, &sz );
    if ( FDL > sz )
    {
      egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "data sign-up invalid length (1)" );
      return res;
    }
    /*@-mayaliasunique@*/
    memcpy( puc , pFD , (size_t)FDL );
    /*@=mayaliasunique@*/
    sz -= FDL;
    estate->cur_tx_packet->SFRD_elen = (u16)( EGTS_SFDR_BUF_SIZE - sz );
  }
  else
  {
    if ( presponce )
    {
      puc = estate->cur_tx_packet->SFRD_encoded;
      sz = EGTS_SFDR_BUF_SIZE;
      (void)egts_write_to_buf( (u8*)&presponce->RPID,(u16)sizeof(presponce->RPID), &puc, &sz );
      (void)egts_write_to_buf( (u8*)&presponce->PR,(u16)sizeof(presponce->PR), &puc, &sz );
      if ( FDL > sz )
      {
        egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "data + responce invalid length" );
        return -1;
      }
      /*@-mayaliasunique@*/
      memcpy( puc , pFD , (size_t)FDL );
      /*@=mayaliasunique@*/
      sz -= FDL;
      estate->cur_tx_packet->SFRD_elen = (u16)( EGTS_SFDR_BUF_SIZE - sz );
    }
    else
    {
      /*@-mayaliasunique@*/
      memcpy( estate->cur_tx_packet->SFRD_encoded , pFD , (size_t)FDL );
      /*@=mayaliasunique@*/
      estate->cur_tx_packet->SFRD_elen = FDL;
    }
  }

  /* compress data */
  if( pprofile->CMP != (u8)0 )
  {
    if ( 0 != ( res = egts_compress_data( estate->cur_tx_packet->SFRD , estate->cur_tx_packet->SFRD_encoded , 
      estate->cur_tx_packet->SFRD_elen , &estate->cur_tx_packet->SFRD_len ) ) )
    {
      egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "data compressing failure" );
      return res;
    }
  }
  else
  {
    /* TODO: */
    memcpy( estate->cur_tx_packet->SFRD , estate->cur_tx_packet->SFRD_encoded , (size_t)estate->cur_tx_packet->SFRD_elen );
    estate->cur_tx_packet->SFRD_len = estate->cur_tx_packet->SFRD_elen;
  }

  /* data in estate->cur_tx_packet->SFRD */

  if ( pprofile->ENA != (u8)EGTS_DATA_ENA_NONE )
  {
    /* encode data */
    if ( 0 != ( res = egts_encode_data( pprofile->SKID , estate->cur_tx_packet->SFRD_encoded , 
      estate->cur_tx_packet->SFRD , estate->cur_tx_packet->SFRD_len , &estate->cur_tx_packet->SFRD_elen ) ) )
    {
      egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "data encoding failure");
      return res;
    }
    estate->cur_tx_packet->SFRD_buf = estate->cur_tx_packet->SFRD_encoded;
    estate->cur_tx_packet->header.FDL = estate->cur_tx_packet->SFRD_elen;
  }
  else
  {
    estate->cur_tx_packet->SFRD_buf = estate->cur_tx_packet->SFRD;
    estate->cur_tx_packet->header.FDL = estate->cur_tx_packet->SFRD_len;
  }


  if ( 0 != egts_header_encode( &estate->cur_tx_packet->header , estate->cur_tx_packet->header_encoded , &estate->cur_tx_packet->header_encoded_len ) )
  {
    egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "header encoding failure");
    return -1;
  }
  estate->cur_tx_packet->header.HL += estate->cur_tx_packet->header_encoded_len;
  /*@-type@*/
  estate->cur_tx_packet->header.HL++; /* crc */
  /*@=type@*/


  estate->cur_tx_packet->header.HCS = egts_header_crc( &estate->cur_tx_packet->header , 
    estate->cur_tx_packet->header_encoded , estate->cur_tx_packet->header_encoded_len );
  
  estate->cur_tx_packet->SFRCS = egts_data_crc( 
    estate->cur_tx_packet->SFRD_buf , 
    estate->cur_tx_packet->header.FDL );

  /* TODO: assemble + tx, check result */

  (void)egts_tx_byte( estate , estate->cur_tx_packet->header.PRV );
  (void)egts_tx_byte( estate , estate->cur_tx_packet->header.SKID );
  (void)egts_tx_byte( estate , egts_get_header_bits( &estate->cur_tx_packet->header ) );
  (void)egts_tx_byte( estate , estate->cur_tx_packet->header.HL );
  (void)egts_tx_byte( estate , estate->cur_tx_packet->header.HE );
  (void)egts_tx_buffer( estate , estate->cur_tx_packet->header_encoded , (u32)estate->cur_tx_packet->header_encoded_len );
  (void)egts_tx_byte( estate , estate->cur_tx_packet->header.HCS );

  (void)egts_tx_buffer( estate , estate->cur_tx_packet->SFRD_buf , (u32)estate->cur_tx_packet->header.FDL );
  /* TODO: ENDIAN */
  (void)egts_tx_buffer( estate , &estate->cur_tx_packet->SFRCS , 2U );
  
  return 0;

}

/*****************************************************************************/
/*                                                                           */
/* egts_tx_packet()                                                          */
/*                                                                           */
/* Description: Function for transmit packet.                                */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            pprofile - transmit settings                                   */
/*            PR - packet priority                                           */
/*            proute - packet routing settings                               */
/*            PID - packet identifier                                        */
/*            presponce - optional response data                             */
/*            precords - array of recors to transmit                         */
/*            nrecords - record count                                        */
/*            ptemp_buf - temporary buffer 64KB for operation                */
/*            temp_buf_sz - temporary buffer size                            */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_tx_packet( egts_state_t* estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  egts_record_t*  precords ,
  u16             nrecords ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  )
{
  u16 sz = temp_buf_sz;
  u16 tsz, sub_sz , subs_sz;
  u16 irecord, isubrecord;

  egts_record_t*       precord;
  egts_subrecord_t*    psubrecord;
  u8* puc = (u8*)ptemp_buf;

  const egts_subrecord_handler_t* chandler = NULL;

  precord = precords;
  for ( irecord = 0U; ( precord != NULL ) && ( irecord < nrecords ); irecord++ )
  {

    sub_sz = 0U;
    subs_sz = 0U;
    psubrecord = precord->psubrecords;
    for ( isubrecord = 0U; 
          ( psubrecord != NULL ) && ( isubrecord < precord->nsubrecords ); 
          isubrecord++ )
    {

      chandler = egts_find_subrecord_handler( precord->record.RST , psubrecord->subrecord.SRT );
      if ( !chandler ) {
        egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "subrecord not supported");
        return -1;
      }
      if ( !psubrecord->SRD ) {
        egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "no subrecord data");
        return -1;
      }
      if ( (u16)-1 == ( sub_sz = chandler->fn_get_size( psubrecord->SRD ) ) ) {
        egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "subrecord size error");
        return -1;
      }

      psubrecord->subrecord.SRL = sub_sz;

      subs_sz += sub_sz + 
        sizeof(u8) + /* SRT */ 
        sizeof(u16) /* SRL */ 
        ;

      psubrecord++;
      
    }
    precord->record.RL = subs_sz;


    tsz = sizeof( precord->record.RL) + sizeof( precord->record.RN);
    if ( precord->record.OBFE != 0U ) {
      tsz += sizeof( precord->record.OID);
    }
    if ( precord->record.EVFE != 0U ) {
      tsz += sizeof( precord->record.EVID);
    }
    if ( precord->record.TMFE != 0U ) {
      tsz += sizeof( precord->record.TM);
    }
    tsz += sizeof( precord->record.SST) + sizeof( precord->record.RST); 
    /* TODO: const for mandatory part */


    if ( tsz + precord->record.RL > sz )
    {
      egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "tx temp buffer full");
      return -1;
    }

    EGTS_WRITE(precord->record.RL,puc,sz)
    EGTS_WRITE(precord->record.RN,puc,sz)
    if ( sz > 0U ) {
      *puc++ = egts_get_record_bits( &precord->record );
      sz--;
    } else {
      egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid record bits");
      return -1;
    }

    if (precord->record.OBFE != 0U ) {
      EGTS_WRITE(precord->record.OID,puc,sz)
    }
    if (precord->record.EVFE != 0U ) {
      EGTS_WRITE(precord->record.EVID,puc,sz)
    }
    if (precord->record.TMFE != 0U ) {
      EGTS_WRITE(precord->record.TM,puc,sz)
    }
    EGTS_WRITE(precord->record.SST,puc,sz)
    EGTS_WRITE(precord->record.RST,puc,sz)

    psubrecord = precord->psubrecords;
    for ( isubrecord = 0U; 
          ( psubrecord != NULL ) && ( isubrecord < precord->nsubrecords ); 
          isubrecord++ )
    {
      chandler = egts_find_subrecord_handler( precord->record.RST , psubrecord->subrecord.SRT );
      if ( !chandler ) {
        egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "subrecord not supported");
        return -1;
      }
      if ( !psubrecord->SRD ) {
        egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "no subrecord data");
        return -1;
      }

      EGTS_WRITE(psubrecord->subrecord.SRT,puc,sz)
      EGTS_WRITE(psubrecord->subrecord.SRL,puc,sz)

      if ( -1 == chandler->fn_write(psubrecord->SRD , &puc , &sz ) ) {
        egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "subrecord write failure");
        return -1;
      }

      psubrecord++;
    }

    precord++;
  }

  if ( 0 != egts_tx_raw_packet( estate , pprofile , PR , proute , PID, presponce,
      ptemp_buf , temp_buf_sz - sz ) )
  {
    egts_tx_error( estate , 0U , EGTS_PC_UNKNOWN , "packet write failure");
    return -1;
  }

  return 0;
}


/******************************************************************************
*
*/
