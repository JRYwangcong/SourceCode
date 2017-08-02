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

#define MAPP_ZUI_CTL_ROTATESTATIC_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "stdlib.h"
#include "stdio.h"
#include "datatype.h"
#include "MsTypes.h"
#include "apiGOP.h"
#include "msAPI_OSD.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIdraw.h"
#include "MApp_ZUI_APIcontrols.h"
#include "MApp_ZUI_ACTglobal.h"

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_CTL_RotateStaticWinProc
///  Window Proc for "rotate static" control,
///     which draw the original static content with 90 degree rotation
///
///  @param [in]       hWnd HWND     window handle
///  @param [in]       pMsg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////

S32 MApp_ZUI_CTL_RotateStaticWinProc(HWND hWnd, PMSG pMsg)
{
    switch(pMsg->message)
    {
        case MSG_PAINT:
            {
                //get buffer GC for offline drawing...
                PAINT_PARAM * param = (PAINT_PARAM*)pMsg->wParam;
                U8 u8OrigFbID = param->dc.u8FbID;
                RECT rcTemp;
                RECT * rcOrig;

                //step1: create temp buffer
                MSAPI_OSDRegion CopyFB;
                MSAPI_OSDRegion PasteFB;
                GFX_Buffer_Format gefmt;
                U16 u16AlignWidth;
                U16 u16AlignHeight;
                #if (MPLAYER_BITMAP_FORMAT == MPLAYER_I8)
                gefmt = GFX_FMT_I8;
                u16AlignWidth = ((param->rect->width + 0x0F) & ~(0x0F));
                u16AlignHeight = ((param->rect->height + 0x0F) & ~(0x0F));
                #else
                gefmt = GFX_FMT_RGB565;
                u16AlignWidth = ((param->rect->width * 2 + 0x0F) & ~(0x0F))/2;
                u16AlignHeight = ((param->rect->height * 2 + 0x0F) & ~(0x0F))/2;
                #endif
                param->dc.u8FbID = MApi_GOP_GWIN_GetFreeFBID();
                MApi_GOP_GWIN_CreateFB(
                    param->dc.u8FbID, 0, 0,u16AlignHeight, u16AlignWidth, gefmt);

                //step2: bitblt original background to temp buffer (270 degree rotate)
                CopyFB.fbID=u8OrigFbID;
                CopyFB.x=param->rect->left;
                CopyFB.y=param->rect->top;
                CopyFB.width=param->rect->width;
                CopyFB.height=param->rect->height;
                PasteFB.fbID=param->dc.u8FbID;
                PasteFB.x=0;
                PasteFB.y=0;
                PasteFB.width=CopyFB.width;
                PasteFB.height=CopyFB.height;
                MApi_GOP_GWIN_Switch2FB(param->dc.u8FbID);
                //we MUST reset clip window before msAPI_OSD_CopyRegion()
                msAPI_OSD_SetClipWindow(
                    0, 0, u16AlignHeight-1, u16AlignWidth-1);

                MApi_GFX_SetRotate(GEROTATE_270);
                msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
                MApi_GFX_SetRotate(GEROTATE_0);

                //step3: draw to temp buffer (swap width and height, and reset origon point)
                //printf("[]rotate=%bu,%bu\n", u8OrigFbID, param->dc.u8FbID);
                RECT_SET(rcTemp, 0, 0, param->rect->height, param->rect->width);
                rcOrig = param->rect;
                param->rect = &rcTemp;
                DEFAULTWINPROC(hWnd, pMsg);
                param->rect = rcOrig;

                //step4: rotate bitblt to target with 90 degree rotation
                CopyFB.fbID=param->dc.u8FbID;
                CopyFB.x=0;
                CopyFB.y=0;
                CopyFB.width=param->rect->height;
                CopyFB.height=param->rect->width;
                PasteFB.fbID=u8OrigFbID;
                PasteFB.x=param->rect->left;
                PasteFB.y=param->rect->top;
                PasteFB.width=CopyFB.width;
                PasteFB.height=CopyFB.height;
                MApi_GOP_GWIN_Switch2FB(u8OrigFbID);
                //we MUST reset clip window before msAPI_OSD_CopyRegion()
                msAPI_OSD_SetClipWindow(
                    0, 0, PANEL_WIDTH-1, PANEL_HEIGHT-1);/*
                    PasteFB.y, PasteFB.x,
                    PasteFB.y+PasteFB.height, PasteFB.x+PasteFB.width);*/

                MApi_GFX_SetRotate(GEROTATE_90);
                msAPI_OSD_CopyRegion(&CopyFB, &PasteFB);
                MApi_GFX_SetRotate(GEROTATE_0);

                MApi_GOP_GWIN_DestroyFB(param->dc.u8FbID);
                param->dc.u8FbID = u8OrigFbID;
            }
            return 0;

        default:
            break;
    }
    return DEFAULTWINPROC(hWnd, pMsg);
}

#undef MAPP_ZUI_CTL_ROTATESTATIC_C
