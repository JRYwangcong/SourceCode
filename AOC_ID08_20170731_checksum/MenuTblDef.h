#if EnablePanelServiceMenu
#include "panel.h"
#endif
#if AllInOneKey
//============AllInOneKey===================================================	 
	//======HotKeyUp=========
#define MIA_HotKeyUp	MIA_ECO	 
	//======HotKeyDown========
	#if Dual
#define MIA_HotKeyDown	MIA_SourceSel
	#else
#define MIA_HotKeyDown	MIA_Auto
	#endif
	//======HotKeyMinus========
	#if AudioFunc
#define MIA_HotKeyMinus MIA_VOL
	#else
#define MIA_HotKeyMinus MIA_Cont
	#endif
	//======HotKeyPlus=========
	#if HotExpansionAdjust
#define MIA_HotKeyPlus	MIA_AdjustExpansion
	#else
#define MIA_HotKeyPlus	MIA_DCR
	#endif
	//======DcrHotKey=========
	#if UnifyHotDcrEco
#define DcrHotKey		MIA_ECO	 
	#else
#define DcrHotKey		MIA_DCR	 
	#endif

#else
//============!AllInOneKey===================================================	 

	//======HotKeyMinus=========
	#if (ModelName ==AOC_919Vz1||ModelName==ENVISION_P971waL_CMI)//petit 20130103  for ID08 old Spec  request	
	#define MIA_HotKeyPlus	MIA_VOL  
        #define MIA_HotKeyMinus      MIA_ECO
	#elif (ModelName ==ENVISION_JRYL1PTR_EV1)//lixw 20160408 for yiqing 156eDP	
		#if EnableHotBrightness
		#define MIA_HotKeyPlus	MIA_Brite//MIA_ECO//MIA_Cont//MIA_AdjustExpansion//MIA_ECO 
		#else
		#define MIA_HotKeyPlus	MIA_ECO//MIA_Cont//MIA_ECO//MIA_Cont//MIA_AdjustExpansion//MIA_ECO 
		#endif
        #define MIA_HotKeyMinus      MIA_AdjustExpansion//MIA_ECO//MIA_AdjustExpansion//MIA_AdjustExpansion
	#else
       
       
   #if AudioFunc  //add FOR 08 OSD Spec  request
#define MIA_HotKeyMinus	MIA_VOL
   #else
#define MIA_HotKeyMinus MIA_ECO
   #endif
	//======HotKeyPlus=========
	#if HotExpansionAdjust   //add FOR 08 OSD Spec  request
	#if(4*PanelWidth == 5*PanelHeight)
#define MIA_HotKeyPlus	MIA_Nothing
    #else
#define MIA_HotKeyPlus	MIA_AdjustExpansion
	#endif
	#else
#define MIA_HotKeyPlus	MIA_Nothing//MIA_DCR
	#endif
		
	#endif

	//======HotKeyMenu=========
#define MIA_HotKeyMenu	MIA_GotoNext
	//======HotKeyExit=========
	#if Dual
#if (ModelName == AOC_2219Vwg||ModelName==AOC_2219Vg||ModelName==ENVISION_G2219_LM220WE1||ModelName==AOC_2219P2_2883||ModelName==AOC_2219Vwg5||ModelName==AOC_2219P2_3329||ModelName==ENVISION_G2360g||ModelName==ENVISION_G2218||ModelName==ENVISION_G2218WG1||ModelName==ENVISION_G2218_PLUS||ModelName==AOC_2219P2_3834)//080624	
#define MIA_HotKeyExit	MIA_Auto
#else
#define MIA_HotKeyExit	MIA_SourceSel
#endif
	#else
#define MIA_HotKeyExit	MIA_Auto
	#endif	
	//======DcrHotKey=========
	#if UnifyHotDcrEco
#define DcrHotKey		MIA_ECO	 
	#else
#define DcrHotKey		MIA_DCR	 
	#endif
#endif



//=====================================================================================
// Key function definition
/*
MenuItemActionType code AdjustSourceKeyEvent[BTN_EndBTN]=
{MIA_DecValue,//MIA_IncValue, //  BTN_Plus
  MIA_IncValue,//MIA_DecValue, // BTN_Minus
  MIA_PrevItem,//MIA_NextItem, 	//MIA_GotoNext, 	// BTN_Menu
  MIA_NextItem,//MIA_PrevItem, 	// MIA_GotoPrev, 	// BTN_Exit
  MIA_SourceSel, 	// BTN_Power
  MIA_SourceSel, //2006-02-22
};

*/

#if AllInOneKey
MenuItemActionType code MainNaviKeyEvent[BTN_EndBTN]=
{
 MIA_NextItem, 	// BTN_Plus
  MIA_PrevItem, 	// BTN_Minus
  MIA_DownItem,  // MIA_IncValue, // MIA_GotoNext, 	// BTN_Menu
  MIA_UpItem, // MIA_DecValue, // MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_SourceSel, //2006-02-22

};
MenuItemActionType code NaviKeyEvent[BTN_EndBTN]=
{
  MIA_IncValue, //  BTN_Plus
  MIA_DecValue, // BTN_Minus
  MIA_NextItem, 	//MIA_GotoNext, 	// BTN_Menu
  MIA_PrevItem, 	// MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_SourceSel, //2006-02-22

};
MenuItemActionType code NaviKeyEvent1[BTN_EndBTN]=
{
  MIA_DecValue, // BTN_Minus
  MIA_IncValue, //  BTN_Plus
  MIA_NextItem, 	//MIA_GotoNext, 	// BTN_Menu
  MIA_PrevItem, 	// MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_SourceSel, //2006-02-22
};

MenuItemActionType code ResetKeyEvents[BTN_EndBTN]=
{
 MIA_IncValue, //  BTN_Plus
 MIA_DecValue, // BTN_Minus
 MIA_NextItem, 	//MIA_GotoNext, 	// BTN_Menu
 MIA_PrevItem, 	// MIA_GotoPrev, 	// BTN_Exit
 MIA_Power, 	// BTN_Power
 MIA_SourceSel, //2006-02-22
 };
MenuItemActionType code FactoryNaviKeyEvent[BTN_EndBTN]=
{
  MIA_IncValue, //  BTN_Plus
  MIA_DecValue, // BTN_Minus
  MIA_JumpAdjustItem, 	//MIA_GotoNext, 	// BTN_Menu
  MIA_JumpNaviItem, 	// MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_Nothing, //2006-02-22
};
MenuItemActionType code FactoryAdjustKeyEvent[BTN_EndBTN]=
{
  MIA_IncValue, //	BTN_Plus
  MIA_DecValue, // BTN_Minus
  MIA_JumpAdjustItem,	//MIA_GotoNext, 	// BTN_Menu
  MIA_JumpNaviItem, 	// MIA_GotoPrev,	// BTN_Exit
  MIA_Power,	// BTN_Power
  MIA_Nothing,
};

MenuItemActionType code FactoryAdjustStayKeyEvent[BTN_EndBTN]=
{
	MIA_IncValue, //	BTN_Plus
	MIA_DecValue, // BTN_Minus
	MIA_NextItem,	//MIA_GotoNext, 	// BTN_Menu
	MIA_JumpNaviItem, 	// MIA_GotoPrev,	// BTN_Exit
	MIA_Power,	// BTN_Power
    MIA_Nothing,
};

MenuItemActionType code NaviExecKeyEvent[BTN_EndBTN]=
{ MIA_Nothing, 	// BTN_Plus
  MIA_Nothing, 	// BTN_Minus
  MIA_JumpAdjustItem,   //MIA_GotoNext,	  // BTN_Menu
  MIA_PrevItem,	  // MIA_GotoPrev,	  // BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_Nothing,
};
//End AllInOneKey
#else

MenuItemActionType code NaviExecKeyEvent[BTN_EndBTN]=
{ MIA_NextItem, 	// BTN_Plus
  MIA_PrevItem, 	// BTN_Minus
  MIA_ExecFunc, 	// BTN_Menu
  MIA_Exit, // MIA_GotoPrev,//MIA_Nothing, 	// BTN_Exit // 070117
  MIA_Power, 	// BTN_Power
  MIA_Nothing,
};


MenuItemActionType code FactoryNaviKeyEvent[BTN_EndBTN]=
{ MIA_NextItem, 	// BTN_Plus
  MIA_PrevItem, 	// BTN_Minus
  MIA_JumpAdjustItem,	// BTN_Menu
  MIA_Exit,//MIA_Nothing, 	// BTN_Exit // 070117
  MIA_Power, 	// BTN_Power
  MIA_Nothing,
};


MenuItemActionType code FactoryAdjustKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_ExecFunc,//MIA_NextItem, 	// BTN_Menu
  MIA_JumpNaviItem,//MIA_Exit,//MIA_Nothing, 	// BTN_Exit // 070117
  MIA_Power, 	// BTN_Power
  MIA_Nothing,
};
/*
MenuItemActionType code FactoryAdjustStayKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_ExecFunc, // BTN_Menu
  MIA_JumpNaviItem, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
#if Dual
  MIA_SourceSel, //2006-02-22
#else
  MIA_JumpNaviItem,
#endif
};
*/

MenuItemActionType code MainNaviKeyEvent[BTN_EndBTN]=
{

  MIA_NextItem, 	// BTN_Plus
  MIA_PrevItem, 	// BTN_Minus
  MIA_GotoNext, // BTN_Menu
  MIA_Exit,    // BTN_Exit
  MIA_Power, 	//MIA_Nothing,  	// BTN_Power  //abing
  MIA_SourceSel,

};
MenuItemActionType code NaviKeyEvent[BTN_EndBTN]=
{
  MIA_IncValue, //  BTN_Plus
  MIA_DecValue, // BTN_Minus
  MIA_AdjustValue, 	//MIA_GotoNext, 	// BTN_Menu
  MIA_ExitAdjustValue, 	// MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// MIA_Nothing, 	// BTN_Power   //abing
  MIA_SourceSel, //2006-02-22

};
MenuItemActionType code NaviKeyEvent1[BTN_EndBTN]=
{
  MIA_DecValue, // BTN_Minus
  MIA_IncValue, //  BTN_Plus
  MIA_AdjustValue, 	//MIA_GotoNext, 	// BTN_Menu
  MIA_ExitAdjustValue, 	// MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// MIA_Nothing, 	// BTN_Power   //abing
  MIA_SourceSel, //2006-02-22

};

MenuItemActionType code ResetKeyEvents[BTN_EndBTN]=
{
 MIA_IncValue,     // BTN_Plus
 MIA_DecValue,     // BTN_Minus
 MIA_AdjustValue,      // BTN_Menu
 MIA_ExitAdjustValue, // MIA_Exit,    // BTN_Exit
 MIA_Power,   // BTN_Power
 MIA_SourceSel, //2006-02-22
 };

#endif




