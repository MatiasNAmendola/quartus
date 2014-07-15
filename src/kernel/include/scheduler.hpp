#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <list.hpp>
#include <definitions.hpp>

#include "thread.hpp"
#include "timer.hpp"

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
			  * @brief	Removes an thread from the schedulers ready queue and put it into the waiting queue
			  *
			  * @param	thrd		Thread to be blocked
			  */
			void block( thread *thrd );

			/**
			  * @brief	Removes an thread from the schedulers waiting queue and put it into the ready queue
			  *
			  * @param	thrd		Thread to be unblocked
			  */
			void unblock( thread *thrd );

			/**
			  * @brief	Removes an thread from the schedulers ready queue and put it into the sleeping queue
			  *
			  * @param	thrd		Thread to be blocked for a given time
			  * @param	msec		Milliseconds to sleep
			  */
			void sleep( thread *thrd, time_t msec );

			/**
			  * @brief	Removes an thread from the schedulers sleeping queue and put it into the ready queue
			  *
			  * @param	thrd		Thread to be waked
			  */
			void wake( thread *thrd );

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
			  * @brief	The schedulers waiting queue; contains all thread waiting for a specific event
			  */
			tools::list<thread>		waiting;

			/**
			  * @brief	The schedulers sleeping queue; contains all thread sleeping
			  */
			tools::list<thread>		sleeping;


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
