#include "include/serial.hpp"

#include "include/mmio.hpp"
#include "include/gpio.hpp"
#include "include/delay.hpp"

using namespace hal::rpi;

serial hal::rpi::seri;

serial::serial(  ) : field_width(0)
{
	io::write(serial::cr, 0x00000000);
	
	io::write(gpio::base + 0x94, 0x00000000);
	delay(150);

	io::write(gpio::base + 0x98,(1 << 14) | (1 << 15));
	delay(150);

	io::write(gpio::base + 0x98, 0x00000000);

	io::write(serial::icr, 0x7FF);

	io::write(serial::ibrd, 1);
	io::write(serial::fbrd, 40);

	io::write(serial::lcrh, (1 << 4) | (1 << 5) | (1 << 6));

	io::write(serial::imsc, 0x7F2);

	io::write(serial::cr, (1 << 0) | (1 << 8) | (1 << 9));
	this->setf(serial::dec_, serial::basefield_);
}

/*
Set/Get fmtflags
*/
serial::fmtflags_t serial::flags(  )
{
	return this->fmtflags_;
}

serial::fmtflags_t serial::flags( serial::fmtflags_t fmtfl )
{
	serial::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl;

	return tmp;
}

serial::fmtflags_t serial::setf( serial::fmtflags_t fmtfl )
{
	serial::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl | this->fmtflags_;

	return tmp;
}

serial::fmtflags_t serial::setf( serial::fmtflags_t fmtfl, serial::fmtflags_t mask )
{
	serial::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl | (this->fmtflags_ & (~mask));

	return tmp;
}

void serial::unsetf( serial::fmtflags_t mask )
{
	this->fmtflags_ = this->fmtflags_ ^ mask;
}

/*
Set field width
*/
size_t serial::setw( size_t wide )
{
	size_t tmp = this->field_width;

	this->field_width = wide;

	return tmp;
}


/*
serial-output Methods
*/
void serial::putc( char c )
{
	while(io::read(serial::flag_register) & serial::buffer_full);

	io::write(serial::data_register, c);
}

void serial::putn( long n )
{
	static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz"; 

	int base = 10;

	int fwidth = this->field_width;

	size_t len = 0;

	char buf[65];
	char *p;

	p = buf + 64;

	*p = '\0';

	if(this->fmtflags_ & serial::hex_)
	{
		base = 16;
	}
	else if(this->fmtflags_ & serial::oct_)
	{
		base = 8;
	}

	do 
	{
		long d = n % base;

		n /= base;

		--p;
		--fwidth;
		len++;
		*p = (char)digits[d];
	} while(n);

	if(this->fmtflags_ & serial::hex_)
	{
		while(len++ < 8)
		{
			*--p = '0';
			--fwidth;
		}

		*--p = 'x';
		--fwidth;
		*--p = '0';
		--fwidth;
	}
	else if(this->fmtflags_ & serial::oct_)
	{
		*--p = '0';
		--fwidth;
	}

	while(fwidth > 0)
	{
		*--p = ' ';
		--fwidth;
	}

	this->puts(p);
}

void serial::puts( const char *s )
{
	while(*s)
	{
		this->putc(*s++);
	}
}

/*
serial-output Operators
*/
serial &serial::operator << ( char c )
{
	this->putc(c);

	return *this;
}

serial &serial::operator << ( unsigned char c )
{
	return (*this << static_cast<char>(c));
}
			
serial &serial::operator << ( short n )
{
	return (*this << static_cast<long>(n));
}

serial &serial::operator << ( unsigned short n )
{
	return (*this << static_cast<unsigned long>(n));
}

serial &serial::operator << ( int n )
{
	return (*this << static_cast<long>(n));
}

serial &serial::operator << ( unsigned int n )
{
	return (*this << static_cast<unsigned long>(n));
}

serial &serial::operator << ( long n )
{
	this->putn(n);
	
	return *this;
}

serial &serial::operator << ( unsigned long n )
{
	this->putn(n);
	
	return *this;
}

serial &serial::operator << ( nullptr_t p )
{
	serial::fmtflags_t tmpf = this->setf(serial::hex_);

	serial &tmpo = (*this << static_cast<unsigned long>((uintptr_t)p));

	this->setf(tmpf);

	return tmpo;
}

serial &serial::operator << ( void *p )
{
	serial::fmtflags_t tmpf = this->setf(serial::hex_);

	serial &tmpo = (*this << static_cast<unsigned long>((uintptr_t)p));

	this->setf(tmpf);

	return tmpo;
}

serial &serial::operator << ( const char *s )
{
	this->puts(s);

	return *this;
}
