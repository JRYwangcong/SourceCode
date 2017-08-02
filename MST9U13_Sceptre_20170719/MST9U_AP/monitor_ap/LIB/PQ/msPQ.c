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


#include "Global.h"
#include "appWindow.h"

#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#if ENABLE_PQ_BIN
#include "drvPQ_Bin.h"
#endif
#include "menufunc.h"
#include "msPQ.h"

#define MSPQ_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && MSPQ_DEBUG
#define MSPQ_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MSPQ_PRINT(format, ...)
#endif

BYTE g_u8PQEnable = 1;
XDATA BYTE g_bPQ_Adaptive_On[DISPLAY_MAX_NUMS];
XDATA BYTE g_bPQ_Busy_On;
XDATA MS_U16 g_ucMADiIdx[PQ_MAX_WINDOW][DISPLAY_MAX_NUMS];
XDATA MS_U16 g_uc10to8Dither[PQ_MAX_WINDOW][DISPLAY_MAX_NUMS];
XDATA BYTE g_ucForceYUV[DISPLAY_MAX_NUMS];
XDATA BYTE g_ucIsDownScaling[DISPLAY_MAX_NUMS];

XDATA PQ_DISPLAY_TYPE g_ucMultiWinMode = PQ_DISPLAY_NONE;
static BYTE g_IsVideoMode[DISPLAY_MAX_NUMS] = {0};

#define PQ_BIN_MAIN_ADDRESS     0x100000
#define PQ_BIN_MAIN_EX_ADDRESS  0x170000
#define PQ_BIN_SUB_ADDRESS      0x180000
#define PQ_BIN_SUB_EX_ADDRESS   0x1D0000


#define PQ_ELEM_NUM_MA  7
MS_U16 PQ_TBL_MODE_MA[][PQ_ELEM_NUM_MA] =
{
    {PQ_IP_MADi_Main,                       PQ_IP_MADi_Motion_Main,             PQ_IP_MADi_MORPHO_Main,     PQ_IP_MADi_DFK_Main,        PQ_IP_MADi_SST_Main,        PQ_IP_MADi_EODiW_Main,      PQ_IP_MADi_Force_Main},
    {PQ_IP_MADi_OFF_Main,                   PQ_IP_MADi_Motion_MOT_PMODE_Main,   PQ_IP_MADi_MORPHO_OFF_Main, PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_P_MODE_ROTATE_Main,         PQ_IP_MADi_Motion_MOT_PMODE_Main,   PQ_IP_MADi_MORPHO_OFF_Main, PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_P_MODE8_444_Main,           PQ_IP_MADi_Motion_MOT_PMODE_Main,   PQ_IP_MADi_MORPHO_OFF_Main, PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_P_MODE_MOT8_Main,           PQ_IP_MADi_Motion_MOT_PMODE_Main,   PQ_IP_MADi_MORPHO_OFF_Main, PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_P_MODE_MC_3Frame_Main,      PQ_IP_MADi_Motion_MOT_PMODE_Main,   PQ_IP_MADi_MORPHO_OFF_Main, PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_P_MODE_MC_6Frame_6R_Main,   PQ_IP_MADi_Motion_MOT_PMODE_Main,   PQ_IP_MADi_MORPHO_OFF_Main, PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_P_MODE_MC_12Frame_8R_Main,  PQ_IP_MADi_Motion_MOT_PMODE_Main,   PQ_IP_MADi_MORPHO_OFF_Main, PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_25_4R_Main,                 PQ_IP_MADi_Motion_MOT_4R_5_Main,    PQ_IP_MADi_MORPHO_OFF_Main,  PQ_IP_MADi_DFK_OFF_Main,   PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_25_4R_MC_Main,              PQ_IP_MADi_Motion_MOT_4R_5_MC_Main, PQ_IP_MADi_MORPHO_M1_Main,  PQ_IP_MADi_DFK_DFK1_Main,   PQ_IP_MADi_SST_SST3_Main,   PQ_IP_MADi_EODiW_OFF_Main,  PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_25_14F_6R_MC_Main,          PQ_IP_MADi_Motion_MOT_4R_5_MC_Main, PQ_IP_MADi_MORPHO_M1_Main,  PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_W2_Main,   PQ_IP_MADi_Force_OFF_Main},
    {PQ_IP_MADi_25_14F_8R_MC_Main,          PQ_IP_MADi_Motion_MOT_4R_5_MC_Main, PQ_IP_MADi_MORPHO_M1_Main,  PQ_IP_MADi_DFK_OFF_Main,    PQ_IP_MADi_SST_OFF_Main,    PQ_IP_MADi_EODiW_W2_Main,   PQ_IP_MADi_Force_OFF_Main},
};

#define PQ_ELEM_NUM_EO  4
MS_U16 PQ_TBL_MODE_EO[][PQ_ELEM_NUM_EO] =
{
    {PQ_IP_EODi_Main,       PQ_IP_EODi_Armored_Main,        PQ_IP_EODi_Armored_KFC_Main,        PQ_IP_EODi_Armored_UCDi_Main},
    {PQ_IP_EODi_OFF_Main,   PQ_IP_EODi_Armored_OFF_Main,    PQ_IP_EODi_Armored_KFC_OFF_Main,    PQ_IP_EODi_Armored_UCDi_OFF_Main},
    {PQ_IP_EODi_SD_8_Main,  PQ_IP_EODi_Armored_OFF_Main,    PQ_IP_EODi_Armored_KFC_OFF_Main,    PQ_IP_EODi_Armored_UCDi_OFF_Main},
    {PQ_IP_EODi_SD_8_Main,  PQ_IP_EODi_Armored_EW1_Main,    PQ_IP_EODi_Armored_KFC_OFF_Main,    PQ_IP_EODi_Armored_UCDi_OFF_Main}
};

#define PQ_ELEM_NUM_UC  3
MS_U16 PQ_TBL_MODE_UC[][PQ_ELEM_NUM_UC] =
{
    {PQ_IP_UCNR_Main,       PQ_IP_UCDi_Main,        PQ_IP_UC_CTL_Main},
    {PQ_IP_UCNR_OFF_Main,   PQ_IP_UCDi_OFF_Main,    PQ_IP_UC_CTL_OFF_Main},
    {PQ_IP_UCNR_SD_2_Main,  PQ_IP_UCDi_OFF_Main,    PQ_IP_UC_CTL_ON_Main},
    {PQ_IP_UCNR_SD_2_Main,  PQ_IP_UCDi_T12_Main,    PQ_IP_UC_CTL_ON_DHD_Main},
    {PQ_IP_UCNR_HD_1_Main,  PQ_IP_UCDi_OFF_Main,    PQ_IP_UC_CTL_ON_UCNR_Main},
    {PQ_IP_UCNR_HD_Main,    PQ_IP_UCDi_OFF_Main,    PQ_IP_UC_CTL_ON_UCNR_Main}
};

#define PQ_ELEM_NUM_FILM  4
MS_U16 PQ_TBL_MODE_FILM[][PQ_ELEM_NUM_FILM] =
{
    {PQ_IP_Film_Main,       PQ_IP_Film32_Main,      PQ_IP_Film22_Main,      PQ_IP_Film_any_Main},
    {PQ_IP_Film_OFF_Main,   PQ_IP_Film32_OFF_Main,  PQ_IP_Film22_OFF_Main,  PQ_IP_Film_any_OFF_Main},
    {PQ_IP_Film_SD_2_Main,  PQ_IP_Film32_OFF_Main,  PQ_IP_Film22_OFF_Main,  PQ_IP_Film_any_OFF_Main},
    {PQ_IP_Film_SD_2_Main,  PQ_IP_Film32_OFF_Main,  PQ_IP_Film22_SD_4_Main, PQ_IP_Film_any_OFF_Main},
    {PQ_IP_Film_SD_2_Main,  PQ_IP_Film32_SD_5_Main, PQ_IP_Film22_SD_10_Main,PQ_IP_Film_any_OFF_Main},
    {PQ_IP_Film_HD_4_Main,  PQ_IP_Film32_OFF_Main,  PQ_IP_Film22_HD_2_Main, PQ_IP_Film_any_OFF_Main},
    {PQ_IP_Film_HD_4_Main,  PQ_IP_Film32_HD_3_Main, PQ_IP_Film22_HD_2_Main, PQ_IP_Film_any_OFF_Main},
};

BYTE _GetScInedx(WORD dwScMask)
{
    BYTE u8SCIdx;
    WORD u8SCMaskTemp;

    u8SCMaskTemp = dwScMask;
    u8SCIdx = 0;
    while(u8SCMaskTemp!=0)
    {
        if(u8SCMaskTemp & BIT0)
        {
            return u8SCIdx;
        }
        u8SCMaskTemp >>=1;
        u8SCIdx ++;
    }
    return 0;

}

