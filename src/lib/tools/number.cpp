#include "include/number.hpp"

int tools::stoi( const char *str, unsigned int base )
{
	int number = 0;
	
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
		unsigned char digit = *str;

		if(digit >= '0' && digit <= '9')
		{
			if((unsigned char)(digit - '0') < base)
			{
				number = number * base + (digit - '0');
			}
		}
		else if(digit >= 'A' && digit <= 'Z')
		{
			if((unsigned char)(digit - 55) < base)
			{
				number = number * base + (digit - 55);
			}
		}
		else if(digit >= 'a' && digit <= 'z')
		{
			if((unsigned char)(digit - 87) < base)
			{
				number = number * base + (digit - 87);
			}
		}
		else
		{
			return (number * sign);
		}

		str++;
	}

	return (number * sign);
}
