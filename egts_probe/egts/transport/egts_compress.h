/*****************************************************************************/
/*                                                                           */
/* File: egts_compress.c                                                     */
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
/* Description: Functions for compressing/decompressing data in EGTS         */ 
/*   Unimplemented properly yet, till real compressing algorithm will de     */
/*   clarified. For debug purpos, simple gamming algorith implemented under  */
/*   EGTS_DBG macro.                                                         */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: egts_compress_data                                             */
/*            egts_decompress_data                                           */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_compress_h
#define egts_compress_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/* egts_compress_data()                                                      */
/*                                                                           */
/* Description: Function for compressing packet frame data                   */
/*                                                                           */
/* Arguments: cmpr_buf - target buffer for compressed data                   */
/*            source_buf - source buffer                                     */
/*            source_size - source buffer size                               */
/*            pcmps_size - resulting compressed size                         */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_compress_data( void* cmpr_buf , void* source_buf , 
  u16 source_size , u16 *pcmps_size );

/*****************************************************************************/
/*                                                                           */
/* egts_decompress_data()                                                    */
/*                                                                           */
/* Description: Function for decompressing packet frame data                 */
/*                                                                           */
/* Arguments: target_buf - target buffer for decompressed data               */
/*            cmpr_buf - buffer with source compressed data                  */
/*            cmps_size - size of source compressed data                     */
/*            ptarget_size - resulting uncompressed data size                */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_decompress_data( void* target_buf , void* cmpr_buf ,  
  u16 cmps_size , u16 *ptarget_size );

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

