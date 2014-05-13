BASEDIR  = .
SRCDIR	 = $(BASEDIR)/src
BUILDDIR = $(BASEDIR)/bin

SRCS = 
OBJS = 

include $(BASEDIR)/config/arch_rpi/Makefile.config
include $(BASEDIR)/config/arch_rpi/Makefile.rules

.PHONY: clean

quartus-rpi: $(OBJS)
	$(LD) $(LDFLAGS) $(LINK) -o $(BUILDDIR)/$@ $^

clean:
	rm $(OBJS)

test:
	qemu-system-arm -cpu arm1176 -m 256 -M versatilepb -no-reboot -name quartus-rpi -serial stdio -kernel $(BUILDDIR)/quartus-rpi

%.o: %.c 
	$(CC) $(PREPROC) $(CCFLAGS) $(INCL) -c -o $@ $^
%.o: %.cpp 
	$(CPP) $(PREPROC) $(CCFLAGS) $(CPPFLAGS) $(INCL) -c -o $@ $^
%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

