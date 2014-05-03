#ifndef STDINT_H
#define STDINT_H

typedef char 			int8_t;
typedef unsigned char 		uint8_t;

typedef char			int_least8_t;
typedef	unsigned char		uint_least8_t;

typedef int_least8_t		int_fast8_t;
typedef	int_least8_t		uint_fast8_t;



typedef short 			int16_t;
typedef unsigned short 		uint16_t;

typedef short			int_least16_t;
typedef	unsigned short		uint_least16_t;

typedef int_least16_t		int_fast16_t;
typedef	int_least16_t		uint_fast16_t;



typedef int 			int32_t;
typedef unsigned int 		uint32_t;

typedef int			int_least32_t;
typedef	unsigned int		uint_least32_t;

typedef int_least32_t		int_fast32_t;
typedef	int_least32_t		uint_fast32_t;



typedef long long 		int64_t;
typedef unsigned long long 	uint64_t;

typedef long long		int_least64_t;
typedef	unsigned long long	uint_least64_t;

typedef int_least64_t		int_fast64_t;
typedef	int_least64_t		uint_fast64_t;



typedef int32_t			intptr_t;
typedef uint32_t 		uintptr_t;
 
#endif
