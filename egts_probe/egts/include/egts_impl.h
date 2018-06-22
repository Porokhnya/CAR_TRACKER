/*****************************************************************************/
/*                                                                           */
/* File: egts_impl.h                                                         */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Protocol EGTS                                             */
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
/* Description: This header contains all privat and public declaration for   */
/*   protocol stack implementation.                                          */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: public : egts_init, egts_tx_packet.                            */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_impl_h
#define egts_impl_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif


/******************************************************************************
*
*/

typedef struct
{
  egts_service_data_subrecord_t    subrecord;
  void*                            SRD;
} egts_subrecord_t;

typedef struct
{
  egts_service_data_record_t                 record;
  egts_subrecord_t*                          psubrecords;
  u16                                        nsubrecords;
} egts_record_t;

/******************************************************************************
*
*/

typedef enum 
{
  est_unknown = 0 ,
  est_begin ,
  est_PRV ,
  est_SKID ,
  est_BITS ,
  est_HL ,
  est_HE ,
  est_HCS ,
  est_SFRD ,
  est_SFRCS 
} egts_packet_state ;


/******************************************************************************
*
*/

/* TODO: find out and determine */
#ifndef EGTS_MAX_PACKET_RECORDS
#define EGTS_MAX_PACKET_RECORDS   32U
#endif

/* TODO: find out and determine */
#ifndef EGTS_MAX_PACKET_SUBRECORDS
#define EGTS_MAX_PACKET_SUBRECORDS   64U
#endif

#define EGTS_SFDR_BUF_SIZE           EGTS_MAX_SFDR_LEN

#define EGTS_BACK_BUF_SIZE_1   (EGTS_BACK_BUF_SIZE+1U)

/******************************************************************************
*
*/

typedef struct
{

  egts_packet_state         rx_state;
  u32                       rx_state_len;
  u16                       calc_SFRCS;


  egts_header_t             header;
  u8                        header_encoded[ EGTS_HEADER_LEN_ENCODED_MAX_LEN ];
  u8                        header_encoded_len; 

  egts_responce_header_t    responce_header;
  egts_signature_header_t   signature_header;
  u8                        have_responce;
  u8                        have_signature;

  u8                        SFRD_encoded[ EGTS_SFDR_BUF_SIZE ];
  u8                        SFRD[ EGTS_SFDR_BUF_SIZE ];
  u8*                       SFRD_buf;
  u16                       SFRD_len;
  /* temp value */
  u16                       SFRD_elen;

  u16                       SFRCS;

  egts_record_t             records[ EGTS_MAX_PACKET_RECORDS ];
  u16                       nrecords;
  egts_subrecord_t          subrecords[ EGTS_MAX_PACKET_SUBRECORDS ];
  u16                       nsubrecords_used;
  u8                        subrecords_data[ EGTS_SFDR_BUF_SIZE ];
  u16                       nsubrecords_data_used;

} egts_packet_state_t;



/******************************************************************************
*
*/

typedef struct
{


  egts_packet_state_t*      cur_rx_packet;
  egts_packet_state_t*      cur_tx_packet;
  
  /* provision for future 
  egts_packet_state_t       rx_packets[ EGTS_RX_QUEUE_SIZE ];
  egts_packet_state_t       tx_packets[ EGTS_TX_QUEUE_SIZE ];
  */

  u8                        backbuf[ EGTS_BACK_BUF_SIZE_1 ];
  u32                       backbuf_head;
  u32                       backbuf_cur;
  u32                       backbuf_tail;

  u32                       rx_error_count;
  u32                       tx_error_count;
  u8                        last_rx_error;
  u8                        last_tx_error;

  /* TODO: may be extended */
  void* ctx;
  
  int  (* fn_tx_buffer)( void* actx , void* pbuf , u32 sz );
  int  (* fn_rx_packet)( void* actx , 
    egts_header_t*            pheader , 
    u8                        signed_up ,
    egts_responce_header_t*   presponce ,
    egts_record_t*            precords ,
    u16                       nrecords ,
    u16                       FDL );
  void  (* fn_tx_error)( void* actx , u16 PID , u8 err , const char* dgb_str );
  void  (* fn_rx_error)( void* actx , u16 PID , u8 err , const char* dgb_str );

} egts_state_t;

