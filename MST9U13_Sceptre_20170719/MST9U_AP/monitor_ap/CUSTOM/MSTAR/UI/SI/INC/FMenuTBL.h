///////////////////////////////////////////////////////////////////////////////
/// @file FMenuTbl.h
/// @brief Define the tables for Factory menus.
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _FMENUTBL_H
#define _FMENUTBL_H

/********************************************************************************/
/*                                              Include Files                                                                         */
/********************************************************************************/
#include "datatype.h"
#include "Board.h"
#include "menudef.h"
#include "FMenuPropStr.h"
#include "menufunc.h"

//extern MenuItemActionType code NothingKeyEvent[];
//extern MenuItemActionType code RootKeyEvent[];
//extern MenuItemActionType code NaviExecNextKeyEvent[];
//extern MenuItemActionType code NothingInputKeyEvent[];
/********************************************************************************/
/*                                              Define                                                                                 */
/********************************************************************************/
/********************************************************************************/
/*                                              Data Type                                                                            */
/********************************************************************************/
/// Menu Key Action Type Definition========================================
MenuItemActionType code FactoryNothingKeyEvent[BTN_END_BTN]=
{ MIA_NOTHING,  // BTN_PLUS
  MIA_NOTHING,  // BTN_MINUS
  MIA_NOTHING,  // BTN_MENU
  MIA_NOTHING,  // BTN_EXIT
  MIA_POWER     // BTN_POWER
};

MenuItemActionType code FactoryNaviKeyEvent[BTN_END_BTN]=
{ MIA_PREV_ITEM, //MIA_NEXT_ITEM, 	// BTN_PLUS
  MIA_NEXT_ITEM, //MIA_PREV_ITEM, 	// BTN_MINUS
  MIA_GOTO_ADJUST_ITEM,	// BTN_MENU
  MIA_FACTORY_EXIT,	 	// BTN_EXIT
  MIA_POWER		// BTN_POWER
};
MenuItemActionType code FactoryNaviExecKeyEvent[BTN_END_BTN]=
{ MIA_PREV_ITEM, //MIA_NEXT_ITEM, 	// BTN_PLUS
  MIA_NEXT_ITEM, //MIA_PREV_ITEM, 	// BTN_MINUS
  MIA_EXEC_FUNC,	// BTN_MENU
  MIA_NOTHING,	 	// BTN_EXIT
  MIA_POWER		// BTN_POWER
};
MenuItemActionType code FactoryAdjustKeyEvent[BTN_END_BTN]=
{ MIA_INC_VALUE, 	// BTN_PLUS
  MIA_DEC_VALUE, 	// BTN_MINUS
  MIA_NEXT_ITEM,		// BTN_MENU
  MIA_NOTHING,	 	// BTN_EXIT
  MIA_POWER		// BTN_POWER
};
MenuItemActionType code FactoryAdjustStayKeyEvent[BTN_END_BTN]=
{ MIA_INC_VALUE, 	// BTN_PLUS
  MIA_DEC_VALUE, 	// BTN_MINUS
  MIA_RETURN_SELECT_ITEM,	// BTN_MENU
  MIA_RETURN_SELECT_ITEM,	 	// BTN_EXIT
  MIA_POWER		// BTN_POWER
};
MenuItemActionType code FactoryNaviExitKeyEvent[BTN_END_BTN]=
{ MIA_PREV_ITEM, //MIA_NEXT_ITEM, 	// BTN_PLUS
  MIA_NEXT_ITEM, //MIA_PREV_ITEM, 	// BTN_MINUS
  MIA_FACTORY_EXIT,	// BTN_MENU
  MIA_FACTORY_EXIT,	 	// BTN_EXIT
  MIA_POWER		// BTN_POWER
};

