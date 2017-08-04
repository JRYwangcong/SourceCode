//#include "MenuFontTbl.h"
#include "MenuTblDef.h"
#include "propfont.h"
#define MenuItemXPos	1
#define MenuItemYPos	4
#define MenuItemYPos1    5

#define X_SIZE	29//31
#if AOC_MENU_FRAME_MODE
#define Y_SIZE	19
#else
#define Y_SIZE	21//22//21
#endif
#define HOTKEY_X_SIZE	27//27
#if AOC_MENU_FRAME_MODE
#define HOTKEY_Y_SIZE	10
#else
#define HOTKEY_Y_SIZE	11
#endif
#define GUAGE_X_POS       16
#define NUM_X_POS           24
#define OPBAR_X_POS        16
#if 1
//extern Bool DrawTimingInfo(Void);
//--------------------LuminanceIconText--------------------------
RadioTextType code DrawLuminanceMenuItemText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,   0, 2,   BrightnessText},
  {dwiRadioGroup,	MenuItemXPos, 5,  	ContrastText},
  {dwiRadioGroup,	MenuItemXPos, 7,  	BrightnessText},
  {dwiRadioGroup,	MenuItemXPos, 9,  	ECOModeText},
  {dwiRadioGroup|dwiOptionBar,  16, 9,  	ECOModeValue},
  {dwiRadioGroup|dwiOptionBar,  16, 11,  	GamaModeValue},
  {dwiRadioGroup,       MenuItemXPos,11,  	GAMAText},
  {dwiRadioGroup|dwiOptionBar,  16, 13,   DcrValueText},
  {dwiRadioGroup|dwiEnd,          MenuItemXPos,13,    DCRText},
};
DrawRadioGroupType code DrawLuminanceMenuItemGroupText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawLuminanceMenuItemText}
};
RadioTextType code DrawLuminanceContrastText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,   0, 2,   BrightnessText},
  {dwiRadioGroup,	MenuItemXPos, 5,  	ContrastText},
  {dwiRadioGroup|dwiEnd,	MenuItemXPos, 7,  	BrightnessText},
};
DrawRadioGroupType code DrawLuminanceContrastGroupText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawLuminanceContrastText}
};
RadioTextType code DrawGamaDcrText[]=      //YF20071218
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,   0, 2,   BrightnessText},
  {dwiRadioGroup|dwiOptionBar,  16, 11,  	GamaModeValue},
  {dwiRadioGroup,       MenuItemXPos,11,  	GAMAText},
  {dwiRadioGroup|dwiOptionBar,  16, 13,   DcrValueText},
  {dwiRadioGroup|dwiEnd,          MenuItemXPos,13,    DCRText},
};
DrawRadioGroupType code DrawGamaDcrGroupText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawGamaDcrText}
};

#endif

#if Enable_Lightsensor
RadioTextType code DrawiCareAdjustText[]=      //YF20071218
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup,	MenuItemXPos, 7,  	BrightnessText},
  {dwiRadioGroup,	MenuItemXPos, 9,  	ECOModeText},
  {dwiRadioGroup|dwiOptionBar,  16, 9,  	ECOModeValue},
  {dwiRadioGroup|dwiOptionBar,  16, 13,   DcrValueText},
  {dwiRadioGroup|dwiEnd,          MenuItemXPos,13,    DCRText},
};
DrawRadioGroupType code DrawiCareAdjustGroupText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawiCareAdjustText}
};

RadioTextType code DrawiCareAdjustText_Blue[]=      //YF20071218
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup,	MenuItemXPos, 5,  	ContrastText},
  {dwiRadioGroup|dwiOptionBar,  16, 11,  	GamaModeValue},
  {dwiRadioGroup|dwiEnd,       MenuItemXPos,11,  	GAMAText},
};
DrawRadioGroupType code DrawiCareAdjustGroupText_Blue[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawiCareAdjustText_Blue}
};

#endif
//--------------------LuminanceIconNumber--------------------------
NumberType code LuminanceNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {/*dwiJustShowValue|*/dwiNumber,       	NUM_X_POS,  MenuItemYPos1,	GetContrastValue},
  {/*dwiJustShowValue|*/dwiNumber|dwiEnd,	NUM_X_POS,  MenuItemYPos1+2,	GetBrightnessValue}
};
DrawNumberType code DrawLuminanceIconNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,6,	 LuminanceNumber
};
//--------------------LuminanceIconGuge--------------------------
GaugeType code LuminanceGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {/*dwiJustShowValue|*/dwiGauge,                    GUAGE_X_POS,	MenuItemYPos1,	GetContrastValue},
  {/*dwiJustShowValue|*/dwiGauge|dwiEnd,		 GUAGE_X_POS,	MenuItemYPos1+2,	GetBrightnessValue},
};
DrawGuageType code DrawLuminanceIconGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
// {dwiGauge,	0,	6,	10, 6,	4,	GetGreenColorValue},
  	0,	6,	9, 	LuminanceGuage
};
#if 0
//=====================================================
//--------------------ImageSetUpText--------------------------
RadioTextType code DrawImageSetupIconText[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup,   0, 2,   ImageSetupText},
  {dwiRadioGroup,	MenuItemXPos, 3,  	ClockText},
  {dwiRadioGroup,	MenuItemXPos, 5,  	FocusText},
  {dwiRadioGroup,	MenuItemXPos, 7,  	HPositionText},
  {dwiRadioGroup|dwiEnd,	MenuItemXPos, 9,  	VPositionText},

};
DrawRadioGroupType code DrawImageSetupIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawImageSetupIconText}
};
#endif

#if 0
//--------------------ImageSetUpIconNumber---------------------
NumberType code ImageSetupNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiNumber,       	26,  3,	GetClockValue},
  {dwiNumber         ,	26,  5,	GetFocusValue},
  {dwiNumber         ,	26,  7,	GetHPositionValue},
  {dwiNumber|dwiEnd,	26,  9,	GetVPositionValue},

};
DrawNumberType code DrawImageSetupIconNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,6,	 ImageSetupNumber
};

//--------------------LuminanceIconGuge--------------------------
GaugeType code ImageSetupGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiGauge,                    18,	3,	GetClockValue},
  {dwiGauge,	   	            18,	5,	GetFocusValue},
  {dwiGauge,                    18,	7,	GetHPositionValue},
  {dwiGauge|dwiEnd,	      18,	9,	GetVPositionValue},
};

DrawGuageType code DrawImageSetupIconGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
// {dwiGauge,	0,	6,	10, 6,	4,	GetGreenColorValue},
  	0,	6,	9, 	 ImageSetupGuage
};
#endif
//---------------------------------------------------------------
//=====================================================
//--------------------ColorTempText--------------------------
#if 1
RadioTextType code DrawColorTempIconText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,   0, 1,   ColorTempText},
 // {dwiRadioGroup,	MenuItemXPos, 3,  	ColorTempText},
//  {dwiOptionBar|dwiRadioGroup,	19, 3,  	ColorTempValueText },
  {dwiRadioGroup,	MenuItemXPos, 6,  	UserColorRText }, // 6 071218
  {dwiRadioGroup,	MenuItemXPos, 8,  	UserColorGText }, // 8 071218
#if Enable_YMC
  {dwiRadioGroup ,	MenuItemXPos, 10,  	UserColorBText },
  {dwiRadioGroup,	MenuItemXPos, 12,  	UserColorYText },
  {dwiRadioGroup,	MenuItemXPos, 14,  	UserColorCText },
  {dwiRadioGroup|dwiEnd,	MenuItemXPos,16,  	UserColorMText },
#else
  {dwiRadioGroup|dwiEnd ,	MenuItemXPos, 10,  	UserColorBText }, // 10 071218
#endif
};
DrawRadioGroupType code DrawColorTempIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawColorTempIconText}
};
#endif
//--------------------ColorTempIconNumber---------------------
NumberType code ColorTempNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiNumber,       	NUM_X_POS,  MenuItemYPos+2,	GetRedColorValue},
  {dwiNumber,			NUM_X_POS,  MenuItemYPos+4,	GetGreenColorValue},
#if Enable_YMC
  {dwiNumber, 			NUM_X_POS,  MenuItemYPos+6,    GetBlueColorValue},
  {dwiNumber,       	NUM_X_POS,  MenuItemYPos+8,	GetYColorValue},
  {dwiNumber,   		NUM_X_POS,  MenuItemYPos+10,	GetCColorValue},
  {dwiNumber|dwiEnd,	NUM_X_POS,  MenuItemYPos+12,	GetMColorValue},
#else
  {dwiNumber|dwiEnd,           NUM_X_POS,  MenuItemYPos+6,    GetBlueColorValue}, // 9 071218
#endif
};
DrawNumberType code DrawColorTempIconNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,6,	 ColorTempNumber
};
//--------------------ColorTempIconGuge--------------------------
GaugeType code ColorTempGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiGauge,                    GUAGE_X_POS,	MenuItemYPos+2,	GetRedColorValue},
  {dwiGauge,	   	            GUAGE_X_POS,	MenuItemYPos+4,	GetGreenColorValue},
  //{dwiGauge,            		GUAGE_X_POS,	MenuItemYPos+6,	GetBlueColorValue},
#if Enable_YMC
  {dwiGauge,	   	            GUAGE_X_POS,	MenuItemYPos+8,	GetYColorValue},
  {dwiGauge,             	    GUAGE_X_POS,	MenuItemYPos+10,	GetCColorValue},
  {dwiGauge|dwiEnd,	            GUAGE_X_POS,	MenuItemYPos+12,	GetMColorValue},
#else
  {dwiGauge|dwiEnd,                    GUAGE_X_POS, MenuItemYPos+6,  GetBlueColorValue}, // 9 071218
#endif
};

DrawGuageType code DrawColorTempIconGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
// {dwiGauge,	0,	6,	10, 6,	4,	GetGreenColorValue},
  	0,	6,	9, 	 ColorTempGuage
};

//=====================================================
#if 0
//--------------------ColorBoostText--------------------------
RadioTextType code DrawColorBoostIconText[]=
{ // Flags,            XPos,	YPos,	DisplayText
 // {dwiRadioGroup,	4, 4,  	FullText},
  {dwiRadioGroup,	MenuItemXPos, 3,  	FullText},
  {dwiRadioGroup|dwiOptionBar,	21, 3,  	FreeColorFullModeText },

  {dwiRadioGroup,	MenuItemXPos, 5,  	NatureSkinText},
  {dwiRadioGroup|dwiOptionBar,	21, 5,  	FreeColorSkinProtectModeText},

  {dwiRadioGroup,	MenuItemXPos, 7,  	GreenFieldText},
  {dwiRadioGroup|dwiOptionBar,	21, 7,  	FreeColorGreenEnhanceModeText},

  {dwiRadioGroup,	MenuItemXPos, 9,  	SkyBlueText},
  {dwiRadioGroup|dwiOptionBar,	21, 9,  	FreeColorBlueEnhanceModeText},

  {dwiRadioGroup,	MenuItemXPos, 11,  	AutoDetectText},
  {dwiRadioGroup|dwiOptionBar,	21, 11,  	FreeColorAutoDetectModeText},

  {dwiRadioGroup,	MenuItemXPos, 13,  	DemoText},
  {dwiRadioGroup|dwiOptionBar|dwiEnd,	21, 13,  	FreeColorDemoModeText},

};
DrawRadioGroupType code DrawColorBoostIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawColorBoostIconText}
};
#endif
//================= BrightFrameMenuItem ===============
//------------------  BrightnessFrameText ---------------------
RadioTextType code DrawBrightFrame[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup,   MenuItemXPos, MenuItemYPos1,   BF_FrameSizeText},
  {dwiRadioGroup,	MenuItemXPos, MenuItemYPos1+2,  	BF_BrightnessText},
  {dwiRadioGroup,	MenuItemXPos, MenuItemYPos1+4,  	BF_ContrastText},
  {dwiRadioGroup,   MenuItemXPos, MenuItemYPos1+6,    BFHPositionText},
  {dwiRadioGroup,   MenuItemXPos, MenuItemYPos1+8,   BFVPositionText},
  {dwiRadioGroup|dwiEnd,   MenuItemXPos, MenuItemYPos1+10,   BF_SwitchText},
  //{dwiOptionBar|dwiRadioGroup|dwiEnd,   19, MenuItemYPos1+10,  BF_SwitchValueText},

};
DrawRadioGroupType code DrawBrightFrameRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawBrightFrame}
};






//--------------------BrightFrameIconNumber--------------------------
NumberType code BrightFrameNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiNumber,       	        NUM_X_POS,  MenuItemYPos1,	GetBFSizeValue},
  {dwiNumber,               NUM_X_POS,  MenuItemYPos1+2, GetSubBrightness100Value},
  {dwiNumber,           	NUM_X_POS,  MenuItemYPos1+4,	GetSubContrast100Value},
  {dwiNumber,               NUM_X_POS,  MenuItemYPos1+6, GetBFHstartValue},
  {dwiNumber|dwiEnd,    NUM_X_POS,  MenuItemYPos1+8,GetBFVstartValue},
};
DrawNumberType code DrawBrightFrameIconNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,6,	 BrightFrameNumber
};
//--------------------LuminanceIconGuge--------------------------
GaugeType code BrightFrameGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiGauge,                    GUAGE_X_POS,	MenuItemYPos1,	GetBFSizeValue},
  {dwiGauge,                    GUAGE_X_POS,	MenuItemYPos1+2,	GetSubBrightness100Value},
  {dwiGauge,                    GUAGE_X_POS,	MenuItemYPos1+4,	GetSubContrast100Value},
    {dwiGauge,                    GUAGE_X_POS, MenuItemYPos1+6,  GetBFHstartValue},
    {dwiGauge|dwiEnd,             GUAGE_X_POS, MenuItemYPos1+8, GetBFVstartValue},

};
DrawGuageType code DrawBrightFrameIconGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
// {dwiGauge,	0,	6,	10, 6,	4,	GetGreenColorValue},
  	0,	6,	9, 	BrightFrameGuage
};

//------------------  OsdSetUpText ---------------------
RadioTextType code DrawOsdSetUp[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiDirectStr/*dwiRadioGroup*/,   10, 1,   MainMonoText},
  {dwiRadioGroup,	MenuItemXPos, OSD_HPosition,  	OsdHPositionText},
  {dwiRadioGroup,	MenuItemXPos, OSD_VPosition,  	OsdVPositionText},
  {dwiRadioGroup,	MenuItemXPos, OSD_Timeout,  	OsdTimeOutText},
  {dwiRadioGroup,   MenuItemXPos, OSD_Trans,        OsdTransparenceText},
  {dwiRadioGroup,	MenuItemXPos, OSD_Language,  	LanguageText},
  #ifdef EnableLightBar
  {dwiOptionBar|dwiRadioGroup,  OPBAR_X_POS, OSD_Language,  	LanguageValueText},
  {dwiRadioGroup|dwiEnd,  MenuItemXPos, OSD_LightBar,  	LightBarText},
#else
  {dwiOptionBar|dwiRadioGroup|dwiEnd,  OPBAR_X_POS, OSD_Language,  	LanguageValueText},
#endif  

};
DrawRadioGroupType code DrawOsdSetUpIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawOsdSetUp}
};

//--------------------OsdSetUpIconNumber--------------------------
NumberType code OsdSetUpNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiNumber,       	NUM_X_POS,  OSD_HPosition,	GetOSDHPositionValue},
  {dwiNumber,       	NUM_X_POS,  OSD_VPosition,	GetOSDVPositionValue},
  {dwiNumber,           NUM_X_POS,  OSD_Trans, 	GetOSDTransparence},
  #ifdef EnableLightBar
  {dwiNumber,                    NUM_X_POS,	OSD_Timeout,	GetOSDTimeValue},
  {dwiNumber|dwiEnd,         NUM_X_POS,	OSD_LightBar,GetLightBarValue},
#else
  {dwiNumber|dwiEnd,         NUM_X_POS,	OSD_Timeout,	GetOSDTimeValue},
#endif


};
DrawNumberType code DrawOsdSetUpIconNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,6,	 OsdSetUpNumber
};
//--------------------OsdSetUpIconGuge--------------------------
GaugeType code OsdSetUpGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiGauge,                    GUAGE_X_POS,	OSD_HPosition,	GetOSDHPositionValue},
  {dwiGauge,                    GUAGE_X_POS,	OSD_VPosition,	GetOSDVPositionValue},
  {dwiGauge,                    GUAGE_X_POS, OSD_Trans,  GetOSDTransparence},
  #ifdef EnableLightBar
  {dwiGauge,                    GUAGE_X_POS,	OSD_Timeout,	GetOSDTime100Value},
  {dwiGauge|dwiEnd,         GUAGE_X_POS,	OSD_LightBar,GetLightBarValue},
