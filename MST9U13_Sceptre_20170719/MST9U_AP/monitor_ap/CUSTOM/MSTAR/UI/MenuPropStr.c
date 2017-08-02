///////////////////////////////////////////////////////////////////////////////
/// @file MenuPropStr.c
/// @brief Menu string functions.
/// @author MStarSemi Inc.
///
/// Functions to deal with prop. strings.
///
/// Features
///  -Multi-Language support.
///  -
///////////////////////////////////////////////////////////////////////////////
#define _MENUPROPSTR_C

#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "Keypaddef.h"
#include "menudef.h"
#include "MenuPropStrTbl.h"
//#include "DebugMsg.h"
#include "msOSD.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "PropFontExtFunc.h"
#include "msOSDDrawText.h"
#include "Menu.h"
#include "appWindow.h"
#include "Userprefdef.h"

extern BYTE xdata g_u8MenuPageIndex;
extern BYTE xdata g_u8MenuTempValue;


BYTE* TxtM3PowerSave1(void)
{
    return strEnterPowerSavingModeM41[PROPFONTTBL_INDEX];
}


BYTE *TxtM4CableNotConnected(void)
{
    return strNoCableConnectM40[PROPFONTTBL_INDEX];  
}
#if 0

BYTE* TxtNoCableMsg(void)
{ return strNoCableMsgM4[PROPFONTTBL_INDEX];
}
BYTE* TxtTimer(void)
{ BYTE code strTimer[]={MSG_NOCABLE_TIMER_START,3};
    return strTimer;
}
BYTE GetTxtNoCableMsgLen(void)
{
    return strNoCableMsgM4[PROPFONTTBL_INDEX][1];
}
#endif
BYTE* TxtM5OutOfRange1(void)
{ return strUnsupportedMode_1M43[PROPFONTTBL_INDEX];
}
BYTE* TxtM5OutOfRange2(void)
{ return strUnsupportedMode_2M43[PROPFONTTBL_INDEX];
}
BYTE* TxtM5OutOfRange3(void)
{ return strUnsupportedMode_3M43[PROPFONTTBL_INDEX];
}

BYTE* TxtM5OutOfRangeSource(void)
{ 
#if ENABLE_DVI_INPUT
  if(USER_PREF_INPUT_TYPE==INPUT_DVI)
   return strUnsupportedMode_DVIM43[PROPFONTTBL_INDEX];
#else
 if(0);
#endif  
 else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)
#if ENABLE_HDMI2ND_INPUT 	
   return strUnsupportedMode_HDMI1M43[PROPFONTTBL_INDEX];
#else
   return strUnsupportedMode_HDMIM43[PROPFONTTBL_INDEX];
#endif
#if ENABLE_HDMI2ND_INPUT
 else if(USER_PREF_INPUT_TYPE==INPUT_HDMI2ND)
   return strUnsupportedMode_HDMI2M43[PROPFONTTBL_INDEX];
#endif 
 else if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)
   return strUnsupportedMode_DPM43[PROPFONTTBL_INDEX];
  else
   return strUnsupportedMode_DVIM43[PROPFONTTBL_INDEX];
}


#if ENABLE_VGA_INPUT
BYTE* TxtAutoAdjustInProgress(void)
{
    return strAutoAdjustmentM42[PROPFONTTBL_INDEX];
}
#endif




///============================================================================

//2007-09-20 Webber : start to build the str for 09 OSD

BYTE* Txt_tPropText0(void)
{
 if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
    return tPropText[0];
 else
    return NULL;	
}
//MenuPage 0 Group
//-- M0 Group Start --
#if 0 //FY12_MENU
BYTE* TxtMenu(void)
{  return strMenuM0[PROPFONTTBL_INDEX];
}
#endif

BYTE* MainFactoryText(void)
{  return strFactoryM8[PROPFONTTBL_INDEX];
}

//Standby Mode
BYTE* TxtMNoignal_01(void)
{ return strNoSignal_01M41[PROPFONTTBL_INDEX];
}
BYTE* TxtMNoignal_02(void)
{ return strNoSignal_02M41[PROPFONTTBL_INDEX];
}

//Brightness and contrast



#if USB_Updata_FW_Enable
BYTE* TxtUSBUpdataFW(void)
{  
    return strUSBUpdataFWM1[PROPFONTTBL_INDEX];
}
BYTE* TxtUSBUpdataFWStatus(void)
{  
  if(USBUpdataFwStatus==USB_Writting)
    return strUSBUpdataFWWrittingM1[PROPFONTTBL_INDEX];
  else if(USBUpdataFwStatus==USB_Fail)
    return strUSBUpdataFWFailM1[PROPFONTTBL_INDEX];
   else if(USBUpdataFwStatus==USB_Pass)
    return strUSBUpdataFWPassM1[PROPFONTTBL_INDEX]; 	
  else 
     return strUSBUpdataFWIdleM1[PROPFONTTBL_INDEX];
 	
}
#endif


#if Enable_HotAdjLowbluray
BYTE* TxtLowBluRay(void)
{  return strLowBluRayM15[PROPFONTTBL_INDEX];
}
#endif




//Color setting menu
//====================================================

#if HKC_RGB_Range_Func
BYTE* TxtUserPrefRGBRange(void)
{
    if (CURRENT_INPUT_COLOR_RANGE==OSD_INPUT_COLOR_RANGE_FULL)
        return strRGBFULLM16[PROPFONTTBL_INDEX];
    if (CURRENT_INPUT_COLOR_RANGE==OSD_INPUT_COLOR_RANGE_LIMITED)
        return strRGBLIMITEDM16[PROPFONTTBL_INDEX];
    return NULL;
}
#endif


//====================================================



#if  Enable_Corss_LHCX_New

