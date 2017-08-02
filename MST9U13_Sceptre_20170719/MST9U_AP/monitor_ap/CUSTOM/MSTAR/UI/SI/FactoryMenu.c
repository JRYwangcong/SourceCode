///////////////////////////////////////////////////////////////////////////////
/// @file FactoryMenu.c
/// @brief
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#include "datatype.h"
#include "FactoryMenu.h"
#include "Board.h"
#include "Global.h"
#include "Keypaddef.h"
#include "Keypad.h"
#include "Ms_rwreg.h"
#include "Menu.h"
#include "ColorPalette.h"
#include "FMenuLoadFont.h"
#include "FMenuFunc.h"
#include "FMenuTBL.h"
#include "Adjust.h"
#include "msOSD.h"
#include "misc.h"
#include "ms_reg.h"
#include "MDebug.h"
#include "UserPref.h"
#include "NVRam.h"
#include "Panel.h"
#include "Power.h"
#include "MenuPropStr.h"
#include "PropFontExtFunc.h"
#include "LoadPropFont1218.h"
#include "appWindow.h"

#define FMENU_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&FMENU_DEBUG
    #define FMENU_printData(str, value)   printData(str, value)
    #define FMENU_printMsg(str)           printMsg(str)
#else
    #define FMENU_printData(str, value)
    #define FMENU_printMsg(str)
#endif

#define CURRENT_F_MENU            tblFMenus[g_u8MenuPageIndex&0x7F]
#define PREV_F_MENU_PAGE          CURRENT_F_MENU.u8PrevMenuPage
#define CURRENT_F_MENU_ITEMS      CURRENT_F_MENU.pstMenuItems
#define F_MENU_ITEM_COUNT         CURRENT_F_MENU.u8MenuItemCount

#define CURRENT_F_MENU_ITEM       CURRENT_F_MENU.pstMenuItems[g_u8MenuItemIndex]
#define NEXT_F_MENU_PAGE          CURRENT_F_MENU_ITEM.u8NextMenuPage
#define CURRENT_F_MENU_ITEM_FUNC  CURRENT_F_MENU_ITEM.stKeyFunction

//Bool FactoryExecuteKeyEvent(MenuItemActionType enMenuAction);
void FactoryDrawOsdMenu(void);
void FactoryDrawOsdMenuItemNumber(BYTE u8ItemIndex, DrawNumberType *pstNumberItem);
void FactoryDrawOsdMenuItemRadioGroup(BYTE u8ItemIndex, DrawRadioGroupType *pstRadioItem);
void FactoryDrawOsdMenuItemValue(BYTE u8ItemIndex, DrawValueType *pstValueItem);
void FactoryDrawAutoResult(BYTE u8Result);
BYTE FactoryGetPrevItem(MenuItemType *pstMenuItem);
BYTE FactoryGetNextItem(MenuItemType *pstMenuItem);
void FactoryDrawOsdMenuItem(BYTE u8ItemIndex, MenuItemType *pstMenuItem);
extern Bool AutoConfig(void);
void BurnInPatternHandler(BYTE Pattern);
 void DrawBackLiteTime(void);
void LoadCommonFont(void);

BYTE xdata OSDMoveHstart=0; 
BYTE xdata OSDMoveVstart=0; 
extern BYTE xdata u8RetValueL;
extern BYTE xdata u8ValueL;



