#ifndef PMM_H
#define PMM_H

#include "kernel.hpp"

#define addr(x,y)		(uintptr_t)(( (x) * 32 + (y) ) * 4096)
#define bitmap_px(addr)		((uintptr_t)addr / 4096 / 32)
#define bitmap_py(addr)		((uintptr_t)addr / 4096 % 32)

namespace kernel
{
	/**
	  * @brief	The Physical Memory Manager.
	  *
	  *		This class contains Methods to initialiset the Physical Memory Manager and Mathods to allocate and free memory.
	  *		This class is implemented as a singleton, so only one instance of this class can be used.
	  */
	class pmm
	{
		SINGLETON(pmm)

		public:
			/**
			  * @brief	Structure containing information about used/free memory.
			  *
			  *		Used to count available memory.
			  */
			struct meminfo_t
			{
				size_t	free;
				size_t	used;
			} PACKED;

			
			/**
			  * @brief	Type used for error checking of. Used as return of the 'init'-Method
			  */
			enum error
			{
				/**
				  * @brief	Indicates that the pmm was initialised successfully.
				  */
				init_ok			= 0x0,


				/**
				  * @brief	Indicates that the pmm was not initialised successfully, the address of the kernel is missing.
				  */
				no_kernel_address	= 0x1,


				/**
				  * @brief	Indicates that the pmm was not initialised successfully, the memory map is missing.
				  */
				no_memory_map		= 0x2
			};



			/**
			  * @brief	Initialises the Physical Memory Manager.
			  */
			error init(  );



			/**
			  * @brief	Allocates one page of memory.
			  *
			  * @return			The address of the allocated page, or 0 if no free page was available
			  */
			uintptr_t alloc(  );

			/**
			  * @brief	Allocates pages of memory.
			  *
			  * @param	n		Number of pages to allocate
			  * @return			The address of the first allocated page, or 0 if no free page was available
			  */
			uintptr_t alloc( size_t n );

			/**
			  * @brief	Allocates pages of memory.
			  *
			  * @param	n		Number of pages to allocate
			  * @param	flags		Flags used for the allocation; e.g. 'isa_dma' to allocate memory suitable for Direct Memory Access
			  * @return			The address of the first allocated page, or 0 if no free page was available
			  */
			uintptr_t alloc( size_t n, uint32_t flags );

			/**
			  * @brief	Frees one page of memory
			  *
			  * @param	addr		The address of the page to be freed
			  */
			void free( uintptr_t addr );

			/**
			  * @brief	Frees pages of memory
			  *
			  * @param	addr		The address of the first page to be freed
			  * @param	n		Number of pages to free
			  */
			void free( uintptr_t addr, size_t n );

			/**
			  * @brief	Checks one page of memory
			  *
			  * @param	addr		The address of the page to be checked
			  * @return			true if the page is unused, flase otherwise
			  */
			bool check( uintptr_t addr );

			/**
			  * @brief	Checks pages of memory
			  *
			  * @param	addr		The address of the first page to be checked
			  * @param	n		Number of pages to check
			  * @return			true if all pages are unused, flase otherwise
			  */
			bool check( uintptr_t addr, size_t n );



			/**
			  * @brief	Flag used to allocate memory suitable for Direct Memory Access
			  */
			static const uint32_t isa_dma 		= 0x1;
			
			/**
			  * @brief	Size of the pmm-bitmap
			  */
			static const uint32_t bitmap_size 	= memory::memory_size_kib / memory::page_size_kib / 32;

		private:
			/**
			  * @brief	Bitmap used for managing the Physical memory.
			  */
			uint32_t 	bitmap[bitmap_size];

	
			/**
			  * @brief	Used to store the last freed address; allows faster allocation of single pages.
			  */
			uintptr_t	fast_access;
	};	
}

#endif
