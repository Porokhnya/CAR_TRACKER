/*****************************************************************************/
/*                                                                           */
/* File: egts_command_data.c                                                 */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service COMMANDS                                          */
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
/*   EGTS_SR_COMMAND_DATA, service COMMANDS, implementation                  */ 
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
#include "egts_command.h"

#ifdef EGTS_DBG
#include "../../transport/egts_dump.h"
#endif

/******************************************************************************
*
*/

/* TODO: draft */
static
u8    egts_commands_rx_buffer[ 65205 ];


/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_COMMANDS_COMMAND_DATA()                                     */
/*                                                                           */
/* Description: Returns size of specified COMMANDS_COMMAND_DATA subrecord    */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_COMMANDS_COMMAND_DATA( void* pv_subrecord );

/*****************************************************************************/
/*                                                                           */
/* egts_read_COMMANDS_COMMAND_DATA()                                         */
/*                                                                           */
/* Description: Read subrecord COMMANDS_COMMAND_DATA from buffer             */
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
int   egts_read_COMMANDS_COMMAND_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz );


/*****************************************************************************/
/*                                                                           */
/* egts_write_COMMANDS_COMMAND_DATA()                                        */
/*                                                                           */
/* Description: Write subrecord COMMANDS_COMMAND_DATA to buffer              */
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
int   egts_write_COMMANDS_COMMAND_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz );

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_COMMANDS_COMMAND_DATA()                                    */
/*                                                                           */
/* Description: Member-wise compare COMMANDS_COMMAND_DATA subrecords         */
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
int  egts_is_equial_COMMANDS_COMMAND_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_COMMANDS_COMMAND_DATA()                                         */
/*                                                                           */
/* Description: Dump content of COMMANDS_COMMAND_DATA subrecord              */
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
void  egts_dump_COMMANDS_COMMAND_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  );

#endif

/*****************************************************************************/
/*                                                                           */
/* egts_get_COMMAND_DATA_types()                                             */
/*                                                                           */
/* Description: Collect COMMAND_DATA subrecord bitfields for Type            */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_COMMAND_DATA_types( egts_COMMANDS_COMMAND_DATA_t* ps );

/*****************************************************************************/
/*                                                                           */
/* egts_set_COMMAND_DATA_types()                                             */
/*                                                                           */
/* Description: Set COMMAND_DATA subrecord bitfields for Type                */
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
void  egts_set_COMMAND_DATA_types( egts_COMMANDS_COMMAND_DATA_t* ps , u8 b );

/*****************************************************************************/
/*                                                                           */
/* egts_get_COMMAND_DATA_types()                                             */
/*                                                                           */
/* Description: Collect COMMAND_DATA subrecord bitfields for Flags           */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_COMMAND_DATA_flags( egts_COMMANDS_COMMAND_DATA_t* ps );

/*****************************************************************************/
/*                                                                           */
/* egts_set_COMMAND_DATA_types()                                             */
/*                                                                           */
/* Description: Set COMMAND_DATA subrecord bitfields for Flags               */
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
void  egts_set_COMMAND_DATA_flags( egts_COMMANDS_COMMAND_DATA_t* ps , u8 b );

/*****************************************************************************/
/*                                                                           */
/* egts_get_COMMAND_bitfields()                                              */
/*                                                                           */
/* Description: Collect COMMAND_DATA subrecord bitfields                     */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_COMMAND_bitfields( egts_COMMAND_t* ps );

/*****************************************************************************/
/*                                                                           */
/* egts_set_COMMAND_bitfields()                                              */
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
void  egts_set_COMMAND_bitfields( egts_COMMAND_t* ps , u8 b );