void BurnInHandle(void)
{
    if ((g_u8MenuPageIndex == BURNIN_MENU)&&BURNIN_MODE_FLAG && g_stMenuFlag.bChangePattern
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU		
	&&((Is_MultiWin_SyncLoss()&&USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)||((SyncLossState(USER_PREF_INPUT_TYPE)) &&(InputTimingStable_NormalFlag)&& USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF))
#else
	&&((SyncLossState(USER_PREF_INPUT_TYPE)) &&(InputTimingStable_NormalFlag))
#endif
    ) // for burnin mode
    {

        g_stMenuFlag.bChangePattern=0;
	 //g_u8ChangePatternCounter = ChangePatternPeriod;	
        g_u8PatternNo = (g_u8PatternNo + 1) % 8;
    #if 0//ENABLE_MULTI_WINDOW_SETTINGS_MENU
        if (USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
        {
            BYTE u8win;
            for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                msAPI_AdjustBackgoundColor(u8win, tblTestPattern[g_u8PatternNo]);
        }
        else
    #endif
        {
	     BurnInPatternHandler(g_u8PatternNo);
        }

		
    }


}

void FactoryMenuHandler(void)
{
    MenuItemActionType enMenuAction=MIA_NOTHING;

/*
   printData("g_u8MenuPageIndex==%d",g_u8MenuPageIndex);
   printData("BURNIN_MODE_FLAG==%d",BURNIN_MODE_FLAG);
   printData("InputTimingStable_NormalFlag==%d",InputTimingStable_NormalFlag);
   printData("SyncLossState(SrcInputPortM)==%d",SyncLossState(SrcInputPortM));
*/
    if (g_stMenuFlag.bShowFactoryMenu)
    {
        g_u8MenuPageIndex=FACTORY_MENU;
        g_u8MenuItemIndex=CURRENT_F_MENU.u8MenuItemCount-2;// exit item

        g_stMenuFlag.bShowFactoryMenu=0;    ///Must clear this flag here
        FactoryExecuteKeyEvent(MIA_REDRAW_MENU);
        return;
    }

    Key_ScanKeypad();   /// Get Button pressed and save to g_u8KeypadButton.


   if((g_u8MenuPageIndex == BURNIN_MENU) && ((!BURNIN_MODE_FLAG)||(InputTimingStable_NormalFlag)&&(!SyncLossState(SrcInputPortM))))
   {
     g_u8MenuPageIndex = MENU_ROOT;
     g_u8MenuItemIndex = 0;
     g_stMenuFlag.bShowChangeMessage = 1;	
       LoadNormalCP();
       //Osd_LoadNormalColor( FALSE );		
     ExecuteKeyEvent(MIA_REDRAW_MENU);
     return;
   
   }	    

   
   if((g_u8MenuPageIndex == FACTORY_MENU )&& (BURNIN_MODE_FLAG) && ((InputTimingStable_NormalFlag)&&(SyncLossState(SrcInputPortM))))
   {
      g_u8MenuPageIndex = BURNIN_MENU;
      g_u8MenuItemIndex = 0;
      g_stMenuFlag.bOsdTimeout=0;
      g_stMenuFlag.bChangePattern = 1;
      msAPI_AdjustBrightness(100);
#if defined(KTC_Project)
	  msAPI_AdjustContrast( DISPLAY_WIN1, 100);
#endif
	  
      DCRBacklight=100;
      FactoryExecuteKeyEvent(MIA_REDRAW_MENU);
   }	


	
/// Map gu8KeypadButton to MenuItemActionType MIA_XXXXX
    if (g_u8KeypadButton >= BTN_END_BTN)
    {
        enMenuAction = MIA_NOTHING;
    }
    else if (CURRENT_F_MENU_ITEM.penKeyEvents)
    {
        enMenuAction = *((CURRENT_F_MENU_ITEM.penKeyEvents) + g_u8KeypadButton);
    }

    ///  Disable all key action except BTN_POWER while sync loss during Burnin Mode
//    if (DO_BURNIN_MODE_FLAG && SYNC_LOSS_STATE() && (g_u8KeypadButton != BTN_POWER))    // Jeff add it in 2006年3月22日上午 02:04:37
//        enMenuAction = MIA_NOTHING;
    if (!InputTimingStableFlag(SrcInputPortM/*//tmp*/)) // prevent osd action except BTN_POWER while mode changing
    {
        if (enMenuAction != MIA_POWER)
            enMenuAction = MIA_NOTHING;
    }

    if (enMenuAction!=MIA_NOTHING)
    {
        FactoryExecuteKeyEvent(enMenuAction);
        ///Keypad Idle
        if (enMenuAction == MIA_INC_VALUE || enMenuAction == MIA_DEC_VALUE)
        {
            if (++g_u8TurboKeyCounter>2)
                Delay1ms(20);
            else
                Delay1ms(150);
        }
        else
        {
            g_u8TurboKeyCounter = 0;
            Delay1ms(250);
        }
    }
    else
        g_u8TurboKeyCounter=0;
	
#if POWER_KEY_FUNC
if (g_stMenuFlag.bOsdTimeout)
{
    if (g_stMenuFlag.bHoldPowerKey) 
    {
        if (POWER_ON_FLAG)
        {
          g_u8MenuPageIndex = MENU_POWER_OFF;
	   ResetOsdTimer();
        }
    }
   ExecuteKeyEvent(MIA_REDRAW_MENU);
}
#endif

}

Bool FactoryExecuteKeyEvent(MenuItemActionType enMenuAction)
{
    Bool    IsProcessEvent = TRUE;
    BYTE    u8TempValue = 0;
    BYTE    u8Rlt = 0;

    while (IsProcessEvent && g_u8MenuPageIndex>=0x80)
    {
        IsProcessEvent = FALSE;
        switch (enMenuAction)
        {
        case MIA_INC_VALUE:
        case MIA_DEC_VALUE:
            if (CURRENT_F_MENU_ITEM_FUNC.AdjustFunction)
            {
                if (CURRENT_F_MENU_ITEM_FUNC.AdjustFunction(enMenuAction))
                {
                #if 0
                    if (CURRENT_F_MENU_ITEM_FUNC.AdjustFunction == AdjustBankNo)
                    {
                        g_u8OsdFontColor=MONO_COLOR(CP_BLUE, CP_BLUE);
                        Osd_DrawContinuousChar(6, 18, SPACE_FONT, 12);
                    }
                #endif
                    FactoryDrawOsdMenuItemValue(g_u8MenuItemIndex, &CURRENT_F_MENU_ITEM.stDisplayValue);
                }

                //SetOSDFlag(OSD_SAVE_SETTING_BIT);
                g_u8SaveUserData|=SAVE_FACTORY_SETTING;
            }
            break;

        case MIA_NEXT_ITEM:
        case MIA_PREV_ITEM:
            u8TempValue = g_u8MenuItemIndex;
            if (enMenuAction == MIA_NEXT_ITEM)
                g_u8MenuItemIndex = FactoryGetNextItem(CURRENT_F_MENU_ITEMS);
            else
                g_u8MenuItemIndex = FactoryGetPrevItem(CURRENT_F_MENU_ITEMS);

                printData("g_u8MenuItemIndex==%d",g_u8MenuItemIndex);
                printData("u8TempValue==%d",u8TempValue);
			
            if (u8TempValue != g_u8MenuItemIndex)
            {

                FactoryDrawOsdMenuItem(u8TempValue, &CURRENT_F_MENU_ITEMS[u8TempValue]);
                FactoryDrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_F_MENU_ITEM);
                if (g_u8MenuPageIndex == FACTORY_MENU)  // for factory mode
                {   
                    if ((g_u8MenuItemIndex == FMI_Item_6500KR && u8TempValue <FMI_Item_6500KR)
                        || (g_u8MenuItemIndex == FMI_Item_6500KB && u8TempValue > FMI_Item_6500KB_Adj) )
                    {
                        SetFactoryColorTemp6500K();
                    }
                    else if ((g_u8MenuItemIndex ==FMI_Item_9300KR && u8TempValue < FMI_Item_9300KR)
                        || (g_u8MenuItemIndex == FMI_Item_9300KB && u8TempValue > FMI_Item_9300KB_Adj))
                    {
                        SetFactoryColorTemp9300K();
                    }
                    else if ((g_u8MenuItemIndex == FMI_Item_5700KR && u8TempValue < FMI_Item_5700KR)
                        || (g_u8MenuItemIndex == FMI_Item_5700KB && u8TempValue > FMI_Item_5700KB_Adj))
                    {
                        SetFactoryColorTemp5700K();
                    }
					/*
                    else if ((g_u8MenuItemIndex == (33+TEXT_ITEM_VALUE-TSUMT_NO_OFFSET_VALUE) && u8TempValue <(33+TEXT_ITEM_VALUE-TSUMT_NO_OFFSET_VALUE))
                        || (g_u8MenuItemIndex == (37+TEXT_ITEM_VALUE-TSUMT_NO_OFFSET_VALUE) && u8TempValue > (37+TEXT_ITEM_VALUE-TSUMT_NO_OFFSET_VALUE)))
                    {
                        //SetFactoryColorTempsRGB();
                         SetFactoryColorTemp6500K();
                    }

                    else if ((g_u8MenuItemIndex == (13+TEXT_ITEM_VALUE-TSUMT_NO_OFFSET_VALUE) && u8TempValue > (13+TEXT_ITEM_VALUE-TSUMT_NO_OFFSET_VALUE))
                        || (g_u8MenuItemIndex == (39+TEXT_ITEM_VALUE-TSUMT_NO_OFFSET_VALUE) && u8TempValue < (39+TEXT_ITEM_VALUE-TSUMT_NO_OFFSET_VALUE)))
                    {
                         SetFactoryColorTemp6500K();
                    }
					*/


                }
            }
            break;

        case MIA_GOTO_ADJUST_ITEM:
        case MIA_RETURN_SELECT_ITEM:
            u8TempValue = g_u8MenuItemIndex;
            if (enMenuAction == MIA_GOTO_ADJUST_ITEM)
                g_u8MenuItemIndex += 1;
            else
                g_u8MenuItemIndex -= 1;

            if (u8TempValue != g_u8MenuItemIndex)
            {
                if (enMenuAction == MIA_RETURN_SELECT_ITEM)
                {
                    if (CURRENT_F_MENU.pstMenuItems[u8TempValue].stKeyFunction.ExecFunction)
                        CURRENT_F_MENU.pstMenuItems[u8TempValue].stKeyFunction.ExecFunction();
                }

                FactoryDrawOsdMenuItem(u8TempValue, &CURRENT_F_MENU_ITEMS[u8TempValue]);
                FactoryDrawOsdMenuItem(g_u8MenuItemIndex, &CURRENT_F_MENU_ITEM);
                Delay1ms(150);
            }

            g_u8KeypadButton = BTN_REPEAT;
            break;

        case MIA_GOTO_NEXT:
        case MIA_GOTO_PREV:
            u8TempValue = g_u8MenuPageIndex;
            g_u8MenuPageIndex = (enMenuAction == MIA_GOTO_NEXT) ? (NEXT_F_MENU_PAGE) : (PREV_F_MENU_PAGE);

            g_u8MenuItemIndex = ((g_u8MenuPageIndex==FACTORY_MENU)? F_MENU_ITEM_COUNT-1:0);//GetMenuItemIndex(u8TempValue);

            if (g_u8MenuPageIndex<0x80)
            {
//                LoadExitFactoryMenuColor();
                ExecuteKeyEvent(MIA_REDRAW_MENU);
                break;
            }
        case MIA_REDRAW_MENU:
//            if (CURRENT_F_MENU.LoadFont!=NULL)
//                CURRENT_F_MENU.LoadFont();

            FactoryDrawOsdMenu();
            FMENU_printData("=>g_u8MenuPageIndex %d", g_u8MenuPageIndex);
            if (CURRENT_F_MENU.ExecFunction)
            {
                CURRENT_F_MENU.ExecFunction();
            #if ENABLE_VGA_INPUT
                if (CURRENT_F_MENU.ExecFunction == AutoColor)
                    Delay1ms(500);
            #endif
            }

            if (!(CURRENT_F_MENU.u8Flags & MPB_STAY))
            {
                IsProcessEvent = TRUE;
                enMenuAction = MIA_GOTO_PREV;
            }

            if (g_u8SaveUserData!=SAVE_NONE)//(CheckOSDFlag(OSD_SAVE_SETTING_BIT))
                SaveUserPrefSetting(u8TempValue); ////////////////////
            g_u8KeypadButton = BTN_REPEAT;
            break;

        case MIA_EXEC_AND_GOTO_NEXT:
        case MIA_EXEC_GOTO:
        case MIA_EXEC_FUNC:
        case MIA_EXEC_GOTO_NEXT_RETURN:
            if (CURRENT_F_MENU_ITEM_FUNC.ExecFunction)
            {
                u8Rlt = CURRENT_F_MENU_ITEM_FUNC.ExecFunction();

            #if ENABLE_VGA_INPUT
                if (CURRENT_F_MENU_ITEM_FUNC.ExecFunction == AutoColor)
                {
                    FactoryDrawAutoResult(u8Rlt);
                    FactoryDrawOsdMenuItemNumber(0, DrawGainRNumber);
                    FactoryDrawOsdMenuItemNumber(0, DrawGainGNumber);
                    FactoryDrawOsdMenuItemNumber(0, DrawGainBNumber);
                }
            #endif
            }

            if (enMenuAction == MIA_EXEC_GOTO_NEXT_RETURN)
            {
                if (u8Rlt)
                    enMenuAction = MIA_GOTO_NEXT;
                else
                    enMenuAction = MIA_RETURN_SELECT_ITEM;
                g_u8KeypadButton = BTN_REPEAT;
                IsProcessEvent = TRUE;
                break;
            }

            if (enMenuAction == MIA_EXEC_GOTO)
            {
                if (u8Rlt)
                    enMenuAction = MIA_GOTO_NEXT;
                else
                    enMenuAction = MIA_GOTO_PREV;
                g_u8KeypadButton = BTN_REPEAT;
                IsProcessEvent = TRUE;
                break;
            }
            else if (enMenuAction == MIA_EXEC_AND_GOTO_NEXT)
            {
                enMenuAction = MIA_GOTO_NEXT;
                IsProcessEvent = TRUE;
                g_u8KeypadButton = BTN_REPEAT;
            }
            break;

        case MIA_POWER:
            FMENU_printMsg("MIA_POWER");
            if (g_u8MenuPageIndex==BURNIN_MENU)
                Clr_DoBurninModeFlag(); /// prevent in case of power on next time with signal plugin---DO_BURNIN_MODE_FLAG still set.

            g_u8MenuPageIndex = MENU_POWER_OFF;
            g_u8MenuItemIndex = 0;
//            LoadExitFactoryMenuColor();//albert 061221 Innolux-issue-38
            ExecuteKeyEvent(MIA_REDRAW_MENU);
            break;

        case MIA_FACTORY_EXIT:
	    Menu_Hide();
        #if defined(KTC_Project)
           CLR_FACTORY_MODE_FLAG();
           SaveMonitorSetting();
        #endif

	     LoadCommonFont();	
            SaveFactorySetting();
            g_u8MenuPageIndex = MENU_ROOT;
            g_u8MenuItemIndex = 0;
//            LoadExitFactoryMenuColor();
            ExecuteKeyEvent(MIA_REDRAW_MENU);
            break;

        case MIA_RESET_BACKLIGHT:
            g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BLUE);
            USER_PREF_BACKLIGHT_TIME = 0;
            Osd_DrawNum(20, 20, 0);    //Min
            //DrawRealStr(24, 15, MinText());
            Osd_Draw5Num(12, 20, 0);     //Hour;
            //DrawRealStr(18, 15, HourText());
            //DrawRealStr(1, 15, BacklightTimeText());
            SaveMonitorSetting();
            break;
			
#if F_ResetItem
        case MIA_RESET_Factory:
	{
		BYTE u8Result; 

		 WAIT_V_OUTPUT_BLANKING();
      		Menu_Hide();
		CLR_FACTORY_MODE_FLAG();
		u8Result = ResetAllSetting();

	     g_u8MenuPageIndex = MENU_ROOT;
            g_u8MenuItemIndex = 0;
//            LoadExitFactoryMenuColor();
            ExecuteKeyEvent(MIA_REDRAW_MENU);
#if 0		
            g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_BLUE);

        if (u8Result)
            DrawRealStr(12, 4, FPassText());
        else
            DrawRealStr(12, 4, FFailText());
#endif


	}
	break;
	
#endif			

        default:
            return FALSE;
            break;
        }
    }

    return TRUE;
}


