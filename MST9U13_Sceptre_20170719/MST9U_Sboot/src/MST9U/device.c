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

#include "device.h"

#define DEVICE(dev) ((struct device *)(dev))

int device_read(device_t dev, char *buf, size_t len)
{
    struct device *d = DEVICE(dev);

    if (d->devio->read)
    {
        return d->devio->read(dev, buf, len, 0);
    }
    return -1;
}

int device_write(device_t dev, const char *buf, size_t len)
{
    struct device *d = DEVICE(dev);

    if (d->devio->write)
    {
        return d->devio->write(dev, buf, len, 0);
    }
    return -1;
}

int device_poll(device_t dev, int types)
{
    struct device *d = DEVICE(dev);

    if (d->devio->poll)
    {
        return d->devio->poll(dev, types);
    }
    return 0;
}
