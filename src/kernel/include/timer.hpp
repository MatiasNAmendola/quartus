#ifndef TIMER_HPP
#define TIMER_HPP

#include <definitions.hpp>

#include <cstdint>
#include <ctime>

#include "../hal/hal.hpp"

namespace kernel
{
	/**
	  * @brief	Namespace containing the system-timer and other timer functions
	  */
	namespace timer
	{		
		/**
		  * @brief	Initialises the system-timer
		  *
		  * @param	irq		The IRQ used by the timer to count ticks
		  * @param	current		The current time
		  * @param	freq		The frequency used by the timer
		  */
		void init( uint8_t irq, time_t current, time_t freq );

		/**
		  * @brief	Function executed every time the timer-irq gets fired
		  *
		  * @param	cpu		The cpu state
		  * @return			The (modified) cpu state
		  */
		cpu::cpu_state *tick( cpu::cpu_state *cpu );
	
		/**
		  * @brief	Counts the timer ticks; increases every time the timer-irq fires.
		  */
		extern time_t 	ticks;
			
		/**
		  * @brief	Counts the seconds since timer initialisation.
		  */
		extern time_t	seconds;

		/**
		  * @brief	The current time as unix-time
		  */
		extern time_t	time;

		/**
		  * @brief	The frequency the timer uses
		  */
		extern time_t	freq;
	}
}

#endif
