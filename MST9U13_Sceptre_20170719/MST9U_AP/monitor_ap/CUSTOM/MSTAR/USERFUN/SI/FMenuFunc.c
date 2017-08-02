#define _FMENUFUNC_C
#include "datatype.h"
#include "Board.h"
#include "menudef.h"
#include "FMenuFunc.h"
#include "Global.h"
#include "Ms_rwreg.h"
#include "misc.h"
#include "Adjust.h"
#include "msACE.h"
#include "menufunc.h"
#include "ms_reg.h"
#include "UserPref.h"
#include "AutoFunc.h"
#include "msOSD.h"
#include "drvADC.h"
#include "appWindow.h"

extern WORD DecIncValue(MENU_ITEM_ACTION action, WORD value, WORD minValue, WORD maxValue, BYTE step);
extern void SaveFactorySetting(void);

WORD GetRColor6500KValue(void)
{
    return USER_PREF_RED_COLOR_6500K;
}

WORD GetGColor6500KValue(void)
{
    return USER_PREF_GREEN_COLOR_6500K;
}

WORD GetBColor6500KValue(void)
{
    return USER_PREF_BLUE_COLOR_6500K;
}

Bool AdjustRedColor6500K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_RED_COLOR_6500K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_RED_COLOR_6500K)
        return FALSE;
    USER_PREF_RED_COLOR_6500K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_6500K, USER_PREF_GREEN_COLOR_6500K, USER_PREF_BLUE_COLOR_6500K);
    return TRUE;
}

Bool AdjustGreenColor6500K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_GREEN_COLOR_6500K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_GREEN_COLOR_6500K)
        return FALSE;
    USER_PREF_GREEN_COLOR_6500K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_6500K, USER_PREF_GREEN_COLOR_6500K, USER_PREF_BLUE_COLOR_6500K);
    return TRUE;
}

Bool AdjustBlueColor6500K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_BLUE_COLOR_6500K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_BLUE_COLOR_6500K)
        return FALSE;
    USER_PREF_BLUE_COLOR_6500K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_6500K, USER_PREF_GREEN_COLOR_6500K, USER_PREF_BLUE_COLOR_6500K);
    return TRUE;
}

WORD GetRColor9300KValue(void)
{
    return USER_PREF_RED_COLOR_9300K;
}

WORD GetGColor9300KValue(void)
{
    return USER_PREF_GREEN_COLOR_9300K;
}

WORD GetBColor9300KValue(void)
{
    return USER_PREF_BLUE_COLOR_9300K;
}

Bool AdjustRedColor9300K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_RED_COLOR_9300K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_RED_COLOR_9300K)
        return FALSE;
    USER_PREF_RED_COLOR_9300K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_9300K, USER_PREF_GREEN_COLOR_9300K, USER_PREF_BLUE_COLOR_9300K);
    return TRUE;
}

Bool AdjustGreenColor9300K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_GREEN_COLOR_9300K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_GREEN_COLOR_9300K)
        return FALSE;
    USER_PREF_GREEN_COLOR_9300K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_9300K, USER_PREF_GREEN_COLOR_9300K, USER_PREF_BLUE_COLOR_9300K);
    return TRUE;
}

Bool AdjustBlueColor9300K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_BLUE_COLOR_9300K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_BLUE_COLOR_9300K)
        return FALSE;
    USER_PREF_BLUE_COLOR_9300K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_9300K, USER_PREF_GREEN_COLOR_9300K, USER_PREF_BLUE_COLOR_9300K);
    return TRUE;
}

#if Enable_ColorTemp_5700K
WORD GetRColor5700KValue(void)
{
    return USER_PREF_RED_COLOR_5700K;
}

WORD GetGColor5700KValue(void)
{
    return USER_PREF_GREEN_COLOR_5700K;
}

WORD GetBColor5700KValue(void)
{
    return USER_PREF_BLUE_COLOR_5700K;
}

Bool AdjustRedColor5700K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_RED_COLOR_5700K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_RED_COLOR_5700K)
        return FALSE;
    USER_PREF_RED_COLOR_5700K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_5700K, USER_PREF_GREEN_COLOR_5700K, USER_PREF_BLUE_COLOR_5700K);
    return TRUE;
}

