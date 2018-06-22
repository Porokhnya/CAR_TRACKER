/*****************************************************************************/
/*                                                                           */
/* File: egts_header_ncode.c                                                 */
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
/* Description: Functions for encoding/decoding packet header in EGTS.       */ 
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
#include "../include/egts_impl.h"
#include "egts_guts.h"

#include "egts_header_encode.h"

/*****************************************************************************/
/*                                                                           */
/* egts_header_encode()                                                      */
/*                                                                           */
/* Description: Produce encoded part of transport header according with      */
/*   HE field of header                                                      */
/*                                                                           */
/* Arguments: phead - transport header                                       */
/*            pecnoded_part - encoded part of header                         */
/*            encoded_size - encoded part size                               */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_header_encode( egts_header_t* phead , void* pecnoded_part , u8* pencoded_size  )
{

  *pencoded_size = (u8)0;
  if ( phead->HE == EGTS_HEADER_ENCODING_NONE ) {
    return egts_header_encode_plain( phead , pecnoded_part , pencoded_size );
  }
#ifdef EGTS_DBG
  if ( phead->HE == EGTS_HEADER_ENCODING_DBG ) {
    return egts_header_encode_dbg( phead , pecnoded_part , pencoded_size  );
  }
#endif
  egts_dbg_printf("unsupported header encoding");
  return -1;
}

/*****************************************************************************/
/*                                                                           */
/* egts_header_decode()                                                      */
/*                                                                           */
/* Description: Decodes transport header according with                      */
/*   HE field of header                                                      */
/*                                                                           */
/* Arguments: phead - transport header                                       */
/*            pecnoded_part - encoded part of header                         */
/*            encoded_size - encoded part size                               */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_header_decode( egts_header_t* phead , void* pecnoded_part , u8 encoded_size  )
{

  if ( phead->HE == EGTS_HEADER_ENCODING_NONE ) {
    return    egts_header_decode_plain( phead , pecnoded_part , encoded_size );
  }
#ifdef EGTS_DBG
  if ( phead->HE == EGTS_HEADER_ENCODING_DBG ) {
    return egts_header_decode_dbg( phead , pecnoded_part , encoded_size  );
  }
#endif
  egts_dbg_printf("unsupported header encoding");
  return -1;
}

/*****************************************************************************/
/*                                                                           */
/* egts_header_encode_plain()                                                */
/*                                                                           */
/* Description: Produce encoded part of transport header with no encoding    */
/*   algorithm (when HE=0)                                                   */
/*                                                                           */
/* Arguments: phead - transport header                                       */
/*            pecnoded_part - encoded part of header                         */
/*            encoded_size - encoded part size                               */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_header_encode_plain( egts_header_t* phead , void* pecnoded_part , u8* pencoded_size  )
{
  u8* puc = (u8*)pecnoded_part;
  u16 sz = EGTS_HEADER_LEN_ENCODED_MAX_LEN; 
  
  *pencoded_size = (u8)0;

  EGTS_WRITE(phead->FDL,puc,sz)
  EGTS_WRITE(phead->PID,puc,sz)
  EGTS_WRITE(phead->PT,puc,sz)
  if ( phead->RTE != 0U )
  {
    EGTS_WRITE(phead->PRA,puc,sz)
    EGTS_WRITE(phead->RCA,puc,sz)
    EGTS_WRITE(phead->TTL,puc,sz)
  }

  *pencoded_size = (u8)(EGTS_HEADER_LEN_ENCODED_MAX_LEN - sz);
  return 0;

}

/*****************************************************************************/
/*                                                                           */
/* egts_header_decode_plain()                                                */
/*                                                                           */
/* Description: Decodes transport header with no encoding                    */
/*   algorithm (when HE=0)                                                   */
/*                                                                           */
/* Arguments: phead - transport header                                       */
/*            pecnoded_part - encoded part of header                         */
/*            encoded_size - encoded part size                               */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_header_decode_plain( egts_header_t* phead , void* pecnoded_part , u8 encoded_size  )
{
  u8* puc = (u8*)pecnoded_part;
  u16 sz = (u16)encoded_size;

  EGTS_READ(phead->FDL,puc,sz)
  EGTS_READ(phead->PID,puc,sz)
  EGTS_READ(phead->PT,puc,sz)
  if ( phead->RTE != 0U )
  {
    EGTS_READ(phead->PRA,puc,sz)
    EGTS_READ(phead->RCA,puc,sz)
    EGTS_READ(phead->TTL,puc,sz)
  }

  if ( sz != 0U ) {
    return -1;
  }
  return 0;

}

/******************************************************************************
*
*/

#ifdef EGTS_DBG

/*****************************************************************************/
/*                                                                           */
/* egts_header_encode_plain()                                                */
/*                                                                           */
/* Description: Produce encoded part of transport header with debug          */
/*   algorithm                                                               */
/*                                                                           */
/* Arguments: phead - transport header                                       */
/*            pecnoded_part - encoded part of header                         */
/*            encoded_size - encoded part size                               */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_header_encode_dbg( egts_header_t* phead , void* pecnoded_part , u8* pencoded_size  )
{
  u8* puc = (u8*)pecnoded_part;
  u16 sz = EGTS_HEADER_LEN_ENCODED_MAX_LEN; 
  u8 i,szc;
  
  *pencoded_size = 0U;

  EGTS_WRITE(phead->FDL,puc,sz)
  EGTS_WRITE(phead->PID,puc,sz)
  EGTS_WRITE(phead->PT,puc,sz)
  if ( phead->RTE != 0U )
  {
    EGTS_WRITE(phead->PRA,puc,sz)
    EGTS_WRITE(phead->RCA,puc,sz)
    EGTS_WRITE(phead->TTL,puc,sz)
  }

  szc = (u8)( EGTS_HEADER_LEN_ENCODED_MAX_LEN - sz );
  /* just gamma by 0x81 and add 2 bytes of spare */
  for ( i = 0U; i < szc; i++ ) {
    ((u8*)pecnoded_part)[i] ^= 0x81U;
  }
  *puc++ = (u8)0xAAU;
  *puc++ = (u8)0xABU;
  szc += 2U;

  *pencoded_size = (u8)szc;
  return 0;
}

/*****************************************************************************/
/*                                                                           */
/* egts_header_decode_dbg()                                                  */
/*                                                                           */
/* Description: Decodes transport header with debug encoding                 */
/*   algorithm                                                               */
/*                                                                           */
/* Arguments: phead - transport header                                       */
/*            pecnoded_part - encoded part of header                         */
/*            encoded_size - encoded part size                               */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int   egts_header_decode_dbg( egts_header_t* phead , void* pecnoded_part , u8  encoded_size  )
{
  u8* puc = (u8*)pecnoded_part;
  u16 i,szc,sz = (u16)encoded_size;
  
  szc = (u16)encoded_size - 2U;
  /* just gamma by 0x81 and add 2 bytes of spare */
  for ( i = 0U; i < szc; i++ ) {
    ((u8*)pecnoded_part)[i] ^= 0x81U;
  }

  EGTS_READ(phead->FDL,puc,sz)
  EGTS_READ(phead->PID,puc,sz)
  EGTS_READ(phead->PT,puc,sz)
  if ( phead->RTE != 0U )
  {
    EGTS_READ(phead->PRA,puc,sz)
    EGTS_READ(phead->RCA,puc,sz)
    EGTS_READ(phead->TTL,puc,sz)
  }

  if ( sz != 2U ) {
    return -1;
  }
  return 0;
}

#endif

/******************************************************************************
*
*/
