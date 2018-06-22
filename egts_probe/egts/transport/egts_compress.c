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
/*	                                                                     */
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
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"
#include "../include/egts.h"

#include "egts_compress.h"

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

int   egts_compress_data( void* cmpr_buf , void* source_buf , u16 source_size , u16 *pcmps_size )
{
#ifdef EGTS_DBG
  u8* pcs = (u8*)source_buf;
  u8* pct = (u8*)cmpr_buf;
  u16 i;
  /* just gamma by 0xA5 */
  for ( i = 0U; i < source_size; i++ ) {
    *pct++ = *pcs++ ^ 0xA5U;
  }
  *pcmps_size = source_size;
  return 0;
#else
  egts_dbg_printf("unsupported data compression")
  return -1;
#endif
}

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

int   egts_decompress_data( void* target_buf , void* cmpr_buf ,  u16 cmps_size , u16 *ptarget_size )
{
#ifdef EGTS_DBG
  u8* pcs = (u8*)cmpr_buf;
  u8* pct = (u8*)target_buf;
  u16 i;
  /* just gamma by 0xA5 */
  for ( i = 0U; i < cmps_size; i++ ) {
    *pct++ = *pcs++ ^ 0xA5U;
  }
  *ptarget_size = cmps_size;
  return 0;
#else
  egts_dbg_printf("unsupported data compression")
  return -1;
#endif
}

/******************************************************************************
*
*/


