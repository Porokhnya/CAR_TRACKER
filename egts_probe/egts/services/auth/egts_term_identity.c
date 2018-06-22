/*****************************************************************************/
/*                                                                           */
/* File: egts_term_identity.c                                                */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service AUTH                                              */
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
/* Description: Parser/Composer/Debuger entries for packet                   */
/*   EGTS_SR_TERM_IDENTITY, service AUTH, implementations.                   */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#include "../../include/egts_config.h"

#include "../../include/egts.h"
#include "../../include/egts_impl.h"
#include "../../transport/egts_guts.h"

#include "../egts_services.h"
#include "../egts_service_handler.h"
#include "egts_auth.h"

#ifdef EGTS_DBG
#include "../../transport/egts_dump.h"
#endif

/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_TERM_IDENTITY()                                        */
/*                                                                           */
/* Description: Returns size of specified AUTH_TERM_IDENTITY subrecord       */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_TERM_IDENTITY( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_TERM_IDENTITY()                                            */
/*                                                                           */
/* Description: Read subrecord AUTH_TERM_IDENTITY from buffer                */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*            SRL - subrecord data length                                    */
/*            ppbuf - pointer to buffer begin                                */
/*            pbuf_sz - size of buffer                                       */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
int   egts_read_AUTH_TERM_IDENTITY( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_TERM_IDENTITY()                                           */
/*                                                                           */
/* Description: Write subrecord AUTH_TERM_IDENTITY to buffer                 */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*            ppbuf - pointer to buffer begin                                */
/*            pbuf_sz - size of buffer                                       */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
int   egts_write_AUTH_TERM_IDENTITY( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_TERM_IDENTITY()                                       */
/*                                                                           */
/* Description: Member-wise compare AUTH_TERM_IDENTITY subrecords            */
/*                                                                           */
/* Arguments: ctx - dump callback context                                    */
/*            pv_subrecord1 - firts subrecord to test                        */
/*            pv_subrecord2 - second subrecord to test                       */
/*                                                                           */
/* Return:    0 if subrecords are equial, nonzero valu otherwise             */
/*                                                                           */
/* Other:     debug use only                                                 */
/*                                                                           */
/*****************************************************************************/

static
int  egts_is_equial_AUTH_TERM_IDENTITY( void* ctx , void* pv_subrecord1,  void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_SERVICE_INFO()                                             */
/*                                                                           */
/* Description: Dump content of AUTH_SERVICE_INFO subrecord                  */
/*                                                                           */
/* Arguments: ctx - dump callback context                                    */
/*            pv_subrecord - subrecord to dump                               */
/*            afn_dump - callback                                            */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     debug use only                                                 */
/*                                                                           */
/*****************************************************************************/

static
void  egts_dump_AUTH_TERM_IDENTITY( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/*****************************************************************************/
/*                                                                           */
/* egts_get_TERM_IDENTITY_bitfields()                                        */
/*                                                                           */
/* Description: Collect AUTH_SERVICE_INFO subrecord bitfields                */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_TERM_IDENTITY_bitfields( egts_AUTH_TERM_IDENTITY_t* ps );

/*****************************************************************************/
/*                                                                           */
/* egts_set_TERM_IDENTITY_bitfields()                                        */
/*                                                                           */
/* Description: Set AUTH_SERVICE_INFO subrecord bitfields                    */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*            b - bitfields byte                                             */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
void  egts_set_TERM_IDENTITY_bitfields( egts_AUTH_TERM_IDENTITY_t* ps , u8 b );

/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_TERM_IDENTITY_bitfields()                                        */
/*                                                                           */
/* Description: Collect AUTH_SERVICE_INFO subrecord bitfields                */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static      
u8    egts_get_TERM_IDENTITY_bitfields( egts_AUTH_TERM_IDENTITY_t* ps )
{
  u8 b = 0U;
  b |= ( ps->MNE & 0x01U );      
  b <<= 1U;
  b |= ( ps->BSE & 0x01U );      
  b <<= 1U;
  b |= ( ps->NIDE & 0x01U );     
  b <<= 1U;
  b |= ( ps->SSRA & 0x01U );     
  b <<= 1U;
  b |= ( ps->LNGCE & 0x01U );    
  b <<= 1U;
  b |= ( ps->IMSIE & 0x01U );    
  b <<= 1U;
  b |= ( ps->IMEIE & 0x01U );    
  b <<= 1U;
  b |= ( ps->HDIDE & 0x01U );
  return b;    
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_TERM_IDENTITY_bitfields()                                        */
/*                                                                           */
/* Description: Set AUTH_SERVICE_INFO subrecord bitfields                    */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*            b - bitfields byte                                             */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
void  egts_set_TERM_IDENTITY_bitfields( egts_AUTH_TERM_IDENTITY_t* ps , u8 b )
{
  ps->HDIDE = b & 0x01U;  
  b >>= 1U;
  ps->IMEIE = b & 0x01U;  
  b >>= 1U;
  ps->IMSIE = b & 0x01U;  
  b >>= 1U;
  ps->LNGCE = b & 0x01U;  
  b >>= 1U;
  ps->SSRA = b & 0x01U;   
  b >>= 1U;
  ps->NIDE = b & 0x01U;   
  b >>= 1U;
  ps->BSE = b & 0x01U;    
  b >>= 1U;
  ps->MNE = b & 0x01U;    
}


/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_TERM_IDENTITY()                                        */
/*                                                                           */
/* Description: Returns size of specified AUTH_TERM_IDENTITY subrecord       */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_TERM_IDENTITY( void* pv_subrecord )
{
  egts_AUTH_TERM_IDENTITY_t* psrec = (egts_AUTH_TERM_IDENTITY_t*)pv_subrecord;
  u16 s = 0U;
  s += sizeof(u32) + sizeof(u8);
  if ( psrec->HDIDE != 0U ) {
    s += sizeof(u16);
  }
  if ( psrec->IMEIE != 0U ) {
    s += EGTS_IMEI_LEN;
  }
  if ( psrec->IMSIE != 0U ) {
    s += EGTS_IMSI_LEN;
  }
  if ( psrec->LNGCE != 0U ) {
    s += EGTS_LNGC_LEN;
  }
  if ( psrec->NIDE != 0U ) {
    s += EGTS_NID_LEN;
  }
  if ( psrec->BSE != 0U ) {
    s += sizeof(u16);
  }
  if ( psrec->MNE != 0U ) {
    s += EGTS_MSISDN_LEN;
  }
  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_TERM_IDENTITY()                                            */
/*                                                                           */
/* Description: Read subrecord AUTH_TERM_IDENTITY from buffer                */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*            SRL - subrecord data length                                    */
/*            ppbuf - pointer to buffer begin                                */
/*            pbuf_sz - size of buffer                                       */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
int   egts_read_AUTH_TERM_IDENTITY( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_AUTH_TERM_IDENTITY_t* psrec = (egts_AUTH_TERM_IDENTITY_t*)pv_subrecord;

  (void)SRL;

  EGTS_READ(psrec->TID,*ppbuf,*pbuf_sz)
    
  if ( *pbuf_sz ) {
    egts_set_TERM_IDENTITY_bitfields( psrec , **ppbuf );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }      
    
  if ( psrec->HDIDE != 0U ) {
    EGTS_READ(psrec->HDID,*ppbuf,*pbuf_sz)
  }
  if ( psrec->IMEIE != 0U ) {
    EGTS_READ_RAW(psrec->IMEI,EGTS_IMEI_LEN,*ppbuf,*pbuf_sz)
  }
  if ( psrec->IMSIE != 0U ) {
    EGTS_READ_RAW(psrec->IMSI,EGTS_IMSI_LEN,*ppbuf,*pbuf_sz)
  }
  if ( psrec->LNGCE != 0U ) {
    EGTS_READ_RAW(psrec->LNGC,EGTS_LNGC_LEN,*ppbuf,*pbuf_sz)
  }
  if ( psrec->NIDE != 0U ) {
    EGTS_READ_RAW(psrec->NID,EGTS_NID_LEN,*ppbuf,*pbuf_sz)
  }
  if ( psrec->BSE != 0U ) {
    EGTS_READ(psrec->BS,*ppbuf,*pbuf_sz)
  }
  if ( psrec->MNE != 0U ) {
    EGTS_READ_RAW(psrec->MSISDN,EGTS_MSISDN_LEN,*ppbuf,*pbuf_sz)
  }
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_TERM_IDENTITY()                                           */
/*                                                                           */
/* Description: Write subrecord AUTH_TERM_IDENTITY to buffer                 */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*            ppbuf - pointer to buffer begin                                */
/*            pbuf_sz - size of buffer                                       */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
int   egts_write_AUTH_TERM_IDENTITY( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_AUTH_TERM_IDENTITY_t* psrec = (egts_AUTH_TERM_IDENTITY_t*)pv_subrecord;
  EGTS_WRITE(psrec->TID,*ppbuf,*pbuf_sz)
  if ( *pbuf_sz )
  {
    **ppbuf = egts_get_TERM_IDENTITY_bitfields( psrec );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }        
    
  if ( psrec->HDIDE != 0U ) {
    EGTS_WRITE(psrec->HDID,*ppbuf,*pbuf_sz)
  }
  if ( psrec->IMEIE != 0U ) {
    EGTS_WRITE_RAW(psrec->IMEI,EGTS_IMEI_LEN,*ppbuf,*pbuf_sz)
  }
  if ( psrec->IMSIE != 0U ) {
    EGTS_WRITE_RAW(psrec->IMSI,EGTS_IMSI_LEN,*ppbuf,*pbuf_sz)
  }
  if ( psrec->LNGCE != 0U ) {
    EGTS_WRITE_RAW(psrec->LNGC,EGTS_LNGC_LEN,*ppbuf,*pbuf_sz)
  }
  if ( psrec->NIDE != 0U ) {
    EGTS_WRITE_RAW(psrec->NID,EGTS_NID_LEN,*ppbuf,*pbuf_sz)
  }
  if ( psrec->BSE != 0U ) {
    EGTS_WRITE(psrec->BS,*ppbuf,*pbuf_sz)
  }
  if ( psrec->MNE != 0U ) {
    EGTS_WRITE_RAW(psrec->MSISDN,EGTS_MSISDN_LEN,*ppbuf,*pbuf_sz)
  }
  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_TERM_IDENTITY()                                       */
/*                                                                           */
/* Description: Member-wise compare AUTH_TERM_IDENTITY subrecords            */
/*                                                                           */
/* Arguments: ctx - dump callback context                                    */
/*            pv_subrecord1 - firts subrecord to test                        */
/*            pv_subrecord2 - second subrecord to test                       */
/*                                                                           */
/* Return:    0 if subrecords are equial, nonzero valu otherwise             */
/*                                                                           */
/* Other:     debug use only                                                 */
/*                                                                           */
/*****************************************************************************/

static
int  egts_is_equial_AUTH_TERM_IDENTITY( void* ctx , void* pv_subrecord1,  void* pv_subrecord2 )
{
  egts_AUTH_TERM_IDENTITY_t* psrec1 = (egts_AUTH_TERM_IDENTITY_t*)pv_subrecord1;
  egts_AUTH_TERM_IDENTITY_t* psrec2 = (egts_AUTH_TERM_IDENTITY_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->TID != psrec2->TID ) {
    egts_dbg_printf("inequieal TID");
    return -1;
  }

  if ( psrec1->MNE != psrec2->MNE ) {
    egts_dbg_printf("inequieal MNE");
    return -1;
  }

  if ( psrec1->BSE != psrec2->BSE ) {
    egts_dbg_printf("inequieal BSE");
    return -1;
  }

  if ( psrec1->NIDE != psrec2->NIDE ) {
    egts_dbg_printf("inequieal NIDE");
    return -1;
  }

  if ( psrec1->SSRA != psrec2->SSRA ) {
    egts_dbg_printf("inequieal SSRA");
    return -1;
  }

  if ( psrec1->LNGCE != psrec2->LNGCE ) {
    egts_dbg_printf("inequieal LNGCE");
    return -1;
  }

  if ( psrec1->IMSIE != psrec2->IMSIE ) {
    egts_dbg_printf("inequieal IMSIE");
    return -1;
  }  

  if ( psrec1->IMEIE != psrec2->IMEIE ) {
    egts_dbg_printf("inequieal IMEIE");
    return -1;
  }  
  
  if ( psrec1->HDIDE != psrec2->HDIDE ) {
    egts_dbg_printf("inequieal HDIDE");
    return -1;
  }  
    
    
  if ( psrec1->HDIDE != 0U ) {
    if ( psrec1->HDID != psrec2->HDID ) {
      egts_dbg_printf("inequieal HDID");
      return -1;
    }     
  }
  
  if ( psrec1->IMEIE != 0U ) {
    if ( 0 != memcmp( psrec1->IMEI , psrec2->IMEI , EGTS_IMEI_LEN ) ) {
      egts_dbg_printf("inequieal IMEI");
      return -1;        
    }
  }
  
  if ( psrec1->IMSIE != 0U ) {
    if ( 0 != memcmp( psrec1->IMSI , psrec2->IMSI , EGTS_IMSI_LEN ) ) {
      egts_dbg_printf("inequieal IMSI");
      return -1;        
    }    
  }
  
  if ( psrec1->LNGCE != 0U ) {
    if ( 0 != memcmp( psrec1->LNGC , psrec2->LNGC , EGTS_LNGC_LEN ) ) {
      egts_dbg_printf("inequieal LNGC");
      return -1;        
    }      
  }
  if ( psrec1->NIDE != 0U ) {
    if ( 0 != memcmp( psrec1->NID , psrec2->NID , EGTS_NID_LEN ) ) {
      egts_dbg_printf("inequieal NID");
      return -1;        
    }     
  }
  
  if ( psrec1->BSE != 0U ) {
    if ( psrec1->BS != psrec2->BS ) {
      egts_dbg_printf("inequieal BS");
      return -1;
    }        
  }
  
  if ( psrec1->MNE != 0U ) {
    if ( 0 != memcmp( psrec1->MSISDN , psrec2->MSISDN , EGTS_MSISDN_LEN ) ) {
      egts_dbg_printf("inequieal MSISDN");
      return -1;        
    }         
  }
  
  return 0;
}


/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_SERVICE_INFO()                                             */
/*                                                                           */
/* Description: Dump content of AUTH_SERVICE_INFO subrecord                  */
/*                                                                           */
/* Arguments: ctx - dump callback context                                    */
/*            pv_subrecord - subrecord to dump                               */
/*            afn_dump - callback                                            */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     debug use only                                                 */
/*                                                                           */
/*****************************************************************************/

static
void  egts_dump_AUTH_TERM_IDENTITY( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_AUTH_TERM_IDENTITY_t* psrec = (egts_AUTH_TERM_IDENTITY_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_TERM_IDENTITY:\n");
  (*afn_dump)( ctx , "  TerminalIdentifier        TID: %u (%08Xh)\n" , psrec->TID , psrec->TID );
  (*afn_dump)( ctx , "  FLAGS: MNE= %u, BSE= %u, NIDE= %u, SSRA= %u, LNGCE= %u, IMSIE= %u, IMEIE= %u, HDIDE= %u\n" ,
     psrec->MNE, psrec->BSE, psrec->NIDE, psrec->SSRA, psrec->LNGCE, psrec->IMSIE, psrec->IMEIE, psrec->HDIDE );

  if( psrec->HDIDE != 0U ) {
    (*afn_dump)( ctx , "  Home Dispatcher Identifier   HDID: %u (%04Xh)\n" , psrec->HDID , psrec->HDID );
  }  else {
    (*afn_dump)( ctx , "  Home Dispatcher Identifier   HDID: --- \n" );
  }

  if( psrec->IMEIE != 0U ) {
    (*afn_dump)( ctx , "  International Mobile Equipment Identity   IMEI:\n" );
    estate_dump_string( psrec->IMEI, EGTS_IMEI_LEN, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  International Mobile Equipment Identity   IMEI: --- \n" );
  }
  
  if( psrec->IMSIE != 0U )  {
    (*afn_dump)( ctx , "  International Mobile Subscriber Identity   IMSI:\n" );
    estate_dump_string( psrec->IMSI, EGTS_IMSI_LEN, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  International Mobile Subscriber Identity   IMSI: --- \n" );
  }

  if( psrec->LNGCE != 0U ) {
    (*afn_dump)( ctx , "  Language Code   LNGC:\n" );
    estate_dump_string( psrec->LNGC, EGTS_LNGC_LEN, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  Language Code   LNGC: --- \n" );
  }

  if( psrec->NIDE != 0U ) {
    (*afn_dump)( ctx , "  Network Identifier   NID:\n" );
    estate_dump_string( psrec->NID, EGTS_NID_LEN, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  Network Identifier   NID: --- \n" );
  }

  if( psrec->BSE != 0U ) {
    (*afn_dump)( ctx , "  Buffer Size   BS: %u (%04Xh)\n", psrec->BS, psrec->BS );
  }  else {
    (*afn_dump)( ctx , "  Buffer Size   BS: --- \n" );
  }

  if( psrec->MNE != 0U ) {
    (*afn_dump)( ctx , "  Mobile Station Integrated Services Digital Network Number   MSISDN:\n" );
    estate_dump_string( psrec->MSISDN, EGTS_MSISDN_LEN, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  Mobile Station Integrated Services Digital Network Number   MSISDN: --- \n" );
  }
}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_AUTH_TERM_IDENTITY = 
{
  /* SRT */             (u8)EGTS_SR_TERM_IDENTITY ,
  /* subrecord_size */  (u16)sizeof(egts_AUTH_TERM_IDENTITY_t) ,
  /* fn_get_size */     egts_get_size_AUTH_TERM_IDENTITY ,
  /* fn_read */         egts_read_AUTH_TERM_IDENTITY ,
  /* fn_write */        egts_write_AUTH_TERM_IDENTITY ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_AUTH_TERM_IDENTITY ,
  /* fn_dump */         egts_dump_AUTH_TERM_IDENTITY 
#endif
};


/******************************************************************************
*
*/
