#ifndef CMOS_HPP
#define CMOS_HPP

#include <cstdint>
#include <ctime>

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Namespace containing functions to work with the CMOS
		  */
		namespace cmos
		{
			/**
			  * @brief	IO-port of the cmos address register
			  */
			const uint8_t addr_port 	= 0x70;

			/**
			  * @brief	IO-port of the cmos data register
			  */
			const uint8_t data_port 	= 0x71;

			/**
			  * @brief	Address of the cmos-field containing the current second
			  */
			const uint8_t addr_sec  	= 0x00;

			/**
			  * @brief	Address of the cmos-field containing the current minute
			  */
			const uint8_t addr_min  	= 0x02;

			/**
			  * @brief	Address of the cmos-field containing the current hour
			  */
			const uint8_t addr_hour 	= 0x04;

			/**
			  * @brief	Address of the cmos-field containing the current day
			  */
			const uint8_t addr_day  	= 0x07;

			/**
			  * @brief	Address of the cmos-field containing the current month
			  */
			const uint8_t addr_month	= 0x08;

			/**
			  * @brief	Address of the cmos-field containing the current year
			  */
			const uint8_t addr_year		= 0x09;

			/**
			  * @brief	Address of the cmos-field containing the current century
			  */
			const uint8_t addr_century	= 0x32;
		
			/**
			  * @brief	Writes a value to a cmos-field
			  *
			  * @param	addr		Adressof the cmos-field to be written
			  * @param	value		Value to be written
			  */
			void write( uint8_t addr, uint8_t value );

			/**
			  * @brief	Reads a value from a cmos-field
			  *
			  * @param	addr		Adressof the cmos-field to be read
			  * @return			Read value
			  */
			uint8_t read( uint8_t addr );

			/**
			  * @brief	Reads the time from the cmos
			  *
			  * @param	hour		Pointer to an integer; the hour is stored there
			  * @param	min		Pointer to an integer; the minute is stored there
			  * @param	sec		Pointer to an integer; the second is stored there
			  */
			void time( time_t *hour, time_t *min, time_t *sec );

			/**
			  * @brief	Reads the date from the cmos
			  *
			  * @param	year		Pointer to an integer; the year is stored there
			  * @param	month		Pointer to an integer; the month is stored there
			  * @param	day		Pointer to an integer; the day is stored there
			  */
			void date( time_t *year, time_t *month, time_t *day );
		}				
	}
}

#endif
