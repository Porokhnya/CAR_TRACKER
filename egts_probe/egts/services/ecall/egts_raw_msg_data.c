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
/*   EGTS_SR_RAW_MSD_DATA, service ECALL, implementaion                      */
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
#include "egts_ecall.h"

#ifdef EGTS_DBG
#include "../../transport/egts_dump.h"
#endif

/******************************************************************************
* prototypes
*/


/*****************************************************************************/
/*                                                                           */
/* egts_get_size_ECALL_RAW_MSD_DATA()                                        */
/*                                                                           */
/* Description: Returns size of specified ECALL_RAW_MSD_DATA subrecord       */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_ECALL_RAW_MSD_DATA( void* pv_subrecord );


/*****************************************************************************/
/*                                                                           */
/* egts_read_ECALL_RAW_MSD_DATA()                                            */
/*                                                                           */
/* Description: Read subrecord ECALL_RAW_MSD_DATA from buffer                */
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
int   egts_read_ECALL_RAW_MSD_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );


/*****************************************************************************/
/*                                                                           */
/* egts_write_ECALL_RAW_MSD_DATA()                                           */
/*                                                                           */
/* Description: Write subrecord ECALL_RAW_MSD_DATA to buffer                 */
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
int   egts_write_ECALL_RAW_MSD_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );


#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_ECALL_RAW_MSD_DATA()                                       */
/*                                                                           */
/* Description: Member-wise compare ECALL_RAW_MSD_DATA subrecords            */
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
int  egts_is_equial_ECALL_RAW_MSD_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 );


/*****************************************************************************/
/*                                                                           */
/* egts_dump_ECALL_RAW_MSD_DATA()                                            */
/*                                                                           */
/* Description: Dump content of ECALL_RAW_MSD_DATA subrecord                 */
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
void  egts_dump_ECALL_RAW_MSD_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif


/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_ECALL_RAW_MSD_DATA()                                        */
/*                                                                           */
/* Description: Returns size of specified ECALL_MSD_DATA subrecord           */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_ECALL_RAW_MSD_DATA( void* pv_subrecord )
{
  egts_ECALL_RAW_MSD_DATA_t* psrec = (egts_ECALL_RAW_MSD_DATA_t*)pv_subrecord;
  u16 s = 0U;
  s += sizeof(u8) + /* FM */
       psrec->MSD_len;

  return s;
}


/*****************************************************************************/
/*                                                                           */
/* egts_read_ECALL_RAW_MSD_DATA()                                            */
/*                                                                           */
/* Description: Read subrecord ECALL_RAW_MSD_DATA from buffer                */
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
int   egts_read_ECALL_RAW_MSD_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_ECALL_RAW_MSD_DATA_t* psrec = (egts_ECALL_RAW_MSD_DATA_t*)pv_subrecord;

  EGTS_READ(psrec->FM,*ppbuf,*pbuf_sz)

  if ( --SRL > EGTS_RAW_MDS_LEN ) {
    return -1;
  }

  psrec->MSD_len = SRL;
  EGTS_READ_RAW(psrec->MSD,SRL,*ppbuf,*pbuf_sz)

  return 0;
}


/*****************************************************************************/
/*                                                                           */
/* egts_write_ECALL_RAW_MSD_DATA()                                           */
/*                                                                           */
/* Description: Write subrecord ECALL_RAW_MSD_DATA to buffer                 */
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
int   egts_write_ECALL_RAW_MSD_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_ECALL_RAW_MSD_DATA_t* psrec = (egts_ECALL_RAW_MSD_DATA_t*)pv_subrecord;

  EGTS_WRITE(psrec->FM,*ppbuf,*pbuf_sz)
  EGTS_WRITE_RAW(psrec->MSD,psrec->MSD_len,*ppbuf,*pbuf_sz)

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_ECALL_RAW_MSD_DATA()                                       */
/*                                                                           */
/* Description: Member-wise compare ECALL_RAW_MSD_DATA subrecords            */
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
int  egts_is_equial_ECALL_RAW_MSD_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 )
{
  egts_ECALL_RAW_MSD_DATA_t* psrec1 = (egts_ECALL_RAW_MSD_DATA_t*)pv_subrecord1;
  egts_ECALL_RAW_MSD_DATA_t* psrec2 = (egts_ECALL_RAW_MSD_DATA_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->FM != psrec2->FM ) {
    egts_dbg_printf("inequieal FM");
    return -1;
  }

  if ( psrec1->MSD_len != psrec2->MSD_len ) {
    egts_dbg_printf("inequieal MSD_len");
    return -1;
  }

  if ( 0 != memcmp( psrec1->MSD , psrec2->MSD , psrec1->MSD_len) ) {
    egts_dbg_printf("inequieal MSD content");
    return -1;
  }

  return 0;
}


/*****************************************************************************/
/*                                                                           */
/* egts_dump_ECALL_RAW_MSD_DATA()                                            */
/*                                                                           */
/* Description: Dump content of ECALL_RAW_MSD_DATA subrecord                 */
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
void  egts_dump_ECALL_RAW_MSD_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  
  egts_ECALL_RAW_MSD_DATA_t* psrec = (egts_ECALL_RAW_MSD_DATA_t*)pv_subrecord;

  (*afn_dump)( ctx , "SR_RAW_MSD_DATA:\n");
  (*afn_dump)( ctx , "  Format             FM: %u (%02Xh)\n" , psrec->FM, psrec->FM );
  (*afn_dump)( ctx , "  Raw MSD           MSD: (%u)\n" , psrec->MSD_len );
  estate_dump_string( psrec->MSD, psrec->MSD_len, ctx, afn_dump );
  
}


#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_ECALL_RAW_MSD_DATA = 
{
  /* SRT */             (u8)EGTS_SR_RAW_MSD_DATA ,
  /* subrecord_size */  (u16)sizeof(egts_ECALL_RAW_MSD_DATA_t) ,
  /* fn_get_size */     egts_get_size_ECALL_RAW_MSD_DATA ,
  /* fn_read */         egts_read_ECALL_RAW_MSD_DATA ,
  /* fn_write */        egts_write_ECALL_RAW_MSD_DATA ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_ECALL_RAW_MSD_DATA ,
  /* fn_dump */         egts_dump_ECALL_RAW_MSD_DATA 
#endif
};


/******************************************************************************
*
*/


