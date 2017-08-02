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
///@file drvsys.h
///@brief System functions: Initialize, interrupt
///@author MStarSemi Inc.
///
///////////////////////////////////////////////////////////////////////////////

#define  _DRV_SYS_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Board.h"
#include "sysinfo.h"
#include "hwreg.h"
//#include "drvGPIO.h"
#include "drvUartDebug.h"
#include "MsTypes.h"
#include "MsDevice.h"
#include "drvMMIO.h"
#include "drvBDMA.h"
#include "drvMIU.h"
#include "drvWDT.h"
#include "SysInit.h"
#include "drvGlobal.h"
#include "drvISR.h"
//#include "drvIIC.h"
#include "debug.h"
#include "drvUART.h"
//#include "drvPM.h"
//#include "msIR.h"
//#include "Panel.h"
//#include "MApp_SaveData.h"
#include "util_symbol.h"
#include "drvCPU.h"
#include "imginfo.h"

#if defined(__aeon__)
#include "risc32_spr.h"
#endif

static U8 u8OrigWdtTimer = 0x0A;    //Default 10 sec

#ifdef MST9U_FPGA
U32 g_FPGA_CPU_CLOCK = 60000000; //36000000
#endif

/******************************************************************************/
/*                     Local Defines                                          */
/******************************************************************************/
#define PM_LOCK_SUPPORT             0// 1   // 1: Enable 0: Disable
#if (PM_LOCK_SUPPORT == 1)
#include "drvSEM.h"
#endif

// set 1 to enable support for download image to dram and run from MSTV_Tool
#define DOWNLOAD_RUN_SERVER     0
#define DBG_SYS_PNT(x)   //x

#define SYSINIT_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && SYSINIT_DEBUG
#define SYSINIT_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define SYSINIT_PRINT(format, ...)
#endif

#if ENABLE_COMPILE_INFO
MS_CMP_Info code g_cmpInfo __attribute__((__section__ (".cmp_img_info"))) =
{
	__DATE__,
	__TIME__
};
#endif

U32 MDrv_Sys_CoproRun(MS_U32 u32BootAddr);
U32 MDrv_Sys_SecuRun(MS_U32 u32BootAddr);
void MDrv_Sys_SetMIUProtect(U32 u32ReadOnlyStart, U32 u32ReadOnlyEnd, U32 u32PageShift);


/******************************************************************************/
/*                     Global Variables                                       */
/******************************************************************************/

// memory map for pre-built libraries
//U32 u32SecbufAddr = ((SECBUF_START_MEMORY_TYPE & MIU1) ? (SECBUF_START_ADR | MIU_INTERVAL) : (SECBUF_START_ADR));
//U32 u32SecbufSize = SECBUF_START_LEN;

U32 u32CRCbufAddr = NULL;

unsigned long RIU_MAP;

#define u8ChipIdMajor   REG8(REG_CHIP_ID_MAJOR)
#define u8ChipIdMinor   REG8(REG_CHIP_ID_MINOR)
#define u8ChipVer       REG8(REG_CHIP_VERSION)
#define u8ChipRev       REG8(REG_CHIP_REVISION)

