#include "include/kernel.hpp"

/*
includes from 'kernel'
*/
#include "include/output.hpp"
#include "include/heap.hpp"
#include "include/new.hpp"
#include "include/timer.hpp"

#include "include/process.hpp"
#include "include/thread.hpp"
#include "include/scheduler.hpp"

#include "include/syscall.hpp"

#include "include/tar.hpp"
#include "include/elf.hpp"

#include <cstring>

using namespace kernel;

extern "C"
{
	static char chr  = 'A';
	
	void thread_entry(  )
	{
		char my_chr = chr++;

		while(1)
		{	
			kout << my_chr;

			asm("int $0xFF" : : "a"(syscall::exit_thread));
		}
	}
}

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
	switch(vmm::error err = vmmgr().init(&kernel_context, vmm::kernel_space_base, vmm::kernel_space_limit))
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
	vmmgr().map(memory::videomem, memory::videomem, vmm::present | vmm::write);

	/*
	Multibootinformation mapping
	*/
	vmmgr().map(align4k((uintptr_t)mbs), align4k((uintptr_t)mbs), vmm::present);
	vmmgr().map(align4k((uintptr_t)mbs->cmdline), align4k((uintptr_t)mbs->cmdline), vmm::present);
	vmmgr().map(align4k((uintptr_t)mbs->boot_loader_name), align4k((uintptr_t)mbs->boot_loader_name), vmm::present);
	vmmgr().map(align4k((uintptr_t)mbs->mods_addr), align4k((uintptr_t)mbs->mods_addr), vmm::present, ((sizeof(multiboot::mods) * mbs->mods_count + 4095) / 4096) + 1);

	multiboot::mods *mods = (multiboot::mods*)mbs->mods_addr;

	for(size_t index = 0; index < mbs->mods_count; index++)
	{
		for(uintptr_t addr = mods[index].mod_start; addr < mods[index].mod_end; addr += memory::page_size_byte)
		{
			vmmgr().map(addr, addr, vmm::present);
		}
	}

	/*
	Activate the kernel virtual-memory context
	*/
	kernel_context.activate();
	kernel::cntxt = &kernel_context;


	/*
	Initialise the system-timer
	*/
	#if defined(ARCH_X86) || defined(ARCH_X64)

	time_t year, month, day, hour, min, sec;
	cmos::date(&year, &month, &day);
	cmos::time(&hour, &min, &sec);

	pit::init(pit::channel_0, pit::rate, 500);

	timer::init(0, unixtime(year, month, day, hour, min, sec), 500);

	time_t time = timer::time;

	kout << output::endl << ctime(&time);

	#elif defined(ARCH_RPI)
	/*
	TODO
	*/
	#endif

	/*
	Get an instance of the process an thread managers
	*/
	processmgr &procmgr   = processmgr::instance(); 
	threadmgr  &thrdmgr   = threadmgr::instance(); 
	scheduler  &scheduler = scheduler::instance();


	/*
	Spawn first process
	*/
	process *proc0 = new process("proc0", "", 0, &kernel_context);

	procmgr.add(proc0);

	thread *thrd00 = new thread(proc0, (uintptr_t)thread_entry, thread::kernel);
	thread *thrd01 = new thread(proc0, (uintptr_t)thread_entry, thread::kernel);
	thread *thrd02 = new thread(proc0, (uintptr_t)thread_entry, thread::kernel);
	thread *thrd03 = new thread(proc0, (uintptr_t)thread_entry, thread::kernel);
	thread *thrd04 = new thread(proc0, (uintptr_t)thread_entry, thread::kernel);

	scheduler.add(thrd00);
	scheduler.add(thrd01);
	scheduler.add(thrd02);
	scheduler.add(thrd03);
	scheduler.add(thrd04);

	thrdmgr.add(thrd00);
	thrdmgr.add(thrd01);
	thrdmgr.add(thrd02);
	thrdmgr.add(thrd03);
	thrdmgr.add(thrd04);
	

	/*
	Spawn new process
	*/
	process *proc1 = new process("proc1", "", 0, &kernel_context);

	procmgr.add(proc1);

	thread *thrd10 = new thread(proc1, (uintptr_t)thread_entry, thread::kernel);
	thread *thrd11 = new thread(proc1, (uintptr_t)thread_entry, thread::kernel);
	thread *thrd12 = new thread(proc1, (uintptr_t)thread_entry, thread::kernel);
	thread *thrd13 = new thread(proc1, (uintptr_t)thread_entry, thread::kernel);
	thread *thrd14 = new thread(proc1, (uintptr_t)thread_entry, thread::kernel);

	scheduler.add(thrd10);
	scheduler.add(thrd11);
	scheduler.add(thrd12);
	scheduler.add(thrd13);
	scheduler.add(thrd14);

	thrdmgr.add(thrd10);
	thrdmgr.add(thrd11);
	thrdmgr.add(thrd12);
	thrdmgr.add(thrd13);
	thrdmgr.add(thrd14);

	/*
	Initialise the syscall interface
	*/
	intr::handler(0xFF, []( cpu::cpu_state *cpu ) { return syscall::handle(cpu); });
	intr::handler(0xEE, []( cpu::cpu_state *cpu ) { kernel::scheduler &sched = kernel::scheduler::instance(); return sched.schedule(cpu); });


	/*
	Test TAR
	*/
	if(mods[0].mod_start != 0x0)
	{
		tar *inittar = new tar(mods[0].mod_start);

		char buffer[6000];

		memset(buffer, 0, 6000);

		inittar->read("elf.app", buffer, 6000);

		elf *app = new elf((uintptr_t)buffer, 6000);

		if(app->check())
		{
			process *proc2 = app->load_process("proc2", "", 0);
		
			if(proc2)
			{
				procmgr.add(proc2);

				thread *thrd = new thread(proc2, app->entry(), thread::kernel);

				thrdmgr.add(thrd);
				scheduler.add(thrd);
			}
		}

		delete app;
		delete inittar;
	}

	/*
	Enable interrupts; give control to threads
	*/
	intr::enable();

	while(1);
}
