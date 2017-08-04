#include "types.h"
#include "board.h"
#include "global.h"
#include "keypaddef.h"
#include "menudef.h"
#include "menutext.h"
#include "menuTextProp.h"
//========= ICON =========



#if PANELSSCSTEPLFUN
  BYTE *SSCSTEPLText(void)
    {
        return strSSCSTEPL;
    }
    BYTE *SSCSTEPHText(void)
    {
        return strSSCSTEPH;
    }
    BYTE *SSCSPANLText(void)
    {
        return strSSCSPANL;
    }
    BYTE *SSCSPANHText(void)
    {
        return strSSCSPANH;
    }
#endif




//BYTE *Main_FText(void)
//{
//    return strFactory;
//}

BYTE *MainMonoText(void)
{
	return 0;
}



#if DISPLAY_CHECK_VOL
BYTE code strChecksumText[]=
{
	_C,_h,_e,_c,_k,_s,_u,_m,'\x0'
};
BYTE *ChecksumText(void)
{
	return strChecksumText;
}
/*
BYTE code strChecksum[]=
{
	_C,_A,_A,_A,'\x0'
};

BYTE *Checksum(void)
{
	return strChecksum;
}
*/
#endif 


//========= TEXT =========
BYTE *NoSyncText(void)
{
	#if LANGUAGE_TYPE == All14Language_AddJap || LANGUAGE_TYPE==ASIA
    return strNoSyncM50[UserPrefLanguage];
	#else
    return strNoSyncM2[UserPrefLanguage];
	#endif
}

#if INPUT_TYPE==INPUT_1A
BYTE * CableNotConnectText( void )
{
return strCableNotConnectM4[UserPrefLanguage];
}
#endif
BYTE *OutOfRangeText(void)
{
    return strUnSupportM5[UserPrefLanguage];
}
BYTE *ResetText_1(void)
{
    return strResetText_1M54[UserPrefLanguage];
}

BYTE *ResetText_2(void)
{
    return strResetText_2M54[UserPrefLanguage];
}
#if 0
BYTE *AutoText_1(void)
{
    return strAutoText_1M55[UserPrefLanguage];
}

BYTE *AutoText_2(void)
{
    return strAutoText_2M55[UserPrefLanguage];
}
#endif
BYTE *OsdLockText(void)
{
    return strOsdLockM56[UserPrefLanguage];
}
#if 0
BYTE *LuminanceText(void)
{
    return strLuminanceM7[UserPrefLanguage];
}
#endif
BYTE *ContrastText(void)
{
    return strContrastM7[UserPrefLanguage];
}
BYTE *BrightnessText(void)
{
    return strBrightnessM7[UserPrefLanguage];
}

#if 0 // AudioFunc
    BYTE *VolumeText(void)
    {
        return strVolumeM58[UserPrefLanguage];
    }

    BYTE *MuteText(void)
    {
        return strMuteM58[UserPrefLanguage];
    }
#endif
BYTE *ECOModeText(void)
{
    return strEcoModeM7[UserPrefLanguage];
}
BYTE *ECOModeValue(void)
{
    if (UserPrefECOMode == 0)
    {
        return strECOStandM7[UserPrefLanguage];
    }
    else if (UserPrefECOMode == 1)
    {
        return strECOTextM7[UserPrefLanguage];
    }
    else if (UserPrefECOMode == 2)
    {
        return strECOInternetM7[UserPrefLanguage];
    }
    else if (UserPrefECOMode == 3)
    {
        return strECOgameM7[UserPrefLanguage];
    }
    else if (UserPrefECOMode == 4)
    {
        return strECOMovieM7[UserPrefLanguage];
    }
    else if (UserPrefECOMode == 5)
    {
        return strECOSportM7[UserPrefLanguage];
    }
    else
    {
        return strECOStandM7[UserPrefLanguage];
    }	
    //Benz 2007.4.23   15:41:25  why ecomode =6   count 0~5 so disable
    /*
    else if ( UserPrefECOMode == 6 )
    {
    return strECOSportM7[UserPrefLanguage];
    }
     */
}
BYTE *GAMAText(void)
{
    return strGAMAM7[UserPrefLanguage];
}
BYTE *GamaModeValue(void)
{
    if (UserPrefGamaMode == 0)
    {
        return strGama1M7[UserPrefLanguage];
    }
    else if (UserPrefGamaMode == 1)
    {
        return strGama2M7[UserPrefLanguage];
    }
    else if (UserPrefGamaMode == 2)
    {
        return strGama3M7[UserPrefLanguage];
    }
    else
   {
        return strGama1M7[UserPrefLanguage];
    }
}
BYTE *DCRText(void)
{
    return strDCRM7[UserPrefLanguage];
}
BYTE *DcrValueText(void)
{

#if 1
    if (UserPrefDcrMode)
    {
        return strOnM7[UserPrefLanguage];
    }
    else
    {
        return strOffM7[UserPrefLanguage];
    }
#else
return 0;
#endif

}
#if Enable_Lightsensor
BYTE *iCareText(void)
{
    return stricareM7[UserPrefLanguage];
}
BYTE *iCareValueText(void)
{
    if(UserPrefiCareMode)
        return strOnM7[UserPrefLanguage];
    else
        return strOffM7[UserPrefLanguage];
}
#endif

