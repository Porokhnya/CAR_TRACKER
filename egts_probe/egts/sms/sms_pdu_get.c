/*****************************************************************************/
/*                                                                           */
/* File: sms_pdu_get.c                                                       */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: SMS Service                                               */
/*                                                                           */
/* Status: Version 1.1                                                       */
/*                                                                           */
/* Language: C                                                               */
/*                                                                           */
/* (c) Copyright JSC Navigation-information systems, 2011                    */
/*                                                                           */
/* Address:                                                                  */
/*     24, Mishina Str., bld.1                                               */
/*     Moscow, Russia                                                        */
/*                                                                           */
/* Description: SMS-DELIVER PDU type parsing routines                        */
/*                                                                           */
/* Additional information: Comments refer to the 3GPP TS 27.005 document     */
/*                                                                           */
/* Functions: sms_pdu_get, sms_extract_pdu_get_smsc,                         */
/*  sms_extract_pdu_get_first_octet, sms_extract_pdu_get_tp_oa,              */
/*  sms_extract_pdu_get_tp_pid, sms_extract_pdu_get_tp_dcs,                  */
/*  sms_extract_pdu_get_tp_scts, sms_extract_pdu_get_tp_udl,                 */
/*  sms_extract_pdu_get_tp_ud.                                               */
/*****************************************************************************/

#include "../include/sms_types.h"

