#include "test_seq.h"
#include "test_aes.h"
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

extern volatile uint32_t core_sync_flag;

int main(void) {
    setup_output_pin(CORE1_TRIGGER_PIN);
    setup_output_pin(CORE1_STATUS_PIN);

    // Tell core0 to run.
    core_sync_flag = 0xDEADBEEF;
    //core_sync_flag = 0xF0F0F0F0;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    trigger_010_pulse(CORE1_TRIGGER_PIN);


    //init_aes();

    //if (check_ecb_encryption()) {
    //    trigger_010_pulse(CORE1_TRIGGER_PIN);
    //} else {
    //    trigger_010_pulse(CORE1_TRIGGER_PIN);
    //    trigger_010_pulse(CORE1_TRIGGER_PIN);
    //}


    // Does not return.
    // Pure asm.
    //run_test_seq();

    while (1);
    

    // Unreachable.
    return 0;
}