#include "include/cpu.hpp"
#include "include/video.hpp"

using namespace hal::x86;

extern "C" 
{
	uint32_t cpuid_supported();
}

void cpu::dump( cpu::cpu_state *cpu )
{
	uint32_t cr0, cr2, cr3, cr4;

	asm( "mov %%cr0, %0" : "=r" (cr0) );
	asm( "mov %%cr2, %0" : "=r" (cr2) );
	asm( "mov %%cr3, %0" : "=r" (cr3) );
	asm( "mov %%cr4, %0" : "=r" (cr4) );

	screen << video::hex << video::endl << "\tInterrupt:   " << cpu->intr   << " \t\tError Code:   " << cpu->error << video::endl;
	screen << video::hex << video::endl << "\tEAX:         " << cpu->eax    << " \t\tEBX:          " << cpu->ebx;
	screen << video::hex << video::endl << "\tECX:         " << cpu->ecx    << " \t\tEDX:          " << cpu->edx;
	screen << video::hex << video::endl << "\ttESI:        " << cpu->esi    << " \t\tEDI:          " << cpu->edi;
	screen << video::hex << video::endl << "\tEBP:         " << cpu->ebp    << " \t\tEIP:          " << cpu->eip;
	screen << video::hex << video::endl << "\tCS:          " << cpu->cs     << " \t\tSS:           " << cpu->ss;
	screen << video::hex << video::endl << "\tEFLAGS:      " << cpu->eflags << " \t\tESP:          " << cpu->esp;
	screen << video::hex << video::endl << "\tCR0:         " << cr0         << " \t\tCR2:          " << cr2;
	screen << video::hex << video::endl << "\tCR3:         " << cr3         << " \t\tCR4:          " << cr4 << video::endl;	
}

bool cpu::cpuid( uint32_t func, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx )
{
	if(cpuid_supported())
	{
		asm("mov %0, %%eax;" : : "a" (func));

		asm("cpuid;" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx));

		return true;
	}

	return false;
}
