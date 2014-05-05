#ifndef IOPORTS_HPP
#define IOPORTS_HPP

#include <cstdint>

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Namespace of the IO-operations.
		  *
		  *		Contains all IO specific stuff.
		  *		e.g. reading or writing IO-ports
		  */
		namespace io
		{
			/**
			  * @brief	Reads 8-bit integer from an IO-port.
			  *
			  * @param	port		IO-port to read
			  * @return			Read Data
			  */
			uint8_t inb( uint16_t port );

			/**
			  * @brief	Reads 16-bit integer from an IO-port.
			  *
			  * @param	port		IO-port to read
			  * @return			Read Data
			  */
			uint16_t inw( uint16_t port );

			/**
			  * @brief	Reads 32-bit integer from an IO-port.
			  *
			  * @param	port		IO-port to read
			  * @return			Read Data
			  */
			uint32_t inl( uint16_t port );


			/**
			  * @brief	Writes 8-bit integer to an IO-port.
			  *
			  * @param	port		IO-port to wite
			  * @param	data		Data to write
			  */
			void outb( uint16_t port, uint8_t data );

			/**
			  * @brief	Writes 16-bit integer to an IO-port.
			  *
			  * @param	port		IO-port to wite
			  * @param	data		Data to write
			  */
			void outw( uint16_t port, uint16_t data );

			/**
			  * @brief	Writes 32-bit integer to an IO-port.
			  *
			  * @param	port		IO-port to wite
			  * @param	data		Data to write
			  */
			void outl( uint16_t port, uint32_t data );
		}
	}
}

#endif
