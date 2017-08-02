

#define _MS_SCALERAP_C_
#include <string.h>
#include "Global.h"
#include "msScalerAP.h"
#include "menufunc.h"
#include "Menu.h"
#include "UserPrefAPI.h"
#include "Detect.h"
#include "Mode.h"
#include "AutoFunc.h"
#include "apiXC_SC_MenuLoad.h"
#include "drvOSDblend.h"
#include "mapi_combo.h"
#if (ENABLE_MWE)
#include "drvMWE.h"
#endif
#if ENABLE_HDMI_DRR
#include "Mcu.h"
#endif
#define MSSCALERAP_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && MSSCALERAP_DEBUG
#define MSSCALERAP_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MSSCALERAP_PRINT(format, ...)
#endif

/////////////////////////////////////////////////////////////////////////
////////////////////////////// API add cb  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPICbAttach(BYTE CbType, void *CbFunc)
{
    CB_FuncList[CbType] = (fpCb)CbFunc;
}
/////////////////////////////////////////////////////////////////////////
////////////////////////////// API add cb  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPICbInit( void )
{
    BYTE u8Idx;

    for( u8Idx = 0 ; u8Idx < enCBMaxNum ; u8Idx++ )
    {
        msAPICbAttach(u8Idx, NULL);
    }
}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API get display window /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetDisplayWin( BYTE ucWinIndex,  ST_WINDOW_INFO* ptDisplayWin )
{
    // add window info to struct
    memcpy (ptDisplayWin, &g_DisplayWindow[ucWinIndex].sDwDisplayWinOri, sizeof(ST_WINDOW_INFO));

}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API set display window /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetDisplayWin( BYTE ucWinIndex,  ST_WINDOW_INFO* ptDisplayWin )
{
    // add window info to struct
    memcpy (&g_DisplayWindow[ucWinIndex].sDwDisplayWinOri, ptDisplayWin, sizeof(ST_WINDOW_INFO));

}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API get capture window /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetCaptureWin( BYTE ucWinIndex,  ST_WINDOW_INFO* ptCaptureWin )
{
    // add window info to struct
    memcpy (ptCaptureWin, &g_DisplayWindow[ucWinIndex].sDwCaptureWin, sizeof(ST_WINDOW_INFO));

}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API set capture window /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetCaptureWin( BYTE ucWinIndex,  ST_WINDOW_INFO* ptCaptureWin )
{
    // add window info to struct
    memcpy (&g_DisplayWindow[ucWinIndex].sDwCaptureWin, ptCaptureWin, sizeof(ST_WINDOW_INFO));

}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API set memory fake capture window//////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetMemFakeCapWin(BYTE ucWinIndex, ST_FAKE_CAP_WIN_INFO sUserFakeCapWin, BOOL bUserFakeCapEable)
{
    signed int swCapWinHend = g_DisplayWindow[ucWinIndex].sDwCaptureWin.x + g_DisplayWindow[ucWinIndex].sDwCaptureWin.width -1;
    signed int swCapWinVend = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y + g_DisplayWindow[ucWinIndex].sDwCaptureWin.height -1;
    signed int swUserFakeCapHend = sUserFakeCapWin.x + sUserFakeCapWin.width -1;
    signed int swUserFakeCapVend = sUserFakeCapWin.y + sUserFakeCapWin.height -1;

    if((!bUserFakeCapEable) || (!sUserFakeCapWin.width) || (!sUserFakeCapWin.height)
        || (sUserFakeCapWin.x > swCapWinHend) || (sUserFakeCapWin.y > swCapWinVend)
        ||(swUserFakeCapHend < g_DisplayWindow[ucWinIndex].sDwCaptureWin.x ) || (swUserFakeCapVend < g_DisplayWindow[ucWinIndex].sDwCaptureWin.y )) //Fake capWin is disabled
    {
        g_DisplayWindow[ucWinIndex].bDwFakeCapWinEnable = FALSE;

        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height = 0;

        if((!sUserFakeCapWin.width) || (!sUserFakeCapWin.height))
        {
            MSSCALERAP_PRINT("\n\n\n\n>> Warning!! Width or height of fake capture window is 0!!\n\n\n\n");
        }
        if((sUserFakeCapWin.x > swCapWinHend) || (sUserFakeCapWin.y > swCapWinVend))
        {
            MSSCALERAP_PRINT("\n\n\n\n>> Warning!! H start or V start of fake capture window is larger than capture window!!\n\n\n\n");
        }
        if((swUserFakeCapHend < g_DisplayWindow[ucWinIndex].sDwCaptureWin.x ) || (swUserFakeCapVend < g_DisplayWindow[ucWinIndex].sDwCaptureWin.y))
        {
            MSSCALERAP_PRINT("\n\n\n\n>> Warning!! H end or V end of fake capture window is smaller than capture window!!\n\n\n\n");
        }
        return;
    }
    else
    {
        g_DisplayWindow[ucWinIndex].bDwFakeCapWinEnable = TRUE;

        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x = sUserFakeCapWin.x;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y = sUserFakeCapWin.y;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width= sUserFakeCapWin.width;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height= sUserFakeCapWin.height;
    }

}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API get window color space/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetColorSpaceInput( BYTE ucWinIndex,  EN_INPUT_COLOR* peColor, EN_COLOR_RANGE* peRange )
{
    *peColor = g_DisplayWindow[ucWinIndex].eInputColorFormat;
    *peRange = g_DisplayWindow[ucWinIndex].eInputColorRange;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API set window color space /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetColorSpaceInput( BYTE ucWinIndex,  EN_INPUT_COLOR eInputColor, EN_COLOR_RANGE eInputRange)
{
    g_DisplayWindow[ucWinIndex].eInputColorFormat = eInputColor;
    g_DisplayWindow[ucWinIndex].eInputColorRange = eInputRange;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API get window color space/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetColorSpaceWindow( BYTE ucWinIndex,  EN_WINDOW_COLOR* peColor )
{
    *peColor = g_DisplayWindow[ucWinIndex].eWinColorFormat;
}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API set window color space /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetColorSpaceWindow( BYTE ucWinIndex,  EN_WINDOW_COLOR eWinColor)
{
    g_DisplayWindow[ucWinIndex].eWinColorFormat = eWinColor;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API get window rotate mode/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetRotateMode( BYTE ucWinIndex,  BYTE* pucRotateMode )
{
    *pucRotateMode = g_DisplayWindow[ucWinIndex].u8DWRotate;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API set window rotate mode/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetRotateMode( BYTE ucWinIndex,  BYTE u8Rotate )
{
    g_DisplayWindow[ucWinIndex].u8DWRotate = u8Rotate;
    msDrvScSetRotateMode(g_DisplayWindow[ucWinIndex].u16DwScMaskIn|g_DisplayWindow[ucWinIndex].u16DwScMaskOut,u8Rotate);
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API get window memory format /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetMemFormat( BYTE ucWinIndex,  BYTE* u8MemFormat )
{
    *u8MemFormat = g_DisplayWindow[ucWinIndex].u8DwMemFormat;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API set window memory format /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetMemFormat( BYTE ucWinIndex,  BYTE u8MemFormat )
{
    g_DisplayWindow[ucWinIndex].u8DwMemFormat = u8MemFormat;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API get window memory fieldnum /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetMemFieldNum( BYTE ucWinIndex,  BYTE* u8MemFieldNum, WORD* u16MemFieldMode )
{
    *u8MemFieldNum = g_DisplayWindow[ucWinIndex].u8DwMemFieldNum;
    *u16MemFieldMode = g_DisplayWindow[ucWinIndex].u16DwMemFieldMode;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API set window memory fieldnum /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetMemFieldNum( BYTE ucWinIndex,  BYTE u8MemFieldNum, WORD u16MemFieldMode )
{
    g_DisplayWindow[ucWinIndex].u8DwMemFieldNum = u8MemFieldNum;
    g_DisplayWindow[ucWinIndex].u16DwMemFieldMode = u16MemFieldMode;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API get window frame delay mode /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetReadCurFrameMode( BYTE ucWinIndex,  BOOL* bReadCurFrame )
{
    *bReadCurFrame = g_DisplayWindow[ucWinIndex].bDwReadCurFrame;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API set window frame delay mode /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetReadCurFrameMode( BYTE ucWinIndex,  BOOL bReadCurFrame )
{
    g_DisplayWindow[ucWinIndex].bDwReadCurFrame = bReadCurFrame;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API get window memory 12/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetMem12( BYTE ucWinIndex,  BYTE* bMem12 )
{
    *bMem12 = g_DisplayWindow[ucWinIndex].bDwMem12;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API set window memory 12/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetMem12( BYTE ucWinIndex,  BYTE bMem12 )
{
    g_DisplayWindow[ucWinIndex].bDwMem12 = bMem12;
}


Bool msAPIWinColorIsYUV(BYTE ucWinIndex)
{
    Bool bYUVMode = TRUE;

    if(g_DisplayWindow[ucWinIndex].eWinColorFormat == eWIN_COLOR_RGB)
        bYUVMode = FALSE;

    return bYUVMode;
}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API Get window eDwStatus/////////////////////////////////
/////////////////////////////////////////////////////////////////////////
BOOL msAPIWinGetDisplayStatus( BYTE ucWinIndex )
{
    if( g_DisplayWindow[ucWinIndex].eDwStatus == WIN_STATUS_OUTPUT_DISPLAYOK )
        return TRUE;

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////
//////////////// API Get window ScMaskOut, especially for Lib //////////////////
/////////////////////////////////////////////////////////////////////////
WORD msAPIWinGetScMaskOut( BYTE ucWinIndex )
{
    return g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
}

/////////////////////////////////////////////////////////////////////////
///////////////// API Get window ScMaskIn, especially for Lib //////////////////
/////////////////////////////////////////////////////////////////////////
WORD msAPIWinGetScMaskIn( BYTE ucWinIndex )
{
    return g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
}
/////////////////////////////////////////////////////////////////////////
///////////////// API Get window DwEnable, especially for Lib //////////////////
/////////////////////////////////////////////////////////////////////////
BOOL msAPIWinGetScDwEnable( BYTE ucWinIndex )
{
    return g_DisplayWindow[ucWinIndex].u8DwEnable;
}
/////////////////////////////////////////////////////////////////////////
////////////////////////// API get input port ///////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinGetInputPort( BYTE ucWinIndex,  BYTE* pu8PortIndex )
{
    *pu8PortIndex = g_DisplayWindow[ucWinIndex].u8DwPortIndex;
}

/////////////////////////////////////////////////////////////////////////
///////////////////// API switch input port, not "add"  /////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSwitchPort( BYTE ucWinIndex, BYTE ucInputPort )
{
    BYTE    ucScIndex = 0;
    BYTE    ucOriInputPort = g_DisplayWindow[ucWinIndex].u8DwPortIndex;
    WORD    uwSCMask = (g_DisplayWindow[ucWinIndex].u16DwScMaskIn | g_DisplayWindow[ucWinIndex].u16DwScMaskOut);

    if( (ucInputPort == ucOriInputPort) || (ucInputPort>=Input_Nums) || (uwSCMask==0) ) // No changed or Invalid
        return;
	
   g_NotSettingLPLL=TRUE;  //  160714_11

    #if SET_BLIT_WITH_OSDFreeSyncSwitchAction
     bOSDFreeSyncOnOffFlag=FALSE;
    #endif

    // clear ScMaskIn ONLY scaler. e.g. when port changed in HBR2 output window L/R only case.
    msDrvSrcInOnlySCUpdate(ucOriInputPort, ucWinIndex);
    uwSCMask = (g_DisplayWindow[ucWinIndex].u16DwScMaskIn | g_DisplayWindow[ucWinIndex].u16DwScMaskOut);

    g_DisplayWindow[ucWinIndex].u8DwPortIndex = ucInputPort;

    //if(g_InputPort[InputPort].eIPType &TYPE_DP)

    {
       printData("msAPI_combo_IPClrSplitFlag ==%d", ucInputPort);
	msAPI_combo_IPClrSplitFlag((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux);
    }

    //Detach First, to prevent some special application that using the same combo port but different <ucInputPort> and overwriting msAPI_combo_IPConfiguration().
    while(uwSCMask!=0)
    {
        if(uwSCMask & BIT0)
        {
            msDrvSrcDetach(ucOriInputPort, ucScIndex);
        }
        uwSCMask >>=1;
        ucScIndex ++;
    }

    ucScIndex = 0;
    uwSCMask = (g_DisplayWindow[ucWinIndex].u16DwScMaskIn | g_DisplayWindow[ucWinIndex].u16DwScMaskOut);
    // Attach
    while(uwSCMask!=0)
    {
        if(uwSCMask & BIT0)
        {
            msDrvSrcAttach(ucInputPort, ucScIndex);
        }
        uwSCMask >>=1;
        ucScIndex ++;
    }
}


/////////////////////////////////////////////////////////////////////////
///////////// API Set Window Direct Display Without Input  //////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetDirectDisplay( BYTE ucWinIndex, BYTE bDirectDisplay, ST_WINDOW_INFO* pCapWin )
{
    g_DisplayWindow[ucWinIndex].bDwDirectDisplay = bDirectDisplay;

    if( bDirectDisplay )
    {
        if( pCapWin )
        {
            g_DisplayWindow[ucWinIndex].sDwAutoWin       = (*pCapWin);
            g_DisplayWindow[ucWinIndex].sDwCaptureWin    = (*pCapWin);
        }
        else
        {
            g_DisplayWindow[ucWinIndex].sDwAutoWin       = g_DisplayWindow[ucWinIndex].sDwDisplayWin;
            g_DisplayWindow[ucWinIndex].sDwCaptureWin    = g_DisplayWindow[ucWinIndex].sDwDisplayWin;
        }

        g_DisplayWindow[ucWinIndex].u8DwPortIndex    = Input_Nums;
        g_DisplayWindow[ucWinIndex].u8DwMemFormat    = MEM_FMT_444_10;
        g_DisplayWindow[ucWinIndex].u8DwMemFieldNum  = 0x02;
        g_DisplayWindow[ucWinIndex].u16DwMemFieldMode= 0x00;
    }
}


/////////////////////////////////////////////////////////////////////////
///////////// API Set Mono Display, MEM_FMT_444_12 only  ////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinSetMonoDisplay( BYTE ucWinIndex, BYTE bDwMono )
{
    g_DisplayWindow[ucWinIndex].bDwMono = bDwMono;
}

/////////////////////////////////////////////////////////////////////////
//////// API enable window, ONLY for CONFIGURED PIP Sub Win////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinEnable( BYTE ucWinIndex, BYTE ucPortIndex, BYTE ucRotateMode )
{
    BYTE    ucSCIndex;
#if ENABLE_SUBSC_HOLLOW_OUT
    BYTE    ucSCOrder = msReadByte(SCL0_00_80);
#endif

	if(g_DisplayWindow[ucWinIndex].u8DwEnable!=ENABLE)
    {
        // add window info to struct
        g_DisplayWindow[ucWinIndex].u8DwEnable          = ENABLE;
        g_DisplayWindow[ucWinIndex].eDwStatus           = WIN_STATUS_FREERUN;
        //g_DisplayWindow[ucWinIndex].sDwDisplayWin       = stDisplayWin;
        //g_DisplayWindow[ucWinIndex].sDwDisplayWinOri    = stDisplayWin;
        g_DisplayWindow[ucWinIndex].u8DwPortIndex       = ucPortIndex;
        g_DisplayWindow[ucWinIndex].u16DwScMaskIn       = 0;
        g_DisplayWindow[ucWinIndex].u16DwScMaskOut      = 0;
        g_DisplayWindow[ucWinIndex].u8DWRotate          = ucRotateMode;
        //g_DisplayWindow[ucWinIndex].u32DwMemBase[0]     = 0;
        //g_DisplayWindow[ucWinIndex].u32DwMemSize[0]     = 0;
        //g_DisplayWindow[ucWinIndex].u32DwMemBase[1]     = 0;
        //g_DisplayWindow[ucWinIndex].u32DwMemSize[1]     = 0;
        g_DisplayWindow[ucWinIndex].bDwMem12            = FALSE;
        g_DisplayWindow[ucWinIndex].bDwForceRot2ScOP    = FALSE;
        g_DisplayWindow[ucWinIndex].bDwForceRot2ScIP    = FALSE;
        g_DisplayWindow[ucWinIndex].bDwForce1P2ScOP     = FALSE;
        g_DisplayWindow[ucWinIndex].bDwDirectDisplay    = FALSE;
        g_DisplayWindow[ucWinIndex].bDwMono               = FALSE;
        g_DisplayWindow[ucWinIndex].bDwFakeCapWinEnable   = FALSE;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x       = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y       = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width   = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height  = 0;

        msDrvScPreDispatch(ucWinIndex, 0);
        MENU_LOAD_START();
        for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
        {
            if( (g_DisplayWindow[ucWinIndex].u16DwScMaskOut) & (BIT0<<ucSCIndex) )
            {
                appmStar_SetPanelTiming(ucSCIndex);
            }
        }
#if ENABLE_SUBSC_HOLLOW_OUT
        msDrvSubSCHollowOut(ucSCOrder&BIT0, ucSCOrder&BIT1);
#endif
        MENU_LOAD_END();
    }
}

/////////////////////////////////////////////////////////////////////////
//////// API disable window, ONLY for CONFIGURED PIP Sub Win //////
/////////////////////////////////////////////////////////////////////////
void msAPIWinDisable( BYTE ucWinIndex )
{
    BYTE    ucSCIndex;
#if ENABLE_SUBSC_HOLLOW_OUT
    BYTE    ucSCOrder = msReadByte(SCL0_00_80);
#endif

	if(g_DisplayWindow[ucWinIndex].u8DwEnable!=DISABLE)
    {
        WAIT_V_OUTPUT_BLANKING_START();
        msAPIWinFreerun(ucWinIndex, TRUE);
        msDrvScStatusClr((g_DisplayWindow[ucWinIndex].u16DwScMaskIn|g_DisplayWindow[ucWinIndex].u16DwScMaskOut), TRUE);
        MENU_LOAD_START();
        for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
        {
            if( (g_DisplayWindow[ucWinIndex].u16DwScMaskIn|g_DisplayWindow[ucWinIndex].u16DwScMaskOut) & (BIT0<<ucSCIndex) )
            {
                appmStar_SetPanelTiming(ucSCIndex);
                msDrvSrcDetach(g_DisplayWindow[ucWinIndex].u8DwPortIndex, ucSCIndex);

            }
        }
#if ENABLE_SUBSC_HOLLOW_OUT
        msDrvSubSCHollowOut(ucSCOrder&BIT0, ucSCOrder&BIT1);
#endif
        MENU_LOAD_END();

        // del window info to struct
        g_DisplayWindow[ucWinIndex].u8DwEnable          = DISABLE;
        g_DisplayWindow[ucWinIndex].eDwStatus           = WIN_STATUS_IDLE;

        g_DisplayWindow[ucWinIndex].sDwAutoWin          = _InitWindow;
        g_DisplayWindow[ucWinIndex].sDwCaptureWin       = _InitWindow;
        //g_DisplayWindow[ucWinIndex].sDwDisplayWin       = _InitWindow;
        //g_DisplayWindow[ucWinIndex].sDwDisplayWinOri    = _InitWindow;
        g_DisplayWindow[ucWinIndex].u8DwPortIndex       = 0; // input port index to display
        g_DisplayWindow[ucWinIndex].u8DWRotate          = 0;
        g_DisplayWindow[ucWinIndex].u16DwScMaskOut      = 0; // OP scaler mask (after mem)
        g_DisplayWindow[ucWinIndex].u16DwScMaskIn       = 0; // IP scaler mask (before mem)
        g_DisplayWindow[ucWinIndex].bDwMem12            = FALSE;
        g_DisplayWindow[ucWinIndex].u8DwMemFormat       = MEM_FMT_NO_USE;
        g_DisplayWindow[ucWinIndex].u8DwMemFieldNum     = 0;
        g_DisplayWindow[ucWinIndex].u16DwMemFieldMode   = 0;
        g_DisplayWindow[ucWinIndex].bDwReadCurFrame     = FALSE;
        // MIU balance
        g_DisplayWindow[ucWinIndex].bDwLRCross          = FALSE;
        g_DisplayWindow[ucWinIndex].bDwLRCenter         = FALSE;
        g_DisplayWindow[ucWinIndex].bDwLRCapCenter      = FALSE; // Capture window centralized
        g_DisplayWindow[ucWinIndex].bDwMemBalance       = FALSE;
        g_DisplayWindow[ucWinIndex].bDwMemForceBase1    = FALSE;  // mem balance mode force using u32DwMemBase[1]
        g_DisplayWindow[ucWinIndex].sDwPQMode.u8MAMode  = SC_MA_MODE_OFF;
        g_DisplayWindow[ucWinIndex].sDwPQMode.u8EOMode  = SC_EO_MODE_OFF;
        g_DisplayWindow[ucWinIndex].sDwPQMode.u8MCMode  = SC_MC_MODE_OFF;
        g_DisplayWindow[ucWinIndex].sDwPQMode.u8FilmMode= SC_FILM_MODE_OFF;
        g_DisplayWindow[ucWinIndex].eWinColorFormat     = eWIN_COLOR_RGB; // window color format for data path
        g_DisplayWindow[ucWinIndex].eInputColorFormat   = eINPUT_COLOR_RGB; // input color format
        g_DisplayWindow[ucWinIndex].eInputColorRange    = eCOLOR_RANGE_LIMIT;
        // etc ...
        g_DisplayWindow[ucWinIndex].bForceYUV           = FALSE;
        g_DisplayWindow[ucWinIndex].bDwForceRot2ScOP    = FALSE; // force rotate using 2sc (both L or R) for output post h down
        g_DisplayWindow[ucWinIndex].bDwForceRot2ScIP    = FALSE; // force rotate using 2sc for input catpre if H > NUM_ROT_LB. (NO H-pre scaling)
        g_DisplayWindow[ucWinIndex].bDwForce1P2ScOP     = FALSE;
        //g_DisplayWindow[ucWinIndex].u32DwMemBase[0]     = 0; // extend for balance mode
        //g_DisplayWindow[ucWinIndex].u32DwMemSize[0]     = 0;
        //g_DisplayWindow[ucWinIndex].u32DwFbSize[0]      = 0;
        //g_DisplayWindow[ucWinIndex].u32DwMemBase[1]     = 0; // extend for balance mode
        //g_DisplayWindow[ucWinIndex].u32DwMemSize[1]     = 0;
        //g_DisplayWindow[ucWinIndex].u32DwFbSize[1]      = 0;
        g_DisplayWindow[ucWinIndex].u8DwUnmuteCntr      = 0; // counter for unmute, after disable freerun
        g_DisplayWindow[ucWinIndex].bDwDirectDisplay    = FALSE; // display memory without input
        g_DisplayWindow[ucWinIndex].bDwMono               = FALSE;
        g_DisplayWindow[ucWinIndex].bDwFakeCapWinEnable   = FALSE;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x       = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y       = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width   = 0;
        g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height  = 0;
    }
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API add window /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//tmp,  consider changed to pass in "struct" from top layer
void msAPIWinAdd( BYTE ucWinIndex, BYTE ucPortIndex, BYTE ucRotateMode, ST_WINDOW_INFO stDisplayWin )
{
    // add window info to struct
    g_DisplayWindow[ucWinIndex].u8DwEnable          = ENABLE;
    g_DisplayWindow[ucWinIndex].eDwStatus           = WIN_STATUS_FREERUN;
    g_DisplayWindow[ucWinIndex].sDwDisplayWin       = stDisplayWin;
    g_DisplayWindow[ucWinIndex].sDwDisplayWinOri    = stDisplayWin;
    g_DisplayWindow[ucWinIndex].u8DwPortIndex       = ucPortIndex;
    g_DisplayWindow[ucWinIndex].u16DwScMaskIn       = 0;
    g_DisplayWindow[ucWinIndex].u16DwScMaskOut      = 0;
    g_DisplayWindow[ucWinIndex].u8DWRotate          = ucRotateMode;
    g_DisplayWindow[ucWinIndex].u32DwMemBase[0]     = 0;
    g_DisplayWindow[ucWinIndex].u32DwMemSize[0]     = 0;
    g_DisplayWindow[ucWinIndex].u32DwMemBase[1]     = 0;
    g_DisplayWindow[ucWinIndex].u32DwMemSize[1]     = 0;
    g_DisplayWindow[ucWinIndex].bDwMem12            = FALSE;
    g_DisplayWindow[ucWinIndex].bDwForceRot2ScOP    = FALSE;
    g_DisplayWindow[ucWinIndex].bDwForceRot2ScIP    = FALSE;
    g_DisplayWindow[ucWinIndex].bDwForce1P2ScOP     = FALSE;
    g_DisplayWindow[ucWinIndex].bDwDirectDisplay    = FALSE;
    g_DisplayWindow[ucWinIndex].bDwMono               = FALSE;
    g_DisplayWindow[ucWinIndex].bDwFakeCapWinEnable   = FALSE;
    g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x       = 0;
    g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y       = 0;
    g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width   = 0;
    g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height  = 0;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API del window /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinDel( BYTE ucWinIndex )
{
    // del window info to struct
    g_DisplayWindow[ucWinIndex].u8DwEnable          = DISABLE;
    g_DisplayWindow[ucWinIndex].eDwStatus           = WIN_STATUS_IDLE;

    g_DisplayWindow[ucWinIndex].sDwAutoWin          = _InitWindow;
    g_DisplayWindow[ucWinIndex].sDwCaptureWin       = _InitWindow;
    g_DisplayWindow[ucWinIndex].sDwDisplayWin       = _InitWindow;
    g_DisplayWindow[ucWinIndex].sDwDisplayWinOri    = _InitWindow;
    g_DisplayWindow[ucWinIndex].u8DwPortIndex       = 0; // input port index to display
    g_DisplayWindow[ucWinIndex].u8DWRotate          = 0;
    g_DisplayWindow[ucWinIndex].u16DwScMaskOut      = 0; // OP scaler mask (after mem)
    g_DisplayWindow[ucWinIndex].u16DwScMaskIn       = 0; // IP scaler mask (before mem)
    g_DisplayWindow[ucWinIndex].bDwMem12            = FALSE;
    g_DisplayWindow[ucWinIndex].u8DwMemFormat       = MEM_FMT_NO_USE;
    g_DisplayWindow[ucWinIndex].u8DwMemFieldNum     = 0;
    g_DisplayWindow[ucWinIndex].u16DwMemFieldMode   = 0;
    g_DisplayWindow[ucWinIndex].bDwReadCurFrame     = FALSE;
    // MIU balance
    g_DisplayWindow[ucWinIndex].bDwLRCross          = FALSE;
    g_DisplayWindow[ucWinIndex].bDwLRCenter         = FALSE;
    g_DisplayWindow[ucWinIndex].bDwLRCapCenter      = FALSE; // Capture window centralized
    g_DisplayWindow[ucWinIndex].bDwMemBalance       = FALSE;
    g_DisplayWindow[ucWinIndex].bDwMemForceBase1    = FALSE;  // mem balance mode force using u32DwMemBase[1]
    g_DisplayWindow[ucWinIndex].sDwPQMode.u8MAMode  = SC_MA_MODE_OFF;
    g_DisplayWindow[ucWinIndex].sDwPQMode.u8EOMode  = SC_EO_MODE_OFF;
    g_DisplayWindow[ucWinIndex].sDwPQMode.u8MCMode  = SC_MC_MODE_OFF;
    g_DisplayWindow[ucWinIndex].sDwPQMode.u8FilmMode= SC_FILM_MODE_OFF;
    g_DisplayWindow[ucWinIndex].eWinColorFormat     = eWIN_COLOR_RGB; // window color format for data path
    g_DisplayWindow[ucWinIndex].eInputColorFormat   = eINPUT_COLOR_RGB; // input color format
    g_DisplayWindow[ucWinIndex].eInputColorRange    = eCOLOR_RANGE_LIMIT;
    // etc ...
    g_DisplayWindow[ucWinIndex].bForceYUV           = FALSE;
    g_DisplayWindow[ucWinIndex].bDwForceRot2ScOP    = FALSE; // force rotate using 2sc (both L or R) for output post h down
    g_DisplayWindow[ucWinIndex].bDwForceRot2ScIP    = FALSE; // force rotate using 2sc for input catpre if H > NUM_ROT_LB. (NO H-pre scaling)
    g_DisplayWindow[ucWinIndex].bDwForce1P2ScOP     = FALSE;
    g_DisplayWindow[ucWinIndex].u32DwMemBase[0]     = 0; // extend for balance mode
    g_DisplayWindow[ucWinIndex].u32DwMemSize[0]     = 0;
    g_DisplayWindow[ucWinIndex].u32DwFbSize[0]      = 0;
    g_DisplayWindow[ucWinIndex].u32DwMemBase[1]     = 0; // extend for balance mode
    g_DisplayWindow[ucWinIndex].u32DwMemSize[1]     = 0;
    g_DisplayWindow[ucWinIndex].u32DwFbSize[1]      = 0;
    g_DisplayWindow[ucWinIndex].u8DwUnmuteCntr      = 0; // counter for unmute, after disable freerun
    g_DisplayWindow[ucWinIndex].bDwDirectDisplay    = FALSE; // display memory without input
    g_DisplayWindow[ucWinIndex].bDwMono               = FALSE;
    g_DisplayWindow[ucWinIndex].bDwFakeCapWinEnable   = FALSE;
    g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x       = 0;
    g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y       = 0;
    g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width   = 0;
    g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height  = 0;
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// API apply window ///////////////////////////////
/////////////////////////////////////////////////////////////////////////
//tmp,  supposely, top layer should mute first.
BOOL msAPIWinApply( void )
{
    BYTE    ucIndex, u8ScalerIdx;
    BOOL    bRtn; // window apply 1:success, 0:fail

    // clear Src & Scaler info (can be further updated to retain previous Src info to speedup Src stable time, e.g. don't clear all Src info by setting timingchangebit)
    msDrvSrcStatusClr();
    msDrvScStatusClr(bSC_MAX_NUMS-1, TRUE);

    // Computing & rearrange SC resouces to window
    bRtn = msDrvScPreDispatch( DISPLAY_MAX_NUMS, 0 );

#if 0 // now always 2p
    // after arranged, if 2p used, update the OP2DisplayWin H start address
    msDrvScDispHStartUpdate();
#endif

    WAIT_V_OUTPUT_BLANKING_START();

    msDrvScMuteScreen(bSC_MAIN|bSC_SUB, TRUE);
#if (CHIP_ID == MST9U) || (CHIP_ID == MST9U2)
    msDrvGOPSetTopSC(bSCL1_MAIN|bSCR1_MAIN);
#endif
    for( ucIndex = SCL0_SUB ; ucIndex < SC_MAX_NUMS-1 ; ucIndex++ )
    {
        msDrvIP1SetOutputFreerun(TRUE, ucIndex);
        appmStar_SetPanelTiming(ucIndex);
        //msDrvHistogramSetRange(ucIndex);
        msDrvScMemMRQ(ucIndex, TRUE);
    }

    mStar_SetupFreeRunMode();

    msDrvWinMemAlloc();

    for( ucIndex = 0 ; ucIndex < DISPLAY_MAX_NUMS-1 ; ucIndex++ )
    {
        if( IsWindowDirectDisplay(ucIndex) )
        {
            WORD u16ScalerMask = g_DisplayWindow[ucIndex].u16DwScMaskOut;

            msDrvScPostDispatch(ucIndex, g_DisplayWindow[ucIndex].sDwAutoWin);

            u8ScalerIdx = 0;
            msDrvScSetStatus(u16ScalerMask, SC_STATUS_OUTPUT_SETUPOK);
            while(u16ScalerMask)
            {
                if(u16ScalerMask & BIT0)
                {
                    g_ScalerInfo[u8ScalerIdx].sSiIPCaptureWin = g_DisplayWindow[ucIndex].sDwDisplayWin;
                    msDrvSetupMode(u8ScalerIdx);
                }

                u8ScalerIdx++;
                u16ScalerMask >>= 1;
            }
        }
    }

    return bRtn;
}

///////////////////////////////////////////////////////////////////
//////// API for window asp/overscan recalculate for OSD runtime update case
//////// bReDispatch: TRUE:
////////                    case 1: ReDispatch scaler if window change involves with central line of panel
////////                               i.e. window is L only /R only <=> LR cross ;  L only <=> R only 
////////                    case 2: ReDispatch scaler if change sample range(OverScan) or color space 
//////// bReDispatch: FALSE: 
////////                    In above case 2, user can either reDispatch scaler or mute screen until 4 input frame time passed for frame garbage/noise prevention
////////                    Because default depth of frame buffer is 4, the latest scaler setting will be applied to old memory content after msAPIWinReSetup() excuted.
////////                    That will result in temporal garbage/noise
//////// bEnFreeRun: TRUE: Set Freerun for ReDispatch Case
//////// bEnFreeRun: FALSE: Only support window is not rotated, and input timing is neither HBR2 nor interlace
///////////////////////////////////////////////////////////////////
//////// Summary of using case
//////// Scenario A: bReDispatch: TRUE, bEnFreeRun: TRUE
////////                -> window change involves with central line of panel
//////// Scenario B: bReDispatch: TRUE, bEnFreeRun: FALSE
////////                -> window change involves with central line of panel but window is not rotated and input is neither HBR2 nor interlace
//////// Scenario C: bReDispatch: FALSE
////////                -> others
///////////////////////////////////////////////////////////////////
BOOL msAPIWinReSetup( BYTE u8IdxWin, BOOL bReDispatch, BOOL bEnFreeRun )
{
    BYTE    ucSCIndex;
    BYTE    ucInputPort = MapWin2Port(u8IdxWin);
    WORD    uwSCMaskOld = (g_DisplayWindow[u8IdxWin].u16DwScMaskIn | g_DisplayWindow[u8IdxWin].u16DwScMaskOut);
    WORD    uwSCMaskDel;
    WORD    uwSCMaskNew;
    BOOL    bRtn = FALSE; // window resetup 1:success, 0:fail
#if ENABLE_SUBSC_HOLLOW_OUT
    BYTE    ucSCOrder = msReadByte(SCL0_00_80);
#endif
    // input timing change, skip setting, for ISR mute case
    if(InputTimingChangeFlag(MapWin2Port(u8IdxWin)))
    {
        return  FALSE;
    }

#if 0//(MST9U_ASIC_1P==1)
    bReDispatch = FALSE;
#endif

    if( bReDispatch )
    {

        if(bEnFreeRun)
        {
            WAIT_V_OUTPUT_BLANKING_START();
            msAPIWinFreerun(u8IdxWin, TRUE);
            msDrvScStatusClr(uwSCMaskOld, bEnFreeRun);
            g_DisplayWindow[u8IdxWin].u16DwScMaskIn  = 0;
            g_DisplayWindow[u8IdxWin].u16DwScMaskOut = 0;
            g_DisplayWindow[u8IdxWin].sDwDisplayWin = g_DisplayWindow[u8IdxWin].sDwDisplayWinOri;

            bRtn = msDrvScPreDispatch(u8IdxWin, DualInFlag(ucInputPort)?0:uwSCMaskOld); // HBR2 need to consider HBR2_Left/HBR2_Right, need re-set mux
            g_DisplayWindow[u8IdxWin].eDwStatus = WIN_STATUS_FREERUN;
            
            uwSCMaskNew = (g_DisplayWindow[u8IdxWin].u16DwScMaskIn | g_DisplayWindow[u8IdxWin].u16DwScMaskOut);
            uwSCMaskDel = uwSCMaskOld - (uwSCMaskOld & uwSCMaskNew);
            msDrvScSetStatus(uwSCMaskNew, SC_STATUS_WINDOW_SETUP);
            MENU_LOAD_START();
            for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
            {
                if( (uwSCMaskNew|uwSCMaskDel) & (BIT0<<ucSCIndex) )
                {
                    appmStar_SetPanelTiming(ucSCIndex);
                    if( (uwSCMaskDel) & (BIT0<<ucSCIndex) ) // removed scaler
                    {
                        //msDrvSrcDetach(ucInputPort, ucSCIndex);
                        msDrvSrcDetach(MapScaler2Port(ucSCIndex), ucSCIndex);
                    }
                }
            }
#if ENABLE_SUBSC_HOLLOW_OUT
            msDrvSubSCHollowOut(ucSCOrder&BIT0, ucSCOrder&BIT1);
#endif
            MENU_LOAD_END();
        }
        else // bEnFreeRun == FALSE
        {
            // Only support window is not rotated, and input timing is neither HBR2 nor interlace
            msDrvScStatusClr(uwSCMaskOld, bEnFreeRun);
            g_DisplayWindow[u8IdxWin].u16DwScMaskIn  = 0;
            g_DisplayWindow[u8IdxWin].u16DwScMaskOut = 0;
            g_DisplayWindow[u8IdxWin].sDwDisplayWin = g_DisplayWindow[u8IdxWin].sDwDisplayWinOri;

            bRtn = msDrvScPreDispatch(u8IdxWin, DualInFlag(ucInputPort)?0:uwSCMaskOld);

            g_DisplayWindow[u8IdxWin].eDwStatus = WIN_STATUS_FREERUN;
            uwSCMaskNew = (g_DisplayWindow[u8IdxWin].u16DwScMaskIn | g_DisplayWindow[u8IdxWin].u16DwScMaskOut);
            uwSCMaskDel = uwSCMaskOld - (uwSCMaskOld & uwSCMaskNew);

            if(INPUT_IS_FAKE_DUAL(ucInputPort))
                msDrvScWindowSetup(msDrvScFakeDualGetM(u8IdxWin), TRUE);
            else
                msDrvScWindowSetup(GetLsbBitPos(uwSCMaskNew), TRUE);

            uwSCMaskNew = (g_DisplayWindow[u8IdxWin].u16DwScMaskIn | g_DisplayWindow[u8IdxWin].u16DwScMaskOut);
            uwSCMaskDel = uwSCMaskOld - (uwSCMaskOld & uwSCMaskNew);


            // PQ/Color and some (DI) memory format for new scaler
            if(g_bAPPWindow_Init&&CB_FuncList[eCB_SetupUserPref]!=NULL)
                ((fpSetupUserPref)CB_FuncList[eCB_SetupUserPref])(u8IdxWin);

            if(IsWindowMemBal(u8IdxWin)) // TBD: Need refine to 2 scaler mode? balance? HBR2 ?
            {
                for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
                {
                    if( (uwSCMaskNew - (uwSCMaskOld&uwSCMaskNew)) & (BIT0<<ucSCIndex) )
                    {
                        if(IsScalerLeft(ucSCIndex)) // For window from R only to Cross Center Line, New L scaler using base[1] temporally.
                            g_DisplayWindow[u8IdxWin].bDwMemForceBase1=1;

                        msDrvScSetupPath(ucSCIndex);
                        msDrvIP1SetOutputFreerun(bEnFreeRun, ucSCIndex);
                        g_DisplayWindow[u8IdxWin].bDwMemForceBase1=0;
                    }
                }
            }

            if(IsWindowMemBal(u8IdxWin)) // TBD: Need refine to 2 scaler mode? balance? HBR2 ?
                ForceDelay1ms(GetVSyncTime(ucInputPort)*2); // wait mem data ready for DI & opm read
            else if((uwSCMaskOld & uwSCMaskNew)==0) // no same scaler
                ForceDelay1ms(GetVSyncTime(ucInputPort)*5); // wait IP stable to prevent ISR mute

            MENU_LOAD_START();
            for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
            {
                if( (uwSCMaskDel) & (BIT0<<ucSCIndex) ) // removed scaler
                {
                    appmStar_SetPanelTiming(ucSCIndex);
                    msDrvSrcDetach(MapScaler2Port(ucSCIndex), ucSCIndex);
                }
            }

            msDrvScSetStatus(uwSCMaskNew, SC_STATUS_OUTPUT_SETUPOK);
            for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
                if( (uwSCMaskNew) & (BIT0<<ucSCIndex) )
                    msDrvSetupMode(ucSCIndex);

#if ENABLE_SUBSC_HOLLOW_OUT
            msDrvSubSCHollowOut(ucSCOrder&BIT0, ucSCOrder&BIT1);
#endif
            msDrvScMuteScreen(uwSCMaskNew, bEnFreeRun);
            for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
                if( (uwSCMaskNew) & (BIT0<<ucSCIndex) )
                    msDrvIP1SetOutputFreerun(bEnFreeRun, ucSCIndex);

            msDrvScSetMemFreeze(u8IdxWin, TRUE);
            MENU_LOAD_END();
            msDrvScStatusClr(uwSCMaskDel, TRUE);
            msDrvScMuteScreen(uwSCMaskDel, TRUE);
            ForceDelay1ms(GetVSyncTime(ucInputPort)*2); // wait garbaged data was overwriten
            msDrvScSetMemFreeze(u8IdxWin, FALSE);
        }// end of if(bEnFreeRun)
    }
    else
    {
        MENU_LOAD_START();

        g_DisplayWindow[u8IdxWin].sDwDisplayWin = g_DisplayWindow[u8IdxWin].sDwDisplayWinOri;
        g_DisplayWindow[u8IdxWin].eDwStatus = WIN_STATUS_FREERUN;
        if(INPUT_IS_FAKE_DUAL(ucInputPort))
            msDrvScWindowSetup(msDrvScFakeDualGetM(u8IdxWin), TRUE);
        else
            msDrvScWindowSetup(GetLsbBitPos(uwSCMaskOld), TRUE);

        msDrvScSetStatus(uwSCMaskOld, SC_STATUS_OUTPUT_SETUPOK);

        for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
            if( (uwSCMaskOld) & (BIT0<<ucSCIndex) )
                msDrvSetupMode(ucSCIndex);

#if ENABLE_SUBSC_HOLLOW_OUT
        msDrvSubSCHollowOut(ucSCOrder&BIT0, ucSCOrder&BIT1);
#endif
        MENU_LOAD_END();
    }

    return bRtn;
}

/////////////////////////////////////////////////////////////////////////
///////////////////////// Window State  handler  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPIWinFreerun(BYTE u8IdxWin, BYTE bEnable)
{
#if ENABLE_SW_FLOCK
    BYTE u8IdxSC;
    WORD u16Mask;

    if( bEnable )
    {
        u8IdxSC = 0;
        u16Mask = (g_DisplayWindow[u8IdxWin].u16DwScMaskOut|g_DisplayWindow[u8IdxWin].u16DwScMaskIn);

        while(u16Mask != 0)
        {
            if(u16Mask & BIT0)
            {
                #if CHIP_ID!=MST9U4
                    msDrvScSetDefaultFiclk(u8IdxSC);
                #endif
                drvmStar_SetupFreeRunMode(u8IdxSC);
            }

            u16Mask >>= 1;
            u8IdxSC++;
        }
        g_DisplayWindow[u8IdxWin].u8DwUnmuteCntr = 0;
        msDrvScMuteScreen(g_DisplayWindow[u8IdxWin].u16DwScMaskOut, bEnable);
    }
    else
    {
        g_DisplayWindow[u8IdxWin].u8DwUnmuteCntr = GetVSyncTime(MapWin2Port(u8IdxWin))*4/*tmp, need consider R/W bank map*/;
    }

    u8IdxSC = 0;
    u16Mask = (g_DisplayWindow[u8IdxWin].u16DwScMaskOut|g_DisplayWindow[u8IdxWin].u16DwScMaskIn);
    while(u16Mask != 0)
    {
        if(u16Mask & BIT0)
        {
            msDrvIP1SetOutputFreerun(bEnable, u8IdxSC);
        }
        u16Mask >>= 1;
        u8IdxSC++;
    }
#else
    WORD u16SCMap;
    BYTE u8IdxSC;
    DWORD u32SCIP1RegBase;
    WORD u16Mask;

    if( bEnable )
    {
        u8IdxSC = 0;
        u16SCMap = (g_DisplayWindow[u8IdxWin].u16DwScMaskOut|g_DisplayWindow[u8IdxWin].u16DwScMaskIn);
        while(u16SCMap != 0)
        {
            if(u16SCMap & BIT0)
            {
                msDrvScSetDefaultFiclk(u8IdxSC);
                drvmStar_SetupFreeRunMode(u8IdxSC);
            }

            u16SCMap >>= 1;
            u8IdxSC++;
        }
        g_DisplayWindow[u8IdxWin].u8DwUnmuteCntr = 0;
        msDrvScMuteScreen(g_DisplayWindow[u8IdxWin].u16DwScMaskOut, bEnable);
    }
    else
    {
        g_DisplayWindow[u8IdxWin].u8DwUnmuteCntr = GetVSyncTime(MapWin2Port(u8IdxWin))*4/*tmp, need consider R/W bank map*/;
    }

    u16Mask = (g_DisplayWindow[u8IdxWin].u16DwScMaskOut|g_DisplayWindow[u8IdxWin].u16DwScMaskIn) & bSC_MAIN;
    if( u16Mask )
    {
        msDrvSCCommonWrite(u16Mask, ENABLE);
        u32SCIP1RegBase = SCIP1RegBase(GetLsbBitPos(u16Mask));
        msWriteBit(SC01_04+u32SCIP1RegBase, bEnable, _BIT7);
        msDrvSCCommonWrite(u16Mask, DISABLE);
    }
    u16Mask = (g_DisplayWindow[u8IdxWin].u16DwScMaskOut|g_DisplayWindow[u8IdxWin].u16DwScMaskIn) & bSC_SUB;
    if( u16Mask )
    {
        msDrvSCCommonWrite(u16Mask, ENABLE);
        u32SCIP1RegBase = SCIP1RegBase(GetLsbBitPos(u16Mask));
        msWriteBit(SC03_04+u32SCIP1RegBase, bEnable, _BIT7);
        msDrvSCCommonWrite(u16Mask, DISABLE);
    }

    u16Mask = (g_DisplayWindow[u8IdxWin].u16DwScMaskOut|g_DisplayWindow[u8IdxWin].u16DwScMaskIn);
    if( (u16Mask & bSCL0_MAIN)
     && (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FB_FLOCK || g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FBL_FLOCK) )
    {
        msDrvSCCommonWrite(SCALER_MASK_MAIN, ENABLE);
        if( bEnable )
            msWrite2Byte(SCL0_10_1A, (PanelVTotal-1));
        else
        {
            //g_DisplayWindow[u8IdxWin].u8DwUnmuteCntr = GetVSyncTime ( MapWin2Port ( u8IdxWin ) ) * 4/*tmp, need consider R/W bank map*/;
            msWrite2Byte(SCL0_10_1A, 0xFFF);

            if(g_ScalerInfo[SCL0_MAIN].u8SiSyncMode == FLM_FB_FLOCK)
            {
                msWriteByteMask(SCL0_20_32,BIT0,BIT0);//Switch output blank polling from VDE falling to Vsync Raising
                WAIT_V_OUTPUT_BLANKING_START();
                msWriteBit(SCL0_10_21, TRUE, _BIT7);
                msWriteByteMask(SCL0_20_32,0x00,BIT0);
            }
        }
        msDrvSCCommonWrite(SCALER_MASK_MAIN, DISABLE);
    }

#endif
}

/////////////////////////////////////////////////////////////////////////
// Set Window Display On Top, only for 2 full screen overlapped wins to do fast swap //
/////////////////////////////////////////////////////////////////////////
#if (CHIP_ID == MST9U) || (CHIP_ID == MST9U2)
void msAPIWinDisplayOnTop(BYTE u8IdxWin)
{
    WORD u16Mask = g_DisplayWindow[u8IdxWin].u16DwScMaskOut;

    MENU_LOAD_START();
    msDrvGOPSetTopSC(u16Mask);
#if ENABLE_SUBSC_HOLLOW_OUT
    msDrvSubSCHollowOut(u16Mask&(bSCL1_MAIN|bSCL1_SUB), u16Mask&(bSCR1_MAIN|bSCR1_SUB));
#endif
    MENU_LOAD_END();
}
#endif

/////////////////////////////////////////////////////////////////////////
//////////////////////// input port freerun update ///////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPISrcFreerunFuncUpdate( BYTE InputPort )
{
    BYTE ucIdxWin;

    if( INPUT_IS_FAKE_DUAL_S(InputPort) && !INPUT_IS_FAKE_DUAL_MONO(InputPort) )
    {
        drvmStar_SetupFreeRunMode(MapPort2DetScIndex(InputPort));
    }
    else
    {
        for( ucIdxWin = DISPLAY_WIN1 ; ucIdxWin < DISPLAY_MAX_NUMS ; ucIdxWin++ )
            if( (InputPort == g_DisplayWindow[ucIdxWin].u8DwPortIndex)
              &&(WIN_STATUS_IDLE != g_DisplayWindow[ucIdxWin].eDwStatus) )
                msAPIWinFreerun(ucIdxWin, TRUE);
    }
}

/////////////////////////////////////////////////////////////////////////
//////////////////////// input switch port update ////////////////////////////
/////////////////////////////////////////////////////////////////////////
#if 0
void msAPISrcSwitchUpdate( BYTE InputPort )
{
    BYTE    u8SiScIndex = 0;
    WORD    SC_BITMASK;
    BYTE    u8InputPortBk;

    SC_BITMASK = g_InputPort[InputPort].u16IPScMask;

    while(SC_BITMASK!=0)
    {
        if(SC_BITMASK & BIT0)
        {
            if(PortSwitchMode(u8SiScIndex)==PS_AUTO)
            {
                u8InputPortBk = MapScaler2Port(u8SiScIndex);
                msDrvSrcDetach(u8InputPortBk, u8SiScIndex);
                u8InputPortBk = (u8InputPortBk+ 1) % Input_Nums;
                msDrvSrcAttach(u8InputPortBk, u8SiScIndex);
                g_DisplayWindow[MapScaler2Win(u8SiScIndex)].u8DwPortIndex = u8InputPortBk;
            }
        }

        SC_BITMASK >>=1;
        u8SiScIndex ++;
    }
}
#endif

void msAPISrcGetInputTiming(BYTE InputPort, InputTimingType *pType)
{
    memcpy( pType, &g_InputPort[InputPort].sIPInputTiming, sizeof(InputTimingType) );
}

BYTE msAPISrcGetVsyncWidth(BYTE u8InputPort)
{
    BYTE u8SCDetWin;
    DWORD u32SCIP1RegBase;
    BYTE u8VsyncWidth;

#if ENABLE_SUBSCIN
    u8SCDetWin = msDrvScGetIdleScalerS();
#else
    u8SCDetWin = msDrvScGetIdleScalerM();
#endif
    u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
    msDrvSetIPMux(u8InputPort, u8SCDetWin);// set idle SC to detect input

    if( u8SCDetWin & BIT0 )   // MAIN
    {
       msWriteByteMask(SC01_06+u32SCIP1RegBase, 0, BIT6);//disable DE only mode
       ForceDelay1ms(500);
       msWriteByteMask(SC01_3F+u32SCIP1RegBase, 0, BIT5); //Vsync Pulse Width Read Enable
       u8VsyncWidth = msReadByte(SC01_45+u32SCIP1RegBase);
    }
    else
    {
       msWriteByteMask(SC03_06+u32SCIP1RegBase, 0, BIT6);//disable DE only mode
       ForceDelay1ms(500);
       msWriteByteMask(SC03_3F+u32SCIP1RegBase, 0, BIT5); //Vsync Pulse Width Read Enable
       u8VsyncWidth = msReadByte(SC03_45+u32SCIP1RegBase);
    }

    return u8VsyncWidth; //return clock count, DVI by idclk domain, ADC by xtal clk domain

}

BYTE msAPISrcGetHsyncWidth(BYTE u8InputPort)
{
    BYTE u8SCDetWin;
    DWORD u32SCIP1RegBase;
    WORD u16HsyncWidth;

#if ENABLE_SUBSCIN
    u8SCDetWin = msDrvScGetIdleScalerS();
#else
    u8SCDetWin = msDrvScGetIdleScalerM();
#endif
    u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
    msDrvSetIPMux(u8InputPort, u8SCDetWin);// set idle SC to detect input

    if( u8SCDetWin & BIT0 )   // MAIN
    {
        msWriteByteMask(SC01_06+u32SCIP1RegBase, 0, BIT6);//disable DE only mode
        msDrvSoftwareReset( RST_IP_MAIN,  u8SCDetWin);
        ForceDelay1ms(500);
        u16HsyncWidth = msReadByte(SC01_44+u32SCIP1RegBase);
#if 0
        msWriteByteMask(SC01_3F+u32SCRegBase, BIT5, BIT5); //Hsync Pulse Width High Byte Read Enable
        u16HsyncWidth |= ((WORD)msReadByte(SC01_45+u32SCRegBase)) << 8)
#endif
    }
    else
    {
        msWriteByteMask(SC03_06+u32SCIP1RegBase, 0, BIT6);//disable DE only mode
        msDrvSoftwareReset( RST_IP_SUB,  u8SCDetWin);
        ForceDelay1ms(500);
        u16HsyncWidth = msReadByte(SC03_44+u32SCIP1RegBase);
#if 0
        msWriteByteMask(SC03_3F+u32SCRegBase, BIT5, BIT5); //Hsync Pulse Width High Byte Read Enable
        u16HsyncWidth |= ((WORD)msReadByte(SC03_45+u32SCRegBase)) << 8)
#endif
    }

    return u16HsyncWidth;  //return clock count, DVI by idclk domain, ADC by xtal clk domain
}

WORD msAPISrcGetHFrontPorchWidth(BYTE u8InputPort)
{
    BYTE u8SCDetWin;
    DWORD u32SCIP1RegBase;
    WORD u16HFrontPorch;

#if ENABLE_SUBSCIN
    u8SCDetWin = msDrvScGetIdleScalerS();
#else
    u8SCDetWin = msDrvScGetIdleScalerM();
#endif
    u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
    msDrvSetIPMux(u8InputPort, u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
       msWriteByteMask(SC01_06+u32SCIP1RegBase, 0, BIT6|BIT4);
    else
       msWriteByteMask(SC03_06+u32SCIP1RegBase, 0, BIT6|BIT4);

    ForceDelay1ms(500);
    u16HFrontPorch = msDrvIP1ReadHtotal(u8SCDetWin) - msDrvIP1ReadAutoHEnd(u8SCDetWin) - 1;

    return u16HFrontPorch; //return pixel count
}

WORD msAPISrcGetVFrontPorchWidth(BYTE u8InputPort)
{
    BYTE u8SCDetWin;
    DWORD u32SCIP1RegBase;
    WORD u16VFrontPorch;

#if ENABLE_SUBSCIN
    u8SCDetWin = msDrvScGetIdleScalerS();
#else
    u8SCDetWin = msDrvScGetIdleScalerM();
#endif
    u32SCIP1RegBase = SCIP1RegBase(u8SCDetWin);
    msDrvSetIPMux(u8InputPort, u8SCDetWin);

    if( u8SCDetWin & BIT0 )   // MAIN
       msWriteByteMask(SC01_06+u32SCIP1RegBase, 0, BIT6|BIT3);
    else
       msWriteByteMask(SC03_06+u32SCIP1RegBase, 0, BIT6|BIT3);

    ForceDelay1ms(500);
    u16VFrontPorch = msDrvIP1ReadVtotal(u8SCDetWin) - msDrvIP1ReadAutoVEnd(u8SCDetWin) - 1;

    return u16VFrontPorch; //return pixel count
}

void msAPISrcSetSyncStableCnt(BYTE u8InputPort, WORD uwValidCnt, WORD uwLossCnt)
{
    g_InputPort[u8InputPort].u16IPInputSyncValidThd = uwValidCnt;
    g_InputPort[u8InputPort].u16IPInputSyncLossThd  = uwLossCnt;
}

/////////////////////////////////////////////////////////////////////////
//////////////////////// Set TMDS HPD to bSetHPD //////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPISrcInitTmdsHPD( BOOL bSetHPD )
{
    BYTE InputPort;

    for( InputPort=First_Input_Port; InputPort < Input_Nums; InputPort++ )
    {
        if(INPUT_IS_TMDS(InputPort))
        {
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, bSetHPD);
        }
    }
}

/////////////////////////////////////////////////////////////////////////
//////////////////////Set HPD Duration for InputPort /////////////////////////
//////////////// HPD low -> clk low -> clk high -> HPD high ///////////////////
/////////////////////////////////////////////////////////////////////////
void msAPISrcSetHPD(BYTE u8InputPort, WORD uwDurationMs)
{
    if((uwDurationMs))//&&(HPD_Enable_Flag))
    {
        printMsg("msAPISrcSetHPD ttttt");
        uwDurationMs = (uwDurationMs/10)*10; // align 10ms
        if(uwDurationMs<((WORD)DFT_HPDCLK_DIFF_PERIOD*2+100)) // check reasonable value for HPD CLK to have 100ms each as suggested.
            uwDurationMs = ((WORD)DFT_HPDCLK_DIFF_PERIOD*2+100);

        g_InputPort[u8InputPort].sIPHPDCntr.u16HPDCnt     = uwDurationMs;
        g_InputPort[u8InputPort].sIPHPDCntr.u16HPDClkLow  = uwDurationMs - DFT_HPDCLK_DIFF_PERIOD;
        g_InputPort[u8InputPort].sIPHPDCntr.u16HPDClkHigh = DFT_HPDCLK_DIFF_PERIOD;
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, DISABLE);

    }
    else
    {
        g_InputPort[u8InputPort].sIPHPDCntr.u16HPDCnt     = 0;
        msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, ENABLE);
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, ENABLE);
    }
}
/////////////////////////////////////////////////////////////////////////
//////////////////////Set Rterm Control for InputPort /////////////////////////
/////////////////////////////////////////////////////////////////////////
/*
void msAPISrcRterClockCtrl(BYTE u8InputPort, WORD uwDurationMs)
{
    if(uwDurationMs)
    {
        uwDurationMs = (uwDurationMs/10)*10; // align 10ms

        g_InputPort[u8InputPort].sIPHPDCntr.u16HPDCnt     = uwDurationMs;
        g_InputPort[u8InputPort].sIPHPDCntr.u16HPDClkLow  = uwDurationMs + 1;//Set clock low in this function instead of event handler
        g_InputPort[u8InputPort].sIPHPDCntr.u16HPDClkHigh = 0;
        msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, DISABLE);
    }
    else
    {
        g_InputPort[u8InputPort].sIPHPDCntr.u16HPDCnt     = 0;
        msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, ENABLE);
    }
}
*/
/////////////////////////////////////////////////////////////////////////
//////////////////////////Check Is Cable Connected/////////////////////////
/////////////////////////////////////////////////////////////////////////
Bool msAPISrcIsCableConnected( BYTE InputPort )
{
#if ENABLE_VGA_INPUT
    if(INPUT_IS_MUX_ADC(InputPort))
    {
        if(hwDSUBCable_Pin)
            return FALSE;
    }
    else
#endif
    {
        if(msAPI_combo_IPGetCableDetect((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)==COMBO_IP_CABLE_NONE)
            return FALSE;
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API set input current color space //////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPISrcSetCurColorSpace( BYTE ucWinIndex, EN_INPUT_COLOR eColor )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);

    InputColorFormat(ucInputPort) = (EN_InputColorFormatType)eColor;
    msDrvIP1SetInputYpbprFormat(g_DisplayWindow[ucWinIndex].u16DwScMaskIn, eColor);
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API get input current color space //////////////////////////
/////////////////////////////////////////////////////////////////////////
EN_INPUT_COLOR msAPISrcGetCurColorSpace( BYTE ucWinIndex )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);

    return (EN_INPUT_COLOR)InputColorFormat(ucInputPort);
}
#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
/////////////////////////////////////////////////////////////////////////
/////////////////// API set input current color range //////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPISrcSetCurColorRange( BYTE ucWinIndex, EN_COLOR_RANGE eColorRange )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);

    InputRangeFormat(ucInputPort) = (EN_InputRangeFormatType)eColorRange;
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API get input current color range //////////////////////////
/////////////////////////////////////////////////////////////////////////
EN_COLOR_RANGE msAPISrcGetCurColorRange( BYTE ucWinIndex )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);

    return (EN_COLOR_RANGE)InputRangeFormat(ucInputPort);
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API set input current colorimetry //////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPISrcSetCurColorimetry( BYTE ucWinIndex, EN_INPUT_YUV_COLORIMETRY_TYPE eColorimetry )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);

    InputColorimetry(ucInputPort) = (EN_InputColorimetryType)eColorimetry;
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API get input current colorimetry //////////////////////////
/////////////////////////////////////////////////////////////////////////
EN_INPUT_YUV_COLORIMETRY_TYPE msAPISrcGetCurColorimetry( BYTE ucWinIndex )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);

    return (EN_INPUT_YUV_COLORIMETRY_TYPE)InputColorimetry(ucInputPort);
}
#endif

/////////////////////////////////////////////////////////////////////////
/////////////////// API get input current color space //////////////////////////
/////////////////////////////////////////////////////////////////////////
Bool msAPISrcCheckHDMI422( BYTE ucInputPort )
{
    BYTE ucSrcColor;
    EN_API_SRC_TYPE eSrcType = msAPISrcGetType(ucInputPort);

    if(!INPUT_IS_VGA(ucInputPort))
    {
        ucSrcColor = msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[ucInputPort].eIPMux)).ucColorType;
        if( (eSrcType & (eAPI_SRCTYPE_HDMI|eAPI_SRCTYPE_MHL))
         && (ucSrcColor == 1 ) )
            return TRUE;
    }

    return FALSE;
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API get input current color space //////////////////////////
/////////////////////////////////////////////////////////////////////////
EN_INPUT_COLOR msAPISrcGetPktColorSpace( BYTE ucWinIndex )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);
    BYTE ucSrcColor;

    if(INPUT_IS_VGA(ucInputPort))
    {
        ucSrcColor = eINPUT_COLOR_RGB;
    }
    else if(INPUT_IS_VD(ucInputPort))
    {
        //ucSrcColor = eINPUT_COLOR_YPBPR;
        ucSrcColor = eINPUT_COLOR_444;
    }
    else
    {
        ucSrcColor = msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[ucInputPort].eIPMux)).ucColorType;
    }

    #if 0
    if(ucSrcColor==0)
        ucSrcColor = eINPUT_COLOR_RGB;
    else
        ucSrcColor = eINPUT_COLOR_YPBPR;
    #endif

    return (EN_INPUT_COLOR)ucSrcColor;
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API get input current color range //////////////////////////
/////////////////////////////////////////////////////////////////////////
EN_COLOR_RANGE msAPISrcGetPktColorRange( BYTE ucWinIndex )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);
    BYTE ucSrcColorRange;

    if(INPUT_IS_VGA(ucInputPort))
    {
        if(USER_PREF_INPUT_COLOR_FORMAT(ucWinIndex) == INPUT_COLOR_YPBPR)
            ucSrcColorRange = COMBO_COLOR_RANGE_LIMIT;
        else
            ucSrcColorRange = COMBO_COLOR_RANGE_FULL;
    }
     else
        ucSrcColorRange = msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[ucInputPort].eIPMux)).ucColorRange;

    switch(ucSrcColorRange)
    {
        case COMBO_COLOR_RANGE_LIMIT:
                ucSrcColorRange = eCOLOR_RANGE_LIMIT;
            break;
        case COMBO_COLOR_RANGE_DEFAULT:
        case COMBO_COLOR_RANGE_FULL:
                ucSrcColorRange = eCOLOR_RANGE_FULL;
            break;
    }

    return (EN_COLOR_RANGE)ucSrcColorRange;
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API Check Input is HDMI mode //////////////////////////
/////////////////////////////////////////////////////////////////////////
Bool msAPISrcCheckHDMIMode( BYTE ucWinIndex )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);
    Bool bIsHDMI = FALSE;

    if(!INPUT_IS_VGA(ucInputPort))
        bIsHDMI = msAPI_combo_IPGetTmdsModeInfo((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux)?TRUE:FALSE;

    return bIsHDMI;
}

/////////////////////////////////////////////////////////////////////////
/////////////////// API get input current color space //////////////////////////
/////////////////////////////////////////////////////////////////////////
EN_INPUT_YUV_COLORIMETRY_TYPE msAPISrcGetPktYuvColorimetry( BYTE ucWinIndex )
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);
    BYTE ucSrcYuvColorimetry;

    if(INPUT_IS_VGA(ucInputPort))
        ucSrcYuvColorimetry = eINPUT_YUV_COLORIMETRY_NODATA;
    else
        ucSrcYuvColorimetry = msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[ucInputPort].eIPMux)).ucYuvColorimetry;

    return (EN_INPUT_YUV_COLORIMETRY_TYPE)ucSrcYuvColorimetry;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////// Get Source Type  //////////////////////////////
