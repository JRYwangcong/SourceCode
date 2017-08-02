/******************************************************************************
 Copyright (c) 2005 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: Ir.h
 [Date]:        04-Feb-2005
 [Comment]:
   Remote control header file.
 [Reversion History]:
*******************************************************************************/
#include "Board.h"

#ifndef _MSIR_H_
#define _MSIR_H_

#ifdef _MSIR_C_
    #define _MSIRDEC_
#else
    #define _MSIRDEC_ extern
#endif
#if(IR_REMOTE_SEL != NO_REMOTE)
//////////////////////////////////////

#define IR_DEBUG_EN     1

#define ISR_TIMER0_INTERVAL     1 // unit: 1ms

// IR Software mode
#if(IR_MODE_SEL == IR_SOFTWARE_MODE)
#define IR_HDCODE_MIN_TIME      900 // 9ms
#define IR_HDCODE_1ST_TIME     1350 // 9+4.5ms
#define IR_HDCODE_RP_TIME      1150 // 9+2.5ms

#define IR_LG0_MIN_TIME        560  // 0.56ms
#define IR_LG0_MAX_TIME        1120 // 1.12ms
#define IR_LG1_MIN_TIME        1120 // 1.12ms
#define IR_LG1_MAX_TIME        2240 // 2.24ms
#elif(IR_MODE_SEL == IR_FULL_INT_RC5_EXT_MODE || IR_MODE_SEL == IR_FULL_INT_RC5_MODE)
#define LONG_BIT_LENGTH 1788 //us
#define LONG_BIT_BOUNDARY (double)3*LONG_BIT_LENGTH/4
#define H_KEEP_TIME    56//444 //us
#define TIME_OUT_TIME 32000 //us
//TIME_OUT  1 count = 1/0.5k = 2ms
#define TIME_OUT_COUNT TIME_OUT_TIME/1000/2
#define WATCH_DOG_TIME 30000 //us
//WATCH DOG 1 count = 1/2k =0.5ms
#define WATCH_DOG_COUNT (double)WATCH_DOG_TIME*2/1000

#elif(IR_MODE_SEL == IR_FULL_INT_RC6_HEAD888MS_MODE || IR_MODE_SEL == IR_FULL_INT_RC6_HEAD444MS_MODE)
//#define LONG_BIT_LENGTH 1788 //us
//#define LONG_BIT_BOUNDARY (double)3*LONG_BIT_LENGTH/4
#define H_KEEP_TIME    28//222 //us
#define TIME_OUT_TIME 32000 //us
#define LONG_PULSE_CALIBRATION_COUNT 192
//TIME_OUT  1 count = 1/0.5k = 2ms
#define TIME_OUT_COUNT TIME_OUT_TIME/1000/2
#define WATCH_DOG_TIME 30000 //us
//WATCH DOG 1 count = 1/2k =0.5ms
#define WATCH_DOG_COUNT (double)WATCH_DOG_TIME*2/1000

#endif

//note!! clock gen
// 38_8c[5:4]
// 02=4M
// 00=1M
// 01=256K

#define IR_CKDIV_NUM           (11)//(0) // 0-255
#define SW_IR_CKDIV_NUM        (0xCC)//(0) // 0-255
#define irGetMinCnt(time)      (double)time*((double)1-IR_TOLERANCE_PER)
#define irGetMaxCnt(time)      (double)time*((double)1+IR_TOLERANCE_PER)
#define irGetCnt(time) (double)time/(IR_CKDIV_NUM+1)



// Calculate IR settings
#if(IR_MODE_SEL == IR_FULL_INT_NEC_MODE || IR_MODE_SEL == IR_FULL_POL_NEC_MODE || IR_MODE_SEL == IR_RAW_DATA_POL_MODE || IR_MODE_SEL == IR_RAW_DATA_INT_MODE)
#define IR_HEADER_CODE_TIME    9000 // us
#define IR_OFF_CODE_TIME       4500 // us
#define IR_OFF_CODE_RP_TIME    2500 // us
#define IR_LOGI_01H_TIME        560 // us
#define IR_LOGI_0_TIME         1120 // us
#define IR_LOGI_1_TIME         2240 // us

#define IR_TIMEOUT_CYC         140000 // us

#define IR_TOLERANCE_PER       0.25

#define IR_HDC_UPB             irGetMaxCnt(IR_HEADER_CODE_TIME)
#define IR_HDC_LOB             irGetMinCnt(IR_HEADER_CODE_TIME)
#define IR_OFC_UPB             irGetMaxCnt(IR_OFF_CODE_TIME)
#define IR_OFC_LOB             irGetMinCnt(IR_OFF_CODE_TIME)
#define IR_OFC_RP_UPB          irGetMaxCnt(IR_OFF_CODE_RP_TIME)
#define IR_OFC_RP_LOB          irGetMinCnt(IR_OFF_CODE_RP_TIME)
#define IR_LG01H_UPB           irGetMaxCnt(IR_LOGI_01H_TIME)
#define IR_LG01H_LOB           irGetMinCnt(IR_LOGI_01H_TIME)
#define IR_LG0_UPB             irGetMaxCnt(IR_LOGI_0_TIME)
#define IR_LG0_LOB             irGetMinCnt(IR_LOGI_0_TIME)
#define IR_LG1_UPB             irGetMaxCnt(IR_LOGI_1_TIME)
#define IR_LG1_LOB             irGetMinCnt(IR_LOGI_1_TIME)
#define IR_RP_TIMEOUT          irGetCnt(IR_TIMEOUT_CYC)