MenuItemActionType code FactoryResetBacklightKeyEvent[BTN_END_BTN]=
{ MIA_PREV_ITEM, //MIA_NEXT_ITEM, 	// BTN_PLUS
  MIA_NEXT_ITEM, //MIA_PREV_ITEM, 	// BTN_MINUS
  MIA_RESET_BACKLIGHT,	// BTN_MENU
  MIA_NOTHING,	 	// BTN_EXIT
  MIA_POWER		// BTN_POWER
};

//**********20120508 Chihlung********

MenuItemActionType code FactoryNothingInputKeyEvent[BTN_END_BTN]=
{ MIA_HotInput,       // BTN_PLUS
  MIA_HotInput,       // BTN_MINUS
  MIA_HotInput,       // BTN_MENU
  MIA_HotInput,       // BTN_EXIT
  MIA_POWER             // BTN_POWER
};

//===========================================================

//==============================================================================================
//================ JL ==============================================================================
//====================================================================================
DrawNumberType code DrawRedColor6500KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(9,	8),	GetRColor6500KValue, 0}
};
DrawNumberType code DrawGreenColor6500KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(15,	8),	GetGColor6500KValue, 0}
};
DrawNumberType code DrawBlueColor6500KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(21,	8),	GetBColor6500KValue, 0}
};

//====================================================================================
DrawNumberType code DrawRedColor9300KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	 COORD(9,  9),	GetRColor9300KValue, 0}
};
DrawNumberType code DrawGreenColor9300KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(15,	9),	GetGColor9300KValue, 0}
};
DrawNumberType code DrawBlueColor9300KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(21,	9),	GetBColor9300KValue, 0}
};
//====================================================================================
#if Enable_ColorTemp_5700K
DrawNumberType code DrawRedColor5700KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	 COORD(9,   10),	GetRColor5700KValue, 0}
};
DrawNumberType code DrawGreenColor5700KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(15,	10),	GetGColor5700KValue, 0}
};
DrawNumberType code DrawBlueColor5700KNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(21,	10),	GetBColor5700KValue, 0}
};
#endif

#if ENABLE_SRGB_FACTORY_ITEM//ENABLE_SRGB_ITEM //Sky120318//Chihlung 20120321
//====================================================================================
DrawNumberType code DrawRedColorsRGBNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(9,	11),	GetRColorsRGBValue, 0}
};
DrawNumberType code DrawGreenColorsRGBNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(15,	11),	GetGColorsRGBValue, 0}
};
DrawNumberType code DrawBlueColorsRGBNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(21,	11),	GetBColorsRGBValue, 0}
};
#endif

//==============================================================================================
DrawNumberType code DrawSubBrightnessNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(10,	12+ENABLE_SRGB_FACTORY_ITEM*3),	GetFSubBrightnessValue, 0}
};
//========================================================================================
#if ENABLE_VGA_INPUT
#if 1
#define GAIN_NUM_FLAGS  NUM_4NUMS
#define GAIN_NUM_OFFSET (-1)
#else
#define GAIN_NUM_FLAGS  0
#define GAIN_NUM_OFFSET 0
#endif
DrawNumberType code DrawGainRNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END|GAIN_NUM_FLAGS,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(9+GAIN_NUM_OFFSET,	6),	GetRedGainValue, 0}
};
DrawNumberType code DrawGainGNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END|GAIN_NUM_FLAGS,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(15+GAIN_NUM_OFFSET,	6),	GetGreenGainValue, 0}
};
DrawNumberType code DrawGainBNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END|GAIN_NUM_FLAGS,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(21+GAIN_NUM_OFFSET,	6),	GetBlueGainValue, 0}
};
#undef GAIN_NUM_FLAGS
#undef GAIN_NUM_OFFSET
#endif

#if 0//CHIP_ID!=CHIP_TSUMT
DrawNumberType code DrawOffsetRNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(9,	5),	GetAdcRedOffsetValue, 0}
};
DrawNumberType code DrawOffsetGNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(15,	5),	GetAdcGreenOffsetValue, 0}
};
DrawNumberType code DrawOffsetBNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(21,	5),	GetAdcBlueOffsetValue, 0}
};
#endif