/////////////////////////////////////////////////////////////////////////
EN_API_SRC_TYPE msAPISrcGetType( BYTE InputPort )
{
    return (EN_API_SRC_TYPE)(g_InputPort[InputPort].eIPType & (eAPI_SRCTYPE_RESERVED-1));
}

void msAPISrcSetType( BYTE InputPort, EN_API_SRC_TYPE eType, BYTE bEnable )
{
    if( bEnable )
    {
        g_InputPort[InputPort].eIPType |= (EN_INPUTPORT_TYPE)eType;
    }
    else
    {
        g_InputPort[InputPort].eIPType &= (EN_INPUTPORT_TYPE)(~eType);
    }
}
/////////////////////////////////////////////////////////////////////////
////////// Currently Only 1 Fake Dual Port (to 1 Window) Is Supported. ////////////
////////// Timing Change Is Required After msAPISrcFakeDualCfg() //////////////
////////// Do not switch to FakeDual when InputPortS is being used. //////////////
////////// Set M/S to the same port for Mono /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPISrcFakeDualCfg(BYTE InputPortM, BYTE InputPortS, BYTE bEnable)
{
    BYTE u8SCDetWin = MapPort2DetScIndex(InputPortS);

    msAPISrcSetType(InputPortM, eAPI_SRCTYPE_FAKE_DUAL, bEnable);
    msAPISrcSetType(InputPortS, eAPI_SRCTYPE_FAKE_DUAL_S, bEnable);

    if(!bEnable && (u8SCDetWin!=SC_MAX_NUMS)) // Fake Dual Port on line, being used
    {
        msDrvSrcDetach(InputPortS, u8SCDetWin); // SUPPOSE Fake Dual Port only Assigned on 1 Scaler/Win.
        msDrvSrcAttach(InputPortM, u8SCDetWin);
    }

    Set_InputTimingChangeFlag(InputPortM);
    Set_InputTimingChangeFlag(InputPortS);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////// Get next port index /////////////////////////////
/////////////////////////////////////////////////////////////////////////
BYTE msAPISrcGetWinMask( BYTE InputPort )
{
    BYTE u8WinMask = 0;
    BYTE u8IdxWin;

    for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
        if( (InputPort == g_DisplayWindow[u8IdxWin].u8DwPortIndex)
          &&(WIN_STATUS_IDLE != g_DisplayWindow[u8IdxWin].eDwStatus) )
            u8WinMask |= (1<<u8IdxWin);

    return u8WinMask;
}
/////////////////////////////////////////////////////////////////////////
/////////////////// Check if input status idle or occupied ///////////////////////
/////////////////////////////////////////////////////////////////////////
Bool msAPISrcCheckStatusIdle( BYTE InputPort )
{
    return (g_InputPort[InputPort].eIPStatus == IP_STATUS_IDLE);
}

/////////////////////////////////////////////////////////////////////////
/////////////// update input port status & to all scalers assigned on it /////////////
/////////////////////////////////////////////////////////////////////////
void msAPISrcStatusUpdate( BYTE InputPort, EN_INPUT_STATUS InputStatus)
{
    BYTE    u8SiScIndex = 0;
    WORD    SC_BITMASK;
    EN_SCALER_DISP_STATUS   SC_STATUS = SC_STATUS_IDLE;

    SC_BITMASK = g_InputPort[InputPort].u16IPScMask;

    g_InputPort[InputPort].eIPStatus = InputStatus;

    switch(g_InputPort[InputPort].eIPStatus)
    {
        case IP_STATUS_FINDMODEOK:
            SC_STATUS = SC_STATUS_UI_SETUP;
            break;

        case IP_STATUS_NOSYNC:
            SC_STATUS = SC_STATUS_OUTPUT_SETUP;
            break;

        default:
            SC_STATUS = SC_STATUS_FREERUN;
            break;
    }

    while(SC_BITMASK!=0)
    {
        if(SC_BITMASK & BIT0)
        {
            //if( (g_InputPort[InputPort].eIPStatus != IP_STATUS_FINDMODEOK)
            //  ||(g_ScalerInfo[u8SiScIndex].eSiDispStatus < SC_STATUS_UI_SETUP))
            //{
            //if( g_ScalerInfo[u8SiScIndex].eSiDispStatus != SC_STATUS_IDLE)
                g_ScalerInfo[u8SiScIndex].eSiDispStatus = SC_STATUS;
            //}
        }

        SC_BITMASK >>=1;
        u8SiScIndex ++;
    }
}

WORD msAPISrcOfflineDetect(WORD u16InputPortMask,WORD u16DetectTimeEach,WORD u16DetectCount)
{
    struct DetectPortStruct InputPort[Input_Nums]={{INIT,SC_MAX_NUMS,0,0,0,0,0}};
    u16InputPortMask &= ((1<<Input_Nums)-1);
    WORD DetectResultMask = u16InputPortMask;
    //sort total time if could
    WORD u8ScalerMask = (1<<SC_MAX_NUMS)-1;
    bit const bSwitchVGAType = g_bInputSOGFlag;
    while(1)//while(InputPortMask)
    {
        BYTE u8ScalerIndex = 0; //do not use off-line scaler
        BYTE u8PortIndex;
        BOOL bSwitchScalerFlag = FALSE;
        for(u8PortIndex=0;u8PortIndex < Input_Nums ;u8PortIndex++)
        {
            switch(InputPort[u8PortIndex].Status)
            {
                case INIT:
                    if(!(u16InputPortMask&(BIT0<<u8PortIndex)))// ||!msAPISrcIsCableConnected(u8PortIndex)
                    {
                        u16InputPortMask &= ~(BIT0<<u8PortIndex);
                        DetectResultMask &= ~(BIT0<<u8PortIndex);
                        InputPort[u8PortIndex].Status = END_SCAN;
                        continue;
                    }
                    else
                    {
                        InputPort[u8PortIndex].DetectTimeTotal= u16DetectCount*u16DetectTimeEach; // DetectTimeTotal, DetectTimePeriod, DetectTimeCurrent
                        InputPort[u8PortIndex].DetectTimeInterval= u16DetectTimeEach;

                        if (g_InputPort[u8PortIndex].eIPStatus == IP_STATUS_IDLE)
                        {
                            for(;u8ScalerIndex<SC_MAX_NUMS-1;u8ScalerIndex++)
                            {
                                if ( ((g_ScalerInfo[u8ScalerIndex].eSiDispStatus == SC_STATUS_IDLE)||(g_ScalerInfo[u8ScalerIndex].eSiDispStatus == SC_STATUS_HOLLOW))
                                 &&(u8ScalerMask&(1<<u8ScalerIndex)) )
                                {
                                    u8ScalerMask &= ~(BIT0<<u8ScalerIndex);
                                    InputPort[u8PortIndex].Scaler = u8ScalerIndex;
                                    msAPI_combo_IPConfiguration(TRUE, (EN_COMBO_IP_SELECT)g_InputPort[u8PortIndex].eIPMux, g_InputPort[u8PortIndex].eIPType);
                                    msDrvSetIPMux(u8PortIndex, InputPort[u8PortIndex].Scaler); //switch MUX
                                    msDrvIP1SetImageVStart( 0, u8ScalerIndex);
                                    bSwitchScalerFlag = TRUE;
                                    InputPort[u8PortIndex].Status = ALLOCATED_RESOURCE;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            InputPort[u8PortIndex].Scaler = g_InputPort[u8PortIndex].u8IPScIndex;
                            InputPort[u8PortIndex].Status = READY_TO_DETECT;
                        }
                    }
                    break;
                case ALLOCATED_RESOURCE:
                case READY_TO_DETECT:

                    if(InputPort[u8PortIndex].DetectTimeStep % InputPort[u8PortIndex].DetectTimeInterval== 0)
                    {
                        // H period
                        WORD inputValue = msDrvIP1ReadHperiod(InputPort[u8PortIndex].Scaler);//_msRead2Byte(SC0_E4) & MASK_13BIT;
                        if (InputPort[u8PortIndex].DetectTimeStep == 0)
                        {
                            InputPort[u8PortIndex].Hperiod = inputValue;
                        }
                        else if( (labs(InputPort[u8PortIndex].Hperiod - inputValue)>((INPUT_IS_DISPLAYPORT(u8PortIndex))?HPeriod_Torlance*8:HPeriod_Torlance)) // H_tolerance
                              || (inputValue == SC_MASK_IP1_HPD || inputValue < 10) )
                        {
                            #if MS_VGA_SOG_EN
                            if(INPUT_IS_VGA(u8PortIndex) && (bSwitchVGAType==g_bInputSOGFlag))
                            {
                                g_bInputSOGFlag^=1;
                                msDrvSetIPMux(u8PortIndex, InputPort[u8PortIndex].Scaler); //switch MUX
                                bSwitchScalerFlag = TRUE;
                                InputPort[u8PortIndex].DetectTimeStep = 0;
                                break;
                            }
                            #endif
                            DetectResultMask &= ~(BIT0<<u8PortIndex);
                            InputPort[u8PortIndex].DetectTimeStep = InputPort[u8PortIndex].DetectTimeTotal;
                        }

                        // Vtotal
                        inputValue = msDrvIP1ReadVtotal(InputPort[u8PortIndex].Scaler);//_msRead2Byte(SC0_E2) & MASK_11BIT;
                        if(InputPort[u8PortIndex].DetectTimeStep == 0)
                        {
                            InputPort[u8PortIndex].Vtotal = inputValue;
                        }
                        else if( (labs(InputPort[u8PortIndex].Vtotal - inputValue)>VTotal_Torlance)  // v_tolerance
                              || (inputValue == SC_MASK_IP1_HVTT || inputValue < 200 ) )
                        {
                            #if MS_VGA_SOG_EN
                            if(INPUT_IS_VGA(u8PortIndex) && (bSwitchVGAType==g_bInputSOGFlag))
                            {
                                g_bInputSOGFlag^=1;
                                msDrvSetIPMux(u8PortIndex, InputPort[u8PortIndex].Scaler); //switch MUX
                                bSwitchScalerFlag = TRUE;
                                InputPort[u8PortIndex].DetectTimeStep = 0;
                                break;
                            }
                            #endif
                            DetectResultMask &= ~(BIT0<<u8PortIndex);
                            InputPort[u8PortIndex].DetectTimeStep = InputPort[u8PortIndex].DetectTimeTotal;
                        }

                        if(InputPort[u8PortIndex].DetectTimeStep >= InputPort[u8PortIndex].DetectTimeTotal)
                        {
                            if(InputPort[u8PortIndex].Status == ALLOCATED_RESOURCE)
                            {
                                msAPI_combo_IPConfiguration(FALSE, (EN_COMBO_IP_SELECT)g_InputPort[u8PortIndex].eIPMux, g_InputPort[u8PortIndex].eIPType);//return function
                                g_InputPort[u8PortIndex].bIPInputHBR2 = FALSE;
                                u8ScalerMask |= BIT0<<InputPort[u8PortIndex].Scaler;
                            }
                            if(INPUT_IS_VGA(u8PortIndex) && (bSwitchVGAType!=g_bInputSOGFlag))
                            {
                                g_bInputSOGFlag=bSwitchVGAType;
                                msDrvSetIPMux(u8PortIndex, InputPort[u8PortIndex].Scaler); //switch MUX
                                bSwitchScalerFlag = TRUE;
                            }
                            u16InputPortMask &= ~(BIT0<<u8PortIndex);
                            InputPort[u8PortIndex].Status = END_SCAN;
                        }
                    }

                    InputPort[u8PortIndex].DetectTimeStep++;
                    break;

                case END_SCAN:
                default:
                    break;
            }
        }
        if(!u16InputPortMask)
            break;
        if(bSwitchScalerFlag)
           Delay1ms(150);
        Delay1ms(1);
    }
    return  DetectResultMask;

}

void msAPISrcOfflineDetectPort(BYTE u8InputPort)
{
    static BYTE u8PortPrev = 0xFF;

    if( (u8PortPrev != 0xFF) && (!g_InputPort[u8PortPrev].u16IPScMask) )
        msAPI_combo_IPConfiguration(FALSE, (EN_COMBO_IP_SELECT)g_InputPort[u8PortPrev].eIPMux, g_InputPort[u8PortPrev].eIPType);
    u8PortPrev = u8InputPort;

    msAPI_combo_IPConfiguration(TRUE, (EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, g_InputPort[u8InputPort].eIPType);
    msDrvSetIPMux(u8InputPort, SCL0_OFFLINE);

    msWriteByteMask(SCL0_13_04,     0, _BIT7);
    msWriteByteMask(SCL0_13_41, _BIT7, _BIT7);
}

BOOL msAPISrcOfflineDetectStatus(void)
{
    WORD u16HPRD, u16Vtt;
    BOOL bStatus = TRUE;

    u16HPRD = msRead2Byte(SCL0_13_40) & SC_MASK_IP1_HPD;
    u16Vtt = msRead2Byte(SCL0_13_3E) & SC_MASK_IP1_HVTT;

    if( (u16HPRD < 10) || (u16HPRD == SC_MASK_IP1_HPD))
        bStatus = FALSE;

    if( (u16Vtt < 200) || (u16Vtt == SC_MASK_IP1_HVTT))
        bStatus = FALSE;

    //MSSCALERAP_PRINT("HPRD:%x\n", u16HPRD);
    //MSSCALERAP_PRINT("VTT:%x\n", u16Vtt);

    return bStatus;
}

void msAPIWinEnableFrameColor(BYTE u8IdxWin, BYTE bEnable)
{
    msDrvScMuteScreen(g_DisplayWindow[u8IdxWin].u16DwScMaskOut, bEnable);
    if (!bEnable)
    {
        WAIT_V_OUTPUT_BLANKING_START();
        msAPIWinFrameColorRGB(u8IdxWin, DEF_FRAME_COLOR_PATTERN);
    }
}
void msAPIWinSetBackgoundColor(BYTE u8Red, BYTE u8Green, BYTE u8Blue)
{
    msDrvSCCommonWrite(SCALER_MASK_ALL, ENABLE);
    msWrite3Byte(SCL0_10_33, u8Red|(u8Green<<8)|(u8Blue<<16));
    msDrvSCCommonWrite(SCALER_MASK_ALL, DISABLE);
}
void msAPIWinFrameColorRGB(BYTE u8IdxWin, BYTE u8Pattern)
{
    BYTE    u8ScIndex = 0;
    DWORD   u32SCRegBase;
    WORD    uwScMask=g_DisplayWindow[u8IdxWin].u16DwScMaskOut;
    DWORD   u32FrmaeColor;

    u32FrmaeColor = (((DWORD)((u8Pattern & 0x03) << 6) | (((u8Pattern&_BIT0)?0x3F:0x00))) << 16);
    u32FrmaeColor |= (((DWORD)((u8Pattern & 0x1C) << 3) | (((u8Pattern&_BIT2)?0x1F:0x00))) << 8);
    u32FrmaeColor |=  ((DWORD)((u8Pattern & 0xE0)) | (((u8Pattern&_BIT5)?0x1F:0x00)));

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            u32SCRegBase = SCRegBase(u8ScIndex);

            if(u8ScIndex & BIT0) // Main
                msWriteByte(SC10_48+u32SCRegBase, u8Pattern);
            else  // Sub
                msWriteByte(SC0F_2F+u32SCRegBase, u8Pattern);
        }

        uwScMask >>=1;
        u8ScIndex ++;
    }

    msDrvSCCommonWrite(SCALER_MASK_ALL, ENABLE);
    msWrite3Byte(SCL0_10_33, u32FrmaeColor);
    msDrvSCCommonWrite(SCALER_MASK_ALL, DISABLE);
}

/******************************************************************************************************************************************
* pDataBGR = BYTE pointer to BGR data arry
* pDataLSB = BYTE pointer to lsb data 2'b00, 2'bR, 2'bG, 2'bB
******************************************************************************************************************************************/
void msAPIWinDrawLine(BYTE u8IdxWin, WORD u16PositionV, WORD u16PositionH, WORD u16PixCount, BYTE *pDataBGR, BYTE *pDataLSB)
{
    DWORD u32BaseAddr = g_DisplayWindow[u8IdxWin].u32DwMemBase[0];
    WORD uwScMask = g_DisplayWindow[u8IdxWin].u16DwScMaskOut;
    DWORD u32SCRegBase;
    BYTE u8ScIndex = 0;

    if( g_DisplayWindow[u8IdxWin].u8DwMemFormat != MEM_FMT_444_10 )
    {
        MSSCALERAP_PRINT("CAN ONLY DRAW BMP ON MEM_444_10 FORMAT, u8DwMemFormat:%d\n", g_DisplayWindow[u8IdxWin].u8DwMemFormat);
        return;
    }

    if( u32BaseAddr & _BIT31 )
        u32BaseAddr = (u32BaseAddr & (~_BIT31)) | R2_ADDR_MASK_MIU1;

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            u32SCRegBase = SCRegBase(u8ScIndex);
            if( u8ScIndex & _BIT0 )
            {
                msWriteByteMask(SC12_03+u32SCRegBase, _BIT3, _BIT3);
                msWriteByteMask(SC12_0F+u32SCRegBase, 0x08, 0x0F);
            }
            else
            {
                msWriteByteMask(SC12_83+u32SCRegBase, _BIT3, _BIT3);
                msWriteByteMask(SC12_8F+u32SCRegBase, 0x08, 0x0F);
            }
        }
        uwScMask >>=1;
        u8ScIndex ++;
    }

    u8ScIndex = MapWin2ScIndex(u8IdxWin);
    msDrvScMemDrawLine(u8ScIndex, u32BaseAddr, u16PositionV, u16PositionH, u16PixCount, pDataBGR, pDataLSB);
}

