/*****************************************************************************/
/*                                                                           */
/* File: egts.h                                                              */
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
/* Description: This header declares general Protocol types and              */
/*   structures. Header is mandatory for any EGTS source code.               */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: -                                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef egts_h
#define egts_h

/******************************************************************************
*
*/

typedef  unsigned char  u8;
typedef  unsigned short u16;
typedef  unsigned long  u32;

typedef  signed char    i8;
typedef  signed short   i16;
typedef  signed long    i32;

/******************************************************************************
*
*/

      
typedef struct egts_route_s
{
  u16   PRA; /* Peer Address , optional , exists when RTE switched on*/
  u16   RCA; /* Recipient Address , optional , exists when RTE switched on */
  u8    TTL; /* Time To Live , optional , exists when RTE switched on */
} egts_route_t;

typedef struct egts_profile_s
{
  u8    SKID;
  u8    ENA;
  u8    CMP;
  u8    HE;
  u8    SGN;  /* 0/1 APPDATA/SIGNED_APPDATA */
} egts_profile_t;

typedef struct egts_header_s
{

  /* Mandatory part ( 5 byte ) */
  u8    PRV; /* Protocol Version */
  u8    SKID; /* Security Key ID */

  /* Bitfield */
  u8 PRF ;/* 2  Prefix */
  u8 RTE ;/* 1  Route */
  u8 ENA ;/* 2  Encryption Algorithm */
  u8 CMP ;/* 1  Compressed */
  u8 PR  ;/* 2   Priority */

    
  u8    HL;  /* Header Length , up to HCS*/
  u8    HE;  /* Header Encoding */

  /* << begin encoded part ( 5 byte + 5(?) byte ) */
  u16   FDL; /* Frame Data Length */
  u16   PID; /* Packet Identifier */
  u8    PT;  /* Packet Type */
  /* <<<< begin optional part ( 5 byte )*/
  u16   PRA; /* Peer Address , optional , exists when RTE switched on*/
  u16   RCA; /* Recipient Address , optional , exists when RTE switched on */
  u8    TTL; /* Time To Live , optional , exists when RTE switched on */
  /* >>>> end optional part */
  /* >> end encoded part */

  u8    HCS; /* Header Check Sum , mandatory , XOR*/

  /* SFRD  Services Frame Data */
  /* SFRCS Services Frame Data Check Sum , CRC16 –CCITT over SFRD*/

} egts_header_t;

#define  EGTS_VERSION   (0x01)

#define  EGTS_PRF       (0x00)


#define  EGTS_HEADER_LEN_FIXED              5U  /* length up to encoding part begin */ 
#define  EGTS_HEADER_LEN_ENCODED            5U  /* FDL, PID, PT */
#define  EGTS_HEADER_LEN_ROUTE              5U  /* PRA, RCA, TTL */
#define  EGTS_HEADER_LEN_ENCODED_MAX_LEN    32U /* TODO: ??? */
#define  EGTS_HEADER_LEN_NO_ROUTE           (EGTS_HEADER_LEN_FIXED+EGTS_HEADER_LEN_ENCODED+1U) /* 12 byte */
#define  EGTS_HEADER_LEN_WITH_ROUTE         (EGTS_HEADER_LEN_NO_ROUTE+EGTS_HEADER_LEN_ROUTE)  /* 17 byte */
#define  EGTS_HEADER_LEN_MAX                (EGTS_HEADER_LEN_FIXED+EGTS_HEADER_LEN_ROUTE+EGTS_HEADER_LEN_ENCODED_MAX_LEN+1U)


/******************************************************************************
*
*/

#define  EGTS_PRIORITY_MAX       3

/******************************************************************************
*
*/

#define  EGTS_HEADER_ENCODING_NONE    (u8)(0)
#ifdef EGTS_DBG
/* till no real encoded defined, debug only */
#define  EGTS_HEADER_ENCODING_DBG     (u8)(1)
#endif


/******************************************************************************
*
*/

#define  EGTS_DATA_ENA_NONE    0U
#ifdef EGTS_DBG
/* till no real encoded defined, debug only */
#define  EGTS_DATA_ENA_DBG     1U
#endif

/******************************************************************************
*
*/

#define  EGTS_PT_RESPONSE         (0)
#define  EGTS_PT_APPDATA          (1)
#define  EGTS_PT_SIGNED_APPDATA   (2)

/******************************************************************************
*
*/

typedef struct
{
  u16    RPID; /* Response Packet ID */
  u8     PR;   /* Processing Result */

} egts_responce_header_t;

/******************************************************************************
*
*/

#define EGTS_MAX_SIGN_LEN       512U

#define EGTS_MAX_SFDR_LEN       65515U

typedef struct
{
  u16   SIGL;                         /* Signature Length */ /* SHORT in doc ... */
  u8    SIGD[ EGTS_MAX_SIGN_LEN ];    /* Signature Data , optioal ??? , */ 

} egts_signature_header_t;

/******************************************************************************
*
*/

typedef struct
{
  u16     RL; /* Record Length */
  u16     RN; /* Record Number */


  /* Bitfields*/
  u8  SSOD ;/* 1  Source Service On Device */
  u8  RSOD ;/* 1  Recipient Service On Device */
  u8  GRP  ;/* 1  Group */
  u8  RPP  ;/* 2  Record Processing Priority */
  u8  TMFE ;/* 1  Time Field Exists */
  u8  EVFE ;/* 1  Event ID Field  Exists */
  u8  OBFE ;/* 1  Object ID  Field Exists */

  u32     OID;  /* Object Identifier, optional , switched on by OBFE */
  u32     EVID; /* Event Identifier, optional , switched on by EVFE */
  u32     TM;   /* Time, optional , seconds from 00:00:00 01.01.2010 UTC , switched on by TMFE */

  u8      SST;  /* Source Service Type */
  u8      RST;  /* Recipient Service Type */

  /* RD Record Data , mandatory ?*/

} egts_service_data_record_t;

/******************************************************************************
*
*/

typedef struct
{

  u8      SRT; /* Subrecord Type */
  u16     SRL; /* Subrecord Length */

  /* SRD Subrecord Data , optional */

} egts_service_data_subrecord_t;

/******************************************************************************
*
*/

#define EGTS_AUTH_SERVICE         (1)
#define EGTS_TELEDATA_SERVICE     (2)
#define EGTS_COMMANDS_SERVICE     (4)
#define EGTS_FIRMWARE_SERVICE     (9)
#define EGTS_ECALL_SERVICE        (10)

