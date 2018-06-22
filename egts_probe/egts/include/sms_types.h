/*****************************************************************************/
/*                                                                           */
/* File: sms_types.h                                                         */
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
/* Description: Used structures, variables and ext. functions definition     */
/*                                                                           */
/* Additional information: Comments refer to the 3GPP TS 27.005 document     */
/*                                                                           */
/* Functions:                                                                */
/*                                                                           */
/*****************************************************************************/

#ifndef SMS_TYPES_H
#define SMS_TYPES_H

/* Types redefinition */

#ifndef EGTS_BASE_TYPES_DEFINED
#define u8  unsigned char
#define i8  signed char
#define u16 unsigned short
#define i16 signed short
#define u32 unsigned int
#define i32 signed int
#define u64 unsigned long
#define i64 signed long
#endif

#define SMS_error  signed int

/* All possible error states predefinition */

#define SMS_GFE_PREFIX   1   /* Global Function Error */

#define SMS_GFE_OK            (SMS_GFE_PREFIX)    /* No errors */
#define SMS_GFE_NULL_PTR      (SMS_GFE_PREFIX+1)  /* Null pointer for data storage received */
#define SMS_GFE_BAD_CHR       (SMS_GFE_PREFIX+2)  /* Input character does not commit requirements */
#define SMS_GFE_OUT_OF_BOUNDS (SMS_GFE_PREFIX+3)  /* Position in data array out of bounds */

#define SMS_PGE_PREFIX   50  /* PDU Get Error */

#define SMS_PGE_OK            (SMS_PGE_PREFIX)     /* No errors */
#define SMS_PGE_NULL_PTR      (SMS_PGE_PREFIX+1)  /* Null pointer for data storage received */
#define SMS_PGE_ADDR_LEN_ERR  (SMS_PGE_PREFIX+2)  /* Address length too big */
#define SMS_PGE_ADDR_FILL_ERR (SMS_PGE_PREFIX+3)  /* Address value F symbol on wrong place */
#define SMS_PGE_ALLOC_ERR     (SMS_PGE_PREFIX+4)  /* Memory allocation failed */
#define SMS_PGE_UDHL_ERR      (SMS_PGE_PREFIX+5)  /* TP-UD udhl have unexpected value */
#define SMS_PGE_IEI_ERR       (SMS_PGE_PREFIX+6)  /* TP-UD ie_identify have unexpected value */
#define SMS_PGE_IEL_ERR       (SMS_PGE_PREFIX+7)  /* TP-UD ie_length have unexpected value */
#define SMS_PGE_UD_SIZE_ERR   (SMS_PGE_PREFIX+8)  /* User Data size overload max 134 */

#define SMS_PSE_PREFIX    100  /* PDU Set Error */

#define SMS_PSE_OK            (SMS_PSE_PREFIX)     /* No errors*/
#define SMS_PSE_ADDR_LEN_ERR  (SMS_PSE_PREFIX+1)  /* Address length too big */
#define SMS_PSE_PMETHOD_FAIL  (SMS_PSE_PREFIX+2)  /* Wrong time conversion method selected */
#define SMS_PSE_WRONG_TIME    (SMS_PSE_PREFIX+3)  /* Unexpected time value input */
    

/* All used structures */

/*
  Anonymous struct,
  bits combination
  described in 9.2.3.9
*/
typedef struct SMS_PID_S
{
  u32 bit7 : 1;  
  u32 bit6 : 1;
  u32 bit5 : 1;
  u32 bit4 : 1;
  u32 bit3 : 1;
  u32 bit2 : 1;
  u32 bit1 : 1;
  u32 bit0 : 1;
} SMS_PID_T;

/*
  Information field in User Data field
*/
typedef struct SMS_IEDATA_S
{
  u16 message_ref_num;            /* partial message id */
  u8 message_count;               /* all parts of full message count */
  u8 message_num;                 /* current partial message number */
} SMS_IEDATA_T;

/*
  Defined in 9.2.3.24
*/
typedef struct SMS_USER_DATA_S
{
  u8 udhl;                      /* full User Data header length */
  u8 ie_ident;                  /* informational element identifier */
  u8 ie_len;                    /* informational element data length */
  SMS_IEDATA_T ie_dat;          /* informational element data fields */
  u8  sm[ 134 ];                /* short message ( static size ) */
} SMS_USER_DATA_T;