Bool AdjustGreenColor5700K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_GREEN_COLOR_5700K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_GREEN_COLOR_5700K)
        return FALSE;
    USER_PREF_GREEN_COLOR_5700K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_5700K, USER_PREF_GREEN_COLOR_5700K, USER_PREF_BLUE_COLOR_5700K);
    return TRUE;
}

Bool AdjustBlueColor5700K(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_BLUE_COLOR_5700K, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_BLUE_COLOR_5700K)
        return FALSE;
    USER_PREF_BLUE_COLOR_5700K = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_5700K, USER_PREF_GREEN_COLOR_5700K, USER_PREF_BLUE_COLOR_5700K);
    return TRUE;
}
#endif

#if ENABLE_SRGB_FACTORY_ITEM//ENABLE_SRGB_ITEM

WORD GetRColorsRGBValue(void)
{
    return USER_PREF_RED_COLOR_SRGB;
}

WORD GetGColorsRGBValue(void)
{
    return USER_PREF_GREEN_COLOR_SRGB;
}

WORD GetBColorsRGBValue(void)
{
    return USER_PREF_BLUE_COLOR_SRGB;
}
Bool AdjustRedColorsRGB(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_RED_COLOR_SRGB, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_RED_COLOR_SRGB)
        return FALSE;
    USER_PREF_RED_COLOR_SRGB = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,CURRENT_CONTRAST, USER_PREF_RED_COLOR_SRGB, USER_PREF_GREEN_COLOR_SRGB, USER_PREF_BLUE_COLOR_SRGB);
    return TRUE;
}

Bool AdjustGreenColorsRGB(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_GREEN_COLOR_SRGB, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_GREEN_COLOR_SRGB)
        return FALSE;
    USER_PREF_GREEN_COLOR_SRGB = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,CURRENT_CONTRAST, USER_PREF_RED_COLOR_SRGB, USER_PREF_GREEN_COLOR_SRGB, USER_PREF_BLUE_COLOR_SRGB);
    return TRUE;
}

Bool AdjustBlueColorsRGB(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_BLUE_COLOR_SRGB, 0, MAX_COLOR_VALUE, 1);
    if (u16TempValue == USER_PREF_BLUE_COLOR_SRGB)
        return FALSE;
    USER_PREF_BLUE_COLOR_SRGB = u16TempValue;
    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,CURRENT_CONTRAST, USER_PREF_RED_COLOR_SRGB, USER_PREF_GREEN_COLOR_SRGB, USER_PREF_BLUE_COLOR_SRGB);
    return TRUE;
}

Bool SetFactoryColorSRGB(void)
{
    CURRENT_COLOR_TEMP=CT_SRGB;
    Menu_AdjustColorSetting();
    return TRUE;
}

#endif

Bool EnableBurninMode(void)
{
    g_u8PatternNo = 0;
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
    {
        BYTE u8win;
        for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
        {
            msAPIWinEnableFrameColor(u8win, TRUE);
        }
    }
    else
#endif
    {
        msAPIWinEnableFrameColor(DISPLAY_WIN1, TRUE);
    }
    return TRUE;
}

Bool AdjustBurnin(MENU_ITEM_ACTION enAction)
{
    enAction = enAction;
    if (BURNIN_MODE_FLAG)
        CLR_BURNIN_MODE_FLAG();
    else
        SET_BURNIN_MODE_FLAG();
    Delay1ms(180);
    SaveMonitorSetting();
    return TRUE;
}


#if ENABLE_VGA_INPUT
Bool AutoColor(void)
{
    Bool    result;

    if((CURRENT_INPUT_IS_VGA())
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU		
	&&(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)
#endif
	)
	;
    else	
    {
       return FALSE;
    }


    result = mStar_AutoColor(INPUT_VGA);
    if (result)
        SaveMonitorSetting();

    if (result)
        SaveFactorySetting();

    return result;
}
#endif

#if 0
Bool AdjustAdcRedOffset(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_ADC_RED_OFFSET, 0, 255, 1);
    if (u16TempValue == USER_PREF_ADC_RED_OFFSET)
        return FALSE;
    USER_PREF_ADC_RED_OFFSET = u16TempValue;
    drvADC_SetRedOffsetCode(USER_PREF_ADC_RED_OFFSET);

    return TRUE;
}

WORD GetAdcRedOffsetValue(void)
{
    return USER_PREF_ADC_RED_OFFSET;
}

