#include "include/heap.hpp"
#include "include/output.hpp"

#include <cstring>

#define item_to_ptr(itm)	((void*)      ((uintptr_t)itm + sizeof(heap::item)))	
#define ptr_to_item(ptr)	((heap::item*)((uintptr_t)ptr - sizeof(heap::item)))

using kernel::heap;

heap kernel::hpmmgr;

heap::heap(  )
{
	this->first 	   = (heap::item*)vmmgr.alloc(vmm::present | vmm::write, 4);
	this->first->flags = heap::flag_free;
	this->first->size  = memory::page_size_byte * 4;
	this->first->next  = nullptr;
	this->first->prev  = nullptr;
}

heap::~heap(  )
{
	this->first = nullptr;
}

void *heap::alloc( size_t size )
{
	while(1)
	{
		if(size >= 4096)
		{
			this->enlarge(size);
		}

		if(!this->first)
		{
			this->enlarge(size);
		}

		heap::item *itm = this->first;

		while(itm)
		{
			if(itm->size >= (size + sizeof(heap::item)) && itm->flags & heap::flag_free)
			{
				size_t ratio = itm->size / (size + sizeof(heap::item));

				if(ratio >= 2)
				{
					this->remove(itm);

					heap::item *part_1;
					heap::item *part_2;

					this->split(itm, &part_1, &part_2, ratio);
	
					this->insert(part_2);

					part_1->flags = heap::flag_used;
					part_2->flags = heap::flag_free;

					return item_to_ptr(part_1);
				}
				else
				{
					this->remove(itm);

					itm->flags = heap::flag_used;

					return item_to_ptr(itm);
				}
			}

			itm = itm->next;
		}

		this->enlarge(size);
	}
}

void heap::free( void *ptr )
{
	if(ptr)
	{
		heap::item *itm = ptr_to_item(ptr);

		if(!(itm->flags & heap::flag_free))
		{
			this->insert(itm);

			itm->flags = heap::flag_free;
		}
		else
		{
			kout << "[heap] double free or corruption (" << output::hex << itm->flags << "): " << ptr << output::endl;
		}
	}
}

void heap::insert( heap::item *itm )
{
	if(itm)
	{
		itm->next = this->first;
		itm->prev = nullptr;

		if(itm->next)
		{
			itm->next->prev = itm;
		}

		this->first = itm;
	}
}

void heap::remove( heap::item *itm )
{
	if(itm)
	{
		if(itm->next)
		{
			itm->next->prev = itm->prev;
		}

		if(itm->prev)
		{
			itm->prev->next = itm->next;
		}

		if(itm == this->first)
		{
			this->first = itm->next;
		}

		itm->next = nullptr;
		itm->prev = nullptr;
	}
}

void heap::split( heap::item *itm, heap::item **part_1, heap::item **part_2, size_t ratio )
{
	this->remove(itm);

	size_t size = itm->size / ratio;

	heap::item *p1 = itm;
	heap::item *p2 = (heap::item*)((uintptr_t)itm + size);

	p1->next = nullptr;
	p1->prev = nullptr;
	p2->next = nullptr;
	p2->prev = nullptr;

	p1->size = size;
	p2->size = size * (ratio - 1);

	*part_1 = p1;
	*part_2 = p2;
}

void heap::enlarge( size_t size )
{
	heap::item *new_itm = (heap::item*)vmmgr.alloc(vmm::present | vmm::write, bytes_to_pages(size + sizeof(heap::item)));

	if(!new_itm)
	{
		/**
		  * FIXME This is an ugly hack
		  */
		asm("int $0x20");

		return;
	}

	new_itm->flags = heap::flag_free;
	new_itm->size  = bytes_to_pages(size + sizeof(heap::item)) * memory::page_size_byte;

	this->insert(new_itm);
}

void *kernel::malloc( size_t size )
{
	return hpmmgr.alloc(size);
}

void *kernel::calloc( size_t n, size_t size )
{
	void *ptr = hpmmgr.alloc(n * size);

	if(ptr)
	{
		memset(ptr, 0, n * size);
	}

	return ptr;
}

void *kernel::realloc( void *ptr, size_t size )
{
	if(ptr)
	{
		heap::item *itm = ptr_to_item(ptr);

		if(size != itm->size)
		{
			void *new_ptr = hpmmgr.alloc(size);
		
			if(new_ptr)
			{
				if(size > itm->size)
				{
					memcpy(new_ptr, ptr, itm->size);
				}
				else
				{
					memcpy(new_ptr, ptr, size);
				}
			}	

			hpmmgr.free(ptr);
	
			return new_ptr;
		}

		return ptr;
	}
	else
	{
		return hpmmgr.alloc(size);
	}
}

void kernel::free( void *ptr )
{
	hpmmgr.free(ptr);
}
