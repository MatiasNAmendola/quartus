#include "include/tar.hpp"
#include "include/heap.hpp"

#include "include/output.hpp"

#include <number.hpp>
#include <cstring>

using kernel::tar;

tar::tar( uintptr_t addr ) : objects(0), size(0)
{
	if(addr)
	{
		for(size_t index = 0; ; index++)
		{
			tar::header_t *header = (tar::header_t*)addr;

			if(header->name[0] == '\0')
			{
				break;
			}

			size_t size = tools::stoi(header->size, 8);

			this->objects	+= 1;
			this->size 	+= size;

			this->headers    	= (tar::header_t**)kernel::realloc(this->headers, this->objects * this->size);
			this->headers[index] 	= header;

			addr += ((size / 512) + 1) * 512;

			if(size % 512)
			{
            			addr += 512;
			}
		} 
	}
}

size_t tar::read( const char *name, char *buffer, size_t size )
{
	if(!buffer)
	{
		return 0;
	}

	tar::header_t *header = nullptr;

	for(size_t index = 0; index < this->objects; index++)
	{
		if(!strcmp(this->headers[index]->name, name))
		{
			header = this->headers[index];
			
			break;
		}
	}

	if(header)
	{
		if((size_t)tools::stoi(header->size, 8) < size)
		{
			size = tools::stoi(header->size, 8);
		}

		memcpy(buffer, (char*)(header + 1), size);

		return size;
	}

	return 0;
}