Bool AdjustAdcGreenOffset(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_ADC_GREEN_OFFSET, 0, 255, 1);
    if (u16TempValue == USER_PREF_ADC_GREEN_OFFSET)
        return FALSE;
    USER_PREF_ADC_GREEN_OFFSET = u16TempValue;
    drvADC_SetGreenOffsetCode(USER_PREF_ADC_GREEN_OFFSET);

    return TRUE;
}

WORD GetAdcGreenOffsetValue(void)
{
    return USER_PREF_ADC_GREEN_OFFSET;
}

Bool AdjustAdcBlueOffset(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_ADC_BLUE_OFFSET, 0, 255, 1);
    if (u16TempValue == USER_PREF_ADC_BLUE_OFFSET)
        return FALSE;
    USER_PREF_ADC_BLUE_OFFSET = u16TempValue;
    drvADC_SetBlueOffsetCode(USER_PREF_ADC_BLUE_OFFSET);

    return TRUE;
}

WORD GetAdcBlueOffsetValue(void)
{
    return USER_PREF_ADC_BLUE_OFFSET;
}
#endif

#if ENABLE_VGA_INPUT
Bool AdjustRedGain(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_ADC_RED_GAIN, 0, MAX_ADC_GAIN_VALUE, 1);
    if (u16TempValue == USER_PREF_ADC_RED_GAIN)
        return FALSE;
    USER_PREF_ADC_RED_GAIN = u16TempValue;
    drvADC_SetRedGainCode(USER_PREF_ADC_RED_GAIN);

    return TRUE;
}

WORD GetRedGainValue(void)
{
    return USER_PREF_ADC_RED_GAIN;
}

Bool AdjustGreenGain(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_ADC_GREEN_GAIN, 0, MAX_ADC_GAIN_VALUE, 1);
    if (u16TempValue == USER_PREF_ADC_GREEN_GAIN)
        return FALSE;
    USER_PREF_ADC_GREEN_GAIN = u16TempValue;
    drvADC_SetGreenGainCode(USER_PREF_ADC_GREEN_GAIN);

    return TRUE;
}

WORD GetGreenGainValue(void)
{
    return USER_PREF_ADC_GREEN_GAIN;
}

Bool AdjustBlueGain(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_ADC_BLUE_GAIN, 0, MAX_ADC_GAIN_VALUE, 1);
    if (u16TempValue == USER_PREF_ADC_BLUE_GAIN)
        return FALSE;
    USER_PREF_ADC_BLUE_GAIN = u16TempValue;
    drvADC_SetBlueGainCode(USER_PREF_ADC_BLUE_GAIN);

    return TRUE;
}

WORD GetBlueGainValue(void)
{
    return USER_PREF_ADC_BLUE_GAIN;
}
#endif

WORD GetFSubBrightnessValue(void)  //JL
{
   return USER_PREF_DEF_SUBBRIGHTNESS;
}

//=================for OSD 75===================================
////////////////////////////////////Ailly/////////////////////////////////////////////
//Course of DecIncValue function 運算後的四捨五入無法進位,
//so factory blue menu brightness value is always less 1.
//u16TempValue+1 can balance brightness value less 1,
//but factory TE already had added 1 by themself,
//so do not need to fix this issue. Ailly 20080409
////////////////////////////////////Ailly/////////////////////////////////////////////
//============================================================
Bool AdjustFSubBrightness(MENU_ITEM_ACTION enAction)  //JL
{

   WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_DEF_SUBBRIGHTNESS, FACTORY_DEF_BRIGHTNESS_MIN, FACTORY_DEF_BRIGHTNESS_MAX, 1);
    #if 0  //20070726 JL
    if (u16TempValue == USER_PREF_DEF_SUBBRIGHTNESS)
        return FALSE;
    #endif
    USER_PREF_DEF_SUBBRIGHTNESS = u16TempValue; //u16TempValue+1 can balance brightness value less 1, detail to see up notes, Ailly

    //mStar_WriteByte(PWM1C, (USER_PREF_DEF_SUBBRIGHTNESS));
#ifdef BACKLIGHT_PWM_REVERSE  //Steven110315
    msAPIPWMDuty(BACKLIGHT_PWM,0xFF-USER_PREF_DEF_SUBBRIGHTNESS);
