/*****************************************************************************/
/*                                                                           */
/* File: egts_part_data.c                                                    */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service FIRMWARE                                          */
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
/*   EGTS_SR_SERVICE_PART_DATA , service FIRMWARE, implementation            */
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
#include "egts_firmware.h"

#ifdef EGTS_DBG
#include "../../transport/egts_dump.h"
#endif

/******************************************************************************
*
*/

/* TODO: draft */
static
u8    egts_firmware_rx_buffer[ 65400 ];

/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_FIRMWARE_PART_DATA()                                        */
/*                                                                           */
/* Description: Returns size of specified FIRMWARE_PART_DATA subrecord       */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_FIRMWARE_PART_DATA( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_FIRMWARE_PART_DATA()                                            */
/*                                                                           */
/* Description: Read subrecord FIRMWARE_PART_DATA from buffer                */
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
int   egts_read_FIRMWARE_PART_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_write_FIRMWARE_PART_DATA()                                           */
/*                                                                           */
/* Description: Write subrecord FIRMWARE_PART_DATA to buffer                 */
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
int   egts_write_FIRMWARE_PART_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_FIRMWARE_PART_DATA()                                       */
/*                                                                           */
/* Description: Member-wise compare FIRMWARE_PART_DATA subrecords            */
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
int  egts_is_equial_FIRMWARE_PART_DATA(  void* ctx , void* pv_subrecord1 , void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_FIRMWARE_PART_DATA()                                            */
/*                                                                           */
/* Description: Dump content of FIRMWARE_PART_DATA subrecord                 */
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
void  egts_dump_FIRMWARE_PART_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/******************************************************************************
* mplementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_FIRMWARE_PART_DATA()                                        */
/*                                                                           */
/* Description: Returns size of specified FIRMWARE_PART_DATA subrecord       */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_FIRMWARE_PART_DATA( void* pv_subrecord )
{
  egts_FIRMWARE_PART_DATA_t* psrec = (egts_FIRMWARE_PART_DATA_t*)pv_subrecord;
  u16 s = 0U;
  s += sizeof(u16) + /* ID */
       sizeof(u16) + /* PN */
       sizeof(u16);/* EPQ */
  if ( psrec->PN == 1U )
  {
    s += sizeof(u8) + /* header.OT + header.MT */
         sizeof(u8) + /* header.CMI */
         sizeof(u16) + /* header.VER */
         sizeof(u16);/* header.WOS */
    s += psrec->header.FN_len + 1U;
  }   
  s += psrec->OD_len;
  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_FIRMWARE_PART_DATA()                                            */
/*                                                                           */
/* Description: Read subrecord FIRMWARE_PART_DATA from buffer                */
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
int   egts_read_FIRMWARE_PART_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  u16  buf_sz0 = *pbuf_sz;
  egts_FIRMWARE_PART_DATA_t* psrec = (egts_FIRMWARE_PART_DATA_t*)pv_subrecord;

  EGTS_READ(psrec->ID,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->PN,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->EPQ,*ppbuf,*pbuf_sz)
  if ( psrec->PN == 1U )
  {
    if ( *pbuf_sz ) {
      egts_set_FIRMWARE_bitfields( &psrec->header , **ppbuf );
      (*ppbuf)++;
      (*pbuf_sz)--;
    } else {
      return -1;
    }  

    EGTS_READ(psrec->header.CMI,*ppbuf,*pbuf_sz)
    EGTS_READ(psrec->header.VER,*ppbuf,*pbuf_sz)
    EGTS_READ(psrec->header.WOS,*ppbuf,*pbuf_sz)
    EGTS_READ_Z(psrec->header.FN,psrec->header.FN_len,EGTS_FN_LEN,*ppbuf,*pbuf_sz)
  }

  buf_sz0 -= *pbuf_sz; /* readed size */
  if ( SRL <= buf_sz0 ) {
    return -1;
  }
  
  psrec->OD_len = SRL - buf_sz0;
  
  /* TODO: draft */
  psrec->OD = egts_firmware_rx_buffer;

  EGTS_READ_RAW(psrec->OD,psrec->OD_len,*ppbuf,*pbuf_sz)

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_FIRMWARE_PART_DATA()                                           */
/*                                                                           */
/* Description: Write subrecord FIRMWARE_PART_DATA to buffer                 */
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
int   egts_write_FIRMWARE_PART_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_FIRMWARE_PART_DATA_t* psrec = (egts_FIRMWARE_PART_DATA_t*)pv_subrecord;

  EGTS_WRITE(psrec->ID,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->PN,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->EPQ,*ppbuf,*pbuf_sz)
  if ( psrec->PN == 1U )
  {
    if ( *pbuf_sz )
    {
      **ppbuf = egts_get_FIRMWARE_bitfields( &psrec->header );
      (*ppbuf)++;
      (*pbuf_sz)--;
    } else {
      return -1;
    }        

    EGTS_WRITE(psrec->header.CMI,*ppbuf,*pbuf_sz)
    EGTS_WRITE(psrec->header.VER,*ppbuf,*pbuf_sz)
    EGTS_WRITE(psrec->header.WOS,*ppbuf,*pbuf_sz)
    EGTS_WRITE_Z(psrec->header.FN,psrec->header.FN_len,*ppbuf,*pbuf_sz)
  }
  EGTS_WRITE_RAW(psrec->OD,psrec->OD_len,*ppbuf,*pbuf_sz)

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_FIRMWARE_PART_DATA()                                       */
/*                                                                           */
/* Description: Member-wise compare FIRMWARE_PART_DATA subrecords            */
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
int  egts_is_equial_FIRMWARE_PART_DATA( void* ctx , void* pv_subrecord1 , void* pv_subrecord2 )
{
  egts_FIRMWARE_PART_DATA_t* psrec1 = (egts_FIRMWARE_PART_DATA_t*)pv_subrecord1;
  egts_FIRMWARE_PART_DATA_t* psrec2 = (egts_FIRMWARE_PART_DATA_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->ID != psrec2->ID ) {
    egts_dbg_printf("inequieal ID");
    return -1;
  }
    
  if ( psrec1->PN != psrec2->PN ) {
    egts_dbg_printf("inequieal PN");
    return -1;
  }

  if ( psrec1->EPQ != psrec2->EPQ ) {
    egts_dbg_printf("inequieal EPQ");
    return -1;
  }
   
    
  if ( psrec1->PN == 1U )
  {
    if ( psrec1->header.OT != psrec2->header.OT ) {
      egts_dbg_printf("inequieal header.OT");
      return -1;
    }
    if ( psrec1->header.MT != psrec2->header.MT ) {
      egts_dbg_printf("inequieal header.MT");
      return -1;
    }
    if ( psrec1->header.CMI != psrec2->header.CMI ) {
      egts_dbg_printf("inequieal header.CMI");
      return -1;
    }
    if ( psrec1->header.VER != psrec2->header.VER ) {
      egts_dbg_printf("inequieal header.VER");
      return -1;
    }
    if ( psrec1->header.WOS != psrec2->header.WOS ) {
      egts_dbg_printf("inequieal header.WOS");
      return -1;
    }
    if ( psrec1->header.FN_len != psrec2->header.FN_len ) {
      egts_dbg_printf("inequieal header.FN_len length");
      return -1;
    }
    if ( psrec1->header.FN_len != 0U ) {
      if ( 0 != memcmp( psrec1->header.FN , psrec2->header.FN , (size_t)psrec1->header.FN_len ) ) {
        egts_dbg_printf("inequieal header.FN");
        return -1;        
      }
    }
  }
  
  if ( psrec1->OD_len != 0U )
  {
    if ( psrec1->OD_len != psrec2->OD_len ) {
      egts_dbg_printf("inequiealOD_len length");
      return -1;
    }
    if ( psrec1->OD_len != 0U ) {
      if ( 0 != memcmp( psrec1->OD , psrec2->OD , (size_t)psrec1->OD_len ) ) {
        egts_dbg_printf("inequieal header.FN");
        return -1;        
      }
    }    
  }


  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_FIRMWARE_PART_DATA()                                            */
/*                                                                           */
/* Description: Dump content of FIRMWARE_PART_DATA subrecord                 */
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
void  egts_dump_FIRMWARE_PART_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_FIRMWARE_PART_DATA_t* psrec = (egts_FIRMWARE_PART_DATA_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_SERVICE_PART_DATA:\n");
  (*afn_dump)( ctx , "  Identity                 ID: %u (%04Xh)\n" , psrec->ID, psrec->ID );
  (*afn_dump)( ctx , "  Part Number              PN: %u (%04Xh)\n" , psrec->PN, psrec->PN );
  (*afn_dump)( ctx , "  Expected Parts Quantity EPQ: %u (%04Xh)\n" , psrec->EPQ, psrec->EPQ );
  if ( psrec->PN == 1U )
  {
    egts_dump_FIRMWARE_DATA_header( ctx , &psrec->header ,  afn_dump );
  }
  (*afn_dump)( ctx , "  Object Data              OD: len=%u\n" , psrec->OD_len );
  estate_dump_string( psrec->OD, (u32)psrec->OD_len, ctx, afn_dump );
}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_FIRMWARE_PART_DATA = 
{
  /* SRT */             (u8)EGTS_SR_SERVICE_PART_DATA ,
  /* subrecord_size */  (u16)sizeof(egts_FIRMWARE_PART_DATA_t) ,
  /* fn_get_size */     egts_get_size_FIRMWARE_PART_DATA ,
  /* fn_read */         egts_read_FIRMWARE_PART_DATA ,
  /* fn_write */        egts_write_FIRMWARE_PART_DATA ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_FIRMWARE_PART_DATA ,
  /* fn_dump */         egts_dump_FIRMWARE_PART_DATA 
#endif
};


/******************************************************************************
*
*/
