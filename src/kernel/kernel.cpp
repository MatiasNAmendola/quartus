#include "include/kernel.hpp"

/*
includes from 'kernel'
*/
#include "include/output.hpp"

using namespace kernel;

void init( multiboot::info *mbs, uint32_t mb_magic )
{
	kout << "QUARTUS " << arch_string << output::endl;

	if(mb_magic != multiboot::magic)
	{
		kout << "Error: no multiboot bootloader!" << output::endl;
		cpu::halt();
	}

	switch(multiboot::error err = multiboot::check_mbs(mbs))
	{
		case multiboot::error::no_mbs:
			kout << output::endl << "No multibootstructure! (" << output::hex << err << ")" << output::endl;
			cpu::halt();
		break;

		case multiboot::error::no_mbs_mods_addr:
			kout << output::endl << "No MODS_ADDR! (" << output::hex << err << ")" << output::endl;
			cpu::halt();
		break;
		
		case multiboot::error::no_mbs_mmap_addr:
			kout << output::endl << "No MMAP_ADDR! (" << output::hex << err << ")" << output::endl;
			cpu::halt();
		break;
		
		default:
			kout << output::endl << "check_mbs OK! (" << output::hex << err << ")" << output::endl;
		break;
	}

	kout << output::endl << "Magic number   (eax): " << output::hex << mb_magic;

	kout << output::endl << "Multiboot info (ebx): " << output::hex << mbs;
	kout << output::endl << "flags:                " << output::hex << mbs->flags;
	kout << output::endl << "mem_lower:            " << output::dec << mbs->mem_lower;
	kout << output::endl << "mem_upper:            " << output::dec << mbs->mem_upper;
	kout << output::endl << "bootdevice:           " << output::hex << mbs->bootdevice;
	kout << output::endl << "cmdline:              " << (const char*)mbs->cmdline;
	kout << output::endl << "mods_count:           " << output::dec << mbs->mods_count;
	kout << output::endl << "mods_addr:            " << output::hex << mbs->mods_addr;
	kout << output::endl << "mmap_length:          " << output::dec << mbs->mmap_length;
	kout << output::endl << "mmap_addr:            " << output::hex << mbs->mmap_addr;
	kout << output::endl << "boot_loader_name:     " << (const char*)mbs->boot_loader_name << output::endl;

	/*
	Initialise the GDT
	*/
	gdt &gdt = gdt::instance();
	gdt.init();
	gdt.load();

	/*
	Initialise the IDT
	*/
	idt &idt = idt::instance();
	idt.init();
	idt.load();

	/*
	Initialise the PIC
	*/
	pic &pic = pic::instance();
	pic.init();
/*
	pic.handler(0, []( cpu::cpu_state *cpu ) { static int counter; kout << "TIMER " << output::dec << counter++ << output::endl; return cpu; });
	pic.enable(0);
*/

	/*
	Initialise the Physical Memory Manager
	*/
	pmm &pmm = pmm::instance();

	switch(pmm::error err = pmm.init(mbs))
	{
		case pmm::error::no_kernel_addr:
			kout << output::endl << "No kernel address! (" << output::hex << err << ")" << output::endl;
			cpu::halt();
		break;

		case pmm::error::no_memory_map:
			kout << output::endl << "No memory map! (" << output::hex << err << ")" << output::endl;
			cpu::halt();
		break;

		default:
			kout << output::endl << "pmm init OK! (" << output::hex << err << ")" << output::endl;
		break;
	}

	intr::enable();

	asm("int $0x0");
		
	while(1);
}
