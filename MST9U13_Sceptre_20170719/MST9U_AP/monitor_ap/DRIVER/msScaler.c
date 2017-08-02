

#define _MS_SCALER_C_
#include "Global.h"
#include "sysinfo2_MST9U.h"
#include "Detect.h"
#include "Adjust.h"
#include "menufunc.h"
#include "Menu.h"
#include "msPQ.h"
#include "appInput.h"
#if ENABLE_HDMI_DRR
#include "Mcu.h"
#endif
#include "Power.h"

#define MSSCALER_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && MSSCALER_DEBUG
#define MSSCALER_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MSSCALER_PRINT(format, ...)
#endif

ST_SCALER_INFO g_ScalerInfo[SC_MAX_NUMS];
const ST_WINDOW_INFO _InitWindow = {0, 0, 0, 0};
WORD g_u16FPLLTuneTimes = 0, g_u16FPLLTuneDelay;
DWORD g_u32FPLLTuneMax, g_u32FPLLTuneMin;
BYTE g_u8SCTimerFlag10ms;
const ST_IMAGE_DE_Offset _InitImgDEOffset = {0, 0, 0, 0};

#if AudioFunc
void mStar_SettingAudioFucntion( BYTE InputPort, BYTE SCDetWin);
#endif

void msDrvScSoftwareReset(BYTE SCDetWin, BYTE ucReset)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);

    msWriteByte(SC00_04 + u32SCRegBase , ucReset);
}

void msDrvScSetScalingFilter(BYTE SCDetWin)
{
//Setting in PQ table with xrule
#if 1 // all remove? PQ set?
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    if( SCDetWin & _BIT0 )
    {
        msWrite2Byte(SC23_16+u32SCRegBase, 0x0303); //H/V y/c scaling filter -Bilinear
    }
    else
    {
        msWrite2Byte(SC23_56+u32SCRegBase, 0x0303); //H/V y/c scaling filter -Bilinear
    }
#else
    SCDetWin = SCDetWin;
#endif
}

void msDrvScSetDefaultFiclk(BYTE SCDetWin)
{
    BYTE u8RegOffset;

    u8RegOffset = IsScalerLeft(SCDetWin) ? ((SCDetWin>>2)*0x20) : (((SCDetWin>>2)*0x20) + 0x40);

    if( SCDetWin & _BIT0 )
        msWriteByteMask(REG_120F65+u8RegOffset, 0x44, 0x77);
    else
        msWriteByteMask(REG_120F64+u8RegOffset, 0x40, 0x70);
}

void msDrvScSetScalingFactor(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    BYTE u8InputPort = MapScaler2Port(SCDetWin);
    BYTE u8WinIndex = MapScaler2Win(SCDetWin);
    DWORD u32HRatioPre, u32HRatioPost, u32VRatioPre, u32VRatioPost, u32HFacInit, u32HFacInitHSD;
    DWORD u32SizeIn, u32SizeOut;
    WORD u16GuardSize, u16WindowShift, u16PackShift;
    BYTE u8RegOffset, u8ScalingType;
    BYTE ucMemFormat = g_DisplayWindow[u8WinIndex].u8DwMemFormat;

    u8ScalingType = 0;

    //----------------------------------------------------------
    // Pre H Down
    u32HRatioPre = 0;
    if( HBR2Flag(u8InputPort) && IsWindowMemBal(u8WinIndex) )
        u32SizeIn = g_DisplayWindow[u8WinIndex].sDwCaptureWin.width/2;
    else
        u32SizeIn = g_DisplayWindow[u8WinIndex].sDwCaptureWin.width;
    if( IsWindowRot90270(u8WinIndex) )
        u32SizeOut = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.height;
    else
        u32SizeOut = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width;

    if( u32SizeIn > u32SizeOut )
    {
        u32HRatioPre = SCALING_RATIO_CB(u32SizeIn, u32SizeOut) | _BIT31;

        u8ScalingType |= SC_SCALING_PRE_H_DOWN;
    }
    else if( u32SizeIn < u32SizeOut )
    {
        MSSCALER_PRINT("Scaling Ratio Error, File:%s, Line:%d, In:%d, Out:%d\r\n", __FILE__, __LINE__, u32SizeIn, u32SizeOut);
    }

    if( SCDetWin & _BIT0 )
        DB_W4B(SC02_08+u32SCRegBase, u32HRatioPre);
    else
        DB_W4B(SC04_08+u32SCRegBase, u32HRatioPre);

    //----------------------------------------------------------
    // Pre V Down w/o Rotate
    u32VRatioPre = 0;
    u32SizeIn = g_DisplayWindow[u8WinIndex].sDwCaptureWin.height;
    if( IsWindowRot90270(u8WinIndex) )
        u32SizeOut = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width;
    else
        u32SizeOut = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.height;

    if( u32SizeIn > u32SizeOut )
    {
        if( 0 )
            u32VRatioPre = SCALING_RATIO_BI(u32SizeIn, u32SizeOut) | (_BIT30|BIT31);
        else
            u32VRatioPre = SCALING_RATIO_CB(u32SizeIn, u32SizeOut) | _BIT31;

        u8ScalingType |= SC_SCALING_PRE_V_DOWN;
    }
    else if( u32SizeIn < u32SizeOut )
    {
        MSSCALER_PRINT("Scaling Ratio Error, File:%s, Line:%d, In:%d, Out:%d\r\n", __FILE__, __LINE__, u32SizeIn, u32SizeOut);
    }

    // FICLK, FCLK
    msWriteByteMask(REG_120F12, 0x00, 0x1F);
    //msWrite2Byte(SC20_24+u32SCRegBase, 0x0001);
    //msWrite2Byte(SC20_26+u32SCRegBase, 0x0801);

    u8RegOffset = IsScalerLeft(SCDetWin) ? ((SCDetWin>>2)*0x20) : (((SCDetWin>>2)*0x20) + 0x40);

    // clear vin_ctrl/cout_ctrl
    msDrvSetScalingPreVCtrlLineIn(SCDetWin, 0,  FALSE);
    msDrvSetScalingPreVCtrlLineOut(SCDetWin,  0,  FALSE);

    if( SCDetWin & _BIT0 )
    {
        DB_W4B(SC02_10+u32SCRegBase, u32VRatioPre);

        if( u32VRatioPre )
            msWriteByteMask(REG_120F65+u8RegOffset, 0x40, 0x77);
        else
            msWriteByteMask(REG_120F65+u8RegOffset, 0x44, 0x77);
    }
    else
    {
        DB_W4B(SC04_10+u32SCRegBase, u32VRatioPre);
        if( u32VRatioPre )
            msWriteByteMask(REG_120F64+u8RegOffset, 0x00, 0x70);
        else
            msWriteByteMask(REG_120F64+u8RegOffset, 0x40, 0x70);
    }

    //for pre-vsd bilinear mode set vin_ctrl/vout_ctrl.
    if(u32VRatioPre & _BIT30)
    {
       msDrvSetScalingPreVCtrlLineIn(SCDetWin, u32SizeIn,  TRUE);
       msDrvSetScalingPreVCtrlLineOut(SCDetWin,  u32SizeOut,  TRUE);
    }

    //----------------------------------------------------------
    // Post H Scaling
    // Scaling Factor
    u32HRatioPost = 0;
    if( g_DisplayWindow[u8WinIndex].bDwFakeCapWinEnable ) // fake cap win is enabled
    {
        if( u8ScalingType & SC_SCALING_PRE_H_DOWN ) // Pre H scaling down
        {
            u32HRatioPre &= ~_BIT31;
            u32SizeIn = (g_DisplayWindow[u8WinIndex].sDwFakeCapWin.width * u32HRatioPre) >> 20;
        }
        else
            u32SizeIn = g_DisplayWindow[u8WinIndex].sDwFakeCapWin.width;
    }
    else
        u32SizeIn = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width;

    if( HBR2Flag(u8InputPort) && IsWindowMemBal(u8WinIndex) )
        u32SizeIn *= 2;

    u32SizeOut = g_DisplayWindow[u8WinIndex].sDwDisplayWin.width;

    if( u32SizeIn != u32SizeOut )
    {
        if( u32SizeIn > u32SizeOut )
            u8ScalingType |= SC_SCALING_POST_H_DOWN;
        else
            u8ScalingType |= SC_SCALING_POST_H_UP;

        u32HRatioPost = SCALING_RATIO_BI(u32SizeIn, u32SizeOut);
        u32HRatioPost = ((u32HRatioPost & 0x0F000000) << 4) | (u32HRatioPost & 0x00FFFFFF);
        u32HRatioPost |= _BIT24;
    }
    else
        u32HRatioPost |= _BIT24|_BIT20;

    if( SCDetWin & _BIT0 )
        DB_W4B(SC23_0E+u32SCRegBase, u32HRatioPost);
    else
        DB_W4B(SC23_4E+u32SCRegBase, u32HRatioPost);

    // Initial Factor
    if( SCDetWin & _BIT0 )
        DB_W4B(SC23_02+u32SCRegBase, 0);
    else
        DB_W4B(SC23_42+u32SCRegBase, 0);

    u32HFacInit = 0;
    u16GuardSize = 0;
    u16WindowShift = 0;
    u16PackShift = 0;
    // R OPM GuardBand
    if( IsWindowRot90(u8WinIndex) )
    {
        if( (ucMemFormat == MEM_FMT_444_12) ) // 7 pix pack
            u16PackShift = (7 - (g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width % 7)) % 7;
        else // 8 pix pack
            u16PackShift = (8 - (g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width % 8)) % 8;
    }

    if( IsScalerRightPos(SCDetWin) )
    {
        u16GuardSize = (SC_GUARD_BAND_PQ * u32SizeIn / u32SizeOut);
        // Start Point of Guardband VOP Before Scaling
        if( g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x > u16GuardSize )
        {
            u16GuardSize = g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x - u16GuardSize;
            g_ScalerInfo[SCDetWin].u8SiGuardBandOut = SC_GUARD_BAND_PQ;
        }
        else
        {
            u16GuardSize = 0;
            g_ScalerInfo[SCDetWin].u8SiGuardBandOut = (g_DisplayWindow[u8WinIndex].sDwDisplayWin.width - g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.width);
        }

        // Start Point of Gaurdband Scaler & Align 64 Round Down
        if( u16GuardSize >= SC_GUARD_BAND )
            u16WindowShift = (u16GuardSize - SC_GUARD_BAND) & 0xFFC0;
        else
            u16WindowShift = 0;

        if( (u32SizeIn != u32SizeOut) && u16GuardSize)
        {
            u32HFacInit = ((DWORD)g_DisplayWindow[u8WinIndex].sDwDisplayWin.width - g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.width - g_ScalerInfo[SCDetWin].u8SiGuardBandOut - g_ScalerInfo[SCDetWin].u16SiLSCImgOffset) * SCALING_RATIO_BI(u32SizeIn, u32SizeOut);
            u16GuardSize = u32HFacInit >> 20;
            u32HFacInit &= 0xFFFFF;
            u16GuardSize += g_ScalerInfo[SCDetWin].u16SiLSCOpmOffset; // opm offset for L SC overscan
        }

        if(SCDetWin & _BIT0)
        {
            if( IsWindowRot90270(u8WinIndex) )
            {
                if( ucMemFormat == MEM_FMT_444_12 )
                    u16PackShift = (u16PackShift + (u16WindowShift % 7));
                else
                    u16PackShift = (u16PackShift + (u16WindowShift % 8));
            }

            DB_WB(SC20_3A+u32SCRegBase, (u16GuardSize-u16WindowShift+u16PackShift));
            if((u16GuardSize-u16WindowShift) > 0xEF)//to avoid over flow
                DB_W2B(SC20_28+u32SCRegBase, 0x30FF);
            else
                DB_W2B(SC20_28+u32SCRegBase, 0x3000|(u16GuardSize-u16WindowShift+16));
        }
        else
            DB_WB(SC20_3B+u32SCRegBase, (u16GuardSize-u16WindowShift));

        // Size Of Guardbnad
        u16GuardSize = g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x - u16WindowShift;
    }
    else
    {
        if(SCDetWin & _BIT0)
        {
            DB_WB(SC20_3A+u32SCRegBase, u16PackShift + g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x - (g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x & 0xFFC0));
            DB_W2B(SC20_28+u32SCRegBase, 0x3030);
        }
        else
            DB_WB(SC20_3B+u32SCRegBase, u16PackShift);
    }

    // R Scaler OPM Guard Band
    if( IsScalerRightPos(SCDetWin) )
    {
        g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Left = u16GuardSize;
        g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Right = 0;
    }
    // L Scaler OPM Guard Band
    else if( g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width != g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width )
    {
        u16GuardSize = (SC_GUARD_BAND_PQ * u32SizeIn / u32SizeOut) + 1;
        u16WindowShift = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width - g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width;
        if( u16GuardSize > u16WindowShift )
        {
            g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Right = u16WindowShift + u16PackShift;
            g_ScalerInfo[SCDetWin].u8SiGuardBandOut = (u16WindowShift * u32SizeOut) / u32SizeIn;
        }
        else
        {
            g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Right = u16GuardSize + u16PackShift;
            g_ScalerInfo[SCDetWin].u8SiGuardBandOut = SC_GUARD_BAND_PQ;
        }

        g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Left = 0;
    }
    // None OPM Guard Band
    else
    {
        g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Left = 0;
        g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Right = 0;

        g_ScalerInfo[SCDetWin].u8SiGuardBandOut = 0;
    }

    if( SCDetWin & _BIT0 )
        DB_W4B(SC23_02+u32SCRegBase, u32HFacInit);
    else
        DB_W4B(SC23_42+u32SCRegBase, u32HFacInit);


    //----------------------------------------------------------
    // HBR2 MIU Balance Mode, L/R Seperated Handle
    u32HFacInitHSD = 0;
    u16PackShift = 0;
    if( HBR2Flag(u8InputPort) && IsWindowMemBal(u8WinIndex) )
    {
        DWORD u32Factor;
#if ENABLE_SCALER_LRSWAP
        if( !IsScalerRight(SCDetWin) )
#else
        if( IsScalerRight(SCDetWin) )
#endif
        {
            u32SizeIn = g_DisplayWindow[u8WinIndex].sDwCaptureWin.width/2;
            u32SizeOut = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width;
            u32HFacInitHSD = ((DWORD)u32SizeIn - DFT_HBR2_GUARDBAND) * SCALING_RATIO_CB(u32SizeIn, u32SizeOut);
            u16GuardSize = u32SizeOut - (u32HFacInitHSD >> 20);
            u32HFacInitHSD &= 0xFFFFF;

            u32SizeIn = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width;
            u32SizeOut = g_DisplayWindow[u8WinIndex].sDwDisplayWin.width/2;
            u32Factor = SCALING_RATIO_BI(u32SizeIn*2, u32SizeOut*2);
            u16PackShift = u32SizeOut - ((u32SizeIn - u16GuardSize) * u32SizeOut / u32SizeIn);
            u32HFacInit = ((DWORD)u32SizeOut - u16PackShift) * u32Factor;
            while( (u32HFacInit >> 20) < (u32SizeIn - u16GuardSize) )
            {
                u32HFacInit += u32Factor;
                u16PackShift--;
            }
            u32HFacInit &= 0xFFFFF;

            if( (u16PackShift > SC_GUARD_BAND_PQ) && g_ScalerInfo[SCDetWin].u8SiFbMode)
            {
                g_ScalerInfo[SCDetWin].u8SiGuardBandOut = SC_GUARD_BAND_PQ;
                u16PackShift -= SC_GUARD_BAND_PQ;
            }
            else
            {
                g_ScalerInfo[SCDetWin].u8SiGuardBandOut = u16PackShift;
                u16PackShift = 0;
            }

            if( SCDetWin & _BIT0 )
                DB_W4B(SC23_02+u32SCRegBase, u32HFacInit);
            else
                DB_W4B(SC23_42+u32SCRegBase, u32HFacInit);
        }
        else
        {
            u32SizeIn = g_DisplayWindow[u8WinIndex].sDwCaptureWin.width/2;
            u32SizeOut = g_DisplayWindow[u8WinIndex].sDwDisplayWin.width/2;
            g_ScalerInfo[SCDetWin].u8SiGuardBandOut = DFT_HBR2_GUARDBAND * u32SizeOut / u32SizeIn;
        }

        u32SizeIn = g_DisplayWindow[u8WinIndex].sDwCaptureWin.width/2;
        u32SizeOut = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width;
        g_ScalerInfo[SCDetWin].u8SiGuardBandOutBalMode = DFT_HBR2_GUARDBAND * u32SizeOut / u32SizeIn;
    }

    if(SCDetWin & _BIT0)
        DB_WB(SC20_38+u32SCRegBase, u16PackShift);
    else
        DB_WB(SC20_39+u32SCRegBase, u16PackShift);

    if( SCDetWin & _BIT0 )
        DB_W4B(SC02_04+u32SCRegBase, u32HFacInitHSD);
    else
        DB_W4B(SC04_04+u32SCRegBase, u32HFacInitHSD);


    //----------------------------------------------------------
    // Post V Scaling
    u32VRatioPost = 0;
    if( g_DisplayWindow[u8WinIndex].bDwFakeCapWinEnable ) // fake cap win is enabled
    {
        if( u8ScalingType & SC_SCALING_PRE_V_DOWN ) // Pre V scaling down
        {
            u32VRatioPre &= ~_BIT31;
            u32SizeIn = (g_DisplayWindow[u8WinIndex].sDwFakeCapWin.height * u32VRatioPre) >> 20;
        }
        else
            u32SizeIn = g_DisplayWindow[u8WinIndex].sDwFakeCapWin.height;
    }
    else
        u32SizeIn = g_ScalerInfo[SCDetWin].sSiSCFMemWindow.height;

    u32SizeOut = g_DisplayWindow[u8WinIndex].sDwDisplayWin.height;

    if( u32SizeIn != u32SizeOut )
    {
        if( u32SizeIn > u32SizeOut )
            u8ScalingType |= SC_SCALING_POST_V_DOWN;
        else
            u8ScalingType |= SC_SCALING_POST_V_UP;

        u32VRatioPost = SCALING_RATIO_BI(u32SizeIn, u32SizeOut) | _BIT24;
    }

    if( SCDetWin & _BIT0 )
        DB_W4B(SC23_12+u32SCRegBase, u32VRatioPost);
    else
        DB_W4B(SC23_52+u32SCRegBase, u32VRatioPost);

    g_ScalerInfo[SCDetWin].u8SiScalingType = (EN_SCALING_TYPE)u8ScalingType;
}

static void msDrvScSetupPathParam(BYTE SCDetWin)
{
#if CHIP_ID == MST9U4
    msDrvIP2Set4PMode(SCDetWin, IP4P_1TO4); // MST9U4_TBD .. refine to port structure
#else
    SCDetWin = SCDetWin;
#endif
}

static void msDrvScSetCaptureWindow(BYTE SCDetWin)
{
    DWORD u32SCIP1RegBase = SCIP1RegBase(SCDetWin);
    BYTE u8WinIndex = MapScaler2Win(SCDetWin);
    BYTE u8InputPort = MapScaler2Port(SCDetWin);
    WORD x = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.x;
    WORD y = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.y;
    WORD width = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.width;
    WORD height = g_ScalerInfo[SCDetWin].sSiIPCaptureWin.height;
    ST_WINDOW_INFO sAutoWin = mStar_ReadAutoWindow(u8InputPort);
    BOOL bIP2CapEnable = g_ScalerInfo[SCDetWin].bSiIP2CapEnable;

    // prevent 1 in 2 out, non-input scaler write
    if( width == 0 )
        y = 0x1FFF;

    if(  sAutoWin.width == 0 )
         sAutoWin.y = 0x1FFF;

    if( InterlaceModeFlag(u8InputPort) && !IsWindowDI(u8WinIndex) )
        height *= 2;

#if CHIP_ID != MST9U4
    if( IsWindowRot90270(u8WinIndex) )
        height += 7;
#endif

    if( HBR2Flag(u8InputPort) && IsWindowMemBal(u8WinIndex) )
        width += DFT_HBR2_GUARDBAND;

    //Green line on window boundary
    //width += 8;
#if (CHIP_ID==MST9U3)
    if( INPUT_IS_HDMI_2_0(u8InputPort) && HBR2Flag(u8InputPort) )
    {
    #if Enable_Auto_Switch_DE_Mode
      if(FreeSyncFlag)
      	{
            if( g_ScalerInfo[SCDetWin].u8SiIPHBR2LR == HBR2_RIGHT )
                 x -= DFT_HBR2_GUARDBAND;
      	}
    #else
    #if !ENABLE_DEONLY_MODE
            if( g_ScalerInfo[SCDetWin].u8SiIPHBR2LR == HBR2_RIGHT )
                 x -= DFT_HBR2_GUARDBAND;
    #endif
    #endif
    }
#endif

    if(bIP2CapEnable)
    {
        if( SCDetWin & BIT0 )   // MAIN
        {
            DB_W2B(SC01_0A+u32SCIP1RegBase, sAutoWin.x);
            DB_W2B(SC01_08+u32SCIP1RegBase, sAutoWin.y);
            DB_W2B(SC01_0C+u32SCIP1RegBase, sAutoWin.height);
            DB_W2B(SC01_0E+u32SCIP1RegBase, sAutoWin.width);
        }
        else
        {
            DB_W2B(SC01_0A+u32SCIP1RegBase, sAutoWin.x);
            DB_W2B(SC01_08+u32SCIP1RegBase, sAutoWin.y);
            DB_W2B(SC01_0C+u32SCIP1RegBase, sAutoWin.height);
            DB_W2B(SC01_0E+u32SCIP1RegBase, sAutoWin.width);
        }

        msDrvIP22ndHCropEn(SCDetWin, ENABLE);
        msDrvIP22ndVCropEn(SCDetWin, ENABLE);
        msDrvIP22ndSetImageHStart(SCDetWin, x-sAutoWin.x);
        msDrvIP22ndSetImageWidth(SCDetWin, width);

        if( InterlaceModeFlag(u8InputPort))
        {
            msDrvIP22ndSetImageVStart(SCDetWin, (y-sAutoWin.y)/2);
            msDrvIP22ndSetImageHeight(SCDetWin, height/2);
        }
        else
        {
            msDrvIP22ndSetImageVStart(SCDetWin, y-sAutoWin.y);
            msDrvIP22ndSetImageHeight(SCDetWin, height);
        }
    }
    else
    {
        if( SCDetWin & BIT0 )   // MAIN
        {
            DB_W2B(SC01_0A+u32SCIP1RegBase, x);
            DB_W2B(SC01_08+u32SCIP1RegBase, y);
            DB_W2B(SC01_0C+u32SCIP1RegBase, height);
            DB_W2B(SC01_0E+u32SCIP1RegBase, width);
        }
        else
        {
            DB_W2B(SC03_0A+u32SCIP1RegBase, x);
            DB_W2B(SC03_08+u32SCIP1RegBase, y);
            DB_W2B(SC03_0C+u32SCIP1RegBase, height);
            DB_W2B(SC03_0E+u32SCIP1RegBase, width);
        }

        msDrvIP22ndHCropEn(SCDetWin, DISABLE);
        msDrvIP22ndVCropEn(SCDetWin, DISABLE);
    }
}

void msDrvScSetFieldDetect(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    DWORD u32SCIP1RegBase = SCIP1RegBase(SCDetWin);
    BYTE IP_PortIndex = MapScaler2Port(SCDetWin);
    BYTE u8RegIP1_10, u8RegIP1_3B, u8RegIP1_42, u8RegIP1_43, u8RegIP1_47;
    WORD u16RegOP1_2223;
    WORD usDPHTotal = 0, usDPVTotal = 0;
    if( INPUT_IS_VD(IP_PortIndex) )
    {
       u8RegIP1_3B = 0x81;
       u8RegIP1_42 = 0x03;
       u8RegIP1_43 = 0xC5;
       u8RegIP1_47 = 0x30;
    }
    else
    {
        if( InterlaceModeFlag(IP_PortIndex) )
        {
            if( INPUT_IS_VGA(IP_PortIndex) )
            {
                u8RegIP1_3B = 0x81;
                u8RegIP1_42 = 0x00;
                u8RegIP1_43 = 0xC1;
                u8RegIP1_47 = 0x00;
            }
            else
            {
                if(INPUT_IS_DISPLAYPORT(IP_PortIndex))
                {
                    u8RegIP1_3B = 0x01;
                    u8RegIP1_42 = 0x3C;
                    u8RegIP1_43 = 0x01;
                    u8RegIP1_47 = 0x00;
                }
                else
                {
                    u8RegIP1_3B = V_IntTorlance;
                    u8RegIP1_42 = 0x00;
                    if(msDrvIP1IsDEOnlyMode(SCDetWin) == TRUE)
                    {
                        if (bHDMI108oiTimingFlag==1)
                            u8RegIP1_43 = 0x01;
                        else
                            u8RegIP1_43 = 0x00;                    }
                    else
                    {
                        u8RegIP1_43 = 0x01;
                    }
                    u8RegIP1_47 = 0x00;
                }
            }
        }
        else
        {
            if( SCDetWin & _BIT0 )  // MAIN
            {
               u8RegIP1_3B = msReadByte(SC01_3B+u32SCIP1RegBase)&0x7F;
               u8RegIP1_42 = msReadByte(SC01_42+u32SCIP1RegBase);
               u8RegIP1_43 = msReadByte(SC01_43+u32SCIP1RegBase)|0xC0;
            }
            else
            {
               u8RegIP1_3B = msReadByte(SC03_3B+u32SCIP1RegBase)&0x7F;
               u8RegIP1_42 = msReadByte(SC03_42+u32SCIP1RegBase);
               u8RegIP1_43 = msReadByte(SC03_43+u32SCIP1RegBase)|0xC0;
            }
            u8RegIP1_47 = 0x00;
        }
    }

#if DP_ASTRO_INTERLACE_PATCH
	 msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[IP_PortIndex].eIPMux, &usDPHTotal, &usDPVTotal);
    if(INPUT_IS_DISPLAYPORT(IP_PortIndex))
    {
    	  if((msAPI_combo_IPGetDPAstorPTGInterlacePatch((EN_COMBO_IP_SELECT)g_InputPort[IP_PortIndex].eIPMux) && ((msDrvIP1ReadSyncStatus(SCDetWin)&INTM_B)||(usDPVTotal > (1200 /2) ))) || msAPI_combo_IPGetDPAstorPTGInterlaceFieldPatch((EN_COMBO_IP_SELECT)g_InputPort[IP_PortIndex].eIPMux))
            u8RegIP1_42 = 0x3F;
        else
            u8RegIP1_42 = 0x3C;
    }
#endif

#if ENABLE_HDMI_DRR
    if((INPUT_IS_HDMI_DRR(IP_PortIndex)))
    {
        u8RegIP1_42 = (u8RegIP1_42|BIT0)&(~BIT1);
        g_ScalerInfo[SCDetWin].eSiIPHDMIDRRChk = FALSE;
        msDrvIPVsyncINTEnable(DISABLE);
    }
    else
        u8RegIP1_42 &= 0xFC;
#endif

    if( InterlaceModeFlag(IP_PortIndex) )
    {
        u8RegIP1_10   = _BIT2;  // Field Time Average
        u16RegOP1_2223 = 0xA001; //[15]: extra request enable, [13]:y-half for interlace, [3:0]:threshold
    }
    else
    {
        u8RegIP1_10 = 0; // Field Time Average
        u16RegOP1_2223 = 0x8001; //[15]: extra request enable, [13]:y-half for interlace, [3:0]:threshold
    }

    if( SCDetWin & _BIT0 )  // MAIN
    {
        msWriteByteMask(SC01_3B+u32SCIP1RegBase, u8RegIP1_3B, 0x9F);
        msWriteByteMask(SC01_42+u32SCIP1RegBase, u8RegIP1_42, 0x03);
        msWriteByte(SC01_43+u32SCIP1RegBase, u8RegIP1_43|0xC0);
        msWriteByte(SC01_47+u32SCIP1RegBase, u8RegIP1_47);
        msWriteByteMask(SC01_10+u32SCIP1RegBase, u8RegIP1_10, _BIT2);
        msWrite2ByteMask(SC20_22+u32SCRegBase, u16RegOP1_2223, 0xA00F);
    }
    else
    {
        msWriteByteMask(SC03_3B+u32SCIP1RegBase, u8RegIP1_3B, 0x9F);
        msWriteByteMask(SC03_42+u32SCIP1RegBase, u8RegIP1_42, 0x03);
        msWriteByte(SC03_43+u32SCIP1RegBase, u8RegIP1_43|0xC0);
        msWriteByte(SC03_47+u32SCIP1RegBase, u8RegIP1_47);
        msWriteByteMask(SC03_10+u32SCIP1RegBase, u8RegIP1_10, _BIT2);
    }
}

static void msDrvScSetMemLRCenterCross(BYTE ucWinIndex)
{
    if( (!IsWindowROnly(ucWinIndex)) && (!IsWindowLOnly(ucWinIndex)) ) // L + R scaler
    {
        g_DisplayWindow[ucWinIndex].bDwLRCross = TRUE;
        if( (g_DisplayWindow[ucWinIndex].sDwDisplayWin.x + (g_DisplayWindow[ucWinIndex].sDwDisplayWin.width/2)) == (PanelWidth/2) )
        {
            g_DisplayWindow[ucWinIndex].bDwLRCenter = TRUE;
        }
        else
        {
            g_DisplayWindow[ucWinIndex].bDwLRCenter = FALSE;
        }
    }
    else
    {
        g_DisplayWindow[ucWinIndex].bDwLRCross = FALSE;
        g_DisplayWindow[ucWinIndex].bDwLRCenter = FALSE;
    }
}

static void msDrvScSetMemLRCapCenter(BYTE ucWinIndex)
{
    //check capture window center
    if( ((g_DisplayWindow[ucWinIndex].sDwCaptureWin.width%2)==0)
      &&(g_DisplayWindow[ucWinIndex].sDwCaptureWin.x+g_DisplayWindow[ucWinIndex].sDwCaptureWin.width/2) == (g_DisplayWindow[ucWinIndex].sDwAutoWin.x+g_DisplayWindow[ucWinIndex].sDwAutoWin.width/2) )
    {
        g_DisplayWindow[ucWinIndex].bDwLRCapCenter = TRUE;
    }
    else
    {
        g_DisplayWindow[ucWinIndex].bDwLRCapCenter = FALSE;
    }
}

