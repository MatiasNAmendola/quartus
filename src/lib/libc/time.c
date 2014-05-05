#include "include/time.h"
#include "include/stdio.h"

extern time_t ktime();


/*
	time_t unixtime(int year, int month, int day, int hour, int minute, int second)

	Aus WIKIPEDIA Die freie Enzyklopädie

	http://de.wikipedia.org/wiki/Unixzeit#Beispiel-Implementierung
*/
time_t unixtime( int year, int month, int day, int hour, int minute, int seconds )
{
	static const short yday[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
 
	time_t unix_time;
	long years = year - 1970 ;
	int leap_year = ((year - 1) - 1968) / 4 - ((year - 1) - 1900) / 100 + ((year - 1) - 1600) / 400;
 
	unix_time = seconds + 60 * minute + 60 * 60 * hour + (yday[month - 1] + day - 1) * 60 * 60 * 24 + (years * 365 + leap_year) * 60 * 60 *24;
 
  	if ( (month > 2) && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) )
	{
    		unix_time += 60 * 60 * 24; 
	}

	return unix_time;
} 


clock_t clock();

double difftime( time_t end, time_t beg )
{
	return (double)(end - beg);
}

time_t mktime( struct tm *timeptr );

time_t time( time_t *timer )
{
	time_t time = 0;

	if(timer != NULL)
	{
		*timer = time;
	}

	return time;
}


char *asctime( const struct tm *timeptr )
{
	static const char wday_name[][4] = 
	{
    		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  	};
  	static const char mon_name[][4] = 
	{
    		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  	};

  	static char result[26];
  	sprintf(result, "%s %s %.2d %.2d:%.2d:%.2d %d\n", wday_name[timeptr->tm_wday], mon_name[timeptr->tm_mon], timeptr->tm_mday, timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec, 1900 + timeptr->tm_year);
  
	return result;
}

char *ctime( const time_t *timer )
{
	return asctime( localtime(timer) );
} 

struct tm *gmtime( const time_t	*timer )
{	
	static int z[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

	int t = *timer;

	int y, m, d, yd;

	static struct tm time;

	time.tm_sec 	= t % 60; t = t / 60;
	time.tm_min 	= t % 60; t = t / 60;
	time.tm_hour	= t % 24; t = t / 24;

	time.tm_yday	= t % 365; t = t / 365;
	time.tm_year	= 1970 + t - 1900;

	y = time.tm_year + 1900;

	int num_leap_years 	= ( ((y - 1) - 1968) / 4 - ((y - 1) - 1900) / 100 + ((y - 1) - 1600) / 400 );
	int is_leap_year 	= ( 1 / (1 + y % 4) - 1 / (1 + y % 100) + 1 / (1 + y % 400) );

	time.tm_yday	= time.tm_yday - num_leap_years;

	yd = time.tm_yday + 1;
	time.tm_mon	= ( yd + 30 + (yd + is_leap_year + 400) / 460 * (2 - is_leap_year) ) * 16 / 489 - 1;
	
	m = time.tm_mon + 1;
	time.tm_mday	= yd + 30 - 489 * m / 16 + (7 + m) / 10 * (2 - is_leap_year) ;

	if(time.tm_mon < 0)
	{
		time.tm_year -= 1;

		y = time.tm_year + 1900;
		is_leap_year 	= ( 1 / (1 + y % 4) - 1 / (1 + y % 100) + 1 / (1 + y % 400) );
		if(is_leap_year)
		{
			time.tm_yday = 366 + time.tm_yday;
		}
		else
		{
			time.tm_yday = 365 + time.tm_yday;
		}

		yd = time.tm_yday + 1;
		time.tm_mon	= ( yd + 30 + (yd + is_leap_year + 400) / 460 * (2 - is_leap_year) ) * 16 / 489 - 1;

		m = time.tm_mon + 1;
		time.tm_mday	= yd + 30 - 489 * m / 16 + (7 + m) / 10 * (2 - is_leap_year) ;
	}
	
	d = time.tm_mday;
    	y -= m < 3;
  	time.tm_wday = (y + y / 4 - y / 100 + y / 400 + z[m - 1] + d) % 7;

	time.tm_isdst	= 0;

	if(time.tm_sec < 0)
	{
		time.tm_sec = 60 + time.tm_sec;
	}

	if(time.tm_min < 0)
	{
		time.tm_min = 59 + time.tm_min;

		if(time.tm_sec == 0)
		{
			time.tm_min += 1;
		}
	}

	if(time.tm_hour < 0)
	{
		time.tm_hour = 23 + time.tm_hour;
	}

	return &time;

}

struct tm *localtime( const time_t *timer )
{
	return gmtime(timer);
}

