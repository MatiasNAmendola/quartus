#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <list.hpp>
#include <definitions.hpp>

#include "thread.hpp"

#include "syscall.hpp"

namespace kernel
{
	/**
	  * @brief	The scheduler
	  */
	class scheduler
	{
		friend cpu::cpu_state *syscall::handle( cpu::cpu_state *cpu );

		SINGLETON(scheduler)

		public:
			/**
			  * @brief	Adds an thread to the schedulers ready queue
			  *
			  * @param	thrd		Thread to be added
			  */
			void add( thread *thrd );

			/**
			  * @brief	Removes an thread from the schedulers ready queue
			  *
			  * @param	thrd		Thread to be removed
			  */
			void remove( thread *thrd );

			/**
			  * @brief	Performs a thread-switch
			  *
			  *		Saves the state of the currently executed thread and switches to the next thread
			  *
			  * @param	cpu		The current cpu-state (state of the current thread)
			  * @return			The new cpu-state (state of the new thread)
			  */
			cpu::cpu_state *schedule( cpu::cpu_state *cpu );

		private:
			/**
			  * @brief	The schedulers ready queue; contains all thread ready for execution
			  */
			tools::list<thread>		ready;

			/**
			  * @brief	The currently executed thread
			  */
			thread				*running;

			/**
			  * @brief	The virtual memory context of the currently executed thread
			  */
			context				*cntxt;
	};
}

#endif
