#ifndef HAL_HPP
#define HAL_HPP

#include <definitions.hpp>

#ifdef arch_x86

#include "x86/include/x86.hpp"
using namespace hal::x86;

#elif arch_x64

#include "x64/include/x64.hpp"
using namespace hal::x64;

#elif arch_rpi

#include "rpi/include/rpi.hpp"
using namespace hal::rpi;

#endif

/**
  * @brief	The Hardware Abstraction Layer namespace
  *
  *		The Hardware Abstration Layer should contain ALL platform dependent stuff.
  *		This namespace should only contain the namespaces of the different architectures.
  */
namespace hal
{

}

#endif