/*****************************************************************************/
/*                                                                           */
/* egts_init()                                                               */
/*                                                                           */
/* Description: Initialization of protocol instance.                         */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            ctx - Opaque context for callback calls                        */
/*            fn_tx_buffer - Callback function for send real data to checnal.*/
/*                           Provided by host application.                   */
/*            fn_rx_buffer - Callback function for process received packet.  */ 
/*            fn_tx_error - Callback function for transmit errors            */
/*            fn_rx_error - Callback function for receiving errors           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern void  egts_init( egts_state_t* estate ,
  void* ctx ,
  int  (* fn_tx_buffer)( void* actx , void* pbuf , u32 sz ) ,
  int  (* fn_rx_packet)( void* actx , 
    egts_header_t*            pheader , 
    u8                        signed_up ,
    egts_responce_header_t*   presponce ,
    egts_record_t*            precords ,
    u16                       nrecords ,  
    u16   FDL ),
  void  (* fn_tx_error)( void* actx , u16 PID , u8 err , const char* dgb_str ),
  void  (* fn_rx_error)( void* actx , u16 PID , u8 err , const char* dgb_str )
  );

/*****************************************************************************/
/*                                                                           */
/* egts_reset_errors()                                                       */
/*                                                                           */
/* Description: Reset error counters.                                        */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern void  egts_reset_errors( egts_state_t* estate );

/*****************************************************************************/
/*                                                                           */
/* egts_rx_byte()                                                            */
/*                                                                           */
/* Description: Function for passing received data to protocol.              */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            uc - received byte                                             */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern void  egts_rx_byte( egts_state_t* estate , u8 uc );

/*****************************************************************************/
/*                                                                           */
/* egts_tx_packet()                                                          */
/*                                                                           */
/* Description: Function for transmit packet.                                */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            pprofile - transmit settings                                   */
/*            PR - packet priority                                           */
/*            proute - packet routing settings                               */
/*            PID - packet identifier                                        */
/*            presponce - optional response data                             */
/*            precords - array of recors to transmit                         */
/*            nrecords - record count                                        */
/*            ptemp_buf - temporary buffer 64KB for operation                */
/*            temp_buf_sz - temporary buffer size                            */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_tx_packet( egts_state_t* estate ,
  egts_profile_t*         pprofile ,
  u8                      PR , 
  egts_route_t*           proute ,
  u16                     PID, 
  egts_responce_header_t* presponce ,
  egts_record_t*          precords ,
  u16                     nrecords ,
  void*                   ptemp_buf ,
  u16                     temp_buf_sz
  );

/*****************************************************************************/
/*                                                                           */
/* egts_set_tm()                                                             */
/*                                                                           */
/* Description: Helper function to set EGTS datetime fileds                  */
/*                                                                           */
/* Arguments: s - seconds, 0..59                                             */
/*            n - minutes, 0..59                                             */
/*            h - hours, 0..24                                               */
/*            d - day of month, 1..31                                        */
/*            m - month, 1..12                                               */
/*            y - year, >=2010                                               */ 
/*                                                                           */
/* Return:    EGTS representaion of datetime, seconds since                  */
/*   00:00:00 01.01.2010 UTC,                                                */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern u32   egts_set_tm( u8 s, u8 n, u8 h, u8 d, u8 m, u32 y );

/*****************************************************************************/
/*                                                                           */
/* egts_get_tm()                                                             */
/*                                                                           */
/* Description: Helper function to convert EGTS datetime field               */
/*                                                                           */
/* Arguments: dt - EGTS datetime                                             */
/*            ps - pointer to get seconds, 0..59                             */
/*            pn - pointer to get minutes, 0..59                             */
/*            ph - pointer to get hours, 0..24                               */
/*            pd - pointer to get day of month, 1..31                        */
/*            pm - pointer to get month, 1..12                               */
/*            py - pointer to get year, >=2010                               */ 
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern void  egts_get_tm( u32 dt , u8 *ps, u8 *pn, u8 *ph, u8 *pd, u8 *pm, u32 *py );

