#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <cpp3ds/utils.h>

void *operator new(size_t size){
	return malloc(size);
}

void *operator new[](size_t size){
	return malloc(size);
}

void operator delete(void *p){
	free(p);
}
 
void operator delete[](void *p){
	free(p);
}

void write_word(int address, int word){
	int *a = (int*) address;
	*a = word;
}

void write_byte(int address, char byte){
	char *a = (char*) address;
	*a = byte;
}

int read_word(int address){
	int *a = (int*) address;
	return *a;
}

uint64_t GetSystemTick() {
	register unsigned long lo64 asm ("r0");
	register unsigned long hi64 asm ("r1");
	asm volatile ( "SVC 0x28" : "=r"(lo64), "=r"(hi64) );
	return ((uint64_t)hi64<<32) | (uint64_t)lo64;
}

void SleepThread(uint64_t ms) {
	ms *= 1000000;
	unsigned long hi32 = (ms >> 32);
	unsigned long lo32 = (ms & 0xffffffff);
	asm volatile ( "SVC 0x0A" : : "r"(lo32), "r"(hi32) );
}

void sleep(uint64_t ms) {
	uint64_t ticks = ms * TICKS_PER_MSEC;
	uint64_t start = GetSystemTick();
	while ((GetSystemTick() - start) < ticks); 
}

void* find_byte_sequence(char* sequence, int num, int base_address){
	char* c = (char*)base_address;  
	while (1){
		if (*c == sequence[0]){
			int i;
			for (i = 1; i < num; i++){
				if(*(c+i) == sequence[i]){

				} else {
					break;
				}
				return (void*)c;
			}
		}
		c++;
	}
}

int strlen(char* string){
	int i;
	for (i = 0; ; i++){
			if (string[i] == 0x00){
				return i;
			}
	}
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[]){
	int i, sign;

	if ((sign = n) < 0)  /* record sign */
		n = -n;          /* make n positive */
	i = 0;
	do {       /* generate digits in reverse order */
		s[i++] = n % 10 + '0';   /* get next digit */
	} while ((n /= 10) > 0);     /* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

/* reverse:  reverse string s in place */
void reverse(char s[]){
	int i, j;
	char c;

	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
