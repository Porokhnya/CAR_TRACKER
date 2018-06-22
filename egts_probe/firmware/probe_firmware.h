/*****************************************************************************/
/*                                                                           */
/* File: probe_filrmware.h                                                   */
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

#ifndef probe_firmware_h
#define probe_firmware_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

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

extern void send_test_FIRMWARE_PART_DATA_1(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  );

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

extern void send_test_FIRMWARE_PART_DATA_2(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  );

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

extern void send_test_FIRMWARE_PART_DATA_1x(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  );

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

extern void send_test_FIRMWARE_PART_DATA_2x(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  );

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

extern void send_test_FIRMWARE_FULL_DATA(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  );

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

extern void send_test_FIRMWARE_FULL_DATA_x(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  );

/******************************************************************************
*
*/

#ifdef  __cplusplus
}
#endif


/******************************************************************************
*
*/

#endif


