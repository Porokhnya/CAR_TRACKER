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
/*	                                                                     */
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
/* Functions: private implemenation                                          */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"

#include "../include/egts.h"
#include "../include/egts_impl.h"

#include "../services/egts_services.h"

/******************************************************************************
*
*/

#ifdef EGTS_DBG

/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* egts_dump_subrecord()                                                     */
/*                                                                           */
/* Description: Debug function for dump the subrecord using specified        */
/*   dump callback                                                           */
/*                                                                           */
/* Arguments: precord - subrecord                                            */
/*            psubrecord - subrecord                                         */
/*            ctx - context of callback call                                 */
/*            fn_dump - callback for debug dump                              */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

static
void  egts_dump_subrecord(
  egts_record_t*      precord ,
  egts_subrecord_t*   psubrecord ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) 
  );

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

const char* egts_TELEDATA_SRC_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case EGTS_SRC_TIMER_IGNITION        : pc =  "TIMER_IGNITION";  break;
  case EGTS_SRC_DISTANCE              : pc =  "DISTANCE";  break;
  case EGTS_SRC_ANGLE                 : pc =  "ANGLE";  break;
  case EGTS_SRC_RESPONSE              : pc =  "RESPONSE";  break;
  case EGTS_SRC_INPUT                 : pc =  "INPUT";  break;
  case EGTS_SRC_TIMER_NO_IGNITION     : pc =  "TIMER_NO_IGNITION";  break;
  case EGTS_SRC_PERIPHERIAL_OFF       : pc =  "PERIPHERIAL_OFF";  break;
  case EGTS_SRC_SPEED_TRESHOLD        : pc =  "SPEED_TRESHOLD";  break;
  case EGTS_SRC_CPU_RESTART           : pc =  "CPU_RESTART";  break;
  case EGTS_SRC_OUTPUT_OVERLOAD       : pc =  "OUTPUT_OVERLOAD";  break;
  case EGTS_SRC_TAMPER                : pc =  "TAMPER ";  break;
  case EGTS_SRC_BATTERY_POWER         : pc =  "BATTERY_POWER";  break;
  case EGTS_SRC_LOW_BATTERY           : pc =  "LOW_BATTERY";  break;
  case EGTS_SRC_ALARM                 : pc =  "ALARM";  break;
  case EGTS_SRC_CALL_REQUEST          : pc =  "CALL_REQUEST";  break;
  case EGTS_SRC_EMERGENCY             : pc =  "EMERGENCY";  break;
  case EGTS_SRC_EXT_SERVICE           : pc =  "EXT_SERVICE";  break;

  case EGTS_SRC_BATTERY_FAULT         : pc =  "BATTERY_FAULT";  break;
  case EGTS_SRC_EXTRA_ACC             : pc =  "EXTRA_ACC";  break;
  case EGTS_SRC_EXTRA_BREAK           : pc =  "EXTRA_BREAK";  break;
  case EGTS_SRC_ERROR_NAV             : pc =  "ERROR_NAV";  break;
  case EGTS_SRC_ERROR_SENSOR          : pc =  "ERROR_SENSOR";  break;
  case EGTS_SRC_ERROR_GSM_AERIAL      : pc =  "ERROR_GSM_AERIAL";  break;
  case EGTS_SRC_ERROR_NAV_AERIAL      : pc =  "ERROR_NAV_AERIAL";  break;
  case EGTS_SRC_LOW_SPEED             : pc =  "LOW_SPEED";  break;
  case EGTS_SRC_NO_IGNITION_MOVE      : pc =  "NO_IGNITION_MOVE";  break;
  case EGTS_SRC_TIMER_EMERGENCY       : pc =  "TIMER_EMERGENCY";  break;
  case EGTS_SRC_NAV_ON_OFF            : pc =  "NAV_ON_OFF";  break;
  case EGTS_SRC_UNSTABLE_NAV          : pc =  "UNSTABLE_NAV";  break;
  case EGTS_SRC_IP_CONNECT            : pc =  "IP_CONNECT";  break;
  case EGTS_SRC_UNSTABLE_GSM          : pc =  "UNSTABLE_GSM";  break;
  case EGTS_SRC_UNSTABLE_IP           : pc =  "UNSTABLE_IP";  break;
  case EGTS_SRC_MODE                  : pc =  "MODE";  break;

  default : break;
  }
  return pc;
}



