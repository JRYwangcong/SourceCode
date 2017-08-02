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

#define MAPP_ZUI_CTL_EFFECTZOOM_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
//#include "msAPI_Timer.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_APIstyletables.h"
#include "MsTypes.h"
#include "apiGOP.h"



static U8 u8ZoomFbId = 0Xff;

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_EffectZoomWinProc
///  Window Proc for "effect zoom" control
///
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2009/2/19
///////////////////////////////////////////////////////////////////////////////
#define LOOP                16
#define DELAY               10  //ms

static void _MApp_ZUI_CTL_Effect_ZoomIn(HWND hWnd, U8 loop, U32 ms)
{
    GRAPHIC_DC *pBuffDC = MApp_ZUI_API_GetBufferDC();
    GRAPHIC_DC *pScrnDC = MApp_ZUI_API_GetScreenDC();
    MSAPI_OSDRegion CopyFB;
    MSAPI_OSDRegion PasteFB;
    GOP_GwinFBAttr fbAttr;

    RECT *subrect = &GETWNDINITRECT(hWnd);
    RECT *orgrect;

    HWND last_succ = GETWNDLASTSUCCESSOR(hWnd);
    HWND child;

    for (child = hWnd; child <= last_succ; child++)
    {
        orgrect = &GETWNDINITRECT(child);
        MApp_ZUI_API_UnionRect(subrect, orgrect);
    }

    MApi_GOP_GWIN_GetFBInfo(pScrnDC->u8FbID, &fbAttr);
    u8ZoomFbId = MApi_GOP_GWIN_GetFreeFBID();
    MApi_GOP_GWIN_CreateFB(u8ZoomFbId, 0, 0,subrect->width*4, subrect->height, fbAttr.fbFmt);

    //we MUST reset clip window before msAPI_OSD_CopyRegion()
    msAPI_OSD_SetClipWindow(0, 0, subrect->width*4-1, subrect->height-1);

    PasteFB.fbID=u8ZoomFbId;
    PasteFB.x=subrect->width;
    PasteFB.y=0;
    PasteFB.width=subrect->width;
    PasteFB.height=subrect->height;

    CopyFB.fbID=pScrnDC->u8FbID;
    CopyFB.x=subrect->left;
    CopyFB.y=subrect->top;
    CopyFB.width=subrect->width;
    CopyFB.height=subrect->height;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    //we MUST reset clip window before msAPI_OSD_CopyRegion()
    msAPI_OSD_SetClipWindow(0, 0, subrect->width*4-1, subrect->height-1);

    PasteFB.fbID=u8ZoomFbId;
    PasteFB.x=subrect->width*2;
    PasteFB.y=0;
    PasteFB.width=subrect->width;
    PasteFB.height=subrect->height;

    CopyFB.fbID=pBuffDC->u8FbID;
    CopyFB.x=subrect->left;
    CopyFB.y=subrect->top;
    CopyFB.width=subrect->width;
    CopyFB.height=subrect->height;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    PasteFB.fbID=u8ZoomFbId;
    PasteFB.x=subrect->width*2+subrect->width;
    PasteFB.y=0;
    PasteFB.width=subrect->width/2+4;
    PasteFB.height=subrect->height/2+4;

    CopyFB.fbID=u8ZoomFbId;
    CopyFB.x=subrect->width*2;
    CopyFB.y=0;
    CopyFB.width=subrect->width;
    CopyFB.height=subrect->height;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    PasteFB.fbID=u8ZoomFbId;
    PasteFB.x=subrect->width*2+subrect->width+subrect->width/2+4;
    PasteFB.y=0;
    PasteFB.width=subrect->width/4+4;
    PasteFB.height=subrect->height/4+4;

    CopyFB.fbID=u8ZoomFbId;
    CopyFB.x=subrect->width*2+subrect->width;
    CopyFB.y=0;
    CopyFB.width=subrect->width/2+4;
    CopyFB.height=subrect->height/2+4;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    U8 i;
    for(i=0; i < loop; i++)
    {
        U16 ww = subrect->width*i/loop;
        U16 hh = subrect->height*i/loop;
        U16 xx = (subrect->width-ww)/2;
        U16 yy = (subrect->height-hh)/2;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(0, 0, subrect->width*4-1, subrect->height-1);

        PasteFB.fbID=u8ZoomFbId;
        PasteFB.x=0;
        PasteFB.y=0;
        PasteFB.width=subrect->width;
        PasteFB.height=subrect->height;

        CopyFB.fbID=u8ZoomFbId;
        CopyFB.x=subrect->width;
        CopyFB.y=0;
        CopyFB.width=subrect->width;
        CopyFB.height=subrect->height;
        msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

        if(ww > subrect->width/2 && hh > subrect->height/2)
        {
            PasteFB.fbID=u8ZoomFbId;
            PasteFB.x=xx;
            PasteFB.y=yy;
            PasteFB.width=ww;
            PasteFB.height=hh;

            CopyFB.fbID=u8ZoomFbId;
            CopyFB.x=subrect->width*2;
            CopyFB.y=0;
            CopyFB.width=subrect->width;
            CopyFB.height=subrect->height;
            msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
        }
        else if (ww > subrect->width/4 && hh > subrect->height/4)
        {
            PasteFB.fbID=u8ZoomFbId;
            PasteFB.x=xx;
            PasteFB.y=yy;
            PasteFB.width=ww;
            PasteFB.height=hh;

            CopyFB.fbID=u8ZoomFbId;
            CopyFB.x=subrect->width*2+subrect->width;
            CopyFB.y=0;
            CopyFB.width=subrect->width/2+4;
            CopyFB.height=subrect->height/2+4;
            msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
        }
        else if (ww > subrect->width/8 && hh > subrect->height/8)
        {
            PasteFB.fbID=u8ZoomFbId;
            PasteFB.x=xx;
            PasteFB.y=yy;
            PasteFB.width=ww;
            PasteFB.height=hh;

            CopyFB.fbID=u8ZoomFbId;
            CopyFB.x=subrect->width*2+subrect->width+subrect->width/2+4;
            CopyFB.y=0;
            CopyFB.width=subrect->width/4+4;
            CopyFB.height=subrect->height/4+4;
            msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
        }

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(subrect->left, subrect->top, subrect->left+subrect->width-1, subrect->top+subrect->height-1);

        PasteFB.fbID=pScrnDC->u8FbID;
        PasteFB.x=subrect->left;
        PasteFB.y=subrect->top;
        PasteFB.width=subrect->width;
        PasteFB.height=subrect->height;

        CopyFB.fbID=u8ZoomFbId;
        CopyFB.x=0;
        CopyFB.y=0;
        CopyFB.width=subrect->width;
        CopyFB.height=subrect->height;
        msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

        MsOS_DelayTask(ms);
    }

    MApi_GOP_GWIN_DestroyFB(u8ZoomFbId);

}

