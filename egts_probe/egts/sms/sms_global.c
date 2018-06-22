/*****************************************************************************/
/*                                                                           */
/* File: sms_global.c                                                        */
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
/* Description: Additional SMS parsing routines                              */
/*                                                                           */
/* Additional information: Comments refer to the 3GPP TS 27.005 document     */
/*                                                                           */
/* Functions: digit_to_char, sms_get_symbol, sms_reverse_symbol,             */
/*    sms_set_symbol, sms_pack_number, sms_split_number                      */
/*                                                                           */
/*****************************************************************************/


#include "../include/sms_types.h"

/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* digit_to_char()                                                           */
/*                                                                           */
/* Description: Convert hexadecimal value to char                            */
/*                                                                           */
/* Arguments: in_digit - hexadecimal value                                   */
/*            out_char - converted result                                    */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static
SMS_error digit_to_char (
  u8 in_digit,        /* hexadecimal value */
  u8 *out_char        /* converted result */
);

/******************************************************************************
*
*/

/*****************************************************************************/
/*                                                                           */
/* digit_to_char()                                                           */
/*                                                                           */
/* Description: Convert hexadecimal value to char                            */
/*                                                                           */
/* Arguments: in_digit - hexadecimal value                                   */
/*            out_char - converted result                                    */
/*                                                                           */
/* Return:    SMS_error                                                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static
SMS_error digit_to_char (
  u8 in_digit,        /* hexadecimal value */
  u8 *out_char        /* converted result */
)
{
  if ( in_digit < 10U ) { /* hex 0-9 */
    *out_char =  in_digit + 48U;
  } else {
    if ( ( in_digit > 9U ) && ( in_digit < 16U ) ) { /* hex A-F */
        *out_char = in_digit + 55U;
    } else {
        return SMS_GFE_BAD_CHR;
    }
  }
  return SMS_GFE_OK;
}

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

SMS_error sms_get_symbol (
  u8 *in_data,     /* Pointer to array with input data */
  u32 data_len,    /* Input data array length */
  u32 *data_pos,   /* Current position in data array */
  u8 *res          /* Result here */
)
{
  if ( data_len <= *data_pos ) {
    return SMS_GFE_OUT_OF_BOUNDS;
  }
  
  *res = in_data[(*data_pos)++];

  return SMS_GFE_OK;
}

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

u8 sms_reverse_symbol (
  u8 symbol /* exchange MSB and LSB */
)
{
  return ((( symbol & 0xF0U ) >> 4 ) | (( symbol & 0x0FU ) << 4 ));
}

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

SMS_error sms_set_symbol (
  u8 *out_data,     /* Pointer to array with output data */
  u32 data_len,     /* Output data array length */
  u32 *data_pos,    /* Current position in data array */
  u8 symbol         /* New symbol for array with data*/
)
{
  if ( *data_pos == data_len ) {
    return SMS_GFE_OUT_OF_BOUNDS;
  }

  out_data[(*data_pos)++] = symbol;
  
  return SMS_GFE_OK;
}

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

SMS_error sms_pack_number (
  u8 *number,                 /* String with full number: can have plus sign */
  u8 number_len,              /* Number length except trailing zero '\0' */
  u8 *packed_number,          /* Packed number */
  u8 *packed_number_len       /* Packed number length */
)
{
  u8 i;
  u8 tmp;

  if ( number_len > 13U ) {  /* only 12 symbols accepted in number and plus sign */
    return SMS_GFE_OUT_OF_BOUNDS;
  }

  tmp = 0U;
  for ( i = 0U; i != number_len; i++ ) {
    if ( number[i] != '+' ) { /* exclude plus sign */
      if ( ( number[i] > 47U ) && ( number[i] < 58U ) ) { /* accept only digit signs */
        packed_number[tmp++] = number[i] - 48U; /* as digit */
      } else {
        return SMS_GFE_BAD_CHR;
      }
    }
  }
  
  if ( tmp % 2U != 0U ) {  /* add F sign in even case */
    packed_number[tmp++] = 0x0FU;
  }

  /* reversing pairs and pack to single hexadecimal*/
  *packed_number_len = 0U;
  for ( i = 0U ; i != tmp; i += 2U ) {
    packed_number[ (*packed_number_len)++ ] = (u8)( packed_number[i + 1U] << 4U ) | packed_number[i];
  }

  (*packed_number_len) *= 2U;

  return SMS_GFE_OK;
}

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

SMS_error sms_split_number ( 
  u8 symbol,            /* hex digit to split */
  u8 *address_value1,   /* number left part */
  u8 *address_value2    /* number right part */
)
{
  u8 dig;

  /* Fill values as char symbols except 0F */
  dig = symbol & 0x0FU;
  if ( ( dig > 9U ) && ( dig < 16U ) ) {
    return SMS_GFE_BAD_CHR;
  }
  (void)digit_to_char ( dig, address_value1 );  /* no reason to handle errors here */

  dig = (u8)( symbol & 0xF0U ) >> 4U;
  if ( dig != 0x0FU ) {
    if ( ( dig > 9U ) && ( dig < 16U) ) {
      return SMS_GFE_BAD_CHR;
    }
    (void)digit_to_char ( dig, address_value2 ); /* no reason to handle errors here */
  }

  return SMS_GFE_OK;
}

/*****************************************************************************
*
*/


