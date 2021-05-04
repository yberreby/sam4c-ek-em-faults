#ifndef EMFI_UTILS_H
#define EMFI_UTILS_H

#define PIOA_OUTPUT_BASE 0x400E0E00UL
#define PIOB_OUTPUT_BASE 0x400e1000UL
#define PIOC_OUTPUT_BASE 0x4800C000UL

extern int core1_has_started;

void setup_output_pin(int pin);

void trigger_010_pulse(int pin);

#endif