/*****************************************************************************/
/*                                                                           */
/* egts_dump_record()                                                        */
/*                                                                           */
/* Description: Debug function for dump the record using specified           */
/*   dump callback                                                           */
/*                                                                           */
/* Arguments: precord - subrecord                                            */
/*            ctx - context of callback call                                 */
/*            fn_dump - callback for debug dump                              */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

static
void  egts_dump_record( 
  egts_record_t*           precord ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) 
  );

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

const char* egts_result_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case EGTS_PC_OK	              : pc = "OK";  break;
  case EGTS_PC_IN_PROGRESS	    : pc = "IN_PROGRESS";  break;
  case EGTS_PC_UNS_PROTOCOL	    : pc = "UNS_PROTOCOL";  break;
  case EGTS_PC_DECRYPT_ERROR    : pc = "DECRYPT_ERROR";  break;
  case EGTS_PC_PROC_DENIED      : pc = "PROC_DENIED";  break;
  case EGTS_PC_INC_HEADERFORM   : pc = "INC_HEADERFORM";  break;
  case EGTS_PC_INC_DATAFORM     : pc = "INC_DATAFORM";  break;
  case EGTS_PC_UNS_TYPE         : pc = "UNS_TYPE"; break;
  case EGTS_PC_NOTEN_PARAMS     : pc = "NOTEN_PARAMS"; break;
  case EGTS_PC_DBL_PROC         : pc = "DBL_PROC"; break;
  case EGTS_PC_PROC_SRC_DENIED  : pc = "PROC_SRC_DENIED"; break;
  case EGTS_PC_HEADERCRC_ERROR  : pc = "HEADERCRC_ERROR"; break;
  case EGTS_PC_DATACRC_ERROR    : pc = "DATACRC_ERROR"; break;
  case EGTS_PC_INVDATALEN       : pc = "INVDATALEN"; break;
  case EGTS_PC_ROUTE_NFOUND     : pc = "ROUTE_NFOUND"; break;
  case EGTS_PC_ROUTE_CLOSED     : pc = "ROUTE_CLOSED"; break;
  case EGTS_PC_ROUTE_DENIED     : pc = "ROUTE_DENIED"; break;
  case EGTS_PC_INVADDR          : pc = "INVADDR"; break;
  case EGTS_PC_TTLEXPIRED       : pc = "TTLEXPIRED"; break;
  case EGTS_PC_NO_ACK           : pc = "NO_ACK"; break;
  case EGTS_PC_OBJ_NFOUND       : pc = "OBJ_NFOUND"; break;
  case EGTS_PC_EVNT_NFOUND      : pc = "EVNT_NFOUND"; break;
  case EGTS_PC_SRVC_NFOUND      : pc = "SRVC_NFOUND"; break;
  case EGTS_PC_SRVC_DENIED      : pc = "SRVC_DENIED"; break;
  case EGTS_PC_SRVC_UNKN        : pc = "SRVC_UNKN"; break;
  case EGTS_PC_AUTH_DENIED      : pc = "AUTH_DENIED"; break;
  case EGTS_PC_ALREADY_EXISTS   : pc = "ALREADY_EXISTS"; break;
  case EGTS_PC_ID_NFOUND        : pc = "ID_NFOUND"; break;
  case EGTS_PC_INC_DATETIME     : pc = "INC_DATETIME"; break;
  case EGTS_PC_IO_ERROR         : pc = "IO_ERROR"; break;
  case EGTS_PC_NO_RES_AVAIL     : pc = "NO_RES_AVAIL"; break;
  case EGTS_PC_MODULE_FAULT     : pc = "MODULE_FAULT"; break;
  case EGTS_PC_MODULE_PWR_FLT   : pc = "MODULE_PWR_FLT"; break;
  case EGTS_PC_MODULE_PROC_FLT  : pc = "MODULE_PROC_FLT"; break;
  case EGTS_PC_MODULE_SW_FLT    : pc = "MODULE_SW_FLT"; break;
  case EGTS_PC_MODULE_FW_FLT    : pc = "MODULE_FW_FLT"; break;
  case EGTS_PC_MODULE_IO_FLT    : pc = "MODULE_IO_FLT"; break;
  case EGTS_PC_MODULE_MEM_FLT   : pc = "MODULE_MEM_FLT"; break;
  case EGTS_PC_TEST_FAILED      : pc = "TEST_FAILED"; break;
  default :
    break;
  }
  return pc;
}

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

