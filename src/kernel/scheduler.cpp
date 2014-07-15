#include "include/scheduler.hpp"
#include "include/memmgr.hpp"

#include "include/output.hpp"

using kernel::scheduler;

scheduler::scheduler(  ) : running(nullptr), cntxt(nullptr)
{

}

void scheduler::add( kernel::thread *thrd )
{
	if(thrd)
	{
		this->ready.push_back(thrd);
	}
}

void scheduler::remove( kernel::thread *thrd )
{
	if(thrd)
	{
		for(tools::list<thread>::iterator it = this->ready.begin(); it != this->ready.end(); it++)
		{
			if(*it == thrd)
			{			
				this->ready.erase(it);	
			}
		}
	}
}

void scheduler::block( kernel::thread *thrd )
{
	if(thrd)
	{
		(*thrd).state = kernel::thread::waiting;

		this->remove(thrd);
		
		this->waiting.push_back(thrd);
	}
}

void scheduler::unblock( kernel::thread *thrd )
{
	if(thrd)
	{
		(*thrd).state = kernel::thread::ready;
	
		this->ready.push_back(thrd);

		for(tools::list<thread>::iterator it = this->waiting.begin(); it != this->waiting.end(); it++)
		{
			if(*it == thrd)
			{
				this->waiting.erase(it);
			}
		}
	}
}

void scheduler::sleep( kernel::thread *thrd, time_t msec )
{
	if(thrd)
	{
		(*thrd).state = kernel::thread::sleeping;
	
		this->remove(thrd);

		if(this->sleeping.empty())
		{
			(*thrd).sleep = msec;

			this->sleeping.push_front(thrd);
		}
		else
		{
			time_t sleep = 0;

			for(tools::list<thread>::iterator it = this->sleeping.begin(); it != this->sleeping.end(); it++)
			{
				sleep += (**it).sleep;

				if(sleep >= msec)
				{
					sleep -= (**it).sleep;

					(*thrd).sleep = msec - sleep;

					(**it).sleep -= (*thrd).sleep;
			
					this->sleeping.insert(it, thrd);

					return;
				}
			}

			(*thrd).sleep = msec - sleep;

			this->sleeping.push_back(thrd);
		}
	}
}

void scheduler::wake( kernel::thread *thrd )
{
	if(thrd)
	{
		(*thrd).state = kernel::thread::ready;
	
		this->ready.push_back(thrd);

		for(tools::list<thread>::iterator it = this->sleeping.begin(); it != this->sleeping.end(); it++)
		{
			if(*it == thrd)
			{
				this->sleeping.erase(it);
			}
		}
	}
}

cpu::cpu_state *scheduler::schedule( cpu::cpu_state *cpu )
{
	if(!cpu)
	{
		return nullptr;
	}

	if(this->sleeping.front())
	{
		kernel::thread *thrd = this->sleeping.front();

		(*thrd).sleep -= 1000 / timer::freq;

		while(thrd && (*thrd).sleep <= 0)
		{
			this->wake(thrd);

			thrd = this->sleeping.front();
		}
	}

	if(!this->ready.front())
	{
		return cpu;
	}

	if(this->running)
	{
		(*this->running).save(cpu);

		switch(this->running->state)
		{
			case kernel::thread::ready:
				this->ready.push_back(this->running);
			break;

			case kernel::thread::waiting:
				this->waiting.push_back(this->running);
			break;

			case kernel::thread::sleeping:

			break;

			default:
				this->ready.push_back(this->running);
			break;			
		}
	}

	this->running = this->ready.pop_front();

	while(this->running && this->running->state != kernel::thread::ready)
	{
		switch(this->running->state)
		{
			case kernel::thread::ready:
				this->ready.push_back(this->running);
			break;

			case kernel::thread::waiting:
				this->waiting.push_back(this->running);
			break;

			case kernel::thread::sleeping:
			
			break;

			default:
				this->ready.push_back(this->running);
			break;			
		}

		this->running = this->ready.pop_front();
	}

	if(this->running)
	{
		cpu = (*this->running).restore();
	
		if(this->running->proc->cntxt != this->cntxt)
		{
			this->cntxt = this->running->proc->cntxt;

			if(this->running->proc->memory_version != kernel::memory_version && this->cntxt != kernel::cntxt)
			{
				(*this->cntxt).copy(*kernel::vmmgr().get_context(), vmm::kernel_space_base, bytes_to_pages(vmm::kernel_space_limit - vmm::kernel_space_base));
			
				this->running->proc->memory_version = --kernel::memory_version;
			}

			(*this->cntxt).activate();
		}
	}

	#if defined(ARCH_X86) || defined(ARCH_X64)

	gdt &gdt = gdt::instance();
	gdt.tss().tss[1] = (uint32_t)(cpu + 1);

	#endif

	return cpu;
}
