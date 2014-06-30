#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <list.hpp>

#include "../../hal/hal.hpp"

#include "syscall.hpp"

namespace kernel
{
	class thread;

	/**
	  * @brief	A Process
	  */
	class process
	{
		friend class thread;
		friend class scheduler;
		friend class processmgr;

		friend cpu::cpu_state *syscall::handle( cpu::cpu_state *cpu );

		public:
			/**
			  * @brief	Type used for process-ids.
			  */
			typedef size_t		id_t;

		private:

		public:
			/**
			  * @brief	The constructor
			  *
			  * @param	name		Name of the new process
			  * @param	cmdline		Commandline passed to the new process
			  * @param	parent		Process-id of the parent process
			  * @param	cntxt		Virtual memory context containing the process-image
			  */
			process( const char name[64], const char cmdline[1024], process::id_t parent, context *cntxt );
			
			/**
			  * @brief	The destructor
			  */
			~process(  );

		private:
			/**
			  * @brief	Generates a new process-id.
			  *
			  * @return			A new, unused process-id
			  */
			static id_t		new_id(  ) { static id_t nid; return nid++; }

			/**
			  * @brief	The process-id
			  */
			id_t			id;

			/**
			  * @brief	The process-id of the parent process
			  */
			id_t			parent;


			/**
			  * @brief	Name of the process
			  */
			char			name[64];

			/**
			  * @brief	Commandline of the process
			  */
			char			cmdline[1024];

			/**
			  * @brief	The virtual memory context of the process
			  */
			context			*cntxt;

			/**
	  		  * @brief	The 'version' of the processs memory context
	  		  */
			size_t			memory_version;
			
			/**
			  * @brief	The virtual memory manager of the process
			  */
			vmm			*memmgr;


			/**
			  * @brief	List containing all threads started by the process
			  */
			tools::list<thread>	thrds;
	};

	/**
	  * @brief	The process manager
	  */
	class processmgr
	{
		SINGLETON(processmgr)

		public:
			/**
			  * @brief	Adds a process to the managers list
			  *
			  * @param	proc		The process to be added
			  */
			void add( process *proc );

			/**
			  * @brief	Removes a process from the managers list
			  *
			  * @param	proc		The process to be removes
			  */
			void remove( process *proc );

			/**
			  * @brief	Gets a process from the managers list
			  *
			  * @param	id		The id of the process
			  */	
			process *get( process::id_t id );

			/**
			  * @brief	Gets a process from the managers list
			  *
			  * @param	name		The name of the process
			  *
			  * @todo			Problem if multiple processes has got the same name
			  */
			process *get( const char name[64] );

			/**
			  * @brief	Gets a process from the managers list
			  *
			  * @param	proc		The process to get
			  */	
			process *get( process *proc );

		private:
			/**
			  * @brief	List containing all processes managed by the process manager
			  */
			tools::list<process> 	procs;
	};
}

#endif