BYTE code strHotKeyGameModeIcon[25]=
{
	HotGameFONT_2COLOR_ICON_START+0,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	HotGameFONT_2COLOR_ICON_START+4,
	HotGameFONT_2COLOR_ICON_START+5,	 
	
	HotGameFONT_2COLOR_ICON_START+6,
	HotGameFONT_2COLOR_ICON_START+7,
	HotGameFONT_2COLOR_ICON_START+8,
	HotGameFONT_2COLOR_ICON_START+9,
	HotGameFONT_2COLOR_ICON_START+10,
	HotGameFONT_2COLOR_ICON_START+11,	 
	
	HotGameFONT_2COLOR_ICON_START+12,
	HotGameFONT_2COLOR_ICON_START+13,
	HotGameFONT_2COLOR_ICON_START+14,
	HotGameFONT_2COLOR_ICON_START+15,
	HotGameFONT_2COLOR_ICON_START+16,
	HotGameFONT_2COLOR_ICON_START+17,	 

	HotGameFONT_2COLOR_ICON_START+18,
	HotGameFONT_2COLOR_ICON_START+19,
	HotGameFONT_2COLOR_ICON_START+20,
	HotGameFONT_2COLOR_ICON_START+21,
	HotGameFONT_2COLOR_ICON_START+22,
	HotGameFONT_2COLOR_ICON_START+23,	 
       '\0'	
};	
BYTE code strHotKeyGameColokIcon[25]=
{
	HotGameFONT_2COLOR_ICON_START+24,
	HotGameFONT_2COLOR_ICON_START+25,
	HotGameFONT_2COLOR_ICON_START+26,
	HotGameFONT_2COLOR_ICON_START+27,
	HotGameFONT_2COLOR_ICON_START+28,
	HotGameFONT_2COLOR_ICON_START+29,	 
	
	HotGameFONT_2COLOR_ICON_START+30,
	HotGameFONT_2COLOR_ICON_START+31,
	HotGameFONT_2COLOR_ICON_START+32,
	HotGameFONT_2COLOR_ICON_START+33,
	HotGameFONT_2COLOR_ICON_START+34,
	HotGameFONT_2COLOR_ICON_START+35,	 
	
	HotGameFONT_2COLOR_ICON_START+36,
	HotGameFONT_2COLOR_ICON_START+37,
	HotGameFONT_2COLOR_ICON_START+38,
	HotGameFONT_2COLOR_ICON_START+39,
	HotGameFONT_2COLOR_ICON_START+40,
	HotGameFONT_2COLOR_ICON_START+41,

	HotGameFONT_2COLOR_ICON_START+42,
	HotGameFONT_2COLOR_ICON_START+43,
	HotGameFONT_2COLOR_ICON_START+44,
	HotGameFONT_2COLOR_ICON_START+45,
	HotGameFONT_2COLOR_ICON_START+46,	 
	HotGameFONT_2COLOR_ICON_START+47,	 
 	 '\0'	
};

BYTE code strHotKeyGameMode1Icon[41]=
{
	HotGameFONT_2COLOR_ICON_START+0,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	HotGameFONT_2COLOR_ICON_START+4,
	HotGameFONT_2COLOR_ICON_START+5,	 
	HotGameFONT_2COLOR_ICON_START+6,
	HotGameFONT_2COLOR_ICON_START+7,
	
	HotGameFONT_2COLOR_ICON_START+8,
	HotGameFONT_2COLOR_ICON_START+9,
	HotGameFONT_2COLOR_ICON_START+10,
	HotGameFONT_2COLOR_ICON_START+11,	 
	HotGameFONT_2COLOR_ICON_START+12,
	HotGameFONT_2COLOR_ICON_START+13,
	HotGameFONT_2COLOR_ICON_START+14,
	HotGameFONT_2COLOR_ICON_START+15,
	
	HotGameFONT_2COLOR_ICON_START+16,
	HotGameFONT_2COLOR_ICON_START+17,	 
	HotGameFONT_2COLOR_ICON_START+18,
	HotGameFONT_2COLOR_ICON_START+19,
	HotGameFONT_2COLOR_ICON_START+20,
	HotGameFONT_2COLOR_ICON_START+21,
	HotGameFONT_2COLOR_ICON_START+22,
	HotGameFONT_2COLOR_ICON_START+23,	

	HotGameFONT_2COLOR_ICON_START+24,
	HotGameFONT_2COLOR_ICON_START+25,
	HotGameFONT_2COLOR_ICON_START+26,
	HotGameFONT_2COLOR_ICON_START+27,
	HotGameFONT_2COLOR_ICON_START+28,
	HotGameFONT_2COLOR_ICON_START+29,	 
	HotGameFONT_2COLOR_ICON_START+30,
	HotGameFONT_2COLOR_ICON_START+31,
	
	HotGameFONT_2COLOR_ICON_START+32,
	HotGameFONT_2COLOR_ICON_START+33,
	HotGameFONT_2COLOR_ICON_START+34,
	HotGameFONT_2COLOR_ICON_START+35,	 
	HotGameFONT_2COLOR_ICON_START+36,
	HotGameFONT_2COLOR_ICON_START+37,
	HotGameFONT_2COLOR_ICON_START+38,
	HotGameFONT_2COLOR_ICON_START+39,
 	 '\0'	
};

BYTE code strHotKeyGameMode2Icon[41]=
{
	HotGameFONT_2COLOR_ICON_START+40,
	HotGameFONT_2COLOR_ICON_START+41,
	HotGameFONT_2COLOR_ICON_START+42,
	HotGameFONT_2COLOR_ICON_START+43,
	HotGameFONT_2COLOR_ICON_START+44,
	HotGameFONT_2COLOR_ICON_START+45,	 
	HotGameFONT_2COLOR_ICON_START+46,
	HotGameFONT_2COLOR_ICON_START+47,
	
	HotGameFONT_2COLOR_ICON_START+48,
	HotGameFONT_2COLOR_ICON_START+49,
	HotGameFONT_2COLOR_ICON_START+50,
	HotGameFONT_2COLOR_ICON_START+51,	 
	HotGameFONT_2COLOR_ICON_START+52,
	HotGameFONT_2COLOR_ICON_START+53,
	HotGameFONT_2COLOR_ICON_START+54,
	HotGameFONT_2COLOR_ICON_START+55,
	
	HotGameFONT_2COLOR_ICON_START+56,
	HotGameFONT_2COLOR_ICON_START+57,	 
	HotGameFONT_2COLOR_ICON_START+58,
	HotGameFONT_2COLOR_ICON_START+59,
	HotGameFONT_2COLOR_ICON_START+60,
	HotGameFONT_2COLOR_ICON_START+61,
	HotGameFONT_2COLOR_ICON_START+62,
	HotGameFONT_2COLOR_ICON_START+63,	

	HotGameFONT_2COLOR_ICON_START+64,
	HotGameFONT_2COLOR_ICON_START+65,
	HotGameFONT_2COLOR_ICON_START+66,
	HotGameFONT_2COLOR_ICON_START+67,
	HotGameFONT_2COLOR_ICON_START+68,
	HotGameFONT_2COLOR_ICON_START+69,	 
	HotGameFONT_2COLOR_ICON_START+70,
	HotGameFONT_2COLOR_ICON_START+71,
	
	HotGameFONT_2COLOR_ICON_START+72,
	HotGameFONT_2COLOR_ICON_START+73,
	HotGameFONT_2COLOR_ICON_START+74,
	HotGameFONT_2COLOR_ICON_START+75,	 
	HotGameFONT_2COLOR_ICON_START+76,
	HotGameFONT_2COLOR_ICON_START+77,
	HotGameFONT_2COLOR_ICON_START+78,
	HotGameFONT_2COLOR_ICON_START+79,
 	 '\0'	
};

