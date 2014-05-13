#ifndef MMIO_HPP
#define MMIO_HPP

#include <cstdint>

namespace hal
{
	namespace rpi
	{
		/**
		  * @brief	Namespace of the IO-operations.
		  *
		  *		Contains all IO specific stuff.
		  *		e.g. reading or writing MMIO
		  */
		namespace io
		{
			/**
			  * @brief	Writes 32-bit integer to an MMIO-register.
			  *
			  * @param	reg		MMIO-register to wite
			  * @param	data		Data to write
			  */
			void write( uint32_t reg, uint32_t data );
	
			/**
			  * @brief	Reads 32-bit integer from an MMIO-register.
			  *
			  * @parm	reg		MMIO-register to read
			  * @return			Read Data
			  */
			uint32_t read( uint32_t reg );
		}
	}
}

#endif
