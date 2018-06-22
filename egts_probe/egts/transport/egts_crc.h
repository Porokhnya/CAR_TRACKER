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
/* Description: Check sum routines                                           */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: egts_header_crc                                                */
/*            egts_data_crc                                                  */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_crc_h
#define egts_crc_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/* egts_header_crc()                                                         */
/*                                                                           */
/* Description: Calculates transport header CRC                              */
/*                                                                           */
/* Arguments: phead - transport header                                       */
/*            pecnoded_part - encoded part of header                         */
/*            encoded_size - encoded part size                               */
/*                                                                           */
/* Return:    CRC                                                            */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern u8    egts_header_crc ( egts_header_t* phead , void* pecnoded_part , u8 encoded_size );

/*****************************************************************************/
/*                                                                           */
/* egts_data_crc()                                                           */
/*                                                                           */
/* Description: Calculates frame data CRC                                    */
/*                                                                           */
/* Arguments: pbuf - frame data                                              */
/*            sz - frame data size                                           */
/*                                                                           */
/* Return:    CRC                                                            */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern u16   egts_data_crc   ( void* pbuf , u16 sz );

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