#if AllInOneKey
	MenuItemActionType code AdjusterKeyEvent[BTN_EndBTN]=
	{
	  MIA_GotoPrev, 	// BTN_Plus
	  MIA_GotoNext, 	// BTN_Minus
	  MIA_IncValue, 	// BTN_Menu
	  MIA_DecValue, // MIA_Exit, 	// BTN_Exit
	  MIA_Power, 	// BTN_Power
	  MIA_SourceSel, //2006-02-22
	};

    MenuItemActionType code AdjusterKey1Event[BTN_EndBTN]=
    {
      MIA_GotoPrev, 	// BTN_Plus
      MIA_GotoNext, 	// BTN_Minus
      MIA_DecValue, 	// BTN_Menu
      MIA_IncValue, // MIA_Exit, 	// BTN_Exit
      MIA_Power, 	// BTN_Power
      MIA_SourceSel, //2006-02-22
    };
#else
#if 1//OSD_2008
	MenuItemActionType code AdjusterKeyEvent[BTN_EndBTN]=
	{
#if AllInOneKey

	  MIA_IncValue,     // BTN_Plus
	  MIA_DecValue,     // BTN_Minus
	  MIA_ExitGotoNext,     // BTN_Menu
	  MIA_ExitGotoPrev, // MIA_Exit,    // BTN_Exit
	  MIA_Power,    // BTN_Power
	  MIA_SourceSel, //2006-02-22

#else
	  MIA_IncValue,     // BTN_Plus
	  MIA_DecValue,     // BTN_Minus
	  MIA_Exit,     // BTN_Menu
	  MIA_Exit, // MIA_Exit,    // BTN_Exit
	  MIA_Power,    //MIA_Nothing,    // BTN_Power
	  MIA_SourceSel, //2006-02-22
#endif
	};

	MenuItemActionType code AdjusterKey1Event[BTN_EndBTN]=
	{ MIA_DecValue,     // BTN_Plus
	  MIA_IncValue,     // BTN_Minus
	  MIA_ExitGotoNext,     // BTN_Menu
	  MIA_ExitGotoPrev, // MIA_Exit,    // BTN_Exit
	  MIA_Power,    // BTN_Power
	  MIA_SourceSel, //2006-02-22
	};
#else
    MenuItemActionType code AdjusterKeyEvent[BTN_EndBTN]=
    { MIA_IncValue, 	// BTN_Plus
      MIA_DecValue, 	// BTN_Minus
      MIA_GotoNext, 	// BTN_Menu
      MIA_GotoPrev, // MIA_Exit, 	// BTN_Exit
      MIA_Power, 	// BTN_Power
      MIA_SourceSel, //2006-02-22
    };

    MenuItemActionType code AdjusterKey1Event[BTN_EndBTN]=
    { MIA_DecValue, 	// BTN_Plus
      MIA_IncValue, 	// BTN_Minus
      MIA_GotoNext, 	// BTN_Menu
      MIA_GotoPrev, // MIA_Exit, 	// BTN_Exit
      MIA_Power, 	// BTN_Power
      MIA_SourceSel, //2006-02-22
    };
#endif
#endif

#if Dual
MenuItemActionType code AdjustExecKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_ExecFunc, 	// BTN_Menu
  MIA_Exit, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_SourceSel, //2006-02-22
};
MenuItemActionType code AdjustNoExecKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_GotoPrev, 	// BTN_Menu
  MIA_GotoPrev, 	//MIA_Exit, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_SourceSel, //2006-02-22
};
#else
//Below code is not used //AdjustExecKeyEvent
MenuItemActionType code AdjustExecKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_ExecFunc, 	// BTN_Menu
  MIA_Exit, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_Exit,
};
MenuItemActionType code AdjustNoExecKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_GotoPrev, 	// BTN_Menu
  MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_Exit,
};
#endif

/*
#if Dual
MenuItemActionType code AdjustDDCHotKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_ExecFunc, 	// BTN_Menu
  MIA_Nothing, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_SourceSel, //2006-02-22
};
#else
MenuItemActionType code AdjustDDCHotKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_ExecFunc, 	// BTN_Menu
  MIA_Nothing, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_Nothing
};
#endif

#if Dual
MenuItemActionType code DirAdjusterKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_GotoPrev, 	// BTN_Menu
  MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_SourceSel, //2006-02-22
};
#else
MenuItemActionType code DirAdjusterKeyEvent[BTN_EndBTN]=
{ MIA_IncValue, 	// BTN_Plus
  MIA_DecValue, 	// BTN_Minus
  MIA_GotoPrev, 	// BTN_Menu
  MIA_GotoPrev,//MIA_Nothing, 	// BTN_Exit // 070117
  MIA_Power, 	// BTN_Power
  MIA_GotoPrev,
};
#endif

#if Dual
MenuItemActionType code InvSubAdjusterKeyEvent[BTN_EndBTN]=
{ MIA_DecValue, 	// BTN_Plus
  MIA_IncValue, 	// BTN_Minus
  MIA_GotoPrev, 	// BTN_Menu
  MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 	// BTN_Power
  MIA_SourceSel, //2006-02-22
};
#else
MenuItemActionType code InvSubAdjusterKeyEvent[BTN_EndBTN]=
{ MIA_DecValue, 	// BTN_Plus
  MIA_IncValue, 	// BTN_Minus
  MIA_GotoPrev, 	// BTN_Menu
  MIA_GotoPrev,//MIA_Nothing, 	// BTN_Exit // 070117
  MIA_Power, 	// BTN_Power
  MIA_GotoPrev
};
#endif
*/


// 071221 modified for key event define of hot volume
#if HotInputSelect
MenuItemActionType code HotInputExecKeyEvent[BTN_EndBTN]=
{  
#if AllInOneKey
	#if 0
	#if HotExpansionAdjust
	 MIA_AdjustExpansion,
	#else   
  	 MIA_DCR,//MIA_Nothing, // MIA_Exit,    // BTN_Exit
	#endif	
    #if AudioFunc
        MIA_VOL,
    #else
        MIA_Cont, // MIA_Nothing ,    // BTN_Minus  // Up key
    #endif
   MIA_NextItem,//MIA_Auto,//MIA_Nothing,     // BTN_Minus
   MIA_ECO, //MIA_Nothing,//MIA_Nothing,     // BTN_Menu
   MIA_Power,    // BTN_Power
   MIA_Nothing, //2006-02-22
	#else
	(MIA_HotKeyPlus-MIA_SourceSel) ? MIA_HotKeyPlus : MIA_NextItem , //BTN_Plus
	(MIA_HotKeyMinus-MIA_SourceSel) ? MIA_HotKeyMinus : MIA_NextItem, 	//BTN_Minus
	(MIA_HotKeyDown-MIA_SourceSel) ? MIA_HotKeyDown : MIA_NextItem, 	//BTN_Menu
	(MIA_HotKeyUp-MIA_SourceSel) ? MIA_HotKeyUp : MIA_NextItem, 	//BTN_Exit
	MIA_Power,	  // BTN_Power
	MIA_Nothing, //2006-02-22
	#endif
#else
	#if 0
       #if AudioFunc
           MIA_VOL,
       #else
           MIA_DCR,     // BTN_Plus
       #endif
	MIA_ECO,//MIA_Nothing,     // BTN_Minus
	MIA_ExecFunc,     // BTN_Menu
	MIA_NextItem, // MIA_Exit,    // BTN_Exit
	MIA_Power, 	//MIA_Nothing,    // BTN_Power
	MIA_Nothing, //2006-02-22
	#else
	(MIA_HotKeyPlus-MIA_SourceSel) ? MIA_HotKeyPlus : MIA_NextItem , //BTN_Plus
	(MIA_HotKeyMinus-MIA_SourceSel) ? MIA_HotKeyMinus : MIA_NextItem, 	//BTN_Minus
	MIA_ExecFunc,
	#if (ModelName==AOC_2219Vwg||ModelName==AOC_2219Vg||ModelName==ENVISION_G2219_LM220WE1||ModelName==AOC_2219P2_2883||ModelName==AOC_2219Vwg5||ModelName==AOC_2219P2_3329||ModelName==ENVISION_G2360g||ModelName==ENVISION_G2218||ModelName==ENVISION_G2218WG1||ModelName==ENVISION_G2218_PLUS||ModelName==AOC_2219P2_3834)
	MIA_HotKeyExit,//BTN_Exit
	#else
	(MIA_SourceSel-MIA_SourceSel) ? MIA_HotKeyExit : MIA_NextItem, 	//BTN_Exit
	#endif
	MIA_Power,	//MIA_Nothing,	  // BTN_Power
	MIA_NextItem, //2006-02-22

	#endif
#endif
};
MenuItemActionType code UnsopportModeKeyEvent[BTN_EndBTN]=
{ 
#if AllInOneKey

MIA_Nothing, // BTN_Plus KeyEvent[BTN_EndBTN];
MIA_Nothing, // BTN_Minus
MIA_SourceSel, // BTN_Menu
MIA_Nothing, //MIA_SourceSel, // BTN_Exit
MIA_Power, // BTN_Power
MIA_Nothing // MIA_SourceSel


#else
	#if (ModelName == AOC_2219Vwg||ModelName==AOC_2219Vg||ModelName==ENVISION_G2219_LM220WE1||ModelName==AOC_2219P2_2883||ModelName==AOC_2219Vwg5||ModelName==AOC_2219P2_3329||ModelName==ENVISION_G2360g||ModelName==ENVISION_G2218||ModelName==ENVISION_G2218WG1||ModelName==ENVISION_G2218_PLUS||ModelName==AOC_2219P2_3834)
	  MIA_Nothing, // BTN_Plus KeyEvent[BTN_EndBTN];
	  MIA_Nothing, // BTN_Minus
	  MIA_Nothing, // BTN_Menu
	  MIA_Nothing, // BTN_Exit
	  MIA_Power, // BTN_Power
	  MIA_SourceSel // MIA_SourceSel
	#else
	  MIA_Nothing, // BTN_Plus KeyEvent[BTN_EndBTN];
	  MIA_Nothing, // BTN_Minus
	  MIA_Nothing, // BTN_Menu
	  MIA_SourceSel, // BTN_Exit
	  MIA_Power, // BTN_Power
	  MIA_Nothing // MIA_SourceSel
	#endif  
#endif
};

