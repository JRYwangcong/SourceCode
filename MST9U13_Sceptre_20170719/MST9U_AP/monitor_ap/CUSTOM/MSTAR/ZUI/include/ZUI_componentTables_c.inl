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
//#include "MApp_Font.h"
#include "MApp_ZUI_APIcomponent.h"
#include "OSDcp_String_EnumIndex.h"
#include "OSDcp_Bitmap_EnumIndex.h"

//NOTE: when you add new OSD page, you need to add here!


#if (UI_SKIN_SEL==UI_SKIN_BM_800X600X1555)

#include "../res800x600x1555_BM/ZUI_components_c.inl"

#elif (UI_SKIN_SEL==UI_SKIN_NON_BM_800X600X1555)

#include "../res800x600x1555/ZUI_components_c.inl"

#endif