BYTE code strHotKeyGameMode1SubIcon[13]=
{
	HotGameFONT_2COLOR_ICON_START+0,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	
	HotGameFONT_2COLOR_ICON_START+4,
	HotGameFONT_2COLOR_ICON_START+5,	 
	HotGameFONT_2COLOR_ICON_START+6,
	HotGameFONT_2COLOR_ICON_START+7,
	
	HotGameFONT_2COLOR_ICON_START+8,
	HotGameFONT_2COLOR_ICON_START+9,
	HotGameFONT_2COLOR_ICON_START+10,
	HotGameFONT_2COLOR_ICON_START+11,	 
 	 '\0'	
};

BYTE code strHotKeyGameMode2SubIcon[13]=
{
	HotGameFONT_2COLOR_ICON_START+12,
	HotGameFONT_2COLOR_ICON_START+13,
	HotGameFONT_2COLOR_ICON_START+14,
	HotGameFONT_2COLOR_ICON_START+15,
	
	HotGameFONT_2COLOR_ICON_START+16,
	HotGameFONT_2COLOR_ICON_START+17,	 
	HotGameFONT_2COLOR_ICON_START+18,
	HotGameFONT_2COLOR_ICON_START+19,
	
	HotGameFONT_2COLOR_ICON_START+20,
	HotGameFONT_2COLOR_ICON_START+21,
	HotGameFONT_2COLOR_ICON_START+22,
	HotGameFONT_2COLOR_ICON_START+23,	
 	 '\0'	
};

BYTE code strHotKeyGameClok_30M_Icon[21]=
{
	HotGameFONT_2COLOR_ICON_START+3*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+3*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+3*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+3*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,

	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	
 	 '\0'	
};

BYTE code strHotKeyGameClok_40M_Icon[21]=
{
	HotGameFONT_2COLOR_ICON_START+4*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+4*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+4*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+4*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,

	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
 	 '\0'	
};

BYTE code strHotKeyGameClok_50M_Icon[21]=
{
	HotGameFONT_2COLOR_ICON_START+5*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+5*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+5*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+5*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,

	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
 	 '\0'	
};

BYTE code strHotKeyGameClok_60M_Icon[21]=
{
	HotGameFONT_2COLOR_ICON_START+6*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+6*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+6*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+6*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,

	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
 	 '\0'	
};

BYTE code strHotKeyGameClok_90M_Icon[21]=
{
	HotGameFONT_2COLOR_ICON_START+9*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+9*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+9*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+9*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,

	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
	
	HotGameFONT_2COLOR_ICON_START,
	HotGameFONT_2COLOR_ICON_START+1,
	HotGameFONT_2COLOR_ICON_START+2,
	HotGameFONT_2COLOR_ICON_START+3,
 	 '\0'	
};


BYTE code strHotKeyGameClokPosition_UpIcon[11]=
{
	HotGameFONT_4COLOR_ICON_START+2*0x08,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0c,
	
	HotGameFONT_4COLOR_ICON_START+2*0x0d,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x11,
 	 '\0'	
};

BYTE code strHotKeyGameClokPosition_CenterIcon[11]=
{
	HotGameFONT_4COLOR_ICON_START+2*0x0a,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0c,
	
	HotGameFONT_4COLOR_ICON_START+2*0x0f,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x11,
 	 '\0'	
};

BYTE code strHotKeyGameClokPosition_DownIcon[11]=
{
	HotGameFONT_4COLOR_ICON_START+2*0x09,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0b,
	HotGameFONT_4COLOR_ICON_START+2*0x0c,
	
	HotGameFONT_4COLOR_ICON_START+2*0x0e,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x10,
	HotGameFONT_4COLOR_ICON_START+2*0x11,
 	 '\0'	
};

BYTE code strHotKeyGameClokShowIcon[11][5]=
{
//==========0
{
	HotGameFONT_2COLOR_ICON_START+0*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+0*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+0*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+0*HotGameClokNumberShift+3,
'\0'	
},
//==========1
{
	HotGameFONT_2COLOR_ICON_START+1*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+1*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+1*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+1*HotGameClokNumberShift+3,
'\0'	
},
//==========2
{
	HotGameFONT_2COLOR_ICON_START+2*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+2*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+2*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+2*HotGameClokNumberShift+3,
'\0'	
},
//==========3
{
	HotGameFONT_2COLOR_ICON_START+3*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+3*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+3*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+3*HotGameClokNumberShift+3,
'\0'	
},
//==========4
{
	HotGameFONT_2COLOR_ICON_START+4*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+4*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+4*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+4*HotGameClokNumberShift+3,
'\0'	
},
//==========5
{
	HotGameFONT_2COLOR_ICON_START+5*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+5*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+5*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+5*HotGameClokNumberShift+3,
'\0'	
},
//==========6
{
	HotGameFONT_2COLOR_ICON_START+6*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+6*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+6*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+6*HotGameClokNumberShift+3,
'\0'	
},
//==========7
{
	HotGameFONT_2COLOR_ICON_START+7*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+7*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+7*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+7*HotGameClokNumberShift+3,
'\0'	
},
//==========8
{
	HotGameFONT_2COLOR_ICON_START+8*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+8*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+8*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+8*HotGameClokNumberShift+3,
'\0'	
},
//==========9
{
	HotGameFONT_2COLOR_ICON_START+9*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+9*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+9*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+9*HotGameClokNumberShift+3,
'\0'	
},
//==========:
{
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+1,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+2,
	HotGameFONT_2COLOR_ICON_START+10*HotGameClokNumberShift+3,
'\0'	
}

};

