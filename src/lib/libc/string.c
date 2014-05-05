#include "include/string.h"

void *memcpy( void *dest, const void *src, size_t n )
{
	unsigned char *tdest = (unsigned char*)dest;
	const unsigned char *tsrc = (const unsigned char*)src;

	while(n--)
	{
			*tdest = *tsrc;
			tdest++; tsrc++;
	}

	return dest;
}

/*
void *memmove( void *dest, const void *src, size_t n )
{
	//TODO

	return dest;
}
*/

char *strcpy( char *dest, const char *src )
{
	return memcpy( (void*)dest, (void*)src, strlen(src) + 1 );
}

char *strncpy( char *dest, const char *src, size_t n )
{
	size_t len = strlen(src) + 1;

	if(len > n)
	{
		len = n;
	}

	memcpy( dest, src, len);

	if(len < n)
	{
		memset( dest + n, '\0', n - len);
	}
	
	return dest;
}


char *strcat( char *dest, const char *src )
{
	char *tdest = (char*)dest;
	const char *tsrc = (const char*)src;
 
	while( *tdest != '\0' )
	{
		tdest++;
	}

	while( *tsrc != '\0' )
	{
		*tdest = *tsrc;
		tdest++; tsrc++;
	}

	*tdest = '\0';

	return dest;
}

char *strncat( char *dest, const char *src, size_t n )
{
	char *tdest = (char*)dest;
	const char *tsrc = (const char*)src;
 
	while( *tdest != '\0' )
	{
		tdest++;
	}

	while( *tsrc != '\0' && n > 0)
	{
		*tdest = *tsrc;
		tdest++; tsrc++; n--;
	}

	*tdest = '\0';

	return dest;
}

int memcmp( const void *ptr1, const void *ptr2, size_t n )
{
	const unsigned char *tptr1 = (const unsigned char*) ptr1;
	const unsigned char *tptr2 = (const unsigned char*) ptr2;

	while(n > 0)
	{
		if( *tptr1 > *tptr2 )
		{
			return 1;
		}
		else if( *tptr1 < *tptr2 )
		{
			return -1;
		}

		tptr1++; tptr2++; n--;
	}

	return 0;
}

int strcmp( const char *str1, const char *str2 )
{
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);

	size_t len_ = len2;

	if(len1 < len2)
	{
		len_ = len1;
	}

	int cmpr = memcmp( str1, str2, len_ );
	
	if(cmpr != 0)
	{
		return cmpr;
	}

	if(len1 > len2)
	{
		return 1;
	}
	else if(len1 < len2)
	{
		return -1;
	}

	return 0;
}

int strncmp( const char *str1, const char *str2, size_t n )
{
	return memcmp( str1, str2, n);
}


void *memchr( const void *ptr, int value, size_t n )
{
	const unsigned char *tptr = (const unsigned char*)ptr;
	
	while(n > 0)
	{
		if(*tptr == (unsigned char)value)
		{
			return (void*)tptr;
		}

		tptr++; n--;
	}

	return NULL;
}

char *strchr( const char *str, int chr )
{
	while(*str != '\0')
	{
		if(*str == chr)
		{
			return (char*)str;
		}

		str++;
	}
	
	return NULL;
}

size_t strcspn( const char *str1, const char *str2 )
{
	size_t pos = 0;

	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);

	size_t i;

	for(; *str1 != '\0'; str1++)
	{
		for(i = 0; i < len2; i++)
		{
			if(*str1 == str2[i])
			{
				return pos;
			}
		}

		pos++;
	}

	return len1;
}

char *strpbrk( const char *str1, const char *str2 )
{
	size_t len2 = strlen(str2);

	size_t i;

	for(; *str1 != '\0'; str1++)
	{
		for(i = 0; i < len2; i++)
		{
			if(*str1 == str2[i])
			{
				return (char*)str1;
			}
		}
	}

	return NULL;
}

char *strrchr( const char *str, int chr )
{
	char *occ = NULL;
	
	while(*str != '\0')
	{
		if(*str == chr)
		{
			occ = (char*)str;
		}

		str++;
	}

	return occ;
}

size_t strspn( const char *str1, const char *str2 )
{
	size_t len = 0;

	size_t i;

	int found = 1;

	while(*str1 && found)
	{
		found = 0;

		for(i = 0; i < strlen(str2); i++)
		{
			if(*str1 == str2[i])
			{
				len++;
				found = 1;
				break;
			}
		}
		
		str1++;
	}

        return len;
}

/*
char *strstr( const char *str1, const char *str2 )
{
	//TODO
	
	return (char*)str1;
}
*/

char *strtok( char *str, const char *delimiters )
{
	static char *prevstr;

	if(str == NULL)
	{
		if(prevstr == NULL)
		{
			return NULL;
		}

		str = prevstr;
	}

	str += strspn(str, delimiters);

	if(*str == '\0')
	{
		return NULL;
	}

	size_t str_new_len = strcspn(str, delimiters);
       
        if(str[str_new_len] == '\0') 
	{
                prevstr = NULL;
	}
        else 
	{
                str[str_new_len] = '\0';
                prevstr = str + str_new_len + 1;
        }

	return str;
}



void *memset( void *ptr, int value, size_t n )
{
	unsigned char *tptr = (unsigned char*)ptr;

	while(n > 0)
	{
		*tptr = value; 
		tptr++; n--;
	}

	return ptr;
}

size_t strlen( const char *str )
{
	size_t len = 0;

	while( *str != '\0' )
	{
		str++; len++;
	}

	return len;
}