BYTE FactoryGetPrevItem(MenuItemType *pstMenuItem)
{
    signed char i;

    if (g_u8MenuItemIndex)
    {
        for (i = g_u8MenuItemIndex - 1; i >= 0; i--)
        {
            if (pstMenuItem[i].u16Flags&MIB_SELECTABLE)
                return i;
        }
    }
/// Last item or can't find selectable item following the pstMenuItem.
    for (i = F_MENU_ITEM_COUNT - 1; i >= 0; i--)
    {
        if (pstMenuItem[i].u16Flags&MIB_SELECTABLE)
            return i;
    }

    return g_u8MenuItemIndex;
}

/// Get the next visible/selectable item on the current menu page.
BYTE FactoryGetNextItem(MenuItemType *pstMenuItem)
{
    BYTE    i;

    if (g_u8MenuItemIndex < F_MENU_ITEM_COUNT - 1)    // current item is not last item.
    {
        for (i = g_u8MenuItemIndex + 1; i < F_MENU_ITEM_COUNT; i++)
        {
            if (pstMenuItem[i].u16Flags&MIB_SELECTABLE)
                return i;
        }
    }
/// Last item or can't find selectable item following the pstMenuItem.
    for (i = 0; i < g_u8MenuItemIndex; i++)
    {
        if (pstMenuItem[i].u16Flags&MIB_SELECTABLE)
            return i;
    }

    return g_u8MenuItemIndex;
}

