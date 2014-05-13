/*
 * Kernel entry point
 */
ENTRY(start)

/*
 * Set the order of the sections
 */
SECTIONS
{
	/*
	 * Set first section to 1 MiB
	 */
	. = 0x8000;

	kernel_start = .;
	text_start = .;
	.text : 
	{
		KEEP(*(.text.boot))
		*(.text)
	}
	. = ALIGN(4096);
	text_end = .;

	data_start = .;
	.data ALIGN(4096) : 
	{
		start_ctors = .;
		KEEP(*( .init_array ));
		KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
		end_ctors = .;

		*(.data)
	}
	. = ALIGN(4096);
	data_end = .;

	rodata_start = .;
	.rodata ALIGN(4096) : 
	{
		*(.rodata)
	}
	. = ALIGN(4096);
	rodata_end = .;

	bss_start = .;
	.bss ALIGN(4096) :
	{
		*(.bss)
    	}
	. = ALIGN(4096);
	bss_end = .;

	kernel_end = .;
}
