#include "test_seq.h"
#include "emfi_utils.h"

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

#define CORE1_TRIGGER_PIN    IOPORT_CREATE_PIN(PIOC, 2)
#define CORE1_STATUS_PIN     IOPORT_CREATE_PIN(PIOC, 3)

int main(void) {
    setup_output_pin(CORE1_TRIGGER_PIN);
    setup_output_pin(CORE1_STATUS_PIN);


    // Immediately before running our test sequence, we tell core0 to start
    // running its own.
    ipc_set_command(IPC0, IPC_INTERRUPT_SRC_IRQ0);


    for (int i = 0; i < 40; i++) {
        asm("nop");
    }

    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");



    // Does not return.
    // Pure asm.
    run_test_seq();

    

    // Unreachable.
    return 0;
}