const char* egts_SST_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case EGTS_SST_IN_SERVICE	    : pc =  "IN_SERVICE"; break;
  case EGTS_SST_OUT_OF_SERVICE  : pc =  "OUT_OF_SERVICE"; break;
  case EGTS_SST_DENIED          : pc =  "DENIED"; break;
  case EGTS_SST_NO_CONF         : pc =  "NO_CONF"; break;
  case EGTS_SST_TEMP_UNAVAIL    : pc =  "TEMP_UNAVAIL"; break;
  default:  break;
  }
  return pc;
}

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

const char* egts_CT_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case CT_COMCONF : pc =  "COMCONF"; break;
  case CT_MSGCONF : pc =  "MSGCONF"; break;
  case CT_MSGFROM : pc =  "MSGFROM"; break;
  case CT_MSGTO   : pc =  "MSGTO"; break;
  case CT_COM     : pc =  "COM"; break;
  case CT_DELCOM  : pc =  "DELCOM"; break;
  case CT_SUBREQ  : pc =  "SUBREQ"; break;
  case CT_DELIV   : pc =  "DELIV"; break;
  default: break;
  }
  return pc;
}

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

const char* egts_CC_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case CC_OK      : pc =  "OK"; break;
  case CC_ERROR   : pc =  "ERROR"; break;
  case CC_ILL     : pc =  "ILL"; break;
  case CC_DEL     : pc =  "DEL"; break;
  case CC_NFOUND  : pc =  "NFOUND"; break;
  case CC_NCONF   : pc =  "NCONF"; break;
  case CC_INPROG  : pc =  "INPROG"; break;
  default: break;
  }
  return pc;
}

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

const char* egts_CHS_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case CHS_CP_1251  : pc =  "CP_1251"; break;
  case CHS_ANSI     : pc =  "ANSI"; break;
  case CHS_BIN      : pc =  "BIN"; break;
  case CHS_LATIN_1  : pc =  "LATIN_1"; break;
  case CHS_BIN_1    : pc =  "BIN_1"; break;
  case CHS_JIS      : pc =  "JIS"; break;
  case CHS_CYRILIC  : pc =  "CYRILIC"; break;
  case CHS_HEBREW   : pc =  "HEBREW"; break;
  case CHS_UCS2     : pc =  "UCS2"; break;
  default: break;
  }
  return pc;
}

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

const char* egts_ACT_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case ACT_PARAM  : pc =  "PARAM"; break;
  case ACT_GET    : pc =  "GET"; break;
  case ACT_SET    : pc =  "SET"; break;
  case ACT_ADD    : pc =  "ADD"; break;
  case ACT_DEL    : pc =  "DEL"; break;
  default: break;
  }
  return pc;
}

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

const char* egts_OT_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case OT_FIRMWARE  : pc =  "FIRMWARE"; break;
  case OT_CONFIG    : pc =  "CONFIG"; break;
  default: break;
  }
  return pc;
}

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

const char* egts_MT_str( u8 r )
{
  const char* pc = "?";  
  switch (r)
  {
  case MT_AUX   : pc =  "AUX"; break;
  case MT_MAIN  : pc =  "MAIN"; break;
  default: break;
  }
  return pc;
}

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

const char* egts_CLT_str( u8 r )
{
  const char* pc = "?";    
  switch (r)
  {
  case CLT_EMERG  : pc =  "EMERG"; break;
  case CLT_TEST   : pc =  "TEST"; break;
  default :  break;
  }
  return pc;
}

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