static void msDrvScSetMemFormat(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    BYTE ucWinIndex = MapScaler2Win(SCDetWin);
    BYTE ucMemFormat = g_DisplayWindow[ucWinIndex].u8DwMemFormat;
    WORD uwMemFieldMode = g_DisplayWindow[ucWinIndex].u16DwMemFieldMode;
    //BYTE ucMAMode = g_DisplayWindow[ucWinIndex].sDwPQMode.u8MAMode;

    g_ScalerInfo[SCDetWin].u8SiFieldNum = g_DisplayWindow[ucWinIndex].u8DwMemFieldNum;

    MSSCALER_PRINT("[msScaler] SC:%d, Format:%d, Field Num:%d, Mode:%x\r\n", SCDetWin, ucMemFormat, g_ScalerInfo[SCDetWin].u8SiFieldNum, uwMemFieldMode);

    if( uwMemFieldMode == 0x0300 )
    {
        g_ScalerInfo[SCDetWin].fSiMemPixByte = 2;
    }
    else if( ucMemFormat == MEM_FMT_444_10 )
    {
        g_ScalerInfo[SCDetWin].fSiMemPixByte = 4;
    }
    else if( ucMemFormat == MEM_FMT_444_12_YM )
    {
        g_ScalerInfo[SCDetWin].fSiMemPixByte = 1.5;
    }
    else
    {
        g_ScalerInfo[SCDetWin].fSiMemPixByte = 3;
    }

    if( SCDetWin & BIT0 )   // MAIN
    {
        if( ucMemFormat == MEM_FMT_NO_USE )
        {
            msWriteByteMask(SC12_02+u32SCRegBase, _BIT7, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_02+u32SCRegBase, _BIT5, _BIT5|_BIT4); // RGB 444 10 bit format

            msWriteByteMask(SC02_14+u32SCRegBase, 0, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_18+u32SCRegBase, 0, _BIT7);     // 422 -> 444 OFF
        }
        else if( ucMemFormat == MEM_FMT_422 )
        {
            msWriteByteMask(SC12_02+u32SCRegBase, 0, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_02+u32SCRegBase, 0, _BIT5|_BIT4); // 422 format

            msWriteByteMask(SC02_14+u32SCRegBase, _BIT7, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_18+u32SCRegBase, _BIT7, _BIT7); // 422 -> 444
        }
        else if( ucMemFormat == MEM_FMT_444_8 )
        {
            msWriteByteMask(SC12_02+u32SCRegBase, 0, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_02+u32SCRegBase, _BIT4, _BIT5|_BIT4); // 444 10 bit format

            msWriteByteMask(SC02_14+u32SCRegBase, 0, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_18+u32SCRegBase, 0, _BIT7);     // 422 -> 444 OFF
        }
        else if( ucMemFormat == MEM_FMT_444_10 )
        {
            msWriteByteMask(SC12_02+u32SCRegBase, 0, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_02+u32SCRegBase, _BIT5, _BIT5|_BIT4); // 444 10 bit format

            msWriteByteMask(SC02_14+u32SCRegBase, 0, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_18+u32SCRegBase, 0, _BIT7);     // 422 -> 444 OFF
        }
        else if( ucMemFormat == MEM_FMT_444_12 )
        {
            msWriteByteMask(SC12_02+u32SCRegBase, 0, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_02+u32SCRegBase, 0, _BIT5|_BIT4); // 444 10 bit format

            msWriteByteMask(SC02_14+u32SCRegBase, 0, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_18+u32SCRegBase, 0, _BIT7);     // 422 -> 444 OFF
        }
        else if( ucMemFormat == MEM_FMT_444_12_YM )
        {
            msWriteByteMask(SC12_02+u32SCRegBase, 0, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_02+u32SCRegBase, 0, _BIT5|_BIT4); // 444 10 bit format

            msWriteByteMask(SC02_14+u32SCRegBase, 0, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_18+u32SCRegBase, _BIT7, _BIT7);     // 422 -> 444 OFF
        }
    }
    else
    {
        if( ucMemFormat == MEM_FMT_NO_USE )
        {
            msWriteByteMask(SC12_82+u32SCRegBase, _BIT7, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_82+u32SCRegBase, _BIT5, _BIT5|_BIT4); // RGB 444 10 bit format

            msWriteByteMask(SC04_14+u32SCRegBase, 0, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_58+u32SCRegBase, 0, _BIT7);     // 422 -> 444 OFF
        }
        else if( ucMemFormat == MEM_FMT_422 )
        {
            msWriteByteMask(SC12_82+u32SCRegBase, 0, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_82+u32SCRegBase, 0, _BIT5|_BIT4); // 422 format

            msWriteByteMask(SC04_14+u32SCRegBase, _BIT7, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_58+u32SCRegBase, _BIT7, _BIT7); // 422 -> 444
        }
        else if( ucMemFormat == MEM_FMT_444_10 )
        {
            msWriteByteMask(SC12_82+u32SCRegBase, 0, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_82+u32SCRegBase, _BIT5, _BIT5|_BIT4); // 444 10 bit format

            msWriteByteMask(SC04_14+u32SCRegBase, 0, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_58+u32SCRegBase, 0, _BIT7);     // 422 -> 444 OFF
        }
        else if( ucMemFormat == MEM_FMT_444_8 )
        {
            msWriteByteMask(SC12_82+u32SCRegBase, 0, _BIT7); // Main frame bufless
            msWriteByteMask(SC12_82+u32SCRegBase, _BIT4, _BIT5|_BIT4); // 444 10 bit format

            msWriteByteMask(SC04_14+u32SCRegBase, 0, _BIT7); // Enable Sub 444 -> 422 filter
            msWriteByteMask(SC23_58+u32SCRegBase, 0, _BIT7);     // 422 -> 444 OFF
        }
        else if( (ucMemFormat == MEM_FMT_444_12)
              || (ucMemFormat == MEM_FMT_444_12_YM) )
        {
            MSSCALER_PRINT("[msScaler] sub can't support MEM_FMT_444_12 && 12_YM\n");
        }
    }
}

static void msDrvScSetMemFetch(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    BYTE u8InputPort = MapScaler2Port(SCDetWin);
    BYTE u8WinIndex = MapScaler2Win(SCDetWin);
    WORD u8HBR2Guardband;

    if( HBR2Flag(u8InputPort) && IsWindowMemBal(u8WinIndex) )
        u8HBR2Guardband = g_ScalerInfo[SCDetWin].u8SiGuardBandOutBalMode;
    else
        u8HBR2Guardband = 0;

    if(SCDetWin & BIT0)
    {
        if( IsWindowHMirror(u8WinIndex) )
            DB_W2B(SC12_1E+u32SCRegBase, ALIGN_2(g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width + u8HBR2Guardband));   // input fetch number
        else
            DB_W2B(SC12_1E+u32SCRegBase, ALIGN_16(g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width + u8HBR2Guardband));   // input fetch number
        //DB_W2B(SC12_2C+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width);           // output offset
        //DB_W2B(SC12_2E+u32SCRegBase, ALIGN_2(g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width));  // output fetch number

        DB_W2B(SC12_30+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.height|_BIT15);     // input vertical limitation
        DB_W2B(SC20_2A+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.height);            // output vertical limitation

    }
    else // Sub window
    {
        DB_W2B(SC12_9E+u32SCRegBase, ALIGN_16(g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width + u8HBR2Guardband));   // input fetch number
        //DB_W2B(SC12_AC+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width);           // output offset
        //DB_W2B(SC12_AE+u32SCRegBase, ALIGN_2(g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width));  // output fetch number

        DB_W2B(SC12_B0+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.height|_BIT15);     // input vertical limitation
        DB_W2B(SC20_2C+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.height);            // output vertical limitation
    }
}

void msDrvScMemMRQ(BYTE u8ScIndex, BOOL bForceBlock)
{
#if ((CHIP_ID==MST9U2) && ((MEMORY_MAP == MMAP_256_256MB) || (MEMORY_MAP == MMAP_512_512MB)))
    DWORD u32SCRegBase = SCRegBase(u8ScIndex);
    BYTE u8WinIndex = MapScaler2Win(u8ScIndex);
    DWORD u32FBAddrStart;
    DWORD u32FBAddrEnd;

    if(bForceBlock)
    {
        u32FBAddrStart = 0;
        u32FBAddrEnd = 0;
    }
    else if( (IsScalerRight(u8ScIndex) && IsWindowMemBal(u8WinIndex)) || g_DisplayWindow[u8WinIndex].bDwMemForceBase1 )
    {
        u32FBAddrStart = g_DisplayWindow[u8WinIndex].u32DwMemBase[1] & (~BIT31);
        u32FBAddrEnd = u32FBAddrStart + g_DisplayWindow[u8WinIndex].u32DwMemSize[1];
    }
    else
    {
        u32FBAddrStart = g_DisplayWindow[u8WinIndex].u32DwMemBase[0] & (~BIT31);
        u32FBAddrEnd = u32FBAddrStart + g_DisplayWindow[u8WinIndex].u32DwMemSize[0];
    }

    u32FBAddrStart >>= 5;
    u32FBAddrEnd >>= 5;
#else
    DWORD u32SCRegBase = SCRegBase(u8ScIndex);

    DWORD u32FBAddrStart = ((DWORD)FB_L0_ADR)>>5;
#if (CHIP_ID==MST9U3)
    DWORD u32FBAddrEnd = ((DWORD)FB_R0_ADR + FB_R0_LEN)>>5;
#else
    DWORD u32FBAddrEnd = ((DWORD)FB_R1_ADR + FB_R1_LEN)>>5;
#endif

    if(bForceBlock)
    {
        u32FBAddrStart = 0;
        u32FBAddrEnd = 0;
    }
#endif

    if( u8ScIndex & _BIT0 )
    {
        DB_W4B(SC0D_80+u32SCRegBase, u32FBAddrStart);
        DB_W4B(SC0D_84+u32SCRegBase, u32FBAddrEnd);
    }
    else
    {
        DB_W4B(SC0D_88+u32SCRegBase, u32FBAddrStart);
        DB_W4B(SC0D_8C+u32SCRegBase, u32FBAddrEnd);
    }
}

static void msDrvScSetMemMiu(BYTE SCDetWin)
{
#if ((CHIP_ID==MST9U2) && ((MEMORY_MAP == MMAP_256_256MB) || (MEMORY_MAP == MMAP_512_512MB)))
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    BYTE u8WinIndex = MapScaler2Win(SCDetWin);
    DWORD u32MemBase;
    BYTE ucMiu, ucMiuCur;

    if( (IsScalerRight(SCDetWin) && IsWindowMemBal(u8WinIndex)) || g_DisplayWindow[u8WinIndex].bDwMemForceBase1 )
        u32MemBase = g_DisplayWindow[MapScaler2Win(SCDetWin)].u32DwMemBase[1];
    else
        u32MemBase = g_DisplayWindow[MapScaler2Win(SCDetWin)].u32DwMemBase[0];

    if( u32MemBase & _BIT31 )
        ucMiu = 0xFF;
    else
        ucMiu = 0x00;

    if( SCDetWin & _BIT0 )
        ucMiuCur = ((msReadByte(SC12_0A+u32SCRegBase)&_BIT4) ? 0xFF : 0x00);
    else
        ucMiuCur = ((msReadByte(SC12_89+u32SCRegBase)&_BIT6) ? 0xFF : 0x00);

    // opm select by MIU hw mode SC client
    if( ucMiuCur != ucMiu )
    {
        MSSCALER_PRINT("[msScaler] SC:%d, MIU%d\n", SCDetWin, (ucMiu?1:0));

        if( SCDetWin & _BIT0 )
        {
            DB_WBMask(SC20_20+u32SCRegBase, 0, _BIT0);
            DB_WBMask(SC12_0A+u32SCRegBase, ucMiu, _BIT4);
        }
        else
        {
            DB_WBMask(SC20_20+u32SCRegBase, 0, _BIT1);
            DB_WBMask(SC12_89+u32SCRegBase, ucMiu, _BIT6);
        }
    }

#else
    SCDetWin = SCDetWin;
#endif
}

static void msDrvScSetMemConfig(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    BYTE u8InputPort = MapScaler2Port(SCDetWin);
    BYTE ucWinIndex = MapScaler2Win(SCDetWin);
    BYTE u8Rotate = g_ScalerInfo[SCDetWin].u8SiRotate;
    DWORD u32IpmMemBase, u32OpmMemBase, u32OffsetBase, u32MemBase, u32FbSize;
    WORD u16IpmOffset, u16IpmFetch, u16OpmOffset, u16LbSecSize;
    BYTE u8FieldNum, u8GuardSize, u8RegOffset, u8SyncSC, u8HBR2Guardband;
    float fPixByte = g_ScalerInfo[SCDetWin].fSiMemPixByte;
    BYTE ucMemFormat = g_DisplayWindow[ucWinIndex].u8DwMemFormat;
    BYTE u8RWBankMode = 0;
    WORD u16SC5B_0203 = msRead2Byte(SC5B_02+u32SCRegBase);
    WORD u16SC12_0405 = msRead2Byte(SC12_04+u32SCRegBase);
    WORD u16SC12_0607 = msRead2Byte(SC12_06+u32SCRegBase);
    WORD u16SC12_0A0B = msRead2Byte(SC12_0A+u32SCRegBase);
    WORD u16SC12_8485 = msRead2Byte(SC12_84+u32SCRegBase);
    WORD u16SC12_8687 = msRead2Byte(SC12_86+u32SCRegBase);
    WORD u16SC12_8A8B = msRead2Byte(SC12_8A+u32SCRegBase);
    WORD u16SC12_CCCD = msRead2Byte(SC12_CC+u32SCRegBase);
    WORD u16SC0204_5455 = 0;

    if( HBR2Flag(u8InputPort) && IsWindowMemBal(ucWinIndex) )
        u8HBR2Guardband = g_ScalerInfo[SCDetWin].u8SiGuardBandOutBalMode;
    else
        u8HBR2Guardband = 0;

    // Set Sync Scaler
    u8SyncSC = g_ScalerInfo[SCDetWin].u8SiSCFMemSyncSC;
    if( (u8SyncSC != SCDetWin) && (!IsWindowMemBal(ucWinIndex)) )
    {
        u8RegOffset = (((u8SyncSC & _BIT2) >> 2) << 1) | (u8SyncSC & _BIT0);
        u8RegOffset = (IsScalerLeft(u8SyncSC)) ? (u8RegOffset|_BIT3) : (u8RegOffset|_BIT3|_BIT2);
        if( SCDetWin & _BIT0 )
            DB_WB(SC11_0E+u32SCRegBase, u8RegOffset);
        else
            DB_WB(SC11_8E+u32SCRegBase, u8RegOffset);
    }
    else
    {
        if( SCDetWin & _BIT0 )
            DB_WB(SC11_0E+u32SCRegBase, 0x00);
        else
            DB_WB(SC11_8E+u32SCRegBase, 0x00);
    }

    // restore default
    u8RegOffset = (IsScalerLeft(SCDetWin)) ? ((SCDetWin>>2)*0x20) : (((SCDetWin>>2)*0x20) + 0x40);

    u8FieldNum = g_ScalerInfo[SCDetWin].u8SiFieldNum;

    if( ucMemFormat == MEM_FMT_444_12_YM )
        u16SC12_CCCD |= (_BIT15|_BIT14|_BIT13);
    else
        u16SC12_CCCD &= ~(_BIT15|_BIT14|_BIT13);
    DB_W2B(SC12_CC+u32SCRegBase, u16SC12_CCCD);

    msWrite2Byte(SC20_50+u32SCRegBase, 0x0840);

    if(SCDetWin & BIT0) // MAIN
    {
        u16SC12_0607 &= ~(BIT13|BIT12); //msWriteByteMask(SC12_07+u32SCRegBase, 0, _BIT5|_BIT4);
        if( ucMemFormat == MEM_FMT_444_12 )
            u8RWBankMode = 0x20;
        else
            u8RWBankMode = 0x40;
        //DB_WBMask(SC02_55+u32SCRegBase, 0, _BIT7); //u16SC0204_5455

        u16SC12_0405 = 0x0000;
        u16SC12_0607 &= ~(BIT5); //msWriteByteMask(SC12_06+u32SCRegBase, 0, _BIT5);
        DB_WBMask(SC12_32+u32SCRegBase, u8FieldNum, 0x1F);

        u16SC12_0A0B &= ~(_BIT10);

        u16SC5B_0203 &= 0x00FF; //msWriteByte(SC5B_03+u32SCRegBase, 0x00);
        DB_WBMask(SC5B_0E+u32SCRegBase, 0x00, 0x03);

        // FIFO Depth
        DB_WB(SC12_48+u32SCRegBase, 0x40);
        msWrite2Byte(SC12_4A+u32SCRegBase, 0x1818);

        // VBlank Pre Fill
        msWriteByteMask(SC20_21+u32SCRegBase, _BIT2, _BIT2);

        // Start of LB
        //msWrite2Byte(SC20_24+u32SCRegBase, 0x0001);
    }
    else
    {
        if( ucMemFormat == MEM_FMT_444_12 )
            u8RWBankMode = 0x20;
        else
            u8RWBankMode = 0x40;
        //DB_WBMask(SC04_55+u32SCRegBase, 0, _BIT7); //u16SC0204_5455

        u16SC12_8485 = 0x0000;
        u16SC12_8687 &= ~(BIT13|BIT12); //msWriteByteMask(SC12_87+u32SCRegBase, 0, _BIT5|_BIT4);
        DB_WBMask(SC12_B2+u32SCRegBase, u8FieldNum, 0x1F);

        // FIFO Depth
        msWrite2Byte(SC12_C0+u32SCRegBase, 0x1010);
        msWrite2Byte(SC12_C2+u32SCRegBase, 0x1010);
        msWrite2Byte(SC12_C4+u32SCRegBase, 0x1010);
        msWrite2Byte(SC12_CA+u32SCRegBase, 0x1010);

        // VBlank Pre Fill
        msWriteByteMask(SC20_21+u32SCRegBase, _BIT3, _BIT3);

        // Start of LB
      #if MST9U_ASIC_1P
        msWrite2Byte(SC20_26+u32SCRegBase, 0x0A00);
      #else	  
        msWrite2Byte(SC20_26+u32SCRegBase, 0x0840);
      #endif	  
    }
    msWriteByteMask(SC12_E0+u32SCRegBase, _BIT2, _BIT2);

    // 12
    if( (ucMemFormat == MEM_FMT_444_12)
     || (ucMemFormat == MEM_FMT_444_12_YM) )
    {
        if( SCDetWin & _BIT0 )
        {
            msWriteByteMask(SC00_80+u32SCRegBase, _BIT3, _BIT3);
            if( ucMemFormat == MEM_FMT_444_12_YM )
                u16SC12_0405 |= 0x410D;
            else
                u16SC12_0405 |= (BIT10);
            u16SC12_0607 |= (BIT3); //msWriteByteMask(SC12_06+u32SCRegBase, _BIT3, _BIT3);
            //msWrite2Byte(SC12_3C+u32SCRegBase, 0x1900);
            msWrite2Byte(SC12_58+u32SCRegBase, 0x0101);
            msWrite2Byte(SC12_5A+u32SCRegBase, 0x0001);
        }
        /* Sub Can't Support
        else
        {
            msWriteByteMask(SC00_80+u32SCRegBase, _BIT2, _BIT2);
            u16SC12_8485 |= (BIT10);
            u16SC12_8687 |= (BIT3); //msWriteByteMask(SC12_86+u32SCRegBase, _BIT3, _BIT3);
            msWrite2Byte(SC12_BC+u32SCRegBase, 0x1900);
            msWrite2Byte(SC12_D8+u32SCRegBase, 0x0101);
            msWrite2Byte(SC12_DA+u32SCRegBase, 0x0001);
        }
        */
    }
    else
    {
        if( SCDetWin & _BIT0 )
        {
            msWriteByteMask(SC00_80+u32SCRegBase, 0, _BIT3);
            //u16SC12_0405 &= ~(BIT10);
            u16SC12_0607 &= ~(BIT3); //msWriteByteMask(SC12_06+u32SCRegBase, 0, _BIT3);
            msWrite2Byte(SC12_3C+u32SCRegBase, 0x0000);
            msWrite2Byte(SC12_58+u32SCRegBase, 0x1212);
            msWrite2Byte(SC12_5A+u32SCRegBase, 0x0012);
        }
        else
        {
            msWriteByteMask(SC00_80+u32SCRegBase, 0, _BIT2);
            //u16SC12_8485 &= ~(BIT10);
            u16SC12_8687 &= ~(BIT3); //msWriteByteMask(SC12_86+u32SCRegBase, 0, _BIT3);
            msWrite2Byte(SC12_BC+u32SCRegBase, 0x0000);
            msWrite2Byte(SC12_D8+u32SCRegBase, 0x1212);
            msWrite2Byte(SC12_DA+u32SCRegBase, 0x0012);
        }
    }

    // OPM Guard Band Offset
    u8GuardSize = g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Right + g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Left;
    if( IsScalerRightPos(SCDetWin) )
        u32OpmMemBase = (g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x - g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Left) + ALIGN_64(g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width) * g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.y;
    else
        u32OpmMemBase = (g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x & 0xFFC0 ) + ALIGN_64(g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width) * g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.y;

    // IPM Write Off
    if( g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width == 0 )
    {
        u16SC0204_5455 |= BIT14; // ip cut in MST9U3. No effect in MST9U1/2
        msDrvScMemMRQ(SCDetWin, TRUE);
    }
    else
        msDrvScMemMRQ(SCDetWin, FALSE);

    // IPM / OPM Setting
    if( (IsScalerRight(SCDetWin) && IsWindowMemBal(ucWinIndex)) || g_DisplayWindow[ucWinIndex].bDwMemForceBase1 )
        u32MemBase = g_DisplayWindow[ucWinIndex].u32DwMemBase[1];
    else
        u32MemBase = g_DisplayWindow[ucWinIndex].u32DwMemBase[0];
    u32OffsetBase = 0;
    if( !((u8Rotate == ROT_90) || (u8Rotate == ROT_270)) )
    {
        // * 4BYTES * #Field
        u32OpmMemBase = (DWORD)((float)u32OpmMemBase * fPixByte * u8FieldNum);

        // IPM Offset Align
        u16IpmOffset = ALIGN_64(g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width + u8HBR2Guardband);

        // V MIRROR
        if( (u8Rotate == ROT_V_MIRROR) || (u8Rotate == ROT_HV_MIRROR) )
        {
            // ( H * (V-1) )
            u32OffsetBase = ((DWORD)u16IpmOffset * (g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.height - 1) );
            u32OffsetBase = (DWORD)((float)u32OffsetBase * fPixByte * u8FieldNum);
            u32IpmMemBase = u32MemBase + u32OffsetBase;
        }
        else
            u32IpmMemBase = u32MemBase;


        // R Scaler IPM Init Offset, HBR2
        u32OffsetBase = (DWORD)g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.x;
        if( u32OffsetBase )
        {
            if( !((u8Rotate == ROT_H_MIRROR) || (u8Rotate == ROT_HV_MIRROR)) )
            {
                u32OffsetBase = (DWORD)((float)u32OffsetBase * fPixByte * u8FieldNum);
                u32IpmMemBase = u32IpmMemBase + u32OffsetBase;
            }
        }
        // L Scaler IPM Init Offset, HBR2
        else
        {
            u32OffsetBase = (DWORD)(g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width - g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width);
            if( ((u8Rotate == ROT_H_MIRROR) || (u8Rotate == ROT_HV_MIRROR))
             && (u32OffsetBase != 0) )
            {
                u32OffsetBase = (DWORD)((float)u32OffsetBase * fPixByte * u8FieldNum);
                u32IpmMemBase = u32IpmMemBase + u32OffsetBase;
            }
        }

        // H/V Mirror Setting
        if(SCDetWin & BIT0)
        {
            DB_W4B(SC12_10+u32SCRegBase, u32IpmMemBase>>5); // Main window Input Video Base address 0

            if( u8Rotate == ROT_H_MIRROR )
            {
                u16SC12_0607 |= (BIT12); //msWriteByteMask(SC12_07+u32SCRegBase, _BIT4, _BIT4);
                u16SC0204_5455 |= BIT15; //DB_WBMask(SC02_55+u32SCRegBase, _BIT7, _BIT7);
            }
            else if( u8Rotate == ROT_V_MIRROR )
            {
                u16SC12_0607 |= (BIT13); //msWriteByteMask(SC12_07+u32SCRegBase, _BIT5, _BIT5);
            }
            else if( u8Rotate == ROT_HV_MIRROR )
            {
                u16SC12_0607 |= (BIT13|BIT12); //msWriteByteMask(SC12_07+u32SCRegBase, _BIT5|_BIT4, _BIT5|_BIT4);
                u16SC0204_5455 |= BIT15; //DB_WBMask(SC02_55+u32SCRegBase, _BIT7, _BIT7);
            }
        }
        else
        {
            DB_W4B(SC12_90+u32SCRegBase, u32IpmMemBase>>5); // Sub window Input Base address 0

            if( u8Rotate == ROT_H_MIRROR )
            {
                u16SC12_8687 |= (BIT12); //msWriteByteMask(SC12_87+u32SCRegBase, _BIT4, _BIT4);
                u16SC0204_5455 |= BIT15; //DB_WBMask(SC04_55+u32SCRegBase, _BIT7, _BIT7);
            }
            else if( u8Rotate == ROT_V_MIRROR )
            {
                u16SC12_8687 |= (BIT13); //msWriteByteMask(SC12_87+u32SCRegBase, _BIT5, _BIT5);
            }
            else if( u8Rotate == ROT_HV_MIRROR )
            {
                u16SC12_8687 |= (BIT13|BIT12); //msWriteByteMask(SC12_87+u32SCRegBase, _BIT5|_BIT4, _BIT5|_BIT4);
                u16SC0204_5455 |= BIT15; //DB_WBMask(SC04_55+u32SCRegBase, _BIT7, _BIT7);
            }
        }

        if( InterlaceModeFlag(u8InputPort)
         && (   (u8Rotate == ROT_H_MIRROR)
             || (u8Rotate == ROT_V_MIRROR)
             || (u8Rotate == ROT_HV_MIRROR) ) )
            u16SC12_0A0B |= _BIT10;

        // OPM Base Address / Offset / Fetch
        if( IsScalerRightPos(SCDetWin) )
            u16OpmOffset = ALIGN_16(g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width + u8GuardSize + u8HBR2Guardband);
        else
            u16OpmOffset = ALIGN_16(g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width + u8GuardSize + u8HBR2Guardband + g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x -(g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.x & 0xFFC0 ));
        if(SCDetWin & BIT0)
        {
            DB_W2B(SC12_1C+u32SCRegBase, u16IpmOffset);                       // output offset
            DB_W4B(SC12_20+u32SCRegBase, (u32MemBase + u32OpmMemBase)>>5);    // Main window Output Base address 0
            DB_W2B(SC12_2C+u32SCRegBase, u16IpmOffset);                       // output offset
            if( u16IpmOffset > u16OpmOffset )
                DB_W2B(SC12_2E+u32SCRegBase, u16OpmOffset);                   // output fetch number
            else
                DB_W2B(SC12_2E+u32SCRegBase, u16IpmOffset);                   // output fetch number
        }
        else
        {
            DB_W2B(SC12_9C+u32SCRegBase, u16IpmOffset);                       // output offset
            DB_W4B(SC12_A0+u32SCRegBase, (u32MemBase + u32OpmMemBase)>>5);    // Sub window output base address 0
            DB_W2B(SC12_AC+u32SCRegBase, u16IpmOffset);                       // output offset
            if( u16IpmOffset > u16OpmOffset )
                DB_W2B(SC12_AE+u32SCRegBase, u16OpmOffset);                   // output fetch number
            else
                DB_W2B(SC12_AE+u32SCRegBase, u16IpmOffset);                   // output fetch number
        }

        // FMCLK
        msWriteByteMask(REG_120F66+u8RegOffset, 0x04, 0x07);
    }
    else // Rotation 90/270
    {
        if( !((ucMemFormat == MEM_FMT_444_10) || (ucMemFormat == MEM_FMT_444_12)) )
        {
            MSSCALER_PRINT("Rotation Memory Format Error 444_10 or 444_12 only, File:%s, Line:%d\r\n", __FILE__, __LINE__);
        }

        if( (ucMemFormat == MEM_FMT_444_12) && u32OpmMemBase )
            u32OpmMemBase = (u32OpmMemBase / 7) * 32;
        else
            u32OpmMemBase = (DWORD)((float)u32OpmMemBase * fPixByte);

        u16IpmOffset = (DWORD)g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.height;
        if( ucMemFormat == MEM_FMT_444_12 )
            u16IpmOffset = (u16IpmOffset/7) + ((u16IpmOffset%7)?3:2);   // default 2 dummy pack
        else
            u16IpmOffset = (u16IpmOffset/8) + ((u16IpmOffset%8)?2:1);   // default 1 dummy pack

        u16IpmFetch = ALIGN_64(g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width);

        if( ucMemFormat == MEM_FMT_444_12 )
            u16LbSecSize = u16IpmFetch / 14 + 1;
        else
            u16LbSecSize = u16IpmFetch / 16;

        if( u8Rotate == ROT_90 )
        {
            // IPM_V * 4BYTES - pack_pixel + (R In Offset)
#if (CHIP_ID==MST9U2) || (CHIP_ID==MST9U3)
            u32OpmMemBase *= 16;
            u32OffsetBase = (u16IpmOffset * 16) - 1 + (u16IpmOffset * g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.x);
            DB_WBMask(SC5B_0E+u32SCRegBase, 0x03, 0x03);
            if( ucMemFormat == MEM_FMT_444_12 )
                u32OpmMemBase += 0x400;  // 2 dummy pack
            else
                u32OpmMemBase += 0x200;  // 1 dummy pack
#else
            u32OffsetBase = u16IpmOffset - 1 + (u16IpmOffset * g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.x);
            if( ucMemFormat == MEM_FMT_444_12 )
                u32OpmMemBase += 0x40;  // 2 dummy pack
            else
                u32OpmMemBase += 0x20;  // 1 dummy pack
#endif

            u16SC5B_0203 |= BIT9; //msWriteByteMask(SC5B_03+u32SCRegBase, _BIT1, _BIT1);
        }
        else if( u8Rotate == ROT_270 )
        {
            // H:IPM_V * (V:MEM_V - 1) * 4BYTES
#if (CHIP_ID==MST9U2) || (CHIP_ID==MST9U3)
            u32OpmMemBase *= 16;
            if( g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.x )
                u32OffsetBase = u16IpmOffset * 16 * (g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width / 16 - 1);
            else
                u32OffsetBase = u16IpmOffset * 16 * (g_ScalerInfo[SCDetWin].sSiSCFMemWindow.height / 16 - 1);
            DB_WBMask(SC5B_0E+u32SCRegBase, 0x03, 0x03);
#else
            if( g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.x )
                u32OffsetBase = u16IpmOffset * (g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width - 1);
            else
                u32OffsetBase = u16IpmOffset * (g_ScalerInfo[SCDetWin].sSiSCFMemWindow.height - 1);
#endif
        }

        u16SC5B_0203 &= 0xFF00;
        u16SC5B_0203 |= (u16LbSecSize&0x00FF); //msWriteByte(SC5B_02+u32SCRegBase, u16LbSecSize);
        DB_W2B(SC5B_04+u32SCRegBase, u16IpmFetch);
        DB_W2B(SC5B_06+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.height);
        DB_W2B(SC5B_0C+u32SCRegBase, u16IpmOffset);
#if (CHIP_ID==MST9U3)
        DB_W2B(SC5B_08+u32SCRegBase, 0x4040);
#else
        DB_W2B(SC5B_08+u32SCRegBase, 0x0A0A);
#endif

        u32FbSize = (g_DisplayWindow[ucWinIndex].u32DwMemSize[0] / 3) & 0xFFFFFE00;
        DB_W4B(SC5B_10+u32SCRegBase, ((u32MemBase)>>5) + u32OffsetBase);
        DB_W4B(SC5B_20+u32SCRegBase, ((u32MemBase + (u32FbSize*1))>>5) + u32OffsetBase);
        DB_W4B(SC5B_30+u32SCRegBase, ((u32MemBase + (u32FbSize*2))>>5) + u32OffsetBase);

        if( ucMemFormat == MEM_FMT_444_12 )
        {
            u16SC12_0405 = 0x07CC;
            u8RWBankMode = 0x40;
            DB_WBMask(SC12_32+u32SCRegBase, 0x6, 0x1F);
        }
        else
        {
            u16SC12_0405 = 0x03CC;
            u8RWBankMode = 0x20;
            DB_WBMask(SC12_32+u32SCRegBase, 0x3, 0x1F);
        }

        u16SC12_0607 |= (BIT5); //msWriteByteMask(SC12_06+u32SCRegBase, _BIT5, _BIT5);
        DB_W2B(SC12_1C+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width); // input offset
        if( ucMemFormat == MEM_FMT_444_12 )
        {
            DB_W2B(SC12_2C+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width + 14); // output offset + 2 dummy pack size
            DB_W2B(SC12_2E+u32SCRegBase, (((g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width + u8GuardSize)/14+2)*14)); // output fetch number
        }
        else
        {
            DB_W2B(SC12_2C+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFMemWindow.width + 8); // output offset + 1 dummy pack size
            DB_W2B(SC12_2E+u32SCRegBase, ALIGN_8(g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width + u8GuardSize)); // output fetch number
        }
        DB_W4B(SC12_20+u32SCRegBase, (u32MemBase + u32OpmMemBase)>>5);
        DB_W4B(SC12_24+u32SCRegBase, (u32MemBase + u32OpmMemBase + (u32FbSize*1))>>5);
        DB_W4B(SC12_34+u32SCRegBase, (u32MemBase + u32OpmMemBase + (u32FbSize*2))>>5);

        if( InterlaceModeFlag(u8InputPort) )
            u16SC12_0A0B |= _BIT10;

        DB_WB(SC12_48+u32SCRegBase, 0x08);

        // FMCLK
        msWriteByteMask(REG_120F66+u8RegOffset, 0x00, 0x07);

        if( ucMemFormat == MEM_FMT_444_12 )
            u16SC5B_0203 |= (BIT15|BIT10|BIT8); //msWriteByteMask(SC5B_03+u32SCRegBase, _BIT7|_BIT2|_BIT0, _BIT7|_BIT2|_BIT0);
        else
        {
            u16SC5B_0203 &= (~BIT8);
            u16SC5B_0203 |= (BIT15|BIT10); //msWriteByteMask(SC5B_03+u32SCRegBase, _BIT7|_BIT2, _BIT7|_BIT2|_BIT0);
        }
    }

    if(SCDetWin & BIT0)
    {
        DB_W2B(SC02_54+u32SCRegBase,u16SC0204_5455);

        DB_W2B(SC12_04+u32SCRegBase,u16SC12_0405);
        DB_W2B(SC12_06+u32SCRegBase,u16SC12_0607);
        DB_W2B(SC12_0A+u32SCRegBase,u16SC12_0A0B|_BIT7);
        DB_W2B(SC12_0A+u32SCRegBase,u16SC12_0A0B);

        DB_W2B(SC5B_02+u32SCRegBase,u16SC5B_0203);
    }
    else
    {
        DB_W2B(SC04_54+u32SCRegBase,u16SC0204_5455);

        DB_W2B(SC12_84+u32SCRegBase,u16SC12_8485);
        DB_W2B(SC12_86+u32SCRegBase,u16SC12_8687);
        DB_W2B(SC12_8A+u32SCRegBase,u16SC12_8A8B|_BIT7);
        DB_W2B(SC12_8A+u32SCRegBase,u16SC12_8A8B);
    }

    if( g_DisplayWindow[ucWinIndex].bDwReadCurFrame )
        u8RWBankMode = 0;

    if(SCDetWin & BIT0) // MAIN
        DB_WBMask(SC12_0F+u32SCRegBase, u8RWBankMode, 0xE0);
    else
        DB_WBMask(SC12_8F+u32SCRegBase, u8RWBankMode, 0xE0);
}

