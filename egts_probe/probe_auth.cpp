/*****************************************************************************/
/*                                                                           */
/* File: probe_auth.c                                                        */
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
/* Description: Test routines for AUTH service                               */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: send_test_AUTH_TERM_IDENTITY                                   */
/*            send_test_AUTH_TERM_IDENTITY1                                  */
/*            send_test_AUTH_TERM_IDENTITY2                                  */
/*            send_test_AUTH_TERM_IDENTITY3                                  */
/*            send_test_AUTH_VEHICLE_DATA                                    */
/*            send_test_AUTH_AUTH_PARAMS                                     */
/*            send_test_AUTH_AUTH_PARAMS1                                    */
/*            send_test_AUTH_AUTH_PARAMS2                                    */
/*            send_test_AUTH_TERM_MODULE_DATA                                */
/*            send_test_AUTH_AUTH_INFO                                       */
/*            send_test_AUTH_SERVICE_INFO                                    */
/*            send_test_AUTH_RESULT_CODE                                     */
/*            send_test_AUTH_INIT                                            */
/*                                                                           */
/*****************************************************************************/

#include <egts_config.h>
#include <egts.h>
#include <egts_impl.h>

/******************************************************************************
*
*/

#include "egts_probe.h"
#include "probe_auth.h"


/*****************************************************************************/
/*                                                                           */
/* send_test_AUTH_TERM_IDENTITY()                                            */
/*                                                                           */
/* Description: Test function for AUTH_TERM_IDENTITY packet                  */ 
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


