#include <egts_config.h>
#include <egts.h>
#include <egts_impl.h>
#include "egts_probe.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T> inline Stream& operator << (Stream &s, T n) { s.print(n); return s; }
#define ENDLINE "\r\n"
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define BUFFER_SIZE 1024
//----------------------------------------------------------------------------------------------------------------------------------------------------
egts_state_t  estate_rx;
egts_state_t  estate_tx;
u8    temp_buf[ BUFFER_SIZE ];
egts_probe_ctx_t       probes_ctx;
//----------------------------------------------------------------------------------------------------------------------------------------------------
int  estate_tx_buffer_loop( void* ctx , void* pbuf , u32 sz ) 
{
  u8 *puc = (u8*)pbuf;

  for ( ; sz; sz-- )
    egts_rx_byte( (egts_state_t*)ctx, *puc++ );

  return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void  estate_tx_error( void* actx , u16 PID , u8 err , const char* dgb_str )
{
  (void)actx;
  (void)PID;
  (void)err;

  if ( dgb_str )
  {
    Serial << F("tx error: ") << dgb_str << ENDLINE;
    //printf( "tx error: %s\n" , dgb_str );
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void  estate_rx_error( void* actx , u16 PID , u8 err , const char* dgb_str )
{
  (void)actx;
  (void)PID;
  (void)err;

  if ( dgb_str )
  {
    Serial << F("rx error: ") << dgb_str << ENDLINE;
   // printf( "rx error: %s\n" , dgb_str );
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void egts_probe_printf( const char* fmt , ... )
{
  /*
  va_list valist;
  va_start(v,fmt);
  vprintf( fmt , v );
  */
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(57600);

  egts_profile_t def_profile;
  u8             def_priority;
  egts_route_t   test_route;
  u16            PID;
  const tf_probe_send*      pfn_probe_send;
  egts_responce_header_t    responce;

  def_profile.CMP  = 0;
  def_profile.ENA  = EGTS_DATA_ENA_NONE;
  def_profile.HE   = EGTS_HEADER_ENCODING_NONE;
  def_profile.SGN  = 0;
  def_profile.SKID = 0;

  def_priority     = 0;

  test_route.PRA   = 1;
  test_route.RCA   = 1;
  test_route.TTL   = 1;

  probes_ctx.precords    = NULL;
  probes_ctx.nrecords    = 0;
  probes_ctx.nerr        = 0;
  probes_ctx.last_result = 0;

  estate_rx.cur_rx_packet = (egts_packet_state_t*)malloc( sizeof(egts_packet_state_t) );

  egts_init( &estate_rx, &probes_ctx , 
    NULL , 
    estate_rx_packet_probe ,
    estate_tx_error ,
    estate_rx_error
    );

  estate_tx.cur_tx_packet = (egts_packet_state_t*)malloc( sizeof(egts_packet_state_t) );

  egts_init( &estate_tx, &estate_rx , 
    estate_tx_buffer_loop ,
    NULL ,
    estate_tx_error ,
    estate_rx_error
    );

  /*
  *
  */


  PID = 1;
  responce.PR   = EGTS_PC_OK;
  responce.RPID = 1000;

  Serial << ENDLINE;
  Serial << "Header encoding - NONE" << ENDLINE;
  Serial << "Data encoding - NONE" << ENDLINE;
  Serial << "Compression - NONE" << ENDLINE;
  Serial << "Sign-up - NONE" << ENDLINE;
  Serial << "Responce - NONE"  << ENDLINE;
  Serial << ENDLINE;
  
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, NULL , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }


  Serial << ENDLINE;
  Serial << "Header encoding - NONE" << ENDLINE;
  Serial << "Data encoding - NONE" << ENDLINE;
  Serial << "Compression - NONE" << ENDLINE;
  Serial << "Sign-up - NONE" << ENDLINE;
  Serial << "Responce - YES" << ENDLINE;
  Serial << ENDLINE;
  
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, &responce , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }

  def_profile.HE   = EGTS_HEADER_ENCODING_DBG;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - NONE" << ENDLINE;
  Serial << "Compression - NONE" << ENDLINE;
  Serial << "Sign-up - NONE" << ENDLINE;
  Serial << "Responce - NONE" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, NULL , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }

  def_profile.HE   = EGTS_HEADER_ENCODING_DBG;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - NONE" << ENDLINE;
  Serial << "Compression - NONE" << ENDLINE;
  Serial << "Sign-up - NONE" << ENDLINE;
  Serial << "Responce - YES" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, &responce , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }


  def_profile.HE   = EGTS_HEADER_ENCODING_DBG;
  def_profile.ENA  = EGTS_DATA_ENA_DBG;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - DBG" << ENDLINE;
  Serial << "Compression - NONE" << ENDLINE;
  Serial << "Sign-up - NONE" << ENDLINE;
  Serial << "Responce - NONE" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, NULL , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }

  def_profile.ENA  = EGTS_DATA_ENA_DBG;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - DBG" << ENDLINE;
  Serial << "Compression - NONE" << ENDLINE;
  Serial << "Sign-up - NONE" << ENDLINE;
  Serial << "Responce - YES" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, &responce ,  temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }


  def_profile.CMP  = 1;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - DBG" << ENDLINE;
  Serial << "Compression - DBG" << ENDLINE;
  Serial << "Sign-up - NONE" << ENDLINE;
  Serial << "Responce - NONE" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, NULL , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }

  def_profile.CMP  = 1;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - DBG" << ENDLINE;
  Serial << "Compression - DBG" << ENDLINE;
  Serial << "Sign-up - NONE" << ENDLINE;
  Serial << "Responce - YES" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, &responce , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }

  def_profile.ENA  = EGTS_DATA_ENA_NONE;
  def_profile.CMP  = 0;
  def_profile.SGN  = 1;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - NONE" << ENDLINE;
  Serial << "Compression - NONE" << ENDLINE;
  Serial << "Sign-up - DBG" << ENDLINE;
  Serial << "Responce - NONE" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, NULL , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }

  def_profile.CMP  = 0;
  def_profile.SGN  = 1;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - DBG" << ENDLINE;
  Serial << "Compression - NONE" << ENDLINE;
  Serial << "Sign-up - DBG" << ENDLINE;
  Serial << "Responce - NONE" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, NULL , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }

  def_profile.SGN  = 1;
  
  Serial << ENDLINE;
  Serial << "Header encoding - DBG" << ENDLINE;
  Serial << "Data encoding - DBG" << ENDLINE;
  Serial << "Compression - DBG" << ENDLINE;
  Serial << "Sign-up - DBG" << ENDLINE;
  Serial << "Responce - NONE" << ENDLINE;
  Serial << ENDLINE;
  
  PID = 1;
  for ( pfn_probe_send = egts_probes_vector;
        *pfn_probe_send;
        pfn_probe_send++ )
  {
    (*pfn_probe_send)( &estate_tx , &def_profile , def_priority , NULL , PID, NULL , temp_buf , ((BUFFER_SIZE)-1) );
    PID++;
  }


  free( estate_rx.cur_rx_packet );
  free( estate_tx.cur_tx_packet );

  Serial << ENDLINE;
  Serial << "erorr count: " << probes_ctx.nerr;
  if ( probes_ctx.nerr )
  {
    Serial << "autotests FAILED" << ENDLINE;
    return;
  }

  Serial << "autotests PASSED"  << ENDLINE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void loop() 
{
  // put your main code here, to run repeatedly:

}
//----------------------------------------------------------------------------------------------------------------------------------------------------

