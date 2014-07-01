#include "include/vmm.hpp"

using namespace hal::x86;

vmm::vmm(  ) : cntxt(nullptr), addr_space_base(0x0), addr_space_limit(0x0)
{
		
}

vmm::~vmm(  )
{
	for(size_t x = bitmap_px(this->addr_space_base); x <= bitmap_px(this->addr_space_limit); x++)
	{
		if(this->bitmap[x] != 0xFFFFFFFF)
		{
			for(size_t y = 0; y < 32 && x <= bitmap_px(this->addr_space_limit); y++)
			{
				if(!(this->bitmap[x] & (1 << y)))
				{
					this->free(addr(x,y));
				}
			}
		}
	}
}

vmm::error vmm::init( context *cntxt, uintptr_t base, uintptr_t limit )
{
	this->cntxt = cntxt;

	this->addr_space_base  = base;
	this->addr_space_limit = limit;

	for(size_t x = 0; x < vmm::bitmap_size; x++)
	{
		this->bitmap[x] = 0x00000000;
	}

	for(uintptr_t addr = this->addr_space_base; addr < this->addr_space_limit; addr += memory::page_size_byte)
	{
		set_bit(this->bitmap[bitmap_px(addr)], bitmap_py(addr));
	}

	if(!memory::kernel_base || !memory::kernel_limit)
	{
		return vmm::error::no_kernel_addr;
	}

	for(uintptr_t addr = memory::kernel_base; addr < memory::kernel_limit; addr += memory::page_size_byte)
	{
		clear_bit(this->bitmap[bitmap_px(addr)], bitmap_py(addr));
	}

	clear_bit(this->bitmap[bitmap_px(0x0)], bitmap_py(0x0));
	clear_bit(this->bitmap[bitmap_px((uintptr_t)this->bitmap)], bitmap_py((uintptr_t)this->bitmap));

	return vmm::error::init_ok;
}

uintptr_t vmm::alloc( uint32_t flags )
{
	pmm &pmm = pmm::instance();

	uintptr_t phys = pmm.alloc();
	uintptr_t virt = 0x0;

	if(!phys)
	{
		return 0x0;
	}

	if(this->fast_access)
	{
		virt = fast_access;

		clear_bit(this->bitmap[bitmap_px(virt)], bitmap_py(virt));

		fast_access = 0x0;

		this->cntxt->map(virt, phys, flags);

		return virt;
	}

	return this->alloc(flags, 1);
}

uintptr_t vmm::alloc( uint32_t flags, size_t n )
{
	pmm &pmm = pmm::instance();

	uintptr_t phys = 0x0;
	uintptr_t virt = 0x0;

	size_t counter = 0;

	if(this->speedup_x > bitmap_px(this->addr_space_limit) || this->speedup_x < bitmap_px(this->addr_space_base))
	{
		this->speedup_x = bitmap_px(this->addr_space_base);
	}

	for(size_t x = this->speedup_x; x <= bitmap_px(this->addr_space_limit) && counter < n; x++)
	{
		if(this->bitmap[x])
		{
			this->speedup_x = x;

			for(size_t y = 0; y < 32 && counter < n; y++)
			{
				if((x > bitmap_px(this->addr_space_limit) /*&& y >= bitmap_py(this->addr_space_limit)*/))
				{
					return 0x0;
				}
				else if(/*x >= bitmap_px(this->addr_space_base) &&*/ x <= bitmap_px(this->addr_space_limit) && ((x == bitmap_px(this->addr_space_base) && y >= bitmap_py(this->addr_space_base)) || x > bitmap_px(this->addr_space_base)))
				{
					if(this->bitmap[x] & (1 << y))
					{
						counter++;

						if(counter >= n)
						{
							virt = addr(x, y) - (n - 1) * memory::page_size_byte;

							for(size_t i = 0; i < n; i++)
							{
								clear_bit(this->bitmap[bitmap_px((virt + i * memory::page_size_byte))], bitmap_py((virt + i * memory::page_size_byte)));

								phys = pmm.alloc();

								if(!phys)
								{
									return 0x0;
								}

								this->cntxt->map((virt + i * memory::page_size_byte), phys, flags);
							}

							return virt;
						}
					}
					else
					{
						counter = 0;
					}
				}
				else
				{
					counter = 0;
				}
			}
		}
	}

	return 0x0;
}

void vmm::free( uintptr_t addr )
{
	if(addr < this->addr_space_base || addr > this->addr_space_limit)
	{
		return;
	}

	uintptr_t phys = this->cntxt->phys(addr);

	pmm &pmm = pmm::instance();
	pmm.free(phys);

	this->cntxt->umap(addr);

	set_bit(this->bitmap[bitmap_px(addr)], bitmap_py(addr));

	this->fast_access = addr;
}

void vmm::free( uintptr_t addr, size_t n )
{
	while(n--)
	{
		this->free(addr);
		
		addr += memory::page_size_byte;
	}
}

bool vmm::map( uintptr_t virt, uintptr_t phys, uint32_t flags )
{
	if(virt < this->addr_space_base || virt > this->addr_space_limit)
	{
		return false;
	}

	clear_bit(this->bitmap[bitmap_px(virt)], bitmap_py(virt));

	return this->cntxt->map(virt, phys, flags);
}

bool vmm::map( uintptr_t virt, uintptr_t phys, uint32_t flags, size_t n )
{
	if(virt < this->addr_space_base || virt > this->addr_space_limit || (virt + memory::page_size_byte * n) > this->addr_space_limit)
	{
		return false;
	}

	while(n--)
	{
		if(!this->map(virt, phys, flags))
		{
			return false;
		}
		
		virt += memory::page_size_byte;
		phys += memory::page_size_byte;
	}

	return true;
}

bool vmm::check( uintptr_t addr )
{
	if(addr < this->addr_space_base || addr > this->addr_space_limit)
	{
		return false;
	}

	return this->bitmap[bitmap_px(addr)] & (1 << bitmap_py(addr));
}

bool vmm::check( uintptr_t addr, size_t n )
{
	if(addr < this->addr_space_base || addr > this->addr_space_limit || (addr + memory::page_size_byte * n) > this->addr_space_limit)
	{
		return false;
	}

	while(n--)
	{
		if(!(this->bitmap[bitmap_px(addr)] & (1 << bitmap_py(addr))))
		{
			return false;
		}

		addr += memory::page_size_byte;
	}

	return true;
}

context *vmm::get_context(  )
{
	return this->cntxt;
}
