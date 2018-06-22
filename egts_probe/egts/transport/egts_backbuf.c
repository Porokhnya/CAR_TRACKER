/*****************************************************************************/
/*                                                                           */
/* File: egts_backbuf.c                                                      */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Transport Layer                                           */
/*                                                                           */
/* Status: Version 1.1                                                       */
/*                                                                           */
/* Language: C                                                               */
/*	                                                                     */
/* (c) Copyright JSC «Navigation-information systems», 2011                  */
/*                                                                           */
/* Address:                                                                  */
/*     24, Mishina Str., bld.1                                               */
/*     Moscow, Russia                                                        */
/*                                                                           */
/* Description: Back-buffer implaementation. Back-buffer is a ring buffer    */
/*   to keep received data for parser rollbacks.                             */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"
#include "../include/egts.h"
#include "../include/egts_impl.h"

#include "egts_backbuf.h"
#include "egts_guts.h"

/*****************************************************************************/
/*                                                                           */
/* egts_push()                                                               */
/*                                                                           */
/* Description: Push byte to back-buffer at tail                             */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            uc - byte to push                                              */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_push( egts_state_t* estate , u8 uc )
{
#ifdef EGTS_DBG
  u32 new_tail;
  new_tail = ( estate->backbuf_tail + 1U ) % EGTS_BACK_BUF_SIZE_1;
  if ( new_tail == estate->backbuf_head )
  {
    egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "backbuf overflow" );
    estate->backbuf_head = (estate->backbuf_head + 1U ) % EGTS_BACK_BUF_SIZE_1;
  }
  estate->backbuf[ estate->backbuf_tail ] = uc;
  estate->backbuf_tail = new_tail;
#else
  estate->backbuf[ estate->backbuf_tail ] = uc;
  estate->backbuf_tail = ( estate->backbuf_tail + 1U ) % EGTS_BACK_BUF_SIZE_1;
#endif
}

/*****************************************************************************/
/*                                                                           */
/* egts_pushback()                                                           */
/*                                                                           */
/* Description: Push byte to back-buffer before current parser position      */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            uc - byte to push                                              */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_pushback( egts_state_t* estate , u8 uc )
{
  u32 new_cur;
  new_cur = ( EGTS_BACK_BUF_SIZE_1 + estate->backbuf_cur - 1U ) % EGTS_BACK_BUF_SIZE_1;
#ifdef EGTS_DBG
  if ( estate->backbuf_cur == estate->backbuf_head  )
  {
    egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "pushback out of range" );
    estate->backbuf_head = new_cur;
  }
#endif
  estate->backbuf[ new_cur ] = uc;
  estate->backbuf_cur = new_cur;
}

/*****************************************************************************/
/*                                                                           */
/* egts_pop()                                                                */
/*                                                                           */
/* Description: Pop byte from back-buffer from current parser position       */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            puc - byte pushed                                              */
/*                                                                           */
/* Return:    zero on OK, nonzero when no data available                     */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

int  egts_pop( egts_state_t* estate , u8* puc )
{
  if ( estate->backbuf_cur == estate->backbuf_tail ) {
    return 0;
  }
  *puc = estate->backbuf[ estate->backbuf_cur ];
  estate->backbuf_cur = ( estate->backbuf_cur + 1U ) % EGTS_BACK_BUF_SIZE_1;
  return 1;
}

/*****************************************************************************/
/*                                                                           */
/* egts_rewind()                                                             */
/*                                                                           */
/* Description: Reset current parser position back to begin and skip nskip   */
/*   bytes                                                                   */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*            nskip - byte count to skip                                     */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_rewind( egts_state_t* estate , u32 nskip )
{
#ifdef EGTS_DBG
  u32 new_cur;
  new_cur = ( estate->backbuf_head + nskip ) % EGTS_BACK_BUF_SIZE_1;
  if ( ( ( EGTS_BACK_BUF_SIZE_1 + new_cur - estate->backbuf_head ) % EGTS_BACK_BUF_SIZE_1 ) >=  
       ( ( EGTS_BACK_BUF_SIZE_1 + estate->backbuf_tail - estate->backbuf_head ) % EGTS_BACK_BUF_SIZE_1 ) )
  {
    egts_rx_error( estate , 0U , EGTS_PC_UNKNOWN , "rewind out of range" );
    new_cur = estate->backbuf_tail; 
  }
  estate->backbuf_cur = new_cur;
#else  
  estate->backbuf_cur = ( estate->backbuf_head + nskip ) % EGTS_BACK_BUF_SIZE_1;
#endif
  estate->backbuf_head = estate->backbuf_cur;
}

/*****************************************************************************/
/*                                                                           */
/* egts_sync()                                                               */
/*                                                                           */
/* Description: Drop back-buffer history                                     */
/*                                                                           */
/* Arguments: estate - protocol instance                                     */
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

void  egts_sync( egts_state_t* estate )
{
  estate->backbuf_head = estate->backbuf_cur;
}

/******************************************************************************
*
*/
