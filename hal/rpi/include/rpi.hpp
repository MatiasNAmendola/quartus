#ifndef RPI_HPP
#define RPI_HPP

namespace hal
{
	/**
	  * @brief	Namespace of the rpi_a architecture. (Raspberry Pi Model A)
	  *
	  *		This is not really a architecture, its a whole hardware platform. The Architecture is ARMv6.
	  *		The Raspberry Pi is a credit-card-sized single-board computer.
	  *		This namespace contains all rpi specific stuff.
	  *		e.g. Interrupt-handling, address-space size, rpi specific Hardware and structures
	  * 
	  * @todo	Implement
	  */
	namespace rpi_a
	{
		/**
		  * @brief	A string, representing the architecture (rpi_a).
		  */
		const char *arch_string	= "rpi_a";
	}

	/**
	  * @brief	Namespace of the rpi_b architecture. (Raspberry Pi Model B)
	  *
	  *		This is not really a architecture, its a whole hardware platform. The Architecture is ARMv6.
	  *		The Raspberry Pi is a credit-card-sized single-board computer.
	  *		This namespace contains all rpi specific stuff.
	  *		e.g. Interrupt-handling, address-space size, rpi specific Hardware and structures
	  *
	  * @todo	Implement
	  */
	namespace rpi_b
	{
		/**
		  * @brief	A string, representing the architecture (rpi_b).
		  */
		const char *arch_string	= "rpi_b";
	}
}

#endif
