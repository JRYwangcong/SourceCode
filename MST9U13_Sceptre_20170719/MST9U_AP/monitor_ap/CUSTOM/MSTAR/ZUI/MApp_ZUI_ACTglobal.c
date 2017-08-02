////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define MAPP_ZUI_ACTGLOBAL_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "Board.h"
#include "datatype.h"
#include "MsCommon.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "MApp_GlobalSettingSt.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "ZUI_tables_h.inl"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_ACTeffect.h"
#include "MApp_ZUI_ACTglobal.h"

#include "MApp_ZUI_APIcontrols.h"
#include "ZUI_exefunc.h"
#include "MApp_Key.h"
#include "apiGOP.h"

#include "MApp_ZUI_ACTLogo.h"
#include "MApp_ZUI_ACTidle.h"
#include "MApp_ZUI_ACTMainMenu.h"
#include "MApp_ZUI_ACTAutoAdjust.h"
#include "MApp_ZUI_ACTDirMsgMenu.h"
#if (ZUI_BM)
#include "MApp_ZUI_BM_ActionManager.h"
#endif
extern ZUI_STATE _eZUIState;
extern E_OSD_ID _eActiveOSD;


/////////////////////////////////////////////////////////
// for customize

///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_ACT_MapToVirtualKeyCode
///     map system-dependent key code to system-independent key code
///     this mapping might be changed for different project/codebase
///
///  @param [in]       IR_key_code U8          system-dependent key code
///
///  @return VIRTUAL_KEY_CODE    system-independent key code
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
VIRTUAL_KEY_CODE _MApp_ZUI_ACT_MapToVirtualKeyCode(U8 IR_key_code)
{
    switch(IR_key_code)
    {
        case ZUI_KEY_POWER:             return VK_POWER;
        case ZUI_KEY_EXIT:              return VK_EXIT;
        case ZUI_KEY_MENU:              return VK_MENU;
        case ZUI_KEY_SELECT:            return VK_SELECT;
        case ZUI_KEY_MUTE:              return VK_MUTE;

        case ZUI_KEY_UP:                return VK_UP;
        case ZUI_KEY_DOWN:              return VK_DOWN;
        case ZUI_KEY_LEFT:              return VK_LEFT;
        case ZUI_KEY_RIGHT:             return VK_RIGHT;
        case ZUI_KEY_INPUT_SOURCE:      return VK_INPUT_SOURCE;
        case ZUI_KEY_PIP:               return VK_PIP;

        default:                    return VK_NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
///  private  _MApp_ZUI_ACT_AppClose
///  close an application, default procedure
///    NOTE: DO NOT MODIFY THIS FUNCTION!
///
///  @return no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
static void _MApp_ZUI_ACT_AppClose(void)
{
    HWND wnd;

    //UNUSED(sender);

    MApp_ZUI_API_SetFocus(HWND_INVALID);

    //for (wnd = HWND_MAX-1; wnd != HWND_INVALID; --wnd) //NOTE: from child to parent
    //  MApp_ZUI_API_PostMessage(wnd, MSG_DESTROYPANES, 0);
    for (wnd = HWND_MAX-1; wnd != HWND_INVALID; --wnd)
        MApp_ZUI_API_SendMessage(wnd, MSG_DESTROY, 0);

    //MApp_Menu_Album_Delete ();

    g_GUI_WindowList = 0;
    g_GUI_WinDrawStyleList =    0;
    g_GUI_WindowPositionList = 0;
    #if ZUI_ENABLE_ALPHATABLE
    g_GUI_WinAlphaDataList = 0;
    #endif
    HWND_MAX = HWND_MAINFRAME+1;
}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_StartupOSD
///  startup and setup a specific osd page
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       id E_OSD_ID     osd page index
///
///  @return BOOLEAN success or not
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_ACT_StartupOSD(U32 id)
{
    //2007/12/26: consider gop switching
    U8 u8PrevGopID = MApi_GOP_GWIN_GetCurrentGOP();

    ZUI_MSG(printf("[]startup:%u [now=%u,%u]\n", (U8)id, (U8)_eActiveOSD, (U8)_eZUIState));

    if (_eZUIState == E_ZUI_STATE_UNKNOW)
        return FALSE;

    if (_eZUIState == E_ZUI_STATE_RUNNING && _eActiveOSD == id)
        return TRUE; //already init

    if (u8PrevGopID != OSD_GOP_ID)
        MApi_GOP_GWIN_SwitchGOP(OSD_GOP_ID);


    if (_eZUIState != E_ZUI_STATE_STANDBY)
    {
        MApp_ZUI_ACT_ShutdownOSD();
    }

    //printf("sizeof(WINDOWALPHADATA)=%bu\n", (U8)sizeof(WINDOWALPHADATA));
    //printf("sizeof(WINDOWDRAWSTYLEDATA)=%bu\n", (U8)sizeof(WINDOWDRAWSTYLEDATA));
    //printf("sizeof(WINDOWDATA)=%bu\n", (U8)sizeof(WINDOWDATA));

    MApp_ZUI_API_SetFocus(HWND_INVALID);
    _eZUIState = E_ZUI_STATE_RUNNING;
    _eActiveOSD = (E_OSD_ID) id;
    MApp_ZUI_API_EmptyMessageQueue();
#if (ZUI_BM)
    //Behavior Manager -Open OSD Handler
    ZUI_MSG(printf("[ZUI]MApp_ZUI_ACT_OpenOSDHandler\n"));            
    MApp_ZUI_ACT_OpenOSDHandler((E_OSD_ID)id);
#else
    //NOTE: if an new OSD page added, please add here!
    switch(id)
    {
        case E_OSD_LOGO:
            MApp_ZUI_ACT_AppShowLogo();
            break;
        case E_OSD_IDLE:
            MApp_ZUI_ACT_AppShowIDLE();
            break;
        case E_OSD_MAIN_MENU:
            MApp_ZUI_ACT_AppShowMainMenu();
            break;
        case E_OSD_AUTO_ADJUST:
            MApp_ZUI_ACT_AppShowAutoAdjust();
            break;
        case E_OSD_DIRMSG_MENU:
            MApp_ZUI_ACT_AppShowDirMsgMenu();
            break;
        default:
            ZUI_DBG_FAIL(printf("[ZUI]GBLSHOW\n"));
            ABORT();
    }

#endif
    if (u8PrevGopID != OSD_GOP_ID)
        MApi_GOP_GWIN_SwitchGOP(u8PrevGopID);

    return TRUE; //handled
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_TerminateOSD
///  terminate current OSD page. default procedure
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @return no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_ACT_TerminateOSD(void)
{
    ZUI_MSG(printf("[]terminate\n"));
#if (ZUI_BM)
    //Behavior Manager -Terminate OSD Handler
    //MApp_ZUI_Custom_TerminateOSDHandler(_eActiveOSD);
    MApp_ZUI_ACT_TerminateOSDHandler(_eActiveOSD);
#else
    //2008-01-31: if (_eZUIState == E_ZUI_STATE_TERMINATE)
    {
        //NOTE: if an new OSD page added, please add here!
        switch(_eActiveOSD)
        {
            case E_OSD_LOGO:
                MApp_ZUI_ACT_TerminateLogo();
                break;
            case E_OSD_IDLE:
                MApp_ZUI_ACT_TerminateIDLE();
                break;
            case E_OSD_MAIN_MENU:
                MApp_ZUI_ACT_TerminateMainMenu();
                break;
            case E_OSD_AUTO_ADJUST:
                 MApp_ZUI_ACT_TerminateAutoAdjust();
                break;
            case E_OSD_DIRMSG_MENU:
                 MApp_ZUI_ACT_TerminateDirMsgMenu();
                break;
            default:
                ZUI_DBG_FAIL(printf("[ZUI]GBLTERM\n"));
                ABORT();

        }
    }
#endif
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_ShutdownOSD
///  shutdown and destory current osd page. (this function will change ZUI state)
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @return no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_ACT_ShutdownOSD(void)
{
    //2007/12/26: consider gop switching
    U8 u8PrevGopID = MApi_GOP_GWIN_GetCurrentGOP();

    ZUI_MSG(printf("[]shutdown [now=%u,%u]\n", (U8)_eActiveOSD, (U8)_eZUIState));

    if (_eZUIState == E_ZUI_STATE_UNKNOW)
        return;
    if (_eZUIState == E_ZUI_STATE_STANDBY )
        return;

    if (u8PrevGopID != OSD_GOP_ID)
        MApi_GOP_GWIN_SwitchGOP(OSD_GOP_ID);

    _MApp_ZUI_ACT_AppClose();
    MApp_ZUI_ACT_TerminateOSD();

    MApp_ZUI_API_TerminateGDI(); //shutdown right now! destroy gwin and goto standby state!

    MApp_ZUI_API_EmptyMessageQueue(); //clear message queue, before leave
    _eZUIState = E_ZUI_STATE_STANDBY;
    _eActiveOSD = E_OSD_EMPTY;

    if (u8PrevGopID != OSD_GOP_ID)
        MApi_GOP_GWIN_SwitchGOP(u8PrevGopID);

}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_HandleGlobalKey
///  key preprocess handler for each osd page. (will be called before all winproc)
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       key VIRTUAL_KEY_CODE      key code
///
///  @return BOOLEAN true for handled (don't dispatch continully)
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_ACT_HandleGlobalKey(VIRTUAL_KEY_CODE key)
{
    //note: this function will be called in running state

#if (ZUI_BM)
    //return MApp_ZUI_Custom_GlobalKeyHandler(_eActiveOSD, key);
    return MApp_ZUI_ACT_GlobalKeyHandler(_eActiveOSD, key);
#else
    //NOTE: if an new OSD page added, please add here!
    switch(_eActiveOSD)
    {
        case E_OSD_IDLE:
            return MApp_ZUI_ACT_HandleIDLEKey(key);
        case E_OSD_MAIN_MENU:
            return MApp_ZUI_ACT_HandleMainMenuKey(key);
        case E_OSD_DIRMSG_MENU:
            return MApp_ZUI_ACT_HandleDirMsgMenuKey(key);
        case E_OSD_AUTO_ADJUST:
            return MApp_ZUI_ACT_HandleAutoAdjustKey(key);
        case E_OSD_LOGO:
            break;
        default:
            ZUI_DBG_FAIL(printf("[ZUI]GBLKEY\n"));
            ABORT();
    }
    return FALSE;
#endif
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_PostMessageFilter
///  message filter for each window proc type. (used for reduce number of message in queue)
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW WINDOW PROC!
///
///  @param [in]       u8WinProcID U8     window proc ID
///  @param [in]       msg MESSAGE_ENUM    message type
///
///  @return BOOLEAN    true for accept, false for ignore
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_ACT_PostMessageFilter(U8 u8WinProcID, MESSAGE_ENUM msg)
{
    //note: to reduce the usage of message queue, we provide a filter for each type of window proc
    //note: you MUST add a switch case if you added a new window proc

    switch (u8WinProcID)
    {
        case EN_ZUI_DEFAULTWINPROC:
            return FALSE; //ignore all notify


        case EN_ZUI_MAINFRAMEWINPROC:
            //NOTE: main frame MUST accept key down message!
            //      otherwise, all key message will lost!
            return (msg == MSG_KEYDOWN);

        //case EN_ZUI_ACT_MAINPAGE_DYNAMICLIST_WINPROC:
        case EN_ZUI_DYNAMICLIST_WINPROC:
            return (msg == MSG_NOTIFY_SHOW ||
                msg == MSG_NOTIFY_HIDE ||
                msg == MSG_NOTIFY_KEYDOWN ||
                msg == MSG_NOTIFY_CONTENT_CHANGED ||
                msg == MSG_USER||
                msg == (MSG_USER1/*MSG_USER+1*/));

        case EN_ZUI_EFFECT_WINPROC:
            return (msg == MSG_EFFECT_POPUP ||
                        msg == MSG_EFFECT_SLIDEITEM ||
                        msg == MSG_EFFECT_ZOOMIN ||
                        msg == MSG_EFFECT_ZOOMOUT ||
                        msg == MSG_EFFECT_SPREADOUT ||
                        msg == MSG_EFFECT_ROLLUP);

#if 0 ///TODO:fixme
        //case EN_ZUI_TOPICON_WINPROC:
            //return (msg == MSG_NOTIFY_SETFOCUS||
                //msg == MSG_EFFECT_POPUP||
                //msg == MSG_EFFECT_SLIDEITEM);

        case EN_ZUI_INPUT_SOURCE_WINPROC:
            return (msg == MSG_NOTIFY_SETFOCUS ||
                msg == MSG_EFFECT_POPUP||
                msg == MSG_EFFECT_SLIDEITEM);

        case EN_ZUI_CHANNELINFOROOT_WINPROC:
            return (msg == MSG_NOTIFY_SHOW);

        case EN_ZUI_ACT_MAINPAGE_DYNAMICLIST_WINPROC:
        case EN_ZUI_DYNAMICLIST_WINPROC:
            return (msg == MSG_NOTIFY_SHOW ||
                msg == MSG_NOTIFY_HIDE ||
                msg == MSG_NOTIFY_KEYDOWN ||
                msg == MSG_NOTIFY_CONTENT_CHANGED ||
                msg == MSG_USER||
                msg == (MSG_USER1/*MSG_USER+1*/));

        case EN_ZUI_CIMMIMSGBOX_WINPROC:
        case EN_ZUI_MENUCOMMONDLGROOT_WINPROC:
            return (msg == MSG_NOTIFY_SHOW ||
                msg == MSG_NOTIFY_HIDE);

        case EN_ZUI_PREDITRENAMEINPUT_WINPROC:
        case EN_ZUI_MENUPWDINPUT_WINPROC:
        case EN_ZUI_MSGBOX_PWDINPUT_WINPROC:
        case EN_ZUI_CIMMIPWDINPUT_WINPROC:
            return (
                msg == MSG_NOTIFY_HIDE ||
                msg == MSG_NOTIFY_SETFOCUS ||
                msg == MSG_NOTIFY_KILLFOCUS);
#if (ENABLE_DTV_EPG)
        case EN_ZUI_EPG_TIMER_LIST_ITEM_WINPROC:
            return (
                msg == MSG_NOTIFY_SETFOCUS);

        case EN_ZUI_EPG_TIMER_SAVE_DLG_WINPROC:
            return (
                msg==MSG_NOTIFY_SHOW );
#endif  //#if (ENABLE_DTV_EPG)

#if 0
        case EN_ZUI_EFFECT_POPUP_WINPROC:
            return (msg == MSG_EFFECT_POPUP);

        case EN_ZUI_EFFECT_SLIDEITEM_WINPROC:
            return (msg == MSG_EFFECT_SLIDEITEM);

        case EN_ZUI_EFFECT_ZOOM_WINPROC:
            return (msg == MSG_EFFECT_ZOOMIN ||
                msg == MSG_EFFECT_ZOOMOUT);

        case EN_ZUI_EFFECT_ROLL_WINPROC:
            return (msg == MSG_EFFECT_SPREADOUT ||
                msg == MSG_EFFECT_ROLLUP);

#endif


        case EN_ZUI_FLIPPAGE_WINPROC:
            return (msg == MSG_EFFECT);

        case EN_ZUI_GRID_WINPROC:
            return (msg == MSG_NOTIFY_SETFOCUS ||
                msg == MSG_NOTIFY_KEYDOWN);
#endif


        case EN_ZUI_DYNAMICTEXTEFFECT_WINPROC:
            return (msg == MSG_NOTIFY_SHOW ||
                    msg == MSG_EFFECT_DYNATEXT_LEFT ||
                    msg == MSG_EFFECT_DYNATEXT_RIGHT ||
                    msg == MSG_NOTIFY_KEYDOWN);

        case EN_ZUI_ANIMATION_WINPROC:
            return (msg == MSG_CREATE ||
                msg == MSG_NOTIFY_HIDE||
                msg == MSG_TIMER||
                msg == MSG_PAINT);

        case EN_ZUI_MARQUEETEXT_WINPROC:
            return (msg == MSG_CREATE ||
                msg == MSG_DESTROY||
                msg == MSG_NOTIFY_HIDE||
                msg == MSG_TIMER||
                msg == MSG_PAINT);

#if 0 ///TODO:fixme
        case EN_ZUI_KEYBOARDINPUT_WINPROC:
            return (msg == MSG_CREATE ||
                msg == MSG_NOTIFY_SETFOCUS ||
                msg == MSG_NOTIFY_SHOW ||
                msg == MSG_NOTIFY_HIDE ||
                msg == MSG_NOTIFY_KEYDOWN);
#endif

        default:
            return FALSE; //ignore all notify
    }

}

//note: avoid painting loop waste too much time
//please be attention here, if you want input some special function
#if ZUI_EXCEPTION_MULTITASK
void MApp_ZUI_ACT_Exception_MultiTask(void)
{

}
#endif

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_ExecuteWndProc
///  dispatch a window message, depent on window proc ID
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW WINDOW PROC!
///
///  @param [in]       u8WinProcID U8   window proc ID
///  @param [in]       msg PMSG     message type
///
///  @return S32 message execute result
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
S32 MApp_ZUI_ACT_ExecuteWndProc(U8 u8WinProcID, PMSG msg)
{
    //note: don't use function pointer,
    //      so we use a enumeration for all listed controls (included user overrided window proc)

    //note: you MUST add a switch case if you added a new window proc

    /*/WndProc = g_GUI_WindowList[focus].WindowProc;
    if (GETWNDPROC(focus))
    {
        printf("[dispmsg3]focus=%bu,msg=%bu\n", (U8)focus, pMsg->message);
        iRet = GETWNDPROC(focus)(focus, pMsg);

    }*/
    switch (u8WinProcID)
    {
        case EN_ZUI_DEFAULTWINPROC:
            return DEFAULTWINPROC(msg->hwnd, msg);

        case EN_ZUI_MAINFRAMEWINPROC:
            return MAINFRAMEWINPROC(msg->hwnd, msg);

        case EN_ZUI_MARQUEETEXT_WINPROC:
            return MARQUEETEXT_WINPROC(msg->hwnd, msg);
#if 0 ///TODO:fixme
        //case EN_ZUI_TOPICON_WINPROC:
            //return TOPICON_WINPROC(msg->hwnd, msg);

        case EN_ZUI_INPUT_SOURCE_WINPROC:
            return INPUT_SOURCE_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_MAIN_PAGE_DIRECTION_ICON_WINPROC:
            //eturn MAINMENU_TRIICON_WINPROC(msg->hwnd, msg);
#endif

        case EN_ZUI_BGTANSPARENT_WINPROC:
            return BGTRANSPARENT_WINPROC(msg->hwnd, msg);

#if 0 ///TODO:fixme
        //case EN_ZUI_MENUITEMBAR_WINPROC:
        //    return MENUITEMBAR_WINPROC(msg->hwnd, msg);

        case EN_ZUI_BUTTONANICLICK_WINPROC:
            return BUTTONANICLICK_WINPROC(msg->hwnd, msg);

        case EN_ZUI_BUTTONANICLICKCHILD_WINPROC:
            return BUTTONANICLICKCHILD_WINPROC(msg->hwnd, msg);
#endif

        case EN_ZUI_DYNAMICTEXT_WINPROC:
            return DYNAMICTEXT_WINPROC(msg->hwnd, msg);

        case EN_ZUI_ANIMATION_WINPROC:
            return ANIMATION_WINPROC(msg->hwnd, msg);

        case EN_ZUI_DYNAMICTEXTEFFECT_WINPROC:
            return DYNAMICTEXTEFFECT_WINPROC(msg->hwnd, msg);

        case EN_ZUI_DYNAMICCOLORTEXT_WINPROC:
            return DYNAMICCOLORTEXT_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_MENUROOT_WINPROC:
        //    return MENUROOT_WINPROC(msg->hwnd, msg);

        case EN_ZUI_BALLPROGRESSBAR_WINPROC:
            return BALLPROGRESSBAR_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_BALLPROGRESSBAR_FOCUSSTYLE_WINPROC:
            //return BALLPROGRESSBAR_FOCUSSTYLE_WINPROC(msg->hwnd, msg);

        case EN_ZUI_RECTPROGRESSBAR_2_WINPROC:
            return RECTPROGRESSBAR_2_WINPROC(msg->hwnd, msg);

        case EN_ZUI_BUTTONCLICK_WINPROC:
            return BUTTONCLICK_WINPROC(msg->hwnd, msg);

        case EN_ZUI_ROTATESTATIC_WINPROC:
            return ROTATESTATIC_WINPROC(msg->hwnd, msg);

#if 0 ///TODO:fixme
#if ENABLE_DMP
        case EN_ZUI_DMP_FILE_SELECT_THUMBNAIL_WINPROC:
            return DMP_FILE_SELECT_THUMBNAIL_WINPROC(msg->hwnd, msg);
        case EN_ZUI_DMP_PROGRESS_WINPROC:
            return DMP_PROGRESS_WINPROC(msg->hwnd, msg);
        case EN_ZUI_PERCENTPROGRESSBAR_WINPROC:
            return PERCENTPROGRESSBAR_WINPROC(msg->hwnd, msg);
        case EN_ZUI_DMP_PLAY_STATUS_WINPROC:
            return DMP_PLAY_STATUS_WINPROC(msg->hwnd, msg);
        case EN_ZUI_DMP_ALERT_WINPROC:
            return DMP_ALERT_WINPROC(msg->hwnd, msg);
        case EN_ZUI_TEXTVIEWER_WINPROC:
            // TODO: fix me, need MApp_Text_CurPageReload?
            MApp_Text_CurPageReload();
            return TEXTVIEWER_WINPROC(msg->hwnd, msg);
        case EN_ZUI_PREVIEW_WINPROC:
            return DMP_PREVIEW_WINPROC(msg->hwnd, msg);
        case EN_ZUI_DMP_VOLUME_WINPROC:
            return DMP_VOLUME_WINPROC(msg->hwnd, msg);
        case EN_ZUI_DMP_EQ_PLAY_WINPROC:
            return DMP_EQ_PLAY_WINPROC(msg->hwnd, msg);
        case EN_ZUI_DMP_MARQUEE_WINPROC:
            return DMP_MARQUEE_WINPROC(msg->hwnd,msg);
        case EN_ZUI_DMP_MOVIERESUME_WINPROC:
            return DMP_MOVIERESUME_WINPROC(msg->hwnd, msg);
#endif

        //case EN_ZUI_MENUPICTUREPAGE1_WINPROC:
            //return MENUPICTUREPAGE1_WINPROC(msg->hwnd, msg);

        case EN_ZUI_CHANNELINFOROOT_WINPROC:
            return CHANNELINFOROOT_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_INPUTSOURCELIST_WINPROC:
        //    return INPUTSOURCELIST_WINPROC(msg->hwnd, msg);
#if ENABLE_DTV
        case EN_ZUI_DTVMANUALTUNING_WINPROC:
            return DTVMANUALTUNING_WINPROC(msg->hwnd, msg);
#endif

#if DVB_C_ENABLE
        case EN_ZUI_CADTVMANUALTUNING_WINPROC:
            return CADTVMANUALTUNING_WINPROC(msg->hwnd, msg);
#endif
        case EN_ZUI_ATVMANUALTUNING_WINPROC:
            return ATVMANUALTUNING_WINPROC(msg->hwnd, msg);

        case EN_ZUI_SCREENSAVER_WINPROC:
            return SCREENSAVER_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_AUDIOVOLUME_WINPROC:
        //    return AUDIOVOLUME_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_PROGRAMEDITROOT_WINPROC:
        //    return PROGRAMEDITROOT_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_AUDIOLANGUAGELIST_WINPROC:
        //    return AUDIOLANGUAGELIST_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_SUBTITLELANGUAGELIST_WINPROC:
        //    return SUBTITLELANGUAGELIST_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_HOTKEYOPTION_WINPROC:
        //    return HOTKEYOPTION_WINPROC(msg->hwnd, msg);

        //case EN_ZUI_MESSAGEBOX_WINPROC:
        //    return MESSAGEBOX_WINPROC(msg->hwnd, msg);
#endif

        case EN_ZUI_DYNAMICLIST_WINPROC:
            return DYNAMICLIST_WINPROC(msg->hwnd, msg);

#if 0 ///TODO:fixme
        case EN_ZUI_ACT_MAINPAGE_DYNAMICLIST_WINPROC:
            return MApp_ZUI_ACT_Mainpage_DynamicListWinProc(msg->hwnd, msg);

        //case EN_ZUI_INSTALLGUIDEROOT_WINPROC:
        //    return INSTALLGUIDEROOT_WINPROC(msg->hwnd, msg);

        case EN_ZUI_MENUCOMMONDLGROOT_WINPROC:
            return MENUCOMMONDLGROOT_WINPROC(msg->hwnd, msg);

        case EN_ZUI_MENUPWDINPUT_WINPROC:
            return MENUPWDINPUT_WINPROC(msg->hwnd, msg);

        case EN_ZUI_MSGBOX_PWDINPUT_WINPROC:
            return MSGBOXPWDINPUT_WINPROC(msg->hwnd, msg);
#if (ENABLE_CI)
        case EN_ZUI_CIMMIPWDINPUT_WINPROC:
            return CIMMIPWDINPUT_WINPROC(msg->hwnd, msg);
#endif
        //case EN_ZUI_CHANNELLISTROOT_WINPROC:
        //    return CHANNELLISTROOT_WINPROC(msg->hwnd, msg);
#endif

        case EN_ZUI_DYNAMICBITMAP_WINPROC:
            return DYNAMICBITMAP_WINPROC(msg->hwnd, msg);

#if 0 ///TODO:fixme
        case EN_ZUI_PREDITRENAMEINPUT_WINPROC:
            return PREDITRENAMEINPUT_WINPROC(msg->hwnd, msg);
#endif

        case EN_ZUI_KEYBOARDINPUT_WINPROC:
            return KEYBOARDINPUT_WINPROC(msg->hwnd, msg);

#if 0 ///TODO:fixme
        //case EN_ZUI_EPGROOT_WINPROC:
        //    return EPGROOT_WINPROC(msg->hwnd, msg);
#if (ENABLE_DTV_EPG)
  //      case EN_ZUI_EPGSERVICEBAR_WINPROC:
  //          return EPGSERVICEBAR_WINPROC(msg->hwnd, msg);
        case EN_ZUI_EPGPROGRAMMEGUIDE_TITLE_WINPROC:
            return EPGPROGRAMMEGUIDE_TITLE_WINPROC(msg->hwnd, msg);
        case EN_ZUI_EPGPROGRAMMEGUIDE_TIMEITEM_WINPROC:
            return EPGPROGRAMMEGUIDE_TIMEITEM_WINPROC(msg->hwnd, msg);
        case EN_ZUI_EPGTIMEITEMEVENT_WINPROC:
            return EPGTIMEITEMEVENT_WINPROC(msg->hwnd, msg);
        case EN_ZUI_EPGPROGRAMMEGUIDE_CHANNELITEM_WINPROC:
            return EPGPROGRAMMEGUIDE_CHANNELITEM_WINPROC(msg->hwnd, msg);
        case EN_ZUI_EPGCHANNELITEMEVENT_WINPROC:
            return EPGCHANNELITEMEVENT_WINPROC(msg->hwnd, msg);
//        case EN_ZUI_EPGTIMEPANE_WINPROC:
 //           return EPGTIMEPANE_WINPROC(msg->hwnd, msg);

        case EN_ZUI_EPGUPDATEALLTIMEITEM_WINPROC:
            return EPG_UPDATEALLTIMEITEM_WINPROC(msg->hwnd, msg);
#endif  //#if (ENABLE_DTV_EPG)


        case EN_ZUI_DYNAMICEPGPUNCTEXT_WINPROC:
            return DYNAMICEPGPUNCTEXT_WINPROC(msg->hwnd, msg);
#endif
        case EN_ZUI_DYNAMICPUNCTEXT_WINPROC:
            return DYNAMICPUNCTEXT_WINPROC(msg->hwnd, msg);

        case EN_ZUI_AUTOCLOSE_WINPROC:
            return AUTOCLOSE_WINPROC(msg->hwnd, msg);

#if 0 ///TODO:fixme
#if (ENABLE_CI)
        case EN_ZUI_CIMMIMSGBOX_WINPROC:
            return CIMMIMSGBOX_WINPROC(msg->hwnd, msg);

        case EN_ZUI_CIMMI_WINPROC:
            return CIMMI_WINPROC(msg->hwnd, msg);
#endif
        case EN_ZUI_MSGBOXTEXTPANE_WINPROC:
            return MSGBOXTEXTPANE_WINPROC(msg->hwnd, msg);

        case EN_ZUI_AUTOTUNINGSKIPATV_WINPROC:
            return AUTOTUNINGSKIPATV_WINPROC(msg->hwnd, msg);
        case EN_ZUI_AUTOTUNINGSKIPDTV_WINPROC:
            return AUTOTUNINGSKIPDTV_WINPROC(msg->hwnd, msg);
#if (ENABLE_DTV_EPG)
        case EN_ZUI_EPG_TIMER_LIST_ITEM_WINPROC:
            return EPG_TIMER_LIST_ITEM_WINPROC(msg->hwnd, msg);

        case EN_ZUI_EPG_TIMER_SAVE_DLG_WINPROC:
            return EPG_TIMER_SAVE_DLG_WINPROC(msg->hwnd, msg);
#endif  //#if (ENABLE_DTV_EPG)
#ifdef ENABLE_BT
        case EN_ZUI_BTTOP_WINPROC:
            return MApp_ZUI_ACT_BTTopWinProc(msg->hwnd, msg);
        case EN_ZUI_BTLINKPHOTO_WINPROC:
            return MApp_ZUI_ACT_BTLinkPhotoWinProc(msg->hwnd, msg);
        case EN_ZUI_BT_DESCRIPTION_WINPROC:
            return MApp_ZUI_ACT_BTDescriptionWinProc(msg->hwnd, msg);
#endif

#ifdef ENABLE_KTV
        case EN_ZUI_KTV_WINPROC:
            return MApp_ZUI_ACT_KTVWinProc(msg->hwnd, msg);
#endif

#ifdef ENABLE_GAME
        case EN_ZUI_GAME_BROWSER_WINPROC:
            {
                extern S32 MApp_ZUI_ACT_GameBrowseWinProc(HWND hwnd, PMSG msg);
                return MApp_ZUI_ACT_GameBrowseWinProc(msg->hwnd, msg);
            }
#endif
#endif

        case EN_ZUI_EFFECT_WINPROC:
            return EFFECT_WINPROC(msg->hwnd, msg);

#if 0 ///TODO:fixme
#if 0
        case EN_ZUI_EFFECT_POPUP_WINPROC:
            return EFFECTPOPUP_WINPROC(msg->hwnd, msg);

        case EN_ZUI_EFFECT_SLIDEITEM_WINPROC:
            return EFFECTSLIDEITEM_WINPROC(msg->hwnd, msg);

        case EN_ZUI_EFFECT_ZOOM_WINPROC:
            return EFFECTZOOM_WINPROC(msg->hwnd, msg);

        case EN_ZUI_EFFECT_ROLL_WINPROC:
            return EFFECTROLL_WINPROC(msg->hwnd, msg);
#endif

/*
        case EN_ZUI_MOTIONTRANS_ICON_WINPROC:
            return MApp_ZUI_CTL_MotionTrans_Icon_WinProc(msg->hwnd, msg);

        case EN_ZUI_MOTIONTRANS_WINPROC:
            return MApp_ZUI_CTL_MotionTransWinProc(msg->hwnd, msg);

        case EN_ZUI_FAKE3D_ICON_WINPROC:
            return MApp_ZUI_CTL_Fake3D_Icon_WinProc(msg->hwnd, msg);

        case EN_ZUI_FAKE3D_WINPROC:
            return MApp_ZUI_CTL_Fake3D_WinProc(msg->hwnd, msg);
*/



        case EN_ZUI_FLIPPAGE_WINPROC:
            return FLIPPAGE_WINPROC(msg->hwnd, msg);
#endif

        case EN_ZUI_GRID_WINPROC:
            return MApp_ZUI_CTL_GridWinProc(msg->hwnd, msg);

#if 0 ///TODO:fixme
        case EN_ZUI_INSTALL_GUIDE_WINPROC:
            return INSTALL_GUIDE_WINPROC(msg->hwnd, msg);
#endif

        default:
            ZUI_DBG_FAIL(printf("[ZUI]GBLWPROC\n"));
            ABORT();

    }
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_ExecuteWndAction
///  execute a specific action. (this will dispatch by osd page)
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       act U16       action ID
///
///  @return BOOLEAN    true for accept, false for ignore
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_ACT_ExecuteWndAction(U16 act)
{
    if (_eActiveOSD == E_OSD_EMPTY)
    {
        return FALSE;
    }

    //global action....NOTE: don't write special case here!!
    switch (act)
    {
        case EN_EXE_NAV_KEY_UP:
            MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_UP);
            return TRUE;

        case EN_EXE_NAV_KEY_DOWN:
            MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_DOWN);
            return TRUE;

        case EN_EXE_NAV_KEY_LEFT:
            MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_LEFT);
            return TRUE;

        case EN_EXE_NAV_KEY_RIGHT:
            MApp_ZUI_API_SetFocusByNav(MApp_ZUI_API_GetFocus(), NAV_RIGHT);
            return TRUE;


        case EN_EXE_NOTIFY_PARENT_KEY_SELECT:
            MApp_ZUI_API_PostMessage(
                MApp_ZUI_API_GetParent(MApp_ZUI_API_GetFocus()),
                MSG_NOTIFY_KEYDOWN, VK_SELECT);
            return TRUE;
        case EN_EXE_NOTIFY_PARENT_KEY_UP:
            MApp_ZUI_API_PostMessage(
                MApp_ZUI_API_GetParent(MApp_ZUI_API_GetFocus()),
                MSG_NOTIFY_KEYDOWN, VK_UP);
            return TRUE;

        case EN_EXE_NOTIFY_PARENT_KEY_DOWN:
            MApp_ZUI_API_PostMessage(
                MApp_ZUI_API_GetParent(MApp_ZUI_API_GetFocus()),
                MSG_NOTIFY_KEYDOWN, VK_DOWN);
            return TRUE;

        case EN_EXE_NOTIFY_PARENT_KEY_LEFT:
            MApp_ZUI_API_PostMessage(
                MApp_ZUI_API_GetParent(MApp_ZUI_API_GetFocus()),
                MSG_NOTIFY_KEYDOWN, VK_LEFT);
            return TRUE;

        case EN_EXE_NOTIFY_PARENT_KEY_RIGHT:
            MApp_ZUI_API_PostMessage(
                MApp_ZUI_API_GetParent(MApp_ZUI_API_GetFocus()),
                MSG_NOTIFY_KEYDOWN, VK_RIGHT);
            return TRUE;

        case EN_EXE_REPAINT_ALL:
            MApp_ZUI_API_InvalidateRect(NULL);
            return TRUE;

        default:
            break;

    }
#if (ZUI_BM)
    if( MApp_ZUI_ACT_ActionHandler( (EN_OSD_WNDEXEACTION_ENUM)act) )
    {
        return TRUE;
    }
#else
    //NOTE: if an new OSD page added, please add here!
    switch(_eActiveOSD)
    {
        case E_OSD_LOGO:
            return MApp_ZUI_ACT_ExecuteLogoAction(act);
        case E_OSD_IDLE:
            return MApp_ZUI_ACT_ExecuteIDLEAction(act);
        case E_OSD_MAIN_MENU:
            return MApp_ZUI_ACT_ExecuteMainMenuAction(act);
        case E_OSD_AUTO_ADJUST:
            return MApp_ZUI_ACT_ExecuteAutoAdjustAction(act);
        case E_OSD_DIRMSG_MENU:
            return MApp_ZUI_ACT_ExecuteDirMsgMenuAction(act);
        default:
          #if 0
            ZUI_DBG_FAIL(printf("[ZUI]GBLACT=%bu,%bu\n", _eActiveOSD, act));
          #else // Modified by coverity_0450
            ZUI_DBG_FAIL(printf("[ZUI]GBLACT=%d,%d\n", (U8)_eActiveOSD, act));
          #endif
            ABORT();

    }
#endif
    return FALSE; //false for continue passing key event....
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_GetDynamicText
///  dynamic text content provider for "dynamic text" control
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       hwnd HWND     window handle we are processing
///
///  @return LPTSTR    string content, return 0 if want to display empty string
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
LPTSTR MApp_ZUI_ACT_GetDynamicText(HWND hwnd)
{
    //NOTE: if an new OSD page added, please add here!
#if (ZUI_BM)
    LPTSTR text = NULL;
    text = MApp_ZUI_ACT_GetTextHandler(hwnd);
    return text;
#else
    switch(_eActiveOSD)
    {
        case E_OSD_IDLE:
            return MApp_ZUI_ACT_GetIDLEDynamicText(hwnd);
        case E_OSD_MAIN_MENU:
            return MApp_ZUI_ACT_GetMainMenuDynamicText(hwnd);
        case E_OSD_DIRMSG_MENU:
            return MApp_ZUI_ACT_GetDirMsgMenuDynamicText(hwnd);
        default:
            ZUI_DBG_FAIL(printf("[ZUI]GBLDTEXT\n"));
            ABORT();
    }
    return 0; //false for empty string

#endif
}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_GetDynamicBitmap
///  dynamic bitmap content provider for "dynamic bitmap" control
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       hwnd HWND     window handle we are processing
///  @param [in]       type DRAWSTYLE_TYPE     drawing type (normal/focus/disabled)
///
///  @return U16      bitmap index, return 0xFFFF if want to display empty bitmap
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
U16 MApp_ZUI_ACT_GetDynamicBitmap(HWND hwnd, DRAWSTYLE_TYPE type)
{
    //NOTE: if an new OSD page added, please add here!
    UNUSED(hwnd);
    UNUSED(type);
#if (ZUI_BM)
    U16 bitmap = MApp_ZUI_ACT_GetBitmapHandler(hwnd, type);
    return bitmap;
#else    
    switch(_eActiveOSD)
    {
        case E_OSD_IDLE:
            return MApp_ZUI_ACT_GetIDLEDynamicBitmap(hwnd, type);
        case E_OSD_MAIN_MENU:
            return MApp_ZUI_ACT_GetMainMenuDynamicBitmap(hwnd, type);
        default:
            ZUI_DBG_FAIL(printf("[ZUI]GBLDBMP\n"));
            ABORT();
    }
    return 0xFFFF; //for empty bitmap
#endif
}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_GetDynamicValue
///  integer number provider for progress bar/checkbox/radio button controls
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       hwnd HWND     window handle we are processing
///
///  @return S16     integer value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
S16 MApp_ZUI_ACT_GetDynamicValue(HWND hwnd)
{

    //NOTE: if an new OSD page added, please add here!
#if (ZUI_BM)
    return MApp_ZUI_ACT_GetValueHandler(hwnd);
#else
    UNUSED(hwnd);
    switch(_eActiveOSD)
    {
        case E_OSD_IDLE:
            return MApp_ZUI_ACT_GetIDLEDynamicValue(hwnd);
        case E_OSD_MAIN_MENU:
            return MApp_ZUI_ACT_GetMainMenuDynamicValue(hwnd);
        default:
            ZUI_DBG_FAIL(printf("[ZUI]GBLDVALUE\n"));
            ABORT();
    }
    return 0; //false for empty string
#endif
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_GetDynamicValue_2
///  2nd integer number provider for dual progress bar
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       hwnd HWND     window handle we are processing
///
///  @return S16     integer value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
S16 MApp_ZUI_ACT_GetDynamicValue_2(HWND hwnd)
{
    UNUSED(hwnd);

    switch(_eActiveOSD)
    {
#if 0 ///TODO: fixme
    #if ENABLE_PVR
        case E_OSD_PVR:
            return MApp_ZUI_ACT_GetPvrDynamicValue_2(hwnd);
    #endif
#endif

        default:
            return 100;  //background rect fill the whole line
    }
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_QueryDynamicListItemStatus
///  status provider for dynamic list controls
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       hwnd HWND     window handle we are processing
///
///  @return GUI_ENUM_DYNAMIC_LIST_STATE     item status
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
GUI_ENUM_DYNAMIC_LIST_STATE MApp_ZUI_ACT_QueryDynamicListItemStatus(HWND hwnd)
{
    //NOTE: if an new OSD page added, please add here!
    UNUSED(hwnd);
#if (ZUI_BM)
    return MApp_ZUI_ACT_GetDynamicListItemStatusHandler(hwnd);
#else
    switch(_eActiveOSD)
    {
        case E_OSD_IDLE:
            return MApp_ZUI_ACT_QueryIDLEItemStatus(hwnd);

        case E_OSD_MAIN_MENU:
            return MApp_ZUI_ACT_QueryMainMenuItemStatus(hwnd);

        default:
            ZUI_DBG_FAIL(printf("[ZUI]GBLDLIST\n"));
            ABORT();
    }
    return EN_DL_STATE_NORMAL;
#endif
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_ACT_GetDynamicColor
///  status provider for dynamic color text/marquee
///    NOTE: DO NOT MODIFY THIS FUNCTION UNLESS YOU WANT TO ADD NEW OSD PAGE HANDLER!
///
///  @param [in]       hwnd HWND     window handle we are processing
///  @param [in]       type DRAWSTYLE_TYPE     drawing type (normal/focus/disabled)
///  @param [in]       colorOriginal OSD_COLOR     original color
///
///  @return OSD_COLOR     color
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
OSD_COLOR MApp_ZUI_ACT_GetDynamicColor(HWND hwnd, DRAWSTYLE_TYPE type, OSD_COLOR colorOriginal)
{
    //NOTE: if an new OSD page added, please add here!
    UNUSED(hwnd);
    UNUSED(type);

#if (ZUI_BM)
    OSD_COLOR osdColor = MApp_ZUI_ACT_GetColorHandler(hwnd, type, colorOriginal);
    return osdColor;
#else    
    switch(_eActiveOSD)
    {
        case E_OSD_IDLE:
            return MApp_ZUI_ACT_GetIDLEDynamicColor(hwnd, type, colorOriginal);
        case E_OSD_MAIN_MENU:
            return MApp_ZUI_ACT_GetMainMenuDynamicColor(hwnd, type, colorOriginal);
        default:
            break;
    }
#endif
    return colorOriginal;
}


#undef MAPP_ZUI_ACTGLOBAL_C


