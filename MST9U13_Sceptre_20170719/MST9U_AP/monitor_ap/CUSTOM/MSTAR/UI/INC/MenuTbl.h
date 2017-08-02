///////////////////////////////////////////////////////////////////////////////
/// @file MenuTbl.h
/// @brief Head file to define menu structure data.
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///  -
///////////////////////////////////////////////////////////////////////////////
#ifndef _MENUTBL_H
#define _MENUTBL_H

#include "MenuTblDef.h"
#include "MenuIcon.h"
#include "MenuPropFont.h"
#include "MenuFont.h"

//Define
#define PROP_NUM(i) (PROP_NUM_START+PROP_NUM_LEN*(i)) //For Prop Number location in font memory
#if ENABLE_WIDE_MODE
#define WM_YOFF 2
#else
#define WM_YOFF 0
#endif
#define DCR_YOFF 0

#if ENABLE_VGA_INPUT
#define NO_VGA_SHIFT    0
#else
#define NO_VGA_SHIFT    2
#endif

#if ADJ_BackLight_Enable  
#define PropNumIndex       1
#else
#define PropNumIndex       0
#endif


#define SubTxtXPos                         2
#define SubUserTxtXPos                 (SubTxtXPos+ 1)
#define SubAdvancedTxtXPos             (SubUserTxtXPos+ 1)

#define SubLedModeTxtXPos          (SubTxtXPos+ 1)



#define QuickStartTxtYPos              11
#define PictureTxtYPos                   15
#define ColorTxtYPos                      19
#define ColorUserTxtYPos               (ColorTxtYPos+1)
#define ColorAdvancedTxtYPos      (ColorUserTxtYPos+4)

#define SystemTxtYPos                   23
#define SystemLedModeTxtYPos      (SystemTxtYPos+4)
#define SystemLedModeLEDUSERTxtYPos      (SystemTxtYPos+4)


#define Picture_VGA_TxtYPos          30


#define VGA_SettingY_Pos               26

//ENABLE_ShowInformation_SETTINGS_MENU


//Mstar Logo  //Steven110526
DrawIconType code DrawMainMenuLogoIcon[]=
{//u8Flafs, u8XPos, u8YPos, u8IconWidth,    u8IconHeight,   u8Color,    u8SelColor, fpGetIcon
    {ICO_OVER512|ICO_END,  COORD(1,  WF_YHEIGHT-4), GetMainMenuLogoIcon}
};


RadioTextType code MainMenuQSDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+3)),  TxtMainQSDescription_1},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+4)),  TxtMainQSDescription_2},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+5)),  TxtMainQSDescription_3},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+6)),  TxtMainQSDescription_4},

  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawMainMenuQSDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, MainMenuQSDescRadioText}
};
RadioTextType code MainMenuPicDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+3)),  TxtMainPicDescription_1},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+4)),  TxtMainPicDescription_2},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+5)),  TxtMainPicDescription_3},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+6)),  TxtMainPicDescription_4},

  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawMainMenuPicDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, MainMenuPicDescRadioText}
};

RadioTextType code MainMenuColDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+3)),  TxtMainColDescription_1},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+4)),  TxtMainColDescription_2},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+5)),  TxtMainColDescription_3},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+6)),  TxtMainColDescription_4},

  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawMainMenuColDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, MainMenuColDescRadioText}
};

RadioTextType code MainMenuSysDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+3)),  TxtMainSysDescription_1},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+4)),  TxtMainSysDescription_2},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+5)),  TxtMainSysDescription_3},
  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+6)),  TxtMainSysDescription_4},

  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawMainMenuSysDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, MainMenuSysDescRadioText}
};


RadioTextType code MainMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
//  {TXT_OVER_512,  COORD(1, (Main_TopLine_YPos-1)),  TxtCurrentSourceInput},
//  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+3)),  TxtDescription_1},
//  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+4)),  TxtDescription_2},
//  {TXT_OVER_256,  COORD(1, (Main_Description_YPos+5)),  TxtDescription_3},

  {TXT_OVER_512,  COORD(3, (MM_YSIZE-2)),  TxtMenu},
  {TXT_OVER_512,  COORD(9, (MM_YSIZE-2)),  TxtUp},
  {TXT_OVER_512,  COORD(13, (MM_YSIZE-2)),  TxtDown},
  {TXT_OVER_512,  COORD(19, (MM_YSIZE-2)),  TxtExit},
  {TXT_OVER_512,  COORD(25, (MM_YSIZE-2)),  TxtPower},

  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawMainMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, MainMenuRadioText}
};


MenuItemType code MainMenuItems[]=
{
  {
    COORD(0,(10+Main_Quick_Start_IDX*9)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    NaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawMainMenuQSDescRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

 //MENU_PICTURE
 {
   COORD(0,(10+Main_Picture_IDX*9)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
   NaviKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawMainMenuPicDescRadioGroup, // DrawRadioGroupType
 	NULL, //DrawDisplaySettingsIcon//DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },

    //MENU_COLOR
  {
    COORD(0,(10+Main_Color_IDX*9)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    NaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawMainMenuColDescRadioGroup,	// DrawRadioGroupType
      NULL, //DrawColorSettingsIcon//DrawIconType
    },
    MIB_SELECTABLE// u16Flags
  },
 
 //MENU_SYSTEM
 {
   COORD(0,(10+Main_System_IDX*9)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8SelColor
   MENU_SYSTEM, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
   NaviKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawMainMenuSysDescRadioGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },
  //Factory
  {
    COORD(0,  0), // u8XPos, u8YPos;
    MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_ADJ_COLOR_L2, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_512,// u8TXTFlags;
    MainFactoryText, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetGotoFactoryMenu			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawAudioSettingsIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_FACTORY_ITEM// u16Flags
  },

    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      DrawMainMenuRadioGroup,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};


RadioTextType code BackLightDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtBackLightDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtBackLightDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawBackLightDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, BackLightDescRadioText}
};

RadioTextType code BrightnessDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtBrightnessDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtBrightnessDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawBrightnessDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, BrightnessDescRadioText}
};

RadioTextType code ContrastDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtContrastDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtContrastDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawContrastDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, ContrastDescRadioText}
};

RadioTextType code PresetDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtPresetDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtPresetDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawPresetDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, PresetDescRadioText}
};

RadioTextType code AspectDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtAspectDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtAspectDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawAspectDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, AspectDescRadioText}
};

RadioTextType code SourceDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtSourceDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtSourceDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawSourceDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, SourceDescRadioText}
};

RadioTextType code BlueLightDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtBlueLightDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtBlueLightDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawBlueLightDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, BlueLightDescRadioText}
};

#if Free_Sync_Enable
RadioTextType code FreeSyncDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtFreeSyncDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtFreeSyncDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawFreeSyncDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, FreeSyncDescRadioText}
};
#endif

DrawGaugeType code DrawQSBackLightGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (QuickStartTxtYPos+QuickStart_BackLight_IDX*1)),GetBrightnessValue},
};

DrawNumberType code DrawQSBackLightNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (QuickStartTxtYPos+QuickStart_BackLight_IDX*1)),	GetBrightnessValue, PROP_NUM(0)},
};

DrawGaugeType code DrawQSBrightnessGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (QuickStartTxtYPos+QuickStart_Brightness_IDX*1)),GetrgbBrightnessValue},
};

DrawNumberType code DrawQSBrightnessNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (QuickStartTxtYPos+QuickStart_Brightness_IDX*1)),	GetrgbBrightnessValue, PROP_NUM(1)},
};

DrawGaugeType code DrawQSContrastGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (QuickStartTxtYPos+QuickStart_Contrast_IDX*1)),GetContrastValue},
};

DrawNumberType code DrawQSContrastNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (QuickStartTxtYPos+QuickStart_Contrast_IDX*1)),	GetContrastValue, PROP_NUM(2)},
};

RadioTextType code QSPressetModeRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_Preset_IDX*1)),  TxtQuickPresetModeStatus},
};
DrawRadioGroupType code DrawQSPressetModeRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, QSPressetModeRadioText}
};

RadioTextType code QSAspectRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_Aspect_IDX*1)),  TxtQuickAspectRatioStatus},
};
DrawRadioGroupType code DrawQSAspectRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, QSAspectRadioText}
};

RadioTextType code QSSourceRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_Source_IDX*1)),  TxtQuickSourceStatus},
};
DrawRadioGroupType code DrawQSSourceRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, QSSourceRadioText}
};

RadioTextType code QSBlueLightRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_BlueLight_IDX*1)),  TxtQuickBlueLightStatus},
};
DrawRadioGroupType code DrawQSBlueLightGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, QSBlueLightRadioText}
};
#if Free_Sync_Enable
RadioTextType code QSFreeSyncRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_FreeSync_IDX*1)),  TxtQuickFreeSyncOnOff},
};
DrawRadioGroupType code DrawQSFreeSyncGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, QSFreeSyncRadioText}
};
#endif
///////////////////////////////////////////////////////////

/*
DrawGaugeType code DrawQuickStartGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {0, 0, 10, COORD(Gauge_XPos, (QuickStartTxtYPos+QuickStart_BackLight_IDX*1)),GetBrightnessValue},
  {0, 0, 10, COORD(Gauge_XPos, (QuickStartTxtYPos+QuickStart_Brightness_IDX*1)),GetrgbBrightnessValue},
  {BAR_END, 0, 10, COORD(Gauge_XPos, (QuickStartTxtYPos+QuickStart_Contrast_IDX*1)),GetContrastValue},

};
*/
/*
DrawNumberType code DrawQuickStartNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW,	MONO_COLOR(CP_FG_COLOR_L2,CP_WHITE), COORD(Number_XPos, (QuickStartTxtYPos+QuickStart_BackLight_IDX*1)),	GetBrightnessValue, PROP_NUM(0)},
  {NUM_DRAW_ARROW,	MONO_COLOR(CP_FG_COLOR_L2,CP_WHITE), COORD(Number_XPos, (QuickStartTxtYPos+QuickStart_Brightness_IDX*1)),	GetrgbBrightnessValue, PROP_NUM(1)},
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_FG_COLOR_L2,CP_WHITE), COORD(Number_XPos, (QuickStartTxtYPos+QuickStart_Contrast_IDX*1)),	GetContrastValue, PROP_NUM(2)},
};


RadioTextType code QuickStartMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_Preset_IDX*1)),  TxtQuickPresetModeStatus},
  {TXT_OVER_256,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_Aspect_IDX*1)),  TxtQuickAspectRatioStatus},
  {TXT_OVER_256,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_BlueLight_IDX*1)),  TxtBlueLightStatus},
  {TXT_OVER_256,  COORD(GroupText_XPos, (QuickStartTxtYPos+QuickStart_FreeSync_IDX*1)),  TxtQuickFreeSyncOnOff},

  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawQuickStartMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, QuickStartMenuRadioText}
};
*/

