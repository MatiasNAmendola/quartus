#ifndef MAPPER_HPP
#define MAPPER_HPP

#include <cstdint>
#include <cstddef>
#include <cstdbool>

#include "memory.hpp"
#include "pmm.hpp"

#define PD_INDEX( virt_addr )	virt_addr / 4096 / 1024							
#define PT_INDEX( virt_addr )	virt_addr / 4096 % 1024

namespace hal
{
	namespace x86
	{	
		/**
		  * @brief	Class describing an x86 virtual memory context (paging)
		  */
		class context
		{
			public:
				/**
				  * @brief	Type of a page directory
				  */
				typedef uint32_t	pagedir_t;

				/**
				  * @brief	Type of a page table
				  */
				typedef uint32_t	pagetab_t;



				/**
				  * @brief	The constructor
				  */
				context(  );

				/**
				  * @brief	The destructor
				  */
				~context(  );

				/**
				  * @brief	Activates this context; Loads the page directory to cr3.
				  */
				void activate(  );

				/**
				  * @brief	Maps a virtual address to a physical
				  *
				  * @param	virt		Virtual address to map.
				  * @param	phys		Physical address to map to.
				  * @param	flags		Flags used to set the mapping. e.g the user-space flag
				  * @return			true if the mapping was set successfully, false otherwise.
				  */
				bool map( uintptr_t virt, uintptr_t phys, uint32_t flags );

				/**
				  * @brief	Maps a virtual address range to a physical
				  *
				  * @param	virt		First virtual address to map.
				  * @param	phys		First physical address to map to.
				  * @param	flags		Flags used to set the mapping. e.g the user-space flag
				  * @param	n		Number of pages to map
				  * @return			true if the mapping was set successfully, false otherwise.
				  */
				bool map( uintptr_t virt, uintptr_t phys, uint32_t flags, size_t n );

				/**
				  * @brief	Remove the mapping of a virtual address
				  *
				  * @param	virt		Virtual address to unmap.
				  * @return			true if the mapping was removed successfully, false otherwise.
				  */
				bool umap( uintptr_t virt );

				/**
				  * @brief	Remove the mapping of a virtual address range
				  *
				  * @param	virt		First virtual address to unmap
				  * @param	n		Number of Pages to unmap
				  * @return			true if the mapping was removed successfully, false otherwise.
				  */
				bool umap( uintptr_t virt, size_t n );

				/**
				  * @brief	Gets the physical address the virtual address is mapped to.
				  *
				  * @param	virt		The virtual adress
				  * @return			The physical address
				  */
				uintptr_t phys( uintptr_t virt );

				/**
				  * @brief	Gets the flags the virtual address is mapped with.
				  *
				  * @param	virt		The virtual adress
				  * @return			The flags
				  */
				uint32_t flags( uintptr_t virt );

				/**
				  * @brief	Makes a copy of the mapping from another context.
				  *
				  *		e.g. used for kernel-mappings
				  *		start address and number of pages should be page table aligned
				  *
				  * @param	src		The context the copy is made of
				  * @param	start		Address of the first mapping to copy
				  * @param	n		Number of page mappings to copy
				  * @return			true if the mapping was copied successfully, false otherwise.
				  */
				bool copy( context &src, uintptr_t start, size_t n );



				/**
				  * @brief	The page is present.
				  */
				static const uint32_t page_present 	= 0x01;

				/**
				  * @brief	The page is writable.
				  */
				static const uint32_t page_write	= 0x02;

				/**
				  * @brief	The page is accessible for user space.
				  */
				static const uint32_t page_user		= 0x04;

				/**
				  * @brief	Use Write-Through-Caching for the page.
				  */
				static const uint32_t page_wtcache	= 0x08;

				/**
				  * @brief	Do not use caching for the page.
				  */
				static const uint32_t page_nocache	= 0x10;

			private:
				/**
				  * @brief	The page directory of the context
				  */
				pagedir_t *pagedir;

				/**
				  * @brief	Size of a page directory
				  */
				static const size_t pagedir_size = 1024;

				/**
				  * @brief	Size of a page table
				  */
				static const size_t pagetab_size = 1024;

				/**
				  * @brief	The page directory will be mapped here
				  */
				static const uintptr_t pagedir_mapping 	   = 0xFFFFF000;

				/**
				  * @brief	The page tables will be mapped here
				  */
				static const uintptr_t pagetab_mapping 	   = 0xFFC00000;

				/**
				  * @brief 	Used to map an other page directory temporarily.
				  */
				static const uintptr_t tmp_pagedir_mapping = 0xFFBFE000;

				/**
				  * @brief 	Used to map an other page table temporarily.
				  */
				static const uintptr_t tmp_pagetab_mapping = 0xFFBFF000;
		};

		/**
		  * @brief	Namespace containig global information about contexts and mappings (paging)
		  */
		namespace mapper
		{
			/**
			  * @brief	Set true if paging has been activated.
			  */
			extern bool paging_activated;

			/**
			  * @brief	Used to store the current context.
			  *
			  *		The current context is the context whos page directory is stored in cr3 right now.
			  */
			extern context *current_context;
		}
	}
}

#endif
