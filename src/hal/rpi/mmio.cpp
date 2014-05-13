#include "include/mmio.hpp"

using namespace hal::rpi;

void io::write( uint32_t reg, uint32_t data )
{
	asm volatile("str %[data], [%[reg]]" : : [reg]"r"((uint32_t*)reg), [data]"r"(data));
}

uint32_t io::read( uint32_t reg )
{
	uint32_t data;

	asm volatile("ldr %[data], [%[reg]]" : [data]"=r"(data) : [reg]"r"((uint32_t*)reg));

	return data;
}
