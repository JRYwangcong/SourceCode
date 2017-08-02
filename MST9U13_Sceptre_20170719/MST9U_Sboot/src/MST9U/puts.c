#include "stdio.h"
#include "drvAeonUART.h"

int puts(const char *s)
{
    int i;

    for (; (i = *s); s++)
    {
        uart_putc(i);
    }
    uart_putc('\n');

    return 0;
}

