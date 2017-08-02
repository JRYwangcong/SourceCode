#define _FMENUPROPSTR_C

#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "menudef.h"
#include "FMenuPropStr.h"
#include "MDebug.h"


/// Prop. String ===================================================

BYTE code strAutoColorM20[]=
{
    //123456789012345678901234567890
    "Auto Color"
};
BYTE code strPassM20[]=
{
    //123456789012345678901234567890
    "Pass"
};
BYTE code strFailM20[]=
{
    //123456789012345678901234567890
    "Fail"
};
BYTE code strGainM20[]=
{
    //123456789012345678901234567890
    "Gain"
};
BYTE code strOffsetM20[]=
{
    //123456789012345678901234567890
    "Offset"
};
BYTE code strRedM20[]=
{
    //123456789012345678901234567890
    "R"
};
BYTE code strGreenM20[]=
{
    //123456789012345678901234567890
    "G"
};
BYTE code strBlueM20[]=
{
    //123456789012345678901234567890
    "B"
};
BYTE code strColorTempM20[]=
{
    //123456789012345678901234567890
    "ColorTemp"
};
BYTE code str6500KM20[]=
{
    //123456789012345678901234567890
    "6500K"
};
BYTE code str5700KM20[]=
{
    //123456789012345678901234567890
#ifdef SenYo_Project  
    "7300K"
#elif defined(KTC_Project)
    "7500K"
#else
    "5400K"
#endif    
};

BYTE code str9300KM20[]=
{
    //123456789012345678901234567890
    "9300K"
};
BYTE code strBurninM20[]=
{
    //123456789012345678901234567890
    "BrunIn"
};
BYTE code strOnM20[]=
{
    //123456789012345678901234567890
    "On "
};
BYTE code strOffM20[]=
{
    //123456789012345678901234567890
    "Off"
};
BYTE code strBankM20[]=
{
    //123456789012345678901234567890
    "Bank"
};
BYTE code strScalerM20[]=
{
    //123456789012345678901234567890
    "Scaler"
};
BYTE code strADCM20[]=
{
    //123456789012345678901234567890
    "ADC"
};
BYTE code strTConM20[]=
{
    //123456789012345678901234567890
    "Tcon"
};
BYTE code strAddrM20[]=
{
    //123456789012345678901234567890
    "Addr"
};
BYTE code strValueM20[]=
{
    //123456789012345678901234567890
    "Value"
};
BYTE code strExitM20[]=
{
    //123456789012345678901234567890
    "Exit"
};

#if defined(KTC_Project)
BYTE code strKeyTestM20[]=
{
    //123456789012345678901234567890
    "Key Test"
};
#endif

BYTE code strBacklighttimeM20[]=
{
    //123456789012345678901234567890
    "BLightTime\x02F"
};
BYTE code strHourM20[]=
{
    //123456789012345678901234567890
    "Hour"
};
/*
BYTE code strRestBacklightM20[]=
{
    //123456789012345678901234567890
    "ResetBackLight"
};
*/
BYTE code strMinM20[]=
{
    //123456789012345678901234567890
    "Min"
};
BYTE code strFactoryResetM20[]=
{
    //123456789012345678901234567890
    "FactoryReset"
};
BYTE code strEDIDWPM20[]=
{
    //123456789012345678901234567890
    "EDIDWP"
};

BYTE code strLOGOM20[]=
{
    //123456789012345678901234567890
    "LOGO"
};

BYTE code strPanelSSCFreq[] = 
{
    //123456789012345678901234567890
    "Panel SSC Freq"
};

BYTE code strPanelSSCPermillage[] = 
{
    //123456789012345678901234567890
    "Panel SSC Perm"
};

BYTE code strDDRSSCFreq[] = 
{
    //123456789012345678901234567890
    "DDR SSC Freq"
};

BYTE code strDDRSSCPermillage[] = 
{
    //123456789012345678901234567890
    "DDR SSC Perm"
};


BYTE code strFactoryResetTextM20[] = 
{
    //123456789012345678901234567890
    "Factroy Reset"
};



