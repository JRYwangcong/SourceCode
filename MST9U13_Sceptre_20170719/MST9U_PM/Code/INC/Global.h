#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "Board.h"

///////////////////////////////////////////////////////
#include <math.h>
#include <intrins.h>
#include "types.h"
#include "GPIO_DEF.h"
#include "debug.h"
#include "Common.h"
#include "ms_rwreg.h"
#include "ms_reg.h"
#include "Reg52.h"
#include "inputport.h"

#include "misc.h"
#include "halRwreg.h"
#include "Mcu.h"
#include "DefaultCompilerOption.h"
#include "drvPM.h"


//////////////////////////////////////////////////////////////////////////
/// XDATA MAP MUST Sync with R2 - 51 KeilC project setting about xdata start/size////
#define XDATA_SIZE              0x2000
#define XDATA_ADDR_START        0x4000 // keep 0x4000.

#define XDATA_MHL_CBUS_START0   0x4000 // 0x4000 ~ 0x40FF
#define XDATA_MHL_CBUS_START1   XDATA_MHL_CBUS_START0 + 256	//0x4100 ~ 0x41FF
#define XDATA_MHL_CBUS_START2   XDATA_MHL_CBUS_START1 + 256	//0x4200 ~ 0x42FF
#define XDATA_MHL_CBUS_START3   XDATA_MHL_CBUS_START2 + 256	//0x4300 ~ 0x43FF
#define XDATA_MHL_CBUS_START4   XDATA_MHL_CBUS_START3 + 256	//0x4400 ~ 0x44FF
#define XDATA_MHL_CBUS_START5   XDATA_MHL_CBUS_START4 + 256	//0x4500 ~ 0x45FF

#define XDATA_SWDB_ADDR_START   XDATA_MHL_CBUS_START5 + 0x100 // SWDB - allocate maximum 256 Bytes >=  sizeof(StuSWDBCtrl)
#define XDATA_DDC_ADDR_START    XDATA_SWDB_ADDR_START + 0x100 // DDC max = 256B*5=0x500, 0X4C00-0x0500=0x4700, In current driver code, maximum 256 Bytes needed.
// ...
// reserved for IP that needs xdata
// ...
#define XDATA_MBX_XD2MIU_ADDR   0x4FFC // 4Bytes, xd2miu base address, won't be initialized in 51
#define XDATA_51_START          0x5000 // Setting in KeilC & startup.a51, start = 0x5000, size = 0x1000, for 51 variables
#define XDATA_ADDR_END          0x5FFF
/// XDATA MAP MUST Sync with R2 - 51 KeilC project setting about xdata start/size////
//////////////////////////////////////////////////////////////////////////
#define LINEAR_MAPPING_VALUE(orgNow,orgMin,orgMax,newMin,newMax) \
    ((((DWORD)((orgNow)-(orgMin)))*((newMax)-(newMin))+(((orgMax)-(orgMin))>>1))/((orgMax)-(orgMin))+(newMin))

extern BYTE xdata g_u8KeypadPressData;

#if ENABLE_WATCH_DOG_INT
extern BYTE xdata u8WDT_Status;
#endif

#if EnableTime1Interrupt
extern  WORD XDATA g_u16MsCounter1; // for System timer
extern  BOOL XDATA bSecondTimer1;
extern  BYTE idata u8SecondTIMER1;
#endif

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
#define HIBYTE(value)             ((BYTE)((value) >> 8))
#define LOBYTE(value)             ((BYTE)(value))


extern BYTE g_ucTimer0_TH0;
extern BYTE g_ucTimer0_TL0;
extern BYTE xdata g_u8SystemSpeedMode;
extern bit g_bMcuPMClock;
extern bit bBuzzer;
extern bit g_bCorePowerStatus;
extern bit g_bR2Status;
#if ENABLE_DRAM_SELFREFRESH
extern bit g_bSTRStatus;
#endif

#if (IR_REMOTE_SEL!=NO_REMOTE)
extern BYTE xdata IRkeyDelayCounter;
#endif

extern BYTE xdata VKeyPad;
extern WORD xdata SystemFlags; // for system status flags

extern bit SecondFlag;
extern bit ms10Flag;
extern bit ms50Flag;
extern bit ms500Flag; //
#define Set_SecondFlag()            (SecondFlag = 1)
#define Clr_SecondFlag()            (SecondFlag = 0)
#define Set_ms10Flag()          (ms10Flag = 1)
#define Clr_ms10Flag()          (ms10Flag = 0)
#define Set_ms50Flag()          (ms50Flag = 1)
#define Clr_ms50Flag()          (ms50Flag = 0)
#define Set_ms500Flag()         (ms500Flag = 1)
#define Clr_ms500Flag()         (ms500Flag = 0)

