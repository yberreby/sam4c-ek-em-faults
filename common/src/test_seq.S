.thumb
.syntax unified
.cpu cortex-m4

.include "constants.inc.s"

.global run_test_seq
.global please_hardfault

run_test_seq:
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


trig:
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



// Just loop forever.
stop:
  b stop


// Gibberish to trigger a Hard Fault due to an undefined instruction
please_hardfault:
  nop
  // 32-bit permanently undefined instruction.
  .inst.w 0xF7F0A0000
  // 16-bit permanently undefined instruction.
  .inst.n 0xde00