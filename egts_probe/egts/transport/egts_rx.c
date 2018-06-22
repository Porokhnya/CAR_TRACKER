/*****************************************************************************/
/*                                                                           */
/* File: egts_rx.c                                                           */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Transport Layer                                           */
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
/* Description: Main EGTS packet parser state mashine.                       */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: public : egts_rx_byte.                                         */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"

#include "../include/egts.h"
#include "../include/egts_impl.h"
#include "egts_guts.h"

#include "egts_crc.h"
#include "egts_header_encode.h"
#include "egts_encode.h"
#include "egts_compress.h"
#include "egts_backbuf.h"

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

void  egts_rx_byte( egts_state_t*  estate , u8 uc )
{
  egts_packet_rx_byte( estate , estate->cur_rx_packet , uc );
}

/*****************************************************************************/
/*                                                                           */
/* egts_packet_rx_byte()                                                     */
/*                                                                           */
/* Description: Function for passing received data to protocol.              */
/*                                                                           */
/* Arguments: estate - Protocol instance                                     */
/*            epacket - current packet                                       */
/*            uc - received byte                                             */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

void  egts_packet_rx_byte( 
  egts_state_t*        estate ,
  egts_packet_state_t* epacket , 
  u8 uc )
{

  egts_push( estate , uc );

  while ( 0 != egts_pop( estate , &uc ) )
  {

    switch ( epacket->rx_state )
    {

    case est_unknown :
      epacket->rx_state      = est_begin;
      epacket->rx_state_len  = 0U;
      egts_pushback( estate ,uc );
      /* egts_reset_state( estate ); */
      break;

    case est_begin :
      epacket->header.PRV = uc;
      if ( epacket->header.PRV != (u8)EGTS_VERSION )
      {
        /* remain state est_begin, just skip 1 byte */
        egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid PRV" );
        egts_sync( estate );
        break;
      }
      epacket->rx_state      = est_PRV;
      epacket->rx_state_len  = 0U;
      break;

    case est_PRV :
      epacket->header.SKID = uc;
      epacket->rx_state       = est_SKID;
      epacket->rx_state_len   = 0U;
      break;

    case est_SKID :
      egts_set_header_bits( &epacket->header , uc );
      if ( epacket->header.PRF != (u8)EGTS_PRF )
      {
        egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid PRF" );
        egts_rewind( estate , 1U );
        break;
      }
      epacket->rx_state       = est_BITS;
      epacket->rx_state_len   = 0U;
      break;

    case est_BITS :
      epacket->header.HL      = uc;
      epacket->rx_state       = est_HL;
      epacket->rx_state_len   = 0U;
      break;

    case est_HL :
      epacket->header.HE   = uc;
      if ( epacket->header.HE == EGTS_HEADER_ENCODING_NONE )
      {
        /* no encoding, check known header length */
        if ( epacket->header.RTE != 0U )
        {
          /* has optional route fields PRA,RCA,TTL (+5 byte) */
          if ( epacket->header.HL != EGTS_HEADER_LEN_WITH_ROUTE )
          {
            egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid HL (HE=0,RTE=1)" );
            egts_rewind( estate , 1U );
            break;
          }
        }
        else
        {
          /* no optional route fields */
          if ( epacket->header.HL != (u8)EGTS_HEADER_LEN_NO_ROUTE )
          {
            egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid HL (HE=0,RTE=0)" );
            egts_rewind( estate , 1U );
            break;
          }
        }
      }
      else
      {
        /* some encoding, unknown header length */
        if ( epacket->header.HL > EGTS_HEADER_LEN_MAX )
        {
          egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "too big HL (HE!=0)" );
          egts_rewind( estate , 1U );
          break;
        }
      }
  
      epacket->rx_state       = est_HE;
      epacket->rx_state_len   = 0U;

      epacket->header_encoded_len = 0U;
      break;
      

    case est_HE :
      if ( (u8)( EGTS_HEADER_LEN_FIXED + epacket->rx_state_len + 1U ) < epacket->header.HL )
      {
        epacket->header_encoded[ epacket->rx_state_len++ ] = uc;
        epacket->header_encoded_len++;
      }
      else
      {
        epacket->rx_state       = est_HCS;
        epacket->rx_state_len   = 0U;
        egts_pushback( estate ,uc );
      }
      break;


    case est_HCS :
      epacket->header.HCS   = uc; 
      if (  epacket->header.HCS == egts_header_crc( &epacket->header , 
        epacket->header_encoded , epacket->header_encoded_len ) )
      {
        if ( 0 != egts_header_decode( &epacket->header , 
          epacket->header_encoded , epacket->header_encoded_len ) )
        {
          egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "header encoding failure" );
          egts_rewind( estate , 1U );
          break;
        }
        if ( epacket->header.FDL > EGTS_MAX_SFDR_LEN )
        {
          egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "FDL too big" );
          egts_rewind( estate , 1U );
          break;
        }
        epacket->rx_state       = est_SFRD;
        epacket->rx_state_len   = 0U;
        break;
      }
      else
      {
        egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "header CRC failure" );
        egts_rewind( estate , 1U );
        break;
      }

    case est_SFRD :

      if ( epacket->header.ENA == (u8)EGTS_DATA_ENA_NONE )
      {
        /* data not encoded, write to instant buffer */
        if ( epacket->rx_state_len < epacket->header.FDL ) {
          epacket->SFRD[ epacket->rx_state_len++ ] = uc;
        } else {
          epacket->rx_state       = est_SFRCS;
          epacket->rx_state_len   = 0U;
          egts_pushback( estate ,uc );
          break;
        }
      }
      else
      {
        /* data encoded , write tor temp byffer */
        if ( epacket->rx_state_len < epacket->header.FDL ) {
          epacket->SFRD_encoded[ epacket->rx_state_len++ ] = uc;
        } else {
          epacket->rx_state       = est_SFRCS;
          epacket->rx_state_len   = 0U;
          egts_pushback( estate ,uc );
          break;
        }
      }
      break;

    case est_SFRCS :
      if ( epacket->rx_state_len < 2U )
      {
        EGTS_WRITE_FIELD(&epacket->SFRCS,uc,epacket->rx_state_len,2);
        if ( epacket->rx_state_len < 2U ) {
          break;
        }
      }

      if ( epacket->header.ENA == EGTS_DATA_ENA_NONE ) {
            epacket->calc_SFRCS = egts_data_crc( epacket->SFRD , epacket->header.FDL );
      } else {
        epacket->calc_SFRCS = egts_data_crc( epacket->SFRD_encoded , epacket->header.FDL );
      }

      if ( epacket->calc_SFRCS != epacket->SFRCS )
      {
        egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "data CRC failure" );
        egts_rewind( estate , 1U );
        break;
      }

      /* crc ok*/
      if ( epacket->header.ENA != EGTS_DATA_ENA_NONE )
      {
        if ( 0 != egts_decode_data( epacket->header.SKID , 
          epacket->SFRD , epacket->SFRD_encoded ,  
          epacket->header.FDL , &epacket->SFRD_len ) )
        {
          egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "encoding failure");
          egts_rewind( estate , 1U );
          break;
        }
      }
      else
      {
        epacket->SFRD_len = epacket->header.FDL;
      }

      /* data in SFRD */
      if ( epacket->header.CMP != 0U )
      {
        epacket->SFRD_elen = epacket->SFRD_len;
        if ( 0 != egts_decompress_data( epacket->SFRD_encoded , 
          epacket->SFRD , epacket->SFRD_elen , 
          &epacket->SFRD_len ) )
        {
          egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "decompress failure");
          egts_rewind( estate , 1U );
        }
        epacket->SFRD_buf = epacket->SFRD_encoded;
      }
      else
      {
        epacket->SFRD_buf = epacket->SFRD;
      }


      /* TODO: fb_rx_error( u8 ) */

      if ( 0 != egts_proceed_rx_packet( 
        estate , 
        epacket ,
        &epacket->header , 
        epacket->SFRD_buf ,  
        epacket->SFRD_len ) )
      {
        /* TODO: ... */
      }

      epacket->rx_state      = est_begin;
      epacket->rx_state_len  = 0U;
      egts_reset_state( epacket );
      egts_sync( estate );

      break;

    default:
      epacket->rx_state      = est_begin;
      epacket->rx_state_len  = 0U;
      egts_pushback( estate , uc );
      egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "invalid rx state");
      /* TODO: claenup packet state */
      break;

    } /* switch */

  } /* while */

}


/******************************************************************************
*
*/
