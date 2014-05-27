#ifndef HEAP_HPP
#define HEAP_HPP

#include <cstdint>
#include <cstddef>
#include <cstdbool>

#include "memmgr.hpp"

namespace kernel
{
	/**
	  * @brief	The Heap Memory Manager of the kernel
	  */
	class heap
	{
		friend void *kernel::realloc( void *ptr, size_t size );

		private:
			/**
			  * @brief	Structure of a heap item describing a block of memory.
			  */
			struct item
			{
				/**
				  * @brief	Flags of the item (used/free)
				  */
				size_t		flags;

				/**
				  * @brief	Size of the memory block
				  */
				size_t		size;

				/**
				  * @brief	Pointer to the next item
				  */
				item		*next;

				/**
				  * @brief	Pointer to the previous item
				  */
				item		*prev;
			} PACKED;

		public:
			/**
			  * @brief	The constructor
			  */
			heap(  );

			/**
			  * @brief	The destructor
			  */
			~heap(  );			
		
			/**
			  * @brief	Allocates a block of size bytes of memory
			  *
			  * @param	size		Number of Bytes to be allocated
			  * @return			Pointer to allocated memory or null pointer if allocation failed
			  */
			void *alloc( size_t size );

			/**
			  * @brief	Deallocates a block of memory
			  *
			  * @param	ptr		Pointer to a memory block
			  */
			void free( void *ptr );

		private:
			/**
			  * @brief	Inserts a new item into the heap.
			  *
			  * @param	item		Item to insert
			  */	
			void insert( item *item );

			/**
			  * @brief	Removs an item from the heap.
			  *
			  * @param	itm		Item to remove
			  */
			void remove( item *itm );

			/**
			  * @brief	Splits an item
			  *
			  * @param	itm		Item to be splited
			  * @param	part_1		The first part of the splitted item; 1/ratio of the size of the item
			  * @param	part_2		The second part of the splitted item
			  * @param	ratio		Ratio to split; (e.g. 4 = 1/4 8 = 1/8)
			  */
			void split( item *itm, item **part_1, item **part_2, size_t ratio );

			/**
			  * @brief	Enlarges the heap
			  *
			  * @param	size		Number of bytes
			  */
			void enlarge( size_t size );

			/**
			  * @brief	The first item of the heap
			  *
			  *		The items of the heap are organised with a liked list.
			  */
			item			*first		= nullptr;

			/**
			  * @brief	Flag indicating a used memor block
			  */
			static const size_t 	flag_used	= 0x00;

			/**
			  * @brief	Flag indicating a free memor block
			  */
			static const size_t 	flag_free	= 0x01;
	};

	/**
	  * @brief	The Heap Memory Manager of the kernel
	  */
	extern heap hpmmgr;

	/**
	  * @brief	Allocates a block of memory; std-c
	  *
	  * @param	size		Number of Bytes to be allocated
	  * @return			Pointer to allocated memory or null pointer if allocation failed
	  */
	void *malloc( size_t size );

	/**
	  * @brief	Allocates a block of memory for an array of and initializes all its bits to zero; std-c
	  *
	  * @param	n		Number of elements
	  * @param	size		Size of each element
	  * @return			Pointer to allocated memory or null pointer if allocation failed
	  */
	void *calloc( size_t n, size_t size );
	
	/**
	  * @brief	Changes the size of the memory block pointed to by ptr; std-c
	  *
	  * @param	ptr		Pointer to the memory block
	  * @param	size		New size of the memory block
	  * @return			Pointer to the new memory block
	  */
	void *realloc( void *ptr, size_t size );

	/**
	  * @brief	Deallocates a block of memory; std-c
	  *
	  * @param	ptr		Pointer to a memory block
	  */
	void free( void *ptr );
}

#endif
