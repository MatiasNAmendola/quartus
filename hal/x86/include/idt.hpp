#ifndef IDT_HPP
#define IDT_HPP

#include <cstdint>
#include <cstddef>

#include <definitions.hpp>

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Class representing the Interrupt Descriptor Table.
		  *
		  *		This class contains Methods to initialise and load the Interrupt Descriptor Table.
		  *		It also contains the Interrupt Descriptor Table itself.
		  *		This class is implemented as a singleton, so only one instance of this class can be used (it is not possible to set up more than one IDT).
		  */
		class idt
		{
			SINGLETON(idt)

			private:
				/**
				  * @brief	Structure used to set up a descriptor within the IDT.
				  *
				  * @todo	Use for IDT-entries
				  */
				struct descriptor_t
				{
					/**
					  * @brief	Bits 0-15 of the offset of the ISR (offset within the segment)
					  */
					uint16_t	offset1;
					
					/**
					  * @brief	The selector of a codesegment
					  */
					uint16_t	selector;
					
					/**
					  * @brief	Reserved; not used
					  */
					uint8_t		reserved;

					/**
					  * @brief	Information about the interrupt (type, privilege level, ...)
					  */
					uint8_t		flags;

					/**
					  * @brief	Bits 16-31 of the offset of the ISR (offset within the segment)
					  */
					uint16_t	offset2;
				} PACKED;

			public:
				/**
				  * @brief	The destructor
				  */
				~idt(  );

				/**
				  * @brief	Initialises the Interrupt Descriptor Table.
				  *
				  *		Sets descriptors for all interrupts:
				  */
				void init(  );

				/**
				  * @brief	Loads the Interrupt Descriptor Table
				  *
				  */
				void load(  );

				/**
				  * @brief	Number of descriptors.
				  */
				static const size_t descriptor_count 	= 256;
			
			private:
				/**
				  * @brief	The 'real' Interrupt Descriptor Table
				  *
				  *		This memory-structure is filled by the 'set_entry'-method and is loaded by the 'load'-method.
				  */
				uint64_t descriptor_table[descriptor_count];

				/**
				  * @brief	Sets up a new entry within the GDT.
				  *
				  * @param	index		The index of the new entry within the IDT
				  * @param	fn		Pointer to the Interrupt Service Routine
				  * @param	selector	The selector of a codesegment to switch to
				  * @param	flags		The flags of the new entry
				  */
				void set_entry( size_t index, void (*fn)(), uint32_t selector, uint32_t flags );
		};
	}
}

#endif
