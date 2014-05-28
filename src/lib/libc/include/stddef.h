#ifndef STDDEF_H
#define STDDEF_H

#ifndef NULL
	#define NULL ((void*) 0)
#endif

#define offsetof(TYPE, MEMBER) ((size_t)((char*)&((TYPE *)(0))->MEMBER))

typedef __PTRDIFF_TYPE__	ptrdiff_t;
typedef __SIZE_TYPE__		size_t;

#endif
