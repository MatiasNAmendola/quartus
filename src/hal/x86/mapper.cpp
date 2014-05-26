#include "include/mapper.hpp"
#include "include/memory.hpp"

using namespace hal::x86;

bool mapper::paging_activated 		= false;

context *mapper::current_context	= nullptr;

context::context(  )
{
	if(mapper::paging_activated)
	{
		pmm &pmm = pmm::instance();
		this->pagedir = (pagedir_t*)pmm.alloc();

		mapper::current_context->map(context::tmp_pagedir_mapping, (uintptr_t)this->pagedir, context::page_present | context::page_write);

		pagedir_t *pd = (pagedir_t*)context::tmp_pagedir_mapping;

		for(size_t index = 0; index < context::pagedir_size; index++)
		{
			pd[index] = 0;
		}

		pd[PD_INDEX(context::pagedir_mapping)] = (pagetab_t)this->pagedir | context::page_present | context::page_write;

		this->map(0x1000, 0x1000, context::page_present | context::page_write, 1023);
	}
	else
	{
		pmm &pmm = pmm::instance();
		this->pagedir = (pagedir_t*)pmm.alloc();

		for(size_t index = 0; index < context::pagedir_size; index++)
		{
			this->pagedir[index] = 0;
		}

		this->pagedir[PD_INDEX(context::pagedir_mapping)] = (pagetab_t)this->pagedir | context::page_present | context::page_write;

		this->map(0x1000, 0x1000, context::page_present | context::page_write, 1023);
	}
}

context::~context(  )
{
	pagedir_t *pd = this->pagedir;
	pagetab_t *pt;

	if(mapper::paging_activated)
	{
		if(this == mapper::current_context)
		{
			/**
			  * WTF?
			  */
		}
		else
		{
			mapper::current_context->map(context::tmp_pagedir_mapping, (uintptr_t)pd, context::page_present | context::page_write);

			pd = (pagedir_t*)context::tmp_pagedir_mapping;

			for(size_t index = 0; index < context::pagedir_size; index++)
			{
				pt = (pagetab_t*)(pd[index] & ~0xFFF);

				if(pt)
				{
					pmm &pmm = pmm::instance();
					pmm.free((uintptr_t)pt);

					pd[index] = (pagetab_t)0;
				}
			}
		}

		pmm &pmm = pmm::instance();
		pmm.free((uintptr_t)this->pagedir);
	}
	else
	{
		for(size_t index = 0; index < context::pagedir_size; index++)
		{
			pt = (pagetab_t*)(pd[index] & ~0xFFF);

			if(pt)
			{
				pmm &pmm = pmm::instance();
				pmm.free((uintptr_t)pt);

				pd[index] = (pagetab_t)0;
			}
		}

		pmm &pmm = pmm::instance();
		pmm.free((uintptr_t)this->pagedir);
	}
}

void context::activate(  )
{
	asm volatile("mov %0, %%cr3" : : "r" (this->pagedir));

	mapper::current_context = this;

	if(!mapper::paging_activated)
	{
		uint32_t cr0;

		asm volatile("mov %%cr0, %0" :  "=r" (cr0));
    		asm volatile("mov %0, %%cr0" : : "r" (cr0 | (1 << 31) | (1 << 16)));

		mapper::paging_activated = true;
	}
}

bool context::map( uintptr_t virt, uintptr_t phys, uint32_t flags )
{
	if((virt % 0x1000) != 0 || (phys % 0x1000) != 0)
	{
		return false;
	}

	pagedir_t *pd = this->pagedir;
	pagetab_t *pt;

	if(mapper::paging_activated)
	{
		if(this == mapper::current_context)
		{
			pd = (pagedir_t*)context::pagedir_mapping;

			if(pd[PD_INDEX(virt) ] & context::page_present)
			{
				pt = (pagetab_t*)(context::pagetab_mapping + PD_INDEX(virt) * 0x1000);
			}
			else
			{
				pmm &pmm = pmm::instance();
				pt = (pagetab_t*)pmm.alloc();

				if(!pt)
				{
					return false;
				}

				pd[PD_INDEX(virt)] = (pagetab_t)pt | flags;

				pt = (pagetab_t*)(context::pagetab_mapping + PD_INDEX(virt) * 0x1000);
		
				for(size_t index = 0; index < context::pagetab_size; index++)
				{
					pt[index] = 0;
				}
			}

			pt[PT_INDEX(virt)] = phys | flags;
		}
		else
		{
			mapper::current_context->map(context::tmp_pagedir_mapping, (uintptr_t)pd, context::page_present | context::page_write);

			pd = (pagedir_t*)context::tmp_pagedir_mapping;

			if(pd[PD_INDEX(virt)] & context::page_present)
			{
				pt = (pagetab_t*)(pd[PD_INDEX(virt)] & ~0xFFF);

				mapper::current_context->map(context::tmp_pagetab_mapping, (uintptr_t)pt, context::page_present | context::page_write);

				pt = (pagetab_t*)context::tmp_pagetab_mapping;
			}
			else
			{
				pmm &pmm = pmm::instance();
				pt = (pagetab_t*)pmm.alloc();

				if(!pt)
				{
					return false;
				}

				pd[PD_INDEX(virt)] = (pagetab_t)pt | flags;

				mapper::current_context->map(context::tmp_pagetab_mapping, (uintptr_t)pt, context::page_present | context::page_write);

				pt = (pagetab_t*)context::tmp_pagetab_mapping;
		
				for(size_t index = 0; index < context::pagetab_size; index++)
				{
					pt[index] = 0;
				}
			}
		}

		pt[PT_INDEX(virt)] = phys | flags;
	}
	else
	{
		if(pd[PD_INDEX(virt)] & context::page_present)
		{
			pt = (pagetab_t*)(pd[PD_INDEX(virt)] & ~0xFFF);
		}
		else
		{
			pmm &pmm = pmm::instance();
			pt = (pagetab_t*)pmm.alloc();

			if(!pt)
			{
				return false;
			}

			for(size_t index = 0; index < context::pagetab_size; index++)
			{
				pt[index] = 0;
			}

			pd[PD_INDEX(virt)] = (pagetab_t)pt | flags;
		}

		pt[PT_INDEX(virt)] = phys | flags;
	}

	asm volatile("invlpg (%0)" :: "r" (virt) : "memory");

	return true;
}

