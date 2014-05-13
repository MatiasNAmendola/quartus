#ifndef GPIO_HPP
#define GPIO_HPP

#include <cstdint>

namespace hal
{
	namespace rpi
	{
		/**
		  * @brief	Namespace of the GPIO.
		  */
		namespace gpio
		{
			const uint32_t	base = 0x20200000;
		}
	}
}

#endif
