/*****************************************************************************/
/*                                                                           */
/* File: egts_crc.h                                                          */
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
/* Description: Header encoding routines                                     */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: egts_header_encode                                             */
/*            egts_header_decode                                             */
/*            egts_header_encode_plain                                       */
/*            egts_header_encode_dbg                                         */
/*            egts_header_decode_dbg                                         */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_header_encode_h
#define egts_header_encode_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

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

extern int   egts_header_encode( egts_header_t* phead , 
  void* pecnoded_part , u8* pencoded_size  );

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

extern int   egts_header_decode( egts_header_t* phead , 
  void* pecnoded_part , u8 encoded_size  );

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

extern int   egts_header_encode_plain( egts_header_t* phead , 
  void* pecnoded_part , u8* pencoded_size  );

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

extern int   egts_header_decode_plain( egts_header_t* phead , 
  void* pecnoded_part , u8 encoded_size  );


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

extern int   egts_header_encode_dbg( egts_header_t* phead , 
  void* pecnoded_part , u8* pencoded_size  );

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

extern int   egts_header_decode_dbg( egts_header_t* phead , 
  void* pecnoded_part , u8 encoded_size  );

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