#endif
MenuItemActionType code HotKeyEvent[BTN_EndBTN]=
{
#if AllInOneKey
   #if 0
   MIA_IncValue, // MIA_Exit,    // BTN_Exit
   MIA_DecValue, // MIA_Exit,    // BTN_Exit
   MIA_SourceSel,//MIA_Auto,//MIA_Nothing,     // BTN_Minus
   MIA_ECO,//MIA_Nothing,     // BTN_Menu
   MIA_Nothing,    // BTN_Power
   MIA_Nothing, //2006-02-22
   #else
   MIA_IncValue, // MIA_Exit,    // BTN_Exit
   MIA_DecValue, // MIA_Exit,    // BTN_Exit
   MIA_HotKeyDown,//MIA_Auto,//MIA_Nothing,     // BTN_Minus
   MIA_HotKeyUp,//MIA_Nothing,     // BTN_Menu
   MIA_Nothing,    // BTN_Power
   MIA_Nothing, //2006-02-22
   #endif
#else
    MIA_IncValue,     // BTN_Plus
	MIA_DecValue,     // BTN_Minus
	MIA_Exit,     // BTN_Menu
	MIA_HotKeyExit,//edit by lizzie.ding,100524   MIA_Exit, //BTN_Exit	
	MIA_Power, 	//MIA_Nothing,    // BTN_Power
	MIA_SourceSel, //2006-02-22

#endif
};
// 071221 modified for key event define of hot dcr
MenuItemActionType code DCRHotKeyEvent[BTN_EndBTN]=
{
#if AllInOneKey
   #if 0
   #if HotExpansionAdjust   
   MIA_AdjustExpansion,
   #else
   MIA_IncValue, // MIA_Exit,    // BTN_Exit
   #endif
   #if AudioFunc
        MIA_VOL,
   #else
        MIA_Cont,
   #endif
    MIA_SourceSel,//MIA_Auto,
    #if UnifyHotDcrEco
    MIA_IncValue,
	#else
	MIA_ECO,
	#endif
    MIA_Nothing,    // BTN_Power
    MIA_Nothing, //2006-02-22
	#else
	(MIA_HotKeyPlus-DcrHotKey) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus
	(MIA_HotKeyMinus-DcrHotKey) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus
	(MIA_HotKeyDown-DcrHotKey) ? MIA_HotKeyDown : MIA_IncValue, 	//BTN_Menu
	(MIA_HotKeyUp-DcrHotKey) ? MIA_HotKeyUp : MIA_IncValue, 	//BTN_Exit
	MIA_Nothing,	// BTN_Power
	MIA_Nothing, //2006-02-22
	#endif
#else
	#if 0
    MIA_IncValue,     // BTN_Plus
	MIA_ECO,     // BTN_Minus
	MIA_Exit,     // BTN_Menu
	MIA_Exit, // MIA_Exit,    // BTN_Exit
	MIA_Power, 	//MIA_Nothing,    // BTN_Power
	MIA_SourceSel, //2006-02-22
	#else
#if (ModelName ==ENVISION_JRYL1PTR_EV1)//lixw 20160408 for yiqing 156eDP	
	MIA_DecValue,//MIA_Nothing,//MIA_DecValue,//MIA_IncValue,//(MIA_HotKeyPlus-DcrHotKey) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus
	MIA_Nothing,//MIA_DecValue,//MIA_Nothing,//(MIA_HotKeyMinus-DcrHotKey) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus
	MIA_Exit,	  // BTN_Menu
	MIA_Exit,    //edit by lizzie.ding,100524   MIA_Exit, //BTN_Exit	
	MIA_Power,	//MIA_Nothing,	  // BTN_Power
	MIA_SourceSel, //20140718
#else
	MIA_Nothing,//MIA_IncValue,//(MIA_HotKeyPlus-DcrHotKey) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus
	MIA_DecValue,//(MIA_HotKeyMinus-DcrHotKey) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus
	MIA_Exit,	  // BTN_Menu
	MIA_Exit,    //edit by lizzie.ding,100524   MIA_Exit, //BTN_Exit	
	MIA_Power,	//MIA_Nothing,	  // BTN_Power
	MIA_SourceSel, //20140718
#endif
	#endif

#endif
};
#if HotExpansionAdjust
MenuItemActionType code ExpansionHotKeyEvent[BTN_EndBTN]=
{
#if AllInOneKey
   #if 0
   MIA_IncValue, // MIA_Exit,    // BTN_Exit
   #if AudioFunc
        MIA_VOL,
   #else
        MIA_Cont,
   #endif
    MIA_SourceSel,//MIA_Auto,
    MIA_ECO,
    MIA_Nothing,    // BTN_Power
    MIA_Nothing, //2006-02-22
	#else
	(MIA_HotKeyPlus-MIA_AdjustExpansion) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus
	(MIA_HotKeyMinus-MIA_AdjustExpansion) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus
	(MIA_HotKeyDown-MIA_AdjustExpansion) ? MIA_HotKeyDown : MIA_IncValue, 	//BTN_Menu
	(MIA_HotKeyUp-MIA_AdjustExpansion) ? MIA_HotKeyUp : MIA_IncValue, 	//BTN_Exit
     MIA_Nothing, 		// BTN_Power
     MIA_SourceSel,
	#endif
#else
	#if 0
    MIA_IncValue,     // BTN_Plus
	MIA_ECO,     // BTN_Minus
	MIA_Exit,     // BTN_Menu
	MIA_Exit, // MIA_Exit,    // BTN_Exit
	MIA_Power, 	//MIA_Nothing,    // BTN_Power
	MIA_SourceSel, //2006-02-22
	#else
#if (ModelName ==ENVISION_JRYL1PTR_EV1)//lixw 20160408 for yiqing 156eDP	
	MIA_Nothing,//(MIA_HotKeyPlus-MIA_AdjustExpansion) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus//140716 Gino
	MIA_IncValue,//MIA_DecValue,//(MIA_HotKeyMinus-MIA_AdjustExpansion) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus//140716 Gino
	MIA_Exit,	  // BTN_Menu
	MIA_Exit,//MIA_HotKeyExit,//edit by lizzie.ding,100524   MIA_Exit, //BTN_Exit//140716 Gino	 
	MIA_Power,	//MIA_Nothing,	  // BTN_Power
	MIA_SourceSel, //20140718	
#else
	#if 0//ScanKey//ModelName==AOC_G2261w  ||  ModelName==ENVISION_P2471w ||ModelName==ENVISION_P2271w||ModelName==ENVISION_P971wl
	(MIA_HotKeyPlus-MIA_AdjustExpansion) ? MIA_HotKeyPlus : MIA_HotKeyPlus , //BTN_Plus
	#else	
	MIA_IncValue,//(MIA_HotKeyPlus-MIA_AdjustExpansion) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus//140716 Gino
	#endif
	MIA_Nothing,//MIA_DecValue,//(MIA_HotKeyMinus-MIA_AdjustExpansion) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus//140716 Gino
	MIA_Exit,	  // BTN_Menu
	MIA_Exit,//MIA_HotKeyExit,//edit by lizzie.ding,100524   MIA_Exit, //BTN_Exit//140716 Gino	 
	MIA_Power,	//MIA_Nothing,	  // BTN_Power
	MIA_SourceSel, //20140718	
	#endif
	#endif
#endif
};

#endif
// 071221 modified for key define of hot eco
MenuItemActionType code ECOHotKeyEvent[BTN_EndBTN]=
{
#if AllInOneKey
	#if 0
	#if HotExpansionAdjust
		 MIA_AdjustExpansion,
	#else	
        MIA_DCR,//MIA_Nothing, // MIA_Exit,    // BTN_Exit
	#endif
    #if AudioFunc
        MIA_VOL,
    #else
        MIA_Cont, // MIA_Nothing ,    // BTN_Minus  // Up key
    #endif
   MIA_SourceSel,//MIA_Auto,//MIA_Nothing,     // BTN_Minus
   MIA_IncValue,     // BTN_Menu
   MIA_Nothing,    // BTN_Power
   MIA_Nothing, //2006-02-22
	#else
	(MIA_HotKeyPlus-MIA_ECO) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus
	(MIA_HotKeyMinus-MIA_ECO) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus
	(MIA_HotKeyDown-MIA_ECO) ? MIA_HotKeyDown : MIA_IncValue, 	//BTN_Menu
	(MIA_HotKeyUp-MIA_ECO) ? MIA_HotKeyUp : MIA_IncValue, 	//BTN_Exit
     MIA_GotoNext, 		// BTN_Power
     MIA_SourceSel,
	#endif
#else
	#if 0
    #if AudioFunc
        MIA_VOL,
    #else
        MIA_DCR,     // BTN_Plus
    #endif
	MIA_IncValue,     // BTN_Minus
	MIA_Exit,     // BTN_Menu
	MIA_Exit, // MIA_Exit,    // BTN_Exit
	MIA_Power, 	//MIA_Nothing,    // BTN_Power
	MIA_SourceSel, //2006-02-22
	#else
	#if (ModelName ==ENVISION_JRYL1PTR_EV1)//lixw 20160408 for yiqing 156eDP	
	MIA_DecValue,//MIA_Nothing,//MIA_DecValue,//MIA_IncValue,//(MIA_HotKeyPlus-MIA_ECO) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus//140716 Gino	
	MIA_Nothing,//MIA_DecValue,//MIA_Nothing,//(MIA_HotKeyMinus-MIA_ECO) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus
	MIA_GotoPrev, // BTN_Menu
	MIA_Exit,// 121018 coding modified // MIA_HotKeyExit, // BTN_Exit
	MIA_Power, // BTN_Power//20140718
	#else
	MIA_Nothing,//MIA_IncValue,//(MIA_HotKeyPlus-MIA_ECO) ? MIA_HotKeyPlus : MIA_IncValue , //BTN_Plus//140716 Gino	
	MIA_DecValue,//(MIA_HotKeyMinus-MIA_ECO) ? MIA_HotKeyMinus : MIA_IncValue, 	//BTN_Minus
	MIA_GotoPrev, // BTN_Menu
	MIA_Exit,// 121018 coding modified // MIA_HotKeyExit, // BTN_Exit
	MIA_Power, // BTN_Power//20140718
	#endif
	#endif
#endif
};
//===========================================================

//===========================================================
#if AllInOneKey
// 071221 modified for key event define of root menu page
MenuItemActionType code RootKeyEvent[BTN_EndBTN]=
{
#if 0
#if HotExpansionAdjust
	MIA_AdjustExpansion,
#else
    MIA_DCR,  // BTN_Plus       // Down key
#endif
#if AudioFunc
    MIA_VOL,
#else
    MIA_Cont, // MIA_Nothing ,    // BTN_Minus  // Up key
#endif
#if INPUT_TYPE==INPUT_1A
    MIA_Auto, 
#else
    MIA_SourceSel,//MIA_Auto, 	// BTN_Menu
#endif
    MIA_ECO,//MIA_GotoECO,	// BTN_Exit
    MIA_GotoNext, // MIA_Power, 	 // BTN_Power
    MIA_SourceSel,
#else
	MIA_HotKeyPlus, 	//BTN_Plus
	MIA_HotKeyMinus,	//BTN_Minus
	MIA_HotKeyDown, 	//BTN_Menu
	MIA_HotKeyUp,		//BTN_Exit
    MIA_GotoNext, 		// BTN_Power
    MIA_SourceSel,
#endif
};
#else

MenuItemActionType code RootKeyEvent[BTN_EndBTN]=
{
#if 0

#if AudioFunc
  MIA_VOL,  	// BTN_Minus
#else
  MIA_DCR , //  BTN_Plus
#endif
  MIA_ECO, // BTN_Minus
  MIA_GotoNext, // BTN_Menu
#if Dual  
  MIA_SourceSel,//MIA_Auto,  // BTN_Exit  //071222
#else
  MIA_Auto,
#endif  
  MIA_Power, 	 // BTN_Power
  MIA_SourceSel,
#else



MIA_HotKeyPlus, 	//BTN_Plus
MIA_HotKeyMinus,	//BTN_Minus
MIA_HotKeyMenu, 	//BTN_Menu
MIA_HotKeyExit,		//BTN_Exit
MIA_Power,		// BTN_Power
MIA_SourceSel,
#endif

};

