#include <trng.h>
#include <pmc.h>

#include "test_trng.h"



volatile uint32_t rand_g;

void TRNG_Handler(void)
{
    uint32_t status;

    status = trng_get_interrupt_status(TRNG);

    if ((status & TRNG_ISR_DATRDY) == TRNG_ISR_DATRDY) {
        rand_g = trng_read_output_data(TRNG);
    }
}


void init_trng() {
    /* Configure PMC */
    pmc_enable_periph_clk(ID_TRNG);

    /* Enable TRNG */
    trng_enable(TRNG);

    /* Enable TRNG interrupt */
    NVIC_DisableIRQ(TRNG_IRQn);
    NVIC_ClearPendingIRQ(TRNG_IRQn);
    NVIC_SetPriority(TRNG_IRQn, 0);
    NVIC_EnableIRQ(TRNG_IRQn);
    trng_enable_interrupt(TRNG);
}

