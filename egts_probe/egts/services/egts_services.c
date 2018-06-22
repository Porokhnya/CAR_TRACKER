/*****************************************************************************/
/*                                                                           */
/* File: egts_services.c                                                     */
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
/* Functions: egts_find_subrecord_handler                                    */
/*                                                                           */
/*****************************************************************************/



#include "../include/egts_config.h"
#include "../include/egts.h"

#include "egts_services.h"
#include "egts_service_handler.h"

#include "auth/egts_auth.h"
#include "commands/egts_command.h"
#include "firmware/egts_firmware.h"
#include "ecall/egts_ecall.h"

/******************************************************************************
*
*/

static
const egts_service_handler_t* const egts_services[] = 
{
  &cservice_AUTH ,
  &cservice_COMMANDS,
  &cservice_FIRMWARE,
  &cservice_ECALL,
  NULL
};

/*****************************************************************************/
/*                                                                           */
/* egts_find_subrecord_handler()                                             */
/*                                                                           */
/* Description: Function returns handler for specified Service Type and      */
/*   Subrecord Type                                                          */
/*                                                                           */
/* Arguments: ST - Service Type                                              */
/*            SRT - Subrecord Type                                           */
/*                                                                           */
/* Return:    Requested handler if found or NULL otherwise.                  */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

const egts_subrecord_handler_t*   egts_find_subrecord_handler( u8 ST , u8 SRT )
{
  const egts_service_handler_t* const* pservice_handler;
  const egts_subrecord_handler_t* const * psubrecord_handler;

  for ( pservice_handler = egts_services;
        ( pservice_handler != NULL ) && ( *pservice_handler != NULL );
        pservice_handler++ )
  {
    if ( (*pservice_handler)->ST == ST )
    {
      for ( psubrecord_handler = (*pservice_handler)->chandlers;
           (  psubrecord_handler != NULL ) && ( *psubrecord_handler != NULL );
            psubrecord_handler++ )
      {
        if ( (*psubrecord_handler)->SRT == SRT ) {
          return *psubrecord_handler;
        }
      }
    }
  }

  return NULL;
}

/******************************************************************************
*
*/