/******************************************************************************
*
*/

#define EGTS_SR_RESPONSE   (0)


/* and service . EGTS_SR_RESPONSE */

typedef struct
{
  u16  CRN;  /* Confirmed Record Number */
  u8   RST;  /* Record Status */
} egts_RESPONSE_t;


/******************************************************************************
*   EGTS_AUTH_SERVICE
*     subrecords
*/

#define EGTS_SR_TERM_IDENTITY     (1)
#define EGTS_SR_MODULE_DATA       (2)
#define EGTS_SR_VEHICLE_DATA      (3)
#define EGTS_SR_AUTH_PARAMS       (6)
#define EGTS_SR_AUTH_INFO         (7)
#define EGTS_SR_SERVICE_INFO      (8)
#define EGTS_SR_RESULT_CODE       (9)


/**********************************************
*
*/

#define EGTS_IMEI_LEN     15U
#define EGTS_IMSI_LEN     16U
#define EGTS_LNGC_LEN     3U
#define EGTS_NID_LEN      3U
#define EGTS_MSISDN_LEN   15U

/* EGTS_AUTH_SERVICE . EGTS_SR_TERM_IDENTITY 
    AC -> TP ,
    every session start
*/
typedef struct
{
  u32  TID; /* Terminal Identifier */

  /* Bitfields */
  u8 MNE    ;/* 1 */
  u8 BSE	  ;/* 1 */
  u8 NIDE   ;/* 1 */
  u8 SSRA   ;/* 1 */
  u8 LNGCE  ;/* 1 */
  u8 IMSIE  ;/* 1 */
  u8 IMEIE  ;/* 1 */
  u8 HDIDE  ;/* 1 */

  u16   HDID; /* Home Dispatcher Identifier, optional,  switched on by HDIDE */
  u8    IMEI[ EGTS_IMEI_LEN ]; /* International Mobile Equipment Identity ,optional , switched on by IMEIE */
  u8    IMSI[ EGTS_IMSI_LEN ]; /* International Mobile Subscriber Identity ,optional , switched on by IMSIE */
  u8    LNGC[ EGTS_LNGC_LEN ];  /* Language Code ,optional , switched on by LNGCE */
  u8    NID[ EGTS_NID_LEN ];   /* Network Identifier , optional , switched on by NIDE */
  u16   BS;         /* Buffer Size , optional , switched on by BSE ??? 1024, 2048, 4096 ??? */
  u8    MSISDN[ EGTS_MSISDN_LEN ]; /* Mobile Station Integrated Services Digital Network Number , optional , switched on by MNE */

} egts_AUTH_TERM_IDENTITY_t;

/**********************************************
*
*/

#define EGTS_SRN_LEN      32U
#define EGTS_DSCR_LEN     32U

/* EGTS_AUTH_SERVICE . EGTS_SR_MODULE_DATA 
  AC -> TP , optional ,
  usualy come this sub-records combined into one record
*/

typedef struct
{
  u8    MT;  /* Module Type */
  u32   VID; /* Vendor Identifier */
  u16   FWV; /* Firmware Version */
  u16   SWV; /* Software Version */
  u8    MD; /* Modification */
  u8    ST; /* State , 1=ON,0=OFF, >127=EGTS_PC_XXX */
  u8    SRN[ EGTS_SRN_LEN+1U ]; /* Serial Number , optional */
  u16   SRN_len;
  /* D Delimiter=0	, mandatory */
  u8    DSCR[ EGTS_DSCR_LEN+1U ]; /* Description , optional */
  u16   DSCR_len;
  /* D Delimiter=0	, mandatory */

} egts_AUTH_TERM_MODULE_DATA_t;


/**********************************************
*
*/

#define EGTS_VIN_LEN      17U

/* EGTS_AUTH_SERVICE . EGTS_SR_VEHICLE_DATA ( coupled with EGTS_SR_TERM_IDENTITY )
  AP -> TP
*/

typedef struct
{
  u8    VIN[ EGTS_VIN_LEN ]; /* Vehicle Identification Number */
  u32   VHT;       /* Vehicle Type */
  u32   VPST;      /* Vehicle Propulsion Storage Type */
} egts_AUTH_VEHICLE_DATA_t;

/**********************************************
*
*/

#define EGTS_PBK_LEN   512U
#define EGTS_SS_LEN    255U
#define EGTS_EXP_LEN   255U

/* EGTS_AUTH_SERVICE . EGTS_SR_AUTH_PARAMS 
    AC <- TP
*/
typedef struct
{

  /* Bitfields */
  u8 EXE	  ;/* 1 */
  u8 SSE	  ;/* 1 */
  u8 MSE	  ;/* 1 */
  u8 ISLE	  ;/* 1 */
  u8 PKE	  ;/* 1 */
  u8 ENA    ;/* 1 */

  u16   PKL; /* Public Key Length , optional , switched on by PKE */
  u8    PBK[ EGTS_PBK_LEN ]; /* Public Key , optional , switched on by PKE */
  u16   ISL;  /* Identity String Length , optional , switched on by ISLE*/
  u16   MSZ; /* Mod Size , optional , switched on by MSE*/
  u8    SS[ EGTS_SS_LEN+1U ]; /* Server Sequence, optional , switched on by SSE */
  u16   SS_len;
  /* D Delimiter=0	, optional , depends of SSE */
  u8    EXP[ EGTS_EXP_LEN+1U ];  /* Exp , , optional , switched on by EXE */
  u16   EXP_len;
  /* D Delimiter=0	, optional , depends of EXE */

} egts_AUTH_AUTH_PARAMS_t;

/**********************************************
*
*/

#define  EGTS_UNM_LEN       32U
#define  EGTS_UPSW_LEN      32U

/* EGTS_AUTH_SERVICE . EGTS_SR_AUTH_INFO 
    AC -> TP
*/
typedef struct
{
  u8  UNM[ EGTS_UNM_LEN+1U ]; /* User Name */
  u16 UNM_len;
  /* D Delimiter=0	, mandatory */
  u8  UPSW[ EGTS_UPSW_LEN+1U ]; /* User Password */
  u16 UPSW_len;
  /* D Delimiter=0	, mandatory */
  u8  SS[ EGTS_SS_LEN+1U ]; /* Server Sequence , optional */
  u16 SS_len;
  /* D Delimiter=0	, optional ? */
} egts_AUTH_AUTH_INFO_t;