WORD _GetScMask(BYTE u8WinIdx, BOOL bMain)
{
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut | g_DisplayWindow[u8WinIdx].u16DwScMaskIn;

    WORD u16ScMaskTemp = u16ScalerMask;
    BYTE ucMainScCnt, ucSubScCnt;
    WORD u16ScMainMask, u16ScSubMask;

    BYTE u8ScIdx = 0;

    ucMainScCnt = 0; ucSubScCnt = 0;
    u16ScMainMask = u16ScSubMask = 0;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            if (u8ScIdx % 2)
            {
                u16ScMainMask |= (1<< u8ScIdx);
                ucMainScCnt++;
            }
            else
            {
                u16ScSubMask |= (1<<u8ScIdx);
                ucSubScCnt++;
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);

    if (bMain)
        return u16ScMainMask;
    else
        return u16ScSubMask;

}


void _InitPQTable(BYTE bMain)
{
    PQ_DISPLAY_TYPE eDispType;

    MSPQ_PRINT("[PQ Change Table] %s\n", bMain? "Main PQ" : "Sub PQ");

    eDispType = g_ucMultiWinMode;
    g_bPQ_Busy_On = TRUE;
    //initial PQ structure
    MS_PQ_Init_Info sXC_PQ_InitData;

    sXC_PQ_InitData.bDDR2 = FALSE;
    sXC_PQ_InitData.u32DDRFreq = 1866;
    sXC_PQ_InitData.u8BusWidth = 16;
    //sXC_PQ_InitData.u32miu0em_size = mminfo.miu0_mem_size;
    //sXC_PQ_InitData.u32miu1em_size = mminfo.miu1_mem_size;

    if(PanelWidth <= 2560)
        sXC_PQ_InitData.u16PnlWidth = 2560;
    else
        sXC_PQ_InitData.u16PnlWidth = 3840;

    sXC_PQ_InitData.u16PnlHeight = PanelHeight;

    //FIXME:
    // sXC_PQ_InitData.u16OSD_hsize = UI_OSD_PANE_W;
    sXC_PQ_InitData.u16OSD_hsize = PanelWidth;

    sXC_PQ_InitData.u16Pnl_vtotal = PanelVTotal;


    sXC_PQ_InitData.u8PQBinCnt = 0;
    sXC_PQ_InitData.u8PQTextBinCnt = 0;
    sXC_PQ_InitData.u8PQBinCustomerCnt = 0;

#if(ENABLE_PQ_BIN)
    MS_PQ_CAP_INFO stPQCaps;
    //BYTE *pu8MainBuf, *pu8MainExBuf;
    BYTE pu8MainBuf[0x30] = {0};
    BYTE pu8MainExBuf[0x30] = {0};
    BYTE pu8SubBuf[0x30] = {0};
    BYTE pu8SubExBuf[0x30] = {0};
    DWORD    dwFileLen;

    stPQCaps.bPIP_Supported = TRUE;
    Flash_ReadTbl(PQ_BIN_MAIN_ADDRESS, &pu8MainBuf[0], 0x28); //read header first
    if (pu8MainBuf[0] == 0x4D && pu8MainBuf[1] == 0x53 && pu8MainBuf[2] == 0x54)    //check if bin file exist in flash
    {
        printf ("PQ main Bin file is Exist...\n");
        dwFileLen = (pu8MainBuf[0x27] << 24) + (pu8MainBuf[0x26] << 16) + (pu8MainBuf[0x25] << 8) + pu8MainBuf[0x24];
        printf ("PQ main Bin file length = %d\n", dwFileLen);


        //main_ex ....
        Flash_ReadTbl(PQ_BIN_MAIN_EX_ADDRESS, &pu8MainExBuf[0], 0x28); //read header first
        dwFileLen = (pu8MainExBuf[0x27] << 24) + (pu8MainExBuf[0x26] << 16) + (pu8MainExBuf[0x25] << 8) + pu8MainExBuf[0x24];
        printf ("PQ main_Ex Bin file length = %d\n", dwFileLen);


        //.....

        if(pu8MainBuf && pu8MainExBuf)
        {
            sXC_PQ_InitData.u8PQBinCnt = 2;
            sXC_PQ_InitData.stPQBinInfo[0].u8PQID = (BYTE) PQ_BIN_STD_MAIN;
            sXC_PQ_InitData.stPQBinInfo[0].PQBin_PhyAddr = 0x100000;//(MS_PHYADDR) pu8MainBuf;
            sXC_PQ_InitData.stPQBinInfo[1].u8PQID = (BYTE) PQ_BIN_EXT_MAIN;
            sXC_PQ_InitData.stPQBinInfo[1].PQBin_PhyAddr = 0x170000;//(MS_PHYADDR)pu8MainExBuf;
        }

        // sub PQ... start
        Flash_ReadTbl(PQ_BIN_SUB_ADDRESS, &pu8SubBuf[0], 0x28); //read header first
        dwFileLen = (pu8SubBuf[0x27] << 24) + (pu8SubBuf[0x26] << 16) + (pu8SubBuf[0x25] << 8) + pu8SubBuf[0x24];
        MSPQ_PRINT ("PQ Sub Bin file length = %d\n", dwFileLen);


        //main_ex ....
        Flash_ReadTbl(PQ_BIN_SUB_EX_ADDRESS, &pu8SubExBuf[0], 0x28); //read header first
        dwFileLen = (pu8SubExBuf[0x27] << 24) + (pu8SubExBuf[0x26] << 16) + (pu8SubExBuf[0x25] << 8) + pu8SubExBuf[0x24];
        MSPQ_PRINT ("PQ main_Ex Bin file length = %d\n", dwFileLen);


            if(pu8SubBuf && pu8SubExBuf)
            {
                sXC_PQ_InitData.u8PQBinCnt = 4;
            sXC_PQ_InitData.stPQBinInfo[2].u8PQID = (BYTE) PQ_BIN_STD_SUB;
            sXC_PQ_InitData.stPQBinInfo[2].PQBin_PhyAddr = 0x180000;//(MS_PHYADDR) pu8MainBuf;
            sXC_PQ_InitData.stPQBinInfo[3].u8PQID = (BYTE) PQ_BIN_EXT_SUB;
            sXC_PQ_InitData.stPQBinInfo[3].PQBin_PhyAddr = 0x1D0000;//(MS_PHYADDR)pu8MainExBuf;
            }

        sXC_PQ_InitData.u8PQTextBinCnt = 0;
        sXC_PQ_InitData.u8PQBinCustomerCnt = 0 ;


        }
    else
    {
        printf ("PQ main Bin file is not Exist...\n");
        sXC_PQ_InitData.u8PQBinCnt = 0;
        sXC_PQ_InitData.u8PQTextBinCnt = 0;
        sXC_PQ_InitData.u8PQBinCustomerCnt = 0 ;
    }
#else
    sXC_PQ_InitData.u8PQBinCnt = 0;
    sXC_PQ_InitData.u8PQTextBinCnt = 0;
    sXC_PQ_InitData.u8PQBinCustomerCnt = 0 ;
#endif

    //check if need to use common write...

    g_bSkipCommTable = TRUE;
#if (PQ_ONLY_SUPPORT_BIN == 0)
    if (bMain)
    {
        MDrv_PQ_Init(PQ_MAIN_WINDOW, eDispType, &sXC_PQ_InitData);
    }
    else
    {
        MDrv_PQ_Init(PQ_SUB_WINDOW, eDispType, &sXC_PQ_InitData);
    }
#endif
    /*
    u16ScMaskTemp = bMain? bSC_MAIN: bSC_SUB;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            u32ScBaseAddr = SCRegBase(u8ScIdx);
            if (u8ScIdx % 2)
            {
                MDrv_PQ_Init(PQ_MAIN_WINDOW, eDispType, &sXC_PQ_InitData);
            }
            else
            {
                MDrv_PQ_Init(PQ_SUB_WINDOW, eDispType, &sXC_PQ_InitData);
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);
    */
}

