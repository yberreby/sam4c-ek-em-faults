#include "emfi_utils.h"
#include <pio.h>
#include <ioport.h>

void setup_output_pin(int pin) {
    ioport_set_pin_dir(pin, IOPORT_DIR_OUTPUT);
    ioport_enable_pin(pin);
}

// Quite useful for debugging.
void trigger_010_pulse(int pin) {
    ioport_set_pin_level(pin, 0);
    ioport_set_pin_level(pin, 1);
    ioport_set_pin_level(pin, 0);
}