/******************************************************************************************************************************************
* OP2 timing gen. u8Pattern:0x00~0x0B
******************************************************************************************************************************************/
void msAPIOp2PatternGenerator(BYTE u8IdxWin, BOOL bEnable, BYTE u8Pattern)
{
    BYTE    u8ScIndex = 0;
    DWORD   u32SCRegBase;
    WORD    uwScMask=g_DisplayWindow[u8IdxWin].u16DwScMaskOut;

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            u32SCRegBase = SCRegBase(u8ScIndex);
            msWriteByte(SC25_60+u32SCRegBase, (bEnable ? BIT0: 0));
            msWriteByte(SC25_62+u32SCRegBase, u8Pattern&0x0F);
        }
        uwScMask >>=1;
        u8ScIndex ++;
    }
}

/////////////////////////////////////////////////////////////////////////
// OSD Blend Position Control
void msAPIOSDBlendWindowPos(WORD u16HPos, WORD u16YPos)
{
    msDrvOSDBlend_WindowPos(u16HPos, u16YPos);
}

// H, V position %
void msAPIOSDBlendWindowPosRatio(BYTE u16HRatio, WORD u16VRatio)
{
    msDrvOSDBlend_WindowPosRatio(u16HRatio, u16VRatio);
}

// OSD H, V size
void msAPIOSDBlendWindowSize(WORD u16Width, WORD u16Height)
{
    msDrvOSDBlend_WindowSize(u16Width, u16Height);
}

