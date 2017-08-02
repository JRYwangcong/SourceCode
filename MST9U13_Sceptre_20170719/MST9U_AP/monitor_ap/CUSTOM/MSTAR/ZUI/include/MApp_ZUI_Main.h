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

#ifndef _MAPP_ZUI_MAIN_H
#define _MAPP_ZUI_MAIN_H

#include "datatype.h"
#include "Board.h"
#include "debug.h"


#ifdef MAPP_ZUI_APIWINDOW_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

typedef enum
{
    E_ZUI_STATE_UNKNOW=0,
    E_ZUI_STATE_STANDBY=1,
    E_ZUI_STATE_RUNNING=2,
    E_ZUI_STATE_TRANSEFF_DOING=3,
    E_ZUI_STATE_TRANSEFF_END=4,
    E_ZUI_STATE_TERMINATE=5,
    E_ZUI_STATE_MAX=6
}ZUI_STATE;

typedef enum
{
    RET_ZUI_RUNNING,
    RET_ZUI_EXIT,
}ZUI_RET;

//////////////////////////////////////////////////////////////////
//  Configuration
//
#define ZUI_DBG_ERR 1 // 0//1
#define ZUI_DBG_RECURSIVE 0


#define CPU_BEON    0x0000
#define CPU_8051    0x0001
#define CPU_AEON    0x0002

// CPU type
#define CPU_TYPE    CPU_AEON//CPU_8051

#if (CPU_TYPE == CPU_8051)
    #define _ZUI_TBLSEG     code
    #define _MP_TBLSEG      code
#else
    #define _ZUI_TBLSEG
    #define _MP_TBLSEG
#endif // (CPU_TYPE == CPU_8051)

#if ZUI_DBG_ERR
    #define ABORT()         { while(1); }//GEN_EXCEP
    #define ZUI_DBG_FAIL(x) MS_DEBUG_MSG(x)
#else // ZUI_DBG_ERR
    #define ABORT()
    #define ZUI_DBG_FAIL(x) //x
#endif  // ZUI_DBG_ERR
    #define ZUI_MSG(x)      //x


#ifndef ENABLE_MAINPAGE_EFFECT
    #define ENABLE_MAINPAGE_EFFECT      ENABLE
#endif

#ifndef UNUSED //to avoid compile warnings...
    #define UNUSED(var) (void)((var) = (var))
#endif

#define _ZUI_MALLOC(x)      msAPI_Memory_Allocate((x),    BUF_ID_ZUI)
#define _ZUI_FREE(x)        MSAPI_MEMORY_FREE((x),BUF_ID_ZUI)

#define ZUI_ENABLE_ALPHATABLE               1
#define ZUI_ENABLE_REDUCED_ALPHATABLE       1

////////////////////////////////////////////////////////////////


INTERFACE void MApp_ZUI_ProcessKey(U8 u8IRKey);
INTERFACE ZUI_RET MApp_ZUI_MainTask(void);
INTERFACE BOOLEAN MApp_ZUI_CheckNeedDrawing(void);
INTERFACE BOOLEAN MApp_ZUI_Init(void);
INTERFACE ZUI_STATE MApp_ZUI_GetState(void);
INTERFACE U32 MApp_ZUI_GetActiveOSD(void);


INTERFACE BOOLEAN MApp_ZUI_ACT_StartupOSD(U32 id);
INTERFACE void MApp_ZUI_ACT_ShutdownOSD(void);
INTERFACE BOOLEAN MApp_ZUI_ACT_ExecuteWndAction(U16 act);

INTERFACE void MApp_ZUI_MainLoop(void);
INTERFACE void MApp_Event_Handler(void);
#undef INTERFACE

#endif  // _APP_ZUI_H

