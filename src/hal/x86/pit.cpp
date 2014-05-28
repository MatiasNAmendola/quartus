#include "include/pit.hpp"
#include "include/ioports.hpp"

using namespace hal::x86;

void pit::init( pit::channel ch, pit::mode md, time_t freq )
{
	time_t counter = pit::internal_freq / freq;

	io::outb(pit::init_port, (ch << 6) | (0x03 << 4) | (md << 1));
		
	switch(ch)							
	{
		case pit::channel_0:
			io::outb(0x40, counter & 0xFF);		
   			io::outb(0x40, counter >> 8);
		break;

		case pit::channel_1:
			io::outb(0x41, counter & 0xFF);
   			io::outb(0x41, counter >> 8);
		break;

		case pit::channel_2:
			io::outb(0x42, counter & 0xFF);
   			io::outb(0x42, counter >> 8);
		break;
	}
}