void msPQ_Init(EN_PQ_DISPLAY_TYPE enMultiWindowMode)
{
    PQ_DISPLAY_TYPE eDispType;

    WORD u16ScalerMask = bSC_MAIN;
    BYTE u8ScIdx = 0;
    BYTE u8WinIdx = 0;

    /*
    if (USER_PREF_MULTI_WIN_MODE== eMULTIWIN_2_PIP)
        eDispType = PQ_DISPLAY_PIP;
    else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
        eDispType = PQ_DISPLAY_ONE;
    else
        eDispType = PQ_DISPLAY_POP;
    */
    eDispType = (PQ_DISPLAY_TYPE)enMultiWindowMode;

    MSPQ_PRINT("[PQ_Init] DisplayTYpe = %x\n", eDispType);

    if (g_ucMultiWinMode == eDispType)
    {
        MSPQ_PRINT("[PQ_Init] Display type not change... don't init PQ\n");
        return;
    }
    else
        g_ucMultiWinMode = eDispType;

    g_bPQ_Busy_On = TRUE;
    for (u8WinIdx = 0; u8WinIdx < DISPLAY_MAX_NUMS; u8WinIdx++)
        g_ucIsDownScaling[u8WinIdx] = FALSE;
    //initial PQ structure
    MS_PQ_Init_Info sXC_PQ_InitData;

    sXC_PQ_InitData.bDDR2 = FALSE;
    sXC_PQ_InitData.u32DDRFreq = 1866;
    sXC_PQ_InitData.u8BusWidth = 16;
    //sXC_PQ_InitData.u32miu0em_size = mminfo.miu0_mem_size;
    //sXC_PQ_InitData.u32miu1em_size = mminfo.miu1_mem_size;

    if(PanelWidth <= 2560)
        sXC_PQ_InitData.u16PnlWidth = 2560;
    else
        sXC_PQ_InitData.u16PnlWidth = 3840;

    sXC_PQ_InitData.u16PnlHeight = PanelHeight;
    MSPQ_PRINT("[PQ_Init] Panel width = %d; Panel Height = %d\n",sXC_PQ_InitData.u16PnlWidth, sXC_PQ_InitData.u16PnlHeight);
    //FIXME:
    // sXC_PQ_InitData.u16OSD_hsize = UI_OSD_PANE_W;
    sXC_PQ_InitData.u16OSD_hsize = PanelWidth;

    sXC_PQ_InitData.u16Pnl_vtotal = PanelVTotal;


    sXC_PQ_InitData.u8PQBinCnt = 0;
    sXC_PQ_InitData.u8PQTextBinCnt = 0;
    sXC_PQ_InitData.u8PQBinCustomerCnt = 0;

#if(ENABLE_PQ_BIN)
    MS_PQ_CAP_INFO stPQCaps;
    //BYTE *pu8MainBuf, *pu8MainExBuf;
    BYTE pu8MainBuf[0x30] = {0};
    BYTE pu8MainExBuf[0x30] = {0};
    BYTE pu8SubBuf[0x30] = {0};
    BYTE pu8SubExBuf[0x30] = {0};
    DWORD    dwFileLen;
    stPQCaps.bPIP_Supported = TRUE;
    Flash_ReadTbl(PQ_BIN_MAIN_ADDRESS, &pu8MainBuf[0], 0x28); //read header first
    if (pu8MainBuf[0] == 0x4D && pu8MainBuf[1] == 0x53 && pu8MainBuf[2] == 0x54)    //check if bin file exist in flash
    {
        MSPQ_PRINT ("PQ main Bin file is Exist...\n");
        dwFileLen = (pu8MainBuf[0x27] << 24) + (pu8MainBuf[0x26] << 16) + (pu8MainBuf[0x25] << 8) + pu8MainBuf[0x24];
        MSPQ_PRINT ("PQ main Bin file length = %d\n", dwFileLen);


        //main_ex ....
        Flash_ReadTbl(PQ_BIN_MAIN_EX_ADDRESS, &pu8MainExBuf[0], 0x28); //read header first
        dwFileLen = (pu8MainExBuf[0x27] << 24) + (pu8MainExBuf[0x26] << 16) + (pu8MainExBuf[0x25] << 8) + pu8MainExBuf[0x24];
        MSPQ_PRINT ("PQ main_Ex Bin file length = %d\n", dwFileLen);



        if(pu8MainBuf && pu8MainExBuf)
        {
            sXC_PQ_InitData.u8PQBinCnt = 2;
            sXC_PQ_InitData.stPQBinInfo[0].u8PQID = (BYTE) PQ_BIN_STD_MAIN;
            sXC_PQ_InitData.stPQBinInfo[0].PQBin_PhyAddr = 0x100000;//(MS_PHYADDR) pu8MainBuf;
            sXC_PQ_InitData.stPQBinInfo[1].u8PQID = (BYTE) PQ_BIN_EXT_MAIN;
            sXC_PQ_InitData.stPQBinInfo[1].PQBin_PhyAddr = 0x170000;//(MS_PHYADDR)pu8MainExBuf;
        }

        // sub PQ... start
        Flash_ReadTbl(PQ_BIN_SUB_ADDRESS, &pu8SubBuf[0], 0x28); //read header first
        dwFileLen = (pu8SubBuf[0x27] << 24) + (pu8SubBuf[0x26] << 16) + (pu8SubBuf[0x25] << 8) + pu8SubBuf[0x24];
        MSPQ_PRINT ("PQ Sub Bin file length = %d\n", dwFileLen);


        //sub_ex ....
        Flash_ReadTbl(PQ_BIN_SUB_EX_ADDRESS, &pu8SubExBuf[0], 0x28); //read header first
        dwFileLen = (pu8SubExBuf[0x27] << 24) + (pu8SubExBuf[0x26] << 16) + (pu8SubExBuf[0x25] << 8) + pu8SubExBuf[0x24];
        MSPQ_PRINT ("PQ main_Ex Bin file length = %d\n", dwFileLen);


            if(pu8SubBuf && pu8SubExBuf)
            {
                sXC_PQ_InitData.u8PQBinCnt = 4;
            sXC_PQ_InitData.stPQBinInfo[2].u8PQID = (BYTE) PQ_BIN_STD_SUB;
            sXC_PQ_InitData.stPQBinInfo[2].PQBin_PhyAddr = 0x180000;//(MS_PHYADDR) pu8MainBuf;
            sXC_PQ_InitData.stPQBinInfo[3].u8PQID = (BYTE) PQ_BIN_EXT_SUB;
            sXC_PQ_InitData.stPQBinInfo[3].PQBin_PhyAddr = 0x1D0000;//(MS_PHYADDR)pu8MainExBuf;
            }

        sXC_PQ_InitData.u8PQTextBinCnt = 0;
        sXC_PQ_InitData.u8PQBinCustomerCnt = 0 ;

                }
    else
                {
        printf ("PQ main Bin file is not Exist...\n");
        sXC_PQ_InitData.u8PQBinCnt = 0;
        sXC_PQ_InitData.u8PQTextBinCnt = 0;
        sXC_PQ_InitData.u8PQBinCustomerCnt = 0 ;
    }
#else
    sXC_PQ_InitData.u8PQBinCnt = 0;
    sXC_PQ_InitData.u8PQTextBinCnt = 0;
    sXC_PQ_InitData.u8PQBinCustomerCnt = 0 ;
#endif


    g_bSkipCommTable = FALSE;

    do{
        if (u16ScalerMask & BIT0)
        {
            u32ScBaseAddr = SCRegBase(u8ScIdx);
            /*
            if (u8ScIdx % 2)
            {
                MDrv_PQ_Init(PQ_MAIN_WINDOW, eDispType, &sXC_PQ_InitData);
            }
            else
            {
                MDrv_PQ_Init(PQ_SUB_WINDOW, eDispType, &sXC_PQ_InitData);
            }*/
#if (PQ_ONLY_SUPPORT_BIN == 0)
            MDrv_PQ_Init(PQ_SUB_WINDOW, eDispType, &sXC_PQ_InitData);
#endif
            MDrv_PQ_Init(PQ_MAIN_WINDOW, eDispType, &sXC_PQ_InitData);
        }
        u16ScalerMask >>= 1;
        u8ScIdx++;
    } while (u16ScalerMask > 0);


}