/**********************************************
*
*/


/* EGTS_AUTH_SERVICE . EGTS_SR_SERVICE_INFO 
    AC <-> TP
*/
typedef struct
{
  u8  ST; /* Service Type , EGTS_XXX_SERVICE*/
  u8  SST;  /* Service Statement , EGTS_SST_XXXX*/

  /* Bitfields */
  u8 SRVA  ;/* 1  Service Attribute 0=supported, 1=requested*/
  /*u8 spare : 5;*/
  u8 SRVRP ;/* 2  Service Routing Priority , 00..11 */


} egts_AUTH_SERVICE_INFO_t;


#define EGTS_SST_IN_SERVICE     (0)
#define EGTS_SST_OUT_OF_SERVICE (128)
#define EGTS_SST_DENIED         (129)
#define EGTS_SST_NO_CONF        (130)
#define EGTS_SST_TEMP_UNAVAIL   (131)

/**********************************************
*
*/

/* EGTS_AUTH_SERVICE . EGTS_SR_RESULT_CODE 
    AC <- TP
*/
typedef struct
{
  u8 RCD; /* ResultCode */
} egts_AUTH_RESULT_CODE_t;

/******************************************************************************
*  EGTS_COMMANDS_SERVICE
*/


#define EGTS_SR_COMMAND_DATA      (51)

/******************************************************************************
*
*/

#define  EGTS_AC_LEN              255 

/* for CT_COM */
typedef struct
{
  u16  ADR;  /* Address */

  /* Bitfields */
  u8  SZ  ; /* 4  Size , treat as 2 pow SZ*/
  u8  ACT ; /* 4  Action */

  u16 CCD; /* Command Code */
  /* DT  Data , optional */
} egts_COMMAND_t;

#define EGTS_COMMAND_MAX_DT      65200

/* for CT_COMCONF */
typedef struct
{
  u16   ADR;  /* Address */
  u16   CCD;  /* Command Code */
  /* DT , Data , optional , binary , 0...65200 */
} egts_CONF_t;

/* EGTS_COMMANDS_SERVICE . EGTS_SR_COMMAND_DATA */
typedef struct
{

  /* Bitfields */
  u8  CT   ;/* 4  Command Type , CT_XXX */
  u8  CCT  ;/* 4   Command Confirmation Type , CC_XXX for CT_COMCONF, CT_MSGCONF, CT_DELIV*/

  u32  CID;  /* Command Identifier */
  u32  SID;  /* Source Identifier */

  /* Bitfields */
  /*u8 spare : 6;*/
  u8 ACFE  ;/* 1   Authorization Code Field Exists */
  u8 CHSFE ;/* 1   Charset Field Exists */

  u8  CHS; /* Charset , optional , switched on by CHSFE , CHS_XXX*/
  u8  ACL; /* Authorization Code Length , optional , switched on by ACFE */
  u8  AC[ EGTS_AC_LEN ]; /* Authorization Code , optional , switched on by ACFE */


  egts_COMMAND_t    command;
  egts_CONF_t       conf;

  /* CD , Command Data, exclude COMMAND/CONF 0...65205 */
  u8*   DT;
  u16   DT_len;

} egts_COMMANDS_COMMAND_DATA_t;

/* */
#define  CT_COMCONF   1U
#define  CT_MSGCONF   2U
#define  CT_MSGFROM   3U
#define  CT_MSGTO     4U
#define  CT_COM       5U
#define  CT_DELCOM    6U
#define  CT_SUBREQ    7U
#define  CT_DELIV     8U

/* */
#define  CC_OK        0U
#define  CC_ERROR     1U
#define  CC_ILL       2U
#define  CC_DEL       3U
#define  CC_NFOUND    4U
#define  CC_NCONF     5U
#define  CC_INPROG    6U

/* */
#define  CHS_CP_1251  0U /* CP-1251 */
#define  CHS_ANSI     1U /* IA5 (CCITT T.50)/ASCII (ANSI X3.4) */
#define  CHS_BIN      2U /* binary */
#define  CHS_LATIN_1  3U /* Latin 1 (ISO-8859-1) */
#define  CHS_BIN_1    4U /* binary , oops */
#define  CHS_JIS      5U /* JIS (X 0208-1990) */
#define  CHS_CYRILIC  6U /* Cyrllic (ISO-8859-5) */
#define  CHS_HEBREW   7U /* Latin/Hebrew (ISO-8859-8) */
#define  CHS_UCS2     8U /* UCS2 (ISO/IEC-10646) */
/*TODO: document*/
#define  CHS_UNKNOWN  255U


/* for CT_COM */
#define ACT_PARAM    0U
#define ACT_GET      1U
#define ACT_SET      2U
#define ACT_ADD      3U
#define ACT_DEL      4U



/* TODO: list */
#define  EGTS_RAW_DATA          0U  /* binary , up ro 65200 */
#define  EGTS_TEST_MODE         1U  /* byte , 1=START,0=STOP */
#define  EGTS_TEST_GET_ERRORS   4U  /* - , responce 16 */
#define  EGTS_TEST_CLEAR_ERRORS 5U  /* = */
#define  EGTS_CONFIG_RESET      6U  /* = */
#define  EGTS_SET_AUTH_CODE     7U  /* binary ??? */
#define  EGTS_RESTART           8U  /* - */


#define  EGTS_SELF_TEST_ RESULT  2U /* string ??? */


/* parameters */
#define EGTS_RADIO_MUTE_DELAY                   0x0201  /* INT , default 500 */
#define EGTS_RADIO_UNMUTE_DELAY                 0x0202  /* INT , default 500 */

#define EGTS_GPRS_APN                           0x0203  /* STRING , BASE  */
#define EGTS_SERVER_ADDRESS                     0x0204	/* STRING	, BASE , default "" */
#define EGTS_SIM_PIN                            0x0205	/* INT	, BASE , default 0 */
#define EGTS_AUTOMATIC_REGISTRATION             0x0207	/* BOOLEAN	, BASE , default 1 */
#define EGTS_SELFTEST_INTERVAL                  0x0208	/* INT	, BASE , default 0 */
#define EGTS_POST_TEST_REGISTRATION_TIME        0x0209	/* INT	, BASE , default 0 */
#define EGTS_TEST_MODE_END_DISTANCE             0x020A	/* INT	, BASE , default 300 */
#define EGTS_GARAGE_MODE_END_DISTANCE           0x020B	/* INT	, BASE , default 300 */
#define EGTS_GARAGE_MODE_PIN 	                  0x020C	/* ENUM {NONE=0, PIN_1 =1,.. PIN_8=8}	0 */
#define EGTS_TEST_MODE_WATCHDOG 	              0x020E	/* INT	, BASE , default 10 */

