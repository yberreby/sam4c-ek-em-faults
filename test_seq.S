.thumb
.syntax unified
.cpu cortex-m4


.global run_test_seq
.global please_hardfault

run_test_seq:
  mov.w         r1, 0
  b.w run_test_seq

please_hardfault:
  nop
  .word 0x4003ed70
  .word 0xDEADBEEF
  .word 0xFF00FF0F
