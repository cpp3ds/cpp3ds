#if !defined(TESTING) && !defined(SIMULATION)

#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>

extern int  __HEAP_START;

extern "C" {

	void __cxa_pure_virtual() {
		// Error handling here
	}

	// TODO: Update syscalls when more hardware access is available.
	// https://sourceware.org/newlib/libc.html#Syscalls
	caddr_t _sbrk ( int incr ){
		static unsigned char *heap = NULL;
		unsigned char *prev_heap;
		if (heap == NULL) {
			heap = (unsigned char *)&__HEAP_START;
		}
		prev_heap = heap;
		/* check removed to show basic approach */
		heap += incr;
		return (caddr_t) prev_heap;
	}
	int _close(int file) { return -1; }
	int _fstat(int file, struct stat *st) { return 0; }
	int _isatty(int file) { return 1; }
	int _lseek(int file, int ptr, int dir) { return 0; }
	int _open(const char *name, int flags, int mode) { return -1; }
	int _read(int file, char *ptr, int len) { return 0; }
	int _write(int file, char *ptr, int len) { return 0; }
	int _getpid(){ return 1; }
	int _kill(int pid, int sig) { return -1; }
	void _exit(int code) { }

}

namespace std {
	// STL error throwing procedures
	void __throw_bad_alloc(){}
	void __throw_length_error(char const*){}
}

#endif
