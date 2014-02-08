#ifndef TESTING
.section ".text"
.global _start
_start:
  BL main
  B .
#endif
