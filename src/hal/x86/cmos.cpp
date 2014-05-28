#include "include/cmos.hpp"
#include "include/ioports.hpp"

#include <bcd.hpp>

using namespace hal::x86;

void cmos::write( uint8_t addr, uint8_t value )
{
	addr = (io::inb(cmos::addr_port) & 0x80) | (addr & 0x7F);

	io::outb(cmos::addr_port, addr);
	io::outb(cmos::data_port, value);
}

uint8_t cmos::read(  uint8_t addr )
{
	addr = (io::inb(cmos::addr_port) & 0x80) | (addr & 0x7F);

	io::outb(cmos::addr_port, addr);

	return io::inb(cmos::data_port);	
}

void cmos::time( time_t *hour, time_t *min, time_t *sec )
{
	*hour 	= tools::bcd::decode(cmos::read(cmos::addr_hour));
	*min  	= tools::bcd::decode(cmos::read(cmos::addr_min));
	*sec  	= tools::bcd::decode(cmos::read(cmos::addr_sec));
}

void cmos::date( time_t *year, time_t *month, time_t *day )
{
	*year 	= tools::bcd::decode(cmos::read(cmos::addr_century)) * 100 + tools::bcd::decode(cmos::read(cmos::addr_year));
	*month  = tools::bcd::decode(cmos::read(cmos::addr_month));
	*day  	= tools::bcd::decode(cmos::read(cmos::addr_day));
}
