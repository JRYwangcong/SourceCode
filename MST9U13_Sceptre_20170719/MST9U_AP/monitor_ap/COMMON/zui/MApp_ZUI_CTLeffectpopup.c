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

#define MAPP_ZUI_CTL_EFFECTPOPUP_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
//#include "msAPI_Timer.h"
#include "MsTypes.h"
#include "apiGOP.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIstyletables.h"

extern BOOLEAN _MApp_ZUI_API_WindowProcOnIdle(void);

static U8 u8PopupFbId = 0Xff;
///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_EffectPopupWinProc
///  Window Proc for "effect popup" control
///
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2009/2/11
///////////////////////////////////////////////////////////////////////////////
#define XFACTOR             2   //decimal
#define YFACTOR             4   //decimal
#define LOOP                10
#define DELAY               20  //ms

static void _MApp_ZUI_CTL_Effect_Popup(HWND hWnd, U8 xfac, U8 yfac, S8 loop, U32 ms)
{
    GRAPHIC_DC *pScrnDC = MApp_ZUI_API_GetScreenDC();
    MSAPI_OSDRegion CopyFB;
    MSAPI_OSDRegion PasteFB;
    const RECT *winrect = &GETWNDINITRECT(HWND_MAINFRAME);
    RECT rect;
    GOP_GwinFBAttr fbAttr;

    MApp_ZUI_API_GetWindowRect(hWnd, &rect);

    MApi_GOP_GWIN_GetFBInfo(pScrnDC->u8FbID, &fbAttr);

    u8PopupFbId = MApi_GOP_GWIN_GetFreeFBID();
    MApi_GOP_GWIN_CreateFB(u8PopupFbId, 0, 0, winrect->width, winrect->height, fbAttr.fbFmt);

    RECT fg;
    RECT bg;

    RECT_SET(fg, rect.left, rect.top, rect.width, rect.height);
    RECT_SET(bg, rect.left-((rect.width*xfac/10)/2), rect.top-((rect.height*yfac/10)/2), rect.width+(rect.width*xfac/10), rect.height+(rect.height*yfac/10));

    msAPI_OSD_SetClipWindow(0, 0, 0+fg.width-1, 0+fg.height-1);
    CopyFB.fbID=pScrnDC->u8FbID;
    CopyFB.x=fg.left;
    CopyFB.y=fg.top;
    CopyFB.width=fg.width;
    CopyFB.height=fg.height;

    PasteFB.fbID=u8PopupFbId;
    PasteFB.x=0;
    PasteFB.y=0;
    PasteFB.width=CopyFB.width;
    PasteFB.height=CopyFB.height;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    msAPI_OSD_SetClipWindow(0, fg.height, bg.width-1, fg.height+bg.height-1);
    CopyFB.fbID=pScrnDC->u8FbID;
    CopyFB.x=bg.left;
    CopyFB.y=bg.top;
    CopyFB.width=bg.width;
    CopyFB.height=bg.height;

    PasteFB.fbID=u8PopupFbId;
    PasteFB.x=0;
    PasteFB.y=fg.height;
    PasteFB.width=CopyFB.width;
    PasteFB.height=CopyFB.height;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    S8 i;
    for(i=loop; i >= 0; i--)
    {
        msAPI_OSD_SetClipWindow(0, (fg.height)+(bg.height), bg.width-1, (fg.height)+(bg.height)+bg.height-1);
        CopyFB.fbID=u8PopupFbId;
        CopyFB.x=0;
        CopyFB.y=fg.height;
        CopyFB.width=bg.width;
        CopyFB.height=bg.height;

        PasteFB.fbID=u8PopupFbId;
        PasteFB.x=0;
        PasteFB.y=(fg.height)+(bg.height);
        PasteFB.width=CopyFB.width;
        PasteFB.height=CopyFB.height;
        msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);


        RECT item;
        RECT_SET(item, ((rect.width*xfac/10)*(loop-i)/loop)/2, ((rect.height*yfac/10)*(loop-i)/loop)/2, rect.width+((rect.width*xfac/10)*i/loop), rect.height+((rect.height*yfac/10)*i/loop));

        msAPI_OSD_SetClipWindow(0+(item.left), (fg.height)+(bg.height)+(item.top), 
        0+(item.left)+item.width-1, (fg.height)+(bg.height)+(item.top)+item.height-1);
        CopyFB.fbID=u8PopupFbId;
        CopyFB.x=0;
        CopyFB.y=0;
        CopyFB.width=fg.width;
        CopyFB.height=fg.height;

        PasteFB.fbID=u8PopupFbId;
        PasteFB.x=0+(item.left);
        PasteFB.y=(fg.height)+(bg.height)+(item.top);
        PasteFB.width=item.width;
        PasteFB.height=item.height;
        msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

        msAPI_OSD_SetClipWindow(bg.left, bg.top, bg.left+bg.width-1, bg.top+bg.height-1);
        CopyFB.fbID=u8PopupFbId;
        CopyFB.x=0;
        CopyFB.y=(fg.height)+(bg.height);
        CopyFB.width=bg.width;
        CopyFB.height=bg.height;

        PasteFB.fbID=pScrnDC->u8FbID;
        PasteFB.x=bg.left;
        PasteFB.y=bg.top;
        PasteFB.width=CopyFB.width;
        PasteFB.height=CopyFB.height;
        msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

        MsOS_DelayTask(ms);
    }

    MApi_GOP_GWIN_DestroyFB(u8PopupFbId);

}

S32 MApp_ZUI_CTL_EffectPopupWinProc(HWND hWnd, PMSG pMsg)
{
    switch(pMsg->message)
    {
        case MSG_EFFECT_POPUP:
            _MApp_ZUI_API_WindowProcOnIdle();
            _MApp_ZUI_CTL_Effect_Popup(MApp_ZUI_API_GetFocus(), XFACTOR, YFACTOR, LOOP, DELAY);
            return 0;

        default:
            break;
    }
    return DEFAULTWINPROC(hWnd, pMsg);
}

#undef MAPP_ZUI_CTL_EFFECTPOPUP_C
