////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#include "ZUI_exefunc.h"

#define CLRKEY_LO_R (0x00)
#define CLRKEY_LO_G (0x00)
#define CLRKEY_LO_B (0x00)
#define CLRKEY_HI_R (0x00)
#define CLRKEY_HI_G (0x00)
#define CLRKEY_HI_B (0x00)
#define E_OSD_GWIN0 0
#define E_OSD_GWIN1 1

#if (UI_SKIN_SEL==UI_SKIN_BM_800X600X1555)

#include "../res800x600x1555_BM/OSDTbl_c.inl"

#elif (UI_SKIN_SEL==UI_SKIN_NON_BM_800X600X1555)

#include "../res800x600x1555/OSDTbl_c.inl"


#endif
