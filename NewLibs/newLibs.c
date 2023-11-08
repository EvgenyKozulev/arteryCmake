#include "newLibs.h"

static int MemRegs;
usart_type *usart_x = STDOUT_DEBUG_PRINTF;
__attribute__((weak)) void _close(void)
{
    while (1)
    {
        /* code */
    }
}
__attribute__((weak)) void _lseek(void)
{
    while (1)
    {
        /* code */
    }
}
__attribute__((weak)) void _read(void)
{
    while (1)
    {
        /* code */
    }
}

// todo необходимо переделать на Semihosting
// Функция _ioputchar () in NewLib
__attribute__((weak)) int _write(__attribute__((unused)) int fd, const void *buffer, unsigned int count)
{
    uint8_t *buf = (uint8_t *)buffer;
    for (uint32_t i = 0; i < count; i++)
    {
        while (usart_flag_get(usart_x, USART_TDBE_FLAG) == RESET)
            ;
        usart_x->dt = buf[i];
    }
    return count;
}

__attribute__((weak)) void _exit(__attribute__((unused)) int st)
{
    while (1)
    {
        /* code */
    }
}

// todo необходимо сделать возможность выбора heap, определить статический массив
__attribute__((weak)) void *_sbrk(int incr)
{
    extern char __heap_start; // set by linker
    extern char __heap_end;   // set by linker

    static char *heap_end; /* Previous end of heap or 0 if none */
    char *prev_heap_end;

    if (0 == heap_end)
    {
        heap_end = &__heap_start; /* Initialize first time round */
    }

    prev_heap_end = heap_end;
    heap_end += incr;
    // check
    if (heap_end < (&__heap_end))
    {
        MemRegs += incr;
    }
    else
    {
        return (char *)-1;
    }
    return (void *)prev_heap_end;

} /* _sbrk () */

__attribute__((weak)) int _getpid(void)
{
    // static int ret = 0;
    return 0;
}

__attribute__((weak)) int _fstat(void)
{
    return 0;
}

__attribute__((weak)) int _isatty(void)
{
    return 0;
}

__attribute__((weak)) int _kill(__attribute__((unused)) int a)
{
    while (1)
        ;
    return 1;
}