void msAPIOSDBlendWindowReconfig(void)
{
    msDrvOSDBlend_WindowReconfig();
}

void msAPIOSDBlendEnable(BOOL bEnable)
{
    msDrvOSDBlend_Enable(bEnable);
}

void msAPIOSDBlendFBSwitch(BOOL bEnable)
{
    msDrvOSDBlendFBSwitch(bEnable);
}

void msAPIOSDBlendInit(BYTE source)
{
    msDrvOSDBlend_Init(source);
}

#if (ENABLE_MWE)
void msAPIMWEWinConfig(BYTE u8IdxWin, ST_WINDOW_INFO sMWEWin, BOOL bEnableMWE)
{
    BYTE    u8ScalerIdx = 0;
    //DWORD   u32SCRegBase;
    WORD    u16ScalerMask=g_DisplayWindow[u8IdxWin].u16DwScMaskOut;
    ST_WINDOW_INFO sDisplayWin,sMWELeft,sMWERight;
    bool bLeftOnly = FALSE;
    bool bRightOnly = FALSE;
    bool bDisWin2sc = TRUE;
    BYTE u8LeftScaler = 0;
    BYTE u8RightScaler = 0;


    MSSCALERAP_PRINT("\r\n Set MWE(%d)", u8IdxWin);
    MSSCALERAP_PRINT("\r\n MWE x = %d, ", sMWEWin.x);
    MSSCALERAP_PRINT("\r\n MWE y = %d, ", sMWEWin.y);
    MSSCALERAP_PRINT("\r\n MWE width = %d, ", sMWEWin.width);
    MSSCALERAP_PRINT("\r\n MWE hieh = %d, ", sMWEWin.height);

    if(bEnableMWE)
        SET_MWE_FUNC_ENABLE_FLAG();
    else
        CLR_MWE_FUNC_ENABLE_FLAG();

    msAPIWinGetDisplayWin(u8IdxWin, &sDisplayWin);
    if(IsWindowLOnly(u8IdxWin) || IsWindowROnly(u8IdxWin))
    {
        bDisWin2sc = FALSE;
    }

    sMWEWin.x += sDisplayWin.x;
    if(sMWEWin.x > (PANEL_WIDTH/2))
        bRightOnly = TRUE;
    if((sMWEWin.x + sMWEWin.width)<= (PANEL_WIDTH/2))
        bLeftOnly = TRUE;

    sMWEWin.y += sDisplayWin.y;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            if(IsScalerLeftPos(u8ScalerIdx))
                u8LeftScaler = u8ScalerIdx;
            if(IsScalerRightPos(u8ScalerIdx))
                u8RightScaler = u8ScalerIdx;

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

    if(bRightOnly)
    {
        MSSCALERAP_PRINT("\r\n MWE Right Only, scIndx= %d", u8RightScaler);
        sMWERight.x = sMWEWin.x-(PANEL_WIDTH/2);
        sMWERight.x = sMWEWin.x;
        sMWERight.y = sMWEWin.y;
        sMWERight.width= sMWEWin.width;
        sMWERight.height= sMWEWin.height;

        sMWELeft.x = 0;
        sMWELeft.y = 0;
        sMWELeft.width= 0;
        sMWELeft.height= 0;

        msDrvMWEWindowEnable(u8LeftScaler, eMWE_WIN2, FALSE, FALSE);
        msDrvMWEWindowEnable(u8RightScaler, eMWE_WIN2, bEnableMWE, FALSE);
        msDrvMWEWindowConfig(u8RightScaler, eMWE_WIN2, sMWERight);
        if(bDisWin2sc)
            msDrvMWEWindowConfig(u8LeftScaler, eMWE_WIN2, sMWELeft);
    }
    else if(bLeftOnly)
    {
        MSSCALERAP_PRINT("\r\n MWE Left Only, scIndx = %d", u8LeftScaler);
        sMWELeft.x = sMWEWin.x;
        sMWELeft.y = sMWEWin.y;
        sMWELeft.width= sMWEWin.width;
        sMWELeft.height= sMWEWin.height;

        sMWERight.x = 0;
        sMWERight.y = 0;
        sMWERight.width= 0;
        sMWERight.height= 0;

        msDrvMWEWindowEnable(u8LeftScaler, eMWE_WIN2, bEnableMWE, FALSE);
        msDrvMWEWindowEnable(u8RightScaler, eMWE_WIN2, FALSE, FALSE);
        msDrvMWEWindowConfig(u8LeftScaler, eMWE_WIN2, sMWELeft);
        if(bDisWin2sc)
            msDrvMWEWindowConfig(u8RightScaler, eMWE_WIN2, sMWERight);
    }
    else
    {
        MSSCALERAP_PRINT("\r\n MWE Left scIndx= %d, Right scIndex=%d\r\n", u8LeftScaler, u8RightScaler);

        msDrvMWEWindowEnable(u8LeftScaler, eMWE_WIN2, bEnableMWE, FALSE);
        msDrvMWEWindowEnable(u8RightScaler, eMWE_WIN2, bEnableMWE, FALSE);

        sMWELeft.x = sMWEWin.x;
        sMWELeft.y = sMWEWin.y;
        sMWELeft.width= (PANEL_WIDTH/2) - sMWEWin.x;
        sMWELeft.height= sMWEWin.height;
        msDrvMWEWindowConfig(u8LeftScaler, eMWE_WIN2, sMWELeft);

        sMWERight.x = 0;
        sMWERight.y = sMWEWin.y;
        sMWERight.width= sMWEWin.width - sMWELeft.width;
        sMWERight.height= sMWEWin.height;
        msDrvMWEWindowConfig(u8RightScaler, eMWE_WIN2, sMWERight);
    }

}

