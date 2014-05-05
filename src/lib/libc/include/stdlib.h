#ifndef STDLIB_H
#define STDLIB_H

#define EXIT_FAILURE	1
#define EXIT_SUCCESS	0

#ifndef NULL
	#define NULL		((void*)0x0)
#endif

#define RAND_MAX	32767

typedef __SIZE_TYPE__	size_t;

typedef struct 
{
	int quot;
  	int rem;
} div_t;

typedef struct 
{
	long int quot;
  	long int rem;
} ldiv_t;

typedef struct 
{
	long long quot;
	long long rem;
} lldiv_t;

int atoi( const char *str );
long int atol( const char *str );
long long int atoll( const char *str );


int rand();
void srand( unsigned int seed );


int abs( int n );
div_t div( int number, int denom );
long int labs( long int n );
ldiv_t ldiv( long int numer, long int denom );
long long int llabs (long long int n);
lldiv_t lldiv( long long int numer, long long int denom );


#endif