BYTE* HotKeyGameModetext(void)
{return strGameTitleM18[PROPFONTTBL_INDEX];
}
BYTE* HotKeyGameSelectModetext(void)
{return strGameCursorM18[PROPFONTTBL_INDEX];
}
BYTE* HotKeyGameClokModetext(void)
{return strGameTimeM18[PROPFONTTBL_INDEX];
}
BYTE* HotKeyGameClokPositionModetext(void)
{return strGamePositionM18[PROPFONTTBL_INDEX];
}

BYTE* HotKeyGameModeIcon(void)
{return strHotKeyGameModeIcon;
}
BYTE* HotKeyGameClokIcon(void)
{return strHotKeyGameColokIcon;
}

BYTE* HotKeyGameMode1Icon(void)
{return strHotKeyGameMode1Icon;
}
BYTE* HotKeyGameMode2Icon(void)
{return strHotKeyGameMode2Icon;
}
BYTE* HotKeyGameMode1SubIcon(void)
{return strHotKeyGameMode1SubIcon;
}
BYTE* HotKeyGameMode2SubIcon(void)
{return strHotKeyGameMode2SubIcon;
}

BYTE* HotKeyGameClok_30M_Icon(void)
{return strHotKeyGameClok_30M_Icon;
}
BYTE* HotKeyGameClok_40M_Icon(void)
{return strHotKeyGameClok_40M_Icon;
}
BYTE* HotKeyGameClok_50M_Icon(void)
{return strHotKeyGameClok_50M_Icon;
}
BYTE* HotKeyGameClok_60M_Icon(void)
{return strHotKeyGameClok_60M_Icon;
}
BYTE* HotKeyGameClok_90M_Icon(void)
{return strHotKeyGameClok_90M_Icon;
}

BYTE* HotKeyGameClokPosition_UpIcon(void)
{return strHotKeyGameClokPosition_UpIcon;
}

BYTE* HotKeyGameClokPosition_CenterIcon(void)
{return strHotKeyGameClokPosition_CenterIcon;
}

BYTE* HotKeyGameClokPosition_DownIcon(void)
{return strHotKeyGameClokPosition_DownIcon;
}

BYTE* HotKeyGameClokShowIcon(BYTE number)
{return strHotKeyGameClokShowIcon[number];
}
#endif






//************Main Menu***************************
//************************************************
BYTE* TxtMainQSDescription_1(void)
{  return strContent01M111[PROPFONTTBL_INDEX];
}
BYTE* TxtMainQSDescription_2(void)
{  return strContent02M111[PROPFONTTBL_INDEX];
}
BYTE* TxtMainQSDescription_3(void)
{  return strContent03M111[PROPFONTTBL_INDEX];
}
BYTE* TxtMainQSDescription_4(void)
{  return strContent04M111[PROPFONTTBL_INDEX];
}

BYTE* TxtMainPicDescription_1(void)
{  return strContent01M222[PROPFONTTBL_INDEX];
}
BYTE* TxtMainPicDescription_2(void)
{  return strContent02M222[PROPFONTTBL_INDEX];
}
BYTE* TxtMainPicDescription_3(void)
{  return strContent03M222[PROPFONTTBL_INDEX];
}
BYTE* TxtMainPicDescription_4(void)
{  return strContent04M222[PROPFONTTBL_INDEX];
}

BYTE* TxtMainColDescription_1(void)
{  return strContent01M333[PROPFONTTBL_INDEX];
}
BYTE* TxtMainColDescription_2(void)
{  return strContent02M333[PROPFONTTBL_INDEX];
}
BYTE* TxtMainColDescription_3(void)
{  return strContent03M333[PROPFONTTBL_INDEX];
}
BYTE* TxtMainColDescription_4(void)
{  return strContent04M333[PROPFONTTBL_INDEX];
}


BYTE* TxtMainSysDescription_1(void)
{  return strContent01M444[PROPFONTTBL_INDEX];
}
BYTE* TxtMainSysDescription_2(void)
{  return strContent02M444[PROPFONTTBL_INDEX];
}
BYTE* TxtMainSysDescription_3(void)
{  return strContent03M444[PROPFONTTBL_INDEX];
}
BYTE* TxtMainSysDescription_4(void)
{  return strContent04M444[PROPFONTTBL_INDEX];
}


BYTE* TxtMenu(void)
{  return strMenuM8[PROPFONTTBL_INDEX];
}
BYTE* TxtUp(void)
{  return strUpM8[PROPFONTTBL_INDEX];
}
BYTE* TxtDown(void)
{  return strDownM8[PROPFONTTBL_INDEX];
}
BYTE* TxtExit(void)
{  return strExitM8[PROPFONTTBL_INDEX];
}
BYTE* TxtPower(void)
{  return strPowerM8[PROPFONTTBL_INDEX];
}

//************QuickStart Menu**********************
//************************************************
BYTE* TxtQuickBackLight(void)
{  return strBacklightM1[PROPFONTTBL_INDEX];
}
BYTE* TxtQuickBrightness(void)
{  return strBrithtnessM1[PROPFONTTBL_INDEX];
}
BYTE* TxtQuickContrast(void)
{  return strContrastM1[PROPFONTTBL_INDEX];
}
BYTE* TxtQuickPresetMode(void)
{  return strPresetM1[PROPFONTTBL_INDEX];
}
BYTE* TxtQuickAspectRatio(void)
{  return strAspectRatioM1[PROPFONTTBL_INDEX];
}
BYTE* TxtQuickSource(void)
{  return strSourceM1[PROPFONTTBL_INDEX];
}
BYTE* TxtQuickBlueLight(void)
{  return strBlueLightM1[PROPFONTTBL_INDEX];
}
BYTE* TxtQuickFreeSync(void)
{  return strFreeSyncM1[PROPFONTTBL_INDEX];
}
BYTE* TxtQuickFreeSyncOnOff(void)
{  
   if(INPUT_IS_DVI(MapWin2Port(USER_PREF_WIN_SEL)))
      return strOFFM1[PROPFONTTBL_INDEX];
   else if(FreeSyncFlag)
      return strONM1[PROPFONTTBL_INDEX];
   else
      return strOFFM1[PROPFONTTBL_INDEX];
}