#else
  {dwiGauge|dwiEnd,         GUAGE_X_POS,	OSD_Timeout,	GetOSDTime100Value},
#endif


};
DrawGuageType code DrawOsdSetUpIconGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
// {dwiGauge,	0,	6,	10, 6,	4,	GetGreenColorValue},
  	0,	6,	9, 	OsdSetUpGuage
};

/*

RadioTextType code DrawInputSelect[]=
{ // Flags,            XPos,	YPos,	DisplayText
 // {dwiOptionBar|dwiRadioGroup|dwiEnd,  OPBAR_X_POS, OSD_Language,  	LanguageValueText},
   {dwiRadioGroup,	MenuItemXPos, 6,  	InputSourceText },
};
DrawRadioGroupType code DrawInputSelectIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawInputSelect}
};
*/

//------------------  ExtraText ---------------------
#if 0
RadioTextType code DrawExtra[]=
{ // Flags,            XPos,	YPos,	DisplayText
#if !OSD_2008
  {dwiRadioGroup,   0, 2,   ExtraText},
#endif
 #if Dual
  {dwiRadioGroup,	MenuItemXPos, 3,  	InputSelectText},
  {dwiOptionBar|dwiRadioGroup,	20, 3,  	InputSourceText},
 #endif
  {dwiRadioGroup,	MenuItemXPos, 3+(2*Dual),  	AutoConfigText},
  {dwiOptionBar|dwiRadioGroup,	20, 3+(2*Dual),  	YesText},
#if DDCCI_ENABLE
  {dwiRadioGroup,	MenuItemXPos, 5+(2*Dual)+2*DDCCI_ENABLE,  	DDCCIText},
  {dwiOptionBar|dwiRadioGroup,	20, 5+(2*Dual)+2*DDCCI_ENABLE,  	DDCCIValueText},
#endif
#if Enable_Expansion
  {dwiRadioGroup,	MenuItemXPos, 5+(2*Dual),  	ExpansionText},
  {dwiOptionBar|dwiRadioGroup,	20, 5+(2*Dual),  	ExpansionValueText},
#endif
  {dwiRadioGroup,	MenuItemXPos, 5+ExtraOsd_offset+(2*Dual),  	ResetText},
  {dwiOptionBar|dwiRadioGroup,   20, 5+ExtraOsd_offset+(2*Dual),      YesText},
  {dwiOptionBar1|dwiRadioGroup,	20, 7+ExtraOsd_offset+(2*Dual),  	InformationArrowText},
  {dwiRadioGroup|dwiEnd,  MenuItemXPos, 7+ExtraOsd_offset+(2*Dual),  	InformationText},
};
DrawRadioGroupType code DrawExtraIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawExtra}
};
#if !OSD_2008
RadioTextType code DrawExtra_1[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup|dwiEnd,    0, 2,   ExtraText},
};
DrawRadioGroupType code DrawExtraIconRatioText_1[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawExtra_1}
};
#endif
#endif


//=========== MainMenuItems ================================


MenuItemType code MainMenuItems[]=
{
// 0 Luminance
   {  iconx1_xpos, icon1y_ypos, // XPos, YPos;
	0, 2, // ForeColor, BackColor;
	0, 9, // SelForeColor, SelBackColor;
	LuminanceMenu,///NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	MainNaviKeyEvent,
	{ NULL, 		// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL, // DrawLuminanceIconNumber,// DrawNumberType
	  NULL, // DrawLuminanceIconGuage,// DrawGuageType
	  NULL, // DrawLuminanceIconRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
    	NULL, //  MenuPage7, // //Font
	mibSelectable|mibHighlight // |mibClrInner // Flags
  },
// 1 ImageSetUpMenu
   {  iconx2_xpos, icon1y_ypos, // XPos, YPos;
	0, 3, // ForeColor, BackColor;
	0, 10, // SelForeColor, SelBackColor;
	ImageSetupMenu,//NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	MainNaviKeyEvent,
	{ NULL, 		// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL, // DrawImageSetupIconNumber,// DrawNumberType
	  NULL, //  DrawImageSetupIconGuage,// DrawGuageType
	  NULL, //  DrawImageSetupIconRatioText,//DrawImageSetupIconRatioText,// DrawRadioGroupType
	},
    NULL, //    MenuPage13, //Font
	mibSelectable|mibDVIDisable|mibHighlight // |mibClrInner  // Flags
  },

// 2 ColorTempIconMenu
   {  iconx3_xpos, icon1y_ypos, // XPos, YPos;
	0, 4, // ForeColor, BackColor;
	0, 11, // SelForeColor, SelBackColor;
	ColorTempMenu,//NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	MainNaviKeyEvent,
	{ NULL, 		// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL, // DrawColorTempIconNumber,// DrawNumberType
	  NULL, //  DrawColorTempIconGuage,// DrawGuageType
	  NULL, //  DrawColorTempIconRatioText,//DrawImageSetupIconRatioText,// DrawRadioGroupType
	},
    NULL, //   MenuPage18, //Font
	mibSelectable|mibHighlight// |mibClrInner  // Flags
  },


// 3  FreeColor
   {  iconx1_xpos, icon2y_ypos, // XPos, YPos;
	0,5, // ForeColor, BackColor;
	0,12, // SelForeColor, SelBackColor;
	ColorBoostMenu,//NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	MainNaviKeyEvent,
	{ NULL, 		// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL, // DrawColorBoostIconRatioText//DrawColorTempIconRatioText,//DrawRadioGroupType
	},
    NULL, //   MenuPage26, //Font
	mibSelectable|mibHighlight// |mibClrInner//|mibDCRDisable // Flags
  },
// 4  BrightFrame
   {  iconx2_xpos, icon2y_ypos, // XPos, YPos;
	0,6, // ForeColor, BackColor;
	0,13, // SelForeColor, SelBackColor;
	BrightFrameMenu,////NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	MainNaviKeyEvent,
	{ NULL, 		// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL, // DrawBrightFrameIconNumber,// DrawNumberType
	  NULL, // DrawBrightFrameIconGuage,// DrawGuageType
	  NULL, // DrawBrightFrameRatioText,//DrawBrightnessFrameIconRatioText,//DrawColorTempIconRatioText,//DrawRadioGroupType
	},
    NULL, //     MenuPage33,  //Font
	mibSelectable|mibHighlight// |mibClrInner//|mibDCRDisable// Flags
	#if PICTUREBOOST_FUN_EN
	|mibPicBoostDisable
	#endif
  },
// 5  OsdSetUp
   {  iconx3_xpos, icon2y_ypos, // XPos, YPos;
	0,7, // ForeColor, BackColor;
	0,14, // SelForeColor, SelBackColor;
	OsdSetUpMenu,///NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	MainNaviKeyEvent,
	{ NULL, 		// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL, // DrawOsdSetUpIconNumber,// DrawNumberType
	  NULL, // DrawOsdSetUpIconGuage,// DrawGuageType
	  NULL, // DrawOsdSetUpIconRatioText,//DrawColorTempIconRatioText,//DrawRadioGroupType
	},
     NULL, //  MenuPage44, //Font
	mibSelectable|mibHighlight// |mibClrInner// Flags
  },
// 6  Extra
   {  iconx1_xpos, icon3y_ypos, // XPos, YPos;
	0,8, // ForeColor, BackColor;
	0,15, // SelForeColor, SelBackColor;
	ExtraMenu,//NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	MainNaviKeyEvent,
	{ NULL, 		// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL, // DrawExtraIconRatioText,//DrawColorTempIconRatioText,//DrawRadioGroupType
	},
    NULL, //   MenuPage49, //Font
	mibSelectable|mibHighlight// |mibClrInner// Flags
  },
// 4  reset
   {  iconx2_xpos, icon3y_ypos, // XPos, YPos;
    0,6, // ForeColor, BackColor;
    0,13, // SelForeColor, SelBackColor;
    ResetMenu,////NextMenuPage;
    DWI_Icon,// DrawMenuItemType;
    MainMonoText, // DisplayText;
    MainNaviKeyEvent,
    { NULL,         // AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    { NULL, // DrawBrightFrameIconNumber,// DrawNumberType
      NULL, // DrawBrightFrameIconGuage,// DrawGuageType
      NULL, // DrawBrightFrameRatioText,//DrawBrightnessFrameIconRatioText,//DrawColorTempIconRatioText,//DrawRadioGroupType
    },
    NULL, //  Font
    mibSelectable|mibHighlight// |mibClrInner//|mibDCRDisable// Flags
  },
// 4  exit
   {  iconx3_xpos, icon3y_ypos, // XPos, YPos;
    0,6, // ForeColor, BackColor;
    0,13, // SelForeColor, SelBackColor;
    RootMenu,////NextMenuPage;
    DWI_Icon,// DrawMenuItemType;
    MainMonoText, // DisplayText;
    MainNaviKeyEvent,
    { NULL,         // AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    { NULL, // DrawBrightFrameIconNumber,// DrawNumberType
      NULL, // DrawBrightFrameIconGuage,// DrawGuageType
      NULL, // DrawBrightFrameRatioText,//DrawBrightnessFrameIconRatioText,//DrawColorTempIconRatioText,//DrawRadioGroupType
    },
    NULL, //  Font
    mibSelectable|mibHighlight// |mibClrInner//|mibDCRDisable// Flags
  },
// 11 Factory,
   {  1, 0, // XPos, YPos;
	0, 2, // ForeColor, BackColor;
	0, 13, // SelForeColor, SelBackColor;
//	0, 2, // ForeColor, BackColor;
//	0, 2, // SelForeColor, SelBackColor;
	FactoryMenu,//BrightnessMenu, //NextMenuPage;
	DWI_Text, // DrawMenuItemType;
	MainMonoText,//Main_FText, // DisplayText;
	MainNaviKeyEvent,
	{ NULL, 		// AdjustFunction
	  NULL			// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL,//DrawExitIconRatioText//DrawManagementRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibFactoryItem // Flags
  },
};


//=========== LuminanceMenuItems ================================
//--------------------ContrastNumber--------------------------
NumberType code ContrastNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1,	GetContrastValue},
};
DrawNumberType code DrawContrastNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 ContrastNumber
};
//--------------------ContrastIconGuge--------------------------
GaugeType code ContrastGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1,	GetContrastValue},
};
DrawGuageType code DrawContrastGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	ContrastGuage
};

//--------------------BrightnessNumber--------------------------
NumberType code BrightnessNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1+2,	GetBrightnessValue},
};
DrawNumberType code DrawBrightnessNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 BrightnessNumber
};
//--------------------BrightnessIconGuge--------------------------
GaugeType code BrightnessGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1+2,	GetBrightnessValue},
};
DrawGuageType code DrawBrightnessGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	10,	9, 	BrightnessGuage
};

#if AudioFunc
//--------------------VolumeNumber--------------------------
NumberType code VolumeNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS-5,  5,	GetVolumeValue},
};
DrawNumberType code DrawVolumeNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 VolumeNumber
};
//--------------------VolumeIconGuge--------------------------
GaugeType code VolumeGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS-5,	5,	GetVolumeValue},
};
DrawGuageType code DrawVolumeGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	10,	9, 	VolumeGuage
};
#else
NumberType code HotConNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS-5,  5,	GetContrastValue},
};
DrawNumberType code DrawHotConNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 HotConNumber
};
//--------------------VolumeIconGuge--------------------------
GaugeType code HotConGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS-5,	5,	GetContrastValue},
};
DrawGuageType code DrawHotConGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	10,	9, 	HotConGuage
};
#endif
#if EnableHotBrightness
NumberType code HotBriNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS-5,  5,	GetBrightnessValue},
};
DrawNumberType code DrawHotBriNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 HotBriNumber
};
//--------------------VolumeIconGuge--------------------------
GaugeType code HotBriGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS-5,	5,	GetBrightnessValue},
};
DrawGuageType code DrawHotBriGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	10,	9, 	HotBriGuage
};

#endif
//--------------------ECOMODEText--------------------------
RadioTextType code DrawECOText[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiEnd|dwiOptionBar,  OPBAR_X_POS, MenuItemYPos1+4,  	ECOModeValue},
};
DrawRadioGroupType code DrawECOIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	 12, NULL,DrawECOText}
};

//--------------------GAMAMODEText--------------------------
RadioTextType code DrawGAMAText[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiEnd|dwiOptionBar,  OPBAR_X_POS,MenuItemYPos1+6,  	GamaModeValue},
};
DrawRadioGroupType code DrawGAMAIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawGAMAText}
};

//------------- DCRText  --------------------
RadioTextType code DrawDcr[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiEnd|dwiOptionBar,	OPBAR_X_POS, MenuItemYPos1+8,  	DcrValueText },
};
DrawRadioGroupType code DrawDcrIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawDcr}
};
//---------------------------------------------------------------
#if Enable_Lightsensor
RadioTextType code DrawiCare[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiEnd|dwiOptionBar,	OPBAR_X_POS, MenuItemYPos1+10,  	iCareValueText },
};
DrawRadioGroupType code DrawiCareIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawiCare}
};
#endif

