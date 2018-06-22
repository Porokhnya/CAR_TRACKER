/*****************************************************************************/
/*                                                                           */
/* File: egts_firmware.c                                                     */
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

#include "../../include/egts_config.h"
#include "../../include/egts.h"

#include "../egts_services.h"
#include "../egts_service_handler.h"
#include "egts_firmware.h"
#include "../egts_response.h"

/******************************************************************************
*
*/

u8    egts_get_FIRMWARE_bitfields( egts_FIRMWARE_DATA_header_t* ps )
{
  u8 b = 0U;
  b |= ( ps->OT & 0x03U );     
  b <<= 2U;
  b |= ( ps->MT & 0x03U );
  return b; 
}

void  egts_set_FIRMWARE_bitfields( egts_FIRMWARE_DATA_header_t* ps , u8 b )
{
  ps->MT  = b & 0x03U;   
  b >>= 2U;
  ps->OT  = b & 0x03U;    
}

/******************************************************************************
*
*/

static
const egts_subrecord_handler_t* const  chandlers_FIRMWARE[] = 
{
  &chandler_RESPONSE,
  &chandler_FIRMWARE_PART_DATA,
  &chandler_FIRMWARE_FULL_DATA,
  /*@-nullassign@*/
  NULL
  /*@=nullassign@*/
};

const egts_service_handler_t    cservice_FIRMWARE =
{
  /* ST */          (u8)EGTS_FIRMWARE_SERVICE ,
  /* chandlers */   chandlers_FIRMWARE
};

/******************************************************************************
*
*/
