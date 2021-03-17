# Include the common Makefile, which will also include the project specific
# config.mk file.
MAKEFILE_PATH = Makefile.sam.in
include $(MAKEFILE_PATH)

.PHONY: server serial pretty

server:
	JLinkGDBServer -device AT91SAM4S16C -endian little -if SWD -speed auto -ir -LocalhostOnly

serial:
	tio -b 62500 -d 8 -f none -s 1 --parity none /dev/ttyUSB0


# Work around buggy Makefile.sam.in
clean: custom_clean

custom_clean:
	rm -rf emfi_flash*

pretty:
	uncrustify --replace --no-backup -c uncrustify.cfg *.c include/*.h

openocd:
	openocd -f interface/jlink.cfg -c "transport select swd" -f target/at91sam4cXXX.cfg