MenuItemType code LuminanceMenuItems[]=
{
// 0 Contrast
   {  MenuItemXPos, MenuItemYPos1, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // ContrastMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ContrastText, // DisplayText;
	NaviKeyEvent,
	{ AdjustContrast,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawContrastNumber,// DrawNumberType
	  DrawContrastGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibsRGBDisable| mibDCRDisable | mibStdEnable  // Flags
  },
// 1 Brightness
   {  MenuItemXPos, MenuItemYPos1+2, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // BrightnessMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BrightnessText, // DisplayText;
	NaviKeyEvent,
	{ AdjustBrightness,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBrightnessNumber,// DrawNumberType
	  DrawBrightnessGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
#if Enable_Lightsensor
	mibSelectable| mibsRGBDisable | mibDCRDisable | mibStdEnable | mibiCareDisable// Flags
#else
	mibSelectable| mibsRGBDisable | mibDCRDisable | mibStdEnable// Flags
#endif	
  },
// 2 ECOModeSelect
   {  MenuItemXPos, MenuItemYPos1+4, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // ECOMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ECOModeText, // DisplayText;
	NaviKeyEvent,
	{ AdjustECOMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawECOIconRatioText // DrawRadioGroupType
	},
      NULL, //Font
#if Enable_Lightsensor
	mibSelectable | mibDCRDisable |mibsRGBDisable | mibiCareDisable// Flags
#else
	mibSelectable | mibDCRDisable |mibsRGBDisable // Flags
#endif	
  },
// 3 GAMAModeSelect
   {  MenuItemXPos, MenuItemYPos1+6, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // GAMAMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	GAMAText, // DisplayText;
	NaviKeyEvent,
 	{ AdjustGamaMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{NULL,// DrawBrightnessNumber,// DrawNumberType
	  NULL,//DrawBrightnessGuage,// DrawGuageType
	  DrawGAMAIconRatioText // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable | mibDCRDisable|mibsRGBDisable// Flags
  },
// 4 DCR
   {  MenuItemXPos, MenuItemYPos1+8, // XPos, YPos;
    0, 6, // ForeColor, BackColor;
    0, 12, // SelForeColor, SelBackColor;
    MainMenu, // DCRMenu,///NextMenuPage;
    DWI_Text,// DrawMenuItemType;
    DCRText, // DisplayText;
    NaviKeyEvent,
    { AdjustDcrMode,// AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    {NULL,// DrawBrightnessNumber,// DrawNumberType
      NULL,//DrawBrightnessGuage,// DrawGuageType
      DrawDcrIconRatioText // DrawRadioGroupType
    },
      NULL, //Font
#if Enable_Lightsensor
    mibSelectable | mibiCareDisable// Flags
#else
    mibSelectable // Flags
#endif    
  },
#if Enable_Lightsensor
// 5 iCare
   {  MenuItemXPos, MenuItemYPos1+10, // XPos, YPos;
    0, 6, // ForeColor, BackColor;
    0, 12, // SelForeColor, SelBackColor;
    MainMenu, // DCRMenu,///NextMenuPage;
    DWI_Text,// DrawMenuItemType;
    iCareText, // DisplayText;
    NaviKeyEvent,
    { AdjustiCareMode,// AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    {NULL,// DrawBrightnessNumber,// DrawNumberType
      NULL,//DrawBrightnessGuage,// DrawGuageType
      DrawiCareIconRatioText // DrawRadioGroupType
    },
      NULL, //Font
    mibSelectable // Flags
  },
#endif
  #if 1
    {   0, 1, // XPos, YPos;
        0, 10, // ForeColor, BackColor;
        0, 12, // SelForeColor, SelBackColor;
        MainMenu,///NextMenuPage;
        DWI_Icon,// DrawMenuItemType;
        MainMonoText,//LuminanceText, // DisplayText;
        NaviKeyEvent,
        { NULL,// AdjustFunction
          NULL,//AutoConfig// ExecFunction
        },
        { NULL,// DrawNumberType
          NULL,// DrawGuageType
          NULL, //rawMenuRadioGroup,
        },
          NULL, //Font
      0// Flags
    },
   #endif
};


#if !1//OSD_2008
//============ Contrast MenuItem==============
MenuItemType code ContrastMenuItems[]=
{
// 0 Contrast
   {  MenuItemXPos, 3, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	LuminanceMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ContrastText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustContrast,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawContrastNumber,// DrawNumberType
	  DrawContrastGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ Brightness MenuItem==============
MenuItemType code BrightnessMenuItems[]=
{
// 0 Brightness
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	LuminanceMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BrightnessText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustBrightness,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBrightnessNumber,// DrawNumberType
	  DrawBrightnessGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if 1//OSD_2008
#if 0
//============ HotKeyDCR MenuItems ==============
RadioTextType code DrawDcrText[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiEnd|dwiOptionBar,  OPBAR_X_POS+1, 5,  	DcrValueText},
};
DrawRadioGroupType code DrawHotDcrIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	 12, NULL,DrawDcrText}
};
#endif
MenuItemType code HotKeyDCRMenuItems[]=
{
   {  5, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	RootMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	 NULL,//DCRText, // DisplayText;
	 DCRHotKeyEvent,//HotKeyEvent,
	{ AdjustDcrMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL,//DrawHotDcrIconRatioText,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};


#endif

#if Enable_Lightsensor
MenuItemType code HotKeyiCareMenuItems[]=
{
   {  5, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	RootMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	 NULL,//DCRText, // DisplayText;
	 DCRHotKeyEvent,//HotKeyEvent,
	{ AdjustiCareMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL,//DrawHotDcrIconRatioText,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif

#if HotExpansionAdjust
MenuItemType code HotKeyExpansionMenuItems[]=
{
   {  5, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	RootMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	 NULL,//DCRText, // DisplayText;
	 ExpansionHotKeyEvent,//HotKeyEvent,
	{ AdjustExpansionMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL,//DrawHotDcrIconRatioText,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

#endif

#if AudioFunc
//============ Volume MenuItem==============
MenuItemType code VolumeMenuItems[]=
{
// 0 Brightness
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	RootMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // 2007/10/24  by KK   VolumeText, // DisplayText;
	 HotKeyEvent,
	{ AdjustVolume,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawVolumeNumber,// DrawNumberType
	  DrawVolumeGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

#if 0
//------------- DHotKeyMuteMenu  --------------------
RadioTextType code DrawHotKeyMuteInfo[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup|dwiEnd,	0, 4,  	MuteText},

};
DrawRadioGroupType code DrawHotKeyMuteIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12, NULL,DrawHotKeyMuteInfo}
};
MenuItemType code HotKeyMuteMenuItems[]=
{//0 Reset Icon
  {  MenuItemXPos, 4, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	0,  6, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	LockKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	   NULL// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawHotKeyMuteIconRationText	// DrawRadioGroupType
	},
       NULL, //Font
	mibSelectable // Flags
  }
};

#endif

#else

MenuItemType code HotKeyConMenuItems[]=
{
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	RootMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // 2007/10/24  by KK   VolumeText, // DisplayText;
	 HotKeyEvent,
	{ AdjustContrast,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawHotConNumber,// DrawNumberType
	  DrawHotConGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if EnableHotBrightness
MenuItemType code HotKeyBriMenuItems[]=
{
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	RootMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // 2007/10/24  by KK   VolumeText, // DisplayText;
	 HotKeyEvent,
	{ AdjustBrightness,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawHotBriNumber,// DrawNumberType
	  DrawHotBriGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif

#if HotInputSelect

MenuItemType code HotInputSelectMenuItems[]=
{
#if (INPUT_TYPE == INPUT_1A1D)
   {  0, 4, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	RootMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	InputAnalogText, // 2007/10/24  by KK   VolumeText, // DisplayText;
	 HotInputExecKeyEvent,
	{ NULL,// AdjustFunction
	  SetInputToAnalog//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
 {  0, 6, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	RootMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	InputDigitalText, // 2007/10/24  by KK   VolumeText, // DisplayText;
	 HotInputExecKeyEvent,
	{ NULL,// AdjustFunction
	  SetInputToDigital//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

#elif (INPUT_TYPE == INPUT_1A2D)
  {  0, 4, // XPos, YPos;
   0, 6, // ForeColor, BackColor;
   0, 14, // SelForeColor, SelBackColor;
   RootMenu,///NextMenuPage;
   DWI_Text,// DrawMenuItemType;
   InputAnalogText, // 2007/10/24  by KK   VolumeText, // DisplayText;
	HotInputExecKeyEvent,
   { NULL,// AdjustFunction
	 SetInputToAnalog,//AutoConfig// ExecFunction
   },
   { NULL,// DrawNumberType
	 NULL,// DrawGuageType
	 NULL//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
   },
	 NULL, //Font
   mibSelectable // Flags
 },
{ 0, 5, // XPos, YPos;
   0, 6, // ForeColor, BackColor;
   0, 14, // SelForeColor, SelBackColor;
   RootMenu,///NextMenuPage;
   DWI_Text,// DrawMenuItemType;
   InputDigitalText, // 2007/10/24	by KK	VolumeText, // DisplayText;
	HotInputExecKeyEvent,
   { NULL,// AdjustFunction
	 SetInputToDigital//AutoConfig// ExecFunction
   },
   { NULL,// DrawNumberType
	 NULL,// DrawGuageType
	 NULL//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
   },
	 NULL, //Font
   mibSelectable // Flags
 },
{ 0, 6, // XPos, YPos;
   0, 6, // ForeColor, BackColor;
   0, 14, // SelForeColor, SelBackColor;
   RootMenu,///NextMenuPage;
   DWI_Text,// DrawMenuItemType;
   InputHDMIText, // 2007/10/24  by KK	 VolumeText, // DisplayText;
	HotInputExecKeyEvent,
   { NULL,// AdjustFunction
	 SetInputToHDMI//AutoConfig// ExecFunction
   },
   { NULL,// DrawNumberType
	 NULL,// DrawGuageType
	 NULL//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
   },
	 NULL, //Font
   mibSelectable // Flags
 },



#elif (INPUT_TYPE == INPUT_1A1H)

  { 0, 4, // XPos, YPos;
   0, 6, // ForeColor, BackColor;
   0, 14, // SelForeColor, SelBackColor;
   RootMenu,///NextMenuPage;
   DWI_Text,// DrawMenuItemType;
   InputAnalogText, // 2007/10/24  by KK   VolumeText, // DisplayText;
	HotInputExecKeyEvent,
   { NULL,// AdjustFunction
	 SetInputToAnalog//AutoConfig// ExecFunction
   },
   { NULL,// DrawNumberType
	 NULL,// DrawGuageType
	 NULL//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
   },
	 NULL, //Font
   mibSelectable // Flags
 },

{  0, 6, // XPos, YPos;
   0, 6, // ForeColor, BackColor;
   0, 14, // SelForeColor, SelBackColor;
   RootMenu,///NextMenuPage;
   DWI_Text,// DrawMenuItemType;
   InputHDMIText, // 2007/10/24  by KK	 VolumeText, // DisplayText;
	HotInputExecKeyEvent,
   { NULL,// AdjustFunction
	 SetInputToHDMI//AutoConfig// ExecFunction
   },
   { NULL,// DrawNumberType
	 NULL,// DrawGuageType
	 NULL//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
   },
	 NULL, //Font
   mibSelectable // Flags
 },



#endif
  
};


#endif


//============ ECOMode MenuItem==============
MenuItemType code ECOMODEMenuItems[]=
{
// 0 ECOMemu
   {  MenuItemXPos, 7, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	LuminanceMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ECOModeText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustECOMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL, // DrawNumberType
	  NULL, //  DrawGuageType
	 DrawECOIconRatioText//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ GAMAMod MenuItem==============
MenuItemType code GAMAMODEMenuItems[]=
{
// 0 GAMAMemu
   {  MenuItemXPos, 9, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	LuminanceMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	GAMAText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustGamaMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawGAMAIconRatioText//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ DCRMenu ==============
MenuItemType code DCRMenuItems[]=
{
// 0 DCRMenu
   {  MenuItemXPos, 11, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	LuminanceMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DCRText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustDcrMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawDcrIconRatioText //DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
//#endif
//=========== ImageSetUpMenuItems ================================
//--------------------ClockNumber--------------------------
NumberType code ClockNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1,	GetClockValue},
};
DrawNumberType code DrawClockNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 ClockNumber
};
//--------------------ClockIconGuge--------------------------
GaugeType code ClockGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1,	GetClockValue},
};
DrawGuageType code DrawClockGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	ClockGuage
};

//--------------------FocusNumber--------------------------
NumberType code FocusNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1+2,	GetFocusValue},
};
DrawNumberType code DrawFocusNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 FocusNumber
};
//--------------------FocusIconGuge--------------------------
GaugeType code FocusGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1+2,	GetFocusValue},
};
DrawGuageType code DrawFocusGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	FocusGuage
};

//--------------------HPositionNumber--------------------------
NumberType code HPositionNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1+4,	GetHPositionValue},
};
DrawNumberType code DrawHPositionNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 HPositionNumber
};
//--------------------HPositionIconGuge--------------------------
GaugeType code HPositionGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1+4,	GetHPositionValue},
};
DrawGuageType code DrawHPositionGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	HPositionGuage
};

//--------------------VPositionNumber--------------------------
NumberType code VPositionNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1+6,	GetVPositionValue},
};
DrawNumberType code DrawVPositionNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 VPositionNumber
};
//--------------------HPositionIconGuge--------------------------
GaugeType code VPositionGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1+6,	GetVPositionValue},
};
DrawGuageType code DrawVPositionGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	VPositionGuage
};

//-------------ClockNumber------------------------------------------
MenuItemType code ImageSetupMenuItems[]=
{
// 0 ClockMenu
   {  MenuItemXPos, MenuItemYPos1, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // ClockMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ClockText, // DisplayText;
	NaviKeyEvent,
	{ AdjustClock,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawClockNumber,// DrawNumberType
	   DrawClockGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 1 FocusMenu
   {  MenuItemXPos, MenuItemYPos1+2, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // FocusMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FocusText, // DisplayText;
	NaviKeyEvent,
	{ AdjustFocus,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawFocusNumber,// DrawNumberType
	   DrawFocusGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 2 HPositionMenu
   {  MenuItemXPos, MenuItemYPos1+4, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // HPositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	HPositionText, // DisplayText;
	NaviKeyEvent,
	{ AdjustHPosition,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawHPositionNumber,// DrawNumberType
	   DrawHPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 3 VPositionMenu
   {  MenuItemXPos, MenuItemYPos1+6, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // VPositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	VPositionText, // DisplayText;
	NaviKeyEvent,
	{ AdjustVPosition,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawVPositionNumber,// DrawNumberType
	   DrawVPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
    {   0, 1, // XPos, YPos;
        0, 10, // ForeColor, BackColor;
        0, 12, // SelForeColor, SelBackColor;
        MainMenu,///NextMenuPage;
        DWI_Icon,// DrawMenuItemType;
        MainMonoText,//ImageSetupText,//ExtraText, // DisplayText;
        NaviKeyEvent,
        { NULL,// AdjustFunction
          NULL,//AutoConfig// ExecFunction
        },
        { NULL,// DrawNumberType
          NULL,// DrawGuageType
          NULL, //rawMenuRadioGroup,
        },
          NULL, //Font
      0// Flags
    },
};

#if !1//OSD_2008
//============ Clock MenuItem==============
MenuItemType code ClockMenuItems[]=
{
// 0 Clock
   {  MenuItemXPos, 3, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ImageSetupMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ClockText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustClock,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawClockNumber,// DrawNumberType
	  DrawClockGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ Focus MenuItem==============
MenuItemType code FocusMenuItems[]=
{
// 0 Focus
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ImageSetupMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FocusText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustFocus,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawFocusNumber,// DrawNumberType
	  DrawFocusGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};


//============ HPosition MenuItem==============
MenuItemType code HPositionMenuItems[]=
{
// 0 HPosition
   {  MenuItemXPos, 7, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ImageSetupMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	HPositionText, // DisplayText;
	 AdjusterKey1Event,
	{ AdjustHPosition,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawHPositionNumber,// DrawNumberType
	  DrawHPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ HPosition MenuItem==============
MenuItemType code VPositionMenuItems[]=
{
// 0 VPosition
   {  MenuItemXPos, 9, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ImageSetupMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	VPositionText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustVPosition,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawVPositionNumber,// DrawNumberType
	  DrawVPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif

//================= ColorTempItems ======================
//--------------------ColorTempText--------------------------
RadioTextType code DrawColorTempSelectText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiEnd|dwiOptionBar,	OPBAR_X_POS, MenuItemYPos,  	ColorTempValueText },
};
DrawRadioGroupType code DrawColorTempSelectIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawColorTempSelectText}
};

//--------------------UserColorRedNumber--------------------------
NumberType code UserColorRedNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos+2,	GetRedColorValue},
};
DrawNumberType code DrawUserColorRedNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 UserColorRedNumber
};
//--------------------UserColorRedIconGuge--------------------------
GaugeType code UserColorRedGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos+2,	GetRedColorValue},
};
DrawGuageType code DrawUserColorRedGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	UserColorRedGuage
};

//--------------------UserColorGreenNumber--------------------------
NumberType code UserColorGreenNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos+4,	GetGreenColorValue},
};
DrawNumberType code DrawUserColorGreenNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 UserColorGreenNumber
};
//--------------------UserColorGreenIconGuge--------------------------
GaugeType code UserColorGreenGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos+4,	GetGreenColorValue},
};
DrawGuageType code DrawUserColorGreenGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	UserColorGreenGuage
};

//--------------------UserColorBlueNumber--------------------------
NumberType code UserColorBlueNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos+6,	GetBlueColorValue},
};
DrawNumberType code DrawUserColorBlueNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 UserColorBlueNumber
};
//--------------------UserColorBlueIconGuge--------------------------
GaugeType code UserColorBlueGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos+6,	GetBlueColorValue},
};
DrawGuageType code DrawUserColorBlueGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	UserColorBlueGuage
};
#if Enable_YMC
//--------------------UserColorYNumber--------------------------
NumberType code UserColorYNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos+8,	GetYColorValue},
};
DrawNumberType code DrawUserColorYNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 UserColorYNumber
};
//--------------------UserColorYIconGuge--------------------------
GaugeType code UserColorYGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos+8,	GetYColorValue},
};
DrawGuageType code DrawUserColorYGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	UserColorYGuage
};

//--------------------UserColorCNumber--------------------------
NumberType code UserColorCNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos+10,	GetCColorValue},
};
DrawNumberType code DrawUserColorCNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,14,	 UserColorCNumber
};
//--------------------UserColorYIconGuge--------------------------
GaugeType code UserColorCGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos+10,	GetCColorValue},
};
DrawGuageType code DrawUserColorCGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	UserColorCGuage
};

//--------------------UserColorMNumber--------------------------
NumberType code UserColorMNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos+12,	GetMColorValue},
};
DrawNumberType code DrawUserColorMNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,16,	 UserColorMNumber
};
//--------------------UserColorMIconGuge--------------------------
GaugeType code UserColorMGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos+12,	GetMColorValue},
};
DrawGuageType code DrawUserColorMGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	UserColorMGuage
};
#endif