#if 0
BYTE *Main_FText(void)
{
    return strMain_FM60[UserPrefLanguage];
}
#endif
#if 0
BYTE *ImageSetupText(void)
{
    return strImageSetUpM13[UserPrefLanguage];
}
#endif
BYTE *ClockText(void)
{
    return strClockM13[UserPrefLanguage];
}
BYTE *FocusText(void)
{
    return strFocusM13[UserPrefLanguage];
}
BYTE *HPositionText(void)
{
    return strHPositionM13[UserPrefLanguage];
}
BYTE *VPositionText(void)
{
    return strVPositionM13[UserPrefLanguage];
}
BYTE *ColorTempText(void)
{
    return strColorTempM18[UserPrefLanguage];
}
BYTE *ColorTempValueText(void)
{
    if (UserPrefColorTemp == CTEMP_Warm1)
    {
        return strWarm1M18[UserPrefLanguage];
    }
    //   else if (UserPrefColorTemp==CTEMP_Warm2)
    //      return strWarm2M18[UserPrefLanguage];
    else if (UserPrefColorTemp == CTEMP_Normal)
    {
        return strNormalM18[UserPrefLanguage];
    }
    else if (UserPrefColorTemp == CTEMP_Cool1)
    {
        return strCool1M18[UserPrefLanguage];
    }
    //  else if (UserPrefColorTemp==CTEMP_Cool2)
    //     return strCool2M18[UserPrefLanguage];
    else if (UserPrefColorTemp == CTEMP_SRGB)
    {
        return strsRGBM18[UserPrefLanguage];
    }
    else if (UserPrefColorTemp == CTEMP_USER)
    {
        return strUserM18[UserPrefLanguage];
    }
    else
    {
        return strWarm1M18[UserPrefLanguage];
    }
}
BYTE *UserColorRText(void)
{
    return strUserColorRM18[UserPrefLanguage];
}
BYTE *UserColorGText(void)
{
    return strUserColorGM18[UserPrefLanguage];
}
BYTE *UserColorBText(void)
{
    return strUserColorBM18[UserPrefLanguage];
}
#if Enable_YMC
    BYTE *UserColorYText(void)
    {
        return strUserColorYM18[UserPrefLanguage];
    }
    BYTE *UserColorCText(void)
    {
        return strUserColorCM18[UserPrefLanguage];
    }
    BYTE *UserColorMText(void)
    {
        return strUserColorMM18[UserPrefLanguage];
    }