MenuItemType code QuickStartMenuItems[]=
{
  {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_BackLight_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtQuickBackLight, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawQSBackLightNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_SELECTABLE|MIB_BRIGHTNESS_DISABLE // u16Flags
  },
  
  {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_BackLight_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustBrightness,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawQSBackLightNumber,	// DrawNumberType
      DrawQSBackLightGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_BRIGHTNESS_DISABLE // u16Flags
  },

  {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Brightness_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtQuickBrightness, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawQSBrightnessNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawColorSettingsIcon//DrawIconType
    },
    MIB_SELECTABLE|MIB_BRIGHTNESS_DISABLE// u16Flags
  },
   {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Brightness_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustrgbBrightness,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawQSBrightnessNumber,	// DrawNumberType
      DrawQSBrightnessGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_BRIGHTNESS_DISABLE // u16Flags
  },

 {
   COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Contrast_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_QUICKSTART, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtQuickContrast, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { DrawQSContrastNumber, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawDisplaySettingsIcon//DrawIconType
   },
   MIB_SELECTABLE|MIB_BRIGHTNESS_DISABLE// u16Flags
 },
   {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Contrast_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustContrast,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawQSContrastNumber,	// DrawNumberType
      DrawQSContrastGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_BRIGHTNESS_DISABLE // u16Flags
  },

 {
   COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Preset_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_QUICKSTART, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtQuickPresetMode, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawQSPressetModeRadioGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },
   {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Preset_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterReverseNaviKeyEvent,
    { AdjustColorPresetMode,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawQSPressetModeRadioGroup,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

 {
   COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Aspect_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_QUICKSTART, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtQuickAspectRatio, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawQSAspectRadioGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },
   {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Aspect_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustWideModeValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawQSAspectRadioGroup,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

 {
   COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Source_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_QUICKSTART, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtQuickSource, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawQSSourceRadioGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },
   {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_Source_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterFuncNaviKeyEvent,
    { AdjustInputValue,	// AdjustFunction
      SetInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawQSSourceRadioGroup,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },


 {
   COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_BlueLight_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_QUICKSTART, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtQuickBlueLight, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawQSBlueLightGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },
   {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_BlueLight_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLowbluray,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawQSBlueLightGroup,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
  #if Free_Sync_Enable
 {
   COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_FreeSync_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_QUICKSTART, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtQuickFreeSync, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawQSFreeSyncGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE|MIB_FREESYNC_DISABLE// u16Flags
 },

    {
    COORD(SubTxtXPos,(QuickStartTxtYPos+QuickStart_FreeSync_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustFreeSync,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawQSFreeSyncGroup,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_FREESYNC_DISABLE // u16Flags
  },
 #endif

    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      NULL,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};




RadioTextType code DCRDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtDCRDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtDCRDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawDCRDesccRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, DCRDescRadioText}
};

RadioTextType code SharpnessDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtSharpnessDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtSharpnessDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawSharpnessDesccRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, SharpnessDescRadioText}
};


RadioTextType code PictureMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
//  {TXT_OVER_256,  COORD(1, 49),  TxtDescription},
  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawPictureMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, PictureMenuRadioText}
};


DrawGaugeType code DrawPicBackLightGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (PictureTxtYPos+Picture_BackLight_IDX*1)),GetBrightnessValue},
};

DrawNumberType code DrawPicBackLightNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (PictureTxtYPos+Picture_BackLight_IDX*1)),	GetBrightnessValue, PROP_NUM(0)},
};

DrawGaugeType code DrawPicBrightnessGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (PictureTxtYPos+Picture_Brightness_IDX*1)),GetrgbBrightnessValue},
};

DrawNumberType code DrawPicBrightnessNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (PictureTxtYPos+Picture_Brightness_IDX*1)),	GetrgbBrightnessValue, PROP_NUM(1)},
};

DrawGaugeType code DrawPicContrastGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (PictureTxtYPos+Picture_Contrast_IDX*1)),GetContrastValue},
};

DrawNumberType code DrawPicContrastNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (PictureTxtYPos+Picture_Contrast_IDX*1)),	GetContrastValue, PROP_NUM(2)},
};

RadioTextType code PicPressetModeRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (PictureTxtYPos+Picture_Preset_IDX*1)),  TxtPicturePresetModeStatus},
};
DrawRadioGroupType code DrawPicPressetModeRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, PicPressetModeRadioText}
};

RadioTextType code PicDCRRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (PictureTxtYPos+Picture_DCR_IDX*1)),  TxtPictureDCRStatus},
};
DrawRadioGroupType code DrawPicDCRRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, PicDCRRadioText}
};

RadioTextType code PicAspectRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (PictureTxtYPos+Picture_Aspect_IDX*1)),  TxtPictureAspectRatioStatus},
};
DrawRadioGroupType code DrawPicAspectRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, PicAspectRadioText}
};

DrawGaugeType code DrawPicSharpnessGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (PictureTxtYPos+Picture_Sharpness_IDX*1)),GetSharpnessValue},
};

DrawNumberType code DrawPicSharpnessNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (PictureTxtYPos+Picture_Sharpness_IDX*1)),	GetSharpnessValue, PROP_NUM(3)},
};



MenuItemType code PictureMenuItems[]=
{
  {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_BackLight_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtPictureBackLight, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawPicBackLightNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_BRIGHTNESS_DISABLE // u16Flags
  },

  {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_BackLight_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustBrightness,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawPicBackLightNumber,	// DrawNumberType
      DrawPicBackLightGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_BRIGHTNESS_DISABLE // u16Flags
  },
	
  {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_Brightness_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtPictureBrightness, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawPicBrightnessNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawColorSettingsIcon//DrawIconType
    },
    MIB_SELECTABLE|MIB_BRIGHTNESS_DISABLE// u16Flags
  },
   {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_Brightness_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustrgbBrightness,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawPicBrightnessNumber,	// DrawNumberType
      DrawPicBrightnessGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_BRIGHTNESS_DISABLE // u16Flags
  },
 {
   COORD(SubTxtXPos,(PictureTxtYPos+Picture_Contrast_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtPictureContrast, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { DrawPicContrastNumber, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawDisplaySettingsIcon//DrawIconType
   },
   MIB_SELECTABLE|MIB_BRIGHTNESS_DISABLE// u16Flags
 },
   {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_Contrast_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustContrast,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawPicContrastNumber,	// DrawNumberType
      DrawPicContrastGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_BRIGHTNESS_DISABLE // u16Flags
  },

 {
   COORD(SubTxtXPos,(PictureTxtYPos+Picture_Preset_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtPicturePresetMode, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawPicPressetModeRadioGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },
   {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_Preset_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterReverseNaviKeyEvent,
    { AdjustColorPresetMode,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawPicPressetModeRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

 {
   COORD(SubTxtXPos,(PictureTxtYPos+Picture_DCR_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtPictureDCR, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawPicDCRRadioGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE|MIB_DCR_ENABLE// u16Flags
 },
    {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_DCR_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustMenuTempValueFlipFlop,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawPicDCRRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_DCR_ENABLE // u16Flags
  },
 {
   COORD(SubTxtXPos,(PictureTxtYPos+Picture_Aspect_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtPictureAspectRatio, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	DrawPicAspectRadioGroup, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },
   {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_Aspect_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_QUICKSTART, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustWideModeValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawPicAspectRadioGroup,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
 {
   COORD(SubTxtXPos,(PictureTxtYPos+Picture_Sharpness_IDX*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtPictureSharpness, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { DrawPicSharpnessNumber, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE// u16Flags
 },

  {
    COORD(SubTxtXPos,(PictureTxtYPos+Picture_Sharpness_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustSharpness,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawPicSharpnessNumber,	// DrawNumberType
      DrawPicSharpnessGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
///////////////////////////////////////////////////////////////////
#if ENABLE_VGA_INPUT
 {
   COORD(SubTxtXPos,(Picture_VGA_TxtYPos+(Picture_AutoAdjust_IDX-Picture_AutoAdjust_IDX)*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtAuto_Adjust, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE|MIB_DVI_DISABLE// u16Flags
 },
 /*
 {
   COORD(SubTxtXPos,(Picture_VGA_TxtYPos+(Picture_AutoColor_IDX-Picture_AutoAdjust_IDX)*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtAuto_Color, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE|MIB_DVI_DISABLE// u16Flags
 },
 */
 {
   COORD(SubTxtXPos,(Picture_VGA_TxtYPos+(Picture_HPosition_IDX-Picture_AutoAdjust_IDX)*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtH_Position, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE|MIB_DVI_DISABLE// u16Flags
 },
 {
   COORD(SubTxtXPos,(Picture_VGA_TxtYPos+(Picture_VPosition_IDX-Picture_AutoAdjust_IDX)*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtV_Position, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE|MIB_DVI_DISABLE// u16Flags
 },
 {
   COORD(SubTxtXPos,(Picture_VGA_TxtYPos+(Picture_Clock_IDX-Picture_AutoAdjust_IDX)*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtClock, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE|MIB_DVI_DISABLE// u16Flags
 },   
  {
   COORD(SubTxtXPos,(Picture_VGA_TxtYPos+(Picture_Phase_IDX-Picture_AutoAdjust_IDX)*1)), // u8XPos, u8YPos;
   MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
   MENU_PICTURE, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtPhase, // DisplayText;
   NaviAdjusterKeyEvent,
   { NULL, // AdjustFunction
 	NULL		  // ExecFunction
   },// *stDisplayValue;
   { NULL, // DrawNumberType
 	NULL, // DrawGaugeType
 	NULL, // DrawRadioGroupType
 	NULL, //DrawOtherSettingsIcon  //DrawIconType
   },
   MIB_SELECTABLE|MIB_DVI_DISABLE// u16Flags
 },   
#endif
    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      DrawPictureMenuRadioGroup,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};




RadioTextType code ColorTempDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtColorTempDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtColorTempDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawColorTempDesccRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, ColorTempDescRadioText}
};

RadioTextType code ColorTempAdvancedDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtColorTempAdvancedDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtColorTempAdvancedDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawColorTempAdvancedDesccRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, ColorTempAdvancedDescRadioText}
};


RadioTextType code GammaDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtGammaDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtGammaDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawGammaDesccRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, GammaDescRadioText}
};
RadioTextType code TintDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtTintDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtTintDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawTintDesccRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, TintDescRadioText}
};
RadioTextType code SaturationDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtSaturationDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtSaturationDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawSaturationDesccRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, SaturationDescRadioText}
};

RadioTextType code ColTemperatureRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (ColorTxtYPos+Color_Temperature_IDX*1)),  TxtColorTemperatureStatus},
};
DrawRadioGroupType code DrawColTemperatureRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, ColTemperatureRadioText}
};

RadioTextType code ColGammaRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (ColorTxtYPos+4+Color_Gamma_IDX*1)),  TxtColorGammaStatus},
};
DrawRadioGroupType code DrawColGammaRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, ColGammaRadioText}
};

RadioTextType code ColBlueLightRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (ColorTxtYPos+4+Color_BlueLight_IDX*1)),  TxtColorBlueLightStatus},
};
DrawRadioGroupType code DrawColBlueLightRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, ColBlueLightRadioText}
};


DrawGaugeType code DrawColTintGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorTxtYPos+4+Color_Tint_IDX*1)),GetHueValue},
};

DrawNumberType code DrawColTintNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorTxtYPos+4+Color_Tint_IDX*1)),	GetHueValue, PROP_NUM(0)},
};

DrawGaugeType code DrawColSaturationGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorTxtYPos+4+Color_Saturation_IDX*1)),GetSaturationValue},
};

DrawNumberType code DrawColSaturationNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorTxtYPos+4+Color_Saturation_IDX*1)),	GetSaturationValue, PROP_NUM(1)},
};





RadioTextType code ColorMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  //{TXT_OVER_256,  COORD(1, 49),  TxtDescription},
  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawColorMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, ColorMenuRadioText}
};


