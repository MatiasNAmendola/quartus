#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <definitions.hpp>

#include <cstdint>
#include <cstddef>

namespace hal
{
	namespace x86
	{
		/**
		  * @brief	Namespace for video-colors
		  *
		  *		Contains definitions of Fore- and Background colors.
		  */
		namespace color
		{		
			/**
			  * @brief	Type used for colors
			  */			
			enum color_t
			{
				black 		= 0x0,
				blue 		= 0x1,		
				green 		= 0x2,
				cyan 		= 0x3,
				red 		= 0x4,
				magenta 	= 0x5,
				brown 		= 0x6,
				light_gray 	= 0x7,
				dark_gray 	= 0x8,
				light_blue 	= 0x9,
				light_green 	= 0xA,
				light_cyan 	= 0xB,
				light_red 	= 0xC,
				light_magenta 	= 0xD,
				yellow 		= 0xE,
				white 		= 0xF
			};

			/**
			  * @brief	Type used for background colors
			  *
			  *		Used to differentiate betwen fore- and background.
			  */
			struct background
			{
				/**
				  * @brief	The color
				  */
				color_t	color;
			
				/**
				  * @brief	Constructor
				  */
				inline background( color_t pColor ) : color(pColor) {  }; 
			};
		}

		/**
		  * @brief 	A simple screen driver; Textmode used for output
		  *
		  *		Similar to the class 'ostream' of th c++ standard library.
		  */
		class video
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
				  * @brief	Height of the screen.
				  */
				static const size_t height		= 25;

				/**
				  * @brief	Width of the screen.
				  */
				static const size_t width		= 80;

				/**
				  * @brief	Width of the tabulator ('\\t')
				  */
				static const size_t tab_width		= 8;



				/**
				  * @brief 	Standard constructor
				  */
				video(  );

				

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
				  * @brief	Sets the foreground color
				  *
				  * @param	color		The color to use
				  */
				void set_color( color::color_t color );

				/**
				  * @brief	Sets the background color
				  *
				  * @param	color		The color to use
				  */
				void set_color( const color::background &color );
				


				/**
				  * @brief	Clears the screen
				  */
				void clear(  );

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
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( char c );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	c		Character to write
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( unsigned char c );
			
				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; short)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( short n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; unsigned short)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( unsigned short n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; int)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( int n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; unsigned int)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( unsigned int n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; long)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( long n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	n		Number to write (integer; unsigned long)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( unsigned long n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @todo
				  *
				  * @param	n		Number to write (integer; long long)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( long long n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @todo
				  *
				  * @param	n		Number to write (integer; unigned long long)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( unsigned long long n );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  *		Used for 'nullptr'
				  *
				  * @param	p		Pointer to write
				  * @return			The 'video' object (*this)
				  */	
				video &operator << ( nullptr_t p );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	p		Pointer to write
				  * @return			The 'video' object (*this)
				  */	
				video &operator << ( void *p );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	s		String to write (cstring; const char *)
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( const char *s );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  *		Used for manipulators.
				  *		Calls pf(*this)
				  *
				  * @param	pf		A function that takes and returns a 'video' object
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( video &(*pf)(video &scrn) ) { return pf(*this); }	

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	color		Foreground color to set
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( color::color_t color );

				/**
				  * @brief 	Overloaded operator for output
				  *
				  * @param	color		Background color to set
				  * @return			The 'video' object (*this)
				  */
				video &operator << ( const color::background &color );


			
				/**
				  * @brief	Insert newline
				  *
				  * @param	scrn		'video' object affected
				  * @return			The 'video' object argument (scrn)
				  */
				static video &endl( video &scrn ) { scrn << '\n'; return scrn; }

				/**
				  * @brief	Insert null character
				  *
				  * @param	scrn		'scrn' object affected
				  * @return			The 'scrn' object argument (scrb)
				  */
				static video &ends( video &scrn ) { scrn << '\0'; return scrn; }

				/**
				  * @brief	Use decimal base
				  *
				  * @param	scrn		'video' object affected
				  * @return			The 'video' object argument (out)
				  */
				static video &dec( video &scrn ) { scrn.setf(video::dec_, video::basefield_); return scrn; }

				/**
				  * @brief	Use hexadecimal base
				  *
				  * @param	scrn		'output' object affected
				  * @return			The 'video' object argument (out)
				  */
				static video &hex( video &scrn ) { scrn.setf(video::hex_, video::basefield_); return scrn; }

				/**
				  * @brief	Use octal base
				  *
				  * @param	scrn		'output' object affected
				  * @return			The 'video' object argument (out)
				  */
				static video &oct( video &scrn ) { scrn.setf(video::oct_, video::basefield_); return scrn; }

			private:		
				/**
				  * @brief	Pointer to the video-memory
				  */
				uint8_t 	*videomem = (uint8_t*)0xB8000;

				/**
				  * @brief	The color used to print characters
				  */
				uint8_t 	color;				

				/**
				  * @brief	The horizontal-position on the screen
				  */
				size_t 		pos_x;

				/**
				  * @brief	The vertical-position on the screen
				  */	
				size_t 		pos_y;
			
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

		extern video screen;
	}
}

#endif
