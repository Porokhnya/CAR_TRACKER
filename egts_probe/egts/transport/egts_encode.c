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
/*	                                                                     */
/* (c) Copyright JSC «Navigation-information systems», 2011                  */
/*                                                                           */
/* Address:                                                                  */
/*     24, Mishina Str., bld.1                                               */
/*     Moscow, Russia                                                        */
/*                                                                           */
/* Description: Functions for encoding/decoding data in EGTS.                */ 
/*   Unimplemented properly yet, till real encode algorithm will de          */
/*   clarified. For debug purpos, simple gamming algorith implemented under  */
/*   EGTS_DBG macro.                                                         */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions:                                                                */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"

#include "../include/egts.h"

#include "egts_encode.h"

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

int   egts_encode_data( u8 SKID , void* pencoded_buf , void* source_buf , u16 source_size , u16 *pencoded_size )
{
#ifdef EGTS_DBG
  return egts_encode_data_dbg( SKID , pencoded_buf , source_buf , source_size , pencoded_size );
#else
  egts_dbg_printf("unsupported data encoding")
  *pencoded_size = source_size;
  return -1;
#endif
}

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

int   egts_decode_data( u8 SKID , void* target_buf , void* encoded_buf ,  u16 encoded_size , u16 *ptarget_size )
{

#ifdef EGTS_DBG
  return egts_decode_data_dbg( SKID , target_buf , encoded_buf , encoded_size , ptarget_size );
#else
  egts_dbg_printf("unsupported data encoding")
  *ptarget_size = encoded_size;
  return -1;
#endif
}

/******************************************************************************
*
*/

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

int   egts_encode_data_dbg(  u8 SKID , void* pencoded_buf , void* source_buf , u16 source_size , u16 *pencoded_size )
{
  u8* pcs = (u8*)source_buf;
  u8* pct = (u8*)pencoded_buf;
  u16 i;

  (void)SKID;

  /* just gamma by 0x7F and add 8 bytes of spare at head */
  for ( i = 0U; i < 8U; i++ ) {
    *pct++ = (u8)'0'+(u8)i;
  }
  for ( i = 0U; i < source_size; i++ ) {
    *pct++ = *pcs++ ^ 0x7FU; 
  }
  *pencoded_size = 8U + source_size;
  return 0;

}

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

int   egts_decode_data_dbg(  u8 SKID , void* target_buf , void* encoded_buf ,  u16 encoded_size , u16 *ptarget_size )
{
  u8* pcs = (u8*)encoded_buf;
  u8* pct = (u8*)target_buf;
  u16 i;

  (void)SKID;

  if ( encoded_size < 8U ) {
    return -1;
  }
  for ( i = 0U; i < 8U; i++ )
  {
    if ( *pcs++ != (u8)'0'+(u8)i ) {
      return -1;
    }
  }
  encoded_size -= 8U;
  for ( i = 0U; i < encoded_size; i++ ) {
    *pct++ = *pcs++ ^ 0x7FU;
  }

  *ptarget_size = encoded_size;
  return 0;
}

#endif

/******************************************************************************
*
*/

