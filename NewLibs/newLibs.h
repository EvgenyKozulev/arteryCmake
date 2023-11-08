#ifndef __NewLibCallStabs_h__
#define __NewLibCallStabs_h__
#include "at32f435_437_usart.h"
#include <stdint.h>
#warning "You use the functions of the standard library (NEWLIB), so you should implement these methods according to your own platform If you're sure you don't need to implement these, just add an empty implementation to the link add link."



void _close(void);
void _lseek(void);
void _read(void);
int _write(int fd, const void *buffer, unsigned int count);
void _exit(int st);
void *_sbrk(int incr);

int _getpid(void);
int _fstat(void);
int _isatty(void);
int _kill(int a);
#define STDOUT_DEBUG_PRINTF USART1
#endif //__NewLibCallStabs_h__