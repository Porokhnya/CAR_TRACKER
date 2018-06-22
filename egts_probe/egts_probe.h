/*****************************************************************************/
/*                                                                           */
/* File: egts_probe.h                                                        */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Tests                                                     */
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
/* Description: Misceleniouse declarations for autotests. It is not a part   */
/*   for mandatory EGTS sources.                                             */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions:                                                                */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_probe_h
#define egts_probe_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

/*****************************************************************************
*
*/

typedef void (*tf_probe_send)(
  egts_state_t*   estate ,
  egts_profile_t* pprofile ,
  u8              PR , 
  egts_route_t*   proute ,
  u16             PID, 
  egts_responce_header_t* presponce ,
  void*           ptemp_buf ,
  u16             temp_buf_sz
  );

typedef struct
{
  egts_responce_header_t*   presponce;
  egts_record_t*            precords;
  u16                       nrecords;
  int                       nerr;
  int                       last_result;
} egts_probe_ctx_t;

/*****************************************************************************/
/*                                                                           */
/* estate_rx_packet_probe()                                                  */
/*                                                                           */
/* Description: Probe analizer. For special probes only.                     */
/*                                                                           */
/* Arguments: ctx - call context                                             */
/*            pheader - transport header                                     */
/*            signed_up - signature mark, 1/0                                */
/*            presponce - responce data                                      */
/*            precords - array of records                                    */
/*            nrecords - record count                                        */
/*            FDL - frame data length                                        */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

extern int  estate_rx_packet_probe( void* ctx , 
  egts_header_t*            pheader , 
  u8                        signed_up ,
  egts_responce_header_t*   presponce ,
  egts_record_t*            precords ,
  u16                       nrecords ,
  u16                       FDL );

extern egts_probe_ctx_t       probes_ctx;

extern const tf_probe_send  egts_probes_vector[];

/*****************************************************************************/
/*                                                                           */
/* egts_probe_printf()                                                       */
/*                                                                           */
/* Description: Probe debug callback. For special probes only.               */
/*                                                                           */
/* Arguments: fmt - format specifier                                         */
/*            ... - some arguments                                           */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

extern void egts_probe_printf( const char* fmt , ... );


/******************************************************************************
*
*/

#ifdef  __cplusplus
}
#endif

/*****************************************************************************
*
*/

#endif