//====================================================================================
RadioTextType code BurninRadioText[]=
{ // u8Flags,		u8XPos,	u8YPos,	DisplayText
  {TXT_END,	COORD(12,12+ENABLE_SRGB_FACTORY_ITEM*3),	BurninOnOffText},
};
DrawRadioGroupType code DrawBurninRadioGroup[]=
{ //u8ForeColor,	u8BackColor,		GetValue	pstRadioText
  {MONO_COLOR(CP_RED,CP_BLUE), MONO_COLOR(CP_RED,CP_BLUE),	 NULL,	BurninRadioText}
};

RadioTextType code FEDIDWPRatioText[]=
{ // u8Flags,		u8XPos,	u8YPos,	DisplayText
  {TXT_END,	COORD(12,13+ENABLE_SRGB_FACTORY_ITEM*3),	FEDIDWPOnOffText},
};
DrawRadioGroupType code DrawFEDIDWPRadioGroup[]=
{ //u8ForeColor,	u8BackColor,		GetValue	pstRadioText
  {MONO_COLOR(CP_RED,CP_BLUE), MONO_COLOR(CP_RED,CP_BLUE),	 NULL,	FEDIDWPRatioText}
};
#if 1//ENABLE_ENERGY_STAR_LOGO
//Sky101029 ES logo on/off
RadioTextType code LogoRatioText[]=
{ // u8Flags,		u8XPos,	u8YPos,	DisplayText
  {TXT_END,	COORD(12,14+ENABLE_SRGB_FACTORY_ITEM*3),	LogoOnOffText},
};

DrawRadioGroupType code DrawLogoRadioGroup[]=
{ //u8ForeColor,	u8BackColor,		GetValue	pstRadioText
  {MONO_COLOR(CP_RED,CP_BLUE), MONO_COLOR(CP_RED,CP_BLUE),	 NULL,	LogoRatioText}
};
#endif

#if ENABLE_FACTORY_SSCADJ

DrawNumberType code DrawSSCFrequencyNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE), COORD(18, 15+ENABLE_SRGB_FACTORY_ITEM*3), GetSSCFrequencyValue, 0}
};

DrawNumberType code DrawSSCPermillageNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE), COORD(18, 16+ENABLE_SRGB_FACTORY_ITEM*3), GetSSCPermillageValue, 0}
};

DrawNumberType code DrawDDRSSCFrequencyNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE), COORD(18, 17+ENABLE_SRGB_FACTORY_ITEM*3), GetDDRSSCFrequencyValue, 0}
};

DrawNumberType code DrawDDRSSCPermillageNumber[]=
{ // u8Flags,	u8Color,	u8XPos,	u8YPos,	GetValue,u8PropNumAddr
  {NUM_END,	MONO_COLOR(CP_RED,CP_BLUE), COORD(18, 18+ENABLE_SRGB_FACTORY_ITEM*3), GetDDRSSCPermillageValue, 0}
};

#endif

//====================================================================================
#if 0
RadioTextType code BankNoText[]=
{ // u8Flags,		u8XPos,	u8YPos,	DisplayText
  {TXT_END,		COORD(6,19+ENABLE_SRGB_FACTORY_ITEM+ENABLE_COLOR_TEMP_OSD*3),	BankNameText}
};
DrawRadioGroupType code DrawBankNoRadioGroup[]=
{ //u8ForeColor,	u8BackColor,		GetValue	pstRadioText
  {MONO_COLOR(CP_RED,CP_BLUE), MONO_COLOR(CP_RED,CP_BLUE),	 NULL,	BankNoText}
};
DrawNumberType code DrawAddrNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_HEX,	MONO_COLOR(CP_RED,CP_BLUE),	 COORD(6,	20+ENABLE_SRGB_FACTORY_ITEM*3),	GetAddrValue},
  {NUM_HEX|NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(17,	20+ENABLE_SRGB_FACTORY_ITEM*3),	GetAddrRegValue}
};

