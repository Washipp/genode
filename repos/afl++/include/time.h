#ifndef GENODE_TIME_H
#define GENODE_TIME_H

#include "sys/select.h"

struct timezone
{
    int tz_minuteswest;		/* Minutes west of GMT.  */
    int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
};
typedef long int time_t;

struct tm
{
    int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
    int tm_min;			/* Minutes.	[0-59] */
    int tm_hour;			/* Hours.	[0-23] */
    int tm_mday;			/* Day.		[1-31] */
    int tm_mon;			/* Month.	[0-11] */
    int tm_year;			/* Year	- 1900.  */
    int tm_wday;			/* Day of week.	[0-6] */
    int tm_yday;			/* Days in year.[0-365]	*/
    int tm_isdst;			/* DST.		[-1/0/1]*/
    long int tm_gmtoff;		/* Seconds east of UTC.  */
    const char *tm_zone;		/* Timezone abbreviation.  */
};

#ifdef __cplusplus
extern "C" {
#endif

/* Get the current time of day, putting it into *TV.
If TZ is not null, *TZ must be a struct timezone, and both fields
will be set to zero.
Calling this function with a non-null TZ is obsolete;
use localtime etc. instead.
This function itself is semi-obsolete;
most callers should use time or clock_gettime instead. */
int gettimeofday(struct timeval *__restrict __tv, void *__restrict __tz);

/* Return the current time and put it in *TIMER if TIMER is not NULL.  */
time_t time(time_t *__timer);

/* Return the `struct tm' representation of *TIMER in local time, using *TP to store the result.  */
struct tm *localtime_r(const time_t *__restrict __timer, struct tm *__restrict __tp);

/* Return the `struct tm' representation of *TIMER in the local timezone.  */
struct tm *localtime(const time_t *__timer);

#ifdef __cplusplus
}
#endif

#endif //GENODE_TIME_H
