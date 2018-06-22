/*****************************************************************************/
/*                                                                           */
/* File: egts_impl.h                                                         */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Protocol EGTS                                             */
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
/* Description: This header contains some privat declaration for             */
/*   protocol stack implementation.                                          */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: public :                                                       */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_guts_h
#define egts_guts_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

/******************************************************************************
*
*/

/* TODO: provision for future */
#define EGTS_TX_QUEUE_SIZE   1
#define EGTS_RX_QUEUE_SIZE   1

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

extern void  egts_reset_state( egts_packet_state_t* epacket );

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

extern void  egts_rx_error( egts_state_t* estate , u16 PID , u8 err , const char* dgb_str );

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

extern void  egts_tx_error( egts_state_t* estate , u16 PID , u8 err , const char* dgb_str );

/*****************************************************************************/
/*                                                                           */
/* egts_tx_raw_packet()                                                      */
/*                                                                           */
/* Description: Internal function for transmit packet.                       */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            pprofile - transmit settings                                   */
/*            PR - packet priority                                           */
/*            proute - packet routing settings                               */
/*            PID - packet identifier                                        */
/*            presponce - optional response data                             */
/*            pFD - packet frame data                                        */
/*            FDL - packet frame data length                                 */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_tx_raw_packet( egts_state_t* estate ,
  egts_profile_t*         pprofile ,
  u8                      PR ,
  egts_route_t*           proute ,
  u16                     PID,
  egts_responce_header_t* presponce ,
  void*                   pFD,
  u16                     FDL
  );

/*****************************************************************************/
/*                                                                           */
/* egts_packet_rx_byte()                                                     */
/*                                                                           */
/* Description: Function for passing received data to protocol.              */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            epacket - current packet                                       */
/*            uc - received byte                                             */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

void  egts_packet_rx_byte( 
  egts_state_t*        estate ,
  egts_packet_state_t* epacket , 
  u8 uc );

/*****************************************************************************/
/*                                                                           */
/* egts_proceed_rx_packet()                                                  */
/*                                                                           */
/* Description: Function for proceed received packet with raw frame data.    */
/*   Decomposes frame data to records/subrecords                             */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            epacket - packet state mashine                                 */
/*            pheader - packet transport header                              */
/*            pFD - packet frame data                                        */
/*            FDL - packet frame data length                                 */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int  egts_proceed_rx_packet( 
   egts_state_t*            estate ,
   egts_packet_state_t*     epacket ,
   egts_header_t*           pheader ,
   void* pFD ,
   u16   FDL );

/*****************************************************************************/
/*                                                                           */
/* egts_tx_byte()                                                            */
/*                                                                           */
/* Description: Internal wrapper for transmit                                */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            uc - byte to transmit                                          */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_tx_byte( egts_state_t* estate , u8 uc );

/*****************************************************************************/
/*                                                                           */
/* egts_tx_byte()                                                            */
/*                                                                           */
/* Description: Internal wrapper for transmit                                */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            pbuf - buffer to transmit                                      */
/*            sz - buffer size                                               */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_tx_buffer( egts_state_t* estate , void* pbuf , u32 sz );

/******************************************************************************
*
*/

#ifdef EGTS_CPU_LE
#define EGTS_WRITE_FIELD(pv,uc,ofs,sz)   do { ((u8*)(pv))[(ofs)++]=(uc); } while (0)
#define EGTS_READ_FIELD(pv,uc,ofs,sz)   do { (uc)=((u8*)(pv))[(ofs)++]; } while (0)
#elif  EGTS_CPU_BE
#define EGTS_WRITE_FIELD(pv,uc,ofs,sz)   do { ((u8*)(pv))[(sz)-1-((ofs)++)]=(uc) } while (0)
#define EGTS_READ_FIELD(pv,uc,ofs,sz)   do { (uc)=((u8*)(pv))[(sz)-1-((ofs)++)] } while (0)
#else
#endif

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

extern int   egts_write_to_buf_le ( u8* puv , u16 spuv , u8** ppuc , u16* psz );

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

extern int   egts_read_from_buf_le( u8* puv , u16 spuv , u8** ppuc , u16* psz );

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

extern int   egts_write_to_buf_be ( u8* puv , u16 spuv , u8** ppuc , u16* psz );

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

extern int   egts_read_from_buf_be( u8* puv , u16 spuv , u8** ppuc , u16* psz );

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

extern u16   egts_read_from_buf_z( u8* puv , u16 max_sz , u8** ppuc , u16* psz );

/*****************************************************************************
*
*/

#define EGTS_READ_Z(pt,l,mx,p,s)      { (l) = egts_read_from_buf_z((pt),(mx),&(p),&(s)); \
  if ( ((l) == 0xFFFFU) || ((l) > (mx)) ) \
    {  return -1; }\
    (pt)[(l)] = 0U; \
  }