DrawNumberType code DrawAddrRegValueNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_HEX|NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(17,	20+ENABLE_SRGB_FACTORY_ITEM*3),	GetRegValue}
};

DrawNumberType code DrawRegValueNumber[]=
{ // u8Flags,	u8ForeColor,	u8BackColor,	u8XPos,	u8YPos,	GetValue
  {NUM_HEX|NUM_END,	MONO_COLOR(CP_RED,CP_BLUE),	COORD(17,	20+ENABLE_SRGB_FACTORY_ITEM*3),	GetAddrRegValue}
};
#endif
//====================================================================================
RadioTextType code FFactoryText[]=
{ // u8Flags,		u8XPos,	u8YPos,	DisplayText

  {0,   COORD(1, 0),     FWVerSionText},

  {0,   COORD(1, 1),     F_ChipText},
  {0,   COORD(19, 1),   DateText},
  
  {0,   COORD(1, 2),     F_PanelText},
  {0,   COORD(7, 2),     F_PanelNameText},
  
  {0,   COORD(1, 3),     F_PanelSizeText},
  {0,   COORD(7, 3),     F_PanelSizeNumber},


#if ENABLE_VGA_INPUT
  {0,   COORD(1,5),	AdcOffsetText},
  {0,   COORD(1,6),	AdcGainText},
#endif
  {0,   COORD(1,7),     ColorTempText},

  {0,   COORD(1,8),     FNormalText},
  {0,   COORD(1,9),     FCoolText},
#if Enable_ColorTemp_5700K  
  {0,   COORD(1,10),    FWarmText},
#endif  
  #if ENABLE_SRGB_FACTORY_ITEM//Sky120318//Chihlung 20120321
  {0,   COORD(1,11),    FsRGBText},
  #endif

  {TXT_END,  COORD(1,6),	NULL}
};
DrawRadioGroupType code DrawFactoryRadioGroup[]=
{ //u8ForeColor,	u8BackColor,		GetValue	pstRadioText
  {MONO_COLOR(CP_WHITE,CP_BLUE),	MONO_COLOR(CP_WHITE,CP_BLUE),  NULL,	FFactoryText}
};


/// Menu Items=====================================================

RadioTextType code FBurnInText[]=
{ // u8Flags,		u8XPos,	u8YPos,	DisplayText


  {0,   COORD(1, 0),     FWVerSionText},

  {0,   COORD(1, 1),     F_ChipText},
  {0,   COORD(19, 1),   DateText},

  
  {0,   COORD(1, 2),     F_PanelText},
  {0,   COORD(7, 2),     F_PanelNameText},
  
  {0,   COORD(1, 4),     F_PanelSizeText},
  {0,   COORD(7, 4),     F_PanelSizeNumber},
 
  {0,   COORD(1, 6),       BacklightTimeText},
  {0,   COORD(17, 6),     HourText},
  {0,   COORD(24, 6),     MinText},

  
  {TXT_END,COORD(1,6),	NULL}
};

DrawRadioGroupType code DrawFactoryBurnInRadioGroup[]=
{ //u8ForeColor,	u8BackColor,		GetValue	pstRadioText
  {MONO_COLOR(CP_WHITE,CP_BLUE),	MONO_COLOR(CP_WHITE,CP_BLUE),  NULL,	FBurnInText}
};

