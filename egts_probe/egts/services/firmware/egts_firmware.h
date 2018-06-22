/*****************************************************************************/
/*                                                                           */
/* File: egts_firmware.h                                                     */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service FIRMWARE                                          */
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
/* Description: Parser/Composer/Debuger entries for service FIRMWARE         */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_firmware_h
#define egts_firmware_h

/******************************************************************************
*
*/

extern const egts_subrecord_handler_t  chandler_FIRMWARE_PART_DATA;
extern const egts_subrecord_handler_t  chandler_FIRMWARE_FULL_DATA;

/******************************************************************************
*
*/

extern const egts_service_handler_t    cservice_FIRMWARE;

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

extern  u8    egts_get_FIRMWARE_bitfields( egts_FIRMWARE_DATA_header_t* ps );

extern void  egts_set_FIRMWARE_bitfields( egts_FIRMWARE_DATA_header_t* ps , u8 b );

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