void msPQ_LoadTimingSetting(BYTE ucWinIndex, ST_WINDOW_INFO sAutoWindow, BOOL bDumpPQ)
{
    if (bDumpPQ)
        g_bPQ_Busy_On = TRUE;
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut | g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    u16ScalerMask &= 0xFF;
    BYTE u8PortIdx = MapWin2Port(ucWinIndex);

    WORD u16ScMaskTemp = u16ScalerMask;

    BYTE u8ScIdx = 0;
    MS_PQ_Mode_Info pModeInfo;
    MS_PQ_Hdmi_Info     stHdmi_Info;
    EN_WINDOW_COLOR  eWinColor = eWIN_COLOR_RGB;
    EN_INPUT_COLOR   eInputColor = eINPUT_COLOR_RGB;
    EN_COLOR_RANGE   eInputColorRange = eCOLOR_RANGE_LIMIT;


    MSPQ_PRINT("[PQ LoadTiming] ScalerMask = %4x\n", u16ScalerMask);
    u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window

    //memset(&pModeInfo, 0, sizeof(MS_PQ_Mode_Info));

    PQ_INPUT_SOURCE_TYPE enInputSourceType = PQ_INPUT_SOURCE_HDMI;
#if ENABLE_VGA_INPUT
    if (INPUT_IS_ANALOG1(u8PortIdx))
        enInputSourceType = PQ_INPUT_SOURCE_VGA;
#endif
#if ENABLE_DVI_INPUT
    if (INPUT_IS_DVI(u8PortIdx))
        enInputSourceType = PQ_INPUT_SOURCE_DVI;
#endif
#if ENABLE_HDMI_INPUT
    if (INPUT_IS_HDMI(u8PortIdx))
        enInputSourceType = PQ_INPUT_SOURCE_HDMI;
#endif

    msAPIWinGetColorSpaceWindow(ucWinIndex, &eWinColor);
    msAPIWinGetColorSpaceInput(ucWinIndex, &eInputColor, &eInputColorRange);
    pModeInfo.u16display_hsize = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
    pModeInfo.u16display_vsize = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
    if (SrcFlags(u8PortIdx)&bInterlaceMode)
        pModeInfo.bInterlace = TRUE;
    else
        pModeInfo.bInterlace = FALSE;

    //stHdmi_Info.enColorFmt = (PQ_HDMI_COLOR_TYPE)msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[u8PortIdx].eIPMux)).ucColorType;//PQ_HDMI_COLOR_RGB;
    stHdmi_Info.enColorFmt = PQ_HDMI_COLOR_RGB;
    if (eInputColor == eINPUT_COLOR_RGB)
        stHdmi_Info.enColorFmt = PQ_HDMI_COLOR_RGB;
    else if (eInputColor == eINPUT_COLOR_422 ||  eInputColor == eINPUT_COLOR_420 || eInputColor == eINPUT_COLOR_YPBPR)
        stHdmi_Info.enColorFmt = PQ_HDMI_COLOR_YUV_422;
    else if (eInputColor == eINPUT_COLOR_444)
        stHdmi_Info.enColorFmt = PQ_HDMI_COLOR_YUV_444;


    if (g_IsVideoMode[ucWinIndex])
    {
        stHdmi_Info.bIsHDMI = TRUE;
        stHdmi_Info.bIsHDMIPC = FALSE;
    }
    else
    {
        //pModeInfo.bInterlace = FALSE;
        stHdmi_Info.bIsHDMI = FALSE;
        stHdmi_Info.bIsHDMIPC = TRUE;
    }

    MSPQ_PRINT("[PQ LoadTiming] stHdmi_Info.enColorFmt = %d\n", stHdmi_Info.enColorFmt);

#if 0
    if ( (stHdmi_Info.enColorFmt == PQ_HDMI_COLOR_RGB) &&(IsWindowDI(ucWinIndex) ||  YUV_COLOR_DOMAIN_FLAG) )
    {
        stHdmi_Info.enColorFmt = PQ_HDMI_COLOR_YUV_422;
    }
#else
    if (bDumpPQ)
    {
        if ((stHdmi_Info.enColorFmt == PQ_HDMI_COLOR_RGB) && (eWinColor != eWIN_COLOR_RGB))
            stHdmi_Info.enColorFmt = PQ_HDMI_COLOR_YUV_444;
        else if (stHdmi_Info.enColorFmt > PQ_HDMI_COLOR_RGB && (eWinColor == eWIN_COLOR_RGB))
            stHdmi_Info.enColorFmt = PQ_HDMI_COLOR_RGB;
    }
    if (g_ucForceYUV[ucWinIndex])
        stHdmi_Info.enColorFmt = PQ_HDMI_COLOR_YUV_422;

#endif

#if 1
    pModeInfo.u16input_hsize = sAutoWindow.width > PANEL_WIDTH ? PANEL_WIDTH : sAutoWindow.width;
    pModeInfo.u16input_vsize = sAutoWindow.height > PANEL_HEIGHT ? PANEL_HEIGHT : sAutoWindow.height;
#else
    pModeInfo.u16input_hsize = sAutoWindow.width;//g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
    pModeInfo.u16input_vsize = sAutoWindow.height;//g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
#endif	
    pModeInfo.bFBL = !g_ScalerInfo[u8ScIdx].u8SiFbMode;
    pModeInfo.u16input_vtotal = g_InputPort[u8PortIdx].sIPInputTiming.VTotal;
    pModeInfo.u16input_vfreq = V_FREQ_IN(u8PortIdx) * 10;
    pModeInfo.u16ouput_vfreq = GetVfreq() * 10;

    //check if need to use common write...

    u8ScIdx = 0;


    bool isWinRot = IsWindowRot90270(ucWinIndex);
    //msAPIWinGetReadCurFrameMode(ucWinIndex, &g_bReadCurFrameMode);
  #if  (!FREESYNC_SW_PATCH_FOR_WITHLINEBUFEER)
    msAPIWinGetReadCurFrameMode(ucWinIndex, &g_bReadCurFrameMode);
  #else
    g_bReadCurFrameMode = msDrvGetFrameSyncBankDiffZero();
  #endif
    do{
        if (u16ScMaskTemp & BIT0)
        {
            u32ScBaseAddr = SCRegBase(u8ScIdx);
            if (u8ScIdx % 2)
            {
                //u16ScMainMask |= (1<< u8ScIdx);
                //ucMainScCnt++;
                MDrv_PQ_SetHDMIInfo(PQ_MAIN_WINDOW, &stHdmi_Info);
                MDrv_PQ_Set_ModeInfo(PQ_MAIN_WINDOW, enInputSourceType, &pModeInfo);
                MDrv_PQ_DesideSrcType(PQ_MAIN_WINDOW, enInputSourceType);
                if (bDumpPQ)
                {
                #if 1
                    MDrv_PQ_LoadSettings(PQ_MAIN_WINDOW, isWinRot);
                    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, g_ucMADiIdx[PQ_MAIN_WINDOW][ucWinIndex], PQ_IP_MADi_Main);
                    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIndex], PQ_IP_10to8_Dither_Main);
                #else   // bypass PQ table setting...
                    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, g_ucMADiIdx[PQ_MAIN_WINDOW][ucWinIndex], PQ_IP_MADi_Main);
                    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIndex], PQ_IP_10to8_Dither_Main);
                    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_C_SRAM1_Main), PQ_IP_C_SRAM1_Main);
                    MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_C_SRAM2_Main), PQ_IP_C_SRAM2_Main);
                #endif
                    MSPQ_PRINT("[PQ LoadTiming] Dump Main table finished...\n");
                }
            }
            else
            {
                //u16ScSubMask |= (1<<u8ScIdx);
                //ucSubScCnt++;
                _InitPQTable(FALSE);
                MDrv_PQ_SetHDMIInfo(PQ_SUB_WINDOW, &stHdmi_Info);
                MDrv_PQ_Set_ModeInfo(PQ_SUB_WINDOW, enInputSourceType, &pModeInfo);
                MDrv_PQ_DesideSrcType(PQ_SUB_WINDOW, enInputSourceType);
                if (bDumpPQ)
                {
                    if (u16ScalerMask & bSC_MAIN)    //both main & sub case( sub in)
                    {
                        //load sub PQ table first...
                        MSPQ_PRINT("[PQ LoadTiming]                      baseAddr = %8x\n", u32ScBaseAddr);
                        //msDrvSCCommonWrite(u16ScSubMask, TRUE);

                        MDrv_PQ_LoadSettings(PQ_SUB_WINDOW, isWinRot);
                        //20140211, due to change MADi setting into skip rule, so both case need to dump MADi setting
                        MSPQ_PRINT("[PQ_LoadTiming][Sub in] madi index = %d\n", g_ucMADiIdx[PQ_SUB_WINDOW][ucWinIndex]);
                        MDrv_PQ_LoadTable(PQ_SUB_WINDOW, g_ucMADiIdx[PQ_MAIN_WINDOW][ucWinIndex], PQ_IP_MADi_Sub);
                        MDrv_PQ_LoadTable(PQ_SUB_WINDOW, g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIndex], PQ_IP_10to8_Dither_Sub);
                        MSPQ_PRINT("[PQ LoadTiming] Dump Sub table finished...\n");
                        //Reload Main PQ table...


                    }
                    else    //only sub win case
                    {
                        //msDrvSCCommonWrite(u16ScSubMask, TRUE);
                        MDrv_PQ_LoadSettings(PQ_SUB_WINDOW, isWinRot);
                        MDrv_PQ_LoadTable(PQ_SUB_WINDOW, g_ucMADiIdx[PQ_SUB_WINDOW][ucWinIndex], PQ_IP_MADi_Sub);
                        MDrv_PQ_LoadTable(PQ_SUB_WINDOW, g_uc10to8Dither[PQ_SUB_WINDOW][ucWinIndex], PQ_IP_10to8_Dither_Sub);
                    }

                }
                _InitPQTable(TRUE);
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);

    if (bDumpPQ)
        g_bPQ_Busy_On = FALSE;

}

void msPQ_GetMemInfo(BYTE ucWinIdx, BYTE *u8MemFmt, BYTE *u8FieldNum, WORD *u16FiledMode)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIdx].u16DwScMaskOut | g_DisplayWindow[ucWinIdx].u16DwScMaskIn;

    WORD u16ScMaskTemp = u16ScalerMask;
    BYTE ucMainScCnt, ucSubScCnt;
    WORD u16ScMainMask, u16ScSubMask;
    BYTE u8ScIdx = 0;

    ucMainScCnt = 0; ucSubScCnt = 0;
    u16ScMainMask = u16ScSubMask = 0;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            if (u8ScIdx % 2)
            {
                u16ScMainMask |= (1<< u8ScIdx);
                ucMainScCnt++;
            }
            else
            {
                u16ScSubMask |= (1<<u8ScIdx);
                ucSubScCnt++;
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);

    MS_BOOL bMem422 = true;
    MS_U16 u16MADiIdx, u16MemFormat;
    MS_U8  u8BPP;
    *u8FieldNum = 4;    //default value;
    *u16FiledMode = 0;
    u16MemFormat = 0;
    u16MADiIdx = 0;
    BYTE  u8DiTable[7];
    PQ_DEINTERLACE_MODE eDeInterlaceMode;
    bool isWinRot = IsWindowRot90270(ucWinIdx);

    if (ucMainScCnt > 0)
    {
        u16MemFormat = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_MemFormat_Main);

        if (isWinRot)
        {
            u16MADiIdx = PQ_IP_MADi_P_MODE_ROTATE_Main;
            u16MemFormat = 1;   //444 10 bits
        }
        else if (IsWindowRot(ucWinIdx))
        {
            if (InterlaceModeFlag(MapWin2Port(ucWinIdx)))
                u16MADiIdx = PQ_IP_MADi_P_MODE8_Main;
            else
                u16MADiIdx = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_MADi_Main);
        }
        else
            u16MADiIdx = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_MADi_Main);

        if (u16MemFormat == 0)
            bMem422 = true;
        else
            bMem422 = false;

        g_ucMADiIdx[PQ_MAIN_WINDOW][ucWinIdx] = u16MADiIdx;
