/*****************************************************************************/
/*                                                                           */
/* File: egts_module_data.c                                                  */
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
/*   EGTS_SR_MODULE_DATA, service AUTH, implementations.                     */
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
/* egts_get_size_AUTH_TERM_MODULE_DATA()                                     */
/*                                                                           */
/* Description: Returns size of specified AUTH_TERM_MODULE_DATA subrecord    */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_TERM_MODULE_DATA( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_TERM_MODULE_DATA()                                         */
/*                                                                           */
/* Description: Read subrecord AUTH_TERM_MODULE_DATA from buffer             */
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
int   egts_read_AUTH_TERM_MODULE_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_TERM_MODULE_DATA()                                        */
/*                                                                           */
/* Description: Write subrecord AUTH_TERM_MODULE_DATA to buffer              */
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
int   egts_write_AUTH_TERM_MODULE_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_TERM_MODULE_DATA()                                    */
/*                                                                           */
/* Description: Member-wise compare AUTH_TERM_MODULE_DATA subrecords         */
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
int  egts_is_equial_AUTH_TERM_MODULE_DATA( void* ctx , void* pv_subrecord1 , void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_TERM_MODULE_DATA()                                         */
/*                                                                           */
/* Description: Dump content of AUTH_TERM_MODULE_DATA subrecord              */
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
void  egts_dump_AUTH_TERM_MODULE_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_AUTH_TERM_MODULE_DATA()                                     */
/*                                                                           */
/* Description: Returns size of specified AUTH_TERM_MODULE_DATA subrecord    */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_AUTH_TERM_MODULE_DATA( void* pv_subrecord )
{
  egts_AUTH_TERM_MODULE_DATA_t* psrec = (egts_AUTH_TERM_MODULE_DATA_t*)pv_subrecord;
  u16 s = 0U;
  s += sizeof(u8) +  /*MT*/
       sizeof(u32) + /*VID*/
       sizeof(u16) + /*FWV*/
       sizeof(u16) + /*SWV*/
       sizeof(u8) +  /*MD*/
       sizeof(u8) +  /*ST*/

       psrec->SRN_len +
       psrec->DSCR_len;

  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_AUTH_TERM_MODULE_DATA()                                         */
/*                                                                           */
/* Description: Read subrecord AUTH_TERM_MODULE_DATA from buffer             */
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
int   egts_read_AUTH_TERM_MODULE_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  egts_AUTH_TERM_MODULE_DATA_t* psrec = (egts_AUTH_TERM_MODULE_DATA_t*)pv_subrecord;

  (void)SRL;

  EGTS_READ(psrec->MT,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->VID,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->FWV,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->SWV,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->MD,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->ST,*ppbuf,*pbuf_sz)

  EGTS_READ_Z(psrec->SRN,psrec->SRN_len,EGTS_SRN_LEN,*ppbuf,*pbuf_sz)

  EGTS_READ_Z(psrec->DSCR,psrec->DSCR_len,EGTS_DSCR_LEN,*ppbuf,*pbuf_sz)

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_AUTH_TERM_MODULE_DATA()                                        */
/*                                                                           */
/* Description: Write subrecord AUTH_TERM_MODULE_DATA to buffer              */
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
int   egts_write_AUTH_TERM_MODULE_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{

  egts_AUTH_TERM_MODULE_DATA_t* psrec = (egts_AUTH_TERM_MODULE_DATA_t*)pv_subrecord;

  EGTS_WRITE(psrec->MT,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->VID,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->FWV,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->SWV,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->MD,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->ST,*ppbuf,*pbuf_sz)

  EGTS_WRITE_Z(psrec->SRN,psrec->SRN_len,*ppbuf,*pbuf_sz)

  EGTS_WRITE_Z(psrec->DSCR,psrec->DSCR_len,*ppbuf,*pbuf_sz)

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_AUTH_TERM_MODULE_DATA()                                    */
/*                                                                           */
/* Description: Member-wise compare AUTH_TERM_MODULE_DATA subrecords         */
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
int  egts_is_equial_AUTH_TERM_MODULE_DATA( void* ctx , void* pv_subrecord1 , void* pv_subrecord2 )
{
  egts_AUTH_TERM_MODULE_DATA_t* psrec1 = (egts_AUTH_TERM_MODULE_DATA_t*)pv_subrecord1;
  egts_AUTH_TERM_MODULE_DATA_t* psrec2 = (egts_AUTH_TERM_MODULE_DATA_t*)pv_subrecord2;
  
  (void)ctx;

  if ( psrec1->MT != psrec2->MT ) {
    egts_dbg_printf("inequieal MT");
    return -1;
  }

  if ( psrec1->VID != psrec2->VID ) {
    egts_dbg_printf("inequieal VID");
    return -1;
  }

  if ( psrec1->FWV != psrec2->FWV ) {
    egts_dbg_printf("inequieal FWV");
    return -1;
  }

  if ( psrec1->SWV != psrec2->SWV ) {
    egts_dbg_printf("inequieal SWV");
    return -1;
  }

  if ( psrec1->MD != psrec2->MD ) {
    egts_dbg_printf("inequieal MD");
    return -1;
  }

  if ( psrec1->ST != psrec2->ST ) {
    egts_dbg_printf("inequieal ST");
    return -1;
  }

  if ( psrec1->SRN_len != psrec2->SRN_len ) {
    egts_dbg_printf("inequieal SRN_len length");
    return -1;
  }
  if ( psrec1->SRN_len != 0U ) {
    if ( 0 != memcmp( psrec1->SRN , psrec2->SRN , (size_t)psrec1->SRN_len ) ) {
      egts_dbg_printf("inequieal PKL");
      return -1;        
    }
  }

  if ( psrec1->DSCR_len != psrec2->DSCR_len ) {
    egts_dbg_printf("inequieal DSCR_len length");
    return -1;
  }
  if ( psrec1->DSCR_len != 0U ) {
    if ( 0 != memcmp( psrec1->DSCR , psrec2->DSCR , (size_t)psrec1->DSCR_len ) ) {
      egts_dbg_printf("inequieal PKL");
      return -1;        
    }
  }
  
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_AUTH_TERM_MODULE_DATA()                                         */
/*                                                                           */
/* Description: Dump content of AUTH_TERM_MODULE_DATA subrecord              */
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
void  egts_dump_AUTH_TERM_MODULE_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_AUTH_TERM_MODULE_DATA_t* psrec = (egts_AUTH_TERM_MODULE_DATA_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_MODULE_DATA:\n");
  (*afn_dump)( ctx , "  Module Type        MT: %u (%02Xh)\n" , psrec->MT , psrec->MT );
  (*afn_dump)( ctx , "  Vendor Identifier  VID: %u (%08Xh)\n" , psrec->VID , psrec->VID );
  (*afn_dump)( ctx , "  Firmware Version   FWV: %04Xh\n" , psrec->FWV );
  (*afn_dump)( ctx , "  Software Version   SWV: %04Xh\n" , psrec->SWV );
  (*afn_dump)( ctx , "  Modification       MD: %u (%02Xh)\n" , psrec->MD, psrec->MD );
  (*afn_dump)( ctx , "  State              ST: %u (%02Xh)\n" , psrec->ST, psrec->ST );

  (*afn_dump)( ctx , "  Serial Number      SRN:\n" , psrec->SRN, psrec->SRN );
  estate_dump_string_z( psrec->SRN, EGTS_SRN_LEN , ctx, afn_dump );

  (*afn_dump)( ctx , "  Description        DSCR:\n" , psrec->DSCR, psrec->DSCR );
  estate_dump_string_z( psrec->DSCR, EGTS_DSCR_LEN , ctx, afn_dump );
}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_AUTH_TERM_MODULE_DATA = 
{
  /* SRT */             (u8)EGTS_SR_MODULE_DATA ,
  /* subrecord_size */  (u16)sizeof(egts_AUTH_TERM_MODULE_DATA_t) ,
  /* fn_get_size */     egts_get_size_AUTH_TERM_MODULE_DATA ,
  /* fn_read */         egts_read_AUTH_TERM_MODULE_DATA ,
  /* fn_write */        egts_write_AUTH_TERM_MODULE_DATA ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_AUTH_TERM_MODULE_DATA ,
  /* fn_dump */         egts_dump_AUTH_TERM_MODULE_DATA 
#endif
};


/******************************************************************************
*
*/
