#define _ADJSUT_C_
#include <math.h>
#include "datatype.h"
#include "Global.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "MDebug.h"
#include "Common.h"
#include "msOSD.h"
#include "Board.h"
#include "menudef.h"
#include "MsDLC.h"
#include "msACE.h"
#include "appAdjust.h"
#include "misc.h"
#include "drvOSD.h"
#include "drvGPIO.h"
#include "Adjust.h"
#include "appAdjust.h"
#include "drvAudio.h"
////////////////////////////////////////
//#include "drvAdjust.h"
//#include "halRwreg.h"
///////////////////////////////////////
#if ENABLE_DPS
#include "msDPS_Setting.h"
#endif
//#include "EepromMap.h"
//===================================================

#if ENABLE_DEMURA_FUNCTION
#include "drvDemura.h"
#endif
#if ENABLE_5707
#include "amp_i2s_tas5707.h"
#endif

//API of Border ===============================================================================================================
void msAPI_EnableBorder(BYTE u8WinIdx, Bool bEnable)
{
    msDrvEnableBorder(u8WinIdx, bEnable);
}
void msAPI_SetBorderFormat(BYTE u8WinIdx, BYTE u8Left, BYTE u8Right, BYTE u8Up, BYTE u8Down, BYTE u8color)
{
    msDrvSetBorderFormat(u8WinIdx, u8Left, u8Right, u8Up, u8Down, u8color);
}

/**************************************************************************
* Set border width as equal at the same time.
*
*   Limit range of width: 0x0F
*   Can choose border inside/outside
*
**************************************************************************/
void msAPI_SetBorderBatchFormat(BYTE u8WinIdx, BYTE u8Width, BorderDirectionType u8Direction, BYTE u8Color)
{
    BYTE u8MSB;
    if(u8Width > 0x0F)
    {
        u8Width = 0x0F;
    }
    u8MSB = u8Width<<4;

    if(u8Direction == eBorderInside)
    {
        msDrvSetBorderFormat(u8WinIdx, u8Width, u8MSB, u8Width, u8MSB, u8Color);
    }
    else //if (u8Direction == eBorderOutside)
    {
        msDrvSetBorderFormat(u8WinIdx, u8MSB, u8Width, u8MSB, u8Width, u8Color);
    }
}

#if (ENABLE_MWE)
// MWE Function
void msAPI_MWEActive(BYTE u8WinIdx, Bool bEnable)
{
    if(bEnable)
    {
        SET_MWE_WIN_CONTROL_FLAG();
        msDrvMWE_GetScalerMask(u8WinIdx);
    }
    else
    {
        msDrvMWE_ResetScalerMask(u8WinIdx);
        CLR_MWE_WIN_CONTROL_FLAG();
    }
}


#endif

// Local function declaration
void msAPI_AdjustSharpness(BYTE u8DispWin, BYTE u8Sharpness, BYTE u8Step)
{
    EN_WINDOW_COLOR  eColor = eWIN_COLOR_RGB;
    msAPIWinGetColorSpaceWindow(u8DispWin, &eColor);

                    // 0    1     2     3     4     5     6     7     8     9     10
//    BYTE SharpTbl[]={0x80, 0x70, 0x60, 0x40, 0x20, 0x00, 0x90, 0xC0, 0xD0, 0xE0, 0xFF};
//    BYTE SharpTbl[]={ 0x70, 0x40,  0x00, 0x90, 0xD0};
    BYTE SharpTbl[]={ 0x40, 0x20,  0x00, 0x90, 0xD0};

    if(msAPIWinColorIsYUV(u8DispWin))
    {
        msDrvAdjustSharpness(u8DispWin, SharpTbl[u8Sharpness], u8Step, eWIN_COLOR_YUV);
    }
    else
    {
        msDrvAdjustSharpness(u8DispWin, SharpTbl[u8Sharpness], u8Step, eWIN_COLOR_RGB);
    }
}
void msAPI_AdjustBrightness( BYTE u8Brightness )
{
   WORD u16Brightness;


    #if ENABLE_DPS
    u8Brightness = u8Brightness * u8DPSCurBacklight /255;
    #endif
    u16Brightness=APPAdjustBrightness(u8Brightness);
    #ifdef BACKLIGHT_PWM_REVERSE           //+Duty power
    msAPIPWMDuty(BACKLIGHT_PWM, 0xFF-u16Brightness);
    #else                                //-Duty power
    msAPIPWMDuty(BACKLIGHT_PWM, u16Brightness);
    #endif
}

void msAPI_AdjustRGBColor( BYTE u8DispWin, WORD u16Red, WORD u16Green, WORD u16Blue)
{
    u16Red  = GetRealRgbGain(u16Red);
    u16Green= GetRealRgbGain(u16Green);
    u16Blue = GetRealRgbGain(u16Blue);

	//if(CURRENT_PICTURE_MODE==CT_STANDARD||CURRENT_PICTURE_MODE==CT_USER)
         	u16Blue = (u16Blue-u16Blue*(USERPREFLOWBLURAY*0.1));


    #if (COLOR_ACCURACY==COLOR_8_BIT)
    {
        if (msAPIWinColorIsYUV(u8DispWin))
        {//YUV-->YUV
            msAdjustVideoRGB(u8DispWin, (BYTE)(u16Red), (BYTE)(u16Green), (BYTE)(u16Blue));
        }
        else
        {//RGB-->RGB
            msAdjustPCRGB(u8DispWin, (BYTE)(u16Red), (BYTE)(u16Green), (BYTE)(u16Blue));
        }
    }
    #elif (COLOR_ACCURACY==COLOR_10_BIT)
    {
        if (msAPIWinColorIsYUV(u8DispWin))
        {//YUV-->YUV
            msAdjustVideoRGB_10Bits(u8DispWin, (u16Red), (u16Green), (u16Blue));
        }
        else
        {//RGB-->RGB
            msAdjustPCRGB_10Bits(u8DispWin, (u16Red), (u16Green), (u16Blue));
        }
    }
    #endif
}

void msAPI_InitialVideoColorMatrix(BYTE u8DispWin, WORD wHue, BYTE ucSaturation, WORD u16Contrast, WORD wRCon, WORD wGCon, WORD wBCon)
 {
    EN_WINDOW_COLOR  eColor = eWIN_COLOR_RGB;

    msAPIWinGetColorSpaceWindow(u8DispWin, &eColor);

    wRCon  = GetRealRgbGain(wRCon);
    wGCon= GetRealRgbGain(wGCon);
    wBCon = GetRealRgbGain(wBCon);
    ucSaturation = GetRealSaturation(ucSaturation);

	//if(CURRENT_PICTURE_MODE==CT_STANDARD||CURRENT_PICTURE_MODE==CT_USER)
         	wBCon = (wBCon-wBCon*(USERPREFLOWBLURAY*0.1));

    #if (COLOR_ACCURACY==COLOR_8_BIT)
    {
        //if Color format is Ypbpr, or format is RGB && CSC enable(Y to R).
        if(msAPIWinColorIsYUV(u8DispWin))
        {//YUV-->YUV
            u16Contrast = GetRealVideoContrast(u16Contrast);
        }
        else
        {//RGB-->RGB
            u16Contrast = GetRealPcContrast(u16Contrast);
        }
    }
    #elif (COLOR_ACCURACY==COLOR_10_BIT)
    {
        if(msAPIWinColorIsYUV(u8DispWin))
        {//YUV-->YUV
            u16Contrast = GetRealVideoContrast(u16Contrast);
        }
        else
        {//RGB-->RGB
            u16Contrast = GetRealPcContrast(u16Contrast);
        }
    }
    #endif

    msInitialVideoColorMatrix( u8DispWin,  wHue,  ucSaturation,  u16Contrast,  wRCon,  wGCon,  wBCon);
}

void msAPI_AdjustContrast( BYTE u8DispWin, WORD u16Contrast)
{
    EN_WINDOW_COLOR  eWinColor = eWIN_COLOR_RGB;
    EN_INPUT_COLOR   eInputColor = eINPUT_COLOR_RGB;
    EN_COLOR_RANGE  eInputRange = eCOLOR_RANGE_FULL;

#if ENABLE_DPS
    u16Contrast = u16Contrast * u16DPSCurYgain / 1024 ;
#endif
    msAPIWinGetColorSpaceWindow(u8DispWin, &eWinColor);
    msAPIWinGetColorSpaceInput(u8DispWin, &eInputColor, &eInputRange);
   #if (COLOR_ACCURACY==COLOR_8_BIT)
    {
        //if Color format is Ypbpr, or format is RGB && CSC enable(Y to R).
        if (msAPIWinColorIsYUV(u8DispWin))
        {//YUV-->YUV
            msACESetRGBColorRange(u8DispWin, FALSE);
            u16Contrast = GetRealVideoContrast(u16Contrast);
            msAdjustVideoContrast(u8DispWin, (BYTE)u16Contrast);
        }
        else
        {//RGB-->RGB
            msACESetRGBColorRange(u8DispWin, (eInputRange == eCOLOR_RANGE_LIMIT));
            u16Contrast = GetRealPcContrast(u16Contrast);

            msAdjustPCContrast(u8DispWin, (BYTE)u16Contrast);
        }
    }
    #elif (COLOR_ACCURACY==COLOR_10_BIT)
    {
        if (msAPIWinColorIsYUV(u8DispWin))
        {//YUV-->YUV
            msACESetRGBColorRange(u8DispWin, FALSE);
            u16Contrast = GetRealVideoContrast(u16Contrast);
            msAdjustVideoContrast_10Bits(u8DispWin, u16Contrast);
        }
        else
        {//RGB-->RGB
            msACESetRGBColorRange(u8DispWin, (eInputRange == eCOLOR_RANGE_LIMIT));
            u16Contrast = GetRealPcContrast(u16Contrast);
            msAdjustPCContrast_10Bits(u8DispWin, u16Contrast);
        }
    }
    #endif
}

void msAPI_DeGammaLoadTbl_256E_12B(BYTE u8WinIndex, WORD **pu8Table) //sam 141203
{
     BYTE i;
    msDrvDeGammaEnable(u8WinIndex, FALSE);
    for (i = 0; i < 3; i++)
        msDrvDeGammaLoadTbl_256E_12B(u8WinIndex, i, pu8Table);
}

void msAPI_AdjustBlacklevel( BYTE u8DispWin ,WORD u16Redblacklevel, WORD u16Greenblacklevel, WORD u16Blueblacklevel)
{
    u16Redblacklevel  = GetRealBlackLevel(u16Redblacklevel);
    u16Blueblacklevel = GetRealBlackLevel(u16Blueblacklevel);
    u16Greenblacklevel= GetRealBlackLevel(u16Greenblacklevel);
    #if (COLOR_ACCURACY == COLOR_8_BIT)
    {
        //msDrvAdjustBlacklevel(u8DispWin, (BYTE)u16Redblacklevel, (BYTE)u16Greenblacklevel, (BYTE)u16Blueblacklevel);
        msAdjustRGBOffset(u8DispWin,  u16Redblacklevel<<2, u16Greenblacklevel<<2, u16Blueblacklevel<<2);
    }
    #elif (COLOR_ACCURACY == COLOR_10_BIT)
    {
        //msDrvAdjustBlacklevel_10Bits(u8DispWin, u16Redblacklevel, u16Greenblacklevel, u16Blueblacklevel);
        msAdjustRGBOffset(u8DispWin,  u16Redblacklevel, u16Greenblacklevel, u16Blueblacklevel);
    }
    #endif
    msDrvBlackLevelEnable(u8DispWin,ENABLE);
}

void msAPI_AdjustHPosition( BYTE u8DispWin , WORD u16Position)
{
    BYTE u8ScalerIdx = 0;
    BYTE u8InputPort = MapWin2Port(u8DispWin);
    WORD u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskIn;
    ST_WINDOW_INFO  sCaptureWin;

    msAPIWinGetCaptureWin(u8DispWin, &sCaptureWin);
    sCaptureWin.x = u16Position;
    msAPIWinSetCaptureWin(u8DispWin, &sCaptureWin);

    if(HBR2Flag(u8InputPort))
    {
        //WAIT_V_INPUT_BLANKING_START(GetLsbBitPos(u16SCMask));
        msDrvAdjustHBR2HPosition(u8DispWin, u16Position);
    }
    else
    {
        while(u16SCMask)
        {
            if(u16SCMask & BIT0)
            {
                msDrvIP1SetImageHStart(u16Position, u8ScalerIdx);//_msWrite2Byte(SC0_07, position);
            }
            u8ScalerIdx++;
            u16SCMask >>= 1;
        }
    }
}

void msAPI_AdjustVPosition( BYTE u8DispWin , WORD u16Position)
{
    BYTE u8ScalerIdx = 0;
    WORD u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskIn;
    ST_WINDOW_INFO  sCaptureWin;

    msAPIWinGetCaptureWin(u8DispWin, &sCaptureWin);
    sCaptureWin.y = u16Position;
    msAPIWinSetCaptureWin(u8DispWin, &sCaptureWin);

    while(u16SCMask)
    {
        if(u16SCMask & BIT0)
        {
            msDrvIP1SetImageVStart(u16Position, u8ScalerIdx);//_msWrite2Byte(SC0_07, position);
        }
        u8ScalerIdx++;
        u16SCMask >>= 1;
    }
}

