#ifndef GDT_HPP
#define GDT_HPP

#include <cstdint>
#include <cstddef>

#include <definitions.hpp>

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Class representing the Global Descriptor Table.
		  *
		  *		This class contains Methods to initialise and load the Global Descriptor Table.
		  *		It also contains the Global Descriptor Table itself.
		  *		This class is implemented as a singleton, so only one instance of this class can be used (it is not possible to set up more than one GDT).
		  */
		class gdt
		{
			SINGLETON(gdt)

			public:
				/**
				  * @brief	Structure used to set up a Task State Segment.
				  *
				  * @todo	Implement
				  */
				struct tss_t
				{

				} PACKED;

			private:
				/**
				  * @brief	Structure used to set up a descriptor within the GDT.
				  *
				  * @todo	Use for GDT-entries
				  */
				struct descriptor_t
				{
					/**
					  * @brief	Bits 0-15 of the segment-limit
					  */
					uint16_t	limit;
					
					/**
					  * @brief	Bits 0-15 of the segment-base-address
					  */
					uint16_t	base0;
					
					/**
					  * @brief	Bits 16-23 of the segment-base-address
					  */
					uint8_t		base1;

					/**
					  * @brief	Access information of the segment (Ring, executable, ...)
					  */
					uint8_t		access;

					/**
					  * @brief	Bits 16-19 of the segment-limit; bits 0-3 of the flags
					  */
					uint8_t		limit_flags;

					/**
					  * @brief	Bits 24-31 of the segment-base-address
					  */
					uint8_t		base2;
				} PACKED;

			public:
				/**
				  * @brief	The destructor
				  */
				~gdt(  );

				/**
				  * @brief	Initialises the Global Descriptor Table.
				  *
				  *		Sets up following descriptors:
				  *			- nulldescriptor
				  *			- codesegment-kernel
				  *			- datasegment-kernel
				  *			- codesegment-user
				  *			- datasegment-user
				  *			- task state segment for multitasking
				  */
				void init(  );

				/**
				  * @brief	Loads the Global Descriptor Table
				  *
				  *		Loads th GDTR with a pointer to the GDT.
				  */
				void load(  );



				/**
				  * @brief	Number of descriptors.
				  *	
				  *		1 nulldescriptor + 2 descriptors for kernel + 2 for user + 1 tss
				  */
				static const size_t descriptor_count 	= 1 + 2 + 2 + 1;

				/**
				  * @brief	Size of the Task State Segment
				  */
				static const size_t tss_size		= 32;




				/**
				  * @brief	GDT-flag datasegment
				  */
				static const uint32_t dataseg 		= 0x02;

				/**
				  * @brief	GDT-flag codesegment
				  */
				static const uint32_t codeseg 		= 0x0A;

				/**
				  * @brief	GDT-flag tss
				  */
				static const uint32_t tss386		= 0x09;

				/**
				  * @brief	GDT-flag segment
				  */
				static const uint32_t segment		= 0x10;

				/**
				  * @brief	GDT-flag ring0 - kernel mode
				  */
				static const uint32_t ring0		= 0x00;

				/**
				  * @brief	GDT-flag ring1 - not used
				  */
				static const uint32_t ring1		= 0x20;

				/**
				  * @brief	GDT-flag ring2 - not used
				  */
				static const uint32_t ring2		= 0x40;

				/**
				  * @brief	GDT-flag ring3 - user mode
				  */
				static const uint32_t ring3		= 0x60;
		
				/**
				  * @brief	GDT-flag present
				  */
				static const uint32_t present		= 0x80;

				/**
				  * @brief	GDT-flag use a granularity of 4 KiB for the 'limit' field
				  */
				static const uint32_t gran4k		= 0x800;

				/**
				  * @brief	GDT-flag use 32-bit Protected Mode
				  */
				static const uint32_t bit32		= 0x400;
			
			private:
				/**
				  * @brief	The 'real' Global Descriptor Table
				  *
				  *		This memory-structure is filled by the 'set_entry'-method and is loaded by the 'load'-method.
				  */
				uint64_t descriptor_table[descriptor_count];

				/**
				  * @brief	The Task State Segment
				  *
				  *		This memory-structure is used as the Task State Segment.
				  */
				uint32_t tss[tss_size];

				/**
				  * @brief	Sets up a new entry within the GDT.
				  *
				  * @param	index		The index of the new entry within the GDT
				  * @param	base		The base-address of the new segment
				  * @param	limit		The limit of the new segment
				  * @param	flags		The flags of the new entry
				  */
				void set_entry( size_t index, uintptr_t base, size_t limit, uint32_t flags );
		};
	}
}

#endif