const char* egts_ACType_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case ACT_MANUAL : pc =  "MANUAL";  break;
  case ACT_AUTO   : pc =  "AUTO"; break;
  default : break;
  }
  return pc;
}

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

const char* egts_TEST_MODE_str( u8 r )
{
  const char* pc = "?";
  switch (r)
  {
  case TEST_MODE_SEQ_ALL_TESTS    : pc = "SEQ_ALL_TESTS"; break;
  case TEST_MODE_CALL_CENTER      : pc = "CALL_CENTER"; break;
  case TEST_MODE_EXT_CALL_CENTER  : pc = "EXT_CALL_CENTER"; break;
  case TEST_MODE_MIC              : pc = "MIC"; break;
  case TEST_MODE_AUDIO            : pc = "AUDIO"; break;
  case TEST_MODE_IGNITION         : pc = "IGNITION"; break;
  case TEST_MODE_USER_INTERFACE   : pc = "USER_INTERFACE"; break;
  case TEST_MODE_ACCUM            : pc = "ACCUM"; break;
  case TEST_MODE_SENSOR           : pc = "SENSOR"; break;
  default:  break;
  }
  return pc;
}

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

void  estate_dump_string(
  u8*   p_str , 
  u32   len ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) 
)
{
  u32 i,l;
  u8* puc;

  l=0U;
  i=0U;
  for ( puc = (u8*)p_str; i < len; i++ )
  {
    if ( ( *puc < (u8)' ' ) || ( *puc > (u8)127 ) )
    {
      (*fn_dump)( ctx , "\\x%02X" , *puc++);
      l += 4U;
    }
    else
    {
      (*fn_dump)( ctx , "%c" , *puc++);
      l += 1U;
    }
    if ( l >= 76U )
    {
      (*fn_dump)( ctx , "\n");
      l = 0U;
    }
  }
  (*fn_dump)( ctx , "\n");
}

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

void  estate_dump_string_z(
  u8*   p_str , 
  u32   len ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) )
{
  u32 i,l;
  u8* puc;

  l=0U;
  i=0U;
  for ( puc = (u8*)p_str; (i < len) && (*puc != 0U); i++ )
  {
    if ( (*puc < (u8)' ') || ( *puc > 127U ) )
    {
      (*fn_dump)( ctx , "\\x%02X" , *puc++);
      l += 4U;
    }
    else
    {
      (*fn_dump)( ctx , "%c" , *puc++);
      l += 1U;
    }
    if ( l >= 76U )
    {
      (*fn_dump)( ctx , "\n");
      l = 0U;
    }
  }
  (*fn_dump)( ctx , "\n");
}

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
  void (* afn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  egts_FIRMWARE_DATA_header_t* psrec = (egts_FIRMWARE_DATA_header_t*)pv_subrecord;
  (*afn_dump)( ctx , "SR_DATA_header:\n");
  (*afn_dump)( ctx , "  Object Type                      OT: %u (%s)\n"     , psrec->OT, egts_OT_str(psrec->OT) );
  (*afn_dump)( ctx , "  Module Type                      MT: %u (%s)\n"     , psrec->MT, egts_MT_str(psrec->MT) );
  (*afn_dump)( ctx , "  Component or Module Identifier   CMI: %u (%02Xh)\n" , psrec->CMI, psrec->CMI );
  (*afn_dump)( ctx , "  Version                          VER: %u (%04Xh)\n" , psrec->VER, psrec->VER );
  (*afn_dump)( ctx , "  Whole Object Signature           WOS: %u (%04Xh)\n" , psrec->WOS, psrec->WOS );
  if ( psrec->FN_len != 0U )
  {
    (*afn_dump)( ctx , "  File Name                        FN:\n");
    estate_dump_string( psrec->FN, (u32)psrec->FN_len, ctx, afn_dump );
  }
  else
  {
    (*afn_dump)( ctx , "  File Name                        FN: -- \n");
  }

}

