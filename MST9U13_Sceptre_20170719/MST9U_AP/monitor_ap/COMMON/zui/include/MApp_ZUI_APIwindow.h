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

#ifndef _MAPP_ZUI_WINDOW_H
#define _MAPP_ZUI_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

    /**
     * \defgroup msgs Messages
     * @{
     */

#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIgdi.h"

#ifdef MAPP_ZUI_APIWINDOW_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define MAX_INVALIDATE_CLIP_RECT_NUMBER 3

typedef struct _INVALIDATE_CLIP
{
    BOOLEAN bNeedRepaint;
    RECT stRect[MAX_INVALIDATE_CLIP_RECT_NUMBER];
    U8 u8RectIndex;
}INVALIDATE_CLIP;


typedef enum _MESSAGE_ENUM
{
    MSG_NULLMSG = 0,

/**
 * \def MSG_KEYDOWN
 * \brief User presses a key down.
 *
 * This message is posted to the current active window when the user
 * presses a key down.
 *
 * \code
 * MSG_KEYDOWN
 * S32 scancode = (S32)wParam;
 * DWORD key_flags = (DWORD)lParam;
 * \endcode
 *
 * \param scancode The scan code of the pressed key.
 * \param key_flags The shift key status when this message occurred.
 *
 * \sa MSG_KEYUP, key_defs
 *
 * Example:
 *
 * \include keydown.c
 */
    MSG_KEYDOWN = 10,  //this message WILL ALWAYS put to message queue


/**
 * \def MSG_CREATE
 * \brief Indicates the window has been created, and gives you a chance to initialize your private objects.
 *
 * This messages is sent to the window after the window has been created and registered
 * to the system. You can initialize your own objects when you receive this message,
 * and return zero to the system in order to indicates the success of initialization.
 * If you return non-zero to the system after handled this message, the created window
 * will be destroied immediately.
 *
 * \code
 * MSG_CREATE for main windows:
 * PMAINWINCREATE create_info = (PMAINWINCREATE)lParam;
 *
 * MSG_CREATE for controls:
 * HWND parent = (HWND)wParam;
 * DWORD add_data = (DWORD)lParam;
 * \endcode
 *
 * \param create_info The pointer to the MAINWINCREATE structure which is passed to
 *        CreateMainWindow function.
 * \param parent The handle to the parent window of the control.
 * \param add_data The first additional data passed to CreateWindow function.
 *
 * \sa CreateMainWindow, CreateWindow, MAINWINCREATE
 */
    MSG_CREATE =        20,  //this message WILL NOT put to message queue

    //MSG_INITPANES =       21, //init pane/container, received after MSG_CREATE
    //MSG_DESTROYPANES =    22, //deinit pane/container, received before MSG_DESTROY

/**
 * \def MSG_DESTROY
 * \brief Indicates the window will be destroied.
 *
 * This message is sent to the window when \a DestroyMainWindow
 * or \a DestroyWindow is calling. You can destroy your private objects
 * when receiving this message.
 *
 * If you return non-zero to the system after handle this message, the process
 * of \a DestroyMainWindow and \a DestroyWindow will return immediately.
 *
 * \sa DestroyMainWindow, DestroyWindow
 */
    MSG_DESTROY =        23,  //this message WILL NOT put to message queue

/**
 * \def MSG_PAINT
 * \brief Sent to the window if the window contains an invalid region.
 *
 * This message is sent to the window if the window contains an invalid region.
 *
 * \code
 * MSG_PAINT
 * const CLIPRGN* inv_rgn = (const CLIPRGN*) lParam;
 * \endcode
 *
 * \param inv_rgn The pointer to the invalid region of the window.
 */
    MSG_PAINT =          30,  //this message WILL NOT put to message queue

/**
 * \def MSG_NOTIFY   for general notify message
 *
*/
    MSG_NOTIFY          = 40,  //this message WILL ALWAYS put to message queue

/**
 * \def MSG_NOTIFY_SHOW   notify if some window has been show
 *
*/
    MSG_NOTIFY_SHOW     =     41,

/**
 * \def MSG_NOTIFY_HIDE   notify if some window has been hide
 *
*/
    MSG_NOTIFY_HIDE     =     42,

/**
 * \def MSG_NOTIFY_ENABLE   notify if some window has been enable
 *
*/
    MSG_NOTIFY_ENABLE  =      43,

/**
 * \def MSG_NOTIFY_DISABLE   notify if some window has been disable
 *
*/
    MSG_NOTIFY_DISABLE=       44,

/**
 * \def MSG_NOTIFY_SETFOCUS   notify if some window has been focus
 *
*/
    MSG_NOTIFY_SETFOCUS=      45,

/**
 * \def MSG_NOTIFY_KILLFOCUS   notify if focus of some window has been killed
 *
*/
    MSG_NOTIFY_KILLFOCUS=     46,

/**
 * \def MSG_NOTIFY_MOVED   notify if some window has been moved
 *
*/
    MSG_NOTIFY_MOVED=           47,

/**
 * \def MSG_NOTIFY_KEYDOWN   notify if some window has been received some key
 *
*/
    MSG_NOTIFY_KEYDOWN=         48,

/**
 * \def MSG_NOTIFY_CONTENT_CHANGED   notify if content of some window has been changed
 *
*/
    MSG_NOTIFY_CONTENT_CHANGED= 49,

/**
 * \def MSG_TIMER
 * \brief Indicates a timer has expired.
 *
 * This message is sent to the window when a timer expired.
 *
 * \code
 * MSG_TIMER
 * S32 timer_id = (S32)wParam;
 * \endcode
 *
 * \param timer_id The identifier of the timer has expired.
 */
    MSG_TIMER           = 100,  //this message WILL NOT put to message queue

/**
 * \def MSG_EFFECT
 * \brief Indicates a effect to perform.
 *
 * This message is sent to the window when a timer expired.
 *
 * \code
 * MSG_EFFECT
 * U32 effect_id = (S32)wParam;
 * \endcode
 *
 * \param effect_id The effect to perform.
 */
    MSG_EFFECT          = 200,
    MSG_EFFECT_POPUP    = 201,
    MSG_EFFECT_ZOOMIN   = 202,
    MSG_EFFECT_ZOOMOUT  = 203,
    MSG_EFFECT_SPREADOUT= 204,
    MSG_EFFECT_ROLLUP   = 205,
    MSG_EFFECT_DYNATEXT_LEFT = 206,
    MSG_EFFECT_DYNATEXT_RIGHT = 207,
    MSG_EFFECT_SLIDEITEM    = 208,

/**
 * \def MSG_USER
 * \brief The first user-defined message.
 *
 * MediaPlayer reserved the range from 0x0800 to 0xEFFF for user-defined messages.
 * MSG_USER is the first user-defined message you can use it by your own.
 */
    MSG_USER             = 0x0800,
    MSG_USER1            = 0x0801,
    MSG_DECODE           = 0x0802,


    } MESSAGE_ENUM;