MenuItemType code ColorMenuItems[]=
{
  {
    COORD(SubTxtXPos,(ColorTxtYPos+Color_Temperature_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorTemperature, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawColTemperatureRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

   {
    COORD(SubTxtXPos,(ColorTxtYPos+Color_Temperature_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustColorTemperature,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawColTemperatureRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },


  {
    COORD(SubTxtXPos,(ColorTxtYPos+4+Color_Gamma_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorGamma, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawColGammaRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
   {
    COORD(SubTxtXPos,(ColorTxtYPos+4+Color_Gamma_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterReverseNaviKeyEvent,
    { AdjustGammaValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawColGammaRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
  //Hue
  {
    COORD(SubTxtXPos,(ColorTxtYPos+4+Color_Tint_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorTint, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawColTintNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_USER_DISABLE // u16Flags
  },
  {
    COORD(SubTxtXPos,(ColorTxtYPos+4+Color_Tint_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustVideoHue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawColTintNumber,	// DrawNumberType
      DrawColTintGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_USER_DISABLE // u16Flags
  },

  {
    COORD(SubTxtXPos,(ColorTxtYPos+4+Color_Saturation_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorSaturation, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawColSaturationNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_USER_DISABLE // u16Flags
  },

  {
    COORD(SubTxtXPos,(ColorTxtYPos+4+Color_Saturation_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustVideoSaturation,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawColSaturationNumber,	// DrawNumberType
      DrawColSaturationGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_USER_DISABLE // u16Flags
  },

  {
    COORD(SubTxtXPos,(ColorTxtYPos+4+Color_BlueLight_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorBlueLight, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawColBlueLightRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
     {
    COORD(SubTxtXPos,(ColorTxtYPos+4+Color_BlueLight_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLowbluray,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawColBlueLightRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      NULL,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};



DrawGaugeType code DrawRedGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorUserTxtYPos+User_Red_ItemIDX*1)),GetUserRedColorValue},
};
DrawNumberType code DrawRedNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorUserTxtYPos+User_Red_ItemIDX*1)),	GetUserRedColorValue, PROP_NUM(2)},
};
DrawGaugeType code DrawGreenGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorUserTxtYPos+User_Green_ItemIDX*1)),GetUserGreenColorValue},
};
DrawNumberType code DrawGreenNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorUserTxtYPos+User_Green_ItemIDX*1)),	GetUserGreenColorValue, PROP_NUM(3)},
};
DrawGaugeType code DrawBlueGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorUserTxtYPos+User_Blue_ItemIDX*1)),GetUserBlueColorValue},
};
DrawNumberType code DrawBlueNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorUserTxtYPos+User_Blue_ItemIDX*1)),	GetUserBlueColorValue, PROP_NUM(4)},
};


RadioTextType code UserRGBCMYRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_RealText|TXT_OVER_256|TXT_END,  COORD(GroupText_XPos,(ColorUserTxtYPos+User_RGBCMY_ItemIDX*1)),  TxtSystemLedModeIcon},
};
DrawRadioGroupType code DrawUserRGBCMYRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, UserRGBCMYRadioText}
};



MenuItemType code UserMenuItems[]=
{
  //Red
  {
    COORD(SubUserTxtXPos,(ColorUserTxtYPos+User_Red_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorRed, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawRedNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubUserTxtXPos,(ColorUserTxtYPos+User_Red_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustUserRedColor,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawRedNumber,	// DrawNumberType
      DrawRedGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },
  //Green
  {
    COORD(SubUserTxtXPos,(ColorUserTxtYPos+User_Green_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorGreen, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawGreenNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubUserTxtXPos,(ColorUserTxtYPos+User_Green_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustUserGreenColor,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawGreenNumber,	// DrawNumberType
      DrawGreenGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },
//Blue
  {
    COORD(SubUserTxtXPos,(ColorUserTxtYPos+User_Blue_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorBlue, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawBlueNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubUserTxtXPos,(ColorUserTxtYPos+User_Blue_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustUserBlueColor,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawBlueNumber,	// DrawNumberType
      DrawBlueGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },


  {
    COORD(SubUserTxtXPos,(ColorUserTxtYPos+User_RGBCMY_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ADVANCED, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtAdvanced, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawUserRGBCMYRadioGroup,	// DrawRadioGroupType
      NULL,   //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },

{
    COORD(SubUserTxtXPos,(ColorUserTxtYPos+User_RGBCMY_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },



    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      NULL,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};



DrawGaugeType code DrawAdvancedRGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorAdvancedTxtYPos+Advanced_Red_ItemIDX*1)),GetCustomColorValue_R},
};
DrawNumberType code DrawAdvancedRNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorAdvancedTxtYPos+Advanced_Red_ItemIDX*1)),	GetCustomColorValue_R, PROP_NUM(5)},
};
DrawGaugeType code DrawAdvancedGGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorAdvancedTxtYPos+Advanced_Green_ItemIDX*1)),GetCustomColorValue_G},
};
DrawNumberType code DrawAdvancedGNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorAdvancedTxtYPos+Advanced_Green_ItemIDX*1)),	GetCustomColorValue_G, PROP_NUM(6)},
};
DrawGaugeType code DrawAdvancedBGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorAdvancedTxtYPos+Advanced_Blue_ItemIDX*1)),GetCustomColorValue_B},
};
DrawNumberType code DrawAdvancedBNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorAdvancedTxtYPos+Advanced_Blue_ItemIDX*1)),	GetCustomColorValue_B, PROP_NUM(7)},
};
DrawGaugeType code DrawAdvancedCGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorAdvancedTxtYPos+Advanced_Cyan_ItemIDX*1)),GetCustomColorValue_C},
};
DrawNumberType code DrawAdvancedCNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorAdvancedTxtYPos+Advanced_Cyan_ItemIDX*1)),	GetCustomColorValue_C, PROP_NUM(8)},
};
DrawGaugeType code DrawAdvancedMGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorAdvancedTxtYPos+Advanced_Magenta_ItemIDX*1)),GetCustomColorValue_M},
};
DrawNumberType code DrawAdvancedMNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorAdvancedTxtYPos+Advanced_Magenta_ItemIDX*1)),	GetCustomColorValue_M, PROP_NUM(9)},
};
DrawGaugeType code DrawAdvancedYGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (ColorAdvancedTxtYPos+Advanced_Yellow_ItemIDX*1)),GetCustomColorValue_Y},
};
DrawNumberType code DrawAdvancedYNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (ColorAdvancedTxtYPos+Advanced_Yellow_ItemIDX*1)),	GetCustomColorValue_Y, PROP_NUM(10)},
};



MenuItemType code AdvancedMenuItems[]=
{
  //R
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Red_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorRed, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedRNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Red_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustCustomColor_R,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedRNumber,	// DrawNumberType
      DrawAdvancedRGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },
  //Green
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Green_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorGreen, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedGNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Green_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustCustomColor_G,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedGNumber,	// DrawNumberType
      DrawAdvancedGGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },
//Blue
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Blue_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorBlue, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedBNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Blue_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustCustomColor_B,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedBNumber,	// DrawNumberType
      DrawAdvancedBGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },

//Cyan
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Cyan_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorCyan, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedCNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Cyan_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustCustomColor_C,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedCNumber,	// DrawNumberType
      DrawAdvancedCGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },

//Magenta
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Magenta_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorMagenta, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedMNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Magenta_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustCustomColor_M,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedMNumber,	// DrawNumberType
      DrawAdvancedMGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },

//Yellow
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Yellow_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtColorYellow, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedYNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_ColorTemp_DISABLE // u16Flags
  },
  {
    COORD(SubAdvancedTxtXPos,(ColorAdvancedTxtYPos+Advanced_Yellow_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_COLOR, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustCustomColor_Y,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawAdvancedYNumber,	// DrawNumberType
      DrawAdvancedYGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_ColorTemp_DISABLE // u16Flags
  },


	
    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      NULL,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};





RadioTextType code ODDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtODDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtODDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawODDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, ODDescRadioText}
};
RadioTextType code SleepModeDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtSleepModeDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtSleepModeDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawSleepModeDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, SleepModeDescRadioText}
};

RadioTextType code LedModeAllDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxLedModeAllDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxLedModeAllDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawLedModeAllDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, LedModeAllDescRadioText}
};

RadioTextType code LedModeDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxLedModeDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxLedModeDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawLedModeDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, LedModeDescRadioText}
};
RadioTextType code LedColorDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxLedColorDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxLedColorDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawLedColorDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, LedColorDescRadioText}
};
RadioTextType code LedBrightnessDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxLedBrightnessDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxLedBrightnessDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawLedBrightnessDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, LedBrightnessDescRadioText}
};




RadioTextType code LanguageDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtLanguageDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtLanguageDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawLanguageDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, LanguageDescRadioText}
};
RadioTextType code OSDHDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtOSDHDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtOSDHDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawOSDHDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, OSDHDescRadioText}
};
RadioTextType code OSDVDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtOSDVDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtOSDVDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawOSDVDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, OSDVDescRadioText}
};
RadioTextType code OSDTimerDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtOSDTimerDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtOSDTimerDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawOSDTimerDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, OSDTimerDescRadioText}
};
RadioTextType code OSDTransDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxtOSDTransDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxtOSDTransDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawOSDTransDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, OSDTransDescRadioText}
};
RadioTextType code VolumeDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxVolumeDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxVolumeDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawVolumeDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, VolumeDescRadioText}
};
RadioTextType code ResetDescRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,  COORD(1, (Main_Description_YPos+3)),  TxResetDes01},
  {0,  COORD(1, (Main_Description_YPos+4)),  TxResetDes02},
 
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawResetDescRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, ResetDescRadioText}
};


RadioTextType code SysODRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_OverDrive_IDX*1)),  TxtSystemOverDriveStatus},
};
DrawRadioGroupType code DrawSysODRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysODRadioText}
};
#if Free_Sync_Enable
RadioTextType code SysFreeSyncRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_FreeSync_IDX*1)),  TxtSystemFreeSyncStatus},
};
DrawRadioGroupType code DrawSysFreeSyncRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysFreeSyncRadioText}
};
#endif
RadioTextType code SysSleepModeRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_SleepMode_IDX*1)),  TxtSystemSleepModeStatus},
};
DrawRadioGroupType code DrawSysSleepModeRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysSleepModeRadioText}
};
#if LED_HOT_Menu_Func
RadioTextType code SysLedModeRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_LedMode_IDX*1)),  TxtSystemLedModeIndexStatus},
};
DrawRadioGroupType code DrawSysLedModeRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysLedModeRadioText}
};
#endif
RadioTextType code SysLanguageRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_Language_IDX*1)),  TxtSystemLanguageStatus},
};
DrawRadioGroupType code DrawSysLanguageRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysLanguageRadioText}
};

RadioTextType code SysResetRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_Reset_IDX*1)),  TxtSystemResetStatus},
};
DrawRadioGroupType code DrawSysResetRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysResetRadioText}
};

RadioTextType code SysTransRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_512|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_OSDTransparencia_IDX*1)),  TxtSystemTransStatus},
};
DrawRadioGroupType code DrawSysTransRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysTransRadioText}
};

DrawGaugeType code DrawOSDHPositionGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemTxtYPos+System_OSDHPosition_IDX*1)),GetOSDHPositionValue},
};
DrawNumberType code DrawOSDHPositionNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemTxtYPos+System_OSDHPosition_IDX*1)),	GetOSDHPositionValue, PROP_NUM(0)},
};
#if Enable_OSD_Vposition
DrawGaugeType code DrawOSDVPositionGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemTxtYPos+System_OSDVPosition_IDX*1)),GetOSDVPositionValue},
};
DrawNumberType code DrawOSDVPositionNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemTxtYPos+System_OSDVPosition_IDX*1)),	GetOSDVPositionValue, PROP_NUM(1)},
};
#endif
DrawGaugeType code DrawOSDTimerGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemTxtYPos+System_OSDTimer_IDX*1)),GetOSDTimerValue100},
};
DrawNumberType code DrawOSDTimerNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemTxtYPos+System_OSDTimer_IDX*1)),	GetOSDTimerValue, PROP_NUM(2)},
};

/*
DrawGaugeType code DrawOSDTransparencyGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemTxtYPos+System_OSDTransparencia_IDX*1)),GetTransparencyGauge},
};
DrawNumberType code DrawOSDTransparencyNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemTxtYPos+System_OSDTransparencia_IDX*1)),	GetTransparencyValue, PROP_NUM(3)},
};
*/
#if ENABLE_AUDIO_SETTINGS_MENU
DrawGaugeType code DrawVolumeGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemTxtYPos+System_Volume_IDX*1)),GetVolumeValue},
};
DrawNumberType code DrawVolumeNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemTxtYPos+System_Volume_IDX*1)),	GetVolumeValue, PROP_NUM(4)},
};
#endif


RadioTextType code SystemMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  //{TXT_OVER_256,  COORD(1, 49),  TxtDescription},
  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawSystemMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, SystemMenuRadioText}
};


