BASEDIR  = .
SRCDIR	 = $(BASEDIR)/src
BUILDDIR = $(BASEDIR)/bin

SRCS = 
OBJS = 

include $(BASEDIR)/config/arch_x64/Makefile.config
include $(BASEDIR)/config/arch_x64/Makefile.rules

.PHONY: clean

quartus-x64:
	@echo "\033[0;31mSorry, x64 support is not implemented yet.\033[0m"

clean:


test:
	@echo "\033[0;31mSorry, x64 support is not implemented yet.\033[0m"

iso:
	@echo "\033[0;31mSorry, x64 support is not implemented yet.\033[0m"

%.o: %.c 
	$(CC) $(PREPROC) $(CCFLAGS) $(INCL) -c -o $@ $^
%.o: %.cpp 
	$(CPP) $(PREPROC) $(CCFLAGS) $(CPPFLAGS) $(INCL) -c -o $@ $^
%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

