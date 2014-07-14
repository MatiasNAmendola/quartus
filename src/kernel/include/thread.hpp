#ifndef THREAD_HPP
#define THREAD_HPP

#include "../../hal/hal.hpp"

#include "process.hpp"

#include "syscall.hpp"

namespace kernel
{
	namespace ipc
	{
		class service;
	}

	/**
	  * @brief	A thread
	  */
	class thread
	{
		friend class process;
		friend class scheduler;
		friend class threadmgr;

		friend class ipc::service;

		friend cpu::cpu_state *syscall::handle( cpu::cpu_state *cpu );

		public:
			/**
			  * @brief	Type used for thread-ids.
			  */
			typedef size_t		id_t;

			/**
			  * @brief	The state of a thread
			  */
			enum state_t
			{
				/**
				  * @brief	Thread is running
				  */
				running = 0,

				/**
				  * @brief	Thread is ready to run
				  */
				ready 	= 0,
	
				/**
				  * @brief	Thread is sleeping, waiting for time
				  */
				sleeping,

				/**
				  * @brief	Thread is waiting for an specific event
				  */
				waiting
			};

			/**
			  * @brief	Information structure describing a thread.
			  */
			struct info_t
			{
				/**
				  * @brief	The thread id
				  */
				id_t		id;

				/**
				  * @brief	The state of the thread
				  */
				state_t		state;

				/**
				  * @brief	The flags of the thread
				  */
				size_t		flags;
			};
			
		private:
			

		public:
			/**
			  * @brief	The constructor
			  * 
			  * @param	proc		The Process, the thread will be part of
			  * @param	entry		The entry point of the new thread
			  * @param	flags		Flag; used for thread specialisation
			  */
			thread( process *proc, uintptr_t entry, size_t flags );

			/**
			  * @brief	The constructor
			  * 
			  * @param	proc		The Process, the thread will be part of
			  * @param	entry		The entry point of the new thread
			  * @param	args		Arguments passed to the new thread
			  * @param	flags		Flag; used for thread specialisation
			  */
			thread( process *proc, uintptr_t entry, uintptr_t args, size_t flags );

			/**
			  * @brief	The destructor
			  */
			~thread(  );

			/**
			  * @brief	Get information about the thread
			  * 
			  * @return			Thread information structure containing information on the thread.
			  */
			info_t info(  );

			/**
			  * @brief	Saves the cpu-state to the thread
			  */
			void save( cpu::cpu_state *cpu );
		
			/**
			  * @brief	Restores the cpu-state from the thread
			  */
			cpu::cpu_state *restore(  );

			/**
			  * @brief	Size of the kernel-space stack
			  */
			static const size_t	kstack_size 	= memory::page_size_byte;

			/**
			  * @brief	Size of the user-space stack
			  */
			static const size_t	ustack_size 	= memory::page_size_byte * 4;

			/**
			  * @brief	Flag used for kernel-space threads
			  */
			static const size_t	kernel		= 0x01;

			/**
			  * @brief	Flag used for user-space threads
			  */
			static const size_t	user		= 0x02;

		private:
			/**
			  * @brief	Generates a new thread-id.
			  *
			  * @return			A new, unused thread-id
			  */
			static id_t		new_id(  ) { static id_t nid; return nid++; }

			/**
			  * @brief	The thread-id
			  */
			id_t			id;

			/**
			  * @brief	The state of the thread
			  */
			state_t			state;

			/**
			  * @brief	Flags of the thread
			  */
			size_t			flags;	

			/**
			  * @brief	Timer used for sleeping
			  */
			time_t			sleep;	

			/**
			  * @brief	The cpu-state of the thread
			  */
			cpu::cpu_state		*cpu;

			/**
			  * @brief	The user-space stack of the thread
			  */
			uintptr_t		ustack;

			/**
			  * @brief	The kernel-space stack of the thread
			  */
			uintptr_t		kstack;

			/**
			  * @brief	The process the thread belongs to
			  */
			process			*proc;
	};

	/**
	  * @brief	The thread manager
	  */
	class threadmgr
	{
		SINGLETON(threadmgr)

		public:
			/**
			  * @brief	Adds a thread to the managers list
			  *
			  * @param	thrd		The thread to be added
			  */
			void add( thread *thrd );

			/**
			  * @brief	Removes a thread from the managers list
			  *
			  * @param	thrd		The thread to be removes
			  */
			void remove( thread *thrd );

			/**
			  * @brief	Gets a thread from the managers list
			  *
			  * @param	id		The id of the thread
			  */	
			thread *get( thread::id_t id );


			/**
			  * @brief	Gets a thread from the managers list
			  *
			  * @param	thrd		The thread to get
			  */	
			thread *get( thread *thrd );

		private:
			/**
			  * @brief	List containing all threads managed by the thread manager
			  */
			tools::list<thread> 	thrds;
	};
}

#endif