#define EGTS_USE_GPRS_WHITE_LIST                0x0230	/* BOOLEAN	, BASE , default 1 */
#define EGTS_GPRS_WHITE_LIST                    0x0231	/* ARRAY OF STRING [20]	, BASE , default all "" */

#define EGTS_TEST_REGISTRATION_TIMEOUT          0x0241	/* INT	, BASE , default 5 */
#define EGTS_TEST_REGISTRATION_PERIOD           0x0242	/* INT	, BASE , default 0 */

#define EGTS_GNSS_POWER_OFF_TIME                0x0301	/* INT	, BASE , default 0 */
#define EGTS_GNSS_DATA_RATE                     0x0302	/* INT/ 1, 2,5,10	, BASE , default 1 */
#define EGTS_GNSS_MIN_ELEVATION                 0x0303	/* INT/ 5…15	, BASE , default 5 */

#define EGTS_UNIT_SERIAL_NUMBER                 0x0400	/* STRING	, BASE , Read-only, default "" */
#define EGTS_UNIT_HW_VERSION                    0x0401	/* STRING	, BASE , Read-only, default "" */
#define EGTS_UNIT_SW_VERSION                    0x0402	/* STRING	, BASE , Read-only, default "" */
#define EGTS_UNIT_VENDOR_ID                     0x0403	/* INT	, BASE , Read-only, default 0 */
#define EGTS_UNIT_ID                            0x0404	/* INT	, BASE , default 0 */
#define EGTS_UNIT_IMEI                          0x0405	/* STRING	, BASE , Read-only, default "" */
#define EGTS_UNIT_RS485_BAUD_RATE               0x0406	/* INT	, default 19200 */
#define EGTS_UNIT_RS485_STOP_BITS               0x0407	/* INT	, default 1 */
#define EGTS_UNIT_RS485_PARITY                  0x0408	/* INT/0,1,2	, default 0 */
#define EGTS_UNIT_LANGUAGE_ID                   0x0410	/* INT	, BASE , default 0 */
#define EGTS_UNIT_HOME_DISPATCHER_ID            0x0411	/* INT	, BASE , default 0 */
#define EGTS_SERVICE_AUTH_METHOD                0x0412	/* INT	, default 1 */
#define EGTS_SERVER_CHECK_IN_PERIOD             0x0413	/* INT	, default 30 */
#define EGTS_SERVER_CHECK_IN_ATTEMPTS           0x0414	/* INT	, default 5 */
#define EGTS_SERVER_PACKET_TOUT                 0x0415	/* INT	, default 5 */
#define EGTS_SERVER_PACKET_RETRANSMIT_ATTEMPTS	0x0416	/* INT	, default 3 */
#define EGTS_UNIT_MIC_LEVEL                     0x0417	/* INT/0…10	, default 8 */
#define EGTS_UNIT_SPK_LEVEL                     0x0418	/* INT/0…10	, default 6 */



/******************************************************************************
* EGTS_FIRMWARE_SERVICE
*/


#define   EGTS_SR_SERVICE_PART_DATA   (33)
#define   EGTS_SR_SERVICE_FULL_DATA   (34)

/**********************************************
*
*/


#define EGTS_FN_LEN               64U
#define EGTS_FIRMWARE_MAX_DATA    65400U

typedef struct
{
  /* Birfields */
  /*u8 spare : 4;*/
  u8 OT ;/* 2  Object Type */
  u8 MT ;/* 2  Module Type */

  u8   CMI; /* Component or Module Identifier */
  u16  VER; /* Version */
  u16  WOS; /* Whole Object Signature , ÑRC16-CCITT of whole data */
  u8   FN[ EGTS_FN_LEN+1U ]; /* File Name , optional */
  u16  FN_len;
  /* D , Delimiter)	, mandatory */
} egts_FIRMWARE_DATA_header_t;


/* EGTS_FIRMWARE_SERVICE . EGTS_SR_SERVICE_PART_DATA */
typedef struct
{
  u16   ID; /* Identity */
  u16   PN; /* Part Number */
  u16   EPQ; /* Expected Parts Quantity */
  egts_FIRMWARE_DATA_header_t   header;/* ODH  , Object Data Header , BINARY	0…71 , for PN==1 only */
  
  /* OD  , Object Data, mandatory , BINARY	1 ... 65400 */
  u8*   OD;
  u16   OD_len;

} egts_FIRMWARE_PART_DATA_t;


#define OT_FIRMWARE   (0)
#define OT_CONFIG     (1)

#define MT_AUX        (0)
#define MT_MAIN       (1)


/**********************************************
*
*/

/* EGTS_FIRMWARE_SERVICE . EGTS_SR_SERVICE_FULL_DATA */
typedef struct
{
  egts_FIRMWARE_DATA_header_t  header;
  /* OD , Object Data */
  u8*   OD;
  u16   OD_len;
} egts_FIRMWARE_FULL_DATA_t;


/******************************************************************************
* EGTS_ECALL_SERVICE
*/

#define     EGTS_SR_ACCEL_DATA        20U
#define     EGTS_SR_RAW_MSD_DATA      40U
#define     EGTS_SR_MSD_DATA          50U
#define     EGTS_SR_TRACK_DATA        62U

/**********************************************
*
*/

#define EGTS_ADS_MAX_COUNT   255

typedef struct
{
  u16  RTM;  /* Relative Time */
  i16  XAAV; /* X Axis Acceleration Value , [31]-sign , in 0.1m/c2 */
  i16  YAAV; /* Y Axis Acceleration Value , [31]-sign , in 0.1m/c2 */
  i16  ZAAV; /* Z Axis Acceleration Value , [31]-sign , in 0.1m/c2 */
} egts_ACCEL_t;


/* EGTS_ECALL_SERVICE . EGTS_SR_ACCEL_DATA */
typedef struct
{
  u8  SA;       /* Structures Amount */
  u32 ATM;      /* AbsoluteTime , seconds since 00:00:00 01.01.2010 UTC epoche */
  egts_ACCEL_t  ADS[ EGTS_ADS_MAX_COUNT ];  /* Accelerometer Data Structure */
} egts_ECALL_ACCEL_DATA_t;