// bDirection: 0 for left, 1 for right
void msAPI_AdjustOutputHPosition( BYTE u8DispWin , WORD u16ShiftAmount , BOOL bDirection )
{
    BYTE u8ScalerIdx = 0;
    WORD u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskOut;
    WORD u16HStart, u16HEnd, u16HEndPre;
    BYTE u8SCDBBkup[SC_MAX_NUMS];
    WORD u16PnlHDEEnd = (PanelHStart/(msDrvISSCDualPort() ? 2:1) + PANEL_WIDTH/(msDrvISSCDualPort() ? 2:1) -1); // Not real register setting, please refer to msDrvmStarInit().

    WAIT_V_OUTPUT_BLANKING_START();
    while(u16SCMask)
    {
        if(u16SCMask & BIT0)
        {
            u8SCDBBkup[u8ScalerIdx] = msReadByte(SC00_02+SCRegBase(u8ScalerIdx));
            // enable double buffer
            msWriteByte(SC00_02+SCRegBase(u8ScalerIdx), 0x05);
        }
        u8ScalerIdx++;
        u16SCMask >>= 1;
    }

    u8ScalerIdx = 0;
    u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskOut;
    while(u16SCMask)
    {
        if(u16SCMask & BIT0)
        {
            u16HStart = msDrvOP2GetImageHStart(u8ScalerIdx);
            u16HEnd = u16HEndPre = msDrvOP2GetImageHEnd(u8ScalerIdx);

            u16HStart = (bDirection)?(u16HStart + u16ShiftAmount):(u16HStart - u16ShiftAmount);
            u16HEnd = (bDirection)?(u16HEnd + u16ShiftAmount):(u16HEnd - u16ShiftAmount);

            if((u16HEndPre>u16PnlHDEEnd) && (u16HEnd<=u16PnlHDEEnd))
                u16HEnd -=1;
            else if((u16HEndPre<u16PnlHDEEnd) && (u16HEnd>=u16PnlHDEEnd))
                u16HEnd +=1;

            msDrvOP2SetImageHStart(u8ScalerIdx, u16HStart);
            msDrvOP2SetImageHEnd(u8ScalerIdx, u16HEnd);
        }
        u8ScalerIdx++;
        u16SCMask >>= 1;
    }

    WAIT_V_OUTPUT_BLANKING_START();
    WAIT_V_OUTPUT_BLANKING_START();
    u8ScalerIdx = 0;
    u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskOut;
    while(u16SCMask)
    {
        if(u16SCMask & BIT0)
        {
            // restore double buffer
            msWriteByte(SC00_02+SCRegBase(u8ScalerIdx), u8SCDBBkup[u8ScalerIdx] );
        }
        u8ScalerIdx++;
        u16SCMask >>= 1;
    }
}

// bDirection: 0 for up, 1 for down
void msAPI_AdjustOutputVPosition( BYTE u8DispWin , WORD u16ShiftAmount , BOOL bDirection )
{
    BYTE u8ScalerIdx = 0;
    WORD u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskOut;
    WORD u16VStart, u16VEnd;
    WORD u16SCVStart,u16SCVEnd;
    BYTE u8SCDBBkup[SC_MAX_NUMS];

    WAIT_V_OUTPUT_BLANKING_START();
    while(u16SCMask)
    {
        if(u16SCMask & BIT0)
        {
            u8SCDBBkup[u8ScalerIdx] = msReadByte(SC00_02+SCRegBase(u8ScalerIdx));
            // enable double buffer
            msWriteByte(SC00_02+SCRegBase(u8ScalerIdx), 0x05);
        }
        u8ScalerIdx++;
        u16SCMask >>= 1;
    }

    u8ScalerIdx = 0;
    u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskOut;
    while(u16SCMask)
    {
        if(u16SCMask & BIT0)
        {
            u16VStart = msDrvOP2GetImageVStart(u8ScalerIdx);
            u16VEnd = msDrvOP2GetImageVEnd(u8ScalerIdx);

            u16VStart = (bDirection)?(u16VStart + u16ShiftAmount):(u16VStart - u16ShiftAmount);
            u16VEnd = (bDirection)?(u16VEnd + u16ShiftAmount):(u16VEnd - u16ShiftAmount);

            u16SCVStart = VERTICAL_DE_START + g_ScalerInfo[u8ScalerIdx].sSiOP2DisplayWin.y;
            u16SCVEnd = u16SCVStart + g_ScalerInfo[u8ScalerIdx].sSiOP2DisplayWin.height - 1;

            if(u16VEnd>u16SCVEnd || u16VStart>=u16SCVStart)
            {
                u16VEnd = u16SCVEnd;
            }
            msDrvOP2SetImageVStart(u8ScalerIdx, u16VStart);
            msDrvOP2SetImageVEnd(u8ScalerIdx, u16VEnd);
        }
        u8ScalerIdx++;
        u16SCMask >>= 1;
    }

    WAIT_V_OUTPUT_BLANKING_START();
    WAIT_V_OUTPUT_BLANKING_START();
    u8ScalerIdx = 0;
    u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskOut;
    while(u16SCMask)
    {
        if(u16SCMask & BIT0)
        {
            // restore double buffer
            msWriteByte(SC00_02+SCRegBase(u8ScalerIdx), u8SCDBBkup[u8ScalerIdx] );
        }
        u8ScalerIdx++;
        u16SCMask >>= 1;
    }
}

/*===============================================================================================
    ePATTERN_COLOR_RED      = 0xE0,
    ePATTERN_COLOR_GREEN    = 0x1C,
    ePATTERN_COLOR_BLUE     = 0x03,
    ePATTERN_COLOR_CYNA     = 0x1F,
    ePATTERN_COLOR_MEGENTA  = 0xE3,
    ePATTERN_COLOR_YELLOW   = 0xFC,
    ePATTERN_COLOR_WHITE    = 0xFF,
    ePATTERN_COLOR_BLACK    = 0x00,
===============================================================================================*/
BGColorType tblTestPattern[8]=
{
    ePATTERN_COLOR_RED,
    ePATTERN_COLOR_GREEN,
    ePATTERN_COLOR_BLUE,
    ePATTERN_COLOR_CYNA,
    ePATTERN_COLOR_MEGENTA,
    ePATTERN_COLOR_YELLOW,
    ePATTERN_COLOR_WHITE,
    ePATTERN_COLOR_BLACK,
};
void msAPI_AdjustBackgoundColor( BYTE u8DispWin ,BGColorType u8Pattern)
{
    WAIT_V_OUTPUT_BLANKING();
    msAPIWinFrameColorRGB(u8DispWin, u8Pattern);
}


