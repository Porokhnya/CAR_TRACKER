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
/*   EGTS_SR_TRACK_DATA, service ECALL, implementaion                        */
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
/* egts_get_size_ECALL_TRACK_DATA()                                          */
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
u16   egts_get_size_ECALL_TRACK_DATA( void* pv_subrecord );


/*****************************************************************************/
/*                                                                           */
/* egts_read_ECALL_TRACK_DATA()                                              */
/*                                                                           */
/* Description: Read subrecord ECALL_TRACK_DATA from buffer                  */
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
int   egts_read_ECALL_TRACK_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );


/*****************************************************************************/
/*                                                                           */
/* egts_write_ECALL_TRACK_DATA()                                             */
/*                                                                           */
/* Description: Write subrecord ECALL_TRACK_DATA to buffer                   */
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
int   egts_write_ECALL_TRACK_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );


#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_ECALL_TRACK_DATA()                                         */
/*                                                                           */
/* Description: Member-wise compare ECALL_TRACK_DATA subrecords              */
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
int  egts_is_equial_ECALL_TRACK_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 );


/*****************************************************************************/
/*                                                                           */
/* egts_dump_ECALL_TRACK_DATA()                                              */
/*                                                                           */
/* Description: Dump content of ECALL_TRACK_DATA subrecord                   */
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
void  egts_dump_ECALL_TRACK_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif


/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_ECALL_TRACK_DATA()                                          */
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
u16   egts_get_size_ECALL_TRACK_DATA( void* pv_subrecord )
{
  egts_ECALL_TRACK_DATA_t* psrec = (egts_ECALL_TRACK_DATA_t*)pv_subrecord;
  u16 i;
  u16 s = 0U;

  s += sizeof(u8) +  /* FM */
       sizeof(u32); /* ATM */
  for ( i = 0; i < psrec->SA; i++ )
  {
    if ( psrec->TDS[i].TNDE ) {
      s += 12;
    }
  }

  return s;
}


/*****************************************************************************/
/*                                                                           */
/* egts_read_ECALL_TRACK_DATA()                                              */
/*                                                                           */
/* Description: Read subrecord ECALL_TRACK_DATA from buffer                  */
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
int   egts_read_ECALL_TRACK_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{


  u16 i;
  u8  uc;
  egts_ECALL_TRACK_DATA_t* psrec = (egts_ECALL_TRACK_DATA_t*)pv_subrecord;

  (void)SRL;

  EGTS_READ(psrec->SA,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->ATM,*ppbuf,*pbuf_sz)

  for ( i = 0; i < psrec->SA; i++ )
  {
    if ( *pbuf_sz < 1 ) {
      return -1;
    }
    uc = **ppbuf;
    psrec->TDS[i].TNDE = ( uc & 0x80 ) ? 1 : 0;
    psrec->TDS[i].LOHS = ( uc & 0x40 ) ? 1 : 0;
    psrec->TDS[i].LAHS = ( uc & 0x20 ) ? 1 : 0;
    psrec->TDS[i].RTM  = uc & 0x1F;
    (*ppbuf)++;
    (*pbuf_sz)--;
    if ( psrec->TDS[i].TNDE )
    {
      if ( *pbuf_sz < 11 ) {
        return -1;
      }
      EGTS_READ(psrec->TDS[i].LAT,*ppbuf,*pbuf_sz)
      EGTS_READ(psrec->TDS[i].LONG,*ppbuf,*pbuf_sz)
      psrec->TDS[i].SPD = 0;
      psrec->TDS[i].DIR = 0;
      psrec->TDS[i].SPD |= **ppbuf;
      (*ppbuf)++;
      (*pbuf_sz)--;
      uc = **ppbuf;
      psrec->TDS[i].SPD |= ( ( uc & 0x7F ) << 8 );
      psrec->TDS[i].DIR |= ( ( uc & 0x80 ) << 1 );
      (*ppbuf)++;
      (*pbuf_sz)--;
      uc = **ppbuf;
      psrec->TDS[i].DIR |= uc;
      (*ppbuf)++;
      (*pbuf_sz)--;
    }
  }

  return 0;
}


