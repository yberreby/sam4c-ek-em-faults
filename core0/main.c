#include "test_seq.h"
#include "test_aes.h"
#include "test_trng.h"
#include "test_icm.h"
#include "emfi_utils.h"

#include <aes.h>
#include <board.h>
#include <cmcc.h>
#include <compiler.h>
#include <efc.h>
#include <genclk.h>
#include <icm.h>
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

// Pin definitions.
#define CLOCK_PIN            IOPORT_CREATE_PIN(PIOA, 29)
#define CORE0_TRIGGER_PIN    IOPORT_CREATE_PIN(PIOB, 18)
#define CORE0_STATUS_PIN     IOPORT_CREATE_PIN(PIOB, 19)

extern uint8_t core1_image_start;
extern uint8_t core1_image_end;

static void copy_core1_image_into_sram1(void) {
    memcpy(
        (char *)IRAM1_ADDR,
        (char *)&core1_image_start,
        (int)&core1_image_end - (int)&core1_image_start
        );
}

static void start_core1(void) {
    copy_core1_image_into_sram1();
    rstc_deassert_reset_of_coprocessor(
        RSTC,
        RSTC_CPMR_CPROCEN | RSTC_CPMR_CPEREN
        );
}


static void setup_clock_pin(void) {
    // genclk enables us to route a clock signal to an output pin.
    genclk_enable_config(
        GENCLK_PCK_1,
        GENCLK_PCK_SRC_PLLBCK,
        GENCLK_PCK_PRES_1
        );

    ioport_set_pin_mode(CLOCK_PIN, IOPORT_MODE_MUX_A);

    // Disabling a pin lets a multiplexed peripheral drive it.
    ioport_disable_pin(CLOCK_PIN);
}


// Unused for now.
static void set_long_flash_wait_states(void) {
    // FWS = cycles -1
    efc_set_wait_state(EFC, 6);
}


extern volatile uint32_t core_sync_flag;

volatile bool aes_ok = false;

int main(void) {
    // Set up the clocks.
    // This will also enable the coprocessor clock according to the
    // corresponding macro(s), as well as the SRAM1 and SRAM2 clocks.
    sysclk_init();

    // Will initialize ioport, among other things.
    board_init();

    // Disable cache controller for both cores.
    cmcc_disable(CMCC0);
    cmcc_disable(CMCC1);

    // Initializing the AES coprocessor from core0
    // saves us a LOT of trouble down the road.
    init_aes();

    //init_trng();

    // set_long_flash_wait_states()

    // Set up output pins for this core only.
    setup_clock_pin();
    setup_output_pin(CORE0_STATUS_PIN);
    setup_output_pin(CORE0_TRIGGER_PIN);


    // test_icm();

    core_sync_flag = 0;
    start_core1();

    // Busy wait. Faster than IPC.
    // Wait for a signal from core1.
    while (core_sync_flag != 0xDEADBEEF);
    trigger_010_pulse(CORE0_TRIGGER_PIN);


    while (1);

    //run_test_seq();

    // Unreachable.
    return 0;
}


