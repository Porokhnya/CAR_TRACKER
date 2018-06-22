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
/*                                                                           */
/* (c) Copyright JSC «Navigation-information systems», 2011                  */
/*                                                                           */
/* Address:                                                                  */
/*     24, Mishina Str., bld.1                                               */
/*     Moscow, Russia                                                        */
/*                                                                           */
/* Description: Back-buffer implementation. Back-buffer is a ring buffer     */
/*   to keep received data for parser rollbacks.                             */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_backbuf_h
#define egts_backbuf_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

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

extern void  egts_push( egts_state_t* estate , u8 uc );

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

extern void  egts_pushback( egts_state_t* estate , u8 uc );

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

extern int   egts_pop( egts_state_t* estate , u8* puc );

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

extern void  egts_rewind( egts_state_t* estate , u32 nskip );

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

extern void  egts_sync( egts_state_t* estate );

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