#define MSG_FIRSTMSG        0x00
#define MSG_LASTMSG         0xFF

#define MSG_FIRSTKEYMSG     10
#define MSG_LASTKEYMSG      19

//#define MSG_FIRSTPOSTMSG    0x0020
//#define MSG_LASTPOSTMSG     0x005F

#define MSG_FIRSTCREATEMSG 20
#define MSG_LASTCREATEMSG    29

#define MSG_FIRSTPAINTMSG   30
#define MSG_LASTPAINTMSG    39

//#define MSG_FIRSTSESSIONMSG 0x00D0
//#define MSG_LASTSESSIONMSG  0x00EF

//#define MSG_FIRSTWINDOWMSG  0x00F0
//#define MSG_LASTWINDOWMSG   0x010F

#define MSG_FIRSTCONTROLMSG 40
#define MSG_LASTCONTROLMSG    99

#define MSG_FIRSTSYSTEMMSG    100
#define MSG_LASTSYSTEMMSG     109

//#define MSG_FIRSTUSERMSG    0x0800
//#define MSG_LASTUSERMSG     0xEFFF

#define MAKEWPARAM(hiword, loword) (WPARAM)((U32)hiword<<16|(U32)loword)
#if !defined(HIWORD)
#define HIWORD(dword) ((dword >> 16)&0x0000FFFF)
#endif
#if !defined(LOWORD)
#define LOWORD(dword) (dword & 0x0000FFFF)
#endif