MenuItemType code ColorTempMenuItems[]=
{
// 0 ColorTempSelect
   {  MenuItemXPos, MenuItemYPos, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // ColorTempSelectMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ColorTempText, // DisplayText;
	NaviKeyEvent,
	{ AdjustColorTempMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawColorTempSelectIconRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 1 UserColorRedMenu
   {  MenuItemXPos, MenuItemYPos+2, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // RedMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorRText, // DisplayText;
	NaviKeyEvent,
	{ AdjustRedColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorRedNumber,// DrawNumberType
	   DrawUserColorRedGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibUserColor // Flags
  },
// 2 UserColorGreenMenu
   {  MenuItemXPos, MenuItemYPos+4, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // GreenMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorGText, // DisplayText;
	NaviKeyEvent,
	{ AdjustGreenColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorGreenNumber,// DrawNumberType
	   DrawUserColorGreenGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibUserColor // Flags
  },
// 3 UserColorBlueMenu
   {  MenuItemXPos, MenuItemYPos+6, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // BlueMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorBText, // DisplayText;
	NaviKeyEvent,
	{ AdjustBlueColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorBlueNumber,// DrawNumberType
	   DrawUserColorBlueGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibUserColor // Flags
   },

#if Enable_YMC
// 4 UserColorYMenu
   {  MenuItemXPos, MenuItemYPos+8, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // YMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorYText, // DisplayText;
	NaviKeyEvent,
	{ AdjustYColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorYNumber,// DrawNumberType
	   DrawUserColorYGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibUserColor // Flags
  },
// 5 UserColorCMenu
   {  MenuItemXPos, MenuItemYPos+10, // XPos, YPos;
    0, 6, // ForeColor, BackColor;
    0, 12, // SelForeColor, SelBackColor;
    MainMenu, // CMenu,///NextMenuPage;
    DWI_Text,// DrawMenuItemType;
    UserColorCText, // DisplayText;
    NaviKeyEvent,
    { AdjustCColor,// AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    { DrawUserColorCNumber,// DrawNumberType
       DrawUserColorCGuage,// DrawGuageType
      NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
    },
      NULL, //Font
    mibSelectable|mibUserColor // Flags
  },

// 6 UserColorMMenu
   {  MenuItemXPos, MenuItemYPos+12, // XPos, YPos;
    0, 6, // ForeColor, BackColor;
    0, 12, // SelForeColor, SelBackColor;
    MainMenu, // MMenu,///NextMenuPage;
    DWI_Text,// DrawMenuItemType;
    UserColorMText, // DisplayText;
    NaviKeyEvent,
    { AdjustMColor,// AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    { DrawUserColorMNumber,// DrawNumberType
       DrawUserColorMGuage,// DrawGuageType
      NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
    },
      NULL, //Font
    mibSelectable|mibUserColor // Flags
  },
#endif
#if AUTO_COLOR_ITEM_EN
// 7 AUTO COLOR ITEM
   {  MenuItemXPos, MenuItemYPos+8, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // ColorTempSelectMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	AutoColorText1, // DisplayText;
	NaviExecKeyEvent,//NaviKeyEvent,
	{ NULL,// AdjustFunction
	  AutoColor,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
#endif
	{   0, 1, // XPos, YPos;
	    0, 10, // ForeColor, BackColor;
	    0, 12, // SelForeColor, SelBackColor;
	    MainMenu,///NextMenuPage;
	    DWI_Icon,// DrawMenuItemType;
	    MainMonoText,//ExtraText, // DisplayText;
	    NaviKeyEvent,
	    { NULL,// AdjustFunction
	      NULL,//AutoConfig// ExecFunction
	    },
	    { NULL,// DrawNumberType
	      NULL,// DrawGuageType
	      NULL, //rawMenuRadioGroup,
	    },
	      NULL, //Font
	  0// Flags
	},
};

#if 0

//============ ColorTempSelectMenuItem==============
MenuItemType code ColorTempSelectMenuItems[]=
{
// 0 ColorTempSelect
   {  MenuItemXPos, 3, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorTempMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ColorTempText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustColorTempMode,// AdjustFunction
	   NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawColorTempSelectIconRatioText//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL,//Font
	mibSelectable // Flags
  },
};
#endif
#if !1//OSD_2008
//============ UserColorRed MenuItem==============
MenuItemType code UserColorRedMenuItems[]=
{
// 0 UserColorRedMENU
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorTempMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorRText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustRedColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorRedNumber,// DrawNumberType
	  DrawUserColorRedGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
//============ UserColorGreen MenuItem==============
MenuItemType code UserColorGreenMenuItems[]=
{
// 0 UserColorGreenMENU
   {  MenuItemXPos, 7, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorTempMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorGText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustGreenColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorGreenNumber,// DrawNumberType
	  DrawUserColorGreenGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
//============ UserColorBlue MenuItem==============
MenuItemType code UserColorBlueMenuItems[]=
{
// 0 UserColorBlueMENU
   {  MenuItemXPos, 9, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorTempMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorBText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustBlueColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorBlueNumber,// DrawNumberType
	  DrawUserColorBlueGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

#if Enable_YMC
//============ UserColorY MenuItem==============
MenuItemType code UserColorYMenuItems[]=
{
// 0 UserColorYMENU
   {  MenuItemXPos, 11, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorTempMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorYText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustYColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorYNumber,// DrawNumberType
	  DrawUserColorYGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
//============ UserColorC MenuItem==============
MenuItemType code UserColorCMenuItems[]=
{
// 0 UserColorYMENU
   {  MenuItemXPos, 13, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorTempMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorCText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustCColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorCNumber,// DrawNumberType
	  DrawUserColorCGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
//============ UserColorM MenuItem==============
MenuItemType code UserColorMMenuItems[]=
{
// 0 UserColorMMENU
   {  MenuItemXPos, 15, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorTempMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	UserColorMText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustMColor,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawUserColorMNumber,// DrawNumberType
	  DrawUserColorMGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#endif
//============ ColorBoostMenuItem==============

//--------------------FULLText--------------------------
RadioTextType code DrawFullText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS, MenuItemYPos1,  	FreeColorFullModeText },
};
DrawRadioGroupType code DrawFullIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawFullText}
};

//--------------------NatureSkinText--------------------------
RadioTextType code DrawSkinProtectText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS, MenuItemYPos1+2,  	FreeColorSkinProtectModeText },
};
DrawRadioGroupType code DrawSkinProtectIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawSkinProtectText}
};

//--------------------GreenFieldText--------------------------
RadioTextType code DrawGreenEnhanceText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS, MenuItemYPos1+4,  	FreeColorGreenEnhanceModeText },
};
DrawRadioGroupType code DrawGreenEnhanceIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawGreenEnhanceText}
};

//--------------------SkyBlueText--------------------------
RadioTextType code DrawBlueEnhanceText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS, MenuItemYPos1+6,  	FreeColorBlueEnhanceModeText },
};
DrawRadioGroupType code DrawBlueEnhanceIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawBlueEnhanceText}
};


//--------------------AutoDetectText--------------------------
RadioTextType code DrawAutoDetectText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS, MenuItemYPos1+8,  	FreeColorAutoDetectModeText },
};
DrawRadioGroupType code DrawAutoDetectIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawAutoDetectText}
};


//--------------------DemoText--------------------------
RadioTextType code DrawDemoText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS, MenuItemYPos1+10,  	FreeColorDemoModeText },
};
DrawRadioGroupType code DrawDemoIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawDemoText}
};

MenuItemType code ColorBoostMenuItems[]=
{
// 0 Full
   {  MenuItemXPos, MenuItemYPos1, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // FullMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FullText, // DisplayText;
	NaviKeyEvent,
	{ AdjustColorBoostMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawFullIconRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 1 SkinProtect
   {  MenuItemXPos, MenuItemYPos1+2, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // SkinProtectMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NatureSkinText, // DisplayText;
	NaviKeyEvent,
	{ AdjustSkinProtectMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawSkinProtectIconRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable// Flags
  },
// 2 GreenEnhance
   {  MenuItemXPos, MenuItemYPos1+4, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // GreenEnhanceMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	GreenFieldText, // DisplayText;
	NaviKeyEvent,
	{ AdjustGreenEnhanceMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawGreenEnhanceIconRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable// Flags
  },

// 3 BlueEnhance
   {  MenuItemXPos, MenuItemYPos1+6, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // BlueEnhanceMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SkyBlueText, // DisplayText;
	NaviKeyEvent,
	{ AdjustBlueEnhanceMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawBlueEnhanceIconRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 4 AutoDetect
   {  MenuItemXPos, MenuItemYPos1+8, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // AutoDetectMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	AutoDetectText, // DisplayText;
	NaviKeyEvent,
	{ AdjustAutoDetectMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawAutoDetectIconRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 5 Demo
   {  MenuItemXPos ,MenuItemYPos1+10, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // DemoMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DemoText, // DisplayText;
	NaviKeyEvent,
	{ AdjustDemoMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawDemoIconRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable// Flags
  },
    {   0, 1, // XPos, YPos;
        0, 10, // ForeColor, BackColor;
        0, 12, // SelForeColor, SelBackColor;
        MainMenu,///NextMenuPage;
        DWI_Icon,// DrawMenuItemType;
        MainMonoText,//ExtraText, // DisplayText;
        NaviKeyEvent,
        { NULL,// AdjustFunction
          NULL,//AutoConfig// ExecFunction
        },
        { NULL,// DrawNumberType
          NULL,// DrawGuageType
          NULL, //rawMenuRadioGroup,
        },
          NULL, //Font
      0// Flags
    },
};

#if 0
//===========  FreeColorFullMode MenuItem==============
//------------- FreeColorFullModeText  --------------------
RadioTextType code DrawFull[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	21, 3,  	FreeColorFullModeText },
};
DrawRadioGroupType code DrawFullModeIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawFull}
};


MenuItemType code FreeColorFullMODEMenuItems[]=
{
// 0 FullMemu
   {  MenuItemXPos, 3, // XPos, YPos;
	MenuItemXPos, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorBoostMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FullText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustColorBoostMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawFullModeIconRatioText,//DrawMenuRadioGroup
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if 0
//------------- SkinProtectModeText  --------------------
RadioTextType code DrawSkinProtect[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	21, 5,  	FreeColorSkinProtectModeText},
};
DrawRadioGroupType code DrawSkinProtectModeIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawSkinProtect}
};


MenuItemType code FreeColorSkinProtectMODEMenuItems[]=
{
// 0 SkinProtectMemu
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorBoostMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NatureSkinText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustSkinProtectMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawSkinProtectModeIconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if 0
//------------- GreenEnhanceModeText  --------------------
RadioTextType code DrawGreenEnhance[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	21, 7,  	FreeColorGreenEnhanceModeText},
};
DrawRadioGroupType code DrawGreenEnhanceModeIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawGreenEnhance}
};


MenuItemType code FreeColorGreenEnhanceMenuItems[]=
{
// 0 GreenEnhanceMemu
   {  MenuItemXPos, 7, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorBoostMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	GreenFieldText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustGreenEnhanceMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawGreenEnhanceModeIconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

#endif
#if 0
//------------- BlueEnhanceModeText  --------------------
RadioTextType code DrawBlueEnhance[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	21, 9,  	FreeColorBlueEnhanceModeText},
};
DrawRadioGroupType code DrawBlueEnhanceModeIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawBlueEnhance}
};


MenuItemType code FreeColorBlueEnhanceMenuItems[]=
{
// 0 BlueEnhanceMemu
   {  MenuItemXPos, 9, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorBoostMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SkyBlueText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustBlueEnhanceMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawBlueEnhanceModeIconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};


//------------- AutoDetectModeText  --------------------
RadioTextType code DrawAutoDetect[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	21, 11,  	FreeColorAutoDetectModeText},
};
DrawRadioGroupType code DrawAutoDetectModeIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawAutoDetect}
};


MenuItemType code FreeColorAutoDetectMenuItems[]=
{
// 0 AutoDetectMemu
   {  MenuItemXPos, 11, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorBoostMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	AutoDetectText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustAutoDetectMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawAutoDetectModeIconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//------------- DemoModeText  --------------------
RadioTextType code DrawDemo[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	21, 13,  	 FreeColorDemoModeText},
};
DrawRadioGroupType code DrawDemoModeIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawDemo}
};


MenuItemType code ColorBoostDemoMenuItems[]=
{
// 0 DemoMemu
   {  MenuItemXPos, 13, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ColorBoostMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DemoText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustDemoMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawDemoModeIconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif

//--------------BrightMenuItems --------------------------
//--------------BF_Size--------------------------
NumberType code BF_SizeNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1,	GetBFSizeValue},
};
DrawNumberType code DrawBF_SizeNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 BF_SizeNumber
};
//--------------------BF_SizeIconGuge--------------------------
GaugeType code BF_BFSizeGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1,	GetBFSizeValue},
};
DrawGuageType code DrawBF_SizeGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	BF_BFSizeGuage
};

//--------------BF_BrightNumber--------------------------
NumberType code BF_BrightNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1+2,	GetSubBrightness100Value},
};
DrawNumberType code DrawBF_BrightNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 BF_BrightNumber
};
//--------------------BF_BrightIconGuge--------------------------
GaugeType code BF_BrightGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1+2,	GetSubBrightness100Value},
};
DrawGuageType code DrawBF_BrightGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	BF_BrightGuage
};

//--------------BF_ContrastNumber--------------------------
NumberType code BF_ContrastNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1+4,	GetSubContrast100Value},
};
DrawNumberType code DrawBF_ContrastNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 BF_ContrastNumber
};
//--------------------BF_ContrastIconGuge--------------------------
GaugeType code BF_ContrastGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1+4,	GetSubContrast100Value},
};
DrawGuageType code DrawBF_ContrastGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	BF_ContrastGuage
};
#if 0
//--------------BF_HueNumber--------------------------
NumberType code BF_HueNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	26,  9,	GetHue100Value},
};
DrawNumberType code DrawBF_HueNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 BF_HueNumber
};
//--------------------BF_HueIconGuge--------------------------
GaugeType code BF_HueGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             18,	9,	GetHue100Value},
};
DrawGuageType code DrawBF_HueGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	BF_HueGuage
};
//--------------BF_SaturationNumber--------------------------
NumberType code BF_SaturationNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	26,  11,	GetSaturation100Value},
};
DrawNumberType code DrawBF_SaturationNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 BF_SaturationNumber
};
//--------------------BF_SaturationIconGuge--------------------------
GaugeType code BF_SaturationGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             18,	11,	GetSaturation100Value},
};
DrawGuageType code DrawBF_SaturationGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	BF_SaturationGuage
};
//--------------------BF_SaturationIconGuge--------------------------

RadioTextType code DrawPositionRadioText[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiOptionBar1|dwiEnd,	19, 13,  	 BF_RightArrowText },
};
DrawRadioGroupType code DrawPositionIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12, NULL, DrawPositionRadioText}
};
#endif
//--------------------GAMAMODEText--------------------------
RadioTextType code DrawSwitchText[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiOptionBar|dwiEnd,  OPBAR_X_POS ,MenuItemYPos1+10,  	BF_SwitchValueText},

};
DrawRadioGroupType code DrawSwitchIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawSwitchText}
};

//------------- BFPosition  --------------------

#if  !1//OSD_2008
RadioTextType code DrawBrightFrameMenu[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup|dwiEnd,    0, 2,   PictureBoostText},
};
DrawRadioGroupType code DrawBrightFrameMenuRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12, NULL,DrawBrightFrameMenu}
};
#endif
#if 0
//--------------------BFHSizeNumber--------------------------
NumberType code BFSizeNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	26,  3,	GetBFSizeValue},
};
DrawNumberType code DrawBFSizeNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 BFSizeNumber
};
//--------------------BFHSizeIconGuge--------------------------
GaugeType code BFSizeGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             18,	3,	GetBFSizeValue},
};
DrawGuageType code DrawBFSizeGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	BFSizeGuage
};
#endif
//--------------------BFHPositionNumber--------------------------
NumberType code BFHPositionNumber[]=
{ // Flags, 	XPos,	YPos,	GetHalue
    {dwiEnd,       	NUM_X_POS,  MenuItemYPos1+6,	GetBFHstartValue},

};
DrawNumberType code DrawBFHPositionNumber[]=
{ // F ForeColor,	BackColor,	GetHalue
     0,12,	 BFHPositionNumber
};
//--------------------BFHPositionIconGuge--------------------------
GaugeType code BFHPositionGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetHalue
	{dwiEnd,             GUAGE_X_POS,	MenuItemYPos1+6,	GetBFHstartValue},
};
DrawGuageType code DrawBFHPositionGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetHalue
  	0,	12,	9, 	BFHPositionGuage
};

