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

#ifndef MAPP_INIT_H
#define MAPP_INIT_H

#include "datatype.h"
#include "msAPI_OSD.h"

#ifdef MAPP_INIT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define MAX_BITMAP  E_ZUI_BMP_MAX
INTERFACE BMPHANDLE Osdcp_bmpHandle[MAX_BITMAP];

INTERFACE void MApp_PreInit(void);

#undef INTERFACE
#endif