/*
  Different predefined types of validity period
*/
typedef enum SMS_TP_VP_S
{
  UNKNOWN = 0,    /* no type selected, error */
  NONE,           /* no period included */
  RELATIVE,       /* set delivery period in relative format */
  ABSOLUTE,       /* set delivery period in absolute format */
  ABSSMALL,       /* set period as absolute small type notation */
  SECONDS,        /* set period as seconds notation */
  ERELATIVE,      /* set delivery period in relative format but enhanced */
  SINGLE          /* no period, single shot */
} SMS_TP_VP_T;

/*
  Packed value 9.2.3.12.1
*/
typedef struct SMS_RELATIVE_S
{
  u8 val;   /* Time value in special format */
} SMS_RELATIVE_T;

/*
  Described in 9.2.3.11
*/
typedef struct SMS_ABSOLUTE_S 
{
    u8 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 min;
    u8 sec;
    u8 timezone;
} SMS_ABSOLUTE_T;

/*
  -----011b case in Enhanced format
*/
typedef struct SMS_ABSOLUTE_SMALL_S
{
    u8 hour;
    u8 min;
    u8 sec;
} SMS_ABSOLUTE_SMALL_T;

/*
  -----010b case in Enhanced format
*/
typedef struct SMS_SEC_S
{
  u8 val;   /* Time value in seconds */
} SMS_SEC_T;

/*
  Time presentation mixed struct
*/
typedef struct SMS_VALIDITY_PERIOD_S
{
  SMS_TP_VP_T type;
  union {
    SMS_RELATIVE_T        relative;
    SMS_ABSOLUTE_T        absolute;
    SMS_ABSOLUTE_SMALL_T  abssmall;
    SMS_SEC_T             seconds;
  } time_format;
} SMS_VALIDITY_PERIOD_T;

/*
  Defined in 9.2.3.11
  all fields in semi-octets representation
*/
typedef struct SMS_TIME_STAMP_S
{
  u8 year;
  u8 month;
  u8 day;
  u8 hour;
  u8 minute;
  u8 second;
  u8 timezone;
} SMS_TIME_STAMP_T;

/*
  First octet of PDU SMS-DELIVER
  Described in 9.2.3
*/
typedef struct SMS_DELIVER_FIRST_OCTET_S
{
  u32 tp_mti : 2;  /* Message type indicator. Parameter describing the
                  message type.
                  9.2.3.1
                  */
  u32 tp_mms : 1;  /* More messages to send. Parameter indicating
                  whether or not there are more messages to send.
                  9.2.3.2
                  */
  u32 bits34 : 2;  /* Unused bits
                  Include for completeness of octet
                  */
  u32 tp_sri : 1;  /* Status report indication. Parameter indicating
                  if the SME has requested a status report.
                  9.2.3.4
                  */
  u32 tp_udhi : 1; /* User data header indicator. Parameter indicating
                  that the TP-UD field contains a Header.
                  9.2.3.23
                  */
  u32 tp_rp : 1;   /* Reply path. Parameter indicating that
                  Reply Path exists.
                  9.2.3.17
                  */
} SMS_DELIVER_FIRST_OCTET_T;

/*
  Defined in 3GPP TS 23.038 [4]
*/
typedef struct SMS_DATA_CODING_SCHEME_S
{
  u32 octet_low : 4;   /* Bits from 0 to 3 */
  u32 octet_high : 4;  /* Bits from 4 to 7 */
} SMS_DATA_CODING_SCHEME_T;

/*
  Defined in 9.1.2.5
  All fields in semi-octets representation
*/
typedef struct SMS_TYPE_OF_ADDRESS_S
{
  u8 type_of_number;        /* predefined val 'International number' */
  u8 num_plan_id;           /* predefined val 'ISDN/telephone numbering plan' */
} SMS_TYPE_OF_ADDRESS_T;

/*
  Defined in 9.1.2.5
*/
typedef struct SMS_ADDRESS_S
{
  u8 address_length;                      /* address fields count */
  SMS_TYPE_OF_ADDRESS_T type_of_address;  /* struct with address type */
  u8 address_value[ 12 ];                 /* number value in a packed view
                                          ( static size )
                                          */
} SMS_ADDRESS_T;

