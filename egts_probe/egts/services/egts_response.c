/*****************************************************************************/
/*                                                                           */
/* File: egts_msd_data.c                                                     */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service ECALL                                             */
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
/*   subrecord RESPONCE, any service, implementaion                          */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"
#include "../include/egts.h"
#include "../include/egts_impl.h"
#include "../transport/egts_guts.h"

#include "egts_services.h"
#include "egts_response.h"

#ifdef EGTS_DBG
#include "../transport/egts_dump.h"
#endif

/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_RESPONSE()                                                  */
/*                                                                           */
/* Description: Returns size of specified RESPONSE subrecord                 */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_RESPONSE( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_RESPONSE()                                                      */
/*                                                                           */
/* Description: Read subrecord RESPONSE from buffer                          */
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
int   egts_read_RESPONSE( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_RESPONSE()                                                     */
/*                                                                           */
/* Description: Write subrecord RESPONSE to buffer                           */
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
int   egts_write_RESPONSE( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_RESPONSE()                                                 */
/*                                                                           */
/* Description: Member-wise compare RESPONSE subrecords                      */
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
int  egts_is_equial_RESPONSE( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_RESPONSE()                                                      */
/*                                                                           */
/* Description: Dump content of RESPONSE subrecord                           */
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
void  egts_dump_RESPONSE( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_RESPONSE()                                                  */
/*                                                                           */
/* Description: Returns size of specified RESPONSE subrecord                 */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_RESPONSE( void* pv_subrecord )
{
  u16 s;

  (void)pv_subrecord;
 
  s = 
    (u16)sizeof(u16) +
    (u16)sizeof(u8);
  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_RESPONSE()                                                      */
/*                                                                           */
/* Description: Read subrecord RESPONSE from buffer                          */
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
int   egts_read_RESPONSE( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_RESPONSE_t* psrec = (egts_RESPONSE_t*)pv_subrecord;

  (void)SRL;

  EGTS_READ(psrec->CRN,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->RST,*ppbuf,*pbuf_sz)

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_RESPONSE()                                                     */
/*                                                                           */
/* Description: Write subrecord RESPONSE to buffer                           */
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
int   egts_write_RESPONSE( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{

  egts_RESPONSE_t* psrec = (egts_RESPONSE_t*)pv_subrecord;

  EGTS_WRITE(psrec->CRN,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->RST,*ppbuf,*pbuf_sz)

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_RESPONSE()                                                 */
/*                                                                           */
/* Description: Member-wise compare RESPONSE subrecords                      */
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
int  egts_is_equial_RESPONSE( void* ctx , void* pv_subrecord1 , void* pv_subrecord2  )
{
  egts_RESPONSE_t* psrec1 = (egts_RESPONSE_t*)pv_subrecord1;
  egts_RESPONSE_t* psrec2 = (egts_RESPONSE_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->CRN != psrec2->CRN ) {
    egts_dbg_printf("inequieal CRN length");
    return -1;
  }
  if ( psrec1->RST != psrec2->RST ) {
    egts_dbg_printf("inequieal CRN length");
    return -1;
  }

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_RESPONSE()                                                      */
/*                                                                           */
/* Description: Dump content of RESPONSE subrecord                           */
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
void  egts_dump_RESPONSE( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_RESPONSE_t* psrec = (egts_RESPONSE_t*)pv_subrecord;
  (*afn_dump)( ctx , "RESPONSE:\n");

  (*afn_dump)( ctx , "  Confirmed Record Number     CRN: %u (%04x)\n" , psrec->CRN, psrec->CRN);
  (*afn_dump)( ctx , "  Record Status               RST: %u (%04x) %s\n" , 
    psrec->RST, psrec->RST , egts_result_str(psrec->RST) );

}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_RESPONSE = 
{
  /* SRT */             (u8)EGTS_SR_RESPONSE ,
  /* subrecord_size */  (u16)sizeof(egts_RESPONSE_t) ,
  /* fn_get_size */     egts_get_size_RESPONSE ,
  /* fn_read */         egts_read_RESPONSE ,
  /* fn_write */        egts_write_RESPONSE ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_RESPONSE,
  /* fn_dump */         egts_dump_RESPONSE
#endif
};


/******************************************************************************
*
*/


