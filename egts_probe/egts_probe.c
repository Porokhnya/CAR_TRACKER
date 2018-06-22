/*****************************************************************************/
/*                                                                           */
/* File: egts_probe.c                                                        */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Tests                                                     */
/*                                                                           */
/* Status: Version 1.1                                                       */
/*                                                                           */
/* Language: C                                                               */
/*                                                                           */
/* (c) Copyright JSC �Navigation-information systems�, 2011                  */
/*                                                                           */
/* Address:                                                                  */
/*     24, Mishina Str., bld.1                                               */
/*     Moscow, Russia                                                        */
/*                                                                           */
/* Description: Misceleniouse routines for autotests. It is not a part for   */
/*   mandatory EGTS sources.                                                 */
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions:                                                                */
/*                                                                           */
/*****************************************************************************/

#include "egts/include/egts_config.h"
#include "egts/include/egts.h"
#include "egts/include/egts_impl.h"
#include "egts/transport/egts_guts.h"

#include "egts_probe.h"
#include "egts/services/egts_services.h"

/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* egts_compare_records()                                                    */
/*                                                                           */
/* Description: Memberwise compare two records                               */
/*                                                                           */
/* Arguments: precord1 - first record                                        */
/*            precord2 - second record                                       */
/*                                                                           */
/* Return:    zero if equial success, nonzero otherwise                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static
int egts_compare_records( 
  egts_record_t*   precord1 ,
  egts_record_t*   precord2 );

/*****************************************************************************/
/*                                                                           */
/* egts_compare_records()                                                    */
/*                                                                           */
/* Description: Memberwise compare two records                               */
/*                                                                           */
/* Arguments: precord1 - first record                                        */
/*            precord2 - second record                                       */
/*                                                                           */
/* Return:    zero if equial success, nonzero otherwise                      */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static
int egts_compare_records( 
  egts_record_t*   precord1 ,
  egts_record_t*   precord2 )
{
  if ( precord1->record.RL != precord2->record.RL ) {
    egts_dbg_printf("inequial RL");
    return -1;
  }
  if ( precord1->record.RN != precord2->record.RN ) {
    egts_dbg_printf("inequial RN");
    return -1;
  }
  if ( precord1->record.SSOD != precord2->record.SSOD ) {
    egts_dbg_printf("inequial SSOD");
    return -1;
  }
  if ( precord1->record.RSOD != precord2->record.RSOD ) {
    egts_dbg_printf("inequial RSOD");
    return -1;
  }
  if ( precord1->record.GRP != precord2->record.GRP ) {
    egts_dbg_printf("inequial GRP");
    return -1;
  }
  if ( precord1->record.RPP != precord2->record.RPP ) {
    egts_dbg_printf("inequial RPP");
    return -1;
  }
  if ( precord1->record.TMFE != precord2->record.TMFE ) {
    egts_dbg_printf("inequial TMFE");
    return -1;
  }
  if ( precord1->record.EVFE != precord2->record.EVFE ) {
    egts_dbg_printf("inequial EVFE");
    return -1;
  }
  if ( precord1->record.OBFE != precord2->record.OBFE ) {
    egts_dbg_printf("inequial OBFE");
    return -1;
  }

  if (precord1->record.OBFE )
  {
    if ( precord1->record.OID != precord2->record.OID ) {
      egts_dbg_printf("inequial OID");
      return -1;
    }
  }
  if (precord1->record.EVFE )
  {
    if ( precord1->record.EVID != precord2->record.EVID ) {
      egts_dbg_printf("inequial EVID");
      return -1;
    }
  }
  if (precord1->record.TMFE )
  {
    if ( precord1->record.TM != precord2->record.TM ) {
      egts_dbg_printf("inequial TM");
      return -1;
    }
  }

  if ( precord1->record.SST != precord2->record.SST ) {
    egts_dbg_printf("inequial SST");
    return -1;
  }
  if ( precord1->record.RST != precord2->record.RST ) {
    egts_dbg_printf("inequial RST");
    return -1;
  }

  return 0;
}


/*****************************************************************************/
/*                                                                           */
/* estate_rx_packet_probe()                                                  */
/*                                                                           */
/* Description: Probe analizer. For special probes only.                     */
/*                                                                           */
/* Arguments: ctx - call context                                             */
/*            pheader - transport header                                     */
/*            signed_up - signature mark, 1/0                                */
/*            presponce - responce data                                      */
/*            precords - array of records                                    */
/*            nrecords - record count                                        */
/*            FDL - frame data length                                        */
/*                                                                           */
/* Return:    zero on success, nonzero on error                              */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

