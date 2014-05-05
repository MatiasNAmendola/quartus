#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include <definitions.hpp>

#include <cstdint>
#include <cstdbool>

#include "cpu.hpp"

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Namespace for interrupt-handling
		  *
		  *		Contains interrupt-handling definitions and functions.
		  *		e.g. interrupt-mappings (exceptions, irqs, ...); enable/disable interrupts
		  */
		namespace intr
		{
			/**
			  * @brief	Function-type of Interrupt-handlers		
			  */
			typedef cpu::cpu_state * (*handler_t)( cpu::cpu_state *cpu );

					

			/**
			  * @brief	Enables interrupts
			  */
			inline void enable(  )  { asm("sti"); }

			/**
			  * @brief	Disables interrupts
			  */
			inline void disable(  ) { asm("cli"); }


			/**
			  * @brief	Sets an interrupt handler
			  *
			  * @param	intr		Interrupt number
			  * @param	handler		Pointer to an interrupt handler function
			  */
			void handler( uint32_t intr, handler_t handler );

			/**
			  * @brief	Sets the handler for non-critical-exceptions
			  *
			  * @param	handler		Pointer to an interrupt handler function
			  */
			void ncex( handler_t handler );

			/**
			  * @brief	The exception handler
			  *
			  *		Prints an error message and dumps the cpu-state.
			  *
			  * @param	cpu		The cpu-state; contains information about the exception(number; error code)
			  * @return			If the exception is not critical, the function returns the cpu-state, otherwise the function never returns.
			  */
			cpu::cpu_state *handle_exception( cpu::cpu_state *cpu );

			/**
			  * @brief	The global interrupt handler
			  *
			  *		Handles ALL interrupts; If the interrupt is an exception, the exception-handler is called
					If the interrupt is a 'normal' interrupt and a handler is registerd, the handler is called,
			  *		otherwise the exception handler is called.
			  *
			  * @param	cpu		The cpu-state; contains information about the interrupt(number)
			  */
			cpu::cpu_state *handle_interrupt( cpu::cpu_state *cpu );



			/**
			  * @brief	The null-handler; Used to indicate that no handler was set or to reset an Interrupt-handler.
			  */
			constexpr const handler_t null_handler = nullptr;


			/**
			  * @brief	The number of interrupts
			  */
			const size_t intr_count		= 256;


			/**
			  * @brief	Number of the first exception
			  */
			const uint32_t exception_base 	= 0x00;

			/**
			  *@brief	Number of the last exception
			  */
			const uint32_t exception_limit 	= 0x2F;

			/**
			  * @brief	Number of the first IRQ (IRQ 0;Timer)
			  */
			const uint32_t irq_base 	= 0x30;

			/**
			  * @brief	Number of the last IRQ (IRQ 15; sec. ATA)
			  */
			const uint32_t irq_limit 	= 0x3F;			
		}
	}
}

#endif
