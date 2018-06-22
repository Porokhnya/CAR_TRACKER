/*****************************************************************************/
/*                                                                           */
/* File: egts_auth_info.c                                                    */
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
/*   EGTS_SR_AUTH_INFO, service AUTH, implementations.                       */
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
/* egts_get_size_AUTH_AUTH_INFO()                                            */
/*                                                                           */
/* Description: Returns size of specified AUTH_AUTH_INFO subrecord           */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_AUTH_INFO( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_AUTH_INFO()                                                */
/*                                                                           */
/* Description: Read subrecord AUTH_AUTH_INFO from buffer                    */
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
int   egts_read_AUTH_AUTH_INFO( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_AUTH_INFO()                                               */
/*                                                                           */
/* Description: Write subrecord AUTH_AUTH_INFO to buffer                     */
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
int   egts_write_AUTH_AUTH_INFO( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_AUTH_INFO()                                           */
/*                                                                           */
/* Description: Member-wise compare AUTH_AUTH_INFO subrecords                */
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
int  egts_is_equial_AUTH_AUTH_INFO( void* ctx , void* pv_subrecord1 , void* pv_subrecord2  );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_AUTH_INFO()                                                */
/*                                                                           */
/* Description: Dump content of AUTH_AUTH_INFO subrecord                     */
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
void  egts_dump_AUTH_AUTH_INFO( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) );
    
#endif
    
/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_AUTH_INFO()                                            */
/*                                                                           */
/* Description: Returns size of specified AUTH_AUTH_INFO subrecord           */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_AUTH_INFO( void* pv_subrecord )
{
  egts_AUTH_AUTH_INFO_t* psrec = (egts_AUTH_AUTH_INFO_t*)pv_subrecord;
  u16 s = 0U;
  s += 
    psrec->UNM_len + 1U +
    psrec->UPSW_len + 1U +
    psrec->SS_len;
  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_AUTH_INFO()                                                */
/*                                                                           */
/* Description: Read subrecord AUTH_AUTH_INFO from buffer                    */
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
int   egts_read_AUTH_AUTH_INFO( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_AUTH_AUTH_INFO_t* psrec = (egts_AUTH_AUTH_INFO_t*)pv_subrecord;

  (void)SRL;

  EGTS_READ_Z(psrec->UNM,psrec->UNM_len,EGTS_UNM_LEN,*ppbuf,*pbuf_sz)
  EGTS_READ_Z(psrec->UPSW,psrec->UPSW_len,EGTS_UPSW_LEN,*ppbuf,*pbuf_sz)
  EGTS_READ_Z(psrec->SS,psrec->SS_len,EGTS_SS_LEN,*ppbuf,*pbuf_sz)

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_AUTH_INFO()                                               */
/*                                                                           */
/* Description: Write subrecord AUTH_AUTH_INFO to buffer                     */
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
int   egts_write_AUTH_AUTH_INFO( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{

  egts_AUTH_AUTH_INFO_t* psrec = (egts_AUTH_AUTH_INFO_t*)pv_subrecord;

  EGTS_WRITE_Z(psrec->UNM,psrec->UNM_len,*ppbuf,*pbuf_sz)
  EGTS_WRITE_Z(psrec->UPSW,psrec->UPSW_len,*ppbuf,*pbuf_sz)
  EGTS_WRITE_Z(psrec->SS,psrec->SS_len,*ppbuf,*pbuf_sz)

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_AUTH_INFO()                                           */
/*                                                                           */
/* Description: Member-wise compare AUTH_AUTH_INFO subrecords                */
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
int  egts_is_equial_AUTH_AUTH_INFO( void* ctx , void* pv_subrecord1 , void* pv_subrecord2  )
{
  egts_AUTH_AUTH_INFO_t* psrec1 = (egts_AUTH_AUTH_INFO_t*)pv_subrecord1;
  egts_AUTH_AUTH_INFO_t* psrec2 = (egts_AUTH_AUTH_INFO_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->UNM_len != psrec2->UNM_len ) {
    egts_dbg_printf("inequieal UNM_len length");
    return -1;
  }
  if ( psrec1->UNM_len != 0U ) {
    if ( 0 != memcmp( psrec1->UNM , psrec2->UNM , (size_t)psrec1->UNM_len ) ) {
      egts_dbg_printf("inequieal UNM");
      return -1;        
    }
  }
    
  if ( psrec1->UPSW_len != psrec2->UPSW_len ) {
    egts_dbg_printf("inequieal UPSW_len length");
    return -1;
  }
  if ( psrec1->UPSW_len != 0U ) {
    if ( 0 != memcmp( psrec1->UPSW , psrec2->UPSW , (size_t)psrec1->UPSW_len ) ) {
      egts_dbg_printf("inequieal UPSW");
      return -1;        
    }
  }
  
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

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_AUTH_INFO()                                                */
/*                                                                           */
/* Description: Dump content of AUTH_AUTH_INFO subrecord                     */
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
void  egts_dump_AUTH_AUTH_INFO( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_AUTH_AUTH_INFO_t* psrec = (egts_AUTH_AUTH_INFO_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_AUTH_INFO:\n");

  (*afn_dump)( ctx , "  User Name         UNM: size=%u \n" , psrec->UNM_len);
  estate_dump_string_z( psrec->UNM, EGTS_UNM_LEN , ctx, afn_dump );

  (*afn_dump)( ctx , "  User Password     UPSW: size=%u \n" , psrec->UPSW_len);
  estate_dump_string_z( psrec->UPSW, EGTS_UPSW_LEN , ctx, afn_dump );

  (*afn_dump)( ctx , "  Server Sequence     SS: size=%u \n" , psrec->SS_len);
  estate_dump_string_z( psrec->SS, EGTS_SS_LEN , ctx, afn_dump );
}
#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_AUTH_AUTH_INFO = 
{
  /* SRT */             (u8)EGTS_SR_AUTH_INFO ,
  /* subrecord_size */  (u16)sizeof(egts_AUTH_AUTH_INFO_t) ,
  /* fn_get_size */     egts_get_size_AUTH_AUTH_INFO ,
  /* fn_read */         egts_read_AUTH_AUTH_INFO ,
  /* fn_write */        egts_write_AUTH_AUTH_INFO ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_AUTH_AUTH_INFO ,
  /* fn_dump */         egts_dump_AUTH_AUTH_INFO 
#endif
};


/******************************************************************************
*
*/