#if ENABLE_SRGB_FACTORY_ITEM
BYTE code strsRGBM20[]=
{
    //123456789012345678901234567890
    "sRGB"
};
#endif



BYTE code strChangeDate[] =
{
    //123456789012345678901234567890
    ChangeDate
};

BYTE code strFWVerSion[] =
{
    //123456789012345678901234567890
    FWVersion
};


BYTE code strChip[] =
{
    //123456789012345678901234567890
  #if (PanelType==PanelLSM315HP01||PanelType==PanelLSM315DP02||PanelType==PanelM315DVR011||PanelType==PanelLSM270HP06)
    "MST9104"
  #else
 #if (PanelType==PanelLSM315DP01||PanelType==PanelLTM340YP01||PanelType==PanelLSM270DP01)   
    "MST9U11"
 #else
    "MST9U13"
 #endif
 #endif
};

BYTE code strPanelName[] =  //
{
    //123456789012345678901234567890
    PanelName
};
BYTE code strPanelSize[] =  //
{
    //123456789012345678901234567890
    "Size\x02F"
};
BYTE code strPanel[] =  //
{
    //123456789012345678901234567890
    "Panel\x02F"
};
BYTE code PanelSizeNumber[] =  //
{
    //123456789012345678901234567890
    PanelSize_Num
};




#if 0//ENABLE_ENERGY_STAR_LOGO
//Sky101029, about ES logo on/off
BYTE code strESLogoM20[1][2]=
{ { 0x97, 0x05},
};
#endif

#if 0 //White120131,update BID_Counter_for_FY13
BYTE code strResetBIDM20[1][2]=
{ { 0x9C, 0x05},
};
#endif
#if 0
BYTE code strBIDConutM20[1][2]=
{ { 0xA1, 0x02},
};
#endif


#if 0//ENABLE_COLOR_TEMP_OSD

BYTE code str10000KM20[1][2]=
{ { 0xA3, 0x04},
};
BYTE code str7500KM20[1][2]=
{ { 0xA7, 0x04},
};
BYTE code str5000KM20[1][2]=
{ { 0xAB, 0x04},
};

#endif


///String Functions============================================================
#if ENABLE_VGA_INPUT
BYTE* AutoColorText(void)
{ return strAutoColorM20;
}

BYTE* AdcGainText(void)
{ return strGainM20;
}
#endif

BYTE* FactoryRedText(void)
{ return strRedM20;
}
BYTE* FactoryGreenText(void)
{ return strGreenM20;
}
BYTE* FactoryBlueText(void)
{ return strBlueM20;
}
#if ENABLE_VGA_INPUT
BYTE* AdcOffsetText(void)
{ return strOffsetM20;
}
#endif

BYTE* FWarmText(void) //JL
{ return str5700KM20;
}
BYTE* FCoolText(void)
{ return str9300KM20;
}
BYTE* FNormalText(void)
{ return str6500KM20;
}

#if ENABLE_SRGB_FACTORY_ITEM//ENABLE_SRGB_ITEM //Sky120318//Chihlung 20120321
BYTE* FsRGBText(void)
{ return strsRGBM20;
}
#endif

BYTE* BurninText(void)
{ return strBurninM20;
}
BYTE* BurninOnOffText(void)
{ return (BURNIN_MODE_FLAG)?(strOnM20):(strOffM20);
}

BYTE* FEDIDWPOnOffText(void)   //JL
{ //return (HW_DDCWP_PIN)?(strOnM20[0]):(strOffM20[0]);
  return (DDC_WP_FLAG)?(strOnM20):(strOffM20);
}

BYTE *F_PanelSSCFreqText(void)
{
    return strPanelSSCFreq;
}

BYTE *F_PanelSSCPermillageText(void)
{
    return strPanelSSCPermillage;
}
BYTE *F_DDRSSCFreqText(void)
{
    return strDDRSSCFreq;
}

BYTE *F_DDRSSCPermillageText(void)
{
    return strDDRSSCPermillage;
}


