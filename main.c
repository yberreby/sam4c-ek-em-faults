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
#include <sleep.h>
#include <status_codes.h>
#include <stdlib.h>
#include <sysclk.h>
#include <tc.h>

#define TRIGGER_PIN    IOPORT_CREATE_PIN(PIOB, 12)
#define STATUS_PIN     IOPORT_CREATE_PIN(PIOB, 6)
#define CLOCK_PIN      IOPORT_CREATE_PIN(PIOA, 29)

// These volatile globals are here so I can inspect from a debugger.
volatile int cpu_hz;
volatile int periph_hz;

int main(void) {
    // Set up the clocks.
    // This will also enable the coprocessor clock according to the
    // corresponding macro(s), as well as the SRAM1 and SRAM2 clocks.
    sysclk_init();

    // Will initialize ioport, among other things.
    board_init();

    /* Disable cache controller for core 0 */
    cmcc_disable(CMCC0);

    // FWS = cycles -1
    efc_set_wait_state(EFC, 6);

    // genclk enables us to route a clock signal to an output pin.
    genclk_enable_config(
        GENCLK_PCK_1,
        GENCLK_PCK_SRC_PLLBCK,
        GENCLK_PCK_PRES_1
    );

    ioport_set_pin_mode(CLOCK_PIN, IOPORT_MODE_MUX_A);

    // Disabling a pin lets a multiplexed peripheral drive it.
    ioport_disable_pin(CLOCK_PIN);


    /* Set up output pins */
    ioport_set_pin_dir(TRIGGER_PIN, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(STATUS_PIN,  IOPORT_DIR_OUTPUT);
    ioport_enable_pin(TRIGGER_PIN);
    ioport_enable_pin(STATUS_PIN);

    cpu_hz = sysclk_get_cpu_hz();
    periph_hz = sysclk_get_peripheral_hz();

    // Does not return.
    // Pure asm.
    run_test_seq();

    // Unreachable.
    return 0;
}



// Functions I'm setting aside for later:

//pmc_switch_mck_to_mainck();
// Very important to copy the code for the coprocessor before deasserting
// its reset, or else it will run gibberish!

// rstc_deassert_reset_of_coprocessor(
//     RSTC,
//     // not sure we need the second one
//     RSTC_CPMR_CPROCEN //| RSTC_CPMR_CPEREN
// );

//genclk_config_set_source();

// // Enable debug features.
// CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

// setup_dwt();
// teardown_dwt();

/* Enable/Disable interrupts globally */
//cpu_irq_disable();
//cpu_irq_enable();

// Not sure when this one would be needed.
//irq_initialize_vectors();


// Intrinsics: https://www.keil.com/pack/doc/CMSIS/Core/html/group__intrinsic__CPU__gr.html#gacb2a8ca6eae1ba4b31161578b720c199


/*

void setup_dwt() {
  asm("nop");
  DWT->CTRL = DWT_CTRL_CYCCNTENA_Msk;
  asm("nop");
}

void teardown_dwt() {
  asm("nop");
  DWT->CTRL = 0;
  asm("nop");
}

*/