//----------JL-----for draw serial number---------
//extern BYTE code strEDIDWPM20[1][2];
extern BYTE xdata  tPropText[][2];
extern BYTE code strMode20M20[1][2];//20080416, strMode19M20[1][2];
BYTE*  SerialNumber(void)
{
#if 0 //XXXXX
    BYTE i;
    BYTE xdata temp[13];
////////////////////////////////////////////////////
    for(i=0;i<12;i++)
    {
        if(i<5)
            NVRam_ReadByte((SERIAL_NUMBER_ADDR+3)+i, & temp[i]);
        else
            NVRam_ReadByte((SERIAL_NUMBER_ADDR+3+5)+i, & temp[i]);
    }
////////////////////////////////////////////////////

   // for(i=0;i<12;i++)
   //    NVRam_ReadByte(SERIAL_NUMBER_ADDR+i, & temp[i]);

    temp[12]=0;
    //tPropText[1][0]=strEDIDWPM20[0][0]+strEDIDWPM20[0][1];// EDIDWP must be last string FM_TEXT1_START+6;
    tPropText[1][0]=strMode20M20[0][0];//20080416, strMode19M20[0][0]+strMode19M20[0][1];//strMode19M20 last string
    pstPropFontSet1218=tPropFontSetArialNarrow13Bold;
    g_u8PropFontFlags=SPACE1PIXEL|LEFT_ALIGN;
    tPropText[1][1]=LoadPropFontText(tPropText[1][0],temp);
    return tPropText[1];
#else
    return NULL;
#endif
}
void FactoryDrawOsdMenu(void)
{
    WORD u16Value;
    if (CURRENT_F_MENU.u8Flags & MPB_INVISIBLE)
    {
        Menu_Hide();
    }
    else
    {
        BYTE    i;

        Menu_Hide();

#if ENABLE_OSD_ROTATION
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

		
        Osd_SetWindowSize(GET_MENU_XSIZE(CURRENT_F_MENU), GET_MENU_YSIZE(CURRENT_F_MENU)/*, RESET*/);
        if (g_u8MenuPageIndex == FACTORY_MENU)
        {
//          Osd_SetPosition(0, 100);
	 #if (Enable_Panel_Rotate_180)
            Osd_MenuHPosition(100,TRUE);
            Osd_MenuVPosition(100,TRUE);
	 #else
            Osd_MenuHPosition(0,TRUE);
            Osd_MenuVPosition(0,TRUE);
	#endif			

//          LoadFactorymenuColor();
        }
	 else if(g_u8MenuPageIndex == BURNIN_MENU)
	 {
            OSDMoveHstart=50;
            OSDMoveVstart=50;
            Osd_SetPosition(OSDMoveHstart,OSDMoveVstart);

	 }
//        else
//            Osd_SetPosition(USER_PREF_OSD_H_START, USER_PREF_OSD_V_START);

        g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BLUE);
        Osd_DrawBlankPlane(0,0,GET_MENU_XSIZE(CURRENT_F_MENU),GET_MENU_YSIZE(CURRENT_F_MENU));

	 
    ///Load Fonts
        if (CURRENT_F_MENU.LoadFont!=NULL)
            CURRENT_F_MENU.LoadFont();
                // draw osd menu item
        for (i = 0; i < F_MENU_ITEM_COUNT; i++)
        {
            FactoryDrawOsdMenuItem(i, &CURRENT_F_MENU.pstMenuItems[i]);
        }

        if (FACTORY_MODE_FLAG)
        {// 2007/1.17   xxxx
             //--------JL--
            if (g_u8MenuPageIndex == FACTORY_MENU)
            {
                //g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BLUE);  //JL
                //DrawPropStr(1,0,  ModelNameText());
                //DrawPropStr(g_u8OsdWindowWidth-8,0,  VersionText());
                // g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BLUE);
                //  DrawPropStr(1, 3, PanelText());
                //DrawPropStr(g_u8OsdWindowWidth-18,0,  PanelTypeText()); //JL
                //DrawPropStr(1,17,  SerialNumber());		//JL
                g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BLUE);
                u16Value = (USER_PREF_BACKLIGHT_TIME % 3600ul) / 60;
                Osd_DrawNum(20, 20, u16Value); //Min
                DrawRealStr(24, 20, MinText());
                u16Value = USER_PREF_BACKLIGHT_TIME / 3600ul;
                Osd_Draw5Num(12, 20, u16Value);  //Hour;
                DrawRealStr(18, 20, HourText());
                DrawRealStr(1, 20, BacklightTimeText());

            }
        }
        Menu_Show();

    }
}