/*
  first octet of PDU SMS-SUBMIT
  Described in 9.2.3
*/
typedef struct SMS_SUBMIT_FIRST_OCTET_S
{
  u32 tp_mti : 2;  /* Message type indicator. Bits no 1 and 0 are set to 0 and
                      1 respectively to indicate that this PDU is an SMS-SUBMIT
                      9.2.3.1
                  */
  u32 tp_rd : 1;   /* Reject duplicates. Parameter indicating whether or not
                      the SC shall accept an SMS-SUBMIT for an SM still held in
                      the SC which has the same TP-MR and the same TP-DA as a			
                      previously submitted SM from the same OA.
                      9.2.3.25
                  */
  u32 tp_vpf : 2;  /* Validity Period Format.
                      Bit4 and Bit3 specify the TP-VP field.
                      9.2.3.3
                  */
  u32 tp_srr : 1;  /* Status report request. This bit is set to 1 if a status
                      report is requested
                      9.2.3.5
                  */
  u32 tp_udhi : 1; /* User data header indicator. This bit is set to 1 if the
                      User Data field starts with a header
                      9.2.3.23
                  */
  u32 tp_rp : 1;   /* Reply path.
                    Parameter indicating that reply path exists.
                    9.2.3.17
                  */
} SMS_SUBMIT_FIRST_OCTET_T;

/*
  PDU message SMS-DELIVER
  Described in 9.2.2.1
*/
typedef struct SMS_DELIVER_S
{
  SMS_ADDRESS_T smsc;           /* Address of short message service center */

  SMS_DELIVER_FIRST_OCTET_T fo; /* Fist octet with bit fields */
  SMS_ADDRESS_T tp_oa;          /* Originating address. Address of the
                                originating SME.
                                9.2.3.7
                                */
  SMS_PID_T tp_pid;             /* Protocol Identifier. Parameter identifying
                                the above layer protocol, if any.
                                9.2.3.9
                                */
  SMS_DATA_CODING_SCHEME_T tp_dcs;  /* Data coding scheme. Parameter identifying
                                the coding scheme within the TP-User-Data.
                                9.2.3.10
                                */
  SMS_TIME_STAMP_T tp_scts;     /* Service centre time stamp. Parameter
                                identifying time when the SC received
                                the message. 7 octets long!!!
                                9.2.3.11
                                */
  u8 tp_udl;                    /* User data length. Parameter indicating the
                                length of the TP-User-Data field to follow.
                                9.2.3.16
                                */
  SMS_USER_DATA_T tp_ud;        /* User data, as is
                                9.2.3.24
                                */
  /* non SMS-SUBMIT PDU fields */
  u8 disable_smsc_field;        /* exclude smsc part from pdu */
} SMS_DELIVER_T;

/*
  PDU message SMS-SUBMIT
  Defined in 9.2.2.2
*/
typedef struct SMS_SUBMIT_S
{
  SMS_SUBMIT_FIRST_OCTET_T fo;  /* Fist octet with bit fields. */
  u8 tp_mr;                     /* Message reference in case of
                                complicated message. Parameter identifying the
                                SMS-SUBMIT, The "00" value here lets the phone
                                set the message reference number itself.
                                9.2.3.6
                                */
  SMS_ADDRESS_T tp_da;          /* Destination address. Address of the
                                destination SME.
                                9.2.3.8
                                */
  SMS_PID_T tp_pid;             /* Protocol identifier. Parameter identifying
                                the above layer protocol, if any.
                                9.2.3.9
                                */
  SMS_DATA_CODING_SCHEME_T tp_dcs;  /* Data coding scheme. Parameter identifying
                                the coding scheme within the TP-User-Data.
                                9.2.3.10
                                */
  SMS_VALIDITY_PERIOD_T tp_vp;  /* Validity period. Parameter identifying the
                                time from where the message is no longer valid.
                                0, 1 or 7 octets!!!
                                9.2.3.12
                                */
  u8 tp_udl;                    /* User data length. Parameter indicating the
                                length of the TP-User-Data field to follow.
                                9.2.3.16
                                */
  SMS_USER_DATA_T tp_ud;        /* User data. as is
                                9.2.3.24
                                */
  SMS_ADDRESS_T smsc;           /* short message service center */

  /* non SMS-SUBMIT PDU fields */
  u8 *tp_da_number_in_string;     /* string with receiver ME number */
  u8 tp_da_number_in_string_len;  /* length of string with number */
  SMS_TP_VP_T used_time_format;   /* Flag with preferred time format and
                                  validity period
                                  */
  u64 time_in_seconds;            /* timeout interval in seconds */
  u8 *smsc_number_in_string;      /* string with receiver ME number */
  u8 smsc_number_in_string_len;   /* length of string with number */
  u8 include_smsc;                /* 1 if smsc set in PDU else insert 0x00 */
  u8 disable_smsc_field;          /* excluding smsc part from pdu */
} SMS_SUBMIT_T;

