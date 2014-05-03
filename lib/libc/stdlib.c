#include "include/stdlib.h"

int atoi( const char *str )
{
	int r = 0;
	
	int sign = 1;
	
	while( *str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' || *str == '\v' )
	{
		str++;
	}

	if(*str == '-')
	{
		sign = -1;
		str++;
	}

	while(*str != '\0')
	{
		if( (*str - '0') < 0 || (*str - '0') > 9 )
		{
			return (r * sign);
		}
		else
		{
			r = r * 10 + (*str - '0');
		}

		str++;
	}

	return (r * sign);
}

long int atol( const char *str )
{
	long int r = 0;
	
	long int sign = 1;
	
	while( *str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' || *str == '\v' )
	{
		str++;
	}

	if(*str == '-')
	{
		sign = -1;
		str++;
	}

	while(*str != '\0')
	{
		if( (*str - '0') < 0 || (*str - '0') > 9 )
		{
			return (r * sign);
		}
		else
		{
			r = r * 10 + (*str - '0');
		}

		str++;
	}

	return (r * sign);
}

long long int atoll( const char *str )
{
	long long int r = 0;
	
	long long int sign = 1;
	
	while( *str == ' ' || *str == '\t' || *str == '\r' || *str == '\n' || *str == '\v' )
	{
		str++;
	}

	if(*str == '-')
	{
		sign = -1;
		str++;
	}

	while(*str != '\0')
	{
		if( (*str - '0') < 0 || (*str - '0') > 9 )
		{
			return (r * sign);
		}
		else
		{
			r = r * 10 + (*str - '0');
		}

		str++;
	}

	return (r * sign);
}

static unsigned long ran = 1;

int rand()
{
	ran = ran * 135797531 + 123456;

	return (ran % RAND_MAX + 1);
}

void srand( unsigned int seed )
{
	ran = seed;
}

int abs( int n )
{
	if(n > 0)
	{
		return n;
	}
	else
	{
		return (n * (-1));
	}
}

div_t div( int numer, int denom )
{
	div_t r;

	r.quot = numer / denom;
	r.rem  = numer % denom;
	
	return r;
}

long int labs( long int n )
{
	if(n > 0)
	{
		return n;
	}
	else
	{
		return (n * (-1));
	}
}

ldiv_t ldiv( long int numer, long int denom )
{
	ldiv_t r;

	r.quot = numer / denom;
	r.rem  = numer % denom;
	
	return r;
}

long long int llabs (long long int n)
{
	if(n > 0)
	{
		return n;
	}
	else
	{
		return (n * (-1));
	}
}

/*lldiv_t lldiv( long long int numer, long long int denom )
{
	lldiv_t r;

	r.quot = numer / denom;
	r.rem  = numer % denom;
	
	return r;
}*/
