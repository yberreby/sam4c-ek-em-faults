// XXX: track down the asf.h include
// dep graph?

#include "conf_clock.h"
#include "conf_board.h"


#include "asf.h"

#include "efc.h"
#include "stdlib.h"
#include "sysclk.h"
#include "genclk.h"
#include "cmcc.h"

#include "ioport.h"
#include "stdio_serial.h"

#define STRING_EOL    "\r"
#define STRING_HEADER "-- EMFI --\r\n" \
	"-- "BOARD_NAME " --\r\n" \
	"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

#define TRIGGER_PIN    IOPORT_CREATE_PIN(PIOB, 12)
#define STATUS_PIN     IOPORT_CREATE_PIN(PIOB, 6)


#define CLOCK_PIN      IOPORT_CREATE_PIN(PIOA, 29)


/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}


static void print_clk_info(void) {
	printf("CPU Hz: %lu\n\r", sysclk_get_cpu_hz());
	printf("Peripheral clock Hz: %lu\n\r", sysclk_get_peripheral_hz());
}


// Defined externally, in test_seq.S
void run_test_seq(void);
void please_hardfault(void);


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


volatile int foo = &DWT->CTRL;


// Intrinsics: https://www.keil.com/pack/doc/CMSIS/Core/html/group__intrinsic__CPU__gr.html#gacb2a8ca6eae1ba4b31161578b720c199
//

volatile int hz;
volatile int hz2;

int main(void) {
	/* Initialize the SAM system */
    // This will also enable the coprocessor clock according to the
    // corresponding macro(s), as well as the SRAM1 and SRAM2 clocks.
	sysclk_init();

    //pmc_switch_mck_to_mainck();

	board_init();

    // Very important to copy the code for the coprocessor before deasserting
    // its reset, or else it will run gibberish!

    // rstc_deassert_reset_of_coprocessor(
    //     RSTC,
    //     // not sure we need the second one
    //     RSTC_CPMR_CPROCEN //| RSTC_CPMR_CPEREN
    // );


    ioport_init();



    //genclk_config_set_source();

    genclk_enable_config(
        GENCLK_PCK_1,
        //GENCLK_PCK_SRC_MAINCK_XTAL,
        GENCLK_PCK_SRC_PLLBCK,
        GENCLK_PCK_PRES_1
    );

    ioport_set_pin_mode(CLOCK_PIN, IOPORT_MODE_MUX_A);
    ioport_disable_pin(CLOCK_PIN);




	/* Enable/Disable interrupts globally */
	//cpu_irq_disable();
	//cpu_irq_enable();

    // Not sure when this one would be needed.
	//irq_initialize_vectors();

	/* Disable cache controller for core 0 */
	cmcc_disable(CMCC0);

	/* Initialize the console uart, for debug output */
	configure_console();

	/* Output startup info to serial port */
	puts(STRING_HEADER);

	/* Send info about the clock frequencies to the serial port */
	print_clk_info();

	/* Set up output pins */
	ioport_set_pin_dir(TRIGGER_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(STATUS_PIN,  IOPORT_DIR_OUTPUT);
    ioport_enable_pin(TRIGGER_PIN);
    ioport_enable_pin(STATUS_PIN);

    hz = sysclk_get_cpu_hz();
    hz2 = sysclk_get_peripheral_hz();

    // FWS = cycles -1
    efc_set_wait_state(EFC, 6);


    // ioport_set_pin_level(TRIGGER_PIN, 0);
    // ioport_set_pin_level(TRIGGER_PIN, 1);
    // ioport_set_pin_level(TRIGGER_PIN, 0);
    // ioport_set_pin_level(STATUS_PIN, 1);
    // ioport_set_pin_level(STATUS_PIN, 0);

    // while (1);
    // // Enable debug features.
    // CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // setup_dwt();
    // teardown_dwt();

    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    // asm("nop");
    

	// Does not return.
	// Pure asm.
	run_test_seq();

	return 0;
}


