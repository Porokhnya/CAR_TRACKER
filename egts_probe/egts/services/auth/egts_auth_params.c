/*****************************************************************************/
/*                                                                           */
/* File: egts_auth_params.c                                                  */
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
/*   EGTS_SR_AUTH_PARAMS, service AUTH, implementations.                     */
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
/* egts_get_size_AUTH_AUTH_PARAMS()                                          */
/*                                                                           */
/* Description: Returns size of specified AUTH_AUTH_PARAMS subrecord         */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_AUTH_PARAMS( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_AUTH_PARAMS()                                              */
/*                                                                           */
/* Description: Read subrecord AUTH_AUTH_PARAMS from buffer                  */
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
int   egts_read_AUTH_AUTH_PARAMS( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_AUTH_PARAMS()                                             */
/*                                                                           */
/* Description: Write subrecord AUTH_AUTH_PARAMS to buffer                   */
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
int   egts_write_AUTH_AUTH_PARAMS( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_AUTH_PARAMS()                                         */
/*                                                                           */
/* Description: Member-wise compare AUTH_AUTH_PARAMS subrecords              */
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
int  egts_is_equial_AUTH_AUTH_PARAMS( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_AUTH_PARAMS()                                              */
/*                                                                           */
/* Description: Dump content of AUTH_AUTH_PARAMS subrecord                   */
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
void  egts_dump_AUTH_AUTH_PARAMS( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/*****************************************************************************/
/*                                                                           */
/* egts_get_AUTH_PARAMS_bits()                                               */
/*                                                                           */
/* Description: Collect AUTH_AUTH_PARAMS subrecord bitfields                 */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static 
u8    egts_get_AUTH_PARAMS_bits( egts_AUTH_AUTH_PARAMS_t* ps );

/*****************************************************************************/
/*                                                                           */
/* egts_set_AUTH_PARAMS_bits()                                               */
/*                                                                           */
/* Description: Set AUTH_AUTH_PARAMS subrecord bitfields                     */
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
void  egts_set_AUTH_PARAMS_bits( egts_AUTH_AUTH_PARAMS_t* ps , u8 b );

/******************************************************************************
* implemantation
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_AUTH_PARAMS_bits()                                               */
/*                                                                           */
/* Description: Collect AUTH_AUTH_PARAMS subrecord bitfields                 */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static 
u8    egts_get_AUTH_PARAMS_bits( egts_AUTH_AUTH_PARAMS_t* ps )
{
  u8 b = 0U;
  b |= ( ps->EXE & 0x01U );    
  b <<= 1U;
  b |= ( ps->SSE & 0x01U );    
  b <<= 1U;
  b |= ( ps->MSE & 0x01U );    
  b <<= 1U;
  b |= ( ps->ISLE & 0x01U );   
  b <<= 1U;
  b |= ( ps->PKE & 0x01U );    
  b <<= 1U;
  b |= ( ps->ENA & 0x01U );
  return b; 
}


/*****************************************************************************/
/*                                                                           */
/* egts_set_AUTH_PARAMS_bits()                                               */
/*                                                                           */
/* Description: Set AUTH_AUTH_PARAMS subrecord bitfields                     */
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
void  egts_set_AUTH_PARAMS_bits( egts_AUTH_AUTH_PARAMS_t* ps , u8 b )
{
  ps->ENA   = b & 0x01U;   
  b >>= 1U;
  ps->PKE   = b & 0x01U;   
  b >>= 1U;
  ps->ISLE  = b & 0x01U;   
  b >>= 1U;  
  ps->MSE   = b & 0x01U;   
  b >>= 1U;  
  ps->SSE   = b & 0x01U;   
  b >>= 1U;  
  ps->EXE   = b & 0x01U;
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_AUTH_PARAMS()                                          */
/*                                                                           */
/* Description: Returns size of specified AUTH_AUTH_PARAMS subrecord         */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_AUTH_PARAMS( void* pv_subrecord )
{
  egts_AUTH_AUTH_PARAMS_t* psrec = (egts_AUTH_AUTH_PARAMS_t*)pv_subrecord;
  u16 s = 0U;
  s += sizeof(u8);

  if ( psrec->PKE != 0U ) {
    s += psrec->PKL;
  }
  if ( psrec->ISLE != 0U ) {
    s += sizeof(u16);
  }
  if ( psrec->MSE != 0U ) {
    s += sizeof(u16);
  }
  if ( psrec->SSE != 0U ) {
    s += psrec->SS_len + 1U;
  }
  if ( psrec->EXE != 0U ) {
    s += psrec->EXP_len + 1U;
  }

  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_AUTH_PARAMS()                                              */
/*                                                                           */
/* Description: Read subrecord AUTH_AUTH_PARAMS from buffer                  */
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
int   egts_read_AUTH_AUTH_PARAMS( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_AUTH_AUTH_PARAMS_t* psrec = (egts_AUTH_AUTH_PARAMS_t*)pv_subrecord;

  (void)SRL;

  if ( *pbuf_sz > 0U ) {
    egts_set_AUTH_PARAMS_bits( psrec , **ppbuf );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }


  if ( psrec->PKE != 0U )
  {
    EGTS_READ(psrec->PKL,*ppbuf,*pbuf_sz)
    if ( ( psrec->PKL == 0U ) ||
        ( psrec->PKL > EGTS_PBK_LEN ) ) {
      return -1;
    }
    EGTS_READ_RAW(psrec->PBK,psrec->PKL,*ppbuf,*pbuf_sz)
  }
  else {
    psrec->PKL = 0U;
  }

  if ( psrec->ISLE != 0U ) {
    EGTS_READ(psrec->ISL,*ppbuf,*pbuf_sz)
  }
  if ( psrec->MSE != 0U ) {
    EGTS_READ(psrec->MSZ,*ppbuf,*pbuf_sz)
  }
  if ( psrec->SSE != 0U ) {
    EGTS_READ_Z(psrec->SS,psrec->SS_len,EGTS_SS_LEN,*ppbuf,*pbuf_sz)
  }

  if ( psrec->EXE != 0U ) {
    EGTS_READ_Z(psrec->EXP,psrec->EXP_len,EGTS_EXP_LEN,*ppbuf,*pbuf_sz)
  }

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_AUTH_PARAMS()                                             */
/*                                                                           */
/* Description: Write subrecord AUTH_AUTH_PARAMS to buffer                   */
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
int   egts_write_AUTH_AUTH_PARAMS( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_AUTH_AUTH_PARAMS_t* psrec = (egts_AUTH_AUTH_PARAMS_t*)pv_subrecord;

  if ( *pbuf_sz > 0U ) {
    *(*ppbuf)++ = egts_get_AUTH_PARAMS_bits( psrec );
    (*pbuf_sz)--;
  } else {
    return -1;
  }

  if ( psrec->PKE != 0U )
  {
    if ( ( psrec->PKL == 0U ) ||
        ( psrec->PKL > EGTS_PBK_LEN ) ) {
      return -1;
    }
    EGTS_WRITE(psrec->PKL,*ppbuf,*pbuf_sz)
    EGTS_WRITE_RAW(psrec->PBK,psrec->PKL,*ppbuf,*pbuf_sz)
  }

  if ( psrec->ISLE != 0U ) {
    EGTS_WRITE(psrec->ISL,*ppbuf,*pbuf_sz)
  }

  if ( psrec->MSE != 0U ) {
    EGTS_WRITE(psrec->MSZ,*ppbuf,*pbuf_sz)
  }

  if ( psrec->SSE != 0U ) {
    EGTS_WRITE_Z(psrec->SS,psrec->SS_len,*ppbuf,*pbuf_sz)
  }

  if ( psrec->EXE != 0U )  {
    EGTS_WRITE_Z(psrec->EXP,psrec->EXP_len,*ppbuf,*pbuf_sz)
  }

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_AUTH_PARAMS()                                         */
/*                                                                           */
/* Description: Member-wise compare AUTH_AUTH_PARAMS subrecords              */
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
int  egts_is_equial_AUTH_AUTH_PARAMS( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 )
{
  egts_AUTH_AUTH_PARAMS_t* psrec1 = (egts_AUTH_AUTH_PARAMS_t*)pv_subrecord1;
  egts_AUTH_AUTH_PARAMS_t* psrec2 = (egts_AUTH_AUTH_PARAMS_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->EXE != psrec2->EXE ) {
    egts_dbg_printf("inequieal EXE");
    return -1;
  }
  
  if ( psrec1->SSE != psrec2->SSE ) {
    egts_dbg_printf("inequieal SSE");
    return -1;
  }
  
  if ( psrec1->MSE != psrec2->MSE ) {
    egts_dbg_printf("inequieal MSE");
    return -1;
  }

  if ( psrec1->ISLE != psrec2->ISLE ) {
    egts_dbg_printf("inequieal ISLE");
    return -1;
  }

  if ( psrec1->PKE != psrec2->PKE ) {
    egts_dbg_printf("inequieal PKE");
    return -1;
  }
  
  if ( psrec1->ENA != psrec2->ENA ) {
    egts_dbg_printf("inequieal ENA");
    return -1;
  }
    
  if ( psrec1->PKE != 0U ) {
    if ( psrec1->PKL != psrec2->PKL ) {
      egts_dbg_printf("inequieal PKL length");
      return -1;
    }
    if ( psrec1->PKL != 0U ) {
      if ( 0 != memcmp( psrec1->PBK , psrec2->PBK , (size_t)psrec1->PKL ) ) {
        egts_dbg_printf("inequieal PKL");
        return -1;        
      }
    }
  }
           
  if ( psrec1->ISLE != 0U ) {
    if ( psrec1->ISL != psrec2->ISL ) {
      egts_dbg_printf("inequieal ISL");
      return -1;
    }
  }
           
  if ( psrec1->MSE != 0U ) {
    if ( psrec1->MSZ != psrec2->MSZ ) {
      egts_dbg_printf("inequieal MSZ");
      return -1;
    }    
  }
           
  if ( psrec1->SSE != 0U ) {
    if ( psrec1->SS_len != psrec2->SS_len ) {
      egts_dbg_printf("inequieal SS_len length");
      return -1;
    }
    if ( psrec1->SS_len != 0U ) {
      if ( 0 != memcmp( psrec1->SS , psrec2->SS , (size_t)psrec1->SS_len ) ) {
        egts_dbg_printf("inequieal SS");
        return -1;        
      }
    }
  }
           
  if ( psrec1->EXE != 0U ) {
    if ( psrec1->EXP_len != psrec2->EXP_len ) {
      egts_dbg_printf("inequieal EXP_len length");
      return -1;
    }
    if ( psrec1->EXP_len != 0U ) {
      if ( 0 != memcmp( psrec1->EXP , psrec2->EXP , (size_t)psrec1->EXP_len ) ) {
        egts_dbg_printf("inequieal EXP");
        return -1;        
      }
    }    
  }
  
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_AUTH_PARAMS()                                              */
/*                                                                           */
/* Description: Dump content of AUTH_AUTH_PARAMS subrecord                   */
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
void  egts_dump_AUTH_AUTH_PARAMS( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_AUTH_AUTH_PARAMS_t* psrec = (egts_AUTH_AUTH_PARAMS_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_AUTH_PARAMS:\n");
  
  (*afn_dump)( ctx , "  FLAGS: EXE= %u, SSE= %u, MSE= %u, ISLE= %u, PKE= %u, ENA= %u\n" ,
     psrec->EXE, psrec->SSE, psrec->MSE, psrec->ISLE, psrec->PKE, psrec->ENA );

  if( psrec->PKE != (u8)0 ) {
    (*afn_dump)( ctx , "  Public Key Length      PKL: %u (%04Xh)\n" , psrec->PKL , psrec->PKL );
  }  else {
    (*afn_dump)( ctx , "  Public Key Length      PKL: --- \n" );
  }

  if( psrec->PKE != (u8)0 )
  {
    (*afn_dump)( ctx , "  Public Key             PBK:\n" );
    estate_dump_string( psrec->PBK, (u32)psrec->PKL, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  Public Key             PBK: --- \n" );
  }

  (*afn_dump)( ctx , "  Identity String Length ISL: %u (%04Xh)\n" , psrec->ISL , psrec->ISL );

  if( psrec->MSE != (u8)0 ) {
    (*afn_dump)( ctx , "  Mod Size               MSZ: %u (%04Xh)\n" , psrec->MSZ , psrec->MSZ );
  }  else {
    (*afn_dump)( ctx , "  Mod Size               MSZ: --- \n" );
  }

  if( psrec->SSE != (u8)0 )  {
    (*afn_dump)( ctx , "  Server Sequence        SS:\n" );
    estate_dump_string_z( psrec->SS, EGTS_SS_LEN, ctx, afn_dump );
  } else {
    (*afn_dump)( ctx , "  Server Sequence        SS: --- \n" );
  }

  if( psrec->EXE != (u8)0 )  {
    (*afn_dump)( ctx , "  Exp                    EXP:\n" );
    estate_dump_string_z( psrec->EXP, EGTS_EXP_LEN, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  Exp                    EXP: --- \n" );
  }
}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_AUTH_AUTH_PARAMS = 
{
  /* SRT */             (u8)EGTS_SR_AUTH_PARAMS ,
  /* subrecord_size */  (u16)sizeof(egts_AUTH_AUTH_PARAMS_t) ,
  /* fn_get_size */     egts_get_size_AUTH_AUTH_PARAMS ,
  /* fn_read */         egts_read_AUTH_AUTH_PARAMS ,
  /* fn_write */        egts_write_AUTH_AUTH_PARAMS ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_AUTH_AUTH_PARAMS ,
  /* fn_dump */         egts_dump_AUTH_AUTH_PARAMS
#endif
};


/******************************************************************************
*
*/
