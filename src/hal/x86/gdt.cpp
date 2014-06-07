#include "include/gdt.hpp"

using namespace hal::x86;

/*
Constructor; initialises the gdt with 0
*/
gdt::gdt(  )
{
	for(size_t index = 0; index < gdt::descriptor_count; index++)
	{
		gdt::descriptor_table[index] = 0;
	}
}

/*
Destructor
*/
gdt::~gdt(  )
{

}

/*
Initialises the GDT; Nulldescriptor, Kernel-code/Kernel-data, User-code/user-data, tss (for multitasking)
*/
void gdt::init(  )
{
	//Nulldescriptor	
	this->set_entry(0, 0, 0, 0); 

	//Codsegment Ring0				
	this->set_entry(1, 0, 0xfffff, (gdt::segment | gdt::bit32 | gdt::codeseg | gdt::gran4k | gdt::present | gdt::ring0));

	//Datasegment Ring0			
	this->set_entry(2, 0, 0xfffff, (gdt::segment | gdt::bit32 | gdt::dataseg | gdt::gran4k | gdt::present | gdt::ring0));

	//Codsegment Ring3				
	this->set_entry(3, 0, 0xfffff, (gdt::segment | gdt::bit32 | gdt::codeseg | gdt::gran4k | gdt::present | gdt::ring3));
	
	//Datasegment Ring3				
	this->set_entry(4, 0, 0xfffff, (gdt::segment | gdt::bit32 | gdt::dataseg | gdt::gran4k | gdt::present | gdt::ring3));				
	
	//TSS
	this->set_entry(5, (uintptr_t)(this->tss().tss), sizeof(tss_t), (gdt::tss386 | gdt::present | gdt::ring3));
}

/*
Loads the GDT
*/
void gdt::load(  )
{
	struct 
	{
   		uint16_t limit;
    		void	 *pointer;
	} PACKED gdtp;
	
	gdtp.limit 	= gdt::descriptor_count * 8 - 1;
    	gdtp.pointer 	= gdt::descriptor_table;

	asm volatile("lgdt %0" : : "m" (gdtp));

	asm volatile("mov $0x10, %ax");
	asm volatile("mov %ax, %ds  ");
	asm volatile("mov %ax, %es  ");
	asm volatile("mov %ax, %fs  ");
	asm volatile("mov %ax, %gs  ");
	asm volatile("mov %ax, %ss  ");
	asm volatile("ljmp $0x8, $.1");
	asm volatile(".1:           ");

	asm volatile("ltr %%ax" : : "a" (5 << 3));
}

/*
Sets a GDT-entry
*/
void gdt::set_entry( size_t index, uintptr_t base, size_t limit, uint32_t flags )
{
	gdt::descriptor_table[index] = limit & 0xffffLL;
	gdt::descriptor_table[index] |= (base & 0xffffffLL) << 16;
	gdt::descriptor_table[index] |= (flags & 0xffLL) << 40;
	gdt::descriptor_table[index] |= ((limit >> 16) & 0xfLL) << 48;
	gdt::descriptor_table[index] |= ((flags >> 8 )& 0xffLL) << 52;
	gdt::descriptor_table[index] |= ((base >> 24) & 0xffLL) << 56;
}
