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


// Pin definitions.
#define CLOCK_PIN            IOPORT_CREATE_PIN(PIOA, 29)
#define CORE0_TRIGGER_PIN    IOPORT_CREATE_PIN(PIOB, 6)
#define CORE0_STATUS_PIN     IOPORT_CREATE_PIN(PIOB, 12)


// Symbol used by test_seq.S
uint32_t pio_output_base = PIOB_OUTPUT_BASE;

extern uint8_t core1_image_start;
extern uint8_t core1_image_end;

static void copy_core1_image_into_sram1(void) {
    memcpy(
        (char *)IRAM1_ADDR,
        (char *)&core1_image_start,
        (int)&core1_image_end - (int)&core1_image_start
    );
}

static void start_core1() {
    copy_core1_image_into_sram1();
    rstc_deassert_reset_of_coprocessor(
        RSTC,
        RSTC_CPMR_CPROCEN | RSTC_CPMR_CPEREN
    );
}


static void setup_clock_pin() {
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

volatile bool ecb_ciph_ok = false;
volatile bool ecb_deciph_ok = false;


static void ipc_core1_signal_handler(Ipc *p, enum ipc_interrupt_source mask)
{
   // Does not return.
   // Pure asm.
   run_test_seq();
   
   // This will never actually get called...
   ipc_clear_command(p, mask);
}


// Unused for now.
static void set_long_flash_wait_states() {
  // FWS = cycles -1
  efc_set_wait_state(EFC, 6);
}



static void setup_ipc() {
    ipc_enable(IPC0);
    ipc_set_handler(IPC0, IPC_INTERRUPT_SRC_IRQ0, ipc_core1_signal_handler);
    ipc_enable_interrupt(IPC0, IPC_INTERRUPT_SRC_IRQ0);
    NVIC_EnableIRQ(IPC0_IRQn);
}

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
    
    // Prepare NVIC, interrupt handler for IPC.
    setup_ipc();

    // set_long_flash_wait_states()

    // Set up output pins for this core only.
    setup_clock_pin();
    setup_output_pin(CORE0_STATUS_PIN);
    setup_output_pin(CORE0_TRIGGER_PIN);

    start_core1();

    // Block. We'll want to wait for a signal from core1.
    // XXX: this could just be a busy wait.
    while (1);

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


    // init_aes();
    // ecb_ciph_ok = check_ecb_encryption();
    // ecb_deciph_ok = check_ecb_decryption();

    // while (1);

