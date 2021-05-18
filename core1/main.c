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


extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;



void init_relocate() {
	uint32_t *pSrc, *pDest;

	/* Initialize the relocate segment */
	pSrc = &_etext;
	pDest = &_srelocate;

	if (pSrc > pDest) {
		/* Copy segment block from beginning to end */
		for (; pDest < &_erelocate;) {
			*pDest++ = *pSrc++;
		}
	} else if (pSrc < pDest) {
		/* Copy segment block from end to beginning */
		uint32_t bytes_relocate = (uint32_t)&_erelocate - (uint32_t)&_srelocate;
		pSrc = (uint32_t*)((uint32_t)pSrc + bytes_relocate) - 1;
		pDest = (uint32_t*)((uint32_t)pDest + bytes_relocate) - 1;

		for (; bytes_relocate; bytes_relocate -= 4) {
			*pDest-- = *pSrc--;
		}
	}

}


int main(void) {
    //init_relocate();


    setup_output_pin(CORE1_TRIGGER_PIN);
    setup_output_pin(CORE1_STATUS_PIN);

    // Tell core0 to run.
    core_sync_flag = 0xDEADBEEF;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    trigger_010_pulse(CORE1_TRIGGER_PIN);
    trigger_010_pulse(CORE1_TRIGGER_PIN);


    // init_aes();


    // trigger_010_pulse(CORE1_TRIGGER_PIN);

    // if (check_ecb_decryption()) {
    //     trigger_010_pulse(CORE1_TRIGGER_PIN);
    // } else {
    //     trigger_010_pulse(CORE1_TRIGGER_PIN);
    //     trigger_010_pulse(CORE1_TRIGGER_PIN);
    // }
    // trigger_010_pulse(CORE1_TRIGGER_PIN);


    // Does not return.
    // Pure asm.
    //run_test_seq();

    while (1);
    

    // Unreachable.
    return 0;
}