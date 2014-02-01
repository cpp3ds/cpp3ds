ENTRY(_start)
SECTIONS
{
 . = 0x080C3EE0;
 .text : {
   *cpp3ds-crt0.o (.text)
   *(.text)
 }
 .data : { *(.data) }
 .bss : { *(.bss COMMON) }
 . = . + 0x4000; /* 64kB of Heap memory */
 _end = .;
 . = ALIGN(4);
 . = . + 0x1000; /* 4kB of stack memory */
 stack_top = .;
}
PROVIDE(__HEAP_START = _end );