//--------------------BFVPositionNumber--------------------------
NumberType code BFVPositionNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  MenuItemYPos1+8,	GetBFVstartValue},
};
DrawNumberType code DrawBFVPositionNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 BFVPositionNumber
};
//--------------------BFVPositionIconGuge--------------------------
GaugeType code BFVPositionGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	MenuItemYPos1+8,	GetBFVstartValue},
};
DrawGuageType code DrawBFVPositionGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	BFVPositionGuage
};
//------------- BFPosition  --------------------
#if !1//OSD_2008
RadioTextType code DrawBFPosition[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup|dwiEnd,	0, 2,  	BFPositionText},
};
DrawRadioGroupType code DrawBFPositionRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12, NULL,DrawBFPosition}
};
#endif

MenuItemType code BrightFrameMenuItems[]=
{
// 0 BF_FrameSize
   {  MenuItemXPos, MenuItemYPos1, // XPos, YPos;
    0, 6, // ForeColor, BackColor;
    0, 12, // SelForeColor, SelBackColor;
    MainMenu, // BFFrameSizeMenu,///NextMenuPage;
    DWI_Text,// DrawMenuItemType;
    BF_FrameSizeText, // DisplayText;
    NaviKeyEvent,
    { AdjustBFSize,// AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    {  DrawBF_SizeNumber,// DrawNumberType
       DrawBF_SizeGuage,// DrawGuageType
       NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
    },
      NULL, //Font
    mibSelectable|mibBFEnable|mibDemoDisable // Flags
  },

// 1 BF_Brightness
   {  MenuItemXPos, MenuItemYPos1+2, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // BF_BrightnessMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BF_BrightnessText, // DisplayText;
	NaviKeyEvent,
	{  AdjustSubBrightness,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBF_BrightNumber,// DrawNumberType
	  DrawBF_BrightGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibBFEnable// Flags
  },
// 2 BF_Contrastness
   {  MenuItemXPos, MenuItemYPos1+4, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // BF_ContrastMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BF_ContrastText, // DisplayText;
	NaviKeyEvent,
	{ AdjustSubContrast,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBF_ContrastNumber,// DrawNumberType
	  DrawBF_ContrastGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibBFEnable // Flags
  },
// 2 BFHPositionMenu
   {  MenuItemXPos, MenuItemYPos1+6, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // BFHPositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BFHPositionText, // DisplayText;
	NaviKeyEvent,
	{ AdjustBFHstart,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{  DrawBFHPositionNumber,// DrawNumberType
	   DrawBFHPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibBFEnable// Flags
  },
// 3 BFVPositionMenu
   {  MenuItemXPos, MenuItemYPos1+8, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // BFVPositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BFVPositionText, // DisplayText;
	NaviKeyEvent,
	{ AdjustBFVstart,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBFVPositionNumber,// DrawNumberType
	   DrawBFVPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable|mibBFEnable// Flags
  },
// 5 BF_Switch
   {  MenuItemXPos, MenuItemYPos1+10, // XPos, YPos;
    0, 6, // ForeColor, BackColor;
    0, 12, // SelForeColor, SelBackColor;
    MainMenu, // BFSwitchMenu,///NextMenuPage;
    DWI_Text,// DrawMenuItemType;
    BF_SwitchText, // DisplayText;
    NaviKeyEvent,
    { AdjustBF_Switch,// AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    { NULL,// DrawNumberType
      NULL,// DrawGuageType
      DrawSwitchIconRatioText ,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
    },
      NULL, //Font
    mibSelectable // Flags
  },

    {   0, 1, // XPos, YPos;
        0, 10, // ForeColor, BackColor;
        0, 12, // SelForeColor, SelBackColor;
        MainMenu,///NextMenuPage;
        DWI_Icon,// DrawMenuItemType;
        MainMonoText,//ExtraText, // DisplayText;
        NaviKeyEvent,
        { NULL,// AdjustFunction
          NULL,//AutoConfig// ExecFunction
        },
        { NULL,// DrawNumberType
          NULL,// DrawGuageType
          NULL, //rawMenuRadioGroup,
        },
          NULL, //Font
      0// Flags
    },
};
#if 0
//============ BF_BrightnessMnuItem==============
MenuItemType code BF_BrightnessMenuItems[]=
{
// 0 BF_BrightnessMENU
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	BrightFrameMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BF_BrightnessText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustSubBrightness,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBF_BrightNumber,// DrawNumberType
	  DrawBF_BrightGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ BF_ContrastMnuItem==============
MenuItemType code BF_ContrastMenuItems[]=
{
// 0 BF_ContrastMENU
   {  MenuItemXPos, 7, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	BrightFrameMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BF_ContrastText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustSubContrast,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBF_ContrastNumber,// DrawNumberType
	  DrawBF_ContrastGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ BF_HueMnuItem==============
MenuItemType code BF_HueMenuItems[]=
{
// 0 BF_HueMENU
   {  MenuItemXPos, 9, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	BrightFrameMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
#if !1//OSD_2008
	BF_HueText, // DisplayText;
#else
	NULL, // DisplayText;
#endif
	AdjusterKeyEvent,
	{ AdjustHue,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBF_HueNumber,// DrawNumberType
	  DrawBF_HueGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ BF_SaturationMnuItem==============
MenuItemType code BF_SaturationMenuItems[]=
{
// 0 BF_SaturationMENU
   {  MenuItemXPos, 11, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	BrightFrameMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
#if !1//OSD_2008
	BF_SaturationText, // DisplayText;
#else
	NULL, // DisplayText;
#endif
	AdjusterKeyEvent,
	{AdjustSaturation,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBF_SaturationNumber,// DrawNumberType
	  DrawBF_SaturationGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ BFSwitchMenuItems ==============
MenuItemType code BFSwitchMenuItems[]=
{
   {  MenuItemXPos, 13, // XPos, YPos;

	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	BrightFrameMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BF_SwitchText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustBF_Switch,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawSwitchIconRatioText,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if 0
//-------------ClockNumber------------------------------------------
MenuItemType code BFPositionMenuItems[]=
{
/*
// 0 BFHSizeMenu
   {  4, 4, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	BFFrameSizeMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BF_FrameSizeText, // DisplayText;
	NaviKeyEvent,
	{ NULL,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{  DrawBFSizeNumber,// DrawNumberType
	   DrawBFSizeGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
*/
// 2 BFHPositionMenu
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	BFHPositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BFHPositionText, // DisplayText;
	NaviKeyEvent,
	{ NULL,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBFHPositionNumber,// DrawNumberType
	   DrawBFHPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable// Flags
  },
// 3 BFVPositionMenu
   {  MenuItemXPos, 7, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	BFVPositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BFVPositionText, // DisplayText;
	NaviKeyEvent,
	{ NULL,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBFVPositionNumber,// DrawNumberType
	   DrawBFVPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable// Flags
  },
// 4 PictureBoostText
   {  0, 2, // XPos, YPos;
	0, 6, // SelForeColor, SelBackColor;
	0, 12, // SelForeColor, SelBackColor;
	NULL,///NextMenuPage;
	NULL,// DrawMenuItemType;
	 NULL, // DisplayText;
	NaviKeyEvent,
	{ NULL,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{NULL,//// DrawNumberType
	  NULL,/// DrawGuageType
#if !1//OSD_2008
	DrawBFPositionRatioText,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
#else
	NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
#endif
	},
      NULL, //Font
	0 // Flags
  },
};
#endif
#if 0
//============ BFHSizeMenuMenuItems ==============
MenuItemType code BFFrameSizeMenuItems[]=
{
// 0 BF_HSizeMENU
   {  MenuItemXPos, 3, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	BrightFrameMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BF_FrameSizeText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustBFSize,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBFSizeNumber,// DrawNumberType
	   DrawBFSizeGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if 0
//============ BFVSizeMenuMenuItems ==============
MenuItemType code BFVSizeMenuMenuItems[]=
{
// 0 BF_VSizeMenu
   {  4, 6, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	BF_PositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BFVsizeText, // DisplayText;
	AdjusterKeyEvent,
	{AdjustBFVSize,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBFVSizeNumber,// DrawNumberType
	   DrawBFVSizeGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if 0
//============ BFHPositionMenuMenuItems ==============
MenuItemType code BFHPositionMenuMenuItems[]=
{
// 0 BF_HPositionMENU
   {  MenuItemXPos, 9, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
#if 1//OSD_2008
    BrightFrameMenu,///NextMenuPage;
#else
	BF_PositionMenu,///NextMenuPage;
#endif
	DWI_Text,// DrawMenuItemType;
	BFHPositionText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustBFHstart,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBFHPositionNumber,// DrawNumberType
	   DrawBFHPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if 0
//============ BFVPositionMenuMenuItems ==============
MenuItemType code BFVPositionMenuMenuItems[]=
{
// 0 BF_VPositionMENU
#if 1
   {  MenuItemXPos, 11, // XPos, YPos;
#else
	{  MenuItemXPos, 7, // XPos, YPos;
#endif
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
#if 1
    BrightFrameMenu,///NextMenuPage;
#else
	BF_PositionMenu,///NextMenuPage;
#endif
	DWI_Text,// DrawMenuItemType;
	BFVPositionText, // DisplayText;
	AdjusterKeyEvent,
	{ AdjustBFVstart,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawBFVPositionNumber,// DrawNumberType
	   DrawBFVPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

#endif



//---------------------------------------------------------------------------------

//=========== OsdMenuItems ================================
//--------------------ContrastNumber--------------------------
NumberType code OsdHPositionNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  OSD_HPosition,	GetOSDHPositionValue},
};
DrawNumberType code DrawOsdHPositionNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 OsdHPositionNumber
};
//--------------------OsdHPositionIconGuge--------------------------
GaugeType code OsdHPositionGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	OSD_HPosition,	GetOSDHPositionValue},
};
DrawGuageType code DrawOsdHPositionGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	12,	9, 	OsdHPositionGuage
};

//--------------------OsdVPositionNumber--------------------------
NumberType code OsdVPositionNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  OSD_VPosition,	GetOSDVPositionValue},
};
DrawNumberType code DrawOsdVPositionNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 OsdVPositionNumber
};
//--------------------OsdVPositionIconGuge--------------------------
GaugeType code OsdVPositionGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	OSD_VPosition,	GetOSDVPositionValue},
};
DrawGuageType code DrawOsdVPositionGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	10,	9, 	OsdVPositionGuage
};


//--------------------OsdTimeOutNumber--------------------------
NumberType code OsdTimeOutNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  OSD_Timeout,	GetOSDTimeValue},
};
DrawNumberType code DrawOsdTimeOutNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 OsdTimeOutNumber
};
//--------------------OsdTimeOutIconGuge--------------------------
GaugeType code OsdTimeOutGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	OSD_Timeout,	GetOSDTime100Value},
};
DrawGuageType code DrawOsdTimeOutGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	10,	9, 	OsdTimeOutGuage
};


//--------------------OsdTransNumber--------------------------
NumberType code OsdTransNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  OSD_Trans,	GetOSDTransparence},
};
DrawNumberType code DrawOsdTransNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 OsdTransNumber
};
//--------------------OsdTransIconGuge--------------------------
GaugeType code OsdTansGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	OSD_Trans,	GetOSDTransparence},
};
DrawGuageType code DrawOsdTransGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	10,	9, 	OsdTansGuage
};


//--------------------LanguageText--------------------------
RadioTextType code DrawLanguageText[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiOptionBar|dwiEnd,  OPBAR_X_POS, OSD_Language,  	LanguageValueText},
};
DrawRadioGroupType code DrawLanguageIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawLanguageText}
};
//---------------------------------------------------------------

#ifdef EnableLightBar
//--------------------LightBarNumber--------------------------
NumberType code LightBarNumber[]=
{ // Flags, 	XPos,	YPos,	GetValue
  {dwiEnd,       	NUM_X_POS,  OSD_LightBar,	 GetLightBarValue},
};
DrawNumberType code DrawLightBarNumber[]=
{ // F ForeColor,	BackColor,	GetValue
     0,12,	 LightBarNumber
};
//--------------------LightBarIconGuge--------------------------
GaugeType code LightBarGuage[]=
{ // Flags, ForeColor,	BackColor,	Length, 	GetValue
  {dwiEnd,             GUAGE_X_POS,	OSD_LightBar,	GetLightBarValue},
};
DrawGuageType code DrawLightBarGuage[]=
{ //  ForeColor,	BackColor,	Length,,	GetValue
  	0,	10,	9, 	LightBarGuage
};
#endif

MenuItemType code OsdSetUpMenuItems[]=
{
// 0 OsdHPosition
   {  MenuItemXPos, OSD_HPosition, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // OsdHPositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OsdHPositionText, // DisplayText;
	NaviKeyEvent,
	{ AdjustOSDHPosition,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawOsdHPositionNumber,// DrawNumberType
	  DrawOsdHPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 1 OsdVPosition
   {  MenuItemXPos, OSD_VPosition, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // OsdVPositionMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OsdVPositionText, // DisplayText;
	NaviKeyEvent,
	{ AdjustOSDVPosition,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawOsdVPositionNumber,// DrawNumberType
	  DrawOsdVPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 2 OsdTimeOut
   {  MenuItemXPos, OSD_Timeout, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // OsdTimeOutMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OsdTimeOutText, // DisplayText;
	NaviKeyEvent,
	{ AdjustOSDTime,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawOsdTimeOutNumber,// DrawNumberType
	  DrawOsdTimeOutGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

#if 1
// 3 Transparence
   {  MenuItemXPos, OSD_Trans, // XPos, YPos;
    0, 6, // ForeColor, BackColor;
    0, 12, // SelForeColor, SelBackColor;
    MainMenu, // OsdTransparenceMenu,///NextMenuPage;
    DWI_Text,// DrawMenuItemType;
    OsdTransparenceText, // DisplayText;
    NaviKeyEvent,
    { AdjustOSDTransparence,// AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    { DrawOsdTransNumber,// DrawNumberType
      DrawOsdTransGuage,// DrawGuageType
      NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
    },
      NULL, //Font
    mibSelectable // Flags
  },
#endif
// 4 Language
   {  MenuItemXPos, OSD_Language, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // LanguageMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	LanguageText, // DisplayText;
	NaviKeyEvent,
 	{ AdjustLanguage,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{NULL,// DrawBrightnessNumber,// DrawNumberType
	  NULL,//DrawBrightnessGuage,// DrawGuageType
	  DrawLanguageIconRatioText // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
#ifdef EnableLightBar
// 5 light bar
   {  MenuItemXPos, OSD_LightBar, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu, // LanguageMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	LightBarText, // DisplayText;
	NaviKeyEvent,
 	{ AdjustLightBar,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{DrawLightBarNumber,// DrawNumberType
	  DrawLightBarGuage,// DrawGuageType
	  NULL, // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
#endif
//
   {  0, 1, // XPos, YPos;
	0, 10, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu,///NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	NaviKeyEvent,
 	{ NULL,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL , // DrawOsdSetUpIconRatioText // DrawRadioGroupType
	},
      NULL, //Font
	NULL // Flags
  },
};
#if 0
//============ OsdHPositionMenuItem==============
MenuItemType code OsdHPositionMenuItems[]=
{
// 0 OsdHPosition
   {  MenuItemXPos, 3, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	OsdSetUpMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OsdHPositionText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustOSDHPosition,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawOsdHPositionNumber,// DrawNumberType
	  DrawOsdHPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ OsdVPositionMenuItem==============
MenuItemType code OsdVPositionMenuItems[]=
{
// 0 OsdVPosition
   {  MenuItemXPos, 5, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	OsdSetUpMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OsdVPositionText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustOSDVPosition,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawOsdVPositionNumber,// DrawNumberType
	  DrawOsdVPositionGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

//============ OsdTimeOutMenuItem==============
MenuItemType code OsdTimeOutMenuItems[]=
{
// 0 OsdTimeOutPosition
   {  MenuItemXPos, OSD_Timeout, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	OsdSetUpMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OsdTimeOutText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustOSDTime,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawOsdTimeOutNumber,// DrawNumberType
	  DrawOsdTimeOutGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};


//============ OsdTransparenceMenuItem==============
MenuItemType code OsdTransparenceMenuItems[]=
{
// 0 Osd Transparence
   {  MenuItemXPos, OSD_Trans, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	OsdSetUpMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OsdTransparenceText, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustOSDTransparence,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawOsdTransNumber,// DrawNumberType
	  DrawOsdTransGuage,// DrawGuageType
	  NULL,//DrawMenuRadioGroup,//DrawAutoSetupRadioGroup // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
//------------- LanguageeModeText  --------------------
#if !1//
RadioTextType code DrawLanguage[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup,	4, 4,	Language1Text},
  {dwiOptionBar|dwiRadioGroup|dwiEnd,	19, 4,  	LanguageValue1Text},
};
DrawRadioGroupType code DrawLanguageRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	14,NULL,DrawLanguage}
};
#endif
#if 0
MenuItemType code LanguageMenuItems[]=
{
// 0 LanguageMemu
   {  0, 2, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 6, // SelForeColor, SelBackColor;
	OsdSetUpMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // Language1Text, // DisplayText;
	 AdjusterKeyEvent,
	{ AdjustLanguage,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL,//DrawLanguageRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
//============ ExtraMenuItem==============

//--------------------InputSelectText--------------------------
#if(!Youweipaike)
RadioTextType code DrawInputSelect[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS+1, MenuItemYPos,  	InputSourceText },
};
DrawRadioGroupType code DrawInputSelectIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawInputSelect}
};
#endif
#if 0
//--------------------AutoConfigText--------------------------
RadioTextType code DrawAutoConfig[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	20, 3+(2*Dual),  	YesText },
};
DrawRadioGroupType code DrawAutoConfigIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawAutoConfig}
};
#endif
//--------------------ddcci Text--------------------------
#if Youweipaike
#if DDCCI_ENABLE
RadioTextType code DrawDDCCI[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS+1, MenuItemYPos,   	DDCCIValueText },
};
DrawRadioGroupType code DrawDDCCIIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawDDCCI}
};
#endif
#else
#if DDCCI_ENABLE
RadioTextType code DrawDDCCI[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS+1, MenuItemYPos+2+(2*1)/*Dual)*/+2*DDCCI_ENABLE,   	DDCCIValueText },
};
DrawRadioGroupType code DrawDDCCIIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawDDCCI}
};
#endif
#endif

#ifdef OffPower      //090331 xiandi.yu Auto Off Power
NumberType code DrawOffTimerNumber[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiOptionBar|dwiZERONumber|dwiEnd,	OPBAR_X_POS+2, MenuItemYPos+2+(2*1)/*Dual)*/+2*DDCCI_ENABLE+2, GetAutoPowerOffValue},
};
DrawNumberType code DrawAutoOffTimerNumber[]=
{ //ForeColor,	BackColor,	 GetValue
  0,	12, DrawOffTimerNumber
};
#endif

#if Enable_Expansion
RadioTextType code DrawExpansion[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	OPBAR_X_POS+1, MenuItemYPos+2+(2*1)/*Dual)*/,   	ExpansionValueText},
};
DrawRadioGroupType code DrawExpansionText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawExpansion}
};
#endif
#if 0
//--------------------ResetText--------------------------
RadioTextType code DrawReset[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	20, 5+ExtraOsd_offset+(2*Dual),  	YesText },
};
DrawRadioGroupType code DrawResetIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawReset}
};
#endif
#if 0
//--------------------InformationText--------------------------
RadioTextType code DrawInformation[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
#if !1//OSD_2008
  {dwiOptionBar1|dwiEnd,	20, 7+ExtraOsd_offset+(2*Dual),  	InformationArrowText },
#else
  {dwiOptionBar1|dwiEnd,    20, 5+ExtraOsd_offset+(2*Dual),     InformationArrowText },
#endif

};
DrawRadioGroupType code DrawInformationIconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawInformation}
};
#endif
RadioTextType code DrawAutoConfigText[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiOptionBar|dwiEnd, OPBAR_X_POS+1, MenuItemYPos+(2*1)/*Dual)*/,  	AutoSwitchValueText},
};
DrawRadioGroupType code DrawAutoConfigRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawAutoConfigText}
};

