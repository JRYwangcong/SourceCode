///////////////////////////////////////////////////////////////////////////////
/// @file Menu.c
/// @brief OSD GUI function.
/// @author MStarSemi Inc.
///
/// OSD action dispatch function.
///
/// Features
///  -OSD handler
///  -Function to draw OSD.
///
///////////////////////////////////////////////////////////////////////////////
#define _MENU_C

#include <stdlib.h>
#include <string.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "Keypaddef.h"
#include "ColorPalette.h"
#include "menudef.h"
#include "Keypad.h"
#include "Ms_rwreg.h"
#include "drvOSD.h"
#include "msOSD.h"
#include "mStar.h"
#include "menufunc.h"
#include "MenuTbl.h"
#include "ms_reg.h"
#include "MDebug.h" //change position eshin
#include "misc.h"
#include "NVRam.h"
#include "Adjust.h"
//#include "userpref.h"
#include "Power.h"
#include "MsDLC.h"
#include "Detect.h"
#include "msflash.h"
#include "DDC.h"
#include "Mcu.h"
#include "Common.h"
#include "MenuFont.h"
//#include "msid_v1.h"  // Rex 100706
#include "GPIO_DEF.h"
#include "LoadPropFont1218.h"
//#include "halRwreg.h"
#if ENABLE_DPS
#include "msDPS_Setting.h"
#endif

#include "Menu.h"
#include "appWindow.h"
#include "drvmsOVD.h"
#if ENABLE_OSDC_OSDD
#include "drvOSDblend.h"
#endif
#include "FactoryMenu.h"

#include "DrawLogo.h"

#include "drvAudio.h"

#include "MenuStrTbl.h"

#define MENU_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&MENU_DEBUG
    #define MENU_printData(str, value)   printData(str, value)
    #define MENU_printMsg(str)           printMsg(str)
#else
    #define MENU_printData(str, value)
    #define MENU_printMsg(str)
#endif
///////////////////////////////////////////////////////////////////////////
#define CURRENT_MENU            tblMenus[g_u8MenuPageIndex]
#define PREV_MENU_PAGE          CURRENT_MENU.u8PrevMenuPage
#define CURRENT_MENU_ITEMS      CURRENT_MENU.pstMenuItems
#define MENU_ITEM_COUNT         CURRENT_MENU.u8MenuItemCount

#define CURRENT_MENU_ITEM       CURRENT_MENU.pstMenuItems[g_u8MenuItemIndex]
#define NEXT_MENU_PAGE          CURRENT_MENU_ITEM.u8NextMenuPage
#define CURRENT_MENU_ITEM_FUNC  CURRENT_MENU_ITEM.stKeyFunction

#define NO_SELECT_ITEM      0xFF
//Common Area Fonts-Mono
#define CENTER_LINE         0x01
#define TWO_BLOCK           0x02

#define BOTTOM_LINE         0x04
#define TOP_LINE            0x05
#define TWO_BLOCK1          TWO_BLOCK


#define Main_QuickStart_Xpos    ((USER_PREF_LANGUAGE==LANG_Spanish)?)
#define Main_Picture_Xpos          ((USER_PREF_LANGUAGE==LANG_Spanish)?)
#define Main_Color_Xpos             ((USER_PREF_LANGUAGE==LANG_Spanish)?)
#define Main_System_Xpos         ((USER_PREF_LANGUAGE==LANG_Spanish)?)

//========================================================================
BYTE   g_u8MenuPageIndex = 0;
BYTE   g_u8PrevMenuPageIndex = 0;

BYTE   g_u8MenuItemIndex = 0;
BYTE   g_u8PrevMenuItemIndex = 0;

BYTE   g_u8TurboKeyCounter = 0;
BYTE   g_u8MenuTempValue = 0;
BYTE   g_u8OriginalSetting=0;

BYTE u8InputTextLen=7;
BYTE u8ResolutionTextLen=7;
BYTE  xdata InputStatusWinNo = 0;


static WORD   u16MoveX, u16MoveY;
MenuFlagType  g_stMenuFlag;

#if Enable_Corss_LHCX_New
extern void DynamicLoad_HotGameFont(BYTE g_u8MenuPageIndex);
void DrawHotKeyGameClokShow(void);
void Osd_Draw2x2Icon(BYTE xPos, BYTE yPos, BYTE *str);
void Osd_Draw4x3Icon(BYTE xPos, BYTE yPos, BYTE *str);

BYTE xdata HotGameModeItemIndex;
BYTE xdata HotGameSelectItemIndex;
BYTE xdata HotGameClokItemIndex;
BYTE xdata HotGameClokPositionItemIndex;
#endif

//========================================================================
void SaveMonitorSetting(void);
void SaveModeSetting(void);
void SaveUserPrefSetting(BYTE u8MenuPageIndex);
void SaveFactorySetting(void);
BYTE GetMenuItemIndex(BYTE u8MenuPageIndex);
BYTE GetPrevItem(MenuItemType *pstMenuItem);
BYTE GetNextItem(MenuItemType *pstMenuItem);
Bool ExecuteKeyEvent(MenuItemActionType enMenuAction);
Bool IsUserPrefValueGrayout(fpGetValue fpValue);
Bool IsUserPrefOptionGrayout(fpDisplayText fpStr);
void DrawOsdMenu(Bool ShowMenu);
void DrawOsdMenuItem(BYTE u8ItemIndex, MenuItemType *pstMenuItem);
void DrawOsdMenuItemText(BYTE u8ItemIndex, MenuItemType *pstMenuItem);
void DrawOsdMenuItemValue(BYTE u8ItemIndex, DrawValueType *pstValueItem);
void DrawOsdMenuItemNumber(BYTE u8ItemIndex, DrawNumberType *pstNumberItem);
void DrawOsdMenuItemGauge(BYTE u8ItemIndex, DrawGaugeType *pstGaugeItem);
void DrawOsdMenuItemRadioGroup(BYTE u8ItemIndex, DrawRadioGroupType *pstRadioItem);
void MoveOsdWindow(void);
void Menu_ClearOsdCounterAndFlag(void);
void DrawOsdMenuItemIcons(BYTE u8ItemIndex,DrawIconType* pstMultiIcon);
void DrawItemCursor(BYTE u8ItemIndex,  BYTE u8CursorColorType);
void LoadMenuCommonPropFont(void);
void LoadMenuDescriptionPropFont(void);
void DrawDescription(BYTE menupage);
void DynamicLoadFont(BYTE menuPageIndex);
void DrawColorMenuFrame(BYTE SelectEnable);
void DrawSystemMenuFrame(BYTE Select);
void DrawGuideText(void);
static void ReCoverFocusedOption(void);
void LoadMainTextFont(void);
void ReDrawMainStringText(void);
#if LED_HOT_Menu_Func
void DrawLedMenuItems(void);
#endif

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
void DrawMainMenuMultiWindowInfo(void);
#endif
BYTE GetHighLightStatusIndex( BYTE InputType );

BYTE* TxtMMResolution(void);
void msAPI_combo_LoadEDID_Func(void);
void mhal_DP_HPDControl(BYTE ucPortSelect, Bool bSetHPD);
//void mhal_tmds_Set_HPD(MSCombo_TMDS_PORT_INDEX enInputPort, Bool bSetHPD);
void msAPI_combo_IPControlHPD(EN_COMBO_IP_SELECT ucComboSelect, Bool bSetHPD);

Bool SetColorPresetMode(void);
void EDID_ReLoad_Func(BYTE Reload);
void msAPI_AudioSourceSel( BYTE InputPort );
void Set_PIP_Border(void);
Bool SetPIPPositionParameter(void);
void Osd_LoadHotMuteFont( void );
void MultiOSDWin(void);
#if New_Cross_Enable||Enable_Corss_FY||Enable_Corss_LHCX||Enable_Corss_RunFeng
void Osd_DrawCrossFont(void);
void Osd_LoadHOTCROSSFont( void );
#else
void Osd_DrawCrossFont(BYTE ColorIndex);
#endif
void ReLoadHDMI_EDID(void);
void ReLoadDVI_EDID(void);
void InitOsdMainWinDouble(void);
void Osd_LoadHOTLedFont( void );
void Osd_DrawHotLedMenu(BYTE DemoFlag);


////////////////////////////////////////////////////////////////////////////////////////////
void Menu_InitOSD(void)
{
#if ENABLE_WORD_FONT_INDEX //White120911
    {
        extern xdata WORD u16Font16x18StartIndex;
        u16Font16x18StartIndex=0x101; //init 16x18 font start index;
    }
#endif

#if (CHIP_ID == MST9U3)
	// Leona blends OSD on FRC. To prevent garbage, set GOP as the OSD data path for IP main, IP sub, VOP (actually there is no GOP in Leona).
	msWriteByteMask(SCL0_00_81, 0x00, 0x3F);
	msWriteByte(SCL0_40_40, 0x01);	// [0] layer0 L enable
	#if (ENABLE_OSD_LR)
	msWriteByte(SCL0_40_42, 0x01);	// [0] layer0 R enable
	#endif
	msWriteByteMask( 0x101C02, 0x01, 0x05 );	// [0] OSD_L enable, [2] to ip enable
	msWriteByteMask( 0x101C03, 0x21, 0x31 );	// [8] alpha inv, [13:12] ip_de_sel : 0/1 : ipm/ips; 0/1/2/3: op/oc/frc0/frc1
    msWriteByteMask(SCL0_10_46, BIT7|BIT5, BIT7|BIT5);  //reg_osd_blend0_alpha_sel  //About Transparency setting
#else
    #if 1
    msDrvOSDBlend_Enable(DISABLE);
    #else
    msWriteByteMask(SCL0_00_81, BIT4, BIT4);  //[5:4]: 0: GOP  1: OSD 2: OSD compressor
    msWriteByteMask(SCL0_00_0D, BIT7, BIT7);  //reg_gop0_en
    msWriteByteMask(SCL0_10_46, BIT7|BIT5, BIT7|BIT5);  //reg_osd_blend0_alpha_sel  //About Transparency setting
    #endif
#endif


	msWriteByteMask(OSD1_00, 0x00, 0x03); //Set to OSD1 BK0

    drvOSD_ClrOSD_SRAM();

    msWriteByteMask(OSD1_03, BIT2, BIT2);       //OSD wr priority is over display for both cafsram/psram

#if (ENABLE_OSDC_OSDD && _LIMIT_OSDSIZE_)
    msWrite2Byte(OSD1_04, LIMIT_OSDC_HSIZE);         //set osd h range size
    msWrite2Byte(OSD1_06, LIMIT_OSDC_VSIZE);        //set osd v range size
#else
	#if (CHIP_ID == MST9U3) && (ENABLE_OSD_LR)
	msWrite2Byte(OSD1_04, PANEL_WIDTH/2 + OD_GUARD_BAND);         //set osd h range size
	#else
    msWrite2Byte(OSD1_04, PANEL_WIDTH);         //set osd h range size
   	#endif
    msWrite2Byte(OSD1_06, PANEL_HEIGHT);        //set osd v range size
#endif

    //msWriteByteMask(OSD1_02,BIT1,BIT1);       //force blending
    //Init cafsram
    msWrite2Byte(OSD1_08, OSD_FONT_START_ENTRY);               //set font data entry base,
    msWriteByteMask(OSD1_0B, (OSD_FONT_UNIT_SIZE-1)<<4, BIT4|BIT5);  //Set unit size to 3 entries, [5:4] value+1 entries
#if 0
    //init OSD_WIN0 for full screen test pattern
    drvOSD_SetWndCtrl(OSD_WIN0, OSD_WND_CTRL0,
                     (OWC0_CA_DUP_MODE|
                      OWC0_FP_MODE|
                      OWC0_GROUP_SEL0|
                      OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
                      OWC0MASK_ALL);
    drvOSD_SetWndCtrl(OSD_WIN0, OSD_WND_CTRL1,
                     (OWC1_1BP_44MODE_EN|
                      OWC1_44MODE_TRS_EN|
                      OWC1_FONT_HEIGHT(18)),
                      OWC1MASK_ALL);
    drvOSD_SetWndCtrl(OSD_WIN0, OSD_WND_CTRL2,
                     (OWC2_GD_COLOR_R(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_G(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_B(OSD_COLOR_FIX)),
                      OWC2MASK_ALL);
    drvOSD_SetWndFixAttr(OSD_WIN0, _ENABLE, 0x00);

    drvOSD_SetWndFixAlpha(OSD_WIN0,OSD_ALPHA_LV2,0x00); //PG   //(1-a)*low_lvl+a*top_lvl
    drvOSD_SetWndFixAlpha(OSD_WIN0,OSD_ALPHA_LV1,0x00); //LV2
    drvOSD_SetWndFixAlpha(OSD_WIN0,OSD_ALPHA_OUT,0x3F); //OSD
#endif
    drvOSD_SetWndCABaseAddr(OSD_WIN0,0x0000);
    drvOSD_SetWndShiftPixel(OSD_WIN0, 0, 0);    //avoid compiler warning
    drvOSD_SetWndFixColor(OSD_WIN0, 0, 0, 0);   //avoid compiler warning

    //init OSD_MAIN_WND
    drvOSD_SetWndCtrl(OSD_MAIN_WND, OSD_WND_CTRL0,
                     (OWC0_FP_MODE|
                      OWC0_GROUP_SEL0|
                      OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
                      OWC0MASK_ALL);
    drvOSD_SetWndCtrl(OSD_MAIN_WND, OSD_WND_CTRL1,
                     (OWC1_1BP_44MODE_EN|
                      OWC1_44MODE_TRS_EN|
                      OWC1_FONT_HEIGHT(18)),
                      OWC1MASK_ALL);
    drvOSD_SetWndCtrl(OSD_MAIN_WND, OSD_WND_CTRL2,
                     (OWC2_GD_COLOR_R(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_G(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_B(OSD_COLOR_FIX)),
                      OWC2MASK_ALL);
    drvOSD_SetWndCtrl(OSD_MAIN_WND, OSD_WND_CTRL3,
                     (OWC3_LV1_KM_SWITCH|OWC3MASK_CKEY_INV),
                      OWC3MASK_ALL);

    drvOSD_SetWndFixAlpha(OSD_MAIN_WND,OSD_ALPHA_LV2,0x00); //PG   //(1-a)*low_lvl+a*top_lvl
    drvOSD_SetWndFixAlpha(OSD_MAIN_WND,OSD_ALPHA_LV1,0x3F); //FG
    drvOSD_SetWndFixAlpha(OSD_MAIN_WND,OSD_ALPHA_OUT,0x3F); //OSD
    drvOSD_SetWndCABaseAddr(OSD_MAIN_WND,OSD_MAIN_WND_CA_BASE);

    //init OSD_BUTTON_WND
	drvOSD_SetWndCtrl(OSD_BUTTON_WND, OSD_WND_CTRL0,
					 (OWC0_FP_MODE|
					  OWC0_GROUP_SEL0|
					  OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
					  OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
					  OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
					  OWC0MASK_ALL);
	drvOSD_SetWndCtrl(OSD_BUTTON_WND, OSD_WND_CTRL1,
					 (OWC1_1BP_44MODE_EN|
					  OWC1_44MODE_TRS_EN|
					  OWC1_FONT_HEIGHT(18)),
					  OWC1MASK_ALL);
	drvOSD_SetWndCtrl(OSD_BUTTON_WND, OSD_WND_CTRL2,
					 (OWC2_GD_COLOR_R(OSD_COLOR_FIX)|
					  OWC2_GD_COLOR_G(OSD_COLOR_FIX)|
					  OWC2_GD_COLOR_B(OSD_COLOR_FIX)),
					  OWC2MASK_ALL);
	drvOSD_SetWndCtrl(OSD_BUTTON_WND, OSD_WND_CTRL3,
					 (OWC3_LV1_KM_SWITCH|OWC3MASK_CKEY_INV),
					  OWC3MASK_ALL);
	drvOSD_SetWndFixAlpha(OSD_BUTTON_WND,OSD_ALPHA_LV2,0x00); //PG	 //(1-a)*low_lvl+a*top_lvl
	drvOSD_SetWndFixAlpha(OSD_BUTTON_WND,OSD_ALPHA_LV1,0x3F); //FG
	drvOSD_SetWndFixAlpha(OSD_BUTTON_WND,OSD_ALPHA_OUT,0x3F); //OSD
	drvOSD_SetWndCABaseAddr(OSD_BUTTON_WND,OSD_BUTTON_WND_CA_BASE);

	//init OSD_CURSOR_WND
    drvOSD_SetWndCtrl(OSD_ICON_WND, OSD_WND_CTRL0,
                     (OWC0_FP_MODE|
                      OWC0_GROUP_SEL0|
                      //OWC0_GROUP_SEL1|
                      OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
                      OWC0MASK_ALL);
    drvOSD_SetWndCtrl(OSD_ICON_WND, OSD_WND_CTRL1,
                     (OWC1_1BP_44MODE_EN|
                      OWC1_44MODE_TRS_EN|
                      OWC1_FONT_HEIGHT(18)),
                      OWC1MASK_ALL);
    drvOSD_SetWndCtrl(OSD_ICON_WND, OSD_WND_CTRL2,
                     (OWC2_GD_COLOR_R(OSD_COLOR_FIX)|//OSD_GD_EG3
                      OWC2_GD_COLOR_G(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_B(OSD_COLOR_FIX)),
                      OWC2MASK_ALL);
    drvOSD_SetWndCtrl(OSD_ICON_WND, OSD_WND_CTRL3,
                      (OWC3_1BPP_ATTR_BIT1|OWC3_LV1_KM_SWITCH|OWC3MASK_CKEY_INV),
                      OWC3MASK_ALL);

    drvOSD_SetWndFixAlpha(OSD_ICON_WND,OSD_ALPHA_LV2,0x00); //PG   //(1-a)*low_lvl+a*top_lvl
    drvOSD_SetWndFixAlpha(OSD_ICON_WND,OSD_ALPHA_LV1,0x3F); //PG
    drvOSD_SetWndFixAlpha(OSD_ICON_WND,OSD_ALPHA_OUT,0x3F); //OSD	
    
	drvOSD_SetWndCABaseAddr(OSD_ICON_WND,OSD_ICON_WND_CA_BASE);    


	//init OSD_SUB_WND
    drvOSD_SetWndCtrl(OSD_SUB_WND, OSD_WND_CTRL0,
                     (OWC0_FP_MODE|
                      OWC0_GROUP_SEL0|
                      //OWC0_GROUP_SEL1|
                      OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
                      OWC0MASK_ALL);
    drvOSD_SetWndCtrl(OSD_SUB_WND, OSD_WND_CTRL1,
                     (OWC1_1BP_44MODE_EN|
                      OWC1_44MODE_TRS_EN|
                      OWC1_FONT_HEIGHT(18)),
                      OWC1MASK_ALL);
    drvOSD_SetWndCtrl(OSD_SUB_WND, OSD_WND_CTRL2,
                     (OWC2_GD_COLOR_R(OSD_COLOR_FIX)|//OSD_GD_EG5
                      OWC2_GD_COLOR_G(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_B(OSD_COLOR_FIX)),
                      OWC2MASK_ALL);
    drvOSD_SetWndCtrl(OSD_SUB_WND, OSD_WND_CTRL3,
                      (OWC3_1BPP_ATTR_BIT1|OWC3_LV1_KM_SWITCH|OWC3MASK_CKEY_INV),
                      OWC3MASK_ALL);
	drvOSD_SetWndFixAlpha(OSD_SUB_WND,OSD_ALPHA_LV2,0x00); //PG   //(1-a)*low_lvl+a*top_lvl
	drvOSD_SetWndFixAlpha(OSD_SUB_WND,OSD_ALPHA_LV1,0x00); //FG
	drvOSD_SetWndFixAlpha(OSD_SUB_WND,OSD_ALPHA_OUT,0x3F); //OSD
	
	drvOSD_SetWndCABaseAddr(OSD_SUB_WND,OSD_SUB_WND_CA_BASE);

    drvOSD_SetWndCtrl(OSD_SUB2_WND, OSD_WND_CTRL0,
                     (OWC0_FP_MODE|
                      OWC0_GROUP_SEL0|
                      //OWC0_GROUP_SEL1|
                      OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
                      OWC0MASK_ALL);
    drvOSD_SetWndCtrl(OSD_SUB2_WND, OSD_WND_CTRL1,
                     (OWC1_1BP_44MODE_EN|
                      OWC1_44MODE_TRS_EN|
                      OWC1_FONT_HEIGHT(18)),
                      OWC1MASK_ALL);
    drvOSD_SetWndCtrl(OSD_SUB2_WND, OSD_WND_CTRL2,
                     (OWC2_GD_COLOR_R(OSD_COLOR_FIX)|//OSD_GD_EG5
                      OWC2_GD_COLOR_G(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_B(OSD_COLOR_FIX)),
                      OWC2MASK_ALL);
    drvOSD_SetWndCtrl(OSD_SUB2_WND, OSD_WND_CTRL3,
                      (OWC3_1BPP_ATTR_BIT1|OWC3_LV1_KM_SWITCH|OWC3MASK_CKEY_INV),
                      OWC3MASK_ALL);
	drvOSD_SetWndFixAlpha(OSD_SUB2_WND,OSD_ALPHA_LV2,0x00); //PG   //(1-a)*low_lvl+a*top_lvl
	drvOSD_SetWndFixAlpha(OSD_SUB2_WND,OSD_ALPHA_LV1,0x00); //FG
	drvOSD_SetWndFixAlpha(OSD_SUB2_WND,OSD_ALPHA_OUT,0x3F); //OSD
	
	drvOSD_SetWndCABaseAddr(OSD_SUB2_WND,OSD_SUB2_WND_CA_BASE);    	

	//init OSD_CURSOR_WND
    drvOSD_SetWndCtrl(OSD_CURSOR_WND, OSD_WND_CTRL0,
                     (OWC0_FP_MODE|
                      OWC0_GROUP_SEL0|
                      //OWC0_GROUP_SEL1|
                      OWC0_LV1_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_LV2_ALPHA_SRC(OSD_A_FIX)|
                      OWC0_OUT_ALPHA_SRC(OSD_A_FIX)),
                      OWC0MASK_ALL);
    drvOSD_SetWndCtrl(OSD_CURSOR_WND, OSD_WND_CTRL1,
                     (OWC1_1BP_44MODE_EN|
                      OWC1_44MODE_TRS_EN|
                      OWC1_FONT_HEIGHT(18)),
                      OWC1MASK_ALL);
    drvOSD_SetWndCtrl(OSD_CURSOR_WND, OSD_WND_CTRL2,
                     (OWC2_GD_COLOR_R(OSD_COLOR_FIX)|//OSD_GD_EG2
                      OWC2_GD_COLOR_G(OSD_COLOR_FIX)|
                      OWC2_GD_COLOR_B(OSD_COLOR_FIX)),
                      OWC2MASK_ALL);
    drvOSD_SetWndCtrl(OSD_CURSOR_WND, OSD_WND_CTRL3,
                      (OWC3_1BPP_ATTR_BIT1|OWC3_LV1_KM_SWITCH|OWC3MASK_CKEY_INV),
                      OWC3MASK_ALL);

    drvOSD_SetWndFixAlpha(OSD_CURSOR_WND,OSD_ALPHA_LV2,0x00); //PG   //(1-a)*low_lvl+a*top_lvl
    drvOSD_SetWndFixAlpha(OSD_CURSOR_WND,OSD_ALPHA_LV1,0x00); //PG
    drvOSD_SetWndFixAlpha(OSD_CURSOR_WND,OSD_ALPHA_OUT,0x3F); //OSD

  #if 0 //TSUMV //White121112
    drvOSD_SetGDEngineEnableWnd(OSD_GD_EG3,OSD_CURSOR_WIN_BIT,OSD_CURSOR_WIN_BIT);
    drvOSD_SetGDEngineEnableWnd(OSD_GD_EG4,OSD_CURSOR_WIN_BIT,OSD_CURSOR_WIN_BIT);
    drvOSD_SetGDEngineEnableWnd(OSD_GD_EG5,OSD_CURSOR_WIN_BIT,OSD_CURSOR_WIN_BIT);
  #endif

    OSD_SET_WND(OSD_MAIN_WND);
    Osd_SetPosition(50, 50); // set osd position
    InitOsdAfterLogo();
    //WrapLayerFlag.bButtonOsd=0;
    //WrapLayerFlag.bSync2Main=0;
#if ENABLE_OSD_LR_DOUBLE
  if(FACTORY_MODE_FLAG||(!g_stMenuFlag.bDisplayLogo))
  InitOsdMainWinDouble();
#endif
	
}

#if ENABLE_OSD_LR_DOUBLE
void InitOsdMainWinDouble(void)
{
   msWriteByteMask(OSD2_29, BIT6, BIT6);      //Set Osd h range Size
   msWriteByteMask(OSD2_2B, BIT6, BIT6);      //Set Osd V range Size
   
   msWriteByteMask(OSD2_49, BIT6, BIT6);      //Set Osd h range Size
   msWriteByteMask(OSD2_4B, BIT6, BIT6);      //Set Osd V range Size

   msWriteByteMask(OSD2_69, BIT6, BIT6);      //Set Osd h range Size
   msWriteByteMask(OSD2_6B, BIT6, BIT6);      //Set Osd V range Size

   msWriteByteMask(OSD2_89, BIT6, BIT6);      //Set Osd h range Size
   msWriteByteMask(OSD2_8B, BIT6, BIT6);      //Set Osd V range Size

   msWriteByteMask(OSD2_A9, BIT6, BIT6);      //Set Osd h range Size
   msWriteByteMask(OSD2_AB, BIT6, BIT6);      //Set Osd V range Size

   msWriteByteMask(OSD2_C9, BIT6, BIT6);      //Set Osd h range Size
   msWriteByteMask(OSD2_CB, BIT6, BIT6);      //Set Osd V range Size

}
#endif

void Menu_SetCursor(BYTE u8Win_Num,Bool u8Enable)
{
    Osd_SetWindowEnable(u8Win_Num,u8Enable);
  #if 0
    drvOSD_SetTGenOn(OSD_TG0,u8Enable);
    drvOSD_SetTGenOn(OSD_TG1,u8Enable);
  #endif
}

void DisableAllSubWin(void)
{
	Osd_DisableSubWin(0);
	Osd_DisableSubWin(1);
	Osd_DisableSubWin(2);
	Osd_DisableSubWin(3);
	Osd_DisableSubWin(4);
	Delay1ms(100);
}

void Menu_Show(void)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
 if((!InputStatusMultiWin))
#endif 	
    drvOSD_SetWndOn(OSD_MAIN_WND, TRUE);
  //  if (IS_MAIN_L0(g_u8MenuPageIndex) || IS_MAIN_L1(g_u8MenuPageIndex))
   //     drvOSD_SetWndOn(MAIN_GD_WIN,TRUE);

#if ENABLE_OSDC_OSDD && (!ZUI)
    msDrvOSDBlend_Enable(TRUE);
#endif

	#if ENABLE_MMD_DCR
	SET_SMARTCONTRAST_BACKLIGHT_ON();
	if(DCR_Status >= DCR_BL_READY)
	{
		DCR_Status = DCR_BL_READY;
		DCRWaitBLOffCounter = 0;
		HighContCounter = 0;	
	}
	#endif
	
	g_WindowEnable|=BIT7;
	    if(g_WindowEnable&BIT0)
	        Osd_EnableSubWin(0);
	    if(g_WindowEnable&BIT1)	
	        Osd_EnableSubWin(1);
	    if(g_WindowEnable&BIT2)	
	        Osd_EnableSubWin(2);
	    if(g_WindowEnable&BIT3)	
	        Osd_EnableSubWin(3);
	    if(g_WindowEnable&BIT4)	
	        Osd_EnableSubWin(4);
	

    Set_OSDShowFlag();

printMsg("Menu_Show");
}

void Menu_Hide(void)
{
#if ENABLE_OSDC_OSDD && (!ZUI)
    msDrvOSDBlend_Enable(FALSE);
#endif
    Menu_SetCursor(OSD_CURSOR_WND,FALSE);

    drvOSD_SetWndOn(OSD_MAIN_WND, FALSE);
    drvOSD_SetWndOn(MAIN_GD_WIN,FALSE);

#if ENABLE_MMD_DCR
	SET_SMARTCONTRAST_BACKLIGHT_OFF();
#endif

	g_WindowEnable&=~BIT7;
	Osd_DisableSubWin(4);
	Osd_DisableSubWin(3);
	Osd_DisableSubWin(2);
	Osd_DisableSubWin(1);
	Osd_DisableSubWin(0);

	
    Clr_OSDShowFlag();
    Clr_LR_UpDown_SwitchFlag();	
    Clr_Left_EnterExit_SwitchFlag();	
    Clr_Right_EnterExit_SwitchFlag();	
	
printMsg("Menu_Hide");
}

/*********************************************************************************************/
///Init g_u8MenuPageIndex, g_u8MenuItemIndex, g_u16OsdCounter and clear OSD timeout flag.
///@param NONE
///
/*********************************************************************************************/
void Menu_InitVariable( void )
{
    g_u8MenuItemIndex = 0;
    Menu_ClearOsdCounterAndFlag();
    Clr_LR_UpDown_SwitchFlag();	
    Clr_Left_EnterExit_SwitchFlag();	
    Clr_Right_EnterExit_SwitchFlag();	
	
    if (POWER_ON_FLAG)
    {
        g_u8MenuPageIndex = MENU_ROOT;
        //printMsg("Menu_InitVariable RootMenu");
    }
    else
    {
        g_u8MenuPageIndex = MENU_POWER_OFF;
        PowerOffSystem();
        //printMsg("Menu_InitVariable PowerOff");
    }
#if (USE_ADJUSTABLE_MENU && ENABLE_MSTV_UART_DEBUG && MENU_TBL_CHECK)
    if (DoMenuTable_XXX_FUNC_FlagCheck(tblMenus,sizeof(tblMenus)/sizeof(MenuPageType)))
        MENU_printMsg(">MENU TABLE STRUCTURE OK!");
#endif

    if((g_stMenuFlag.bShowInputIcon)
#if LOGO_ENABLE
    ||(g_stMenuFlag.bDisplayLogo)
#endif	
    )
    {
        Menu_InitAction();
    }
}


/// Call by ExecuteKeyEvent() to set the g_u8MenuTempValue for MIA_GOTO_ADJUST_ITEM/MIA_REDRAW_MENU action.
///g_u8MenuTempValue is option focus.
static void InitAdjusterFocusedOption(void)
{

printData("InitAdjusterFocusedOption g_u8MenuTempValue==%d",g_u8MenuTempValue);

/*
    if (CURRENT_MENU.pstMenuItems
        && CURRENT_MENU_ITEM.stDisplayValue.pstDrawRadioGroup
         && CURRENT_MENU_ITEM.stDisplayValue.pstDrawRadioGroup->GetUserPrefOption)
    {
        if (CURRENT_MENU_ITEM.stDisplayValue.pstDrawRadioGroup->GetUserPrefOption==GetColorPresetValue)
        {
            g_u8OriginalSetting=CURRENT_PICTURE_MODE;
            g_u8MenuTempValue=CURRENT_PICTURE_MODE;
        }
    #if ENABLE_SUPER_RESOLUTION
        else if (CURRENT_MENU_ITEM.stDisplayValue.pstDrawRadioGroup->GetUserPrefOption==GetSuperResolutionValue)
        {
            g_u8OriginalSetting=CURRENT_SUPER_RESOLUTION;
            g_u8MenuTempValue=CURRENT_SUPER_RESOLUTION;
        }
    #endif
        else
            g_u8MenuTempValue=CURRENT_MENU_ITEM.stDisplayValue.pstDrawRadioGroup->GetUserPrefOption();
    }
   else 
*/

  if(IS_MAIN_L1(g_u8MenuPageIndex))	
   {
      if(g_u8MenuPageIndex==MENU_QUICKSTART)
      {
       if(g_u8MenuItemIndex==QS_Aspect_Adj_IDX)
       {
            g_u8OriginalSetting=USER_PREF_WIDE_MODE(SrcInputPortM);
            g_u8MenuTempValue=USER_PREF_WIDE_MODE(SrcInputPortM);
       }
  	else if(g_u8MenuItemIndex==QS_Source_Adj_IDX)
  	{

	  #if Enable_AutoInput_Menu	
             g_u8MenuTempValue=USER_PREF_INPUT_PRIORITY;
             g_u8OriginalSetting=USER_PREF_INPUT_PRIORITY;
	 #else
             g_u8MenuTempValue=USER_PREF_INPUT_TYPE;
             g_u8OriginalSetting=USER_PREF_INPUT_TYPE;
	 #endif
  	}
       else if(g_u8MenuItemIndex==QS_Preset_Adj_IDX)
       {
            g_u8OriginalSetting=CURRENT_PICTURE_MODE;
            g_u8MenuTempValue=CURRENT_PICTURE_MODE;
           
       }
    	}   
      else if(g_u8MenuPageIndex==MENU_PICTURE)
      {
         if(g_u8MenuItemIndex==Pic_Aspect_Adj_IDX)
         {
              g_u8OriginalSetting=USER_PREF_WIDE_MODE(SrcInputPortM);
              g_u8MenuTempValue=USER_PREF_WIDE_MODE(SrcInputPortM);
         }
         else if(g_u8MenuItemIndex==Pic_Preset_Adj_IDX)
         {
            g_u8OriginalSetting=CURRENT_PICTURE_MODE;
            g_u8MenuTempValue=CURRENT_PICTURE_MODE;
           
          }
      }
      else if(g_u8MenuPageIndex==MENU_COLOR)
      {
         if(g_u8MenuItemIndex==Col_Gamma_Adj_IDX)
         {
              g_u8OriginalSetting=CURRENT_GAMMA;
              g_u8MenuTempValue=CURRENT_GAMMA;
         }
	  else if(g_u8MenuItemIndex==Col_Temperature_Adj_IDX) 
	  {
              g_u8OriginalSetting=CURRENT_COLOR_TEMP;
              g_u8MenuTempValue=CURRENT_COLOR_TEMP;
	  }
      }
      else if(g_u8MenuPageIndex==MENU_SYSTEM
#if LED_HOT_Menu_Func
	  ||g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER
#endif
)
      {
         if(g_u8MenuItemIndex==Sys_Language_Adj_IDX)
         {
              g_u8OriginalSetting=USER_PREF_LANGUAGE;
              g_u8MenuTempValue=USER_PREF_LANGUAGE;
         }
         else if(g_u8MenuItemIndex==Sys_SleepMode_Adj_IDX)
         {
              g_u8OriginalSetting=USER_PREF_SLEEPMODE;
              g_u8MenuTempValue=USER_PREF_SLEEPMODE;
         }
      }
	  

   }

printData("g_u8MenuTempValue==%d",g_u8MenuTempValue);

	
}




/// Call by ExecuteKeyEvent() to set the g_u8MenuTempValue for MIA_GOTO_ADJUST_ITEM/MIA_REDRAW_MENU action.
///g_u8MenuTempValue is option focus.
static void ReCoverFocusedOption(void)
{

   if(IS_MAIN_L1(g_u8MenuPageIndex))	
   {
      if(g_u8MenuPageIndex==MENU_QUICKSTART)
      {
      /*
       if(g_u8MenuItemIndex==QS_Aspect_Adj_IDX)
       {
            USER_PREF_WIDE_MODE(SrcInputPortM)=g_u8OriginalSetting;
       }
  	else */if(g_u8MenuItemIndex==QS_Source_Adj_IDX)
  	{
	  #if Enable_AutoInput_Menu	
             USER_PREF_INPUT_PRIORITY=g_u8OriginalSetting;
	 #else
             USER_PREF_INPUT_TYPE=g_u8OriginalSetting;
	 #endif
  	}

    	}   
      else if(g_u8MenuPageIndex==MENU_PICTURE)
      {
      /*
         if(g_u8MenuItemIndex==Pic_Aspect_Adj_IDX)
         {
              USER_PREF_WIDE_MODE(SrcInputPortM)=g_u8OriginalSetting;
         }
	*/	 
      }
	/*  
      else if(g_u8MenuPageIndex==MENU_SYSTEM)
      {
         if(g_u8MenuItemIndex==Sys_Language_Adj_IDX)
         {
              USER_PREF_LANGUAGE=g_u8OriginalSetting;
         }
         else if(g_u8MenuItemIndex==Sys_SleepMode_Adj_IDX)
         {
              USER_PREF_SLEEPMODE=g_u8OriginalSetting;
         }
      }
      */
	  

   }


	
}



///1) Reset OSD Timer after drawing OSD.
///2) Reset OSD while any key action accepted.
///3) Some OSD donot reset timer while any key pressed(MIA_NOTHING).
///4) Reset OSD Timer During hold MENU key and released before 15 sec timeout
void ResetOsdTimer(void)
{
    switch(g_u8MenuPageIndex)
    {
        case MENU_POWER_OFF:
        case MENU_POWER_ON:
        case MENU_ROOT:
            g_u16OsdCounter=0;
            break;
        case MENU_UNSUPPORTED_MODE:
        #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
            if(USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                g_u16OsdCounter=5;
            else
        #endif
            g_u16OsdCounter=0;
            break;

        case MENU_CABLE_NOT_CONNECTED:
            g_u16OsdCounter=10;
            break;

        case MENU_STANDBY:
            if (PowerSavingFlag)
                g_u16OsdCounter=0;
            else
                g_u16OsdCounter=5;
            break;

        case MENU_HOTBRIGHTNESS:
        case MENU_HOTCONTRAST:
#if ENABLE_AUDIO_SETTINGS_MENU			
        case MENU_HOTVOLUME:
	 case MENU_HOTMUTE:
#endif			
	 case MENU_HOTPICTUREFORMAT:
 #if SleepMode_Enable	 	
	 case MENU_SLEEPMODE:
 #endif	 	
        case MENU_HOTDCR:
 #if Enable_HotAdjLowbluray			
        case MENU_HOTLOWBLURAY:
 #endif			
	 case MENU_HOTPICTUREMODE:
#if HKC_RGB_Range_Func	 	
	 case MENU_HOTRGBRANGE:
#endif	 	
	 #if (defined(FengYuan_Project))
		g_u16OsdCounter = 2;
	 #else
		g_u16OsdCounter = 5;
	 #endif	
            break;

        case MENU_HOTINPUT:
		g_u16OsdCounter = 5;
            break;

#if 0//LED_HOT_Menu_Func    
        case MENU_HOTLEDMODE:
		g_u16OsdCounter =8; 
            break;
#endif			
			
#if 0//(defined(LianHeChuangXin_Project))
	 case MENU_KEY_GUIDE:
		g_u16OsdCounter = 5;
            break;
#endif			
			
	 case MENU_HOTCROSS:
		g_u16OsdCounter = 0;
            break;
			
#if Enable_Corss_LHCX_New
	 case HotKeyGameSubSelectMenu:
	 case HotKeyGameClokShowMenu:	 	
		g_u16OsdCounter = 0;
            break;
#endif			

        default:
            if (CURRENT_MENU.u8Flags & MPB_OSD_COUNT3)
                {
       #if LOGOTimeOutDelayForVBY1PanelLock  //  160729_02
		g_u16OsdCounter = 3 + g_u8OsdCountOffset;
       #else
                g_u16OsdCounter = 3;
       #endif
                }
            else if (CURRENT_MENU.u8Flags & MPB_OSD_COUNT5)
            {
                g_u16OsdCounter = 5;
            }
            else if (FACTORY_MODE_FLAG)
                g_u16OsdCounter = 0; ///Disable OSD Timeout while Factory mode flag is set.
            else if((g_u8MenuPageIndex!=MENU_STANDBY)&&(g_u8MenuPageIndex!=MENU_CABLE_NOT_CONNECTED)&&(g_u8MenuPageIndex!=MENU_ENTERPOWERSAVING))
                g_u16OsdCounter = USER_PREF_OSD_TIME;
    }
    ResetMsCounter();
    g_stMenuFlag.bOsdTimeout=0;
   //printData("USER_PREF_OSD_TIME==%d",USER_PREF_OSD_TIME);
   //printData("g_u16OsdCounter==%d",g_u16OsdCounter);

}

void KeypadIdle(MenuItemActionType enMenuAction)
{
    if (enMenuAction == MIA_INC_VALUE || enMenuAction == MIA_DEC_VALUE)
    {
    //printMsg("00000000000000000000");
        if (CURRENT_MENU_ITEM.stDisplayValue.pstDrawGauge)  ///Draw Gauge Bar. Handle Gauge Bar Turbo Key
        {
        //printMsg("1111111111111111111111");
            if (CURRENT_MENU_ITEM_FUNC.AdjustFunction == AdjustOSDTimer)
            {
                if (++g_u8TurboKeyCounter>2)
                    //Delay1ms(50);
                    Delay1ms(20);
                else
                    Delay1ms(150);
            }
            else if (++g_u8TurboKeyCounter>2)
                Delay1ms(20);
            else
                Delay1ms(150);
        }
        else    ///No gauge bar. Options selector.
        {
            Delay1ms(200);
        }
    }
    else if (enMenuAction == MIA_GOTO_ADJUST_ITEM|| enMenuAction == MIA_RETURN_SELECT_ITEM)
    {
        g_u8TurboKeyCounter=0;
        Delay1ms(100);
    }
    else if (enMenuAction == MIA_NEXT_ITEM || enMenuAction == MIA_PREV_ITEM)
    {
        g_u8TurboKeyCounter=0;
        if (g_u8MenuPageIndex==MENU_MAIN)
        {
            Delay1ms(200);
        }
        else
            Delay1ms(250);
    }
    else
    {
        g_u8TurboKeyCounter=0;
        Delay1ms(200);
    }
}
///Handle the actions after some OSD Menu Timeout.
void OsdTimeoutHandler(void)
{
    g_stMenuFlag.bOsdTimeout=0;
    g_u8MenuItemIndex = 0;

   printData("OsdTimeoutHandler--MenuPage==%d", g_u8MenuPageIndex);

#if LOGO_ENABLE
    if (g_stMenuFlag.bDisplayLogo)
    {
        g_stMenuFlag.bDisplayLogo=0;
	 Menu_Hide();
	 
     #if PowerSequence_New
	 Power_TurnOffPanel();
     #else	 
	 hw_ClrBlacklit();
     #endif	

	{BYTE u8win;
        for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
        {
            msAPIWinEnableFrameColor(u8win, FALSE);
        }
	}
       mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
	 
	#if (VBY1_PowerSequence)		
	  PowerOnBackLightCounter=0;
	#endif
	 
        g_stMenuFlag.bOsdLoadData=0;       // set flag for reload osd data
        g_u8MenuPageIndex = MENU_ROOT;
        g_u8MenuItemIndex = 0;
        Set_InputTimingChangeFlag(SrcInputPortM);
        SrcFlags(SrcInputPortM) |= SyncLoss;
	 InitOsdAfterLogo();	
    
    #if ENABLE_OSD_LR_DOUBLE
       InitOsdMainWinDouble();
    #endif
	 
    }
    else 
#endif		
#if OSD_LOCK_Enable
    if (g_stMenuFlag.bHoldMenuKey) //White111005 promote Hold menu key priority ,Hold Menu Key for 10 sec
    {
        if (OSD_LOCK_FLAG)
        {
            CLR_OSD_LOCK_FLAG();
            CLR_HOT_OSD_LOCK_FLAG();
        }
        else
        {
            SET_OSD_LOCK_FLAG();
            SET_HOT_OSD_LOCK_FLAG();
            
        }
	Menu_Hide();	
        g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
        #if 0
        SaveMonitorSetting();       // for AC on/off can not save
        #else //Jison 080123
        g_u8SaveUserData|=SAVE_MONITOR_SETTING;
        #endif
	 ResetOsdTimer();

    }
   else	
#endif	
#if LHCX_EnterFactoryMode
    if (g_stMenuFlag.bHoldMenuKey) //White111005 promote Hold menu key priority ,Hold Menu Key for 3 sec
    {
        printMsg("SET_FACTORY_MODE_FLAG");
	if((USER_PREF_BRIGHTNESS==88)&&(g_u8MenuPageIndex==MENU_ROOT||g_u8MenuPageIndex==MENU_KEY_GUIDE))
	{
        SET_FACTORY_MODE_FLAG();
        SET_BURNIN_MODE_FLAG();		
	 Menu_Hide();	
        g_u8MenuPageIndex = FACTORY_MENU;
	 g_u8MenuItemIndex=0;// exit item	
        SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));
	 ResetOsdTimer();
        FactoryExecuteKeyEvent(MIA_REDRAW_MENU);
        return;
	}
    }
   else	
#endif	
#if (defined(RunFeng_Project))
    if (g_stMenuFlag.bHoldMenuKey) //White111005 promote Hold menu key priority ,Hold Menu Key for 3 sec
    {
        printMsg("MIA_AUTO");
	 Menu_Hide();	
	 g_u8MenuPageIndex = MENU_HOT_AUTO;
	 g_u8MenuItemIndex=0;// exit item	
	 g_stMenuFlag.bHoldMenuKey=0;
	 ResetOsdTimer();
    }
   else	
#endif	
#if POWER_KEY_FUNC
    if (g_stMenuFlag.bHoldPowerKey) 
    {
        if (POWER_ON_FLAG)
        {
          g_u8MenuPageIndex = MENU_POWER_OFF;
	   ResetOsdTimer();
        }
    }
   else	
#endif
#if 1//ENABLE_STFC_TIMEOUT_SLEEP
     if ((g_u8MenuPageIndex==MENU_STANDBY)||(g_u8MenuPageIndex==MENU_CABLE_NOT_CONNECTED))
    {
	     g_u8MenuPageIndex=MENU_ENTERPOWERSAVING;
	     Menu_Hide();	
	     //Delay1ms(300);
    }
   else	 
#endif
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
     if ((Is_MultiWin_SyncLoss()&&USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)||((SyncLossState(USER_PREF_INPUT_TYPE)) && (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)))
#else
     if (((SyncLossState(USER_PREF_INPUT_TYPE)) ))
#endif
	{
        //if (CableNotConnectedFlag(USER_PREF_INPUT_TYPE))
        //    g_u8MenuPageIndex = MENU_CABLE_NOT_CONNECTED;
        //else
            g_u8MenuPageIndex = MENU_STANDBY;
		
        g_u8MenuItemIndex=0;
	 Menu_Hide();	
	 ResetOsdTimer();
    }
    else if (UnsupportedModeFlag(USER_PREF_INPUT_TYPE))
    {
        g_u8MenuPageIndex = MENU_UNSUPPORTED_MODE;
	 Menu_Hide();	
	 ResetOsdTimer();
    }
    else
    {
   #if ENABLE_VGA_INPUT
      if(!(USER_PREF_AUTOTIME(USER_PREF_INPUT_TYPE)&AUTO_TIMES_BIT)&&Is_All_VGAPort_DisplayOk_In_MultiWin())
       {
          g_u8MenuPageIndex = MENU_HOT_AUTO;
       }   
	else
   #endif		
          g_u8MenuPageIndex = MENU_ROOT;

     #if Enable_Corss_FY
        if(HotCrossMenuFlag)
        {
            g_u8MenuPageIndex=MENU_HOTCROSS;
            g_u8MenuItemIndex = 0;
        }
     #endif
   
    }

if (g_u8SaveUserData!=SAVE_NONE)
    SaveUserPrefSetting(g_u8MenuPageIndex);

	
    g_u8MenuItemIndex=0;
    g_stMenuFlag.bShowInputIcon=0;	
    ExecuteKeyEvent(MIA_REDRAW_MENU);
}

MenuItemActionType OsdLockRedirector(MenuItemActionType enAction)
{
    MenuItemActionType enMenuAction=enAction;   /// Default value = orignal action

    if (g_u8MenuPageIndex==MENU_STANDBY)
    {
        if (enAction==MIA_SHOW_POWER_SAVE)
            return enAction;
        else if (PowerSavingFlag && enAction==MIA_HotInput)
        {
            mStar_SetupFreeRunMode();
            Power_TurnOffAmberLed();
            Power_TurnOnGreenLed();
            Clr_PowerSavingFlag();
            Power_TurnOnPanel();
        }
    }
#if LOGO_ENABLE	
    else if (g_u8MenuPageIndex==MENU_LOGO) //Jison 080807
        return enAction;
#endif	


#if OSD_LOCK_Enable

#if ENABLE_MULTI_INPUT
    if (HOT_OSD_LOCK_FLAG && enAction != MIA_NOTHING && enAction != MIA_SHOW_POWER_SAVE) //Jison 080310
#else
    if (HOT_OSD_LOCK_FLAG && g_u8MenuPageIndex!=MENU_CABLE_NOT_CONNECTED && enAction != MIA_SHOW_POWER_SAVE)
#endif
    {
        switch(g_u8KeypadButton)
        {
            case BTN_PLUS:
            case BTN_MINUS:
            case BTN_EXIT:
            case BTN_MENU:
            case BTN_INCVALUE:
            case BTN_DECVALUE:
                g_u8MenuPageIndex=MENU_HOT_MENU_LOCK;
                g_u8MenuItemIndex=0;
                enMenuAction=MIA_REDRAW_MENU;
                break;
            case BTN_POWER:
                enMenuAction=MIA_POWER;
                break;
            default:
                enMenuAction=MIA_NOTHING;
        }
    }
#if ENABLE_MULTI_INPUT
    else if (OSD_LOCK_FLAG && enAction != MIA_NOTHING && enAction != MIA_SHOW_POWER_SAVE ) 
#else
    else if (OSD_LOCK_FLAG && g_u8MenuPageIndex!=MENU_CABLE_NOT_CONNECTED && enAction != MIA_SHOW_POWER_SAVE) //cy 0317
#endif
    {
        switch(g_u8KeypadButton)
        {
            case BTN_PLUS:
            case BTN_MINUS:
            case BTN_EXIT:
            case BTN_INCVALUE:
            case BTN_DECVALUE:
                g_u8MenuPageIndex=MENU_HOT_MENU_LOCK;
                g_u8MenuItemIndex=0;
                enMenuAction=MIA_REDRAW_MENU;
                break;

            case BTN_POWER:
                enMenuAction=MIA_POWER;
                break;
            default:
                enMenuAction=MIA_NOTHING;
        }
    }    
#endif

    return enMenuAction;
}

//*************************************************************************
//Function name:        CheckDisplayChange
//Passing parameter:    void
//Return parameter:     void 
//Description:         check window status change and set menu display change flag
//*************************************************************************

void CheckDisplayChange(void)
{
   static EN_WINDOW_STATUS eWin_preDwStatus[DISPLAY_MAX_NUMS] = {0,0,0,0};
   BYTE i;
    
   for(i=0; i< DISPLAY_MAX_NUMS; i++)
   {
     if(g_DisplayWindow[i].eDwStatus != eWin_preDwStatus[i]) 
     {
        g_stMenuFlag.bAnyWinDisplayChange=1;
        //g_stMenuFlag.bWinDisplayChangeStatus[i]=1;
        eWin_preDwStatus[i] = g_DisplayWindow[i].eDwStatus;
     }     
   }

}

void CheckInputPortChange(void)
{ BYTE i;

    for(i=0; i< Get_MultiWin_Number(); i++)
    {
         if((!SyncLossState(MapWin2Port(i)))&&g_stMenuFlag.bSwitchInputPortFlag[i])
	  { 
	       g_stMenuFlag.bSwitchInputPortFlag[i]=0;
              if(g_stMenuFlag.bSwitchInputPortStatus[i] !=USER_PREF_WIN_SRC(i))
              {
	         g_stMenuFlag.bDisplayInputMsg[i] = 1;
//	         g_stMenuFlag.bDisplayInputStatus[i] = 0;
              }
	  }
	 //MENU_printData("\r\n AAAAAA----win=%d,SavePort=%d,InputPort=%d,Display=%d",i,USER_PREF_WIN_SRC(i),g_stMenuFlag.bSwitchInputPortStatus[i],g_stMenuFlag.bDisplayInputMsg[i] );
    }

}



void Menu_OsdHandler(void)
{
    MenuItemActionType enMenuAction=MIA_NOTHING;

    if (g_u8MenuPageIndex>=0x80)
        return;

    //CheckDisplayChange();

    if( g_stMenuFlag.bAnyWinDisplayChange
#if LOGO_ENABLE
	&&!g_stMenuFlag.bDisplayLogo
#endif
)
    {
       printData("Is_MultiWin_SyncLoss==%d",Is_MultiWin_SyncLoss());
       printData("SyncLossState(USER_PREF_INPUT_TYPE)==%d",SyncLossState(USER_PREF_INPUT_TYPE));
       //printData("Is_DisplayOk_In_MultiWin==%d",Is_DisplayOk_In_MultiWin());
        g_u8SyncLossOsdCounter=0;
        g_stMenuFlag.bAnyWinDisplayChange=0; 

#if 0
     g_stMenuFlag.bShowChangeMessage=1;
#else		
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU		
    if(USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
    {
	if ((!Is_MultiWin_SyncLoss())&&(Is_DisplayOk_In_MultiWin()))
	{
           g_stMenuFlag.bShowChangeMessage=0;
	if(USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_3)	   
   	   g_u8SyncLossOsdCounter=60;
	else
   	   g_u8SyncLossOsdCounter=40;
	   printData("g_u8SyncLossOsdCounter 3333==%d",g_u8SyncLossOsdCounter);
	}	 
	else
	{
	   g_stMenuFlag.bShowChangeMessage=0;
	if(USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_3)	   
   	   g_u8SyncLossOsdCounter=60;
	else
   	   g_u8SyncLossOsdCounter=40;
	   printData("g_u8SyncLossOsdCounter==%d",g_u8SyncLossOsdCounter);
	}
    }
    else
#endif	
	   g_stMenuFlag.bShowChangeMessage=1;	
#endif


}

    if( g_stMenuFlag.bShowChangeMessage&&BacklightFlag
#if LOGO_ENABLE
&&!g_stMenuFlag.bDisplayLogo
#endif
	)
    {
        CheckInputPortChange();
        g_stMenuFlag.bShowChangeMessage=0;
	 #if (!ZUI)
	 #if Free_Sync_Enable
	 if((OSDFreeSyncActionFlag)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(((g_u8MenuPageIndex == MENU_QUICKSTART)&&(g_u8MenuItemIndex==QS_FreeSync_Adj_IDX))||((g_u8MenuPageIndex == MENU_SYSTEM)&&(g_u8MenuItemIndex==Sys_FreeSync_Adj_IDX))))
	 {
          Menu_Show();
	   OSDActiveCounter=300;	  
          //Clr_OSDFreeSyncActionFlag();
	 }
        else
	 #endif
        { 
           Clr_OSDFreeSyncActionFlag();
           Menu_InitAction(); // initial menu osd state
        }
       #endif
        return;
    }


    Key_ScanKeypad();   /// Get Button pressed and save to g_u8KeypadButton.

/// Map gu8KeypadButton to MenuItemActionType MIA_XXXXX
    if (g_u8KeypadButton >= BTN_END_BTN)
    {
        enMenuAction = MIA_NOTHING;
    }
    else if(DDCCI_FORCE_POWER_SAVE_FLAG && g_u8KeypadButton<BTN_POWER ) //White110307 fix button key behavior for MCCS Reduced Power off
    {
        Set_InputTimingChangeFlag(USER_PREF_INPUT_TYPE);
        return;
    }
#if 0//ENABLE_STFC_TIMEOUT_SLEEP
    else if (PowerSavingFlag && g_u8MenuPageIndex==MENU_CABLE_NOT_CONNECTED && g_u8KeypadButton<BTN_POWER)
    {
        g_u8KeypadButton=BTN_NOTHING;
        Power_TurnOffAmberLed();
        Power_TurnOnGreenLed();
        Clr_InputTimingChangeFlag(USER_PREF_INPUT_TYPE);
        Power_TurnOnPanel();
        enMenuAction = MIA_REDRAW_MENU;
    }
#endif
#if 0 //Mike110622
    else if (FACTORY_MODE_FLAG && g_u8MenuPageIndex==MENU_ROOT && g_u8KeypadButton==BTN_PLUS)
    {
        g_stMenuFlag.bShowFactoryMenu=1;
        return;
    }
#endif
    else if (CURRENT_MENU_ITEM.penKeyEvents)
    {
        enMenuAction = * ((CURRENT_MENU_ITEM.penKeyEvents) + g_u8KeypadButton);
    }

    if (!InputTimingStableFlag(SrcInputPortM)
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
        && (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
#endif
        &&(g_u8MenuPageIndex != MENU_HOTINPUT)
    ) // prevent osd action except BTN_POWER while mode changing
    {
        if (enMenuAction != MIA_POWER)
            enMenuAction = MIA_NOTHING;
    }

    enMenuAction=OsdLockRedirector(enMenuAction);

#if Enable_Corss_LHCX_New
	if((HotGameTimeShowFlag)&&(HotGameTimeUpdateFlag)&&(g_u8MenuPageIndex==HotKeyGameClokShowMenu)&&(enMenuAction==MIA_NOTHING)&&(!(FreeRunModeFlag(MapPort2DetScIndex(USER_PREF_INPUT_TYPE))))) //menuAction!=MIA_GotoNext&&menuAction!=MIA_GotoPrev
	{
	       Clr_HotGameTimeUpdateFlag();
		DrawHotKeyGameClokShow();   
		//enMenuAction=MIA_HotGameClokShow;
	}
#endif

/// Call ExecuteKeyEvent() to dispatch action and reset OSD timer.
    if (enMenuAction)
    {

    #if SleepMode_Enable
      if((OSDShowFlag)&&(g_u8MenuPageIndex == MENU_POWEROFFMSG))
      {
        Menu_Hide();
        USER_PREF_SLEEPMODE=SleepMode_Off;
        SleepModeCounter=0;
        Clr_ForceEnterPowerOffFlag();
        Clr_ForceEnterPowerOffMsgFlag();	
        Clr_EnterPowerOffMsgChangeNumFlag();
    	
      }
    #endif
	
        ResetOsdTimer();    /// A valid key pressed.
        ExecuteKeyEvent(enMenuAction);
        KeypadIdle(enMenuAction);
    }
    else
        g_u8TurboKeyCounter=0;  ///Reset counter if MIA_NOTHING

    if (g_stMenuFlag.bOsdTimeout)
        OsdTimeoutHandler();

    if (g_u8MenuPageIndex == MENU_CABLE_NOT_CONNECTED && !PowerSavingFlag)  // && FreeRunModeFlag) // for led flash
    {
        if (g_stMenuFlag.bMoveOsd)
        {
            g_stMenuFlag.bMoveOsd=0;
            MoveOsdWindow();
            msAPIOSDBlendFBSwitch(ENABLE);
        }
    }
#if 0//ENABLE_MULTI_INPUT
    else if (g_u8MenuPageIndex==MENU_INPUT && g_stMenuFlag.bPressSrcInputKey && g_u16OsdCounter<=1) /// There r two cases to show InputMenu 1) Power On, 2) Pressed Input SRC Key.
    {///Switch input src after showing MENU_INPUT over 2 sec
        msAPIOSDBlendFBSwitch(DISABLE);
        if (FreeRunModeFlag(USER_PREF_INPUT_TYPE))
        {
	     SwitchPort(USER_PREF_WIN_SEL, g_u8MenuTempValue);		
            g_u8PowerDownCounter = 0;
            Menu_ClearOsdCounterAndFlag();
        }
        g_stMenuFlag.bPressSrcInputKey=0;
    }
#endif
    else
        msAPIOSDBlendFBSwitch(DISABLE);

#if SleepMode_Enable
  if((ForceEnterPowerOffMsgFlag)&&(SleepModeCounter!=0)&&(BacklightFlag)
  #if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
  	&&(!((Is_MultiWin_SyncLoss()&&USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)||((SyncLossState(USER_PREF_INPUT_TYPE)) && USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)))
  #else
  	&&(!(SyncLossState(USER_PREF_INPUT_TYPE)) )
  #endif
  )
  {
    BYTE xdata tNumberText[2];

    if((OSDShowFlag)&&(g_u8MenuPageIndex == MENU_POWEROFFMSG))
    {
     if(EnterPowerOffMsgChangeNumFlag)
     {
      Clr_EnterPowerOffMsgChangeNumFlag();
      g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BK_COLOR_L0);

      tNumberText[1]=LoadPropNumber(PROP_NUM_START, SleepModeCounter,NUM_DRAW_ARROW);
      tNumberText[0]=PROP_NUM_START;
      Osd_DrawContinuousChar(16, 2, SPACE_FONT, 2);//clear
      DrawPropStr(16, 2,tNumberText );
     }
    }
   else 
    {
        g_u8MenuPageIndex = MENU_POWEROFFMSG;
        g_u8MenuItemIndex = 0;
        ExecuteKeyEvent(MIA_REDRAW_MENU);
        g_u16OsdCounter = 0;
        g_stMenuFlag.bOsdTimeout=0;
        ResetMsCounter();
    }

  }
#endif	


    if (DDCCI_AUTO_ADJUST_FLAG)
    {
        CLR_DDCCI_AUTO_ADJUST_FLAG();
    #if ENABLE_VGA_INPUT
        AutoConfig();
        g_u8MenuPageIndex = MENU_ROOT;
        g_u8MenuItemIndex = 0;
        ExecuteKeyEvent(MIA_REDRAW_MENU);
    #endif
    }


#if ENABLE_DEMURA_FUNCTION
    if (g_u8MenuPageIndex == MENU_DEMURA_MEASURE_MODE)
    {
        EnterDeMuraMeasureMode();
    }
#endif


}


Bool ExecuteKeyEvent(MenuItemActionType enMenuAction)
{
    Bool  xdata  IsProcessEvent = TRUE;
    Bool  xdata  result;
    BYTE  xdata  u8TempValue = 0;
    BYTE  xdata  u8Rlt = 0;

    while (IsProcessEvent && g_u8MenuPageIndex<0x80)
    {
        IsProcessEvent = FALSE;
        switch (enMenuAction)
        {
        case MIA_INC_VALUE:
        case MIA_DEC_VALUE:

            if (CURRENT_MENU_ITEM_FUNC.AdjustFunction)
            {

                if (CURRENT_MENU_ITEM_FUNC.AdjustFunction(enMenuAction))
                {
                    BYTE i;

                   if(IS_MAIN_L1(g_u8MenuPageIndex))
                   {
                       g_stMenuFlag.bThisItemSelected=1;
                   }
		    if(((g_u8MenuPageIndex==MENU_QUICKSTART)&&(g_u8MenuItemIndex==QS_Preset_Adj_IDX))||((g_u8MenuPageIndex==MENU_PICTURE)&&(g_u8MenuItemIndex==Pic_Preset_Adj_IDX||g_u8MenuItemIndex==Pic_DCR_Adj_IDX)))
		    {
                        DrawOsdMenuItem(QS_BackLight_IDX, &CURRENT_MENU.pstMenuItems[QS_BackLight_IDX]);
                        DrawOsdMenuItem(QS_Brightness_IDX, &CURRENT_MENU.pstMenuItems[QS_Brightness_IDX]);
                        DrawOsdMenuItem(QS_Contrast_IDX, &CURRENT_MENU.pstMenuItems[QS_Contrast_IDX]);
			if((g_u8MenuPageIndex==MENU_PICTURE)&&(g_u8MenuItemIndex==Pic_Preset_Adj_IDX))
			{
                        DrawOsdMenuItem(Pic_DCR_IDX, &CURRENT_MENU.pstMenuItems[Pic_DCR_IDX]);
			}
			
		    }
                  else if((g_u8MenuPageIndex==MENU_COLOR)&&(g_u8MenuItemIndex==Col_Temperature_Adj_IDX))
                   {
			     g_stMenuFlag.bEnableItemSelect=1;	   
			/*	 
                        if(CURRENT_COLOR_TEMP==PCT_Advanced)
                        {
                            for (i = 0; i < tblMenus[MENU_ADVANCED].u8MenuItemCount; i++)
                            {   
                              if ((tblMenus[MENU_ADVANCED].pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
                              continue; //Don't draw Adjust items in L2
                              DrawOsdMenuItem(i, &tblMenus[MENU_ADVANCED].pstMenuItems[i]);
                            }
							
                           g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
                           
                           for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+5); i++)
                           {
                            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                           }	
                            LoadMenuDescriptionPropFont();
                            DrawDescription(g_u8MenuPageIndex);
                        }
                        else
			*/				
                        {
                            for (i = 0; i < tblMenus[MENU_USER].u8MenuItemCount; i++)
                            {   
                              if ((tblMenus[MENU_USER].pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
                              continue; //Don't draw Adjust items in L2
                              DrawOsdMenuItem(i, &tblMenus[MENU_USER].pstMenuItems[i]);
                            }

                           DrawOsdMenuItem(Col_Tint_IDX, &tblMenus[MENU_COLOR].pstMenuItems[Col_Tint_IDX]);
                           DrawOsdMenuItem(Col_Saturation_IDX, &tblMenus[MENU_COLOR].pstMenuItems[Col_Saturation_IDX]);
                           //DrawOsdMenuItem(Advanced_Yellow_IDX, &tblMenus[MENU_ADVANCED].pstMenuItems[Advanced_Yellow_IDX]);
                           for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+5); i++)
                           {
                            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                           }	
                            LoadMenuDescriptionPropFont();
                            DrawDescription(g_u8MenuPageIndex);
							
                        }
			   g_stMenuFlag.bEnableItemSelect=0;			
                   }
		    else if((g_u8MenuPageIndex==MENU_SYSTEM
               #if LED_HOT_Menu_Func
			||g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER
               #endif
			)&&(g_u8MenuItemIndex==Sys_Language_Adj_IDX))
		    {
                           BYTE g_u8MenuTempIndex_Prev=g_u8MenuItemIndex;
                           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
                           
                           for (i=(SystemTxtYPos); i<((SystemTxtYPos)+13); i++)
                           {
                            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                           }	
                
                           g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_BK_COLOR_H);
                	     OSD_TEXT_HI_ADDR_SET_BIT8();		
                           for(i=(SystemTxtYPos); i<((SystemTxtYPos)+13); i++)
                           {
                            Osd_DrawRealChar(0, i, MAIN_Left_Line);
                           }
                           for(i=(SystemTxtYPos); i<((SystemTxtYPos)+13); i++)
                           {
                            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
                           }
                           OSD_TEXT_HI_ADDR_CLR_TO_0();
					   
                           g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
                           
                           for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+5); i++)
                           {
                            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                           }	

                         //======================================
                         //Clear "Current Input Source" "Menu Up Down Exit Power"	
                         /*
                            g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
                            // Osd_DrawContinuousChar(1, (Main_TopLine_YPos-1), SPACE_FONT, 0x10);
                            for (i=(Main_Buttom_YPos+1); i<(Main_Buttom_YPos+2); i++)
                            {
                             Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                            }	
				*/			
                         //======================================
                         //Clear Main Text
                         //======================================
                           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_BK_COLOR_H);
                           for (i=(Main_TopLine_YPos+1); i<(Main_TopLine_YPos+1+11); i++)
                           {
                            Osd_DrawContinuousChar(9, i, SPACE_FONT, 22);
                           }	
                           g_u8OsdFontColor=MONO_COLOR(CP_SubGround,CP_SubGround);
                           for (i=(Main_TopLine_YPos+14); i<(Main_TopLine_YPos+14+2); i++)
                           {
                            Osd_DrawContinuousChar(9, i, SPACE_FONT, 22);
                           }	

                           g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
                           for (i=(Main_Description_YPos+1); i<(Main_Description_YPos+3); i++)
                           {
                            Osd_DrawContinuousChar(1, i, SPACE_FONT, 15);
                           }	
						   
						   
				LoadMainTextFont();

				ReDrawMainStringText();		 
                         //======================================
						  

				SetLanguage();
                           if (CURRENT_MENU.LoadFont!=NULL)
                           {
                              CURRENT_MENU.LoadFont();
                           }
                          for (i = 0; i < MENU_ITEM_COUNT; i++)
                          {   
                              if ((CURRENT_MENU.pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
                                  continue; //Don't draw Adjust items in L2
                              if (i!=g_u8MenuItemIndex)
                                  DrawOsdMenuItem(i, &CURRENT_MENU.pstMenuItems[i]);
                          }
		 #if LED_HOT_Menu_Func	   
			  if(g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER)			  
                        {
                          g_u8MenuTempIndex_Prev=g_u8MenuPageIndex;
                          g_u8MenuPageIndex=MENU_LEDMODE;   
                          DrawLedMenuItems();
                          g_u8MenuPageIndex=g_u8MenuTempIndex_Prev; 
                        }
              #endif          
                       //DrawGuideText();
                       g_u8MenuTempIndex_Prev=g_u8MenuItemIndex;
                        g_u8MenuItemIndex=Sys_Language_IDX;
                        DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU.pstMenuItems[g_u8MenuItemIndex]);
			   OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8			
			  if(g_u8MenuPageIndex==MENU_SYSTEM) 
			  {

                        Osd_DrawRealChar(21, (SystemTxtYPos+System_Language_IDX*1), ARROW_LEFT); 
                        Osd_DrawRealChar(30, (SystemTxtYPos+System_Language_IDX*1), ARROW_RIGHT); 
			  }
			  else
			  {
                        Osd_DrawRealChar(21, (SystemTxtYPos+4+System_Language_IDX*1), ARROW_LEFT); 
                        Osd_DrawRealChar(30, (SystemTxtYPos+4+System_Language_IDX*1), ARROW_RIGHT); 
			  }
			   OSD_TEXT_HI_ADDR_CLR_TO_0(); //enable bit 8			
						
                        g_u8MenuItemIndex=g_u8MenuTempIndex_Prev;
                        DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU.pstMenuItems[g_u8MenuItemIndex]);

                        LoadMenuDescriptionPropFont();
          
                        DrawDescription(g_u8MenuPageIndex);

						 
		    }
			#if LED_HOT_Menu_Func
		   else if((g_u8MenuPageIndex==MENU_SYSTEM||g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER)&&(g_u8MenuItemIndex==Sys_LedMode_Adj_IDX))
		   	{
                        //DrawOsdMenuItemRadioGroup(0, DrawLedColorRadioGroup);
        
                       //Clear System Select
                      if(USER_PREF_LEDMODE_INDEX==LedModeIndex_User) 
                      {
                       g_stMenuFlag.bEnableItemSelect=1;
                       g_u8MenuPageIndex=MENU_SYSTEMLLEDUSER;
			  DrawSystemMenuFrame(0);		
			  
			  DrawLedMenuItems();
                      }
			else
			{
                       g_stMenuFlag.bEnableItemSelect=0;
			 if(g_u8MenuPageIndex!=MENU_SYSTEM)	
			 {
                       g_u8MenuPageIndex=MENU_SYSTEM;
			  DrawSystemMenuFrame(0);		   
			 }
			}
                       
                       g_stMenuFlag.bEnableItemSelect=0;
        
                       {
                         u8TempValue=g_u8MenuItemIndex;
        		    g_u8MenuItemIndex=0xFE;		 
                         DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
        		   g_u8MenuItemIndex=u8TempValue;		 
                       }
        		//Osd_DrawContinuousChar(GroupText_XPos, (SystemTxtYPos+System_LedMode_IDX*1), SPACE_FONT, 6);//Clear LedMode >>
        		
                       //g_u8MenuPageIndex = MENU_LEDMODE;
                       //enMenuAction = MIA_GOTO_NEXT1;
                       //IsProcessEvent = TRUE;
						
		   	}
		  #endif   
                    DrawOsdMenuItemValue(g_u8MenuItemIndex, &CURRENT_MENU_ITEM.stDisplayValue);


                    g_stMenuFlag.bThisItemSelected=0;
				  
                      g_u8SaveUserData|=SAVE_MONITOR_SETTING;
                }
                else
                {

                }

	#if 0//ENABLE_AUDIO_SETTINGS_MENU		
                if((CURRENT_MENU_ITEM_FUNC.AdjustFunction==AdjustVolume)
          #if (defined(HuaKe_Project))
		&&(enMenuAction==MIA_INC_VALUE)  
	   #endif			 
		)
                {
                 if(AUDIO_MUTE_FLAG)
                 {
                   CLR_AUDIO_MUTE_FLAG();
                   SetAudioMuteOnOff();
                 }
                }
	#endif	
	
	   ResetOsdTimer();
	}

       break;

        case MIA_NEXT_ITEM:
        case MIA_PREV_ITEM:
            u8TempValue = g_u8MenuItemIndex;
		 
            if (enMenuAction == MIA_NEXT_ITEM)
                g_u8MenuItemIndex = GetNextItem(CURRENT_MENU_ITEMS);
            else
                g_u8MenuItemIndex = GetPrevItem(CURRENT_MENU_ITEMS);

	       g_u8PrevMenuItemIndex=g_u8MenuItemIndex;

			
		printData("MIA_ u8TempValue==%d",u8TempValue);
		printData("MIA_ g_u8MenuItemIndex==%d",g_u8MenuItemIndex);

		  
            if (u8TempValue != g_u8MenuItemIndex)
            {				
                DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);


             if(IS_MAIN_L0(g_u8MenuPageIndex))
             {
                {BYTE i;
                    g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
                    
                    for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+7); i++)
                    {
                    Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                    }	
                }
                  LoadMainMenuFont();

                 DrawDescription(g_u8MenuPageIndex);
				  
             }
             else if(IS_MAIN_L1(g_u8MenuPageIndex))
             {
               BYTE i;

             if((g_u8MenuPageIndex!=MENU_ADVANCED))
	       {
                g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
                
                for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+5); i++)
                {
                 Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                }	
                LoadMenuDescriptionPropFont();
 
                DrawDescription(g_u8MenuPageIndex);
             	 }
			   
             }
				
               DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);

		  
               g_u8MenuTempValue=0;
		
    #if Enable_Corss_LHCX_New
	 if(/*MenuPageIndex==HotKeyGameClokMenu||*/g_u8MenuPageIndex==HotKeyGameClokShowMenu)
		{
		//KeypadButton=BTN_Repeat;
		//Delay1ms(20);
		//MenuItemIndex=MenuItemIndex++;
			if(g_u8MenuItemIndex==HotKeyGameClok_30MItem)
				GameModeTime_M=30;
			else if(g_u8MenuItemIndex==HotKeyGameClok_40MItem)
				GameModeTime_M=40;
			else if(g_u8MenuItemIndex==HotKeyGameClok_50MItem)
				GameModeTime_M=50;
			else if(g_u8MenuItemIndex==HotKeyGameClok_60MItem)
				GameModeTime_M=60;
			else if(g_u8MenuItemIndex==HotKeyGameClok_90MItem)
				GameModeTime_M=90;
				GameModeTime_S=0;
			//enMenuAction=MIA_HotGameClokShow;
			//IsProcessEvent=TRUE;
		}
    #endif
				
            }
            break;

        case MIA_GOTO_ADJUST_ITEM:
        case MIA_RETURN_SELECT_ITEM:
        case MIA_UNDO_RETURN_SELECT_ITEM:

            u8TempValue = g_u8MenuItemIndex;
            Clr_OSDFreeSyncActionFlag();
           if( (g_u8MenuPageIndex==MENU_COLOR)&&(g_u8MenuItemIndex==Col_Temperature_Adj_IDX)&&(enMenuAction==MIA_RETURN_SELECT_ITEM )&&(CURRENT_COLOR_TEMP==PCT_USER/*||CURRENT_COLOR_TEMP==PCT_Advanced*/))
           {

                //Clear Color Select
             u8TempValue=g_u8MenuItemIndex;
             g_u8MenuItemIndex=0xFE;		 
	      DrawItemCursor(u8TempValue,CS_NORMAL);		 
	      DrawOsdMenuItem(Col_Temperature_IDX, &CURRENT_MENU.pstMenuItems[Col_Temperature_IDX]);  
             //DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
             g_u8MenuItemIndex=u8TempValue;		 

              g_u8MenuPageIndex = MENU_USER;
              enMenuAction = MIA_GOTO_NEXT1;
              g_u8MenuItemIndex = 0;
              IsProcessEvent = TRUE;
              break;
           }
	    else if( (g_u8MenuPageIndex==MENU_USER)&&(g_u8MenuItemIndex==User_RGBCMY_IDX)&&(enMenuAction==MIA_GOTO_ADJUST_ITEM )&&(CURRENT_COLOR_TEMP==PCT_USER))   
           {
               //Clear Color Select
               g_stMenuFlag.bEnableItemSelect=1; 
               g_stMenuFlag.bEnableItemSelectSub=1; 
			   
               g_u8MenuPageIndex=MENU_COLOR;
               u8TempValue=g_u8MenuItemIndex;
               g_u8MenuItemIndex=0xFE;		 
               DrawColorMenuFrame(0);
 		 //g_u8MenuItemIndex=u8TempValue;		 
               g_stMenuFlag.bEnableItemSelect=0;
               g_stMenuFlag.bEnableItemSelectSub=0; 

               {
 		   g_u8MenuPageIndex=MENU_USER;	   
                 //u8TempValue=g_u8MenuItemIndex;
		   //g_u8MenuItemIndex=0xFE;		 
                 DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
		   //g_u8MenuItemIndex=u8TempValue;		 
               }
		Osd_DrawContinuousChar(GroupText_XPos, (ColorUserTxtYPos+User_RGBCMY_ItemIDX*1), SPACE_FONT, 6);//Clear >>
		
               g_u8MenuPageIndex = MENU_ADVANCED;
               enMenuAction = MIA_GOTO_NEXT1;
               IsProcessEvent = TRUE;
			  
              break;
           }
       #if LED_HOT_Menu_Func
	    else if((enMenuAction==MIA_RETURN_SELECT_ITEM)&&(g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER)&&(g_u8MenuItemIndex==Sys_LedMode_Adj_IDX))	   
	    {
               //Clear MENU_SYSTEMLLEDUSER Select
               //g_stMenuFlag.bEnableItemSelect=1;
               //DrawSystemMenuFrame(0);
               //g_stMenuFlag.bEnableItemSelect=0;

               {
                 u8TempValue=Sys_LedMode_IDX;
		   g_u8MenuItemIndex=0xFE;		 
                 DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
		   g_u8MenuItemIndex=u8TempValue;		 
               }
		//Osd_DrawContinuousChar(GroupText_XPos, (SystemTxtYPos+System_LedMode_IDX*1), SPACE_FONT, 6);//Clear LedMode >>
		
               g_u8MenuPageIndex = MENU_LEDMODE;
               enMenuAction = MIA_GOTO_NEXT1;
               IsProcessEvent = TRUE;

              break;

	    }
     #endif		
		
	     printData("g_u8MenuPageIndex==%d", g_u8MenuPageIndex);
		
            if (enMenuAction == MIA_GOTO_ADJUST_ITEM)
            {
                g_u8MenuItemIndex += 1;

               InitAdjusterFocusedOption(); //Init g_u8MenuTempValue
            }
            else    ///MIA_RETURN_SELECT_ITEM or MIA_UNDO_RETURN_SELECT_ITEM
            {

                ReCoverFocusedOption();			
                g_u8MenuItemIndex -=1;
				
            }


            if (u8TempValue!=g_u8MenuItemIndex)
            {
                if (enMenuAction==MIA_RETURN_SELECT_ITEM || enMenuAction==MIA_UNDO_RETURN_SELECT_ITEM)
                {

                    if (enMenuAction==MIA_RETURN_SELECT_ITEM && CURRENT_MENU.pstMenuItems[u8TempValue].stKeyFunction.ExecFunction)
                    {
                        result=CURRENT_MENU.pstMenuItems[u8TempValue].stKeyFunction.ExecFunction();
						
                    }
                    else  //MIA_UNDO_RETURN_SELECT_ITEM
                    {

                    }

                   if((result==TRUE)&&(CURRENT_MENU.pstMenuItems[u8TempValue].stKeyFunction.ExecFunction==ResetAllSetting))
                   	{
                 #if 1
                          g_u8MenuPageIndex = MENU_ROOT;
                          enMenuAction = MIA_REDRAW_MENU;
                          g_u8MenuItemIndex = 0;
                          IsProcessEvent = TRUE;
                          break;

                  #else
                           BYTE i;
                           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
                           
                           for (i=(SystemTxtYPos); i<((SystemTxtYPos)+12); i++)
                           {
                            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                           }	
                
                           g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_BK_COLOR_H);
                	      OSD_TEXT_HI_ADDR_SET_BIT8();	
                           for(i=(SystemTxtYPos); i<((SystemTxtYPos)+12); i++)
                           {
                            Osd_DrawRealChar(0, i, MAIN_Left_Line);
                           }
                           for(i=(SystemTxtYPos); i<((SystemTxtYPos)+12); i++)
                           {
                            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
                           }
				OSD_TEXT_HI_ADDR_CLR_TO_0();		   
                          g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
                          
                          for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+5); i++)
                          {
                           Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                          }	

                          //======================================
                         //Clear "Current Input Source" "Menu Up Down Exit Power"	
                            g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
                             Osd_DrawContinuousChar(1, (Main_TopLine_YPos-1), SPACE_FONT, 0x10);
                            for (i=(Main_Buttom_YPos+1); i<(Main_Buttom_YPos+2); i++)
                            {
                             Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                            }	
                         //======================================

				SetLanguage();
                           if (CURRENT_MENU.LoadFont!=NULL)
                           {
                              CURRENT_MENU.LoadFont();
                           }
				LoadMenuDescriptionPropFont();
                          for (i = 0; i < MENU_ITEM_COUNT; i++)
                          {   
                              if ((CURRENT_MENU.pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
                                  continue; //Don't draw Adjust items in L2
                              if (i!=g_u8MenuItemIndex)
                                  DrawOsdMenuItem(i, &CURRENT_MENU.pstMenuItems[i]);
                          }

			   DrawGuideText();
			   DrawDescription(g_u8MenuPageIndex);
                        DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
                        DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
                        g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
			   OSD_TEXT_HI_ADDR_SET_BIT8();			
                        Osd_DrawRealChar(0,0,MAIN_Top_Left);//Clear "F" Redarw topleft Frame
                        OSD_TEXT_HI_ADDR_CLR_TO_0();
			   Menu_Show();
		#endif	   
                   	}
			else	   
                    {
                     if(!IS_MAIN_L1(g_u8MenuPageIndex))
                        DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
                        DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
                    }

                }
                else    //MIA_GOTO_ADJUST_ITEM
                {
                       /*
                   if(IS_MAIN_L1(g_u8MenuPageIndex))
                    {
                        DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
                        DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
                    }
		      else	
			*/  	
                    {
                     if(!IS_MAIN_L1(g_u8MenuPageIndex))
                       DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
                        DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
                    }
                }
            }
            g_u8KeypadButton = BTN_REPEAT;
            break;


        case MIA_GOTO_NEXT1:
        case MIA_GOTO_PREV1:

             if(enMenuAction==MIA_GOTO_NEXT1)
             	{
               BYTE i=0;

		 if(/*g_u8MenuPageIndex == MENU_LEDMODE||*/g_u8MenuPageIndex == MENU_ADVANCED)
		 {
                  for (i = 0; i < MENU_ITEM_COUNT; i++)
                  {   
                      if ((CURRENT_MENU.pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
                          continue; //Don't draw Adjust items in L2
                      if (i!=g_u8MenuItemIndex)
                          DrawOsdMenuItem(i, &CURRENT_MENU.pstMenuItems[i]);
                  }	
                				  
		 }
				  
                 g_u8MenuItemIndex=0;
                 DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
	#if LED_HOT_Menu_Func	
              if(g_u8MenuPageIndex == MENU_LEDMODE)
               {
                 g_u8OsdFontColor=MONO_COLOR(CP_MainGround, CP_MainGround);
                 
                 for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+5); i++)
                 {
                    Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                 }	
                    LoadMenuDescriptionPropFont();
                    DrawDescription(g_u8MenuPageIndex);
                }
	#endif		 
             	}
		else
		{
		  BYTE i;
                   u8TempValue=g_u8MenuItemIndex;
                   g_u8MenuItemIndex=0xFE;		 
                   DrawOsdMenuItem(u8TempValue, &CURRENT_MENU_ITEMS[u8TempValue]);
                   
                   if((g_u8MenuPageIndex == MENU_USER)/*||(g_u8MenuPageIndex == MENU_ADVANCED)*/)
                   {
                      g_u8MenuPageIndex = MENU_COLOR;
                      g_u8MenuItemIndex = Col_Temperature_IDX;
			 g_stMenuFlag.bEnableItemSelect=0;		   
                      //DrawItemCursor(Col_Temperature_IDX,CS_SELECTED);		 
                      //DrawOsdMenuItem(Col_Temperature_IDX, &CURRENT_MENU.pstMenuItems[Col_Temperature_IDX]);  
                      DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
                      g_u8MenuItemIndex = Col_Temperature_Adj_IDX;
                      DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
			 InitAdjusterFocusedOption(); //Init g_u8MenuTempValue		 
                  }
	   #if LED_HOT_Menu_Func
                   else if(g_u8MenuPageIndex == MENU_LEDMODE)
                   {
                       g_u8MenuPageIndex = MENU_SYSTEMLLEDUSER;
                       g_u8MenuItemIndex = Sys_LedMode_IDX;
                       DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
                       g_u8MenuItemIndex = Sys_LedMode_Adj_IDX;
                       DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
					   
                      g_u8OsdFontColor=MONO_COLOR(CP_MainGround, CP_MainGround);
                      
                      for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+5); i++)
                      {
                         Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                      }	
                         LoadMenuDescriptionPropFont();
                         DrawDescription(g_u8MenuPageIndex);
                   }
	   #endif			   
                   else if(g_u8MenuPageIndex == MENU_ADVANCED)
                   {
                       g_u8MenuPageIndex = MENU_COLOR;
                       g_u8MenuItemIndex = 0xFE;
                       DrawColorMenuFrame(0);
                       g_u8MenuPageIndex = MENU_USER;
                       g_u8MenuItemIndex = User_RGBCMY_IDX;
                       DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU_ITEM);
                      g_u8OsdFontColor=MONO_COLOR(CP_MainGround, CP_MainGround);
                      /*
                      for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+5); i++)
                      {
                         Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                      }	
                         LoadMenuDescriptionPropFont();
                         DrawDescription(g_u8MenuPageIndex);
			 */			 
                   }
		     		   
                   //IsProcessEvent = FALSE;
		}
            g_u8KeypadButton = BTN_REPEAT;
		
            break;

        case MIA_GOTO_NEXT:
        case MIA_GOTO_PREV:
		   
                if (((!BURNIN_MODE_FLAG)&&(FreeRunModeFlag(MapPort2DetScIndex(USER_PREF_INPUT_TYPE))&&SyncLossState(USER_PREF_INPUT_TYPE))
	    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU	  	
		  	&&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
	    #endif	  	
		  )
	    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU	  	
               ||((USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)&&Is_MultiWin_SyncLoss())
          #endif     
		)
                {
                    g_u8MenuPageIndex=MENU_STANDBY;
		      g_u8MenuItemIndex=0;			
                    enMenuAction=MIA_REDRAW_MENU;               
                    break;
                }
				
                if(g_u8MenuPageIndex==MENU_MAIN&&enMenuAction==MIA_GOTO_NEXT)
                {
                    if (BURNIN_MODE_FLAG && SyncLossState(USER_PREF_INPUT_TYPE))
                       break;
                }
		  else if((g_u8MenuPageIndex==BURNIN_MENU)&&(enMenuAction==MIA_GOTO_NEXT)&&(BURNIN_MODE_FLAG && FreeRunModeFlag(MapPort2DetScIndex(USER_PREF_INPUT_TYPE))))
		  {
		       Menu_Hide();
                     InitOsdAfterLogo();
		  }
               if((g_u8MenuPageIndex==MENU_MAIN)&&(enMenuAction==MIA_GOTO_PREV)&&(BURNIN_MODE_FLAG && SyncLossState(USER_PREF_INPUT_TYPE)))
               {
                    g_u8MenuPageIndex=BURNIN_MENU;
		      g_u8MenuItemIndex=0;			
                    g_stMenuFlag.bOsdTimeout=0;
		      g_u16OsdCounter = 0;			
                    g_stMenuFlag.bChangePattern = 1;
		      msAPI_AdjustBrightness(100);			
		      DCRBacklight=100;	  
                    FactoryExecuteKeyEvent(MIA_REDRAW_MENU);
		      break;			
               }



	#if Enable_Corss_LHCX_New
      		if (enMenuAction==MIA_GOTO_NEXT)
      		{
			if(g_u8MenuPageIndex==HotKeyGameClokMenu)
			{
				if(g_u8MenuItemIndex==HotKeyGameClok_30MItem)
					GameModeTime_M=30;
				else if(g_u8MenuItemIndex==HotKeyGameClok_40MItem)
					GameModeTime_M=40;
				else if(g_u8MenuItemIndex==HotKeyGameClok_50MItem)
					GameModeTime_M=50;
				else if(g_u8MenuItemIndex==HotKeyGameClok_60MItem)
					GameModeTime_M=60;
				else if(g_u8MenuItemIndex==HotKeyGameClok_90MItem)
					GameModeTime_M=90;
				GameModeTime_S=0;
			}
			else if (g_u8MenuPageIndex==HotKeyGameClokPositionMenu)
			{
				if(g_u8MenuItemIndex==HotKeyGameClokPosition_UpItem)
					GameModeTime_Position=HotKeyGameClokPosition_UpItem;
				else if(g_u8MenuItemIndex==HotKeyGameClokPosition_CenterItem)
					GameModeTime_Position=HotKeyGameClokPosition_CenterItem;
				else if(g_u8MenuItemIndex==HotKeyGameClokPosition_DownItem)
					GameModeTime_Position=HotKeyGameClokPosition_DownItem;
			}
		}
		if(g_u8MenuPageIndex==HotKeyGameClokShowMenu)
		{
			GameModeTime_Position=HotKeyGameClokPosition_Close;
			Clr_HotGameTimeShowFlag();
		}
	#endif
		  
	     u8TempValue = g_u8MenuPageIndex;
            g_u8PrevMenuPageIndex=u8TempValue;
            g_u8PrevMenuItemIndex=g_u8MenuItemIndex;
     #if LED_HOT_Menu_Func
	  if((g_u8MenuPageIndex==MENU_MAIN)&&(enMenuAction==MIA_GOTO_NEXT)&&(NEXT_MENU_PAGE==MENU_SYSTEM)&&(USER_PREF_LEDMODE_INDEX==LedModeIndex_User))
            g_u8MenuPageIndex=MENU_SYSTEMLLEDUSER;
	  else
      #endif	  
            g_u8MenuPageIndex=(enMenuAction==MIA_GOTO_NEXT)?(NEXT_MENU_PAGE):(PREV_MENU_PAGE);
            g_u8MenuItemIndex = GetMenuItemIndex(u8TempValue);
		   
                if(CURRENT_MENU.pstMenuItems[g_u8MenuItemIndex].u16Flags&MIB_IR_SELECT)
                {
                   Set_Left_EnterExit_SwitchFlag();
		     Set_Right_EnterExit_SwitchFlag();	 		   
                   //printMsg("Set_LR_UpDown_SwitchFlag");
                }
               else
               {
                   Clr_Left_EnterExit_SwitchFlag();
		     Clr_Right_EnterExit_SwitchFlag();	 		   
               }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        case MIA_REDRAW_MENU:

            {
                MENU_printData("g_u8MenuPageIndex %d", g_u8MenuPageIndex);

		  Clr_LR_UpDown_SwitchFlag();
		  	

             #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                       if(InputStatusMultiWin)
                        {
                        g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN2] = 0;
                        g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN3] = 0;
                        g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN4] = 0;
                        
                            g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1] = 1;
                      if(USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_2_PIP)
                            g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN2] = 1;
                        if(USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_3)
                            g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN3] = 1;
                        if(USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_4)	
                           g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN4] = 1;
                        MultiOSDWin();
			}
			else
		#endif		
                       DrawOsdMenu(TRUE);

                ResetOsdTimer();

                if (CURRENT_MENU.ExecFunction)
                {
                    CURRENT_MENU.ExecFunction();
		      printData("g_u8PowerDownCounter==%d",g_u8PowerDownCounter);
                }
            }
            if (!(CURRENT_MENU.u8Flags & MPB_STAY))
            {
                IsProcessEvent = TRUE;
                enMenuAction = MIA_GOTO_PREV;
            }
			
            if ((g_u8SaveUserData!=SAVE_NONE)&&(g_u8MenuPageIndex==MENU_ROOT))
                SaveUserPrefSetting(u8TempValue);
			
            g_u8KeypadButton = BTN_REPEAT;
#if 1 //D_O_RESET_RETURN
            if (g_u8MenuPageIndex==MENU_ROOT)
            {
            #if 1 //Steven110622, Disable Shadow and SubWin1 gradient function
                Osd_SetWindowEnable(MAIN_GD_WIN,FALSE);
              #if 0//CHIP_ID<CHIP_TSUMV
                Osd_Shadow(FALSE, 0x00, 0x00, 0x00);
              #endif
            #endif
                g_u8MenuTempValue=0;    //Cleared for MENU_HOT_AUTO
            }
#endif
            break;

        case MIA_EXEC_AND_GOTO_NEXT:
        case MIA_EXEC_AND_GOTO_PREV:
        case MIA_EXEC_GOTO: //TRUE=GOTO_NEXT else GOTO_PREV
        case MIA_EXEC_FUNC:
        case MIA_EXEC_GOTO_NEXT_RETURN: //TRUE=GOTO_NEXT else RETURN

            if (CURRENT_MENU_ITEM_FUNC.ExecFunction)
            {
                u8Rlt = CURRENT_MENU_ITEM_FUNC.ExecFunction();

			
                if ((!u8Rlt)&&(CURRENT_MENU_ITEM_FUNC.ExecFunction==SetInputSelect))
                {
                if (((!BURNIN_MODE_FLAG)&&(FreeRunModeFlag(MapPort2DetScIndex(USER_PREF_INPUT_TYPE))&&SyncLossState(USER_PREF_INPUT_TYPE))
	    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU	  	
		  	&&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
	    #endif	  	
		  )
	    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU	  	
               ||((USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)&&Is_MultiWin_SyncLoss())
          #endif     
		)
                {
                    g_u8MenuPageIndex=MENU_STANDBY;
		      g_u8MenuItemIndex=0;			
                    enMenuAction=MIA_REDRAW_MENU;               
                    break;
                }
               }

                if ((u8Rlt)&&(CURRENT_MENU_ITEM_FUNC.ExecFunction==ResetAllSetting))
                {
                      LoadMainTextFont();
                }
			
                if (enMenuAction == MIA_EXEC_GOTO_NEXT_RETURN)
                {
                    if (u8Rlt)
                    {
                        enMenuAction = MIA_GOTO_NEXT;
                        //if (!(CURRENT_MENU_ITEM_FUNC.ExecFunction==SetColorTempValue && USER_PREF_COLOR_TEMP==PCT_USER)) //Jison 080716
                        //    g_u8SaveUserData=SAVE_NONE; //Jison: Options item needn't to save while goto next menu
                    }
                    else
                        enMenuAction = MIA_RETURN_SELECT_ITEM;
                    g_u8KeypadButton = BTN_REPEAT;
                    IsProcessEvent = TRUE;
                    break;
                }
                else if (enMenuAction == MIA_EXEC_GOTO)
                {
                    if (u8Rlt)
                        enMenuAction = MIA_GOTO_NEXT;
                    else
                        enMenuAction = MIA_GOTO_PREV;
                    g_u8KeypadButton = BTN_REPEAT;
                    IsProcessEvent = TRUE;
                    break;
                }
            }

            if (enMenuAction == MIA_EXEC_AND_GOTO_NEXT)
            {
                enMenuAction = MIA_GOTO_NEXT;
                IsProcessEvent = TRUE;
                g_u8KeypadButton = BTN_REPEAT;
            }
            else if (enMenuAction == MIA_EXEC_AND_GOTO_PREV)
            {
                enMenuAction = MIA_GOTO_PREV;
                IsProcessEvent = TRUE;
                g_u8KeypadButton = BTN_REPEAT;
            }
            break;

#if ENABLE_VGA_INPUT
        case MIA_AUTO:
            {
                enMenuAction = MIA_REDRAW_MENU;

                if (OSD_LOCK_FLAG)
                {
                    g_u8MenuPageIndex=MENU_HOT_MENU_LOCK;
                }
                else
                {

                 if((CURRENT_INPUT_IS_VGA())
		#if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&PXP_VGA_Auto_Enable) 	
		      ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))
		      ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))))
		      ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))||((USER_PREF_INPUT_TYPE_SUBWIN3==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN3)))))
              #endif
		      )
                    g_u8MenuPageIndex = MENU_HOT_AUTO;
                  else	
                    g_u8MenuPageIndex = MENU_ROOT;
                }
                g_u8KeypadButton=BTN_REPEAT;
                g_u8MenuItemIndex = 0;
                IsProcessEvent = TRUE;
            }
            break;
#endif

        case MIA_EXIT:

                if (OSD_LOCK_FLAG)
                {
                    g_u8MenuPageIndex=MENU_HOT_MENU_LOCK;
                }
                else
                {
                    g_u8MenuPageIndex = MENU_ROOT;
                }
                g_u8KeypadButton=BTN_REPEAT;
                g_u8MenuItemIndex = 0;
                enMenuAction = MIA_REDRAW_MENU;
                IsProcessEvent = TRUE;

            break;

			
        case MIA_SHOW_POWER_SAVE:
            mStar_SetupFreeRunMode();
            Power_TurnOffAmberLed();
            Power_TurnOnGreenLed();
            Power_TurnOffAmberLed();    // Jeff add it in 2006年3月29日下午 05:31:38
            Clr_PowerSavingFlag();
            Power_TurnOnPanel();
            g_u8MenuPageIndex = MENU_STANDBY;
            enMenuAction = MIA_REDRAW_MENU;
            g_u8MenuItemIndex = 0;
            IsProcessEvent = TRUE;
            break;

#if ENABLE_MULTI_INPUT
        case MIA_INPUT_TYPE:    /// Src Input Key Pressed to show MENU_INPUT
            g_u8MenuPageIndex = MENU_INPUT;
            if (SyncLossState(SrcInputPortM/*//tmp*/))
            {
                g_u8PowerDownCounter = 0;
                PowerOnSystem();
                g_stMenuFlag.bOsdSwitchInputPort=1;
                Clr_InputTimingChangeFlag(SrcInputPortM/*//tmp*/);
                Power_TurnOnPanel();
            }
            g_stMenuFlag.bPressSrcInputKey=1;
	#if Enable_AutoInput_Menu		
            g_u8MenuTempValue=USER_PREF_INPUT_PRIORITY; ///Init g_u8MenuTempValue
       #else
            g_u8MenuTempValue=USER_PREF_INPUT_TYPE; ///Init g_u8MenuTempValue
       #endif
            enMenuAction = MIA_REDRAW_MENU;
            g_u8MenuItemIndex = 0;
            IsProcessEvent = TRUE;
            break;

        case MIA_SWITCH_SOURCE: ///Switch the string on MENU_INPUT,Switch source after 2 sec idle(in Menu_OsdHandler()).

            if (!g_stMenuFlag.bPressSrcInputKey)    ///Disable the switch src function while showing InputMenu after power on.
                break;
            if (!FreeRunModeFlag(SCL0_MAIN/*//tmp*/))
                drvmStar_SetupFreeRunMode(SCL0_MAIN/*//tmp*/);
            g_u8MenuTempValue=(g_u8MenuTempValue+1)%INPUT_PRIORITY_NUMS;
            enMenuAction=MIA_REDRAW_MENU;
            g_u8MenuItemIndex=0;
            IsProcessEvent=TRUE;
            break;
#endif

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	case MIA_HotMulti_PXP:
	case MIA_HotMulti_PIP:
       case MIA_HotMulti_PBP_OFF:
       case MIA_HotMulti_PBP_2WIN:
       case MIA_HotMulti_PBP_3WIN:
       case MIA_HotMulti_PBP_4WIN:
		
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
			}
			else
		#endif		
			if(!FACTORY_MODE_FLAG)
			{
			  BYTE PXPChange=0;
		    #if DP_PBP_RELOAD_EDID	
			BYTE HPD_Status=0;
		    #endif	
			#if HDMI_PBP_RELOAD_EDID
			BYTE HDMI_HPD_Status=0;
			#endif
			#if DVI_PBP_RELOAD_EDID
			BYTE DVI_HPD_Status=0;
			#endif

			
				//SET_OSD_FUNCTION_ACTION_FLAG();
			  	//hw_ClrBlacklit();
			  if(enMenuAction==MIA_HotMulti_PIP)	
			  {
			  #if (!HKC_PCToolCommand)	
			  	if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
		  		{
                               USER_PREF_MULTI_WIN_MODE=eMULTIWIN_OFF;	
			  	}
				else
			  #endif		
                               USER_PREF_MULTI_WIN_MODE=eMULTIWIN_2_PIP;	
				PXPChange=1;
			  }
			  else if(enMenuAction==MIA_HotMulti_PBP_OFF&&USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
			  {
                            USER_PREF_MULTI_WIN_MODE=eMULTIWIN_OFF;	
				PXPChange=1;
			  }
			  else if(enMenuAction==MIA_HotMulti_PBP_2WIN&&USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_2_POP1)
			  {
                               USER_PREF_MULTI_WIN_MODE=eMULTIWIN_2_POP1;	
				PXPChange=1;
			  }
			  else if(enMenuAction==MIA_HotMulti_PBP_3WIN&&USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_3)
			  {
                               USER_PREF_MULTI_WIN_MODE=eMULTIWIN_3;	
				PXPChange=1;
			  }
			  else if(enMenuAction==MIA_HotMulti_PBP_4WIN&&USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_4)
			  {
                               USER_PREF_MULTI_WIN_MODE=eMULTIWIN_4;	
				PXPChange=1;
			  }
			  else if(enMenuAction==MIA_HotMulti_PXP)
			  {
                             USER_PREF_MULTI_WIN_MODE=(USER_PREF_MULTI_WIN_MODE+1)%eMULTIWIN_MAX;	
				PXPChange=1;
			  }
			       printData("USER_PREF_MULTI_WIN_MODE 111==%d",USER_PREF_MULTI_WIN_MODE);

			if(!PXPChange)
				return FALSE;
				//Clr_AdjustPipPositionFlag();


                 	Set_HDMI1HPDFlag();   
                 	Set_HDMI2HPDFlag();   
                 	Set_HDMI3HPDFlag();   

				WAIT_V_OUTPUT_BLANKING();
                            mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
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
                                         //mhal_DP_HPDControl(4, FALSE);
                   #if Free_Sync_Enable                      
                        if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
                             Clr_FreeSyncFlag();
		     #endif								 
					      msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[INPUT_DISPLAYPORT].eIPMux, FALSE);	 		 
					      HPD_Status=1;			 

                                         EDID_ReLoad_Func(0);
                                         msAPI_combo_LoadEDID_Func();
                           		      printMsg("DP_PBP_RELOAD_EDID 555555");	  
                                         //ForceDelay1ms(600); // delay 1 msec				
                         }
 	              #if Free_Sync_Enable
                        else if((FreeSyncFlag)&&(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF))
                         {
                             Clr_FreeSyncFlag();
                         	 SetFreeSyncOnOffFunction(DISPLAY_WIN1,((FreeSyncFlag)?1:0),800);
                         }  
			 #endif			
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
                           		      printMsg("HDMI_PBP_RELOAD_EDID DVI");	  
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
                  
					   
				mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);   	   
                  #if Enable_Menu_Panel_Rotate_180
                         if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
                         {
                            USER_PREF_ROTATE_INDEX=Rotate_Normal;
                         }
		    #endif		
			
			  
				SetPIPPositionParameter();
							
                            APPWindow_Configure((eMULTIWIN_Type)USER_PREF_MULTI_WIN_MODE, (ePIPSIZE_Type)USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2));
                            
			     if(DCR_ENABLE_FLAG)
				{
                               CLR_DCR_ENABLE_FLAG();
				   SetDCRFunction();	 		   
				}
                           	  
				 
                            g_u8SaveUserData|=SAVE_MONITOR_SETTING;


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

	                     Set_PIP_Border();
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

			}
			g_u8KeypadButton=BTN_REPEAT;

			break;			

			case MIA_HotMultiView_PIP_Swap:
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				Menu_Hide();
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
			else
		#endif		
			if(!FACTORY_MODE_FLAG)
			{
                 	Set_HDMI1HPDFlag();   
                 	Set_HDMI2HPDFlag();   
                 	Set_HDMI3HPDFlag();   
						
			  if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)	
			  {  
			#if 0//AudioFunc  
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
				//SET_OSD_FUNCTION_ACTION_FLAG();
				WAIT_V_OUTPUT_BLANKING();
                            mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
				Delay1ms(100);			
			       Osd_Set_PiP_PbP_Swap();
			  }
	  
			}
			g_u8KeypadButton=BTN_REPEAT;
			break;		


 #if ((defined(LeXingEnMa_Project))&&ENABLE_AUDIO_SETTINGS_MENU)
			case MIA_HotMulti_PIP_AudioSwap:
		 #if OSD_LOCK_Enable	
			if(OSD_LOCK_FLAG)
			{
				Menu_Hide();
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
			else
		#endif		
			if(!FACTORY_MODE_FLAG)
			{
						
			  if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)	
			  {  
              #if Audio_Source_Enable
			    BYTE USER_PREF_AUDIOSOURCE_Main,USER_PREF_AUDIOSOURCE_Sub;
			
			    if(PIP_MAIN_AUDIO_FLAG)
                           CLR_PIP_MAIN_AUDIO_FLAG();
			    else
                           SET_PIP_MAIN_AUDIO_FLAG();
                        
                          USER_PREF_AUDIOSOURCE_Main=GetCurAudioSourceIndex(USER_PREF_INPUT_TYPE);
                          USER_PREF_AUDIOSOURCE_Sub=GetCurAudioSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN1);
						  
			  if(PIP_MAIN_AUDIO_FLAG)			
			  {
			     gAudioSourceSelectableIndex=USER_PREF_AUDIOSOURCE_Main;	
			    USER_PREF_AUDIOSOURCE_Main=USER_PREF_INPUT_TYPE;		 
			  }
			  else
			  {
			     gAudioSourceSelectableIndex=USER_PREF_AUDIOSOURCE_Sub;		
			     USER_PREF_AUDIOSOURCE_Main=USER_PREF_INPUT_TYPE_SUBWIN1;		 
			  }

                     
			   //if(PIP_MAIN_AUDIO_FLAG)	
                           msAPI_AudioSourceSel(USER_PREF_AUDIOSOURCE_Main);
			   //else
                        //   SetAudioSource(USER_PREF_INPUT_TYPE_SUBWIN1);
			   
                           SetAudioMuteOnOff();
           		
                           msAPI_AdjustVolume(USER_PREF_VOLUME);
					 
			      Delay1ms(100);

			      SaveMonitorSetting();		  
			        
		#else	  
			    BYTE USER_PREF_AUDIOSOURCE_Main;
			
			    if(PIP_MAIN_AUDIO_FLAG)
                           CLR_PIP_MAIN_AUDIO_FLAG();
			    else
                           SET_PIP_MAIN_AUDIO_FLAG();
                        
                          //USER_PREF_AUDIOSOURCE_Main=GetCurAudioSourceIndex(USER_PREF_INPUT_TYPE);
                          //USER_PREF_AUDIOSOURCE_Sub=GetCurAudioSourceIndex(USER_PREF_INPUT_TYPE_SUBWIN1);
                          
			  if(PIP_MAIN_AUDIO_FLAG)			  
			  {
			     USER_PREF_AUDIOSOURCE_Main=USER_PREF_INPUT_TYPE;	
			     	 
			  }
			  else
			  {
			     USER_PREF_AUDIOSOURCE_Main=USER_PREF_INPUT_TYPE_SUBWIN1;		
			  }

			/*		 
			   if(PIP_MAIN_AUDIO_FLAG)	
                           SetAudioSource(USER_PREF_INPUT_TYPE);
			   else
                           SetAudioSource(USER_PREF_INPUT_TYPE_SUBWIN1);
                      */
			      msAPI_AudioSourceSel(USER_PREF_AUDIOSOURCE_Main);
					 
                           SetAudioMuteOnOff();
           		
                           msAPI_AdjustVolume(USER_PREF_VOLUME);
					 
			      Delay1ms(100);
		#endif		  
			  }
	  
			}
			g_u8KeypadButton=BTN_REPEAT;
			break;		
#endif
			
			case MIA_HotMulti_PIP_Size:
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				Menu_Hide();
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
			else
		#endif		
			if(!FACTORY_MODE_FLAG)
			{
                 	Set_HDMI1HPDFlag();   
                 	Set_HDMI2HPDFlag();   
                 	Set_HDMI3HPDFlag();   

		       if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
			{
  		  #if (!HKC_PCToolCommand)	
                       OrigAdjPipHpos=(100-GetPIPHPositionValue());
                       OrigAdjPipVpos=GetPIPVPositionValue();

                      USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) = (USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2))%ePIPSIZE_MAX;		

                       if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)==ePIPSIZE_SMALL)
                       {
                          USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) = ePIPSIZE_MIDDLE;		
                       }
                       else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)==ePIPSIZE_MIDDLE)
                       {	
                          USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) = ePIPSIZE_LARGE;			
                       }
                       else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)==ePIPSIZE_LARGE)
                       {		
                          USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) = ePIPSIZE_SMALL; 
                       }
                     //printData("USER_PREF_PIP_POSITION 1111==%d",USER_PREF_PIP_POSITION);
     		#endif			   


                        //Set PIP size function here...
                        if (OrigAdjPipHpos!=0)
                            USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2) = (DWORD)((PANEL_WIDTH-GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)))*OrigAdjPipHpos)/100;
                        if (OrigAdjPipVpos!=0)
                            USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2) = (DWORD)((PANEL_HEIGHT-GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)))*OrigAdjPipVpos)/100;

                   //if(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1))
                   //   mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
              
                        WAIT_V_OUTPUT_BLANKING_START();
                        APPWindow_SetSize(DISPLAY_WIN2, USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2), USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2), GetPipWindowWidth(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)), GetPipWindowHeight(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2)));

	              g_u8SaveUserData|=SAVE_MONITOR_SETTING;
			}
			}
			g_u8KeypadButton=BTN_REPEAT;
			break;		
			
			case MIA_HotMulti_PIP_Position:
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				Menu_Hide();
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
			else
		#endif		
			if(!FACTORY_MODE_FLAG)
			{

		       if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
			{
                 	Set_HDMI1HPDFlag();   
                 	Set_HDMI2HPDFlag();   
                 	Set_HDMI3HPDFlag();   

		  #if (!HKC_PCToolCommand)	
                       USER_PREF_PIP_POSITION = (USER_PREF_PIP_POSITION)%PIPPosition_Max;		
                     //printData("USER_PREF_PIP_POSITION 0000==%d",USER_PREF_PIP_POSITION);
                       if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                       {
                          USER_PREF_PIP_POSITION = PIPPosition_Top_Right;		
                       }
                       else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                       {	
                          USER_PREF_PIP_POSITION = PIPPosition_Bottom_Right;			
                       }
                       else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
                       {		
                          USER_PREF_PIP_POSITION = PIPPosition_Bottom_Left; 
                       }
                       else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                       {		
                          USER_PREF_PIP_POSITION = PIPPosition_Top_Left; 
                       }
		   #endif			   
                     //printData("USER_PREF_PIP_POSITION 1111==%d",USER_PREF_PIP_POSITION);

                     //mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);   

                     SetPIPPositionParameter();
					 
                     WAIT_V_OUTPUT_BLANKING_START();
                     APPWindow_SetPosition(DISPLAY_WIN2, USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2), USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2));
                     
                     Set_PIP_Border();
	              g_u8SaveUserData|=SAVE_MONITOR_SETTING;
			}
			}
			g_u8KeypadButton=BTN_REPEAT;
			break;		
 #endif


	case MIA_HotInput_DP:
	case MIA_HotInput_HDMI1:
	case MIA_HotInput_HDMI2:
	case MIA_HotInput_HDMI3:
                    Menu_Hide();
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				Menu_Hide();
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
			else
		#endif		
			{
			 BYTE ChangePort=0;
		#if ENABLE_DP_INPUT	 
			   if(enMenuAction==MIA_HotInput_DP&&USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT)		
			   {
                           USER_PREF_INPUT_TYPE=INPUT_DISPLAYPORT;
			      ChangePort=1;			   
			   }
		#else
		         if(0);
		#endif	   
		#if ENABLE_HDMI_INPUT	   
			   else if(enMenuAction==MIA_HotInput_HDMI1&&USER_PREF_INPUT_TYPE!=INPUT_HDMI)
			   {
                           USER_PREF_INPUT_TYPE=INPUT_HDMI;
			      ChangePort=1;			   
			   }
			#if ENABLE_HDMI2ND_INPUT   
			   else if(enMenuAction==MIA_HotInput_HDMI2&&USER_PREF_INPUT_TYPE!=INPUT_HDMI2ND)
			   {
                           USER_PREF_INPUT_TYPE=INPUT_HDMI2ND;
			      ChangePort=1;			   
			   }
			#endif
			#if ENABLE_HDMI3ND_INPUT
			   else if(enMenuAction==MIA_HotInput_HDMI3&&USER_PREF_INPUT_TYPE!=INPUT_HDMI3ND)
			   {
                           USER_PREF_INPUT_TYPE=INPUT_HDMI3ND;
			      ChangePort=1;			   
			   }
			#endif   
		#endif	   
			   if(ChangePort)
			   {
                           Set_HDMI1HPDFlag();   
                           Set_HDMI2HPDFlag();   
                           Set_HDMI3HPDFlag();   
                           SwitchPort(DISPLAY_WIN1, USER_PREF_INPUT_TYPE);		
			   }
                            g_u8SaveUserData|=SAVE_MONITOR_SETTING; //Jison
			}
			g_u8KeypadButton=BTN_REPEAT;
	break;



#if ENABLE_WIDE_MODE
            		case MIA_HotPictureFormat:

                 #if OSD_LOCK_Enable
			if(OSD_LOCK_FLAG)
			{
				Menu_Hide();
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
                    else
		  #endif				

				if(g_u8MenuPageIndex != MENU_HOTPICTUREFORMAT)
				{
					g_u8MenuPageIndex = MENU_HOTPICTUREFORMAT;
					g_u8MenuItemIndex = 0;
					enMenuAction = MIA_REDRAW_MENU;
					IsProcessEvent = TRUE;	 
				}

			break;
#endif

            		case MIA_HotPictureMode:

                 #if OSD_LOCK_Enable
			if(OSD_LOCK_FLAG)
			{
				Menu_Hide();
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
                    else
		  #endif			
		  	{

                            CURRENT_PICTURE_MODE =(CURRENT_PICTURE_MODE +1)%CT_NUMS;
                            
                            printData("CURRENT_PICTURE_MODE==%d",CURRENT_PICTURE_MODE);
                            
                            Menu_AdjustColorSetting();		  
                            g_u8SaveUserData|=SAVE_MONITOR_SETTING;
                            
                            g_u8MenuPageIndex = MENU_HOTPICTUREMODE;
                            g_u8MenuItemIndex = 0;
                            enMenuAction = MIA_REDRAW_MENU;
                            IsProcessEvent = TRUE;
                            g_u8KeypadButton = BTN_REPEAT;
                            
        	        }	
		 break;
			
         #if 0//SleepMode_Enable
            		case MIA_HotSleepMode:
				Menu_Hide();

                 #if OSD_LOCK_Enable
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
                    else
		  #endif				
                    	{


			USER_PREF_SLEEPMODE = (USER_PREF_SLEEPMODE+1)%SleepMode_Max;

                   if(USER_PREF_SLEEPMODE==SleepMode_10Min)
                       SleepModeCounter=10*60;
                   else if(USER_PREF_SLEEPMODE==SleepMode_20Min)
                       SleepModeCounter=20*60;
                   else if(USER_PREF_SLEEPMODE==SleepMode_30Min)
                       SleepModeCounter=30*60;
                   else if(USER_PREF_SLEEPMODE==SleepMode_40Min)
                       SleepModeCounter=40*60;
                   else if(USER_PREF_SLEEPMODE==SleepMode_50Min)
                       SleepModeCounter=50*60;
                   else if(USER_PREF_SLEEPMODE==SleepMode_60Min)
                       SleepModeCounter=60*60;
                   else if(USER_PREF_SLEEPMODE==SleepMode_120Min)
                       SleepModeCounter=120*60;
                   else
                       SleepModeCounter=0;

                       g_u8SaveUserData|=SAVE_MONITOR_SETTING;
   			  g_u8KeypadButton = BTN_REPEAT;
                       g_u8MenuPageIndex = MENU_SLEEPMODE;
                       g_u8MenuItemIndex = 0;
                       enMenuAction = MIA_REDRAW_MENU;
                       IsProcessEvent = TRUE;	
			  
                    	}
			break;
        #endif

	case MIA_HotBrightness:
                    Menu_Hide();
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
			}
			else
		#endif		
			{
				g_u8MenuPageIndex = MENU_HOTBRIGHTNESS;
				g_u8MenuItemIndex = 0;
			}
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
	break;

	case MIA_HotContrast:
                    Menu_Hide();
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
			}
			else
		#endif		
			{
				g_u8MenuPageIndex = MENU_HOTCONTRAST;
				g_u8MenuItemIndex = 0;
			}
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
	break;

#if Enable_HotAdjLowbluray
	case MIA_Lowbluray:
                    Menu_Hide();
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
			}
			else
		#endif		
			{
				g_u8MenuPageIndex = MENU_HOTLOWBLURAY;
				g_u8MenuItemIndex = 0;
			}
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
	break;
#endif

#if 0//LED_HOT_Menu_Func
            		case MIA_HotLedMode:
				Menu_Hide();

                 #if OSD_LOCK_Enable
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
                    else
		  #endif				
                    	{
                            //UserprefLEDColorful=USER_PREF_LEDColorful_MODE;
                            //UserprefLedStrength=USER_PREF_LedStrength;
                            //UserprefLedModeType=USER_PREF_LedModeType;    	
				g_u8MenuPageIndex = MENU_HOTLEDMODE;
				g_u8MenuItemIndex = 0;
			}
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	

			break;
#endif


 #if ENABLE_AUDIO_SETTINGS_MENU
	case MIA_HotVolume:
                    Menu_Hide();
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
			}
			else
		#endif		
			{
				g_u8MenuPageIndex = MENU_HOTVOLUME;
				g_u8MenuItemIndex = 0;
			}
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
	break;
 #endif
	case MIA_HotInput:
				
             #ifdef TaiWan_Project		
			if(DISPLAY_UNSUPPORT_MODE_FLAG)
                       break;
	     #endif		
				Menu_Hide();
				
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	

			}
			else
		#endif		
			{
                    #if 1
                            g_u8MenuPageIndex = MENU_HOTINPUT;
                            g_u8MenuItemIndex = USER_PREF_INPUT_TYPE;
                            enMenuAction = MIA_REDRAW_MENU;
                            IsProcessEvent = TRUE;
                            g_u8KeypadButton = BTN_REPEAT;

			#else
                         #if Enable_AutoInput_Menu
                            USER_PREF_INPUT_PRIORITY =(USER_PREF_INPUT_PRIORITY +1)%INPUT_PRIORITY_NUMS;
                            
                            SwitchPort(USER_PREF_WIN_SEL, Get_SourceSwitchType());	
                         #else
                            USER_PREF_INPUT_TYPE =(USER_PREF_INPUT_TYPE +1)%Input_Nums;
                            
                            SwitchPort(USER_PREF_WIN_SEL, USER_PREF_INPUT_TYPE);	
			    #endif			 
                            g_u8SaveUserData|=SAVE_MONITOR_SETTING;
                            g_u8SwitchPortCounter = 0; // sync locked, counter reset to 0	
                            
                            g_u8MenuPageIndex = MENU_ROOT;
                            g_u8MenuItemIndex = 0;
                            enMenuAction = MIA_REDRAW_MENU;
                            IsProcessEvent = TRUE;
                            g_u8KeypadButton = BTN_REPEAT;
                     #endif
                       break;
                       
                       }
	break;				

#if Enable_Corss_LHCX_New
        case MIA_HOTCROSS:
                  Menu_Hide();
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
                    else
		#endif				
                    	{              

                       g_u8MenuPageIndex = HotKeyGameModeMenu;
                       g_u8MenuItemIndex = 0;
  			  enMenuAction = MIA_REDRAW_MENU;
  		         IsProcessEvent = TRUE;	

   			  g_u8KeypadButton = BTN_REPEAT;
			 }
		break;
/*
		 case MIA_HotGameClokShow:	

			if (FreeRunModeFlag(USER_PREF_INPUT_TYPE))
            			break;
			//	MenuPageIndex=HotKeyGameClokShowMenu;
			//	MenuItemIndex=0;
				
				if(HotGameTimeShowFlag)
					DrawHotKeyGameClokShow();
			break;
*/		
#else
      case MIA_HOTCROSS:
                    Menu_Hide();
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
                    else
		#endif				
                    	{              

			if(g_u8MenuPageIndex == MENU_HOTCROSS)
			{
                     #if New_Cross_Enable
	                   UserPrefCrossType++;
			     Osd_LoadHOTCROSSFont();		    
			    if	(UserPrefCrossType==Cross_OFF)
			    {
			     UserPrefCrossType=Cross_Large_Y;
	                   g_u8MenuPageIndex=MENU_ROOT;
			     g_u8KeypadButton = BTN_REPEAT;
	                   enMenuAction=MIA_REDRAW_MENU;
	                   g_u8MenuItemIndex=0;
	                   IsProcessEvent=TRUE;
			    }
			    else
			    {
		            Osd_DrawCrossFont();
			     g_u16OsdCounter = 0;	
			     g_stMenuFlag.bOsdTimeout=0;	 
			     Menu_Show();	 
			    }
			     g_u8KeypadButton=BTN_REPEAT;
			     break;
			#elif Enable_Corss_FY
	                   UserPrefCrossType++;
			         
			    if	(UserPrefCrossType==Cross_Color_OFF)
			    {
			     UserPrefCrossType=Cross_Color_Y;
	                   g_u8MenuPageIndex=MENU_ROOT;
			     g_u8KeypadButton = BTN_REPEAT;
	                   enMenuAction=MIA_REDRAW_MENU;
	                   g_u8MenuItemIndex=0;
	                   IsProcessEvent=TRUE;
			     Clr_HotCrossMenuFlag();		   
			     break;	 
			    }
			    else
			    {
			     Set_HotCrossMenuFlag();
		            Osd_DrawCrossFont();
			     g_u16OsdCounter = 0;	
			     g_stMenuFlag.bOsdTimeout=0;	 
			     Menu_Show();	 
			    }
			     g_u8KeypadButton=BTN_REPEAT;
				 
			     break;	
			#elif Enable_Corss_LHCX

			  if(DDCCIActiveFlag)
			  {
                          Clr_DDCCIActiveFlag();
			  }
			  else
	                   UserPrefCrossType=(UserPrefCrossType+1)%Cross_Max;

			     Osd_LoadHOTCROSSFont();		
			    /*	 
			    if	(UserPrefCrossType==Cross_OFF)
			    {
			     UserPrefCrossType=Cross_Mode1_G;
	                   g_u8MenuPageIndex=MENU_ROOT;
			     g_u8KeypadButton = BTN_REPEAT;
	                   enMenuAction=MIA_REDRAW_MENU;
	                   g_u8MenuItemIndex=0;
	                   IsProcessEvent=TRUE;
			    }
			    else
			     */		
			    {
		            Osd_DrawCrossFont();
			     g_u16OsdCounter = 0;	
			     g_stMenuFlag.bOsdTimeout=0;	 
			     Menu_Show();	 
			    }
			     g_u8KeypadButton=BTN_REPEAT;
			     break;
			#elif Enable_Corss_RunFeng
                        UserPrefCrossType=(UserPrefCrossType+1)%Cross_Mode_MaxNumber;
			
			    {
	                   g_u8MenuPageIndex=MENU_ROOT;
			     g_u8KeypadButton = BTN_REPEAT;
	                   enMenuAction=MIA_REDRAW_MENU;
	                   g_u8MenuItemIndex=0;
	                   IsProcessEvent=TRUE;
			    }


			     g_u8KeypadButton=BTN_REPEAT;
			     break;			

			#else
  			  if(!MenuCross_RedFlag)
  			  {
                          Osd_DrawCrossFont(1);	
                          Menu_Show();
  			     Set_MenuCross_RedFlag();
  			  }
			  else
			  {
 			     Clr_MenuCross_RedFlag();
                          g_u8MenuPageIndex = MENU_ROOT;
                          g_u8MenuItemIndex = 0;
                          enMenuAction = MIA_REDRAW_MENU;
                          IsProcessEvent = TRUE;				  
			   }
                     #endif
			  
			}
			else
			{

                    #if 0//Enable_Corss_RunFeng
                        Osd_LoadHOTCROSSFont();		
                        Osd_DrawCrossFont();
                        g_u16OsdCounter = 0;	
                        Menu_Show();	 
		      #else			
			  Clr_MenuCross_RedFlag();
			  Set_HotCrossMenuFlag();
                       g_u8MenuPageIndex = MENU_HOTCROSS;
                       g_u8MenuItemIndex = 0;
  			  enMenuAction = MIA_REDRAW_MENU;
  		         IsProcessEvent = TRUE;	
		      #endif		 
			}
   			  g_u8KeypadButton = BTN_REPEAT;
			  g_u16OsdCounter = 0;	
			  g_stMenuFlag.bOsdTimeout=0;	 
                    	}
	break;
	
#endif

      case MIA_HotDCR:
                    Menu_Hide();
		#if OSD_LOCK_Enable		
			if(OSD_LOCK_FLAG)
			{
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
                    else
		#endif				
                    	{              
                    	
		   //if(g_u8MenuPageIndex = HotMuteMsgMenu)		
		   {
                      if(AUDIO_MUTE_FLAG)
                      {
                         CLR_AUDIO_MUTE_FLAG();
                      }
                      else
                      {
                         SET_AUDIO_MUTE_FLAG();
                      }
		   }

                       g_u8MenuPageIndex = MENU_HOTDCR;
                       g_u8MenuItemIndex = 0;
  			  enMenuAction = MIA_REDRAW_MENU;
  		         IsProcessEvent = TRUE;	
                       g_u8SaveUserData|=SAVE_MONITOR_SETTING;
   			  g_u8KeypadButton = BTN_REPEAT;
                    	}
	break;
	#if HKC_RGB_Range_Func
            	  case MIA_HotRGBRange:

               if(
		#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	   	
		 (USER_PREF_MULTI_WIN_MODE==eMULTIWIN_OFF)&&
		#endif 
	        (CURRENT_INPUT_IS_HDMI()||CURRENT_INPUT_IS_DISPLAYPORT())&&(g_u8MenuPageIndex==MENU_HOTRGBRANGE||g_u8MenuPageIndex==MENU_ROOT
	     #if (defined(LianHeChuangXin_Project))		   	
	         ||g_u8MenuPageIndex==MENU_KEY_GUIDE
            #endif
		  ))
		  ;
		  else if(((g_u8MenuPageIndex>MENU_ROOT)&&(g_u8MenuPageIndex<MAX_MENU)))
		  {
   			  g_u8KeypadButton = BTN_REPEAT;
                       g_u8MenuPageIndex = MENU_ROOT;
                       g_u8MenuItemIndex = 0;
                       enMenuAction = MIA_REDRAW_MENU;
                       IsProcessEvent = TRUE;	
			  break;		   
		  }

                 #if OSD_LOCK_Enable
			if(OSD_LOCK_FLAG)
			{
				Menu_Hide();
				g_u8MenuPageIndex = MENU_HOT_MENU_LOCK;
				g_u8MenuItemIndex = 0;
				enMenuAction = MIA_REDRAW_MENU;
			       IsProcessEvent = TRUE;	
			}
                    else
		  #endif				
                    	{

                     if(CURRENT_INPUT_COLOR_RANGE==OSD_INPUT_COLOR_RANGE_FULL)
			 	CURRENT_INPUT_COLOR_RANGE	=OSD_INPUT_COLOR_RANGE_LIMITED;
			else
			 	CURRENT_INPUT_COLOR_RANGE	=OSD_INPUT_COLOR_RANGE_FULL;
					 
                       printData("CURRENT_INPUT_COLOR_RANGE==%d",CURRENT_INPUT_COLOR_RANGE);

			  WAIT_V_OUTPUT_BLANKING();		   
                       APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
					  
                       g_u8SaveUserData|=SAVE_MONITOR_SETTING;
   			  g_u8KeypadButton = BTN_REPEAT;

                       g_u8MenuPageIndex = MENU_HOTRGBRANGE;
                       g_u8MenuItemIndex = 0;
                       enMenuAction = MIA_REDRAW_MENU;
                       IsProcessEvent = TRUE;	
			  
                    	}
			break;
	#endif
	
        case MIA_POWER:

            MENU_printMsg("MIA_POWER");
            if (POWER_ON_FLAG)
            {//Power off
                g_u8MenuPageIndex = MENU_POWER_OFF;
                if (DoBurninModeFlag) //Must clear this flag while power off
                    Clr_DoBurninModeFlag();
            }
            else
            {//Power on
                g_u8MenuPageIndex = MENU_POWER_ON;
            #if ENABLE_MULTI_INPUT
		#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
		if(USER_PREF_MULTI_WIN_MODE > eMULTIWIN_OFF )
		#endif	
			g_stMenuFlag.bDisplayInputMsg[0]=1;	
            #endif
            }
            g_u8MenuItemIndex = 0;
            enMenuAction = MIA_REDRAW_MENU;
            IsProcessEvent = TRUE;
            break;



			
        default:
            return FALSE;
            break;
        }
    }

    return TRUE;
}


#if 0 //CHIP_ID<CHIP_TSUMV
void Osd_SetGradient(BYTE u8En, BYTE u8Win_Num, BYTE u8Grad_App, BYTE u8Ini_Color_En, BYTE u8RGB_CLR)
{
    if(u8En)
    {
        msWriteByteMask(OSD1_15, BIT3, BIT3); //BIT3=1
        msWriteByte(OSD1_15, msReadByte(OSD1_15)|u8Win_Num);

        if (u8Grad_App)
            msWriteByte(OSD1_16, 0x40);  //BIT6=1
        else
            msWriteByte(OSD1_16, 0x00);  //BIT6=0

        if (u8Ini_Color_En)
            msWriteByteMask(OSD1_16, BIT7, BIT7);  //BIT7=1
        else
            msWriteByteMask(OSD1_16,    0, BIT7);  //BIT7=0
    }
    else
        msWriteByteMask(OSD1_15,    0, BIT3);

// RGB starting gradient color
    msWriteByte(OSD1_16, msReadByte(OSD1_16)+((u8RGB_CLR<<4)&0x30));
}
#endif

#if 0 ////White130330 temp
void SetGradualColor(void)
{
    drvOSD_SetTGenCtrl(OSD_TG0, OSD_COLOR_FIX, CURSOR_TOP_R, CURSOR_TOP_G, CURSOR_TOP_B);
    drvOSD_SetTGenCtrl(OSD_TG1, OSD_COLOR_FIX, CURSOR_DOWN_R, CURSOR_DOWN_G, CURSOR_DOWN_B);
    drvOSDSetMask0(1 ,_ENABLE);
}
#endif

#if ENABLE_DEMURA_FUNCTION
//#define MEASURE_SIZE_W  5//5
//#define MEASURE_SIZE_H  3//5
void DrawDeMuraMeasurePattern(void)
{
    BYTE u8PosX=0;
    BYTE u8PosY=0;
    BYTE u8MeasureWindow_X=0;
    BYTE u8MeasureWindow_Y=0;
#if ENABLE_OSDC_OSDD
#if _LIMIT_OSDSIZE_
#define OSD_WIDTH_SIZE      LIMIT_OSDC_HSIZE
#define OSD_HEIGHT_SIZE     LIMIT_OSDC_VSIZE
#else
#define OSD_WIDTH_SIZE      (PANEL_WIDTH)
#define OSD_HEIGHT_SIZE     (PANEL_HEIGHT)
#endif
#else
#define OSD_WIDTH_SIZE      (PANEL_WIDTH)
#define OSD_HEIGHT_SIZE     (PANEL_HEIGHT)
#endif

    MENU_printData("USER_PREF_DEMURA_MEASURE_COMMAND << %d >> \n", USER_PREF_DEMURA_MEASURE_COMMAND);

    u8PosX=(USER_PREF_DEMURA_MEASURE_COMMAND%g_MEASURE_SIZE_W);
    u8PosY=(USER_PREF_DEMURA_MEASURE_COMMAND/g_MEASURE_SIZE_W);
    u8MeasureWindow_X=(g_u8OsdWindowWidth/(g_MEASURE_SIZE_W*2));
    u8MeasureWindow_Y=(g_u8OsdWindowHeight/(g_MEASURE_SIZE_H*2));

    Osd_SetTransparency(0);
    g_u8OsdFontColor=MONO_COLOR(CP_CLEAR,CP_CLEAR);
    Osd_DrawBlankPlane(0, 0, (BYTE)(OSD_WIDTH_SIZE/12), (BYTE)(OSD_HEIGHT_SIZE/18));
    if (DEMURA_MEASURE_STANDBY_FLAG)
    {
        MENU_printMsg("DeMura Measure Standby >> \n");
        g_u8OsdFontColor=MONO_COLOR(CP_GRAY_224,CP_GRAY_224);
        Osd_DrawBlankPlane(((g_u8OsdWindowWidth/g_MEASURE_SIZE_W)*u8PosX), ((g_u8OsdWindowHeight/g_MEASURE_SIZE_H)*u8PosY),(g_u8OsdWindowWidth/(g_MEASURE_SIZE_W)),(g_u8OsdWindowHeight/(g_MEASURE_SIZE_H)));
        g_u8OsdFontColor=MONO_COLOR(CP_WHITE,CP_WHITE);
        Osd_DrawBlankPlane((u8MeasureWindow_X/2)+((g_u8OsdWindowWidth/g_MEASURE_SIZE_W)*u8PosX), ((u8MeasureWindow_Y/2)+((g_u8OsdWindowHeight/g_MEASURE_SIZE_H)*u8PosY)),(g_u8OsdWindowWidth/(g_MEASURE_SIZE_W*2)),(g_u8OsdWindowHeight/(g_MEASURE_SIZE_H*2)));
    }
    else
    {
        MENU_printMsg("DeMura Measure ... >> \n");
        g_u8OsdFontColor=MONO_COLOR(CP_WHITE,CP_WHITE);
        Osd_DrawBlankPlane(((g_u8OsdWindowWidth/g_MEASURE_SIZE_W)*u8PosX), ((g_u8OsdWindowHeight/g_MEASURE_SIZE_H)*u8PosY),(g_u8OsdWindowWidth/(g_MEASURE_SIZE_W)),(g_u8OsdWindowHeight/(g_MEASURE_SIZE_H)));
    }
    CLR_DEMURA_MEASURE_STANDBY_FLAG();

#undef OSD_WIDTH_SIZE
#undef OSD_HEIGHT_SIZE
}
#endif

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
void DrawMainMenuMultiWindowInfo(void)
{

   printMsg("DrawMainMenuMultiWindowInfo");
    g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L2,CP_BK_COLOR_L2);
    Osd_DrawContinuousChar((MM_XSTART_L1+2+u8InputTextLen),     (MM_YSIZE-8), SPACE_FONT, ((MM_XSTART_L2+1)-(MM_XSTART_L1+2+u8InputTextLen)));
    Osd_DrawContinuousChar((MM_XSTART_L1+1+u8ResolutionTextLen),(MM_YSIZE-7), SPACE_FONT, 12);
    Osd_DrawContinuousChar((MM_XSTART_L2+2+u8InputTextLen),     (MM_YSIZE-8), SPACE_FONT, ((MM_XSTART_L2+1)-(MM_XSTART_L1+2+u8InputTextLen)));
    Osd_DrawContinuousChar((MM_XSTART_L2+1+u8ResolutionTextLen),(MM_YSIZE-7), SPACE_FONT, 12);
    Osd_DrawContinuousChar((MM_XSTART_L1+2+u8InputTextLen),     (MM_YSIZE-5), SPACE_FONT, ((MM_XSTART_L2+1)-(MM_XSTART_L1+2+u8InputTextLen)));
    Osd_DrawContinuousChar((MM_XSTART_L1+1+u8ResolutionTextLen),(MM_YSIZE-4), SPACE_FONT, 12);
 #if Enable_Multi_Win4		
    Osd_DrawContinuousChar((MM_XSTART_L2+2+u8InputTextLen),     (MM_YSIZE-5), SPACE_FONT, ((MM_XSTART_L2+1)-(MM_XSTART_L1+2+u8InputTextLen)));
    Osd_DrawContinuousChar((MM_XSTART_L2+1+u8ResolutionTextLen),(MM_YSIZE-4), SPACE_FONT, 12);
#endif	

    LoadCurrentResolutionStringFont();
    printData("(MapWin2Port(DISPLAY_WIN1)==%d",(MapWin2Port(DISPLAY_WIN1)));
    switch(USER_PREF_MULTI_WIN_MODE)
    {
        case eMULTIWIN_4:
            {
            g_u8OsdFontColor=MONO_COLOR(CP_MODEL_COLOR,CP_BK_COLOR_L2);
			
            //if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                DrawPropStr((MM_XSTART_L1+2+u8InputTextLen), (MM_YSIZE-8), InputSourceText_MultiWindow1());
            //if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                DrawPropStr((MM_XSTART_L2+2+u8InputTextLen), (MM_YSIZE-8), InputSourceText_MultiWindow2());
            //if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN3)))
                DrawPropStr((MM_XSTART_L1+2+u8InputTextLen), (MM_YSIZE-5), InputSourceText_MultiWindow3());
	#if Enable_Multi_Win4		
            //if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN4)))
                DrawPropStr((MM_XSTART_L2+2+u8InputTextLen), (MM_YSIZE-5), InputSourceText_MultiWindow4());
       #endif
            OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                DrawPropStr((MM_XSTART_L1+1+u8ResolutionTextLen), (MM_YSIZE-7), tPropText[2]);
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                DrawPropStr((MM_XSTART_L2+1+u8ResolutionTextLen), (MM_YSIZE-7), tPropText[3]);
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN3)))
                DrawPropStr((MM_XSTART_L1+1+u8ResolutionTextLen), (MM_YSIZE-4), tPropText[4]);
	#if Enable_Multi_Win4		
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN4)))
                DrawPropStr((MM_XSTART_L2+1+u8ResolutionTextLen), (MM_YSIZE-4), tPropText[5]);
	#endif		
            OSD_TEXT_HI_ADDR_CLR_TO_0(); //disable bit 8
            }
            break;
        case eMULTIWIN_3:
            {
            g_u8OsdFontColor=MONO_COLOR(CP_MODEL_COLOR,CP_BK_COLOR_L2);
            //if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                DrawPropStr((MM_XSTART_L1+2+u8InputTextLen), (MM_YSIZE-8), InputSourceText_MultiWindow1());
            //if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                DrawPropStr((MM_XSTART_L2+2+u8InputTextLen), (MM_YSIZE-8), InputSourceText_MultiWindow2());
            //if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN3)))
                DrawPropStr((MM_XSTART_L1+2+u8InputTextLen), (MM_YSIZE-5), InputSourceText_MultiWindow3());

            OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                DrawPropStr((MM_XSTART_L1+1+u8ResolutionTextLen), (MM_YSIZE-7), tPropText[2]);
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                DrawPropStr((MM_XSTART_L2+1+u8ResolutionTextLen), (MM_YSIZE-7), tPropText[3]);
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN3)))
                DrawPropStr((MM_XSTART_L1+1+u8ResolutionTextLen), (MM_YSIZE-4), tPropText[4]);
            OSD_TEXT_HI_ADDR_CLR_TO_0(); //disable bit 8
            }
            break;
        case eMULTIWIN_2_POP2:
        case eMULTIWIN_2_POP1:
        case eMULTIWIN_2_PIP:
            {
            g_u8OsdFontColor=MONO_COLOR(CP_MODEL_COLOR,CP_BK_COLOR_L2);
            //if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                DrawPropStr((MM_XSTART_L1+2+u8InputTextLen), (MM_YSIZE-8), InputSourceText_MultiWindow1());
           // if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                DrawPropStr((MM_XSTART_L2+2+u8InputTextLen), (MM_YSIZE-8), InputSourceText_MultiWindow2());

            OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN1)))
                DrawPropStr((MM_XSTART_L1+1+u8ResolutionTextLen), (MM_YSIZE-7), tPropText[2]);
            if (ReadyForChangeFlag(USER_PREF_WIN_SRC(DISPLAY_WIN2)))
                DrawPropStr((MM_XSTART_L2+1+u8ResolutionTextLen), (MM_YSIZE-7), tPropText[3]);
            OSD_TEXT_HI_ADDR_CLR_TO_0(); //disable bit 8
            }
            break;
        case eMULTIWIN_OFF:
            {
            g_u8OsdFontColor=MONO_COLOR(CP_MODEL_COLOR,CP_BK_COLOR_L2);
            DrawPropStr((MM_XSTART_L1+2+u8InputTextLen), (MM_YSIZE-8), InputSourceText_MultiWindow1());
					
            OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8
            DrawPropStr((MM_XSTART_L1+1+u8ResolutionTextLen), (MM_YSIZE-7), tPropText[2]);
            OSD_TEXT_HI_ADDR_CLR_TO_0(); //disable bit 8

            }
            break;
    }
}


#endif


#if (PanelType==PanelT430QVN01)  
#define KeyGuideXPos      0x02
#define KeyGuideYPos      0x01
#define KeyGuideShift      0x05

#define MainMenuKeyGuideXPos      0x02
#define MainMenuKeyGuideYPos      WF_YHEIGHT-3
#define MainMenuKeyGuideShift      0x09
#elif (PanelType==PanelM350DVR01_01)
#define KeyGuideXPos      0x02
#define KeyGuideYPos      0x01
#define KeyGuideShift      0x04

#define MainMenuKeyGuideXPos      0x02
#define MainMenuKeyGuideYPos      WF_YHEIGHT-3
#define MainMenuKeyGuideShift      0x09
#elif (PanelType==PanelLSM315DP01)
#define KeyGuideXPos      0x02
#define KeyGuideYPos      0x01
#define KeyGuideShift      0x05

#define MainMenuKeyGuideXPos      0x02
#define MainMenuKeyGuideYPos      WF_YHEIGHT-3
#define MainMenuKeyGuideShift      0x09
#elif (PanelType==PanelLSM315HP01)
     #if (defined(GK3B_Project))
     #define KeyGuideXPos      0x02
     #define KeyGuideYPos      0x01//0x0A
     #define KeyGuideShift      0x02
     
     #define MainMenuKeyGuideXPos      0x02
     #define MainMenuKeyGuideYPos      WF_YHEIGHT-3
     #define MainMenuKeyGuideShift      0x09
     #else
     #define KeyGuideXPos      0x01
     #define KeyGuideYPos      0x01
     #define KeyGuideShift      0x04
     
     #define MainMenuKeyGuideXPos      0x02
     #define MainMenuKeyGuideYPos      WF_YHEIGHT-3
     #define MainMenuKeyGuideShift      0x09
     #endif
#elif 0//(PanelType==PanelLSM270DP01)
#define KeyGuideXPos      0x02
#define KeyGuideYPos      0x02
#define KeyGuideShift      0x05

#define MainMenuKeyGuideXPos      0x02
#define MainMenuKeyGuideYPos      WF_YHEIGHT-3
#define MainMenuKeyGuideShift      0x09
#else
#define KeyGuideXPos      0x02
#define KeyGuideYPos      0x01
#define KeyGuideShift      0x06

#define MainMenuKeyGuideXPos      0x02
#define MainMenuKeyGuideYPos      WF_YHEIGHT-3
#define MainMenuKeyGuideShift      0x09
#endif





void DrawMessageFrame( void )
{
BYTE i,j;
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	if(InputStatusMultiWin)
		;
	else
#endif		
	{
		DisableAllSubWin();
	}

        if (IS_WARNING_MENU(g_u8MenuPageIndex)
        #if ENABLE_MULTI_INPUT  //Steven101116
            || g_u8MenuPageIndex==MENU_INPUT
        #endif
            )
            g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
        else
            g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
        Osd_DrawBlankPlane(0,0,GET_MENU_XSIZE(CURRENT_MENU),GET_MENU_YSIZE(CURRENT_MENU));
		
        if (IS_WARNING_MENU(g_u8MenuPageIndex)
        #if ENABLE_MULTI_INPUT
            || g_u8MenuPageIndex==MENU_INPUT
        #endif
	     //||HOT_MENU_STATUS(g_u8MenuPageIndex)	
	     ||g_u8MenuPageIndex==MENU_HOTPICTUREFORMAT
	 #if ENABLE_AUDIO_SETTINGS_MENU    
	     ||g_u8MenuPageIndex==MENU_HOTMUTE
	 #endif    

	 #if SleepMode_Enable    
	     ||g_u8MenuPageIndex==MENU_SLEEPMODE
	 #endif    
	     ||g_u8MenuPageIndex==MENU_HOTDCR
        #if ENABLE_VGA_INPUT
	     ||g_u8MenuPageIndex==MENU_AUTO
	     ||g_u8MenuPageIndex==MENU_HOT_AUTO
	#endif	
	     ||g_u8MenuPageIndex==MENU_HOTPICTUREMODE
#if HKC_RGB_Range_Func	 	
	     ||g_u8MenuPageIndex==MENU_HOTRGBRANGE
#endif	 	
            )
        {

           OSD_TEXT_HI_ADDR_SET_BIT8();
           g_u8OsdFontColor=MONO_COLOR(CP_SL_COLOR_SubFrame,CP_BK_COLOR_H);
           Osd_DrawRealChar(0,0,MM_Top_Left);
           Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,0,MM_Top_Right);
		   
           Osd_DrawRealChar(0,GET_MENU_YSIZE(CURRENT_MENU)-1,MM_Bottom_Left);
           Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,GET_MENU_YSIZE(CURRENT_MENU)-1,MM_Bottom_Right);
           
           g_u8OsdFontColor=MONO_COLOR(CP_SL_COLOR_SubFrame,CP_BK_COLOR_H);
           Osd_DrawContinuousChar(1, 0, LOWBYTE(MM_Top_Mid), GET_MENU_XSIZE(CURRENT_MENU)-2);
           Osd_DrawContinuousChar(1, GET_MENU_YSIZE(CURRENT_MENU)-1, LOWBYTE(MM_Bottom_Mid), GET_MENU_XSIZE(CURRENT_MENU)-2);

           g_u8OsdFontColor=MONO_COLOR(CP_SL_COLOR_SubFrame,CP_BK_COLOR_H);
           for(i=1; i<GET_MENU_YSIZE(CURRENT_MENU)-1; i++)
           {
              Osd_DrawRealChar(0, i, MM_Left_Mid);
           }
           for(i=1; i<GET_MENU_YSIZE(CURRENT_MENU)-1; i++)
           {
              Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MM_Rightt_Mid);
           }

           if(IS_WARNING_MENU(g_u8MenuPageIndex))
           {
              g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_SEL,CP_BK_COLOR_H);
              Osd_DrawContinuousChar(1, 3, LOWBYTE(MAIN_Mid_Line), GET_MENU_XSIZE(CURRENT_MENU)-2);
              g_u8OsdFontColor=FOUR_COLOR(41);
              OSD_TEXT_HI_ADDR_SET_BIT9();
              Osd_DrawRealChar(0, 3, MM_4COLOR_CURSOR_START+8*0x02);//topleft
              Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, 3, MM_4COLOR_CURSOR_START+9*0x02);//topright

              OSD_TEXT_HI_ADDR_SET_BIT8();
              g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_SEL,CP_BK_COLOR_H);
            for (i=0; i<2; i++)
              for(j=0; j<14; j++)
              {
                 Osd_DrawRealChar(11+j, 1+i, strMSGWindow[i][j]);
              }	
			  
              OSD_TEXT_HI_ADDR_CLR_TO_0();
           }
    	   

        #if ENABLE_MULTI_INPUT
            if( g_u8MenuPageIndex==MENU_INPUT)
            {
               OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
              g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H);
 		if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)
 		{
                  for (i=0; i<2; i++)
                  for(j=0; j<5; j++)
                  {
                     Osd_DrawRealChar(8+j, 2+i, strDP_Window[i][j]);
                  }	
		}
		#if ENABLE_HDMI2ND_INPUT
		else if(USER_PREF_INPUT_TYPE==INPUT_HDMI2ND)
		{
                  for (i=0; i<2; i++)
                  for(j=0; j<10; j++)
                  {
                     Osd_DrawRealChar(6+j, 2+i, strHDM2Window[i][j]);
                  }	
		 }
		else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)
		{
                  for (i=0; i<2; i++)
                  for(j=0; j<10; j++)
                  {
                     Osd_DrawRealChar(6+j, 2+i, strHDM1Window[i][j]);
                  }	

		}
		#else
		else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)
		{
                  for (i=0; i<2; i++)
                  for(j=0; j<9; j++)
                  {
                     Osd_DrawRealChar(6+j, 2+i, strHDMWindow[i][j]);
                  }	

		}
	        #endif
	  #if ENABLE_DVI_INPUT	
		else if(USER_PREF_INPUT_TYPE==INPUT_DVI)
		{
                  for (i=0; i<2; i++)
                  for(j=0; j<7; j++)
                  {
                     Osd_DrawRealChar(7+j, 2+i, strDVIWindow[i][j]);
                  }	

		}
	 #endif	
               OSD_TEXT_HI_ADDR_CLR_TO_0();	 		
            }
        #endif
            if( g_u8MenuPageIndex==MENU_HOTPICTUREMODE)
            {
               OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
              g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H);
 		if(CURRENT_PICTURE_MODE==CT_STANDARD)
 		{
 		 if(USER_PREF_LANGUAGE==LANG_French)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<14; j++)
                  {
                     Osd_DrawRealChar(3+j, 2+i, strStaWindow_Fren[i][j]);
                  }	
 		 }
		 else if(USER_PREF_LANGUAGE==LANG_Spanish)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<13; j++)
                  {
                     Osd_DrawRealChar(4+j, 2+i, strStaWindow_Span[i][j]);
                  }	
 		 }
		 else if(USER_PREF_LANGUAGE==LANG_TChinese)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<6; j++)
                  {
                     Osd_DrawRealChar(7+j, 2+i, strStaWindow_TCH[i][j]);
                  }	
 		 }
		 else
		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<13; j++)
                  {
                     Osd_DrawRealChar(4+j, 2+i, strStaWindow[i][j]);
                  }	
		 }
		}
 		else if(CURRENT_PICTURE_MODE==CT_USER)
 		{
 		 if(USER_PREF_LANGUAGE==LANG_French)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<14; j++)
                  {
                     Osd_DrawRealChar(3+j, 2+i, strUseWindow_Fren[i][j]);
                  }	
 		 }
 		 else if(USER_PREF_LANGUAGE==LANG_Spanish)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<12; j++)
                  {
                     Osd_DrawRealChar(4+j, 2+i, strUseWindow_Span[i][j]);
                  }	
 		 }
 		 else if(USER_PREF_LANGUAGE==LANG_TChinese)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<12; j++)
                  {
                     Osd_DrawRealChar(5+j, 2+i, strUseWindow_TCH[i][j]);
                  }	
 		 }
		 else
		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<7; j++)
                  {
                     Osd_DrawRealChar(7+j, 2+i, strUSEWindow[i][j]);
                  }	
		 }
		}
 		else if(CURRENT_PICTURE_MODE==CT_MOVIE)
 		{
 		 if(USER_PREF_LANGUAGE==LANG_French)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<6; j++)
                  {
                     Osd_DrawRealChar(7+j, 2+i, strMovWindow_Fren[i][j]);
                  }	
 		 }
 		 else if(USER_PREF_LANGUAGE==LANG_Spanish)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<12; j++)
                  {
                     Osd_DrawRealChar(4+j, 2+i, strMovWindow_Span[i][j]);
                  }	
 		 }
 		 else if(USER_PREF_LANGUAGE==LANG_TChinese)
 		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<6; j++)
                  {
                     Osd_DrawRealChar(7+j, 2+i, strMovWindow_TCH[i][j]);
                  }	
 		 }
		 else
		 {
                  for (i=0; i<2; i++)
                  for(j=0; j<9; j++)
                  {
                     Osd_DrawRealChar(6+j, 2+i, strMovWindow[i][j]);
                  }	
		 }
		}
 		else if(CURRENT_PICTURE_MODE==CT_VIVID)
 		{
                  for (i=0; i<2; i++)
                  for(j=0; j<7; j++)
                  {
                     Osd_DrawRealChar(7+j, 2+i, strECOWindow[i][j]);
                  }	
		}
 		else if(CURRENT_PICTURE_MODE==CT_FPS)
 		{
                  for (i=0; i<2; i++)
                  for(j=0; j<7; j++)
                  {
                     Osd_DrawRealChar(7+j, 2+i, strFPSWindow[i][j]);
                  }	
		}
 		else if(CURRENT_PICTURE_MODE==CT_RTS)
 		{
                  for (i=0; i<2; i++)
                  for(j=0; j<7; j++)
                  {
                     Osd_DrawRealChar(7+j, 2+i, strRTSWindow[i][j]);
                  }	
		}

               OSD_TEXT_HI_ADDR_CLR_TO_0();	 		
            }
		

        }
       else if(g_u8MenuPageIndex==MENU_HOTINPUT)
       {
               OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
              g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H);
                  for (i=0; i<2; i++)
                  for(j=0; j<19; j++)
                  {
                     Osd_DrawRealChar(2+j, i, strSouWindow[i][j]);
                  }	
               OSD_TEXT_HI_ADDR_CLR_TO_0();	 		
       }
    #if 0//LED_HOT_Menu_Func    
 	else if(g_u8MenuPageIndex==MENU_HOTLEDMODE)
	 {  BYTE i;
             
           //g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_TRANSPARENT_COLOR);
           g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_TRANSPARENT_COLOR);
          //Top
           Osd_DrawRealChar(0,0,0x10+SM_CHECK_2C_FONT_START-9);
           Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,0,0x11+SM_CHECK_2C_FONT_START-9);
		   
            g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_FG_COLOR_L2);
          Osd_DrawContinuousChar(1, 0, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
           Osd_DrawContinuousChar(0, 1, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));
           Osd_DrawContinuousChar(0, 2, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));

		   
         //Buttom
           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_TRANSPARENT_COLOR);
           Osd_DrawRealChar(0,GET_MENU_YSIZE(CURRENT_MENU)-1,0x12+SM_CHECK_2C_FONT_START-9);
           Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,GET_MENU_YSIZE(CURRENT_MENU)-1,0x13+SM_CHECK_2C_FONT_START-9);

           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
           Osd_DrawContinuousChar(1, GET_MENU_YSIZE(CURRENT_MENU)-1, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);

		   
           for(i=3; i<GET_MENU_YSIZE(CURRENT_MENU)-1; i++)
           {
              Osd_DrawContinuousChar(0, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));
           }
 
 	 }
    #endif    
   #if (defined(LianHeChuangXin_Project))    
	 else if(g_u8MenuPageIndex==MENU_KEY_GUIDE)
	 { 
             
           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_TRANSPARENT_COLOR);
           Osd_DrawRealChar(0,0,0x10+SM_CHECK_2C_FONT_START-9);
           Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,0,0x11+SM_CHECK_2C_FONT_START-9);

           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
           Osd_DrawContinuousChar(1, 0, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
           for(i=1; i<GET_MENU_YSIZE(CURRENT_MENU); i++)
           {
              Osd_DrawContinuousChar(0, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));
           }
			 
	   #if (defined(GK3B_Project))
            g_u8OsdFontColor=MONO_COLOR(CP_Orange,CP_BK_COLOR_H);
	      OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	

                   Osd_DrawRealChar(KeyGuideXPos, KeyGuideYPos+KeyGuideShift*0,(LOBYTE(MM_GUIDE_FONT_START))+4*2);
                   Osd_DrawRealChar(KeyGuideXPos+1, KeyGuideYPos+KeyGuideShift*0,(LOBYTE(MM_GUIDE_FONT_START))+1+4*2);

                   Osd_DrawRealChar(KeyGuideXPos, KeyGuideYPos+1+KeyGuideShift*1,(LOBYTE(MM_GUIDE_FONT_START))+0*2);
                   Osd_DrawRealChar(KeyGuideXPos+1, KeyGuideYPos+1+KeyGuideShift*1,(LOBYTE(MM_GUIDE_FONT_START))+1+0*2);

                   Osd_DrawRealChar(KeyGuideXPos, KeyGuideYPos+1+KeyGuideShift*2,(LOBYTE(MM_GUIDE_FONT_START))+1*2);
                   Osd_DrawRealChar(KeyGuideXPos+1, KeyGuideYPos+1+KeyGuideShift*2,(LOBYTE(MM_GUIDE_FONT_START))+1+1*2);

                   Osd_DrawRealChar(KeyGuideXPos, KeyGuideYPos+1+KeyGuideShift*3,(LOBYTE(MM_GUIDE_FONT_START))+2*2);
                   Osd_DrawRealChar(KeyGuideXPos+1, KeyGuideYPos+1+KeyGuideShift*3,(LOBYTE(MM_GUIDE_FONT_START))+1+2*2);

                   Osd_DrawRealChar(KeyGuideXPos, KeyGuideYPos+1+KeyGuideShift*4,(LOBYTE(MM_GUIDE_FONT_START))+3*2);
                   Osd_DrawRealChar(KeyGuideXPos+1, KeyGuideYPos+1+KeyGuideShift*4,(LOBYTE(MM_GUIDE_FONT_START))+1+3*2);

		OSD_TEXT_HI_ADDR_CLR_TO_0();	 
	   #else
            g_u8OsdFontColor=MONO_COLOR(CP_Orange,CP_BK_COLOR_H);
	      OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
             for(i=0; i<5; i++)
          	{
                Osd_DrawRealChar(KeyGuideXPos+KeyGuideShift*i, KeyGuideYPos,(LOBYTE(MM_GUIDE_FONT_START))+i*2);
                Osd_DrawRealChar(KeyGuideXPos+1+KeyGuideShift*i, KeyGuideYPos,(LOBYTE(MM_GUIDE_FONT_START))+1+i*2);
          	}
		OSD_TEXT_HI_ADDR_CLR_TO_0();	 
	   #endif

	 }
   #endif    

}


void ClearMainFrame(void)
{ BYTE i;
 	if(IS_MAIN_L0(g_u8MenuPageIndex))
 	{
        //printData("g_u8PrevMenuItemIndex==%d",g_u8PrevMenuItemIndex);
       OSD_TEXT_HI_ADDR_SET_BIT8();	
      //Clear Select Sub Item
      if(g_u8PrevMenuPageIndex==MENU_QUICKSTART)	
      	{
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            //Osd_DrawContinuousChar(1, (QuickStartTxtYPos+g_u8PrevMenuItemIndex*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
            Osd_DrawRealChar(0, (QuickStartTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Left_Line);
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (QuickStartTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Right_Line);
      	}	
	else if(g_u8PrevMenuPageIndex==MENU_PICTURE)  
	{
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            //Osd_DrawContinuousChar(1, (PictureTxtYPos+g_u8PrevMenuItemIndex*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
            Osd_DrawRealChar(0, (PictureTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Left_Line);
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (PictureTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Right_Line);

	}
	else if(g_u8PrevMenuPageIndex==MENU_COLOR)
	{
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            //Osd_DrawContinuousChar(1, (ColorTxtYPos+g_u8PrevMenuItemIndex*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
         if(g_u8PrevMenuItemIndex>=2)
	   {
            Osd_DrawRealChar(0, (ColorTxtYPos+4+(g_u8PrevMenuItemIndex/2)*1), MAIN_Left_Line);
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (ColorTxtYPos+4+(g_u8PrevMenuItemIndex/2)*1), MAIN_Right_Line);
	   }
	 else
	   {
            Osd_DrawRealChar(0, (ColorTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Left_Line);
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (ColorTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Right_Line);
	   }
	}
	else if(g_u8PrevMenuPageIndex==MENU_SYSTEM)
	{
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            //Osd_DrawContinuousChar(1, (SystemTxtYPos+g_u8PrevMenuItemIndex*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
            Osd_DrawRealChar(0, (SystemTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Left_Line);
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (SystemTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Right_Line);
	}
#if LED_HOT_Menu_Func	
	else if(g_u8PrevMenuPageIndex==MENU_SYSTEMLLEDUSER)
	{
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            //Osd_DrawContinuousChar(1, (SystemTxtYPos+g_u8PrevMenuItemIndex*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
          if(g_u8PrevMenuItemIndex>=Sys_Language_IDX)
         {
            Osd_DrawRealChar(0, (SystemLedModeLEDUSERTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Left_Line);
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (SystemLedModeLEDUSERTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Right_Line);
         }
	  else
	  {
            Osd_DrawRealChar(0, (SystemTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Left_Line);
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (SystemTxtYPos+(g_u8PrevMenuItemIndex/2)*1), MAIN_Right_Line);
	  }
	}
#endif
      //Clear Select Sub Background
      /*
            g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
            
            for (i=(7+4*(g_u8PrevMenuPageIndex-MENU_QUICKSTART)); i<(7+4*((g_u8PrevMenuPageIndex-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            }	
     */       
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);
#if LED_HOT_Menu_Func	
	if(g_u8PrevMenuPageIndex==MENU_SYSTEMLLEDUSER)
	{
            for(i=(7+4*(MENU_SYSTEM-MENU_QUICKSTART)); i<(7+4*((MENU_SYSTEM-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawRealChar(0, i, MAIN_Left_Line);
            }
            for(i=(7+4*(MENU_SYSTEM-MENU_QUICKSTART)); i<(7+4*((MENU_SYSTEM-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
            }
	}
	
	else
#endif	
	{
            for(i=(7+4*(g_u8PrevMenuPageIndex-MENU_QUICKSTART)); i<(7+4*((g_u8PrevMenuPageIndex-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawRealChar(0, i, MAIN_Left_Line);
            }
            for(i=(7+4*(g_u8PrevMenuPageIndex-MENU_QUICKSTART)); i<(7+4*((g_u8PrevMenuPageIndex-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
            }
	}

/////////////////////////////////////////////////////VGA Setting
#if ENABLE_VGA_INPUT
        if(g_u8PrevMenuPageIndex==MENU_PICTURE)
        {

	/*	
         //if(!CURRENT_INPUT_IS_VGA())
          {
            g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
            
            for (i=(VGA_SettingY_Pos); i<(VGA_SettingY_Pos+4*1); i++)
            {
               Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            }	
         }
	*/		
         //if(!CURRENT_INPUT_IS_VGA())
         {
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);
			
            for(i=VGA_SettingY_Pos; i<(VGA_SettingY_Pos+4*1); i++)
            {
                Osd_DrawRealChar(0, i, MAIN_Left_Line);
            }
            for(i=VGA_SettingY_Pos; i<(VGA_SettingY_Pos+4*1); i++)
            {
                Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
            }
         }				
              if(!CURRENT_INPUT_IS_VGA())
              {
                   g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
              /*     
                   for (i=(VGA_SettingY_Pos+4); i<((VGA_SettingY_Pos+4)+9); i++)
                   {
                    Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                   }	
		*/		   
                   g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);
       			
                   for(i=(VGA_SettingY_Pos+4); i<((VGA_SettingY_Pos+4)+9); i++)
                   {
                    Osd_DrawRealChar(0, i, MAIN_Left_Line);
                   }
                   for(i=(VGA_SettingY_Pos+4); i<((VGA_SettingY_Pos+4)+9); i++)
                   {
                    Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
                   }

              }
          }
#endif		
/////////////////////////////////////////////////////
	OSD_TEXT_HI_ADDR_CLR_TO_0();	  
       }


}




static void DrawFrame(void)
{
BYTE i;
BYTE j;

    if (IS_WARNING_MENU(g_u8MenuPageIndex) || IS_MSG_STATUS(g_u8MenuPageIndex)
       ||g_u8MenuPageIndex==MENU_HOTPICTUREFORMAT
    #if ENABLE_AUDIO_SETTINGS_MENU   
       ||g_u8MenuPageIndex==MENU_HOTMUTE
    #endif   
       ||g_u8MenuPageIndex==MENU_HOTINPUT

    #if SleepMode_Enable   
       ||g_u8MenuPageIndex==MENU_SLEEPMODE
    #endif   
       ||g_u8MenuPageIndex==MENU_HOTDCR
#if ENABLE_VGA_INPUT
       ||g_u8MenuPageIndex==MENU_AUTO
       ||g_u8MenuPageIndex==MENU_HOT_AUTO
#endif	
       ||g_u8MenuPageIndex==MENU_HOTPICTUREMODE
#if HKC_RGB_Range_Func	 	
	||g_u8MenuPageIndex==MENU_HOTRGBRANGE
#endif	 	
   #if (defined(LianHeChuangXin_Project))    
       ||g_u8MenuPageIndex==MENU_KEY_GUIDE
   #endif    
   #if 0//LED_HOT_Menu_Func    
       ||g_u8MenuPageIndex==MENU_HOTLEDMODE
   #endif    
	)//||HOT_MENU_STATUS(g_u8MenuPageIndex))
    {
         DrawMessageFrame();
    }
    else if ((IS_MAIN_L0(g_u8MenuPageIndex))&&(g_u8PrevMenuPageIndex==MENU_ROOT))
    {

	g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
	OSD_TEXT_HI_ADDR_SET_BIT8();
       Osd_DrawRealChar(0,0,MAIN_Top_Left);//topleft
       Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,0,MAIN_Top_Right);//topright
       Osd_DrawRealChar(0,GET_MENU_YSIZE(CURRENT_MENU)-1,MAIN_Bottom_Left);
       Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,GET_MENU_YSIZE(CURRENT_MENU)-1,MAIN_Bottom_Right);//topright

       Osd_DrawContinuousChar(1, 0, LOWBYTE(MAIN_Top_Mid), GET_MENU_XSIZE(CURRENT_MENU)-2);
       for(i=1; i<(GET_MENU_YSIZE(CURRENT_MENU)-1); i++)
        {
              Osd_DrawRealChar(0, i, MAIN_Left_Line);
        }
       for(i=1; i<(GET_MENU_YSIZE(CURRENT_MENU)-1); i++)
       {
              Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
       }
       Osd_DrawContinuousChar(1, GET_MENU_YSIZE(CURRENT_MENU)-1, LOWBYTE(MAIN_Bottom_Mid), GET_MENU_XSIZE(CURRENT_MENU)-2);

//Topline
	g_u8OsdFontColor=MONO_COLOR(0x04,0x01);
       Osd_DrawContinuousChar(1, Main_TopLine_YPos, LOWBYTE(MAIN_Top_Line), GET_MENU_XSIZE(CURRENT_MENU)-2);
	g_u8OsdFontColor=FOUR_COLOR(4);
	OSD_TEXT_HI_ADDR_SET_BIT9();
       Osd_DrawRealChar(0,Main_TopLine_YPos,MM_4COLOR_CURSOR_START);//topleft
       Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,Main_TopLine_YPos, MM_4COLOR_CURSOR_START+1*0x02);//topright
       OSD_TEXT_HI_ADDR_CLR_TO_0();

//Description 
	OSD_TEXT_HI_ADDR_SET_BIT8();
	g_u8OsdFontColor=MONO_COLOR(0x04,0x01);
       Osd_DrawContinuousChar(1, Main_Description_YPos, LOWBYTE(MAIN_Mid_Line), GET_MENU_XSIZE(CURRENT_MENU)-2);
	g_u8OsdFontColor=FOUR_COLOR(4);
	OSD_TEXT_HI_ADDR_SET_BIT9();
       Osd_DrawRealChar(0,Main_Description_YPos,MM_4COLOR_CURSOR_START+2*0x02);//topleft
       Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,Main_Description_YPos, MM_4COLOR_CURSOR_START+3*0x02);//topright
       OSD_TEXT_HI_ADDR_CLR_TO_0();


	g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H);
	if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
	for (i=0; i<2; i++)
	for(j=0; j<5; j++)
		{
			 Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_TChinese[i][j]);
		}
	}
	else if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
	for (i=0; i<2; i++)
	for(j=0; j<12; j++)
		{
			 Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_Span[i][j]);
		}
	}
	else
	{
		for (i=0; i<2; i++)
		for(j=0; j<11; j++)
		{
	          if(USER_PREF_LANGUAGE==LANG_French)
			 Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_Fran[i][j]);
		   else 	  
			 Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow[i][j]);
		}
	}
//Buttom 
	OSD_TEXT_HI_ADDR_SET_BIT8();
	g_u8OsdFontColor=MONO_COLOR(0x04,0x01);
       Osd_DrawContinuousChar(1, Main_Buttom_YPos, LOWBYTE(MAIN_Mid_Line), GET_MENU_XSIZE(CURRENT_MENU)-2);
	g_u8OsdFontColor=FOUR_COLOR(4);
	OSD_TEXT_HI_ADDR_SET_BIT9();
       Osd_DrawRealChar(0,Main_Buttom_YPos,MM_4COLOR_CURSOR_START+2*0x02);//topleft
       Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,Main_Buttom_YPos, MM_4COLOR_CURSOR_START+3*0x02);//topright
       OSD_TEXT_HI_ADDR_CLR_TO_0();

//Title Logo
	g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
	OSD_TEXT_HI_ADDR_SET_BIT9();
	for (i=0; i<3; i++)
	for(j=0; j<30; j++)
	{
	   	
		 Osd_DrawRealChar(1+j, 1+i, strTitLogoWindow[i][j]);
	}	
       OSD_TEXT_HI_ADDR_CLR_TO_0();

	g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H);
       DrawPropStr(0x01, (Main_TopLine_YPos-1), InputSourceText_MultiWindow1());

	   LoadCurrentResolutionStringFont();

 	OSD_TEXT_HI_ADDR_SET_BIT9();
       DrawPropStr((MM_XSIZE-1-u8ResolutionTextLen), (Main_TopLine_YPos-1), Txt_tPropText0());
       OSD_TEXT_HI_ADDR_CLR_TO_0();

    }
    else if ((IS_MAIN_L1(g_u8MenuPageIndex))||(IS_MAIN_L0(g_u8MenuPageIndex)))  //SubMenu
    {

         if(IS_MAIN_L0(g_u8MenuPageIndex))
         {
            g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H);
		if(USER_PREF_LANGUAGE==LANG_TChinese)
		{
		for (i=0; i<2; i++)
		for(j=0; j<5; j++)
			{
				 Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_TChinese[i][j]);
			}
		}
		else if(USER_PREF_LANGUAGE==LANG_Spanish)
		{
		for (i=0; i<2; i++)
		for(j=0; j<12; j++)
			{
				 Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_Span[i][j]);
			}
		}
		else
		{
	            for (i=0; i<2; i++)
	             for(j=0; j<11; j++)
	            {
	                if(USER_PREF_LANGUAGE==LANG_French)
	                Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_Fran[i][j]);
	                else 	  
	                Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow[i][j]);
	            }	
		}

         }
         else if(IS_MAIN_L1(g_u8MenuPageIndex))
         {
          //Midline
           if((g_u8MenuPageIndex!=MENU_SYSTEM)
          #if LED_HOT_Menu_Func
		   &&(g_u8MenuPageIndex!=MENU_SYSTEMLLEDUSER)
         #endif
	    )
 	    {
             g_u8OsdFontColor=MONO_COLOR(0x04,0x01);
	      OSD_TEXT_HI_ADDR_SET_BIT8();		 
             Osd_DrawContinuousChar(1, (Sub_MidLine_YPos+(g_u8MenuPageIndex-MENU_QUICKSTART)*4), LOWBYTE(MAIN_Top_Line), GET_MENU_XSIZE(CURRENT_MENU)-2);
             g_u8OsdFontColor=FOUR_COLOR(4);
             OSD_TEXT_HI_ADDR_SET_BIT9();
             Osd_DrawRealChar(0,(Sub_MidLine_YPos+(g_u8MenuPageIndex-MENU_QUICKSTART)*4),MM_4COLOR_CURSOR_START+0*0x02);//topleft
             Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1,(Sub_MidLine_YPos+(g_u8MenuPageIndex-MENU_QUICKSTART)*4), MM_4COLOR_CURSOR_START+1*0x02);//topright
             OSD_TEXT_HI_ADDR_CLR_TO_0();
           }

      //Select Sub Background
            g_u8OsdFontColor=MONO_COLOR(CP_SubGround,CP_SubGround);
    #if LED_HOT_Menu_Func	  
          if(g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER)
          {
            for (i=(7+4*(MENU_SYSTEM-MENU_QUICKSTART)); i<(7+4*((MENU_SYSTEM-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            }	
            OSD_TEXT_HI_ADDR_SET_BIT8();
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_SubGround);
            for(i=(7+4*(MENU_SYSTEM-MENU_QUICKSTART)); i<(7+4*((MENU_SYSTEM-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawRealChar(0, i, MAIN_Left_Line);
            }
            for(i=(7+4*(MENU_SYSTEM-MENU_QUICKSTART)); i<(7+4*((MENU_SYSTEM-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
            }
            OSD_TEXT_HI_ADDR_CLR_TO_0();
          }
	   else
     #endif	   
	   {
            for (i=(7+4*(g_u8MenuPageIndex-MENU_QUICKSTART)); i<(7+4*((g_u8MenuPageIndex-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            }	
            OSD_TEXT_HI_ADDR_SET_BIT8();
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_SubGround);
            for(i=(7+4*(g_u8MenuPageIndex-MENU_QUICKSTART)); i<(7+4*((g_u8MenuPageIndex-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawRealChar(0, i, MAIN_Left_Line);
            }
            for(i=(7+4*(g_u8MenuPageIndex-MENU_QUICKSTART)); i<(7+4*((g_u8MenuPageIndex-MENU_QUICKSTART)+1)); i++)
            {
             Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
            }
            OSD_TEXT_HI_ADDR_CLR_TO_0();
	   }


	//Sub Icon1
              OSD_TEXT_HI_ADDR_SET_BIT9();
                  if(g_u8MenuPageIndex==MENU_QUICKSTART)
		     g_u8OsdFontColor=FOUR_COLOR(23);	
		    else
		     g_u8OsdFontColor=FOUR_COLOR(22);	
   
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                	 Osd_DrawRealChar(4+j, 8+i, strSubQuWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();

	//Sub Text1
           if(g_u8MenuPageIndex==MENU_QUICKSTART)
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_SubGround);
           else
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	 if(USER_PREF_LANGUAGE==LANG_Spanish)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<20; j++)
                {
                	 Osd_DrawRealChar(10+j, 8+i, strQuiWindow_Span[i][j]);
                }	
	 }
	 else if(USER_PREF_LANGUAGE==LANG_French)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, 8+i, strQuiWindow_Fran[i][j]);
                }	
	 }
	 else if(USER_PREF_LANGUAGE==LANG_TChinese)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<10; j++)
                {
                	 Osd_DrawRealChar(14+j, 8+i, strQuiWindow_TChinese[i][j]);
                }	
	 }
	 else
	 {
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, 8+i, strQuiWindow[i][j]);
                }	
	 }
	 
	if(g_u8MenuPageIndex==MENU_PICTURE)
	{
	//Sub Icon2
              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                   if(strSubPicWindow[i][j]==SubPic(0)||strSubPicWindow[i][j]==SubPic(2)||strSubPicWindow[i][j]==SuEPic(0)||strSubPicWindow[i][j]==SuEPic(2))
		       g_u8OsdFontColor=FOUR_COLOR(26);	
		     else// if(strSubPicWindow[i][j]==SubPic(1)||strSubPicWindow[i][j]==SubPic(3)||strSubPicWindow[i][j]==SuEPic(1)||strSubPicWindow[i][j]==SuEPic(3))	
		       g_u8OsdFontColor=FOUR_COLOR(30);
                	 Osd_DrawRealChar(4+j, (8+1*4)+i, strSubPicWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();
	
	//Sub Text2
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_SubGround);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+1*4)+i, strPicWindow_Span[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
              for (i=0; i<2; i++)
                for(j=0; j<9; j++)
                {
                	 Osd_DrawRealChar(14+j, (8+1*4)+i, strPicWindow_Fran[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+1*4)+i, strPicWindow_TChinese[i][j]);
                }	
	}
	else
	{
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+1*4)+i, strPicWindow[i][j]);
                }	
	}
/////////////////////////////////////////////////////VGA Setting
#if ENABLE_VGA_INPUT
         if(CURRENT_INPUT_IS_VGA())
            g_u8OsdFontColor=MONO_COLOR(CP_SubGround,CP_SubGround);
         else
            g_u8OsdFontColor=MONO_COLOR(CP_UnSelGround,CP_UnSelGround);
            
            for (i=(VGA_SettingY_Pos); i<(VGA_SettingY_Pos+4*1); i++)
            {
               Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            }	
			
         if(CURRENT_INPUT_IS_VGA())
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_SubGround);
	  else		
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_UnSelGround);
	     OSD_TEXT_HI_ADDR_SET_BIT8();		
            for(i=VGA_SettingY_Pos; i<(VGA_SettingY_Pos+4*1); i++)
            {
                Osd_DrawRealChar(0, i, MAIN_Left_Line);
            }
            for(i=VGA_SettingY_Pos; i<(VGA_SettingY_Pos+4*1); i++)
            {
                Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
            }
	    OSD_TEXT_HI_ADDR_CLR_TO_0();
		
              if(CURRENT_INPUT_IS_VGA())
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_SubGround);
           else
                g_u8OsdFontColor=MONO_COLOR(CP_UnSel,CP_UnSelGround);
              for (i=0; i<2; i++)
                for(j=0; j<20; j++)
                {
                	 Osd_DrawRealChar(2+j, (VGA_SettingY_Pos+1)+i, strVGAWindow[i][j]);
                }	
				
              if(!CURRENT_INPUT_IS_VGA())
              {
                   g_u8OsdFontColor=MONO_COLOR(CP_Black,CP_Black);
                   
                   for (i=(VGA_SettingY_Pos+4); i<((VGA_SettingY_Pos+4)+9); i++)
                   {
                    Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                   }	
                   g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Black);
       	OSD_TEXT_HI_ADDR_SET_BIT8();				
                   for(i=(VGA_SettingY_Pos+4); i<((VGA_SettingY_Pos+4)+9); i++)
                   {
                    Osd_DrawRealChar(0, i, MAIN_Left_Line);
                   }
                   for(i=(VGA_SettingY_Pos+4); i<((VGA_SettingY_Pos+4)+9); i++)
                   {
                    Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
                   }
	      OSD_TEXT_HI_ADDR_CLR_TO_0();
              }
#endif			 
/////////////////////////////////////////////////////

	//Sub Icon3
              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                   if(strSubColWindow[i][j]==SubCol(5))
		       g_u8OsdFontColor=FOUR_COLOR(34);	
	 	     else if(strSubColWindow[i][j]==SubCol(6))		   
		       g_u8OsdFontColor=FOUR_COLOR(36);	
		     else 
		       g_u8OsdFontColor=FOUR_COLOR(32);
                	 Osd_DrawRealChar(4+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strSubColWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();				
	//Sub Text3
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);

	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strColWindow_Span[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strColWindow_Fran[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strColWindow_TChinese[i][j]);
                }	
	}
	else
	{
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strColWindow[i][j]);
                }	
	}

	//Sub Icon4
   	         g_u8OsdFontColor=FOUR_COLOR(38);
           OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                	 Osd_DrawRealChar(4+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strSubSysWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();

	//Sub Text4
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strSysWindow_Span[i][j]);
                }
	 }
	else if(USER_PREF_LANGUAGE==LANG_French)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<13; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strSysWindow_Fran[i][j]);
                }
	 }
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strSysWindow_TChinese[i][j]);
                }
	 }
	else
	 {
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strSYSWindow[i][j]);
                }
	 }
				
	}
	else if(g_u8MenuPageIndex==MENU_COLOR)
	{
	//Sub Icon2
              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                   if(strSubPicWindow[i][j]==SubPic(0)||strSubPicWindow[i][j]==SubPic(2)||strSubPicWindow[i][j]==SuEPic(0)||strSubPicWindow[i][j]==SuEPic(2))
		       g_u8OsdFontColor=FOUR_COLOR(24);	
		     else// if(strSubPicWindow[i][j]==SubPic(1)||strSubPicWindow[i][j]==SubPic(3)||strSubPicWindow[i][j]==SuEPic(1)||strSubPicWindow[i][j]==SuEPic(3))	
		       g_u8OsdFontColor=FOUR_COLOR(28);
                	 Osd_DrawRealChar(4+j, (8+1*4)+i, strSubPicWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();
	
	//Sub Text2
                 g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+1*4)+i, strPicWindow_Span[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
              for (i=0; i<2; i++)
                for(j=0; j<9; j++)
                {
                	 Osd_DrawRealChar(14+j, (8+1*4)+i, strPicWindow_Fran[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+1*4)+i, strPicWindow_TChinese[i][j]);
                }	
	}
	else
	{
            for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+1*4)+i, strPicWindow[i][j]);
                }	
	}

	//Sub Icon3
              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                   if(strSubColWindow[i][j]==SubCol(5))
		       g_u8OsdFontColor=FOUR_COLOR(35);	
	 	     else if(strSubColWindow[i][j]==SubCol(6))		   
		       g_u8OsdFontColor=FOUR_COLOR(37);	
		     else 
		       g_u8OsdFontColor=FOUR_COLOR(33);
                	 Osd_DrawRealChar(4+j, (8+2*4)+i, strSubColWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();				
	//Sub Text3
                 g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_SubGround);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
           for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+2*4)+i, strColWindow_Span[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
           for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+2*4)+i, strColWindow_Fran[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
           for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+2*4)+i, strColWindow_TChinese[i][j]);
                }
	}
	else
	{
           for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+2*4)+i, strColWindow[i][j]);
                }
	}

	//Sub Icon4
   	         g_u8OsdFontColor=FOUR_COLOR(38);
           OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                	 Osd_DrawRealChar(4+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strSubSysWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();

	//Sub Text4
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);

	if(USER_PREF_LANGUAGE==LANG_Spanish)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strSysWindow_Span[i][j]);
                }
	 }
	else if(USER_PREF_LANGUAGE==LANG_French)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<13; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strSysWindow_Fran[i][j]);
                }
	 }
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strSysWindow_TChinese[i][j]);
                }
	 }
	else
	 {	
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strSYSWindow[i][j]);
                }	
	 }
 ////////////////////////////////////////////////////

 ////////////////////////////////////////////////////
				
	}
	else if(g_u8MenuPageIndex==MENU_SYSTEM
  #if LED_HOT_Menu_Func
	||g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER
  #endif
	)
	{

	//Sub Icon2
              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                   if(strSubPicWindow[i][j]==SubPic(0)||strSubPicWindow[i][j]==SubPic(2)||strSubPicWindow[i][j]==SuEPic(0)||strSubPicWindow[i][j]==SuEPic(2))
		       g_u8OsdFontColor=FOUR_COLOR(24);	
		     else// if(strSubPicWindow[i][j]==SubPic(1)||strSubPicWindow[i][j]==SubPic(3)||strSubPicWindow[i][j]==SuEPic(1)||strSubPicWindow[i][j]==SuEPic(3))	
		       g_u8OsdFontColor=FOUR_COLOR(28);
                	 Osd_DrawRealChar(4+j, (8+1*4)+i, strSubPicWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();
	
 	//Sub Text2
               g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);	
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+1*4)+i, strPicWindow_Span[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
              for (i=0; i<2; i++)
                for(j=0; j<9; j++)
                {
                	 Osd_DrawRealChar(14+j, (8+1*4)+i, strPicWindow_Fran[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+1*4)+i, strPicWindow_TChinese[i][j]);
                }	
	}
	else
	{	
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+1*4)+i, strPicWindow[i][j]);
                }	
	}

	//Sub Icon3
              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                   if(strSubColWindow[i][j]==SubCol(5))
		       g_u8OsdFontColor=FOUR_COLOR(34);	
	 	     else if(strSubColWindow[i][j]==SubCol(6))		   
		       g_u8OsdFontColor=FOUR_COLOR(36);	
		     else 
		       g_u8OsdFontColor=FOUR_COLOR(32);
                	 Osd_DrawRealChar(4+j, (8+2*4)+i, strSubColWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();
				
	//Sub Text3
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
           for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+2*4)+i, strColWindow_Span[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
           for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+2*4)+i, strColWindow_Fran[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
           for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+2*4)+i, strColWindow_TChinese[i][j]);
                }
	}
	else
	{
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+2*4)+i, strColWindow[i][j]);
                }	
	}
				

	//Sub Icon4
   	         g_u8OsdFontColor=FOUR_COLOR(39);
           OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                	 Osd_DrawRealChar(4+j, (8+3*4)+i, strSubSysWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();


	//Sub Text4
                 g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_SubGround);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+3*4)+i, strSysWindow_Span[i][j]);
                }	
	 }		
	else if(USER_PREF_LANGUAGE==LANG_French)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<13; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+3*4)+i, strSysWindow_Fran[i][j]);
                }	
	 }		
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+3*4)+i, strSysWindow_TChinese[i][j]);
                }	
	 }		
	else
	 {
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+3*4)+i, strSYSWindow[i][j]);
                }	
	 }	
	
	}
	else
	{
	//Sub Icon2
              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                   if(strSubPicWindow[i][j]==SubPic(0)||strSubPicWindow[i][j]==SubPic(2)||strSubPicWindow[i][j]==SuEPic(0)||strSubPicWindow[i][j]==SuEPic(2))
		       g_u8OsdFontColor=FOUR_COLOR(24);	
		     else// if(strSubPicWindow[i][j]==SubPic(1)||strSubPicWindow[i][j]==SubPic(3)||strSubPicWindow[i][j]==SuEPic(1)||strSubPicWindow[i][j]==SuEPic(3))	
		       g_u8OsdFontColor=FOUR_COLOR(28);
                	 Osd_DrawRealChar(4+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strSubPicWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();
	
	//Sub Text2
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strPicWindow_Span[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
              for (i=0; i<2; i++)
                for(j=0; j<9; j++)
                {
                	 Osd_DrawRealChar(14+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strPicWindow_Fran[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strPicWindow_TChinese[i][j]);
                }
	}
	else
	{
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+0*4)+i, strPicWindow[i][j]);
                }
	}

	//Sub Icon3
              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                   if(strSubColWindow[i][j]==SubCol(5))
		       g_u8OsdFontColor=FOUR_COLOR(34);	
	 	     else if(strSubColWindow[i][j]==SubCol(6))		   
		       g_u8OsdFontColor=FOUR_COLOR(36);	
		     else 
		       g_u8OsdFontColor=FOUR_COLOR(32);
                	 Osd_DrawRealChar(4+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strSubColWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();
				
	//Sub Text3
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strColWindow_Span[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strColWindow_Fran[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strColWindow_TChinese[i][j]);
                }
	}
	else
	{
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+1*4)+i, strColWindow[i][j]);
                }
	}


	//Sub Icon4
   	         g_u8OsdFontColor=FOUR_COLOR(38);
           OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<2; i++)
                for(j=0; j<4; j++)
                {
                	 Osd_DrawRealChar(4+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+2*4)+i, strSubSysWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();

	//Sub Text4
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	   {
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+2*4)+i, strSysWindow_Span[i][j]);
                }	
	   }
	else if(USER_PREF_LANGUAGE==LANG_French)
	   {
              for (i=0; i<2; i++)
                for(j=0; j<13; j++)
                {
                	 Osd_DrawRealChar(12+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+2*4)+i, strSysWindow_Fran[i][j]);
                }	
	   }
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	   {
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+2*4)+i, strSysWindow_TChinese[i][j]);
                }	
	   }
	else
	   {
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, ((Sub_MidLine_YPos+1+(g_u8MenuPageIndex-MENU_QUICKSTART)*4)+1+2*4)+i, strSYSWindow[i][j]);
                }	
	   }

	}

	
         }
		 
    
    }

	
#if Enable_Corss_LHCX_New
    	
	else if(g_u8MenuPageIndex==HotKeyGameModeMenu
		||g_u8MenuPageIndex==HotKeyGameSelectMenu
		||g_u8MenuPageIndex==HotKeyGameSubSelectMenu
		||g_u8MenuPageIndex==HotKeyGameClokMenu
		||g_u8MenuPageIndex==HotKeyGameClokShowMenu
			)		
	{
	
		if(g_u8MenuPageIndex==HotKeyGameClokShowMenu)
		g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_BK_COLOR_H);
		else
		g_u8OsdFontColor=MONO_COLOR(CP_SL_COLOR_L1,CP_BK_COLOR_H);

           for(i=0; i<GET_MENU_YSIZE(CURRENT_MENU); i++)
           {
              Osd_DrawContinuousChar(0, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));
           }
	    if(g_u8MenuPageIndex==HotKeyGameClokShowMenu)	   
             DrawHotKeyGameClokShow();
	}
	else if(g_u8MenuPageIndex==HotKeyGameClokPositionMenu)
	{
		g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BK_COLOR_H);
		for(j=10;j<50;j++)
		{
			OSD_TEXT_HI_ADDR_SET_BIT8();
			Osd_DrawRealChar(j,13,HotGameFrameFONT_2COLOR_ICON_START+8);
			OSD_TEXT_HI_ADDR_CLR_TO_0();
		}
	}
   
#endif
    else if(!IS_MAIN_L1(g_u8MenuPageIndex))    //Message Menu
    {
        g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L0,CP_BK_COLOR_L0);
        Osd_DrawBlankPlane(0,0,g_u8OsdWindowWidth,g_u8OsdWindowHeight);
    }
}


void DrawColorMenuFrame(BYTE SelectEnable)
{
{
	BYTE i;
       SelectEnable=SelectEnable;

  //Clear System Enter Advanced
           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
           
           for (i=ColorAdvancedTxtYPos; i<(ColorAdvancedTxtYPos+12); i++)
           {
            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
           }	

           g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_BK_COLOR_H);
	OSD_TEXT_HI_ADDR_SET_BIT8();		
           for(i=ColorAdvancedTxtYPos; i<(ColorAdvancedTxtYPos+12); i++)
           {
            Osd_DrawRealChar(0, i, MAIN_Left_Line);
           }
           for(i=ColorAdvancedTxtYPos; i<(ColorAdvancedTxtYPos+12); i++)
           {
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
           }
        OSD_TEXT_HI_ADDR_CLR_TO_0();

    for (i = 0; i < MENU_ITEM_COUNT; i++)
    {   
        if ((CURRENT_MENU.pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
            continue; //Don't draw Adjust items in L2
        if (i!=g_u8MenuItemIndex)
            DrawOsdMenuItem(i, &CURRENT_MENU.pstMenuItems[i]);
    }


}

/*
	BYTE i;

     if(SelectEnable)
           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
     else	 
           g_u8OsdFontColor=MONO_COLOR(CP_Black,CP_Black);
           
           for (i=(ColorUserTxtYPos); i<((ColorUserTxtYPos)+3); i++)
           {
            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
           }	
      if(SelectEnable)
           g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_BK_COLOR_H);
      else	 
           g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Black);
	OSD_TEXT_HI_ADDR_SET_BIT8();		
           for(i=(ColorUserTxtYPos); i<((ColorUserTxtYPos)+3); i++)
           {
            Osd_DrawRealChar(0, i, MAIN_Left_Line);
           }
           for(i=(ColorUserTxtYPos); i<((ColorUserTxtYPos)+3); i++)
           {
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
           }
        OSD_TEXT_HI_ADDR_CLR_TO_0();

     if(CURRENT_COLOR_TEMP==PCT_Advanced)
           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
     else	 
           g_u8OsdFontColor=MONO_COLOR(CP_Black,CP_Black);
           
           for (i=(ColorUserTxtYPos+3); i<((ColorUserTxtYPos)+6); i++)
           {
            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
           }	
      if(CURRENT_COLOR_TEMP==PCT_Advanced)
           g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_BK_COLOR_H);
      else	 
           g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Black);
	OSD_TEXT_HI_ADDR_SET_BIT8();		
           for(i=(ColorUserTxtYPos+3); i<((ColorUserTxtYPos)+6); i++)
           {
            Osd_DrawRealChar(0, i, MAIN_Left_Line);
           }
           for(i=(ColorUserTxtYPos+3); i<((ColorUserTxtYPos)+6); i++)
           {
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
           }
        OSD_TEXT_HI_ADDR_CLR_TO_0();
*/
}


void DrawSystemMenuFrame(BYTE Select)
{
	BYTE i;
       Select=Select;

  //Clear System Enter LedModeMenu
           g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_BK_COLOR_H);
           
           for (i=(SystemLedModeTxtYPos); i<((SystemLedModeTxtYPos)+12); i++)
           {
            Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
           }	

           g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_BK_COLOR_H);
	OSD_TEXT_HI_ADDR_SET_BIT8();		
           for(i=(SystemLedModeTxtYPos); i<((SystemLedModeTxtYPos)+12); i++)
           {
            Osd_DrawRealChar(0, i, MAIN_Left_Line);
           }
           for(i=(SystemLedModeTxtYPos); i<((SystemLedModeTxtYPos)+12); i++)
           {
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
           }
        OSD_TEXT_HI_ADDR_CLR_TO_0();

    for (i = Sys_Language_IDX; i < MENU_ITEM_COUNT; i++)
    {   
        if ((CURRENT_MENU.pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
            continue; //Don't draw Adjust items in L2
        if (i!=g_u8MenuItemIndex)
            DrawOsdMenuItem(i, &CURRENT_MENU.pstMenuItems[i]);
    }


}

#if LED_HOT_Menu_Func
void DrawLedMenuItems(void)
{
  BYTE u8TempValue, i;

     u8TempValue=g_u8MenuItemIndex;
g_u8MenuItemIndex=0xF0;		 
  for (i = 0; i < tblMenus[MENU_LEDMODE].u8MenuItemCount; i++)
  {   
      if ((tblMenus[MENU_LEDMODE].pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
          continue; //Don't draw Adjust items in L2
      if (i!=g_u8MenuItemIndex)
          DrawOsdMenuItem(i, &tblMenus[MENU_LEDMODE].pstMenuItems[i]);
  }
g_u8MenuItemIndex=u8TempValue;		 


}
#endif

void DrawGuideText(void)
{
    DrawOsdMenuItemRadioGroup(0, DrawMainMenuRadioGroup);
}
void LoadPercentPropFont(void);
void DrawPerectnText(void)
{
    DrawOsdMenuItemRadioGroup(0, DrawMainMenuRadioGroup);
}


#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
void SetMultiOSDWin(void)
{
	BYTE HPos=0,VPos=0;
	WORD PosTemp=0;
	BYTE xdata k = 0;
	
	for(InputStatusWinNo= 0;InputStatusWinNo < DISPLAY_MAX_NUMS;InputStatusWinNo++)
	{

		if(g_stMenuFlag.bDisplayInputMsg[InputStatusWinNo] && g_u8MenuPageIndex == MENU_INPUT)
		{			
			if(InputStatusWinNo ==DISPLAY_WIN2)
			{
				OSD_SET_WND(OSD_SUB2_WND);
			}
			else	
			{
				OSD_SET_WND(InputStatusWinNo+2);
			}
			
			if(InputStatusWinNo == DISPLAY_WIN1)
				Osd_SetMultiWindowSize(GET_MENU_XSIZE(CURRENT_MENU)-3, GET_MENU_YSIZE(CURRENT_MENU)-2,OSD_BUTTON_WND, OSD_MSG1_WND_CA_BASE);
			else if(InputStatusWinNo == DISPLAY_WIN2)
				Osd_SetMultiWindowSize(GET_MENU_XSIZE(CURRENT_MENU)-3, GET_MENU_YSIZE(CURRENT_MENU)-2,OSD_SUB2_WND, OSD_MSG2_WND_CA_BASE);							
			else if(InputStatusWinNo == DISPLAY_WIN3)
				Osd_SetMultiWindowSize(GET_MENU_XSIZE(CURRENT_MENU)-3, GET_MENU_YSIZE(CURRENT_MENU)-2,OSD_ICON_WND, OSD_MSG3_WND_CA_BASE);								
			else if(InputStatusWinNo == DISPLAY_WIN4)	
				Osd_SetMultiWindowSize(GET_MENU_XSIZE(CURRENT_MENU)-3, GET_MENU_YSIZE(CURRENT_MENU)-2,OSD_SUB_WND, OSD_MSG4_WND_CA_BASE);							

			printData("bDisplayInputMsg == %d",InputStatusWinNo);

			printData("g_stMenuFlag.bDisplayInputNoSignalMsg == %d",g_stMenuFlag.bDisplayInputNoSignalMsg[InputStatusWinNo]);
			
			switch(USER_PREF_MULTI_WIN_MODE)
			{
				case eMULTIWIN_2_PIP:
					if(InputStatusWinNo == DISPLAY_WIN1)
						Osd_SetMultiPosition(50,50,OSD_BUTTON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN2)
					{
						  //printData("USER_PREF_PIP_POSITION==%d",USER_PREF_PIP_POSITION);
                                   #if Enable_Panel_Rotate_180
 						PosTemp = (USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2)*100)/PANEL_WIDTH;
						//printData("999999999999==%d",PosTemp);		   
						if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_SMALL)
						{
						  //printData("ePIPSIZE_SMALL_H_POS==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 HPos = 15;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 HPos = 85;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
                                                 HPos = 15;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                                                 HPos = 85;
						   else
                                                 HPos = (BYTE) (PosTemp+24);
						}
						else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_MIDDLE)
						{
						  //printData("ePIPSIZE_MIDDLE_H_POS==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 HPos = 19;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 HPos = 82;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
							HPos = 19;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                                                 HPos = 82;
						   else
                                                 HPos = (BYTE) (PosTemp+30);
						}
						else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_LARGE)		
						{
						  //printData("ePIPSIZE_LARGE_H_POS==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 HPos = 21;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 HPos = 80;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
							HPos = 21;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                                                 HPos = 80;
						   else
                                                 HPos = (BYTE) (PosTemp+35);
						}
//////////////////////////////////////////////////////////////////////////////////////////////////						
						PosTemp = (USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2)*100)/PANEL_HEIGHT;
                                          printData("000000000000000000000==%d",PosTemp);		
						if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_SMALL)
						{
						  //printData("ePIPSIZE_SMALL VPos==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 VPos = 84;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 VPos = 84;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
                                                 VPos = 15;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                                                 VPos = 15;
						   else
                                                 VPos = (BYTE) (PosTemp+2);

						}
						else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_MIDDLE)
						{
						  //printData("ePIPSIZE_MIDDLE VPos==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 VPos = 80;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 VPos = 80;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
							VPos = 19;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
							VPos = 19;
						   else
                                                 VPos = (BYTE) (PosTemp+2);
						}
						else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_LARGE)			
						{
						  //printData("ePIPSIZE_LARGE VPos==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 VPos = 79;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 VPos = 79;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
							VPos = 21;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
							VPos = 21;
						   else
                                                 VPos = (BYTE) (PosTemp+2);
						}
						//VPos = (BYTE) (PosTemp*118/100);	
						 //printData("HPos HPos==%d",HPos);
						 //printData("VPos VPos==%d",VPos);
						Osd_SetMultiPosition(HPos,VPos,OSD_SUB2_WND);

                                 #else//Right Top
 						PosTemp = (USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2)*100)/PANEL_WIDTH;
						//printData("999999999999==%d",PosTemp);		   
						if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_SMALL)
						{
						  //printData("ePIPSIZE_SMALL_H_POS==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 HPos = 85;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 HPos = 15;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
                                                 HPos = 85;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                                                 HPos = 15;
						   else
                                                 HPos = (BYTE) (PosTemp+24);
						}
						else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_MIDDLE)
						{
						  //printData("ePIPSIZE_MIDDLE_H_POS==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 HPos = 82;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 HPos = 19;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
							HPos = 82;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                                                 HPos = 19;
						   else
                                                 HPos = (BYTE) (PosTemp+30);
						}
						else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_LARGE)		
						{
						  //printData("ePIPSIZE_LARGE_H_POS==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 HPos = 80;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 HPos = 21;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
							HPos = 80;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                                                 HPos = 21;
						   else
                                                 HPos = (BYTE) (PosTemp+35);
						}
//////////////////////////////////////////////////////////////////////////////////////////////////						
						PosTemp = (USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2)*100)/PANEL_HEIGHT;
                                          printData("000000000000000000000==%d",PosTemp);		
						if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_SMALL)
						{
						  //printData("ePIPSIZE_SMALL VPos==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 VPos = 15;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 VPos = 15;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
                                                 VPos = 84;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
                                                 VPos = 84;
						   else
                                                 VPos = (BYTE) (PosTemp+2);

						}
						else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_MIDDLE)
						{
						  //printData("ePIPSIZE_MIDDLE VPos==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 VPos = 19;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 VPos = 19;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
							VPos = 80;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
							VPos = 80;
						   else
                                                 VPos = (BYTE) (PosTemp+2);
						}
						else if(USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) == ePIPSIZE_LARGE)			
						{
						  //printData("ePIPSIZE_LARGE VPos==%d",PosTemp);
						   if(USER_PREF_PIP_POSITION==PIPPosition_Top_Right)
                                                 VPos = 21;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
                                                 VPos = 21;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Right)
							VPos = 79;
						   else if(USER_PREF_PIP_POSITION==PIPPosition_Bottom_Left)
							VPos = 79;
						   else
                                                 VPos = (BYTE) (PosTemp+2);
						}
						//VPos = (BYTE) (PosTemp*118/100);	
						 //printData("HPos HPos==%d",HPos);
						 //printData("VPos VPos==%d",VPos);
						Osd_SetMultiPosition(HPos,VPos,OSD_SUB2_WND);

                            #endif
						
					}
														
				break;
				case eMULTIWIN_2_POP1:
				#if Enable_Panel_Rotate_180
					if(InputStatusWinNo == DISPLAY_WIN1)
						Osd_SetMultiPosition(78,50,OSD_BUTTON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN2)
						Osd_SetMultiPosition(24,50,OSD_SUB2_WND);
				#else
					if(InputStatusWinNo == DISPLAY_WIN1)
						Osd_SetMultiPosition(24,50,OSD_BUTTON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN2)
						Osd_SetMultiPosition(78,50,OSD_SUB2_WND);
				#endif	
				break;
				case eMULTIWIN_3:
				#if Enable_Panel_Rotate_180
					if(InputStatusWinNo == DISPLAY_WIN1)
						Osd_SetMultiPosition(78,50,OSD_BUTTON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN2)
						Osd_SetMultiPosition(24,75,OSD_SUB2_WND);	
					else if(InputStatusWinNo == DISPLAY_WIN3)
						Osd_SetMultiPosition(24,25,OSD_ICON_WND);	
				#else
					if(InputStatusWinNo == DISPLAY_WIN1)
						Osd_SetMultiPosition(24,50,OSD_BUTTON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN2)
						Osd_SetMultiPosition(78,25,OSD_SUB2_WND);	
					else if(InputStatusWinNo == DISPLAY_WIN3)
						Osd_SetMultiPosition(77,75,OSD_ICON_WND);	
				#endif	
				break;
				case eMULTIWIN_4:
				#if Enable_Panel_Rotate_180
					if(InputStatusWinNo == DISPLAY_WIN1)
						Osd_SetMultiPosition(78,25,OSD_BUTTON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN2)
						Osd_SetMultiPosition(24,25,OSD_SUB2_WND);	
					else if(InputStatusWinNo == DISPLAY_WIN3)
						Osd_SetMultiPosition(78,75,OSD_ICON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN4)	
						Osd_SetMultiPosition(24,75,OSD_SUB_WND);		
				#else
					if(InputStatusWinNo == DISPLAY_WIN1)
						Osd_SetMultiPosition(24,25,OSD_BUTTON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN2)
						Osd_SetMultiPosition(78,25,OSD_SUB2_WND);	
					else if(InputStatusWinNo == DISPLAY_WIN3)
						Osd_SetMultiPosition(24,75,OSD_ICON_WND);
					else if(InputStatusWinNo == DISPLAY_WIN4)	
						Osd_SetMultiPosition(78,75,OSD_SUB_WND);		
				#endif	
				break;	
				case eMULTIWIN_OFF:
				break;
			}	
			//if(g_stMenuFlag.bDisplayInputMsg[InputStatusWinNo] && g_u8MenuPageIndex == MENU_INPUT)
				//g_stMenuFlag.bDisplayInputMsg[InputStatusWinNo]=0;
		
			DrawMessageFrame();		
			Delay1ms(25);
			for( k = 0; k < MENU_ITEM_COUNT; k++ )
			{
				DrawOsdMenuItem( k, &CURRENT_MENU.pstMenuItems[k] );
			}

		}
	}
}
#endif

void ClrOSDSetting(BYTE u8Win,BYTE u8Atr)
{
	drvOSD_SetWndFixColor(u8Win,0,0x7d,0xa5);//PIP PBP info background color
	drvOSD_SetWndFixAttr(u8Win,0,u8Atr);
}

void MultiOSDWin(void)
{
	BYTE uc;

	Menu_Hide();	
	
	//ClrOSDSetting(OSD_SUB2_WND,0x02);
	//ClrOSDSetting(OSD_ICON_WND,0x03);	
	//ClrOSDSetting(OSD_BUTTON_WND,0x04);
	//ClrOSDSetting(OSD_SUB_WND,0x06);	

       //OSD_SET_WND(OSD_MAIN_WND);
#if ENABLE_OSD_ROTATION
#if (defined(LianHeChuangXin_Project))
  if(g_u8MenuPageIndex==MENU_KEY_GUIDE)
  {
        msWriteByteMask(OSD1_03, 0, BIT7|BIT6);
  }
 else	   
#endif
  {
 #if Enable_Panel_Rotate_180
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
       	msWriteByteMask(OSD1_03, BIT6, BIT7|BIT6);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)	
        msWriteByteMask(OSD1_03, 0, BIT7|BIT6);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_270)	
   	      msWriteByteMask(OSD1_03, BIT7|BIT6, BIT7|BIT6);
   else   	
        msWriteByteMask(OSD1_03, BIT7, BIT7|BIT6);
 #else
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
        msWriteByteMask(OSD1_03, BIT7|BIT6, BIT7|BIT6);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)	
        msWriteByteMask(OSD1_03, BIT7, BIT7|BIT6);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_270)	
        msWriteByteMask(OSD1_03, BIT6, BIT7|BIT6);
   else
        msWriteByteMask(OSD1_03, 0, BIT7|BIT6);
  #endif 
  }
#endif


    if (CURRENT_MENU.LoadFont!=NULL)
        CURRENT_MENU.LoadFont();

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	SetMultiOSDWin();
#endif	
	Menu_Show();	
	OSD_SET_WND(OSD_MAIN_WND);
	for(uc = 0;uc<4;uc++)
	{
		g_stMenuFlag.bDisplayInputMsg[uc] = 0;
 	}

	InputStatusWinNo = 0;
	//PrevPage = g_u8MenuPageIndex;
	//ExitSubItemIndex = 0;
	//ExitStatusItemIndex = 0;	
}



void SetMenuPositionNormal(void)
{
    Osd_SetPosition( USER_PREF_OSD_H_POSITION, USER_PREF_OSD_V_POSITION );

    //Osd_MenuHPosition(90,TRUE);
    //Osd_MenuVPosition(90,TRUE);
}

#if ENABLE_OSD_ROTATION
void SetMenuPositionRotated(void)
{
    //Osd_MenuHPosition(90,TRUE);
    //Osd_MenuVPosition(90,TRUE);
#if (defined(LianHeChuangXin_Project))
  if(g_u8MenuPageIndex==MENU_KEY_GUIDE)
  {
    Osd_SetPosition( USER_PREF_OSD_H_POSITION, USER_PREF_OSD_V_POSITION );
  }
 else	   
#endif
    Osd_SetPosition( USER_PREF_OSD_H_POSITION, USER_PREF_OSD_V_POSITION );
}

void MenuRotate(Bool Rotate)
{
    Delay1ms(50);
    WAIT_V_OUTPUT_BLANKING();
    Menu_Hide();

    Rotate=Rotate;
}
#endif
#if ENABLE_WIDE_MODE //Jison 080430
void ResetMenuPosition(void)
{
    Menu_Hide();
  #if ENABLE_OSD_ROTATION
    if(USER_PREF_OSD_ROTATION!=Osd_Rotation_Normal)
        SetMenuPositionRotated();
    else
  #endif
        SetMenuPositionNormal();
    Menu_Show();
}
#endif

Bool IsItemGrayed(WORD u16Flags, BYTE ItemIndex)
{

ItemIndex=ItemIndex;
//printData("333333 IsItemGrayed==%d",ItemIndex);

#if ENABLE_DIGITAL_INPUT
    if ((u16Flags & MIB_DVI_DISABLE))
    {
	if(CURRENT_INPUT_IS_VGA())
        return FALSE;
	else
        return TRUE;
    }
#endif

   if((CURRENT_COLOR_TEMP!=PCT_USER)/*&&(CURRENT_COLOR_TEMP!=PCT_Advanced)*/&&(u16Flags&MIB_ColorTemp_DISABLE)&&(g_u8MenuPageIndex==MENU_COLOR))
   	return TRUE;

#if Free_Sync_Enable
    if ((u16Flags & MIB_FREESYNC_DISABLE))
    {
     if((INPUT_IS_DISPLAYPORT(MapWin2Port(USER_PREF_WIN_SEL))||(INPUT_IS_HDMI_2_0(MapWin2Port(USER_PREF_WIN_SEL)))
   #if EDID_1920x1080_144
        ||(INPUT_IS_HDMI(MapWin2Port(USER_PREF_WIN_SEL)))
   #endif     
	 )
   #if ENABLE_MULTI_WINDOW_SETTINGS_MENU		
      &&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
   #endif   
	 )
        return FALSE;
	else
        return TRUE;
    }
#endif

#if 1
   if ((u16Flags & MIB_BRIGHTNESS_DISABLE))
   {
       if((CURRENT_PICTURE_MODE==CT_USER)&&(!DCR_ENABLE_FLAG))
         return FALSE;
	else   
	  return TRUE;
   }
#endif

#if 1
   if ((u16Flags & MIB_DCR_ENABLE))
   {
       if((CURRENT_PICTURE_MODE==CT_STANDARD)||(CURRENT_PICTURE_MODE==CT_USER)/*||(CURRENT_PICTURE_MODE==CT_VIVID)*/)
         return FALSE;
	else   
	  return TRUE;
   }
#endif

#if 1
   if ((u16Flags & MIB_USER_DISABLE))
   {
       if((CURRENT_COLOR_TEMP!=PCT_USER))
         return FALSE;
	else   
	  return TRUE;
   }
#endif


#if 0//ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if ((u16Flags&MIB_PIP_PBP_DISABLE) && (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF))
        return TRUE;
#endif
	


#if 0//((INPUT_TYPE==INPUT_1A) && !ENABLE_DCR && !ENABLE_AUDIO_SETTINGS_MENU)
    u16Flags=u16Flags;
#endif
    return FALSE;
}

Bool IsItemHidden(WORD u16Flags)
{
    if ((u16Flags & MIB_FACTORY_ITEM) && !FACTORY_MODE_FLAG)
        return TRUE;

#if 0//defined(_HIDE_GRAYOUT_ITEM_)
    if (IsItemGrayed(u16Flags))
        return TRUE;
#endif
    return FALSE;
}


Bool IsOptionGrayed(BYTE u8Option)
{

//        return TRUE;
UNUSED(u8Option);

  

    return FALSE;
}



BYTE GetHighLightStatusIndex( BYTE InputType )
{
  #if Enable_AutoInput_Menu

     if(InputType==INPUT_PRIORITY_AUTO)
	return SOURCE_AUTO_SWITCH_IDX;

#if ENABLE_VGA_INPUT
	else if (InputType == INPUT_PRIORITY_VGA)
	return SOURCE_VGA_IDX;
#else
      if(0);
#endif	

   #if ENABLE_DVI_INPUT
	else if (InputType == INPUT_PRIORITY_DVI)
	return SOURCE_DVI_IDX;
      #if ENABLE_DVI2ND_INPUT
	else if (InputType == INPUT_PRIORITY_DVI2ND)
	return SOURCE_DVI2_IDX;
      #endif	
       #if ENABLE_DVI3ND_INPUT
	else if (InputType == INPUT_PRIORITY_DVI3ND)
	return SOURCE_DVI3_IDX;
      #endif	
  #endif
  
  #if ENABLE_HDMI_INPUT
	else if (InputType == INPUT_PRIORITY_HDMI)
	return SOURCE_HDMI1_IDX;
 #if ENABLE_HDMI2ND_INPUT
	else if (InputType == INPUT_PRIORITY_HDMI2ND)
	return SOURCE_HDMI2_IDX;
 #endif	
 #if ENABLE_HDMI3ND_INPUT
	else if (InputType == INPUT_PRIORITY_HDMI3ND)
	return SOURCE_HDMI3_IDX;
 #endif	
 #endif
 
	else if (InputType == INPUT_PRIORITY_DP)
	return SOURCE_Displayport1_IDX;
#if ENABLE_DP2ND_INPUT	
	else if (InputType == INPUT_PRIORITY_DP2ND)
	return SOURCE_Displayport2_IDX;	
#endif


#else


#if ENABLE_VGA_INPUT
	if (InputType == INPUT_VGA)
	return SOURCE_VGA_IDX;
#else
      if(0);
#endif	

   #if ENABLE_DVI_INPUT
	else if (InputType == INPUT_DVI)
	return SOURCE_DVI_IDX;
      #if ENABLE_DVI2ND_INPUT
	else if (InputType == INPUT_DVI2ND)
	return SOURCE_DVI2_IDX;
      #endif	
       #if ENABLE_DVI3ND_INPUT
	else if (InputType == INPUT_DVI3ND)
	return SOURCE_DVI3_IDX;
      #endif	
  #endif
  
  #if ENABLE_HDMI_INPUT
	else if (InputType == INPUT_HDMI)
	return SOURCE_HDMI1_IDX;
 #if ENABLE_HDMI2ND_INPUT
	else if (InputType == INPUT_HDMI2ND)
	return SOURCE_HDMI2_IDX;
 #endif	
 #if ENABLE_HDMI3ND_INPUT
	else if (InputType == INPUT_HDMI3ND)
	return SOURCE_HDMI3_IDX;
 #endif	

 #endif
 #if ENABLE_DP_INPUT
	else if (InputType == INPUT_DISPLAYPORT)
	return SOURCE_Displayport1_IDX;
  #if ENABLE_DP2ND_INPUT	
	else if (InputType == INPUT_DISPLAYPORT2ND)
	return SOURCE_Displayport2_IDX;	
  #endif
  #endif
#endif

      return 0xFF;

}

#if ENABLE_AUDIO_SETTINGS_MENU
#define MuM(x)   (LOBYTE(HotKey_ICON_Addr)+x)

 BYTE code strMutWindow[6][6]=
{
	{	 MuM(0x00), MuM(0x01), MuM(0x02), MuM(0x03), MuM(0x04), 
	 0},
	{	 MuM(0x05), MuM(0x06), MuM(0x07), MuM(0x08), MuM(0x09), 
	 0},
	{	 MuM(0x0A), MuM(0x0B), MuM(0x0C), MuM(0x0D), MuM(0x0E), 
	 0},
	{	 MuM(0x0F), MuM(0x10), MuM(0x11), MuM(0x12), MuM(0x13), 
	 0},
	{	 MuM(0x14), MuM(0x15), MuM(0x16), MuM(0x17), MuM(0x18), 
	 0},
	{	 MuM(0x19), MuM(0x1A), MuM(0x1B), MuM(0x1C), MuM(0x1D), 
	 0}
};
#endif




#if ENABLE_AUDIO_SETTINGS_MENU
void Osd_DrawMuteFont(void)
{
   BYTE ItemIndex,i;
   

      g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BK_COLOR_L1);

 if(AUDIO_MUTE_FLAG)    
 {
     for(ItemIndex=3; ItemIndex<6; ItemIndex++)
       for(i=0; i<5; i++)
        Osd_DrawRealChar( 10+i, ItemIndex, strMutWindow[ItemIndex][i]);

 }
 else
 {
     for(ItemIndex=0; ItemIndex<3; ItemIndex++)
       for(i=0; i<5; i++)
        Osd_DrawRealChar( 10+i, 3+ItemIndex, strMutWindow[ItemIndex][i]);

 }
  
}
#endif

#if New_Cross_Enable
#define CrM(x)        (LOBYTE(HotKey_ICON_Addr)+x)
 BYTE code strCroWindow_L[6][11]=
{
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x01), CrM(0x02), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x03), CrM(0x04), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x05), CrM(0x06), CrM(0x05), CrM(0x07), CrM(0x08), CrM(0x09), CrM(0x0A), CrM(0x0B), 
		 CrM(0x0C), CrM(0x05), 
	 0},
	{	 CrM(0x0D), CrM(0x0E), CrM(0x0D), CrM(0x0F), CrM(0x10), CrM(0x11), CrM(0x12), CrM(0x13), 
		 CrM(0x14), CrM(0x0D), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x15), CrM(0x16), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x01), CrM(0x02), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0}
};

 BYTE code strCroWindow_M[6][11]=
{
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x01), CrM(0x02), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x03), CrM(0x04), CrM(0x05), CrM(0x06), CrM(0x07), CrM(0x08), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x09), CrM(0x0A), CrM(0x0B), CrM(0x0C), CrM(0x0D), CrM(0x0E), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x0F), CrM(0x10), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0}
};

 BYTE code strCroWindow_S[6][11]=
{
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x01), CrM(0x02), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x03), CrM(0x04), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
		 CrM(0x00), CrM(0x00), 
	 0}
};

 
void Osd_DrawCrossFont(void)
{

   BYTE ItemIndex,i;
   
   if(UserPrefCrossType==Cross_Large_Y||UserPrefCrossType==Cross_Middle_y||UserPrefCrossType==Cross_Small_Y)
      g_u8OsdFontColor=MONO_COLOR(CP_YELLOW, CP_CLEAR);
   else 
      g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_CLEAR);
OSD_TEXT_HI_ADDR_SET_BIT8();   	
         for(ItemIndex=0; ItemIndex<6; ItemIndex++)
           for(i=0; i<10; i++)
           {
           if(UserPrefCrossType==Cross_Large_Y||UserPrefCrossType==Cross_Large_R)
            Osd_DrawRealChar( i, ItemIndex, strCroWindow_L[ItemIndex][i]);
	    else if(UserPrefCrossType==Cross_Middle_y||UserPrefCrossType==Cross_Middle_R)
            Osd_DrawRealChar( i, ItemIndex, strCroWindow_M[ItemIndex][i]);
	    else 
            Osd_DrawRealChar( i, ItemIndex, strCroWindow_S[ItemIndex][i]);
           }
OSD_TEXT_HI_ADDR_CLR_TO_0();   	
		   
}

#elif Enable_Corss_FY

#define CRM(x)        (LOBYTE(HotKey_ICON_Addr)+x)
 BYTE code strCROWindow_ONE[6][11]=
{
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x01), CRM(0x02), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x03), CRM(0x04), CRM(0x05), CRM(0x06), CRM(0x07), CRM(0x08), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x09), CRM(0x0A), CRM(0x0B), CRM(0x0A), CRM(0x0C), CRM(0x0D), CRM(0x0A), CRM(0x0E), 
		 CRM(0x0A), CRM(0x0F), 
	 0},
	{	 CRM(0x10), CRM(0x11), CRM(0x12), CRM(0x11), CRM(0x13), CRM(0x14), CRM(0x11), CRM(0x15), 
		 CRM(0x11), CRM(0x16), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x17), CRM(0x18), CRM(0x19), CRM(0x1A), CRM(0x1B), CRM(0x1C), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x1D), CRM(0x1E), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0}
};



 BYTE code strCROWindow_TOW[6][11]=
{
	{	 CRM(0x00), CRM(0x01), CRM(0x02), CRM(0x03), CRM(0x04), CRM(0x05), CRM(0x06), CRM(0x07), 
		 CRM(0x08), CRM(0x00), 
	 0},
	{	 CRM(0x09), CRM(0x0A), CRM(0x0B), CRM(0x00), CRM(0x0C), CRM(0x0B), CRM(0x00), CRM(0x0C), 
		 CRM(0x0D), CRM(0x0E), 
	 0},
	{	 CRM(0x0F), CRM(0x10), CRM(0x08), CRM(0x11), CRM(0x12), CRM(0x13), CRM(0x14), CRM(0x01), 
		 CRM(0x15), CRM(0x16), 
	 0},
	{	 CRM(0x17), CRM(0x18), CRM(0x0B), CRM(0x19), CRM(0x1A), CRM(0x1B), CRM(0x1C), CRM(0x0C), 
		 CRM(0x1D), CRM(0x1E), 
	 0},
	{	 CRM(0x1F), CRM(0x20), CRM(0x08), CRM(0x00), CRM(0x01), CRM(0x08), CRM(0x00), CRM(0x01), 
		 CRM(0x21), CRM(0x22), 
	 0},
	{	 CRM(0x00), CRM(0x0C), CRM(0x23), CRM(0x24), CRM(0x25), CRM(0x26), CRM(0x27), CRM(0x28), 
		 CRM(0x0B), CRM(0x00), 
	 0}
};


 BYTE code strCROWindow_THR[6][11]=
{
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x01), CRM(0x02), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x03), CRM(0x04), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x05), CRM(0x06), CRM(0x07), CRM(0x06), CRM(0x08), CRM(0x09), CRM(0x0A), CRM(0x0B), 
		 CRM(0x0A), CRM(0x0C), 
	 0},
	{	 CRM(0x0D), CRM(0x0E), CRM(0x0F), CRM(0x0E), CRM(0x10), CRM(0x11), CRM(0x12), CRM(0x13), 
		 CRM(0x12), CRM(0x14), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x15), CRM(0x16), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x17), CRM(0x18), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0}
};
 
void Osd_DrawCrossFont(void)
{

   BYTE ItemIndex,i;
   
   if(UserPrefCrossType==Cross_Color_Y)
      g_u8OsdFontColor=MONO_COLOR(CP_YELLOW, CP_CLEAR);
   else 
      g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_CLEAR);
OSD_TEXT_HI_ADDR_SET_BIT8();      	
         for(ItemIndex=0; ItemIndex<6; ItemIndex++)
           for(i=0; i<10; i++)
           {
           if(USER_PREF_LOSMODE==Cross_Mode_1)
            Osd_DrawRealChar( i, ItemIndex, strCROWindow_ONE[ItemIndex][i]);
	    else if(USER_PREF_LOSMODE==Cross_Mode_2)
            Osd_DrawRealChar( i, ItemIndex, strCROWindow_TOW[ItemIndex][i]);
	    else 
            Osd_DrawRealChar( i, ItemIndex, strCROWindow_THR[ItemIndex][i]);
           }
OSD_TEXT_HI_ADDR_CLR_TO_0();      	
		   
}

#elif Enable_Corss_LHCX
#define CrM(x)        (LOBYTE(HotKey_ICON_Addr)+x)

 BYTE code strCorWindow_1[6][13]=
{
	{	 CrM(0x00), CrM(0x00), CrM(0x01), CrM(0x02), CrM(0x03), CrM(0x04), CrM(0x05), CrM(0x06), 
		 CrM(0x07), CrM(0x08), CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x09), CrM(0x0A), CrM(0x0B), CrM(0x0C), CrM(0x0D), CrM(0x0E), CrM(0x0F), 
		 CrM(0x10), CrM(0x11), CrM(0x12), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x13), CrM(0x14), CrM(0x15), CrM(0x16), CrM(0x17), CrM(0x18), CrM(0x19), 
		 CrM(0x1A), CrM(0x14), CrM(0x1B), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x1C), CrM(0x1D), CrM(0x1E), CrM(0x1F), CrM(0x20), CrM(0x21), CrM(0x22), 
		 CrM(0x23), CrM(0x1D), CrM(0x24), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x25), CrM(0x26), CrM(0x27), CrM(0x28), CrM(0x29), CrM(0x2A), CrM(0x2B), 
		 CrM(0x2C), CrM(0x2D), CrM(0x2E), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x2F), CrM(0x30), CrM(0x31), CrM(0x32), CrM(0x33), CrM(0x34), 
		 CrM(0x35), CrM(0x36), CrM(0x00), CrM(0x00), 
	 0}
};

 BYTE code strCorWindow_2[6][13]=
{
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x01), CrM(0x02), CrM(0x00), 
		 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x03), CrM(0x04), CrM(0x05), CrM(0x06), 
		 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x07), CrM(0x08), CrM(0x09), CrM(0x0A), CrM(0x0B), CrM(0x0C), CrM(0x0D), 
		 CrM(0x0E), CrM(0x0F), CrM(0x10), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x11), CrM(0x12), CrM(0x13), CrM(0x14), CrM(0x15), CrM(0x16), CrM(0x17), 
		 CrM(0x18), CrM(0x19), CrM(0x1A), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x1B), CrM(0x1C), CrM(0x1D), CrM(0x1E), 
		 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
	 0},
	{	 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x1F), CrM(0x20), CrM(0x00), 
		 CrM(0x00), CrM(0x00), CrM(0x00), CrM(0x00), 
	 0}
};

 
void Osd_DrawCrossFont(void)
{

   BYTE ItemIndex,i;
   
   if(UserPrefCrossType==Cross_Mode1_G||UserPrefCrossType==Cross_Mode2_G)
      g_u8OsdFontColor=MONO_COLOR(CP_YELLOW, CP_CLEAR);
   else 
      g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_CLEAR);
   	
OSD_TEXT_HI_ADDR_SET_BIT8();      	
         for(ItemIndex=0; ItemIndex<6; ItemIndex++)
           for(i=0; i<12; i++)
           {
           if(UserPrefCrossType==Cross_Mode1_G||UserPrefCrossType==Cross_Mode1_R)
            Osd_DrawRealChar( i, ItemIndex, strCorWindow_1[ItemIndex][i]);
	    else 
            Osd_DrawRealChar( i, ItemIndex, strCorWindow_2[ItemIndex][i]);
           }
OSD_TEXT_HI_ADDR_CLR_TO_0();      	
}

#elif Enable_Corss_RunFeng
#define CRM(x)        (LOBYTE(HotKey_ICON_Addr)+x)


 BYTE code strCROWindow[6][11]=
{
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x01), CRM(0x02), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x03), CRM(0x04), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x05), CRM(0x06), CRM(0x07), CRM(0x08), CRM(0x09), CRM(0x0A), CRM(0x05), CRM(0x06), 
		 CRM(0x07), CRM(0x08), 
	 0},
	{	 CRM(0x0B), CRM(0x0C), CRM(0x0D), CRM(0x0E), CRM(0x0F), CRM(0x10), CRM(0x0B), CRM(0x0C), 
		 CRM(0x0D), CRM(0x0E), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x11), CRM(0x12), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x13), CRM(0x14), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0}
};


 BYTE code strCROWindow_2[6][11]=
{
	{	 CRM(0x00), CRM(0x01), CRM(0x02), CRM(0x03), CRM(0x04), CRM(0x05), CRM(0x06), CRM(0x07), 
		 CRM(0x08), CRM(0x00), 
	 0},
	{	 CRM(0x09), CRM(0x0A), CRM(0x00), CRM(0x00), CRM(0x0B), CRM(0x0C), CRM(0x00), CRM(0x00), 
		 CRM(0x0D), CRM(0x0E), 
	 0},
	{	 CRM(0x0F), CRM(0x10), CRM(0x11), CRM(0x12), CRM(0x13), CRM(0x14), CRM(0x15), CRM(0x16), 
		 CRM(0x10), CRM(0x17), 
	 0},
	{	 CRM(0x18), CRM(0x19), CRM(0x1A), CRM(0x1B), CRM(0x1C), CRM(0x1D), CRM(0x1E), CRM(0x1F), 
		 CRM(0x19), CRM(0x20), 
	 0},
	{	 CRM(0x21), CRM(0x22), CRM(0x00), CRM(0x00), CRM(0x23), CRM(0x24), CRM(0x00), CRM(0x00), 
		 CRM(0x25), CRM(0x26), 
	 0},
	{	 CRM(0x00), CRM(0x27), CRM(0x28), CRM(0x29), CRM(0x2A), CRM(0x2B), CRM(0x2C), CRM(0x2D), 
		 CRM(0x2E), CRM(0x00), 
	 0}
};

BYTE code strCROWindow_3[6][11]=
{
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x01), CRM(0x02), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x03), CRM(0x04), CRM(0x05), CRM(0x06), CRM(0x07), CRM(0x08), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x09), CRM(0x0A), CRM(0x0B), CRM(0x0C), CRM(0x0D), CRM(0x0E), CRM(0x0F), CRM(0x10), 
		 CRM(0x0A), CRM(0x11), 
	 0},
	{	 CRM(0x12), CRM(0x13), CRM(0x14), CRM(0x15), CRM(0x16), CRM(0x17), CRM(0x18), CRM(0x19), 
		 CRM(0x13), CRM(0x1A), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x1B), CRM(0x1C), CRM(0x1D), CRM(0x1E), CRM(0x1F), CRM(0x20), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x21), CRM(0x22), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0}
};

void Osd_DrawCrossFont(void)
{

   BYTE ItemIndex,i;
   
      g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_CLEAR);
   	
OSD_TEXT_HI_ADDR_SET_BIT8();      	
         for(ItemIndex=0; ItemIndex<6; ItemIndex++)
           for(i=0; i<10; i++)
           {
           if(UserPrefCrossType==Cross_Mode1)
            Osd_DrawRealChar( i, ItemIndex, strCROWindow[ItemIndex][i]);
	    else if(UserPrefCrossType==Cross_Mode2)	   
            Osd_DrawRealChar( i, ItemIndex, strCROWindow_2[ItemIndex][i]);
	    else 
            Osd_DrawRealChar( i, ItemIndex, strCROWindow_3[ItemIndex][i]);
           }
OSD_TEXT_HI_ADDR_CLR_TO_0();      	
}

#else
#define CRM(x)   (LOBYTE(HotKey_ICON_Addr)+x)

 BYTE code strCROWindow[6][11]=
{
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x01), CRM(0x02), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x03), CRM(0x04), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x05), CRM(0x06), CRM(0x07), CRM(0x08), CRM(0x09), CRM(0x0A), CRM(0x05), CRM(0x06), 
		 CRM(0x07), CRM(0x08), 
	 0},
	{	 CRM(0x0B), CRM(0x0C), CRM(0x0D), CRM(0x0E), CRM(0x0F), CRM(0x10), CRM(0x0B), CRM(0x0C), 
		 CRM(0x0D), CRM(0x0E), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x11), CRM(0x12), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0},
	{	 CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x00), CRM(0x13), CRM(0x14), CRM(0x00), CRM(0x00), 
		 CRM(0x00), CRM(0x00), 
	 0}
};

void Osd_DrawCrossFont(BYTE ColorIndex)
{
   BYTE ItemIndex,i;
   
   if(ColorIndex==0)
      g_u8OsdFontColor=MONO_COLOR(CP_YELLOW, CP_CLEAR);
   else 
      g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_CLEAR);
   	
OSD_TEXT_HI_ADDR_SET_BIT8();      	
         for(ItemIndex=0; ItemIndex<6; ItemIndex++)
           for(i=0; i<10; i++)
            Osd_DrawRealChar( i, ItemIndex, strCROWindow[ItemIndex][i]);
    
OSD_TEXT_HI_ADDR_CLR_TO_0();      	
}

#endif


#if 0//LED_HOT_Menu_Func
#define LeM(x)   (HotKey_Led_ICON_Addr+x)
#define LedMenuLogoXPos     0x04
#define LedMenuLogoYPos     0x05//0x07

#if New_LED_Menu_Icon
 WORD code strLedWindow[15][27]=
{
	{	 LeM(0x00), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x02), 
	 0},
	{	 LeM(0x03), LeM(0x04), LeM(0x05), LeM(0x06), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x08), LeM(0x09), LeM(0x0A), LeM(0x0B), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x0C), LeM(0x0D), 
		 LeM(0x07), LeM(0x0E), 
	 0},
	{	 LeM(0x03), LeM(0x07), LeM(0x0F), LeM(0x10), LeM(0x11), LeM(0x12), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x13), LeM(0x14), LeM(0x15), LeM(0x16), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x17), LeM(0x18), LeM(0x19), LeM(0x1A), 
		 LeM(0x07), LeM(0x0E), 
	 0},
	{	 LeM(0x03), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x1B), LeM(0x1C), LeM(0x1D), LeM(0x1E), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x1F), LeM(0x20), LeM(0x21), LeM(0x22), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x23), LeM(0x24), LeM(0x25), LeM(0x26), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x0E), 
	 0},
	{	 LeM(0x03), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x27), LeM(0x28), 
		 LeM(0x29), LeM(0x2A), LeM(0x07), LeM(0x2B), LeM(0x2C), LeM(0x2D), LeM(0x2E), LeM(0x07), 
		 LeM(0x2F), LeM(0x30), LeM(0x31), LeM(0x32), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x0E), 
	 0},
	{	 LeM(0x03), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x33), LeM(0x34), LeM(0x07), LeM(0x2B), LeM(0x2C), LeM(0x2D), LeM(0x2E), LeM(0x07), 
		 LeM(0x35), LeM(0x36), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x0E), 
	 0},
	{	 LeM(0x03), LeM(0x07), LeM(0x07), LeM(0x37), LeM(0x38), LeM(0x39), LeM(0x39), LeM(0x39), 
		 LeM(0x39), LeM(0x39), LeM(0x39), LeM(0x3A), LeM(0x2C), LeM(0x2D), LeM(0x3B), LeM(0x39), 
		 LeM(0x39), LeM(0x39), LeM(0x39), LeM(0x39), LeM(0x39), LeM(0x3C), LeM(0x3D), LeM(0x07), 
		 LeM(0x07), LeM(0x0E), 
	 0},
	{	 LeM(0x03), LeM(0x07), LeM(0x3E), LeM(0x3F), LeM(0x07), LeM(0x07), LeM(0x40), LeM(0x41), 
		 LeM(0x41), LeM(0x41), LeM(0x41), LeM(0x42), LeM(0x2C), LeM(0x2D), LeM(0x43), LeM(0x41), 
		 LeM(0x41), LeM(0x41), LeM(0x41), LeM(0x44), LeM(0x07), LeM(0x07), LeM(0x45), LeM(0x46), 
		 LeM(0x07), LeM(0x0E), 
	 0},
	{	 LeM(0x03), LeM(0x07), LeM(0x47), LeM(0x48), LeM(0x49), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x2B), LeM(0x2C), LeM(0x2D), LeM(0x2E), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x4A), LeM(0x4B), LeM(0x4C), 
		 LeM(0x07), LeM(0x0E), 
	 0},
	{	 LeM(0x03), LeM(0x4D), LeM(0x4E), LeM(0x07), LeM(0x4F), LeM(0x50), LeM(0x51), LeM(0x52), 
		 LeM(0x53), LeM(0x53), LeM(0x53), LeM(0x54), LeM(0x55), LeM(0x56), LeM(0x57), LeM(0x53), 
		 LeM(0x53), LeM(0x53), LeM(0x58), LeM(0x51), LeM(0x59), LeM(0x5A), LeM(0x07), LeM(0x5B), 
		 LeM(0x5C), LeM(0x0E), 
	 0},
	{	 LeM(0x5D), LeM(0x5E), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x5F), LeM(0x07), LeM(0x07), LeM(0x60), LeM(0x01), 
		 LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x61), 
		 LeM(0x62), LeM(0x63), 
	 0},
	{	 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x2B), LeM(0x07), LeM(0x07), LeM(0x2E), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x07), 
	 0},
	{	 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x64), LeM(0x65), LeM(0x66), LeM(0x67), LeM(0x07), 
		 LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), LeM(0x07), 
		 LeM(0x07), LeM(0x07), 
	 0},
	{	 LeM(0x07), LeM(0x07), LeM(0x68), LeM(0x69), LeM(0x6A), LeM(0x6B), LeM(0x6C), LeM(0x6D), 
		 LeM(0x6E), LeM(0x6F), LeM(0x70), LeM(0x71), LeM(0x72), LeM(0x73), LeM(0x74), LeM(0x75), 
		 LeM(0x76), LeM(0x77), LeM(0x78), LeM(0x79), LeM(0x7A), LeM(0x7B), LeM(0x7C), LeM(0x7D), 
		 LeM(0x07), LeM(0x07), 
	 0},
	{	 LeM(0x07), LeM(0x7E), LeM(0x7F), LeM(0x80), LeM(0x80), LeM(0x80), LeM(0x80), LeM(0x80), 
		 LeM(0x80), LeM(0x81), LeM(0x82), LeM(0x80), LeM(0x83), LeM(0x84), LeM(0x85), LeM(0x86), 
		 LeM(0x81), LeM(0x80), LeM(0x80), LeM(0x80), LeM(0x80), LeM(0x80), LeM(0x80), LeM(0x87), 
		 LeM(0x88), LeM(0x07), 
	 0}
};

#else
 WORD code strLedWindow[15][27]=
{
	{	 LeM(0x00), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x02), 
	 0},
	{	 LeM(0x03), LeM(0x04), LeM(0x05), LeM(0x06), LeM(0x07), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x09), LeM(0x0A), LeM(0x0B), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x03), LeM(0x08), LeM(0x0D), LeM(0x0E), LeM(0x0F), LeM(0x10), LeM(0x07), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x11), LeM(0x12), LeM(0x13), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x14), LeM(0x15), LeM(0x16), LeM(0x17), LeM(0x18), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x03), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x19), LeM(0x1A), LeM(0x1B), LeM(0x1C), 
		 LeM(0x1D), LeM(0x08), LeM(0x08), LeM(0x1E), LeM(0x1F), LeM(0x20), LeM(0x21), LeM(0x08), 
		 LeM(0x08), LeM(0x22), LeM(0x23), LeM(0x24), LeM(0x25), LeM(0x18), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x03), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x26), LeM(0x27), 
		 LeM(0x28), LeM(0x29), LeM(0x08), LeM(0x2A), LeM(0x2B), LeM(0x03), LeM(0x2C), LeM(0x08), 
		 LeM(0x2D), LeM(0x2E), LeM(0x2F), LeM(0x30), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x03), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x31), LeM(0x32), LeM(0x08), LeM(0x33), LeM(0x2B), LeM(0x03), LeM(0x34), LeM(0x08), 
		 LeM(0x35), LeM(0x36), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x03), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x37), LeM(0x2B), LeM(0x03), LeM(0x38), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x03), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x39), LeM(0x2B), LeM(0x03), LeM(0x3A), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x03), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x3B), LeM(0x2B), LeM(0x03), LeM(0x3C), LeM(0x3D), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x03), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x3E), LeM(0x2B), LeM(0x03), LeM(0x3F), LeM(0x40), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x0C), 
	 0},
	{	 LeM(0x41), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x42), LeM(0x2B), LeM(0x03), LeM(0x43), LeM(0x01), 
		 LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), LeM(0x01), 
		 LeM(0x01), LeM(0x44), 
	 0},
	{	 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x45), LeM(0x2B), LeM(0x03), LeM(0x46), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), 
	 0},
	{	 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x47), LeM(0x2B), LeM(0x03), LeM(0x48), LeM(0x08), 
		 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), 
	 0},
	{	 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x49), LeM(0x4A), 
		 LeM(0x4B), LeM(0x4C), LeM(0x4D), LeM(0x4E), LeM(0x4F), LeM(0x50), LeM(0x51), LeM(0x52), 
		 LeM(0x53), LeM(0x54), LeM(0x55), LeM(0x56), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), 
	 0},
	{	 LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x57), 
		 LeM(0x58), LeM(0x59), LeM(0x5A), LeM(0x5B), LeM(0x5C), LeM(0x5D), LeM(0x5E), LeM(0x5F), 
		 LeM(0x59), LeM(0x60), LeM(0x61), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), LeM(0x08), 
		 LeM(0x08), LeM(0x08), 
	 0}
};

#endif


void Osd_DrawHotLedMenu(BYTE DemoFlag)
{
   BYTE ItemIndex,i;
   BYTE ReDrawCounter=0;

   	
if(!DemoFlag)
{
      g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2, CP_BK_COLOR_H);
	  
      OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8		   
	  
    for(ItemIndex=0; ItemIndex<15; ItemIndex++)
       for(i=0; i<26; i++)
       {
          if(((i>=1)&&(i<=9)&&(ItemIndex>=1)&&(ItemIndex<=5))||((i>=16)&&(i<=23)&&(ItemIndex>=1)&&(ItemIndex<=5)))
             ;
	   else	  
            Osd_DrawRealChar( LedMenuLogoXPos+i, LedMenuLogoYPos+ItemIndex, strLedWindow[ItemIndex][i]);
       }

      //Item0
      //Minus
      Osd_DrawRealChar(LedColorTextXPos+3, (LedColorTextYPos+LedColorTextShiftXPos*HotLedColorIDX),(LOBYTE(MM_GUIDE_FONT_START))+0x0A);
      //Plus
      Osd_DrawRealChar(LedColorTextXPos+18, (LedColorTextYPos+LedColorTextShiftXPos*HotLedColorIDX),(LOBYTE(MM_GUIDE_FONT_START))+0x0B);

      //Item1
      //Minus
      Osd_DrawRealChar(LedColorTextXPos+3, (LedColorTextYPos+LedColorTextShiftXPos*HotLedStrengthIDX),(LOBYTE(MM_GUIDE_FONT_START))+0x0A);
      //Plus
      Osd_DrawRealChar(LedColorTextXPos+18, (LedColorTextYPos+LedColorTextShiftXPos*HotLedStrengthIDX),(LOBYTE(MM_GUIDE_FONT_START))+0x0B);

      //Item2
      //Minus
      Osd_DrawRealChar(LedColorTextXPos+3, (LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeIDX),(LOBYTE(MM_GUIDE_FONT_START))+0x0A);
      //Plus
      Osd_DrawRealChar(LedColorTextXPos+18, (LedColorTextYPos+LedColorTextShiftXPos*HotLedColorModeIDX),(LOBYTE(MM_GUIDE_FONT_START))+0x0B);

      OSD_TEXT_HI_ADDR_CLR_TO_0(); //enable bit 8		   
}

   if((USER_PREF_LEDColorful_MODE==LedColorful)&&(DemoFlag))
   {
      OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8		   
    for(ReDrawCounter=0; ReDrawCounter<7; ReDrawCounter++)
    {

   if(ReDrawCounter==LedRed)
      g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_BK_COLOR_H);
   else if(ReDrawCounter==LedYellow)
      g_u8OsdFontColor=MONO_COLOR(CP_YELLOW, CP_BK_COLOR_H);
   else if(ReDrawCounter==LedGreen)
      g_u8OsdFontColor=MONO_COLOR(CP_GREEN, CP_BK_COLOR_H);
   else if(ReDrawCounter==LedCyan)
      g_u8OsdFontColor=MONO_COLOR(CP_Cyan, CP_BK_COLOR_H);
   else if(ReDrawCounter==LedBlue)
      g_u8OsdFontColor=MONO_COLOR(CP_BLUE, CP_BK_COLOR_H);
   else if(ReDrawCounter==LedPurple)
      g_u8OsdFontColor=MONO_COLOR(CP_Purple, CP_BK_COLOR_H);
   else if(ReDrawCounter==LedWhite)
      g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BK_COLOR_H);

	 
    for(ItemIndex=0; ItemIndex<15; ItemIndex++)
       for(i=0; i<26; i++)
       {
          if(((i>=1)&&(i<=9)&&(ItemIndex>=1)&&(ItemIndex<=5))||((i>=16)&&(i<=23)&&(ItemIndex>=1)&&(ItemIndex<=5)))
            Osd_DrawRealChar( LedMenuLogoXPos+i, LedMenuLogoYPos+ItemIndex, strLedWindow[ItemIndex][i]);
       }
	Delay1ms(200);
    }
      OSD_TEXT_HI_ADDR_CLR_TO_0(); //enable bit 8		   
   }
   else
   {
   if(USER_PREF_LEDColorful_MODE==LedRed)
      g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_BK_COLOR_H);
   else if(USER_PREF_LEDColorful_MODE==LedYellow)
      g_u8OsdFontColor=MONO_COLOR(CP_YELLOW, CP_BK_COLOR_H);
   else if(USER_PREF_LEDColorful_MODE==LedGreen)
      g_u8OsdFontColor=MONO_COLOR(CP_GREEN, CP_BK_COLOR_H);
   else if(USER_PREF_LEDColorful_MODE==LedCyan)
      g_u8OsdFontColor=MONO_COLOR(CP_Cyan, CP_BK_COLOR_H);
   else if(USER_PREF_LEDColorful_MODE==LedBlue)
      g_u8OsdFontColor=MONO_COLOR(CP_BLUE, CP_BK_COLOR_H);
   else if(USER_PREF_LEDColorful_MODE==LedPurple)
      g_u8OsdFontColor=MONO_COLOR(CP_Purple, CP_BK_COLOR_H);
   else if(USER_PREF_LEDColorful_MODE==LedWhite)
      g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BK_COLOR_H);
   else
      g_u8OsdFontColor=MONO_COLOR(CP_MainGround, CP_BK_COLOR_H);

      OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8		   
    for(ItemIndex=0; ItemIndex<15; ItemIndex++)
       for(i=0; i<26; i++)
       {
          if(((i>=1)&&(i<=9)&&(ItemIndex>=1)&&(ItemIndex<=5))||((i>=16)&&(i<=23)&&(ItemIndex>=1)&&(ItemIndex<=5)))
            Osd_DrawRealChar( LedMenuLogoXPos+i, LedMenuLogoYPos+ItemIndex, strLedWindow[ItemIndex][i]);
       }
      OSD_TEXT_HI_ADDR_CLR_TO_0(); //enable bit 8		   	   
  }	   
   
  
}
#endif


void DrawOsdMenu(Bool ShowMenu)
{
    BYTE  xdata  i;
    Bool redrawFlags;
    BYTE xdata g_u8MenuTempIndex_Prev;
	
       Clr_DDCCIActiveFlag();
       Clr_OSDFreeSyncActionFlag();
	   
       g_stMenuFlag.bThisItemSelected=0;
	g_stMenuFlag.bEnableItemSelect=0;   
      g_stMenuFlag.bEnableItemSelectSub=0; 	   
      //printData("HotCrossMenuFlag==%x",HotCrossMenuFlag);
      if((HotCrossMenuFlag)&&(g_u8MenuPageIndex==MENU_ROOT))
    	{
          Clr_HotCrossMenuFlag();
    	}

	DisableAllSubWin();

       OSD_SET_WND(OSD_MAIN_WND);
#if ENABLE_OSD_ROTATION
#if ((defined(LianHeChuangXin_Project))&&(!Enable_Panel_Rotate_180))
  if(g_u8MenuPageIndex==MENU_KEY_GUIDE)
  {
        msWriteByteMask(OSD1_03, 0, BIT7|BIT6);
  }
 else	   
#endif
  {
 #if Enable_Panel_Rotate_180
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
        msWriteByteMask(OSD1_03, BIT7|BIT6, BIT7|BIT6);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)	
        msWriteByteMask(OSD1_03, 0, BIT7|BIT6);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_270)	
        msWriteByteMask(OSD1_03, BIT6, BIT7|BIT6);
   else
        msWriteByteMask(OSD1_03, BIT7, BIT7|BIT6);
 #else
    if (USER_PREF_OSD_ROTATION==Osd_Rotation_90)
        msWriteByteMask(OSD1_03, BIT7|BIT6, BIT7|BIT6);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_180)	
        msWriteByteMask(OSD1_03, BIT7, BIT7|BIT6);
   else if(USER_PREF_OSD_ROTATION==Osd_Rotation_270)	
        msWriteByteMask(OSD1_03, BIT6, BIT7|BIT6);
   else
        msWriteByteMask(OSD1_03, 0, BIT7|BIT6);
  #endif 
  }
#endif

#if Enable_Corss_LHCX_New
    if(((g_u8MenuPageIndex==HotKeyGameModeMenu)&&(g_u8PrevMenuPageIndex==HotKeyGameSelectMenu||g_u8PrevMenuPageIndex==HotKeyGameClokMenu||g_u8PrevMenuPageIndex==HotKeyGameClokPositionMenu))
    ||((g_u8MenuPageIndex==HotKeyGameSelectMenu)&&(g_u8PrevMenuPageIndex==HotKeyGameModeMenu))
    ||((g_u8MenuPageIndex==HotKeyGameClokMenu)&&(g_u8PrevMenuPageIndex==HotKeyGameModeMenu||g_u8PrevMenuPageIndex==HotKeyGameClokPositionMenu))
    ||(g_u8MenuPageIndex==HotKeyGameSubSelectMenu)
    ||((g_u8MenuPageIndex==HotKeyGameClokPositionMenu)&&(g_u8PrevMenuPageIndex==HotKeyGameModeMenu||g_u8PrevMenuPageIndex==HotKeyGameClokMenu))
    )
    {
    if((g_u8MenuPageIndex==HotKeyGameClokPositionMenu)&&(g_u8PrevMenuPageIndex==HotKeyGameClokMenu))
    ;
    else
    	{
	if(g_u8MenuPageIndex==HotKeyGameClokShowMenu)
	  g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_BK_COLOR_H);
	else
	   g_u8OsdFontColor=MONO_COLOR(CP_SL_COLOR_L1,CP_BK_COLOR_H);

           for(i=0; i<GET_MENU_YSIZE(CURRENT_MENU); i++)
           {
              Osd_DrawContinuousChar(0, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));
           }
    	}
    }
    else
#endif
    if ((IS_MAIN_L1(g_u8MenuPageIndex))||(IS_MAIN_L0(g_u8MenuPageIndex)&&(g_u8PrevMenuPageIndex!=MENU_ROOT)))
      ;
   else
    {
        WAIT_V_OUTPUT_BLANKING();
        Menu_Hide();
    }

 
    if (CURRENT_MENU.u8Flags & MPB_INVISIBLE)
    {
            return;
    }
	
    redrawFlags=FALSE;
	
   if (GET_MENU_XSIZE(CURRENT_MENU)!=g_u8OsdWindowWidth
       ||GET_MENU_YSIZE(CURRENT_MENU)!=g_u8OsdWindowHeight
       ||(!OSDShowFlag) )
       	redrawFlags=TRUE;
  
	
    if (redrawFlags)
    {
        Osd_SetTransparency(USER_PREF_TRANSPARENCY);
		
    //Set OSD Size
#if Enable_Corss_LHCX_New
	 if(g_u8MenuPageIndex!=HotKeyGameClokPositionMenu)
#endif
        Osd_SetWindowSize(GET_MENU_XSIZE(CURRENT_MENU), GET_MENU_YSIZE(CURRENT_MENU)/*, RETAIN*/);


   printData("ReDraw DrawOsdMenu==%d", g_u8MenuPageIndex);
   printData("ReDraw g_u8MenuItemIndex==%d",g_u8MenuItemIndex);
   printData("ReDraw USER_PREF_INPUT_PRIORITY==%d",USER_PREF_INPUT_PRIORITY);
//   printData("ReDraw HotKeyGameClokShowMenu==%d",HotKeyGameClokShowMenu);

    //Set OSD Position
        if ((CURRENT_MENU.u8Flags & MPB_CENTER))
        {
            if (g_u8MenuPageIndex == MENU_CABLE_NOT_CONNECTED)
            {
                g_stMenuFlag.bOsdReverseX=0;
                g_stMenuFlag.bOsdReverseY=0;
            }
            u16MoveX=Osd_MenuHPosition(50,TRUE);
            u16MoveY=Osd_MenuVPosition(50,TRUE);
        }
	else if(g_u8MenuPageIndex==MENU_INPUT)
		Osd_SetPosition( 99, 2 );
	else if(g_u8MenuPageIndex==MENU_HOTPICTUREMODE)
		Osd_SetPosition( 99, 2 );
	else if(g_u8MenuPageIndex==MENU_HOTINPUT)
		Osd_SetPosition( 99, 98 );
		
#if (defined(LianHeChuangXin_Project))
       else if(g_u8MenuPageIndex==MENU_KEY_GUIDE)
       {
     #if (defined(K3D_Project)||defined(K3L_CrossOver_Project))  
           Osd_SetPosition( 88, 0 );
     #elif (defined(K3C_Project)||defined(K3L_LED_Project))
           Osd_SetPosition( 50, 0 );
     #elif (defined(K3D_FHD144_Project))
           Osd_SetPosition( 89, 0 );
     #elif (defined(GK3B_Project))
           Osd_SetPosition( 100, 2);
     #elif (defined(W3K_Project))
           Osd_SetPosition( 96, 0 );
     #elif (defined(V3K_Project)||defined(V3L_Project)||defined(V3G_Project))
           Osd_SetPosition( 93, 0 );
     #else	 
           Osd_SetPosition( 50, 0 );
     #endif	 
       }
#endif
  #if Enable_Corss_LHCX_New
      else if (g_u8MenuPageIndex==HotKeyGameClokShowMenu)
      {
        printData("GameModeTime_Position==%d",GameModeTime_Position);
         if(GameModeTime_Position==HotKeyGameClokPosition_UpItem)
            Osd_SetPosition(0, 98);
         else if(GameModeTime_Position==HotKeyGameClokPosition_CenterItem)
            Osd_SetPosition(0, 50);
         else if(GameModeTime_Position==HotKeyGameClokPosition_DownItem)
            Osd_SetPosition(0, 2);
	  else 
            Osd_SetPosition(50, 50);
	  	
         Set_HotGameTimeShowFlag();
      }
  #endif
        else
        {
        #if ENABLE_OSD_ROTATION
            if (USER_PREF_OSD_ROTATION!=Osd_Rotation_Normal)
                SetMenuPositionRotated();
            else
        #endif
                SetMenuPositionNormal();
        }

    #if LOGO_ENABLE	
        if( g_u8MenuPageIndex == MENU_LOGO)
        {
           Osd_SetTransparency(0);		
           DrawLogo();
           Menu_Show();
           return;
        }
     #endif

	   g_u8OsdFontColor=MONO_COLOR(CP_SL_COLOR_L1,CP_BK_COLOR_H);
           for(i=0; i<GET_MENU_YSIZE(CURRENT_MENU); i++)
           {
              Osd_DrawContinuousChar(0, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));
           }

    ///Load Fonts
 #if Enable_Corss_LHCX_New
   if((g_u8MenuPageIndex>=HotKeyGameModeMenu&&g_u8MenuPageIndex<=HotKeyGameClokShowMenu)&&(g_u8MenuPageIndex!=HotKeyGameClokPositionMenu))
      DynamicLoad_HotGameFont(g_u8MenuPageIndex);
#endif

    if (CURRENT_MENU.LoadFont!=NULL)
    	{
        CURRENT_MENU.LoadFont();
    	}
    	 LoadMenuCommonPropFont();
	 DynamicLoadFont(g_u8MenuPageIndex);	 
	if(IS_MAIN_L0(g_u8MenuPageIndex))
	 LoadMainMenuFont();	 


    #if ENABLE_DEMURA_FUNCTION
        if (g_u8MenuPageIndex==MENU_DEMURA_MEASURE_MODE)
        {
            Menu_Hide();
            DrawDeMuraMeasurePattern();
        }
        else
    #endif
        if (g_u8MenuPageIndex!=MENU_HOT_MENU_LOCK
    #if LOGO_ENABLE
       &&g_u8MenuPageIndex!=MENU_LOGO
    #endif
	)
        {
            DrawFrame();
        }

#if ENABLE_AUDIO_SETTINGS_MENU
     if(g_u8MenuPageIndex == MENU_HOTMUTE)	
    	{
  	   Osd_DrawMuteFont();	
    	}
     else
 #endif	 	
	if(g_u8MenuPageIndex == MENU_HOTCROSS)	
    	{
    #if New_Cross_Enable||Enable_Corss_FY||Enable_Corss_LHCX||Enable_Corss_RunFeng
  	   Osd_DrawCrossFont();	
   #else	 
  	   Osd_DrawCrossFont(0);	
   #endif
    	}

    }
   else
   {

   if((IS_MAIN_L1(g_u8MenuPageIndex))||(IS_MAIN_L0(g_u8MenuPageIndex)))
   {
	g_u8OsdFontColor=MONO_COLOR(CP_SL_COLOR_L1,CP_BK_COLOR_H);
       for(i=7; i<Main_Description_YPos; i++)
       {
                Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
       }

       ClearMainFrame();

//Clear Description
     {
         g_u8OsdFontColor=MONO_COLOR(CP_MainGround,CP_MainGround);
         
         for (i=(Main_Description_YPos+3); i<(Main_Description_YPos+7); i++)
         {
         Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
         }	
     }

	 if(IS_MAIN_L0(g_u8MenuPageIndex))
	 {

          LoadMainMenuFont();	 
          
          //Clear Sub MidLine
          g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
	   OSD_TEXT_HI_ADDR_SET_BIT8();	
     #if LED_HOT_Menu_Func
	 if(g_u8PrevMenuPageIndex==MENU_SYSTEMLLEDUSER)  
 	 {
          Osd_DrawRealChar(0, (Sub_MidLine_YPos+(MENU_SYSTEM-MENU_QUICKSTART)*4), MAIN_Left_Line);
          Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (Sub_MidLine_YPos+(MENU_SYSTEM-MENU_QUICKSTART)*4), MAIN_Right_Line);	
	 }
	 else
     #endif	 
	 {
          Osd_DrawRealChar(0, (Sub_MidLine_YPos+(g_u8PrevMenuPageIndex-MENU_QUICKSTART)*4), MAIN_Left_Line);
          Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (Sub_MidLine_YPos+(g_u8PrevMenuPageIndex-MENU_QUICKSTART)*4), MAIN_Right_Line);	
	 }
          OSD_TEXT_HI_ADDR_CLR_TO_0();
	 }

         if(IS_MAIN_L1(g_u8MenuPageIndex))
         {
          //ReDraw Line Color
           g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
 	   OSD_TEXT_HI_ADDR_SET_BIT8();
      #if LED_HOT_Menu_Func	   
	   if(g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER)
	   {
          for(i=(7+10*(MENU_SYSTEM-MENU_QUICKSTART)); i<(7+10*((MENU_SYSTEM-MENU_QUICKSTART)+1)); i++)
           {
                  Osd_DrawRealChar(0, i, MAIN_Left_Line);
           }
           for(i=(7+10*(MENU_SYSTEM-MENU_QUICKSTART)); i<(7+10*((MENU_SYSTEM-MENU_QUICKSTART)+1)); i++)
           {
                  Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
           }
	   }
	   else
	#endif   
	   {
          for(i=(7+10*(g_u8MenuPageIndex-MENU_QUICKSTART)); i<(7+10*((g_u8MenuPageIndex-MENU_QUICKSTART)+1)); i++)
           {
                  Osd_DrawRealChar(0, i, MAIN_Left_Line);
           }
           for(i=(7+10*(g_u8MenuPageIndex-MENU_QUICKSTART)); i<(7+10*((g_u8MenuPageIndex-MENU_QUICKSTART)+1)); i++)
           {
                  Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
           }
	   }
           OSD_TEXT_HI_ADDR_CLR_TO_0();
         }

    if (CURRENT_MENU.LoadFont!=NULL)
    	{
        CURRENT_MENU.LoadFont();
    	}
	 DynamicLoadFont(g_u8MenuPageIndex);	 

	 
        LoadMenuDescriptionPropFont();
		
   }
      DrawFrame();

    }



    // draw osd menu item
    for (i = 0; i < MENU_ITEM_COUNT; i++)
    {   
        if ((CURRENT_MENU.pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
            continue; //Don't draw Adjust items in L2
        if (i!=g_u8MenuItemIndex)
            DrawOsdMenuItem(i, &CURRENT_MENU.pstMenuItems[i]);
    }

    if(g_u8MenuPageIndex==MENU_COLOR)
    {
      g_stMenuFlag.bEnableItemSelect=1;
      g_u8MenuTempIndex_Prev=g_u8MenuItemIndex;	  
      g_u8MenuItemIndex=0xFE;	  
  /*  
     if(CURRENT_COLOR_TEMP==PCT_Advanced)
     	{
        for (i = 0; i < tblMenus[MENU_ADVANCED].u8MenuItemCount; i++)
        {   
            if ((tblMenus[MENU_ADVANCED].pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
                continue; //Don't draw Adjust items in L2
                DrawOsdMenuItem(i, &tblMenus[MENU_ADVANCED].pstMenuItems[i]);
         }
     	}
      else
*/	  	
      	{
        for (i = 0; i < tblMenus[MENU_USER].u8MenuItemCount; i++)
        {   
            if ((tblMenus[MENU_USER].pstMenuItems[i].u16Flags&MIB_ADJUST_ITEM))
                continue; //Don't draw Adjust items in L2
                DrawOsdMenuItem(i, &tblMenus[MENU_USER].pstMenuItems[i]);
         }
      g_u8MenuItemIndex=g_u8MenuTempIndex_Prev;	  

           //DrawOsdMenuItem(Advanced_Cyan_IDX, &tblMenus[MENU_ADVANCED].pstMenuItems[Advanced_Cyan_IDX]);
           //DrawOsdMenuItem(Advanced_Magenta_IDX, &tblMenus[MENU_ADVANCED].pstMenuItems[Advanced_Magenta_IDX]);
           //DrawOsdMenuItem(Advanced_Yellow_IDX, &tblMenus[MENU_ADVANCED].pstMenuItems[Advanced_Yellow_IDX]);
      	}
	 g_stMenuFlag.bEnableItemSelect=0; 
     }
#if LED_HOT_Menu_Func	
    else if(g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER)
    	{
    	   g_u8MenuTempIndex_Prev=g_u8MenuPageIndex;
	   g_u8MenuPageIndex=MENU_LEDMODE;   
          DrawLedMenuItems();
	   g_u8MenuPageIndex=g_u8MenuTempIndex_Prev; 
    	}
#endif	
    // Draw current item last to avoid the icon color issue.
    if ((g_u8MenuItemIndex!=NO_SELECT_ITEM))//&&(!STATUS_MENUPAGE))
        DrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_MENU.pstMenuItems[g_u8MenuItemIndex]);

        DrawDescription(g_u8MenuPageIndex);

#if 0//LED_HOT_Menu_Func
     if(g_u8MenuPageIndex == MENU_HOTLEDMODE)	
    	{
    	   Osd_LoadHOTLedFont();
  	   Osd_DrawHotLedMenu(0);	
    	}
 #endif	 	

    if (ShowMenu)
        Menu_Show();

}


#if Enable_Corss_LHCX_New
void Osd_Draw2x2Icon(BYTE xPos, BYTE yPos, BYTE *str)
{
  BYTE drawCode=TRUE;
    //BYTE dispChar ;  
   // BYTE dispPtr   ; 
	
   
	
    if (yPos&BIT7)
        drawCode=FALSE;
	
   // yPos&=0x3F;//~(BIT7|BIT6);
   #ifdef NewOSDEngine	
    yPos&=0x7F;
   #else
    yPos&=0x3F;//~(BIT7|BIT6);
   #endif
OSD_TEXT_HI_ADDR_SET_BIT8();//msWriteByte(OSDDF_REG, DISPCD8_B);
Osd_DrawRealChar(xPos+0, yPos, str[0]);
Osd_DrawRealChar(xPos+1, yPos, str[1]);
Osd_DrawRealChar(xPos+0, yPos+1, str[2]);
Osd_DrawRealChar(xPos+1, yPos+1, str[3]);

OSD_TEXT_HI_ADDR_CLR_TO_0();//msWriteByte(OSDDF_REG, 0);
}
void Osd_Draw4x3Icon(BYTE xPos, BYTE yPos, BYTE *str)
{
  BYTE drawCode=TRUE;
    //BYTE dispChar ;  
   // BYTE dispPtr   ; 
	
   
	
    if (yPos&BIT7)
        drawCode=FALSE;
	
   // yPos&=0x3F;//~(BIT7|BIT6);
   #ifdef NewOSDEngine	
    yPos&=0x7F;
   #else
    yPos&=0x3F;//~(BIT7|BIT6);
   #endif
OSD_TEXT_HI_ADDR_SET_BIT8();//msWriteByte(OSDDF_REG, DISPCD8_B);
Osd_DrawRealChar(xPos+0, yPos, str[0]);
Osd_DrawRealChar(xPos+1, yPos, str[1]);
Osd_DrawRealChar(xPos+2, yPos, str[2]);
Osd_DrawRealChar(xPos+3, yPos, str[3]);

Osd_DrawRealChar(xPos+0, yPos+1, str[4]);
Osd_DrawRealChar(xPos+1, yPos+1, str[5]);
Osd_DrawRealChar(xPos+2, yPos+1, str[6]);
Osd_DrawRealChar(xPos+3, yPos+1, str[7]);

Osd_DrawRealChar(xPos+0, yPos+2, str[8]);
Osd_DrawRealChar(xPos+1, yPos+2, str[9]);
Osd_DrawRealChar(xPos+2, yPos+2, str[10]);
Osd_DrawRealChar(xPos+3, yPos+2, str[11]);




OSD_TEXT_HI_ADDR_CLR_TO_0();//msWriteByte(OSDDF_REG, 0);
}

void Osd_Draw5x2Icon(BYTE xPos, BYTE yPos, BYTE *str)
{
 BYTE drawCode=TRUE;
    //BYTE dispChar ;  
   // BYTE dispPtr   ; 
	
   
	
    if (yPos&BIT7)
        drawCode=FALSE;
	
   // yPos&=0x3F;//~(BIT7|BIT6);
   #ifdef NewOSDEngine	
    yPos&=0x7F;
   #else
    yPos&=0x3F;//~(BIT7|BIT6);
   #endif
OSD_TEXT_HI_ADDR_SET_BIT9();//msWriteByte(OSDDF_REG, DISPCD8_B);

Osd_DrawRealChar(xPos+0, yPos, str[0]);
Osd_DrawRealChar(xPos+1, yPos, str[1]);
Osd_DrawRealChar(xPos+2, yPos, str[2]);
Osd_DrawRealChar(xPos+3, yPos, str[3]);
Osd_DrawRealChar(xPos+4, yPos, str[4]);

Osd_DrawRealChar(xPos+0, yPos+1, str[5]);
Osd_DrawRealChar(xPos+1, yPos+1, str[6]);
Osd_DrawRealChar(xPos+2, yPos+1, str[7]);
Osd_DrawRealChar(xPos+3, yPos+1, str[8]);
Osd_DrawRealChar(xPos+4, yPos+1, str[9]);

OSD_TEXT_HI_ADDR_CLR_TO_0();//msWriteByte(OSDDF_REG, 0);
}

void Osd_Draw6x4Icon(BYTE xPos, BYTE yPos, BYTE *str)
{
  BYTE drawCode=TRUE;
    //BYTE dispChar ;  
   // BYTE dispPtr   ; 
	
   
	
    if (yPos&BIT7)
        drawCode=FALSE;
	
   // yPos&=0x3F;//~(BIT7|BIT6);
   #ifdef NewOSDEngine	
    yPos&=0x7F;
   #else
    yPos&=0x3F;//~(BIT7|BIT6);
   #endif
OSD_TEXT_HI_ADDR_SET_BIT8();//msWriteByte(OSDDF_REG, DISPCD8_B);

Osd_DrawRealChar(xPos, yPos, str[0]);
Osd_DrawRealChar(xPos+1, yPos, str[1]);
Osd_DrawRealChar(xPos+2, yPos, str[2]);
Osd_DrawRealChar(xPos+3, yPos, str[3]);
Osd_DrawRealChar(xPos+4, yPos, str[4]);
Osd_DrawRealChar(xPos+5, yPos, str[5]);

Osd_DrawRealChar(xPos, yPos+1, str[6]);
Osd_DrawRealChar(xPos+1, yPos+1, str[7]);
Osd_DrawRealChar(xPos+2, yPos+1, str[8]);
Osd_DrawRealChar(xPos+3, yPos+1, str[9]);
Osd_DrawRealChar(xPos+4, yPos+1, str[10]);
Osd_DrawRealChar(xPos+5, yPos+1, str[11]);

Osd_DrawRealChar(xPos, yPos+2, str[12]);
Osd_DrawRealChar(xPos+1, yPos+2, str[13]);
Osd_DrawRealChar(xPos+2, yPos+2, str[14]);
Osd_DrawRealChar(xPos+3, yPos+2, str[15]);
Osd_DrawRealChar(xPos+4, yPos+2, str[16]);
Osd_DrawRealChar(xPos+5, yPos+2, str[17]);

Osd_DrawRealChar(xPos, yPos+3, str[18]);
Osd_DrawRealChar(xPos+1, yPos+3, str[19]);
Osd_DrawRealChar(xPos+2, yPos+3, str[20]);
Osd_DrawRealChar(xPos+3, yPos+3, str[21]);
Osd_DrawRealChar(xPos+4, yPos+3, str[22]);
Osd_DrawRealChar(xPos+5, yPos+3, str[23]);

OSD_TEXT_HI_ADDR_CLR_TO_0();//msWriteByte(OSDDF_REG, 0);
}

void Osd_Draw8x5Icon(BYTE xPos, BYTE yPos, BYTE *str)
{
	BYTE drawCode=TRUE;
    //BYTE dispChar ;  
   // BYTE dispPtr   ; 
	
   
	
    if (yPos&BIT7)
        drawCode=FALSE;
	
   // yPos&=0x3F;//~(BIT7|BIT6);
   #ifdef NewOSDEngine	
    yPos&=0x7F;
   #else
    yPos&=0x3F;//~(BIT7|BIT6);
   #endif
OSD_TEXT_HI_ADDR_SET_BIT8();//msWriteByte(OSDDF_REG, DISPCD8_B);

Osd_DrawRealChar(xPos,     yPos, str[0]);
Osd_DrawRealChar(xPos+1, yPos, str[1]);
Osd_DrawRealChar(xPos+2, yPos, str[2]);
Osd_DrawRealChar(xPos+3, yPos, str[3]);
Osd_DrawRealChar(xPos+4, yPos, str[4]);
Osd_DrawRealChar(xPos+5, yPos, str[5]);
Osd_DrawRealChar(xPos+6, yPos, str[6]);
Osd_DrawRealChar(xPos+7, yPos, str[7]);

Osd_DrawRealChar(xPos,     yPos+1, str[8]);
Osd_DrawRealChar(xPos+1, yPos+1, str[9]);
Osd_DrawRealChar(xPos+2, yPos+1, str[10]);
Osd_DrawRealChar(xPos+3, yPos+1, str[11]);
Osd_DrawRealChar(xPos+4, yPos+1, str[12]);
Osd_DrawRealChar(xPos+5, yPos+1, str[13]);
Osd_DrawRealChar(xPos+6, yPos+1, str[14]);
Osd_DrawRealChar(xPos+7, yPos+1, str[15]);

Osd_DrawRealChar(xPos,     yPos+2, str[16]);
Osd_DrawRealChar(xPos+1, yPos+2, str[17]);
Osd_DrawRealChar(xPos+2, yPos+2, str[18]);
Osd_DrawRealChar(xPos+3, yPos+2, str[19]);
Osd_DrawRealChar(xPos+4, yPos+2, str[20]);
Osd_DrawRealChar(xPos+5, yPos+2, str[21]);
Osd_DrawRealChar(xPos+6, yPos+2, str[22]);
Osd_DrawRealChar(xPos+7, yPos+2, str[23]);

Osd_DrawRealChar(xPos+0, yPos+3, str[24]);
Osd_DrawRealChar(xPos+1, yPos+3, str[25]);
Osd_DrawRealChar(xPos+2, yPos+3, str[26]);
Osd_DrawRealChar(xPos+3, yPos+3, str[27]);
Osd_DrawRealChar(xPos+4, yPos+3, str[28]);
Osd_DrawRealChar(xPos+5, yPos+3, str[29]);
Osd_DrawRealChar(xPos+6, yPos+3, str[30]);
Osd_DrawRealChar(xPos+7, yPos+3, str[31]);

Osd_DrawRealChar(xPos,     yPos+4, str[32]);
Osd_DrawRealChar(xPos+1, yPos+4, str[33]);
Osd_DrawRealChar(xPos+2, yPos+4, str[34]);
Osd_DrawRealChar(xPos+3, yPos+4, str[35]);
Osd_DrawRealChar(xPos+4, yPos+4, str[36]);
Osd_DrawRealChar(xPos+5, yPos+4, str[37]);
Osd_DrawRealChar(xPos+6, yPos+4, str[38]);
Osd_DrawRealChar(xPos+7, yPos+4, str[39]);

OSD_TEXT_HI_ADDR_CLR_TO_0();//msWriteByte(OSDDF_REG, 0);
}

void Osd_Draw2x2x5Icon(BYTE xPos, BYTE yPos, BYTE *str)
{
	Osd_Draw2x2Icon( xPos,  yPos, str);
	Osd_Draw2x2Icon( xPos+2,  yPos, str+4);
	Osd_Draw2x2Icon( xPos+4,  yPos, str+8);
	Osd_Draw2x2Icon( xPos+6,  yPos, str+12);
	Osd_Draw2x2Icon( xPos+8,  yPos, str+16);
}

 void DrawOsd_GameModeItemFrame(BYTE itemindex)
{
	BYTE i;//,j
	
	if(g_u8MenuPageIndex==HotKeyGameModeMenu)
	{
		if(itemindex==g_u8MenuItemIndex)
		{
			g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_BK_COLOR_L0);
		}
		else
		{
			g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_FG_COLOR_L2);
		}
		
			OSD_TEXT_HI_ADDR_SET_BIT8();
			Osd_DrawRealChar((9+22*itemindex), 5, HotGameFrameFONT_2COLOR_ICON_START);
			Osd_DrawRealChar((28+22*itemindex), 5, HotGameFrameFONT_2COLOR_ICON_START+1);
			Osd_DrawRealChar((9+22*itemindex), 14, HotGameFrameFONT_2COLOR_ICON_START+2);
			Osd_DrawRealChar((28+22*itemindex), 14, HotGameFrameFONT_2COLOR_ICON_START+3);
			for(i=(10+22*itemindex);i<(10+18+22*itemindex);i++)
			{
				Osd_DrawRealChar(i,5 , HotGameFrameFONT_2COLOR_ICON_START+4);
				Osd_DrawRealChar(i,14 , HotGameFrameFONT_2COLOR_ICON_START+5);
			}
			OSD_TEXT_HI_ADDR_CLR_TO_0();
		for(i=6;i<14;i++)
		{
			//DrawRealStr(10+22*itemindex, i, "                  ");
                     Osd_DrawContinuousChar(10+22*itemindex, i, SPACE_FONT, 18);

			OSD_TEXT_HI_ADDR_SET_BIT8();
			Osd_DrawRealChar(9+22*itemindex, i, HotGameFrameFONT_2COLOR_ICON_START+6);
			Osd_DrawRealChar(28+22*itemindex, i, HotGameFrameFONT_2COLOR_ICON_START+7);
			OSD_TEXT_HI_ADDR_CLR_TO_0();
		}
	}
	
	else if(g_u8MenuPageIndex==HotKeyGameSelectMenu)
	{
		if(itemindex==g_u8MenuItemIndex)
		{
			g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_BK_COLOR_L0);
		}
		else
		{
			g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_FG_COLOR_L2);
		}
			OSD_TEXT_HI_ADDR_SET_BIT8();
			Osd_DrawRealChar((3+14*itemindex), 6, HotGameFrameFONT_2COLOR_ICON_START);
			Osd_DrawRealChar((14+14*itemindex), 6, HotGameFrameFONT_2COLOR_ICON_START+1);
			Osd_DrawRealChar((3+14*itemindex), 12, HotGameFrameFONT_2COLOR_ICON_START+2);
			Osd_DrawRealChar((14+14*itemindex), 12, HotGameFrameFONT_2COLOR_ICON_START+3);
			for(i=(4+14*itemindex);i<(14+14*itemindex);i++)
			{
				Osd_DrawRealChar(i,6 , HotGameFrameFONT_2COLOR_ICON_START+4);
				Osd_DrawRealChar(i,12 , HotGameFrameFONT_2COLOR_ICON_START+5);
			}
			OSD_TEXT_HI_ADDR_CLR_TO_0();
			
		for(i=7;i<12;i++)
		{
			//DrawRealStr(4+14*itemindex, i, "          ");
			
                     Osd_DrawContinuousChar(4+14*itemindex, i, SPACE_FONT, 10);
			
			OSD_TEXT_HI_ADDR_SET_BIT8();
			Osd_DrawRealChar(3+14*itemindex, i, HotGameFrameFONT_2COLOR_ICON_START+6);
			Osd_DrawRealChar(14+14*itemindex, i, HotGameFrameFONT_2COLOR_ICON_START+7);
			OSD_TEXT_HI_ADDR_CLR_TO_0();
		}
	}
	
	else if(g_u8MenuPageIndex==HotKeyGameClokMenu)
	{
		if(itemindex==g_u8MenuItemIndex)
		{
			//Osd_Set256TextColor(4, Color_4);
			//g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_MSG_BK_H);
			g_u8OsdFontColor=FOUR_COLOR(24);//gray
		}
		else
		{
			//Osd_Set256TextColor(5, Color_4);
			//g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L0,CP_MSG_BK_H);
			g_u8OsdFontColor=FOUR_COLOR(25);//gray
		}

			OSD_TEXT_HI_ADDR_SET_BIT9();
			
			Osd_DrawRealChar((10+14*((itemindex<3)?itemindex:(itemindex-3))), (3+((itemindex<3)?0:5)), HotGameFONT_4COLOR_ICON_START+2*2);
			Osd_DrawRealChar((21+14*((itemindex<3)?itemindex:(itemindex-3))), (3+((itemindex<3)?0:5)), HotGameFONT_4COLOR_ICON_START+2*3);
			Osd_DrawRealChar((10+14*((itemindex<3)?itemindex:(itemindex-3))), (6+((itemindex<3)?0:5)), HotGameFONT_4COLOR_ICON_START+2*6);
			Osd_DrawRealChar((21+14*((itemindex<3)?itemindex:(itemindex-3))), (6+((itemindex<3)?0:5)), HotGameFONT_4COLOR_ICON_START+2*7);
			for(i=(11+14*((itemindex<3)?itemindex:(itemindex-3)));i<(21+14*((itemindex<3)?itemindex:(itemindex-3)));i++)
			{
				Osd_DrawRealChar(i,(3+((itemindex<3)?0:5)) , HotGameFONT_4COLOR_ICON_START+2*5);
				Osd_DrawRealChar(i,(6+((itemindex<3)?0:5)) , HotGameFONT_4COLOR_ICON_START+2*4);
			}
			for(i=4;i<6;i++)
			{
				//DrawRealStr(10+14*itemindex, j, "            ");
				Osd_DrawRealChar((10+14*((itemindex<3)?itemindex:(itemindex-3))), i+((itemindex<3)?0:5), HotGameFONT_4COLOR_ICON_START+2*0);
				Osd_DrawRealChar((21+14*((itemindex<3)?itemindex:(itemindex-3))), i+((itemindex<3)?0:5), HotGameFONT_4COLOR_ICON_START+2*1);
			}

			OSD_TEXT_HI_ADDR_CLR_TO_0();
		
	}	
}

 void DrawHotKeyGameClokShow(void)
 {
	BYTE i;

	    if(GameModeTime_S==0&&GameModeTime_M==0)
		//Osd_SetTextMonoColor(0x07,0x00);
		g_u8OsdFontColor=MONO_COLOR(CP_GREEN,CP_CLEAR);
	    else
		//Osd_SetTextMonoColor(0x04,0x00);
		g_u8OsdFontColor=MONO_COLOR(CP_FG_COLOR_L2,CP_CLEAR);
		
	for(i=0;i<10;i++)
	{
		OSD_TEXT_HI_ADDR_SET_BIT8();
		Osd_DrawRealChar(i,0 , HotGameFrameFONT_2COLOR_ICON_START+0x09);
		Osd_DrawRealChar(i,3 , HotGameFrameFONT_2COLOR_ICON_START+0x0A);
		OSD_TEXT_HI_ADDR_CLR_TO_0();
	}
		
    if(GameModeTime_S==0&&GameModeTime_M==0)
	//Osd_SetTextMonoColor(0x02,0x07);
	g_u8OsdFontColor=MONO_COLOR(CP_MSG_BK_H,CP_GREEN);
    else
	//Osd_SetTextMonoColor(0x06,0x04);
	g_u8OsdFontColor=MONO_COLOR(CP_Black,CP_FG_COLOR_L2);


//CURRENT_MENU_ITEMS->unPos
	//CURRENT_MENU.CURRENT_MENU_ITEMS
	OSD_TEXT_HI_ADDR_SET_BIT8();
	Osd_Draw2x2Icon(0,1,HotKeyGameClokShowIcon(GameModeTime_M/10));	//strHotKeyGameClokShowIcon[GameModeTime_M/10]
	Osd_Draw2x2Icon(2,1,HotKeyGameClokShowIcon(GameModeTime_M%10));	//strHotKeyGameClokShowIcon[GameModeTime_M%10]
	Osd_Draw2x2Icon(4,1,HotKeyGameClokShowIcon(10));					//strHotKeyGameClokShowIcon[10]
	Osd_Draw2x2Icon(6,1,HotKeyGameClokShowIcon(GameModeTime_S/10));	//strHotKeyGameClokShowIcon[GameModeTime_S/10]
	Osd_Draw2x2Icon(8,1,HotKeyGameClokShowIcon(GameModeTime_S%10));	//strHotKeyGameClokShowIcon[GameModeTime_S%10]


/*
	Osd_Draw2x2Icon(PTR_GET_MI_XPOS(CURRENT_MENU.pstMenuItems),    PTR_GET_MI_YPOS(CURRENT_MENU.pstMenuItems),HotKeyGameClokShowIcon(GameModeTime_M/10));//strHotKeyGameClokShowIcon[GameModeTime_M/10]
	Osd_Draw2x2Icon(PTR_GET_MI_XPOS(CURRENT_MENU.pstMenuItems)+2,PTR_GET_MI_YPOS(CURRENT_MENU.pstMenuItems),HotKeyGameClokShowIcon(GameModeTime_M%10));//strHotKeyGameClokShowIcon[GameModeTime_M%10]
	Osd_Draw2x2Icon(PTR_GET_MI_XPOS(CURRENT_MENU.pstMenuItems)+4,PTR_GET_MI_YPOS(CURRENT_MENU.pstMenuItems),HotKeyGameClokShowIcon(10));//strHotKeyGameClokShowIcon[10]
	Osd_Draw2x2Icon(PTR_GET_MI_XPOS(CURRENT_MENU.pstMenuItems)+6,PTR_GET_MI_YPOS(CURRENT_MENU.pstMenuItems),HotKeyGameClokShowIcon(GameModeTime_S/10));//strHotKeyGameClokShowIcon[GameModeTime_S/10]
	Osd_Draw2x2Icon(PTR_GET_MI_XPOS(CURRENT_MENU.pstMenuItems)+8,PTR_GET_MI_YPOS(CURRENT_MENU.pstMenuItems),HotKeyGameClokShowIcon(GameModeTime_S%10));//strHotKeyGameClokShowIcon[GameModeTime_S%10]
*/
	OSD_TEXT_HI_ADDR_CLR_TO_0();

//PTR_GET_MI_XPOS(CURRENT_MENU.pstMenuItems), PTR_GET_MI_YPOS(pstMenuItem)
//CURRENT_MENU.pstMenuItems[BriCon_NR_IDX*2]
 }


#endif



void ReDrawMainStringText(void)
{
  BYTE i, j;
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_BK_COLOR_H);
 	 if(USER_PREF_LANGUAGE==LANG_Spanish)
 	 {
               for (i=0; i<2; i++)
                 for(j=0; j<20; j++)
                 {
                 	 Osd_DrawRealChar(10+j, 8+i, strQuiWindow_Span[i][j]);
                 }	
 	 }
 	 else if(USER_PREF_LANGUAGE==LANG_French)
 	 {
               for (i=0; i<2; i++)
                 for(j=0; j<17; j++)
                 {
                 	 Osd_DrawRealChar(10+j, 8+i, strQuiWindow_Fran[i][j]);
                 }	
 	 }
 	 else if(USER_PREF_LANGUAGE==LANG_TChinese)
 	 {
               for (i=0; i<2; i++)
                 for(j=0; j<11; j++)
                 {
                 	 Osd_DrawRealChar(14+j, 8+i, strQuiWindow_TChinese[i][j]);
                 }	
 	 }
 	 else
 	 {
               for (i=0; i<2; i++)
                 for(j=0; j<17; j++)
                 {
                 	 Osd_DrawRealChar(10+j, 8+i, strQuiWindow[i][j]);
                 }	
 	 }

               g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_BK_COLOR_H);	
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+1*4)+i, strPicWindow_Span[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
              for (i=0; i<2; i++)
                for(j=0; j<9; j++)
                {
                	 Osd_DrawRealChar(14+j, (8+1*4)+i, strPicWindow_Fran[i][j]);
                }	
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+1*4)+i, strPicWindow_TChinese[i][j]);
                }	
	}
	else
	{	
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+1*4)+i, strPicWindow[i][j]);
                }	
	}

				
	//Sub Text3
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_BK_COLOR_H);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	{
           for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+2*4)+i, strColWindow_Span[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_French)
	{
           for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+2*4)+i, strColWindow_Fran[i][j]);
                }
	}
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	{
           for (i=0; i<2; i++)
                for(j=0; j<8; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+2*4)+i, strColWindow_TChinese[i][j]);
                }
	}
	else
	{
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+2*4)+i, strColWindow[i][j]);
                }	
	}
				


	//Sub Text4
                 g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,CP_SubGround);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+3*4)+i, strSysWindow_Span[i][j]);
                }	
	 }		
	else if(USER_PREF_LANGUAGE==LANG_French)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<13; j++)
                {
                	 Osd_DrawRealChar(12+j, (8+3*4)+i, strSysWindow_Fran[i][j]);
                }	
	 }	
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	 {
              for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(15+j, (8+3*4)+i, strSysWindow_TChinese[i][j]);
                }	
	 }		
	else
	 {
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(10+j, (8+3*4)+i, strSYSWindow[i][j]);
                }	
	 }		


            g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_H);
	     if(USER_PREF_LANGUAGE==LANG_TChinese)
	     	{
	            for (i=0; i<2; i++)
	             for(j=0; j<6; j++)
	            {
	                Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_TChinese[i][j]);
	             }
		 }
		else if(USER_PREF_LANGUAGE==LANG_Spanish)
		{
		for (i=0; i<2; i++)
		for(j=0; j<12; j++)
			{
				 Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_Span[i][j]);
			}
		}
	    else
	    	{
	            for (i=0; i<2; i++)
	             for(j=0; j<11; j++)
	            {
	                if(USER_PREF_LANGUAGE==LANG_French)
	                Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow_Fran[i][j]);
	                else 	  
	                Osd_DrawRealChar(1+j, (Main_Description_YPos+1)+i, strDesWindow[i][j]);
	            }	
	    	}

}


void DrawDescription(BYTE menupage)
{
  if(menupage==MENU_QUICKSTART)
  {
     if(g_u8MenuItemIndex<=QS_BackLight_Adj_IDX)
       DrawOsdMenuItemRadioGroup(0, DrawBackLightDescRadioGroup);
     else if(g_u8MenuItemIndex<=QS_Brightness_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawBrightnessDescRadioGroup);
     else if(g_u8MenuItemIndex<=QS_Contrast_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawContrastDescRadioGroup);
     else if(g_u8MenuItemIndex<=QS_Preset_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawPresetDescRadioGroup);
     else if(g_u8MenuItemIndex<=QS_Aspect_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawAspectDescRadioGroup);
     else if(g_u8MenuItemIndex<=QS_Source_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawSourceDescRadioGroup);
     else if(g_u8MenuItemIndex<=QS_BlueLight_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawBlueLightDescRadioGroup);
     #if Free_Sync_Enable
     else if(g_u8MenuItemIndex<=QS_FreeSync_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawFreeSyncDescRadioGroup);
     #endif


  }
  else if(menupage==MENU_PICTURE)
  {
     if(g_u8MenuItemIndex<=Pic_BackLight_Adj_IDX)
       DrawOsdMenuItemRadioGroup(0, DrawBackLightDescRadioGroup);
     else if(g_u8MenuItemIndex<=Pic_Brightness_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawBrightnessDescRadioGroup);
     else if(g_u8MenuItemIndex<=Pic_Contrast_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawContrastDescRadioGroup);
     else if(g_u8MenuItemIndex<=Pic_Preset_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawPresetDescRadioGroup);
     else if(g_u8MenuItemIndex<=Pic_DCR_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawDCRDesccRadioGroup);
     else if(g_u8MenuItemIndex<=Pic_Aspect_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawAspectDescRadioGroup);
     else if(g_u8MenuItemIndex<=Pic_Sharpness_Adj_IDX)	 
       DrawOsdMenuItemRadioGroup(0, DrawSharpnessDesccRadioGroup);

	 
  }
   else if(menupage==MENU_COLOR)
  {
     if(g_u8MenuItemIndex<=Col_Temperature_Adj_IDX)
     {
     //if(CURRENT_COLOR_TEMP==PCT_Advanced)
    //   DrawOsdMenuItemRadioGroup(0, DrawColorTempAdvancedDesccRadioGroup);
    // else	 	
       DrawOsdMenuItemRadioGroup(0, DrawColorTempDesccRadioGroup);
     }
    else if(g_u8MenuItemIndex<=Col_Gamma_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawGammaDesccRadioGroup);
    else if(g_u8MenuItemIndex<=Col_Tint_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawTintDesccRadioGroup);
    else if(g_u8MenuItemIndex<=Col_Saturation_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawSaturationDesccRadioGroup);
    else if(g_u8MenuItemIndex<=Col_BlueLight_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawBlueLightDescRadioGroup);


  }
   else if(menupage==MENU_SYSTEM
#if LED_HOT_Menu_Func
   ||menupage==MENU_SYSTEMLLEDUSER
#endif
)
  {
     if(g_u8MenuItemIndex<=Sys_OverDrive_Adj_IDX)
       DrawOsdMenuItemRadioGroup(0, DrawODDescRadioGroup);
    #if Free_Sync_Enable
    else if(g_u8MenuItemIndex<=Sys_FreeSync_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawFreeSyncDescRadioGroup);
    #endif
    else if(g_u8MenuItemIndex<=Sys_SleepMode_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawSleepModeDescRadioGroup);
#if LED_HOT_Menu_Func
    else if(g_u8MenuItemIndex<=Sys_LedMode_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawLedModeAllDescRadioGroup);
#endif       
    else if(g_u8MenuItemIndex<=Sys_Language_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawLanguageDescRadioGroup);
    else if(g_u8MenuItemIndex<=Sys_OSDHPosition_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawOSDHDescRadioGroup);
#if Enable_OSD_Vposition	
    else if(g_u8MenuItemIndex<=Sys_OSDVPosition_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawOSDVDescRadioGroup);
#endif	
    else if(g_u8MenuItemIndex<=Sys_OSDTimer_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawOSDTimerDescRadioGroup);
    else if(g_u8MenuItemIndex<=Sys_OSDTransparencia_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawOSDTransDescRadioGroup);
#if ENABLE_AUDIO_SETTINGS_MENU	
    else if(g_u8MenuItemIndex<=Sys_Volume_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawVolumeDescRadioGroup);
#endif	
    else if(g_u8MenuItemIndex<=Sys_Reset_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawResetDescRadioGroup);

  }
#if LED_HOT_Menu_Func   
  else if(menupage==MENU_LEDMODE)
  {
    if(g_u8MenuItemIndex<=Led_Mode_Adj_IDX)
       DrawOsdMenuItemRadioGroup(0, DrawLedModeDescRadioGroup);
    else if(g_u8MenuItemIndex<=Led_Color_Adj_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawLedColorDescRadioGroup);
    else if(g_u8MenuItemIndex<=Led_Brigntness_Adj_IDX)
       DrawOsdMenuItemRadioGroup(0, DrawLedBrightnessDescRadioGroup);
  }
#endif
  else if(menupage==MENU_USER)
  {
    if(g_u8MenuItemIndex>=User_RGBCMY_IDX)	 	
       DrawOsdMenuItemRadioGroup(0, DrawColorTempAdvancedDesccRadioGroup);
    else
       DrawOsdMenuItemRadioGroup(0, DrawColorTempDesccRadioGroup);

  }
  

}

//////////////////////////////////////////////////////////////////////
#define QuM_Icon(x)            (MM_2COLOR_CURSOR_START+x)
#define Quc(x)            (MM_4COLOR_CURSOR_START+10*2+0x02*x)

 BYTE code strQucIconWindow[4][7]=
{
	{	 QuM_Icon(0x00), Quc(0x00), Quc(0x01), Quc(0x02), QuM_Icon(0x01), QuM_Icon(0x02), 
	 0},
	{	 QuM_Icon(0x03), Quc(0x03), QuM_Icon(0x04), Quc(0x04), QuM_Icon(0x05), QuM_Icon(0x06), 
	 0},
	{	 QuM_Icon(0x07), QuM_Icon(0x08), QuM_Icon(0x09), QuM_Icon(0x0A), QuM_Icon(0x0B), QuM_Icon(0x0C), 
	 0},
	{	 QuM_Icon(0x0D), QuM_Icon(0x0D), QuM_Icon(0x0E), QuM_Icon(0x0F), QuM_Icon(0x0D), QuM_Icon(0x0D), 
	 0}
};


#define PiM_Icon(x)            (MM_2COLOR_Picture_Start+x)
#define Pic(x)            (MM_4COLOR_CURSOR_START+15*2+0x02*x)
#define PiE(x)            (MM_8COLOR_ICON_START+0x03*x)

 BYTE code strPicIconWindow[4][7]=
{
	{	 PiM_Icon(0x00), PiM_Icon(0x01), Pic(0x00), Pic(0x01), PiM_Icon(0x02), PiM_Icon(0x03), 
	 0},
	{	 PiM_Icon(0x04), Pic(0x02), PiE(0x00), PiE(0x01), PiE(0x02), PiM_Icon(0x05), 
	 0},
	{	 PiM_Icon(0x06), Pic(0x03), Pic(0x04), Pic(0x05), Pic(0x06), PiM_Icon(0x07), 
	 0},
	{	 PiM_Icon(0x08), PiM_Icon(0x09), PiM_Icon(0x0A), PiM_Icon(0x0B), PiM_Icon(0x09), PiM_Icon(0x08), 
	 0}
};


#define CoM_Icon(x)            (MM_2COLOR_Color_Start+x)
#define Col(x)            (MM_4COLOR_CURSOR_START+22*2+0x02*x)

 BYTE code strColIconWindow[4][7]=
{
	{	 CoM_Icon(0x00), CoM_Icon(0x01), Col(0x00), Col(0x01), CoM_Icon(0x02), CoM_Icon(0x00), 
	 0},
	{	 CoM_Icon(0x03), Col(0x02), Col(0x03), CoM_Icon(0x04), CoM_Icon(0x05), CoM_Icon(0x00), 
	 0},
	{	 CoM_Icon(0x06), Col(0x04), CoM_Icon(0x07), CoM_Icon(0x08), CoM_Icon(0x09), CoM_Icon(0x0A), 
	 0},
	{	 CoM_Icon(0x00), CoM_Icon(0x0B), CoM_Icon(0x0C), Col(0x05), CoM_Icon(0x0D), CoM_Icon(0x00), 
	 0}
};


#define SyM_Icon(x)            (MM_2COLOR_System_Start+x)
#define Sys(x)            (MM_4COLOR_CURSOR_START+28*2+0x02*x)
 BYTE code strSysIconWindow[4][7]=
{
	{	 SyM_Icon(0x00), Sys(0x00), Sys(0x01), Sys(0x02), SyM_Icon(0x01), SyM_Icon(0x02), 
	 0},
	{	 SyM_Icon(0x03), Sys(0x03), Sys(0x04), SyM_Icon(0x04), Sys(0x05), SyM_Icon(0x05), 
	 0},
	{	 SyM_Icon(0x06), Sys(0x06), Sys(0x07), Sys(0x08), Sys(0x09), SyM_Icon(0x07), 
	 0},
	{	 SyM_Icon(0x08), SyM_Icon(0x08), SyM_Icon(0x09), SyM_Icon(0x0A), SyM_Icon(0x08), SyM_Icon(0x08), 
	 0}
};


 ///Draw Item Cursor
void DrawItemCursor(BYTE u8ItemIndex, BYTE u8CursorColorType)
{
    BYTE i,j;
    BYTE u8Temp=Osd_DoubleBuffer(FALSE); //Jison 091116

	//UNUSED(u8CursorColor);

    //WAIT_V_OUTPUT_BLANKING();
    //Menu_SetCursor(OSD_CURSOR_WND,FALSE);
    if (IS_MAIN_L0(g_u8MenuPageIndex))
    {
	  WAIT_V_OUTPUT_BLANKING();
				
	if(u8ItemIndex==Main_Quick_Start_IDX)	
	{

              if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                g_u8OsdFontColor=MONO_COLOR(0x03,0x03);
           else
                g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
              for (i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
              {
                	 Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
              }	
              OSD_TEXT_HI_ADDR_SET_BIT8();
              for(i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
              {
                     Osd_DrawRealChar(0, i, LOWBYTE(MAIN_Left_Line));
              }
              for(i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
              {
                     Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, LOWBYTE(MAIN_Right_Line));
              }

              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<4; i++)
                for(j=0; j<6; j++)
                {
                 if(strQucIconWindow[i][j]>=Quc(0))
                 {
                  if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
		     g_u8OsdFontColor=FOUR_COLOR(6);	
		    else
		     g_u8OsdFontColor=FOUR_COLOR(5);	
                 }
		   else if(strQucIconWindow[i][j]==QuM_Icon(4))	
		   {
                     g_u8OsdFontColor=MONO_COLOR(CP_ADJ_COLOR_L2, CP_BK_COLOR_L1);
		    }
		   else if(strQucIconWindow[i][j]==QuM_Icon(5)||strQucIconWindow[i][j]==QuM_Icon(8)||strQucIconWindow[i][j]==QuM_Icon(9)||strQucIconWindow[i][j]==QuM_Icon(10)||strQucIconWindow[i][j]==QuM_Icon(11))
		   {
                     g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_Black);
		   }
		   else
		   {
                   if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                     g_u8OsdFontColor=MONO_COLOR(CP_Frame, CP_Black);
                   else
                     g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H,CP_Black);
		    }
                	 Osd_DrawRealChar(5+j, (10+u8ItemIndex*10)+i, strQucIconWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();
		
              if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x03);
           else
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	  if(USER_PREF_LANGUAGE==LANG_Spanish)
              for (i=0; i<2; i++)
                for(j=0; j<20; j++)
                {
                	 Osd_DrawRealChar(11+j, (11+u8ItemIndex*10)+i, strQuiWindow_Span[i][j]);
                }
	  else if(USER_PREF_LANGUAGE==LANG_French)
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(12+j, (11+u8ItemIndex*10)+i, strQuiWindow_Fran[i][j]);
                }
	  else if(USER_PREF_LANGUAGE==LANG_TChinese)
              for (i=0; i<2; i++)
                for(j=0; j<10; j++)
                {
                	 Osd_DrawRealChar(15+j, (11+u8ItemIndex*10)+i, strQuiWindow_TChinese[i][j]);
                }
	  else
	   {
              for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(12+j, (11+u8ItemIndex*10)+i, strQuiWindow[i][j]);
                }
	   }
				
	}
	else if(u8ItemIndex==Main_Picture_IDX)	
	{
              if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                g_u8OsdFontColor=MONO_COLOR(0x03,0x03);
           else
                g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
                for (i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
                {
                	 Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                }	
              OSD_TEXT_HI_ADDR_SET_BIT8();
              for(i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
               {
                     Osd_DrawRealChar(0, i, LOWBYTE(MAIN_Left_Line));
               }
              for(i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
              {
                     Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, LOWBYTE(MAIN_Right_Line));
              }


              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<4; i++)
                for(j=0; j<6; j++)
                {

                 if(strPicIconWindow[i][j]>=PiE(0))
                 {
		     g_u8OsdFontColor=FOUR_COLOR(12);	
                 }
                 else if(strPicIconWindow[i][j]==Pic(0)||strPicIconWindow[i][j]==Pic(1))
                 {
                  if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
		     g_u8OsdFontColor=FOUR_COLOR(8);	
		    else
		     g_u8OsdFontColor=FOUR_COLOR(7);	
                 }
                 else if(strPicIconWindow[i][j]==Pic(2)||strPicIconWindow[i][j]==Pic(3)||strPicIconWindow[i][j]==Pic(4))
                 {
		     g_u8OsdFontColor=FOUR_COLOR(9);	
                 }
                 else if(strPicIconWindow[i][j]==Pic(5)||strPicIconWindow[i][j]==Pic(6))
                 {
		     g_u8OsdFontColor=FOUR_COLOR(10);	
                 }
		   else
		   {
                   if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                     g_u8OsdFontColor=MONO_COLOR(CP_Frame, CP_Black);
                   else
                     g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H, CP_Black);
		    }
                	 Osd_DrawRealChar(5+j, (10+u8ItemIndex*10)+i, strPicIconWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();
			  
              if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x03);
           else
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	  {
                for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(14+j, (11+u8ItemIndex*10)+i, strPicWindow_Span[i][j]);
                }	
	   }
	else if(USER_PREF_LANGUAGE==LANG_French)
	  {
                for (i=0; i<2; i++)
                for(j=0; j<9; j++)
                {
                	 Osd_DrawRealChar(16+j, (11+u8ItemIndex*10)+i, strPicWindow_Fran[i][j]);
                }	
	   }
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	  {
                for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(17+j, (11+u8ItemIndex*10)+i, strPicWindow_TChinese[i][j]);
                }	
	   }
	else
	  {
                for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(12+j, (11+u8ItemIndex*10)+i, strPicWindow[i][j]);
                }	
	   }
	 }
	else if(u8ItemIndex==Main_Color_IDX)	
	{
              if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                g_u8OsdFontColor=MONO_COLOR(0x03,0x03);
           else
                g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
                for (i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
                {
                	 Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                }	
                OSD_TEXT_HI_ADDR_SET_BIT8();
            for(i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
               {
                     Osd_DrawRealChar(0, i, LOWBYTE(MAIN_Left_Line));
               }
              for(i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
              {
                     Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
              }


              OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<4; i++)
                for(j=0; j<6; j++)
                {

                 if(strColIconWindow[i][j]==Col(0)||strColIconWindow[i][j]==Col(1)||strColIconWindow[i][j]==Col(2)||strColIconWindow[i][j]==Col(3))
                 {
                  if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
		     g_u8OsdFontColor=FOUR_COLOR(15);	
		    else
		     g_u8OsdFontColor=FOUR_COLOR(14);	
                 }
		  else if(strColIconWindow[i][j]==Col(4))	
	      	  {
                  if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
		     g_u8OsdFontColor=FOUR_COLOR(17);	
		    else
		     g_u8OsdFontColor=FOUR_COLOR(16);	
		  }
		  else if(strColIconWindow[i][j]==Col(5))	
	      	  {
                  if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
		     g_u8OsdFontColor=FOUR_COLOR(19);	
		    else
		     g_u8OsdFontColor=FOUR_COLOR(18);	
		  }
		  else if(strColIconWindow[i][j]==CoM_Icon(4))	
		   {
                     g_u8OsdFontColor=MONO_COLOR(CP_YELLOW, CP_Black);
		   }
		  else if(strColIconWindow[i][j]==CoM_Icon(7))	
		   {
                     g_u8OsdFontColor=MONO_COLOR(CP_PURPLE, CP_Black);
		   }
		  else if(strColIconWindow[i][j]==CoM_Icon(8))	
		   {
                     g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_Black);
		   }
		   else
		   {
                   if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                     g_u8OsdFontColor=MONO_COLOR(CP_Frame, CP_Black);
                   else
                     g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H, CP_Black);
		    }
                	 Osd_DrawRealChar(5+j, (10+u8ItemIndex*10)+i, strColIconWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();	
		
              if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x03);
           else
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	if(USER_PREF_LANGUAGE==LANG_Spanish)
	  {
                for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(12+j, (11+u8ItemIndex*10)+i, strColWindow_Span[i][j]);
                }	
	  }
	else if(USER_PREF_LANGUAGE==LANG_French)
	  {
                for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(14+j, (11+u8ItemIndex*10)+i, strColWindow_Fran[i][j]);
                }	
	  }
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	  {
                for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(17+j, (11+u8ItemIndex*10)+i, strColWindow_TChinese[i][j]);
                }	
	   }
	else
	  {
                for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(12+j, (11+u8ItemIndex*10)+i, strColWindow[i][j]);
                }	
	  }
	}
	else if(u8ItemIndex==Main_System_IDX)	
	{
		
              if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                g_u8OsdFontColor=MONO_COLOR(0x03,0x03);
           else
                g_u8OsdFontColor=MONO_COLOR(0x03,0x01);
                for (i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
                {
                	 Osd_DrawContinuousChar(1, i, SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
                }	
              OSD_TEXT_HI_ADDR_SET_BIT8();
              for(i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
               {
                     Osd_DrawRealChar(0, i, LOWBYTE(MAIN_Left_Line));
               }
              for(i=(7+10*u8ItemIndex); i<(7+10*(u8ItemIndex+1)); i++)
              {
                     Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, i, MAIN_Right_Line);
              }

		OSD_TEXT_HI_ADDR_SET_BIT9();
              for (i=0; i<4; i++)
                for(j=0; j<6; j++)
                {

                if(strSysIconWindow[i][j]>=Sys(0))	
	      	  {
                  if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
		     g_u8OsdFontColor=FOUR_COLOR(21);	
		    else
		     g_u8OsdFontColor=FOUR_COLOR(20);	
		  }
                else if(strSysIconWindow[i][j]==SyM_Icon(4))	
                {
                     g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_WHITE);
                }
		  else
		  {
                   if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                     g_u8OsdFontColor=MONO_COLOR(CP_Frame, CP_Black);
                   else
                     g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_H, CP_Black);
		    }
                	 Osd_DrawRealChar(5+j, (10+u8ItemIndex*10)+i, strSysIconWindow[i][j]);
                }	
		OSD_TEXT_HI_ADDR_CLR_TO_0();	

		
              if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x03);
           else
                g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1,0x01);
	  if(USER_PREF_LANGUAGE==LANG_Spanish)
	   {
                for (i=0; i<2; i++)
                for(j=0; j<12; j++)
                {
                	 Osd_DrawRealChar(14+j, (11+u8ItemIndex*10)+i, strSysWindow_Span[i][j]);
                }
	   }
	  else if(USER_PREF_LANGUAGE==LANG_French)
	   {
                for (i=0; i<2; i++)
                for(j=0; j<13; j++)
                {
                	 Osd_DrawRealChar(14+j, (11+u8ItemIndex*10)+i, strSysWindow_Fran[i][j]);
                }
	   }
	else if(USER_PREF_LANGUAGE==LANG_TChinese)
	  {
                for (i=0; i<2; i++)
                for(j=0; j<7; j++)
                {
                	 Osd_DrawRealChar(17+j, (11+u8ItemIndex*10)+i, strSysWindow_TChinese[i][j]);
                }	
	   }
	  else
	   {
                for (i=0; i<2; i++)
                for(j=0; j<17; j++)
                {
                	 Osd_DrawRealChar(12+j, (11+u8ItemIndex*10)+i, strSYSWindow[i][j]);
                }
	   }
	 }

				


    }
//=======================================================================
    else if (IS_MAIN_L1(g_u8MenuPageIndex))    //SUB MENU // EG1
    {
	WAIT_V_OUTPUT_BLANKING();
      if(g_u8MenuPageIndex==MENU_QUICKSTART)	
      	{
            if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
            else
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            Osd_DrawContinuousChar(1, (QuickStartTxtYPos+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (QuickStartTxtYPos+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (QuickStartTxtYPos+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
      	}	
	else if(g_u8MenuPageIndex==MENU_PICTURE)  
	{
            if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
            else
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            Osd_DrawContinuousChar(1, (PictureTxtYPos+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (PictureTxtYPos+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (PictureTxtYPos+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
	}
	else if((g_u8MenuPageIndex==MENU_COLOR)&&(g_stMenuFlag.bEnableItemSelect==0))
	{
	  BYTE YPosShift=0;
	  
            if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
            else
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);
     if(u8ItemIndex>=2)
	  YPosShift=4;
     else
	  YPosShift=0;
            Osd_DrawContinuousChar(1, (ColorTxtYPos+YPosShift+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);

            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (ColorTxtYPos+YPosShift+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (ColorTxtYPos+YPosShift+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
	}
	else if((g_u8MenuPageIndex==MENU_SYSTEM)&&(g_stMenuFlag.bEnableItemSelect==0))
	{
            if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
            else
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            Osd_DrawContinuousChar(1, (SystemTxtYPos+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (SystemTxtYPos+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (SystemTxtYPos+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
	}
#if LED_HOT_Menu_Func	
	else if((g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER)&&(g_stMenuFlag.bEnableItemSelect==0))
	{
            if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
            else
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);
         if(u8ItemIndex>=Sys_Language_IDX)
         {
            Osd_DrawContinuousChar(1, (SystemLedModeLEDUSERTxtYPos+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (SystemLedModeLEDUSERTxtYPos+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (SystemLedModeLEDUSERTxtYPos+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
         }
	  else
	  {
            Osd_DrawContinuousChar(1, (SystemTxtYPos+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);
            
            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (SystemTxtYPos+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (SystemTxtYPos+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
	  }
	}
#endif
	else if((g_u8MenuPageIndex==MENU_USER))
	{

            if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
            else
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            Osd_DrawContinuousChar(1, (ColorUserTxtYPos+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);

            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (ColorUserTxtYPos+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (ColorUserTxtYPos+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
	}
	else if((g_u8MenuPageIndex==MENU_ADVANCED))
	{

            if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
            else
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            Osd_DrawContinuousChar(1, (ColorAdvancedTxtYPos+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);

            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (ColorAdvancedTxtYPos+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (ColorAdvancedTxtYPos+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
	}
#if LED_HOT_Menu_Func	
	else if((g_u8MenuPageIndex==MENU_LEDMODE))
	{
            if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
            else
            g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);

            Osd_DrawContinuousChar(1, (SystemLedModeTxtYPos+(u8ItemIndex/2)*1), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU)-2);

            OSD_TEXT_HI_ADDR_SET_BIT8();
            Osd_DrawRealChar(0, (SystemLedModeTxtYPos+(u8ItemIndex/2)*1), LOWBYTE(MAIN_Left_Line));
            Osd_DrawRealChar(GET_MENU_XSIZE(CURRENT_MENU)-1, (SystemLedModeTxtYPos+(u8ItemIndex/2)*1), MAIN_Right_Line);
	     OSD_TEXT_HI_ADDR_CLR_TO_0();		
	}
#endif
    }
   else if(g_u8MenuPageIndex==MENU_HOTINPUT)
    {
      BYTE xdata DVIYPos=2;
      BYTE xdata HDMIYPos=4;
      BYTE xdata HDMI2YPos=6;
      BYTE xdata DPYPos=8;

#if (MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
      DPYPos=2;
      HDMIYPos=4;
      HDMI2YPos=6;
      DVIYPos=8;	  
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
      DVIYPos=0;
      HDMIYPos=4;
      HDMI2YPos=6;
      DPYPos=2;
#else
   #if (ENABLE_DVI_INPUT&&ENABLE_HDMI2ND_INPUT)
      DVIYPos=2;
      HDMIYPos=4;
      HDMI2YPos=6;
      DPYPos=8;
   #elif ((!ENABLE_DVI_INPUT)&&ENABLE_HDMI2ND_INPUT)	  
      DVIYPos=0;
      HDMIYPos=2;
      HDMI2YPos=4;
      DPYPos=6;
   #else
      DVIYPos=2;
      HDMIYPos=4;
      HDMI2YPos=0;
      DPYPos=6;
   #endif
#endif	  
             if(u8CursorColorType==CS_HIGHLIGHT||u8CursorColorType==CS_SELECTED)
             g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_Frame);
             else
             g_u8OsdFontColor=MONO_COLOR(CP_Frame,CP_MainGround);
 
             Osd_DrawContinuousChar(0, (2+u8ItemIndex*2), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));
             Osd_DrawContinuousChar(0, (3+u8ItemIndex*2), SPACE_FONT, GET_MENU_XSIZE(CURRENT_MENU));
 
 		if(u8ItemIndex==INPUT_DISPLAYPORT)
 		{
              if(g_u8MenuItemIndex==u8ItemIndex)
		  g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL);
		else	  
		  g_u8OsdFontColor=MONO_COLOR(CP_SubGround, CP_BK_COLOR_H);

                OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
                 for (i=0; i<2; i++)
                  for(j=0; j<5; j++)
                  {
                     Osd_DrawRealChar(9+j, DPYPos+i, strDP_Window[i][j]);
                  }	
               OSD_TEXT_HI_ADDR_CLR_TO_0();	 		
		}
		#if ENABLE_HDMI2ND_INPUT
		else if(u8ItemIndex==INPUT_HDMI2ND)
		{
              if(g_u8MenuItemIndex==u8ItemIndex)
		  g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL);
		else	  
		  g_u8OsdFontColor=MONO_COLOR(CP_SubGround, CP_BK_COLOR_H);

                OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
                  for (i=0; i<2; i++)
                  for(j=0; j<10; j++)
                  {
                     Osd_DrawRealChar(7+j, HDMI2YPos+i, strHDM2Window[i][j]);
                  }	
               OSD_TEXT_HI_ADDR_CLR_TO_0();	 		
		}
		else if(u8ItemIndex==INPUT_HDMI)
		{
              if(g_u8MenuItemIndex==u8ItemIndex)
		  g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL);
		else	  
		  g_u8OsdFontColor=MONO_COLOR(CP_SubGround, CP_BK_COLOR_H);

                OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
                  for (i=0; i<2; i++)
                  for(j=0; j<10; j++)
                  {
                     Osd_DrawRealChar(7+j, HDMIYPos+i, strHDM1Window[i][j]);
                  }	
               OSD_TEXT_HI_ADDR_CLR_TO_0();	 		
		}
		#else
		else if(u8ItemIndex==INPUT_HDMI)
		{
              if(g_u8MenuItemIndex==u8ItemIndex)
		  g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL);
		else	  
		  g_u8OsdFontColor=MONO_COLOR(CP_SubGround, CP_BK_COLOR_H);

                OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
                  for (i=0; i<2; i++)
                  for(j=0; j<9; j++)
                  {
                     Osd_DrawRealChar(7+j, HDMIYPos+i, strHDMWindow[i][j]);
                  }	
               OSD_TEXT_HI_ADDR_CLR_TO_0();	 		
		}
		#endif
		
	#if ENABLE_DVI_INPUT	
		else if(u8ItemIndex==INPUT_DVI)
		{
              if(g_u8MenuItemIndex==u8ItemIndex)
		  g_u8OsdFontColor=MONO_COLOR(CP_BK_COLOR_L1, CP_BK_COLOR_SEL);
		else	  
		  g_u8OsdFontColor=MONO_COLOR(CP_SubGround, CP_BK_COLOR_H);

                OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8	
                  for (i=0; i<2; i++)
                  for(j=0; j<7; j++)
                  {
                     Osd_DrawRealChar(8+j, DVIYPos+i, strDVIWindow[i][j]);
                  }	
               OSD_TEXT_HI_ADDR_CLR_TO_0();	 		
		}
	#endif	
	   
    }
   

    Osd_DoubleBuffer(u8Temp&BIT0);
}



// draw menu item
void DrawOsdMenuItem(BYTE u8ItemIndex, MenuItemType *pstMenuItem)
{
#if 1
    if (IsItemHidden(pstMenuItem->u16Flags))
        return;     //Do not draw hidden item
#endif
  //- Cursor Part  Start -

    if (IS_MAIN_L0(g_u8MenuPageIndex)||(IS_MAIN_L1(g_u8MenuPageIndex)&&(u8ItemIndex%2==0))||(g_u8MenuPageIndex==MENU_HOTINPUT))
    {

        if (pstMenuItem->u16Flags & MIB_SELECTABLE)
        {

	     if ( u8ItemIndex == g_u8MenuItemIndex)
            {
                    DrawItemCursor(u8ItemIndex,CS_SELECTED);
            }
            else if (g_u8MenuItemIndex!=NO_SELECT_ITEM && u8ItemIndex!=NO_SELECT_ITEM) //Jison 080502
                DrawItemCursor(u8ItemIndex,CS_NORMAL);
         }

    }

#if Enable_Corss_LHCX_New
   if(g_u8MenuPageIndex>=HotKeyGameModeMenu&&g_u8MenuPageIndex<=HotKeyGameClokPositionMenu)
   {
       if(pstMenuItem->u16Flags & MIB_SELECTABLE)
          DrawOsd_GameModeItemFrame(u8ItemIndex);
   }
#endif
	
#if 0
    if((pstMenuItem->u16Flags&MIB_ColorTemp_DISABLE)&&(g_u8MenuPageIndex==MENU_COLOR)&&((CURRENT_COLOR_TEMP==PCT_USER)/*||(CURRENT_COLOR_TEMP==PCT_Advanced)*/))
    {
     if((CURRENT_COLOR_TEMP==PCT_USER)&&(u8ItemIndex>=Advanced_Cyan_IDX))
	 g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_MainGround);	
     else	 
        g_u8OsdFontColor=pstMenuItem->u8Color;
    }
    else
#endif		
    if (pstMenuItem->u16Flags & MIB_SELECTABLE && u8ItemIndex == g_u8MenuItemIndex)
        g_u8OsdFontColor=pstMenuItem->u8SelColor;
    else
        g_u8OsdFontColor=pstMenuItem->u8Color;

   

    if (IsItemGrayed(pstMenuItem->u16Flags, u8ItemIndex))
    {
#if ENABLE_VGA_INPUT    
     if((MENU_PICTURE==MENU_PICTURE)&&(u8ItemIndex>=Pic_AutoAdjust_IDX))
        g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_Black);			
     else		
#endif	 	
        g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_MainGround);
/*	
     if((g_u8MenuPageIndex==MENU_QUICKSTART)||((g_u8MenuPageIndex==MENU_PICTURE)&&(u8ItemIndex<=Pic_Contrast_Adj_IDX)))
        g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_MainGround);			
     else		   
        g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_Black);
*/	 
    }


if (!((pstMenuItem->u16Flags & MIB_ADJUST_ITEM) && (u8ItemIndex != g_u8MenuItemIndex)))
{
    DrawOsdMenuItemText(u8ItemIndex, pstMenuItem);
}

if((IS_MAIN_L0(g_u8MenuPageIndex))&&(pstMenuItem->u16Flags & MIB_SELECTABLE))
{
   if((u8ItemIndex == g_u8MenuItemIndex))
    DrawOsdMenuItemValue(u8ItemIndex, &(pstMenuItem->stDisplayValue));
   	
}
else
    DrawOsdMenuItemValue(u8ItemIndex, &(pstMenuItem->stDisplayValue));

}

//=========================================================================

// draw menu item display text
void DrawOsdMenuItemText(BYTE u8ItemIndex, MenuItemType *pstMenuItem)
{
 BYTE YPosShift=0;
 
    u8ItemIndex=u8ItemIndex;

    if (pstMenuItem->DisplayText == NULL)
        return;
    if (pstMenuItem->DisplayText()==NULL)
        return;///Donnot draw if NULL
#if LED_HOT_Menu_Func
if((g_u8MenuPageIndex==MENU_SYSTEM)&&(u8ItemIndex>=Sys_Language_IDX)&&(g_stMenuFlag.bEnableItemSelect==1))
    YPosShift=4;
else 
#endif
if((g_u8MenuPageIndex==MENU_COLOR)&&(u8ItemIndex>=Col_Gamma_IDX)&&(g_stMenuFlag.bEnableItemSelectSub==1))
    YPosShift=6;
else
    YPosShift=0;

    if ((pstMenuItem->u8TXTFlags & TXT_OVER_256))
        OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8
     else if ((pstMenuItem->u8TXTFlags & TXT_OVER_512))
        OSD_TEXT_HI_ADDR_SET_BIT9(); //enable bit 9

   if ((pstMenuItem->u8TXTFlags & TXT_FULL_CENTER))
    {
        DrawFullCenterText(PTR_GET_MI_XPOS(pstMenuItem), PTR_GET_MI_YPOS(pstMenuItem), pstMenuItem->DisplayText());
    }
    else if ((pstMenuItem->u8TXTFlags & TXT_RealText))
    {
        DrawRealStr(PTR_GET_MI_XPOS(pstMenuItem), PTR_GET_MI_YPOS(pstMenuItem), pstMenuItem->DisplayText());
    }
#if Enable_Corss_LHCX_New
    else if ((pstMenuItem->u8TXTFlags & TXT_ICON))
    {
          if(g_u8MenuPageIndex==HotKeyGameModeMenu)
          {
            Osd_Draw6x4Icon(PTR_GET_MI_XPOS(pstMenuItem), PTR_GET_MI_YPOS(pstMenuItem), pstMenuItem->DisplayText());	
          }
          else if(g_u8MenuPageIndex==HotKeyGameClokPositionMenu)
          {
            Osd_Draw5x2Icon(PTR_GET_MI_XPOS(pstMenuItem), PTR_GET_MI_YPOS(pstMenuItem),  pstMenuItem->DisplayText());	
          }
	    else if(g_u8MenuPageIndex==HotKeyGameSubSelectMenu)
          {
            Osd_Draw4x3Icon(PTR_GET_MI_XPOS(pstMenuItem), PTR_GET_MI_YPOS(pstMenuItem),  pstMenuItem->DisplayText());	
          }
          else if(g_u8MenuPageIndex==HotKeyGameSelectMenu)
          {
            Osd_Draw8x5Icon(PTR_GET_MI_XPOS(pstMenuItem), PTR_GET_MI_YPOS(pstMenuItem),  pstMenuItem->DisplayText());	
          }
          else if(g_u8MenuPageIndex==HotKeyGameClokMenu)
          {
            Osd_Draw2x2x5Icon(PTR_GET_MI_XPOS(pstMenuItem), PTR_GET_MI_YPOS(pstMenuItem),  pstMenuItem->DisplayText());	
          }
	}
#endif	  	
    else
    {
    #if 0
        printf("[OSD]current Textitem Xpos = %d \n", PTR_GET_MI_XPOS(pstMenuItem));
        printf("[OSD]current Textitem Ypos = %d \n", PTR_GET_MI_YPOS(pstMenuItem));
    #endif
        DrawPropStr(PTR_GET_MI_XPOS(pstMenuItem), (PTR_GET_MI_YPOS(pstMenuItem)+YPosShift), pstMenuItem->DisplayText());
    }
    if ((pstMenuItem->u8TXTFlags & TXT_OVER_256)||(pstMenuItem->u8TXTFlags & TXT_OVER_512))
        OSD_TEXT_HI_ADDR_CLR_TO_0();
}

//=========================================================================
// draw menu item display value
void DrawOsdMenuItemValue(BYTE u8ItemIndex, DrawValueType *pstValueItem)
{
    if (pstValueItem->pstDrawRadioGroup)
        DrawOsdMenuItemRadioGroup(u8ItemIndex, pstValueItem->pstDrawRadioGroup);
	
    if (pstValueItem->pstDrawIcons)
        DrawOsdMenuItemIcons(u8ItemIndex, pstValueItem->pstDrawIcons);

    if (pstValueItem->pstDrawNumber)
        DrawOsdMenuItemNumber(u8ItemIndex, pstValueItem->pstDrawNumber);

    if (pstValueItem->pstDrawGauge)
        DrawOsdMenuItemGauge(u8ItemIndex, pstValueItem->pstDrawGauge);
}

//=========================================================================
// draw number
void DrawOsdMenuItemNumber(BYTE u8ItemIndex, DrawNumberType *pstNumberItem)
{

#if USE_PROP_NUMBER
    {
        BYTE xdata tNumberText[2];
        BYTE xdata i=0;
        BYTE xdata XPOS=0;		
        BYTE xdata YPosShift=0;		
        //BYTE u8XTemp=PTR_GET_DN_XPOS(pstNumberItem);

    #define DRAW_ITEM   (pstNumberItem + i)
#if LED_HOT_Menu_Func	
if((g_u8MenuPageIndex==MENU_SYSTEM)&&(u8ItemIndex>=Sys_Language_IDX)&&(g_stMenuFlag.bEnableItemSelect==1))
    YPosShift=4;
else 
#endif
if((g_u8MenuPageIndex==MENU_COLOR)&&(u8ItemIndex>=Col_Gamma_IDX)&&(g_stMenuFlag.bEnableItemSelectSub==1))
    YPosShift=6;
else
    YPosShift=0;

        while(1)
        {
        #if 0//defined(_HIDE_GRAYOUT_ITEM_) || defined(_L2_USER_PREF_SETTING_HIDDEN_)
            if (!(IS_MAIN_L1(g_u8MenuPageIndex) && IsUserPrefValueGrayout(DRAW_ITEM->GetValue)))
        #endif
            {
                g_u8OsdFontColor=pstNumberItem->u8Color;
 //   #define XPOS    u8ItemIndex
                XPOS=PTR_GET_DN_XPOS(DRAW_ITEM);
                if (XPOS== 0)
                {
                    XPOS = (g_u8OsdWindowWidth - 4) / 2 - 1;
                }
                if (DRAW_ITEM->u8Flags & NUM_DRAW_ARROW)
                {

                 if((g_u8MenuPageIndex==MENU_COLOR)&&(g_stMenuFlag.bEnableItemSelect==1))
                 {
                  if(0);
			//if(((g_u8MenuPageIndex==MENU_COLOR)&&(g_stMenuFlag.bEnableItemSelect==1))&&(u8ItemIndex==Advanced_Cyan_IDX||u8ItemIndex==Advanced_Magenta_IDX||u8ItemIndex==Advanced_Yellow_IDX)&&(CURRENT_COLOR_TEMP!=PCT_Advanced))	   
			//   g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_MainGround);	
		#if ENABLE_VGA_INPUT	
		       else if((g_u8MenuPageIndex==MENU_COLOR)&&(u8ItemIndex>=Pic_AutoAdjust_IDX))
			   g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_Black);		
		#endif	   
                     else if(IsUserPrefValueGrayout(DRAW_ITEM->GetValue))
			   g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_MainGround);			
			else
			   g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_MainGround);
                 }
		 else
		   {
                  if (IS_MAIN_L1(g_u8MenuPageIndex))
                    {
                      if(IsUserPrefValueGrayout(DRAW_ITEM->GetValue))
                      {
                       if((g_u8MenuPageIndex==MENU_QUICKSTART)||((g_u8MenuPageIndex==MENU_PICTURE)&&(u8ItemIndex<=Pic_Contrast_Adj_IDX)))
			   g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_MainGround);		
			  else if((g_u8MenuPageIndex==MENU_COLOR)&&(u8ItemIndex>=Col_Gamma_IDX))		   
			   g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_MainGround);		
			  else		   
			   g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_Black);			
                      }
			else if(g_u8MenuItemIndex==u8ItemIndex)
			   g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_Frame);		
			else
			   g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_MainGround);		
                    }

                 if((IS_MAIN_L1(g_u8MenuPageIndex))&&(g_u8MenuItemIndex%2==1)&&(g_stMenuFlag.bThisItemSelected==0))
                   {
                     OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8
                     Osd_DrawRealChar(XPOS-2, PTR_GET_DN_YPOS(DRAW_ITEM), ARROW_LEFT); 
                     Osd_DrawRealChar(XPOS+3, PTR_GET_DN_YPOS(DRAW_ITEM), ARROW_RIGHT); 
			OSD_TEXT_HI_ADDR_CLR_TO_0();  
                   }
		    }
                }
                tNumberText[1]=LoadPropNumber(DRAW_ITEM->u8PropNumAddr, DRAW_ITEM->GetValue(),DRAW_ITEM->u8Flags);
                tNumberText[0]=DRAW_ITEM->u8PropNumAddr;
                Osd_DrawContinuousChar(XPOS, (PTR_GET_DN_YPOS(DRAW_ITEM)+YPosShift), SPACE_FONT, PROP_NUM_LEN);//clear
                DrawPropStr(XPOS, (PTR_GET_DN_YPOS(DRAW_ITEM)+YPosShift),tNumberText );
            }
            if (DRAW_ITEM->u8Flags & NUM_END)
                break;
            i++;
        }
//    #undef XPOS
    #undef DRAW_ITEM
    }
#else
    #define XPOS    u8ItemIndex

    XPOS=pstNumberItem->u8XPos;
    g_u8OsdFontColor=pstNumberItem->u8Color;

    if (XPOS == 0)
    {
        XPOS = (g_u8OsdWindowWidth - 4) / 2 - 1;
    }

    Osd_DrawNum(XPOS, pstNumberItem->u8YPos, pstNumberItem->GetValue());
#undef XPOS
#endif
}

//=========================================================================
//Steven110531
#define GAUGE_FONT_BAR_HL       (MM_4COLOR_CURSOR_START+4*2)
#define GAUGE_FONT_BAR_BODY      (COMMON_FONT_START+0x0A)


void DrawOsdMenuItemGauge(BYTE u8ItemIndex, DrawGaugeType *pstGaugeItem)  //Steven110531
{
	BYTE leadPtr,XPOS,YPOS,XPOSShift=0;
 	WORD tempValue;
	BYTE value;
	BYTE length;
	BYTE j=0;

UNUSED(u8ItemIndex);
#define DRAW_GITEM (pstGaugeItem + j)

while(1)
{
	value=DRAW_GITEM->GetValue();
	length=DRAW_GITEM->u8Length;
	YPOS = PTR_GET_DG_YPOS(DRAW_GITEM);
	XPOS = PTR_GET_DG_XPOS(DRAW_GITEM);

	if (value>100)
		value=100;
// if(g_u8MenuItemIndex==u8ItemIndex)
	g_u8OsdFontColor=MONO_COLOR(CP_Black, CP_BK_COLOR_SEL);	
// else
// 	g_u8OsdFontColor=MONO_COLOR(CP_Black, CP_BK_COLOR_H);	
       OSD_TEXT_HI_ADDR_SET_BIT8();			
	Osd_DrawRealChar(XPOS+1, YPOS, GAUGE_FONT_BAR_BODY);
	Osd_DrawRealChar(XPOS+1+length-1, YPOS, GAUGE_FONT_BAR_BODY+1);
 	tempValue=(WORD)YPOS*10+(XPOS+1); 
       OSD_TEXT_HI_ADDR_CLR_TO_0();	
  	length-=2;	
  	leadPtr=(WORD)value*length/10;
   	

   #define i	value
  	tempValue=leadPtr/10;
   
  	if (tempValue==length && value<100)
    	tempValue--;

// if(g_u8MenuItemIndex==u8ItemIndex)
	g_u8OsdFontColor=MONO_COLOR(CP_Black, CP_BK_COLOR_SEL);	
// else
// 	g_u8OsdFontColor=MONO_COLOR(CP_Black, CP_BK_COLOR_H);	
    OSD_TEXT_HI_ADDR_SET_BIT8();		
  	for (i=1; i<=tempValue; i++) 
    {
		Osd_DrawRealChar(XPOS+1+i, YPOS, GAUGE_FONT_BAR_BODY+2);//6
    }
  	OSD_TEXT_HI_ADDR_CLR_TO_0();	
  	i=i+1;
  	if (i<=length) 
    { 	
	   	tempValue=(WORD)YPOS*10+(XPOS+i); 
	g_u8OsdFontColor=MONO_COLOR(CP_Black, CP_BK_COLOR_H);	
	OSD_TEXT_HI_ADDR_SET_BIT8();		
      	for (; i<=length; i++)
        {
			Osd_DrawRealChar(XPOS+1+i, YPOS, GAUGE_FONT_BAR_BODY+2);
     	}
	OSD_TEXT_HI_ADDR_CLR_TO_0();		
    }
   #undef i
   
  	if (leadPtr<length*10)
    { 	value=leadPtr%10;
      	XPOSShift+=leadPtr/10+1;
      	leadPtr=((WORD)value*12+5)/10;
	//printData("----------leadPtr==%d",leadPtr);
	//printData("----------value==%d",value);
      	if (leadPtr<3 && value)
      		leadPtr=3;
       if(DRAW_GITEM->GetValue()==0)
       {
	 g_u8OsdFontColor=MONO_COLOR(CP_Black, CP_BK_COLOR_H);		
	 OSD_TEXT_HI_ADDR_SET_BIT8();		
	 Osd_DrawRealChar(XPOS+1+XPOSShift, YPOS, GAUGE_FONT_BAR_BODY+2);
	 OSD_TEXT_HI_ADDR_CLR_TO_0();	
       }
	else
	{
       OSD_TEXT_HI_ADDR_SET_BIT9();		
	g_u8OsdFontColor=FOUR_COLOR(40);	
      	Osd_DrawRealChar(XPOS+1+XPOSShift, YPOS, (GAUGE_FONT_BAR_HL+(leadPtr/3)*2));
       OSD_TEXT_HI_ADDR_CLR_TO_0();	
	}
  	}

	if(DRAW_GITEM->u8Flags & BAR_END)
	   break;
	j++;
}
}



//=========================================================================
// draw radio text
void DrawOsdMenuItemRadioGroup(BYTE u8ItemIndex, DrawRadioGroupType *pstRadioItem)
{
    BYTE  xdata  i = 0;
    BYTE  xdata  YPosShift = 0;

    if ((pstRadioItem->pstRadioText)==NULL)
        return;
#if LED_HOT_Menu_Func	
if((g_u8MenuPageIndex==MENU_SYSTEM)&&(u8ItemIndex>=Sys_Language_IDX)&&(g_stMenuFlag.bEnableItemSelect==1))
    YPosShift=4;
else 
#endif
if((g_u8MenuPageIndex==MENU_COLOR)&&(u8ItemIndex>=Col_Gamma_IDX)&&(g_stMenuFlag.bEnableItemSelectSub==1))
    YPosShift=6;
else
    YPosShift=0;


    if((g_u8MenuPageIndex==MENU_SYSTEM
#if LED_HOT_Menu_Func
	||g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER
#endif
    )&&(u8ItemIndex==Sys_OSDTransparencia_IDX||(u8ItemIndex==Sys_OSDTransparencia_Adj_IDX)))
    {
          LoadPercentPropFont();
    }


#define DRAW_ITEM   (pstRadioItem->pstRadioText + i)

    while (1)
    {
        if ((DRAW_ITEM->DisplayText) && (DRAW_ITEM->DisplayText()!=NULL)) ///Donot draw if return NULL
        {
            if ((DRAW_ITEM->u8Flags & TXT_OVER_256))
                OSD_TEXT_HI_ADDR_SET_BIT8(); //enable bit 8
            else if((DRAW_ITEM->u8Flags & TXT_OVER_512))     
                OSD_TEXT_HI_ADDR_SET_BIT9(); //enable bit 9

            {

                {
                     if(IsUserPrefOptionGrayout(DRAW_ITEM->DisplayText))
                     {
                       printData("-----end g_u8OsdFontColor==%x",g_u8OsdFontColor);
	                g_u8OsdFontColor=MONO_COLOR(CP_UnSel, CP_MainGround);	
                     }
                     else if (u8ItemIndex==g_u8MenuItemIndex)
                        g_u8OsdFontColor=pstRadioItem->u8SelColor;
                     else
                        g_u8OsdFontColor=pstRadioItem->u8Color;

		      if((DRAW_ITEM->u8Flags & TXT_CENTER)&&(DRAW_ITEM->u8Flags & TXT_ARROW))
                        DrawCenterDrawArrowText(PTR_GET_DT_XPOS(DRAW_ITEM), (PTR_GET_DT_YPOS(DRAW_ITEM)+YPosShift), DRAW_ITEM->DisplayText());
                    else if (DRAW_ITEM->u8Flags & TXT_CENTER)
                        DrawCenterText(PTR_GET_DT_XPOS(DRAW_ITEM), (PTR_GET_DT_YPOS(DRAW_ITEM)+YPosShift), DRAW_ITEM->DisplayText());
                    else if (DRAW_ITEM->u8Flags & TXT_FULL_CENTER)
                        DrawFullCenterText(PTR_GET_DT_XPOS(DRAW_ITEM), (PTR_GET_DT_YPOS(DRAW_ITEM)+YPosShift), DRAW_ITEM->DisplayText());
                    else if ((DRAW_ITEM->u8Flags& TXT_RealText))
                         DrawRealStr(PTR_GET_DT_XPOS(DRAW_ITEM), (PTR_GET_DT_YPOS(DRAW_ITEM)+YPosShift), DRAW_ITEM->DisplayText());
                #if 0//LED_HOT_Menu_Func    
		      else if(g_u8MenuPageIndex==MENU_HOTLEDMODE)
		      	{
                        DrawArrowCenterText(PTR_GET_DT_XPOS(DRAW_ITEM), PTR_GET_DT_YPOS(DRAW_ITEM), DRAW_ITEM->DisplayText());
		      	}
		 #endif		                       	
                   else
                    	{
                        DrawPropStr(PTR_GET_DT_XPOS(DRAW_ITEM), (PTR_GET_DT_YPOS(DRAW_ITEM)+YPosShift), DRAW_ITEM->DisplayText());
                    	}
                }
            }
            if ((DRAW_ITEM->u8Flags & TXT_OVER_256)||(DRAW_ITEM->u8Flags & TXT_OVER_512))
                OSD_TEXT_HI_ADDR_CLR_TO_0();
        }

        if (DRAW_ITEM->u8Flags & TXT_END)
            break;
        i++;
    }

#undef DRAW_ITEM
}


//=========================================================================
#if USB_Updata_FW_Enable	
void DrawOsdMenuItem_USB(void)
{
  BYTE i;
  if((g_u8MenuPageIndex==MENU_INPUT_SOURCE)&&(g_u8MenuItemIndex==SOURCE_USB_UPDATA))
  {

    for (i = 0; i < 8; i++)
       Osd_DrawRealChar(MM_XSTART_L2+1+i, 3+(SOURCE_USB_UPDATA*2), SPACE_FONT);

      DrawOsdMenuItemValue(g_u8MenuItemIndex, &CURRENT_MENU_ITEM.stDisplayValue);
  }
}
#endif

//=========================================================================
void DrawOsdMenuItemIcons(BYTE u8ItemIndex,DrawIconType* pstIcons)
{
    int xdata i=0;

    if (pstIcons==NULL)
        return;
#define DRAW_ITEM   (pstIcons + i)
    while(1)
    {
        if (DRAW_ITEM->GetIcon)
        {
            if(IS_WARNING_MENU(g_u8MenuPageIndex)||IS_MSG_STATUS(g_u8MenuPageIndex))
                DrawIcon(PTR_GET_DI_XPOS(DRAW_ITEM),PTR_GET_DI_YPOS(DRAW_ITEM),DRAW_ITEM->GetIcon(),DRAW_ITEM->u8Flags, CS_SELECTED);
            else if (u8ItemIndex==g_u8MenuItemIndex)
            {
                DrawIcon(PTR_GET_DI_XPOS(DRAW_ITEM),PTR_GET_DI_YPOS(DRAW_ITEM),DRAW_ITEM->GetIcon(),DRAW_ITEM->u8Flags, CS_HIGHLIGHT);
            }
            else
                DrawIcon(PTR_GET_DI_XPOS(DRAW_ITEM),PTR_GET_DI_YPOS(DRAW_ITEM),DRAW_ITEM->GetIcon(),DRAW_ITEM->u8Flags, CS_NORMAL);
        }
        if (DRAW_ITEM->u8Flags & ICO_END)
            break;
        i++;
    }
#undef DRAW_ITEM
}

//=========================================================================
#if 1 //Steven110609
Bool IsUserPrefValueGrayout(fpGetValue fpValue)
{
    if (fpValue==NULL)
        return FALSE;
#if ENABLE_DIGITAL_INPUT && ENABLE_VGA_INPUT
    if (((fpValue == GetHPositionValue)
        || (fpValue == GetVPositionValue)
        || (fpValue == GetClockValue)
        || (fpValue == GetPhaseValue)
        ))
    	{
    	/*
    if ((CURRENT_INPUT_IS_VGA())
#if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&PXP_VGA_Auto_Enable)		
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))||((USER_PREF_INPUT_TYPE_SUBWIN3==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN3)))))
#endif
	)	
	*/
	if(CURRENT_INPUT_IS_VGA())
        return FALSE;
	else
        return TRUE;
    	}
#endif
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (((fpValue==GetPIPHPositionValue) || (fpValue==GetPIPVPositionValue)) && (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_2_PIP))
        return TRUE;
#endif
    if ((fpValue==GetBrightnessValue||fpValue==GetrgbBrightnessValue||fpValue==GetContrastValue) && ((CURRENT_PICTURE_MODE!=CT_USER)||DCR_ENABLE_FLAG))
        return TRUE;

	if((fpValue==GetUserRedColorValue||fpValue==GetUserGreenColorValue||fpValue==GetUserBlueColorValue)&&(CURRENT_COLOR_TEMP!=PCT_USER)/*&&(CURRENT_COLOR_TEMP!=PCT_Advanced)*/)
	 return TRUE;
	
// 	if((fpValue==GetCustomColorValue_R||fpValue==GetCustomColorValue_G||fpValue==GetCustomColorValue_B)&&(CURRENT_COLOR_TEMP!=PCT_Advanced))
// 	 return TRUE;

#if Low_Blue_Light_Enable
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if ((fpValue==GetLowblurayValue_Number) && (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF))
        return TRUE;
#endif	
#endif

#if 1
    if ((fpValue==GetHueValue||fpValue==GetSaturationValue) && (CURRENT_COLOR_TEMP==PCT_USER))
        return TRUE;
#endif	



    return FALSE;
}
#endif

#if 1
Bool IsUserPrefOptionGrayout(fpDisplayText fpStr)
{
    if (!fpStr)
        return FALSE;

	if((g_u8MenuPageIndex==MENU_COLOR)&&((CURRENT_COLOR_TEMP!=PCT_USER)&&(fpStr==TxtSystemLedModeIcon)))
	{
		return TRUE;
	}

 #if Free_Sync_Enable
	if((((g_u8MenuPageIndex==MENU_QUICKSTART)&&(fpStr==TxtQuickFreeSyncOnOff))||((g_u8MenuPageIndex==MENU_SYSTEM
   #if LED_HOT_Menu_Func
	||g_u8MenuPageIndex==MENU_SYSTEMLLEDUSER
  #endif
	)&&(fpStr==TxtSystemFreeSyncStatus))))
	{
	   if((INPUT_IS_DISPLAYPORT(MapWin2Port(USER_PREF_WIN_SEL))||(INPUT_IS_HDMI_2_0(MapWin2Port(USER_PREF_WIN_SEL)))
   #if EDID_1920x1080_144
        ||(INPUT_IS_HDMI(MapWin2Port(USER_PREF_WIN_SEL)))
   #endif     
	   )
   #if ENABLE_MULTI_WINDOW_SETTINGS_MENU		
      &&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
   #endif   
	 )
		return FALSE;
          else
		return TRUE;
	}
 #endif

 	if((g_u8MenuPageIndex==MENU_PICTURE)&&(((CURRENT_PICTURE_MODE==CT_MOVIE)||(CURRENT_PICTURE_MODE==CT_VIVID)||(CURRENT_PICTURE_MODE==CT_FPS)||(CURRENT_PICTURE_MODE==CT_RTS))&&(fpStr==TxtPictureDCRStatus)))
	{
		return TRUE;
	}

  
    return FALSE;
}
#endif

Bool IsMenuItemSelectable(WORD u16Flags, BYTE ItemIndex)
{
    if (IsItemHidden(u16Flags) || IsItemGrayed(u16Flags, ItemIndex))
        return FALSE;
    if (u16Flags & MIB_SELECTABLE)
        return TRUE;
    return FALSE;
}
BYTE GetPrevItem(MenuItemType *pstMenuItem)
{
    signed char i;

    if (g_u8MenuItemIndex)
    {
        for (i = g_u8MenuItemIndex - 1; i >= 0; i--)
        {
            if (IsMenuItemSelectable(pstMenuItem[i].u16Flags, i))
                return i;
        }
    }
/// Last item or can't find selectable item following the pstMenuItem.
    for (i = MENU_ITEM_COUNT - 1; i >= 0; i--)
    {
        if (IsMenuItemSelectable(pstMenuItem[i].u16Flags, i))
            return i;
    }

    return g_u8MenuItemIndex;
}

/// Get the next visible/selectable item on the current menu page.
BYTE GetNextItem(MenuItemType *pstMenuItem)
{
    BYTE    i;

    if (g_u8MenuItemIndex < MENU_ITEM_COUNT - 1)    // current item is not last item.
    {
        for (i = g_u8MenuItemIndex + 1; i < MENU_ITEM_COUNT; i++)
        {
            if (IsMenuItemSelectable(pstMenuItem[i].u16Flags, i))
                return i;
        }
    }
/// Last item or can't find selectable item following the pstMenuItem.
    for (i = 0; i < g_u8MenuItemIndex; i++)
    {
        if (IsMenuItemSelectable(pstMenuItem[i].u16Flags, i))
            return i;
    }

    return g_u8MenuItemIndex;
}

BYTE GET_Source_Index(void)
{
#if (MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)
      if(USER_PREF_INPUT_TYPE==INPUT_VGA)
        return 0;
      else if(USER_PREF_INPUT_TYPE==INPUT_DVI)	  
	 return 1;
      else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)	  
	 return 2;
      else if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)	  
	 return 3;
       else	  
        return 0;
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
      if(USER_PREF_INPUT_TYPE==INPUT_VGA)
        return 0;
      else if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)	  
	 return 1;
      else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)	  
	 return 2;
      else if(USER_PREF_INPUT_TYPE==INPUT_HDMI2ND)	  
	 return 3;
      else if(USER_PREF_INPUT_TYPE==INPUT_HDMI3ND)	  
	 return 4;
       else	  
        return 0;
#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7||MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1||MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
	 return USER_PREF_INPUT_TYPE;
/*
       if(USER_PREF_INPUT_TYPE==INPUT_DVI)	  
	 return 0;
      else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)	  
	 return 1;
      else if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)	  
	 return 2;
       else	  
        return 0;
*/
#else
      if(USER_PREF_INPUT_TYPE==INPUT_VGA)
        return 0;
      else if(USER_PREF_INPUT_TYPE==INPUT_DVI)	  
	 return 1;
      else if(USER_PREF_INPUT_TYPE==INPUT_HDMI)	  
	 return 2;
      else if(USER_PREF_INPUT_TYPE==INPUT_DISPLAYPORT)	  
	 return 3;
       else	  
        return 0;

#endif
}

BYTE GetMenuItemIndex(BYTE u8PrevMenuPageIndex)
{

#if Enable_Corss_LHCX_New
    if (u8PrevMenuPageIndex==MENU_ROOT)
    	{
		if(g_u8MenuPageIndex==HotKeyGameModeMenu)
		{
			return 0;
		}
    	}
    if (u8PrevMenuPageIndex==HotKeyGameModeMenu)
   	HotGameModeItemIndex=g_u8MenuItemIndex;
   else if (u8PrevMenuPageIndex==HotKeyGameSubSelectMenu||u8PrevMenuPageIndex==HotKeyGameSelectMenu)//
   	HotGameSelectItemIndex=g_u8MenuItemIndex;
   else if (u8PrevMenuPageIndex==HotKeyGameClokMenu||u8PrevMenuPageIndex==HotKeyGameClokShowMenu)
   	HotGameClokItemIndex=g_u8MenuItemIndex;
   else if (u8PrevMenuPageIndex==HotKeyGameClokPositionMenu)
   	HotGameClokPositionItemIndex=g_u8MenuItemIndex;
#endif

    if (g_u8MenuPageIndex == MENU_MAIN)
    {
    #if LED_HOT_Menu_Func
        if(u8PrevMenuPageIndex==MENU_SYSTEMLLEDUSER)
            return (MENU_SYSTEM-MENU_QUICKSTART);
        else 
    #endif    
	 if (IS_MAIN_L1(u8PrevMenuPageIndex))
        {
            return (u8PrevMenuPageIndex-MENU_QUICKSTART);
        }
        else
            return 0;
    }
   else if(g_u8MenuPageIndex == MENU_INPUT_SOURCE||g_u8MenuPageIndex==MENU_HOTINPUT)	
   {
  #if Enable_AutoInput_Menu
         return USER_PREF_INPUT_PRIORITY;
   #else
         return GET_Source_Index();
   #endif
   
   }
#if Enable_Corss_LHCX_New
    else if (g_u8MenuPageIndex==HotKeyGameModeMenu)
    {
    	if(HotGameModeItemIndex>=2)
    		HotGameModeItemIndex=0;
    	return HotGameModeItemIndex;
    }
    else if (g_u8MenuPageIndex==HotKeyGameSubSelectMenu||g_u8MenuPageIndex==HotKeyGameSelectMenu)
    {
    	if(HotGameSelectItemIndex>=4)
    		HotGameSelectItemIndex=0;
    	return HotGameSelectItemIndex;
    }
    else if (g_u8MenuPageIndex==HotKeyGameClokMenu||g_u8MenuPageIndex==HotKeyGameClokShowMenu)
    {
    	if(HotGameClokItemIndex>=5)
    		HotGameClokItemIndex=0;
    	return HotGameClokItemIndex;
    }
    else if (g_u8MenuPageIndex==HotKeyGameClokPositionMenu)
    {
    	if(HotGameClokPositionItemIndex>=3)
    		HotGameClokPositionItemIndex=0;
    	return HotGameClokPositionItemIndex;
    }
#endif
   else	
        g_u8MenuItemIndex=0;
	
    if (!IsMenuItemSelectable(CURRENT_MENU_ITEM.u16Flags, g_u8MenuItemIndex))
        g_u8MenuItemIndex = GetNextItem(CURRENT_MENU_ITEMS);
    return g_u8MenuItemIndex;
}

void SaveUserPrefSetting(BYTE u8MenuPageIndex)
{
    MENU_printMsg("SaveUserPrefSetting");
    MENU_printData("SaveUserPrefSetting==%x",g_u8SaveUserData );
    u8MenuPageIndex = u8MenuPageIndex;
    if (g_u8SaveUserData&SAVE_MONITOR_SETTING)
        SaveMonitorSetting();
#if ENABLE_VGA_INPUT
    if (g_u8SaveUserData&SAVE_MODE_SETTING)
        SaveModeSetting();
#endif
    if (g_u8SaveUserData&SAVE_FACTORY_SETTING)
        SaveFactorySetting();
    //ClearOSDFlag(OSD_SAVE_SETTING_BIT);
    g_u8SaveUserData=SAVE_NONE;
}

#if 0  //Steven110603
#define TBX_POS ((FLY_WND_XSIZE-MSG_TIME_BAR_LENGTH)>>1)
#define TBY_POS 8
#define TIMEBAR_FONT0     MSG_TIME_BAR_MONO_START
#define TIMEBAR_FONT1     MSG_TIME_BAR_4COLOR_START
#define TIMEBAR_FONT2     MSG_TIME_BAR_4COLOR_START+2
#define TIMEBAR_FONT3     MSG_TIME_BAR_4COLOR_START+4
#define TIMEBAR_FONT4     MSG_TIME_BAR_4COLOR_START+6
#define TIMEBAR_FONT5     MSG_TIME_BAR_4COLOR_START+8
#define TIMEBAR_FONT6     MSG_TIME_BAR_4COLOR_START+10
#define TIMEBAR_FONT7     MSG_TIME_BAR_4COLOR_START+12
#define TIMEBAR_FONT8     MSG_TIME_BAR_4COLOR_START+14
#define TIMEBAR_FONT9     MSG_TIME_BAR_4COLOR_START+16
#define TIMEBAR_FONT10    MSG_TIME_BAR_4COLOR_START+18
#define TIMEBAR_FONT11    MSG_TIME_BAR_4COLOR_START+20
#define TIMEBAR_FONT12    MSG_TIME_BAR_MONO_START
#define TB_PIXEL_STEP       1
void DrawTimeBar(WORD u16Value)
{
    WORD u16TotalPixel;
    BYTE code u8Index[]={TIMEBAR_FONT11,TIMEBAR_FONT10,TIMEBAR_FONT9,TIMEBAR_FONT8,TIMEBAR_FONT7,TIMEBAR_FONT6,TIMEBAR_FONT5,TIMEBAR_FONT4,TIMEBAR_FONT3,TIMEBAR_FONT2,TIMEBAR_FONT1};

    //PRINT_DATA("[OSD]u8Value = %d", u16Value);
    u16TotalPixel=((DWORD)u16Value*MSG_TIME_BAR_LENGTH*12)/300; //Total pixels
    //PRINT_DATA("[OSD]u16TotalPixel = %d", u16TotalPixel);
#define i   u16Value
    g_u8OsdFontColor=MONO_COLOR(CP_CURSOR_TOP, CP_MSG_BK);
    u16TotalPixel=(u16TotalPixel/TB_PIXEL_STEP)*TB_PIXEL_STEP;
    WAIT_V_OUTPUT_BLANKING();
    for (i=0; i<((DWORD)MSG_TIME_BAR_LENGTH*12-u16TotalPixel)/12; i++) //Draw Full
        Osd_DrawRealChar(TBX_POS+i,TBY_POS,TIMEBAR_FONT0);
    u16TotalPixel=(u16TotalPixel%12)/TB_PIXEL_STEP;
    //PRINT_DATA("[OSD]Rest = %d", u16TotalPixel);
    //PRINT_DATA("[OSD]i = %d", i);
    if (u16TotalPixel)
    {
        g_u8OsdFontColor=FOUR_COLOR(12);
        OSD_TEXT_HI_ADDR_SET_BIT9(); //enable bit 9
        Osd_DrawRealChar(TBX_POS+i, TBY_POS, u8Index[u16TotalPixel-1]);
        OSD_TEXT_HI_ADDR_CLR_TO_0(); //disable bit 9
        i++;
    }
    //PRINT_DATA("[OSD]i = %d", i);

    g_u8OsdFontColor=MONO_COLOR(CP_GRAY3,CP_MSG_BK);
    for (; i<MSG_TIME_BAR_LENGTH; i++)    //Draw empty
    {
        Osd_DrawRealChar(TBX_POS+i,TBY_POS,TIMEBAR_FONT12);
    }
#undef i
}
#endif

#define FLY_STEP    10
void MoveOsdWindow(void)
{
    if (g_stMenuFlag.bOsdReverseX)
    {
        if (u16MoveX <= Osd_MenuHPosition(0,FALSE))
        {
            g_stMenuFlag.bOsdReverseX=0;
            u16MoveX+=FLY_STEP;
        }
        else
        {
            if (u16MoveX>FLY_STEP)
                u16MoveX-=FLY_STEP;
            else
                u16MoveX=Osd_MenuHPosition(0,FALSE);
        }
    }
    else
    {
        if (u16MoveX >= Osd_MenuHPosition(100,FALSE))
        {
            g_stMenuFlag.bOsdReverseX=1;
            u16MoveX-=FLY_STEP;
        }
        else
        {
            if (u16MoveX+FLY_STEP<Osd_MenuHPosition(100,FALSE))
                u16MoveX+=FLY_STEP;
            else
                u16MoveX=Osd_MenuHPosition(100,FALSE);
        }
    }
//-------------------------------------------------
    if (g_stMenuFlag.bOsdReverseY)
    {
        if (u16MoveY <= Osd_MenuVPosition(0,FALSE))
        {
            g_stMenuFlag.bOsdReverseY=0;
            u16MoveY+=FLY_STEP;
        }
        else
        {
            if (u16MoveY>FLY_STEP)
                u16MoveY-=FLY_STEP;
            else
                u16MoveY=Osd_MenuVPosition(0,FALSE);
        }
    }
    else
    {
        if (u16MoveY >= Osd_MenuVPosition(100,FALSE))
        {
            g_stMenuFlag.bOsdReverseY=1;
            u16MoveY-=FLY_STEP;
        }
        else
        {
            if (u16MoveY+FLY_STEP<Osd_MenuVPosition(100,FALSE))
                u16MoveY+=FLY_STEP;
            else
                u16MoveY=Osd_MenuVPosition(100,FALSE);
        }
    }
    SetOsdPositionReg(u16MoveX, u16MoveY);
    //LoadTimePropNumber(MSG_NOCABLE_TIMER_START, g_u16OsdCounter); //FY12_MSG
    //DrawTimeBar(g_u16OsdCounter);  //Steven110609
}

void Menu_ClearOsdCounterAndFlag(void)
{
    g_stMenuFlag.bOsdTimeout=0;
    g_u16OsdCounter=0;
}

#if (USE_ADJUSTABLE_MENU && ENABLE_MSTV_UART_DEBUG && MENU_TBL_CHECK)
Bool DoMenuTable_XXX_FUNC_FlagCheck(MenuPageType *pstMenuTbl, BYTE u8TblSize)
{
    BYTE g_u8MenuPageIndex,g_u8MenuItemIndex,i;
    Bool Rlt=TRUE;
#define MENU_PAGE_FLAG      pstMenuTbl[g_u8MenuPageIndex].u8Flags
#define MENU_PAGE_CHECK     pstMenuTbl[g_u8MenuPageIndex].unPos.stCXY.u8Check
#define MENU_ITEM_TXT_FLAG  pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].u8TXTFlags
#define MENU_ITEM_TXT_CHECK pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].unPos.stCXY.u8Check
#define NUM_PTR             pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawNumber
#define NUM_FLAG            pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawNumber[i].u8Flags
#define NUM_CHECK           pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawNumber[i].unPos.stCXY.u8Check
#define BAR_PTR             pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawGauge
#define BAR_FLAG            pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawGauge[i].u8Flags
#define BAR_CHECK           pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawGauge[i].unPos.stCXY.u8Check
#define TXT_PTR             pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawRadioGroup
#define TXT_FLAG            pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawRadioGroup[0].pstRadioText[i].u8Flags
#define TXT_CHECK           pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawRadioGroup[0].pstRadioText[i].unPos.stCXY.u8Check
#define ICO_PTR             pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawIcons
#define ICO_FLAG            pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawIcons[i].u8Flags
#define ICO_CHECK           pstMenuTbl[g_u8MenuPageIndex].pstMenuItems[g_u8MenuItemIndex].stDisplayValue.pstDrawIcons[i].unPos.stCXY.u8Check

    for (g_u8MenuPageIndex=0;g_u8MenuPageIndex<u8TblSize;g_u8MenuPageIndex++)
    {
        if (((MENU_PAGE_FLAG&MPB_FUNC) && MENU_PAGE_CHECK!=0xFF)
            || (!(MENU_PAGE_FLAG&MPB_FUNC) && MENU_PAGE_CHECK==0xFF))
        {
            MENU_printData("MenuPageType Flag ERROR, g_u8MenuPageIndex=%d",g_u8MenuPageIndex);
            Rlt=FALSE;
        }
        for (g_u8MenuItemIndex=0;g_u8MenuItemIndex<pstMenuTbl[g_u8MenuPageIndex].u8MenuItemCount;g_u8MenuItemIndex++)
        {
            if (((MENU_ITEM_TXT_FLAG&TXT_FUNC) && MENU_ITEM_TXT_CHECK!=0xFF)
                || (!(MENU_ITEM_TXT_FLAG&TXT_FUNC) && MENU_ITEM_TXT_CHECK==0xFF))
            {
                MENU_printData(">g_u8MenuPageIndex=%d",g_u8MenuPageIndex);
                MENU_printData(">MenuItemType Flag ERROR, MenuItemIndex=%d",g_u8MenuItemIndex);
                Rlt=FALSE;
            }
            if (NUM_PTR)
            {
                i=0;
                while(1)
                {
                    if (((NUM_FLAG&NUM_FUNC) && NUM_CHECK!=0xFF) || (!(NUM_FLAG&NUM_FUNC) && NUM_CHECK==0xFF))
                    {
                        MENU_printData(">g_u8MenuPageIndex=%d",g_u8MenuPageIndex);
                        MENU_printData(">MenuItemIndex=%d",g_u8MenuItemIndex);
                        MENU_printData(">DrawNumberType Flag ERROR, index=%d",i);
                        Rlt=FALSE;
                    }
                    if (NUM_FLAG&NUM_END)
                        break;
                    i++;
                }
            }
            if (BAR_PTR)
            {
                i=0;
                while(1)
                {
                    if (((BAR_FLAG&BAR_FUNC) && BAR_CHECK!=0xFF) || (!(BAR_FLAG&BAR_FUNC) && BAR_CHECK==0xFF))
                    {
                        MENU_printData(">g_u8MenuPageIndex=%d",g_u8MenuPageIndex);
                        MENU_printData(">MenuItemIndex=%d",g_u8MenuItemIndex);
                        MENU_printData(">DrawGaugeType Flag ERROR, index=%d",i);
                        Rlt=FALSE;
                    }
                    if (BAR_FLAG&BAR_END)
                        break;
                    i++;
                }
            }
            if (TXT_PTR)
            {
                i=0;
                while(1)
                {
                    if (((TXT_FLAG&TXT_FUNC) && TXT_CHECK!=0xFF) || (!(TXT_FLAG&TXT_FUNC) && TXT_CHECK==0xFF))
                    {
                        MENU_printData(">g_u8MenuPageIndex=%d",g_u8MenuPageIndex);
                        MENU_printData(">MenuItemIndex=%d",g_u8MenuItemIndex);
                        MENU_printData(">DrawRadioGroupType Flag ERROR, index=%d",i);
                        Rlt=FALSE;
                    }
                    if (TXT_FLAG&TXT_END)
                        break;
                    i++;
                }
            }
            if (ICO_PTR)
            {
                i=0;
                while(1)
                {
                    if (((ICO_FLAG&ICO_FUNC) && ICO_CHECK!=0xFF) || (!(ICO_FLAG&ICO_FUNC) && ICO_CHECK==0xFF))
                    {
                        MENU_printData(">g_u8MenuPageIndex=%d",g_u8MenuPageIndex);
                        MENU_printData(">MenuItemIndex=%d",g_u8MenuItemIndex);
                        MENU_printData(">DrawIconType Flag ERROR, index=%d",i);
                        Rlt=FALSE;
                    }
                    if (ICO_FLAG&ICO_END)
                        break;
                    i++;
                }
            }
        }
    }
#undef MENU_PAGE_FLAG
#undef MENU_PAGE_CHECK
#undef MENU_ITEM_TXT_FLAG
#undef MENU_ITEM_TXT_CHECK
#undef NUM_PTR
#undef NUM_FLAG
#undef NUM_CHECK
#undef BAR_PTR
#undef BAR_FLAG
#undef BAR_CHECK
#undef TXT_PTR
#undef TXT_FLAG
#undef TXT_CHECK
#undef ICO_PTR
#undef ICO_FLAG
#undef ICO_CHECK
    return Rlt;
}
#endif


