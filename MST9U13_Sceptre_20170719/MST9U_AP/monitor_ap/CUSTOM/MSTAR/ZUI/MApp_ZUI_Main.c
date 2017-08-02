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

#define MAPP_ZUI_MAIN_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


///////////////////////////////////////////////////////////////////////////////////////////////////
// Description: Menu GUI window/control basic functions
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_ACTeffect.h"
//#include "msAPI_Timer.h"
#include "MApp_ZUI_ACTglobal.h"
//#include "MApp_ZUI_ACTsetclockfunc.h"
#include "OSDcp_Bitmap_EnumIndex.h"
#include "OSDcp_String_EnumIndex.h"

#include "apiGOP.h"
#include "ZUI_tables_h.inl"
#include "ZUI_tables_c.inl"
#include "ZUI_componentTables_h.inl"
#include "ZUI_componentTables_c.inl"
#include "ZUI_posTables_c.inl"
#include "ZUI_styleTables_c.inl"
#include "ZUI_alphaTables_c.inl"

#include "MApp_Key.h"
#include "UserPrefDef.h"
#include "menufunc.h"
#include "Keypad.h"
#include "Power.h"
#include "Global.h"
#include "MApp_ZUI_APIEasing.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------

#define ZUI_DBG(x) //x
#define ZUI_KEY_EASING 1
#define ZUI_KEY_EASING_TYPE E_EASING_LINEAR
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------
ZUI_STATE _eZUIState = E_ZUI_STATE_UNKNOW;
E_OSD_ID  _eActiveOSD = E_OSD_EMPTY;

extern BOOLEAN _MApp_ZUI_API_WindowProcOnTimer(void);
extern BOOLEAN _MApp_ZUI_API_WindowProcOnIdle(void);
//extern void MApp_ZUI_ACT_TerminateOSD(void);
extern BOOLEAN _MApp_ZUI_API_AllocateVarData(void);

#define ZUI_STATUS_MONITOR 0
#if ZUI_STATUS_MONITOR
//#include "msAPI_Timer.h"
U32 zui_monitor_timer = 0;
ZUI_STATE preZUIState = E_ZUI_STATE_UNKNOW;
#endif

extern void MApp_ZUI_ACTcoexistWin_Init(void);
U32 u32TimerDelay=FALSE;
U32 u32DetectTimer = FALSE;
BOOLEAN g_bSourceChangeFlag =0;
extern BOOLEAN g_bAutoAdjustFlag;

