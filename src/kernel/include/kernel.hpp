#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <definitions.hpp>

/*
Includes from 'hal'
*/
#include "../../hal/hal.hpp"

/*
Includes from 'multiboot'
*/
#include "../../multiboot/include/multiboot.hpp"

/*
includes from 'kernel'
*/
#include "output.hpp"

/**
  * @brief	The kernel namepace.
  *
  *		Contains functions and definitions for the kernel.
  *		Contains gloabal kernel-variables.
  */
namespace kernel
{
	/**
	  * @brief	The standard-output of the kernel
	  *
	  *		The standard-output is defined with a function (c++ lambda-function) putting a char to 'screen' (defined in 'hal').
	  *		If compiled with 'QEMU_DEBUG' option, the char is also send to the qemu serial port.
	  */
	#ifdef QEMU_DEBUG
	output kout = output( []( char c ){ screen.putc(c); io::outb(definitions::qemu_serial, c); } );
	#else
	output kout = output( []( char c ){ screen.putc(c); } );
	#endif
}

/**
  * @brief 	The init-function of the kernel
  *
  *		This function initialises all important data-structures, hard- and software.
  *			e.g. the memory-manager and the interrupt-system are initialised
  *
  * @param	mbs		Multiboot-structure (passed by bootloader)
  * @param	mb_magic	Multiboot-magicnumber (passed by bootloader; used to check if loaded by a compatible loader)
  *
  */
extern "C" void init( multiboot::multiboot_info *mbs, uint32_t mb_magic );

#endif
