#include "include/stdio.h"
#include "include/ctype.h"
#include "include/stdlib.h"
#include "include/string.h"

char *sputn( char *s, int n, int base, int precision )
{
	static const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz"; 
	
	static char buf[65];
	char *p;

	memset(buf, 0, 65);

	int len = 0;

	p = buf + 64;

	do 
	{
		int d = n % base;

		n /= base;

		--p;
		*p = (char)digits[d];

		len++;
		
	} while(n);

	while(len < precision)
	{
		*--p = '0';

		len++;
	}

	if(s)
	{
		strcpy(s, p);
	}

	return p;
}

int snprintf( char *s, size_t size, const char *format, ... )
{
	va_list vl;
	va_start(vl, format);

	int n = vsnprintf(s, size, format, vl );

	va_end(vl);

	return n;
}

int sprintf( char *s, const char *format, ... )
{
	va_list vl;
	va_start(vl, format);

	int n = vsprintf(s, format, vl );

	va_end(vl);

	return n;
}

int vsnprintf(char *s, size_t size, const char *format, va_list arg )
{
	size_t written_char = 0;

	if(format == NULL || size == 0)
	{
		return -1;
	}

	size_t n, precision, width;
	char chr;
	
	static char chrar[65];
	char *str = chrar;

	precision = 1;
	width 	  = 1;

	while(*format != '\0' && written_char < size)
	{
		str = chrar;

		if(*format == '%')
		{
			format++;

			if(*format == '.')
			{
					format++;

					if(*format == '*')
					{
						precision = va_arg(arg, int);
                                                format++;
					}
					else if(*format >= '0' && *format <= '9')
					{
						precision = atoi(format);
					
						while (*format >= '0' && *format <= '9') 
						{
							format++;
						}
					}
					else
					{
						precision = 1;
					}
			}
			else if(*format >= '0' && *format <= '9')
			{
				width = atoi(format);
					
				while (*format >= '0' && *format <= '9') 
				{
					format++;
				}
			}
			else if(*format == '*')
			{
				width = va_arg(arg, int);
                                format++;
			}
			
			switch(*format)
			{
				case 'd':
				case 'i':
					n = va_arg(arg, int);

					sputn(str, n, 10, precision);

					precision = 1;

					while(strlen(str) < width--)
					{
						*s = ' ';
						s++;
						written_char++;
					}

					width = 0;

					while(*str)
					{
						*s = *str;
						s++; str++;
						written_char++;
					}
				break;

				case 'u':
					n = va_arg(arg, unsigned int);

					sputn(str, n, 10, precision);

					precision = 1;

					while(strlen(str) < width--)
					{
						*s = ' ';
						s++;
						written_char++;
					}

					width = 0;

					while(*str)
					{
						*s = *str;
						s++; str++;
						written_char++;
					}
				break;
	
				case 'o':
					n = va_arg(arg, unsigned int);

					sputn(str, n, 8, precision);

					precision = 1;

					while(strlen(str) < width--)
					{
						*s = ' ';
						s++;
						written_char++;
					}

					width = 0;

					while(*str)
					{
						*s = *str;
						s++; str++;
						written_char++;
					}
				break;
			
				case 'x':
					n = va_arg(arg, unsigned int);

					sputn(str, n, 16, precision);

					precision = 1;

					while(strlen(str) < width--)
					{
						*s = ' ';
						s++;
						written_char++;
					}

					width = 0;

					while(*str)
					{
						*s = *str;
						s++; str++;
						written_char++;
					}
				break;

				case 'X':
					n = va_arg(arg, unsigned int);

					sputn(str, n, 16, precision);

					precision = 1;

					while(strlen(str) < width--)
					{
						*s = ' ';
						s++;
						written_char++;
					}

					width = 0;

					while(*str)
					{
						*s = toupper(*str);
						s++; str++;
						written_char++;
					}
				break;

				case 'f':
					//TODO
				break;
			
				case 'F':
					//TODO
				break;

				case 'e':
					//TODO
				break;

				case 'E':
					//TODO
				break;

				case 'a':
					//TODO
				break;

				case 'A':
					//TODO
				break;

				case 'c':
					chr = (char)va_arg(arg, int);
					*s = chr;
					s++;
					written_char++;
				break;

				case 's':
					str = va_arg(arg, char*);

					while(strlen(str) < width--)
					{
						*s = ' ';
						s++;
						written_char++;
					}

					width = 0;

					while(*str)
					{
						*s = *str;
						s++; str++;
						written_char++;
					}
				break;

				case 'p':
					n = (int)va_arg(arg, void*);

					sputn(str, n, 16, precision);

					precision = 1;

					while(strlen(str) < width--)
					{
						*s = ' ';
						s++;
						written_char++;
					}

					width = 0;

					while(*str)
					{
						*s = *str;
						s++; str++;
						written_char++;
					}
				break;

				case 'n':
					//TODO
				break;

				case '%':
					*s = '%';
					s++;
					written_char++;
				break;

				default:
					*s = *format;
					s++;
					written_char++;
				break;
			}
		}
		else
		{
			*s = *format;
			s++;
			written_char++;
		}

		format++;
	}

	*s = '\0';

	return written_char;
}

int vsprintf( char *s, const char *format, va_list arg )
{
	 return vsnprintf(s, (size_t)-1, format, arg);
}
