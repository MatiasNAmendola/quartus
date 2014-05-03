#include "include/ioports.hpp"

using namespace hal::x86;

/*
Port lesen Byte
*/
uint8_t io::inb( uint16_t port )						
{
	uint8_t result;

      	asm volatile ("inb %1, %0" : "=a" (result) : "d" (port));

	return result;
}

/*
Port lesen Word (2 Byte)
*/
uint16_t io::inw( uint16_t port )						
{
	uint16_t result;

      	asm volatile ("inw %1, %0" : "=a" (result) : "d" (port));

	return result;
}

/*
Port lesen DWord (4 Byte)
*/
uint32_t io::inl( uint16_t port )						
{
	uint32_t result;

      	asm volatile ("inl %1, %0" : "=a" (result) : "d" (port));

	return result;
}

/*
Port schreiben Byte
*/
void io::outb( uint16_t port, uint8_t data )					
{
      	asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}

/*
Port schreiben Word (2 Byte)
*/
void io::outw( uint16_t port, uint16_t data )					
{
      	asm volatile ("outw %0, %1" : : "a" (data), "Nd" (port));
}

/*
Port schreiben DWord (4 Byte)
*/
void io::outl( uint16_t port, uint32_t data )					
{
      	asm volatile ("outl %0, %1" : : "a" (data), "Nd" (port));
}