#define EGTS_WRITE_Z(v,z,p,s)         { if (0 != egts_write_to_buf_le((v),(z),&(p),&(s))) { return -1; }\
                                        if ((s)>0U) { *(p)++ = (u8)0; (s)--; } else { return -1; } }
#ifdef EGTS_CPU_LE
#define egts_read_from_buf(puv,spuv,ppuc,psz)  egts_read_from_buf_le((puv),(spuv),(ppuc),(psz))
#define egts_write_to_buf(puv,spuv,ppuc,psz)   egts_write_to_buf_le((puv),(spuv),(ppuc),(psz))
#define EGTS_READ(v,p,s)              if (0 != egts_read_from_buf_le((u8*)&(v),(u16)sizeof(v),&(p),&(s))) { return -1; }
#define EGTS_WRITE(v,p,s)             if (0 != egts_write_to_buf_le((u8*)&(v),(u16)sizeof(v),&(p),&(s))) { return -1; }
#define EGTS_READ_BE(v,p,s)              if (0 != egts_read_from_buf_be((u8*)&(v),(u16)sizeof(v),&(p),&(s))) { return -1; }
#define EGTS_WRITE_BE(v,p,s)             if (0 != egts_write_to_buf_be((u8*)&(v),(u16)sizeof(v),&(p),&(s))) { return -1; }
#elif EGTS_CPU_BE
#define egts_read_from_buf(puv,spuv,ppuc,psz)  egts_read_from_buf_be((puv),(spuv),(ppuc),(psz))
#define egts_write_to_buf(puv,spuv,ppuc,psz)   egts_write_to_buf_be((puv),(spuv),(ppuc),(psz))
#define EGTS_READ(v,p,s)              if (0 != egts_read_from_buf_be((u8*)&(v),(u16)sizeof(v),&(p),&(s))) { return -1; }
#define EGTS_WRITE(v,p,s)             if (0 != egts_write_to_buf_be((u8*)&(v),(u16)sizeof(v),&(p),&(s))) { return -1; }
#define EGTS_READ_BE(v,p,s)              if (0 != egts_read_from_buf_le((u8*)&(v),(u16)sizeof(v),&(p),&(s))) { return -1; }
#define EGTS_WRITE_BE(v,p,s)             if (0 != egts_write_to_buf_le((u8*)&(v),(u16)sizeof(v),&(p),&(s))) { return -1; }
#endif


#define EGTS_READ_RAW(v,z,p,s)        if (0 != egts_read_from_buf_le((v),(z),&(p),&(s))) { return -1; }
#define EGTS_WRITE_RAW(v,z,p,s)       if (0 != egts_write_to_buf_le((v),(z),&(p),&(s))) { return -1; }

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

extern  void*  egts_acquire_subrecord_buffer( egts_packet_state_t* epacket , u16 sz );

/*****************************************************************************/
/*                                                                           */
/* egts_proceed_rx_record()                                                  */
/*                                                                           */
/* Description: Record parser                                                */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            epacket - packet state mashine                                 */
/*            precord - resulting record                                     */
/*            ppbuf - raw buffer to parse                                    */
/*            pbuf_sz - raw buffer length                                    */
/*                                                                           */
/* Return:    zero on OK, nonzero value otherwise                            */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int  egts_proceed_rx_record(
  egts_state_t*         estate ,
  egts_packet_state_t*  epacket ,
  egts_record_t*        precord ,
  u8**  ppbuf ,
  u16*  pbuf_sz );

/*****************************************************************************/
/*                                                                           */
/* egts_proceed_rx_subrecord()                                               */
/*                                                                           */
/* Description: Subrecord parser                                             */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            epacket - packet state mashine                                 */
/*            precord - record for contained subrecords                      */
/*            ppbuf - raw buffer to parse                                    */
/*            pbuf_sz - raw buffer length                                    */
/*                                                                           */
/* Return:    zero on OK, nonzero value otherwise                            */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int  egts_proceed_rx_subrecord(
  egts_state_t*         estate ,
  egts_packet_state_t*  epacket ,
  egts_record_t*        precord ,
  u8**  ppbuf ,
  u16*  pbuf_sz );

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

extern u8    egts_get_header_bits( egts_header_t* pheader );

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

extern void  egts_set_header_bits( egts_header_t* pheader , u8 b );

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

extern u8    egts_get_record_bits( egts_service_data_record_t* precord );

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

extern void  egts_set_record_bits( egts_service_data_record_t* precord , u8 b );

/******************************************************************************
*
*/

#ifdef  __cplusplus
}
#endif

/******************************************************************************
*
*/

#endif


