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

/**
  * @brief	The kernel namepace.
  *
  *		Contains functions and definitions for the kernel.
  *		Contains gloabal kernel-variables.
  */
namespace kernel
{

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
extern "C" void init( multiboot::info *mbs, uint32_t mb_magic );

#endif
