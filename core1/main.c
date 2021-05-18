#include "test_seq.h"
#include "test_aes.h"
#include "emfi_utils.h"

#include <aes.h>
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
extern volatile bool aes_output_ready;

int main(void) {
    // Set up pins.
    setup_output_pin(CORE1_TRIGGER_PIN);
    setup_output_pin(CORE1_STATUS_PIN);

    // Tell core0 to resume running.
    core_sync_flag = 0xDEADBEEF;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    // This pulse is here to help us synchronize the two cores.
    trigger_010_pulse(CORE1_TRIGGER_PIN);


    // Manually start encryption or decryption process.
    aes_start(AES);

    // This pulse to mark the start of our waiting.
    trigger_010_pulse(CORE1_TRIGGER_PIN);
    while (!aes_output_ready);
    // Pulse to show the data is ready.
    trigger_010_pulse(CORE1_TRIGGER_PIN);

    // Does not return.
    // Pure asm.
    //run_test_seq();

    while (1);
    

    // Unreachable.
    return 0;
}