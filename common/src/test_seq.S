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

.set STATE_TARGET_POS, 0x20160000
.set STATE_TARGET_INIT, 0xAAAAAAAA
.set FAULT_CANARY, 0xFAFAFAFA


// - assumes r10 contains the next address at which to store the register value
// - clobbers r10, r11
.macro str_reg_state reg:req
  mov.n r11, \reg
  str.w r11, [r10], #4
.endm


// This macro assumes that r10 contains the address at which to store register
// states.
// Layout:
//   PC
//   sp
//   lr
//   r0 to r9 (included)
//   xPSR
// = 14 words

.macro store_cpu_state_in_ram
  ldr.w r10, =STATE_TARGET_POS

  // Setting it first because the rest will keep incrementing it.
  str_reg_state pc
 
  // Since this is not a function but a macro,
  // sp and lr should remain untouched.
  str_reg_state sp
  str_reg_state lr
  
  // Store unclobbered general-purpose registers.
  str_reg_state r0
  str_reg_state r1
  str_reg_state r2
  str_reg_state r3
  str_reg_state r4
  str_reg_state r5
  str_reg_state r6
  str_reg_state r7
  str_reg_state r8
  str_reg_state r9
  
  // r10, r11, r12 are always clobbered. No use storing them.
  
  // Store XPSR
  mrs r11, XPSR
  str.w r11, [r10], #4
.endm




run_test_seq:
  // For debugging.
  store_cpu_state_in_ram

  bl initialize_registers

  // Load base address for PIO{A,B,C} registers into R10.
  // pio_output_base is an externally-defined symbol,
  // that lets us choose one of those three PIO modules
  // at linking time.
  ldr  R10,=pio_output_base

  // trigger_pin_mask  and status_pin_mask are also externally defined.
  // To use pin N of a given PIO, you need a (1 << N) mask.

  ldr  R11,=trigger_pin_mask

  ldr  R12,=status_pin_mask

  //
  // 0 - 1 - 0 pulse trigger.
  //
  str.w  R11,[R10, #52]
  str.w  R11,[R10, #48]
  str.w  R11,[R10, #52]
  nop.w
  nop.w


  // Seq start
  str.w  R12,[R10, #48]

  bl run_attacked_code

  // Sequence end.
  str.w  R12,[R10, #52]

  // // Use this to check the exception handlers.
  //b please_hardfault
  
  
  store_cpu_state_in_ram
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




// Align these...
.thumb_func
Default_Handler:
  store_cpu_state_in_ram

  // Little canary value in r12 to say we faulted.
  // For attached debugging.
  ldr r12, =FAULT_CANARY
  
  // Loop forever
fault_handler_loop:
  b fault_handler_loop

.thumb_set UsageFault_Handler, Default_Handler
.thumb_set NMI_Handler, Default_Handler
.thumb_set MemManage_Handler, Default_Handler
.thumb_set HardFault_Handler, Default_Handler
.thumb_set BusFault_Handler, Default_Handler



// Keeping this handy for tests.
/*

  // Set N flag.
  movs r11, #-1
  movs r12, #-1
  // Set Q flag
  qadd r11, r11, r12
  // Set Z flag.
  movs r12, #0
  // Set C flag
  mov r11, #-1
  adds r11, r11, r11
*/