#define EGTS_COO_INVALID      0x7FFFFFFFU
#define EGTS_COO16_INVALID    0x7FFFU


/**********************************************
*
*/

#define EGTS_RAW_MDS_LEN    1024U

/* EGTS_ECALL_SERVICE . EGTS_SR_RAW_MSD_DATA */
typedef struct
{
  u8  FM;        /* Format */
  u16 MSD_len;
  u8  MSD[ EGTS_RAW_MDS_LEN ];
} egts_ECALL_RAW_MSD_DATA_t;

#define EGTS_MDS_FM_UNKNOWN     0U
#define EGTS_MDS_FM_EN_15722    1U

/**********************************************
*
*/

#define EGTS_MDS_AD_LEN       56

/* EGTS_ECALL_SERVICE . EGTS_SR_MSD_DATA */
typedef struct
{
  u8  FV; /* Format Version */
  u8  MI; /* Message Identifier , 1++ */

  /* Bitfields */
  /*u8 spare : 1;*/
  u8    VT ;/* 4  Vehicle Type */
  u8  POCN ;/* 1  Position Confidence */
  u8   CLT ;/* 1  Call Type */
  u8   ACT ;/* 1  Activation Type */

  u8    VIN[ EGTS_VIN_LEN ]; /* Vehicle Identification Number */
  u8    VPST;      /* Vehicle Propulsion Storage Type */
  u32   TS;        /* Time Stamp , seconds from 00:00:00 01.01.1970 BIG-ENDIAN */
  u32   PLAT;      /* Position Latitude */
  u32   PLON;      /* Position Longitude */
  u8    VD;        /* Vehicle Direction */

  
  u8   pvr_latd1;
  u8   pvr_lond1;
  u8   pvr_latd2;
  u8   pvr_lond2;
  u8   nop;
  u8   ad;
  /*u8   spare1 : 2;*/

  u16   RVP_LATD_1; /* Optional, switched on by size, Recent Vehicle Position n-1 Latitude Delta , optional , -512...512, BIG-ENDIAN */
  u16   RVP_LOND_1; /* Optional, switched on by size, Recent Vehicle Position n-1 Longitude Delta , optional , -512...512, BIG-ENDIAN */
  u16   RVP_LATD_2; /* Optional, switched on by size, Recent Vehicle Position n-2 Latitude Delta , optional , -512...512, BIG-ENDIAN */
  u16   RVP_LOND_2; /* Optional, switched on by size, Recent Vehicle Position n-2 Longitude Delta , optional , -512...512, BIG-ENDIAN */
  u8    NOP;      /* Optional, switched on by size, Number Of Passengers , optional  */
  u8    AD[ EGTS_MDS_AD_LEN ];   /* Optional, switched on by size, Additional Data */
  u16   AD_len; 

} egts_ECALL_MSD_DATA_t;


#define RVP_SPARE      0xFFFFU
#define NOP_SPARE      0xFF


#define CLT_EMERG      0
#define CLT_TEST       1


#define ACT_MANUAL     0
#define ACT_AUTO       1

/**********************************************
*
*/

typedef struct
{
  u8    TNDE; /*:1 Track Node Data Exist */
  u8    LOHS; /*:1 */
  u8    LAHS; /*:1 */
  u8    RTM;  /*:5 Relative Time, in 0.1 sec*/

  u32   LAT;  /* Latitude , degree,  (WGS - 84) / 90 * 0xFFFFFFFF */
  u32   LONG; /* Longitude , degree,  (WGS - 84) / 180 * 0xFFFFFFFF */
  u16   SPD;  /* Speed, in 0.01 km/h */
  u16   DIR;  /* Direction , in degree */

} egts_TRACK_t;

/* EGTS_ECALL_SERVICE . EGTS_SR_MSD_DATA */
typedef struct
{
  u8            SA;       /* Structures Amount */
  u32           ATM;      /* Absolute Time , seconds since 00:00:00 01.01.2010 UTC */
  egts_TRACK_t  TDS[255]; /* Track Data Structure */
} egts_ECALL_TRACK_DATA_t;

/******************************************************************************
* commands for ECALL
*/

#define EGTS_ECALL_REQ	              0x0112

/* command EGTS_ECALL_MSD_REQ */ 
typedef struct 
{
  u8   ACT;         /* ACT_XXX */
} EGTS_ECALL_REQ_t;

/******************************************************************************
*
*/

#define EGTS_ECALL_MSD_REQ	          0x0113

/* command EGTS_ECALL_MSD_REQ */ 
typedef struct 
{
  u32  MID;
  u8   TRANSPORT;       /* EGTS_TRANSPORT_XXX */
} EGTS_ECALL_MSD_REQ_t;

#define EGTS_TRANSPORT_ANY        0U
#define EGTS_TRANSPORT_VOICE      1U
#define EGTS_TRANSPORT_SMS        2U
#define EGTS_TRANSPORT_EGTS       3U

/******************************************************************************
* EGTS_TELEDATA_SERVICE
*/

#define     EGTS_SR_RECORD_RESPONSE        (0)
#define     EGTS_SR_POS_DATA               (16)
#define     EGTS_SR_EXT_POS_DATA           (17)
#define     EGTS_SR_AD_SENSORS_DATA        (18)
#define     EGTS_SR_COUNTERS_DATA          (19)
/* the same as in ECALL service
#define     EGTS_SR_ACCEL_DATA             (20) */
#define     EGTS_SR_STATE_DATA             (21)
#define     EGTS_SR_LOOPIN_DATA            (22)
#define     EGTS_SR_ABS_DIG_SENS_DATA      (23)
#define     EGTS_SR_ABS_AN_SENS_DATA       (24)
#define     EGTS_SR_ABS_CNTR_DATA          (25)
#define     EGTS_SR_ABS_LOOPIN_DATA        (26)

/******************************************************************************
*
*/