/*****************************************************************************/
/*                                                                           */
/* egts_write_ECALL_TRACK_DATA()                                             */
/*                                                                           */
/* Description: Write subrecord ECALL_TRACK_DATA to buffer                   */
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
int   egts_write_ECALL_TRACK_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  u16 i;
  u8  uc;
  egts_ECALL_TRACK_DATA_t* psrec = (egts_ECALL_TRACK_DATA_t*)pv_subrecord;

  EGTS_WRITE(psrec->SA,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->ATM,*ppbuf,*pbuf_sz)

  for ( i = 0; i < psrec->SA; i++ )
  {
    if ( *pbuf_sz < 1 ) {
      return -1;
    }
    uc = ( psrec->TDS[i].RTM & 0x1F );
    if ( psrec->TDS[i].TNDE ) 
    {
      uc |= 0x80;
      if ( psrec->TDS[i].LOHS ) {
        uc |= 0x40;
      }
      if ( psrec->TDS[i].LAHS ) {
        uc |= 0x20;
      }
      *(*ppbuf)++ = uc;
      (*pbuf_sz)--;
      if ( psrec->TDS[i].TNDE )
      {
        if ( *pbuf_sz < 11 ) {
          return -1;
        }
        EGTS_WRITE(psrec->TDS[i].LAT,*ppbuf,*pbuf_sz)
        EGTS_WRITE(psrec->TDS[i].LONG,*ppbuf,*pbuf_sz)
        uc = (u8)( psrec->TDS[i].SPD & 0xFF );
        EGTS_WRITE(uc,*ppbuf,*pbuf_sz)
        uc = (u8)( ( psrec->TDS[i].SPD >> 8 ) & 0x7F );
        uc |= (u8)( ( psrec->TDS[i].DIR >> 1 ) & 0x80 );
        EGTS_WRITE(uc,*ppbuf,*pbuf_sz)
        uc = (u8)( psrec->TDS[i].DIR & 0xFF );
        EGTS_WRITE(uc,*ppbuf,*pbuf_sz)
      }
    }
    else
    {
      *(*ppbuf)++ = uc;
      (*pbuf_sz)--;
    }
  }

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_ECALL_TRACK_DATA()                                         */
/*                                                                           */
/* Description: Member-wise compare ECALL_TRACK_DATA subrecords              */
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
int  egts_is_equial_ECALL_TRACK_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 )
{
  u16 i;

  egts_ECALL_TRACK_DATA_t* psrec1 = (egts_ECALL_TRACK_DATA_t*)pv_subrecord1;
  egts_ECALL_TRACK_DATA_t* psrec2 = (egts_ECALL_TRACK_DATA_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->SA != psrec2->SA ) {
    egts_dbg_printf("inequieal FM");
    return -1;
  }

  if ( psrec1->ATM != psrec2->ATM ) {
    egts_dbg_printf("inequieal MSD_len");
    return -1;
  }

  for ( i = 0; i < psrec1->SA; i++ )
  {
   
    if ( psrec1->TDS[i].TNDE != psrec2->TDS[i].TNDE ) {
      egts_dbg_printf("inequieal TNDE i");
      return -1;
    }

    if ( psrec1->TDS[i].RTM != psrec2->TDS[i].RTM ) {
      egts_dbg_printf("inequieal RTM i");
      return -1;
    }   

    if ( psrec1->TDS[i].TNDE )
    {

      if ( psrec1->TDS[i].TNDE != psrec2->TDS[i].TNDE ) {
        egts_dbg_printf("inequieal TNDE i");
        return -1;
      }    

      if ( psrec1->TDS[i].LAHS != psrec2->TDS[i].LAHS ) {
        egts_dbg_printf("inequieal LAHS i");
        return -1;
      }   

      if ( psrec1->TDS[i].LAT != psrec2->TDS[i].LAT ) {
        egts_dbg_printf("inequieal LAT i");
        return -1;
      }   

      if ( psrec1->TDS[i].LONG != psrec2->TDS[i].LONG ) {
        egts_dbg_printf("inequieal LONG i");
        return -1;
      }   

      if ( psrec1->TDS[i].SPD != psrec2->TDS[i].SPD ) {
        egts_dbg_printf("inequieal SPD i");
        return -1;
      } 

      if ( psrec1->TDS[i].DIR != psrec2->TDS[i].DIR ) {
        egts_dbg_printf("inequieal DIR i");
        return -1;
      } 

    }

  }


  return 0;
}


/*****************************************************************************/
/*                                                                           */
/* egts_dump_ECALL_TRACK_DATA()                                              */
/*                                                                           */
/* Description: Dump content of ECALL_TRACK_DATA subrecord                   */
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
void  egts_dump_ECALL_TRACK_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{

  u16 i;
  u8 s,n,h,d,m;
  u32 y;
  egts_ECALL_TRACK_DATA_t* psrec = (egts_ECALL_TRACK_DATA_t*)pv_subrecord;

  (*afn_dump)( ctx , "SR_TRCAK_DATA:\n");
  (*afn_dump)( ctx , "  Structures Amount    SA: %u\n" , psrec->SA );
  egts_get_tm( psrec->ATM , &s, &n, &h, &d, &m, &y );
  (*afn_dump)( ctx , "  Absolute Time       ATM: %02u.%02u.%04u %02u:%02u:%02u (%u) (%08Xh)\n" ,
    d, m, y, h, n, s, 
    psrec->ATM, psrec->ATM );
  
  for ( i = 0; i < psrec->SA; i++ )
  {
    if ( psrec->TDS[i].TNDE )
    {
      (*afn_dump)( ctx , "  TDS[%u]: \n" , i+1 );
      (*afn_dump)( ctx , "     LOHS: %u\n" , psrec->TDS[i].LOHS );
      (*afn_dump)( ctx , "     LAHS: %u\n" , psrec->TDS[i].LAHS );
      (*afn_dump)( ctx , "      RTM: %u\n" , psrec->TDS[i].RTM );

      (*afn_dump)( ctx , "      LAT: %u (%08Xh)\n" , psrec->TDS[i].LAT , psrec->TDS[i].LAT );
      (*afn_dump)( ctx , "     LONG: %u (%08Xh)\n" , psrec->TDS[i].LONG , psrec->TDS[i].LONG );
      (*afn_dump)( ctx , "      SPD: %u\n" , psrec->TDS[i].SPD );
      (*afn_dump)( ctx , "      DIR: %u\n" , psrec->TDS[i].DIR );

    }
    else
    {
      (*afn_dump)( ctx , "TDS[%u]: -\n" , i+1 );
    }
  }

}


#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_ECALL_TRACK_DATA = 
{
  /* SRT */             (u8)EGTS_SR_TRACK_DATA ,
  /* subrecord_size */  (u16)sizeof(egts_ECALL_TRACK_DATA_t) ,
  /* fn_get_size */     egts_get_size_ECALL_TRACK_DATA ,
  /* fn_read */         egts_read_ECALL_TRACK_DATA ,
  /* fn_write */        egts_write_ECALL_TRACK_DATA ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_ECALL_TRACK_DATA ,
  /* fn_dump */         egts_dump_ECALL_TRACK_DATA 
#endif
};


/******************************************************************************
*
*/


