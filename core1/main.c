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



// Symbol used by test_seq.S
uint32_t pio_output_base = PIOC_OUTPUT_BASE;

uint32_t trigger_pin_mask = 1UL << 2;
uint32_t status_pin_mask   = 1UL << 3;


#define CORE1_TRIGGER_PIN    IOPORT_CREATE_PIN(PIOC, 2)
#define CORE1_STATUS_PIN     IOPORT_CREATE_PIN(PIOC, 3)

int main(void) {
    setup_output_pin(CORE1_TRIGGER_PIN);
    //setup_output_pin(CORE1_STATUS_PIN);


    ioport_set_pin_level(CORE1_TRIGGER_PIN, 1);

    //trigger_010_pulse(CORE1_TRIGGER_PIN);
    //trigger_010_pulse(CORE1_STATUS_PIN);

    // Immediately before running our test sequence, we tell core0 to start
    // running its own.
    ipc_set_command(IPC0, IPC_INTERRUPT_SRC_IRQ0);
    
    // Does not return.
    // Pure asm.
    run_test_seq();

    // Unreachable.
    return 0;
}