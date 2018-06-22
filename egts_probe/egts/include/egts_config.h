/*****************************************************************************/
/*                                                                           */
/* File: egts_config.h                                                       */
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
/* Description: This header contains the set of macro for customisation and  */
/*   porting.                                                                */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: -                                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_config_h
#define egts_config_h

/******************************************************************************
*
*/


#include <string.h>
#include <stdio.h>

/******************************************************************************
*  Macro switches CPU architecture.
*    EGTS_CPU_LE  - little-endian
*    EGTS_CPU_BE  - big-endian
*/

#define EGTS_CPU_LE

/******************************************************************************
* Macro enables debug features. For release build EGTS_DBG can be undefined.
*/

#define EGTS_DBG

/******************************************************************************
* Macro defines debug log routine. Without debug log macro can be defined
*   as 
* #define egts_dbg_printf(s)   do {} while(0)
* 
*/

#define egts_dbg_printf(s)   do { printf("%s\n",(s)); } while(0)

/******************************************************************************
* Macro EGTS_ALIGN enable CPU architecture align. When EGTS_ALIGN defined
    also musr me deined EGTS_ALIGN_MASK.

    align 2  -  EGTS_ALIGN_MASK   0x0001
    align 4  -  EGTS_ALIGN_MASK   0x0003
    align 8  -  EGTS_ALIGN_MASK   0x0007

#define EGTS_ALIGN            1
#define EGTS_ALIGN_MASK       0x0007

*/


/******************************************************************************
*
*/


#endif

