/*****************************************************************************/
/*                                                                           */
/* File: egts_commands.c                                                     */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service COMMANDS                                          */
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
/* Description: Parser/Composer/Debuger entries for service COMMANDS         */
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
#include "egts_command.h"
#include "../egts_response.h"

/******************************************************************************
*
*/

static
const egts_subrecord_handler_t* const  chandlers_COMMANDS[] = 
{
  &chandler_RESPONSE,
  &chandler_COMMANDS_COMMAND_DATA,
  /*@-nullassign@*/
  NULL
  /*@=nullassign@*/
};

const egts_service_handler_t    cservice_COMMANDS =
{
  /* ST */          (u8)EGTS_COMMANDS_SERVICE ,
  /* chandlers */   chandlers_COMMANDS
};

/******************************************************************************
*
*/