void FactoryDrawOsdMenuItem(BYTE u8ItemIndex, MenuItemType *pstMenuItem)
{

    if (pstMenuItem->u16Flags & MIB_SELECTABLE && u8ItemIndex == g_u8MenuItemIndex)
        g_u8OsdFontColor=pstMenuItem->u8SelColor;
    else
        g_u8OsdFontColor=pstMenuItem->u8Color;

        DrawOsdMenuItemText(u8ItemIndex, pstMenuItem);

    //if (pstMenuItem->stDisplayValue)  //XXXXX
        FactoryDrawOsdMenuItemValue(u8ItemIndex, &(pstMenuItem->stDisplayValue));  //->stDisplayValue);
}



void FactoryDrawOsdMenuItemNumber(BYTE u8ItemIndex, DrawNumberType *pstNumberItem)
{
    if (g_u8MenuPageIndex == FACTORY_MENU)
    {
        if (CURRENT_F_MENU_ITEM.stDisplayValue.pstDrawNumber == pstNumberItem)
            g_u8OsdFontColor=pstNumberItem->u8Color;
        else
            g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BLUE);
    }
    else
        g_u8OsdFontColor=pstNumberItem->u8Color;
    u8ItemIndex = 0;
#define DRAW_ITEM   (pstNumberItem + u8ItemIndex)
    while(1)
    {
        if (DRAW_ITEM->u8Flags&NUM_HEX)
            Osd_DrawHex(PTR_GET_DN_XPOS(DRAW_ITEM), PTR_GET_DN_YPOS(DRAW_ITEM), DRAW_ITEM->GetValue());
        else if (DRAW_ITEM->u8Flags&NUM_4NUMS)
            DrawNumUnsigned(PTR_GET_DN_XPOS(DRAW_ITEM), PTR_GET_DN_YPOS(DRAW_ITEM), 4,DRAW_ITEM->GetValue());
        else
            Osd_DrawNum(PTR_GET_DN_XPOS(DRAW_ITEM), PTR_GET_DN_YPOS(DRAW_ITEM), DRAW_ITEM->GetValue());
        if (DRAW_ITEM->u8Flags&NUM_END)
            break;
        u8ItemIndex++;
    }
