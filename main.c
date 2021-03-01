// XXX: track down the asf.h include
// dep graph?

#include "conf_clock.h"
#include "conf_board.h"

#include "asf.h"

#include "stdlib.h"
#include "sysclk.h"

#include "ioport.h"
#include "stdio_serial.h"


#define STRING_EOL    "\r"
#define STRING_HEADER "-- EMFI --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

#define TRIGGER_PIN    IOPORT_CREATE_PIN(PIOA, 0)
#define STATUS_PIN     IOPORT_CREATE_PIN(PIOA, 1)


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


int main(void)
{
	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Initialize the console uart */
	configure_console();

	/* Output startup info to serial port */
	puts(STRING_HEADER);

    /* Send info about the clock frequencies to the serial port */
    print_clk_info();

    ioport_init();


    /* Setup output pins */
    ioport_set_pin_dir(TRIGGER_PIN, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(STATUS_PIN,  IOPORT_DIR_OUTPUT);
    
    // TODO

    return 0;
}
