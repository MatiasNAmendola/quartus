#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <cstdint>
#include <cstddef>

namespace kernel
{
	/**
	  * @brief 	The standard output-class of the kernel
	  *
	  *		Similar to the class 'ostream' of th c++ standard library.
	  */
	class output
	{	
		public:
			/**
			  * @brief 	Type for a funtion printing a char
			  *		
			  *		Defines the function-pointer-type for printing a char.
			  *		Used as parameter to pass funtions.
			  *			e.g. the 'output' class uses an object of this type for character-output
			  */
			typedef void (*outc_t)( char c );

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
			  * @brief 	Standard constructor
			  */
			output(  );

			/**
			  * @brief 	Constructor
			  * 
			  * @param	pf		Pointer to a function used for character-output
			  */
			output( outc_t pf );



			/**
			  * @brief 	Set the private-member out.
			  * 		
			  * @param	pf		Pointer to a function used for character-output
			  */
			void seto( outc_t pf );

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
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( char c );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	c		Character to write
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( unsigned char c );
			
			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	n		Number to write (integer; short)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( short n );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	n		Number to write (integer; unsigned short)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( unsigned short n );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	n		Number to write (integer; int)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( int n );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	n		Number to write (integer; unsigned int)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( unsigned int n );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	n		Number to write (integer; long)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( long n );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	n		Number to write (integer; unsigned long)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( unsigned long n );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @todo
			  *
			  * @param	n		Number to write (integer; long long)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( long long n );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @todo
			  *
			  * @param	n		Number to write (integer; unigned long long)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( unsigned long long n );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  *		Used for 'nullptr'
			  *
			  * @param	p		Pointer to write
			  * @return			The 'output' object (*this)
			  */	
			output &operator << ( nullptr_t p );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	p		Pointer to write
			  * @return			The 'output' object (*this)
			  */	
			output &operator << ( void *p );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  * @param	s		String to write (cstring; const char *)
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( const char *s );

			/**
			  * @brief 	Overloaded operator for output
			  *
			  *		Used for manipulators.
			  *		Calls pf(*this)
			  *
			  * @param	pf		A function that takes and returns a 'output' object
			  * @return			The 'output' object (*this)
			  */
			output &operator << ( output &(*pf)(output &out) ) { return pf(*this); }
		

			
			/**
			  * @brief	Insert newline
			  *
			  * @param	out		'output' object affected
			  * @return			The 'output' object argument (out)
			  */
			static output &endl( output &out ) { out << '\n'; return out; }

			/**
			  * @brief	Insert null character
			  *
			  * @param	out		'output' object affected
			  * @return			The 'output' object argument (out)
			  */
			static output &ends( output &out ) { out << '\0'; return out; }

			/**
			  * @brief	Use decimal base
			  *
			  * @param	out		'output' object affected
			  * @return			The 'output' object argument (out)
			  */
			static output &dec( output &out ) { out.setf(output::dec_, output::basefield_); return out; }

			/**
			  * @brief	Use hexadecimal base
			  *
			  * @param	out		'output' object affected
			  * @return			The 'output' object argument (out)
			  */
			static output &hex( output &out ) { out.setf(output::hex_, output::basefield_); return out; }

			/**
			  * @brief	Use octal base
			  *
			  * @param	out		'output' object affected
			  * @return			The 'output' object argument (out)
			  */
			static output &oct( output &out ) { out.setf(output::oct_, output::basefield_); return out; }

		private:
			/**
			  * @brief 	Pointer to the function used for character-output
			  * 
			  * 		Internally used for all output operations.
			  *		Must be set for each 'output' object
			  */
			outc_t 		out_;
			
			/**
			  * @brief 	format-flag filed
			  * 
			  * 		Hold information about the base used for number output.
			  *		
			  */
			fmtflags_t	fmtflags_;
	};

	extern output kout;
}

#endif