#endif



#if(IR_MODE_SEL==IR_RAW_DATA_POL_MODE ||IR_MODE_SEL==IR_RAW_DATA_INT_MODE)
#define IR_RAW_DATA_NUM        4

#elif(IR_MODE_SEL == IR_SOFTWARE_MODE)

#define IR_SW_DATA_NUM             32
#define IR_SW_INT_POS_TRIG         0x1F
#define IR_SW_INT_NEG_TRIG         0x2F
#define IR_SW_INT_POS_NEG_TRIG     0x3F
#define HEADER_UPPER_BOND          0x14D
#define HEADER_LOWER_BOND          0x10D
#define ZERO_LB                    0x35
#define ZERO_UB                    0x55
#define ONE_LB                     0x75
#define ONE_UB                     0x95

#endif

//    IR command delay time
#define IR_DELAY_TIME  (3000 / ISR_TIMER0_INTERVAL) // IR command start in INT1 to IR command end time
#define IR_DELAY_TIME0 (150 / ISR_TIMER0_INTERVAL)  // IR 1st command to repeat command delay time
#define IR_DELAY_TIME1 (50 / ISR_TIMER0_INTERVAL)  // IR command hold time after last repeat command
#define IR_REPEAT_START_TIME    (800 / ISR_TIMER0_INTERVAL)
#define IR_REPEAT_END_TIME      (550 / ISR_TIMER0_INTERVAL)

//////////////////////////////////////
//////////////////////////////////////
_MSIRDEC_ XDATA BYTE g_ucIrCode;
_MSIRDEC_ XDATA BOOL g_bIRRepeat;
_MSIRDEC_ bit g_bIrDetect;  // IR command detect flag

#if(IR_MODE_SEL==IR_FULL_INT_NEC_MODE || IR_MODE_SEL==IR_FULL_POL_NEC_MODE)
_MSIRDEC_ XDATA BYTE g_IRReceiveOneByte;
_MSIRDEC_ XDATA BYTE g_IRBufferEmpty;
_MSIRDEC_ XDATA BOOL g_bIRRepeatTimeOut;
_MSIRDEC_ XDATA WORD g_wIRRepeatTimer;
#elif(IR_MODE_SEL==IR_RAW_DATA_POL_MODE ||IR_MODE_SEL==IR_RAW_DATA_INT_MODE)
_MSIRDEC_ XDATA BYTE g_IRRawModeDone;
_MSIRDEC_ XDATA BYTE g_IRRawData[IR_RAW_DATA_NUM];
_MSIRDEC_ XDATA BYTE g_ucIRRawModeCount;
#elif(IR_MODE_SEL == IR_SOFTWARE_MODE)
_MSIRDEC_ XDATA BYTE g_ucKeyData;
_MSIRDEC_ XDATA BYTE g_ucIRSWModeCount;
_MSIRDEC_ XDATA BOOL g_bIRSWDecoding;
_MSIRDEC_ XDATA BOOL g_bIRSWDone;
_MSIRDEC_ XDATA DWORD g_wIRCounter[IR_SW_DATA_NUM+1];
_MSIRDEC_ XDATA BOOL g_bIRStart;
_MSIRDEC_ XDATA BYTE g_ucIrBitsCnt; // IR bits counter
_MSIRDEC_ XDATA BYTE g_ucIrDataFifo; // IR data FIFO
_MSIRDEC_ XDATA WORD g_wIRWholeData;
_MSIRDEC_ XDATA BOOL g_bIRComplete;
_MSIRDEC_ XDATA WORD g_wIRRepeatTimer;
_MSIRDEC_ XDATA BOOL g_bIRRepeatTimeOut;
 #if DEBUG_IR_SW_MODE
_MSIRDEC_ XDATA DWORD g_wIRTestCounter[35];
_MSIRDEC_ XDATA BYTE g_ucCounter;
#endif

#elif(IR_MODE_SEL == IR_FULL_INT_RC5_EXT_MODE || IR_MODE_SEL == IR_FULL_INT_RC5_MODE ||IR_MODE_SEL == IR_FULL_INT_RC6_HEAD888MS_MODE ||IR_MODE_SEL == IR_FULL_INT_RC6_HEAD444MS_MODE)
_MSIRDEC_ XDATA BOOL g_bIRRepeatTimeOut;
_MSIRDEC_ XDATA WORD g_wIRRepeatTimer;
#endif

// IR key
_MSIRDEC_ bit g_bIrNumKeyStart; // IR 0-9 key pressed start flag
_MSIRDEC_ bit g_bIrKeyNumEnd;   // IR 0-9 key pressed end flag

_MSIRDEC_ bit g_bIr100Key; // pressed +100 key

_MSIRDEC_ XDATA BYTE g_ucIrNumKey;      // IR number key
_MSIRDEC_ XDATA WORD g_wIrNumKeyTimer; // IR number key timer

//////////////////////////////////////
// Subroutines
//////////////////////////////////////
_MSIRDEC_ void irInitialize(void);
_MSIRDEC_ void irInterruptProcess(void);
_MSIRDEC_ void irDetectTimer0( void );
_MSIRDEC_ void irPollingHandler(void);

_MSIRDEC_ void irDecodeRepaeat(WORD wRepeatTime);
_MSIRDEC_ void irDecodeEnd(void);
#endif  //(IR_REMOTE_SEL != NO_REMOTE)
#endif
