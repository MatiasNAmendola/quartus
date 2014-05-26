#include "include/kernel.hpp"

/*
includes from 'kernel'
*/
#include "include/output.hpp"

using namespace kernel;

void init( multiboot::info *mbs, uint32_t mb_magic )
{
	kout << "QUARTUS " << arch_string << output::endl;

	#ifdef ARCH_USE_MULTIBOOT
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
			kout << output::endl << "No module structure! (" << output::hex << err << ")" << output::endl;
			cpu::halt();
		break;
		
		case multiboot::error::no_mbs_mmap_addr:
			kout << output::endl << "No memory map! (" << output::hex << err << ")" << output::endl;
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
	#endif

	#if defined(ARCH_X86) || defined(ARCH_X64)
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
	#elif defined(ARCH_RPI)
	/*
	TODO
	*/
	#endif

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

	/*
	Create virtual memory context for the kernel
	*/
	context kernel_context = context();

	/*
	Initialise the Virtual Memory Manager
	*/
	switch(vmm::error err = memmgr.init(&kernel_context, vmm::kernel_space_base, vmm::kernel_space_limit))
	{
		case vmm::error::no_kernel_addr:
			kout << output::endl << "No kernel address! (" << output::hex << err << ")" << output::endl;
			cpu::halt();
		break;

		default:
			kout << output::endl << "vmm init OK! (" << output::hex << err << ")" << output::endl;
		break;
	}

	/*
	Video-memory mapping
	*/
	memmgr.map(memory::videomem, memory::videomem, vmm::present | vmm::write);

	/*
	Multibootinformation mapping
	*/
	memmgr.map(align4k((uintptr_t)mbs), align4k((uintptr_t)mbs), vmm::present);
	memmgr.map(align4k((uintptr_t)mbs->cmdline), align4k((uintptr_t)mbs->cmdline), vmm::present);
	memmgr.map(align4k((uintptr_t)mbs->boot_loader_name), align4k((uintptr_t)mbs->boot_loader_name), vmm::present);
	memmgr.map(align4k((uintptr_t)mbs->mods_addr), align4k((uintptr_t)mbs->mods_addr), vmm::present, ((sizeof(multiboot::mods) * mbs->mods_count + 4095) / 4096));

	multiboot::mods *mods = (multiboot::mods*)mbs->mods_addr;

	for(size_t index = 0; index < mbs->mods_count; index++)
	{
		for(uintptr_t addr = mods[index].mod_start; addr < mods[index].mod_end; addr += memory::page_size_byte)
		{
			memmgr.map(addr, addr, vmm::present);
		}
	}

	/*
	Activate the kernel virtual-memory context
	*/
	kernel_context.activate();

	intr::enable();
		
	while(1);
}