BYTE* TxtQuickAspectRatioStatus(void)
{
 if(USER_PREF_WIDE_MODE(SrcInputPortM)==eASPECT_16_9) 
   return str16_9M1[PROPFONTTBL_INDEX];
else if(USER_PREF_WIDE_MODE(SrcInputPortM)==eASPECT_4_3)
   return str4_3M1[PROPFONTTBL_INDEX];
else if(USER_PREF_WIDE_MODE(SrcInputPortM)==eASPECT_5_4)
   return str5_4M1[PROPFONTTBL_INDEX];
else if(USER_PREF_WIDE_MODE(SrcInputPortM)==eASPECT_1_1)
   return str1_1M1[PROPFONTTBL_INDEX];
else
   return strFullM1[PROPFONTTBL_INDEX];

}

BYTE* TxtQuickPresetModeStatus(void)
{ 
  if(CURRENT_PICTURE_MODE==CT_STANDARD)
   return strStandardM1[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_USER)
   return strUserM1[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_MOVIE)
   return strMovieM1[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_VIVID)
   return strECOM1[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_FPS)
   return strFPSM1[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_RTS)
   return strRTSM1[PROPFONTTBL_INDEX];
else
    return strStandardM1[PROPFONTTBL_INDEX];

}

BYTE* TxtQuickBlueLightStatus(void)
{
 if(USERPREFLOWBLURAY==0) 
   return strOFFM1[PROPFONTTBL_INDEX];
else if(USERPREFLOWBLURAY==1)
   return strPercent10M1[PROPFONTTBL_INDEX];
else if(USERPREFLOWBLURAY==2)
   return strPercent30M1[PROPFONTTBL_INDEX];
else if(USERPREFLOWBLURAY==3)
   return strPercent40M1[PROPFONTTBL_INDEX];
else
   return strOFFM1[PROPFONTTBL_INDEX];

}

BYTE* TxtQuickSourceStatus(void)
{
#if Enable_AutoInput_Menu
if(USER_PREF_INPUT_PRIORITY==INPUT_PRIORITY_AUTO) 
   return strAutoM1[PROPFONTTBL_INDEX];
#if ENABLE_VGA_INPUT
else if(USER_PREF_INPUT_PRIORITY==INPUT_PRIORITY_VGA)
   return strVGAM1[PROPFONTTBL_INDEX];
#endif
#if ENABLE_DVI_INPUT
else if(USER_PREF_INPUT_PRIORITY==INPUT_PRIORITY_DVI)
   return strDVIM1[PROPFONTTBL_INDEX];
#endif
#if ENABLE_HDMI_INPUT
else if(USER_PREF_INPUT_PRIORITY==INPUT_PRIORITY_HDMI)
#if ENABLE_HDMI2ND_INPUT
   return strHDMI1M1[PROPFONTTBL_INDEX];
#else	
   return strHDMIM1[PROPFONTTBL_INDEX];
#endif
#endif
#if ENABLE_HDMI2ND_INPUT
else if(USER_PREF_INPUT_TYPE==INPUT_HDMI2ND)
   return strHDMI2M1[PROPFONTTBL_INDEX];
#endif
#if ENABLE_DP_INPUT
else if(USER_PREF_INPUT_PRIORITY==INPUT_PRIORITY_DP)
   return strDPM1[PROPFONTTBL_INDEX];
#endif


#else
if(USER_PREF_INPUT_TYPE==Input_Nums) 
   return strAutoM1[PROPFONTTBL_INDEX];
#if ENABLE_VGA_INPUT
else if(USER_PREF_INPUT_TYPE==INPUT_VGA)
   return strVGAM1[PROPFONTTBL_INDEX];
#endif
#if ENABLE_DVI_INPUT
else if(USER_PREF_INPUT_TYPE==INPUT_DVI)
   return strDVIM1[PROPFONTTBL_INDEX];
#endif
#if ENABLE_HDMI_INPUT
else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)
#if ENABLE_HDMI2ND_INPUT
   return strHDMI1M1[PROPFONTTBL_INDEX];
#else
   return strHDMIM1[PROPFONTTBL_INDEX];
#endif
#endif
#if ENABLE_HDMI2ND_INPUT
else if(USER_PREF_INPUT_TYPE==INPUT_HDMI2ND)
   return strHDMI2M1[PROPFONTTBL_INDEX];
#endif
#if ENABLE_DP_INPUT
else if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)
   return strDPM1[PROPFONTTBL_INDEX];
#endif


#endif


else
   return strAutoM1[PROPFONTTBL_INDEX];

}


BYTE* TxtBackLightDes01(void)
{
    return strBackLightDes01M9[PROPFONTTBL_INDEX];
}
BYTE* TxtBackLightDes02(void)
{
    return strBackLightDes02M9[PROPFONTTBL_INDEX];
}
BYTE* TxtBrightnessDes01(void)
{
    return strBrightnessDes01M10[PROPFONTTBL_INDEX];
}
BYTE* TxtBrightnessDes02(void)
{
    return strBrightnessDes02M10[PROPFONTTBL_INDEX];
}
BYTE* TxtContrastDes01(void)
{
    return strContrastDes01M11[PROPFONTTBL_INDEX];
}
BYTE* TxtContrastDes02(void)
{
    return strContrastDes02M11[PROPFONTTBL_INDEX];
}
BYTE* TxtPresetDes01(void)
{
    return strPresetDes01M12[PROPFONTTBL_INDEX];
}
BYTE* TxtPresetDes02(void)
{
    return strPresetDes02M12[PROPFONTTBL_INDEX];
}
BYTE* TxtAspectDes01(void)
{
    return strAspectDes01M13[PROPFONTTBL_INDEX];
}
BYTE* TxtAspectDes02(void)
{
    return strAspectDes02M13[PROPFONTTBL_INDEX];
}
BYTE* TxtSourceDes01(void)
{
    return strSourceDes01M33[PROPFONTTBL_INDEX];
}
BYTE* TxtSourceDes02(void)
{
    return strSourceDes02M33[PROPFONTTBL_INDEX];
}
BYTE* TxtBlueLightDes01(void)
{
    return strBlueLightDes01M14[PROPFONTTBL_INDEX];
}
BYTE* TxtBlueLightDes02(void)
{
    return strBlueLightDes02M14[PROPFONTTBL_INDEX];
}
BYTE* TxtFreeSyncDes01(void)
{
    return strFreeSyncDes01M15[PROPFONTTBL_INDEX];
}
BYTE* TxtFreeSyncDes02(void)
{
    return strFreeSyncDes02M15[PROPFONTTBL_INDEX];
}


