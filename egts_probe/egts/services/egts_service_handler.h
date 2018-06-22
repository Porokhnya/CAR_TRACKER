/*****************************************************************************/
/*                                                                           */
/* File: egts_service_handler.h                                              */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service Layer                                             */
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
/* Description: Service Layer handlers  declarations                         */
/*                                                                           */
/* Additional information:                                                   */
/*                                                                           */
/* Functions:                                                                */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_service_handler_h
#define egts_service_handler_h

/******************************************************************************
*
*/

typedef struct
{
  u8    ST;   /* Service Type */    
  const egts_subrecord_handler_t* const * chandlers;
} egts_service_handler_t;

/******************************************************************************
*
*/

#endif

