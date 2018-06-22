/*****************************************************************************/
/*                                                                           */
/* File: egts_result_code.c                                                  */
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
/*   EGTS_SR_RESULT_CODE, service AUTH, implementations.                     */
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

/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_RESULT_CODE()                                          */
/*                                                                           */
/* Description: Returns size of specified AUTH_RESULT_CODE subrecord         */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_RESULT_CODE( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_RESULT_CODE()                                              */
/*                                                                           */
/* Description: Read subrecord AUTH_RESULT_CODE from buffer                  */
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
int   egts_read_AUTH_RESULT_CODE( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_RESULT_CODE()                                             */
/*                                                                           */
/* Description: Write subrecord AUTH_RESULT_CODE to buffer                   */
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
int   egts_write_AUTH_RESULT_CODE( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_RESULT_CODE()                                         */
/*                                                                           */
/* Description: Member-wise compare AUTH_RESULT_CODE subrecords              */
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
int  egts_is_equial_AUTH_RESULT_CODE( void* ctx , void* pv_subrecord1 , void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_RESULT_CODE()                                              */
/*                                                                           */
/* Description: Dump content of AUTH_RESULT_CODE subrecord                   */
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
void  egts_dump_AUTH_RESULT_CODE( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/******************************************************************************
* implementation
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_RESULT_CODE()                                          */
/*                                                                           */
/* Description: Returns size of specified AUTH_RESULT_CODE subrecord         */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_RESULT_CODE( void* pv_subrecord )
{
  u16 s = 0U;

  (void)pv_subrecord;

  s += sizeof(u8);
  return s;
}


/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_RESULT_CODE()                                              */
/*                                                                           */
/* Description: Read subrecord AUTH_RESULT_CODE from buffer                  */
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
int   egts_read_AUTH_RESULT_CODE( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_AUTH_RESULT_CODE_t* psrec = (egts_AUTH_RESULT_CODE_t*)pv_subrecord;

  (void)SRL;

  EGTS_READ(psrec->RCD,*ppbuf,*pbuf_sz)
  return 0;
}


/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_RESULT_CODE()                                             */
/*                                                                           */
/* Description: Write subrecord AUTH_RESULT_CODE to buffer                   */
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
int   egts_write_AUTH_RESULT_CODE( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_AUTH_RESULT_CODE_t* psrec = (egts_AUTH_RESULT_CODE_t*)pv_subrecord;
  EGTS_WRITE(psrec->RCD,*ppbuf,*pbuf_sz)
  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_RESULT_CODE()                                         */
/*                                                                           */
/* Description: Member-wise compare AUTH_RESULT_CODE subrecords              */
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
int  egts_is_equial_AUTH_RESULT_CODE( void* ctx , void* pv_subrecord1 , void* pv_subrecord2 )
{
  egts_AUTH_RESULT_CODE_t* psrec1 = (egts_AUTH_RESULT_CODE_t*)pv_subrecord1;
  egts_AUTH_RESULT_CODE_t* psrec2 = (egts_AUTH_RESULT_CODE_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->RCD != psrec2->RCD ) {
    egts_dbg_printf("inequieal RCD");
    return -1;
  }

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_RESULT_CODE()                                              */
/*                                                                           */
/* Description: Dump content of AUTH_RESULT_CODE subrecord                   */
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
void  egts_dump_AUTH_RESULT_CODE( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_AUTH_RESULT_CODE_t* psrec = (egts_AUTH_RESULT_CODE_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_RESULT_CODE:\n");
  (*afn_dump)( ctx , "  ResultCode   RCD:%u (%02Xh)\n" , psrec->RCD, psrec->RCD );
}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_AUTH_RESULT_CODE = 
{
  /* SRT */             (u8)EGTS_SR_RESULT_CODE ,
  /* subrecord_size */  (u16)sizeof(egts_AUTH_RESULT_CODE_t) ,
  /* fn_get_size */     egts_get_size_AUTH_RESULT_CODE,
  /* fn_read */         egts_read_AUTH_RESULT_CODE ,
  /* fn_write */        egts_write_AUTH_RESULT_CODE ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_AUTH_RESULT_CODE ,
  /* fn_dump */         egts_dump_AUTH_RESULT_CODE 
#endif
};


/******************************************************************************
*
*/
