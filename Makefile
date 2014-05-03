SRCS := $(shell find -name '*.cpp') \
	$(shell find -name '*.c') \
	$(shell find -name '*.S')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

INCLUDE = -I lib/libc/include -I lib/libc++/include -I definitions

C 		= gcc -std=c11
CPP 		= g++ -std=c++11
LD 		= ld

ASFLAGS 	= -m32
CFLAGS 		= -m32 -nostdinc -nostdlib -fno-builtin -fno-leading-underscore -Wall -Wextra -pedantic-errors -g -fno-stack-protector
CFLAGSPP 	= -nostdinc++ -fno-use-cxa-atexit -fno-rtti -fno-exceptions -fno-threadsafe-statics
LDFLAGS 	= -melf_i386 -Tkernel.ld

kernel++: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c 
	$(C) $(CFLAGS) $(INCLUDE) -c -o $@ $^

%.o: %.cpp 
	$(CPP) $(CFLAGS) $(CFLAGSPP) $(INCLUDE) -c -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

clean:
	rm $(OBJS) 

doc:
	doxygen Doxyfile

doc-clean:
	rm -r doc

all:
	make kernel++
	make doc

.PHONY: clean
.PHONY: doc
.PHONY: doc-clean
.PHONY: all
