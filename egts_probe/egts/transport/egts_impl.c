/*****************************************************************************/
/*                                                                           */
/* File: egts_impl.c                                                         */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Transport Layer                                           */
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
/* Description: Misceleniouse EGTS functions. Most of then for private       */
/*   implementaton guts.                                                     */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: public : egts_init, egts_tx_packet.                            */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"

#include "../include/egts.h"
#include "../include/egts_impl.h"

#include "egts_guts.h"

/*****************************************************************************/
/*                                                                           */
/* egts_init()                                                               */
/*                                                                           */
/* Description: Initialization of protocol instance.                         */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            ctx - Opaque context for callback calls                        */
/*            fn_tx_buffer - Callback function for send real data to checnal.*/
/*                           Provided by host application.                   */
/*            fn_rx_buffer - Callback function for process received packet.  */ 
/*            fn_tx_error - Callback function for transmit errors            */
/*            fn_rx_error - Callback function for receiving errors           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

void  egts_init(   
  egts_state_t* estate ,
  void* ctx ,
  int  (* fn_tx_buffer)( void* actx , void* pbuf , u32 sz ) ,
  int  (* fn_rx_packet)( void* actx , 
    egts_header_t*            pheader , 
    u8                        signed_up ,
    egts_responce_header_t*   presponce ,
    egts_record_t*            precords ,
    u16                       nrecords ,  
    u16   FDL ),
  void  (* fn_tx_error)( void* actx , u16 PID , u8 err , const char* dgb_str ),
  void  (* fn_rx_error)( void* actx , u16 PID , u8 err , const char* dgb_str )
  )
{

  if ( estate->cur_rx_packet ) 
  {
    estate->cur_rx_packet->rx_state = est_unknown;
    estate->cur_rx_packet->rx_state_len = 0U;
  }

  /* TODO: provosion for future */
  /*
  estate->cur_rx_packet = estate->rx_packets;
  estate->cur_tx_packet = estate->tx_packets;
  */

  estate->backbuf_head  = 0U;
  estate->backbuf_cur   = 0U;
  estate->backbuf_tail  = 0U;

  estate->ctx           = ctx;

  estate->rx_error_count  = 0;
  estate->rx_error_count  = 0;
  estate->last_rx_error   = EGTS_PC_OK;
  estate->last_tx_error   = EGTS_PC_OK;

  estate->fn_tx_buffer  = fn_tx_buffer;
  estate->fn_rx_packet  = fn_rx_packet;
  estate->fn_tx_error   = fn_tx_error;
  estate->fn_rx_error   = fn_rx_error;
 
}

/*****************************************************************************/
/*                                                                           */
/* egts_reset_errors()                                                       */
/*                                                                           */
/* Description: Reset error counters.                                        */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

void  egts_reset_errors( egts_state_t* estate )
{
  estate->rx_error_count  = 0;
  estate->rx_error_count  = 0;
  estate->last_rx_error   = EGTS_PC_OK;
  estate->last_tx_error   = EGTS_PC_OK;
}

/*****************************************************************************/
/*                                                                           */
/* egts_rx_error()                                                           */
/*                                                                           */
/* Description: Function for receiving error traceback.                      */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            PID - packet identifier                                        */
/*            err - error code                                               */
/*            dgb_str - optional debug string                                */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_rx_error( egts_state_t* estate , u16 PID , u8 err , const char* dgb_str )
{
  estate->rx_error_count++;
  estate->last_rx_error = err;
  if ( estate->fn_rx_error != NULL ) {
    estate->fn_rx_error( estate->ctx , PID , err , dgb_str );
  }
}

/*****************************************************************************/
/*                                                                           */
/* egts_tx_error()                                                           */
/*                                                                           */
/* Description: Function for transmit error traceback.                       */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            PID - packet identifier                                        */
/*            err - error code                                               */
/*            dgb_str - optional debug string                                */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_tx_error( egts_state_t* estate , u16 PID , u8 err , const char* dgb_str )
{
  estate->tx_error_count++;
  estate->last_tx_error = err;
  if ( estate->fn_tx_error != NULL ) {
    estate->fn_tx_error( estate->ctx , PID , err , dgb_str );
  }
}

