.thumb
.syntax unified
.cpu cortex-m4

.global run_test_seq
.global please_hardfault

.global UsageFault_Handler
.global NMI_Handler
.global MemManage_Handler
.global HardFault_Handler
.global BusFault_Handler

const1:  .word   0x00000000
const2:  .word   0x00000000
const3:  .word   0x00000000
const4:  .word   0x00000000
const5:  .word   0xFFFFFFFF
const6:  .word   0xFFFFFFFF
const7:  .word   0xFFFFFFFF
const8:  .word   0xFFFFFFFF
const9:  .word   0xF010010F
const10: .word   0xF030030F
const11: .word   0xF070070F
const12: .word   0xF0F00F0F
const13: .word   0xF01FF10F
const14: .word   0xF03FF30F
const15: .word   0xF07FF70F
const16: .word   0xF0FFFF0F
const17: .word   0x11111118


piob_output_base:
  .word 0x400e1000

.set DWT_CTRL, 0xE0001000
.set DWT_CYCCNT, DWT_CTRL+0x4
.set DWT_CTRL_CYCCNTENA, 0x1
.set DWT_CTRL_CYCCNTDSB, 0xFFFFFFFE
.set PC_TARGET_POS, 0x20160000
.set PC_TARGET_INIT, 0xAAAAAAAA
.set FAULT_CANARY, 0xBACABACA

run_test_seq:
  // Reset PC value stored in RAM.
  ldr r1, =PC_TARGET_INIT
  ldr r2, =PC_TARGET_POS
  str r1, [r2]
  // End reset RAM PC.

  bl initialize_registers

  //
  // DWT_CTRL setup
  //

  // Store address of DWT_CTRL in r12,
  // and its initial value in r10.
  ldr r12, =DWT_CTRL
  ldr r10, [r12]

  //
  // Disable CYCCNT.
  //

  // Compute new value of DWT_CTRL, store it in r10.
  ldr r11, =DWT_CTRL_CYCCNTDSB
  and r10, r10, r11

  // Store it back.
  str r10, [r12]


  // Clear DWT_CYCCNT.
  ldr r12, =DWT_CYCCNT
  mov r11, #0
  str r11, [r12]


  // Constant corresponding to setting the DWT_CTRL_CYCCNTENA bit.
  ldr r12, =DWT_CTRL
  ldr r11, =DWT_CTRL_CYCCNTENA
  orr r10, r10, r11
  str r10, [r12]


  // End DWT_CTRL setup.

  //
  // Load base address for PIOB registers into R10.
  //
  ldr  R10,=piob_output_base
  ldr  R10,[R10]


  // Pulse trigger pin: PB12
  movs R11, #4096

  // Status pin:  PB6
  movs R12, #64

  
//trig:
  //
  // 0 - 1 - 0 pulse trigger.
  //
  str.w  R11,[R10, #52]
  str.w  R11,[R10, #48]
  str.w  R11,[R10, #52]

  nop.w
  nop.w

  // Sequence start.
  str.w  R12,[R10, #48]

  bl run_attacked_code

  // Sequence end.
  str.w  R12,[R10, #52]

  //b trig

  //
  // Tear down DWT_CTRL
  // - Disable cycle counter
  //

  // Store address of DWT_CTRL in r12,
  // and its initial value in r10.
  ldr r12, =DWT_CTRL
  ldr r10, [r12]

  //
  // Disable CYCCNT.
  //

  // Compute new value of DWT_CTRL, store it in r10.
  ldr r11, =DWT_CTRL_CYCCNTDSB
  and r10, r10, r11

  // Store it back.
  str r10, [r12]


  // End DWT_CTRL teardown.

  // Store PC value.
  ldr.w r10, =PC_TARGET_POS
  // A wide instruction CANNOT work
  // https://www.keil.com/support/man/docs/armasm/armasm_dom1361289878994.htm
  mov.n r11, pc
  str.w r11, [r10]

  // Use this to check the exception handlers.
  b please_hardfault

  // Just loop forever.
stop:
  b stop


initialize_registers:
  // Ensure that some constants end up in flash.
  ldr R2,=const1
  ldr R2,=const2
  ldr R2,=const3
  ldr R2,=const4
  ldr R2,=const5
  ldr R2,=const6
  ldr R2,=const7
  ldr R2,=const8
  ldr R2,=const9
  ldr R2,=const10
  ldr R2,=const11
  ldr R2,=const12

  // Clear canary registers
  and R0, #0x00000000
  and R1, #0x00000000
  and R2, #0x00000000
  and R2, #0x00000000
  and R3, #0x00000000
  and R4, #0x00000000
  and R5, #0x00000000
  and R6, #0x00000000
  and R7, #0x00000000
  and R8, #0x00000000
  and R9, #0x00000000

  // Set canary registers to known values.
  orr R0, #0x11111111
  orr R1, #0x00011000
  orr R2, #0x00033000
  orr R3, #0x00077000
  orr R4, #0x000FF000
  orr R5, #0x0AA00000
  orr R6, #0x09900000
  orr R7, #0x05A00000
  orr R8, #0x00000550
  orr R9, #0x00000AA0

  bx lr


run_attacked_code:
  // XXX: NOP count?
  nop.w
  nop.w
  nop.w
  nop.w
  nop.w
  nop.w
  nop.w
  nop.w
  add.w R1,R1, #0x1
  add.w R1,R1, #0x2
  add.w R1,R1, #0x4
  add.w R1,R1, #0x8
  add.w R8,R8, #0x1
  add.w R8,R8, #0x2
  add.w R8,R8, #0x4
  add.w R8,R8, #0x8
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
  // XXX: nop count?
  bx lr

// Gibberish to trigger a Hard Fault due to an undefined instruction
please_hardfault:
  nop
  // 32-bit permanently undefined instruction.
  .inst.w 0xF7F0A000
  // 16-bit permanently undefined instruction.
  .inst.n 0xde00


UsageFault_Handler:
NMI_Handler:
MemManage_Handler:
HardFault_Handler:
BusFault_Handler:
  // Store PC in RAM.
  ldr.w r10, =PC_TARGET_POS
  mov.n r11, pc
  str.w r11, [r10]

  // Little canary value in r12 to say we faulted.
  ldr r12, =FAULT_CANARY

  // Loop forever
fault_handler_loop:
  b fault_handler_loop
  