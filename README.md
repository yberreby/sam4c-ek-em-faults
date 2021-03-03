# SAM4C-EK EM Fault Injection

```bash
# To run JLink's GDB server 
make server
# To connect to serial port
make serial
# To run gdb
make debug_sram
# or
make debug_flash
```


## Documentation

[Everything for the SAM4C](https://asf.microchip.com/docs/latest/sam4c/html/).

- [IOPORT](https://asf.microchip.com/docs/latest/sam4c/html/group__ioport__group.html): for GPIO
- [sysclk](https://asf.microchip.com/docs/latest/sam4c/html/group__sysclk__group.html): not sure
- [PMC](https://asf.microchip.com/docs/latest/sam4c/html/group__sam__drivers__pmc__group.html):
  seems to be where to set the clocks
- [AES](https://asf.microchip.com/docs/latest/sam4c/html/group__asfdoc__sam__drivers__aes__group.html)
- [EEFC](https://asf.microchip.com/docs/latest/sam4c/html/group__sam__drivers__efc__group.html)
- [EFS](https://asf.microchip.com/docs/latest/sam4c/html/group__sam__services__flash__efc__group.html)
- [irq](https://asf.microchip.com/docs/latest/sam4c/html/group__interrupt__group.html): global interrupt management
  - enable/disable IRQ
  - set specific handlers
    - can't find the definitions for now
      - sam/utils/cmsis/sam4c/include/sam4c4c_0.h or sam/utils/cmsis/sam4c/include/sam4c4c_1.h?
      - `grep HardFault_IRQn` to find the definitions


## Things we'll want to do

Disable I-cache, D-cache; reset them.

Disable interrupts


## Unclear



Les ES doivent être échantillonnées à la même fréquence que le CPU.

Trouver pins avec connecteur sur la carte.
En prévoir 2 :
- 1 qui va faire un trig: 0 1 0
- 1 qui reste à 1 quand on a fini la séquence
- 


Prendre la fréquence minimum possible en gardant égalité.
  16 MHz actuellement sur STM32

Les 2 oscillateurs nous intéressent.


DWT => regarder au niveau de la doc Cortex M4


p. 271 datasheet: SysTick
=> chercher dans CMSIS

Garder un oeil sur les interruptions / exceptions
  => il doit y en avoir un registre général

équivalent du PSR?


Dans un premier temps, sur un coeur:
- horloges
- trigger en sortie sur GPIO
- code de test


Plus tard : possibilité d'autres coeurs ?

labo 3A113: avec Oualid

Regarder horloge pour AES (est-ce qu'il tourne vite ?)
  p. 574

p.50 : Peripherals Identifiers (AES)





Prendre ce qu'il y a de plus simple, entre 2 coeurs et AES



## Important information

The PIO controller is responsible for GPIO and is fed directly from the PMC by
the Peripheral Clock.
It can be programmed before setting up the clock.

The peripheral clock needs to be enabled.
=> easy way to do this in C: IOPORT/ioport_init()

TODO: find pins.
  User guide p. 24


32.4.1 Pin multiplexing

might need to set pin mode
  https://asf.microchip.com/docs/latest/sam4c/html/group__ioport__group.html#ioport_modes


open drain seems to be for 32.5.6 Multi-Drive Control: unneeded here

We'll need a pull-down?

PIO_SODR = Set Output Data Register
CODR = Clear Output

PIO_PSR is controlled by PIO_PER / PDR (if disabled, peripheral has control)

(TODO: write an article about this)

"Enabling the pull-down resistor while the pull-up resistor is still enabled is
not possible."


"Figure 32-3 shows how the outputs are driven either by writing PIO_SODR or
PIO_CODR, or by directly writingPIO_ODSR. This last case is valid only if the
corresponding bit in PIO_OWSR is set"

use PIO_OWER to enable writing, then write to PIO_ODSR


edge levels: irrelevant, no interrupts here

high-drive: more current, such as for LEDs



TODO: let's drive an LED :p

TODO: let's drive clock directly to an output

TODO: use ASM for output


where's the ground?

maybe redo shit with Arduino

bypass mode is to connect an external oscillator



5.3.2: Core 0 Cache Controller (CMCC0) is enabled (only used if the application link address for the Core 0 is 0x11000000)



---

TODO: set wait states

TODO: find out how to use external oscillator