/*****************************************************************************
*  prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_smsc()                                                */
/*                                                                           */
/* Description: Read SMSC                                                    */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            smsc - struct to save extracted data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
SMS_error sms_extract_pdu_get_smsc (
  u8 *data,             /* data array for parse */
  u32 data_len,         /* array length */
  u32 *curr_pos,        /* current position in data stream */
  SMS_ADDRESS_T *smsc   /* struct to save extracted data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_first_octet()                                         */
/*                                                                           */
/* Description: Read SMS first octet record                                  */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            fo - struct for save extracted data                            */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
SMS_error sms_extract_pdu_get_first_octet (
  u8 *data,                           /* data array for parse */
  u32 data_len,                       /* array length */
  u32 *curr_pos,                      /* current position in data stream */
  SMS_DELIVER_FIRST_OCTET_T *fo       /* struct for save extracted data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_oa()                                               */
/*                                                                           */
/* Description: Read SMS address record                                      */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_oa - struct for save extracted data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
SMS_error sms_extract_pdu_get_tp_oa (
  u8 *data,             /* data array for parse */
  u32 data_len,         /* array length */
  u32 *curr_pos,        /* current position in data stream */
  SMS_ADDRESS_T *tp_oa  /* struct to save extracted data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_pid()                                              */
/*                                                                           */
/* Description: Read SMS PID                                                 */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_oa - struct for save extracted data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
SMS_error sms_extract_pdu_get_tp_pid (
  u8 *data,             /* data array for parse */
  u32 data_len,         /* array length */
  u32 *curr_pos,        /* current position in data stream */
  SMS_PID_T *tp_pid     /* struct to save extracted data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_dcs()                                              */
/*                                                                           */
/* Description: Read SMS DCS                                                 */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_oa - struct for save extracted data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
SMS_error sms_extract_pdu_get_tp_dcs (
  u8 *data,                         /* data array for parse */
  u32 data_len,                     /* array length */
  u32 *curr_pos,                    /* current position in data stream */
  SMS_DATA_CODING_SCHEME_T *tp_dcs  /* struct to save extracted data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_scts()                                             */
/*                                                                           */
/* Description: Read SMS Time Stamp                                          */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_oa - struct for save extracted data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
SMS_error sms_extract_pdu_get_tp_scts (
  u8 *data,                  /* data array for parse */
  u32 data_len,              /* array length */
  u32 *curr_pos,             /* current position in data stream */
  SMS_TIME_STAMP_T *tp_scts  /* struct to save extracted data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_ud()                                               */
/*                                                                           */
/* Description: Read SMS Time Stamp                                          */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_ud - struct for save extracted data                         */
/*            tp_udl - user data length                                      */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
SMS_error sms_extract_pdu_get_tp_ud (
  u8 *data,                         /* data array for parse */
  u32 data_len,                     /* array length */
  u32 *curr_pos,                    /* current position in data stream */
  SMS_USER_DATA_T *tp_ud,           /* struct to save extracted data */
  u8 tp_udl                         /* User data length */
);

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_udl()                                              */
/*                                                                           */
/* Description: Read SMS UDL                                                 */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_udl - user data length                                      */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
SMS_error sms_extract_pdu_get_tp_udl (
  u8 *data,                         /* data array for parse */
  u32 data_len,                     /* array length */
  u32 *curr_pos,                    /* current position in data stream */
  u8 *tp_udl                        /* user data length */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pdu_get()                                                             */
/*                                                                           */
/* Description: Parse PDU representation to structure                        */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            sds - struct container for parsed data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/


SMS_error sms_pdu_get (
  u8 *data,             /* data array for parse */
  u32 data_len,         /* array length */
  SMS_DELIVER_T *sds    /* struct container for parsed data */
)
{
  u32 curr_pos = 0U;     /* current position in data stream */
  SMS_error rval;

  if ( sds->disable_smsc_field != 1U )
  {
    rval = sms_extract_pdu_get_smsc ( data, data_len, &curr_pos, &sds->smsc );
    if ( rval != SMS_PGE_OK ) {
      return rval;
    }
  }

  rval = sms_extract_pdu_get_first_octet ( data, data_len, &curr_pos, &sds->fo );
  if ( rval != SMS_PGE_OK ) {
    return rval;
  }

  rval = sms_extract_pdu_get_tp_oa ( data, data_len, &curr_pos, &sds->tp_oa );
  if ( rval != SMS_PGE_OK ) {
    return rval;
  }

  rval = sms_extract_pdu_get_tp_pid ( data, data_len, &curr_pos, &sds->tp_pid );
  if ( rval != SMS_PGE_OK ) {
    return rval;
  }

  rval = sms_extract_pdu_get_tp_dcs ( data, data_len, &curr_pos, &sds->tp_dcs );
  if ( rval != SMS_PGE_OK ) {
    return rval;
  }

  rval = sms_extract_pdu_get_tp_scts ( data, data_len,
                                                    &curr_pos, &sds->tp_scts );
  if ( rval != SMS_PGE_OK ) {
    return rval;
  }

  rval = sms_extract_pdu_get_tp_udl ( data, data_len, &curr_pos, &sds->tp_udl );
  if ( rval != SMS_PGE_OK ) {
    return rval;
  }

  rval = sms_extract_pdu_get_tp_ud ( data, data_len, &curr_pos, &sds->tp_ud, sds->tp_udl );
  if ( rval != SMS_PGE_OK ) {
    return rval;
  }

  return SMS_PGE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_smsc()                                                */
/*                                                                           */
/* Description: Read SMSC                                                    */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            smsc - struct to save extracted data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_extract_pdu_get_smsc (
  u8 *data,             /* data array for parse */
  u32 data_len,         /* array length */
  u32 *curr_pos,        /* current position in data stream */
  SMS_ADDRESS_T *smsc   /* struct to save extracted data */
)
{
  SMS_error rval;
  u8 symbol;
  u8 i, j, k;
  
  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  smsc->address_length = symbol;

  if ( smsc->address_length == 0U ) { /* SMSC center set on SIM card */
    return SMS_PGE_OK;
  }

  /*
    address_length containing number of octets in number,
    except fill bits container 'F' dup.
    odd values with 'F' only supported: 1, 3, 5, 7
  */
  if ( smsc->address_length > 7U ) {
    return SMS_PGE_ADDR_LEN_ERR;
  }

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  smsc->type_of_address.num_plan_id = symbol & 0x0FU;
  smsc->type_of_address.type_of_number = (u8)( ( symbol & 0xF0U ) >> 4U );

  /*walk trough octets! not semi-octets*/
  j = 0U;
  k = smsc->address_length - 1U;
  for ( i = 0U ; i != k; i++ )
  {
    rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }

    /* Fill values as char symbols except 'F'.*/
    if ( ( symbol & 0x0FU ) == 0x0FU ) {
      return SMS_PGE_ADDR_FILL_ERR;
    }

    /*Warn! Unsafe memory address call */
    rval = sms_split_number ( symbol, smsc->address_value + j, smsc->address_value + ( j + 1U ) );
    if ( rval != SMS_GFE_OK ) {
      return ( rval );
    }
    j += 2U;
  }

  return SMS_PGE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_first_octet()                                         */
/*                                                                           */
/* Description: Read SMS first octet record                                  */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            fo - struct for save extracted data                            */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_extract_pdu_get_first_octet (
  u8 *data,                           /* data array for parse */
  u32 data_len,                       /* array length */
  u32 *curr_pos,                      /* current position in data stream */
  SMS_DELIVER_FIRST_OCTET_T *fo       /* struct for save extracted data */
)
{
  SMS_error rval;
  u8 symbol;

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  fo->tp_mti  = (u32)( (u32)symbol & 0x03U );
  fo->tp_mms  = (u32)( ( (u32)symbol >> 2U ) & 0x01U );
  fo->bits34  = (u32)( ( (u32)symbol >> 3U ) & 0x03U );
  fo->tp_sri  = (u32)( ( (u32)symbol >> 5U ) & 0x01U );
  fo->tp_udhi = (u32)( ( (u32)symbol >> 6U ) & 0x01U );
  fo->tp_rp   = (u32)( ( (u32)symbol >> 7U ) & 0x01U );

  return SMS_PGE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_oa()                                               */
/*                                                                           */
/* Description: Read SMS address record                                      */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_oa - struct for save extracted data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_extract_pdu_get_tp_oa (
  u8 *data,             /* data array for parse */
  u32 data_len,         /* array length */
  u32 *curr_pos,        /* current position in data stream */
  SMS_ADDRESS_T *tp_oa  /* struct to save extracted data */
)
{
  SMS_error rval;
  u8 symbol;
  u8 bytes_in_num;
  u8 i, j;
  
  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_oa->address_length = symbol;

  /*
    address_length containing number of useful semi-octets in number,
    except fill bits container 'F' dup.
    odd values with 'F' only supported: 0, 2, 4, 6, 8, 10, 12 symbols
    (six bytes max)
  */
  if ( tp_oa->address_length > 12U ) {
    return SMS_PGE_ADDR_LEN_ERR;
  }

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_oa->type_of_address.num_plan_id =  symbol & 0x0FU ;
  tp_oa->type_of_address.type_of_number = ( symbol & 0xF0U ) >> 4U;

  /* if even append to odd, split into octets count*/
  bytes_in_num = ( tp_oa->address_length % 2U == 0U ) ?
    ( tp_oa->address_length >> 1U ) : ( ( tp_oa->address_length + 1U ) >> 1U );

  /*walk trough octets! not semi-octets*/
  j = 0U;
  for ( i = 0U ; i != bytes_in_num; i++ )
  {
    rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }

    if ( ( symbol & 0x0FU ) == 0x0FU ) {
      return SMS_PGE_ADDR_FILL_ERR;
    }
    
    /* Warn! Unsafe memory address call */
    rval = sms_split_number ( symbol, &tp_oa->address_value[j], &tp_oa->address_value[ j + 1U ] );
    if ( rval != SMS_GFE_OK ) {
      return ( rval );
    }
    
    j += 2U;
  }

  return SMS_PGE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_pid()                                              */
/*                                                                           */
/* Description: Read SMS PID                                                 */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_oa - struct for save extracted data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_extract_pdu_get_tp_pid (
  u8 *data,             /* data array for parse */
  u32 data_len,         /* array length */
  u32 *curr_pos,        /* current position in data stream */
  SMS_PID_T *tp_pid     /* struct to save extracted data */
)
{
  SMS_error rval;
  u8 symbol;

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_pid->bit0 = (u32)( (u32)symbol & 0x01U );
  tp_pid->bit1 = (u32)( ((u32)symbol >> 1U) & 0x01U );
  tp_pid->bit2 = (u32)( ((u32)symbol >> 2U) & 0x01U );
  tp_pid->bit3 = (u32)( ((u32)symbol >> 3U) & 0x01U );
  tp_pid->bit4 = (u32)( ((u32)symbol >> 4U) & 0x01U );
  tp_pid->bit5 = (u32)( ((u32)symbol >> 5U) & 0x01U );
  tp_pid->bit6 = (u32)( ((u32)symbol >> 6U) & 0x01U );
  tp_pid->bit7 = (u32)( ((u32)symbol >> 7U) & 0x01U );

  return SMS_PGE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_dcs()                                              */
/*                                                                           */
/* Description: Read SMS DCS                                                 */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_oa - struct for save extracted data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_extract_pdu_get_tp_dcs (
  u8 *data,                         /* data array for parse */
  u32 data_len,                     /* array length */
  u32 *curr_pos,                    /* current position in data stream */
  SMS_DATA_CODING_SCHEME_T *tp_dcs  /* struct to save extracted data */
)
{
  SMS_error rval;
  u8 symbol;

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  

  tp_dcs->octet_high = ( (u32)symbol & 0xF0U ) >> 4U;
  tp_dcs->octet_low = (u32)symbol & 0x0FU;

  return SMS_PGE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_scts()                                             */
/*                                                                           */
/* Description: Read SMS Time Stamp                                          */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_oa - struct for save extracted data                         */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_extract_pdu_get_tp_scts (
  u8 *data,                  /* data array for parse */
  u32 data_len,              /* array length */
  u32 *curr_pos,             /* current position in data stream */
  SMS_TIME_STAMP_T *tp_scts  /* struct to save extracted data */
)
{
  SMS_error rval;
  u8 symbol;

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_scts->year = sms_reverse_symbol ( symbol );

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_scts->month = sms_reverse_symbol ( symbol );

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_scts->day = sms_reverse_symbol ( symbol );

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_scts->hour = sms_reverse_symbol ( symbol );

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_scts->minute = sms_reverse_symbol ( symbol );

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_scts->second = sms_reverse_symbol ( symbol );

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  tp_scts->timezone = sms_reverse_symbol ( symbol );
  
  return SMS_PGE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_udl()                                              */
/*                                                                           */
/* Description: Read SMS UDL                                                 */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_udl - user data length                                      */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_extract_pdu_get_tp_udl (
  u8 *data,                         /* data array for parse */
  u32 data_len,                     /* array length */
  u32 *curr_pos,                    /* current position in data stream */
  u8 *tp_udl                        /* user data length */
)
{
  SMS_error rval;
  u8 symbol;

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  *tp_udl = symbol;

  return SMS_PGE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_extract_pdu_get_tp_ud()                                               */
/*                                                                           */
/* Description: Read SMS Time Stamp                                          */
/*                                                                           */
/* Arguments: data - data array for parse                                    */
/*            data_len - array length                                        */
/*            curr_pos - current position in data stream                     */
/*            tp_ud - struct for save extracted data                         */
/*            tp_udl - user data length                                      */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_extract_pdu_get_tp_ud (
  u8 *data,                         /* data array for parse */
  u32 data_len,                     /* array length */
  u32 *curr_pos,                    /* current position in data stream */
  SMS_USER_DATA_T *tp_ud,           /* struct to save extracted data */
  u8 tp_udl                         /* User data length */
)
{
  SMS_error rval;
  u8 symbol;
  u8 i;

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  tp_ud->udhl = symbol;

  /*
    Predefined size 6 or 5, depend on reference counter resolution
  */
  if ( ( tp_ud->udhl != 0x06U ) && ( tp_ud->udhl != 0x05U ) ) {
    return SMS_PGE_UDHL_ERR;
  }

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  tp_ud->ie_ident = symbol;

  /*
    Predefined: concatenated message with 16bit or 8bit reference number
  */
  if ( ( tp_ud->ie_ident != 0x08U ) && ( tp_ud->ie_ident != 0x00U ) ) {
    return SMS_PGE_IEI_ERR;
  }

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  tp_ud->ie_len = symbol;

  /*
    Predefined size 4 or 3, depend on reference number (16bit or 8bit)
  */
  if ( ( tp_ud->ie_len != 0x04U)  && ( tp_ud->ie_len != 0x03U ) ) {
    return SMS_PGE_IEL_ERR;
  }

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  if ( tp_ud->ie_ident == 0x08U ) {
    tp_ud->ie_dat.message_ref_num = symbol;
    tp_ud->ie_dat.message_ref_num <<= 8U;
    rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }
    tp_ud->ie_dat.message_ref_num += symbol;
  }
  else
  {
    tp_ud->ie_dat.message_ref_num = symbol;
    tp_ud->ie_dat.message_ref_num <<= 8U;
  }

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  tp_ud->ie_dat.message_count = symbol;

  rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  tp_ud->ie_dat.message_num = symbol;

  /* cut IE part */
  tp_udl -= tp_ud->udhl;

  /*
    Maximum size 134
   */
  if ( tp_udl > 134U ) {
    return SMS_PGE_UD_SIZE_ERR;
  }

  for ( i = 0U; i != tp_udl; i++ ) {
    rval = sms_get_symbol ( data, data_len, curr_pos, &symbol );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }
    tp_ud->sm[i] = symbol;
  }

  return SMS_PGE_OK;
}

/*****************************************************************************
*
*/



