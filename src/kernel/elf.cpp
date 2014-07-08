#include "include/elf.hpp"
#include "include/memmgr.hpp"

#include "../hal/hal.hpp"

#include <cstring>

using kernel::elf;

elf::elf( uintptr_t addr, size_t size ) : image(addr), size(size)
{
	if(!this->image || !this->size)
	{
		return;
	}

		
}

bool elf::check(  )
{
	if(!this->image || !this->size)
	{
		return false;
	}

	if(memcmp((void*)this->image, (void*)elf::magic, sizeof(elf::magic)))
	{
		return false;
	}

	elf::header_t *header = (elf::header_t*)this->image;

	if(header->ident[4] != elf::class_32 || header->ident[5] != elf::data_2lsb || header->ident[6] != 1 || header->machine != machine_386)
	{
		return false;
	}

	return true;
}

kernel::process *elf::load_process( const char name[64], const char cmdline[1024], process::id_t parent )
{
	if(!this->check())
	{
		return nullptr;
	}

	elf::header_t *header = (elf::header_t*)this->image;

	if(header->type != elf::type_exec || header->machine != elf::machine_386 || !header->phoff || !header->phnum)
	{
		return nullptr;
	}

	elf::program_header_t *program_header = (elf::program_header_t*)(this->image + header->phoff);

	size_t nsize = 0;

	for(size_t counter = 0; counter < header->phnum && program_header != nullptr; counter++, program_header++)
	{
		if(program_header->type == elf::ph_load)
		{
			nsize += program_header->vaddr - header->entry + program_header->memsz;
		}
	}	

	pmm &pmm = pmm::instance();

	uintptr_t physd = pmm.alloc(bytes_to_pages(nsize));
	
	if(!physd)
	{
		return nullptr;
	}

	uintptr_t virtd = kernel::vmmgr().alloc_vonly(bytes_to_pages(nsize));

	if(!virtd)
	{
		pmm.free(physd, bytes_to_pages(nsize));

		return nullptr;
	}

	if(!kernel::vmmgr().map(virtd, physd, vmm::present | vmm::write, bytes_to_pages(nsize)))
	{
		pmm.free(physd, bytes_to_pages(nsize));
		kernel::vmmgr().free(virtd, bytes_to_pages(nsize));

		return nullptr;
	}

	program_header = (elf::program_header_t*)(this->image + header->phoff);

	for(size_t counter = 0; counter < header->phnum && program_header != nullptr; counter++, program_header++)
	{
		if(program_header->type == elf::ph_load)
		{
			memcpy((void*)(virtd + (program_header->vaddr - header->entry)), (void*)(this->image + program_header->offset), program_header->filesz);
		}
	}	

	kernel::vmmgr().umap(virtd, bytes_to_pages(nsize));
	
	context *cntxt = new context();

	if(!cntxt)
	{
		pmm.free(physd, bytes_to_pages(nsize));
		kernel::vmmgr().free(virtd, bytes_to_pages(nsize));

		return nullptr;
	}

	kernel::process *new_proc = new process(name, cmdline, parent, cntxt);

	if(!new_proc)
	{
		pmm.free(physd, bytes_to_pages(nsize));
		kernel::vmmgr().free(virtd, bytes_to_pages(nsize));

		delete cntxt;

		return nullptr;
	}

	if(!new_proc->memmgr->map(header->entry, physd, vmm::present | vmm::write | vmm::user, bytes_to_pages(nsize)))
	{
		pmm.free(physd, bytes_to_pages(nsize));
		kernel::vmmgr().free(virtd, bytes_to_pages(nsize));

		delete cntxt;
		delete new_proc;
	}

	return new_proc;
}

uintptr_t elf::entry(  )
{
	if(!this->check())
	{
		return 0x0;
	}

	elf::header_t *header = (elf::header_t*)this->image;

	if(header->type != elf::type_exec || header->machine != elf::machine_386 || !header->phoff || !header->phnum)
	{
		return 0x0;
	}

	return header->entry;
}
