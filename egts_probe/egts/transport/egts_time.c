/*****************************************************************************/
/*                                                                           */
/* File: egts_tx.c                                                           */
/*                                                                           */
/* System: ERA GLONASS terminal communication protocol reference impl.       */
/*                                                                           */
/* Component Name: Transport Layer                                           */
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
/* Description: Date and time related functions.                             */ 
/*                                                                           */
/* Additional information: -                                                 */
/*                                                                           */
/* Functions: public : egts_tx_packet.                                       */
/*                                                                           */
/*****************************************************************************/

#include "../include/egts_config.h"

#include "../include/egts.h"
#include "../include/egts_impl.h"

/******************************************************************************
*
*/

#define SECS_DAY        (24L * 60L * 60L)
#define LEAPYEAR(year)  (!((year) % 4U) && (((year) % 100U) || !((year) % 400U)))
#define YEARSIZE(year)  (LEAPYEAR(year) ? 366U : 365U)

const u32 _ytab[2][12] = { 
        { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

/******************************************************************************
* prototypes
*/

/*****************************************************************************/
/*                                                                           */
/* egts_compose_datetime()                                                   */
/*                                                                           */
/* Description: Helper function to compose datatime field using specified    */
/*   epoche                                                                  */
/*                                                                           */
/* Arguments: s - seconds, 0..59                                             */
/*            n - minutes, 0..59                                             */
/*            h - hours, 0..24                                               */
/*            d - day of month, 1..31                                        */
/*            m - month, 1..12                                               */
/*            y - year, >=2010                                               */ 
/*            epoche - epoche year                                           */
/*                                                                           */
/* Return:    EGTS representaion of datetime                                 */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
u32 egts_compose_datetime( u8 s, u8 n, u8 h, u8 d, u8 m, u32 y , u32 epoche );

/*****************************************************************************/
/*                                                                           */
/* egts_decompose_datetime()                                                 */
/*                                                                           */
/* Description: Helper function to decompose datatime field using specified  */
/*   epoche                                                                  */
/*                                                                           */
/* Arguments: dt - datetime in seconds since epoche                          */  
/*            ps - seconds, 0..59                                            */
/*            pn - minutes, 0..59                                            */
/*            ph - hours, 0..24                                              */
/*            pd - day of month, 1..31                                       */
/*            pm - month, 1..12                                              */
/*            py - year, >=2010                                              */ 
/*            epoche - epoche year                                           */
/*                                                                           */
/* Return:    EGTS representaion of datetime                                 */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static
void  egts_decompose_datetime( u32 dt , u8 *ps, u8 *pn, u8 *ph, u8 *pd, u8 *pm, u32 *py , u32 epoche );

/******************************************************************************
*
*/

/*****************************************************************************/
/*                                                                           */
/* egts_compose_datetime()                                                   */
/*                                                                           */
/* Description: Helper function to compose datatime field using specified    */
/*   epoche                                                                  */
/*                                                                           */
/* Arguments: s - seconds, 0..59                                             */
/*            n - minutes, 0..59                                             */
/*            h - hours, 0..24                                               */
/*            d - day of month, 1..31                                        */
/*            m - month, 1..12                                               */
/*            y - year, >=2010                                               */ 
/*            epoche - epoche year                                           */
/*                                                                           */
/* Return:    EGTS representaion of datetime                                 */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static 
u32 egts_compose_datetime( u8 s, u8 n, u8 h, u8 d, u8 m, u32 y , u32 epoche )
{
  u32 dayno;
  u32 year, mon, dyear, dmon;
  
  dyear = y;
  dayno = 0U;
  for ( year = epoche; year < dyear; year++ ) {
    dayno += YEARSIZE(year);
  }
  
  dmon = m - 1;
  for ( mon = 0; mon < dmon; mon++ ) {
    dayno += _ytab[LEAPYEAR(year)][mon];
  }

  dayno += ( d - 1 );
    
  return  dayno * SECS_DAY + 
          (u32)h * 3600U +
          (u32)n * 60U +
          (u32)s;
}  

/*****************************************************************************/
/*                                                                           */
/* egts_decompose_datetime()                                                 */
/*                                                                           */
/* Description: Helper function to decompose datatime field using specified  */
/*   epoche                                                                  */
/*                                                                           */
/* Arguments: dt - datetime in seconds since epoche                          */  
/*            ps - seconds, 0..59                                            */
/*            pn - minutes, 0..59                                            */
/*            ph - hours, 0..24                                              */
/*            pd - day of month, 1..31                                       */
/*            pm - month, 1..12                                              */
/*            py - year, >=2010                                              */ 
/*            epoche - epoche year                                           */
/*                                                                           */
/* Return:    EGTS representaion of datetime                                 */
/*                                                                           */
/* Other:                                                                    */
/*                                                                           */
/*****************************************************************************/

static
void  egts_decompose_datetime( u32 dt , u8 *ps, u8 *pn, u8 *ph, u8 *pd, u8 *pm, u32 *py , u32 epoche )
{
  u32 dayclock, dayno;
  u32 year = epoche;
    
  dayclock = dt % SECS_DAY;
  dayno = dt / SECS_DAY;

  *ps = (u8)(dayclock % 60U);
  *pn = (u8)( (dayclock % 3600U) / 60U );
  *ph = (u8)( dayclock / 3600U );

  while (dayno >= YEARSIZE(year)) 
  {
    dayno -= YEARSIZE(year);
    year++;
  }

  *pm = 0U;
  while ( dayno >= _ytab[LEAPYEAR(year)][*pm] ) 
  {
    dayno -= _ytab[LEAPYEAR(year)][*pm];
    (*pm)++;
  }
  (*pm)++;

  *py = year;
  *pd = (u8)( dayno + 1 );

}

/*****************************************************************************/
/*                                                                           */
/* egts_set_tm()                                                             */
/*                                                                           */
/* Description: Helper function to set EGTS datetime fileds                  */
/*                                                                           */
/* Arguments: s - seconds, 0..59                                             */
/*            n - minutes, 0..59                                             */
/*            h - hours, 0..24                                               */
/*            d - day of month, 1..31                                        */
/*            m - month, 1..12                                               */
/*            y - year, >=2010                                               */ 
/*                                                                           */
/* Return:    EGTS representaion of datetime, seconds since                  */
/*   00:00:00 01.01.2010 UTC,                                                */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

u32   egts_set_tm( u8 s, u8 n, u8 h, u8 d, u8 m, u32 y )
{
  /* epoche 00:00:00 01.01.2010 UTC */
  return egts_compose_datetime( s , n , h , d , m , y , 2010U );
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_tm()                                                             */
/*                                                                           */
/* Description: Helper function to convert EGTS datetime field               */
/*                                                                           */
/* Arguments: dt - EGTS datetime                                             */
/*            ps - pointer to get seconds, 0..59                             */
/*            pn - pointer to get minutes, 0..59                             */
/*            ph - pointer to get hours, 0..24                               */
/*            pd - pointer to get day of month, 1..31                        */
/*            pm - pointer to get month, 1..12                               */
/*            py - pointer to get year, >=2010                               */ 
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

void  egts_get_tm( u32 dt , u8 *ps, u8 *pn, u8 *ph, u8 *pd, u8 *pm, u32 *py )
{
  /* epoche 00:00:00 01.01.2010 UTC */
  egts_decompose_datetime( dt , ps, pn, ph, pd, pm, py , 2010U );
}

/*****************************************************************************/
/*                                                                           */
/* egts_set_tm()                                                             */
/*                                                                           */
/* Description: Helper function to set EGTS datetime fileds to               */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: s - seconds, 0..59                                             */
/*            n - minutes, 0..59                                             */
/*            h - hours, 0..24                                               */
/*            d - day of month, 1..31                                        */
/*            m - month, 1..12                                               */
/*            y - year, >=2010                                               */ 
/*                                                                           */
/* Return:    EGTS representaion of datetime, seconds since                  */
/*   epoche 00:00:00 01.01.1970 UTC                                          */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

u32   egts_set_msd_ts( u8 s, u8 n, u8 h, u8 d, u8 m, u32 y )
{
  /* epoche 00:00:00 01.01.1970 UTC */
  return egts_compose_datetime( s , n , h , d , m , y , 1970U );
}

/*****************************************************************************/
/*                                                                           */
/* egts_get_tm()                                                             */
/*                                                                           */
/* Description: Helper function to convert EGTS datetime field fo            */
/*   egts_ECALL_MSD_DATA_t                                                   */
/*                                                                           */
/* Arguments: dt - MSD EGTS datetime, seconds since                          */
/*                   epoche 00:00:00 01.01.1970 UTC                          */
/*            ps - pointer to get seconds, 0..59                             */
/*            pn - pointer to get minutes, 0..59                             */
/*            ph - pointer to get hours, 0..24                               */
/*            pd - pointer to get day of month, 1..31                        */
/*            pm - pointer to get month, 1..12                               */
/*            py - pointer to get year, >=2010                               */ 
/*                                                                           */
/* Return:    nothing                                                        */
/*                                                                           */
/* Other:     function intended for public usage                             */
/*                                                                           */
/*****************************************************************************/

void  egts_get_msd_ts( u32 dt , u8 *ps, u8 *pn, u8 *ph, u8 *pd, u8 *pm, u32 *py )
{
  /* epoche 00:00:00 01.01.1970 UTC */
  egts_decompose_datetime( dt , ps, pn, ph, pd, pm, py , 1970U );
}

/******************************************************************************
*
*/

