#ifndef CPU_HPP
#define CPU_HPP

#include <definitions.hpp>

#include <cstdint>
#include <cstddef>
#include <cstdbool>

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Namespace of the cpu.
		  *
		  *		Contains all cpu specific stuff.
		  *		e.g. a structure defining a cpu-state, a method to dump the cpu-state
		  */
		namespace cpu
		{
			/**
			  * @brief	Structure defining a cpu-state.
			  *
			  *		Contains registers of the cpu.
			  *		This architecture is a 32-bit architecture, so all fields have a size of 32 bits.
			  *		Used by interrupt handlers to pass information (interrupt number, error code, ...)
			  */
			struct cpu_state
			{
				/**
				  * @brief	The eax-register of x86-cpu
				  */
				uint32_t   eax;

				/**
				  * @brief	The ebx-register of x86-cpu
				  */
				uint32_t   ebx;

				/**
				  * @brief	The ecx-register of x86-cpu
				  */
				uint32_t   ecx;

				/**
				  * @brief	The edx-register of x86-cpu
				  */
				uint32_t   edx;

				/**
				  * @brief	The esi-register of x86-cpu
				  */
				uint32_t   esi;

				/**
				  * @brief	The edi-register of x86-cpu
				  */
				uint32_t   edi;

				/**
				  * @brief	The ebp-register of x86-cpu
				  */
				uint32_t   ebp;

				/**
				  * @brief	The interrupt number
				  */
				uint32_t   intr;
		
				/**
				  * @brief	The error code; pushed to stack by some exceptions
				  */
				uint32_t   error;

				/**
				  * @brief	The eip-register of x86-cpu; used as instruction-pointer
				  */
				uint32_t   eip;

				/**
				  * @brief	The cs-register of x86-cpu
				  */
				uint32_t   cs;

				/**
				  * @brief	The eflags-register of x86-cpu
				  */
				uint32_t   eflags;

				/**
				  * @brief	The esp-register of x86-cpu; used as stack-pointer
				  */
				uint32_t   esp;

				/**
				  * @brief	The ss-register of x86-cpu
				  */
				uint32_t   ss;	

				inline uint32_t &param0(  ) { return this->eax; }
				inline uint32_t &param1(  ) { return this->ebx; }			
				inline uint32_t &param2(  ) { return this->ecx; }			
				inline uint32_t &param3(  ) { return this->edx; }			
				inline uint32_t &param4(  ) { return this->esi; }
				inline uint32_t &param5(  ) { return this->edi; }
				inline uint32_t &param6(  ) { return this->ebp; }	
				inline uint32_t &syscall_number(  ) { return this->eax; }
			} /*PACKED*/;

			/**
			  * @brief	Stops the cpu (the whole system)
			  *
			  *		Disables interrups and stops the cpu.
			  */
			inline void halt(  ) { while(true) { asm("cli; hlt"); } }

			/**
			  * @brief	Prints the contents of the 'cpu_state'-structure to the standard output.
			  */
			void dump( cpu_state *cpu );

			/**
			  * @brief	Executes the 'cpuid' command (if supported by cpu=).
			  *
			  * @param	func		cpuid-function to be executed
			  * @param	eax		Pointer to 32-bit integer; used to pass the value stored into the eax-register by the cpu
			  * @param	ebx		Pointer to 32-bit integer; used to pass the value stored into the ebx-register by the cpu
			  * @param	ecx		Pointer to 32-bit integer; used to pass the value stored into the ecx-register by the cpu
			  * @param	edx		Pointer to 32-bit integer; used to pass the value stored into the edx-register by the cpu
			  * @return			true, if 'cpuid' is supported and has been executed, flase otherwise
			  */	
			bool cpuid( uint32_t func, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx );
		}
	}
}

#endif