/*
  All functions with global call access
*/

/*****************************************************************************/
/*                                                                           */
/* sms_get_symbol()                                                          */
/*                                                                           */
/* Description: Read symbol from buffer                                      */
/*                                                                           */
/* Arguments: in_data - Pointer to array with input data                     */
/*            data_len - Input data array length                             */
/*            data_pos - Current position in data array                      */
/*            res - Result here                                              */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

extern SMS_error sms_get_symbol (
  u8 *in_data,     /* Pointer to array with input data */
  u32 data_len,    /* Input data array length */
  u32 *data_pos,   /* Current position in data array */
  u8 *res          /* Result here */
);

/*****************************************************************************/
/*                                                                           */
/* sms_set_symbol()                                                          */
/*                                                                           */
/* Description: Write symbol to buffer                                       */
/*                                                                           */
/* Arguments: in_data - Pointer to array with output data                    */
/*            out_data - Output data array length                            */
/*            data_pos - Current position in data array                      */
/*            symbol - New symbol for array with data                        */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

extern SMS_error sms_set_symbol (
  u8 *out_data,     /* Pointer to array with output data */
  u32 data_len,     /* Output data array length */
  u32 *data_pos,    /* Current position in data array */
  u8 symbol         /* New symbol for array with data*/
);

/*****************************************************************************/
/*                                                                           */
/* sms_set_symbol()                                                          */
/*                                                                           */
/* Description: Convert symbol                                               */
/*                                                                           */
/* Arguments: symbol - exchange MSB and LSB                                  */
/*                                                                           */
/* Return:    reversed symbol                                                */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

extern u8 sms_reverse_symbol (
  u8 symbol /* exchange MSB and LSB */
);

/*****************************************************************************/
/*                                                                           */
/* sms_pack_number()                                                         */
/*                                                                           */
/* Description: Pack number from string representation to binary             */
/*                                                                           */
/* Arguments: number - String with full number: can have plus sign           */
/*            number_len - Number length except trailing zero '\0'           */
/*            packed_number - Packed number                                  */
/*            packed_number_len - Packed number length                       */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

extern SMS_error sms_pack_number (
  u8 *number,                 /* String with full number: can have plus sign */
  u8 number_len,              /* Number length except trailing zero '\0' */
  u8 *packed_number,          /* Packed number */
  u8 *packed_number_len       /* Packed number length */
);

/*****************************************************************************/
/*                                                                           */
/* sms_split_number()                                                        */
/*                                                                           */
/* Description: Split byte to two hexadecimal digit                          */
/*                                                                           */
/* Arguments: symbol - hex digit to split                                    */
/*            address_value1 - number left part                              */
/*            address_value2 - number right part                             */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

extern SMS_error sms_split_number ( 
  u8 symbol,            /* hex digit to split */
  u8 *address_value1,   /* number left part */
  u8 *address_value2    /* number right part */
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

extern SMS_error sms_pdu_get (
  u8 *data,             /* data array for parse */
  u32 data_len,         /* array length */
  SMS_DELIVER_T *sds    /* struct container for parsed data */
);

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

extern SMS_error sms_pdu_set ( 
  u8 *data,             /* data array for save parsed struct */
  u32 *data_len,        /* predefined memory size of data array */
  SMS_SUBMIT_T *sss     /* struct container with data */
);

/*****************************************************************************
*
*/

#endif /* SMS_TYPES_H */