#endif
#if AUTO_COLOR_ITEM_EN
BYTE *AutoColorText1(void)
{
    return strAutoColorM18[UserPrefLanguage];
}
#endif
#if 0
BYTE *ColorBoostText(void)
{
    return strColorBoostM26[UserPrefLanguage];
}
#endif
BYTE *FullText(void)
{
    return strFullM26[UserPrefLanguage];
}
BYTE *NatureSkinText(void)
{
    return strNatureSkinM26[UserPrefLanguage];
}
BYTE *GreenFieldText(void)
{
    return strGreenFieldM26[UserPrefLanguage];
}
BYTE *SkyBlueText(void)
{
    return strSkyBlueM26[UserPrefLanguage];
}
BYTE *AutoDetectText(void)
{
    return strAutoDetectM26[UserPrefLanguage];
}
BYTE *DemoText(void)
{
    return strDemoM26[UserPrefLanguage];
}
BYTE *FreeColorFullModeText(void)
{
    if (FullEnhanceFlag)
    {
        return strOnM26[UserPrefLanguage];
    }
    else
    {
        return strOffM26[UserPrefLanguage];
    }
}
BYTE *FreeColorSkinProtectModeText(void)
{
    if (SkinFlag)
    {
        return strOnM26[UserPrefLanguage];
    }
    else
    {
        return strOffM26[UserPrefLanguage];
    }
}
BYTE *FreeColorGreenEnhanceModeText(void)
{
    if (GreenFlag)
    {
        return strOnM26[UserPrefLanguage];
    }
    else
    {
        return strOffM26[UserPrefLanguage];
    }
}
BYTE *FreeColorBlueEnhanceModeText(void)
{
    if (BlueFlag)
    {
        return strOnM26[UserPrefLanguage];
    }
    else
    {
        return strOffM26[UserPrefLanguage];
    }
}
BYTE *FreeColorAutoDetectModeText(void)
{
    if (ColorAutoDetectFlag)
    {
        return strOnM26[UserPrefLanguage];
    }
    else
    {
        return strOffM26[UserPrefLanguage];
    }
}
BYTE *FreeColorDemoModeText(void)
{
    if (DemoFlag)
    {
        return strOnM26[UserPrefLanguage];
    }
    else
    {
        return strOffM26[UserPrefLanguage];
    }
}
#if 0
BYTE *PictureBoostText(void) // picture boost
{
    return strPictureBoostM33[UserPrefLanguage];
}
#endif
BYTE *BF_FrameSizeText(void) // frame size
{
    return strBF_FrameSizeM33[UserPrefLanguage];
}

BYTE *BF_BrightnessText(void) // brightness
{
    return strBF_BrightnessM33[UserPrefLanguage];
}

BYTE *BF_ContrastText(void) // contrast
{
    return strBF_ContrastM33[UserPrefLanguage];
}

BYTE *BF_SwitchText(void) // switch(brith frame)
{
    return strBF_SwitchM33[UserPrefLanguage];
}
#if 0
BYTE *BF_RightArrowText(void)
{
    return 0; // strRightArrowM33; // [UserPrefLanguage];
}
#endif
BYTE *BF_SwitchValueText(void)
{
    if (PictureBoostFlag)
    {
        return strOnM33[UserPrefLanguage];
    }
    else
    {
        return strOffM33[UserPrefLanguage];
    }
}
    BYTE *AutoSwitchValueText(void)
    {
        if (DoAutoConfigFlag)
        {
            return strYesM49[UserPrefLanguage];
        }
        else
        {
            return strNoM49[UserPrefLanguage];
        }
    }
    BYTE *ResetValueText(void)
    {
	#if LANGUAGE_TYPE==All14Language_AddJap
        if (DoResetFlag)
        {
            return strYesM51[UserPrefLanguage];
        }
        else
        {
            return strNoM51[UserPrefLanguage];
        }
	#else
		if (DoResetFlag)
		{
			return strYesM49[UserPrefLanguage];
		}
		else
		{
			return strNoM49[UserPrefLanguage];
		}
	#endif
    }
	BYTE *BFHPositionText(void)
	{
	    return strBF_HPositionM33[UserPrefLanguage];
	}
	BYTE *BFVPositionText(void)
	{
	    return strBF_VPositionM33[UserPrefLanguage];
	}


BYTE *OsdHPositionText(void)
{
    return strOsdHPositionM44[UserPrefLanguage];
}

BYTE *OsdVPositionText(void)
{
    return strOsdVPositionM44[UserPrefLanguage];
}

BYTE *OsdTransparenceText(void)
{
    return strTransparenceM44[UserPrefLanguage];
}

BYTE *OsdTimeOutText(void)
{
    return strOsdTimeOutM44[UserPrefLanguage];
}

BYTE *LanguageText(void)
{
    return strLanguageM44[UserPrefLanguage];
}
#ifdef EnableLightBar
BYTE *LightBarText(void)
{
    return strLEDBirghtnessM44[UserPrefLanguage];
}
#endif
BYTE *LanguageValueText(void)
{
#if LANGUAGE_TYPE== ASIA


    if (UserPrefLanguage == LANG_English)
        return strEnglishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Spanish)
        return strEspanolM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_France)
        return strFrancaisM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Portugues)
        return strPortuguesM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Russia)
        return strRissuaM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_SChina)
        return strSChinaM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_TChina)
        return strTChinaM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Korea)
        return strKoreaM44[UserPrefLanguage];
    else
        return strSChinaM44[UserPrefLanguage];
	