// This function is used for Window Fading Feature
// bEnable: 1: force output frame color, 0:disable force output frame color
// u8Red, u8Green, u8Blue used when bEnable=1
// This API will bakup original frame color first when 1st enabled, restored them when disabled
void msAPI_ForceFrameColor(BOOL bEnable, BYTE u8Red, BYTE u8Green, BYTE u8Blue)
{
    static DWORD u32FrameColorBK = 0xFFFFFFFF;
    static BYTE u8ScalerBK = 0;
    BYTE u8SCIndex;

    if(bEnable)
    {
        if(u32FrameColorBK == 0xFFFFFFFF) // 1st time Enable
        {
            u32FrameColorBK = msRead3Byte(SCL0_10_33);
            for( u8SCIndex=SCL0_SUB; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=2 )
            {
                u8ScalerBK |= ((msReadByte(SC20_20+SCRegBase(u8SCIndex))&0x03)<<u8SCIndex);
                msWriteByteMask((SC20_20+SCRegBase(u8SCIndex)), 0x00, 0x03);
            }
        }
        msDrvSCCommonWrite(SCALER_MASK_ALL, ENABLE);
        msWrite3Byte(SCL0_10_33, u8Red|(u8Green<<8)|(u8Blue<<16));
        msDrvSCCommonWrite(SCALER_MASK_ALL, DISABLE);
    }
    else
    {
        if(u32FrameColorBK != 0xFFFFFFFF) // enable -> disable
        {
            for( u8SCIndex=SCL0_SUB; u8SCIndex < SC_MAX_NUMS-1 ; u8SCIndex+=2 )
            {
                msWriteByteMask((SC20_20+SCRegBase(u8SCIndex)), (u8ScalerBK>>u8SCIndex)&0x03, 0x03);
            }
            u8ScalerBK = 0;
            msDrvSCCommonWrite(SCALER_MASK_ALL, ENABLE);
            msWrite3Byte(SCL0_10_33, u32FrameColorBK);
            msDrvSCCommonWrite(SCALER_MASK_ALL, DISABLE);
            u32FrameColorBK = 0xFFFFFFFF;
        }
    }
}

#if ENABLE_VGA_INPUT
void msAPI_AdjustVGASourceType( BYTE u8DispWin )
{
    EN_INPUT_COLOR eInputColor;
    EN_COLOR_RANGE eInputRange;

    msAPIWinGetColorSpaceInput(u8DispWin, &eInputColor, &eInputRange);

    if(eInputColor != eINPUT_COLOR_RGB) //(eInputColor == eINPUT_COLOR_YPBPR)
    {
        if(g_bInputSOGFlag)
            drvADC_SetADCSource(ADC_TABLE_SOURCE_YUV);  // input is SOG-Yuv
        else
            drvADC_SetADCSource(ADC_TABLE_SOURCE_YUV_HV);  // input is HV-Yuv
    }
    else // (eInputColor == eINPUT_COLOR_RGB)
    {
        if(g_bInputSOGFlag)
            drvADC_SetADCSource(ADC_TABLE_SOURCE_SOG);  // input is SOG-RGB
        else
            drvADC_SetADCSource(ADC_TABLE_SOURCE_RGB);  // input is HV-RGB
    }
}
#endif
/*************************************
*CSC:
*               <RGB>       <YCbCr>
*   <Standard>  Disable     Disable
*   <Movie>     Enable      Disable
*
*
*Y sub 16:
*               <RGB>       <YCbCr>
*   <Standard>  Disable     Enable
*   <Movie>     Disable     Enable

*************************************/
Bool msAPI_SetInputCSC(BYTE u8DispWin, Bool bYUVDomain)
{
    EN_INPUT_COLOR eInputColor;
    EN_COLOR_RANGE eInputRange;

    Bool bEnableR2Y = FALSE;
    Bool bEnableYSub16 = FALSE;

    msAPIWinGetColorSpaceInput(u8DispWin, &eInputColor, &eInputRange);

    if(bYUVDomain) // y mode
    {
        if(eInputColor == eINPUT_COLOR_RGB)
        {
            bEnableR2Y = TRUE;
        }

        #if EABLE_Y_EXTENSION
        if(eInputColor == eINPUT_COLOR_RGB)
        {
            if(eInputRange == eCOLOR_RANGE_LIMIT)
            {
                bEnableYSub16 = TRUE;
            }
            else
            {
                bEnableYSub16 = FALSE;
            }
        }
        else
        #endif
        {
            if(eInputRange == eCOLOR_RANGE_FULL)
            {
                bEnableYSub16 = FALSE;
            }
            else
            {
                bEnableYSub16 = TRUE;
            }
        }
    }

    //Y-16 enable
    msDrvColorFormat_YSub16Enable(u8DispWin, YSUB_16_SEL, bEnableYSub16);

    //CSC enable
    msDrvColorFormat_CSCEnable(u8DispWin, CSC_MATRIX_SEL,  bEnableR2Y);

    return FALSE;

}
//API of Saturation============================================================================================================
void msAPI_AdjustSaturation(BYTE u8WinIndex, BYTE u8Saturation)
{
    msAdjustVideoSaturation(u8WinIndex, u8Saturation);
}

//API of Hue===================================================================================================================
void msAPI_AdjustVideoHue(BYTE u8WinIndex, WORD u8Hue)
{
    msAdjustVideoHue(u8WinIndex, u8Hue);
}

//API of Custom Hue============================================================================================================
/*enum  //Color Index
{
    eCUSTOM_HUE_R,
    eCUSTOM_HUE_G,
    eCUSTOM_HUE_B,
    eCUSTOM_HUE_C,
    eCUSTOM_HUE_M,
    eCUSTOM_HUE_Y,
};*/
void msAPI_AdjustCustomHue(BYTE u8WinIndex, BYTE u8HueValue, BYTE u8ColorIndex)
{
    msDrvAdjustCustomHue(u8WinIndex, u8HueValue, u8ColorIndex);
}
void msAPI_CustomHueEnable(BYTE u8WinIndex, Bool bEnable)
{
    msDrvCustomHueEnable(u8WinIndex, bEnable);
}

#if (CHIP_ID == MST9U3)
void msAPI_AdjustCustomSatRGBMode(BYTE u8WinIndex, BYTE u8Value, BYTE u8ColorIndex)
{
    msDrvAdjustCustomSatRGBMode(u8WinIndex, u8Value, u8ColorIndex);
}
void msAPI_CustomSatEnableRGBMode(BYTE u8WinIndex, Bool bEnable)
{
    msDrvCustomSatEnableRGBMode(u8WinIndex, bEnable);
}

void msAPI_AdjustCustomBriRGBMode(BYTE u8WinIndex, BYTE u8Value, BYTE u8ColorIndex)
{
    msDrvAdjustCustomBriRGBMode(u8WinIndex, u8Value, u8ColorIndex);
}
void msAPI_CustomBriEnableRGBMode(BYTE u8WinIndex, Bool bEnable)
{
    msDrvCustomBriEnableRGBMode(u8WinIndex, bEnable);
}
#endif