extern WORD ms_Counter; // for System timer
extern BYTE xdata Second;
extern WORD xdata g_u32MailCounter;
extern WORD xdata CheckPMcounter;

extern BYTE xdata KeypadButton;
extern BYTE xdata ModeDetectCounter;

extern WORD xdata PM_SleepModeCounter;

#ifndef DDC_BUFFER_LENGTH
#define DDC_BUFFER_LENGTH   50//MaxBufLen 50 // ddc buffer length
#endif
#define _UART_BUFFER_LENGTH_    DDC_BUFFER_LENGTH
extern BYTE xdata SIORxBuffer[_UART_BUFFER_LENGTH_];
extern BYTE xdata SIORxIndex;
extern BYTE ReSyncCounter;
extern bit TI0_FLAG;

extern BYTE xdata SIORxBuffer1[_UART_BUFFER_LENGTH_];
extern BYTE xdata SIORxIndex1;
extern BYTE ReSyncCounter1;
extern bit TI1_FLAG;


extern WORD DelayCounter;
extern bit bDelayFlag;
extern BYTE xdata TimeOutCounter;

extern BYTE XDATA MailBox[];
extern DWORD XDATA XDATA_TO_DRAM_ADDR;
#if ENABLE_MBX
extern bit bMailNT;
#endif

#if ENABLE_R2_INT_M51_Test
extern bit bR2_int_M51_flag;
extern BYTE u8nonPMFIQStatus;
extern BYTE u8HostS2D;
#endif


extern BYTE xdata g_ucPWMTimer;
extern BYTE TransNumToBit(BYTE value);

extern bit DebugOnlyFlag;
#define Set_DebugOnlyFlag()          (DebugOnlyFlag=1)
#define Clr_DebugOnlyFlag()             (DebugOnlyFlag=0)

extern void Init_GlobalVariables( void );

/// Suspend type
typedef enum
{
    E_MSOS_PRIORITY,            ///< Priority-order suspension
    E_MSOS_FIFO,                ///< FIFO-order suspension
} MsOSAttribute;

#define MSOS_PROCESS_SHARED     0x00000001
#define MSOS_PROCESS_PRIVATE    0x00000000

#define MsOS_CreateMutex(a, b, c)  1
#define MsOS_In_Interrupt()     FALSE
#define MsOS_ObtainMutex(a, b)  TRUE
#define MsOS_ReleaseMutex(a)
extern Bool MsOS_DeleteMutex(Slong s32MutexId);




#ifdef IR_MODE_SEL_2
#define bIR_Mode_SelBit         				BIT0
#define IR_Mode_Sel_Flag       			(SystemFlags&bIR_Mode_SelBit)
#define Set_IR_Mode_Sel_Flag()			(SystemFlags|=bIR_Mode_SelBit)
#define Clr_IR_Mode_Sel_Flag()			(SystemFlags&=~bIR_Mode_SelBit)
#endif

#ifdef IR_MODE_SEL_3
#define bIR_Mode_Sel3Bit         				BIT1
#define IR_Mode_Sel3_Flag       			(SystemFlags&bIR_Mode_Sel3Bit)
#define Set_IR_Mode_Sel3_Flag()			(SystemFlags|=bIR_Mode_Sel3Bit)
#define Clr_IR_Mode_Sel3_Flag()			(SystemFlags&=~bIR_Mode_Sel3Bit)
#endif

#ifdef IR_MODE_SEL_4
#define bIR_Mode_Sel4Bit         				BIT2
#define IR_Mode_Sel4_Flag       			(SystemFlags&bIR_Mode_Sel4Bit)
#define Set_IR_Mode_Sel4_Flag()			(SystemFlags|=bIR_Mode_Sel4Bit)
#define Clr_IR_Mode_Sel4_Flag()			(SystemFlags&=~bIR_Mode_Sel4Bit)
#endif

#ifdef IR_MODE_SEL_5
#define bIR_Mode_Sel5Bit         				BIT3
#define IR_Mode_Sel5_Flag       			(SystemFlags&bIR_Mode_Sel5Bit)
#define Set_IR_Mode_Sel5_Flag()			(SystemFlags|=bIR_Mode_Sel5Bit)
#define Clr_IR_Mode_Sel5_Flag()			(SystemFlags&=~bIR_Mode_Sel5Bit)
#endif

#define bSleepModeBit         				BIT4
#define SleepMode_Flag       			(SystemFlags&bSleepModeBit)
#define Set_SleepMode_Flag()			(SystemFlags|=bSleepModeBit)
#define Clr_SleepMode_Flag()			(SystemFlags&=~bSleepModeBit)


#endif
