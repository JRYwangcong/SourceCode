//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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

#ifndef _DEBUG_H
#define _DEBUG_H

#include "datatype.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

/*definition ====================================================================================*/
#define WINDOWS        1
#define CYGWIN        2
#define SIMULATOR    3
#define MCU8051        4
#define OPENRISC    5
#define LINUX        6
#define PLATFORM    MCU8051
/*===============================================================================================*/

#define SYSTEM_DEBUG    1// 0
#define SHOW_MEMORY_MAP 0    // print memory at system startup
#define STACK_GUARD     0    // put a sentinel to stack and report if it is changed
#define TIMING_DBG      0


#define IR_ENABLE_UART_INPUT            0
#define ENABLE_DLC_TEST_BY_MSTVTOOL     1

//-----------type "uu" to udpate software by USB--------------------------------
#define DEBUG_USE_UART_COMMAND            0

#define STATE_PRINT         1// 0

#ifdef ENABLE_MSTV_UART_DEBUG
#define DEBUG_INFO(y) y
#else
#define DEBUG_INFO(y)
#endif

#if STATE_PRINT
#define DEBUG_STATE(y) y
#else
#define DEBUG_STATE(y)
#endif

#define ERR_NONE                        0x0000
#define ERR_OUT_OF_MEMORY               0x0001
#define ERR_MEMORY_NOT_ALLOCATED        0x0002
#define ERR_BIN_ID_NOT_FOUND            0x0003

#define ERR_FONT_GENERIC_ERROR          0x0100
#define ERR_FONT_BIN_NOT_FOUND          0x0101
#define ERR_FONT_OUT_OF_BUFFER          0x0102
#define ERR_FONT_WRONG_FORMAT           0x0103

#define ERR_BMP_GENERIC_ERROR           0x0200
#define ERR_BMP_BIN_NOT_FOUND           0x0201
#define ERR_BMP_OUT_OF_BUFFER           0x0202

#define ERR_FB_ID_OUT_OF_RANGE          0x0300
#define ERR_FB_ID_NOT_ALLOCATED         0x0301
#define ERR_FB_ID_ALREADY_ALLOCATED     0x0302
#define ERR_FB_OUT_OF_MEMORY            0x0303
#define ERR_FB_OUT_OF_ENTRY             0x0304

#define ERR_MVD_GENERIC_ERROR           0x1000
#define ERR_MVD_BOOT_TIMEOUT            0x1001
#define ERR_MVD_MODE_UNSUPPORTED        0x1002

#define ERR_MAD_GENERIC_ERROR           0x1100

#define ERR_DEMUX_GENERIC_ERROR         0x1200

#define ERR_GWIN_GENERIC_ERROR          0x1300
#define ERR_GWIN_ID_OUT_OF_RANGE        0x1301
#define ERR_GWIN_ID_NOT_ALLOCATED       0x1302
#define ERR_GWIN_ID_ALREADY_ALLOCATED   0x1303
#define ERR_GWIN_ID_ALREADY_MAPPED      0x1304

#define ERR_GE_GENERIC_ERROR            0x1400
#define ERR_GE_OUT_OF_FONT_TABLE        0x1401
#define ERR_GE_OUT_OF_BITMAP_TABLE      0x1402

#define ERR_UIMENU_GENERIC_ERROR        0x8000

// You can add FUNCTION_RESULT's member below for any result of operation
// to describe why function could not finish operation successfully.
// [NOTICE] Do not compare return value of function with E_RESULT_FAILURE.
//          Because there are many kinds of failure.
//          For example,
//          ex1)    if( E_RESULT_FAILURE == msAPI_CM_AddProgram(...) )
//          Instead of using above, use like below.
//          ex2)    if( E_RESULT_SUCCESS != msAPI_CM_AddProgram(...) )
/// Function result enumerate
typedef enum
{
    E_RESULT_SUCCESS = 0,                 ///< Result Success
    E_RESULT_FAILURE,                     ///< Result Failure
    E_RESULT_INVALID_PARAMETER,         ///< Result Invalid Parameter
    E_RESULT_OUTOF_MEMORY,               ///< Result Out of memory
    E_RESULT_OUTOF_EEPROM,               ///< Result Out of EEPROM
    E_RESULT_COMMUNICATION_ERROR,       ///< Result Communication Error
    E_RESULT_SAVE_TO_EEPROM_FAIL,       ///< Result Save to EEPROM fail
    E_RESULT_NOTHING,                    ///< Result Nothing
    E_RESULT_ILLEGAL_ACCESS,            ///< Result Illegal access
    E_RESULT_UNSUPPORTED                ///< Result unsupported
} FUNCTION_RESULT;

