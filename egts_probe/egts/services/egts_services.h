/*****************************************************************************/
/*                                                                           */
/* File: egts_services.h                                                     */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Service Layer                                             */
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
/* Description: Service Layer handlers  declarations                         */
/*                                                                           */
/* Additional information:                                                   */
/*                                                                           */
/* Functions: egts_find_subrecord_handler                                    */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_services_h
#define egts_services_h


/******************************************************************************
*
*/

typedef struct
{
  u8    SRT;  /* Sub Record Type */
  u16   subrecord_size;
  u16   (* fn_get_size)( void* pv_subrecord );
  int   (* fn_read)( void* pv_subrecord , u16 SRL , u8**  ppbuf , u16*  pbuf_sz );
  int   (* fn_write)( void* pv_subrecord , u8**  ppbuf , u16*  pbuf_sz );
#ifdef EGTS_DBG
  int   (* fs_is_equial)( void* ctx , void* pv_subrecord1 , void* pv_subrecord2 );
  void  (* fn_dump)( void* ctx , void* pv_subrecord ,
    void (* afn_dump)( void* actx , const char* fmt , ... ) );
#endif
} egts_subrecord_handler_t;


/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/* egts_find_subrecord_handler()                                             */
/*                                                                           */
/* Description: Function returns handler for specified Service Type and      */
/*   Subrecord Type                                                          */
/*                                                                           */
/* Arguments: ST - Service Type                                              */
/*            SRT - Subrecord Type                                           */
/*                                                                           */
/* Return:    Requested handler if found or NULL otherwise.                  */
/*                                                                           */
/* Other:     function used internally                                       */
/*                                                                           */
/*****************************************************************************/

extern  const egts_subrecord_handler_t*  egts_find_subrecord_handler( u8 ST , u8 SRT );

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