#endif

BYTE msAPISrcGetDRRFPS(void)
{
    DWORD u32OvsPeriod;
    WORD u16TimeOutCnt;
    BYTE u8Vfreq = 0xFF;

#if CHIP_ID==MST9U4
    BYTE u8OutputFrameDiv = ((msReadByte(REG_LPLL_1C) & 0xF0))   +((msReadByte(REG_LPLL_19) & 0xF0)>>4 ) +  1;
#else
    BYTE u8OutputFrameDiv = ((msReadByte(REG_LPLL_19) & 0xF0)>>4 ) + ((msReadByte(REG_LPLL_1C) & BIT1)<<3) + 1;
#endif

    u16TimeOutCnt = 600;
    while(u16TimeOutCnt!=0)
    {
        u16TimeOutCnt--;
        u32OvsPeriod = msRead3Byte ( REG_LPLL_46 );

        if ( u32OvsPeriod != 0 )
        {
            u8Vfreq = (((DWORD)CRYSTAL_CLOCK * u8OutputFrameDiv) + u32OvsPeriod/2)/u32OvsPeriod ;
            break;
        }
    }

    return u8Vfreq;
}

#if ENABLE_HDMI_DRR
void msAPISrcHDMIDRRChk(void)
{
    BYTE u8SCIdx;

    for( u8SCIdx = 0; u8SCIdx<SC_MAX_NUMS; u8SCIdx++ )
    {
        DWORD u32SCIP1RegBase = SCIP1RegBase(u8SCIdx);

        if((INPUT_IS_HDMI_DRR(MapScaler2Port(u8SCIdx))) && (g_ScalerInfo[u8SCIdx].eSiIPHDMIDRRChk))
        {
            if(u8SCIdx & BIT0)
                msWriteByteMask(u32SCIP1RegBase+SC01_42, BIT0, BIT1|BIT0);
            else
                msWriteByteMask(u32SCIP1RegBase+SC03_42, BIT0, BIT1|BIT0);

            g_ScalerInfo[u8SCIdx].eSiIPHDMIDRRChk = FALSE;
            msDrvIPVsyncINTEnable(DISABLE);
        }
    }
}
#endif