static void msDrvScSetMemory(BYTE SCDetWin)
{
    msDrvScSetMemFormat(SCDetWin);
    msDrvScSetMemFetch(SCDetWin);
    msDrvScSetMemConfig(SCDetWin);
    msDrvScSetMemMiu(SCDetWin);
}

static void msDvrScSetupPathDump(BYTE SCDetWin)
{
    SCDetWin = SCDetWin;
    MSSCALER_PRINT("[ScalerInfo:%d]\r\n", SCDetWin);
    MSSCALER_PRINT("u8SiDelayLines:%x\r\n", g_ScalerInfo[SCDetWin].u8SiDelayLines);
    MSSCALER_PRINT("u8SiSCFMemSyncSC:%x\r\n", g_ScalerInfo[SCDetWin].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("u8SiScalingType:%x\r\n", g_ScalerInfo[SCDetWin].u8SiScalingType);
    MSSCALER_PRINT("sSiOP2GuardBand.u8Left:%x\r\n", g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Left);
    MSSCALER_PRINT("sSiOP2GuardBand.u8Right:%x\r\n", g_ScalerInfo[SCDetWin].sSiOP2GuardBand.u8Right);
    MSSCALER_PRINT("u8SiGuardBandOut:%x\r\n\r\n", g_ScalerInfo[SCDetWin].u8SiGuardBandOut);
}

static void msDrvScSetColorSpace(BYTE SCDetWin)
{
#if (CSC_MATRIX_SEL == IP_CSC_MATRIX)
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
#endif
    BYTE ucWinIndex = MapScaler2Win(SCDetWin);
    EN_WINDOW_COLOR eWinColorFormat = g_DisplayWindow[ucWinIndex].eWinColorFormat;
    EN_INPUT_COLOR eInputColorFormat = g_DisplayWindow[ucWinIndex].eInputColorFormat;
    BYTE bMemYUV, bIpCsc;

    if( (eInputColorFormat == eINPUT_COLOR_YPBPR) || (eWinColorFormat >= eWIN_COLOR_YUV) )
        bMemYUV = 1;
    else
        bMemYUV = 0;

    if( (eInputColorFormat != eINPUT_COLOR_YPBPR) && bMemYUV )
        bIpCsc = 1;
    else
        bIpCsc = 0;

    g_ScalerInfo[SCDetWin].u8SiIpCSC = bIpCsc;

#if (CSC_MATRIX_SEL == IP_CSC_MATRIX)
    if( SCDetWin & _BIT0 )
        msWriteBit(SC02_80+u32SCRegBase, bIpCsc, _BIT3);
    else
        msWriteBit(SC04_80+u32SCRegBase, bIpCsc, _BIT3);
#endif
}

static void msDrvScSetFilm(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);

    // reg_det_field_win_pixel, OPM
    msWrite2Byte(SC0A_B8+u32SCRegBase, 0x00);
    msWrite2Byte(SC0A_BA+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.width);
    msWrite2Byte(SC0A_BC+u32SCRegBase, 0x00);
    msWrite2Byte(SC0A_BE+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFOpmWindow.height);

    // reg_det_frame_win, IPM
    msWrite2Byte(SC0A_C8+u32SCRegBase, 0x00);
    msWrite2Byte(SC0A_CA+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.width);
    msWrite2Byte(SC0A_CC+u32SCRegBase, 0x00);
    msWrite2Byte(SC0A_CE+u32SCRegBase, g_ScalerInfo[SCDetWin].sSiSCFIpmWindow.height);
}

static void msDrvScSetMono(BYTE SCDetWin)
{
    BYTE ucWinIndex = MapScaler2Win(SCDetWin);
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    DWORD u32IPMuxRegBase = u32SCRegBase - 0x100;

    if( g_DisplayWindow[ucWinIndex].bDwMono )
    {
        if( SCDetWin & _BIT0 )
        {
            if( g_ScalerInfo[SCDetWin].u8SiIPHBR2LR == HBR2_RIGHT )
                msWriteByteMask(u32IPMuxRegBase + 0x22, _BIT6, _BIT6);
            else
                msWriteByteMask(u32IPMuxRegBase + 0x22, 0, _BIT6);
        }
        else
        {
            if( g_ScalerInfo[SCDetWin].u8SiIPHBR2LR == HBR2_RIGHT )
                msWriteByteMask(u32IPMuxRegBase + 0x22, _BIT7, _BIT7);
            else
                msWriteByteMask(u32IPMuxRegBase + 0x22, 0, _BIT7);
        }
        msWriteByteMask(SC10_82+u32SCRegBase, _BIT6, _BIT6);
    }
    else
    {
        if( SCDetWin & _BIT0 )
            msWriteByteMask(u32IPMuxRegBase + 0x22, 0, _BIT6);
        else
            msWriteByteMask(u32IPMuxRegBase + 0x22, 0, _BIT7);

        msWriteByteMask(SC10_82+u32SCRegBase, 0, _BIT6);
    }
}

void msDrvScSetupPath(BYTE SCDetWin)
{
    msDrvScSetupPathParam(SCDetWin);

    msDrvScSetCaptureWindow(SCDetWin);
    msDrvScSetFieldDetect(SCDetWin);

    msDrvScSetScalingFactor(SCDetWin);
    msDrvScSetScalingFilter(SCDetWin);

    msDrvScSetMemory(SCDetWin);

    msDrvScSetFilm(SCDetWin);

    msDrvScSetColorSpace(SCDetWin);
    msDrvScSetMono(SCDetWin);

    msDvrScSetupPathDump(SCDetWin);
}

static void msDrvScSetOutputSyncControl(BYTE ucSCIndex)
{
    DWORD u32SCRegBase = SCRegBase(ucSCIndex);

    if( g_ScalerInfo[ucSCIndex].u8SiFbMode )
    {
        msWriteByteMask(SC10_21+u32SCRegBase, BIT6, _BIT6|_BIT1);
    }
    else
    {
        msWriteByteMask(SC10_21+u32SCRegBase, BIT6|BIT1, _BIT6|_BIT1);
    }
}

void msDrvScSetupPathInit(void)
{
    BYTE enScalerIndex;

    for( enScalerIndex=SCL0_SUB ; enScalerIndex < SC_MAX_NUMS ; enScalerIndex++ )
    {
        g_ScalerInfo[enScalerIndex].u8SiScIndex          = enScalerIndex;
#if (CHIP_ID==MST9U3)
        g_ScalerInfo[enScalerIndex].u32SiRegBase         = ((enScalerIndex%SCL0_OFFLINE)<SCR0_SUB)?BANK_SC_M_L0:BANK_SC_S_R0;
#else
        g_ScalerInfo[enScalerIndex].u32SiRegBase         = ((enScalerIndex%SCL0_OFFLINE)<SCR0_SUB)?BANK_SC_M_L0:\
                                                           ((enScalerIndex%SCL0_OFFLINE)<SCL1_SUB)?BANK_SC_S_R0:\
                                                           ((enScalerIndex%SCL0_OFFLINE)<SCR1_SUB)?BANK_SC_S_L1:BANK_SC_S_R1;
#endif
        g_ScalerInfo[enScalerIndex].eSiDispStatus        = SC_STATUS_IDLE;
        g_ScalerInfo[enScalerIndex].u8SiWinIndex         = DISPLAY_MAX_NUMS;
        g_ScalerInfo[enScalerIndex].u8SiFbMode           = TRUE;
        g_ScalerInfo[enScalerIndex].u8SiSyncMode         = FLM_INIT;
        g_ScalerInfo[enScalerIndex].u8SiSCFlags          = bFreeRunModeBit;
        g_ScalerInfo[enScalerIndex].u8SiIPPortIndex      = Input_Nums;

        // mem limit
        msDrvScMemMRQ(enScalerIndex, FALSE);
        msDrvScSetOutputSyncControl(enScalerIndex);

        if( enScalerIndex & _BIT0 )
            msWriteByteMask(SC0D_70+SCRegBase(enScalerIndex), 0x03, 0x03);
        else
            msWriteByteMask(SC20_3E+SCRegBase(enScalerIndex), _BIT4, _BIT4);
    }
}

/////////////////////////////////////////////////////////////////////////
//////////////////////////// clear scaler status  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScStatusClr( WORD u16SCMask, BOOL bFreeRun )
{
    BYTE u8SCIndex;

    for( u8SCIndex=SCL0_SUB ; u8SCIndex < SC_MAX_NUMS ; u8SCIndex++ )
    {
        if( (u16SCMask & (BIT0<<u8SCIndex)) )
        {
            g_ScalerInfo[u8SCIndex].eSiDispStatus   = SC_STATUS_IDLE;
            g_ScalerInfo[u8SCIndex].sSiIPCaptureWin = _InitWindow;
            g_ScalerInfo[u8SCIndex].sSiOP2DisplayWin= _InitWindow;
            g_ScalerInfo[u8SCIndex].u8SiSCFMemSyncSC= u8SCIndex;
            g_ScalerInfo[u8SCIndex].sSiSCFMemWindow = _InitWindow;
            g_ScalerInfo[u8SCIndex].sSiSCFIpmWindow = _InitWindow;
            g_ScalerInfo[u8SCIndex].sSiSCFOpmWindow = _InitWindow;
            g_ScalerInfo[u8SCIndex].u8SiIPHBR2LR    = HBR2_LEFT;
            g_ScalerInfo[u8SCIndex].u8SiWinIndex = DISPLAY_MAX_NUMS;
            g_ScalerInfo[u8SCIndex].u8SiGuardBandOut= 0;
            g_ScalerInfo[u8SCIndex].sSiOP2GuardBand.u8Left  = 0;
            g_ScalerInfo[u8SCIndex].sSiOP2GuardBand.u8Right = 0;
            if(bFreeRun)
            {
                msDrvIP1SetOutputFreerun(TRUE, u8SCIndex);
            }
        }
    }

#if UseINT
    mStar_EnableModeChangeINT(FALSE, u16SCMask);
#endif
}

/////////////////////////////////////////////////////////////////////////
///////////////////////// check if output multi-win ///////////////////////////
/////////////////////////////////////////////////////////////////////////
BOOL msDrvCheckMultiWin( void )
{
    BYTE    ucWinIndex;
    BYTE    ucWinCount = 0;

    for( ucWinIndex=DISPLAY_WIN1; ucWinIndex < DISPLAY_MAX_NUMS; ucWinIndex++ )
    {
        if(g_DisplayWindow[ucWinIndex].u8DwEnable == ENABLE)
        {
            ucWinCount++;
        }
    }

    if(ucWinCount>1)
        return TRUE;
    else
        return FALSE;
}

/////////////////////////////////////////////////////////////////////////
//////////////////// scaler source pre-dispatch LR for win  /////////////////////
/////////////////////////////////////////////////////////////////////////
BOOL msDrvScPreDispatchLR( BYTE ucWinIndexStart, BYTE ucWinIndexMax, WORD uwPreScMask )
{
    BYTE ucWinIndex;
    BYTE ucSCIndexL = SCL0_MAIN;
    BYTE ucSCIndexR = SCR0_MAIN;
    BOOL bRtn = TRUE; // dispatch success or not

    for( ucWinIndex = ucWinIndexStart; ucWinIndex <= ucWinIndexMax; ucWinIndex++ )
    {
        if(g_DisplayWindow[ucWinIndex].u8DwEnable == ENABLE)
        {
            if( (!IsWindowROnly(ucWinIndex)) && (!IsWindowLOnly(ucWinIndex)) ) // L + R scaler
            {
                ucSCIndexL = msDrvScGetIdleScalerL();
                ucSCIndexR = msDrvScGetIdleScalerR();

                if( (ucSCIndexL >= (SC_MAX_NUMS-1)) || (ucSCIndexR >= (SC_MAX_NUMS-1))
                 || (IsWindowRot90270(ucWinIndex)&&((ucSCIndexL & ucSCIndexR & BIT0) == 0)) )
                {
                    bRtn = FALSE;
                    break;
                }
                msDrvScPreAttach(ucSCIndexL, ucWinIndex);
                msDrvScPreAttach(ucSCIndexR, ucWinIndex);
#if MST9U_ASIC_1P2SC
                g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width  = (PANEL_WIDTH/2) - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x;
                g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.x      = (PANEL_WIDTH/2);
                g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.width -= ((PANEL_WIDTH/2) - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x);
#endif
                if(!(uwPreScMask & (BIT0<<ucSCIndexL)))
                {
                    msDrvSrcAttach(g_DisplayWindow[ucWinIndex].u8DwPortIndex, ucSCIndexL);
                }
                if(!(uwPreScMask & (BIT0<<ucSCIndexR)))
                {
                    msDrvSrcAttach(g_DisplayWindow[ucWinIndex].u8DwPortIndex, ucSCIndexR);
                }

            }
            msDrvScSetMemLRCenterCross(ucWinIndex);
        }
    }

    return bRtn;
}

/////////////////////////////////////////////////////////////////////////
////////////////// scaler source pre-dispatch L or R for win  ////////////////////
/////////////////////////////////////////////////////////////////////////
BOOL msDrvScPreDispatchLorR( BYTE ucWinIndexStart, BYTE ucWinIndexMax, WORD uwPreScMask )
{
    BYTE ucWinIndex;
    BYTE ucSCIndexL = SCL0_MAIN;
    BYTE ucSCIndexR = SCR0_MAIN;
    BOOL bRtn = TRUE; // dispatch success or not

    for( ucWinIndex = ucWinIndexStart; ucWinIndex <= ucWinIndexMax; ucWinIndex++ )
    {
        if(g_DisplayWindow[ucWinIndex].u8DwEnable == ENABLE)
        {
            if(IsWindowROnly(ucWinIndex)) // R scaler
            {
                ucSCIndexR = msDrvScGetIdleScalerR();

                if( (ucSCIndexR >= (SC_MAX_NUMS-1))
                 || (IsWindowRot90270(ucWinIndex)&&((ucSCIndexR & BIT0) == 0)) )
                {
                    bRtn = FALSE;
                    break;
                }

                msDrvScPreAttach(ucSCIndexR, ucWinIndex);
                if(!(uwPreScMask & (BIT0<<ucSCIndexR)))
                {
                    msDrvSrcAttach(g_DisplayWindow[ucWinIndex].u8DwPortIndex, ucSCIndexR);
                }
            }
            else if(IsWindowLOnly(ucWinIndex)) // L scaler
            {
                ucSCIndexL = msDrvScGetIdleScalerL();

                if( (ucSCIndexL >= (SC_MAX_NUMS-1))
                 || (IsWindowRot90270(ucWinIndex)&&((ucSCIndexL & BIT0) == 0)) )
                {
                    bRtn = FALSE;
                    break;
                }

                msDrvScPreAttach(ucSCIndexL, ucWinIndex);
                if(!(uwPreScMask & (BIT0<<ucSCIndexL)))
                {
                    msDrvSrcAttach(g_DisplayWindow[ucWinIndex].u8DwPortIndex, ucSCIndexL);
                }

            }
        }
    }
    return bRtn;
}

/////////////////////////////////////////////////////////////////////////
//////////////////// scaler source pre-dispatch 1P for win  //////////////////////
/////////////////////////////////////////////////////////////////////////
BOOL msDrvScPreDispatch1P( BYTE ucWinIndexStart, BYTE ucWinIndexMax, WORD uwPreScMask )
{
    BYTE ucWinIndex;
    BYTE ucSCIndexL = SCL0_MAIN;
    BOOL bRtn = TRUE; // dispatch success or not

    for( ucWinIndex = ucWinIndexStart; ucWinIndex <= ucWinIndexMax; ucWinIndex++ )
    {
        if(g_DisplayWindow[ucWinIndex].u8DwEnable == ENABLE)
        {
            ucSCIndexL = msDrvScGetIdleScalerL();

            if( (ucSCIndexL >= (SC_MAX_NUMS-1))
             || (IsWindowRot90270(ucWinIndex)&&((ucSCIndexL & BIT0) == 0)) )
            {
                bRtn = FALSE;
                break;
            }

            msDrvScPreAttach(ucSCIndexL, ucWinIndex);
            if(!(uwPreScMask & (BIT0<<ucSCIndexL)))
            {
                msDrvSrcAttach(g_DisplayWindow[ucWinIndex].u8DwPortIndex, ucSCIndexL);
            }
        }
    }
    return bRtn;
}
/////////////////////////////////////////////////////////////////////////
///////////////////// scaler source pre-dispatch for win  //////////////////////
/////////////////////////////////////////////////////////////////////////
BOOL msDrvScPreDispatch( BYTE ucWinIndex, WORD uwPreScMask )
{
    BYTE    ucWinIndexStart= DISPLAY_WIN1;
    BYTE    ucWinIndexMax = DISPLAY_MAX_NUMS-1;
    BOOL    bRtn = TRUE; // dispatch success or not

#if 0 // now always 2p
    BOOL    bSCOutputDualMode = FALSE; // FALSE: 1P, TRUE: 2P
    bSCOutputDualMode = msDrvScCheckOutputDualMode(); // FALSE: 1P, TRUE: 2P
#endif

    if(ucWinIndex != DISPLAY_MAX_NUMS)
    {
        ucWinIndexStart = ucWinIndex;
        ucWinIndexMax   = ucWinIndex;
    }

    for( ucWinIndex = ucWinIndexStart; ucWinIndex <= ucWinIndexMax; ucWinIndex++ )
    {
        g_DisplayWindow[ucWinIndex].bDwForce1P2ScOP = FALSE;
    }

#if defined(MST9U_FPGA_1P) || ((MST9U_ASIC_1P==1) && (MST9U_ASIC_1P2SC==0)) // FPGA 1 win only for 1 sc sof/ASIC 1P
    // check L only
    bRtn &= msDrvScPreDispatch1P(ucWinIndexStart, ucWinIndexMax, uwPreScMask);
#else
#if MST9U_ASIC_1P2SC
    if(!msDrvCheckMultiWin()) // multi-win OFF
    {
        bRtn &= msDrvScPreDispatch1P(ucWinIndexStart, ucWinIndexMax, uwPreScMask);
    }
    else
#endif
    {
        // check L+R first, for memory allocation over 2Kx2K case (use L + R memory)
        bRtn &= msDrvScPreDispatchLR(ucWinIndexStart, ucWinIndexMax, uwPreScMask);
        // check L or R only
        bRtn &= msDrvScPreDispatchLorR(ucWinIndexStart, ucWinIndexMax, uwPreScMask);
    }
#endif

    MSSCALER_PRINT("\n[msScaler] Scaler PreDispatch Result(1:ok, 0:fail) : %d\n", bRtn);

    return bRtn;
}

/////////////////////////////////////////////////////////////////////////
////////// attach window to scaler, L/R sc implies L/R side of window display ////////
/////////////////////////////////////////////////////////////////////////
void msDrvScPreAttach( BYTE ucSCIndex, BYTE ucWinIndex )
{
    if(g_DisplayWindow[ucWinIndex].u16DwScMaskOut == 0) // not assigned yet
    {
        g_DisplayWindow[ucWinIndex].u16DwScMaskOut   = BIT0<<ucSCIndex;
    }
    else
    {
        g_DisplayWindow[ucWinIndex].u16DwScMaskOut  |= (BIT0<<ucSCIndex);
    }

    g_ScalerInfo[ucSCIndex].u8SiWinIndex        = ucWinIndex;
    g_ScalerInfo[ucSCIndex].eSiDispStatus       = SC_STATUS_FREERUN;
    g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin    = g_DisplayWindow[ucWinIndex].sDwDisplayWin;
    g_ScalerInfo[ucSCIndex].u8SiRotate          = g_DisplayWindow[ucWinIndex].u8DWRotate;

#if !defined(MST9U_FPGA_1P) && (MST9U_ASIC_1P==0)    // refine DisplayWin range
    if(IsWindowROnly(ucWinIndex)) // R scaler
    {
        g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin.x         -= (PANEL_WIDTH/2);
    }
    else if(IsWindowLOnly(ucWinIndex)) // L scaler
    {
        // none
    }
    else // L + R scaler
    {
#if ENABLE_SCALER_LRSWAP
        if(IsScalerRight(ucSCIndex)) // Left
#else
        if(IsScalerLeft(ucSCIndex)) // Left
#endif
        {
            g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin.width  = (PANEL_WIDTH/2) - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x;
        }
        else // right
        {
            g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin.x      = 0;
            g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin.width -= ((PANEL_WIDTH/2) - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x);
        }
    }
#endif
}

/////////////////////////////////////////////////////////////////////////
////////////////////////////// set scaler rotate mode ///////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScSetRotateMode( WORD uwScMask, BYTE ucRotate)
{
    BYTE    u8SiScIndex = 0;

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            g_ScalerInfo[u8SiScIndex].u8SiRotate = ucRotate;
        }

        uwScMask >>=1;
        u8SiScIndex ++;
    }
}

/////////////////////////////////////////////////////////////////////////
///// Scaler reduce check on 1 win, if 2 SC ==> 1SC, return remained scaler  ///////
/////////////////////////////////////////////////////////////////////////
BYTE msDrvScPostWinUpdate2To1( BYTE ucSCIndex1,  BYTE ucSCIndex2 )
{
    BYTE    ucWinIndex   = MapScaler2Win(ucSCIndex1);
    BYTE    ucInputPort  = MapScaler2Port(ucSCIndex1);
    BYTE    ucSCIndexDel = SC_MAX_NUMS;
    BYTE    ucSCIndexRmn = SC_MAX_NUMS;

#if MST9U_ASIC_1P2SC
    if( (!msDrvCheckMultiWin() && ((ucSCIndex1 & (BIT1|BIT0))==BIT0))// main sc & L
       ||(msDrvCheckMultiWin() && (IsWindowLOnly(ucWinIndex))) )
#elif ENABLE_SCALER_LRSWAP
    if( (IsScalerRight(ucSCIndex1) && (IsWindowLOnly(ucWinIndex)))
      ||(IsScalerLeft(ucSCIndex1) && (IsWindowROnly(ucWinIndex))) )
#else
    if( (IsScalerLeft(ucSCIndex1) && (IsWindowLOnly(ucWinIndex)))
      ||(IsScalerRight(ucSCIndex1) && (IsWindowROnly(ucWinIndex))) )
#endif
    {
        ucSCIndexRmn = ucSCIndex1;
        ucSCIndexDel = ucSCIndex2;
    }
    else
    {
        ucSCIndexRmn = ucSCIndex2;
        ucSCIndexDel = ucSCIndex1;
    }

    msDrvScStatusClr(BIT0<<ucSCIndexDel, TRUE);

    if(g_DisplayWindow[ucWinIndex].u16DwScMaskIn & (BIT0<<ucSCIndexDel))
        g_DisplayWindow[ucWinIndex].u16DwScMaskIn -= (BIT0<<ucSCIndexDel);

    g_DisplayWindow[ucWinIndex].u16DwScMaskOut -= (BIT0<<ucSCIndexDel);
    msDrvSrcDetach(ucInputPort, ucSCIndexDel);

    return ucSCIndexRmn;
}

/////////////////////////////////////////////////////////////////////////
/////// Scaler added check on 1 win, if 1 SC ==> 2SC, return added scaler  /////////
/////////////////////////////////////////////////////////////////////////
BYTE msDrvScPostWinUpdate1To2( BYTE ucSCIndex1)
{
    BYTE    ucWinIndex   = MapScaler2Win(ucSCIndex1);
    BYTE    ucInputPort  = MapScaler2Port(ucSCIndex1);
    BYTE    ucSCIndexAdd = SC_MAX_NUMS;

#if ENABLE_SCALER_LRSWAP
    if(IsScalerRight(ucSCIndex1))
#else
    if(IsScalerLeft(ucSCIndex1))
#endif
    {
        ucSCIndexAdd = msDrvScGetIdleScalerR();
    }
    else
    {
        ucSCIndexAdd = msDrvScGetIdleScalerL();
    }

    if( (ucSCIndexAdd >= (SC_MAX_NUMS-1))
     || (IsWindowRot90270(ucWinIndex)&&((ucSCIndexAdd & BIT0) == 0)) )
    {
        ucSCIndexAdd = SC_MAX_NUMS;
    }

    if(ucSCIndexAdd < (SC_MAX_NUMS-1))
    {
        msDrvScPreAttach(ucSCIndexAdd, ucWinIndex);
        msDrvSrcAttach(ucInputPort, ucSCIndexAdd);
    }

    MSSCALER_PRINT(">> Scaler ReduceUpdate2 Index(Must<%d) :%d \n",SCL0_OFFLINE, ucSCIndexAdd);

    return ucSCIndexAdd;
}

