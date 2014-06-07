#ifndef NEW_HPP
#define NEW_HPP

#include "heap.hpp"

/**
  * @brief	Default allocation function; std-c++
  *
  * @param	size		Number of bytes to be allocated
  * @return			Pointer to allocated memory or null pointer if allocation failed
  */
void *operator new( size_t size )
{
	return kernel::calloc(1, size);
}

/**
  * @brief	Default allocation function (used for arrays); std-c++
  *
  * @param	size		Number of bytes to be allocated
  * @return			Pointer to allocated memory or null pointer if allocation failed
  */
void *operator new[]( size_t  size )
{
	return kernel::calloc(1, size);
}

/**
  * @brief	Default deallocation function; std-c++
  *
  * @param	ptr		Pointer to the memory block to be deallocated
  */
void operator delete( void *ptr )
{
	kernel::free(ptr);
}

/**
  * @brief	Default deallocation function (used for arrays); std-c++
  *
  * @param	ptr		Pointer to the memory block to be deallocated
  */
void operator delete[]( void *ptr )
{
	kernel::free(ptr);
}

#endif