#endif
//============================================================
MenuItemActionType code LockKeyEvent[BTN_EndBTN]=
{ MIA_GotoNext, 		// BTN_Plus
  MIA_GotoNext, 		// BTN_Minus
  MIA_GotoNext, 	// BTN_Menu
  #if !AllInOneKey
  #if (ModelName == AOC_2219Vwg||ModelName==AOC_2219Vg||ModelName==ENVISION_G2219_LM220WE1||ModelName==AOC_2219P2_2883||ModelName==AOC_2219Vwg5||ModelName==AOC_2219P2_3329||ModelName==ENVISION_G2360g||ModelName==ENVISION_G2218||ModelName==ENVISION_G2218WG1||ModelName==ENVISION_G2218_PLUS||ModelName==AOC_2219P2_3834)
  MIA_Exit,
  #else
  MIA_SourceSel,
  #endif
  #else
  MIA_GotoNext, 	// BTN_Exit
  #endif
  MIA_Power, 	 // BTN_Power
  #if (ModelName == AOC_2219Vwg||ModelName==AOC_2219Vg||ModelName==ENVISION_G2219_LM220WE1||ModelName==AOC_2219P2_2883||ModelName==AOC_2219Vwg5||ModelName==AOC_2219P2_3329||ModelName==ENVISION_G2360g||ModelName==ENVISION_G2218||ModelName==ENVISION_G2218WG1||ModelName==ENVISION_G2218_PLUS||ModelName==AOC_2219P2_3834)
 MIA_SourceSel,
 #else
 MIA_Nothing,
 #endif
};
//============================================================
MenuItemActionType code LogoKeyEvent[BTN_EndBTN]=
{ MIA_Nothing, // BTN_Plus KeyEvent[BTN_EndBTN];
  MIA_Nothing, // BTN_Minus
  MIA_Nothing, // BTN_Menu
  MIA_Nothing, // BTN_Exit
  MIA_Power, // BTN_Power
  MIA_Nothing

};


//============================================================


MenuItemActionType code SourceSeKeyEvent[BTN_EndBTN]=
{ MIA_Nothing, // BTN_Plus KeyEvent[BTN_EndBTN];
  MIA_Nothing, // BTN_Minus
  MIA_Nothing, // BTN_Menu
  #if !AllInOneKey
  MIA_SourceSel, // BTN_Exit
  #else
  MIA_Nothing,
  #endif
  MIA_Power, // BTN_Power
  MIA_Nothing // MIA_SourceSel

};

MenuItemActionType code NothingKeyEvent[BTN_EndBTN]=
{ MIA_Nothing, // BTN_Plus KeyEvent[BTN_EndBTN];
  MIA_Nothing, // BTN_Minus
  MIA_Nothing, // BTN_Menu
  MIA_Nothing, // BTN_Exit
  MIA_Power, // BTN_Power
  MIA_Nothing // MIA_SourceSel

};
MenuItemActionType code NothingKeyEvent1[BTN_EndBTN]=
{ MIA_Nothing, // BTN_Plus KeyEvent[BTN_EndBTN];
  MIA_Nothing, // BTN_Minus
  MIA_Nothing, // BTN_Menu
  MIA_Nothing, // BTN_Exit
  MIA_Power, // BTN_Power
#if Dual
  MIA_SourceSel, //2006-02-22
#else
  MIA_Nothing,
#endif
};
// 071221 modified for power saving key event
MenuItemActionType code PwrSavingKeyEvent[BTN_EndBTN]=
{ 
#if AllInOneKey
  MIA_Nothing,  // BTN_Plus
  MIA_Nothing, // BTN_Minus
#if Dual
    MIA_SourceSel,
#else
  MIA_Nothing, // BTN_Menu
#endif  
  MIA_Nothing, // BTN_Exit
  MIA_Power, // BTN_Power
  MIA_Nothing // MIA_SourceSel

#else

  MIA_Nothing,  // BTN_Plus
  MIA_Nothing, // BTN_Minus
  MIA_Nothing,
 #if Dual
  MIA_Nothing,//MIA_SourceSel,
 #else	  
  MIA_Nothing, // BTN_Exit
 #endif
  MIA_Power, // BTN_Power
  MIA_Nothing // MIA_SourceSel

#endif

};
#ifdef OffPower
MenuItemActionType code OffPowerWarningKeyEvent[BTN_EndBTN]=
{ MIA_GotoPrev,     // BTN_Plus
  MIA_GotoPrev,     // BTN_Minus
  MIA_GotoPrev, 	// BTN_Menu
  MIA_GotoPrev, 	// BTN_Exit
  MIA_Power, 		// BTN_Power
  MIA_GotoPrev, 	// BTN_Source
};
#endif
//============================================================
#if 0
#if Dual
MenuItemActionType code ECOHotKeyEvent[BTN_EndBTN]=
{ MIA_IncValue,     // BTN_Plus
  MIA_DecValue,     // BTN_Minus
  MIA_GotoPrev, // BTN_Menu
  MIA_GotoPrev, // BTN_Exit
  MIA_GotoPrev, // MIA_Power, // BTN_Power
  MIA_SourceSel, //2006-02-22
};
#else
MenuItemActionType code ECOHotKeyEvent[BTN_EndBTN]=
{ MIA_IncValue,     // BTN_Plus
  MIA_DecValue,     // BTN_Minus
  MIA_GotoPrev, // BTN_Menu
  MIA_GotoPrev,//MIA_Nothing, // BTN_Exit // 070117
  MIA_GotoPrev, // MIA_Power, // BTN_Power
  MIA_GotoPrev,
};
#endif

#endif
//============================================================
MenuItemType code PowerOffMenuItems[]=
{ { 0, 1, // XPos, YPos;
	0, 1, // ForeColor, BackColor;
	0, 1, // SelForeColor, SelBackColor;
	PowerOnMenu, //NextMenuPage;
	DWI_CenterText,// DrawMenuItemType;
	NULL, // DisplayText;
	NothingKeyEvent,//PowerOffKeyEvent, //KeyEvent
	{ NULL, // AdjustFunction
	  NULL			// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};

RadioTextType code DrawStandbyMenu[]=
{ // Flags,            XPos,	YPos,	DisplayText
  {dwiRadioGroup|dwiEnd,0, 4,  	InputText },
};
DrawRadioGroupType code DrawStandbyMenuRationText[]=
{ //ForeColor,	BackColor,	GetValue	RadioText
  {0,	12,NULL,DrawStandbyMenu}
};
MenuItemType code StandbyMenuItems[]=
{ { 0, 5, // XPos, YPos;
	CP_BlueColor, CP_WhiteColor, // ForeColor, BackColor;
	0, CP_LightColor, // SelForeColor, SelBackColor;	//20070403 CP_WhiteColor, 0x88,	//
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NoSyncText, // DisplayText;
#if 1//AllInOneKey // 071219 modified for power saving key event
	PwrSavingKeyEvent,//NothingKeyEvent,//PowerOffKeyEvent, //KeyEvent 
#else
	NothingKeyEvent,//PowerOffKeyEvent, //KeyEvent 
#endif	
	{ NULL, // AdjustFunction
	  NULL,////DrawMenuTitle// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawStandbyMenuRationText	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};

MenuItemType code SleepMenuItems[]=
{ { 1, 2, // XPos, YPos;
	CP_BlueColor, CP_LightBlue, // ForeColor, BackColor;
	CP_BlueColor, CP_LightBlue, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_CenterText,// DrawMenuItemType;
	NULL,//GoingToSleepText, // DisplayText;
	NULL,//NothingKeyEvent,//PowerOffKeyEvent, //KeyEvent
	{ NULL, // AdjustFunction
	  NULL,//WakeUpSystem // ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};

MenuItemType code CableNotConnectedMenuItems[]=
{ { 0, 5,//4, // XPos, YPos;
	CP_BlueColor, CP_WhiteColor, // ForeColor, BackColor;
	0, CP_LightColor, // SelForeColor, SelBackColor;	//20070403 CP_WhiteColor, 0x88,	//
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
#if INPUT_TYPE==INPUT_1A	//071225
	CableNotConnectText,//NULL, //CableNotConnected1Text, // DisplayText;
#else
	NULL,
#endif	
	NothingKeyEvent,//PowerOffKeyEvent, //KeyEvent
	{ NULL, // AdjustFunction
	  NULL,////DrawMenuTitle// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // Connected
  { 0, 4, // XPos, YPos;
	CP_BlueColor, CP_WhiteColor, // ForeColor, BackColor;
	0, CP_LightColor, // SelForeColor, SelBackColor;	//20070403 CP_WhiteColor, 0x88,	//
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	 NULL, //CableNotConnected2Text, // DisplayText;
	NothingKeyEvent,  //KeyEvent
	{ NULL, // AdjustFunction
	  NULL			// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	0 // Flags
  }
};

MenuItemType code UnsupportedModeMenuItems[]=
{ // Input Not
  { 0, 5, // XPos, YPos;
	CP_BlueColor, CP_WhiteColor, // ForeColor, BackColor;
	0, 6, // SelForeColor, SelBackColor;	//20070403 CP_WhiteColor, 0x88,	//
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OutOfRangeText, // DisplayText;
#if HotInputSelect
	UnsopportModeKeyEvent,
#else
	SourceSeKeyEvent,  //KeyEvent
#endif	
	{ NULL, // AdjustFunction
	  NULL,//DrawMenuTitle		// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
};

MenuItemType code RootMenuItems[]=
{ // virtual item
  { 0, 1, // XPos, YPos;
	0, 1, // ForeColor, BackColor;
	0, 1, // SelForeColor, SelBackColor;
	MainMenu, //NextMenuPage;
	NULL, //DWI_CenterText,// DrawMenuItemType;
	NULL, // DisplayText;
	RootKeyEvent,
	{ NULL, // AdjustFunction
	  NULL,//AutoConfig// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  }
};
#if 0//EnablePanelServiceMenu
/*
NumberType code DrawPanelDither[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	12,	0,	GetPanelDither}
};
DrawNumberType code DrawPanelDitherNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelDither}
};*/
RadioTextType code PanelColorDepthText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	18, 0, ColorDepthText}
};
DrawRadioGroupType code DrawPanelDitherGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,PanelColorDepthText}
};

//------------------------------------------------------------------------------------------------------------
NumberType code DrawPanelOnTiming1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	2,	GetPanelOnTiming1}
};
DrawNumberType code DrawPanelOnTiming1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelOnTiming1}
};
NumberType code DrawPanelOnTiming2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	2,	GetPanelOnTiming2}
};
DrawNumberType code DrawPanelOnTiming2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelOnTiming2}
};

NumberType code DrawPanelOffTiming1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	3,	GetPanelOffTiming1}
};
DrawNumberType code DrawPanelOffTiming1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelOffTiming1}
};
NumberType code DrawPanelOffTiming2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	3,	GetPanelOffTiming2}
};
DrawNumberType code DrawPanelOffTiming2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelOffTiming2}
};