#else
    msAPIPWMDuty(BACKLIGHT_PWM,USER_PREF_DEF_SUBBRIGHTNESS);
#endif
    //    g_u8DDCCIAControl = 0x10;

    return TRUE;

}

Bool AdjustFacEDIDWP(MENU_ITEM_ACTION enAction) ////JL
{
    enAction = enAction;

    if (DDC_WP_FLAG)
    {
      CLR_DDC_WP_FLAG();
      hw_ClrDDC_WP();
    }
    else
    {
      SET_DDC_WP_FLAG();
      hw_SetDDC_WP();
    }
    Delay1ms(500);
    return TRUE;
}

#if 1//ENABLE_ENERGY_STAR_LOGO
Bool AdjustLogoDisplay(MENU_ITEM_ACTION enAction)
{
    enAction = enAction;
    if (OSD_LOGO_FLAG)
        CLR_OSD_LOGO_FLAG();
    else
        SET_OSD_LOGO_FLAG();
    Delay1ms(180);
    SaveMonitorSetting();
    return TRUE;
}
#endif
#if 0
Bool AdjustBankNo(MENU_ITEM_ACTION enAction)
{
    if (enAction == MIA_INC_VALUE)
        g_u8MSBankNo = (g_u8MSBankNo + 1) % 3;
    else
        g_u8MSBankNo = (g_u8MSBankNo + 2) % 3;

    return TRUE;
}

Bool AdjustAddr(MENU_ITEM_ACTION enAction)
{
    if (enAction == MIA_INC_VALUE)
        g_u8MSAddr = (g_u8MSAddr + 1) % 0x100;
    else
        g_u8MSAddr = (g_u8MSAddr + 255) % 0x100;

    return TRUE;
}

WORD GetAddrValue(void)
{
    return g_u8MSAddr;
}

Bool AdjustRegValue(MENU_ITEM_ACTION enAction)
{
    // PRINT_DATA("adjustA %d",g_u8MSValue);
    if (enAction == MIA_INC_VALUE)
        g_u8MSValue = (WORD) (g_u8MSValue + 1) % 0x100;
    else
        g_u8MSValue = (WORD) (g_u8MSValue + 255) % 0x100;

    return TRUE;
}

WORD GetAddrRegValue(void)
{
    if (g_u8MSBankNo == 0)      // scaler
    {
        scWriteByte(SC0_00, 0x00);
        g_u8MSValue = scReadByte(g_u8MSAddr);
    }
    else if (g_u8MSBankNo == 1) // adc
    {
        scWriteByte(SC0_00, 0x01);
        g_u8MSValue = scReadByte(g_u8MSAddr);
        scWriteByte(SC0_00, 0x00);
    }
    else if (g_u8MSBankNo == 2) // TCON
    {
        scWriteByte(SC0_00, 0x02);
        g_u8MSValue = scReadByte(g_u8MSAddr);
        scWriteByte(SC0_00, 0x00);
    }

    return g_u8MSValue;
}

#if 1
WORD GetRegValue(void)
{
    return g_u8MSValue;
}
#endif
Bool WriteRegValue(void)
{
    if (g_u8MSBankNo == 0)      // scaler
    {
        scWriteByte(SC0_00, 0x00);
        scWriteByte(g_u8MSAddr, g_u8MSValue);
    }
    else if (g_u8MSBankNo == 1) // adc
    {
        scWriteByte(SC0_00, 0x01);
        scWriteByte(g_u8MSAddr, g_u8MSValue);
        scWriteByte(0x00, 0x00);
    }
    else if (g_u8MSBankNo == 2) // TCON
    {
        scWriteByte(SC0_00, 0x02);
        scWriteByte(g_u8MSAddr, g_u8MSValue);
        scWriteByte(SC0_00, 0x00);
    }

    return TRUE;
}
#endif

//========================================================================================
//SSC
#if ENABLE_FACTORY_SSCADJ 
WORD GetSSCFrequencyValue(void)
{
	return USER_PREF_SSC_FREQUENCY;
}


WORD GetSSCPermillageValue(void)
{
	return USER_PREF_SSC_PERMILLAGE_ADJUST;
}

