#include "stdio.h"
#include "drvAeonUART.h"
#include "hwreg.h"

int printf(const char *fmt, ...)
{
#if ENABLE_PRINT_MSG
    int i, iret;
    char szPrint[PRINTF_CHAR_MAX];
    va_list ap;

    va_start(ap, fmt);
    iret = vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    for (i = 0; i < iret; i++)
    {
        uart_putc(szPrint[i]);
    }

    return iret;
#else
    fmt = fmt;
    return 0;
#endif
}

int printk(const char *fmt, ...) __attribute__ ((weak, alias("printf")));

