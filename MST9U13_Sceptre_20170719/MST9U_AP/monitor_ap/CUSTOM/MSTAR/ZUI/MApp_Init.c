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

#define MAPP_INIT_C

//------------------------------------------------------------------------------
//                            Header Files
//------------------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include "Board.h"
#include "datatype.h"
#include "debug.h"
#include "MsTypes.h"
#include "Global.h"
// Common Definition
#include "sysinfo.h"
#include "drvOSDblend.h"
#include "drvSERFLASH.h"
#include "apiGOP.h"
#include "msAPI_OSD.h"
#include "msAPI_OCP.h"
#include "MApp_LoadFontInit.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_Init.h"

//------------------------------------------------------------------------------
//                               Debug
//------------------------------------------------------------------------------
#define INIT_DBINFO(x)  //x
#define PWR_DBG(y)      //y

/********************************************************************************/
/*                                Static                                        */
/********************************************************************************/
MS_BOOL _sc_is_interlace_tmp(void)
{
    return 0;
}

MS_U16 _sc_get_h_cap_start_tmp(void)
{
    return 0x50;
}

void _Sys_PQ_ReduceBW_ForOSD_tmp(MS_U8 PqWin, MS_BOOL bOSD_On)
{
    PqWin=bOSD_On=0;
}

/********************************************************************************/
/*                               Functions                                      */
/********************************************************************************/

