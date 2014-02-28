#ifdef SIMULATION
#include <sim3ds/utils.h>
#endif

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define TICKS_PER_SEC 0x80C0000
#define TICKS_PER_MSEC (TICKS_PER_SEC/1000)

uint64_t GetSystemTick();
void SleepThread(uint64_t ms);
void sleep(uint64_t ms);

void *operator new(size_t size);
 
void *operator new[](size_t size);
 
void operator delete(void *p);
 
void operator delete[](void *p);

void write_byte(int address, char byte);
void write_word(int address, int word);
int read_word(int address);
void* find_byte_sequence(char* sequence, int num, int base_address);
int strlen(char* string); 
void itoa(int n, char s[]);
void reverse(char s[]);

#endif