/*****************************************************************************/
/*                                                                           */
/* egts_reset_state()                                                        */
/*                                                                           */
/* Description: Internal function for packet composer/decomposer             */
/*   state mashine                                                           */
/*                                                                           */
/* Arguments: epacket - packet state to reset                                */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_reset_state( egts_packet_state_t* epacket )
{
  epacket->nrecords              = 0U;
  epacket->nsubrecords_used      = 0U;
  epacket->nsubrecords_data_used = 0U;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_to_buf_le()                                                    */
/*                                                                           */
/* Description: Internal helper to write integer value to buffer with        */
/*   little-endian byte order                                                */
/*                                                                           */
/* Arguments: puv - pointer to value                                         */
/*            spuv - value size in bytes                                     */
/*            ppuc - pointer to buffer begin                                 */
/*            psz - buffer size                                              */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_write_to_buf_le( u8* puv , u16 spuv , u8** ppuc , u16* psz )
{
  for ( ; (spuv != 0U) && (spuv <= *psz); spuv-- )
  {
    *(*ppuc)++ = *puv++;
    (*psz)--;
  }
  if ( spuv != 0U ) {
    return -1;
  }
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_from_buf_le()                                                   */
/*                                                                           */
/* Description: Internal helper to read integer value from buffer with       */
/*   little-endian byte order                                                */
/*                                                                           */
/* Arguments: puv - pointer to value                                         */
/*            spuv - value size in bytes                                     */
/*            ppuc - pointer to buffer begin                                 */
/*            psz - buffer size                                              */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_read_from_buf_le( u8* puv , u16 spuv , u8** ppuc , u16* psz )
{
  for ( ; (spuv != 0U) && (spuv <= *psz); spuv-- )
  {
    *puv++ = *(*ppuc)++;
    (*psz)--;
  }
  if (spuv != 0U ) {
    return -1;
  }
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_write_to_buf_be()                                                    */
/*                                                                           */
/* Description: Internal helper to write integer value to buffer with        */
/*   big-endian byte order                                                   */
/*                                                                           */
/* Arguments: puv - pointer to value                                         */
/*            spuv - value size in bytes                                     */
/*            ppuc - pointer to buffer begin                                 */
/*            psz - buffer size                                              */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_write_to_buf_be( u8* puv , u16 spuv , u8** ppuc , u16* psz )
{
  for ( ; (spuv != 0U) && (spuv <= *psz); spuv-- )
  {
    *(*ppuc)++ = puv[ spuv - 1 ];
    (*psz)--;
  }
  if ( spuv != 0U ) {
    return -1;
  }
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_from_buf_be()                                                   */
/*                                                                           */
/* Description: Internal helper to read integer value from buffer with       */
/*   big-endian byte order                                                   */
/*                                                                           */
/* Arguments: puv - pointer to value                                         */
/*            spuv - value size in bytes                                     */
/*            ppuc - pointer to buffer begin                                 */
/*            psz - buffer size                                              */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_read_from_buf_be( u8* puv , u16 spuv , u8** ppuc , u16* psz )
{
  for ( ; (spuv != 0U) && (spuv <= *psz); spuv-- )
  {
    puv[ spuv - 1 ] = *(*ppuc)++;
    (*psz)--;
  }
  if (spuv != 0U ) {
    return -1;
  }
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_read_from_buf_be()                                                   */
/*                                                                           */
/* Description: Internal helper to read zero-terminated string from          */
/*   buffer with big-endian byte order                                       */
/*                                                                           */
/* Arguments: puv - pointer to string                                        */
/*            max_sz - value string length in bytes                          */
/*            ppuc - pointer to buffer begin                                 */
/*            psz - buffer size                                              */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

u16   egts_read_from_buf_z( u8* puv , u16 max_sz , u8** ppuc , u16* psz )
{
  u16 i;
  for ( i = 0U; (i < max_sz) && (*psz != 0U) && (**ppuc != 0U); i++ ){
    *puv++ = *(*ppuc)++;
    (*psz)--;
  }
  if ( *psz == 0U ){
    return 0xFFFFU;
  }
  *(*ppuc)++ = 0U; /* eat delimiter */
  (*psz)--;
  return i;
}

/*****************************************************************************/
/*                                                                           */
/* egts_tx_byte()                                                            */
/*                                                                           */
/* Description: Internal helper for memory managerment                       */
/*                                                                           */
/* Arguments: epacket - packet state                                         */
/*            sz - size of bufer requested                                   */
/*                                                                           */
/* Return:    pointer to reserved buffer                                     */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/


void*   egts_acquire_subrecord_buffer( egts_packet_state_t* epacket , u16 sz )
{
  u16  p = epacket->nsubrecords_data_used;
#ifdef EGTS_ALIGN
  p = ( p | EGTS_ALIGN_MASK ) + 1;
#endif  
  if ( p + sz >= EGTS_MAX_SFDR_LEN ) {
    return NULL;
  }
  epacket->nsubrecords_data_used = p + sz;
  return epacket->subrecords_data + p;
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_header_bits()                                                    */
/*                                                                           */
/* Description: Helper function for collect Transport Header bitfields       */
/*                                                                           */
/* Arguments: pheader - transport header                                     */
/*                                                                           */
/* Return:    byte with header bitfileds                                     */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

u8    egts_get_header_bits( egts_header_t* pheader )
{
  u8 b = 0U;
  b |= ( pheader->PRF & 0x03U );    
  b <<= 1U;
  b |= ( pheader->RTE & 0x01U );   
  b <<= 2U;
  b |= ( pheader->ENA & 0x03U );   
  b <<= 1U;  
  b |= ( pheader->CMP & 0x01U );   
  b <<= 2U;  
  b |= ( pheader->PR & 0x03U );  
  return b;
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_header_bits()                                                    */
/*                                                                           */
/* Description: Helper function for set up Transport Header bitfields        */
/*                                                                           */
/* Arguments: pheader - transport header                                     */
/*            b - bitfields byte                                             */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_set_header_bits( egts_header_t* pheader , u8 b )
{
  pheader->PR   = b & 0x03U;   
  b >>= 2U;
  pheader->CMP  = b & 0x01U;   
  b >>= 1U;
  pheader->ENA  = b & 0x03U;  
  b >>= 2U;  
  pheader->RTE  = b & 0x01U;   
  b >>= 1U;  
  pheader->PRF  = b & 0x03U;
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_record_bits()                                                    */
/*                                                                           */
/* Description: Helper function for collect Record bitfields                 */
/*                                                                           */
/* Arguments: precord - record                                               */
/*                                                                           */
/* Return:    byte with header bitfileds                                     */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

u8    egts_get_record_bits( egts_service_data_record_t* precord )
{
  u8 b = 0U;
  b |= ( precord->SSOD & 0x01U );    
  b <<= 1U;
  b |= ( precord->RSOD & 0x01U );    
  b <<= 1U;
  b |= ( precord->GRP & 0x01U );     
  b <<= 2U;
  b |= ( precord->RPP & 0x03U );     
  b <<= 1U;
  b |= ( precord->TMFE & 0x01U );    
  b <<= 1U;
  b |= ( precord->EVFE & 0x01U );    
  b <<= 1U;
  b |= ( precord->OBFE & 0x01U );  
  return b;  
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_record_bits()                                                    */
/*                                                                           */
/* Description: Helper function for set up Record bitfields                  */
/*                                                                           */
/* Arguments: precord - record                                               */
/*            b - bitfields byte                                             */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_set_record_bits( egts_service_data_record_t* precord , u8 b )
{
  precord->OBFE  = b & 0x01U;   
  b >>= 1U;
  precord->EVFE  = b & 0x01U;   
  b >>= 1U;
  precord->TMFE  = b & 0x01U;   
  b >>= 1U;  
  precord->RPP   = b & 0x03U;   
  b >>= 2U;  
  precord->GRP   = b & 0x01U;   
  b >>= 1U;  
  precord->RSOD  = b & 0x01U;   
  b >>= 1U;  
  precord->SSOD  = b & 0x01U;  
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_msd_coo()                                                        */
/*                                                                           */
/* Description: Helper function to set EGTS coordinate fileds to             */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: deg - degrees with sign                                        */
/*            n - arc minutes, 0..59                                         */
/*            s - arc seconds, 0..59                                         */
/*            ms - mili arc seconds, 0..1000                                 */
/*                                                                           */
/* Return:    EGTS MSG representaion of coordinate                           */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

u32   egts_set_msd_coo( i16 deg , u8 n, u8 s , u16 ms )
{
  if ( deg >= 0 ) {
    return  (u32)deg * 3600000U + (u32)n * 60000U + (u32)s * 1000U + (u32)ms;
  }
  return ~( (u32)(-deg) * 3600000U + (u32)n * 60000U + (u32)s * 1000U + (u32)ms ) + 1U;
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_msd_coo()                                                        */
/*                                                                           */
/* Description: Helper function to get EGTS coordinate fileds from           */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: coo - EGTS MSD representation of coordinate                    */  
/*            pdeg - pointer to get degrees with sign                        */
/*            pn - pointer to get arc minutes, 0..59                         */
/*            ps - pointer to get arc seconds, 0..59                         */
/*            pms - pointer to get mili arc seconds, 0..1000                 */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

void  egts_get_msd_coo( u32 coo , i16 *pdeg , u8 *pn, u8 *ps , u16 *pms )
{
  u8 neg = 0;
  if ( 0 != ( coo & 0x80000000U ) ) {
    neg =1;
    coo = ~coo + 1;
  }
  *pms = (u16)( coo % 1000U );
  coo /= 1000;
  *ps = (u8)( coo % 60 );
  coo /= 60;
  *pn = (u8)( coo % 60 );
  coo /= 60;
  if ( neg ) {
    *pdeg = -(i16)coo;
  } else {
    *pdeg = (i16)coo;
  }
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_msd_coo16()                                                      */
/*                                                                           */
/* Description: Helper function to set EGTS relative coordinate fileds to    */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: n - arc minutes, 0..59                                         */
/*            s - arc seconds, 0..59                                         */
/*            ms - mili arc seconds, 0..1000                                 */
/*                                                                           */
/* Return:    EGTS MSG representaion of relative coordinate                  */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

u16   egts_set_msd_coo16( i8 n, u8 s , u16 ms )
{
  if ( n >= 0 ) {
    return (u16)( (u32)n * 600U + (u32)s * 10U + (u32)ms/100 );
  } 
  return  ~(u16)( (u32)-n * 600U + (u32)s * 10U + (u32)ms/100 ) + 1;
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_msd_coo16()                                                      */
/*                                                                           */
/* Description: Helper function to get EGTS relative coordinate fileds from  */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: coo - EGTS MSD representation of relative coordinate           */  
/*            pn - pointer to get arc minutes, 0..59                         */
/*            ps - pointer to get arc seconds, 0..59                         */
/*            pms - pointer to get mili arc seconds, 0..1000                 */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

void  egts_get_msd_coo16( u16 coo , i8 *pn, u8 *ps , u16 *pms )
{
  u8 neg = 0;
  if ( 0 != ( coo & 0x8000U ) ) {
    neg = 1;
    coo = ~coo + 1;
  }
  *pms = (u16)( coo % 10U ) * 100;
  coo /= 10;
  *ps = (u8)( coo % 60 );
  coo /= 60;
  if ( neg ) {
    *pn = -(i8)coo;
  } else {
    *pn = (i8)coo;
  }
}

/******************************************************************************
*
*/

