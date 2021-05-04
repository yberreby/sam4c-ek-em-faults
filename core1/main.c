#include "test_seq.h"
#include "emfi_constants.h"

#include <board.h>
#include <cmcc.h>
#include <compiler.h>
#include <efc.h>
#include <genclk.h>
#include <interrupt.h>
#include <ipc.h>
#include <ioport.h>
#include <led.h>
#include <parts.h>
#include <pio.h>
#include <pmc.h>
#include <rstc.h>
#include <sleep.h>
#include <status_codes.h>
#include <stdlib.h>
#include <string.h>
#include <sysclk.h>
#include <tc.h>



// Symbol used by test_seq.S
void* pio_output_base = PIOC_OUTPUT_BASE;


int main(void) {
    // Immediately before running our test sequence, we tell core0 to start
    // running its own.
    ipc_set_command(IPC0, IPC_INTERRUPT_SRC_IRQ0);
    
    // Does not return.
    // Pure asm.
    run_test_seq();

    // Unreachable.
    return 0;
}