/* EGTS_TELEDATA_SERVICE . EGTS_SR_POS_DATA */
typedef struct
{
  u32   NTM;  /* Navigation Time , seconds since 00:00:00 01.01.2010 UTC */
  u32   LAT;  /* Latitude , degree,  (WGS - 84) / 90 * 0xFFFFFFFF */
  u32   LONG; /* Longitude , degree,  (WGS - 84) / 180 * 0xFFFFFFFF */
  
  /*FLG Flags*/ 
  u8    ALTE;  /*:1 */
  u8    LOHS;  /*:1 */
  u8    LAHS;  /*:1 */
  u8    MV;    /*:1 */
  u8    BB;    /*:1 */
  u8    FIX;   /*:2 */
  u8    VLD;   /*:1 */

  u16   SPD;     /* Speed , 0.1 miles , 14 bit only*/
  u8    DIRH;    /*:1 */
  u8    ALTS;    /*:1 */

  u8    DIR;     /* Direction */
  u32   ODM;     /* Odometer, 3b */
  u8    DIN;     /* Digital Inputs */
  u8    SRC;     /* Source */
  u32   ALT;     /* Altitude, 3b */

  u8    srcd;    /* SRCD exists */
  u16   SRCD;    /* Source Data */

} egts_TELEDATA_POS_DATA_t;


#define   EGTS_FIX_UNKNOWN   0
#define   EGTS_FIX_2D        1
#define   EGTS_FIX_3D        2


#define   EGTS_SRC_TIMER_IGNITION        0
#define   EGTS_SRC_DISTANCE              1
#define   EGTS_SRC_ANGLE                 2
#define   EGTS_SRC_RESPONSE              3
#define   EGTS_SRC_INPUT                 4
#define   EGTS_SRC_TIMER_NO_IGNITION     5
#define   EGTS_SRC_PERIPHERIAL_OFF       6
#define   EGTS_SRC_SPEED_TRESHOLD        7
#define   EGTS_SRC_CPU_RESTART           8
#define   EGTS_SRC_OUTPUT_OVERLOAD       9
#define   EGTS_SRC_TAMPER                10
#define   EGTS_SRC_BATTERY_POWER         11
#define   EGTS_SRC_LOW_BATTERY           12
#define   EGTS_SRC_ALARM                 13
#define   EGTS_SRC_CALL_REQUEST          14
#define   EGTS_SRC_EMERGENCY             15
#define   EGTS_SRC_EXT_SERVICE           16

#define   EGTS_SRC_BATTERY_FAULT         19
#define   EGTS_SRC_EXTRA_ACC             20
#define   EGTS_SRC_EXTRA_BREAK           21
#define   EGTS_SRC_ERROR_NAV             22
#define   EGTS_SRC_ERROR_SENSOR          23
#define   EGTS_SRC_ERROR_GSM_AERIAL      24
#define   EGTS_SRC_ERROR_NAV_AERIAL      25
#define   EGTS_SRC_LOW_SPEED             27
#define   EGTS_SRC_NO_IGNITION_MOVE      28
#define   EGTS_SRC_TIMER_EMERGENCY       29
#define   EGTS_SRC_NAV_ON_OFF            30
#define   EGTS_SRC_UNSTABLE_NAV          31
#define   EGTS_SRC_IP_CONNECT            32
#define   EGTS_SRC_UNSTABLE_GSM          33
#define   EGTS_SRC_UNSTABLE_IP           34
#define   EGTS_SRC_MODE                  35


/* EGTS_TELEDATA_SERVICE . EGTS_SR_EXT_POS_DATA */
typedef struct
{
  /*spare :3 */
  u8    NSFE;  /*:1 Navigation System Field Exists */
  u8    SFE;   /*:1 Satellites Field Exists */
  u8    PFE;   /*:1 PDOP Field Exists */
  u8    HFE;   /*:1 HDOP Field Exists */
  u8    VFE;   /*:1 VDOP Field Exists */

  u16   VDOP; /* Vertical Dilution of Precision */
  u16   HDOP; /* Horizontal Dilution of Precision */
  u16   PDOP; /* Position Dilution of Precision */
  u8    SAT;  /* Satellites */
  u16   NS;   /* Navigation System */

} egts_TELEDATA_EXT_POS_DATA_t;

#define EGTS_NS_UNKNOWN    0
#define EGTS_NS_GLONASS    1
#define EGTS_NS_GPS        (1<<1)
#define EGTS_NS_GALILEO    (1<<2)
#define EGTS_NS_COMPASS    (1<<3)
#define EGTS_NS_BEIDOU     (1<<4)
#define EGTS_NS_DORIS      (1<<5)
#define EGTS_NS_IRNSS      (1<<6)
#define EGTS_NS_QZSS       (1<<7)


/* EGTS_TELEDATA_SERVICE . EGTS_SR_AD_SENSORS_DATA */
typedef struct
{
  /* DIOE8 DIOE7 DIOE6 DIOE5 DIOE4 DIOE3 DIOE2 DIOE1 */
  u8   DIOE;
  u8   DOUT;  /* Digital Outputs */
  /* ASFE8 ASFE7 ASFE6 ASFE5 ASFE4 ASFE3 ASFE2 ASFE1 */
  u8   ASFE;
  u8   ADIO[8];
  u32  ANS[8];    /* BINARY 3 */
} egts_TELEDATA_AD_SENSORS_DATA_t;


/* EGTS_TELEDATA_SERVICE . EGTS_SR_COUNTERS_DATA */
typedef struct
{
  /* CFE8 CFE7 CFE6 CFE5 CFE4 CFE3 CFE2 CFE1 */
  u8   CFE;      /* Counter Field Exists */
  u32  CN[8];    /* BINARY 3 */
} egts_TELEDATA_COUNTERS_DATA_t;


/* EGTS_TELEDATA_SERVICE . EGTS_SR_ACCEL_DATA 
the same as in ECALL service
typedef struct
{
  u8   SA;      
  u32  ATM;     
  egts_ACCEL_t  ADS[256];
} egts_TELEDATA_ACCEL_DATA_t;
*/
typedef egts_ECALL_ACCEL_DATA_t egts_TELEDATA_ACCEL_DATA_t;


/* EGTS_TELEDATA_SERVICE . EGTS_SR_ EGTS_SR_STATE_DATA */
typedef struct
{
  u8   ST;     /* State */
  u8   MPSV;   /* Main Power Source Voltage, in 0.1V */
  u8   BBV;    /* Back Up Battery Voltage, in 0.1V */
  u8   IBV;    /* Internal Battery Voltage, in 0.1V */

  /*spare :5 */
  u8   NMS;  /*:1 Navigation Module State */
  u8   IBU;  /*:1 Internal Battery Used */
  u8   BBU;  /*:1 Back Up Battery Used */

} egts_TELEDATA_STATE_DATA_t;


#define EGTS_MODE_IDLE            0U
#define EGTS_MODE_ERA             1U
#define EGTS_MODE_ACIVE           2U
#define EGTS_MODE_EMERGENCY_CALL  3U
#define EGTS_MODE_EMERGENCY_MON   4U
#define EGTS_MODE_TEST            5U
#define EGTS_MODE_SERVICE         6U
#define EGTS_MODE_FIRMWARE_LOAD   7U


