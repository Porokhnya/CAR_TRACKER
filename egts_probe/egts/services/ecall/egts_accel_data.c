/*****************************************************************************/
/*                                                                           */
/* File: egts_accel_data.c                                                   */
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
/*   EGTS_SR_ACCEL_DATA, service ECALL, implementaion                        */
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

/******************************************************************************
* prototypes
*/


/*****************************************************************************/
/*                                                                           */
/* egts_get_size_ECALL_ACCEL_DATA()                                          */
/*                                                                           */
/* Description: Returns size of specified ECALL_ACCEL_DATA subrecord         */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_ECALL_ACCEL_DATA( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_ECALL_ACCEL_DATA()                                              */
/*                                                                           */
/* Description: Read subrecord ECALL_ACCEL_DATA from buffer                  */
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
int   egts_read_ECALL_ACCEL_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_ECALL_ACCEL_DATA()                                             */
/*                                                                           */
/* Description: Write subrecord ECALL_ACCEL_DATA to buffer                   */
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
int   egts_write_ECALL_ACCEL_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_ECALL_ACCEL_DATA()                                         */
/*                                                                           */
/* Description: Member-wise compare ECALL_ACCEL_DATA subrecords              */
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
int  egts_is_equial_ECALL_ACCEL_DATA( void* ctx , void* pv_subrecord1 , void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_ECALL_ACCEL_DATA()                                              */
/*                                                                           */
/* Description: Dump content of ECALL_ACCEL_DATA subrecord                   */
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
void  egts_dump_ECALL_ACCEL_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) ); 
    
#endif
    
/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_ECALL_ACCEL_DATA()                                          */
/*                                                                           */
/* Description: Returns size of specified ECALL_ACCEL_DATA subrecord         */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_ECALL_ACCEL_DATA( void* pv_subrecord )
{
  egts_ECALL_ACCEL_DATA_t* psrec = (egts_ECALL_ACCEL_DATA_t*)pv_subrecord;
  u16 s = 0U;
  s += sizeof(u8) + /* SA */
       sizeof(u32);/* ATM */
  s += 8U * (u16)psrec->SA;
  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_ECALL_ACCEL_DATA()                                              */
/*                                                                           */
/* Description: Read subrecord ECALL_ACCEL_DATA from buffer                  */
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
int   egts_read_ECALL_ACCEL_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  int i;
  egts_ECALL_ACCEL_DATA_t* psrec = (egts_ECALL_ACCEL_DATA_t*)pv_subrecord;

  (void)SRL;

  EGTS_READ(psrec->SA,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->ATM,*ppbuf,*pbuf_sz)
  for ( i = 0; i < (int)psrec->SA; i++ )
  {
    EGTS_READ(psrec->ADS[i].RTM,*ppbuf,*pbuf_sz)
    EGTS_READ(psrec->ADS[i].XAAV,*ppbuf,*pbuf_sz)
    EGTS_READ(psrec->ADS[i].YAAV,*ppbuf,*pbuf_sz)
    EGTS_READ(psrec->ADS[i].ZAAV,*ppbuf,*pbuf_sz)
  }

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_ECALL_ACCEL_DATA()                                             */
/*                                                                           */
/* Description: Write subrecord ECALL_ACCEL_DATA to buffer                   */
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
int   egts_write_ECALL_ACCEL_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  int i;
  egts_ECALL_ACCEL_DATA_t* psrec = (egts_ECALL_ACCEL_DATA_t*)pv_subrecord;

  EGTS_WRITE(psrec->SA,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->ATM,*ppbuf,*pbuf_sz)
  for ( i = 0; i < (int)psrec->SA; i++ )
  {
    EGTS_WRITE(psrec->ADS[i].RTM,*ppbuf,*pbuf_sz)
    EGTS_WRITE(psrec->ADS[i].XAAV,*ppbuf,*pbuf_sz)
    EGTS_WRITE(psrec->ADS[i].YAAV,*ppbuf,*pbuf_sz)
    EGTS_WRITE(psrec->ADS[i].ZAAV,*ppbuf,*pbuf_sz)
  }

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG


/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_ECALL_ACCEL_DATA()                                         */
/*                                                                           */
/* Description: Member-wise compare ECALL_ACCEL_DATA subrecords              */
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
int  egts_is_equial_ECALL_ACCEL_DATA( void* ctx , void* pv_subrecord1 , void* pv_subrecord2 )
{
  int i;
  egts_ECALL_ACCEL_DATA_t* psrec1 = (egts_ECALL_ACCEL_DATA_t*)pv_subrecord1;
  egts_ECALL_ACCEL_DATA_t* psrec2 = (egts_ECALL_ACCEL_DATA_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->SA != psrec2->SA ) {
    egts_dbg_printf("inequieal SA");
    return -1;
  }
  
  if ( psrec1->ATM != psrec2->ATM ) {
    egts_dbg_printf("inequieal ATM");
    return -1;
  }

  for ( i = 0; i < (int)psrec1->SA; i++ )
  {
    if ( psrec1->ADS[i].RTM != psrec2->ADS[i].RTM ) {
      egts_dbg_printf("inequieal ADS[i].RTM");
      return -1;
    }
    if ( psrec1->ADS[i].XAAV != psrec2->ADS[i].XAAV ) {
      egts_dbg_printf("inequieal ADS[i].XAAV");
      return -1;
    }
    if ( psrec1->ADS[i].YAAV != psrec2->ADS[i].YAAV ) {
      egts_dbg_printf("inequieal ADS[i].YAAV");
      return -1;
    }
    if ( psrec1->ADS[i].ZAAV != psrec2->ADS[i].ZAAV ) {
      egts_dbg_printf("inequieal ADS[i].ZAAV");
      return -1;
    }    
  }
  
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_ECALL_ACCEL_DATA()                                              */
/*                                                                           */
/* Description: Dump content of ECALL_ACCEL_DATA subrecord                   */
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
void  egts_dump_ECALL_ACCEL_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  int i;
  u8  s,n,h,d,m;
  u32 y;

  egts_ECALL_ACCEL_DATA_t* psrec = (egts_ECALL_ACCEL_DATA_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_ACCEL_DATA:\n");
  (*afn_dump)( ctx , "  Structures Amount               SA: %u (%02Xh)\n" , psrec->SA, psrec->SA );
  egts_get_tm( psrec->ATM , &s, &n, &h, &d, &m, &y );
  (*afn_dump)( ctx , "  Absolute Time                   ATM: %02u.%02u.%04u %02u:%02u:%02u  (%u) (%08Xh)\n" , 
    d, m, y, h, n, s, 
    psrec->ATM, psrec->ATM );
  for ( i = 0; i < (int)psrec->SA; i++ )
  {
    (*afn_dump)( ctx , "  t:%u, X:%i, Y:%i, Z:%i\n" , 
      psrec->ADS[i].RTM, 
      psrec->ADS[i].XAAV, 
      psrec->ADS[i].YAAV, 
      psrec->ADS[i].ZAAV    );
  }

}


#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_ECALL_ACCEL_DATA = 
{
  /* SRT */             (u8)EGTS_SR_ACCEL_DATA ,
  /* subrecord_size */  (u16)sizeof(egts_ECALL_ACCEL_DATA_t) ,
  /* fn_get_size */     egts_get_size_ECALL_ACCEL_DATA ,
  /* fn_read */         egts_read_ECALL_ACCEL_DATA ,
  /* fn_write */        egts_write_ECALL_ACCEL_DATA ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_ECALL_ACCEL_DATA ,
  /* fn_dump */         egts_dump_ECALL_ACCEL_DATA 
#endif
};


/******************************************************************************
*
*/
