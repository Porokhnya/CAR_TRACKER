/*****************************************************************************/
/*                                                                           */
/* File: egts_dump.c                                                         */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Debug                                                     */
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
/* Description: Misceleniouse debug routines. Not a part of EGTS mandatory   */
/*   source.                                                                 */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: egts_result_str                                                */
/*            egts_SST_str                                                   */
/*            egts_CT_str                                                    */
/*            egts_CC_str                                                    */
/*            egts_CHS_str                                                   */
/*            egts_ACT_str                                                   */
/*            egts_OT_str                                                    */
/*            egts_MT_str                                                    */
/*            egts_CLT_str                                                   */
/*            egts_ACType_str                                                */
/*            egts_TEST_MODE_str                                             */
/*            estate_dump_string                                             */
/*            estate_dump_string_z                                           */
/*            egts_dump_FIRMWARE_DATA_header                                 */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_dump_h
#define egts_dump_h

/******************************************************************************
*
*/

#ifdef EGTS_DBG

/******************************************************************************
*
*/

#ifdef  __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/*                                                                           */
/* egts_result_str()                                                         */
/*                                                                           */
/* Description: Return string representation of Result Code                  */
/*                                                                           */
/* Arguments: r - Result Code                                                */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_result_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_SST_str()                                                            */
/*                                                                           */
/* Description: Return string representation of Service Statement            */
/*                                                                           */
/* Arguments: r - Service Statement                                          */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_SST_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_CT_str()                                                             */
/*                                                                           */
/* Description: Return string representation of Command Type                 */
/*                                                                           */
/* Arguments: r - Command Type                                               */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_CT_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_CT_str()                                                             */
/*                                                                           */
/* Description: Return string representation of Command Confirmation Type    */
/*                                                                           */
/* Arguments: r - Command Confirmation Type                                  */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_CC_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_CHS_str()                                                            */
/*                                                                           */
/* Description: Return string representation of Charset                      */
/*                                                                           */
/* Arguments: r - Charset                                                    */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_CHS_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_CHS_str()                                                            */
/*                                                                           */
/* Description: Return string representation of command Action               */
/*                                                                           */
/* Arguments: r - Action                                                     */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_ACT_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_OT_str()                                                             */
/*                                                                           */
/* Description: Return string representation of Object Type                  */
/*                                                                           */
/* Arguments: r - Object Type                                                */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_OT_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_MT_str()                                                             */
/*                                                                           */
/* Description: Return string representation of Module Type                  */
/*                                                                           */
/* Arguments: r - Module Type                                                */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_MT_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_CLT_str()                                                            */
/*                                                                           */
/* Description: Return string representation of Call Type                    */
/*                                                                           */
/* Arguments: r - Call Type                                                  */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_CLT_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_CLT_str()                                                            */
/*                                                                           */
/* Description: Return string representation of Activation Type              */
/*                                                                           */
/* Arguments: r - Activation Type                                            */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_ACType_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* egts_CLT_str()                                                            */
/*                                                                           */
/* Description: Return string representation of Test Mode                    */
/*                                                                           */
/* Arguments: r - Test Mode                                                  */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_TEST_MODE_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* estate_dump_string()                                                      */
/*                                                                           */
/* Description: Debug helper for dump string value                           */
/*                                                                           */
/* Arguments: p_str - string to dump                                         */
/*            len - string length                                            */
/*            ctx - context of callback call                                 */
/*            fn_dump - callback for debug dump                              */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern void  estate_dump_string(
  u8*   p_str , 
  u32   len ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) );

/*****************************************************************************/
/*                                                                           */
/* estate_dump_string_z()                                                    */
/*                                                                           */
/* Description: Debug helper for dump zero-terminated string value           */
/*                                                                           */
/* Arguments: p_str - string to dump                                         */
/*            len - maximum string length                                    */
/*            ctx - context of callback call                                 */
/*            fn_dump - callback for debug dump                              */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern void  estate_dump_string_z(
  u8*   p_str , 
  u32   len ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) );

/*****************************************************************************/
/*                                                                           */
/* egts_dump_FIRMWARE_DATA_header()                                          */
/*                                                                           */
/* Description: Debug helper for dump egts_FIRMWARE_DATA_header_t stucture   */
/*                                                                           */
/* Arguments: ctx - context of callback call                                 */
/*            pv_subrecord - pointer to egts_FIRMWARE_DATA_header_t          */
/*            fn_dump - callback for debug dump                              */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

void  egts_dump_FIRMWARE_DATA_header( void* ctx , void* pv_subrecord ,
  void (* afn_dump)( void* actx , const char* fmt , ... ) );


/*****************************************************************************/
/*                                                                           */
/* egts_TELEDATA_SRC_str()                                                   */
/*                                                                           */
/* Description: Return string representation of TELEDATA SRC                 */
/*                                                                           */
/* Arguments: r - TELEDATA SRC                                               */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

extern const char* egts_TELEDATA_SRC_str( u8 r );

/*****************************************************************************/
/*                                                                           */
/* estate_dump_packet()                                                      */
/*                                                                           */
/* Description: Debug function for dump the whole packet using specified     */
/*   dump callback                                                           */
/*                                                                           */
/* Arguments: pheader - transport header                                     */
/*            signed_up - signature mark, 1/0                                */
/*            presponce - responce data                                      */
/*            precords - array of records                                    */
/*            nrecords - record count                                        */
/*            ctx - context of callback call                                 */
/*            fn_dump - callback for debug dump                              */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

void  estate_dump_packet( 
  egts_header_t*            pheader ,
  u8                        signed_up ,
  egts_responce_header_t*   presponce ,
  egts_record_t*            precords ,
  u16                       nrecords ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) 
  );

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

/******************************************************************************
*
*/

#endif 

