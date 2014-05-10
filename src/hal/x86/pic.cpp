#include "include/pic.hpp"
#include "include/ioports.hpp"

using namespace hal::x86;

pic::pic(  )
{

}

/*
Initialise the PIC
*/
void pic::init(  )
{
	//remap the IRQs - master pic
	io::outb(pic::pic1_cmd, 0x11);
	io::outb(pic::pic1_data, pic::irq_base);
	io::outb(pic::pic1_data, 0x04);
	io::outb(pic::pic1_data, 0x01);

	//remap the IRQs - slave pic
	io::outb(pic::pic2_cmd, 0x11);
	io::outb(pic::pic2_data, pic::irq_base + 8);
	io::outb(pic::pic2_data, 0x04);
	io::outb(pic::pic2_data, 0x01);

	//disable all IRQs
	this->mask(0xFFFF);

	//register the same handler for all IRQs
	for(uint32_t i = pic::irq_base; i < pic::irq_limit; i++)
	{
		intr::handler(i, []( cpu::cpu_state *cpu ) { pic &pic = pic::instance(); cpu = pic.irq(cpu); return cpu; });
	}
}

/*
get the irq-mask
*/
uint16_t pic::mask(  )
{
	uint16_t mask1 = (uint16_t)io::inb(pic::pic1_data);
	uint16_t mask2 = (uint16_t)io::inb(pic::pic2_data) << 8;

	return mask1 | mask2;
}

/*
set the irq-mask
*/
uint16_t pic::mask( uint16_t mask )
{
	uint16_t ret = this->mask();
	
	io::outb(pic::pic1_data, (uint8_t)mask);
	io::outb(pic::pic2_data, (uint8_t)mask >> 8);

	return ret;
}

/*
enable an irq
*/
void pic::enable( uint8_t irq )
{
	mask(this->mask() ^ (1 << (uint16_t)irq));
}

/*
disable an irq
*/
void pic::disable( uint8_t irq )
{
	mask(this->mask() | (1 << (uint16_t)irq));
}

/*
send the end of interrupt to the pic
*/
void pic::eoi( uint8_t irq )
{
	if(irq >= 0x07) 						
	{
		io::outb(pic::pic2_cmd, 0x20);
	}

	io::outb(pic::pic1_cmd, 0x20);
}

/*
set handler for irq
*/
void pic::handler( uint8_t irq, pic::handler_t handler )
{
	handlers[irq] = handler;
}

/*
Handler for all IRQs
*/
cpu::cpu_state *pic::irq( cpu::cpu_state *cpu )
{
	cpu::cpu_state *new_cpu = cpu;

	uint32_t irq = cpu->intr - pic::irq_base;

	if(this->handlers[irq] != pic::null_handler)
	{
		new_cpu = this->handlers[irq](cpu);
	}

	this->eoi(irq);

	return new_cpu;
}
