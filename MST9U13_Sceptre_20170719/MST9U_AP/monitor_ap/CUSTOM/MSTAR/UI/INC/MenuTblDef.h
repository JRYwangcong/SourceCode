///////////////////////////////////////////////////////////////////////////////
/// @file MenuTblDef.h
/// @brief Head file to define menu structure data.
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///  -
///////////////////////////////////////////////////////////////////////////////

#ifndef _MENUTBLDEF_H
#define _MENUTBLDEF_H

#include "menudef.h"
#include "MenuPropStr.h"
#include "PropFontExtFunc.h"

MenuItemActionType code AllNothingKeyEvent[BTN_END_BTN]= //Jison 100323
{ MIA_NOTHING,  // BTN_PLUS
  MIA_NOTHING,  // BTN_MINUS
  MIA_NOTHING,  // BTN_MENU
  MIA_NOTHING,  // BTN_EXIT
  MIA_NOTHING,   // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};
MenuItemActionType code NothingKeyEvent[BTN_END_BTN]=
{ MIA_NOTHING,  // BTN_PLUS
  MIA_NOTHING,  // BTN_MINUS
  MIA_NOTHING,  // BTN_MENU
  MIA_NOTHING,  // BTN_EXIT
  MIA_POWER,     // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code StandbyKeyEvent[BTN_END_BTN]=
{ MIA_NOTHING,  // BTN_PLUS
  MIA_NOTHING,  // BTN_MINUS
  MIA_NOTHING,  // BTN_MENU
  MIA_HotInput,        // BTN_EXIT
  MIA_POWER,             // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code NothingInputKeyEvent[BTN_END_BTN]=
{ MIA_NOTHING,          // BTN_PLUS
  MIA_NOTHING,          // BTN_MINUS
  MIA_NOTHING,          // BTN_MENU
  MIA_HotInput,        // BTN_EXIT
  MIA_POWER,             // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};




MenuItemActionType code RootKeyEvent[BTN_END_BTN]=
{
  MIA_HotPictureMode,        // BTN_PLUS
  MIA_HOTCROSS,        // BTN_MINUS
  MIA_GOTO_NEXT,        // BTN_MENU
  MIA_HotInput,        // BTN_EXIT
  MIA_POWER,             // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};
MenuItemActionType code HOTKeyEvent[BTN_END_BTN]=
{ MIA_HotPictureMode,    // BTN_PLUS
  MIA_HOTCROSS,    // BTN_MINUS
  MIA_GOTO_PREV,    // BTN_MENU
  MIA_GOTO_PREV,    // BTN_EXIT
  MIA_POWER,         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};
MenuItemActionType code HOTAdjustKeyEvent[BTN_END_BTN]=
{ MIA_INC_VALUE,    // BTN_PLUS
  MIA_DEC_VALUE,    // BTN_MINUS
  MIA_NOTHING,    // BTN_MENU
  MIA_GOTO_PREV,    // BTN_EXIT
  MIA_POWER,         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};


MenuItemActionType code AdjusterKeyHotKeyGameMenuEvent[BTN_END_BTN]=
{ 
   MIA_NEXT_ITEM,    // BTN_PLUS
   MIA_PREV_ITEM,    // BTN_MINUS
   MIA_GOTO_NEXT,    // BTN_MENU
   MIA_GOTO_PREV,    // BTN_EXIT
   MIA_POWER, 		// BTN_Power
   BTN_INCVALUE,		//  BTN_IncVal
   BTN_DECVALUE,		//  BTN_DecVal

};

MenuItemActionType code RestKeyEvent[BTN_END_BTN]=
{
  MIA_NOTHING,        // BTN_PLUS
  MIA_NOTHING,        // BTN_MINUS
  MIA_GOTO_NEXT,        // BTN_MENU
  MIA_GOTO_NEXT,        // BTN_EXIT
  MIA_POWER,             // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code SwitchSourceKeyEvent[BTN_END_BTN]=
{ MIA_NOTHING,          // BTN_PLUS
  MIA_NOTHING,          // BTN_MINUS
  MIA_GOTO_NEXT,          // BTN_MENU
  MIA_GOTO_PREV,    // BTN_EXIT
  MIA_POWER,             // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};


//For Navigating ---Start--------------------------------------------
MenuItemActionType code NaviExecPrevKeyEvent[BTN_END_BTN]=
{ 
  MIA_NEXT_ITEM,    // BTN_PLUS
  MIA_PREV_ITEM,    // BTN_MINUS
  MIA_EXEC_AND_GOTO_PREV,//MIA_NOTHING,              // BTN_MENU //Jison 080423
  MIA_GOTO_PREV,   // BTN_EXIT
  MIA_POWER,                 // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code NaviKeyEvent[BTN_END_BTN]=
{ 
  MIA_PREV_ITEM,    // BTN_PLUS
  MIA_NEXT_ITEM,    // BTN_MINUS
  MIA_GOTO_NEXT,    // BTN_MENU
  MIA_GOTO_PREV,    // BTN_EXIT
  MIA_POWER,         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};


MenuItemActionType code HOTKeySleepModeEvent[BTN_END_BTN]=
{ MIA_NOTHING,    // BTN_PLUS
  MIA_NOTHING,    // BTN_MINUS
  MIA_NOTHING,    // BTN_MENU
  MIA_GOTO_PREV,    // BTN_EXIT
  MIA_POWER,         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};


MenuItemActionType code NaviAdjusterKeyEvent[BTN_END_BTN]=
{ 
  MIA_PREV_ITEM,    // BTN_PLUS
  MIA_NEXT_ITEM,    // BTN_MINUS
  MIA_GOTO_ADJUST_ITEM,        // BTN_MENU
  MIA_GOTO_PREV, // BTN_EXIT
  MIA_POWER,             // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code NaviExecFuncKeyEvent[BTN_END_BTN]=
{ 
  MIA_PREV_ITEM,    // BTN_PLUS
  MIA_NEXT_ITEM,    // BTN_MINUS
  MIA_EXEC_FUNC,        // BTN_MENU
  MIA_GOTO_PREV,        // BTN_EXIT
  MIA_POWER,             // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code NaviAdjuster1KeyEvent[BTN_END_BTN]=
{ 
  MIA_PREV_ITEM,    // BTN_PLUS
  MIA_NEXT_ITEM,    // BTN_MINUS
  MIA_GOTO_ADJUST_ITEM,        // BTN_MENU
  MIA_GOTO_PREV1, // BTN_EXIT
  MIA_POWER,             // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};



/*
MenuItemActionType code NaviExecGotoKeyEvent[BTN_END_BTN]=
{ MIA_PREV_ITEM,    // BTN_PLUS
  MIA_NEXT_ITEM,    // BTN_MINUS
  MIA_GOTO_PREV,//MIA_NOTHING,      // BTN_MENU //Jison 080423
  MIA_EXEC_GOTO,    // BTN_EXIT
  MIA_POWER         // BTN_POWER
};
*/
//For Navigating ---End--------------------------------------------

//For Adjuster  ---Start-------------------------------------

MenuItemActionType code AdjusterNaviKeyEvent[BTN_END_BTN]=
{ MIA_INC_VALUE,                    // BTN_PLUS
  MIA_DEC_VALUE,                    // BTN_MINUS
  MIA_RETURN_SELECT_ITEM,      // BTN_MENU
  MIA_UNDO_RETURN_SELECT_ITEM,           // BTN_EXIT
  MIA_POWER,                         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};
MenuItemActionType code AdjusterReverseNaviKeyEvent[BTN_END_BTN]=
{ MIA_DEC_VALUE,                    // BTN_PLUS
  MIA_INC_VALUE,                    // BTN_MINUS
  MIA_RETURN_SELECT_ITEM,      // BTN_MENU
  MIA_UNDO_RETURN_SELECT_ITEM,           // BTN_EXIT
  MIA_POWER,                         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code AdjusterFuncNaviKeyEvent[BTN_END_BTN]=
{ MIA_INC_VALUE,                    // BTN_PLUS
  MIA_DEC_VALUE,                    // BTN_MINUS
  MIA_EXEC_FUNC,      // BTN_MENU
  MIA_UNDO_RETURN_SELECT_ITEM,           // BTN_EXIT
  MIA_POWER,                         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code AdjusterReverseFuncNaviKeyEvent[BTN_END_BTN]=
{ MIA_DEC_VALUE,                    // BTN_PLUS
  MIA_INC_VALUE,                    // BTN_MINUS
  MIA_EXEC_FUNC,      // BTN_MENU
  MIA_UNDO_RETURN_SELECT_ITEM,           // BTN_EXIT
  MIA_POWER,                         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code NaviExecNextKeyEvent[BTN_END_BTN]=
{ MIA_INC_VALUE,                    // BTN_PLUS
  MIA_DEC_VALUE,                    // BTN_MINUS
  MIA_EXEC_AND_GOTO_NEXT,            // BTN_MENU
  MIA_UNDO_RETURN_SELECT_ITEM,   // BTN_EXIT
  MIA_POWER,                 // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code InvAdjusterNaviKeyEvent[BTN_END_BTN]=
{ MIA_DEC_VALUE,                    // BTN_PLUS
  MIA_INC_VALUE,                    // BTN_MINUS
  MIA_RETURN_SELECT_ITEM,      // BTN_MENU
  MIA_UNDO_RETURN_SELECT_ITEM,           // BTN_EXIT
  MIA_POWER,                         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};

MenuItemActionType code AdjusterExecGotoNextReturnKeyEvent[BTN_END_BTN]=
{ MIA_INC_VALUE,                    // BTN_PLUS
  MIA_DEC_VALUE,                    // BTN_MINUS
  MIA_EXEC_GOTO_NEXT_RETURN,      // BTN_MENU
  MIA_UNDO_RETURN_SELECT_ITEM,        // BTN_EXIT
  MIA_POWER,                         // BTN_POWER
  MIA_NOTHING,  // BTN_INCVALUE
  MIA_NOTHING   // BTN_DECVALUE
};



MenuItemActionType code GuideKeyEvent[BTN_END_BTN]=
{
  MIA_GOTO_NEXT,        // BTN_PLUS
  MIA_GOTO_NEXT,        // BTN_MINUS
  MIA_GOTO_NEXT,        // BTN_MENU
  MIA_GOTO_NEXT,        // BTN_EXIT
  MIA_GOTO_NEXT,             // BTN_POWER
  MIA_GOTO_NEXT,  // BTN_INCVALUE
  MIA_GOTO_NEXT    // BTN_DECVALUE
};

//===========================================================
MenuItemType code PowerOffMenuItems[]=
{ { COORD(0, 1), // u8XPos, u8YPos;
    MONO_COLOR(0, 1), // u8Color;
    MONO_COLOR(0, 1), // u8SelColor;
    MENU_POWER_ON, //u8NextMenuPage;
    TXT_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    NothingKeyEvent,
    { NULL,	// AdjustFunction
      NULL			// ExecFunction
    },// stDisplayValue;
    { NULL,	// DrawNumberType
      NULL,	// DrawGaugeType
      NULL,	// DrawRadioGroupType
      NULL  //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};

MenuItemType code PowerOnMenuItems[]=
{ { COORD(0, 1), // u8XPos, u8YPos;
    MONO_COLOR(0, 1), // u8Color;
    MONO_COLOR(0, 1), // u8SelColor;
    MENU_POWER_ON, //u8NextMenuPage;
    TXT_CENTER,// u8TXTFlags;
    NULL, // DisplayText;
    AllNothingKeyEvent,
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },// stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      NULL, // DrawRadioGroupType
      NULL  //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};



RadioTextType code StandbyRadioText[]=  //Steven110601
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  { TXT_OVER_256|TXT_FULL_CENTER, COORD(0, 7), TxtMNoignal_01},
  { TXT_OVER_256|TXT_FULL_CENTER|TXT_END, COORD(0, 10), TxtMNoignal_02},
};
DrawRadioGroupType code DrawStandbyRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
  {MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H), MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H), NULL, StandbyRadioText}
};
MenuItemType code StandbyMenuItems[]=
{
  { COORD(MSG_XSTART, 1),   // XPos, YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT,          //NextMenuPage;
    0,//TXT_FULL_CENTER,// u8TXTFlags;
    NULL,//TxtMessage,  // DisplayText;
    StandbyKeyEvent,
    { NULL,	            // AdjustFunction
      NULL			    // ExecFunction
    },                  // stDisplayValue;
    { NULL,	            // DrawNumberType
      NULL,	            // DrawGaugeType
      DrawStandbyRadioGroup,	// DrawRadioGroupType
      NULL                      //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};

RadioTextType code EnterPowerSavingRadioText[]=
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  { TXT_OVER_256|TXT_FULL_CENTER, COORD(0, 5), TxtMNoignal_01},
  { TXT_OVER_256|TXT_FULL_CENTER, COORD(0, 8), TxtMNoignal_02},
  { TXT_OVER_256|TXT_FULL_CENTER|TXT_END,COORD(0, 11),TxtM3PowerSave1},  //Steven110519
};
DrawRadioGroupType code DrawEnterPowerSavingRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
  {MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H),	MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H),  NULL,EnterPowerSavingRadioText}
};

MenuItemType code EnterPowerSavingMenuItems[]=
{
  // Power Save 1
  { COORD(0, 7), // u8XPos, u8YPos;
    MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT,          //u8NextMenuPage;
    0,                  // u8TXTFlags;
    NULL,//TxtMessage,  // DisplayText;
    NothingInputKeyEvent,
    { NULL,	            // AdjustFunction
      NULL			    // ExecFunction
    },                  // stDisplayValue;
    { NULL,	            // DrawNumberType
      NULL,	            // DrawGaugeType
      DrawEnterPowerSavingRadioGroup,	// DrawRadioGroupType
      NULL                      //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  },
};

/// Cable Not Connected---------------------------------------

MenuItemType code CableNotConnectedMenuItems[]=
{
  { COORD(0, 7), // u8XPos, u8YPos;
    MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H), // u8Color
    MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H), // u8SelColor
    MENU_ROOT,              //u8NextMenuPage;
    TXT_OVER_256|TXT_FULL_CENTER,                      //u8TXTFlags;
    TxtM4CableNotConnected, //DisplayText;
    NothingInputKeyEvent,
    { NULL,	                //AdjustFunction
      NULL			        //ExecFunction
    },                      //stDisplayValue;
    { NULL,	                //DrawNumberType
      NULL,	                //DrawGaugeType
      NULL,                 //DrawRadioGroupType
      NULL                  //DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};

/// Out Of Range--------------------------------------------------

RadioTextType code OORRadioText[]=  //Mike110531
{ // u8Flags,	u8XPos,	u8YPos,	DisplayText
  { TXT_FULL_CENTER|TXT_OVER_256, COORD(0, 5), TxtM5OutOfRangeSource},
  { TXT_FULL_CENTER|TXT_OVER_256, COORD(0, 8), TxtM5OutOfRange1},
  { TXT_FULL_CENTER|TXT_OVER_256, COORD(0, 11), TxtM5OutOfRange2},
  { TXT_FULL_CENTER|TXT_OVER_256|TXT_END, COORD(0, 12), TxtM5OutOfRange3},
};

DrawRadioGroupType code DrawOutOfRangeRadioGroup[]=
{ //u8Color,		u8SelColor,	GetValue	pstRadioText
  {MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H),	MONO_COLOR(CP_BK_COLOR_L0, CP_BK_COLOR_H), NULL, OORRadioText}
};


MenuItemType code UnsupportedModeMenuItems[]=
{ //
  { COORD(MSG_XSTART, 3),                //XPos, YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), //u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_H), //u8SelColor
    MENU_ROOT,              //u8NextMenuPage;
    0,                      //u8TXTFlags;
    NULL,//TxtMessage,      //DisplayText;
    NothingKeyEvent,
    { NULL,	                //AdjustFunction
      NULL			        //ExecFunction
    },                      //stDisplayValue;
    { NULL,	                //DrawNumberType
      NULL,	                //DrawGaugeType
      DrawOutOfRangeRadioGroup,	    //DrawRadioGroupType
      NULL                          //DrawIconType
    },
    0//MIB_SELECTABLE       //u16Flags
  },

};
MenuItemType code RootMenuItems[]=
{ // virtual item
  { //COORD(0, 1), // u8XPos, u8YPos;
    COORD(0, 2), // u8XPos, u8YPos;
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0), // u8Color
    MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0), // u8SelColor
#if 0//(defined(LianHeChuangXin_Project))
    MENU_KEY_GUIDE, //u8NextMenuPage;
#else
    MENU_MAIN, //u8NextMenuPage;
#endif    
    TXT_FULL_CENTER,// u8TXTFlags;
    NULL,   // DisplayText;
#if 0//(defined(LianHeChuangXin_Project))
    GuideKeyEvent,
 #else   
    RootKeyEvent,
 #endif   
    { NULL, // AdjustFunction
      NULL  // ExecFunction
    },      // stDisplayValue;
    { NULL, // DrawNumberType
      NULL, // DrawGaugeType
      NULL, // DrawRadioGroupType
      NULL  // DrawIconType
    },
    0//MIB_SELECTABLE // u16Flags
  }
};

#endif   ///_MENUTBLDEF_H