bool context::map( uintptr_t virt, uintptr_t phys, uint32_t flags, size_t n )
{
	uintptr_t vaddr = virt;
	uintptr_t paddr = phys;

	for(size_t i = 0; i < n; i++)
	{
		if(!this->map(vaddr, paddr, flags))
		{
			return false;
		}

		vaddr += 0x1000;
		paddr += 0x1000;
	}

	return true;
}

bool context::umap( uintptr_t virt )
{
	return this->map(virt, 0x0, 0x0);
}

bool context::umap( uintptr_t virt, size_t n )
{
	uintptr_t vaddr = virt;

	for(size_t i = 0; i < n; i++)
	{
		if(!this->umap(vaddr))
		{
			return false;
		}

		vaddr += 0x1000;
	}

	return true;
}

uintptr_t context::phys( uintptr_t virt )
{
	if((virt % 0x1000) != 0)
	{
		return 0x0;
	}

	pagedir_t *pd = this->pagedir;
	pagetab_t *pt;

	if(mapper::paging_activated)
	{
		if(this == mapper::current_context)
		{
			pd = (pagedir_t*)context::pagedir_mapping;

			if(pd[PD_INDEX(virt) ] & context::page_present)
			{
				pt = (pagetab_t*)(context::pagetab_mapping + PD_INDEX(virt) * 0x1000);

				if(pt)
				{
					return (pt[PT_INDEX(virt)] & ~0xFFF);
				}
			}
		}
		else
		{
			mapper::current_context->map(context::tmp_pagedir_mapping, (uintptr_t)pd, context::page_present | context::page_write);

			pd = (pagedir_t*)context::tmp_pagedir_mapping;


			if(pd[PD_INDEX(virt)] & context::page_present)
			{
				pt = (pagetab_t*)(pd[PD_INDEX(virt)] & ~0xFFF);

				mapper::current_context->map(context::tmp_pagetab_mapping, (uintptr_t)pt, context::page_present | context::page_write);

				pt = (pagetab_t*)context::tmp_pagetab_mapping;

				if(pt)
				{
					return (pt[PT_INDEX(virt)] & ~0xFFF);
				}
			}
		}
	}
	else
	{
		if(pd[PD_INDEX(virt)] & context::page_present)
		{
			pt = (pagetab_t*)(pd[PD_INDEX(virt)] & ~0xFFF);

			if(pt)
			{
				return (pt[PT_INDEX(virt)] & ~0xFFF);
			}
		}
	}

	return 0x0;
}

uint32_t context::flags( uintptr_t virt )
{
	if((virt % 0x1000) != 0)
	{
		return 0x0;
	}

	pagedir_t *pd = this->pagedir;
	pagetab_t *pt;

	if(mapper::paging_activated)
	{
		if(this == mapper::current_context)
		{
			pd = (pagedir_t*)context::pagedir_mapping;

			if(pd[PD_INDEX(virt) ] & context::page_present)
			{
				pt = (pagetab_t*)(context::pagetab_mapping + PD_INDEX(virt) * 0x1000);

				if(pt)
				{
					return (pt[PT_INDEX(virt)] & 0xFFF);
				}
			}
		}
		else
		{
			mapper::current_context->map(context::tmp_pagedir_mapping, (uintptr_t)pd, context::page_present | context::page_write);

			pd = (pagedir_t*)context::tmp_pagedir_mapping;

			if(pd[PD_INDEX(virt)] & context::page_present)
			{
				pt = (pagetab_t*)(pd[PD_INDEX(virt)] & ~0xFFF);

				mapper::current_context->map(context::tmp_pagetab_mapping, (uintptr_t)pt, context::page_present | context::page_write);

				pt = (pagetab_t*)context::tmp_pagetab_mapping;

				if(pt)
				{
					return (pt[PT_INDEX(virt)] & 0xFFF);
				}
			}
		}
	}
	else
	{
		if(pd[PD_INDEX(virt)] & context::page_present)
		{
			pt = (pagetab_t*)(pd[PD_INDEX(virt)] & ~0xFFF);

			if(pt)
			{
				return (pt[PT_INDEX(virt)] & 0xFFF);
			}
		}
	}

	return 0x0;
}

bool context::copy( context &src, uintptr_t start, size_t n )
{
	uintptr_t vaddr = start;

	for(size_t i = 0; i < n; i++)
	{
		if(!this->map(vaddr, src.phys(vaddr), src.flags(vaddr)))
		{
			return false;
		}

		vaddr += 0x1000;
	}

	return true;
}
