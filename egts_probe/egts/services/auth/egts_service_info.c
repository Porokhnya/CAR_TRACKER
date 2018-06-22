/*****************************************************************************/
/*                                                                           */
/* File: egts_service_info.c                                                 */
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
/*   EGTS_SR_SERVICE_INFO, service AUTH, implementations.                    */
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
/* egts_get_size_AUTH_SERVICE_INFO()                                         */
/*                                                                           */
/* Description: Returns size of specified AUTH_SERVICE_INFO subrecord        */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_SERVICE_INFO( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_SERVICE_INFO()                                             */
/*                                                                           */
/* Description: Read subrecord AUTH_SERVICE_INFO from buffer                 */
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
int   egts_read_AUTH_SERVICE_INFO( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_SERVICE_INFO()                                            */
/*                                                                           */
/* Description: Write subrecord AUTH_SERVICE_INFO to buffer                  */
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
int   egts_write_AUTH_SERVICE_INFO( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_SERVICE_INFO()                                        */
/*                                                                           */
/* Description: Member-wise compare AUTH_SERVICE_INFO subrecords             */
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
int  egts_is_equial_AUTH_SERVICE_INFO( void* ctx , void* pv_subrecord1, void* pv_subrecord2 );

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
void  egts_dump_AUTH_SERVICE_INFO( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/*****************************************************************************/
/*                                                                           */
/* egts_get_SERVICE_INFO_bitfields()                                         */
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
u8    egts_get_SERVICE_INFO_bitfields( egts_AUTH_SERVICE_INFO_t* ps );

/*****************************************************************************/
/*                                                                           */
/* egts_set_SERVICE_INFO_bitfields()                                         */
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
void  egts_set_SERVICE_INFO_bitfields( egts_AUTH_SERVICE_INFO_t* ps , u8 b );

/******************************************************************************
* implementation
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_SERVICE_INFO_bitfields()                                         */
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
u8    egts_get_SERVICE_INFO_bitfields( egts_AUTH_SERVICE_INFO_t* ps )
{
  u8 b = 0U;
  b |= ( ps->SRVA & 0x01U );    
  b <<= 7U;
  b |= ( ps->SRVRP & 0x03U );
  return b;   
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_SERVICE_INFO_bitfields()                                         */
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
void  egts_set_SERVICE_INFO_bitfields( egts_AUTH_SERVICE_INFO_t* ps , u8 b )
{
  ps->SRVRP = b & 0x03U;  
  b >>= 7U;
  ps->SRVA  = b & 0x01U; 
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_SERVICE_INFO()                                         */
/*                                                                           */
/* Description: Returns size of specified AUTH_SERVICE_INFO subrecord        */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_SERVICE_INFO( void* pv_subrecord )
{
  u16 s = 0U;

  (void)pv_subrecord;

  s += sizeof(u8) + sizeof(u8) + sizeof(u8);
  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_SERVICE_INFO()                                             */
/*                                                                           */
/* Description: Read subrecord AUTH_SERVICE_INFO from buffer                 */
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
int   egts_read_AUTH_SERVICE_INFO( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_AUTH_SERVICE_INFO_t* psrec = (egts_AUTH_SERVICE_INFO_t*)pv_subrecord;

  (void)SRL;

  EGTS_READ(psrec->ST,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->SST,*ppbuf,*pbuf_sz)
  if ( *pbuf_sz ) {
    egts_set_SERVICE_INFO_bitfields( psrec , **ppbuf );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }  
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_SERVICE_INFO()                                            */
/*                                                                           */
/* Description: Write subrecord AUTH_SERVICE_INFO to buffer                  */
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
int   egts_write_AUTH_SERVICE_INFO( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_AUTH_SERVICE_INFO_t* psrec = (egts_AUTH_SERVICE_INFO_t*)pv_subrecord;
  EGTS_WRITE(psrec->ST,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->SST,*ppbuf,*pbuf_sz)
  if ( *pbuf_sz )
  {
    **ppbuf = egts_get_SERVICE_INFO_bitfields( psrec );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }      
  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_SERVICE_INFO()                                        */
/*                                                                           */
/* Description: Member-wise compare AUTH_SERVICE_INFO subrecords             */
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
int  egts_is_equial_AUTH_SERVICE_INFO( void* ctx , void* pv_subrecord1, void* pv_subrecord2 )
{
  egts_AUTH_SERVICE_INFO_t* psrec1 = (egts_AUTH_SERVICE_INFO_t*)pv_subrecord1;
  egts_AUTH_SERVICE_INFO_t* psrec2 = (egts_AUTH_SERVICE_INFO_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->ST != psrec2->ST ) {
    egts_dbg_printf("inequieal ST");
    return -1;
  }

  if ( psrec1->SST != psrec2->SST ) {
    egts_dbg_printf("inequieal SST");
    return -1;
  }

  if ( psrec1->SRVA != psrec2->SRVA ) {
    egts_dbg_printf("inequieal SRVA");
    return -1;
  }

  if ( psrec1->SRVRP != psrec2->SRVRP ) {
    egts_dbg_printf("inequieal SRVRP");
    return -1;
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
void  egts_dump_AUTH_SERVICE_INFO( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_AUTH_SERVICE_INFO_t* psrec = (egts_AUTH_SERVICE_INFO_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_SERVICE_INFO:\n");
  (*afn_dump)( ctx , "  Service Type         ST: %u (%02Xh)\n" , psrec->ST, psrec->ST );
  (*afn_dump)( ctx , "  Service Statement    SST: %u (%s)\n" , psrec->SST, egts_SST_str(psrec->SST) );
  (*afn_dump)( ctx , "  Service Parameters:  SRVA= %u, SRVRP= %u\n" , psrec->SRVA, psrec->SRVRP );
}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_AUTH_SERVICE_INFO = 
{
  /* SRT */             (u8)EGTS_SR_SERVICE_INFO ,
  /* subrecord_size */  (u16)sizeof(egts_AUTH_SERVICE_INFO_t) ,
  /* fn_get_size */     egts_get_size_AUTH_SERVICE_INFO ,
  /* fn_read */         egts_read_AUTH_SERVICE_INFO ,
  /* fn_write */        egts_write_AUTH_SERVICE_INFO ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_AUTH_SERVICE_INFO ,
  /* fn_dump */         egts_dump_AUTH_SERVICE_INFO 
#endif
};


/******************************************************************************
*
*/
