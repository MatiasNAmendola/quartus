include ./config/Makefile.config
include ./config/Makefile.rules

default: all

.PHONY: clean
.PHONY: doc

doc: Doxyfile
	rm -rf doc
	mkdir doc
	doxygen Doxyfile

quartus: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/$@ $^

all:
	make --no-print-directory quartus
	make --no-print-directory doc

qemu-test:
	qemu-system-i386 -cpu pentium3 -k de -m 1024 -serial stdio -name quartus -kernel $(BUILDDIR)/quartus

%.o: %.c 
	$(CC) $(CCFLAGS) $(INCLUDE) -c -o $@ $^
%.o: %.cpp 
	$(CPP) $(CCFLAGS) $(CPPFLAGS) $(INCLUDE) -c -o $@ $^
%.o: %.S
	$(CC) $(ASFLAGS) -c -o $@ $^

clean:
	rm $(OBJS) 
