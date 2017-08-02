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

#define MAPP_ZUI_ALPHATABLE_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


///////////////////////////////////////////////////////////////////////////////////////////////////
// Description: tables for GUI window/control layout and styles
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIalphatables.h"

//NOTE: when you add new OSD page, you need to add here!

#if (ZUI_ENABLE_ALPHATABLE)
///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetDisableAlpha
///  query window disable alpha value
///
///  @param [in]       hWnd HWND     target window handle
///
///  @return U8  alpha value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

U8 MApp_ZUI_API_GetDisableAlpha (HWND hWnd)
{
    if (hWnd == HWND_INVALID)
        return 0;
#if ZUI_ENABLE_REDUCED_ALPHATABLE
    {
        WINDOWALPHADATA _ZUI_TBLSEG * pAlpha = g_GUI_WinAlphaDataList;
        while( pAlpha->hwnd != HWND_INVALID )
        {
            if (pAlpha->hwnd == hWnd)
                return pAlpha->u8DisableAlpha;
            pAlpha++;
        }
        return pAlpha->u8DisableAlpha;
    }
#else

    return GETWNDDISABLEALPHA(hWnd);

#endif
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetFocusAlpha
///  query window focus alpha value
///
///  @param [in]       hWnd HWND     target window handle
///
///  @return U8  alpha value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
U8 MApp_ZUI_API_GetFocusAlpha (HWND hWnd)
{
    if (hWnd == HWND_INVALID)
        return 0;

#if ZUI_ENABLE_REDUCED_ALPHATABLE
    {
        WINDOWALPHADATA _ZUI_TBLSEG * pAlpha = g_GUI_WinAlphaDataList;
        while( pAlpha->hwnd != HWND_INVALID )
        {
            if (pAlpha->hwnd == hWnd)
                return pAlpha->u8FocusAlpha;
            pAlpha++;
        }
        return pAlpha->u8FocusAlpha;
    }
#else

    return GETWNDFOCUSALPHA(hWnd);

#endif
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetNormalAlpha
///  query window normal alpha value
///
///  @param [in]       hWnd HWND     target window handle
///
///  @return U8  alpha value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
U8 MApp_ZUI_API_GetNormalAlpha (HWND hWnd)
{
    if (hWnd == HWND_INVALID)
        return 0;

#if ZUI_ENABLE_REDUCED_ALPHATABLE
    {
        WINDOWALPHADATA _ZUI_TBLSEG * pAlpha = g_GUI_WinAlphaDataList;
        while( pAlpha->hwnd != HWND_INVALID )
        {
            if (pAlpha->hwnd == hWnd)
                return pAlpha->u8NormalAlpha;
            pAlpha++;
        }
        return pAlpha->u8NormalAlpha;
    }
#else

    return GETWNDNORMALALPHA(hWnd);

#endif
}

#endif //ZUI_ENABLE_ALPHATABLE


#undef MAPP_ZUI_APITABLES_C