/**
 * The message structure.
 * \sa GetMessage, PostMessage, msgs
 */
typedef struct _MSG
{
    /** the handle to the window which receives this message. */
    HWND             hwnd;
    /** the message identifier. */
    MESSAGE_ENUM              message;
    /** The first parameter of the message (32-bit integer). */
    WPARAM           wParam;
    /** The second parameter of the message  */
    //LPARAM           lParam;

    //U32     time;

} MSG;
typedef MSG* PMSG;



INTERFACE BOOLEAN MApp_ZUI_API_InitMsgQueue (void);
//unused INTERFACE void MApp_ZUI_API_DestroyMsgQueue (void);

    /**
     * @}
     */


    /**
     * \defgroup styles Window styles
     * @{
     */

/**
 * \def WS_NONE
 * \brief None style.
 */
#define WS_NONE             0x00

/**
 * \def WS_VISIBLE
 * \brief Creates a window initially visible.
 */
#define WS_VISIBLE          BIT0

/**
 * \def WS_FOCUSABLE
 * \brief Creates a window can be focused.
 */
#define WS_FOCUSABLE        BIT1

/**
 * \def WS_DISABLED
 * \brief Creates a window initially disabled.
 */
#define WS_DISABLED         BIT2

/**
 * WS_MOVABLE
 * Creates a window can be moved.
 */
//all windows could be movable...#define WS_MOVABLE          0x0008

/**
 * WS_EX_TRANSPARENT
 * The window is transparent.
 * This style is only implemented for control.
 */
//#define WS_TRANSPARENT      0x20

#define WS_PERPIXELALPHA    BIT7 //old flag name
#define WS_SRCALPHAREPLACEDSTALPHA WS_PERPIXELALPHA //new flag name

    /** @} end of styles */


    /**
     * \defgroup msg_func Message process functions
     * @{
     */
INTERFACE BOOLEAN MApp_ZUI_API_GetMessage (PMSG pMsg);

INTERFACE BOOLEAN MApp_ZUI_API_PostMessage (HWND hWnd, MESSAGE_ENUM iMsg, WPARAM wParam);

INTERFACE S32 MApp_ZUI_API_SendMessage (HWND hWnd, MESSAGE_ENUM iMsg, WPARAM wParam);

INTERFACE S32 MApp_ZUI_API_DispatchMessage (PMSG pMsg);

INTERFACE void MApp_ZUI_API_EmptyMessageQueue (void);

INTERFACE BOOLEAN MApp_ZUI_API_IsExistRemainingMessage (MESSAGE_ENUM eMsg);

    /** @} end of msg_func */



/**
 * \def HWND_MAINFRAME
 * \brief Mainframe -- root of each osd page
 */
/**
 * \def HWND_INVALID
 * \brief for an invalid window handle
 */

#define HWND_MAINFRAME        0
#define HWND_INVALID        0xFFFF



    /**
     * \defgroup window_general_fns General window operations
     * @{
     */

/**
 * \def SW_HIDE
 * \brief try to hide window
 */
/**
 * \def SW_SHOW
 * \brief try to show window
 */
#define SW_HIDE              0x00
#define SW_SHOW              0x01
//#define SW_SHOWNORMAL        0x0100

