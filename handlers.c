// TODO: use macros

#include "stdio_serial.h"

void our_eh(void)
{ 
  puts("something bad happened\r");
  while(1); 
}



/* Function prototype for exception table items (interrupt handler). */
typedef void (*IntFunc) (void);

void UsageFault_Handler(void) {
  our_eh();
}


void NMI_Handler(void) {
  our_eh();
}


void MemManage_Handler(void) {
  our_eh();
}


void HardFault_Handler(void) {
  our_eh();
}


void BusFault_Handler(void) {
  our_eh();
}



