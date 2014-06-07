#include "include/process.hpp"
#include "include/thread.hpp"
#include "include/scheduler.hpp"

#include <cstring>

using kernel::process;
using kernel::processmgr;

//kernel::processmgr *kernel::procmgr;

process::process( const char name[64], const char cmdline[1024], process::id_t parent, context *cntxt ) : memory_version(0)
{
	strcpy(this->name, name);
	strcpy(this->cmdline, cmdline);

	this->id = process::new_id();

	this->parent = parent;

	this->cntxt = cntxt;

	this->memmgr = new vmm;
	(*this->memmgr).init(this->cntxt, vmm::user_space_base, vmm::user_space_limit);
}

process::~process(  )
{
	scheduler &scheduler = scheduler::instance();
	threadmgr &thrdmgr = threadmgr::instance();

	for(tools::list<thread>::iterator it = this->thrds.begin(); it != this->thrds.end(); it++)
	{
		thread *thrd = *it;

		if(thrd)
		{
			scheduler.remove(thrd);
			thrdmgr.remove(thrd);
		}
		
		delete thrd;
	}

	(*this->memmgr).clear();
	delete this->memmgr;
}

processmgr::processmgr(  )
{

}

void processmgr::add( process *proc )
{
	if(proc && this->get(proc) == nullptr)
	{
		this->procs.push_back(proc);
	}
}

void processmgr::remove( process *proc )
{
	for(tools::list<process>::iterator it = this->procs.begin(); it != this->procs.end(); it++)
	{
		if(*it == proc)
		{
			this->procs.erase(it);	
		}
	}
}

process *processmgr::get( process::id_t id )
{
	if(this->procs.empty())
	{
		return nullptr;
	}

	for(tools::list<process>::iterator it = this->procs.begin(); it != this->procs.end(); it++)
	{
		if(it->id == id)
		{
			return *it;
		}
	}

	return nullptr;
}

process *processmgr::get( const char name[64] )
{
	if(this->procs.empty())
	{
		return nullptr;
	}

	for(tools::list<process>::iterator it = this->procs.begin(); it != this->procs.end(); it++)
	{
		if(!strcmp(it->name, name))
		{
			return *it;
		}
	}

	return nullptr;
}

process *processmgr::get( process *proc )
{
	if(this->procs.empty())
	{
		return nullptr;
	}

	for(tools::list<process>::iterator it = this->procs.begin(); it != this->procs.end(); it++)
	{
		if(*it == proc)
		{
			return *it;
		}
	}

	return nullptr;
}