/* EGTS_TELEDATA_SERVICE . EGTS_SR_LOOPIN_DATA */
typedef struct
{
  /* LIFE8 LIFE7 LIFE6 LIFE5 LIFE4 LIFE3 LIFE2 LIFE1 */
  u8  LIFE;   /* Loop In Field Exists */
  u8  LIS[8]; /* Loop In State */
} egts_TELEDATA_LOOPIN_DATA_t;

#define EGTS_LOOPIN_NORMAL          0U
#define EGTS_LOOPIN_ALARM           1U
#define EGTS_LOOPIN_CUT             2U
#define EGTS_LOOPIN_SHORT_CUT_GND   3U
#define EGTS_LOOPIN_SHORT_CUT_POW   4U


/* EGTS_TELEDATA_SERVICE . EGTS_SR_ABS_DIG_SENS_DATA */
typedef struct
{
  u16  DSN;   /* Digital Sensor Number */
  u8   DSST;  /* Digital Sensor State */
} egts_TELEDATA_ABS_DIG_SENS_DATA_t;


/* EGTS_TELEDATA_SERVICE . EGTS_SR_ABS_AN_SENS_DATA */
typedef struct
{
  u8   ASN;   /* Analog Sensor Number */
  u32  ASV;   /* Analog Sensor Value, 3b */
} egts_TELEDATA_ABS_AN_SENS_DATA_t;


/* EGTS_TELEDATA_SERVICE . EGTS_SR_ABS_CNTR_DATA */
typedef struct
{
  u8   CN;    /* Counter Number */
  u32  CNV;   /* Counter Value, 3b */
} egts_TELEDATA_ABS_CNTR_DATA_t;

/* EGTS_TELEDATA_SERVICE . EGTS_SR_ABS_LOOPIN_DATA */
typedef struct
{
  u16  LIN;   /* Loop In Number */
  u8   LIS;   /* Loop In State */
} egts_TELEDATA_ABS_LOOPIN_DATA_t;


/* EGTS_TELEDATA_SERVICE commands */

#define EGTS_FLEET_DOUT_ON            0x0009U

#define EGTS_FLEET_DOUT_OFF           0x000AU

#define EGTS_FLEET_GET_DOUT_DATA      0x000BU

#define EGTS_FLEET_GET_POS_DATA       0x000CU
/* response - EGTS_SR_POS_DATA */

#define EGTS_FLEET_GET_SENSORS_DATA   0x000DU
/* response - EGTS_SR_POS_DATA, EGTS_SR_AD_SENSORS */

#define EGTS_FLEET_GET_LIN_DATA       0x000EU
/* response - EGTS_SR_POS_DATA, EGTS_SR_LOOPIN_DATA */

#define EGTS_FLEET_GET_CIN_DATA       0x000FU
/* response - EGTS_SR_POS_DATA, EGTS_SR_COUNTERS_DATA */

#define EGTS_FLEET_GET_STATE          0x0010U
/* response - EGTS_SR_POS_DATA, EGTS_SR_STATE_DATA */

#define EGTS_FLEET_ODOM_CLEAR         0x0011U


/* EGTS_TELEDATA_SERVICE parameters */

#define EGTS_FLEET_ON                     0x0261U
#define EGTS_FLEET_IGN_ON_PERIOD          0x0262U
#define EGTS_FLEET_IGN_OFF_PERIOD         0x0263U
#define EGTS_FLEET_DIST_THRESHOLD         0x0264U
#define EGTS_FLEET_COURSE_THRESHOLD       0x0265U
#define EGTS_FLEET_MAX_SPEED_THRESHOLD    0x0266U
#define EGTS_FLEET_MIN_SPEED_THRESHOLDS   0x0267U
#define EGTS_FLEET_MIN_BATTERY_VOLTAGE    0x0268U
#define EGTS_FLEET_POS_ACCEL_THRESHOLD    0x0269U
#define EGTS_FLEET_NEG_ACCEL_THRESHOLD    0x026AU
#define EGTS_FLEET_EM_MON_PERIOD          0x026BU
#define EGTS_FLEET_NAVI_TRB_THRESHOLD     0x026CU
#define EGTS_FLEET_CONN_TRB_THRESHOLD     0x026DU
#define EGTS_FLEET_GSM_REG_TRB_THRESHOLD  0x026EU
#define EGTS_FLEET_POS_USE_ALT            0x026FU
#define EGTS_FLEET_EXT_POS_DATA_FLAGS     0x0270U
#define EGTS_FLEET_SR_MASK                0x0271U
#define EGTS_FLEET_DIN_MASK               0x0272U
#define EGTS_FLEET_AIN_MASK               0x0273U
#define EGTS_FLEET_CIN_MASK               0x0274U
#define EGTS_FLEET_LIN_MASK               0x0275U
#define EGTS_FLEET_USE_ABS_SENS_DATA      0x0276U



/******************************************************************************
*
*/

#define EGTS_TEST_MODE_START_TEST	    0x0003	/* u8 TEST_MODE_XXX */

/* command EGTS_TEST_MODE_START_TEST */ 
typedef struct 
{
  u8   TEST;       /* TEST_MODE_XXX */
} EGTS_TEST_MODE_START_TEST_t;

#define TEST_MODE_SEQ_ALL_TESTS       0
#define TEST_MODE_CALL_CENTER         1
#define TEST_MODE_EXT_CALL_CENTER     2
#define TEST_MODE_MIC                 3
#define TEST_MODE_AUDIO               4
#define TEST_MODE_IGNITION            5
#define TEST_MODE_USER_INTERFACE      6
#define TEST_MODE_ACCUM               7
#define TEST_MODE_SENSOR              8


/******************************************************************************
*
*/

#define EGTS_TEST_MODE_START_TEST_RESPONCE      0x0003   /* u8[8] */


/******************************************************************************
*
*/

/* ECALL parameters */

#define EGTS_ECALL_BLACK_LIST                   0x0206	/* ARRAY OF STRING[20], default all "" */
#define EGTS_ECALL_TEST_NUMBER                  0x020D	/* STRING	"" */