/******************************************************************************/
void MDrv_Sys_InitUartForXtalClk( char cOutputChar )
{
    UNUSED(cOutputChar);
    MDrv_WriteRegBit(0x1E03, ENABLE, BIT2);          // enable UART RX
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_Sys_Get_BinInfo()
/// @brief \b Function \b Description : Get Specific Bin information
/// @param <IN>        \b None        :
/// @param <OUT>       \b pBinInfo    : Get Bin Information
/// @param <RET>       \b BOOLEAN     : Success or Fail
/// @param <GLOBAL>    \b None        :
////////////////////////////////////////////////////////////////////////////////
void MDrv_Sys_Get_BinInfo(BININFO *pBinInfo, BOOLEAN *pbResult)
{
    BININFO NewBinInfo;

    // Added by coverity_0505
    memset(&NewBinInfo, 0, sizeof(BININFO));

    NewBinInfo.B_ID = pBinInfo->B_ID;
    *pbResult = Get_BinInfo(&NewBinInfo);
    pBinInfo->B_FAddr = NewBinInfo.B_FAddr;
    pBinInfo->B_Len   = NewBinInfo.B_Len;
    pBinInfo->B_IsComp = NewBinInfo.B_IsComp;
}

/******************************************************************************/
/// DO NOT MODIFY THIS FUNCTION, IT IS VERY DANGEROUS! (called by vectors.S)
/******************************************************************************/

#define STACK_SIZE      131072

//---------------------------------------------------------
//---------------------------------------------------------
#if ENABLE_MSTV_UART_DEBUG
static void console_init(void)
{
#ifdef MST9U_FPGA
    MDrv_UART_Init(DEFAULT_UART_DEV, 38400);
    mdrv_uart_connect(E_UART_PORT_GPIOX03_04,DEFAULT_UART_DEV);
#else
    //before querying CPU_clock, need to set base address for driver
    MDrv_UART_Init(DEFAULT_UART_DEV, 115200);
    mdrv_uart_connect(DEFAULT_UART_PORT,DEFAULT_UART_DEV);
#endif

    SYSINIT_PRINT("\nDefault port(0x%x) connecting to 0x%x\n", DEFAULT_UART_PORT, mdrv_uart_get_connection(DEFAULT_UART_PORT));
    SYSINIT_PRINT("\n=== console_init ok (%s %s) ===\n\n", __TIME__, __DATE__);
}
#endif

void MDrv_Timer_Init(void) // Now support mode 1
{
    gTimerCount0 = 0;
    gSystemTimeCount = 0;
    gTimerDownCount0 = 0;
    gTimerDownCount1 = 0;
    gu8100msTH = 0;
    g_u16ElapsedTimeSecond = 0;
    g_u8TimeInfo_Flag = 0;
}

void start(void)
{
    extern U32 __heap;
    extern U32 __heap_end;
    extern U8  _readonly_start[];
    extern U8  _readonly_end[];

    extern int  main(void);
    //extern void MDrv_Pad_Init(void);
    extern void MDrv_ISR_Init(void);
    extern void mhal_stack_init(void *stack);
    extern void MDrv_Timer_ISR_Register(void);

    U32 pageshift;
    U32 u32Start;
    U32 u32End;
    U32 u32Tmp;
    char *stack;

    // Setup stack
    stack = (char *)malloc(STACK_SIZE);
    if (stack)
    {
        stack += STACK_SIZE;
        mhal_stack_init(stack);
    }
    else
    {
        ASSERT(0);
    }
#if ENABLE_WATCH_DOG
#if (!BLOADER)
    MDrv_WDT_Init(E_WDT_DBGLV_NONE);
#endif
#endif
    MDrv_MMIO_GetBASE((MS_U32 *) &RIU_MAP, &u32Tmp, MS_MODULE_PM);

#if (PM_LOCK_SUPPORT == 1)
    MDrv_SEM_Init();
#endif

    MDrv_MIU_SetIOMapBase();
    MDrv_MIU_ProtectAlign(&pageshift);
    u32Start = MsOS_VA2PA((U32)_readonly_start);
    u32End = MsOS_VA2PA((U32)_readonly_end);

    // Set MIU Protect: Read-only & Scaler-IPW
    MDrv_Sys_SetMIUProtect(u32Start, u32End, pageshift);

    Util_InitSymbolTBL();

//MDrv_Pad_Init();
//mdrv_gpio_init();

    MDrv_COPRO_GetBase();

#if ENABLE_MSTV_UART_DEBUG
    console_init();
#endif

    SYSINIT_PRINT("Hello MST9U: (RIU_MAP=%lx), (STACK:%lx)\n",RIU_MAP, ((DWORD)stack));

    //printf("stack_top=0x%x, stack_size=0x%x\n", stack, STACK_SIZE);
    //printf("read only: u32Start=0x%x, u32End=0x%x\n", u32Start, u32End);
    //printf("heap => %x, => %x \n",&__heap, &__heap_end);
    if ((&__heap_end) <= (&__heap))
    {
        SYSINIT_PRINT("## ERROR!! MMAP lyout for CPU code is not enough!!!\n");
    }
    else if (((U32)&__heap_end - (U32)&__heap) < 0x4B000)
    {
        U32 u32FreeSize;
        u32FreeSize = (U32)&__heap_end - (U32)&__heap;
        SYSINIT_PRINT("## heap size is %dK under 300K!!\n", (u32FreeSize>>10));
    }

    if (((U32)_readonly_start & 0xFFF) || ((U32)_readonly_end & 0xFFF))
    {
        SYSINIT_PRINT("\r\n## ERROR!! ReadOnly area is not alignment 4K\n");
    }

    //MDrv_IIC_Init();

    //if ( ENABLE_POWER_GOOD_DETECT )
    //{
    //    MDrv_WriteRegBit(0x1EE4, ENABLE, 0x40);
    //    MDrv_WriteRegBit(0x1EE4, ENABLE, 0x1F);
    //}

    if (DOWNLOAD_RUN_SERVER)
    {
        //mhal_interrupt_disable();
        MsOS_CPU_DisableInterrupt();
        putchar( 'F' );
        putchar( 'W' );
        while (1)
        {
            MDrv_Sys_ClearWatchDog();
        }
    }
    else
    {
        if (ENABLE_WATCH_DOG == ENABLE)
        {
            MDrv_Sys_ClearWatchDog();
        }
        else
        {
            MDrv_Sys_DisableWatchDog();
        }
    }


    {
        extern void ProcessSysTrap( MHAL_SavedRegisters *pHalReg, U32 vector );
        MsOS_CPU_AttachException( E_EXCEPTION_TRAP, ProcessSysTrap, E_EXCEPTION_TRAP );

        MDrv_BDMA_Init(MIU_INTERVAL);
        MDrv_MMIO_Init(); // I/O remap
        MsOS_Init();
        MDrv_ISR_Init();
    }

    MDrv_Timer_ISR_Register();
    MDrv_Timer_Init();
    MsOS_CPU_EnableInterrupt();

#ifdef __aeon__
    MsOS_CPU_UnMaskInterrupt(E_INTERRUPT_FIQ); //unmask FIQ
    MsOS_CPU_UnMaskInterrupt(E_INTERRUPT_IRQ); //unmask IRQ
#endif


#if (ENABLE_CP_R2)
    {
        U32 u32CP_BIN_Addr = CP_R2_MEM_ADR;
        MDrv_Sys_CoproRun(u32CP_BIN_Addr);
    }
#endif

#if (ENABLE_SECU_R2)
	{
		U32 u32CP_BIN_Addr = SECU_R2_MEM_ADR;

        #if (CHIP_ID == MST9U3)
            MDrv_WriteByteMask(0x120F49, 0x00, 0x01);//reg_ckg_aesdma
        #endif
        
        MDrv_Sys_SecuRun(u32CP_BIN_Addr);
    }
#endif

    // jump to main() should never return
    main();

    while(1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_Sys_Set_CIDInfo()
/// @brief \b Function \b Description : To Set Customer Info
/// @param <IN>        \b pCIDInfo    : Input the CID Info
/// @param <OUT>       \b None        :
/// @param <GLOBAL>    \b None        :
////////////////////////////////////////////////////////////////////////////////
void MDrv_Sys_Set_CIDInfo(U8 *pCIDInfo)
{
    MDrv_WriteByte(0x1033D0,pCIDInfo[0]);
    MDrv_WriteByte(0x1033D1,pCIDInfo[1]);
    MDrv_WriteByte(0x1033D2,pCIDInfo[2]);
    MDrv_WriteByte(0x1033D3,pCIDInfo[3]);

}

/******************************************************************************/
/// Copy variable from DRAM space to XDATA space
/// @param pAddr \b IN XDATA pointer
/// @param u32MiuAddr \b IN MIU address
/// @param u16Length \b IN size to copy
/******************************************************************************/

void MDrv_Sys_CopyMIU2VAR(U32 srcaddr, void *dstaddr, U16 len)
{
    memcpy(dstaddr, (U8 *)srcaddr, len);
}

// for backward compatible, do not use!
#undef MDrv_MIU_SDRAM2VARCopy
void MDrv_MIU_SDRAM2VARCopy(U32 srcaddr, void *dstaddr, U16 len)
{
    memcpy(dstaddr, (U8 *)srcaddr, len);
}

/******************************************************************************/
/// Copy variable from XDATA space to DRAM space
/// @param pAddr \b IN XDATA pointer
/// @param u32MiuAddr \b IN MIU address
/// @param u16Length \b IN size to copy
/******************************************************************************/

void MDrv_Sys_CopyVAR2MIU(void *srcaddr, U32 dstaddr, U16 len)
{
    memcpy((U8 *)dstaddr, srcaddr, len);
}

// for backward compatible, do not use!
#undef MDrv_MIU_VAR2SDRAMCopy
void MDrv_MIU_VAR2SDRAMCopy(void *srcaddr, U32 dstaddr, U16 len)
{
    memcpy((U8 *)dstaddr, srcaddr, len);
}

/******************************************************************************/
//
//   128T +------+          +------+ 128T
//     |  |      |          |      |  |
//     +--+ 8051 |          | Aeon +--+
//        |      |          |      |
//        +--+---+          +---+--+
//       +---+----+        +----+---+
//       |        |        |        |
//   boot_aeon   sw_rst   sw_rst  boot_8051
//
// When boot from 8051
//      reboot:   use reg_reset_cpu0 with password
//      run aeon: set reg_sw_reset_cpu1 to 1
// When boot from Aeon
//      reboot:   use reg_reset_cpu1 with password
//      run 8051: set reg_sw_reset_cpu0 to 1
/******************************************************************************/
void MDrv_Sys_WholeChipReset( void )
{
  #if 1
    MDrv_WriteByte( RIUBASE_PM_MISC+0x5C, 0xFF );
    MDrv_Write4Byte(0xEA0, 0x51685168);
    MDrv_WriteRegBit(RIUBASE_PM_MISC+0x52, 1, BIT7);
    MDrv_WriteRegBit(RIUBASE_PM_MISC+0x52, 1, BIT6);
    MDrv_WriteByte( RIUBASE_PM_MISC+0x5C, 0x79 );
  #else
    MDrv_WDT_SetTimer(0, 1);
  #endif

    while(1);
}

/******************************************************************************/
///Reset 8051 to reboot
/******************************************************************************/
void MDrv_Sys_Reboot( void )
{
  #if 1
    MDrv_Write2Byte( REG_RESET_CPU_8051, 0x029F );
    MDrv_Write2Byte( REG_RESET_CPU_8051, 0x829F ); // can't ignore second 0x9F
  #else
    MDrv_WDT_SetTimer(0, 1);
  #endif
    while(1);
}


void MDrv_Sys_SetWatchDogTimer( U8 sec )
{
     if( sec == 0 )
     {
        u8OrigWdtTimer = 1;
     }
     else
     {
        u8OrigWdtTimer = sec;
     }
}

void  MDrv_Sys_EnableWatchDog(void)
{
#if (ENABLE_WATCH_DOG == ENABLE )
    MDrv_Write4Byte(WDT_TIMER_0, ( MDrv_Sys_WatchDogCycles( u8OrigWdtTimer ) ));
#endif
}

//----------------------------------------------------------------
void MDrv_Sys_DisableWatchDog( void )
{
    MDrv_Write4Byte( WDT_TIMER_0, 0x0000UL );
}

U32 MDrv_Sys_CoproRun(MS_U32 u32BootAddr)
{
    U32 src_addr, bin_len;
    BININFO cpr2_binfo;

    cpr2_binfo.B_ID = BIN_ID_CODE_CP_R2;
    if (!Get_BinInfo(&cpr2_binfo))
    {
        SYSINIT_PRINT("Get CP_R2 BINFO (B_ID=0x%x) fail..\n", cpr2_binfo.B_ID);
        return FALSE;
    }

    src_addr = cpr2_binfo.B_FAddr;
    bin_len  = cpr2_binfo.B_Len;
    //copy flash data to dram for cp_r2
    if (!MDrv_BDMA_FlashCopy2Dram( src_addr, u32BootAddr, bin_len ))
    {
        return FALSE;
    }

    if (!MDrv_COPRO_Init_Front())
    {
        return FALSE;
    }

    SYSINIT_PRINT("******************************************\r\n");
    SYSINIT_PRINT("hk_r2 is going to release cp_r2..\r\n");
    SYSINIT_PRINT("******************************************\r\n");
    MsOS_DelayTask(500);
    MDrv_COPRO_Init_End(u32BootAddr);

    return TRUE;
}

U32 MDrv_Sys_SecuRun(MS_U32 u32BootAddr)
{
    U32 src_addr, bin_len;
    BININFO r2_binfo;

    //Load SECU_DRAM code to DRAM
    r2_binfo.B_ID = BIN_ID_CODE_SECU_R2;
    if (!Get_BinInfo(&r2_binfo))
    {
        SYSINIT_PRINT("Get SECU_ROM_R2 BINFO (B_ID=0x%x) fail..\n", r2_binfo.B_ID);
        return FALSE;
    }

    src_addr = r2_binfo.B_FAddr;
    bin_len  = r2_binfo.B_Len + 256; //wilson: 256 for signature size;

	#if (ENABLE_SECU_R2 == 1) && (ENABLE_HDCP22 == 1)
	//fill necessary information for Secu ROM booting
	{
		#include "drvHDCPMbx.h"
		#include "drvHDCPRx.h"

		WORD i = 0;
		ULONG MBXBufAddr = 0;

		//clear mbx
		for ( i = 0; i < 128; i++ )
			MDrv_WriteByte(HDCPMBX_START_ADDR + i, 0x00);

		//fill header
		MDrv_WriteByte(HDCPMBX_START_ADDR, 'M');
		MDrv_WriteByte(HDCPMBX_START_ADDR + 1, 'S');
		MDrv_WriteByte(HDCPMBX_START_ADDR + 2, 'H');
		MDrv_WriteByte(HDCPMBX_START_ADDR + 3, 'D');

        //fill address & size
        MBXBufAddr = drv_HDCPRx_GetTxBufAddr();

        MDrv_Write4Byte(HDCPMBX_START_ADDR + 4, u32BootAddr >> 12);
        MDrv_Write4Byte(HDCPMBX_START_ADDR + 8, 0x10000);
        MDrv_Write4Byte(HDCPMBX_START_ADDR + 12, u32BootAddr >> 12); //sec dram start
        MDrv_Write4Byte(HDCPMBX_START_ADDR + 16, SECU_R2_MEM_LEN); //sec dram size
        MDrv_Write4Byte(HDCPMBX_START_ADDR + 20, MBXBufAddr); //mbx start
        MDrv_Write4Byte(HDCPMBX_START_ADDR + 24, 48); //mbx size

		i = 0;
		while ((MIU_DRAM_LEN >> i) > 0)
		{
			i++;
		}
		i -= 21;

        MDrv_WriteByte(0x101440, 0x10 + i); //setting and lock DRAM size
	}
	#endif

    //copy flash data to dram for cp_r2
    if (!MDrv_BDMA_FlashCopy2Dram( src_addr, u32BootAddr, bin_len ))
    {
        return FALSE;
    }

    SYSINIT_PRINT("******************************************\r\n");
    SYSINIT_PRINT("hk_r2 is going to release Secu_r2..\r\n");
    SYSINIT_PRINT("******************************************\r\n");
    MsOS_DelayTask(100);
	#if (ENABLE_SECU_R2_DBG == 1)
    mdrv_uart_connect(DEFAULT_UART_PORT,E_UART_AEON_PQ_R2);
	#endif
    MsOS_EnableInterrupt(E_INT_FIQ_HST3TOHST1_INT);
    RIU[0x100E80] = 0x2F;   //enable Secu_R2

    return TRUE;
}

void MDrv_Sys_SetMIUProtect(U32 u32ReadOnlyStart, U32 u32ReadOnlyEnd, U32 u32PageShift)
{
    U8 u8ProtectID[16]; //(Current MIU driver:[0~3]:G0, [4~7]:G1, [8~11]:G2, [12~15]:G3)

    // Read-Only Protect
    u8ProtectID[0]=u8ProtectID[1]=u8ProtectID[2]=u8ProtectID[3]=MIU_CLIENT_NONE;
    MDrv_MIU_Protect(0, 0, u8ProtectID, ALIGN(u32ReadOnlyStart + ((1<<u32PageShift) - 1), u32PageShift), ALIGN(u32ReadOnlyEnd, u32PageShift),DISABLE, ENABLE);

    // Scaler-IPW-Write Limit
#if (CHIP_ID == MST9U || CHIP_ID == MST9U2) // 4 SC
    u8ProtectID[4]=MIU_CLIENT_SC0_IPW_W;
    u8ProtectID[5]=MIU_CLIENT_SC1_IPW_W;
    u8ProtectID[6]=MIU_CLIENT_SC2_IPW_W;
    u8ProtectID[7]=MIU_CLIENT_SC3_IPW_W;
#elif (CHIP_ID == MST9U3) // 2 SC
    u8ProtectID[4]=MIU_CLIENT_SC0_IPW_W;
    u8ProtectID[5]=MIU_CLIENT_SC1_IPW_W;
    u8ProtectID[6]=u8ProtectID[7]=MIU_CLIENT_NONE;
#elif (CHIP_ID == MST9U4) // ?? SC
    #warning "TBD - MST9U4 MIU Protect!"
#else
    #error Unknow CHIP ID
#endif

#if ((MEMORY_MAP == MMAP_128_128MB) || (MEMORY_MAP == MMAP_256_256MB) || (MEMORY_MAP == MMAP_512_512MB)) // 2 MIU
    MDrv_MIU_Protect(0, 1, u8ProtectID, 0x00000000, ALIGN(FB_MIU0_ADR, u32PageShift),ENABLE, ENABLE);
    MDrv_MIU_Protect(1, 1, u8ProtectID, 0x00000000, ALIGN(FB_MIU1_ADR, u32PageShift),ENABLE, ENABLE);
#else // 1 MIU
    MDrv_MIU_Protect(0, 1, u8ProtectID, 0x00000000, ALIGN(FB_L0_ADR, u32PageShift),ENABLE, ENABLE);
#endif
}

