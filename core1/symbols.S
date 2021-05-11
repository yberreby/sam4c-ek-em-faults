.global pio_output_base
.global trigger_pin_mask
.global status_pin_mask

.set pio_output_base, 0x4800C000
.set trigger_pin_mask, 1 << 2
.set status_pin_mask, 1 << 3