static void _MApp_ZUI_CTL_Effect_ZoomOut(HWND hWnd, U8 loop, U32 ms)
{
    GRAPHIC_DC *pBuffDC = MApp_ZUI_API_GetBufferDC();
    GRAPHIC_DC *pScrnDC = MApp_ZUI_API_GetScreenDC();
    MSAPI_OSDRegion CopyFB;
    MSAPI_OSDRegion PasteFB;
    GOP_GwinFBAttr fbAttr;

    RECT *subrect = &GETWNDINITRECT(hWnd);
    RECT *orgrect;

    HWND last_succ = GETWNDLASTSUCCESSOR(hWnd);
    HWND child;

    for (child = hWnd; child <= last_succ; child++)
    {
        orgrect = &GETWNDINITRECT(child);
        MApp_ZUI_API_UnionRect(subrect, orgrect);
    }

    MApi_GOP_GWIN_GetFBInfo(pScrnDC->u8FbID, &fbAttr);
    u8ZoomFbId = MApi_GOP_GWIN_GetFreeFBID();
    MApi_GOP_GWIN_CreateFB(u8ZoomFbId, 0, 0,subrect->width*4, subrect->height, fbAttr.fbFmt);

    //we MUST reset clip window before msAPI_OSD_CopyRegion()
    msAPI_OSD_SetClipWindow(0, 0, subrect->width*4-1, subrect->height-1);

    PasteFB.fbID=u8ZoomFbId;
    PasteFB.x=subrect->width;
    PasteFB.y=0;
    PasteFB.width=subrect->width;
    PasteFB.height=subrect->height;

    CopyFB.fbID=pBuffDC->u8FbID;
    CopyFB.x=subrect->left;
    CopyFB.y=subrect->top;
    CopyFB.width=subrect->width;
    CopyFB.height=subrect->height;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    //we MUST reset clip window before msAPI_OSD_CopyRegion()
    msAPI_OSD_SetClipWindow(0, 0, subrect->width*4-1, subrect->height-1);

    PasteFB.fbID=u8ZoomFbId;
    PasteFB.x=subrect->width*2;
    PasteFB.y=0;
    PasteFB.width=subrect->width;
    PasteFB.height=subrect->height;

    CopyFB.fbID=pScrnDC->u8FbID;
    CopyFB.x=subrect->left;
    CopyFB.y=subrect->top;
    CopyFB.width=subrect->width;
    CopyFB.height=subrect->height;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    PasteFB.fbID=u8ZoomFbId;
    PasteFB.x=subrect->width*2+subrect->width;
    PasteFB.y=0;
    PasteFB.width=subrect->width/2+4;
    PasteFB.height=subrect->height/2+4;

    CopyFB.fbID=u8ZoomFbId;
    CopyFB.x=subrect->width*2;
    CopyFB.y=0;
    CopyFB.width=subrect->width;
    CopyFB.height=subrect->height;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    PasteFB.fbID=u8ZoomFbId;
    PasteFB.x=subrect->width*2+subrect->width+subrect->width/2+4;
    PasteFB.y=0;
    PasteFB.width=subrect->width/4+4;
    PasteFB.height=subrect->height/4+4;

    CopyFB.fbID=u8ZoomFbId;
    CopyFB.x=subrect->width*2+subrect->width;
    CopyFB.y=0;
    CopyFB.width=subrect->width/2+4;
    CopyFB.height=subrect->height/2+4;
    msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

    U8 i;
    for(i=loop; i > 0; i--)
    {
        U16 ww = subrect->width*i/loop;
        U16 hh = subrect->height*i/loop;
        U16 xx = (subrect->width-ww)/2;
        U16 yy = (subrect->height-hh)/2;

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(0, 0, subrect->width*4-1, subrect->height-1);

        PasteFB.fbID=u8ZoomFbId;
        PasteFB.x=0;
        PasteFB.y=0;
        PasteFB.width=subrect->width;
        PasteFB.height=subrect->height;

        CopyFB.fbID=u8ZoomFbId;
        CopyFB.x=subrect->width;
        CopyFB.y=0;
        CopyFB.width=subrect->width;
        CopyFB.height=subrect->height;
        msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

        if(ww > subrect->width/2 && hh > subrect->height/2)
        {
            PasteFB.fbID=u8ZoomFbId;
            PasteFB.x=xx;
            PasteFB.y=yy;
            PasteFB.width=ww;
            PasteFB.height=hh;

            CopyFB.fbID=u8ZoomFbId;
            CopyFB.x=subrect->width*2;
            CopyFB.y=0;
            CopyFB.width=subrect->width;
            CopyFB.height=subrect->height;
            msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
        }
        else if (ww > subrect->width/4 && hh > subrect->height/4)
        {
            PasteFB.fbID=u8ZoomFbId;
            PasteFB.x=xx;
            PasteFB.y=yy;
            PasteFB.width=ww;
            PasteFB.height=hh;

            CopyFB.fbID=u8ZoomFbId;
            CopyFB.x=subrect->width*2+subrect->width;
            CopyFB.y=0;
            CopyFB.width=subrect->width/2+4;
            CopyFB.height=subrect->height/2+4;
            msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
        }
        else if (ww > subrect->width/8 && hh > subrect->height/8)
        {
            PasteFB.fbID=u8ZoomFbId;
            PasteFB.x=xx;
            PasteFB.y=yy;
            PasteFB.width=ww;
            PasteFB.height=hh;

            CopyFB.fbID=u8ZoomFbId;
            CopyFB.x=subrect->width*2+subrect->width+subrect->width/2+4;
            CopyFB.y=0;
            CopyFB.width=subrect->width/4+4;
            CopyFB.height=subrect->height/4+4;
            msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
        }

        //we MUST reset clip window before msAPI_OSD_CopyRegion()
        msAPI_OSD_SetClipWindow(subrect->left, subrect->top, 
        subrect->left+subrect->width-1, subrect->top+subrect->height-1);

        PasteFB.fbID=pScrnDC->u8FbID;
        PasteFB.x=subrect->left;
        PasteFB.y=subrect->top;
        PasteFB.width=subrect->width;
        PasteFB.height=subrect->height;

        CopyFB.fbID=u8ZoomFbId;
        CopyFB.x=0;
        CopyFB.y=0;
        CopyFB.width=subrect->width;
        CopyFB.height=subrect->height;
        msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);

        MsOS_DelayTask(ms);
    }

    MApi_GOP_GWIN_DestroyFB(u8ZoomFbId);

}


S32 MApp_ZUI_CTL_EffectZoomWinProc(HWND hWnd, PMSG pMsg)
{
    switch(pMsg->message)
    {
        case MSG_EFFECT_ZOOMIN:
            MApp_ZUI_API_PrepareBufferDC(MApp_ZUI_API_GetBufferDC());
            _MApp_ZUI_CTL_Effect_ZoomIn(hWnd, LOOP, DELAY);
            return 0;

        case MSG_EFFECT_ZOOMOUT:
            MApp_ZUI_API_PrepareBufferDC(MApp_ZUI_API_GetBufferDC());
            _MApp_ZUI_CTL_Effect_ZoomOut(hWnd, LOOP, DELAY);
            return 0;

        default:
            break;
    }
    return DEFAULTWINPROC(hWnd, pMsg);
}

#undef MAPP_ZUI_CTL_EFFECTZOOM_C
