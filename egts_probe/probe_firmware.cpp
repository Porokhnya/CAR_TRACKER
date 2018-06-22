/*****************************************************************************/
/*                                                                           */
/* File: probe_filrmware.c                                                   */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: EGTS Test Suite                                           */
/*                                                                           */
/* Status: Version 1.1                                                       */
/*                                                                           */
/* Language: C                                                               */
/*                                                                           */
/* (c) Copyright JSC �Navigation-information systems�, 2011                  */
/*                                                                           */
/* Address:                                                                  */
/*     24, Mishina Str., bld.1                                               */
/*     Moscow, Russia                                                        */
/*                                                                           */
/* Description: Test routines for FIRMWARE service                           */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: send_test_FIRMWARE_PART_DATA_1                                 */
/*            send_test_FIRMWARE_PART_DATA_2                                 */
/*            send_test_FIRMWARE_PART_DATA_1x                                */
/*            send_test_FIRMWARE_PART_DATA_2x                                */
/*            send_test_FIRMWARE_FULL_DATA                                   */
/*            send_test_FIRMWARE_FULL_DATA_x                                 */
/*                                                                           */
/*****************************************************************************/

#include <egts_config.h>
#include <egts.h>
#include <egts_impl.h>

/******************************************************************************
*
*/

#include "egts_probe.h"
#include "probe_firmware.h"

/******************************************************************************
*
*/

/*for probes only */
u8 egts_firmware_tx_buf[ EGTS_FIRMWARE_MAX_DATA ];

/*****************************************************************************/
/*                                                                           */
/* send_test_FIRMWARE_PART_DATA_1()                                          */
/*                                                                           */
/* Description: Test function for FIRMWARE_PART_DATA packet                  */
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