#if ENABLE_SUBSC_HOLLOW_OUT
static void msDrvDelHollowSC( void )
{
    BYTE    u8SCIndex, u8Step=2;

    for( u8SCIndex=SCL0_SUB; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
    {
        if(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_HOLLOW)
        {
            g_ScalerInfo[u8SCIndex].eSiDispStatus = SC_STATUS_IDLE;
            g_ScalerInfo[u8SCIndex].sSiOP2DisplayWin.width = 0;
            g_ScalerInfo[u8SCIndex].u8SiWinIndex = DISPLAY_MAX_NUMS;
            msDrvSetTimingGen(u8SCIndex);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
//////// bL1Top, bR1Top configs the window display order - SCL0_00_80[1:0] ////////
//////////////////////////////////////////////////////////////////////////
void msDrvSubSCHollowOut( BOOL bL1Top, BOOL bR1Top )
{
    BYTE i,j,u8StartOffset; // u8StartOffset: related to L0/L1, R0/R1 Top/Buttom order
    BYTE u8BtmSCM, u8BtmSCS, u8TopSCM, u8TopSCS;
    ST_WINDOW_INFO sBTM; // Bottom Scaler Range
    ST_WINDOW_INFO sTOP; // Top Scaler Range
    ST_WINDOW_INFO sOLP; // Overlap Range

    msDrvDelHollowSC();//Release hollow scaler

    //i=0-> Check L0_Main
    //i=1-> Check R0_Main
    for(i=0;i<2;i++)
    {
        u8StartOffset = (i?bR1Top:bL1Top)?0x00:0x04;
        u8BtmSCM = (SCL0_MAIN+i*2+u8StartOffset)%(SC_MAX_NUMS-1);
        u8BtmSCS = u8BtmSCM-1;
        // Check L0_Main/R0_Main & Its Sub Invalid & Main Rotate
        if( (g_ScalerInfo[u8BtmSCM].eSiDispStatus == SC_STATUS_IDLE)
          ||(g_ScalerInfo[u8BtmSCM].eSiDispStatus == SC_STATUS_FREERUN)
          ||(g_ScalerInfo[u8BtmSCM].u8SiRotate == ROT_90) // Rorate 90
          ||(g_ScalerInfo[u8BtmSCM].u8SiRotate == ROT_270) // Rotate 270
          ||(g_ScalerInfo[u8BtmSCS].eSiDispStatus != SC_STATUS_IDLE) ) // Sub Invalid for Hollow
        {
            continue;
        }

        sBTM = g_ScalerInfo[u8BtmSCM].sSiOP2DisplayWin;
        u8TopSCM = (SCL1_MAIN+i*2+u8StartOffset)%(SC_MAX_NUMS-1);
        u8TopSCS = u8TopSCM-1;
        // check Overlap with L1_Main_Sub, R1_Main_Sub
        for(j=u8TopSCS;j<=u8TopSCM;j++)
        {
            sTOP = g_ScalerInfo[j].sSiOP2DisplayWin;

            // Check Top Valid & Overlap
            if( (g_ScalerInfo[j].eSiDispStatus == SC_STATUS_IDLE) // Top Scaler Not Active
              ||(g_ScalerInfo[j].sSiOP2DisplayWin.width == 0) // Top Scaler Not Active
              ||(sBTM.x > (sTOP.x+sTOP.width-1)) || (sTOP.x > (sBTM.x+sBTM.width-1)) //no overlap
              ||(sTOP.y > (sBTM.y+sBTM.height-1)) || (sBTM.y > (sTOP.y+sTOP.height-1)) ) //no overlap
            {
                continue;
            }

            //Find out overlap area
            sOLP.x     = (sBTM.x  <= sTOP.x )?sTOP.x:sBTM.x;
            sOLP.width = ((sTOP.x+sTOP.width) <= (sBTM.x+sBTM.width))?(sTOP.x+sTOP.width-sOLP.x):(sBTM.x+sBTM.width-sOLP.x);
            sOLP.y     = (sBTM.y  <= sTOP.y )?sTOP.y:sBTM.y;
            sOLP.height= ((sTOP.y+sTOP.height) <= (sBTM.y+sBTM.height))?(sTOP.y+sTOP.height-sOLP.y):(sBTM.y+sBTM.height-sOLP.y);

#if (CHIP_ID==MST9U) || (CHIP_ID==MST9U2)
            //Prevent from sub_H_end too close to Main_H_end
            if(sOLP.width <= 32)
            {
                continue;
            }
            sOLP.width -= 32;
#endif

            g_ScalerInfo[u8BtmSCS].eSiDispStatus       = SC_STATUS_HOLLOW;
            g_ScalerInfo[u8BtmSCS].sSiOP2DisplayWin    = sOLP;
            msDrvSetTimingGen(u8BtmSCS);
        }
    }
}
#endif
/////////////////////////////////////////////////////////////////////////
/////////////////////// Get idle scaler index for INPUT ////////////////////////
/////////////////////////////////////////////////////////////////////////
BYTE msDrvScGetIdleScalerM( void )
{
    BYTE    u8SCIndex, u8Step=2;

#if (MST9U_ASIC_1P==1)
    u8Step = 4; // skip R
#endif

    // M
    for( u8SCIndex=SCL0_MAIN; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
    {
        if(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_IDLE)
        {
            break;
        }
    }

    if( u8SCIndex>=(SC_MAX_NUMS-1) )
    {
        u8SCIndex = SC_MAX_NUMS-1;
    }

    return u8SCIndex;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////// Get idle scaler index for INPUT ////////////////////////
/////////////////////////////////////////////////////////////////////////
#if ENABLE_SUBSCIN
BYTE msDrvScGetIdleScalerS( void )
{
    SCHAR    u8SCIndex, u8Step=2, u8SCInvStart = 0;

#if (MST9U_ASIC_1P==1)&&(CHIP_ID!=MST9U3)
    u8Step = 4; // skip R
    u8SCInvStart = SCL1_SUB;
#endif

    // S
#if (ENABLE_SUBSC_HOLLOW_OUT)
    u8SCInvStart=SCR1_SUB;
    for( u8SCIndex=u8SCInvStart; u8SCIndex >= SCL0_SUB ; u8SCIndex-=u8Step )
#else
    u8SCInvStart = u8SCInvStart;
    for( u8SCIndex=SCL0_SUB; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
#endif
    {
        if( (g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_IDLE)
          ||(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_HOLLOW) )
        {
            return u8SCIndex;
        }
    }

    if( (u8SCIndex>=(SC_MAX_NUMS-1)) || (u8SCIndex<0) )
    {
        u8SCIndex = SC_MAX_NUMS-1;
    }

    return u8SCIndex;
}

BYTE msDrvScGetIdleScalerSS( BYTE ucSCIndex ) // Get Sub scaler of ucSCIndex(Main). Self-Sub scaler
{
    BYTE u8RtnSCIdx = SC_MAX_NUMS-1;

    if( (ucSCIndex & BIT0) // Main scaler
      &&( (g_ScalerInfo[ucSCIndex-1].eSiDispStatus == SC_STATUS_IDLE)
         ||(g_ScalerInfo[ucSCIndex-1].eSiDispStatus == SC_STATUS_HOLLOW)) )
    {
        u8RtnSCIdx = ucSCIndex-1;
    }

    return u8RtnSCIdx;
}
#endif
/////////////////////////////////////////////////////////////////////////
//////////////////// Get idle L scaler index for OUTPUT////////////////////////
/////////////////////////////////////////////////////////////////////////
static BYTE msDrvScGetIdleScalerPhyL(void)
{
    SCHAR    u8SCIndex;
    BYTE     u8Step = 4;

#if MST9U_ASIC_1P2SC
    u8Step = 8;
#endif

    // M
    for( u8SCIndex=SCL0_MAIN; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
    {
        if(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_IDLE)
        {
            return u8SCIndex;
        }
    }
#if ENABLE_SUBSCOUT
    // S
  #if (ENABLE_SUBSC_HOLLOW_OUT)
    for( u8SCIndex=SCL1_SUB; u8SCIndex >= SCL0_SUB ; u8SCIndex-=u8Step )
  #else
    for( u8SCIndex=SCL0_SUB; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
  #endif
    {
        if( (g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_IDLE)
          ||(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_HOLLOW) )

        {
            return u8SCIndex;
        }
    }
#endif

    if( (u8SCIndex>=(SC_MAX_NUMS-1)) || (u8SCIndex<0) )
    {
        u8SCIndex = SC_MAX_NUMS-1;
    }

    return u8SCIndex;
}

static BYTE msDrvScGetIdleScalerPhyR(void)
{
    SCHAR    u8SCIndex;
    BYTE     u8Step = 4;

#if MST9U_ASIC_1P2SC
    u8Step = 8;
#endif

    // M
  #if MST9U_ASIC_1P2SC
    for( u8SCIndex=SCL1_MAIN; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
  #else
    for( u8SCIndex=SCR0_MAIN; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
  #endif
    {
        if(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_IDLE)
        {
            return u8SCIndex;
        }
    }

#if ENABLE_SUBSCOUT
    // S
  #if MST9U_ASIC_1P2SC
    for( u8SCIndex=SCL1_SUB; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
  #elif (ENABLE_SUBSC_HOLLOW_OUT)
    for( u8SCIndex=SCR1_SUB; u8SCIndex >= SCR0_SUB ; u8SCIndex-=u8Step )
  #else
    for( u8SCIndex=SCR0_SUB; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=u8Step )
  #endif
    {
        if( (g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_IDLE)
          ||(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_HOLLOW) )
        {
            return u8SCIndex;
        }
    }
#endif

    if( (u8SCIndex>=(SC_MAX_NUMS-1)) || (u8SCIndex<0) )
    {
        u8SCIndex = SC_MAX_NUMS-1;
    }

    return u8SCIndex;
}

BYTE msDrvScGetIdleScalerL( void )
{
#if ENABLE_SCALER_LRSWAP
    return msDrvScGetIdleScalerPhyR();
#else
    return msDrvScGetIdleScalerPhyL();
#endif
}

/////////////////////////////////////////////////////////////////////////
////////////////////// Get idle R scaler index for OUTPUT /////////////////////
/////////////////////////////////////////////////////////////////////////
BYTE msDrvScGetIdleScalerR( void )
{
#if ENABLE_SCALER_LRSWAP
    return msDrvScGetIdleScalerPhyL();
#else
    return msDrvScGetIdleScalerPhyR();
#endif
}


/////////////////////////////////////////////////////////////////////////
////////////////// Get idle L Main scaler index for OUTPUT//////////////////////
/////////////////////////////////////////////////////////////////////////
BYTE msDrvScGetIdleScalerLM( void )
{
    BYTE    u8SCIndex;

    // M
    for( u8SCIndex=SCL0_MAIN; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=4 )
    {
        if(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_IDLE)
        {
            return u8SCIndex;
        }
    }

    if( u8SCIndex>=(SC_MAX_NUMS-1) )
    {
        u8SCIndex = SC_MAX_NUMS-1;
    }

    return u8SCIndex;
}

/////////////////////////////////////////////////////////////////////////
//////////////////// Get idle R Main scaler index for OUTPUT ///////////////////
/////////////////////////////////////////////////////////////////////////
BYTE msDrvScGetIdleScalerRM( void )
{
    BYTE    u8SCIndex;

    // M
    for( u8SCIndex=SCR0_MAIN; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=4 )
    {
        if(g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_IDLE)
        {
            return u8SCIndex;
        }
    }

    if( u8SCIndex>=(SC_MAX_NUMS-1) )
    {
        u8SCIndex = SC_MAX_NUMS-1;
    }

    return u8SCIndex;
}

/////////////////////////////////////////////////////////////////////////
////////////////// Get IP Stable Scaler index from u16SCMask  /////////////////
/////////////////////////////////////////////////////////////////////////
BYTE msDrvScGetIPStableScaler( WORD u16SCMask )
{
    BYTE    u8SCIndex;

    for( u8SCIndex=SCL0_SUB; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=1 )
    {
        if( (BIT0<<u8SCIndex) & u16SCMask )
        {
            if(g_ScalerInfo[u8SCIndex].u8SiIPRstCntr==0)
            {
                break;
            }
        }
    }

    return u8SCIndex;
}

/////////////////////////////////////////////////////////////////////////
////////////////// Get Scaler Reg Base, especially for Lib  //////////////////////
/////////////////////////////////////////////////////////////////////////
DWORD msDrvScGetRegBase( BYTE ucSCIndex )
{
    return SCRegBase(ucSCIndex);
}

/////////////////////////////////////////////////////////////////////////
////////////////////// PostDispatch Scaler Window Init   //////////////////////
/////////////////////////////////////////////////////////////////////////
static void msDrvScPostMemWinInit( BYTE ucSCIndex )
{
    g_ScalerInfo[ucSCIndex].sSiIPCaptureWin = _InitWindow;
    g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin= _InitWindow;
    g_ScalerInfo[ucSCIndex].sSiSCFMemWindow = _InitWindow;
    g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow = _InitWindow;
    g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow = _InitWindow;
}

/////////////////////////////////////////////////////////////////////////
////////////PostDispatch mem window HBR2 force using 2SC individually // ////////
/////////////////////////////////////////////////////////////////////////
void msDrvScPostMemWinCalHBR2Force2SC( BYTE ucSCIndexL, BYTE ucSCIndexR, ST_WINDOW_INFO sAutoWindow)
{
    BYTE ucWinIndex = MapScaler2Win(ucSCIndexL);
    WORD uwWinCapAlign, uwWinDispAlign, uwInOnlySCMask=0;

    uwInOnlySCMask  = g_DisplayWindow[ucWinIndex].u16DwScMaskOut ^ g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    uwInOnlySCMask &= g_DisplayWindow[ucWinIndex].u16DwScMaskIn;

#if 1
    if( uwInOnlySCMask )
    {
        msDrvScStatusClr(uwInOnlySCMask, TRUE);
        g_DisplayWindow[ucWinIndex].u16DwScMaskIn -= (uwInOnlySCMask);
        msDrvSrcDetach(MapWin2Port(ucWinIndex), GetLsbBitPos(uwInOnlySCMask));
        uwInOnlySCMask = 0;
        ucSCIndexR = SC_MAX_NUMS;
    }

    // Get 2nd scaler for HBR2
    if( ucSCIndexR!=SC_MAX_NUMS ) // original OUTPUT R scaler valid, L+R window
    {
        //sub sc need refined if non-rotate <--> rotate
    }
    else if( IsWindowLOnly(ucWinIndex) )// L only window
    {
        ucSCIndexR = msDrvScGetIdleScalerL();
    }
    else // R only window
    {
        ucSCIndexR = msDrvScGetIdleScalerR();
    }
#else
    // Release in-only sub scaler if changed from non-Rotate to Rotate.
    if((uwInOnlySCMask&SCALER_MASK_SUB) && IsWindowRot90270(ucWinIndex))
    {
        msDrvSrcInOnlySCUpdate(MapWin2Port(ucWinIndex), ucWinIndex);
        uwInOnlySCMask = 0;
    }

    // need check both ENABLE_SUBSCIN & ENABLE_SUBSCOUT for HBR2 sub scaler
#if !(ENABLE_SUBSCIN && ENABLE_SUBSCOUT)
    if( (uwInOnlySCMask&SCALER_MASK_SUB)||((ucSCIndexR!=SC_MAX_NUMS)&&(!(ucSCIndexR&_BIT0))) )
    {
        if(uwInOnlySCMask&SCALER_MASK_SUB)
        {
            ucSCIndexR = GetLsbBitPos(uwInOnlySCMask);
        }
        msDrvScStatusClr((BIT0<<ucSCIndexR));
        g_DisplayWindow[ucWinIndex].u16DwScMaskIn -= (((BIT0<<ucSCIndexR)));
        msDrvSrcDetach(MapWin2Port(ucWinIndex), ucSCIndexR);
        uwInOnlySCMask = 0;
        ucSCIndexR = SC_MAX_NUMS;
    }
#endif

    // Get 2nd scaler for HBR2
    if( ucSCIndexR!=SC_MAX_NUMS ) // original OUTPUT R scaler valid
    {
        //sub sc need refined if non-rotate <--> rotate
    }
    else if( uwInOnlySCMask )
    {
        ucSCIndexR = GetLsbBitPos(uwInOnlySCMask);
    }
    else
#if ENABLE_SUBSCIN && ENABLE_SUBSCOUT
    if( !IsWindowRot90270(ucWinIndex))
    {
        ucSCIndexR = msDrvScGetIdleScalerS();
    else
    }
#endif
    {
        ucSCIndexR = msDrvScGetIdleScalerM();
    }
#endif
    MSSCALER_PRINT(">> Scaler PostDispatch Index(Must<%d) :%d \n",SCL0_OFFLINE, ucSCIndexR);

    msDrvScPostMemWinInit(ucSCIndexL);
    msDrvScPostMemWinInit(ucSCIndexR);

    g_DisplayWindow[ucWinIndex].u16DwScMaskIn   |= ((BIT0<<ucSCIndexL)|(BIT0<<ucSCIndexR));
    g_DisplayWindow[ucWinIndex].u16DwScMaskOut  |= ((BIT0<<ucSCIndexL)|(BIT0<<ucSCIndexR));

    g_ScalerInfo[ucSCIndexR].u8SiWinIndex        = ucWinIndex;
    g_ScalerInfo[ucSCIndexR].eSiDispStatus       = SC_STATUS_WINDOW_SETUP;
    g_ScalerInfo[ucSCIndexR].u8SiRotate          = g_DisplayWindow[ucWinIndex].u8DWRotate;

    // need refine... if no need to remove .. R at the beginning
    if( !((g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR == HBR2_RIGHT) && (MapScaler2Port(ucSCIndexR)==MapWin2Port(ucWinIndex))) ) // prevent setup again
    {
        g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR        = HBR2_RIGHT;
        msDrvSrcAttach(g_DisplayWindow[ucWinIndex].u8DwPortIndex, ucSCIndexR);
    }

    // Set Capture, Disp window , win should be initially 0
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.x      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.x;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.y      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width/2;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.x      = sAutoWindow.x;
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.y      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y;
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width/2;
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.x;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.y;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width/2;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;

    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.x      = 0;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.y      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.y;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width/2;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;


    // set sync scaler
    g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexL;
    g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexL;


    //64*1sc align
#if 0
    if(IsWindowRot90270(ucWinIndex))
    {
        uwWinCapAlign  = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width & 0xFFC0);

        //uwWinDispAlign = ALIGN_128(g_DisplayWindow[ucWinIndex].sDwDisplayWin.height);
        uwWinDispAlign = (g_DisplayWindow[ucWinIndex].sDwDisplayWin.height & 0xFFC0);
    }
    else
#endif
    {
        uwWinCapAlign  = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width & 0xFF80);
        uwWinDispAlign = (g_DisplayWindow[ucWinIndex].sDwDisplayWin.width & 0xFF80);
    }
#if 0
    if(IsWindowRot90270(ucWinIndex))
    {
        // H
        if( uwWinCapAlign > uwWinDispAlign ) // H down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width)*uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = uwWinDispAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = uwWinDispAlign;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width)*uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = uwWinCapAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = uwWinCapAlign;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
        }

        // V
#if (CHIP_ID==MST9U2) || (CHIP_ID==MST9U3)
        if(g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.width) // V down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width)*g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        else
#endif
        // V up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width)*g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
    }
    else // no rotate
#endif
    {
        // H
        if( uwWinCapAlign > uwWinDispAlign ) // H down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinDispAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width); // scaling ratio?;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;//uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;//((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width)*uwWinDispAlign;

            //g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width = uwWinDispAlign - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            //g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width = uwWinDispAlign;
            //g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            //g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x     = 0;
            //g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x     = 0;
            //g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            //g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinCapAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width); // scaling ratio?;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;//uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;//((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width)*uwWinCapAlign;

            //g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width = uwWinCapAlign - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            //g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width = uwWinCapAlign;
            //g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            //g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x     = 0;
            //g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x     = 0;
            //g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            //g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        // V
        if( (g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.height)
          &&(!(   IsWindowDI(ucWinIndex)
               && (g_DisplayWindow[ucWinIndex].u16DwScMaskIn & bSC_MAIN)
        ))) // V down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;

            //g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            //g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            //g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
        }
        else // V up or 1:1 or DI_post_v_down(force HBR2 mode)
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

            //g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            //g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            //g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
        }
    }
    g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow;
    g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow;
    g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow;


    MSSCALER_PRINT("\n[msScaler] >> 2:2 (Balance:1)\n");
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexL, g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height);
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexR, g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n\n",g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height);
}

////////////////////////////////////////////////////////////////////////////
////////////PostDispatch mem window refine for Rot 2SC IP case // ////////////////
////////////////////////////////////////////////////////////////////////////
void msDrvScPostMemWinCalRefineRot2ScIP( BYTE ucWinIndex )
{
    WORD uwSCMask   = g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    BYTE ucSCIndexL, ucSCIndexR=SC_MAX_NUMS;
    BYTE ucPortIndex = MapWin2Port(ucWinIndex);

    ucSCIndexL = GetLsbBitPos(uwSCMask);
    ucSCIndexL = g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC;
    uwSCMask  -= (BIT0<<ucSCIndexL);
    if(uwSCMask)
    {
        ucSCIndexR = GetLsbBitPos(uwSCMask);
    }

    if(ucSCIndexR == SC_MAX_NUMS) // In no R scaler
    {
        uwSCMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut - (BIT0<<ucSCIndexL);
        if(uwSCMask)
        {
            ucSCIndexR = GetLsbBitPos(uwSCMask);
        }

        if(ucSCIndexR == SC_MAX_NUMS) // Out no R scaler
        {
            if(IsWindowForceRot2ScOP(ucWinIndex)) // Need Main SC in the same side.
            {
                if(IsWindowLOnly(ucWinIndex))
                    ucSCIndexR = msDrvScGetIdleScalerLM();
                else// if(IsWindowROnly(ucWinIndex))
                    ucSCIndexR = msDrvScGetIdleScalerRM();
            }
            else
            {
                ucSCIndexR = msDrvScGetIdleScalerM();
            }

            g_ScalerInfo[ucSCIndexR].u8SiWinIndex   = ucWinIndex;
            g_ScalerInfo[ucSCIndexR].eSiDispStatus  = SC_STATUS_WINDOW_SETUP;
            g_ScalerInfo[ucSCIndexR].u8SiRotate     = g_DisplayWindow[ucWinIndex].u8DWRotate;
            msDrvScPostMemWinInit(ucSCIndexR);
            MSSCALER_PRINT(">> Scaler PostDispatch Index(Must<%d) :%d \n",SCL0_OFFLINE, ucSCIndexR);
        }

        msDrvSrcAttach(ucPortIndex, ucSCIndexR);
    }

    g_DisplayWindow[ucWinIndex].u16DwScMaskIn |= (BIT0<<ucSCIndexR);

    // Set Capture, Disp window , win should be initially 0
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.x      = g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.x + ((g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width/2) & 0xFFC0);
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.y      = g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.y;
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.width  = g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width - ((g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width/2) & 0xFFC0);
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.height = g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.height;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.x      = g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.x;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.y      = g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.y;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width  = (g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width/2) & 0xFFC0;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.height = g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.height;

    // set sync scaler
    g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexL;
    g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexL;

    // set Mem window
    g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width;
    g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height;

    // set Ipm window
    g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x + ((g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width/2) & 0xFFC0);
    g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width - ((g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width/2) & 0xFFC0);
    g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height;
    g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x;
    g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = (g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width/2) & 0xFFC0;
    g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height;

    MSSCALER_PRINT("\n[msScaler] >> RefineRot2ScIP\n");
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexL, g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height);
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexR, g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n\n",g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height);

}

////////////////////////////////////////////////////////////////////////////
////////////PostDispatch mem window refine for Rot 2SC OP case // ////////////////
////////////////////////////////////////////////////////////////////////////
void msDrvScPostMemWinCalRefineRot2ScOP( BYTE ucWinIndex )
{
    WORD uwSCMask   = g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    BYTE ucSCIndexL, ucSCIndexR=SC_MAX_NUMS;
    BYTE ucInputPort = MapWin2Port(ucWinIndex);

    ucSCIndexL = GetLsbBitPos(uwSCMask);
    uwSCMask  -= (BIT0<<ucSCIndexL);
    if(uwSCMask)
    {
        ucSCIndexR = GetLsbBitPos(uwSCMask);
    }

    if(ucSCIndexR == SC_MAX_NUMS) // no R scaler
    {
        if(IsWindowLOnly(ucWinIndex) && (msDrvScGetIdleScalerLM()<(SC_MAX_NUMS-1)))
            ucSCIndexR = msDrvScGetIdleScalerLM();
        else if(IsWindowROnly(ucWinIndex) && (msDrvScGetIdleScalerRM()<(SC_MAX_NUMS-1)))
            ucSCIndexR = msDrvScGetIdleScalerRM();
        else
        {
            MSSCALER_PRINT("\n\n\n\n>> Scaler PostDispatch Refine Rot2ScOP Index Fail \n\n\n\n");
            return;
        }

        g_ScalerInfo[ucSCIndexR].u8SiWinIndex        = ucWinIndex;
        g_ScalerInfo[ucSCIndexR].eSiDispStatus       = SC_STATUS_WINDOW_SETUP;
        g_ScalerInfo[ucSCIndexR].u8SiRotate          = g_DisplayWindow[ucWinIndex].u8DWRotate;
        msDrvScPostMemWinInit(ucSCIndexR);

        g_ScalerInfo[ucSCIndexR].u8SiIPPortIndex = ucInputPort;
        g_InputPort[ucInputPort].u16IPScMask |= (BIT0<<ucSCIndexR);
    }

    g_DisplayWindow[ucWinIndex].u16DwScMaskOut |= (BIT0<<ucSCIndexR);

    // Set Capture, Disp window , win should be initially 0
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.x      = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x + g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/2;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.y      = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.width  = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width - g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/2;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.height = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height;

    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x      = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y      = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width  = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/2;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height;

    // set sync scaler
    g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexL;
    g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexL;

    // set Mem window
    g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width;
    g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height;

    // set Opm window
    g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x + g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width/2;
    g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width/2;
    g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height;
    g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x;
    g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width/2;
    g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height;

    MSSCALER_PRINT("\n[msScaler] >> RefineRot2ScOP\n");
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexL, g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height);
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexR, g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n\n",g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height);

}

////////////////////////////////////////////////////////////////////////////
/////////////PostDispatch mem window refine for 1P 2SC OP case // ////////////////
////////////////////////////////////////////////////////////////////////////
void msDrvScPostMemWinCalRefine1P2ScOP( BYTE ucWinIndex )
{
    WORD uwSCMask   = g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    BYTE ucSCIndexL, ucSCIndexR=SC_MAX_NUMS;
    BYTE ucInputPort = MapWin2Port(ucWinIndex);

//    BYTE ucTemp;

    ucSCIndexL = GetLsbBitPos(uwSCMask);
    uwSCMask  -= (BIT0<<ucSCIndexL);
    if(uwSCMask)
    {
        ucSCIndexR = GetLsbBitPos(uwSCMask);
    }

//    if( !(ucSCIndexL&BIT0) && (ucSCIndexR&BIT0) && (ucSCIndexR!=SC_MAX_NUMS) ) // Main as L when HBR2 with M+S
//    {
//        ucTemp = ucSCIndexL;
//        ucSCIndexL = ucSCIndexR;
//        ucSCIndexR = ucTemp;
//    }

    if(ucSCIndexR == SC_MAX_NUMS) // no R scaler
    {
        if(/*(IsWindowRot90270(ucWinIndex)) && */(msDrvScGetIdleScalerLM()<(SC_MAX_NUMS-1)))
            ucSCIndexR = msDrvScGetIdleScalerLM();
        //else if((!IsWindowRot90270(ucWinIndex)) && (msDrvScGetIdleScalerSS(ucSCIndexL)<(SC_MAX_NUMS-1)))
        //    ucSCIndexR = msDrvScGetIdleScalerSS(ucSCIndexL);
        else
        {
            MSSCALER_PRINT("\n\n\n\n>> Scaler PostDispatch Refine 1P2ScOP Index Fail \n\n\n\n");
            return;
        }

        g_ScalerInfo[ucSCIndexR].u8SiWinIndex        = ucWinIndex;
        g_ScalerInfo[ucSCIndexR].eSiDispStatus       = SC_STATUS_WINDOW_SETUP;
        g_ScalerInfo[ucSCIndexR].u8SiRotate          = g_DisplayWindow[ucWinIndex].u8DWRotate;
        msDrvScPostMemWinInit(ucSCIndexR);

        g_ScalerInfo[ucSCIndexR].u8SiIPPortIndex = ucInputPort;
        g_InputPort[ucInputPort].u16IPScMask |= (BIT0<<ucSCIndexR);
    }

    g_DisplayWindow[ucWinIndex].u16DwScMaskOut |= (BIT0<<ucSCIndexR);

    // Set Capture, Disp window , win should be initially 0
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.x      = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x + g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/2;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.y      = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.width  = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width - g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/2;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.height = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height;

    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x      = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y      = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width  = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width/2;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height;

    // set sync scaler
    g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexL;
    g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexL;

    // set Mem window
    g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width;
    g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height;

    // set Opm window
    g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x + g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width/2;
    g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width/2;
    g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height;
    g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x;
    g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width/2;
    g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height;

    MSSCALER_PRINT("\n[msScaler] >> Refine1P2ScOP\n");
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexL, g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height);
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexR, g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n\n",g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height);

}

/////////////////////////////////////////////////////////////////////////
///////////PostDispatch mem window calculation on case 1 ==> 1 ////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScPostMemWinCal1To1( BYTE ucSCIndex )
{
    BYTE ucWinIndex = MapScaler2Win(ucSCIndex);
    WORD uwWinCapAlign, uwWinDispAlign;

    g_DisplayWindow[ucWinIndex].u16DwScMaskIn |= (BIT0<<ucSCIndex);

    msDrvScPostMemWinInit(ucSCIndex);

    g_ScalerInfo[ucSCIndex].u8SiSCFMemSyncSC        = ucSCIndex;
    g_ScalerInfo[ucSCIndex].sSiIPCaptureWin    = g_DisplayWindow[ucWinIndex].sDwCaptureWin;
    g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin   = g_DisplayWindow[ucWinIndex].sDwDisplayWin;

#if (!MST9U_ASIC_1P)
    if(g_DisplayWindow[ucWinIndex].sDwDisplayWin.x >= (PanelWidth/2)) // R scaler
    {
        g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin.x -= (PanelWidth/2);
    }
#endif

    //64*1sc align for rotate case
    if(IsWindowRot90270(ucWinIndex))
    {
#if (CHIP_ID==MST9U || CHIP_ID==MST9U2)
        if( IsWindowRot270(ucWinIndex)
        ||
          ( (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width > NUM_ROT_LB)
          &&(g_DisplayWindow[ucWinIndex].sDwDisplayWin.height > NUM_ROT_LB)
#if ENABLE_ROT_2SCIP
          &&(g_DisplayWindow[ucWinIndex].sDwCaptureWin.width > g_DisplayWindow[ucWinIndex].sDwDisplayWin.height)
#endif
          )
        )
        {
            uwWinCapAlign  = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width & 0xFFC0);
            uwWinDispAlign = (g_DisplayWindow[ucWinIndex].sDwDisplayWin.height & 0xFFC0);
        }
        else
#endif
        {
            uwWinCapAlign  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
            uwWinDispAlign = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
        }
    }

    if(IsWindowRot90270(ucWinIndex))
    {
        // H
        if( uwWinCapAlign > uwWinDispAlign ) // H down
        {
            g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.width  = uwWinDispAlign;
            g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.height = uwWinDispAlign;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.width  = uwWinCapAlign;
            g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.height = uwWinCapAlign;

        }

        // V
#if (CHIP_ID==MST9U2) || (CHIP_ID==MST9U3)
        if(g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.width) // V down
        {
            g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
        }
        else
#endif
        // V up or 1:1
        {
            g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
        }
    }
    else // no rotate
    {
        // H
        if(g_DisplayWindow[ucWinIndex].sDwCaptureWin.width > g_DisplayWindow[ucWinIndex].sDwDisplayWin.width) // H down
        {
            g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.width = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.width = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.width = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.width = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
            g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.width = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
            g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.width = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
        }
        // V
        if( (g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.height)
          &&(!(   IsWindowDI(ucWinIndex)
               && (g_DisplayWindow[ucWinIndex].u16DwScMaskIn & bSC_MAIN)
        ))) // V down
        {
            g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
        }
        else // V up or 1:1 or DI_post_v_down
        {
            g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
        }
    }

    MSSCALER_PRINT("\n[msScaler] >> 1:1, Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndex, g_ScalerInfo[ucSCIndex].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndex].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndex].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n\n",g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndex].sSiSCFOpmWindow.height);
}
/////////////////////////////////////////////////////////////////////////
///////////PostDispatch mem window calculation on case 1 ==> 2 ////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScPostMemWinCal1To2( BYTE ucSCIndexL, BYTE ucSCIndexR )
{
    BYTE ucWinIndex = MapScaler2Win(ucSCIndexL);
    WORD uwWinCapAlign, uwWinDispAlign;

    g_DisplayWindow[ucWinIndex].u16DwScMaskIn   |= (BIT0<<ucSCIndexL);

    msDrvScPostMemWinInit(ucSCIndexL);
    msDrvScPostMemWinInit(ucSCIndexR);

    // Set Capture, Disp window , win should be initially 0
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin         = g_DisplayWindow[ucWinIndex].sDwCaptureWin;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.x;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.y;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width  = (PanelWidth/2) - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;

#if MST9U_ASIC_1P2SC
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.x      = (PanelWidth/2);
#else
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.x      = 0;
#endif
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.y      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.y;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width - g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;


    // set sync scaler
    g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexL;
    g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexL;

    //64*1sc align for rotate case
    if(IsWindowRot90270(ucWinIndex))
    {
#if (CHIP_ID==MST9U || CHIP_ID==MST9U2)
        if( IsWindowRot270(ucWinIndex)
        ||
          ( (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width > NUM_ROT_LB)
          &&(g_DisplayWindow[ucWinIndex].sDwDisplayWin.height > NUM_ROT_LB)
#if ENABLE_ROT_2SCIP
          &&(g_DisplayWindow[ucWinIndex].sDwCaptureWin.width > g_DisplayWindow[ucWinIndex].sDwDisplayWin.height)
#endif
          )
        )
        {
            uwWinCapAlign  = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width & 0xFFC0);
            uwWinDispAlign = (g_DisplayWindow[ucWinIndex].sDwDisplayWin.height & 0xFFC0);
        }
        else
#endif
        {
            uwWinCapAlign  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
            uwWinDispAlign = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
        }
    }

    if(IsWindowRot90270(ucWinIndex))
    {
        // H
        if( uwWinCapAlign > uwWinDispAlign ) // H down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = uwWinDispAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = 0; // other vars should be initially 0
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = uwWinDispAlign;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = 0;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = uwWinCapAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = 0; // other vars should be initially 0
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = uwWinCapAlign;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = 0;
        }

        // V
#if (CHIP_ID==MST9U2) || (CHIP_ID==MST9U3)
        if(g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.width) // V down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width*(g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN)/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width);

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = 0; // other vars should be initially 0
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        else
#endif
        // V up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width*(g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN)/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width);

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = 0; // other vars should be initially 0
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
    }
    else // no rotate
    {
        // H
        if(g_DisplayWindow[ucWinIndex].sDwCaptureWin.width > g_DisplayWindow[ucWinIndex].sDwDisplayWin.width) // H down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width = 0; // other vars should be initially 0
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width*g_DisplayWindow[ucWinIndex].sDwCaptureWin.width/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width); // scaling ratio?

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        // V
        if( (g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.height)
          &&(!(   IsWindowDI(ucWinIndex)
               && (g_DisplayWindow[ucWinIndex].u16DwScMaskIn & bSC_MAIN)
        ))) // V down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = 0; // other vars should be initially 0
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
        }
        else // V up or 1:1 or DI_post_v_down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
        }
    }

    if( IsWindowMemBal(ucWinIndex) )
    {
        g_DisplayWindow[ucWinIndex].u16DwScMaskIn |= (BIT0<<ucSCIndexR);
        g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin = g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin;
        g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow;
    }

    MSSCALER_PRINT("\n[msScaler] >> 1:2 (balance:%d)\n",g_DisplayWindow[ucWinIndex].bDwMemBalance);
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexL, g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height);
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexR, g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n\n",g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height);
}

