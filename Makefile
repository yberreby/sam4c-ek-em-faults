.PHONY: server serial clean # pretty

all:
	# Building core1 first is necessary.
	# How else could we include it in core0's ELF to memcpy it?
	make -C core1
	make -C core0


# Work around buggy Makefile.sam.in
clean:
	rm -rf {core0,core1}/emfi_core{0,1}_flash*
	make -C core1 clean
	make -C core0 clean


server:
	JLinkGDBServer -device AT91SAM4S16C -endian little -if SWD -speed auto -ir -LocalhostOnly

pretty:
	# The null_glob avoids erroring out when there are no .h files.
	zsh -c "setopt null_glob; \
	uncrustify --replace --no-backup -c uncrustify.cfg {common,core0,core1}/**/*.{c,h}"

# openocd:
# 	openocd -f interface/jlink.cfg -c "transport select swd" -f target/at91sam4cXXX.cfg