#if (PANEL_EDP)		
   #if eDPTX10Bits
        g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIdx] = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_10to8_Dither_Main);
   #else
        g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIdx] = PQ_IP_10to8_Dither_Rand_Dith_Main;
   #endif
#elif (PANEL_VBY1())
   #if (PANEL_VBY1_1CH_10Bit||PANEL_VBY1_2CH_10Bit||PANEL_VBY1_4CH_10Bit||PANEL_VBY1_8CH_10Bit)
        g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIdx] = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_10to8_Dither_Main);
   #else
        g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIdx] = PQ_IP_10to8_Dither_Rand_Dith_Main;
   #endif
#else
        g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIdx] = MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, PQ_IP_10to8_Dither_Main);
#endif

        MDrv_PQ_LoadTableData(PQ_MAIN_WINDOW, u16MADiIdx, PQ_IP_MADi_Main, &u8DiTable[0],7);
        MSPQ_PRINT("[MSPQ] is Mem422 = %d, MemFormat = %d, MADi Idx = %d\n", bMem422, u16MemFormat, u16MADiIdx);
        MDrv_PQ_GetBPPInfoFromMADi(PQ_MAIN_WINDOW, bMem422, u16MADiIdx, u16MemFormat, &u8BPP, &eDeInterlaceMode, u8FieldNum);
        *u16FiledMode = u8DiTable[5] | (u8DiTable[6] << 8);
    }

    if (ucSubScCnt > 0) //there will be two condition: only sub scaler and both sub and main
    {
        u16MemFormat = MDrv_PQ_GetTableIndex(PQ_SUB_WINDOW, PQ_IP_MemFormat_Sub);

        if (isWinRot)
        {
            u16MADiIdx = PQ_IP_MADi_P_MODE_ROTATE_Sub;
            u16MemFormat = 1;   //444 10bits
        }
        else if (IsWindowRot(ucWinIdx))
        {
            if (InterlaceModeFlag(MapWin2Port(ucWinIdx)))
                u16MADiIdx = PQ_IP_MADi_P_MODE8_Sub;
            else
                u16MADiIdx = MDrv_PQ_GetTableIndex(PQ_SUB_WINDOW, PQ_IP_MADi_Sub);
        }
        else
            u16MADiIdx = MDrv_PQ_GetTableIndex(PQ_SUB_WINDOW, PQ_IP_MADi_Sub);

        if (u16MemFormat == 0)
            bMem422 = true;
        else
            bMem422 = false;

        if (ucMainScCnt > 0) //sub and main need to use the same madi setting
        {
            g_ucMADiIdx[PQ_SUB_WINDOW][ucWinIdx] = g_ucMADiIdx[PQ_MAIN_WINDOW][ucWinIdx];   //sub madi setting is same as main scaler
            g_uc10to8Dither[PQ_SUB_WINDOW][ucWinIdx] = g_uc10to8Dither[PQ_MAIN_WINDOW][ucWinIdx];
        }
        else        //only sub scaler case
        {
            g_ucMADiIdx[PQ_SUB_WINDOW][ucWinIdx] = u16MADiIdx;
            g_uc10to8Dither[PQ_SUB_WINDOW][ucWinIdx] = MDrv_PQ_GetTableIndex(PQ_SUB_WINDOW, PQ_IP_10to8_Dither_Sub);
            MDrv_PQ_LoadTableData(PQ_SUB_WINDOW, u16MADiIdx, PQ_IP_MADi_Sub, &u8DiTable[0],7);
            MSPQ_PRINT("[MSPQ] (Sub) is Mem422 = %d, MemFormat = %d, MADi Idx = %d\n", bMem422, u16MemFormat, u16MADiIdx);
            MDrv_PQ_GetBPPInfoFromMADi(PQ_SUB_WINDOW, bMem422, u16MADiIdx, u16MemFormat, &u8BPP, &eDeInterlaceMode, u8FieldNum);
            *u16FiledMode = u8DiTable[4] | (u8DiTable[5] << 8);
        }

    }

    //*u16FiledMode = u8DiTable[5] | (u8DiTable[6] << 8);



    if (u16MemFormat == 0)
        *u8MemFmt = MEM_FMT_422;
    else if (u16MemFormat == 1)
#if (PANEL_EDP)		
   #if eDPTX10Bits
        *u8MemFmt = MEM_FMT_444_10;
   #else
        *u8MemFmt = MEM_FMT_444_8;
   #endif
#elif (PANEL_VBY1())
   #if (PANEL_VBY1_1CH_10Bit||PANEL_VBY1_2CH_10Bit||PANEL_VBY1_4CH_10Bit||PANEL_VBY1_8CH_10Bit)
        *u8MemFmt = MEM_FMT_444_10;
   #else
        *u8MemFmt = MEM_FMT_444_8;
   #endif
#else
        *u8MemFmt = MEM_FMT_444_10;
#endif
    else if (u16MemFormat == 2)
        *u8MemFmt = MEM_FMT_444_8;
    else
        *u8MemFmt = MEM_FMT_NO_USE;

#if 0 //ENABLE_DEINTERLACE
    if (GetDeinterlaceValue())
    {
        *u8FiledMode = 0;
        *u8FieldNum = 4;
    }
#endif

}

void msPQ_ForceYUV422(BYTE ucWinIdx, BOOL bForceYUV422)
{
    g_ucForceYUV[ucWinIdx] = bForceYUV422;
}


void msPQ_LoadXRuleTable(BYTE ucWinIndex)
{
    g_bPQ_Busy_On = TRUE;
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut | g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    BYTE u8PortIdx = MapWin2Port(ucWinIndex);

    BYTE bPreVSD;
    BYTE bPreHSD;
    BYTE bInterlace;
    BYTE bYUV;
    MS_U16 u16InpSize;
    MS_U16 u16OutSize;
    BYTE u8ScIdx = 0;
    //MS_BOOL  bRgbFullRange = FALSE;
    bPreHSD = SCALING_H_DOWN(u8ScIdx);
    bPreVSD = SCALING_V_DOWN(u8ScIdx);



    if(msAPIWinColorIsYUV(ucWinIndex))
        bYUV = TRUE;
    else
        bYUV = FALSE;

    if (SrcFlags(u8PortIdx)&bInterlaceMode)
        bInterlace = TRUE;
    else
        bInterlace = FALSE;

    do{
        if (u16ScalerMask & BIT0)
        {
            u32ScBaseAddr = SCRegBase(u8ScIdx);
            if (u8ScIdx % 2)
            {
                u16InpSize = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
                u16OutSize = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
                MDrv_PQ_LoadScalingTable(PQ_MAIN_WINDOW, E_XRULE_HSD, bPreVSD, bInterlace, bYUV, u16InpSize, u16OutSize);
                MDrv_PQ_LoadScalingTable(PQ_MAIN_WINDOW, E_XRULE_HSP, bPreVSD, bInterlace, bYUV, u16InpSize, u16OutSize);
                u16InpSize = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
                u16OutSize = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
                MDrv_PQ_LoadScalingTable(PQ_MAIN_WINDOW, E_XRULE_VSD, bPreVSD, bInterlace, bYUV, u16InpSize, u16OutSize);
                MDrv_PQ_LoadScalingTable(PQ_MAIN_WINDOW, E_XRULE_VSP, bPreVSD, bInterlace, bYUV, u16InpSize, u16OutSize);
            }
            else
            {
                _InitPQTable(FALSE);
                u16InpSize = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
                u16OutSize = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
                MDrv_PQ_LoadScalingTable(PQ_SUB_WINDOW, E_XRULE_HSD, bPreVSD, bInterlace, bYUV, u16InpSize, u16OutSize);
                MDrv_PQ_LoadScalingTable(PQ_SUB_WINDOW, E_XRULE_HSP, bPreVSD, bInterlace, bYUV, u16InpSize, u16OutSize);
                u16InpSize = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
                u16OutSize = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
                MDrv_PQ_LoadScalingTable(PQ_SUB_WINDOW, E_XRULE_VSD, bPreVSD, bInterlace, bYUV, u16InpSize, u16OutSize);
                MDrv_PQ_LoadScalingTable(PQ_SUB_WINDOW, E_XRULE_VSP, bPreVSD, bInterlace, bYUV, u16InpSize, u16OutSize);
                _InitPQTable(TRUE);
            }
        }
        u16ScalerMask >>= 1;
        u8ScIdx++;
    } while (u16ScalerMask > 0);


    if ( (g_ucMultiWinMode == PQ_DISPLAY_PIP) && (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width > g_DisplayWindow[ucWinIndex].sDwDisplayWin.width) && (g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.height))
        g_ucIsDownScaling[ucWinIndex] = TRUE;
    else
        g_ucIsDownScaling[ucWinIndex] = FALSE;

    g_bPQ_Busy_On = FALSE;

}



