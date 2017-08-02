/*
 * Copyright (c) 2013 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * monitor_ap/kernel/system/inc/xmodem.h
 *
 * Date:        08-Oct-2013
 * Comment:
 * Reversion History:
 */

#ifndef _XMODEM_H_
#define _XMODEM_H_

#ifdef _XMODEM_C_
#define INTERFACE
#else
#define INTERFACE  extern
#endif

/* XMODEM error codes */
#define EXMSTART    (-1)
#define EXMCANCEL   (-2)
#define EXMUNKNOWN  (-3)

/* XMODEM status codes */
#define XMSUCCESS   (0)
#define XMREADY     (1)
#define XMNEXT      (2)
#define XMRESEND    (3)

typedef enum _xmodem_mode_enum
{
    XMODEM_NON_ACTIVE = 0,
    XMODEM,
} xmodem_mode_t;

INTERFACE xmodem_mode_t xmodem_mode;

INTERFACE void xmodem_init(void);
INTERFACE void xmodem_packet_receiver(int c);
INTERFACE int xmodem_file_download(void);
#if (XMODEM_DWNLD_ENABLE)
INTERFACE int xmodem_write_flash(void);
#endif

#undef INTERFACE
#endif /* _XMODEM_H_ */

