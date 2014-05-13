#ifndef RPI_HPP
#define RPI_HPP

#include "serial.hpp"
#include "memory.hpp"
#include "mmio.hpp"
#include "gpio.hpp"
#include "delay.hpp"

namespace hal
{
	/**
	  * @brief	Namespace of the rpi architecture. (Raspberry Pi Model)
	  *
	  *		This is not really a architecture, its a whole hardware platform. The Architecture is ARMv6.
	  *		The Raspberry Pi is a credit-card-sized single-board computer.
	  *		This namespace contains all rpi specific stuff.
	  *		e.g. Interrupt-handling, address-space size, rpi specific Hardware and structures
	  * 
	  * @todo	Implement
	  */
	namespace rpi
	{
		/**
		  * @brief	A string, representing the architecture (rpi).
		  */
		extern const char *arch_string;
	}
}

#endif
