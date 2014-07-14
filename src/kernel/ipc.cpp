#include "include/ipc.hpp"
#include "include/scheduler.hpp"

#include <cstring>

using kernel::ipc::service;
using kernel::ipc::servicemgr;

uintptr_t kernel::ipc::shm_map( vmm *dest, vmm *src, uintptr_t addr, size_t n )
{
	if(!dest || !src || !addr || !n)
	{
		return 0;
	}

	uintptr_t phys = src->get_context()->phys(addr);

	uintptr_t virt = dest->alloc_vonly(n);

	if(!dest->map(virt, phys, vmm::present | vmm::write | vmm::user, n))
	{
		dest->free(virt);

		return 0;
	}

	return virt;
}

service::service( char name[max_service_name], kernel::process *proc, uintptr_t entry, size_t max_threads, size_t max_data )
{
	strcpy(this->name, name);

	this->proc		= proc;

	this->entry 		= entry;
	this->max_threads 	= max_threads;
	this->max_data 		= max_data;
}

service::~service(  )
{
	while(this->message_queue.pop_front());
}

size_t service::fetch( kernel::ipc::message_t *message )
{
	if(!message)
	{
		return 0;
	}

	size_t ret = this->message_queue.size();

	kernel::ipc::message_t *front = this->message_queue.pop_front();

	if(front)
	{
		memcpy(message, front, sizeof(kernel::ipc::message_t));
	}
	else
	{
		memset(message, 0, sizeof(kernel::ipc::message_t));
	}
	
	return ret;
}

void service::finish( kernel::ipc::message_t *message )
{
	if(!message)
	{
		return;
	}

	if(!isaligned(message->question1.data) || !isaligned(message->question2.data) || !isaligned(message->answer1.data) || !isaligned(message->answer2.data))
	{
		message->error = kernel::ipc::unaligned;
	}

	if(!is_user_space(message->question1.data) || !is_user_space(message->question2.data) || !is_user_space(message->answer1.data) || !is_user_space(message->answer2.data))
	{
		message->error = kernel::ipc::illmem;
	}

	/*
	Unmap shared memory blocks
	*/
	this->proc->memmgr->umap((uintptr_t)message->question1.data, bytes_to_pages(message->question1.data_size));
	this->proc->memmgr->umap((uintptr_t)message->question2.data, bytes_to_pages(message->question2.data_size));
	this->proc->memmgr->umap((uintptr_t)message->answer1.data, bytes_to_pages(message->answer1.data_size));
	this->proc->memmgr->umap((uintptr_t)message->answer2.data, bytes_to_pages(message->answer2.data_size));
	
		
	threadmgr  &thrdmgr   = threadmgr::instance(); 
	scheduler  &scheduler = scheduler::instance();

	thread *sender = thrdmgr.get(message->sender);

	/*
	Pass returned error code to sender
	*/
	sender->cpu->param0() = message->error;

	/*
	Unblock the sender thread
	*/
	scheduler.unblock(sender);
}

int service::_do_( kernel::thread *sender, kernel::ipc::message_t *message )
{
	if(!sender || !message)
	{
		message->error = kernel::ipc::illmem;

		return message->error;
	}

	if(!isaligned(message->question1.data) || !isaligned(message->question2.data) || !isaligned(message->answer1.data) || !isaligned(message->answer2.data))
	{
		message->error = kernel::ipc::unaligned;

		return message->error;
	}

	if(!is_user_space(message->question1.data) || !is_user_space(message->question2.data) || !is_user_space(message->answer1.data) || !is_user_space(message->answer2.data))
	{
		message->error = kernel::ipc::illmem;

		return message->error;
	}

	if(message->question1.data_size > this->max_data || message->question2.data_size > this->max_data || message->answer1.data_size > this->max_data || message->answer2.data_size > this->max_data)
	{
		message->error = kernel::ipc::toobig;

		return message->error;
	}

	/*
	Create copy of message object
	*/
	kernel::ipc::message_t *new_message = new kernel::ipc::message_t;

	if(!new_message)
	{
		message->error = kernel::ipc::outofmem;

		return message->error;
	}

	memcpy(new_message, message, sizeof(kernel::ipc::message_t));

	new_message->sender = sender->id;

	/*
	Maps the data fields into the service
	*/
	new_message->question1.data = (void*)kernel::ipc::shm_map(this->proc->memmgr, sender->proc->memmgr, (uintptr_t)new_message->question1.data, bytes_to_pages(new_message->question1.data_size));
	new_message->question2.data = (void*)kernel::ipc::shm_map(this->proc->memmgr, sender->proc->memmgr, (uintptr_t)new_message->question2.data, bytes_to_pages(new_message->question2.data_size));
	new_message->answer1.data   = (void*)kernel::ipc::shm_map(this->proc->memmgr, sender->proc->memmgr, (uintptr_t)new_message->answer1.data, bytes_to_pages(new_message->answer1.data_size));
	new_message->answer2.data   = (void*)kernel::ipc::shm_map(this->proc->memmgr, sender->proc->memmgr, (uintptr_t)new_message->answer2.data, bytes_to_pages(new_message->answer2.data_size));
	
	this->message_queue.push_back(new_message);

	scheduler  &scheduler = scheduler::instance();

	/*
	Create popup thread handling the ipc request (if supported by service)
	*/
	if(this->entry && this->running_threads < this->max_threads)
	{
		threadmgr  &thrdmgr   = threadmgr::instance(); 

		thread *popup = new thread(this->proc, this->entry, thread::kernel);

		if(popup)
		{
			scheduler.add(popup);
			thrdmgr.add(popup);

			this->running_threads++;
		}
	}

	/*
	Block the sender thread
	*/
	scheduler.block(sender);

	return 0;
}

servicemgr::servicemgr(  )
{

}

void servicemgr::add( service *srvc )
{
	if(srvc && this->get(srvc) == nullptr)
	{
		this->srvcs.push_back(srvc);
	}
}

void servicemgr::remove( service *srvc )
{
	for(tools::list<service>::iterator it = this->srvcs.begin(); it != this->srvcs.end(); it++)
	{
		if(*it == srvc)
		{
			this->srvcs.erase(it);	
		}
	}
}

service *servicemgr::get( const char name[kernel::ipc::max_service_name] )
{
	if(this->srvcs.empty())
	{
		return nullptr;
	}

	for(tools::list<service>::iterator it = this->srvcs.begin(); it != this->srvcs.end(); it++)
	{
		if(!strcmp(it->name, name))
		{
			return *it;
		}
	}

	return nullptr;
}

service *servicemgr::get( service *srvc )
{
	if(this->srvcs.empty())
	{
		return nullptr;
	}

	for(tools::list<service>::iterator it = this->srvcs.begin(); it != this->srvcs.end(); it++)
	{
		if(*it == srvc)
		{
			return *it;
		}
	}

	return nullptr;
}