WORD msAPISrcGetHFreqCombo10X(BYTE u8InputPort)
{
    WORD u16PixClk, u16Htt, u16Vtt, u16Hfreq = 0;

    if(INPUT_IS_DISPLAYPORT(u8InputPort))
        msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, &u16Htt, &u16Vtt);
    else if(INPUT_IS_TMDS(u8InputPort))
        u16Htt = msDrvIP1ReadHtotal(MapPort2DetScIndex(u8InputPort))*(HBR2Flag(u8InputPort)?2:1) ;
    else
        u16Htt = 0;

    u16PixClk = msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux);
    if(u16Htt!=0)
    {
        u16Hfreq = ((DWORD)u16PixClk * 100 + u16Htt/2)/u16Htt;
    }
    return u16Hfreq;
}

void msAPISrcForceInputFormat(BYTE u8IdxWin, Bool bForceEn, WORD u16DataFormat)
{
    WORD u16TempData = 0;
    WORD uwScMask = g_DisplayWindow[u8IdxWin].u16DwScMaskIn;
    DWORD u32SCRegBase = 0;
    BYTE u8ScIndex = 0;

    #if (CHIP_ID == MST9U4)
    u16TempData = u16TempData;
    #endif

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            u32SCRegBase = msDrvScGetRegBase(u8ScIndex);//SCIP1RegBase(u8ScIndex);
            if( u8ScIndex & _BIT0 )
            {
                #if (CHIP_ID == MST9U4)
                    if(bForceEn)
                    {
                        if((u16DataFormat == eINPUT_COLOR_422) || (u16DataFormat == eINPUT_COLOR_444))
                        {
                            msWriteByteMask(u32SCRegBase + SC02_1E, BIT5, BIT5);
                            msWriteByteMask(u32SCRegBase + SC02_1E, (u16DataFormat==eINPUT_COLOR_422 ? BIT4 : 0), BIT4);
                        }
                        else
                        {
                            msWriteByteMask(u32SCRegBase + SC02_1E, 0, (BIT5|BIT4));
                        }
                    }
                    else
                    {
                        msWriteByteMask(u32SCRegBase + SC02_1E, 0, (BIT5|BIT4));
                    }
                #else
                    if(bForceEn)
                    {
                        msWrite2ByteMask(u32SCRegBase + SC01_04, u16DataFormat, (BIT15|BIT2|BIT1|BIT0));
                    }
                    else
                    {
                        u16TempData = msRead2Byte(u32SCRegBase + SC01_04);
                        u16TempData &= 0x7FFF;
                        msWrite2Byte(u32SCRegBase + SC01_04, u16TempData);
                    }
                #endif
            }
            else
            {
                #if (CHIP_ID == MST9U4)
                    if(bForceEn)
                    {
                        if((u16DataFormat == eINPUT_COLOR_422) || (u16DataFormat == eINPUT_COLOR_444))
                        {
                            msWriteByteMask(u32SCRegBase + SC04_1E, BIT5, BIT5);
                            msWriteByteMask(u32SCRegBase + SC04_1E, (u16DataFormat==eINPUT_COLOR_422 ? BIT4 : 0), BIT4);
                        }
                        else
                        {
                            msWriteByteMask(u32SCRegBase + SC04_1E, 0, (BIT5|BIT4));
                        }
                    }
                    else
                    {
                        msWriteByteMask(u32SCRegBase + SC04_1E, 0, (BIT5|BIT4));
                    }
                #else
                    if(bForceEn)
                    {
                        msWrite2ByteMask(u32SCRegBase + SC03_04, u16DataFormat, (BIT15|BIT2|BIT1|BIT0));
                    }
                    else
                    {
                        u16TempData = msRead2Byte(u32SCRegBase + SC03_04);
                        u16TempData &= 0x7FFF;
                        msWrite2Byte(u32SCRegBase + SC03_04, u16TempData);
                    }
                #endif
            }
        }
        uwScMask >>=1;
        u8ScIndex ++;
    }
}