MenuItemType code ExtraMenuItems[]=
{
#if 1//Dual

	#if Youweipaike
	#if DDCCI_ENABLE
// 1  DDCCI
   { MenuItemXPos, MenuItemYPos, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu,//DDCCIMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DDCCIText,// DisplayText;
	NaviKeyEvent,
	{ AdjustDDCCI,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawDDCCIIconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable// Flags
  },
#endif

#else
// 0 InputSelect
   {  MenuItemXPos, MenuItemYPos, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu,// InputSelectMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	InputSelectText, // DisplayText;
	NaviKeyEvent,//AdjustSourceKeyEvent,
	{
	#if Dual
	AdjustSource,//ChangeSource,// AdjustFunction
	  #else
	  NULL,//AutoConfig// ExecFunction
	  #endif
	  ChangeSource,//NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawInputSelectIconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
  #endif
  #endif
// 1 AutoConfig
   {  MenuItemXPos, MenuItemYPos+(2*1),//Dual), // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	AutoMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	AutoConfigText, // DisplayText;
	NaviKeyEvent,
	{ AdjustAutoConfig,// AdjustFunction
	  NULL// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawAutoConfigRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable|mibDVIDisable// Flags
  },

#if Enable_Expansion
// 1  DDCCI
   {   MenuItemXPos, MenuItemYPos+2+(2*1),//Dual), // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu,//ExpansionMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ExpansionText,// DisplayText;
	NaviKeyEvent,
	{ AdjustExpansionMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawExpansionText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable | mibSkipImageRatio// Flags
  },
#endif

#if (!Youweipaike)
#if DDCCI_ENABLE
// 1  DDCCI
   { MenuItemXPos, MenuItemYPos+2+(2*1/*Dual)*/)+2*DDCCI_ENABLE, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu,//DDCCIMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DDCCIText,// DisplayText;
	NaviKeyEvent,
	{ AdjustDDCCI,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawDDCCIIconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable// Flags
  },
#endif
#endif
#ifdef OffPower       //090331 xiandi.yu Auto Off Power
// 1  OffTimer
   { MenuItemXPos, MenuItemYPos+2+(2*1/*Dual)*/)+2*DDCCI_ENABLE+2, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu,//DDCCIMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OffTimerText,// DisplayText;
	NaviKeyEvent,
	{ AdjustAutoPowerOff,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ DrawAutoOffTimerNumber,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable// Flags
  },
#endif


#if !1//OSD_2008
// 2 Reset
   {  MenuItemXPos, 5+ExtraOsd_offset+(2*Dual), // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	ResetMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ResetText, // DisplayText;
	NaviKeyEvent,
	{ NULL,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawResetIconRatioText,//DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 3 Information
   {  MenuItemXPos, 7+ExtraOsd_offset+(2*Dual), // XPos, YPos;
    0, 6, // ForeColor, BackColor;
    0, 12, // SelForeColor, SelBackColor;
    InformationMenu,//MainMenu,///NextMenuPage;
    DWI_Text,// DrawMenuItemType;
    InformationText, // DisplayText;
    NaviKeyEvent,
    { NULL,// AdjustFunction
      NULL,//AutoConfig// ExecFunction
    },
    { NULL,// DrawNumberType
      NULL,// DrawGuageType
      DrawInformationIconRatioText,//DrawMenuRadioGroup,
    },
      NULL, //Font
    mibSelectable // NULL // Flags
  },
#else
// 3 Information
   {
   #ifdef OffPower  
   MenuItemXPos, MenuItemYPos+2+ExtraOsd_offset+(2*1)+1,//Dual), // XPos, YPos;
   #else
   MenuItemXPos, MenuItemYPos+2+ExtraOsd_offset+(2*1),//Dual), // XPos, YPos;
   #endif
	0, 6, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
       MainMenu,//InformationMenu,//MainMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	InformationText, // DisplayText;
	NaviKeyEvent,
	{ NULL,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL, // DrawInformationIconRatioText,////DrawMenuRadioGroup,
	},
      NULL, //Font
	NULL, // Flags
  },
#endif
// 4 Extra
   {  0, 1, // XPos, YPos;
	0, 10, // ForeColor, BackColor;
	0, 12, // SelForeColor, SelBackColor;
	MainMenu,//MainMenu,///NextMenuPage;
	DWI_Icon,// DrawMenuItemType;
	MainMonoText, // DisplayText;
	NaviKeyEvent,
	{ NULL,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  NULL,//DrawExtraIconRatioText_1,//rawMenuRadioGroup,
	},
      NULL, //Font
	0// Flags
  },

};
#if 0
//------------- InputSelectMenuItem  --------------------

RadioTextType code DrawInputTypeIcon[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	20, 3,  	InputTypeSelectText},
};
DrawRadioGroupType code DrawInputTypeIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawInputTypeIcon}
};

MenuItemType code InputSelectMenuItems[]=
{
// 0 Analog
   {  MenuItemXPos, 3, // XPos, YPos;
	0, 14, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ExtraMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	InputSelectText, // DisplayText;
	NaviExecKeyEvent,//AdjustExecKeyEvent,
	{NULL,//AdjustInputSource,// AdjustFunction
	  ChangeSourceToAnalog,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawInputTypeIconRationText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
#if Dual
// 1 Digital
   {  MenuItemXPos, 3, // XPos, YPos;
	0, 14, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ExtraMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	InputSelectText, // DisplayText;
	 NaviExecKeyEvent,
	{ NULL, // AdjustFunction
	  ChangeSourceToDigital,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawInputTypeIconRationText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
  #endif
};
#endif
#if 0
//------------- AutoConfigMenuItem  --------------------
RadioTextType code DrawYesIcon[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	olorTempText},
  {0,	4, 3+(2*Dual),  	AutoConfigText },
  {dwiOptionBar|dwiEnd,	20, 3+(2*Dual),  	YesText},
};
DrawRadioGroupType code DrawYesIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawYesIcon}
};

RadioTextType code DrawNoIcon[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {0,	4, 3+(2*Dual),  	AutoConfigText },
  {dwiOptionBar|dwiEnd,	20, 3+(2*Dual),  	NoText},
};
DrawRadioGroupType code DrawNoIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawNoIcon}
};

MenuItemType code AutoConfigMenuItems[]=
{
// 1 No
   {  4, 3+(2*Dual), // XPos, YPos;
	0, 14, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ExtraMenu,///NextMenuPage;
	DWI_Text,//// DrawMenuItemType;
	AutoConfigText, // DisplayText;
	 NaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawNoIconRationText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 0 Yes
   {  4, 3+(2*Dual), // XPos, YPos;
	0, 14, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	AutoMenu,///NextMenuPage;
	DWI_Text,//// DrawMenuItemType;
	AutoConfigText, // DisplayText;
	NaviKeyEvent,
     {
	NULL,// AdjustFunction
      	NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawYesIconRationText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },

};
#endif
//------------- DCCCIMenuItem  --------------------
#if 0//DDCCI_ENABLE
RadioTextType code DrawDDCCIIcon[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	20, 5+(2*Dual),  	DDCCIValueText},
};
DrawRadioGroupType code DrawDDCCI2IconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawDDCCIIcon}
};

MenuItemType code DDCCIMenuItems[]=
{
// 0
   {  MenuItemXPos, 5+(2*Dual), // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ExtraMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DDCCIText, // DisplayText;
	AdjustNoExecKeyEvent, //Benz 2007.4.13   22:32:56	 AdjustExecKeyEvent,
	{ AdjustDDCCI,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawDDCCI2IconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

#endif

#if 0//Enable_Expansion
RadioTextType code DrawExpansionIcon[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {dwiOptionBar|dwiEnd,	20, 5+(2*Dual)+2*DDCCI_ENABLE,  	ExpansionValueText},
};
DrawRadioGroupType code DrawExpansion2IconRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawExpansionIcon}
};

MenuItemType code ExpansionMenuItems[]=
{
// 0
   {  4, 5+(2*Dual)+2*DDCCI_ENABLE, // XPos, YPos;
	0, 6, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ExtraMenu,///NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ExpansionText, // DisplayText;
AdjustNoExecKeyEvent, //Benz 2007.4.13   22:32:56	 AdjustExecKeyEvent,

	{ AdjustExpansionMode,// AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawExpansion2IconRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

#endif

//------------- ResetMenuItem  --------------------

#if  1//OSD_2008

RadioTextType code DrawResetSwitchText[]=
{ // Flags,            XPos,	YPos,	DisplayText
    //{0,   4, 2+(2*Dual)+1,   ResetText },
  {dwiOptionBar|dwiEnd, OPBAR_X_POS+1-2, 5,  	ResetValueText},//{dwiOptionBar|dwiEnd, OPBAR_X_POS+1, 2+(2*Dual)+1,  	ResetValueText},
};
DrawRadioGroupType code DrawResetRatioText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawResetSwitchText}
};

MenuItemType code ResetMenuItems[]=
{
   {  3, 5, // XPos, YPos;//5, 2+(2*Dual)+1, // XPos, YPos;
	0, 10, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ResetWaitMenu,///NextMenuPage;
	DWI_Text,//// DrawMenuItemType;
	ResetText, // DisplayText;
	ResetKeyEvents,//NaviKeyEvent,
	{ AdjustResetSwitch, // AdjustFunction
	  NULL,//Reset// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	  DrawResetRatioText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
#if AOC_MENU_FRAME_MODE
{   0, 1, // XPos, YPos;
	    0, 10, // ForeColor, BackColor;
	    0, 12, // SelForeColor, SelBackColor;
	    MainMenu,///NextMenuPage;
	    DWI_Icon,// DrawMenuItemType;
	    MainMonoText,//ExtraText, // DisplayText;
	    NaviKeyEvent,
	    { NULL,// AdjustFunction
	      NULL,//AutoConfig// ExecFunction
	    },
	    { NULL,// DrawNumberType
	      NULL,// DrawGuageType
	      NULL, //rawMenuRadioGroup,
	    },
	      NULL, //Font
	  0// Flags
	},
 #endif
};
#else
RadioTextType code DrawResetYesIcon[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	olorTempText},
  {0,	4, 5+ExtraOsd_offset+(2*Dual),  	ResetText },
  {dwiOptionBar|dwiEnd,	20, 5+ExtraOsd_offset+(2*Dual),  	YesText},
};
DrawRadioGroupType code DrawResetYesIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawResetYesIcon}
};

RadioTextType code DrawResetNoIcon[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	ColorTempText},
  {0,	4, 5+ExtraOsd_offset+(2*Dual),  	ResetText },
  {dwiOptionBar|dwiEnd,	20, 5+ExtraOsd_offset+(2*Dual),  	NoText},
};
DrawRadioGroupType code DrawResetNoIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawResetNoIcon}
};
MenuItemType code ResetMenuItems[]=
{
// 1 No
   {  4, 5+ExtraOsd_offset+(2*Dual), // XPos, YPos;
	0, 14, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ExtraMenu,///NextMenuPage;
	DWI_Text,//// DrawMenuItemType;
	ResetText, // DisplayText;
	 NaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL,//Reset// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawResetNoIconRationText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
// 0 Yes
   {  4, 5+ExtraOsd_offset+(2*Dual), // XPos, YPos;
	0, 14, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ResetWaitMenu,///NextMenuPage;
	DWI_Text,//// DrawMenuItemType;
	ResetText, // DisplayText;
	 NaviKeyEvent,
     {
	NULL,// AdjustFunction
      	NULL,// AReset// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 DrawResetYesIconRationText,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
  {   0, 1, // XPos, YPos;
	  0, 10, // ForeColor, BackColor;
	  0, 12, // SelForeColor, SelBackColor;
	  MainMenu,///NextMenuPage;
	  DWI_Icon,// DrawMenuItemType;
	  MainMonoText,//ExtraText, // DisplayText;
	  NaviKeyEvent,
	  { NULL,// AdjustFunction
	    NULL,//AutoConfig// ExecFunction
	  },
	  { NULL,// DrawNumberType
	    NULL,// DrawGuageType
	    NULL, //rawMenuRadioGroup,
	  },
	    NULL, //Font
  	0// Flags
  },
};
#endif
#if 0
MenuItemType code InformationMenuItems[]=
{
//
   {  4, 7+ExtraOsd_offset, // XPos, YPos;
	0, 14, // ForeColor, BackColor;
	0, 14, // SelForeColor, SelBackColor;
	ExtraMenu,///NextMenuPage;
	DWI_Text,//// DrawMenuItemType;
	 NULL, // DisplayText;
	 NaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL,//Reset// ExecFunction
	},
	{ NULL,// DrawNumberType
	  NULL,// DrawGuageType
	 NULL,//DrawMenuRadioGroup,
	},
      NULL, //Font
	mibSelectable // Flags
  },
};
#endif
#if 0
//------------- HotKeyECOMenuItem  --------------------
RadioTextType code DrawHotKeyECOText[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	olorTempText},
  {dwiRadioGroup|dwiEnd,0, 4,  	ECOModeValue},

};
DrawRadioGroupType code DrawHotKeyECOIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL, DrawHotKeyECOText}
};
#endif

MenuItemType code HotKeyECOMenuItems[]=
{//0  Icon
  {   0, 5, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	CP_BlackColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_CenterText,// DrawMenuItemType;
	NULL,//LogoText, // DisplayText;
	ECOHotKeyEvent,//HotKeyEvent,//AdjusterKeyEvent,//HotEcoKeyEvent, // ECOHotKeyEvent,// KeyEvent
	{ AdjustECOMode, // AdjustFunction
	  NULL	// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL, // DrawHotKeyECOIconRationText// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};


//------------- ResetMenuItem  --------------------
RadioTextType code DrawResetWaitMenu[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	olorTempText},
  {dwiRadioGroup,	0, 4,      	ResetText_1 },
  {dwiRadioGroup|dwiEnd,0, 5,  	ResetText_2 },

};
DrawRadioGroupType code DrawResetWaitMenuIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawResetWaitMenu}
};


MenuItemType code ResetWaitMenuMenuItems[]=
{//0 Reset Icon
  {   0, 5, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	CP_BlackColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL,//LogoText, // DisplayText;
	NothingKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	  NULL	// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawResetWaitMenuIconRationText// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};

//------------- InputInfoMenu  --------------------
#if DisplayInputInfo
RadioTextType code DrawInputInfo[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup|dwiEnd,0, 5,  	InputConnectorText },
};
DrawRadioGroupType code DrawInputInfoIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawInputInfo}
};
#endif
MenuItemType code InputInfoMenuItems[]=
{//0 Reset Icon
  {   0, 5, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	0,  6, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_CenterText,// DrawMenuItemType;
	NULL, // DisplayText;
	NothingKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	  NULL	// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
#if DisplayInputInfo
        DrawInputInfoIconRationText
#else
	  NULL	// DrawRadioGroupType
#endif
	},
    	NULL, //Font
	NULL // Flags
  }
};

#if DisplayEnergyStar
MenuItemType code EngStarMenuItems[]=
{//0 Reset Icon
  {   0, 5, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	0,  6, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_CenterText,// DrawMenuItemType;
	NULL, // DisplayText;
#if AllInOneKey // 071219 modified for power saving key event
	PwrSavingKeyEvent,//NothingKeyEvent,//PowerOffKeyEvent, //KeyEvent 
#else
	NothingKeyEvent,//PowerOffKeyEvent, //KeyEvent 
#endif	
	{ NULL, // AdjustFunction
	  NULL	// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
    	NULL, //Font
	NULL // Flags
  }
};

#endif

#if DDCCI_ENABLE
//------------- DdcciInfoMenu  --------------------
RadioTextType code DrawDdcciInfo[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup|dwiEnd,	20, 4,  	DDCCIValueText},

};
DrawRadioGroupType code DrawDdcciInfoIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	6,NULL,DrawDdcciInfo}
};
MenuItemType code DdcciInfoMenuItems[]=
{//0 Reset Icon
  {   4, 4, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	0,  6, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DDCCIText, // DisplayText;
	 LockKeyEvent,// KeyEvent //Benz 2007.4.24   21:48:06
	{ NULL, // AdjustFunction
	   NULL// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawDdcciInfoIconRationText	// DrawRadioGroupType
	},
       MenuPage49, //Font
	mibSelectable // Flags
  }
};
#endif
#if 0
//------------- AutoMenuItem  --------------------
RadioTextType code DrawAuto[]=
{ // Flags,            XPos,	YPos,	DisplayText
//  {dwiRadioGroup,	4, 3,  	olorTempText},
  {dwiRadioGroup,	0, 4,      	AutoText_1 },
  {dwiRadioGroup|dwiEnd,0, 5,  	AutoText_2 },

};
DrawRadioGroupType code DrawAutoIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawAuto}
};
#endif

MenuItemType code AutoMenuItems[]=
{//0 Reset Icon
  {   0, 5, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	0,  6, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL,//LogoText, // DisplayText;
	NothingKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	  NULL	// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL,//DrawAutoIconRationText	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};

//------------- AutoMenuItem  --------------------
RadioTextType code DrawOsdLock[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup|dwiEnd,0, 5,  	OsdLockText },

};
DrawRadioGroupType code DrawOsdLockIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawOsdLock}
};

MenuItemType code OsdLockMenuItems[]=
{
  // OsdLockMenu
  { 0, 5, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	0,  6, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	LockKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	  NULL			// ExecFunction
	},
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawOsdLockIconRationText	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};





