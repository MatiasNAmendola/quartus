#include "include/scheduler.hpp"
#include "include/memmgr.hpp"

using kernel::scheduler;

scheduler::scheduler(  ) : running(nullptr), cntxt(nullptr)
{

}

void scheduler::add( kernel::thread *thrd )
{
	this->ready.push_back(thrd);
}

void scheduler::remove( kernel::thread *thrd )
{
	for(tools::list<thread>::iterator it = this->ready.begin(); it != this->ready.end(); it++)
	{
		if(*it == thrd)
		{
			this->ready.erase(it);	
		}
	}
}

cpu::cpu_state *scheduler::schedule( cpu::cpu_state *cpu )
{
	if(!cpu)
	{
		return nullptr;
	}

	if(!this->ready.front())
	{
		return cpu;
	}

	if(this->running)
	{
		(*this->running).save(cpu);

		this->ready.push_back(this->running);
	}

	this->running = this->ready.pop_front();

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
