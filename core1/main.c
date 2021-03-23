#include "test_seq.h"

#include <board.h>
#include <cmcc.h>
#include <compiler.h>
#include <efc.h>
#include <genclk.h>
#include <interrupt.h>
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

#define TRIGGER_PIN    IOPORT_CREATE_PIN(PIOB, 12)
#define STATUS_PIN     IOPORT_CREATE_PIN(PIOB, 6)
#define CLOCK_PIN      IOPORT_CREATE_PIN(PIOA, 29)


int main(void) {
    int volatile * const dst = (int *)0x201000AA;
    *dst = 0xBB;

    while (1);

    /* Set up output pins */
    ioport_set_pin_dir(TRIGGER_PIN, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(STATUS_PIN,  IOPORT_DIR_OUTPUT);
    ioport_enable_pin(TRIGGER_PIN);
    ioport_enable_pin(STATUS_PIN);

    ioport_set_pin_level(TRIGGER_PIN, false);
    ioport_set_pin_level(STATUS_PIN, false);

    ioport_set_pin_level(TRIGGER_PIN, true);
    ioport_set_pin_level(TRIGGER_PIN, false);
    ioport_set_pin_level(TRIGGER_PIN, true);
    ioport_set_pin_level(TRIGGER_PIN, false);
    ioport_set_pin_level(TRIGGER_PIN, true);


    ioport_set_pin_level(STATUS_PIN, true);
    ioport_set_pin_level(STATUS_PIN, false);
    ioport_set_pin_level(STATUS_PIN, true);
    ioport_set_pin_level(STATUS_PIN, false);
    ioport_set_pin_level(STATUS_PIN, true);




    while (1);

    // Does not return.
    // Pure asm.
    //run_test_seq();

    // Unreachable.
    return 0;
}