MenuItemType code AutoColorMenuItems[]=
{//0 Reset Icon
  {   10, 5, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	0,  6, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	AutoColorMenuText,//LogoText, // DisplayText;
	NothingKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	  NULL	// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL, // DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};







//===============================================================
MenuItemType code BurninMenuItems[]=
{// Contrast Icon
  { 2, 1, // XPos, YPos;
	CP_RedColor, CP_WhiteColor, // ForeColor, BackColor;
	CP_RedColor, CP_WhiteColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_CenterText,// DrawMenuItemType;
	NULL, // DisplayText;
	NothingKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	  NULL			// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};


MenuItemType code LogoMenuItems[]=
{//0 Reset Icon
  { 0, 9, // 8, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	CP_BlackColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	NULL,//DWI_Text,// DrawMenuItemType;
	NULL,//LogoText, // DisplayText;
	LogoKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	  NULL	// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};

#ifdef OffPower
//------------- OffPowerWarningMenuItem  --------------------
RadioTextType code DrawOffPowerWarningMenu[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup,		 0, 4,	MonitorOffText_1 },
  {dwiRadioGroup|dwiEnd, 0, 5, 	MonitorOffText_2 },

};
DrawRadioGroupType code DrawOffPowerWarningMenuIconRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawOffPowerWarningMenu}
};


MenuItemType code OffPowerWarningMenuItems[]=
{//0 Reset Icon
  {   0, 5, // XPos, YPos;
	CP_BlackColor, CP_WhiteColor, // ForeColor, BackColor;
	CP_BlackColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL,//LogoText, // DisplayText;
	OffPowerWarningKeyEvent,// KeyEvent
	{ NULL, // AdjustFunction
	  NULL	// ExecFunction
	},// *DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawOffPowerWarningMenuIconRationText// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};
#endif

//==============================================================================================
MenuPageType code tblMenus[]=
{ // 0 PowerOffMenu
  { 16, 3, //	XSize, YSize;
	PowerOnMenu,// PrevMenuPage;
	PowerOffMenuItems, // MenuItems;
	sizeof(PowerOffMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	PowerOffSystem,// ExecFunction;
       NULL, // Fonts
	mpbInvisible|mpbStay // 	Flags;
  },
  // 1 PowerOnMenu
  { 16, 3, //	XSize, YSize;
	RootMenu,// PrevMenuPage;
	NULL, // MenuItems;
	NULL, // MenuItemCount;
	PowerOnSystem,// ExecFunction;
      NULL, // Fonts
	mpbInvisible // 	Flags;
  },
  // 2 StandbyOnMenu
  { HOTKEY_X_SIZE/*29*/, HOTKEY_Y_SIZE, //	XSize, YSize;
	RootMenu,// PrevMenuPage;
	StandbyMenuItems, // MenuItems;
	sizeof(StandbyMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	EnablePowerDownCounter, // ExecFunction;
	#if LANGUAGE_TYPE == All14Language_AddJap || LANGUAGE_TYPE == ASIA
    MenuPage50, // Fonts
	#else
    MenuPage2, // Fonts
    #endif
	mpbStay|mpbRedraw//|mpbCenter//	Flags;
  },

/*
  // 3 SuspendOnMenu
  { 24, 5, //	XSize, YSize;
	RootMenu,// PrevMenuPage;
	StandbyMenuItems, // MenuItems;
	sizeof(StandbyMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	EnablePowerDownCounter, // ExecFunction;
	mpbStay //	 Flags;
  },
*/
  // 3 SleepMenu
  { HOTKEY_X_SIZE/*29*/, 13, //	XSize, YSize;
	RootMenu,// PrevMenuPage;
	SleepMenuItems, // MenuItems;
	sizeof(SleepMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	EnablePowerDownCounter, // ExecFunction;
      NULL, // Fonts
	mpbStay|mpbRedraw//|mpbCenter //	 Flags;
  },
  // 4 CableNotConnectMenu
  { HOTKEY_X_SIZE/*29*/, 11,//13, //	XSize, YSize;
	RootMenu,// PrevMenuPage;
	CableNotConnectedMenuItems, // MenuItems;
	sizeof(CableNotConnectedMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL,//OsdTimerCountDown, //EnablePowerDownCounter, //NULL, // ExecFunction;
#if INPUT_TYPE==INPUT_1A	//071225
    MenuPage4,//  NULL, // Fonts
#else
	NULL,
#endif	
	mpbMoving|mpbStay//|mpbCenter//	Flags;
  },
    // 5 UnsupportedModeMenu
//	{ 24, 6, // 	XSize, YSize;
	{ HOTKEY_X_SIZE/*29*/, HOTKEY_Y_SIZE,// 	XSize, YSize;
	RootMenu,// PrevMenuPage;
	UnsupportedModeMenuItems, // MenuItems;
	sizeof(UnsupportedModeMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL,//OsdTimerCountDown, //NULL, // ExecFunction;
      MenuPage5, // Fonts
	mpbMoving|mpbStay//|mpbCenter//	Flags;
  },
  // 6 RootMenu
  { 16, 3, //	XSize, YSize;
	RootMenu,// PrevMenuPage;
	RootMenuItems, // MenuItems;
	sizeof(RootMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	EnterRootMenu, // ExecFunction;
      NULL, // Fonts
	mpbInvisible|mpbStay //   Flags;
  },
//7 MainMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	RootMenu,// PrevMenuPage;
	MainMenuItems, // MenuItems;
	sizeof(MainMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL,//MenuPage60, //MenuPage7, // Fonts
	mpbStay|mpbBrowse|mpbRedraw//   Flags;
  },
//8 LuminanceMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	MainMenu,// PrevMenuPage;
	LuminanceMenuItems, // MenuItems;
	sizeof(LuminanceMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    MenuPage7, // // Fonts
	mpbStay|mpbBrowse|mpbRedraw //   Flags;
  },

#if !1//OSD_2008

//9 ContrastMenu
  {   31/*29*/, 21, //	XSize, YSize;
	LuminanceMenu,// PrevMenuPage;
	ContrastMenuItems, // MenuItems;
	sizeof(ContrastMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse//|mpbAdjust //   Flags;
  },
//10 BrightnessMenu
  {   31/*29*/, 21, //	XSize, YSize;
	LuminanceMenu,// PrevMenuPage;
	BrightnessMenuItems, // MenuItems;
	sizeof(BrightnessMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse//|mpbAdjust //   Flags;
  },
//11 ECOMODEMenu
  {   31/*29*/, 21, //	XSize, YSize;
	LuminanceMenu,// PrevMenuPage;
	ECOMODEMenuItems, // MenuItems;
	sizeof(ECOMODEMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse//|mpbAdjust //   Flags;
  },

//12 GAMAMODEMenu
  {   31/*29*/, 21, //	XSize, YSize;
	LuminanceMenu,// PrevMenuPage;
	GAMAMODEMenuItems, // MenuItems;
	sizeof(GAMAMODEMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse//|mpbAdjust //   Flags;
  },

//13 DCRMenu
  {   31/*29*/, 21, //    XSize, YSize;
	  LuminanceMenu,// PrevMenuPage;
	  DCRMenuItems, // MenuItems;
	  sizeof(DCRMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	  NULL, // ExecFunction;
	  NULL, // // Fonts
	  mpbStay|mpbBrowse//|mpbAdjust //   Flags;
  },
  #endif
//14 ImageSetupMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	MainMenu,// PrevMenuPage;
	ImageSetupMenuItems, // MenuItems;
	sizeof(ImageSetupMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    MenuPage13, // NULL,  // Fonts
	mpbStay|mpbBrowse|mpbRedraw //   Flags;
  },
#if !1//OSD_2008
//15 ClockMenu,
  {   31/*29*/, 21, //	XSize, YSize;
	ImageSetupMenu,// PrevMenuPage;
	ClockMenuItems, // MenuItems;
	sizeof(ClockMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//16 FocusMenu,
  {   31/*29*/, 21, //	XSize, YSize;
	ImageSetupMenu,// PrevMenuPage;
	FocusMenuItems, // MenuItems;
	sizeof(FocusMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//17 HPositionMenu,
  {   31/*29*/, 21, //	XSize, YSize;
	ImageSetupMenu,// PrevMenuPage;
	HPositionMenuItems, // MenuItems;
	sizeof(HPositionMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//18 VPositionMenu,
  {   31/*29*/, 21, //	XSize, YSize;
	ImageSetupMenu,// PrevMenuPage;
	VPositionMenuItems, // MenuItems;
	sizeof(VPositionMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
#endif
//19 ColorTempMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	MainMenu,// PrevMenuPage;
	ColorTempMenuItems, // MenuItems;
	sizeof(ColorTempMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    MenuPage18, // NULL, // // Fonts
	mpbStay|mpbBrowse|mpbRedraw //   Flags;
  },
#if 0
//20 ColorTempSelectMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ColorTempMenu,// PrevMenuPage;
	ColorTempSelectMenuItems, // MenuItems;
	sizeof(ColorTempSelectMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL,//SetColorTemp, // NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
#endif
#if !1//OSD_2008
//21 UserColorRedMenu,
  {   31/*29*/, 21, //	XSize, YSize;
	ColorTempMenu,// PrevMenuPage;
	UserColorRedMenuItems, // MenuItems;
	sizeof(UserColorRedMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//22 UserColorGreenMenu,
  {   31/*29*/, 21, //	XSize, YSize;
	ColorTempMenu,// PrevMenuPage;
	UserColorGreenMenuItems, // MenuItems;
	sizeof(UserColorGreenMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//23 UserColorBlueMenu,
  {   31/*29*/, 21, //	XSize, YSize;
	ColorTempMenu,// PrevMenuPage;
	UserColorBlueMenuItems, // MenuItems;
	sizeof(UserColorBlueMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },

 #if Enable_YMC
 //24 UserColorYMenu,
    {   31/*29*/, 21, //    XSize, YSize;
      ColorTempMenu,// PrevMenuPage;
      UserColorYMenuItems, // MenuItems;
      sizeof(UserColorYMenuItems)/sizeof(MenuItemType), // MenuItemCount;
      NULL, // ExecFunction;
         NULL, // // Fonts
      mpbStay|mpbBrowse //   Flags;
    },
  //25 UserColorCMenu,
    {   31/*29*/, 21, //    XSize, YSize;
      ColorTempMenu,// PrevMenuPage;
      UserColorCMenuItems, // MenuItems;
      sizeof(UserColorCMenuItems)/sizeof(MenuItemType), // MenuItemCount;
      NULL, // ExecFunction;
         NULL, // // Fonts
      mpbStay|mpbBrowse //   Flags;
    },
  //26 UserColorMMenu,
    {   31/*29*/, 21, //    XSize, YSize;
      ColorTempMenu,// PrevMenuPage;
      UserColorMMenuItems, // MenuItems;
      sizeof(UserColorMMenuItems)/sizeof(MenuItemType), // MenuItemCount;
      NULL, // ExecFunction;
         NULL, // // Fonts
      mpbStay|mpbBrowse //   Flags;
    },
#endif
#endif
//27 ColorBoostMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	MainMenu,// PrevMenuPage;
	ColorBoostMenuItems, // MenuItems;
	sizeof(ColorBoostMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    MenuPage26, //   NULL, // // Fonts
	mpbStay|mpbBrowse|mpbRedraw //   Flags;
  },
#if 0
//28 FullMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ColorBoostMenu,// PrevMenuPage;
	FreeColorFullMODEMenuItems, // MenuItems;
	sizeof(FreeColorFullMODEMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//29 SkinProtectMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ColorBoostMenu,// PrevMenuPage;
	FreeColorSkinProtectMODEMenuItems, // MenuItems;
	sizeof(FreeColorSkinProtectMODEMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//30 GreenEnhanceMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ColorBoostMenu,// PrevMenuPage;
	FreeColorGreenEnhanceMenuItems, // MenuItems;
	sizeof(FreeColorGreenEnhanceMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//31 BlueEnhanceMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ColorBoostMenu,// PrevMenuPage;
	FreeColorBlueEnhanceMenuItems, // MenuItems;
	sizeof(FreeColorBlueEnhanceMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//32 AutoDetectMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ColorBoostMenu,// PrevMenuPage;
	FreeColorAutoDetectMenuItems, // MenuItems;
	sizeof(FreeColorAutoDetectMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//33 DemoMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ColorBoostMenu,// PrevMenuPage;
	ColorBoostDemoMenuItems, // MenuItems;
	sizeof(ColorBoostDemoMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
#endif
//34 BrightFrameMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	MainMenu,// PrevMenuPage;
	BrightFrameMenuItems, // MenuItems;
	sizeof(BrightFrameMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
      MenuPage33, // // Fonts
	mpbStay|mpbBrowse|mpbRedraw// |mpbClrInner//   Flags;
  },
#if 0
//35 BF_FrameSizeMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
  	BrightFrameMenu,// PrevMenuPage;
  	BFFrameSizeMenuItems, // MenuItems;
  	sizeof(BFFrameSizeMenuItems)/sizeof(MenuItemType), // MenuItemCount;
  	NULL, // ExecFunction;
    NULL, // // Fonts
  	mpbStay|mpbBrowse //   Flags;
  },


//36 BF_BrightnessMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	BrightFrameMenu,// PrevMenuPage;
	BF_BrightnessMenuItems, // MenuItems;
	sizeof(BF_BrightnessMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },

//37 BF_ContrastMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	BrightFrameMenu,// PrevMenuPage;
	BF_ContrastMenuItems, // MenuItems;
	sizeof(BF_ContrastMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },

//38 BF_HueMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	BrightFrameMenu,// PrevMenuPage;
	BF_HueMenuItems, // MenuItems;
	sizeof(BF_HueMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },


//39 BF_SaturationMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	BrightFrameMenu,// PrevMenuPage;
	BF_SaturationMenuItems, // MenuItems;
	sizeof(BF_SaturationMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },

  //40 BFSwitchMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
      BrightFrameMenu,// PrevMenuPage;
      BFSwitchMenuItems, // MenuItems;
      sizeof(BFSwitchMenuItems)/sizeof(MenuItemType), // MenuItemCount;
      NULL, // ExecFunction;
      NULL, // // Fonts
      mpbStay|mpbBrowse //   Flags;
  },

 //41 BF_PositionMenu
  {  X_SIZE ,  Y_SIZE, //	XSize, YSize;
	BrightFrameMenu,// PrevMenuPage;
	BFPositionMenuItems,// MenuItems;
	sizeof(BFPositionMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
#if !1//OSD_2008
       MenuPage38, // // Fonts
#else
       NULL, // // Fonts
#endif
	mpbStay|mpbBrowse|mpbClrInner//   Flags;
  },
        #endif
  #if 0
//35 BF_VsizeMenu
  {   29, 18, //	XSize, YSize;
	BrightFrameMenu,// PrevMenuPage;
	BFVSizeMenuMenuItems, // MenuItems;
	sizeof(BFVSizeMenuMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
  #endif
  #if 0
//42 BF_HPosition
  {  X_SIZE ,  Y_SIZE, //	XSize, YSize;
#if 1//OSD_2008
	BrightFrameMenu,// PrevMenuPage;
#else
	BF_PositionMenu, // BrightFrameMenu,// PrevMenuPage;
#endif
	BFHPositionMenuMenuItems, // MenuItems;
	sizeof(BFHPositionMenuMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//43 BF_VPositionMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
#if 1//OSD_2008
    BrightFrameMenu,// PrevMenuPage;
#else
	BF_PositionMenu, // BrightFrameMenu,// PrevMenuPage;
#endif
	BFVPositionMenuMenuItems, // MenuItems;
	sizeof(BFVPositionMenuMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
  #endif

//44 OsdSetUpMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	MainMenu,// PrevMenuPage;
	 OsdSetUpMenuItems ,// MenuItems;
	sizeof( OsdSetUpMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    	MenuPage44, //Font
	mpbStay|mpbBrowse|mpbRedraw //   Flags;
  },
#if 0
//45 OsdHPositionMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	OsdSetUpMenu,// PrevMenuPage;
	 OsdHPositionMenuItems ,// MenuItems;
	sizeof( OsdHPositionMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//46 OsdVPositionMenu
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	OsdSetUpMenu,// PrevMenuPage;
	 OsdVPositionMenuItems ,// MenuItems;
	sizeof( OsdVPositionMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
//47 OsdTimeOutMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	OsdSetUpMenu,// PrevMenuPage;
	OsdTimeOutMenuItems ,// MenuItems;
	sizeof(OsdTimeOutMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
#if 1//OSD_2008
//47 OsdTransparenceMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	OsdSetUpMenu,// PrevMenuPage;
	OsdTransparenceMenuItems ,// MenuItems;
	sizeof(OsdTransparenceMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
#endif

//48 LanguageMenu,
  {   31/*29*/, 10/*18*/, //	XSize, YSize;
	OsdSetUpMenu,// PrevMenuPage;
	LanguageMenuItems ,// MenuItems;
	sizeof(LanguageMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    NULL, //   MenuPage45, //Font
	mpbStay|mpbBrowse|mpbClrInner//   Flags;
  },
#endif
//49 ExtraMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	MainMenu,// PrevMenuPage;
	ExtraMenuItems ,// MenuItems;
	sizeof(ExtraMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    MenuPage49, // // Fonts
	mpbStay|mpbBrowse|mpbRedraw // |mpbClrInner //   Flags;
  },
#if 0
//50 InputSelectMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ExtraMenu, // MainMenu,// PrevMenuPage;
	InputSelectMenuItems ,// MenuItems;
	sizeof(InputSelectMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },

//51 AutoConfigMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ExtraMenu, // MainMenu,// PrevMenuPage;
	AutoConfigMenuItems ,// MenuItems;
	sizeof(AutoConfigMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
 #endif
#if 0//DDCCI_ENABLE
//52 DDCCIMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ExtraMenu, // MainMenu,// PrevMenuPage;
	DDCCIMenuItems ,// MenuItems;
	sizeof(DDCCIMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
#endif
#if 0//Enable_Expansion
//52 ExpansionMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ExtraMenu, // MainMenu,// PrevMenuPage;
	ExpansionMenuItems ,// MenuItems;
	sizeof(ExpansionMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
       NULL, // // Fonts
	mpbStay|mpbBrowse //   Flags;
  },
#endif
//52 ResetMenu,
#if 1//OSD_2008
  {   HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	MainMenu,// PrevMenuPage;
	ResetMenuItems ,// MenuItems;
	sizeof(ResetMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
	#if LANGUAGE_TYPE==All14Language_AddJap
    MenuPage51, // // Fonts
    #else
    MenuPage49, // // Fonts
	#endif
	mpbStay|mpbBrowse //   Flags;
  },
#else
  {   31/*29*/, 21, //  XSize, YSize;
	  ExtraMenu, // MainMenu,// PrevMenuPage;
	  ResetMenuItems ,// MenuItems;
	  sizeof(ResetMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	  NULL, // ExecFunction;
	     NULL, // // Fonts
	  mpbStay|mpbBrowse //   Flags;
  },
#endif
#if 0
//53 InformationMenu,
  {   X_SIZE ,  Y_SIZE, //	XSize, YSize;
	ExtraMenu, // MainMenu,// PrevMenuPage;
	InformationMenuItems ,// MenuItems;
	sizeof(InformationMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
#if 1//OSD_2008
       MenuPage49, // // Fonts
#else
       MenuPage50, // // Fonts
#endif
	mpbStay|mpbBrowse|mpbClrInner//|mpbRedraw //   Flags;
  },
#endif
  // 54 HotKeyECOMenu,
  {     HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	  RootMenu,// PrevMenuPage;
	  HotKeyECOMenuItems ,// MenuItems;
	  sizeof(HotKeyECOMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	  NULL, // ExecFunction;
	  NULL,//MenuPage7, // // Fonts
	  mpbStay|mpbBrowse|mpbRedraw //   Flags; // 071218
  },

#if 1//OSD_2008
//54 HotKeyDCRMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	RootMenu,// PrevMenuPage;
	HotKeyDCRMenuItems ,// MenuItems;
	sizeof(HotKeyDCRMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL ,// ResetAllSetting, // ExecFunction;
    NULL,//MenuPage7, // // Fonts
    mpbStay|mpbBrowse/*|mpbCenter*/|mpbRedraw //   Flags;
  },
#endif
#if Enable_Lightsensor
// HotKeyiCareMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	RootMenu,// PrevMenuPage;
	HotKeyiCareMenuItems ,// MenuItems;
	sizeof(HotKeyiCareMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL ,// ResetAllSetting, // ExecFunction;
    NULL,//MenuPage7, // // Fonts
    mpbStay|mpbBrowse/*|mpbCenter*/|mpbRedraw //   Flags;
  },
#endif
#if HotExpansionAdjust
 {
	HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
   RootMenu,// PrevMenuPage;
   HotKeyExpansionMenuItems ,// MenuItems;
   sizeof(HotKeyExpansionMenuItems)/sizeof(MenuItemType), // MenuItemCount;
   NULL ,// ResetAllSetting, // ExecFunction;
   NULL,//MenuPage7, // // Fonts
   mpbStay|mpbBrowse/*|mpbCenter*/|mpbRedraw //   Flags;
 },
#endif

#if AudioFunc
 // 54 HotKeyVolMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	  RootMenu,// PrevMenuPage;
	  VolumeMenuItems ,// MenuItems;
	  sizeof(VolumeMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	  NULL, // ExecFunction;
	  NULL,//MenuPage58, // // Fonts
	  mpbStay|mpbBrowse|mpbRedraw //   Flags; // 071218
  },
 #if 0
 // 54 HotKeyMuteMenu,
  {  HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	  RootMenu,// PrevMenuPage;
	  HotKeyMuteMenuItems ,// MenuItems;
	  sizeof(HotKeyMuteMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	  SetMute, // ExecFunction;
	  MenuPage58, // // Fonts
	  mpbStay|mpbBrowse|mpbRedraw //   Flags;
  },
#endif
#else 
 // 54 HotConMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	  RootMenu,// PrevMenuPage;
	  HotKeyConMenuItems ,// MenuItems;
	  sizeof(HotKeyConMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	  NULL, // ExecFunction;
	  NULL,//MenuPage58, // // Fonts
	  mpbStay|mpbBrowse|mpbRedraw //   Flags;
  },
#endif
#if EnableHotBrightness
 // 54 HotBriMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	  RootMenu,// PrevMenuPage;
	  HotKeyBriMenuItems ,// MenuItems;
	  sizeof(HotKeyBriMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	  NULL, // ExecFunction;
	  NULL,//MenuPage58, // // Fonts
	  mpbStay|mpbBrowse|mpbRedraw //   Flags;
  },
#endif
#if HotInputSelect
//HotInputSelectMenu
	{	
#if (INPUT_TYPE==INPUT_1A1D||INPUT_TYPE==INPUT_1A1H)
	HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
#elif (INPUT_TYPE==INPUT_1A2D)	
	HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
#else
	HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
#endif	
	  RootMenu,// PrevMenuPage;
	  HotInputSelectMenuItems ,// MenuItems;
	  sizeof(HotInputSelectMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	  EnterHotInputMenu ,// ResetAllSetting, // ExecFunction;
	  NULL,//MenuPage49, // // Fonts
	  mpbRedraw | mpbStay//	Flags;
	},
#endif
//54 ResetWaitMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
    RootMenu,// PrevMenuPage;
    ResetWaitMenuMenuItems ,// MenuItems;
    sizeof(ResetWaitMenuMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    DoResetConfig ,// ResetAllSetting, // ExecFunction;
    MenuPage54, // // Fonts
    mpbRedraw//   Flags;
  },
//55 AutoMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	RootMenu,// PrevMenuPage;
	AutoMenuItems ,// MenuItems;
	sizeof(AutoMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	DoAutoConfig, // ExecFunction;
       NULL,//MenuPage55, // // Fonts
	mpbRedraw //   Flags; // 071218
  },

//56 OsdLockMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	RootMenu,// PrevMenuPage;
	OsdLockMenuItems ,// MenuItems;
	sizeof(OsdLockMenuItems)/sizeof(MenuItemType), // MenuItemCount;
      OsdCountDown, // ExecFunction;
      MenuPage56, // Fonts
    mpbStay//|mpbCenter //   Flags;
  },

// 57 AutoColorMenu
 { 31/*29*/, 13, // 	XSize, YSize;
   RootMenu,// PrevMenuPage;
   AutoColorMenuItems, // MenuItems;
    sizeof(AutoColorMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    AutoColor, // ExecFunction;
    MenuPage57, // Fonts
    mpbRedraw//|mpbCenter//   Flags;
  },
 // 58 BurninMenu
  { 29, 13, // 	XSize, YSize;
    RootMenu,// PrevMenuPage;
    BurninMenuItems, // MenuItems;
    sizeof(BurninMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    EnableBurninMode, // ExecFunction;
    NULL, // Fonts
    mpbStay|mpbInvisible//   Flags;
  },
  #if  1
  // 59 FactoryMenu
  { 27, 29, //17,  	XSize, YSize;  wmz 20050106
    RootMenu,// PrevMenuPage;
    FactoryMenuItems, // MenuItems;
    sizeof(FactoryMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    NULL, // ExecFunction;
    NULL, // Fonts
    mpbStay //   Flags;
  },
  #endif
  // 60 LogoMenu
  {
#if (DisplayLogo==EnvisionLOGO)
      64, 7,  // 	XSize, YSize;   EnvisionLogo
#elif(DisplayLogo==StaplesLOGO)
      36,12,   // 	XSize, YSize;
 #elif(DisplayLogo==PuntaLOGO)
      61,10,   // 	XSize, YSize;
 #elif(DisplayLogo==AcerLOGO)
      54,13,   // 	XSize, YSize;
 #elif(DisplayLogo==WearnesLOGO)
      34,4,   // 	XSize, YSize;
 #else //   EnvisionLogo
      39, 9,   // 	XSize, YSize;
  #endif
	MainMenu,// PrevMenuPage;
	LogoMenuItems, // MenuItems;
	sizeof(LogoMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	EnterDisplayLogo, // ExecFunction;
	NULL, // Fonts
	mpbStay|mpbCenter|mpbLogoFrame //	Flags;
  },
  // 61 InputInfoMenu
  {
     HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
    MainMenu,// PrevMenuPage;
    InputInfoMenuItems, // MenuItems;
    sizeof(InputInfoMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    NULL, // ExecFunction;
#if DisplayInputInfo
    MenuPage50,
#else
     NULL, // Fonts
#endif
    mpbStay|mpbRedraw//|mpbCenter   Flags;
  },
#if DisplayEnergyStar
  // 61 EnergyStarMenu
  {
     HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
    RootMenu,// PrevMenuPage;
    EngStarMenuItems, // MenuItems;
    sizeof(EngStarMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    NULL, // ExecFunction;
     NULL, // Fonts
    mpbStay//|mpbCenter //   Flags;
  },
#endif
#if DDCCI_ENABLE
// 61  DdcciInfoMenu
  {
     HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
    RootMenu,// PrevMenuPage;
    DdcciInfoMenuItems, // MenuItems;
    sizeof(DdcciInfoMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    ExecDDC2Bi, // ExecFunction;
    MenuPage49, // Fonts
    mpbStay//|mpbCenter //   Flags;
  },
#endif
#if 0//EnablePanelServiceMenu
// AjustPanlMenu,                                                   
  {   39 , 26, //	XSize, YSize;
    RootMenu,// PrevMenuPage;  
    AjustPanlMenuItems, // MenuItems;    
    sizeof(AjustPanlMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    NULL, // ExecFunction;
    NULL, // Fonts          
    mpbStay //   Flags;
  },
#endif
#ifdef OffPower
// OffPowerWarningMenu,
  {    HOTKEY_X_SIZE , HOTKEY_Y_SIZE,
	RootMenu,// PrevMenuPage;
	OffPowerWarningMenuItems ,// MenuItems;
	sizeof(OffPowerWarningMenuItems)/sizeof(MenuItemType), // MenuItemCount;
	NULL, // ExecFunction;
    MenuPage59,//MenuPage55, // // Fonts
	mpbStay|mpbRedraw //   Flags; // 071218
  },
#endif
};