void msPQ_SetGRule_NR(BYTE ucWinIndex, BYTE level)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;

    BYTE u8ScIdx = 0;
    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;

            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window

            MDrv_PQ_LoadNRTable(eWin, (PQ_3D_NR_FUNCTION_TYPE)level);
        }
        u8ScIdx++;
        u16ScalerMask>>=1;
    }while (u16ScalerMask>0);

}

void msPQ_SetGRule_OSDBW(BYTE ucWinIndex, BYTE bOn)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;
    PQ_OSD_BW_FUNCTION_TYPE type;
    BYTE u8ScIdx = 0;
    if (bOn)
        type = PQ_OSD_BW_ON;
    else
        type = PQ_OSD_BW_NUM;

    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;

            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window


            MDrv_PQ_LoadOSD_BWTable(eWin, type);
        }
        u8ScIdx++;
        u16ScalerMask>>=1;
    }   while (u16ScalerMask>0);

}

///level: 0 = OFF, 1 = Low, 2 = MID, 3 = High
void msPQ_SetGRule_MpegNR(BYTE ucWinIndex, BYTE level)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;
    PQ_MPEG_NR_FUNCTION_TYPE type;
    type = (PQ_MPEG_NR_FUNCTION_TYPE)level;
    BYTE u8ScIdx = 0;
    if (level > PQ_MPEG_NR_HIGH)
    {
        MSPQ_PRINT("[PQ GRule] Input level value of MPEG NR is over range...\n");
        return;
    }
    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;

            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window

            MDrv_PQ_LoadMPEGNRTable(eWin, type);
        }
        u8ScIdx++;
        u16ScalerMask>>=1;
    }while (u16ScalerMask>0);
}

//Bypass feature, level: 0 = IP bypass, 1 = IP on
void msPQ_SetGRule_Bypass_NR(BYTE ucWinIndex, BYTE level)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;
    PQ_BYPASS_FUNCTION_TYPE type;
    type = (PQ_BYPASS_FUNCTION_TYPE)level;
    BYTE u8ScIdx = 0;
    if (level >= PQ_BYPASS_NUM)
    {
        MSPQ_PRINT("[PQ GRule] Input level value of Bypass Feature is over range...\n");
        return;
    }
    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;

            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window


            MDrv_PQ_LoadBypassNRTable(eWin, type);
        }
        u8ScIdx++;
        u16ScalerMask>>=1;
    }while (u16ScalerMask>0);
}

//Bypass feature, level: 0 = IP bypass, 1 = IP on
void msPQ_SetGRule_Bypass_DI(BYTE ucWinIndex, BYTE level)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;
    PQ_BYPASS_FUNCTION_TYPE type;
    type = (PQ_BYPASS_FUNCTION_TYPE)level;
    BYTE u8ScIdx = 0;
    if (level >= PQ_BYPASS_NUM)
    {
        MSPQ_PRINT("[PQ GRule] Input level value of Bypass Feature is over range...\n");
        return;
    }
    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;

            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window


            MDrv_PQ_LoadBypassDiTable(eWin, type);
        }
        u8ScIdx++;
        u16ScalerMask>>=1;
    }while (u16ScalerMask>0);
}

//Bypass feature, level: 0 = IP bypass, 1 = IP on
void msPQ_SetGRule_Bypass_Peaking(BYTE ucWinIndex, BYTE level)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;
    PQ_BYPASS_FUNCTION_TYPE type;
    type = (PQ_BYPASS_FUNCTION_TYPE)level;
    BYTE u8ScIdx = 0;
    if (level >= PQ_BYPASS_NUM)
    {
        MSPQ_PRINT("[PQ GRule] Input level value of Bypass Feature is over range...\n");
        return;
    }
    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;
            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window
            MDrv_PQ_LoadBypassPeakingTable(eWin, type);
        }
        u8ScIdx++;
        u16ScalerMask>>=1;
    }while (u16ScalerMask>0);
}

//Bypass feature, level: 0 = IP bypass, 1 = IP on
void msPQ_SetGRule_Bypass_Others(BYTE ucWinIndex, BYTE level)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;
    PQ_BYPASS_FUNCTION_TYPE type;
    type = (PQ_BYPASS_FUNCTION_TYPE)level;
    BYTE u8ScIdx = 0;
    if (level >= PQ_BYPASS_NUM)
    {
        MSPQ_PRINT("[PQ GRule] Input level value of Bypass Feature is over range...\n");
        return;
    }
    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;
            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window
            MDrv_PQ_LoadBypassOthersTable(eWin, type);
        }
        u16ScalerMask>>=1;
        u8ScIdx++;
    }while (u16ScalerMask>0);
}
//********end bypass feature

///level: 0 = OFF, 1 = UHD, 2 = high_plus, 3 = high, 4 = middle_plus, 5 = middle, 6= low
void msPQ_SetGRule_SR(BYTE ucWinIndex, BYTE level)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;
    PQ_SR_FUNCTION_TYPE type;
    type = (PQ_SR_FUNCTION_TYPE)level;
    BYTE u8ScIdx = 0;

    if (level >= ePQ_SR_Numbers)
    {
        MSPQ_PRINT("[PQ GRule] Input level value of SR is over range...\n");
        return;
    }

    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;

            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window
            if (eWin == PQ_SUB_WINDOW)
                _InitPQTable(FALSE);
#if !ENABLE_MWE
            if ( (type != PQ_SR_OFF) && (g_ucIsDownScaling[ucWinIndex] == TRUE))
                MDrv_PQ_LoadSRTable(eWin, PQ_SR_DOWN_SCALING);
            else
#endif
                MDrv_PQ_LoadSRTable(eWin, type);

            if (eWin == PQ_SUB_WINDOW)
                _InitPQTable(TRUE);
        }
        u8ScIdx++;
        u16ScalerMask>>=1;
    }while (u16ScalerMask>0);


}

void msPQ_SetGRule_FilmMode(BYTE ucWinIndex, BYTE bOn)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;
    BYTE u8ScIdx = 0;
    PQ_FILM_MODE_FUNCTION_TYPE type;
    if (bOn)
        type = PQ_FilmMode_ON;
    else
        type = PQ_FilmMode_OFF;

    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;
            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window
            MDrv_PQ_LoadFilmModeTable(eWin, type);
        }
        u16ScalerMask>>=1;
        u8ScIdx++;
    }while (u16ScalerMask>0);
}

void msPQ_SetGRule_DynmCont(BYTE ucWinIndex, BYTE bOn)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;

    BYTE u8ScIdx = 0;
    PQ_DYNAMIC_CONTRAST_FUNCTION_TYPE type;
    if (bOn)
        type = PQ_DynContr_ON;
    else
        type = PQ_DynContr_OFF;
    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;
            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window
            MDrv_PQ_LoadDynamicContrastTable(eWin, type);
        }
        u16ScalerMask>>=1;
        u8ScIdx++;
    }while (u16ScalerMask>0);
}

void msPQ_SetGRule_UltraClear(BYTE ucWinIndex, BYTE bOn)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    PQ_WIN eWin;

    BYTE u8ScIdx = 0;
    PQ_FEATURE_SWITCH_TYPE type;
    if (bOn)
        type = PQ_FEATURE_SWITCH_ON;
    else
        type = PQ_FEATURE_SWITCH_OFF;
    do{
        if (u16ScalerMask & _BIT0)
        {
            if (u8ScIdx % 2 == 1)
                eWin = PQ_MAIN_WINDOW;
            else
                eWin = PQ_SUB_WINDOW;
            u32ScBaseAddr = SCRegBase(u8ScIdx); //update base address of input window
            MDrv_PQ_Load_ULTRACLEAR_Table(eWin, type);
        }
        u16ScalerMask>>=1;
        u8ScIdx++;
    }while (u16ScalerMask>0);
}

