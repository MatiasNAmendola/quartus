#ifndef PIC_HPP
#define PIC_HPP

#include <definitions.hpp>

#include <cstdint>
#include <cstddef>
#include <cstdbool>

#include "cpu.hpp"
#include "interrupts.hpp"

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Class representing the Programmable Interrupt Controller.
		  *
		  *		This class contains Methods to initialiset the Programmable Interrupt Controller.
		  *		This class is implemented as a singleton, so only one instance of this class can be used (it is not possible to set up more than one PIC).
		  */
		class pic
		{
			SINGLETON(pic)

			public:
				/**
				  * @brief	Function-type of IRQ-handlers		
				  */
				typedef cpu::cpu_state * (*handler_t)( cpu::cpu_state *cpu );



				/**
				  * @brief	Initialises the Programmable Interrupt Controller.
				  *
				  *		Remaps the IRQs; Sets up the default handler for IRQs.
				  */
				void init(  );

				/**
				  * @brief	Gets the IRQ-mask.
				  *
				  * @return			IRQ-mask
				  */
				uint16_t mask(  );

				/**
				  * @brief	Sets an IRQ-mask.
				  *
				  * @param	mask		IRQ-mask to set
				  * @return			IRQ-mask before call
				  */	
				uint16_t mask( uint16_t mask );

				/**
				  * @brief	Enables an IRQ
				  *
				  * @param	irq		IRQ to enable
				  */
				void enable( uint8_t irq );

				/**
				  * @brief	Disables an IRQ
				  *
				  * @param	irq		IRQ to disable
				  */
				void disable( uint8_t irq );

				/**
				  * @brief	Sends the End of Interrupt signal to the PIC
				  *
				  * @param	irq		IRQ number
				  */
				void eoi( uint8_t irq );
		
				/**
				  * @brief	Sets a handler for an IRQ
				  *
				  * @param	irq		IRQ number
				  * @param	handler		Pointer to a function used as IRQ-handler
				  */
				void handler( uint8_t irq, handler_t handler );



				/**
				  * @brief	The null-handler; Used to indicate that no handler was set or to reset an IRQ-handler.
				  */
				constexpr static const handler_t null_handler = nullptr;	

				/**
				  * @brief	Number of IRQs
				  */
				static const size_t	irq_count = 16;

				/**
				  * @brief	Number of the first IRQ (IRQ 0;Timer)
				  */
				static const uint32_t	irq_base  = intr::irq_base;

				/**
				  * @brief	Number of the last IRQ (IRQ 15; sec. ATA)
				  */
				static const uint32_t	irq_limit = intr::irq_limit;

				/**
				  * @brief	The Command-port of the first PIC
				  */
				static const uint16_t	pic1_cmd  = 0x20;

				/**
				  * @brief	The Data-port of the first PIC
				  */
				static const uint16_t	pic1_data = 0x21;

				/**
				  * @brief	The Command-port of the second PIC
				  */
				static const uint16_t	pic2_cmd  = 0xA0;

				/**
				  * @brief	The Data-port of the second PIC
				  */
				static const uint16_t	pic2_data = 0xA1;

			private:
				/**
				  * @brief	Array containing the IRQ-handlers.
				  */
				handler_t handlers[irq_count];

				/**
				  * @brief	Handler for all IRQs
				  *
				  * @param	cpu		The cpu-state; used to identify the Interrupt(IRQ)
				  * @return			Returns an cpu-state to restore.
				  */
				cpu::cpu_state *irq( cpu::cpu_state *cpu );
		};
	}
}

#endif