//API of Histogram============================================================================================================
// Histogram Range, 8 pixels per unit
#if 0
void msAPI_HistogramSetRange(BYTE u8WinIndex)
{
    //WORD u16WinHStart = g_DisplayWindow[u8WinIndex].sDwDisplayWin.x;

    EN_WINDOW_COLOR  eColor = eWIN_COLOR_RGB;
    msAPIWinGetColorSpaceWindow(u8WinIndex, &eColor);
    msDrvHistogramYSetRGBMode(u8WinIndex, ((eColor == eWIN_COLOR_RGB)? TRUE:FALSE));

    //Get real Hstart/End
    //u16HStart -= u16WinHStart;
    //u16HEnd -= u16WinHStart;

    msDrvHistogramSetRange(u8WinIndex);
}
#endif

void msAPI_AutoSetHistogramRange(BYTE u8WinIndex)
{
    EN_WINDOW_COLOR  eColor = eWIN_COLOR_RGB;
    msAPIWinGetColorSpaceWindow(u8WinIndex, &eColor);
    msDrvHistogramYSetRGBMode(u8WinIndex, ((eColor == eWIN_COLOR_RGB)? TRUE:FALSE));
    msDrvAutoSetHistogramRange(u8WinIndex);
}

// Histogram Request Report
void msAPI_HistogramReportEnable(BYTE u8WinIdx)
{
    msDrvHistogramReportEnable(u8WinIdx);
}

// Histogram Check ACK
Bool msAPI_HistogramCheckACK(BYTE u8WinIdx)
{
    return msDrvHistogramCheckACK(u8WinIdx);
}

// Histogram Data, u8Section == 32
void msAPI_HistogramGetData(BYTE u8WinIndex, BYTE u8Section, DWORD *pu32Data)
{
    // need check HISTOGRAM_READY_FLAG   TBD
    msDrvHistogramGetData(u8WinIndex , u8Section , pu32Data);
}

void msAPI_HistogramUpDateData(BYTE u8WinIndex)
{
    msDrvHistogramUpDateData(u8WinIndex);
}

void msAPI_SetDLCCurve(BYTE u8WinIndex, BYTE *u8Table)
{
    msDrvSetDLCCurve(u8WinIndex, &u8Table[0]);
}

void msAPI_SetDLCEnable(BYTE u8WinIndex, Bool bEnable)
{
    EN_WINDOW_COLOR ucColor;
    msAPIWinGetColorSpaceWindow(u8WinIndex, &ucColor);
    msDrvSetDLCEnable(u8WinIndex, bEnable, ucColor);
}

//API of Over Drive==========================================================================================================
#if ENABLE_RTE
void msAPI_OverDriveInit()
{
    msDrvOverDriverInit();
}
void msAPI_OverDriveEnable(BOOL bEnable)
{
    msDrvOverDriveOnOff(bEnable);
}

#if ENABLE_OD_MTB
void msAPI_OverDriveMultiTable(BYTE index)
{
    msLoad_MultiODTable(index);
}
#else
void msAPI_OverDriveTable(BYTE *pODTbl)
{
    msDrvOverDriveTable(pODTbl);
}
#endif
#endif

//API of GAMMA===============================================================================================================
//Load Gamma LUT (Assigned: WindowIndex, TableIndex, GammaFormat)
void msAPI_GammaLoadTbl_256E_8B(BYTE u8WinIndex, BYTE **pu8TableIndex)
{
    msDrvGammaLoadTbl_256E_8B(u8WinIndex, pu8TableIndex);
}
void msAPI_GammaLoadTbl_256E_10B(BYTE u8WinIndex, BYTE **pu8TableIndex)
{
    msDrvGammaLoadTbl_256E_10B(u8WinIndex, pu8TableIndex);
}

void msAPI_GammaLoadTbl_1024E_14B(BYTE u8WinIndex, BYTE **pu8TableIndex)
{
    msDrvGammaLoadTbl_1024E_14B(u8WinIndex, pu8TableIndex);
}

//Gamma Enable/Disable
void msAPI_GammaEnable(BYTE u8WinIndex, BYTE bEnable)
{
    msDrvGammaEnable(u8WinIndex, bEnable);
}

static BYTE ucDeGammaFunc[DISPLAY_MAX_NUMS] = {_DISABLE};
void msAPI_DeGammaLoadTbl_Compressed(BYTE u8WinIndex, BYTE **pu8TableIndex, WORD** pu16SegPntTbl)
{
    BYTE i;
    //msDrvDeGammaEnable(u8WinIndex, FALSE);
    for (i = 0; i < DISPLAY_MAX_NUMS; i++)
        if (msAPIWinGetScDwEnable(i))
            msDrvDeGammaEnable(i, FALSE);

    for (i = 0; i < 3; i++)
    {
    	#if Enable_AutoDownloadGamma
            msDrvDeGammaLoadTbl_Compress_ByDMA(u8WinIndex, i, pu8TableIndex);
        	msDrvDeGamma_Compress_DMA_Reader_Trigger(u8WinIndex);
			msDrvDeGammaLoadTbl_Compress_Offset(u8WinIndex, i, pu8TableIndex, pu16SegPntTbl);
		#else
        msDrvDeGammaLoadTbl_Compress(u8WinIndex, i, pu8TableIndex, pu16SegPntTbl);
		#endif
	}
	
    ucDeGammaFunc[u8WinIndex] = _ENABLE;

    for (i = 0; i < DISPLAY_MAX_NUMS; i++)
        if (msAPIWinGetScDwEnable(i) && (ucDeGammaFunc[i] == _ENABLE))
            msDrvDeGammaEnable(i, TRUE);
}

void msAPI_DeGammaLoadTbl(BYTE u8WinIndex, BYTE **pu8TableIndex)
{
    BYTE i;
    //msDrvDeGammaEnable(u8WinIndex, FALSE);
    for (i = 0; i < DISPLAY_MAX_NUMS; i++)
        if (msAPIWinGetScDwEnable(i))
            msDrvDeGammaEnable(i, FALSE);

    for (i = 0; i < 3; i++)
#if Enable_AutoDownloadGamma
        msDrvDeGammaWriteTbl_256E_12B_ByDMA(u8WinIndex, i, pu8TableIndex);
    msDrvDeGamma_DMA_Reader_Trigger(u8WinIndex);
#else
        msDrvDeGammaLoadTbl(u8WinIndex, i, pu8TableIndex);
#endif
    ucDeGammaFunc[u8WinIndex] = _ENABLE;

#if (!defined(W315D_Project))
     for (i = 0; i < DISPLAY_MAX_NUMS; i++)
         if (msAPIWinGetScDwEnable(i) && (ucDeGammaFunc[i] == _ENABLE))
             msDrvDeGammaEnable(i, TRUE);
#endif
}


