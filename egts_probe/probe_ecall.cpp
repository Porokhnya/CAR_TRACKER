/*****************************************************************************/
/*                                                                           */
/* File: probe_ecall.c                                                       */
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
/* Description: Test routines for ECALL service                              */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: send_test_ECALL_ACCEL_DATA                                     */
/*            send_test_ECALL_ACCEL_DATA_x                                   */
/*            send_test_ECALL_MSD_DATA                                       */
/*            send_test_ECALL_MSD_DATA_n                                     */
/*            send_test_ECALL_MSD_DATA_x                                     */
/*                                                                           */
/*****************************************************************************/

#include <egts_config.h>
#include <egts.h>
#include <egts_impl.h>

#include "egts_probe.h"
#include "probe_ecall.h"

/*****************************************************************************/
/*                                                                           */
/* send_test_ECALL_ACCEL_DATA()                                              */
/*                                                                           */
/* Description: Test function for ECALL_ACCEL_DATA packet                    */
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

void send_test_ECALL_ACCEL_DATA(
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
  u8 i;
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_ECALL_ACCEL_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.SA   = 10;
  sr.ATM  = egts_set_tm( 2, 3, 4, 5, 6, 2011 );
  for ( i = 0; i < sr.SA; i++ )
  {
    sr.ADS[i].RTM = i;
    sr.ADS[i].XAAV = i;
    sr.ADS[i].YAAV = i;
    sr.ADS[i].ZAAV = i;
  }

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_ACCEL_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_ACCEL_DATA (simple) ... ");
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
/* send_test_ECALL_ACCEL_DATA_x()                                            */
/*                                                                           */
/* Description: Test function for ECALL_ACCEL_DATA packet                    */
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

void send_test_ECALL_ACCEL_DATA_x(
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
  u8 i;
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_ECALL_ACCEL_DATA_t   sr;

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
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.SA   = EGTS_ADS_MAX_COUNT;
  sr.ATM  = egts_set_tm( 2, 3, 4, 5, 6, 2011 );
  for ( i = 0; i < sr.SA; i++ )
  {
    sr.ADS[i].RTM = i;
    sr.ADS[i].XAAV = i;
    sr.ADS[i].YAAV = i;
    sr.ADS[i].ZAAV = i;
  }

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_ACCEL_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_ACCEL_DATA (max length) ... ");
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
/* send_test_ECALL_MSD_DATA()                                                */
/*                                                                           */
/* Description: Test function for ECALL_MSD_DATA packet                      */
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

void send_test_ECALL_MSD_DATA(
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

  egts_ECALL_MSD_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.FV   = 1;
  sr.MI   = 1;
  
  sr.VT     = 1;   
  sr.POCN   = 1;   
  sr.CLT    = 1;
  sr.ACT    = 1;

  memset( sr.VIN , 0x00 , sizeof(sr.VIN) );
  strcpy( (char*)sr.VIN , "0123456789" );

  sr.VPST  = 123;
  sr.TS    = egts_set_msd_ts( 3, 4, 5, 6, 7, 2011 );

  sr.PLAT  = egts_set_msd_coo( -123, 45, 45, 666 );
  sr.PLON  = egts_set_msd_coo(  123, 45, 45, 666 );
  sr.VD    = 123;

  sr.pvr_latd1  = 1;
  sr.RVP_LATD_1 = egts_set_msd_coo16( 12, 34, 500 );

  sr.pvr_lond1  = 1;
  sr.RVP_LOND_1 = egts_set_msd_coo16( -12, 34, 500 );

  sr.pvr_latd2  = 1;
  sr.RVP_LATD_2 = egts_set_msd_coo16( 12, 34, 500 );

  sr.pvr_lond2  = 1;
  sr.RVP_LOND_2 = egts_set_msd_coo16( -12, 34, 500 );

  sr.nop  = 1;
  sr.NOP  = 5;

  sr.ad   = 1;
  memset( sr.AD , 0 , sizeof(sr.AD) );
  strcpy( (char*)sr.AD , "0123456789" );
  sr.AD_len = 10;
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_MSD_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_MSD_DATA (simple) ... ");
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
/* send_test_ECALL_MSD_DATA_n()                                              */
/*                                                                           */
/* Description: Test function for ECALL_MSD_DATA packet                      */
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

void send_test_ECALL_MSD_DATA_n(
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

  egts_ECALL_MSD_DATA_t   sr;

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
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.FV   = 1;
  sr.MI   = 1;
  
  sr.VT     = 1;   
  sr.POCN   = 1;   
  sr.CLT    = 1;
  sr.ACT    = 1;

  memset( sr.VIN , 0x00 , sizeof(sr.VIN) );
  strcpy( (char*)sr.VIN , "0123456789" );

  sr.VPST  = 123;
  sr.TS    = egts_set_msd_ts( 3, 4, 5, 6, 7, 2011 );

  sr.PLAT  = egts_set_msd_coo( -123, 45, 45, 666 );
  sr.PLON  = egts_set_msd_coo(  123, 45, 45, 666 );
  sr.VD    = 123;

  sr.pvr_latd1  = 0;

  sr.pvr_lond1  = 0;

  sr.pvr_latd2  = 0;

  sr.pvr_lond2  = 0;

  sr.nop  = 0;

  sr.ad   = 0;

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_MSD_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_MSD_DATA (min length) ... ");
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
/* send_test_ECALL_MSD_DATA_x()                                              */
/*                                                                           */
/* Description: Test function for ECALL_MSD_DATA packet                      */
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

void send_test_ECALL_MSD_DATA_x(
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

  egts_ECALL_MSD_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.FV   = 1;
  sr.MI   = 1;
  
  sr.VT     = 1;   
  sr.POCN   = 1;   
  sr.CLT    = 1;
  sr.ACT    = 1;

  memset( sr.VIN , '0' , sizeof(sr.VIN) );

  sr.VPST  = 123;
  sr.TS    = egts_set_msd_ts( 3, 4, 5, 6, 7, 2011 );

  sr.PLAT  = egts_set_msd_coo( -123, 45, 45, 666 );
  sr.PLON  = egts_set_msd_coo(  123, 45, 45, 666 );
  sr.VD    = 123;

  sr.pvr_latd1  = 1;
  sr.RVP_LATD_1 = egts_set_msd_coo16( -12, 34, 500 );

  sr.pvr_lond1  = 1;
  sr.RVP_LOND_1 = egts_set_msd_coo16( 12, 34, 500 );

  sr.pvr_latd2  = 1;
  sr.RVP_LATD_2 = egts_set_msd_coo16( -12, 34, 500 );

  sr.pvr_lond2  = 1;
  sr.RVP_LOND_2 = egts_set_msd_coo16( 12, 34, 500 );

  sr.nop  = 1;
  sr.NOP  = 5;

  sr.ad   = 1;
  memset( sr.AD , '0' , EGTS_MDS_AD_LEN );
  sr.AD_len = EGTS_MDS_AD_LEN;
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_MSD_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_MSD_DATA (max length) ... ");
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
/* send_test_ECALL_RAW_MSD_DATA_1()                                          */
/*                                                                           */
/* Description: Test function for ECALL_RAW_MSD_DATA packet                  */
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

void send_test_ECALL_RAW_MSD_DATA_1(
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

  egts_ECALL_RAW_MSD_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.FM   = EGTS_MDS_FM_UNKNOWN;
  sr.MSD_len = 10;
  memset( sr.MSD , 'S' , sr.MSD_len );
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_RAW_MSD_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_RAW_MSD_DATA (simple) ... ");
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
/* send_test_ECALL_RAW_MSD_DATA_2()                                          */
/*                                                                           */
/* Description: Test function for ECALL_RAW_MSD_DATA packet                  */
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

void send_test_ECALL_RAW_MSD_DATA_2(
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

  egts_ECALL_RAW_MSD_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.FM   = EGTS_MDS_FM_UNKNOWN;
  sr.MSD_len = EGTS_RAW_MDS_LEN;
  memset( sr.MSD , 'S' , sr.MSD_len );
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_RAW_MSD_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_RAW_MSD_DATA (max length) ... ");
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
/* send_test_ECALL_TRACK_DATA_1()                                            */
/*                                                                           */
/* Description: Test function for ECALL_TRACK_DATA packet                    */
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

void send_test_ECALL_TRACK_DATA_1(
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

  egts_ECALL_TRACK_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.SA   = 1U;
  sr.ATM  = 300000U;
  sr.TDS[0].TNDE = 0;
  sr.TDS[0].RTM  = 0;
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_TRACK_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_TRACK_DATA (min length) ... ");
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
/* send_test_ECALL_TRACK_DATA_2()                                            */
/*                                                                           */
/* Description: Test function for ECALL_TRACK_DATA packet                    */
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

void send_test_ECALL_TRACK_DATA_2(
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
  u16 i;
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_ECALL_TRACK_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.SA   = 10U;
  sr.ATM  = 300000U;
  for ( i = 0; i < sr.SA; i++ )
  {

    sr.TDS[i].TNDE = 1;
    sr.TDS[i].LOHS = 1;
    sr.TDS[i].LAHS = 1;
    sr.TDS[i].RTM  = 10+i;

    sr.TDS[i].LAT   = 10000U * i;
    sr.TDS[i].LONG  = 10100U * i;
    sr.TDS[i].SPD   = 1000U * i;
    sr.TDS[i].DIR   = 100U + 20*i;

  }
  
  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_TRACK_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_TRACK_DATA (10 items) ... ");
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
/* send_test_ECALL_TRACK_DATA_3()                                            */
/*                                                                           */
/* Description: Test function for ECALL_TRACK_DATA packet                    */
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

void send_test_ECALL_TRACK_DATA_3(
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
  u16 i;
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_ECALL_TRACK_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.SA   = 25U;
  sr.ATM  = 300000U;

  for ( i = 0; i < 10; i++ )
  {

    sr.TDS[i].TNDE = 1;
    sr.TDS[i].LOHS = 1;
    sr.TDS[i].LAHS = 1;
    sr.TDS[i].RTM  = 5+i;

    sr.TDS[i].LAT   = 10000U * i;
    sr.TDS[i].LONG  = 10100U * i;
    sr.TDS[i].SPD   = 1000U * i;
    sr.TDS[i].DIR   = 100U + 10*i;
  }

  for ( ; i < 15; i++ )
  {

    sr.TDS[i].TNDE = 0;
    sr.TDS[i].RTM  = 10+i;

  }

  for ( i = 0; i < 25; i++ )
  {

    sr.TDS[i].TNDE = 1;
    sr.TDS[i].LOHS = 1;
    sr.TDS[i].LAHS = 1;
    sr.TDS[i].RTM  = 5+i;

    sr.TDS[i].LAT   = 10000U * i;
    sr.TDS[i].LONG  = 10100U * i;
    sr.TDS[i].SPD   = 1000U * i;
    sr.TDS[i].DIR   = 100U + 10*i;
  }

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_TRACK_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_TRACK_DATA (10 items, gap 5 items , 10 items) ... ");
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
/* send_test_ECALL_TRACK_DATA_4()                                            */
/*                                                                           */
/* Description: Test function for ECALL_TRACK_DATA packet                    */
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

void send_test_ECALL_TRACK_DATA_4(
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
  u16 i;
  egts_record_t               record;
  egts_subrecord_t            subrecord;

  egts_ECALL_TRACK_DATA_t   sr;

  record.record.RL   = 0;  /* calculated automaticaly */
  record.record.RN   = 1;
  record.record.SSOD = 0;
  record.record.RSOD = 0;
  record.record.GRP  = 0;
  record.record.RPP  = 0;
  record.record.TMFE = 0;   record.record.TM   = 0;
  record.record.EVFE = 0;   record.record.EVID = 0;
  record.record.OBFE = 0;   record.record.OID  = 0;
  record.record.SST  = EGTS_ECALL_SERVICE;
  record.record.RST  = EGTS_ECALL_SERVICE;

  /**************************************
  *
  */

  sr.SA   = 255U;
  sr.ATM  = 300000U;

  for ( i = 0; i < 255; i++ )
  {

    sr.TDS[i].TNDE = 1;
    sr.TDS[i].LOHS = 1;
    sr.TDS[i].LAHS = 1;
    sr.TDS[i].RTM  = (u8)(( 5U + i ) & 0x1F);

    sr.TDS[i].LAT   = 1000U * i;
    sr.TDS[i].LONG  = 1010U * i;
    sr.TDS[i].SPD   = 100U * i;
    sr.TDS[i].DIR   = 100U + i;
  }

  /**************************************
  *
  */

  /* subrec.subrecord.SRL - auto */
  subrecord.subrecord.SRT = EGTS_SR_TRACK_DATA;
  subrecord.SRD           = (void*)&sr;
  record.psubrecords = &subrecord;
  record.nsubrecords = 1;
  
  probes_ctx.presponce = presponce;
  probes_ctx.precords = &record;
  probes_ctx.nrecords = 1;
  probes_ctx.last_result = 0;
  egts_probe_printf("ECALL.EGTS_SR_TRACK_DATA (all items) ... ");
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
