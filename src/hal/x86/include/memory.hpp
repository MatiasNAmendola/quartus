#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <cstddef>
#include <cstdbool>

#define align4k(addr)	((addr) & (~0xFFF))

#define bytes_to_pages(bytes)	((bytes + memory::page_size_byte - 1) / memory::page_size_byte)

extern const void *kernel_start;
extern const void *kernel_end;

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Memory namespace
		  *	
		  *		Contains definitions and constants describing the memory layout.
		  */
		namespace memory
		{
			/**
			  * @brief	Use Physical Address Extension?
			  */
			const bool     pae		= false;



			/**
			  * @brief	Size of a memory-page in KiB
			  */
			const uint32_t page_size_kib	= 4;

			/**
			  * @brief	Size of a memory-page in byte
			  */
			const uint32_t page_size_byte 	= page_size_kib * 1024;



			/**
			  * @brief	Size of the Memory/Address-space in GiB
			  */
			const uint32_t memory_size_gib	= 4;

			/**
			  * @brief	Size of the Memory/Address-space in MiB
			  */
			const uint32_t memory_size_mib	= memory_size_gib * 1024;

			/**
			  * @brief	Size of the Memory/Address-space in KiB
			  */
			const uint32_t memory_size_kib 	= memory_size_mib * 1024;

			/**
			  * @brief	Size of the Memory/Address-space in byte
			  */
			const uint32_t memory_size_byte	= memory_size_kib * 1024;

			

			/**
			  * @brief	First address of the memory area useable for ISA DMA.
			  */
			const uintptr_t isa_dma_base	= 0x00001000;

			/**
			  * @brief	Last address of the memory area useable for ISA DMA.
			  */
			const uintptr_t isa_dma_limit	= 0x01000000;


			/**
			  * @brief	First address of the kernel
			  */
			const uintptr_t kernel_base 	= (uintptr_t)&kernel_start;

			/**
			  * @brief	Last address of the kernel
			  */
			const uintptr_t kernel_limit	= (uintptr_t)&kernel_end;

			/**
			  * @brief	The video memory.
			  */
			const uintptr_t videomem	= 0xB8000;
		}
	}
}

#endif
