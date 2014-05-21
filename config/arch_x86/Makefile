BASEDIR  = .
SRCDIR	 = $(BASEDIR)/src
BUILDDIR = $(BASEDIR)/bin
LOGDIR	 = $(BASEDIR)/log

SRCS = 
OBJS = 

include $(BASEDIR)/config/arch_x86/Makefile.config
include $(BASEDIR)/config/arch_x86/Makefile.rules

.PHONY: clean

quartus-x86: $(OBJS)
	$(LD) $(LDFLAGS) $(LINK) -o $(BUILDDIR)/$@ $^

clean:
	rm $(OBJS)

test:
	qemu-system-i386 -cpu pentium3 -D $(LOGDIR)/test-x86.log -d int -k de -m 1024 -serial stdio -name quartus-x86 -kernel $(BUILDDIR)/quartus-x86

%.o: %.c 
	$(CC) $(PREPROC) $(CCFLAGS) $(INCL) -c -o $@ $^
%.o: %.cpp 
	$(CPP) $(PREPROC) $(CCFLAGS) $(CPPFLAGS) $(INCL) -c -o $@ $^
%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