/////////////////////////////////////////////////////////////////////////
///////////PostDispatch mem window calculation on case 2 ==> 1 ////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScPostMemWinCal2To1( BYTE ucSCIndex, ST_WINDOW_INFO sAutoWindow )
{
    BYTE    ucWinIndex = MapScaler2Win(ucSCIndex);
    BYTE    ucSCIndexL = ucSCIndex;     // capture L side, does not implies L scaler
    BYTE    ucSCIndexR = SC_MAX_NUMS;   // capture R side, does not implies R scaler
    WORD    uwWinCapAlign, uwWinDispAlign, uwInOnlySCMask=0;
    DWORD   uwAutoWinMid = sAutoWindow.x + sAutoWindow.width/2;
    DWORD   uwDisplayWinCapEnd = g_DisplayWindow[ucWinIndex].sDwCaptureWin.x+g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
    int     swAlignOffset= 0;
    BOOL    bForceLCapAlign = 0;
    BYTE    ucPortIndex = g_DisplayWindow[ucWinIndex].u8DwPortIndex;
    BYTE    ucGuardBand = DFT_HBR2_GUARDBAND;

    uwInOnlySCMask  = g_DisplayWindow[ucWinIndex].u16DwScMaskOut ^ g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    uwInOnlySCMask &= g_DisplayWindow[ucWinIndex].u16DwScMaskIn;

    // Release in-only sub scaler if changed from non-Rotate to Rotate.
    if((uwInOnlySCMask&SCALER_MASK_SUB) && IsWindowRot90270(ucWinIndex))
    {
        msDrvSrcInOnlySCUpdate(MapWin2Port(ucWinIndex), ucWinIndex);
        uwInOnlySCMask = 0;
    }

    // Get 2nd scaler for HBR2
    if( uwInOnlySCMask )
    {
        ucSCIndexR = GetLsbBitPos(uwInOnlySCMask);
    }
    else
#if ENABLE_SUBSCIN
    if( !IsWindowRot90270(ucWinIndex))
    {
#if MST9U_ASIC_1P2SC
        if(IsWindowForce1P2ScOP(ucWinIndex))
            ucSCIndexR = msDrvScGetIdleScalerLM();
        else
            ucSCIndexR = msDrvScGetIdleScalerRM();
#else
        ucSCIndexR = msDrvScGetIdleScalerS();
#endif
    }
    else
#endif
    {
        if(IsWindowForceRot2ScOP(ucWinIndex))
        {
            if(IsWindowLOnly(ucWinIndex) && (msDrvScGetIdleScalerLM()<(SC_MAX_NUMS-1)))
                ucSCIndexR = msDrvScGetIdleScalerLM();
            else if(IsWindowROnly(ucWinIndex) && (msDrvScGetIdleScalerRM()<(SC_MAX_NUMS-1)))
                ucSCIndexR = msDrvScGetIdleScalerRM();
            // 3 SC if msDrvScGetIdleScalerM(), BTW, Rot no sub out
            //else
            //    ucSCIndexR = msDrvScGetIdleScalerM();
        }
        else
        {
            if(IsWindowForce1P2ScOP(ucWinIndex))
                ucSCIndexR = msDrvScGetIdleScalerLM();
            else
                ucSCIndexR = msDrvScGetIdleScalerM();
        }
    }
    MSSCALER_PRINT(">> Scaler PostDispatch Index(Must<%d) :%d \n",SCL0_OFFLINE, ucSCIndexR);

    g_DisplayWindow[ucWinIndex].u16DwScMaskIn   |= (BIT0<<ucSCIndexL);

    // setting new added scaler
    g_DisplayWindow[ucWinIndex].u16DwScMaskIn   |= (BIT0<<ucSCIndexR);
    g_ScalerInfo[ucSCIndexR].u8SiWinIndex        = ucWinIndex;
    g_ScalerInfo[ucSCIndexR].eSiDispStatus       = SC_STATUS_WINDOW_SETUP;
    g_ScalerInfo[ucSCIndexR].u8SiRotate          = g_DisplayWindow[ucWinIndex].u8DWRotate;

    if( INPUT_IS_FAKE_DUAL(ucPortIndex) )
    {
        ucGuardBand = 0;
        if( MapScaler2Port(ucSCIndexR) != msDrvSrcFakeDualGetS() )
        {
            msDrvSrcDetach(MapScaler2Port(ucSCIndexR), ucSCIndexR);
            g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR = HBR2_RIGHT;
            msDrvSrcAttach(msDrvSrcFakeDualGetS(), ucSCIndexR);
        }
    }
    else if( !((g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR == HBR2_RIGHT) && (MapScaler2Port(ucSCIndexR)==MapWin2Port(ucWinIndex))) ) // prevent setup again
    {
        g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR = HBR2_RIGHT;
        msDrvSrcAttach(ucPortIndex, ucSCIndexR);
    }

// need HBR2 mux
// check if port HBR2 (set by IP, then update to L HBR2mux of All sc on it ???) ==> R scaler.HBR2mux

    msDrvScPostMemWinInit(ucSCIndexL);
    msDrvScPostMemWinInit(ucSCIndexR);

#if (SC_FORCE_CAP_ALIGN_EN)
    // Force  L scaler cap align condition: LRcross, LR not centralized. NO Pre-H-scaling
    if( (!IsWindowMemBal(ucWinIndex))
      &&(g_DisplayWindow[ucWinIndex].sDwCaptureWin.width <= (IsWindowRot90270(ucWinIndex)?g_DisplayWindow[ucWinIndex].sDwDisplayWin.height:g_DisplayWindow[ucWinIndex].sDwDisplayWin.width))
      &&((g_DisplayWindow[ucWinIndex].sDwCaptureWin.x < uwAutoWinMid)
        &&(uwDisplayWinCapEnd > uwAutoWinMid)))
    {
        bForceLCapAlign = 1;
    }

    // Calculate Force L Cap Align Offset
    if(bForceLCapAlign)
    {
        if(((uwAutoWinMid - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)%SC_FORCE_CAP_ALIGN)<=SC_FORCE_CAP_ALIGN/2)
            swAlignOffset = 0 - ((uwAutoWinMid - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)%SC_FORCE_CAP_ALIGN);
        else
        {
            swAlignOffset = SC_FORCE_CAP_ALIGN - ((uwAutoWinMid - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)%SC_FORCE_CAP_ALIGN);
            if((uwAutoWinMid + swAlignOffset)>uwDisplayWinCapEnd)
                swAlignOffset = uwDisplayWinCapEnd - uwAutoWinMid;
        }
    }
#endif

    // Set Capture, Disp window , win should be initially 0
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin       = g_DisplayWindow[ucWinIndex].sDwDisplayWin;

#if (!MST9U_ASIC_1P)
    if(g_DisplayWindow[ucWinIndex].sDwDisplayWin.x >= (PanelWidth/2)) // R scaler
    {
        g_ScalerInfo[ucSCIndex].sSiOP2DisplayWin.x -= (PanelWidth/2);
    }
#endif

    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.x      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.x;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.y      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width  = (uwAutoWinMid<=g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)?0:\
                                                      (uwAutoWinMid>uwDisplayWinCapEnd)?g_DisplayWindow[ucWinIndex].sDwCaptureWin.width:\
                                                      (uwAutoWinMid - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x + swAlignOffset);
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.height = (g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width)?g_DisplayWindow[ucWinIndex].sDwCaptureWin.height:0;

    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.x      = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.x >= uwAutoWinMid)?\
                                                      (g_DisplayWindow[ucWinIndex].sDwCaptureWin.x + sAutoWindow.x - uwAutoWinMid + ucGuardBand):\
                                                      ((DWORD)sAutoWindow.x + ucGuardBand + swAlignOffset);
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.y      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y;
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width - g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width;//g_DisplayWindow[ucWinIndex].sSiIPCaptureWin.x + g_DisplayWindow[ucWinIndex].sSiIPCaptureWin.width - (sAutoWindow.x + sAutoWindow.width/2);
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

    // set sync scaler
    if(g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width)
    {
        g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexL;
        g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexL;
    }
    else
    {
        g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexR;
        g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexR;
    }

#if (CHIP_ID == MST9U3)
    // set sync scaler(SC which receive splitter L data refer to SC that receive splitter R data)
    if( (g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR == HBR2_RIGHT)
     && (g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.width) )
    {
        g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC = ucSCIndexR;
        g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC = ucSCIndexR;
    }
    else
    {
        g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC = ucSCIndexL;
        g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC = ucSCIndexL;
    }
#endif

    //64*2sc align
    if(bForceLCapAlign)
    {
        uwWinCapAlign  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
        uwWinDispAlign = uwWinCapAlign;
    }
    else if(IsWindowRot90270(ucWinIndex))
    {
        uwWinCapAlign  = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width & 0xFF80);
        uwWinDispAlign = (g_DisplayWindow[ucWinIndex].sDwDisplayWin.height & 0xFF80);
    }
    else
    {
        uwWinCapAlign  = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width & 0xFF80);
        uwWinDispAlign = (g_DisplayWindow[ucWinIndex].sDwDisplayWin.width & 0xFF80);
    }

    if(IsWindowRot90270(ucWinIndex))
    {
        // H
        if( uwWinCapAlign > uwWinDispAlign ) // H down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinDispAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width);
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = uwWinDispAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = 0;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinCapAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width);
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = uwWinCapAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = 0;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
        }

        // V
#if (CHIP_ID==MST9U2) || (CHIP_ID==MST9U3)
        if(g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.width) // V down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = 0;

            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = 0;
        }
        else
#endif
        // V up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = 0;

            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = 0;
        }
    }
    else // no rotate
    {
        // H
        if( uwWinCapAlign > uwWinDispAlign ) // H down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinDispAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width); // scaling ratio?;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width = uwWinDispAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width = uwWinDispAlign - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width = 0;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x     = 0;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinCapAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width); // scaling ratio?;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width = uwWinCapAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width = uwWinCapAlign - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width = 0;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x     = 0;
        }
        // V
        if( (g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.height)
          &&(!(   IsWindowDI(ucWinIndex)
               && (g_DisplayWindow[ucWinIndex].u16DwScMaskIn & bSC_MAIN)
        ))) // V down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = 0;
        }
        else // V up or 1:1 or DI_post_v_down(force HBR2 mode)
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = 0;
        }
    }

    if( (bForceLCapAlign)
      &&(g_DisplayWindow[ucWinIndex].sDwCaptureWin.width>(IsWindowRot90270(ucWinIndex)?g_DisplayWindow[ucWinIndex].sDwDisplayWin.height:g_DisplayWindow[ucWinIndex].sDwDisplayWin.width)) )
    {
        MSSCALER_PRINT("\n\n\n\n>> Error!! ForceCapAlign not Support in H-Pre-down !!\n\n\n\n");
    }

    MSSCALER_PRINT("\n[msScaler] >> 2:1\n");
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexL, g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height);
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexR, g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n\n",g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height);
}

/////////////////////////////////////////////////////////////////////////
///////////PostDispatch mem window calculation on case 2 ==> 2 ////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScPostMemWinCal2To2( BYTE ucSCIndexL, BYTE ucSCIndexR, ST_WINDOW_INFO sAutoWindow )
{
    BYTE    ucWinIndex = MapScaler2Win(ucSCIndexL);
    WORD    uwWinCapAlign, uwWinDispAlign;
    DWORD   uwAutoWinMid = sAutoWindow.x + sAutoWindow.width/2;
    DWORD   uwDisplayWinCapEnd = g_DisplayWindow[ucWinIndex].sDwCaptureWin.x+g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
    int     swAlignOffset= 0;
    BOOL    bForceLCapAlign = 0;
    BYTE    ucPortIndex = g_DisplayWindow[ucWinIndex].u8DwPortIndex;
    BYTE    ucGuardBand = DFT_HBR2_GUARDBAND;

    g_DisplayWindow[ucWinIndex].u16DwScMaskIn   |= (BIT0<<ucSCIndexL);
    g_DisplayWindow[ucWinIndex].u16DwScMaskIn   |= (BIT0<<ucSCIndexR);

    if( INPUT_IS_FAKE_DUAL(ucPortIndex) )
    {
        ucGuardBand = 0;
        if( MapScaler2Port(ucSCIndexR) != msDrvSrcFakeDualGetS() )
        {
            msDrvSrcDetach(MapScaler2Port(ucSCIndexR), ucSCIndexR);
            g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR = HBR2_RIGHT;
            msDrvSrcAttach(msDrvSrcFakeDualGetS(), ucSCIndexR);
        }
    }
    else if( !((g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR == HBR2_RIGHT) && (MapScaler2Port(ucSCIndexR)==MapWin2Port(ucWinIndex))) ) // prevent setup again
    {
        g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR = HBR2_RIGHT;
        msDrvSrcAttach(ucPortIndex, ucSCIndexR);
    }

// need HBR2 mux
// check if port HBR2 (set by IP, then update to L HBR2mux of All sc on it ???) ==> R scaler.HBR2mux

    msDrvScPostMemWinInit(ucSCIndexL);
    msDrvScPostMemWinInit(ucSCIndexR);

#if (SC_FORCE_CAP_ALIGN_EN)
    // Force  L scaler cap align condition: LRcross, LR not centralized. NO Pre-H-scaling
    if( (!IsWindowMemBal(ucWinIndex))
      &&(g_DisplayWindow[ucWinIndex].sDwCaptureWin.width <= (IsWindowRot90270(ucWinIndex)?g_DisplayWindow[ucWinIndex].sDwDisplayWin.height:g_DisplayWindow[ucWinIndex].sDwDisplayWin.width))
      &&((g_DisplayWindow[ucWinIndex].sDwCaptureWin.x < uwAutoWinMid)
        &&(uwDisplayWinCapEnd > uwAutoWinMid)))
    {
        bForceLCapAlign = 1;
    }

    // Calculate Force L Cap Align Offset
    if(bForceLCapAlign)
    {
        if(((uwAutoWinMid - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)%SC_FORCE_CAP_ALIGN)<=SC_FORCE_CAP_ALIGN/2)
            swAlignOffset = 0 - ((uwAutoWinMid - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)%SC_FORCE_CAP_ALIGN);
        else
        {
            swAlignOffset = SC_FORCE_CAP_ALIGN - ((uwAutoWinMid - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)%SC_FORCE_CAP_ALIGN);
            if((uwAutoWinMid + swAlignOffset)>uwDisplayWinCapEnd)
                swAlignOffset = uwDisplayWinCapEnd - uwAutoWinMid;
        }
    }
#endif

    // Set Capture, Disp window , win should be initially 0
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.x      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.x;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.y      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y;
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width  = (uwAutoWinMid<=g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)?0:\
                                                      (uwAutoWinMid>uwDisplayWinCapEnd)?g_DisplayWindow[ucWinIndex].sDwCaptureWin.width:\
                                                      (uwAutoWinMid - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x + swAlignOffset);
    g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.height = (g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width)?g_DisplayWindow[ucWinIndex].sDwCaptureWin.height:0;

    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.x      = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.x >= uwAutoWinMid)?\
                                                      (g_DisplayWindow[ucWinIndex].sDwCaptureWin.x + sAutoWindow.x - uwAutoWinMid + ucGuardBand):\
                                                      ((DWORD)sAutoWindow.x + ucGuardBand + swAlignOffset);
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.y      = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y;
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width - g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width;//g_DisplayWindow[ucWinIndex].sSiIPCaptureWin.x + g_DisplayWindow[ucWinIndex].sSiIPCaptureWin.width - (sAutoWindow.x + sAutoWindow.width/2);
    g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.x      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.x;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.y      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.y;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width  = (PanelWidth/2) - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x;
    g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;

#if MST9U_ASIC_1P2SC
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.x      = (PanelWidth/2);
#else
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.x      = 0;
#endif
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.y      = g_DisplayWindow[ucWinIndex].sDwDisplayWin.y;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width - g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width;
    g_ScalerInfo[ucSCIndexR].sSiOP2DisplayWin.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;


    // set sync scaler
    if(g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width)
    {
        g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexL;
        g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexL;
    }
    else
    {
        g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC        = ucSCIndexR;
        g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC        = ucSCIndexR;
    }

#if (CHIP_ID == MST9U3)
    // set sync scaler(SC which receive splitter L data refer to SC that receive splitter R data)
    if( (g_ScalerInfo[ucSCIndexR].u8SiIPHBR2LR == HBR2_RIGHT)
     && (g_ScalerInfo[ucSCIndexR].sSiIPCaptureWin.width) )
    {
        g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC = ucSCIndexR;
        g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC = ucSCIndexR;
    }
    else
    {
        g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC = ucSCIndexL;
        g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC = ucSCIndexL;
    }
#endif

    //64*2sc align
    if(bForceLCapAlign)
    {
        uwWinCapAlign  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
        uwWinDispAlign = uwWinCapAlign;
    }
    else if(IsWindowRot90270(ucWinIndex))
    {
        uwWinCapAlign  = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width & 0xFF80);
        uwWinDispAlign = (g_DisplayWindow[ucWinIndex].sDwDisplayWin.height & 0xFF80);
    }
    else
    {
        uwWinCapAlign  = (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width & 0xFF80);
        uwWinDispAlign = (g_DisplayWindow[ucWinIndex].sDwDisplayWin.width & 0xFF80);
    }

    if(IsWindowRot90270(ucWinIndex))
    {
        // H
        if( uwWinCapAlign > uwWinDispAlign ) // H down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinDispAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width);
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = uwWinDispAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = uwWinDispAlign;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinCapAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width);
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = uwWinCapAlign;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = uwWinCapAlign;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
        }

        // V