#elif LANGUAGE_TYPE==EUROPE

    if (UserPrefLanguage == LANG_English)
        return strEnglishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Spanish)
        return strEspanolM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_France)
        return strFrancaisM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_German)
        return strDeutschM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Italian)
        return strItalianoM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Russia)
        return strRissuaM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Swedish)
        return strSwedishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Polish)
        return strPolishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Czech)
        return strCzechM44[UserPrefLanguage];    
    else
        return strEnglishM44[UserPrefLanguage];
	
#elif LANGUAGE_TYPE==All13Language
	
	if (UserPrefLanguage == LANG_English)
		return strEnglishM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Spanish)
		return strEspanolM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_France)
		return strFrancaisM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Portugues)
		return strPortuguesM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Russia)
		return strRissuaM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_German)
        return strDeutschM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Italian)
       return strItalianoM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Swedish)
       return strSwedishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Polish)
       return strPolishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Czech)
       return strCzechM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_SChina)
		return strSChinaM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_TChina)
		return strTChinaM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Korea)
		return strKoreaM44[UserPrefLanguage];
	else
        	return strSChinaM44[UserPrefLanguage];
		
	#elif LANGUAGE_TYPE==All14Language_AddJap
	
	if (UserPrefLanguage == LANG_English)
		return strEnglishM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Spanish)
		return strEspanolM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_France)
		return strFrancaisM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Portugues)
		return strPortuguesM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Russia)
		return strRissuaM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_German)
        return strDeutschM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Italian)
       return strItalianoM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Swedish)
       return strSwedishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Polish)
       return strPolishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Czech)
       return strCzechM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_SChina)
		return strSChinaM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_TChina)
		return strTChinaM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Korea)
		return strKoreaM44[UserPrefLanguage];
	else if (UserPrefLanguage == LANG_Japan)
		return strJapanM44[UserPrefLanguage];
       else
        	return strSChinaM44[UserPrefLanguage];
#else
    if (UserPrefLanguage == LANG_English)
        return strEnglishM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Spanish)
        return strEspanolM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_France)
        return strFrancaisM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_German)
        return strDeutschM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Italian)
        return strItalianoM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Portugues)
        return strPortuguesM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_Russia)
        return strRissuaM44[UserPrefLanguage];
    else if (UserPrefLanguage == LANG_SChina)
        return strSChinaM44[UserPrefLanguage];
    else
        return strEnglishM44[UserPrefLanguage];
#endif

}

#if 1//Dual
#if (!Youweipaike)
BYTE *InputSelectText(void)
{
    return strInputSelectM49[UserPrefLanguage];
}
#endif
#endif

BYTE *AutoConfigText(void)
{
    return strAutoConfigM49[UserPrefLanguage];
}

BYTE *ResetText(void)
{
	#if LANGUAGE_TYPE==All14Language_AddJap
    return strResetM51[UserPrefLanguage];
	#else
    return strResetM49[UserPrefLanguage];
	#endif
}

BYTE *InformationText(void)
{
    return strInformationM49[UserPrefLanguage];
}

// 071218 reserved for extra page fw version
#if 0
BYTE *VersionText(void)
{
    return strVersionM49[UserPrefLanguage];
}


BYTE *FW_VerText(void)
{
   return strFW_VerM49[UserPrefLanguage];
}
#endif

#if DDCCI_ENABLE
    BYTE *DDCCIText(void)
    {
        return strDDCCIM49[UserPrefLanguage];
    }
    BYTE *DDCCIValueText(void)
    {
        if (DDCciFlag)
        {
            return strYesM49[UserPrefLanguage];
        }
        else
        {
            return strNoM49[UserPrefLanguage];
        }
    }
#endif
#ifdef OffPower         //090331 xiandi.yu Auto Off Power
    BYTE *OffTimerText(void)
    {
       return strOffTimerM49[UserPrefLanguage];
    }
	BYTE *MonitorOffText_1(void)
	{
	    return strMonitorOff1M59[UserPrefLanguage];
	}

	BYTE *MonitorOffText_2(void)
	{
	    return strMonitorOff2M59[UserPrefLanguage];
	}
#endif

