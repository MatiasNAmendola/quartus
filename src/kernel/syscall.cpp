#include "include/syscall.hpp"

#include "include/process.hpp"
#include "include/thread.hpp"
#include "include/scheduler.hpp"

#include "include/ipc.hpp"

#include "include/timer.hpp"

#include "include/elf.hpp"

#include "include/output.hpp"

#include <cstring>

cpu::cpu_state *kernel::syscall::handle( cpu::cpu_state *cpu )
{
	processmgr &procmgr  	 = processmgr::instance();
	threadmgr &thrdmgr   	 = threadmgr::instance();
	scheduler &scheduler 	 = scheduler::instance();

	ipc::servicemgr &srvcmgr = ipc::servicemgr::instance();

	pmm &pmm = pmm::instance();

	process *proc  = nullptr;
	thread 	*thrd  = nullptr;

	ipc::service *srvc = nullptr;

	elf	*image = nullptr;

	process::info_t	pinfo;
	thread::info_t  tinfo;

	switch(cpu->syscall_number())
	{
		case syscall::spawn_raw:

		break;

		case syscall::spawn_elf:
			image = new elf((uintptr_t)cpu->param1(), cpu->param2());

			if(image && image->check())
			{
				if(scheduler.running && scheduler.running->proc)
				{
					proc = image->load_process((char*)cpu->param3(), (char*)cpu->param4(), (process::id_t)scheduler.running->proc->id);
				}
				else
				{
					proc = image->load_process((char*)cpu->param3(), (char*)cpu->param4(), 0);
				}

				if(proc)
				{
					procmgr.add(proc);

					thrd = new thread(proc, image->entry(), cpu->param5());

					thrdmgr.add(thrd);
					scheduler.add(thrd);

					cpu->param0() = proc->id;
				}
				else
				{
					cpu->param0() = -1;
				}
			}
			else
			{
					cpu->param0() = -1;
			}

			delete image;
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

			thrd = new thread(proc, (uintptr_t)cpu->param1(), (uintptr_t)cpu->param2(), (size_t)cpu->param3());

			if(thrd)
			{
				scheduler.add(thrd);
				thrdmgr.add(thrd);

				cpu->param0() = thrd->id;
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

		case syscall::sleep_thread:

		break;

		case syscall::wake_thread:

		break;

		case syscall::thread_info:
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

		

		case syscall::memory_alloc:
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

		case syscall::memory_free:
			proc = scheduler.running->proc;

			if(proc)
			{
				proc->memmgr->free(cpu->param1(), bytes_to_pages(cpu->param2()));
			}
			else
			{

			}
		break;

		case syscall::memory_info:
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

		case syscall::ipc_setup:
			srvc = new ipc::service((char*)cpu->param1(), scheduler.running->proc, cpu->param2(), cpu->param3(), cpu->param4());

			if(srvc)
			{
				srvcmgr.add(srvc);

				cpu->param0() = 0;
			}
			else
			{
				cpu->param0() = 1;
			}
		break;

		case syscall::ipc_destroy:
			srvc = srvcmgr.get((char*)cpu->param1());

			if(srvc && srvc->proc == scheduler.running->proc)
			{
				srvcmgr.remove(srvc);

				delete srvc;

				cpu->param0() = 0;
			}
			else
			{
				cpu->param0() = 1;
			}
		break;

		case syscall::ipc_fetch:
			srvc = srvcmgr.get((char*)cpu->param1());

			if(srvc && srvc->proc == scheduler.running->proc)
			{
				cpu->param0() = srvc->fetch((kernel::ipc::message_t*)cpu->param2());
			}
			else
			{
				cpu->param0() = 0;
			}
		break;

		case syscall::ipc_finish:
			srvc = srvcmgr.get((char*)cpu->param1());

			if(srvc && srvc->proc == scheduler.running->proc)
			{
				srvc->finish((kernel::ipc::message_t*)cpu->param2());

				cpu = scheduler.schedule(cpu);
			}
		break;

		case syscall::ipc_do:
			srvc = srvcmgr.get((char*)cpu->param1());

			if(srvc)
			{
				cpu->param0() = srvc->_do_(scheduler.running, (ipc::message_t*)cpu->param2());

				cpu = scheduler.schedule(cpu);
			}
			else
			{
				cpu->param0() = kernel::ipc::notfound;
			}
		break;

		case syscall::get_time:
			cpu->param0() = timer::time;
		break;

		/*
		Syscall only used for testing
		*/
		case 0xB0:
			kernel::kout << (char)cpu->param1();
		break;
	}

	return cpu;
}
