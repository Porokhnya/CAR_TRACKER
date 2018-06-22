/*****************************************************************************/
/*                                                                           */
/* File: sms_pdu_set.c                                                       */
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
/* Description: SMS-SUBMIT PDU parsing routines                              */
/*                                                                           */
/* Additional information: Comments refer to the 3GPP TS 27.005 document     */
/*                                                                           */
/* Functions: sms_pdu_set, sms_pack_pdu_set_smsc,                            */
/* sms_pack_pdu_set_first_octet, sms_pack_pdu_set_tp_mr,                     */
/* sms_pack_pdu_set_tp_da, sms_pack_pdu_set_tp_pid, sms_pack_pdu_set_tp_dcs, */
/* sms_pack_pdu_set_tp_vp, sms_convert_time_relative,                        */
/* sms_convert_time_absolute, sms_convert_time_abssmall,                     */
/* sms_convert_time_seconds, sms_pack_pdu_set_tp_udl,                        */
/* sms_pack_pdu_set_tp_ud.                                                   */
/*                                                                           */
/*****************************************************************************/

#include "../include/sms_types.h"

/*****************************************************************************
*  prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_smsc()                                                   */
/*                                                                           */
/* Description: Pack SMS address to PDU representation                       */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            smsc - struct with data                                        */
/*            smsc_number_in_string - number value in string format          */
/*            smsc_number_in_string_len - number value len                   */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_smsc (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_ADDRESS_T *smsc,               /* struct with data */
  u8 *smsc_number_in_string,         /* number value in string format */
  u8 smsc_number_in_string_len       /* number value len */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_first_octet()                                            */
/*                                                                           */
/* Description: Pack SMS first octet to PDU representation                   */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            fo - struct with data                                          */
/*            vp_type - validation period type                               */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_first_octet (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_SUBMIT_FIRST_OCTET_T *fo,      /* struct with data */
  SMS_TP_VP_T vp_type                /* validation period type */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_mr()                                                  */
/*                                                                           */
/* Description: Pack SMS TP-MR to PDU representation                         */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_mr - tp_mr field                                            */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_mr (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  u8 tp_mr                           /* tp_mr field */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_da()                                                  */
/*                                                                           */
/* Description: Pack SMS TP-DA to PDU representation                         */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_da - struct with data                                       */
/*            tp_da_number_in_string - number value in string format         */
/*            tp_da_number_in_string_len - number value len                  */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_da (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_ADDRESS_T *tp_da,              /* struct with data */
  u8 *tp_da_number_in_string,        /* number value in string format */
  u8 tp_da_number_in_string_len      /* number value len */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_pid()                                                 */
/*                                                                           */
/* Description: Pack SMS PID to PDU representation                           */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_pid - tp_pid value                                          */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_pid (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_PID_T tp_pid                   /* tp_pid value */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_dcs()                                                 */
/*                                                                           */
/* Description: Pack SMS DCS to PDU representation                           */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_dcs - struct with data                                      */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_dcs (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_DATA_CODING_SCHEME_T *tp_dcs   /* struct with data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_vp()                                                  */
/*                                                                           */
/* Description: Pack SMS TP-VP to PDU representation                         */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_vp - struct with data                                       */
/*            pack_method - time pack method                                 */
/*            sec - time value in seconds                                    */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_vp (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_VALIDITY_PERIOD_T *tp_vp,      /* struct with data */
  SMS_TP_VP_T pack_method,           /* time pack method */
  u64  sec                           /* time value in seconds */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_udl()                                                 */
/*                                                                           */
/* Description: Pack SMS UDL to PDU representation                           */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_udl - data value                                            */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_udl (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  u8 tp_udl                          /* data value */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_ud()                                                  */
/*                                                                           */
/* Description: Pack SMS User Data to PDU representation                     */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_ud - user data struct                                       */
/*            tp_udl - size of user data                                     */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_ud (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_USER_DATA_T *tp_ud,            /* user data struct */
  u8 tp_udl                          /* size of user data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_tp_vp()                                                  */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation         */
/*                                                                           */
/* Arguments: sec -  timeout value in seconds                                */
/*            pack_method - select packing method                            */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_tp_vp (
  u64 sec,                          /* timeout value in seconds */
  SMS_TP_VP_T pack_method,          /* select packing method  */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_relative()                                               */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation with    */
/*   relative coding                                                         */
/*                                                                           */
/* Arguments: sec - struct for store packed data                             */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_relative (
  u64 sec,                          /* timeout value in seconds */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_absolute()                                               */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation with    */
/*   absolute coding                                                         */
/*                                                                           */
/* Arguments: sec - struct for store packed data                             */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_absolute (
  u64 sec,                          /* timeout value in seconds */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_abssmall()                                               */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation with    */
/*   absolute small coding                                                   */
/*                                                                           */
/* Arguments: sec - struct for store packed data                             */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_abssmall (
  u64 sec,                          /* timeout value in seconds */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
);

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_seconds()                                                */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation with    */
/*   second count coding                                                     */
/*                                                                           */
/* Arguments: sec - struct for store packed data                             */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_seconds (
  u64 sec,                          /* timeout value in seconds */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
);

/*****************************************************************************
* implementations
*/

/*****************************************************************************/
/*                                                                           */
/* sms_pdu_set()                                                             */
/*                                                                           */
/* Description: Compose PDU representation of SMS                            */
/*                                                                           */
/* Arguments: data - data array for save parsed struct                       */
/*            data_len - predefined memory size of data array                */
/*            sss - struct container with data                               */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

SMS_error sms_pdu_set ( 
  u8 *data,             /* data array for save parsed struct */
  u32 *data_len,        /* predefined memory size of data array */
  SMS_SUBMIT_T *sss     /* struct container with data */
)
{
  u32 curr_pos = (u32)0;   /* current position in data_array */
  SMS_error rval;

  if ( sss->disable_smsc_field != 1U )
  {
    if ( sss->include_smsc == 1U )
    {
      rval = sms_pack_pdu_set_smsc ( data, &curr_pos, *data_len, &sss->smsc,
                   sss->smsc_number_in_string, sss->smsc_number_in_string_len );
      if ( rval != SMS_PSE_OK ) {
        return rval;
      }
    }
    else
    {
      rval = sms_set_symbol ( data, *data_len, &curr_pos, 0x00U );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
    }
  }

  rval = sms_pack_pdu_set_first_octet ( data, &curr_pos, *data_len, &sss->fo,
                                                        sss->used_time_format );
  if ( rval != SMS_PSE_OK ) {
    return rval;
  }

  rval = sms_pack_pdu_set_tp_mr ( data, &curr_pos, *data_len, sss->tp_mr );
  if ( rval != SMS_PSE_OK ) {
    return rval;
  }

  rval = sms_pack_pdu_set_tp_da ( data, &curr_pos, *data_len, &sss->tp_da,
                 sss->tp_da_number_in_string, sss->tp_da_number_in_string_len );
  if ( rval != SMS_PSE_OK ) {
    return rval;
  }

  rval = sms_pack_pdu_set_tp_pid ( data, &curr_pos, *data_len, sss->tp_pid );
  if ( rval != SMS_PSE_OK ) {
    return rval;
  }

  rval = sms_pack_pdu_set_tp_dcs ( data, &curr_pos, *data_len, &sss->tp_dcs );
  if ( rval != SMS_PSE_OK ) {
    return rval;
  }

  if ( sss->used_time_format != NONE ) /* TP-VP field present */
  {
    rval = sms_pack_pdu_set_tp_vp ( data, &curr_pos, *data_len, &sss->tp_vp,
                                  sss->used_time_format, sss->time_in_seconds );
    if ( rval != SMS_PSE_OK ) {
      return rval;
    }
  }

  rval = sms_pack_pdu_set_tp_udl ( data, &curr_pos, *data_len, sss->tp_udl );
  if ( rval != SMS_PSE_OK ) {
    return rval;
  }
  
  rval = sms_pack_pdu_set_tp_ud ( data, &curr_pos, *data_len, &sss->tp_ud,
                                                          sss->tp_udl - (u8)1 );
  if ( rval != SMS_PSE_OK ) {
    return rval;
  }

  /* return filled size */
  *data_len = curr_pos;

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_smsc()                                                   */
/*                                                                           */
/* Description: Pack SMS address to PDU representation                       */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            smsc - struct with data                                        */
/*            smsc_number_in_string - number value in string format          */
/*            smsc_number_in_string_len - number value len                   */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_smsc (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_ADDRESS_T *smsc,               /* struct with data */
  u8 *smsc_number_in_string,         /* number value in string format */
  u8 smsc_number_in_string_len       /* number value len */
)
{
  u8 symbol;
  u8 len_in_bytes;
  u8 i;
  SMS_error rval;

  rval = sms_pack_number ( smsc_number_in_string,
    smsc_number_in_string_len - (u8)1, smsc->address_value,
                                                        &smsc->address_length );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  len_in_bytes = smsc->address_length >> 1U; /* don't forget 'F' odd */

  symbol = len_in_bytes + 1U; /* count from 1 */
  rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  symbol = (u8)( smsc->type_of_address.type_of_number << 4U ) |
    smsc->type_of_address.num_plan_id;
  rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  
  for ( i = 0U; i != len_in_bytes; i++ ) 
  {
    rval = sms_set_symbol ( data, data_len, curr_pos, smsc->address_value[i] );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }
  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_first_octet()                                            */
/*                                                                           */
/* Description: Pack SMS first octet to PDU representation                   */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            fo - struct with data                                          */
/*            vp_type - validation period type                               */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_first_octet (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_SUBMIT_FIRST_OCTET_T *fo,      /* struct with data */
  SMS_TP_VP_T vp_type                /* validation period type */
)
{
  u8 symbol;
  SMS_error rval;
  
  symbol = (u8)fo->tp_rp;
  symbol <<= 1;
  symbol += (u8)fo->tp_udhi;
  symbol <<= 1;
  symbol += (u8)fo->tp_srr;
  symbol <<= 2;
  
  switch ( vp_type )
  {
    case ( RELATIVE ) :
      fo->tp_vpf = 0x02U;
    break;
    case ( ABSOLUTE ) :
      fo->tp_vpf = 0x03U;
    break;
    case ( ABSSMALL ) :
    case ( SECONDS ) :
    case ( ERELATIVE ) :
    case ( SINGLE ) :
      fo->tp_vpf = 0x01U;
    break;
    default :
      fo->tp_vpf = 0x00U;
    break;    
  }

  symbol += (u8)fo->tp_vpf;
  symbol <<= 1;
  symbol += (u8)fo->tp_rd;
  symbol <<= 2;
  symbol += (u8)fo->tp_mti;
  
  rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_mr()                                                  */
/*                                                                           */
/* Description: Pack SMS TP-MR to PDU representation                         */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_mr - tp_mr field                                            */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_mr (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  u8 tp_mr                           /* tp_mr field */
)
{
  SMS_error rval;

  rval = sms_set_symbol ( data, data_len, curr_pos, tp_mr );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_da()                                                  */
/*                                                                           */
/* Description: Pack SMS TP-DA to PDU representation                         */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_da - struct with data                                       */
/*            tp_da_number_in_string - number value in string format         */
/*            tp_da_number_in_string_len - number value len                  */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_da (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_ADDRESS_T *tp_da,              /* struct with data */
  u8 *tp_da_number_in_string,        /* number value in string format */
  u8 tp_da_number_in_string_len      /* number value len */
)
{
  u8 symbol;
  u8 len_in_bytes;
  u8 i;
  SMS_error rval;

  rval = sms_pack_number ( tp_da_number_in_string,
  tp_da_number_in_string_len - 1U, tp_da->address_value,
    &tp_da->address_length );
  if ( rval != SMS_GFE_OK )
  {
    /* HACK: for correct error message */
    if ( rval == SMS_GFE_OUT_OF_BOUNDS ) {
      return SMS_PSE_ADDR_LEN_ERR;
    }
    return rval;
  }

  symbol = tp_da->address_length;
  rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  symbol = (u8)( tp_da->type_of_address.type_of_number << 4U ) |
    tp_da->type_of_address.num_plan_id;
  rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  /* Warn! Unsafe divide used */
  len_in_bytes = tp_da->address_length >> 1U; /* not forget 'F' odd */
  for ( i = 0U; i != len_in_bytes; i++ ) 
  {
    rval = sms_set_symbol ( data, data_len, curr_pos, tp_da->address_value[i] );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }
  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_pid()                                                 */
/*                                                                           */
/* Description: Pack SMS PID to PDU representation                           */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_pid - tp_pid value                                          */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_pid (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_PID_T tp_pid                   /* tp_pid value */
)
{
  SMS_error rval;
  
  rval = sms_set_symbol ( data, data_len, curr_pos, (u8)( 
    ((u32)tp_pid.bit0 << 7U) |
    ((u32)tp_pid.bit1 << 6U) | 
    ((u32)tp_pid.bit2 << 5U) | 
    ((u32)tp_pid.bit3 << 4U) |
    ((u32)tp_pid.bit4 << 3U) | 
    ((u32)tp_pid.bit5 << 2U) | 
    ((u32)tp_pid.bit6 << 1U) | 
    tp_pid.bit7 ) 
    );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_dcs()                                                 */
/*                                                                           */
/* Description: Pack SMS DCS to PDU representation                           */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_dcs - struct with data                                      */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_dcs (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_DATA_CODING_SCHEME_T *tp_dcs   /* struct with data */
)
{
  u8 symbol;
  SMS_error rval;

  symbol = (u8)( ( (u32)tp_dcs->octet_high << 4U ) | (u32)tp_dcs->octet_low );
 
  rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }
  
  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_vp()                                                  */
/*                                                                           */
/* Description: Pack SMS TP-VP to PDU representation                         */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_vp - struct with data                                       */
/*            pack_method - time pack method                                 */
/*            sec - time value in seconds                                    */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_vp (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_VALIDITY_PERIOD_T *tp_vp,      /* struct with data */
  SMS_TP_VP_T pack_method,           /* time pack method */
  u64  sec                           /* time value in seconds */
)
{
  u8 symbol;
  SMS_error rval;

  /* No time interval field for single shot attempt */
  if ( pack_method == SINGLE ) {
    return SMS_PSE_OK;
  }

  rval = sms_convert_time_tp_vp ( sec, pack_method, tp_vp );
  if ( rval != SMS_PSE_OK ) {
    return rval;
  }

  switch ( pack_method ) {

    case RELATIVE :
      rval = sms_set_symbol ( data, data_len, curr_pos, tp_vp->time_format.relative.val );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
    break;

    case ABSOLUTE :
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.absolute.year ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.absolute.month ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.absolute.day ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.absolute.hour ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.absolute.min ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.absolute.sec ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.absolute.timezone ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
    break;

    case ABSSMALL :
      symbol = 0x83U;
      rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.abssmall.hour ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.abssmall.min ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, sms_reverse_symbol ( tp_vp->time_format.abssmall.sec ) );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
    break;

    case SECONDS :
      symbol = 0x82U;
      rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, tp_vp->time_format.seconds.val );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
    break;

    case ERELATIVE :
      symbol = 0x81U;
      rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
      rval = sms_set_symbol ( data, data_len, curr_pos, tp_vp->time_format.relative.val );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
    break;

    case NONE :
      symbol = 0x40U;
      rval = sms_set_symbol ( data, data_len, curr_pos, symbol );
      if ( rval != SMS_GFE_OK ) {
        return rval;
      }
    break;

    default :
      return SMS_PSE_PMETHOD_FAIL;

  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_tp_vp()                                                  */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation         */
/*                                                                           */
/* Arguments: sec -  timeout value in seconds                                */
/*            pack_method - select packing method                            */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_tp_vp (
  u64 sec,                          /* timeout value in seconds */
  SMS_TP_VP_T pack_method,          /* select packing method  */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
)
{
  SMS_error res;
  tp_vp->type = pack_method;

  switch ( pack_method ) {
      case RELATIVE :
        res = sms_convert_time_relative ( sec, tp_vp );
        break;
      case ABSOLUTE :
        res = sms_convert_time_absolute ( sec, tp_vp );
        break;
      case ABSSMALL :
        res = sms_convert_time_abssmall ( sec, tp_vp );
        break;
      case SECONDS :
        res = sms_convert_time_seconds ( sec, tp_vp );
        break;
      case ERELATIVE :
        res = sms_convert_time_relative ( sec, tp_vp );
        break;
      default :
        res = SMS_PSE_PMETHOD_FAIL;
        break;
  }

  return res;
}

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_relative()                                               */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation with    */
/*   relative coding                                                         */
/*                                                                           */
/* Arguments: sec - struct for store packed data                             */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_relative (
  u64 sec,                          /* timeout value in seconds */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
)
{
  u64 tmp;
  
  if ( sec == (u64)0 ) {
    return SMS_PSE_WRONG_TIME;
  }

  if ( sec > (u64)604799 )
  {
    tmp = sec / (u64)604800;
    if ( tmp > (u64)58 ) {
      return SMS_PSE_WRONG_TIME;
    }
    sec = (u64)196 + tmp;
    tp_vp->time_format.relative.val = (u8)sec;
    return SMS_PSE_OK;
  }

  if ( ( sec > (u64)86399 ) && ( sec < (u64)604800 ) )
  {
    tmp = sec / (u64)86400;
    if ( tmp > (u64)28 ) {
      return SMS_PSE_WRONG_TIME;
    }
    sec = (u64)167 + tmp;
    tp_vp->time_format.relative.val = (u8)sec;
    return SMS_PSE_OK;
  }

  if ( ( sec > (u64)43199 ) && ( sec < (u64)86400 ) )
  {
    tmp = sec / (u64)43200;
    if ( tmp > (u64)23 ) {
      return SMS_PSE_WRONG_TIME;
    }
    sec = (u64)143 + tmp;
    tp_vp->time_format.relative.val = (u8)sec;
    return SMS_PSE_OK;
  }

  if ( sec < (u64)43200 )
  {
    tmp = sec / (u64)300;
    if ( tmp > (u64)143 ) {
      return SMS_PSE_WRONG_TIME;
    }
    sec = tmp;
    tp_vp->time_format.relative.val = (u8)sec;
    return SMS_PSE_OK;
  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_absolute()                                               */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation with    */
/*   absolute coding                                                         */
/*                                                                           */
/* Arguments: sec - struct for store packed data                             */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_absolute (
  u64 sec,                          /* timeout value in seconds */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
)
{
  u64 tmp;
  
  if ( sec == (u64)0 ) {
    return SMS_PSE_WRONG_TIME;
  }

  if ( sec > (u64)31535999 )  /* have years */
  {
    tmp = sec % (u64)31536000;
    sec /= (u64)31536000;
    tp_vp->time_format.absolute.year = (u8)sec;
    sec = tmp;
  }
  else
  {
    tp_vp->time_format.absolute.year = (u8)0;
  }

  if ( ( sec > (u64)2419199 ) && ( sec < (u64)31536000 ) ) /* have months */
  {
    tmp = sec % (u64)2419200;
    sec /= (u64)2419200;
    tp_vp->time_format.absolute.month = (u8)sec;
    sec = tmp;
  }
  else
  {
    tp_vp->time_format.absolute.month = (u8)0;
  }

  if ( ( sec > (u64)86399 ) && ( sec < (u64)2419200 ) ) /* have days */
  {
    tmp = sec % (u64)86400;
    sec /= (u64)86400;
    tp_vp->time_format.absolute.day = (u8)sec;
    sec = tmp;
  }
  else
  {
    tp_vp->time_format.absolute.day = (u8)0;
  }

  if ( ( sec > (u64)3599 ) && ( sec < (u64)86400 ) ) /* have hours */
  {
    tmp = sec % (u64)3600;
    sec /= (u64)3600;
    tp_vp->time_format.absolute.hour = (u8)sec;
    sec = tmp;
  }
  else
  {
    tp_vp->time_format.absolute.hour = (u8)0;
  }

  if ( ( sec > (u64)59 ) && ( sec < (u64)3600 ) ) /* have minutes */
  {
    tmp = sec % (u64)60;
    sec /= (u64)60;
    tp_vp->time_format.absolute.min = (u8)sec;
    sec = tmp;
  }
  else
  {
    tp_vp->time_format.absolute.min = (u8)0;
  }

  if ( sec < (u64)60 ) { /* have seconds */
    tp_vp->time_format.absolute.sec = (u8)sec;
  }

  tp_vp->time_format.absolute.timezone = 0x00U;  /* non used */
 
  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_abssmall()                                               */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation with    */
/*   absolute small coding                                                   */
/*                                                                           */
/* Arguments: sec - struct for store packed data                             */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_abssmall (
  u64 sec,                          /* timeout value in seconds */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
)
{
  u64 tmp;
  
  if ( sec == (u64)0 ) {
    return SMS_PSE_WRONG_TIME;
  }

  if ( sec > (u64)3599 ) /* have hours */
  {
    tmp = sec % (u64)3600;
    sec /= (u64)3600;
    tp_vp->time_format.abssmall.hour = (u8)sec;
    sec = tmp;
  }
  else
  {
    tp_vp->time_format.abssmall.hour = 0U;
  }

  if ( ( sec > (u64)59 ) && ( sec < (u64)3600 ) ) /* have minutes */
  {
    tmp = sec % (u64)60;
    sec /= (u64)60;
    tp_vp->time_format.abssmall.min = (u8)sec;
    sec = tmp;
  }
  else
  {
    tp_vp->time_format.abssmall.min = (u8)0;
  }

  if ( sec < (u64)60 ) { /* have seconds */
    tp_vp->time_format.abssmall.sec = (u8)sec;
  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_convert_time_seconds()                                                */
/*                                                                           */
/* Description: Convert Validity Period to PDU packed representation with    */
/*   second count coding                                                     */
/*                                                                           */
/* Arguments: sec - struct for store packed data                             */
/*            tp_vp - struct for store packed data                           */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_convert_time_seconds (
  u64 sec,                          /* timeout value in seconds */
  SMS_VALIDITY_PERIOD_T *tp_vp      /* struct for store packed data */
)
{
  if ( sec > (u64)255 ) {
    return SMS_PSE_WRONG_TIME;
  }

  tp_vp->time_format.seconds.val = (u8)sec;

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_udl()                                                 */
/*                                                                           */
/* Description: Pack SMS UDL to PDU representation                           */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_udl - data value                                            */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_udl (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  u8 tp_udl                          /* data value */
)
{
  SMS_error rval;

  rval = sms_set_symbol ( data, data_len, curr_pos, tp_udl );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  return SMS_PSE_OK;
}

/*****************************************************************************/
/*                                                                           */
/* sms_pack_pdu_set_tp_udl()                                                 */
/*                                                                           */
/* Description: Pack SMS UDL to PDU representation                           */
/*                                                                           */
/* Arguments: data -  data array for packed struct                           */
/*            curr_pos - current position in data stream                     */
/*            data_len - predefined array length                             */
/*            tp_udl - data value                                            */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static SMS_error sms_pack_pdu_set_tp_ud (
  u8 *data,                          /* data array for packed struct */
  u32 *curr_pos,                     /* current position in data stream */
  u32 data_len,                      /* predefined array length */
  SMS_USER_DATA_T *tp_ud,            /* user data struct */
  u8 tp_udl                          /* size of user data */
)
{
  u8 i;
  SMS_error rval;

  rval = sms_set_symbol ( data, data_len, curr_pos, tp_ud->udhl );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  /*
    Predefined value to send 0x00 or 0x08
  */
  rval = sms_set_symbol ( data, data_len, curr_pos, tp_ud->ie_ident );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  rval = sms_set_symbol ( data, data_len, curr_pos, tp_ud->ie_len );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  /*
    Reference value depend on User Data Identification
  */
  if ( tp_ud->ie_ident == 0x08U )
  {
    rval = sms_set_symbol ( data, data_len, curr_pos, 
                                   (u8)( tp_ud->ie_dat.message_ref_num >> 8 ) );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }

    rval = sms_set_symbol ( data, data_len, curr_pos,  (u8)( tp_ud->ie_dat.message_ref_num & 0x00ffU ) );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }
  }
  else
  {
    rval = sms_set_symbol ( data, data_len, curr_pos, 
                                            (u8)tp_ud->ie_dat.message_ref_num );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }
  }

  rval = sms_set_symbol ( data, data_len, curr_pos,
                                                  tp_ud->ie_dat.message_count );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  rval = sms_set_symbol ( data, data_len, curr_pos, tp_ud->ie_dat.message_num );
  if ( rval != SMS_GFE_OK ) {
    return rval;
  }

  /* cut ie header part from data length */
  tp_udl -= tp_ud->udhl;

  for ( i = 0U; i != tp_udl; i++ )
  {
    rval = sms_set_symbol ( data, data_len, curr_pos, tp_ud->sm[i] );
    if ( rval != SMS_GFE_OK ) {
      return rval;
    }
  }

  return SMS_PSE_OK;
}

/*****************************************************************************
*
*/