#if (CHIP_ID != MST9U4)
void msAPISrcSetPixelRepetition(BYTE u8IdxWin, BYTE u8AssignValue)
{
    WORD uwScMask = g_DisplayWindow[u8IdxWin].u16DwScMaskIn;
    DWORD u32SCRegBase = 0;
    BYTE u8ScIndex = 0;

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            u32SCRegBase = SCIP1RegBase(u8ScIndex);
            if( u8ScIndex & _BIT0 )
            {
                msWriteByteMask(u32SCRegBase + SC01_1A, u8AssignValue, (BIT3|BIT2|BIT1|BIT0));
            }
            else
            {
                msWriteByteMask(u32SCRegBase + SC03_1A, u8AssignValue, (BIT3|BIT2|BIT1|BIT0));
            }
        }
        uwScMask >>=1;
        u8ScIndex ++;
    }
}
#endif

// Source Product Descriptor InfoFrame, PB6, Bit3: Native Color Active
// Return 1 : PB6, Bit3 =1
// Return 0 : PB6, Bit3 =0
BOOL msAPISrcGetPktSPDNativeColorActive(BYTE u8InputPort)
{
    BYTE u8PacketData[8];
    msAPI_combo_IPGetPacketContent((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, COMBO_PACKET_SPD_INFOFRAME, 7, u8PacketData);
    return (u8PacketData[6]&BIT3)?TRUE:FALSE;
}
// Source Product Descriptor InfoFrame, PB6, Bit5: Local Dimming Disable Control
// Return 1 : PB6, Bit5 =1
// Return 0 : PB6, Bit5 =0
BOOL msAPISrcGetPktSPDLocalDimmingDisableControl(BYTE u8InputPort)
{
    BYTE u8PacketData[8];
    msAPI_combo_IPGetPacketContent((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, COMBO_PACKET_SPD_INFOFRAME, 7, u8PacketData);
    return (u8PacketData[6]&BIT5)?TRUE:FALSE;
}

