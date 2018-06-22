/*****************************************************************************/
/*                                                                           */
/* File: probe_commands.c                                                    */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: EGTS Test Suite                                           */
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
/* Description: Test routines for COMMANDS service                           */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: send_test_COMMANDS_COMMAND_DATA_conf                           */
/*            send_test_COMMANDS_COMMAND_DATA_confx                          */
/*            send_test_COMMANDS_COMMAND_DATA_cmd                            */
/*            send_test_COMMANDS_COMMAND_DATA_cmdx                           */
/*                                                                           */
/*****************************************************************************/

#include "../egts/include/egts_config.h"
#include "../egts/include/egts.h"
#include "../egts/include/egts_impl.h"

/******************************************************************************
*
*/

#include "../egts_probe.h"
#include "probe_commands.h"

/******************************************************************************
*
*/

/*for probes only */
u8 egts_commands_tx_buf[ EGTS_COMMAND_MAX_DT ];

/*****************************************************************************/
/*                                                                           */
/* send_test_COMMANDS_COMMAND_DATA_conf()                                    */
/*                                                                           */
/* Description: Test function for COMMANDS_COMMAND_DATA packet               */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            pprofile - transmit settings                                   */
/*            PR - packet priority                                           */
/*            proute - packet routing settings                               */
/*            PID - packet identifier                                        */
/*            presponce - optional response data                             */
/*            ptemp_buf - temporary buffer 64KB for operation                */
/*            temp_buf_sz - temporary buffer size                            */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     Not a part of mandatory code. For debug only.                  */
/*                                                                           */
/*****************************************************************************/

void send_test_COMMANDS_COMMAND_DATA_conf(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  )
{

  u8  dt[ 64 ];

  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_COMMANDS_COMMAND_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_COMMANDS_SERVICE;
  record.record.RST  = EGTS_COMMANDS_SERVICE;

   /**************************************
  *
  */

  sr.CT    = CT_COMCONF;
  sr.CCT   = CC_OK;
  sr.CID   = 1;
  sr.SID   = 1;

  sr.CHSFE = 1;
  sr.CHS   = CHS_CP_1251;

  sr.ACFE  = 1;
  memset( sr.AC , 0 , sizeof(sr.AC) );
  strcpy( (char*)sr.AC , "0123456789" );
  sr.ACL   = 10;

  sr.DT = dt;
  memset( sr.DT , 0 , 64 );
  strcpy( (char*)sr.DT , "0123456789" );
  sr.DT_len = 10;

  /* CONF */
  sr.conf.ADR = 12345;
  sr.conf.CCD = 12345;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_COMMAND_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("COMMANDS.EGTS_SR_COMMAND_DATA (simple COMCONF) ... ");
  egts_reset_errors( estate );
  egts_tx_packet( estate , pprofile , PR , proute , PID, presponce ,
    &record , 1 ,
    ptemp_buf ,temp_buf_sz );
  egts_probe_printf( ( ( estate->rx_error_count > 0 ) ||
                     ( estate->tx_error_count > 0 ) || 
                     ( probes_ctx.last_result != 0 ) ) ? "FAILED\n" : "PASSED\n" );
  probes_ctx.precords = NULL;
  probes_ctx.nrecords = 0;
  probes_ctx.last_result = 0;

}

/*****************************************************************************/
/*                                                                           */
/* send_test_COMMANDS_COMMAND_DATA_confx()                                   */
/*                                                                           */
/* Description: Test function for COMMANDS_COMMAND_DATA packet               */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            pprofile - transmit settings                                   */
/*            PR - packet priority                                           */
/*            proute - packet routing settings                               */
/*            PID - packet identifier                                        */
/*            presponce - optional response data                             */
/*            ptemp_buf - temporary buffer 64KB for operation                */
/*            temp_buf_sz - temporary buffer size                            */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     Not a part of mandatory code. For debug only.                  */
/*                                                                           */
/*****************************************************************************/

void send_test_COMMANDS_COMMAND_DATA_confx(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  )
{


  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_COMMANDS_COMMAND_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 1;   
  record.record.TM   = egts_set_tm( 1, 2, 3, 4, 5, 2011 );
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_COMMANDS_SERVICE;
  record.record.RST  = EGTS_COMMANDS_SERVICE;

  /**************************************
  *
  */

  sr.CT    = CT_COMCONF;
  sr.CCT   = CC_OK;
  sr.CID   = 1;
  sr.SID   = 1;

  sr.CHSFE = 1;
  sr.CHS   = CHS_CP_1251;

  sr.ACFE  = 1;
  memset( sr.AC , 0 , sizeof(sr.AC) );
  strcpy( (char*)sr.AC , "0123456789" );
  sr.ACL   = 10;

  sr.DT = egts_commands_tx_buf;
  memset( sr.DT , '0' , sizeof( EGTS_COMMAND_MAX_DT ) );
  sr.DT_len = EGTS_COMMAND_MAX_DT;

  /* CONF */
  sr.conf.ADR = 12345;
  sr.conf.CCD = 12345;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_COMMAND_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("COMMANDS.EGTS_SR_COMMAND_DATA (COMCONF, max length) ... ");
  egts_reset_errors( estate );
  egts_tx_packet( estate , pprofile , PR , proute , PID, presponce ,
    &record , 1 ,
    ptemp_buf ,temp_buf_sz );
  egts_probe_printf( ( ( estate->rx_error_count > 0 ) ||
                     ( estate->tx_error_count > 0 ) || 
                     ( probes_ctx.last_result != 0 ) ) ? "FAILED\n" : "PASSED\n" );
  probes_ctx.precords = NULL;
  probes_ctx.nrecords = 0;
  probes_ctx.last_result = 0;

}

