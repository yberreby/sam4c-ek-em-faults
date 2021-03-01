.thumb
.syntax unified
.cpu cortex-m4

.include "constants.inc.s"

.global run_test_seq
.global please_hardfault

run_test_seq:
  bl initialize_registers
  bl enable_cycle_counter
  bl trigger_pulse_generator
  bl signal_seq_start
  bl run_attacked_code
  bl signal_seq_end
  bl disable_cycle_counter
  b stop


// XXX:
// Limitations of immediates?
// If so, why not ldr everything?
// Are we sure about the initial states of R0-R9?
initialize_registers:
  mov R0,#0
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
  and R0, #0x00000000
  and R1, #0x00000000
  and R2, #0x00000000
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
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  add R1,R1, #0x1
  add R1,R1, #0x2
  add R1,R1, #0x4
  add R1,R1, #0x8
  add R8,R8, #0x1
  add R8,R8, #0x2
  add R8,R8, #0x4
  add R8,R8, #0x8
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
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
