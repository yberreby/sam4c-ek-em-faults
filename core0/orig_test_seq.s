.thumb
.syntax unified
.cpu cortex-m4

.section    .rodata
.include        "registers_and_bits.inc.s"


.section    .text
.type       _start, %function
.global _start

_start:
bl.w          .Lstartup
bl.w          .Linit_clock
bl.w          .gpio_config
b.w          reg_init

.Lstartup:
ldr.w         r0, =RCC_CR             /* resetting clock configuration to it's defaults */
mov.w         r1, HSION               /* setting HSION bit */
str.w         r1, [r0]
ldr.w         r0, =RCC_CFGR           /* clearing RCC_CFGR register */
mov.w         r1, 0
str.w         r1, [r0]
ldr.w         r0, =RCC_CR             /* clearing HSEON, CSSON, PLLON bits */
ldr.w         r1, [r0]
ldr.w         r2, =HSEON | CSSON | PLLON
bic.w         r1, r2
str.w         r1, [r0]
ldr.w         r0, =RCC_PLLCFGR        /* setting the default value from datasheet */
ldr.w         r1, =0x24003010
str.w         r1, [r0]
ldr.w         r0, =RCC_CR             /* clearing HSEBYP bit */
ldr.w         r1, [r0]
bic.w         r1, HSEBYP
str.w         r1, [r0]
ldr.w         r0, =RCC_CIR            /* disabling all interrupts */
mov.w         r1, 0
str.w         r1, [r0]
movs          R0,R0
bx          lr

.Linit_clock:
/* config to ouput the clock signal on  pin pc9*/
ldr.w		      R1, =RCC_CFGR	/* Two bits per pin so bits 30 to 31 control MCO2 (pin pc9) */
ldr.w		      R0, [R1]
and.w	      R0, #0x3FFFFFFF		/* Clear bits to select SYSCLK as clock output */
str.w		      R0, [R1]
ldr.w		      R1, =RCC_CFGR	/* Two bits per pin so bits 27 to 29 control MCO2PRE (pin pc9) */
ldr.w		      R0, [R1]
and.w	      R0, #0xC7FFFFFF		/* Clear bits related to MC02 prescaler */
orr.w       R0, #0x20000000		/* Mode bits set to 100 for div2 */
str.w		      R0, [R1]
ldr.w		      R1, =RCC_AHB1ENR	/* Pseudo-load address in R1 */
ldr.w		      R0, [R1]			/* Copy contents at address in R1 to R0 */
orr.w       R0, #0x04			/* Bitwise OR entire word in R0, result in R0 */
str.w		      R0, [R1]			/* Store R0 contents to address in R1*/
ldr.w		      R1, =GPIOD_MODER	/* Two bits per pin so bits 18 to 19 control pin 9 */
ldr.w		      R0, [R1]
and.w	      R0, #0xFFF3FFFF		/* OR and and both operations reqd for 2 bits */
orr.w 	    R0, #0x00080000		/* Mode bits set to '10' makes the pin mode as alternate */
str.w		      R0, [R1]
ldr.w		      R1, =GPIOD_OSPEEDR	/* Two bits 18 to 19 for pin 9 */
ldr.w		      R0, [R1]
orr.w 	      R0, #0x000C0000		/* Speed bits set to '11' configures pin for high speed */
str.w		      R0, [R1]
ldr.w		      R1, =GPIOD_PUPDR	/* Two bits per pin so bits 18 to 19 control pin 9 */
ldr.w		      R0, [R1]
and.w	      R0, #0xFFF3FFFF		/* Clear bits to disable pullup/pulldown */
str.w		      R0, [R1]
/* config to work with internal oscillator clock 16MHz */
ldr.w         r0, =RCC_CR             /* enabling HSI clock */
ldr.w         r1, [r0]
orr.w       r1, HSION       /* old : HSEON */
str.w         r1, [r0]
1:
ldr.w         r1, [r0]                /* waiting for HSI to be ready */
ands.w        r1, HSIRDY    /* old : HSERDY */
beq.w         1b
ldr.w         r0, =RCC_APB1ENR        /* enable power controller and */
ldr.w         r1, [r0]                /* select regulator voltage scale mode 1*/
orr.w       r1, PWREN               /* to be able to work on 168 MHz */
str.w         r1, [r0]
ldr.w         r0, =PWR_CR
ldr.w         r1, [r0]
orr.w       r1, VOS
str.w         r1, [r0]
ldr.w         r0, =RCC_CFGR           /* configuring peripherals clock prescaling */
ldr.w         r1, [r0]
bic.w         r1, RCC_CFGR_HPRE_DIV1  /* explicitly setting no devision for AHB */
and.w       r1, RCC_CFGR_PPRE2_DIV1 /* APB2 not divided */
and.w       r1, RCC_CFGR_PPRE1_DIV1 /* APB1 not divided */
str.w         r1, [r0]
1:
ldr.w         r0, =RCC_CR             /* enable main PLL */
ldr.w         r1, [r0]                /* wait for HSI to be ready */
ands.w        r1, HSIRDY
beq.w         1b
ldr.w         r0, =FLASH_ACR          /* flash latency clock cycles amount */
ldr.w         r1, [r0]                /* in general this parameter depends on Vdd and system clock frequency */
ldr.w         r2, =ACR_LATENCY_1WS    /* in our case it is 2 cycles (or 1 "wait states") according to the manual */
orr.w       r1, r2
str.w         r1, [r0]
ldr.w         r0, =RCC_CFGR           /* selecting HSI as a source */
ldr.w         r1, [r0]
and.w        r1, SW_HSI
str.w         r1, [r0]
1:
ldr.w         r1, [r0]
and.w         r1, RCC_CFGR_SWS
cmp.w         r1, SWS_HSI
bne.w         1b
movs          R0,R0
bx          lr

