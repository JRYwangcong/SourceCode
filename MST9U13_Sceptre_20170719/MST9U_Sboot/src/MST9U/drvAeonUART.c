////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#include "datatype.h"
#include "drvAeonUART.h"
#include "drvGlobal.h"
#include "hwreg.h"


/*------------------------------------------------------------------------------*/
/*  MACROS                                                                      */
/*------------------------------------------------------------------------------*/

#define BOTH_EMPTY (UART_LSR_TEMT | UART_LSR_THRE)

#define WAIT_FOR_XMITR \
        do { \
                lsr = UART_REG8(UART_LSR); \
        } while ((lsr & BOTH_EMPTY) != BOTH_EMPTY)

#define WAIT_FOR_THRE \
        do { \
                lsr = UART_REG8(UART_LSR); \
        } while ((lsr & UART_LSR_THRE) != UART_LSR_THRE)

#define CHECK_FOR_CHAR (UART_REG8(UART_LSR) & UART_LSR_DR)

#define WAIT_FOR_CHAR \
         do { \
                lsr = UART_REG8(UART_LSR); \
         } while ((lsr & UART_LSR_DR) != UART_LSR_DR)

/*------------------------------------------------------------------------------*/
/*  UART APIS                                                                   */
/*------------------------------------------------------------------------------*/
void uart_init(U32 aeon_clock, U32 uart_clock)
{
    int divisor;

#ifdef __USE__AEON_UART__
    // Reset receiver and transmiter
    UART_REG8(UART_FCR) = UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT | UART_FCR_TRIGGER_1;

    // Set 8 bit char, 1 stop bit, no parity
    UART_REG8(UART_LCR) = UART_LCR_WLEN8 & ~(UART_LCR_STOP2 | UART_LCR_PARITY);
    divisor = ((aeon_clock) / (16 * uart_clock));

    UART_REG8(UART_LCR) |= UART_LCR_DLAB;
    UART_REG8(UART_DLL) = divisor & 0xFF;
    UART_REG8(UART_DLM) = (divisor >> 8) & 0xFF;
    UART_REG8(UART_LCR) &= ~(UART_LCR_DLAB);
#else
    // UART mode
    *(volatile U32*)(0xA0200000+(0x0F6E*4)) &= ~0x0F00;

    UART_REG8(UART_FCR) = UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT | UART_FCR_TRIGGER_14;
    /* Interrupt Enable Register */
    UART_REG8(UART_IER) = UART_IER_RDI;     // Receive data available
    /* Set 8 bit char, 1 stop bit, no parity */
    UART_REG8(UART_LCR) = UART_LCR_WLEN8 & ~(UART_LCR_STOP2 | UART_LCR_PARITY);
    /* Set baud rate */
    divisor = aeon_clock/(16 * uart_clock);
    UART_REG8(UART_LCR) |= UART_LCR_DLAB;
    UART_REG8(UART_DLL) = divisor & 0x000000ff;
    UART_REG8(UART_DLM) = (divisor >> 8) & 0x000000ff;
    UART_REG8(UART_LCR) &= ~(UART_LCR_DLAB);
#endif
}

void uart_putc(char c)
{
#if ENABLE_PRINT_MSG
#ifdef __USE__AEON_UART__
    unsigned char lsr;

    WAIT_FOR_THRE;
    UART_REG8(UART_TX) = c;
    WAIT_FOR_XMITR;
#else
    while (!(UART_REG8(UART_LSR) & UART_LSR_THRE));
    UART_REG8(UART_TX) = c;
#endif
#else
    c = c;
#endif
}

char uart_getc(void)
{
#ifdef __USE__AEON_UART__
    unsigned char lsr, ier;
    char c;

    ier = UART_REG8(UART_IER);
    UART_REG8(UART_IER) = 0;

    WAIT_FOR_CHAR;
    c = UART_REG8(UART_RX);

    UART_REG8(UART_IER) = ier;

    return c;
#else
    char c;

    while (!(UART_REG8(UART_LSR) & UART_LSR_DR));
    c = UART_REG8(UART_RX);

    return c;
#endif
}

char uart_kbhit(void)
{
    if (UART_REG8(UART_LSR) & UART_LSR_DR)
        return 1;
    else
        return 0;

}
void uart_interrupt_enable(BOOLEAN bEnable)
{
    if (bEnable)
        UART_REG8(UART_IER) = UART_IER_RDI;     // Receive data available
    else
        UART_REG8(UART_IER) = 0;                //Receive data disable
}

