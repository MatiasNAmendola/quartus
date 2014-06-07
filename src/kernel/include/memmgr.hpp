#ifndef MEMMGR_HPP
#define MEMMGR_HPP

#include "../../hal/hal.hpp"

namespace kernel
{
	/**
	  * @brief	The 'version' of the kernel memory context
	  */
	extern size_t memory_version;

	/**
	  * @brief	The Virtual Memory Context of the kernel
	  */
	extern context *cntxt;

	/**
	  * @brief	The Virtual Memory Manager of the kernel
	  */
	vmm &vmmgr(  );
}

#endif
