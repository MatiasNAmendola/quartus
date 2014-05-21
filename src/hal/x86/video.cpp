#include "include/video.hpp"

#include "include/ioports.hpp"

using namespace hal::x86;

video hal::x86::screen;

video::video(  ) : color(color::light_gray), pos_x(0), pos_y(0), field_width(0)
{
	this->setf(video::dec_, video::basefield_);

	this->clear();
}

/*
Set/Get fmtflags
*/
video::fmtflags_t video::flags(  )
{
	return this->fmtflags_;
}

video::fmtflags_t video::flags( video::fmtflags_t fmtfl )
{
	video::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl;

	return tmp;
}

video::fmtflags_t video::setf( video::fmtflags_t fmtfl )
{
	video::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl | this->fmtflags_;

	return tmp;
}

video::fmtflags_t video::setf( video::fmtflags_t fmtfl, video::fmtflags_t mask )
{
	video::fmtflags_t tmp = this->fmtflags_;

	this->fmtflags_ = fmtfl | (this->fmtflags_ & (~mask));

	return tmp;
}

void video::unsetf( video::fmtflags_t mask )
{
	this->fmtflags_ = this->fmtflags_ ^ mask;
}

/*
Set field width
*/

size_t video::setw( size_t wide )
{
	size_t tmp = this->field_width;

	this->field_width = wide;

	return tmp;
}

/*
Set color
*/
void video::set_color( color::color_t color )
{
	this->color = (static_cast<uint8_t>(color)) | (this->color & 0xF0);
}

void video::set_color( const color::background &color )
{
	this->color = (static_cast<uint8_t>(color.color)) | (this->color & 0x0F);
}

/*
video-output Methods
*/
void video::clear(  )
{
	for(size_t index = 0; index < 2 * video::height * video::width; index++)
	{
		this->videomem[index] = 0;
	}

	this->pos_x = 0;
	this->pos_y = 0;
}

void video::putc( char c )
{
	switch(c)
	{
		case 0:
		break;

		case '\b':
		break;

		case '\t':			
			this->pos_x += video::tab_width;

			if(this->pos_x >= video::width - video::tab_width)
			{
				this->pos_x = 0;
				this->pos_y++;
			}
		break;	
			
		case '\n':
			this->pos_x = 0;
			this->pos_y++;
		break;

		case '\v':
			this->pos_x = 0;
			this->pos_y++;
		break;

		case '\f':
			this->clear();
		break;

		case '\r':
			this->pos_x = 0;
		break;

		default:
			if(this->pos_x > (video::width - 1))
			{	
				this->pos_x = 0;
				this->pos_y++;
			}

			if(this->pos_y > (video::height - 1))
			{     
				size_t index;

				for(index = 0; index < 2 * video::height * video::width; index++) 
				{
			  		videomem[index] = videomem[index + 160];
		  	     	}

			   	for(; index < 2 * video::height * video::width; index++) 
				{
			       		videomem[index] = 0;
			   	}

				videomem[index] = 0;

			   	this->pos_y--;
		       	}

			videomem[2 * (this->pos_y * 80 + this->pos_x)] 	 = c;
			videomem[2 * (this->pos_y * 80 + this->pos_x) + 1] = this->color;

			this->pos_x++;
		break;
	}
}

void video::putn( long n )
{
	static const char digits[4096] = "0123456789abcdefghijklmnopqrstuvwxyz";

	unsigned long base = 10;

	int fwidth = this->field_width;

	size_t len = 0;

	char buf[65];
	char *p;

	p = buf + 64;

	*p = '\0';

	if(this->fmtflags_ & video::hex_)
	{
		base = 16;
	}
	else if(this->fmtflags_ & video::oct_)
	{
		base = 8;
	}

	do 
	{
		unsigned long d = n % base;

		n /= base;

		--p;
		--fwidth;
		++len;
		*p = (char)digits[d];
	} while((n || ((this->fmtflags_ & video::hex_) && len < 8)) && p >= buf);

	if(this->fmtflags_ & video::hex_)
	{
		*--p = 'x';
		--fwidth;
		*--p = '0';
		--fwidth;
	}
	else if(this->fmtflags_ & video::oct_)
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

void video::puts( const char *s )
{
	while(*s)
	{
		this->putc(*s++);
	}
}

/*
video-output Operators
*/
video &video::operator << ( char c )
{
	this->putc(c);

	return *this;
}

video &video::operator << ( unsigned char c )
{
	return (*this << static_cast<char>(c));
}
			
video &video::operator << ( short n )
{
	return (*this << static_cast<long>(n));
}

video &video::operator << ( unsigned short n )
{
	return (*this << static_cast<unsigned long>(n));
}

video &video::operator << ( int n )
{
	return (*this << static_cast<long>(n));
}

video &video::operator << ( unsigned int n )
{
	return (*this << static_cast<unsigned long>(n));
}

video &video::operator << ( long n )
{
	this->putn(n);
	
	return *this;
}

video &video::operator << ( unsigned long n )
{
	this->putn(n);
	
	return *this;
}

video &video::operator << ( nullptr_t p )
{
	video::fmtflags_t tmpf = this->setf(video::hex_);

	video &tmpo = (*this << static_cast<unsigned long>((uintptr_t)p));

	this->setf(tmpf);

	return tmpo;
}

video &video::operator << ( void *p )
{
	video::fmtflags_t tmpf = this->setf(video::hex_);

	video &tmpo = (*this << static_cast<unsigned long>((uintptr_t)p));

	this->setf(tmpf);

	return tmpo;
}

video &video::operator << ( const char *s )
{
	this->puts(s);

	return *this;
}

video &video::operator << ( color::color_t color )
{
	this->set_color(color);

	return *this;
}

video &video::operator << ( const color::background &color )
{
	this->set_color(color);

	return *this;
}
