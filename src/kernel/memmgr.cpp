#include "include/memmgr.hpp"

#include <cstring>

size_t kernel::memory_version = 0;

vmm &kernel::vmmgr(  ) { static vmm memmgr; kernel::memory_version++; return memmgr; }

context *kernel::cntxt;
