/*****************************************************************************/
/*                                                                           */
/* File: egts_auth.c                                                         */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service AUTH                                              */
/*                                                                           */
/* Status: Version 1.1                                                       */
/*                                                                           */
/* Language: C                                                               */
/*	                                                                         */
/* (c) Copyright JSC «Navigation-information systems», 2011                  */
/*                                                                           */
/* Address:                                                                  */
/*     24, Mishina Str., bld.1                                               */
/*     Moscow, Russia                                                        */
/*                                                                           */
/* Description: Parser/Composer/Debuger entries for service AUTH             */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions:                                                                */
/*                                                                           */
/*****************************************************************************/

#include "../../include/egts_config.h"
#include "../../include/egts.h"

/******************************************************************************
*
*/

#include "../egts_services.h"
#include "../egts_service_handler.h"
#include "egts_auth.h"
#include "../egts_response.h"

/******************************************************************************
*
*/

static
const egts_subrecord_handler_t* const chandlers_AUTH[] = 
{
  &chandler_RESPONSE,
  &chandler_AUTH_AUTH_INFO,
  &chandler_AUTH_AUTH_PARAMS,
  &chandler_AUTH_TERM_MODULE_DATA,
  &chandler_AUTH_TERM_IDENTITY,
  &chandler_AUTH_VEHICLE_DATA,
  &chandler_AUTH_SERVICE_INFO,
  &chandler_AUTH_RESULT_CODE,
  /*@-nullassign@*/
  NULL
  /*@=nullassign@*/
};


const egts_service_handler_t    cservice_AUTH =
{
  /* ST */          (u8)EGTS_AUTH_SERVICE ,
  /* chandlers */   chandlers_AUTH
};

/******************************************************************************
*
*/