Bool AdjustSSCFrequency(MENU_ITEM_ACTION enAction)
{
	WORD u16TempValue;
	u16TempValue = DecIncValue( enAction, USER_PREF_SSC_FREQUENCY, 0, PANEL_SSC_MODULATION_MAX, 1 );
	if( u16TempValue == USER_PREF_SSC_FREQUENCY )
           return FALSE;
	USER_PREF_SSC_FREQUENCY = u16TempValue;
	mStar_SetPanelSSCFactor(USER_PREF_SSC_FREQUENCY,USER_PREF_SSC_PERMILLAGE_ADJUST);
	return TRUE;
}

Bool AdjustSSCPermillage(MENU_ITEM_ACTION enAction)
{
	WORD u16TempValue;
	u16TempValue = DecIncValue( enAction, USER_PREF_SSC_PERMILLAGE_ADJUST, 0, PANEL_SSC_PERCENTAGE_MAX, 1 );
	if( u16TempValue == USER_PREF_SSC_PERMILLAGE_ADJUST )
	    return FALSE;
	USER_PREF_SSC_PERMILLAGE_ADJUST = u16TempValue;
	mStar_SetPanelSSCFactor(USER_PREF_SSC_FREQUENCY,USER_PREF_SSC_PERMILLAGE_ADJUST);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
WORD GetDDRSSCFrequencyValue(void)
{
	return USER_PREF_DDR_SSC_FREQ_MODULATION;
}


WORD GetDDRSSCPermillageValue(void)
{
	return USER_PREF_DDR_SSC_PERMILLAGE_ADJUST;
}

Bool AdjustDDRSSCFrequency(MENU_ITEM_ACTION enAction)
{
	WORD u16TempValue;
	u16TempValue = DecIncValue( enAction, USER_PREF_DDR_SSC_FREQ_MODULATION, 0, DDR_SSC_MODULATION_MAX, 1 );
	if( u16TempValue == USER_PREF_DDR_SSC_FREQ_MODULATION )
           return FALSE;
	USER_PREF_DDR_SSC_FREQ_MODULATION = u16TempValue;
	mStar_SetDDRSSCFactor(USER_PREF_DDR_SSC_FREQ_MODULATION,USER_PREF_DDR_SSC_PERMILLAGE_ADJUST);
	return TRUE;
}

Bool AdjustDDRSSCPermillage(MENU_ITEM_ACTION enAction)
{
	WORD u16TempValue;
	u16TempValue = DecIncValue( enAction, USER_PREF_DDR_SSC_PERMILLAGE_ADJUST, 0, DDR_SSC_PERCENTAGE_MAX, 1 );
	if( u16TempValue == USER_PREF_DDR_SSC_PERMILLAGE_ADJUST )
	    return FALSE;
	USER_PREF_DDR_SSC_PERMILLAGE_ADJUST = u16TempValue;
	mStar_SetDDRSSCFactor(USER_PREF_DDR_SSC_FREQ_MODULATION,USER_PREF_DDR_SSC_PERMILLAGE_ADJUST);
	return TRUE;
}

#endif



Bool SetFactoryColorTemp9300K(void)
{
printMsg("SetFactoryColorTemp9300K");
    //  USER_PREF_COLOR_TEMP=CTEMP_9300K;
    //  Read9300KSetting();
    // msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS_9300K);
    //  msAPI_AdjustContrast(USER_PREF_WIN_SEL , USER_PREF_CONTRAST_9300K);
    USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL) =PCT_9300K;
    Menu_AdjustColorSetting();

    return TRUE;
}
//----------JL
Bool SetFactoryColorTemp5700K(void)
{
printMsg("SetFactoryColorTemp5700K");
    //  USER_PREF_COLOR_TEMP=CTEMP_5700K;
    //  Read9300KSetting();
    // msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS_5700K);
    //  msAPI_AdjustContrast(USER_PREF_WIN_SEL, USER_PREF_CONTRAST_5700K);
    USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL) =PCT_5700K;
    Menu_AdjustColorSetting();

    return TRUE;
}

Bool SetFactoryColorTemp6500K(void)
{
printMsg("SetFactoryColorTemp6500K");
    //  USER_PREF_COLOR_TEMP=CTEMP_6500K;
    //  Read6500KSetting();
    // msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS_6500K);
    //  msAPI_AdjustContrast(USER_PREF_WIN_SEL, USER_PREF_CONTRAST_6500K);
    USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL) =PCT_6500K;
    Menu_AdjustColorSetting();

    return TRUE;
}