#undef DRAW_ITEM

}


void FactoryDrawOsdMenuItemRadioGroup(BYTE u8ItemIndex, DrawRadioGroupType *pstRadioItem)
{
    BYTE    i = 0;

    u8ItemIndex=0;
    if (!(pstRadioItem->pstRadioText))
        return;

#define DRAW_ITEM   (pstRadioItem->pstRadioText + i)
    while (1)
    {   // set high color
        if (DRAW_ITEM->DisplayText && DRAW_ITEM->DisplayText()!=NULL) ///Donot draw if return NULL
        {
            if (g_u8MenuPageIndex == FACTORY_MENU||g_u8MenuPageIndex == BURNIN_MENU)
            {
                if (CURRENT_F_MENU_ITEM.stDisplayValue.pstDrawRadioGroup == pstRadioItem)
                    g_u8OsdFontColor=pstRadioItem->u8Color;
                else
                    g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BLUE);
                DrawRealStr( PTR_GET_DT_XPOS(DRAW_ITEM), PTR_GET_DT_YPOS(DRAW_ITEM), DRAW_ITEM->DisplayText());
            }
            else
            {
                g_u8OsdFontColor=pstRadioItem->u8Color;
                DrawFullCenterText(PTR_GET_DT_XPOS(DRAW_ITEM), PTR_GET_DT_YPOS(DRAW_ITEM), DRAW_ITEM->DisplayText());
            }
        }

        if (DRAW_ITEM->u8Flags & TXT_END)
            break;
        i++;
    }

