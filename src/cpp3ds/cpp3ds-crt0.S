#ifndef TESTING
.section ".text"
.global _start
.global __exidx_start
.global __exidx_end
__exidx_start:	
__exidx_end:
_start:
  BL main
  B .
#endif
