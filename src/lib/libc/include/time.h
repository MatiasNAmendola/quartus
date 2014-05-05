#ifndef TIME_H
#define TIME_H

#ifndef NULL
	#define NULL		((void*)0x0)
#endif

typedef long long	clock_t;
typedef __SIZE_TYPE__	size_t;
typedef long long	time_t;

struct tm
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

clock_t clock();
double difftime( time_t end, time_t beg );
time_t mktime( struct tm *timeptr );
time_t time( time_t *timer );


char *asctime( const struct tm *timeptr );
char *ctime( const time_t *timer ); 
struct tm *gmtime( const time_t *timer );
struct tm *localtime( const time_t *timer );

#endif