/******************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* egts_get_COMMAND_DATA_types()                                             */
/*                                                                           */
/* Description: Collect COMMAND_DATA subrecord bitfields for Type            */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_COMMAND_DATA_types( egts_COMMANDS_COMMAND_DATA_t* ps )
{
  u8 b = 0U;
  b |= ( ps->CT  & 0x0FU );    
  b <<= 4U;
  b |= ( ps->CCT & 0x0FU );
  return b;   
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_COMMAND_DATA_types()                                             */
/*                                                                           */
/* Description: Set COMMAND_DATA subrecord bitfields for Type                */
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
void  egts_set_COMMAND_DATA_types( egts_COMMANDS_COMMAND_DATA_t* ps , u8 b )
{
  ps->CCT = b & 0x0FU;   
  b >>= 4U;
  ps->CT  = b & 0x0FU;
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_COMMAND_DATA_types()                                             */
/*                                                                           */
/* Description: Collect COMMAND_DATA subrecord bitfields for Flags           */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_COMMAND_DATA_flags( egts_COMMANDS_COMMAND_DATA_t* ps )
{
  u8 b = 0U;
  b |= ( ps->ACFE & 0x01U );     
  b <<= 1U;
  b |= ( ps->CHSFE & 0x01U );
  return b;     
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_COMMAND_DATA_types()                                             */
/*                                                                           */
/* Description: Set COMMAND_DATA subrecord bitfields for Flags               */
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
void  egts_set_COMMAND_DATA_flags( egts_COMMANDS_COMMAND_DATA_t* ps , u8 b )
{
  ps->CHSFE = b & 0x01U;   
  b >>= 1U;
  ps->ACFE  = b & 0x01U;  
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_COMMAND_bitfields()                                              */
/*                                                                           */
/* Description: Collect COMMAND_DATA subrecord bitfields                     */
/*                                                                           */
/* Arguments: ps - subrecord                                                 */
/*                                                                           */
/* Return:    bitfields byte                                                 */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u8    egts_get_COMMAND_bitfields( egts_COMMAND_t* ps )
{
  u8 b = 0U;
  b |= ( ps->ACT & 0x0FU );     
  b <<= 4U;
  b |= ( ps->SZ  & 0x0FU );
  return b; 
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_COMMAND_bitfields()                                              */
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
void  egts_set_COMMAND_bitfields( egts_COMMAND_t* ps , u8 b )
{
  ps->SZ   = b & 0x0FU;  
  b >>= 4U;
  ps->ACT  = b & 0x0FU;  
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_size_COMMANDS_COMMAND_DATA()                                     */
/*                                                                           */
/* Description: Returns size of specified COMMANDS_COMMAND_DATA subrecord    */
/*                                                                           */
/* Arguments: pv_subrecord - pointer to subrecord                            */
/*                                                                           */
/* Return:    size of subrecord                                              */
/*                                                                           */
/* Other:     internal use only                                              */
/*                                                                           */
/*****************************************************************************/

static
u16   egts_get_size_COMMANDS_COMMAND_DATA( void* pv_subrecord )
{
  egts_COMMANDS_COMMAND_DATA_t* psrec = (egts_COMMANDS_COMMAND_DATA_t*)pv_subrecord;
  u16 s = 0U;
  s += sizeof(u8) + /* C */
       sizeof(u32) + /* CID */
       sizeof(u32) + /* SID */
       sizeof(u8);/* FLAGS */
  if ( psrec->CHSFE != 0U ) {
    s += sizeof(u8);/* CHS */
  }
  if ( psrec->ACFE != 0U ) {
    s += sizeof(u8) + (u16)psrec->ACL;
  }
  switch ( psrec->CT )
  {
  case CT_COM :
    s += sizeof(u16) + /* ADR */
         sizeof(u8)  + /* F */
         sizeof(u16);/* CCD  */
    break;
  case CT_COMCONF :
    s += sizeof(u16) + /* ADR */
         sizeof(u16);/* CCD  */
    break;
  default:
    break;
  }

  s += psrec->DT_len;

  return s;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_COMMANDS_COMMAND_DATA()                                         */
/*                                                                           */
/* Description: Read subrecord COMMANDS_COMMAND_DATA from buffer             */
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
int   egts_read_COMMANDS_COMMAND_DATA( void* pv_subrecord , u16 SRL , 
  u8**  ppbuf ,
  u16*  pbuf_sz )
{
  u16  buf_sz0 = *pbuf_sz;
  egts_COMMANDS_COMMAND_DATA_t* psrec = (egts_COMMANDS_COMMAND_DATA_t*)pv_subrecord;

  if ( *pbuf_sz ) {
    egts_set_COMMAND_DATA_types( psrec , **ppbuf );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }
    
  EGTS_READ(psrec->CID,*ppbuf,*pbuf_sz)
  EGTS_READ(psrec->SID,*ppbuf,*pbuf_sz)
    
  if ( *pbuf_sz ) {
    egts_set_COMMAND_DATA_flags( psrec , **ppbuf );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }  
    
  if ( psrec->CHSFE != 0U )
  {
    EGTS_READ(psrec->CHS,*ppbuf,*pbuf_sz)
  }
  if ( psrec->ACFE != 0U )
  {
    EGTS_READ(psrec->ACL,*ppbuf,*pbuf_sz)
    if ( psrec->ACL != 0U )
    {
      EGTS_READ_RAW(psrec->AC,(u16)psrec->ACL,*ppbuf,*pbuf_sz)
    }
  }

  switch ( psrec->CT )
  {

  case CT_COM :
    EGTS_READ(psrec->command.ADR,*ppbuf,*pbuf_sz)
    if ( *pbuf_sz ) {
      egts_set_COMMAND_bitfields( &psrec->command , **ppbuf );
      (*ppbuf)++;
      (*pbuf_sz)--;
    } else {
      return -1;
    }  
    EGTS_READ(psrec->command.CCD,*ppbuf,*pbuf_sz)
    break;

  case CT_COMCONF :
    EGTS_READ(psrec->conf.ADR,*ppbuf,*pbuf_sz)
    EGTS_READ(psrec->conf.CCD,*ppbuf,*pbuf_sz)
    break;

  default:
    break;

  }

  buf_sz0 -= *pbuf_sz; /* readed size */
  if ( SRL <= buf_sz0 ) {
    return -1;
  }

  psrec->DT = egts_commands_rx_buffer;

  psrec->DT_len = SRL - buf_sz0;

  EGTS_READ_RAW(psrec->DT,psrec->DT_len,*ppbuf,*pbuf_sz)

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_COMMANDS_COMMAND_DATA()                                        */
/*                                                                           */
/* Description: Write subrecord COMMANDS_COMMAND_DATA to buffer              */
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
int   egts_write_COMMANDS_COMMAND_DATA( void* pv_subrecord ,
  u8**   ppbuf ,
  u16*   pbuf_sz )
{
  egts_COMMANDS_COMMAND_DATA_t* psrec = (egts_COMMANDS_COMMAND_DATA_t*)pv_subrecord;

  if ( *pbuf_sz )
  {
    **ppbuf = egts_get_COMMAND_DATA_types( psrec );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }
    
  EGTS_WRITE(psrec->CID,*ppbuf,*pbuf_sz)
  EGTS_WRITE(psrec->SID,*ppbuf,*pbuf_sz)
  if ( *pbuf_sz )
  {
    **ppbuf = egts_get_COMMAND_DATA_flags( psrec );
    (*ppbuf)++;
    (*pbuf_sz)--;
  } else {
    return -1;
  }    

  if ( psrec->CHSFE != 0U )
  {
    EGTS_WRITE(psrec->CHS,*ppbuf,*pbuf_sz)
  }
  if ( psrec->ACFE != 0U )
  {
    EGTS_WRITE(psrec->ACL,*ppbuf,*pbuf_sz)
    if ( psrec->ACL != 0U )
    {
      EGTS_WRITE_RAW(psrec->AC,(u16)psrec->ACL,*ppbuf,*pbuf_sz)
    }
  }

  switch ( psrec->CT )
  {
  case CT_COM :
    EGTS_WRITE(psrec->command.ADR,*ppbuf,*pbuf_sz)
    if ( *pbuf_sz )
    {
      **ppbuf = egts_get_COMMAND_bitfields( &psrec->command );
      (*ppbuf)++;
      (*pbuf_sz)--;
    } else {
      return -1;
    }  

    EGTS_WRITE(psrec->command.CCD,*ppbuf,*pbuf_sz)
    break;
  case CT_COMCONF :
    EGTS_WRITE(psrec->conf.ADR,*ppbuf,*pbuf_sz)
    EGTS_WRITE(psrec->conf.CCD,*ppbuf,*pbuf_sz)
    break;
  default:
    break;
  }

  EGTS_WRITE_RAW(psrec->DT,psrec->DT_len,*ppbuf,*pbuf_sz)

  return 0;
}


/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_is_equial_COMMANDS_COMMAND_DATA()                                    */
/*                                                                           */
/* Description: Member-wise compare COMMANDS_COMMAND_DATA subrecords         */
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
int  egts_is_equial_COMMANDS_COMMAND_DATA( void* ctx , void* pv_subrecord1 ,  void* pv_subrecord2 )
{
  egts_COMMANDS_COMMAND_DATA_t* psrec1 = (egts_COMMANDS_COMMAND_DATA_t*)pv_subrecord1;
  egts_COMMANDS_COMMAND_DATA_t* psrec2 = (egts_COMMANDS_COMMAND_DATA_t*)pv_subrecord2;

  (void)ctx;

  if ( psrec1->CT != psrec2->CT ) {
    egts_dbg_printf("inequieal CT");
    return -1;
  }
  
  if ( psrec1->CCT != psrec2->CCT ) {
    egts_dbg_printf("inequieal CCT");
    return -1;
  }
  
  if ( psrec1->CID != psrec2->CID ) {
    egts_dbg_printf("inequieal CID");
    return -1;
  }
  
  if ( psrec1->SID != psrec2->SID ) {
    egts_dbg_printf("inequieal SID");
    return -1;
  }
  
  if ( psrec1->ACFE != psrec2->ACFE ) {
    egts_dbg_printf("inequieal ACFE");
    return -1;
  }
  
  if ( psrec1->CHSFE != psrec2->CHSFE ) {
    egts_dbg_printf("inequieal CHSFE");
    return -1;
  }
  
  if ( psrec1->CHS != psrec2->CHS ) {
    egts_dbg_printf("inequieal CHS");
    return -1;
  }
  
  if ( psrec1->ACL != psrec2->ACL ) {
    egts_dbg_printf("inequieal ACL");
    return -1;
  }
  
  if ( psrec1->ACL != psrec2->ACL ) {
    egts_dbg_printf("inequieal ACL");
    return -1;
  }
  
  if ( psrec1->ACL != psrec2->ACL ) {
    egts_dbg_printf("inequieal ACL length");
    return -1;
  }
  if ( psrec1->ACL != 0U ) {
    if ( 0 != memcmp( psrec1->AC , psrec2->AC , (size_t)psrec1->ACL ) ) {
      egts_dbg_printf("inequieal AC");
      return -1;        
    }
  }

  switch ( psrec1->CT )
  {
  case CT_COM :
    if ( psrec1->command.ADR != psrec2->command.ADR ) {
      egts_dbg_printf("inequieal cmd.command.ADR");
      return -1;
    }
    if ( psrec1->command.SZ != psrec2->command.SZ ) {
      egts_dbg_printf("inequieal cmd.command.SZ");
      return -1;
    }
    if ( psrec1->command.ACT != psrec2->command.ACT ) {
      egts_dbg_printf("inequieal cmd.command.ACT");
      return -1;
    }
    if ( psrec1->command.CCD != psrec2->command.CCD ) {
      egts_dbg_printf("inequieal cmd.command.CCD");
      return -1;
    }
    break;
    
  case CT_COMCONF :
    if ( psrec1->conf.ADR != psrec2->conf.ADR ) {
      egts_dbg_printf("inequieal cmd.conf.ADR");
      return -1;
    }    
    if ( psrec1->conf.CCD != psrec2->conf.CCD ) {
      egts_dbg_printf("inequieal cmd.conf.CCD");
      return -1;
    }        
    break;
    
  default:
    break;
    
  }

  if ( psrec1->DT_len != 0U ) {
    if ( psrec1->DT_len != psrec2->DT_len ) {
      egts_dbg_printf("inequieal DT_len length");
      return -1;
    }
    if ( psrec1->DT_len != 0U ) {
      if ( 0 != memcmp( psrec1->DT , psrec2->DT , (size_t)psrec1->DT_len ) ) {
        egts_dbg_printf("inequieal DT");
        return -1;        
      }
    } 
  }

  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_dump_COMMANDS_COMMAND_DATA()                                         */
/*                                                                           */
/* Description: Dump content of COMMANDS_COMMAND_DATA subrecord              */
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
void  egts_dump_COMMANDS_COMMAND_DATA( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_COMMANDS_COMMAND_DATA_t* psrec = (egts_COMMANDS_COMMAND_DATA_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_COMMAND_DATA:\n");
  (*afn_dump)( ctx , "  Command Type               CT: %u (%s)\n" , psrec->CT, egts_CT_str(psrec->CT) );

  if( (  psrec->CT == (u8)CT_COMCONF ) ||
      ( psrec->CT == (u8)CT_MSGCONF ) ||
        ( psrec->CT == (u8)CT_DELIV ) ) {
    (*afn_dump)( ctx , "  Command Confirmation Type  CCT: %u (%s)\n" , psrec->CCT, egts_CC_str(psrec->CCT) );
  }  else {
    (*afn_dump)( ctx , "  Command Confirmation Type  CCT: %u (%s) (unexpected)\n" , psrec->CCT, egts_CC_str(psrec->CCT)  );
  }
  
  (*afn_dump)( ctx , "  Command Identifier         CID: %u (%08Xh)\n" , psrec->CID, psrec->CID );
  (*afn_dump)( ctx , "  Source Identifier          SID: %u (%08Xh)\n" , psrec->SID, psrec->SID );
  (*afn_dump)( ctx , "  FLAGS: ACFE= %u, CHSFE= %u\n" , psrec->ACFE, psrec->CHSFE );

  if( psrec->CHSFE != 0U ) {
    (*afn_dump)( ctx , "  Charset                    CHS: %u (%s)\n" , psrec->CHS, egts_CHS_str(psrec->CHS) );
  }  else {
    (*afn_dump)( ctx , "  Charset                    CHS: ---\n" );
  }

  if( psrec->ACFE != 0U ) {
    (*afn_dump)( ctx , "  Authorization Code Length  ACL: %u (%02Xh)\n" , psrec->ACL, psrec->ACL );
  } else {
    (*afn_dump)( ctx , "  Authorization Code Length  ACL: ---\n" );
  }

  if( psrec->ACFE != 0U )
  {
    (*afn_dump)( ctx , "  Authorization Code         AC:\n" );
    estate_dump_string_z( psrec->AC, (u32)psrec->ACL, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "  Authorization Code         AC: ---\n" );
  }

  switch ( psrec->CT )
  {

  case CT_COM :
    (*afn_dump)( ctx , "    COMMAND:\n" );
    (*afn_dump)( ctx , "    Address                   ADR: %u (%04Xh)\n" , psrec->command.ADR, psrec->command.ADR );
    (*afn_dump)( ctx , "    Size                       SZ: %u \n" , psrec->command.SZ );
    (*afn_dump)( ctx , "    Action                    ACT: %u (%02Xh)\n" , psrec->command.ACT, psrec->command.ACT );
    (*afn_dump)( ctx , "    Command Code              CCD: %u (%04Xh)\n" , psrec->command.CCD, psrec->command.CCD );
    break;

  case CT_COMCONF :
    (*afn_dump)( ctx , "    CONF:\n" );
    (*afn_dump)( ctx , "    Address                   ADR: %u (%04Xh)\n" , psrec->conf.ADR, psrec->conf.ADR );
    (*afn_dump)( ctx , "    Command Code              CCD: %u (%04Xh)\n" , psrec->conf.CCD, psrec->conf.CCD );
    break;

  default:
    break;
    
  }

  if ( psrec->DT_len != 0U )
  {
    (*afn_dump)( ctx , "                               DT: size=%u\n" , psrec->DT_len );
    estate_dump_string( psrec->DT, (u32)psrec->DT_len, ctx, afn_dump );
  }  else {
    (*afn_dump)( ctx , "                               DT: -- \n" );
  }

}

#endif

/******************************************************************************
*
*/

const egts_subrecord_handler_t  chandler_COMMANDS_COMMAND_DATA = 
{
  /* SRT */             (u8)EGTS_SR_COMMAND_DATA ,
  /* subrecord_size */  (u16)sizeof(egts_COMMANDS_COMMAND_DATA_t) ,
  /* fn_get_size */     egts_get_size_COMMANDS_COMMAND_DATA ,
  /* fn_read */         egts_read_COMMANDS_COMMAND_DATA ,
  /* fn_write */        egts_write_COMMANDS_COMMAND_DATA ,
#ifdef EGTS_DBG
  /* fs_is_equial */    egts_is_equial_COMMANDS_COMMAND_DATA ,
  /* fn_dump */         egts_dump_COMMANDS_COMMAND_DATA 
#endif
};


/******************************************************************************
*
*/