void msAPI_DeGammaEnable(BYTE u8WinIndex, BYTE bEnable)
{
    msDrvDeGammaEnable(u8WinIndex, bEnable);
    ucDeGammaFunc[u8WinIndex] = bEnable;
}

static BYTE ucFixGammaFunc[DISPLAY_MAX_NUMS] = {_DISABLE};
void msAPI_FixGammaLoadTbl_256E_12B(BYTE u8WinIndex, BYTE **pu8TableIndex)
{
    BYTE i;
    //msDrvFixGammaEnable(u8WinIndex, FALSE);
    for ( i = 0; i < DISPLAY_MAX_NUMS; i++)
        if (msAPIWinGetScDwEnable(i))
            msDrvFixGammaEnable(i, FALSE);

    for (i = 0; i < 3; i++)
#if Enable_AutoDownloadGamma
        {
            msDrvFixGammaWriteTbl_256E_12B_ByDMA(u8WinIndex, i, pu8TableIndex);
        }
        msDrvFixGamma_DMA_Reader_Trigger(u8WinIndex);
#else
        msDrvFixGammaLoadTbl_256E_10B(u8WinIndex, i, pu8TableIndex);
#endif
    ucFixGammaFunc[u8WinIndex] = _ENABLE;
#if (!defined(W315D_Project))
    for ( i = 0; i < DISPLAY_MAX_NUMS; i++)
        if (msAPIWinGetScDwEnable(i) && (ucFixGammaFunc[i] == _ENABLE))
            msDrvFixGammaEnable(i, TRUE);
#endif
}

void msAPI_FixGammaLoadTbl_256E_16B(BYTE u8WinIndex, BYTE **pu8TableIndex)
{
    BYTE i;
    //msDrvFixGammaEnable(u8WinIndex, FALSE);
    for ( i = 0; i < DISPLAY_MAX_NUMS; i++)
        if (msAPIWinGetScDwEnable(i))
            msDrvFixGammaEnable(i, FALSE);

    for (i = 0; i < 3; i++)
        msDrvFixGammaLoadTbl_256E_16B(u8WinIndex, i, pu8TableIndex);

    ucFixGammaFunc[u8WinIndex] = _ENABLE;
    for ( i = 0; i < DISPLAY_MAX_NUMS; i++)
        if (msAPIWinGetScDwEnable(i) && (ucFixGammaFunc[i] == _ENABLE))
            msDrvFixGammaEnable(i, TRUE);

}

void msAPI_FixGammaEnable(BYTE u8WinIndex, BYTE bEnable)
{
    msDrvFixGammaEnable(u8WinIndex, bEnable);
    ucFixGammaFunc[u8WinIndex] = bEnable;
}
#if AudioFunc
void msAPI_AdjustVolume(BYTE u8Volume)
{
#if !AudioNewCurve
    signed char s8Volume;
#endif
#if !ENABLE_5707
    BYTE Volume;
#endif

//printData("4444444444444 u8Volume==%d",u8Volume);
  if(u8Volume==0||AUDIO_MUTE_FLAG)
  {
    #if ENABLE_DAC
		msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, TRUE);
    #endif
  }
  else
  {

  #if AudioNewCurve
       msAPI_AudioSetVolume( AUDIO_DPGA_ALL, E_AUDIO_LEFT_RIGHT_CH, u8Volume, VOLUME_FRACTION_MINUS_dot0dB );
  #else
	s8Volume = (-1)*GetRealVolume(u8Volume);
	#if ENABLE_DAC
        msAPI_AudioSetVolume( AUDIO_DPGA_ALL, E_AUDIO_LEFT_RIGHT_CH, s8Volume, VOLUME_FRACTION_MINUS_dot0dB );
	#endif
 #endif	
 #if !ENABLE_5707
     Volume=GetRealPWMVolume(u8Volume);
     msAPIPWMDuty(VOLUME_PWM, 0xFF-Volume);
#endif
 
  }
  
}
#endif
//API of Custom Saturation============================================================================================================
void msAPI_LoadICCInitialTable(void)
{
msDrvLoadICCInitialTable();
}
//------------------------------------------------------------------------
// Only  support YUV input
// Adjust vide R,G,B,C,M,Y Saturation
// Input:
//      u8WinIndex: display window select
//      u8HueValue: 0x8F ~ 0x0F (-0x0Fh ~ 0x0Fh)
//     u8ColorIndex: 0 ~ 31  (depend on msDrvLoadICCInitialTable())
//------------------------------------------------------------------------

void msAPI_AdjustCustomSaturation(BYTE u8WinIndex, BYTE u8HueValue, BYTE u8ColorIndex)
{
    #if (CHIP_ID == MST9U3)
    msDrvAdjustCustomSatRGBMode(u8WinIndex, u8HueValue, u8ColorIndex);
    #else 
    msDrvAdjustCustomSaturation(u8WinIndex, u8HueValue, u8ColorIndex);
    #endif
}
void msAPI_CustomSaturationEnable(BYTE u8WinIndex, Bool bEnable)
{
    #if (CHIP_ID == MST9U3)
    msDrvCustomSatEnableRGBMode(u8WinIndex, bEnable);
    #else
    msDrvCustomSaturationEnable(u8WinIndex, bEnable);
    #endif
}

void msAPI_AdjustVIPHue(BYTE u8WinIndex, BYTE u8HueValue, BYTE u8ColorIndex)
{
    msDrvAdjustVIPHue(u8WinIndex, u8HueValue, u8ColorIndex);
}
void msAPI_VIPHueEnable(BYTE u8WinIndex, Bool bEnable)
{
    msDrvVIPHueEnable(u8WinIndex, bEnable);
}

void msAPI_AdjustSkinTone(BYTE u8WinIndex, Bool bEnable, BYTE adjValue)
{
    const BYTE u8ColorIndex = 7;
    msAPI_AdjustVIPHue(u8WinIndex, adjValue, u8ColorIndex);
    msAPI_VIPHueEnable(u8WinIndex, bEnable);
}