#if 1//ENABLE_ENERGY_STAR_LOGO
BYTE* FESLogoText(void)
{
   return strLOGOM20;
}

BYTE* LogoOnOffText(void)
{ 
  return (OSD_LOGO_FLAG)?(strOnM20):(strOffM20);
}
#endif
BYTE* EDIDWPText(void)
{
   return strEDIDWPM20;
}

#if 0
BYTE* BankText(void)
{ return strBankM20[0];
}

BYTE* BankNameText(void)
{ if (g_u8MSBankNo==1)
    return strADCM20[0];
  else if (g_u8MSBankNo==2)
    return strTConM20[0];
  else
    return strScalerM20[0];
}

BYTE* AddressText(void)
{ return strAddrM20[0];
}
BYTE* ValueText(void)
{ return strValueM20[0];
}
#endif
BYTE* ExitText(void)
{ return strExitM20;
}

#if defined(KTC_Project)
BYTE* KeyTestText(void)
{ return strKeyTestM20;
}

#endif

#if ENABLE_VGA_INPUT
BYTE* FAutoColorText(void)
{ return strAutoColorM20;
}
#endif

#if F_ResetItem
BYTE* FactoryResetText(void)
{
   return strFactoryResetTextM20;
}

#endif

/*
BYTE* ResetBacklightText(void)
{
   return strRestBacklightM20;
}
*/
BYTE* DateText(void)
{
return strChangeDate;	
}
BYTE* FWVerSionText(void)
{
return strFWVerSion;	
}

BYTE *F_ChipText(void)
{
    return strChip;
}
BYTE *F_PanelText(void)
{
   return strPanel;
}
BYTE *F_PanelNameText(void)
{
    return strPanelName;
}
BYTE *F_PanelSizeText(void)
{
    return strPanelSize;
}
BYTE *F_PanelSizeNumber(void)
{
    return PanelSizeNumber;
}

///------------------------------------------------------------------------
///------------------------------------------------------------------------
BYTE* BacklightTimeText(void)
{
   return strBacklighttimeM20;
}

BYTE* HourText(void)
{
	return strHourM20;
}

#if ENABLE_VGA_INPUT
BYTE* PassText(void)
{ return strPassM20;
}
BYTE* FailText(void)
{ return strFailM20;
}
#endif
#if 0  //JL
BYTE* PanelText(void)
{ return strPanelM20[0];
}

BYTE* FactoryVersionText(void)
{
    return strFactoryVersionM20[0];
}
#endif
BYTE* MinText(void)
{ return strMinM20;
}
BYTE* ColorTempText(void)
{ return strColorTempM20;
}

#if ENABLE_VGA_INPUT||F_ResetItem
BYTE* FPassText(void)
{ return strPassM20;
}
BYTE* FFailText(void)
{ return strFailM20;
}
#endif
extern BYTE xdata  tPropText[][2];
#include "LoadPropFont1218.h"
#include "PropFontExtFunc.h"
#if 0
BYTE* FactoryModelName(void)
{
/*
   #if defined(_US2208W_)
       return strModelNameM20;
   #else
	return  strMode19M20;
   #endif
*/
    BYTE ModelName[16]=MODEL_NAME;
    ModelName[15]=0; //end string

    tPropText[1][0]=strMode19M20[0][0] ;
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE1PIXEL|LEFT_ALIGN;
    tPropText[1][1]=LoadPropFontText(tPropText[1][0],(ModelName+5));
    return tPropText[1];
}
#endif
BYTE* FClearUesrAreaText(void)
{
  return strFactoryResetM20;
}

#if 0//ENABLE_ENERGY_STAR_LOGO
//Sky101029, about ES logo on/off
BYTE* FESLogoText(void)
{
  return strESLogoM20[0];
}
#endif

#if 0 //White120131,update BID_Counter_for_FY13
BYTE* ResetBIDText(void) //White111107,BID_Counter_for_FY13
{ return strResetBIDM20[0];
}
#endif
#if 0
BYTE* BIDCountText(void)
{ return strBIDConutM20[0];
}
#endif