#if (CHIP_ID==MST9U2) || (CHIP_ID==MST9U3)
        if(g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.width) // V down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width*(g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN)/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width);

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwDisplayWin.width&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        else
#endif
        // V up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width  = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width*(g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN)/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width);

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width  = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height&SC_ROT_V_ALIGN;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width  = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x      = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x      = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
    }
    else // no rotate
    {
        // H
        if( uwWinCapAlign > uwWinDispAlign ) // H down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinDispAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width); // scaling ratio?;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width*uwWinDispAlign/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width);

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width = uwWinDispAlign - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width = uwWinDispAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        else // H up or 1:1
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiIPCaptureWin.width*uwWinCapAlign/g_DisplayWindow[ucWinIndex].sDwCaptureWin.width); // scaling ratio?;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width = ((float)g_ScalerInfo[ucSCIndexL].sSiOP2DisplayWin.width*uwWinCapAlign/g_DisplayWindow[ucWinIndex].sDwDisplayWin.width);

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width = uwWinCapAlign - g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width = uwWinCapAlign;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width = g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width - g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;

            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x     = 0;
            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x     = g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width;
        }
        // V
        if( (g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > g_DisplayWindow[ucWinIndex].sDwDisplayWin.height)
          &&(!(   IsWindowDI(ucWinIndex)
               && (g_DisplayWindow[ucWinIndex].u16DwScMaskIn & bSC_MAIN)
        ))) // V down
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwDisplayWin.height;
        }
        else // V up or 1:1 or DI_post_v_down(force HBR2 mode)
        {
            g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;

            g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
            g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height = g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
        }
    }

    if( (bForceLCapAlign)
      &&(g_DisplayWindow[ucWinIndex].sDwCaptureWin.width>(IsWindowRot90270(ucWinIndex)?g_DisplayWindow[ucWinIndex].sDwDisplayWin.height:g_DisplayWindow[ucWinIndex].sDwDisplayWin.width)) )
    {
        MSSCALER_PRINT("\n\n\n\n>> Error!! ForceCapAlign not Support in H-Pre-down !!\n\n\n\n");
    }

    MSSCALER_PRINT("\n[msScaler] >> 2:2\n");
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexL, g_ScalerInfo[ucSCIndexL].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexL].sSiSCFOpmWindow.height);
    MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d, SyncSC:%d\n",ucWinIndex, ucSCIndexR, g_ScalerInfo[ucSCIndexR].u8SiSCFMemSyncSC);
    MSSCALER_PRINT("[msScaler] Ipm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFIpmWindow.height);
    MSSCALER_PRINT("[msScaler] Mem x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFMemWindow.height);
    MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n\n",g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndexR].sSiSCFOpmWindow.height);
}

void msDrvWinMemAlloc(void)
{
#if 0   // Window Size * 4Byte * 4Frame
    DWORD u32MemBase, u32Size, u32HSize, u32VSize;
    BYTE u8WinIndex, bUpdate;

    u32MemBase = FB_L0_ADR;
    bUpdate = 0;
    for( u8WinIndex = 0 ; u8WinIndex < DISPLAY_MAX_NUMS ; u8WinIndex++ )
    {
        if( g_DisplayWindow[u8WinIndex].u8DwEnable )
        {
            u32HSize = ALIGN_128(g_DisplayWindow[u8WinIndex].sDwDisplayWin.width);
            u32VSize = ALIGN_128(g_DisplayWindow[u8WinIndex].sDwDisplayWin.height);
            u32Size = u32HSize * u32VSize;

            if( (g_DisplayWindow[u8WinIndex].u32DwMemSize[0] != u32Size) || bUpdate)
            {
                bUpdate = 1;
                g_DisplayWindow[u8WinIndex].u32DwMemBase[0] = u32MemBase;
                g_DisplayWindow[u8WinIndex].u32DwMemSize[0] = u32Size * 4 * SC_FIELD_NUM;
                g_DisplayWindow[u8WinIndex].u32DwFbSize[0] = u32Size;
            }

            u32MemBase = g_DisplayWindow[u8WinIndex].u32DwMemBase[0] + g_DisplayWindow[u8WinIndex].u32DwMemSize;
        }
    }
#else   // Aoccording to Ratio of Each Window Size
#if ((CHIP_ID==MST9U2) && ((MEMORY_MAP == MMAP_256_256MB) || (MEMORY_MAP == MMAP_512_512MB)))
    //DWORD u32FBSizeMiu0 = ((DWORD)FB_MIU0_LEN);
    DWORD u32FBSizeMiu = ((DWORD)FB_MIU0_LEN);
    DWORD u32FBSizeMiu1 = ((DWORD)FB_MIU1_LEN);
    DWORD u32WinSizeTotal = 0, u32WinSizeTotal2 = 0;
    DWORD u32MemBase, u32MemBase2, u32Size;
    DWORD u32FBSizeMin, u32FBSizeMin2;
    DWORD u32WinSize[DISPLAY_MAX_NUMS * 2];
    BYTE u8Index, u8WinIndex, u8WinCount, u8WinMinIndex, u8WinMinIndex2, u8WinCountBalance;
    BYTE u8WinPIP0[2] = {0}, u8WinPIP1[2] = {0}, u8PIPIndex = 0xFF;

    u8WinCount = 0;
    u32FBSizeMin = 0;
    u32FBSizeMin2 = 0;
    u8WinMinIndex = 0xFF;
    u8WinMinIndex2 = 0xFF;
    u8WinCountBalance = 0;
    for( u8Index = 0 ; u8Index < (DISPLAY_MAX_NUMS * 2) ; u8Index++ )
    {
        u8WinIndex = u8Index >> 1;
        if( g_DisplayWindow[u8WinIndex].u8DwEnable )
        {
            if( u8Index & _BIT0 )
            {
                if( IsWindowLRCross(u8WinIndex) )   // for balance MIU, allocate 2nd buffer in another MIU
                {
                    u32WinSize[u8Index] = (g_DisplayWindow[u8WinIndex].sDwDisplayWin.width * g_DisplayWindow[u8WinIndex].sDwDisplayWin.height) / 2;
                    u32WinSizeTotal2 += u32WinSize[u8Index];
                    u8WinCountBalance++;
                }
            }
            else
            {
                u32WinSize[u8Index] = (g_DisplayWindow[u8WinIndex].sDwDisplayWin.width * g_DisplayWindow[u8WinIndex].sDwDisplayWin.height);

                if( (u32FBSizeMin == 0)
                 || (u32WinSize[u8Index] < u32FBSizeMin) )
                {
                    u32FBSizeMin2 = u32FBSizeMin;
                    u8WinMinIndex2 = u8WinMinIndex;

                    u32FBSizeMin = u32WinSize[u8Index];
                    u8WinMinIndex = u8Index;
                }
                else if( (u32FBSizeMin2 == 0)
                 || (u32WinSize[u8Index] < u32FBSizeMin2) )
                {
                    u32FBSizeMin2 = u32WinSize[u8Index];
                    u8WinMinIndex2 = u8Index;
                }

                u32WinSizeTotal += u32WinSize[u8Index];
                u8WinCount++;
            }

            if( u8WinCount == 1 )
            {
                if( u8Index & _BIT0 )
                    u8WinPIP0[1] = u8Index;
                else
                    u8WinPIP0[0] = u8Index;
            }
            else if( u8WinCount == 2 )
            {
                if( u8Index & _BIT0 )
                    u8WinPIP1[1] = u8Index;
                else
                    u8WinPIP1[0] = u8Index;
            }
        }
    }

    if( u8WinCount == 1 )
        u8WinMinIndex = 0xFF;
    if( u8WinCount == 2 )
        u8WinMinIndex2 = 0xFF;
    if( u8WinCountBalance > 1 )
        u8WinMinIndex = 0xFF;
    if( u8WinCountBalance > 0 )
        u8WinMinIndex2 = 0xFF;

    MSSCALER_PRINT("msDrvWinMemAlloc(), u8WinCount:%d, u8WinCountBalance:%d, u8WinMinIndex:%d, u8WinMinIndex2:%d\n", u8WinCount, u8WinCountBalance, u8WinMinIndex, u8WinMinIndex2);

    if( u8WinMinIndex != 0xFF )
    {
        u32WinSizeTotal -= u32WinSize[u8WinMinIndex];
        u32WinSizeTotal2 += u32WinSize[u8WinMinIndex];
    }
    if( u8WinMinIndex2 != 0xFF )
    {
        u32WinSizeTotal -= u32WinSize[u8WinMinIndex2];
        u32WinSizeTotal2 += u32WinSize[u8WinMinIndex2];
    }

    // special handle for PIP case, size init at small and switch to large, buffer not enough
    if( (u8WinCount == 2)
     && ((u32WinSize[u8WinPIP0[0]] + u32WinSize[u8WinPIP0[1]]) != (u32WinSize[u8WinPIP1[0]] + u32WinSize[u8WinPIP1[1]])))
    {
        if( (u32WinSize[u8WinPIP0[0]] + u32WinSize[u8WinPIP0[1]]) > (2 * (u32WinSize[u8WinPIP1[0]] + u32WinSize[u8WinPIP1[1]])) )
        {
            u32WinSize[u8WinPIP0[0]] = 3;
            u32WinSize[u8WinPIP0[1]] = 2;
            u32WinSize[u8WinPIP1[0]] = 1;
            u32WinSize[u8WinPIP1[1]] = 0;
            u32WinSizeTotal = 3;
            u32WinSizeTotal2 = 3;

            if( u8WinCountBalance == 2 )
                u8PIPIndex = 2;
        }
        else if( (u32WinSize[u8WinPIP1[0]] + u32WinSize[u8WinPIP1[1]]) > (2 * (u32WinSize[u8WinPIP0[0]] + u32WinSize[u8WinPIP0[1]])) )
        {
            u32WinSize[u8WinPIP0[0]] = 1;
            u32WinSize[u8WinPIP0[1]] = 0;
            u32WinSize[u8WinPIP1[0]] = 3;
            u32WinSize[u8WinPIP1[1]] = 2;
            u32WinSizeTotal = 3;
            u32WinSizeTotal2 = 3;

            if( u8WinCountBalance == 2 )
                u8PIPIndex = 0;
        }
    }

    u32MemBase = FB_MIU1_ADR;
    u32MemBase2 = FB_MIU0_ADR;
    u32FBSizeMiu = (u32FBSizeMiu >> 20);
    u32FBSizeMiu1 = (u32FBSizeMiu1 >> 20);
    for( u8Index = 0 ; u8Index < (DISPLAY_MAX_NUMS * 2) ; u8Index++ )
    {
        u8WinIndex = u8Index >> 1;
        if( g_DisplayWindow[u8WinIndex].u8DwEnable )
        {
            if( ((u8Index & _BIT0) && IsWindowLRCross(u8WinIndex))
             || (u8Index == u8WinMinIndex)
             || (u8Index == u8WinMinIndex2)
             || ((u8PIPIndex != 0xFF) && (u8Index == u8PIPIndex)) )
            {
                u32Size = u32FBSizeMiu * u32WinSize[u8Index] / u32WinSizeTotal2;
                u32Size = (u32Size << 20);

                if( u8Index & _BIT0 )
                {
                    g_DisplayWindow[u8WinIndex].u32DwMemBase[1] = u32MemBase2;
                    g_DisplayWindow[u8WinIndex].u32DwMemSize[1] = u32Size;
                    g_DisplayWindow[u8WinIndex].u32DwFbSize[1] = u32WinSize[u8Index];
                }
                else
                {
                    g_DisplayWindow[u8WinIndex].u32DwMemBase[0] = u32MemBase2;
                    g_DisplayWindow[u8WinIndex].u32DwMemSize[0] = u32Size;
                    g_DisplayWindow[u8WinIndex].u32DwFbSize[0] = u32WinSize[u8Index];
                    g_DisplayWindow[u8WinIndex].u32DwMemBase[1] = u32MemBase2 + u32Size/2;
                    g_DisplayWindow[u8WinIndex].u32DwMemSize[1] = u32Size/2;
                    g_DisplayWindow[u8WinIndex].u32DwFbSize[1] = 0;
                }

                u32MemBase2 = u32MemBase2 + u32Size;

                if( (u8PIPIndex != 0xFF) && (u8Index == u8PIPIndex) ) //skip PIP sub window
                    u8Index++;
            }
            else if( !(u8Index & _BIT0) )
            {
                u32Size = u32FBSizeMiu1 * u32WinSize[u8Index] / u32WinSizeTotal;
                u32Size = (u32Size << 20);

                g_DisplayWindow[u8WinIndex].u32DwMemBase[0] = u32MemBase|_BIT31;
                g_DisplayWindow[u8WinIndex].u32DwMemSize[0] = u32Size;
                g_DisplayWindow[u8WinIndex].u32DwFbSize[0] = u32WinSize[u8Index];
                g_DisplayWindow[u8WinIndex].u32DwMemBase[1] = (u32MemBase2 + u32Size/2)|_BIT31;
                g_DisplayWindow[u8WinIndex].u32DwMemSize[1] = u32Size/2;
                g_DisplayWindow[u8WinIndex].u32DwFbSize[1] = 0;

                u32MemBase = u32MemBase + u32Size;
            }
        }
        else
        {
            g_DisplayWindow[u8WinIndex].u32DwMemBase[0] = 0;
            g_DisplayWindow[u8WinIndex].u32DwMemSize[0] = 0;
            g_DisplayWindow[u8WinIndex].u32DwFbSize[0] = 0;
            g_DisplayWindow[u8WinIndex].u32DwMemBase[1] = 0;
            g_DisplayWindow[u8WinIndex].u32DwMemSize[1] = 0;
            g_DisplayWindow[u8WinIndex].u32DwFbSize[1] = 0;
        }
    }

#else
#if (CHIP_ID==MST9U3)
    DWORD u32FBSize = ((DWORD)FB_R0_ADR + FB_R0_LEN) - ((DWORD)FB_L0_ADR);
#else
    DWORD u32FBSize = ((DWORD)FB_R1_ADR + FB_R1_LEN) - ((DWORD)FB_L0_ADR);
#endif
    DWORD u32WinSize[DISPLAY_MAX_NUMS];
    DWORD u32WinSizeTotal = 0;
    DWORD u32MemBase, u32Size;
    BYTE u8WinIndex, bUpdate;
    BYTE u8WinCount = 0, u8WinPIP0 = 0, u8WinPIP1 = 0;

    for( u8WinIndex = 0 ; u8WinIndex < DISPLAY_MAX_NUMS ; u8WinIndex++ )
    {
        if( g_DisplayWindow[u8WinIndex].u8DwEnable )
        {
            u32WinSize[u8WinIndex] = (g_DisplayWindow[u8WinIndex].sDwDisplayWin.width) * (g_DisplayWindow[u8WinIndex].sDwDisplayWin.height);

            u8WinCount++;
            if( u8WinCount == 1 )
                u8WinPIP0 = u8WinIndex;
            else if( u8WinCount == 2 )
                u8WinPIP1 = u8WinIndex;
        }
        else
            u32WinSize[u8WinIndex] = 0;

        u32WinSizeTotal += u32WinSize[u8WinIndex];
    }

    // special handle for PIP case, size init at small and switch to large, buffer not enough
    if( (u8WinCount == 2)
     && (u32WinSize[u8WinPIP0] != u32WinSize[u8WinPIP1]) )
    {
        if( u32WinSize[u8WinPIP0] > (2 * u32WinSize[u8WinPIP1]) )
        {
            u32WinSize[u8WinPIP0] = 4;
            u32WinSize[u8WinPIP1] = 1;
            u32WinSizeTotal = 5;
        }
        else if( u32WinSize[u8WinPIP1] > (2 * u32WinSize[u8WinPIP0]) )
        {
            u32WinSize[u8WinPIP0] = 1;
            u32WinSize[u8WinPIP1] = 4;
            u32WinSizeTotal = 5;
        }
    }

    u32MemBase = FB_L0_ADR;
    bUpdate = 0;
    u32FBSize = (u32FBSize >> 20);
    for( u8WinIndex = 0 ; u8WinIndex < DISPLAY_MAX_NUMS ; u8WinIndex++ )
    {
        if( g_DisplayWindow[u8WinIndex].u8DwEnable )
        {
            u32Size = u32FBSize * u32WinSize[u8WinIndex] / u32WinSizeTotal;
            u32Size = (u32Size << 20);
            if( (g_DisplayWindow[u8WinIndex].u32DwMemSize[0] != u32Size) || bUpdate)
            {
                bUpdate = 1;
                g_DisplayWindow[u8WinIndex].u32DwMemBase[0] = u32MemBase;
                g_DisplayWindow[u8WinIndex].u32DwMemSize[0] = u32Size;
                g_DisplayWindow[u8WinIndex].u32DwFbSize[0] = u32WinSize[u8WinIndex];

                g_DisplayWindow[u8WinIndex].u32DwMemBase[1] = u32MemBase + u32Size/2;
                g_DisplayWindow[u8WinIndex].u32DwMemSize[1] = u32Size/2;
                g_DisplayWindow[u8WinIndex].u32DwFbSize[1] = u32WinSize[u8WinIndex];
            }

            u32MemBase = u32MemBase + u32Size;
        }
    }
#endif // end of #if MST9U2
#endif // end of #if 0   // Window Size * 4Byte * 4Frame
}

static void msDrvScMemFakeCapWinInit( BYTE ucSCIndex )
{
    g_ScalerInfo[ucSCIndex].sSiImgDEOffset = _InitImgDEOffset;
    g_ScalerInfo[ucSCIndex].u16SiLSCImgOffset = 0;
    g_ScalerInfo[ucSCIndex].u16SiLSCOpmOffset = 0;
    g_ScalerInfo[ucSCIndex].bSiScalerRightPos = FALSE;
}

/////////////////////////////////////////////////////////////////////////
//////////////////// set fake capture window from memory  //////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScMemFakeCapWin( BYTE ucWinIndex, BOOL bFakeCapEnable )
{
    WORD uwSCMask = 0;
    BYTE ucSCIndex1 = SC_MAX_NUMS, ucSCIndex2 = SC_MAX_NUMS;
    BYTE ucTemp;
    BOOL bHstartCross = 0, bHendCross = 0;

    ST_FAKE_CAP_WIN_INFO sTempCapWin = g_DisplayWindow[ucWinIndex].sDwFakeCapWin;
    signed int swFakeCapHend = g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x + g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width;
    signed int swFakeCapVend = g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y + g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height;
    signed int swDispCapHend = g_DisplayWindow[ucWinIndex].sDwCaptureWin.x + g_DisplayWindow[ucWinIndex].sDwCaptureWin.width;
    signed int swDispCapVend = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y + g_DisplayWindow[ucWinIndex].sDwCaptureWin.height;
    signed int swDispWinHend = g_DisplayWindow[ucWinIndex].sDwDisplayWin.x + g_DisplayWindow[ucWinIndex].sDwDisplayWin.width;

    uwSCMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;

    if(uwSCMask)
    {
        ucSCIndex1 = GetLsbBitPos(uwSCMask);
        uwSCMask -= (BIT0<<ucSCIndex1);
    }
    if(uwSCMask)
    {
        ucSCIndex2 = GetLsbBitPos(uwSCMask);
        uwSCMask -= (BIT0<<ucSCIndex2);
    }
    if(uwSCMask)
    {
        MSSCALER_PRINT("\n\n\n\n>> Error!! SC num of window over 2 !!\n\n\n\n");
    }

    msDrvScMemFakeCapWinInit(ucSCIndex1);

    if(ucSCIndex2!=SC_MAX_NUMS)
        msDrvScMemFakeCapWinInit(ucSCIndex2);

#if ENABLE_SCALER_LRSWAP
    if((ucSCIndex2!=SC_MAX_NUMS) && (IsScalerLeft(ucSCIndex1)))
#else
    if((ucSCIndex2!=SC_MAX_NUMS) && (IsScalerRight(ucSCIndex1)))
#endif
    {
        ucTemp = ucSCIndex1;
        ucSCIndex1 = ucSCIndex2;
        ucSCIndex2 = ucTemp;
    }

    if((ucSCIndex2!=SC_MAX_NUMS) && (g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.x)) //ucSCIndex2 is exatcly right scaler
        g_ScalerInfo[ucSCIndex2].bSiScalerRightPos = TRUE;

    if(!bFakeCapEnable)
    {
        MSSCALER_PRINT("\n[msScaler] >>Fake capture window is disabled!!\n");
        return;
    }
    else //enable memory capWin
    {
        if(IsWindowMemBal(ucWinIndex))
        {
            g_DisplayWindow[ucWinIndex].bDwFakeCapWinEnable = FALSE;
            MSSCALER_PRINT("\n\n\n[msScaler] Memory fake capture window is not supported memory balance mode!!\n");
            MSSCALER_PRINT("\n[msScaler] >>Fake capture window is disabled!!\n");
            return;
        }
        else if(IsWindowRot90270(ucWinIndex))
        {
            g_DisplayWindow[ucWinIndex].bDwFakeCapWinEnable = FALSE;
            MSSCALER_PRINT("\n\n\n[msScaler] Memory fake capture window is not supported window rotation!!\n");
            MSSCALER_PRINT("\n[msScaler] >>Fake capture window is disabled!!\n");
            return;
        }
        // map fake capture window to real capture window for OPM window reset
        if(sTempCapWin.x < g_DisplayWindow[ucWinIndex].sDwCaptureWin.x)//H start underScan
        {
            sTempCapWin.width -= (g_DisplayWindow[ucWinIndex].sDwCaptureWin.x - sTempCapWin.x);
            sTempCapWin.x = g_DisplayWindow[ucWinIndex].sDwCaptureWin.x;
        }

        if(sTempCapWin.y < g_DisplayWindow[ucWinIndex].sDwCaptureWin.y)//V start underScan
        {
            sTempCapWin.height -= (g_DisplayWindow[ucWinIndex].sDwCaptureWin.y - sTempCapWin.y);
            sTempCapWin.y = g_DisplayWindow[ucWinIndex].sDwCaptureWin.y;
        }

        if((sTempCapWin.x + sTempCapWin.width) > swDispCapHend)//H end usnderScan
        {
            sTempCapWin.width -= ((sTempCapWin.x + sTempCapWin.width) - swDispCapHend);
        }

        if((sTempCapWin.y + sTempCapWin.height) > swDispCapVend)//V end underScan
        {
            sTempCapWin.height -= ((sTempCapWin.y + sTempCapWin.height) - swDispCapVend);
        }
        // reset OPM window
#if 0
        if(IsWindowRot90270(ucWinIndex))
        {
            //need to implement
        }
        else// no rotate
#endif
        {
           if(ucSCIndex2 != SC_MAX_NUMS) // 2 output SC
            {
                //image DE of left SC
                g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart = (g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x < g_DisplayWindow[ucWinIndex].sDwCaptureWin.x) ? ((g_DisplayWindow[ucWinIndex].sDwCaptureWin.x - g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x) * g_DisplayWindow[ucWinIndex].sDwDisplayWin.width / g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width) : 0;
                g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVStart = (g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y < g_DisplayWindow[ucWinIndex].sDwCaptureWin.y) ? ((g_DisplayWindow[ucWinIndex].sDwCaptureWin.y - g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y) * g_DisplayWindow[ucWinIndex].sDwDisplayWin.height / g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height) : 0;
                g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVEnd = (swFakeCapVend > swDispCapVend) ? (((swFakeCapVend - swDispCapVend) * g_DisplayWindow[ucWinIndex].sDwDisplayWin.height + g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height / 2 ) / g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height) : 0;

                //image DE of right SC
                g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetHEnd = (swFakeCapHend > swDispCapHend) ? (((swFakeCapHend - swDispCapHend) * g_DisplayWindow[ucWinIndex].sDwDisplayWin.width  + g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width / 2 ) / g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width) : 0;
                g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetVStart = g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVStart;
                g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetVEnd = g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVEnd;
                g_ScalerInfo[ucSCIndex2].u16SiLSCImgOffset = g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart; // image DE of H start of L SC

                //special case for offset of image DE is LR cross
                if(g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart > (PANEL_WIDTH/2 - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x))
                {
                    g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetHStart = (g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart - ((PANEL_WIDTH/2) - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x));
                    g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart = (PANEL_WIDTH/2 - g_DisplayWindow[ucWinIndex].sDwDisplayWin.x);
                    bHstartCross = 1;
                }
                else if(g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetHEnd > (PANEL_WIDTH/2 - (PANEL_WIDTH - swDispWinHend)))
                {
                    g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHEnd = (g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetHEnd - (PANEL_WIDTH/2 - (PANEL_WIDTH - swDispWinHend)));
                    g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetHEnd = (PANEL_WIDTH/2 - (PANEL_WIDTH - swDispWinHend));
                    bHendCross = 1;
                }

                //OPM window of left SC
                g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.x = ((sTempCapWin.x - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x) * g_ScalerInfo[ucSCIndex1].sSiSCFMemWindow.width / g_DisplayWindow[ucWinIndex].sDwCaptureWin.width);
                g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.y = ((sTempCapWin.y - g_DisplayWindow[ucWinIndex].sDwCaptureWin.y) * g_ScalerInfo[ucSCIndex1].sSiSCFMemWindow.height / g_DisplayWindow[ucWinIndex].sDwCaptureWin.height);
                g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.width = bHstartCross ? 0 : \
                                                                (bHendCross ? (g_ScalerInfo[ucSCIndex1].sSiSCFMemWindow.width * sTempCapWin.width / g_DisplayWindow[ucWinIndex].sDwCaptureWin.width)\
                                                                            : ((g_ScalerInfo[ucSCIndex1].sSiOP2DisplayWin.width - g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart)\
                                                                                * (sTempCapWin.width * g_ScalerInfo[ucSCIndex2].sSiSCFMemWindow.width / g_DisplayWindow[ucWinIndex].sDwCaptureWin.width)\
                                                                                / (g_DisplayWindow[ucWinIndex].sDwDisplayWin.width - g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart - g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetHEnd)) );

                g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.height = bHstartCross ? 0 : (g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.height * sTempCapWin.height / g_DisplayWindow[ucWinIndex].sDwCaptureWin.height);

                //OPM window of right SC
                g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.x = g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.x + g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.width;
                g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.y = g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.y;
                g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.width = bHendCross ? 0 : ((g_ScalerInfo[ucSCIndex2].sSiSCFMemWindow.width * sTempCapWin.width / g_DisplayWindow[ucWinIndex].sDwCaptureWin.width) - g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.width);
                g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.height = bHendCross ? 0 : (g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.height * sTempCapWin.height / g_DisplayWindow[ucWinIndex].sDwCaptureWin.height);
                g_ScalerInfo[ucSCIndex2].u16SiLSCOpmOffset = g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.x; // opm.x of L SC

                MSSCALER_PRINT("\n[msScaler] >>fake capture window is enabled!!\n");
                MSSCALER_PRINT("[msScaler] Fake capWin: 2 output SC\n");
                MSSCALER_PRINT("[msScaler] FakeCapWin x:%d, y:%d, w:%d, h:%d\n", g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x, g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y, g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width, g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height);
                MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d\n",ucWinIndex, ucSCIndex1);
                MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.height);
                MSSCALER_PRINT("[msScaler] Image DE offset Hstart:%d, Hend:%d, Vstart:%d, Vend:%d\n", g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart, g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHEnd, g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVStart, g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVEnd);
                MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d\n",ucWinIndex, ucSCIndex2);
                MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndex2].sSiSCFOpmWindow.height);
                MSSCALER_PRINT("[msScaler] Image DE offset Hstart:%d, Hend:%d, Vstart:%d, Vend:%d\n", g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetHStart, g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetHEnd, g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetVStart, g_ScalerInfo[ucSCIndex2].sSiImgDEOffset.u32OffsetVEnd);
                MSSCALER_PRINT("[msScaler] Is scaler rightPos:%b\n", g_ScalerInfo[ucSCIndex2].bSiScalerRightPos);
                MSSCALER_PRINT("[msScaler] Image offset of left SC:%d\n", g_ScalerInfo[ucSCIndex2].u16SiLSCImgOffset);
                MSSCALER_PRINT("[msScaler] Opm offset of left SC:%d\n", g_ScalerInfo[ucSCIndex2].u16SiLSCOpmOffset);
            } // end of  if(ucSCIndex2 != SC_MAX_NUMS)    2 output SC
            else // 1 output SC
            {
                //image DE of output SC
                g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart = (g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x < g_DisplayWindow[ucWinIndex].sDwCaptureWin.x) ? ((g_DisplayWindow[ucWinIndex].sDwCaptureWin.x - g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x) * g_DisplayWindow[ucWinIndex].sDwDisplayWin.width / g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width) : 0;
                g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHEnd = (swFakeCapHend  > swDispCapHend) ? (((swFakeCapHend - swDispCapHend) * g_DisplayWindow[ucWinIndex].sDwDisplayWin.width + g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width / 2) / g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width) : 0;
                g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVStart = (g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y < g_DisplayWindow[ucWinIndex].sDwCaptureWin.y) ? ((g_DisplayWindow[ucWinIndex].sDwCaptureWin.y - g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y) * g_DisplayWindow[ucWinIndex].sDwDisplayWin.height / g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height) : 0;
                g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVEnd = (swFakeCapVend > swDispCapVend) ? (((swFakeCapVend - swDispCapVend) * g_DisplayWindow[ucWinIndex].sDwDisplayWin.height + g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height / 2)/ g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height) : 0;

                //OPM window of output SC
                g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.x = ((sTempCapWin.x - g_DisplayWindow[ucWinIndex].sDwCaptureWin.x) * g_ScalerInfo[ucSCIndex1].sSiSCFMemWindow.width / g_DisplayWindow[ucWinIndex].sDwCaptureWin.width);
                g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.y = ((sTempCapWin.y - g_DisplayWindow[ucWinIndex].sDwCaptureWin.y) * g_ScalerInfo[ucSCIndex1].sSiSCFMemWindow.height / g_DisplayWindow[ucWinIndex].sDwCaptureWin.height);
                g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.width = (g_ScalerInfo[ucSCIndex1].sSiSCFMemWindow.width * sTempCapWin.width / g_DisplayWindow[ucWinIndex].sDwCaptureWin.width);
                g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.height = (g_ScalerInfo[ucSCIndex1].sSiSCFMemWindow.height * sTempCapWin.height / g_DisplayWindow[ucWinIndex].sDwCaptureWin.height);

                MSSCALER_PRINT("\n[msScaler] >>fake capture window is enablled!!\n");
                MSSCALER_PRINT("[msScaler] Fake cap win: 1 output SC\n");
                MSSCALER_PRINT("[msScaler] FakeCapWin x:%d, y:%d, w:%d, h:%d\n", g_DisplayWindow[ucWinIndex].sDwFakeCapWin.x, g_DisplayWindow[ucWinIndex].sDwFakeCapWin.y, g_DisplayWindow[ucWinIndex].sDwFakeCapWin.width, g_DisplayWindow[ucWinIndex].sDwFakeCapWin.height);
                MSSCALER_PRINT("[msScaler] Win:%d, Scaler:%d\n",ucWinIndex, ucSCIndex1);
                MSSCALER_PRINT("[msScaler] Opm x:%d, y:%d, w:%d, h:%d\n",g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.x, g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.y, g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.width, g_ScalerInfo[ucSCIndex1].sSiSCFOpmWindow.height);
                MSSCALER_PRINT("[msScaler] Image DE offset Hstart:%d, Hend:%d, Vstart:%d, Vend:%d\n", g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHStart, g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetHEnd, g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVStart, g_ScalerInfo[ucSCIndex1].sSiImgDEOffset.u32OffsetVEnd);
                MSSCALER_PRINT("[msScaler] Is scaler rightPos:%b\n", g_ScalerInfo[ucSCIndex1].bSiScalerRightPos);
            }
        } // end of no rotate
    } // end of memory capWin is enabled
}

#if MST9U_ASIC_1P2SC
#define CAP_H_THD   3000
static void msDrvScSet1P2Sc(BYTE ucWinIndex)
{
    if( ( (!IsWindowRot90270(ucWinIndex) && (g_DisplayWindow[ucWinIndex].sDwCaptureWin.width >= CAP_H_THD))
         ||(IsWindowRot90270(ucWinIndex) && (g_DisplayWindow[ucWinIndex].sDwCaptureWin.height > NUM_ROT_LB)) )
      &&(!msDrvCheckMultiWin()) )
    {
        g_DisplayWindow[ucWinIndex].bDwForce1P2ScOP = TRUE;
    }
    else
    {
        g_DisplayWindow[ucWinIndex].bDwForce1P2ScOP = FALSE;
    }
}
#endif

/////////////////////////////////////////////////////////////////////////
//////////////////// scaler source post-dispatch for win  //////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScPostDispatch( BYTE ucWinIndex, ST_WINDOW_INFO sAutoWindow )
{
    WORD    uwSCMask = 0;
    BYTE    ucInputPort = MapWin2Port(ucWinIndex);
    BYTE    ucSCIndex1 = SC_MAX_NUMS;
    BYTE    ucSCIndex2 = SC_MAX_NUMS;
    BYTE    ucTemp;

#if 0 // now always 2p
    BOOL    bSCOutputDualMode = FALSE; // FALSE: 1P, TRUE: 2P
    bSCOutputDualMode = msDrvScCheckOutputDualMode(); // FALSE: 1P, TRUE: 2P
#endif

    // Get SC index of window (PreDispatch)
    uwSCMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    if(uwSCMask)
    {
        ucSCIndex1 = GetLsbBitPos(uwSCMask);
        uwSCMask -= (BIT0<<ucSCIndex1);
    }
    if(uwSCMask)
    {
        ucSCIndex2 = GetLsbBitPos(uwSCMask);
        uwSCMask -= (BIT0<<ucSCIndex2);
    }
    if(uwSCMask)
    {
        MSSCALER_PRINT("\n\n\n\n>> Error!! SC num of window over 2 !!\n\n\n\n");
    }

    // exchange order,  scaler L first
#if ENABLE_SCALER_LRSWAP
    if( (ucSCIndex2!=SC_MAX_NUMS) && (IsScalerLeft(ucSCIndex1)) )
#else
    if( (ucSCIndex2!=SC_MAX_NUMS) && (IsScalerRight(ucSCIndex1)) )
#endif
    {
        ucTemp = ucSCIndex1;
        ucSCIndex1 = ucSCIndex2;
        ucSCIndex2 = ucTemp;
    }

    // changed from 2 to 1 sc due to asp ratio
    if( (ucSCIndex2!=SC_MAX_NUMS)&&((IsWindowLOnly(ucWinIndex)) || (IsWindowROnly(ucWinIndex))
#if MST9U_ASIC_1P2SC
        || (!msDrvCheckMultiWin()) // multi-window OFF.
#endif
        ))
    {
        ucSCIndex1 = msDrvScPostWinUpdate2To1(ucSCIndex1, ucSCIndex2); // return remaining scaler
        ucSCIndex2 = SC_MAX_NUMS;
    }
#if !MST9U_ASIC_1P // MST9U_ASIC_1P2SC:TBD
    // changed from 1 to 2 sc due to asp ratio
    else if((ucSCIndex2==SC_MAX_NUMS) && (IsWindowLRCross(ucWinIndex)))
    {
        ucSCIndex2 = msDrvScPostWinUpdate1To2(ucSCIndex1);
        // exchange order,  scaler L first
#if ENABLE_SCALER_LRSWAP
        if( (ucSCIndex2!=SC_MAX_NUMS) && (IsScalerLeft(ucSCIndex1)) )
#else
        if( (ucSCIndex2!=SC_MAX_NUMS) && (IsScalerRight(ucSCIndex1)) )
#endif
        {
            ucTemp = ucSCIndex1;
            ucSCIndex1 = ucSCIndex2;
            ucSCIndex2 = ucTemp;
        }
    }
#endif

#if MST9U_ASIC_1P2SC
    msDrvScSet1P2Sc(ucWinIndex);
#endif

#if ENABLE_ROT_2SCOP// Currently Only For 4K2K Native 2 Wins
    // check for using 2 sc (Both L or R) for rotate 90/270 & post h down case. (can take scaling ratio into consideration)
    if( (IsWindowRotPostHDown(ucWinIndex)) // window rorate 90/270 & post H down
      &&(ucSCIndex2==SC_MAX_NUMS)  // Original window L or R only, before AspRatio
      &&(!IsWindowMemBal(ucWinIndex)) ) // Current setting/flow limit. Can be extended.
    {
        g_DisplayWindow[ucWinIndex].bDwForceRot2ScOP = TRUE;
    }
    else
    {
        g_DisplayWindow[ucWinIndex].bDwForceRot2ScOP = FALSE;
    }
#endif


    // Need refine for Original Scaler sub out but changed from non-rotate to rotate case
    if(DualInFlag(ucInputPort)) // no consider input  2 capture sc reduced to 1sc due to capture window range reduced !!!
    {
        if(IsWindowMemBal(ucWinIndex)) // Force HBR2 using 2 scaler processing individually.
        {
            msDrvScPostMemWinCalHBR2Force2SC(ucSCIndex1, ucSCIndex2, sAutoWindow);
        }
        else if(ucSCIndex2==SC_MAX_NUMS) // 2==>1, display only in L or R (PreDispatch)
        {
            msDrvScPostMemWinCal2To1(ucSCIndex1, sAutoWindow);
        }
        else // 2==>2, display in both L & R (PreDispatch)
        {
            msDrvScPostMemWinCal2To2(ucSCIndex1, ucSCIndex2, sAutoWindow);
        }
    }
    else
    {
        if(ucSCIndex2==SC_MAX_NUMS) // 1==>1,  display only in L or R (PreDispatch)
        {
            msDrvScPostMemWinCal1To1(ucSCIndex1);
        }
        else // 1==>2,  display in both L & R (PreDispatch)
        {
            msDrvScPostMemWinCal1To2(ucSCIndex1, ucSCIndex2);
        }
    }


#if ENABLE_ROT_2SCIP
    // refine mem window for Rotate using 2 SC to capture when H (NO H-pre scaling) > NUM_ROT_LB
    if(!DualInFlag(ucInputPort)) // 1 sc in case
    {
        if( (IsWindowRot90270(ucWinIndex)) && (g_ScalerInfo[ucSCIndex1].sSiSCFIpmWindow.width > NUM_ROT_LB)
          &&((g_DisplayWindow[ucWinIndex].sDwCaptureWin.width <= g_DisplayWindow[ucWinIndex].sDwDisplayWin.height)) )
        {
            msDrvScPostMemWinCalRefineRot2ScIP(ucWinIndex);
            g_DisplayWindow[ucWinIndex].bDwForceRot2ScIP = TRUE;
        }
        else
        {
            msDrvSrcInOnlySCUpdate(ucInputPort, ucWinIndex);
            g_DisplayWindow[ucWinIndex].bDwForceRot2ScIP = FALSE;
        }
    }
#endif


    // refine mem window for bDwForceRot2ScOP == TRUE
    if(IsWindowForceRot2ScOP(ucWinIndex))
    {
        msDrvScPostMemWinCalRefineRot2ScOP(ucWinIndex);
    }

     // refine mem window for bDwForce1P2ScOP == TRUE
    if(IsWindowForce1P2ScOP(ucWinIndex))
    {
        msDrvScPostMemWinCalRefine1P2ScOP(ucWinIndex);
    }
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// set scaler status (bitmask)  ////////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScSetStatus( WORD uwScMask, EN_SCALER_DISP_STATUS eScalerStatus)
{
    BYTE    u8SiScIndex = 0;

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            g_ScalerInfo[u8SiScIndex].eSiDispStatus = eScalerStatus;
        }

        uwScMask >>=1;
        u8SiScIndex ++;
    }
}

/////////////////////////////////////////////////////////////////////////
///////////////////// set scaler output mute (bitmask)  ///////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScMuteScreen( WORD uwScMask, BOOL bEnableMute)
{
    BYTE    u8ScIndex = 0;
    DWORD   u32SCRegBase;

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            u32SCRegBase = SCRegBase(u8ScIndex);

            if(u8ScIndex & BIT0) // Main
                msWriteBit(SC10_32+u32SCRegBase, bEnableMute, _BIT1); // output mute
            else  // Sub
                msWriteBit(SC10_32+u32SCRegBase, bEnableMute, _BIT5); // output mute
        }

        uwScMask >>=1;
        u8ScIndex ++;
    }
}

