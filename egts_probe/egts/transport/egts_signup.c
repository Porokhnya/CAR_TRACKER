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

#include "../include/egts_config.h"

#include "../include/egts.h"

#include "egts_signup.h"

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

int   egts_signup_data       (  void* buf , u16 buf_size , egts_signature_header_t* signup )
{
  (void)buf;
  (void)buf_size;

#ifdef EGTS_DBG
  /* just 10b of spare */
  signup->SIGL = 10U;
  memcpy( signup->SIGD , "0123456789" , 10U );
#endif
  return 0;
}

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

int   egts_check_signup_data (  void* buf , u16 buf_size , egts_signature_header_t* signup )
{

  (void)buf;
  (void)buf_size;


#ifdef EGTS_DBG
  if ( signup->SIGL != 10U ) {
    return -1;
  }
  if ( 0 != memcmp( signup->SIGD , "0123456789" , 10U ) ) {
    return -1;
  }
#endif
  return 0;
}

/******************************************************************************
*
*/