/******************************************************************************
*
*/

static
void  egts_dump_subrecord(
  egts_record_t*      precord ,
  egts_subrecord_t*   psubrecord ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) 
  )
{
  const egts_subrecord_handler_t* chandler = NULL;

  (*fn_dump)( ctx , "SUBRECORD:\n" );
  chandler = egts_find_subrecord_handler( precord->record.RST , psubrecord->subrecord.SRT );
  if ( !chandler )
  {
    (*fn_dump)( ctx , "---unknown subrecord--\n" );
    return;
  }

  if ( psubrecord->SRD ) {
    chandler->fn_dump( ctx , psubrecord->SRD , fn_dump );
  }  else {
    (*fn_dump)( ctx , "---no data--\n" );
  }

}


/*****************************************************************************/
/*                                                                           */
/* egts_dump_record()                                                        */
/*                                                                           */
/* Description: Debug function for dump the record using specified           */
/*   dump callback                                                           */
/*                                                                           */
/* Arguments: precord - subrecord                                            */
/*            ctx - context of callback call                                 */
/*            fn_dump - callback for debug dump                              */
/*                                                                           */
/* Return:    string                                                         */
/*                                                                           */
/* Other:     function used internally for debug                             */
/*                                                                           */
/*****************************************************************************/

static
void  egts_dump_record( 
  egts_record_t*           precord ,
  void* ctx ,
  void (* fn_dump)( void* actx , const char* fmt , ... ) 
  )
{

  egts_subrecord_t*            psubrecord;
  u16                          i;
  u8  s,n,h,d,m;
  u32 y;

  (*fn_dump)( ctx , "RECORD:\n" );
  (*fn_dump)( ctx , "Record Length           RL: %u\n", precord->record.RL );
  (*fn_dump)( ctx , "Record Number           RN: %u\n", precord->record.RN );

  (*fn_dump)( ctx , " * Source Service On Device           SSOD: %u\n", precord->record.SSOD );
  (*fn_dump)( ctx , " * Recipient Service On Device        RSOD: %u\n", precord->record.RSOD );
  (*fn_dump)( ctx , " * Group                               GRP: %u\n", precord->record.GRP );
  (*fn_dump)( ctx , " * Record Processing Priority          RPP: %u\n", precord->record.RPP );
  (*fn_dump)( ctx , " * Time Field Exists                  TMFE: %u\n", precord->record.TMFE );
  (*fn_dump)( ctx , " * Event ID Field  Exists             EVFE: %u\n", precord->record.EVFE );
  (*fn_dump)( ctx , " * Object ID  Field Exists            OBFE: %u\n", precord->record.OBFE );

  if ( precord->record.OBFE != 0U ) {
    (*fn_dump)( ctx , "Object Identifier      OID: %u\n", precord->record.OID );
  } else {
    (*fn_dump)( ctx , "Object Identifier      OID: -\n" );
  }
  if ( precord->record.EVFE != 0U ) {
    (*fn_dump)( ctx , "Event Identifier      EVID: %u\n", precord->record.EVID );
  } else {
    (*fn_dump)( ctx , "Event Identifier      EVID: -\n" );
  }
  if ( precord->record.TMFE != 0U ) {
    egts_get_tm( precord->record.TM , &s, &n, &h, &d, &m, &y );
    (*fn_dump)( ctx , "Time                    TM: %02u.%02u.%04u %02u:%02u:%02u  %08h\n", 
       d, m, y, h, n, s,  
       precord->record.TM );
  } else {
    (*fn_dump)( ctx , "Time                    TM: -\n" );
  }
  (*fn_dump)( ctx , "Source Service Type    SST: %u\n", precord->record.SST );
  (*fn_dump)( ctx , "Recipient Service Type RST: %u\n", precord->record.RST );

  if ( ( precord->nsubrecords != 0U ) && 
       ( precord->psubrecords != NULL ) )
  {
    psubrecord = precord->psubrecords;
    for ( i = 0U; ( psubrecord != NULL ) && ( i < precord->nsubrecords ); i++ )
    {
      egts_dump_subrecord( 
        precord ,
        psubrecord , 
        ctx , fn_dump );
      psubrecord++;
    }
  }

}
  
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
  )
{
  (*fn_dump)( ctx , ">>packet received:\n" );
  (*fn_dump)( ctx , "Protocol Version     PRV: %02Xh \n", pheader->PRV );
  (*fn_dump)( ctx , "Security Key ID     SKID: %02Xh \n", pheader->SKID );
  (*fn_dump)( ctx , "Prefix               PRF: %02Xh \n", pheader->PRF );
  (*fn_dump)( ctx , "Route                RTE: %u \n", pheader->RTE );
  (*fn_dump)( ctx , "Encryption Algorithm ENA: %02Xh \n", pheader->ENA );
  (*fn_dump)( ctx , "Compressed           CMP: %u\n", pheader->CMP );
  (*fn_dump)( ctx , "Priority              PR: %u \n", pheader->PR );
  (*fn_dump)( ctx , "Header Length         HL: %u \n", pheader->HL );
  (*fn_dump)( ctx , "Header Encoding       HE: %02Xh \n", pheader->HE );

  (*fn_dump)( ctx , "Frame Data Length    FDL: %u\n", pheader->FDL );
  (*fn_dump)( ctx , "Packet Identifier    PID: %u\n", pheader->PID );
  (*fn_dump)( ctx , "Packet Type           PT: %04Xh \n", pheader->PT );
  if ( pheader->RTE != 0U )
  {
  (*fn_dump)( ctx , "Peer Address         PRA: %04Xh \n", pheader->PRA );
  (*fn_dump)( ctx , "Recipient Address    RCA: %04Xh \n", pheader->RCA );
  (*fn_dump)( ctx , "Time To Live         TTL: %u \n", pheader->TTL );
  }
  else
  {
  (*fn_dump)( ctx , "Peer Address         PRA: - \n" );
  (*fn_dump)( ctx , "Recipient Address    RCA: - \n" );
  (*fn_dump)( ctx , "Time To Live         TTL: - \n" );
  }
  (*fn_dump)( ctx , "Header Check Sum     HCS: %02Xh \n", pheader->HCS );

  (*fn_dump)( ctx , "-------------------------\n");
  switch ( pheader->PT  )
  {
  case EGTS_PT_RESPONSE:
  (*fn_dump)( ctx , " PT_RESPONSE\n");
    if ( presponce )
    {
  (*fn_dump)( ctx , "Response Packet ID   PID: %u\n" ,  presponce->RPID );
  (*fn_dump)( ctx , "Processing Result     PR: %u (%s)\n" ,  presponce->PR , egts_result_str(presponce->PR) );
    }
    else
    {
  (*fn_dump)( ctx , "Response Packet ID  RPID: error\n" );
  (*fn_dump)( ctx , "Processing Result     PR: error\n" );
    }
    break;
  case EGTS_PT_APPDATA:
  (*fn_dump)( ctx , " PT_APPDATA\n");
    break;
  case EGTS_PT_SIGNED_APPDATA:
  (*fn_dump)( ctx , " PT_SIGNED_APPDATA\n");
  (*fn_dump)( ctx , "Signature Length    SIGL: ...\n" );
  (*fn_dump)( ctx , "Signature Data      SIGD: ...\n" );
  (*fn_dump)( ctx , "Sign-up check: %s\n" , (signed_up!=(u8)0) ? "PASSED" : "FAILED" );
    break;
  default:
  (*fn_dump)( ctx , " PT_? (%u)\n" , pheader->PT );
    break;
  }
  (*fn_dump)( ctx , "-------------------------\n");

  
  if ( ( precords != NULL ) && 
       ( nrecords != 0U ) )
  {
    while ( 0U != nrecords-- ) {
      egts_dump_record( precords++ , ctx , fn_dump );
    }
  }
  else {
    (*fn_dump)( ctx , "Frame Data            FD: error\n");
  }


  (*fn_dump)( ctx , ">>\n\n");

}


/******************************************************************************
*
*/


#endif


