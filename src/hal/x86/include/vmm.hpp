#ifndef VMM_HPP
#define VMM_HPP

#include <cstdint>
#include <cstddef>
#include <cstdbool>

#include "memory.hpp"
#include "mapper.hpp"

#define addr(x,y)		(uintptr_t)(( (x) * 32 + (y) ) * 4096)
#define bitmap_px(addr)		((uintptr_t)addr / 4096 / 32)
#define bitmap_py(addr)		((uintptr_t)addr / 4096 % 32)

#define set_bit(bitmap,bit)	((bitmap) |= (1 << (bit)))
#define clear_bit(bitmap,bit)	((bitmap) ^= (1 << (bit)))


namespace hal
{
	namespace x86
	{
		 /**
	          * @brief	The Virtual Memory Manager.
	          *
	          *		This class contains Methods to initialiset the Virtual Memory Manager and Mathods to allocate and free memory.
	          */
		class vmm
		{
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
			          * @brief	Type used for error checking. Used as return of the 'init'-Method
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
				        no_kernel_addr		= 0x1,
			        };




				/**
				  * @brief	The constructor
				  */
				vmm(  );

				/**
				  * @brief	The destructor
				  */
				~vmm(  );

				/**
			          * @brief	Initialises the Virtual Memory Manager.
			          */
				error init( context *cntxt, uintptr_t base, uintptr_t limit );

				/**
			          * @brief	Allocates one page of memory.
			          *
				  * @param	flags		Flags used for the mapping of the allocated page
			          * @return			The address of the allocated page, or 0 if no free page was available
			          */
			        uintptr_t alloc( uint32_t flags );

			        /**
			          * @brief	Allocates pages of memory.
			          *
				  * @param	flags		Flags used for the mapping of the allocated page
			          * @param	n		Number of pages to allocate
			          * @return			The address of the first allocated page, or 0 if no free page was available
			          */
			        uintptr_t alloc( uint32_t flags, size_t n );

				/**
			          * @brief	Allocates one page of memory without mapping to physical memory.
			          *
				  * @param	flags		Flags used for the mapping of the allocated page
			          * @return			The address of the allocated page, or 0 if no free page was available
			          */
			        uintptr_t alloc_vonly(  );

			        /**
			          * @brief	Allocates pages of memory without mapping to physical memory.
			          *
				  * @param	flags		Flags used for the mapping of the allocated page
			          * @param	n		Number of pages to allocate
			          * @return			The address of the first allocated page, or 0 if no free page was available
			          */
			        uintptr_t alloc_vonly( size_t n );

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
				  * @brief	Maps a virtual address to a physical and marks the address as used
				  *
				  * @param	virt		Virtual address to map.
				  * @param	phys		Physical address to map to.
				  * @param	flags		Flags used to set the mapping. e.g the user-space flag
				  * @return			true if the mapping was set successfully, false otherwise.
				  */
				bool map( uintptr_t virt, uintptr_t phys, uint32_t flags );
			
				/**
				  * @brief	Maps a virtual address range to a physical and marks the address range as used
				  *
				  * @param	virt		First virtual address to map.
				  * @param	phys		First physical address to map to.
				  * @param	flags		Flags used to set the mapping. e.g the user-space flag
				  * @param	n		Number of pages to map
				  * @return			true if the mapping was set successfully, false otherwise.
				  */
				bool map( uintptr_t virt, uintptr_t phys, uint32_t flags, size_t n );

				/**
				  * @brief	Remove the mapping of a virtual address and marks the address as free
				  *
				  * @param	virt		Virtual address to unmap.
				  * @return			true if the mapping was removed successfully, false otherwise.
				  */
				bool umap( uintptr_t virt );

				/**
				  * @brief	Remove the mapping of a virtual address range and marks the address range as free
				  *
				  * @param	virt		First virtual address to unmap
				  * @param	n		Number of Pages to unmap
				  * @return			true if the mapping was removed successfully, false otherwise.
				  */
				bool umap( uintptr_t virt, size_t n );
	
			        /**
			          * @brief	Checks one page of memory
			          *
			          * @param	addr		The address of the page to be checked
			          * @return			true if the page is unused, false otherwise
			          */
			        bool check( uintptr_t addr );

			        /**
			          * @brief	Checks pages of memory
			          *
			          * @param	addr		The address of the first page to be checked
			          * @param	n		Number of pages to check
			          * @return			true if all pages are unused, false otherwise
			          */
			        bool check( uintptr_t addr, size_t n );

				/**
				  * @brief	Gets the memory context.
				  *
				  * @return			The memory context
				  */			
				context *get_context(  );



				/**
				  * @brief	First address of the kernel-space
				  */
				static const uintptr_t kernel_space_base 	= 0x00001000;

				/**
				  * @brief	Last address of the kernel-space
				  */
				static const uintptr_t kernel_space_limit	= 0x03FFFFFF;

				/**
				  * @brief	First address of the user-space
				  */
				static const uintptr_t user_space_base		= 0x04000000;

				/**
				  * @brief	Last address of the user-space
				  */
				static const uintptr_t user_space_limit		= 0xFFBFDFFF;

				/**
				  * @brief	First address of the memory-region where the paging structures are mapped
				  */
				static const uintptr_t struct_space_base	= 0xFFBFE000;

				/**
				  * @brief	Last address of the memory-region where the paging structures are mapped
				  */
				static const uintptr_t struct_space_limit	= 0xFFFFFFFF;

		

				/**
				  * @brief	Flag used to allocate writable memory
				  */
				static const uint32_t write	= context::page_write;

				/**
				  * @brief	The allocated page/pages is/are present.
				  */
				static const uint32_t present 	= context::page_present;

				/**
				  * @brief	Flag used to allocate memory for kernel-space.
				  */
				static const uint32_t kernel	= 0x00;

				/**
				  * @brief	Flag used to allocate memory for user-space.
				  */
				static const uint32_t user	= context::page_user;

				/**
				  * @brief	Flag used to allocate memory with Write-Through-Caching.
				  */
				static const uint32_t wtcache	= context::page_wtcache;

				/**
				  * @brief	Flag used to allocate memory without caching.
				  */
				static const uint32_t nocache	= context::page_nocache;


				/**
			          * @brief	Size of the vmm-bitmap
			          */
			        static const uint32_t bitmap_size 	= memory::memory_size_kib / memory::page_size_kib / 32;
				
			private:
				/**
				  * @brief	The memory context
				  */
				context 	*cntxt;

				/**
				  * @brief	Base address of the address-space managed by the memory manager
				  */
				uintptr_t	addr_space_base;

				/**
				  * @brief	Limit address of the address-space managed by the memory manager
				  */
				uintptr_t	addr_space_limit;

				/**
			          * @brief	Bitmap used for managing the Virtual memory.
			          */
				uint32_t	bitmap[bitmap_size];

				/**
				  * @brief	Used to store the last freed address; allows faster allocation of single pages.
				  */
				uintptr_t	fast_access	= 0x0;

				/**
				  * @brief	Used to speed up memory allocation.
				  */
				size_t		speedup_x	= 0;
		};
	}
}

#endif
