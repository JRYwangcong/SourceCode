///////////////////////////////////////////////////////////////////////////////
/// @file menufunc.h
/// @brief Head file of Menufunc.c
/// @author MStarSemi Inc.
///
/// Functions response to OSD action.
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#define _MENUFUNC_C

#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "Keypaddef.h"
#include "menudef.h"
#include "ms_reg.h"
#include "MDebug.h"
#include "msOSD.h"
#include "drvOSD.h"
#include "Power.h"
#include "misc.h"
#include "AutoFunc.h"
#include "NVRam.h"
//#include "userpref.h"
#include "mStar.h"
#include "Menu.h"
#include "Ms_rwreg.h"
#include "Mcu.h"
#include "MenuFont.h"
//#include "UserPref.h"
//#include "UserPrefAPI.h"
#include "MenuPropStr.h"
#include "msflash.h"
#include "msACE.h"
#include "MsDLC.h"
//#include "msGmaTbl.h"
#include "Panel.h"
#include "menufunc.h"
#include "Panel.h"
#include "PropFontExtFunc.h"
#include "ColorPalette.h"
//#include "ColorFinetune.h"
#include "ColorFinetune_Default.h"
#include "DDC2Bi.H"
#if ENABLE_DAC
#include "drvAudio.h"
#endif
#if ENABLE_RTE
#include "msOVD.h"
#include "drvmsOVD.h"
#endif
#include "Ms_PM.h"
#include "drvADC.h"
#include "msScaler.h"


#include "Adjust.h"
#include "Common.h"
#if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif
#if ENABLE_SUPER_RESOLUTION
#include "MsSR.h"
#endif
#if ENABLE_DPS
#include "msDPS_Setting.h"
#endif
#if ENABLE_3DLUT
#include "drv3DLUT.h"
#endif
#include "appWindow.h"
//#include "msScaler.h"
#include "msScalerAP.h"
#include "appSystem.h"
#include "GPIO_DEF.h"
#include "FactoryMenu.h"

#if Enable_Gamma
#include "Panel_GAMMATBL.h"
#endif
#include "MsDLC_Setting.h"
//#include "drvMcu.h"
//#include "drvmStar.h"
//#include "halRwreg.h"
/////////////////////////////////////////////////////////////////
#if ZUI
#include "apiGOP.h"
#include "MApp_ZUI_APIgdi.h"
#endif
#include "msPQ.h"
#include "menufunc.h"

#include "appInput.h"
#include "mapi_combo.h"

#if ENABLE_DAISY_CHAIN
#include "msDaisyChain.h"
#endif

#include "appSystem.h"
#include "Ms_PM.h"
#include "mhal_DPRx.h"
#include "mapi_tmds.h"
#include "drvmStar.h"

#define MENUFUNC_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&MENUFUNC_DEBUG
    #define MENUFUNC_printData(str, value)   printData(str, value)
    #define MENUFUNC_printMsg(str)           printMsg(str)
#else
    #define MENUFUNC_printData(str, value)
    #define MENUFUNC_printMsg(str)
#endif

extern BYTE xdata   g_u8MenuTempValue;
extern BYTE xdata   g_u8MenuPageIndex;

extern BYTE g_u8PrevMenuItemIndex;

extern WORD OrigAdjPipHpos;
extern WORD OrigAdjPipVpos;

void SaveMonitorSetting(void);
void SaveModeSetting(void);
Bool Menu_AdjustColorSetting(void);
void SaveUserPref(void);
void SaveFactorySetting(void);
void mStar_SetRTE(void);
extern void Init_FactorySetting(void);
BYTE Get_SwitchSource(BYTE InputType);
void msAPI_combo_LoadEDID_Func(void);
void mhal_DP_HPDControl(BYTE ucPortSelect, Bool bSetHPD);
Bool SetColorPresetMode(void);
void EDID_ReLoad_Func(BYTE Reload);
BYTE GetPCT_Tempareture(PresetColorTempType PCT);
void GetColorTempRGB(ColorType *pstColor,BYTE u8ColorTemp);
void msAPI_combo_IPControlHPD(EN_COMBO_IP_SELECT ucComboSelect, Bool bSetHPD);
void ReLoadHDMI_EDID(void);
void ReLoadDVI_EDID(void);
void EarphoneHandler(void);
BYTE GET_Source_Index(void);
void Osd_DrawCrossFont(void);


void ReLoadHDMI_EDID_3840x2160(void);
void ReLoadHDMI14_EDID_3840x2160(void);
void Mstar_InitDDC(void);

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
WORD GetPIPHPositionValue(void);
WORD GetPIPVPositionValue(void);
#endif
#define IC_PEAKING_DEF  0x08

#define DEF_SUBWIN_BORDER_COLOR      0x00
#define DEF_SUBWIN_BORDER_WIDTH     0x02

//For save Scaler Mask
WORD g_u16SavedScalerMask=0;
//IHC//////////////////
BYTE GetRealIhcValue(BYTE u8HueValue)
{
    //  OSD value: 0 ~ 100
    //  Value mapping:
    //      0: 0x3F
    //      50: 0x00
    //      100: -0x3F
    BYTE u8RegVaule = 0x00;

    if(u8HueValue>100)
    {
        return 0x7F;
    }
    if(u8HueValue > 50)
    {
        u8RegVaule = (0x3F*(u8HueValue-50))/50;
        u8RegVaule |= 0x40;
    }
    else if(u8HueValue == 50)
    {
        u8RegVaule = 0x00;
    }
    else // u8RegVaule < 50
    {
        u8RegVaule = (0x3F*(50-u8HueValue))/50;
    }

    MENUFUNC_printData("\n u8RegVaule= 0x%x",u8RegVaule);

    return u8RegVaule;
}


BYTE GetRealICCValue(BYTE u8SaturationValue)
{
    //  OSD value: 0 ~ 100
    //  Value mapping:
    //      0: -0x0F
    //      50: 0x00
    //      100: 0x0F
    BYTE u8RegVaule = 0x00;

	
#if 1
    if(u8SaturationValue>100)
    {
        return 0x1F;
    }
    if(u8SaturationValue > 50)
    {
        u8RegVaule = (0x1F*(u8SaturationValue-50))/50;

    }
    else if(u8SaturationValue == 50)
    {
        u8RegVaule = 0x00;
    }
    else // u8RegVaule < 50
    {
        u8RegVaule = (0x3F*(50-u8SaturationValue))/50;
        u8RegVaule |= 0x40;
    }

#else
    BYTE u8MaxSaturation = 0x00;
#if (CHIP_ID == MST9U3)
    u8MaxSaturation = 0x3F;
#else
    u8MaxSaturation = 0x0F; 
#endif
    if(u8SaturationValue>100)
    {
        return u8MaxSaturation;
    }
    if(u8SaturationValue > 50)
    {
        u8RegVaule = (u8MaxSaturation*(u8SaturationValue-50))/50;

    }
    else if(u8SaturationValue == 50)
    {
        u8RegVaule = 0x00;
    }
    else // u8RegVaule < 50
    {
        u8RegVaule = (u8MaxSaturation*(50-u8SaturationValue))/50;
        u8RegVaule |= 0x80;

    }


#endif

    MENUFUNC_printData("\n RealICCValue = 0x%x",u8RegVaule);

    return u8RegVaule;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WORD DecIncValue(MENU_ITEM_ACTION enAction, WORD u16Value, WORD u16MinValue, WORD u16MaxValue, BYTE u8Step)
{
    BOOLEAN bInc = 0;
    WORD u16TempValue = 0;

#if ZUI
    bInc = enAction; // TRUE = Increase Value, FALSE = Decrease Value
#else
    if (enAction == MIA_INC_VALUE || enAction == MIA_INV_DEC_VALUE)
    {
        bInc = TRUE;
    }
    else if (enAction == MIA_DEC_VALUE || enAction == MIA_INV_INC_VALUE)
    {
        bInc = FALSE;
    }
#endif

    if (bInc) // Increase Value
    {
        if(u16Value >= u16MaxValue)
            return u16MaxValue;
        else
            u16TempValue = u16Value + u8Step;

        if(u16TempValue > u16MaxValue)
            u16TempValue = u16MaxValue;
    }
    else // Decrease Value
    {
        if(u16Value <= u16MinValue || u16Value < u8Step)
            return u16MinValue;
        else
            u16TempValue = u16Value - u8Step;

        if(u16TempValue < u16MinValue)
            u16TempValue = u16MinValue;
    }

    return u16TempValue;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WORD DecIncOSDTimerValue(MENU_ITEM_ACTION enAction, WORD u16Value, WORD u16MinValue, WORD u16MaxValue, BYTE u8Step)
{
    BOOLEAN bInc = 0;
    WORD u16TempValue = 0;

#if ZUI
    bInc = enAction; // TRUE = Increase Value, FALSE = Decrease Value
#else
    if (enAction == MIA_INC_VALUE || enAction == MIA_INV_DEC_VALUE)
    {
        bInc = TRUE;
    }
    else if (enAction == MIA_DEC_VALUE || enAction == MIA_INV_INC_VALUE)
    {
        bInc = FALSE;
    }
#endif

    if (bInc) // Increase Value
    {
       if(u16Value >= u16MaxValue)
            return u16MaxValue;
	else if(u16Value<u8Step)	//10<15
            return u8Step;
       else
            u16TempValue = u16Value + u8Step;

        if(u16TempValue > u16MaxValue)
            u16TempValue = u16MaxValue;
    }
    else // Decrease Value
    {
       if(u16Value <= u16MinValue || u16Value < u8Step)
            return u16MinValue;
	else if(u16Value <= u8Step)
            return u16MinValue;
       else
            u16TempValue = u16Value - u8Step;

        if(u16TempValue < u16MinValue)
            u16TempValue = u16MinValue;
    }

    return u16TempValue;
}


WORD DecIncValueCirculate(MENU_ITEM_ACTION enAction, WORD u16Value, WORD u16MinValue, WORD u16MaxValue, BYTE u8Step)
{
    BOOLEAN bInc = 0;
    WORD u16TempValue = 0;

#if ZUI
    bInc = enAction; // TRUE = Increase Value, FALSE = Decrease Value
#else
    if (enAction == MIA_INC_VALUE)
    {
        bInc = TRUE;
    }
    else if (enAction == MIA_DEC_VALUE)
    {
        bInc = FALSE;
    }
#endif

    if (bInc) // Increase Value
    {
        if (u16Value >= u16MaxValue || u16Value < u16MinValue)
        {
            return u16MinValue;
        }
        else
        {
            u16TempValue = u16Value + u8Step;
        }

        if (u16TempValue > u16MaxValue)
        {
            u16TempValue = u16MinValue;
        }
    }
    else // Decrease Value
    {
        if (u16Value <= u16MinValue || u16Value < u8Step)
        {
            return u16MaxValue;
        }
        else
        {
            u16TempValue = u16Value - u8Step;
        }

        if (u16TempValue < u16MinValue)
        {
            u16TempValue = u16MaxValue;
        }
    }

    return u16TempValue;
}




WORD GetScale100Value( WORD value, WORD minValue, WORD maxValue )
{
    maxValue = ( maxValue <= minValue ) ? minValue : ( maxValue - minValue );
    //maxValue = maxValue - minValue;
    value = value - minValue;
    if( value > 650 )
    {
        value /= 2;
        maxValue /= 2;
    }
    value = (( WORD )value * 100 + maxValue / 2 ) / maxValue;
    value %= 101;
    return value;
}

#if 0
WORD GetScale100ValueEx(WORD u16Value, WORD u16MinValue, WORD u16MaxValue,WORD u16DefValue,BYTE u8DefPercent)
{
    if (u16Value>u16DefValue)
    {
        u16DefValue = u16MaxValue - u16DefValue;
        u16MaxValue = u16MaxValue - u16Value;
        u16Value = 100 - ((DWORD)u16MaxValue * (100 - u8DefPercent) + (u16DefValue >> 1)) / u16DefValue;
    }
    else
    {
        u16DefValue = u16DefValue - u16MinValue;
        u16MinValue = u16Value - u16MinValue;
        u16Value = ((DWORD)u16MinValue * u8DefPercent + (u16DefValue >> 1)) / u16DefValue;
    }

    return u16Value;
}
#endif

void GetColorTempRGB(ColorType *pstColor,BYTE u8ColorTemp)
{

    switch(u8ColorTemp)
    {
        case eCT_SRGB:
            pstColor->u8Red   = USER_PREF_RED_COLOR_SRGB;//GetScale100Value(USER_PREF_RED_COLOR_SRGB, 0, 0xFF);
            pstColor->u8Green= USER_PREF_GREEN_COLOR_SRGB;//GetScale100Value(USER_PREF_GREEN_COLOR_SRGB, 0, 0xFF);
            pstColor->u8Blue=  USER_PREF_BLUE_COLOR_SRGB;//GetScale100Value(USER_PREF_BLUE_COLOR_SRGB, 0, 0xFF);

            break;

        case eCT_9300K:
            pstColor->u8Red   = USER_PREF_RED_COLOR_9300K;//GetScale100Value(USER_PREF_RED_COLOR_9300K, 0, 0xFF);
            pstColor->u8Green= USER_PREF_GREEN_COLOR_9300K;//GetScale100Value(USER_PREF_GREEN_COLOR_9300K, 0, 0xFF);
            pstColor->u8Blue=  USER_PREF_BLUE_COLOR_9300K;//GetScale100Value(USER_PREF_BLUE_COLOR_9300K, 0, 0xFF);
            break;

        case eCT_6500K:
            pstColor->u8Red   = USER_PREF_RED_COLOR_6500K;//GetScale100Value(USER_PREF_RED_COLOR_6500K, 0, 0xFF);
            pstColor->u8Green= USER_PREF_GREEN_COLOR_6500K;//GetScale100Value(USER_PREF_GREEN_COLOR_6500K, 0, 0xFF);
            pstColor->u8Blue=  USER_PREF_BLUE_COLOR_6500K;//GetScale100Value(USER_PREF_BLUE_COLOR_6500K, 0, 0xFF);
            break;

        case eCT_5700K:
            pstColor->u8Red   = USER_PREF_RED_COLOR_5700K;//GetScale100Value(USER_PREF_RED_COLOR_5700K, 0, 0xFF);
            pstColor->u8Green= USER_PREF_GREEN_COLOR_5700K;//GetScale100Value(USER_PREF_GREEN_COLOR_5700K, 0, 0xFF);
            pstColor->u8Blue=  USER_PREF_BLUE_COLOR_5700K;//GetScale100Value(USER_PREF_BLUE_COLOR_5700K, 0, 0xFF);
            break;

        case eCT_10000K:
            pstColor->u8Red   = USER_PREF_RED_COLOR_10000K;//GetScale100Value(USER_PREF_RED_COLOR_10000K, 0, 0xFF);
            pstColor->u8Green= USER_PREF_GREEN_COLOR_10000K;//GetScale100Value(USER_PREF_GREEN_COLOR_10000K, 0, 0xFF);
            pstColor->u8Blue=  USER_PREF_BLUE_COLOR_10000K;//GetScale100Value(USER_PREF_BLUE_COLOR_10000K, 0, 0xFF);
            break;

        case eCT_7500K:
            pstColor->u8Red   = USER_PREF_RED_COLOR_7500K;//GetScale100Value(USER_PREF_RED_COLOR_7500K, 0, 0xFF);
            pstColor->u8Green= USER_PREF_GREEN_COLOR_7500K;//GetScale100Value(USER_PREF_GREEN_COLOR_7500K, 0, 0xFF);
            pstColor->u8Blue=  USER_PREF_BLUE_COLOR_7500K;//GetScale100Value(USER_PREF_BLUE_COLOR_7500K, 0, 0xFF);
            break;

        case eCT_5000K:
            pstColor->u8Red   = USER_PREF_RED_COLOR_5000K;//GetScale100Value(USER_PREF_RED_COLOR_5000K, 0, 0xFF);
            pstColor->u8Green= USER_PREF_GREEN_COLOR_5000K;//GetScale100Value(USER_PREF_GREEN_COLOR_5000K, 0, 0xFF);
            pstColor->u8Blue=  USER_PREF_BLUE_COLOR_5000K;//GetScale100Value(USER_PREF_BLUE_COLOR_5000K, 0, 0xFF);
            break;

        default:
            pstColor->u8Red     =CURRENT_RED_COLOR;
            pstColor->u8Green   =CURRENT_GREEN_COLOR;
            pstColor->u8Blue    =CURRENT_BLUE_COLOR;
            break;
    }
}

void Menu_BurnInModeCheck(void)
{
    if (BURNIN_MODE_FLAG)
        Set_DoBurninModeFlag();
    else
        Clr_DoBurninModeFlag();
}

Bool SetGotoFactoryMenu(void)
{
    g_stMenuFlag.bShowFactoryMenu=1;
    //g_u8MenuPageIndex=FACTORY_MENU;
    printMsg("SetGotoFactoryMenu");
    Menu_Hide();
    return TRUE;
}


 
 void Menu_InitAction(void)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
 BYTE k;
#endif
 
    drvOSD_Init();

#if LOGO_ENABLE
    if (g_stMenuFlag.bDisplayLogo)
    {
        if (g_u8MenuPageIndex == MENU_LOGO)
            return;
        g_u8MenuPageIndex = MENU_LOGO;
    }
    else 
#endif		
   if(g_stMenuFlag.bShowInputIcon)
   {
        if (g_u8MenuPageIndex == MENU_HOTINPUT)
            return;
        g_u8MenuPageIndex = MENU_HOTINPUT;
   }
   else
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
   if (((Is_MultiWin_SyncLoss())&&USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)||((SyncLossState(USER_PREF_INPUT_TYPE)) && (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)))
#else
   if (SyncLossState(USER_PREF_INPUT_TYPE))
#endif
   {
            Menu_BurnInModeCheck();
            if (DoBurninModeFlag)
            	{
                g_u8MenuPageIndex = BURNIN_MENU;   
             #if LED_HOT_Menu_Func		
             	 {
                    if(!IS_LED_Power())
                    {
             	          Set_LED_Power();
                     if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)
                        ForceDelay1ms(53);//(49);
                     else//LedModeIndex_User
                     {
              	if((USER_PREF_LedModeType==LedBreath)	)  
                        ForceDelay1ms(53);
                     else    
                        ForceDelay1ms(55);//55//53
                     }
                    }
             
             		 if(FACTORY_MODE_FLAG||BURNIN_MODE_FLAG)	
             		 {
             	          SetLedFunc(LED_Number, LedColorful, LedMax, LedBreath);
			   Clr_LEDActiveOnFlag();				  
             		 }
                 }
             #endif

		}
            else
            {
            #if AIO_Board
                #if SwitchPortByDP_Detect
                   if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)
                  g_u8MenuPageIndex = MENU_STANDBY;
                #endif
	     #else		
               //if (CableNotConnectedFlag(USER_PREF_INPUT_TYPE))
               //   g_u8MenuPageIndex = MENU_CABLE_NOT_CONNECTED;
              //else
                  g_u8MenuPageIndex = MENU_STANDBY;
	     #endif		  
            }
    }
    else if (UnsupportedModeFlag(USER_PREF_INPUT_TYPE)||DISPLAY_UNSUPPORT_MODE_FLAG)
    {
        g_u8MenuPageIndex = MENU_UNSUPPORTED_MODE;
    }
#if 1//ndef KTC_Project
    else if (FACTORY_MODE_FLAG)
    {
        g_u8MenuPageIndex=FACTORY_MENU;
    }
#endif	
    else
    {
    #if ENABLE_VGA_INPUT
        MENUFUNC_printData("USER_PREF_AUTO_TIMES %d", USER_PREF_AUTOTIME(USER_PREF_INPUT_TYPE));
   #endif


      LEDModeCounter=0;

    	{


         Set_HDMI1HPDFlag();   
         Set_HDMI2HPDFlag();   
         Set_HDMI3HPDFlag();   
        printData("MapWin2Port(USER_PREF_WIN_SEL)==%d",MapWin2Port(USER_PREF_WIN_SEL));
#if ((defined(FengYuan_Project))&&(!HDMI_HPD_Enable))		
  #if ENABLE_HDMI_INPUT
        if((USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)&&(USER_PREF_INPUT_PRIORITY==INPUT_PRIORITY_AUTO)&&(MapWin2Port(USER_PREF_WIN_SEL)==INPUT_PRIORITY_HDMI))
             Clr_HDMI1HPDFlag();
  #if ENABLE_HDMI2ND_INPUT
        else if((USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)&&(USER_PREF_INPUT_PRIORITY==INPUT_PRIORITY_AUTO)&&(MapWin2Port(USER_PREF_WIN_SEL)==INPUT_PRIORITY_HDMI2ND))
             Clr_HDMI2HPDFlag();
  #endif		
  #if ENABLE_HDMI3ND_INPUT
        else if((USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)&&(USER_PREF_INPUT_PRIORITY==INPUT_PRIORITY_AUTO)&&(MapWin2Port(USER_PREF_WIN_SEL)==INPUT_PRIORITY_HDMI3ND))
             Clr_HDMI3HPDFlag();
  #endif
  #endif	
#endif  
  
    #if ENABLE_VGA_INPUT
         if (!(USER_PREF_AUTOTIME(USER_PREF_INPUT_TYPE)&AUTO_TIMES_BIT) && Is_All_VGAPort_DisplayOk_In_MultiWin())
            g_u8MenuPageIndex = MENU_HOT_AUTO;
        else
    #endif
        {
                g_u8MenuPageIndex = MENU_ROOT;
	#if ((!AIO_Board)||SwitchPortByDP_Detect)		
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
			if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
			{
				if(g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1])
				{
				    if(g_u8MenuPageIndex == MENU_INPUT)
				    {   
					Menu_Hide();
				    }
				     g_u8MenuPageIndex = MENU_INPUT;
                                 g_u16OsdCounter = 3;
                                 ResetMsCounter();
				}	
				g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1] = 0;
			}
			else
			{
				for(k=0;k<DISPLAY_MAX_NUMS;k++)
				{
					if(g_stMenuFlag.bDisplayInputMsg[k])
					{
 				             if(g_u8MenuPageIndex == MENU_INPUT)
				             {   
				                g_u16OsdCounter = 3;
				                ResetMsCounter();
					         Menu_Hide();
				              }
						g_u8MenuPageIndex = MENU_INPUT;
						break;
					}	
				}
			}	
		#else
				if(g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1])
				{
				    if(g_u8MenuPageIndex == MENU_INPUT)
				    {   
				       g_u16OsdCounter = 3;
				       ResetMsCounter();
					Menu_Hide();
				    }
				     g_u8MenuPageIndex = MENU_INPUT;
				}	
				g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1] = 0;
		#endif
	  #endif	
		
        }
          
          #if FreeSync_Not_Support_Info
           if((FreeSyncMsgFlag))
           {
              g_u8MenuPageIndex=MENU_FREESYNC_INFO;
              g_u8MenuItemIndex = 0;
              Clr_FreeSyncMsgFlag();	
           }
          #endif

	
    	}
	
    }

    if(!SyncLossState(USER_PREF_INPUT_TYPE))
    {
        if(MapWin2Port(USER_PREF_WIN_SEL)!=USER_PREF_INPUT_TYPE)
        {
            USER_PREF_INPUT_TYPE=MapWin2Port(USER_PREF_WIN_SEL);
            //NVRam_WriteByte(GET_WIN_INPUTSOURCE(USER_PREF_WIN_SEL), USER_PREF_INPUT_TYPE);
        }
    }

#if LOGO_ENABLE
    if (g_u8MenuPageIndex != MENU_LOGO)
#endif
        Menu_ClearOsdCounterAndFlag();

    g_u8PowerDownCounter = 0;

if(g_u8MenuPageIndex==MENU_HOTINPUT)
{
  #if Enable_AutoInput_Menu
         g_u8MenuItemIndex= USER_PREF_INPUT_PRIORITY;
   #else
         g_u8MenuItemIndex= GET_Source_Index();
   #endif
}
else if(HotCrossMenuFlag&&(g_u8MenuPageIndex!=MENU_INPUT))
{
    g_u8MenuPageIndex=MENU_HOTCROSS;
    g_u8MenuItemIndex = 0;
    //Osd_DrawCrossFont();	
    //Clr_HotCrossMenuFlag();	
    // return;	 
}
else
    g_u8MenuItemIndex = 0;

    g_u8PrevMenuItemIndex=0;

        MENUFUNC_printData("Menu_InitAction MenuPage %d", g_u8MenuPageIndex);

#if AIO_Board
   if((g_u8MenuPageIndex>=MENU_ROOT)&&(g_u8MenuPageIndex<MAX_MENU))
   {
      CLR_FACTORY_MODE_FLAG();
      CLR_BURNIN_MODE_FLAG();
      SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));
   }
#endif

#if LED_HOT_Menu_Func		  
    if((POWER_ON_FLAG)/*&&(LED_FUNC_ON_FLAG)*/&&(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)&&(!FACTORY_MODE_FLAG)&&(!BURNIN_MODE_FLAG)&&(!LEDActiveOnFlag)&&((g_u8MenuPageIndex>=MENU_ROOT)&&(g_u8MenuPageIndex<MAX_MENU)))
    {

	if(!IS_LED_Power())
	 {
               Set_LED_Power();
    
              if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)
                 ForceDelay1ms(53);//(49);
              else//LedModeIndex_User
              {
       	     if((USER_PREF_LedModeType==LedBreath)	)  
                 ForceDelay1ms(53);
              else    
                 ForceDelay1ms(55);//55//53
              }
	  }
	
        if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)             	
         SetLedFunc(LED_Number, LedColorful, LedMax, LedBreath);
	else	
         SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
         Set_LEDActiveOnFlag();
    }
    else if((USER_PREF_LEDMODE_INDEX==LedModeIndex_Off)&&(!FACTORY_MODE_FLAG)&&(!BURNIN_MODE_FLAG))
    {
       if(IS_LED_Power())
       {
           SetLedFunc(0xFF, 0xFF, 0xFF, 0xFF);
           ForceDelay1ms(60);
           Clr_LED_Power();
       }

    }
#endif	

    if (g_u8MenuPageIndex<0x80)
    {
        ExecuteKeyEvent(MIA_REDRAW_MENU);
    }
    else
        FactoryExecuteKeyEvent(MIA_REDRAW_MENU);

#if ENABLE_VGA_INPUT
    if (UnsupportedModeFlag(USER_PREF_INPUT_TYPE))
    {
        if (!FreeRunModeFlag(MapPort2DetScIndex(USER_PREF_INPUT_TYPE)))
        {
            if (!(USER_PREF_AUTOTIME(USER_PREF_INPUT_TYPE)&AUTO_TIMES_BIT))
                AutoConfig();
        }
    }
#endif

        Menu_SaveInputStatus();

  

}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
WORD Get_MultiWin_Number(void )
{
    BYTE xdata u8IdxWin= 0;
    BYTE xdata UserWinNumber=0;
	
		
     for(u8IdxWin=0 ; u8IdxWin<DISPLAY_MAX_NUMS ; u8IdxWin++)
     {         	  
         if(g_DisplayWindow[u8IdxWin].eDwStatus!=WIN_STATUS_IDLE)
	    UserWinNumber++;

     }
	 
     return UserWinNumber;
}

Bool Is_MultiWin_SyncLoss(void)
{
    BYTE xdata u8IdxWin= 0;
    BYTE xdata LossMask=0;
    BYTE xdata SyncLossMask=0;
	
	
     for(u8IdxWin=0 ; u8IdxWin<Get_MultiWin_Number() ; u8IdxWin++)
     {	    LossMask|=1<<u8IdxWin;
     }
	
   
     for(u8IdxWin=0 ; u8IdxWin<Get_MultiWin_Number() ; u8IdxWin++)
     {	  if(SyncLossState(MapWin2Port(u8IdxWin)))
	     SyncLossMask|=1<<u8IdxWin;
     }
	 
    
	 
     if(SyncLossMask==LossMask)
      return TRUE;
     else
      return FALSE;
}

void Set_MultiWin_InputTimingChangeFlag(void)
{
    BYTE xdata u8IdxWin= 0;
	
    for(u8IdxWin=0 ; u8IdxWin<Get_MultiWin_Number() ; u8IdxWin++)
    {	  
           Set_InputTimingChangeFlag(MapWin2Port(u8IdxWin));
    }

}

//------------------------------------------------------------------------------------------

BYTE GetMulWindowInputPortReadyNums(void)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
  if((USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP)||(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1))
  {
	 return ((ReadyForChangeFlag(USER_PREF_INPUT_TYPE)?1:0)+(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)?1:0));
  }
  else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
  {
	 return ((ReadyForChangeFlag(USER_PREF_INPUT_TYPE)?1:0)+(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)?1:0)+(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)?1:0));
  }
  else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_4)
  {
	 return ((ReadyForChangeFlag(USER_PREF_INPUT_TYPE)?1:0)+(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)?1:0)+(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)?1:0)+(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN3)?1:0));
  }
  else 
#endif  	
	 return (ReadyForChangeFlag(USER_PREF_INPUT_TYPE)?1:0);
  
}

Bool Is_DisplayOk_In_MultiWin(void)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
    BYTE xdata u8IdxWin= 0;
    BYTE xdata WinNums;
#endif
    BYTE xdata OutPut_OK_Mask=0;  

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
   if((USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP)||(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1))
   {
     WinNums=2;
   }
   else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
   {
     WinNums=3;
   }
   else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
   {
     WinNums=4;
   }
   else
   {
     WinNums=1;
   }
   
     for(u8IdxWin=0 ; u8IdxWin<WinNums ; u8IdxWin++)
     {	  
         //printData("g_DisplayWindow[u8IdxWin].eDwStatus==%d",g_DisplayWindow[u8IdxWin].eDwStatus);
         //printData("GetMulWindowInputPortReadyNums()==%d",GetMulWindowInputPortReadyNums());
	  if(g_DisplayWindow[u8IdxWin].eDwStatus==WIN_STATUS_OUTPUT_DISPLAYOK)
	     OutPut_OK_Mask++;
     }
#else

         //printData("g_DisplayWindow[u8IdxWin].eDwStatus==%d",g_DisplayWindow[u8IdxWin].eDwStatus);
         //printData("GetMulWindowInputPortReadyNums()==%d",GetMulWindowInputPortReadyNums());
	  if(g_DisplayWindow[1].eDwStatus==WIN_STATUS_OUTPUT_DISPLAYOK)
	     OutPut_OK_Mask=1;

#endif

     if((OutPut_OK_Mask==GetMulWindowInputPortReadyNums()))
     {
      return TRUE;
     }
    else
      return FALSE;

	
}
//------------------------------------------------------------------------------------------
Bool Is_All_VGAPort_DisplayOk_In_MultiWin(void)
{
    BYTE xdata u8IdxWin= 0;
    BYTE xdata VGAPort_Num=0;
    BYTE xdata DisplayOk_Num=0;
   
	
     for(u8IdxWin=0 ; u8IdxWin<Get_MultiWin_Number() ; u8IdxWin++)
     {	  
	   if(g_InputPort[MapWin2Port(u8IdxWin)].eIPType == TYPE_ANALOG)
	   {  
	      VGAPort_Num++;
             if(g_DisplayWindow[u8IdxWin].eDwStatus==WIN_STATUS_OUTPUT_DISPLAYOK)
	          DisplayOk_Num++;
	   }
     }
	
    if(VGAPort_Num)
    {
        if(VGAPort_Num==DisplayOk_Num)
          return TRUE;
        else
         return FALSE;
    }
    else
      return FALSE;
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void Menu_SaveInputStatus(void)
{
	BYTE ucTemp; // 20140310.  Gary.Wang. Condition for don't write Nvram if port data is the same.
	if (!SyncLossState(USER_PREF_INPUT_TYPE))
	{
		NVRam_ReadByte(EEP_SYSTEM_MEMBER_ADDR(u8SYS_MltWinSRC[DISPLAY_WIN1]), &ucTemp );
		if (USER_PREF_INPUT_TYPE != ucTemp)
		{
			NVRam_WriteByte(EEP_SYSTEM_MEMBER_ADDR(u8SYS_MltWinSRC[DISPLAY_WIN1]), USER_PREF_INPUT_TYPE);
		}
	}

	if (!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1))
	{
		NVRam_ReadByte(EEP_SYSTEM_MEMBER_ADDR(u8SYS_MltWinSRC[DISPLAY_WIN2]), &ucTemp );
		if (USER_PREF_INPUT_TYPE != ucTemp)
		{
			NVRam_WriteByte(EEP_SYSTEM_MEMBER_ADDR(u8SYS_MltWinSRC[DISPLAY_WIN2]), USER_PREF_INPUT_TYPE_SUBWIN1);
		}
	}

	if (!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN2))
	{
		NVRam_ReadByte(EEP_SYSTEM_MEMBER_ADDR(u8SYS_MltWinSRC[DISPLAY_WIN3]), &ucTemp );
		if (USER_PREF_INPUT_TYPE != ucTemp)
		{
			NVRam_WriteByte(EEP_SYSTEM_MEMBER_ADDR(u8SYS_MltWinSRC[DISPLAY_WIN3]), USER_PREF_INPUT_TYPE_SUBWIN2);
		}
	}

	if (!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN3))
	{
		NVRam_ReadByte(EEP_SYSTEM_MEMBER_ADDR(u8SYS_MltWinSRC[DISPLAY_WIN4]), &ucTemp );
		if (USER_PREF_INPUT_TYPE != ucTemp)
		{
			NVRam_WriteByte(EEP_SYSTEM_MEMBER_ADDR(u8SYS_MltWinSRC[DISPLAY_WIN4]), USER_PREF_INPUT_TYPE_SUBWIN3);
		}
	}
}


Bool EnterStandbyOnMenu(void)
{
#if 1//Enter Power Saving Msg

    g_u16OsdCounter=3;
    ResetMsCounter();
    g_stMenuFlag.bOsdTimeout=0;    
    g_u8PowerDownCounter=0;	
    Clr_ForcePowerSavingFlag();

#else

#if AIO_Board
  #if SwitchPortByDP_Detect
   if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)
   {
    g_u8PowerDownCounter=5;
    Clr_ForcePowerSavingFlag();
  }
   else
  #endif
   {
    g_u8PowerDownCounter=0;
    Set_ForcePowerSavingFlag();
  }
#else
#if Internal_Power_Setting
    g_u8PowerDownCounter=15;
#else
    g_u8PowerDownCounter=5;
#endif
#endif

    ResetMsCounter();
    g_stMenuFlag.bOsdTimeout=0;    
	
#endif	
    return TRUE;
}

Bool EnterCheckCableMenu(void)
{
#if 1//Enter Power Saving Msg

    g_u16OsdCounter=4;
    ResetMsCounter();
    g_stMenuFlag.bOsdTimeout=0;    
    g_u8PowerDownCounter=0;	
    Clr_ForcePowerSavingFlag();

#else
#if AIO_Board
  #if SwitchPortByDP_Detect
   if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)
   {
    g_u8PowerDownCounter=5;
    Clr_ForcePowerSavingFlag();
  }
   else
  #endif
   {
    g_u8PowerDownCounter=0;
    Set_ForcePowerSavingFlag();
  }
#else
#if Internal_Power_Setting
    g_u8PowerDownCounter=15;
#else
    g_u8PowerDownCounter=5;
#endif
#endif

    ResetMsCounter();
    g_stMenuFlag.bOsdTimeout=0;    
	
#endif

    return TRUE;
}

#if ENABLE_MULTI_INPUT
Bool EnterInputMenu(void)
{
    //g_stMenuFlag.bOsdSwitchInputPort=0;
    //g_stMenuFlag.bDisplayInputMsg=0;
    return TRUE;
}
#endif

Bool EnterSuspendOnMenu(void)
{
#if AIO_Board
  #if SwitchPortByDP_Detect
   if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)
   {
    g_u8PowerDownCounter=5;
    Clr_ForcePowerSavingFlag();
  }
   else
  #endif
   {
    g_u8PowerDownCounter=0;
    Set_ForcePowerSavingFlag();
  }
#else
#if Internal_Power_Setting
    g_u8PowerDownCounter=15;
#else
    g_u8PowerDownCounter=3;
#endif
#endif
    g_u16OsdCounter=0;
    ResetMsCounter();
    g_stMenuFlag.bOsdTimeout=0;    
    return TRUE;
}

Bool PowerOffSystem(void)
{
    CLR_POWER_ON_FLAG();


#if AudioFunc//ENABLE_5707
   hw_SetMute();
#endif
   hw_Clr_SPIFGPIO_EN_Pin();
#if ENABLE_DAC
#if EnableDACPowerDown
    msAPI_AudioPowerDown();
#endif
#endif


    SaveMonitorSetting();
#if ENABLE_VGA_INPUT
    SaveModeSetting();
#endif
    SaveFactorySetting();

    //Power_TurnOffLed();
    Power_TurnOnAmberLed();

    Power_TurnOffPanel();
	
 #if 0//ENABLE_DP_INPUT||ENABLE_HDMI_INPUT
    SetFreeSyncOnOffFunction(DISPLAY_WIN1,0, 800); // only for single window (win0)
 #endif

 #if LED_HOT_Menu_Func
 if(IS_LED_Power())
   SetLedFunc(0xFF, 0xFF, 0xFF, 0xFF);
   Clr_LED_Power();
 #endif
 

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
    {
        BYTE u8win;
        for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
        {
            msAPIWinEnableFrameColor(u8win, FALSE);
        }
    }
    else
#endif
    {
        msAPIWinEnableFrameColor(DISPLAY_WIN1, FALSE);
    }
    Clr_PowerSavingFlag();

    //DDC2Bi_Init();
    //EX1 = 1;

    Clr_DoBurninModeFlag();
    CLR_BURNIN_MODE_FLAG();
    CLR_FACTORY_MODE_FLAG();
    Clr_OSDFreeSyncActionFlag();
	
    SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));
    MENUFUNC_printData("PowerOffSystem %x", MONITOR_FLAGS);

#if LOGO_ENABLE
    if (g_stMenuFlag.bDisplayLogo)      // 2005/4/4 ¤U¤È 02:34:22 by keng
    {
        g_stMenuFlag.bDisplayLogo=0;
        g_stMenuFlag.bOsdLoadData=1;
    }
    else
#endif		
    if (g_stMenuFlag.bShowDiagPattern)
    {
        g_stMenuFlag.bShowDiagPattern=0; //Jison 090319 SC0_32 already clear, just clr flag
    }
    g_stMenuFlag.bShowInputIcon=0;	
    Clr_DoModeSettingFlag(SrcInputPortM/*//tmp*/);
    g_stMenuFlag.bPressSrcInputKey=0; //Jison 080516

#if ENABLE_USB_INPUT
    hw_USB_HI();
#endif

    msAPI_PMSetMode(ePM_POWER_DCOFF);

    return TRUE;
}

Bool PowerOnSystem(void)
{
    // supposely should power on from 51
    return TRUE;
}

#if LOGO_ENABLE
Bool EnterDisplayLogo(void)
{

    Menu_Hide();//Mike 120215 Fix Logo garbage.
    Power_TurnOnPanel();
    msAPI_AdjustBrightness(80);	
 #if LOGOTimeOutDelayForVBY1PanelLock  //  160729_02
#if PANEL_VBY1()
    #if (PanelType == PanelLSM315HP01)//(MS_PANEL_TYPE==PanelCMI40UltraHD||MS_PANEL_TYPE==PanelTPVLVM400K1QS1AD82P0X ||MS_PANEL_TYPE==PanelTPT315B5HP01)
        #if ( CHIP_ID == MST9U3)
        #define VX1_HPDN_STATUS     (msReadByte(REG_140619)&BIT7)
        #define VX1_LOCKN_STATUS    (msReadByte(REG_140619)&BIT6)
        #else
        #define VX1_HPDN_STATUS     (msReadByte(REG_1407D1)&BIT7)
        #define VX1_LOCKN_STATUS    (msReadByte(REG_1407D1)&BIT6)
        #endif
    	if(VX1_LOCKN_STATUS||VX1_HPDN_STATUS)
    	{
    		g_u8OsdCountOffset=1;//2;
    	}
    	else
    	{
    		g_u8OsdCountOffset=0;
    	}
    	//printf("___)00000000_____EnterDisplayLogo=%d\n", 0);
    	//printf("__u_g_u8OsdCountOffset=%d\n", g_u8OsdCountOffset);
    #endif
#endif
 #endif
	
    Menu_Show();//Mike 120215 Fix Logo garbage.
    printMsg("EnterDisplayLogo");
    return TRUE;
}
#endif
/*
BYTE GetSubWndRealContrast(BYTE ucContrast)
{
    return (USER_PREF_DEF_SUBCONTRAST + ucContrast-DEF_CONTRAST);
}
*/

Bool EnterDisplayInputHotMenu(void)
{
   if((ePMWakeupStatus==ePMSTS_KEYWAKEUP_ACT)&&(!BacklightFlag))
   {
    Menu_Hide();//Mike 120215 Fix Logo garbage.
    Power_TurnOnPanel();
    Menu_Show();//Mike 120215 Fix Logo garbage.
  
    printMsg("EnterDisplayInputHotMenu");
    return TRUE;
   }
   else
    return FALSE;
   	
}



#if ADJ_BackLight_Enable
Bool AdjustrgbBrightness(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_rgbBRIGHTNESS, 0, 100, 1);

    if (u16TempValue == USER_PREF_rgbBRIGHTNESS)
        return FALSE;
    USER_PREF_rgbBRIGHTNESS = u16TempValue;

    Clr_AdjDcrBacklightFlag();
    CURRENT_RED_BLACK_LEVEL=USER_PREF_rgbBRIGHTNESS;
    CURRENT_GREEN_BLACK_LEVEL=USER_PREF_rgbBRIGHTNESS;
    CURRENT_BLUE_BLACK_LEVEL=USER_PREF_rgbBRIGHTNESS;

    msAPI_AdjustBlacklevel(USER_PREF_WIN_SEL , CURRENT_RED_BLACK_LEVEL, CURRENT_GREEN_BLACK_LEVEL, CURRENT_BLUE_BLACK_LEVEL);


    g_u8DDCCIAControl = 0x10;
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;

    return TRUE;
}

WORD GetrgbBrightnessValue(void)
{
    return USER_PREF_rgbBRIGHTNESS;
}
#endif


Bool AdjustContrast(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;


if(CURRENT_PICTURE_MODE==CT_USER)
{
    u16TempValue = DecIncValue(enAction, CURRENT_CONTRAST_USER, MIN_CONTRAST_VALUE, MAX_CONTRAST_VALUE, 1);

    if (u16TempValue == CURRENT_CONTRAST_USER)
    {
        return FALSE;
    }
    CURRENT_CONTRAST_USER = u16TempValue;
}
else
{
    u16TempValue = DecIncValue(enAction, CURRENT_CONTRAST, MIN_CONTRAST_VALUE, MAX_CONTRAST_VALUE, 1);

    if (u16TempValue == CURRENT_CONTRAST)
    {
        return FALSE;
    }
    CURRENT_CONTRAST = u16TempValue;

}	

    UserPrefContrast_Temp=u16TempValue;
    msAPI_AdjustContrast(USER_PREF_WIN_SEL , u16TempValue);

    g_u8DDCCIAControl = 0x12;
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;
	
    return TRUE;
}

WORD GetContrastValue(void)
{

    return UserPrefContrast_Temp;//CURRENT_CONTRAST;
}

Bool AdjustBrightness(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

if(CURRENT_PICTURE_MODE==CT_USER)
{
    u16TempValue = DecIncValue(enAction, USER_PREF_BRIGHTNESS_USER, 0, 100, 1);

    if (u16TempValue == USER_PREF_BRIGHTNESS_USER)
        return FALSE;
    USER_PREF_BRIGHTNESS_USER = u16TempValue;

}
else
{
    u16TempValue = DecIncValue(enAction, USER_PREF_BRIGHTNESS, 0, 100, 1);

    if (u16TempValue == USER_PREF_BRIGHTNESS)
        return FALSE;
    USER_PREF_BRIGHTNESS = u16TempValue;

}

    Clr_AdjDcrBacklightFlag();
    UserPrefBrightness_Temp=u16TempValue;
    msAPI_AdjustBrightness(u16TempValue);

   if(DCR_ENABLE_FLAG)
   {
      if (DCRBacklight!=DCR_TargetValue)
   	   DCRBacklight=DCR_TargetValue;
   }	
   else
   	DCRBacklight=u16TempValue;
    g_u8DDCCIAControl = 0x10;
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;

    return TRUE;
}

WORD GetBrightnessValue(void)
{
    return UserPrefBrightness_Temp;//USER_PREF_BRIGHTNESS;
}

#if ENABLE_VGA_INPUT
//====================================================
Bool AdjustPhase(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

#if OSD_PHASE_ADJ_METHOD == OSD_PHASE_ADJ_BY_REAL
    u16TempValue = DecIncValue(enAction, UPPHASE_GET_REAL_VALUE(Input_Analog1), 0, MAX_PHASE_VALUE, 1);
    UPPHASE_SET_BY_REAL_VALUE(u16TempValue, (Input_Analog1));
#else
    u16TempValue = DecIncValue(enAction, UPPHASE_GET_OSD_VALUE(Input_Analog1), 0, 100, 1);
    UPPHASE_SET_BY_OSD_VALUE(u16TempValue, (Input_Analog1));
#endif
    drvADC_SetPhaseCode(UPPHASE_GET_REAL_VALUE(Input_Analog1));
    g_u8DDCCIAControl = 0x3E;

    return TRUE;
}

WORD GetPhaseValue(void)
{
#if ENABLE_DIGITAL_INPUT
    if (!CURRENT_INPUT_IS_VGA())
        return 50;
#endif
    return UPPHASE_GET_OSD_VALUE(Input_Analog1);
}

//====================================================
Bool AdjustClock(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_H_TOTAL(Input_Analog1), MIN_CLOCK(Input_Analog1), MAX_CLOCK(Input_Analog1), 1);

    if (u16TempValue == USER_PREF_H_TOTAL(Input_Analog1))
        return FALSE;
    USER_PREF_H_TOTAL(Input_Analog1) = u16TempValue;
    WAIT_V_OUTPUT_BLANKING_START();
    drvADC_AdjustHTotal(USER_PREF_H_TOTAL(Input_Analog1));
    Delay1ms(25);
    g_u8DDCCIAControl = 0x0E;
    return TRUE;
}

WORD GetClockValue(void)    // adjust range
{
#if ENABLE_DIGITAL_INPUT
    if (!CURRENT_INPUT_IS_VGA())
        return 50;
#endif
    return GetScale100Value(USER_PREF_H_TOTAL(Input_Analog1), MIN_CLOCK(Input_Analog1), MAX_CLOCK(Input_Analog1));
}

//====================================================
Bool AdjustHPosition(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, USER_PREF_H_START(Input_Analog1), MIN_H_START(Input_Analog1), MAX_H_START(Input_Analog1), 1);
    if (u16TempValue == USER_PREF_H_START(Input_Analog1))
        return FALSE;
    USER_PREF_H_START(Input_Analog1) = u16TempValue;
    msAPI_AdjustHPosition(USER_PREF_WIN_SEL , USER_PREF_H_START(Input_Analog1));
    g_u8DDCCIAControl = 0x20;//ADJ_H_POSITION;

    return TRUE;
}

WORD GetHPositionValue(void)
{
#if ENABLE_DIGITAL_INPUT
    if (!CURRENT_INPUT_IS_VGA())
        return 50;
#endif
    return 100 - GetScale100Value(USER_PREF_H_START(Input_Analog1), MIN_H_START(Input_Analog1), MAX_H_START(Input_Analog1));
}

//====================================================
Bool AdjustVPosition(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, USER_PREF_V_START(Input_Analog1), MIN_V_START(Input_Analog1), MAX_V_START(Input_Analog1), 1);
    if (u16TempValue == USER_PREF_V_START(Input_Analog1))
        return FALSE;
    USER_PREF_V_START(Input_Analog1) = u16TempValue;
    WAIT_V_OUTPUT_BLANKING();
    msAPI_AdjustVPosition(USER_PREF_WIN_SEL , USER_PREF_V_START(Input_Analog1));

    g_u8DDCCIAControl = 0x30;//ADJ_V_POSITION;
    return TRUE;
}

WORD GetVPositionValue(void)
{
#if ENABLE_DIGITAL_INPUT
    if (!CURRENT_INPUT_IS_VGA())
        return 50;
#endif
return GetScale100Value(USER_PREF_V_START(Input_Analog1), MIN_V_START(Input_Analog1), MAX_V_START(Input_Analog1));
}
#endif //ENABLE_VGA_INPUT
//=====================================================

Bool AdjustMenuTempValueFlipFlop(MENU_ITEM_ACTION enAction)
{
    enAction = enAction;
    g_u8MenuTempValue = (g_u8MenuTempValue) ? (0) : (1);
    Delay1ms(50);
	SetDCRFunction();
    return TRUE;
}

//====================================================
Bool AdjustUserBlueColor(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, CURRENT_BLUE_COLOR, 0, MAX_USER_B_COLOR_VALUE, 1);
    if (u16TempValue == CURRENT_BLUE_COLOR)
        return FALSE;

    CURRENT_BLUE_COLOR = u16TempValue;

    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
    Clr_LR_UpDown_SwitchFlag();	
    Clr_Left_EnterExit_SwitchFlag();	
	
    g_u8DDCCIAControl = 0x1A;

    return TRUE;
}
WORD GetUserBlueColorValue(void)
{
	if(CURRENT_COLOR_TEMP==PCT_9300K)
		return GetScale100Value(USER_PREF_BLUE_COLOR_9300K, 0, MAX_USER_B_COLOR_VALUE);
	else if(CURRENT_COLOR_TEMP==PCT_6500K)
		return GetScale100Value(USER_PREF_BLUE_COLOR_6500K, 0, MAX_USER_R_COLOR_VALUE);
	else if(CURRENT_COLOR_TEMP==PCT_5700K)
		return GetScale100Value(USER_PREF_BLUE_COLOR_5700K, 0, MAX_USER_R_COLOR_VALUE);
	else
    	       return GetScale100Value(CURRENT_BLUE_COLOR, 0, MAX_USER_B_COLOR_VALUE);
}
//====================================================
Bool AdjustUserGreenColor(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, CURRENT_GREEN_COLOR, 0, MAX_USER_G_COLOR_VALUE, 1);
    if (u16TempValue == CURRENT_GREEN_COLOR)
        return FALSE;

    CURRENT_GREEN_COLOR = u16TempValue;

    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
    Clr_LR_UpDown_SwitchFlag();	
    Clr_Left_EnterExit_SwitchFlag();	
    g_u8DDCCIAControl = 0x18;

    return TRUE;
}
WORD GetUserGreenColorValue(void)
{
	if(CURRENT_COLOR_TEMP==PCT_9300K)
		return GetScale100Value(USER_PREF_GREEN_COLOR_9300K, 0, MAX_USER_G_COLOR_VALUE);
	else if(CURRENT_COLOR_TEMP==PCT_6500K)
		return GetScale100Value(USER_PREF_GREEN_COLOR_6500K, 0, MAX_USER_G_COLOR_VALUE);
	else if(CURRENT_COLOR_TEMP==PCT_5700K)
		return GetScale100Value(USER_PREF_GREEN_COLOR_5700K, 0, MAX_USER_G_COLOR_VALUE);
	else
    	       return GetScale100Value(CURRENT_GREEN_COLOR, 0, MAX_USER_G_COLOR_VALUE);

}
//====================================================
Bool AdjustUserRedColor(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_RED_COLOR, 0, MAX_USER_R_COLOR_VALUE, 1);
    if (u16TempValue == CURRENT_RED_COLOR)
        return FALSE;

    CURRENT_RED_COLOR = u16TempValue;

    msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
    Clr_LR_UpDown_SwitchFlag();	
    Clr_Left_EnterExit_SwitchFlag();	
    g_u8DDCCIAControl = 0x16;

    return TRUE;
}
WORD GetUserRedColorValue(void)
{
	if(CURRENT_COLOR_TEMP==PCT_9300K)
		return GetScale100Value(USER_PREF_RED_COLOR_9300K, 0, MAX_USER_R_COLOR_VALUE);
	else if(CURRENT_COLOR_TEMP==PCT_6500K)
		return GetScale100Value(USER_PREF_RED_COLOR_6500K, 0, MAX_USER_R_COLOR_VALUE);
	else if(CURRENT_COLOR_TEMP==PCT_5700K)
		return GetScale100Value(USER_PREF_RED_COLOR_5700K, 0, MAX_USER_R_COLOR_VALUE);
 	else
    	       return GetScale100Value(CURRENT_RED_COLOR, 0, MAX_USER_R_COLOR_VALUE);
}
//====================================================
WORD GetHueValue(void)
{
    return GetScale100Value(CURRENT_HUE, 0, 100);
}
//==========================================
WORD GetProperHue(BYTE ucHue)
{
    WORD temp;
    temp = HUE_ANGLE-((WORD)HUE_ANGLE*ucHue/100);
    temp = ((temp < HUE_ANGLE/2) ? 360 - (HUE_ANGLE/2 - temp):  (temp - HUE_ANGLE/2) ) ;

    MENUFUNC_printData("Proper Hue: %d\r\n", temp);

    return temp;
}

void Set_Hue_Setting(void)
{
  BYTE Hue_R;
  BYTE Hue_G;
  BYTE Hue_B;
  BYTE Hue_C;
  BYTE Hue_M;
  BYTE Hue_Y;


if(CURRENT_COLOR_TEMP==PCT_USER)//PCT_Advanced
{
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(CURRENT_Color_R), eIHC_R);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(CURRENT_Color_G), eIHC_G);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(CURRENT_Color_B), eIHC_B);
	
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(100-CURRENT_Color_C), eIHC_C);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(100-CURRENT_Color_M), eIHC_M);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(100-CURRENT_Color_Y), eIHC_Y);
}
else
{
     Hue_R=Hue_G=Hue_B=Hue_C=Hue_M=Hue_Y= CURRENT_HUE;

    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(Hue_R), eIHC_R);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(Hue_G), eIHC_G);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(Hue_B), eIHC_B);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(Hue_C), eIHC_C);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(Hue_M), eIHC_M);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(Hue_Y), eIHC_Y);
}

}

Bool AdjustVideoHue(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;

    u16TempValue = DecIncValue(enAction, CURRENT_HUE, 0, 100, 1);
    if (u16TempValue == CURRENT_HUE)
        return FALSE;
    CURRENT_HUE = u16TempValue;

    Set_Hue_Setting();
    //msAPI_AdjustVideoHue(USER_PREF_WIN_SEL,GetProperHue(CURRENT_HUE));
    Clr_LR_UpDown_SwitchFlag();	
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;	
    return TRUE;
}

WORD GetSaturationValue(void)
{
    return CURRENT_SATURATION;
}


void Set_Saturation_Setting(void)
{
  BYTE Saturation_R;
  BYTE Saturation_G;
  BYTE Saturation_B;
  BYTE Saturation_C;
  BYTE Saturation_M;
  BYTE Saturation_Y;


if(CURRENT_COLOR_TEMP==PCT_USER)//PCT_Advanced
{
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(CURRENT_Color_R), eICC_R);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(CURRENT_Color_G), eICC_G);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(CURRENT_Color_B), eICC_B);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(100-CURRENT_Color_C), eICC_C);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(100-CURRENT_Color_M), eICC_M);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(100-CURRENT_Color_Y), eICC_Y);

}
else
{
    Saturation_R=Saturation_G=Saturation_B=Saturation_C=Saturation_M=Saturation_Y= CURRENT_SATURATION;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(Saturation_R), eICC_R);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(Saturation_G), eICC_G);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(Saturation_B), eICC_B);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(Saturation_C), eICC_C);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(Saturation_M), eICC_M);
    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(Saturation_Y), eICC_Y);
}

}

Bool AdjustVideoSaturation(MENU_ITEM_ACTION enAction)
{
    WORD u16TempValue;
    //WORD u16Temp;

    u16TempValue = CURRENT_SATURATION;
    u16TempValue = (MENU_ITEM_ACTION)DecIncValue(enAction, u16TempValue, 0, 100, 1);

    if(u16TempValue == CURRENT_SATURATION)
        return FALSE;

    CURRENT_SATURATION = u16TempValue;

   // u16Temp = GetRealSaturation(u16TempValue);

 //if (msAPIWinColorIsYUV(USER_PREF_WIN_SEL))
    Set_Saturation_Setting();
   
    
    //msAPI_AdjustSaturation(USER_PREF_WIN_SEL, u16Temp);
    Clr_LR_UpDown_SwitchFlag();	
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;
    return TRUE;
}



#if ENABLE_VGA_INPUT
Bool AutoConfig(void)
{
    Bool    result=FALSE;
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
    BYTE xdata u8IdxWin= 0;
#endif

    if ((CURRENT_INPUT_IS_VGA())
#if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&PXP_VGA_Auto_Enable)		
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))||((USER_PREF_INPUT_TYPE_SUBWIN3==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN3)))))
#endif
  )	
	;
    else	
       return FALSE;

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
  if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
  {
    result = mStar_AutoGeomtry(USER_PREF_WIN_SEL);
    if(result)
    {
        USER_PREF_AUTOTIME(USER_PREF_INPUT_TYPE) = AUTO_TIMES_BIT;
        SaveUserPref();
    }
  }
  else
  {

        for(u8IdxWin=0 ; u8IdxWin<DISPLAY_MAX_NUMS ; u8IdxWin++)
        {
            if(g_DisplayWindow[u8IdxWin].eDwStatus==WIN_STATUS_OUTPUT_DISPLAYOK)
	          if(MapWin2Port(u8IdxWin)==INPUT_VGA)
                 {  result=0;
		#if ENABLE_VGA_INPUT	  
                     result = mStar_AutoGeomtry(u8IdxWin);
		#endif
			printData("\r\n AA--result=%d", result);
                     if(result)
                    {
                       USER_PREF_AUTOTIME(USER_PREF_INPUT_TYPE) = AUTO_TIMES_BIT;
                       SaveUserPref();
                     }
		   }

          }
  }
#else
    result = mStar_AutoGeomtry(USER_PREF_WIN_SEL);
    if(result)
    {
        USER_PREF_AUTOTIME(USER_PREF_INPUT_TYPE) = AUTO_TIMES_BIT;
        SaveUserPref();
    }

#endif
	
    g_u8DDCCIAControl = 0x1E;
    return result;
}
#endif


//------------------------------------------------------------------------------------------
void SetPropFontIndex(void) // richard OSD Language
{
	if(USER_PREF_LANGUAGE == LANG_English)
        PropFontIndex = PROPINDEX_English;
	else if(USER_PREF_LANGUAGE == LANG_Spanish)
        PropFontIndex = PROPINDEX_Spanish;
	else if(USER_PREF_LANGUAGE == LANG_French)
        PropFontIndex = PROPINDEX_French;
	else if(USER_PREF_LANGUAGE == LANG_German)
        PropFontIndex = PROPINDEX_German;
	else if(USER_PREF_LANGUAGE == LANG_Italian)
        PropFontIndex = PROPINDEX_Italian;
	else if(USER_PREF_LANGUAGE == LANG_Portuguese)
        PropFontIndex = PROPINDEX_Portuguese;
	else if(USER_PREF_LANGUAGE == LANG_Russian)
        PropFontIndex = PROPINDEX_Russian;
	else if(USER_PREF_LANGUAGE == LANG_SChinese)
	    PropFontIndex= PROPINDEX_SChinese;
	else if(USER_PREF_LANGUAGE == LANG_Turkish)
        PropFontIndex = PROPINDEX_Turkish;
	else if(USER_PREF_LANGUAGE == LANG_Korean)
        PropFontIndex = PROPINDEX_Korean;
	else if(USER_PREF_LANGUAGE == LANG_Japanese)
        PropFontIndex = PROPINDEX_Japanese;
	else if(USER_PREF_LANGUAGE == LANG_TChinese)
        PropFontIndex = PROPINDEX_TChinese;
	else if(USER_PREF_LANGUAGE == LANG_BrazilPortuguese)
        PropFontIndex = PROPINDEX_BrazilPortuguese;
	else if(USER_PREF_LANGUAGE == LANG_Dutch)
        PropFontIndex = PROPINDEX_Dutch;
	else if(USER_PREF_LANGUAGE == LANG_Swedish)
        PropFontIndex = PROPINDEX_Swedish;
	else if(USER_PREF_LANGUAGE == LANG_Finnish)
        PropFontIndex = PROPINDEX_Finnish;
	else if(USER_PREF_LANGUAGE == LANG_Polish)
        PropFontIndex = PROPINDEX_Polish;
	else if(USER_PREF_LANGUAGE == LANG_Czech)
        PropFontIndex = PROPINDEX_Czech;
	else if(USER_PREF_LANGUAGE == LANG_Hungarian)
        PropFontIndex = PROPINDEX_Hungarian;
	else if(USER_PREF_LANGUAGE == LANG_Greek)
        PropFontIndex = PROPINDEX_Greek;
	else if(USER_PREF_LANGUAGE == LANG_Ukrainian)
        PropFontIndex = PROPINDEX_Ukrainian;    
    else
		PropFontIndex = LANG_English;

}



WORD GetLanguageValue(void)
{
    return USER_PREF_LANGUAGE;
}

Bool SetLanguage(void)
{

  //  if (USER_PREF_LANGUAGE == g_u8MenuTempValue)
   //     return FALSE;
   // USER_PREF_LANGUAGE = g_u8MenuTempValue;
	
   //printData("USER_PREF_LANGUAGE==%d",USER_PREF_LANGUAGE);

    SetPropFontIndex();	
    //g_u8SaveUserData|=SAVE_MONITOR_SETTING; //080123 Removed by Jison. Flag is set at MIA_INC_VALUE/MIA_DEC_VALUE event.
    Delay1ms(50);
    return TRUE;
}

Bool AdjustLanguageValue(MENU_ITEM_ACTION enAction)
{
    if (enAction==MIA_INC_VALUE)
    	{
    	if(USER_PREF_LANGUAGE>=1)
        USER_PREF_LANGUAGE=(USER_PREF_LANGUAGE-1)%LANG_INDEX_MAX;
	else 
        USER_PREF_LANGUAGE=(LANG_INDEX_MAX-1);
    	}
    else 
        USER_PREF_LANGUAGE=(USER_PREF_LANGUAGE+1)%LANG_INDEX_MAX;

   printData("USER_PREF_LANGUAGE==%d",USER_PREF_LANGUAGE);
	
    return TRUE;
}



//------------------------------------------------------------------------------------------
#if Audio_Source_Enable
BYTE GetOSDAudioInputType(BYTE InputPort)
{
	BYTE osdsrcinput = 0;
#if (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
	if (InputPort==INPUT_VGA)
	  osdsrcinput = DEF_AUDIOSOURCE_0;
#if ENABLE_DVI_INPUT
	else if (InputPort==INPUT_DVI)
	  osdsrcinput = DEF_AUDIOSOURCE_0;
#if ENABLE_DVI2ND_INPUT
	else if (InputPort==INPUT_DVI2ND)
	  osdsrcinput = DEF_AUDIOSOURCE_0;
#endif
#if ENABLE_DVI3ND_INPUT
	else if (InputPort==INPUT_DVI3ND)
	  osdsrcinput = DEF_AUDIOSOURCE_0;
#endif
#endif
#if ENABLE_HDMI_INPUT
	else if (InputPort==INPUT_HDMI)
	  osdsrcinput = DEF_AUDIOSOURCE_2;
#if ENABLE_HDMI2ND_INPUT	
	else if (InputPort==INPUT_HDMI2ND)
	  osdsrcinput = DEF_AUDIOSOURCE_3;
#endif		
#if ENABLE_HDMI3ND_INPUT	
	else if (InputPort==INPUT_HDMI3ND)
	  osdsrcinput = DEF_AUDIOSOURCE_4;
#endif		
#if ENABLE_HDMI4ND_INPUT	
	else if (InputPort==INPUT_HDMI4ND)
	  osdsrcinput = DEF_AUDIOSOURCE_4;
#endif	
#endif
#if ENABLE_DP_INPUT
	else if (InputPort==INPUT_DISPLAYPORT)
	  osdsrcinput = DEF_AUDIOSOURCE_1;
#if ENABLE_DP2ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT2ND)
	  osdsrcinput = DEF_AUDIOSOURCE_1;
#endif
#if ENABLE_DP3ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT3ND)
	  osdsrcinput = DEF_AUDIOSOURCE_1;
#endif
#if ENABLE_DP4ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT4ND)
	  osdsrcinput = DEF_AUDIOSOURCE_1;
#endif
#if ENABLE_DP5ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT5ND)
	  osdsrcinput = DEF_AUDIOSOURCE_1;
#endif
#if ENABLE_DP6ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT6ND)
	  osdsrcinput = DEF_AUDIOSOURCE_1;
#endif

#endif

#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)


#if ENABLE_DVI_INPUT
	
	 if (InputPort==INPUT_DVI)
	  osdsrcinput = 0;
	
#if ENABLE_DVI2ND_INPUT
	 if (InputPort==INPUT_DVI2ND)
	  osdsrcinput = 0;
#endif
#if ENABLE_DVI3ND_INPUT
	 if (InputPort==INPUT_DVI3ND)
	  osdsrcinput = 0;
#endif
#endif
#if ENABLE_HDMI_INPUT
	else if (InputPort==INPUT_HDMI)
	  osdsrcinput = 1;
#if ENABLE_HDMI2ND_INPUT	
	else if (InputPort==INPUT_HDMI2ND)
	  osdsrcinput = 1;
#endif		
#if ENABLE_HDMI3ND_INPUT	
	else if (InputPort==INPUT_HDMI3ND)
	  osdsrcinput = 1;
#endif		
#if ENABLE_HDMI4ND_INPUT	
	else if (InputPort==INPUT_HDMI4ND)
	  osdsrcinput = 1;
#endif	
#endif
#if ENABLE_DP_INPUT
	else if (InputPort==INPUT_DISPLAYPORT)
	  osdsrcinput = 2;
#if ENABLE_DP2ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT2ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP3ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT3ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP4ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT4ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP5ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT5ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP6ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT6ND)
	  osdsrcinput = 2;
#endif

#endif

#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)

#if ENABLE_HDMI_INPUT
	else if (InputPort==INPUT_HDMI)
	  osdsrcinput = 0;
#if ENABLE_HDMI2ND_INPUT	
	else if (InputPort==INPUT_HDMI2ND)
	  osdsrcinput = 0;
#endif		
#if ENABLE_HDMI3ND_INPUT	
	else if (InputPort==INPUT_HDMI3ND)
	  osdsrcinput = 0;
#endif		
#if ENABLE_HDMI4ND_INPUT	
	else if (InputPort==INPUT_HDMI4ND)
	  osdsrcinput = 0;
#endif	
#endif
#if ENABLE_DP_INPUT
	else if (InputPort==INPUT_DISPLAYPORT)
	  osdsrcinput = 1;
#if ENABLE_DP2ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT2ND)
	  osdsrcinput = 1;
#endif
#if ENABLE_DP3ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT3ND)
	  osdsrcinput = 1;
#endif
#if ENABLE_DP4ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT4ND)
	  osdsrcinput = 1;
#endif
#if ENABLE_DP5ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT5ND)
	  osdsrcinput = 1;
#endif
#if ENABLE_DP6ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT6ND)
	  osdsrcinput = 1;
#endif

#endif

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1)

#if ENABLE_HDMI_INPUT
       if (InputPort==INPUT_HDMI)
	  osdsrcinput = 0;
#if ENABLE_HDMI2ND_INPUT	
	else if (InputPort==INPUT_HDMI2ND)
	  osdsrcinput = 1;
#endif		
#if ENABLE_HDMI3ND_INPUT	
	else if (InputPort==INPUT_HDMI3ND)
	  osdsrcinput = 2;
#endif		
#if ENABLE_HDMI4ND_INPUT	
	else if (InputPort==INPUT_HDMI4ND)
	  osdsrcinput = 2;
#endif	
#endif
#if ENABLE_DP_INPUT
	else if (InputPort==INPUT_DISPLAYPORT)
	  osdsrcinput = 3;
#if ENABLE_DP2ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT2ND)
	  osdsrcinput = 3;
#endif
#if ENABLE_DP3ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT3ND)
	  osdsrcinput = 3;
#endif
#if ENABLE_DP4ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT4ND)
	  osdsrcinput = 3;
#endif
#if ENABLE_DP5ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT5ND)
	  osdsrcinput = 3;
#endif
#if ENABLE_DP6ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT6ND)
	  osdsrcinput = 3;
#endif
#endif

#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
#if ENABLE_DVI_INPUT
	 if (InputPort==INPUT_DVI)
	  osdsrcinput = 0;
#if ENABLE_DVI2ND_INPUT
	 if (InputPort==INPUT_DVI2ND)
	  osdsrcinput = 0;
#endif
#if ENABLE_DVI3ND_INPUT
	 if (InputPort==INPUT_DVI3ND)
	  osdsrcinput = 0;
#endif
#endif
#if ENABLE_HDMI_INPUT
	else if (InputPort==INPUT_HDMI)
	  osdsrcinput = 1;
#if ENABLE_HDMI2ND_INPUT	
	else if (InputPort==INPUT_HDMI2ND)
	  osdsrcinput = 2;
#endif		
#if ENABLE_HDMI3ND_INPUT	
	else if (InputPort==INPUT_HDMI3ND)
	  osdsrcinput = 2;
#endif		
#if ENABLE_HDMI4ND_INPUT	
	else if (InputPort==INPUT_HDMI4ND)
	  osdsrcinput = 2;
#endif	
#endif
#if ENABLE_DP_INPUT
	else if (InputPort==INPUT_DISPLAYPORT)
	  osdsrcinput = 3;
#if ENABLE_DP2ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT2ND)
	  osdsrcinput = 3;
#endif
#if ENABLE_DP3ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT3ND)
	  osdsrcinput = 3;
#endif
#if ENABLE_DP4ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT4ND)
	  osdsrcinput = 3;
#endif
#if ENABLE_DP5ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT5ND)
	  osdsrcinput = 3;
#endif
#if ENABLE_DP6ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT6ND)
	  osdsrcinput = 3;
#endif

#endif

#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
#if ENABLE_HDMI_INPUT
	else if (InputPort==INPUT_HDMI)
	  osdsrcinput = 0;
#if ENABLE_HDMI2ND_INPUT	
	else if (InputPort==INPUT_HDMI2ND)
	  osdsrcinput = 1;
#endif		
#if ENABLE_HDMI3ND_INPUT	
	else if (InputPort==INPUT_HDMI3ND)
	  osdsrcinput = 1;
#endif		
#if ENABLE_HDMI4ND_INPUT	
	else if (InputPort==INPUT_HDMI4ND)
	  osdsrcinput = 1;
#endif	
#endif
#if ENABLE_DP_INPUT
	else if (InputPort==INPUT_DISPLAYPORT)
	  osdsrcinput = 2;
#if ENABLE_DP2ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT2ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP3ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT3ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP4ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT4ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP5ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT5ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP6ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT6ND)
	  osdsrcinput = 2;
#endif

#endif



#else
#if ENABLE_VGA_INPUT
	if (InputPort==INPUT_VGA)
	  osdsrcinput = 0;
#endif	
#if ENABLE_DVI_INPUT
	else if (InputPort==INPUT_DVI)
	  osdsrcinput = 0;
#if ENABLE_DVI2ND_INPUT
	else if (InputPort==INPUT_DVI2ND)
	  osdsrcinput = 0;
#endif
#if ENABLE_DVI3ND_INPUT
	else if (InputPort==INPUT_DVI3ND)
	  osdsrcinput = 0;
#endif
#endif
#if ENABLE_HDMI_INPUT
	else if (InputPort==INPUT_HDMI)
	  osdsrcinput = 1;
#if ENABLE_HDMI2ND_INPUT	
	else if (InputPort==INPUT_HDMI2ND)
	  osdsrcinput = 1;
#endif		
#if ENABLE_HDMI3ND_INPUT	
	else if (InputPort==INPUT_HDMI3ND)
	  osdsrcinput = 1;
#endif		
#if ENABLE_HDMI4ND_INPUT	
	else if (InputPort==INPUT_HDMI4ND)
	  osdsrcinput = 1;
#endif	
#endif
#if ENABLE_DP_INPUT
	else if (InputPort==INPUT_DISPLAYPORT)
	  osdsrcinput = 2;
#if ENABLE_DP2ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT2ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP3ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT3ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP4ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT4ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP5ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT5ND)
	  osdsrcinput = 2;
#endif
#if ENABLE_DP6ND_INPUT	
	else if (InputPort==INPUT_DISPLAYPORT6ND)
	  osdsrcinput = 2;
#endif

#endif
#endif

	return osdsrcinput;
}


WORD GetAudioSourceValue(void)
{
    return (gAudioSourceSelectableIndex);
}


#if Audio_Input_Jack_Only_For_Output
#define AudioSourceFirst         AIDIOSOURCE_HDMI1
#else
#define AudioSourceFirst         AIDIOSOURCE_LINE_IN
#endif

Bool AdjustAudioSourceValue(MENU_ITEM_ACTION enAction)
{
#if Audio_Input_Jack_Only_For_Output
 if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF))
  	return FALSE;
#else
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
 if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)&&(INPUT_IS_VGA(USER_PREF_INPUT_TYPE)||INPUT_IS_DVI(USER_PREF_INPUT_TYPE)))
  	return FALSE;
 #endif
 #endif

  AudioReStart:

    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, AudioSourceFirst, AIDIOSOURCE_TYPE_MAX-1, 1);

#if ENABLE_HDMI_INPUT
  if((g_u8MenuTempValue==AIDIOSOURCE_HDMI1)&&(!HDMI_DP_AUDIO_ENABLE(INPUT_HDMI)))
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, AudioSourceFirst, AIDIOSOURCE_TYPE_MAX-1, 1);
#endif
#if ENABLE_HDMI2ND_INPUT
  if((g_u8MenuTempValue==AIDIOSOURCE_HDMI2)&&(!HDMI_DP_AUDIO_ENABLE(INPUT_HDMI2ND)))
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, AudioSourceFirst, AIDIOSOURCE_TYPE_MAX-1, 1);
#endif
#if ENABLE_HDMI3ND_INPUT
  if((g_u8MenuTempValue==AIDIOSOURCE_HDMI3)&&(!HDMI_DP_AUDIO_ENABLE(INPUT_HDMI3ND)))
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, AudioSourceFirst, AIDIOSOURCE_TYPE_MAX-1, 1);
#endif
#if ENABLE_DP_INPUT
  if((g_u8MenuTempValue==AIDIOSOURCE_DP1)&&(!HDMI_DP_AUDIO_ENABLE(INPUT_DISPLAYPORT)))     
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, AudioSourceFirst, AIDIOSOURCE_TYPE_MAX-1, 1);
#endif
if(
#if ENABLE_HDMI_INPUT	
   ((g_u8MenuTempValue==AIDIOSOURCE_HDMI1)&&(!HDMI_DP_AUDIO_ENABLE(INPUT_HDMI)))
#else
   (0)
#endif
#if ENABLE_HDMI2ND_INPUT
   ||((g_u8MenuTempValue==AIDIOSOURCE_HDMI2)&&(!HDMI_DP_AUDIO_ENABLE(INPUT_HDMI2ND)))
#endif
#if ENABLE_HDMI3ND_INPUT
   ||((g_u8MenuTempValue==AIDIOSOURCE_HDMI3)&&(!HDMI_DP_AUDIO_ENABLE(INPUT_HDMI3ND)))
#endif
#if ENABLE_DP_INPUT
   ||((g_u8MenuTempValue==AIDIOSOURCE_DP1)&&(!HDMI_DP_AUDIO_ENABLE(INPUT_DISPLAYPORT)))
#endif   
)
    goto AudioReStart;

	return TRUE;
}


Bool OSD_SetAudioSource( void )
{
#if 0
	if( g_u8MenuTempValue == 0)
	{
		USER_PREF_AUDIO_SOURCE=AIDIOSOURCE_LINE_IN;
	}
#if ENABLE_HDMI_INPUT	
	else if( g_u8MenuTempValue == 1)
	{
		USER_PREF_AUDIO_SOURCE=AIDIOSOURCE_HDMI1;
	}
#endif	
#if ENABLE_HDMI2ND_INPUT	
	else if( g_u8MenuTempValue == 2)
	{
		USER_PREF_AUDIO_SOURCE=AIDIOSOURCE_HDMI2;
	}
#endif	
#if ENABLE_HDMI3ND_INPUT	
	else if( g_u8MenuTempValue == 2)
	{
		USER_PREF_AUDIO_SOURCE=AIDIOSOURCE_HDMI3;
	}
#endif	
#if ENABLE_DP_INPUT	
	else if( g_u8MenuTempValue == 2)
	{
	       EnableDPMute();		
		USER_PREF_AUDIO_SOURCE=AIDIOSOURCE_DP1;
	}
#endif	
#if ENABLE_DP2ND_INPUT	
	else if( g_u8MenuTempValue == 4)
	{
		USER_PREF_AUDIO_SOURCE=AIDIOSOURCE_DP2;
	}
#endif	
#else
      USER_PREF_AUDIO_SOURCE=g_u8MenuTempValue;

	if( USER_PREF_AUDIO_SOURCE == AIDIOSOURCE_DP1||USER_PREF_AUDIO_SOURCE == AIDIOSOURCE_DP2)
	{
	       EnableDPMute();		
	}

#endif



    SetAudioSource(USER_PREF_INPUT_TYPE);
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;


	return TRUE;
}

void SetAudioSource( BYTE ucInputPort)
{
       ucInputPort=ucInputPort;
	   
       printData("USER_PREF_AUDIO_SRC(INPUT_HDMI)==%d",USER_PREF_AUDIO_SRC(INPUT_HDMI));
#if ENABLE_DP_INPUT	   
       printData("USER_PREF_AUDIO_SRC(INPUT_DISPLAYPORT)==%d",USER_PREF_AUDIO_SRC(INPUT_DISPLAYPORT));
#endif
       printData("USER_PREF_AUDIO_SOURCE==%d",USER_PREF_AUDIO_SOURCE);

	{
	#if ENABLE_HDMI_INPUT	
	     if((USER_PREF_AUDIO_SOURCE==AIDIOSOURCE_HDMI1)&&(HDMI_DP_AUDIO_ENABLE(INPUT_HDMI)))
	    {   
	        printMsg("msAPI_AudioSourceSel(INPUT_HDMI)");
               msAPI_AdjustVolume(0);			 
	        msAPI_AudioSourceSel(INPUT_HDMI);
               SetAudioMuteOnOff();
	     	 gAudioSourceSelectableIndex = AIDIOSOURCE_HDMI1;
		 msAPI_AdjustVolume(USER_PREF_VOLUME);	   	 
	    }
	#else
           if(0)
		  ;
	#endif
	#if ENABLE_HDMI2ND_INPUT	 
	    else if(USER_PREF_AUDIO_SOURCE==AIDIOSOURCE_HDMI2&&(HDMI_DP_AUDIO_ENABLE(INPUT_HDMI2ND)))
	    {   
	        printMsg("msAPI_AudioSourceSel(INPUT_HDMI2)");
               msAPI_AdjustVolume(0);			 
	        msAPI_AudioSourceSel(INPUT_HDMI2ND);
               SetAudioMuteOnOff();
	     	 gAudioSourceSelectableIndex = AIDIOSOURCE_HDMI2;
		 msAPI_AdjustVolume(USER_PREF_VOLUME);	   	 
	    }
	#endif	
	#if ENABLE_HDMI3ND_INPUT	 
	    else if(USER_PREF_AUDIO_SOURCE==AIDIOSOURCE_HDMI3&&(HDMI_DP_AUDIO_ENABLE(INPUT_HDMI3ND)))
	    {   
	        printMsg("msAPI_AudioSourceSel(INPUT_HDMI3)");
               msAPI_AdjustVolume(0);			 
	        msAPI_AudioSourceSel(INPUT_HDMI3ND);
               SetAudioMuteOnOff();
	     	 gAudioSourceSelectableIndex = AIDIOSOURCE_HDMI3;
		 msAPI_AdjustVolume(USER_PREF_VOLUME);	   	 
	    }
	#endif	
	
	#if ENABLE_DP_INPUT	
	    else if((USER_PREF_AUDIO_SOURCE==AIDIOSOURCE_DP1)&&(HDMI_DP_AUDIO_ENABLE(INPUT_DISPLAYPORT)))
	    {  
	         printMsg("msAPI_AudioSourceSel(INPUT_DISPLAYPORT)");
                msAPI_AdjustVolume(0);		
	         msAPI_AudioSourceSel(INPUT_DISPLAYPORT);
                SetAudioMuteOnOff();
	     	  gAudioSourceSelectableIndex = AIDIOSOURCE_DP1;	 
                msAPI_AdjustVolume(USER_PREF_VOLUME);	    
	    }
	#endif	
	#if ENABLE_DP2ND_INPUT
	    else if(USER_PREF_AUDIO_SOURCE==AIDIOSOURCE_DP2&&(HDMI_DP_AUDIO_ENABLE(INPUT_DISPLAYPORT2ND)))
	    {    
	         printMsg("msAPI_AudioSourceSel(INPUT_DISPLAYPORT2)");
                msAPI_AdjustVolume(0);		
	         msAPI_AudioSourceSel(INPUT_DISPLAYPORT2ND);
                SetAudioMuteOnOff();
	     	  gAudioSourceSelectableIndex = AIDIOSOURCE_DP2;	 
                msAPI_AdjustVolume(USER_PREF_VOLUME);	    
	    }
	#endif	
	    else
	    {    
	    	#if (Audio_Input_Jack_Only_For_Output)
                msAPI_AdjustVolume(0);		
	         msAPI_AudioSourceSel(INPUT_HDMI);
                SetAudioMuteOnOff();
	     	  gAudioSourceSelectableIndex = AIDIOSOURCE_HDMI1;	 
                msAPI_AdjustVolume(USER_PREF_VOLUME);	    
              #else
	         printMsg("msAPI_AudioSourceSel(INPUT_VGA)");
                msAPI_AdjustVolume(0);	    
	   #if ENABLE_VGA_INPUT			
                msAPI_AudioSourceSel(INPUT_VGA);
	   #elif ENABLE_DVI_INPUT
                msAPI_AudioSourceSel(INPUT_DVI);
	   #endif
                SetAudioMuteOnOff();
                gAudioSourceSelectableIndex = AIDIOSOURCE_LINE_IN;	
                msAPI_AdjustVolume(USER_PREF_VOLUME);	    
		  //USER_PREF_AUDIO_SOURCE=gAudioSourceSelectableIndex;
		#endif  
	    }
	}

	   
}



#endif

#if 1//SleepMode_Enable
WORD GetSleepModeValue(void)
{
    return USER_PREF_SLEEPMODE;
}


Bool SetSleepMode(void)
{
    if (USER_PREF_SLEEPMODE == g_u8MenuTempValue)
        return FALSE;
     g_u8MenuTempValue=USER_PREF_SLEEPMODE;
	
   //printData("USER_PREF_SLEEPMODE==%d",USER_PREF_SLEEPMODE);

   if(USER_PREF_SLEEPMODE==SleepMode_30Min)
	SleepModeCounter=30*60;
   else if(USER_PREF_SLEEPMODE==SleepMode_60Min)
	SleepModeCounter=60*60;
   else if(USER_PREF_SLEEPMODE==SleepMode_120Min)
	SleepModeCounter=120*60;
   else
	SleepModeCounter=0;

    Delay1ms(50);
    return TRUE;
}

Bool AdjustSleepModeValue(MENU_ITEM_ACTION enAction)
{
	
    if (enAction==MIA_INC_VALUE)
        USER_PREF_SLEEPMODE=(USER_PREF_SLEEPMODE+1)%SleepMode_Max;
    else if (USER_PREF_SLEEPMODE>0)
        USER_PREF_SLEEPMODE=(USER_PREF_SLEEPMODE-1);
    else
        USER_PREF_SLEEPMODE=SleepMode_Max-1;

   printData("USER_PREF_SLEEPMODE==%d",USER_PREF_SLEEPMODE);
	SetSleepMode();
    return TRUE;
}
#elif Enable_Corss_FY
WORD GetCrossModeValue(void)
{
    return USER_PREF_LOSMODE;
}


Bool SetCrossMode(void)
{
    if (USER_PREF_LOSMODE == g_u8MenuTempValue)
        return FALSE;
    USER_PREF_LOSMODE = g_u8MenuTempValue;
	
   //printData("USER_PREF_LOSMODE==%d",USER_PREF_LOSMODE);
    //Osd_LoadHOTCROSSFont();		
    Delay1ms(50);
    return TRUE;
}

Bool AdjustCrossModeValue(MENU_ITEM_ACTION enAction)
{

    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, Cross_Mode_1, Cross_Mode_Max-1, 1);
    Delay1ms(50);
	
   printData("g_u8MenuTempValue==%d",g_u8MenuTempValue);
	
    return TRUE;
}


#endif

void SaveUserPref(void)
{
    SaveMonitorSetting();
#if ENABLE_VGA_INPUT
    if ((CURRENT_INPUT_IS_VGA())
#if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&PXP_VGA_Auto_Enable)		
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))||((USER_PREF_INPUT_TYPE_SUBWIN3==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN3)))))
#endif
)
        SaveModeSetting();
#endif
}
#if DP_Version_Enable
void SetDPConfigType(void)
{
    msAPI_combo_IPSetDPVersion((EN_COMBO_IP_SELECT) g_InputPort[MapWin2Port(USER_PREF_WIN_SEL)].eIPMux, (EN_DP_VERSION)USER_PREF_DPConfigType);
    //msAPISrcSetHPD(MapWin2Port(USER_PREF_WIN_SEL), DFT_HPD_PERIOD); // set reasonable value, e.g. >= 300ms
    ApplyDaisyChainType();
}
Bool OSD_SetDPConfigType(void)
{
    if (USER_PREF_DPConfigType == g_u8MenuTempValue)
        return FALSE;
    USER_PREF_DPConfigType = g_u8MenuTempValue;
    SaveMonitorSetting();
    SetDPConfigType();
    return TRUE;
}
WORD GetDPConfigTypeValue(void)
{
    return USER_PREF_DPConfigType;
}
Bool AdjustDPConfigType(MENU_ITEM_ACTION enAction)
{
#if defined(KTC_Project)
	return TRUE;
#endif

    if (enAction==MIA_DEC_VALUE)
        g_u8MenuTempValue=(g_u8MenuTempValue+1)%DPconfigtype_NUMS;
    else if (g_u8MenuTempValue>0)
        g_u8MenuTempValue=(g_u8MenuTempValue-1);
    else
        g_u8MenuTempValue=DPconfigtype_NUMS-1;
    return TRUE;
}
#endif

#if HDMI_VerSion_Enable
void SetHDMIConfigType(void)
{
   
   msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	 	
   msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE); 
   
   ReLoadHDMI_EDID_3840x2160();
   ForceDelay1ms(500); // delay 1 msec				
   
   msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE); 
   msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	
   
}
Bool OSD_SetHDMIConfigType(void)
{
//printData("2222 g_u8MenuTempValue==%d",g_u8MenuTempValue);
//printData("2222 USER_PREF_HDMIConfigType==%d",USER_PREF_HDMIConfigType);

    if (USER_PREF_HDMIConfigType == g_u8MenuTempValue)
        return FALSE;
    USER_PREF_HDMIConfigType = g_u8MenuTempValue;
    SaveMonitorSetting();
    SetHDMIConfigType();
    return TRUE;
}
WORD GetHDMIConfigTypeValue(void)
{
    return USER_PREF_HDMIConfigType;
}
Bool AdjustHDMIConfigType(MENU_ITEM_ACTION enAction)
{
   g_u8MenuTempValue=USER_PREF_HDMIConfigType;
//printData("0000 g_u8MenuTempValue==%d",g_u8MenuTempValue);
    if (enAction==MIA_DEC_VALUE)
        g_u8MenuTempValue=(g_u8MenuTempValue+1)%HDMIconfigtype_NUMS;
    else if (g_u8MenuTempValue>0)
        g_u8MenuTempValue=(g_u8MenuTempValue-1);
    else
        g_u8MenuTempValue=HDMIconfigtype_NUMS-1;

//printData("11111 g_u8MenuTempValue==%d",g_u8MenuTempValue);
    return TRUE;
}
#endif


#if Enable_Menu_Panel_Rotate_180
/*
void Set_Rotate_EDID_Updata(void)
{
   mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
   msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[SrcInputPortM].eIPMux, FALSE);	 	

   Set_LoadVGAEdidFlag();
   Set_LoadDigital1EdidFlag();
   Set_LoadDigital2EdidFlag();
   Set_LoadDigital3EdidFlag();

   Mstar_InitDDC();
   EDID_ReLoad_Func(0);
   msAPI_combo_LoadEDID_Func();
      
   msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[SrcInputPortM].eIPMux, TRUE);	
   mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
}
*/

Bool Set_RotateType(void)
{
//printData("2222 g_u8MenuTempValue==%d",g_u8MenuTempValue);
//printData("2222 USER_PREF_ROTATE_INDEX==%d",USER_PREF_ROTATE_INDEX);
//BYTE USER_PREF_ROTATE_INDEX_Prev=USER_PREF_ROTATE_INDEX;

    if (USER_PREF_ROTATE_INDEX == g_u8MenuTempValue)
        return FALSE;
    USER_PREF_ROTATE_INDEX = g_u8MenuTempValue;

    APPWindow_Configure((eMULTIWIN_Type)USER_PREF_MULTI_WIN_MODE, (ePIPSIZE_Type)USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2));
/*
if(((USER_PREF_ROTATE_INDEX==Rotate_Normal)&&(USER_PREF_ROTATE_INDEX_Prev==Rotate_180))
   ||((USER_PREF_ROTATE_INDEX==Rotate_180)&&(USER_PREF_ROTATE_INDEX_Prev==Rotate_Normal))
   ||((USER_PREF_ROTATE_INDEX==Rotate_90)&&(USER_PREF_ROTATE_INDEX_Prev==Rotate_270))
   ||((USER_PREF_ROTATE_INDEX==Rotate_270)&&(USER_PREF_ROTATE_INDEX_Prev==Rotate_90))
)
   ;
else	
    Set_Rotate_EDID_Updata();
*/	
    return TRUE;
}
WORD GetRotateTypeValue(void)
{
    return USER_PREF_ROTATE_INDEX;
}
Bool AdjustRotateType(MENU_ITEM_ACTION enAction)
{
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, Rotate_Normal, RotateIndex_NUMS-1, 1);
    Delay1ms(50);

//printData("11111 g_u8MenuTempValue==%d",g_u8MenuTempValue);
    return TRUE;
}
#endif



#if ENABLE_VGA_INPUT
Bool ResetPositionSetting(void)
{
    BYTE u8ModeIndex, u8Flag;
    BYTE InputPort=MapWin2Port(USER_PREF_WIN_SEL);

    if ((CURRENT_INPUT_IS_VGA())
#if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&PXP_VGA_Auto_Enable)		
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))||((USER_PREF_INPUT_TYPE_SUBWIN3==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN3)))))
#endif
)	
    {
        USER_PREF_AUTO_HSTART(InputPort)=StandardModeHStart(InputPort);//Reset USER_PREF_AUTO_H_START to avoid USER_PREF_H_START out of (MIN_H_START,MAX_H_START) range if following auto config process failed.
        USER_PREF_AUTO_VSTART(InputPort)=StandardModeVStart(InputPort);
        //reset current H/V position to default
        USER_PREF_H_START(InputPort)=StandardModeHStart(InputPort);
        USER_PREF_V_START(InputPort)=StandardModeVStart(InputPort);
        USER_PREF_H_TOTAL(InputPort)=StandardModeHTotal(InputPort);
        drvADC_AdjustHTotal(USER_PREF_H_TOTAL(InputPort));      //avoid auto HTotal failed in the following auto config process.
    }
//-------------------------------------------------
    USER_PREF_AUTOTIME(InputPort)&=~AUTO_TIMES_BIT;

    u8Flag=SrcFlags(InputPort); //UserModeFlag
    u8ModeIndex=g_u8SaveIndex;
    SrcFlags(InputPort)&=~bUserMode;
    for (g_u8SaveIndex=0; g_u8SaveIndex<PRESET_MODE_NUMBER; g_u8SaveIndex++)
    {
        USER_PREF_MODE_CHECKSUM(Input_Analog1)=0xFF;
        SaveNVRAMBlock( EEP_MODE_MEMBER_ADDR( u8MODE_CheckSum ), sizeof(USER_PREF_MODE_CHECKSUM(Input_Analog1)) );
    }
    SrcFlags(InputPort)|=bUserMode;
    for (g_u8SaveIndex=0; g_u8SaveIndex<USER_MODE_NUMBER; g_u8SaveIndex++)
    {
		USER_PREF_MODE_CHECKSUM(Input_Analog1)=0xFF;
        SaveNVRAMBlock( EEP_MODE_MEMBER_ADDR( u8MODE_CheckSum ), sizeof(USER_PREF_MODE_CHECKSUM(Input_Analog1)) );
    }
    SrcFlags(InputPort)=u8Flag;
    g_u8SaveIndex=u8ModeIndex;
    SaveModeSetting();

    return TRUE;
}
#endif

extern Bool msDrvSetupMode(BYTE SCDetWin);
Bool ResetColorSettingAll(void)
{
    BYTE i,j;
    BYTE USER_PREF_WIN_SRC_BK[WIND_CNT];
    //USER_PREF_WIN_SEL               = DISPLAY_WIN1;
    WAIT_V_OUTPUT_BLANKING();


    USER_PREF_rgbBRIGHTNESS=DEF_rgbBRIGHTNESS;

	
    for (i=0; i<PORT_CNT; i++)
    {
        USER_PREF_WIDE_MODE(i)                = DEF_WIDE_MODE;
    }

    for (i=0; i<WIND_CNT; i++)
    {
        USER_PREF_WIN_SRC_BK[i] = USER_PREF_WIN_SRC(i);
    }

    for(i=0;i<PORT_CNT;i++)
    {
        for (j=0; j<WIND_CNT; j++)
        {
            USER_PREF_WIN_SRC(j) = i;

            USER_PREF_CONTRAST(j)            = DEF_CONTRAST;
            USER_PREF_CONTRAST_USER(j)            = DEF_CONTRAST;
            USER_PREF_INPUT_COLOR_FORMAT(j)  = DEF_INPUT_COLOR_FORMAT;
            USER_PREF_INPUT_COLOR_RANGE(j)  = OSD_INPUT_COLOR_RANGE_DEF;
            USER_PREF_GAMMA(j)               = DEF_GAMMA;

            USER_PREF_SATURATION(j)          = DEF_VIDEO_SATURATION;
            USER_PREF_HUE(j)                 = DEF_VIDEO_HUE;
			
            USER_PREF_COLOR_TEMP(j)          = DEF_COLOR_TEMP;
            USER_PREF_PICTURE_MODE(j)          = DEF_PICTURE_MODE;
			
            USER_PREF_SHARPNESS(j)           = DEF_SHARPNESS;
 
            USER_PREF_LOWBLUELIGHT(j)      = DEF_LOWBLUELIGHT;  
				
            USER_PREF_RED_COLOR(j)           = DEF_R_COLOR_VALUE;
            USER_PREF_GREEN_COLOR(j)         = DEF_G_COLOR_VALUE;
            USER_PREF_BLUE_COLOR(j)          = DEF_B_COLOR_VALUE;
            USER_PREF_R_HUE(j)               = DEF_R_HUE;
            USER_PREF_G_HUE(j)               = DEF_G_HUE;
            USER_PREF_B_HUE(j)               = DEF_B_HUE;
            USER_PREF_C_HUE(j)               = DEF_C_HUE;
            USER_PREF_M_HUE(j)               = DEF_M_HUE;
            USER_PREF_Y_HUE(j)               = DEF_Y_HUE;
            USER_PREF_R_SATURATION(j)        = DEF_R_SATURATION;
            USER_PREF_G_SATURATION(j)        = DEF_G_SATURATION;
            USER_PREF_B_SATURATION(j)        = DEF_B_SATURATION;
            USER_PREF_C_SATURATION(j)        = DEF_C_SATURATION;
            USER_PREF_M_SATURATION(j)        = DEF_M_SATURATION;
            USER_PREF_Y_SATURATION(j)        = DEF_Y_SATURATION;

            USER_PREF_Color_R(j)        = DEF_COLOR_R;
            USER_PREF_Color_G(j)        = DEF_COLOR_G;
            USER_PREF_Color_B(j)        = DEF_COLOR_B;
            USER_PREF_Color_C(j)        = DEF_COLOR_C;
            USER_PREF_Color_M(j)        = DEF_COLOR_M;
            USER_PREF_Color_Y(j)        = DEF_COLOR_Y;
			
#if ADJ_BackLight_Enable
            USER_PREF_RED_BLACK_LEVEL(j)     = USER_PREF_rgbBRIGHTNESS;
            USER_PREF_GREEN_BLACK_LEVEL(j)   = USER_PREF_rgbBRIGHTNESS;
            USER_PREF_BLUE_BLACK_LEVEL(j)    = USER_PREF_rgbBRIGHTNESS;
#else
            USER_PREF_RED_BLACK_LEVEL(j)     = DEF_RED_BLACK_LEVEL;
            USER_PREF_GREEN_BLACK_LEVEL(j)   = DEF_GREEN_BLACK_LEVEL;
            USER_PREF_BLUE_BLACK_LEVEL(j)    = DEF_BLUE_BLACK_LEVEL;
#endif			
            USER_PREF_SUPER_RESOLUTION(j)    = DEF_SUPER_RESOLUTION;
            USER_PREF_NOISE_REDUCTION(j)     = DEF_NOISE_REDUCTION;
        }


    }
    for (i=0; i<WIND_CNT; i++)
    {
        USER_PREF_WIN_SRC(i) = USER_PREF_WIN_SRC_BK[i];
    }

        USER_PREF_BRIGHTNESS               = DEF_BRIGHTNESS;
        USER_PREF_BRIGHTNESS_USER      = DEF_BRIGHTNESS;

        USERPREFLOWBLURAY=DEF_LOWBLUELIGHT;
        USER_PREF_DLC_MODE=DEF_DLC_MODE;
#if ENABLE_DEMURA_FUNCTION
        CLR_UNIFORMITY_COMPENSATION_FLAG();
	 CLR_DEMURA_MEASURE_STANDBY_FLAG();	
#endif	 


        Menu_AdjustColorSetting();
#if ENABLE_SUPER_RESOLUTION
        CURRENT_SUPER_RESOLUTION = DEF_SUPER_RESOLUTION;
        UserPrefNoiseReduction=CURRENT_SUPER_RESOLUTION;

        //SetSuperResolution();
        msPQ_SetGRule_SR(USER_PREF_WIN_SEL, CURRENT_SUPER_RESOLUTION);
#endif
//	USERPREF_MAINSR = DEF_SUPER_RESOLUTION;


    //SaveMonitorSetting();
    return TRUE;
}




Bool ResetAllSetting(void)
{
#if DP_PBP_RELOAD_EDID	
    BYTE HPD_Status=0;
#endif
#if HDMI_PBP_RELOAD_EDID	
    BYTE HDMI_HPD_Status=0;
#endif	
#if DVI_PBP_RELOAD_EDID	
    BYTE DVI_HPD_Status=0;
#endif	

 if(g_u8MenuTempValue==0)
   return FALSE;
 
    g_u8DDCCIAControl = 0x04;
/*
    WAIT_V_OUTPUT_BLANKING();
    for(i=0; i<DISPLAY_MAX_NUMS; i++)
    {
        msAPIWinEnableFrameColor(i, TRUE);
    }
*/
    //Menu_Hide();

  #if USB_Updata_FW_Enable	
  if(USBUpdataFwStatus!=USB_Writting)
    USBUpdataFwStatus=USB_Idle;
  #endif		


    USER_PREF_OSD_TIME = DEF_OSD_TIMEOUT;

    USER_PREF_OSD_H_POSITION = DEF_OSD_H_POSITION;
    USER_PREF_OSD_V_POSITION = DEF_OSD_V_POSITION;
    //CLR_MENU_ROTATION_FLAG();
	
    USER_PREF_OSD_ROTATION=Osd_Rotation_Normal;
#if (!(defined(GK3B_Project)))
    SetMenuPositionNormal();
#endif

    USER_PREF_TRANSPARENCY = DEF_TRANSPARENCY;
#if DP_Version_Enable
  if(USER_PREF_DPConfigType != DEF_DPTYPE)
   {
    USER_PREF_DPConfigType = DEF_DPTYPE;
    SetDPConfigType();
  }
#endif

#if HDMI_VerSion_Enable
  if(USER_PREF_HDMIConfigType != DEF_HDMITYPE)
   {
    USER_PREF_HDMIConfigType = DEF_HDMITYPE;
    SetHDMIConfigType();
  }
#elif Enable_Menu_Panel_Rotate_180
   if(USER_PREF_ROTATE_INDEX != Rotate_Normal)
   {
       USER_PREF_ROTATE_INDEX=Rotate_Normal;
	if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)   
       APPWindow_Configure((eMULTIWIN_Type)USER_PREF_MULTI_WIN_MODE, (ePIPSIZE_Type)USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2));
   }
#endif


if(USER_PREF_LOSMODE!=Cross_Mode_1)
{
    USER_PREF_LOSMODE =Cross_Mode_1;
}


#if HDMI_HPD_Enable
if(!MENU_HDMI_HPD_FLAG)
{
    SET_MENU_HDMI_HPD_FLAG();
}
#endif

#if (AudioFunc) //XXXXX
    CLR_AUDIO_MUTE_FLAG();
    USER_PREF_AUDIO_SRC(0)   = DEF_AUDIOSOURCE_0;
    USER_PREF_AUDIO_SRC(1)   = DEF_AUDIOSOURCE_1;
    USER_PREF_AUDIO_SRC(2)   = DEF_AUDIOSOURCE_2;
    USER_PREF_AUDIO_SRC(3)   = DEF_AUDIOSOURCE_3;
    USER_PREF_AUDIO_SRC(4)   = DEF_AUDIOSOURCE_4;
    //USER_PREF_AUDIO_SRC(5)   = DEF_AUDIOSOURCE_5;
	
    USER_PREF_VOLUME = DEF_VOLUME; //Steven110617
   
#if Auto_Switch_AudioSource
     USER_PREF_AUDIO_SOURCE=GetCurAudioSourceIndex(USER_PREF_INPUT_TYPE);
#endif

#if Audio_Source_Enable
    SetAudioSource(USER_PREF_INPUT_TYPE);
#endif

#if ENABLE_ADJ_AUDIO_OUTPUT	
#if ((defined(KTC_Project)&&PanelType==PanelLSM315HP01)||(defined(ForZhouYang315HP_Project)))
     USER_PREF_AUDIO_OUTPUTTYPE              = EarPhone;
#else
    USER_PREF_AUDIO_OUTPUTTYPE               = Speaker;
#endif
    //OSD_SetAudioOutput();
#endif	
	
    //msAudioDAC_Mute(TRUE);//AudioMute(TRUE);Sky120509
    //Delay1ms(10);
    //AudioMuxCtrl();
    //Delay1ms(10);
    //msAudioDAC_Mute(FALSE);//AudioMute(FALSE);Sky120509
    SetAudioMuteOnOff();
    msAPI_AdjustVolume(USER_PREF_VOLUME);
#endif

#if HKC_RGB_Range_Func	
 if(CURRENT_INPUT_COLOR_RANGE!= OSD_INPUT_COLOR_RANGE_DEF)
  {
    CURRENT_INPUT_COLOR_RANGE=OSD_INPUT_COLOR_RANGE_DEF;
    APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
   }
#endif	

#if 1//SleepMode_Enable
    USER_PREF_SLEEPMODE              =  DEF_SLEEPMODE;
    SleepModeCounter=0;
#endif

#if ENABLE_WIDE_MODE
if(USER_PREF_WIDE_MODE(SrcInputPortM)  != DEF_WIDE_MODE)
{
    Menu_Hide();
    USER_PREF_WIDE_MODE(SrcInputPortM)  = DEF_WIDE_MODE;
    SetWideMode();
}
#endif

#if 1//(defined(RunFeng_Project))
    if(FACTORY_MODE_FLAG)
    {
       USER_PREF_BACKLIGHT_TIME = 0;
    }
#endif	

    ResetColorSettingAll();


#if LED_HOT_Menu_Func
 if((USER_PREF_LEDMODE_INDEX  != DEF_LED_MODE_INDEX_TYPE)/*||(LED_FUNC_ON_FLAG)*/||(USER_PREF_LEDColorful_MODE!=DEF_LED_MODE)||(USER_PREF_LedStrength!=DEF_LED_STRENGTH)||(USER_PREF_LedModeType!=DEF_LED_MODE_TYPE))
 {
    //CLR_LED_FUNC_ON_FLAG();
    USER_PREF_LEDMODE_INDEX = DEF_LED_MODE_INDEX_TYPE;
	
    USER_PREF_LEDColorful_MODE           =DEF_LED_MODE;
    USER_PREF_LedStrength           =DEF_LED_STRENGTH;
    USER_PREF_LedModeType           =DEF_LED_MODE_TYPE;
 if(IS_LED_Power())	
    SetLedFunc(0xFF, 0xFF, 0xFF, 0xFF);
    ForceDelay1ms(60);
    Clr_LED_Power();	  
    Clr_LEDActiveOnFlag();		  
    //SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
 }
/*
 if((!LED_FUNC_ON_FLAG)||(USER_PREF_LEDColorful_MODE!=DEF_LED_MODE)||(USER_PREF_LedStrength!=DEF_LED_STRENGTH)||(USER_PREF_LedModeType!=DEF_LED_MODE_TYPE))
 {
    USER_PREF_LEDColorful_MODE           =DEF_LED_MODE;
    USER_PREF_LedStrength           =DEF_LED_STRENGTH;
    USER_PREF_LedModeType           =DEF_LED_MODE_TYPE;
    SET_LED_FUNC_ON_FLAG();
    SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
 }
 */
#endif

if(USER_PREF_LEDMODE!=DEF_LEDMODE)
{
   USER_PREF_LEDMODE=DEF_LEDMODE;
    if ( USER_PREF_LEDMODE == OffLedMode )
    {
       hw_ClrGreenLed();
       hw_ClrAmberLed();
    }
    else if ( USER_PREF_LEDMODE == GreenLedMode )
    {
       hw_SetGreenLed();
       hw_ClrAmberLed();
    }
    else if ( USER_PREF_LEDMODE == RedLedMode )
    {
       hw_ClrGreenLed();
       hw_SetAmberLed();
    }
   SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u8SYS_LedMode), sizeof(USER_PREF_LEDMODE));
}


#if ENABLE_DCR
        CLR_DCR_ENABLE_FLAG();
	 CLR_DCR_FLAG_ENABLE();
#endif
#if ENABLE_RTE
  if((USER_PREF_RESPONSE_TIME!=DEF_RESPONSE_TIME))//||(RTE_ENABLE_FLAG))
  {
   USER_PREF_RESPONSE_TIME=DEF_RESPONSE_TIME;
    mStar_SetRTE();	
  }
#endif
    //printData("SrcInputPortM==%d",SrcInputPortM);
    if(CURRENT_PICTURE_MODE==CT_VIVID)
    {
    UserPrefBrightness_Temp=30;
    UserPrefContrast_Temp=50;
    }
   else
   {
    UserPrefBrightness_Temp=USER_PREF_BRIGHTNESS;
    UserPrefContrast_Temp=CURRENT_CONTRAST;
   }
    msAPI_AdjustBrightness(UserPrefBrightness_Temp);
    DCRBacklight=UserPrefBrightness_Temp;	
    msAPI_AdjustContrast(USER_PREF_WIN_SEL , UserPrefContrast_Temp);
    SetSharpness();


#if ENABLE_MULTI_WINDOW_SETTINGS_MENU

    USER_PREF_WIN_SEL                       = DEF_MULTIWIN_SEL;
    USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)  = DEF_PIP_SIZE;

    USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = DEF_PIP_H_POSITION;
    USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = DEF_PIP_V_POSITION;

if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
{
    //printMsg("555555555555");
    Menu_Hide();
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);   
    USER_PREF_PIP_POSITION =PIPPosition_Top_Right;
    USER_PREF_MULTI_WIN_MODE=eMULTIWIN_OFF;	

#if DP_PBP_RELOAD_EDID
#if Disable_4K60HZ_Timing
 if(((DP_EDID_3840x2160_Flag)&&(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF))
 ||((!DP_EDID_3840x2160_Flag)&&(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)) )
#else
  if(((DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))))
  ||((!DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DISPLAYPORT))))
  )
  #endif
       {
         
         msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, FALSE);	 		 
         HPD_Status=1;
         
         EDID_ReLoad_Func(0);
         msAPI_combo_LoadEDID_Func();
         printMsg("DP_PBP_RELOAD_EDID 00000000");	  
         //ForceDelay1ms(600); // delay 1 msec				
        }
 #endif
 
#if HDMI_PBP_RELOAD_EDID
#if Disable_4K60HZ_Timing
    if(((HDMI_EDID_3840x2160_Flag)&&(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF))
       ||((!HDMI_EDID_3840x2160_Flag)&&(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)) )
#else
 if(((HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))))
 ||((!HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))))
 ||((HDMI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI))))
 ||((HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_HDMI))))
 )
 #endif
  {
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	 	
        HDMI_HPD_Status=1;			 

        ReLoadHDMI_EDID();
        printMsg("HDMI_PBP_RELOAD_EDID 0000000");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif

#if DVI_PBP_RELOAD_EDID
 if(((DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))))
 ||((!DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))))
 ||((DVI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DVI))))
 ||((!DVI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DVI))))
 ||((DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DVI))))
 ||((!DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DVI))))
 )
  {
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, FALSE);	 	
        DVI_HPD_Status=1;			 

        ReLoadDVI_EDID();
        printMsg("DVI_PBP_RELOAD_EDID DVI");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif

#if ENABLE_DP_INPUT||ENABLE_HDMI_INPUT
    if(FreeSyncFlag)
    {
       Clr_FreeSyncFlag();
	SetFreeSyncOnOffFunction(DISPLAY_WIN1,((FreeSyncFlag)?1:0),800);
    }
   else	
#endif
{
 #if (DP_PBP_RELOAD_EDID||HDMI_PBP_RELOAD_EDID||DVI_PBP_RELOAD_EDID)
 if(HPD_Status
 #if (HDMI_PBP_RELOAD_EDID)		
 ||HDMI_HPD_Status
 #endif	
 #if (DVI_PBP_RELOAD_EDID)		
 ||DVI_HPD_Status
 #endif	
   )				  
{
     hw_ClrBlacklit();
     //mStar_SetupFreeRunMode();
#if 0//Disable_MultiWindow_AutoSwitch
     ForceDelay1ms(300); // delay 1 msec	
#else
     ForceDelay1ms(600); // delay 1 msec	
#endif     
}
 #endif                 
}

    mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);   	
#if Disable_MultiWindow_AutoSwitch
 if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
 {

       USER_PREF_INPUT_TYPE_SUBWIN1=(USER_PREF_INPUT_TYPE+1)%Input_Nums;
       USER_PREF_INPUT_TYPE_SUBWIN2=(USER_PREF_INPUT_TYPE_SUBWIN1+1)%Input_Nums;
    	
     if(USER_PREF_MULTI_WIN_MODE>eMULTIWIN_2_POP1)
       SwitchPort(DISPLAY_WIN3 , USER_PREF_INPUT_TYPE_SUBWIN2);
     if(USER_PREF_MULTI_WIN_MODE>eMULTIWIN_3)
       SwitchPort(DISPLAY_WIN4 , USER_PREF_INPUT_TYPE_SUBWIN3);
       SwitchPort(DISPLAY_WIN2 , USER_PREF_INPUT_TYPE_SUBWIN1);
  }
#endif
    APPWindow_Configure((eMULTIWIN_Type)USER_PREF_MULTI_WIN_MODE, (ePIPSIZE_Type)USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2));


   {
    BYTE uc;
	for(uc = 0;uc<4;uc++)
	{
		g_stMenuFlag.bDisplayInputMsg[uc] = 0;
 	}
   }
}
else
{
#if ENABLE_DP_INPUT||ENABLE_HDMI_INPUT
    if(FreeSyncFlag)
    {
       Menu_Hide();
       Clr_FreeSyncFlag();
	SetFreeSyncOnOffFunction(DISPLAY_WIN1,((FreeSyncFlag)?1:0),800);
    }
#endif

}
#else
#if ENABLE_DP_INPUT||ENABLE_HDMI_INPUT
    if(FreeSyncFlag)
    {
       Menu_Hide();
       Clr_FreeSyncFlag();
	SetFreeSyncOnOffFunction(DISPLAY_WIN1,((FreeSyncFlag)?1:0),800);
    }
#endif

#endif

#if Enable_AutoInput_Menu
    USER_PREF_INPUT_PRIORITY=DEF_INPUT_PRIORITY;
#endif
/*
    if (DEF_MENU_ROTATION)
        SET_MENU_ROTATION_FLAG();
    else
        CLR_MENU_ROTATION_FLAG();
*/
    if (DEF_DDCCI_SETTING==_ON_)
        SET_DDCCI_ENABLE_FLAG();
    else
        CLR_DDCCI_ENABLE_FLAG();
    //DDCID_CTRL();  //Steven120705, DDC ID on/off by input source //XXXXX

    Clr_DoBurninModeFlag();
    CLR_BURNIN_MODE_FLAG();
#if (defined(LianHeChuangXin_Project))	
    CLR_FACTORY_MODE_FLAG();
#endif

#if ENABLE_VGA_INPUT
    ResetPositionSetting();
#endif

/*
    WAIT_V_OUTPUT_BLANKING();
    for(i=0; i<DISPLAY_MAX_NUMS; i++)
    {
        msAPIWinEnableFrameColor(i, FALSE);
    }
*/
    SaveMonitorSetting();

#if DP_PBP_RELOAD_EDID
	  if(HPD_Status==1)
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, TRUE);	
#endif	  

#if HDMI_PBP_RELOAD_EDID
	  if(HDMI_HPD_Status==1)
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	
#endif	  

#if DVI_PBP_RELOAD_EDID
	  if(DVI_HPD_Status==1)
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, TRUE);	
#endif	  

#ifndef SenYo_Project
 if(USER_PREF_LANGUAGE!= DEF_LANGUAGE)
 {
    Menu_Hide();
    USER_PREF_LANGUAGE = DEF_LANGUAGE;
    SetPropFontIndex();	
    SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u8SYS_Language), sizeof(USER_PREF_LANGUAGE));	
 }
 #endif
 
    return TRUE;
}

//=====================================================================
WORD GetColorPresetValue(void)
{
    return CURRENT_PICTURE_MODE;
}
Bool AdjustColorPresetMode(MENU_ITEM_ACTION enAction)
{
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, CT_STANDARD, CT_NUMS-1, 1);
	
    //APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
    //Menu_AdjustColorSetting();
    //SetColorPresetMode();

    SetColorPresetMode();
    return TRUE;
}

Bool SetColorPresetMode(void)
{

   if(CURRENT_PICTURE_MODE==g_u8MenuTempValue)
     return FALSE;
   
    if((g_u8MenuTempValue==CT_GAME) || (g_u8MenuTempValue==CT_MOVIE))
        g_u8DDCCIAControl = 0xDC;
    else
        g_u8DDCCIAControl = 0x14;

    CURRENT_PICTURE_MODE=g_u8MenuTempValue;
	

    USER_PREF_HOT_PICTURE_INDEX=CURRENT_PICTURE_MODE;

{
    BYTE i,j;
    BYTE USER_PREF_WIN_SRC_BK[WIND_CNT];

    for (i=0; i<WIND_CNT; i++)
    {
        USER_PREF_WIN_SRC_BK[i] = USER_PREF_WIN_SRC(i);
    }

   for(i=0;i<PORT_CNT;i++)
    {
        for (j=0; j<WIND_CNT; j++)
        {
            USER_PREF_WIN_SRC(j) = i;
            USER_PREF_PICTURE_MODE(j)            = g_u8MenuTempValue;
        }
     }
    for (i=0; i<WIND_CNT; i++)
    {
        USER_PREF_WIN_SRC(i) = USER_PREF_WIN_SRC_BK[i];
    }
}
	
    //APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
    Menu_AdjustColorSetting();

    return TRUE;
}

Bool AdjustColorRangeValue(MENU_ITEM_ACTION enAction)
{
    if (enAction==MIA_DEC_VALUE) //dec
        g_u8MenuTempValue=(g_u8MenuTempValue+1)%OSD_INPUT_COLOR_RANGE_MAX;
    else if (g_u8MenuTempValue>0) //inc
        g_u8MenuTempValue=(g_u8MenuTempValue-1);
    else
        g_u8MenuTempValue=OSD_INPUT_COLOR_RANGE_MAX-1;
    return TRUE;
}
Bool SetInputColorRangeValue(void)
{
    if (g_u8MenuTempValue==CURRENT_INPUT_COLOR_RANGE)
        return FALSE;

    CURRENT_INPUT_COLOR_RANGE = g_u8MenuTempValue;

    WAIT_V_OUTPUT_BLANKING(); //Jison 090312 use black pattern for temporary image garbage during color format changed
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);

    APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
    WAIT_V_OUTPUT_BLANKING();
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
    return TRUE;
}

WORD GetColorRangeRealValue(void)
{
    MENUFUNC_printData("Current Color Range: %d", CURRENT_INPUT_COLOR_RANGE);
    return CURRENT_INPUT_COLOR_RANGE;
}

WORD GetGammaValue(void)
{
    return CURRENT_GAMMA;
}
Bool AdjustGammaValue(MENU_ITEM_ACTION enAction)
{
//    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, eGAMMA_18, eGAMMA_MAX-1, 1);

    if (enAction==MIA_DEC_VALUE)
        g_u8MenuTempValue=(g_u8MenuTempValue+1)%eGAMMA_MAX;
    else if (g_u8MenuTempValue>0)
        g_u8MenuTempValue=(g_u8MenuTempValue-1);
    else
        g_u8MenuTempValue=eGAMMA_MAX-1;

    Delay1ms(50);

    SetGamma();

    return TRUE;
}

#if Enable_Gamma
#define GAMMA_ENABLE 0x80
#define GAMMA_INDEX_MASK 0x7F
static BYTE ucGammaFunc[DISPLAY_MAX_NUMS] = {GAMMA_INDEX_MASK,GAMMA_INDEX_MASK,GAMMA_INDEX_MASK,GAMMA_INDEX_MASK};
void LoadGamma(BYTE u8DispWin, BYTE * * pu8TableIndex)
{
#if 0//Enable_AutoDownloadGamma
    msAPI_GammaEnable(u8DispWin, _DISABLE);
    msAPI_GammaLoadTbl_1024E_14B(u8DispWin, pu8TableIndex);
    msAPI_GammaEnable(u8DispWin, _ENABLE);
#else
    BYTE u8WinIdx;

    //if(((ucGammaFunc[u8DispWin] & GAMMA_INDEX_MASK) != CURRENT_GAMMA) ||
    //    ((ucGammaFunc[u8DispWin] & GAMMA_ENABLE) != GAMMA_ENABLE))
    {

        //msAPI_GammaEnable(u8DispWin, _DISABLE);
        //fix to disable all gamma for displayed window
        for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
        {
            if(msAPIWinGetScDwEnable(u8WinIdx))
            {
                msAPI_GammaEnable(u8WinIdx, _DISABLE);
            }

        }
        //-----------------------------------------------
        msAPI_GammaLoadTbl_1024E_14B(u8DispWin, pu8TableIndex);

        ucGammaFunc[u8DispWin] = CURRENT_GAMMA;
        ucGammaFunc[u8DispWin] |= GAMMA_ENABLE;
        //msAPI_GammaEnable(u8DispWin, _ENABLE);
        //restore gamma for all displayed window
      if(CURRENT_GAMMA!=eGAMMA_OFF)
      	{
        for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
        {
            if(msAPIWinGetScDwEnable(u8WinIdx) && (ucGammaFunc[u8DispWin] & GAMMA_ENABLE))  //window is enabled and gamma is also enabled
            {
                msAPI_GammaEnable(u8WinIdx, _ENABLE);
            }

        }
       }
        //----------------------------------------
    }
#endif
}

 #if Enable_AutoDownloadGamma
 void TestLoadGamma(BYTE u8DispWin, BYTE GamaIdx)
 {
         CURRENT_GAMMA=GamaIdx;
         MENUFUNC_printData("CURRENT_GAMMA==%d",CURRENT_GAMMA);
         msAPI_GammaEnable(u8DispWin, _DISABLE);
         //LoadGamma(u8DispWin, tAllFullGammaTab[CURRENT_GAMMA]);
	  LoadGammaFunc(u8DispWin); 
         msAPI_GammaEnable(u8DispWin, _ENABLE);
 }
 #endif


void LoadGammaFunc(BYTE u8DispWin)
{
 BYTE u8WinIdx;
 
     #if (defined(FengYuan_Project)||defined(XuLi_Project)||defined(MingCai_Project))
       if(CURRENT_PICTURE_MODE==CT_GAME)
        ;//LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[0]);
	else if(CURRENT_PICTURE_MODE==CT_GAME2)   
        ;//LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[1]);
	else if(CURRENT_PICTURE_MODE==CT_GAME3)
        ;//LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[2]);
       else
     #elif 1//Enable_ECOGameMode
       if(CURRENT_PICTURE_MODE==CT_GAME)
        ;//LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[0]);
	else if(CURRENT_PICTURE_MODE==CT_FPS)   
        ;//LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[1]);
	else if(CURRENT_PICTURE_MODE==CT_RTS)
        ;//LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[2]);
	else if(CURRENT_PICTURE_MODE==CT_MOVIE)
        ;//LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[3]);
       else
     #endif	 
     	{
     	 if(CURRENT_GAMMA==eGAMMA_OFF)
     	 {
             for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
             {
                 if(msAPIWinGetScDwEnable(u8WinIdx))
                 {
                     msAPI_GammaEnable(u8WinIdx, _DISABLE);
                 }
     
             }
     	 }
	 else	 
        LoadGamma(u8DispWin, tAllFullGammaTab[CURRENT_GAMMA]);
     	}

}

 
Bool SetGamma(void)
{
    if (g_u8MenuTempValue==USER_PREF_GAMMA(USER_PREF_WIN_SEL))
        return FALSE;
    CURRENT_GAMMA=g_u8MenuTempValue;

    LoadGammaFunc(USER_PREF_WIN_SEL);

   return TRUE;
}

void LoadDeGamma(BYTE u8DispWin, BYTE ** pu8TableIndex, WORD** pu16SegPntTbl)
{
    msAPI_DeGammaLoadTbl_Compressed(u8DispWin, pu8TableIndex, pu16SegPntTbl);
    msAPI_DeGammaEnable(u8DispWin, _ENABLE);
}

void LoadFixGamma(BYTE u8DispWin, BYTE ** pu8TableIndex)
{
    msAPI_FixGammaLoadTbl_256E_12B(u8DispWin, pu8TableIndex);
    msAPI_FixGammaEnable(u8DispWin, _ENABLE);
}

#endif

Bool SelectGain(void)
{
    if (g_u8MenuTempValue < GAIN_NUMS)
        return TRUE;//Goto Next
   return FALSE;
}
WORD GetSelectGainValue(void)
{
    return 0;
}
Bool AdjustGainVale(MENU_ITEM_ACTION enAction)
{
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, GAIN_R, GAIN_NUMS-1, 1);
    Delay1ms(50);
    return TRUE;
}

#if (Low_Blue_Light_Enable||Enable_HotAdjLowbluray)

#define R_30 	103
#define G_30 	100
#define B_30 	100

#define R_40 	103
#define G_40 	90
#define B_40 	90

#define R_60 	103
#define G_60 	80
#define B_60 	80

#define R_70 	103
#define G_70 	70
#define B_70 	70

Bool AdjustLowbluray(MENU_ITEM_ACTION enAction)
{ 	
    WORD    u16TempValue;
     short    *pu8ColorTable=NULL;
    ColorType CT;

if(enAction!=MIA_SET_VALUE)
{
    u16TempValue = DecIncValueCirculate(enAction, USERPREFLOWBLURAY, MinLowblurayValue, MaxLowblurayValue, 1);

    if (u16TempValue == USERPREFLOWBLURAY)
    {
        return FALSE;
    }

       USERPREFLOWBLURAY=u16TempValue;
}
printData("USERPREFLOWBLURAY==%d",USERPREFLOWBLURAY);

            pu8ColorTable = NULL;//(short *)tStandardColorCorrectionMatrix;
            GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));

              if(msAPIWinColorIsYUV(USER_PREF_WIN_SEL))
              {
                  msAPI_InitialVideoColorMatrix(USER_PREF_WIN_SEL, GetProperHue(CURRENT_HUE),
                  CURRENT_SATURATION,UserPrefContrast_Temp, CT.u8Red, CT.u8Green, CT.u8Blue);
              }
              else
              {
                  //contrast /R/G/B
                  msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CT.u8Red, CT.u8Green, CT.u8Blue);
              }
            g_u8DDCCIAControl = 0x12;



  	return TRUE;
}

WORD GetLowblurayValue_100(void)
{ 
return GetScale100Value(USERPREFLOWBLURAY, 0, MaxLowblurayValue);
}
WORD GetLowblurayValue_Number(void)
{ 
return GetScale100Value(USERPREFLOWBLURAY, 0, MaxLowblurayValue);
}

WORD GetLowblurayValue(void)
{ 
return USERPREFLOWBLURAY;
}

#elif Enable_Lowbluray_FY
Bool SetLowbluray(void)
{ 	
    //WORD    u16TempValue;
     //short    *pu8ColorTable=NULL;
    ColorType CT;
	
    if (g_u8MenuTempValue==USERPREFLOWBLURAY)
        return FALSE;
    USERPREFLOWBLURAY=g_u8MenuTempValue;

            //pu8ColorTable = NULL;//(short *)tStandardColorCorrectionMatrix;
            GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));

	    {
                          if(msAPIWinColorIsYUV(USER_PREF_WIN_SEL))
                          {
                      
                              msAPI_InitialVideoColorMatrix(USER_PREF_WIN_SEL, GetProperHue(CURRENT_HUE),
                              CURRENT_SATURATION,UserPrefContrast_Temp, CT.u8Red, CT.u8Green, CT.u8Blue);
                          }
                          else
                          {
                              //contrast /R/G/B
                              msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CT.u8Red, CT.u8Green, CT.u8Blue);
                          }
                        g_u8DDCCIAControl = 0x12;
	    }
			
  	return TRUE;
}

Bool AdjustLowbluray(MENU_ITEM_ACTION enAction)
{

    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, LOW_BLUE_LIGHT_OFF, LOW_BLUE_LIGHT_MAX-1, 1);
    Delay1ms(50);

}


WORD GetLowblurayValue(void)
{ 
return USERPREFLOWBLURAY;
}

#endif	


#if LED_HOT_Menu_Func
void Osd_DrawHotLedMenu(BYTE DemoFlag);

void SetLedFunc(BYTE LedNumber, BYTE LedColor,BYTE LedStrength,BYTE LedColorMode)
{
	BOOL HandTry1=0;
	BOOL HandTry2=0;
	BOOL HandTry3=0;
	BYTE Retry=0;

      //if(LedStrength>=LedMin)	
       //LedStrength=LedStrength-1;

      //if(LedColor==LedColorful)
       //LedColor=LedColor+1;

	if(LedColorMode==LedBreath)
	LedColorMode=LedFlicker;
	else if(LedColorMode==LedFlicker)
	LedColorMode=LedBreath;
	  
	HandTry1=SentLedCommand(LedNumber, LedColor, LedStrength, LedColorMode);
	Delay1ms(30);
	HandTry2=SentLedCommand(LedNumber, LedColor, LedStrength, LedColorMode);
	Delay1ms(30);
	HandTry3=SentLedCommand(LedNumber, LedColor, LedStrength, LedColorMode);
	Delay1ms(30);
	if(HandTry1&&HandTry2&&HandTry3)
	{
	    return TRUE;

	}
	else
	{
	       for(Retry=0;Retry<10;Retry++)
	       {
	   		if(SentLedCommand(LedNumber, LedColor, LedStrength, LedColorMode)==TRUE)
	   		{
				break;
			}
		   	Delay1ms(100);
		  }
	    	//return FALSE;
	}


}
/*
Bool SetLedColor(void)
{

    if (USER_PREF_LEDColorful_MODE == UserprefLEDColorful)
        return FALSE;
    USER_PREF_LEDColorful_MODE = UserprefLEDColorful;
	
 //  printData("USER_PREF_LEDColorful_MODE==%d",USER_PREF_LEDColorful_MODE);


    g_u8SaveUserData|=SAVE_MONITOR_SETTING; 
    Delay1ms(50);
    return TRUE;
}
*/
Bool AdjustLedColorValue(MENU_ITEM_ACTION enAction)
{
/*
if(USER_PREF_LedModeType==LedNormal||USER_PREF_LedModeType==LedFlicker)
{
    if (enAction==MIA_INC_VALUE)
    	{
    	if(USER_PREF_LEDColorful_MODE>=1)
        USER_PREF_LEDColorful_MODE=(USER_PREF_LEDColorful_MODE-1)%LedColorful;
	else 
        USER_PREF_LEDColorful_MODE=(LedColorful-1);
    	}
    else 
        USER_PREF_LEDColorful_MODE=(USER_PREF_LEDColorful_MODE+1)%LedColorful;
}
else
*/
{
    if (enAction==MIA_INC_VALUE)
    	{
    	if(USER_PREF_LEDColorful_MODE>=1)
        USER_PREF_LEDColorful_MODE=(USER_PREF_LEDColorful_MODE-1)%LedColor_max;
	else 
        USER_PREF_LEDColorful_MODE=(LedColor_max-1);
    	}
    else 
        USER_PREF_LEDColorful_MODE=(USER_PREF_LEDColorful_MODE+1)%LedColor_max;
}

  // printData("USER_PREF_LEDColorful_MODE==%d",USER_PREF_LEDColorful_MODE);
   
if(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)//(LED_FUNC_ON_FLAG)
   SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
   g_u8SaveUserData|=SAVE_MONITOR_SETTING; 
   
    return TRUE;
}
/*
Bool SetLedStrength(void)
{

    if (USER_PREF_LedStrength == USER_PREF_LedStrength)
        return FALSE;
    USER_PREF_LedStrength = USER_PREF_LedStrength;
	
 //  printData("USER_PREF_LedStrength==%d",USER_PREF_LedStrength);


    g_u8SaveUserData|=SAVE_MONITOR_SETTING; 
    Delay1ms(50);
    return TRUE;
}
*/
Bool AdjustLedStrengthValue(MENU_ITEM_ACTION enAction)
{
    if (enAction==MIA_DEC_VALUE)
    	{
    	if(USER_PREF_LedStrength>=1)
        USER_PREF_LedStrength=(USER_PREF_LedStrength-1)%LedStrength_max;
	else 
        USER_PREF_LedStrength=(LedStrength_max-1);
    	}
    else 
        USER_PREF_LedStrength=(USER_PREF_LedStrength+1)%LedStrength_max;

   //    printData("AdjustLedStrengthValue==%d",USER_PREF_LedStrength);
   
if(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)//(LED_FUNC_ON_FLAG)
   SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);

   g_u8SaveUserData|=SAVE_MONITOR_SETTING; 
   
    return TRUE;
}
/*
Bool SetLedMode(void)
{

    if (USER_PREF_LedModeType == USER_PREF_LedModeType)
        return FALSE;
    USER_PREF_LedModeType = USER_PREF_LedModeType;
	
//   printData("USER_PREF_LedModeType==%d",USER_PREF_LedModeType);


    g_u8SaveUserData|=SAVE_MONITOR_SETTING; 
    Delay1ms(50);
    return TRUE;
}
*/
Bool AdjustLedModeValue(MENU_ITEM_ACTION enAction)
{
#if 1

 //if((USER_PREF_LedModeType==LedBreath)&&(USER_PREF_LEDColorful_MODE==LedColorful))
 //   USER_PREF_LEDColorful_MODE=LedRed;
 
 
    if (enAction==MIA_DEC_VALUE)
    	{
    	if(USER_PREF_LedModeType>=1)
        USER_PREF_LedModeType=(USER_PREF_LedModeType-1)%LedMode_Max;
	else 
        USER_PREF_LedModeType=(LedMode_Max-1);
    	}
    else 
        USER_PREF_LedModeType=(USER_PREF_LedModeType+1)%LedMode_Max;

#else
if(USER_PREF_LEDColorful_MODE<LedColorful)
{
    if (enAction==MIA_DEC_VALUE)
    	{
    	if(USER_PREF_LedModeType>=1)
        USER_PREF_LedModeType=(USER_PREF_LedModeType-1)%LedMode_Max;
	else 
        USER_PREF_LedModeType=(LedMode_Max-1);
    	}
    else 
        USER_PREF_LedModeType=(USER_PREF_LedModeType+1)%LedMode_Max;
}
else
{
/*
    if(USER_PREF_LedModeType==LedBreath)
	USER_PREF_LedModeType=LedSport;
    else
	*/
	USER_PREF_LedModeType=LedBreath;

}
#endif

 //      printData("AdjustLedModeValue==%d",USER_PREF_LedModeType);
if(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)//(LED_FUNC_ON_FLAG)
   SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
   g_u8SaveUserData|=SAVE_MONITOR_SETTING; 
   
    return TRUE;
}

Bool SetLedModeCancel(void)
{
#if 0//LED_HOT_Menu_Func
  USER_PREF_LEDColorful_MODE=USER_PREF_LEDColorful_MODE;
  USER_PREF_LedStrength=USER_PREF_LedStrength;
  USER_PREF_LedModeType=USER_PREF_LedModeType;
#endif	
   //CLR_LED_FUNC_ON_FLAG();
   SetLedFunc(0xFF, 0xFF, 0xFF, 0xFF);
   g_u8SaveUserData|=SAVE_MONITOR_SETTING; 
    return TRUE;
}


Bool AdjustLedModeIndexValue(MENU_ITEM_ACTION enAction)
{
	
    if (enAction==MIA_INC_VALUE)
        USER_PREF_LEDMODE_INDEX=(USER_PREF_LEDMODE_INDEX+1)%LedModeIndex_Max;
    else if (USER_PREF_LEDMODE_INDEX>0)
        USER_PREF_LEDMODE_INDEX=(USER_PREF_LEDMODE_INDEX-1);
    else
        USER_PREF_LEDMODE_INDEX=LedModeIndex_Max-1;

   printData("USER_PREF_LEDMODE_INDEX==%d",USER_PREF_LEDMODE_INDEX);


   if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Off)
   {
      //CLR_LED_FUNC_ON_FLAG();
      SetLedFunc(0xFF, 0xFF, 0xFF, 0xFF);
      ForceDelay1ms(60);
      Clr_LED_Power();	  
      Clr_LEDActiveOnFlag();		  
   }
   else
   {

   if(!IS_LED_Power())
    {
      Set_LED_Power();

       if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)
          ForceDelay1ms(53);//(49);
       else//LedModeIndex_User
       {
	if((USER_PREF_LedModeType==LedBreath)	)  
          ForceDelay1ms(53);
       else    
          ForceDelay1ms(55);//55//53
       }
   }
      //SET_LED_FUNC_ON_FLAG();
      Set_LEDActiveOnFlag();	
    if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)
      SetLedFunc(LED_Number, LedColorful, LedMax, LedBreath);
     else	
      SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
   }

    return TRUE;
}



#endif

Bool SelectHueSaturation(void)
{
    if (g_u8MenuTempValue < 2)
        return TRUE;//Goto Next
   return FALSE;
}
WORD GetSelectHueSaturationValue(void)
{
    return 0;
}


Bool SetColorTemperature(void)
{
    if (g_u8MenuTempValue==CURRENT_COLOR_TEMP)
        return FALSE;
    CURRENT_COLOR_TEMP=g_u8MenuTempValue;

    Menu_AdjustColorSetting();
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;
   return TRUE;
}
WORD GetColorTemperatureValue(void)
{
    return CURRENT_COLOR_TEMP;
}
Bool AdjustColorTemperature(MENU_ITEM_ACTION enAction)
{
#if 0
#if Enable_ColorTemp_5700K
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, PCT_5700K, PCT_NUMS-1, 1);
#else
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, PCT_6500K, PCT_NUMS-1, 1);
#endif
#else
    if (enAction==MIA_DEC_VALUE)
        g_u8MenuTempValue=(g_u8MenuTempValue+1)%PCT_NUMS;
    else if (g_u8MenuTempValue>0)
        g_u8MenuTempValue=(g_u8MenuTempValue-1)%PCT_NUMS;
    else
        g_u8MenuTempValue=PCT_NUMS-1;
#endif

    Delay1ms(50);
	SetColorTemperature();

    return TRUE;
}
BYTE GetPCT_Tempareture(PresetColorTempType PCT)
{

#if defined(KTC_Project)
    switch(PCT)
    {
        case PCT_5000K:
            return eCT_5000K;
        case PCT_5700K:
	      return eCT_6500K;	
	//	return eCT_5700K;	  
        case PCT_6500K:
		return eCT_5700K;
          //  return eCT_6500K;
        case PCT_7500K:
            return eCT_7500K;
        case PCT_9300K:
            return eCT_9300K;
         case PCT_USER:
            return eCT_USER;
        case PCT_10000K:
            return eCT_10000K;
           break;
        default:
            return eCT_6500K;
            break;
    }

#else
    switch(PCT)
    {
        case PCT_5000K:
            return eCT_5000K;
        case PCT_5700K:
            return eCT_5700K;
        case PCT_6500K:
            return eCT_6500K;
        case PCT_7500K:
            return eCT_7500K;
        case PCT_9300K:
            return eCT_9300K;
         case PCT_USER:
            return eCT_USER;
        case PCT_10000K:
            return eCT_10000K;
           break;
        default:
            return eCT_6500K;
            break;
    }

#endif	
}

#if (ENABLE_MWE)
Bool MWE_InitColorSetting(void)
{
    BYTE u8TempInputSrc;
    if(MWE_FUNC_ENABLE_FLAG)
    {
        msAPI_MWEActive(USER_PREF_WIN_SEL, TRUE);
        u8TempInputSrc = USER_PREF_INPUT_TYPE;
        USER_PREF_INPUT_TYPE = 7;
        Menu_AdjustColorSetting();

        msAPI_MWEActive(USER_PREF_WIN_SEL, FALSE);
        USER_PREF_INPUT_TYPE = u8TempInputSrc;
        Menu_AdjustColorSetting();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif


Bool Menu_AdjustColorSetting(void)
{
    short    *pu8ColorTable=NULL;
    ColorType CT;
    BYTE i;
#if (defined(W315D_Project))
    BYTE u8WinIdx=0;	
#endif

    switch(CURRENT_PICTURE_MODE)
    {
        case CT_STANDARD:
            #if ENABLE_3DLUT
            mStar_Setup3DLUT( USER_PREF_WIN_SEL,0 , DISABLE);
            #endif
            pu8ColorTable = NULL;//(short *)tStandardColorCorrectionMatrix;
            GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));
	     UserPrefBrightness_Temp=80;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	50;//CURRENT_CONTRAST;
		 if(DCR_FLAG_ENABLE)
		 {
			SET_DCR_ENABLE_FLAG();
			Set_AdjDcrBacklightFlag(); 
		 }
		 else
		 {
		 	CLR_DCR_ENABLE_FLAG();
		 	Clr_AdjDcrBacklightFlag(); 
		 }
		 UserPrefNoiseReduction = SUPER_RESOLUTION_OFF;
            break;

		case CT_USER:
		#if ENABLE_3DLUT
		mStar_Setup3DLUT( USER_PREF_WIN_SEL,0 , DISABLE);
		#endif
		pu8ColorTable = NULL;//(short *)tStandardColorCorrectionMatrix;
		GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));
		 UserPrefBrightness_Temp=USER_PREF_BRIGHTNESS_USER;		
		 UserPrefContrast_Temp= CURRENT_CONTRAST_USER;
		 if(DCR_FLAG_ENABLE)
		 {
			SET_DCR_ENABLE_FLAG();
			Set_AdjDcrBacklightFlag(); 
		 }
		 else
		 {
		 	CLR_DCR_ENABLE_FLAG();
		 	Clr_AdjDcrBacklightFlag(); 
		 }
		 UserPrefNoiseReduction = CURRENT_SUPER_RESOLUTION;
		 DCRBacklight = UserPrefBrightness_Temp;
	   break;

        case CT_VIVID:
            #if ENABLE_3DLUT
            mStar_Setup3DLUT( USER_PREF_WIN_SEL,0 , DISABLE);
            #endif
            pu8ColorTable = NULL;//((short *)tVivdColorCorrectionMatrix;
            GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));
	#if (defined(FengYuan_Project))		
 	     UserPrefBrightness_Temp=USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	55;
	#elif (defined(SenYo_Project))	 
	     UserPrefBrightness_Temp=80;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	DEF_CONTRAST;//CURRENT_CONTRAST;
	#elif (defined(LianHeChuangXin_Project))
	     UserPrefBrightness_Temp=30;//80;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	50;//CURRENT_CONTRAST;
	#elif(defined(KTC_Project))
	     UserPrefBrightness_Temp=85;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	50;//CURRENT_CONTRAST;
	#else
	     UserPrefBrightness_Temp=80;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	60;//CURRENT_CONTRAST;
	#endif


		CLR_DCR_ENABLE_FLAG();
		Clr_AdjDcrBacklightFlag(); 
		UserPrefNoiseReduction = SUPER_RESOLUTION_OFF;
          break;

        case CT_GAME:
            #if ENABLE_3DLUT
            mStar_Setup3DLUT( USER_PREF_WIN_SEL, 2 , DISABLE);
            #endif
            pu8ColorTable = NULL;//((short *)tGameMatrix;
            GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));
	#if (defined(LianHeChuangXin_Project))		
	     UserPrefBrightness_Temp=80;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	60;//CURRENT_CONTRAST;
	#else
	     UserPrefBrightness_Temp=90;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	50;//CURRENT_CONTRAST;
	#endif
		CLR_DCR_ENABLE_FLAG();
		Clr_AdjDcrBacklightFlag(); 
		UserPrefNoiseReduction = SUPER_RESOLUTION_LOW;
            break;
			
        case CT_MOVIE:
            #if ENABLE_3DLUT
            mStar_Setup3DLUT( USER_PREF_WIN_SEL, 1 , DISABLE);
            #endif
            pu8ColorTable = NULL;//((short *)tMovieColorCorrectionMatrix;
            GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));
	#if (defined(LianHeChuangXin_Project))
	     UserPrefBrightness_Temp=90;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	80;//CURRENT_CONTRAST;	     
	#elif (defined(SenYo_Project))
	     UserPrefBrightness_Temp=100;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	DEF_CONTRAST;//CURRENT_CONTRAST;
	#elif(defined(KTC_Project))
	     UserPrefBrightness_Temp=100;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	50;//CURRENT_CONTRAST;	     
	#else
	     UserPrefBrightness_Temp=100;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	60;//CURRENT_CONTRAST;
	#endif     
	
		CLR_DCR_ENABLE_FLAG();
		Clr_AdjDcrBacklightFlag(); 
		UserPrefNoiseReduction = SUPER_RESOLUTION_OFF;
            break;
#if 1//Enable_ECOGameMode			
        case CT_FPS:
            #if ENABLE_3DLUT
            mStar_Setup3DLUT( USER_PREF_WIN_SEL, 3 , DISABLE);
            #endif
            pu8ColorTable = NULL;//((short *)tGameMatrix;
            GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));
	#if (defined(LianHeChuangXin_Project))		
	     UserPrefBrightness_Temp=90;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	60;//CURRENT_CONTRAST;
	#else
	     UserPrefBrightness_Temp=90;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	70;//CURRENT_CONTRAST;
	#endif
	
		CLR_DCR_ENABLE_FLAG();
		Clr_AdjDcrBacklightFlag(); 
		UserPrefNoiseReduction = SUPER_RESOLUTION_LOW;
            break;

        case CT_RTS:
            #if ENABLE_3DLUT
            mStar_Setup3DLUT( USER_PREF_WIN_SEL,  2, DISABLE);
            #endif
            pu8ColorTable = NULL;//((short *)tGameMatrix;
            GetColorTempRGB(&CT, GetPCT_Tempareture((PresetColorTempType)CURRENT_COLOR_TEMP));
	#if (defined(LianHeChuangXin_Project))		
	     UserPrefBrightness_Temp=90;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	60;//CURRENT_CONTRAST;
	#else
	     UserPrefBrightness_Temp=85;//USER_PREF_BRIGHTNESS;		
	     UserPrefContrast_Temp=	65;//CURRENT_CONTRAST;
	#endif     
	
		CLR_DCR_ENABLE_FLAG();
		Clr_AdjDcrBacklightFlag(); 
		UserPrefNoiseReduction = SUPER_RESOLUTION_HIGHEST;
            break;
#endif			
			
        //----------------------------------no use
    }

    for( i = 0; i < MAX_DISPALY_WINDOWS; ++ i )
        msACESetColorCorrectionTable(i,pu8ColorTable );

    WAIT_V_OUTPUT_BLANKING_START(); //Jison 081230

    //Set Input Ypbpr Format for In Only Scaler
    msAPISrcSetCurColorSpace(USER_PREF_WIN_SEL, msAPISrcGetCurColorSpace(USER_PREF_WIN_SEL));

    //Set CSC
    msAPI_SetInputCSC(USER_PREF_WIN_SEL, msAPIWinColorIsYUV(USER_PREF_WIN_SEL));
/*
    if(msAPIWinColorIsYUV(USER_PREF_WIN_SEL))
    {
        msAPI_InitialVideoColorMatrix(USER_PREF_WIN_SEL, GetProperHue(CURRENT_HUE),
        CURRENT_SATURATION,CURRENT_CONTRAST, CT.u8Red, CT.u8Green, CT.u8Blue);
    }
    else
*/		
    {
        //contrast /R/G/B	
        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CT.u8Red, CT.u8Green, CT.u8Blue);
    }

    // Adjust Hue in VOP
    msAPI_CustomHueEnable(USER_PREF_WIN_SEL , ENABLE);

    // Adjust Saturation in VIP
    msAPI_CustomSaturationEnable(USER_PREF_WIN_SEL , ENABLE);
    


    Set_Hue_Setting();
    Set_Saturation_Setting();
	

    //Black Level (Offset)
 {
 #if (defined(SenYo_Project))
    WORD tempHeight=0, tempWidth=0;
     BYTE ucInputPort = MapWin2Port(DISPLAY_WIN1);
     tempWidth = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN1)));
     tempHeight = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN1)));
	 
  if(((tempWidth==1920)&&(tempHeight==1080))&&(INPUT_IS_HDMI(ucInputPort)))
     msAPI_AdjustBlacklevel(USER_PREF_WIN_SEL , 42, 42, 42);
 else
#endif 	
    msAPI_AdjustBlacklevel(USER_PREF_WIN_SEL , CURRENT_RED_BLACK_LEVEL, CURRENT_GREEN_BLACK_LEVEL, CURRENT_BLUE_BLACK_LEVEL);
  }

//Brightness
#if ENABLE_DCR
    MF_DCRCheckBrightness();
#else //080905
    msAPI_AdjustBrightness(UserPrefBrightness_Temp);
#endif
    msAPI_AdjustContrast(USER_PREF_WIN_SEL, UserPrefContrast_Temp);

#if Enable_Gamma
    //Load Gamma
    if (g_stMenuFlag.bDoNotLoadGamme) //080905
    {
        g_stMenuFlag.bDoNotLoadGamme=0;
        msAPI_GammaEnable(USER_PREF_WIN_SEL, _DISABLE);
    ucGammaFunc[USER_PREF_WIN_SEL] = GAMMA_INDEX_MASK;
    }
#if (defined(W315D_Project))
    else if(CURRENT_COLOR_TEMP==PCT_5700K)
    {

      msLoadDeltaEColorMode(0);//sRGB

        for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
        {
            if(msAPIWinGetScDwEnable(u8WinIdx))
            {
                msAPI_GammaEnable(u8WinIdx, _ENABLE);	  
                msAPI_DeGammaEnable(u8WinIdx, _ENABLE);
                msAPI_FixGammaEnable(u8WinIdx, _ENABLE);
                msAPI_ColorMatrixEnable(u8WinIdx, _ENABLE, 0);
            }
        }

      //	_WinSelIDRestore();
    }
#endif
    else
    {
#if (defined(W315D_Project))
        for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
        {
            if(msAPIWinGetScDwEnable(u8WinIdx))
            {
                msAPI_GammaEnable(u8WinIdx, _DISABLE);	  
                msAPI_DeGammaEnable(u8WinIdx, _DISABLE);
                msAPI_FixGammaEnable(u8WinIdx, _DISABLE);
                msAPI_ColorMatrixEnable(u8WinIdx, _DISABLE, 0);
            }
        }
#endif

     #if (defined(FengYuan_Project)||defined(XuLi_Project)||defined(MingCai_Project))
       if(CURRENT_PICTURE_MODE==CT_GAME)
        LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[0]);
	else if(CURRENT_PICTURE_MODE==CT_GAME2)   
        LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[1]);
	else if(CURRENT_PICTURE_MODE==CT_GAME3)
        LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[2]);
       else
     #elif 1//Enable_ECOGameMode
       if(CURRENT_PICTURE_MODE==CT_GAME)
        LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[0]);
	else if(CURRENT_PICTURE_MODE==CT_FPS)   
        LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[1]);
	else if(CURRENT_PICTURE_MODE==CT_RTS)
        LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[2]);
	else if(CURRENT_PICTURE_MODE==CT_MOVIE)
        LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab_Game[3]);
       else
     #endif	 
	 LoadGammaFunc(USER_PREF_WIN_SEL);
        //LoadGamma(USER_PREF_WIN_SEL, tAllFullGammaTab[CURRENT_GAMMA]);
    }
#endif

    SetSharpness();

#if ENABLE_SUPER_RESOLUTION  //Steven120509
    //SetSuperResolution();
    msPQ_SetGRule_SR(USER_PREF_WIN_SEL, CURRENT_SUPER_RESOLUTION);
#endif

#if ENABLE_RTE
    mStar_SetRTE();
#endif

#if ENABLE_VGA_INPUT
    if(INPUT_IS_VGA(MapWin2Port(USER_PREF_WIN_SEL)))
    {
        msAPI_AdjustVGASourceType(USER_PREF_WIN_SEL);
    }
#endif


    return FALSE;

}

#if ENABLE_WIDE_MODE
void SetWideMode(void)
{
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);  
    Set_OSDActionFlag();	
    WAIT_V_OUTPUT_BLANKING();
    msAPIWinReSetup(USER_PREF_WIN_SEL, TRUE, TRUE); // if window is not in the center, scaler removed or new added is required due to asp change. In this case we need ReDispatch
    BlackScreenCounter=30;
    WAIT_V_OUTPUT_BLANKING();
    //ForceDelay1ms(500);
    //mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);   
}
Bool Menu_SetWideMode(void)
{
    //if (g_u8OriginalSetting == USER_PREF_WIDE_MODE(SrcInputPortM))
    //    return FALSE;
    // g_u8OriginalSetting=USER_PREF_WIDE_MODE(SrcInputPortM) ;
    //Jison: Add Set Wide Mode function here...

    SetWideMode();
    //ResetMenuPosition();

   return TRUE;
}
WORD GetWideModeValue(void)
{
    return USER_PREF_WIDE_MODE(SrcInputPortM);
}
Bool AdjustWideModeValue(MENU_ITEM_ACTION enAction)
{

    if (enAction==MIA_DEC_VALUE)
        USER_PREF_WIDE_MODE(SrcInputPortM)=(USER_PREF_WIDE_MODE(SrcInputPortM)+1)%eASPECT_MAX;
    else if (USER_PREF_WIDE_MODE(SrcInputPortM)>0)
        USER_PREF_WIDE_MODE(SrcInputPortM)=(USER_PREF_WIDE_MODE(SrcInputPortM)-1)%eASPECT_MAX;
    else
        USER_PREF_WIDE_MODE(SrcInputPortM)=eASPECT_MAX-1;
	
printData("g_u8OriginalSetting==%d",g_u8OriginalSetting);
printData("USER_PREF_WIDE_MODE(SrcInputPortM)==%d",USER_PREF_WIDE_MODE(SrcInputPortM));
       Menu_SetWideMode();
	   
    return TRUE;
}
#endif

#if (AudioFunc)

void SetAudioMuteOnOff( void )
{
printData("4444444444444SetAudioMuteOnOff==%x",AUDIO_MUTE_FLAG);

     //if(AUDIOMUTE_FLAG)
     if(AUDIO_MUTE_FLAG)
     {
        hw_Clr_SPIFGPIO_EN_Pin();
        hw_SetMute();
	#if ENABLE_DAC
        msAPI_AudioSetEarphoneMute(TRUE);
        msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, TRUE);
	#endif
     }
     else
     {
       if(hw_HP_DET_Pin)
       {
            CLR_EARPHONE_EN_FLAG();
       }  
       else
            SET_EARPHONE_EN_FLAG();

#if ((MS_BOARD_TYPE_SEL == BD_MST9UHL_A0))
     if(CURRENT_INPUT_IS_ANALOG1()
   #if ENABLE_DVI_INPUT
	 ||CURRENT_INPUT_IS_DIGITAL_DVI()
	#if ENABLE_DVI2ND_INPUT 
	 ||CURRENT_INPUT_IS_DIGITAL_DVI2ND()
	#endif 
 	#if ENABLE_DVI3ND_INPUT 
	 ||CURRENT_INPUT_IS_DIGITAL_DVI3ND()
	#endif 
   #endif
	 )
   	{
   	  #if ENABLE_ADJ_AUDIO_OUTPUT
              USER_PREF_AUDIO_OUTPUTTYPE=Speaker;
	  #elif (!Audio_Input_Jack_Only_For_Line_In)
	       CLR_EARPHONE_EN_FLAG();
              msAPI_AdjustVolume(0);
              hw_SetMute();		   
              printMsg("Speaker");
              //hw_Set_HP_MUTE();	
              //hw_Clr_SPIFGPIO_EN_Pin();
		msAPI_AudioSetEarphoneMute(TRUE);	   

              hw_ClrMute();		
              msAPI_AdjustVolume(USER_PREF_VOLUME);	  
	 #endif
   	}
       else
       {

   	 #if ENABLE_ADJ_AUDIO_OUTPUT
           if(hw_HP_DET_Pin)
           {
           ;
           }
	    else
            USER_PREF_AUDIO_OUTPUTTYPE=Speaker;
	#endif
	 }
#endif	   

#if ENABLE_ADJ_AUDIO_OUTPUT
              OSD_SetAudioOutput();
#elif (Audio_Input_Jack_Only_For_Line_In)
	       CLR_EARPHONE_EN_FLAG();
              msAPI_AdjustVolume(0);
              //hw_SetMute();		   
              printMsg("Speaker");
              //hw_Set_HP_MUTE();	
              //hw_Clr_SPIFGPIO_EN_Pin();
		msAPI_AudioSetEarphoneMute(TRUE);	   

              hw_ClrMute();		
              msAPI_AdjustVolume(USER_PREF_VOLUME);	  
#elif Audio_Input_Jack_Only_For_Output
              SET_EARPHONE_EN_FLAG();
              msAPI_AdjustVolume(0);
              hw_SetMute();
              printMsg("EarPhone");
              //hw_Clr_HP_MUTE();
              //hw_Clr_SPIFGPIO_EN_Pin();
              msAPI_AudioSetEarphoneMute(FALSE);	   
	  #if 0//(defined(LianHeChuangXin_Project))		  
  		msWrite2ByteMask(REG_112CE8, 0x0000, 0x0001);
	  #endif
              msAPI_AdjustVolume(USER_PREF_VOLUME);
#else
              EarphoneHandler();
#endif

#if (!ENABLE_ADJ_AUDIO_OUTPUT)
        hw_Set_SPIFGPIO_EN_Pin();
#endif
	#if ENABLE_DAC
        //msAPI_AudioSetEarphoneMute(TRUE);
        if(USER_PREF_VOLUME!=0) 
        msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, FALSE);
	#endif
     }
}
Bool OSD_SetAudioMuteOnOff(MENU_ITEM_ACTION enAction)
{
    if( AUDIO_MUTE_FLAG)
    {
        CLR_AUDIO_MUTE_FLAG();
    }
    else
    {
        SET_AUDIO_MUTE_FLAG();
    }

    MENUFUNC_printData("\r\n =================AUDIO_MUTE_FLAG=%x",AUDIO_MUTE_FLAG);

    SetAudioMuteOnOff();
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;
    return TRUE;
}

WORD GetMuteOnOffValue(void)
{
    return (AUDIO_MUTE_FLAG?_ON_:_OFF_);
}
#endif

#if ENABLE_AUDIO_SETTINGS_MENU
Bool ResetAudioSettings(void)
{
    //XXXXX


    SaveMonitorSetting();
    return TRUE;
}
#endif

#if Free_Sync_Enable
Bool OSD_SetFreeSyncOnOff( void )
{

     #if SET_BLIT_WITH_OSDFreeSyncSwitchAction
       bOSDFreeSyncOnOffFlag=TRUE;
     #endif

 #if ENABLE_DP_INPUT||ENABLE_HDMI_INPUT
    //SetDRRFunction(DISPLAY_WIN1, ((FreeSyncFlag)?1:0), 800);
    SetFreeSyncOnOffFunction(DISPLAY_WIN1,((FreeSyncFlag)?1:0),800); // only for single window (win0)
 #endif
	
    SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));	  	
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;
    return TRUE;
}

WORD GetFreeSyncOnOffValue(void)
{
    return (FreeSyncFlag?_ON_:_OFF_);
}

Bool AdjustFreeSync(MENU_ITEM_ACTION enAction)
{
     enAction=enAction;
  if(FreeSyncFlag)
   Clr_FreeSyncFlag();
  else
   Set_FreeSyncFlag();	
  
   OSD_SetFreeSyncOnOff();	

   Set_OSDFreeSyncActionFlag();

   
    return TRUE;
}


#endif



#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
Bool Menu_SetMultiWindowModes(void)
{
#if DP_PBP_RELOAD_EDID	
  BYTE HPD_Status=0;
#endif
#if HDMI_PBP_RELOAD_EDID
  BYTE HDMI_HPD_Status=0;
#endif
#if DVI_PBP_RELOAD_EDID
  BYTE DVI_HPD_Status=0;
#endif


    if (g_u8MenuTempValue == USER_PREF_MULTI_WIN_MODE)
        return FALSE;

    USER_PREF_MULTI_WIN_MODE = g_u8MenuTempValue;

	
    Set_HDMI1HPDFlag();   
    Set_HDMI2HPDFlag();   
    Set_HDMI3HPDFlag();   

    printData("USER_PREF_MULTI_WIN_MODE 2222==%d",USER_PREF_MULTI_WIN_MODE);
#if DP_PBP_RELOAD_EDID
#if Disable_4K60HZ_Timing
  if(((DP_EDID_3840x2160_Flag)&&(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF))
     ||((!DP_EDID_3840x2160_Flag)&&(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)) )
#else
  if(((DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))))
  ||((!DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DISPLAYPORT))))
  )
#endif
   {
                   #if Free_Sync_Enable                      
                        if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
                             Clr_FreeSyncFlag();
		     #endif								 
                  //mhal_DP_HPDControl(4, FALSE);
                  msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, FALSE);	 	
		    HPD_Status=1;

                  EDID_ReLoad_Func(0);
                  msAPI_combo_LoadEDID_Func();
    		    printMsg("DP_PBP_RELOAD_EDID 1111111111");	  
                  //ForceDelay1ms(600); // delay 1 msec				
   }
#endif

#if HDMI_PBP_RELOAD_EDID
#if Disable_4K60HZ_Timing
  if(((HDMI_EDID_3840x2160_Flag)&&(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF))
     ||((!HDMI_EDID_3840x2160_Flag)&&(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)) )
#else
 if(((HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))))
 ||((!HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))))
 ||((HDMI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI))))
 ||((HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_HDMI))))
 )
 #endif
  {
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	 		 
        HDMI_HPD_Status=1;	
		
        ReLoadHDMI_EDID();
        printMsg("HDMI_PBP_RELOAD_EDID 555555");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif

#if DVI_PBP_RELOAD_EDID
 if(((DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))))
 ||((!DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))))
 ||((DVI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DVI))))
 ||((!DVI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DVI))))
 ||((DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DVI))))
 ||((!DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DVI))))
 )
  {
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, FALSE);	 		 
        DVI_HPD_Status=1;	
		
        ReLoadDVI_EDID();
        printMsg("DVI_PBP_RELOAD_EDID 555555");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif



#if AudioFunc
   if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetModeChangeAudioMuteOnOff]!=NULL)
   ((fpCustomerSetModeChangeAudioMuteOnOff)CB_FuncList[eCB_CustomerSetModeChangeAudioMuteOnOff])();
   else
   {
      audio_EnableAudioAfterSignalLock_Flag=AUDIO_MUTE;			 
      msAPI_AdjustVolume(0);
      Delay1ms(100);
      hw_SetMute();	
      hw_Clr_SPIFGPIO_EN_Pin();	  
      #if ENABLE_DAC
      msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, TRUE);
      msAPI_AudioSetEarphoneMute(TRUE);
      msAPI_AudioSetMute( AUDIO_SOFTWARE_MUTE, E_SW_TIMING_CHANGE_MUTE_ON);
      #endif
      #if ENABLE_DAC
      msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
      #endif
   }
#endif

#if Free_Sync_Enable
   if((FreeSyncFlag)&&(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF))
  {
     Clr_FreeSyncFlag();
     SetFreeSyncOnOffFunction(DISPLAY_WIN1,((FreeSyncFlag)?1:0),800);
  }  
  else 
#endif			
{
#if (DP_PBP_RELOAD_EDID||HDMI_PBP_RELOAD_EDID||DVI_PBP_RELOAD_EDID)
if(HPD_Status
#if (HDMI_PBP_RELOAD_EDID)		
||HDMI_HPD_Status
#endif	
#if (DVI_PBP_RELOAD_EDID)		
||DVI_HPD_Status
#endif	
)		
{
     hw_ClrBlacklit();
     //mStar_SetupFreeRunMode();
#if 0//Disable_MultiWindow_AutoSwitch	 
     ForceDelay1ms(300); // delay 1 msec		
#else
     ForceDelay1ms(600); // delay 1 msec		
#endif
}
#endif                 
}

    mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);   
#if Enable_Menu_Panel_Rotate_180
   if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
   {
    USER_PREF_ROTATE_INDEX=Rotate_Normal;
   }
#endif

#if Disable_MultiWindow_AutoSwitch
 if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
 {

       USER_PREF_INPUT_TYPE_SUBWIN1=(USER_PREF_INPUT_TYPE+1)%Input_Nums;
       USER_PREF_INPUT_TYPE_SUBWIN2=(USER_PREF_INPUT_TYPE_SUBWIN1+1)%Input_Nums;
    	
     if(USER_PREF_MULTI_WIN_MODE>eMULTIWIN_2_POP1)
       SwitchPort(DISPLAY_WIN3 , USER_PREF_INPUT_TYPE_SUBWIN2);
     if(USER_PREF_MULTI_WIN_MODE>eMULTIWIN_3)
       SwitchPort(DISPLAY_WIN4 , USER_PREF_INPUT_TYPE_SUBWIN3);
       SwitchPort(DISPLAY_WIN2 , USER_PREF_INPUT_TYPE_SUBWIN1);
  }
#endif

    SetPIPPositionParameter();
    APPWindow_Configure((eMULTIWIN_Type)USER_PREF_MULTI_WIN_MODE, (ePIPSIZE_Type)USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2));
    Set_PIP_Border();

  if(DCR_ENABLE_FLAG)
  {
      CLR_DCR_ENABLE_FLAG();
      SetDCRFunction();	 		   
  }


    if(USER_PREF_MULTI_WIN_MODE > eMULTIWIN_OFF)
    {
         g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN2]=1;
         g_stMenuFlag.bDisplayInputNoSignalMsg[DISPLAY_WIN2]=1;
       
          if(USER_PREF_MULTI_WIN_MODE > eMULTIWIN_2_POP1)
          {
             g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN3]=1;
             g_stMenuFlag.bDisplayInputNoSignalMsg[DISPLAY_WIN3]=1;
             if(USER_PREF_MULTI_WIN_MODE > eMULTIWIN_3)
             {
               g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN4]=1;
               g_stMenuFlag.bDisplayInputNoSignalMsg[DISPLAY_WIN4]=1;
             }
          }
    }
    g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;
    
    g_stMenuFlag.bDisplayInputNoSignalMsg[DISPLAY_WIN1]=1;
    
    //g_stMenuFlag.bAnyWinDisplayChange=1;

    Set_MultiWin_InputTimingChangeFlag();

{
    BYTE xdata u8IdxWin= 0;		
	
     for(u8IdxWin=0 ; u8IdxWin<4 ; u8IdxWin++)
     {	  
            Clr_InputTimingStableFlag(MapWin2Port(u8IdxWin));
	     SrcFlags(MapWin2Port(u8IdxWin)) |= SyncLoss;			 
     }
	 Clr_InputTimingStable_NormalFlag();		
}
    g_u8SwitchPortCounter = 0;

#if DP_PBP_RELOAD_EDID
	  if(HPD_Status==1)
             //mhal_DP_HPDControl(4, TRUE);
	      msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, TRUE);	 	
#endif	  
#if HDMI_PBP_RELOAD_EDID
	  if(HDMI_HPD_Status==1)
              msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	 	
#endif	  
#if DVI_PBP_RELOAD_EDID
	  if(DVI_HPD_Status==1)
              msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, TRUE);	 	
#endif	  

	
   return TRUE;
}

WORD GetMultiWindowModesValue(void)
{
    return USER_PREF_MULTI_WIN_MODE;
}

Bool AdjustMultiWindowModesValue(MENU_ITEM_ACTION enAction)
{
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, eMULTIWIN_OFF, eMULTIWIN_MAX-1, 1);
    return TRUE;
}

/*
Bool Menu_SetMultiWindowSelect(void)
{
    if (g_u8MenuTempValue==USER_PREF_WIN_SEL)
        return FALSE;
    USER_PREF_WIN_SEL=g_u8MenuTempValue;

   return TRUE;
}

WORD GetWindowSelectValue(void)
{
    return USER_PREF_WIN_SEL;
}

Bool AdjustMultiWindowSelectValue(MENU_ITEM_ACTION enAction)
{
    BYTE u8MultiWinSelItemNUMs;

    if (USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)
        u8MultiWinSelItemNUMs=DISPLAY_MAX_NUMS-1;
    else if (USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)
        u8MultiWinSelItemNUMs=(DISPLAY_MAX_NUMS-2);
    else //PIP && PBP
        u8MultiWinSelItemNUMs=(DISPLAY_MAX_NUMS-3);

    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, DISPLAY_WIN1, u8MultiWinSelItemNUMs, 1);
    return TRUE;
}
*/
//=================================================================
//=================================================================

//------------------------------------------------------------------------------------------
BYTE Get_SwitchSourceIndex(BYTE InputType)
{    
#if (MS_BOARD_TYPE_SEL ==  BD_MST9UHL_A0)
      if(InputType==INPUT_VGA)
         return 0;
      else if(InputType==INPUT_DVI)
         return 1;
      else if(InputType==INPUT_HDMI)
         return 2;
      else if(InputType==INPUT_DISPLAYPORT)
         return 3;
      else
         return 0;
#elif (MS_BOARD_TYPE_SEL ==  BD_MST9UHL_V3)
   #if (defined(FengYuan_Project)||defined(XuLi_Project)||defined(MingCai_Project))
      if(InputType==INPUT_DISPLAYPORT)
         return 0;
      else if(InputType==INPUT_HDMI)
         return 1;
      else if(InputType==INPUT_HDMI2ND)
         return 2;
      else if(InputType==INPUT_HDMI3ND)
         return 3;
      else if(InputType==INPUT_VGA)
         return 4;
      else
         return 0;
   #else
      if(InputType==INPUT_VGA)
         return 0;
      else if(InputType==INPUT_DISPLAYPORT)
         return 1;
      else if(InputType==INPUT_HDMI)
         return 2;
      else if(InputType==INPUT_HDMI2ND)
         return 3;
      else if(InputType==INPUT_HDMI3ND)
         return 4;
      else
         return 0;
   #endif	  
#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)
      if(InputType==INPUT_DVI)
         return 0;
      else if(InputType==INPUT_HDMI)
         return 1;
      else if(InputType==INPUT_DISPLAYPORT)
         return 2;
      else
         return 0;
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)	  
      if(InputType==INPUT_HDMI)
         return 0;
      else if(InputType==INPUT_DISPLAYPORT)
         return 1;
      else
         return 0;
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1)
       if(InputType==INPUT_HDMI)
         return 0;
      else if(InputType==INPUT_HDMI2ND)
         return 1;
      else if(InputType==INPUT_HDMI3ND)
         return 2;
      else if(InputType==INPUT_DISPLAYPORT)
         return 3;
      else
         return 0;
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
       if(InputType==INPUT_DVI)
         return 0;
      else if(InputType==INPUT_HDMI)
         return 1;
      else if(InputType==INPUT_HDMI2ND)
         return 2;
      else if(InputType==INPUT_DISPLAYPORT)
         return 3;
      else
         return 0;	  	
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
      if(InputType==INPUT_HDMI)
         return 0;
      else if(InputType==INPUT_HDMI2ND)
         return 1;
      else if(InputType==INPUT_DISPLAYPORT)
         return 2;
      else
         return 0;	  	
	  
	  
#else//BD_MST9UHL_A0 
      if(InputType==INPUT_VGA)
         return 0;
      else if(InputType==INPUT_DVI)
         return 1;
      else if(InputType==INPUT_HDMI)
         return 2;
      else if(InputType==INPUT_DISPLAYPORT)
         return 3;
      else
         return 0;

#endif

}

Bool Menu_SetMultiWindow2Input(void)
{
#if (!Disable_4K60HZ_Timing)
#if DP_PBP_RELOAD_EDID	
BYTE HPD_Status=0;
#endif
#if HDMI_PBP_RELOAD_EDID
BYTE HDMI_HPD_Status=0;
#endif
#if DVI_PBP_RELOAD_EDID
BYTE DVI_HPD_Status=0;
#endif
#endif

    if (g_u8MenuTempValue==Get_SwitchSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN1))
        return FALSE;

    USER_PREF_INPUT_TYPE_SUBWIN1=Get_SwitchSource(g_u8MenuTempValue);
	
    //USER_PREF_INPUT_TYPE_SUBWIN1=g_u8MenuTempValue;

#if (!Disable_4K60HZ_Timing)
#if DP_PBP_RELOAD_EDID
  if(((DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))))
  ||((!DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DISPLAYPORT))))
  )
       {
                  //mhal_DP_HPDControl(4, FALSE);
                  msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, FALSE);	 	
		    HPD_Status=1;

                  EDID_ReLoad_Func(0);
                  msAPI_combo_LoadEDID_Func();
    		   printMsg("DP_PBP_RELOAD_EDID 222222222");	  
                  //ForceDelay1ms(600); // delay 1 msec				
      }
 #endif
#if HDMI_PBP_RELOAD_EDID
 if(((HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))))
 ||((!HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))))
 ||((HDMI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI))))
 ||((HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_HDMI))))
 )
  {
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	 		 
        HDMI_HPD_Status=1;			 
        ReLoadHDMI_EDID();
        printMsg("HDMI_PBP_RELOAD_EDID 555555");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif

#if DVI_PBP_RELOAD_EDID
 if(((DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))))
 ||((!DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))))
 ||((DVI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DVI))))
 ||((!DVI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DVI))))
 ||((DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DVI))))
 ||((!DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DVI))))
 )
  {
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, FALSE);	 		 
        DVI_HPD_Status=1;			 
        ReLoadDVI_EDID();
        printMsg("DVI_PBP_RELOAD_EDID 555555");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif

#if (DP_PBP_RELOAD_EDID||HDMI_PBP_RELOAD_EDID||DVI_PBP_RELOAD_EDID)
if(HPD_Status
#if (HDMI_PBP_RELOAD_EDID)		
||HDMI_HPD_Status
#endif	
#if (DVI_PBP_RELOAD_EDID)		
||DVI_HPD_Status
#endif	
)				  
{
     hw_ClrBlacklit();
     mStar_SetupFreeRunMode();
     ForceDelay1ms(600); // delay 1 msec		
}
#endif                 
#endif

    Set_HDMI1HPDFlag();   
    Set_HDMI2HPDFlag();   
    Set_HDMI3HPDFlag();   
    SwitchPort(DISPLAY_WIN2 , USER_PREF_INPUT_TYPE_SUBWIN1);

#if (!Disable_4K60HZ_Timing)
#if DP_PBP_RELOAD_EDID
	  if(HPD_Status==1)
            //mhal_DP_HPDControl(4, TRUE);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, TRUE);	 	
#endif	  
#if HDMI_PBP_RELOAD_EDID
	  if(HDMI_HPD_Status==1)
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	 	
#endif	  
#if DVI_PBP_RELOAD_EDID
	  if(DVI_HPD_Status==1)
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, TRUE);	 	
#endif	  
#endif
	
   return TRUE;
}

WORD GetWindow2InputValue(void)
{
  
    return Get_SwitchSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN1);
}

Bool AdjustMultiWindow2InputValue(MENU_ITEM_ACTION enAction)
{

    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, First_Input_Port, Input_Nums-1, 1);

    return TRUE;
}
#if Enable_Multi_Win3
Bool Menu_SetMultiWindow3Input(void)
{
#if (!Disable_4K60HZ_Timing)
#if DP_PBP_RELOAD_EDID	
BYTE HPD_Status=0;
#endif
#if HDMI_PBP_RELOAD_EDID
BYTE HDMI_HPD_Status=0;
#endif
#if DVI_PBP_RELOAD_EDID
BYTE DVI_HPD_Status=0;
#endif
#endif
    if (g_u8MenuTempValue==Get_SwitchSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN2))
        return FALSE;
	
//    USER_PREF_INPUT_TYPE_SUBWIN2=g_u8MenuTempValue;
	
    USER_PREF_INPUT_TYPE_SUBWIN2=Get_SwitchSource(g_u8MenuTempValue);

#if (!Disable_4K60HZ_Timing)
#if DP_PBP_RELOAD_EDID
  if(((DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))))
  ||((!DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DISPLAYPORT))))
  )
       {
                  //mhal_DP_HPDControl(4, FALSE);
                  msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, FALSE);	 	
		    HPD_Status=1;

                  EDID_ReLoad_Func(0);
                  msAPI_combo_LoadEDID_Func();
    		   printMsg("DP_PBP_RELOAD_EDID 222222222");	  
                  //ForceDelay1ms(600); // delay 1 msec				
      }
 #endif
#if HDMI_PBP_RELOAD_EDID
 if(((HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))))
 ||((!HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))))
 ||((HDMI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI))))
 ||((HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_HDMI))))
 )
  {
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	 		 
        HDMI_HPD_Status=1;			 
        ReLoadHDMI_EDID();
        printMsg("HDMI_PBP_RELOAD_EDID 555555");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif

#if DVI_PBP_RELOAD_EDID
 if(((DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))))
 ||((!DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))))
 ||((DVI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DVI))))
 ||((!DVI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DVI))))
 ||((DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DVI))))
 ||((!DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DVI))))
 )
  {
        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, FALSE);	 		 
        DVI_HPD_Status=1;			 
        ReLoadDVI_EDID();
        printMsg("DVI_PBP_RELOAD_EDID 555555");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif

	

#if (DP_PBP_RELOAD_EDID||HDMI_PBP_RELOAD_EDID||DVI_PBP_RELOAD_EDID)
if(HPD_Status
#if (HDMI_PBP_RELOAD_EDID)		
||HDMI_HPD_Status
#endif	
#if (DVI_PBP_RELOAD_EDID)		
||DVI_HPD_Status
#endif	
)				  
{
     hw_ClrBlacklit();
     mStar_SetupFreeRunMode();
     ForceDelay1ms(600); // delay 1 msec		
}
#endif                 
#endif

    Set_HDMI1HPDFlag();   
    Set_HDMI2HPDFlag();   
    Set_HDMI3HPDFlag();   
    SwitchPort(DISPLAY_WIN3 , USER_PREF_INPUT_TYPE_SUBWIN2);

#if (!Disable_4K60HZ_Timing)
#if DP_PBP_RELOAD_EDID
	  if(HPD_Status==1)
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, TRUE);	 	
#endif	  
#if HDMI_PBP_RELOAD_EDID
	  if(HDMI_HPD_Status==1)
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	 	
#endif	  
#if DVI_PBP_RELOAD_EDID
	  if(DVI_HPD_Status==1)
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, TRUE);	 	
#endif	  
#endif	
   return TRUE;
}

WORD GetWindow3InputValue(void)
{
    return Get_SwitchSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN2);
}

Bool AdjustMultiWindow3InputValue(MENU_ITEM_ACTION enAction)
{
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, First_Input_Port, Input_Nums-1, 1);
    return TRUE;
}
#endif
#if Enable_Multi_Win4
Bool Menu_SetMultiWindow4Input(void)
{
    if (g_u8MenuTempValue==Get_SwitchSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN3))
        return FALSE;
	
    //USER_PREF_INPUT_TYPE_SUBWIN3=g_u8MenuTempValue;
    USER_PREF_INPUT_TYPE_SUBWIN3=Get_SwitchSource(g_u8MenuTempValue);

    Set_HDMI1HPDFlag();   
    Set_HDMI2HPDFlag();   
    Set_HDMI3HPDFlag();   
    SwitchPort(DISPLAY_WIN4 , USER_PREF_INPUT_TYPE_SUBWIN3);
	
   return TRUE;
}
WORD GetWindow4InputValue(void)
{
    return Get_SwitchSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN3);
}

Bool AdjustMultiWindow4InputValue(MENU_ITEM_ACTION enAction)
{
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, First_Input_Port, Input_Nums-1, 1);
    return TRUE;
}
#endif

//=================================================================
//=================================================================

//------------------------------------------------------------------------------------------
Bool SetMultiWindowSwap(BYTE WIN1,BYTE WIN2 )
{
   BYTE Win1_Temp_SRC,Win2_Temp_SRC;
#if (!Disable_4K60HZ_Timing)
#if DP_PBP_RELOAD_EDID	   
   BYTE HPD_Status=0;
#endif
 #if HDMI_PBP_RELOAD_EDID  
   BYTE HDMI_HPD_Status=0;
 #endif
 #if DVI_PBP_RELOAD_EDID  
   BYTE DVI_HPD_Status=0;
 #endif
#endif 

    Win1_Temp_SRC=USER_PREF_WIN_SRC(WIN1);
    Win2_Temp_SRC=USER_PREF_WIN_SRC(WIN2);
 
   printData("Win1_Temp_SRC==%d",Win1_Temp_SRC);
   printData("Win2_Temp_SRC==%d",Win2_Temp_SRC);
    USER_PREF_WIN_SRC(WIN1)=Win2_Temp_SRC;
    USER_PREF_WIN_SRC(WIN2)=Win1_Temp_SRC;
	
   printData("USER_PREF_WIN_SRC(WIN1)==%d",USER_PREF_WIN_SRC(WIN1));
   printData("USER_PREF_WIN_SRC(WIN2)==%d",USER_PREF_WIN_SRC(WIN2));

#if Enable_AutoInput_Menu
  if(USER_PREF_INPUT_PRIORITY != INPUT_PRIORITY_AUTO)
  {
 #if ENABLE_VGA_INPUT 
   if(Win2_Temp_SRC==INPUT_VGA) 
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_VGA;
 #endif  
 
 #if (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)
 #if ENABLE_DVI_INPUT  
    if(Win2_Temp_SRC==INPUT_DVI)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DVI;
  #if ENABLE_DVI2ND_INPUT	
     if(Win2_Temp_SRC==INPUT_DVI2ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DVI2ND;
  #endif	
  #if ENABLE_DVI3ND_INPUT	
    if(Win2_Temp_SRC==INPUT_DVI3ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DVI3ND;
  #endif	
#endif

#else
 #if ENABLE_DVI_INPUT  
    if(Win2_Temp_SRC==INPUT_DVI)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DVI;
  #if ENABLE_DVI2ND_INPUT	
    else if(Win2_Temp_SRC==INPUT_DVI2ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DVI2ND;
  #endif	
  #if ENABLE_DVI3ND_INPUT	
    else if(Win2_Temp_SRC==INPUT_DVI3ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DVI3ND;
  #endif	
#endif
#endif

  #if ENABLE_HDMI_INPUT  
    if(Win2_Temp_SRC==INPUT_HDMI)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_HDMI;
  #if ENABLE_HDMI2ND_INPUT
    else if(Win2_Temp_SRC==INPUT_HDMI2ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_HDMI2ND;
  #endif	
  #if ENABLE_HDMI3ND_INPUT	
    else if(Win2_Temp_SRC==INPUT_HDMI3ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_HDMI3ND;
  #endif	
  #if ENABLE_HDMI4ND_INPUT	
    else if(Win2_Temp_SRC==INPUT_HDMI4ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_HDMI4ND;
  #endif	
#endif
  #if ENABLE_DP_INPUT  
   else if(Win2_Temp_SRC==INPUT_DISPLAYPORT)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DP;
  #if ENABLE_DP2ND_INPUT
   else if(Win2_Temp_SRC==INPUT_DISPLAYPORT2ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DP2ND;
  #endif	
  #if ENABLE_DP3ND_INPUT	
   else if(Win2_Temp_SRC==INPUT_DISPLAYPORT3ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DP3ND;
  #endif	
  #if ENABLE_DP4ND_INPUT	
   else if(Win2_Temp_SRC==INPUT_DISPLAYPORT4ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DP4ND;
  #endif	
  #if ENABLE_DP5ND_INPUT	
   else if(Win2_Temp_SRC==INPUT_DISPLAYPORT5ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DP5ND;
  #endif	
  #if ENABLE_DP6ND_INPUT	
   else if(Win2_Temp_SRC==INPUT_DISPLAYPORT6ND)
     USER_PREF_INPUT_PRIORITY = INPUT_PRIORITY_DP6ND;
  #endif	
#endif
  }
#endif


#if (!Disable_4K60HZ_Timing)

#if DP_PBP_RELOAD_EDID
  if(((DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))))
  ||((!DP_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT))))
 ||((DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DISPLAYPORT)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DISPLAYPORT))))
 ||((!DP_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DISPLAYPORT))))
  )
       {
                  //mhal_DP_HPDControl(4, FALSE);
                  msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, FALSE);	 	
		    HPD_Status=1;

                  EDID_ReLoad_Func(0);
                  msAPI_combo_LoadEDID_Func();
    		    printMsg("DP_PBP_RELOAD_EDID 5555555555");	  
                  //ForceDelay1ms(600); // delay 1 msec				
      }
 #endif
#if HDMI_PBP_RELOAD_EDID
 if(((HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))))
 ||((!HDMI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))))
 ||((HDMI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_HDMI))))
 ||((HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_HDMI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_HDMI))))
 ||((!HDMI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_HDMI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_HDMI))))
 )
  {

        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	 		 
        HDMI_HPD_Status=1;		
		
        ReLoadHDMI_EDID();
        printMsg("HDMI_PBP_RELOAD_EDID 555555");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif

#if DVI_PBP_RELOAD_EDID
 if(((DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE>=eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))))
 ||((!DVI_EDID_3840x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))))
 ||((DVI_EDID_1920x2160_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DVI))))
 ||((!DVI_EDID_1920x2160_Flag)&&(((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE==INPUT_DVI))))
 ||((DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=INPUT_DVI)&&(USER_PREF_INPUT_TYPE_SUBWIN2!=INPUT_DVI))))
 ||((!DVI_EDID_1920x1080_Flag)&&((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI))||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DVI||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DVI))))
 )
  {

        msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, FALSE);	 		 
        DVI_HPD_Status=1;		
		
        ReLoadDVI_EDID();
        printMsg("DVI_PBP_RELOAD_EDID 555555");	  
        //ForceDelay1ms(600); // delay 1 msec				
 }
#endif
#endif

#if AudioFunc     
   if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetModeChangeAudioMuteOnOff]!=NULL)
   ((fpCustomerSetModeChangeAudioMuteOnOff)CB_FuncList[eCB_CustomerSetModeChangeAudioMuteOnOff])();
   else
   {
      audio_EnableAudioAfterSignalLock_Flag=AUDIO_MUTE;			 
      msAPI_AdjustVolume(0);
      Delay1ms(100);
      hw_SetMute();	
      hw_Clr_SPIFGPIO_EN_Pin();	  
      #if ENABLE_DAC
      msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, TRUE);
      msAPI_AudioSetEarphoneMute(TRUE);
      msAPI_AudioSetMute( AUDIO_SOFTWARE_MUTE, E_SW_TIMING_CHANGE_MUTE_ON);
      #endif
      #if ENABLE_DAC
      msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
      #endif
   }
#endif

#if (!Disable_4K60HZ_Timing)
#if (DP_PBP_RELOAD_EDID||HDMI_PBP_RELOAD_EDID||DVI_PBP_RELOAD_EDID)
if(HPD_Status
#if (HDMI_PBP_RELOAD_EDID)		
||HDMI_HPD_Status
#endif	
#if (DVI_PBP_RELOAD_EDID)		
||DVI_HPD_Status
#endif	
)				  
{
     hw_ClrBlacklit();
     mStar_SetupFreeRunMode();
     ForceDelay1ms(600); // delay 1 msec		
}
#endif                 
#endif
    //Clr_AdjustPipPositionFlag();
    mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);   
    APPWindow_Configure((eMULTIWIN_Type)USER_PREF_MULTI_WIN_MODE, (ePIPSIZE_Type)USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2));

#if (!Disable_4K60HZ_Timing)
#if DP_PBP_RELOAD_EDID
	  if(HPD_Status==1)
            //mhal_DP_HPDControl(4, TRUE);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, TRUE);	 	
#endif	  
#if HDMI_PBP_RELOAD_EDID
	  if(HDMI_HPD_Status==1)
             msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	 
#endif	  
#if DVI_PBP_RELOAD_EDID
	  if(DVI_HPD_Status==1)
             msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DVI].eIPMux, TRUE);	 
#endif	  
#endif

   return TRUE;
}

Bool Osd_Set_PiP_PbP_Swap( void )
{

       //g_stMenuFlag.bSetOSDPrepareForTimingChange=1;
	SetMultiWindowSwap(DISPLAY_WIN1,DISPLAY_WIN2);
       g_u8SaveUserData|=SAVE_MONITOR_SETTING;
	return TRUE;
}
//------------------------------------------------------------------------------------------

Bool Menu_SetPIPSize(void)
{
    OrigAdjPipHpos=(100-GetPIPHPositionValue());
    OrigAdjPipVpos=GetPIPVPositionValue();

    if (g_u8MenuTempValue == USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))
        return FALSE;
    USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) = g_u8MenuTempValue;

    //Set PIP size function here...
    if (OrigAdjPipHpos!=0)
        USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = (DWORD)((PANEL_WIDTH-GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)))*OrigAdjPipHpos)/100;
    if (OrigAdjPipVpos!=0)
        USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = (DWORD)((PANEL_HEIGHT-GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)))*OrigAdjPipVpos)/100;
 //if(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1))
 //   mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
    WAIT_V_OUTPUT_BLANKING_START();
    APPWindow_SetSize(DISPLAY_WIN2, USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2), USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2), GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)), GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)));
    //  mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
 return TRUE;
}

WORD GetPIPSizeValue(void)
{
    return USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2);
}

Bool AdjustPIPSizeValue(MENU_ITEM_ACTION enAction)
{
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, ePIPSIZE_SMALL, ePIPSIZE_MAX-1, 1);
    return TRUE;
}

Bool AdjustPIPHPosition(MENU_ITEM_ACTION enAction)
{
    WORD u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2), 0, (PANEL_WIDTH-GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))), 3);

    if (u16TempValue == USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2))
        return FALSE;
    USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = u16TempValue;

    WAIT_V_OUTPUT_BLANKING_START();
    APPWindow_SetPosition(DISPLAY_WIN2, USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2), USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2));
    return TRUE;
}

WORD GetPIPHPositionValue(void)
{
    //return USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2);
    return 100 - GetScale100Value(USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2), 0, (PANEL_WIDTH-GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))));
}

Bool AdjustPIPVPosition(MENU_ITEM_ACTION enAction)
{
    WORD u16TempValue;

    u16TempValue = DecIncValue(enAction, USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2), 0, (PANEL_HEIGHT-GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))), 3);

    if (u16TempValue == USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2))
        return FALSE;
    USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = u16TempValue;

    WAIT_V_OUTPUT_BLANKING_START();
    APPWindow_SetPosition(DISPLAY_WIN2, USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2), USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2));
    return TRUE;
}

WORD GetPIPVPositionValue(void)
{
    //return USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2);
    return GetScale100Value(USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2), 0, (PANEL_HEIGHT-GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))));
}



void Set_PIP_Border(void)
{
      if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP)
      {
	  msAPI_EnableBorder(DISPLAY_WIN2,ENABLE);
	  msAPI_SetBorderBatchFormat(DISPLAY_WIN2, DEF_SUBWIN_BORDER_WIDTH, 0, DEF_SUBWIN_BORDER_COLOR);
      }
	  /*
      else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1)	
      {
	  msAPI_EnableBorder(DISPLAY_WIN2,ENABLE);
	  msAPI_SetBorderBatchFormat(DISPLAY_WIN2, DEF_SUBWIN_BORDER_WIDTH, 0, DEF_SUBWIN_BORDER_COLOR);
      }
	  */
      else
      {
	  msAPI_EnableBorder(DISPLAY_WIN1,DISABLE);
	  msAPI_EnableBorder(DISPLAY_WIN2,DISABLE);
	  msAPI_EnableBorder(DISPLAY_WIN3,DISABLE);
	  msAPI_EnableBorder(DISPLAY_WIN4,DISABLE);
      }

}

//------------------------------------------------------------------------------------------

WORD GetPIPPositionValue(void)
{
    return USER_PREF_PIP_POSITION;
}

Bool Osd_Set_PiP_Position(MENU_ITEM_ACTION enAction)
{
	/////////////////////////////////////////////////////
	//
	// PiP Position: Top-Right, Top-Left, Bottom-Right, Bottom-Left 
	//
	/////////////////////////////////////////////////////
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, PIPPosition_Top_Right, PIPPosition_Max-1, 1);
	
/*		
	if(g_u8MenuItemIndex==0x00)
	{
	   USER_PREF_PIP_POSITION = PIPPosition_Top_Right;		
	}
	else if(g_u8MenuItemIndex==0x01)
	{	
	   USER_PREF_PIP_POSITION = PIPPosition_Top_Left;			
	}
	else if(g_u8MenuItemIndex==0x02)
	{		
	   USER_PREF_PIP_POSITION = PIPPosition_Bottom_Right; 
	}
	else if(g_u8MenuItemIndex==0x03)
	{		
	   USER_PREF_PIP_POSITION = PIPPosition_Bottom_Left; 
	}
	else 
	{		
	   USER_PREF_PIP_POSITION = PIPPosition_Top_Right; 
	}
*/	
//	SetPIPPosition();
       g_u8SaveUserData|=SAVE_MONITOR_SETTING;
	return TRUE;
}

Bool SetPIPPositionParameter(void)
{
   
    switch(USER_PREF_PIP_POSITION)
    {
 #if Enable_Panel_Rotate_180
	case PIPPosition_Top_Left:
		USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = PANEL_WIDTH - GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))-PIPPosition_Offset;
		USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = PANEL_HEIGHT - GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))-PIPPosition_Offset;
	break;
	case PIPPosition_Top_Right:
		USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = PIPPosition_Offset;
		USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = PANEL_HEIGHT - GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))-PIPPosition_Offset;
	break;
	case PIPPosition_Bottom_Left:
		USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = PANEL_WIDTH - GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))-PIPPosition_Offset;
		USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = PIPPosition_Offset;
	break;
	case PIPPosition_Bottom_Right:
		USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = PIPPosition_Offset;
		USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = PIPPosition_Offset;
	break;
 #else
	case PIPPosition_Top_Left:
		USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = PIPPosition_Offset;
		USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = PIPPosition_Offset;
	break;
	case PIPPosition_Top_Right:
		USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = PANEL_WIDTH - GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))-PIPPosition_Offset;
		USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = PIPPosition_Offset;
	break;
	case PIPPosition_Bottom_Left:
		USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = PIPPosition_Offset;
		USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = PANEL_HEIGHT - GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))-PIPPosition_Offset;
		break;
	case PIPPosition_Bottom_Right:
		USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = PANEL_WIDTH - GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))-PIPPosition_Offset;
		USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = PANEL_HEIGHT - GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))-PIPPosition_Offset;
		break;
   #endif		
	default:
	break;
    }

  
     return TRUE;
}

Bool SetPIPPosition(void)
{
//       Set_AdjustPipPositionFlag();
    if (g_u8MenuTempValue == USER_PREF_PIP_POSITION)
        return FALSE;

      USER_PREF_PIP_POSITION=g_u8MenuTempValue;
	//mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);   
	SetPIPPositionParameter();
	WAIT_V_OUTPUT_BLANKING_START();
	APPWindow_SetPosition(DISPLAY_WIN2, USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2), USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2));
	
	Set_PIP_Border();
	printMsg("Set_PIP_Border");
  	//mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);   

     return TRUE;
}



#endif //ENABLE_MULTI_WINDOW_SETTINGS_MENU


#if 0//Mike Remove it.
Bool CheckLCDConditionValue(void)
{
    Delay1ms(50);
    if (g_u8MenuTempValue == 0)
        return TRUE;
    return FALSE;
}
#endif


#if ENABLE_DCR
WORD GetDCREnableRealValue(void)
{
return (DCR_ENABLE_FLAG?_ON_:_OFF_);   
}
Bool SetDCRFunction(void)
{
    if((g_u8MenuTempValue==_OFF_ && !DCR_ENABLE_FLAG) ||
        (g_u8MenuTempValue==_ON_ && DCR_ENABLE_FLAG))
    {
        Delay1ms(50);
        return FALSE;
    }
    if (g_u8MenuTempValue==_OFF_)
    {
    
	 CLR_DCR_FLAG_ENABLE();
        CLR_DCR_ENABLE_FLAG();
	 Clr_AdjDcrBacklightFlag();	
        #if ENABLE_DPS
        msDPS_Off_Setting();
        #endif
    }
    else
    {
    
	 SET_DCR_FLAG_ENABLE();
        SET_DCR_ENABLE_FLAG();
	 Set_AdjDcrBacklightFlag();	
        #if ENABLE_DPS
        msDPS_On_Setting();
        #endif

    }
 #if ENABLE_DCR
    MF_DCRCheckBrightness();
#else //080905
    msAPI_AdjustBrightness(UserPrefBrightness_Temp);
#endif
    
    SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));
    Delay1ms(50);
    return FALSE;
}

Bool SetHotDCRFunction(void)
{

    if (DCR_ENABLE_FLAG)
    {
        CLR_DCR_ENABLE_FLAG();
	 Clr_AdjDcrBacklightFlag();	
        #if ENABLE_DPS
        msDPS_Off_Setting();
        #endif
    }
    else
    {
        SET_DCR_ENABLE_FLAG();
	 Set_AdjDcrBacklightFlag();	
        #if ENABLE_DPS
        msDPS_On_Setting();
        #endif

    }
 #if ENABLE_DCR
    MF_DCRCheckBrightness();
#else //080905
    msAPI_AdjustBrightness(UserPrefBrightness_Temp);
#endif

    SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));
    Delay1ms(50);
    return FALSE;
}


#endif

#if HDMI_HPD_Enable
WORD GetHDMIHPDRealValue(void)
{
return (MENU_HDMI_HPD_FLAG?_ON_:_OFF_);   
}
Bool SetHDMIHPDFunction(void)
{
BYTE SourceIndex;
    if((g_u8MenuTempValue==_OFF_ && !MENU_HDMI_HPD_FLAG) ||
        (g_u8MenuTempValue==_ON_ && MENU_HDMI_HPD_FLAG))
    {
        Delay1ms(50);
        return FALSE;
    }
    if (g_u8MenuTempValue==_OFF_)
    {
        CLR_MENU_HDMI_HPD_FLAG();
    }
    else
    {
        SET_MENU_HDMI_HPD_FLAG();
    }
	
if((MENU_HDMI_HPD_FLAG))
{
    SourceIndex=MapWin2Port(USER_PREF_WIN_SEL);
    //SwitchInputSource(g_u8MenuTempValue);

    Set_HDMI1HPDFlag();   
    Set_HDMI2HPDFlag();   
    Set_HDMI3HPDFlag();   
#if HDMI_HPD_Enable
   if(MENU_HDMI_HPD_FLAG)
#endif
{
     if((!INPUT_IS_DISPLAYPORT(SourceIndex))&&(!INPUT_IS_VGA(SourceIndex)))
     {
            //msAPISrcSetHPD(SourceIndex, DFT_HPD_PERIOD); // set reasonable value, e.g. >= 300ms
         #if (defined(XuLi_Project))//mqsh for GTX 750 Ti HDMI 2.0 Switch port display error(input(3840x2160 60) OSD show 1920x2160 60)
            msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);
            ForceDelay1ms(30);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	
            ForceDelay1ms(300);
            msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	 
	#else
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	
            ForceDelay1ms(300);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	 				
	#endif
     }		 
}

//    SwitchPort(USER_PREF_WIN_SEL, SourceIndex);	
	
}
    
    SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));
    Delay1ms(50);
    return FALSE;
}
#endif


#if ENABLE_MMD_DCR
   WORD xdata AveLumi;
void MF_DCRHandler(void)
{
if( !SyncLossState(USER_PREF_INPUT_TYPE) && (InputTimingStable_NormalFlag) && DCR_PixelCount != 0)
{
       AveLumi=(DWORD)DCR_PixelWeight*256/DCR_PixelCount;
 
	//printData("LumCount %d", DCR_PixelWeight);
	//printData("g_MaxPixel %d", DCR_MaxPixel );
	//printData("AveLumi %d", AveLumi);
       //printData("PixelCount==%d", DCR_PixelCount);
       //printData("DCR_TargetValue==%d", DCR_TargetValue);
       //printData("DCRBacklight==%d", DCRBacklight);
        AveLumi= ((DWORD)AveLumi*100/255);
	if(AveLumi <= BlackRange)
	{
		if(DCR_MaxPixel<= MaxPixelValue)
		{
			DCR_TargetValue = ((WORD)DCRDarkBacklight*100/255);
			if(DCR_Status==DCR_IDLE)
				DCR_Status=DCR_START_WAIT_BL_READY;
		}
		else
		{
			DCR_TargetValue = ((WORD)DCRMinBacklight*100/255);
		}
	}
	else if(AveLumi > WhiteRange)
	{
		DCR_TargetValue = 100;//DCRMaxBacklight;
	}
	else
	{
		DCR_TargetValue = AveLumi;//(DCRMaxBacklight+DCRMinBacklight)/2;
	}

	
	if (DCRBacklight != DCR_TargetValue)
	{
	printData("DCRBacklight 000==%d", DCRBacklight);
	printData("DCR_TargetValue 000==%d", DCR_TargetValue);
	Set_AdjDcrBacklightFlag();

		if (DCRBacklight < DCR_TargetValue)
		{
              #if 1//(defined(ZhouYang_Project))
                  if(abs(DCRBacklight-DCR_TargetValue)>90)
                     DCRBacklight+=30;
                  else if(abs(DCRBacklight-DCR_TargetValue)>75)	  
                     DCRBacklight+=15;
                  else if(abs(DCRBacklight-DCR_TargetValue)>25)	  
                     DCRBacklight+=5;
                  else
		#endif		  			
			DCRBacklight++;
		}
		else
		{
              #if 1//(defined(ZhouYang_Project))
                  if(abs(DCRBacklight-DCR_TargetValue)>90)
                     DCRBacklight-=30;
                  else if(abs(DCRBacklight-DCR_TargetValue)>75)	  
                     DCRBacklight-=15;
                  else if(abs(DCRBacklight-DCR_TargetValue)>25)	  
                     DCRBacklight-=5;
                  else
		#endif		  	
			DCRBacklight--;
		}
	}
	if( AdjDcrBacklightFlag	)
	{
	       if(DCRBacklight == DCR_TargetValue)
	       {
                Clr_AdjDcrBacklightFlag();	
	       }
	       printData("DCRBacklight==%d", DCRBacklight);
		MF_DCRAdjustBrightness(DCRBacklight);
	}			
}

}
void MF_DCRAdjustBrightness(BYTE u8Brightness)
{
    msAPI_AdjustBrightness(u8Brightness);
}
void MF_DCRCheckBrightness(void)
{
    if(DCR_ENABLE_FLAG)
        MF_DCRAdjustBrightness(DCRBacklight);
    else
        msAPI_AdjustBrightness(UserPrefBrightness_Temp);

}

void SmartConntrastTimerHandler(void)
{
		HighSmartContrastHandler(USER_PREF_WIN_SEL);
		if (DCRWaitBLReadyCounter)
		{
			--DCRWaitBLReadyCounter;
			#if ENABLE_DCR
			printData("DCRWaitBLReadyCounter===%d",DCRWaitBLReadyCounter);
			#endif
		}
		if (DCRWaitBLOffCounter)
		{
			--DCRWaitBLOffCounter;
			#if ENABLE_DCR
			printData("DCRWaitBLOffCounter===%d",DCRWaitBLOffCounter);
			#endif
		}
		if (HighContCounter)
		{
			--HighContCounter;
			#if ENABLE_DCR
			printData("HighContCounter===%d",HighContCounter);
			#endif
		}
}
void HighSmartContrastHandler(BYTE u8WinIndex)
{
    u8WinIndex=u8WinIndex;
    if(DCR_Status == DCR_IDLE)
    {
        if(DCRWaitBLOffCounter)
            DCRWaitBLOffCounter = 0;

        if(HighContCounter)
            HighContCounter = 0;

        return;
    }

    switch(DCR_Status)
    {
        case DCR_START_WAIT_BL_READY:
            DCRWaitBLReadyCounter = 0;
            DCR_Status++;
        break;

        case DCR_WAIT_BL_READY:
            if(DCRWaitBLReadyCounter==0)
            DCR_Status++;
        break;

        case DCR_BL_READY:
            if(DCR_TargetValue==DCRDarkBacklight && (DCR_MaxPixel<= MaxPixelValue))
            {
                DCRWaitBLOffCounter = DefBacklightOnCounter2min;
                DCR_Status++;
            }
        break;

        case DCR_START_WAIT_BL_OFF:
            if(DCR_TargetValue==DCRDarkBacklight && (DCR_MaxPixel<= MaxPixelValue))
            {
                if(DCRWaitBLOffCounter==0)
		    {
			DCR_Status++;
		     }
            }
            else
            {
                DCRWaitBLOffCounter = 0;
                DCR_Status = DCR_BL_READY;
            }
        break;

        case DCR_WAIT_BL_OFF:
            if(DCR_TargetValue==DCRDarkBacklight && (DCR_MaxPixel <= MaxPixelValue))
            {
			if(!OSDShowFlag)//No OSD
			{
				hw_ClrBlacklit();
                          #if 0//(VBY1_PowerSequence||EDP_PowerPanelSequence)
                            PowerOnBackLightCounter=0;
                          #endif
				g_u16OsdCounter = 0;
				g_stMenuFlag.bOsdTimeout = 1;			  	
				DCR_Status++;
            		}
            }
            else
            {
                DCR_Status = DCR_BL_READY;
            }
        break;

        case DCR_BL_OFF:
            if(DCR_TargetValue==DCRDarkBacklight && (DCR_MaxPixel<= MaxPixelValue))
            {
                HighContCounter = 0;
                DCR_Status++;
            }
            else
            {
                hw_SetBlacklit();
                DCR_Status = DCR_BL_READY;
            }
        break;

        case DCR_WAIT_EXIT_HIGH_CONT:
            if(DCR_TargetValue==DCRDarkBacklight && (DCR_MaxPixel<= MaxPixelValue))
            {
            
            }
            else
            {
                hw_SetBlacklit();
                HighContCounter = 0;
                DCR_Status = DCR_BL_READY;
            }
        break;

        case DCR_EXIT_HIGH_CONT:
            hw_SetBlacklit();
            DCR_Status = DCR_WAIT_HISTOGRAM_CHANGE;//DCR_BL_READY;
        break;

        case DCR_WAIT_HISTOGRAM_CHANGE:
            if(DCR_TargetValue==DCRDarkBacklight && ( DCR_MaxPixel<= MaxPixelValue))
            {
            }
	     else
	     {
            DCR_Status = DCR_BL_READY;
	     }
        break;

        default:
        break;
    }
}
#endif




#if ENABLE_MULTI_INPUT

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

BYTE Get_AutoSource_NextPort(BYTE InputType)
{    
   //BYTE InputType_Temp;
#if (MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)
      if(InputType==INPUT_VGA)
         return INPUT_DVI;
      else if(InputType==INPUT_DVI)
         return INPUT_HDMI;
      else if(InputType==INPUT_HDMI)
         return INPUT_DISPLAYPORT;
      else if(InputType==INPUT_DISPLAYPORT)
         return INPUT_VGA;
      else
         return INPUT_VGA;
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
      if(InputType==INPUT_VGA)
         return INPUT_DISPLAYPORT;
      else if(InputType==INPUT_DISPLAYPORT)
         return INPUT_HDMI;
      else if(InputType==INPUT_HDMI)
         return INPUT_HDMI2ND;
      else if(InputType==INPUT_HDMI2ND)
         return INPUT_HDMI3ND;
      else if(InputType==INPUT_HDMI3ND)
         return INPUT_VGA;
      else
         return INPUT_VGA;
#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)
 #if AIO_Board
    #if SwitchPortByDP_Detect
       if(!hw_DP_DET_Pin)
         return INPUT_DISPLAYPORT;
    #endif
         InputType=InputType;
         return INPUT_HDMI;
 #else
      if(InputType==INPUT_DVI)
         return INPUT_HDMI;
      else if(InputType==INPUT_HDMI)
         return INPUT_DISPLAYPORT;
      else if(InputType==INPUT_DISPLAYPORT)
         return INPUT_DVI;
      else
         return INPUT_DVI;
  #endif
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)
      if(InputType==INPUT_HDMI)
         return INPUT_DISPLAYPORT;
      else if(InputType==INPUT_DISPLAYPORT)
         return INPUT_HDMI;
      else
         return INPUT_HDMI;

#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1)
      if(InputType==INPUT_HDMI)
         return INPUT_HDMI2ND;
      else if(InputType==INPUT_HDMI2ND)
         return INPUT_HDMI3ND;
      else if(InputType==INPUT_HDMI3ND)
         return INPUT_DISPLAYPORT;
      else if(InputType==INPUT_DISPLAYPORT)
         return INPUT_HDMI;
      else
         return INPUT_HDMI;
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)	  
      if(InputType==INPUT_DISPLAYPORT)
         return INPUT_HDMI;
      else if(InputType==INPUT_HDMI)
         return INPUT_HDMI2ND;
      else if(InputType==INPUT_HDMI2ND)
         return INPUT_DVI;
      else if(InputType==INPUT_DVI)
         return INPUT_DISPLAYPORT;
      else
         return INPUT_DISPLAYPORT;
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)	  
      if(InputType==INPUT_HDMI)
         return INPUT_HDMI2ND;
      else if(InputType==INPUT_HDMI2ND)
         return INPUT_DISPLAYPORT;
      else if(InputType==INPUT_DISPLAYPORT)
         return INPUT_HDMI;
      else
         return INPUT_DISPLAYPORT;
	  

#else//BD_MST9UXX_V1 
      if(InputType==INPUT_VGA)
         return INPUT_DVI;
      else if(InputType==INPUT_DVI)
         return INPUT_HDMI;
      else if(InputType==INPUT_HDMI)
         return INPUT_DISPLAYPORT;
      else if(InputType==INPUT_DISPLAYPORT)
         return INPUT_VGA;
      else
         return INPUT_VGA;

#endif
	//InputType_Temp=(InputType+1)%Input_Nums;
	
       //return InputType_Temp;
	  
}


BYTE Get_SwitchSource(BYTE InputType)
{    
#if (MS_BOARD_TYPE_SEL ==  BD_MST9UHL_A0)
      if(InputType==0)
         return INPUT_VGA;
      else if(InputType==1)
         return INPUT_DVI;
      else if(InputType==2)
         return INPUT_HDMI;
      else if(InputType==3)
         return INPUT_DISPLAYPORT;
      else
         return INPUT_VGA;
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V3)
   #if (defined(FengYuan_Project)||defined(XuLi_Project)||defined(MingCai_Project))
      if(InputType==0)
         return INPUT_DISPLAYPORT;
      else if(InputType==1)
         return INPUT_HDMI;
      else if(InputType==2)
         return INPUT_HDMI2ND;
      else if(InputType==3)
         return INPUT_HDMI3ND;
      else if(InputType==4)
         return INPUT_VGA;
      else
         return INPUT_VGA;
   #else
      if(InputType==0)
         return INPUT_VGA;
      else if(InputType==1)
         return INPUT_DISPLAYPORT;
      else if(InputType==2)
         return INPUT_HDMI;
      else if(InputType==3)
         return INPUT_HDMI2ND;
      else if(InputType==4)
         return INPUT_HDMI3ND;
      else
         return INPUT_VGA;
   #endif	  
#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)
  #if AIO_Board
         InputType=InputType;
         return INPUT_HDMI;
  #else
       if(InputType==0)
         return INPUT_DVI;
      else if(InputType==1)
         return INPUT_HDMI;
      else if(InputType==2)
         return INPUT_DISPLAYPORT;
      else
         return INPUT_DVI;
  #endif
 #elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)
       if(InputType==0)
         return INPUT_HDMI;
      else if(InputType==1)
         return INPUT_DISPLAYPORT;
      else
         return INPUT_HDMI;
 #elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1)
       if(InputType==0)
         return INPUT_HDMI;
      else if(InputType==1)
         return INPUT_HDMI2ND;
      else if(InputType==2)
         return INPUT_HDMI3ND;
      else if(InputType==3)
         return INPUT_DISPLAYPORT;
      else
         return INPUT_HDMI;
 #elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
       if(InputType==0)
         return INPUT_DVI;
      else if(InputType==1)
         return INPUT_HDMI;
      else if(InputType==2)
         return INPUT_HDMI2ND;
      else if(InputType==3)
         return INPUT_DISPLAYPORT;
      else
         return INPUT_DVI; 
 #elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
      if(InputType==0)
         return INPUT_HDMI;
      else if(InputType==1)
         return INPUT_HDMI2ND;
      else if(InputType==2)
         return INPUT_DISPLAYPORT;
      else
         return INPUT_HDMI; 

	  
#else//BD_MST9UHL_A0 
      if(InputType==0)
         return INPUT_VGA;
      else if(InputType==1)
         return INPUT_DVI;
      else if(InputType==2)
         return INPUT_HDMI;
      else if(InputType==3)
         return INPUT_DISPLAYPORT;
      else
         return INPUT_VGA;

#endif

}


#if Enable_AutoInput_Menu

BYTE Get_SourceSwitchType(void)
{
   BYTE ucInputPortType=USER_PREF_INPUT_TYPE;


     if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_AUTO)
	 ucInputPortType=USER_PREF_INPUT_TYPE;	
  #if ENABLE_VGA_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_VGA)
	 ucInputPortType=INPUT_VGA;	
  #endif

  #if ENABLE_DVI_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DVI)
	 ucInputPortType=INPUT_DVI;	
    #if ENABLE_DVI2ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DVI2ND)
	 ucInputPortType=INPUT_DVI2ND;	
    #endif
    #if ENABLE_DVI3ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DVI3ND)
	 ucInputPortType=INPUT_DVI3ND;	
    #endif
  #endif
  #if ENABLE_HDMI_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI)
	 ucInputPortType=INPUT_HDMI;	
    #if ENABLE_HDMI2ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI2ND)
	 ucInputPortType=INPUT_HDMI2ND;	
    #endif
    #if ENABLE_HDMI3ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI3ND)
	 ucInputPortType=INPUT_HDMI3ND;	
    #endif
    #if ENABLE_HDMI4ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_HDMI4ND)
	 ucInputPortType=INPUT_HDMI4ND;	
    #endif

  #endif
  #if ENABLE_DP_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP)
	 ucInputPortType=INPUT_DISPLAYPORT;	
    #if ENABLE_DP2ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP2ND)
	 ucInputPortType=INPUT_DISPLAYPORT2ND;	
    #endif
    #if ENABLE_DP3ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP3ND)
	 ucInputPortType=INPUT_DISPLAYPORT3ND;	
    #endif
    #if ENABLE_DP4ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP4ND)
	 ucInputPortType=INPUT_DISPLAYPORT4ND;	
    #endif
    #if ENABLE_DP5ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP5ND)
	 ucInputPortType=INPUT_DISPLAYPORT5ND;	
    #endif
    #if ENABLE_DP6ND_INPUT
    else if (USER_PREF_INPUT_PRIORITY == INPUT_PRIORITY_DP6ND)
	 ucInputPortType=INPUT_DISPLAYPORT6ND;	
    #endif

  #endif

    return ucInputPortType;
 

}
#endif

void SwitchPort( BYTE ucWinIndex, BYTE ucInputPort )
{
//    BYTE uc;
    if (!SyncLossState(MapWin2Port(USER_PREF_WIN_SEL)))
    {
        WAIT_V_OUTPUT_BLANKING();
    }
       hw_ClrBlacklit();
       mStar_SetupFreeRunMode();
	   
      //msAPI_AudioSetEarphoneMute(TRUE);				
      //msAPI_AdjustVolume(0);     
      //hw_SetMute();
      //audio_EnableAudioAfterSignalLock_Flag=AUDIO_MUTE;
     printData("SwitchPort 0000000000==%d",ucInputPort );
    g_DisplayWindow[ucWinIndex].bDwReadCurFrame=0;		
	  
#if (!AIO_Board  )
#if HDMI_HPD_Enable
   if(MENU_HDMI_HPD_FLAG)
#endif
{
     if((!INPUT_IS_DISPLAYPORT(ucInputPort))&&(!INPUT_IS_VGA(ucInputPort)))
     {
        #if (defined(XuLi_Project))//mqsh for GTX 750 Ti HDMI 2.0 Switch port display error(input(3840x2160 60) OSD show 1920x2160 60)
            msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, FALSE);
            ForceDelay1ms(30);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, FALSE);	
            ForceDelay1ms(300);
            msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, TRUE);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, TRUE);	 
	#else
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, FALSE);	
            ForceDelay1ms(300);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, TRUE);	 				
	#endif
        //msAPISrcSetHPD(ucInputPort, DFT_HPD_PERIOD); // set reasonable value, e.g. >= 300ms
     }		
}
#endif	 

     msAPIWinSwitchPort(ucWinIndex, ucInputPort);
#if ENABLE_VGA_INPUT	 
     if(ucInputPort==INPUT_VGA)
     { 
         // VGA force Separate Sync
         mStar_SetAnalogInputPort(ucInputPort, FALSE); 
     }
#endif       

    USER_PREF_WIN_SRC(ucWinIndex) = ucInputPort;		

    SrcFlags(ucInputPort) &= ~( bUnsupportMode | bUserMode | bNativeMode );
    
    Clr_PowerSavingFlag();

    //g_stMenuFlag.bSetOSDPrepareForTimingChange=1;
    g_stMenuFlag.bSwitchInputPortStatus[ucWinIndex]=ucInputPort;
    g_stMenuFlag.bSwitchInputPortFlag[ucWinIndex]=1;

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
 if(USER_PREF_MULTI_WIN_MODE > eMULTIWIN_OFF)
 {
	g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN2]=1;
	g_stMenuFlag.bDisplayInputNoSignalMsg[ucWinIndex]=1;
	
	if(USER_PREF_MULTI_WIN_MODE > eMULTIWIN_2_POP1)
	{
		g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN3]=1;
		g_stMenuFlag.bDisplayInputNoSignalMsg[ucWinIndex]=1;
		if(USER_PREF_MULTI_WIN_MODE > eMULTIWIN_3)
		{
			g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN4]=1;
			g_stMenuFlag.bDisplayInputNoSignalMsg[ucWinIndex]=1;
		}
	}
	
  }	
 else
 {
	g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;
 }
 #else
	g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;
#endif 


    //g_stMenuFlag.bDisplayInputNoSignalMsg[ucWinIndex]=1;   
    //g_stMenuFlag.bOsdSwitchInputPort=1;
    //g_stMenuFlag.bAnyWinDisplayChange=1;
	

    SrcFlags(ucInputPort) |= SyncLoss;
    Set_InputTimingChangeFlag(ucInputPort);
    g_u8SwitchPortCounter = 0; 
}


Bool SetInputSelect(void)
{
BYTE SourceIndex;

#if Enable_AutoInput_Menu

  if(USER_PREF_INPUT_PRIORITY==g_u8OriginalSetting)
    return FALSE;
  
    //USER_PREF_INPUT_PRIORITY=g_u8MenuTempValue;

    SourceIndex=Get_SourceSwitchType();
#else
    //g_u8MenuTempValue=Get_SwitchSource(g_u8MenuItemIndex);

  if(USER_PREF_INPUT_TYPE==g_u8OriginalSetting)
    return FALSE;

    SourceIndex=USER_PREF_INPUT_TYPE;
    //USER_PREF_INPUT_TYPE=g_u8MenuTempValue	;
#endif	
    g_stMenuFlag.bShowInputIcon=0;	
    //SwitchInputSource(g_u8MenuTempValue);

    Set_HDMI1HPDFlag();   
    Set_HDMI2HPDFlag();   
    Set_HDMI3HPDFlag();   
	
    g_u8SwitchPortCounter = 0; // sync locked, counter reset to 0	

    SwitchPort(USER_PREF_WIN_SEL, SourceIndex);		

#if Disable_MultiWindow_AutoSwitch
 if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
 {

 /*
     if((USER_PREF_INPUT_TYPE!=USER_PREF_INPUT_TYPE_SUBWIN1)&&(USER_PREF_INPUT_TYPE!=USER_PREF_INPUT_TYPE_SUBWIN2)&&(USER_PREF_INPUT_TYPE_SUBWIN1!=USER_PREF_INPUT_TYPE_SUBWIN2))
     	;
     else if(USER_PREF_INPUT_TYPE==USER_PREF_INPUT_TYPE_SUBWIN1)
     {
       USER_PREF_INPUT_TYPE_SUBWIN1=(USER_PREF_INPUT_TYPE_SUBWIN1+1)%Input_Nums;
	if(USER_PREF_INPUT_TYPE_SUBWIN1==USER_PREF_INPUT_TYPE_SUBWIN2)   
       USER_PREF_INPUT_TYPE_SUBWIN1=(USER_PREF_INPUT_TYPE_SUBWIN1+1)%Input_Nums;
     }
     else if(USER_PREF_INPUT_TYPE==USER_PREF_INPUT_TYPE_SUBWIN2)
     {
       USER_PREF_INPUT_TYPE_SUBWIN2=(USER_PREF_INPUT_TYPE_SUBWIN2+1)%Input_Nums;
	if(USER_PREF_INPUT_TYPE_SUBWIN1==USER_PREF_INPUT_TYPE_SUBWIN2)   
       USER_PREF_INPUT_TYPE_SUBWIN2=(USER_PREF_INPUT_TYPE_SUBWIN2+1)%Input_Nums;
     }
     else if(USER_PREF_INPUT_TYPE_SUBWIN1==USER_PREF_INPUT_TYPE_SUBWIN2)	 
     {
       USER_PREF_INPUT_TYPE_SUBWIN2=(USER_PREF_INPUT_TYPE_SUBWIN2+1)%Input_Nums;
	if(USER_PREF_INPUT_TYPE==USER_PREF_INPUT_TYPE_SUBWIN2)   
       USER_PREF_INPUT_TYPE_SUBWIN2=(USER_PREF_INPUT_TYPE_SUBWIN2+1)%Input_Nums;
     }
*/	 

       USER_PREF_INPUT_TYPE_SUBWIN1=(USER_PREF_INPUT_TYPE+1)%Input_Nums;
       USER_PREF_INPUT_TYPE_SUBWIN2=(USER_PREF_INPUT_TYPE_SUBWIN1+1)%Input_Nums;
 
     if(USER_PREF_MULTI_WIN_MODE>eMULTIWIN_2_POP1)
       SwitchPort(DISPLAY_WIN3 , USER_PREF_INPUT_TYPE_SUBWIN2);
     if(USER_PREF_MULTI_WIN_MODE>eMULTIWIN_3)
       SwitchPort(DISPLAY_WIN4 , USER_PREF_INPUT_TYPE_SUBWIN3);
       SwitchPort(DISPLAY_WIN2 , USER_PREF_INPUT_TYPE_SUBWIN1);
  }
#endif

    g_u8SaveUserData|=SAVE_MONITOR_SETTING; //Jison
    return TRUE;
}




Bool SetHotInputSelect(void)
{
BYTE SourceIndex;

#if Enable_AutoInput_Menu

  if(USER_PREF_INPUT_PRIORITY==g_u8MenuItemIndex)
    return FALSE;
  
    USER_PREF_INPUT_PRIORITY=g_u8MenuItemIndex;

    SourceIndex=Get_SourceSwitchType();
#else
    //g_u8MenuTempValue=Get_SwitchSource(g_u8MenuItemIndex);

  if(USER_PREF_INPUT_TYPE==g_u8MenuItemIndex)
    return FALSE;

    USER_PREF_INPUT_TYPE=g_u8MenuItemIndex;
	
    SourceIndex=USER_PREF_INPUT_TYPE;
#endif	
    g_stMenuFlag.bShowInputIcon=0;	
	
    g_u8SwitchPortCounter = 0; // sync locked, counter reset to 0	

    SwitchPort(USER_PREF_WIN_SEL, SourceIndex);		

    g_u8SaveUserData|=SAVE_MONITOR_SETTING; //Jison
    return TRUE;
}


Bool AdjustInputValue(MENU_ITEM_ACTION enAction)
{
if(enAction==MIA_INC_VALUE)
	enAction=MIA_DEC_VALUE;
else
	enAction=MIA_INC_VALUE;

#if Enable_AutoInput_Menu
    USER_PREF_INPUT_PRIORITY = DecIncValueCirculate(enAction, USER_PREF_INPUT_PRIORITY, INPUT_PRIORITY_AUTO, (INPUT_PRIORITY_NUMS-1), 1);
#else
    USER_PREF_INPUT_TYPE = DecIncValueCirculate(enAction, USER_PREF_INPUT_TYPE, First_Input_Port, (Input_Nums-1), 1);
#endif
    Delay1ms(50);

printData("USER_PREF_INPUT_TYPE==%d",USER_PREF_INPUT_TYPE);

    return TRUE;
}



WORD GetInputValue(void)
{
    return USER_PREF_INPUT_TYPE;
}

#endif

#if USB_Updata_FW_Enable
extern int usb_check(BOOL bFullDL);

WORD GetUSBUpdataFWStatus(void)
{
    return (USBUpdataFwStatus%USB_MAX);
}

Bool SetUSBUpdataFW(void)
{


if(USBUpdataFwStatus==USB_Writting)
    return FALSE;

    USBUpdataFwStatus=USB_Writting;
    usb_check(1);

    return TRUE;
}
#endif



#if ENABLE_RTE  //Steven120511

#if (defined(LeXingEnMa_Project))
   #if (PanelType==PanelCMI28UltraHD)
   BYTE code tODStrength[]={0x00, 0x05, 0x03, 0x01};
   #elif (PanelType==PanelLC430EQE_FHM2)
   BYTE code tODStrength[]={0x00, 0x03, 0x02, 0x01};
   #elif (PanelType==PanelM320QAN01)
   BYTE code tODStrength[]={0x00, 0x03, 0x02, 0x01};
   //BYTE code tODStrength[]={0x00, 0x05, 0x04, 0x03};//20151201 Special Setting
   #else
   BYTE code tODStrength[]={0x00, 0x0F, 0x07, 0x03};
   #endif
#elif (PanelType==PanelM350DVR01_01||PanelType==PanelM350DVA01_1)
BYTE code tODStrength[]={0x00, 0x0F, 0x07, 0x03};
#elif (defined(LianHeChuangXin_Project))
BYTE code tODStrength[]={0x00, 0x0A, 0x07, 0x03};
#elif (PanelType==PanelM320QAN01)
BYTE code tODStrength[]={0x00, 0x0F, 0x0A, 0x04};
#elif (PanelType==PanelLSC400FN04)
BYTE code tODStrength[]={0x00, 0x0F, 0x08, 0x04};
#else
BYTE code tODStrength[]={0x00, 0x0F, 0x08, 0x04};
#endif

void SetOverDriveStrength(BYTE Value)
{    
      msWriteByte(SCL0_16_23, Value);
}

void mStar_SetRTE(void)
{
    BYTE ucStrength = 0x0F;
    
    ucStrength = tODStrength[USER_PREF_RESPONSE_TIME];
    
    
    SetOverDriveStrength(ucStrength);
	
    if (USER_PREF_RESPONSE_TIME!=RT_OFF)
    {
        msAPI_OverDriveEnable(TRUE);
    }
    else
    {
        msAPI_OverDriveEnable(FALSE);
    }
}


Bool SetRTEFunction(void)
{
#if 1
    //if (g_u8MenuTempValue == USER_PREF_RESPONSE_TIME)
   //     return FALSE;
   // USER_PREF_RESPONSE_TIME = g_u8MenuTempValue;

#else
    if (g_u8MenuTempValue==_OFF_ && RTE_ENABLE_FLAG) //Select Off
        CLR_RTE_ENABLE_FLAG();
    else if (g_u8MenuTempValue==_ON_ && (!RTE_ENABLE_FLAG))
        SET_RTE_ENABLE_FLAG();
    else
        return FALSE;
    NVRam_WriteWord(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), MONITOR_FLAGS);
#endif

    mStar_SetRTE();
    Delay1ms(50);

    return FALSE;
}
WORD GetResponseTimeValue(void)
{
#if 1
    return (USER_PREF_RESPONSE_TIME);
#else
    return (RTE_ENABLE_FLAG ? _ON_ : _OFF_);
#endif
}

Bool AdjustResponseTimeValue(MENU_ITEM_ACTION enAction)
{
    USER_PREF_RESPONSE_TIME = DecIncValueCirculate(enAction, USER_PREF_RESPONSE_TIME, RT_OFF, RT_MAX-1, 1);
    Delay1ms(50);
    SetRTEFunction();	
	
    return TRUE;
}


#endif

void SetSharpness(void)
{
    WAIT_V_OUTPUT_BLANKING();
    msAPI_AdjustSharpness(USER_PREF_WIN_SEL, CURRENT_SHARPNESS, ((CURRENT_SHARPNESS>=DEF_SHARPNESS)?1:0));
}

Bool AdjustSharpness(MENU_ITEM_ACTION enAction)
{
    WORD tempValue;

    tempValue=DecIncValue(enAction, CURRENT_SHARPNESS, 0, MAX_SHARPNESS, 1);
    Delay1ms(150);

    if (tempValue==CURRENT_SHARPNESS)
        return FALSE;
    CURRENT_SHARPNESS=tempValue;

    WAIT_V_OUTPUT_BLANKING();
    SetSharpness();

    return TRUE;
}
WORD GetSharpnessValue(void)
{
    return GetScale100Value(CURRENT_SHARPNESS,0,MAX_SHARPNESS);
}





//======================================================================
// OSD Setup Item Define
//======================================================================
Bool AdjustOSDHPosition(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, USER_PREF_OSD_H_POSITION, 0, 100, 1);

    if (u16TempValue == USER_PREF_OSD_H_POSITION)
    {
        return FALSE;
    }
    USER_PREF_OSD_H_POSITION= u16TempValue;
		
    Osd_SetPosition(USER_PREF_OSD_H_POSITION, USER_PREF_OSD_V_POSITION);

    g_u8SaveUserData|=SAVE_MONITOR_SETTING;
    
    return TRUE;
}
WORD GetOSDHPositionValue(void)
{
    return GetScale100Value(USER_PREF_OSD_H_POSITION, 0, 100);
}
//------------------------------------------------------------------------------------------
#if Enable_OSD_Vposition
Bool AdjustOSDVPosition(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, USER_PREF_OSD_V_POSITION, 0, 100, 1);

    if (u16TempValue == USER_PREF_OSD_V_POSITION)
    {
        return FALSE;
    }
    USER_PREF_OSD_V_POSITION= u16TempValue;
    Osd_SetPosition(USER_PREF_OSD_H_POSITION, USER_PREF_OSD_V_POSITION);

    g_u8SaveUserData|=SAVE_MONITOR_SETTING;
    
    return TRUE;
}
WORD GetOSDVPositionValue(void)
{
    return GetScale100Value(USER_PREF_OSD_V_POSITION, 0, 100);
}
#endif
//------------------------------------------------------------------------------------------

Bool AdjustOSDTimer(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncOSDTimerValue(enAction, USER_PREF_OSD_TIME, MIN_OSD_TIMEOUT, MAX_OSD_TIMEOUT, 15);
    if (u16TempValue == USER_PREF_OSD_TIME)
        return FALSE;
    USER_PREF_OSD_TIME = u16TempValue;

    return TRUE;
}
#if 1  //Mike 110524

WORD GetOSDTimerValue(void)
{
    return USER_PREF_OSD_TIME;
}

WORD GetOSDTimerValue100(void)
{
    return GetScale100Value(USER_PREF_OSD_TIME, MIN_OSD_TIMEOUT, 60);;
}
#endif
//------------------------------------------------------------------------------------------
Bool AdjustTransparency(MENU_ITEM_ACTION enAction)
{
    WORD tempValue;

    tempValue=DecIncValue(enAction, USER_PREF_TRANSPARENCY, 0, MAX_TRANSPARENCY, 1);

    if (tempValue==USER_PREF_TRANSPARENCY)
        return FALSE;
    USER_PREF_TRANSPARENCY=tempValue;

#if ZUI
    MApi_GOP_GWIN_SetBlending(MApp_ZUI_API_QueryGWinID(), TRUE, 0x3F-USER_PREF_TRANSPARENCY*8);
    #if (CHIP_ID==MST9U2)
    MApi_GOP_GWIN_SetBlending(MApp_ZUI_API_QueryGWin2ID(), TRUE, 0x3F-USER_PREF_TRANSPARENCY*8);
    #endif
#else
    Osd_SetTransparency(USER_PREF_TRANSPARENCY);
#endif

    Delay1ms(150);
    //SetOSDFlag(OSD_SAVE_SETTING_BIT);
    //g_u8SaveUserData|=SAVE_MONITOR_SETTING; //080123 Removed by Jison. Flag is set at MIA_INC_VALUE/MIA_DEC_VALUE event.
    return TRUE;

}
WORD GetTransparencyValue(void)
{
	return USER_PREF_TRANSPARENCY;
}

WORD GetTransparencyGauge(void)
{
    return GetScale100Value(USER_PREF_TRANSPARENCY,0,MAX_TRANSPARENCY);
}

#if 0  //Mike 110524
WORD InitOne(void)
{
    return 1;
}
#endif
#if ENABLE_OSD_ROTATION
WORD GetMenuRotationValue(void)
{
    return USER_PREF_OSD_ROTATION;
}

Bool SetMenuRotationValue(void)
{
    if (USER_PREF_OSD_ROTATION == g_u8MenuTempValue)
        return FALSE;
    USER_PREF_OSD_ROTATION = g_u8MenuTempValue;


   if(USER_PREF_OSD_ROTATION!=Osd_Rotation_Normal)
       MenuRotate(TRUE);
   else
       MenuRotate(FALSE);

	
    //SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));

    return TRUE;

}


Bool AdjustMenuRotationValue(MENU_ITEM_ACTION enAction)
{
#if 1
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, Osd_Rotation_Normal, Osd_RotationIndex_NUMS-1, 1);
    Delay1ms(50);

#else
     printData("g_u8MenuTempValue==%d", g_u8MenuTempValue);
	
    if (enAction==MIA_INC_VALUE)
    	{
    	if(g_u8MenuTempValue>=1)
        g_u8MenuTempValue=(g_u8MenuTempValue-1)%Osd_RotationIndex_NUMS;
	else 
        g_u8MenuTempValue=(Osd_RotationIndex_NUMS-1);
    	}
    else 
        g_u8MenuTempValue=(g_u8MenuTempValue+1)%Osd_RotationIndex_NUMS;

     printData("g_u8MenuTempValue 00000==%d", g_u8MenuTempValue);
#endif
    return TRUE;

}



#endif
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Bool ClearHoldMenuKeyFlag(void)
{
    g_stMenuFlag.bHoldMenuKey=0;
    return TRUE;
}


void CloseDiagPattern(void)
{
    g_stMenuFlag.bShowDiagPattern=0;
    WAIT_V_OUTPUT_BLANKING();
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
    {
        BYTE u8win;
        for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
            msAPIWinEnableFrameColor(u8win, FALSE);
    }
    else
#endif
    {
        msAPIWinEnableFrameColor(DISPLAY_WIN1,FALSE);
    }
}

extern xdata BYTE g_u8OsdFontColor;
Bool ShowDiagPattern(void)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    BYTE u8win;
#endif
    mStar_SetupFreeRunMode();
    switch(g_u8MenuTempValue)
    {
        case 0:
            {
        #if UseINT
                mStar_EnableModeChangeINT(FALSE, SCALER_MASK_ALL); //Jison 111101
        #endif
            WAIT_V_OUTPUT_BLANKING();
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                    {
                        msAPIWinEnableFrameColor(u8win, TRUE);
                        msAPIWinFrameColorRGB(u8win, 0x6E);
                    }
                }
                else
            #endif
                {
                    msAPIWinEnableFrameColor(DISPLAY_WIN1, TRUE);
                    msAPIWinFrameColorRGB(DISPLAY_WIN1, 0x6E);
                }
            }
            break;
        case 1:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_RED);
                }
                else
            #endif
            msAPI_AdjustBackgoundColor(DISPLAY_WIN1,ePATTERN_COLOR_RED);
            }
            break;
        case 2:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_GREEN);
                }
                else
            #endif
            msAPI_AdjustBackgoundColor(DISPLAY_WIN1,ePATTERN_COLOR_GREEN);
            }
            break;
        case 3:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_BLUE);
                }
                else
            #endif
            msAPI_AdjustBackgoundColor(DISPLAY_WIN1,ePATTERN_COLOR_BLUE);
            }
            break;
        case 4:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_BLACK);
                }
                else
            #endif
                    msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_BLACK);
            }
            break;
        case 5:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_WHITE);
                }
                else
            #endif
            msAPI_AdjustBackgoundColor(DISPLAY_WIN1,ePATTERN_COLOR_WHITE);
            }
            break;
        default:
            CloseDiagPattern();
            Set_InputTimingChangeFlag(SrcInputPortM/*//tmp*/);
            break;
    }
    g_u8MenuTempValue++;
    Delay1ms(50);
    return TRUE;
}




#if ENABLE_NOISE_REDUCTION
void SetNoiseReduction(void)
{
    switch(CURRENT_NOISE_REDUCTION)
    {
  #if 1//(defined(LianHeChuangXin_Project))
        case NR_LOW:
            msPQ_SetGRule_NR(USER_PREF_WIN_SEL,0);
            break;

        case NR_MEDIUM:
            msPQ_SetGRule_NR(USER_PREF_WIN_SEL,0);
            break;

        case NR_HIGH:
            msPQ_SetGRule_NR(USER_PREF_WIN_SEL,0);
            break;
  #else
        case NR_LOW:
            msPQ_SetGRule_NR(USER_PREF_WIN_SEL,1);
            break;

        case NR_MEDIUM:
            msPQ_SetGRule_NR(USER_PREF_WIN_SEL,2);
            break;

        case NR_HIGH:
            msPQ_SetGRule_NR(USER_PREF_WIN_SEL,3);
            break;
  #endif
        case NR_OFF:
        default:
            msPQ_SetGRule_NR(USER_PREF_WIN_SEL,0);
            break;
    }
}
Bool Menu_SetNoiseReduction(void)
{
    if (g_u8MenuTempValue==CURRENT_NOISE_REDUCTION)
        return FALSE;
    CURRENT_NOISE_REDUCTION=g_u8MenuTempValue;

    SetNoiseReduction();

    return TRUE;
}
WORD GetNoiseReductionValue(void)
{
    return CURRENT_NOISE_REDUCTION;
}
Bool AdjustNoiseReductionValue(MENU_ITEM_ACTION enAction)
{
    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, NR_OFF, NR_MAX-1, 1);
    Delay1ms(50);
	Menu_SetNoiseReduction();
    return TRUE;
}
#endif
#if ENABLE_DEINTERLACE
Bool SetDeinterlace(void)
{
    if (g_u8MenuTempValue==_OFF_ && DEINTERLACE_FLAG) //Select Disable
        CLR_DEINTERLACE_FLAG();
    else if (g_u8MenuTempValue==_ON_ && (!DEINTERLACE_FLAG))
        SET_DEINTERLACE_FLAG();
    else
        return FALSE;
    SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_Misc), sizeof(MISC_FLAGS));

    msAPIWinReSetup(USER_PREF_WIN_SEL, FALSE, TRUE);
    APPWindow_SetColorSpace(USER_PREF_WIN_SEL);

    return TRUE;
}
WORD GetDeinterlaceValue(void)
{
    return (DEINTERLACE_FLAG ? _ON_ : _OFF_);
}
#endif
#if ENABLE_DEMURA_FUNCTION
Bool SetUniformityCompensation(void)
{
    if (g_u8MenuTempValue==_OFF_ && UNIFORMITY_COMPENSATION_FLAG) //Select Disable
        CLR_UNIFORMITY_COMPENSATION_FLAG();
    else if (g_u8MenuTempValue==_ON_ && (!UNIFORMITY_COMPENSATION_FLAG))
        SET_UNIFORMITY_COMPENSATION_FLAG();
    else
        return FALSE;
    SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_Misc), sizeof(MISC_FLAGS));

    msAPI_DeMuraEnable(UNIFORMITY_COMPENSATION_FLAG);

    return TRUE;
}
WORD GetUniformityCompensationValue(void)
{
    return (UNIFORMITY_COMPENSATION_FLAG ? _ON_ : _OFF_);
}
#endif
#if ENABLE_DLC
Bool SetDLCFunction(void)
{
    if (g_u8MenuTempValue == USER_PREF_DLC_MODE)
        return FALSE;
    USER_PREF_DLC_MODE = g_u8MenuTempValue;

    if(USER_PREF_DLC_MODE==MODE1_DLC)
        setWinDlcOnOff(ENABLE,DLC_MEDIUM);
    else
        setWinDlcOnOff(DISABLE, DLC_MEDIUM);

   return TRUE;
}
WORD GetDLCValue(void)
{
    return USER_PREF_DLC_MODE;
}
#endif

#if ENABLE_SUPER_RESOLUTION
Bool AdjustSuperResolution(void)
    {
    if (CURRENT_SUPER_RESOLUTION==SUPER_RESOLUTION_OFF)
        CLR_SUPER_RESOLUTION_FLAG();
    else
        SET_SUPER_RESOLUTION_FLAG();

    EN_PQ_SUPER_RESOLUTION_LEVEL    ucSrIdx = ePQ_SR_OFF;

if (CURRENT_SUPER_RESOLUTION == 1)
	ucSrIdx = ePQ_SR_Level_Low;
else if (CURRENT_SUPER_RESOLUTION == 2)
	ucSrIdx = ePQ_SR_Level_Middle;
else if (CURRENT_SUPER_RESOLUTION == 3)
	ucSrIdx = ePQ_SR_Level_High;
else if (CURRENT_SUPER_RESOLUTION == 4)
	ucSrIdx = ePQ_SR_Level_UHD;

    msPQ_SetGRule_SR(USER_PREF_WIN_SEL, ucSrIdx);

    //APPWindow_SetColorSpace(USER_PREF_WIN_SEL);

    MENUFUNC_printData("SR mode:%d", CURRENT_SUPER_RESOLUTION);

    return TRUE;
}

WORD GetSuperResolutionValue(void)
{
    return g_u8OriginalSetting;
}

Bool SetSuperResolutionFunction(void)
{
    Bool Rlt=FALSE;
    if (g_u8OriginalSetting==g_u8MenuTempValue)
    {
        Delay1ms(50);
        return FALSE;
    }

    CURRENT_SUPER_RESOLUTION=g_u8MenuTempValue;

    //WAIT_V_OUTPUT_BLANKING();
    AdjustSuperResolution();

    return Rlt;
}

Bool AdjustSuperResolutionValue(MENU_ITEM_ACTION enAction)
{
    CURRENT_SUPER_RESOLUTION = DecIncValueCirculate(enAction, CURRENT_SUPER_RESOLUTION, SUPER_RESOLUTION_OFF, SUPER_RESOLUTION_NUMS-1, 1);
    //CURRENT_SUPER_RESOLUTION=g_u8MenuTempValue;
 	UserPrefNoiseReduction = CURRENT_SUPER_RESOLUTION;

    //WAIT_V_OUTPUT_BLANKING();
    AdjustSuperResolution();
    return TRUE;
}
#endif

#if 1//(defined(_NEW_CONTRAST_FORMULA_)) //Standard
// Jison 080723
// Can be used for default contrast 50 or 75.
// Can be used for difference MAX_COLOR_VALUE ex:219/255
// Use MAX_CONTRAST_GAIN to decide how much level saturation while contrast is 100.
BYTE GetMainWndRealContrast( BYTE u8DispWin ,BYTE u8Contrast,BYTE u8Color)
{
    u8DispWin = u8DispWin;

    if(u8Contrast>DEF_CONTRAST)
        u8Contrast=(((DWORD)u8Color*((DWORD)MAX_CONTRAST_GAIN*(u8Contrast-DEF_CONTRAST)+(WORD)128*(100-u8Contrast))+(((WORD)(100-DEF_CONTRAST)*MAX_COLOR_VALUE)>>1))/((WORD)(100-DEF_CONTRAST)*MAX_COLOR_VALUE));
#if 1//DELL_VISTA_REQUIREMENT  //XXXXX
    else if(u8Contrast>CONTRAST_VISTA_REQ)
        u8Contrast=(((DWORD)u8Color*((DWORD)128*(u8Contrast-CONTRAST_VISTA_REQ)+(WORD)CONTRAST_GAIN_VISTA_REQ*(DEF_CONTRAST-u8Contrast))+(((WORD)(DEF_CONTRAST-CONTRAST_VISTA_REQ)*MAX_COLOR_VALUE)>>1))/((WORD)(DEF_CONTRAST-CONTRAST_VISTA_REQ)*MAX_COLOR_VALUE));
    else
        u8Contrast=(((DWORD)u8Color*((DWORD)CONTRAST_GAIN_VISTA_REQ*(u8Contrast-MIN_CONTRAST_VALUE)+(WORD)MIN_CONTRAST_GAIN*(CONTRAST_VISTA_REQ-u8Contrast))+(((WORD)(CONTRAST_VISTA_REQ-MIN_CONTRAST_VALUE)*MAX_COLOR_VALUE)>>1))/((WORD)(CONTRAST_VISTA_REQ-MIN_CONTRAST_VALUE)*MAX_COLOR_VALUE));
#else
    else
        u8Contrast=(((DWORD)u8Color*((DWORD)128*(u8Contrast-MIN_CONTRAST_VALUE)+(WORD)MIN_CONTRAST_GAIN*(DEF_CONTRAST-u8Contrast))+(((WORD)(DEF_CONTRAST-MIN_CONTRAST_VALUE)*MAX_COLOR_VALUE)>>1))/((WORD)(DEF_CONTRAST-MIN_CONTRAST_VALUE)*MAX_COLOR_VALUE));
#endif
    //PRINT_DATA("Contrast Gain=%d",u8Contrast);
    return u8Contrast;
}
#endif

//IHC===========================================================================================================

WORD GetCustomHueValue_R(void)
{
    return GetScale100Value(CURRENT_R_HUE, 0, MAX_CUSTOM_HUE_VALUE);
}

Bool AdjustCustomHue_R(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_R_HUE, MIN_CUSTOM_HUE_VALUE, MAX_CUSTOM_HUE_VALUE, 1);

    if (u16TempValue == CURRENT_R_HUE)
    {
        return FALSE;
    }
    CURRENT_R_HUE= u16TempValue;

    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_R);
    return TRUE;
}
WORD GetCustomHueValue_G(void)
{
    return GetScale100Value(CURRENT_G_HUE, 0, MAX_CUSTOM_HUE_VALUE);
}

Bool AdjustCustomHue_G(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_G_HUE, MIN_CUSTOM_HUE_VALUE, MAX_CUSTOM_HUE_VALUE, 1);

    if (u16TempValue == CURRENT_G_HUE)
    {
        return FALSE;
    }
    CURRENT_G_HUE= u16TempValue;

    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_G);
    return TRUE;
}
WORD GetCustomHueValue_B(void)
{
    return GetScale100Value(CURRENT_B_HUE, 0, MAX_CUSTOM_HUE_VALUE);
}

Bool AdjustCustomHue_B(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_B_HUE, MIN_CUSTOM_HUE_VALUE, MAX_CUSTOM_HUE_VALUE, 1);

    if (u16TempValue == CURRENT_B_HUE)
    {
        return FALSE;
    }
    CURRENT_B_HUE= u16TempValue;

    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_B);
    return TRUE;
}

WORD GetCustomHueValue_C(void)
{
    return GetScale100Value(CURRENT_C_HUE, 0, MAX_CUSTOM_HUE_VALUE);
}

Bool AdjustCustomHue_C(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_C_HUE, MIN_CUSTOM_HUE_VALUE, MAX_CUSTOM_HUE_VALUE, 1);

    if (u16TempValue == CURRENT_C_HUE)
    {
        return FALSE;
    }
    CURRENT_C_HUE= u16TempValue;

    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_C);
    return TRUE;
}
WORD GetCustomHueValue_M(void)
{
    return GetScale100Value(CURRENT_M_HUE, 0, MAX_CUSTOM_HUE_VALUE);
}

Bool AdjustCustomHue_M(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_M_HUE, MIN_CUSTOM_HUE_VALUE, MAX_CUSTOM_HUE_VALUE, 1);

    if (u16TempValue == CURRENT_M_HUE)
    {
        return FALSE;
    }
    CURRENT_M_HUE= u16TempValue;

    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_M);
    return TRUE;
}
WORD GetCustomHueValue_Y(void)
{
    return GetScale100Value(CURRENT_Y_HUE, 0, MAX_CUSTOM_HUE_VALUE);
}

Bool AdjustCustomHue_Y(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_Y_HUE, MIN_CUSTOM_HUE_VALUE, MAX_CUSTOM_HUE_VALUE, 1);

    if (u16TempValue == CURRENT_Y_HUE)
    {
        return FALSE;
    }
    CURRENT_Y_HUE= u16TempValue;

    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_Y);
    return TRUE;
}

//ICC===========================================================================================================

WORD GetCustomSATValue_R(void)
{
    return GetScale100Value(CURRENT_R_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE);
}

Bool AdjustCustomSaturation_R(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_R_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE, 1);

    if (u16TempValue == CURRENT_R_SATURATION)
    {
        return FALSE;
    }
    CURRENT_R_SATURATION= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_R);


    //DB_printData("\n eICC_R = 0x%x",eICC_R);

    return TRUE;
}
WORD GetCustomSATValue_G(void)
{
    return GetScale100Value(CURRENT_G_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE);
}


Bool AdjustCustomSaturation_G(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_G_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE, 1);

    if (u16TempValue == CURRENT_G_SATURATION)
    {
        return FALSE;
    }
    CURRENT_G_SATURATION= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_G);

    //DB_printData("\n eICC_R = 0x%x",eICC_G);

    return TRUE;
}
WORD GetCustomSATValue_B(void)
{
    return GetScale100Value(CURRENT_B_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE);
}


Bool AdjustCustomSaturation_B(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_B_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE, 1);

    if (u16TempValue == CURRENT_B_SATURATION)
    {
        return FALSE;
    }
    CURRENT_B_SATURATION= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_B);


    //DB_printData("\n eICC_R = 0x%x",eICC_B);

    return TRUE;
}
WORD GetCustomSATValue_C(void)
{
    return GetScale100Value(CURRENT_C_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE);
}


Bool AdjustCustomSaturation_C(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_C_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE, 1);

    if (u16TempValue == CURRENT_C_SATURATION)
    {
        return FALSE;
    }
    CURRENT_C_SATURATION= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_C);

    //DB_printData("\n eICC_R = 0x%x",eICC_C);

    return TRUE;
}
WORD GetCustomSATValue_M(void)
{
    return GetScale100Value(CURRENT_M_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE);
}


Bool AdjustCustomSaturation_M(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_M_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE, 1);

    if (u16TempValue == CURRENT_M_SATURATION)
    {
        return FALSE;
    }
    CURRENT_M_SATURATION= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_M);


    //DB_printData("\n eICC_R = 0x%x",eICC_M);

    return TRUE;
}
WORD GetCustomSATValue_Y(void)
{
    return GetScale100Value(CURRENT_Y_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE);
}


Bool AdjustCustomSaturation_Y(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_Y_SATURATION, MIN_CUSTOM_SATURATION_VALUE, MAX_CUSTOM_SATURATION_VALUE, 1);

    if (u16TempValue == CURRENT_Y_SATURATION)
    {
        return FALSE;
    }
    CURRENT_Y_SATURATION= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_Y);

    //DB_printData("\n eICC_R = 0x%x",eICC_Y);

    return TRUE;
}

//==============================================================================================================
WORD GetCustomColorValue_R(void)
{
if(CURRENT_COLOR_TEMP==PCT_USER)//PCT_Advanced
    return GetScale100Value(CURRENT_Color_R, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE);
else
    return 50;	
}


Bool AdjustCustomColor_R(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_Color_R, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE, 1);

    if (u16TempValue == CURRENT_Color_R)
    {
        return FALSE;
    }
    CURRENT_Color_R= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_R);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_R);

    return TRUE;
}

WORD GetCustomColorValue_G(void)
{
if(CURRENT_COLOR_TEMP==PCT_USER)//PCT_Advanced
    return GetScale100Value(CURRENT_Color_G, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE);
else
    return 50;	
}

Bool AdjustCustomColor_G(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_Color_G, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE, 1);

    if (u16TempValue == CURRENT_Color_G)
    {
        return FALSE;
    }
    CURRENT_Color_G= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_G);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_G);

    return TRUE;
}

WORD GetCustomColorValue_B(void)
{
if(CURRENT_COLOR_TEMP==PCT_USER)//PCT_Advanced
    return GetScale100Value(CURRENT_Color_B, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE);
else
    return 50;	
}

Bool AdjustCustomColor_B(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_Color_B, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE, 1);

    if (u16TempValue == CURRENT_Color_B)
    {
        return FALSE;
    }
    CURRENT_Color_B= u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_B);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_B);

    return TRUE;
}

WORD GetCustomColorValue_C(void)
{
if(CURRENT_COLOR_TEMP==PCT_USER)//PCT_Advanced
    return GetScale100Value(CURRENT_Color_C, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE);
else
    return 50;	
}

Bool AdjustCustomColor_C(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_Color_C, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE, 1);

    if (u16TempValue == CURRENT_Color_C)
    {
        return FALSE;
    }
    CURRENT_Color_C= u16TempValue;
    u16TempValue=100-u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_C);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_C);

    return TRUE;
}

WORD GetCustomColorValue_M(void)
{
if(CURRENT_COLOR_TEMP==PCT_USER)//PCT_Advanced
    return GetScale100Value(CURRENT_Color_M, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE);
else
    return 50;	
}

Bool AdjustCustomColor_M(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_Color_M, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE, 1);

    if (u16TempValue == CURRENT_Color_M)
    {
        return FALSE;
    }
    CURRENT_Color_M= u16TempValue;
    u16TempValue=100-u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_M);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_M);

    return TRUE;
}

WORD GetCustomColorValue_Y(void)
{
if(CURRENT_COLOR_TEMP==PCT_USER)//PCT_Advanced
    return GetScale100Value(CURRENT_Color_Y, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE);
else
    return 50;	
}


Bool AdjustCustomColor_Y(MENU_ITEM_ACTION enAction)
{
    WORD    u16TempValue;
    u16TempValue = DecIncValue(enAction, CURRENT_Color_Y, MIN_CUSTOM_COLOR_VALUE, MAX_CUSTOM_COLOR_VALUE, 1);

    if (u16TempValue == CURRENT_Color_Y)
    {
        return FALSE;
    }
    CURRENT_Color_Y= u16TempValue;

    u16TempValue=100-u16TempValue;

    msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(u16TempValue), eICC_Y);
    msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(u16TempValue), eIHC_Y);


    return TRUE;
}

//==============================================================================================================
#if ENABLE_DEMURA_FUNCTION
void EnterDeMuraMeasureMode(void)
{
    //MENUFUNC_printMsg("Enter DeMura Measure Mode ... \n");
  #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
    {
        BYTE u8win;
        for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
        {
            msAPIWinEnableFrameColor(u8win, TRUE);
            msAPI_AdjustBackgoundColor(u8win, (BGColorType)0x00);
        }
        for(u8win=0; u8win<DISPLAY_WIN3; u8win++)
            msAPIWinReSetup(u8win, FALSE, TRUE);
    }
    else
  #endif
    {
        msAPIWinEnableFrameColor(DISPLAY_WIN1, TRUE);
        msAPI_AdjustBackgoundColor(DISPLAY_WIN1, (BGColorType)0x00);
    }
}
void CloseDeMuraMeasureMode(void)
{
    //MENUFUNC_printMsg("Close DeMura Measure Mode ... \n");
    Menu_Hide();
    InitOsdAfterLogo();
    g_u8MenuPageIndex=MENU_ROOT;
  #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
    {
        BYTE u8win;
        for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
        {
            msAPIWinEnableFrameColor(u8win, FALSE);
        }
    }
    else
  #endif
    {
        msAPIWinEnableFrameColor(DISPLAY_WIN1, FALSE);
    }
}
#endif


Bool AdjustResetValue(MENU_ITEM_ACTION enAction)
{
    enAction = enAction;
    g_u8MenuTempValue = (g_u8MenuTempValue) ? (0) : (1);
    Delay1ms(50);

   printData("g_u8MenuTempValue==%d",g_u8MenuTempValue);
	
    return TRUE;
}


#if ENABLE_JUMP
static void msComboGPIOConfig(BYTE u8ComboPort, BYTE u8Channel, BYTE u8PadType, Bool u8GPIOType)//type:0->output, 1->input
{
    DWORD u32BankOffset = u8ComboPort << 8;

    msWriteBit(REG_1120F2 + u32BankOffset, !u8GPIOType, (BIT0<<u8Channel)<<(u8PadType*4));//OEZ
}

static void msComboGPIOOutputConfig(BYTE u8ComboPort, BYTE u8Channel, BYTE u8OutputVal)
{
    DWORD u32BankOffset = u8ComboPort << 8;

    msWriteBit(REG_1120F3 + u32BankOffset, u8OutputVal, BIT0<<u8Channel);//OEZ
}

static Bool msComboGPIOInput(BYTE u8ComboPort, BYTE u8Channel, BYTE u8PadType)
{
    DWORD u32BankOffset = u8ComboPort << 8;

    return ((msReadByte(REG_1120F1 + u32BankOffset)>>(u8Channel+u8PadType*4))&BIT0);
}

static void msComboPNShort(BYTE u8ComboPort, BYTE u8Channel, Bool bEnable)
{
    DWORD u32BankOffset = u8ComboPort << 8;

    msWriteBit(REG_1120F3 + u32BankOffset, bEnable, BIT4<<u8Channel);
}

static void msComboOpenShortTest(BYTE u8ComboPort, BYTE u8ComboType)
{
    BYTE u8TestFail, u8TestFailShort, u8ComboGpioOffset=u8ComboPort*4;
    BYTE i,j,k,m,n;
    BYTE u8Channel,u8ChannelTmp;
    BYTE u8ShortChannel,u8ShortChannelTmp;
    BYTE u8ComboPortDual = u8ComboPort;
    BYTE u8ComboPortChannel, u8ComboPortShortChannel;

    WORD u16REG_000406 =  msRead2Byte(REG_000406);//Backup CEC
    WORD u16REG_000416 =  msRead2Byte(REG_000416);//Backup CEC
    WORD u16REG_000462 =  msRead2Byte(REG_000462);//Backup Pad pull high
    BYTE u16REG_000420 =  msReadByte(_REG_GPIO0_OUT);//Backup GPIO
    BYTE u16REG_000422 =  msReadByte(_REG_GPIO0_OEZ);//Backup GPIO
    BYTE u16REG_000426 =  msReadByte(_REG_GPIO1_OUT);//Backup GPIO
    BYTE u16REG_000428 =  msReadByte(_REG_GPIO1_OEZ);//Backup GPIO
    BYTE u16REG_00042C =  msReadByte(_REG_GPIO2_OUT);//Backup GPIO
    BYTE u16REG_00042E =  msReadByte(_REG_GPIO2_OEZ);//Backup GPIO
    BYTE u16REG_000434 =  msReadByte(_REG_GPIO4_OUT);//Backup GPIO
    BYTE u16REG_000436 =  msReadByte(_REG_GPIO4_OEZ);//Backup GPIO
    WORD u16REG_00043A =  msRead2Byte(_REG_GPIO5_OUT);//Backup GPIO
    WORD u16REG_00043D =  msRead2Byte(_REG_GPIO6_OUT);//Backup GPIO
    WORD u16REG_000480 =  msRead2Byte(_REG_GPIO_X0_OUT);//Backup GPIO
    WORD u16REG_000484 =  msRead2Byte(_REG_GPIO_X1_OUT);//Backup GPIO
    WORD u16REG_000488 =  msRead2Byte(_REG_GPIO_X2_OUT);//Backup GPIO
    WORD u16REG_00048C =  msRead2Byte(_REG_GPIO_X3_OUT);//Backup GPIO
    WORD u16REG_000490 =  msRead2Byte(_REG_GPIO_X4_OUT);//Backup GPIO
    WORD u16REG_000494 =  msRead2Byte(_REG_GPIO_X5_OUT);//Backup GPIO
    DWORD u16REG_1120F0=  msRead4Byte(REG_1120F0+(u8ComboPort<<8));//Backup

    // Start 1, Start2
    for(i=0;i<2;i++)
    {
        u8TestFail = 0;
        u8TestFailShort = 0;
        u8Channel = i?LAN3:LAN0;
        u8ShortChannel = i?LAN1:LAN2;
        // check open
        for(j=0;j<2;j++)//Two step process for u8Channel <-->u8ShortChannel
        {
            u8ChannelTmp = (j)?u8ShortChannel:u8Channel;
            u8ShortChannelTmp = (j)?u8Channel:u8ShortChannel;
            u8ComboPortChannel = (u8ChannelTmp==LAN0)?u8ComboPort:u8ComboPortDual;
            u8ComboPortShortChannel= (u8ShortChannelTmp==LAN0)?u8ComboPort:u8ComboPortDual;

            for(k=0;k<2;k++)//Two step process for RX_P <-->RX_N
            {
                if(u8TestFail)
                {
                    break;
                }
                msComboGPIOConfig(u8ComboPortChannel, u8ChannelTmp, (k)?RX_P:RX_N, GPIO_OUT);
                msComboGPIOConfig(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P, GPIO_IN);
                msComboGPIOConfig(u8ComboPortShortChannel, u8ShortChannelTmp, RX_N, GPIO_IN);
                msComboGPIOConfig(u8ComboPortShortChannel, u8ShortChannelTmp, RX_P, GPIO_IN);
                msComboPNShort(u8ComboPortChannel, u8ChannelTmp, _DISABLE);
                msComboPNShort(u8ComboPortShortChannel, u8ShortChannelTmp, _ENABLE);
#if (OPEN_SHORT_MODE == CAPACITANCE_MODE)
                for(m=0;m<CAP_MODE_RTY_CNT;m++)
                {
                    msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _HIGH);
                    DelayUs(50);
                    msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _LOW);
                    DelayUs(50);
                    if ( (msComboGPIOInput(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P)==_LOW)
                       &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, RX_N)==_LOW)
                       &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, RX_P)==_LOW) )
                    {
                        for(n=0;n<CAP_MODE_RTY_CNT;n++)
                        {
                            msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _LOW);
                            DelayUs(50);
                            msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _HIGH);
                            DelayUs(50);
                            if ( (msComboGPIOInput(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P)==_HIGH)
                               &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, RX_N)==_HIGH)
                               &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, RX_P)==_HIGH) )
                            {
                                break;
                            }
                            ForceDelay1ms(1);
                        }
                        break;
                    }
                    ForceDelay1ms(1);
                }

                if( (m>=CAP_MODE_RTY_CNT) || (n>=CAP_MODE_RTY_CNT) )
                {
                        u8TestFail = 1;
                        MENUFUNC_printData("Open/Short Testing %d Fail---Open",(i+1));
                }
#else // resistor mode
                msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _HIGH);
                DelayUs(50);
                msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _LOW);
                DelayUs(50);
                if( (msComboGPIOInput(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P)==_LOW)
                  &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, RX_N)==_LOW)
                  &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, RX_P)==_LOW) )
                {
                    msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _LOW);
                    DelayUs(50);
                    msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _HIGH);
                    DelayUs(50);
                    if( (msComboGPIOInput(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P)==_LOW)
                      ||(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, RX_N)==_LOW)
                      ||(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, RX_P)==_LOW) )
                    {
                        u8TestFail = 1;
                        MENUFUNC_printData("Open/Short Testing %d Fail---Open",(i+1));
                    }
                }
                else
                {
                    u8TestFail = 1;
                    MENUFUNC_printData("Open/Short Testing %d Fail---Open",(i+1));
                }
#endif
            }
        }

        // check short
        for(j=0;j<2;j++)//Two step process for u8Channel <-->u8ShortChannel
        {
            if(u8TestFail)
            {
                break;
            }
            u8ChannelTmp = (j)?u8ShortChannel:u8Channel;
            u8ShortChannelTmp = (j)?u8Channel:u8ShortChannel;
            u8ComboPortChannel = (u8ChannelTmp==LAN0)?u8ComboPort:u8ComboPortDual;
            u8ComboPortShortChannel= (u8ShortChannelTmp==LAN0)?u8ComboPort:u8ComboPortDual;
            for(k=0;k<2;k++)//Two step process for RX_P <-->RX_N
            {
                msComboGPIOConfig(u8ComboPortChannel, u8ChannelTmp, (k)?RX_P:RX_N, GPIO_OUT);
                msComboGPIOConfig(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P, GPIO_IN);
                msComboGPIOConfig(u8ComboPortShortChannel, u8ShortChannelTmp, RX_N, GPIO_IN);
                msComboGPIOConfig(u8ComboPortShortChannel, u8ShortChannelTmp, RX_P, GPIO_IN);

                msComboPNShort(u8ComboPortChannel, u8ChannelTmp, DISABLE);
                msComboPNShort(u8ComboPortShortChannel, u8ShortChannelTmp, DISABLE);

#if (OPEN_SHORT_MODE == CAPACITANCE_MODE)
                for(m=0;m<CAP_MODE_RTY_CNT;m++)
                {
                    msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _HIGH);
                    DelayUs(50);
                    msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _LOW);
                    DelayUs(50);
                    if ( (msComboGPIOInput(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P)==_LOW)
                       &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, (k)?RX_P:RX_N)==_LOW) )
                    {
                        for(n=0;n<CAP_MODE_RTY_CNT;n++)
                        {
                            msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _LOW);
                            DelayUs(50);
                            msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _HIGH);
                            DelayUs(50);
                            if ( (msComboGPIOInput(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P)==_HIGH)
                               &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, (k)?RX_P:RX_N)==_HIGH) )
                            {
                                u8TestFailShort++;
                                break;
                            }
                            ForceDelay1ms(1);
                        }
                        break;
                    }
                    ForceDelay1ms(1);
                }

                if( (m>=CAP_MODE_RTY_CNT) || (n>=CAP_MODE_RTY_CNT) )
                {
                    break;
                }
#else // resistor mode
                msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _HIGH);
                DelayUs(50);
                msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _LOW);
                DelayUs(50);
                if( (msComboGPIOInput(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P)==_LOW)
                  &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, (k)?RX_P:RX_N)==_LOW) )
                {
                    msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _LOW);
                    DelayUs(50);
                    msComboGPIOOutputConfig(u8ComboPortChannel, u8ChannelTmp, _HIGH);
                    DelayUs(50);
                    if( (msComboGPIOInput(u8ComboPortChannel, u8ChannelTmp, (k)?RX_N:RX_P)==_HIGH)
                      &&(msComboGPIOInput(u8ComboPortShortChannel, u8ShortChannelTmp, (k)?RX_P:RX_N)==_HIGH) )
                    {
                        u8TestFailShort++;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
#endif
            }
            if(u8TestFailShort<((j+1)*2))
            {
                break;
            }
        }

        if(u8TestFailShort >= 4)
        {
            u8TestFail = 1;
            MENUFUNC_printData("Open/Short Testing %d Fail---Short",(i+1));
        }
        else if(u8TestFail==0)
        {
            MENUFUNC_printData("Open/Short Testing %d Pass",(i+1));
        }

        if( (i==1) && (u8ComboType == DVID) && (u8ComboPortDual==u8ComboPort) )
        {
            MENUFUNC_printMsg("Open/Short Testing DVI-Dual");
            u8ComboPortDual = (u8ComboPort%2)?(u8ComboPort-1):(u8ComboPort+1);
            i=0xFF;
            continue;
        }
    }

    //Start3
    msWrite2Byte(REG_000462, 0xFFFF); // Disable pad pull-high
    msWrite2ByteMask(REG_000406, 0, BIT11|BIT4); // Disable CEC function.
    msWrite2ByteMask(REG_000416, 0, BIT9|BIT8); // Disable CEC function.

    if( (u8ComboType == TMDS) || (u8ComboType == DVID) ) // HPD -> CEC
    {
        if(u8ComboPort == (MUX_COMBO2-1)) //GPIOX22, GPIO26
        {
            msWriteBit(_REG_GPIO_X0_OEZ+u8ComboGpioOffset, 0, BIT2);  // HPD GPIOX22 output mode.
            msWriteBit(_REG_GPIO_X0_OUT+u8ComboGpioOffset, 1, BIT2);  // HPD GPIOX22 output 1.
            msWriteBit(_REG_GPIO2_OEZ, 1, BIT6);  // CEC GPIO26 input mode.

            // Is HPD-CEC short?
            ForceDelay1ms(2);
            if ( msReadByte(_REG_GPIO2_IN) & BIT6 )
            {
                msWriteBit(_REG_GPIO_X0_OUT+u8ComboGpioOffset, 0, BIT2);  // HPD GPIOX22 output 0.
                ForceDelay1ms(2);
                if ( !(msReadByte(_REG_GPIO2_IN) & BIT6) )
                {
                    MENUFUNC_printMsg("Open/Short Testing 3:HPD->CEC - Short");
                }
                else
                {
                    MENUFUNC_printMsg("Open/Short Testing 3:HPD->CEC - Open");
                }
            }
            else
            {
                MENUFUNC_printMsg("Open/Short Testing 3:HPD->CEC - Open");
            }
        }
        else if(u8ComboPort == (MUX_COMBO3-1)) //GPIOX32, GPIO23
        {
            msWriteBit(_REG_GPIO_X0_OEZ+u8ComboGpioOffset, 0, BIT2);  // HPD GPIOX32 output mode.
            msWriteBit(_REG_GPIO_X0_OUT+u8ComboGpioOffset, 1, BIT2);  // HPD GPIOX32 output 1.
            msWriteBit(_REG_GPIO2_OEZ, 1, BIT3);  // CEC GPIO23 input mode.

            // Is HPD-CEC short?
            ForceDelay1ms(2);
            if ( msReadByte(_REG_GPIO2_IN) & BIT3 )
            {
                msWriteBit(_REG_GPIO_X0_OUT+u8ComboGpioOffset, 0, BIT2);  // HPD GPIOX22 output 0.
                ForceDelay1ms(2);
                if ( !(msReadByte(_REG_GPIO2_IN) & BIT3) )
                {
                    MENUFUNC_printMsg("Open/Short Testing 3:HPD->CEC - Short");
                }
                else
                {
                    MENUFUNC_printMsg("Open/Short Testing 3:HPD->CEC - Open");
                }
            }
            else
            {
                MENUFUNC_printMsg("Open/Short Testing 3:HPD->CEC - Open");
            }
        }
        else if(u8ComboPort == (MUX_COMBO5-1)) //GPIOX42, NO_CEC
        {
            MENUFUNC_printMsg("Open/Short Testing 3:HPD->CEC - N.A.");
        }
        else
        {
            MENUFUNC_printMsg("Open/Short Testing 3:HPD->CEC Please Implement for New Board");
        }
    }
    else // HPD -> DP_PWR
    {
        MENUFUNC_printMsg("Open/Short Testing 3:HPD->DP_PWR - N.A.");
    }

    //Start4
    if( (u8ComboType == TMDS) || (u8ComboType == DVID) ) // Jig -> 5V (5V detect)
    {
        switch(u8ComboPort)
        {
            case (MUX_COMBO2-1):
            case (MUX_COMBO3-1):
                msWriteBit(_REG_GPIO_X0_OEZ+u8ComboGpioOffset, 1, BIT1);  // HPD GPIOXn1 input mode.
                // Is 5V Det?
                ForceDelay1ms(2);
                if ( msReadByte(_REG_GPIO_X0_IN+u8ComboGpioOffset) & BIT1 )
                {
                    MENUFUNC_printMsg("Open/Short Testing 4:Jig->5V - Det High");
                }
                else
                {
                    MENUFUNC_printMsg("Open/Short Testing 4:Jig->5V - Det Low");
                }
                break;

            default:
                MENUFUNC_printMsg("Open/Short Testing 4:Jig->5V - N.A.");
                break;
        }
    }
    else // AUXP -> AUXN
    {
        WORD u16REG_0011A8 =  msRead2Byte(REG_0011A8+(u8ComboPort<<8));//Backup Aux

        msWrite2Byte(REG_0011A8+(u8ComboPort<<8), 0x3040);
        msWriteByteMask( _REG_GPIO_X0_OEZ+u8ComboGpioOffset, BIT3, BIT4|BIT3);
        ForceDelay1ms(10);
        for(m=0;m<CAP_MODE_RTY_CNT;m++)
        {
            msWriteByteMask( _REG_GPIO_X0_OUT+u8ComboGpioOffset, BIT4, BIT4);
            ForceDelay1ms(2);
            msWriteByteMask( _REG_GPIO_X0_OUT+u8ComboGpioOffset, 0, BIT4);
            if ((msReadByte(_REG_GPIO_X0_IN+u8ComboGpioOffset)&BIT3)==0)
            {
                for(n=0;n<CAP_MODE_RTY_CNT;n++)
                {
                    msWriteByteMask( _REG_GPIO_X0_OUT+u8ComboGpioOffset, 0, BIT4);
                    ForceDelay1ms(2);
                    msWriteByteMask( _REG_GPIO_X0_OUT+u8ComboGpioOffset, BIT4, BIT4);
                    if ((msReadByte(_REG_GPIO_X0_IN+u8ComboGpioOffset)&BIT3)==BIT3)
                    {
                        break;
                    }
                    ForceDelay1ms(1);
                }
                break;
            }
            ForceDelay1ms(1);
        }

        if( (m<CAP_MODE_RTY_CNT) && (n<CAP_MODE_RTY_CNT) )
        {
            msWriteByteMask( _REG_GPIO_X0_OEZ+u8ComboGpioOffset, BIT4, BIT4|BIT3);
            ForceDelay1ms(10);
            for(m=0;m<CAP_MODE_RTY_CNT;m++)
            {
                msWriteByteMask( _REG_GPIO_X0_OUT+u8ComboGpioOffset, BIT3, BIT3);
                ForceDelay1ms(2);
                msWriteByteMask( _REG_GPIO_X0_OUT+u8ComboGpioOffset, 0, BIT3);
                if ((msReadByte(_REG_GPIO_X0_IN+u8ComboGpioOffset)&BIT4)==0)
                {
                    for(n=0;n<CAP_MODE_RTY_CNT;n++)
                    {
                        msWriteByteMask( _REG_GPIO_X0_OUT+u8ComboGpioOffset, 0, BIT3);
                        ForceDelay1ms(2);
                        msWriteByteMask( _REG_GPIO_X0_OUT+u8ComboGpioOffset, BIT3, BIT3);
                        if ((msReadByte(_REG_GPIO_X0_IN+u8ComboGpioOffset)&BIT4)==BIT4)
                        {
                            break;
                        }
                        ForceDelay1ms(1);
                    }
                    break;
                }
                ForceDelay1ms(1);
            }

            if( (m<CAP_MODE_RTY_CNT) && (n<CAP_MODE_RTY_CNT) )
            {
                MENUFUNC_printMsg("Open/Short Testing 4:AUXP->AUXN - Short");
            }
            else
            {
                MENUFUNC_printMsg("Open/Short Testing 4:AUXP->AUXN - Open");
            }
        }
        else
        {
            MENUFUNC_printMsg("Open/Short Testing 4:AUXP->AUXN - Open");
        }
        msWrite2Byte(REG_0011A8+(u8ComboPort<<8), u16REG_0011A8); // Restore Aux
    }

    // Restore GPIO settings
    msWrite2Byte(REG_000406, u16REG_000406);
    msWrite2Byte(REG_000416, u16REG_000416);
    msWrite2Byte(REG_000462, u16REG_000462);
    msWriteByte(_REG_GPIO0_OUT, u16REG_000420);
    msWriteByte(_REG_GPIO0_OEZ, u16REG_000422);
    msWriteByte(_REG_GPIO1_OUT, u16REG_000426);
    msWriteByte(_REG_GPIO1_OEZ, u16REG_000428);
    msWriteByte(_REG_GPIO2_OUT, u16REG_00042C);
    msWriteByte(_REG_GPIO2_OEZ, u16REG_00042E);
    msWriteByte(_REG_GPIO4_OUT, u16REG_000434);
    msWriteByte(_REG_GPIO4_OEZ, u16REG_000436);
    msWrite2Byte(_REG_GPIO5_OUT, u16REG_00043A);
    msWrite2Byte(_REG_GPIO6_OUT, u16REG_00043D);
    msWrite2Byte(_REG_GPIO_X0_OUT, u16REG_000480);
    msWrite2Byte(_REG_GPIO_X1_OUT, u16REG_000484);
    msWrite2Byte(_REG_GPIO_X2_OUT, u16REG_000488);
    msWrite2Byte(_REG_GPIO_X3_OUT, u16REG_00048C);
    msWrite2Byte(_REG_GPIO_X4_OUT, u16REG_000490);
    msWrite2Byte(_REG_GPIO_X5_OUT, u16REG_000494);
    msWrite4Byte(REG_1120F0+(u8ComboPort<<8), u16REG_1120F0);
}

static void msVGAOpenShortTest(void)
{
    BYTE i,result=0, resultTimeout=0;
    WORD u16Timeout;
    BYTE u8SCL0_13_42 = msReadByte(SCL0_13_42);
    BYTE u8REG_0003A6 = msReadByte(REG_0003A6);
    WORD u16REG_102508= msRead2Byte(REG_102508);
    BYTE u8REG_10250C = msReadByte(REG_10250C);
    WORD u16REG_102652= msRead2Byte(REG_102652);
    WORD u16REG_102666= msRead2Byte(REG_102666);
    WORD u16REG_10267A= msRead2Byte(REG_10267A);
    BYTE u8REG_102565 = msReadByte(REG_102565);
    BYTE u8REG_10256C = msReadByte(REG_10256C);
    BYTE u8REG_000434 = msReadByte(REG_000434);
    BYTE u8REG_000436 = msReadByte(REG_000436);
    BYTE u8REG_10254E = msReadByte(REG_10254E);
    BYTE u8SCL0_13_48 = msReadByte(SCL0_13_48);
    BYTE u8REG_102E03 = msReadByte(REG_102E03);
    BYTE u8SCL0_13_06 = msReadByte(SCL0_13_06);
    BYTE u8SCL0_13_04 = msReadByte(SCL0_13_04);


    msWriteByteMask(SCL0_13_42, _BIT2, _BIT5|_BIT4|_BIT3|_BIT2); // prevent HV sync status from being forced
    msWriteByteMask(REG_000436,0x00,BIT0);
    msWriteByteMask(REG_000434,0x00,BIT0); //  force GPIO=0

    // Check SCL -> R/G/B
    msWriteByte(REG_0003A6,0x0E);
    ForceDelay1ms(10);
    msWrite2Byte(REG_102508,0x0000);
    msWriteByte(REG_10250C,0x00);
    msWrite2Byte(REG_102652,0x01FC);
    msWrite2Byte(REG_102666,0x01FC);
    msWrite2Byte(REG_10267A,0x01FC);
    msWriteByteMask(REG_102565,0x0C,0x0C); // force Vclamp
    msWriteByteMask(REG_10256C,0x10,0x18); // force Vclamp=0.6V

    u16Timeout = 3000;
    msWriteByteMask(REG_10254E,BIT1,BIT1); // read ADC data on
    ForceDelay1ms(2); // wait register ok
    // wait latch value reset to low
    while(((msRead2Byte(REG_102550)>0x30) || (msRead2Byte(REG_102552)>0x30) || (msRead2Byte(REG_102554)>0x30)) && (--u16Timeout))
    {
        ForceDelay1ms(8);
        msWriteByteMask(REG_10254E,BIT1,BIT1); // read ADC data on
        ForceDelay1ms(2); // wait register ok
    }

    // check R/G/B timeout -> should not happen or open
    if(u16Timeout==0)
    {
        for(i=0;i<3;i++)
        {
            if(msRead2Byte(REG_102550+i*2)>0x30)
                resultTimeout |= (BIT0<<i);
        }
    }
    msWrite2Byte(REG_102508,0x0400); // iclp_rgb
    msWriteByteMask(REG_000434,BIT0,BIT0); //  force GPIO=x
    ForceDelay1ms(2);
    msWriteByteMask(REG_10254E,BIT1,BIT1); // read ADC data on
    ForceDelay1ms(2); // wait register ok
    for(i=0;i<3;i++)
    {
        if((msRead2Byte(REG_102550+i*2)>0x100) && !(resultTimeout&(BIT0<<i)))
            result |= (BIT0<<i);
    }

    // Check SDA -> HSYNC
    msWriteByte(SCL0_13_48,0x00);
    msWriteByte(SCL0_13_04,BIT5);
    msWriteByte(REG_102E03,0x00); // ip mux
    msWriteByteMask(REG_000436,0x00,BIT1);
    msWriteByteMask(REG_000434,0x00,BIT1);
    ForceDelay1ms(10);
    if((msReadByte(SCL0_13_3C)&BIT3) == 0)
    {
        msWriteByteMask(REG_000436,0x00,BIT1);
        msWriteByteMask(REG_000434,BIT1,BIT1);
        ForceDelay1ms(10);
        if(msReadByte(SCL0_13_3C)&BIT3)
            result |= BIT3;
    }

    // Check SDA -> VSYNC
    msWriteByteMask(SCL0_13_06,BIT5,BIT5);
    msWriteByteMask(REG_000436,0x00,BIT1);
    msWriteByteMask(REG_000434,0x00,BIT1);
    ForceDelay1ms(10);
    if((msReadByte(SCL0_13_3C)&BIT2) == 0)
    {
        msWriteByteMask(REG_000436,0x00,BIT1);
        msWriteByteMask(REG_000434,BIT1,BIT1);
        ForceDelay1ms(10);
        if(msReadByte(SCL0_13_3C)&BIT2)
            result |= BIT4;
    }

    // restore setting
    msWriteByte(SCL0_13_42, u8SCL0_13_42);
    msWriteByte(REG_0003A6, u8REG_0003A6);
    msWrite2Byte(REG_102508, u16REG_102508);
    msWriteByte(REG_10250C, u8REG_10250C);
    msWrite2Byte(REG_102652, u16REG_102652);
    msWrite2Byte(REG_102666, u16REG_102666);
    msWrite2Byte(REG_10267A, u16REG_10267A);
    msWriteByte(REG_102565, u8REG_102565);
    msWriteByte(REG_10256C, u8REG_10256C);
    msWriteByte(REG_000434, u8REG_000434);
    msWriteByte(REG_000436, u8REG_000436);
    msWriteByte(REG_10254E, u8REG_10254E);
    msWriteByte(SCL0_13_48, u8SCL0_13_48);
    msWriteByte(REG_102E03, u8REG_102E03);
    msWriteByte(SCL0_13_06, u8SCL0_13_06);
    msWriteByte(SCL0_13_04, u8SCL0_13_04);

    // printf test result
    if(result & BIT0) // SCL -> R
    {
        MENUFUNC_printMsg("Open/Short Testing SCL->R - Short");
    }
    else
    {
        MENUFUNC_printMsg("Open/Short Testing SCL->R - Open");
    }
    if(result & BIT1) // SCL -> G
    {
        MENUFUNC_printMsg("Open/Short Testing SCL->G - Short");
    }
    else
    {
        MENUFUNC_printMsg("Open/Short Testing SCL->G - Open");
    }
    if(result & BIT2) // SCL -> B
    {
        MENUFUNC_printMsg("Open/Short Testing SCL->B - Short");
    }
    else
    {
        MENUFUNC_printMsg("Open/Short Testing SCL->B - Open");
    }
    if(result & BIT3) // SDA -> HSYNC
    {
        MENUFUNC_printMsg("Open/Short Testing SDA->HSYNC - short");
    }
    else
    {
        MENUFUNC_printMsg("Open/Short Testing SDA->HSYNC - Open");
    }
    if(result & BIT4) // SDA -> VSYNC
    {
        MENUFUNC_printMsg("Open/Short Testing SDA->VSYNC - short");
    }
    else
    {
        MENUFUNC_printMsg("Open/Short Testing SDA->VSYNC - Open");
    }
}
void msInputOpenShortTest(void)
{
    BYTE u8REG_001781 = msReadByte(REG_001781);//Backup Rterm
    WORD u8REG_0017AC = msRead2Byte(REG_0017AC);//Backup
    WORD u8REG_0017A2 = msRead2Byte(REG_0017A2);//Backup Rterm
    WORD u8REG_0017C4 = msRead2Byte(REG_0017C4);//Backup Rterm
    BYTE InputPort;

    msWriteByteMask(REG_001781, 0x3F, 0x3F);//Turn off Rtem
    msWrite2Byte(REG_0017AC, 0x555);//Test settings
    msWrite2Byte(REG_0017A2, 0xFFF);//Turn off Rtem
    msWrite2Byte(REG_0017C4, 0xFFFF);//Turn off Rtem

    for( InputPort=First_Input_Port; InputPort < Input_Nums; InputPort++ )
    {
        if(g_InputPort[InputPort].eIPMux==MUX_ADC)//ADC
        {
            MENUFUNC_printMsg("***** VGA Connector Test Start *****");
            MENUFUNC_printMsg("=====================================");
            msVGAOpenShortTest();
            MENUFUNC_printMsg("=====================================\n");
        }
        else // Combo - TMDS/DP
        {
            MENUFUNC_printData("***** Combo%d Connector Test Start *****",g_InputPort[InputPort].eIPMux-1);
            MENUFUNC_printMsg("=====================================");
            if( g_InputPort[InputPort].eIPType & (TYPE_DVI_SINGLE|TYPE_HDMI|TYPE_MHL) )
                msComboOpenShortTest(g_InputPort[InputPort].eIPMux-1,TMDS);
            else if( g_InputPort[InputPort].eIPType & (TYPE_DVI_DUAL) )
                msComboOpenShortTest(g_InputPort[InputPort].eIPMux-1,DVID);
            else
                msComboOpenShortTest(g_InputPort[InputPort].eIPMux-1,DP);
            MENUFUNC_printMsg("=====================================\n");
        }
    }

    msWriteByteMask(REG_001781, u8REG_001781, 0x3F);//Restore Rtem settings
    msWrite2Byte(REG_0017AC, u8REG_0017AC);//Restore Test settings
    msWrite2Byte(REG_0017A2, u8REG_0017A2);//Restore Rtem settings
    msWrite2Byte(REG_0017C4, u8REG_0017C4);//Restore Rtem settings
}
static void msLVDSGPIOConfig(void)
{
    msWrite2Byte(REG_MOD1_20, 0x0000);
    msWrite2Byte(REG_MOD1_22, 0x0000);
    msWrite2Byte(REG_MOD1_24, 0x0000);
    msWrite2Byte(REG_MOD2_82, 0x0000);
    msWrite2Byte(REG_MOD2_84, 0x0000);
    msWrite2Byte(REG_MOD2_86, 0x0000);// Output Config

    msWrite3Byte(REG_MOD2_D2, 0x0000);// External GPIO Enable
    msWrite3Byte(REG_MOD1_30, 0x0000);// External GPIO Enable

    msWrite2Byte(REG_MOD1_46, 0x0000);// PN Swap
    msWrite2Byte(REG_MOD1_48, 0x0000);
    msWrite2Byte(REG_MOD2_AE, 0x0000);
}

static void msLVDSGPIOOuputConfig(WORD u16Mask, BYTE u8MODChannel)
{
    if(u8MODChannel)
    {
        msWrite2Byte(REG_MOD1_28, u16Mask);
        msWrite2Byte(REG_MOD1_2A, u16Mask);
        msWrite2Byte(REG_MOD1_2C, u16Mask);
    }
    else
    {
        msWrite2Byte(REG_MOD2_C6, u16Mask);
        msWrite2Byte(REG_MOD2_C8, u16Mask);
        msWrite2Byte(REG_MOD2_CA, u16Mask);
    }
}

static void msLVDSSetGPIOOuputPattern(WORD u16Pattern, BYTE u8MODChannel)
{
    if(u8MODChannel)
    {
        msWrite2Byte(REG_MOD1_0C, u16Pattern);
        msWrite2Byte(REG_MOD1_0E, u16Pattern);
        msWrite2Byte(REG_MOD1_10, u16Pattern);
    }
    else
    {
        msWrite2Byte(REG_MOD2_C0, u16Pattern);
        msWrite2Byte(REG_MOD2_C2, u16Pattern);
        msWrite2Byte(REG_MOD2_C4, u16Pattern);
    }
}

void msLVDSOpenShortTest(void)
{
    WORD u16REG_MOD1_20, u16REG_MOD1_22, u16REG_MOD1_24, u16REG_MOD2_82, u16REG_MOD2_84, u16REG_MOD2_86;
    WORD u16REG_MOD1_46, u16REG_MOD1_48, u16REG_MOD2_AE, u16REG_MOD1_10, u16REG_MOD1_2C;
    DWORD u32REG_MOD2_C0, u32REG_MOD2_C4, u32REG_MOD2_C8, u32REG_MOD1_30, u32REG_MOD1_0C, u32REG_MOD1_28, u32REG_MOD2_D2;
    WORD u16Pattern, u16PinMask;
    DWORD u32RegGPIOIn, u32RegGPIOOut, u32RegGPIOOez;
    unsigned long long u64PinResult1, u64PinResult2, u64ShortPin, u64ShortPin2, u64PinMask;
    BYTE i, j, k;
    BYTE u8ShortFlag = 0;

    MENUFUNC_printMsg("***** LVDS Connector Test Start *****\n");
    MENUFUNC_printMsg("=====================================");
    // Backup MOD settings
    u16REG_MOD1_20 = msRead2Byte(REG_MOD1_20);
    u16REG_MOD1_22 = msRead2Byte(REG_MOD1_22);
    u16REG_MOD1_24 = msRead2Byte(REG_MOD1_24);
    u16REG_MOD2_82 = msRead2Byte(REG_MOD2_82);
    u16REG_MOD2_84 = msRead2Byte(REG_MOD2_84);
    u16REG_MOD2_86 = msRead2Byte(REG_MOD2_86);// Output Config

    u32REG_MOD2_C0 = msRead4Byte(REG_MOD2_C0);
    u32REG_MOD2_C4 = msRead4Byte(REG_MOD2_C4);
    u32REG_MOD2_C8 = msRead4Byte(REG_MOD2_C8);
    u32REG_MOD2_D2 = msRead3Byte(REG_MOD2_D2);

    u32REG_MOD1_0C = msRead4Byte(REG_MOD1_0C);
    u16REG_MOD1_10 = msRead2Byte(REG_MOD1_10);
    u32REG_MOD1_28 = msRead4Byte(REG_MOD1_28);
    u16REG_MOD1_2C = msRead2Byte(REG_MOD1_2C);
    u32REG_MOD1_30 = msRead3Byte(REG_MOD1_30);

    u16REG_MOD1_46 = msRead2Byte(REG_MOD1_46);// PN swap
    u16REG_MOD1_48 = msRead2Byte(REG_MOD1_48);
    u16REG_MOD2_AE = msRead2Byte(REG_MOD2_AE);

    // Switch MOD to GPIO mode by setting MOD output mode to TTL mode.
    msLVDSGPIOConfig();

    for(k=0;k<2;k++)// k=0, MOD E,F,G,H, k=1, MOD A,B,C,D
    {
        u32RegGPIOIn = (k?REG_MOD1_14:REG_MOD2_CC);
        u32RegGPIOOut = (k?REG_MOD1_0C:REG_MOD2_C0);
        u32RegGPIOOez = (k?REG_MOD1_28:REG_MOD2_C6);
        //Set positive pin to output, nagetive pin to input.
        u16PinMask = 0x5555;
        msLVDSGPIOOuputConfig(u16PinMask, k);
        //Pattern to search P-N and N-P short case.
        u16Pattern = 0xAAAA;
        msLVDSSetGPIOOuputPattern(u16Pattern, k);
        ForceDelay1ms(2);
        u64PinResult1 = (((msRead2Byte(u32RegGPIOIn))&u16PinMask)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+2))&u16PinMask)<<16)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+4))&u16PinMask)<<32));

        u16Pattern = 0x0000;
        msLVDSSetGPIOOuputPattern(u16Pattern, k);
        ForceDelay1ms(2);
        u64PinResult2 = (((msRead2Byte(u32RegGPIOIn))&u16PinMask)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+2))&u16PinMask)<<16)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+4))&u16PinMask)<<32));
        u64ShortPin = u64PinResult1^u64PinResult2;

        //Pattern to search P-P, N-N short case.(+/- 1 channel)
        u16PinMask = 0xCCCC;
        msLVDSGPIOOuputConfig(u16PinMask, k);
        u16Pattern = 0x3333;
        msLVDSSetGPIOOuputPattern(u16Pattern, k);
        ForceDelay1ms(2);
        u64PinResult1 = (((msRead2Byte(u32RegGPIOIn))&u16PinMask)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+2))&u16PinMask)<<16)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+4))&u16PinMask)<<32));

        u16Pattern = 0x0000;
        msLVDSSetGPIOOuputPattern(u16Pattern, k);
        ForceDelay1ms(2);
        u64PinResult2 = (((msRead2Byte(u32RegGPIOIn))&u16PinMask)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+2))&u16PinMask)<<16)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+4))&u16PinMask)<<32));
        u64ShortPin |= u64PinResult1^u64PinResult2;
        //printData("u64ShortPin_H_1 = %x",u64ShortPin>>32);
        //printData("u64ShortPin_M = %x",u64ShortPin>>16);
        //printData("u64ShortPin_L = %x",u64ShortPin);
        //Pattern to search P-P, N-N short case.(+/- 2 channel)
        u16PinMask = 0xF0F0;
        msLVDSGPIOOuputConfig(u16PinMask, k);
        u16Pattern = 0x0F0F;
        msLVDSSetGPIOOuputPattern(u16Pattern, k);

        ForceDelay1ms(2);
        u64PinResult1 = (((msRead2Byte(u32RegGPIOIn))&u16PinMask)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+2))&u16PinMask)<<16)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+4))&u16PinMask)<<32));

        u16Pattern = 0x0000;
        msLVDSSetGPIOOuputPattern(u16Pattern, k);
        ForceDelay1ms(2);
        u64PinResult2 = (((msRead2Byte(u32RegGPIOIn))&u16PinMask)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+2))&u16PinMask)<<16)|((unsigned long long)((msRead2Byte(u32RegGPIOIn+4))&u16PinMask)<<32));


        u64ShortPin |= u64PinResult1^u64PinResult2;
        //printData("u64ShortPin_H_2 = %x",u64ShortPin>>32);
        //printData("u64ShortPin_M = %x",u64ShortPin>>16);
        //printData("u64ShortPin_L = %x",u64ShortPin);
        for(i=0;i<64;i++)
        {
            if((u64ShortPin >> i) & BIT0)
            {
                u8ShortFlag = 1;
                if(i%2)
                    MENUFUNC_printData("%d Channel N short with", i/2 + (k?0:24));
                else
                    MENUFUNC_printData("%d Channel P short with", i/2 + (k?0:24));
                // Search which pin short with.
                u64PinMask = (unsigned long long)BIT0<<i;
                msWrite2Byte(u32RegGPIOOez, ~u64PinMask);
                msWrite2Byte(u32RegGPIOOez + 2, (~u64PinMask)>>16);
                msWrite2Byte(u32RegGPIOOez + 4, (~u64PinMask)>>32);

                msWrite2Byte(u32RegGPIOOut, u64PinMask);
                msWrite2Byte(u32RegGPIOOut + 2, u64PinMask>>16);
                msWrite2Byte(u32RegGPIOOut + 4, u64PinMask>>32);
                ForceDelay1ms(2);
                u64PinResult1 = ((msRead2Byte(u32RegGPIOIn))|((unsigned long long)(msRead2Byte(u32RegGPIOIn+2))<<16)|((unsigned long long)(msRead2Byte(u32RegGPIOIn+4))<<32))&(~u64PinMask);

                msLVDSSetGPIOOuputPattern(0x0000, k);
                ForceDelay1ms(2);
                u64PinResult2 = ((msRead2Byte(u32RegGPIOIn))|((unsigned long long)(msRead2Byte(u32RegGPIOIn+2))<<16)|((unsigned long long)(msRead2Byte(u32RegGPIOIn+4)))<<32)&(~u64PinMask);

                u64ShortPin2 = u64PinResult1^u64PinResult2;

                u64ShortPin &= (~u64ShortPin2);
                for(j=0;j<64;j++)
                {
                    if((u64ShortPin2 >> j) & BIT0)
                    {
                        if(j%2)
                            MENUFUNC_printData("%d Channel N", j/2 + (k?0:24));
                        else
                            MENUFUNC_printData("%d Channel P", j/2 + (k?0:24));
                    }
                }
                MENUFUNC_printMsg("=================================");
            }
        }
    }
    if(!u8ShortFlag)
        MENUFUNC_printMsg("LVDS Open/Short Testing PASS!!!!!!");
    // Restore MOD settings
    msWrite2Byte(REG_MOD1_20, u16REG_MOD1_20);
    msWrite2Byte(REG_MOD1_22, u16REG_MOD1_22);
    msWrite2Byte(REG_MOD1_24, u16REG_MOD1_24);
    msWrite2Byte(REG_MOD2_82, u16REG_MOD2_82);
    msWrite2Byte(REG_MOD2_84, u16REG_MOD2_84);
    msWrite2Byte(REG_MOD2_86, u16REG_MOD2_86);

    msWrite4Byte(REG_MOD2_C0, u32REG_MOD2_C0);
    msWrite4Byte(REG_MOD2_C4, u32REG_MOD2_C4);
    msWrite4Byte(REG_MOD2_C8, u32REG_MOD2_C8);
    msWrite3Byte(REG_MOD2_D2, u32REG_MOD2_D2);

    msWrite4Byte(REG_MOD1_0C, u32REG_MOD1_0C);
    msWrite2Byte(REG_MOD1_10, u16REG_MOD1_10);
    msWrite4Byte(REG_MOD1_28, u32REG_MOD1_28);
    msWrite2Byte(REG_MOD1_2C, u16REG_MOD1_2C);
    msWrite3Byte(REG_MOD1_30, u32REG_MOD1_30);

    msWrite2Byte(REG_MOD1_46, u16REG_MOD1_46);
    msWrite2Byte(REG_MOD1_48, u16REG_MOD1_48);
    msWrite2Byte(REG_MOD2_AE, u16REG_MOD2_AE);

    MENUFUNC_printMsg("====LVDS Open/Short Test Done====");
}
//==============================================================================================================
#endif
#if ENABLE_AUDIO_SETTINGS_MENU
Bool AdjustVolume(MENU_ITEM_ACTION enAction)
{
    WORD u16TempValue;
    u16TempValue=DecIncValue(enAction, USER_PREF_VOLUME, VOLUME_OSD_MIN, VOLUME_OSD_MAX, 1);

    if (u16TempValue==USER_PREF_VOLUME)
        return FALSE;

    USER_PREF_VOLUME=u16TempValue;

    //printData("--adjust------------value====%x",u16TempValue);

#if 0
    if(AUDIOMUTE_FLAG)
    {
        CLR_AUDIOMUTE_FLAG();
        SetAudioMuteOnOff();
    }
#endif

    msAPI_AdjustVolume(USER_PREF_VOLUME);
    //g_u8DDCCIAControl = VCP_SPEAKER_VOLUME;
    //Set_NewControlFlag();
    //g_u8SaveUserData|=SAVE_MONITOR_SETTING;
    return TRUE;
}
WORD GetVolumeValue(void)
{
    return GetScale100Value(USER_PREF_VOLUME,VOLUME_OSD_MIN,VOLUME_OSD_MAX);
}


//------------------------------------------------------------------------------------------
#if ((!ENABLE_ADJ_AUDIO_OUTPUT)&&(!Audio_Input_Jack_Only_For_Line_In)&&(!Audio_Input_Jack_Only_For_Output))
void EarphoneHandler(void)
{
  
   if((USER_PREF_VOLUME==VOLUME_OSD_MIN)
#if ENABLE_AUDIO_SETTINGS_MENU
   ||AUDIO_MUTE_FLAG
#endif
   )
	return;

   if (POWER_ON_FLAG&&BacklightFlag&& !SyncLossState(USER_PREF_INPUT_TYPE) &&(!PowerSavingFlag)&&bPanelOnFlag &&BacklightFlag && g_DisplayWindow[DISPLAY_WIN1].eDwStatus == WIN_STATUS_OUTPUT_DISPLAYOK)//USER_PREF_WIN_SEL
   {


  #if 0//Audio_Input_Jack_Only_For_Output//audio input jack only for output 
              if(hw_HP_DET_Pin)
              {
                if((!EARPHONE_EN_FLAG))
                {
                   hw_SetMute();
                   msAPI_AdjustVolume(0);
                   SET_EARPHONE_EN_FLAG();	
                   printMsg("hw_Clr_HP_MUTE");
                   //hw_Clr_HP_MUTE();
                   Delay1ms(100);
                   msAPI_AudioSetEarphoneMute(FALSE);	   		     		   
                   msAPI_AdjustVolume(USER_PREF_VOLUME);
		/*		   
                      hw_SetMute();		   
                      SET_EARPHONE_EN_FLAG();	
                      msAPI_AdjustVolume(0);
                      printMsg("hw_Set_HP_MUTE");
                      hw_Set_HP_MUTE();		
                      Delay1ms(200);   
                      hw_ClrMute();		
                      msAPI_AdjustVolume(USER_PREF_VOLUME);
		*/		   
                }
              }
   	      else
   	      {
   	         if(EARPHONE_EN_FLAG)
   	   	   {
   	   	     if((!InputTimingChangeFlag(USER_PREF_INPUT_TYPE))&&(InputTimingStable_NormalFlag)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!PowerSavingFlag)&&bPanelOnFlag)
   	   	     {
/*
                   hw_SetMute();
                   msAPI_AdjustVolume(0);
                   CLR_EARPHONE_EN_FLAG();	
                   printMsg("hw_Clr_HP_MUTE");
                   hw_Clr_HP_MUTE();
                   Delay1ms(100);
                   msAPI_AdjustVolume(USER_PREF_VOLUME);
*/

                      hw_SetMute();		   
                      CLR_EARPHONE_EN_FLAG();	
                      msAPI_AdjustVolume(0);
                      printMsg("hw_Set_HP_MUTE");
                      //hw_Set_HP_MUTE();		
                      Delay1ms(200);   
                      msAPI_AudioSetEarphoneMute(FALSE);	   		     		   
                      hw_ClrMute();		
                      msAPI_AdjustVolume(USER_PREF_VOLUME);

   	   	     }
   	   	   }
   	        }

  #else

#if ((MS_BOARD_TYPE_SEL == BD_MST9UHL_A0))
     if(CURRENT_INPUT_IS_ANALOG1()
   #if ENABLE_DVI_INPUT
	 ||CURRENT_INPUT_IS_DIGITAL_DVI()
	#if ENABLE_DVI2ND_INPUT 
	 ||CURRENT_INPUT_IS_DIGITAL_DVI2ND()
	#endif 
 	#if ENABLE_DVI3ND_INPUT 
	 ||CURRENT_INPUT_IS_DIGITAL_DVI3ND()
	#endif 
   #endif
      )
      return;
#endif
              if(hw_HP_DET_Pin)
              {
                if((!EARPHONE_EN_FLAG))
                {
                    SET_EARPHONE_EN_FLAG();
                    msAPI_AdjustVolume(0);
                    hw_SetMute();
                    printMsg("EarPhone");
                    //hw_Clr_HP_MUTE();
                    //hw_Clr_SPIFGPIO_EN_Pin();
                    msAPI_AudioSetEarphoneMute(FALSE);	   

                    msAPI_AdjustVolume(USER_PREF_VOLUME);
					
                }
              }
   	      else
   	      {
   	         if(EARPHONE_EN_FLAG)
   	   	   {
   	   	     if((!InputTimingChangeFlag(USER_PREF_INPUT_TYPE))&&(InputTimingStable_NormalFlag)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!PowerSavingFlag)&&bPanelOnFlag)
   	   	     {
   	   	            CLR_EARPHONE_EN_FLAG();
                          msAPI_AdjustVolume(0);
                          hw_SetMute();		   
                          printMsg("Speaker");
                          //hw_Set_HP_MUTE();	
                          //hw_Clr_SPIFGPIO_EN_Pin();
           		     msAPI_AudioSetEarphoneMute(TRUE);	   

                          hw_ClrMute();		
                          msAPI_AdjustVolume(USER_PREF_VOLUME);

   	   	     }
   	   	   }
   	        }
    #endif
   }

}
#endif



#endif

#if ENABLE_ADJ_AUDIO_OUTPUT
Bool OSD_SetAudioOutput( void )
{

       if(USER_PREF_AUDIO_OUTPUTTYPE==Speaker)
       {
               hw_SetMute();		   
               msAPI_AdjustVolume(0);
               printMsg("Speaker");
               //hw_Set_HP_MUTE();	
               hw_Clr_SPIFGPIO_EN_Pin();
		 msAPI_AudioSetEarphoneMute(TRUE);	   
               Delay1ms(50);   
               hw_ClrMute();		
               msAPI_AdjustVolume(USER_PREF_VOLUME);
       }
#if Enable_SPDIF	   
	else if(USER_PREF_AUDIO_OUTPUTTYPE==SPDIF_OUT)  
       {
               hw_SetMute();		   
               msAPI_AdjustVolume(0);
               printMsg("SPDIF_OUT");
		 msAPI_AudioSetEarphoneMute(TRUE);	   
               Delay1ms(50);   
               hw_Set_SPIFGPIO_EN_Pin();
               msAPI_AdjustVolume(USER_PREF_VOLUME);
       }
#endif	
	else
	{
          //if(USER_PREF_AUDIO_OUTPUTTYPE==EarPhone)
	       {
               hw_SetMute();
               msAPI_AdjustVolume(0);
               printMsg("EarPhone");
               //hw_Clr_HP_MUTE();
               hw_Clr_SPIFGPIO_EN_Pin();
               msAPI_AudioSetEarphoneMute(FALSE);	   
               Delay1ms(50);
               msAPI_AdjustVolume(USER_PREF_VOLUME);
	   	}
	   }
    g_u8SaveUserData|=SAVE_MONITOR_SETTING;


	return TRUE;
}


WORD GetAudioOutputValue(void)
{
    return (USER_PREF_AUDIO_OUTPUTTYPE);
}

Bool AdjustAudioOutputValue(MENU_ITEM_ACTION enAction)
{

    g_u8MenuTempValue = DecIncValueCirculate(enAction, g_u8MenuTempValue, EarPhone, OutputNums-1, 1);
    if (g_u8MenuTempValue==USER_PREF_AUDIO_OUTPUTTYPE)
        return FALSE;
    USER_PREF_AUDIO_OUTPUTTYPE=g_u8MenuTempValue;

	return TRUE;
}
#endif

#if ENABLE_DP_INPUT||ENABLE_HDMI_INPUT
void SetFreeSyncOnOffFunction(BYTE ucWinIndex, Bool bEnable, WORD uwDurationMs)
{
    BYTE ucInputPort = MapWin2Port(ucWinIndex);
    BYTE u8InputPort = 0;

 #if ENABLE_DP_INPUT	
	if(INPUT_IS_DISPLAYPORT(ucInputPort))
	{
#if ENABLE_DAISY_CHAIN
	    if(g_InputPort[ucInputPort].eIPType & TYPE_DAISY_CHAIN)
	    {
	        BYTE u8Replay = 0;

	        msDCSetDRRDPCD( bEnable?DRR_ENABLE:DRR_DISABLE, &u8Replay );
	    }
	    else
#endif
	    {
	       
               Set_HDMI1HPDFlag();   
               Set_HDMI2HPDFlag();   
               Set_HDMI3HPDFlag();   
          if(bEnable)
              msAPI_combo_IPSetDPVersion((EN_COMBO_IP_SELECT) g_InputPort[MapWin2Port(USER_PREF_WIN_SEL)].eIPMux, DP12);
	   else	  
              msAPI_combo_IPSetDPVersion((EN_COMBO_IP_SELECT) g_InputPort[MapWin2Port(USER_PREF_WIN_SEL)].eIPMux, (EN_DP_VERSION)USER_PREF_DPConfigType);
	       //  msAPISrcSetHPD(ucInputPort, uwDurationMs);
	       //msAPI_combo_IPSetDPDRRFunction((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, bEnable);

            for( u8InputPort=First_Input_Port; u8InputPort < Input_Nums; u8InputPort++ )
            {
                if(g_InputPort[u8InputPort].eIPType&TYPE_DP)
                    msAPI_combo_IPSetDPDRRFunction((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, bEnable);
            }
		   
 	       msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, FALSE);	 		 
 	       //HPD_Status=1;			 
           #if (Free_Sync_Enable &&(defined(FengYuan_Project)))//||defined(LianHeChuangXin_Project)))    
               EDID_ReLoad_Func(0);
               msAPI_combo_LoadEDID_Func();
	     #endif	
		 ForceDelay1ms(800); // delay 1 msec		 
		 msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, TRUE);	 		
	    }
	}
	else
  #endif	
  #if 1//(defined(LianHeChuangXin_Project))
	 if (INPUT_IS_HDMI(ucInputPort))
	{

   //printMsg("00000 Freesync HPD");
	  /*
             if((!INPUT_IS_DISPLAYPORT(ucInputPort))&&(!INPUT_IS_VGA(ucInputPort)))
             {
                msAPISrcSetHPD(ucInputPort, DFT_HPD_PERIOD); // set reasonable value, e.g. >= 300ms
             }	
	 */
  if((abs(InputWidth-1920)<10)&&(abs(InputHeight-1080)<10)&&(abs(InputVfreq-600)<10))	 
	 mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
	#if 1//(defined(K3C_Project))
             //hw_ClrBlacklit();
             msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, FALSE);	
             ForceDelay1ms(650);
             msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[ucInputPort].eIPMux, TRUE);	 	
             //SrcFlags(ucInputPort) |= SyncLoss;
             //Set_InputTimingChangeFlag(ucInputPort);
             g_u8SwitchPortCounter = 0; 
	#endif 	
	
	//Set_OSDActionFlag();
  if((abs(InputWidth-1920)<10)&&(abs(InputHeight-1080)<10)&&(abs(InputVfreq-600)<10))	 
  {
       SrcFlags(ucInputPort) |= SyncLoss;
       Set_InputTimingChangeFlag(ucInputPort);
  }
              /*  
              msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE);	 	
              msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, FALSE); 
              
              msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE); 
              msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_HDMI].eIPMux, TRUE);	
              */
	}
   #else
      ;
   #endif

}
#endif


BYTE GetVfreqType( void )
{
   BYTE ucType=0;
   WORD u16InputVfreq=0;
   WORD hFreq=0;
   WORD vFreq=0;
   WORD hFreqTemp=0;
   WORD vFreqTemp=0;
   BYTE u8DetSCIndex = MapPort2DetScIndex(USER_PREF_INPUT_TYPE);
   // vFreq = msDrvIP1ReadVtotal(u8DetSCIndex);//_msRead2Byte(SC0_E2) & 0xFFF;
    hFreqTemp=msDrvIP1ReadHperiod(u8DetSCIndex);
    hFreq = HFREQ( hFreqTemp );
    vFreqTemp=msDrvIP1ReadVtotal(u8DetSCIndex);
    vFreq = VFREQ( hFreq,   vFreqTemp );
     u16InputVfreq=vFreq;
   //u16InputVfreq=VFREQ( HFREQ( SrcHPeriod(MapWin2Port(USER_PREF_WIN_SEL)) ), SrcVTotal(MapWin2Port(USER_PREF_WIN_SEL)) );
#if (PanelType == PanelM350DVR01_01||PanelType==PanelM350DVA01_1)
   if(SyncLossState(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
	ucType=1;
   else
#endif
   if(u16InputVfreq<480 ||u16InputVfreq>1500)
   	ucType=0;
   else if(u16InputVfreq>(750+20) && u16InputVfreq<1500)  //    160714_09  // update by VSC Change For 75hz/76Hz timing B/I issue
   	ucType=1;
 //  printData("V ucType==%d", ucType);
   //printData("V u16InputVfreq==%d", u16InputVfreq);

   return ucType;
}

#if 1//INPUT_SAME_OUT
DWORD Get_TMDS_ClockRateType( BYTE InputPort )
{
    DWORD u16InputDCK=0;
    DWORD u16OutputDCK=0;

    u16InputDCK=msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)/100;
    //u16InputDCK=mapi_tmds_GetClockRate(InputPort);
      printData("TMDS u16InputDCK==%d", u16InputDCK);
#if (PanelType==PanelLSM315HP01)
  if(u16InputDCK<180)//1920*1080CVT 173Mhz
#else
  if(u16InputDCK<PanelMinDCLK)
#endif  
    {
		if((GetInputVfreq(DISPLAY_WIN1)>950))
	   		u16OutputDCK= Panel_120CLK;
		else
			u16OutputDCK= PanelMinDCLK;
		
     printData("TMDS u16OutputDCK 0000==%d", u16InputDCK);
      return u16OutputDCK*1000;
    }
    else
    {
    	u16InputDCK=msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)/100;
  #if (PanelType==PanelLSM315HP01)		
	if(abs(u16InputDCK-333)<5)//144
   	   u16OutputDCK= 339;
 	else if(abs(u16InputDCK-296)<5)//120
   	   u16OutputDCK= 283;
	else
  #endif	
       if(u16InputDCK>PanelMaxDCLK)
          u16OutputDCK=PanelMaxDCLK;
	else   
   	   u16OutputDCK= u16InputDCK;//Panel_120CLK;
       printData("TMDS u16OutputDCK 1111==%d", u16OutputDCK);
       return u16OutputDCK*1000;
    }
    //printData("TMDS tempVFreq==%d", GetInputVfreq(DISPLAY_WIN1));

    //return u16OutputDCK*1000;
}

WORD GetCurrentInputVfreq(void);
Bool CheckPanelNormalTiming(void)
{
	WORD u16InputVfreq=GetCurrentInputVfreq()*10;
	printf("___Get_Panel_Timing Input Freq=%d\n", u16InputVfreq);
	if( (u16InputVfreq < (PanelMaxDCLK+10)) && (u16InputVfreq > (480-20)) )
	{
		return TRUE;
	}
	return FALSE;
}

#endif


#if USE_FIXED_DCLK_FOR_FREESYNCMODE
DWORD GetFreesyncFixClock(BYTE InputPort)
{
    WORD u16InputDCK = 0;
    DWORD u16OutputDCK = 0;

//    u16InputDCK = mapi_tmds_GetClockRate(InputPort);
	u16InputDCK = msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)/100;
#if 0//PANEL_VBY1()
    u16OutputDCK = PanelMinDCLK+10;
#else
    if(u16InputDCK<PanelMinDCLK)
    {
        u16OutputDCK = PanelMinDCLK+10;
    }
    else //if(u16InputDCK>PanelMaxDCLK)
    {
        u16OutputDCK = PanelMaxDCLK-10;
    }
    //else
        //u16OutputDCK = u16InputDCK;
#endif

#if 0
    if( u16InputDCK < 180 ) //1920*1080CVT 173Mhz
    {
        if( (Get_MultiWin_TimingInfo_Height(DISPLAY_WIN1) <= 800) && (u16InputDCK > 100) )
            u16OutputDCK = PanelDCLK;//Panel_120CLK;
        else
            u16OutputDCK = PanelMinDCLK;
    }
    else if( u16InputDCK > 300 ) //1920*1080CVT 173Mhz
    {
        u16OutputDCK = PanelMaxDCLK;
    }
    else
    {
        u16OutputDCK = PanelDCLK;//Panel_120CLK;
    }
#endif
    printData("TMDS u16InputDCK==%d", u16InputDCK);
    printData("TMDS u16OutputDCK==%d", u16OutputDCK);

    return u16OutputDCK * 1000;
}

DWORD GetFreesyncHTotal(BYTE InputPort)
{
        WORD u16TempHtotal;
		WORD u16InputDCK = 0;
		u16InputDCK = msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux)/100;
        
        #if 0//((PanelType==PanelAUOM250HTN010)||(PanelType==PanelAUOM270HTN020))//240Hz
            if((abs(u16InputDCK - 148)<15)) // 60Hz
            {
                u16TempHtotal = PanelMaxHTotal;
            }
            else if((abs(u16InputDCK - 235)<15)) // 100Hz
            {
                u16TempHtotal = PanelMaxHTotal;
            }
            else if((abs(u16InputDCK - 296)<15))// 120Hz
            {
                u16TempHtotal = 3584;
            }
            else if((abs(u16InputDCK - 324)<15)) // 144Hz
            {
                u16TempHtotal = 3104;
            }
            else if((abs(u16InputDCK - 570)<15)) // 240Hz
            {
                u16TempHtotal = PanelHTotal;
            }
            else
            {
                u16TempHtotal = PanelHTotal;
            }
        #elif 0//((PanelType==PanelAUOM250HTN012)||(PanelType==PanelAUOM270HTN022)) //144hz
            if((abs(u16InputDCK - 148)<15))  // 60Hz
            {
                u16TempHtotal = PanelMaxHTotal;
            }
            else if((abs(u16InputDCK - 235)<15)) // 100Hz
            {
                u16TempHtotal = 3816;
            }
            else if((abs(u16InputDCK - 296)<15)) // 120Hz
            {
                u16TempHtotal = 3144;
            }
            else if((abs(u16InputDCK - 324)<15)) //144Hz
            {
                u16TempHtotal = 2736;
            }
            else
            {
                u16TempHtotal = PanelHTotal;
            }
        #else
            u16TempHtotal = PanelHTotal;
        #endif

        return u16TempHtotal;
    }
#endif



#if ENABLE_DP_INPUT
void ApplyDaisyChainType(void)
{
    #if (ENABLE_DAISY_CHAIN)
    switch(USER_PREF_DPConfigType)
    {
          case DP11:  //DP1.1 SST
          {
                BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
                MENUFUNC_printMsg("DP Switch to DP1.1\n");
                u8SendData = 0x1;
                while(!(msDCSetCmdSetSysMode(u8SendData, &u8ReplyData)) && --u8Retry)
                MENUFUNC_printData("[PM] DC Setup Retry %d\n",u8Retry);
          }
          break;
          case DP12: //DP1.2 SST
          {
                BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
                MENUFUNC_printMsg("DP Switch to DP1.2 SST\n");
                u8SendData = 0x2;
                while(!(msDCSetCmdSetSysMode(u8SendData, &u8ReplyData)) && --u8Retry)
                MENUFUNC_printData("[PM] DC Setup Retry %d\n",u8Retry);
          }
          break;
		  #if 0
          case DP12MST: //DP1.2 MST
          {
                BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
                MENUFUNC_printMsg("DP Switch to DP1.2 MST\n");
                u8SendData = 0x3;
                while(!(msDCSetCmdSetSysMode(u8SendData, &u8ReplyData)) && --u8Retry)
                MENUFUNC_printData("[PM] DC Setup Retry %d\n",u8Retry);
          }
          break;
		  #endif
        default:

            break;
    }
    #endif //#if (ENABLE_DAISY_CHAIN == 0x1)
}
#endif


#if !Enable_AutoInput_Menu	
//------------------------------------------------------------------------------------------
void Set_PMModeFlag( BYTE ePMMode )
{	
    BYTE XDATA i;
 #if EnableDdcLed 
     g_sMailBoxR2.u16PMFlag=g_sMailBoxR2.u16PMFlag|((USER_PREF_LEDMODE==OffLedMode)?0:BIT0);
 #else
     g_sMailBoxR2.u16PMFlag= 0x0000;
 #endif	  
    g_sMailBoxR2.u16PMPortSkipIndex = 0x0000;	
     
     if(ePMMode == ePM_POWER_DPMS)
     {
          if(1)//(AUTOSWITCHPORT_FLAG)
          {  g_sMailBoxR2.u16PMPortSkipIndex = 0x0000;	
	       #if 0//!ENABLE_DVI_INPUT
              g_sMailBoxR2.u16PMPortSkipIndex |= (1 << INPUT_DVI);  //Disable INPUT_DVI port
              #endif
	   }
	   else
          {
              for(i=0; i<Input_Nums; i++)
              {
                  g_sMailBoxR2.u16PMPortSkipIndex |= (1 << i);  //Disable All port
              }
    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU			  
             if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
             {   
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE);  //enable current port
             }
             else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1)
             {  
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE);  //enable current port
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE_SUBWIN1);  //enable current port
             }
             else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_3)
             {   
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE);  //enable current port
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE_SUBWIN1);  //enable current port
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE_SUBWIN2);  //enable current port
		  
             }
             else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_4)
             {   
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE);  //enable current port
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE_SUBWIN1);  //enable current port
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE_SUBWIN2);  //enable current port
                 g_sMailBoxR2.u16PMPortSkipIndex &=~ (1 << USER_PREF_INPUT_TYPE_SUBWIN3);  //enable current port
		  	
             }	
   #endif			 
	  }
    }
}
#endif

//------------------------------------------------------------------------------------------
Bool Is_PM_Mode_State(void)
{
     if( (g_ePMMode == ePM_POWER_DCOFF) || (g_ePMMode == ePM_POWER_DPMS) )
        return TRUE;
    return FALSE;
}


//------------------------------------------------------------------------------------------
void SetIsrTimingChangeStatus(BYTE ucInputPort)
{
#if ENABLE_TIMINGCHANGE_CLRBL_ONLY||ENABLE_TIMINGCHANGE_TURNOFFPANEL
    BYTE u8IdxWin;
#endif

     ucInputPort=ucInputPort;
     Clr_PowerSavingFlag();
#if ENABLE_TIMINGCHANGE_CLRBL_ONLY	     //  160714_14
	for(u8IdxWin=0 ; u8IdxWin<DISPLAY_MAX_NUMS ; u8IdxWin++)    //20140408 Rick add for solve timing change garbage while use less 1 frame mode
	{
		if ((ucInputPort == MapWin2Port(u8IdxWin))&&( USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF )&&( g_stMenuFlag.bDisplayLogo==0 )&&((bPanelOnFlag==TRUE)&&(bOnlyClrBL==FALSE)))//&&(g_DisplayWindow[u8IdxWin].bDwReadCurFrame))
		{
			bPanelOnFlag=0;
			bOnlyClrBL=TRUE;
			hw_ClrBlacklit();
		}
	}
#elif ENABLE_TIMINGCHANGE_TURNOFFPANEL
	for(u8IdxWin=0 ; u8IdxWin<DISPLAY_MAX_NUMS ; u8IdxWin++)    //20140408 Rick add for solve timing change garbage while use less 1 frame mode
	{
		if ((ucInputPort == MapWin2Port(u8IdxWin))&&( USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF )&&( g_stMenuFlag.bDisplayLogo==0 )&&((bPanelOnFlag==TRUE)))//&&(g_DisplayWindow[u8IdxWin].bDwReadCurFrame))
		{
			bTurnOffPanelWhenTimingChange=TRUE;
		}
	}
#else
   #if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&(defined(LianHeChuangXin_Project)))
       if((USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
        ;
	else   
   #endif	
   #if 0//Free_Sync_Enable     
       if((OSDFreeSyncActionFlag)&&(OSDShowFlag))
        ;
	else   
   #endif		
        hw_ClrBlacklit();
#endif

	 
     APPInput_DetectSyncStableCntInit();
     printMsg("SetIsrTimingChangeStatus");
	 
}

#if AudioFunc
void SetModeChangeAudioMuteOnOff( void )
{
    BYTE i;
    Bool MultiWinDisplayFlage=0;
     //audio_EnableAudioAfterSignalLock_Flag=AUDIO_IDLE;

	 
    
      for(i=0; i< DISPLAY_MAX_NUMS; i++)
     {
        //if(g_DisplayWindow[i].eDwStatus == WIN_STATUS_OUTPUT_DISPLAYOK) 
        if((!SyncLossState(MapWin2Port(i)))&&(bPanelOnFlag))
        {  
           MultiWinDisplayFlage=1;
	    break;
        }

     }
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	  
     if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF&&MultiWinDisplayFlage)
     {
   //    printData("USER_PREF_AUDIO_SOURCE==%d",USER_PREF_AUDIO_SOURCE);
#if ENABLE_VGA_INPUT	   
      // printData("pUserPref->sUdSystem.u8SYS_AudioSRC[INPUT_VGA]==%d",pUserPref->sUdSystem.u8SYS_AudioSRC[INPUT_VGA]);
 #endif	   
  //     printData("audio_EnableAudioAfterSignalLock_Flag==%d",audio_EnableAudioAfterSignalLock_Flag);
	   
	#if ENABLE_HDMI_INPUT	
	     if((pUserPref->sUdSystem.u8SYS_AudioSRC[INPUT_HDMI]==AIDIOSOURCE_HDMI1)&&(HDMI_DP_AUDIO_ENABLE(INPUT_HDMI)))
	    {  
	    }
	#else
           if(0)
		  ;
	#endif
	#if ENABLE_HDMI2ND_INPUT	 
	    else if(pUserPref->sUdSystem.u8SYS_AudioSRC[INPUT_HDMI2ND]==AIDIOSOURCE_HDMI2&&(HDMI_DP_AUDIO_ENABLE(INPUT_HDMI2ND)))
	    {   
	    }
	#endif	
	#if ENABLE_HDMI3ND_INPUT	 
	    else if(pUserPref->sUdSystem.u8SYS_AudioSRC[INPUT_HDMI3ND]==AIDIOSOURCE_HDMI3&&(HDMI_DP_AUDIO_ENABLE(INPUT_HDMI3ND)))
	    {   
	    }
	#endif	
	
	#if ENABLE_DP_INPUT	
	    else if((pUserPref->sUdSystem.u8SYS_AudioSRC[INPUT_DISPLAYPORT]==AIDIOSOURCE_DP1)&&(HDMI_DP_AUDIO_ENABLE(INPUT_DISPLAYPORT)))
	    {  
	    }
	#endif	
	#if ENABLE_DP2ND_INPUT
	    else if(pUserPref->sUdSystem.u8SYS_AudioSRC[INPUT_DISPLAYPORT2ND]==AIDIOSOURCE_DP2&&(HDMI_DP_AUDIO_ENABLE(INPUT_DISPLAYPORT2ND)))
	    {    
	    }
	#endif	
	    else if(USER_PREF_AUDIO_SOURCE==AIDIOSOURCE_LINE_IN)
	    {    
	    }
	    else
	    {
          if (audio_EnableAudioAfterSignalLock_Flag==AUDIO_MUTE)
              return;
		printMsg("666666666666");
	       audio_EnableAudioAfterSignalLock_Flag=AUDIO_MUTE;	
              msAPI_AdjustVolume(0);
              hw_SetMute();		
		hw_Clr_SPIFGPIO_EN_Pin();	  
            #if ENABLE_DAC
              msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, TRUE);
              msAPI_AudioSetEarphoneMute(TRUE);
              msAPI_AudioSetMute( AUDIO_SOFTWARE_MUTE, E_SW_TIMING_CHANGE_MUTE_ON);
            #endif
            #if ENABLE_DAC
              msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
            #endif
	    }
     }
     else
#endif	 	
     {
     if (audio_EnableAudioAfterSignalLock_Flag==AUDIO_MUTE)
         return;
     
         audio_EnableAudioAfterSignalLock_Flag=AUDIO_MUTE;	
         msAPI_AdjustVolume(0);
     	  hw_SetMute();		
	  hw_Clr_SPIFGPIO_EN_Pin();	  
 	#if ENABLE_DAC
         msAPI_AudioSetDpgaMute(AUDIO_DPGA_ALL, TRUE);
         msAPI_AudioSetEarphoneMute(TRUE);
         msAPI_AudioSetMute( AUDIO_SOFTWARE_MUTE, E_SW_TIMING_CHANGE_MUTE_ON);
	#endif
	#if ENABLE_DAC
         msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
	#endif
     }

}
#endif


