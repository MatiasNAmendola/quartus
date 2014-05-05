#include "include/multiboot.hpp"

multiboot::error multiboot::check_mbs( multiboot::multiboot_info *mbs )
{
	if(!mbs)
	{
		return multiboot::error::no_mbs;
	}

	if(!mbs->cmdline)
	{
		mbs->cmdline = multiboot::empty_cmdline;
	}

	if(mbs->mods_count > 0 && !mbs->mods_addr)
	{
		return multiboot::error::no_mbs_mods_addr;
	}
	
	if(static_cast<int32_t>(mbs->mods_count) < 0)
	{
		mbs->mods_count = 0;
	}

	if(!mbs->boot_loader_name)
	{
		mbs->boot_loader_name = multiboot::no_loader_name;
	}

	if(!mbs->mmap_addr)
	{
		return multiboot::error::no_mbs_mmap_addr;
	}

	return multiboot::error::mbs_check_ok;
}