MenuItemType code SystemMenuItems[]=
{
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OverDrive_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOverDrive, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysODRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_OverDrive_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustResponseTimeValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysODRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
#if Free_Sync_Enable
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_FreeSync_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemFreeSync, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysFreeSyncRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_FREESYNC_DISABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_FreeSync_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustFreeSync,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysFreeSyncRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_FREESYNC_DISABLE // u16Flags
  },
#endif
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_SleepMode_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemSleepMode, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysSleepModeRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_SleepMode_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustSleepModeValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysSleepModeRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

#if LED_HOT_Menu_Func
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_LedMode_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemLedModeText, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysLedModeRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_LedMode_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLedModeIndexValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysLedModeRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
#endif



  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_Language_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemLanguage, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysLanguageRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
 {
    COORD(SubTxtXPos,(SystemTxtYPos+System_Language_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLanguageValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysLanguageRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OSDHPosition_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOSDHPosition, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDHPositionNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OSDHPosition_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustOSDHPosition,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDHPositionNumber,	// DrawNumberType
      DrawOSDHPositionGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
  #if Enable_OSD_Vposition
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OSDVPosition_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOSDVPosition, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDVPositionNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OSDVPosition_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustOSDVPosition,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDVPositionNumber,	// DrawNumberType
      DrawOSDVPositionGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
 #endif 
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OSDTimer_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOSDTimer, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDTimerNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OSDTimer_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustOSDTimer,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDTimerNumber,	// DrawNumberType
      DrawOSDTimerGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OSDTransparencia_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOSDTransparency, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysTransRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OSDTransparencia_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustTransparency,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysTransRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
#if ENABLE_AUDIO_SETTINGS_MENU
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_Volume_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemVolume, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawVolumeNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_Volume_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustVolume,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawVolumeNumber,	// DrawNumberType
      DrawVolumeGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
#endif

  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_Reset_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEM, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemReset, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_Reset_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    {AdjustResetValue ,	// AdjustFunction
      ResetAllSetting			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysResetRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

	
    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      DrawSystemMenuRadioGroup,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};


#if LED_HOT_Menu_Func
RadioTextType code SysODLEDUSERRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_OverDrive_IDX*1)),  TxtSystemOverDriveStatus},
};
DrawRadioGroupType code DrawSysODLEDUSERRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysODLEDUSERRadioText}
};
#if Free_Sync_Enable
RadioTextType code SysFreeSyncLEDUSERRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_FreeSync_IDX*1)),  TxtSystemFreeSyncStatus},
};
DrawRadioGroupType code DrawSysFreeSyncLEDUSERRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysFreeSyncLEDUSERRadioText}
};
#endif
RadioTextType code SysSleepModeLEDUSERRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_SleepMode_IDX*1)),  TxtSystemSleepModeStatus},
};
DrawRadioGroupType code DrawSysSleepModeLEDUSERRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysSleepModeLEDUSERRadioText}
};

RadioTextType code SysLedModeLEDUSERRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemTxtYPos+System_LedMode_IDX*1)),  TxtSystemLedModeIndexStatus},
};
DrawRadioGroupType code DrawSysLedModeLEDUSERRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysLedModeLEDUSERRadioText}
};

RadioTextType code SysLanguageLEDUSERRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemLedModeLEDUSERTxtYPos+System_Language_IDX*1)),  TxtSystemLanguageStatus},
};
DrawRadioGroupType code DrawSysLanguageLEDUSERRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysLanguageLEDUSERRadioText}
};

RadioTextType code SysResetLEDUSERRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemLedModeLEDUSERTxtYPos+System_Reset_IDX*1)),  TxtSystemResetStatus},
};
DrawRadioGroupType code DrawSysResetLEDUSERRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysResetLEDUSERRadioText}
};

RadioTextType code SysTransLEDUSERRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_512|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemLedModeLEDUSERTxtYPos+System_OSDTransparencia_IDX*1)),  TxtSystemTransStatus},
};
DrawRadioGroupType code DrawSysTransLEDUSERRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, SysTransLEDUSERRadioText}
};

DrawGaugeType code DrawOSDHPositionLEDUSERGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemLedModeLEDUSERTxtYPos+System_OSDHPosition_IDX*1)),GetOSDHPositionValue},
};
DrawNumberType code DrawOSDHPositionLEDUSERNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemLedModeLEDUSERTxtYPos+System_OSDHPosition_IDX*1)),	GetOSDHPositionValue, PROP_NUM(0)},
};
#if Enable_OSD_Vposition
DrawGaugeType code DrawOSDVPositionLEDUSERGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemLedModeLEDUSERTxtYPos+System_OSDVPosition_IDX*1)),GetOSDVPositionValue},
};
DrawNumberType code DrawOSDVPositionLEDUSERNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemLedModeLEDUSERTxtYPos+System_OSDVPosition_IDX*1)),	GetOSDVPositionValue, PROP_NUM(1)},
};
#endif
DrawGaugeType code DrawOSDTimerLEDUSERGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemLedModeLEDUSERTxtYPos+System_OSDTimer_IDX*1)),GetOSDTimerValue100},
};
DrawNumberType code DrawOSDTimerLEDUSERNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemLedModeLEDUSERTxtYPos+System_OSDTimer_IDX*1)),	GetOSDTimerValue, PROP_NUM(2)},
};

/*
DrawGaugeType code DrawOSDTransparencyGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemTxtYPos+System_OSDTransparencia_IDX*1)),GetTransparencyGauge},
};
DrawNumberType code DrawOSDTransparencyNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemTxtYPos+System_OSDTransparencia_IDX*1)),	GetTransparencyValue, PROP_NUM(3)},
};
*/
#if ENABLE_AUDIO_SETTINGS_MENU
DrawGaugeType code DrawVolumeLEDUSERGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, Gauge_Height, COORD(Gauge_XPos, (SystemLedModeLEDUSERTxtYPos+System_Volume_IDX*1)),GetVolumeValue},
};
DrawNumberType code DrawVolumeLEDUSERNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_DRAW_ARROW|NUM_END,	MONO_COLOR(CP_WHITE ,CP_BK_COLOR_H), COORD(Number_XPos, (SystemLedModeLEDUSERTxtYPos+System_Volume_IDX*1)),	GetVolumeValue, PROP_NUM(4)},
};
#endif


RadioTextType code SystemMenuLEDUSERRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  //{TXT_OVER_256,  COORD(1, 49),  TxtDescription},
  
  {TXT_END,	      COORD(1,19),	NULL}
};
DrawRadioGroupType code DrawSystemMenuLEDUSERRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), NULL, SystemMenuLEDUSERRadioText}
};


MenuItemType code SystemMenuLEDUSERItems[]=
{
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_OverDrive_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOverDrive, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysODLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_OverDrive_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustResponseTimeValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysODLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
#if Free_Sync_Enable
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_FreeSync_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemFreeSync, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysFreeSyncLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE|MIB_FREESYNC_DISABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_FreeSync_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustFreeSync,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysFreeSyncLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_FREESYNC_DISABLE // u16Flags
  },
#endif
  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_SleepMode_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemSleepMode, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysSleepModeLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_SleepMode_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustSleepModeValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysSleepModeLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

  {
    COORD(SubTxtXPos,(SystemTxtYPos+System_LedMode_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemLedModeText, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysLedModeLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemTxtYPos+System_LedMode_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLedModeIndexValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysLedModeLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },




  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_Language_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemLanguage, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysLanguageLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
 {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_Language_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLanguageValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysLanguageLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_OSDHPosition_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOSDHPosition, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDHPositionLEDUSERNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_OSDHPosition_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustOSDHPosition,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDHPositionLEDUSERNumber,	// DrawNumberType
      DrawOSDHPositionLEDUSERGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
  #if Enable_OSD_Vposition
  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_OSDVPosition_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOSDVPosition, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDVPositionLEDUSERNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_OSDVPosition_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustOSDVPosition,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDVPositionLEDUSERNumber,	// DrawNumberType
      DrawOSDVPositionLEDUSERGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
 #endif 
  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_OSDTimer_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOSDTimer, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDTimerLEDUSERNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  
  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_OSDTimer_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustOSDTimer,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawOSDTimerLEDUSERNumber,	// DrawNumberType
      DrawOSDTimerLEDUSERGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_OSDTransparencia_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemOSDTransparency, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysTransLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_OSDTransparencia_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustTransparency,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysTransLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
#if ENABLE_AUDIO_SETTINGS_MENU
  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_Volume_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemVolume, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawVolumeLEDUSERNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_Volume_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustVolume,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { DrawVolumeLEDUSERNumber,	// DrawNumberType
      DrawVolumeLEDUSERGauge,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },
#endif

  {
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_Reset_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemReset, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubTxtXPos,(SystemLedModeLEDUSERTxtYPos+System_Reset_IDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    {AdjustResetValue ,	// AdjustFunction
      ResetAllSetting			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawSysResetLEDUSERRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

	
    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      DrawSystemMenuLEDUSERRadioGroup,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};

#endif


#if LED_HOT_Menu_Func

RadioTextType code LedColorRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemLedModeTxtYPos+Led_Color_ItemIDX*1)),  TxtSystemLedColorStatus},
};
DrawRadioGroupType code DrawLedColorRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, LedColorRadioText}
};

RadioTextType code LedBrightnessRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemLedModeTxtYPos+Led_Brightness_ItemIDX*1)),  TxtSystemLedBrightnessStatus},
};
DrawRadioGroupType code DrawLedBrightnessRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, LedBrightnessRadioText}
};

RadioTextType code LedModeRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_OVER_256|TXT_CENTER|TXT_ARROW|TXT_END,  COORD(GroupText_XPos, (SystemLedModeTxtYPos+Led_Mode_ItemIDX*1)),  TxtSystemLedModeStatus},
};
DrawRadioGroupType code DrawLedModeRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL), NULL, LedModeRadioText}
};


