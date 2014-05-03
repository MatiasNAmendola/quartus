SRCS = $(shell find -name '*.[cS]')
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = gcc
AR = ar

ASFLAGS = -m32
CFLAGS = -c -m32 -Wall -g -fno-stack-protector -nostdinc
ARFLAGS= crs

libc: $(OBJS)
	$(AR) $(ARFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

clean:
	rm $(OBJS)

.PHONY: clean