/*****************************************************************************/
/*                                                                           */
/* egts_set_tm()                                                             */
/*                                                                           */
/* Description: Helper function to set EGTS datetime fileds to               */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: s - seconds, 0..59                                             */
/*            n - minutes, 0..59                                             */
/*            h - hours, 0..24                                               */
/*            d - day of month, 1..31                                        */
/*            m - month, 1..12                                               */
/*            y - year, >=2010                                               */ 
/*                                                                           */
/* Return:    EGTS representaion of datetime, seconds since                  */
/*   epoche 00:00:00 01.01.1970 UTC                                          */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern u32   egts_set_msd_ts( u8 s, u8 n, u8 h, u8 d, u8 m, u32 y );

/*****************************************************************************/
/*                                                                           */
/* egts_get_tm()                                                             */
/*                                                                           */
/* Description: Helper function to convert EGTS datetime field fo            */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: dt - MSD EGTS datetime, seconds since                          */
/*                   epoche 00:00:00 01.01.1970 UTC                          */
/*            ps - pointer to get seconds, 0..59                             */
/*            pn - pointer to get minutes, 0..59                             */
/*            ph - pointer to get hours, 0..24                               */
/*            pd - pointer to get day of month, 1..31                        */
/*            pm - pointer to get month, 1..12                               */
/*            py - pointer to get year, >=2010                               */ 
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern void  egts_get_msd_ts( u32 dt , u8 *ps, u8 *pn, u8 *ph, u8 *pd, u8 *pm, u32 *py );

/*****************************************************************************/
/*                                                                           */
/* egts_set_msd_coo()                                                        */
/*                                                                           */
/* Description: Helper function to set EGTS coordinate fileds to             */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: deg - degrees with sign                                        */
/*            n - arc minutes, 0..59                                         */
/*            s - arc seconds, 0..59                                         */
/*            ms - mili arc seconds, 0..1000                                 */
/*                                                                           */
/* Return:    EGTS MSG representaion of coordinate                           */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern u32   egts_set_msd_coo( i16 deg , u8 n, u8 s , u16 ms );

/*****************************************************************************/
/*                                                                           */
/* egts_get_msd_coo()                                                        */
/*                                                                           */
/* Description: Helper function to get EGTS coordinate fileds from           */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: coo - EGTS MSD representation of coordinate                    */  
/*            pdeg - pointer to get degrees with sign                        */
/*            pn - pointer to get arc minutes, 0..59                         */
/*            ps - pointer to get arc seconds, 0..59                         */
/*            pms - pointer to get mili arc seconds, 0..1000                 */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern void  egts_get_msd_coo( u32 coo , i16 *pdeg , u8 *pn, u8 *ps , u16 *pms );

/*****************************************************************************/
/*                                                                           */
/* egts_set_msd_coo16()                                                      */
/*                                                                           */
/* Description: Helper function to set EGTS relative coordinate fileds to    */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: n - arc minutes, 0..59                                         */
/*            s - arc seconds, 0..59                                         */
/*            ms - mili arc seconds, 0..1000                                 */
/*                                                                           */
/* Return:    EGTS MSG representaion of relative coordinate                  */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern u16   egts_set_msd_coo16( i8 n, u8 s , u16 ms );

/*****************************************************************************/
/*                                                                           */
/* egts_get_msd_coo16()                                                      */
/*                                                                           */
/* Description: Helper function to get EGTS relative coordinate fileds from  */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: coo - EGTS MSD representation of relative coordinate           */  
/*            pn - pointer to get arc minutes, 0..59                         */
/*            ps - pointer to get arc seconds, 0..59                         */
/*            pms - pointer to get mili arc seconds, 0..1000                 */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

extern void  egts_get_msd_coo16( u16 coo , i8 *pn, u8 *ps , u16 *pms );

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


