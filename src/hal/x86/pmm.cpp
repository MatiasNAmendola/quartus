#include "include/pmm.hpp"

using namespace hal::x86;

pmm::pmm(  )
{

}

pmm::error pmm::init( multiboot::info *mbs )
{
	if(!mbs)
	{
		return pmm::error::no_memory_map;
	}

	multiboot::mmap *mmap 		= (multiboot::mmap*)mbs->mmap_addr;
	multiboot::mmap *mmap_end 	= (multiboot::mmap*)mbs->mmap_addr + mbs->mmap_length;

	if(!mmap)
	{
		return pmm::error::no_memory_map;
	}

	for(size_t x = 0; x < pmm::bitmap_size; x++)
	{
		this->bitmap[x] = 0xFFFFFFFF;
	} 


	for(; mmap < mmap_end; mmap++)
	{
		if(mmap->type == 1)
		{
			uintptr_t addr 		= mmap->base;
			uintptr_t addr_end 	= mmap->base + mmap->length;

			for(; addr < addr_end; addr += 0x1000)
			{
				this->free(addr);		
			}
		}
	}

	if(!memory::kernel_base || !memory::kernel_limit)
	{
		return pmm::error::no_kernel_addr;
	}

	for(uintptr_t addr = memory::kernel_base; addr < memory::kernel_limit; addr += 0x1000)
	{
		clear_bit(this->bitmap[bitmap_px(addr)], bitmap_py(addr));
	}

	clear_bit(this->bitmap[bitmap_px(0x0)], bitmap_py(0x0));
	clear_bit(this->bitmap[bitmap_px((uintptr_t)this->bitmap)], bitmap_py((uintptr_t)this->bitmap));

	if(this->check(memory::isa_dma_limit))
	{
		this->speedup_x = bitmap_px(memory::isa_dma_limit);
	}

	return pmm::error::init_ok;
}

uintptr_t pmm::alloc(  )
{
	if(fast_access)
	{
		uintptr_t addr = fast_access;

		clear_bit(this->bitmap[bitmap_px(addr)], bitmap_py(addr));

		fast_access = 0x0;

		return addr;
	}

	if(this->speedup_x >= pmm::bitmap_size)
	{
		this->speedup_x = 0;
	}

	for(size_t x = this->speedup_x; x < pmm::bitmap_size; x++)
	{
		if(this->bitmap[x])
		{
			this->speedup_x = x;

			for(size_t y = 0; y < 32; y++)
			{
				if(this->bitmap[x] & (1 << y))
				{
					clear_bit(this->bitmap[x], y);

					return addr(x, y);
				}
			}
		}
	}

	return 0x0;
}

uintptr_t pmm::alloc( size_t n )
{
	size_t counter = 0;

	if(this->speedup_x >= pmm::bitmap_size)
	{
		this->speedup_x = 0;
	}

	for(size_t x = this->speedup_x; x < pmm::bitmap_size && counter < n; x++)
	{
		if(this->bitmap[x])
		{
			this->speedup_x = x;

			for(size_t y = 0; y < 32 && counter < n; y++)
			{
				if(this->bitmap[x] & (1 << y))
				{
					counter++;

					if(counter >= n)
					{
						uintptr_t addr = addr(x, y) - (n - 1) * 4096;

						for(size_t i = 0; i < n; i++)
						{
							clear_bit(this->bitmap[bitmap_px((addr + i * 4096))], bitmap_py((addr + i * 4096)));
						}

						return addr;
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

uintptr_t pmm::alloc( size_t n, uint32_t flags )
{
	size_t counter = 0;

	if(flags & pmm::isa_dma)
	{
		for(size_t x = bitmap_px(memory::isa_dma_base); x <= bitmap_px(memory::isa_dma_limit) && counter < n; x++)
		{
			if(this->bitmap[x])
			{
				for(size_t y = 0; y < 32 && counter < n; y++)
				{
					if((x >= bitmap_px(memory::isa_dma_limit) /*&& y >= bitmap_py(memory::isa_dma_limit)*/))
					{
						return 0x0;
					}
					else if((/*x >= bitmap_px(memory::isa_dma_base) &&*/ x <= bitmap_px(memory::isa_dma_limit)) && (x == bitmap_px(memory::isa_dma_base) && y >= bitmap_py(memory::isa_dma_base)))
					{
						if(this->bitmap[x] & (1 << y))
						{
							counter++;

							if(counter >= n)
							{
								uintptr_t addr = addr(x, y) - (n - 1) * 4096;

								for(size_t i = 0; i < n; i++)
								{
									clear_bit(this->bitmap[bitmap_px((addr + i * 4096))], bitmap_py((addr + i * 4096)));
								}

								return addr;
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
	}

	return 0x0;
}

void pmm::free( uintptr_t addr )
{
	set_bit(this->bitmap[bitmap_px(addr)], bitmap_py(addr));

	this->fast_access = addr;
}

void pmm::free( uintptr_t addr, size_t n )
{
	while(n--)
	{
		this->free(addr);
		
		addr += 0x1000;
	}
}

bool pmm::check( uintptr_t addr )
{
	return this->bitmap[bitmap_px(addr)] & (1 << bitmap_py(addr));
}

bool pmm::check( uintptr_t addr, size_t n )
{
	while(n--)
	{
		if(!(this->bitmap[bitmap_px(addr)] & (1 << bitmap_py(addr))))
		{
			return false;
		}

		addr += 0x1000;
	}

	return true;
}

