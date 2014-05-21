#include "include/output.hpp"

/*
Includes from 'hal'
*/
#include "../hal/hal.hpp"

using namespace kernel;

/**
  * @brief	The standard-output of the kernel
  *
  *		The standard-output is defined with a function (c++ lambda-function) putting a char to 'screen' or 'seri' (defined in 'hal').
  *		If compiled with 'QEMU_DEBUG' option, the char is also send to the qemu serial port.
  */
#if defined(OUTPUT_SCREEN) && defined(QEMU_DEBUG) 
output kernel::kout = output( []( char c ){ screen.putc(c); io::outb(definitions::qemu_serial, c); } );
#elif defined(OUTPUT_SCREEN)
output kernel::kout = output( []( char c ){ screen.putc(c); } );
#elif defined(OUTPUT_SERIAL)
output kernel::kout = output( []( char c ){ seri.putc(c); } );
#endif

/*
Constructors
*/
output::output(  ) : out_(nullptr)
{
	this->setf(output::dec_, output::basefield_);
}

output::output( outc_t pf ) : out_(pf)
{
	this->setf(output::dec_, output::basefield_);
}

/*
Set output-method
*/
void output::seto( outc_t pf )
{
	this->out_ = pf;
}

/*
Set/Get fmtflags
*/
output::fmtflags_t output::flags(  )
{
	return this->fmtflags_;
}

output::fmtflags_t output::flags( output::fmtflags_t fmtfl )
{
	output::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl;

	return tmp;
}

output::fmtflags_t output::setf( output::fmtflags_t fmtfl )
{
	output::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl | this->fmtflags_;

	return tmp;
}

output::fmtflags_t output::setf( output::fmtflags_t fmtfl, output::fmtflags_t mask )
{
	output::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl | (this->fmtflags_ & (~mask));

	return tmp;
}

void output::unsetf( output::fmtflags_t mask )
{
	this->fmtflags_ = this->fmtflags_ ^ mask;
}

/*
Output Methods
*/
void output::putc( char c )
{
	this->out_(c);
}

void output::putn( long n )
{
	static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz"; 

	unsigned long base = 10;

	size_t len = 0;

	char buf[65];
	char *p;

	p = buf + 64;

	*p = '\0';

	if(this->fmtflags_ & output::hex_)
	{
		base = 16;
	}
	else if(this->fmtflags_ & output::oct_)
	{
		base = 8;
	}

	do 
	{
		unsigned long d = n % base;

		n /= base;

		--p;
		len++;
		*p = (char)digits[d];
	} while((n || ((this->fmtflags_ & output::hex_) && len < 8)) && p >= buf);

	if(this->fmtflags_ & output::hex_)
	{
		*--p = 'x';
		*--p = '0';
	}
	else if(this->fmtflags_ & output::oct_)
	{
		*--p = '0';
	}

	this->puts(p);
}

void output::puts( const char *s )
{
	while(*s)
	{
		this->out_(*s++);
	}
}

/*
Output Operators
*/
output &output::operator << ( char c )
{
	this->putc(c);

	return *this;
}

output &output::operator << ( unsigned char c )
{
	return (*this << static_cast<char>(c));
}
			
output &output::operator << ( short n )
{
	return (*this << static_cast<long>(n));
}

output &output::operator << ( unsigned short n )
{
	return (*this << static_cast<unsigned long>(n));
}

output &output::operator << ( int n )
{
	return (*this << static_cast<long>(n));
}

output &output::operator << ( unsigned int n )
{
	return (*this << static_cast<unsigned long>(n));
}

output &output::operator << ( long n )
{
	this->putn(n);
	
	return *this;
}

output &output::operator << ( unsigned long n )
{
	this->putn(n);
	
	return *this;
}

output &output::operator << ( long long n )
{
	return (*this << static_cast<long>(n));
}

output &output::operator << ( unsigned long long n )
{
	return (*this << static_cast<unsigned long>(n));
}


output &output::operator << ( nullptr_t p )
{
	output::fmtflags_t tmpf = this->setf(output::hex_);

	output &tmpo = (*this << static_cast<unsigned long>((uintptr_t)p));

	this->setf(tmpf);

	return tmpo;
}

output &output::operator << ( void *p )
{
	output::fmtflags_t tmpf = this->setf(output::hex_);

	output &tmpo = (*this << static_cast<unsigned long>((uintptr_t)p));

	this->setf(tmpf);

	return tmpo;
}

output &output::operator << ( const char *s )
{
	this->puts(s);

	return *this;
}