NumberType code DrawPanelHSyncWidth[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	5,	GetPanelHSyncWidth}
};
DrawNumberType code DrawPanelHSyncWidthNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelHSyncWidth}
};
NumberType code DrawPanelHSyncBackPorch[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	5,	GetPanelHSyncBackPorch}
};
DrawNumberType code DrawPanelHSyncBackPorchNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelHSyncBackPorch}
};
NumberType code DrawPanelVSyncWidth[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	6,	GetPanelVSyncWidth}
};
DrawNumberType code DrawPanelVSyncWidthNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelVSyncWidth}
};
NumberType code DrawPanelVSyncBackPorch[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	6,	GetPanelVSyncBackPorch}
};
DrawNumberType code DrawPanelVSyncBackPorchNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelVSyncBackPorch}
};
NumberType code DrawPanelWidth[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	7,	GetPanelWidth}
};
DrawNumberType code DrawPanelWidthNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelWidth}
};
NumberType code DrawPanelHeight[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	7,	GetPanelHeight}
};
DrawNumberType code DrawPanelHeightNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelHeight}
};
NumberType code DrawPanelHTotal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	8,	GetPanelHTotal}
};
DrawNumberType code DrawPanelHTotalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelHTotal}
};
NumberType code DrawPanelVTotal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	8,	GetPanelVTotal}
};
DrawNumberType code DrawPanelVTotalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelVTotal}
};
NumberType code DrawPanelMaxHTotal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	9,	GetPanelMaxHTotal}
};
DrawNumberType code DrawPanelMaxHTotalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelMaxHTotal}
};
NumberType code DrawPanelMinHTotal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	9,	GetPanelMinHTotal}
};
DrawNumberType code DrawPanelMinHTotalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelMinHTotal}
};
/*NumberType code DrawPanelMaxVTotal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	10,	GetPanelMaxVTotal}
};
DrawNumberType code DrawPanelMaxVTotalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelMaxVTotal}
};
NumberType code DrawPanelMinVTotal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	10,	GetPanelMinVTotal}
};
DrawNumberType code DrawPanelMinVTotalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelMinVTotal}
};*/

NumberType code DrawPanelDCLK[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	10,	GetPanelDCLK}
};
DrawNumberType code DrawPanelDCLKNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelDCLK}
};

NumberType code DrawPanelMinDCLK[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	10,	GetPanelMinDCLK}
};
DrawNumberType code DrawPanelMinDCLKNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelMinDCLK}
};

NumberType code DrawPanelMaxDCLK[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	11,	GetPanelMaxDCLK}
};
DrawNumberType code DrawPanelMaxDCLKNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawPanelMaxDCLK}
};

RadioTextType code ABPortSwapRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	35, 13, ABPortSwapOnOffText}
};
DrawRadioGroupType code DrawABPortSwapGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,ABPortSwapRatioText}
};

RadioTextType code DualPixelOutputRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	16, 13, DualPixelOutputOnOffText}
};
DrawRadioGroupType code DrawDualPixelOutputGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,DualPixelOutputRatioText}
};


RadioTextType code PNSwapRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	16, 14, PNSwapOnOffText}
};
DrawRadioGroupType code DrawPNSwapGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,PNSwapRatioText}
};

RadioTextType code ChannelSwapRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	35, 14, ChannelSwapOnOffText}
};
DrawRadioGroupType code DrawChannelSwapGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,ChannelSwapRatioText}
};
RadioTextType code MSBLSBExchangeRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	16, 15, MSBLSBExchangeOnOffText}
};
DrawRadioGroupType code DrawMSBLSBExchangeGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,MSBLSBExchangeRatioText}
};

RadioTextType code TIModeRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	35, 15, TIModeOnOffText}
};
DrawRadioGroupType code DrawTIModeGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,TIModeRatioText}
};

RadioTextType code OddMSBLSBExchangeRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	16, 16, OddMSBLSBExchangeOnOffText}
};
DrawRadioGroupType code DrawOddMSBLSBExchangeGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,OddMSBLSBExchangeRatioText}
};

RadioTextType code EvenMSBLSBExchangeRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	35, 16, EvenMSBLSBExchangeOnOffText}
};
DrawRadioGroupType code DrawEvenMSBLSBExchangeGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,EvenMSBLSBExchangeRatioText}
};

RadioTextType code OddRBExchangeRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	16, 17, OddRBExchangeOnOffText}
};
DrawRadioGroupType code DrawOddRBExchangeGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,OddRBExchangeRatioText}
};

RadioTextType code EvenRBExchangeRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	35, 17, EvenRBExchangeOnOffText}
};
DrawRadioGroupType code DrawEvenRBExchangeGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,EvenRBExchangeRatioText}
};

/*RadioTextType code DECurrentValue[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	15, 17, DECurrentValueText}
};
DrawRadioGroupType code DrawDECurrentValueGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,DECurrentValue}
};
RadioTextType code ClockCurrentValue[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	34, 17, ClockCurrentValueText}
};
DrawRadioGroupType code DrawClockCurrentValueGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,ClockCurrentValue}
};
RadioTextType code OddCurrentValue[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	15, 18, OddCurrentValueText}
};
DrawRadioGroupType code DrawOddCurrentValueGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,OddCurrentValue}
};
RadioTextType code EvenCurrentValue[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	34, 18, EvenCurrentValueText}
};
DrawRadioGroupType code DrawEvenCurrentValueGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,EvenCurrentValue}
};
*/
NumberType code DrawBurstModeFreq[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	19,	GetBurstModeFreq}
};
DrawNumberType code DrawBurstModeFreqNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawBurstModeFreq}
};

NumberType code DrawDefDuty[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	19,	GetDefDuty}
};
DrawNumberType code DrawDefDutyNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawDefDuty}
};
NumberType code DrawMinDuty[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	13,	20,	GetMinDuty}
};
DrawNumberType code DrawMinDutyNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawMinDuty}
};
NumberType code DrawMaxDuty[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	20,	GetMaxDuty}
};
DrawNumberType code DrawMaxDutyNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawMaxDuty}
};

NumberType code DrawSSCSTEPL[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	15,	21,	GetSSCSTEPL}
};
DrawNumberType code DrawSSCSTEPLNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSSCSTEPL}
};
NumberType code DrawSSCSTEPH[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	34,	21,	GetSSCSTEPH}
};
DrawNumberType code DrawSSCSTEPHNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSSCSTEPH}
};
NumberType code DrawSSCSPANL[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	15,	22,	GetSSCSPANL}
};
DrawNumberType code DrawSSCSPANLNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSSCSPANL}
};
NumberType code DrawSSCSPANH[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	34,	22,	GetSSCSPANH}
};
DrawNumberType code DrawSSCSPANHNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSSCSPANH}
};

RadioTextType code ChangeModePanelVddRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	16, 23, ChangeModePanelVddOnOffText}
};
DrawRadioGroupType code DrawChangeModePanelVddGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	NULL,ChangeModePanelVddRatioText}
};
NumberType code DrawSwing[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	32,	11,	GetSwingValue}
};
DrawNumberType code DrawSwingNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSwing}
};
//------------------------------------------------------------------------------------------------------------

#endif

//==============================================================================================
NumberType code DrawFContrast[]=
{ // Flags,  XPos,	YPos,	GetValue
  {dwiEnd, 11, 2,	GetContrastValue}
};
DrawNumberType code DrawFContrastNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawFContrast}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFBrightness[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	11, 3,	GetBrightnessValue}
};
DrawNumberType code DrawFBrightnessNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,DrawFBrightness}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGainR[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  #ifdef TSUMXXT		//110316 Modify
  {dwiEnd,	8,	2,	GetRedGainValue}
  #else
  {dwiEnd,	9,	2,	GetRedGainValue}
  #endif
};
DrawNumberType code DrawGainRNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawGainR}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGainG[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  #ifdef TSUMXXT		//110316 Modify
  {dwiEnd,  14, 2,  GetGreenGainValue}
  #else
  {dwiEnd,	15, 2,	GetGreenGainValue}
  #endif
};
DrawNumberType code DrawGainGNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawGainG}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGainB[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  #ifdef TSUMXXT		//110316 Modify
  {dwiEnd,  20, 2,  GetBlueGainValue}
  #else
  {dwiEnd,	21, 2,	GetBlueGainValue}
  #endif
};
DrawNumberType code DrawGainBNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawGainB}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawOffsetR[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,9,	3,	GetAdcRedOffsetValue}
};
DrawNumberType code DrawOffsetRNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawOffsetR}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawOffsetG[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	15, 3,	GetAdcGreenOffsetValue}
};
DrawNumberType code DrawOffsetGNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawOffsetG}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawOffsetB[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	21, 3,	GetAdcBlueOffsetValue}
};
DrawNumberType code DrawOffsetBNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawOffsetB}
};
//------------------------------------------------------------------------------------------------------------
//================================================================================================
//------------------------------------------------------------------------------------------------------------
NumberType code DrawRedColorCool1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	9,	5,	GetRColorCool1Value}
};
DrawNumberType code DrawRedColorCool1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawRedColorCool1}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGreenColorCool1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	15, 5,	GetGColorCool1Value}
};
DrawNumberType code DrawGreenColorCool1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawGreenColorCool1}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawBlueColorCool1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 5,	GetBColorCool1Value}
};
DrawNumberType code DrawBlueColorCool1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawBlueColorCool1}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFContrastCool1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 6,	GetContrastCool1Value}
};
DrawNumberType code DrawFContrastCool1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	 DrawFContrastCool1}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFBrightnessCool1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,9, 6,	GetBrightnessCool1Value}
};
DrawNumberType code DrawFBrightnessCool1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawFBrightnessCool1}
};
//------------------------------------------------------------------------------------------------------------
//================================================================================================
//------------------------------------------------------------------------------------------------------------

#if 0 //Cool2

NumberType code DrawRedColorCool2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	9,	8,	GetRColorCool2Value}
};
DrawNumberType code DrawRedColorCool2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawRedColorCool2}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGreenColorCool2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	15, 8,	GetGColorCool2Value}
};
DrawNumberType code DrawGreenColorCool2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawGreenColorCool2}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawBlueColorCool2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 8,	GetBColorCool2Value}
};
DrawNumberType code DrawBlueColorCool2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawBlueColorCool2}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFContrastCool2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 9,	GetContrastCool2Value}
};
DrawNumberType code DrawFContrastCool2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	 DrawFContrastCool2}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFBrightnessCool2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,9, 9,	GetBrightnessCool2Value}
};
DrawNumberType code DrawFBrightnessCool2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawFBrightnessCool2}
};
#endif //Cool2

//------------------------------------------------------------------------------------------------------------
//================================================================================================
//------------------------------------------------------------------------------------------------------------
NumberType code DrawRedColorWarm1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	9,	11,	GetRColorWarm1Value}
};
DrawNumberType code DrawRedColorWarm1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawRedColorWarm1}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGreenColorWarm1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	15, 11,	GetGColorWarm1Value}
};
DrawNumberType code DrawGreenColorWarm1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawGreenColorWarm1}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawBlueColorWarm1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 11,	GetBColorWarm1Value}
};
DrawNumberType code DrawBlueColorWarm1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawBlueColorWarm1}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFContrastWarm1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 12,	GetContrastWarm1Value}
};
DrawNumberType code DrawFContrastWarm1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	 DrawFContrastWarm1}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFBrightnessWarm1[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,9, 12,	GetBrightnessWarm1Value}
};
DrawNumberType code DrawFBrightnessWarm1Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawFBrightnessWarm1}
};
//------------------------------------------------------------------------------------------------------------
//================================================================================================
//------------------------------------------------------------------------------------------------------------
#if 0//Warm2

