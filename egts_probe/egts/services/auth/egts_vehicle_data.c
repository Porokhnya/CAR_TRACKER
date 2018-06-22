/*****************************************************************************/
/*                                                                           */
/* File: egts_vehicle_data.c                                                 */
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
/*   EGTS_SR_VEHICLE_DATA, service AUTH, implementations.                    */
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
/* egts_get_size_AUTH_VEHICLE_DATA()                                         */
/*                                                                           */
/* Description: Returns size of specified AUTH_VEHICLE_DATA subrecord        */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_VEHICLE_DATA( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_VEHICLE_DATA()                                             */
/*                                                                           */
/* Description: Read subrecord AUTH_VEHICLE_DATA from buffer                 */
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
int   egts_read_AUTH_VEHICLE_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_VEHICLE_DATA()                                            */
/*                                                                           */
/* Description: Write subrecord AUTH_VEHICLE_DATA to buffer                  */
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
int   egts_write_AUTH_VEHICLE_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG


/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_VEHICLE_DATA()                                        */
/*                                                                           */
/* Description: Member-wise compare AUTH_VEHICLE_DATA subrecords             */
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
int  egts_is_equial_AUTH_VEHICLE_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_VEHICLE_DATA()                                             */
/*                                                                           */
/* Description: Dump content of AUTH_VEHICLE_DATA subrecord                  */
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
void  egts_dump_AUTH_VEHICLE_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/******************************************************************************
* implementation
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_VEHICLE_DATA()                                         */
/*                                                                           */
/* Description: Returns size of specified AUTH_VEHICLE_DATA subrecord        */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_VEHICLE_DATA( void* pv_subrecord )
{
  u16 s = 0U;

  (void)pv_subrecord;

  s += EGTS_VIN_LEN + sizeof(u32) + sizeof(u32);
  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_VEHICLE_DATA()                                             */
/*                                                                           */
/* Description: Read subrecord AUTH_VEHICLE_DATA from buffer                 */
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
int   egts_read_AUTH_VEHICLE_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_AUTH_VEHICLE_DATA_t* psrec = (egts_AUTH_VEHICLE_DATA_t*)pv_subrecord;

  (void)SRL;

  memset(psrec->VIN,0,EGTS_VIN_LEN);
  EGTS_READ_RAW(psrec->VIN,EGTS_VIN_LEN,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->VHT,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->VPST,*ppbuf,*pbuf_sz)

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_VEHICLE_DATA()                                            */
/*                                                                           */
/* Description: Write subrecord AUTH_VEHICLE_DATA to buffer                  */
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
int   egts_write_AUTH_VEHICLE_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_AUTH_VEHICLE_DATA_t* psrec = (egts_AUTH_VEHICLE_DATA_t*)pv_subrecord;

  EGTS_WRITE_RAW(psrec->VIN,EGTS_VIN_LEN,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->VHT,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->VPST,*ppbuf,*pbuf_sz)
  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_VEHICLE_DATA()                                        */
/*                                                                           */
/* Description: Member-wise compare AUTH_VEHICLE_DATA subrecords             */
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
int  egts_is_equial_AUTH_VEHICLE_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 )
{
  egts_AUTH_VEHICLE_DATA_t* psrec1 = (egts_AUTH_VEHICLE_DATA_t*)pv_subrecord1;
  egts_AUTH_VEHICLE_DATA_t* psrec2 = (egts_AUTH_VEHICLE_DATA_t*)pv_subrecord2;

  (void)ctx;

  if ( 0 != memcmp( psrec1->VIN , psrec2->VIN , EGTS_VIN_LEN ) ) {
    egts_dbg_printf("inequieal VIN");
    return -1;        
  }     
    
  if ( psrec1->VHT != psrec2->VHT ) {
    egts_dbg_printf("inequieal VHT");
    return -1;
  }

  if ( psrec1->VPST != psrec2->VPST ) {
    egts_dbg_printf("inequieal VPST");
    return -1;
  }
  
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_VEHICLE_DATA()                                             */
/*                                                                           */
/* Description: Dump content of AUTH_VEHICLE_DATA subrecord                  */
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
void  egts_dump_AUTH_VEHICLE_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_AUTH_VEHICLE_DATA_t* psrec = (egts_AUTH_VEHICLE_DATA_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_VEHICLE_DATA:\n");
  
  (*afn_dump)( ctx , "  Vehicle Identification Number   VIN:\n" );
  estate_dump_string( psrec->VIN, EGTS_VIN_LEN, ctx, afn_dump );

  (*afn_dump)( ctx , "  Vehicle Type                    VHT: %u (%08Xh)\n" , psrec->VHT, psrec->VHT );
  (*afn_dump)( ctx , "  Vehicle Propulsion Storage     VPST: %u (%08Xh)\n" , psrec->VPST, psrec->VPST );
}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_AUTH_VEHICLE_DATA = 
{
  /* SRT */             (u8)EGTS_SR_VEHICLE_DATA ,
  /* subrecord_size */  (u16)sizeof(egts_AUTH_VEHICLE_DATA_t) ,
  /* fn_get_size */     egts_get_size_AUTH_VEHICLE_DATA ,
  /* fn_read */         egts_read_AUTH_VEHICLE_DATA ,
  /* fn_write */        egts_write_AUTH_VEHICLE_DATA ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_AUTH_VEHICLE_DATA ,
  /* fn_dump */         egts_dump_AUTH_VEHICLE_DATA 
#endif
};


/******************************************************************************
*
*/
