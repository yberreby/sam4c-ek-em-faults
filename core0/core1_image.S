#define CORE1_IMAGE_FILE "./core1_image.bin"

.section .rodata
.global core1_image_start
.global core1_image_end

.align 8
core1_image_start:
.incbin CORE1_IMAGE_FILE
.align 8
core1_image_end: