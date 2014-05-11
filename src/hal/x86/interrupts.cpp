#include "include/interrupts.hpp"

#include "include/ioports.hpp"
#include "include/video.hpp"
#include "include/x86.hpp"

using namespace hal::x86;

static intr::handler_t handlers[intr::intr_count];
static intr::handler_t ncex_handler = intr::null_handler;

/*
Register a handler for Interrupts
*/
void intr::handler( uint32_t intr, intr::handler_t handler )
{
	handlers[intr] = handler;
}

/*
Handler for non critical exceptions
*/
void intr::ncex( handler_t handler )
{
	ncex_handler = handler;
}

static const char *error_name[intr::exception_limit - intr::exception_base] =
{
	"Devide by Zero                  ",
	"Debug                           ",
	"Non Maskable Interrupt          ",
	"Breakpoint                      ",
	"Overflow                        ",
	"Bound Range                     ",
	"Invalid Opcode                  ",
	"Device Not Available            ",
	"Double Fault                    ",
	"Coprocessor Segment Overrun     ",
	"Invalid TSS                     ",
	"Segment not Present             ",
	"Stack Fault                     ",
	"General Protection              ",
	"Page Fault                      ",
	"                                ",	
	"x87 Floating Point              ",
	"Alignment Check                 ",
	"Machine Check                   ",
	"SIMD Floating Point             ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"Security-sensitive event in Host",
	"                                ",
	"Out of memory                   "
};

static const char *error_type[intr::exception_limit - intr::exception_base]
{
	"# Fault",
	"# Fault",
	"    # -",
	" # Trap",
	" # Trap",
	"# Fault",
	"# Fault",
	"# Fault",
	"# Abort",
	"    # -",
	"# Fault",
	"# Fault",
	"# Fault",
	"# Fault",
	"# Fault",
	"    # -",
	"# Fault",
	"# Fault",
	"# Abort",
	"# Fault",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"    # -",
	"# Abort"
};

const char *no_error_name = "                                ";
const char *no_error_type = "    # -";

/*
Handler for Exceptions
*/
cpu::cpu_state *handle_exception( cpu::cpu_state *cpu )
{
	cpu::cpu_state *new_cpu = cpu;

	if(ncex_handler != intr::null_handler && ((cpu->intr == 0x0E && cpu->error & 0x04) || cpu->intr == 0xEE))
	{
		new_cpu = ncex_handler(cpu);
	}
	else
	{
		screen.clear();

		if(/*cpu->intr >= intr::exception_base && */cpu->intr <= intr::exception_limit)
		{							
			screen << color::red << "\tEXCEPTION!\t    " << error_name[cpu->intr] << "   " << error_type[cpu->intr] << video::endl;
	
			screen << color::white << video::endl << "################################################################################" << video::endl;
			screen << color::brown << video::endl << "                     QUARTUS " << arch_string << "  # " << __DATE__ << "  " << __TIME__ << video::endl;
			screen << color::white << video::endl << "################################################################################" << video::endl;
	
			cpu::dump(cpu);

			screen << color::white << video::endl << "################################################################################" << video::endl;
			screen << color::white << video::endl << "\t  System has been shut down to prevent damage on your computer" << video::endl;
			screen << color::white << video::endl << "################################################################################" << video::endl;

			cpu::halt();
		}
		else
		{				
			screen << color::red << "\tUNKNOWN ERROR!\t" << no_error_name << "   " << no_error_type << video::endl;
	
			screen << color::white << video::endl << "################################################################################" << video::endl;
			screen << color::brown << video::endl << "                     QUARTUS " << arch_string << "  # " << __DATE__ << "  " << __TIME__ << video::endl;
			screen << color::white << video::endl << "################################################################################" << video::endl;
	
			cpu::dump(cpu);

			screen << color::white << video::endl << "################################################################################" << video::endl;
			screen << color::white << video::endl << "\t  System has been shut down to prevent damage on your computer" << video::endl;
			screen << color::white << video::endl << "################################################################################" << video::endl;

			cpu::halt();
		}
	}

	return new_cpu;
}

/*
Handler for all Interrupts
*/
extern "C" cpu::cpu_state *handle_interrupt( cpu::cpu_state *cpu );
cpu::cpu_state *handle_interrupt( cpu::cpu_state *cpu )
{
	cpu::cpu_state *new_cpu = cpu;

	/*
	Exceptions
	*/
	if(/*cpu->intr >= intr::exception_base && */cpu->intr <= intr::exception_limit) 							
	{	
		new_cpu = handle_exception(cpu);
      	}
	/*
	Interrupts with registered handlers; e.g. IRQs
	*/
	else if(handlers[cpu->intr] != intr::null_handler)
	{
		new_cpu = handlers[cpu->intr](cpu);
	}
	/*
	Interrupts without registered handlers;
	*/
	else
	{
		new_cpu = handle_exception(cpu);
	}

	return new_cpu;
}
