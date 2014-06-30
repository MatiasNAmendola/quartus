#ifndef SYSCALL_HPP
#define SYSCALL_HPP

#include "../../hal/hal.hpp"

namespace kernel
{
	/**
	  * @brief	Namespace for the syscalls
	  */
	namespace syscall
	{
		/**
		  * @brief	Nubers of the system calls
		  */
		enum numbers
		{
			/**
			  * @brief	Number of the syscall spawning a new process from raw image
			  */
			spawn_raw 	= 0x00,

			/**
			  * @brief	Number of the syscall spawning a new process from elf image
			  */
			spawn_elf 	= 0x01,
	
			/**
			  * @brief	Number of the syscall exiting the calling process
			  */
			exit_proc 	= 0x03,

			/**
			  * @brief	Number of the syscall killing a process
			  */
			kill_proc 	= 0x04,

			/**
			  * @brief	Number of the syscall gathering information about a process
			  */
			proc_info 	= 0x05,


			/**
			  * @brief	Number of the syscall creating a new thread within the calling process
			  */
			create_thread 	= 0x10,

			/**
			  * @brief	Number of the syscall exiting the calling thread
			  */
			exit_thread	= 0x11,

			/**
			  * @brief	Number of the syscall killing a thread
			  */
			kill_thread	= 0x12,

			/**
			  * @brief	Number of the sleep-syscall
			  */
			sleep_thread	= 0x13,

			/**
			  * @brief	Number of the syscall waking a thread
			  */
			wake_thread	= 0x14,

			/**
			  * @brief	Number of the syscall gathering information about a thread
			  */			
			thread_info	= 0x15,


			/**
			  * @brief	Number of the syscall allocating memory for the calling process
			  */
			memory_alloc	= 0x20,

			/**
			  * @brief	Number of the syscall freeing memory for the calling process
			  */
			memory_free	= 0x21,

			/**
			  * @brief	Number of the syscall gathering information about the memory
			  */	
			memory_info	= 0x22
		};

		/**
		  * @brief	Function handling a system call
		  *
		  * @param	cpu		The cpu-state of the calling process/thread used to pass infomation and parameters
		  * @return			The cpu-state used to return information
		  */
		cpu::cpu_state *handle( cpu::cpu_state *cpu );
	}

	
}

#endif
