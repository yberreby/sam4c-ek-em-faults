.global core_sync_flag
.set core_sync_flag, 0x20010000

.global aes_output_ready
.set aes_output_ready, 0x20010004


.global aes_output_data
.set aes_output_data, 0x20010200

// From objdump:
// 2001fffc g       .text	00000000 __ram_end__

// DO NOT place shared objects in SRAM1 or SRAM2,
// they might get overwritten when booting up core1!