//------------------------------------------------------------------------
// Only  support YUV input
// Adjust vide R,G,B,C,M,Y brightness
// Input:
//      u8WinIndex: display window select
//      u8BriValue: 0x00 ~ 0x3F (0 ~ 63)
//     u8ColorIndex: 0 ~ 15  (depend on msDrvLoadICCInitialTable())
//                   1 ~ 6 -> R, G, B, C, M, Y
//------------------------------------------------------------------------
void msAPI_CustomBrightnessEnable(BYTE u8WinIndex, Bool bEnable)
{
    #if (CHIP_ID == MST9U3)
    msDrvCustomBriEnableRGBMode(u8WinIndex, bEnable);
    #else
    msDrvCustomBrightnessEnable(u8WinIndex, bEnable);
    #endif
}
void msAPI_AdjustCustomBrightness(BYTE u8WinIndex, BYTE u8BriValue, BYTE u8ColorIndex)
{
    #if (CHIP_ID == MST9U3)
    msDrvAdjustCustomBriRGBMode(u8WinIndex, u8BriValue, u8ColorIndex-1);
    #else
    msDrvAdjustCustomBrightness(u8WinIndex, u8BriValue, u8ColorIndex);
    #endif
}

void msAPI_ColorMatrixEnable(BYTE u8WinIndex, BYTE bEnable, BYTE bCarryEn)
{
    msDrvColorMatrixEnable(u8WinIndex, bEnable, bCarryEn);
}

#if ENABLE_DEMURA_FUNCTION
///////////////////////////////////////////////////////////////////////////
//   DeMura function (L0)
///////////////////////////////////////////////////////////////////////////
void msAPI_DeMuraEnable(BYTE bEnable)
{
    drvEnabledDemura(bEnable);
}
#endif
///////////////////////////////////////////////////////////////////////////
//   MWE Window
///////////////////////////////////////////////////////////////////////////
#if 0
void msAPIMWEWindowConfig(BYTE u8IdxWin, ST_WINDOW_INFO stWinInfo)
{
    BYTE    u8ScNo = 0;
    BYTE    u8ScIndex =0;
    DWORD   u32SCRegBase;
    WORD    uwScMask = g_DisplayWindow[u8IdxWin].u16DwScMaskOut;
    BYTE    u8ScIndxBuff[2];

    u8ScIndxBuff[0] = 0xFF;
    u8ScIndxBuff[1] = 0xFF;

    while(uwScMask!=0)
    {
        if(uwScMask & BIT0)
        {
            u8ScIndxBuff[u8ScNo]= u8ScIndex;
            u8ScNo++;
        }
        uwScMask >>=1;
        u8ScIndex ++;

        if(u8ScNo >2)
            break;
    }

    if(IsWindowLOnly(u8IdxWin) || IsWindowROnly(u8IdxWin))
    {

        msDrvMWEWindowConfig(u8ScIndxBuff[0], eMWE_WIN1, stWinInfo.u16x, stWinInfo.u16y, stWinInfo.u16Width, stWinInfo.u16Height);

    }
    else
    {
        // if(stWinInfo.u16x +  stWinInfo.u16Width
        if(IsScalerLeftPos(u8ScIndxBuff[0]))
        {
             msDrvMWEWindowConfig(u8ScIndxBuff[0], eMWE_WIN1, stWinInfo.u16x, stWinInfo.u16y, stWinInfo.u16Width, stWinInfo.u16Height);
        }
        else
        {

        }
    }


}
#endif
enum ColorTableTypes{
    VOP_CM = 0,             //18 bytes
    XYYCC_CM,               //18 bytes
    RGB_IHC,                //6 bytes
    RGB_ICC,                //6 bytes
    RGB_IBC,                //6 bytes
    RGB_OFFSET,             //6 bytes
    PRE_CON,                //6 bytes
    POST_CON,               //6 bytes
    YUV_ICC,                //n bytes, default 6 bytes
    YUV_IBC,                //n bytes, default 6 bytes
    YUV_IHC,                //n bytes, default 6 bytes
    YUV_FCC,                //5 bytes, FCC Num/Strength/Cb/Cr/Distance
    YUV_DLC,                //16 bytes
    DE_GAMMA,               //Reserved
    FIX_GAMMA,              //Reserved
    POST_GAMMA,             // 3*320 bytes
    NUM_OF_COLORTYPES,
};