int  estate_rx_packet_probe( void* ctx , 
  egts_header_t*            pheader , 
  u8                        signed_up ,
  egts_responce_header_t*   presponce ,
  egts_record_t*            precords ,
  u16                       nrecords ,
  u16                       FDL )
{
  u16 i,j;
  egts_record_t *precord1, *precord2;
  const egts_subrecord_handler_t* chandler = NULL;
  egts_subrecord_t *psubrecord1, *psubrecord2;

  (void)ctx;
  (void)pheader;
  (void)signed_up;
  (void)FDL;

  if ( !precords || !nrecords )
  {
    egts_dbg_printf("noting received");
    probes_ctx.nerr++;
    return probes_ctx.last_result = -1;
  }
  if ( !probes_ctx.precords || !probes_ctx.nrecords )
  {
    egts_dbg_printf("probe context error");
    probes_ctx.nerr++;
    return probes_ctx.last_result = -1;
  }
  if ( nrecords != probes_ctx.nrecords )
  {
    egts_dbg_printf("inequial record count");
    probes_ctx.nerr++;
    return probes_ctx.last_result = -1;
  }

  if ( presponce )
  {
    if ( !probes_ctx.presponce )
    {
      egts_dbg_printf("unexpected responce");
      probes_ctx.nerr++;
      return probes_ctx.last_result = -1;
    }
    if ( probes_ctx.presponce->PR != presponce->PR )
    {
      egts_dbg_printf("inequial responce.PR");
      probes_ctx.nerr++;
      return probes_ctx.last_result = -1;
    }
    if ( probes_ctx.presponce->RPID != presponce->RPID )
    {
      egts_dbg_printf("inequial responce.RPID");
      probes_ctx.nerr++;
      return probes_ctx.last_result = -1;
    }
  }
  else
  {
    if ( probes_ctx.presponce )
    {
      egts_dbg_printf("responce missed");
      probes_ctx.nerr++;
      return probes_ctx.last_result = -1;
    }
  }

  for ( precord1 = precords, precord2 = probes_ctx.precords, i = 0; 
        i < nrecords; 
        i++ , precord1++ , precord2++ )
  {
    if ( 0 != egts_compare_records( precord1 , precord2 ) )
    {
      egts_dbg_printf("records inequial");
      probes_ctx.nerr++;
      return probes_ctx.last_result = -1;
    }
    if ( !precord1->psubrecords || !precord1->nsubrecords )
    {
      egts_dbg_printf("no subrecords in received record");
      probes_ctx.nerr++;
      return probes_ctx.last_result = -1;
    }
    if ( precord1->nsubrecords != precord2->nsubrecords )
    {
      egts_dbg_printf("inequial subrecords count");
      probes_ctx.nerr++;
      return probes_ctx.last_result = -1;
    }
    for ( psubrecord1 = precord1->psubrecords , psubrecord2 = precord2->psubrecords , j = 0; 
          j < precord1->nsubrecords; 
          j++ , psubrecord1++ , psubrecord2++ )
    {
      if ( !psubrecord1->SRD )
      {
        egts_dbg_printf("no data in subrecord");
        probes_ctx.nerr++;
        return probes_ctx.last_result = -1;
      }
      if ( !psubrecord2->SRD )
      {
        egts_dbg_printf("no data in probe context subrecord");
        probes_ctx.nerr++;
        return probes_ctx.last_result = -1;
      }
      if ( psubrecord1->subrecord.SRT != psubrecord2->subrecord.SRT )
      {
        egts_dbg_printf("inequial subrecords types");
        probes_ctx.nerr++;
        return probes_ctx.last_result = -1;
      }
      if ( psubrecord1->subrecord.SRL != psubrecord2->subrecord.SRL )
      {
        egts_dbg_printf("inequial subrecords data length");
        probes_ctx.nerr++;
        return probes_ctx.last_result = -1;
      }
      chandler = egts_find_subrecord_handler( precord1->record.RST , psubrecord1->subrecord.SRT );
      if ( !chandler )
      {
        egts_dbg_printf("subrecord handler not found");
        probes_ctx.nerr++;
        return probes_ctx.last_result = -1;;
      }
      if ( 0 != chandler->fs_is_equial(
        NULL ,
        psubrecord1->SRD , 
        psubrecord2->SRD ) )
      {
        egts_dbg_printf("subrecords context inequial");
        probes_ctx.nerr++;
        return probes_ctx.last_result = -1;
      }
    }
  }

  return probes_ctx.last_result = 0;
}


/******************************************************************************
*
*/

