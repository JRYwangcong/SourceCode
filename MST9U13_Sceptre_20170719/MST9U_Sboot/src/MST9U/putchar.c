////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///////////////////////////////////////////////////////////////////////////////
#include "stdio.h"
#include "device.h"

static device_t console;

int __putchar(int c);
int __kbhit(void);
int __getchar(void);

void setconsole(int device_handle)
{
    console = (device_t) device_handle;
}

int __putchar(int c)
{
    char x;

    if (console == DEVICE_NULL)
    {
        return -1;
    }
    if (c == '\n')
    {
        x = '\r';
        device_write(console, &x, 1);
    }
    x = c;
    device_write(console, &x, 1);
    return c;
}

int __kbhit(void)
{
    if (console != DEVICE_NULL)
    {
        return device_poll(console, POLLIN);
    }
    return 0;
}

int __getchar(void)
{
    char x;

    if ((console != DEVICE_NULL) && (device_read(console, &x, 1) == 1))
    {
        return (int) (unsigned char) x;
    }
    return -1;
}

int putchar(int)   __attribute__ ((weak, alias("__putchar")));
int kbhit(void)    __attribute__ ((weak, alias("__kbhit")));
int getchar(void)  __attribute__ ((weak, alias("__getchar")));