BYTE* TxtDCRDes01(void)
{
    return strDCRDes01M16[PROPFONTTBL_INDEX];
}
BYTE* TxtDCRDes02(void)
{
    return strDCRDes02M16[PROPFONTTBL_INDEX];
}
BYTE* TxtSharpnessDes01(void)
{
    return strSharpnessDes01M17[PROPFONTTBL_INDEX];
}
BYTE* TxtSharpnessDes02(void)
{
    return strSharpnessDes02M17[PROPFONTTBL_INDEX];
}
BYTE* TxtColorTempDes01(void)
{
    return strColorTempDes01M18[PROPFONTTBL_INDEX];
}
BYTE* TxtColorTempDes02(void)
{
    return strColorTempDes02M18[PROPFONTTBL_INDEX];
}

BYTE* TxtColorTempAdvancedDes01(void)
{
    return strAdvancedDes01M32[PROPFONTTBL_INDEX];
}
BYTE* TxtColorTempAdvancedDes02(void)
{
    return strAdvancedDes02M32[PROPFONTTBL_INDEX];
}

BYTE* TxtGammaDes01(void)
{
    return strGammaDes01M19[PROPFONTTBL_INDEX];
}
BYTE* TxtGammaDes02(void)
{
    return strGammaDes02M19[PROPFONTTBL_INDEX];
}

BYTE* TxtTintDes01(void)
{
    return strTintDes01M20[PROPFONTTBL_INDEX];
}
BYTE* TxtTintDes02(void)
{
    return strTintDes02M20[PROPFONTTBL_INDEX];
}
BYTE* TxtSaturationDes01(void)
{
    return strSaturationDes01M21[PROPFONTTBL_INDEX];
}
BYTE* TxtSaturationDes02(void)
{
    return strSaturationDes02M21[PROPFONTTBL_INDEX];
}

BYTE* TxtODDes01(void)
{
    return strODDes01M22[PROPFONTTBL_INDEX];
}
BYTE* TxtODDes02(void)
{
    return strODDes02M22[PROPFONTTBL_INDEX];
}
BYTE* TxtSleepModeDes01(void)
{
    return strSleepModeDes01M23[PROPFONTTBL_INDEX];
}
BYTE* TxtSleepModeDes02(void)
{
    return strSleepModeDes02M23[PROPFONTTBL_INDEX];
}
BYTE* TxLedModeAllDes01(void)
{
    return strLEDModeAllDes01M37[PROPFONTTBL_INDEX];
}
BYTE* TxLedModeAllDes02(void)
{
    return strLEDModeAllDes02M37[PROPFONTTBL_INDEX];
}


BYTE* TxLedModeDes01(void)
{
    return strLEDModeDes01M34[PROPFONTTBL_INDEX];
}
BYTE* TxLedModeDes02(void)
{
    return strLEDModeDes02M34[PROPFONTTBL_INDEX];
}
BYTE* TxLedColorDes01(void)
{
    return strLEDColorDes01M35[PROPFONTTBL_INDEX];
}
BYTE* TxLedColorDes02(void)
{
    return strLEDColorDes02M35[PROPFONTTBL_INDEX];
}
BYTE* TxLedBrightnessDes01(void)
{
    return strLEDBrightnessDes01M36[PROPFONTTBL_INDEX];
}
BYTE* TxLedBrightnessDes02(void)
{
    return strLEDBrightnessDes02M36[PROPFONTTBL_INDEX];
}



BYTE* TxtLanguageDes01(void)
{
    return strLanguageDes01M24[PROPFONTTBL_INDEX];
}
BYTE* TxtLanguageDes02(void)
{
    return strLanguageDes02M24[PROPFONTTBL_INDEX];
}
BYTE* TxtOSDHDes01(void)
{
    return strOSDHDes01M25[PROPFONTTBL_INDEX];
}
BYTE* TxtOSDHDes02(void)
{
    return strOSDHDes02M25[PROPFONTTBL_INDEX];
}
BYTE* TxtOSDVDes01(void)
{
    return strOSDVDes01M26[PROPFONTTBL_INDEX];
}
BYTE* TxtOSDVDes02(void)
{
    return strOSDVDes02M26[PROPFONTTBL_INDEX];
}
BYTE* TxtOSDTimerDes01(void)
{
    return strOSDTimerDes01M27[PROPFONTTBL_INDEX];
}
BYTE* TxtOSDTimerDes02(void)
{
    return strOSDTimerDes02M27[PROPFONTTBL_INDEX];
}
BYTE* TxtOSDTransDes01(void)
{
    return strOSDTransDes01M28[PROPFONTTBL_INDEX];
}
BYTE* TxtOSDTransDes02(void)
{
    return strOSDTransDes02M28[PROPFONTTBL_INDEX];
}
BYTE* TxVolumeDes01(void)
{
    return strVolumeDes01M29[PROPFONTTBL_INDEX];
}
BYTE* TxVolumeDes02(void)
{
    return strVolumeDes02M29[PROPFONTTBL_INDEX];
}
BYTE* TxResetDes01(void)
{
    return strResetDes01M30[PROPFONTTBL_INDEX];
}
BYTE* TxResetDes02(void)
{
    return strResetDes02M30[PROPFONTTBL_INDEX];
}