void send_test_FIRMWARE_PART_DATA_1(
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

  u8  test_buf[64];

  egts_FIRMWARE_PART_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_FIRMWARE_SERVICE;
  record.record.RST  = EGTS_FIRMWARE_SERVICE;

  /**************************************
  *
  */

  sr.ID    = 12345;
  sr.PN    = 1;
  sr.EPQ   = 1;

  sr.header.OT   = OT_FIRMWARE;
  sr.header.MT   = MT_AUX;

  sr.header.CMI  = 123;
  sr.header.VER  = 1;
  sr.header.WOS  = 12346;
  sr.header.FN_len = 10;

  memset( sr.header.FN , 0 , sizeof(sr.header.FN) );
  strcpy( (char*)sr.header.FN , "abcdeabcde" );

  sr.OD_len = 64;
  sr.OD = test_buf;
  memset( test_buf , 0xFF , 64 );

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_SERVICE_PART_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("FIRMWARE.EGTS_SR_SERVICE_PART_DATA (simple, 64b, first) ... ");
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
/* send_test_FIRMWARE_PART_DATA_2()                                          */
/*                                                                           */
/* Description: Test function for FIRMWARE_PART_DATA packet                  */
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

void send_test_FIRMWARE_PART_DATA_2(
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

  u8  test_buf[64];

  egts_FIRMWARE_PART_DATA_t   sr;

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
  record.record.SST  = EGTS_FIRMWARE_SERVICE;
  record.record.RST  = EGTS_FIRMWARE_SERVICE;

  /**************************************
  *
  */

  sr.ID    = 12345;
  sr.PN    = 2;
  sr.EPQ   = 2;

  sr.OD_len = 64;
  sr.OD = test_buf;
  memset( test_buf , 0xFF , 64 );

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_SERVICE_PART_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("FIRMWARE.EGTS_SR_SERVICE_PART_DATA (simple, 64b, second) ... ");
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
/* send_test_FIRMWARE_PART_DATA_1x()                                         */
/*                                                                           */
/* Description: Test function for FIRMWARE_PART_DATA packet                  */
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

void send_test_FIRMWARE_PART_DATA_1x(
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

  egts_FIRMWARE_PART_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_FIRMWARE_SERVICE;
  record.record.RST  = EGTS_FIRMWARE_SERVICE;

  /**************************************
  *
  */

  sr.ID    = 12345;
  sr.PN    = 1;
  sr.EPQ   = 1;

  sr.header.OT   = OT_FIRMWARE;
  sr.header.MT   = MT_AUX;

  sr.header.CMI  = 123;
  sr.header.VER  = 1;
  sr.header.WOS  = 12346;
  sr.header.FN_len = 10;

  memset( sr.header.FN , 0 , sizeof(sr.header.FN) );
  strcpy( (char*)sr.header.FN , "abcdeabcde" );

  sr.OD_len = EGTS_FIRMWARE_MAX_DATA;
  sr.OD = egts_firmware_tx_buf;
  memset( egts_firmware_tx_buf , 0xA5 , EGTS_FIRMWARE_MAX_DATA );

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_SERVICE_PART_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("FIRMWARE.EGTS_SR_SERVICE_PART_DATA (simple, max length, first) ... ");
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
/* send_test_FIRMWARE_PART_DATA_2x()                                         */
/*                                                                           */
/* Description: Test function for FIRMWARE_PART_DATA packet                  */
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

void send_test_FIRMWARE_PART_DATA_2x(
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

  egts_FIRMWARE_PART_DATA_t   sr;

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
  record.record.SST  = EGTS_FIRMWARE_SERVICE;
  record.record.RST  = EGTS_FIRMWARE_SERVICE;

  /**************************************
  *
  */

  sr.ID    = 12345;
  sr.PN    = 2;
  sr.EPQ   = 2;

  sr.OD_len = EGTS_FIRMWARE_MAX_DATA;
  sr.OD = egts_firmware_tx_buf;
  memset( egts_firmware_tx_buf , 0xA5 , EGTS_FIRMWARE_MAX_DATA );

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_SERVICE_PART_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("FIRMWARE.EGTS_SR_SERVICE_PART_DATA (simple, max length, second) ... ");
  egts_tx_packet( estate , pprofile , PR , proute , PID, presponce ,
    &record , 1 ,
    ptemp_buf ,temp_buf_sz );
  egts_probe_printf( (probes_ctx.last_result) ? "FAILED\n" : "PASSED\n" );
  probes_ctx.precords = NULL;
  probes_ctx.nrecords = 0;
  probes_ctx.last_result = 0;

}

/*****************************************************************************/
/*                                                                           */
/* send_test_FIRMWARE_FULL_DATA()                                            */
/*                                                                           */
/* Description: Test function for FIRMWARE_FULL_DATA packet                  */
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

void send_test_FIRMWARE_FULL_DATA(
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

  u8  test_buf[64];

  egts_FIRMWARE_FULL_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_FIRMWARE_SERVICE;
  record.record.RST  = EGTS_FIRMWARE_SERVICE;

  /**************************************
  *
  */

  sr.header.OT   = OT_FIRMWARE;
  sr.header.MT   = MT_AUX;

  sr.header.CMI  = 123;
  sr.header.VER  = 1;
  sr.header.WOS  = 12346;
  sr.header.FN_len = 10;

  memset( sr.header.FN , 0 , sizeof(sr.header.FN) );
  strcpy( (char*)sr.header.FN , "abcdeabcde" );


  sr.OD_len = 64;
  sr.OD = test_buf;
  memset( test_buf , 0xFF , 64 );

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_SERVICE_FULL_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("FIRMWARE.EGTS_SR_SERVICE_FULL_DATA (simple, 64b) ... ");
  egts_tx_packet( estate , pprofile , PR , proute , PID, presponce ,
    &record , 1 ,
    ptemp_buf ,temp_buf_sz );
  egts_probe_printf( (probes_ctx.last_result) ? "FAILED\n" : "PASSED\n" );
  probes_ctx.precords = NULL;
  probes_ctx.nrecords = 0;
  probes_ctx.last_result = 0;
}

/*****************************************************************************/
/*                                                                           */
/* send_test_FIRMWARE_FULL_DATA_x()                                          */
/*                                                                           */
/* Description: Test function for FIRMWARE_FULL_DATA packet                  */
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

void send_test_FIRMWARE_FULL_DATA_x(
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

  egts_FIRMWARE_FULL_DATA_t   sr;

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
  record.record.SST  = EGTS_FIRMWARE_SERVICE;
  record.record.RST  = EGTS_FIRMWARE_SERVICE;

  /**************************************
  *
  */

  sr.header.OT   = OT_FIRMWARE;
  sr.header.MT   = MT_AUX;

  sr.header.CMI  = 123;
  sr.header.VER  = 1;
  sr.header.WOS  = 12346;
  sr.header.FN_len = 10;

  memset( sr.header.FN , 0 , sizeof(sr.header.FN) );
  strcpy( (char*)sr.header.FN , "abcdeabcde" );


  sr.OD_len = EGTS_FIRMWARE_MAX_DATA;
  sr.OD = egts_firmware_tx_buf;
  memset( egts_firmware_tx_buf , 0xA5 , EGTS_FIRMWARE_MAX_DATA );

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_SERVICE_FULL_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("FIRMWARE.EGTS_SR_SERVICE_FULL_DATA (simple, max length) ... ");
  egts_tx_packet( estate , pprofile , PR , proute , PID, presponce ,
    &record , 1 ,
    ptemp_buf ,temp_buf_sz );
  egts_probe_printf( (probes_ctx.last_result) ? "FAILED\n" : "PASSED\n" );
  probes_ctx.precords = NULL;
  probes_ctx.nrecords = 0;
  probes_ctx.last_result = 0;
}

/******************************************************************************
*
*/