.gpio_config:
ldr.w		      R1, =RCC_AHB1ENR	/* Pseudo-load address in R1 */
ldr.w		      R0, [R1]			/* Copy contents at address in R1 to R0 */
orr.w 	      R0, #0x04			/* Bitwise OR entire word in R0, result in R0 */
str.w		      R0, [R1]			/* Store R0 contents to address in R1 */
ldr.w		      R1, =GPIOC_MODER	/* Two bits per pin so bits 12 to 13 control pin 6 */
ldr.w		      R0, [R1]
and.w	        R0, #0xFFFFCFFF	/* OR and and both operations reqd for 2 bits */
orr.w         R0, #0x00001000		/* Mode bits set to '01' makes the pin mode as output */
str.w		      R0, [R1]
ldr.w		      R1, =GPIOC_OTYPER	/* Type bit '0' configures pin for push-pull */
ldr.w		      R0, [R1]
and.w 	      R0, #0xFFFFFFBF
str.w		      R0, [R1]
ldr.w		      R1, =GPIOC_OSPEEDR	/* Two bits per pin so bits 12 to 13 control pin 6*/
ldr.w		      R0, [R1]
orr.w 	      R0, #0x00003000		/* Speed bits set to '11' configures pin for high speed */
str.w		      R0, [R1]
ldr.w		      R1, =GPIOC_PUPDR	/* Two bits per pin so bits 12 to 13 control pin 6 */
ldr.w		      R0, [R1]
and.w	        R0, #0xFFFFCFFF		/* Clear bits to disable pullup-pulldown */
str.w		      R0, [R1]
ldr.w		      R1, =GPIOC_MODER	/* Two bits 22 to 23 for pin 11 */
ldr.w		      R0, [R1]
and.w	        R0, #0xFF3FFFFF		/* OR and and both operations reqd for 2 bits */
orr.w         R0, #0x00400000		/* Mode bits set to '01' makes the pin mode as output */
str.w		      R0, [R1]
ldr.w		      R1, =GPIOC_OTYPER	/* Type bit '0' configures pin for push-pull */
ldr.w		      R0, [R1]
and.w 	      R0, #0xFFFFF7FF
str.w		      R0, [R1]
ldr.w		      R1, =GPIOC_OSPEEDR	/* Two bits 22 to 23 for pin 11 */
ldr.w		      R0, [R1]
orr.w 	      R0, #0x00C00000		/* Speed bits set to '11' configures pin for high speed */
str.w		      R0, [R1]
ldr.w		      R1, =GPIOC_PUPDR	/* Two bits 22 to 23 for pin 11 */
ldr.w		      R0, [R1]
and.w	        R0, #0xFF3FFFFF		/* Clear bits to disable pullup-pulldown */
str.w		      R0, [R1]
movs          R0,R0
bx          lr

