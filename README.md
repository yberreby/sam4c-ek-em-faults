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


