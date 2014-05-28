#include "include/bcd.hpp"

uint8_t  tools::bcd::decode( uint8_t  input )
{
	uint8_t output = 0;

	output = output * 10 + ((input & 0xF0) >> 4);
	output = output * 10 + ((input & 0x0F) >> 0);
	
	return output;
}

uint16_t tools::bcd::decode( uint16_t input )
{
	uint16_t output = 0;

	output = output * 10 + ((input & 0xF000) >> 12);
	output = output * 10 + ((input & 0x0F00) >> 8);
	output = output * 10 + ((input & 0x00F0) >> 4);
	output = output * 10 + ((input & 0x000F) >> 0);
	
	return output;
}

uint32_t tools::bcd::decode( uint32_t input )
{
	uint32_t output = 0;

	output = output * 10 + ((input & 0xF0000000) >> 28);
	output = output * 10 + ((input & 0x0F000000) >> 24);
	output = output * 10 + ((input & 0x00F00000) >> 20);
	output = output * 10 + ((input & 0x000F0000) >> 16);
	output = output * 10 + ((input & 0x0000F000) >> 12);
	output = output * 10 + ((input & 0x00000F00) >> 8);
	output = output * 10 + ((input & 0x000000F0) >> 4);
	output = output * 10 + ((input & 0x0000000F) >> 0);
	
	return output;
}

uint8_t  tools::bcd::encode( uint8_t  input )
{
	uint8_t output = 0;

	output |= ((input % 10) << 0) & 0x0F;

	input  /= 10; 

	output |= ((input % 10) << 4) & 0xF0;
	
	return output;
}

uint16_t tools::bcd::encode( uint16_t input )
{
	uint16_t output = 0;

	output |= ((input % 10) <<  0) & 0x000F;

	input  /= 10; 
	output |= ((input % 10) <<  4) & 0x00F0;
	
	input  /= 10; 
	output |= ((input % 10) <<  8) & 0x0F00;

	input  /= 10; 
	output |= ((input % 10) << 12) & 0xF000;

	return output;
}

uint32_t tools::bcd::encode( uint32_t input )
{
	uint32_t output = 0;

	output |= ((input % 10) <<  0) & 0x00000000F;

	input  /= 10; 
	output |= ((input % 10) <<  4) & 0x0000000F0;
	
	input  /= 10; 
	output |= ((input % 10) <<  8) & 0x000000F00;

	input  /= 10; 
	output |= ((input % 10) << 12) & 0x0000F000;

	input  /= 10; 
	output |= ((input % 10) << 16) & 0x000F0000;

	input  /= 10; 
	output |= ((input % 10) << 20) & 0x00F00000;

	input  /= 10; 
	output |= ((input % 10) << 24) & 0x0F000000;

	input  /= 10; 
	output |= ((input % 10) << 28) & 0xF0000000;

	return output;
}
