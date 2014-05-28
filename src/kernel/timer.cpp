#include "include/timer.hpp"

using namespace kernel;

time_t timer::ticks 	= 0;
time_t timer::seconds 	= 0;
time_t timer::time 	= 0;
time_t timer::freq 	= 0;

void timer::init( uint8_t irq, time_t current, time_t freq )
{
	timer::ticks 	= 0;
	timer::seconds 	= 0;
	timer::time 	= current;
	timer::freq	= freq;	

	pic &pic = pic::instance();
	pic.handler(irq, timer::tick);
	pic.enable(irq);
}

cpu::cpu_state *timer::tick( cpu::cpu_state *cpu )
{
	timer::ticks++;

	if(!(timer::ticks % timer::freq))
	{
		timer::ticks = 0;
		timer::seconds++;
		timer::time++;
	}

	return cpu;
}
