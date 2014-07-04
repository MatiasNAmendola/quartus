#ifndef ELF_HPP
#define ELF_HPP

#include <cstdint>
#include <cstddef>
#include <cstdbool>

#include "process.hpp"

namespace kernel
{
	/**
	  * @brief	Basic implementation of the elf file format
	  */
	class elf
	{
		public:

		private:
			/**
			  * @brief	Structure of the ELF header
			  */
			struct header_t
			{
				uint8_t 		ident[16];
				uint16_t		type;
				uint16_t		machine;
				uint32_t		version;
				uint32_t		entry;
				uint32_t		phoff;
				uint32_t		shoff;
				uint32_t		flags;
				uint16_t		ehsize;
				uint16_t		phentsize;
				uint16_t		phnum;
				uint16_t		shentsize;
				uint16_t		shnum;
				uint16_t		shstrndex;	
			};

			/**
			  * @brief	Structure of the ELF program header
			  */
			struct program_header_t
			{
				uint32_t		type;
				uint32_t		offset;
				uint32_t		vaddr;
				uint32_t		paddr;
				uint32_t		filesz;
				uint32_t		memsz;
				uint32_t		flags;
				uint32_t		align;
			};

			/**
			  * @brief	Magic string of the ELF format
			  */
			const uint8_t			magic[4] 	= { 0x7F, 'E', 'L', 'F' };

			/**
			  * @brief	Class of the ELF-file - no/invalid class
			  */
			static const uint8_t		class_none	= 0;

			/**
			  * @brief	Class of the ELF-file - 32-bit-object
			  */
			static const uint8_t		class_32	= 1;

			/**
			  * @brief	Class of the ELF-file - 64-bit-object
			  */
			static const uint8_t		class_64	= 2;

			/**
			  * @brief	Data-format of the ELF-file - no/invalid format
			  */
			static const uint8_t		data_none	= 0;
	
			/**
			  * @brief	Data-format of the ELF-file - Little-Endian 
			  */
			static const uint8_t		data_2lsb	= 1;

			/**
			  * @brief	Data-format of the ELF-file - Big-Endian 
			  */
			static const uint8_t		data_2msb	= 2;

			/**
			  * @brief	Type of the ELF-file - no type
			  */
			static const uint16_t		type_none	= 0;

			/**
			  * @brief	Type of the ELF-file - relocatable file
			  */
			static const uint16_t		type_rel	= 1;

			/**
			  * @brief	Type of the ELF-file - executeable file
			  */
			static const uint16_t		type_exec	= 2;

			/**
			  * @brief	Type of the ELF-file - shared-object file
			  */
			static const uint16_t		type_dyn	= 3;

			/**
			  * @brief	Type of the ELF-file - ???
			  */
			static const uint16_t		type_core	= 4;

			/**
			  * @brief	Processor typ of the ELF-file - Intel 80386 
			  */
			static const uint16_t		machine_386	= 3;

			/**
			  * @brief	Type of the program header - type load
			  */
			static const uint32_t		ph_load		= 1;
		public:
			/**
			  * @brief	The constructor
			  *
			  * @param	addr		The address of the elf-image
			  * @param	size		The size of the elf-image
			  */
			elf( uintptr_t addr, size_t size );

			/**
			  * @brief	Check the elf
			  *
			  * @return 			Returns true if the image is a valid ELF-file; false otherwise
			  */
			bool check(  );
			
			/**
			  * @brief	Loads a process out of the ELF-file
			  *
			  * @return 			Returns a pointer to the new process or a nullpointer if the operation failed
			  */
			process *load_process( const char name[64], const char cmdline[1024], process::id_t parent );

			/**
			  * @brief	If the ELF is an executeable file, get the entry point of process main thread
			  *
			  * @return			The entry point or a nullpointer
			  */
			uintptr_t entry(  );

		private:
			/**
			  * @brief	The address of the elf-image
			  */
			uintptr_t	image;
		
			/**
			  * @brief	The size of the elf-image
			  */
			size_t		size;
	};	
}

#endif