NumberType code DrawRedColorWarm2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	9,	14,	GetRColorWarm2Value}
};
DrawNumberType code DrawRedColorWarm2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawRedColorWarm2}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGreenColorWarm2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	15, 14,	GetGColorWarm2Value}
};
DrawNumberType code DrawGreenColorWarm2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawGreenColorWarm2}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawBlueColorWarm2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 14,	GetBColorWarm2Value}
};
DrawNumberType code DrawBlueColorWarm2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawBlueColorWarm2}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFContrastWarm2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 15,	GetContrastWarm2Value}
};
DrawNumberType code DrawFContrastWarm2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	 DrawFContrastWarm2}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFBrightnessWarm2[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,9, 15,	GetBrightnessWarm2Value}
};
DrawNumberType code DrawFBrightnessWarm2Number[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawFBrightnessWarm2}
};
#endif //Warm2
//------------------------------------------------------------------------------------------------------------
//================================================================================================
//------------------------------------------------------------------------------------------------------------
NumberType code DrawRedColorNormal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	9,	8,	GetRColorNormalValue}
};
DrawNumberType code DrawRedColorNormalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	DrawRedColorNormal}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGreenColorNormal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	15, 8,	GetGColorNormalValue}
};
DrawNumberType code DrawGreenColorNormalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawGreenColorNormal}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawBlueColorNormal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 8,	GetBColorNormalValue}
};
DrawNumberType code DrawBlueColorNormalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawBlueColorNormal}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFContrastNormal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 9,	GetContrastNormalValue}
};
DrawNumberType code DrawFContrastNormalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor,	 DrawFContrastNormal}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFBrightnessNormal[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,9, 9,	GetBrightnessNormalValue}
};
DrawNumberType code DrawFBrightnessNormalNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawFBrightnessNormal}
};
//------------------------------------------------------------------------------------------------------------

//================================================================================================
//====================================================================================
//------------------------------------------------------------------------------------------------------------
NumberType code DrawRedColorSRGB[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	9,	14, GetRColorSRGBValue}
};
DrawNumberType code DrawRedColorSRGBNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawRedColorSRGB}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawGreenColorSRGB[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,		15, 14, GetGColorSRGBValue}
};
DrawNumberType code DrawGreenColorSRGBNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,  DrawGreenColorSRGB}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawBlueColorSRGB[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	21, 14, GetBColorSRGBValue}
};
DrawNumberType code DrawBlueColorSRGBNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawBlueColorSRGB}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFContrastSRGB[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,21, 15,	GetContrastSRGBValue}
};
DrawNumberType code DrawFContrastSRGBNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,		DrawFContrastSRGB}
};
//------------------------------------------------------------------------------------------------------------
NumberType code DrawFBrightnessSRGB[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,	9, 15,	GetBrightnessSRGBValue}
};
DrawNumberType code DrawFBrightnessSRGBNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	DrawFBrightnessSRGB}
};
//------------------------------------------------------------------------------------------------------------

//====================================================================================

/*
RadioTextType code BankNoText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
  {dwiEnd,		6,	25, BankNameText}
};
DrawRadioGroupType code DrawBankNoRadioGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor, CP_BlueColor,	NULL,	BankNoText}
};
//---------------------------------------------------------
NumberType code DrawAddr[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	 6, 26, GetAddrValue}
};
DrawNumberType code DrawAddrNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	  DrawAddr}
};
//----------------------------------------------------------
NumberType code DrawAddrRegValue[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,		17, 26, GetAddrRegValue}
};
DrawNumberType code DrawAddrRegValueNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,	 DrawAddrRegValue}
};
//----------------------------------------------------------
NumberType code DrawRegValue[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	17, 26, GetRegValue}
};
DrawNumberType code DrawRegValueNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor, DrawRegValue}
};

//*/

#if MS_OVD
RadioTextType code OverDriveRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	10, 21, OverDriveOnOffText}
};
DrawRadioGroupType code DrawOverDriveRadioGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	GetOverDriveValue, OverDriveRatioText}
};

#endif

//----------------------------------------------------------
RadioTextType code BurninRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
 // {dwiEnd,	10, 12, OffText},
  {dwiEnd,	10, 25, BurninOnOffText}
};
DrawRadioGroupType code DrawBurninRadioGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	GetBurninValue, BurninRatioText}
};
#if Enable_ProductModeAdjust
RadioTextType code ProductModeRatioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
  {dwiEnd,	10, 20, ProductModeOnOffText}
};
DrawRadioGroupType code DrawProductModeRadioGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor,CP_BlueColor,	GetProductModeValue, ProductModeRatioText}
};
#endif
#if 0//EnablePanelServiceMenu
RadioTextType code PanelTpyeRadioText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
  {dwiEnd,		7,	23, PanelTpyeText}//20130402  petit
};
DrawRadioGroupType code DrawPanelTpyeRadioGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_RedColor, CP_BlueColor,	NULL,	PanelTpyeRadioText}
};


#endif
// 090310
#if EnableVcomAdjustItem || PANEL_VCOM_ADJUST		//121031 Modify
NumberType code DrawVCOMValue[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	10, 20, GetVCOMValue}
};
DrawNumberType code DrawVCOMValueNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {CP_RedColor,	CP_BlueColor, DrawVCOMValue}
};
#endif

//====================================================================================
RadioTextType code FactoryText[]=
{ // Flags, 	XPos,	YPos,	DisplayText
  { 0,		23, 	0,	VersionDateText},
  {0,			14,	0,	ChangeDateText},
  {0,			1,	0,	ModelNameText},
  { 0,		1,	2,	AdcGainText},
  { 0,		1,	3,	AdcOffsetText},
  { 0,		1,	5,	COOL1Text},
  { 0,		1,	8,  	NORMALText},
  { 0,		1,	11,  WARM1Text},
  {0,	             1,	14,     sRGBText},
    #if DISPLAY_CHECK_VOL
{ 0,		1,	24,	ChecksumText},
//{ 0,		13,	24,	Checksum},
  #endif
  #if 0//EnablePanelServiceMenu
  {dwiEnd,	             1,	27,     PanelOnText},

  //{dwiEnd,	1,	28, PanelText},
  #else
  #if BlacklitTimeOnShow
  {0,	             1,	26,     PanelOnText},
  #endif
  {0,	   1,		28,		  IcModelText},
  {dwiEnd,	1,	27,    PanelText},
  #endif
};

DrawRadioGroupType code DrawFactoryRadioGroup[]=
{ //ForeColor,	BackColor,		GetValue	RadioText
  {CP_WhiteColor,	CP_BlueColor,	NULL,	FactoryText}
};

/*
NumberType code FactoryPanelOnTimeNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiEnd,12, 27,	GetPanelOnTimeValue}
};
DrawNumberType code DrawFactoryPanelOnTimeNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,		FactoryPanelOnTimeNumber}
};
//*/

#if PANELSSCSTEPLFUN
NumberType code DrawSSCSTEPL[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	15,	17,	GetSSCSTEPL}
};
DrawNumberType code DrawSSCSTEPLNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSSCSTEPL}
};
NumberType code DrawSSCSTEPH[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	15,	18,	GetSSCSTEPH}
};
DrawNumberType code DrawSSCSTEPHNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSSCSTEPH}
};
NumberType code DrawSSCSPANL[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	15,	19,	GetSSCSPANL}
};
DrawNumberType code DrawSSCSPANLNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSSCSPANL}
};
NumberType code DrawSSCSPANH[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {dwiHex|dwiEnd,	15,	20,	GetSSCSPANH}
};
DrawNumberType code DrawSSCSPANHNumber[]=
{ // Flags, ForeColor,	BackColor,	XPos,	YPos,	GetValue
  {	CP_RedColor,	CP_BlueColor,DrawSSCSPANH}
};

#endif