#undef DRAW_ITEM
}

void FactoryDrawOsdMenuItemValue(BYTE u8ItemIndex, DrawValueType *pstValueItem)
{
    if (pstValueItem->pstDrawRadioGroup)
    	{
        FactoryDrawOsdMenuItemRadioGroup(u8ItemIndex, pstValueItem->pstDrawRadioGroup);
    	}
    if (pstValueItem->pstDrawNumber)
        FactoryDrawOsdMenuItemNumber(u8ItemIndex, pstValueItem->pstDrawNumber);
//    if (pstValueItem->pstDrawGauge)
//        DrawOsdMenuItemGauge(u8ItemIndex, pstValueItem->pstDrawGauge);
}

 void DrawBackLiteTime(void)
{ 
  WORD u16Value;

     g_u8OsdFontColor=MONO_COLOR(CP_WHITE, CP_BLUE);
     u16Value = (USER_PREF_BACKLIGHT_TIME % 3600ul) / 60;
     Osd_DrawNum(20, 6, u16Value); //Min
     DrawRealStr(24, 15, MinText());

     u16Value = USER_PREF_BACKLIGHT_TIME / 3600ul;
     Osd_Draw5Num(11, 6, u16Value);  //Hour;
     DrawRealStr(17, 15, HourText());

     if(!OSDShowFlag)
	Menu_Show(); 	
}

