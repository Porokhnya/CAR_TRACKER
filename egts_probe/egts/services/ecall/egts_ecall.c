/*****************************************************************************/
/*                                                                           */
/* File: egts_ecall.c                                                        */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service ECALL                                             */
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
/* Description: Parser/Composer/Debuger entries for service ECALL            */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#include "../../include/egts_config.h"
#include "../../include/egts.h"


#include "../egts_services.h"
#include "../egts_service_handler.h"
#include "egts_ecall.h"
#include "../egts_response.h"

#include "../commands/egts_command.h"

/******************************************************************************
*
*/


static
const egts_subrecord_handler_t* const  chandlers_ECALL[] = 
{
  &chandler_RESPONSE,
  &chandler_COMMANDS_COMMAND_DATA,
  &chandler_ECALL_ACCEL_DATA, 
  &chandler_ECALL_MSD_DATA,
  &chandler_ECALL_RAW_MSD_DATA,
  &chandler_ECALL_TRACK_DATA,
  NULL
};

const egts_service_handler_t    cservice_ECALL =
{
  /* ST */          (u8)EGTS_ECALL_SERVICE ,
  /* chandlers */   chandlers_ECALL
};

/******************************************************************************
*
*/