//************Picture Menu*************************
//************************************************

BYTE* TxtPictureBackLight(void)
{  return strBacklightM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPictureContrast(void)
{  return strContrastM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPictureBrightness(void)
{  return strBrithtnessM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPicturePresetMode(void)
{  return strPresetM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPictureDCR(void)
{  return strDCRM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPictureAspectRatio(void)
{  return strAspectRatioM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPictureSharpness(void)
{  return strSharpnessM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPicturePresetModeStatus(void)
{ 
  if(CURRENT_PICTURE_MODE==CT_STANDARD)
   return strStandardM2[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_USER)
   return strUserM2[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_MOVIE)
   return strMovieM2[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_VIVID)
   return strECOM2[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_FPS)
   return strFPSM2[PROPFONTTBL_INDEX];
 else if(CURRENT_PICTURE_MODE==CT_RTS)
   return strRTSM2[PROPFONTTBL_INDEX];
else
    return strStandardM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPictureDCRStatus(void)
{  
if((CURRENT_PICTURE_MODE==CT_STANDARD)||(CURRENT_PICTURE_MODE==CT_USER))
{
  if(DCR_ENABLE_FLAG)
   return strONM2[PROPFONTTBL_INDEX];
  else
   return strOFFM2[PROPFONTTBL_INDEX];
}
else
   return strOFFM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPictureAspectRatioStatus(void)
{  
 if(USER_PREF_WIDE_MODE(SrcInputPortM)==eASPECT_16_9) 
   return str16_9M2[PROPFONTTBL_INDEX];
else if(USER_PREF_WIDE_MODE(SrcInputPortM)==eASPECT_4_3)
   return str4_3M2[PROPFONTTBL_INDEX];
else if(USER_PREF_WIDE_MODE(SrcInputPortM)==eASPECT_5_4)
   return str5_4M2[PROPFONTTBL_INDEX];
else if(USER_PREF_WIDE_MODE(SrcInputPortM)==eASPECT_1_1)
   return str1_1M2[PROPFONTTBL_INDEX];
else
   return strFullM2[PROPFONTTBL_INDEX];
}

BYTE* TxtAuto_Adjust(void)
{  return strAutoAdjustM2[PROPFONTTBL_INDEX];
}
BYTE* TxtAuto_Color(void)
{  return strAutoColorM2[PROPFONTTBL_INDEX];
}
BYTE* TxtH_Position(void)
{  return strHPositionM2[PROPFONTTBL_INDEX];
}
BYTE* TxtV_Position(void)
{  return strVPositionM2[PROPFONTTBL_INDEX];
}
BYTE* TxtClock(void)
{  return strClockM2[PROPFONTTBL_INDEX];
}
BYTE* TxtPhase(void)
{  return strPhaseM2[PROPFONTTBL_INDEX];
}



//************Color Menu*************************
//************************************************

BYTE* TxtColorTemperature(void)
{  return strColorTempM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorGamma(void)
{  return strGammaM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorTint(void)
{  return strTintM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorSaturation(void)
{  return strSaturationM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorBlueLight(void)
{  return strBlueLightM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorTemperatureStatus(void)
{ 
  if(CURRENT_COLOR_TEMP==PCT_5700K)
    return strWarmM3[PROPFONTTBL_INDEX];	
 else if(CURRENT_COLOR_TEMP==PCT_6500K)
    return strNormalM3[PROPFONTTBL_INDEX];	
 else if(CURRENT_COLOR_TEMP==PCT_9300K)
    return strCoolM3[PROPFONTTBL_INDEX];	
 //else if(CURRENT_COLOR_TEMP==PCT_Advanced)
//    return strAdvancedM3[PROPFONTTBL_INDEX];	
 else
    return strUSERM3[PROPFONTTBL_INDEX];	
  
}


BYTE* TxtAdvanced(void)
{  return strAdvancedM3[PROPFONTTBL_INDEX];
}

BYTE* TxtColorRed(void)
{  return strRedM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorGreen(void)
{  return strGreenM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorBlue(void)
{  return strBlueM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorCyan(void)
{  return strCyanM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorMagenta(void)
{  return strMagentaM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorYellow(void)
{  return strYellowM3[PROPFONTTBL_INDEX];
}
BYTE* TxtColorGammaStatus(void)
{  
  if(CURRENT_GAMMA==eGAMMA_18)
   return strGamma18M3[PROPFONTTBL_INDEX];
  else if(CURRENT_GAMMA==eGAMMA_20)
   return strGamma20M3[PROPFONTTBL_INDEX];
  else if(CURRENT_GAMMA==eGAMMA_22)
   return strGamma22M3[PROPFONTTBL_INDEX];
  else if(CURRENT_GAMMA==eGAMMA_24)
   return strGamma24M3[PROPFONTTBL_INDEX];
  else
   return strOFFM3[PROPFONTTBL_INDEX];
  
}
BYTE* TxtColorBlueLightStatus(void)
{  
  if(USERPREFLOWBLURAY==1)
   return strPercent10M3[PROPFONTTBL_INDEX];
  else if(USERPREFLOWBLURAY==2)
   return strPercent30M3[PROPFONTTBL_INDEX];
  else if(USERPREFLOWBLURAY==3)
   return strPercent40M3[PROPFONTTBL_INDEX];
  else
   return strOFFM3[PROPFONTTBL_INDEX];
}


//************System Menu*************************
//************************************************

BYTE* TxtSystemOverDrive(void)
{  return strOverdriveM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemFreeSync(void)
{  return strFreeSyncM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemSleepMode(void)
{  return strSleepModeM4[PROPFONTTBL_INDEX];
}
#if LED_HOT_Menu_Func
BYTE* TxtSystemLedModeText(void)
{  return strLedModeM4[PROPFONTTBL_INDEX];
}
#endif
BYTE* TxtSystemLanguage(void)
{  return strLanguageM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemOSDHPosition(void)
{  return strOSDHPositionM4[PROPFONTTBL_INDEX];
}
#if Enable_OSD_Vposition
BYTE* TxtSystemOSDVPosition(void)
{  return strOSDVPositionM4[PROPFONTTBL_INDEX];
}
#endif
BYTE* TxtSystemOSDTimer(void)
{  return strOSDTimerM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemOSDTransparency(void)
{  return strOSDTransparencyM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemVolume(void)
{  return strVolumeM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemReset(void)
{  return strResetM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemConfirm(void)
{  return strConfirmM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemCancel(void)
{  return strCancelM4[PROPFONTTBL_INDEX];
}

BYTE* TxtSystemLanguageStatus(void)
{
 if(USER_PREF_LANGUAGE==LANG_English)
   return strEnglishM4[PROPFONTTBL_INDEX];
 else if(USER_PREF_LANGUAGE==LANG_Spanish)
   return strEspanolM4[PROPFONTTBL_INDEX];
 else if(USER_PREF_LANGUAGE==LANG_French)
   return strFrancaisM4[PROPFONTTBL_INDEX];
 else if(USER_PREF_LANGUAGE==LANG_TChinese)
   return strSChineseM4[PROPFONTTBL_INDEX];
 else
   return strEnglishM4[PROPFONTTBL_INDEX];
}

BYTE* TxtSystemOverDriveStatus(void)
{ 
 if(USER_PREF_RESPONSE_TIME==RT_ON)
   return strONM4[PROPFONTTBL_INDEX];
 else
   return strOFFM4[PROPFONTTBL_INDEX];
}

BYTE* TxtSystemFreeSyncStatus(void)
{ 
 if(INPUT_IS_DVI(MapWin2Port(USER_PREF_WIN_SEL)))
   return strOFFM4[PROPFONTTBL_INDEX];
 else if(FreeSyncFlag)
   return strONM4[PROPFONTTBL_INDEX];
 else
   return strOFFM4[PROPFONTTBL_INDEX];
}

BYTE* TxtSystemSleepModeStatus(void)
{ 
 if(USER_PREF_SLEEPMODE==SleepMode_30Min)
   return strSleepMode30minM4[PROPFONTTBL_INDEX];
 else if(USER_PREF_SLEEPMODE==SleepMode_60Min)
   return strSleepMode60minM4[PROPFONTTBL_INDEX];
 else if(USER_PREF_SLEEPMODE==SleepMode_120Min)
   return strSleepMode120minM4[PROPFONTTBL_INDEX];
 else
   return strOFFM4[PROPFONTTBL_INDEX];
}

BYTE* TxtSystemResetStatus(void)
{ 
 if(g_u8MenuTempValue==1)
   return strConfirmM4[PROPFONTTBL_INDEX];
 else
   return strCancelM4[PROPFONTTBL_INDEX];
}


BYTE* TxtSystemTransStatus(void)
{ 
 if(USER_PREF_TRANSPARENCY==1)
   return strOSDTrans01M51[PROPFONTTBL_INDEX];
else if (USER_PREF_TRANSPARENCY==2)
   return strOSDTrans02M52[PROPFONTTBL_INDEX];
else if (USER_PREF_TRANSPARENCY==3)
   return strOSDTrans03M53[PROPFONTTBL_INDEX];
else if (USER_PREF_TRANSPARENCY==4)
   return strOSDTrans04M54[PROPFONTTBL_INDEX];
 else
   return strOSDTrans00M50[PROPFONTTBL_INDEX];
}

#if LED_HOT_Menu_Func
BYTE* TxtSystemLedColor(void)
{ 
   return strColorM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemLedBrightness(void)
{ 
   return strBrightnessM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemLedMode(void)
{ 
   return strLightModeM4[PROPFONTTBL_INDEX];
}

BYTE* TxtSystemLedColorStatus(void)
{ 
if(USER_PREF_LEDColorful_MODE==LedRed)
   return strRedM4[PROPFONTTBL_INDEX];
else if(USER_PREF_LEDColorful_MODE==LedYellow)
   return strYellowM4[PROPFONTTBL_INDEX];
else if(USER_PREF_LEDColorful_MODE==LedGreen)
   return strGreenM4[PROPFONTTBL_INDEX];
else if(USER_PREF_LEDColorful_MODE==LedCyan)
   return strCyanM4[PROPFONTTBL_INDEX];
else if(USER_PREF_LEDColorful_MODE==LedBlue)
   return strBlueM4[PROPFONTTBL_INDEX];
else if(USER_PREF_LEDColorful_MODE==LedPurple)
   return strPurpleM4[PROPFONTTBL_INDEX];
//else if(USER_PREF_LEDColorful_MODE==LedWhite)
//   return strWhiteM4[PROPFONTTBL_INDEX];
else
   return strColorfulM4[PROPFONTTBL_INDEX];
}
BYTE* TxtSystemLedBrightnessStatus(void)
{ 
if(USER_PREF_LedStrength==LedMin)
   return strSoftM4[PROPFONTTBL_INDEX];
else if(USER_PREF_LedStrength==LedMid)
   return strNormalM4[PROPFONTTBL_INDEX];
else if(USER_PREF_LedStrength==LedMax)
   return strStrongM4[PROPFONTTBL_INDEX];
else
   return strOFFM4[PROPFONTTBL_INDEX];
	
}
BYTE* TxtSystemLedModeStatus(void)
{ 
if(USER_PREF_LedModeType==LedBreath)
   return strBreatheM4[PROPFONTTBL_INDEX];
else if(USER_PREF_LedModeType==LedFlicker)
   return strFlickerM4[PROPFONTTBL_INDEX];
else 
   return strFixedM4[PROPFONTTBL_INDEX];
	
}
#endif
BYTE code strLedMode[]=
{
    ARROW_RIGHT,ARROW_RIGHT,0x00
};

BYTE* TxtSystemLedModeIcon(void)
{ 
   return strLedMode;
}
#if LED_HOT_Menu_Func
BYTE* TxtSystemLedModeIndexStatus(void)
{ 
 if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)
   return strColorfulM4[PROPFONTTBL_INDEX];
 else if(USER_PREF_LEDMODE_INDEX==LedModeIndex_User)
   return strLEDModeUserM4[PROPFONTTBL_INDEX];
 else
   return strOFFM4[PROPFONTTBL_INDEX];
}
#endif










//Multi-Window Settings  //M108



