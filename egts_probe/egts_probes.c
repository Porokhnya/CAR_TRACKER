/*****************************************************************************/
/*                                                                           */
/* File: egts_probes.c                                                       */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Tests                                                     */
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
/* Description: Set of test functions.                                       */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions:                                                                */
/*                                                                           */
/*****************************************************************************/

#include "egts/include/egts_config.h"
#include "egts/include/egts.h"
#include "egts/include/egts_impl.h"

/******************************************************************************
*
*/

#include "egts_probe.h"

#include "auth/probe_auth.h"
#include "commands/probe_commands.h"
#include "firmware/probe_firmware.h"
#include "ecall/probe_ecall.h"

/******************************************************************************
*
*/

const tf_probe_send  egts_probes_vector[] = 
{

  send_test_AUTH_TERM_IDENTITY,
  send_test_AUTH_TERM_IDENTITY1,
  send_test_AUTH_TERM_IDENTITY2,
  send_test_AUTH_TERM_IDENTITY3,

  send_test_AUTH_VEHICLE_DATA,

  send_test_AUTH_AUTH_PARAMS,
  send_test_AUTH_AUTH_PARAMS1,
  send_test_AUTH_AUTH_PARAMS2,

  send_test_AUTH_TERM_MODULE_DATA,
  send_test_AUTH_AUTH_INFO,
  send_test_AUTH_SERVICE_INFO,
  send_test_AUTH_RESULT_CODE,

  send_test_COMMANDS_COMMAND_DATA_conf,
  send_test_COMMANDS_COMMAND_DATA_confx,
  send_test_COMMANDS_COMMAND_DATA_cmd,
  send_test_COMMANDS_COMMAND_DATA_cmdx,

  send_test_FIRMWARE_PART_DATA_1,
  send_test_FIRMWARE_PART_DATA_2,
  send_test_FIRMWARE_PART_DATA_1x,
  send_test_FIRMWARE_PART_DATA_2x,

  send_test_FIRMWARE_FULL_DATA,
  send_test_FIRMWARE_FULL_DATA_x,

  send_test_ECALL_ACCEL_DATA,
  send_test_ECALL_ACCEL_DATA_x,

  send_test_ECALL_MSD_DATA,
  send_test_ECALL_MSD_DATA_n,
  send_test_ECALL_MSD_DATA_x,

  send_test_ECALL_RAW_MSD_DATA_1,
  send_test_ECALL_RAW_MSD_DATA_2,
  

  send_test_ECALL_TRACK_DATA_1,
  send_test_ECALL_TRACK_DATA_2,
  send_test_ECALL_TRACK_DATA_3,
  send_test_ECALL_TRACK_DATA_4,

  send_test_AUTH_INIT,

  NULL
};

/******************************************************************************
*
*/