MenuItemType code FactoryMenuItems[]=
{ // 0.Auto Color
  { 1, 1, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	AutoColorText, // DisplayText;
	NaviExecKeyEvent,
	{ NULL, // AdjustFunction
	  AutoColor// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

  // 1. Gain R
  { 8, 2, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 2.Gain R adjust
  { 10, 2, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustRedGain,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGainRNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 3.Gain G
  { 14, 2, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable// Flags
  },
  // 4.Gain G adjust
  { 16, 2, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustGreenGain,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGainGNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 5.Gain B
  { 20, 2, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 6.Gain B adjust
  { 20, 2, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBlueGain,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGainBNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 7.Offset R
  { 8, 3, // XPos, YPos;
	#ifdef TSUMXXT		//110316 Modify
	CP_GrayColor,  CP_BlueColor,//CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#else
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#endif
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	#ifdef TSUMXXT		//110316 Modify
	NULL 		  // Flags
	#else
	mibSelectable // Flags
	#endif
  },
  // 8.Offset R adjust
  { 10, 3, // XPos, YPos;
	#ifdef TSUMXXT		//110316 Modify
	CP_GrayColor,  CP_BlueColor,//CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#else
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#endif
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustAdcRedOffset,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawOffsetRNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	#ifdef TSUMXXT		//110316 Modify
	mibDrawValue			   // Flags
	#else
	mibDrawValue|mibSelectable // Flags
	#endif
  },
  // 9.Offset G
  { 14, 3, // XPos, YPos;
	#ifdef TSUMXXT		//110316 Modify
	CP_GrayColor,  CP_BlueColor,//CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#else
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#endif
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL,//AutoColor// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	#ifdef TSUMXXT		//110316 Modify
	NULL		  // Flags
	#else
	mibSelectable // Flags
	#endif
  },
  // 10.Offset G adjust
  { 16, 3, // XPos, YPos;
	#ifdef TSUMXXT		//110316 Modify
	CP_GrayColor,  CP_BlueColor,//CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#else
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#endif
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustAdcGreenOffset, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawOffsetGNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	#ifdef TSUMXXT		//110316 Modify
	mibDrawValue			   // Flags
	#else
	mibDrawValue|mibSelectable // Flags
	#endif
  },
  // 11.Offset B
  { 20, 3, // XPos, YPos;
	#ifdef TSUMXXT		//110316 Modify
	CP_GrayColor,  CP_BlueColor,//CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#else
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#endif
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	#ifdef TSUMXXT		//110316 Modify
	NULL		  // Flags
	#else
	mibSelectable // Flags
	#endif
  },
  // 12.Offset B adjust
  { 20, 3, // XPos, YPos;
	#ifdef TSUMXXT		//110316 Modify
	CP_GrayColor,  CP_BlueColor,//CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#else
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	#endif
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustAdcBlueOffset,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawOffsetBNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	#ifdef TSUMXXT		//110316 Modify
	mibDrawValue			   // Flags
	#else
	mibDrawValue|mibSelectable// Flags
	#endif
  },

#if 1

  //===Cool1===============================================
  // 13.Cool1_ R
  { 8, 5, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 14.Cool1_R adjust
  { 10, 5, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustRedColorCool1,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawRedColorCool1Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
  // 15.Cool_G
  { 14, 5, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 16.Cool_G adjust
  { 16, 5, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustGreenColorCool1,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGreenColorCool1Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
  // 17.Cool1 B
  { 20, 5, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

  // 18.Cool1 B adjust
  { 20, 5, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBlueColorCool1, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawBlueColorCool1Number, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
 // 19.C2- Cool1 Brightness
	{ 1, 6, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryBrightnessText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibSelectable // Flags
	},
	// 20.C2- Cool1 Brightness adjust
	{ 10, 6, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustBrightnessCool1, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFBrightnessCool1Number,	  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibDrawValue|mibSelectable// Flags
	},
	// 21.C2- Cool1 Contrast
	{ 14, 6, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryContrastText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibSelectable // Flags
	},
	// 22.C2-Cool1 Contrast adjust
	{ 22, 6, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustContrastCool1,   // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFContrastCool1Number,  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibDrawValue|mibSelectable // Flags
	},
//==========Cool2====================================

#if 0

  // 23.Cool2_ R
  { 8, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 24.Cool2_R adjust
  { 10, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustRedColorCool2,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawRedColorCool2Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 25.Cool_G
  { 14, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 26.Cool_G adjust
  { 16, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustGreenColorCool2,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGreenColorCool2Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 27.Cool2 B
  { 20, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

  // 28.Cool2 B adjust
  { 20, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBlueColorCool2, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawBlueColorCool2Number, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
 // 29.C2- Cool2 Brightness
	{ 1, 9, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryBrightnessText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibSelectable // Flags
	},
	// 30.C2- Cool2 Brightness adjust
	{ 10, 9, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustBrightnessCool2, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFBrightnessCool2Number,	  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibDrawValue|mibSelectable // Flags
	},
	//31.C2- Cool2 Contrast
	{ 14, 9, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryContrastText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibSelectable // Flags
	},
	// 32.C2-Cool2 Contrast adjust
	{ 22, 9, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustContrastCool2,   // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFContrastCool2Number,  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibDrawValue |mibSelectable// Flags
	},

#endif

//==========Normal====================================
  // 53.Normal_ R
  { 8, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 54.Normal_R adjust
  { 10, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustRedColorNormal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawRedColorNormalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
  // 55.Cool_G
  { 14, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 56.Cool_G adjust
  { 16, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustGreenColorNormal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGreenColorNormalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 57.Normal B
  { 20,8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

  // 58.Normal B adjust
  { 20, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBlueColorNormal, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawBlueColorNormalNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
 // 59.C2- Normal Brightness
	{ 1, 9, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryBrightnessText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibSelectable // Flags
	},
// 60.C2- Normal Brightness adjust
	{ 10, 9, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustBrightnessNormal, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFBrightnessNormalNumber,	  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibDrawValue |mibSelectable// Flags
	},
// 61.C2- Normal Contrast
	{ 14, 9, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryContrastText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibSelectable // Flags
	},
// 62.C2-Normal Contrast adjust
	{ 22, 9, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustContrastNormal,   // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFContrastNormalNumber,  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibDrawValue |mibSelectable// Flags
	},

//==========Warm1====================================
  // 33.Warm1_ R
  { 8, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 34.Warm1_R adjust
  { 10, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustRedColorWarm1,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawRedColorWarm1Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 35.Cool_G
  { 14, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 36.Cool_G adjust
  { 16, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustGreenColorWarm1,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGreenColorWarm1Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 37.Warm1 B
  { 20,11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

  // 38.Warm1 B adjust
  { 20, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBlueColorWarm1, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawBlueColorWarm1Number, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
 // 39.C2- Warm1 Brightness
	{ 1, 12, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryBrightnessText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibSelectable // Flags
	},
// 40.C2- Warm1 Brightness adjust
	{ 10, 12, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustBrightnessWarm1, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFBrightnessWarm1Number,	  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibDrawValue|mibSelectable // Flags
	},
// 41.C2- Warm1 Contrast
	{ 14, 12, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryContrastText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibSelectable // Flags
	},
// 42.C2-Warm1 Contrast adjust
	{ 22, 12, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustContrastWarm1,   // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFContrastWarm1Number,  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibDrawValue|mibSelectable // Flags
	},
//==========Warm2====================================


#if 0
  // 43.Warm2_ R
  { 8, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 44.Warm2_R adjust
  { 10, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustRedColorWarm2,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawRedColorWarm2Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 45.Cool_G
  { 14, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 46.Cool_G adjust
  { 16, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustGreenColorWarm2,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGreenColorWarm2Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 47.Warm2 B
  { 20,14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

  // 48.Warm2 B adjust
  { 20, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBlueColorWarm2, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawBlueColorWarm2Number, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
 // 49.C2- Warm2 Brightness
	{ 1, 15, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryBrightnessText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibSelectable // Flags
	},
// 50.C2- Warm2 Brightness adjust
	{ 10, 15, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustBrightnessWarm2, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFBrightnessWarm2Number,	  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibDrawValue|mibSelectable // Flags
	},
// 51.C2- Warm2 Contrast
	{ 14, 15, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryContrastText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibSelectable // Flags
	},
// 52.C2-Warm2 Contrast adjust
	{ 22, 15, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustContrastWarm2,   // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFContrastWarm2Number,  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibDrawValue|mibSelectable // Flags
	},

#endif
#if 0 // move upper place
//==========Normal====================================
  // 53.Normal_ R
  { 8, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 54.Normal_R adjust
  { 10, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustRedColorNormal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawRedColorNormalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
  // 55.Cool_G
  { 14, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 56.Cool_G adjust
  { 16, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustGreenColorNormal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGreenColorNormalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 57.Normal B
  { 20,11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

  // 58.Normal B adjust
  { 20, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBlueColorNormal, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawBlueColorNormalNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable// Flags
  },
 // 59.C2- Normal Brightness
	{ 1, 12, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryBrightnessText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibSelectable // Flags
	},
// 60.C2- Normal Brightness adjust
	{ 10, 12, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustBrightnessNormal, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFBrightnessNormalNumber,	  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibDrawValue |mibSelectable// Flags
	},
// 61.C2- Normal Contrast
	{ 14, 12, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryContrastText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibSelectable // Flags
	},
// 62.C2-Normal Contrast adjust
	{ 22, 12, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustContrastNormal,   // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFContrastNormalNumber,  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibDrawValue |mibSelectable// Flags
	},
#endif
 //========================================================================================================
  //63. SRGB R
  { 8, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryRedText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 64.SRGB R adjust
  { 10, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustRedColorSRGB,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawRedColorSRGBNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 65.SRGB G
  { 14, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryGreenText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

  // 66.SRGB G adjust
  { 16, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustGreenColorSRGB, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawGreenColorSRGBNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // 67.SRGB B
  { 20, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	FactoryBlueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 68.SRGB B adjust
  { 20, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBlueColorSRGB, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawBlueColorSRGBNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
 },
	// 69.Brightness
	{ 1, 15, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryBrightnessText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibSelectable // Flags
	},
	// 70.SRGB Brightness adjust
	{ 10, 15, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustBrightnessSRGB, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFBrightnessSRGBNumber,   // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibDrawValue|mibSelectable // Flags
	},
	// 71.SRGB Contrast
	{ 14, 15, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  FactoryContrastText, // DisplayText;
	  FactoryNaviKeyEvent,
	  { NULL, // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { NULL, // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
        NULL, //Font
	  mibSelectable // Flags
	},
	//72.SRGB Contrast adjust
	{ 22, 15, // XPos, YPos;
	  CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	  CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	  RootMenu, //NextMenuPage;
	  DWI_Text,// DrawMenuItemType;
	  NULL, // DisplayText;
	  FactoryAdjustKeyEvent,
	  { AdjustContrastSRGB,   // AdjustFunction
		NULL// ExecFunction
	  },// DisplayValue;
	  { DrawFContrastSRGBNumber,  // DrawNumberType
		NULL, // DrawGuageType
		NULL  // DrawRadioGroupType
	  },
         NULL, //Font
	  mibDrawValue|mibSelectable // Flags
	},

#if PANELSSCSTEPLFUN
	    //Output PLL spread spectrum Step Low Byte
  { 1, 17, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SSCSTEPLText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output PLL spread spectrum Step Low Byte Adjust
  { 15, 17, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustSSCSTEPL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSSCSTEPLNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output PLL spread spectrum Step High Byte
  { 1, 18, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SSCSTEPHText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output PLL spread spectrum Step High Byte Adjust
  { 15, 18, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustSSCSTEPH, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSSCSTEPHNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output PLL spread spectrum Span Low Byte
  { 1, 19, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SSCSPANLText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output PLL spread spectrum Span Low Byte Adjust
  { 15, 19, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustSSCSPANL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSSCSPANLNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output PLL spread spectrum Span High Byte
  { 1, 20, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SSCSPANHText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output PLL spread spectrum Span High Byte Adjust
  { 15, 20, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustSSCSPANH, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSSCSPANHNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
  #endif


//==============================================================================================
#if Enable_ProductModeAdjust
  //ProductMode
  { 1, 20, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ProductModeText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 74Burnin Adjust
  { 1, 20, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustProductMode, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawProductModeRadioGroup	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue |mibSelectable// Flags
  },
#endif
// 090310	
#if EnableVcomAdjustItem || PANEL_VCOM_ADJUST		//121031 Modify
  { 1, 20, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	VCOMText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // VCOM Adjust
  { 10, 20, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustVCOMValue,// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawVCOMValueNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
#endif

#if MS_OVD
  { 1, 21, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OverDriveText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 74Burnin Adjust
  { 1, 21, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustOverDrive,// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawOverDriveRadioGroup	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
#endif

  //73 Burnin
  { 1, 25, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BurninText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // 74Burnin Adjust
  { 1, 25, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBurnin, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawBurninRadioGroup	// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue |mibSelectable// Flags
  },
#if 0//EnablePanelServiceMenu
  // Panel Select
  { 1, 23, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelSelectText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // Panel Type Adjust
  { 15, 23, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustPanelTpye, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawPanelTpyeRadioGroup// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  //It is a virtual item.
    { 0, 0, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	NULL,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibInvisible// Flags
  },

  { 1, 24, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	AdjustPanelParaText, // DisplayText; 
	NaviExecKeyEvent,
	{ NULL, // AdjustFunction
	  Test// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
#endif
/*
  // Bank
  { 1, 25, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BankText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // Bank Adjust
  { 2, 25, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustBankNo, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawBankNoRadioGroup// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // Address
  { 1, 26, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	AddressText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // Address Adjust
  { 1, 26, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustAddr,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawAddrNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  // ValueText
  { 11, 26, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ValueText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  // ValueText Adjust
  { 11, 26, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	#if AllInOneKey
	FactoryAdjustStayKeyEvent,
	#else
	FactoryAdjustKeyEvent,
	#endif
	{ AdjustRegValue,	// AdjustFunction
	#if AllInOneKey
	  NULL// ExecFunction
	  #else
	  WriteRegValue// ExecFunction
	  #endif
	},// DisplayValue;
	{ DrawRegValueNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue|mibSelectable // Flags
  },
  //*/
  // Descript
  { 20, 10, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_WhiteColor, CP_BlueColor,// SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ NULL,// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL,// DrawNumberType
	  NULL, // DrawGuageType
	  DrawFactoryRadioGroup//DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue // Flags
  }
#endif



};
#if  0//EnablePanelServiceMenu
MenuItemType code AjustPanlMenuItems[]=
{
  // 0. PanelDitherText
  { 1, 0, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelDitherText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 1.PanelDither adjust
  { 18, 0, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelDither,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL,//DrawPanelDitherNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  DrawPanelDitherGroup	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 2.PanelOnTiming1Text
  { 1, 2, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelOnTiming1Text, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable// Flags
  },
  // 3.PanelOnTiming1 adjust
  { 17, 2, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelOnTiming1,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelOnTiming1Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 4.PanelOnTiming2Text
  { 20, 2, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelOnTiming2Text, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 5.PanelOnTiming2 adjust
  { 32, 2, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelOnTiming2,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelOnTiming2Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 6.PanelOffTiming1Text
  { 1, 3, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelOffTiming1Text, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 7.PanelOffTiming1  adjust
  { 13, 3, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelOffTiming1,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelOffTiming1Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 8.PanelOffTiming2Text
  { 20, 3, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelOffTiming2Text, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 9.PanelOffTiming2 adjust
  { 32, 3, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelOffTiming2,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelOffTiming2Number,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 10.PanelHSyncWidthText
  { 1, 5, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelHSyncWidthText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 11.PanelHSyncWidth adjust
  { 13, 5, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelHSyncWidth,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelHSyncWidthNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 12.PanelHSyncBackPorchText
  {20, 5, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelHSyncBackPorchText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 13.PanelHSyncBackPorch adjust
  { 32, 5, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelHSyncBackPorch,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelHSyncBackPorchNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 16.PanelVSyncWidthText
  { 1, 6, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelVSyncWidthText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 15.PanelVSyncWidth adjust
  { 13, 6, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelVSyncWidth,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelVSyncWidthNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 16.PanelVSyncBackPorchText
  { 20, 6, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelVSyncBackPorchText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 17.PanelVSyncBackPorch adjust
  { 32, 6, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelVSyncBackPorch,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelVSyncBackPorchNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 18.PanelWidthText
  { 1, 7, // XPos, YPos;
	CP_GrayColor,CP_BlueColor, //CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelWidthText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	NULL,//mibSelectable // Flags
  },
  // 19.PanelWidth adjust
  { 13, 7, // XPos, YPos;
	CP_GrayColor,CP_BlueColor,//CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ NULL,//AdjustPanelWidth,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelWidthNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue,//mibDrawValue|mibSelectable // Flags
  },
  // 20.PanelHeightText
  { 20, 7, // XPos, YPos;
	CP_GrayColor,CP_BlueColor,//CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelHeightText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	NULL,//mibSelectable // Flags
  },
  // 23.PanelHeight adjust
  { 32, 7, // XPos, YPos;
	CP_GrayColor,CP_BlueColor,// ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ NULL,//AdjustPanelHeight,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelHeightNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue,//mibDrawValue|mibSelectable // Flags
  },
  // 22.PanelHTotalText
  { 1, 8, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelHTotalText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 23.PanelHTotal adjust
  { 13, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelHTotal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelHTotalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 24.PanelVTotalText
  { 20, 8, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelVTotalText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 25.PanelVTotal adjust
  { 32, 8, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelVTotal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelVTotalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 26.PanelMaxHTotalText
  { 1, 9, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelMaxHTotalText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 27.PanelMaxHTotal adjust
  { 13, 9, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelMaxHTotal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelMaxHTotalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 28.PanelMinHTotalText
  { 20, 9, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelMinHTotalText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 29.PanelMinHTotal adjust
  { 32, 9, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelMinHTotal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelMinHTotalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
 /* // 30.PanelMaxVTotalText
  { 1, 10, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelMaxVTotalText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 31.PanelMaxVTotal adjust
  { 13, 10, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelMaxVTotal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelMaxVTotalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 32.PanelMinVTotalText
  { 20, 10, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelMinVTotalText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 33.PanelMinVTotal adjust
  { 32, 10, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelMinVTotal,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelMinVTotalNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },*/

 
  // 34.PanelDCLKText
  { 1, 10, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelDCLKText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 35.PanelDCLK adjust
  { 13, 10, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelDCLK,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelDCLKNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
 
    // 38.PanelMinDCLKText
  { 20, 10, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelMinDCLKText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 39.PanelMinDCLK adjust
  { 32, 10, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelMinDCLK,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelMinDCLKNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
  // 36.PanelMaxDCLKText
  { 1, 11, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PanelMaxDCLKText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // 37.PanelMaxDCLK adjust
  {13, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPanelMaxDCLK,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawPanelMaxDCLKNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },

       //lvds output swing control
  { 20, 11, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SwingText, // DisplayText;
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
  //lvds output swing control Adjust
  { 32, 11, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	FactoryAdjustKeyEvent,
	{ AdjustSwing, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSwingNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font
	mibDrawValue |mibSelectable// Flags
  },
/***********************Bank 0 0x43********************************/
 
    //Dual Pixel Output
  { 1, 13, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DualPixelOutputText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // Dual Pixel Output Adjust
  { 16, 13, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustDualPixelOutput, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawDualPixelOutputGroup	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
   //A/B Port Swap (only used when Dual Pixel Output)
  { 20, 13, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ABPortSwapText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // A/B Port Swap Adjust
  { 35, 13, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustABPortSwap, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawABPortSwapGroup	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
  
  
/***********************Bank 0 0x42********************************/
    //P/N swap
  { 1, 14, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	PNSwapText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // P/N swap Adjust
  { 16, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustPNSwap, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawPNSwapGroup	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Channel Swap
  { 20, 14, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ChannelSwapText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // P/N swap Adjust
  { 35, 14, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustChannelSwap, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawChannelSwapGroup	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //MSB/LSB Exchange
  { 1, 15, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	MSBLSBExchangeText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // MSB/LSB Exchange Adjust
  { 16, 15, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustMSBLSBExchange, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawMSBLSBExchangeGroup	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //TI Mode
  { 20, 15, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	TIModeText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // TI Mode Adjust
  { 35, 15, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustTIMode, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawTIModeGroup	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Odd channel MSB/LSB Exchange.
  { 1, 16, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OddMSBLSBExchangeText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // Odd channel MSB/LSB Exchange Adjust
  { 16, 16, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustOddMSBLSBExchange, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawOddMSBLSBExchangeGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Even channel MSB/LSB Exchange.
  { 20, 16, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	EvenMSBLSBExchangeText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // Even channel MSB/LSB Exchange Adjust
  { 35, 16, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustEvenMSBLSBExchange, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawEvenMSBLSBExchangeGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Odd channel Red/Blue bus Exchange.
  { 1, 17, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OddRBExchangeText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // Odd channel Red/Blue bus Exchange Adjust
  { 16, 17, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustOddRBExchange, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawOddRBExchangeGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Even channel Red/Blue bus Exchange.
  { 20, 17, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	EvenRBExchangeText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // Even channel Red/Blue bus Exchange Adjust
  { 35, 17, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustEvenRBExchange, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawEvenRBExchangeGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
   /* //Output DE Driving current select.
  { 1, 17, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DECurrentText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // Output DE Driving current  adjust
  { 15, 17, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustDECurrent, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawDECurrentValueGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output Clock Driving current select.
  { 20, 17, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ClockCurrentText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // Output Clock Driving current adjust
  { 34, 17, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustClockCurrent, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawClockCurrentValueGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output data Odd channel Driving current select
  { 1, 18, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	OddCurrentText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // Output data Odd channel Driving current adjust
  { 15, 18, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustOddCurrent, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawOddCurrentValueGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output data Even channel Driving current select.
  { 20, 18, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	EvenCurrentText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output data Even channel Driving current adjust
  { 34, 18, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustEvenCurrent, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawEvenCurrentValueGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },*/
    //BurstModeFreq
  { 1, 19, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	BurstModeFreqText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  // BurstModeFreq adjust
  { 13, 19, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustBurstModeFreq,	// AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawBurstModeFreqNumber,	// DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue|mibSelectable // Flags
  },
    //Default Duty
  { 20, 19, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	DefDutyText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Default Duty Adjust
  { 32, 19, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustDefDuty, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawDefDutyNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Min Duty
  { 1, 20, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	MinDutyText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Min Duty Adjust
  { 13, 20, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustMinDuty, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawMinDutyNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Max Duty
  { 20, 20, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	MaxDutyText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Max Duty Adjust
  { 32, 20, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustMaxDuty, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawMaxDutyNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output PLL spread spectrum Step Low Byte
  { 1, 21, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SSCSTEPLText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output PLL spread spectrum Step Low Byte Adjust
  { 15, 21, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustSSCSTEPL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSSCSTEPLNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output PLL spread spectrum Step High Byte
  { 20, 21, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SSCSTEPHText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output PLL spread spectrum Step High Byte Adjust
  { 34, 21, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustSSCSTEPH, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSSCSTEPHNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output PLL spread spectrum Span Low Byte
  { 1, 22, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SSCSPANLText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output PLL spread spectrum Span Low Byte Adjust
  { 15, 22, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustSSCSPANL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSSCSPANLNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
    //Output PLL spread spectrum Span High Byte
  { 20, 22, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SSCSPANHText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Output PLL spread spectrum Span High Byte Adjust
  { 34, 22, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustSSCSPANH, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ DrawSSCSPANHNumber, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
  
    //Change Mode Panel Vdd On/Off
  { 1, 23, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	ChangeModePanelVddText, // DisplayText; 
	FactoryNaviKeyEvent,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font				
	mibSelectable // Flags
  },
  //Change Mode Panel Vdd On/Off Adjust
  { 16, 23, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText; 
	FactoryAdjustKeyEvent,
	{ AdjustChangeModePanelVddOnOff, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  DrawChangeModePanelVddGroup// DrawRadioGroupType
	},
      NULL, //Font				
	mibDrawValue |mibSelectable// Flags
  },
 #if AllInOneKey
      //It is a virtual item.
    { 0, 0, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	NULL,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibInvisible// Flags
  },
#endif
  { 1, 25, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	InitialSettingText, // DisplayText;
	NaviExecKeyEvent,
	{ NULL, // AdjustFunction
	  InitialPanelSetting// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },
 #if AllInOneKey
      //It is a virtual item.
    { 0, 0, // XPos, YPos;
	CP_WhiteColor, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	NULL, // DisplayText;
	NULL,
	{ NULL, // AdjustFunction
	  NULL// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibInvisible// Flags
  },
#endif
  { 20, 25, // XPos, YPos;
	CP_LightBlue, CP_BlueColor, // ForeColor, BackColor;
	CP_RedColor, CP_BlueColor, // SelForeColor, SelBackColor;
	RootMenu, //NextMenuPage;
	DWI_Text,// DrawMenuItemType;
	SaveSettingText, // DisplayText;
	#if AllInOneKey
	FactoryAdjustKeyEvent,
	#else
	NaviExecKeyEvent,
	#endif
	{ NULL, // AdjustFunction
	  SaveCurrentPanelSetting// ExecFunction
	},// DisplayValue;
	{ NULL, // DrawNumberType
	  NULL, // DrawGuageType
	  NULL	// DrawRadioGroupType
	},
      NULL, //Font
	mibSelectable // Flags
  },

};

#endif
