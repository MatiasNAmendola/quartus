#ifndef STRING_H
#define STRING_H

#ifndef NULL
	#define NULL		((void*)0x0)
#endif

typedef __SIZE_TYPE__  	size_t;

void *memcpy( void *dest, const void *src, size_t n );
void *memmove( void *dest, const void *src, size_t n );	//TODO
char *strcpy( char *dest, const char *src );
char *strncpy( char *dest, const char *src, size_t n );

char *strcat( char *dest, const char *src );
char *strncat( char *dest, const char *src, size_t n );

int memcmp( const void *ptr1, const void *ptr2, size_t n );
int strcmp( const char *str1, const char *str2 );
int strncmp( const char *str1, const char *str2, size_t n );

void *memchr( const void *ptr, int value, size_t n );
char *strchr( const char *str, int chr );
size_t strcspn( const char *str1, const char *str2 );
char *strpbrk( const char *str1, const char *str2 );
char *strrchr( const char *str, int chr );

size_t strspn( const char *str1, const char *str2 );	
char *strstr( const char *str1, const char *str2 );	//TODO
char *strtok( char *str, const char *delimiters );	

void *memset( void *ptr, int value, size_t n );
size_t strlen( const char *str );

#endif