#define MAX_X           100
#define MAX_Y          100
#define MIN_X           0
#define MIN_Y           0
#define OSD_MOVESTEP_X      3
#define OSD_MOVESTEP_Y      1
void OSDMovePosition(void)
{
 	BYTE x,y;
		static BYTE yb;
 		 if(OSDMoveHstart < MAX_X){
        OSDMoveHstart += OSD_MOVESTEP_X;
        if(OSDMoveHstart >= MAX_X){
            OSDMoveHstart =2*MAX_X;
        }
    }
    else{
        OSDMoveHstart -= OSD_MOVESTEP_X;
         if(OSDMoveHstart < MAX_X){
            OSDMoveHstart = MIN_X+1;
        }
    }
    if(OSDMoveVstart < MAX_Y){
        OSDMoveVstart += OSD_MOVESTEP_Y;
        if(OSDMoveVstart >= MAX_Y){
            OSDMoveVstart = 2*MAX_Y;
        }
    }
    else{
        OSDMoveVstart -= OSD_MOVESTEP_Y;
        if(OSDMoveVstart <MAX_Y){
            OSDMoveVstart = MIN_Y+1;;
        }
    }
	x=OSDMoveHstart;
	y=OSDMoveVstart;
	if(x>MAX_X)
		x-=MAX_X;
	if(y>MAX_Y)
		y-=MAX_Y;
	if(yb==MAX_Y-1)
		y=MAX_Y;
	else
	y=MAX_Y-y;
	yb=y;
	Osd_SetPosition(x,y);
}


#if ENABLE_VGA_INPUT
void FactoryDrawAutoResult(BYTE u8Result)
{
    g_u8OsdFontColor=MONO_COLOR(CP_RED, CP_BLUE);
    if (g_u8MenuPageIndex == FACTORY_MENU)
    {
        if (u8Result)
            DrawRealStr(12, 4, FPassText());
        else
            DrawRealStr(12, 4, FFailText());
    }
    else if (g_u8MenuPageIndex == AUTO_COLOR_MENU)
    {
        if (u8Result)
            DrawRealStr(12, 3, PassText());
        else
            DrawRealStr(12, 3, FailText());
    }
}
#endif



void BurnInPatternHandler(BYTE Pattern)
{
#if 0//ndef HKC_Project  	
       Menu_Hide();
	if(tblTestPattern[Pattern]==ePATTERN_COLOR_WHITE)
	{
          g_u8OsdFontColor=MONO_COLOR(9, 5);
	   DrawRealStr(0, 0, F_AGINGText());
	}
	else
	{
          g_u8OsdFontColor=MONO_COLOR(0, 0); 
	   DrawRealStr(0, 0, F_AGINGText());
	}
#endif	
	#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	if (USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
	{ 
	    BYTE u8win;
	    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
	    {
	       msAPIWinEnableFrameColor(u8win, TRUE);
		msAPI_AdjustBackgoundColor(u8win, tblTestPattern[Pattern]);
	    }
	}
	else
	#endif
	{
	       msAPIWinEnableFrameColor(DISPLAY_WIN1, TRUE);
		msAPI_AdjustBackgoundColor(DISPLAY_WIN1, tblTestPattern[Pattern]);
	}

	#if defined(KTC_Project)
	  msAPI_AdjustBrightness(100);
	  msAPI_AdjustContrast( DISPLAY_WIN1, 100);
       #endif
	 
    #if 1//(defined(HKC_Project))
	DrawBackLiteTime();
     if(g_u8MenuPageIndex == BURNIN_MENU)	
	OSDMovePosition();
    #endif	
#if 0//ndef HKC_Project  	
       Menu_Show();
#endif
}