#if Enable_Expansion
    BYTE *ExpansionText(void)
    {
        return strExpansionModeM49[UserPrefLanguage];
    }
    BYTE *ExpansionValueText(void)
    {
        if (UserprefExpansionMode == Expansion_Aspect
		#if WidePanel
		&&ExpansionFlag
		#endif
		)
        {
            return strASPECTM49[UserPrefLanguage];
        }
        else
        {
            return strFULLM49[UserPrefLanguage];
        }
    }
#endif
#if(!Youweipaike)
BYTE *InputSourceText(void)
{
    if (UserPrefInputSelectType == InputSelectType_Analog)
    {
        return strAnalogM49[UserPrefLanguage];
    }

#if 1 // 071218 source select
    #if (INPUT_TYPE == INPUT_1A1D)
        else if(UserPrefInputSelectType == InputSelectType_Digital)
            return strDigitalM49[UserPrefLanguage];
        else if(UserPrefInputSelectType == InputSelectType_Auto)
            return strAutoM49[UserPrefLanguage];
    #elif (INPUT_TYPE == INPUT_1A2D)
        else if(UserPrefInputSelectType == InputSelectType_Digital)
            return strDigitalM49[UserPrefLanguage];
        else if(UserPrefInputSelectType == InputSelectType_HDMI)
            return strHDMIM49[UserPrefLanguage];
        else
            return strAutoM49[UserPrefLanguage];
    #elif (INPUT_TYPE == INPUT_1A1H)
        else if(UserPrefInputSelectType == InputSelectType_HDMI)
            return strHDMIM49[UserPrefLanguage];
        else
            return strAutoM49[UserPrefLanguage];
    #endif
#if (INPUT_TYPE == INPUT_1A)//
	else
	     return strAnalogM49[UserPrefLanguage];
#endif
#else
#if ENABLE_HDMI&&(MainBoardType==MainBoard_2810_B || MainBoardType==MainBoard_715G2808)//yuanfa
	else return strHDMIM49[UserPrefLanguage];
#else
    else
    {
        return strDigitalM49[UserPrefLanguage];
    }
#endif
#endif
}
#endif
#if HotInputSelect
#if (INPUT_TYPE == INPUT_1A1D || INPUT_TYPE == INPUT_1A2D)
BYTE *InputDigitalText(void)
{
	return strDigitalM49[UserPrefLanguage];
}
#endif
BYTE *InputAnalogText(void)
{
	return strAnalogM49[UserPrefLanguage];
}
#if (INPUT_TYPE == INPUT_1A2D || INPUT_TYPE == INPUT_1A1H)
BYTE *InputHDMIText(void)
{
	return strHDMIM49[UserPrefLanguage];
}
#endif
#endif
#if 0
    BYTE *AnalogText(void)
    {
        return strAnalogM49[UserPrefLanguage];
    }
    BYTE *DigitalText(void)
    {
        return strDigitalM49[UserPrefLanguage];
    }
#endif
#if 0
BYTE *InputTypeSelectText(void)
{
    if (UserPrefInputSelectType == Input_Analog1)
    {
        return strAnalogM49[UserPrefLanguage];
    }
    else
    {
        return strDigitalM49[UserPrefLanguage];
    }
}
#endif
#if 0
BYTE *YesText(void)
{
    return strYesM49[UserPrefLanguage];
}

BYTE *NoText(void)
{
    return strNoM49[UserPrefLanguage];
}

BYTE *InformationArrowText(void)
{
    return 0;
    ; //strRightArrowM33; // [UserPrefLanguage];
}
#endif
//BYTE* InformationExtraText(void)
//{
//     return strExtraM50[UserPrefLanguage];
//}
#if 0
BYTE *InputText(void)
{
    return strInputM50[UserPrefLanguage];
}
#endif
#if DisplayInputInfo
BYTE *InputConnectorText(void)
{
    if (SrcInputType == Input_Analog1)
    {
        return strDSUBM50[UserPrefLanguage];
    }
#if ENABLE_HDMI
    #if INPUT_TYPE==INPUT_1A1H
    else if(SrcInputType == Input_Digital)
    #else
    else if(SrcInputType == Input_Digital2)
    #endif
        return strHDMIM50[UserPrefLanguage];
#endif
    else
    {
       return strDVIM50[UserPrefLanguage];
    }
}
#endif
BYTE *InputText(void)
{
	#if Dual
	if(UserPrefInputSelectType == InputSelectType_Auto)
	{
		return FALSE;
	}
    else if (SrcInputType == Input_Analog1)
    {
        return strDSUBM50[UserPrefLanguage];
    }
	#if ENABLE_HDMI
    #if INPUT_TYPE==INPUT_1A1H
    else if(SrcInputType == Input_Digital)
    #else
    else if(SrcInputType == Input_Digital2)
    #endif
        return strHDMIM50[UserPrefLanguage];
	#endif
    else
    {
       return strDVIM50[UserPrefLanguage];
    }
	#else
	return FALSE;
	#endif
}

