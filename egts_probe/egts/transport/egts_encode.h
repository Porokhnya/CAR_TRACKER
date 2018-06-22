/*****************************************************************************/
/*                                                                           */
/* File: egts_encode.c                                                       */
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
/* Description: Functions for encoding/decoding frame data in EGTS.          */ 
/*   Unimplemented properly yet, till real encode algorithm will de          */
/*   clarified. For debug purpos, simple gamming algorith implemented under  */
/*   EGTS_DBG macro.                                                         */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: egts_encode_data                                               */
/*            egts_decode_data                                               */
/*            egts_encode_data_dbg                                           */
/*            egts_decode_data_dbg                                           */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_encode_h
#define egts_encode_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/* egts_encode_data()                                                        */
/*                                                                           */
/* Description: Encodes packet frame data (do nothing in current             */
/*   version)                                                                */
/*                                                                           */
/* Arguments: SKID - security key ID                                         */
/*            pencoded_buf - buffer for encoded frame data                   */
/*            source_buf - source buffer with plain data                     */
/*            source_size - source buffer size                               */
/*            pencoded_size - resulting encoded buffer size                  */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_encode_data( u8 SKID , void* pencoded_buf , 
  void* source_buf , u16 source_size , u16 *pencoded_size );

/*****************************************************************************/
/*                                                                           */
/* egts_decode_data()                                                        */
/*                                                                           */
/* Description: Decodes packet frame data (do nothing in current version)    */
/*   algorithm                                                               */
/*                                                                           */
/* Arguments: SKID - security key ID                                         */
/*            target_buf - resulting plain frame data buffer                 */
/*            encoded_buf - source encoded data                              */
/*            encoded_size - source encoded buffer size                      */
/*            ptarget_size - resulting plain data size                       */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_decode_data( u8 SKID , void* target_buf , 
  void* encoded_buf ,  u16 encoded_size , u16 *ptarget_size );


#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_encode_data_dbg()                                                    */
/*                                                                           */
/* Description: Encodes packet frame data with debug algorithm.              */
/*   For debug only.                                                         */
/*                                                                           */
/* Arguments: SKID - security key ID                                         */
/*            pencoded_buf - buffer for encoded frame data                   */
/*            source_buf - source buffer with plain data                     */
/*            source_size - source buffer size                               */
/*            pencoded_size - resulting encoded buffer size                  */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_encode_data_dbg( u8 SKID , void* pencoded_buf , 
  void* source_buf , u16 source_size , u16 *pencoded_size );

/*****************************************************************************/
/*                                                                           */
/* egts_decode_data_dbg()                                                    */
/*                                                                           */
/* Description: Decodes packet frame data with debug algorithm.              */
/*   For debug only.                                                         */ 
/*                                                                           */
/* Arguments: SKID - security key ID                                         */
/*            target_buf - resulting plain frame data buffer                 */
/*            encoded_buf - source encoded data                              */
/*            encoded_size - source encoded buffer size                      */
/*            ptarget_size - resulting plain data size                       */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_decode_data_dbg( u8 SKID , void* target_buf , 
  void* encoded_buf ,  u16 encoded_size , u16 *ptarget_size );

/******************************************************************************
*
*/

#endif

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