MenuItemType code LedModeMenuItems[]=
{
  {
    COORD(SubLedModeTxtXPos,(SystemLedModeTxtYPos+Led_Mode_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemLedMode, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedModeRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubLedModeTxtXPos,(SystemLedModeTxtYPos+Led_Mode_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLedModeValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedModeRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

  {
    COORD(SubLedModeTxtXPos,(SystemLedModeTxtYPos+Led_Color_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemLedColor, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedColorRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubLedModeTxtXPos,(SystemLedModeTxtYPos+Led_Color_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLedColorValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedColorRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

  {
    COORD(SubLedModeTxtXPos,(SystemLedModeTxtYPos+Led_Brightness_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    TxtSystemLedBrightness, // DisplayText;
    NaviAdjuster1KeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedBrightnessRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

{
    COORD(SubLedModeTxtXPos,(SystemLedModeTxtYPos+Led_Brightness_ItemIDX*1)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_SYSTEMLLEDUSER, //u8NextMenuPage;
    TXT_OVER_256,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterNaviKeyEvent,
    { AdjustLedStrengthValue,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedBrightnessRadioGroup,	// DrawRadioGroupType
      NULL, //DrawBriteAndContrastIcon,//DrawBriteContIcon  //DrawIconType
    },
    MIB_ADJUST_ITEM // u16Flags
  },

    //Menu Header
  {
    COORD(2,1), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_SEL), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,//TXT_OVER_256,// u8TXTFlags;
    NULL,//TxtMenu, // DisplayText; //FY12_MENU
    NaviKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      NULL,   // DrawRadioGroupType
      NULL //DrawInputIconIcon //DrawIconType  //Steven110519
    },
    0// u16Flags
  },


};
#endif

#if USB_Updata_FW_Enable
RadioTextType code USBUpdataFWRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0|TXT_END,  COORD(MM_XSTART_L2+1,  3+(SOURCE_USB_UPDATA*2)),	TxtUSBUpdataFWStatus},
};
DrawRadioGroupType code DrawUSBUpdataFWRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
  {MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L2), MONO_COLOR(CP_ADJ_COLOR_L2, CP_BK_COLOR_L2), GetUSBUpdataFWStatus,USBUpdataFWRadioText}
};
#endif

#if ENABLE_MULTI_INPUT

MenuItemType code InputSourceMenuItems[]=  //Steven110519
{
#if Enable_AutoInput_Menu
 // AutoSelect
  {COORD(0,  3+(SOURCE_AUTO_SWITCH_IDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL//DrawVGAMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_VGA_INPUT
 // VGA
  {COORD(0,  3+(SOURCE_VGA_IDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawVGAMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_DVI_INPUT
 // Input_Digital1
  { COORD(0,3+(SOURCE_DVI_IDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_HDMI_INPUT
 // Input_Digital2
  { COORD(MM_XSTART_L1+3,3+(SOURCE_HDMI1_IDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

#if ENABLE_HDMI2ND_INPUT
 // Input_Digital3
  { COORD(MM_XSTART_L1+3,3+(SOURCE_HDMI2_IDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if ENABLE_HDMI3ND_INPUT
 // Input_Digital4
  { COORD(MM_XSTART_L1+3,3+(SOURCE_HDMI3_IDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#endif

#if ENABLE_DP_INPUT
 // Input_Digital3
  { COORD(MM_XSTART_L1+3,3+(SOURCE_Displayport1_IDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if USB_Updata_FW_Enable
 // USB UPDATA
  { COORD(MM_XSTART_L1+3,3+(SOURCE_USB_UPDATA)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetUSBUpdataFW			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawUSBUpdataFWRadioGroup,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

};

#endif


#if ENABLE_VGA_INPUT
MenuItemType code AutoMenuItems[]=
{
  { COORD(0, 4), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE,CP_BK_COLOR_L0), // u8Color
    MONO_COLOR(CP_WHITE,CP_BK_COLOR_L0), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_FULL_CENTER,// u8TXTFlags;
    TxtAutoAdjustInProgress, // DisplayText;
    NothingKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};
#endif
#if ENABLE_MULTI_INPUT
/// For Input Menu Items ###############################################
MenuItemType code InputMenuItems[]=
{//
  { COORD(0, 3), // u8XPos, u8YPos;
    MONO_COLOR(CP_Orange,CP_MSG_BK), // u8Color
    MONO_COLOR(CP_Orange,CP_MSG_BK), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,// u8TXTFlags;
    NULL, // DisplayText;
    SwitchSourceKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// *stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      NULL,//DrawInputMenuRadioGroup,  // DrawRadioGroupType
      NULL //DrawInputMenuIcon  //DrawIconType  //Steven110519
    },
    0//MIB_SELECTABLE // u16Flags
  }
};
#endif

DrawIconType code DrawHotMenuLockIcon[]=
{//u8Flafs, u8XPos, u8YPos, u8IconWidth,    u8IconHeight,   u8Color,    u8SelColor, fpGetIcon
    {ICO_END,  COORD(0,  0), GetMenuLockIcon}
};

MenuItemType code HotMenuLockMenuItems[]=
{//
  { COORD(0, 0), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE,CP_WHITE), // u8Color
    MONO_COLOR(CP_WHITE,CP_WHITE), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NothingKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      DrawHotMenuLockIcon  //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};

//==============================================================================================
#if 0 //Steven110525
DrawIconType code DrawLogoMenuIcon[]=
{//u8Flafs, u8XPos, u8YPos, u8IconWidth,    u8IconHeight,   u8Color,    u8SelColor, fpGetIcon
    {0,  COORD(0,  8), GetLogoMstarDownIcon},
    {ICO_OVER256,  COORD(0,  0), GetLogoMstarTopIcon},
    {ICO_END,  COORD(16,  0), GetLogoMstarTop1Icon}
};
#endif

MenuItemType code LogoMenuItems[]=
{//0 Reset Icon
  { COORD(0, 2), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE,CP_WHITE), // u8Color
    MONO_COLOR(CP_WHITE,CP_WHITE), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_CENTER,// u8TXTFlags;
    NULL,//LogoText, // DisplayText;
    NothingKeyEvent,
    { NULL,	// AdjustFunction
      NULL	// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
















///MsgBox==========================================================================
#if ENABLE_AUDIO_SETTINGS_MENU
DrawGaugeType code DrawVolumeMenuGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, AdjustGaugeLenght, COORD(AdjustGaugeXPos, AdjustGaugeYPos),0}
};
DrawNumberType code DrawVolumeMenuNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_END|NUM_DRAW_ARROW,	MONO_COLOR(CP_ADJ_COLOR_L2,CP_BK_COLOR_L2), COORD(AdjustNumXPos ,AdjustNumYPos), 0, PROP_NUM(0)}
};
MenuItemType code HotVolumeMenuItems[] =
{
    // HotVolume adjust
    { COORD(AdjustTextXPos,AdjustTextYPos), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL,//TxtVolume, // DisplayText;
       HOTAdjustKeyEvent,
        {
            NULL,//AdjustVolume,  // AdjustFunction
            NULL          // ExecFunction
        },// *DisplayValue;
        {
            DrawVolumeMenuNumber,  // DrawNumberType
            DrawVolumeMenuGauge,   // DrawGaugeType
            NULL,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};
#endif


DrawGaugeType code DrawBrightnessMenuGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, AdjustGaugeLenght, COORD(AdjustGaugeXPos, AdjustGaugeYPos),GetBrightnessValue}
};
DrawNumberType code DrawBrightnessMenuNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_END|NUM_DRAW_ARROW,	MONO_COLOR(CP_ADJ_COLOR_L2,CP_BK_COLOR_L2), COORD(AdjustNumXPos ,AdjustNumYPos), GetBrightnessValue, PROP_NUM(0)}
};
MenuItemType code HotBrightnessMenuItems[] =
{
    // HotVolume adjust
    { COORD(AdjustTextXPos,AdjustTextYPos), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL, // DisplayText;
       HOTAdjustKeyEvent,
        {
            AdjustBrightness,  // AdjustFunction
            NULL          // ExecFunction
        },// *DisplayValue;
        {
            DrawBrightnessMenuNumber,  // DrawNumberType
            DrawBrightnessMenuGauge,   // DrawGaugeType
            NULL,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};


DrawGaugeType code DrawContrastMenuGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, AdjustGaugeLenght, COORD(AdjustGaugeXPos, AdjustGaugeYPos),GetContrastValue}
};
DrawNumberType code DrawContrastMenuNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_END|NUM_DRAW_ARROW,	MONO_COLOR(CP_ADJ_COLOR_L2,CP_BK_COLOR_L2), COORD(AdjustNumXPos ,AdjustNumYPos), GetContrastValue, PROP_NUM(0)}
};
MenuItemType code HotContrastMenuItems[] =
{
    // HotVolume adjust
    { COORD(AdjustTextXPos,AdjustTextYPos), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL, // DisplayText;
       HOTAdjustKeyEvent,
        {
            AdjustContrast,  // AdjustFunction
            NULL          // ExecFunction
        },// *DisplayValue;
        {
            DrawContrastMenuNumber,  // DrawNumberType
            DrawContrastMenuGauge,   // DrawGaugeType
            NULL,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};


#if Enable_HotAdjLowbluray
DrawGaugeType code DrawLowBlurayMenuGauge[]=
{// u8Flags, u8Color, u8Length, (u8XPos,u8YPos), GetValue
  {BAR_END, 0, AdjustGaugeLenght, COORD(AdjustGaugeXPos+2, AdjustGaugeYPos),GetLowblurayValue_100}
};
DrawNumberType code DrawLowBlurayMenuNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_END|NUM_DRAW_ARROW,	MONO_COLOR(CP_ADJ_COLOR_L2,CP_BK_COLOR_L2), COORD(AdjustNumXPos ,AdjustNumYPos), GetLowblurayValue_Number, PROP_NUM(0)}
};
MenuItemType code HotLowBlurayMenuItems[] =
{
    // AdjLowbluray adjust
    { COORD(AdjustTextXPos,AdjustTextYPos), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       TxtLowBluRay, // DisplayText;
       HOTAdjustKeyEvent,
        {
            AdjustLowbluray,  // AdjustFunction
            NULL          // ExecFunction
        },// *DisplayValue;
        {
            DrawLowBlurayMenuNumber,  // DrawNumberType
            DrawLowBlurayMenuGauge,   // DrawGaugeType
            NULL,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};
#endif


#if ENABLE_WIDE_MODE //Sky 101111
RadioTextType code HotPictureFormatMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
{TXT_END,	COORD(MM_XSTART_L2+1,13),	NULL} //null item for end string
};
DrawRadioGroupType code DrawHotPictureFormatMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
{MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L1), MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L1), NULL, HotPictureFormatMenuRadioText}
};

MenuItemType code HotPictureFormatMenuItems[] =
{
    // HotPictureFormatMenu adjust
    { COORD(0,0), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL, // DisplayText;
      HOTKeyEvent,
      { 
        NULL,	// AdjustFunction
        NULL// ExecFunction
       },// *DisplayValue;
        {
            NULL,  // DrawNumberType
            NULL,   // DrawGaugeType
            DrawHotPictureFormatMenuRadioGroup,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};
#endif


MenuItemType code HotPictureModeMenuItems[] =
{
    // HotPictureFormatMenu adjust
    { COORD(0,0), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL, // DisplayText;
      HOTKeyEvent,
      { 
        NULL,	// AdjustFunction
        NULL// ExecFunction
       },// *DisplayValue;
        {
            NULL,  // DrawNumberType
            NULL,   // DrawGaugeType
            NULL,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};

#if HKC_RGB_Range_Func
RadioTextType code HotRGBRangeMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
{TXT_FULL_CENTER,  COORD(0, 3),	TxtUserPrefRGBRange},
{TXT_END,	COORD(MM_XSTART_L2+1,13),	NULL} //null item for end string
};
DrawRadioGroupType code DrawHotRGBRangeMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
{MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L1), MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L1), NULL, HotRGBRangeMenuRadioText}
};

MenuItemType code HotRGBRangeMenuItems[] =
{
    // HotPictureFormatMenu adjust
    { COORD(0,0), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL, // DisplayText;
      HOTKeyEvent,
      { 
        NULL,	// AdjustFunction
        NULL// ExecFunction
       },// *DisplayValue;
        {
            NULL,  // DrawNumberType
            NULL,   // DrawGaugeType
            DrawHotRGBRangeMenuRadioGroup,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};
#endif

#if ENABLE_AUDIO_SETTINGS_MENU
MenuItemType code HotMuteMenuItems[] =
{
    // HotMuteMenu adjust
    { COORD(0,0), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL, // DisplayText;
      HOTKeyEvent,
      { 
        NULL,	// AdjustFunction
        NULL// ExecFunction
       },// *DisplayValue;
        {
            NULL,  // DrawNumberType
            NULL,   // DrawGaugeType
            NULL,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};
#endif

RadioTextType code HotDCRMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
{TXT_END,	COORD(MM_XSTART_L2+1,13),	NULL} //null item for end string
};
DrawRadioGroupType code DrawHotDCRMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
{MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L1), MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L1), NULL, HotDCRMenuRadioText}
};

MenuItemType code HotDCRMenuItems[] =
{
    // HotDCRMenu adjust
    { COORD(0,0), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL, // DisplayText;
      HOTKeyEvent,
      { 
        NULL,	// AdjustFunction
        NULL// ExecFunction
       },// *DisplayValue;
        {
            NULL,  // DrawNumberType
            NULL,   // DrawGaugeType
            DrawHotDCRMenuRadioGroup,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};


MenuItemType code HotCrossMenuItems[] =
{
    // HotCrossMenu adjust
    { COORD(0,0), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       NULL, // DisplayText;
      HOTKeyEvent,
      { 
        NULL,	// AdjustFunction
        NULL// ExecFunction
       },// *DisplayValue;
        {
            NULL,  // DrawNumberType
            NULL,   // DrawGaugeType
            NULL,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          MIB_ADJUST_ITEM// u16Flags
    },
};


#if SleepMode_Enable
RadioTextType code PowerOffMsgRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
 {0,	COORD(9, 2), TxtPowerOffMsg},
 {0,	COORD(19, 2), TxtPowerOffS},
  
{TXT_END,	COORD(8,5),	NULL} //null item for end string
};
DrawRadioGroupType code DrawPowerOffMsgRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0), MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0), NULL, PowerOffMsgRadioText}
};

MenuItemType code PowerOffMsgMenuItems[]=
{
  { COORD(0, 2), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE,CP_BK_COLOR_L0), // u8Color
    MONO_COLOR(CP_WHITE,CP_BK_COLOR_L0), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    RootKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawPowerOffMsgRadioGroup,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};
#endif


#if SleepMode_Enable
RadioTextType code HotSleepModeMenuRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
{ 0, COORD(16, 4), TxtSleepModeIndx},
{TXT_END,	COORD(MM_XSTART_L2+1,13),	NULL} //null item for end string
};
DrawRadioGroupType code DrawHotSleepModeMenuRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
{MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L1), MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_L1), NULL, HotSleepModeMenuRadioText}
};

MenuItemType code HotSleepModeMenuItems[] =
{
    // HotPictureFormatMenu adjust
    { COORD(7, 4), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
       MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
       MENU_ROOT, //NextMenuPage;
       0,//TXT_RIGHT,// u8TXTFlags;
       TxtSleepMode, // DisplayText;
      HOTKeySleepModeEvent,
      { 
        NULL,	// AdjustFunction
        NULL// ExecFunction
       },// *DisplayValue;
        {
            NULL,  // DrawNumberType
            NULL,   // DrawGaugeType
            DrawHotSleepModeMenuRadioGroup,  // DrawRadioGroupType
            NULL  //DrawIconType
        },
          0// u16Flags
    },
};
#endif

#if (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
MenuItemType code HotInputMenuItems[] =
{
#if Enable_AutoInput_Menu
 // AutoSelect
  {COORD(0,  (2+(SOURCE_AUTO_SWITCH_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL//DrawVGAMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_VGA_INPUT
 // VGA
  {COORD(0,  (2+(SOURCE_VGA_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawVGAMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if ENABLE_DP_INPUT
 // Input_Digital3
  { COORD(0, (2+(SOURCE_Displayport1_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if ENABLE_HDMI_INPUT
 // Input_Digital2
  { COORD(0,(2+(SOURCE_HDMI1_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

#if ENABLE_HDMI2ND_INPUT
 // Input_Digital3
  { COORD(0, (2+(SOURCE_HDMI2_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if ENABLE_HDMI3ND_INPUT
 // Input_Digital4
  { COORD(0, (2+(SOURCE_HDMI3_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#endif

#if ENABLE_DVI_INPUT
 // Input_Digital1
  { COORD(0,(2+(SOURCE_DVI_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if USB_Updata_FW_Enable
 // USB UPDATA
  { COORD(0, (2+(SOURCE_USB_UPDATA))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetUSBUpdataFW			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawUSBUpdataFWRadioGroup,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

};

#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
MenuItemType code HotInputMenuItems[] =
{
#if Enable_AutoInput_Menu
 // AutoSelect
  {COORD(0,  (2+(SOURCE_AUTO_SWITCH_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL//DrawVGAMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_DP_INPUT
 // Input_Digital3
  { COORD(0, (2+(SOURCE_Displayport1_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if ENABLE_VGA_INPUT
 // VGA
  {COORD(0,  (2+(SOURCE_VGA_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawVGAMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_DVI_INPUT
 // Input_Digital1
  { COORD(0,(2+(SOURCE_DVI_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_HDMI_INPUT
 // Input_Digital2
  { COORD(0,(2+(SOURCE_HDMI1_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

#if ENABLE_HDMI2ND_INPUT
 // Input_Digital3
  { COORD(0, (2+(SOURCE_HDMI2_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if ENABLE_HDMI3ND_INPUT
 // Input_Digital4
  { COORD(0, (2+(SOURCE_HDMI3_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#endif


#if USB_Updata_FW_Enable
 // USB UPDATA
  { COORD(0, (2+(SOURCE_USB_UPDATA))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetUSBUpdataFW			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawUSBUpdataFWRadioGroup,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

};


#else
MenuItemType code HotInputMenuItems[] =
{
#if Enable_AutoInput_Menu
 // AutoSelect
  {COORD(0,  (2+(SOURCE_AUTO_SWITCH_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL//DrawVGAMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_VGA_INPUT
 // VGA
  {COORD(0,  (2+(SOURCE_VGA_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawVGAMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_DVI_INPUT
 // Input_Digital1
  { COORD(0,(2+(SOURCE_DVI_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#if ENABLE_HDMI_INPUT
 // Input_Digital2
  { COORD(0,(2+(SOURCE_HDMI1_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },

#if ENABLE_HDMI2ND_INPUT
 // Input_Digital3
  { COORD(0, (2+(SOURCE_HDMI2_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if ENABLE_HDMI3ND_INPUT
 // Input_Digital4
  { COORD(0, (2+(SOURCE_HDMI3_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif
#endif

#if ENABLE_DP_INPUT
 // Input_Digital3
  { COORD(0, (2+(SOURCE_Displayport1_IDX))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetHotInputSelect			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

#if USB_Updata_FW_Enable
 // USB UPDATA
  { COORD(0, (2+(SOURCE_USB_UPDATA))), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NaviExecNextKeyEvent,
    { NULL,	// AdjustFunction
      SetUSBUpdataFW			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawUSBUpdataFWRadioGroup,	// DrawRadioGroupType
      NULL  //DrawDVIMultiIcon  //DrawIconType
    },
    MIB_SELECTABLE // u16Flags
  },
#endif

};

#endif





#if ENABLE_DEMURA_FUNCTION
MenuItemType code DeMuraMeasureModeItems[]=
{
 {  COORD(0, 3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    TXT_FUNC,//TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NothingKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL
    },
    0 // u16Flags
  },
};
#endif


MenuItemType code KeyGuideMenuItems[]=
{
 {  COORD(0, 3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0), // u8SelColor
    MENU_MAIN, //u8NextMenuPage;
    TXT_FUNC,//TXT_FULL_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    RootKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL
    },
    0 // u16Flags
  },
};



#if 0//LED_HOT_Menu_Func

RadioTextType code LedColorRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_END,	                    COORD((LedColorTextXPos+10),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorIDX)), TxtLedColorMode},
};
DrawRadioGroupType code DrawLedColorRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), MONO_COLOR(CP_ADJ_COLOR_L2, CP_BK_COLOR_H), NULL, LedColorRadioText}
};

RadioTextType code LedStrengthRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_END,	                    COORD((LedColorTextXPos+10),(LedColorTextYPos+LedColorTextShiftXPos*HotLedStrengthIDX)), TxtLedStrengthType},
};
DrawRadioGroupType code DrawLedStrengthRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), MONO_COLOR(CP_ADJ_COLOR_L2, CP_BK_COLOR_H), NULL, LedStrengthRadioText}
};

RadioTextType code LedModeRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {TXT_END,	                    COORD((LedColorTextXPos+10),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeIDX)), TxtLedModeType},
};
DrawRadioGroupType code DrawLedModeRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), MONO_COLOR(CP_ADJ_COLOR_L2, CP_BK_COLOR_H), NULL, LedModeRadioText}
};


RadioTextType code HotLedModeRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  {0,	                    COORD((LedColorTextXPos+10),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorIDX)), TxtLedColorMode},
  {0,	                    COORD((LedColorTextXPos+10),(LedColorTextYPos+LedColorTextShiftXPos*HotLedStrengthIDX)), TxtLedStrengthType},
  {0,	                    COORD((LedColorTextXPos+10),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeIDX)), TxtLedModeType},
  
  {TXT_END,	      COORD(MM_XSTART_L2+1,19),	NULL}
};
DrawRadioGroupType code DrawHotLedModeRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
 {MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), MONO_COLOR(CP_ADJ_COLOR_L2, CP_BK_COLOR_H), NULL, HotLedModeRadioText}
};



MenuItemType code HotLEDModeMenuItems[]=
{
    // Led Color
 {  COORD((LedColorTextXPos-2),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorIDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,// u8TXTFlags;
    TxtLedColor, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
  },
    //Led Color Adjuster
  { COORD((LedColorTextXPos-2),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorIDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT,
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterFuncNaviKeyEvent,
    { AdjustLedColorValue,	// AdjustFunction
      NULL// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedColorRadioGroup,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_IR_DELAY|MIB_IR_SELECT// u16Flags
  },

    // Led Strength
 {  COORD((LedColorTextXPos-2),(LedColorTextYPos+LedColorTextShiftXPos*HotLedStrengthIDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,// u8TXTFlags;
    TxtLedStrength, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
  },
    //Led Strength Adjuster
  { COORD((LedColorTextXPos-2),(LedColorTextYPos+LedColorTextShiftXPos*HotLedStrengthIDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT,
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterFuncNaviKeyEvent,
    { AdjustLedStrengthValue,	// AdjustFunction
      NULL// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedStrengthRadioGroup,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_IR_DELAY|MIB_IR_SELECT// u16Flags
  },

    // Led Mode
 {  COORD((LedColorTextXPos-2),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeIDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,// u8TXTFlags;
    TxtLedMode, // DisplayText;
    NaviAdjusterKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
  },
    //Led Mode Adjuster
  { COORD((LedColorTextXPos-2),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeIDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT,
    0,// u8TXTFlags;
    NULL, // DisplayText;
    AdjusterFuncNaviKeyEvent,
    { AdjustLedModeValue,	// AdjustFunction
      NULL// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLedModeRadioGroup,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    MIB_ADJUST_ITEM|MIB_IR_DELAY|MIB_IR_SELECT// u16Flags
  },
/*
    // Led Mode Confirm
 {  COORD(LedColorTextXPos,(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeConfirmIDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_L1), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,// u8TXTFlags;
    TxtLedModeConfirm, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetLedModeConfirm			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
  },
*/
    // Led Mode Cancel
 {  COORD((LedColorTextXPos-2),(LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeCancelIDX)), // u8XPos, u8YPos;
    MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_RED, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT, //u8NextMenuPage;
    0,// u8TXTFlags;
    TxtLedModeCancel, // DisplayText;
    NaviExecFuncKeyEvent,
    { NULL,	// AdjustFunction
      SetLedModeCancel			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
  },
	
	
  //NULL item for showing current settings
  {  COORD(TitleTextXPos,TitleTextYPos), // u8XPos, u8YPos;
     MONO_COLOR(CP_BK_COLOR_H, CP_FG_COLOR_L2), // u8Color
     MONO_COLOR(CP_BK_COLOR_H, CP_FG_COLOR_L2), // u8SelColor
     MENU_ROOT, //u8NextMenuPage;
     TXT_FULL_CENTER,// u8TXTFlags;
     TxtLEDModeTitle, // DisplayText;
     NaviKeyEvent,
     { NULL,	// AdjustFunction
       NULL			// ExecFunction
     },// *stDisplayValue;
     { NULL,	// DrawNumberType
       NULL,	// DrawGaugeType
      DrawHotLedModeRadioGroup, //DrawRadioGroupType
       NULL  //DrawIconType
     },
     0// u16Flags
  }
};

#endif


#if Enable_Corss_LHCX_New

RadioTextType code DrawGameMenuRatio[]=
{ // Flags,					XPos,	YPos,	DisplayText
  {TXT_END,	                    COORD(0x1C, 0x02), HotKeyGameModetext}
};
DrawRadioGroupType code DrawGameMenuRatioGroup[]=
{ //ForeColor,					BackColor,						GetValue	RadioText
  {MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), NULL, DrawGameMenuRatio}
};


MenuItemType code HotKeyGameMenuItems[]=
{
     {COORD(0x10,  8), // u8XPos, u8YPos;
       MONO_COLOR(CP_Black, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_Black, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameModeIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,//DrawGameMenuRatioGroup	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
	
     {COORD(0x26,  8), // u8XPos, u8YPos;
       MONO_COLOR(CP_Black, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_Black, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameClokMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClokIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,	// DrawNumberType
	NULL,	// DrawGuageType
	NULL,   // DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
	
     {COORD(0x1A,  8), // u8XPos, u8YPos;
       MONO_COLOR(CP_FG_COLOR_L2, CP_Black), // u8Color
       MONO_COLOR(CP_FG_COLOR_L2, CP_Black), // u8SelColor
	HotKeyGameClokMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	DrawGameMenuRatioGroup,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
       0// u16Flags
	}
};


RadioTextType code DrawGameSelectMenuRatio[]=
{ // Flags,					XPos,	YPos,	DisplayText
  {TXT_END,	                    COORD(0x1C, 0x02), HotKeyGameSelectModetext}
};
DrawRadioGroupType code DrawGameSelectMenuRatioGroup[]=
{ //ForeColor,					BackColor,						GetValue	RadioText
  {MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), NULL, DrawGameSelectMenuRatio}
};
MenuItemType code HotKeyGameSelectMenuItems[]=
{
	///0
     {COORD(0x5,  7), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_GREEN, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameSubSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameMode1Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
	///1
     {COORD(0x13,  7), // u8XPos, u8YPos;
       MONO_COLOR(CP_MSG_BK_H, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_MSG_BK_H, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameSubSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameMode1Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
	///2
     {COORD(0x21,  7), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_GREEN, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameSubSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameMode2Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
	///3
     {COORD(0x2F,  7), // u8XPos, u8YPos;
       MONO_COLOR(CP_MSG_BK_H, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_MSG_BK_H, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameSubSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameMode2Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
     {COORD(0x1A,  2), // u8XPos, u8YPos;
       MONO_COLOR(CP_FG_COLOR_L2, CP_Black), // u8Color
       MONO_COLOR(CP_FG_COLOR_L2, CP_Black), // u8SelColor
	HotKeyGameSubSelectMenu, //NextMenuPage;
	0,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	DrawGameSelectMenuRatioGroup,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
       0// u16Flags
	}
};

MenuItemType code HotKeyGameSubSelectMenuItems[]=
{
	///0
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_CLEAR), // u8Color
       MONO_COLOR(CP_GREEN, CP_CLEAR), // u8SelColor
	HotKeyGameSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameMode1SubIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
	///1
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_MSG_BK_H, CP_CLEAR), // u8Color
       MONO_COLOR(CP_MSG_BK_H, CP_CLEAR), // u8SelColor
	HotKeyGameSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameMode1SubIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
	///2
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_CLEAR), // u8Color
       MONO_COLOR(CP_GREEN, CP_CLEAR), // u8SelColor
	HotKeyGameSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameMode2SubIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
	///3
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_MSG_BK_H, CP_CLEAR), // u8Color
       MONO_COLOR(CP_MSG_BK_H, CP_CLEAR), // u8SelColor
	HotKeyGameSelectMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameMode2SubIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	}
};


RadioTextType code DrawGameClokMenuRatio[]=
{ // Flags,					XPos,	YPos,	DisplayText
  {TXT_END,	                    COORD(0x1C, 0x02), 	HotKeyGameClokModetext}
};
DrawRadioGroupType code DrawGameClokMenuRatioGroup[]=
{ //ForeColor,					BackColor,						GetValue	RadioText
  {MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), NULL, DrawGameClokMenuRatio}
};
MenuItemType code HotKeyGameClokMenuItems[]=
{
//-0-HotKeyGameClok_30MItem
     {COORD(0x0B,  4), // u8XPos, u8YPos;
       MONO_COLOR(CP_Black, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_Black, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameClokPositionMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClok_30M_Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-1-HotKeyGameClok_40MItem
     {COORD(0x19,  4), // u8XPos, u8YPos;
       MONO_COLOR(CP_Black, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_Black, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameClokPositionMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClok_40M_Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-2-HotKeyGameClok_50MItem
     {COORD(0x27,  4), // u8XPos, u8YPos;
       MONO_COLOR(CP_Black, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_Black, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameClokPositionMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClok_50M_Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-3-HotKeyGameClok_60MItem
     {COORD(0x0B,  9), // u8XPos, u8YPos;
       MONO_COLOR(CP_Black, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_Black, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameClokPositionMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClok_60M_Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-4-HotKeyGameClok_90MItem
     {COORD(0x19,  9), // u8XPos, u8YPos;
       MONO_COLOR(CP_Black, CP_FG_COLOR_L2), // u8Color
       MONO_COLOR(CP_Black, CP_BK_COLOR_L0), // u8SelColor
	HotKeyGameClokPositionMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClok_90M_Icon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
     {COORD(0x1A,  2), // u8XPos, u8YPos;
       MONO_COLOR(CP_FG_COLOR_L2, CP_Black), // u8Color
       MONO_COLOR(CP_FG_COLOR_L2, CP_Black), // u8SelColor
	HotKeyGameClokPositionMenu, //NextMenuPage;
	0,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	DrawGameClokMenuRatioGroup,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
       0// u16Flags
	}
};



RadioTextType code DrawGameClokPositionMenuRatio[]=
{ // Flags,					XPos,	YPos,	DisplayText
  {TXT_END,	                    COORD(0x02, 0x0E), 		HotKeyGameClokPositionModetext}
};
DrawRadioGroupType code DrawGameClokPositionMenuRatioGroup[]=
{ //ForeColor,					BackColor,						GetValue	RadioText
  {MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), NULL, 	DrawGameClokPositionMenuRatio}
};
MenuItemType code HotKeyGameClokPositionMenuItems[]=
{
//-0-HotKeyGameClokPosition_UpItem
     {COORD(0x0A,  0x0E), // u8XPos, u8YPos;
       FOUR_COLOR(27), // u8Color
       FOUR_COLOR(26), // u8SelColor
	HotKeyGameClokShowMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClokPosition_UpIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-1-HotKeyGameClokPosition_CenterItem
     {COORD(0x18,  0x0E), // u8XPos, u8YPos;
       FOUR_COLOR(27), // u8Color
       FOUR_COLOR(26), // u8SelColor
	HotKeyGameClokShowMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClokPosition_CenterIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-2-HotKeyGameClokPosition_DownItem
     {COORD(0x26,  0x0E), // u8XPos, u8YPos;
       FOUR_COLOR(27), // u8Color
       FOUR_COLOR(26), // u8SelColor
	HotKeyGameClokShowMenu, //NextMenuPage;
	TXT_ICON,// DrawMenuItemType;
	HotKeyGameClokPosition_DownIcon, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
     {COORD(0x1A,  0x02), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8Color
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8SelColor
	HotKeyGameClokShowMenu, //NextMenuPage;
	0,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{  NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	DrawGameClokPositionMenuRatioGroup,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
       0// u16Flags
	}
};

MenuItemType code HotKeyGameClokWhowMenuItems[]=
{
//-0-HotKeyGameClok_30MItem
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8Color
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8SelColor
	HotKeyGameClokMenu, //NextMenuPage;
	0,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-1-HotKeyGameClok_40MItem
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8Color
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8SelColor
	HotKeyGameClokMenu, //NextMenuPage;
	0,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-2-HotKeyGameClok_50MItem
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8Color
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8SelColor
	HotKeyGameClokMenu, //NextMenuPage;
	0,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-3-HotKeyGameClok_60MItem
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8Color
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8SelColor
	HotKeyGameClokMenu, //NextMenuPage;
	0,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	},
//-4-HotKeyGameClok_90MItem
     {COORD(0,  0), // u8XPos, u8YPos;
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8Color
       MONO_COLOR(CP_GREEN, CP_MSG_BK_H), // u8SelColor
	HotKeyGameClokMenu, //NextMenuPage;
	0,// DrawMenuItemType;
	NULL, // DisplayText;
	AdjusterKeyHotKeyGameMenuEvent,// KeyEvent
	{ NULL,//AdjustVolume,	// AdjustFunction
	NULL			// ExecFunction
	},
	{ 
	NULL,//DrawHotKeyVolumeIconNumber,//DrawHotKeyVolumeIconNumber,	// DrawNumberType
	NULL,//DrawHotKeyVolumeIconGuage,//DrawHotKeyVolumeIconGuage,	// DrawGuageType
	NULL,	// DrawRadioGroupType
       NULL  //DrawIconType
	},
        MIB_SELECTABLE|MIB_IR_SELECT// u16Flags
	}
};



#endif




//==============================================================================================
MenuPageType code tblMenus[]=
{ // 0 MENU_POWER_OFF
  { SIZE(16, 3), // u8XSize, u8YSize;
    MENU_POWER_ON,// u8PrevMenuPage;
    PowerOffMenuItems, // pstMenuItems;
    sizeof(PowerOffMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    PowerOffSystem,// ExecFunction;
    NULL, // LoadFont
    MPB_INVISIBLE|MPB_STAY //     u8Flags;
  },
  // 1 MENU_POWER_ON
  { SIZE(16, 3), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    PowerOffMenuItems, // pstMenuItems; //Jison 100323
    sizeof(PowerOffMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    PowerOnSystem,// ExecFunction;
    NULL, // LoadFont
    MPB_INVISIBLE |MPB_STAY//     u8Flags;
  },
  // 2 MENU_STANDBY
  {
    //SIZE(MSG_XSIZE, 7+2), //   u8XSize, u8YSize;
    SIZE(36, 17),          //   u8XSize, u8YSize;  //Steven110601
    MENU_ROOT,// u8PrevMenuPage;
    StandbyMenuItems, // pstMenuItems;
    sizeof(StandbyMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    EnterStandbyOnMenu, // ExecFunction;
    LoadPropFontAndInputIconFont,//DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
  // 3 MENU_CABLE_NOT_CONNECT
  { SIZE(36, 17),//SIZE(FLY_WND_XSIZE, 13),// u8XSize, u8YSize;   //30  //Mike110531
    MENU_ROOT,// u8PrevMenuPage;
    CableNotConnectedMenuItems,//,StandbyMenuItems, // pstMenuItems;
    sizeof(CableNotConnectedMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    EnterCheckCableMenu, // ExecFunction;
    LoadPropFontAndInputIconFont, // LoadFont
    MPB_STAY|MPB_CENTER //   u8Flags;
  },

  // 4 MENU_ENTERPOWERSAVING
  {
    SIZE(36, 17),          //   u8XSize, u8YSize;  //Steven110601
    MENU_ROOT,// u8PrevMenuPage;
    EnterPowerSavingMenuItems, // pstMenuItems;
    sizeof(EnterPowerSavingMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    EnterSuspendOnMenu, // ExecFunction;
    LoadPropFontAndInputIconFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  }, 

  // 5 MENU_UNSUPPORTED_MODE
  {
    SIZE(36, 17),//SIZE(MSG_XSIZE, 15), //   u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    UnsupportedModeMenuItems, // pstMenuItems;
    sizeof(UnsupportedModeMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    LoadUnsupportedModeMenuFont, // LoadFont
    MPB_STAY|MPB_CENTER//|MPB_FUNC//   u8Flags;  //Mike110531
  },
  // 6 MENU_ROOT
  { //SIZE(18, 3), // u8XSize, u8YSize;
    SIZE(24, 4), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    RootMenuItems, // pstMenuItems;
    sizeof(RootMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,// ExecFunction;
    NULL, // LoadFont
    MPB_INVISIBLE|MPB_STAY //   u8Flags;
  },
#if (defined(LianHeChuangXin_Project))
  //  MENU_KEY_GUIDE
  { 
 #if (PanelType==PanelT430QVN01)
    SIZE(26, 3), // u8XSize, u8YSize;
 #elif (PanelType==PanelM350DVR01_01)
    SIZE(22, 3), // u8XSize, u8YSize;
 #elif (PanelType==PanelLSM315DP01)
    SIZE(26, 3), // u8XSize, u8YSize;
 #elif (PanelType==PanelLSM315HP01)
    SIZE(20, 3), // u8XSize, u8YSize;
 #else
    SIZE(30, 3), // u8XSize, u8YSize;
 #endif   
    MENU_ROOT,// u8PrevMenuPage;
    KeyGuideMenuItems, // pstMenuItems;
    sizeof(KeyGuideMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,// ExecFunction;
    NULL, // LoadFont
    MPB_STAY //   u8Flags;
  },
#endif	
  //============================================================
#if LOGO_ENABLE  
   //  MENU_LOGO  //
  {
    SIZE(LogoMenuHSize, LogoMenuVSize), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    LogoMenuItems, // pstMenuItems;
    sizeof(LogoMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    EnterDisplayLogo, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY|MPB_CENTER|MPB_OSD_COUNT3 //   u8Flags;
  },
#endif  
#if ENABLE_VGA_INPUT
   //  MENU_AUTO
  { SIZE(STATUS_XSIZE, 7), // u8XSize, u8YSize;
    MENU_MAIN,// u8PrevMenuPage;
    AutoMenuItems, // pstMenuItems;
    sizeof(AutoMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    AutoConfig, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_CENTER //   u8Flags;
  },
    //  MENU_HOT_AUTO
  { SIZE(STATUS_XSIZE, 7), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    AutoMenuItems, // pstMenuItems;
    sizeof(AutoMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    AutoConfig, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_CENTER //   u8Flags;
  },
#endif
#if ENABLE_MULTI_INPUT
  //  MENU_INPUT
  { SIZE(SM_XSIZE, SM_YSIZE), // u8XSize, u8YSize;  //Mike110531
    MENU_ROOT,// u8PrevMenuPage;
    InputMenuItems, // pstMenuItems;
    sizeof(InputMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    EnterInputMenu, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY|MPB_OSD_COUNT3 //   u8Flags;
  },
#endif
    //MENU_HOT_MENU_LOCK
   { SIZE(12, 7), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotMenuLockMenuItems, // pstMenuItems;
    sizeof(HotMenuLockMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    ClearHoldMenuKeyFlag, // ExecFunction;
    LoadMenuLockIconFont, // LoadFont
    MPB_STAY|MPB_CENTER|MPB_OSD_COUNT3 //   u8Flags;
  },  
#if ENABLE_DEMURA_FUNCTION
  // MENU_DEMURA_MEASURE_MODE,
  { SIZE((1920/12), (1080/18)), //u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    DeMuraMeasureModeItems, // pstMenuItems;
    sizeof(DeMuraMeasureModeItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY //   u8Flags;
  },
#endif
  //  MENU_MAIN
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_ROOT,// u8PrevMenuPage;
    MainMenuItems, // pstMenuItems;
    sizeof(MainMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY//   u8Flags;
  },
///SubMenu
//MENU_QUICKSTART
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_MAIN,// u8PrevMenuPage;
    QuickStartMenuItems, // pstMenuItems;
    sizeof(QuickStartMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY//   u8Flags;
  },

//MENU_PICTURE
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_MAIN,// u8PrevMenuPage;
    PictureMenuItems, // pstMenuItems;
    sizeof(PictureMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY//   u8Flags;
  },

//MENU_COLOR
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_MAIN,// u8PrevMenuPage;
    ColorMenuItems, // pstMenuItems;
    sizeof(ColorMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY//   u8Flags;
  },

//MENU_SYSTEM
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_MAIN,// u8PrevMenuPage;
    SystemMenuItems, // pstMenuItems;
    sizeof(SystemMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY//   u8Flags;
  },
#if LED_HOT_Menu_Func
//MENU_SYSTEMLLEDUSER
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_MAIN,// u8PrevMenuPage;
    SystemMenuLEDUSERItems, // pstMenuItems;
    sizeof(SystemMenuLEDUSERItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY//   u8Flags;
  },
#endif
//MENU_USER
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_COLOR,// u8PrevMenuPage;
    UserMenuItems, // pstMenuItems;
    sizeof(UserMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY//   u8Flags;
  },

//MENU_ADVANCED
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_COLOR,// u8PrevMenuPage;
    AdvancedMenuItems, // pstMenuItems;
    sizeof(AdvancedMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY//   u8Flags;
  },
#if LED_HOT_Menu_Func
//MENU_LEDMODE
  { SIZE(MM_XSIZE, MM_YSIZE), //u8XSize, u8YSize
    MENU_SYSTEMLLEDUSER,// u8PrevMenuPage;
    LedModeMenuItems, // pstMenuItems;
    sizeof(LedModeMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL,//SetGradualColor, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY//   u8Flags;
  },
#endif









#if ENABLE_MULTI_INPUT
  //  MENU_INPUT_SOURCE
  { SIZE(21, 10), // u8XSize, u8YSize;
    MENU_MAIN,// u8PrevMenuPage;
    InputSourceMenuItems, // pstMenuItems;
    sizeof(InputSourceMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    LoadInputSourceMenuFont,//DynamicLoadPropFont, // LoadFont
    MPB_STAY //   u8Flags;
  },
#endif
#if ENABLE_AUDIO_SETTINGS_MENU
  //  MENU_HOTVOLUME
  { SIZE(AdjustMenuHSize, AdjustMenuVSize),// u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotVolumeMenuItems, // pstMenuItems;
    sizeof(HotVolumeMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
#endif  
  //  MENU_HOTBRIGHTNESS
  { SIZE(AdjustMenuHSize, AdjustMenuVSize),// u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotBrightnessMenuItems, // pstMenuItems;
    sizeof(HotBrightnessMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
  //  MENU_HOTCONTRAST
  { SIZE(AdjustMenuHSize, AdjustMenuVSize),// u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotContrastMenuItems, // pstMenuItems;
    sizeof(HotContrastMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
#if Enable_HotAdjLowbluray
  //  MENU_HOTLOWBLURAY
  { SIZE(AdjustMenuHSize, AdjustMenuVSize),// u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotLowBlurayMenuItems, // pstMenuItems;
    sizeof(HotLowBlurayMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
#endif
  //  MENU_HOTPICTUREFORMAT
  { SIZE(SM_XSIZE, SM_YSIZE), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotPictureFormatMenuItems, // pstMenuItems;
    sizeof(HotPictureFormatMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
  //  MENU_HOTPICTUREMODE
  { SIZE(SM_XSIZE, SM_YSIZE), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotPictureModeMenuItems, // pstMenuItems;
    sizeof(HotPictureModeMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    NULL, // LoadFont
    MPB_STAY//   u8Flags;
  },

  #if ENABLE_AUDIO_SETTINGS_MENU  
  //  MENU_HOTMUTE
  { SIZE(SM_XSIZE, SM_YSIZE), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotMuteMenuItems, // pstMenuItems;
    sizeof(HotMuteMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    Osd_LoadHotMuteFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
#endif  
#if HKC_RGB_Range_Func
  //  MENU_HOTRGBRANGE
  { SIZE(SM_XSIZE, SM_YSIZE), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotRGBRangeMenuItems, // pstMenuItems;
    sizeof(HotRGBRangeMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
#endif
  //  MENU_HOTDCR
  { SIZE(SM_XSIZE, SM_YSIZE), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotDCRMenuItems, // pstMenuItems;
    sizeof(HotDCRMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    SetHotDCRFunction, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },



  //  MENU_HOTINPUT
  { 
#if Enable_AutoInput_Menu
   SIZE(23, 10), // u8XSize, u8YSize;
#else
  #if(MS_BOARD_TYPE_SEL == BD_MST9UHD_DV1)
  SIZE(23, 10),
  #else
   SIZE(23, 8), // u8XSize, u8YSize;
   #endif
#endif  
    MENU_ROOT,// u8PrevMenuPage;
    HotInputMenuItems, // pstMenuItems;
    sizeof(HotInputMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    EnterDisplayInputHotMenu, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY//   u8Flags;
  },
  //  MENU_HOTCROSS
  { 
#if Enable_Corss_LHCX
    SIZE(12, 6), // u8XSize, u8YSize;
#else
    SIZE(10, 6), // u8XSize, u8YSize;
#endif
    MENU_ROOT,// u8PrevMenuPage;
    HotCrossMenuItems, // pstMenuItems;
    sizeof(HotCrossMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    Osd_LoadHOTCROSSFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
#if SleepMode_Enable 
    //  MENU_POWEROFFMSG
  { SIZE(STATUS_XSIZE, 5), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    PowerOffMsgMenuItems, // pstMenuItems;
    sizeof(PowerOffMsgMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER //   u8Flags;
  },
#endif

#if SleepMode_Enable  
  //  MENU_SLEEPMODE
  { SIZE(28, 7), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotSleepModeMenuItems, // pstMenuItems;
    sizeof(HotSleepModeMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
#endif  

#if 0//LED_HOT_Menu_Func
  //  MENU_HOTLEDMODE
  { SIZE(60, 24), // u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    HotLEDModeMenuItems, // pstMenuItems;
    sizeof(HotLEDModeMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, // LoadFont
    MPB_STAY|MPB_CENTER//   u8Flags;
  },
#endif

#if Enable_Corss_LHCX_New
   // HotKeyGameModeMenu
  { SIZE(60, 20), // u8XSize, u8YSize;
   MENU_ROOT,// PrevMenuPage;
   HotKeyGameMenuItems, // MenuItems;
   sizeof(HotKeyGameMenuItems)/sizeof(MenuItemType), // MenuItemCount;
    NULL, // ExecFunction;
    DynamicLoadPropFont, //Fonts
    MPB_STAY|MPB_CENTER//   u8Flags;
   },
   // HotKeyGameSelectMenu
  { SIZE(60, 20), // u8XSize, u8YSize;
   HotKeyGameModeMenu,// PrevMenuPage;
   HotKeyGameSelectMenuItems, // MenuItems;
   sizeof(HotKeyGameSelectMenuItems)/sizeof(MenuItemType), // MenuItemCount;
   NULL, // ExecFunction;
   NULL, //Fonts
    MPB_STAY|MPB_CENTER//   u8Flags;
   },
   // HotKeyGameSubSelectMenu
  { SIZE(4, 3), // u8XSize, u8YSize;
   HotKeyGameSelectMenu,// PrevMenuPage;
   HotKeyGameSubSelectMenuItems, // MenuItems;
   sizeof(HotKeyGameSubSelectMenuItems)/sizeof(MenuItemType), // MenuItemCount;
   NULL, // ExecFunction;
   NULL, //Fonts
    MPB_STAY|MPB_CENTER//   u8Flags;
   },
   // HotKeyGameClokMenu
  { SIZE(60, 20), // u8XSize, u8YSize;
   HotKeyGameModeMenu,// PrevMenuPage;
   HotKeyGameClokMenuItems, // MenuItems;
   sizeof(HotKeyGameClokMenuItems)/sizeof(MenuItemType), // MenuItemCount;
   NULL, // ExecFunction;
   NULL, //Fonts
    MPB_STAY|MPB_CENTER//   u8Flags;
   },
   // HotKeyGameClokPositionMenu
  { SIZE(60, 20), // u8XSize, u8YSize;
   HotKeyGameClokMenu,// PrevMenuPage;
   HotKeyGameClokPositionMenuItems, // MenuItems;
   sizeof(HotKeyGameClokPositionMenuItems)/sizeof(MenuItemType), // MenuItemCount;
   NULL, // ExecFunction;
   NULL, //Fonts
    MPB_STAY|MPB_CENTER//   u8Flags;
   },
   //HotKeyGameClokShowMenu,
  { SIZE(10, 4), // u8XSize, u8YSize;
   HotKeyGameClokMenu,// PrevMenuPage;
   HotKeyGameClokWhowMenuItems, // MenuItems;
   sizeof(HotKeyGameClokWhowMenuItems)/sizeof(MenuItemType), // MenuItemCount;
   NULL, // ExecFunction;
   NULL, //Fonts
    MPB_STAY//   u8Flags;
   },


#endif




};

#endif   ///_MENUTBL_H


