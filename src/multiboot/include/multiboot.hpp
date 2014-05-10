#ifndef MULTIBOOT_HPP
#define MULTIBOOT_HPP

#include <cstdint>

/**
  * @brief	The multiboot namespace
  *
  *		Contains the implementation of the multiboot-standard.
  */
namespace multiboot
{
	/**
	  * @brief	Type used for error checking of the multiboot-structure
	  */
	enum error
	{
		/**
		  * @brief	Indicates that the checked multiboot-structure is ok.
		  */
		mbs_check_ok			= 0x00,
		
		/**
		  * @brief	Indicates that the checked data is not a valid multiboot-structure.
		  */
		no_mbs 				= 0x01,
		
		/**
		  * @brief	Indicates that the checked multiboot-structure does not contain a valid pointer to a module structure.
		  */
		no_mbs_mods_addr		= 0x02,

		/**
		  * @brief	Indicates that the checked multiboot-structure does not contain a valid pointer to a memory map.
		  */
		no_mbs_mmap_addr 		= 0x03
	};

	/**
	  * @brief	Multiboot-magicnumber (provided by bootloader; used to check if loaded by a compatible loader)
	  */
	static const uint32_t magic		= 0x2BADB002;

	/**
	  * @brief	Used if no command-line was provided by the bootloader.
	  *
	  *		Used to fill the 'cmdline' filed of the multiboot-structure with a valid pointer
	  */
	static const uint32_t empty_cmdline	= (uint32_t)" ";

	/**
	  * @brief	Used if no bootloader name was provided by the bootloader.
	  *		
	  *		Used to fill the 'boot_loader_name' filed of the multiboot-structure with a valid pointer
	  */
	static const uint32_t no_loader_name	= (uint32_t)"No name";

	/**
	  * @brief	The multiboot-Structure
	  *
	  *		This structure will be passed to the kernel, if loaded by a multiboot-bootloader.
	  *		Contains information about the bootloader, memory, loded modules, drives ...
	  */
	typedef struct
	{
		/**
		  * @brief	Indicates the presence and validity of other fields in the Multiboot information structure.
		  */
		uint32_t flags;

		/**
		  * @brief	Indicates the amount of lower memory in kilobytes.
		  */
		uint32_t mem_lower;

		/**
		  * @brief	Indicates the amount of upper memory in kilobytes.
		  */
		uint32_t mem_upper;

		/**
		  * @brief	Indicates which bios disk device the boot loader loaded the OS image from
		  */
		uint32_t bootdevice;

		/**
		  * @brief	Contains the physical address of the command line to be passed to the kernel.
		  */
		uint32_t cmdline;

		/**
		  * @brief	Contains the number of modules loaded by the bootloader.
		  */
		uint32_t mods_count;

		/**
		  * @brief	Contains the physical address of the first module structure.
		  */
		uint32_t mods_addr;

		/**
		  * @brief	Not used; ignored
		  *
		  * @todo	Add a better description; replace with the right structure
		  */ 
		uint32_t syms1;

		/**
		  * @brief	Not used; ignored
		  *
		  * @todo	Add a better description; replace with the right structure
		  */ 
		uint32_t syms2;
		
		/**
		  * @brief	Not used; ignored
		  *
		  * @todo	Add a better description; replace with the right structure
		  */ 
		uint32_t syms3;

		/**
		  * @brief	Not used; ignored
		  *
		  * @todo	Add a better description; replace with the right structure
		  */ 
		uint32_t syms4;

		/**
		  * @brief	Contains the total size of the memory-map.
		  */
		uint32_t mmap_length;

		/**
		  * @brief	Contains the physical address of the memory-map.
		  *		The map provided is guaranteed to list all standard ram that should be available for normal use. 
		  */
		uint32_t mmap_addr;

		/**
		  * @brief	Contains the total size of the drive-structures.
		  */
		uint32_t drives_length;

		/**
		  * @brief	Contains the physical address of the drive-structure.
		  */
		uint32_t drives_addr;

		/**
		  * @brief	Contains the address of the rom configuration table returned by the GET CONFIGURATION bios call.
		  */
		uint32_t config_table;

		/**
		  * @brief	Contains the physical address of the name of the bootloader booting the kernel.
		  *
		  *		The name is a normal C-style zero-terminated string.
		  */
		uint32_t boot_loader_name;

		/**
		  * @brief	Contains the physical address of an apm table.
		  */
		uint32_t apm_table;
	} info;

	/**
	  * @brief	The memory-map-structure
	  *
	  *		Contains information about the memory.
	  */
	typedef struct
	{
		/**
		  * @brief	Contains the size of the associated structure in bytes.
		  */
		uint32_t size;

		/**
		  * @brief	Contains the starting adress of the address range represented by the structure.
		  */
		uint64_t base;

		/**
		  * @brief	Contains the size of the address range represented by the structure in bytes.
		  */
		uint64_t length;

		/**
		  * @brief	Contains the tpe of the address range represented by the structure.
		  *		
		  *		Where a value of 1 indicates available ram, and all other values currently indicated a reserved area.
		  */
		uint32_t type;

	} mmap;

	/**
	  * @brief	The module-structure
	  *
	  *		Contains information about a loaded module.
	  */
	typedef struct
	{
		/**
		  * @brief	The start address of the module
		  */
		uint32_t mod_start;

		/**
		  * @brief	The end address of the module
		  */
		uint32_t mod_end;

		/**
		  * @brief	Provides an arbitrary string to be associated with the particular boot module
		  */
		uint32_t string;

		/**
		  * @brief	Reserved; ignored
		  */
		uint32_t reserved;
	} mods;

	/**
	  * @brief	The drive-structure
	  */
	typedef struct
	{
		/**
		  * @brief	Contains the size of this structure.
		  */
		uint32_t size;

		/**
		  * @brief	Contains the BIOS drive number.
		  */
		uint8_t	 number;

		/**
		  * @brief	Represents the access mode used by the boot loader.
		  *
		  *		0 CHS mode (traditional cylinder/head/sector addressing mode)
		  *		1 LBA mode (Logical Block Addressing mode)
		  */
		uint8_t  mode;

		/**
		  * @brief	Contains the number of the cylinders.
		  */
		uint16_t cylinders;

		/**
		  * @brief	Contains the number of the heads.
		  */
		uint8_t  heads;

		/**
		  * @brief	Contains the number of the sectors per .
		  */
		uint8_t  sectors;
	
		/**
		  * @todo	drive ports
		  */	
	} drives;

	/**
	  * @brief	Checks the given multiboot-structure for validity.
	  *
	  *		Checks, if the fileds 'mods_addr' and 'mmap_addr' are set.
	  *		Checks, if a valid command-line name is set. If not, the function sets a valid(empty) command-line.
	  *		Checks, if a valid bootloader name is set. If not, the function sets a valid name.
	  *
	  * @param	mbs		Pointer to the structure to be checked
	  * @return			Returns the result of the check
	  */
	error check_mbs( info *mbs );
}

#endif
