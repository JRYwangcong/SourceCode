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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define  _MDRV_PQ_C_

#ifdef MSOS_TYPE_LINUX
#include <pthread.h>
#endif
#include <string.h>



#include "Global.h"


#include "color_reg.h"


#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"
#include "drvPQ_Bin.h"
//#include "apiPNL.h"

#include "QualityMode.h"
#include "mhal_pq_adaptive.h"

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif


#define PQTAB_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_PQTAB) x; } while(0);
#define SRULE_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_SRULE) x; } while(0);
#define CSCRULE_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_CSCRULE) x; } while(0);
#define PQGRULE_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_GRULE) x; } while(0);
#define PQBW_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_BW) x; } while(0);
#define PQMADi_DBG(x)   do { if( _u16PQDbgSwitch & PQ_DBG_MADI) x; } while(0);
#define PQINFO_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_INFO) x; } while(0);
#define PQIOCTL_DBG(x)  do { if( _u16PQDbgSwitch & PQ_DBG_IOCTL) x; } while(0);
#define PQP2P_DBG(x)  do { if( _u16PQDbgSwitch & PQ_DBG_P2P) x; } while(0);

#define DRVPQ_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && DRVPQ_DEBUG
#define DRVPQ_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#define PQMODE_DBG(x)   x
#else
#define DRVPQ_PRINT(format, ...)
#define PQMODE_DBG(x)
#endif

//================================================================================

#define MApi_XC_R2BYTE(u32Reg)	\
    msRead2Byte(0x80000000 + (((MS_U16)u32Reg & 0xFF00)<<16) + u32ScBaseAddr + ((MS_U8)u32Reg & 0xFF))

#define MApi_XC_W2BYTEMSK(u32Reg, u16Val, u16mask)  \
    msWrite2ByteMask(0x80000000 + (((MS_U16)u32Reg & 0xFF00)<<16) + u32ScBaseAddr + (u32Reg & 0xFF), u16Val, u16mask)

#define MApi_XC_W2BYTE(u32Reg, u16Val)  \
    msWrite2Byte(0x80000000 + (((MS_U16)u32Reg & 0xFF00)<<16) + u32ScBaseAddr + (u32Reg & 0xFF), u16Val)

#define MApi_XC_WriteByteMask(u32Reg, u8Val, u8Msk)  \
    msWriteByteMask(0x80000000 + (((MS_U16)u32Reg & 0xFF00)<<16) + u32ScBaseAddr + (u32Reg & 0xFF), u8Val, u8Msk)

#define MApi_XC_ReadByte(u32Reg) \
    msReadByte(0x80000000 + (((MS_U16)u32Reg & 0xFF00)<<16) + u32ScBaseAddr + (u32Reg & 0xFF))

#define MApi_XC_ReadByteMask(u32Reg, u8mask) \
    (msReadByte(0x80000000 + (((MS_U16)u32Reg & 0xFF00)<<16) + u32ScBaseAddr + (u32Reg & 0xFF)) & (u8mask))

#define MApi_XC_R2BYTEMSK(u32Reg, u16mask) \
    (msRead2Byte(0x80000000 + (((MS_U16)u32Reg & 0xFF00)<<16) + u32ScBaseAddr + (u32Reg & 0xFF)) & (u16mask))

//====================================================================================

static MSIF_Version _drv_pq_version = {
    .DDI = { PQ_DRV_VERSION },
};

static MS_U16 _u16PQSrcType[PQ_MAX_WINDOW];
MS_U16 _u16PQSrcType_DBK_Detect[PQ_MAX_WINDOW];     //For Auto_DBK SW driver used
MS_BOOL g_bSkipCommTable = FALSE;
MS_BOOL g_bReadCurFrameMode = FALSE;
MS_BOOL g_bPqInit = FALSE;

static PQ_INPUT_SOURCE_TYPE _enInputSourceType[PQ_MAX_WINDOW];
static MS_BOOL _bColorRange0_255[PQ_MAX_WINDOW];
//static MS_BOOL _bGameModeOn[PQ_MAX_WINDOW];
//static MS_U8 _u8RWmappingNum[PQ_MAX_WINDOW];
//static MS_BOOL _bRWNumIsReset[PQ_MAX_WINDOW];
static MS_U16 _u16SRAM1 = PQ_BIN_IP_NULL;
static MS_U16 _u16SRAM2 = PQ_BIN_IP_NULL;

static MS_BOOL _bCustomerMainPQEn = FALSE ;
static MS_BOOL _bCustomerSubPQEn = FALSE ;

//whether current status is Point-to-point mode
static MS_BOOL _bPointToPointMode = FALSE;

//whether current status is Dualview display mode
static MS_BOOL _bDualViewEnable = FALSE;

static MS_U16 _u16PQDbgSwitch = FALSE;
static MS_BOOL _bDS_En = FALSE;
static MS_BOOL _bDMSV12L_PQConfigEn = FALSE; //this flag is coming from PQ file
#if PQ_EN_DMS_SW_CTRL
static MS_BOOL _bDMSV12L_PIPEn = TRUE; //TRUE: it's not PIP/POP; FALSE: PIP/POP case want to disable DMS12L
#endif
static MS_U16 _u16RW_Method = 0;

static MS_U16 _u16PQDelyaTime = 0;
static MS_U16 _u16PQSuggestedFrameNum[PQ_MAX_WINDOW];

static BYTE _u8PQ_Backup_BypassTbl_Main[255];
static BYTE _u8PQ_Backup_BypassTbl_Sub[255];


typedef struct
{
    // input timing
    MS_U16 u16input_hsize;
    MS_U16 u16input_vtotal;
    MS_U16 u16input_vfreq;

    // output timing
    MS_U16 u16output_hsize;
    MS_U16 u16output_vtotal;
    MS_U16 u16output_vfreq;

    // memory format
    MS_BOOL bFBL;
    MS_BOOL bMemFmt422;
    MS_BOOL bInterlace;
    MS_BOOL b4RMode;
    MS_U8 u8BitsPerPixel;

    // osd
    MS_BOOL bOSD_On;
    MS_U16 u16OSD_hsize;
    MS_U8 u8OSD_BitsPerPixel;

    // dram
    MS_U32 u32MemBW;
    MS_BOOL bSC_MIUSel;
    MS_BOOL bGOP_MIUSel;
    MS_BOOL bDDR2;
    MS_U32  u32DDRFreq;
    MS_U8   u8BusWidth;
    MS_U32 u32Miu0MemSize;
    MS_U32 u32Miu1MemSize;

    // PVR
    MS_U32 u32PVR_BW;

    //AEON
    MS_U32 u32MISC_BW;
} BW_INFO_t;

#if (PQ_ENABLE_PIP)
#if (PQ_ONLY_SUPPORT_BIN == 0)
static PQ_DISPLAY_TYPE genDisplayType = PQ_DISPLAY_ONE;
#endif
#endif

static MS_PQ_INFO   _info = {.u16Input_Src_Num = 128, .u8IP_Num = 217,};
static MS_PQ_Status _status = {.bIsInitialized = FALSE, .bIsRunning = FALSE, };
static MS_PQ_RFBL_INFO _stRFBL_Info = {.bEnable = FALSE, .bFilm = FALSE , .u8MADiType = 0, };
//static BW_INFO_t bw_info;


#if(ENABLE_PQ_BIN)
static MS_BOOL gbPQBinEnable = 0;
MS_PQBin_Header_Info stPQBinHeaderInfo[MAX_PQ_BIN_NUM];
//extern MS_PQTextBin_Header_Info stPQTextBinHeaderInfo[MAX_PQ_TEXT_BIN_NUM];
//extern MS_BOOL gbEnablePQTextBin;
#endif

MS_S32 _PQ_Mutex = -1;
DWORD u32ScBaseAddr = 0x102F00;

#ifdef MSOS_TYPE_LINUX
pthread_mutex_t _PQ_MLoad_Mutex;
#endif

#ifndef ENABLE_VGA_EIA_TIMING
#define ENABLE_VGA_EIA_TIMING             1
#endif

#ifndef PQ_QM_NETMM_VIDEO
#define PQ_QM_NETMM_VIDEO                 0
#endif

#ifndef PQ_ENABLE_3D_STRENGTHEN_NR //to avoid compile warnings...
#define PQ_ENABLE_3D_STRENGTHEN_NR        0
#endif

#ifndef PQ_QM_24HZMM_VIDEO
#define PQ_QM_24HZMM_VIDEO                0
#endif

#ifndef PQ_QM_HDMI_FP_VIDEO
#define PQ_QM_HDMI_FP_VIDEO                0
#endif

#define GateFor30P 450

#define PQ_BIN_CUSTOMER_PANEL_INDEX        0

MS_PQ_Hdmi_Info _stHDMI_Info[PQ_MAX_WINDOW];
MS_PQ_Vd_Info _stVD_Info[PQ_MAX_WINDOW];
MS_PQ_Mode_Info  _stMode_Info[PQ_MAX_WINDOW];
MS_U8 _u8ModeIndex[PQ_MAX_WINDOW];
MS_PQ_MuliMedia_Info _stMultiMedia_Info[PQ_MAX_WINDOW];
MS_PQ_Dtv_Info _stDTV_Info[PQ_MAX_WINDOW];
MS_PQ_Init_Info _stPQ_Info;
MS_BOOL _gbMemfmt422[PQ_MAX_WINDOW]={1, 1};
MS_BOOL _gIsSrcHDMode[PQ_MAX_WINDOW];
PQ_FOURCE_COLOR_FMT _gFourceColorFmt[PQ_MAX_WINDOW];
MS_BOOL _gIs3D_En = 0;
MS_BOOL _gIsH264 = 0;
MS_BOOL _gIsMMplayer=0;
MS_BOOL _bBypassModeOn[PQ_MAX_WINDOW]={0, 0};
MS_BOOL _gIsRmvb[PQ_MAX_WINDOW];
MS_BOOL _gIsNetworkMM = 0;

//////////////////////////////////////////////////////////////////
// PQ Patch
MS_BOOL _bOSD_On = FALSE;

//static MS_BOOL bSetFrameCount = TRUE;

#if 0 //def _PDEBUG
#define PTH_PQ_RET_CHK(_pf_) \
    ({ \
        int r = _pf_; \
        if (r != 0 && r != ETIMEDOUT) \
            fprintf(stderr, "[PTHREAD] %s: %d: %s: %s\n", __FILE__, __LINE__, #_pf_, strerror(r)); \
        r; \
    })
#else
#define PTH_PQ_RET_CHK(_pf_) //_pf_
#endif


#ifdef MSOS_TYPE_LINUX
#define PQ_MLOAD_ENTRY() PTH_PQ_RET_CHK(pthread_mutex_lock(&_PQ_MLoad_Mutex))
#define PQ_MLOAD_RETURN() PTH_PQ_RET_CHK(pthread_mutex_unlock(&_PQ_MLoad_Mutex))
#else
#define PQ_MLOAD_ENTRY()
#define PQ_MLOAD_RETURN()
#endif

//////////////////////////////////////////////////////////////////
// OS related
//
//
#define XC_PQ_WAIT_MUTEX              (TICK_PER_ONE_MS * 50)              // 50 ms

/*
// Mutex & Lock
static MS_S32 _s32XC_PQMutex = -1;


#define XC_PQ_ENTRY()                 {   if (_s32XC_PQMutex < 0) { return E_XC_ACE_FAIL; }        \
                                        if(OS_OBTAIN_MUTEX(_s32XC_PQMutex, XC_PQ_WAIT_MUTEX) == FALSE) return E_XC_ACE_OBTAIN_MUTEX_FAIL;  }

#define XC_PQ_RETURN(_ret)            {   OS_RELEASE_MUTEX(_s32XC_PQMutex); return _ret; }
*/

//////////////////////////////////////////////////////////////////

void MDrv_PQ_Init(PQ_WIN eWin, PQ_DISPLAY_TYPE eDispType, MS_PQ_Init_Info *pstPQInitInfo)
{
    PQTABLE_INFO PQTableInfo;

#if(ENABLE_PQ_BIN)
    MS_U8 u8ID;
    BYTE i;
#endif

    _bCustomerMainPQEn = FALSE ;
    _bCustomerSubPQEn = FALSE ;
    _PQ_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "_XC_Mutex", MSOS_PROCESS_SHARED);

    if(_PQ_Mutex == -1)
    {
        (printf("[MAPI PQ][%06d] create mutex fail\n", __LINE__));
        return;
    }

    memset(&PQTableInfo, 0, sizeof(PQTableInfo));

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
    PTH_PQ_RET_CHK(pthread_mutexattr_t attr);
    PTH_PQ_RET_CHK(pthread_mutexattr_init(&attr));
    PTH_PQ_RET_CHK(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
    PTH_PQ_RET_CHK(pthread_mutex_init(&_PQ_MLoad_Mutex, &attr));
#endif
#endif

    _status.bIsInitialized = TRUE;
    _status.bIsRunning     = TRUE;

#if(ENABLE_PQ_BIN)

    #if 0 // not pq bin text
    if(pstPQInitInfo->u8PQTextBinCnt)
    {
        gbEnablePQTextBin = 1;
        for(i = 0; i < pstPQInitInfo->u8PQTextBinCnt; i++)
        {
            u8ID = pstPQInitInfo->stPQTextBinInfo[i].u8PQID;
            stPQTextBinHeaderInfo[u8ID].u8BinID = pstPQInitInfo->stPQTextBinInfo[i].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQTextBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQTextBinInfo[i].PQBin_PhyAddr);
#else
            stPQTextBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQTextBinInfo[i].PQBin_PhyAddr);
#endif
            MDrv_PQTextBin_Parsing(&stPQTextBinHeaderInfo[u8ID]);
        }
    }
    else
    {
        //printf("NonPQBin_Text !!\n");
        gbEnablePQTextBin = 0;
    }
    #endif
    if(pstPQInitInfo->u8PQBinCnt)
    {
        gbPQBinEnable = 1;
        MDrv_PQBin_SetDisplayType((PQ_BIN_DISPLAY_TYPE)eDispType);

        switch(pstPQInitInfo->u16PnlWidth)
        {
            case 2560:
                MDrv_PQBin_SetPanelID(1);
                break;
            case 3840:
            default:
                MDrv_PQBin_SetPanelID(0);
                break;
        }

        //because main and sub's common table may be different,
        //so we will take main's common table as correct one.
        //so here load sub first and then use main's overwrite sub's
        //Sub first
        for(i = 2; i < pstPQInitInfo->u8PQBinCnt; i++)
        {
            DRVPQ_PRINT("[DRVPQ_BIN] sub bin%d...\n", i);
            u8ID = pstPQInitInfo->stPQBinInfo[i].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[i].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#endif
            MDrv_PQBin_Parsing(&stPQBinHeaderInfo[u8ID]);
            MDrv_PQBin_LoadCommTable(MDrv_PQBin_GetPanelIdx(), &stPQBinHeaderInfo[u8ID]);
        }

        if(pstPQInitInfo->u8PQBinCustomerCnt == 2)
        {
            u8ID = pstPQInitInfo->stPQBinInfo[5].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[5].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[5].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[5].PQBin_PhyAddr);
#endif
            MDrv_PQBin_Parsing(&stPQBinHeaderInfo[u8ID]);
            MDrv_PQBin_LoadCommTable(PQ_BIN_CUSTOMER_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
            _bCustomerSubPQEn = TRUE ;
        }


        //Main second
        for(i = 0; i < ((pstPQInitInfo->u8PQBinCnt <= 2) ? pstPQInitInfo->u8PQBinCnt : 2); i++)
        {
            DRVPQ_PRINT("[DRVPQ_BIN] main bin%d...\n", i);
            u8ID = pstPQInitInfo->stPQBinInfo[i].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[i].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[i].PQBin_PhyAddr);
#endif
            DRVPQ_PRINT("[DRVPQ_BIN] main bin... address = %x...\n", stPQBinHeaderInfo[u8ID].u32BinStartAddress);
            MDrv_PQBin_Parsing(&stPQBinHeaderInfo[u8ID]);
            MDrv_PQBin_LoadCommTable(MDrv_PQBin_GetPanelIdx(), &stPQBinHeaderInfo[u8ID]);
        }

        if(pstPQInitInfo->u8PQBinCustomerCnt >= 1)
        {
            u8ID = pstPQInitInfo->stPQBinInfo[4].u8PQID;
            stPQBinHeaderInfo[u8ID].u8BinID = (MS_U8)pstPQInitInfo->stPQBinInfo[4].u8PQID;
#ifndef MSOS_TYPE_LINUX
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = MS_PA2KSEG0(pstPQInitInfo->stPQBinInfo[4].PQBin_PhyAddr);
#else
            stPQBinHeaderInfo[u8ID].u32BinStartAddress = /*MS_PA2KSEG0*/(pstPQInitInfo->stPQBinInfo[4].PQBin_PhyAddr);
#endif
            MDrv_PQBin_Parsing(&stPQBinHeaderInfo[u8ID]);
            MDrv_PQBin_LoadCommTable(PQ_BIN_CUSTOMER_PANEL_INDEX, &stPQBinHeaderInfo[u8ID]);
            _bCustomerMainPQEn = TRUE ;
        }
        g_bPqInit = TRUE;
    }
    else
#endif
    {
#if(PQ_ONLY_SUPPORT_BIN)
        //PQTableInfo = PQTableInfo;
#if(ENABLE_PQ_BIN)
        //gbPQBinEnable = 1;
#endif
#else
        //MS_BOOL g_bSkipCommTable = FALSE;
        //printf("NonPQBin !! \n");
#if(ENABLE_PQ_BIN)
        gbPQBinEnable = 0;
#endif
		if (eWin == PQ_SUB_WINDOW)
	        MDrv_PQ_Set_DisplayType_Sub(pstPQInitInfo->u16PnlWidth, eDispType, g_bSkipCommTable);
		else
        	MDrv_PQ_Set_DisplayType_Main(pstPQInitInfo->u16PnlWidth, eDispType, g_bSkipCommTable);
        g_bPqInit = TRUE;
#if(ENABLE_PQ_LOAD_TABLE_INFO)
		if (eWin == PQ_MAIN_WINDOW)
		{
        	MDrv_PQ_PreInitLoadTableInfo(MAIN);
        	MDrv_PQ_PreInitLoadTableInfo(MAINEX);
		}
#if PQ_ENABLE_PIP
		if (eWin == PQ_SUB_WINDOW)
		{
        	MDrv_PQ_PreInitLoadTableInfo(SUB);
        	MDrv_PQ_PreInitLoadTableInfo(SUBEX);
		}
#endif

#endif
#endif // end of #if(PQ_ONLY_SUPPORT_BIN == 0)
    }

#if PQ_ENABLE_PIP
    //dump sub c_sram3, c_sram4 to avoid garbage when enable pip, add these two into skip rule
	if (eWin == PQ_SUB_WINDOW)
	{
	    MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_IP_C_SRAM3_C121_Sub, PQ_IP_C_SRAM3_Sub);
	    MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_IP_C_SRAM4_C2121_Sub, PQ_IP_C_SRAM4_Sub);
	}
#endif



#if 0		//$$$ unknown function
    // Set BK12 [40] [1:0] = b'11 to enable hw double buffer write
    // at blanking area for changing memory format by L_BK_SCMI(0x02)
    // this register is added after T2 U04
    Hal_PQ_set_memfmt_doublebuffer(ENABLE);

    memcpy(&_stPQ_Info, pstPQInitInfo, sizeof(MS_PQ_Init_Info));

    memset(&bw_info, 0x00, sizeof(bw_info));
    bw_info.bDDR2 = pstPQInitInfo->bDDR2;
    bw_info.u32DDRFreq = pstPQInitInfo->u32DDRFreq;
    bw_info.u8BusWidth = pstPQInitInfo->u8BusWidth;
    bw_info.u16output_vtotal = pstPQInitInfo->u16Pnl_vtotal;
    bw_info.u16output_hsize = pstPQInitInfo->u16PnlWidth;
    bw_info.u16OSD_hsize = pstPQInitInfo->u16OSD_hsize;
    bw_info.u32Miu0MemSize = pstPQInitInfo->u32miu0em_size;
    bw_info.u32Miu1MemSize = pstPQInitInfo->u32miu1em_size;

    for(i = 0; i < PQ_MAX_WINDOW; i++)
    {
        _bColorRange0_255[i] = TRUE;
        _bGameModeOn[i] = FALSE;
        _u8RWmappingNum[i] = 0;
        _bRWNumIsReset[i] = TRUE;
        _u16PQSuggestedFrameNum[i] = 0;
    }
#endif

#if (PQ_ENABLE_PIP)
#if (PQ_ONLY_SUPPORT_BIN == 0)
    genDisplayType = eDispType;
#endif
#endif
}

MS_BOOL MDrv_PQ_Exit(void)
{
    return true;
}

MS_U16 MDrv_PQ_GetIPNum(PQ_WIN eWindow)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetIPNum(&stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            return 0xFFFF;
#else
            return MDrv_PQ_GetIPNum_(MAIN);
#endif
        }
    }

#if (PQ_ENABLE_PIP)
    if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetIPNum(&stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            return 0xFFFF;
#else
            return MDrv_PQ_GetIPNum_(SUB);
#endif
        }
    }
#endif

    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}

MS_U16 MDrv_PQ_GetTableNum(PQ_WIN eWindow, MS_U16 u16PQIPIdx)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetTableNum(MDrv_PQBin_GetPanelIdx(), &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            UNUSED(u16PQIPIdx);
            return 0xFFFF;
#else
            return MDrv_PQ_GetTableNum_(MAIN, (MS_U8)u16PQIPIdx);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetTableNum(MDrv_PQBin_GetPanelIdx(), &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            return 0xFFFF;
#else
            return MDrv_PQ_GetTableNum_(SUB, (MS_U8)u16PQIPIdx);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}

MS_U16 MDrv_PQ_GetCurrentTableIndex(PQ_WIN eWindow, MS_U16 u16PQIPIdx)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetCurrentTableIndex(MDrv_PQBin_GetPanelIdx(),  u16PQIPIdx);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            return 0xFFFF;
#else
            return MDrv_PQ_GetCurrentTableIndex_(MAIN, (MS_U8)u16PQIPIdx);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            return MDrv_PQBin_GetCurrentTableIndex(MDrv_PQBin_GetPanelIdx(),  u16PQIPIdx);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            return 0xFFFF;
#else
            return MDrv_PQ_GetCurrentTableIndex_(SUB, (MS_U8)u16PQIPIdx);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
        return 0xFFFF;
    }
}

//Get main page setting from u16IPIdx(column), according to current input source type(row)
MS_U16 MDrv_PQ_GetTableIndex(PQ_WIN eWindow, MS_U16 u16IPIdx)
{
    MS_U8 u16TabIdx = 0;

    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 MDrv_PQBin_GetPanelIdx(),
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16TabIdx = 0;
#else
            u16TabIdx = MDrv_PQ_GetTableIndex_(MAIN, _u16PQSrcType[eWindow], (MS_U8)u16IPIdx);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 MDrv_PQBin_GetPanelIdx(),
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16TabIdx = 0;
#else
            u16TabIdx = MDrv_PQ_GetTableIndex_(SUB, _u16PQSrcType[eWindow], (MS_U8)u16IPIdx);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16TabIdx;
}

//Get main page setting from u16IPIdx(column), according to current input source type(row)
MS_U16 MDrv_PQ_GetCustomerTableIndex(PQ_WIN eWindow, MS_U16 u16IPIdx)
{

    MS_U8 u16TabIdx = 0;


    if(eWindow == PQ_MAIN_WINDOW)
    {
#if ENABLE_PQ_BIN
        if(gbPQBinEnable)
        {

            u16TabIdx = MDrv_PQBin_GetTableIndex(_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 PQ_BIN_CUSTOMER_PANEL_INDEX,
                                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN]);

        }
#else
        u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
#endif

    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if ENABLE_PQ_BIN
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetTableIndex(_u16PQSrcType[eWindow],
                                                 u16IPIdx,
                                                 PQ_BIN_CUSTOMER_PANEL_INDEX,
                                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB]);
        }
#else
        u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
#endif
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16TabIdx;

}

static MS_U16 MDrv_PQ_GetXRuleIPIndex(PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIP)
{
    MS_U8 u16IPIdx = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16IPIdx = MDrv_PQBin_GetXRuleIPIndex(u16XRuleType,
                                                  u16XRuleIP,
                                                  &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16IPIdx = 0;
#else
            u16IPIdx = MDrv_PQ_GetXRuleIPIndex_(MAIN, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIP);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16IPIdx = MDrv_PQBin_GetXRuleIPIndex(u16XRuleType,
                                                  u16XRuleIP,
                                                  &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16IPIdx = 0;
#else
            u16IPIdx = MDrv_PQ_GetXRuleIPIndex_(SUB, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIP);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16IPIdx;
}

static MS_U16 MDrv_PQ_GetXRuleTableIndex(PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP)
{
    MS_U16 u16TabIdx = 0;

    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetXRuleTableIndex(u16XRuleType,
                        u16XRuleIdx,
                        u16XRuleIP,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16TabIdx = 0;
#else
            u16TabIdx = MDrv_PQ_GetXRuleTableIndex_(MAIN, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIdx, (MS_U8)u16XRuleIP);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16TabIdx = MDrv_PQBin_GetXRuleTableIndex(u16XRuleType,
                        u16XRuleIdx,
                        u16XRuleIP,
                        &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16TabIdx = 0;
#else
            u16TabIdx = MDrv_PQ_GetXRuleTableIndex_(SUB, (MS_U8)u16XRuleType, (MS_U8)u16XRuleIdx, (MS_U8)u16XRuleIP);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16TabIdx;
}

static MS_U16 MDrv_PQ_GetXRuleIPNum(PQ_WIN eWindow, MS_U16 u16XRuleType)
{
    MS_U16 u16IPNum = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16IPNum = MDrv_PQBin_GetXRuleIPNum(u16XRuleType,
                                                &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16IPNum = 0;
#else
            u16IPNum = MDrv_PQ_GetXRuleIPNum_(MAIN, (MS_U8)u16XRuleType);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16IPNum = MDrv_PQBin_GetXRuleIPNum(u16XRuleType,
                                                &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16IPNum = 0;
#else
            u16IPNum = MDrv_PQ_GetXRuleIPNum_(SUB, (MS_U8)u16XRuleType);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16IPNum;
}

static MS_U16 MDrv_PQ_GetGRule_LevelIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleLevelIndex)
{
    MS_U16 u16Ret = 0;
    if (!g_bPqInit)
        return 0xFF;

#if(ENABLE_PQ_BIN)
    PQ_BIN_TYPE enPQBinTypeMain = PQ_BIN_STD_MAIN;
#if (PQ_ENABLE_PIP)
    PQ_BIN_TYPE enPQBinTypeSub = PQ_BIN_STD_SUB;
#endif
#endif
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
       if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum)
        {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
            PQGRULE_DBG(printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum));
            u16GRuleType = u16GRuleType - stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum;
            enPQBinTypeMain = PQ_BIN_CUSTOMER_MAIN;
            if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum)
            {
                printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum);
                return 0;
            }
#else
            printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum);
            return 0;
#endif
        }

        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_LevelIndex(MDrv_PQBin_GetPanelIdx(),
                     u16GRuleType,
                     u16GRuleLevelIndex,
                     &stPQBinHeaderInfo[enPQBinTypeMain]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
#else
            u16Ret = MDrv_PQ_GetGRule_LevelIndex_(MAIN, (MS_U8)u16GRuleType, (MS_U8)u16GRuleLevelIndex);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum)
        {
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
            PQGRULE_DBG(printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum));
            u16GRuleType = u16GRuleType - stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum;
            enPQBinTypeSub = PQ_BIN_CUSTOMER_SUB;
            if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum)
            {
                printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum);
                return 0;
            }
#else
            printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum);
            return 0;
#endif
        }

        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_LevelIndex(MDrv_PQBin_GetPanelIdx(),
                     u16GRuleType,
                     u16GRuleLevelIndex,
                     &stPQBinHeaderInfo[enPQBinTypeSub]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
#else
            u16Ret = MDrv_PQ_GetGRule_LevelIndex_(SUB, (MS_U8)u16GRuleType, (MS_U8)u16GRuleLevelIndex);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
    return u16Ret;
}

static MS_U16 MDrv_PQ_GetGRule_IPIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = MDrv_PQBin_GetGRule_IPIndex(u16GRuleType,
                                                 u16GRuleIPIndex,
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
#else
            u16Ret = MDrv_PQ_GetGRule_IPIndex_(MAIN, (MS_U8)u16GRuleType, (MS_U8)u16GRuleIPIndex);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = MDrv_PQBin_GetGRule_IPIndex(u16GRuleType,
                                                 u16GRuleIPIndex,
                                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
#else
            u16Ret = MDrv_PQ_GetGRule_IPIndex_(SUB, (MS_U8)u16GRuleType, (MS_U8)u16GRuleIPIndex);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}

#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE || ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
static MS_U16 MDrv_PQ_GetCustomerGRule_IPIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = MDrv_PQBin_GetGRule_IPIndex(u16GRuleType,
                                                 u16GRuleIPIndex,
                                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN]);
        }
#endif
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = MDrv_PQBin_GetGRule_IPIndex(u16GRuleType,
                                                 u16GRuleIPIndex,
                                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB]);
        }
#endif
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}
#endif

static MS_U16 MDrv_PQ_GetGRule_TableIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    MS_U16 u16PQ_IP_Idx = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(u16GRuleType,
                     _u16PQSrcType[eWindow],
                     u16PQ_NRIdx,
                     u16GRuleIPIndex,
                     &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
#else
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(MAIN, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
#endif
        }
        if (u16Ret == PQ_IP_ORG)
        {
            u16PQ_IP_Idx = MDrv_PQ_GetGRule_IPIndex(PQ_MAIN_WINDOW, u16GRuleType, u16GRuleIPIndex);
            u16Ret = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, u16PQ_IP_Idx);
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(u16GRuleType,
                     _u16PQSrcType[eWindow],
                     u16PQ_NRIdx,
                     u16GRuleIPIndex,
                     &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
#else
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(SUB, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}

#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE || ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
static MS_U16 MDrv_PQ_GetGRule_CustomerTableIndex(PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex)
{
    MS_U16 u16Ret = 0;
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(u16GRuleType,
                     _u16PQSrcType[eWindow],
                     u16PQ_NRIdx,
                     u16GRuleIPIndex,
                     &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
#else
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(MAIN, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            u16Ret = (MS_U8) MDrv_PQBin_GetGRule_TableIndex(u16GRuleType,
                     _u16PQSrcType[eWindow],
                     u16PQ_NRIdx,
                     u16GRuleIPIndex,
                     &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN)
            u16Ret = 0;
#else
            u16Ret = MDrv_PQ_GetGRule_TableIndex_(SUB, (MS_U8)u16GRuleType, _u16PQSrcType[eWindow], (MS_U8)u16PQ_NRIdx, (MS_U8)u16GRuleIPIndex);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

    return u16Ret;
}
#endif


void MDrv_PQ_LoadTableData(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableData(MDrv_PQBin_GetPanelIdx(),
                                     u16TabIdx,
                                     u16PQIPIdx,
                                     &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],
                                     pTable,
                                     u16TableSize);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTableData_(MAIN, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx, pTable, u16TableSize);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableData(MDrv_PQBin_GetPanelIdx(),
                                     u16TabIdx,
                                     u16PQIPIdx,
                                     &stPQBinHeaderInfo[PQ_BIN_STD_SUB],
                                     pTable,
                                     u16TableSize);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTableData_(SUB, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx, pTable, u16TableSize);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}

void MDrv_PQ_LoadTable(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx)
{
    if (!g_bPqInit)
        return;
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTable(MDrv_PQBin_GetPanelIdx(),
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTable_(MAIN, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
            //20140205 for Read Current frame mode
            if (u16PQIPIdx == PQ_IP_MADi_Main && g_bReadCurFrameMode == TRUE)
                MApi_XC_WriteByteMask(REG_SC_BK12_07_H, 0x00, 0xE0);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTable(MDrv_PQBin_GetPanelIdx(),
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTable_(SUB, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
            //20140205 for Read Current frame mode
            if (u16PQIPIdx == PQ_IP_MADi_Sub && g_bReadCurFrameMode == TRUE)
                MApi_XC_WriteByteMask(REG_SC_BK12_47_H, 0x00, 0xE0);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}

void MDrv_PQ_LoadCustomerTable(PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTable(PQ_BIN_CUSTOMER_PANEL_INDEX,
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTable_(MAIN, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTable(PQ_BIN_CUSTOMER_PANEL_INDEX,
                                 u16TabIdx,
                                 u16PQIPIdx,
                                 &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB]);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_LoadTable_(SUB, (MS_U8)u16TabIdx, (MS_U8)u16PQIPIdx);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}

void MDrv_PQ_CheckSettings(PQ_WIN eWindow)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            //ToDo MAINEX
            MDrv_PQBin_CheckCommTable(MDrv_PQBin_GetPanelIdx(),
                                      &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);

            MDrv_PQBin_CheckTableBySrcType(_u16PQSrcType[eWindow],
                                           PQ_BIN_IP_ALL,
                                           MDrv_PQBin_GetPanelIdx(),
                                           &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],
                                           eWindow);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
#if(ENABLE_PQ_EX)
            MDrv_PQ_CheckCommTable_(MAINEX);
            MDrv_PQ_CheckTableBySrcType_(MAINEX, _u16PQSrcType[eWindow], PQ_IP_ALL);
#endif
            MDrv_PQ_CheckCommTable_(MAIN);
            MDrv_PQ_CheckTableBySrcType_(MAIN, _u16PQSrcType[eWindow], PQ_IP_ALL);
#endif
        }
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            //ToDo SUBEX
            MDrv_PQBin_CheckCommTable(MDrv_PQBin_GetPanelIdx(),
                                      &stPQBinHeaderInfo[PQ_BIN_STD_SUB]);

            MDrv_PQBin_CheckTableBySrcType(_u16PQSrcType[eWindow],
                                           PQ_BIN_IP_ALL,
                                           MDrv_PQBin_GetPanelIdx(),
                                           &stPQBinHeaderInfo[PQ_BIN_STD_SUB],
                                           eWindow);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_CheckCommTable_(SUBEX);
            MDrv_PQ_CheckTableBySrcType_(SUBEX, _u16PQSrcType[eWindow], PQ_IP_ALL);
            MDrv_PQ_CheckCommTable_(SUB);
            MDrv_PQ_CheckTableBySrcType_(SUB, _u16PQSrcType[eWindow], PQ_IP_ALL);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}


#if(ENABLE_PQ_MLOAD)
void MDrv_PQ_Set_MLoadEn(PQ_WIN eWindow, MS_BOOL bEn)
{
    if(MApi_XC_MLoad_GetStatus() != E_MLOAD_ENABLED)
    {
        bEn = FALSE;
    }

    if(bEn)
    {
        PQ_MLOAD_ENTRY();
    }

    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)

        if(gbPQBinEnable)
        {
            MDrv_PQBin_Set_MLoadEn(bEn);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_Set_MLoadEn_(MAIN, bEn);
#endif
        }

    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_Set_MLoadEn(bEn);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_Set_MLoadEn_(SUB, bEn);
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }


    if(bEn == FALSE)
    {
        PQ_MLOAD_RETURN();
    }
}
#endif

void MDrv_PQ_LoadSettings(PQ_WIN eWindow, MS_BOOL isWinRot)
{
    if (!g_bPqInit)
        return;
    DRVPQ_PRINT("[DRVPQ] Src Index = %d\n",_u16PQSrcType[eWindow]);
    if(eWindow == PQ_MAIN_WINDOW)
    {
        //load swdriver here before all the others PQ settings.
        //Pls don't add swDriver in to skip rule, otherwise below code cannot set swDriver.
        MDrv_PQ_LoadTable(eWindow, MDrv_PQ_GetTableIndex(eWindow, PQ_IP_SwDriver_Main), PQ_IP_SwDriver_Main);
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],
                                          eWindow,
                                          isWinRot);

            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_EXT_MAIN],
                                          eWindow,
                                          isWinRot);

        }
        else
#endif
        {
            PQTAB_DBG(printf("[PQ_LoadSetting] SrcType = %u", _u16PQSrcType[eWindow]));
#if(PQ_ONLY_SUPPORT_BIN == 0)
            PQTAB_DBG(printf("MAIN table\n"));
            MDrv_PQ_LoadTableBySrcType_(MAIN, _u16PQSrcType[eWindow], PQ_IP_ALL, isWinRot);
#if(ENABLE_PQ_EX == 1)
            PQTAB_DBG(printf("MAINEX table\n"));
            MDrv_PQ_LoadTableBySrcType_(MAINEX, _u16PQSrcType[eWindow], PQ_IP_ALL, isWinRot);
#endif
            PQTAB_DBG(printf("...done\n"));
#endif
        }

#if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
#if PQ_UC_CTL
        MDrv_PQ_LoadTable(eWindow, MDrv_PQ_GetTableIndex(eWindow, PQ_IP_UC_CTL_Main), PQ_IP_UC_CTL_Main);
#endif
#if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_STD_SUB],
                                          eWindow, isWinRot);

            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_EXT_SUB],
                                          eWindow, isWinRot);
        }
        else
#endif
        {
#if(PQ_ONLY_SUPPORT_BIN == 0)
            PQTAB_DBG(printf("SUB table\n"));
            MDrv_PQ_LoadTableBySrcType_(SUB, _u16PQSrcType[eWindow] + 0x80, PQ_IP_ALL, isWinRot);

            PQTAB_DBG(printf("SUB MAINEX table\n"));
            MDrv_PQ_LoadTableBySrcType_(SUBEX, _u16PQSrcType[eWindow] + 0x80, PQ_IP_ALL, isWinRot);
            PQTAB_DBG(printf("...done\n"));
#endif
        }
    }
#endif
    else
    {
        MS_ASSERT(0);
    }

#if(ENABLE_PQ_LOAD_TABLE_INFO)
    if(eWindow == PQ_MAIN_WINDOW)
    {
        MDRV_PQ_PrintLoadTableInfo(PQ_LOAD_TABLE_MAIN);
        MDRV_PQ_PrintLoadTableInfo(PQ_LOAD_TABLE_MAIN_EX);
    }
#if PQ_ENABLE_PIP
    else if(eWindow == PQ_SUB_WINDOW)
    {
        MDRV_PQ_PrintLoadTableInfo(PQ_LOAD_TABLE_SUB);
        MDRV_PQ_PrintLoadTableInfo(PQ_LOAD_TABLE_SUB_EX);
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
#endif


}

void MDrv_PQ_LoadCustomerSettings(PQ_WIN eWindow)
{
    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {

            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          PQ_BIN_CUSTOMER_PANEL_INDEX,
                                          &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN],
                                          eWindow, FALSE);

        }
#endif
    }
#if (PQ_ENABLE_PIP)
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {
            MDrv_PQBin_LoadTableBySrcType(_u16PQSrcType[eWindow],
                                          PQ_BIN_IP_ALL,
                                          MDrv_PQBin_GetPanelIdx(),
                                          &stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB],
                                          eWindow, FALSE);
        }
#endif
    }
#endif
    else
    {
        MS_ASSERT(0);
    }
}

void MDrv_PQ_Set_DTVInfo(PQ_WIN eWindow, MS_PQ_Dtv_Info *pstPQDTVInfo)
{
    memcpy(&_stDTV_Info[eWindow], pstPQDTVInfo, sizeof(MS_PQ_Dtv_Info));

    PQINFO_DBG(printf("PQ DTV Info:Win=%u, type=%u\n", eWindow, _stDTV_Info[eWindow].eType));
}

void MDrv_PQ_Set_MultiMediaInfo(PQ_WIN eWindow, MS_PQ_MuliMedia_Info *pstPQMMInfo)
{
    memcpy(&_stMultiMedia_Info[eWindow], pstPQMMInfo, sizeof(MS_PQ_MuliMedia_Info));
    PQINFO_DBG(printf("PQ MM Info:Win=%u, type=%u\n", eWindow, _stMultiMedia_Info[eWindow].eType));
}

void MDrv_PQ_Set_VDInfo(PQ_WIN eWindow, MS_PQ_Vd_Info *pstPQVDInfo)
{
    memcpy(&_stVD_Info[eWindow], pstPQVDInfo, sizeof(MS_PQ_Vd_Info));

    PQINFO_DBG(printf("PQ VD Info:Win=%u, SigType=%u, bSCARTRGB=%u, VIFIn=%u\n",
                      eWindow,
                      _stVD_Info[eWindow].enVideoStandard,
                      _stVD_Info[eWindow].bIsSCART_RGB,
                      _stVD_Info[eWindow].bIsVIFIN));

}

void MDrv_PQ_Set_ModeInfo(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo)
{

    memcpy(&_stMode_Info[eWindow], pstPQModeInfo, sizeof(MS_PQ_Mode_Info));

    //printf("PQ Set Mode Info: %s, Src=%x\n", (eWindow)?("SubWin"):("MainWin"), enInputSourceType);
#if 1
    if(QM_IsSourceYPbPr(enInputSourceType))
    {
        if(((QM_H_Size_Check_x1(pstPQModeInfo->u16input_hsize, 720)) ||
                (QM_H_Size_Check_x2(pstPQModeInfo->u16input_hsize, 720)) ||
                (QM_H_Size_Check_x4(pstPQModeInfo->u16input_hsize, 720)) ||
                (QM_H_Size_Check_x8(pstPQModeInfo->u16input_hsize, 720))) &&
                (pstPQModeInfo->u16input_vsize < 500) &&
                (pstPQModeInfo->u16input_vfreq < 650))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                                    PQ_MD_720x480_60I :
                                    PQ_MD_720x480_60P;

        }
        else if(((QM_H_Size_Check_x1(pstPQModeInfo->u16input_hsize, 720)) ||
                 (QM_H_Size_Check_x2(pstPQModeInfo->u16input_hsize, 720)) ||
                 (QM_H_Size_Check_x4(pstPQModeInfo->u16input_hsize, 720)) ||
                 (QM_H_Size_Check_x8(pstPQModeInfo->u16input_hsize, 720))) &&
                (pstPQModeInfo->u16input_vsize < 600) &&
                (pstPQModeInfo->u16input_vfreq < 550))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                                    PQ_MD_720x576_50I :
                                    PQ_MD_720x576_50P;
        }
        else if(((QM_H_Size_Check_x1(pstPQModeInfo->u16input_hsize, 1280)) ||
                 (QM_H_Size_Check_x2(pstPQModeInfo->u16input_hsize, 1280)) ||
                 (QM_H_Size_Check_x4(pstPQModeInfo->u16input_hsize, 1280)) ||
                 (QM_H_Size_Check_x8(pstPQModeInfo->u16input_hsize, 1280))) &&
                (pstPQModeInfo->u16input_vsize < 800) &&
                (pstPQModeInfo->bInterlace == FALSE))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                                    PQ_MD_1280x720_50P :
                                    PQ_MD_1280x720_60P;

        }
        else if(((QM_H_Size_Check_x1(pstPQModeInfo->u16input_hsize, 1920)) ||
                 (QM_H_Size_Check_x2(pstPQModeInfo->u16input_hsize, 1920)) ||
                 (QM_H_Size_Check_x4(pstPQModeInfo->u16input_hsize, 1920)) ||
                 (QM_H_Size_Check_x8(pstPQModeInfo->u16input_hsize, 1920))) &&
                (pstPQModeInfo->u16input_vsize < 1100))
        {
            if(pstPQModeInfo->bInterlace)
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                                        PQ_MD_1920x1080_50I :
                                        PQ_MD_1920x1080_60I;
            }
            else
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 245) ?  PQ_MD_1920x1080_24P :
                                        (pstPQModeInfo->u16input_vfreq < 270) ?  PQ_MD_1920x1080_25P :
                                        (pstPQModeInfo->u16input_vfreq < 350) ?  PQ_MD_1920x1080_30P :
                                        (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1920x1080_50P :
                                        PQ_MD_1920x1080_60P;

            }
        }
        else
        {
            _u8ModeIndex[eWindow] = PQ_MD_720x576_50I;
        }
    }
#else
	if (INPUT_IS_VGA(enInputSourceType) || INPUT_IS_DVI(enInputSourceType) )
	{

	}
#endif
    else if(QM_IsSourceHDMI(enInputSourceType))
    {
        if((pstPQModeInfo->u16input_hsize < 1500) &&
                (pstPQModeInfo->u16input_vsize < 500) &&
                (pstPQModeInfo->u16input_vfreq < 650))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                                    PQ_MD_720x480_60I :
                                    PQ_MD_720x480_60P;

        }
        else if((pstPQModeInfo->u16input_hsize < 1500) &&
                (pstPQModeInfo->u16input_vsize < 600) &&
                (pstPQModeInfo->u16input_vfreq <  550))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->bInterlace) ?
                                    PQ_MD_720x576_50I :
                                    PQ_MD_720x576_50P;
        }
        else if((pstPQModeInfo->u16input_hsize < 1300) &&
                (pstPQModeInfo->u16input_vsize < 800) &&
                (pstPQModeInfo->bInterlace == FALSE))
        {
            _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                                    PQ_MD_1280x720_50P :
                                    PQ_MD_1280x720_60P;

        }
        else if((pstPQModeInfo->u16input_hsize < 1930) &&
                (pstPQModeInfo->u16input_vsize < 1100))
        {
            if(pstPQModeInfo->bInterlace)
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                                        PQ_MD_1920x1080_50I :
                                        PQ_MD_1920x1080_60I;
            }
            else
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 245) ?  PQ_MD_1920x1080_24P :
                                        (pstPQModeInfo->u16input_vfreq < 270) ?  PQ_MD_1920x1080_25P :
                                        (pstPQModeInfo->u16input_vfreq < 350) ?  PQ_MD_1920x1080_30P :
                                        (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1920x1080_50P :
                                        PQ_MD_1920x1080_60P;

            }
        }
        else if(pstPQModeInfo->u16input_hsize < 1350 &&
                pstPQModeInfo->u16input_vsize < 1550)
        {
            //special handle 1280X1470p
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1280x720_50P :
                               PQ_MD_1280x720_60P;
            }
        }
        else if(pstPQModeInfo->u16input_hsize < 1930 &&
                pstPQModeInfo->u16input_vsize < 2300)
        {
            //special handle 1920X2205p
            if(pstPQModeInfo->bInterlace)
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 550) ?
                               PQ_MD_1920x1080_50I :
                               PQ_MD_1920x1080_60I;
            }
            else
            {
                _u8ModeIndex[eWindow] = (pstPQModeInfo->u16input_vfreq < 250) ?  PQ_MD_1920x1080_24P :
                               (pstPQModeInfo->u16input_vfreq < 350) ?  PQ_MD_1920x1080_30P :
                               (pstPQModeInfo->u16input_vfreq < 550) ?  PQ_MD_1920x1080_50P :
                               PQ_MD_1920x1080_60P;

            }
        }
        else
        {
            _u8ModeIndex[eWindow] = PQ_MD_720x576_50I;
        }
    }
    else
    {
        _u8ModeIndex[eWindow] = PQ_MD_Num;
    }

    if(QM_IsSourceYPbPr(enInputSourceType) || QM_IsSourceHDMI(enInputSourceType))
    {
        if(_u8ModeIndex[eWindow] <= (MS_U8)PQ_MD_720x576_50P)
        _gIsSrcHDMode[eWindow] = 0;
        else
            _gIsSrcHDMode[eWindow] = 1;

    }
    else if(QM_IsSourceVD(enInputSourceType))
    {
            _gIsSrcHDMode[eWindow] = 0;
    }
    else
    {
        if(pstPQModeInfo->u16input_hsize >= 1200)
            _gIsSrcHDMode[eWindow] = 1;
        else
            _gIsSrcHDMode[eWindow] = 0;
    }

    PQINFO_DBG(
        printf("PQ ModeInfo:%d input(%d, %d), disp(%d, %d), ModeIdx=%d, FBL=%u, Interlace=%u, InV=%u, OutV=%u, inVtt=%u\n",
               eWindow,
               _stMode_Info[eWindow].u16input_hsize,
               _stMode_Info[eWindow].u16input_vsize,
               _stMode_Info[eWindow].u16display_hsize,
               _stMode_Info[eWindow].u16display_vsize,
               _u8ModeIndex[eWindow],
               _stMode_Info[eWindow].bFBL,
               _stMode_Info[eWindow].bInterlace,
               _stMode_Info[eWindow].u16input_vfreq,
               _stMode_Info[eWindow].u16ouput_vfreq,
               _stMode_Info[eWindow].u16input_vtotal);
    );

}

void MDrv_PQ_SetHDMIInfo(PQ_WIN eWindow, const MS_PQ_Hdmi_Info* const pstPQHDMIInfo)
{
    _stHDMI_Info[eWindow].bIsHDMI = pstPQHDMIInfo->bIsHDMI;
    _stHDMI_Info[eWindow].enColorFmt = pstPQHDMIInfo->enColorFmt;

    PQINFO_DBG(printf("PQ HDMI, bHDMI=%u, colorfmt=%u\n",
                      _stHDMI_Info[eWindow].bIsHDMI,
                      _stHDMI_Info[eWindow].enColorFmt));
}

void MDrv_PQ_SetHDMI_PC(PQ_WIN eWindow, MS_BOOL bIsTrue)
{
    _stHDMI_Info[eWindow].bIsHDMIPC = bIsTrue;
}

MS_BOOL MDrv_PQ_GetHDMI_PC_Status(PQ_WIN eWindow)
{
    return _stHDMI_Info[eWindow].bIsHDMIPC;
}

void MDrv_PQ_DesideSrcType(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType)
{

    _enInputSourceType[eWindow] = enInputSourceType;
    _u16PQSrcType[eWindow] = QM_InputSourceToIndex(eWindow, enInputSourceType);
    _u16PQSrcType_DBK_Detect[eWindow] = _u16PQSrcType[eWindow];                     //For Auto_DBK SW driver used
    PQTAB_DBG(printf("[PQ_DesideSrcType] window=%u, enInputSrcType=%u, SrcType=%u\n",
                     eWindow, enInputSourceType, _u16PQSrcType[eWindow]));

    Hal_PQ_set_sourceidx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, _u16PQSrcType[eWindow]);

#if(ENABLE_PQ_LOAD_TABLE_INFO)

    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {

        }
        else
#endif
        {
            MDrv_PQ_Set_LoadTableInfo_SrcType(MAIN, _u16PQSrcType[eWindow]);
            MDrv_PQ_Set_LoadTableInfo_SrcType(MAINEX, _u16PQSrcType[eWindow]);
        }
    }
#if PQ_ENABLE_PIP
    else if(eWindow == PQ_SUB_WINDOW)
    {
#if(ENABLE_PQ_BIN)
        if(gbPQBinEnable)
        {

        }
        else
#endif
        {
            MDrv_PQ_Set_LoadTableInfo_SrcType(SUB, _u16PQSrcType[eWindow]);
            MDrv_PQ_Set_LoadTableInfo_SrcType(SUBEX, _u16PQSrcType[eWindow]);
        }
    }
#endif

#endif //#if(ENABLE_PQ_LOAD_TABLE_INFO)

}

PQ_INPUT_SOURCE_TYPE MDrv_PQ_GetInputSourceType(PQ_WIN eWindow)
{
    return _enInputSourceType[eWindow];
}

MS_U16 MDrv_PQ_GetSrcType(PQ_WIN eWindow)
{
    return _u16PQSrcType[eWindow];
}

MS_U16 MDrv_PQ_GetDelayTime(void)
{
    return _u16PQDelyaTime;
}

// if not HSD case, ignore return value
static MS_BOOL _MDrv_PQ_LoadScalingTable(PQ_WIN eWindow,
        MS_U8 eXRuleType,
        MS_U8 u8XRuleIP,
        MS_BOOL bPreV_ScalingDown,
        MS_BOOL bInterlace,
        MS_BOOL bColorSpaceYUV,
        MS_U16 u16InputSize,
        MS_U16 u16SizeAfterScaling)
{
    MS_U32 u32Ratio;
    MS_U16 u16IPIdx;
#if PQ_ENABLE_PIP
    MS_U16 u16SFilter = 0;
#endif

#if(ENABLE_PQ_BIN)
    MS_U16 u16TabIdx = gbPQBinEnable == 0 ? PQ_IP_NULL : PQ_BIN_IP_NULL;
#else
    MS_U16 u16TabIdx = PQ_IP_NULL;
#endif
    MS_U8 u8XRuleIdx = 0xFF;

    if(u16InputSize == 0)
        u32Ratio = 2000;
    else
        u32Ratio = ((MS_U32) u16SizeAfterScaling * 1000) / u16InputSize;

    u16IPIdx = MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u8XRuleIP);

    SRULE_DBG(printf("SRuleIP=%d, IPIdx=%d, input=%d, output=%d, ratio=%ld, ",
                     (MS_U16)u8XRuleIP, (MS_U16)u16IPIdx,
                     u16InputSize, u16SizeAfterScaling, u32Ratio));

    if(bPreV_ScalingDown && bInterlace)
    {
        u8XRuleIdx = PQ_HSDRule_PreV_ScalingDown_Interlace_Main;
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
    }
    else if(bPreV_ScalingDown && (!bInterlace))
    {
        u8XRuleIdx = PQ_HSDRule_PreV_ScalingDown_Progressive_Main;
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
    }


#if(ENABLE_PQ_BIN)
    if(((gbPQBinEnable == 0) && (u16TabIdx != PQ_IP_NULL)) ||
            ((gbPQBinEnable == 1) && (u16TabIdx != PQ_BIN_IP_NULL)))
#else
    if(u16TabIdx != PQ_IP_NULL)
#endif
    {

        SRULE_DBG(printf("u8XRuleIdx: PreV down, interlace:%u", bInterlace));
        SRULE_DBG(printf("(a)tabidx=%u\n", (MS_U16)u16TabIdx));
    }

#if(ENABLE_PQ_BIN)
    if(((gbPQBinEnable == 1) && (u16TabIdx == PQ_BIN_IP_NULL)) ||
            ((gbPQBinEnable == 0) && (u16TabIdx == PQ_IP_NULL)))
#else
    if(u16TabIdx == PQ_IP_NULL)
#endif
    {
        if(u32Ratio > 1000)
        {
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
            SRULE_DBG(printf("u8XRuleIdx: >x1, "));
            SRULE_DBG(printf("(c)tabidx=%u\n", (MS_U16)u16TabIdx));
        }
        else
        {
            if(bColorSpaceYUV)
            {
                if(u32Ratio == 1000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_10x_YUV_Main;
                else if(u32Ratio >= 900)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_09x_YUV_Main;
                else if(u32Ratio >= 800)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_08x_YUV_Main;
                else if(u32Ratio >= 700)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_07x_YUV_Main;
                else if(u32Ratio >= 600)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_06x_YUV_Main;
                else if(u32Ratio >= 500)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_05x_YUV_Main;
                else if(u32Ratio >= 400)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_04x_YUV_Main;
                else if(u32Ratio >= 300)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_03x_YUV_Main;
                else if(u32Ratio >= 200)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_02x_YUV_Main;
                else if(u32Ratio >= 100)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_01x_YUV_Main;
                else
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_00x_YUV_Main;
            }
            else
            {
                if(u32Ratio == 1000)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_10x_RGB_Main;
                else if(u32Ratio >= 900)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_09x_RGB_Main;
                else if(u32Ratio >= 800)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_08x_RGB_Main;
                else if(u32Ratio >= 700)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_07x_RGB_Main;
                else if(u32Ratio >= 600)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_06x_RGB_Main;
                else if(u32Ratio >= 500)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_05x_RGB_Main;
                else if(u32Ratio >= 400)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_04x_RGB_Main;
                else if(u32Ratio >= 300)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_03x_RGB_Main;
                else if(u32Ratio >= 200)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_02x_RGB_Main;
                else if(u32Ratio >= 100)
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_01x_RGB_Main;
                else
                    u8XRuleIdx = PQ_HSDRule_ScalingDown_00x_RGB_Main;
            }

            SRULE_DBG(printf("u8XRuleIdx=%u, ", (MS_U16)u8XRuleIdx));
            if(u8XRuleIdx == 0xFF)
            {
                MS_ASSERT(0);
                return 1;
            }

            u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, (MS_U16)eXRuleType, (MS_U16)u8XRuleIdx, (MS_U16)u8XRuleIP);
#if(ENABLE_PQ_BIN)
            if(((gbPQBinEnable == 1) && (u16TabIdx == PQ_BIN_IP_NULL)) ||
                    ((gbPQBinEnable == 0) && (u16TabIdx == PQ_IP_NULL)))
#else
            if(u16TabIdx == PQ_IP_NULL)
#endif
            {
                u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
                SRULE_DBG(printf("(d)tabidx=%u\n", u16TabIdx));
            }
            else
            {
                SRULE_DBG(printf("(e)tabidx=%u\n", u16TabIdx));
            }
        }
    }

#if PQ_EN_DMS_SW_CTRL
    if((u16IPIdx == PQ_IP_DMS_V_12L_Main) && (PQ_MAIN_WINDOW == eWindow))
    {
        if(u16TabIdx == PQ_IP_DMS_V_12L_OFF_Main)
        {
            _bDMSV12L_PQConfigEn = FALSE;
        }
        else
        {
            _bDMSV12L_PQConfigEn = TRUE;
        }
    }

    // App will call halt_subwin to disconnet sub window.
    // It will casue PQ display type to be PQ_DISPLAY_ONE.
    // But, sub window is still on and DMS_LV_12 will be set
    // to On in HSD rulee for main window.
    // For fix this issue, we need to check sub window wheter
    // on or off to descide DMS_LV_12 off or on.

    //Disable DMS_V12_L
    // 1.VSD : V prescaling, 2. PIP, 3. FBL
        if((((eXRuleType == E_XRULE_VSD)&&(bPreV_ScalingDown))
            ||(_stMode_Info[eWindow].bFBL)
#if PQ_TB_FOR_TV
            || (E_XC_3D_INPUT_MODE_NONE != MApi_XC_Get_3D_Input_Mode(MAIN_WINDOW))
#endif
#if PQ_ENABLE_PIP
            || (PQ_SUB_WINDOW == eWindow)
            || ((PQ_MAIN_WINDOW == eWindow) && (_bDMSV12L_PIPEn == FALSE))
#endif
            )
          &&
            ((PQ_IP_DMS_V_12L_Main == MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u8XRuleIP))
#if PQ_ENABLE_PIP
             || (PQ_IP_DMS_V_12L_Sub == MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u8XRuleIP))
#endif
             )
        )
        {
            //Disable De-Mosquito
            SRULE_DBG(printf("[SRule] Disable De-Mosquito...\n"));
            u16TabIdx = PQ_IP_DMS_V_12L_OFF_Main;
            u16IPIdx = PQ_IP_DMS_V_12L_Main;
            eWindow = PQ_MAIN_WINDOW;
        }
#endif
#if PQ_ENABLE_PIP
    if(((u16IPIdx == PQ_IP_SRAM3_Sub) || (u16IPIdx == PQ_IP_SRAM4_Sub) ) &&(PQ_SUB_WINDOW == eWindow))
    {
            //$$$
            //SC_BK_SWITCH(0x23);
            u16SFilter =  MApi_XC_R2BYTE(REG_SC_BK23_0B_L);//msRead2Byte(u32ScBaseAddr + (REG_SC_BK23_0B_L & 0xFF));//MApi_XC_R2BYTE(REG_SC_BK23_0B_L);
            MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, 0x0303 , 0xFFFF );
            //msWrite2ByteMask(u32ScBaseAddr + (REG_SC_BK23_0B_L & 0xFF), 0x0303, 0xFFFF);
    }
#endif

#if (ENABLE_PQ_MLOAD)
    if(eXRuleType == E_XRULE_HSD)
    {
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
    }
#endif

    if (u16IPIdx == PQ_IP_DMS_V_12L_Main)
        u16TabIdx = PQ_IP_DMS_V_12L_OFF_Main;

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);

    if(PQ_MAIN_WINDOW == eWindow)
    {
        switch (u16IPIdx)
        {
            case PQ_IP_SRAM1_Main:
                _u16SRAM1 = u16TabIdx;
                break;

            case PQ_IP_SRAM2_Main:
                _u16SRAM2 = u16TabIdx;
                break;

            default:
                break;
        }
    }

#if (ENABLE_PQ_MLOAD)
    if(eXRuleType == E_XRULE_HSD)
    {
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
    }
#endif

#if PQ_ENABLE_PIP
    if(((u16IPIdx == PQ_IP_SRAM3_Sub) || (u16IPIdx == PQ_IP_SRAM4_Sub)) &&(PQ_SUB_WINDOW == eWindow))
    {
        //SC_BK_SWITCH(0x23);
        //msWrite2ByteMask(u32ScBaseAddr + (REG_SC_BK23_0B_L & 0xFF), u16SFilter, 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, u16SFilter , 0xFFFF );
    }
#endif

    if(eXRuleType == E_XRULE_VSD)
        return (u16TabIdx  == PQ_IP_VSD_Bilinear_Main) ; // PreVSDMode: 0:Cb, 1:Bilinear
    else
        return (u16TabIdx != PQ_IP_HSD_Y_CB_Main); // PreHSDMode - 0:Cb, 1:Adv

}

MS_BOOL MDrv_PQ_LoadScalingTable(PQ_WIN eWindow,
                                 MS_U8 eXRuleType,
                                 MS_BOOL bPreV_ScalingDown,
                                 MS_BOOL bInterlace,
                                 MS_BOOL bColorSpaceYUV,
                                 MS_U16 u16InputSize,
                                 MS_U16 u16SizeAfterScaling)
{
    MS_BOOL bRet = 0; // default is CB mode
    MS_U16 i;

    if(eXRuleType > 3)
        MS_ASSERT(0);

    SRULE_DBG(printf("[PQ_LoadScalingTable] HSD/VSD/HSP/VSP:%u\n", (MS_U16)eXRuleType));

    for(i = 0; i < MDrv_PQ_GetXRuleIPNum(eWindow, (MS_U16)eXRuleType); i++)
    {
        MS_BOOL bSDMode;
        bSDMode = _MDrv_PQ_LoadScalingTable(eWindow,
                                            eXRuleType,
                                            i,
                                            bPreV_ScalingDown,
                                            bInterlace,
                                            bColorSpaceYUV,
                                            u16InputSize,
                                            u16SizeAfterScaling);
        bRet |= (bSDMode << (1 * i));
    }

    if(eXRuleType == E_XRULE_VSD)
    {
        bRet &= 0x01;
    }
#if 0
    //121 filter
    if(((MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_LINE_ALTERNATIVE) ||
        (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_TOP_BOTTOM)) &&
        (MApi_XC_Get_3D_HW_Version() > 0))
    {
        if((u16InputSize >= u16SizeAfterScaling) && (eXRuleType == E_XRULE_VSP))
        {
            //if post V down, we will load 121 filter, and when it's post scaling case
            //reload 121 filter's VSP_Y=SRAM_1_4Tap, VSP_C=C_SRAM_1,
            //                    SRAM1=InvSinc4Tc4p4Fc50Apass01Astop55, C_SRAM1=C121 for main-win
            //reload 121 filter's VSP_Y=SRAM_3_4Tap, VSP_C=C_SRAM_3,
            //                    SRAM3=InvSinc4Tc4p4Fc50Apass01Astop55, C_SRAM3=C121 for sub-win
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VSP_Y_SRAM_1_4Tap_Main, PQ_IP_VSP_Y_Main);
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VSP_C_C_SRAM_1_Main, PQ_IP_VSP_C_Main);
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_SRAM1_InvSinc4Tc4p4Fc50Apass01Astop55_Main, PQ_IP_SRAM1_Main);
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_C_SRAM1_C121_Main, PQ_IP_C_SRAM1_Main);

            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_IP_VSP_Y_SRAM_3_4Tap_Sub, PQ_IP_VSP_Y_Sub);
            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_IP_VSP_C_C_SRAM_3_Sub, PQ_IP_VSP_C_Sub);
            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_IP_SRAM3_InvSinc4Tc4p4Fc50Apass01Astop55_Sub, PQ_IP_SRAM3_Sub);
            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_IP_C_SRAM3_C121_Sub, PQ_IP_C_SRAM3_Sub);
        }
    }
#endif

    return bRet;
}


//////////////////////////////////////////////////////////////////
// set color range of input source and take effect immediately
//
void MDrv_PQ_SetColorRange(PQ_WIN eWindow, MS_BOOL bColorRange0_255)
{
    _bColorRange0_255[PQ_MAIN_WINDOW] = bColorRange0_255;
    _bColorRange0_255[PQ_SUB_WINDOW] = bColorRange0_255;
    MDrv_PQ_SetCSC(eWindow, _gFourceColorFmt[eWindow]);
}

MS_BOOL MDrv_PQ_Get_CSC_XRuleIdx(MS_BOOL *pbInputColorSpaceRGB,
                                            MS_U16 * pu16DoCSC,
                                            PQ_WIN eWindow,
                                            PQ_FOURCE_COLOR_FMT enFourceColor,
                                            MS_BOOL bLoadPQTable)
{
    MS_U16 u16XRuleIdx, u16XRuleIP;
    MS_U16 u16IPIdx, u16TabIdx;
    MS_U16 eXRuleType = E_XRULE_CSC;
    MS_BOOL bInputTypeVideo;
    MS_BOOL bInputResolutionHD = _gIsSrcHDMode[eWindow];

    _gFourceColorFmt[eWindow] = enFourceColor;

#if ENABLE_VGA_EIA_TIMING
    if ((QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
        QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow], eWindow)) && !QM_IsInterlaced(eWindow))
#else
    if(QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
       (QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow], eWindow) && !QM_IsInterlaced(eWindow)))
#endif
    {
        CSCRULE_DBG(printf("PC mode\n"));
        bInputTypeVideo = FALSE;

#if (PQ_ENABLE_PIP == 1)
        if(eWindow == PQ_SUB_WINDOW)
        {
            CSCRULE_DBG(printf("[%s][%d] VGA in sub video case, force PC mode to video mode \n", __FUNCTION__, __LINE__));
            bInputTypeVideo = TRUE;
        }
#endif
    }
    else
    {
        CSCRULE_DBG(printf("VIDEO mode\n"));
        bInputTypeVideo = TRUE;
    }
/*$$$
    if(_bDualViewEnable == TRUE)
    {
        CSCRULE_DBG(printf("DualView mode\n"));
        bInputTypeVideo = TRUE;
    }
    else if(MDrv_PQ_Get_PointToPoint(eWindow))
    {
        CSCRULE_DBG(printf("PointToPoint mode\n"));
        bInputTypeVideo = FALSE;
    }
*/
    if( QM_IsSourceVGA(_enInputSourceType[eWindow]) ||
       (QM_IsSourceDVI_HDMIPC(_enInputSourceType[eWindow],eWindow)) ||
       (QM_IsSourceHDMI_Video(_enInputSourceType[eWindow],eWindow) && (QM_HDMIPC_COLORRGB(eWindow))) )
    {
        CSCRULE_DBG(printf("Input RGB\n"));
        (*pbInputColorSpaceRGB) = TRUE;
    }
    else
    {
        CSCRULE_DBG(printf("Input YUV\n"));
        (*pbInputColorSpaceRGB) = FALSE;
    }

    CSCRULE_DBG(printf("VIDEO mode=%u, FourColor=%u, HD%u, InputRGB=%u, _bColorRange0_255=%u\n", bInputTypeVideo, enFourceColor, bInputResolutionHD, *pbInputColorSpaceRGB, _bColorRange0_255[eWindow]));
    if (bInputTypeVideo)
    {
        if (*pbInputColorSpaceRGB)
        {
            if (eWindow == PQ_MAIN_WINDOW)
            {
                if (bInputResolutionHD == FALSE)
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Main : // fource to RGB and input is RGB, hence no need change
                                 PQ_CSCRule_Video_RGB_SD_0_255_Main;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Main :
                                 PQ_CSCRule_Video_RGB_SD_16_235_Main;
                    }
                }
                else // InputResolutionHD
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Main :
                                 PQ_CSCRule_Video_RGB_HD_0_255_Main;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Main :
                                 PQ_CSCRule_Video_RGB_HD_16_235_Main;
                    }
                }
            }
#if PQ_ENABLE_PIP
            else
            {
                if (bInputResolutionHD == FALSE)
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Sub : // fource to RGB and input is RGB, hence no need change
                                 PQ_CSCRule_Video_RGB_SD_0_255_Sub;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_SD_Sub :
                                 PQ_CSCRule_Video_RGB_SD_16_235_Sub;
                    }
                }
                else // InputResolutionHD
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Sub :
                                 PQ_CSCRule_Video_RGB_HD_0_255_Sub;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_Video_YUV_HD_Sub :
                                 PQ_CSCRule_Video_RGB_HD_16_235_Sub;
                    }
                }
            }
#endif
        }
        else // InputColorSpaceYUV
        {
            if(enFourceColor == PQ_FOURCE_COLOR_RGB )
            {
                MS_ASSERT(0);
                PQTAB_DBG(printf("InputColorSpace is YUV, Fource Color Space is RGB!!!\n"));
            }

            if (eWindow == PQ_MAIN_WINDOW)
            {
                if (bInputResolutionHD == FALSE)
                {
                    u16XRuleIdx = PQ_CSCRule_Video_YUV_SD_Main;
                }
                else // InputResolutionHD
                {
                    u16XRuleIdx = PQ_CSCRule_Video_YUV_HD_Main;
                }
            }
#if PQ_ENABLE_PIP
            else
            {
                if (bInputResolutionHD == FALSE)
                {
                    u16XRuleIdx = PQ_CSCRule_Video_YUV_SD_Sub;
                }
                else // InputResolutionHD
                {
                    u16XRuleIdx = PQ_CSCRule_Video_YUV_HD_Sub;
                }
            }
#endif
        }
    }
    else // InputTypePC
    {
        if (*pbInputColorSpaceRGB)
        {
            if (eWindow == PQ_MAIN_WINDOW)
            {
                if (bInputResolutionHD == FALSE)
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Main :
                                 PQ_CSCRule_PC_RGB_SD_0_255_Main;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Main :
                                 PQ_CSCRule_PC_RGB_SD_16_235_Main;
                    }
                }
                else // InputResolutionHD
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Main :
                                 PQ_CSCRule_PC_RGB_HD_0_255_Main;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Main :
                                 PQ_CSCRule_PC_RGB_HD_16_235_Main;
                    }
                }
            }
#if PQ_ENABLE_PIP
            else
            {
                if (bInputResolutionHD == FALSE)
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Sub :
                                 PQ_CSCRule_PC_RGB_SD_0_255_Sub;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_SD_Sub :
                                 PQ_CSCRule_PC_RGB_SD_16_235_Sub;
                    }
                }
                else // InputResolutionHD
                {
                    if (_bColorRange0_255[eWindow])
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Sub :
                                 PQ_CSCRule_PC_RGB_HD_0_255_Sub;
                    }
                    else // bInputDataRange16_235
                    {
                        u16XRuleIdx = enFourceColor == PQ_FOURCE_COLOR_RGB ?
                                 PQ_CSCRule_PC_YUV_HD_Sub :
                                 PQ_CSCRule_PC_RGB_HD_16_235_Sub;
                    }
                }
            }
#endif
        }
        else // InputColorSpaceYUV
        {
            if(enFourceColor == PQ_FOURCE_COLOR_RGB )
            {
                MS_ASSERT(0);
                PQTAB_DBG(printf("InputColorSpace is YUV, Fource Color Space is RGB!!!\n"));
            }

            if(eWindow == PQ_MAIN_WINDOW)
            {
                if (bInputResolutionHD == FALSE)
                {
                    u16XRuleIdx = PQ_CSCRule_PC_YUV_SD_Main;
                }
                else // InputResolutionHD
                {
                    u16XRuleIdx = PQ_CSCRule_PC_YUV_HD_Main;
                }
            }
#if PQ_ENABLE_PIP
            else
            {
                if (bInputResolutionHD == FALSE)
                {
                    u16XRuleIdx = PQ_CSCRule_PC_YUV_SD_Sub;
                }
                else // InputResolutionHD
                {
                    u16XRuleIdx = PQ_CSCRule_PC_YUV_HD_Sub;
                }
            }
#endif
        }
    }

    for(u16XRuleIP=0; u16XRuleIP<MDrv_PQ_GetXRuleIPNum(eWindow, eXRuleType); u16XRuleIP++)
    {
        u16IPIdx = MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u16XRuleIP);
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, eXRuleType, u16XRuleIdx, u16XRuleIP);
        CSCRULE_DBG(printf("u16XRuleIdx:%u, u16XRuleIP=%u, IPIdx=%u, TabIdx=%u\n", u16XRuleIdx, u16XRuleIP, u16IPIdx, u16TabIdx));

        if(bLoadPQTable)
        {
            MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
        }

        if(u16TabIdx)
        {
            if(u16IPIdx == Hal_PQ_get_csc_ip_idx(MS_CSC_IP_CSC))
            {
                (*pu16DoCSC) |= 0x1;
            }
            else if(u16IPIdx == Hal_PQ_get_csc_ip_idx(MS_CSC_IP_VIP_CSC))
            {
                (*pu16DoCSC) |= 0x02;
            }
            else
            {
                (*pu16DoCSC) |= 0x00;
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////
// get memory color format
//
MS_BOOL MDrv_PQ_Get_MemYUVFmt(PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enFourceColor)
{
    MS_U16 u16DoCSC = 0;
    MS_BOOL bInputColorSpaceRGB;
    MS_BOOL bMemYUV;

    MDrv_PQ_Get_CSC_XRuleIdx(&bInputColorSpaceRGB, &u16DoCSC, eWindow, enFourceColor, FALSE);

    if(bInputColorSpaceRGB)
    {
        if(u16DoCSC & 0x01)
        {
            bMemYUV = TRUE;
        }
        else
        {
            bMemYUV = FALSE;
        }
    }
    else
    {
        bMemYUV = TRUE;
    }

    return bMemYUV;
}


//////////////////////////////////////////////////////////////////
// do color space conversion from RGB to YUV according to input sourcce and context
// return:
//        TRUE: use YUV color space
//        FALSE: otherwise
//
MS_BOOL MDrv_PQ_SetCSC(PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enFourceColor)
{
    MS_U16 u16DoCSC = 0;
    MS_BOOL bInputColorSpaceRGB;


    MDrv_PQ_Get_CSC_XRuleIdx(&bInputColorSpaceRGB, &u16DoCSC, eWindow, enFourceColor, TRUE);

    u16DoCSC = (u16DoCSC != 0);
    if ((bInputColorSpaceRGB && u16DoCSC) || (!bInputColorSpaceRGB))
    {
        return TRUE;    // use YUV space
    }
    else
    {
        return FALSE;   // use RGB space
    }
}

static void _MDrv_PQ_LoadNRTable(PQ_WIN eWindow, MS_U16 u16PQ_NRIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;

    if (eWindow == PQ_MAIN_WINDOW)
    {
        if(u16PQ_NRIdx > PQ_GRULE_NR_NUM_Main)
        {
            MS_ASSERT(0);
            return;
        }

        for(i = 0; i < PQ_GRULE_NR_IP_NUM_Main; i++)
        {
            u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_NR_Main, i);

            if(u16PQ_NRIdx == PQ_GRULE_NR_NUM_Main)  // see it as default
                u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
            else
                u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_NR_Main, u16PQ_NRIdx, i);

            PQGRULE_DBG(printf("[NR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_NRIdx, i, u16IPIdx, u16TabIdx));
            MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
        }

#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
        if(_bCustomerMainPQEn)
        {
            if(u16PQ_NRIdx > PQ_GRULE_NR_NUM_Main_Color)
            {
                MS_ASSERT(0);
                return;
            }

            for(i = 0; i < PQ_GRULE_NR_IP_NUM_Main_Color; i++)
            {
                u16IPIdx = MDrv_PQ_GetCustomerGRule_IPIndex(eWindow, PQ_GRule_NR_Main_Color, i);

                if(u16PQ_NRIdx == PQ_GRULE_NR_NUM_Main_Color)  // see it as default
                    u16TabIdx = MDrv_PQ_GetCustomerTableIndex(eWindow, u16IPIdx);
                else
                    u16TabIdx = MDrv_PQ_GetGRule_CustomerTableIndex(eWindow, PQ_GRule_NR_Main_Color, u16PQ_NRIdx, i);

                MDrv_PQ_LoadCustomerTable(eWindow, u16TabIdx, u16IPIdx);
            }
        }
#endif

    }
#if PQ_ENABLE_PIP
    else if (eWindow == PQ_SUB_WINDOW)
    {
        if(u16PQ_NRIdx > PQ_GRULE_NR_NUM_Sub)
        {
            MS_ASSERT(0);
            return;
        }

        for(i = 0; i < PQ_GRULE_NR_IP_NUM_Sub; i++)
        {
            u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_NR_Sub, i);

            if(u16PQ_NRIdx == PQ_GRULE_NR_NUM_Sub)  // see it as default
                u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
            else
                u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_NR_Sub, u16PQ_NRIdx, i);

            PQGRULE_DBG(printf("[NR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_NRIdx, i, u16IPIdx, u16TabIdx));
            MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
        }

#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
        if(_bCustomerSubPQEn)
        {
            if(u16PQ_NRIdx > PQ_GRULE_NR_NUM_Sub_Color)
            {
                MS_ASSERT(0);
                return;
            }

            for(i = 0; i < PQ_GRULE_NR_IP_NUM_Sub_Color; i++)
            {
                u16IPIdx = MDrv_PQ_GetCustomerGRule_IPIndex(eWindow, PQ_GRule_NR_Sub_Color, i);

                if(u16PQ_NRIdx == PQ_GRULE_NR_NUM_Sub_Color)  // see it as default
                    u16TabIdx = MDrv_PQ_GetCustomerTableIndex(eWindow, u16IPIdx);
                else
                    u16TabIdx = MDrv_PQ_GetGRule_CustomerTableIndex(eWindow, PQ_GRule_NR_Sub_Color, u16PQ_NRIdx, i);

                MDrv_PQ_LoadCustomerTable(eWindow, u16TabIdx, u16IPIdx);
            }
        }
#endif

    }
#endif
    else
    {
        PQGRULE_DBG(printf("[NR] Not support case \n"));
    }

}

void MDrv_PQ_LoadNRTable(PQ_WIN eWindow, PQ_3D_NR_FUNCTION_TYPE en3DNRType)
{
    PQGRULE_DBG(printf("[PQ_LoadNRTable] "));
    MS_U16 u16PQ_NRIdx;

    if (eWindow == PQ_MAIN_WINDOW)
    {
        if(en3DNRType == PQ_3D_NR_OFF)
        {
            PQGRULE_DBG(printf("Off\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Off_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Off_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO)
        {
            // Auto NR will be process in msAPI_DynamicNR_Handler(), hence we only need to pre-setup the PQ table.
            PQGRULE_DBG(printf("Auto\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_LOW)
        {
            PQGRULE_DBG(printf("Low\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_MID)
        {
            PQGRULE_DBG(printf("Mid\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Middle_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Middle_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_HIGH)
        {
            PQGRULE_DBG(printf("High\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_High_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_High_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
#if (PQ_ENABLE_MULTI_LEVEL_AUTO_NR == 1)
        else if(en3DNRType == PQ_3D_NR_AUTO_LOW_L)
        {
            PQGRULE_DBG(printf("AUTO_LOW_L\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_Low_L_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO_LOW_M)
        {
            PQGRULE_DBG(printf("AUTO_LOW_M\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_Low_M_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO_LOW_H)
        {
            PQGRULE_DBG(printf("AUTO_LOW_H\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_Low_H_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO_MID_L)
        {
            PQGRULE_DBG(printf("AUTO_MID_L\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Middle_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_Middle_L_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO_MID_M)
        {
            PQGRULE_DBG(printf("AUTO_MID_M\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Middle_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_Middle_M_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO_MID_H)
        {
            PQGRULE_DBG(printf("AUTO_MID_H\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Middle_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_Middle_H_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO_HIGH_L)
        {
            PQGRULE_DBG(printf("AUTO_HIGH_L\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_High_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_High_L_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO_HIGH_M)
        {
            PQGRULE_DBG(printf("AUTO_HIGH_M\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_High_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_High_M_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO_HIGH_H)
        {
            PQGRULE_DBG(printf("AUTO_HIGH_H\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_High_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Auto_High_H_Main);
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
#endif
        else if(en3DNRType == PQ_3D_NR_DEFAULT)
        {
            PQGRULE_DBG(printf("Default\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Main, PQ_GRule_NR_Low_3D_Main);
            }
            else
#endif
            {
                u16PQ_NRIdx = PQ_GRULE_NR_NUM_Main;
            }
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else
        {
            MS_ASSERT(0);
        }
    }
#if PQ_ENABLE_PIP
    else if (eWindow == PQ_SUB_WINDOW)
    {
        if(en3DNRType == PQ_3D_NR_OFF)
        {
            PQGRULE_DBG(printf("Off\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Sub, PQ_GRule_NR_Off_Sub);
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_AUTO)
        {
            // Auto NR will be process in msAPI_DynamicNR_Handler(), hence we only need to pre-setup the PQ table.
            PQGRULE_DBG(printf("Auto\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Sub, PQ_GRule_NR_Low_Sub);
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_LOW)
        {
            PQGRULE_DBG(printf("Low\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Sub, PQ_GRule_NR_Low_Sub);
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_MID)
        {
            PQGRULE_DBG(printf("Mid\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Sub, PQ_GRule_NR_Middle_Sub);
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_HIGH)
        {
            PQGRULE_DBG(printf("High\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_NR_Sub, PQ_GRule_NR_High_Sub);
            _MDrv_PQ_LoadNRTable(eWindow, u16PQ_NRIdx);
        }
        else if(en3DNRType == PQ_3D_NR_DEFAULT)
        {
            PQGRULE_DBG(printf("Default\n"));
            _MDrv_PQ_LoadNRTable(eWindow, PQ_GRULE_NR_NUM_Sub);
        }
        else
        {
            MS_ASSERT(0);
        }
    }
#endif
    else
    {
        PQGRULE_DBG(printf("Not support case \n"));
    }

#if 0 //trunk don't have janus code, it's obsolete
#ifdef __AEONR2__ //patch for DDR2-800 on Janus
    if(QM_IsInterlaced(eWindow) && QM_IsSourceMultiMedia(MDrv_PQ_GetInputSourceType(eWindow)) && (MApi_XC_ReadByte(0x110D30)==0x66))
    {
        MS_U16 u16Input_HSize;
        MS_U16 u16Input_VSize;
        u16Input_HSize = _stMode_Info[eWindow].u16input_hsize;
        u16Input_VSize = _stMode_Info[eWindow].u16input_vsize;

        if(u16Input_HSize>1280 && u16Input_VSize>=900)
        {
            MApi_XC_WriteByte(0x102F00, 0x12);
            MApi_XC_WriteByte(0x102F03, 0x27);
            MApi_XC_WriteByte(0x102F00, 0x06);
            MApi_XC_WriteByte(0x102F42, MApi_XC_ReadByte(0x102F42)&0xFE);
        }
    }
#endif
#endif
}

#if PQ_GRULE_OSD_BW_ENABLE
static void _MDrv_PQ_LoadOSD_BWTable(PQ_WIN eWindow, MS_U16 u16PQ_OSD_BW_Idx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;

    if(u16PQ_OSD_BW_Idx > PQ_GRULE_OSD_BW_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_OSD_BW_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_OSD_BW_Main, i);

        if(u16PQ_OSD_BW_Idx == PQ_GRULE_OSD_BW_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSD_BW_Idx, i);

        PQGRULE_DBG(printf("[NR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_OSD_BW_Idx, i, u16IPIdx, u16IPIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}


void MDrv_PQ_LoadOSD_BWTable(PQ_WIN eWindow, PQ_OSD_BW_FUNCTION_TYPE enOSD_BW_Type)
{
    PQGRULE_DBG(printf("[PQ_Load_OSD_BW_Table] "));
    MS_U16 u16PQ_NRIdx;

    if(enOSD_BW_Type == PQ_OSD_BW_ON)
    {
        PQGRULE_DBG(printf("On\n"));

        #if PQ_GRULE_DDR_SELECT_ENABLE
        if(bw_info.u32Miu1MemSize == 0)
       {
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_1DDR_On_Main);
            _MDrv_PQ_LoadOSD_BWTable(eWindow, u16PQ_NRIdx);
        }
        else
        {
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_2DDR_On_Main);
            _MDrv_PQ_LoadOSD_BWTable(eWindow, u16PQ_NRIdx);
        }
        #else
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_Main);
        _MDrv_PQ_LoadOSD_BWTable(eWindow, u16PQ_NRIdx);
       #endif
    }
    else
    {
        MS_ASSERT(0);
    }
}
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
static void _MDrv_PQ_LoadFilmModeTable(PQ_WIN eWindow, MS_U16 u16PQ_FilmModeIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_FilmModeIdx > PQ_GRULE_FILM_MODE_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_FILM_MODE_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_FILM_MODE_Main, i);
        EN_IP_Info  sFilmModeTbl;
        //Dump common table first
        if (eWindow == PQ_MAIN_WINDOW)
            sFilmModeTbl = MDrv_PQ_GetTable_(MAIN, PQ_IP_COMM, u16IPIdx);
        else
            sFilmModeTbl = MDrv_PQ_GetTable_(SUB, PQ_IP_COMM, u16IPIdx);

        if (eWindow == PQ_MAIN_WINDOW)
            MDrv_PQ_DumpTable_(MAIN, &sFilmModeTbl);
        else
            MDrv_PQ_DumpTable_(SUB, &sFilmModeTbl);

        if(u16PQ_FilmModeIdx == PQ_GRULE_FILM_MODE_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_FILM_MODE_Main, u16PQ_FilmModeIdx, i);

        PQGRULE_DBG(printf("[FM]SRC: %u, FM: %u, FMIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_FilmModeIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif


void MDrv_PQ_LoadFilmModeTable(PQ_WIN eWindow, PQ_FILM_MODE_FUNCTION_TYPE enFilmModeType)
{
#if PQ_GRULE_FILM_MODE_ENABLE
    MS_U16 u16PQ_FilmModeIdx;

    if(enFilmModeType == PQ_FilmMode_OFF)
    {
        u16PQ_FilmModeIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_FILM_MODE_Main, PQ_GRule_FILM_MODE_Off_Main);
        _MDrv_PQ_LoadFilmModeTable(eWindow, u16PQ_FilmModeIdx);

    }
    else if(enFilmModeType == PQ_FilmMode_ON)
    {
        u16PQ_FilmModeIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_FILM_MODE_Main, PQ_GRule_FILM_MODE_On_Main);
        _MDrv_PQ_LoadFilmModeTable(eWindow, u16PQ_FilmModeIdx);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enFilmModeType);
#endif
}

#if PQ_GRULE_SR_ENABLE
static void _MDrv_PQ_LoadSRTable(PQ_WIN eWindow, MS_U16 u16PQ_SRIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;


    if(u16PQ_SRIdx > ((eWindow == PQ_MAIN_WINDOW)?PQ_GRULE_SR_NUM_Main: PQ_GRULE_SR_NUM_Sub))
    {
        PQGRULE_DBG(printf("[GRULE_SR] Wrong SR index...\n"));
        return;
    }

    for(i = 0; i < ((eWindow == PQ_MAIN_WINDOW)?PQ_GRULE_SR_IP_NUM_Main : PQ_GRULE_SR_IP_NUM_Sub); i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, eWindow == PQ_MAIN_WINDOW?PQ_GRule_SR_Main: PQ_GRule_SR_Sub, i);

#if (PQ_ONLY_SUPPORT_BIN == 0)
        EN_IP_Info  sSrTbl;
        //Dump common table first
        if (eWindow == PQ_MAIN_WINDOW)
            sSrTbl = MDrv_PQ_GetTable_(MAIN, PQ_IP_COMM, u16IPIdx);
        else
            sSrTbl = MDrv_PQ_GetTable_(SUB, PQ_IP_COMM, u16IPIdx);

        if (eWindow == PQ_MAIN_WINDOW)
            MDrv_PQ_DumpTable_(MAIN, &sSrTbl);
        else
            MDrv_PQ_DumpTable_(SUB, &sSrTbl);
#else
        MS_PQBin_IP_Table_Info stTableInfo;
        BYTE bin_id;
        if (eWindow == PQ_MAIN_WINDOW)
            bin_id = 0;
        else
            bin_id = 1;
        MDrv_PQBin_GetTable(MDrv_PQBin_GetPanelIdx(), PQ_BIN_IP_COMM, u16IPIdx, &stTableInfo, &stPQBinHeaderInfo[bin_id]);
        MDrv_PQBin_DumpTable(&stTableInfo);
#endif
        if(u16PQ_SRIdx == PQ_GRULE_SR_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16TabIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, (eWindow == PQ_MAIN_WINDOW?PQ_GRule_SR_Main : PQ_GRule_SR_Sub), u16PQ_SRIdx, i);

        PQGRULE_DBG(printf("[SR]SRC: %u, SR: %u, SRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_SRIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }

}
#endif

void MDrv_PQ_LoadSRTable(PQ_WIN eWindow, PQ_SR_FUNCTION_TYPE enSRType)
{
#if PQ_GRULE_SR_ENABLE
    MS_U16 u16PQ_SRIdx;

    u16PQ_SRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow,eWindow == PQ_MAIN_WINDOW? PQ_GRule_SR_Main : PQ_GRule_SR_Sub, enSRType);
        _MDrv_PQ_LoadSRTable(eWindow, u16PQ_SRIdx);

#else
    UNUSED(eWindow);
    UNUSED(enSRType);
#endif

}

#if PQ_GRULE_BYPASS_NR_ENABLE
static void _MDrv_PQ_LoadBypassNRTable(PQ_WIN eWindow, MS_U16 u16PQ_BypassIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_BypassIdx > PQ_GRULE_Bypass_NR_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_Bypass_NR_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_Bypass_NR_Main, i);

        if(u16PQ_BypassIdx == PQ_GRULE_Bypass_NR_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16TabIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_Bypass_NR_Main, u16PQ_BypassIdx, i);

        PQGRULE_DBG(printf("[SR]SRC: %u, SR: %u, SRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_BypassIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadBypassNRTable(PQ_WIN eWindow, PQ_BYPASS_FUNCTION_TYPE enBypassOff)
{
#if PQ_GRULE_BYPASS_NR_ENABLE
    MS_U16 u16PQ_BypassNrIdx;

    if(enBypassOff == PQ_BYPASS_ON)
    {
        u16PQ_BypassNrIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_Bypass_NR_Main, PQ_GRule_Bypass_NR_Off_Main);
        _MDrv_PQ_LoadBypassNRTable(eWindow, u16PQ_BypassNrIdx);

    }
    else if(enBypassOff == PQ_BYPASS_OFF)
    {
        u16PQ_BypassNrIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_Bypass_NR_Main, PQ_GRule_Bypass_NR_On_Main);
        _MDrv_PQ_LoadBypassNRTable(eWindow, u16PQ_BypassNrIdx);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enBypassOff);
#endif
}

#if PQ_GRULE_BYPASS_DI_ENABLE
static void _MDrv_PQ_LoadBypassDITable(PQ_WIN eWindow, MS_U16 u16PQ_BypassIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_BypassIdx > PQ_GRULE_Bypass_DI_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_Bypass_DI_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_Bypass_DI_Main, i);

        if(u16PQ_BypassIdx == PQ_GRULE_Bypass_DI_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16TabIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_Bypass_DI_Main, u16PQ_BypassIdx, i);

        PQGRULE_DBG(printf("[SR]SRC: %u, SR: %u, SRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_BypassIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadBypassDiTable(PQ_WIN eWindow, PQ_BYPASS_FUNCTION_TYPE enBypassOff)
{
#if PQ_GRULE_BYPASS_DI_ENABLE
    MS_U16 u16PQ_BypassDiIdx;

    if(enBypassOff == PQ_BYPASS_ON)
    {
        u16PQ_BypassDiIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_Bypass_DI_Main, PQ_GRule_Bypass_DI_Off_Main);
        _MDrv_PQ_LoadBypassDITable(eWindow, u16PQ_BypassDiIdx);

    }
    else if(enBypassOff == PQ_BYPASS_OFF)
    {
        u16PQ_BypassDiIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_Bypass_DI_Main, PQ_GRule_Bypass_DI_On_Main);
        _MDrv_PQ_LoadBypassDITable(eWindow, u16PQ_BypassDiIdx);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enBypassOff);
#endif
}

#if PQ_GRULE_BYPASS_PEAKING_ENABLE
static void _MDrv_PQ_LoadBypassPeakingTable(PQ_WIN eWindow, MS_U16 u16PQ_BypassIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_BypassIdx > PQ_GRULE_Bypass_Peaking_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_Bypass_Peaking_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_Bypass_Peaking_Main, i);

        if(u16PQ_BypassIdx == PQ_GRULE_Bypass_Peaking_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16TabIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_Bypass_Peaking_Main, u16PQ_BypassIdx, i);

        PQGRULE_DBG(printf("[SR]SRC: %u, SR: %u, SRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_BypassIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadBypassPeakingTable(PQ_WIN eWindow, PQ_BYPASS_FUNCTION_TYPE enBypassOff)
{
#if PQ_GRULE_BYPASS_PEAKING_ENABLE
    MS_U16 u16PQ_BypassPeakingIdx;

    if(enBypassOff == PQ_BYPASS_ON)
    {
        u16PQ_BypassPeakingIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_Bypass_Peaking_Main, PQ_GRule_Bypass_Peaking_Off_Main);
        _MDrv_PQ_LoadBypassPeakingTable(eWindow, u16PQ_BypassPeakingIdx);

    }
    else if(enBypassOff == PQ_BYPASS_OFF)
    {
        u16PQ_BypassPeakingIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_Bypass_Peaking_Main, PQ_GRule_Bypass_Peaking_On_Main);
        _MDrv_PQ_LoadBypassPeakingTable(eWindow, u16PQ_BypassPeakingIdx);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enBypassOff);
#endif
}

#if PQ_GRULE_BYPASS_OTHERS_ENABLE
static void _MDrv_PQ_LoadBypassOthersTable(PQ_WIN eWindow, MS_U16 u16PQ_BypassIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_BypassIdx > PQ_GRULE_Bypass_Others_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_Bypass_Others_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_Bypass_Others_Main, i);

        if(u16PQ_BypassIdx == PQ_GRULE_Bypass_Others_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16TabIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_Bypass_Others_Main, u16PQ_BypassIdx, i);

        PQGRULE_DBG(printf("[SR]SRC: %u, SR: %u, SRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_BypassIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadBypassOthersTable(PQ_WIN eWindow, PQ_BYPASS_FUNCTION_TYPE enBypassOff)
{
#if PQ_GRULE_BYPASS_OTHERS_ENABLE
    MS_U16 u16PQ_BypassOthersIdx;

    if(enBypassOff == PQ_BYPASS_ON)
    {
        u16PQ_BypassOthersIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_Bypass_Others_Main, PQ_GRule_Bypass_Others_Off_Main);
        _MDrv_PQ_LoadBypassOthersTable(eWindow, u16PQ_BypassOthersIdx);

    }
    else if(enBypassOff == PQ_BYPASS_OFF)
    {
        u16PQ_BypassOthersIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_Bypass_Others_Main, PQ_GRule_Bypass_Others_On_Main);
        _MDrv_PQ_LoadBypassOthersTable(eWindow, u16PQ_BypassOthersIdx);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enBypassOff);
#endif
}

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
static void _MDrv_PQ_DynamicContrastTable(PQ_WIN eWindow, MS_U16 u16PQ_DynContrIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_DynContrIdx > PQ_GRULE_DYNAMIC_CONTRAST_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_DYNAMIC_CONTRAST_Main, i);

        if(u16PQ_DynContrIdx == PQ_GRULE_DYNAMIC_CONTRAST_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16TabIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DYNAMIC_CONTRAST_Main, u16PQ_DynContrIdx, i);

        PQGRULE_DBG(printf("[DC]SRC: %u, DC: %u, DCIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_DynContrIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadDynamicContrastTable(PQ_WIN eWindow, PQ_DYNAMIC_CONTRAST_FUNCTION_TYPE enDynamicContrastType)
{
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
    MS_U16 u16PQ_DynContrIdx;

    if(enDynamicContrastType == PQ_DynContr_OFF)
    {
        u16PQ_DynContrIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_DYNAMIC_CONTRAST_Main, PQ_GRule_DYNAMIC_CONTRAST_Off_Main);
        _MDrv_PQ_DynamicContrastTable(eWindow, u16PQ_DynContrIdx);
    }
    else if(enDynamicContrastType == PQ_DynContr_ON)
    {
        u16PQ_DynContrIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_DYNAMIC_CONTRAST_Main, PQ_GRule_DYNAMIC_CONTRAST_On_Main);
        _MDrv_PQ_DynamicContrastTable(eWindow, u16PQ_DynContrIdx);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enDynamicContrastType);
#endif
}

static MS_BOOL _MDrv_PQ_SetGRuleStatus(PQ_WIN eWindow, MS_U16 u16GruleType, MS_U16 u16PQ_GroupIdx)
{
#if(ENABLE_PQ_BIN)

    PQ_BIN_TYPE enPQBinTypeMain = PQ_BIN_STD_MAIN;
#if (PQ_ENABLE_PIP)
    PQ_BIN_TYPE enPQBinTypeSub = PQ_BIN_STD_SUB;
#endif

    if(gbPQBinEnable)
    {
        MS_U16 i, u16IPIdx = 0, u16TabIdx = 0, u16GroupNum = 0, u16IPNum = 0;

        if(eWindow == PQ_MAIN_WINDOW)
        {
            if(u16GruleType >= stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum)
            {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                    __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum);
                u16GruleType = u16GruleType - stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum;
                enPQBinTypeMain = PQ_BIN_CUSTOMER_MAIN;
                if(u16GruleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum)
                {
                    printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum);
                    return FALSE;
                }
#else
                printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum);
                return FALSE;
#endif
            }
            u16GroupNum = MDrv_PQBin_GetGRule_GroupNum(u16GruleType, &stPQBinHeaderInfo[enPQBinTypeMain]);
            u16IPNum = MDrv_PQBin_GetGRule_IPNum(u16GruleType, &stPQBinHeaderInfo[enPQBinTypeMain]);
        }
        #if (PQ_ENABLE_PIP)
        else if(eWindow == PQ_SUB_WINDOW)
        {
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
                printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                    __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum);
                u16GruleType = u16GruleType - stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum;
                enPQBinTypeSub = PQ_BIN_CUSTOMER_SUB;
                if(u16GruleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum)
                {
                    printf("CustomerPQ: ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum);
                    return 0;
                }
#else
                printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GruleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum);
                return FALSE;
#endif
            u16GroupNum = MDrv_PQBin_GetGRule_GroupNum(u16GruleType, &stPQBinHeaderInfo[enPQBinTypeSub]);
            u16IPNum = MDrv_PQBin_GetGRule_IPNum(u16GruleType, &stPQBinHeaderInfo[enPQBinTypeSub]);
        }
        #endif

        if ((PQ_BIN_IP_NULL == u16GroupNum) || (PQ_BIN_IP_NULL == u16IPNum))
        {
            return FALSE;
        }

        if(u16PQ_GroupIdx >= u16GroupNum)
        {
            printf("=ERROR!!!=%s, %d, u16PQ_GroupIdx:%d, out of range:%d=\n", __FUNCTION__, __LINE__, u16PQ_GroupIdx, u16GroupNum);
            return FALSE;
        }

#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE || ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
        if((enPQBinTypeMain == PQ_BIN_CUSTOMER_MAIN) || (enPQBinTypeSub == PQ_BIN_CUSTOMER_SUB))
        {
            for(i = 0; i < u16IPNum; i++)
            {
                u16IPIdx = MDrv_PQ_GetCustomerGRule_IPIndex(eWindow, u16GruleType, i);

                if(u16PQ_GroupIdx == u16GroupNum)  // see it as default
                {
                    u16TabIdx = MDrv_PQ_GetCustomerTableIndex(eWindow, u16TabIdx);
                }
                else
                {
                    u16TabIdx = MDrv_PQ_GetGRule_CustomerTableIndex(eWindow, u16GruleType, u16PQ_GroupIdx, i);
                }
                PQGRULE_DBG(printf("[DC]SRC: %u, DC: %u, DCIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                                   _u16PQSrcType[eWindow], u16PQ_GroupIdx, i, u16IPIdx, u16TabIdx));
                MDrv_PQ_LoadCustomerTable(eWindow, u16TabIdx, u16IPIdx);
            }
        }
        else
#endif
        {
        for(i = 0; i < u16IPNum; i++)
        {
            u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, u16GruleType, i);

            if(u16PQ_GroupIdx == u16GroupNum)  // see it as default
                u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16TabIdx);
            else
                u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, u16GruleType, u16PQ_GroupIdx, i);

            PQGRULE_DBG(printf("[DC]SRC: %u, DC: %u, DCIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                               _u16PQSrcType[eWindow], u16PQ_GroupIdx, i, u16IPIdx, u16TabIdx));
            MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
        }
    }
    }
    return TRUE;
#else
    UNUSED(eWindow);
    UNUSED(u16GruleType);
    UNUSED(u16PQ_GroupIdx);

    return FALSE;
#endif
}

MS_BOOL MDrv_PQ_SetGRuleStatus(PQ_WIN eWindow, MS_U16 u16GruleType, MS_U16 u16GruleStatus)
{
    MS_U16 u16PQ_CurLvlIdx = 0;
    u16PQ_CurLvlIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, u16GruleType, u16GruleStatus);

    return _MDrv_PQ_SetGRuleStatus(eWindow, u16GruleType, u16PQ_CurLvlIdx);
}

MS_U16 MDrv_PQ_GetGRule_LvlNum(PQ_WIN eWindow, MS_U16 u16GRuleType)
{
    MS_U16 u16Ret = 0;

#if(ENABLE_PQ_BIN)

    PQ_BIN_TYPE enPQBinTypeMain = PQ_BIN_STD_MAIN;
#if (PQ_ENABLE_PIP)
    PQ_BIN_TYPE enPQBinTypeSub = PQ_BIN_STD_SUB;
#endif

    if(gbPQBinEnable)
    {
        if(eWindow == PQ_MAIN_WINDOW)
        {
            if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum)
            {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                PQGRULE_DBG(printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                    __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum));
                u16GRuleType = u16GRuleType - stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum;
                enPQBinTypeMain = PQ_BIN_CUSTOMER_MAIN;
                if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum)
                {
                    printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_MAIN].u16GRule_RuleNum);
                return 0;
            }
#else
                printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum);
                return 0;
#endif
            }

            u16Ret = MDrv_PQBin_GetGRule_LvlNum(MDrv_PQBin_GetPanelIdx(), u16GRuleType, &stPQBinHeaderInfo[enPQBinTypeMain]);
        }
#if (PQ_ENABLE_PIP)
        else if(eWindow == PQ_SUB_WINDOW)
        {
            if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum)
            {
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
                PQGRULE_DBG(printf("(%s, %d)GRuleType(%u) is bigger than STD PQ's(%u), so consider it as CUSTOMER PQ's GRule Type\n",
                    __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum));
                u16GRuleType = u16GRuleType - stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum;
                enPQBinTypeSub = PQ_BIN_CUSTOMER_SUB;
                if(u16GRuleType >= stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum)
            {
                    printf("CustomerPQ: =ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_CUSTOMER_SUB].u16GRule_RuleNum);
                    return 0;
                }
#else
                printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum);
                return 0;
#endif
            }

            u16Ret = MDrv_PQBin_GetGRule_LvlNum(MDrv_PQBin_GetPanelIdx(), u16GRuleType, &stPQBinHeaderInfo[enPQBinTypeSub]);
        }
#endif
        else
        {
            printf("=Error=%s, %d, =Not support case \n", __FUNCTION__, __LINE__);
        }

        if (PQ_BIN_IP_NULL == u16Ret)
        {
            return 0;
        }
    }
#else
    UNUSED(eWindow);
    UNUSED(u16GRuleType);
#endif

    return u16Ret;
}

MS_U16 MDrv_PQ_GetGRule_GRuleNum(PQ_WIN eWindow)
{
    MS_U16 u16Ret = 0;

#if(ENABLE_PQ_BIN)
    if(gbPQBinEnable)
    {
        if(eWindow == PQ_MAIN_WINDOW)
        {
            u16Ret = stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u16GRule_RuleNum;
        }
#if (PQ_ENABLE_PIP)
        else if(eWindow == PQ_SUB_WINDOW)
        {
            u16Ret = stPQBinHeaderInfo[PQ_BIN_STD_SUB].u16GRule_RuleNum;
        }
#endif
        else
        {
            printf("=Error=%s, %d, =Not support case \n", __FUNCTION__, __LINE__);
        }
    }
#else
    UNUSED(eWindow);
#endif

    return u16Ret;
}

#if PQ_GRULE_MPEG_NR_ENABLE
static void _MDrv_PQ_LoadMPEGNRTable(PQ_WIN eWindow, MS_U16 u16PQ_NRIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_NRIdx > PQ_GRULE_MPEG_NR_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_MPEG_NR_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_MPEG_NR_Main, i);

        if(u16PQ_NRIdx == PQ_GRULE_MPEG_NR_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_MPEG_NR_Main, u16PQ_NRIdx, i);

        PQGRULE_DBG(printf("[MPEGNR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_NRIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadMPEGNRTable(PQ_WIN eWindow, PQ_MPEG_NR_FUNCTION_TYPE enMPEGNRType)
{
#if(PQ_GRULE_MPEG_NR_ENABLE)
    PQGRULE_DBG(printf("[PQ_LoadMPEGNRTable] "));
    MS_U16 u16PQ_NRIdx;

    if(enMPEGNRType == PQ_MPEG_NR_OFF)
    {
        PQGRULE_DBG(printf("Off\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_Off_Main);
        _MDrv_PQ_LoadMPEGNRTable(eWindow, u16PQ_NRIdx);
    }
    else if( enMPEGNRType == PQ_MPEG_NR_AUTO )
    {
        // Auto NR will be process in msAPI_DynamicNR_Handler(), hence we only need to pre-setup the PQ table.
        PQGRULE_DBG(printf("Auto\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_Low_Main);
        _MDrv_PQ_LoadMPEGNRTable(eWindow, u16PQ_NRIdx);
    }
    else if(enMPEGNRType == PQ_MPEG_NR_LOW)
    {
        PQGRULE_DBG(printf("Low\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_Low_Main);
        _MDrv_PQ_LoadMPEGNRTable(eWindow, u16PQ_NRIdx);
    }
    else if(enMPEGNRType == PQ_MPEG_NR_MID)
    {
        PQGRULE_DBG(printf("Mid\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_Middle_Main);
        _MDrv_PQ_LoadMPEGNRTable(eWindow, u16PQ_NRIdx);
    }
    else if(enMPEGNRType == PQ_MPEG_NR_HIGH)
    {
        PQGRULE_DBG(printf("High\n"));
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_High_Main);
        _MDrv_PQ_LoadMPEGNRTable(eWindow, u16PQ_NRIdx);
    }
    else if(enMPEGNRType == PQ_MPEG_NR_DEFAULT)
    {
        PQGRULE_DBG(printf("Default\n"));
        _MDrv_PQ_LoadMPEGNRTable(eWindow, PQ_GRULE_MPEG_NR_NUM_Main);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enMPEGNRType);
#endif
}
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
static void _MDrv_PQ_Load_ULTRACLEAR_Table(PQ_WIN eWindow, MS_U16 u16PQ_ULTRACLEAR_Idx)
{
    MS_U16 i = 0, u16IPIdx = 0, u16TabIdx = 0;

    if(u16PQ_ULTRACLEAR_Idx > PQ_GRULE_ULTRAT_CLEAR_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_ULTRAT_CLEAR_Main, i);

        if(u16PQ_ULTRACLEAR_Idx == PQ_GRULE_ULTRAT_CLEAR_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_ULTRAT_CLEAR_Main, u16PQ_ULTRACLEAR_Idx, i);

        PQGRULE_DBG(printf("[UC]SRC: %u, UC: %u, UCIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_ULTRACLEAR_Idx, i, u16IPIdx, u16IPIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_Load_ULTRACLEAR_Table(PQ_WIN eWindow, PQ_FEATURE_SWITCH_TYPE enCtrlType)
{
    MS_U16 u16PQ_TableIdx = 0;
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
    PQGRULE_DBG(printf("[PQ_Load_ULTRACLEAR_Table] "));
    if(enCtrlType == PQ_FEATURE_SWITCH_ON)
    {
        PQGRULE_DBG(printf("On\n"));
        u16PQ_TableIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_ULTRAT_CLEAR_Main, PQ_GRule_ULTRAT_CLEAR_On_Main);
        _MDrv_PQ_Load_ULTRACLEAR_Table(eWindow, u16PQ_TableIdx);
    }
    else
    {
        PQGRULE_DBG(printf("Off\n"));
        u16PQ_TableIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_ULTRAT_CLEAR_Main, PQ_GRule_ULTRAT_CLEAR_Off_Main);
        _MDrv_PQ_Load_ULTRACLEAR_Table(eWindow, u16PQ_TableIdx);
    }
#else
    UNUSED(u16PQ_TableIdx);
    UNUSED(eWindow);
    UNUSED(enCtrlType);

#endif

}

MS_BOOL MDrv_PQ_GRULE_Get_Support_Status(PQ_SUPPORTED_TYPE enType)
{
    MS_BOOL bStatus = FALSE;
    switch(enType)
    {
        case E_PQ_SUPPORTED_FILM:
            if(PQ_GRULE_FILM_MODE_ENABLE)
                bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_NR:
            if(PQ_GRULE_NR_ENABLE)
                bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_MPEG_NR:
            if(PQ_GRULE_MPEG_NR_ENABLE)
                bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_BLACK_LEVEL:
            bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_ULTRA_CLEAR:
            if(PQ_GRULE_ULTRAT_CLEAR_ENABLE)
                bStatus = TRUE;
            break;
        default:
            break;
    }

    return bStatus;

}

MS_U8 _MDrv_PQ_wait_output_vsync(PQ_WIN ePQWin, MS_U8 u8NumVSyncs, MS_U16 u16Timeout)
{
    MS_U32 u32Time;
    MS_U8 bVSync;
    MS_BOOL bMainWin = (ePQWin == PQ_MAIN_WINDOW) ? TRUE : FALSE;

    bVSync = 0;
    u32Time = MsOS_GetSystemTime();

    while(1)
    {
        if(Hal_PQ_get_output_vsync_value(bMainWin) == bVSync)
        {
            u8NumVSyncs--;
            if(bVSync && (u8NumVSyncs == 0))
                break;
            bVSync = !bVSync;
        }

        if((MsOS_GetSystemTime() - u32Time)  >= u16Timeout)
            break;
    }

    return u8NumVSyncs;
}

MS_U8 _MDrv_PQ_wait_input_vsync(PQ_WIN ePQWin,  MS_U8 u8NumVSyncs, MS_U16 u16Timeout)
{
    MS_U32 u32Time;
    MS_BOOL bMainWin = (ePQWin == PQ_MAIN_WINDOW) ? TRUE : FALSE;
    MS_U8 u8VsyncPolarity = Hal_PQ_get_input_vsync_polarity(bMainWin);
    MS_U8 bVSync = !u8VsyncPolarity;

    u32Time = MsOS_GetSystemTime();

    while(1)
    {
        if(Hal_PQ_get_input_vsync_value(bMainWin) == bVSync)
        {
            u8NumVSyncs--;
            if((bVSync == u8VsyncPolarity) && (u8NumVSyncs == 0))
                break;
            bVSync = !bVSync;
        }

        if((MsOS_GetSystemTime() - u32Time) >= u16Timeout)
        {
            //printf("!!input vsync timeout\n");
            break;
        }
    }

    return u8NumVSyncs;
}

#if (PQ_XRULE_DB_ENABLE == 1)
void MDrv_PQ_Set_DBRule(PQ_WIN eWindow, MS_U16 u16MADi_Idx)
{
    MS_U16 eXRuleType = E_XRULE_CSC;
    MS_U16 u16XRuleIdx, u16XRuleIP;
    MS_U16 u16IPIdx, u16TabIdx;

    eXRuleType = (_stMode_Info[eWindow].u16input_vfreq > 550) ? E_XRULE_DB_NTSC : E_XRULE_DB_PAL;

    if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_MC) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_MC_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_27_4R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_27_4R_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_27_2R) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_27_2R_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE8_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE10) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE10_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT8) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE_MOT8_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT10) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_P_MODE_MOT10_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_4R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_2R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_MC) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_6R_MC_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_14F_6R_MC) == u16MADi_Idx)   //Add New MADi mode
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_14F_6R_MC_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_MC_NW) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_MC_NW_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_MC_NW) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_6R_MC_NW_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_884) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_4R_884_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R_884) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_25_2R_884_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_4R_880_Main;
    }
    else if(Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R_880) == u16MADi_Idx)
    {
        u16XRuleIdx = PQ_DBRule_NTSC_24_2R_880_Main;
    }
    else
    {
        u16XRuleIdx = 0xFF;
    }

    for(u16XRuleIP = 0; u16XRuleIP < MDrv_PQ_GetXRuleIPNum(eWindow, eXRuleType); u16XRuleIP++)
    {
        if(u16XRuleIdx == 0xFF)
            continue;

        u16IPIdx = MDrv_PQ_GetXRuleIPIndex(eWindow, eXRuleType, u16XRuleIP);
        u16TabIdx = MDrv_PQ_GetXRuleTableIndex(eWindow, eXRuleType, u16XRuleIdx, u16XRuleIP);
        //(printf("u16XRuleIdx:%u, u16XRuleIP=%u, IPIdx=%u, TabIdx=%u\n", u16XRuleIdx, u16XRuleIP, u16IPIdx, u16TabIdx));

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif
#if PQ_TB_FOR_TV
void MDrv_PQ_EnableMADIForce(PQ_WIN eWindow, MS_BOOL bFullMotion)
{
#if PQ_ENABLE_FORCE_MADI
    if(bFullMotion)
    {
        if(PQ_MAIN_WINDOW == eWindow)
        {
            MDrv_PQ_LoadTable(eWindow, PQ_IP_MADi_Force_YC_FullMotion_Main, PQ_IP_MADi_Force_Main);
        }
        else
        {
            MDrv_PQ_LoadTable(eWindow, PQ_IP_MADi_Force_YC_FullMotion_Sub, PQ_IP_MADi_Force_Sub);
        }
    }
    else
    {
        if(PQ_MAIN_WINDOW == eWindow)
        {
            MDrv_PQ_LoadTable(eWindow, PQ_IP_MADi_Force_YC_FullStill_Main, PQ_IP_MADi_Force_Main);
        }
        else
        {
            MDrv_PQ_LoadTable(eWindow, PQ_IP_MADi_Force_YC_FullStill_Sub, PQ_IP_MADi_Force_Sub);
        }
    }
#else
UNUSED(eWindow);
UNUSED(bFullMotion);
#endif
}

//////////////////////////////////////////////////////////////////
// Load MADi/444To422/422To444 table
//
// parameter:
//    [IN]  u8PQTabType
//          bMemFmt422: TRUE  - 422
//                      FALSE - 444
//          bFBL:       TRUE  - framebuffer-less mode
//                      FALSE - framebuffer mode
//    [OUT] u8BitsPerPixel
//                      bits per pixel for decided memory format
//
// return:  deinterlace (MADi) mode
//
PQ_DEINTERLACE_MODE MDrv_PQ_SetMemFormat(PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_BOOL bFBL, MS_U8 *pu8BitsPerPixel)
{
    MS_U16 u16TabIdx_MemFormat = 0;
    MS_U16 u16TabIdx_MADi = 0;
    MS_U16 u16TabIdx_MADi_Motion=0xFFFF;
    MS_U16 u16TabIdx_444To422 = 0;
    MS_U16 u16TabIdx_422To444 = 0;
    PQ_DEINTERLACE_MODE eDeInterlaceMode = PQ_DEINT_OFF;
    MS_U8 u8FrameCount = 4;

    if(eWindow == PQ_MAIN_WINDOW)
    {
        MApi_XC_Set_OPWriteOffEnable(ENABLE, MAIN_WINDOW); //default
    }
    else
    {
        PQINFO_DBG(printf("No need to enable OP write with sub window, it would change main window setting \n"));
    }

    PQBW_DBG(printf("==>In PQ : eWindow =%d\n", eWindow));

    if(!bFBL)
    {
        // if MADi mode change from 25 <=> 27, cropping base address need to be recalculated.
#if PQ_GRULE_OSD_BW_ENABLE
        if(((_stMode_Info[eWindow].u16input_hsize >= 1280) && (_stMode_Info[eWindow].u16input_vsize >= 720)) &&
                (_bOSD_On))
        {
            MDrv_PQ_GetMADiFromGrule(eWindow, &u16TabIdx_MADi_Motion, &u16TabIdx_MADi);
        }
        else
#endif
        {
            MDrv_PQ_GetMADiInGeneral(eWindow, &u16TabIdx_MADi_Motion, &u16TabIdx_MADi);
#if PQ_GRULE_OSD_BW_ENABLE
            MDrv_PQ_Patch2Rto4RForFieldPackingMode(eWindow, u16TabIdx_MADi);
#endif
        }

        MDrv_PQ_GetMADiForRFBL(eWindow, bFBL, &u16TabIdx_MADi_Motion, &u16TabIdx_MADi);
    }
    else // FBL
    {
        MS_U8 u8FblMode = Hal_PQ_get_madi_fbl_mode(bMemFmt422, _stMode_Info[eWindow].bInterlace);
        u16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, (MS_MADI_TYPE)u8FblMode);
    }


    PQMADi_DBG(printf("Memory is %s mode \n", (bFBL)?("FBL"):("FB")));
    PQMADi_DBG(printf("get MADi idx = %u\n", u16TabIdx_MADi));

    MDrv_PQ_GetMemFmtInGeneral(eWindow, bMemFmt422, &u16TabIdx_MemFormat, &u16TabIdx_444To422, &u16TabIdx_422To444);

    PQTAB_DBG(printf("%s :u16TabIdx_444To422=%u\n", eWindow?"SubWin":"MainWin",u16TabIdx_444To422));
    PQTAB_DBG(printf("%s :u16TabIdx_422To444=%u\n", eWindow?"SubWin":"MainWin",u16TabIdx_422To444));
    PQMADi_DBG(printf("MemFmt is 422 : %s\n",(bMemFmt422)?("Yes"):("No")));

    MDrv_PQ_GetBPPInfoFromMADi(eWindow,
                               bMemFmt422,
                               u16TabIdx_MADi,
                               u16TabIdx_MemFormat,
                               pu8BitsPerPixel,
                               &eDeInterlaceMode,
                               &u8FrameCount);

    PQMADi_DBG(printf("BitPerPixel =%d, DIMode =%x\n", *pu8BitsPerPixel, eDeInterlaceMode));

    _gbMemfmt422[eWindow] = bMemFmt422;

    MDrv_PQ_ChangeMemConfigFor3D(eWindow, bMemFmt422, &u16TabIdx_MemFormat, &u16TabIdx_444To422, &u16TabIdx_422To444,
                                    pu8BitsPerPixel, &u16TabIdx_MADi_Motion, &u16TabIdx_MADi);

    if(eWindow == PQ_MAIN_WINDOW)
    {
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, u16TabIdx_MemFormat, PQ_IP_MemFormat_Main);
#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
    MDrv_PQ_LoadTable(eWindow, u16TabIdx_MADi, PQ_IP_MADi_Main);
#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif

#if PQ_GRULE_OSD_BW_ENABLE
        if(0xFFFF != u16TabIdx_MADi_Motion)
        {
            PQTAB_DBG(printf("u16TabIdx_MADi_Motion=%u\n", u16TabIdx_MADi_Motion));
            MDrv_PQ_LoadTable(eWindow, u16TabIdx_MADi_Motion, PQ_IP_MADi_Motion_Main);
        }
#endif
        MDrv_PQ_LoadTable(eWindow, u16TabIdx_444To422, PQ_IP_444To422_Main);
        MDrv_PQ_LoadTable(eWindow, u16TabIdx_422To444, PQ_IP_422To444_Main);
    }
#if PQ_ENABLE_PIP
    else
    {
        MDrv_PQ_LoadTable(PQ_SUB_WINDOW, u16TabIdx_MemFormat, PQ_IP_MemFormat_Sub);
        MDrv_PQ_LoadTable(eWindow, u16TabIdx_MADi, PQ_IP_MADi_Sub);
        MDrv_PQ_LoadTable(eWindow, u16TabIdx_444To422, PQ_IP_444To422_Sub);
        MDrv_PQ_LoadTable(eWindow, u16TabIdx_422To444, PQ_IP_422To444_Sub);

        //R W bank mapping mode patch for Qmap hasn't this item for sub
        if(_stMode_Info[eWindow].bInterlace)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK12_47_L, 0x4000 , 0xE000 );
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK12_47_L, 0x2000 , 0xE000 );
        }
    }
#endif

    PQTAB_DBG(printf("u16TabIdx_MemFormat=%u\n", u16TabIdx_MemFormat));
    PQTAB_DBG(printf("%s, u16TabIdx_MADi=%u\n", eWindow?"SUB_WIN":"MAIN_WIN", u16TabIdx_MADi));
    PQTAB_DBG(printf("%s, u16TabIdx_MADi=%u\n", eWindow?"SUB_WIN":"MAIN_WIN", u16TabIdx_MADi));
    PQTAB_DBG(printf("%s :u16TabIdx_444To422=%u\n", eWindow?"SUB_WIN":"MAIN_WIN", u16TabIdx_444To422));
    PQTAB_DBG(printf("%s :u16TabIdx_422To444=%u\n", eWindow?"SUB_WIN":"MAIN_WIN", u16TabIdx_422To444));

#if (PQ_XRULE_DB_ENABLE == 1)
    MDrv_PQ_Set_DBRule(eWindow, u16TabIdx_MADi);
#endif

    MDrv_PQ_ForceBPPForDynamicMemFmt(eWindow, bMemFmt422, pu8BitsPerPixel);
    MDrv_PQ_SuggestFrameNum(eWindow, u16TabIdx_MADi, u8FrameCount);

    if(_bDS_En)
    {
        _u16RW_Method = Hal_PQ_get_rw_method(eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE);
        Hal_PQ_set_rw_method(eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE, 0x4000);
    }

    _bRWNumIsReset[eWindow] = TRUE;

    return eDeInterlaceMode;
}

void MDrv_PQ_ReduceBW_ForOSD(PQ_WIN eWindow, MS_BOOL bOSD_On)
{
#if PQ_GRULE_OSD_BW_ENABLE
    MS_U8 u8BitPerPixel = 0;
{
    if((_stMode_Info[eWindow].u16input_hsize >= 1280) && (_stMode_Info[eWindow].u16input_vsize >= 720))
    {
        PQBW_DBG(printf("[PQ_ReduceBW_ForOSD]:%u\n", bOSD_On));

        _bOSD_On = bOSD_On;
        bSetFrameCount = FALSE;

        MDrv_PQ_SetMemFormat(
            eWindow,
            _gbMemfmt422[eWindow],
            _stMode_Info[eWindow].bFBL,
            &u8BitPerPixel);
        bSetFrameCount = TRUE;

    }
}
#else
    UNUSED(eWindow);
    UNUSED(bOSD_On);
#endif

    return;
}

void MDrv_PQ_ReduceBW_ForPVR(PQ_WIN eWindow, MS_BOOL bPVR_On)
{
    UNUSED(eWindow);
    UNUSED(bPVR_On);
}

void _MDrv_PQ_Set_MVOP_UVShift(MS_BOOL bEnable)
{
    MS_U8 regval;

    regval = MApi_XC_ReadByte(VOP_MPG_JPG_SWITCH);

    if(((regval & 0x10) == 0x10) && ((regval & 0x3) == 0x2))
    {
        // 422 with MCU control mode
        if(bEnable)
        {
            MS_ASSERT(0);
        }
    }

    // output 420 and interlace
    //[IP - Sheet] : Main Page --- 420CUP
    //[Project] :  Titania2
    //[Description]:   Chroma artifacts when 420to422 is applied duplicate method.
    //[Root cause]: Apply 420to422 average algorithm to all DTV input cases.
    //The average algorithm must cooperate with MVOP.
    MApi_XC_WriteByteMask(VOP_UV_SHIFT, (bEnable) ? 1 : 0, 0x3);
}

void MDrv_PQ_Set420upsampling(PQ_WIN eWindow,
                              MS_BOOL bFBL,
                              MS_BOOL bPreV_ScalingDown,
                              MS_U16 u16V_CropStart)
{
    MS_U16 u16TabIdx;

    u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_420CUP_Main);

    PQTAB_DBG(printf("[PQ_Set420upsampling]: SrcType:%u, FBL:%u, PreV down:%u, V_CropStart:%u, u8TabIdx=%u, ",
                     _u16PQSrcType[eWindow], bFBL, bPreV_ScalingDown, u16V_CropStart, u16TabIdx));

    if((u16TabIdx == Hal_PQ_get_420_cup_idx(MS_420_CUP_ON)) && (!bPreV_ScalingDown) && (!bFBL))
    {
        PQTAB_DBG(printf("UVShift: on\n"));
        _MDrv_PQ_Set_MVOP_UVShift(ENABLE);
        MDrv_PQ_LoadTable(eWindow, Hal_PQ_get_420_cup_idx(MS_420_CUP_ON), PQ_IP_420CUP_Main);
    }
    else    // P mode should not do UV shift
    {
        PQTAB_DBG(printf("UVShift: off\n"));
        _MDrv_PQ_Set_MVOP_UVShift(DISABLE);
        MDrv_PQ_LoadTable(eWindow, Hal_PQ_get_420_cup_idx(MS_420_CUP_OFF), PQ_IP_420CUP_Main);
    }

    if((u16V_CropStart & 0x3) == 0)       // crop lines are multiple of 4
        Hal_PQ_set_420upSample(0x6666);
    else if((u16V_CropStart & 0x1) == 0)  // crop lines are multiple of 2
        Hal_PQ_set_420upSample(0x9999);
    else
        MS_ASSERT(0);
}

void MDrv_PQ_SetFilmMode(PQ_WIN eWindow, MS_BOOL bEnable)
{
    MS_U16 u16TabIdx;

    PQTAB_DBG(printf("[PQ_SetFilmMode]: PQTabType=%u, enable=%u\n", eWindow, bEnable));

    if(bEnable)
        u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_Film32_Main);
    else
        u16TabIdx = PQ_IP_Film32_OFF_Main;

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, PQ_IP_Film32_Main);

    if(bEnable)
        u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_Film22_Main);
    else
        u16TabIdx = PQ_IP_Film22_OFF_Main;

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, PQ_IP_Film22_Main);
}


void MDrv_PQ_SetNonLinearScaling(PQ_WIN eWindow, MS_U8 u8Level, MS_BOOL bEnable)
{
    MS_U16 u16TabIdx;
    PQTAB_DBG(printf("[PQ_SetNonLinearScaling]: Level=%u, enable=%u\n", u8Level, bEnable));

    if(eWindow == PQ_SUB_WINDOW)
    {
        //sub window don't need nonLinear scaling
        return;
    }

    if(bEnable)
    {
        switch(_stPQ_Info.u16PnlWidth)
        {
            case 1920:
                switch(u8Level)
                {
                    case 2:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1920_2);
                        break;
                    case 1:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1920_1);
                        break;
                    case 0:
                    default:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1920_0);
                        break;
                }
                break;
            case 1680:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1680);
                break;
            case 1440:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1440);
                break;
            case 1366:
                switch(u8Level)
                {
                    case 2:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1366_2);
                        break;
                    case 1:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1366_1);
                        break;
                    case 0:
                    default:
                        u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_1366_0);
                        break;

                }
                break;
            default:
                u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_OFF);
                break;

                break;
        }
    }
    else
    {
        u16TabIdx = Hal_PQ_get_hnonlinear_idx(MS_HNONLINEAR_OFF);
    }

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, PQ_IP_HnonLinear_Main);
}

void _MDrv_PQ_CloneSram(void)
{
    MS_U16 u16CSRAM1 = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_C_SRAM1_Main);
    MS_U16 u16CSRAM2 = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_C_SRAM2_Main);

#if(ENABLE_PQ_BIN)
    if(gbPQBinEnable)
    {
        MS_PQBin_IP_Table_Info stTableInfo;
        memset(&stTableInfo, 0, sizeof(MS_PQBin_IP_Table_Info));

        MDrv_PQBin_GetTable(MDrv_PQBin_GetPanelIdx(),
                        _u16SRAM1,
                        PQ_IP_SRAM1_Main,
                        &stTableInfo,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        stTableInfo.u16TableType = PQ_TABTYPE_SRAM3;
        MDrv_PQBin_DumpTable(&stTableInfo);

        MDrv_PQBin_GetTable(MDrv_PQBin_GetPanelIdx(),
                        _u16SRAM2,
                        PQ_IP_SRAM2_Main,
                        &stTableInfo,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        stTableInfo.u16TableType = PQ_TABTYPE_SRAM4;
        MDrv_PQBin_DumpTable(&stTableInfo);

        MDrv_PQBin_GetTable(MDrv_PQBin_GetPanelIdx(),
                        u16CSRAM1,
                        PQ_IP_C_SRAM1_Main,
                        &stTableInfo,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        stTableInfo.u16TableType = PQ_TABTYPE_C_SRAM3;
        MDrv_PQBin_DumpTable(&stTableInfo);

        MDrv_PQBin_GetTable(MDrv_PQBin_GetPanelIdx(),
                        u16CSRAM2,
                        PQ_IP_C_SRAM2_Main,
                        &stTableInfo,
                        &stPQBinHeaderInfo[PQ_BIN_STD_MAIN]);
        stTableInfo.u16TableType = PQ_TABTYPE_C_SRAM4;
        MDrv_PQBin_DumpTable(&stTableInfo);
    }
    else
#endif
    {
#if(PQ_ONLY_SUPPORT_BIN == 0)
        EN_IP_Info stIpInfo;
        memset(&stIpInfo, 0, sizeof(EN_IP_Info));
        stIpInfo = MDrv_PQ_GetTable_(MAIN, _u16SRAM1, PQ_IP_SRAM1_Main);
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM3, (void *)((MS_U32)&stIpInfo.pIPTable[stIpInfo.u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM1)]));

        stIpInfo = MDrv_PQ_GetTable_(MAIN, _u16SRAM2, PQ_IP_SRAM2_Main);
        Hal_PQ_set_y_sram_table(SC_FILTER_Y_SRAM4, (void *)((MS_U32)&stIpInfo.pIPTable[stIpInfo.u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_Y_SRAM2)]));

        stIpInfo = MDrv_PQ_GetTable_(MAIN, u16CSRAM1, PQ_IP_C_SRAM1_Main);
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM3, (void *)((MS_U32)&stIpInfo.pIPTable[stIpInfo.u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM1)]));

        stIpInfo = MDrv_PQ_GetTable_(MAIN, u16CSRAM2, PQ_IP_C_SRAM2_Main);
        Hal_PQ_set_c_sram_table(SC_FILTER_C_SRAM4, (void *)((MS_U32)&stIpInfo.pIPTable[stIpInfo.u8TabIdx * Hal_PQ_get_sram_size(SC_FILTER_C_SRAM2)]));
#endif
    }
}

void MDrv_PQ_3DCloneforPIP(MS_BOOL bIpSync)
{
    MVOP_Handle stHdl = { E_MVOP_MODULE_MAIN };
    MS_BOOL bEnAltOutputState = FALSE;

    if (E_MVOP_OK == MDrv_MVOP_GetCommand(&stHdl, E_MVOP_CMD_GET_3DLR_ALT_OUT, &bEnAltOutputState, sizeof(bEnAltOutputState)))
    {
        //printf("%s Get3DLRAltOutput=0x%x\n", __FUNCTION__, bEnAltOutputState);
    }
    else
    {
        //printf("Fail to query E_MVOP_CMD_GET_3DLR_ALT_OUT!!\n");
    }

    if((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MApi_XC_Get_3D_Output_Mode()))
    {
        _MDrv_PQ_CloneSram();
        MApi_XC_ACE_3DClonePQMap(E_ACE_WEAVETYPE_NONE);
        //printf("3D:confirm to clone main/sub win\n");

        if((MApi_XC_Get_3D_HW_Version() < 2) && bIpSync)
        {
            MApi_XC_3DMainSub_IPSync();
            //printf("3D:confirm to do main/sub win sync\n");
        }
    }
    else if(((E_XC_3D_INPUT_TOP_BOTTOM == MApi_XC_Get_3D_Input_Mode(MAIN_WINDOW)) && (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode()))
    || ((E_XC_3D_INPUT_NORMAL_2D == MApi_XC_Get_3D_Input_Mode(MAIN_WINDOW)) && (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())))
    {
        if(MApi_XC_Get_3D_HW_Version() < 2)
        {
            MDrv_PQ_EnableMADIForce(PQ_MAIN_WINDOW, TRUE);
#if PQ_MADI_DFK
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_MADi_DFK_OFF_Main, PQ_IP_MADi_DFK_Main);
#endif
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_EODi_OFF_Main, PQ_IP_EODi_Main);
        }
    }
    else if(MApi_XC_IsCurrentFrameBufferLessMode() && bEnAltOutputState)
    {
        MApi_XC_ACE_3DClonePQMap(E_ACE_WEAVETYPE_H);
    }
    MApi_XC_3D_PostPQSetting(MAIN_WINDOW);
}
#endif
void MDrv_PQ_DisableFilmMode(PQ_WIN eWindow, MS_BOOL bOn)
{
    if(bOn)
    {
        MDrv_PQ_SetFilmMode(eWindow, DISABLE);
    }
    else
    {
        MDrv_PQ_SetFilmMode(eWindow, ENABLE);
    }
}

MS_BOOL MDrv_PQ_DisableUCFeature(PQ_WIN eWindow)
{
    MS_BOOL bRet = TRUE;
    UNUSED(eWindow);
#if PQ_EN_UCNR_OFF

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_UCNR_OFF_Main, PQ_IP_UCNR_Main);
#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_UC_CTL_OFF_Main, PQ_IP_UC_CTL_Main);
#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_UCDi_OFF_Main, PQ_IP_UCDi_Main);
#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif

    bRet = TRUE;
#else
    bRet = FALSE;
#endif
    return bRet;
}

MS_BOOL MDrv_PQ_SetDMSV12L(PQ_WIN eWindow, MS_BOOL bEnable)
{
#if PQ_EN_DMS_SW_CTRL
    MS_U16 u16TabIdx, u16IPIdx;

    if(bEnable == TRUE)
    {
        u16TabIdx = PQ_IP_DMS_V_12L_ON_Main;
    }
    else
    {
        u16TabIdx = PQ_IP_DMS_V_12L_OFF_Main;
    }
    u16IPIdx = PQ_IP_DMS_V_12L_Main;
    eWindow = PQ_MAIN_WINDOW;

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif

    MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);

#if(ENABLE_PQ_MLOAD)
    MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif
    _bDMSV12L_PIPEn = bEnable;

#else
    UNUSED(eWindow);
    UNUSED(bEnable);
#endif

    return TRUE;
}

MS_BOOL MDrv_PQ_GetDMSV12LFromXRuleTable(PQ_WIN eWindow)
{
    UNUSED(eWindow);

    return _bDMSV12L_PQConfigEn;
}

E_DRVPQ_ReturnValue MDrv_PQ_GetLibVer(const MSIF_Version **ppVersion)
{
    if(ppVersion == NULL)
    {
        return E_DRVPQ_RET_FAIL;
    }

    *ppVersion = &_drv_pq_version;
    return E_DRVPQ_RET_OK;
}


PQ_ReturnValue MDrv_PQ_GetInfo(MS_PQ_INFO *pInfo)
{
    memcpy((void*)pInfo, (void*)&_info, sizeof(MS_PQ_INFO));
    return E_PQ_RET_OK;
}

MS_BOOL  MDrv_PQ_GetStatus(MS_PQ_Status *pStatus)
{
    memcpy((void*)pStatus, (void*)&_status, sizeof(MS_PQ_Status));
    return FALSE;
}

MS_BOOL  MDrv_PQ_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    _u16PQDbgSwitch = u16DbgSwitch;
    return TRUE;
}

void MDrv_PQ_Set_DisplayType(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType)
{
    //Qmap 1920,1920_PIP,1920_POP should be the same
    //for changing easily purpose, here overwrite the enDisplayType to DISPLAY_ONE, so we don't need to change QMap excel for it.
    //enDisplaType = PQ_DISPLAY_ONE;

#if (PQ_ENABLE_PIP)
    PQINFO_DBG(printf("%s: enDisplaType=%u, u16DisplayWidth=%u\n", __FUNCTION__, enDisplaType, u16DisplayWidth));
#if(ENABLE_PQ_BIN)
    if(gbPQBinEnable)
    {
        MDrv_PQBin_SetPanelID(u16DisplayWidth == 1366 ? 1 : 0);
        MDrv_PQBin_SetDisplayType(enDisplaType == PQ_DISPLAY_ONE ? PQ_BIN_DISPLAY_ONE :
                                  enDisplaType == PQ_DISPLAY_PIP ? PQ_BIN_DISPLAY_PIP :
                                  PQ_BIN_DISPLAY_POP);
    }
    else
#endif
    {
#if (PQ_ONLY_SUPPORT_BIN == 0)
        MS_BOOL bSkipCommTable = TRUE;

        if(genDisplayType != enDisplaType)
        {
            MDrv_PQ_Set_DisplayType_Sub(u16DisplayWidth, enDisplaType, bSkipCommTable);
            MDrv_PQ_Set_DisplayType_Main(u16DisplayWidth, enDisplaType, bSkipCommTable);
            genDisplayType = enDisplaType;
        }

#endif // #if PQ_ONLY_SUPPORT_BIN

    }

#else
    UNUSED(u16DisplayWidth);
    UNUSED(enDisplaType);
#endif
}

void MDrv_PQ_MADiForceMotionC(PQ_WIN eWindow, MS_BOOL bEnable)
{
    MS_U16 u16DataC = 0;
    u16DataC = Hal_PQ_get_force_c_motion(eWindow == PQ_MAIN_WINDOW);

    if(eWindow == PQ_SUB_WINDOW)
    {
        u16DataC  <<= 8;
    }

    if(bEnable)
    {
        u16DataC |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
    }
    else
    {
        u16DataC &= (eWindow == PQ_MAIN_WINDOW ? 0x000F : 0x0F00);
    }
    Hal_PQ_set_force_c_motion(eWindow == PQ_MAIN_WINDOW, u16DataC);

}

void MDrv_PQ_MADiForceMotionY(PQ_WIN eWindow, MS_BOOL bEnable)
{
    MS_U16 u16Data = 0;

    u16Data = Hal_PQ_get_force_y_motion(eWindow == PQ_MAIN_WINDOW);

    if(eWindow == PQ_SUB_WINDOW)
    {
        u16Data <<= 8;
    }
    if(bEnable)
    {
        u16Data |= (eWindow == PQ_MAIN_WINDOW ? 0x0080 : 0x8000);
    }
    else
    {
        u16Data &= (eWindow == PQ_MAIN_WINDOW ? 0x003F : 0x3F00);
    }

    Hal_PQ_set_force_y_motion(eWindow == PQ_MAIN_WINDOW, u16Data);
}

void MDrv_PQ_SetRFblMode(MS_BOOL bEnable, MS_BOOL bFilm)
{
    _stRFBL_Info.bEnable = bEnable;
    _stRFBL_Info.bFilm = bFilm;

    if(!bEnable)    //default mode
    {
        MDrv_PQ_MADiForceMotionY(PQ_MAIN_WINDOW, FALSE);
        MDrv_PQ_MADiForceMotionC(PQ_MAIN_WINDOW, FALSE);

        _stRFBL_Info.u8MADiType = MS_MADI_24_2R;
        //printf("[%s]-[%d] : Y/C motion : ( 0 , 0 ) \n", __FUNCTION__, __LINE__);
    }
    else
    {
        _stRFBL_Info.u8MADiType = (bFilm) ? (MS_U8)MS_MADI_24_RFBL_FILM : (MS_U8)MS_MADI_24_RFBL_NFILM;
        MDrv_PQ_MADiForceMotionY(PQ_MAIN_WINDOW, FALSE);
        MDrv_PQ_MADiForceMotionC(PQ_MAIN_WINDOW, (!bFilm));

        //printf("[%s]-[%d] : Y/C motion : ( 0 , %d) \n", __FUNCTION__, __LINE__,(!bFilm));
    }
}

void MDrv_PQ_GetRFblMode(MS_PQ_RFBL_INFO * pstInfo)
{
    memcpy(pstInfo, &_stRFBL_Info, sizeof(MS_PQ_RFBL_INFO));
    return;
}

#if (PQ_ENABLE_VD_SAMPLING)

MS_BOOL MDrv_PQ_Get_VDSampling_Info(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, PQ_VIDEOSTANDARD_TYPE eStandard, MS_PQ_VD_Sampling_Info *pInfo)
{
    PQ_VIDEOSTANDARD_TYPE eCurStandard = _stVD_Info[eWindow].enVideoStandard;
    MS_U16 u16CurSrc = _u16PQSrcType[eWindow];
    MS_U16 u16TableIdx;
    MS_U16 u16IPIdx = Hal_PQ_get_ip_idx(MS_PQ_IP_VD_SAMPLING);
    MS_U8 u8SampleTable[10];

    memset(u8SampleTable, 0, sizeof(u8SampleTable));

    PQINFO_DBG(printf("[PQ] GetVDSampling: win:%d, src:%d, standard:%d, PQIP=%x\n", eWindow, enInputSourceType, eStandard, u16IPIdx));
    if((((enInputSourceType >= PQ_INPUT_SOURCE_TV) && (enInputSourceType <= PQ_INPUT_SOURCE_SVIDEO))||(enInputSourceType == PQ_INPUT_SOURCE_SCART)) &&
            (eStandard < E_PQ_VIDEOSTANDARD_NOTSTANDARD) &&
            (u16IPIdx != 0xFFFF))
    {
        _stVD_Info[eWindow].enVideoStandard = eStandard;
        _u16PQSrcType[eWindow] = QM_InputSourceToIndex(eWindow, enInputSourceType);



        u16TableIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        PQINFO_DBG(printf("[PQ] VDSampling: TableIdx=%d\n", u16TableIdx));
#if(ENABLE_PQ_BIN)
        if(((gbPQBinEnable == 1) && (u16TableIdx == PQ_BIN_IP_NULL))
                || ((gbPQBinEnable == 0) && (u16TableIdx == PQ_IP_NULL)))
#else
        if(u16TableIdx == PQ_IP_NULL)
#endif
        {
            pInfo->eType = PQ_VD_SAMPLING_OFF;
        }
        else
        {
            MDrv_PQ_LoadTableData(eWindow, u16TableIdx, u16IPIdx, u8SampleTable, 10);
            pInfo->u16Vstart = ((MS_U16)u8SampleTable[0]) | (((MS_U16)u8SampleTable[1])<<8);
            pInfo->u16Hstart = ((MS_U16)u8SampleTable[2]) | (((MS_U16)u8SampleTable[3])<<8);
            pInfo->u16Hsize  = ((MS_U16)u8SampleTable[4]) | (((MS_U16)u8SampleTable[5]) << 8);
            pInfo->u16Vsize  = ((MS_U16)u8SampleTable[6]) | (((MS_U16)u8SampleTable[7]) << 8);
            pInfo->u16Htt    = ((MS_U16)u8SampleTable[8]) | (((MS_U16)u8SampleTable[9]) << 8);
            pInfo->eType = PQ_VD_SAMPLING_ON;

            PQINFO_DBG(printf("VD x:%x, y:%x, w:%x, h:%x, Htt=%x \n",
                    pInfo->u16Hstart, pInfo->u16Vstart, pInfo->u16Hsize, pInfo->u16Vsize, pInfo->u16Htt));
        }

        _stVD_Info[eWindow].enVideoStandard  = eCurStandard;
        _u16PQSrcType[eWindow] = u16CurSrc;
    }
    else
    {
        pInfo->eType = PQ_VD_SAMPLING_OFF;
    }
    return TRUE;
}

#endif


MS_BOOL _MDrv_PQ_Get_HSDSampling(PQ_WIN eWindow, MS_U32 *pu32ratio, MS_BOOL *bADVMode)
{
    MS_U16 u16IPIdx = Hal_PQ_get_ip_idx(MS_PQ_IP_HSD_SAMPLING);
    MS_U16 u16TableIdx;
    MS_BOOL bret = TRUE;

    if(u16IPIdx != 0xFFFF)
    {
        u16TableIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
    }
    else
    {
#if(ENABLE_PQ_BIN)
        u16TableIdx = (gbPQBinEnable) ? PQ_BIN_IP_NULL : PQ_IP_NULL;
#else
        u16TableIdx = PQ_IP_NULL;
#endif
    }


#if(ENABLE_PQ_BIN)
    if(((gbPQBinEnable == 1) && (u16TableIdx == PQ_BIN_IP_NULL)) ||
            ((gbPQBinEnable == 0) && (u16TableIdx == PQ_IP_NULL)))
#else
    if(u16TableIdx == PQ_IP_NULL)
#endif
    {
        *pu32ratio = 0;
        bret = FALSE;
    }
#if(ENABLE_PQ_BIN)
    else if(( MDrv_PQBin_GetSkipRule(PQ_IP_HSD_Y_Main, &stPQBinHeaderInfo[PQ_BIN_STD_MAIN])) ||
            ( MDrv_PQBin_GetSkipRule(PQ_IP_HSD_C_Main, &stPQBinHeaderInfo[PQ_BIN_STD_MAIN])))
#else
    else if((MST_SkipRule_IP_Main[PQ_IP_HSD_Y_Main] == 1) || (MST_SkipRule_IP_Main[PQ_IP_HSD_C_Main] == 1))
#endif
    {
        SRULE_DBG(printf("HSD_Y, HSD_C is True in SKipRule, driver consider HSDSampling is not defined in QMap.\n"));
        *pu32ratio = 0;
        bret = FALSE;
    }
    else
    {
#if PQ_NEW_HSD_SAMPLING_TYPE
        if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_1_000))
        {
            *pu32ratio = 1000;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_1_125))
        {
            *pu32ratio = 875;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_1_250))
        {
            *pu32ratio = 750;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_1_375))
        {
            *pu32ratio = 625;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_1_500))
        {
            *pu32ratio = 500;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_1_625))
        {
            *pu32ratio = 375;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_1_750))
        {
            *pu32ratio = 250;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_1_875))
        {
            *pu32ratio = 125;
        }
        else if(u16TableIdx == Hal_PQ_get_hsd_sampling_idx(MS_HSD_SAMPLING_Div_2_000))
        {
            *pu32ratio = 1;
        }
        else
#endif
        {
            *pu32ratio = 0;
        }

        bret = TRUE;
    }

    // Load HSD_Y & HSD_C
    if(bret)
    {
        MS_U16 u16HSD_Y_idx, u16HSD_C_idx;

        u16HSD_Y_idx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_HSD_Y_Main);
        MDrv_PQ_LoadTable(eWindow, u16HSD_Y_idx, PQ_IP_HSD_Y_Main);

        u16HSD_C_idx = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_HSD_C_Main);
        MDrv_PQ_LoadTable(eWindow, u16HSD_C_idx, PQ_IP_HSD_C_Main);

        SRULE_DBG(printf("HSD_Y:%d, HSD_C:%d\n", u16HSD_Y_idx, u16HSD_C_idx));

        *bADVMode = ((u16HSD_Y_idx == PQ_IP_HSD_Y_CB_Main) &&
                     (u16HSD_C_idx == PQ_IP_HSD_C_CB_Main)) ? FALSE : TRUE;
    }
    else
    {
        *bADVMode = FALSE;
    }


    SRULE_DBG(printf("[PQ] GetHSDSampling, IPIdx=%d, Table=%d, ratio=%ld ret=%d\n", u16IPIdx, u16TableIdx, *pu32ratio, bret));

    return bret;
}


MS_BOOL MDrv_PQ_IOCTL(PQ_WIN eWindow, MS_U32 u32Flag, void *pBuf, MS_U32 u32BufSize)
{
    MS_BOOL bret = TRUE;

    switch(u32Flag)
    {
        case E_PQ_IOCTL_HSD_SAMPLING:
        {
            if(u32BufSize == sizeof(PQ_HSD_SAMPLING_INFO))
            {
                MS_U32 u32ratio;
                MS_BOOL bADVMode;

                bret = _MDrv_PQ_Get_HSDSampling(eWindow, &u32ratio, &bADVMode);
                ((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio = u32ratio;
                ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode = bADVMode;
                bret = TRUE;

                //printf("ratio %lx, bADVmode=%d\n\n",((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio, ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode);

            }
            else
            {
                //printf("Size is not correct, in=%ld, %d\n", u32BufSize, sizeof(PQ_HSD_SAMPLING_INFO));
                ((PQ_HSD_SAMPLING_INFO *)pBuf)->u32ratio = 0;
                ((PQ_HSD_SAMPLING_INFO *)pBuf)->bADVMode = 0;
                bret = FALSE;
            }
            break;
        }

        case E_PQ_IOCTL_PREVSD_BILINEAR:
            bret = TRUE;
            *((MS_BOOL *)pBuf) = TRUE;
            break;

        case E_PQ_IOCTL_ADC_SAMPLING:
            if(u32BufSize == sizeof(PQ_ADC_SAMPLING_INFO))
            {

                PQ_INPUT_SOURCE_TYPE enInputSrc = ((PQ_ADC_SAMPLING_INFO *)pBuf)->enPQSourceType;
                MS_PQ_Mode_Info      *pModeInfo = &(((PQ_ADC_SAMPLING_INFO *)pBuf)->stPQModeInfo);
                MS_PQ_ADC_SAMPLING_TYPE enADCSamplingType = MDrv_PQ_Get_ADCSampling_Info(eWindow, enInputSrc, pModeInfo);

                if((enADCSamplingType == E_PQ_ADC_SAMPLING_NOT_SUPPORT) || (enADCSamplingType == E_PQ_ADC_SAMPLING_NONE))
                {
                    ((PQ_ADC_SAMPLING_INFO *)pBuf)->u16ratio = 1;
                    bret = FALSE;
                }
                else
                {
                    ((PQ_ADC_SAMPLING_INFO *)pBuf)->u16ratio = (MS_U16)enADCSamplingType;
                    bret = TRUE;
                }
            }
            else
            {
                bret = FALSE;
                ((PQ_ADC_SAMPLING_INFO *)pBuf)->u16ratio = 1;
            }
            break;
        case E_PQ_IOCTL_RFBL_CTRL:
            //printf("IOCTL %lx\n", u32Flag);
#if PQ_ENABLE_RFBL
            bret = MDrv_PQ_Get_RFBL_Info(eWindow);
#else
            bret = FALSE;
#endif
            UNUSED(pBuf);
            UNUSED(u32BufSize);
            break;

        case E_PQ_IOCTL_PQ_SUGGESTED_FRAMENUM:
        {
            if(u32BufSize == sizeof(MS_U16))
            {
                *((MS_U16 *)pBuf) = _u16PQSuggestedFrameNum[eWindow];
                bret = TRUE;
            }
            else
            {
                *((MS_U16 *)pBuf) = 0;
                bret = FALSE;
            }
            break;
        }

        default:
            printf("unknown IOCTL %lx\n", u32Flag);
            UNUSED(pBuf);
            UNUSED(u32BufSize);
            bret = FALSE;
            break;
    }

    //printf("[PQ] IOCTL : win:%x, Flag=%08lx, size=%ld, ret:%x\n", eWindow, u32Flag, u32BufSize, bret);

    return bret;
}


MS_PQ_ADC_SAMPLING_TYPE MDrv_PQ_Get_ADCSampling_Info(PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo)
{
    MS_PQ_ADC_SAMPLING_TYPE enRetType;
    MS_U16 u16PQSrcType;
    MS_U16 u16TableIdx;
    MS_PQ_Mode_Info stModeInfo;

    u16PQSrcType = _u16PQSrcType[eWindow];
    memcpy(&stModeInfo, &_stMode_Info[eWindow], sizeof(MS_PQ_Mode_Info));

    MDrv_PQ_Set_ModeInfo(eWindow, enInputSourceType, pstPQModeInfo);
    _u16PQSrcType[eWindow] = QM_InputSourceToIndex(eWindow, enInputSourceType);

    if(Hal_PQ_get_ip_idx(MS_PQ_IP_ADC_SAMPLING) == 0xFFFF)
    {
        return E_PQ_ADC_SAMPLING_NOT_SUPPORT;
    }

    u16TableIdx = MDrv_PQ_GetTableIndex(eWindow, Hal_PQ_get_ip_idx(MS_PQ_IP_ADC_SAMPLING));

    if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(MS_ADC_SAMPLING_X_1))
    {
        enRetType = E_PQ_ADC_SAMPLING_X_1;
    }
    else if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(MS_ADC_SAMPLING_X_2))
    {
        enRetType = E_PQ_ADC_SAMPLING_X_2;
    }
    else if(u16TableIdx == Hal_PQ_get_adc_sampling_idx(MS_ADC_SAMPLING_X_4))
    {
        enRetType = E_PQ_ADC_SAMPLING_X_4;
    }
    else
    {
        enRetType = E_PQ_ADC_SAMPLING_NONE;
    }

    memcpy(&_stMode_Info[eWindow], &stModeInfo, sizeof(MS_PQ_Mode_Info));
    _u16PQSrcType[eWindow] = u16PQSrcType;

    return enRetType;
}

MS_BOOL MDrv_PQ_Set3D_OnOff(MS_BOOL bEn)
{
    _gIs3D_En = bEn;
    return TRUE;
}

MS_BOOL MDrv_PQ_SetH264_OnOff(MS_BOOL bEn)
{
    _gIsH264 = bEn;
    return TRUE;
}

MS_BOOL MDrv_PQ_SetNetworkMM_OnOff(MS_BOOL bEn)
{
    _gIsNetworkMM = bEn;
    return TRUE;
}

MS_BOOL MDrv_PQ_SetMM_OnOff(MS_BOOL bEn)
{
    _gIsMMplayer = bEn;
    return TRUE;
}

MS_BOOL MDrv_PQ_SetRmvb_OnOff(MS_BOOL bEn, PQ_WIN ePQWindow)
{
    _gIsRmvb[ePQWindow] = bEn;
    return TRUE;
}
#if PQ_TB_FOR_TV
MS_BOOL MDrv_PQ_GetCaps(MS_PQ_CAP_INFO *pInfo)
{
    MS_BOOL bRet = TRUE;
#if(PQ_ENABLE_PIP)
    const XC_ApiInfo* pXC_ApiInfo = MApi_XC_GetInfo();
    if (pXC_ApiInfo != NULL)
    {
        pInfo->bPIP_Supported = ((pXC_ApiInfo->u8MaxWindowNum) >= 2) ? TRUE : FALSE;
    }
    else
    {
        pInfo->bPIP_Supported = FALSE;
        bRet = FALSE;
    }
#else
    pInfo->bPIP_Supported = FALSE;
#endif

#if(PQ_ENABLE_3D_VIDEO)
    pInfo->b3DVideo_Supported = TRUE;
#else
    pInfo->b3DVideo_Supported = FALSE;
#endif

    return bRet;
}
#endif
void MDRV_PQ_PrintLoadTableInfo(MS_PQ_PQLOADTABLE_WIN enPQWin)
{
#if(ENABLE_PQ_LOAD_TABLE_INFO)

    MS_U16 u16IPIdx;
    MS_U8  u8TabIdx;

    if(enPQWin == PQ_LOAD_TABLE_MAIN)
        printf("[TableInfo::Main] \n");
    else if(enPQWin == PQ_LOAD_TABLE_MAIN_EX)
        printf("[TableInfo::Main Ex] \n");
    else if(enPQWin == PQ_LOAD_TABLE_SUB)
        printf("[TableInfo::SUB] \n");
    else if(enPQWin == PQ_LOAD_TABLE_SUB_EX)
        printf("[TableInfo::SUB Ex] \n");
    else
        return;

    for(u16IPIdx = 0; u16IPIdx < PQ_IP_NUM_Main; ++ u16IPIdx)
    {
        printf(" %u(%s)=", u16IPIdx, MDrv_PQ_GetIPName(u16IPIdx));  // IP Name

        if(enPQWin == PQ_LOAD_TABLE_MAIN)
        {
            u8TabIdx = MDrv_PQ_Get_LoadTableInfo_IP_Tab(MAIN, u16IPIdx);
            if(u8TabIdx == 0xFF)
                printf("_____\n");
            else
                printf("%u(%s)\n", u8TabIdx, MDrv_PQ_GetTableName(u16IPIdx, u8TabIdx));
        }
        else if(enPQWin == PQ_LOAD_TABLE_MAIN_EX)
        {
            u8TabIdx = MDrv_PQ_Get_LoadTableInfo_IP_Tab(MAINEX, u16IPIdx);
            if(u8TabIdx == 0xFF)
                printf("_____\n");
            else
                printf("Tab%u\n", u8TabIdx);
        }

#if PQ_ENABLE_PIP
        else if(enPQWin == PQ_LOAD_TABLE_SUB)
        {
            u8TabIdx = MDrv_PQ_Get_LoadTableInfo_IP_Tab(SUB, u16IPIdx);
            if(u8TabIdx == 0xFF)
                printf("_____\n");
            else
                printf("%u(%s)\n", u8TabIdx, MDrv_PQ_GetTableName(u16IPIdx, u8TabIdx));
        }
        else if(enPQWin == PQ_LOAD_TABLE_SUB_EX)
        {
            u8TabIdx = MDrv_PQ_Get_LoadTableInfo_IP_Tab(SUBEX, u16IPIdx);
            if(u8TabIdx == 0xFF)
                printf("_____\n");
            else
                printf("Tab%u\n", u8TabIdx);

        }
#endif
        else
        {
        }
    }
#else
    UNUSED(enPQWin);
#endif
}

MS_BOOL MDrv_PQ_SetDS_OnOFF(PQ_WIN eWindow, MS_BOOL bEn)
{
    //store the original value
    _u16RW_Method = Hal_PQ_get_rw_method(eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE);
    if(bEn)
    {
        _bDS_En = TRUE;
    }
    else
    {
        _bDS_En = FALSE;
        Hal_PQ_set_rw_method(eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE, _u16RW_Method);
    }

    return TRUE;
}

MS_BOOL MDrv_PQ_Get_RFBL_Info(PQ_WIN eWindow)
{
    MS_BOOL bret = TRUE;

    MS_U16 u16TabIdx_MADi = 0;
//  PQ_WIN eWindow = (bIsMainWindow == TRUE) ? (PQ_MAIN_WINDOW):(PQ_SUB_WINDOW);

    u16TabIdx_MADi = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MADi_Main);
    (printf("[PQ] MADI: u16TabIdx_MADi=%d\n", u16TabIdx_MADi));

#if(ENABLE_PQ_BIN)
    if(((gbPQBinEnable == 1) && (u16TabIdx_MADi == PQ_BIN_IP_NULL)) ||
            ((gbPQBinEnable == 0) && (u16TabIdx_MADi == PQ_IP_NULL)))
#else
    if(u16TabIdx_MADi == PQ_IP_NULL)
#endif
    {
        //MApi_XC_EnableRequest_FrameBufferLess(DISABLE);

        bret = FALSE;
        printf("[PQ] MADI: Get Null\n");
        printf("[PQ] MADI: DISABLE RFBL\n");

    }
    else
    {
        if((u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_2D))
                || (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_25D))
                || (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D))
                || (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D_YC))
                || (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D_FILM))
                || (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE10_MOT))
                || (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE8_MOT))

          )
        {
            //MApi_XC_EnableRequest_FrameBufferLess(ENABLE);
            printf("[PQ] MADI: ENABLE RFBL\n");

        }
        else
        {
            //MApi_XC_EnableRequest_FrameBufferLess(DISABLE);
            printf("[PQ] MADI: DISABLE RFBL\n");

        }
        bret = TRUE;
    }

    return bret;
}

MS_BOOL MDrv_PQ_CheckHDMode(PQ_WIN eWindow)
{
    return _gIsSrcHDMode[eWindow];
}

//for Super Resolution interlace and SD mode detected use
MS_BOOL MDrv_PQ_Get_Interlace_SD_mode(void)
{
    if(QM_IsInterlaced(PQ_MAIN_WINDOW) && (QM_GetInputHSize(PQ_MAIN_WINDOW) < 1500))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if PQ_GRULE_PTP_ENABLE
static void _MDrv_PQ_LoadPTPTable(PQ_WIN eWindow, MS_U16 u16PQ_PTP_Idx)
{
    MS_U16 i, u16IPIdx=0, u16TabIdx=0;

    if(eWindow == PQ_SUB_WINDOW)
    {
        return;
    }

    if (u16PQ_PTP_Idx > PQ_GRULE_PTP_LVL_NUM_Main){
        MS_ASSERT(0); return;
    }

    for(i=0; i<PQ_GRULE_PTP_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex(eWindow, PQ_GRule_PTP_Main, i);

        if (u16PQ_PTP_Idx == PQ_GRULE_PTP_NUM_Main) // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex(eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_PTP_Main, u16PQ_PTP_Idx, i);

        PQP2P_DBG(printf("[PTP]SRC: %u, u16PQ_PTP_Idx: %u, PIPIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
            _u16PQSrcType[eWindow], u16PQ_PTP_Idx, i, u16IPIdx, u16IPIdx);)

        MDrv_PQ_LoadTable(eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif


void MDrv_PQ_LoadPTPTable(PQ_WIN eWindow, PQ_PTP_FUNCTION_TYPE enPTP_Type)
{
#if PQ_GRULE_PTP_ENABLE
    PQGRULE_DBG(printf("[PQ_LoadPTPTable] "));
    MS_U16 u16PQ_PTPIdx;

    if(eWindow == PQ_SUB_WINDOW)
    {
        return;
    }

     if( enPTP_Type == PQ_PTP_PTP )
     {
         PQP2P_DBG(printf("PTP\n");)
         u16PQ_PTPIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_PTP_Main, PQ_GRule_Lvl_PTP_On_Main);
         _MDrv_PQ_LoadPTPTable(eWindow, u16PQ_PTPIdx);
     }
     else
     {
         MS_ASSERT(0);
     }
#else
    UNUSED(eWindow);
    UNUSED(enPTP_Type);
#endif
}

MS_BOOL MDrv_PQ_SetDualViewState(MS_BOOL bEnable)
{
    _bDualViewEnable = bEnable;
    return TRUE;
}

MS_BOOL MDrv_PQ_GetDualViewState(void)
{
    return _bDualViewEnable;
}
#if PQ_TB_FOR_TV
MS_BOOL MDrv_PQ_Set_PointToPoint(XC_SETWIN_INFO *pstXC_SetWin_Info,
                               MS_BOOL bEnable,
                               PQ_WIN eWindow)
{
    //sub window don't support point to point
    if(eWindow == PQ_SUB_WINDOW)
    {
        return FALSE;
    }

    if(bEnable)
    {
        //Center the display window
        pstXC_SetWin_Info->stDispWin.x = (PanelWidth- pstXC_SetWin_Info->stCropWin.width)/2;
        pstXC_SetWin_Info->stDispWin.width = pstXC_SetWin_Info->stCropWin.width;
        pstXC_SetWin_Info->stDispWin.y = (PanelHeight- pstXC_SetWin_Info->stCropWin.height)/2;
        pstXC_SetWin_Info->stDispWin.height = pstXC_SetWin_Info->stCropWin.height;

        //RGB HDMI, DVI, VGA
        if((((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == TRUE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
             ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == FALSE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
             (IsSrcTypeVga(pstXC_SetWin_Info->enInputSourceType))) &&
             (!pstXC_SetWin_Info->bInterlace))
        {
            MApi_XC_EnableForceRGBin(TRUE,  MAIN_WINDOW);
        }
        //MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_444, eWindow);
        MApi_XC_Set_MemFmt(E_MS_XC_MEM_FMT_444);
    }
    else
    {
        MApi_XC_EnableForceRGBin(FALSE,  MAIN_WINDOW);
        //MApi_XC_Set_MemFmtEx(E_MS_XC_MEM_FMT_AUTO, eWindow);
        MApi_XC_Set_MemFmt(E_MS_XC_MEM_FMT_AUTO);
    }

    PQP2P_DBG(printf("[MDrv_PQ_Set_PointToPoint] True or False: %u\n",bEnable);)
    _bPointToPointMode = bEnable;

    return TRUE;
}
#endif
MS_BOOL MDrv_PQ_Get_PointToPoint(PQ_WIN eWindow)
{
    //Only Main window support point to point
    if(eWindow == PQ_MAIN_WINDOW)
    {
        return _bPointToPointMode;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_PQ_Check_PointToPoint_Condition(XC_SETWIN_INFO *pstXC_SetWin_Info,PQ_WIN eWindow)
{
    UNUSED(eWindow);
    MS_BOOL bRet = FALSE;

#if PQ_GRULE_PTP_ENABLE
    // HDMI RGB, HDMI YUV444, DVI, YPbPr, VGA.
    // all in progressive mode
    PQP2P_DBG(printf("----HDMI source=%u, HDMIPacket=%u, RGB=%u, YUV444=%u, Ypbpr=%u, VGA=%u, interlace=%u\n",
            (IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)),
            (_stHDMI_Info[eWindow].bIsHDMI == TRUE),
            (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW)),
            (QM_HDMIPC_COLORYUV444(PQ_MAIN_WINDOW)),
            (IsSrcTypeYPbPr(pstXC_SetWin_Info->enInputSourceType)),
            (IsSrcTypeVga(pstXC_SetWin_Info->enInputSourceType)),
            (pstXC_SetWin_Info->bInterlace)
            );)
    //at this time, the PQ input source has not been inited. so we use XC_SETWIN_INFO to check
    //we cannot do the PTP when timing size is bigger than panel size
    if((((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == TRUE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
         ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == TRUE) && (QM_HDMIPC_COLORYUV444(PQ_MAIN_WINDOW))) ||
         ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == FALSE) && (QM_HDMIPC_COLORYUV444(PQ_MAIN_WINDOW))) ||
         ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == FALSE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
         (IsSrcTypeVga(pstXC_SetWin_Info->enInputSourceType)) ||
         (IsSrcTypeYPbPr(pstXC_SetWin_Info->enInputSourceType))) &&
         (!pstXC_SetWin_Info->bInterlace) &&
         ((g_IPanel.Width() >= pstXC_SetWin_Info->stCropWin.width) && (g_IPanel.Height() >= pstXC_SetWin_Info->stCropWin.height) &&
           (eWindow == PQ_MAIN_WINDOW))
         )
    {
        bRet = TRUE;
    }
#else
    UNUSED(pstXC_SetWin_Info);
#endif

    PQP2P_DBG(printf("[MDrv_PQ_Check_PointToPoint_Condition] %u\n",bRet);)

    return bRet;
}

MS_BOOL MDrv_PQ_Update_MemFormat(void)
{
    MS_U8 u8BitPerPixel = 0;
    MDrv_PQ_SetMemFormat(
        PQ_MAIN_WINDOW,
        _gbMemfmt422[PQ_MAIN_WINDOW],
        _stMode_Info[PQ_MAIN_WINDOW].bFBL,
        &u8BitPerPixel);

    return TRUE;
}

MS_U16 MDrv_PQ_GetHsize(PQ_WIN enPQWin)
{
    return _stMode_Info[enPQWin].u16input_hsize;
}

MS_U16 MDrv_PQ_GetVsize(PQ_WIN enPQWin)
{
    return _stMode_Info[enPQWin].u16input_vsize;
}

MS_BOOL MDrv_PQ_IsInterlace(PQ_WIN enPQWin)
{
    return _stMode_Info[enPQWin].bInterlace;
}

MS_BOOL MDrv_PQ_LOW_3dQuality(MS_BOOL bEnable)
{
    if(bEnable)
    {
#if PQ_VIP_CTL
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_Peaking_band_AV_NTSC_Main, PQ_IP_VIP_Peaking_band_Main);
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_VNMR_S2_Main, PQ_IP_VIP_VNMR_Main);
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_VLPF_coef1_0x6_Main, PQ_IP_VIP_VLPF_coef1_Main);
#endif
    }
    else
    {
#if PQ_VIP_CTL
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_Peaking_band_Main), PQ_IP_VIP_Peaking_band_Main);
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_VNMR_Main), PQ_IP_VIP_VNMR_Main);
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_VLPF_coef1_Main), PQ_IP_VIP_VLPF_coef1_Main);
#endif
    }
    return TRUE;
}

MS_BOOL MDrv_PQ_MWE_SetEnhanceQuality(void)
{
#if PQ_VIP_RGBCMY_CTL
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_R_INC_3_Main, PQ_IP_VIP_ICC_R_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_G_INC_8_Main, PQ_IP_VIP_ICC_G_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_B_INC_6_Main, PQ_IP_VIP_ICC_B_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_C_INC_6_Main, PQ_IP_VIP_ICC_C_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_M_INC_6_Main, PQ_IP_VIP_ICC_M_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_Y_INC_7_Main, PQ_IP_VIP_ICC_Y_Main);
#endif
    return TRUE;
}

MS_BOOL MDrv_PQ_MWE_RestoreEnhanceQuality(void)
{
#if PQ_VIP_RGBCMY_CTL
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_R_Main), PQ_IP_VIP_ICC_R_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_G_Main), PQ_IP_VIP_ICC_G_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_B_Main), PQ_IP_VIP_ICC_B_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_C_Main), PQ_IP_VIP_ICC_C_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_M_Main), PQ_IP_VIP_ICC_M_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_Y_Main), PQ_IP_VIP_ICC_Y_Main);
#endif
    return TRUE;
}

MS_BOOL MDrv_PQ_MWE_RestoreOffQuality(void)
{
#if PQ_VIP_CTL
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_ON_Main, PQ_IP_VIP_ICC_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_IBC_ON_Main, PQ_IP_VIP_IBC_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_IHC_ON_Main, PQ_IP_VIP_IHC_Main);
#endif
    return TRUE;
}

MS_BOOL MDrv_PQ_MWE_SetOffQuality(void)
{
#if PQ_VIP_CTL
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_ICC_OFF_Main, PQ_IP_VIP_ICC_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_IBC_OFF_Main, PQ_IP_VIP_IBC_Main);
    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_IP_VIP_IHC_OFF_Main, PQ_IP_VIP_IHC_Main);
#endif
    return TRUE;
}

void MDrv_PQ_SetSelectCSC(MS_U16 u16selection, PQ_WIN eWindow)
{
    HAL_PQ_set_SelectCSC(u16selection, (eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE));
}

MS_BOOL MDrv_PQ_GetR2YEqSelect(PQ_WIN eWindow)
{
    if(PQ_MAIN_WINDOW == eWindow)
    {
        //return ((MApi_XC_R2BYTEMSK(REG_SC_BK18_6F_L, BIT(2)) == 0)? FALSE : TRUE);
        SC_BK_SWITCH(0x18);
        return ((msRead2Byte(u32ScBaseAddr + (REG_SC_BK18_6F_L & 0xFF)) & BIT(2)) == 0)? FALSE : TRUE;
    }
    else
    {
        //return ((MApi_XC_R2BYTEMSK(REG_SC_BK18_6F_L, BIT(6)) == 0)? FALSE : TRUE);
        SC_BK_SWITCH(0x18);
        return ((msRead2Byte(u32ScBaseAddr + (REG_SC_BK18_6F_L & 0xFF)) & BIT(6)) == 0)? FALSE : TRUE;
    }
}

MS_BOOL MDrv_PQ_Set_DisplayType_Sub(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType, MS_BOOL bSkipCommTable)
{
#if PQ_ENABLE_PIP && (PQ_ONLY_SUPPORT_BIN == 0)
        PQTABLE_INFO PQTableInfo;
        PQTABLE_INFO PQTableInfoEx;

        memset(&PQTableInfo, 0, sizeof(PQTABLE_INFO));
        memset(&PQTableInfoEx, 0, sizeof(PQTABLE_INFO));

        // Load Sub PQ
        switch(enDisplaType)
        {
            default:
            case PQ_DISPLAY_ONE:
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_2560_Sub;
#if (PQ_GRULE_NR_ENABLE)
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_2560_NR_Sub;
#endif
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_3840_Sub;
#if (PQ_GRULE_NR_ENABLE)
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_3840_NR_Sub;
#endif
                }
                break;

            case PQ_DISPLAY_PIP:
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_2560_PIP_Sub;
#if (PQ_GRULE_NR_ENABLE)
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_2560_PIP_NR_Sub;
#endif
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_3840_PIP_Sub;
#if (PQ_GRULE_NR_ENABLE)
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_3840_PIP_NR_Sub;
#endif
                }
                break;

            case PQ_DISPLAY_POP:
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_2560_POP_Sub;
#if (PQ_GRULE_NR_ENABLE)
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_2560_POP_NR_Sub;
#endif
                }
                else
                {
                    PQTableInfo.pQuality_Map_Aray = (void*)QMAP_3840_POP_Sub;
#if (PQ_GRULE_NR_ENABLE)
                    PQTableInfo.pGRule_Level[E_GRULE_NR] = (void*)MST_GRule_3840_POP_NR_Sub;
#endif
                }
                break;
        }

        //add super resolution
        PQTableInfo.pGRule_Level[E_GRULE_SR] = (void*)MST_GRule_3840_SR_Sub;

        // table config parameter
        PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Sub;
        PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Sub;
        PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Sub;
        PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Sub;

        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSD] = PQ_HSDRule_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSD] = PQ_VSDRule_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSP] = PQ_HSPRule_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSP] = PQ_VSPRule_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_CSC] = PQ_CSCRule_IP_NUM_Sub;
#if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_NTSC] = PQ_DBRule_NTSC_IP_NUM_Sub;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_PAL] = PQ_DBRule_PAL_IP_NUM_Sub;
#endif

        PQTableInfo.pXRule_IP_Index[E_XRULE_HSD] = (void*)MST_HSDRule_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_VSD] = (void*)MST_VSDRule_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_HSP] = (void*)MST_HSPRule_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_VSP] = (void*)MST_VSPRule_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_CSC] = (void*)MST_CSCRule_IP_Index_Sub;
#if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.pXRule_IP_Index[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_IP_Index_Sub;
        PQTableInfo.pXRule_IP_Index[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_IP_Index_Sub;
#endif

        PQTableInfo.pXRule_Array[E_XRULE_HSD] = (void*)MST_HSDRule_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_VSD] = (void*)MST_VSDRule_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_HSP] = (void*)MST_HSPRule_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_VSP] = (void*)MST_VSPRule_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_CSC] = (void*)MST_CSCRule_Array_Sub;
#if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.pXRule_Array[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_Array_Sub;
        PQTableInfo.pXRule_Array[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_Array_Sub;
#endif

        PQTableInfo.u8PQ_GRule_Num[E_GRULE_NR] = PQ_GRULE_NR_NUM_Sub;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_NR] = PQ_GRULE_NR_IP_NUM_Sub;
        PQTableInfo.pGRule_IP_Index[E_GRULE_NR] = (void*)MST_GRule_NR_IP_Index_Sub;
        PQTableInfo.pGRule_Array[E_GRULE_NR] = (void*)MST_GRule_NR_Sub;

        //add sub super resolution
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_SR] = PQ_GRULE_SR_NUM_Sub;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_SR] = PQ_GRULE_SR_IP_NUM_Sub;
        PQTableInfo.pGRule_IP_Index[E_GRULE_SR] = (void*)MST_GRule_SR_IP_Index_Sub;
        PQTableInfo.pGRule_Array[E_GRULE_SR] = (void*)MST_GRule_SR_Sub;

        MDrv_PQ_AddTable_(SUB, &PQTableInfo);
        if(!bSkipCommTable)
        {
            MDrv_PQ_LoadCommTable_(SUB);
        }


        // table config parameter
        PQTableInfoEx = PQTableInfo;

        PQTableInfoEx.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Sub_Ex;
        PQTableInfoEx.u8PQ_IP_Num = PQ_IP_NUM_Sub_Ex;
        PQTableInfoEx.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Sub_Ex;
        PQTableInfoEx.pSkipRuleIP = (void*)MST_SkipRule_IP_Sub_Ex;

        switch(enDisplaType)
        {
            default:
            case PQ_DISPLAY_ONE:
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_2560_Sub_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_3840_Sub_Ex;
                }
                break;

            case PQ_DISPLAY_PIP:
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_2560_PIP_Sub_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_3840_PIP_Sub_Ex;
                }
                break;

            case PQ_DISPLAY_POP:
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_2560_POP_Sub_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_3840_POP_Sub_Ex;
                }
                break;
        }
        MDrv_PQ_AddTable_(SUBEX, &PQTableInfoEx);
        if(!bSkipCommTable)
        {
            MDrv_PQ_LoadCommTable_(SUBEX);
        }
        return TRUE;
#else
    UNUSED(u16DisplayWidth);
    UNUSED(enDisplaType);
    UNUSED(bSkipCommTable);
    return FALSE;
#endif
}

MS_BOOL MDrv_PQ_Set_DisplayType_Main(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType, MS_BOOL bSkipCommTable)
{
#if (PQ_ONLY_SUPPORT_BIN == 0)
        PQTABLE_INFO PQTableInfo;
        PQTABLE_INFO PQTableInfoEx;

        memset(&PQTableInfo, 0, sizeof(PQTABLE_INFO));
        memset(&PQTableInfoEx, 0, sizeof(PQTABLE_INFO));

        switch(enDisplaType)
        {
            default:
            case PQ_DISPLAY_ONE:
                PQTableInfo.pQuality_Map_Aray = (void*)((u16DisplayWidth <= 2560)?QMAP_2560_Main: QMAP_3840_Main);
#if (PQ_GRULE_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_NR_Main: MST_GRule_3840_NR_Main);
#endif
#if (PQ_GRULE_SR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_SR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_SR_Main: MST_GRule_3840_SR_Main);

#endif
#if (PQ_GRULE_MPEG_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_MPEG_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_MPEG_NR_Main: MST_GRule_3840_MPEG_NR_Main);
#endif
#if (PQ_GRULE_FILM_MODE_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_FilmMode] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_FILM_MODE_Main: MST_GRule_3840_FILM_MODE_Main);
#endif
#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_DynContr] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_DYNAMIC_CONTRAST_Main: MST_GRule_3840_DYNAMIC_CONTRAST_Main);
#endif
#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_ULTRACLEAR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_ULTRAT_CLEAR_Main: MST_GRule_3840_ULTRAT_CLEAR_Main);
#endif

#if (PQ_GRULE_BYPASS_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_Bypass_NR_Main: MST_GRule_3840_Bypass_NR_Main);
#endif
#if (PQ_GRULE_BYPASS_DI_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_DI] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_Bypass_DI_Main: MST_GRule_3840_Bypass_DI_Main);
#endif
#if (PQ_GRULE_BYPASS_PEAKING_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_Peaking] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_Bypass_Peaking_Main: MST_GRule_3840_Bypass_Peaking_Main);
#endif
#if (PQ_GRULE_BYPASS_OTHERS_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_Others] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_Bypass_Others_Main: MST_GRule_3840_Bypass_Others_Main);
#endif

#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_DDR_SELECT] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_DDR_SELECT_Main: MST_GRule_3840_DDR_SELECT_Main);
#endif
#if (PQ_GRULE_OSD_BW_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_OSD_BW] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_OSD_BW_Main: MST_GRule_3840_OSD_BW_Main);
#endif
#if (PQ_GRULE_PTP_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_PTP] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PTP_Main: MST_GRule_3840_PTP_Main);
#endif
                 break;
#if PQ_ENABLE_PIP
            case PQ_DISPLAY_PIP:
                PQTableInfo.pQuality_Map_Aray = (void*)((u16DisplayWidth <= 2560)?QMAP_2560_PIP_Main: QMAP_3840_PIP_Main);
#if (PQ_GRULE_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_NR_Main: MST_GRule_3840_PIP_NR_Main);
#endif
#if PQ_GRULE_SR_ENABLE
                PQTableInfo.pGRule_Level[E_GRULE_SR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_SR_Main: MST_GRule_3840_PIP_SR_Main);
#endif
#if (PQ_GRULE_MPEG_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_MPEG_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_MPEG_NR_Main: MST_GRule_3840_PIP_MPEG_NR_Main);
#endif
#if (PQ_GRULE_FILM_MODE_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_FilmMode] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_FILM_MODE_Main: MST_GRule_3840_PIP_FILM_MODE_Main);
#endif
#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_DynContr] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_DYNAMIC_CONTRAST_Main: MST_GRule_3840_PIP_DYNAMIC_CONTRAST_Main);
#endif
#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_ULTRACLEAR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_ULTRAT_CLEAR_Main: MST_GRule_3840_PIP_ULTRAT_CLEAR_Main);
#endif
#if (PQ_GRULE_BYPASS_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_Bypass_NR_Main: MST_GRule_3840_PIP_Bypass_NR_Main);
#endif
#if (PQ_GRULE_BYPASS_DI_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_DI] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_Bypass_DI_Main: MST_GRule_3840_PIP_Bypass_DI_Main);
#endif
#if (PQ_GRULE_BYPASS_PEAKING_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_Peaking] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_Bypass_Peaking_Main: MST_GRule_3840_PIP_Bypass_Peaking_Main);
#endif
#if (PQ_GRULE_BYPASS_OTHERS_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_Others] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_Bypass_Others_Main: MST_GRule_3840_PIP_Bypass_Others_Main);
#endif
#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_DDR_SELECT] =
                    (void*)((u16DisplayWidth == 1366)?MST_GRule_1366_PIP_DDR_SELECT_Main:MST_GRule_1920_PIP_DDR_SELECT_Main);
#endif
#if (PQ_GRULE_OSD_BW_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_OSD_BW] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_PIP_OSD_BW_Main: MST_GRule_3840_PIP_OSD_BW_Main);
#endif
#if (PQ_GRULE_PTP_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_PTP] =
                    (void*)((u16DisplayWidth == 1366)?MST_GRule_1366_PIP_PTP_Main:MST_GRule_1920_PIP_PTP_Main);
#endif

                break;

            case PQ_DISPLAY_POP:
                PQTableInfo.pQuality_Map_Aray = (void*)((u16DisplayWidth <= 2560)?QMAP_2560_POP_Main: QMAP_3840_POP_Main);
#if (PQ_GRULE_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_NR_Main: MST_GRule_3840_POP_NR_Main);
#endif
#if (PQ_GRULE_SR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_SR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_SR_Main: MST_GRule_3840_POP_SR_Main);
#endif
#if (PQ_GRULE_MPEG_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_MPEG_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_MPEG_NR_Main: MST_GRule_3840_POP_MPEG_NR_Main);
#endif
#if (PQ_GRULE_FILM_MODE_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_FilmMode] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_FILM_MODE_Main: MST_GRule_3840_POP_FILM_MODE_Main);
#endif
#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_DynContr] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_DYNAMIC_CONTRAST_Main: MST_GRule_3840_POP_DYNAMIC_CONTRAST_Main);
#endif
#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_ULTRACLEAR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_ULTRAT_CLEAR_Main: MST_GRule_3840_POP_ULTRAT_CLEAR_Main);
#endif
#if (PQ_GRULE_BYPASS_NR_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_NR] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_Bypass_NR_Main: MST_GRule_3840_POP_Bypass_NR_Main);
#endif
#if (PQ_GRULE_BYPASS_DI_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_DI] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_Bypass_DI_Main: MST_GRule_3840_POP_Bypass_DI_Main);
#endif
#if (PQ_GRULE_BYPASS_PEAKING_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_Peaking] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_Bypass_Peaking_Main: MST_GRule_3840_POP_Bypass_Peaking_Main);
#endif
#if (PQ_GRULE_BYPASS_OTHERS_ENABLE)
                PQTableInfo.pGRule_Level[E_GRULE_Bypass_Others] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_Bypass_Others_Main: MST_GRule_3840_POP_Bypass_Others_Main);
#endif
#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_DDR_SELECT] =
                    (void*)((u16DisplayWidth == 1366)?MST_GRule_1366_POP_DDR_SELECT_Main:MST_GRule_1920_POP_DDR_SELECT_Main);
#endif
#if (PQ_GRULE_OSD_BW_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_OSD_BW] =
                    (void*)((u16DisplayWidth <= 2560)?MST_GRule_2560_POP_OSD_BW_Main: MST_GRule_3840_POP_OSD_BW_Main);
#endif
#if (PQ_GRULE_PTP_ENABLE == 1)
                PQTableInfo.pGRule_Level[E_GRULE_PTP] =
                    (void*)((u16DisplayWidth == 1366)?MST_GRule_1366_POP_PTP_Main:MST_GRule_1920_POP_PTP_Main);
#endif
                break;
#endif
        }

        // table config parameter
        PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Main;
        PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Main;
        PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Main;
        PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Main;

        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSD] = PQ_HSDRule_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSD] = PQ_VSDRule_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSP] = PQ_HSPRule_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSP] = PQ_VSPRule_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_CSC] = PQ_CSCRule_IP_NUM_Main;
#if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_NTSC] = PQ_DBRule_NTSC_IP_NUM_Main;
        PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_PAL]  = PQ_DBRule_PAL_IP_NUM_Main;
#endif

        PQTableInfo.pXRule_IP_Index[E_XRULE_HSD] = (void*)MST_HSDRule_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_VSD] = (void*)MST_VSDRule_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_HSP] = (void*)MST_HSPRule_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_VSP] = (void*)MST_VSPRule_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_CSC] = (void*)MST_CSCRule_IP_Index_Main;
#if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.pXRule_IP_Index[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_IP_Index_Main;
        PQTableInfo.pXRule_IP_Index[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_IP_Index_Main;
#endif

        PQTableInfo.pXRule_Array[E_XRULE_HSD] = (void*)MST_HSDRule_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_VSD] = (void*)MST_VSDRule_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_HSP] = (void*)MST_HSPRule_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_VSP] = (void*)MST_VSPRule_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_CSC] = (void*)MST_CSCRule_Array_Main;
#if (PQ_XRULE_DB_ENABLE == 1)
        PQTableInfo.pXRule_Array[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_Array_Main;
        PQTableInfo.pXRule_Array[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_Array_Main;
#endif

        PQTableInfo.u8PQ_GRule_Num[E_GRULE_NR] = PQ_GRULE_NR_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_NR] = PQ_GRULE_NR_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_NR] = (void*)MST_GRule_NR_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_NR] = (void*)MST_GRule_NR_Main;
#if (PQ_GRULE_SR_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_SR] = PQ_GRULE_SR_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_SR] = PQ_GRULE_SR_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_SR] = (void*)MST_GRule_SR_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_SR] = (void*)MST_GRule_SR_Main;
#endif
#if (PQ_GRULE_MPEG_NR_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_MPEG_NR] = PQ_GRULE_MPEG_NR_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_MPEG_NR] = PQ_GRULE_MPEG_NR_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_MPEG_NR] = (void*)MST_GRule_MPEG_NR_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_MPEG_NR] = (void*)MST_GRule_MPEG_NR_Main;
#endif

#if (PQ_GRULE_FILM_MODE_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_FilmMode] = PQ_GRULE_FILM_MODE_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_FilmMode] = PQ_GRULE_FILM_MODE_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_FilmMode] = (void*)MST_GRule_FILM_MODE_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_FilmMode] = (void*)MST_GRule_FILM_MODE_Main;
#endif

#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_DynContr] = PQ_GRULE_DYNAMIC_CONTRAST_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_DynContr] = PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_DynContr] = (void*)MST_GRule_DYNAMIC_CONTRAST_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_DynContr] = (void*)MST_GRule_DYNAMIC_CONTRAST_Main;
#endif

#if (PQ_GRULE_BYPASS_DI_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_Bypass_DI] = PQ_GRULE_Bypass_DI_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_Bypass_DI] = PQ_GRULE_Bypass_DI_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_Bypass_DI] = (void*)MST_GRule_Bypass_DI_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_Bypass_DI] = (void*)MST_GRule_Bypass_DI_Main;
#endif

#if (PQ_GRULE_BYPASS_NR_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_Bypass_NR] = PQ_GRULE_Bypass_NR_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_Bypass_NR] = PQ_GRULE_Bypass_NR_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_Bypass_NR] = (void*)MST_GRule_Bypass_NR_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_Bypass_NR] = (void*)MST_GRule_Bypass_NR_Main;
#endif

#if (PQ_GRULE_BYPASS_PEAKING_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_Bypass_Peaking] = PQ_GRULE_Bypass_Peaking_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_Bypass_Peaking] = PQ_GRULE_Bypass_Peaking_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_Bypass_Peaking] = (void*)MST_GRule_Bypass_Peaking_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_Bypass_Peaking] = (void*)MST_GRule_Bypass_Peaking_Main;
#endif

#if (PQ_GRULE_BYPASS_OTHERS_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_Bypass_Others] = PQ_GRULE_Bypass_Others_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_Bypass_Others] = PQ_GRULE_Bypass_Others_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_Bypass_Others] = (void*)MST_GRule_Bypass_Others_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_Bypass_Others] = (void*)MST_GRule_Bypass_Others_Main;
#endif

#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_ULTRACLEAR] = PQ_GRULE_ULTRAT_CLEAR_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_ULTRACLEAR] = PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_ULTRACLEAR] = (void*)MST_GRule_ULTRAT_CLEAR_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_ULTRACLEAR] = (void*)MST_GRule_ULTRAT_CLEAR_Main;
#endif

#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_DDR_SELECT] = PQ_GRULE_DDR_SELECT_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_DDR_SELECT] = PQ_GRULE_DDR_SELECT_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_DDR_SELECT] = (void*)MST_GRule_DDR_SELECT_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_DDR_SELECT] = (void*)MST_GRule_DDR_SELECT_Main;
#endif

#if (PQ_GRULE_OSD_BW_ENABLE == 1)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_OSD_BW] = PQ_GRULE_OSD_BW_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_OSD_BW] = PQ_GRULE_OSD_BW_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_OSD_BW] = (void*)MST_GRule_OSD_BW_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_OSD_BW] = (void*)MST_GRule_OSD_BW_Main;
#endif

#if (PQ_GRULE_PTP_ENABLE == 1)
        PQTableInfo.u8PQ_GRule_Num[E_GRULE_PTP] = PQ_GRULE_PTP_NUM_Main;
        PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_PTP] = PQ_GRULE_PTP_IP_NUM_Main;
        PQTableInfo.pGRule_IP_Index[E_GRULE_PTP] = (void*)MST_GRule_PTP_IP_Index_Main;
        PQTableInfo.pGRule_Array[E_GRULE_PTP] = (void*)MST_GRule_PTP_Main;
#endif

        MDrv_PQ_AddTable_(MAIN, &PQTableInfo);
        if(!bSkipCommTable)
        {
            MDrv_PQ_LoadCommTable_(MAIN);
        }

#if (ENABLE_PQ_EX)
        // table config parameter
        PQTableInfoEx = PQTableInfo;

        PQTableInfoEx.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Main_Ex;
        PQTableInfoEx.u8PQ_IP_Num = PQ_IP_NUM_Main_Ex;
        PQTableInfoEx.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Main_Ex;
        PQTableInfoEx.pSkipRuleIP = (void*)MST_SkipRule_IP_Main_Ex;

        switch(enDisplaType)
        {
            default:
            case PQ_DISPLAY_ONE:
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_2560_Main_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_3840_Main_Ex;
                }
                break;

            case PQ_DISPLAY_PIP:
#if PQ_ENABLE_PIP
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_2560_PIP_Main_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_3840_PIP_Main_Ex;
                }
#endif
                break;

            case PQ_DISPLAY_POP:
#if PQ_ENABLE_PIP
                if(u16DisplayWidth <= 2560)
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_2560_POP_Main_Ex;
                }
                else
                {
                    PQTableInfoEx.pQuality_Map_Aray = (void*)QMAP_3840_POP_Main_Ex;
                }
#endif
                break;
        }

        MDrv_PQ_AddTable_(MAINEX, &PQTableInfoEx);
        if(!bSkipCommTable)
        {
            MDrv_PQ_LoadCommTable_(MAINEX);
        }
        if((MS_U16)QM_INPUTTYPE_NUM_Main != (MS_U16)QM_INPUTTYPE_NUM_Main_Ex)
        {
            printf("PQ INPUTTYPE NUM MISMATCH: MAIN != EX");
            MS_ASSERT(0);
        }
#endif
        return TRUE;
#else
    UNUSED(u16DisplayWidth);
    UNUSED(enDisplaType);
    UNUSED(bSkipCommTable);
    return FALSE;
#endif
}


MS_BOOL MDrv_PQ_GetBPPInfoFromMADi(PQ_WIN eWindow,
                                              MS_BOOL bMemFmt422,
                                              MS_U16 u16TabIdx_MADi,
                                              MS_U16 u16TabIdx_MemFormat,
                                              MS_U8 *pu8BitsPerPixel,
                                              PQ_DEINTERLACE_MODE *peDeInterlaceMode,
                                              MS_U8 *pu8FrameCount
                                              )
{
    _u16PQDelyaTime= 0;
#if (CHIP_ID==MST9U3)
    *pu8FrameCount = 3;
#else
    *pu8FrameCount = 4;
#endif

    if (eWindow == PQ_MAIN_WINDOW && u16TabIdx_MADi == 0)
    {
        *pu8BitsPerPixel = 16;

        return TRUE;
    }
    if ((eWindow == PQ_MAIN_WINDOW && ( u16TabIdx_MADi == PQ_IP_MADi_25_14F_6R_Main || u16TabIdx_MADi == PQ_IP_MADi_25_14F_6R_NW_Main ||
        u16TabIdx_MADi == PQ_IP_MADi_25_14F_8R_NW_Main ))
        || (eWindow == PQ_SUB_WINDOW && (u16TabIdx_MADi == PQ_IP_MADi_25_14F_6R_NW_Sub || u16TabIdx_MADi == PQ_IP_MADi_25_14F_8R_NW_Sub
            ||u16TabIdx_MADi == PQ_IP_MADi_25_14F_6R_Sub)))
    {
        *pu8BitsPerPixel = 16;
        *pu8FrameCount = 14;
        return TRUE;
    }
    if(bMemFmt422)
    {
        // decide bits/pixel and deinterlace mode flag
        if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_4R))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            _u16PQDelyaTime= 60;
            *pu8FrameCount = 8;
            PQMADi_DBG(printf("24_4R PQ_DEINT_3DDI_HISTORY \n"));
        }
        else if( (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_2R)) ||
            (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_2R_880)))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            _u16PQDelyaTime= 60;
            PQMADi_DBG(printf("24_2R PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            _u16PQDelyaTime= 60;
#if (CHIP_ID==MST9U3)
                *pu8FrameCount = 6;
#else
                *pu8FrameCount = 8;
#endif
            PQMADi_DBG(printf("25 4R PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            _u16PQDelyaTime= 60;
            *pu8FrameCount = 8;
            PQMADi_DBG(printf("25 4R MC_PQ_DEINT_3DDI_HISTORY\n"));
            /*
            if(MApi_XC_GetMirrorModeType() != MIRROR_NORMAL)
            {
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                printf("!!Error: Cannot set MS_MADI_25_4R_MC(4 fields mode) for Mirror(at least 6 fields). Fix your PQ excel!!\n");
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }*/
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW)? TRUE: FALSE, MS_MADI_25_4R_MC_NW))
        {
            *pu8FrameCount = 8;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_MC))
        {
            *pu8BitsPerPixel = 24;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R))   //$$$ New for testing
        {
            *pu8BitsPerPixel = 24;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_14F_6R_MC))  //Add New MADi mode
        {
            *pu8BitsPerPixel = 24;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 14;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 14F 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_14F_8R))  //Add New MADi mode
        {
            *pu8BitsPerPixel = 24;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 14;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 14F 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_14F_8R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 14;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 14F 8R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_12F_8R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 12;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 12F 8R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_16F_8R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 16;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 16F 8R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_16F_6R_MC))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 16;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 16F 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_MC_NW))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *pu8FrameCount = 8;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 6R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_FilmPreDet))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            _u16PQDelyaTime= 60;
            PQMADi_DBG(printf("25 6R FilmPreDet_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_6R_FilmPreDet_PAL))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            _u16PQDelyaTime= 60;
            PQMADi_DBG(printf("25 6R FilmPreDet PAL_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_6R_FilmPreDet))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            _u16PQDelyaTime= 60;
            PQMADi_DBG(printf("24 6R FilmPreDet_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_6R_FilmPreDet_PAL))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            _u16PQDelyaTime= 60;
            PQMADi_DBG(printf("24 6R FilmPreDet PAL_PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_8F_4R_MC))
        {
            *pu8BitsPerPixel = 24;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            *pu8FrameCount = 8;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("25 8F 4R MC_PQ_DEINT_3DDI_HISTORY\n"));
        }
#if PQ_ENABLE_RFBL
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_2D)) // Need to ReDefine
        {
            *pu8BitsPerPixel = 24;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_25D)) // Need to ReDefine
        {
            *pu8BitsPerPixel = 24;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D)) // Need to ReDefine
        {
            *pu8BitsPerPixel = 24;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D_YC))
        {
            *pu8BitsPerPixel = 24;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_3D_FILM))
        {
            *pu8BitsPerPixel = 24;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
        }
#endif
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            _u16PQDelyaTime= 60;
            PQMADi_DBG(printf("25 2 PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_26_4R))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            _u16PQDelyaTime= 60;
            *pu8FrameCount = 8;
            PQMADi_DBG(printf("26 4 PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_26_2R))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            _u16PQDelyaTime= 60;
            PQMADi_DBG(printf("26 2 PQ_DEINT_3DDI_HISTORY\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_27_4R))
        {
            *pu8BitsPerPixel = 16;
            *peDeInterlaceMode = PQ_DEINT_3DDI;
            _u16PQDelyaTime= 60;
            *pu8FrameCount = 8;
            PQMADi_DBG(printf("27 4 PQ_DEINT_3DDI\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_27_2R))
        {
            *pu8BitsPerPixel = 16;
            *peDeInterlaceMode = PQ_DEINT_3DDI;
            _u16PQDelyaTime= 60;
            PQMADi_DBG(printf("27 2 PQ_DEINT_3DDI\n"));
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8))
        {
            *pu8BitsPerPixel = 16;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 40;
            PQMADi_DBG(printf("P 8 PQ_DEINT_2DDI_BOB\n"));
            //*pu8FrameCount = 2;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE10))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 40;
            PQMADi_DBG(printf("P 10 PQ_DEINT_2DDI_BOB\n"));
            //*pu8FrameCount = 2;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT8))
        {
            *pu8BitsPerPixel = 20;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 40;
            PQMADi_DBG(printf("P M 8 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 4;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT10))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 40;
            PQMADi_DBG(printf("P M 10 PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 4;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_RFBL_FILM))
        {
            *pu8BitsPerPixel = 12;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //printf("====================================   MS_MADI_24_RFBL_FILM\n");
            //printf("24 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_24_RFBL_NFILM))
        {
            *pu8BitsPerPixel = 12;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_3DDI_HISTORY;
            //printf("====================================   MS_MADI_24_RFBL_NFILM\n");
            //printf("24 PQ_DEINT_3DDI_HISTORY\n");
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_FBL_DNR))
        {
            *pu8BitsPerPixel = 16;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            //printf("====================================   MS_MADI_FBL_DNR\n");
            //printf("24 PQ_DEINT_2DDI_AVG\n");
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_FBL_MIU))
        {
            *pu8BitsPerPixel = 16;
            _u16PQDelyaTime= 60;
            *peDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            //printf("====================================   MS_MADI_FBL_MIU\n");
            //printf("24 PQ_DEINT_2DDI_AVG\n");
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_NO_MIU))
        {
            *pu8BitsPerPixel = 16;
            _u16PQDelyaTime= 40;
            *peDeInterlaceMode = PQ_DEINT_2DDI_AVG;
            //printf("====================================   MS_MADI_P_MODE8_NO_MIU\n");
            //printf("24 PQ_DEINT_2DDI_AVG\n");
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT10_8Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P M 10 8Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 8;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MOT10_4Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE, MAIN_WINDOW);
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P M 10 4Frame PQ_DEINT_2DDI_BOB\n"));
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_3Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P M 10 3Frame PQ_DEINT_2DDI_BOB\n"));
            //*pu8FrameCount = 3;
            /*
            if((MApi_XC_GetMirrorModeType() != MIRROR_NORMAL) &&
               (QM_GetInputVFreq(eWindow) > 235) && (QM_GetInputVFreq(eWindow) < 245))
            {
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                printf("!!Error: Cannot set MS_MADI_P_MODE_MC_3Frame for mirror 24p(at least 4 frame number). Fix your PQ excel!!\n");
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }*/
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_4Frame))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            //MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            PQMADi_DBG(printf("P M 10 3Frame PQ_DEINT_2DDI_BOB\n"));
            *pu8FrameCount = 4;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_6Frame_6R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            //MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            *pu8FrameCount = 6;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_8Frame_6R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            //MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            *pu8FrameCount = 8;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_12Frame_8R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            //MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            *pu8FrameCount = 12;
        }
        else if (u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE_MC_16Frame_8R))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 125; //When MC mode and read/write bank mapping mode set 2, have to delay 125 ms .
            //MApi_XC_Set_OPWriteOffEnable(DISABLE,MAIN_WINDOW);
            *pu8FrameCount = 16;
        }
#if PQ_ENABLE_RFBL
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE10_MOT))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 40;
            PQMADi_DBG(printf("RFBL P M 10 PQ_DEINT_2DDI_BOB\n"));
            //*pu8FrameCount = 2;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_RFBL_P_MODE8_MOT))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            _u16PQDelyaTime= 40;
            PQMADi_DBG(printf("RFBL P M 8 PQ_DEINT_2DDI_BOB\n"));
            //*pu8FrameCount = 2;
        }
#endif
        else
        {
            MS_ASSERT(0);
            PQMADi_DBG(printf("None\n"));
        }

    }
    else
    {
        //in order to disable user mode,
        if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_444))
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            PQMADi_DBG(printf("P 8 444 PQ_DEINT_2DDI_BOB\n"));
            //*pu8FrameCount = 2;
        }
        else if(u16TabIdx_MADi == Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE10_444))
        {
            *pu8BitsPerPixel = 30;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            PQMADi_DBG(printf("P 10 444 PQ_DEINT_2DDI_BOB\n"));
            //*pu8FrameCount = 2;
        }

        if((u16TabIdx_MemFormat == PQ_IP_MemFormat_444_10BIT_Main)
#if PQ_ENABLE_PIP
            ||(u16TabIdx_MemFormat == PQ_IP_MemFormat_444_10BIT_Sub)
#endif
            )
        {
            *pu8BitsPerPixel = 32;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            PQMADi_DBG(printf("PQ_IP_MemFormat_444_10BIT\n"));
#if (CHIP_ID==MST9U3)
            *pu8FrameCount = 3;
#else
            *pu8FrameCount = 4;
#endif
        }
        else if((u16TabIdx_MemFormat == PQ_IP_MemFormat_444_8BIT_Main)
#if PQ_ENABLE_PIP
            ||(u16TabIdx_MemFormat == PQ_IP_MemFormat_444_8BIT_Sub)
#endif
            )
        {
            *pu8BitsPerPixel = 24;
            *peDeInterlaceMode = PQ_DEINT_2DDI_BOB;
            PQMADi_DBG(printf("PQ_IP_MemFormat_444_8BIT\n"));
#if (CHIP_ID==MST9U3)
            *pu8FrameCount = 3;
#else
            *pu8FrameCount = 4;
#endif
        }
        else
        {
            *pu8BitsPerPixel = 24;
            PQMADi_DBG(printf("PQ_IP_MemFormat_422MF_Main\n"));
            //*pu8FrameCount = 2;
        }

        if ( MApi_XC_R2BYTE(REG_SC_BK2A_02_L) & BIT(15) )   //if on need disable.
        {
#if(ENABLE_PQ_MLOAD)
            MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
#if PQ_UC_CTL
            MDrv_PQ_LoadTable(eWindow, PQ_IP_UC_CTL_OFF_Main, PQ_IP_UC_CTL_Main);
#endif
#if(ENABLE_PQ_MLOAD)
            MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif
        }
    }

    if((eWindow == PQ_SUB_WINDOW) && (_stMode_Info[eWindow].bInterlace) /*&& (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_MODE_NONE)*/)
    {
        PQMADi_DBG(printf("sub window to set 6 or more fields mode for interlace in non-3D case to fix sub non-fpll problem\n"));
        *pu8FrameCount = (*pu8FrameCount > 6) ? *pu8FrameCount : 6;
    }

    return TRUE;
}


MS_BOOL MDrv_PQ_GetMemFmtInGeneral(PQ_WIN eWindow,
                                            MS_BOOL bMemFmt422,
                                            MS_U16 *pu16TabIdx_MemFormat,
                                            MS_U16 *pu16TabIdx_444To422,
                                            MS_U16 *pu16TabIdx_422To444
                                            )
{
    if(bMemFmt422)
    {
        if(eWindow == PQ_MAIN_WINDOW)
        {
            *pu16TabIdx_MemFormat = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MemFormat_Main);
            *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Main;
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx((MS_422To444_TYPE)Hal_PQ_get_422To444_mode(bMemFmt422));
        }
#if PQ_ENABLE_PIP
        else
        {
            *pu16TabIdx_MemFormat = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MemFormat_Sub);
            *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Sub;
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx((MS_422To444_TYPE)Hal_PQ_get_422To444_mode(bMemFmt422));
        }
#endif
    }
    else
    {
        // use 444_10BIT mode only if < 1440x1080
        // frame buffer 1920x1080x3x2 == 1440x1080x4x2
        // also bandwidth not enough for 1920x1080 444_10BIT
        if(eWindow == PQ_MAIN_WINDOW)
        {
            *pu16TabIdx_MemFormat = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MemFormat_Main);
            *pu16TabIdx_444To422 = PQ_IP_444To422_OFF_Main;
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx((MS_422To444_TYPE)Hal_PQ_get_422To444_mode(bMemFmt422));
        }
#if PQ_ENABLE_PIP
        else
        {
            *pu16TabIdx_MemFormat = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MemFormat_Sub);
            *pu16TabIdx_444To422 = PQ_IP_444To422_OFF_Sub;
            *pu16TabIdx_422To444 = Hal_PQ_get_422To444_idx((MS_422To444_TYPE)Hal_PQ_get_422To444_mode(bMemFmt422));
        }
#endif
    }

    return TRUE;
}


MS_BOOL MDrv_PQ_GetMADiInGeneral(PQ_WIN eWindow, MS_U16 *pu16TabIdx_MADi_Motion, MS_U16 *pu16TabIdx_MADi)
{
    UNUSED(pu16TabIdx_MADi_Motion);
    UNUSED(pu16TabIdx_MADi);
#if PQ_GRULE_DDR_SELECT_ENABLE
    if(eWindow == PQ_MAIN_WINDOW)
    {
         MS_U16 u16PQ_DDRSELIdx = 0;
         if(bw_info.u32Miu1MemSize==0) // 1 DDR
         {
              PQMADi_DBG(printf("MDrv_PQ_GetMADiInGeneral: 1 DDR \n"));
              u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_1DDR_Main);
              *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 0); // only MADi ip
         }
         else // 2 DDR
         {
              PQMADi_DBG(printf("MDrv_PQ_GetMADiInGeneral: 2 DDR \n"));
              u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_2DDR_Main);
              *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 0); // only MADi ip
         }
    }
    else
    {
         *pu16TabIdx_MADi = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MADi_Sub);
    }
#else //DDR select disable
    {
        if(eWindow == PQ_MAIN_WINDOW)
        {
            *pu16TabIdx_MADi = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MADi_Main);
        }
#if PQ_ENABLE_PIP
        else
        {
            *pu16TabIdx_MADi = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_MADi_Sub);

        }
#endif
       //printf("$$[%s]:%d, %s, u16TabIdx_MADi=%u\n", __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", u16TabIdx_MADi);
    }
#endif

    return TRUE;
}

#if PQ_TB_FOR_TV
MS_BOOL MDrv_PQ_GetMADiFromGrule(PQ_WIN eWindow, MS_U16 *pu16TabIdx_MADi_Motion, MS_U16 *pu16TabIdx_MADi)
{
#if PQ_GRULE_OSD_BW_ENABLE
    MS_U16 u16PQ_OSDBWIdx = 0;

    if(eWindow == PQ_MAIN_WINDOW)
    {
    #if PQ_GRULE_DDR_SELECT_ENABLE
        {
            if(bw_info.u32Miu1MemSize ==0) // 1 DDR
            {
                u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_1DDR_On_Main);
                *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
            }
            else // 2 DDR
            {
                u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_2DDR_On_Main);
                *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
            }
        }
    #else //DDR select disable
        {
            u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex(eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_Main);
            *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
        }
    #endif

    #if (PQ_GRULE_OSD_BW_IP_NUM_Main > 1)
        *pu16TabIdx_MADi_Motion = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 1); // Add MADi_Motion ip
    #else
        UNUSED(pu16TabIdx_MADi_Motion);
    #endif
    }
    else
    {
        // TODO: Should sub window need to consider the OSD BW ??
    }
#else
    UNUSED(eWindow);
    UNUSED(pu16TabIdx_MADi_Motion);
    UNUSED(pu16TabIdx_MADi);
#endif

    return TRUE;
}

MS_BOOL MDrv_PQ_GetMADiForRFBL(PQ_WIN eWindow, MS_BOOL bFBL, MS_U16 *pu16TabIdx_MADi_Motion, MS_U16 *pu16TabIdx_MADi)
{
    UNUSED(pu16TabIdx_MADi_Motion);
    if((_stRFBL_Info.bEnable)&&(!bFBL))
    {
        *pu16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, (MS_MADI_TYPE)_stRFBL_Info.u8MADiType);
        if(!_stMode_Info[eWindow].bInterlace)
        {
            *pu16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_NO_MIU);

            if(eWindow == PQ_MAIN_WINDOW)
            {
                MDrv_PQ_MADiForceMotionY(PQ_MAIN_WINDOW, FALSE);
                MDrv_PQ_MADiForceMotionC(PQ_MAIN_WINDOW, FALSE);
            }
            else
            {
                MDrv_PQ_MADiForceMotionY(PQ_SUB_WINDOW, FALSE);
                MDrv_PQ_MADiForceMotionC(PQ_SUB_WINDOW, FALSE);
            }
            //printf("[%s]-[%d] :  disable Y/C motion\n", __FUNCTION__, __LINE__);
        }
        //printf("[%s]-[%d] : u16TabIdx_MADi = %d\n",__FUNCTION__,__LINE__,u16TabIdx_MADi);
    }

    return TRUE;
}

MS_BOOL MDrv_PQ_Patch2Rto4RForFieldPackingMode(PQ_WIN eWindow, MS_U16 u16TabIdx_MADi)
{
#if PQ_GRULE_OSD_BW_ENABLE
    // Patch: in OSD turn off case, when 2R-->4R, J2, M10 with new OPM system will flash(4R-->2R is OK)
    // we need to set 4R first, and wait some time, and do other MAdi setting, the flash will be ok.
    // which is only for non-OS platform patch, since OS platform won't turn off OSD for DFB system.
    // Only FieldPackingMode need this solution
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if((MApi_XC_IsFieldPackingModeSupported() &&
        (_stMode_Info[eWindow].u16input_hsize>=1280)&&
        (_stMode_Info[eWindow].u16input_vsize>=720) &&
        (!_bOSD_On)) &&
        ((u16TabIdx_MADi == PQ_IP_MADi_24_4R_Main) || (u16TabIdx_MADi == PQ_IP_MADi_25_4R_Main) ||
        (u16TabIdx_MADi == PQ_IP_MADi_26_4R_Main) || (u16TabIdx_MADi == PQ_IP_MADi_27_4R_Main)
#if PQ_MADI_88X_MODE
        || (u16TabIdx_MADi == PQ_IP_MADi_24_4R_880_Main)
        || (u16TabIdx_MADi == PQ_IP_MADi_25_4R_880_Main)
        || (u16TabIdx_MADi == PQ_IP_MADi_25_4R_884_Main)
#endif
        ))
        {
            //set to 4R first,
            MApi_XC_W2BYTEMSK(REG_SC_BK12_03_L, 0 , 0x0002 );
            //delay
            _MDrv_PQ_wait_output_vsync(eWindow, 1, 90);
        }
    }
#else
    UNUSED(eWindow);
    UNUSED(u16TabIdx_MADi);
#endif
    return TRUE;
}

MS_BOOL MDrv_PQ_ChangeMemConfigFor3D(PQ_WIN eWindow,
                                            MS_BOOL bMemFmt422,
                                            MS_U16 *pu16TabIdx_MemFormat,
                                            MS_U16 *pu16TabIdx_444To422,
                                            MS_U16 *pu16TabIdx_422To444,
                                            MS_U8 *pu8BitsPerPixel,
                                            MS_U16 *pu16TabIdx_MADi_Motion,
                                            MS_U16 *pu16TabIdx_MADi
                                            )
{
    UNUSED(pu16TabIdx_MADi_Motion);
    if(eWindow == PQ_MAIN_WINDOW)
    {
        //reduce mem size, put it into 16M, 2 frame mode.
        if((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING)
            && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
            && (_stMode_Info[eWindow].u16input_hsize == 1920)
            && (_stMode_Info[eWindow].u16input_vsize == 2205))
        {
#if PQ_EN_UCNR_3D_MADI
            *pu16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_2BYTE_3D);
#else
            *pu16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_444);
#endif
            *pu8BitsPerPixel = 16;

            // DVI is 444 input, we need to conver it to 422, otherwise DVI will use 32 bpp instead, not 16
            // because 444 format combine to the setting of pu16TabIdx_MemFormat
            if(!bMemFmt422)
            {
                //PQ_IP_MemFormat_Main 422
                *pu16TabIdx_MemFormat = PQ_IP_MemFormat_422MF_Main;
                *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Main;
                //PQ_IP_422To444_Main on
                *pu16TabIdx_422To444 = PQ_IP_422To444_ON_Main;
                PQBW_DBG(printf("3D: force 422, 444to422 and 422to444 ON\n");)
            }

            printf("3D: modify progressive memory fmt for 1080p framepacking\n");
        }

        if(MApi_XC_Get_3D_HW_Version() < 2)
        {
            //PQ main page maybe MADi mode to 8 field mode, but it cannot do madi for line by line output.
            //so we force it back to normal 25_2R like mode.
            if((MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_TOP_BOTTOM) && (TRUE == _stMode_Info[eWindow].bInterlace))
            {
                // we must use MS_MADI_25_4R or 2R to avoid MC madi mode for dnr offset calcuation problem.
                if(TRUE == _stMode_Info[eWindow].bInterlace)
                {
                    if((_stMode_Info[eWindow].u16input_vsize >= 900) &&
                        (_stMode_Info[eWindow].u16input_hsize >= 1440) &&
                        (bw_info.u32Miu1MemSize==0))// 1ddr case, should save bw
                    {
                        *pu16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R);
                        PQBW_DBG(printf("3D: force to 25_2R\n");)
                    }
                    else
                    {
                        *pu16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_4R);
                        PQBW_DBG(printf("3D: force to 25_4R\n");)
                    }
                }

                //Madi do line by line must use 422 format
                if(!bMemFmt422)
                {
                    //PQ_IP_MemFormat_Main 422
                    *pu16TabIdx_MemFormat = PQ_IP_MemFormat_422MF_Main;
                    *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Main;
                    //PQ_IP_422To444_Main on
                    *pu16TabIdx_422To444 = PQ_IP_422To444_ON_Main;
                    PQBW_DBG(printf("3D: force 422, 444to422 and 422to444 ON\n");)
                }

                // we can only process 422 format for E_XC_3D_INPUT_TOP_BOTTOM.
                // so, if it's 444 format, we have to force it to be 422 and the u8BitsPerPixel is also changed to 24.
                *pu8BitsPerPixel = 24;
                PQBW_DBG(printf("3D:force u8BitsPerPixel to 24\n");)
            }

            //frame alterntive must use bob mode
            if((E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC == MApi_XC_Get_3D_Output_Mode()) ||
               (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode()))
            {
                MDrv_PQ_EnableMADIForce(PQ_MAIN_WINDOW, TRUE);
#if PQ_MADI_DFK
                MDrv_PQ_LoadTable(eWindow, PQ_IP_MADi_DFK_OFF_Main, PQ_IP_MADi_DFK_Main);
#endif
                MDrv_PQ_LoadTable(eWindow, PQ_IP_EODi_OFF_Main, PQ_IP_EODi_Main);
            }
            //line by line must use weave mode
            else if(E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
            {
                MDrv_PQ_EnableMADIForce(eWindow, FALSE);
            }

            //None 3D case: by Ethan.Lee, saveing memory size for 1920x1200
            if ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_MODE_NONE) &&
                (MApi_XC_Get_3D_Output_Mode()== E_XC_3D_OUTPUT_MODE_NONE) )
            {
                // Saving memory size for 1920x1200P HDMI 20110421EL
                if (  (_stMode_Info[eWindow].u16input_vsize > 1100) && (QM_IsSourceHDMI_Video(_enInputSourceType[eWindow], eWindow)))
                {
                    *pu16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_P_MODE8_444);
                    *pu8BitsPerPixel = 16;
                    if(!bMemFmt422)
                    {
                        //PQ_IP_MemFormat_Main 422
                        *pu16TabIdx_MemFormat = PQ_IP_MemFormat_422MF_Main;
                        *pu16TabIdx_444To422 = PQ_IP_444To422_ON_Main;
                        //PQ_IP_422To444_Main on
                        *pu16TabIdx_422To444 = PQ_IP_422To444_ON_Main;
                        PQBW_DBG(printf("3D: force 422, 444to422 and 422to444 ON\n");)
                    }
                }
            }
        }

        //Frame packing interlace must use 25_2R for
        // A. Madi for line by line output for old 3D engine. Or,
        // B. better quality. (P mode will shaking a little)
        if(((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_L) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_R) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_TOP_BOTTOM) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_LINE_ALTERNATIVE) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            || ((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING) && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE) && (_stMode_Info[eWindow].u16input_vsize == 2228))
            )
        {
            //framepacking interlace 1080i
            *pu16TabIdx_MADi = Hal_PQ_get_madi_idx((eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, MS_MADI_25_2R);
            _stMode_Info[eWindow].bInterlace = TRUE;
            *pu8BitsPerPixel = 24;

            PQBW_DBG(printf("3D: force to 25_2R\n");)
        }
    }
    return TRUE;
}

MS_BOOL MDrv_PQ_ForceBPPForDynamicMemFmt(PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_U8 *pu8BitsPerPixel)
{
    //force Memory FMT and use the same pixel number to avoid the garbage when memory mode is changed.
    if(bMemFmt422)
    {
        MS_U16 u16MemFMT= 0;

        if(!_stMode_Info[eWindow].bInterlace)
        {
            // for dynamically memory format changing, we need to keep it as 24 bits
            if(SUPPORT_DYNAMIC_MEM_FMT)//STB could be false
            {
                //Please refer to "MDrv_PQ_ChangeMemConfigFor3D"
                //reduce mem size, put it into 16M, 2 frame mode.
                if(!((MApi_XC_Get_3D_Input_Mode((SCALER_WIN)eWindow) == E_XC_3D_INPUT_FRAME_PACKING)
                    && (MApi_XC_Get_3D_Output_Mode() == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
                    && (_stMode_Info[eWindow].u16input_hsize == 1920)
                    && (_stMode_Info[eWindow].u16input_vsize == 2205)))
                *pu8BitsPerPixel = 24;
            }
        }

        if (eWindow == PQ_MAIN_WINDOW)
        {
            u16MemFMT = MApi_XC_R2BYTEMSK(REG_SC_BK12_02_L, 0xFF);
        }
        else
        {
            u16MemFMT = MApi_XC_R2BYTEMSK(REG_SC_BK12_42_L, 0xFF);
        }
        if((u16MemFMT == 0xBB) || (u16MemFMT == 0xAA) || (u16MemFMT == 0x99) || (u16MemFMT == 0x88))
        {
                *pu8BitsPerPixel = 24;
        }
        else if((u16MemFMT == 0x55) || (u16MemFMT == 0x66) || (u16MemFMT == 0x44))
        {
                *pu8BitsPerPixel = 20;
        }
        else
        {
            //decide by original setting
        }
    }
    return TRUE;
}

MS_BOOL MDrv_PQ_SuggestFrameNum(PQ_WIN eWindow, MS_U16 u16TabIdx_MADi, MS_U8 u8FrameCount)
{
    //the chips, who are older than SCMI_V2, set frame count reg by Qmap.
    //the chips, who are SCMI_V2, the frame count is suggested by Qmap, but sw code judge and replace the value.
    //      and then, set to hw reg
    //note: SUPPORT_SCMI_V2 is corresponding withe utopia flag: _FIELD_PACKING_MODE_SUPPORTED
    //      we can remove SUPPORT_SCMI_V2 later
    if(MApi_XC_IsFieldPackingModeSupported())
    {
        //Set Frame number
        UNUSED(u16TabIdx_MADi);
        if(bSetFrameCount )
        {
            _u16PQSuggestedFrameNum[eWindow] = u8FrameCount;
            printf("[%s %d] _u16PQSuggestedFrameNum[%u] = %u\n", __FILE__, (int)__LINE__, eWindow, u8FrameCount);
        }
    }
    return TRUE;
}
MS_BOOL MDrv_PQ_SetGameMode(PQ_WIN eWindow, MS_BOOL bEnable)
{
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;

    #if(ENABLE_PQ_MLOAD == 0)
        printf("*** [Game mode] please enable PQ Mload \n");
    #endif

    #if PQ_UC_CTL
    MS_U16 u16_UCDI_IDX;
    MS_U16 u16_UcCTL_IDX;
    #endif

    MS_U8  u8RWmappingNum = 0;
    _bGameModeOn[eWindow] = bEnable;

    if(bEnable)
    {
        if(_bRWNumIsReset[eWindow])
        {
            if (eWindow == PQ_MAIN_WINDOW)
            {
                _u8RWmappingNum[eWindow] = MApi_XC_R2BYTEMSK(REG_SC_BK12_07_L, BIT(15)|BIT(14)) >> 14;
            }
            else
            {
                _u8RWmappingNum[eWindow] = MApi_XC_R2BYTEMSK(REG_SC_BK12_47_L, BIT(15)|BIT(14)) >> 14;
            }
            _bRWNumIsReset[eWindow] = FALSE;
        }

        // set the R/W bank mapping
        if (_stMode_Info[eWindow].bInterlace
        && (_stMode_Info[eWindow].u16input_vtotal <= 750)
        && (QM_IsSourceYPbPr(_enInputSourceType[eWindow]) || QM_IsSourceSV(_enInputSourceType[eWindow]) || QM_IsSourceAV(_enInputSourceType[eWindow])))
        {
            u8RWmappingNum = 0x01;
        }
        else
            u8RWmappingNum = _u8RWmappingNum[eWindow];

        MDrv_PQ_LoadNRTable(eWindow, PQ_3D_NR_OFF);

        //------ UC part ----
        #if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
        #endif

        #if PQ_UC_CTL
        if(eWindow == PQ_MAIN_WINDOW )
        {
            MDrv_PQ_LoadTable(eWindow, PQ_IP_UCDi_OFF_Main, PQ_IP_UCDi_Main);
        }
        #endif

        //For the mutex consideration, MDrv_PQ_Set_MLoadEn(True)(False) can only protect once MDrv_PQ_LoadTable()
        #if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
        #endif

        #if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
        #endif

        #if PQ_UC_CTL
        if(eWindow == PQ_MAIN_WINDOW )
        {
            MDrv_PQ_LoadTable(eWindow, PQ_IP_UC_CTL_OFF_Main, PQ_IP_UC_CTL_Main);
        }
        #endif

        #if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
        #endif

    }
    else
    {
        // Restore original value
        u8RWmappingNum = _u8RWmappingNum[eWindow];

        MDrv_PQ_LoadNRTable(eWindow, PQ_3D_NR_AUTO);

        //------ UC part ----
        #if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
        #endif

        #if PQ_UC_CTL
        if(eWindow == PQ_MAIN_WINDOW )
        {
            u16_UCDI_IDX= MDrv_PQ_GetTableIndex(eWindow, PQ_IP_UCDi_Main);

            MDrv_PQ_LoadTable(eWindow, u16_UCDI_IDX, PQ_IP_UCDi_Main);
        }
        #endif

        #if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
        #endif

        //For the mutex consideration, MDrv_PQ_Set_MLoadEn(True)(False) can only protect once MDrv_PQ_LoadTable()
        #if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
        #endif

        #if PQ_UC_CTL
        if(eWindow == PQ_MAIN_WINDOW )
        {
            u16_UcCTL_IDX = MDrv_PQ_GetTableIndex(eWindow, PQ_IP_UC_CTL_Main);

            MDrv_PQ_LoadTable(eWindow, u16_UcCTL_IDX , PQ_IP_UC_CTL_Main);
        }
        #endif

        #if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
        #endif

    }

    if (eWindow == PQ_MAIN_WINDOW)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK12_07_L, u8RWmappingNum<<14, BIT(15)|BIT(14));
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK12_47_L, u8RWmappingNum<<14, BIT(15)|BIT(14));
    }

    return TRUE;
}
MS_BOOL MDrv_PQ_GetGameMode_Status(PQ_WIN eWindow)
{
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;
    else
        return _bGameModeOn[eWindow];
}

MS_BOOL MDrv_PQ_SetBypassMode(PQ_WIN eWindow, MS_BOOL bEnable)
{
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;

    _bBypassModeOn[eWindow] = bEnable;
    return TRUE;
}

MS_BOOL MDrv_PQ_GetBypassModeStatus(PQ_WIN eWindow)
{
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;
    else
        return _bBypassModeOn[eWindow];
}
#endif
char*  MDrv_PQ_GetVersion(PQ_WIN eWindow)
{
#if(ENABLE_PQ_BIN)

    char* PQ_Version;

    if(PQ_MAIN_WINDOW == eWindow)
    {
        PQ_Version = stPQBinHeaderInfo[PQ_BIN_STD_MAIN].u8Version;
    }
    else
    {
        PQ_Version = stPQBinHeaderInfo[PQ_BIN_STD_SUB].u8Version;
    }
    return PQ_Version;

#else
    UNUSED(eWindow);
    return NULL;
#endif

}

MS_U16 QM_InputSourceToIndex(PQ_WIN eWindow, MS_U8 enInputSourceType)
{
    MS_U16 u16SrcType = 0;
    MS_U16 u16Input_HSize = QM_GetInputHSize(eWindow);
    MS_U16 u16Input_VSize = QM_GetInputVSize(eWindow);
    MS_U16 u16Input_VFreq = QM_GetInputVFreq(eWindow);
    MS_BOOL bIsInterlaced = QM_IsInterlaced(eWindow);

#if (PQ_QM_CVBS)
    PQ_VIDEOSTANDARD_TYPE eStandard;
#endif

#if (PQ_QM_HDMI_PC) || (PQ_QM_PC)
    MS_U16 u16Disp_HSize = QM_GetDispHSize(eWindow);
    MS_U16 u16Disp_VSize = QM_GetDispVSize(eWindow);

    PQMODE_DBG(printf("\r\n QM: Src = %u, H = %u, V = %u", enInputSourceType, u16Input_HSize, u16Input_VSize));
    PQMODE_DBG(printf("\r\n     VFreq = %u, IsInterlace = %u", u16Input_VFreq, (MS_U16)bIsInterlaced));
    PQMODE_DBG(printf("\r\n     DispH = %u, DispV = %u", u16Disp_HSize, u16Disp_VSize));
#endif

#if (PQ_QM_BYPASS_COLOR)

    //  Only when bypass mode is on .
    if (_bBypassModeOn[eWindow])
    {
        if (bIsInterlaced)
            u16SrcType = QM_BypassColor_interlace_Main;
        else
            u16SrcType = QM_BypassColor_progressive_Main;

        return u16SrcType;
    }
#endif

#if ENABLE_VGA_EIA_TIMING
    // Only Progressiv mode
    if ((QM_IsSourceVGA(enInputSourceType) || QM_IsSourceDVI_HDMIPC(enInputSourceType, eWindow)) && (!bIsInterlaced))
#else
    // Only DVI progressive mode, VGA could be interlace
    if (QM_IsSourceVGA(enInputSourceType) || (QM_IsSourceDVI_HDMIPC(enInputSourceType, eWindow) && !bIsInterlaced))
#endif
    {
        //DVI YUV mode is some special pattern gen timing in Korea company
        if(QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORYUV444(eWindow))
        {
#if (PQ_QM_HDMI_PC)
            // PC YUV444
            if (u16Input_HSize < u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hup_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hup_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_444_PC_Hup_Vno_Main;
                }
            }
            else if (u16Input_HSize > u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hdown_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hdown_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_444_PC_Hdown_Vno_Main;
                }
            }
            else
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hno_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hno_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_444_PC_Hno_Vno_Main;
                }
            }
#endif
        }
        else if (QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORYUV422(eWindow))
        {
#if (PQ_QM_HDMI_PC)
            // PC YUV422
            if (u16Input_HSize < u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hup_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hup_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_422_PC_Hup_Vno_Main;
                }
            }
            else if (u16Input_HSize > u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hdown_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hdown_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_422_PC_Hdown_Vno_Main;
                }
            }
            else
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hno_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hno_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_422_PC_Hno_Vno_Main;
                }
            }
#endif
        }
        else //(QM_HDMIPC_COLORRGB(eWindow)) & default
        {
#if (PQ_QM_PC)
            // PC RGB
            if (u16Input_HSize < u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Main;
                }
            }
            else if (u16Input_HSize > u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Main;
                }
            }
            else
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Main;
                }
            }
#endif
        }
    }
    else if (QM_IsSourceHDMI(enInputSourceType)
#if ENABLE_VGA_EIA_TIMING
             || QM_IsSourceVGA(enInputSourceType) //interlace VGA comes here
#endif
            )
    {
#if (PQ_QM_HMDI)
#if 0//debug message
    if(QM_HDMIPC_COLORRGB(eWindow))
        printf("HDMI RGB\n");
    else if(QM_HDMIPC_COLORYUV444(eWindow))
        printf("HDMI YUV 444\n");
    else
        printf("HDMI YUV 422\n");
#endif
        if( QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORRGB(eWindow) &&
            (MDrv_PQ_Get_PointToPoint(eWindow)) && (!bIsInterlaced))
        {
            // HDMI RGB progressive
            u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Main;
        }
        else if( QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORYUV444(eWindow) &&
                 (MDrv_PQ_Get_PointToPoint(eWindow)) && (!bIsInterlaced))
        {
            // HDMI YUV444 progressive
            u16SrcType = QM_HDMI_444_PC_Hno_Vno_Main;
        }
        else if (QM_HDMIPC_COLORRGB(eWindow))
        {
            // HDMI
            if ((u16Input_HSize < 800) && (u16Input_VSize < 500))
            {
                if (bIsInterlaced)
                    u16SrcType = QM_HDMI_RGB_480i_Main;
                else
                    u16SrcType = QM_HDMI_RGB_480p_Main;
            }
            else if ((u16Input_HSize < 800) && (u16Input_VSize < 600))
            {
                if (bIsInterlaced)
                    u16SrcType = QM_HDMI_RGB_576i_Main;
                else
                    u16SrcType = QM_HDMI_RGB_576p_Main;
            }
            else if ((u16Input_HSize < 1300) && (u16Input_VSize < 800) && (!bIsInterlaced))
            {
                if (u16Input_VFreq > 550)
                {
                    u16SrcType = QM_HDMI_RGB_720p_60hz_Main;
                }
                else if(u16Input_VFreq > 250)
                {
                    u16SrcType = QM_HDMI_RGB_720p_50hz_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_RGB_720p_24hz_Main;
                }
            }
            else if (u16Input_HSize >= 3840)
            {
                u16SrcType = QM_HDMI_RGB_4K2K_Main;

            }
            else
            {
                if (bIsInterlaced)
                {
#if PQ_QM_HDMI_FP_VIDEO
                    if(u16Input_VSize > 2000) //1920x2228 i
                    {
                        u16SrcType = QM_HDMI_FP_1080i_Main;
                    }
                    else
#endif
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_HDMI_RGB_1080i_60hz_Main;
                    }
                    else
                    {
                        u16SrcType = QM_HDMI_RGB_1080i_50hz_Main;
                    }
                }
                else
                {
#if PQ_QM_HDMI_FP_VIDEO
                    if(u16Input_VSize > 2000) //1920x2205 p
                    {
                        u16SrcType = QM_HDMI_FP_1080p_Main;
                    }
                    else if(u16Input_VSize > 1200) //1280x1470p
                    {
                        u16SrcType = QM_HDMI_FP_720p_Main;
                    }
                    else
#endif
                    // 1500 is for framepacking case vsize 2205
                    if (u16Input_VFreq > 550 || u16Input_VSize > 1500)
                    {
                        u16SrcType = QM_HDMI_RGB_1080p_60hz_Main;
                    }
                    else if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_HDMI_RGB_1080p_50hz_Main;
                    }
                    else  // Other timing
                    {
                        u16SrcType = QM_HDMI_RGB_1080p_24hz_Main;
                    }
                }
            }
        }
        else
        {
            // HDMI YUV
            if ((u16Input_HSize < 800) && (u16Input_VSize < 500))
            {
                if (bIsInterlaced)
                    u16SrcType = QM_HDMI_480i_Main;
                else
                    u16SrcType = QM_HDMI_480p_Main;
            }
            else if ((u16Input_HSize < 800) && (u16Input_VSize < 600))
            {
                if (bIsInterlaced)
                    u16SrcType = QM_HDMI_576i_Main;
                else
                    u16SrcType = QM_HDMI_576p_Main;
            }
            else if ((u16Input_HSize < 1300) && (u16Input_VSize < 800) && (!bIsInterlaced))
            {
                if (u16Input_VFreq > 550)
                {
                    u16SrcType = QM_HDMI_720p_60hz_Main;
                }
                else if(u16Input_VFreq > 250)
                {
                    u16SrcType = QM_HDMI_720p_50hz_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_720p_24hz_Main;
                }
            }
            else if (u16Input_HSize >= 3840)
            {
                u16SrcType = QM_HDMI_4K2K_Main;

            }
            else
            {
                if (bIsInterlaced)
                {
#if PQ_QM_HDMI_FP_VIDEO
                    if(u16Input_VSize > 2000) //1920x2228 i
                    {
                        u16SrcType = QM_HDMI_FP_1080i_Main;
                    }
                    else
#endif
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_HDMI_1080i_60hz_Main;
                    }
                    else
                    {
                        u16SrcType = QM_HDMI_1080i_50hz_Main;
                    }
                }
                else
                {
#if PQ_QM_HDMI_FP_VIDEO
                    if(u16Input_VSize > 2000) //1920x2205 p
                    {
                        u16SrcType = QM_HDMI_FP_1080p_Main;
                    }
                    else if(u16Input_VSize > 1200) //1280x1470p
                    {
                        u16SrcType = QM_HDMI_FP_720p_Main;
                    }
                    else
#endif
                    // 1500 is for framepacking case vsize 2205
                    if (u16Input_VFreq > 550 || u16Input_VSize > 1500)
                    {
                        u16SrcType = QM_HDMI_1080p_60hz_Main;
                    }
                    else if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_HDMI_1080p_50hz_Main;
                    }
                    else  // Other timing
                    {
                        u16SrcType = QM_HDMI_1080p_24hz_Main;
                    }
                }
            }
        }
#endif
    }
    else if (QM_IsSourceYPbPr(enInputSourceType))
    {
#if (PQ_QM_YPBPR)
        if( QM_IsSourceYPbPr(enInputSourceType) &&
            (MDrv_PQ_Get_PointToPoint(eWindow)) && (!bIsInterlaced))
        {
            // YUV444 progressive
            u16SrcType = QM_HDMI_444_PC_Hno_Vno_Main;
        }
        else if (QM_IsYPbPr_720x480_60I(eWindow)){
            u16SrcType = QM_YPbPr_480i_Main;
        }
        else if (QM_IsYPbPr_720x480_60P(eWindow)){
            u16SrcType = QM_YPbPr_480p_Main;
        }
        else if (QM_IsYPbPr_720x576_50I(eWindow)){
            u16SrcType = QM_YPbPr_576i_Main;
        }
        else if (QM_IsYPbPr_720x576_50P(eWindow)){
            u16SrcType = QM_YPbPr_576p_Main;
        }
        else if (QM_IsYPbPr_1280x720_50P(eWindow)){
            u16SrcType = QM_YPbPr_720p_50hz_Main;
        }
        else if (QM_IsYPbPr_1280x720_60P(eWindow)){
            u16SrcType = QM_YPbPr_720p_60hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_50I(eWindow)){
            u16SrcType = QM_YPbPr_1080i_50hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_60I(eWindow)){
            u16SrcType = QM_YPbPr_1080i_60hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_24P(eWindow) || QM_IsYPbPr_1920x1080_30P(eWindow) || QM_IsYPbPr_1920x1080_25P(eWindow) ){
            u16SrcType = QM_YPbPr_1080p_24hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_50P(eWindow)){
            u16SrcType = QM_YPbPr_1080p_50hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_60P(eWindow)){
            u16SrcType = QM_YPbPr_1080p_60hz_Main;
        }
        else {
            u16SrcType = QM_YPbPr_720p_24hz_Main;
        }
#endif
    }
    else if (QM_IsSourceDTV(enInputSourceType))
    {
#if (PQ_QM_DTV)
        if(QM_IsDTV_IFRAME(eWindow))
        {
          #if 1 //( CHIP_FAMILY_TYPE != CHIP_FAMILY_S7J)
            //If PQ has devide the iframe setting according to P/I mode, then use new PQ srctype
            if (u16Input_VSize < 720)
            {
                if (bIsInterlaced)
                    u16SrcType = QM_DTV_iFrame_SD_interlace_Main;
                else
                    u16SrcType = QM_DTV_iFrame_SD_progressive_Main;
            }
            else
            {
                if (bIsInterlaced)
                    u16SrcType = QM_DTV_iFrame_HD_interlace_Main;
                else
                    u16SrcType = QM_DTV_iFrame_HD_progressive_Main;
            }
          #else
             u16SrcType = QM_DTV_iFrame_Main;
          #endif
        }
        else if (QM_IsDTV_MPEG2(eWindow))
        {
            if (u16Input_VSize < 500)
            {
                if ((u16Input_HSize < 400) && bIsInterlaced)
                {
                    u16SrcType = QM_DTV_480i_352x480_MPEG2_Main;
                }
                else
                {
                    if (bIsInterlaced)
                        u16SrcType = QM_DTV_480i_MPEG2_Main;
                    else
                        u16SrcType = QM_DTV_480p_MPEG2_Main;
                }
            }
            else if (u16Input_VSize < 650)
            {
                if (bIsInterlaced)
                    u16SrcType = QM_DTV_576i_MPEG2_Main;
                else
                    u16SrcType = QM_DTV_576p_MPEG2_Main;
            }
            else if ((u16Input_VSize < 900) && (bIsInterlaced==0))
            {
                if (u16Input_VFreq > 550)
                {
                    u16SrcType = QM_DTV_720p_60hz_MPEG2_Main;
                }
                else if(u16Input_VFreq > 250)
                {
                    u16SrcType = QM_DTV_720p_50hz_MPEG2_Main;
                }
                else
                {
                    u16SrcType = QM_DTV_720p_24hz_MPEG2_Main;
                }
            }
            else
            {
                if (bIsInterlaced)
                {
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_DTV_1080i_60hz_MPEG2_Main;
                    }
                    else
                    {
                        u16SrcType = QM_DTV_1080i_50hz_MPEG2_Main;
                    }
                }
                else
                {
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_DTV_1080p_60hz_MPEG2_Main;
                    }
                    else if(u16Input_VFreq > 250)
                    {
                        u16SrcType = QM_DTV_1080p_50hz_MPEG2_Main;
                    }
                    else
                    {
                        u16SrcType = QM_DTV_1080p_24hz_MPEG2_Main;
                    }

                }
            }
        }
        else // QM_IsDTV_H264(eWindow)
        {
            if ((u16Input_HSize < 1260) && (u16Input_VSize < 650))
            {
                if (u16Input_VSize < 500)
                {
                    if ((u16Input_HSize < 400) && bIsInterlaced)
                    {
                        u16SrcType = QM_DTV_480i_352x480_H264_Main;
                    }
                    else
                    {
                        if (bIsInterlaced)
                            u16SrcType = QM_DTV_480i_H264_Main;
                        else
                            u16SrcType = QM_DTV_480p_H264_Main;
                    }
                }
                else
                {
                    if (bIsInterlaced)
                        u16SrcType = QM_DTV_576i_H264_Main;
                    else
                        u16SrcType = QM_DTV_576p_H264_Main;
                }
            }
            else
            {
                if ((u16Input_VSize < 900) && (bIsInterlaced==0))
                {
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_DTV_720p_60hz_H264_Main;
                    }
                    else if(u16Input_VFreq > 250)
                    {
                        u16SrcType = QM_DTV_720p_50hz_H264_Main;
                    }
                    else
                    {
                        u16SrcType = QM_DTV_720p_24hz_H264_Main;
                    }
                }
                else
                {
                    if (bIsInterlaced)
                    {
                        if (u16Input_VFreq > 550)
                        {
                            u16SrcType = QM_DTV_1080i_60hz_H264_Main;
                        }
                        else
                        {
                            u16SrcType = QM_DTV_1080i_50hz_H264_Main;
                        }
                    }
                    else
                    {
                        if (u16Input_VFreq > 550)
                        {
                            u16SrcType = QM_DTV_1080p_60hz_H264_Main;
                        }
                        else if(u16Input_VFreq > 250)
                        {
                            u16SrcType = QM_DTV_1080p_50hz_H264_Main;
                        }
                        else
                        {
                            u16SrcType = QM_DTV_1080p_24hz_H264_Main;
                        }
                    }
                }
            }
        }
#endif
    }
    else if (QM_IsSourceMultiMedia(enInputSourceType))
    {
#if (PQ_QM_MM_VIDEO) || (PQ_QM_MM_PHOTO)
#if PQ_QM_NETMM_VIDEO
        if(_gIsNetworkMM)
        {
            if ((u16Input_HSize <= 720) && (u16Input_VSize <= 576))
            {
                if (bIsInterlaced)
                {
                    u16SrcType = QM_Multimedia_video_online_SD_interlace_Main;
                }
                else
                {
                    if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_Multimedia_video_online_SD_progressive_Main;
                    }
                    else
                    {
                        u16SrcType = QM_Multimedia_video_online_SD_progressive_24hz_Main;
                    }
                }
            }
            else
            {
                if (bIsInterlaced)
                {
                    u16SrcType = QM_Multimedia_video_online_HD_interlace_Main;
                }
                else
                {
                    if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_Multimedia_video_online_HD_progressive_Main;
                    }
                    else
                    {
                        u16SrcType = QM_Multimedia_video_online_HD_progressive_24hz_Main;
                    }
                }
            }
        }
        else
#endif
        if (QM_IsMultiMediaMOVIE(eWindow))
        {
            if ((u16Input_HSize <= 720) && (u16Input_VSize <= 576))
            {
                if (bIsInterlaced)
                {
                    u16SrcType = QM_Multimedia_video_SD_interlace_Main;
                }
                else
                {
#if PQ_QM_24HZMM_VIDEO
                    if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_Multimedia_video_SD_progressive_Main;
                    }
                    else
                    {
                        u16SrcType = QM_Multimedia_video_SD_progressive_24hz_Main;
                    }
#else
                        u16SrcType = QM_Multimedia_video_SD_progressive_Main;
#endif
                }
            }
            else
            {
                if (bIsInterlaced)
                {
                    u16SrcType = QM_Multimedia_video_HD_interlace_Main;
                }
                else
                {
#if PQ_QM_24HZMM_VIDEO
                    if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_Multimedia_video_HD_progressive_Main;
                    }
                    else
                    {
                        u16SrcType = QM_Multimedia_video_HD_progressive_24hz_Main;
                    }
#else
                    u16SrcType = QM_Multimedia_video_HD_progressive_Main;
#endif
                }
            }
        }
        else //QM_IsMultiMediaPHOTO(eWindow)
        {
            if ((u16Input_HSize <= 720) && (u16Input_VSize <= 576))
            {
                u16SrcType = QM_Multimedia_photo_SD_progressive_Main;
            }
            else
            {
                u16SrcType = QM_Multimedia_photo_HD_progressive_Main;
            }
        }
#endif
    }
    else if (QM_IsSourceScartRGB(enInputSourceType, eWindow))
    {
#if (PQ_QM_CVBS)
        if (u16Input_VFreq > 550)
        {
            u16SrcType = QM_SCART_RGB_NTSC_Main;
        }
        else
        {
            u16SrcType = QM_SCART_RGB_PAL_Main;
        }
#endif
    }
    else if (QM_IsSourceScartCVBS(enInputSourceType, eWindow))
    {
#if (PQ_QM_CVBS)
        eStandard = QM_GetATVStandard(eWindow);
        switch(eStandard)
        {
        case E_PQ_VIDEOSTANDARD_PAL_M:
            u16SrcType = QM_SCART_AV_PAL_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_N:
            u16SrcType = QM_SCART_AV_PAL_N_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_44:
            u16SrcType = QM_SCART_AV_NTSC_44_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_60:
            u16SrcType = QM_SCART_AV_PAL_60_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_M:
            u16SrcType = QM_SCART_AV_NTSC_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_SECAM:
            u16SrcType = QM_SCART_AV_SECAM_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_BGHI:
        default:
            u16SrcType = QM_SCART_AV_PAL_BGHI_Main;
            break;
        }
#endif
    }
    else if (QM_IsSourceATV(enInputSourceType))
    {
#if (PQ_QM_CVBS)
        eStandard = QM_GetATVStandard(eWindow);
        if (QM_FRONTEND_RFIN(eWindow))
        {
            switch(eStandard)
            {
            case E_PQ_VIDEOSTANDARD_PAL_M:
                u16SrcType = QM_RF_PAL_M_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_N:
                u16SrcType = QM_RF_PAL_N_Main;
                break;
            case E_PQ_VIDEOSTANDARD_NTSC_44:
                u16SrcType = QM_RF_NTSC_44_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_60:
                u16SrcType = QM_RF_PAL_60_Main;
                break;
            case E_PQ_VIDEOSTANDARD_NTSC_M:
                u16SrcType = QM_RF_NTSC_M_Main;
                break;
            case E_PQ_VIDEOSTANDARD_SECAM:
                u16SrcType = QM_RF_SECAM_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_BGHI:
            default:
                u16SrcType = QM_RF_PAL_BGHI_Main;
                break;
            }
        }
        else // QM_FRONTEND_VIFIN(eWindow)
        {
            switch(eStandard)
            {
            case E_PQ_VIDEOSTANDARD_PAL_M:
                u16SrcType = QM_VIF_PAL_M_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_N:
                u16SrcType = QM_VIF_PAL_N_Main;
                break;
            case E_PQ_VIDEOSTANDARD_NTSC_44:
                u16SrcType = QM_VIF_NTSC_44_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_60:
                u16SrcType = QM_VIF_PAL_60_Main;
                break;
            case E_PQ_VIDEOSTANDARD_NTSC_M:
                u16SrcType = QM_VIF_NTSC_M_Main;
                break;
            case E_PQ_VIDEOSTANDARD_SECAM:
                u16SrcType = QM_VIF_SECAM_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_BGHI:
            default:
                u16SrcType = QM_VIF_PAL_BGHI_Main;
                break;
            }
        }
#endif
    }
    else if (QM_IsSourceSV(enInputSourceType))
    {
#if (PQ_QM_CVBS)
        eStandard = QM_GetATVStandard(eWindow);
        switch(eStandard)
        {
        case E_PQ_VIDEOSTANDARD_PAL_M:
            u16SrcType = QM_SV_PAL_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_N:
            u16SrcType = QM_SV_PAL_N_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_44:
            u16SrcType = QM_SV_NTSC_44_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_60:
            u16SrcType = QM_SV_PAL_60_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_M:
            u16SrcType = QM_SV_NTSC_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_SECAM:
            u16SrcType = QM_SV_SECAM_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_BGHI:
        default:
            u16SrcType = QM_SV_PAL_BGHI_Main;
            break;
        }
#endif
    }
    else // AV
    {
#if (PQ_QM_CVBS)
        eStandard = QM_GetATVStandard(eWindow);
        switch(eStandard)
        {
        case E_PQ_VIDEOSTANDARD_PAL_M:
            u16SrcType = QM_AV_PAL_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_N:
            u16SrcType = QM_AV_PAL_N_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_44:
            u16SrcType = QM_AV_NTSC_44_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_60:
            u16SrcType = QM_AV_PAL_60_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_M:
            u16SrcType = QM_AV_NTSC_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_SECAM:
            u16SrcType = QM_AV_SECAM_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_BGHI:
        default:
            u16SrcType = QM_AV_PAL_BGHI_Main;
            break;
        }
#endif
    }
    return u16SrcType;

}

void MDrv_PQ_LoadUserPrefTimingTable(PQ_WIN eWindow, MS_U8 ucTimingIndex, MS_BOOL isWinRot)
{

#if (PQ_ONLY_SUPPORT_BIN == 0)
    if (eWindow == PQ_MAIN_WINDOW)
    {
        MDrv_PQ_LoadTableBySrcType_(MAIN, ucTimingIndex, PQ_IP_ALL, isWinRot);
#if(ENABLE_PQ_EX == 1)
        MDrv_PQ_LoadTableBySrcType_(MAINEX, ucTimingIndex, PQ_IP_ALL, isWinRot);
#endif
    }
    else if (eWindow == PQ_SUB_WINDOW)
    {
        MDrv_PQ_LoadTableBySrcType_(SUB, ucTimingIndex, PQ_IP_ALL, isWinRot);
#if(ENABLE_PQ_EX == 1)
        MDrv_PQ_LoadTableBySrcType_(SUBEX, ucTimingIndex, PQ_IP_ALL, isWinRot);
#endif
    }

#else
    UNUSED(eWindow);
    UNUSED(ucTimingIndex);
    UNUSED(isWinRot);

#endif
}

MS_BOOL _MApi_XC_FrameLockCheck(void)
{
    //return    (MApi_XC_ReadByte(0x140555) & 1);
    return (msReadByte(0x140555) & 1);
}


void MDrv_PQ_FilmDriverHWVer1(void)
{
    MS_BOOL bIsFilmXOO = 0;
    MS_U32 u32Film22AdaptiveCounter1Th = 0;
    static MS_U16 _u16RdBank_Prv=0;
    MS_U16 u16RdBank;
    MS_U32 u32CurCounter1=0, u32CurCounter3=0, u32FixThofC3=0;
    static MS_U32 _u32Counter3_2=0, _u32Counter3_3=0;
    static MS_U32 _u32Counter1_2=0, _u32Counter1_3=0, _u32Counter1_4=0;
    static MS_U32 _u32CounterA_2=0, _u32CounterA_3=0;
    static MS_U32 _u32CounterB_2=0, _u32CounterB_3=0;
    static MS_U8 _u8Film22LikeCounter, _u8Film22LikeCounter2;
    static MS_U8 _u8Film22LikeCounter_ForSlowMotionDisableMCDi;
    static MS_U8 _u8SceneChangeFrame;
    static MS_U8 _u8IsFilmXOOCounter=0;
    static MS_BOOL _bIs2ndFilmXOOCounter;
    MS_BOOL bCounter1Have2xDiff, bFilm, bFilmAny;
    MS_U32 u32CurCounterA, u32CurCounterB;
    static MS_U8 _u8SmallCounter3Cnt;
    static MS_U8 _u8Film22LikeCounterA, _u8Film22LikeCounterB;
    static MS_U8 _u8Film22LikeCounter3_Big;
    static MS_U8 _u8Film32LikeCounter3_Big;
    static MS_U8 _u8Film22LikeCounter1_Big;
    static MS_U8 _u8VerticalCnt, _u8Unmatch3Cnt;
    static MS_U8  _u8FilmsmallCounter1 = 0 ;
    MS_BOOL bIsHDSource;
    MS_U16 u16Unmatch_3, u16Unmatch_1;
    static MS_U8 _u8NoCounter3_Cnt, _u8BigCounter3_Cnt, _u8BigCounter1_Cnt, _u8BigCounter_Cnt3;
    static MS_U8 _u8BigCounter1_Cnt2, _u8SmallCounter1_Cnt, _u8BigCounter3_Cnt2;
    MS_BOOL bVerticalMovingU ;
    MS_BOOL bIs60Hz = ( MApi_XC_R2BYTE(REG_SC_BK0A_10_L) & 0x8000 ) != 0; // checkfilm32 enable for 60Hz source
    static MS_U8 _u8StillCounter3;
    MS_U32 u32Counter3;
    static MS_U8 _u8MovingCharacterCounter;
#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    MS_U32 u32Reg7C, u32Reg7D, u32Reg7E;
#endif
    MS_U16 u16InvStillFrameCnt;

    u16RdBank = ( MApi_XC_R2BYTE(REG_SC_BK12_3A_L) & 0xF0 ) >> 4;
    bIsHDSource = ( MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF) > 0x700 );

    u32CurCounterA = 0  ; // MApi_XC_R2BYTE(REG_SC_BK0A_4A_L);
    u32CurCounterB = 0  ; // MApi_XC_R2BYTE(REG_SC_BK0A_4B_L);

    u32CurCounter1 = MApi_XC_R2BYTE(REG_SC_BK0A_0A_L);
    u32CurCounter3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

    if(u16RdBank == _u16RdBank_Prv)  // Not change read bank
    {
        return ;
    }

    bFilmAny = ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040);
    bFilm = (( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008) ||  // film22
            ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010) ||  // film32
            ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040) );   // film any

    // check XOO
    if( ( (!bFilm) && (u32CurCounter3 > 0x200) && (_u32Counter3_2< 0x80) && (_u32Counter3_3< 0x80) ) || //not film
        ( (!bFilm) && (u32CurCounter3 > 0x80) && (_u32Counter3_2< 0x20) && (_u32Counter3_3< 0x20) ) ) //not film
    {
        if ( _u8IsFilmXOOCounter > 0 )
        {
            _bIs2ndFilmXOOCounter = true;
        }

        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if( bFilm && (!bFilmAny) && (u32CurCounter3 > 0x200) && (u32CurCounter3 > (_u32Counter3_2*2)) && (u32CurCounter3 > (_u32Counter3_3*2)) ) //32 or 22
    {
        if( _u8IsFilmXOOCounter > 0 )
        {
            _bIs2ndFilmXOOCounter = true;
        }
        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if(  bFilm && (u32CurCounter3 > 0x80)  && (u32CurCounter3 > (_u32Counter3_2*2)) && (u32CurCounter3 > (_u32Counter3_3*2)) ) // film-any
    {
        _bIs2ndFilmXOOCounter = true;
        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if( _u8IsFilmXOOCounter != 0 )
    {
        _u8IsFilmXOOCounter --;
    }
    else
    {
        _bIs2ndFilmXOOCounter = false;
    }

    //detect scene change into film...
    if( (u32CurCounter1 > (_u32Counter1_4*4)) && (u32CurCounter1 > (_u32Counter1_2*4)) && (u32CurCounter1 > (_u32Counter1_3*4)) )
        _u8SceneChangeFrame = 60;
    else if( _u8SceneChangeFrame != 0 )
        _u8SceneChangeFrame --;

    if( (u32CurCounter3 > 0x80) && (_u32Counter3_2 < 0x10) && (_u32Counter3_3 > 0x80) )
        _u8Film22LikeCounter += 2;
    else if( (u32CurCounter3 < 0x10) && (_u32Counter3_2 > 0x80) && (_u32Counter3_3 < 0x10) )
        _u8Film22LikeCounter += 2;
    else if( (u32CurCounter1 > (_u32Counter1_2 + 0x200)) && (_u32Counter1_3 > (_u32Counter1_2 + 0x200)) )
        _u8Film22LikeCounter += 1;
    else if( (_u32Counter1_2 > (u32CurCounter1 + 0x200)) && (_u32Counter1_2 > (_u32Counter1_3 + 0x200)) )
        _u8Film22LikeCounter += 1;
    else if ( _u8Film22LikeCounter > 0 )
        _u8Film22LikeCounter --;

    if( u32CurCounter3 < 0x80 ) // for slow motion film22-like
    {
        if( (u32CurCounter1 > (_u32Counter1_2*2)) && (_u32Counter1_3 > (_u32Counter1_2*2)) )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi += 1;
        else if ( (_u32Counter1_2 > (u32CurCounter1*2)) && (_u32Counter1_2 > (_u32Counter1_3*2)) )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi += 1;
        else if ( _u8Film22LikeCounter_ForSlowMotionDisableMCDi > 0 )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi --;
    }
    else if( _u8Film22LikeCounter_ForSlowMotionDisableMCDi > 0 )
    {
        _u8Film22LikeCounter_ForSlowMotionDisableMCDi --;
    }

    if( _u8Film22LikeCounter_ForSlowMotionDisableMCDi > 10 )
        _u8Film22LikeCounter_ForSlowMotionDisableMCDi = 10;

    if( (u32CurCounter3 > 0x80) && (_u32Counter3_2 < 0x10) && (_u32Counter3_3 > 0x80) )
        _u8Film22LikeCounter2 += 1;
    else if( (u32CurCounter3 < 0x10) && (_u32Counter3_2 > 0x80) && (_u32Counter3_3 < 0x10) )
        _u8Film22LikeCounter2 += 1;
    else
        _u8Film22LikeCounter2 = 0;

    if( _u8Film22LikeCounter2 > 10 )
        _u8Film22LikeCounter2 = 10;

    if( _u8Film22LikeCounter > 10 ) // clamp
        _u8Film22LikeCounter = 10;


    if(u32CurCounter1<0xF)
        _u8FilmsmallCounter1 ++;
    else
        _u8FilmsmallCounter1 = 0;

    if(_u8FilmsmallCounter1>10)
        _u8FilmsmallCounter1 = 10;

    if( _u32Counter3_2 < _u32Counter3_3 )
        u32FixThofC3 = _u32Counter3_2*4;
    else
        u32FixThofC3 = _u32Counter3_3*4;

    bCounter1Have2xDiff = (((u32CurCounter1 < 0xf000) && (_u32Counter1_2 < 0xf000)) &&
                        ((u32CurCounter1 > (_u32Counter1_2*2)) || ((u32CurCounter1*2) < _u32Counter1_2)));

    if( u32FixThofC3 > 0x260)
        u32FixThofC3 = 0x260;
    else if( u32FixThofC3 < 0x80)
        u32FixThofC3 = 0x80;

    if((!bFilm) && (_u8Film22LikeCounter2 > 5) && (!bCounter1Have2xDiff) ) //when moving sub-pixel, counter3 will like film22, check counter1 don't have big difference
        u32FixThofC3 = 0x80;   // moving shibasogu buildingg

    MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, (MS_U16)u32FixThofC3);
    MApi_XC_W2BYTE(REG_SC_BK0A_41_L, 0xFFFF);
    MApi_XC_W2BYTE(REG_SC_BK0A_5C_L, 0);
    MApi_XC_W2BYTE(REG_SC_BK0A_5E_L, 0);
    MApi_XC_W2BYTE(REG_SC_BK0A_20_L, 0x10);

    if( u32CurCounter1 > _u32Counter1_2 ) //max
        u32Film22AdaptiveCounter1Th = u32CurCounter1;
    else
        u32Film22AdaptiveCounter1Th = _u32Counter1_2;

    if(u32CurCounter3<5)
    {
        _u8SmallCounter3Cnt += 1;

        if(_u8SmallCounter3Cnt>10)
            _u8SmallCounter3Cnt = 10;
    }
    else
    {
        _u8SmallCounter3Cnt --  ;
    }

    if( (u32CurCounterA > (_u32CounterA_2 + 0x200/0x10)) && (_u32CounterA_3 > (_u32CounterA_2 + 0x200/0x10)))
        _u8Film22LikeCounterA += 1;
    else if(( _u32CounterA_2 > (u32CurCounterA + 0x200/0x10)) && (_u32CounterA_2 >( _u32CounterA_3 + 0x200/0x10)))
        _u8Film22LikeCounterA += 1;
    else if( _u8Film22LikeCounterA > 0 )
        _u8Film22LikeCounterA --;

    if(_u8Film22LikeCounterA>10)
        _u8Film22LikeCounterA = 10;

    if((u32CurCounterB > (_u32CounterB_2 + 0x200/0x10)) &&( _u32CounterB_3 > (_u32CounterB_2 + 0x200/0x10)))
        _u8Film22LikeCounterB += 1;
    else if(( _u32CounterB_2 > (u32CurCounterB + 0x200/0x10)) && (_u32CounterB_2 > (_u32CounterB_3 + 0x200/0x10)))
        _u8Film22LikeCounterB += 1;
    else if( _u8Film22LikeCounterB > 0 )
        _u8Film22LikeCounterB --;

    if(_u8Film22LikeCounterB>10)
        _u8Film22LikeCounterB = 10;


    if ((u32CurCounter3 > ((_u32Counter3_2+1) *10)) && (_u32Counter3_3 > ((_u32Counter3_2+1)*10)))
        _u8Film22LikeCounter3_Big += 1;
    else if((_u32Counter3_2 > ((u32CurCounter3+1)*10)) && (_u32Counter3_2 > ((_u32Counter3_3+1)*10)))
        _u8Film22LikeCounter3_Big += 1;
    else if(_u8Film22LikeCounter3_Big > 0 )
        _u8Film22LikeCounter3_Big --;

    if(_u8Film22LikeCounter3_Big>10)
        _u8Film22LikeCounter3_Big = 10;

    if( ((u32CurCounter3 > (_u32Counter3_2*4))||(u32CurCounter3 > (_u32Counter3_3*4)) ) && (u32CurCounter3>0x40) )
        _u8Film32LikeCounter3_Big += 1;
    else if( ( (_u32Counter3_2 > (_u32Counter3_3*4))||(_u32Counter3_2 > (u32CurCounter3*4)) ) && (_u32Counter3_2>0x40))
        _u8Film32LikeCounter3_Big += 1;
    else if( ( (_u32Counter3_3 > (u32CurCounter3*4))||(_u32Counter3_3 > (_u32Counter3_2*4)) ) && (_u32Counter3_3>0x40))
        _u8Film32LikeCounter3_Big += 1;
    else
        _u8Film32LikeCounter3_Big --;

    if(_u8Film32LikeCounter3_Big>10)
        _u8Film32LikeCounter3_Big = 10;

    if ( (u32CurCounter1 > ((_u32Counter1_2+1)*20)) && (_u32Counter1_3 > ((_u32Counter1_2+1)*20)))
        _u8Film22LikeCounter1_Big += 1;
    else if (( _u32Counter1_2 > ((u32CurCounter1+1)*20)) && (_u32Counter1_2 > ((_u32Counter1_3+1)*20)))
        _u8Film22LikeCounter1_Big += 1;
    else if ( _u8Film22LikeCounter1_Big > 0 )
        _u8Film22LikeCounter1_Big --;

    if(_u8Film22LikeCounter1_Big>10)
        _u8Film22LikeCounter1_Big = 10;


    //for KURO patch

    MS_U16 KURO_patch_en = 0;

    if( u32CurCounter3 >_u32Counter3_2+0x20 && u32CurCounter1+0x20 < _u32Counter1_2 && u32CurCounter1==0 )
        KURO_patch_en = 1;
    else if( u32CurCounter3+0x20 <_u32Counter3_2 && u32CurCounter1 > _u32Counter1_2+0x20 && _u32Counter1_2==0)
        KURO_patch_en = 1;


    if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008 )
    {  //in film22
        if(((u32CurCounter3 > (_u32Counter3_2+1)*10)||(_u32Counter3_2 > (u32CurCounter3+1)*10)))
        {
            if(_u8Film22LikeCounter1_Big == 10)
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*3/8;
            }
            else
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*9/8;

                if( bIsHDSource )
                {
                    if( u32Film22AdaptiveCounter1Th < 0x400/0x2 *6)
                        u32Film22AdaptiveCounter1Th = 0x400/0x2 *6;
                }
                else
                {
                    if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                        u32Film22AdaptiveCounter1Th = 0x400/0x2;
                }
            }
        }
        else
        {
            if ( MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)) ) // 2R
            {
                u32Film22AdaptiveCounter1Th = 0xFFFF;
            }
            else
            {
                if(_u8Film22LikeCounter1_Big == 10)
                     u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*3/8;
                else
                    u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;
            }

        }
    }
    else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010 )
    {  //in film32
        if((u32CurCounter3 > (_u32Counter3_2*10))||(_u32Counter3_2 > (u32CurCounter3*10))||(_u32Counter3_3 > (_u32Counter3_2*10)))
        {
            if( (u32CurCounter1>= _u32Counter1_2) && (u32CurCounter1 >= _u32Counter1_3)  )
                u32Film22AdaptiveCounter1Th = u32CurCounter1 * 2;
            else if((_u32Counter1_2>= u32CurCounter1) && (_u32Counter1_2 >= _u32Counter1_3))
                u32Film22AdaptiveCounter1Th = _u32Counter1_2 * 2;
            else if((_u32Counter1_3>= u32CurCounter1) &&( _u32Counter1_3 >= _u32Counter1_2))
                u32Film22AdaptiveCounter1Th = _u32Counter1_3 * 2;

            if( bIsHDSource )
            {
                if( u32Film22AdaptiveCounter1Th < (0x400/0x2*6) )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2 *6;
            }
            else
            {
                if( u32Film22AdaptiveCounter1Th < (0x400/0x2) )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;
            }
        }
        else
        {
            if ( MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)) ) // 2R
            {
                u32Film22AdaptiveCounter1Th = 0xFFFF;
            }
            else
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;
           }
        }
    }
    else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 )
    {  //in filmany
        u32Film22AdaptiveCounter1Th = 0x800;
    }
    else
    {
        if(_u8SmallCounter3Cnt==10)
        {
            if((_u8Film22LikeCounterA>5) || (_u8Film22LikeCounterB>5))
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;
                if( bIsHDSource )
                {
                    if( u32Film22AdaptiveCounter1Th < (0x400/0x2*6) )
                        u32Film22AdaptiveCounter1Th = 0x400/0x2*6;
                }
                else
                {
                    if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                        u32Film22AdaptiveCounter1Th = 0x400/0x2;
                }
            }
            else
            {
                if( bIsHDSource )
                {
                    u32Film22AdaptiveCounter1Th = 0x400/0x2*6;
                }
                else
                {
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;
                }
            }
        }
        else
        {
            u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

            if( bIsHDSource )
            {
                if( u32Film22AdaptiveCounter1Th < 0x400/0x2*6 )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2*6;
            }
            else
            {
                if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;
            }
        }

        if(_u8Film22LikeCounter3_Big>8 || _u8Film32LikeCounter3_Big>8)
            u32Film22AdaptiveCounter1Th  = 0x7500;
    }


    if( bIsHDSource )
    {
        if( u32Film22AdaptiveCounter1Th < 0x400/0x4*6 )
            u32Film22AdaptiveCounter1Th = 0x400/0x4*6;
    }
    else
    {
        if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010 ) //in Film32
        {
            if( u32Film22AdaptiveCounter1Th < 0xF00)
                u32Film22AdaptiveCounter1Th = 0xF00;
        }
        else
        {
            if( u32Film22AdaptiveCounter1Th < 0x800)
                u32Film22AdaptiveCounter1Th = 0x800;
        }

    }

    _u32Counter3_3 = _u32Counter3_2;
    _u32Counter3_2  = u32CurCounter3;
    _u32Counter1_4 = _u32Counter1_3;
    _u32Counter1_3 = _u32Counter1_2;
    _u32Counter1_2  = u32CurCounter1;

    _u32CounterA_3 = _u32CounterA_2;
    _u32CounterA_2  = u32CurCounterA;
    _u32CounterB_3 = _u32CounterB_2;
    _u32CounterB_2  = u32CurCounterB;

    bIsFilmXOO = (_u8IsFilmXOOCounter != 0);

#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    u32Reg7C = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32Reg7D = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32Reg7E = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);

    // for all chip
    if( bIsHDSource )
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0200) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
    else
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0100) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
#else
    // for no MCNR
    bVerticalMovingU = false;
#endif

    u16Unmatch_3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);
    u16Unmatch_1 = MApi_XC_R2BYTE(REG_SC_BK0A_0A_L);

    // for sec , activate counter 1
    if( u16Unmatch_3 < 0x40 )
    {
        _u8NoCounter3_Cnt ++;
    }
    else
    {
        _u8NoCounter3_Cnt = 0;
    }

    // deactivate counter 1 jump out
    if( u16Unmatch_3 > 0x6000  )
    {
        _u8BigCounter3_Cnt = 10;
    }
    else if(_u8BigCounter3_Cnt > 0)
    {
        _u8BigCounter3_Cnt --;
    }

    if( u16Unmatch_3 > 0x1000  )
    {
        _u8BigCounter3_Cnt2 = 10;
    }
    else if(_u8BigCounter3_Cnt2 > 0)
    {
        _u8BigCounter3_Cnt2 --;
    }

    // deactivate counter 1 jump out
#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    if( u16Unmatch_1 > 0xF000  )
#else
    if( u16Unmatch_1 > 0xA000  )
#endif
    {
        _u8BigCounter1_Cnt = 10;
    }
    else if(_u8BigCounter1_Cnt > 0)
    {
        _u8BigCounter1_Cnt --;
    }

    if( u16Unmatch_1 > 0xc000)
    {
        if( _u8BigCounter_Cnt3 < 80)
            _u8BigCounter_Cnt3 ++;
    }
    else
        _u8BigCounter_Cnt3 = 0;

    if( u16Unmatch_1 > 0x1000 )
        _u8BigCounter1_Cnt2 = 0x8;
    else if( _u8BigCounter1_Cnt2 != 0)
        _u8BigCounter1_Cnt2 --;

    if( u16Unmatch_1 < 0x0800 )
    {
        _u8SmallCounter1_Cnt = 10;
    }
    else if(_u8SmallCounter1_Cnt > 0)
    {
        _u8SmallCounter1_Cnt --;
    }

    if((MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2500) || (MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2700) )//(stXCStatus.bInterlace)
    {
        if(_u8NoCounter3_Cnt > 20)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x01)<<8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10)<<8, 0xFF00);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x04)<<8, 0x1F00);  // for SD cross hatch
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10)<<8, 0xFF00);
        }

        // Film32 On,f2 32 film mode enable
        if( (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(14) ) ==  BIT(14) )
           || (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(15) ) ==  BIT(15) ) )
        {
            if(!_MApi_XC_FrameLockCheck() ) // Make not frame LOCKED then turn off FilmMode
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, BIT(1), BIT(1)); //hardware film mode DISABLE
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, BIT(1)|BIT(2), BIT(1) | BIT(2)); //software film mode DISABLE
            }
            else                                   // Make sure frame LOCKED then turn on FilmMode
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1)); //hardware film mode ENABLE
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2)); //software film mode ENABLE

                ///--> new add function for skip film mode in still pattern
                if((!MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)))&&(!MApi_XC_R2BYTEMSK(REG_SC_BK0A_02_L, BIT(14))))    // 2R mode can't enter this loop
                {
                    u16Unmatch_3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

                    if( (u16Unmatch_3 > 0x80) )
                        _u8Unmatch3Cnt = 0;
                    else if(_u8Unmatch3Cnt == 0xFF)
                        _u8Unmatch3Cnt = 0xFF;
                    else
                        _u8Unmatch3Cnt = _u8Unmatch3Cnt + 1;

                    ///--> Jump out Film for LG tower
                    if( !bVerticalMovingU )
                        _u8VerticalCnt = 0;
                    else if(_u8VerticalCnt == 0xFF)
                        _u8VerticalCnt = 0xFF;
                    else
                        _u8VerticalCnt = _u8VerticalCnt + 1;
                }
            }
        }
        else
        {
            // DEFAULT value of hardware and software film mode suggested by SY.Shih
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1));
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2));
        }

        // film-any continuous error line patch, not easy to enter film, but easy to exit film
        if ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 ) // film-any status
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_57_L, 0x08, 0xff);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_57_L, 0x20, 0xff);

        if ( bIsHDSource && (!bFilm) )
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_15_L, 0x0008, 0x000F ); // for film32, also favor de-bouncing
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_15_L, 0x0008, 0x000F ); // for De-bouncing

        if(_u8VerticalCnt>0x03)          // only active at Film
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
        }
        else
        {
            if((u32CurCounter3<0x9) && (u32CurCounter1<0x9))
            {
                _u8StillCounter3++;
            }
            else
            {
                _u8StillCounter3 = 0;
            }

            if(_u8StillCounter3>=8)
            {
                _u8StillCounter3 = 8;
            }

            if(u32Film22AdaptiveCounter1Th>0xFFFF)
                u32Film22AdaptiveCounter1Th = 0xFFFF;

            MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, (MS_U16)u32Film22AdaptiveCounter1Th );


              if(_u8StillCounter3==8 || KURO_patch_en==1 || _u8FilmsmallCounter1==10) //default
            {
                if ( MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)) )  // 2R
                {

                }
                else //4R
                {
                    MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0 );
                    MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, 0 );
                }
            }

            if( bIs60Hz ) //NTSC -> turn off fast film22
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
            else // PAL
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, BIT(15), BIT(15));
        }

        // for SEC pattern gen
        if(_u8NoCounter3_Cnt > 20)
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x00FF, 0xFFFF); // disable film-any
        else if ( !bIsFilmXOO )
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0080, 0xFFFF); // disable film-any
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0810, 0xFFFF);

        // for auto 1
        // film32
        u32Counter3  = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);


        if( u32Counter3 < 0x100 )
            _u8MovingCharacterCounter = 0; //
        else if(_u8MovingCharacterCounter != 0xff )
            _u8MovingCharacterCounter ++;

        if ( MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, BIT(12)) ) // auto 1 , deal with video-over-film
        {
        }
        else // default
        {
            if ( bIsHDSource )
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_06_L, 0xCF40);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, BIT(13),BIT(13) );
                //MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x0fff);
            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_06_L, 0xA810);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, 0x00 ,BIT(13) );
                MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x0250);
            }

            MApi_XC_W2BYTE(REG_SC_BK0A_16_L, 0x00D8);
            MApi_XC_W2BYTE(REG_SC_BK0A_1E_L, 0x0551);
        }

        if ( MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, BIT(12)) ) // auto 1 , deal with video-over-film
        {
        }
        else // default
        {
            if ( bIsHDSource )
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_07_L, 0x7F10);

            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_07_L, 0x7F08);
            }
        }
    } // end interlace loop
    _u16RdBank_Prv = u16RdBank;

    u16InvStillFrameCnt = 15;

    if((_u8Film22LikeCounter > 0x5) || (_u8Film22LikeCounter_ForSlowMotionDisableMCDi > 0x5))
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2F_L, 0x8000, 0xFFFF); //disable MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, 0xF000, 0xFF00); //Favor SST
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, 0x0000, 0xFF00); //Disable KFC-cplx
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2F_L, 0x0000, 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, 0x2100, 0xFF00);
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, ((MS_U16)0xF000)|((MS_U16)(u16InvStillFrameCnt<<8)), 0xFF00);
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)(u16InvStillFrameCnt<<4), 0x00F0);
    }
#if 0
    if( ( MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK01_42_L, 0x0001) == 0x1 )
    {
        if( ( MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008 )
        {

            printk("in22 \n");
        }
        else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010 )
        {
            printk("in32 \n");
        }
        else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 )
        {
            printk("inany \n");
        }
        else
        {
            printk("out \n");
        }

        printk("um3 = %lx, um1 = %lx,reg3 = %x, reg1 = %x , 4a= %x , 4b= %x, idx= %lx \n",u32CurCounter3, u32CurCounter1,MApi_XC_R2BYTE(REG_SC_BK0A_0D_L),MApi_XC_R2BYTE(REG_SC_BK0A_0C_L) ,MApi_XC_R2BYTE(REG_SC_BK0A_4A_L),MApi_XC_R2BYTE(REG_SC_BK0A_4B_L),u16RdBank);
    }
#endif
}

MS_U32  _Read2Byte(MS_U8 ucWin, MS_U16 addr)
{
    MS_U32 u32BaseAddr;
    MS_U8  u8ScIdx = 0;
    MS_U16 u16ScMask = g_DisplayWindow[ucWin].u16DwScMaskIn;
    MS_U32 ret = 0;
    u32BaseAddr = u32ScBaseAddr;
    do
    {
        if (u16ScMask & _BIT0)
        {
            if (u8ScIdx % 2)
            {
                u32ScBaseAddr = SCRegBase(u8ScIdx);
                ret += MApi_XC_R2BYTE(addr);
            }
        }
        u8ScIdx++;
        u16ScMask >>=1;

    } while (u16ScMask > 0);
    u32ScBaseAddr = u32BaseAddr;
    return ret;
}

void MDrv_PQ_FilmDriverHWVer2(BYTE ucWin)
{
    MS_BOOL bIsFilmXOO = 0;
    MS_U32 u32Film22AdaptiveCounter1Th = 0;
    static MS_U16 _u16RdBank_Prv[DISPLAY_MAX_NUMS]={0};
    MS_U16 u16RdBank;
    MS_U32 u32CurCounter1=0, u32CurCounter3=0, u32FixThofC3=0;
    static MS_U16 continuousNotVideoOverFilm[DISPLAY_MAX_NUMS];
    static MS_U16 curMaxErrLine[DISPLAY_MAX_NUMS];
    static MS_U16 _prvMaxErrLine[DISPLAY_MAX_NUMS];
    static MS_U32 _u32Counter3_2[DISPLAY_MAX_NUMS]={0}, _u32Counter3_3[DISPLAY_MAX_NUMS]={0};
    static MS_U32 _u32Counter1_2[DISPLAY_MAX_NUMS]={0}, _u32Counter1_3[DISPLAY_MAX_NUMS]={0}, _u32Counter1_4[DISPLAY_MAX_NUMS]={0};
    static MS_U32 _u32CounterA_2[DISPLAY_MAX_NUMS]={0}, _u32CounterA_3[DISPLAY_MAX_NUMS]={0};
    static MS_U32 _u32CounterB_2[DISPLAY_MAX_NUMS]={0}, _u32CounterB_3[DISPLAY_MAX_NUMS]={0};
    static MS_U8 _u8Film22LikeCounter[DISPLAY_MAX_NUMS], _u8Film22LikeCounter2[DISPLAY_MAX_NUMS];
    static MS_U8 _u8Film22LikeCounter_ForSlowMotionDisableMCDi[DISPLAY_MAX_NUMS];
    static MS_U8 _u8SceneChangeFrame[DISPLAY_MAX_NUMS];
    static MS_U8 _u8IsFilmXOOCounter[DISPLAY_MAX_NUMS]={0};
    static MS_BOOL _bIs2ndFilmXOOCounter[DISPLAY_MAX_NUMS];
    MS_BOOL bCounter1Have2xDiff, bFilm, bFilmAny;
    MS_U32 u32CurCounterA = 0, u32CurCounterB = 0;
    static MS_U8 _u8SmallCounter3Cnt[DISPLAY_MAX_NUMS];
    static MS_U8 _u8Film22LikeCounterA[DISPLAY_MAX_NUMS], _u8Film22LikeCounterB[DISPLAY_MAX_NUMS];
    static MS_U8 _u8Film22LikeCounter3_Big[DISPLAY_MAX_NUMS];
    static MS_U8 _u8Film32LikeCounter3_Big[DISPLAY_MAX_NUMS];
    static MS_U8 _u8Film22LikeCounter1_Big[DISPLAY_MAX_NUMS];
    static MS_U8 _u8VerticalCnt[DISPLAY_MAX_NUMS], _u8Unmatch3Cnt[DISPLAY_MAX_NUMS];
    MS_BOOL bFilm32;   //not used
    MS_BOOL bIsHDSource;
    MS_U16 u16Unmatch_3 = 0, u16Unmatch_1 = 0;
    static MS_U8 _u8NoCounter3_Cnt[DISPLAY_MAX_NUMS], _u8BigCounter3_Cnt[DISPLAY_MAX_NUMS], _u8BigCounter1_Cnt[DISPLAY_MAX_NUMS], _u8BigCounter_Cnt3[DISPLAY_MAX_NUMS];
    static MS_U8 _u8BigCounter1_Cnt2[DISPLAY_MAX_NUMS], _u8SmallCounter1_Cnt[DISPLAY_MAX_NUMS], _u8BigCounter3_Cnt2[DISPLAY_MAX_NUMS];
    MS_BOOL bVerticalMovingU ;
    MS_BOOL bIs60Hz = ( MApi_XC_R2BYTE(REG_SC_BK0A_10_L) & 0x8000 ) != 0; // checkfilm32 enable for 60Hz source
    static MS_U8 _u8StillCounter3[DISPLAY_MAX_NUMS];
    MS_U32 u32Counter3;
    static MS_U8 _u8MovingCharacterCounter[DISPLAY_MAX_NUMS];
#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    MS_U32 u32Reg7C, u32Reg7D, u32Reg7E;
#endif

    u16RdBank = ( MApi_XC_R2BYTE(REG_SC_BK12_3A_L) & 0xF0 ) >> 4;
    bIsHDSource = ( MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF) > 0x700 );


    u32CurCounterA = _Read2Byte(ucWin, REG_SC_BK0A_4A_L);
    u32CurCounterB = _Read2Byte(ucWin, REG_SC_BK0A_4B_L);

    u32CurCounter1 = _Read2Byte(ucWin, REG_SC_BK0A_0A_L);
    u32CurCounter3 = _Read2Byte(ucWin, REG_SC_BK0A_0B_L);

    if(u16RdBank == _u16RdBank_Prv[ucWin])  // Not change read bank
    {
        return ;
    }

    bFilmAny = ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040);
    bFilm = (( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008) ||  // film22
            ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010) ||  // film32
            ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040) );   // film any

    // check XOO
    if( ( (!bFilm) && (u32CurCounter3 > 0x200) && (_u32Counter3_2[ucWin]< 0x80) && (_u32Counter3_3[ucWin]< 0x80) ) || //not film
        ( (!bFilm) && (u32CurCounter3 > 0x80) && (_u32Counter3_2[ucWin]< 0x20) && (_u32Counter3_3[ucWin]< 0x20) ) ) //not film
    {
        if ( _u8IsFilmXOOCounter[ucWin] > 0 )
        {
            _bIs2ndFilmXOOCounter[ucWin] = true;
        }

        _u8IsFilmXOOCounter[ucWin] = 10; // need to hit XOO on every 10 frames
    }
    else if( bFilm && (!bFilmAny) && (u32CurCounter3 > 0x200) && (u32CurCounter3 > (_u32Counter3_2[ucWin]*2)) && (u32CurCounter3 > (_u32Counter3_3[ucWin]*2)) ) //32 or 22
    {
        if( _u8IsFilmXOOCounter[ucWin] > 0 )
        {
            _bIs2ndFilmXOOCounter[ucWin] = true;
        }
        _u8IsFilmXOOCounter[ucWin] = 10; // need to hit XOO on every 10 frames
    }
    else if(  bFilmAny && (u32CurCounter3 > 0x80)  && (u32CurCounter3 > (_u32Counter3_2[ucWin]*2)) && (u32CurCounter3 > (_u32Counter3_3[ucWin]*2)) ) // film-any
    {
        _bIs2ndFilmXOOCounter[ucWin] = true;
        _u8IsFilmXOOCounter[ucWin] = 10; // need to hit XOO on every 10 frames
    }
    else if( _u8IsFilmXOOCounter[ucWin] != 0 )
    {
        _u8IsFilmXOOCounter[ucWin] --;
    }
    else
    {
        _bIs2ndFilmXOOCounter[ucWin] = false;
    }

    //detect scene change into film...
    if( (u32CurCounter1 > (_u32Counter1_4[ucWin]*4)) && (u32CurCounter1 > (_u32Counter1_2[ucWin]*4)) && (u32CurCounter1 > (_u32Counter1_3[ucWin]*4)) )
        _u8SceneChangeFrame[ucWin] = 60;
    else if( _u8SceneChangeFrame[ucWin] != 0 )
        _u8SceneChangeFrame[ucWin] --;

    if( (u32CurCounter3 > 0x80) && (_u32Counter3_2[ucWin] < 0x10) && (_u32Counter3_3[ucWin] > 0x80) )
        _u8Film22LikeCounter[ucWin] += 2;
    else if( (u32CurCounter3 < 0x10) && (_u32Counter3_2[ucWin] > 0x80) && (_u32Counter3_3[ucWin] < 0x10) )
        _u8Film22LikeCounter[ucWin] += 2;
    else if( (u32CurCounter1 > (_u32Counter1_2[ucWin] + 0x20)) && (_u32Counter1_3[ucWin] > (_u32Counter1_2[ucWin] + 0x20)) )
        _u8Film22LikeCounter[ucWin] += 1;
    else if( (_u32Counter1_2[ucWin] > (u32CurCounter1 + 0x20)) && (_u32Counter1_2[ucWin] > (_u32Counter1_3[ucWin] + 0x20)) )
        _u8Film22LikeCounter[ucWin] += 1;
    else if ( _u8Film22LikeCounter[ucWin] > 0 )
        _u8Film22LikeCounter[ucWin] --;

    if( u32CurCounter3 < 0x80 ) // for slow motion film22-like
    {
        if( (u32CurCounter1 > (_u32Counter1_2[ucWin]*2)) && (_u32Counter1_3[ucWin] > (_u32Counter1_2[ucWin]*2)) )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi[ucWin] += 1;
        else if ( (_u32Counter1_2[ucWin] > (u32CurCounter1*2)) && (_u32Counter1_2[ucWin] > (_u32Counter1_3[ucWin]*2)) )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi[ucWin] += 1;
        else if ( _u8Film22LikeCounter_ForSlowMotionDisableMCDi[ucWin] > 0 )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi[ucWin] --;
    }
    else if( _u8Film22LikeCounter_ForSlowMotionDisableMCDi[ucWin] > 0 )
    {
        _u8Film22LikeCounter_ForSlowMotionDisableMCDi[ucWin] --;
    }

    if( _u8Film22LikeCounter_ForSlowMotionDisableMCDi[ucWin] > 10 )
        _u8Film22LikeCounter_ForSlowMotionDisableMCDi[ucWin] = 10;

    if( (u32CurCounter3 > 0x80) && (_u32Counter3_2[ucWin] < 0x10) && (_u32Counter3_3[ucWin] > 0x80) )
        _u8Film22LikeCounter2[ucWin] += 1;
    else if( (u32CurCounter3 < 0x10) && (_u32Counter3_2[ucWin] > 0x80) && (_u32Counter3_3[ucWin] < 0x10) )
        _u8Film22LikeCounter2[ucWin] += 1;
    else
        _u8Film22LikeCounter2[ucWin] = 0;

    if( _u8Film22LikeCounter2[ucWin] > 10 )
        _u8Film22LikeCounter2[ucWin] = 10;

    if( _u8Film22LikeCounter[ucWin] > 10 ) // clamp
        _u8Film22LikeCounter[ucWin] = 10;

    if( _u32Counter3_2[ucWin] < _u32Counter3_3[ucWin] )
        u32FixThofC3 = _u32Counter3_2[ucWin]*4;
    else
        u32FixThofC3 = _u32Counter3_3[ucWin]*4;

    // for Eular book1 6-aix pattern
    if ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010 )
    {
        // in film32 FixofC3 = max( c3, c3_2, c3_3 )/4
        if ( u32FixThofC3 < u32CurCounter3/4  )
            u32FixThofC3 = u32CurCounter3/4;
        if ( u32FixThofC3 < _u32Counter3_2[ucWin]/4  )
            u32FixThofC3 = _u32Counter3_2[ucWin]/4;
        if ( u32FixThofC3 < _u32Counter3_3[ucWin]/4  )
            u32FixThofC3 = _u32Counter3_3[ucWin]/4;

        if ( _u32Counter3_2[ucWin] > 0x400 || _u32Counter3_3[ucWin] > 0x400 || u32CurCounter3 > 0x400 )
        {
            if ( u32FixThofC3 < 0x180 )
                u32FixThofC3 = 0x180;
        }
      else if( u32FixThofC3 < 0x80)
        u32FixThofC3 = 0x80;
    }
    else if ( continuousNotVideoOverFilm[ucWin] > 0x10 )  // for book1 gladiator
    {
        // in film32 FixofC3 = max( c3, c3_2, c3_3 )/4
        if ( u32FixThofC3 < u32CurCounter3/4  )
            u32FixThofC3 = u32CurCounter3/4;
        if ( u32FixThofC3 < _u32Counter3_2[ucWin]/4  )
            u32FixThofC3 = _u32Counter3_2[ucWin]/4;
        if ( u32FixThofC3 < _u32Counter3_3[ucWin]/4  )
            u32FixThofC3 = _u32Counter3_3[ucWin]/4;

      if( u32FixThofC3 < 0x80)
        u32FixThofC3 = 0x80;
    }
    else
    {
      if( u32FixThofC3 > 0x210)
          u32FixThofC3 = 0x210;
      else if( u32FixThofC3 < 0x80)
          u32FixThofC3 = 0x80;
    }

    bCounter1Have2xDiff = (((u32CurCounter1 < 0xf00) && (_u32Counter1_2[ucWin] < 0xf00)) &&
                        ((u32CurCounter1 > (_u32Counter1_2[ucWin]*2)) || ((u32CurCounter1*2) < _u32Counter1_2[ucWin])));


    if((!bFilm) && (_u8Film22LikeCounter2[ucWin] > 5) && (!bCounter1Have2xDiff) ) //when moving sub-pixel, counter3 will like film22, check counter1 don't have big difference
        u32FixThofC3 = 0x80;   // moving shibasogu buildingg

    MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, u32FixThofC3);
    MApi_XC_W2BYTE(REG_SC_BK0A_41_L, 0xFFFF);
    MApi_XC_W2BYTE(REG_SC_BK0A_5C_L, 0);
    MApi_XC_W2BYTE(REG_SC_BK0A_5E_L, 0);
    MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, 0, 0x0F );
    MApi_XC_W2BYTE(REG_SC_BK0A_20_L, 0x10);

    if( u32CurCounter1 > _u32Counter1_2[ucWin] ) //max
        u32Film22AdaptiveCounter1Th = u32CurCounter1;
    else
        u32Film22AdaptiveCounter1Th = _u32Counter1_2[ucWin];

    if(u32CurCounter3<5)
    {
        _u8SmallCounter3Cnt[ucWin] += 1;

        if(_u8SmallCounter3Cnt[ucWin]>10)
            _u8SmallCounter3Cnt[ucWin] = 10;
    }
    else
    {
        _u8SmallCounter3Cnt[ucWin] --  ;
    }

    if( (u32CurCounterA > (_u32CounterA_2[ucWin] + 0x200/0x10)) && (_u32CounterA_3[ucWin] > (_u32CounterA_2[ucWin] + 0x200/0x10)))
        _u8Film22LikeCounterA[ucWin] += 1;
    else if(( _u32CounterA_2[ucWin] > (u32CurCounterA + 0x200/0x10)) && (_u32CounterA_2[ucWin] >( _u32CounterA_3[ucWin] + 0x200/0x10)))
        _u8Film22LikeCounterA[ucWin] += 1;
    else if( _u8Film22LikeCounterA[ucWin] > 0 )
        _u8Film22LikeCounterA[ucWin] --;

    if(_u8Film22LikeCounterA[ucWin]>10)
        _u8Film22LikeCounterA[ucWin] = 10;

    if((u32CurCounterB > (_u32CounterB_2[ucWin] + 0x200/0x10)) &&( _u32CounterB_3[ucWin] > (_u32CounterB_2[ucWin] + 0x200/0x10)))
        _u8Film22LikeCounterB[ucWin] += 1;
    else if(( _u32CounterB_2[ucWin] > (u32CurCounterB + 0x200/0x10)) && (_u32CounterB_2[ucWin] > (_u32CounterB_3[ucWin] + 0x200/0x10)))
        _u8Film22LikeCounterB[ucWin] += 1;
    else if( _u8Film22LikeCounterB[ucWin] > 0 )
        _u8Film22LikeCounterB[ucWin] --;

    if(_u8Film22LikeCounterB[ucWin]>10)
        _u8Film22LikeCounterB[ucWin] = 10;


    if ((u32CurCounter3 > ((_u32Counter3_2[ucWin]+1) *10)) && (_u32Counter3_3[ucWin] > ((_u32Counter3_2[ucWin]+1)*10)))
        _u8Film22LikeCounter3_Big[ucWin] += 1;
    else if((_u32Counter3_2[ucWin] > ((u32CurCounter3+1)*10)) && (_u32Counter3_2[ucWin] > ((_u32Counter3_3[ucWin]+1)*10)))
        _u8Film22LikeCounter3_Big[ucWin] += 1;
    else if(_u8Film22LikeCounter3_Big[ucWin] > 0 )
        _u8Film22LikeCounter3_Big[ucWin] --;

    if(_u8Film22LikeCounter3_Big[ucWin]>10)
        _u8Film22LikeCounter3_Big[ucWin] = 10;

    if( (u32CurCounter3 > (_u32Counter3_2[ucWin]*4))||(u32CurCounter3 > (_u32Counter3_3[ucWin]*4)))
        _u8Film32LikeCounter3_Big[ucWin] += 1;
    else if( (_u32Counter3_2[ucWin] > (_u32Counter3_3[ucWin]*4))||(_u32Counter3_2[ucWin] > (u32CurCounter3*4)))
        _u8Film32LikeCounter3_Big[ucWin] += 1;
    else if( (_u32Counter3_3[ucWin] > (u32CurCounter3*4))||(_u32Counter3_3[ucWin] > (_u32Counter3_2[ucWin]*4)))
        _u8Film32LikeCounter3_Big[ucWin] += 1;
    else
        _u8Film32LikeCounter3_Big[ucWin] --;

    if(_u8Film32LikeCounter3_Big[ucWin]>10)
        _u8Film32LikeCounter3_Big[ucWin] = 10;

    if ( (u32CurCounter1 > ((_u32Counter1_2[ucWin]+1)*20)) && (_u32Counter1_3[ucWin] > ((_u32Counter1_2[ucWin]+1)*20)))
        _u8Film22LikeCounter1_Big[ucWin] += 1;
    else if (( _u32Counter1_2[ucWin] > ((u32CurCounter1+1)*20)) && (_u32Counter1_2[ucWin] > ((_u32Counter1_3[ucWin]+1)*20)))
        _u8Film22LikeCounter1_Big[ucWin] += 1;
    else if ( _u8Film22LikeCounter1_Big[ucWin] > 0 )
        _u8Film22LikeCounter1_Big[ucWin] --;

    if(_u8Film22LikeCounter1_Big[ucWin]>10)
        _u8Film22LikeCounter1_Big[ucWin] = 10;

    //for KURO patch
    MS_U16 KURO_patch_en = 0;
    {
        if( u32CurCounter3 >_u32Counter3_2[ucWin]+0x20 && u32CurCounter1+0x20 < _u32Counter1_2[ucWin] && u32CurCounter1==0 )
            KURO_patch_en = 1;
        else if( u32CurCounter3 <_u32Counter3_2[ucWin]+0x20 && u32CurCounter1 > _u32Counter1_2[ucWin]+0x20 && _u32Counter1_2[ucWin]==0 )
            KURO_patch_en = 1;
    }

    //for SEC pattern Gen.
    MS_U16 MSPG4233_patch_en = 0;
    {
        if( u32CurCounter3 >_u32Counter3_2[ucWin]+0x20 && u32CurCounter1+0x20 < _u32Counter1_2[ucWin] && _u32Counter3_2[ucWin]==0 )
            MSPG4233_patch_en = 1;
        else if( u32CurCounter3 <_u32Counter3_2[ucWin]+0x20 && u32CurCounter1 > _u32Counter1_2[ucWin]+0x20 && u32CurCounter3==0 )
            MSPG4233_patch_en = 1;
    }

    if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008 )
    {  //in film22
        if(((u32CurCounter3 > (_u32Counter3_2[ucWin]+1)*10)||(_u32Counter3_2[ucWin] > (u32CurCounter3+1)*10)))
        {
            if(_u8Film22LikeCounter1_Big[ucWin] == 10)
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*3/8;

            else
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*9/8;

                if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;
            }
        }
        else
        {
            if(_u8Film22LikeCounter1_Big[ucWin] == 10)
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*3/8;
            else
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

        }
    }
    else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010 )
    {  //in film32
        if((u32CurCounter3 > (_u32Counter3_2[ucWin]*10))||(_u32Counter3_2[ucWin] > (u32CurCounter3*10))||(_u32Counter3_3[ucWin] > (_u32Counter3_2[ucWin]*10)))
        {
            if( (u32CurCounter1>= _u32Counter1_2[ucWin]) && (u32CurCounter1 >= _u32Counter1_3[ucWin])  )
                u32Film22AdaptiveCounter1Th = u32CurCounter1 * 2;
            else if((_u32Counter1_2[ucWin]>= u32CurCounter1) && (_u32Counter1_2[ucWin] >= _u32Counter1_3[ucWin]))
                u32Film22AdaptiveCounter1Th = _u32Counter1_2[ucWin] * 2;
            else if((_u32Counter1_3[ucWin]>= u32CurCounter1) &&( _u32Counter1_3[ucWin] >= _u32Counter1_2[ucWin]))
                u32Film22AdaptiveCounter1Th = _u32Counter1_3[ucWin] * 2;

            if( u32Film22AdaptiveCounter1Th < (0x400/0x2) )
                u32Film22AdaptiveCounter1Th = 0x400/0x2;
        }
        else
        {
            u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;
        }
    }
    else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 )
    {  //in filmany
        if( (u32CurCounter3 > (_u32Counter3_2[ucWin]*10))||(_u32Counter3_2[ucWin] > (u32CurCounter3*10))||(_u32Counter3_3[ucWin] > (_u32Counter3_2[ucWin]*10)))
        {
            u32Film22AdaptiveCounter1Th = _u32Counter1_2[ucWin] * 2;

            if( u32Film22AdaptiveCounter1Th < 0x400/0x4)
                u32Film22AdaptiveCounter1Th = 0x400/0x4;
        }
        else
        {
            u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

            if( u32Film22AdaptiveCounter1Th < (0x400/0x4))
                u32Film22AdaptiveCounter1Th = 0x400/0x4;
        }
    }
    else
    {
        if(_u8SmallCounter3Cnt[ucWin]==10)
        {
            if((_u8Film22LikeCounterA[ucWin]>5) || (_u8Film22LikeCounterB[ucWin]>5))
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

                if( u32Film22AdaptiveCounter1Th > 0x400/0x2 )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;

            }
            else
            {
                u32Film22AdaptiveCounter1Th = 0x400/0x2;
            }
        }
        else
        {
            u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

            if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                u32Film22AdaptiveCounter1Th = 0x400/0x2;
        }

        if(_u8Film22LikeCounter3_Big[ucWin]>8)
            u32Film22AdaptiveCounter1Th  = 0xFFFF;
    }


    if( u32Film22AdaptiveCounter1Th < 0x400/0x4)
        u32Film22AdaptiveCounter1Th = 0x400/0x4;

    _prvMaxErrLine[ucWin] = curMaxErrLine[ucWin];
    _u32Counter3_3[ucWin] = _u32Counter3_2[ucWin];
    _u32Counter3_2[ucWin]  = u32CurCounter3;
    _u32Counter1_4[ucWin] = _u32Counter1_3[ucWin];
    _u32Counter1_3[ucWin] = _u32Counter1_2[ucWin];
    _u32Counter1_2[ucWin]  = u32CurCounter1;

    _u32CounterA_3[ucWin] = _u32CounterA_2[ucWin];
    _u32CounterA_2[ucWin]  = u32CurCounterA;
    _u32CounterB_3[ucWin] = _u32CounterB_2[ucWin];
    _u32CounterB_2[ucWin]  = u32CurCounterB;

    bIsFilmXOO = (_u8IsFilmXOOCounter[ucWin] != 0);

    bFilm = ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008) ||  // film22
            ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010) ||  // film32
            ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040) ;   // film any
    bFilm32 = ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010);

#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    u32Reg7C = _Read2Byte(ucWin, REG_SC_BK2A_7C_L);
    u32Reg7D = _Read2Byte(ucWin, REG_SC_BK2A_7D_L);
    u32Reg7E = _Read2Byte(ucWin, REG_SC_BK2A_7E_L);

    // for all chip
    if( bIsHDSource )
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0200) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
    else
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0100) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
#else
    // for no MCNR
    bVerticalMovingU = false;
#endif


    u16Unmatch_3 = _Read2Byte(ucWin, REG_SC_BK0A_0B_L);
    u16Unmatch_1 = _Read2Byte(ucWin, REG_SC_BK0A_0A_L);
    curMaxErrLine[ucWin] = _Read2Byte(ucWin, REG_SC_BK0A_44_L) & 0xff;


                if ( curMaxErrLine[ucWin] < 0x8 || _prvMaxErrLine[ucWin] < 0x8 )
                {
                    if ( continuousNotVideoOverFilm[ucWin] < 0xf0 )
                      continuousNotVideoOverFilm[ucWin] ++;
                }
                else
                    continuousNotVideoOverFilm[ucWin] = 0;

    // for sec , activate counter 1
    if( u16Unmatch_3 < 0x40 )
    {
        _u8NoCounter3_Cnt[ucWin] ++;
    }
    else
    {
        _u8NoCounter3_Cnt[ucWin] = 0;
    }

    // deactivate counter 1 jump out
    if( u16Unmatch_3 > 0x6000  )
    {
        _u8BigCounter3_Cnt[ucWin] = 10;
    }
    else if(_u8BigCounter3_Cnt[ucWin] > 0)
    {
        _u8BigCounter3_Cnt[ucWin] --;
    }

    if( u16Unmatch_3 > 0x1000  )
    {
        _u8BigCounter3_Cnt2[ucWin] = 10;
    }
    else if(_u8BigCounter3_Cnt2[ucWin] > 0)
    {
        _u8BigCounter3_Cnt2[ucWin] --;
    }

    // deactivate counter 1 jump out
#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    if( u16Unmatch_1 > 0xF000  )
#else
    if( u16Unmatch_1 > 0xA000  )
#endif
    {
        _u8BigCounter1_Cnt[ucWin] = 10;
    }
    else if(_u8BigCounter1_Cnt[ucWin] > 0)
    {
        _u8BigCounter1_Cnt[ucWin] --;
    }

    if( u16Unmatch_1 > 0xc000)
    {
        if( _u8BigCounter_Cnt3[ucWin] < 80)
            _u8BigCounter_Cnt3[ucWin] ++;
    }
    else
        _u8BigCounter_Cnt3[ucWin] = 0;

    if( u16Unmatch_1 > 0x1000 )
        _u8BigCounter1_Cnt2[ucWin] = 0x8;
    else if( _u8BigCounter1_Cnt2[ucWin] != 0)
        _u8BigCounter1_Cnt2[ucWin] --;

    if( u16Unmatch_1 < 0x0800 )
    {
        _u8SmallCounter1_Cnt[ucWin] = 10;
    }
    else if(_u8SmallCounter1_Cnt[ucWin] > 0)
    {
        _u8SmallCounter1_Cnt[ucWin] --;
    }

    if((MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2500) || (MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2700) )//(stXCStatus.bInterlace)
    {
        if(_u8NoCounter3_Cnt[ucWin] > 20)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x01)<<8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10)<<8, 0xFF00);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x04)<<8, 0x1F00);  // for SD cross hatch
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10)<<8, 0xFF00);
        }

        // Film32 On,f2 32 film mode enable
        if( (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(14) ) ==  BIT(14) )
           || (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(15) ) ==  BIT(15) ) )
        {
            if(!_MApi_XC_FrameLockCheck() ) // Make not frame LOCKED then turn off FilmMode
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, BIT(1), BIT(1)); //hardware film mode DISABLE
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, BIT(1)|BIT(2), BIT(1) | BIT(2)); //software film mode DISABLE
            }
            else                                   // Make sure frame LOCKED then turn on FilmMode
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1)); //hardware film mode ENABLE
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2)); //software film mode ENABLE

                ///--> new add function for skip film mode in still pattern
                if((!MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)))&&(!MApi_XC_R2BYTEMSK(REG_SC_BK0A_02_L, BIT(14))))    // 2R mode can't enter this loop
                {
                    u16Unmatch_3 = _Read2Byte(ucWin, REG_SC_BK0A_0B_L);

                    if( (u16Unmatch_3 > 0x80) )
                        _u8Unmatch3Cnt[ucWin] = 0;
                    else if(_u8Unmatch3Cnt[ucWin] == 0xFF)
                        _u8Unmatch3Cnt[ucWin] = 0xFF;
                    else
                        _u8Unmatch3Cnt[ucWin] = _u8Unmatch3Cnt[ucWin] + 1;

                    ///--> Jump out Film for LG tower
                    if( !bVerticalMovingU )
                        _u8VerticalCnt[ucWin] = 0;
                    else if(_u8VerticalCnt[ucWin] == 0xFF)
                        _u8VerticalCnt[ucWin] = 0xFF;
                    else
                        _u8VerticalCnt[ucWin] = _u8VerticalCnt[ucWin] + 1;
                }
            }
        }
        else
        {
            // DEFAULT value of hardware and software film mode suggested by SY.Shih
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1));
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2));
        }

        // film-any continuous error line patch, not easy to enter film, but easy to exit film
        if ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 ) // film-any status
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_57_L, 0x08, 0xff);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_57_L, 0x20, 0xff);

        if ( bIsHDSource && (!bFilm) )
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_15_L, 0x0008, 0x000F ); // for film32, also favor de-bouncing
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_15_L, 0x0008, 0x000F ); // for De-bouncing

        if(_u8VerticalCnt[ucWin]>0x03)          // only active at Film
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
        }
        else
        {
            if((u32CurCounter3<0x9) && (u32CurCounter1<0x9))
            {
                _u8StillCounter3[ucWin]++;
            }
            else
            {
                _u8StillCounter3[ucWin] = 0;
            }

            if(_u8StillCounter3[ucWin]>=8)
            {
                _u8StillCounter3[ucWin] = 8;
            }

            if(u32Film22AdaptiveCounter1Th>0xFFFF)
                u32Film22AdaptiveCounter1Th = 0xFFFF;

            MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, u32Film22AdaptiveCounter1Th );


            if(_u8StillCounter3[ucWin]==8 || KURO_patch_en==1 || MSPG4233_patch_en==1)
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0 );
                MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, 0 );
            }

            if( bIs60Hz ) //NTSC -> turn off fast film22
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
            else // PAL
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, BIT(15), BIT(15));
        }

        // for SEC pattern gen
        if(_u8NoCounter3_Cnt[ucWin] > 20)
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x00FF, 0xFFFF); // disable film-any
        else if ( !bIsFilmXOO )
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0080, 0xFFFF); // disable film-any
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0808, 0xFFFF);

        // for auto 1
        // film32
        u32Counter3  = _Read2Byte(ucWin, REG_SC_BK0A_0B_L);


        if( u32Counter3 < 0x100 )
            _u8MovingCharacterCounter[ucWin] = 0; //
        else if(_u8MovingCharacterCounter[ucWin] != 0xff )
            _u8MovingCharacterCounter[ucWin] ++;

        if ( MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, BIT(12)) ) // auto 1 , deal with video-over-film
        {
            if ( bIsHDSource )
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_06_L, 0xBF3A);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, BIT(13),BIT(13) );
                MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0xAFFF);
            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_06_L, 0xA810);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, 0x00 ,BIT(13) );
                MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x4FFF);
            }

            MApi_XC_W2BYTE(REG_SC_BK0A_16_L, 0x0058);
            MApi_XC_W2BYTE(REG_SC_BK0A_1E_L, 0xFFF1);
        }
        else // default
        {
            if ( bIsHDSource )
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_06_L, 0xCF40);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, BIT(13),BIT(13) );
                MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x0fff);
            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_06_L, 0xA810);
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, 0x00 ,BIT(13) );

                if ( continuousNotVideoOverFilm[ucWin] > 0x10 )
                    MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x0800);
                else
                  MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x0250);
            }

            MApi_XC_W2BYTE(REG_SC_BK0A_16_L, 0x00D8);
            MApi_XC_W2BYTE(REG_SC_BK0A_1E_L, 0x0551);
        }

        if ( MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, BIT(12)) ) // auto 1 , deal with video-over-film
        {
            if ( bIsHDSource )
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_07_L, 0xBF40);
            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_07_L, 0x7F30);
            }
        }
        else // default
        {
            if ( bIsHDSource )
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_07_L, 0x7F10);

            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_07_L, 0x7F08);
            }
        }
    } // end interlace loop
    _u16RdBank_Prv[ucWin] = u16RdBank;
#if 0
    if( ( MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK01_42_L, 0x0001) == 0x1 )
    {
        if( ( MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008 )
        {

            printk("in22 \n");
        }
        else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010 )
        {
            printk("in32 \n");
        }
        else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 )
        {
            printk("inany \n");
        }
        else
        {
            printk("out \n");
        }

        printk("um3 = %lx, um1 = %lx,reg3 = %x, reg1 = %x , 4a= %x , 4b= %x, idx= %lx \n",u32CurCounter3, u32CurCounter1,MApi_XC_R2BYTE(REG_SC_BK0A_0D_L),MApi_XC_R2BYTE(REG_SC_BK0A_0C_L) ,MApi_XC_R2BYTE(REG_SC_BK0A_4A_L),MApi_XC_R2BYTE(REG_SC_BK0A_4B_L),u16RdBank);
    }
#endif
}

void MDrv_PQ_Film_MNT_PatchOnly(BYTE ucWin)
{
    MS_U8   _scIdx;
    MS_BOOL bVerticalPatch[DISPLAY_MAX_NUMS] = {FALSE}, bKuroPatch[DISPLAY_MAX_NUMS] = {FALSE};
    MS_U32 u32BkSC0A_23[DISPLAY_MAX_NUMS], u32BkSC0A_0C[DISPLAY_MAX_NUMS], u32BkSC0A_0D[DISPLAY_MAX_NUMS];

    static MS_U16 _u16RdBank_Prv[DISPLAY_MAX_NUMS]={0};
    MS_U16 u16RdBank;
    MS_U32 u32CurCounter1=0, u32CurCounter3=0, u32FixThofC3=0;

    static MS_U32 _u32Counter3_2[DISPLAY_MAX_NUMS]={0}, _u32Counter3_3[DISPLAY_MAX_NUMS]={0};
    static MS_U32 _u32Counter1_2[DISPLAY_MAX_NUMS]={0}, _u32Counter1_3[DISPLAY_MAX_NUMS]={0}, _u32Counter1_4[DISPLAY_MAX_NUMS]={0};
    static MS_U8 _u8VerticalCnt[DISPLAY_MAX_NUMS];
    MS_BOOL bIsHDSource;

    MS_BOOL bVerticalMovingU ;
#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    MS_U32 u32Reg7C, u32Reg7D, u32Reg7E;
#endif
    if (u32ScBaseAddr == 0x102D00)
        _scIdx = 0;
    else if (u32ScBaseAddr == 0x102B00)
        _scIdx = 1;
    else if (u32ScBaseAddr == 0x103100)
        _scIdx = 2;
    else
        _scIdx = 3;
    u16RdBank = ( MApi_XC_R2BYTE(REG_SC_BK12_3A_L) & 0xF0 ) >> 4;
    bIsHDSource = ( MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF) > 0x700 );


    u32CurCounter1 = _Read2Byte(ucWin, REG_SC_BK0A_0A_L);
    u32CurCounter3 = _Read2Byte(ucWin, REG_SC_BK0A_0B_L);

    if(u16RdBank == _u16RdBank_Prv[ucWin])  // Not change read bank
    {
        return ;
    }





    MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, u32FixThofC3);
    MApi_XC_W2BYTE(REG_SC_BK0A_41_L, 0xFFFF);
    MApi_XC_W2BYTE(REG_SC_BK0A_5C_L, 0);
    MApi_XC_W2BYTE(REG_SC_BK0A_5E_L, 0);
    MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, 0, 0x0F );
    MApi_XC_W2BYTE(REG_SC_BK0A_20_L, 0x10);



    //for KURO patch
    MS_U16 KURO_patch_en = 0;
    {
        if( u32CurCounter3 >_u32Counter3_2[ucWin]+0x20 && u32CurCounter1+0x20 < _u32Counter1_2[ucWin] && u32CurCounter1==0 )
            KURO_patch_en = 1;
        else if( u32CurCounter3 <_u32Counter3_2[ucWin]+0x20 && u32CurCounter1 > _u32Counter1_2[ucWin]+0x20 && _u32Counter1_2[ucWin]==0 )
            KURO_patch_en = 1;
    }

    //for SEC pattern Gen.
    MS_U16 MSPG4233_patch_en = 0;
    {
        if( u32CurCounter3 >_u32Counter3_2[ucWin]+0x20 && u32CurCounter1+0x20 < _u32Counter1_2[ucWin] && _u32Counter3_2[ucWin]==0 )
            MSPG4233_patch_en = 1;
        else if( u32CurCounter3 <_u32Counter3_2[ucWin]+0x20 && u32CurCounter1 > _u32Counter1_2[ucWin]+0x20 && u32CurCounter3==0 )
            MSPG4233_patch_en = 1;
    }

    _u32Counter3_3[ucWin] = _u32Counter3_2[ucWin];
    _u32Counter3_2[ucWin]  = u32CurCounter3;
    _u32Counter1_4[ucWin] = _u32Counter1_3[ucWin];
    _u32Counter1_3[ucWin] = _u32Counter1_2[ucWin];
    _u32Counter1_2[ucWin]  = u32CurCounter1;






#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    u32Reg7C = _Read2Byte(ucWin, REG_SC_BK2A_7C_L);
    u32Reg7D = _Read2Byte(ucWin, REG_SC_BK2A_7D_L);
    u32Reg7E = _Read2Byte(ucWin, REG_SC_BK2A_7E_L);

    // for all chip
    if( bIsHDSource )
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0200) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
    else
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0100) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
#else
    // for no MCNR
    bVerticalMovingU = false;
#endif




    if((MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2500) || (MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2700) )//(stXCStatus.bInterlace)
    {


        // Film32 On,f2 32 film mode enable
        if( (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(14) ) ==  BIT(14) )
           || (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(15) ) ==  BIT(15) ) )
        {
            if(!_MApi_XC_FrameLockCheck() ) // Make not frame LOCKED then turn off FilmMode
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, BIT(1), BIT(1)); //hardware film mode DISABLE
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, BIT(1)|BIT(2), BIT(1) | BIT(2)); //software film mode DISABLE
            }
            else                                   // Make sure frame LOCKED then turn on FilmMode
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1)); //hardware film mode ENABLE
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2)); //software film mode ENABLE

                ///--> new add function for skip film mode in still pattern
                if((!MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)))&&(!MApi_XC_R2BYTEMSK(REG_SC_BK0A_02_L, BIT(14))))    // 2R mode can't enter this loop
                {


                    ///--> Jump out Film for LG tower
                    if( !bVerticalMovingU )
                        _u8VerticalCnt[ucWin] = 0;
                    else if(_u8VerticalCnt[ucWin] == 0xFF)
                        _u8VerticalCnt[ucWin] = 0xFF;
                    else
                        _u8VerticalCnt[ucWin] = _u8VerticalCnt[ucWin] + 1;
                }
            }
        }
        else
        {
            // DEFAULT value of hardware and software film mode suggested by SY.Shih
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1));
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2));
        }



        if(_u8VerticalCnt[ucWin]>0x03)          // only active at Film
        {
            u32BkSC0A_23[_scIdx] = MApi_XC_R2BYTEMSK(REG_SC_BK0A_23_L, BIT(15));
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
            bVerticalPatch[_scIdx] = TRUE;

        }
        else
        {
            if (bVerticalPatch[_scIdx])
            {
                bVerticalPatch[_scIdx] = FALSE;
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, u32BkSC0A_23[_scIdx], BIT(15));
            }

            if( KURO_patch_en==1 || MSPG4233_patch_en==1)
            {
                u32BkSC0A_0C[_scIdx] = MApi_XC_R2BYTE(REG_SC_BK0A_0C_L);
                u32BkSC0A_0D[_scIdx] = MApi_XC_R2BYTE(REG_SC_BK0A_0D_L);
                MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0 );
                MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, 0 );
                bKuroPatch[_scIdx] = TRUE;
            }
            else
            {
                if (bKuroPatch[_scIdx])
                {
                    bKuroPatch[_scIdx] = FALSE;
                    MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, u32BkSC0A_0C[_scIdx] );
                    MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, u32BkSC0A_0D[_scIdx] );
                }
            }




        }


    } // end interlace loop
    _u16RdBank_Prv[ucWin] = u16RdBank;
#if 0
    if( ( MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK01_42_L, 0x0001) == 0x1 )
    {
        if( ( MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008 )
        {

            printk("in22 \n");
        }
        else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010 )
        {
            printk("in32 \n");
        }
        else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 )
        {
            printk("inany \n");
        }
        else
        {
            printk("out \n");
        }

        printk("um3 = %lx, um1 = %lx,reg3 = %x, reg1 = %x , 4a= %x , 4b= %x, idx= %lx \n",u32CurCounter3, u32CurCounter1,MApi_XC_R2BYTE(REG_SC_BK0A_0D_L),MApi_XC_R2BYTE(REG_SC_BK0A_0C_L) ,MApi_XC_R2BYTE(REG_SC_BK0A_4A_L),MApi_XC_R2BYTE(REG_SC_BK0A_4B_L),u16RdBank);
    }
#endif
}

#ifndef PQ_FILM_DRIVER_VER
#define PQ_FILM_DRIVER_VER               2
#endif
void MDrv_PQ_AdaptiveTuning(BYTE ucWin)
{

    SC_ApiStatus status;
    status.bInterlace = _stMode_Info[PQ_MAIN_WINDOW].bInterlace;
    status.u16InputVFreq = _stMode_Info[PQ_MAIN_WINDOW].u16input_vfreq;
    Hal_PQ_SetStatus(&status);
    Hal_PQ_AdaptiveTuning(u32ScBaseAddr, ucWin);
#if 1
#if (PQ_FILM_DRIVER_VER == 1)
    MDrv_PQ_FilmDriverHWVer1();
#elif (PQ_FILM_DRIVER_VER == 2)
    MDrv_PQ_FilmDriverHWVer2(ucWin);
#elif (PQ_FILM_DRIVER_VER == 0)
    MDrv_PQ_Film_MNT_PatchOnly(ucWin);
#endif
#endif

}

void _MDrv_PQ_BackupTbl(PQ_WIN eWin)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask, idx;
    MS_U8 u8CurBank = 0xff;

#if (PQ_ONLY_SUPPORT_BIN == 0)
    EN_IP_Info _sByPassIP;


    //get bypass table info(reg and mask value)
    if (eWin == PQ_MAIN_WINDOW)
        _sByPassIP = MDrv_PQ_GetTable_(MAIN, PQ_IP_COMM, PQ_IP_PQ_ByPass_Main);
    else
        _sByPassIP = MDrv_PQ_GetTable_(SUB, PQ_IP_COMM, PQ_IP_PQ_ByPass_Sub);
    idx = 0;
    while (1)
    {
        u32Addr = (_sByPassIP.pIPTable[0]<<8) + _sByPassIP.pIPTable[1];
        u8Mask  = _sByPassIP.pIPTable[2];

        if (u32Addr == _END_OF_TBL_) // check end of table
            break;

        u8CurBank = u32Addr >> 8;
        PQ_DUMP_DBG(printf("XC bk =%x, addr=%x, msk=%x, value=%x\n", (MS_U16)((u32Addr&0xFF00)>>8), (MS_U16)(u32Addr&0xFF), (MS_U16)u8Mask, (MS_U16)u8Value));


        //u32Addr = u32ScBaseAddr | (u32Addr & 0x00FF);

		PQ_DUMP_DBG(printf("[PQ_REG_FUNC]: addr = %02x, value = %x, mask = %x\n", u32Addr, u8Value, u8Mask));

        //PQ_REG_FUNC(u32Addr, u8Value, u8Mask);
        if (eWin == PQ_MAIN_WINDOW)
            _u8PQ_Backup_BypassTbl_Main[idx++] =  MApi_XC_ReadByteMask(u32Addr, u8Mask);
        else
            _u8PQ_Backup_BypassTbl_Sub[idx++] =  MApi_XC_ReadByteMask(u32Addr, u8Mask);
        _sByPassIP.pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+_sByPassIP.u8TabNums); // next
    }


#else

    MS_PQBin_IP_Table_Info stTableInfo;
    BYTE bin_id, u8Addr;
    MS_U32 u32Offset, u32RegAddr;
    if (eWin == PQ_MAIN_WINDOW)
        bin_id = 0;
    else
        bin_id = 1;
    MDrv_PQBin_GetTable(MDrv_PQBin_GetPanelIdx(), PQ_BIN_IP_COMM, PQ_IP_PQ_ByPass_Main, &stTableInfo, &stPQBinHeaderInfo[bin_id]);

    idx = 0;
    u32Addr = stTableInfo.u32TabOffset;
    for(idx=0; idx<stTableInfo.u16RegNum; idx++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)stTableInfo.u16GroupNum) * (MS_U32)idx;
        u8CurBank = MDrv_PQBin_GetByteData((u32Addr+u32Offset), &u32Offset);
        u8Addr    = MDrv_PQBin_GetByteData(u32Addr+u32Offset, &u32Offset);
        u8Mask    = MDrv_PQBin_GetByteData(u32Addr+u32Offset, &u32Offset);

        u32Offset +=  (MS_U32)stTableInfo.u16GroupIdx;
        u32RegAddr = (u8CurBank << 8) | u8Addr;
        if (eWin == PQ_MAIN_WINDOW)
            _u8PQ_Backup_BypassTbl_Main[idx] =  MApi_XC_ReadByteMask(u32RegAddr, u8Mask);
        else
            _u8PQ_Backup_BypassTbl_Sub[idx] =  MApi_XC_ReadByteMask(u32RegAddr, u8Mask);
    }

#endif
}


void _MDrv_PQ_BackupByPass(PQ_WIN eWin)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask, idx;
    MS_U8 u8CurBank = 0xff;

#if (PQ_ONLY_SUPPORT_BIN == 0)
    EN_IP_Info _sByPassIP;

    //get bypass table info(reg and mask value)
    if (eWin == PQ_MAIN_WINDOW)
        _sByPassIP = MDrv_PQ_GetTable_(MAIN, PQ_IP_COMM, PQ_IP_PQ_ByPass_Main);
    else
        _sByPassIP = MDrv_PQ_GetTable_(SUB, PQ_IP_COMM, PQ_IP_PQ_ByPass_Sub);



    idx = 0;
    while (1)
    {
        u32Addr = (_sByPassIP.pIPTable[0]<<8) + _sByPassIP.pIPTable[1];
        u8Mask  = _sByPassIP.pIPTable[2];

        if (u32Addr == _END_OF_TBL_) // check end of table
            break;

        u8CurBank = u32Addr >> 8;
        PQ_DUMP_DBG(printf("XC bk =%x, addr=%x, msk=%x, value=%x\n", (MS_U16)((u32Addr&0xFF00)>>8), (MS_U16)(u32Addr&0xFF), (MS_U16)u8Mask, (MS_U16)u8Value));


        //u32Addr = u32ScBaseAddr | (u32Addr & 0x00FF);

		PQ_DUMP_DBG(printf("[PQ_REG_FUNC]: addr = %02x, value = %x, mask = %x\n", u32Addr, u8Value, u8Mask));

        if (eWin == PQ_MAIN_WINDOW)
            MApi_XC_WriteByteMask(u32Addr, _u8PQ_Backup_BypassTbl_Main[idx++], u8Mask);
        else
            MApi_XC_WriteByteMask(u32Addr, _u8PQ_Backup_BypassTbl_Sub[idx++], u8Mask);
        _sByPassIP.pIPTable+=(REG_ADDR_SIZE+REG_MASK_SIZE+_sByPassIP.u8TabNums); // next
    }

#else
    MS_PQBin_IP_Table_Info stTableInfo;
    BYTE bin_id, u8Addr;
    MS_U32 u32Offset, u32RegAddr;
    if (eWin == PQ_MAIN_WINDOW)
        bin_id = 0;
    else
        bin_id = 1;
    MDrv_PQBin_GetTable(MDrv_PQBin_GetPanelIdx(), PQ_BIN_IP_COMM, PQ_IP_PQ_ByPass_Main, &stTableInfo, &stPQBinHeaderInfo[bin_id]);

    idx = 0;
    u32Addr = stTableInfo.u32TabOffset;
    for(idx=0; idx<stTableInfo.u16RegNum; idx++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)stTableInfo.u16GroupNum) * (MS_U32)idx;
        u8CurBank = MDrv_PQBin_GetByteData((u32Addr+u32Offset), &u32Offset);
        u8Addr    = MDrv_PQBin_GetByteData(u32Addr+u32Offset, &u32Offset);
        u8Mask    = MDrv_PQBin_GetByteData(u32Addr+u32Offset, &u32Offset);

        u32Offset +=  (MS_U32)stTableInfo.u16GroupIdx;
        u32RegAddr = (u8CurBank << 8) | u8Addr;
        if (eWin == PQ_MAIN_WINDOW)
            MApi_XC_WriteByteMask(u32RegAddr, _u8PQ_Backup_BypassTbl_Main[idx], u8Mask);
        else
            MApi_XC_WriteByteMask(u32RegAddr, _u8PQ_Backup_BypassTbl_Sub[idx], u8Mask);
    }

#endif

}



void MDrv_PQ_ByPassMode(PQ_WIN eWindow, MS_U8 bOn)
{

    if (bOn)
    {
#if (PQ_ONLY_SUPPORT_BIN == 0)
        EN_IP_Info  sByPassTbl;
        _MDrv_PQ_BackupTbl(eWindow == PQ_MAIN_WINDOW? PQ_MAIN_WINDOW : PQ_SUB_WINDOW);

        if (eWindow == PQ_MAIN_WINDOW)
        {
            sByPassTbl = MDrv_PQ_GetTable_(MAIN, PQ_IP_COMM, PQ_IP_PQ_ByPass_Main);
            MDrv_PQ_DumpTable_(MAIN, &sByPassTbl);
        }
        else
        {
            sByPassTbl = MDrv_PQ_GetTable_(SUB, PQ_IP_COMM, PQ_IP_PQ_ByPass_Sub);
            MDrv_PQ_DumpTable_(SUB, &sByPassTbl);
        }
#else
        MS_PQBin_IP_Table_Info stTableInfo;
        BYTE bin_id;
        _MDrv_PQ_BackupTbl(eWindow == PQ_MAIN_WINDOW? PQ_MAIN_WINDOW : PQ_SUB_WINDOW);
        if (eWindow == PQ_MAIN_WINDOW)
            bin_id = 0;
        else
            bin_id = 1;
        MDrv_PQBin_GetTable(MDrv_PQBin_GetPanelIdx(), PQ_BIN_IP_COMM, PQ_IP_PQ_ByPass_Main, &stTableInfo, &stPQBinHeaderInfo[bin_id]);
        MDrv_PQBin_DumpTable(&stTableInfo);
#endif
    }
    else
    {
        _MDrv_PQ_BackupByPass(eWindow == PQ_MAIN_WINDOW? PQ_MAIN_WINDOW : PQ_SUB_WINDOW);
    }


}