/////////////////////////////////////////////////////////////////////////
///////////////////////// clear all input port status  //////////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvSrcStatusClr( void )
{
    BYTE u8InputPort;

#if UseINT
    mStar_EnableModeChangeINT(FALSE, SCALER_MASK_ALL);
#endif

    for( u8InputPort=First_Input_Port; u8InputPort < Input_Nums; u8InputPort++ )
    {
        g_InputPort[u8InputPort].u8IPScIndex  = SC_MAX_NUMS;
        g_InputPort[u8InputPort].u16IPScMask  = 0;
        g_InputPort[u8InputPort].eIPStatus    = IP_STATUS_IDLE;
        g_InputPort[u8InputPort].bIPInputHBR2 = FALSE;
#ifndef MST9U_FPGA
        msAPI_combo_IPConfiguration(FALSE, (EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, g_InputPort[u8InputPort].eIPType);
#endif
    }
}

/////////////////////////////////////////////////////////////////////////
//////////////////////// detach scaler to input port///////////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvSrcDetach( BYTE InputPort, BYTE SCDetWin )
{
    if(g_InputPort[InputPort].u16IPScMask & (BIT0<<SCDetWin)) // scaler assigned in input port
    {
        if(g_InputPort[InputPort].u16IPScMask == (BIT0<<SCDetWin)) // only this scaler in input port
        {
            g_InputPort[InputPort].u8IPScIndex  = SC_MAX_NUMS;
            g_InputPort[InputPort].u16IPScMask  = 0;
            g_InputPort[InputPort].eIPStatus    = IP_STATUS_IDLE;
#if UseINT
            mStar_EnableModeChangeINT(FALSE, BIT0<<SCDetWin);
#endif
#ifndef MST9U_FPGA
            msAPI_combo_IPConfiguration(FALSE, (EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, g_InputPort[InputPort].eIPType);
#endif
            g_InputPort[InputPort].bIPInputHBR2 = FALSE;
        }
        else
        {
            g_InputPort[InputPort].u16IPScMask &= (~(BIT0<<SCDetWin));

            if(g_InputPort[InputPort].u8IPScIndex == SCDetWin) // detecting scaler removed, need assigned another scaler
            {
#if UseINT
                mStar_EnableModeChangeINT(FALSE, BIT0<<SCDetWin);
#endif
                g_InputPort[InputPort].u8IPScIndex = msDrvScGetIPStableScaler(g_InputPort[InputPort].u16IPScMask);
                if(g_InputPort[InputPort].u8IPScIndex == (SC_MAX_NUMS-1))
                {
                    //Set_InputTimingChangeFlag(InputPort);
                    g_InputPort[InputPort].u8IPScIndex = GetLsbBitPos(g_InputPort[InputPort].u16IPScMask);
                }
                else
                {
#if UseINT
                    mStar_EnableModeChangeINT(TRUE, BIT0<<(g_InputPort[InputPort].u8IPScIndex));
#endif
                }

                if(msDrvIP1ReadImageVStart(g_InputPort[InputPort].u8IPScIndex) == 0x1FFF)
                {
                    msDrvIP1SetImageVStart( 0, g_InputPort[InputPort].u8IPScIndex);
                }
            }
        }

        g_ScalerInfo[SCDetWin].u8SiIPPortIndex = Input_Nums;
    }
}


/////////////////////////////////////////////////////////////////////////
////////////////////////// attach scaler to input port/////////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvSrcAttach( BYTE InputPort, BYTE SCDetWin )
{
    if(g_InputPort[InputPort].u8IPScIndex == SC_MAX_NUMS) // not assigned yet
    {
        g_InputPort[InputPort].u8IPScIndex  = SCDetWin;
        g_InputPort[InputPort].u16IPScMask  = BIT0<<SCDetWin;
        g_InputPort[InputPort].u8IPSysFlags = 0;//bInputTimingChangeBit;
        g_InputPort[InputPort].eIPStatus    = IP_STATUS_WAITSYNC;
        g_InputPort[InputPort].bIPInputHBR2 = FALSE;
#ifndef MST9U_FPGA
        msAPI_combo_IPConfiguration(TRUE, (EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, g_InputPort[InputPort].eIPType);
#endif
        MSSCALER_PRINT("[msScaler] SourceAttach:%d, DetSC:%d\n",InputPort, SCDetWin);
    }
    else // already scaler assigned for detection
    {
        g_InputPort[InputPort].u16IPScMask |= (BIT0<<SCDetWin);
    }

    msDrvSrcSyncStatusToSC(InputPort, SCDetWin);
    msDrvSetIPMux(InputPort, SCDetWin);
    g_ScalerInfo[SCDetWin].u8SiIPPortIndex = InputPort;
}

//////////////////////////////////////////////////////////////////////////////////////////
///// clear ScMaskIn ONLY scaler. e.g. when changed from 2P -> 1P in HBR2 output window L/R only case.  ////
//////////////////////////////////////////////////////////////////////////////////////////
void msDrvSrcInOnlySCUpdate(BYTE u8InputPort, BYTE u8WinIndex)
{
    BYTE u8IdxWin = DISPLAY_WIN1;
    BYTE u8IdxWinEnd = DISPLAY_MAX_NUMS-1;
    WORD u16SCMaskDel = 0;

    if(INPUT_IS_FAKE_DUAL_S(u8InputPort) && (!INPUT_IS_FAKE_DUAL_MONO(u8InputPort)))
        return;

    if(u8WinIndex != DISPLAY_MAX_NUMS)
    {
        u8IdxWin = u8WinIndex;
        u8IdxWinEnd = u8WinIndex;
    }

    for( ; u8IdxWin <= u8IdxWinEnd ; u8IdxWin++ )
    {
        if( (u8InputPort == g_DisplayWindow[u8IdxWin].u8DwPortIndex)
          &&(WIN_STATUS_IDLE != g_DisplayWindow[u8IdxWin].eDwStatus) )
        {
            u16SCMaskDel  = g_DisplayWindow[u8IdxWin].u16DwScMaskOut ^ g_DisplayWindow[u8IdxWin].u16DwScMaskIn;
            u16SCMaskDel &= g_DisplayWindow[u8IdxWin].u16DwScMaskIn;

            if(u16SCMaskDel != 0) // 2->1
            {
                msDrvScStatusClr(u16SCMaskDel, TRUE);
                g_DisplayWindow[u8IdxWin].u16DwScMaskIn -= (u16SCMaskDel);
                if(INPUT_IS_FAKE_DUAL(u8InputPort))
                    msDrvSrcDetach(msDrvSrcFakeDualGetS(), GetLsbBitPos(u16SCMaskDel));
                else
                    msDrvSrcDetach(u8InputPort, GetLsbBitPos(u16SCMaskDel));
            }
            else
            {
                u16SCMaskDel = msDrvScFakeDualGetS(u8IdxWin);
                if(INPUT_IS_FAKE_DUAL(u8InputPort) && (!INPUT_IS_FAKE_DUAL_MONO(u8InputPort)) && ( u16SCMaskDel != SC_MAX_NUMS) )// for 2->2 Fake Dual case, keep scaler but detach slave port, attach to master port
                {
                    msDrvSrcDetach(msDrvSrcFakeDualGetS(), u16SCMaskDel);
                    g_DisplayWindow[u8IdxWin].u16DwScMaskIn -= (BIT0<<u16SCMaskDel);
                    if(g_DisplayWindow[u8IdxWin].u16DwScMaskOut & (BIT0<<u16SCMaskDel))
                        msDrvSrcAttach(u8InputPort, u16SCMaskDel);
                }
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////
///// set HBR2 flag & mux, eIPType should consider TYPE_HBR2_0, TYPE_HBR2_1  ////
/////////////////////////////////////////////////////////////////////////
void msDrvSrcSetHBR2FlagMux(BYTE u8InputPort, BOOL bHBR2In, EN_INPUTPORT_TYPE eIPType)
{
    BYTE    u8ScIndex = 0;
    WORD    u16ScMask = 0;

    // clear ScMaskIn ONLY scaler. when changed from 2P -> 1P in HBR2 output window L/R only case.
    if( HBR2Flag(u8InputPort) && (!bHBR2In) )
    {
        msDrvSrcInOnlySCUpdate(u8InputPort, DISPLAY_MAX_NUMS);
    }

    g_InputPort[u8InputPort].bIPInputHBR2 = bHBR2In;
    if(eIPType!=0)
        g_InputPort[u8InputPort].eIPType      = eIPType; // should consider TYPE_HBR2_0, TYPE_HBR2_1

    u16ScMask = g_InputPort[u8InputPort].u16IPScMask;

    // after input HBR2 changed, need to re-set  mux again for each scaler
    while(u16ScMask!=0)
    {
        if(u16ScMask & BIT0)
        {
            msDrvSetIPMux(u8InputPort, u8ScIndex);
        }
        u16ScMask >>=1;
        u8ScIndex ++;
    }

    if(INPUT_IS_HBR2(u8InputPort))
    {
        msAPI_combo_IPSetDPGuardBand((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, DFT_HBR2_GUARDBAND);
    }
}

/////////////////////////////////////////////////////////////////////////
////////////////////////////// Src config init //////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvSrcConfigInit( void )
{
    BYTE u8InputPort;

    if(g_eBootStatus == eBOOT_STATUS_DCON)
    {
        msAPI_combo_IPSetPowerStatus(COMBO_IP_POWER_DC_ON);
    }
    else if(g_eBootStatus == eBOOT_STATUS_DPMSON)
    {
        msAPI_combo_IPSetPowerStatus(COMBO_IP_POWER_PS_ON);
    }
    else
    {
        msAPI_combo_IPSetPowerStatus(COMBO_IP_POWER_AC_ON);
    }

    for( u8InputPort=First_Input_Port; u8InputPort < Input_Nums; u8InputPort++ )
    {
        if(g_InputPort[u8InputPort].eIPType == TYPE_VD)
            continue;
#ifndef MST9U_FPGA
        msAPI_combo_IPConfiguration((g_InputPort[u8InputPort].eIPStatus != IP_STATUS_IDLE), (EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, g_InputPort[u8InputPort].eIPType);
#endif
        g_InputPort[u8InputPort].bIPInputHBR2 = FALSE;
    }
}

///////////////////////////////////////////////////////
/////////////// Sync input port status to scaler /////////////
///////////////////////////////////////////////////////
void msDrvSrcSyncStatusToSC( BYTE InputPort, BYTE SCDetWin)
{
    EN_SCALER_DISP_STATUS   SC_STATUS = SC_STATUS_IDLE;

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

    g_ScalerInfo[SCDetWin].eSiDispStatus = SC_STATUS;
}
/////////////////////////////////////////////////////////////////////////
///////////////////////////// monitor input timing  //////////////////////////
/////////////////////////////////////////////////////////////////////////
static void msDrvSMInput( void )
{
    BYTE InputPort;
    static BYTE ucPrevStatus[Input_Nums];

    for( InputPort=First_Input_Port; InputPort < Input_Nums; InputPort++ )
    {
        // print status for debug
        if(g_InputPort[InputPort].eIPStatus != ucPrevStatus[InputPort])
        {
            ucPrevStatus[InputPort] = g_InputPort[InputPort].eIPStatus;
            //MSSCALER_PRINT("[SMInput] Port: %d, status: %d \n",InputPort, g_InputPort[InputPort].eIPStatus);
        }

        switch(g_InputPort[InputPort].eIPStatus)
        {
            case IP_STATUS_IDLE:
                continue;

            case IP_STATUS_FINDMODE:
                msFindMode(InputPort);
                break;

            case IP_STATUS_WAITSYNC:
            case IP_STATUS_NOSYNC:
            case IP_STATUS_FINDMODEOK:
                if( !ModeDetectCounter(InputPort) || InputTimingChangeFlag(InputPort) )
                {
                    mStar_MonitorInputTiming(InputPort);
                    ModeDetectCounter(InputPort) = 20;
                }
                break;

            default:
                break;
        }
    }
}

void msDrvScSetFblMode(BYTE ucWinIndex, BOOL bEnable)
{
    WORD u16SCMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;
    BYTE u8SCIndex1 = SC_MAX_NUMS;
    BYTE u8SCIndex2 = SC_MAX_NUMS;

    if(u16SCMask)
    {
        u8SCIndex1 = GetLsbBitPos(u16SCMask);
        u16SCMask -= (BIT0<<u8SCIndex1);
    }
    if(u16SCMask)
    {
        u8SCIndex2 = GetLsbBitPos(u16SCMask);
        u16SCMask -= (BIT0<<u8SCIndex2);
    }
    if(u16SCMask)
    {
        MSSCALER_PRINT("\n\n\n\n>> Error!! SC num of window over 2 !!\n\n\n\n");
    }

   if(bEnable) //enable frame buffer less mode
   {
        g_ScalerInfo[u8SCIndex1].u8SiFbMode = 0;

        if(u8SCIndex2 != SC_MAX_NUMS)
            g_ScalerInfo[u8SCIndex2].u8SiFbMode = 0;

        MSSCALER_PRINT("\n Enable frame buffer less mode!!\n");
   }
   else
   {
        g_ScalerInfo[u8SCIndex1].u8SiFbMode = 1;

        if(u8SCIndex2 != SC_MAX_NUMS)
            g_ScalerInfo[u8SCIndex2].u8SiFbMode = 1;

        MSSCALER_PRINT("\n Frame buffer mode\n");
   }
}

static void msDrvScSetMemBalance(BYTE ucInputPort, BYTE ucWinIndex)
{
#if ((CHIP_ID==MST9U2) && ((MEMORY_MAP == MMAP_256_256MB) || (MEMORY_MAP == MMAP_512_512MB)))
    if( IsWindowLRCross(ucWinIndex)
     && (
            ( HBR2Flag(ucInputPort) && IsWindowLRCenter(ucWinIndex) && (!IsWindowRot(ucWinIndex)) && IsWindowLRCapCenter(ucWinIndex) )
         || IsWindowDI(ucWinIndex)
        )
    )
        g_DisplayWindow[ucWinIndex].bDwMemBalance = TRUE;
    else
        g_DisplayWindow[ucWinIndex].bDwMemBalance = FALSE;

#elif((CHIP_ID==MST9U3) && ENABLE_4K_FBL_MODE)
    if( IsWindowLRCross(ucWinIndex)
     && ( HBR2Flag(ucInputPort) && IsWindowLRCenter(ucWinIndex) && (!IsWindowRot(ucWinIndex)) && IsWindowLRCapCenter(ucWinIndex) )

       )
    {
        g_DisplayWindow[ucWinIndex].bDwMemBalance = TRUE;
        msDrvScSetFblMode(ucWinIndex, TRUE);
    }
    else
    {
        g_DisplayWindow[ucWinIndex].bDwMemBalance = FALSE;
        msDrvScSetFblMode(ucWinIndex, FALSE);
    }
#elif 0 // set to 1 if window is LR cross and use 2 input scalers for DI
       // ((MEMORY_MAP == MMAP_128MB) || (MEMORY_MAP == MMAP_256MB) || (MEMORY_MAP == MMAP_512MB))
    ucInputPort = ucInputPort;

    if( IsWindowLRCross(ucWinIndex) && IsWindowDI(ucWinIndex) )
        g_DisplayWindow[ucWinIndex].bDwMemBalance = TRUE;
    else
        g_DisplayWindow[ucWinIndex].bDwMemBalance = FALSE;
#else
    ucInputPort = ucInputPort;
    ucWinIndex = ucWinIndex;
#endif
}

void msDrvScSetMemFreeze(BYTE ucWinIndex, BOOL bEnable)
{
    WORD    uwSCMask;
    WORD    uwSCMaskIn  = g_DisplayWindow[ucWinIndex].u16DwScMaskIn;
    WORD    uwSCMaskOut = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;

    if(bEnable) // Freeze
    {
        if(IsWindowRot90270(ucWinIndex)) // TBD - Rotate+FRC
        {
            //msDrvScMuteScreen(uwSCMaskNew, TRUE);
        }
        else //if(GetVfreq() != V_FREQ_IN(ucInputPort)) // FRC
        {
            uwSCMask = (uwSCMaskIn|uwSCMaskOut) & bSC_MAIN;
            if( uwSCMask )
            {
                msDrvSCCommonWrite(uwSCMask, ENABLE);
                msWriteBit(SC12_03+SCRegBase(GetLsbBitPos(uwSCMask)), TRUE, _BIT3);
                msDrvSCCommonWrite(uwSCMask, DISABLE);
            }
            uwSCMask = (uwSCMaskIn|uwSCMaskOut) & bSC_SUB;
            if( uwSCMask )
            {
                msDrvSCCommonWrite(uwSCMask, ENABLE);
                msWriteBit(SC12_83+SCRegBase(GetLsbBitPos(uwSCMask)), TRUE, _BIT3);
                msDrvSCCommonWrite(uwSCMask, DISABLE);
            }
        }
    }
    else // Release
    {
        if(IsWindowRot90270(ucWinIndex)) // TBD - Rotate+FRC
        {
            //ForceDelay1ms(GetVSyncTime(ucInputPort)*4); // wait garbaged data was overwriten - rotate 3 frame mode
            //msDrvScMuteScreen(uwSCMaskOld, bEnFreeRun);
        }
        else //if(GetVfreq() != V_FREQ_IN(ucInputPort)) // FRC
        {
            //ForceDelay1ms(GetVSyncTime(MapWin2Port(ucWinIndex))*2); // wait garbaged data was overwriten
            uwSCMask = (uwSCMaskIn|uwSCMaskOut) & bSC_MAIN;
            if( uwSCMask )
            {
                msDrvSCCommonWrite(uwSCMask, ENABLE);
                msWriteBit(SC12_03+SCRegBase(GetLsbBitPos(uwSCMask)), FALSE, _BIT3);
                msWriteBit(SC12_0F+SCRegBase(GetLsbBitPos(uwSCMask)), FALSE, _BIT3);
                msDrvSCCommonWrite(uwSCMask, DISABLE);
            }

            uwSCMask = (uwSCMaskIn|uwSCMaskOut) & bSC_SUB;
            if( uwSCMask )
            {
                msDrvSCCommonWrite(uwSCMask, ENABLE);
                msWriteBit(SC12_83+SCRegBase(GetLsbBitPos(uwSCMask)), FALSE, _BIT3);
                msWriteBit(SC12_8F+SCRegBase(GetLsbBitPos(uwSCMask)), FALSE, _BIT3);
                msDrvSCCommonWrite(uwSCMask, DISABLE);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////
/////////////////////// scaler window IO setup  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScWindowSetup( BYTE SCDetWin, BOOL bSkipAutoWin )
{
    BYTE InputPort = MapScaler2Port(SCDetWin);
    BYTE u8WinIndex = MapScaler2Win(SCDetWin);
    ST_WINDOW_INFO sAutoWin;
    BYTE bSupportTiming, ucSCIndex;
    WORD uwScMaskOri;

    if( INPUT_IS_FAKE_DUAL_S(InputPort) && !(INPUT_IS_FAKE_DUAL_MONO(InputPort)) ) // Note: msAPIWinReSetup() should pass-in Master scaler.
    {
        sAutoWin = mStar_ReadAutoWindow(InputPort); // use detecting sc to get auto win
        g_InputPort[InputPort].sIPAutoWin = sAutoWin;
        msDrvScSetStatus( (1<<SCDetWin), SC_STATUS_OUTPUT_SETUP);
        return;
    }

    /////////////////////////////////////////////////////
    //// API setting IO window based on mode index, asp mode //
    /////////////////////////////////////////////////////
    // 1. Get IO setting of window
    if(bSkipAutoWin)
    {
        sAutoWin = g_InputPort[InputPort].sIPAutoWin;
    }
    else
    {
        if(g_ScalerInfo[SCDetWin].u8SiIPRstCntr!=0) // wait IP stable
            return;

        sAutoWin = mStar_ReadAutoWindow(InputPort); // use detecting sc to get auto win
        g_InputPort[InputPort].sIPAutoWin = sAutoWin;
    }

    if(DualInFlag(InputPort)) // flag set by DP combo ??
        sAutoWin.width *= 2;

    if( InterlaceModeFlag(InputPort) && !IsWindowDI(u8WinIndex) )
        sAutoWin.height /= 2;

	
    g_DisplayWindow[u8WinIndex].sDwAutoWin = sAutoWin;
    g_DisplayWindow[u8WinIndex].sDwDisplayWin = g_DisplayWindow[u8WinIndex].sDwDisplayWinOri;

    // call back to custom  appWindow, returned capture window x, width should be even  when HBR2 ????
    if(g_bAPPWindow_Init&&CB_FuncList[eCB_SetupWindow]!=NULL)
        ((fpSetupWindow)CB_FuncList[eCB_SetupWindow])(u8WinIndex, g_InputPort[InputPort].sIPInputTiming, sAutoWin, &bSupportTiming, &g_DisplayWindow[u8WinIndex].sDwCaptureWin, &g_DisplayWindow[u8WinIndex].sDwDisplayWin);

    if(!bSupportTiming )
        SrcFlags(InputPort) |= bUnsupportMode;

    if(!UnsupportedModeFlag(InputPort))
    {
        if(g_bAPPWindow_Init&&CB_FuncList[eCB_SetupMPQ]!=NULL)
        ((fpSetupMPQ)CB_FuncList[eCB_SetupMPQ])(u8WinIndex, g_InputPort[InputPort].sIPInputTiming, sAutoWin, &g_DisplayWindow[u8WinIndex].sDwPQMode);
    }
    msDrvScSetMemLRCenterCross(u8WinIndex);
    msDrvScSetMemLRCapCenter(u8WinIndex);
    msDrvScSetMemBalance(InputPort, u8WinIndex);

#if 0 // now always 2p
    msDrvScDispHStartUpdate();
#endif

    // 2. msArrangeWin2SC() ==> arrange IO from Win to SC, May need one more SC of window due to HBR2 detected!!!
    // 3. need refine to update status to multiple SC if multi SC assigned to 1 win
    if( UnsupportedModeFlag(InputPort) )
    {
        msDrvScSetStatus(g_DisplayWindow[u8WinIndex].u16DwScMaskOut, SC_STATUS_WINDOW_UNSUPPORT);
    }
    else
    {
#if INPUT_SAME_OUT
        if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetCurFrameMode]!=NULL)       //20140219 Rick add for input lag
        ((fpCustomerSetCurFrameMode)CB_FuncList[eCB_CustomerSetCurFrameMode])(u8WinIndex);
#endif
	
        uwScMaskOri = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;
        msDrvScPostDispatch(u8WinIndex, sAutoWin);

        msDrvScMemFakeCapWin(u8WinIndex, g_DisplayWindow[u8WinIndex].bDwFakeCapWinEnable);

        if( INPUT_IS_FAKE_DUAL(InputPort) && !(INPUT_IS_FAKE_DUAL_MONO(InputPort)) )
            msDrvScSetStatus( (1<<SCDetWin), SC_STATUS_OUTPUT_SETUP );
        else
            msDrvScSetStatus( (g_DisplayWindow[u8WinIndex].u16DwScMaskIn | g_DisplayWindow[u8WinIndex].u16DwScMaskOut), SC_STATUS_OUTPUT_SETUP);
        // check if display window position/size changed due to ..., e.g. asp ratio, and then update the display timing gen
        if( (g_DisplayWindow[u8WinIndex].sDwDisplayWin.x != g_DisplayWindow[u8WinIndex].sDwDisplayWinOri.x)
          ||(g_DisplayWindow[u8WinIndex].sDwDisplayWin.y != g_DisplayWindow[u8WinIndex].sDwDisplayWinOri.y)
          ||(g_DisplayWindow[u8WinIndex].sDwDisplayWin.width!= g_DisplayWindow[u8WinIndex].sDwDisplayWinOri.width)
          ||(g_DisplayWindow[u8WinIndex].sDwDisplayWin.height!= g_DisplayWindow[u8WinIndex].sDwDisplayWinOri.height) )
        {
            for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
            {
                if( (uwScMaskOri|g_DisplayWindow[u8WinIndex].u16DwScMaskOut) & (BIT0<<ucSCIndex) )
                {
                    appmStar_SetPanelTiming(ucSCIndex);
                }
            }
        }
        else if(uwScMaskOri - (uwScMaskOri&g_DisplayWindow[u8WinIndex].u16DwScMaskOut)) // scaler removed due to bDwForce1P2ScOP change
        {
            for( ucSCIndex = SCL0_SUB ; ucSCIndex < SC_MAX_NUMS-1 ; ucSCIndex++ )
            {
                if( (uwScMaskOri - (uwScMaskOri&g_DisplayWindow[u8WinIndex].u16DwScMaskOut)) & (BIT0<<ucSCIndex) )
                {
                    appmStar_SetPanelTiming(ucSCIndex);
                }
            }
        }
    }

#if ENABLE_SUBSC_HOLLOW_OUT
    if(!bSkipAutoWin) // for WinResetup NoMute Flow
        msDrvSubSCHollowOut(msReadByte(SCL0_00_80)&BIT0,msReadByte(SCL0_00_80)&BIT1);
#endif
}

EN_SCALER_DISP_STATUS msDrvOsdInit( BYTE SCDetWin )
{
    SCDetWin = SCDetWin;

#if (!ZUI)
    if (g_stMenuFlag.bOsdLoadData)
    {
        g_stMenuFlag.bOsdLoadData=0;
        Menu_InitOSD();
    }
    //Menu_InitAction(); // initial menu osd state
    return SC_STATUS_WINDOW_SETUP;

#else

    return SC_STATUS_WINDOW_SETUP;

#endif

}

/////////////////////////////////////////////////////////////////////////
///////////////////////// Scaler State  handler  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
static void msDrvSMScaler( void )
{
    BYTE SCDetWin;
    static BYTE ucPrevStatus[SC_MAX_NUMS];

    for( SCDetWin=SCL0_SUB ; SCDetWin < SC_MAX_NUMS ; SCDetWin++ )
    {
        // print status for debug
        if(g_ScalerInfo[SCDetWin].eSiDispStatus != ucPrevStatus[SCDetWin])
        {
            ucPrevStatus[SCDetWin] = g_ScalerInfo[SCDetWin].eSiDispStatus;
            //MSSCALER_PRINT("[SMScaler] SC: %d, status: %d \n",SCDetWin, g_ScalerInfo[SCDetWin].eSiDispStatus);
        }

        switch(g_ScalerInfo[SCDetWin].eSiDispStatus)
        {
            case SC_STATUS_IDLE:
            case SC_STATUS_HOLLOW:
                continue;

            case SC_STATUS_FREERUN:
                break;

            case SC_STATUS_UI_SETUP:
                g_ScalerInfo[SCDetWin].eSiDispStatus = msDrvOsdInit(SCDetWin);
                break;

            case SC_STATUS_WINDOW_SETUP:
                MENU_LOAD_START();
                msDrvScWindowSetup(SCDetWin, FALSE);
                MENU_LOAD_END();
                break;

            case SC_STATUS_OUTPUT_SETUP:    // msScalerSetup()
            case SC_STATUS_WINDOW_UNSUPPORT:
                mStar_ModeHandler(SCDetWin);
                break;

            case SC_STATUS_OUTPUT_SETUPOK:  // msScalerSetup()
            default:
                 break;
        }
    }
}


static EN_WINDOW_STATUS msDrvSMWindowCheck(BYTE u8IdxWin)
{
    EN_WINDOW_STATUS eStatus = g_DisplayWindow[u8IdxWin].eDwStatus;
    WORD u16SCMap = (g_DisplayWindow[u8IdxWin].u16DwScMaskIn | g_DisplayWindow[u8IdxWin].u16DwScMaskOut);
    BYTE u8ChkFreerun;
    WORD u16SCMapChg = 0;
    BYTE u8IdxSC = 0;

    if( eStatus == WIN_STATUS_FREERUN )    // WIN_STATUS_FREERUN
        u8ChkFreerun = 0;
    else                                    // WIN_STATUS_OUTPUT_DISPLAYOK
        u8ChkFreerun = 1;

    while(u16SCMap != 0)
    {
        if( u16SCMap & BIT0 )
        {
            if( (u8ChkFreerun && (g_ScalerInfo[u8IdxSC].eSiDispStatus != SC_STATUS_OUTPUT_SETUPOK))
             || (!u8ChkFreerun && (g_ScalerInfo[u8IdxSC].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK)) )
                u16SCMapChg |= (1<<u8IdxSC);
        }

        u16SCMap >>= 1;
        u8IdxSC++;
    }

    u16SCMap = (g_DisplayWindow[u8IdxWin].u16DwScMaskIn | g_DisplayWindow[u8IdxWin].u16DwScMaskOut);

    if( u16SCMap == u16SCMapChg )
    {
        if( eStatus == WIN_STATUS_FREERUN )
        {
            if(UnsupportedModeFlag(MapWin2Port(u8IdxWin)))
            {
                //Menu_InitAction(); // initial menu osd state
                eStatus = WIN_STATUS_OUTPUT_DISPLAYOK;				
            }
            else
            {
                if( g_DisplayWindow[u8IdxWin].u8DwUnmuteCntr==0 )
                {
                    if(g_bAPPWindow_Init&&CB_FuncList[eCB_SetupUserPref]!=NULL)
                        ((fpSetupUserPref)CB_FuncList[eCB_SetupUserPref])(u8IdxWin);
		      WAIT_V_OUTPUT_BLANKING_START();			
                    msAPIWinFreerun(u8IdxWin, FALSE);
                    if( u16SCMap &  bSCL0_MAIN)
                    {
#if FPLL_TUNE_SW_HTT
                        if( (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_NO_LOCK) && (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_FBL) && (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_FBL_FLOCK) )
                            msDrvResetOutputHtt();
#elif !FPLL_TUNE_SW_MODE
                        if( (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_NO_LOCK) && (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_FBL) && (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_FB_FLOCK) && (g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_FBL_FLOCK)
                     #if (INPUT_SAME_OUT)//&&(!QHD_2560x1440_FreeSync_FrameSyncMode))
                           &&(g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_FB_FAST_LOCK)&&(g_ScalerInfo[SCL0_MAIN].u8SiSyncMode!=FLM_FB_NEW_LOCK)
			#endif		 
			      )
                            msDrvResetDClkPLL();
#endif
                    }
#if 0//PowerSequence_New
                 {
                 #if ENABLE_DAISY_CHAIN
                     if(CURRENT_INPUT_IS_DC_RX())
                     {
                         if(!PowerSavingFlag && (!g_u8DCMSTFlag || (!SyncLossState(InputPort) && InputTimingStableFlag(InputPort))))
                         {
                             Power_TurnOnPanel(); // turn on panel
                             g_u8DCMSTFlag = 0;
                         }
                     }
                     else
                     {
                         Power_TurnOnPanel(); // turn on panel
                     }
                 
                 #else
                 
                   if(!bPanelOnFlag)
                   {
                     Power_TurnOnPanel(); // turn on panel
                   }
                   else   
                 #endif
                    #if (VBY1_PowerSequence)
                     if(!BackLightOnFlag)
                     {
                       printMsg("Set_BackLightOnFlag");
                       Set_BackLightOnFlag();
                       g_stMenuFlag.bAnyWinDisplayChange=1;
                     }
                    #else
                      if((!BacklightFlag))
                      {
                         printMsg("BACKLIGHTT_FLAG");
                         hw_SetBlacklit();	
                         g_stMenuFlag.bAnyWinDisplayChange=1;
                      }
                    #endif
                 }
#endif
                     printMsg("33333333333333");
                       if((!OSDActionFlag))
                       g_stMenuFlag.bAnyWinDisplayChange=1;

		      if(BlackScreenFlag&&(!OSDActionFlag))
		      	{
                    #if FreeSync_Not_Support_Info
                      if(!FreeSyncMsgFlag)
                    #endif		      	
                       mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF); 
		      	}
			
					
                }
                else if( g_DisplayWindow[u8IdxWin].u8DwUnmuteCntr==1 ) // unmute timeout
                {
                    g_DisplayWindow[u8IdxWin].u8DwUnmuteCntr = 0;
                #if ENABLE_FACTORY_SSCADJ 
                    mStar_SetPanelSSCFactor(USER_PREF_SSC_FREQUENCY,USER_PREF_SSC_PERMILLAGE_ADJUST);
                #else
                    mStar_SetPanelSSCFactor(PANEL_SSC_MODULATION_DEF, PANEL_SSC_PERCENTAGE_DEF);
		  #endif		
                    WAIT_V_OUTPUT_BLANKING_START();
                    msDrvScMuteScreen(g_DisplayWindow[u8IdxWin].u16DwScMaskOut, FALSE);
                
                    eStatus = WIN_STATUS_OUTPUT_DISPLAYOK;


#if 1//PowerSequence_New
                 {
                 #if ENABLE_DAISY_CHAIN
                     if(CURRENT_INPUT_IS_DC_RX())
                     {
                         if(!PowerSavingFlag && (!g_u8DCMSTFlag || (!SyncLossState(InputPort) && InputTimingStableFlag(InputPort))))
                         {
                             Power_TurnOnPanel(); // turn on panel
                             g_u8DCMSTFlag = 0;
                         }
                     }
                     else
                     {
                         Power_TurnOnPanel(); // turn on panel
                     }
                 
                 #else
                 
                   if(!bPanelOnFlag)
                   {
                     Power_TurnOnPanel(); // turn on panel
                   }
                   else   
                 #endif
                    #if (VBY1_PowerSequence)
                     if(!BackLightOnFlag)
                     {
                       printMsg("Set_BackLightOnFlag");
                       Set_BackLightOnFlag();
                       g_stMenuFlag.bAnyWinDisplayChange=1;
                     }
                    #else
                      if((!BacklightFlag))
                      {
                         printMsg("BACKLIGHTT_FLAG");
                         hw_SetBlacklit();	
                         g_stMenuFlag.bAnyWinDisplayChange=1;
                      }
                    #endif
                 }
#endif

           #if 1//PowerSequence_New	
           #if AudioFunc
               //tmp need update !! audio out decided by AP
             #if Auto_Switch_AudioSource
               mStar_SettingAudioFucntion( MapScaler2Port(u8IdxWin), u8IdxWin );
	      #else	   
               mStar_SettingAudioFucntion( USER_PREF_INPUT_TYPE, u8IdxWin );
	      #endif	  
           #endif
           #endif
            

                   printMsg("4444444444444444");
		      if(BlackScreenFlag&&(!OSDActionFlag))
		      	{
                    #if FreeSync_Not_Support_Info
                      if(!FreeSyncMsgFlag)
                    #endif		      	
                       mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF); 
		      	}

  
                }
				
            }
        }
        else
        {
            msAPIWinFreerun(u8IdxWin, TRUE);
        #if 0//(!ZUI)
            Menu_InitAction(); // initial menu osd state
        #endif
            eStatus = WIN_STATUS_FREERUN;
        }
    }

    return eStatus;
}

static void msDrvSMWindow( void )
{
    BYTE u8IdxWin;
    static BYTE ucPrevStatus[DISPLAY_MAX_NUMS];

    for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
    {
        g_bPQ_Adaptive_On[u8IdxWin] = g_DisplayWindow[u8IdxWin].u8DwEnable;

        if( (!g_DisplayWindow[u8IdxWin].u8DwEnable) || IsWindowDirectDisplay(u8IdxWin) )
            continue;

        // print status for debug
        if(g_DisplayWindow[u8IdxWin].eDwStatus != ucPrevStatus[u8IdxWin])
        {
            ucPrevStatus[u8IdxWin] = g_DisplayWindow[u8IdxWin].eDwStatus;
            //MSSCALER_PRINT("[SMWindow] Win: %d, status: %d \n",u8IdxWin, g_DisplayWindow[u8IdxWin].eDwStatus);
        }

        switch(g_DisplayWindow[u8IdxWin].eDwStatus)
        {
            case WIN_STATUS_FREERUN:
            case WIN_STATUS_OUTPUT_DISPLAYOK:
                g_DisplayWindow[u8IdxWin].eDwStatus = msDrvSMWindowCheck(u8IdxWin);
                break;

            case WIN_STATUS_IDLE:
            default:
                break;
        }

        // move to the beginning of for loop
        //if(g_DisplayWindow[u8IdxWin].eDwStatus == WIN_STATUS_OUTPUT_DISPLAYOK)
        //    g_bPQ_Adaptive_On[u8IdxWin] = 1;
        //else
        //    g_bPQ_Adaptive_On[u8IdxWin] = 0;

#if !UseVsyncINT
        msPQ_AdaptiveTuning(u8IdxWin);
#endif
    }
}

static void msDrvPLLTuning(void)
{
    static BYTE u8LPLLTimer = 0;
    BYTE u16Status = msReadByte(REG_LPLL_54);
    DWORD u32FactorNew;

    if( ++u8LPLLTimer < g_u16FPLLTuneDelay )
        return;
    u8LPLLTimer = 0;

    if( g_u16FPLLTuneTimes )
    {
        if( (--g_u16FPLLTuneTimes) == 0 )
        {
            MSSCALER_PRINT("FPLL_T No Lock\n");
        }
    }

    if( !g_u16FPLLTuneTimes )
        return;

    if( u16Status == 0xF4 )
    {
        MSSCALER_PRINT("FPLL_T Lock\n");
        g_u16FPLLTuneTimes = 0;
        return;
    }

    u32FactorNew = msRead3Byte(REG_LPLL_50);

    if( u32FactorNew > g_u32FPLLTuneMax )
        u32FactorNew = g_u32FPLLTuneMax;

    if( u32FactorNew < g_u32FPLLTuneMin )
        u32FactorNew = g_u32FPLLTuneMin;

    msWrite3Byte(REG_LPLL_1E ,u32FactorNew);
}

/////////////////////////////////////////////////////////////////////////
////////////////////////////// Event  handler  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
static void msDrvEventHandler( void )
{
    BYTE    enInputPort, u8SCIndex, u8WinIndex;

    if(g_u8SCTimerFlag10ms==1)
    {
        for(enInputPort = First_Input_Port ; enInputPort < Input_Nums ; enInputPort++)
        {
            if(ModeDetectCounter(enInputPort))
            {
                if(ModeDetectCounter(enInputPort) > 10)
                    ModeDetectCounter(enInputPort) -= 10;
                else
                    ModeDetectCounter(enInputPort) = 0;
            }

            if(g_InputPort[enInputPort].sIPHPDCntr.u16HPDCnt)
            {
                if(g_InputPort[enInputPort].sIPHPDCntr.u16HPDCnt > 10)
                    g_InputPort[enInputPort].sIPHPDCntr.u16HPDCnt -= 10;
                else
                    g_InputPort[enInputPort].sIPHPDCntr.u16HPDCnt = 0;


                if(g_InputPort[enInputPort].sIPHPDCntr.u16HPDCnt==g_InputPort[enInputPort].sIPHPDCntr.u16HPDClkLow)
		#if 0//AIO_Board			
                    msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[enInputPort].eIPMux, DISABLE);
		#else
                    msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[enInputPort].eIPMux, ENABLE);
		#endif
                else if(g_InputPort[enInputPort].sIPHPDCntr.u16HPDCnt==g_InputPort[enInputPort].sIPHPDCntr.u16HPDClkHigh)
                    msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[enInputPort].eIPMux, ENABLE);
                else if(g_InputPort[enInputPort].sIPHPDCntr.u16HPDCnt==0x00)
                    msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[enInputPort].eIPMux, ENABLE);
            }
        }

        for(u8SCIndex = SCL0_SUB; u8SCIndex < SC_MAX_NUMS; u8SCIndex++)
        {
            if(g_ScalerInfo[u8SCIndex].u8SiIPRstCntr)
            {
                if(g_ScalerInfo[u8SCIndex].u8SiIPRstCntr > 10)
                    g_ScalerInfo[u8SCIndex].u8SiIPRstCntr -= 10;
                else
                    g_ScalerInfo[u8SCIndex].u8SiIPRstCntr = 0;
            }

#if 0 // IP2 HVSD Counter Status Polling
{
            #define HVSDTol   8

            if( (g_ScalerInfo[u8SCIndex].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK)
             && (!UnsupportedModeFlag(MapScaler2Port(u8SCIndex)))
             && (g_ScalerInfo[u8SCIndex].sSiIPCaptureWin.width)
             && (g_ScalerInfo[u8SCIndex].sSiSCFIpmWindow.width))
            {
                BYTE u8SCPort = MapScaler2Port(u8SCIndex);
                BYTE u8SCWin = MapScaler2Win(u8SCIndex);
                BYTE u8HSDTol = HVSDTol + ((HBR2Flag(u8SCPort)&&IsWindowMemBal(u8SCWin))?DFT_HBR2_GUARDBAND:0);
                BYTE u8VSDTol = HVSDTol + ((IsWindowRot90270(u8SCWin))?8:0);
                BYTE u8VSDFactor = (InterlaceModeFlag(u8SCPort) && (msDrvIP1ReadSyncStatus(u8SCIndex)&INTM_B))?2:1;
                BYTE u8IpmDIFactor = (InterlaceModeFlag(u8SCPort) && !IsWindowDI(u8SCWin))?2:1;
                if( (labs(msDrvIP2ReadHSDCnt(u8SCIndex) - g_ScalerInfo[u8SCIndex].sSiSCFIpmWindow.width)>u8HSDTol)
                  ||(labs(msDrvIP2ReadVSDCnt(u8SCIndex)*u8VSDFactor - g_ScalerInfo[u8SCIndex].sSiSCFIpmWindow.height*u8IpmDIFactor)>u8VSDTol)
                  ||(msDrvIP2ReadHSDCnt(u8SCIndex) > msDrvIP1ReadImageWidth(u8SCIndex))
                  ||(msDrvIP2ReadVSDCnt(u8SCIndex) > msDrvIP1ReadImageHeight(u8SCIndex)) )
                {
                    MSSCALER_PRINT( "HSD/VSD cnt changed_1, SC:%d, H/Vtol:%d/%d,  VSDFac:%d\n",u8SCIndex,u8HSDTol,u8VSDTol,u8VSDFactor);
                    ForceDelay1ms(100); // re-set capture size, need at least 1 frame to make H/VSD counter stable
                    u8VSDFactor = (InterlaceModeFlag(u8SCPort) && (msDrvIP1ReadSyncStatus(u8SCIndex)&INTM_B))?2:1;
                    if( (labs(msDrvIP2ReadHSDCnt(u8SCIndex) - g_ScalerInfo[u8SCIndex].sSiSCFIpmWindow.width)>u8HSDTol)
                      ||(labs(msDrvIP2ReadVSDCnt(u8SCIndex)*u8VSDFactor - g_ScalerInfo[u8SCIndex].sSiSCFIpmWindow.height*u8IpmDIFactor)>u8VSDTol)
                      ||(msDrvIP2ReadHSDCnt(u8SCIndex) > msDrvIP1ReadImageWidth(u8SCIndex))
                      ||(msDrvIP2ReadVSDCnt(u8SCIndex) > msDrvIP1ReadImageHeight(u8SCIndex)) )
                    {
                        MSSCALER_PRINT( "HSD/VSD cnt changed_2\n");
                        MSSCALER_PRINT( "HSD:%x, IPM_H:%x, Cap_H:%x\n",msDrvIP2ReadHSDCnt(u8SCIndex),g_ScalerInfo[u8SCIndex].sSiSCFIpmWindow.width,msDrvIP1ReadImageWidth(u8SCIndex));
                        MSSCALER_PRINT( "VSD:%x, IPM_V:%x, Cap_V:%x, VSDFac:%d, IpmDIFac:%d\n",msDrvIP2ReadVSDCnt(u8SCIndex)*u8VSDFactor,g_ScalerInfo[u8SCIndex].sSiSCFIpmWindow.height*u8IpmDIFactor,msDrvIP1ReadImageHeight(u8SCIndex),u8VSDFactor,u8IpmDIFactor);
                        if( u8SCIndex & BIT0)//main window
                            msDrvSoftwareReset( RST_IP_MAIN,  u8SCIndex);
                        else
                            msDrvSoftwareReset( RST_IP_SUB, u8SCIndex);
                    }
                }
            }
}
#endif

        }

        for(u8WinIndex = DISPLAY_WIN1; u8WinIndex < DISPLAY_MAX_NUMS; u8WinIndex++)
        {
            if(g_DisplayWindow[u8WinIndex].u8DwUnmuteCntr)
            {
                if(g_DisplayWindow[u8WinIndex].u8DwUnmuteCntr > 10)
                    g_DisplayWindow[u8WinIndex].u8DwUnmuteCntr -= 10;
                else
                    g_DisplayWindow[u8WinIndex].u8DwUnmuteCntr = 1; // keep 1 for msDrvSMWindow state
            }
        }

        msDrvPLLTuning();

        g_u8SCTimerFlag10ms = 0;
    }

#if PANEL_VBY1()
    if(g_u8VBY1CheckFlag==1)
    {
        g_u8VBY1CheckFlag = 0;
        if(bPanelOnFlag == 1) // prevent doing training before i_gen_tuning
        {
                #if (PanelType == PanelLSM315HP01)//(MS_PANEL_TYPE==PanelCMI40UltraHD||MS_PANEL_TYPE==PanelTPVLVM400K1QS1AD82P0X ||MS_PANEL_TYPE==PanelTPT315B5HP01)
                  #if ( CHIP_ID == MST9U3)
                  #define VX1_HPDN_STATUS     (msReadByte(REG_140619)&BIT7)
                  #define VX1_LOCKN_STATUS    (msReadByte(REG_140619)&BIT6)
                  #else
                   #define VX1_HPDN_STATUS     (msReadByte(REG_1407D1)&BIT7)
                   #define VX1_LOCKN_STATUS    (msReadByte(REG_1407D1)&BIT6)
                  #endif

                  #if SET_BLIT_WITH_OSDFreeSyncSwitchAction
                   if(bOSDFreeSyncOnOffFlag)
        	         {
            	       	MSSCALER_PRINT("\r\n bOSDFreeSyncOnOffFlag Lock Panel Again\r\n");
            			msDrvCheckVBY1LockN(1);

            			if((VX1_LOCKN_STATUS||VX1_HPDN_STATUS))
            			{
            				MSSCALER_PRINT("\r\n Lock Again NG\r\n");
            			}
            			else
            			{
            				bOSDFreeSyncOnOffFlag=FALSE;
            			}
        		     }
    		      else
                  #endif
                    if(VX1_LOCKN_STATUS||VX1_HPDN_STATUS)
                     {
                         MSSCALER_PRINT("\r\n AAAA----------VB1_LOCK_NG\r\n");
                         Power_TurnOffPanel();
                         ForceDelay1ms( 600 );
                         Power_TurnOnPanel();
                     }
           #else
             msDrvCheckVBY1LockN(1);
           #endif
        }
    }
#endif

}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////  SC handler  /////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msDrvScHandler( void )
{
    // State Machine Input
    msDrvSMInput();

    // State Machine Scaler
    msDrvSMScaler();

    // State Machine Window
    msDrvSMWindow();

    // Event Handler
    msDrvEventHandler();
}
///////////////////////////////////////////////////////////////////////////////
// <Function>: msDrvSoftwareReset
//
// <Description>: MST chip software reset.
//
// <Parameter>:  -  <Flow>  -  <Description>
//-----------------------------------------------------------------------------
// ucParam       -    In    -  Software reset parameter
///////////////////////////////////////////////////////////////////////////////
void msDrvSoftwareReset(DWORD ucParam, BYTE SCDetWin)
{
    BYTE u8BKIDCLK;
    WORD ucParam1, ucParam2;

    ucParam1 = (BYTE)(ucParam & 0xFF);
    ucParam2 = (WORD)(ucParam & 0xFFFF00) >> 8;


    if( ucParam1 )
    {
        ForceDelay1ms(2);
        SC0_SCALER_RESET(ucParam1, SCRegBase(SCDetWin));
        ForceDelay1ms(2);

        u8BKIDCLK = msDrvGetIPCLKMux(SCDetWin);
        msDrvSetIPCLKMux(ID_CLK_ODCLK, SCDetWin);
        msDrvSetIPCLKMux(u8BKIDCLK, SCDetWin);

        SC0_SCALER_RESET(0x00, SCRegBase(SCDetWin));
        ForceDelay1ms(2);
        g_ScalerInfo[SCDetWin].u8SiIPRstCntr = 230; // Vin 24Hz ~ 42ms *5 frame = 210ms + 20 margin
    }

    if( ucParam2 )
    {
        msWrite2Byte(REG_10250E, ucParam2);//(BK1_0D, ucParam2);
        msWrite2Byte(REG_10250E, 0x00);
    }

}


//
void msDrvSetScalingPreVCtrlLineOut(BYTE SCDetWin, WORD u16LineCount, Bool bEnable)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    BYTE InputPort = MapScaler2Port(SCDetWin);
    WORD u16OutputLineCount = 0x00;
    BYTE u8Divider;



    if (bEnable)
    {
        u8Divider = InterlaceModeFlag(InputPort) ?(2) :(1);
        u16OutputLineCount = BIT15;
        u16OutputLineCount |= (u16LineCount/u8Divider)& 0x1FFF;
    }

    if( SCDetWin & _BIT0 )
    {
        msWrite2Byte(SC02_6E+u32SCRegBase, u16OutputLineCount);
    }
    else
    {
        msWrite2Byte(SC04_6E+u32SCRegBase, u16OutputLineCount);
    }

}

/*
 bEnable : Enable input line count.
 bUserMode : 1 -> the input line count will ref u32UserLineCount
                   0 -> the input line count will ref V capture win
*/
void msDrvSetScalingPreVCtrlLineIn( BYTE SCDetWin, WORD u16UserLineCount, Bool bEnable)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    BYTE InputPort = MapScaler2Port(SCDetWin);

    WORD u16InputLineCount = 0x00;

    if (bEnable)
    {
        u16InputLineCount = BIT15;

        if (InterlaceModeFlag(InputPort))
        {
            u16InputLineCount |= BIT14;
            u16InputLineCount |= (u16UserLineCount/2) & 0x1FFF;
        }
    }

    if( SCDetWin & _BIT0 )
    {
        msWrite2Byte(SC02_6C+u32SCRegBase, u16InputLineCount);
    }
    else
    {
        msWrite2Byte(SC04_6C+u32SCRegBase, u16InputLineCount);
    }

}

#define HD_ACT_TH 10
#define HDSDD_SPEED 1
#define HDSDD_DIV 0
#define HD_SPEED 2
#define SD_SPEED 8
void msDrv_SDHD_DETECT(BYTE SCDetWin)
{
    MS_U8 u8Value;
    MS_U16 u16MinPixel, u16MaxPixel, MaxMinPixel;
    MS_U32 HDcnt_Act, HDcnt_NonAct;
    MS_U32 SDcnt_Act, SDcnt_NonAct;
    static MS_U16 HD_ConfirmCnt = 0xFF; // 0x7FFF == 0
    static MS_U8 u8PrevLuma = 0;
    static MS_U16 u16PrevMinPixel = 0;
    static MS_U16 u16PrevMaxPixel = 0;
    MS_U32 HD_ConfirmCnt_Reg;
    MS_U16 HDSDD_Det_threshold, HDSDD_Det_offset;
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    //BYTE InputPort = MapScaler2Port(SCDetWin);

    HDcnt_Act = ((DWORD)msRead2Byte(SC02_EA+u32SCRegBase) << 16) + msRead2Byte(SC02_E8+u32SCRegBase);
    SDcnt_Act = ((DWORD)msRead2Byte(SC02_EE+u32SCRegBase) << 16) + msRead2Byte(SC02_EC+u32SCRegBase);
    HDcnt_NonAct =  ((DWORD)msRead2Byte(SC02_F2+u32SCRegBase) << 16) + msRead2Byte(SC02_F0+u32SCRegBase);
    SDcnt_NonAct =  ((DWORD)msRead2Byte(SC02_F6+u32SCRegBase) << 16) + msRead2Byte(SC02_F4+u32SCRegBase);
    HDSDD_Det_offset = (WORD)msRead2Byte(SC22_FE+u32SCRegBase)&0xFFFF;

    MSSCALER_PRINT("====HDcnt_Act===%x\n",HDcnt_Act);
    MSSCALER_PRINT("====SDcnt_Act===%x\n",SDcnt_Act);
    MSSCALER_PRINT("====HDcnt_NonAct===%x\n",HDcnt_NonAct);
    MSSCALER_PRINT("====SDcnt_NonAct===%x\n",SDcnt_NonAct);

    u8Value = (MS_U32)((MS_U32)u8PrevLuma * 3 + 128) >> 2;
    u8PrevLuma = u8Value;

    u16MaxPixel = msRead2Byte(SC1A_16+u32SCRegBase)& 0x00FF;
    u16MaxPixel = (u16PrevMaxPixel * 3 + u16MaxPixel) >> 2;
    u16PrevMaxPixel = u16MaxPixel;

    u16MinPixel = (msRead2Byte(SC1A_16+u32SCRegBase)&0xFF00) >> 8;
    u16MinPixel = (u16PrevMinPixel * 3 + u16MinPixel) >> 2;
    u16PrevMinPixel = u16MinPixel;

    MaxMinPixel = u16MaxPixel - u16MinPixel;
    MSSCALER_PRINT("===u8MinPixel=%x==\n",u16MinPixel);
    MSSCALER_PRINT("===MaxMinPixel=%x==\n",MaxMinPixel);


    if(u8Value >= 0xFF)
        u8Value = 0xFF;
    MSSCALER_PRINT("===AvgLuma=%x==\n",u8Value);

    HDSDD_Det_threshold = HDSDD_Det_offset + ((MaxMinPixel * 6) >> 2) + (((WORD)u8Value * 5) >> 2);
    MSSCALER_PRINT("===HDSDD_Det_threshold=%x==\n",HDSDD_Det_threshold);

#if 0
    {
        if(u8Value > 0x80)
            Cur_HDSDD_Det_threshold = u16HDSDD_Det_Threshold;
        else if(u8Value > 0x40)
            Cur_HDSDD_Det_threshold = ((u16HDSDD_Det_Threshold * 3) >> 2);
        else if(u8Value > 0x20)
            Cur_HDSDD_Det_threshold = (u16HDSDD_Det_Threshold >> 1);
        else
            Cur_HDSDD_Det_threshold = (u16HDSDD_Det_Threshold >> 2);
    }
    else
        Cur_HDSDD_Det_threshold = u16HDSDD_Det_Threshold;
#endif

    if(HDSDD_Det_threshold >= 0xFFFF)
    {
        HDSDD_Det_threshold = 0xFFFF;
    }
    msWrite2ByteMask(SC02_E2, (MS_U16)HDSDD_Det_threshold, 0xFFFF);
    msWrite2ByteMask(SC02_E4, (MS_U16)(HDSDD_Det_threshold >> 1), 0xFFFF);

    HDcnt_Act = (msRead2Byte(SC02_EA+u32SCRegBase) << 16) + msRead2Byte(SC02_E8+u32SCRegBase);
    SDcnt_Act = (msRead2Byte(SC02_EE+u32SCRegBase) << 16) + msRead2Byte(SC02_EC+u32SCRegBase);
    HDcnt_NonAct =  (msRead2Byte(SC02_F2+u32SCRegBase) << 16) + msRead2Byte(SC02_F0+u32SCRegBase);
    SDcnt_NonAct =  (msRead2Byte(SC02_F6+u32SCRegBase) << 16) + msRead2Byte(SC02_F4+u32SCRegBase);

    MSSCALER_PRINT("====HD_ConfirmCnt===%x\n",HD_ConfirmCnt);

    if(HDcnt_Act > HD_ACT_TH && SDcnt_Act != 0)
    {
        if(HD_ConfirmCnt < HD_SPEED)
            HD_ConfirmCnt = 0;
        else
            HD_ConfirmCnt = HD_ConfirmCnt - HD_SPEED;
    }
    else if(SDcnt_Act != 0)  //&& HDcnt_NonAct !=0) // && (HDcnt_NonAct < 0x90000))
    {
        if(HD_ConfirmCnt > (0xFF - SD_SPEED))
            HD_ConfirmCnt = 0xFF;
        else
            HD_ConfirmCnt = HD_ConfirmCnt + SD_SPEED;

    }
    else
    {
        if(HD_ConfirmCnt < 1)
            HD_ConfirmCnt = 0;
        else
            HD_ConfirmCnt = HD_ConfirmCnt - 1;
    }

    if(HD_ConfirmCnt > 0x80)
        HD_ConfirmCnt_Reg = HD_ConfirmCnt - 0x80;
    else
        HD_ConfirmCnt_Reg = 0;

    HD_ConfirmCnt_Reg = (HD_ConfirmCnt_Reg * HDSDD_SPEED) >> HDSDD_DIV;

    if(HD_ConfirmCnt_Reg > 0x3F)
        HD_ConfirmCnt_Reg = 0x3F;

    MSSCALER_PRINT("====HD_ConfirmCnt_Reg===%x\n",HD_ConfirmCnt_Reg);

    msWrite2ByteMask(SC23_A8+u32SCRegBase, (MS_U16)HD_ConfirmCnt_Reg, 0x003F);

}

void msDrvSetChromaScalingFilter(BYTE ucWinIndex, EN_DIRECTION eDirection,
                    EN_SCALING_FILTER_MODE eMode, EN_SCALING_FILTER_SRAM eSRAM)
{
    BYTE ucSCIndex = 0;
    DWORD u32SCRegBase  = 0;
    DWORD u32SCReg = 0;
    WORD uwSCMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;

    // Check input arguments
    if ((ucWinIndex >= DISPLAY_MAX_NUMS) || (uwSCMask == 0))
    {
        MSSCALER_PRINT("[ERROR (Chroma)]: No Such Window.\n");
        return;
    }
    if (eMode > BICUBIC)
    {
        MSSCALER_PRINT("[ERROR (Chroma)]: Mode Not Supported.\n");
        return;
    }

    while(uwSCMask) // Find the scaler(s) of the display window
    {
        if(uwSCMask & BIT0) // a scaler for displaying this window is found
        {
            u32SCRegBase = SCRegBase(ucSCIndex);    // base addr of the found scaler

            if(ucSCIndex & BIT0)    // Main
            {
                if (eDirection == HORIZONTAL)   // H
                    u32SCReg = SC23_16+u32SCRegBase;
                else                            // V
                    u32SCReg = SC23_17+u32SCRegBase;
            }
            else                    // Sub
            {
                if (eDirection == HORIZONTAL)   // H
                    u32SCReg = SC23_56+u32SCRegBase;
                else                            // V
                    u32SCReg = SC23_57+u32SCRegBase;
            }

            switch (eMode)
            {
                case BYPASS:
                case BILINEAR:
                    msWriteBit(u32SCReg, 0, BIT4);
                    msWriteByteMask(u32SCReg, eMode<<1, BIT3|BIT2|BIT1);
                    break;
                case BICUBIC:
                    if (eSRAM < C_SRAM0)    // if chroma 4-tap scaling uses luma SRAM
                    {
                        msWriteBit(u32SCReg, 1, BIT4);
                        msWriteBit(u32SCReg, eSRAM, BIT5);
                    }
                    else
                    {
                        msWriteBit(u32SCReg, 0, BIT4);
                        msWriteByteMask(u32SCReg, eSRAM<<1, BIT3|BIT2|BIT1);
                    }
                    break;
                default:
                    break;
            }
        }
        ucSCIndex++;
        uwSCMask >>= 1;
    }
}

void msDrvSetLumaScalingFilter(BYTE ucWinIndex, EN_DIRECTION eDirection,
                    EN_SCALING_FILTER_MODE eMode, EN_SCALING_FILTER_SRAM eSRAM)
{
    BYTE ucSCIndex = 0;
    DWORD u32SCRegBase  = 0;
    DWORD u32SCReg = 0;
    DWORD u32SC_6Tap_Reg = 0;
    DWORD u32SC_8Tap_Reg = 0;
    WORD uwSCMask = g_DisplayWindow[ucWinIndex].u16DwScMaskOut;

    // Check input arguments
    if ((ucWinIndex >= DISPLAY_MAX_NUMS) || (uwSCMask == 0))
    {
        MSSCALER_PRINT("[ERROR (Luma)]: No Such Window.\n");
        return;
    }
    if ((eMode >= BICUBIC) && (eSRAM > Y_SRAM1))
    {
        MSSCALER_PRINT("[ERROR (Luma)]: Wrong SRAM.\n");
        return;
    }

    while(uwSCMask) // Find the scaler(s) of the display window
    {
        if(uwSCMask & BIT0) // a scaler for displaying this window is found
        {
            u32SCRegBase = SCRegBase(ucSCIndex);    // base addr of the found scaler

            if(ucSCIndex & BIT0)    // Main
            {
                if (eDirection == HORIZONTAL)   // H
                    u32SCReg = SC23_16+u32SCRegBase;
                else                            // V
                    u32SCReg = SC23_17+u32SCRegBase;

                u32SC_6Tap_Reg = SC23_26+u32SCRegBase;
                u32SC_8Tap_Reg = SC23_27+u32SCRegBase;
            }
            else                    // Sub
            {
                if (eDirection == HORIZONTAL)   // H
                    u32SCReg = SC23_56+u32SCRegBase;
                else                            // V
                    u32SCReg = SC23_57+u32SCRegBase;

                u32SC_6Tap_Reg = SC23_66+u32SCRegBase;
                u32SC_8Tap_Reg = SC23_67+u32SCRegBase;
            }

            switch (eMode)
            {
                case BYPASS:
                case BILINEAR:
                    msWriteBit(u32SCReg, 0, BIT6);
                    msWriteByteMask(u32SCReg, eMode, BIT0);
                    break;
                case BICUBIC:
                case Y_6_TAP:
                case Y_8_TAP:
                    msWriteBit(u32SCReg, 1, BIT6);
                    msWriteBit(u32SCReg, eSRAM, BIT7);
                    break;
                default:
                    break;
            }

            // Disable or Enable 6-tap or 8-tap if needed
            if (eDirection == HORIZONTAL)
            {
                if (eMode == Y_6_TAP)
                    msWriteBit(u32SC_6Tap_Reg, 1, BIT4);
                else    // make sure disable Y-6-tap (H) when not using it
                    msWriteBit(u32SC_6Tap_Reg, 0, BIT4);

                if (eMode == Y_8_TAP)
                    msWriteBit(u32SC_8Tap_Reg, 1, BIT7);
                else    // make sure disable Y-8-tap (H) when not using it
                    msWriteBit(u32SC_8Tap_Reg, 0, BIT7);
            }
            else
            {
                if (eMode == Y_6_TAP)
                    msWriteBit(u32SC_6Tap_Reg, 1, BIT5);
                else    // make sure disable Y 6-tap (V) when not using it
                    msWriteBit(u32SC_6Tap_Reg, 0, BIT5);

                if (eMode == Y_8_TAP)
                {
                    msWriteBit(u32SC_8Tap_Reg, 1, BIT3);
                }
                else    // make sure disable Y 8-tap (V) when not using it
                    msWriteBit(u32SC_8Tap_Reg, 0, BIT3);
            }

            // VERTICAL Y 8-tap need to enable 12 line buffer mode
            if ((msReadByte(SC23_27+u32SCRegBase)&BIT3) || (msReadByte(SC23_67+u32SCRegBase)&BIT3))
                msWriteBit(SC20_20+u32SCRegBase, 1, BIT4);
            else    // Disable 12 line buffer mode when nobody (main, sub) is using  VERTICAL Y 8-tap
                msWriteBit(SC20_20+u32SCRegBase, 0, BIT4);
        }
        ucSCIndex++;
        uwSCMask >>= 1;
    }
}

void msDrvScMemDrawLine(BYTE SCDetWin, DWORD u32BaseAddr, WORD u16PositionV, WORD u16PositionH, WORD u16PixCount, BYTE *pDataBGR, BYTE *pDataLSB)
{
    DWORD u32BaseAddrPix, u32PackNum, u32PackSize;
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    WORD u16LineOffset;
    BYTE *pMAddrB, *pMAddrG, *pMAddrR, *pMAddrLSB, u6PackOffset;
    BYTE u8FieldNum;
    WORD i;

    if( SCDetWin & _BIT0 )
    {
        u16LineOffset = msRead2Byte(SC12_2C+u32SCRegBase);
        u8FieldNum = msReadByte(SC12_32+u32SCRegBase) & 0x1F;
    }
    else
    {
        u16LineOffset = msRead2Byte(SC12_AC+u32SCRegBase);
        u8FieldNum = msReadByte(SC12_B2+u32SCRegBase) & 0x1F;
    }
    u32PackSize = u8FieldNum * 4 * 0x20;
    //MSSCALER_PRINT("u16LineOffset:0x%.X\n", u16LineOffset);

    u32PackNum = ((DWORD)u16PositionV * ((u16LineOffset + 0x10) >> 5));
    u32PackNum += (u16PositionH >> 5);
    //MSSCALER_PRINT("u32PackNum:0x%.X\n", u32PackNum);

    u32BaseAddrPix = u32BaseAddr + u32PackNum * u32PackSize;
    //MSSCALER_PRINT("u32BaseAddrPix:0x%.X\n", u32BaseAddrPix);

    if( (u16PositionH + u16PixCount) > u16LineOffset )
        u16PixCount = u16LineOffset - u16PositionH;
    //MSSCALER_PRINT("u16PixCount:0x%.X\n", u16PixCount);

    u6PackOffset = (u16PositionH & 0x001F);
    pMAddrB = (BYTE *)(u32BaseAddrPix + u6PackOffset);
    pMAddrG = (BYTE *)(u32BaseAddrPix + u6PackOffset + (0x20*u8FieldNum));
    pMAddrR = (BYTE *)(u32BaseAddrPix + u6PackOffset + (0x20*u8FieldNum)*2);
    pMAddrLSB = (BYTE *)(u32BaseAddrPix + u6PackOffset + (0x20*u8FieldNum)*3);

    for( i = 0 ; i < u16PixCount ; i++ )
    {
        u6PackOffset = (u16PositionH + i) & 0x001F;
        if( (u6PackOffset == 0) && (i != 0) )
        {
            u32BaseAddrPix += u32PackSize;
            pMAddrB = (BYTE *)(u32BaseAddrPix + u6PackOffset);
            pMAddrG = (BYTE *)(u32BaseAddrPix + u6PackOffset + (0x20*u8FieldNum));
            pMAddrR = (BYTE *)(u32BaseAddrPix + u6PackOffset + (0x40*u8FieldNum));
            pMAddrLSB = (BYTE *)(u32BaseAddrPix + u6PackOffset + (0x60*u8FieldNum));
        }

        *pMAddrB = *(pDataBGR++);
        *pMAddrG = *(pDataBGR++);
        *pMAddrR = *(pDataBGR++);
        pMAddrB++;
        pMAddrG++;
        pMAddrR++;

        if( pDataLSB )
        {
            *pMAddrLSB = *(pDataLSB++);
            pMAddrLSB++;
        }
        else
        {
            *pMAddrLSB = 0x00;
        }
    }
}

// Return Fake Dual Master Port
BYTE msDrvSrcFakeDualGetM( void )
{
    BYTE InputPort;
    for( InputPort=First_Input_Port; InputPort < Input_Nums; InputPort++ )
        if( INPUT_IS_FAKE_DUAL(InputPort) )
            break;

    return InputPort;
}
// Return Fake Dual Slave Port
BYTE msDrvSrcFakeDualGetS( void )
{
    BYTE InputPort;
    for( InputPort=First_Input_Port; InputPort < Input_Nums; InputPort++ )
        if( INPUT_IS_FAKE_DUAL_S(InputPort) )
            break;

    return InputPort;
}
// Return Fake Dual Master Scaler
BYTE msDrvScFakeDualGetM( BYTE u8IdxWin )
{
    BYTE u8IdxSC=0;
    WORD u16ScMask = g_DisplayWindow[u8IdxWin].u16DwScMaskIn|g_DisplayWindow[u8IdxWin].u16DwScMaskOut;

    while(u16ScMask != 0)
    {
        if(u16ScMask & BIT0)
            if( INPUT_IS_FAKE_DUAL(MapScaler2Port(u8IdxSC)) )
                break;

        u16ScMask >>= 1;
        u8IdxSC++;
    }

    if(u16ScMask==0)
        u8IdxSC = SC_MAX_NUMS;

    return u8IdxSC;
}
// Return Fake Dual Slave Scaler
BYTE msDrvScFakeDualGetS( BYTE u8IdxWin )
{
    BYTE u8IdxSC=0;
    WORD u16ScMask = g_DisplayWindow[u8IdxWin].u16DwScMaskIn;

    while(u16ScMask != 0)
    {
        if(u16ScMask & BIT0)
            if( INPUT_IS_FAKE_DUAL_S(MapScaler2Port(u8IdxSC)) )
                break;

        u16ScMask >>= 1;
        u8IdxSC++;
    }

    if(u16ScMask==0)
        u8IdxSC = SC_MAX_NUMS;

    return u8IdxSC;
}

BYTE msDrvSrcMapIPMuxToPort( BYTE eIPMux)  // rtn: PortIdx or Input_Nums if IPMux not used.
{
    BYTE rtn=0;

    for(rtn=0;rtn<Input_Nums;rtn++)
        if((BYTE)g_InputPort[rtn].eIPMux == eIPMux)
            break;

    return rtn;
}
