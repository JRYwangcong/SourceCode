///////////////////////////////////////////////////////////////////////////////
/// @file Menu.h
/// @brief Head file of Menu.c
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _MENU_H
#define _MENU_H

//#include "types.h"
#include "menudef.h"

//#define FIRST_ITEM_CURSOR_TOP_CHAR  0xEF
//#define CURSOR_TOP_CHAR             0x21
//#define CURSOR_BOTTOM_CHAR          0x20
typedef struct
{
  BYTE bOsdTimeout:1;
  BYTE bCP_Change:1;
/// This flag is used by Menu_OsdHandler() to notify FactoryMenuHandler() for showing Factory Menu
/// while "Factory" item in Factory Reset Menu is selected.
/// Remember clear this flag while prepare to show Factory Menu.
  BYTE bShowFactoryMenu:1;
  BYTE bDisplayLogo:1;
  BYTE bDisplayInputMsg[DISPLAY_MAX_NUMS];
  BYTE bDisplayImageOutOfRangeMsg[DISPLAY_MAX_NUMS]; //????? current only one OOR message 
  BYTE bDisplayInputNoSignalMsg[DISPLAY_MAX_NUMS];
  BYTE bOsdLoadData:1;
  BYTE bChangePattern:1;
  BYTE bMoveOsd:1;
  BYTE bOsdReverseX:1;
  BYTE bOsdReverseY:1;
  BYTE bShowInputIcon:1;
  BYTE bDoNotLoadGamme:1;
  BYTE bOsdSwitchInputPort:1;
  BYTE bShowDiagPattern:1;
  BYTE bHoldDiagPatternKey:1;
  BYTE bHoldMenuKey:1;
  BYTE bPressSrcInputKey:1;
  BYTE bThisItemSelected:1;
  BYTE bEnableItemSelect:1;
  BYTE bEnableItemSelectSub:1;

  BYTE bAnyWinDisplayChange:1;                                    	// for any window Display change
  BYTE bShowChangeMessage:1;                            
  BYTE bSwitchInputPortStatus[DISPLAY_MAX_NUMS];        // Menu Switch InputPort Status 
  BYTE bSwitchInputPortFlag[DISPLAY_MAX_NUMS]; 			                    // Switch InputPort Flag

  BYTE bHoldPowerKey:1;

  
} MenuFlagType;

//#define NAVIGATE_CHANGE_COLOR   1


extern void Menu_InitOSD(void);
extern void Menu_Show(void);
extern void Menu_Hide(void);
extern void Menu_InitVariable(void);
extern void Menu_OsdHandler(void);
extern void Menu_ClearOsdCounterAndFlag(void);
extern BYTE GetPrevItem(MenuItemType *pstMenuItem);
extern BYTE GetNextItem(MenuItemType *pstMenuItem);
extern void DrawOsdMenuItem(BYTE u8ItemIndex, MenuItemType *pstMenuItem);
extern void SaveUserPrefSetting(BYTE g_u8MenuPageIndex);

extern Bool ExecuteKeyEvent(MenuItemActionType enMenuAction);
extern void DrawOsdMenuItemText(BYTE u8ItemIndex, MenuItemType *pstMenuItem);
extern Bool CheckOSDFlag(WORD u16Flag);
extern void SetOSDFlag(WORD u16Flag);
extern void ClearOSDFlag(WORD u16Flag);
//extern BYTE GetCurrentMenuFlags();
extern void ResetOsdTimer(void);
extern void SetMenuPositionNormal(void);
extern void SetMenuPositionRotated(void);
extern void MenuRotate(Bool Rotate);
#if ENABLE_WIDE_MODE
extern void ResetMenuPosition(void);
#endif
#if 1//defined(_MSTAR_OSD_)  //Steven110602
extern void Osd_Shadow(Bool u8Enable, BYTE u8ClolorIndex ,BYTE u8Width,BYTE u8Height);
extern void Osd_SetGradient(BYTE u8En, BYTE u8Win_Num, BYTE u8Grad_App, BYTE u8Ini_Color_En, BYTE u8RGB_CLR);
#endif

#if (USE_ADJUSTABLE_MENU && ENABLE_MSTV_UART_DEBUG)
extern Bool DoMenuTable_XXX_FUNC_FlagCheck(MenuPageType *pstMenuTbl, BYTE u8TblSize);
#endif
#ifndef _MENU_C
extern BYTE xdata  g_u8MenuPageIndex;
extern BYTE xdata  g_u8MenuItemIndex;
extern BYTE xdata  g_u8MenuTempValue;
extern BYTE xdata  g_u8TurboKeyCounter;
extern BYTE xdata  g_u8OriginalSetting;
#endif
extern BYTE  xdata InputStatusWinNo;

extern MenuFlagType xdata g_stMenuFlag;

extern void CheckInputPortChange(void);

#endif   /// _MENU_H

