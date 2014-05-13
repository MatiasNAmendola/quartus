#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <definitions.hpp>

#include <cstdint>
#include <cstddef>

namespace hal
{
	namespace rpi
	{
		/**
		  * @brief 	A simple serial driver; Textmode used for output
		  *
		  *		Similar to the class 'ostream' of th c++ standard library.
		  */
		class serial
		{	
			public:
				/**
				  * @brief 	Type for format flags
				  *		
				  *		Defines the type used for format flags.
				  *		Format flags are used to set the base used for number output.
				  */ 
				typedef uint32_t fmtflags_t;



				/**
				  * @brief	Format flag for decimal output
				  *
				  */
				static const fmtflags_t dec_		= 0x1;

				/**
				  * @brief	Format flag for hexadecimal output
				  *
				  */
				static const fmtflags_t hex_		= 0x2;

				/**
				  * @brief	Format flag for octal output
				  *
				  */
				static const fmtflags_t oct_		= 0x4;

				/**
				  * @brief	Bitmask of the basefield
				  *
				  *		The basefield is the part of the format flags, holding information about the base.
				  */
				static const fmtflags_t basefield_	= 0x7;

				/**
				  * @brief	Width of the tabulator ('\\t')
				  */
				static const size_t tab_width		= 8;



				/**
				  * @brief 	Standard constructor
				  */
				serial(  );

				

				/**
				  * @brief 	Get/set format flags
				  *
				  * @return			format-flags
				  */
				fmtflags_t flags(  );

				/**
				  * @brief 	Get/set format flags
				  *
				  * @param	fmtfl		format-flags
				  * @return			format-flags (before call)
				  *	
				  */
				fmtflags_t flags( fmtflags_t fmtfl );

				/**
				  * @brief 	Sets specific format-flags
				  *
				  * @param	fmtfl		Format flags to be set
				  * @return			format-flags (before call)
				  */
				fmtflags_t setf( fmtflags_t fmtfl );

				/**
				  * @brief 	Sets specific format-flags
				  *
				  * @param	fmtfl		Format flags to be set
				  * @param	mask		Bitmask containing the flags to be modified
				  * @return			format-flags (before call)
				  */
				fmtflags_t setf( fmtflags_t fmtfl, fmtflags_t mask );

				/**
				  * @brief 	Clear specific format flags
				  *
				  * @param	mask		Bitmask specifying the flags to be cleared
				  * @return			format-flags (before call)	
				  */
				void unsetf( fmtflags_t mask );

	

				/**
				  * @brief	Sets the field width
				  *
				  * @param	wide		Number of characters to be used as field width.
				  * @return			field width before call
				  */
				size_t setw( size_t wide );

				

				/**
				  * @brief 	Put character
				  *
				  * @param	c		Character to write
				  */
				void putc( char c );

				/**
				  * @brief 	Put number (integer; long)
				  *
				  * @param	n		number to write
				  */
				void putn( long n );

				/**
				  * @brief 	Put string (cstring; const char *)
				  *
				  * @param	s		String to write
				  */
				void puts( const char *s );



				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	c		Character to write
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( char c );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	c		Character to write
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( unsigned char c );
			
				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; short)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( short n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; unsigned short)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( unsigned short n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; int)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( int n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; unsigned int)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( unsigned int n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; long)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( long n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; unsigned long)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( unsigned long n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @todo
				  *
				  * @param	n		Number to write (integer; long long)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( long long n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @todo
				  *
				  * @param	n		Number to write (integer; unigned long long)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( unsigned long long n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  *		Used for 'nullptr'
				  *
				  * @param	p		Pointer to write
				  * @return			The 'serial' object (*this)
				  */	
				serial &operator << ( nullptr_t p );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	p		Pointer to write
				  * @return			The 'serial' object (*this)
				  */	
				serial &operator << ( void *p );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	s		String to write (cstring; const char *)
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( const char *s );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  *		Used for manipulators.
				  *		Calls pf(*this)
				  *
				  * @param	pf		A function that takes and returns a 'serial' object
				  * @return			The 'serial' object (*this)
				  */
				serial &operator << ( serial &(*pf)(serial &ser) ) { return pf(*this); }	


			
				/**
				  * @brief	Insert newline
				  *
				  * @param	ser		'serial' object affected
				  * @return			The 'serial' object argument (ser)
				  */
				static serial &endl( serial &ser ) { ser << '\n'; return ser; }

				/**
				  * @brief	Insert null character
				  *
				  * @param	ser		'scrn' object affected
				  * @return			The 'scrn' object argument (ser)
				  */
				static serial &ends( serial &ser ) { ser << '\0'; return ser; }

				/**
				  * @brief	Use decimal base
				  *
				  * @param	ser		'serial' object affected
				  * @return			The 'serial' object argument (ser)
				  */
				static serial &dec( serial &ser ) { ser.setf(serial::dec_, serial::basefield_); return ser; }

				/**
				  * @brief	Use hexadecimal base
				  *
				  * @param	ser		'output' object affected
				  * @return			The 'serial' object argument (ser)
				  */
				static serial &hex( serial &ser ) { ser.setf(serial::hex_, serial::basefield_); return ser; }

				/**
				  * @brief	Use octal base
				  *
				  * @param	ser		'output' object affected
				  * @return			The 'serial' object argument (ser)
				  */
				static serial &oct( serial &ser ) { ser.setf(serial::oct_, serial::basefield_); return ser; }

			
				static const uint32_t buffer_full = (1 << 5);

				/**
				  * @brief	Serial base register
				  */
				static const uint32_t base_register 	= 0x20201000;

				/**
				  * @brief	Serial data register
				  */
				static const uint32_t data_register 	= base_register + 0x00;

				static const uint32_t rsrecr 		= base_register + 0x04;

				/**
				  * @brief	Serial flag register
				  */
				static const uint32_t flag_register 	= base_register + 0x18;

				static const uint32_t ilpr 		= base_register + 0x20;
				static const uint32_t ibrd 		= base_register + 0x24;
				static const uint32_t fbrd 		= base_register + 0x28;
				static const uint32_t lcrh 		= base_register + 0x2C;
				static const uint32_t cr 		= base_register + 0x30;
				static const uint32_t ifls 		= base_register + 0x34;
				static const uint32_t imsc 		= base_register + 0x38;
				static const uint32_t ris 		= base_register + 0x3C;
				static const uint32_t mis 		= base_register + 0x40;
				static const uint32_t icr 		= base_register + 0x44;
				static const uint32_t dmacr 		= base_register + 0x48;
				static const uint32_t itcr 		= base_register + 0x80;
				static const uint32_t itip 		= base_register + 0x84;
				static const uint32_t itop 		= base_register + 0x88;
				static const uint32_t tdr 		= base_register + 0x8C;

			private:
				/**
				  * @brief 	format-flag filed
				  * 
				  * 		Hold information about the base used for number output.
				  */
				fmtflags_t	fmtflags_;

				/**
				  * @brief	Width of the next formatted element to insert.
				  */
				size_t		field_width;
		};

		extern serial seri;
	}
}

#endif