void msPQ_SetMPQ(BYTE ucWinIndex)
{
    WORD u16ScMaskOrig = g_DisplayWindow[ucWinIndex].u16DwScMaskOut | g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    WORD u16ScMask;
    BYTE u8TblIndex, i;

    u16ScMask = u16ScMaskOrig & bSC_MAIN;
    if( u16ScMask )
    {
        u32ScBaseAddr = SCRegBase(GetLsbBitPos(u16ScMask));

        msDrvSCCommonWrite(u16ScMask, TRUE);

        u8TblIndex = g_DisplayWindow[ucWinIndex].sDwPQMode.u8MAMode;
        for( i = 0 ; i < PQ_ELEM_NUM_MA ; i++ )
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_TBL_MODE_MA[u8TblIndex+1][i], PQ_TBL_MODE_MA[0][i]);

        u8TblIndex = g_DisplayWindow[ucWinIndex].sDwPQMode.u8EOMode;
        for( i = 0 ; i < PQ_ELEM_NUM_EO ; i++ )
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_TBL_MODE_EO[u8TblIndex+1][i], PQ_TBL_MODE_EO[0][i]);

        u8TblIndex = g_DisplayWindow[ucWinIndex].sDwPQMode.u8MCMode;
        for( i = 0 ; i < PQ_ELEM_NUM_UC ; i++ )
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_TBL_MODE_UC[u8TblIndex+1][i], PQ_TBL_MODE_UC[0][i]);

        u8TblIndex = g_DisplayWindow[ucWinIndex].sDwPQMode.u8FilmMode;
        for( i = 0 ; i < PQ_ELEM_NUM_FILM ; i++ )
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, PQ_TBL_MODE_FILM[u8TblIndex+1][i], PQ_TBL_MODE_FILM[0][i]);

        msDrvSCCommonWrite(u16ScMask, FALSE);
    }

    u16ScMask = u16ScMaskOrig & bSC_SUB;
    if( u16ScMask )
    {
        u32ScBaseAddr = SCRegBase(GetLsbBitPos(u16ScMask));

        msDrvSCCommonWrite(u16ScMask, TRUE);

        u8TblIndex = g_DisplayWindow[ucWinIndex].sDwPQMode.u8MAMode;
        for( i = 0 ; i < PQ_ELEM_NUM_MA ; i++ )
            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_TBL_MODE_MA[u8TblIndex+1][i], PQ_TBL_MODE_MA[0][i]);

        u8TblIndex = g_DisplayWindow[ucWinIndex].sDwPQMode.u8EOMode;
        for( i = 0 ; i < PQ_ELEM_NUM_EO ; i++ )
            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_TBL_MODE_EO[u8TblIndex+1][i], PQ_TBL_MODE_EO[0][i]);

        u8TblIndex = g_DisplayWindow[ucWinIndex].sDwPQMode.u8MCMode;
        for( i = 0 ; i < PQ_ELEM_NUM_UC ; i++ )
            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_TBL_MODE_UC[u8TblIndex+1][i], PQ_TBL_MODE_UC[0][i]);

        u8TblIndex = g_DisplayWindow[ucWinIndex].sDwPQMode.u8FilmMode;
        for( i = 0 ; i < PQ_ELEM_NUM_FILM ; i++ )
            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, PQ_TBL_MODE_FILM[u8TblIndex+1][i], PQ_TBL_MODE_FILM[0][i]);

        msDrvSCCommonWrite(u16ScMask, FALSE);
    }
}

void msPQ_UserDumpTable(BYTE ucWinIndex, BYTE ucIPIndex, BYTE ucTableIndex)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut | g_DisplayWindow[ucWinIndex].u16DwScMaskIn;

    WORD u16ScMaskTemp = u16ScalerMask;
    BYTE ucMainScCnt, ucSubScCnt;
    WORD u16ScMainMask, u16ScSubMask;
    BYTE u8ScIdx = 0;


    ucMainScCnt = 0; ucSubScCnt = 0;
    u16ScMainMask = u16ScSubMask = 0;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            if (u8ScIdx % 2)
            {
                u16ScMainMask |= (1<< u8ScIdx);
                ucMainScCnt++;
            }
            else
            {
                u16ScSubMask |= (1<<u8ScIdx);
                ucSubScCnt++;
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);
    u32ScBaseAddr = SCRegBase(MapWin2ScIndex(ucWinIndex));
    if (ucWinIndex == 0xFF)
    {
        u32ScBaseAddr = 0x102F00;
        msDrvSCCommonWrite(bSC_MAIN | bSC_SUB, TRUE);
        MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, ucTableIndex, ucIPIndex);
        msDrvSCCommonWrite(bSC_MAIN | bSC_SUB, FALSE);

    }
    else
    {
        if (ucMainScCnt > 0)    //common wirte main sc
        {
            msDrvSCCommonWrite(u16ScMainMask, TRUE);
            MDrv_PQ_LoadTable(PQ_MAIN_WINDOW, ucTableIndex, ucIPIndex);
            msDrvSCCommonWrite(u16ScMainMask, FALSE);
        }
        if (ucSubScCnt > 0)
        {
            _InitPQTable(FALSE);
            msDrvSCCommonWrite(u16ScSubMask, TRUE);
            MDrv_PQ_LoadTable(PQ_SUB_WINDOW, ucTableIndex, ucIPIndex);
            msDrvSCCommonWrite(u16ScSubMask, FALSE);
            _InitPQTable(TRUE);
        }
    }
}

BYTE msPQ_GetTableIndex(BYTE ucWinIndex, BYTE ucIPIndex)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut | g_DisplayWindow[ucWinIndex].u16DwScMaskIn;

    WORD u16ScMaskTemp = u16ScalerMask;
    BYTE ucMainScCnt, ucSubScCnt;
    WORD u16ScMainMask, u16ScSubMask;
    BYTE u8ScIdx = 0;


    ucMainScCnt = 0; ucSubScCnt = 0;
    u16ScMainMask = u16ScSubMask = 0;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            if (u8ScIdx % 2)
            {
                u16ScMainMask |= (1<< u8ScIdx);
                ucMainScCnt++;
            }
            else
            {
                u16ScSubMask |= (1<<u8ScIdx);
                ucSubScCnt++;
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);

    if (ucMainScCnt > 0)    //common wirte main sc
    {
        return (BYTE)MDrv_PQ_GetTableIndex(PQ_MAIN_WINDOW, ucIPIndex);

    }
    else
    {
        return (BYTE)MDrv_PQ_GetTableIndex(PQ_SUB_WINDOW, ucIPIndex);

    }

}


char*   msPQ_GetIPName(BYTE ucIPIndex)
{

    return  MDrv_PQ_GetIPName(ucIPIndex);
}

void msPQ_UpdatePQTable(BYTE ucWinIdx)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIdx].u16DwScMaskOut | g_DisplayWindow[ucWinIdx].u16DwScMaskIn;
    WORD u16ScMaskTemp = u16ScalerMask;
    BYTE ucMainScCnt, ucSubScCnt;
    WORD u16ScMainMask, u16ScSubMask;
    BYTE u8ScIdx = 0;
    PQ_DISPLAY_TYPE eDispType;
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_MULTI_WIN_MODE== eMULTIWIN_2_PIP)
        eDispType = PQ_DISPLAY_PIP;
    else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
        eDispType = PQ_DISPLAY_ONE;
    else
        eDispType = PQ_DISPLAY_POP;
#else
        eDispType = PQ_DISPLAY_ONE;
#endif

    ucMainScCnt = 0; ucSubScCnt = 0;
    u16ScMainMask = u16ScSubMask = 0;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            if (u8ScIdx % 2)
            {
                u16ScMainMask |= (1<< u8ScIdx);
                ucMainScCnt++;
            }
            else
            {
                u16ScSubMask |= (1<<u8ScIdx);
                ucSubScCnt++;
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);

    if (ucMainScCnt > 0)    //common wirte main sc
    {
        MDrv_PQ_Set_DisplayType_Main(PanelWidth, eDispType, TRUE);
    }
    if (ucSubScCnt > 0)
    {
        MDrv_PQ_Set_DisplayType_Sub(PanelWidth, eDispType, TRUE);
    }

}