//debounce key
void _MApp_ZUI_KeyDebounce(void);
U16 u16DStepCurrent = 0;

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_Menu_MainTask
///  ZUI task/service entry
///
///  @return ZUI_RET latest state for ZUI task
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
ZUI_RET MApp_ZUI_MainTask(void)
{
    static ZUI_STATE s_eZUIPrevState = E_ZUI_STATE_UNKNOW;

#if ZUI_STATUS_MONITOR
    if (MsOS_GetSystemTime() - zui_monitor_timer > 10*1000)
    {
        printf("_eZUIState=%bx\n",_eZUIState);
        zui_monitor_timer = MsOS_GetSystemTime();
    }
    if(preZUIState != _eZUIState)
    {
        printf("_eZUIState=%bx\n",_eZUIState);
        preZUIState = _eZUIState;
    }
#endif

    switch (_eZUIState)
    {
        case E_ZUI_STATE_STANDBY:
            //PRINT("\n[MainTask] standby state");

            //note: don't dispatch this message, just let it go...

            break;

        case E_ZUI_STATE_TERMINATE:
            //2007/12/13: MApp_ZUI_API_EmptyMessageQueue();
            //2007/12/13: MApp_ZUI_API_TerminateGDI();
            //called inside MApp_ZUI_ACT_ShutdownOSD(): MApp_ZUI_ACT_TerminateOSD();
            MApp_ZUI_ACT_ShutdownOSD();

            s_eZUIPrevState = _eZUIState = E_ZUI_STATE_STANDBY;
            return RET_ZUI_EXIT;

        case E_ZUI_STATE_RUNNING:
            {
                MSG msg;

                //printf("[MainTask] runing state\n");
                //keep all messages are processed...
                while(MApp_ZUI_API_GetMessage(&msg))
                {
                    //printf("[taskX]\n");
                    //printf("[MainTask] got message win %u, msg %bu, wparam %lu\n", (U16)msg.hwnd, (U8)msg.message, (U32)msg.wParam);
                    MApp_ZUI_API_DispatchMessage (&msg);
                }
                //Note: in order to increase response time, so always check for invalide region   /*else*/

                //Note: if we are transfering to transition effect doing state, do nothing for this case:
                if (E_ZUI_STATE_RUNNING == _eZUIState)
                {
                    //do drawing first, at this time, message queue is empty..
                    _MApp_ZUI_API_WindowProcOnIdle();
                    _MApp_ZUI_API_WindowProcOnTimer();
                }
            }
            break;


        //case E_ZUI_STATE_TRANSEFF_BEGIN:
        //    MApp_Menu_TransInEffBegin();
        //    break;
        case E_ZUI_STATE_TRANSEFF_DOING:
            //2008/3/19: first time changed to E_ZUI_STATE_TRANSEFF_DOING
            //           process all remaining messages first..
            //           ex: MSG_NOTIFY_CONTENT_CHANGED
            if (_eZUIState != s_eZUIPrevState)
            {
                MSG msg;
                //printf("s_eZUIPrevState=%bx\n",s_eZUIPrevState);
                //keep all messages are processed...
                while(MApp_ZUI_API_GetMessage(&msg))
                {
                    MApp_ZUI_API_DispatchMessage (&msg);
                }
            }
            MApp_ZUI_ACT_TransitionEffectDoing();
            break;

        case E_ZUI_STATE_TRANSEFF_END:
            MApp_ZUI_ACT_TransitionEffectEnd();
            break;

        default:
            break;
    }
    s_eZUIPrevState = _eZUIState;

    //Event handle
    MApp_Event_Handler();
    return RET_ZUI_RUNNING;

}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_Init
///  ZUI system initialize
///
///  @return BOOLEAN TRUE for success, FALSE for fail
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_Init(void)
{
    HWND_MAX = HWND_MAINFRAME;
    OSDPAGE_BLENDING_ENABLE = FALSE;
    OSDPAGE_BLENDING_VALUE = ZUI_GWIN_BLENDING;
    OSDPAGE_ROTATE_ANGLE = OSD_ROTATE_0;
    g_bOSDPAGE_ROTATE_BUFFER = FALSE;
    if (!_MApp_ZUI_API_AllocateVarData()) //we have default HWND_MAX
    {
        ZUI_DBG_FAIL(printf("[ZUI]ALINIT\n"));
        ABORT();
        return FALSE;
    }

    _eZUIState = E_ZUI_STATE_STANDBY;
    _eActiveOSD = E_OSD_EMPTY;
    if (MApp_ZUI_API_InitMsgQueue() == FALSE)
    {
        ZUI_DBG_FAIL(printf("[ZUI]MQINIT\n"));
        ABORT();
        return FALSE;
    }

    //done in MApp_ZUI_API_InitMsgQueue(): MApp_ZUI_API_EmptyMessageQueue();
    MApp_ZUI_API_InitGDIVariables();

    if (g_stZuiMenuFlag.bDisplayLogo)
    {
        MApp_ZUI_ACT_StartupOSD(E_OSD_LOGO);
        
        _MApp_ZUI_API_WindowProcOnIdle();

        Power_TurnOnPanel();
    }

    //MApp_ZUI_ACTcoexistWin_Init();
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
///  public  MApp_ZUI_ProcessKey
///  send a key event into message queue
///
///  @param [in]       u8IRKey U8       key code
///
///  @return no return value
///
///  @author MStarSemi @date 2007/3/2
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_ProcessKey(U8 u8IRKey)
{
    extern VIRTUAL_KEY_CODE _MApp_ZUI_ACT_MapToVirtualKeyCode(U8 IR_key_code);

    if ((_eZUIState == E_ZUI_STATE_RUNNING) || (_eZUIState == E_ZUI_STATE_TRANSEFF_DOING)) //if (u8IRKey != KEY_NULL)
    {
        VIRTUAL_KEY_CODE vk_code = _MApp_ZUI_ACT_MapToVirtualKeyCode(u8IRKey);
        if (vk_code != VK_NULL)
        {
            //printf("[]st=%bu,at=%bu\n", _eZUIState, _eActiveOSD);
            ZUI_MSG(printf("KeyDown:%x\n", u8IRKey));
            MApp_ZUI_API_PostMessage(HWND_MAINFRAME, MSG_KEYDOWN, vk_code);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
///  public  MApp_ZUI_GetState
///  query current state of ZUI
///
///  @return ZUI_STATE current state
///
///  @author MStarSemi @date 2007/3/2
///////////////////////////////////////////////////////////////////////////////
ZUI_STATE MApp_ZUI_GetState(void)
{
    return _eZUIState;
}

///////////////////////////////////////////////////////////////////////////////
///  public  MApp_ZUI_GetActiveOSD
///  query for the current OSD page in ZUI system
///
///  @return E_OSD_ID current osd page
///
///  @author MStarSemi @date 2007/3/2
///////////////////////////////////////////////////////////////////////////////
U32 MApp_ZUI_GetActiveOSD(void)
{
    return (U32) _eActiveOSD;
}

void MApp_ZUI_MainLoop(void)
{
    Key_ScanKeypad();
    if (E_OSD_EMPTY == MApp_ZUI_GetActiveOSD())
    {
        switch (g_u8KeypadButton)//u8KeyCode)
        {
            case ZUI_KEY_MENU:
            case ZUI_KEY_UP:
            case ZUI_KEY_DOWN:
            case ZUI_KEY_INPUT_SOURCE:
            {
                if(!SyncLossState(SrcInputPortM))
                    MApp_ZUI_ACT_StartupOSD(E_OSD_MAIN_MENU);
                break;
            }
            case ZUI_KEY_POWER:
            {
                if(POWER_ON_FLAG)
                {//Power off
                    PowerOffSystem();
                }
                else
                {//Power on
                    PowerOnSystem();
                }
                 break;
            }

        default:
            break;
        }
        
    }

    _MApp_ZUI_KeyDebounce();
    
    MApp_ZUI_MainTask();
}

void MApp_Event_Handler(void)
{
	#if ENABLE_VGA_INPUT
    if(MsOS_Timer_DiffTimeFromNow(u32TimerDelay)>500 
		&& g_bAutoAdjustFlag 
		&& USER_PREF_INPUT_TYPE==INPUT_VGA
        &&(!SyncLossState(USER_PREF_WIN_SRC(USER_PREF_WIN_SEL)))) //do auto adjust
    {
        g_bAutoAdjustFlag = 0;
        if(AutoConfig())
        {
            MApp_ZUI_ACT_ShutdownOSD();
            MApp_ZUI_ACT_StartupOSD(E_OSD_MAIN_MENU);
        }
        else // do auto error
            MApp_ZUI_ACT_ShutdownOSD();
    }
	else
	#endif	
    if(MsOS_Timer_DiffTimeFromNow(u32TimerDelay)>3000 && g_bSourceChangeFlag)
    {
        g_bSourceChangeFlag =0;
        u32TimerDelay=0;
        MApp_ZUI_ACT_StartupOSD(E_OSD_DIRMSG_MENU); // Show input source type
    }
    else if(MsOS_Timer_DiffTimeFromNow(u32DetectTimer) > 5000)
    {
        u32DetectTimer=MsOS_GetSystemTime();
        if(SyncLossState(SrcInputPortM) && MApp_ZUI_GetActiveOSD() !=E_OSD_DIRMSG_MENU)
        {
            MApp_ZUI_ACT_ShutdownOSD();
            MApp_ZUI_ACT_StartupOSD(E_OSD_DIRMSG_MENU); // show no cable
        }

    }

}

void _MApp_ZUI_KeyDebounce(void)
{
#if ZUI_KEY_EASING     
    if(g_u8KeypadButton != ZUI_KEY_NULL && g_u8KeypadButton != ZUI_KEY_NOTHING)
    {
        if(g_u16ZUIDebounceTime == g_u16DTotalTime)
        {
            MApp_ZUI_ProcessKey(g_u8KeypadButton);//u8KeyCode);   
            g_u16ZUIDebounceTime--;
        }
        else if( g_u16ZUIDebounceTime == 0)
        {    
            MApp_ZUI_ProcessKey(g_u8KeypadButton);//u8KeyCode);   
            g_u8KeypadButton = ZUI_KEY_NULL;//KEY_NULL;//u8KeyCode = KEY_NULL;

            if(u16DStepCurrent < ZUI_DEBOUNCE_STEP_DURATION)
            {
                U16 temp = MApp_ZUI_API_Easing(ZUI_KEY_EASING_TYPE , u16DStepCurrent, 0, ZUI_DEBOUNCE_TIME, ZUI_DEBOUNCE_STEP_DURATION);
                ZUI_MSG(printf("temp = %d, cur = %d\n" , temp, u16DStepCurrent));
                g_u16DTotalTime = ZUI_DEBOUNCE_TIME - temp;
                g_u16ZUIDebounceTime = g_u16DTotalTime;
                u16DStepCurrent++;
            }  
        }
    }
    else
    {
        //reset
        g_u16ZUIDebounceTime = ZUI_DEBOUNCE_TIME;
        u16DStepCurrent = 0;
        g_u16DTotalTime = ZUI_DEBOUNCE_TIME;
    }
#else

    if(g_u8KeypadButton != ZUI_KEY_NULL && g_u8KeypadButton != ZUI_KEY_NOTHING)
    {
        if(g_u16ZUIDebounceTime == ZUI_DEBOUNCE_TIME)
        {
            MApp_ZUI_ProcessKey(g_u8KeypadButton);//u8KeyCode);   
            g_u16ZUIDebounceTime--;
        }
        else if( g_u16ZUIDebounceTime == 0)
        {    
            MApp_ZUI_ProcessKey(g_u8KeypadButton);//u8KeyCode);   
            g_u8KeypadButton = ZUI_KEY_NULL;//KEY_NULL;//u8KeyCode = KEY_NULL;
        }
    }
    else
    {
        g_u16ZUIDebounceTime = ZUI_DEBOUNCE_TIME;
    }
#endif
}
#undef MAPP_ZUI_MAIN_C
