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
/*   EGTS_SR_MSD_DATA, service ECALL, implementaion                          */
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
/* egts_get_size_ECALL_MSD_DATA()                                            */
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
u16   egts_get_size_ECALL_MSD_DATA( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_ECALL_MSD_DATA()                                                */
/*                                                                           */
/* Description: Read subrecord ECALL_MSD_DATA from buffer                    */
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
int   egts_read_ECALL_MSD_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_ECALL_MSD_DATA()                                               */
/*                                                                           */
/* Description: Write subrecord ECALL_MSD_DATA to buffer                     */
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
int   egts_write_ECALL_MSD_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_ECALL_MSD_DATA()                                           */
/*                                                                           */
/* Description: Member-wise compare ECALL_MSD_DATA subrecords                */
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
int  egts_is_equial_ECALL_MSD_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_ECALL_MSD_DATA()                                                */
/*                                                                           */
/* Description: Dump content of ECALL_MSD_DATA subrecord                     */
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
void  egts_dump_ECALL_MSD_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/*****************************************************************************/
/*                                                                           */
/* egts_get_MSD_bitfields()                                                  */
/*                                                                           */
/* Description: Collect ECALL_MSD_DATA subrecord bitfields                   */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_MSD_bitfields( egts_ECALL_MSD_DATA_t* ps );

/*****************************************************************************/
/*                                                                           */
/* egts_set_MSD_bitfields()                                                  */
/*                                                                           */
/* Description: Set COMMAND_DATA subrecord bitfields                         */
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
void  egts_set_MSD_bitfields( egts_ECALL_MSD_DATA_t* ps , u8 b );

/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_MSD_bitfields()                                                  */
/*                                                                           */
/* Description: Collect ECALL_MSD_DATA subrecord bitfields                   */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_MSD_bitfields( egts_ECALL_MSD_DATA_t* ps )
{
  u8 b = 0U;
  b |= ( ps->VT & 0x0FU );    
  b <<= 1U;
  b |= ( ps->POCN & 0x01U );
  b <<= 1U;
  b |= ( ps->CLT & 0x01U );   
  b <<= 1U;
  b |= ( ps->ACT & 0x01U ); 
  return b;   
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_MSD_bitfields()                                                  */
/*                                                                           */
/* Description: Set COMMAND_DATA subrecord bitfields                         */
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
void  egts_set_MSD_bitfields( egts_ECALL_MSD_DATA_t* ps , u8 b )
{
  ps->ACT  = b & 0x01U;   
  b >>= 1U;
  ps->CLT  = b & 0x01U;   
  b >>= 1U;
  ps->POCN = b & 0x01U;   
  b >>= 1U;
  ps->VT   = b & 0x0FU;
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_ECALL_MSD_DATA()                                            */
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
u16   egts_get_size_ECALL_MSD_DATA( void* pv_subrecord )
{
  egts_ECALL_MSD_DATA_t* psrec = (egts_ECALL_MSD_DATA_t*)pv_subrecord;
  u16 s = 0U;
  s += sizeof(u8) + /* FV */
       sizeof(u8) + /* MI */
       sizeof(u8) + /* CN */
       EGTS_VIN_LEN + /* VIN */
       sizeof(u8) + /* VPST */
       sizeof(u32) + /* TS */
       sizeof(u32) + /* PLAT */
       sizeof(u32) + /* PLON */
       sizeof(u8); /* VD */


  if ( ( psrec->pvr_latd1 != 0U ) ||
       ( psrec->pvr_lond1 != 0U ) ||
       ( psrec->pvr_latd2 != 0U ) ||
       ( psrec->pvr_lond2 != 0U ) ||
       ( psrec->nop != 0U ) ||
       ( psrec->ad != 0U ) )
  {
    s += sizeof(i16);
  }
  
  if ( ( psrec->pvr_lond1 != 0U ) ||
       ( psrec->pvr_latd2 != 0U ) ||
       ( psrec->pvr_lond2 != 0U ) ||
       ( psrec->nop != 0U ) ||
       ( psrec->ad != 0U ) )
  {
    s += sizeof(i16);
  }


  if ( ( psrec->pvr_latd2 != 0U ) ||
       ( psrec->pvr_lond2 != 0U ) ||
       ( psrec->nop != 0U ) ||
       ( psrec->ad != 0U ) )
  {
    s += sizeof(i16);
  }

  if ( ( psrec->pvr_lond2 != 0U ) ||
       ( psrec->nop != 0U ) ||
       ( psrec->ad != 0U ) )
  {
    s += sizeof(i16);
  }

  if ( ( psrec->nop != 0U ) ||
       ( psrec->ad != 0U ) )
  {
    s += sizeof(u8);
  }

  if ( psrec->ad != 0U )
  {
    s += psrec->AD_len;
  }

  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_ECALL_MSD_DATA()                                                */
/*                                                                           */
/* Description: Read subrecord ECALL_MSD_DATA from buffer                    */
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
int   egts_read_ECALL_MSD_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  u16  buf_sz0 = *pbuf_sz;
  egts_ECALL_MSD_DATA_t* psrec = (egts_ECALL_MSD_DATA_t*)pv_subrecord;

  EGTS_READ(psrec->FV,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->MI,*ppbuf,*pbuf_sz)
  if ( *pbuf_sz ) {
    egts_set_MSD_bitfields( psrec , **ppbuf );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }    
  EGTS_READ_RAW(psrec->VIN,EGTS_VIN_LEN,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->VPST,*ppbuf,*pbuf_sz)
  EGTS_READ_BE(psrec->TS,*ppbuf,*pbuf_sz)
  EGTS_READ_BE(psrec->PLAT,*ppbuf,*pbuf_sz)
  EGTS_READ_BE(psrec->PLON,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->VD,*ppbuf,*pbuf_sz)

  buf_sz0 -= *pbuf_sz; /* readed size */
  if ( SRL < buf_sz0 ) {
    return -1;
  }
  SRL -= buf_sz0; /* size left */

  psrec->pvr_latd1 = 0U;
  psrec->pvr_lond1 = 0U;
  psrec->pvr_latd2 = 0U;
  psrec->pvr_lond2 = 0U;
  psrec->nop       = 0U;
  psrec->AD_len    = 0U;
  psrec->ad        = 0U;

  if ( SRL >= (u16)sizeof(i16) )
  {
    EGTS_READ_BE(psrec->RVP_LATD_1,*ppbuf,*pbuf_sz)
    psrec->pvr_latd1 = 1U;
    SRL -= sizeof(i16);
  }
  else
  {
    if ( SRL != 0U ) {
      return -1;
    }
  }

  if ( SRL >= (u16)sizeof(i16) )
  {
    EGTS_READ_BE(psrec->RVP_LOND_1,*ppbuf,*pbuf_sz)
    psrec->pvr_lond1 = 1U;
    SRL -= sizeof(i16);
  }
  else
  {
    if ( SRL != 0U ) {
      return -1;
    }
  }

  if ( SRL >= (u16)sizeof(i16) )
  {
    EGTS_READ_BE(psrec->RVP_LATD_2,*ppbuf,*pbuf_sz)
    psrec->pvr_latd2 = 1U;
    SRL -= sizeof(i16);
  }
  else
  {
    if ( SRL != 0U ) {
      return -1;
    }
  }

  if ( SRL >= (u16)sizeof(i16) )
  {
    EGTS_READ_BE(psrec->RVP_LOND_2,*ppbuf,*pbuf_sz)
    psrec->pvr_lond2 = 1U;
    SRL -= sizeof(i16);
  }
  else
  {
    if ( SRL != 0U ) {
      return -1;
    }
  }

  if ( SRL >= (u16)sizeof(u8) )
  {
    EGTS_READ(psrec->NOP,*ppbuf,*pbuf_sz)
    psrec->nop = 1U;
    SRL -= sizeof(u8);
  }

  if ( SRL != 0U )
  {
    /* has additional data */
    if ( SRL > (u16)EGTS_MDS_AD_LEN ) {
      return -1;
    }
    EGTS_READ_RAW(psrec->AD,SRL,*ppbuf,*pbuf_sz)
    psrec->AD_len = SRL;
    psrec->ad   = 1U;
  }

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_ECALL_MSD_DATA()                                               */
/*                                                                           */
/* Description: Write subrecord ECALL_MSD_DATA to buffer                     */
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
int   egts_write_ECALL_MSD_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_ECALL_MSD_DATA_t* psrec = (egts_ECALL_MSD_DATA_t*)pv_subrecord;

  EGTS_WRITE(psrec->FV,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->MI,*ppbuf,*pbuf_sz)
  if ( *pbuf_sz )
  {
    **ppbuf = egts_get_MSD_bitfields( psrec );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }      

  EGTS_WRITE_RAW(psrec->VIN,EGTS_VIN_LEN,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->VPST,*ppbuf,*pbuf_sz)
  EGTS_WRITE_BE(psrec->TS,*ppbuf,*pbuf_sz)
  EGTS_WRITE_BE(psrec->PLAT,*ppbuf,*pbuf_sz)
  EGTS_WRITE_BE(psrec->PLON,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->VD,*ppbuf,*pbuf_sz)

  if ( psrec->pvr_latd1 != 0U )
  {
    EGTS_WRITE_BE(psrec->RVP_LATD_1,*ppbuf,*pbuf_sz)
  }
  else
  {
    if ( ( psrec->pvr_lond1 != 0U ) ||
         ( psrec->pvr_latd2 != 0U ) ||
         ( psrec->pvr_lond2 != 0U ) ||
         ( psrec->nop != 0U ) ||
         ( psrec->ad != 0U ) )
    {
      psrec->RVP_LATD_1 = RVP_SPARE;
      EGTS_WRITE_BE(psrec->RVP_LATD_1,*ppbuf,*pbuf_sz)
    }
  }

  if ( psrec->pvr_lond1 != 0U )
  {
    EGTS_WRITE_BE(psrec->RVP_LOND_1,*ppbuf,*pbuf_sz)
  }
  else
  {
    if ( ( psrec->pvr_latd2 != 0U ) ||
         ( psrec->pvr_lond2 != 0U ) ||
         ( psrec->nop != 0U ) ||
         ( psrec->ad != 0U ) )
    {
      psrec->RVP_LOND_1 = RVP_SPARE;
      EGTS_WRITE_BE(psrec->RVP_LOND_1,*ppbuf,*pbuf_sz)
    }
  }


  if ( psrec->pvr_latd2 != 0U )
  {
    EGTS_WRITE_BE(psrec->RVP_LATD_2,*ppbuf,*pbuf_sz)
  }
  else
  {
    if ( ( psrec->pvr_lond2 != 0U ) ||
         ( psrec->nop != 0U ) ||
         ( psrec->ad != 0U ) )
    {
      psrec->RVP_LATD_2 = RVP_SPARE;
      EGTS_WRITE(psrec->RVP_LATD_2,*ppbuf,*pbuf_sz)
    }
  }

  if ( psrec->pvr_lond2 != 0U )
  {
    EGTS_WRITE_BE(psrec->RVP_LOND_2,*ppbuf,*pbuf_sz)
  }
  else
  {
    if ( ( psrec->nop != 0U ) ||
         ( psrec->ad != 0U ) )
    {
      psrec->RVP_LOND_2 = RVP_SPARE;
      EGTS_WRITE_BE(psrec->RVP_LOND_2,*ppbuf,*pbuf_sz)
    }
  }

  if ( psrec->nop != 0U )
  {
    EGTS_WRITE(psrec->NOP,*ppbuf,*pbuf_sz)
  }
  else
  {
    if ( psrec->ad != 0U )
    {
      psrec->NOP = (u8)NOP_SPARE;
      EGTS_WRITE(psrec->NOP,*ppbuf,*pbuf_sz)
    }
  }

  if ( ( psrec->ad != 0U ) && 
       ( psrec->AD_len != 0U ) )
  {
    EGTS_WRITE_RAW(psrec->AD,psrec->AD_len,*ppbuf,*pbuf_sz)
  }

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_ECALL_MSD_DATA()                                           */
/*                                                                           */
/* Description: Member-wise compare ECALL_MSD_DATA subrecords                */
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
int  egts_is_equial_ECALL_MSD_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 )
{
  egts_ECALL_MSD_DATA_t* psrec1 = (egts_ECALL_MSD_DATA_t*)pv_subrecord1;
  egts_ECALL_MSD_DATA_t* psrec2 = (egts_ECALL_MSD_DATA_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->FV != psrec2->FV ) {
    egts_dbg_printf("inequieal FV");
    return -1;
  }
  
  if ( psrec1->MI != psrec2->MI ) {
    egts_dbg_printf("inequieal MI");
    return -1;
  }
  
  if ( psrec1->VT != psrec2->VT ) {
    egts_dbg_printf("inequieal VT");
    return -1;
  }
  
  if ( psrec1->POCN != psrec2->POCN ) {
    egts_dbg_printf("inequieal POCN");
    return -1;
  }

  if ( psrec1->CLT != psrec2->CLT ) {
    egts_dbg_printf("inequieal CLT");
    return -1;
  }

  if ( psrec1->ACT != psrec2->ACT ) {
    egts_dbg_printf("inequieal ACT");
    return -1;
  }
  
  if ( 0 != memcmp( psrec1->VIN , psrec2->VIN , EGTS_VIN_LEN ) ) {
    egts_dbg_printf("inequieal VIN");
    return -1;        
  }
  
  if ( psrec1->VPST != psrec2->VPST ) {
    egts_dbg_printf("inequieal VPST");
    return -1;
  }
  
  if ( psrec1->TS != psrec2->TS ) {
    egts_dbg_printf("inequieal TS");
    return -1;
  }
  
  if ( psrec1->PLAT != psrec2->PLAT ) {
    egts_dbg_printf("inequieal PLAT");
    return -1;
  }
  
  if ( psrec1->PLON != psrec2->PLON ) {
    egts_dbg_printf("inequieal PLON");
    return -1;
  }

  if ( psrec1->VD != psrec2->VD ) {
    egts_dbg_printf("inequieal VD");
    return -1;
  }
  
  if ( psrec1->pvr_latd1 != psrec2->pvr_latd1 ) {
    egts_dbg_printf("inequieal pvr_latd1");
    return -1;
  }
  
  if ( psrec1->pvr_lond1 != psrec2->pvr_lond1 ) {
    egts_dbg_printf("inequieal pvr_lond1");
    return -1;
  }
  
  if ( psrec1->pvr_latd2 != psrec2->pvr_latd2 ) {
    egts_dbg_printf("inequieal pvr_latd2");
    return -1;
  }
  
  if ( psrec1->pvr_lond2 != psrec2->pvr_lond2 ) {
    egts_dbg_printf("inequieal pvr_lond2");
    return -1;
  }
  
  if ( psrec1->nop != psrec2->nop ) {
    egts_dbg_printf("inequieal nop");
    return -1;
  }
  
  if ( psrec1->ad != psrec2->ad ) {
    egts_dbg_printf("inequieal ad");
    return -1;
  }

  if ( psrec1->pvr_latd1 != 0U ) {
    if ( psrec1->RVP_LATD_1 != psrec2->RVP_LATD_1 ) {
      egts_dbg_printf("inequieal RVP_LATD_1");
      return -1;
    }    
  }
  if ( psrec1->pvr_lond1 != 0U ) {
    if ( psrec1->RVP_LOND_1 != psrec2->RVP_LOND_1 ) {
      egts_dbg_printf("inequieal RVP_LOND_1");
      return -1;
    }        
  }
  if ( psrec1->pvr_latd2 != 0U ) {
    if ( psrec1->RVP_LATD_2 != psrec2->RVP_LATD_2 ) {
      egts_dbg_printf("inequieal RVP_LATD_2");
      return -1;
    }      
  }
  if ( psrec1->pvr_lond2 != 0U ) {
    if ( psrec1->RVP_LOND_2 != psrec2->RVP_LOND_2 ) {
      egts_dbg_printf("inequieal RVP_LOND_2");
      return -1;
    }       
  }
  if ( psrec1->nop != 0U ) {
    if ( psrec1->NOP != psrec2->NOP ) {
      egts_dbg_printf("inequieal NOP");
      return -1;
    }      
  }
  if ( psrec1->ad != 0U ) {
    if ( psrec1->AD_len != psrec2->AD_len ) {
      egts_dbg_printf("inequieal AD_len length");
      return -1;
    }
    if ( psrec1->AD_len != 0U ) {
      if ( 0 != memcmp( psrec1->AD , psrec2->AD , (size_t)psrec1->AD_len ) ) {
        egts_dbg_printf("inequieal AD");
        return -1;        
      }
    } 
  }
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_ECALL_MSD_DATA()                                                */
/*                                                                           */
/* Description: Dump content of ECALL_MSD_DATA subrecord                     */
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
void  egts_dump_ECALL_MSD_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  u8  s,n,h,d,m;
  u32 y;

  i16 cdeg;
  u8  cn,cs;
  i8  in;
  u16 cms;

  egts_ECALL_MSD_DATA_t* psrec = (egts_ECALL_MSD_DATA_t*)pv_subrecord;

  (*afn_dump)( ctx , "SR_MSD_DATA:\n");
  (*afn_dump)( ctx , "  Format Version     FV: %u (%02Xh)\n" , psrec->FV, psrec->FV );
  (*afn_dump)( ctx , "  Message Identifier MI: %u (%02Xh)\n" , psrec->MI, psrec->MI );
  (*afn_dump)( ctx , "  Control            CN:\n" );
  (*afn_dump)( ctx , "    Vehicle Type           VT: %u (%01Xh)\n" , psrec->VT, psrec->VT );
  (*afn_dump)( ctx , "    Position Confidence  POCN: %u (%01Xh)\n" , psrec->POCN, psrec->POCN );
  (*afn_dump)( ctx , "    Call Type             CLT: %u (%s)\n" , psrec->CLT, egts_CLT_str(psrec->CLT) );
  (*afn_dump)( ctx , "    Activation Type       ACT: %u (%s)\n" , psrec->ACT, egts_ACType_str(psrec->ACT) );

  (*afn_dump)( ctx , "  Vehicle Identification Number     VIN:");
  estate_dump_string( psrec->VIN, EGTS_VIN_LEN, ctx, afn_dump );
  (*afn_dump)( ctx , "  Vehicle Propulsion Storage Type  VPST: %u (%02Xh)\n" , psrec->VPST, psrec->VPST );
  egts_get_msd_ts( psrec->TS , &s, &n, &h, &d, &m, &y );
  (*afn_dump)( ctx , "  Time Stamp                         TS: %02u.%02u.%04u %02u:%02u:%02u (%u) (%08Xh)\n" ,
    d, m, y, h, n, s, 
    psrec->TS, psrec->TS );

  if ( psrec->PLAT == EGTS_COO_INVALID ) {
    (*afn_dump)( ctx , "  Position Latitude                PLAT: -\n" );
  } else {
    egts_get_msd_coo( psrec->PLAT , &cdeg , &cn, &cs, &cms );
    (*afn_dump)( ctx , "  Position Latitude                PLAT: %i %02u'%02u.%03u''  %i (%08Xh)\n" ,
      cdeg, cn, cs, cms ,
      psrec->PLAT, psrec->PLAT );
  }

  if ( psrec->PLON == EGTS_COO_INVALID ) {
    (*afn_dump)( ctx , "  Position Longitude               PLON: %i (%08Xh)\n" , psrec->PLON, psrec->PLON );
  } else {
    egts_get_msd_coo( psrec->PLON , &cdeg , &cn, &cs, &cms );
    (*afn_dump)( ctx , "  Position Longitude               PLON: %i %02u'%02u.%03u''  %i (%08Xh)\n" , 
      cdeg, cn, cs, cms ,
      psrec->PLON, psrec->PLON );
  }

 (*afn_dump)( ctx , "  Vehicle Direction                  VD: %u (%02Xh)\n" , psrec->VD, psrec->VD );

  if ( psrec->pvr_latd1 != 0U &&
       psrec->RVP_LATD_1 != EGTS_COO16_INVALID ) {
    egts_get_msd_coo16( psrec->RVP_LATD_1 , &in, &cs, &cms );
    (*afn_dump)( ctx , "  Recent Vehicle Position n-1 Latitude Delta   RVP_LATD_1: %02i'%02u.%03u''  %i (%04Xh)\n" ,
      in, cs, cms ,
      psrec->RVP_LATD_1, psrec->RVP_LATD_1 );
  }  else {
    (*afn_dump)( ctx , "  Recent Vehicle Position n-1 Latitude Delta   RVP_LATD_1: -- \n");
  }

  if ( psrec->pvr_lond1 != 0U &&
       psrec->RVP_LOND_1 != EGTS_COO16_INVALID ) {
    egts_get_msd_coo16( psrec->RVP_LOND_1 , &in, &cs, &cms );
    (*afn_dump)( ctx , "  Recent Vehicle Position n-1 Longitude Delta  RVP_LOND_1: %02i'%02u.%03u''  %i (%04Xh)\n" , 
      in, cs, cms ,
      psrec->RVP_LOND_1, psrec->RVP_LOND_1 );
  }  else {
    (*afn_dump)( ctx , "  Recent Vehicle Position n-1 Longitude Delta  RVP_LOND_1: -- \n" );
  }

  if ( psrec->pvr_latd2 != 0U &&
       psrec->RVP_LATD_2 != EGTS_COO16_INVALID ) {
    egts_get_msd_coo16( psrec->RVP_LATD_2 , &in, &cs, &cms );
    (*afn_dump)( ctx , "  Recent Vehicle Position n-2 Latitude Delta   RVP_LATD_2: %02i'%02u.%03u''  %i (%04Xh)\n" , 
      in, cs, cms ,
      psrec->RVP_LATD_2, psrec->RVP_LATD_2 );
  }  else {
    (*afn_dump)( ctx , "  Recent Vehicle Position n-2 Latitude Delta   RVP_LATD_2: -- \n" );
  }

  if ( psrec->pvr_lond2 != 0U &&
       psrec->RVP_LOND_2 != EGTS_COO16_INVALID ) {
    egts_get_msd_coo16( psrec->RVP_LOND_2 , &in, &cs, &cms );
    (*afn_dump)( ctx , "  Recent Vehicle Position n-2 Longitude Delta  RVP_LOND_2: %02i'%02u.%03u''  %i (%04Xh)\n" , 
      in, cs, cms ,
      psrec->RVP_LOND_2, psrec->RVP_LOND_2 );
  }  else {
    (*afn_dump)( ctx , "  Recent Vehicle Position n-2 Longitude Delta  RVP_LOND_2: -- \n" );
  }

  if ( psrec->nop != 0U ) {
    (*afn_dump)( ctx , "  Number Of Passengers                                  NOP: %u (%02Xh)\n" , psrec->NOP, psrec->NOP );
  }  else {
    (*afn_dump)( ctx , "  Number Of Passengers                                  NOP: -- \n" );
  }

  if ( psrec->ad != 0U )
  {
    (*afn_dump)( ctx , "  Additional Data                                        AD:\n");
    estate_dump_string( psrec->AD, (u32)psrec->AD_len, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  Additional Data                                        AD: -- \n");
  }

}


#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_ECALL_MSD_DATA = 
{
  /* SRT */             (u8)EGTS_SR_MSD_DATA ,
  /* subrecord_size */  (u16)sizeof(egts_ECALL_MSD_DATA_t) ,
  /* fn_get_size */     egts_get_size_ECALL_MSD_DATA ,
  /* fn_read */         egts_read_ECALL_MSD_DATA ,
  /* fn_write */        egts_write_ECALL_MSD_DATA ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_ECALL_MSD_DATA ,
  /* fn_dump */         egts_dump_ECALL_MSD_DATA 
#endif
};


/******************************************************************************
*
*/