INTERFACE BOOLEAN MApp_ZUI_API_ShowWindow (HWND hWnd, U32 iCmdShow);

INTERFACE BOOLEAN MApp_ZUI_API_EnableWindow (HWND hWnd, BOOLEAN fEnable);

INTERFACE BOOLEAN MApp_ZUI_API_IsWindowEnabled (HWND hWnd);

INTERFACE BOOLEAN MApp_ZUI_API_IsWindowVisible (HWND hWnd);

INTERFACE void MApp_ZUI_API_InvalidateRect (const RECT* prc);
INTERFACE void MApp_ZUI_API_InvalidateWindow(HWND hWnd);
INTERFACE void MApp_ZUI_API_InvalidateAllSuccessors (HWND hwnd);

INTERFACE BOOLEAN MApp_ZUI_API_IsSuccessor(HWND parent, HWND child);
INTERFACE BOOLEAN MApp_ZUI_API_IsParentFocused(HWND hwnd);
INTERFACE HWND MApp_ZUI_API_GetParent(HWND hwnd);
INTERFACE U8 MApp_ZUI_API_CountChildren(HWND hParent);
INTERFACE S16 MApp_ZUI_API_GetChildIndex(HWND hChild);
INTERFACE HWND MApp_ZUI_API_GetLastSuccessor(HWND hwnd);
INTERFACE U32 MApp_ZUI_API_GetWindowData(HWND hwnd);
INTERFACE U8 MApp_ZUI_API_GetWindowProcId(HWND hwnd);

    /**
     * @}
     */


    /**
     * \defgroup pos_func Window position/size management
     * @{
     */
INTERFACE void MApp_ZUI_API_MoveWindowByOffset (HWND hWnd, S16 LeftOffset, S16 TopOffset, S16 WidthOffset, S16 HeightOffset );
INTERFACE BOOLEAN MApp_ZUI_API_MoveWindow (HWND hWnd, const RECT* prc);
INTERFACE BOOLEAN MApp_ZUI_API_MoveAllSuccessors (HWND hWnd, S16 x, S16 y);

INTERFACE void MApp_ZUI_API_GetWindowRect(HWND hwnd, RECT * rect);
INTERFACE void MApp_ZUI_API_GetWindowInitialRect(HWND hwnd, RECT * rect);
INTERFACE void MApp_ZUI_API_RestoreAllWindowRect(void);
INTERFACE void MApp_ZUI_API_RestoreWindowRect(HWND hwnd);


    /**
     * @}
     */


/******************************** Timer Support ******************************/
    /**
     * \defgroup timer_fns Timer operations
     * @{
     */

INTERFACE BOOLEAN MApp_ZUI_API_SetTimer (HWND hWnd, U32 u32Id, U32 period);

INTERFACE BOOLEAN MApp_ZUI_API_KillTimer (HWND hWnd, U32 id);

INTERFACE BOOLEAN MApp_ZUI_API_ResetTimer (HWND hWnd, U32 id);

INTERFACE BOOLEAN MApp_ZUI_API_IsExistTimer (HWND hWnd, U32 id);

    /** @} end of timer_fns */


    /**
     * \defgroup focus_func Focus management functions
     * @{
     */
INTERFACE HWND MApp_ZUI_API_GetFocus(void);
INTERFACE BOOLEAN MApp_ZUI_API_SetFocus(HWND hwnd);
INTERFACE void MApp_ZUI_API_SetPaneFocus(HWND hwnd);
INTERFACE void MApp_ZUI_API_SetFocusByNav(HWND hwnd, U8 nav);

INTERFACE void MApp_ZUI_API_StoreFocusCheckpoint(void);
INTERFACE HWND MApp_ZUI_API_RestoreFocusCheckpoint(void);
INTERFACE HWND MApp_ZUI_API_GetFocusCheckpoint(void);
    /** @} end of focus */

#undef INTERFACE

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_WINDOW_H */

