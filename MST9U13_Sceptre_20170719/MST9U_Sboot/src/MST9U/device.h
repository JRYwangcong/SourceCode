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

#ifndef __PRANA_DEVICE_H__
#define __PRANA_DEVICE_H__

#include "datatype.h"

#define DEVICE_NULL ((device_t)0)

typedef unsigned long device_t;

//
// device PM event
//
typedef enum
{
    E_EVT_SUSPEND   = 1,
    E_EVT_SHUTDOWN  = 2,
} eDEVEVT;

//
// poll types
//
#define POLLIN      0x01
#define POLLOUT     0x02
#define POLLHUP     0x04

//
// device I/O operations
//
struct devio
{
    int     (*open) (device_t dev, int mode);
    int     (*close)(device_t dev);
    int     (*read) (device_t dev, char *buf, size_t len, int off);
    int     (*write)(device_t dev, const char *buf, size_t len, int off);
    int     (*poll) (device_t dev, int types);
    int     (*ioctl)(device_t dev, int request, unsigned long arg);
    int     (*event)(eDEVEVT evt);
};


#define DEVICE_MAGIC    (('D' << 24) | ('E' << 16) | ('V' << 8) | 'I')

//
// device structure
//
struct device
{
    int             magic;              // magic number
    int             refcnt;             // reference count
    int             flags;              // device characteristics
    struct devio    *devio;             // device i/o table
};

#define device_valid(dev) ((dev)->magic == DEVICE_MAGIC)

MST_BEGIN_DECL

int device_read(device_t dev, char *buf, size_t len);
int device_write(device_t dev, const char *buf, size_t len);
int device_poll(device_t dev, int types);

MST_END_DECL

#endif /* __PRANA_DEVICE_H__ */
