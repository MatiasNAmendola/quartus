#ifndef X86_HPP
#define X86_HPP

#include "ioports.hpp"
#include "cpu.hpp"
#include "gdt.hpp"
#include "idt.hpp"
#include "pic.hpp"
#include "interrupts.hpp"
#include "video.hpp"
#include "memory.hpp"
#include "pmm.hpp"

namespace hal
{
	/**
	  * @brief	Namespace of the x86 architecture.
	  *
	  *		This namespace contains all x86 specific stuff.
	  *		e.g. Interrupt-handling, address-space size, x86 specific Hardware and structures
	  */
	namespace x86
	{
		/**
		  * @brief	A string, representing the architecture (x86).
		  */
		extern const char *arch_string;
	}
}

#endif