BYTE *ResolutionText(void)
{
return strResolutionM49[UserPrefLanguage];

}
BYTE *HResText(void)
{
return strHResM49[UserPrefLanguage];

}
BYTE *VResText(void)
{
return strVResM49[UserPrefLanguage];

}
BYTE *X_Text(void)
{
return strXM49[UserPrefLanguage];
}
BYTE *HFrequenceText(void)
{
return strHfreqM49[UserPrefLanguage];
}
BYTE *VFrequenceText(void)
{
return strVreqM49[UserPrefLanguage];

}
BYTE *KHzText(void)
{
    return strKHZM49[UserPrefLanguage];
}
BYTE *HzText(void)
{
return strHzM49[UserPrefLanguage];
}
BYTE *AutoColorPassText(void)
{
    return strAutoColorPassM57[UserPrefLanguage];
}
BYTE *AutoColorFailText(void)
{
    return strAutoColorFailM57[UserPrefLanguage];
}
BYTE *AutoColorMenuText(void)
{
    return strAutoColorM57[UserPrefLanguage];
}
#if 0
    BYTE *FactoryIcon(void)
    {
        return strFactoryIcon;
    }
    BYTE *OsdLockText(void)
    {
        return strOsdLockText[UserPrefLanguage];
    }
#endif
//======================================================================
//======================================================================
BYTE *PassText(void)
{
    return strPass;
}

BYTE *FailText(void)
{
    return strFail;
}
#if 1
    //for Factory Alignment
    BYTE *VersionDateText(void)
    {
        return strVersionDate;
    }
    BYTE *ChangeDateText(void)
    {
        return strChangeDate;
    }
    BYTE *ModelNameText(void)
    {
        return strModelName;
    }

    BYTE *FactoryBrightnessText(void)
    {
        return strFactoryBrightness;
    }
    BYTE *FactoryContrastText(void)
    {
        return strFactoryContrast;
    }
#if 1//EnablePanelServiceMenu
    BYTE *PanelText(void)
    {
        return strPanel;
    }
#endif

BYTE *IcModelText(void)
{
    return strIcModelText;
}
#if BlacklitTimeOnShow
    BYTE *PanelOnText(void)
    {
        return strPanelOn;
    }
#endif
    //BYTE* DateText(void)
    //{ return strDate;
    //}
    BYTE *AutoColorText(void)
    {
        return strAutoColor;
    }
    BYTE *AdcGainText(void)
    {
        return strAdcGain;
    }
    BYTE *FactoryRedText(void)
    {
        return strFactoryRed;
    }
    BYTE *FactoryGreenText(void)
    {
        return strFactoryGreen;
    }
    BYTE *FactoryBlueText(void)
    {
        return strFactoryBlue;
    }
    BYTE *AdcOffsetText(void)
    {
        return strAdcOffset;
    }
    BYTE *COOL1Text(void)
    {
        return strFCool1;
    }
    //BYTE* COOL2Text(void)
    //{ return strFCool2;
    //}
    BYTE *WARM1Text(void)
    {
        return strFWarm1;
    }

    BYTE *sRGBText(void)
    {
        return strFsRGB;
    }
    BYTE *NORMALText(void)
    {
        return strFNORMAL;
    }
#endif
#if 1
    BYTE *BurninText(void)
    {
        return strBurnin;
    }
#endif
#if Enable_ProductModeAdjust
    BYTE *ProductModeText(void)
    {
        return strDFM;
    }
    BYTE *ProductModeOnOffText(void)
    {
        return (ProductModeFlag) ? (strOnOff[1]): (strOnOff[0]);
    }
#endif

#if 0
    BYTE *OnText(void)
    {
        return strOn;
    }
    BYTE *OffText(void)
    {
        return strOff;
    }
#endif
#if 1
    BYTE *BurninOnOffText(void)
    {
        return (BurninModeFlag) ? (strOnOff[1]): (strOnOff[0]);
    }
