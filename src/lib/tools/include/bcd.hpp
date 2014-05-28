#ifndef BCD_HPP
#define BCD_HPP

#include <cstdint>

namespace tools
{
	namespace bcd
	{
		uint8_t  decode( uint8_t  input );
		uint16_t decode( uint16_t input );
		uint32_t decode( uint32_t input );

		uint8_t  encode( uint8_t  input );
		uint16_t encode( uint16_t input );
		uint32_t encode( uint32_t input );
	}
}

#endif