reg_init:
ldr.w		      R1, =DWT_CYCCNT	/* Reset cycle counter */
ldr.w		      R0, [R1]
and.w	        R0, #0x00000000
str.w		      R0, [R1]
ldr.w         R2,=const1
ldr.w         R2,=const2
ldr.w         R2,=const3
ldr.w         R2,=const4
ldr.w         R2,=const5
ldr.w         R2,=const6
ldr.w         R2,=const7
ldr.w         R2,=const8
ldr.w         R2,=const9
ldr.w         R2,=const10
ldr.w         R2,=const11
ldr.w         R2,=const12
and.w         R0, #0x00000000
and.w         R1, #0x00000000
and.w         R2, #0x00000000
orr.w         R0, #0x11111111
orr.w         R1, #0x00011000
orr.w         R2, #0x00033000
orr.w         R3, #0x00077000
orr.w         R4, #0x000FF000
orr.w         R5, #0x0AA00000
orr.w         R6, #0x09900000
orr.w         R7, #0x05A00000
orr.w         R8, #0x00000550
orr.w         R9, #0x00000AA0
ldr.w         R10, =FLASH_ACR          /* Flash config addr*/
nop.w
b.w           trig_pulse

trig_pulse:
ldr.w		      R12, =DWT_CTRL	/* Enable cycle counter */
ldr.w		      R11, [R12]
orr.w	        R11, CYCCNTENA
str.w		      R11, [R12]
ldr.w		      R12, =GPIOC_ODR /* Enable trig for pulse generator */
ldr.w		      R11, [R12]
orr.w 	      R11, #0x40
str.w		      R11, [R12]
ldr.w		      R12, =GPIOC_ODR
ldr.w		      R11, [R12]
and.w	        R11, #0xFFFFFFBF
str.w		      R11, [R12]
nop.w
nop.w
nop.w
b.w           trig_start_seq

trig_start_seq:
ldr.w		      R12, =GPIOC_ODR /* trig for sequence start at 1 */
ldr.w		      R11, [R12]
orr.w 	      R11, #0x800
str.w		      R11, [R12]
ldr.w         R11, [R10] /* cache config */
ldr.w         R12, =FLASH_ACR_ALLDSB   /* all disabled */
and.w         R11, R12
str.w         R11, [R10]
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
add.w	        R1,R1, #0x1
add.w	        R1,R1, #0x2
add.w	        R1,R1, #0x4
add.w	        R1,R1, #0x8
add.w	        R8,R8, #0x1
add.w	        R8,R8, #0x2
add.w	        R8,R8, #0x4
add.w	        R8,R8, #0x8
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
nop.w
ldr.w		      R11, =GPIOC_ODR /* trig of test sequence at 0 */
ldr.w		      R12, [R11]
and.w	        R12, #0xFFFFF7FF
str.w		      R12, [R11]
ldr.w		      R12, =DWT_CTRL	/* Disable cycle counter */
ldr.w		      R11, [R12]
and.w	        R11, CYCCNTDSB
str.w		      R11, [R12]
ldr.w         R10, =FLASH_ACR          /* reset instruction and data cache*/
ldr.w         R11, [R10]
ldr.w         R12, =FLASH_ACR_ALLDSB
and.w         R11, R12
str.w         R11, [R10]
ldr.w         R11, [R10]
ldr.w         R12, =FLASH_ACR_IDCRST
orr.w         R11, R12
str.w         R11, [R10]
nop.w
nop.w
nop.w
nop.w
nop.w
b.w           stop

.size       _start, . - _start

stop:
     b.w stop

.section    isr_vector
.word       0
.word       _start + 1
.word       _nmi_handler + 1
.word       _hard_fault + 1
.word       _memory_fault + 1
.word       _bus_fault + 1
.word       _usage_fault + 1

_dummy:
_nmi_handler:
_hard_fault:
_memory_fault:
_bus_fault:
_usage_fault:
      ldr.w		      R11, =GPIOC_ODR /* trig of test sequence at 0 */
      ldr.w		      R12, [R11]
      and.w	        R12, #0xFFFFF7FF
      str.w		      R12, [R11]
      ldr.w		      R12, =DWT_CTRL	/* Disable cycle counter */
      ldr.w		      R11, [R12]
      and.w	        R11, CYCCNTDSB
      str.w		      R11, [R12]
      b.w _dummy