/*
    BYTE *BankText(void)
    {
        return strBank;
    }
    BYTE *BankNameText(void)
    {
        return strBankName[MSBankNo];
    }
    BYTE *AddressText(void)
    {
        return strAddress;
    }
    BYTE *ValueText(void)
    {
        return strValue;
    }
	//*/
#endif
#if 0//EnablePanelServiceMenu
    BYTE *PanelSelectText(void)
    {
        return strPanelSelect;
    }
    BYTE *PanelTpyeText(void)
    {
        return strPanel[PanelSelect];
    }

#endif

#if MS_OVD
BYTE *OverDriveText(void)
{
    return strOverDrive;
}

    BYTE *OverDriveOnOffText(void)
    {
        return (OverDriveOnFlag) ? (strOnOff[1]): (strOnOff[0]);
    }
#endif


#if  0// EnablePanelServiceMenu
    BYTE *AdjustPanelParaText(void)
    {
        return strAdjustPanelPara;
    }

    BYTE *PanelDitherText(void)
    {
        return strPanelDither;
    }
	
    BYTE *ColorDepthText(void)
    {
    	if(PanelSettingFlag&bPanelColorDepth)
		return str8BitColor;
	else
		return str6BitColor;
    }

    BYTE *PanelOnTiming1Text(void)
    {
        return strPanelOnTiming1;
    }

    BYTE *PanelOnTiming2Text(void)
    {
        return strPanelOnTiming2;
    }

    BYTE *PanelOffTiming1Text(void)
    {
        return strPanelOffTiming1;
    }

    BYTE *PanelOffTiming2Text(void)
    {
        return strPanelOffTiming2;
    }
	

    BYTE *PanelHSyncWidthText(void)
    {
        return strPanelHSyncWidth;
    }

    BYTE *PanelHSyncBackPorchText(void)
    {
        return strPanelHSyncBackPorch;
    }

    BYTE *PanelVSyncWidthText(void)
    {
        return strPanelVSyncWidth;
    }

    BYTE *PanelVSyncBackPorchText(void)
    {
        return strPanelVSyncBackPorch;
    }

    BYTE *PanelWidthText(void)
    {
        return strPanelWidth;
    }

    BYTE *PanelHeightText(void)
    {
        return strPanelHeight;
    }

    BYTE *PanelHTotalText(void)
    {
        return strPanelHTotal;
    }

    BYTE *PanelVTotalText(void)
    {
        return strPanelVTotal;
    }

    BYTE *PanelMaxHTotalText(void)
    {
        return strPanelMaxHTotal;
    }

    BYTE *PanelMinHTotalText(void)
    {
        return strPanelMinHTotal;
    }

   /* BYTE *PanelMaxVTotalText(void)
    {
        return strPanelMaxVTotal;
    }

    BYTE *PanelMinVTotalText(void)
    {
        return strPanelMinVTotal;
    }*/

    BYTE *PanelDCLKText(void)
    {
        return strPanelDCLK;
    }

    BYTE *PanelMaxDCLKText(void)
    {
        return strPanelMaxDCLK;
    }

    BYTE *PanelMinDCLKText(void)
    {
        return strPanelMinDCLK;
    }
    BYTE *SwingText(void)
    {
        return strSwingText;
    }
#if 0
    BYTE *Scaler0x42Text(void)
    {
        return strScaler0x42;
    }
    BYTE *Scaler0x43Text(void)
    {
        return strScaler0x43;
    }
    BYTE *Scaler0xF2Text(void)
    {
        return strScaler0xF2;
    }
    BYTE *Scaler0xF5Text(void)
    {
        return strScaler0xF5;
    }
