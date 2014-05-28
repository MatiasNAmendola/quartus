#ifndef PIT_HPP
#define PIT_HPP

#include <cstdint>
#include <ctime>

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Namespace containing functions to work with the Programmable Interval Timer
		  */
		namespace pit
		{
			/**
			  * @brief 	Enumeration-type for the different pit channels
			  */
			enum channel
			{
				/**
				  * @brief	pit-channel 0
				  */
				channel_0 = 0x0,

				/**
				  * @brief	pit-channel 0
				  */
				channel_1 = 0x1,

				/**
				  * @brief	pit-channel 0
				  */
				channel_2 = 0x2	
			};

			/**
			  * @brief 	Enumeration-type for the different modes the pit can operate at
			  */
			enum mode
			{
				interrupt_on_terminal_count	= 0x0,
				one_shot			= 0x1,
				rate				= 0x2,
				square_wave			= 0x3,
				software_triggered_strobe 	= 0x4,
				hardware_triggered_strobe 	= 0x5
			};

			/**	
			  * @brief	The internal frequency the of the PIT
			  */
			const time_t	internal_freq 	= 1193182;

			/**
			  * @brief	IO-port of the counter register of the pit-channel 0
			  */	
			const uint8_t	counter_ch0 	= 0x40;

			/**
			  * @brief	IO-port of the counter register of the pit-channel 1
			  */
			const uint8_t	counter_ch1 	= 0x41;
			
			/**
			  * @brief	IO-port of the counter register of the pit-channel 2
			  */
			const uint8_t	counter_ch2 	= 0x42;

			/**
			  * @brief	IO-port of the init-register of the pit
			  */
			const uint8_t	init_port	= 0x43;
	
			/**
			  * @brief	Initialises the Programmable Interval Timer
			  *
			  * @param	ch		The channel the pit is initialised for
			  * @param	md		The mode the pit is initialised with
			  * @param	freq		The frequency the pit uses to firer IRQs
			  */
			void init( channel ch, mode md, time_t freq );
		}
	}
}

#endif
