/*****************************************************************************/
/*                                                                           */
/* File: egts_ecall.h                                                        */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service ECALL                                             */
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
/* Description: Parser/Composer/Debuger entries for service ECALL            */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_ecall_h
#define egts_ecall_h

/******************************************************************************
*
*/

extern const egts_subrecord_handler_t  chandler_ECALL_ACCEL_DATA;
extern const egts_subrecord_handler_t  chandler_ECALL_MSD_DATA;
extern const egts_subrecord_handler_t  chandler_ECALL_RAW_MSD_DATA;
extern const egts_subrecord_handler_t  chandler_ECALL_TRACK_DATA;

/******************************************************************************
*
*/

extern const egts_service_handler_t    cservice_ECALL;

/******************************************************************************
*
*/


#endif