#if 0
void msWarning(unsigned int ErrCode);
void msFatal(unsigned int ErrCode);
#else
#define msWarning(c)    do {} while (0)
#define msFatal(c)      do { printf("Error(%d) in %s(%d)\n", c, __FUNCTION__, __LINE__); } while (0)
#endif

extern char code g_str_assert[]; // defined in STARTUP.A51

#ifdef MSOS_TYPE_LINUX
#define __ASSERT(expr) (void)((expr) || printf("%s %d\n", __FILE__, __LINE__));
#else
#define __ASSERT(expr)  (void)((expr) || printf(g_str_assert, __FILE__, (int)__LINE__))
#endif
#undef ASSERT
#define ASSERT(expr)    __ASSERT(expr)

//#define MS_CRITICAL_MSG(x)      x

#if ENABLE_DBG
#define MS_DEBUG_MSG(x)         x
#else
#define MS_DEBUG_MSG(x)
#undef ASSERT
#undef __ASSERT
#define ASSERT(x)
#define __ASSERT(x)
#endif

#define HERE MS_DEBUG_MSG(printf("\n%s %s Line %d:\n", __FILE__, __FUNCTION__, __LINE__));
#define BOOT_TIME_DBG()    printf("[%s] %lu -- %lu\n",__FILE__,(U32)__LINE__, msAPI_Timer_GetTime0())

#define debugPrint(a,b)

extern U16 g_debug;


#define PRINT_CUR_FUNCTION()    printf("%s()\n", __FUNCTION__)

#define PRINT_HERE()            printf("%s %u\n", __FILE__, __LINE__)


#define CHECK_MIU_PROTECT() do { \
    { \
        printf("\n%s %u\n", __FILE__, __LINE__); \
        printf(" hit_protect_flag=%x\n", (MDrv_ReadByte(0x1012DE)&0x10)>>4 );\
        printf(" hit_protect_no=%d, hit_protect_id=%x\n", (MDrv_ReadByte(0x1012DE)&0xE0)>>5, (MDrv_ReadByte(0x1012DF)&0x3F) ); \
    } \
} while(0)

#define CHECK_MIU_PROTECT_AND_CLEAR() do { \
    if( MDrv_ReadByte(0x1012DE)&0x10 ) \
    { \
        printf("\n%s %u\n", __FILE__, __LINE__); \
        printf("==== Miu Protect: Invalid access happened!! ===\n");\
        printf(" hit_protect_no=%d, hit_protect_id=%x\n", (MDrv_ReadByte(0x1012DE)&0xE0)>>5, (MDrv_ReadByte(0x1012DF)&0x3F) ); \
        printf("===============================================\n");  \
        MDrv_WriteByteMask(0x1012DE, 0x01, 0x01); /* Clear hit bit */ \
        MDrv_WriteByteMask(0x1012DE, 0x00, 0x01); \
    } \
} while(0)


#if 1 // 20120420
#define CAL_TIME_FUNC_START()
#define CAL_TIME_FUNC_(str)
#define CAL_TIME_FUNC_END()
#else

extern BOOL  g_b_EnableCalTime;
extern U8    g_u8_CalTime_Deep;
extern void PrintCalTimeDeep(U8 u8Deep );


#define EN_CAL_TIME             (g_b_EnableCalTime)

#define CAL_TIME_FUNC_START()   U32 u32CalTime_1, u32CalTime_2; \
                                u32CalTime_2 = u32CalTime_1 = MsOS_GetSystemTime(); \
                                g_u8_CalTime_Deep += 1; \
                                if( EN_CAL_TIME ) \
                                { \
                                    PrintCalTimeDeep(g_u8_CalTime_Deep); \
                                    printf(" %s() at %lu\n", __FUNCTION__, u32CalTime_1); \
                                }

#define CAL_TIME_FUNC_(str)     do{ \
                                    if( EN_CAL_TIME ) \
                                    { \
                                        U32 u32CalTime3 = MsOS_GetSystemTime(); \
                                        PrintCalTimeDeep(g_u8_CalTime_Deep); \
                                        printf(" %s - %u, at %lu use %lu\n", str, __LINE__, u32CalTime3, u32CalTime3-u32CalTime_2 ); \
                                    } \
                                    u32CalTime_2 = MsOS_GetSystemTime(); \
                                } while(0)

#define CAL_TIME_FUNC_END()     do{ \
                                    if( EN_CAL_TIME ) \
                                    { \
                                        U32 u32CalTime3 = MsOS_GetSystemTime(); \
                                        PrintCalTimeDeep(g_u8_CalTime_Deep); \
                                        printf(" %s() end-at %lu use %lu\n", __FUNCTION__, u32CalTime3, u32CalTime3-u32CalTime_1); \
                                    } \
                                    g_u8_CalTime_Deep -= 1; \
                                } while(0)
#endif


#endif    /*_DEBUG_H END */

