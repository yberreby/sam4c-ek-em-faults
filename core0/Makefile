# Include the common Makefile, which will also include the project specific
# config.mk file.
MAKEFILE_PATH = ../Makefile.sam.in
include $(MAKEFILE_PATH)

# Unused for now.
#serial:
#	tio -b 62500 -d 8 -f none -s 1 --parity none /dev/ttyUSB0

build/../core0/core1_image.o: core1_image_inner