/*****************************************************************************/
/*                                                                           */
/* send_test_COMMANDS_COMMAND_DATA_cmd()                                     */
/*                                                                           */
/* Description: Test function for COMMANDS_COMMAND_DATA packet               */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            pprofile - transmit settings                                   */
/*            PR - packet priority                                           */
/*            proute - packet routing settings                               */
/*            PID - packet identifier                                        */
/*            presponce - optional response data                             */
/*            ptemp_buf - temporary buffer 64KB for operation                */
/*            temp_buf_sz - temporary buffer size                            */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     Not a part of mandatory code. For debug only.                  */
/*                                                                           */
/*****************************************************************************/

void send_test_COMMANDS_COMMAND_DATA_cmd(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  )
{

  u8  dt[ 64 ];

  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_COMMANDS_COMMAND_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_COMMANDS_SERVICE;
  record.record.RST  = EGTS_COMMANDS_SERVICE;

  /**************************************
  *
  */

  sr.CT    = CT_COM;
  sr.CCT   = CC_OK;
  sr.CID   = 1;
  sr.SID   = 1;

  sr.CHSFE = 1;
  sr.CHS   = CHS_CP_1251;

  sr.ACFE  = 1;
  memset( sr.AC , 0 , sizeof(sr.AC) );
  strcpy( (char*)sr.AC , "0123456789" );
  sr.ACL   = 10;

  sr.DT = dt;
  memset( sr.DT , 0 , 64 );
  strcpy( (char*)sr.DT , "0123456789" );
  sr.DT_len = 10;

  /* COM */
  sr.command.ADR = 12345;
  sr.command.SZ  = 1;
  sr.command.ACT = 0;
  sr.command.CCD = 12345;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_COMMAND_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("COMMANDS.EGTS_SR_COMMAND_DATA (simple COM) ... ");
  egts_reset_errors( estate );
  egts_tx_packet( estate , pprofile , PR , proute , PID, presponce ,
    &record , 1 ,
    ptemp_buf ,temp_buf_sz );
  egts_probe_printf( ( ( estate->rx_error_count > 0 ) ||
                     ( estate->tx_error_count > 0 ) || 
                     ( probes_ctx.last_result != 0 ) ) ? "FAILED\n" : "PASSED\n" );
  probes_ctx.precords = NULL;
  probes_ctx.nrecords = 0;
  probes_ctx.last_result = 0;

}

/*****************************************************************************/
/*                                                                           */
/* send_test_COMMANDS_COMMAND_DATA_cmdx()                                    */
/*                                                                           */
/* Description: Test function for COMMANDS_COMMAND_DATA packet               */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            pprofile - transmit settings                                   */
/*            PR - packet priority                                           */
/*            proute - packet routing settings                               */
/*            PID - packet identifier                                        */
/*            presponce - optional response data                             */
/*            ptemp_buf - temporary buffer 64KB for operation                */
/*            temp_buf_sz - temporary buffer size                            */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     Not a part of mandatory code. For debug only.                  */
/*                                                                           */
/*****************************************************************************/

void send_test_COMMANDS_COMMAND_DATA_cmdx(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  )
{


  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_COMMANDS_COMMAND_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 1;   
  record.record.TM   = egts_set_tm( 1, 2, 3, 4, 5, 2011 );
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_COMMANDS_SERVICE;
  record.record.RST  = EGTS_COMMANDS_SERVICE;

  /**************************************
  *
  */

  sr.CT    = CT_COM;
  sr.CCT   = CC_OK;
  sr.CID   = 1;
  sr.SID   = 1;

  sr.CHSFE = 1;
  sr.CHS   = CHS_CP_1251;

  sr.ACFE  = 1;
  memset( sr.AC , 0 , sizeof(sr.AC) );
  strcpy( (char*)sr.AC , "0123456789" );
  sr.ACL   = 10;

  sr.DT = egts_commands_tx_buf;
  memset( sr.DT , '0' , sizeof( EGTS_COMMAND_MAX_DT ) );
  sr.DT_len = EGTS_COMMAND_MAX_DT;

  /* COM */
  sr.command.ADR = 12345;
  sr.command.SZ  = 1;
  sr.command.ACT = 0;
  sr.command.CCD = 12345;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_COMMAND_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("COMMANDS.EGTS_SR_COMMAND_DATA (COM, max length) ... ");
  egts_reset_errors( estate );
  egts_tx_packet( estate , pprofile , PR , proute , PID, presponce ,
    &record , 1 ,
    ptemp_buf ,temp_buf_sz );
  egts_probe_printf( ( ( estate->rx_error_count > 0 ) ||
                     ( estate->tx_error_count > 0 ) || 
                     ( probes_ctx.last_result != 0 ) ) ? "FAILED\n" : "PASSED\n" );
  probes_ctx.precords = NULL;
  probes_ctx.nrecords = 0;
  probes_ctx.last_result = 0;

}

/******************************************************************************
*
*/