void msPQ_LoadUserTiming(BYTE ucWinIdx, BYTE ucTimingIdx)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIdx].u16DwScMaskOut | g_DisplayWindow[ucWinIdx].u16DwScMaskIn;
    WORD u16ScMaskTemp = u16ScalerMask;
    BYTE ucMainScCnt, ucSubScCnt;
    WORD u16ScMainMask, u16ScSubMask;
    BYTE u8ScIdx = 0;

    MSPQ_PRINT("[PQ_LoadUserTiming] scaler mask = %u", u16ScalerMask);

    ucMainScCnt = 0; ucSubScCnt = 0;
    u16ScMainMask = u16ScSubMask = 0;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            if (u8ScIdx % 2)
            {
                u16ScMainMask |= (1<< u8ScIdx);
                ucMainScCnt++;
            }
            else
            {
                u16ScSubMask |= (1<<u8ScIdx);
                ucSubScCnt++;
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);
    u32ScBaseAddr = SCRegBase(MapWin2ScIndex(ucWinIdx));
    bool bIsRot = IsWindowRot(ucWinIdx);
    if (ucMainScCnt > 0)    //common wirte main sc
    {
        MSPQ_PRINT("[PQ_LoadUserTiming] ucMainScCnt = %u", ucMainScCnt);
        MSPQ_PRINT("[PQ_LoadUserTiming] u16ScMainMask = %u", u16ScMainMask);
        msDrvSCCommonWrite(u16ScMainMask, TRUE);
        MDrv_PQ_LoadUserPrefTimingTable(PQ_MAIN_WINDOW, ucTimingIdx,bIsRot);
        msDrvSCCommonWrite(u16ScMainMask, FALSE);
    }


    if (ucSubScCnt > 0)
    {
        MSPQ_PRINT("[PQ_LoadUserTiming] ucSubScCnt = %u", ucSubScCnt);
        MSPQ_PRINT("[PQ_LoadUserTiming] u16ScSubMask = %u", u16ScSubMask);
        msDrvSCCommonWrite(u16ScSubMask, TRUE);
        MDrv_PQ_LoadUserPrefTimingTable(PQ_SUB_WINDOW, ucTimingIdx,bIsRot);
        msDrvSCCommonWrite(u16ScSubMask, FALSE);
    }

}
#define DRV_FILM            1
void msPQ_AdaptiveTuning(BYTE ucWin)
{
    if (!g_bPQ_Adaptive_On[ucWin] || g_bPQ_Busy_On)
        return;

    WORD u16ScalerMask = g_DisplayWindow[ucWin].u16DwScMaskOut | g_DisplayWindow[ucWin].u16DwScMaskIn;
    WORD u16ScMaskTemp = u16ScalerMask;
    BYTE ucMainScCnt, ucSubScCnt;
    WORD u16ScMainMask, u16ScSubMask;
    BYTE u8ScIdx = 0;

    Bool bFilm22, bFilm32, bFilmAny, bInFilm;
    DWORD   dwBaseAddrBackup;
    bFilm22 = TRUE;
    bFilm32 = TRUE;
    bFilmAny = TRUE;

    //msAPI_AutoSetHistogramRange(ucWin);
    dwBaseAddrBackup = u32ScBaseAddr;
    msAPI_HistogramReportEnable(ucWin);
    msAPI_HistogramUpDateData(ucWin);


#if DRV_FILM
    do
    {
        if (u16ScMaskTemp & BIT0)
        {
            u32ScBaseAddr = SCRegBase(u8ScIdx);
            if (u8ScIdx % 2)
            {
                bFilm22 &= (msReadByte(u32ScBaseAddr + SC0A_42) & (BIT3|BIT6)) > 0 ? TRUE: FALSE;
                bFilm32 &= (msReadByte(u32ScBaseAddr + SC0A_42) & (BIT4|BIT6)) > 0 ? TRUE: FALSE;
                bFilmAny &= (msReadByte(u32ScBaseAddr + SC0A_42) & BIT6)== BIT6 ? TRUE: FALSE;

            }
            else
            {
                bFilm22 &= (msReadByte(u32ScBaseAddr + SC0A_43) & (BIT3|BIT6))>0 ? TRUE: FALSE;
                bFilm32 &= (msReadByte(u32ScBaseAddr + SC0A_43) & (BIT4|BIT6))>0 ? TRUE: FALSE;
                bFilmAny &= (msReadByte(u32ScBaseAddr + SC0A_43) & BIT6)== BIT6 ? TRUE: FALSE;
            }

        }
        u8ScIdx++;
        u16ScMaskTemp >>= 1;

    }while (u16ScMaskTemp > 0);

    u16ScMaskTemp = u16ScalerMask;  //restore sc mask
    u8ScIdx = 0;
    if (bFilm22 || bFilm32 || bFilmAny)
    {
        bInFilm = TRUE;
        //printf("[Film debug] InFilm\n");
        //maxL0_1 = maxR0_1 = maxL0_3 = maxR0_3 = 0; minL0_1 = minR0_1 = minL0_3 = minR0_3 = 0xFF;
    }
    else
    {
        bInFilm = FALSE;

    }
#endif

    ucMainScCnt = 0; ucSubScCnt = 0;
    u16ScMainMask = u16ScSubMask = 0;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            u32ScBaseAddr = SCRegBase(u8ScIdx);
            if (u8ScIdx % 2)
            {

                MDrv_PQ_AdaptiveTuning(ucWin);
#if DRV_FILM
            if (bInFilm)
            {
                msWriteByteMask(u32ScBaseAddr + SC0A_42, 0, BIT1 | BIT2);
            }
            else
            {
                msWriteByteMask(u32ScBaseAddr + SC0A_42, BIT1|BIT2, BIT1|BIT2);
            }
#endif
            }
            else
            {
#if DRV_FILM
                if (bInFilm)
                {
                    msWriteByteMask(u32ScBaseAddr + SC0A_43, 0, BIT1 | BIT2);
                }
                else
                {
                    msWriteByteMask(u32ScBaseAddr + SC0A_43, BIT1|BIT2, BIT1|BIT2);
                }
#endif
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);
    u32ScBaseAddr = dwBaseAddrBackup;

}



void msPQ_BypassPQ(BYTE ucWinIdx, BYTE bOn)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIdx].u16DwScMaskOut | g_DisplayWindow[ucWinIdx].u16DwScMaskIn;

    WORD u16ScMaskTemp = u16ScalerMask;
    BYTE ucMainScCnt, ucSubScCnt;
    WORD u16ScMainMask, u16ScSubMask;
    BYTE u8ScIdx = 0;

    ucMainScCnt = 0; ucSubScCnt = 0;
    u16ScMainMask = u16ScSubMask = 0;
    do{
        if (u16ScMaskTemp & BIT0)
        {
            if (u8ScIdx % 2)
            {
                u16ScMainMask |= (1<< u8ScIdx);
                ucMainScCnt++;
            }
            else
            {
                u16ScSubMask |= (1<<u8ScIdx);
                ucSubScCnt++;
            }
        }
        u16ScMaskTemp >>= 1;
        u8ScIdx++;
    } while (u16ScMaskTemp > 0);

    u32ScBaseAddr = SCRegBase(MapWin2ScIndex(ucWinIdx));
    if (ucMainScCnt > 0)
    {
        msDrvSCCommonWrite(u16ScMainMask, TRUE);
        MDrv_PQ_ByPassMode(PQ_MAIN_WINDOW, bOn);
        msDrvSCCommonWrite(u16ScMainMask, FALSE);
    }
    else
    {
        msDrvSCCommonWrite(u16ScSubMask, TRUE);
        MDrv_PQ_ByPassMode(PQ_SUB_WINDOW, bOn);
        msDrvSCCommonWrite(u16ScSubMask, FALSE);
    }


}

void msPQ_IsVideoMode(BYTE ucWinIdx, BOOL bVideo)
{
    g_IsVideoMode[ucWinIdx] = bVideo;

}

void msPQ_InitSuperResolution(void)
{
    WORD u16ScalerMask = bSC_MAIN;
    BYTE u8ScIdx = 0;
    DWORD   baseAddr;
    do
    {
        if (u16ScalerMask & _BIT0)
        {
            baseAddr = SCRegBase(u8ScIdx);
            msWriteByteMask(baseAddr + SC19_20, BIT7, BIT7);
        }
        u8ScIdx++;
        u16ScalerMask >>= 1;
    }
    while (u16ScalerMask > 0);

}

void msPQ_CTI_Enable(BYTE ucWinIdx, BOOL bEn)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIdx].u16DwScMaskOut;

    if (u16ScalerMask & bSC_MAIN)
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_PreCTI_Main, bEn? PQ_IP_PreCTI_CTI_MS3_CLF_high_Main : PQ_IP_PreCTI_OFF_Main);

    if (u16ScalerMask & bSC_SUB)
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_PreCTI_Sub, bEn? PQ_IP_PreCTI_CTI_7_Sub: PQ_IP_PreCTI_OFF_Sub);

}

void msPQ_LTI_Enable(BYTE ucWinIdx, BOOL bEn)
{
    WORD u16ScalerMask = g_DisplayWindow[ucWinIdx].u16DwScMaskOut;

    if (u16ScalerMask & bSC_MAIN)
    {
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_Main, bEn? PQ_IP_VIP_LTI_ON_Main: PQ_IP_VIP_LTI_OFF_Main);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_Loc_Main, PQ_IP_VIP_LTI_Loc_cascade_Main);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_band_Main, PQ_IP_VIP_LTI_band_RF_NTSC_Main);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_dering_Main, PQ_IP_VIP_LTI_dering_W3_R0_Main);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_Pcoring_Main, PQ_IP_VIP_LTI_Pcoring_S00_Main);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_gain_Main, PQ_IP_VIP_LTI_gain_0x04_Main);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_alpha_MIN_Main, PQ_IP_VIP_LTI_alpha_MIN_m1_Main);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_alpha_fix_Main, PQ_IP_VIP_LTI_alpha_fix_OFF_Main);

    }

    if (u16ScalerMask & bSC_SUB)
    {
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_Sub, bEn? PQ_IP_VIP_LTI_ON_Sub: PQ_IP_VIP_LTI_OFF_Sub);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_Loc_Sub, PQ_IP_VIP_LTI_Loc_OFF_Sub);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_band_Sub, PQ_IP_VIP_LTI_band_RF_NTSC_Sub);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_dering_Sub, PQ_IP_VIP_LTI_dering_W3_R0_Sub);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_Pcoring_Sub, PQ_IP_VIP_LTI_Pcoring_S00_Sub);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_gain_Sub, PQ_IP_VIP_LTI_gain_0x0F_Sub);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_alpha_MIN_Sub, PQ_IP_VIP_LTI_alpha_MIN_OFF_Sub);
        msPQ_UserDumpTable(ucWinIdx, PQ_IP_VIP_LTI_alpha_fix_Sub, PQ_IP_VIP_LTI_alpha_fix_OFF_Sub);

    }

}

