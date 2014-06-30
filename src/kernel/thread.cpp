#include "include/thread.hpp"
#include "include/memmgr.hpp"
#include "include/heap.hpp"

using kernel::thread;
using kernel::threadmgr;

thread::thread( process *proc, uintptr_t entry, size_t flags )
{
	if(!proc || !entry)
	{
		return;
	}

	this->id = thread::new_id();

	this->proc  = proc;

	this->flags = flags;

	this->kstack = (uintptr_t)kernel::malloc(thread::kstack_size);

	if(flags & thread::kernel)
	{
		this->ustack = (*this->proc->memmgr).alloc(vmm::present | vmm::write, bytes_to_pages(thread::ustack_size));
	}
	else
	{
		this->ustack = (*this->proc->memmgr).alloc(vmm::present | vmm::write | vmm::user, bytes_to_pages(thread::ustack_size));
	}

	this->cpu  = (cpu::cpu_state*)(this->kstack + thread::kstack_size - sizeof(cpu::cpu_state));

	#if defined(ARCH_X86)
	this->cpu->eip = entry;
	this->cpu->esp = this->ustack + thread::ustack_size;

	if(flags & thread::kernel)
	{
		this->cpu->cs = 0x08 | 0x00;
	}
	else
	{
		this->cpu->cs = 0x18 | 0x03;
		this->cpu->ss = 0x20 | 0x03;
	}
	
	this->cpu->eflags = 0x202;
	#elif defined(ARCH_X64)
	/*
	TODO
	*/
	#elif defined(ARCH_RPI)
	/*
	TODO
	*/
	#endif

	this->proc->thrds.push_back(this);
}

thread::~thread(  )
{
	for(tools::list<thread>::iterator it = this->proc->thrds.begin(); it != this->proc->thrds.end(); it++)
	{
		if(*it == this)
		{
			this->proc->thrds.erase(it);
		}
	}

	kernel::free((void*)this->kstack);
	
	(*this->proc->memmgr).free(this->ustack, bytes_to_pages(thread::ustack_size));
}

thread::info_t thread::info(  )
{
	static thread::info_t info;

	info.id    = this->id;
	info.state = this->state;
	info.flags = this->flags;

	return info;
}

void thread::save( cpu::cpu_state *cpu )
{
	this->cpu = cpu;
}

cpu::cpu_state *thread::restore(  )
{
	return this->cpu;
}



threadmgr::threadmgr(  )
{

}

void threadmgr::add( thread *thrd )
{
	if(thrd && this->get(thrd) == nullptr)
	{
		this->thrds.push_back(thrd);
	}
}

void threadmgr::remove( thread *thrd )
{
	for(tools::list<thread>::iterator it = this->thrds.begin(); it != this->thrds.end(); it++)
	{
		if(*it == thrd)
		{
			this->thrds.erase(it);	
		}
	}
}

thread *threadmgr::get( thread::id_t id )
{
	if(this->thrds.empty())
	{
		return nullptr;
	}

	for(tools::list<thread>::iterator it = this->thrds.begin(); it != this->thrds.end(); it++)
	{
		if(it->id == id)
		{
			return *it;
		}
	}

	return nullptr;
}

thread *threadmgr::get( thread *thrd )
{
	if(this->thrds.empty())
	{
		return nullptr;
	}

	for(tools::list<thread>::iterator it = this->thrds.begin(); it != this->thrds.end(); it++)
	{
		if(*it == thrd)
		{
			return *it;
		}
	}

	return nullptr;
}