void send_test_AUTH_TERM_IDENTITY(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz  )
{
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_AUTH_TERM_IDENTITY_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.TID    = 1234;

  sr.SSRA   = 0;

  sr.HDID   = 0;         
  sr.HDIDE  = 0;

  memset( sr.IMEI , 0 , sizeof(sr.IMEI) );
  strcpy( (char*)sr.IMEI , "0123456789" );
  sr.IMEIE = 1;

  memset( sr.IMSI , 0 , sizeof(sr.IMSI) );
  strcpy( (char*)sr.IMSI , "0123456789" );
  sr.IMSIE = 1;

  memset( sr.LNGC , 0 ,  sizeof(sr.LNGC) );
  sr.LNGCE = 0;

  memset( sr.NID , 0 ,  sizeof(sr.NID) );
  sr.NIDE = 0;

  sr.BS  = 4096;
  sr.BSE = 1;

  memset( sr.MSISDN , 0 , sizeof(sr.MSISDN) );
  strcpy( (char*)sr.MSISDN , "0123456789" );
  sr.MNE = 1;
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_TERM_IDENTITY;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;

  probes_ctx.presponce = presponce;
  probes_ctx.precords  = &record;
  probes_ctx.nrecords  = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_TERM_IDENTITY (simple) ... ");
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
/* send_test_AUTH_TERM_IDENTITY()                                            */
/*                                                                           */
/* Description: Test function for AUTH_TERM_IDENTITY packet                  */ 
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

void send_test_AUTH_TERM_IDENTITY1(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz  )
{
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_AUTH_TERM_IDENTITY_t   sr;

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
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.TID    = 1234;

  sr.SSRA   = 0;

  sr.HDID   = 0;  

  sr.HDIDE = 0;

  sr.IMEIE = 0;

  sr.IMSIE = 0;

  sr.LNGCE = 0;

  sr.NIDE = 0;

  sr.BSE = 0;

  sr.MNE = 0;
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_TERM_IDENTITY;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;

  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_TERM_IDENTITY (no optional fields) ... ");
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
/* send_test_AUTH_TERM_IDENTITY2()                                           */
/*                                                                           */
/* Description: Test function for AUTH_TERM_IDENTITY packet                  */ 
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

void send_test_AUTH_TERM_IDENTITY2(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz  )
{
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_AUTH_TERM_IDENTITY_t   sr;

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
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.TID    = 1234;
  
  sr.SSRA   = 0;

  sr.HDID   = 0;         
  sr.HDIDE = 0;

  memset( sr.IMEI , 0 , sizeof(sr.IMEI) );
  strcpy( (char*)sr.IMEI , "0123456789" );
  sr.IMEIE = 1;

  memset( sr.IMSI , 0 , sizeof(sr.IMSI) );
  strcpy( (char*)sr.IMSI , "0123456789" );
  sr.IMSIE = 1;

  memset( sr.LNGC , 0 ,  sizeof(sr.LNGC) );
  memcpy( sr.LNGC , "rus" , 3 );
  sr.LNGCE = 1;

  sr.NID[0] = 1;
  sr.NID[1] = 2;
  sr.NID[2] = 3;
  sr.NIDE = 1;

  sr.BS  = 4096;
  sr.BSE = 1;

  memset( sr.MSISDN , 0 , sizeof(sr.MSISDN) );
  strcpy( (char*)sr.MSISDN , "0123456789" );
  sr.MNE = 1;
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_TERM_IDENTITY;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;

  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_TERM_IDENTITY (all optional fields) ... ");
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
/* send_test_AUTH_TERM_IDENTITY3()                                           */
/*                                                                           */
/* Description: Test function for AUTH_TERM_IDENTITY packet                  */ 
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


void send_test_AUTH_TERM_IDENTITY3(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz  )
{
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_AUTH_TERM_IDENTITY_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.TID    = 1234;

  sr.SSRA   = 0;

  sr.HDID   = 0;         
  sr.HDIDE = 0;

  memset( sr.IMEI , '0' , sizeof(sr.IMEI) );
  sr.IMEIE = 1;

  memset( sr.IMSI , '0' , sizeof(sr.IMSI) );
  sr.IMSIE = 1;

  memset( sr.LNGC , '0' ,  sizeof(sr.LNGC) );
  sr.LNGCE = 1;

  memset( sr.NID , 1 ,  sizeof(sr.NID) );
  sr.NIDE = 1;

  sr.BS  = 4096;
  sr.BSE = 1;

  memset( sr.MSISDN , '0' , sizeof(sr.MSISDN) );
  sr.MNE = 1;
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_TERM_IDENTITY;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;

  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_TERM_IDENTITY (max size) ... ");
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
/* send_test_AUTH_VEHICLE_DATA()                                             */
/*                                                                           */
/* Description: Test function for AUTH_VEHICLE_DATA packet                   */ 
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

void send_test_AUTH_VEHICLE_DATA(
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

  egts_AUTH_VEHICLE_DATA_t   sr;

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
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */



  memset( sr.VIN , '0' , sizeof(sr.VIN) );
  sr.VHT    = 12345;
  sr.VPST   = 23456;
  
  /**************************************
  *
  */


  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_VEHICLE_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_VEHICLE_DATA (simple) ... ");
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
/* send_test_AUTH_AUTH_PARAMS()                                              */
/*                                                                           */
/* Description: Test function for AUTH_AUTH_PARAMS packet                    */ 
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

void send_test_AUTH_AUTH_PARAMS(
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

  egts_AUTH_AUTH_PARAMS_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.ENA = 1;

  sr.PKE = 1;
  sr.PKL = 32;
  memset( sr.PBK , 0 , sizeof(sr.PBK) );
  memset( sr.PBK , 0xFF , sr.PKL );

  sr.ISLE = 1;
  sr.ISL = 32;

  sr.MSE = 1;
  sr.MSZ = 16;

  sr.SSE = 1;
  memset( sr.SS , 0 , sizeof(sr.SS) );
  strcpy( (char*)sr.SS , "0123456789" );
  sr.SS_len = 10;

  sr.EXE = 1;
  memset( sr.EXP , 0 , sizeof(sr.EXP) );
  strcpy( (char*)sr.EXP , "0123456789" );
  sr.EXP_len = 10;

  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_AUTH_PARAMS;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_AUTH_PARAMS (simple) ... ");
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
/* send_test_AUTH_AUTH_PARAMS1()                                             */
/*                                                                           */
/* Description: Test function for AUTH_AUTH_PARAMS packet                    */ 
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

void send_test_AUTH_AUTH_PARAMS1(
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

  egts_AUTH_AUTH_PARAMS_t   sr;

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
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.ENA = 1;

  sr.PKE = 0;

  sr.PKL = 0;

  sr.ISLE = 0;

  sr.MSE = 0;

  sr.SSE = 0;

  sr.SS_len = 0;

  sr.EXE = 0;

  sr.EXP_len = 0;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_AUTH_PARAMS;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_AUTH_PARAMS (no optional fileds) ... ");
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
/* send_test_AUTH_AUTH_PARAMS2()                                             */
/*                                                                           */
/* Description: Test function for AUTH_AUTH_PARAMS packet                    */ 
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

void send_test_AUTH_AUTH_PARAMS2(
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

  egts_AUTH_AUTH_PARAMS_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.PKE = 1;
  sr.PKL = EGTS_PBK_LEN;
  memset( sr.PBK , '0' , sizeof(sr.PBK) );

  sr.ENA = 0;

  sr.ISLE = 1;
  sr.ISL = 32;

  sr.MSE = 1;
  sr.MSZ = 16;

  sr.SSE = 1;
  memset( sr.SS , '0' , sizeof(sr.SS) );
  sr.SS_len = EGTS_SS_LEN;

  sr.EXE = 1;
  memset( sr.EXP , '0' , sizeof(sr.EXP) );
  sr.EXP_len = EGTS_EXP_LEN;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_AUTH_PARAMS;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_AUTH_PARAMS (max size) ... ");
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
/* send_test_AUTH_TERM_MODULE_DATA()                                         */
/*                                                                           */
/* Description: Test function for AUTH_TERM_MODULE_DATA packet               */
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

void send_test_AUTH_TERM_MODULE_DATA(
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

  egts_AUTH_TERM_MODULE_DATA_t   sr;

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
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.MT  = 1;
  sr.VID = 12345;
  sr.FWV = 0x0101;
  sr.SWV = 0x0101;
  sr.MD = 10;
  sr.ST = 1;

  memset( sr.SRN , 0 , sizeof(sr.SRN) );
  strcpy( (char*)sr.SRN , "0123456789" );
  sr.SRN_len = 10;

  memset( sr.DSCR , 0 , sizeof(sr.SRN) );
  strcpy( (char*)sr.DSCR , "0123456789" );
  sr.DSCR_len = 10;
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_MODULE_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_MODULE_DATA (simple) ... ");
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
/* send_test_AUTH_AUTH_INFO()                                                */
/*                                                                           */
/* Description: Test function for AUTH_AUTH_INFO packet                      */
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

void send_test_AUTH_AUTH_INFO(
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

  egts_AUTH_AUTH_INFO_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  memset( sr.UNM , 0 , sizeof(sr.UNM) );
  strcpy( (char*)sr.UNM , "0123456789" );
  sr.UNM_len = 10;

  memset( sr.UPSW , 0 , sizeof(sr.UPSW) );
  strcpy( (char*)sr.UPSW , "0123456789" );
  sr.UPSW_len = 10;
  
  memset( sr.SS , 0 , sizeof(sr.SS) );
  strcpy( (char*)sr.SS , "0123456789" );
  sr.SS_len = 10;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_AUTH_INFO;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_AUTH_INFO (simple) ... ");
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
/* send_test_AUTH_SERVICE_INFO()                                             */
/*                                                                           */
/* Description: Test function for AUTH_SERVICE_INFO packet                   */
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

void send_test_AUTH_SERVICE_INFO(
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

  egts_AUTH_SERVICE_INFO_t   sr;

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
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.ST    = 1;
  sr.SST   = 0;

  sr.SRVA  = 1;
  sr.SRVRP = 2;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_SERVICE_INFO;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_SERVICE_INFO (simple) ... ");
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
/* send_test_AUTH_RESULT_CODE()                                              */
/*                                                                           */
/* Description: Test function for AUTH_RESULT_CODE packet                    */
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

void send_test_AUTH_RESULT_CODE(
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

  egts_AUTH_RESULT_CODE_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr.RCD   = 1;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_RESULT_CODE;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_RESULT_CODE (simple) ... ");
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
/* send_test_AUTH_INIT()                                                     */
/*                                                                           */
/* Description: Test function for AUTH_INIT packet                           */
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

void send_test_AUTH_INIT(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz  )
{
  egts_record_t               record;
  egts_subrecord_t            subrecords[2];

  egts_AUTH_TERM_IDENTITY_t   sr_term_identity;
  egts_AUTH_VEHICLE_DATA_t    sr_vehicle_data;

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
  record.record.SST  = EGTS_AUTH_SERVICE;
  record.record.RST  = EGTS_AUTH_SERVICE;

  /**************************************
  *
  */

  sr_term_identity.TID    = 1234;

  sr_term_identity.SSRA   = 0;

  sr_term_identity.HDID   = 0;         
  sr_term_identity.HDIDE = 0;

  memset( sr_term_identity.IMEI , 0 , sizeof(sr_term_identity.IMEI) );
  strcpy( (char*)sr_term_identity.IMEI , "0123456789" );
  sr_term_identity.IMEIE = 1;

  memset( sr_term_identity.IMSI , 0 , sizeof(sr_term_identity.IMSI) );
  strcpy( (char*)sr_term_identity.IMEI , "0123456789" );
  sr_term_identity.IMSIE = 1;

  memset( sr_term_identity.LNGC , 0 ,  sizeof(sr_term_identity.LNGC) );
  sr_term_identity.LNGCE = 0;

  memset( sr_term_identity.NID , 0 ,  sizeof(sr_term_identity.NID) );
  sr_term_identity.NIDE = 0;

  sr_term_identity.BS  = 4096;
  sr_term_identity.BSE = 1;

  memset( sr_term_identity.MSISDN , 0 , sizeof(sr_term_identity.MSISDN) );
  strcpy( (char*)sr_term_identity.MSISDN , "0123456789" );
  sr_term_identity.MNE = 1;
  
  /* subrecord.SRL - auto */
  subrecords[0].subrecord.SRT = EGTS_SR_TERM_IDENTITY;
  subrecords[0].SRD = (void*)&sr_term_identity;

  /**************************************
  *
  */

  memset( sr_vehicle_data.VIN , '0' , sizeof(sr_vehicle_data.VIN) );
  sr_vehicle_data.VHT    = 12345;
  sr_vehicle_data.VPST   = 23456;
  /* subrecord.SRL - auto */
  subrecords[1].subrecord.SRT = EGTS_SR_VEHICLE_DATA;
  subrecords[1].SRD           = (void*)&sr_vehicle_data;

  /**************************************
  *
  */

  record.psubrecords = subrecords;
  record.nsubrecords = 2;

  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("AUTH.EGTS_SR_TERM_IDENTITY+VEHICLE_DATA(simple) ... ");
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
