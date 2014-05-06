#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <cstddef>
#include <cstdbool>

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
		}
	}
}

#endif
