# Path to top level ASF directory relative to this project directory.
PRJ_PATH = ../xdk-asf-3.49.1

# Target CPU architecture: cortex-m3, cortex-m4
ARCH = cortex-m4

# Target part: none, sam3n4 or sam4l4aa
PART = sam4c16c:1

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET_FLASH = emfi_core1_flash.elf
TARGET_SRAM = emfi_core1_sram.elf

# List of C source files.
CSRCS = \
	../core1/main.c                                    \
       common/services/clock/sam4c/sysclk.c               \
       common/utils/interrupt/interrupt_sam_nvic.c        \
       common/utils/stdio/read.c                          \
       common/utils/stdio/write.c                         \
       sam/boards/sam4c_ek/init.c                         \
       sam/drivers/pio/pio.c                              \
       sam/drivers/pio/pio_handler.c                      \
       sam/drivers/pmc/pmc.c                              \
       sam/drivers/pmc/sleep.c                            \
       sam/drivers/tc/tc.c                                \
       sam/drivers/uart/uart.c                            \
       sam/drivers/usart/usart.c                          \
       sam/drivers/efc/efc.c							  \
       sam/drivers/rstc/rstc.c                            \
       sam/utils/cmsis/sam4c/source/templates/gcc/startup_sam4c.c \
       sam/utils/cmsis/sam4c/source/templates/system_sam4c.c \
       sam/utils/syscalls/gcc/syscalls.c

# List of assembler source files.
ASSRCS = \
	../common/src/test_seq.S

# List of include paths.
INC_PATH = \
       ../common/include \
       common/boards                                      \
       common/services/clock                              \
       common/services/ioport                             \
       common/utils                                       \
       common/utils/stdio/stdio_serial                    \
       sam/boards                                         \
       sam/boards/sam4c_ek                                \
       sam/drivers/pio                                    \
       sam/drivers/pmc                                    \
       sam/drivers/tc                                     \
       sam/drivers/uart                                   \
       sam/drivers/usart                                  \
       sam/drivers/cmcc                                   \
       sam/drivers/efc                                    \
       sam/drivers/rstc                                   \
       sam/utils                                          \
       sam/utils/fpu                                      \
       sam/utils/cmsis/sam4c/include                      \
       sam/utils/cmsis/sam4c/source/templates             \
       sam/utils/header_files                             \
       sam/utils/preprocessor                             \
       thirdparty/CMSIS/Include                           \
       thirdparty/CMSIS/Lib/GCC

# Additional search paths for libraries.
LIB_PATH =  \
       thirdparty/CMSIS/Lib/GCC                          

# List of libraries to use during linking.
LIBS =  \
       arm_cortexM4lf_math_softfp                         \
       m                                                 

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT_FLASH = sam/utils/linker_scripts/sam4c/gcc/sam4c16c_1_sram.ld
LINKER_SCRIPT_SRAM  = sam/utils/linker_scripts/sam4c/gcc/sam4c16c_1_sram.ld

# Path relative to top level directory pointing to a linker script.
DEBUG_SCRIPT_FLASH = sam/boards/sam4c_ek/debug_scripts/gcc/sam4c_ek_sram_1.gdb
DEBUG_SCRIPT_SRAM  = sam/boards/sam4c_ek/debug_scripts/gcc/sam4c_ek_sram_1.gdb

# Project type parameter: all, sram or flash
PROJECT_TYPE        = flash

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS = 

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = -O1

# Extra flags to use when archiving.
ARFLAGS = 

# Extra flags to use when assembling.
ASFLAGS =  \
       -mfloat-abi=softfp                                 \
       -mfpu=fpv4-sp-d16                                 

# Extra flags to use when compiling.
CFLAGS =  \
       -mfloat-abi=softfp                                 \
       -mfpu=fpv4-sp-d16                                 

# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = \
       -D ARM_MATH_CM4=true                               \
       -D BOARD=SAM4C_EK                                  \
       -D __SAM4C16C_1__                                  \
       -D printf=iprintf                                  \
       -D scanf=iscanf

# Extra flags to use when linking
LDFLAGS = \

# Pre- and post-build commands
PREBUILD_CMD =
POSTBUILD_CMD = cp -f $(project).bin ../core0/core1_image.bin