#define EGTS_ECALL_ON                           0x0210	/* BOOLEAN	1 */
#define EGTS_ECALL_SIGNAL_INTERNAL              0x0211	/* BOOLEAN	1 */
#define EGTS_ECALL_SIGNAL_EXTERNAL              0x0212	/* BOOLEAN	1 */
#define EGTS_ECALL_SOS_BUTTON_TIME              0x0213	/* INT	500 */
#define EGTS_ECALL_CRASH_THRESHOLD              0x0214	/* BINARY (X,Y,Z,TIME)	 (SHORT,SHORT , SHORT , SHORT ) */
#define EGTS_ECALL_CRASH_THRESHOLD_NO_IGN	      0x0215	/* BINARY (X,Y,Z,TIME)	( SHORT , SHORT , SHORT , SHORT ) */
#define EGTS_ECALL_MODE_PIN 	                  0x0216	/* ENUM {NONE=0, PIN_1 =1, ... PIN_8=8}	0 */
#define EGTS_ECALL_CCFT                         0x0217	/* INT	60 */
#define EGTS_ECALL_INVITATION_SIGNAL_DURATION   0x0218	/* INT	2 */
#define EGTS_ECALL_SEND_MSG_PERIOD              0x0219	/* INT	2 */
#define EGTS_ECALL_AL_ACK_PERIOD                0x021A	/* INT	2 */
#define EGTS_ECALL_MSD_MAX_TRANSMISSION_TIME    0x021B	/* INT	20 */
#define EGTS_ECALL_NAD_MIN_REGISTRATION_PERIOD  0x021C	/* INT	 */
#define EGTS_ECALL_NAD_DEREGISTRATION_TIMER     0x021D	/* INT	12 */
#define EGTS_ECALL_DIAL_DURATION                0x021E	/* INT	0 */
#define EGTS_ECALL_AUTO_DIAL_ATTEMPTS           0x021F	/* INT	3 */
#define EGTS_ECALL_MANUAL_DIAL_ATTEMPTS         0x0220	/* INT	3 */
#define EGTS_ECALL_AUTO_CAN_CANCEL              0x0221	/* BOOLEAN	1 */
#define EGTS_ECALL_MANUAL_CAN_CANCEL            0x0222	/* BOOLEAN	1 */
#define EGTS_ECALL_SMS_FALLBACK_NUMBER          0x0223	/* STRING	"" */

#define EGTS_CRASH_RECORD_TIME                  0x0251	/* INT/ 0..250	250 */
#define EGTS_CRASH_RECORD_RESOLUTION            0x0252	/* INT/1 …5	1 */
#define EGTS_CRASH_PRE_RECORD_TIME              0x0253	/* INT/ 0…20000	20000 */
#define EGTS_CRASH_PRE_RECORD_RESOLUTION        0x0254	/* INT/ 5…100	100 */

#define EGTS_VEHICLE_VIN                        0x0311	/* STRING	""  */
#define EGTS_VEHICLE_TYPE                       0x0312	/* INT	0 */
#define EGTS_VEHICLE_PROPULSION_STORAGE_TYPE    0x0313	/* INT	0 */


/******************************************************************************
*
*/

#define EGTS_PC_OK	              0U
/* completely done */

#define EGTS_PC_IN_PROGRESS	      1U	    
/* in progress */

#define EGTS_PC_UNS_PROTOCOL	    128U	  
/* unsupported protocol */

#define EGTS_PC_DECRYPT_ERROR     129U		
/* decription error */

#define EGTS_PC_PROC_DENIED       130U		
/* processing denied */

#define EGTS_PC_INC_HEADERFORM    131U		
/* incorrect header format */

#define EGTS_PC_INC_DATAFORM      132U		
/* incorrect data format */

#define EGTS_PC_UNS_TYPE          133U		
/* unsupported type */

#define EGTS_PC_NOTEN_PARAMS      134U		
/* incorrect parameters number */

#define EGTS_PC_DBL_PROC          135U		
/* attempt to retry processing */

#define EGTS_PC_PROC_SRC_DENIED   136U		
/* source data processing denied */

#define EGTS_PC_HEADERCRC_ERROR   137U		
/* header crc error */

#define EGTS_PC_DATACRC_ERROR     138U		
/* data crc error */

#define EGTS_PC_INVDATALEN        139U		
/* invalid data length */

#define EGTS_PC_ROUTE_NFOUND      140U		
/* route not found */

#define EGTS_PC_ROUTE_CLOSED      141U		
/* route closed */

#define EGTS_PC_ROUTE_DENIED      142U		
/* routing denied */

#define EGTS_PC_INVADDR           143U		
/* invalid address */

#define EGTS_PC_TTLEXPIRED        144U		
/* retranslation data amount exceed */

#define EGTS_PC_NO_ACK            145U		
/* no acknowledge */

#define EGTS_PC_OBJ_NFOUND        146U		
/* object not found */

#define EGTS_PC_EVNT_NFOUND       147U		
/* event not found */

#define EGTS_PC_SRVC_NFOUND       148U		
/* service not found */

#define EGTS_PC_SRVC_DENIED       149U		
/* service denied */

#define EGTS_PC_SRVC_UNKN         150U		
/* unknown service type */

#define EGTS_PC_AUTH_DENIED       151U		
/* authorization denied */

#define EGTS_PC_ALREADY_EXISTS    152U		
/* object already exist */

#define EGTS_PC_ID_NFOUND         153U		
/* identificator not found */

#define EGTS_PC_INC_DATETIME      154U		
/* date or time incorrect */

#define EGTS_PC_IO_ERROR          155U		
/* input / output error */

#define EGTS_PC_NO_RES_AVAIL      156U		
/* resources not available */

#define EGTS_PC_MODULE_FAULT      157U		
/* module internal fault */

#define EGTS_PC_MODULE_PWR_FLT    158U		
/* module power fault */

#define EGTS_PC_MODULE_PROC_FLT   159U		
/* module processor fault */

#define EGTS_PC_MODULE_SW_FLT     160U		
/* module software fault */

#define EGTS_PC_MODULE_FW_FLT     161U		
/* module firmware fault */

#define EGTS_PC_MODULE_IO_FLT     162U		
/* module input/output fault */

#define EGTS_PC_MODULE_MEM_FLT    163U		
/* module memory fault */

#define EGTS_PC_TEST_FAILED       164U		
/* test failed */


/* TODO: document */
#define EGTS_PC_UNKNOWN           255U		
/* unknown error*/

/******************************************************************************
*
*/

/* TODO: provosion for future */
#define EGTS_BACK_BUF_SIZE     65535U



/******************************************************************************
*
*/

#endif