void MApp_PreInit(void)
{
    GOP_InitInfo pGopInit;
#if (ZUI)	
    GFX_Config tGFXcfg;

    //init OCP var  20091124EL
    _bOCPFromMem = FALSE;
    //msAPI_DrvInitStep1();
    MDrv_SERFLASH_Init();
    // init Graphics Engine
    tGFXcfg.bIsCompt = TRUE;
    tGFXcfg.bIsHK = TRUE;
    MApi_GFX_Init(&tGFXcfg);
    MApi_GOP_RegisterFBFmtCB(( MS_U32(*)(MS_U16 pitch,MS_U32 addr , MS_U16 fmt ))msAPI_OSD_RESOURCE_SetFBFmt);
    MApi_GOP_RegisterXCIsInterlaceCB(_sc_is_interlace_tmp);
    MApi_GOP_RegisterXCGetCapHStartCB(_sc_get_h_cap_start_tmp);
    MApi_GOP_RegisterXCReduceBWForOSDCB(_Sys_PQ_ReduceBW_ForOSD_tmp);
#endif

    pGopInit.u16PanelWidth = PanelWidth;
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    pGopInit.u16PanelWidth = PanelWidth/2;
#endif
    pGopInit.u16PanelHeight = PanelHeight;

#if (CHIP_ID==MST9U)
#if ENABLE_OSDC_OSDD
#if (!MST9U_ASIC_1P)
#if _LIMIT_OSDSIZE_
    if (pGopInit.u16PanelWidth > LIMIT_OSDC_HSIZE)
    {
        pGopInit.u16PanelWidth = LIMIT_OSDC_HSIZE;
    }
    if (pGopInit.u16PanelHeight > LIMIT_OSDC_VSIZE)
    {
        pGopInit.u16PanelHeight = LIMIT_OSDC_VSIZE;
    }
#endif //#ifdef _LIMIT_OSDSIZE_
#endif //#if (!MST9U_ASIC_1P)
#endif //#if ENABLE_OSDC_OSDD
#endif //#if (CHIP_ID==MST9U)

    pGopInit.u16PanelHStr = PanelHStart/2;
#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    pGopInit.u16PanelGuardBand = GOP_GUARD_BAND; // increase GOP size with guard band
#else
    pGopInit.u16PanelGuardBand = 0; // 
#endif
    pGopInit.u32GOPRBAdr = GOP_GWIN_RB_ADR;//((GOP_GWIN_RB_MEMORY_TYPE & MIU1) ? (GOP_GWIN_RB_ADR | MIU_INTERVAL) : (GOP_GWIN_RB_ADR));
    pGopInit.u32GOPRBLen = GOP_GWIN_RB_LEN;
    //there is a GOP_REGDMABASE_MIU1_ADR for MIU1
    pGopInit.u32GOPRegdmaAdr = ((GOP_REGDMABASE_MEMORY_TYPE & MIU1) ? (GOP_REGDMABASE_ADR | MIU_INTERVAL) : (GOP_REGDMABASE_ADR));//GOP_REGDMABASE_ADR;//
    pGopInit.u32GOPRegdmaLen = GOP_REGDMABASE_LEN;
    pGopInit.bEnableVsyncIntFlip = FALSE;
	pGopInit.u8GOPChipID= CHIP_ID;
    MApi_GOP_Init(&pGopInit);
    MApi_GOP_EnableLBCouple(E_GOP_OSD, TRUE);
    MApi_GOP_EnableLBCouple(E_GOP_OSD_RIGHT, TRUE);
#if (ZUI)
    MApi_GFX_RegisterGetFontCB(msAPI_OSD_RESOURCE_GetFontInfoGFX);
    MApi_GFX_RegisterGetBMPCB(msAPI_OSD_RESOURCE_GetBitmapInfoGFX);

    gScreenWidth = PANEL_WIDTH;
    gScreenHeight = PANEL_HEIGHT;
#endif
#if (CHIP_ID==MST9U)
#if ENABLE_OSDC_OSDD
    #if (!MST9U_ASIC_1P)
    msDrvOSDBlend_Init(GOP_SOURCE);
    msDrvOSDBlend_Enable(TRUE);
    #endif
#if _LIMIT_OSDSIZE_
    gScreenWidth = LIMIT_OSDC_HSIZE;
    gScreenHeight = LIMIT_OSDC_VSIZE;
#endif
#endif
#endif //#if (CHIP_ID==MST9U)

#if (CHIP_ID==MST9U2 || CHIP_ID==MST9U4)
    {
        MS_U8 u8GOPIdx=0;
        for (u8GOPIdx=0; u8GOPIdx<MApi_GOP_GWIN_GetMaxGOPNum(); u8GOPIdx++)
            MApi_GOP_GWIN_SetGOPDst(u8GOPIdx, E_GOP_DST_OSDB);
        msDrvOSDBlend_FrcOsdb_Enable(TRUE);
    }
    MApi_GOP_GWIN_SetHSPipe(E_GOP_OSD_RIGHT,MApi_GOP_GetCurrentHPipe(E_GOP_OSD_RIGHT)-pGopInit.u16PanelGuardBand);
#endif

#if (ZUI)
    /*initialize the OCP address then prepare the bitmap binary*/
    msAPI_OCP_Init();
    msAPI_OCP_PrepareBitmapBinary();
    msAPI_OCP_LoadBitmap(Osdcp_bmpHandle);
    msAPI_OCP_PrepareStringBinary();
    MApp_ZUI_Init();
    MApp_LoadFontInit(0);
#endif

#if 0 // disable 1366x768 stretch to full screen
  #if ((UI_SKIN_SEL ==  UI_SKIN_1366X768X565) \
        ||(UI_SKIN_SEL ==  UI_SKIN_800X600X565) \
        ||(UI_SKIN_SEL ==  UI_SKIN_1366X768X4444) \
        ||(UI_SKIN_SEL ==  UI_SKIN_1366X768X565) \
        ||(UI_SKIN_SEL ==  UI_SKIN_1366X768X8888))
    if((PanelWidth >= GE_ALIGNED_VALUE(1366,16)) && (PanelHeight >= 768))
    {
        MApi_GOP_GWIN_SwitchGOP(E_GOP_OSD);
        MApi_GOP_GWIN_Set_STRETCHWIN(E_GOP_OSD, E_GOP_DST_OP0, 0, 0, GE_ALIGNED_VALUE(1366,16), 768);
        MApi_GOP_GWIN_Set_HSCALE(TRUE, GE_ALIGNED_VALUE(1366,16), PANEL_WIDTH);
        MApi_GOP_GWIN_Set_VSCALE(TRUE, 768, PANEL_HEIGHT);
    }
    else
    {
       //Unsupported resolution
       MS_DEBUG_MSG(printf("[Unsupported Resolution: %d x %d\n]",PanelWidth,PanelHeight));
    }
  #endif
#endif

}

#undef MAPP_INIT_C

