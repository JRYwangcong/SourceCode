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
////////////////////////////////////////////////////////////////////////////////

#ifndef __PRANA_FILE_H__
#define __PRANA_FILE_H__

/// @file file.h
/// @brief file like interface for driver and file

#include "datatype.h"

struct file;

/// file operations for device and file
struct file_operations
{
    int     (*open) (struct file *f, int mode);
    int     (*close)(struct file *f);
    int     (*read) (device_t dev, char *buf, size_t len, int off);
    int     (*write)(device_t dev, const char *buf, size_t len, int off);
    int     (*poll) (device_t dev, int types);
    int     (*ioctl)(device_t dev, int request, unsigned long arg);
    int     (*event)(int evt);          // device PM event
};

/// file structure
struct file
{
    int                     magic;      ///< magic number
    int                     refcnt;     ///< reference count
    int                     flags;      ///< file characteristics
    struct file_operations *fops;       ///< file operations
};

MST_BEGIN_DECL

/// open file
int open(char *filename, int mode);

/// close file
int close(int fd);

int read(int fd, char *buf, size_t len);
int write(int fd, const char *buf, size_t len);
int ioctl(int fd, int request, unsigned long arg);

MST_END_DECL

#endif /* __PRANA_FILE_H__ */
