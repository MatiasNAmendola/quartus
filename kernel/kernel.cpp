#include "include/kernel.hpp"

using namespace kernel;

void init( multiboot::multiboot_info *mbs, uint32_t mb_magic )
{
	kout << "TERTIUS " << arch_string << output::endl;

	if(mb_magic != multiboot::magic)
	{
		kout << "Error: no multiboot bootloader!" << output::endl;
		cpu::halt();
	}

	kout << output::endl << "Magic number   (eax): " << output::hex << mb_magic;

	multiboot::error err = multiboot::check_mbs( mbs );

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

	switch(err)
	{
		case multiboot::error::no_mbs:
			kout << output::endl << "No multibootstructure! (" << err << ")" << output::endl;
			cpu::halt();
		break;

		case multiboot::error::no_mbs_mods_addr:
			kout << output::endl << "No MODS_ADDR! (" << err << ")" << output::endl;
			cpu::halt();
		break;
		
		case multiboot::error::no_mbs_mmap_addr:
			kout << output::endl << "No MMAP_ADDR! (" << err << ")" << output::endl;
			cpu::halt();
		break;
		
		default:
			kout << output::endl << "check_mbs OK! (" << err << ")" << output::endl;
		break;
	}

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
	intr::enable();
		
	while(1);
}
