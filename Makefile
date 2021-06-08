.PHONY: all server clean pretty openocd

all:
	# Building core1 first is necessary.
	# How else could we include it in core0's ELF to memcpy it?
	make -C core1
	cp -f core1/emfi_core1_flash.bin core0/core1_image_inner
	make -C core0


# Work around buggy Makefile.sam.in
clean:
	rm -rf {core0,core1}/emfi_core{0,1}_flash*
	make -C core1 clean
	make -C core0 clean

server:
	JLinkGDBServer -device AT91SAM4C16C -endian little -if SWD -speed auto -ir -LocalhostOnly

server_coproc:
	JLinkGDBServer -JLinkScriptFile SAM4C.JLinkScript -device AT91SAM4C16C -speed 4000 -if swd -port 2345

openocd:
	openocd -f interface/jlink.cfg -c "transport select swd" -f target/at91sam4cXXX.cfg

pretty:
	# The null_glob avoids erroring out when there are no .h files.
	zsh -c "setopt null_glob; \
	uncrustify --replace --no-backup -c uncrustify.cfg {common,core0,core1}/**/*.{c,h}"
