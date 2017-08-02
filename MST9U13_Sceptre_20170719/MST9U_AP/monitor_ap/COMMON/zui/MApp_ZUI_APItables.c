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

#define MAPP_ZUI_APITABLES_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


///////////////////////////////////////////////////////////////////////////////////////////////////
// Description: tables for GUI window/control layout and styles
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APItables.h"
#include "ZUI_tables_h.inl"

extern OSDPAGETABLE code OsdTables[];
extern OSDPAGE_TRANSEFF_TABLE code OsdTransEffTables[];
extern OSDPAGEWNDDRAWSTYLETABLE code OsdDrawStyleTables[];
extern OSDPAGEWNDPOSTABLE code OsdWndPosTables[];

HWND GetWndMaxOfOsdTable(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return OsdTables[id].WndMaxNum;
}

WINDOWDATA* GetWindowListOfOsdTable(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return OsdTables[id].WindowList;
}

WINDOWDRAWSTYLEDATA* GetWindowStyleOfOsdTable(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return OsdDrawStyleTables[id];
}

WINDOWPOSDATA* GetWindowPositionOfOsdTable(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return OsdWndPosTables[id];
}

WINDOWALPHADATA* GetWindowAlphaDataOfOsdTable(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return OsdTables[id].WindowAlphaDataList;
}

BOOLEAN IsBlendingEnabledOfOsdTable(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return OsdTables[id].bEnableAlpha;
}

U8 GetBlendingValueOfOsdTable(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return OsdTables[id].u8AlphaValue;
}

void SetOsdPageBlendingValue(BOOLEAN bEnable, U8 u8Value)
{
    OSDPAGE_BLENDING_ENABLE = bEnable;
    OSDPAGE_BLENDING_VALUE = u8Value;
}

U32 GetOsdTableTransEffIn(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return (U32) (OsdTransEffTables[id].EffectID_Open);
}

U32 GetOsdTableTransEffOut(U32 id)
{
    if ( (id <= E_OSD_EMPTY) || (id >= E_OSD_MAX) )
    {
        ZUI_MSG(printf("%ld is not valid osd id.\n", id));
        return FALSE;
    }

    return (U32) (OsdTransEffTables[id].EffectID_Close);
}

#undef MAPP_ZUI_APITABLES_C