#endif

    BYTE *ABPortSwapText(void)
    {
        return strABPortSwap;
    }
    BYTE *DualPixelOutputText(void)
    {
        return strDualPixelOutput;
    }
    BYTE *ABPortSwapOnOffText(void)
    {
        return (PanelOutputControl2&BIT1) ? (strOnOff[1]): (strOnOff[0]);
    }

    BYTE *DualPixelOutputOnOffText(void)
    {
        return (PanelOutputControl2&BIT0) ? (strOnOff[1]): (strOnOff[0]);
    }



    BYTE *PNSwapText(void)
    {
        return strPNSwap;
    }

    BYTE *ChannelSwapText(void)
    {
        return strChannelSwap;
    }
    BYTE *MSBLSBExchangeText(void)
    {
        return strMSBLSBExchange;
    }
	
    BYTE *TIModeText(void)
    {
        return strTIMode;
    }

    BYTE *OddMSBLSBExchangeText(void)
    {
        return strOddMSBLSBExchange;
    }

    BYTE *EvenMSBLSBExchangeText(void)
    {
        return strEvenMSBLSBExchange;
    }

    BYTE *OddRBExchangeText(void)
    {
        return strOddRBExchange;
    }

    BYTE *EvenRBExchangeText(void)
    {
        return strEvenRBExchange;
    }

   /* BYTE *DECurrentText(void)
    {
        return strDECurrent;
    }

    BYTE *ClockCurrentText(void)
    {
        return strClockCurrent;
    }
	
    BYTE *OddCurrentText(void)
    {
        return strOddCurrent;
    }

    BYTE *EvenCurrentText(void)
    {
        return strEvenCurrent;
    }*/

	
    BYTE *PNSwapOnOffText(void)
    {
        return (PanelOutputControl1&BIT7) ? (strOnOff[1]): (strOnOff[0]);
    }

    BYTE *ChannelSwapOnOffText(void)
    {
        return (PanelOutputControl1&BIT6) ? (strOnOff[1]): (strOnOff[0]);
    }
    BYTE *MSBLSBExchangeOnOffText(void)
    {
        return (PanelOutputControl1&BIT5) ? (strOnOff[1]): (strOnOff[0]);
    }

    BYTE *TIModeOnOffText(void)
    {
        return (PanelOutputControl1&BIT4) ? (strOnOff[1]): (strOnOff[0]);
    }
	
    BYTE *OddMSBLSBExchangeOnOffText(void)
    {
        return (PanelOutputControl1&BIT3) ? (strOnOff[1]): (strOnOff[0]);
    }

    BYTE *EvenMSBLSBExchangeOnOffText(void)
    {
        return (PanelOutputControl1&BIT2) ? (strOnOff[1]): (strOnOff[0]);
    }

    BYTE *OddRBExchangeOnOffText(void)
    {
        return (PanelOutputControl1&BIT1) ? (strOnOff[1]): (strOnOff[0]);
    }

    BYTE *EvenRBExchangeOnOffText(void)
    {
        return (PanelOutputControl1&BIT0) ? (strOnOff[1]): (strOnOff[0]);
    }
    /*BYTE *GetmAText(int i)
    {
    	if(i==0)
        return str4mA;
	else if(i==1)
        return str6mA;
	else if(i==2)
        return str8mA;
	else if(i==3)
        return str12mA;
    }
	
    BYTE *DECurrentValueText(void)
    {
        return GetmAText((PanelODRriving>>6)&0x03);
    }
    BYTE *ClockCurrentValueText(void)
    {
        return GetmAText((PanelODRriving>>4)&0x03);
    }
    BYTE *OddCurrentValueText(void)
    {
        return GetmAText((PanelODRriving>>2)&0x03);
    }
    BYTE *EvenCurrentValueText(void)
    {
        return GetmAText(PanelODRriving&0x03);
    }*/

    BYTE *BurstModeFreqText(void)
    {
        return strBurstModeFreq;
    }
    BYTE *DefDutyText(void)
    {
        return strDefDuty;
    }
    BYTE *MinDutyText(void)
    {
        return strMinDuty;
    }
    BYTE *MaxDutyText(void)
    {
        return strMaxDuty;
    }

    BYTE *ChangeModePanelVddOnOffText(void)
    {
        return (PanelSettingFlag&bChangeModePanelVddOff) ? (strOnOff[0]): (strOnOff[1]);
    }
	
    BYTE *ChangeModePanelVddText(void)
    {
        return strChangeModePanelVdd;
    }
    BYTE *SSCSTEPLText(void)
    {
        return strSSCSTEPL;
    }
    BYTE *SSCSTEPHText(void)
    {
        return strSSCSTEPH;
    }
    BYTE *SSCSPANLText(void)
    {
        return strSSCSPANL;
    }
    BYTE *SSCSPANHText(void)
    {
        return strSSCSPANH;
    }

    BYTE *InitialSettingText(void)
    {
        return strInitialSetting;
    }

    BYTE *SaveSettingText(void)
    {
        return strSaveSetting;
    }
	
#endif

#if EnableVcomAdjustItem||PANEL_VCOM_ADJUST		//121031 Modify
BYTE *VCOMText(void)
{
    return strVCOM;
}
#endif