MenuItemType code BurninMenuItems[]=
{ // virtual item

#if defined(KTC_Project)

 // Exit
  { COORD(20,3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8TXTFlags;
    KeyTestText, // DisplayText;
    FactoryNaviTestKeyEvent,
    { NULL,	// AdjustFunction
      NULL,// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },

  // Exit
  { COORD(18, 4), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8TXTFlags;
    KeyTestText, // DisplayText;
    FactoryNaviTestKeyEvent,
    { NULL,	// AdjustFunction
      NULL,// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },

   // Exit
  { COORD(20, 5), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8TXTFlags;
    KeyTestText, // DisplayText;
    FactoryNaviTestKeyEvent,
    { NULL,	// AdjustFunction
      NULL,// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },

#endif
  { COORD(0, 1), // u8XPos, u8YPos;
    MONO_COLOR(0, 1), // u8ForeColor, u8BackColor;
    MONO_COLOR(0, 1), // u8SelForeColor, u8SelBackColor;
    MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8TXTFlags;
    NULL, // DisplayText;
    FactoryNothingInputKeyEvent, //FactoryNothingKeyEvent
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawFactoryBurnInRadioGroup,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0//MIB_SELECTABLE // u8Flags
  }
};



//******************************************************************************

#if ENABLE_VGA_INPUT
MenuItemType code AutoColorMenuItems[]=
{//0 Position
  { COORD(0, 2), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8SelForeColor, u8SelBackColor;
    MENU_ROOT, //u8NextMenuPage;
    TXT_CENTER,// u8TXTFlags;
    AutoColorText, // DisplayText;
    FactoryNothingKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// *stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0//MIB_SELECTABLE // u8Flags
  }
};
#endif

MenuItemType code FactoryMenuItems[]=
{


#if ENABLE_VGA_INPUT
  // Auto Color
  { COORD(1, (4+FMI_Item_AutoColor)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FAutoColorText, // DisplayText;
    FactoryNaviExecKeyEvent,
    { NULL,	// AdjustFunction
      AutoColor// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
#endif
  //=================================================================


#if ENABLE_VGA_INPUT
  // Gain R
  { COORD(8, (4+FMI_Item_RGain_Adj)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryRedText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Gain R adjust
  { COORD(10, (4+FMI_Item_RGain_Adj)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustRedGain,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawGainRNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Gain G
  { COORD(14, (4+FMI_Item_RGain_Adj)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryGreenText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Gain G adjust
  { COORD(16, (4+FMI_Item_RGain_Adj)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustGreenGain,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawGainGNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Gain B
  { COORD(20, (4+FMI_Item_RGain_Adj)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryBlueText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Gain B adjust
  { COORD(20, (4+FMI_Item_RGain_Adj)), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustBlueGain,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawGainBNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
#endif
#if 1
/**************************************** 6500K ****************************************/
  // Desktop R
  { COORD(8, 8), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryRedText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Desktop R adjust
  { COORD(10, 8), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustRedColor6500K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawRedColor6500KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Desktop G
  { COORD(14, 8), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryGreenText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Desktop G adjust
  { COORD(16, 8), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustGreenColor6500K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawGreenColor6500KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Desktop B
  { COORD(20, 8), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryBlueText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Desktop  adjust
  { COORD(20, 8), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustBlueColor6500K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawBlueColor6500KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0// u8Flags
  },
#endif

/**************************************** Cool (9300K) ****************************************/
  // Cool (9300K) R
  { COORD(8, 9), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryRedText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Cool (9300K) R adjust
  { COORD(10, 9), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustRedColor9300K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawRedColor9300KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Cool (9300K) G
  { COORD(14, 9), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryGreenText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Cool (9300K) G adjust
  { COORD(16, 9), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustGreenColor9300K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawGreenColor9300KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Cool (9300K) B
  { COORD(20, 9), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryBlueText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Cool (9300K) B adjust
  { COORD(20, 9), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustBlueColor9300K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawBlueColor9300KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0// u8Flags
  },
  #if Enable_ColorTemp_5700K
/**************************************** Warm (5700K) ****************************************/
   // Warm 5700K R
  { COORD(8, 10), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryRedText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Warm 5700K R adjust
  { COORD(10, 10), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustRedColor5700K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawRedColor5700KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Warm 5700K G
  { COORD(14, 10), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryGreenText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Warm 5700K G adjust
  { COORD(16, 10), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustGreenColor5700K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawGreenColor5700KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Warm 5700K B
  { COORD(20, 10), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryBlueText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Warm 5700K B adjust
  { COORD(20, 10), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustBlueColor5700K,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawBlueColor5700KNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0// u8Flags
  },
 #endif 
  //=================================================================
#if 0//ENABLE_SRGB_FACTORY_ITEM //Sky120318//Chihlung 20120321
   // sRGB R
  { COORD(8, 11), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryRedText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // sRGB R adjust
  { COORD(10, 11), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustRedColorsRGB,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawRedColorsRGBNumber,// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // sRGB G
  { COORD(14, 11), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryGreenText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // sRGB G adjust
  { COORD(16, 11), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustGreenColorsRGB,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawGreenColorsRGBNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // sRGB B
  { COORD(20, 11), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryBlueText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // sRGB B adjust
  { COORD(20, 11), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustBlueColorsRGB,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawBlueColorsRGBNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0// u8Flags
  },
#endif

  // Burnin
  { COORD(1, 12+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    BurninText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Burnin Adjust
  { COORD(1, 12+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustBurnin,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawBurninRadioGroup,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
    // EDIDWP
  { COORD(1, 13+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    EDIDWPText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // EDIDWP Adjust
  { COORD(1, 13+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustFacEDIDWP,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawFEDIDWPRadioGroup,
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  #if 1//ENABLE_ENERGY_STAR_LOGO
  { COORD(1, 14+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FESLogoText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // ES Logo Adjust
  { COORD(1, 14+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    0,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustLogoDisplay,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawLogoRadioGroup,
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
#endif

#if ENABLE_FACTORY_SSCADJ
// SSCFreq Fre
    { 
    	COORD(1, 15+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
       MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    	FACTORY_MENU, //u8NextMenuPage; 
	TXT_RealText,// DrawItemMethod;
      	F_PanelSSCFreqText, // DisplayText;
      FactoryNaviKeyEvent,
      { NULL,	// AdjustFunction
        NULL// ExecFunction
      },// DisplayValue;
      { NULL,	// DrawFactoryNumberType
        NULL,	// DrawGaugeType
        NULL,	// DrawFactoryRadioGroupType
        NULL	 //DrawIconType
      },
      MIB_SELECTABLE // Flags
    },
    //SSCFreq adjust
    { 
    	COORD(1, 15+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
       MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
       FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
       0,// u8Flags;
       NULL, // DisplayText;
      FactoryAdjustKeyEvent,
      { AdjustSSCFrequency,	// AdjustFunction
        NULL// ExecFunction
      },// DisplayValue;
      { DrawSSCFrequencyNumber,	// DrawFactoryNumberType
        NULL,	// DrawGaugeType
        NULL,	// DrawFactoryRadioGroupType
        NULL	 //DrawIconType
      },
      0 // Flags
    },
    // SSCPermill
    { 
    	COORD(1, 16+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
       MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    	FACTORY_MENU, //u8NextMenuPage; 
	TXT_RealText,// DrawItemMethod;
      	F_PanelSSCPermillageText, // DisplayText;
      FactoryNaviKeyEvent,
      { NULL,	// AdjustFunction
        NULL// ExecFunction
      },// DisplayValue;
      { NULL,	// DrawFactoryNumberType
        NULL,	// DrawGaugeType
        NULL,	// DrawFactoryRadioGroupType
        NULL	 //DrawIconType
      },
      MIB_SELECTABLE // Flags
    },
    //SSCFreq adjust
    { 
    	COORD(1, 16+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
       MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
       FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
       0,// u8Flags;
       NULL, // DisplayText;
      FactoryAdjustKeyEvent,
      { AdjustSSCPermillage,	// AdjustFunction
        NULL// ExecFunction
      },// DisplayValue;
      { DrawSSCPermillageNumber,	// DrawFactoryNumberType
        NULL,	// DrawGaugeType
        NULL,	// DrawFactoryRadioGroupType
        NULL	 //DrawIconType
      },
      0 // Flags
    },

//DDR SSCFreq Fre
    { 
    	COORD(1, 17+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
       MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    	FACTORY_MENU, //u8NextMenuPage; 
	TXT_RealText,// DrawItemMethod;
      	F_DDRSSCFreqText, // DisplayText;
      FactoryNaviKeyEvent,
      { NULL,	// AdjustFunction
        NULL// ExecFunction
      },// DisplayValue;
      { NULL,	// DrawFactoryNumberType
        NULL,	// DrawGaugeType
        NULL,	// DrawFactoryRadioGroupType
        NULL	 //DrawIconType
      },
      MIB_SELECTABLE // Flags
    },
    //DDR SSCFreq adjust
    { 
    	COORD(1, 17+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
       MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
       FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
       0,// u8Flags;
       NULL, // DisplayText;
      FactoryAdjustKeyEvent,
      { AdjustDDRSSCFrequency,	// AdjustFunction
        NULL// ExecFunction
      },// DisplayValue;
      { DrawDDRSSCFrequencyNumber,	// DrawFactoryNumberType
        NULL,	// DrawGaugeType
        NULL,	// DrawFactoryRadioGroupType
        NULL	 //DrawIconType
      },
      0 // Flags
    },
    // DDR SSCPermill
    { 
    	COORD(1, 18+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
       MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    	FACTORY_MENU, //u8NextMenuPage; 
	TXT_RealText,// DrawItemMethod;
      	F_DDRSSCPermillageText, // DisplayText;
      FactoryNaviKeyEvent,
      { NULL,	// AdjustFunction
        NULL// ExecFunction
      },// DisplayValue;
      { NULL,	// DrawFactoryNumberType
        NULL,	// DrawGaugeType
        NULL,	// DrawFactoryRadioGroupType
        NULL	 //DrawIconType
      },
      MIB_SELECTABLE // Flags
    },
    //DDR SSCFreq adjust
    { 
    	COORD(1, 18+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
       MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
       MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
       FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
       0,// u8Flags;
       NULL, // DisplayText;
      FactoryAdjustKeyEvent,
      { AdjustDDRSSCPermillage,	// AdjustFunction
        NULL// ExecFunction
      },// DisplayValue;
      { DrawDDRSSCPermillageNumber,	// DrawFactoryNumberType
        NULL,	// DrawGaugeType
        NULL,	// DrawFactoryRadioGroupType
        NULL	 //DrawIconType
      },
      0 // Flags
    },
		
#endif  



/*
  //reset backlight timer
   { COORD(1, 21), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    ResetBacklightText, // DisplayText;
    FactoryResetBacklightKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  */

#if 0
  // Bank
  { COORD(1, 19+ENABLE_SRGB_FACTORY_ITEM+ENABLE_COLOR_TEMP_OSD*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    0,// u8Flags;
    BankText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Bank Adjust
  { COORD(2, 19+ENABLE_SRGB_FACTORY_ITEM+ENABLE_COLOR_TEMP_OSD*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    0,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustBankNo,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      DrawBankNoRadioGroup,// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },
  // Address
  { COORD(1, 20+ENABLE_SRGB_FACTORY_ITEM+ENABLE_COLOR_TEMP_OSD*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    0,// u8Flags;
    AddressText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // Address Adjust
  { COORD(1, 20+ENABLE_SRGB_FACTORY_ITEM+ENABLE_COLOR_TEMP_OSD*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    0,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { AdjustAddr,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { DrawAddrNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
     0 // u8Flags
  },
  // ValueText
  { COORD(11, 20+ENABLE_SRGB_FACTORY_ITEM+ENABLE_COLOR_TEMP_OSD*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    0,// u8Flags;
    ValueText, // DisplayText;
    FactoryNaviKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  // ValueText Adjust
  { COORD(11, 20+ENABLE_SRGB_FACTORY_ITEM+ENABLE_COLOR_TEMP_OSD*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    0,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustStayKeyEvent,
    { AdjustRegValue,	// AdjustFunction
      WriteRegValue// ExecFunction
    },// stDisplayValue;
    { DrawAddrRegValueNumber,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
      0 // u8Flags
  },
  #if 1 //White120131,update BID_Counter_for_FY13
  //ResetBID    //White111107,BID_Counter_for_FY13
  { COORD(1, 21+ENABLE_SRGB_FACTORY_ITEM+ENABLE_COLOR_TEMP_OSD*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    0,// u8Flags;
    ResetBIDText, // DisplayText;
    FactoryNaviExecKeyEvent,
    { NULL,	// AdjustFunction
      ResetBIDCounter// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
  #endif
#endif


#if F_ResetItem
  //reset backlight timer
   { COORD(1, 22), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    FactoryResetText, // DisplayText;
    FactoryResetKeyEvent,
    { NULL,	// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },


 // Exit
  { COORD(1, 23+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8TXTFlags;
    ExitText, // DisplayText;
    FactoryNaviExitKeyEvent,
    { NULL,	// AdjustFunction
      NULL,// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },

#else

 // Exit
  { COORD(1, 22+ENABLE_SRGB_FACTORY_ITEM*3), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8TXTFlags;
    ExitText, // DisplayText;
    FactoryNaviExitKeyEvent,
    { NULL,	// AdjustFunction
      NULL,// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL	 //DrawIconType
    },
    MIB_SELECTABLE // u8Flags
  },
#endif


  // Descript
  { COORD(20, 10), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BLUE), // u8ForeColor, u8BackColor;
    MONO_COLOR(CP_RED,CP_BLUE), // u8SelForeColor, u8SelBackColor;
    FACTORY_MENU,//MENU_ROOT, //u8NextMenuPage;
    TXT_RealText,// u8Flags;
    NULL, // DisplayText;
    FactoryAdjustKeyEvent,
    { NULL,// AdjustFunction
      NULL// ExecFunction
    },// stDisplayValue;
    { NULL,// DrawNumberType
      NULL,	// DrawGaugeType
      DrawFactoryRadioGroup,//DrawRadioGroupType
      NULL	 //DrawIconType
    },
    0 // u8Flags
  },

};

MenuPageType code tblFMenus[]=
{
  // 0 BURNIN_MENU
  { SIZE(28, 8), // 	u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    BurninMenuItems, // pstMenuItems;
    sizeof(BurninMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    EnableBurninMode, // ExecFunction;
    FactoryMenuLoadFont, // LoadFont
    MPB_STAY//   u8Flags;
  },
#if ENABLE_VGA_INPUT
  // 1 AUTO_COLOR_MENU
  { SIZE(24, 5), // 	u8XSize, u8YSize;
    MENU_ROOT,// u8PrevMenuPage;
    AutoColorMenuItems, // pstMenuItems;
    sizeof(AutoColorMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    AutoColor, // ExecFunction;
    NULL, // LoadFont
    0 //   u8Flags;
  },
#endif
  // 2 FACTORY_MENU
  { SIZE(28, 24+ENABLE_SRGB_FACTORY_ITEM*3), // 	u8XSize, u8YSize; //White111107 resize for FY13 request ,BID_Counter_for_FY13 //X resize 28 ,same as other SI
    MENU_ROOT,// u8PrevMenuPage;
    FactoryMenuItems, // pstMenuItems;
    sizeof(FactoryMenuItems)/sizeof(MenuItemType), // u8MenuItemCount;
    NULL, // ExecFunction;//Sky120321
    FactoryMenuLoadFont, // LoadFont
    MPB_STAY //   u8Flags;
  },

};

#endif ///   _FMENUTBL_H
