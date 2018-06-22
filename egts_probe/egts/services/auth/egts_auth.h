/*****************************************************************************/
/*                                                                           */
/* File: egts_auth.h                                                         */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service AUTH                                              */
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
/* Description: Parser/Composer/Debuger entries for service AUTH,            */
/*   declarations.                                                           */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: -                                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_auth_h
#define egts_auth_h

/******************************************************************************
*
*/

extern const egts_subrecord_handler_t  chandler_AUTH_AUTH_INFO;
extern const egts_subrecord_handler_t  chandler_AUTH_AUTH_PARAMS;
extern const egts_subrecord_handler_t  chandler_AUTH_TERM_MODULE_DATA;
extern const egts_subrecord_handler_t  chandler_AUTH_TERM_IDENTITY;
extern const egts_subrecord_handler_t  chandler_AUTH_VEHICLE_DATA;
extern const egts_subrecord_handler_t  chandler_AUTH_SERVICE_INFO;
extern const egts_subrecord_handler_t  chandler_AUTH_RESULT_CODE;

/******************************************************************************
*
*/

extern const egts_service_handler_t    cservice_AUTH;

/******************************************************************************
*
*/

#endif


