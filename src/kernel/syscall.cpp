#include "include/syscall.hpp"

#include "include/process.hpp"
#include "include/thread.hpp"
#include "include/scheduler.hpp"

#include <cstring>

cpu::cpu_state *kernel::syscall::handle( cpu::cpu_state *cpu )
{
	processmgr &procmgr  = processmgr::instance();
	threadmgr &thrdmgr   = threadmgr::instance();
	scheduler &scheduler = scheduler::instance();

	pmm &pmm = pmm::instance();

	process *proc;
	thread *thrd;

	process::info_t	pinfo;
	thread::info_t  tinfo;

	switch(cpu->syscall_number())
	{
		case syscall::spawn_raw:

		break;

		case syscall::spawn_elf:

		break;

		case syscall::exit_proc:
			proc = scheduler.running->proc;

			if(proc)
			{
				procmgr.remove(proc);

				scheduler.running = nullptr;

				delete proc;

				cpu = scheduler.schedule(cpu);

				if(!scheduler.running)
				{
					cpu::halt();
				}
			}
			else
			{

			}
		break;

		case syscall::kill_proc:
			proc = procmgr.get(cpu->param1());
		
			if(proc && proc != scheduler.running->proc)
			{
				procmgr.remove(proc);

				delete proc;

				cpu->param0() = 0;
			}
			else
			{
				cpu->param0() = 1;	
			}
		break;

		case syscall::proc_info:
			proc = scheduler.running->proc;

			if(proc && (process::info_t*)cpu->param1() != nullptr)
			{
				pinfo = proc->info();

				((process::info_t*)(cpu->param1()))->id	    = pinfo.id;
				((process::info_t*)(cpu->param1()))->parent = pinfo.parent;

				strcpy(((process::info_t*)(cpu->param1()))->name, pinfo.name);
				strcpy(((process::info_t*)(cpu->param1()))->cmdline, pinfo.cmdline);

				cpu->param0() = cpu->param1();
			}
			else
			{
				cpu->param0() = 0x0;
			}
		break;

		case syscall::create_thread:
			proc = scheduler.running->proc;

			thrd = new thread(proc, (uintptr_t)cpu->param1(), (size_t)cpu->param2());

			if(thrd)
			{
				scheduler.add(thrd);
				thrdmgr.add(thrd);

				cpu->param0() = 0;
			}
			else
			{
				cpu->param0() = 1;
			}
		break;

		case syscall::exit_thread:
			thrd = scheduler.running;

			if(thrd)
			{
				thrdmgr.remove(thrd);

				scheduler.running = nullptr;

				cpu = scheduler.schedule(cpu);

				if(!scheduler.running)
				{
					cpu::halt();
				}

				delete thrd;
			}
			else
			{

			}
		break;

		case syscall::kill_thread:
			thrd = thrdmgr.get(cpu->param1());
			
			if(thrd && thrd != scheduler.running)
			{
				scheduler.remove(thrd);
				thrdmgr.remove(thrd);

				delete thrd;

				cpu->param0() = 0;
			}
			else if(thrd && thrd == scheduler.running)
			{
				thrdmgr.remove(thrd);

				scheduler.running = nullptr;

				cpu = scheduler.schedule(cpu);

				if(!scheduler.running)
				{
					cpu::halt();
				}

				delete thrd;
			}
			else
			{
				cpu->param0() = 1;	
			}
		break;

		case sleep_thread:

		break;

		case wake_thread:

		break;

		case thread_info:
			thrd = scheduler.running;

			if(thrd && (thread::info_t*)cpu->param1() != nullptr)
			{
				tinfo = thrd->info();

				((thread::info_t*)(cpu->param1()))->id	  = tinfo.id;
				((thread::info_t*)(cpu->param1()))->state = tinfo.state;
				((thread::info_t*)(cpu->param1()))->flags = tinfo.flags;

				cpu->param0() = cpu->param1();
			}
			else
			{
				cpu->param0() = 0x0;
			}
		break;

		

		case memory_alloc:
			proc = scheduler.running->proc;

			if(proc)
			{
				cpu->param0() = proc->memmgr->alloc(vmm::present | vmm::write | vmm::user, bytes_to_pages(cpu->param1()));
			}
			else
			{
				cpu->param0() = 0x0;
			}
		break;

		case memory_free:
			proc = scheduler.running->proc;

			if(proc)
			{
				proc->memmgr->free(cpu->param1(), bytes_to_pages(cpu->param2()));
			}
			else
			{

			}
		break;

		case memory_info:
			if((pmm::meminfo_t*)cpu->param1() != nullptr)
			{
				((pmm::meminfo_t*)(cpu->param1()))->free = pmm.info().free;
				((pmm::meminfo_t*)(cpu->param1()))->used = pmm.info().used;
	
				cpu->param0() = cpu->param1();
			}
			else
			{
				cpu->param0() = 0x0;
			}
		break;
	}

	return cpu;
}
