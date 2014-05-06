#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define 			arch_x86

#define				QEMU_DEBUG

#define PACKED			__attribute__((packed))

#define SINGLETON( NAME )    			\
	public:                        		\
		static NAME &instance()      	\
		{                            	\
			static NAME _instance;  \
			return _instance;       \
		}                            	\
	private:                       		\
		NAME();               		\
		NAME( const NAME& );	

namespace definitions
{
	#ifdef QEMU_DEBUG
	/**
	  * @brief	IO-port of the qemu serial port
	  *
	  *		Used for debug-output.
	  */
	const unsigned short qemu_serial = 0x3F8;
	#endif
}

#endif
