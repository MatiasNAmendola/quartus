#ifndef STDIO_H
#define STDIO_H

#include "stdarg.h"

#define BUFSIZE		1024
#define EOF		(-1)
#define FILENAME_MAX	65535
#define FOPEN_MAX	8
#define L_tmpnam	20

#ifndef NULL
	#define NULL		((void*)0x0)
#endif

#define TMP_MAX		25

typedef __SIZE_TYPE__	size_t;


int snprintf( char *s, size_t n, const char *format, ... );
int sprintf( char *str, const char *format, ... );


int vsnprintf( char *s, size_t n, const char *format, va_list arg );
int vsprintf( char *s, const char *format, va_list arg );

#endif
