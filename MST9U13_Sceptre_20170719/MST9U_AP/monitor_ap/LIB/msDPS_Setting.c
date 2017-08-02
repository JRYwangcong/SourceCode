
#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Panel.h"
#include "Ms_rwreg.h"
#include "NVRam.h"
#include "Mode.h"
#include "UserPrefDef.h"
#include "Global.h"
#include "misc.h"
#include "menudef.h"
#include "menufunc.h"
#include "MDebug.h"
#include "msOSD.h"
#include "msDPS_Handler.h"
#include "msDPS_Setting.h"
#include "msACE.h"
#include "Adjust.h"
#include "MsDLC_Setting.h"
#include "Common.h"
#include "appWindow.h"

#define MSDPSSETTING_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG && MSDPSSETTING_DEBUG
#define MSDPSSETTING_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MSDPSSETTING_PRINT(format, ...)
#endif

//===================================================================================================================================================================:
#if ENABLE_DPS
//===================================================================================================================================================================:
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///// Y-mapping Table ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
BYTE code DPS_Lumi_Table[17][11] =
{
#if 0
 {255,235,215,195,176,156,136,116, 96, 76, 57,},
 {208,192,176,160,144,127,111, 95, 79, 62, 46,},
 {173,159,145,132,119,106, 92, 79, 65, 51, 39,},
 {143,131,120,109, 98, 87, 76, 65, 54, 43, 32,},
 {118,108, 99, 90, 81, 72, 63, 54, 44, 35, 26,},
 { 96, 88, 81, 74, 66, 59, 51, 44, 36, 29, 22,},
 { 76, 70, 64, 58, 52, 47, 41, 35, 29, 23, 17,},
 { 60, 55, 50, 46, 41, 36, 32, 27, 23, 18, 13,},
 { 45, 42, 38, 35, 31, 28, 24, 21, 17, 14, 10,},
 { 34, 31, 29, 26, 23, 21, 18, 16, 13, 10,  8,},
 { 25, 23, 21, 19, 17, 15, 13, 11,  9,  7,  6,},
 { 17, 15, 14, 13, 11, 10,  9,  8,  6,  5,  4,},
 { 10,  9,  9,  8,  7,  6,  5,  5,  4,  3,  2,},
 {  6,  5,  5,  4,  4,  3,  3,  3,  2,  2,  1,},
 {  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,},
 {  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
 {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
#else
 {255,229,205,179,155,129,104, 78, 53, 26,  1,},
 {223,201,179,157,135,113, 91, 68, 46, 23,  1,},
 {191,172,154,134,116, 97, 78, 58, 40, 20,  1,},
 {164,147,132,115, 99, 83, 67, 50, 34, 16,  1,},
 {138,124,111, 97, 83, 70, 56, 42, 28, 14,  1,},
 {113,102, 91, 79, 68, 57, 46, 34, 23, 12,  0,},
 { 93, 84, 75, 66, 57, 47, 38, 28, 19, 10,  0,},
 { 74, 67, 60, 52, 45, 37, 30, 23, 15,  8,  0,},
 { 58, 52, 47, 41, 35, 29, 24, 18, 12,  6,  0,},
 { 43, 39, 35, 30, 26, 22, 17, 13,  9,  4,  0,},
 { 31, 28, 25, 22, 19, 15, 12,  9,  6,  3,  0,},
 { 22, 20, 17, 15, 13, 11,  9,  7,  4,  2,  0,},
 { 14, 13, 11, 10,  9,  7,  6,  4,  3,  1,  0,},
 {  8,  7,  6,  5,  5,  4,  3,  2,  2,  1,  0,},
 {  3,  3,  2,  2,  2,  2,  1,  1,  1,  0,  0,},
 {  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,},
 {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,},
 #endif
};

//===================================================================================================================================================================:
// XDATA variable Share with DLC (back up)
//===================================================================================================================================================================:
///// ISR //////////////
xdata DWORD  ISR_LumaHistogram32H[32]; // >>6
xdata DWORD  ISR_HistogramTotalPix; // >>6
xdata DWORD  ISR_HistogramTotalLuma; // >>14
xdata BYTE  ISR_HistogramMax;
xdata BYTE  ISR_HistogramMin;

xdata WORD  REG_DPS_UserBrightContlGain;// 0~100: 0~100%
//DPS_AggrPowerSaveMode_EN
xdata WORD  REG_DPS_UserBrightLimitGain;
xdata WORD  REG_DPS_AggressiveYgainGain;//  256   // 0~256: 0~100%
xdata WORD  REG_DPS_AggressivePdutyGain;//  256   // 0~256: 0~100%
xdata WORD  REG_DPS_AggressiveGainRange;//  240   // apply AggressiveGain when ImgYmax < Range

xdata BYTE  REG_DPS_YOUT_ENABLE; // Color step from 0~255 to 16~235
xdata BYTE  REG_DPS_BW_MODE_EN;
xdata BYTE  REG_DPS_BW_MODE_Gain;
xdata BOOL  REG_DPS_RUN_Flag =FALSE;

///// Global Variables ////////////////////////////////////////////////////////////
///// ISR /////////////////////////////////////////////////////////////////////////
xdata BYTE  ISR_preLoopCount = 0;
xdata BYTE  ISR_curLoopCount = 0;
///// DPS /////////////////////////////////////////////////////////////////////////
xdata BYTE  Flag_DPS_Latch_Busy = 0;

xdata BYTE  REG_MouseCount;

xdata BYTE  DPS_YminForWindowSave_EN = 0;
xdata BYTE  DPS_HistPseuMax_MOD1_EN = 0; // Mouse Detection
xdata BYTE  DPS_HistPseuMax_MOD2_EN = 0;
xdata BYTE  REG_DPS_PowerLevel;  //0~FF
xdata WORD  REG_DPS_WindowSaveDensityTH = 0;
xdata BYTE  REG_DPS_WindowSaveYavgLimit = 0;
xdata WORD  REG_DPS_WindowSaveStaticTH = 0;
xdata WORD  REG_DPS_WindowSavePatTH = 0;
xdata BYTE  REG_DPS_WindowSaveMotionDiff = 0;
xdata BYTE  REG_DPS_WindowSaveHistDiffCouThd = 0;
xdata WORD  REG_DPS_WindowSaveFrameNum = 0;
#if DPS_InputProcess_Mode
#define REG_DPS_CutValue_Thd  2
xdata BYTE REG_DPS_TargetYmax = 0;
xdata BYTE REG_DPS_PreYmax = 0;
#endif
xdata BYTE REG_OnePartWinIs_YUV = 0;
#if DPS_FAD_OnOff_Mode
#define REG_DPS_UpdateFadCyc 100
#define REG_DPS_UpdateFadCyc_OFFMode 100
#define REG_DPS_ExitFad_Diff_Ygain 15
#define REG_DPS_ExitFad_Diff_PWM  5
#define REG_DPS_YgainFadStep  4
#define REG_DPS_PWMFadStep  2
#define RGG_DPS_Ygain_Diff  10
#define RGG_DPS_PWM_Diff  10
xdata WORD PreDPSGain = 1024;
xdata WORD PreDPSPWM = 255;
xdata WORD InitialYgain = 1024;
xdata short InitialPWM = 255;
xdata WORD FadYgain = 1024;
xdata short FadPWM = 0xFF;
xdata WORD FadYgainCycLoopCount = 0;
xdata WORD FadPWMCycLoopCount = 0;
xdata BYTE UpdaFadYgainFlag = 1;
xdata BYTE UpdaFadPWMFlag = 1;
xdata BYTE REG_DPS_Off_FadFlag = 0;   
xdata BYTE REG_DPS_On_FadFlag = 0;
xdata BYTE exDPSRunOnFlag = 0 ;
xdata WORD  DPSGain = 1024;
xdata short  DPSPWM = 255;
#endif

//===================================================================================================================================================================:
//////////////////////////////////////////////////////////////////////////////////
///// Gen Output Interface ///////////////////////////////////////////////////////////
///// 1. Backlight adjust /////////////////////////////////////////////////////////////
///// 2. Y-gain adjust ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void msDPS_AdjustBacklight ( BYTE BLValue ) // 0~255 <--> 0%~100%
{

#if DPS_FAD_OnOff_Mode
    xdata WORD fadPWMDiff;
    xdata short TempVar;

    DPSPWM = BLValue;
    if( REG_DPS_On_FadFlag )
    {
        if ( UpdaFadPWMFlag )
        {
            FadPWM = DPSPWM;
            PreDPSPWM = DPSPWM;

            if ( DPSPWM != 255 )
                UpdaFadPWMFlag = 0;

            FadPWM = MIN2 ( FadPWM, 255 );
            FadPWM = MAX2 ( FadPWM, 0 );
        }

        FadPWMCycLoopCount ++ ;

        if(FadPWMCycLoopCount != REG_DPS_UpdateFadCyc )
        {
            TempVar = ( WORD ) ( 255 * USER_PREF_BRIGHTNESS ) / 100  ;
            TempVar = MIN2 ( TempVar,       0xFF );
            TempVar = MAX2 ( TempVar,       0x00 );
            TempVar = ( WORD ) ( ABS2( FadPWM, TempVar ) ) / REG_DPS_UpdateFadCyc;
            fadPWMDiff = MAX2 ( TempVar + 1, REG_DPS_PWMFadStep );

            if ( InitialPWM != BLValue )
            {
                if ( InitialPWM < BLValue )
                    InitialPWM = InitialPWM + fadPWMDiff;
                else
                    InitialPWM = InitialPWM - fadPWMDiff;
            }

            InitialPWM = MIN2 ( InitialPWM,        255 ); //--HW Limitation
            InitialPWM = MAX2 ( InitialPWM, 0 ); //--HW Limitation
            DPSPWM = InitialPWM;
        }
    }
    u8DPSCurBacklight = DPSPWM;

#if DEBUG_PWM
    if (u8DPSCurBacklight != PreDPSPWM)
        MSDPSSETTING_PRINT("\r\n PWM (2) %x\n", u8DPSCurBacklight);  
#endif
    msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
    PreDPSPWM = BLValue;
#else

    #if DEBUG_PWM
    if (u8DPSCurBacklight != BLValue)
        MSDPSSETTING_PRINT("\r\n PWM (1) %x\n", BLValue);  
    #endif
    msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
    u8DPSCurBacklight = BLValue;
    
#endif    
}

void msDPS_AdjustYgain ( WORD YgainValue ) // 2.10 format
{
#if DEBUG_HISTOGRAM
    BYTE xdata i;
    MSDPSSETTING_PRINT("\r\n ===== Histogram Start ======\n");
    for ( i = 0; i < 32; i++ )
        MSDPSSETTING_PRINT("\r\n  %x\n", ISR_LumaHistogram32H[i]);
    MSDPSSETTING_PRINT("\r\n ===== Histogram End ======\n");
#endif

#if DEBUG_HISTOGRAM_PAM
MSDPSSETTING_PRINT("\r\n TotalPix %x\n", ISR_HistogramTotalPix);
MSDPSSETTING_PRINT("\r\n TotalLuma %x\n", ISR_HistogramTotalLuma);
MSDPSSETTING_PRINT("\r\n Max %x\n", ISR_HistogramMax);
MSDPSSETTING_PRINT("\r\n Min %x\n", ISR_HistogramMin);
#endif

#if DPS_FAD_OnOff_Mode
    xdata WORD fadYgainDiff;
    xdata WORD TempVar;

    DPSGain = YgainValue;
    if ( REG_DPS_On_FadFlag )
    {
        if ( UpdaFadYgainFlag )
        {
            FadYgain  = DPSGain;
            PreDPSGain = DPSGain;

            if ( DPSGain != 1024 )
            {
                ;
            }
            UpdaFadYgainFlag = 0;
            FadYgain = MIN2 ( FadYgain, 2047 );
            FadYgain = MAX2 ( FadYgain, 1024 );
        }

        FadYgainCycLoopCount ++ ;

        if ( FadYgainCycLoopCount != REG_DPS_UpdateFadCyc )
        {
            TempVar = ( WORD ) ( 1024 * CURRENT_CONTRAST ) /  128;
            TempVar = MIN2 ( TempVar,       2047 );
            TempVar = MAX2 ( TempVar,       1024 );
            TempVar = ( WORD ) ( ABS2 ( FadYgain , TempVar ) ) / REG_DPS_UpdateFadCyc;
            fadYgainDiff = MAX2 ( TempVar + 1 , REG_DPS_YgainFadStep );

            if ( InitialYgain != YgainValue )
            {
                if ( InitialYgain < YgainValue )
                    InitialYgain = InitialYgain + fadYgainDiff;
                else
                    InitialYgain = InitialYgain - fadYgainDiff;
            }

            InitialYgain = MIN2 ( InitialYgain,       2047 );
            InitialYgain = MAX2 ( InitialYgain,       1024 );
            DPSGain = InitialYgain;

            if ( ( ABS2 ( InitialYgain , PreDPSGain ) ) < REG_DPS_ExitFad_Diff_Ygain &&
                 ( ABS2 ( InitialPWM, PreDPSPWM ) ) < REG_DPS_ExitFad_Diff_PWM &&
                 ( UpdaFadYgainFlag != 0 ) &&
                 ( UpdaFadPWMFlag != 0 ) &&
                 ( DPSGain != 1024 ) &&
                 ( DPSPWM != 255 )
               )
            {
                REG_DPS_On_FadFlag = 0;
                UpdaFadPWMFlag = 0;
                UpdaFadYgainFlag = 0;
            }
        }
        else
            REG_DPS_On_FadFlag = 0;
    }
    else
        REG_DPS_On_FadFlag = 0;
    
    u16DPSCurYgain = DPSGain;
#if DEBUG_Ygain
    if (u16DPSCurYgain != PreDPSGain)
        MSDPSSETTING_PRINT("\r\n Ygain(2)%x\n", u16DPSCurYgain);
#endif
    msAPI_AdjustContrast(USER_PREF_WIN_SEL , CURRENT_CONTRAST);
    PreDPSGain = YgainValue;
#else

    #if DEBUG_Ygain
    if (u16DPSCurYgain != YgainValue)
        MSDPSSETTING_PRINT("\r\n Ygain(1) %x\n", YgainValue);
    #endif
    msAPI_AdjustContrast(USER_PREF_WIN_SEL , CURRENT_CONTRAST);
    u16DPSCurYgain = YgainValue;
#endif

}
//===================================================================================================================================================================:
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///// Function Declaration ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void msDPS_Interrupt(void )
{

    if(ISR_HistogramTotalPix == 0)
        return;

#if DPS_InputProcess_Mode
        REG_DPS_PreYmax = MIN2 ( REG_DPS_PreYmax, 255);
        REG_DPS_PreYmax = MAX2 ( REG_DPS_PreYmax, 0  );
        if(ABS2(REG_DPS_PreYmax, REG_DPS_TargetYmax) > REG_DPS_CutValue_Thd)
        {
            ISR_HistogramMax = REG_DPS_PreYmax;
            REG_DPS_TargetYmax = ISR_HistogramMax;
        }
        else
            ISR_HistogramMax = REG_DPS_TargetYmax;
        REG_DPS_PreYmax = ISR_HistogramMax;
#endif
    ISR_curLoopCount++;
}

void msYuvInputProcess(BYTE En)
{
    //0:YUV is 0~255 1:YUV is 16~235
    if(En)
        REG_DPS_YOUT_ENABLE = 1;
    else
        REG_DPS_YOUT_ENABLE = 0;
}

//////////////////////////////////////////////////////////////////////////////////
///// DPS initail ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void msDPS_ParamInit( void )
{
    //Default brightness 100

    //CurBacklight = 0xFF;
#if DPS_UserBrightControl_EN
    REG_DPS_UserBrightContlGain = USER_PREF_BRIGHTNESS;//0~100//
#else
    REG_DPS_UserBrightContlGain = 100;
#endif

#if DPS_AggrPowerSaveMode_EN
    #if DPS_UserBrightControl_EN
        REG_DPS_UserBrightLimitGain = USER_PREF_BRIGHTNESS * 256 / 100;
    #else
        REG_DPS_UserBrightLimitGain = 256;//"AggressivePdutyGain" it depend on "OSD bri", when OSD bri it sets lower than "AggressivePdutyGain", it need to follow "OSD bri".
    #endif
    REG_DPS_AggressiveYgainGain = 256;// 0~256: 0~100%
    REG_DPS_AggressivePdutyGain = 210;//180;// 0~256: 0~100%
    REG_DPS_AggressiveGainRange = 255;// apply AggressiveGain when ImgYmax < Range
#else
    REG_DPS_UserBrightLimitGain = 256;
    REG_DPS_AggressiveYgainGain = 256;
    REG_DPS_AggressivePdutyGain = 256;
    REG_DPS_AggressiveGainRange = 128;
#endif
    REG_DPS_BW_MODE_EN = 0;

    if(REG_DPS_BW_MODE_EN)
        REG_DPS_BW_MODE_Gain = 230;

    //msYuvInputProcess(0);//0:YUV is 0~255 1:YUV is 16~235

    REG_DPS_RUN_Flag = 1;

    DPS_YminForWindowSave_EN = 0;
    if(DPS_YminForWindowSave_EN)
    {
        REG_DPS_WindowSaveDensityTH = 200;
        REG_DPS_WindowSaveYavgLimit = 16;
        REG_DPS_WindowSaveStaticTH  = 95; //0~100
        REG_DPS_WindowSavePatTH = 90; //0~100
        REG_DPS_WindowSaveMotionDiff = 2; //0~31
        REG_DPS_WindowSaveHistDiffCouThd = 1;
        REG_DPS_WindowSaveFrameNum = 100;
        REG_DPS_PowerLevel = 20;  //0~5
    }

    DPS_HistPseuMax_MOD1_EN = 0;
    DPS_HistPseuMax_MOD2_EN = 0;
    if(DPS_HistPseuMax_MOD1_EN)
        DPS_HistPseuMax_MOD2_EN = 0;

    if(DPS_HistPseuMax_MOD1_EN)
        REG_MouseCount = 1;

#if DPS_InputProcess_Mode
    REG_DPS_TargetYmax = ISR_HistogramMax;//scReadByte ( REG_ADDR_MAIN_MAX_VALUE ); //scRegs[0x3A];
    REG_DPS_PreYmax = REG_DPS_TargetYmax;
#endif

#if DPS_FAD_OnOff_Mode
    exDPSRunOnFlag = 1;
    REG_DPS_On_FadFlag  = 1;
    REG_DPS_Off_FadFlag = 0;
    UpdaFadYgainFlag = 1;
    UpdaFadPWMFlag = 1;
    FadPWMCycLoopCount = 0;
    FadYgainCycLoopCount = 0;
    InitialPWM = DPSPWM;//255;//(255 * UserPrefBrightness ) / 100;
    InitialYgain = DPSGain; //1024;//(WORD)(1024 * UserPrefContrast) / 128 ;
#endif

    Flag_DPS_Latch_Busy = 0;
}

//////////////////////////////////////////////////////////////////////////////////
///// DPS On <--> Off Settings ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#if DPS_FAD_OnOff_Mode
void msDPS_Off_Fad_Handler ( BYTE En )
{
    xdata WORD TempVar1, Diff_Y, Diff_PWM ;

    //WAIT_V_OUTPUT_BLANKING_START();
    if ( En && exDPSRunOnFlag )
    {
        FadPWMCycLoopCount ++;
        FadYgainCycLoopCount ++;
        TempVar1 = 0xFF;
        TempVar1 = ( WORD ) ( ABS2 ( InitialPWM, TempVar1 ) ) / REG_DPS_UpdateFadCyc_OFFMode;
        Diff_PWM = MAX2 ( TempVar1, REG_DPS_PdutyDiffMinTH );
        TempVar1 = 1024;
        TempVar1 = ( WORD ) ( ABS2 ( InitialYgain, TempVar1 ) ) / REG_DPS_UpdateFadCyc_OFFMode;
        Diff_Y = MAX2 ( TempVar1, REG_DPS_YgainDiffMinTH );

        if ( DPSGain != 1024 )
        {
            if ( DPSGain > 1024 )
            {
                DPSGain = DPSGain - Diff_Y;
            }
            else
            {
                DPSGain = DPSGain + Diff_Y;
            }
        }

        DPSGain = MIN2 ( DPSGain,       2047 );
        DPSGain = MAX2 ( DPSGain,       1024 );

        if ( DPSPWM != 0xFF )
        {
            if ( DPSPWM >  0xFF )
                DPSPWM =  DPSPWM - Diff_PWM;
            else
                DPSPWM =  DPSPWM + Diff_PWM;
        }

        DPSPWM = MIN2 ( DPSPWM,       0xFF );
        DPSPWM = MAX2 ( DPSPWM,       0x00 );

        if ( FadPWMCycLoopCount == REG_DPS_UpdateFadCyc_OFFMode ||
             FadYgainCycLoopCount == REG_DPS_UpdateFadCyc_OFFMode ||
             ( DPSGain == 0xFF  && DPSGain == 1024 ) )
        {
            REG_DPS_Off_FadFlag = 0;
            DPSPWM = 0xFF;
            DPSGain = 1024;
        }

        msDPS_AdjustYgain ( DPSGain );
        msDPS_AdjustBacklight (DPSPWM);
    }
}
#endif


void msDPS_On_Setting( void )
{
    msDPS_ParamInit();
    msSetWinHistogramRange();

 }
//===================================================================================================================================================================:
void msDPS_Off_Setting ( void )
{
#if DPS_FAD_OnOff_Mode
    if ( !exDPSRunOnFlag )
        return;
#endif

    REG_DPS_RUN_Flag = 0;
    ISR_curLoopCount = 0;
    ISR_preLoopCount = 0;

#if DPS_FAD_OnOff_Mode
    REG_DPS_Off_FadFlag  = 1;
    REG_DPS_On_FadFlag = 0;
    FadPWMCycLoopCount = 0;
    FadYgainCycLoopCount = 0;
    InitialPWM = DPSPWM;
    InitialYgain = DPSGain;
#else
    msDPS_AdjustYgain ( 0x400 );
    msDPS_AdjustBacklight (0xFF);
#endif
}

void msDPS_MultiWinDataHandler(void)
{
    BYTE ucTmp , u8WinIdx;

    for ( ucTmp = 0; ucTmp < 32; ucTmp++ )
        ISR_LumaHistogram32H[ucTmp] = 0;

    ISR_HistogramTotalPix = 0;
    ISR_HistogramTotalLuma = 0;
    ISR_HistogramMax = 0;
    ISR_HistogramMin = 0xFF;
    /*
    MSDPSSETTING_PRINT("\r\n ============\n", u16Total_Count[u8WinIdx]);
    for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
        MSDPSSETTING_PRINT("\r\n %x\n", u16Total_Count[u8WinIdx]);
    MSDPSSETTING_PRINT("\r\n ============\n", u16Total_Count[u8WinIdx]);
    */
    for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
    {
        if(msAPIWinGetScDwEnable(u8WinIdx))
        {
            for ( ucTmp = 0; ucTmp < 32; ucTmp++ )
                ISR_LumaHistogram32H[ucTmp] += u32Histogram[u8WinIdx][ucTmp]; // "ISR_LumaHistogram32H" need to change "DWORD"

            ISR_HistogramTotalPix += g_u16Total_Count[u8WinIdx];
            ISR_HistogramTotalLuma += g_u16Total_Weight[u8WinIdx] ;

            if(ISR_HistogramMax < g_u8Ymax[u8WinIdx])
                ISR_HistogramMax = g_u8Ymax[u8WinIdx];
            if(ISR_HistogramMin > g_u8Ymin[u8WinIdx])
                ISR_HistogramMin = g_u8Ymin[u8WinIdx];
        }
    }

}

void msGetColorDomain(void)
{
    BYTE u8PortIdx = MapWin2Port(USER_PREF_WIN_SEL);
    EN_COLOR_RANGE_TYPE u8ColorRange = (EN_COLOR_RANGE_TYPE)msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[u8PortIdx].eIPMux)).ucColorRange;

    if(u8ColorRange == COMBO_COLOR_RANGE_LIMIT)
            REG_OnePartWinIs_YUV = 1;
    else  
        REG_OnePartWinIs_YUV = 0;

        msYuvInputProcess(REG_OnePartWinIs_YUV);//0:YUV is 0~255 1:YUV is 16~235
    }


void msDPSHandler(void)
{
    if(!REG_DPS_RUN_Flag)
        return;

    msGetColorDomain();

    //msGetWinHistogramData();

    msDPS_MultiWinDataHandler();

    msDPS_Interrupt();

    msDPS_Handler();

}

//===================================================================================================================================================================:
#endif // #if EnableDPS
//===================================================================================================================================================================:
