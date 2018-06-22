/*****************************************************************************/
/*                                                                           */
/* File: egts_signup.c                                                       */
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
/* Description: Functions for digital signing data in EGTS.                  */ 
/*   Unimplemented properly yet, till real digital sign algorithm will de    */
/*   clarified. For debug purpos, simple predefined sign implemented under   */
/*   EGTS_DBG macro.                                                         */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_signup_h
#define egts_signup_h

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/* egts_signup_data()                                                        */
/*                                                                           */
/* Description: Create data signature. Do nothing in current version.        */
/*                                                                           */
/* Arguments: buf - buffer to signup                                         */
/*            buf_size - buffer size                                         */
/*            signup - record for signatupre data                            */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_signup_data       ( void* buf , u16 buf_size , 
  egts_signature_header_t* signup );

/*****************************************************************************/
/*                                                                           */
/* egts_push()                                                               */
/*                                                                           */
/* Description: Check data signature. Do nothing in current version.         */
/*                                                                           */
/* Arguments: buf - buffer for checking                                      */
/*            buf_size - buffer size                                         */
/*            signup - record with signatupre data                           */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern int   egts_check_signup_data ( void* buf , u16 buf_size , 
  egts_signature_header_t* signup );

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
