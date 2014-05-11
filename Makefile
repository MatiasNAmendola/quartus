include ./config/Makefile.config
include ./config/Makefile.rules

default: all

.PHONY: clean
.PHONY: doc

quartus-x86:
	make --no-print-directory -f ./config/arch_x86/Makefile

quartus-x64:
	make --no-print-directory -f ./config/arch_x64/Makefile

quartus-rpi:
	make --no-print-directory -f ./config/arch_rpi/Makefile

doc: Doxyfile
	@rm -rf doc
	@mkdir doc
	doxygen Doxyfile

all:
	make --no-print-directory quartus-x86
	make --no-print-directory quartus-x64
	make --no-print-directory quartus-rpi

	make --no-print-directory doc

qemu-test-x86:
	make --no-print-directory -f ./config/arch_x86/Makefile test

qemu-test-x64:
	make --no-print-directory -f ./config/arch_x64/Makefile test

qemu-test-rpi:
	make --no-print-directory -f ./config/arch_rpi/Makefile test

clean:
	make --no-print-directory -f ./config/arch_x86/Makefile clean
	make --no-print-directory -f ./config/arch_x64/Makefile clean
	make --no-print-directory -f ./config/arch_rpi/Makefile clean
