#include "include/delay.hpp"

void hal::rpi::delay(int32_t count) 
{
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" : : [count]"r"(count) : "cc");
}