void SetColor (BYTE type, int len, BYTE* pTbl)
{
    printf("SetColor type = %d, len = %d\n", type, len);
    switch(type)
    {
        case VOP_CM:
        {
            short  ColorMatrix[9];
            int idx = 0;
            for (idx = 0; idx < 9; idx++)
            {
                 ColorMatrix[idx] = ((*(pTbl+2*idx) << 8) | *(pTbl+2*idx+1));
            }
            msWriteColorMatrix(USER_PREF_WIN_SEL, ColorMatrix);
            msEnableColorMatrix(USER_PREF_WIN_SEL, TRUE, TRUE);
        }
        break;
        case XYYCC_CM:
        {
            short  ColorMatrix[9];
            int idx = 0;
            for (idx = 0; idx < 9; idx++)
            {
                 ColorMatrix[idx] = ((*(pTbl+2*idx) << 8) | *(pTbl+2*idx+1));
            }
            msWritexvYccColorMatrix(USER_PREF_WIN_SEL, ColorMatrix);
            msEnablexvYccColorMatrix(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        case RGB_IHC:
        {
            int idx = 0;
            for (idx = 0; idx<len; idx++)
            {
               msDrvAdjustCustomHue(USER_PREF_WIN_SEL, *(pTbl+idx), idx);
            }
            msDrvCustomHueEnable(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        #if (CHIP_ID == MST9U3)
        case RGB_ICC:
        {
            int idx = 0;
            for (idx = 0; idx<len; idx++)
            {
               msAPI_AdjustCustomSatRGBMode(USER_PREF_WIN_SEL, *(pTbl+idx), idx);
            }
            msAPI_CustomSatEnableRGBMode(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        case RGB_IBC:
        {
            int idx = 0;
            for (idx = 0; idx<len; idx++)
            {
               msAPI_AdjustCustomBriRGBMode(USER_PREF_WIN_SEL, *(pTbl+idx), idx);
            }
            msAPI_CustomBriEnableRGBMode(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        #endif
        case RGB_OFFSET:  //6 bytes
        {
            WORD rOffset, gOffset, bOffset;
            rOffset = (*(pTbl+1) << 8) | *pTbl;
            gOffset = (*(pTbl+3) << 8) | *(pTbl+2);
            bOffset = (*(pTbl+5) << 8) | *(pTbl+4);
            msDrvAdjustBlacklevel(USER_PREF_WIN_SEL, rOffset, gOffset, bOffset);
            msDrvBlackLevelEnable(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        case PRE_CON:                //6 bytes
        {
            WORD rCon, gCon, bCon;
            rCon = (*(pTbl+1) << 8) | *pTbl;
            gCon = (*(pTbl+3) << 8) | *(pTbl+2);
            bCon = (*(pTbl+5) << 8) | *(pTbl+4);
            msUsePostContrast(USER_PREF_WIN_SEL, FALSE);
            msSetRGBContrast(USER_PREF_WIN_SEL, rCon, gCon, bCon);
            msAdjustRGBContrast(USER_PREF_WIN_SEL);
        }
        break;
        case POST_CON:                //6 bytes
        {
            WORD rCon, gCon, bCon;
            rCon = (*(pTbl+1) << 8) | *pTbl;
            gCon = (*(pTbl+3) << 8) | *(pTbl+2);
            bCon = (*(pTbl+5) << 8) | *(pTbl+4);
            msUsePostContrast(USER_PREF_WIN_SEL, TRUE);
            msSetRGBContrast(USER_PREF_WIN_SEL, rCon, gCon, bCon);
            msAdjustRGBContrast(USER_PREF_WIN_SEL);
        }
        break;
        case YUV_ICC:  //6 bytes
        {
            int idx = 0;
            for (idx = 0; idx<len; idx++)
            {
               msDrvAdjustCustomSaturation(USER_PREF_WIN_SEL, *(pTbl+idx), idx);
            }
            msDrvCustomSaturationEnable(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        case YUV_IBC:  //6 bytes
        {
            int idx = 0;
            for (idx = 0; idx<len; idx++)
            {
               msDrvAdjustCustomBrightness(USER_PREF_WIN_SEL, *(pTbl+idx), idx+1);
            }
            msDrvCustomBrightnessEnable(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        case YUV_IHC:  //6 bytes
        {
            int idx = 0;
            for (idx = 0; idx<len; idx++)
            {
                msAPI_AdjustVIPHue(USER_PREF_WIN_SEL, *(pTbl+idx), idx);
            }
            msAPI_VIPHueEnable(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        case YUV_FCC:  //5 bytes, FCC Num/Strength/Cb/Cr/Distance
        {
            msDrvAdjustFCC(USER_PREF_WIN_SEL, *pTbl, *(pTbl+1), *(pTbl+2), *(pTbl+3), *(pTbl+4));
            msDrvFCCEnable(USER_PREF_WIN_SEL, *pTbl, TRUE);
        }
        break;
        case YUV_DLC:  //16 bytes
        {
            msDrvSetDLCCurve(USER_PREF_WIN_SEL, pTbl);
            msAPI_SetDLCEnable(USER_PREF_WIN_SEL, TRUE);
        }
        break;
        case POST_GAMMA:  //960 bytes
        {
            BYTE *StoredPostGamma[][3] =
            {
                {
                    pTbl,
                    pTbl+320,
                    pTbl+640,
                },
            };
            msAPI_GammaLoadTbl_256E_10B(USER_PREF_WIN_SEL,  StoredPostGamma[0]);
            msAPI_GammaEnable(USER_PREF_WIN_SEL, TRUE);
        }
        break;

    }
}

void CloseColor(BYTE type, int len, BYTE* pTbl)
{
    printf("CloseColor type = %d, len = %d\n", type, len);
    switch(type)
    {
        case VOP_CM:
        {
            msEnableColorMatrix(USER_PREF_WIN_SEL, FALSE, TRUE);
        }
        break;
        case XYYCC_CM:
        {
            msEnablexvYccColorMatrix(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        case RGB_IHC:
        {
            msDrvCustomHueEnable(USER_PREF_WIN_SEL, FALSE);
        }
        #if (CHIP_ID == MST9U3)
        case RGB_ICC:
        {
            msAPI_CustomSatEnableRGBMode(USER_PREF_WIN_SEL, FALSE);
        }
        case RGB_IBC:
        {
            msAPI_CustomBriEnableRGBMode(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        #endif
        case RGB_OFFSET:  //6 bytes
        {
            msDrvBlackLevelEnable(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        case PRE_CON:                //6 bytes
        {
            msEnablePerContrast(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        case POST_CON:                //6 bytes
        {
            msEnablePostContrast(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        case YUV_ICC:  //6 bytes
        {
            msDrvCustomSaturationEnable(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        case YUV_IBC:  //6 bytes
        {
            msDrvCustomBrightnessEnable(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        case YUV_IHC:  //6 bytes
        {
            msAPI_VIPHueEnable(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        case YUV_FCC:  //5 bytes, FCC Num/Strength/Cb/Cr/Distance
        {
            msDrvFCCEnable(USER_PREF_WIN_SEL, *pTbl, FALSE);
        }
        break;
        case YUV_DLC:  //16 bytes
        {
            msAPI_SetDLCEnable(USER_PREF_WIN_SEL, FALSE);
        }
        break;
        case POST_GAMMA:
        {
            msAPI_GammaEnable(USER_PREF_WIN_SEL, FALSE);
        }
        break;

    }
}

void TurnOffWholeUserColorMode(void)
{
    BYTE idx = 0;

    for (idx=0; idx<NUM_OF_COLORTYPES; idx++)
    {
        if (YUV_FCC != idx)
        {
            CloseColor(idx, 0, NULL);
        }
        else
        {
            BYTE FCCRegion = 0;

            for (FCCRegion=0; FCCRegion<8; FCCRegion++)
            {
                CloseColor(idx, 0, &FCCRegion);
            }
        }
    }
}
void SetupUserColorMode(BOOL IsEn, BYTE* pUserColorMode)
{
    printf("SetupUserColorMode IsEn = %d, pUserColorMode = %X\n", IsEn, pUserColorMode);
    BYTE numOfTbls = 0, cntTbls = 0;
    long lenOfTbls = 0;
    long idx = 0;
    BYTE header, ender, tblType;
    BYTE *pTbl;
    int tblLen;
    if (NULL == pUserColorMode)
        return;

    /* Table Structure
      0. numOfTbls (1 byte)
      1. lenOfTbls (4 bytes)
      2. tblHeader (1 bytes) : 0xAB
      4. tblType (1 bytes)
      5. tblLen (2 bytes)
      6. tblData (n bytes)
      7. tblEnder(1 bytes) : 0xBA
    */
    TurnOffWholeUserColorMode();

    numOfTbls = pUserColorMode[0];
    lenOfTbls = (pUserColorMode[1]<<24) + (pUserColorMode[2]<<16) + (pUserColorMode[3]<<8) + pUserColorMode[4];
    idx = 5;

    while(idx < lenOfTbls && cntTbls < numOfTbls)
    {
        header = pUserColorMode[idx];
        tblType = pUserColorMode[idx + 1];
        tblLen = (pUserColorMode[idx + 2] << 8) | pUserColorMode[idx + 3] ;
        ender = pUserColorMode[idx + 4 + tblLen];
        pTbl = &pUserColorMode[idx + 4];
        // Start to write data by table type.
        if (IsEn && header == 0xAB && ender == 0xBA)
            SetColor (tblType, tblLen, pTbl);
        else
            CloseColor(tblType, tblLen, pTbl);
        // End of write data.
        cntTbls++;
        idx += tblLen + 5; // header, ender, tblTypes, tblLen occupied 5 bytes
    }
}


