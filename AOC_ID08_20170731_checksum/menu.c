#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "board.h"
#include "global.h"
#include "debug.h" //change position eshin
#include "keypaddef.h"
#include "menudef.h"
#include "keypad.h"
#include "ms_rwreg.h"
#include "msosd.h"
#include "menufunc.h"
#include "menustr.h"
#include "menutbl.h"
#include "ms_reg.h"
#include "misc.h"
#include "nvram.h"
#include "adjust.h"
#include "userpref.h"
#include "power.h"
#include "msdlc.h"
#include "detect.h"
#include "PropFont.h"
#include "Panel.h"
#include "Mcu.h"
#ifdef ReduceDDC
#include "ddc.h"
#endif

#if HotInputSelect
#include "mStar.h"
#endif

   
#if DEBUG_FLASHADDR&&USEFLASH
#include "msflash.h"
#endif

#if MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A || MainBoardType==MainBoard_715G3959_T
extern void SetScalerGPIO(BYTE pinX, bit HiLow);
#endif

#if ScanKey
    #if Scan_Source == Cypress
    #include "scankey.h"
    #endif
#endif

#if  BlacklitTimeOnShow
 void Draw_BlacklitTimeOn_Info_LGFactoryMenu(void);
#endif
#if F_ResetCheckEdidManueAndSN
Bool FactoryCheckEdidManufactureAndSN( void );
#endif

 void Osd_MainFrameOSD_E1(void);
extern void Osd_DrawHotKeyIcon(BYTE itemIndex, BYTE xpos, BYTE ypos);
extern void LoadHotKeyColorFont(BYTE index);
extern void LoadMainTitleFont(BYTE index);
extern void printData(char *str, WORD value);
extern void printMsg(char *str);
extern void msAccOnOff(BYTE on);
extern void SetColorBoostWindow(BYTE mode);
extern Bool CheckSourceKeyStatus(void);
extern void mStar_SetupGamma(BYTE GamaMode); 
extern void DrawNum(BYTE xPos,BYTE yPos,char len,WORD value);		//110316 Modify
#define CurrentMenu     tblMenus[MenuPageIndex]
#define PrevMenuPage        CurrentMenu.PrevPage
#define CurrentMenuItems    CurrentMenu.MenuItems
#define MenuItemCount       CurrentMenu.ItemCount
#define CurrentMenuItem     CurrentMenu.MenuItems[MenuItemIndex]
#define NextMenuPage        CurrentMenuItem.NextPage
#define CurrentMenuItemFunc CurrentMenuItem.KeyFunction
#define KeyEvent        CurrentMenuItem.KeyEvents[KeypadButton]
#if 0

#if (DisplayLogo==EnvisionLOGO)
    BYTE code EnvisionLogo_1[] =  // EnvisionLogo
    {
        //123456789012345678901234567890
        "\x02\x03\x04\x05\x06\x07\x08"
    };
    BYTE code EnvisionLogo_2[] =
    {
        //1234567890123456789012345678901234
        "\x09\x0A\x0B\x0C\x0D\x0E\x0F"
    };
    BYTE code EnvisionLogo_1_2[] =
    {
        //1234567890123456789012345678901234
        "\x007"
    };
    BYTE code EnvisionLogo_2_2[] =
    {
        //1234567890123456789012345678901234
        "\x00E"
    };
#elif (DisplayLogo==StaplesLOGO)
    BYTE code StaplesOsdLogo_1_1[] =
    {
        "\x02\x03\x04\x05\x06\x07\x08"
    };
    BYTE code StaplesOsdLogo_1_2[] =
    {
        "\x09\x0A\x0B\x0C\x0D\x0E\x0F"
    };
#else
#define AOCLogo_Adr 0x0A
	BYTE code AOCLogo_1[] =
	{
	    //1234567890123456789012345678901234
        AOCLogo_Adr+0, AOCLogo_Adr+1, AOCLogo_Adr+2, AOCLogo_Adr+3,
        AOCLogo_Adr+4, AOCLogo_Adr+5, AOCLogo_Adr+6, '\0'
	};
	BYTE code AOCLogo_2[] =
	{
	    //1234567890123456789012345678901234
        AOCLogo_Adr+7, AOCLogo_Adr+8, AOCLogo_Adr+9, AOCLogo_Adr+10,
        AOCLogo_Adr+11, AOCLogo_Adr+12, AOCLogo_Adr+13, '\0'
	};
#endif

#endif

//========================================================================
#if 1
BYTE code Left_Item[] =
{
  8, //0
  0, //1
  1, //2
  2, //3
  3, //4
  4, //5
  5, //6
  6, //7
  7, //8
};
BYTE code Right_Item[] =
{
  1, //0
  2, //1
  3, //2
  4, //3
  5, //4
  6, //5
  7, //6
  8, //7
  0, //8
};
BYTE code Up_Item[] =
{
  8, //0
  6, //1
  7, //2
  0, //3
  1, //4
  2, //5
  3, //6
  4, //7
  5, //8
};
BYTE code Down_Item[] =
{
  3, //0
  4, //1
  5, //2
  6, //3
  7, //4
  8, //5
  1, //6
  2, //7
  0, //8
};
#endif
//========================================================================
//BYTE  MenuPageIndex = 0;
//BYTE  MenuItemIndex = 0;
//BYTE xdata PrevMenuItemIndex = 0;
BYTE xdata TurboKeyCounter = 0;
BYTE xdata PrevPage = 0;
BYTE xdata moveX=50;
BYTE xdata moveY=50;
BYTE xdata ItemStep = 0; // 2007/9/13  by KK for osd jump
//========================================================================
// 2006/11/10 10:0PM by KK move to userpref.c void SaveUserPrefSetting( BYTE menuPageIndex );
#if !DefaultIconExit//(!AOC_37_Series && ModelName!=ENVISION_P2071l&&ModelName!=ENVISION_P2271wl&&ModelName!=ENVISION_P971wl)
BYTE GetMenuItemIndex(BYTE menuPageIndex);
#endif
BYTE GetPrevItem(MenuItemType *menuItem);
BYTE GetNextItem(MenuItemType *menuItem);
Bool ExecuteKeyEvent(MenuItemActionType menuAction);
void DrawOsdMenu(void);
void DrawOsdMenuItem(BYTE drawIndex, MenuItemType *menuItem);
void DrawOsdMenuItemText(BYTE itemIndex, MenuItemType *menuItem);
void DrawOsdMenuItemValue(BYTE itemIndex, DrawValueType *valueItem);
void DrawOsdMenuItemNumber(BYTE itemIndex, DrawNumberType *numberItem);
void DrawOsdMenuItemGuage(BYTE itemIndex, DrawGuageType *guageItem);
void DrawOsdMenuItemRadioGroup(BYTE itemIndex, DrawRadioGroupType *RadioItem);
//Bool DrawTimingInfo(void);
// 2006/11/10 10:0PM by KK move to userpref.c void SaveFactorySetting( BYTE itemIndex );
void DrawLogo(void);
void DrawOsdBackGround(void);
Bool DrawTimingInfo(void);

void ClrInnerOsd(void);
//=========================================================================
void Menu_InitVariable(void)
{
    MenuPageIndex = RootMenu;
    MenuItemIndex = 0;
    OsdCounter = 0;
    HotKeyCounter = 0;
    if (!PowerOnFlag)
    {
        MenuPageIndex = PowerOffMenu;
    }
}

void Menu_InitAction(void)
{
#if 1//DisplayLogo!=NoBrand    cancel for When have the signal the common EDID lode be  forbid ,add 110121
    //EDID program when cable not connected
    //if ( SyncLossState() && CableNotConnectedFlag ) //2006-02-14
    if (SyncLossState() && IsCableNotConnected()&&!ProductModeFlag)//  2011.1.11   8:43:36 for DFM
      // petit 20121123 for TPV new request
    {
        #ifdef ReduceDDC
		#if !Enable_External_EDID
            mStar_SetDDC_WP();
		#endif
		#ifdef OnlyVGAEDIDInFlash
	            hw_SetDDC_WP();
		#endif
        #else
           // hw_SetDDC_WP();
        #endif 
    }
#endif
       if(LogoON==0)
  	Clr_DisplayLogoFlag();
  
    //printMsg("MenuInitAction....");
//    if (FactoryModeFlag)
//        LoadCommonFont();
    MenuItemIndex = 0;
    if (DisplayLogoFlag)
    {

	 #if DEBUG_FINDMODE
            printData( "MenuPageIndex = %d", MenuPageIndex );
        #endif
        //printMsg("displaymode");
        if (MenuPageIndex == LogoMenu)
        {
            return ;
        }
        MenuPageIndex = LogoMenu;

    }
	else	
	{
#if DisplayEnergyStar
    	if(ShowEpaAfterLogoFlag&&!DoBurninModeFlag&&!ProductModeFlag&&!SyncLossState())
    		{
    		            MenuPageIndex = EnergyStarMenu;
                        DrawOsdMenu();
                        Delay1ms(1000);
						Osd_Hide();
						Delay1ms(30);
						MenuPageIndex = RootMenu;	
                  	  // Osd_DrawContinuesChar(0,4,SpaceFont,OsdWindowWidth);   //071228
                  	  // Osd_DrawContinuesChar(0,5,SpaceFont,OsdWindowWidth);
                  	  // Osd_DrawContinuesChar(0,6,SpaceFont,OsdWindowWidth);

					   
    		
    		}
#endif
        if (UnsupportedModeFlag)
        {
            MenuPageIndex = UnsupportedModeMenu;
            moveX=UserPrefOsdHStart;
            moveY=UserPrefOsdVStart;
            if (OsdLockModeFlag)
            {
                // Clr_OsdLockModeFlag();
                // Clr_ShowOsdLockFlag();
                // NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
            }
        }
        else if (SyncLossState())
        {
            if (CableNotConnectedFlag)
            {
            #if DEBUG_ENABLE
		printMsg("CableNotConnectedFlag");
	      #endif
                if (BurninModeFlag)
                // 2006/10/18 9:47PM by Emily test
                {
                    if (PowerOnFlag)
			{
				Power_TurnOnAmberLed();
			}
			else
			{
				Power_TurnOffLed();
			}
                    MenuPageIndex = BurninMenu;

                }
                else
                {
                    #if Dual
					if(!ProductModeFlag)
					if ((UserPrefInputSelectType != InputSelectType_Auto) )
					{
						MenuPageIndex = InputInfoMenu;
						ExecuteKeyEvent(MIA_RedrawMenu);
						Delay1ms(2000);
						Osd_Hide();
						Delay1ms(50);	
					}
                        MenuPageIndex = StandbyMenu;
    				    OsdCounter = 2;
                    #else 
    				  #if INPUT_TYPE==INPUT_1A   //071225 for Cable not connectd if analog only 
                        MenuPageIndex = CableNotConnectedMenu;//
				if(ProductModeFlag)
                        		OsdCounter = 2;
				else
					OsdCounter=60;
                        moveX=UserPrefOsdHStart;
                        moveY=UserPrefOsdVStart;
			  Power_TurnOnAmberLed();
                      #else
                        MenuPageIndex = StandbyMenu; //CableNotConnectedMenu;//later will check if cancel CableNotConnectedMenu or not
    					OsdCounter = 2;
    				  #endif	
                    #endif
                }
            }
            else
            {
			#if Dual
			if ((UserPrefInputSelectType != InputSelectType_Auto) )
			{
				MenuPageIndex = InputInfoMenu;
				ExecuteKeyEvent(MIA_RedrawMenu);
				Delay1ms(2000);
				Osd_Hide();
				Delay1ms(50);
			}
			#endif
			if (BurninModeFlag)
			{
            #if DEBUG_ENABLE
		printMsg("CableNotConnectedFlag");
	      #endif
				MenuPageIndex = BurninMenu;
			}
			else
			{
	                	MenuPageIndex = StandbyMenu;
	   			OsdCounter = 2;
				Power_TurnOnAmberLed();
			}
            }
            // 2006/9/29 3:48PM by Emily
      
        }
        else if (OsdLockModeFlag && ShowOsdLockFlag && !FactoryModeFlag)
        {
            MenuPageIndex = OsdLockMenu;
            Clr_ShowOsdLockFlag();
        }
        //  else if (UnsupportedModeFlag)
        //      MenuPageIndex=UnsupportedModeMenu;
        /******For PE Request*********
        else if (FactoryModeFlag)
        {
            MenuPageIndex = AutoColorMenu;
        }
        ****************************/
        else if(ProductModeFlag)	//071226
        {
    		MenuPageIndex = RootMenu;
    		OsdCounter=0;
        }	
        else if (UserPrefAutoTimes == 0 && SrcInputType < Input_Digital&&(SrcModeIndex!=PRESET_640x480_60Hz_VESA))//&& !ProductModeFlag)//071226
        {
            MenuPageIndex = AutoMenu;
		AutoColor();
    	#if DisplayInputInfo
    		if(ShowInputInfoFlag)
    			Clr_ShowInputInfoFlag();
    	#endif
    
        }
        else
        {
    #if 1 // 071221
            if (ShowInputInfoFlag)//&& !ProductModeFlag)//071226
            {
                 Clr_ShowInputInfoFlag();
                 MenuPageIndex = InputInfoMenu;
                 OsdCounter = 3;
            }
            else
            MenuPageIndex = RootMenu;
    #else
            if (ShowInputInfoFlag&& !ProductModeFlag)
            {
                MenuPageIndex = InputInfoMenu;
                OsdCounter = 6; //10;
                 Clr_ShowInputInfoFlag();
                DrawOsdMenu();
                Delay1ms(1000); //(500);
            }
            MenuPageIndex = RootMenu;
    #endif        
        }
#if DisplayEnergyStar
		Clr_ShowEpaAfterLogoFlag();
#endif
    }

   	//OsdCounter = 0; // 071221

    
	if (SrcInputType != UserPrefInputType&&!SyncLossState())
        {
         #if !USEFLASH
		NVRam_WriteByte(nvrMonitorAddr(InputType), SrcInputType);
         #else
 		UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
         #endif
            UserPrefInputType = SrcInputType;
        }

// 090703
#if 0//DEBUG_ENABLE
    printData("33333MenuPageIndex:%d", MenuPageIndex);
#endif
    
		if(InputTimingChangeFlag)	//110311 Modify
		{
			if(!DisplayLogoFlag)
			{
				MenuPageIndex = RootMenu;
				OsdCounter = 0;
				Clr_OsdTimeoutFlag();
				return;
			}
		}
			
        ExecuteKeyEvent(MIA_RedrawMenu);
    
    if (UnsupportedModeFlag)
    {
        if (!FreeRunModeFlag && SrcInputType < Input_Digital&& !ProductModeFlag)
        {
            if (UserPrefAutoTimes == 0)
            {
                AutoConfig();
            }
        }
    }
}

BYTE MenuActionProcess(BYTE value)
{
    BYTE menuAction = value;
/*
    if (MenuPageIndex == RootMenu)
    {
        if (PressExitFlag)
        {
        }
    }
*/    

// power and menu use same key,so the action must check again!
	if(PowerKeyFlag && PowerOnFlag)
	{
	    if(PowerOffFlag) //set flag while 3 sec
	    {
	    #if AllInOneKey //Design For Manufacture 	Zhifeng.Wu 20071126 When Press Power Key For 1S Auto Switch Color Temp.
	        if(ProductModeFlag)
	        	{
	        	#if !AOC_37_Series  //091028  for PE request  for DFM, by lizzie
                      if (MenuPageIndex==RootMenu)
                      {
                          BYTE i;
                          UserPrefLastMenuIndex=2;		  
                          MenuPageIndex=ColorTempMenu;
                          MenuItemIndex=0;
                          ExecuteKeyEvent(MIA_RedrawMenu);     
                          for (i=0;i<5;i++)				   
                          {
                              Delay1ms(500);
                              ExecuteKeyEvent(MIA_DecValue);     
                          }                         
                         Clr_MainOSDFlag();
                         Clr_PowerKeyFlag();
                         Clr_DoKeyCountFlag();
                         Clr_PowerOffFlag();
                         menuAction = MIA_Nothing;
                      }
                      else if(MenuPageIndex==ColorTempMenu)
                      {
				Osd_Hide();
				MenuPageIndex=ResetWaitMenu;
                        #if DefaultIconExit//AOC_37_Series
				MenuItemIndex=8;
                        #else
				MenuItemIndex=0;
                        #endif
				Set_DoResetFlag();  
				ExecuteKeyEvent(MIA_RedrawMenu);   
				Clr_DoResetFlag(); 
				ExecuteKeyEvent(MIA_Power);     
				Clr_MainOSDFlag();
				Clr_PowerKeyFlag();
				Clr_DoKeyCountFlag();
				Clr_PowerOffFlag();
				menuAction = MIA_Nothing;
                      }
                      else
			  #endif
	                   menuAction = MIA_Power;
	        	}
		else
	     #endif
	           menuAction = MIA_Power;
	    }
	    else // less 3 sec
        {		
        	if(MainOSDFlag) // show osd menu while release key
            {
                if (MenuPageIndex < RootMenu || MenuPageIndex == LogoMenu)
                {
                    menuAction = MIA_Nothing;

                }
			#if HotInputSelect && AllInOneKey
				else if(MenuPageIndex==HotInputSelectMenu)
				{
					menuAction = MIA_ExecFunc;			
				}
			#endif	
                else
                {
			        menuAction = MIA_GotoNext;
			#if AllInOneKey
			   if(/*MenuPageIndex==FactoryMenu&&*/(CurrentMenuItemFunc.ExecFunction== AutoColor
			   	#if 0//EnablePanelServiceMenu
			   	||CurrentMenuItemFunc.ExecFunction== Test
			   	||CurrentMenuItemFunc.ExecFunction== InitialPanelSetting
			   	||CurrentMenuItemFunc.ExecFunction== SaveCurrentPanelSetting
			   	#endif
			   	))
			     menuAction =MIA_ExecFunc;
            #endif

				#if DEBUG_PRINTDATA
				printData("menuAction1234=%d", menuAction);
				//printData("MenuItemIndex=%d", MenuItemIndex);
				#endif
                }
                Clr_MainOSDFlag();
                Clr_PowerKeyFlag();
                Clr_DoKeyCountFlag();
            }
            else
                menuAction = MIA_Nothing;
        }
	}

#if HotInputSelect
   #if AllInOneKey
if(PowerSavingFlag && KeypadButton==KEY_MENU)
   #else
if(PowerSavingFlag && 
	#if (ModelName == AOC_2219Vwg||ModelName==AOC_2219Vg||ModelName==ENVISION_G2219_LM220WE1||\
		ModelName==AOC_2219P2_2883||ModelName==AOC_2219Vwg5||ModelName==AOC_2219P2_3329||ModelName==ENVISION_G2360g||ModelName==ENVISION_G2218||ModelName==ENVISION_G2218WG1||ModelName==ENVISION_G2218_PLUS||ModelName==AOC_2219P2_3834)
		KeypadButton==BTN_Select
	#else
		KeypadButton==BTN_Exit
	#endif
	)
   #endif
{
	menuAction=MIA_ShowHotInput;
}
#endif	

#if EnableVcomAdjustItem
		if(KeypadButton == BTN_VCOM)
		{
			if (ProductModeFlag)
			{
				menuAction=MIA_HotVCOM;
				//Set_FactoryModeFlag();
				//TurboKeyCounter++;
			}
		}
#endif

    if (menuAction)
    {
    	if(menuAction!=MIA_Power && OsdLockModeFlag )    //080103 prevent key event if Osd Locked
		{           

			if(menuAction != MIA_SourceSel && menuAction != MIA_Auto )
			{
			if (TurboKeyCounter >0)
    				menuAction=MIA_Nothing;
    		
			else if(MenuPageIndex==RootMenu)
    			{
	            		MenuPageIndex=OsdLockMenu;
	            		menuAction=MIA_RedrawMenu;
    			}
			TurboKeyCounter=1;
			}
  		}
    //    printData("osd action %d", menuAction);
        if (!InputTimingStableFlag)
        // prevent osd action while mode changing
        {
            if (menuAction != MIA_Power)
            {
                menuAction = MIA_Nothing;

        #if ScanKey
            #ifdef EnableSlider
                SliderIncDecStep = 0;
            #endif
        #endif
            }
        }
        if (ExecuteKeyEvent(menuAction))
        {
            if (MenuPageIndex > RootMenu)
            {
                ResetOsdTimer();
#if ScanKey
    #if Scan_Source == Synaptics
                KeyLedCounter = UserPrefOsdTime;
                Clr_keyLEDTurnOffFlag();
    #endif                
#endif
            }
            Clr_OsdTimeoutFlag();
        }
        if (menuAction == MIA_IncValue || menuAction == MIA_DecValue || menuAction == MIA_Auto)
        {
            Delay1ms(20);
            if (TurboKeyCounter < 100)
            {
                TurboKeyCounter++;
            }
        }
        #if ECO
            else if ((menuAction == MIA_Exit) || (menuAction == MIA_GotoECO))
        #else
            else if (menuAction == MIA_Exit)
        #endif
        {
            Delay1ms(150);
            KeypadButton = BTN_Repeat;
        }
    }
    else
    {
        TurboKeyCounter = 0;
    }

    return menuAction;

}

void Menu_OsdHandler(void)
{
    BYTE menuAction;

    #if ScanKey

        if(StartScanKeyFlag)
        {
            Key_ScanKeypad();
            Clr_StartScanKeyFlag();
            TPDebunceCounter = SKPollingInterval;
        }

        #if Scan_Source == Cypress
        if(KeyReadyFlag)
        {
            Clr_KeyReadyFlag();
            if (KeypadButton >= BTN_EndBTN)
            {
                menuAction = MIA_Nothing;
            }
            else if (CurrentMenuItem.KeyEvents)
            {
                menuAction = *((CurrentMenuItem.KeyEvents) + KeypadButton);     
            }
#if 0//DEBUG_ENABLE&&DEBUG_SCANKEY// 090624 coding test
printData("  KeypadButton:%x", KeypadButton);
#endif
            menuAction = MenuActionProcess(menuAction);
            
        }
        #else // Synaptics
        if (KeypadButton >= BTN_EndBTN)
        {
            menuAction = MIA_Nothing;
        }
        else if (CurrentMenuItem.KeyEvents)
        {
            menuAction = *((CurrentMenuItem.KeyEvents) + KeypadButton);     
        }

        menuAction = MenuActionProcess(menuAction);
        
        #endif
        
    #else // !ScanKey

    Key_ScanKeypad();
    
    if (KeypadButton >= BTN_EndBTN)
    {
        menuAction = MIA_Nothing;
    }
    else if (CurrentMenuItem.KeyEvents)
    {
        menuAction = *((CurrentMenuItem.KeyEvents) + KeypadButton);     
    }

    menuAction = MenuActionProcess(menuAction);
    
    #endif
    
	#ifdef OffPower
	if(ShowOffPowerWarningFlag)
	{
		ShowOffPowerWarningFlag = 0;
		MenuPageIndex = OffPowerWarningMenu;
		MenuItemIndex = 0;
		ExecuteKeyEvent( MIA_RedrawMenu );
		OsdCounter = 10;
		Clr_OsdTimeoutFlag();
	}
	#endif

    #if ScanKey
        #if Scan_Source == Cypress
        #ifdef LEDBrightCtrlByScalerPWM
        if(PowerOnFlag) // 091020 coding
        {
            if(bPowerLEDBrightActStepFlag)
            {
                ScanKey_LEDBrightAction();
                bPowerLEDBrightActStepFlag = 0;
            }
        }
        #endif
        #endif
    #endif

    if (OsdTimeoutFlag)
    {
	//if (MenuPageIndex == ExtraMenu)
	//     ReadMonitorSetting();
	#if Dual
		if((MenuPageIndex == ExtraMenu)&&(UserPrefInputSelectType != SrcInputType) && (UserPrefInputSelectType != InputSelectType_Auto))	
		{
			if(UserPrefInputSelectType != SrcInputType) 
				UserPrefInputType = SrcInputType = UserPrefInputSelectType;
			ChangeSource();
		}
	#endif
	if (SaveSettingFlag)//To Save The Monitor Setting When OSD Time Out
	{
		Clr_SaveSettingFlag();
		SaveUserPref();
	}

        Clr_OsdTimeoutFlag();
        if (DisplayLogoFlag)
        {
            #ifdef LEDFlash
            SK_TurnOn4LEDs();
            #endif
        
            Clr_DisplayLogoFlag();
            //printMsg("Clear Display Logo Flag");
            Power_TurnOffPanel();
            LoadCommonFont();
            //          InitOsdAfterLogo();
		#if (DisplayLogo==PuntaLOGO||DisplayLogo==WearnesLOGO)
		mStar_AdjustBackgoundColor(5);
		mStar_WriteByte(FWC, 0);
		#endif
        }
#if HotInputSelect
		if (MenuPageIndex==HotInputSelectMenu)
				{		
		#if (INPUT_TYPE == INPUT_1A1D)
					if (MenuItemIndex==InputSelectType_Analog)
						SetInputToAnalog();
					else if (MenuItemIndex==InputSelectType_Digital) 
							SetInputToDigital();
		#elif (INPUT_TYPE == INPUT_1A1H)
					if (MenuItemIndex==InputSelectType_Analog)
						SetInputToAnalog();
					else if (MenuItemIndex==InputSelectType_HDMI) 
							SetInputToHDMI();							
      	#elif (INPUT_TYPE == INPUT_1A2D)		
					if (MenuItemIndex==InputSelectType_Analog) 
						SetInputToAnalog();
					else if(MenuItemIndex==InputSelectType_Digital)
						SetInputToDigital();
					else if (MenuItemIndex==InputSelectType_HDMI)	
						SetInputToHDMI();	
		#endif
				}
		if(UnsupportedModeFlag)
		{
			Osd_Hide();
			Delay1ms(200);
			MenuPageIndex = UnsupportedModeMenu;
			MenuItemIndex = 0;
			#if ScanKey
				#if Scan_Source == Cypress
					#ifdef LEDBrightCtrlByScalerPWM
						SKLEDStatus = SKLEDBright_Dark;
						ScanKey_LEDBrightAction();
					#endif
				#endif
			#endif
			ExecuteKeyEvent(MIA_RedrawMenu);
		}
#endif		
#if INPUT_TYPE==INPUT_1A    //071225 show "no sync" after show "CableNotConnected" 
		if(MenuPageIndex==CableNotConnectedMenu)
		{	
		    Osd_Hide();
			Delay1ms(200);
		    MenuPageIndex=StandbyMenu;
		    ExecuteKeyEvent(MIA_RedrawMenu);
		}
#endif				
        if (MenuPageIndex > RootMenu)
        {

			//if ( MenuPageIndex == ExtraMenu && MenuItemIndex == 0)
			//{

			//	Clr_SaveSettingFlag();
			//	ReadMonitorSetting();
			//}
		if( MenuPageIndex == ResetMenu )	
		{
			if( DoResetFlag)
			{
				ResetAllSetting();
			}
		}
		
		if( (MenuPageIndex == ExtraMenu) && (MenuItemIndex == 1) && DoAutoConfigFlag)	//120328 Modify
			{
            			MenuPageIndex = AutoMenu;
			}
		else
            MenuPageIndex = RootMenu;
		
            MenuItemIndex = 0;
            ExecuteKeyEvent(MIA_RedrawMenu);
#if ScanKey
    #if Scan_Source == Synaptics
            Set_keyLEDTurnOffFlag();
    #endif    
#endif
        }
		EnterKeyCouter=0;
    }
    else if (DisplayLogoFlag)
    {
        if (FlashFlag)
        {
            Clr_FlashFlag();
            menuAction = mStar_ReadByte(BLENDL) &0xF;
            menuAction = (menuAction &0xF8) | ((menuAction &7) + 1);
            mStar_WriteByte(BLENDL, menuAction);
        }
    }
    //}
    if (!DisplayLogoFlag)
    // 2006/10/18 9:17PM by Emily for AC on but do burnin
    {
        if (DoBurninModeFlag && ChangePatternFlag)
        // for burnin mode
        {
            Clr_ChangePatternFlag();
            PatternNo = (PatternNo + 1) % 5;
            mStar_AdjustBackgoundColor(PatternNo);
        }
    }
    if (ms500Flag)
    // for moving osd position
    {
        Clr_ms500Flag();
        if (CurrentMenu.Flags &mpbMoving && !PowerSavingFlag)
        // && FreeRunModeFlag) // for led flash
        {
            if (ReverseXFlag)
            {
                if (moveX == 0)
                {
                    Clr_ReverseXFlag();
                }
                else if(moveX < 0)
                {
                    moveX = 0;
                }
                else if(moveX >100)
                {
                    moveX = 100;
                }
                else
                {
                    moveX -= 1;
                }
            }
            else
            {
                moveX++;
            }
            if (moveX >= 100)
            {
                Set_ReverseXFlag();
            }
            if (ReverseYFlag)
            {
                if (moveY == 0)
                {
                    Clr_ReverseYFlag();
                }
                else if(moveY < 0)
                {
                    moveY = 0;
                }
                else if(moveY >100)
                {
                    moveY = 100;
                }
                else
                {
                    moveY -= 2;
                }
            }
            else
            {
                moveY += 2;
            }
            if (moveY >= 100)
            {
                Set_ReverseYFlag();
            }
            //printData("--->osd X:%d",moveX);
            //printData("--->osd Y:%d",moveY);
#if OSD_PIVOT_EN  //071226
            if(OSDRotateFlag)
            Osd_SetPosition(100-moveX,100-moveY);				
	     else
 #endif	
            Osd_SetPosition(moveX, moveY);
        #if Enable_OsdEngine1
	     Osd_SetOsdEngine(1);
#if OSD_PIVOT_EN  //071226
            if(OSDRotateFlag)
            Osd_SetPosition(100-moveX,100-moveY);				
	     else
 #endif			
            Osd_SetPosition(moveX, moveY);
	     Osd_SetOsdEngine(0);
         #endif
        }
    }

#if FDDCCI_WriteEDID
    if(DDCCIResetPowerOffFlag)
	{
		Clr_DDCCIResetPowerOffFlag();
	#if F_ResetCheckEdidManueAndSN
		if(FactoryCheckEdidManufactureAndSN())
	#endif
		{
		ResetAllSetting();
		ExecuteKeyEvent(MIA_Power);
		}
	}	
#endif

#if  BlacklitTimeOnShow
	if ((BlacklitTimeOn_S==0)&&(BlacklitTimeOn_M>0)&&((BlacklitTimeOn_M %10)==0))//1800) == 0)
	{
	#if USEFLASH
			//MonitorBackLightTime=BlacklitTime;
			MonitorBlacklitTimeOn_S=BlacklitTimeOn_S;
	       	MonitorBlacklitTimeOn_M=BlacklitTimeOn_M;  //                     MonitorSetting.BlacklitTimeOn_M
              	MonitorBlacklitTimeOn_H=BlacklitTimeOn_H;   //MonitorSetting.BlacklitTimeOn_H
               	//MonitorBlacklitTimeOn_HH=BlacklitTimeOn_HH;
		
		SaveMonitorSetting2();//SaveMonitorSetting();
	#else
		SaveBlacklitTime();
	#endif
	}

  	if (MenuPageIndex==FactoryMenu)//((BlacklitTimeOn_S%60)==0)
 	{	
 			Draw_BlacklitTimeOn_Info_LGFactoryMenu();
  	}
#endif
	
		//Osd_Draw4Num(4,    1,		SrcModeIndex);
		//Osd_Draw4Num(4,    2,		SrcVTotal);
		//Osd_Draw4Num(4,    3,		SrcVsyncwidth);
		//Osd_DrawNum(4,    9,		CurrentMax);
		//Osd_DrawNum(4,    10,	DCR_DarkEnhance_Now);
		//Osd_DrawNum(4,    11,	DCR_BacklightAdjustValue);

	
	
}
Bool ExecuteKeyEvent(MenuItemActionType menuAction)
{
    Bool processEvent = TRUE;
    BYTE tempValue; //,Volume; wmz 20051019
    BYTE i;
	
    while (processEvent)
    {
	#if 0//DEBUG_ENABLE
	printData("menuAction==%d", menuAction);
	//printData("MenuPageIndex==%d", MenuPageIndex);
	//printData("EnterKeyCouter==%d", EnterKeyCouter);
	#endif
	
        processEvent = FALSE;
        switch (menuAction)
        {
            case MIA_IncValue:
            case MIA_DecValue:
			#if !AllInOneKey
			
			if ( (MainMenu < MenuPageIndex) &&  (MenuPageIndex<ResetMenu))
			{ 
			       if (EnterKeyCouter == 0 )
			       {
			           //  if ( MenuPageIndex == ImageSetupMenu && MenuItemIndex == 2)
					//	menuAction = ((menuAction == MIA_IncValue) ? (MIA_UpItem): ( MIA_DownItem));
					//else
					       menuAction = ((menuAction == MIA_IncValue) ? (MIA_DownItem): ( MIA_UpItem));
					processEvent = TRUE;
					break;
			       }
			}
			
			#endif
			if((TurboKeyCounter > 0)&&!(CurrentMenuItem.DisplayValue.DrawNumber)   //071225 adjust value once if not release key 
					&&!(CurrentMenuItem.DisplayValue.DrawGuage))
			{	Delay1ms(100);
				break;
			}
			#if !ScanKey
			if( TurboKeyCounter >0&&TurboKeyCounter<20)//20151121 lixw
			{
				break;
			}
			#endif
			#if DEBUG_ENABLE
			printData("UserPrefECOMode22222     ==%d", UserPrefECOMode);
			#endif
			#if UnifyHotDcrEco				
			if (MenuPageIndex == HotKeyECOMenu && UserPrefECOMode==(ECO_Nums-1))
			{
				#if DEBUG_ENABLE
				printData("UserPrefECOMode000     ==%d", UserPrefECOMode);
				#endif
				menuAction=MIA_DCR;
				processEvent=TRUE;
				break;
			}
                        #if Enable_Lightsensor
                        else if(MenuPageIndex == HotKeyDCRMenu)
                        {
				menuAction=MIA_iCare;
				processEvent=TRUE;
				break;
                        }
                        else if(MenuPageIndex == HotKeyiCareMenu)
                        {
				UserPrefECOMode=(ECO_Nums-1);
				MenuPageIndex=HotKeyECOMenu;
				MenuItemIndex=0;
//                            Osd_LoadOrgHotKeyPalette();
                            #if 0//DEBUG_ENABLE
                            printData("  reload", 1);
                            #endif
                        }
                        #else // !Enable_Lightsensor
                        if (MenuPageIndex == HotKeyDCRMenu) 
                        {
                        	#if DEBUG_ENABLE
                        	 printData("UserPrefECOMode1111     ==%d", UserPrefECOMode);
				#endif
                            UserPrefECOMode=(ECO_Nums-1);
                            MenuPageIndex=HotKeyECOMenu;
                            MenuItemIndex=0;
                        }
                        #endif                            
			#endif	
                if (CurrentMenuItemFunc.AdjustFunction)
                {
            #if ScanKey
                #ifdef EnableSlider
                BYTE tempcnt;
                for(tempcnt=0; tempcnt<SliderIncDecStep; tempcnt++)
                    if (CurrentMenuItemFunc.AdjustFunction(menuAction))
                #else
                    if (CurrentMenuItemFunc.AdjustFunction(menuAction))
                #endif
            #else
                    if (CurrentMenuItemFunc.AdjustFunction(menuAction))
            #endif
                    {

                   if (MenuPageIndex == HotKeyECOMenu)
                    {
                        Osd_SetTextColor(0, 0);//0x0A); // Dark Gray
                        for (i=4;i<=6;i++)
                        Osd_DrawContinuesChar(0,i,SpaceFont,OsdWindowWidth);

                        LoadHotKeyColorFont(UserPrefECOMode);
                        if(MenuPageIndex==HotKeyECOMenu)
                            Osd_DrawHotKeyIcon(UserPrefECOMode, 11, 4);
                    }
		     if( MenuPageIndex==HotKeyDCRMenu)
		     	{
			      LoadHotKeyDCRFont();
			#if UnifyHotDcrEco
				Osd_DrawHotKeyIcon(7, 11, 4);
			#else
			   if(UserPrefDcrMode)
                        	 Osd_DrawHotKeyIcon(7, 11, 4);
			   else
			   	{
                        	 	Osd_DrawHotKeyIcon(8, 11, 4);
			   	}			 
			#endif
			}
			 
#if HotExpansionAdjust
			if(MenuPageIndex==HotExpansionMenu)
			{
				LoadHotKeyExpansionFont();			
			#if WaitForDataBlanking
				mStar_WaitForDataBlanking();
			#endif
			if(UserprefExpansionMode==Expansion_Full)
				Osd_DrawHotKeyIcon(10, 11, 4);
			else 
				Osd_DrawHotKeyIcon(9, 11, 4);
				
			}
#endif			 

#if 0
                        if (MenuPageIndex == ColorTempSelectMenu)
                        {
                            Osd_SetTextColor(0, 0); //Center
                            Set_JustShowValueFlag();
                            Osd_SetTextColor(0, 6); //Center
                        }
#endif
                       // if (MenuPageIndex == LanguageMenu)
                        if (MenuPageIndex == OsdSetUpMenu && MenuItemIndex == 4)
                        {
 		#if Enable_Cache
		 MCU_EnableCache();
		#endif
                       
                            Osd_SetTextColor(0, 6); //Center
                            #ifdef EnableLightBar
                            for (i = 4; i <= 16; i++)
                            Osd_DrawContinuesChar(0,i,SpaceFont,29);
                            #else
                             for (i = 4; i <= 14; i++)
                            Osd_DrawContinuesChar(0,i,SpaceFont,18);
                             #endif


                            DynamicLoadFont(CurrentMenu.Fonts);
				//LoadCommonFont1();
#if Enable_OsdEngine1
				Osd_SetOsdEngine(1);
	            	       Osd_SetTextColor(0, 10);//0x0A); // Dark Gray
                            Osd_DrawContinuesChar(0,1,SpaceFont,OsdWindowWidth);
				Osd_SetOsdEngine(0);
#endif
				DrawOsdMenuItemRadioGroup(0, DrawOsdSetUpIconRatioText);
				DrawOsdMenuItemNumber(0, DrawOsdSetUpIconNumber);
				DrawOsdMenuItemGuage(0, DrawOsdSetUpIconGuage);
				Osd_Show();

 		#if Enable_Cache
		 MCU_DisableCache();
 		#endif

                        }

			   if ((MenuPageIndex == ColorBoostMenu) && (MenuItemIndex != 5))
                        {
 				Clr_JustShowValueFlag();
			       //for(i=0;i<5;i++)	
				//    DrawOsdMenuItem(i, &CurrentMenuItems[i]);
				if(DrawFullEnhanceFlag&&MenuItemIndex != 0)
				{
				DrawOsdMenuItem(0, &CurrentMenuItems[0]);
				Clr_DrawFullEnhanceFlag();
				}
				if(DrawSkinFlag&&MenuItemIndex != 1)
				{
				DrawOsdMenuItem(1, &CurrentMenuItems[1]);
				Clr_DrawSkinFlag();
				}
				if(DrawGreenFlag&&MenuItemIndex != 2)
				{
				DrawOsdMenuItem(2, &CurrentMenuItems[2]);
				Clr_DrawGreenFlag();
				}
				if(DrawBlueFlag&&MenuItemIndex != 3)
				{
				DrawOsdMenuItem(3, &CurrentMenuItems[3]);
				Clr_DrawBlueFlag();
				}
				if(DrawColorAutoDetectFlag&&MenuItemIndex != 4)
				{
				DrawOsdMenuItem(4, &CurrentMenuItems[4]);
				Clr_DrawColorAutoDetectFlag();
				}
				DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
				//DrawOsdMenuItem(BoostTempValue, &CurrentMenuItems[BoostTempValue]);
				//Set_JustShowValueFlag();
				//DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
				//BoostTempValue = (MenuItemIndex+5)%5;;
                        }
	
	#if 0
                        // immediately show setting value
                        if (MenuPageIndex >= ColorBoostMenu && MenuPageIndex <= DemoMenu)
                        {
                            DrawOsdMenuItemRadioGroup(MenuItemIndex, DrawColorBoostIconRatioText);
                         //   DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
                        }
	#endif          // 071221 modified for adjusting bf size should clear h/v position
	                    if (MenuPageIndex == BrightFrameMenu && 
                               (MenuItemIndex == 5 || MenuItemIndex == 0))
	                    {
	                        Set_JustShowValueFlag();
	                        if(PictureBoostFlag)
	                            Osd_SetTextColor(0, 6); //Center
	                        else
	                            Osd_SetTextColor( 0, 20 );

	                        DrawOsdMenuItemRadioGroup(0, DrawBrightFrameRatioText);
	                        DrawOsdMenuItemNumber(0, DrawBrightFrameIconNumber);
	                        DrawOsdMenuItemGuage(0, DrawBrightFrameIconGuage);
	                        Clr_JustShowValueFlag();
	                    }
                        if (MenuPageIndex == LuminanceMenu)
                        {
                          if(MenuItemIndex == 2)
                        	{
                            Set_JustShowValueFlag();
				if(UserPrefECOMode==ECO_Standard)
	                            Osd_SetTextColor(0, 6); //Center
                            else
	                            Osd_SetTextColor(0, 20 );

	                        DrawOsdMenuItemRadioGroup(0, DrawLuminanceContrastGroupText);
                            DrawOsdMenuItemNumber(0, DrawLuminanceIconNumber);
                            DrawOsdMenuItemGuage(0, DrawLuminanceIconGuage);
                            Clr_JustShowValueFlag();
                            Osd_SetTextColor(0, 6); //Center
                        	}
			    else if(MenuItemIndex == 4)
			    	{
			    		if(UserPrefColorTemp != CTEMP_SRGB) //YF20071218
			    		
	                    {
	                        Set_JustShowValueFlag();
	                        if(!UserPrefDcrMode)
	                            Osd_SetTextColor(0, 6); //Center
	                        else
	                            Osd_SetTextColor( 0, 20 );

	                        DrawOsdMenuItemRadioGroup(0, DrawLuminanceMenuItemGroupText);
	                        DrawOsdMenuItemNumber(0, DrawLuminanceIconNumber);
	                        DrawOsdMenuItemGuage(0, DrawLuminanceIconGuage);
	                        Clr_JustShowValueFlag();
			    		}
				else
				{
				   if (UserPrefDcrMode)
				   	UserPrefColorTemp = CTEMP_Warm1;
				   Set_JustShowValueFlag();
	                        if(!UserPrefDcrMode && (UserPrefColorTemp != CTEMP_SRGB))
	                            Osd_SetTextColor(0, 6); //Center
	                        else
	                            Osd_SetTextColor( 0, 20 );

	                        DrawOsdMenuItemRadioGroup(0, DrawGamaDcrGroupText);                    
	                       
	                        Clr_JustShowValueFlag();
						}
			    	}
                        #if Enable_Lightsensor
                            else if(MenuItemIndex == 5) // icare item
                            {
                                Set_JustShowValueFlag();
                                
                                if(UserPrefiCareMode)
                                {
                                    Osd_SetTextColor(0, 20);
                                    //UserPrefBrightness = UserPrefBrightnessWarm1 = DefBrightness;
                                    //UserPrefContrast = UserPrefGreenColorWarm1 = DefContrast;
                                    //  TempBrightness= DefBrightness; // 091002 coding, reserved
                                }
                                else
                                    Osd_SetTextColor(0, 6);
                                
                                DrawOsdMenuItemRadioGroup(0, DrawiCareAdjustGroupText);    
                                DrawOsdMenuItemNumber(0, DrawBrightnessNumber);
                                DrawOsdMenuItemGuage(0, DrawBrightnessGuage);
                                
                                Osd_SetTextColor(0, 6);
                                DrawOsdMenuItemRadioGroup(0, DrawiCareAdjustGroupText_Blue);   
                                DrawOsdMenuItemNumber(0, DrawContrastNumber);
                                DrawOsdMenuItemGuage(0, DrawContrastGuage);
                                
                                Clr_JustShowValueFlag();
                            }
                        #endif
                        }
/*
                        Set_JustShowValueFlag();
                        DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
                        Clr_JustShowValueFlag();
                        Osd_SetTextColor(0, 6); //Center

                        if (MenuPageIndex == ColorTempMenu ) //&& MenuItemIndex == 0)
                        {
                            Set_JustShowValueFlag();

				if(UserPrefColorTemp==CTEMP_USER)
	                            Osd_SetTextColor(0, 6); //Center
                            else
	                            Osd_SetTextColor( 0, 20 );

                            //**********eshin 070228******************************
                            //DrawOsdMenuItemText(0, UserColorRedMenuItems);
                            //DrawOsdMenuItemText(0, UserColorGreenMenuItems);
                            //DrawOsdMenuItemText(0, UserColorBlueMenuItems);
                            #if 0//Enable_YMC
                                DrawOsdMenuItemText(0, UserColorYMenuItems);
                                DrawOsdMenuItemText(0, UserColorCMenuItems);
                                DrawOsdMenuItemText(0, UserColorMMenuItems);
                            #endif
                            //***************************************************
	                     DrawOsdMenuItemRadioGroup(0, DrawColorTempIconRatioText);
                            DrawOsdMenuItemNumber(0, DrawColorTempIconNumber);
                            DrawOsdMenuItemGuage(0, DrawColorTempIconGuage);
                           Clr_JustShowValueFlag();
                            Osd_SetTextColor(0, 6); //Center

							
                        }*/
                        if(MenuPageIndex == FactoryMenu
				#if EnablePanelServiceMenu
				||MenuPageIndex==AdjustPanelMenu
				#endif
				)
                        {
                            DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
                        }
                        else
                        {
                            Set_JustShowValueFlag();
                            DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
                            Clr_JustShowValueFlag();
                            Osd_SetTextColor(0, 6); //Center

                            // 071221 modified for color temp menu item color
                            if(MenuPageIndex == ColorTempMenu && MenuItemIndex == 0)//&&!UserPrefDcrMode)
                            {
                            /*
					if(UserPrefColorTemp == CTEMP_SRGB) 
			    		{
	 					UserPrefGamaMode = GAMA1;
						//mStar_SetupGamma(UserPrefGamaMode);
					}
					//*/		
                                if(UserPrefColorTemp==CTEMP_USER&&!UserPrefDcrMode)
    	                            Osd_SetTextColor(0, 6);
                                else
    	                            Osd_SetTextColor( 0, 20);
                                Set_JustShowValueFlag();
    	                         DrawOsdMenuItemRadioGroup(0, DrawColorTempIconRatioText);
                                DrawOsdMenuItemNumber(0, DrawColorTempIconNumber);
                                DrawOsdMenuItemGuage(0, DrawColorTempIconGuage);
                                Clr_JustShowValueFlag();
                            }
                        }

                        Set_SaveSettingFlag();
				#if 0//EnablePanelServiceMenu
			   if(MenuPageIndex==AdjustPanelMenu)
			   	Set_SavePanelSettingFlag();
				#endif

                #if ScanKey
                    #ifdef EnableSlider
                        if(SliderIncDecStep>1)
                        {
                                Delay1ms(20);
                                TurboKeyCounter = 7;
                                SliderIncDecStep--;
                        }
                        else
                        {
                            if (TurboKeyCounter < 7)
                            {
                                Delay1ms(200);//(100);
                            }
                        }
                    #else
                        if (TurboKeyCounter < 7)
                        {
                            Delay1ms(200);//(100);
                        }
                    #endif
                //#else // !ScanKey
                //        if (TurboKeyCounter < 2)//5)
                //        {
                //            Delay1ms(200);//(100);
                //        }
                #endif

                #ifdef EnableLightBar // 090706 reserved
                        //if(MenuPageIndex==OsdSetUpMenu && MenuItemIndex==5)
                        //    KeypadButton = BTN_Repeat;
                #endif
                		 /*
                        if (CurrentMenuItemFunc.AdjustFunction == AdjustAddr || CurrentMenuItemFunc.AdjustFunction == AdjustBankNo)
                        {
                            DrawOsdMenuItemNumber(0, DrawAddrRegValueNumber);
                            //TurboKeyCounter = 0;
                        }
						//*/

                    }
                }
                break;
            case MIA_UpItem:
            case MIA_DownItem:
            case MIA_NextItem:
            case MIA_PrevItem:
		#if AllInOneKey//For Press Up & Down Key To Save the Monitor Setting
			if (SaveSettingFlag)
			{
				Clr_SaveSettingFlag();
				SaveUserPref();
			}
		#endif
		#if WaitingForDrawOSDSUB
			if(FactoryModeFlag&&MenuPageIndex == MainMenu)
		#endif
		#if !Enable_OsdEngine1
				 DisableOsdSubWindow(SUB_WIN_00);
				 //Delay1ms(8); //zjb20071219
		#endif
		#if MWEFunction
		     Set_DrawItemRadioGroupScikeFlag();
		#endif
                //PrevMenuItemIndex=MenuItemIndex;
                UserPrefInputType=SrcInputType;//For OSD Input Type Display Error When User Return To Last Menu           Zhifeng.Wu 20071128
              #if INPUT_TYPE!=INPUT_1A
	        if(UserPrefInputSelectType != InputSelectType_Auto)
	        {
	                UserPrefInputSelectType = SrcInputType;
	        } 
		#endif
                tempValue = MenuItemIndex;
               //printData("menuAction=%x", menuAction);
                if (menuAction == MIA_NextItem)
                {	ItemStep =0;
                    MenuItemIndex = GetNextItem(CurrentMenuItems);
                }
                else if(menuAction == MIA_PrevItem)
                {	ItemStep =0;
                    MenuItemIndex = GetPrevItem(CurrentMenuItems);
            #if !Enable_Lightsensor
			if((MenuPageIndex==LuminanceMenu)&&( UserPrefDcrMode==1))  // AllInOneKey //for the osd error when DCR ON 
			 MenuItemIndex=4;
            #endif
                }
                else if (menuAction == MIA_DownItem)
                {	ItemStep =3;
                    MenuItemIndex = GetNextItem(CurrentMenuItems);
                }
                else//if (menuAction == MIA_UpItem)
                {	ItemStep =6;
                    MenuItemIndex = GetPrevItem(CurrentMenuItems);
            #if !Enable_Lightsensor
			if((MenuPageIndex==LuminanceMenu)&&( UserPrefDcrMode==1))  // !AllInOneKey // 
			 MenuItemIndex=4;
            #endif                
                }
		#if AllInOneKey
		#if EnablePanelServiceMenu
		if((MenuPageIndex==FactoryMenu||MenuPageIndex==AdjustPanelMenu)&&menuAction == MIA_PrevItem)
		#else
		if(MenuPageIndex==FactoryMenu&&menuAction == MIA_PrevItem)
		#endif
			MenuItemIndex=MenuItemIndex+1;
		#endif
                if (tempValue != MenuItemIndex)
                {
                #if 0
                    if (CurrentMenuItem.Flags &mibClrInner)
                    {
                        ClrInnerOsd();
                    }
                #endif
                    if (MenuPageIndex == ColorTempMenu)
                    {
                        for (i = 0; i < MenuItemCount; i++)
                        {
                            DrawOsdMenuItem(i, &CurrentMenuItem);
                        }
                    }
		#if 0
                    if (MenuPageIndex == InputSelectMenu)
                    {
                        UserPrefInputSelectType = MenuItemIndex;
                    }
              #endif
		#if Enable_Cache
		 MCU_EnableCache();
		#endif
                    if (CurrentMenuItem.Fonts)
                    {
                        DynamicLoadFont(CurrentMenuItem.Fonts);
                    }
			#if WaitingForDrawOSDSUB
				if(FactoryModeFlag&&MenuPageIndex == MainMenu)
					#if WaitForDataBlanking  //zjb20071219
					mStar_WaitForDataBlanking();
					#endif
				else
				{if(MenuPageIndex==MainMenu)
					WaitingForOSDFlsh=1;}
			#else
			#if WaitForDataBlanking  //zjb20071219
                        mStar_WaitForDataBlanking();
                     #endif
			#endif
		
                    DrawOsdMenuItem(tempValue, &CurrentMenuItems[tempValue]);

			#if WaitingForDrawOSDSUB
				//if(MenuPageIndex==MainMenu)
				WaitingForOSDFlsh=0;
			#endif

			if( !(AllInOneKey&&(MenuPageIndex==FactoryMenu
			#if EnablePanelServiceMenu
			||MenuPageIndex==AdjustPanelMenu
			#endif
			)))
			{
	                    Set_JustShowValueFlag();
			}
			
                    DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
                    Clr_JustShowValueFlag();
		#if Enable_Cache
		MCU_DisableCache();
		#endif
			//if (MenuPageIndex == MainMenu)
                        //DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
                        #if MWEFunction
		      Clr_DrawItemRadioGroupScikeFlag();
			   #endif
                    #if !AllInOneKey
                        if (MenuPageIndex == FactoryMenu)
                        // for factory mode
                        {
                            if ((MenuItemIndex == 13 && tempValue < 13) || (MenuItemIndex == 21 && tempValue > 22))
                            {
                                SetFactoryColorTempCool1();
                                //Set_SaveSettingFlag();
                            }
                            else if ((MenuItemIndex == 23 && tempValue < 23) || (MenuItemIndex == 31 && tempValue > 32))
                            {
                                SetFactoryColorTempNormal();
                                //Set_SaveSettingFlag();
                            }
                            else if ((MenuItemIndex == 33 && tempValue < 33) || (MenuItemIndex == 41 && tempValue > 42))
                            {
                                SetFactoryColorTempWarm1();
                                //Set_SaveSettingFlag();
                            }
                            else if ((MenuItemIndex == 43 && tempValue < 43) || (MenuItemIndex == 51 && tempValue > 52))
                            {
                                SetFactoryColorTempSRGB();
                                //Set_SaveSettingFlag();
                            }
                            #if 1 //2006-08-03 Andy Due To Color Setting Not Complete
                                if (SaveSettingFlag)
                                {
			 #if USEFLASH
					SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
					if ((tempValue >= 54+Enable_ProductModeAdjust*2+PANEL_VCOM_ADJUST*2)&&//Burn In Flag		//121031 Modify
						(tempValue <= 58+Enable_ProductModeAdjust*2+EnablePanelServiceMenu*4+MS_OVD*2+PANEL_VCOM_ADJUST*2)//Addr		//121031 Modify
						)
						SaveMonitorSetting();//UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
                                    
					    Clr_SaveSettingFlag();
			 #else
					SaveFactorySetting(tempValue);
			 #endif
                                }
                            #endif
                        }
                    #endif
                    #if 0//EnablePanelServiceMenu
				if(MenuPageIndex == AdjustPanelMenu&&SavePanelSettingFlag)
                                {
                                    #if PanelSettingInFlash
					     SavePanelSetting();				
					 #else
                                        SavePanelSettingByItem(tempValue);
					 #endif
					 Clr_SavePanelSettingFlag();
                                }
                    #endif 
                    //Delay1ms(200);
                    Delay1ms(200);

                    #if 0//ScanKey // 090517 coding
                        #if Scan_Source == Cypress
                        Delay1ms(100);
                        #endif
                    #endif
                    
                }
                break;
            case MIA_JumpAdjustItem:
            case MIA_JumpNaviItem:
             tempValue = MenuItemIndex;
                MenuItemIndex += ((menuAction == MIA_JumpAdjustItem) ? (1): ( - 1));
		#if AllInOneKey
			#if EnablePanelServiceMenu
				if((MenuPageIndex==AdjustPanelMenu)&&menuAction == MIA_JumpAdjustItem)
					{
					if(MenuItemIndex==18)
					MenuItemIndex=(MenuItemIndex+5)%MenuItemCount;
					else
					MenuItemIndex=(MenuItemIndex+1)%MenuItemCount;
					}
				if((MenuPageIndex==AdjustPanelMenu)&&menuAction == MIA_JumpNaviItem)
					{
					if(MenuItemIndex==22)
					{
					MenuItemIndex=(MenuItemIndex?MenuItemIndex:MenuItemCount)-5;	
					}
					else
					MenuItemIndex=(MenuItemIndex?MenuItemIndex:MenuItemCount)-1;	
					}
			#endif
			if(MenuPageIndex==FactoryMenu&&menuAction == MIA_JumpAdjustItem)
				MenuItemIndex=MenuItemIndex+1;
			if(MenuPageIndex==FactoryMenu&&menuAction == MIA_JumpNaviItem)
				MenuItemIndex=MenuItemIndex-1;		
		#endif
               #if 1//AllInOneKey
					if (MenuPageIndex == FactoryMenu)
					// for factory mode
					{
						if ((MenuItemIndex == 14 && tempValue < 14) || (MenuItemIndex == 22 && tempValue > 22))
						{
							SetFactoryColorTempCool1();
							//Set_SaveSettingFlag();
						}
						else if ((MenuItemIndex == 24 && tempValue < 24) || (MenuItemIndex == 32 && tempValue > 32))
						{
							SetFactoryColorTempNormal();
							//Set_SaveSettingFlag();
						}
						else if ((MenuItemIndex == 34 && tempValue < 34) || (MenuItemIndex == 42 && tempValue > 42))
						{
							SetFactoryColorTempWarm1();
							//Set_SaveSettingFlag();
						}
						else if ((MenuItemIndex == 44 && tempValue < 44) || (MenuItemIndex == 52 && tempValue > 52))
						{
							SetFactoryColorTempSRGB();
							//Set_SaveSettingFlag();
						}
	      	#if 1 //2006-08-03 Andy Due To Color Setting Not Complete
							if (SaveSettingFlag)
							{
							   
			 #if USEFLASH
					SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
					if ((tempValue >= 54+Enable_ProductModeAdjust*2+PANEL_VCOM_ADJUST*2)&&//Burn In Flag		//121031 Modify
						(tempValue <= 58+Enable_ProductModeAdjust*2+EnablePanelServiceMenu*4+MS_OVD*2+PANEL_VCOM_ADJUST*2)//Addr		//121031 Modify
						)
						SaveMonitorSetting();//UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
					    Clr_SaveSettingFlag();
			 #else
					SaveFactorySetting(tempValue);
			 #endif
			 /*if(tempValue)
			 	{
                                    menuAction = MIA_ExecFunc;
                                    processEvent = TRUE;
                                    break;
			 	}
			 */
							}
		     #endif

					}
         #endif
                 #if 0//EnablePanelServiceMenu
			if(MenuPageIndex == AdjustPanelMenu&&SavePanelSettingFlag)
                            {
                                #if PanelSettingInFlash
				     SavePanelSetting();				
				 #else
                                    SavePanelSettingByItem(tempValue);
				 #endif
				 Clr_SavePanelSettingFlag();
                            }
                #endif 
                //printData("MenuItem Index %d",MenuItemIndex);
                DrawOsdMenuItem(tempValue, &CurrentMenuItems[tempValue]);
                DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
                Delay1ms(200);
                break;

            case MIA_ExitGotoNext:
            case MIA_ExitGotoPrev:
            case MIA_ExecGotoPrev:
				
                if ((MenuPageIndex > MainMenu) && (CurrentMenu.Flags&mpbRedraw))
                {
                	if(menuAction ==MIA_ExecGotoPrev)
                    {
                        CurrentMenu.ExecFunction();

                    }
	                PrevPage = MenuPageIndex;
	                MenuPageIndex = (menuAction == MIA_ExitGotoNext) ? (NextMenuPage): (PrevMenuPage);
	                if (PrevPage == MainMenu && MenuPageIndex == RootMenu)
	                {
	                  #if !AOC_MENU_FRAME_MODE
	                    if (!FactoryModeFlag)
			    #endif
	                        UserPrefLastMenuIndex = MenuItemIndex;
	                }
	                if (PrevPage == RootMenu && MenuPageIndex == MainMenu && OsdLockModeFlag)
	                    MenuPageIndex = OsdLockMenu;
	#if 0
	                if (MenuPageIndex == InputSelectMenu)
	                {
	                    UserPrefInputSelectType = UserPrefInputType;
	                }
	#endif
#if 0
	                //Jeff 1112 //======================
	                if(/*MenuPageIndex==ColorTempSelectMenu&&*/UserPrefColorTemp==CTEMP_USER&&(UserPrefDcrMode==1||FullEnhanceFlag||
						PictureBoostFlag||SkinFlag||BlueFlag||GreenFlag||DemoFlag||ColorAutoDetectFlag))
	                {
	                    msAccOnOff(0); // 2006/11/9 11:27PM by Emily test
	                    UserPrefColorFlags = 0;
	                    UserPrefDcrMode = 0;
	                    ReadColorTempSetting();
	                    Clr_PictureBoostFlag();
	                    SetBFSize(UserPrefBFSize);//SetBFSize(); // jeff add in 1112
	                    Clr_SkinFlag();
	                    Clr_BlueFlag();
	                    Clr_GreenFlag();
	                    Clr_DemoFlag();
	                    Clr_ColorAutoDetectFlag();
	                    Clr_FullEnhanceFlag();
	                    SetColorBoostWindow(COLORBOOST_OFF);
	                }
#endif
	                //=========================
                    #if DefaultIconExit//AOC_37_Series || ModelName==ENVISION_P2071l
                        if(MenuPageIndex == MainMenu)
                            MenuItemIndex = 8;
                        else
                            MenuItemIndex = 0;
                    #else
	                if ((PrevPage == RootMenu && MenuPageIndex == MainMenu))
	                {
	                    // In factory mode or in DVI but stay in ImageSetup page, default in menuitemindex 0
	                    if ((!FactoryModeFlag) && !(SrcInputType == Input_Digital && UserPrefLastMenuIndex == 1))
	                        MenuItemIndex = UserPrefLastMenuIndex;
	                    else
	                        MenuItemIndex = 0;
	                    // while enter factory, then stay in Lum.
	                }
	                else
	                    MenuItemIndex = GetMenuItemIndex(PrevPage);
                    #endif

	                Clr_PressExitFlag();
	                Clr_EnableShowDDCFlag();
	                Clr_PressMenuFlag();
	                Clr_MuteFlag();
	                DrawOsdMenu();
	                if(menuAction == MIA_ExitGotoNext)
		                menuAction = MIA_NextItem;
	                else
		                menuAction = MIA_PrevItem;
	                processEvent = TRUE;
                }
                break;
            case MIA_GotoNext:
            case MIA_GotoPrev:

        		if (MenuPageIndex == ExtraMenu && MenuItemIndex == 0 )
			{
				#if DEBUG_PRINTDATA
				printData("PrevPage=%d", PrevPage);
				printData("MenuItemIndex=%d", MenuItemIndex);
				#endif

			#if INPUT_TYPE!=INPUT_1A
                            if(UserPrefInputSelectType == InputSelectType_Auto)
                            {
                                SwitchPortCntr = 0;  //071226
                                if(!SyncLossState())
                                    ;
                            }
				else
			#endif		
				{
				    // 071221 modified for it is unnecessary to execute 
				    // when current SrcInputType is same with UserPrefInputSelectType
                                if(UserPrefInputSelectType != SrcInputType) 
                                {
                                    UserPrefInputType = SrcInputType = UserPrefInputSelectType;
									Set_SaveSettingFlag();
                                    menuAction = MIA_ExecFunc;
                                    processEvent = TRUE;
                                    break;
                                }
				}
			}
                UserPrefInputType=SrcInputType;//For OSD Input Type Display Error When Time out            Zhifeng.Wu 20071128
                    
	         PrevPage = MenuPageIndex;
                MenuPageIndex = (menuAction == MIA_GotoNext) ? (NextMenuPage): (PrevMenuPage);
				
                if (PrevPage == MainMenu && MenuPageIndex == RootMenu)
                {
                 #if !AOC_MENU_FRAME_MODE
                    if (!FactoryModeFlag)
		   #endif
                        UserPrefLastMenuIndex = MenuItemIndex;
#if ScanKey // led key off 071227
    #if Scan_Source == Synaptics
                    if(0)//(MenuItemIndex == 8)
                    {
                        KeyLedCounter = 0;
                        Set_keyLEDTurnOffFlag();
                    }
    #endif                    
#endif
                }
                if (PrevPage == RootMenu && MenuPageIndex == MainMenu && OsdLockModeFlag)
                    MenuPageIndex = OsdLockMenu;

                if((!DoAutoConfigFlag && MenuPageIndex == AutoMenu)
                  ||(!DoResetFlag &&MenuPageIndex == ResetWaitMenu))
                    MenuPageIndex = MainMenu;
				#if 0
				if(DoResetFlag&&MenuPageIndex == ResetWaitMenu)    //071225
				{
					Osd_Hide();
					Delay1ms(30);
				}
				#endif
#if 0
                if (MenuPageIndex == InputSelectMenu)
                {
                    UserPrefInputSelectType = UserPrefInputType;
                }
#endif
#if 0
                //Jeff 1112 //======================
                if(/*MenuPageIndex==ColorTempSelectMenu&&*/UserPrefColorTemp==CTEMP_USER&&(UserPrefDcrMode==1||FullEnhanceFlag||
					PictureBoostFlag||SkinFlag||BlueFlag||GreenFlag||DemoFlag||ColorAutoDetectFlag))
                {
                    msAccOnOff(0); // 2006/11/9 11:27PM by Emily test
                    UserPrefColorFlags = 0;
                    UserPrefDcrMode = 0;
                    ReadColorTempSetting();
                    Clr_PictureBoostFlag();
                    SetBFSize(UserPrefBFSize);//SetBFSize(); // jeff add in 1112
                    Clr_SkinFlag();
                    Clr_BlueFlag();
                    Clr_GreenFlag();
                    Clr_DemoFlag();
                    Clr_ColorAutoDetectFlag();
                    Clr_FullEnhanceFlag();
                    SetColorBoostWindow(COLORBOOST_OFF);
                }
#endif
                //=========================
                /*if (PrevPage == ColorTempSelectMenu && MenuPageIndex == ColorTempMenu)
                {
                    SetColorTemp(); // for set srgb
                }*/
/*                 //eshin for fatorymenu to root menu may see rubbish
               if (FactoryModeFlag && PrevPage == FactoryMenu && MenuPageIndex == RootMenu)
                {
                    Osd_Hide(); // 2006/8/25 6:31PM by Emily for hide the garbage
                    LoadCommonFont();
                }
*/
            #if DefaultIconExit//AOC_37_Series || ModelName==ENVISION_P2071l
                // 091001 coding, main menu always stays at exit for 37-series
                if(MenuPageIndex == MainMenu)
                    MenuItemIndex = 8; // always statys at exit    
                else
                    MenuItemIndex = 0;
            #else
                if ((PrevPage == RootMenu && MenuPageIndex == MainMenu))
                {
                    // In factory mode or in DVI but stay in ImageSetup page, default in menuitemindex 0
                    #if !DefaultIconLumi//modify 20130102 for ID08 new spec request.
                    if ((!FactoryModeFlag) && !((SrcInputType == Input_Digital || SrcInputType == Input_Digital2) && UserPrefLastMenuIndex == 1))//yuanfa
                        MenuItemIndex = UserPrefLastMenuIndex;
          		 else
		    #endif
			 if (FactoryModeFlag)
			 	MenuItemIndex = 9;
			 else
                        MenuItemIndex = 0;
                    // while enter factory, then stay in Lum.					
                }
                else
                      MenuItemIndex = GetMenuItemIndex(PrevPage);
            #endif
                
         #if 1
                if(PrevPage == MainMenu || MenuPageIndex==MainMenu) // for clean garbage
                {
                    Osd_Hide();
                    Delay1ms(200);
                    //ClrInnerOsd();
                }
#endif
                Clr_PressExitFlag();
                Clr_EnableShowDDCFlag();
                Clr_PressMenuFlag();
                Clr_MuteFlag();
 		#if MWEFunction
 		  Clr_DrawItemRadioGroupScikeFlag();
 		#endif
	if(PrevPage==ResetMenu||MenuPageIndex == ResetWaitMenu)
	 if(DoResetFlag)
		{ 
			ResetAllSetting();
			MenuPageIndex=MainMenu;
                #if DefaultIconExit//AOC_37_Series || ModelName==ENVISION_P2071l
                    MenuItemIndex = 8;
                #endif
			Osd_Hide();
			Delay1ms(200);//To Hide The Garbage When Return To Main Menu      Zhifeng.Wu 080320
		}

            #if 0//ScanKey // 090515 coding for key delay period
                #if Scan_Source == Cypress
                //KeypadButton = BTN_Repeat;
                if(PrevPage == RootMenu || MenuPageIndex==MainMenu)
                    Delay1ms(300);
                #endif
            #endif
     
            case MIA_RedrawMenu:
/*                if (FactoryModeFlag && PrevPage == FactoryMenu)
                {
                    Osd_Hide(); // 2006/8/25 6:31PM by Emily for hide the garbage
                    LoadCommonFont();
                }
*/
                #if DDCCI_ENABLE
                    if (MenuPageIndex == DdcciInfoMenu)
                        CurrentMenu.ExecFunction();
                #endif

		#if Enable_Cache
		 MCU_EnableCache();
		#endif
               DrawOsdMenu();
 		#if Enable_Cache
		 MCU_DisableCache();
 		#endif

                #if DDCCI_ENABLE
                    if (MenuPageIndex == DdcciInfoMenu)
                        Delay1ms(200);
                    //Benz 2007.4.24   21:45:31  so bad solution but no time cover it
                #endif
                
                #if DDCCI_ENABLE
                    if (CurrentMenu.ExecFunction && MenuPageIndex != DdcciInfoMenu)
                #else
                    if (CurrentMenu.ExecFunction)
                #endif
                    {
                        tempValue = CurrentMenu.ExecFunction();
                        if (CurrentMenu.ExecFunction == AutoColor)
                        {
                            Osd_SetTextColor(0, 6);
                            if (tempValue)
                            {
                                Osd_DrawPropStr(10, 3, AutoColorPassText());
                            }
                                else
                            {
                                Osd_DrawPropStr(10, 3, AutoColorFailText());
                            }
                            Delay1ms(200);
                        }
                    }
                    if (!(CurrentMenu.Flags &mpbStay))
                    {
                        processEvent = TRUE;
                        menuAction = MIA_GotoPrev;
                    }
                    if (SaveSettingFlag)
                    {
                        SaveUserPrefSetting(PrevPage);
                    }

            #if ScanKey
                #ifdef LEDBrightCtrlByScalerPWM
                    if(PowerOnFlag)
                    {
                        #if 1 // 091020 coding for key LED behavior, GPM request
                        if(MenuPageIndex == RootMenu || MenuPageIndex == LogoMenu)
                        {
                            KeyLedCounter = 0;
                            SKLEDStatus = SKLEDBright_Dark;
                        }
                        else if(MenuPageIndex >= MainMenu && MenuPageIndex <= OsdLockMenu)
                        {
                            KeyLedCounter = 0;
                            SKLEDStatus = SKLEDBright_Light;
                        }
                        #else
                        if(MenuPageIndex == RootMenu)
                            KeyLedCounter = SKLEDTurnDarkTime;
			   else
                            KeyLedCounter = 0;
                        SKLEDStatus = SKLEDBright_Light;
                        #endif
                    }
                    else
                    {
                        KeyLedCounter = 0;
                    }
                #endif      
            #endif                    
                
                break;
            case MIA_ExecFunc:
				
                if (CurrentMenuItemFunc.ExecFunction)
                {
                    tempValue = 0;
		    #if 0//EnablePanelServiceMenu
                    if (CurrentMenuItemFunc.ExecFunction == Test&&PanelSelect==1)
                    {
				menuAction = MIA_RedrawMenu;
				MenuPageIndex=AdjustPanelMenu;
				#if AllInOneKey
					MenuItemIndex = 1;
				#else
					MenuItemIndex = 0;
				#endif
				processEvent = TRUE;
				break;
                    }
					/*
                    if (CurrentMenuItemFunc.ExecFunction == InitialPanelSetting||CurrentMenuItemFunc.ExecFunction == SaveCurrentPanelSetting)
                    {
                    		processEvent = CurrentMenuItemFunc.ExecFunction();
				if(processEvent)
				{
					menuAction = MIA_RedrawMenu;
					MenuPageIndex=AdjustPanelMenu;
					processEvent = TRUE;
					break;
				}
                    }
			//*/
		   #endif
                    if (CurrentMenuItemFunc.ExecFunction == AutoColor&&MenuPageIndex==FactoryMenu)
                    {
                        Osd_SetTextColor(CP_WhiteColor, CP_BlueColor);
                    }
                    processEvent = CurrentMenuItemFunc.ExecFunction();
                    if (tempValue == 1)
                    {
                        Osd_SetTextColor(CP_BlueColor, CP_WhiteColor);
                        DrawOsdMenuItemValue(MenuItemIndex, &CurrentMenuItem.DisplayValue);
                    }
                    if (CurrentMenuItemFunc.ExecFunction == AutoColor&&MenuPageIndex==FactoryMenu)
                    {
                        Osd_SetTextMonoColor(CP_RedColor, CP_BlueColor);
                        if (processEvent)
                        {
                            Osd_DrawRealStr(12, 1, PassText());
                        }
                            else
                        {
                            Osd_DrawRealStr(12, 1, FailText());
                        }
                        #if 1 //2006-08-03 Andy
                            DrawOsdMenuItemNumber(0, DrawGainRNumber);
                            DrawOsdMenuItemNumber(0, DrawGainGNumber);
                            DrawOsdMenuItemNumber(0, DrawGainBNumber);
							#ifdef TSUMXXT		//110316 Modify
							Osd_SetTextMonoColor(CP_GrayColor, CP_BlueColor);
							#endif
                            DrawOsdMenuItemNumber(0, DrawOffsetRNumber);
                            DrawOsdMenuItemNumber(0, DrawOffsetGNumber);
                            DrawOsdMenuItemNumber(0, DrawOffsetBNumber);
                        #endif
                    }
         		#if HotInputSelect
                    if(MenuPageIndex==HotInputSelectMenu)
					   {
						if(processEvent==FALSE)	
						{
						    if(PrevPage==UnsupportedModeMenu)
							{
							Osd_Hide();
						    moveX = 50;
               				moveY = 50;
							
							#if ScanKey
								#if Scan_Source == Cypress
									#ifdef LEDBrightCtrlByScalerPWM
										SKLEDStatus = SKLEDBright_Dark;
										ScanKey_LEDBrightAction();
									#endif
								#endif
							#endif
							}
							MenuPageIndex=PrevPage;
							MenuItemIndex=0;
							menuAction=MIA_RedrawMenu;	
							processEvent=TRUE;
							break;
						}
								
					   }
				#endif	   
                    processEvent = FALSE;
                    // 2006/11/15 3:45PM by KK for special request
                    #if DDCCI_ENABLE
                        if (MenuPageIndex == DdcciInfoMenu)
                        {
                            menuAction = MIA_Exit;
                            MenuPageIndex = RootMenu;
                            MenuItemIndex = 0;
                            processEvent = TRUE;
                        }
                    #endif
	#if 0
                    if (MenuPageIndex == InputSelectMenu && processEvent == FALSE)
                    {
                        menuAction = MIA_GotoPrev;
                        MenuItemIndex = 0;
                        processEvent = TRUE;
                    }
	#endif
                }
                break;
            case MIA_Exit:
			
                if (PrevPage == MainMenu && MenuPageIndex == RootMenu)
                {
                     #if !AOC_MENU_FRAME_MODE
                    if (!FactoryModeFlag)
			#endif
                            UserPrefLastMenuIndex = MenuItemIndex;
                }
                if (SaveSettingFlag)
                {
                    Clr_SaveSettingFlag();
                    SaveUserPref();
                }
		#if (ModelName == AOC_2219Vwg||ModelName==AOC_2219Vg||ModelName==ENVISION_G2219_LM220WE1||\
			ModelName==AOC_2219P2_2883||ModelName==AOC_2219Vwg5||ModelName==AOC_2219P2_3329||ModelName==ENVISION_G2470||ModelName==ENVISION_G2770||ModelName==ENVISION_G2360g||ModelName==ENVISION_G2218WG1||\
			ModelName==AOC_e2795v||ModelName==ENVISION_G2770L||ModelName==AOC_2219P2_3834 )
		if(UnsupportedModeFlag)
		{
			Osd_Hide();
  			moveX = 50;
			moveY = 50;
			MenuPageIndex=UnsupportedModeMenu;
			MenuItemIndex=0;
			menuAction=MIA_RedrawMenu;	
			processEvent=TRUE;
			break;
		}
		else if(PrevPage == RootMenu&& MenuPageIndex == HotInputSelectMenu&&SyncLossState())
		{
			Osd_Hide();
  			MenuPageIndex=StandbyMenu;
			MenuItemIndex=0;
			menuAction=MIA_RedrawMenu;	
			processEvent=TRUE;
			break;
		}
		else
		#endif
		{
			menuAction = MIA_RedrawMenu;
			PrevPage = MenuPageIndex; //WMZ 050816
			MenuPageIndex = RootMenu;
			MenuItemIndex = 0;
			processEvent = TRUE;
		}
                Clr_PressExitFlag();
                Clr_EnableShowDDCFlag();
                Clr_PressMenuFlag();
                Clr_MuteFlag();

#if ScanKey
    #if Scan_Source == Synaptics
                KeyLedCounter = 0;
                Set_keyLEDTurnOffFlag();
    #endif                
#endif
                break;
            case MIA_ECO:
                if (FreeRunModeFlag||UnsupportedModeFlag)
                {
                    break;
                }
				/*
                 if (UserPrefColorTemp == CTEMP_SRGB)
			{
                   UserPrefColorTemp = CTEMP_Warm1;
                   UserPrefECOMode = ECO_Standard;
			}
                UserPrefDcrMode = 0;
		  SetECO();
                Clr_GreenFlag();
                Clr_SkinFlag();
                Clr_BlueFlag();
                Clr_ColorAutoDetectFlag();
                Clr_FullEnhanceFlag();
                Clr_DemoFlag(); //eshin 070610 HotEco DemoFlag on but picture off
                Clr_PictureBoostFlag();
                Clr_PressExitFlag();
                SetBFSize(UserPrefBFSize);//SetBFSize(); // jeff add in 1112
                  #if !USEFLASH
       		NVRam_WriteByte(nvrMonitorAddr(ECOMode), UserPrefECOMode);
       		NVRam_WriteByte(nvrMonitorAddr(DcrMode), UserPrefDcrMode);
       		NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
                  #else
       		SaveMonitorSetting();
                  #endif*/
                menuAction = MIA_RedrawMenu;
			#if UnifyHotDcrEco
				if(UserPrefDcrMode)
				{
					MenuPageIndex = HotKeyDCRMenu;
					MenuItemIndex = 0;
				}	
				else
			#endif
                    #if Enable_Lightsensor // 090420
                            if(UserPrefiCareMode)
                            {
					MenuPageIndex = HotKeyiCareMenu;
					MenuItemIndex = 0;
                            }
                            else
                    #endif
				{
                MenuPageIndex = HotKeyECOMenu;
                MenuItemIndex = 0;
				}
                processEvent = TRUE;
                KeypadButton = BTN_Repeat; // 071218 addition to prevent hot key event execute twince
                break;
                #if AudioFunc
                case MIA_VOL:
                    if (FreeRunModeFlag||UnsupportedModeFlag)
                    {
                        break;
                    }

                    menuAction = MIA_RedrawMenu;
                    MenuPageIndex = HotKeyVolMenu;
                    MenuItemIndex = 0;
                    processEvent = TRUE;
                    KeypadButton = BTN_Repeat; // 071218 addition to prevent hot key event execute twince
                    break;
                    /*
                    case MIA_Mute:
                    if ( FreeRunModeFlag )
                    {
                    break;
                    }
                    menuAction = MIA_RedrawMenu;
                    MenuPageIndex = HotKeyMuteMenu;
                    MenuItemIndex = 0;
                    processEvent = TRUE;
                    Clr_PressMenuFlag();
                    Clr_MuteFlag();
                    break;
                     */
                #else
		  case MIA_Cont:
                    if (FreeRunModeFlag|| UserPrefECOMode!=ECO_Standard || UserPrefColorTemp == CTEMP_SRGB||UserPrefDcrMode||UnsupportedModeFlag)
                    {
                        break;
                    }

                    menuAction = MIA_RedrawMenu;
                    MenuPageIndex = HotKeyConMenu;
                    MenuItemIndex = 0;
                    processEvent = TRUE;
                    break;		  	
                #endif
		 #if EnableHotBrightness
		  case MIA_Brite:
                    if (FreeRunModeFlag|| UserPrefECOMode!=ECO_Standard || UserPrefColorTemp == CTEMP_SRGB||UserPrefDcrMode||UnsupportedModeFlag)
                    {
                        break;
                    }

                    menuAction = MIA_RedrawMenu;
                    MenuPageIndex = HotKeyBriMenu;
                    MenuItemIndex = 0;
                    processEvent = TRUE;
                    break;		  	
		 #endif
                #if DDCCI_ENABLE
                case MIA_DDC:
                    if (FreeRunModeFlag )
                    {
                        break;
                    }
                    menuAction = MIA_RedrawMenu;
                    MenuPageIndex = DdcciInfoMenu;
                    DynamicLoadFont(CurrentMenu.Fonts);
                    MenuItemIndex = 0;
                    processEvent = TRUE;
                    Clr_PressExitFlag();
                    Clr_EnableShowDDCFlag();
                    break;
                #endif
            case MIA_Auto:
                if (FreeRunModeFlag || SrcInputType == Input_Digital || SrcInputType == Input_Digital2)//yuanfa
                //0311
                {
                    Clr_PressMenuFlag();
                    Clr_PressExitFlag();
                    break;
                }

		#ifdef DCRClearBacklight//When user press eco hotkey,turn on backlight.
		if(!BackLightActiveFlag)
			{
			hw_SetBlacklit();
			}
		//printData("BacklightNow11==%d", BacklightNow);
		//printData("BacklightPrev22==%d", BacklightPrev);
		if(UserPrefDcrMode&&(BacklightPrev<20)&&(BacklightPrev!=BacklightNow))
			{
                     #if InverseBrightness
                     mStar_WriteByte(BrightnessPWM, 0xFF-RealMinBrightnessValue);
                     #else 
                     mStar_WriteByte(BrightnessPWM, RealMinBrightnessValue);
			#endif	
			}
		#endif

			#if (ModelName!=ENVISION_G2218&&ModelName!=ENVISION_G2218WG1||ModelName==ENVISION_G2218_PLUS)//add by win
                #if Dual
                    if (TurboKeyCounter < 2)
                    {
                        Delay1ms(50);
                    }
                    else
                #endif
			#endif
                {
                    TurboKeyCounter = 0;
                    // printMsg( "draw auto menu" );
/*                    if (FactoryModeFlag && MenuPageIndex == RootMenu)
                    {
                        LoadCommonFont();
                    }
                    
 */       
 			
 		    Osd_Hide();//petit 20130114
		    Delay1ms(10);//petit 20130114
					//130114 Henry for OSD Garbage 
                    menuAction = MIA_RedrawMenu;
                    MenuPageIndex = AutoMenu;
                    Set_DoAutoConfigFlag(); // 2007/9/14  by KK for match osd
                    MenuItemIndex = 0;
                    processEvent = TRUE;
                }
                break;
                //2006-02-22
#if HotInputSelect
			case MIA_ShowHotInput:
				//#if(ModelName == AOC_G2261w)
				if (!PowerOnFlag)
				//#else
				//if ((FreeRunModeFlag && OsdLockModeFlag)||!PowerOnFlag)
				//#endif
				break;
				hw_ClrAmberLed();
				hw_SetGreenLed();
				mStar_PowerUp();
				mStar_SetupFreeRunMode();
				//Clr_PowerSavingFlag();
				if (PowerDownCounter)
				{
			       	PowerDownCounter=0;
				}
				MenuPageIndex=HotInputSelectMenu;
				
				if(UserPrefInputSelectType==InputSelectType_Auto)
					MenuItemIndex=SrcInputType;
				else	
		            MenuItemIndex=UserPrefInputSelectType;		
		        PrevPage=RootMenu;
		        Osd_Hide();
		        menuAction=MIA_RedrawMenu;
				processEvent=TRUE;			
				break;	

#endif
            case MIA_SourceSel:
                #if 1 // 071219 reserved for power saving could select source
                    if ((!PowerOnFlag) )//|| (PowerSavingFlag))
                    //2004-09-20   //2004-10-04 Key Disable wjen power-ff or power saving.....
                    {
                        break;
                    }
                #endif  

    #if ScanKey
        #if Scan_Source == Cypress
            #ifdef LEDBrightCtrlByScalerPWM
            SKLEDStatus = SKLEDBright_Light;
            ScanKey_LEDBrightAction();
            #endif
        #endif
    #endif
                
	     if(CheckSourceKeyStatus())
               break;
		 
		 if(( MenuPageIndex == ResetMenu )&& DoResetFlag)	 //071222 for ExtraMenu Key event
		   {
			 Clr_DoResetFlag();
			 SetDefaultSetting(0);//user 
		   }
		 
	//if(SourceKeyCounter>=50&&!SyncLossState() )   //071224
	//if(SourceKeyCounter>=50&&!SyncLossState()&&!UnsupportedModeFlag)   //071224
	if(SourceKeyCounter>=30&&!SyncLossState()&&!UnsupportedModeFlag)   //071224
	{
		if (FreeRunModeFlag || SrcInputType == Input_Digital || SrcInputType == Input_Digital2)//yuanfa
                //0311
		{
                    Clr_PressMenuFlag();
                    Clr_PressExitFlag();
                    SourceKeyCounter=0;
                    break;
              }

                  //  menuAction = MIA_RedrawMenu;
                    MenuPageIndex = AutoMenu;
                    Set_DoAutoConfigFlag(); // 2007/9/14  by KK for match osd
                     MenuItemIndex = 0;
                  //  processEvent = TRUE;
         
      }
      else	
      {
			
// 071218 modified for source is same with UserPrefInputSelectType except InputSelectType_Auto
    #if (INPUT_TYPE != INPUT_1A)
    		#if 0
                    SrcInputType = (SrcInputType+1)%Input_Nums;
                    if(UserPrefInputSelectType != InputSelectType_Auto) // 071221
                        UserPrefInputSelectType = UserPrefInputType = SrcInputType;

					ChangeSource();

                    MenuPageIndex = RootMenu;
                    Set_SaveSettingFlag(); // 2007/9/14  by KK for match osd
                   
                    
//					Set_SourceChangedFlag();//1225
    #endif 
	
	#if HotInputSelect
 	if(MenuPageIndex==UnsupportedModeMenu)
 	{ 	
	 	PrevPage=UnsupportedModeMenu;
 	}
 	else if(MenuPageIndex==StandbyMenu)
 	{
	 	PrevPage=StandbyMenu;	 	
 	}
 	else 
 	{
	 	PrevPage=RootMenu;
	}
	if(UserPrefInputSelectType==InputSelectType_Auto)
		MenuItemIndex=SrcInputType;
	else	
		MenuItemIndex=UserPrefInputSelectType;			
 	MenuPageIndex=HotInputSelectMenu;	
 	//Osd_Hide();
	if ((PowerDownCounter && !UnsupportedModeFlag))
 		PowerDownCounter=0;



	#endif

#endif	
	}
	             menuAction = MIA_RedrawMenu;
                // MenuItemIndex = 0;
                    processEvent = TRUE;
		SourceKeyCounter=0;

		break; 
/*
		  #if INPUT_TYPE==INPUT_1A2D
                   if (SrcInputType == Input_Analog1)
                    {
                     SrcInputType = Input_Digital;
		      hw_SEL_DVI();
                    }
		   else if ( SrcInputType == Input_Digital )
                 {
                    SrcInputType = Input_Digital2; //charles
                    hw_SEL_HDMI();
                 }
		  #else
                 if (SrcInputType == Input_Analog1)
                 {
                        SrcInputType = Input_Digital;				
                 }
		#endif	
                    else
                    {
                        SrcInputType = Input_Analog1;
                    }
                    UserPrefInputType = SrcInputType; // 2006/9/6 5:22PM by Emily

                    ChangeSource();
                    break;
*/            
			#if EnableVcomAdjustItem
			case MIA_HotVCOM:
				if(ProductModeFlag)
				{
					Set_FactoryModeFlag();
					MenuPageIndex = FactoryMenu;
					MenuItemIndex = 54;
					
					menuAction = MIA_RedrawMenu;
					processEvent = TRUE;
					break;
				}
				else
					break;
			#endif
            case MIA_DCR:
                    if (FreeRunModeFlag ||UnsupportedModeFlag)
                    {
                        break;
                    }
		  if (UserPrefColorTemp == CTEMP_SRGB)
		  UserPrefColorTemp = CTEMP_Warm1;
		#if UnifyHotDcrEco
				UserPrefDcrMode=0;
				AdjustDcrMode(0);
        #endif
                MenuPageIndex = HotKeyDCRMenu;
                MenuItemIndex = 0;
                menuAction = MIA_RedrawMenu;
                processEvent = TRUE;
                break;
        #if Enable_Lightsensor
                case MIA_iCare:
                    if (FreeRunModeFlag ||UnsupportedModeFlag)
                        break;
                    UserPrefiCareMode = LightSensorOff;
                    AdjustiCareMode(0);
                    MenuPageIndex = HotKeyiCareMenu;
                    MenuItemIndex = 0;
                    menuAction = MIA_RedrawMenu;
                    processEvent = TRUE;
                break;                    
        #endif
		#if HotExpansionAdjust
			case MIA_AdjustExpansion:
				    if (FreeRunModeFlag ||UnsupportedModeFlag)
                    {
                        break;
						
                    }
				MenuPageIndex = HotExpansionMenu;
                MenuItemIndex = 0;
                menuAction = MIA_RedrawMenu;
                processEvent = TRUE;
				KeypadButton = BTN_Repeat;
				break;
		#endif
            case MIA_Power:

				if (MenuPageIndex == ExtraMenu && MenuItemIndex == 0 )
				{
					Clr_SaveSettingFlag();
					ReadMonitorSetting();		
				}
                if (PowerOnFlag)
                {
                    MenuPageIndex = PowerOffMenu;
                }
                else
                {
			if(ProductModeFlag||BurninModeFlag||FactoryModeFlag)//DC On Reset For PE Request add by win 101204
			{
				//ResetAllSetting();
				Clr_ProductModeFlag();
				//Clr_FactoryModeFlag();
  		 		//Clr_BurninModeFlag();
				//Clr_DoBurninModeFlag();//edit by lizzie,100429.for DC ON/OFF can't clr burnin mode the 1st time
				#if !USEFLASH
				NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
				NVRam_WriteWord(ProductModeAddr,ProductModeOffValue);
				#else
				FactoryProductModeValue=ProductModeOffValue;
				SaveMonitorSetting();//UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
				SaveFactorySetting();
				#endif
			}
                    MenuPageIndex = PowerOnMenu;
                }

                MenuItemIndex = 0;
                menuAction = MIA_RedrawMenu;
                processEvent = TRUE;
                KeypadButton = BTN_Repeat; // 090517 coding
                break;
#if !AllInOneKey
	   case MIA_AdjustValue:
	   	#if MWEFunction
		     Clr_DrawItemRadioGroupScikeFlag();
		#endif
	   	   EnterKeyCouter =(EnterKeyCouter+1);//%5;
	   	   // EnterKeyCouter=1;
		    Set_JustShowValueFlag();
		    DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
		    Clr_JustShowValueFlag();
			

		  
    		    if((( MenuPageIndex == ResetMenu )&& DoResetFlag)    //071222 for ExtraMenu Key event
    		    	||(MenuPageIndex == ResetMenu)
			||(MenuPageIndex == ExtraMenu)&&(MenuItemIndex==1)&&(EnterKeyCouter > 1)
			||(MenuPageIndex == ExtraMenu)&&(MenuItemIndex==0)&&
			(UserPrefInputSelectType != SrcInputType) 
			#if Dual
			&& (UserPrefInputSelectType != InputSelectType_Auto)
			#endif
			)
    		    {
        			EnterKeyCouter=0;
        			menuAction = MIA_GotoNext;	
				processEvent = TRUE;
					//if(MenuPageIndex == ResetMenu)
					//Osd_Hide();
    		    }
    			//else	
		if (EnterKeyCouter > 1)
		    {
					//*/
			menuAction = MIA_ExitAdjustValue;
			processEvent = TRUE;
		    }
		//	Delay1ms(200);
	  //if(MenuPageIndex == ResetMenu)
		//  {
		    //EnterKeyCouter=0;
        	//menuAction = MIA_GotoNext;	
			//processEvent = TRUE;
		    //Osd_Hide();
		  //}
		    break;
	   case MIA_ExitAdjustValue:
	 	  //if (EnterKeyCouter == 0)
	 	   if (EnterKeyCouter == 0|| MenuPageIndex == ResetMenu)
	 	  {
			if(( MenuPageIndex == ResetMenu )&& DoResetFlag)	//071222 for ExtraMenu Key event
	    		  {
				Clr_DoResetFlag();
				SetDefaultSetting(0);//user	
	    	 	  }
			if((MenuPageIndex == ExtraMenu)&&DoAutoConfigFlag)
	    		  {	
	    		  	  Clr_DoAutoConfigFlag();	
	    		  }
			if((MenuPageIndex == ExtraMenu)&&(UserPrefInputSelectType != SrcInputType) 
	    			#if Dual
				&& (UserPrefInputSelectType != InputSelectType_Auto)
				#endif
				)
	    		  {	
	    		  	  UserPrefInputSelectType=SrcInputType;	
	    		  }			
	    		  if((MenuPageIndex == ExtraMenu)&&(MenuItemIndex==0)&&EnterKeyCouter!=0)
				break;  	
			EnterKeyCouter=0;
			menuAction = MIA_GotoNext;
			processEvent = TRUE;
	 	  }
	   	  else
	   	  {
/*
		  	if ((MenuPageIndex == ExtraMenu)&&(MenuItemIndex==0)&&
			(UserPrefInputSelectType != SrcInputType) 
    			#if Dual
			&& (UserPrefInputSelectType != InputSelectType_Auto)
			#endif
			)
			{
				menuAction = MIA_GotoNext;
				processEvent = TRUE;
			}
			else*/
			{
			if((MenuPageIndex == ExtraMenu)&&(UserPrefInputSelectType != SrcInputType) 
	    			#if Dual
				&& (UserPrefInputSelectType != InputSelectType_Auto)
				#endif
				)
	    		  {	
	    		  	  UserPrefInputSelectType=SrcInputType;	
	    		  }			
				EnterKeyCouter=0;
				Set_JustShowValueFlag();
				DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
				Clr_JustShowValueFlag();
				SaveUserPref();//UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
			}
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
//=========================================================================

#if 0
void ClrInnerOsd(void)
{
    BYTE i;
    Osd_SetTextColor(0, 0); //Center
    for (i = 2; i < OsdWindowHeight - 3; i++)
    {
        ;//Osd_DrawRealStr(2, i, tSpace27);

    }
}
#endif
void DrawOsdMenu(void)
{
    Bool redrawFlags;
	bit NoHideHotMenu; //1226
	NoHideHotMenu=FALSE;
    Clr_JustShowValueFlag();
	
#if WaitingForDrawOSDSUB
   if(FactoryModeFlag&&MenuPageIndex==MainMenu)
	DisableOsdSubWindow(SUB_WIN_00);
#endif

	if(LoadOSDDataFlag)//eshin
	{
		Clr_LoadOSDDataFlag();
		Osd_Hide();
		LoadCommonFont();
	    DynamicLoadFont(CurrentMenu.Fonts);
		    #if DEBUG_PRINTDATA
    printMsg("MenuPageIndex=%d");
    
    #endif	
	}

    #if DEBUG_PRINTDATA
    printData("MenuPageIndex=%d",MenuPageIndex);
    printData("MenuItemIndex=%d",MenuItemIndex);	
    #endif	 

    if (CurrentMenu.Flags &mpbInvisible)
    {
        Osd_Hide();
    }
    else
    {
        BYTE i;
        #if 1//DisplayEnergyStar
        BYTE j;
        #endif
        redrawFlags = FALSE;

       
	#if DEBUG_FLASHADDR&&USEFLASH
	if(MenuPageIndex==MainMenu)
     	{printData("FACTORYSET_START= %x", FACTORYSET_START );
     	  printData("MONITORSET_START= %x", MONITORSET_START);
     	  printData("MODESET_START= %x", MODESET_START);		  
     	  printData("HDCPKey_START= %x", HDCPKey_START);	  
	  #ifdef ReduceDDC
     	  printData("ANALOG_DDC_START= %x", ANALOG_DDC_START);
     	  printData("DIGITAL_DDC_START= %x", DIGITAL_DDC_START);
	  #endif
		  
     	  printData("FLASH_FreeBufferAddr= %x", FLASH_FreeBufferAddr);

		  
	}
	#endif

#if 0
        // setup osd window & position
        if (//(CurrentMenu.Flags & mpbClrInner)
          //||(MenuPageIndex==BrightFrameMenu && PrevPage == BF_PositionMenu)
          //||
          (MenuPageIndex==MainMenu)
          //||(MenuPageIndex==MainMenu && (PrevPage<=InformationMenu&& PrevPage >= ResetMenu/*InputSelectMenu*/))
          )
        {
            // printMsg("Clr Center....");
            //if (MenuPageIndex == BF_PositionMenu && 0)//PrevPage >= BF_PositionMenu)
           //     goto skip_clr;
           // ClrInnerOsd();
        } // end of non redraw
#endif
       // skip_clr:
        if (CurrentMenu.Fonts)
        {

            DynamicLoadFont(CurrentMenu.Fonts);
					    #if DEBUG_PRINTDATA
    printMsg("MenuPageIndexsssssssssssssssssssss=%d");
    
    #endif	
            //DisableOsdWindow(2);
        }
        else if (CurrentMenuItem.Fonts)
        {
            Osd_Hide();
            DynamicLoadFont(CurrentMenuItem.Fonts);
   #if DEBUG_PRINTDATA
    printMsg("MenuPageIndexdddddddddddddddddd=%d");
    
    #endif	
        }
        else if (MenuPageIndex == FactoryMenu
	#if EnablePanelServiceMenu
	||MenuPageIndex == AdjustPanelMenu
	#endif
		)
        {
            Osd_Hide(); // 2006/8/25 6:26PM by Emily for hide the garbage
            Delay1ms(50);
            Osd_LoadFacoryFontCP();
			Set_LoadOSDDataFlag();//eshin
        }
	  if (CurrentMenu.XSize==OsdWindowWidth&&CurrentMenu.YSize==OsdWindowHeight)
	    {	
              if(MenuPageIndex==HotKeyECOMenu ||MenuPageIndex==HotKeyDCRMenu
		#if ModelName!=ENVISION_P2271w||ModelName!=ENVISION_P2471w	||ModelName!=ENVISION_P971wl ||ModelName!=ENVISION_P971l ||ModelName!=ENVISION_P2471||ModelName!=ENVISION_P2371l ||ModelName!=ENVISION_P971wl1||ModelName!=ENVISION_P971wal1||ModelName!=ENVISION_P971wal||ModelName!=ENVISION_P2371l2||ModelName!=ENVISION_P971waL2
		#if INPUT_TYPE!=INPUT_1A
			||MenuPageIndex==HotInputSelectMenu
		#endif
		#endif
              #if Enable_Lightsensor // 090416
                     || MenuPageIndex == HotKeyiCareMenu
              #endif
		#if HotExpansionAdjust
			||MenuPageIndex==HotExpansionMenu		
		#endif
		#if AudioFunc
		       || MenuPageIndex==HotKeyVolMenu
		#else
		       ||MenuPageIndex==HotKeyConMenu
		#endif
		#if EnableHotBrightness
		       ||MenuPageIndex==HotKeyBriMenu
		#endif
		)
    	  	    NoHideHotMenu=TRUE;
	    }
	  if (CurrentMenu.XSize!=OsdWindowWidth || CurrentMenu.YSize!=OsdWindowHeight ||
			 !(mStar_ReadOSDByte(IOSDC1)&MWIN_B) || CurrentMenu.Flags&mpbRedraw)
        {
            redrawFlags = TRUE;
        }
        if (redrawFlags)
        {
		 if(NoHideHotMenu)
		 {
		   Osd_DrawContinuesChar(0,4,SpaceFont,OsdWindowWidth);   //071228
    	   Osd_DrawContinuesChar(0,5,SpaceFont,OsdWindowWidth);
    	   Osd_DrawContinuesChar(0,6,SpaceFont,OsdWindowWidth);
		 }
		 if(!NoHideHotMenu)
		  {
            Osd_Hide();

            //mStar_WriteOSDByte(IOSDC2,  0x00); // color bit setting

            Osd_SetWindowSize(CurrentMenu.XSize, CurrentMenu.YSize);
#if Enable_OsdEngine1
	        Osd_SetOsdEngine(1);
            Osd_SetWindowSize(CurrentMenu.XSize, CurrentMenu.YSize);
	        Osd_SetOsdEngine(0);
#endif
		 }
	     if (CurrentMenu.Flags&mpbCenter)
	     {
            Osd_SetPosition(50, 50);
#if Enable_OsdEngine1
	        Osd_SetOsdEngine(1);
            Osd_SetPosition(50, 50);
	     	Osd_SetOsdEngine(0);
#endif
	     }
         else if (FactoryModeFlag)
         {
#if OSD_PIVOT_EN  //071226
            if(OSDRotateFlag)
                Osd_SetPosition(100,0);				
	        else
#endif	
                Osd_SetPosition(0, 100);
#if Enable_OsdEngine1
	        Osd_SetOsdEngine(1);
           #if OSD_PIVOT_EN  //071226
            if(OSDRotateFlag)
            	Osd_SetPosition(100,0);				
	     	else
           #endif	
                Osd_SetPosition(0, 100);
	     	    Osd_SetOsdEngine(0);
#endif
            } // 2006/9/5 9:52AM by Emily Osd_SetPosition(0, 0);
            else if (!(CurrentMenu.Flags &mpbMoving))
            {
             #if OSD_PIVOT_EN  //071226
                if(OSDRotateFlag)
                  Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);			
	     	    else
		     #endif	            
                Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
#if Enable_OsdEngine1
	            Osd_SetOsdEngine(1);
             #if OSD_PIVOT_EN  //071226
                if(OSDRotateFlag)
                  Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);			
	     	    else
		     #endif	
                Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
	     	    Osd_SetOsdEngine(0);
#endif
            }
		
            if (CurrentMenu.Flags &mpbLogoFrame)
            {
                // printMsg("draw logo");
                Osd_SetPosition(50, 50);
                moveX = 50;
                moveY = 50;
#if (DisplayLogo!=NoBrand)
                DrawLogo();
#endif
                mStar_WriteByte(BLENDC, 0x05); // Entire OSD transpancy
                mStar_WriteByte(BLENDL, 0x20); // new transpancy function
            }
            else if (MenuPageIndex == FactoryMenu
		#if EnablePanelServiceMenu
		||MenuPageIndex == AdjustPanelMenu
		#endif
			)
            {
  		        mStar_WriteOSDByte(0x29, 0x00);
		#if 1//Enable_CascadeMode
  		        mStar_WriteOSDByte(SUBW2C, 0x00);
		#endif
                mStar_WriteByte(BLENDC, 0x00);
                Osd_SetTextMonoColor(CP_BlueColor, CP_BlueColor);
                for (i = 0; i <= OsdWindowHeight-1; i++)
                {
                    Osd_DrawContinuesChar( 0 , i , SpaceFont , OsdWindowWidth);
                }
			#if 0//DEBUG_ENABLE
			#if DISPLAY_CHECK_VOL
			printData("Code_checksun      ====%x", Code_checksun);
			#endif
			#endif
            }
            else if(MenuPageIndex==MainMenu)
            {
                #if 1//Enable_OsdEngine1
                    mStar_WriteOSDByte(IOSDC2,  0x83); // color bit setting
                #else
                    mStar_WriteOSDByte(IOSDC2,  0x03);
                #endif
			if (FactoryModeFlag&&MenuItemIndex==9)
				mStar_WriteOSDByte(SUBW2C, 0x00);
				LoadCommonFont();
				Osd_LoadMainMenuIconFont();
            //!!!! need load font   //????
            //    DynamicLoadFont(CurrentMenu.Fonts);
	            DrawOsdBackGround();
            	}




			
       else if(MenuPageIndex==HotKeyECOMenu ||MenuPageIndex==HotKeyDCRMenu
		#if INPUT_TYPE!=INPUT_1A
			||MenuPageIndex==HotInputSelectMenu
		#endif
              #if Enable_Lightsensor // 090416
                     || MenuPageIndex == HotKeyiCareMenu
              #endif
		#if HotExpansionAdjust
			||MenuPageIndex==HotExpansionMenu		
		#endif
		#if AudioFunc
		       || MenuPageIndex==HotKeyVolMenu
		#else
		       ||MenuPageIndex==HotKeyConMenu
		#endif
		#if EnableHotBrightness
		       ||MenuPageIndex==HotKeyBriMenu
		#endif
		)
           {
    #if Enable_OsdEngine1
    		    Osd_SetOsdEngine(1);
      		    mStar_WriteOSDByte(0x23, 0x00);
    		    Osd_SetOsdEngine(0);
    #else
    		    DisableOsdSubWindow(SUB_WIN_00);
    #endif
		        if(!NoHideHotMenu)	//071226
                    DrawOsdBackGround();
		if(OsdLockModeFlag
			#if HotInputSelect
			&&MenuPageIndex!=HotInputSelectMenu
			#endif
			)
			{
				MenuPageIndex=OsdLockMenu;
				DynamicLoadFont(MenuPage56);
				for (i = 0; i < MenuItemCount; i++)
				{
				    DrawOsdMenuItem(i, &CurrentMenuItem);
				}
			}
		else
			{
				#if (EnableHotBrightness) // JueryWu 100127;
					if(MenuPageIndex==HotKeyBriMenu)
					{
						#if DEBUG_ENABLE
						printMsg("Osd_DrawHOTBrightnessIcn1111111111111111");
						#endif
						LoadHotKeyBriFont();
						Osd_DrawMainIcon(0, 3, 4,0);
						//Osd_DrawHOTBrightnessIcn(0, 3, 4,0);
					} 
					else
				#endif
		                if(MenuPageIndex==HotKeyDCRMenu)
		                {
		                   LoadHotKeyDCRFont();
				#if WaitForDataBlanking
							   mStar_WaitForDataBlanking();
				#endif
				#if UnifyHotDcrEco
					Osd_DrawHotKeyIcon(7, 11, 4);
				#else
					  	   if(UserPrefDcrMode)
		                      Osd_DrawHotKeyIcon(7, 11, 4);
					       else
		                      Osd_DrawHotKeyIcon(8, 11, 4);			   	
				#endif						
				}
                            // 090416
                            #if Enable_Lightsensor
                                else if(MenuPageIndex == HotKeyiCareMenu)
                                {
                                    LoadHotKeyiCareFont();
                                    mStar_WaitForDataBlanking();
                                    Osd_DrawHotKeyIcon(11, 11, 4);
                                }
                            #endif
				#if HotExpansionAdjust
						else if(MenuPageIndex==HotExpansionMenu)
						{
						   LoadHotKeyExpansionFont();
				#if WaitForDataBlanking
						  mStar_WaitForDataBlanking();
				#endif
					if(!ExpansionFlag)
					{
							Osd_DrawHotKeyIcon(10, 11, 4);	
					}
					else
					{       
					       #if ModelName==AOC_G2261w
						  if(UserprefExpansionMode==Expansion_Full)
							  Osd_DrawHotKeyIcon(9, 11, 4);
						   else
							  Osd_DrawHotKeyIcon(10, 11, 4);	
						#else
						  if(UserprefExpansionMode==Expansion_Full)
							  Osd_DrawHotKeyIcon(10, 11, 4);
						   else
							  Osd_DrawHotKeyIcon(9, 11, 4);	
						#endif
					}
					#if 0//ModelName==AOC_G2261w||ModelName==ENVISION_P2271w||ModelName==ENVISION_P2471w||ModelName==ENVISION_P971wl
					AdjustExpansionMode();
					#endif
				}

				#endif						
				#if INPUT_TYPE!=INPUT_1A
		                else if(MenuPageIndex==HotInputSelectMenu)
				{
			                mStar_WriteOSDByte(IOSDC2, 0x00); // color bit setting
						DynamicLoadFont(MenuPage49);
		                        for (i = 0; i < MenuItemCount; i++)
		                        {
		                            DrawOsdMenuItem(i, &CurrentMenuItem);
		                        }
				}
				#endif
		                else
		                {
			                  LoadHotKeyColorFont(UserPrefECOMode);
		                      if(MenuPageIndex==HotKeyECOMenu)
			                      Osd_DrawHotKeyIcon(UserPrefECOMode, 11, 4);
		                      else
			                      Osd_DrawHotKeyIcon(ECO_Nums, 3, 4); // because the hotkey of vol at the last item

		                }
			}
            }

         else
         {
#if Enable_OsdEngine1
    		  Osd_SetOsdEngine(1);
      		  mStar_WriteOSDByte(0x23, 0x00);
    		  Osd_SetOsdEngine(0);
#else
		      DisableOsdSubWindow(SUB_WIN_00);
#endif
              LoadCommonFont1();
              if (MenuPageIndex == AutoColorMenu)
                 LoadCommonFont();
    
    		  mStar_WriteOSDByte(IOSDC2,  0x1); // color bit setting
    		  mStar_WriteOSDByte(OSDCFA, _2ndPageColorFontAddr); // 4 color font RAM start address
     		  mStar_WriteOSDByte(OSD8CFFA, _2ndPageColorFontAddr); // 8 color font RAM start address
              mStar_WriteByte(BLENDC, 0x00);
                  //if(!DisplayLogoFlag)
		      if(!NoHideHotMenu)	//071226
                DrawOsdBackGround();
              if(MenuPageIndex == ExtraMenu||MenuPageIndex==AutoMenu)
              {
		       #if AutoProcess
				if(MenuPageIndex==ExtraMenu)	//120328 Modify
	             	Clr_DoAutoConfigFlag();
				else
					Set_DoAutoConfigFlag();
		         if(MenuPageIndex==AutoMenu)
		        {
			#if DisplayLogo == EnvisionLOGO
                        if(MenuPageIndex==AutoMenu)
            	           {
                               #if 1//Enable_OsdEngine1
                                mStar_WriteOSDByte(IOSDC2,  0x83); // color bit setting
                                #else
                                 mStar_WriteOSDByte(IOSDC2,  0x03);
                                #endif

					LoadCommonFont();
					Osd_LoadMainMenuIconFont();
					DrawOsdBackGround();
            	           }
			#endif
               	    OSD_AutoIconFont();
		           Osd_Set256TextColor(16 ,Color_2);			
                   for(j=0;j<2;j++)
                      for(i=0;i<5;i++)
			             Osd_DrawCharDirect(11+i-7,5+j, HotKey2ColorAddr+i+j*5);	
                   Osd_DrawCharDirect(11-7,4,HotKey2ColorAddr+10);     	
       			   Osd_DrawCharDirect(12-7,4,HotKey2ColorAddr+11);
       			   Osd_DrawCharDirect(15-7,4,HotKey2ColorAddr+12); 	
		           Osd_Set256TextColor(4 ,Color_8);				  
			       Osd_DrawCharDirect(13-7,4,HotKey8ColorAddr);
		           Osd_Set256TextColor(8 ,Color_4);					  
			       Osd_DrawCharDirect(14-7,4,HotKey4ColorAddr);	
                       //zjb 20071218 draw '%'
                   Osd_SetTextColor(0, 14);      
			       Osd_DrawCharDirect(25,5,HotKey2ColorAddr+13);	

	            
		      	}
				 
		     #else
			 if(MenuPageIndex==ExtraMenu)	//120328 Modify
				 Clr_DoAutoConfigFlag();
			 else
				 Set_DoAutoConfigFlag();
		       if(MenuPageIndex==AutoMenu)
		      {
               	   OSD_AutoIconFont();
		           Osd_Set256TextColor(16 ,Color_2);			
                   for(j=0;j<2;j++)
                   	for(i=0;i<5;i++)
			          Osd_DrawCharDirect(11+i,5+j, HotKey2ColorAddr+i+j*5);	
                   Osd_DrawCharDirect(11,4,HotKey2ColorAddr+10);     	
			       Osd_DrawCharDirect(12,4,HotKey2ColorAddr+11);
			       Osd_DrawCharDirect(15,4,HotKey2ColorAddr+12); 	
		           Osd_Set256TextColor(4 ,Color_8);				  
			       Osd_DrawCharDirect(13,4,HotKey8ColorAddr);
		           Osd_Set256TextColor(8 ,Color_4);					  
			       Osd_DrawCharDirect(14,4,HotKey4ColorAddr);			  
		      }
		     #endif
			
          }	
                // 071218 modified fro reset menu default is off
          if(MenuPageIndex == ResetMenu)
	          Clr_DoResetFlag();                
#if DisplayEnergyStar
          if(MenuPageIndex==EnergyStarMenu)
          {
              OSD_LoadEnergyStarFont();
              Osd_Set256TextColor(3 ,Color_2);
              for(j=0;j<3;j++)
                 for (i=0;i<4;i++)
                    Osd_DrawCharDirect(11+i,4+j, HotKey2ColorAddr+i + j*4);

          }
#endif
     	}
     } // end redraw
        #if 0
            else if (CurrentMenu.Flags &mpbClrInner)
            {
                if (MenuPageIndex == BF_PositionMenu && PrevPage != BrightFrameMenu)
                    goto next_stage;
                printMsg("Clr Center....");
                Osd_SetTextColor(0, 0); //Center
                for (i = 2; i < OsdWindowHeight - 2; i++)
                {
                    Osd_DrawRealStr(3, i, tSpace26);
                }
            } // end of non redraw
        #endif
    if (MenuPageIndex == ExtraMenu)
    {
            //Osd_Hide();
        DrawTimingInfo();
            // printMsg("Draw Information...");
    }
#if 0
        else if (MenuPageIndex == LanguageMenu)
        {
            Osd_SetTextColor(0, 6); //Center
            Osd_DrawRealStr(2, 2, tSpace27);
        }
#endif
    for (i = 0; i < MenuItemCount; i++)
    {
        DrawOsdMenuItem(i, &CurrentMenu.MenuItems[i]);
            // printData("ITEM -->%d",i);
    }
   	if(MenuPageIndex == LuminanceMenu && UserPrefColorTemp == CTEMP_SRGB)    //071222 if DCR On item 3 is gray
	{
		//if(UserPrefDcrMode)
			MenuItemIndex = 4;
		//else
		//MenuItemIndex = 3;
   	}
		    #if DEBUG_PRINTDATA
    printMsg("MenuPageIndexaaaaaaaaaaaaaaaa=%d");
    
    #endif	
    #if !Dual  //YF20071218
    if(MenuPageIndex==ExtraMenu)
		#if(Youweipaike)
		MenuItemIndex = 0;
		#else
		MenuItemIndex = 1;
		#endif
	#endif
//	if ( EnterKeyCouter <=2)
//	  Clr_JustShowValueFlag();
//	else
    Set_JustShowValueFlag();
    DrawOsdMenuItem(MenuItemIndex, &CurrentMenuItem);
    Clr_JustShowValueFlag();


    if (redrawFlags)
    {
           // if ((MenuPageIndex == MainMenu && PrevPage == LanguageMenu))
               // return ;

      if(MenuPageIndex == LogoMenu)//Zhifeng.Wu 071219 Set Logo Transparence To 0
           Osd_SetTransparence(0);
      else
		   Osd_SetTransparence(UserPrefOsdTrans);
#if Enable_OsdEngine1
		   Osd_SetOsdEngine(1);
                if(MenuPageIndex == LogoMenu)
                    Osd_SetTransparence(0);
                else
		   Osd_SetTransparence(UserPrefOsdTrans);
		   Osd_SetOsdEngine(0);
#endif
			Osd_Show();
        }

    }
    //printMsg("End of Draw Menu...");
}
//=========================================================================
// draw menu item
void DrawOsdMenuItem(BYTE itemIndex, MenuItemType *menuItem)
{
//	BYTE i;

    //printMsg("...Draw MenuItem...");
    if ((menuItem->Flags &mibFactoryItem) && !FactoryModeFlag)
    {
        return ;
    }
    if ((menuItem->Flags &mibSelectable) && itemIndex == MenuItemIndex)
    {

	 if(menuItem->Flags&mibHighlight)
	 {
	 //if(MenuPageIndex==MainMenu && MenuItemIndex==9 && FactoryModeFlag)
	 // Osd_LoadMainFactoryFont();
	 //else
	 #if WaitForDataBlanking  //zjb20071219
                mStar_WaitForDataBlanking();
        #endif
         LoadMainTitleFont(itemIndex);

	 }

        if (MenuPageIndex == FactoryMenu
	#if EnablePanelServiceMenu
	||MenuPageIndex == AdjustPanelMenu
	#endif
		)
        {
            Osd_SetTextMonoColor(menuItem->SelForeColor, menuItem->SelBackColor);
        }
        else
        {
            Osd_SetTextColor(0, 6);
        }
    }
    else
    {
        if (MenuPageIndex == FactoryMenu
	#if EnablePanelServiceMenu
	||MenuPageIndex == AdjustPanelMenu
	#endif
		)
        {
            Osd_SetTextMonoColor(menuItem->ForeColor, menuItem->BackColor);
        }
        else
        {
            Osd_SetTextColor(menuItem->ForeColor, menuItem->BackColor);
       }

    }

	#if AllInOneKey	
	if( menuItem->Flags & mibUserColor  &&(   UserPrefColorTemp != CTEMP_USER||UserPrefDcrMode==1))//||FullEnhanceFlag||
	    //SkinFlag||BlueFlag||GreenFlag||DemoFlag||ColorAutoDetectFlag||PictureBoostFlag) /*&& MenuPageIndex==ColorTempSelectMenu*/)
		Osd_SetTextColor(0, 20); 
	//jeff 1112
	else
	{
 // set the gary item
		if (  (menuItem->Flags & mibDVIDisable && SrcInputType > Input_Analog1)
		||(menuItem->Flags & mibsRGBDisable && (UserPrefColorTemp == CTEMP_SRGB&&UserPrefECOMode==ECO_Standard))
		||( menuItem->Flags & mibStdEnable && ( UserPrefECOMode != ECO_Standard ) )
		||( menuItem->Flags & mibDCRDisable && ( UserPrefDcrMode ) )
		||( menuItem->Flags & mibUserColor  && ( UserPrefColorTemp != CTEMP_USER||UserPrefDcrMode==1 ) )// jeff 1112
		||( menuItem->Flags & mibBFEnable && ( !PictureBoostFlag ) )
		||( menuItem->Flags & mibBFEnable && MenuPageIndex==BrightFrameMenu && UserPrefBFSize == 0)
		||( menuItem->Flags & mibDemoDisable && MenuPageIndex==BrightFrameMenu && DemoFlag)
		#if Enable_Expansion
		||((!ExpansionFlag)&& (MenuPageIndex == ExtraMenu)&&( itemIndex==2))//dq1108
		#endif //YF20071218
		#if(!Dual)
		||(MenuPageIndex == ExtraMenu)&&(itemIndex==0)
		#endif
              #if Enable_Lightsensor
		||( menuItem->Flags & mibiCareDisable && ( UserPrefiCareMode) )
              #endif
		||((MenuPageIndex == ExtraMenu)&&(itemIndex==1)&&(SrcInputType >= Input_Digital))	//071225
      )
    {
        Osd_SetTextColor( 0, 20 );
    }
    }

   if (MenuPageIndex == MainMenu)
   {
	    DrawOsdMenuItemText(itemIndex, menuItem);
	    if (MenuPageIndex == MainMenu && itemIndex == MenuItemIndex)
	    {
		    DrawOsdMenuItemValue(itemIndex, &(menuItem->DisplayValue));
	    } //->DisplayValue);
    }
   else
	{
#if AllInOneKey&& HotInputSelect
			if(MenuPageIndex==HotInputSelectMenu&&itemIndex==MenuItemIndex)
			{
				Osd_SetTextColor(menuItem->SelForeColor,menuItem->SelBackColor);//0,14);//
			}
#endif
		DrawOsdMenuItemText(itemIndex, menuItem);
		
		if (MenuPageIndex == MainMenu && itemIndex == MenuItemIndex)
		{
			DrawOsdMenuItemValue(itemIndex, &(menuItem->DisplayValue));
		} 
		else if (MenuPageIndex != MainMenu)
		{
			if(JustShowValueFlag &&  itemIndex == MenuItemIndex)
			{
				Osd_SetTextColor(0, 14);
			}
			DrawOsdMenuItemValue(itemIndex, &(menuItem->DisplayValue));
		} 
	}
	#else
	{
		bit ItemColorGray=1;

	if ((menuItem->Flags & mibDVIDisable && SrcInputType == Input_Digital)
	||(menuItem->Flags & mibsRGBDisable && (UserPrefColorTemp == CTEMP_SRGB&&UserPrefECOMode==ECO_Standard))
	||( menuItem->Flags & mibStdEnable && ( UserPrefECOMode != ECO_Standard ) )
	||( menuItem->Flags & mibDCRDisable && ( UserPrefDcrMode ) )
	||( menuItem->Flags & mibUserColor  && ( UserPrefColorTemp != CTEMP_USER||UserPrefDcrMode==1 ) )// jeff 1112
	||( menuItem->Flags & mibBFEnable && ( !PictureBoostFlag ) )
	||( menuItem->Flags & mibBFEnable && MenuPageIndex==BrightFrameMenu && UserPrefBFSize == 0)
	||( menuItem->Flags & mibDemoDisable && MenuPageIndex==BrightFrameMenu && DemoFlag)
	#if Enable_Expansion
	||((!ExpansionFlag)&& (MenuPageIndex == ExtraMenu)&&( itemIndex==2))//dq1108
	#endif
	#if !Dual  //YF20071218
	#if(!Youweipaike)
	||(MenuPageIndex == ExtraMenu)&&(itemIndex==0)
	#endif
	#endif	
	||((MenuPageIndex == ExtraMenu)&&(itemIndex==1)&&(SrcInputType >= Input_Digital))//071225
	#if AudioFunc
	||(MenuPageIndex == HotKeyVolMenu)
	#endif
      #if Enable_Lightsensor
	||( menuItem->Flags & mibiCareDisable && ( UserPrefiCareMode) )
      #endif
	   )
	{
		Osd_SetTextColor( 0, 20 );
		ItemColorGray=0;
	}

	if (MenuPageIndex == MainMenu)
	{
		DrawOsdMenuItemText(itemIndex, menuItem);
		
		if (MenuPageIndex == MainMenu && itemIndex == MenuItemIndex)
		{
			DrawOsdMenuItemValue(itemIndex, &(menuItem->DisplayValue));
		} 
	}
	else
	{
		if (MenuPageIndex == StandbyMenu||MenuPageIndex==CableNotConnectedMenu
			||MenuPageIndex==UnsupportedModeMenu)//071225
			Clr_JustShowValueFlag();
		if (MenuPageIndex == FactoryMenu
                    #if EnablePanelServiceMenu
			||MenuPageIndex == AdjustPanelMenu
			#endif 
			)
		{
			mStar_WriteOSDByte(0x29, 0x00);
			mStar_WriteOSDByte(0x2f, 0x00);
			mStar_WriteByte(FWC, 0);
		
			DrawOsdMenuItemText(itemIndex, menuItem);
			DrawOsdMenuItemValue(itemIndex, &(menuItem->DisplayValue));
		}
		else
		{
			if(JustShowValueFlag &&  itemIndex == MenuItemIndex)
			{
				Osd_SetTextColor(0, 14);
			}
			else if(ItemColorGray)
			{
				Osd_SetTextColor(0, 6);
			}

			DrawOsdMenuItemText(itemIndex, menuItem);

			//if(JustShowValueFlag &&  itemIndex == MenuItemIndex && EnterKeyCouter )//|| MenuPageIndex == ResetMenu)
			if(JustShowValueFlag &&  itemIndex == MenuItemIndex && (EnterKeyCouter || MenuPageIndex == ResetMenu))
			{
				Osd_SetTextColor(0, 14);
			}
			else if(ItemColorGray)
			{
				Osd_SetTextColor(0, 6);
			}

#if DEBUG_ENABLE
//	 printData("MenuPageIndex==%d", MenuPageIndex);
//	 printData("JustShowValueFlag==%d", JustShowValueFlag);
//	  printData("EnterKeyCouter==%d", EnterKeyCouter);
#endif
			DrawOsdMenuItemValue(itemIndex, &(menuItem->DisplayValue));
		}
  } //->DisplayValue);
   
}
     #endif

}




extern BYTE* code  Language_Table[];

void DrawMainMenuText(BYTE xPos, BYTE yPos)
{
    BYTE i=0,j=1;
    BYTE str = MAIN_MENU_FONT_START;
    BYTE* PPPPP;





    PPPPP = (BYTE*) Language_Table[UserPrefLanguage];

    PPPPP = *(&(BYTE*)PPPPP+MenuItemIndex);




    while (1)
    {
      if ((*PPPPP) == 0xFF)
      break;
if(j)
{

 xPos = ((OsdWindowWidth-(*PPPPP))/2);
      PPPPP++;
      j=0;
}

  i = ((*PPPPP) & 0x7F);

    while (1)
  {
            if ( i <= 0 )
                break;

      if ((*PPPPP) & 0x80)
      {
       Osd_DrawCharDirect(xPos, yPos, str++);
       Osd_DrawCharDirect(xPos++, yPos+1, str++);
      }
      else
      {
       Osd_DrawCharDirect(xPos++, yPos, str++);
      }

    i--;
  }


    PPPPP++;
    }

}



//=========================================================================
// draw menu item display text
void DrawOsdMenuItemText(BYTE itemIndex, MenuItemType *menuItem)
{
BYTE i;

#if 0//DEBUG_ENABLE
printMsg("------->DrawOsdMenuItemText");
#endif

    if (menuItem->DisplayText == NULL)
    {
        return ;
    }
    if (menuItem->DrawItemMethod == DWI_Icon)
    {



if(MenuPageIndex==MainMenu)
{




	if(MenuItemIndex == itemIndex )
	{
#if Enable_OsdEngine1
		Osd_SetOsdEngine(1);

  		mStar_WriteOSDByte(0x23, 0x01);


    		mStar_WriteOSDByte(0x15, 0x08);
		#if OSD_PIVOT_EN  //071226
		if(OSDRotateFlag)
		{
		mStar_WriteOSDByte(0x1d, 0);
    		mStar_WriteOSDByte(0x1e, 0);
    		mStar_WriteOSDByte(0x1f, 0);
		mStar_WriteOSDByte(0x17, 0x84);
    		mStar_WriteOSDByte(0x18, 0x84);
    		mStar_WriteOSDByte(0x19, 0x84);			
		}
		else
		{
		mStar_WriteOSDByte(0x1d, 0x84);
    		mStar_WriteOSDByte(0x1e, 0x84);
    		mStar_WriteOSDByte(0x1f, 0x84);
		mStar_WriteOSDByte(0x17, 0);
    		mStar_WriteOSDByte(0x18, 0);
    		mStar_WriteOSDByte(0x19, 0);			
		}
              #else
		mStar_WriteOSDByte(0x1d, 0x84);
    		mStar_WriteOSDByte(0x1e, 0x84);
    		mStar_WriteOSDByte(0x1f, 0x84);
		#endif

    		mStar_WriteOSDByte(0x24, 0x3);
    		mStar_WriteOSDByte(0x25, 0x1B);
    		mStar_WriteOSDByte(0x28, 0x10);


		if(MenuItemIndex>=3 && MenuItemIndex<=5)
		{
    			mStar_WriteOSDByte(0x26, 0x1);//0x2);
    			mStar_WriteOSDByte(0x27, 0x10);//0x11);
		}
		else if(MenuItemIndex>=6 && MenuItemIndex<=8)
		{
    			mStar_WriteOSDByte(0x26, 0x06);//0x7);//0x3+10);
    			mStar_WriteOSDByte(0x27, 0x10);//0x11);//0x06+10);
		}
		else
		{
    			mStar_WriteOSDByte(0x26, 0x3);//0x4);
    			mStar_WriteOSDByte(0x27, 0x10);//0x11);
		}


		Osd_SetOsdEngine(0);

              //SetOsdSubWindow(SUB_WIN_00, (menuItem->XPos)-1, (menuItem->XPos)+5 , (menuItem->YPos)-1, (menuItem->YPos)+2, 0);



		Osd_DrawMainIcon(itemIndex, menuItem->XPos, menuItem->YPos, 1);


		Osd_SetTextColor(0, 1);
              Osd_DrawContinuesChar((menuItem->XPos)-1, (menuItem->YPos)-1, SpaceFont, 7);//highlight
              for(i=0 ; i<4 ; i++)
		{
			Osd_DrawCharDirect((menuItem->XPos)-1, (menuItem->YPos)-1+i, SpaceFont);//highlight
			Osd_DrawCharDirect((menuItem->XPos)+5, (menuItem->YPos)-1+i, SpaceFont);//highlight
		}

#else
            //zjb20071219
            #if 0  
	      SetOsdSubWindow(SUB_WIN_00, (menuItem->XPos)-1, (menuItem->XPos)+5 , (menuItem->YPos)-1, (menuItem->YPos)+2, 0);
		Osd_DrawMainIcon(itemIndex, menuItem->XPos, menuItem->YPos, 0);
		Osd_Set256TextColor(11, Color_2);//T);
              for(i=0; i<6;i++)
      		Osd_DrawCharDirect((menuItem->XPos)-1+i, (menuItem->YPos)-1, E1_FRAME_01+i);
              Osd_DrawCharDirect((menuItem->XPos)+5, (menuItem->YPos)-1, E1_FRAME_01);
              for(i=0;i<3;i++)
              {
	        Osd_DrawCharDirect((menuItem->XPos)+5, (menuItem->YPos)+i, E1_FRAME_08+i);
	        Osd_DrawCharDirect((menuItem->XPos)-1, (menuItem->YPos)+i, E1_FRAME_13-i);
              }
		Osd_Set256TextColor(3, Color_2);
	      #else

	       SetOsdSubWindow(SUB_WIN_00, (menuItem->XPos)-1, (menuItem->XPos)+5 , (menuItem->YPos)-1, (menuItem->YPos)+2, 0);
		#if WaitingForDrawOSDSUB
			if(FactoryModeFlag&&MenuPageIndex == MainMenu)
		#endif
		 #if WaitForDataBlanking
                  mStar_WaitForDataBlanking();
               #endif
		Osd_Set256TextColor(11, Color_2);//T);
              for(i=0; i<6;i++)
      		Osd_DrawCharDirect((menuItem->XPos)-1+i, (menuItem->YPos)-1, E1_FRAME_01+i);
              Osd_DrawCharDirect((menuItem->XPos)+5, (menuItem->YPos)-1, E1_FRAME_01);
              for(i=0;i<3;i++)
              {
	        Osd_DrawCharDirect((menuItem->XPos)+5, (menuItem->YPos)+i, E1_FRAME_08+i);
	        Osd_DrawCharDirect((menuItem->XPos)-1, (menuItem->YPos)+i, E1_FRAME_13-i);
              }
              Osd_SetTextColor(0, 1);
		Osd_DrawMainIcon(itemIndex, menuItem->XPos, menuItem->YPos, 0);
		Osd_Set256TextColor(3, Color_2);

	     #endif
#endif


               DrawMainMenuText(0,1 );




     }
    	else
    	{
		#if (WaitingForDrawOSDSUB && (!Enable_OsdEngine1))
			if(FactoryModeFlag&&MenuPageIndex == MainMenu)
				;
			else
			if(WaitingForOSDFlsh) // 081125
			{
				mStar_WaitForDataBlanking();// 081121
				DisableOsdSubWindow(SUB_WIN_00);
			}
		#endif

		Osd_SetTextColor(0, 0); //Center

                Osd_DrawContinuesChar((menuItem->XPos)-1, (menuItem->YPos)-1, SpaceFont, 7);//highlight

		for(i=0 ; i<4 ; i++)
		{
			Osd_DrawCharDirect((menuItem->XPos)-1, (menuItem->YPos)-1+i, SpaceFont);//highlight
			Osd_DrawCharDirect((menuItem->XPos)+5, (menuItem->YPos)-1+i, SpaceFont);//highlight
		}

       	 	Osd_DrawMainIcon(itemIndex, menuItem->XPos, menuItem->YPos, 0);
 		Osd_SetTextColor(0, 10); // clear main title text

              Osd_DrawContinuesChar(0,1,SpaceFont,OsdWindowWidth);
              Osd_DrawContinuesChar(0,2,SpaceFont,OsdWindowWidth);


    	}


}
else
{

              //printData("UserPrefLastMenuIndex   = %d", UserPrefLastMenuIndex);
              LoadMainTitleFont(UserPrefLastMenuIndex);

              Osd_SetTextColor(0, 10);
              MenuItemIndex=UserPrefLastMenuIndex;
	        Osd_Set256TextColor(3, Color_2);		  
               DrawMainMenuText(0,1);
               if(MenuPageIndex==LuminanceMenu && UserPrefECOMode != ECO_Standard)
                MenuItemIndex=2;
               else if(MenuPageIndex==LuminanceMenu && UserPrefDcrMode)
                MenuItemIndex=4;
               else if(MenuPageIndex==BrightFrameMenu && !PictureBoostFlag)
                MenuItemIndex=5;
               else
		  MenuItemIndex=0;
}

    }
    else if (menuItem->DrawItemMethod == DWI_Text)
    {
        if (MenuPageIndex == FactoryMenu
	#if EnablePanelServiceMenu
	||MenuPageIndex == AdjustPanelMenu
	#endif
		)
        {
            Osd_DrawRealStr(menuItem->XPos, menuItem->YPos, menuItem->DisplayText());
        }
	else if(MenuPageIndex==MainMenu && FactoryModeFlag && MenuItemIndex==9)
	{

		Osd_SetTextMonoColor(3, 0);
		Osd_LoadMainFactoryFont();
            	Osd_DrawCharDirect(14, 1, MAIN_MENU_FONT_START);
#if CenterTitleText //071228
				Osd_DrawCharDirect(14, 2, MAIN_MENU_FONT_START+1);
#endif

	}
        else
        {

		Osd_DrawPropStr(menuItem->XPos, menuItem->YPos, menuItem->DisplayText());

	  }
    }

    else
    // full text
    {
        //BYTE i;
        BYTE xPos;
        #define len itemIndex
        if (menuItem->DrawItemMethod == DWI_FullText)
        {
            xPos = menuItem->XPos;
            Osd_DrawRealStr(xPos, menuItem->YPos, menuItem->DisplayText());
        }
        else if (menuItem->DrawItemMethod == DWI_FullCenterText || menuItem->DrawItemMethod == DWI_CenterText)
        {
            if (MenuPageIndex == FactoryMenu
		#if EnablePanelServiceMenu
		||MenuPageIndex == AdjustPanelMenu
		#endif
		)
            {
                Osd_DrawRealStr(xPos, menuItem->YPos, menuItem->DisplayText());
            }
            else
            {
                Osd_DrawPropStr(xPos, (menuItem->YPos) | BIT6, menuItem->DisplayText());
            }
        }
        #if 0
            if (menuItem->DrawItemMethod != DWI_CenterText)
            // full text
            {
                for (i = 0; i < xPos; i++)
                    Osd_DrawChar(i, menuItem->YPos, 0x01);
                // fill space
                for (i = xPos + len; i < OsdWindowWidth; i++)
                    Osd_DrawChar(i, menuItem->YPos, 0x01);
            }
        #endif
        #undef len
    }
}
//=========================================================================
// draw menu item display value
void DrawOsdMenuItemValue(BYTE itemIndex, DrawValueType *valueItem)
{
    //printData("value address %x", (WORD)valueItem);
    if (valueItem->DrawNumber)
    {
        DrawOsdMenuItemNumber(itemIndex, valueItem->DrawNumber);
    }
    if (valueItem->DrawGuage)
    {
        DrawOsdMenuItemGuage(itemIndex, valueItem->DrawGuage);
    }
    if (valueItem->DrawRadioGroup)
    {
        DrawOsdMenuItemRadioGroup(itemIndex, valueItem->DrawRadioGroup);
    }
    #if 0  //2006-08-04 Andy
  if ((valueItem->DrawRadioGroup && itemIndex == MenuItemIndex)
      ||(valueItem->DrawRadioGroup && MenuPageIndex>MainMenu))//||(valueItem->DrawRadioGroup&&MenuPageIndex==FactoryMenu))
            DrawOsdMenuItemRadioGroup(itemIndex, valueItem->DrawRadioGroup);
    #endif
    // printData("value address %x", (WORD)valueItem);
}
//=========================================================================
#if 1
    // draw number
    void DrawOsdMenuItemNumber(BYTE itemIndex, DrawNumberType *numberItem)
    {
        itemIndex = 0;
        #if Multi_Number
            {
                NumberType *number;
                BYTE i = 0;
                number = numberItem->DisplayNumber;
                if (!JustShowValueFlag)
                {
                    //        if (MenuPageIndex==LuminanceMenu&&MenuItemIndex==itemIndex)
                    //          Osd_SetTextMonoColor(numberItem->ForeColor, numberItem->BackColor);
                    //        else
                    {
                        if (number->Flags &dwiNumber)
                        {
                            Osd_SetTextColor(0, numberItem->BackColor);
                        }
                    }
                }
                if (number)
                {
                    #define drawItem    (number+i)
                    #define xPos itemIndex
                    while (1)
                    {
                        xPos = drawItem->XPos;
                        if (xPos == 0)
                        {
                            xPos = (OsdWindowWidth - 4) / 2-1;
                        }
						if (drawItem->Flags &dwiOptionBar)      //090331 xiandi.yu draw offtimer arrow
                     {
                       Osd_DrawContinuesChar(xPos, drawItem->YPos, SpaceFont,( OsdWindowWidth - 3 - xPos));
                       Osd_DrawCharDirect(drawItem->XPos-2, drawItem->YPos, Arrow_Left);
                       Osd_DrawCharDirect(OsdWindowWidth-4, drawItem->YPos, Arrow_Right);
                     }

			#if DISPLAY_CHECK_VOL	
                            //Osd_DrawHex(xPos, drawItem->YPos, drawItem->GetValue());
				Osd_DrawCheckSUM( 15,24 ,Code_checksun);				//Added by dragonheb for checksum
			#endif
			
                        if (drawItem->Flags &dwiHex)
                        {
                            Osd_DrawHex(xPos, drawItem->YPos, drawItem->GetValue());
				#if DISPLAY_CHECK_VOL	
				#if DEBUG_ENABLE
					printMsg("1111111111111");
					printData("Code_checksun===  %d", Code_checksun);
					printMsg("2222222222222");
				#endif
				Osd_DrawCheckSUM( 15,24 ,Code_checksun);				//Added by dragonheb for checksum
				#endif
                        }
						#ifdef OffPower     
						else if(drawItem->Flags &dwiZERONumber)
						{
							Osd_DrawNum(xPos, drawItem->YPos, drawItem->GetValue());
							if(drawItem->GetValue()<10)
								Osd_DrawNum(xPos-1, drawItem->YPos,0);
						}
						#endif
                        else
                        {
                        #if EnablePanelServiceMenu
                        	if (MenuPageIndex==AdjustPanelMenu)
	                            Osd_Draw4Num(xPos, drawItem->YPos, drawItem->GetValue());
				else
			   #endif
			   			#ifdef TSUMXXT		//110316 Modify
							if ((MenuPageIndex==FactoryMenu)&&
								(drawItem->GetValue==GetRedGainValue
								||drawItem->GetValue==GetGreenGainValue
								||drawItem->GetValue==GetBlueGainValue))
									DrawNum(xPos, drawItem->YPos, 4, drawItem->GetValue());
							else
						#endif
								//if((MenuPageIndex==FactoryMenu)&&(drawItem->GetValue==GetPanelOnTimeValue))		//110316 Modify for TPV Request
									//DrawNum(xPos, drawItem->YPos, 7, drawItem->GetValue());
								//else
		                            Osd_DrawNum(xPos, drawItem->YPos, drawItem->GetValue());
                        }
                        if (drawItem->Flags &dwiEnd)
                        {
                            break;
                        }
                        i++;
                    }
                    #undef xPos
                    #undef drawItem
                }
            }
        #else
            #define xPos itemIndex
            xPos = numberItem->XPos;
            if (xPos == 0)
            {
                xPos = (OsdWindowWidth - 4) / 2-1;
            }
            if (FactoryModeFlag && MenuPageIndex == FactoryMenu)
            {

                if (CurrentMenuItem.DisplayValue.DrawNumber == numberItem)
                {
                    Osd_SetTextColor(numberItem->ForeColor, numberItem->BackColor);
                }
                else
                {
                    Osd_SetTextColor(CP_WhiteColor, CP_BlueColor);
                }
            }
            else
            {
                Osd_SetTextColor(numberItem->ForeColor, numberItem->BackColor);
            }
            if (numberItem->Flags &dwiHex)
            {
                Osd_DrawHex(numberItem->XPos, numberItem->YPos, numberItem->GetValue());
            }
            else
            {
                Osd_DrawNum(numberItem->XPos, numberItem->YPos, numberItem->GetValue());
            }
            #undef xPos
        #endif
    }
#endif
//=========================================================================
// draw guage
void DrawOsdMenuItemGuage(BYTE itemIndex, DrawGuageType *gaugeItem)
{
    //BYTE yPos;
    itemIndex = 0;
    #if Multi_Gauge
        {
            BYTE i;
            GaugeType *gauge;
            gauge = gaugeItem->DisplayGauge;
            if (!JustShowValueFlag)
            {
                if (MenuPageIndex != FactoryMenu
			#if EnablePanelServiceMenu
			&&MenuPageIndex != AdjustPanelMenu
			#endif
			)
                {
                    if (gauge->Flags &dwiGauge)
                    {
                        Osd_SetTextColor(0, gaugeItem->BackColor);
                    }
                }
            }
            if (gauge)
            {
                i = 0;
                #define drawItem    (gauge+i)
                #define xPos    itemIndex
                while (1)
                {
                    xPos = drawItem->XPos;
                    if (xPos == 0)
                    {
                        xPos = (OsdWindowWidth - gaugeItem->Length) / 2;
                    }
                    //Osd_SetTextColor(0, gaugeItem->BackColor);
                    Osd_DrawGuage(xPos, drawItem->YPos, gaugeItem->Length, drawItem->GetValue());
                    if (drawItem->Flags &dwiEnd)
                    {
                        break;
                    }
                    i++;
                }
                #undef xPos
                #undef drawItem
            }
        }
    #else
        #define xPos    itemIndex
        xPos = gaugeItem->XPos;
        //yPos=gaugeItem->YPos;
        if (xPos == 0)
        {
            xPos = (OsdWindowWidth - gaugeItem->Length) / 2;
        }
        if (!(CurrentMenu.Flags &mpbAdjust))
        {
            Osd_SetTextColor(gaugeItem->ForeColor, gaugeItem->BackColor);
        }
        else
        {
            Osd_SetTextColor(CP_RedColor, CP_WhiteColor);
        }
        Osd_DrawGuage(gaugeItem->XPos, gaugeItem->YPos, gaugeItem->Length, gaugeItem->GetValue());
        #undef xPos
    #endif
}

//=========================================================================
// draw radio
void DrawOsdMenuItemRadioGroup(BYTE itemIndex, DrawRadioGroupType *radioItem)
{
    RadioTextType *radioText;
    radioText = radioItem->RadioText;
	 #if MWEFunction
    if(DrawItemRadioGroupScikeFlag)
	  return ;
	#endif
    if (radioText)
    {
        BYTE i = 0;
        BYTE xPos;
        BYTE center_flag;
        center_flag = 0;
        itemIndex = 0xFF;
        if (radioItem->GetValue)
        {
            itemIndex = radioItem->GetValue();
        }
        #define drawItem    (radioText+i)
        while (1)
        {
            if (drawItem->DisplayText)
            {
                xPos = drawItem->XPos;

                if (xPos == 0)
                {

                    center_flag = 1;
                    if (MenuPageIndex == FactoryMenu
			#if EnablePanelServiceMenu
			||MenuPageIndex==AdjustPanelMenu
			#endif
				)
                        xPos = (OsdWindowWidth - strlen(drawItem->DisplayText()) + 1) / 2;
                }
                else if ( drawItem->Flags & dwiOptionBar
			/*&& ( UserPrefECOMode == ECO_Internet || UserPrefECOMode == ECO_Standard )*/ )
                {
                      // xPos = xPos - 2;
                     //  printData("  bbb  = %d ", xPos);
                }
                if (!JustShowValueFlag)
                // 2006/9/26 5:23PM by Emily
                {
                    if (drawItem->Flags &dwiRadioGroup)
                    {
                        Osd_SetTextColor(0, radioItem->BackColor);
                    }
                }
                if (drawItem->Flags &dwiOptionBar)
                {
                    // clear selected bar
                    //int j;
                    //for (j = xPos; j < OsdWindowWidth - 3; j++)
                        //Osd_DrawCharDirect(j, drawItem->YPos, 0x01);
                    Osd_DrawContinuesChar(xPos, drawItem->YPos, SpaceFont,( OsdWindowWidth - 3 - xPos));
                    // draw arrow

                    //      printData("  ffffffff  = %d ", xPos);
                    Osd_DrawCharDirect(drawItem->XPos-1, drawItem->YPos, Arrow_Left);
                    //Osd_DrawCharDirect(xPos - 1, drawItem->YPos, 0x001);
                    //Osd_DrawCharDirect(28, drawItem->YPos, 0x001);
                    Osd_DrawCharDirect(OsdWindowWidth-4, drawItem->YPos, Arrow_Right);
                }
                //else if (drawItem->Flags &dwiOptionBar1)
                    //Osd_DrawCharDirect(xPos - 2, drawItem->YPos, Arrow_Right);

                if (center_flag == 1)
                // for set center and clear the color of word
                {
                    if(drawItem->YPos == 1) // for title
                        Osd_SetTextColor(0, 0x0A);
                    else
	                    Osd_SetTextColor(0, 6); //Center
                    Osd_DrawContinuesChar( 2 , drawItem->YPos, SpaceFont , OsdWindowWidth);
                    center_flag = 0;
                }
	#if Enable_Expansion
	  if((!ExpansionFlag)&& (MenuPageIndex == ExtraMenu)&&( itemIndex==2))//dq1108
            {
                Osd_SetTextColor( 0, 20 );
            }
	 #endif
                if (MenuPageIndex == FactoryMenu
			#if EnablePanelServiceMenu
			||MenuPageIndex==AdjustPanelMenu
			#endif
			)
                {
                    Osd_DrawRealStr(xPos, drawItem->YPos, drawItem->DisplayText());
                }
                else
                {
                    if (drawItem->Flags &dwiOptionBar)
                    {

                        Osd_DrawPropStr(xPos , (drawItem->YPos) | BIT6, drawItem->DisplayText());
                    }
			else if(drawItem->Flags & dwiDirectStr)
		  	{
             // printData("UserPrefLastMenuIndex   = %d", UserPrefLastMenuIndex);

				 LoadMainTitleFont(UserPrefLastMenuIndex);
				//LoadCommonFont1();
            			Osd_SetTextColor(0, 10);//0x0A); // Dark Gray
                            Osd_DrawContinuesChar(0,1,SpaceFont,OsdWindowWidth);
                            Osd_DrawContinuesChar(0,2,SpaceFont,OsdWindowWidth);
				MenuItemIndex=UserPrefLastMenuIndex;
				Osd_Set256TextColor(3, Color_2);
				DrawMainMenuText(0,1);
				MenuItemIndex=4;//language

		  	}
                    	else
                    {
                    		Osd_DrawPropStr(xPos, drawItem->YPos, drawItem->DisplayText());

                    }
                }
            }
            if (drawItem->Flags &dwiEnd)
            {
                break;
            }
            i++;
        }
        #undef drawItem
    }
}
#if 0
    Bool DrawTimingInfo(void)
    {
        Osd_SetTextColor(CP_BlackColor, CP_WhiteColor);
        Osd_Draw4Num(8, 4, mStar_ReadWord(SPRHDC_H) &0xfffe);
        return TRUE;
    }
#endif

Bool DrawTimingInfo(void)
{
    WORD freq;
    WORD HDisplay;

    	Osd_SetTextColor(0, 6);
        // 071218 reserved for extra page fw version
	//Osd_DrawPropStr(3, 17, VersionText());
	//Osd_DrawPropStr(15, 17, FW_VerText());
	//Osd_DrawPropStr(3, 14, ResolutionText());
    #if DEBUG_ENABLE
    if(FactoryModeFlag)
    {
        Osd_Draw4Num(12, 11, SrcModeIndex);
        Osd_Draw4Num(12, 12, SrcVTotal);
        Osd_Draw4Num(12, 13, (GetSyncPolarity(SrcFlags)));
    }
    #endif

   #ifdef PanelTpye_1366//ModelName==ENVISION_P971wl
	if (StandardModeGroup == Res_1360x768 && abs(StandardModeVFreq - 600) < 10)
		 Osd_Draw4Num(12, 14, 1360);	
		else
    #endif

	Osd_Draw4Num(12, 14, mStar_ReadWord(SPRHDC_H) &0xfffe);
	Osd_DrawPropStr(3, 14, ResolutionText());//Move down For Polish Resolution is to long
	Osd_DrawPropStr(18, 14, HResText());
	if (TermBuffer == Res_640x350)
	// || StandardModeGroup == Res_720x400)
	{
	    freq = mStar_ReadWord(AOVEND_H) - mStar_ReadWord(AOVST_H) + 1;
	    freq = (freq / 10) *10;
	    Osd_Draw4Num(20, 14, freq &0xfffe);
	}
	else if (TermBuffer == Res_720x480)
	// force to show 480
	{
	    freq = 480;
	    Osd_Draw4Num(20, 14, freq &0xfffe);
	}
	else if (TermBuffer == Res_720x576)
	// force to show 576
	{
	    freq = 576;
	    Osd_Draw4Num(20, 14, freq &0xfffe);
	}
	else if (TermBuffer == Res_1920x1080)
	// || StandardModeGroup == Res_1920x1080)
	// force to show 1080
	{
	    freq = 1080;
	    Osd_Draw4Num(20, 14, freq &0xfffe);
	}
	else
		{
		HDisplay =mStar_ReadWord(SPRVDC_H) &0xfffe;
		  if(DecVScaleFlag)
			{
				HDisplay = HDisplay - VScaleDecValue;
			}
	    Osd_Draw4Num(20, 14, HDisplay);
		}
	Osd_DrawPropStr(21, 14, X_Text());
	Osd_DrawPropStr(26, 14, VResText());
	Osd_DrawPropStr(3, 15, HFrequenceText());
        freq = ((DWORD)MST_CLOCK_MHZ *10+SrcHPeriod / 2) / SrcHPeriod; // round 5
	if ((freq % 10) >=5)
	//round off
	{
	    Osd_DrawNum(14, 15, (freq + 10) / 10);
	}
	else
	{
	    Osd_DrawNum(14, 15, freq / 10);
	}
	Osd_DrawPropStr(18, 15, KHzText());
	Osd_DrawPropStr(3, 16, VFrequenceText());
	freq = ((DWORD)freq *1000) / SrcVTotal;
	
      #if DEBUG_OSD
       printData("freq=%d",freq);
      #endif	 
	if ((freq % 10) >= 5)
	//round off
	{
	    Osd_DrawNum(14, 16, (freq + 10) / 10);
	}
	else
	{
	    Osd_DrawNum(14, 16, freq / 10);
	}
	Osd_DrawPropStr(18, 16, HzText());


    return TRUE;
}
#define NewMethod		1

BYTE GetPrevItem(MenuItemType *menuItem)
{

    signed char i=0;

#if DEBUG_ENABLE
printData("MenuItemIndex===%d", MenuItemIndex);
printData("i===%d", i);
#endif

    if (MenuItemIndex)
    {
        i= MenuItemIndex;
 //	    for (i =  MenuItemIndex -1; i >= 0; i--)
        for (; i >0;)  //poro:2008-09-03,for Envision OSD issue...
        {

            if((MenuPageIndex == MainMenu && ItemStep!=0))
// 2007/9/16  by KK                 if(!FactoryModeFlag && (MenuPageIndex == MainMenu && ItemStep!=0))
            {
#if 1//NewMethod
            i =  Up_Item[i];
#else
                if(ItemStep==8)
                {
                    // i=((i+2)%3)+(i/3)*3;
                    if( i==0 )
                    {
                    	if(FactoryModeFlag)
                    		i=9;
	                    else
                         	i=8;
                    }
                    else
	                    i = (i+5)%8;
                  //  printData("2 prev =%d", i);
                }
                else
				{	i=(i+ItemStep)%9;
	         //       printData("3 prev =%d", i);
                }
#endif
            }
			else
				i--;
			i=i%255;
            #if INPUT_TYPE==INPUT_1A2D||INPUT_TYPE==INPUT_2A2D||INPUT_TYPE==INPUT_1A1H
            if ((menuItem[i].Flags &mibDVIDisable) && (SrcInputType == Input_Digital||SrcInputType == Input_Digital2))
            #else
            if (menuItem[i].Flags &mibDVIDisable && SrcInputType == Input_Digital)
            #endif
            {
            #if DEBUG_ENABLE
		printMsg("if (menuItem[i].Flags &mibDVIDisable && SrcInputType == Input_Digital)");
		#endif
                continue;
            }
            if (menuItem[i].Flags &mibsRGBDisable && (UserPrefColorTemp == CTEMP_SRGB))
            {
                continue;
            }

            if ( menuItem[i].Flags & mibUserColor && ( UserPrefColorTemp != CTEMP_USER ||UserPrefDcrMode==1))//jeff 1112
            {
                continue;
            }
            if (menuItem[i].Flags &mibStdEnable && (UserPrefECOMode != ECO_Standard))
            {
                continue;
            }
            if (menuItem[i].Flags &mibDCRDisable && (UserPrefDcrMode))
            {
                continue;
            }
    #if Enable_Lightsensor
            if (menuItem[i].Flags &mibiCareDisable && (UserPrefiCareMode))
            {
                continue;
            }
    #endif
            if ((menuItem[i].Flags &mibBFEnable) && (!PictureBoostFlag))
            {
                continue;
            }
            if (menuItem[i].Flags &mibFactoryItem && !FactoryModeFlag)
            {
                continue;
            }
            if (menuItem[i].Flags &mibDemoDisable && DemoFlag)
            {
                continue;
            }
  #if Enable_Expansion
	    if((!ExpansionFlag)&& (MenuPageIndex == ExtraMenu)&&( i==2))//dq1108
            {
                continue;
            }
 #endif
 #if !Dual  //YF20071218
 		#if(!Youweipaike)
		if((MenuPageIndex == ExtraMenu)&&(i==0))
            {
                continue;
            }
		#endif
 #endif
 		if((MenuPageIndex == ExtraMenu)&&(i==1)&&(SrcInputType >= Input_Digital))  //071225
            {
                continue;
            } 
	#if PICTUREBOOST_FUN_EN
	      if (menuItem[i].Flags &mibPicBoostDisable )
            {
                continue;
            }
	#endif
            if (menuItem[i].Flags &mibSelectable && !(menuItem[i].Flags &mibDrawValue))
            {
                return i;
            }

        }
    }

    if((MenuPageIndex == MainMenu && ItemStep!=0))
 // 2007/9/16  by KK        if(!FactoryModeFlag && (MenuPageIndex == MainMenu && ItemStep!=0))
   {
		if(ItemStep == 8)
			;//i = itemlist[(i+9)%10];
		else
			i=(MenuItemIndex%3);
   }
   else
       i= MenuItemCount;

   for ( ; i >= 0; )
// 2007/9/13  by KK         for (i = MenuItemCount-1 ; i >= 0; i--)
    {
       if((MenuPageIndex == MainMenu && ItemStep!=0))
// 2007/9/16  by KK            if(!FactoryModeFlag && (MenuPageIndex == MainMenu && ItemStep!=0))
       {
#if 1//NewMethod
            i =  Left_Item[i];
#else
           if(ItemStep==8)
           {
// 2007/9/17  by KK                i=((i+2)%3)+(i/3)*3;
              ;// i = itemlist[(i+9)%10];
           }
           else
	           i=(i+ItemStep)%9;
#endif
       }
       else
           i--;

        #if INPUT_TYPE==INPUT_1A2D||INPUT_TYPE==INPUT_2A2D||INPUT_TYPE==INPUT_1A1H
        if ((menuItem[i].Flags &mibDVIDisable) && (SrcInputType == Input_Digital||SrcInputType == Input_Digital2))
        #else
        if (menuItem[i].Flags &mibDVIDisable && SrcInputType == Input_Digital)
        #endif
        {
            continue;
        }
        if (menuItem[i].Flags &mibsRGBDisable && (UserPrefColorTemp == CTEMP_SRGB))
        {
            continue;
        }
        if (menuItem[i].Flags &mibUserColor && ( UserPrefColorTemp != CTEMP_USER ||UserPrefDcrMode==1))
        // jeff 1112
        {
            continue;
        }
        if (menuItem[i].Flags &mibStdEnable && (UserPrefECOMode != ECO_Standard))
        {
            continue;
        }
        if (menuItem[i].Flags &mibDCRDisable && (UserPrefDcrMode))
        {
            continue;
        }
    #if Enable_Lightsensor
        if (menuItem[i].Flags &mibiCareDisable && (UserPrefiCareMode))
        {
            continue;
        }
    #endif
        if ((menuItem[i].Flags &mibBFEnable) && (!PictureBoostFlag))
        {
            continue;
        }
        if (menuItem[i].Flags &mibFactoryItem && !FactoryModeFlag)
        {
            continue;
        }
        if (menuItem[i].Flags &mibDemoDisable && DemoFlag)
        {
            continue;
        }
#if Enable_Expansion
	    if((!ExpansionFlag)&& (MenuPageIndex == ExtraMenu)&&( i==2))//dq1108
            {
                continue;
            }
#endif
#if !Dual //YF20071218
		if((MenuPageIndex == ExtraMenu)&&(i==0))
	        {
                continue;
            }		
#endif
		if((MenuPageIndex == ExtraMenu)&&(i==1)&&(SrcInputType >= Input_Digital))	//071225
			{
				continue;
			} 
	#if PICTUREBOOST_FUN_EN
	      if (menuItem[i].Flags &mibPicBoostDisable )
            {
                continue;
            }
	#endif
        if (menuItem[i].Flags &mibSelectable && !(menuItem[i].Flags &mibDrawValue))
        {
            return i;
        }

    }
    return MenuItemIndex;

}

BYTE GetNextItem(MenuItemType *menuItem)
{

    BYTE i=0 ,firsttime ;

    if (MenuItemIndex < MenuItemCount - 1)
    // current item is not last item.
    {
        i = MenuItemIndex;
//        for (i = MenuItemIndex + 1; i < MenuItemCount; i++)
        for ( ; i < MenuItemCount-1;)
        {
            if((MenuPageIndex == MainMenu && ItemStep!=0))
 // 2007/9/16  by KK                if(!FactoryModeFlag && (MenuPageIndex == MainMenu && ItemStep!=0))
	{
#if 1//NewMethod
		  i =  Down_Item[i];
#else
                if(ItemStep==8)
                {
// 2007/9/17  by KK                     i=((i+1)%3)+(i/3)*3;
                    i=((i+5)%8); //  i = itemlist[(i+1)%10];
                    if(i==5 && (i/8==0))
                        i=8;
                }
                else
                {
                    i=(i+ItemStep)%9;
                }
#endif
            }
			else
				i++;

            #if INPUT_TYPE==INPUT_1A2D||INPUT_TYPE==INPUT_2A2D||INPUT_TYPE==INPUT_1A1H
            if ((menuItem[i].Flags &mibDVIDisable) && (SrcInputType == Input_Digital||SrcInputType == Input_Digital2))
            #else
            if (menuItem[i].Flags &mibDVIDisable && SrcInputType == Input_Digital)
            #endif
            {
                continue;
            }
            if (menuItem[i].Flags &mibsRGBDisable && (UserPrefColorTemp == CTEMP_SRGB))
            {
                continue;
            }
            if (menuItem[i].Flags &mibUserColor && ( UserPrefColorTemp != CTEMP_USER ||UserPrefDcrMode==1))
            //jeff1112
            {
                continue;
            }
            if (menuItem[i].Flags &mibStdEnable && (UserPrefECOMode != ECO_Standard))
            {
                continue;
            }
            if (menuItem[i].Flags &mibDCRDisable && (UserPrefDcrMode))
            {
                continue;
            }
    #if Enable_Lightsensor
            if (menuItem[i].Flags &mibiCareDisable && (UserPrefiCareMode))
            {
                continue;
            }
    #endif
            if ((menuItem[i].Flags &mibBFEnable) && (!PictureBoostFlag))
            {
                continue;
            }
            if (menuItem[i].Flags &mibFactoryItem && !FactoryModeFlag)
            {
                continue;
            }
            if (menuItem[i].Flags &mibDemoDisable && DemoFlag)
            {
                continue;
            }
#if Enable_Expansion
	    if((!ExpansionFlag)&& (MenuPageIndex == ExtraMenu)&&( i==2))//dq1108
            {
                continue;
            }
#endif
#if !Dual  //YF20071218
		if((MenuPageIndex == ExtraMenu)&&(i==0))
		    {
                continue;
            	   }		
#endif
		if((MenuPageIndex == ExtraMenu)&&(i==1)&&(SrcInputType >= Input_Digital))  //071225
		{
			continue;
		} 
	#if PICTUREBOOST_FUN_EN
	      if (menuItem[i].Flags &mibPicBoostDisable )
            {
                continue;
            }
	#endif
            if (menuItem[i].Flags &mibSelectable && !(menuItem[i].Flags &mibDrawValue))
            {
                return i;
            }
        }
    }

    if((MenuPageIndex == MainMenu && ItemStep!=0))
// 2007/9/16  by KK         if(!FactoryModeFlag && (MenuPageIndex == MainMenu && ItemStep!=0))
	{
		if(ItemStep == 8)
	     ;//   i = itemlist[(i+1)%10];
	    else
		    i=(MenuItemIndex%3);
	}
    else
	    i= 0;

// 2007/9/13  by KK     for (i = 0; i < MenuItemIndex;i++  )
	firsttime=1;
    for (; i < MenuItemIndex;)
    {
        if((MenuPageIndex == MainMenu && ItemStep!=0))
 // 2007/9/16  by KK            if(!FactoryModeFlag && (MenuPageIndex == MainMenu && ItemStep!=0))
        {
#if 1//NewMethod
	     i =  Right_Item[i];
#else
            if(ItemStep==8)
            {
// 2007/9/17  by KK                 i=((i+1)%3)+(i/3)*3;
               ;// i = itemlist[(i+1)%10];
            }
        	else
            {
                i=(i+ItemStep)%9;
            }
#endif
        }
        else
        {
			if(!firsttime)
	        	i++;
            else
	            firsttime=0;
        }
        #if INPUT_TYPE==INPUT_1A2D||INPUT_TYPE==INPUT_2A2D||INPUT_TYPE==INPUT_1A1H
        if ((menuItem[i].Flags &mibDVIDisable) && (SrcInputType == Input_Digital||SrcInputType == Input_Digital2))
        #else
        if (menuItem[i].Flags &mibDVIDisable && SrcInputType == Input_Digital)
        #endif
        {
            continue;
        }
        if (menuItem[i].Flags &mibsRGBDisable && (UserPrefColorTemp == CTEMP_SRGB))
        {
            continue;
        }
        if (menuItem[i].Flags &mibUserColor && ( UserPrefColorTemp != CTEMP_USER ||UserPrefDcrMode==1))
        //jeff 1112
        {
            continue;
        }
        if (menuItem[i].Flags &mibStdEnable && (UserPrefECOMode != ECO_Standard))
        {
            continue;
        }
        if (menuItem[i].Flags &mibDCRDisable && (UserPrefDcrMode))
        {
            continue;
        }
    #if Enable_Lightsensor
        if (menuItem[i].Flags &mibiCareDisable && (UserPrefiCareMode))
        {
            continue;
        }
    #endif
        if ((menuItem[i].Flags &mibBFEnable) && (!PictureBoostFlag))
        {
            continue;
        }
        if (menuItem[i].Flags &mibFactoryItem && !FactoryModeFlag)
        {
            continue;
        }
        if (menuItem[i].Flags &mibDemoDisable && DemoFlag)
        {
            continue;
        }
#if Enable_Expansion
	    if((!ExpansionFlag)&& (MenuPageIndex == ExtraMenu)&&( i==2))//dq1108
            {
                continue;
            }
#endif
#if !Dual  //YF20071218
		#if(!Youweipaike)
		if((MenuPageIndex == ExtraMenu)&&(i==0))
			{
                continue;
            }
		#endif
#endif
	if((MenuPageIndex == ExtraMenu)&&(i==1)&&(SrcInputType >= Input_Digital))
		{
			continue;
		} 
	#if PICTUREBOOST_FUN_EN
	      if (menuItem[i].Flags &mibPicBoostDisable )
            {
                continue;
            }
	#endif
        if (menuItem[i].Flags &mibSelectable && !(menuItem[i].Flags &mibDrawValue))
        {
            return i;
        }
    }
    return MenuItemIndex;

}

#if !DefaultIconExit//(!AOC_37_Series && ModelName!=ENVISION_P2071l&&ModelName!=ENVISION_P2271wl&&ModelName!=ENVISION_P971wl)
// menuPageIndex => prev page index
// MenuPageIndex => current page index
BYTE GetMenuItemIndex(BYTE menuPageIndex)
{
    if (MenuPageIndex == MainMenu)
    {
    	    	#if DEBUG_ENABLE
        printData("menuPageIndex=%d", menuPageIndex);
	#endif

        if (menuPageIndex == ImageSetupMenu)
        {
            return 1;
        }
        else if (menuPageIndex == ColorTempMenu)
        {
            return 2;
        }
        else if (menuPageIndex == ColorBoostMenu)
        {
            return 3;
        }
        else if (menuPageIndex == BrightFrameMenu)
        {
            return 4;
        }
        else if (menuPageIndex == OsdSetUpMenu)
        {
            return 5;
        }
        else if (menuPageIndex == ExtraMenu)
        {
            return 6;
        }
        else if (menuPageIndex == ResetMenu)
        {
            return 7;
        }
    }
#if 0
	else if (MenuPageIndex == ColorBoostMenu)
    {
        if (menuPageIndex == SkinProtectMenu)
        {
            return 1;
        }
        else if (menuPageIndex == GreenEnhanceMenu)
        {
            return 2;
        }
        else if (menuPageIndex == BlueEnhanceMenu)
        {
            return 3;
        }
        else if (menuPageIndex == AutoDetectMenu)
        {
            return 4;
        }
        else if (menuPageIndex == DemoMenu)
        {
            return 5;
        }
    }
#endif
    else if (MenuPageIndex == BrightFrameMenu)
    {
    	#if DEBUG_ENABLE
        printData("menuPageIndex=%d", menuPageIndex);
	#endif
        if (!PictureBoostFlag)
        {
		return 5;
        }
#if 0
        else if (menuPageIndex == BF_BrightnessMenu)
        {
            return 1;
        }

        else if (menuPageIndex == BF_ContrastMenu)
        {
            return 2;
        }

		else if (menuPageIndex == BFHPositionMenu)
		{
		    return 3;
		}
		else if (menuPageIndex == BFVPositionMenu)
		{
		    return 4;
		}
#endif
    }
#if 0
    else if (MenuPageIndex == BF_PositionMenu)
    {
        if (menuPageIndex == BFVPositionMenu)
        {
            return 1;
        }
    }

    else if (MenuPageIndex == OsdSetUpMenu)
    {
        if (menuPageIndex == OsdVPositionMenu)
        {
            return 1;
        }
        else if (menuPageIndex == OsdTimeOutMenu)
        {
            return 2;
        }
        else if (menuPageIndex == OsdTransparenceMenu)
        {
            return 3;
        }

        else if (menuPageIndex == LanguageMenu)
        {
            return 4;
        }

    }
#endif
#if 0
    else if (MenuPageIndex == ExtraMenu)
    {
    #if 0
        #if DDCCI_ENABLE||Enable_Expansion
            #if DDCCI_ENABLE
                if (menuPageIndex == DDCCIMenu)
                {
                    return DDCCI_ENABLE + Dual;
                }
                else
            #endif
            #if Enable_Expansion
                if (menuPageIndex == ExpansionMenu)
                {
                    return DDCCI_ENABLE + Enable_Expansion + Dual;
                }
                else
            #endif
    #endif
            if (menuPageIndex == ResetMenu)
            {
                return 1+DDCCI_ENABLE + Enable_Expansion + Dual;
            }
            else if (menuPageIndex == InformationMenu)
            {
                return 2+DDCCI_ENABLE + Enable_Expansion + Dual;
            }
        #else
            if (menuPageIndex == InformationMenu)
            {
                return 2+Dual;
            }
            else if (menuPageIndex == ResetMenu)
            {
                return 1+Dual;
            }
        #endif

#if 0
        else if (menuPageIndex == AutoConfigMenu)
        {
            return 0+Dual;
        }
#endif
    }

#endif
#if 0
    else if (MenuPageIndex == InputSelectMenu)
    {
        return UserPrefInputType;
    }

    else if (MenuPageIndex == AutoConfigMenu)
    {
        return 1;
    }
#endif
    else if (MenuPageIndex == ResetMenu)
    {
        return 0;
    }
 	
    //else
    return 0;
}
#endif




void DrawOsdBackGround(void)
{
	BYTE i , shiftPos;
#if !Enable_OsdEngine1 && DisplayLogo == EnvisionLOGO
	BYTE tempAddr;
	 (MenuPageIndex==MainMenu)?(tempAddr=Frame2ColorAddr1):(tempAddr=Frame2ColorAddr);
#endif

       if( OsdWindowWidth==HOTKEY_X_SIZE)
        shiftPos=1;
       else
        shiftPos=0;


#if Enable_OsdEngine1
 	Osd_SetTextColor(0, 0x11);
#else
 	Osd_SetTextColor(0, 0);
#endif
       Osd_DrawContinuesChar(0,0,SpaceFont,OsdWindowWidth);

	for (i=OsdWindowHeight-4;i<OsdWindowHeight;i++)
       Osd_DrawContinuesChar(0,i,SpaceFont,OsdWindowWidth);

 	Osd_SetTextColor(0, 0); //Center
	for (i=1; i < OsdWindowHeight-4 ; i++)
	{
      	  Osd_DrawContinuesChar(0,i,SpaceFont,OsdWindowWidth);
	}

#if Enable_OsdEngine1
	mStar_WriteOSDByte(0x15, 0x09);
#if OSD_PIVOT_EN  //071226
	if(OSDRotateFlag)
	{
       mStar_WriteOSDByte(0x17, 0x02);
       mStar_WriteOSDByte(0x18, 0x02);
       mStar_WriteOSDByte(0x19, 0x02);
       mStar_WriteOSDByte(0x1d, 0);
       mStar_WriteOSDByte(0x1e, 0);
       mStar_WriteOSDByte(0x1f, 0);	   
	}
      else
      {
       mStar_WriteOSDByte(0x17, 0);
       mStar_WriteOSDByte(0x18, 0);
       mStar_WriteOSDByte(0x19, 0);
       mStar_WriteOSDByte(0x1d, 0x02);
       mStar_WriteOSDByte(0x1e, 0x02);
       mStar_WriteOSDByte(0x1f, 0x02);	   
      }
#else
       mStar_WriteOSDByte(0x1d, 0x02);
       mStar_WriteOSDByte(0x1e, 0x02);
       mStar_WriteOSDByte(0x1f, 0x02);
#endif
       mStar_WriteOSDByte(0x29, 0x01);
       mStar_WriteOSDByte(0x2b, OsdWindowWidth-1);  //071226
       mStar_WriteOSDByte(0x2c, 0x01);
       mStar_WriteOSDByte(0x2d, 0x02);
       mStar_WriteOSDByte(0x2e, 0x06);
      //SetOsdSubWindow(SUB_WIN_01, BYTE HStar, BYTE HEnd, BYTE VStar, BYTE VEnd, BYTE Color)
       Osd_SetOsdEngine(1);
#endif
	Osd_SetTextColor(0, 0x11);
      	Osd_DrawContinuesChar(0,0,UPMiddle,OsdWindowWidth);
       Osd_DrawCharDirect(OsdWindowWidth-1, 0, UpRight_Corner);
       Osd_DrawCharDirect(0, 0, UpLeft_Corner);
       Osd_SetTextColor(0,  0x00);
       for (i=1;i<OsdWindowHeight;i++)
      	Osd_DrawContinuesChar(0,i,SpaceFont,OsdWindowWidth);

//===============================================================
//===============================================================
//draw AOC small logo
#if DisplayLogo!=NoBrand
        LoadMainSmallLogoFont();
#endif
#if Enable_OsdEngine1
        Osd_Set256TextColor( 17, Color_2);
#else
	#if AOC_MENU_FRAME_MODE
	 Osd_SetTextColor(0, 0);
	#else
        Osd_Set256TextColor( 106, Color_2);
	#endif
#endif
        Osd_DrawContinuesChar(0, OsdWindowHeight-3, SpaceFont, OsdWindowWidth);
        Osd_DrawContinuesChar(0, OsdWindowHeight-2, SpaceFont, OsdWindowWidth);
#if DisplayLogo == AOCLOGO
              for (i=0; i<6; i++)
             {
		Osd_DrawCharDirect(12-shiftPos+i,OsdWindowHeight-3,Frame2ColorAddr+i);
		Osd_DrawCharDirect(12-shiftPos+i,OsdWindowHeight-2,Frame2ColorAddr+6+i);
                }
#elif DisplayLogo == EnvisionLOGO

     #if Enable_OsdEngine1
        for (i=0; i<9; i++)
       {
        Osd_DrawCharDirect(10-shiftPos+i,OsdWindowHeight-3,Frame2ColorAddr+i);
        Osd_DrawCharDirect(10-shiftPos+i,OsdWindowHeight-2,Frame2ColorAddr+9+i);	
       }	 
	Osd_Set256TextColor(18, Color_2);
	Osd_DrawCharDirect(16-shiftPos,OsdWindowHeight-2,Frame2ColorAddr+0xF);
	Osd_DrawCharDirect(16-shiftPos,OsdWindowHeight-3,Frame2ColorAddr+0x6);

     #else
       for (i=0; i<9; i++)
       {
       Osd_DrawCharDirect(10-shiftPos+i,OsdWindowHeight-3,tempAddr+i);
       Osd_DrawCharDirect(10-shiftPos+i,OsdWindowHeight-2,tempAddr+9+i);	
       }
	Osd_Set256TextColor(18, Color_2);
	Osd_DrawCharDirect(16-shiftPos,OsdWindowHeight-2,tempAddr+0xF);
	Osd_DrawCharDirect(16-shiftPos,OsdWindowHeight-3,tempAddr+0x6);
     #endif
	 
#elif DisplayLogo == StaplesLOGO
        for (i=0; i<7; i++)
        {
        Osd_DrawCharDirect(11-shiftPos+i,OsdWindowHeight-3,Frame2ColorAddr+i);
        Osd_DrawCharDirect(11-shiftPos+i,OsdWindowHeight-2,Frame2ColorAddr+7+i);
        }
#endif
//===============================================================
//===============================================================

#if Enable_OsdEngine1
	 Osd_Set256TextColor( 16, Color_2T);
#else
	#if Enable_SubWin&&Enable_GDColor
        SetOsdSubWindow(SUB_WIN_01, 0, OsdWindowWidth-1 , 1, 2, 0);
	#if !AOC_MENU_FRAME_MODE
        SetOsdSubWindow(SUB_WIN_02, 0, OsdWindowWidth-1 , OsdWindowHeight-3, OsdWindowHeight-1, 0);
	#endif
	 Osd_Set256TextColor( 8, Color_2T);
	#else
        Osd_Set256TextColor( 0, Color_2T);
	#endif
#endif
    		Osd_DrawCharDirect(0, OsdWindowHeight-1 , DownLeft_Corner);
    		Osd_DrawCharDirect(OsdWindowWidth-1, OsdWindowHeight-1 , DownRight_Corner);
		//for(i=1;i<OsdWindowWidth-1;i++)
		    //Osd_DrawCharDirect(i, OsdWindowHeight-1 , DownMiddle);
             Osd_DrawContinuesChar(1,OsdWindowHeight - 1,DownMiddle,OsdWindowWidth-2);

#if Enable_OsdEngine1
   	 Osd_SetOsdEngine(0);
#endif

#if Enable_OsdEngine1
	if(MenuPageIndex==MainMenu)
	{
 		Osd_MainFrameOSD_E1();
	}
#endif

}


#if  BlacklitTimeOnShow
void Draw_BlacklitTimeOn_Info_LGFactoryMenu(void)
{ 	
   	Osd_SetTextColor(CP_WhiteColor, CP_BlueColor);

	Osd_Draw4Num(2+8,26, BlacklitTimeOn_H);
    	
	  //Osd_DrawNum(8+8,24, BlacklitTimeOn_M);//7
    	
	//Osd_DrawNum(11+8, 27,BlacklitTimeOn_S);
 
}
 #endif

//======================================================================================
#if (DisplayLogo==EnvisionLOGO)
    // Draw Envision Logo
    BYTE code EnvisionLogoText[7][65] =
    {
        //   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5   6   7   8   9   0   1   2   3   4
  {"\x01\x02\x02\x02\x02\x02\x02\x03\x04\x05\x06\x07\x08\x08\x08\x08\x09\x0A\x08\x0B\x0C\x08\x08\x08\x08\x08\x0D\x0E\x0F\x10\x11\x08\x12\x13\x14\x14\x14\x14\x15\x16\x17\x09\x18\x08\x19\x1A\x1B\x14\x14\x14\x14\x1C\x1D\x1E\x09\x1F\x20\x21\x08\x08\x08\x08\x08\x01"},
  {"\x22\x08\x08\x08\x08\x08\x08\x08\x23\x24\x25\x26\x27\x08\x08\x08\x28\x29\x08\x2A\x2B\x2C\x08\x08\x08\x08\x2D\x2E\x08\x2F\x30\x08\x31\x32\x08\x08\x08\x08\x08\x33\x34\x28\x24\x08\x35\x36\x08\x08\x08\x08\x08\x08\x37\x38\x28\x39\x3A\x3B\x3C\x08\x08\x08\x08\x22"},
  {"\x22\x3D\x3D\x3D\x3D\x3D\x3D\x3E\x23\x24\x08\x3F\x40\x41\x42\x08\x28\x29\x08\x08\x43\x44\x17\x08\x08\x45\x46\x08\x08\x2F\x30\x08\x47\x48\x49\x4A\x4A\x4A\x4B\x4C\x08\x28\x24\x08\x4D\x4E\x08\x08\x08\x08\x08\x08\x4F\x30\x28\x39\x08\x50\x51\x52\x07\x08\x08\x22"},
  {"\x22\x53\x53\x53\x53\x53\x53\x54\x23\x24\x08\x08\x55\x56\x57\x58\x28\x29\x08\x08\x08\x59\x5A\x08\x5B\x5C\x5D\x08\x08\x2F\x30\x08\x5E\x5F\x60\x61\x61\x61\x62\x63\x64\x28\x24\x08\x4D\x4E\x08\x08\x08\x08\x08\x08\x4F\x30\x28\x39\x08\x08\x65\x66\x67\x27\x08\x22"},
  {"\x22\x68\x68\x68\x68\x68\x68\x69\x23\x24\x08\x08\x08\x08\x6A\x6B\x6C\x29\x08\x08\x08\x65\x6D\x6E\x6F\x70\x08\x08\x08\x2F\x30\x08\x71\x72\x42\x08\x08\x08\x73\x74\x75\x28\x24\x08\x76\x77\x42\x08\x08\x08\x08\x78\x79\x7A\x28\x39\x08\x08\x08\x08\x7B\x7C\x7D\x22"},
  {"\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7F\x80\x81\x08\x08\x08\x08\x08\x82\x83\x84\x08\x08\x08\x08\x85\x7E\x86\x08\x08\x08\x08\x87\x88\x08\x89\x8A\x8B\x8B\x8B\x8B\x8C\x8D\x8E\x8F\x81\x08\x90\x91\x92\x93\x93\x93\x93\x94\x95\x08\x8F\x96\x08\x08\x08\x08\x08\x97\x98\x7E"},
  {"\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x99\x99\x99\x99\x99\x9A\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08\x08"},
    };
    BYTE code EnvisionLogoText_1[7][11] =
    {
        //   1   2   3   4   5   6   7   8   9   0   1   2   3   4
   {"\x19\x1A\x1B\x14\x14\x14\x14\x1C\x1D\x1E"},
   {"\x35\x36\x08\x08\x08\x08\x08\x08\x37\x38"},
   {"\x4D\x4E\x08\x08\x08\x08\x08\x08\x4F\x30"},
   {"\x4D\x4E\x08\x08\x08\x08\x08\x08\x4F\x30"},
   {"\x76\x77\x42\x08\x08\x08\x08\x78\x79\x7A"},
   {"\x90\x91\x92\x93\x93\x93\x93\x94\x95\x08"},
   {"\x08\x08\x99\x99\x99\x99\x99\x9A\x08\x08"}
    };
#elif (DisplayLogo==StaplesLOGO)
    BYTE code StaplesLogoText[13][37] =
    {
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x01"},// 1
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x01\x01\x01\x01\x01\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x21\x01"},// 2
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x22\x23\x24\x25\x26\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x27\x01"},// 3
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x28\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x29\x2a\x2b\x02\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38"},// 4
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x39\x02\x02\x02\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x02\x44\x45\x46\x47\x48\x49\x4a\x02\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x02\x02\x02\x54"},// 5
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x55\x02\x02\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x02\x02\x74"},// 6
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x75\x02\x02\x76\x77\x78\x79\x7a\x7b\x7c\x02\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x02\x02\x93"},// 7
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x94\x02\x02\x95\x96\x97\x98\x99\x9a\x01\x9b\x9c\x9d\x9e\x9f\xa0\xa1\x02\x02\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\x02\x02\x02\xaf"},// 8
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\xb0\x02\x02\xb1\xb2\xb3\xb4\xb5\x02\xb6\xb7\xb8\x02\x02\xb9\xba\xbb\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\xbc"},// 9
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x01\xbd\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\xbe\xbf\xc0\xc1\xc2\xc3\xc4"},// 10
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x01\xc5\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\x02\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\x01\x01\x01\x01\x01\x01"},// 11
        //    1     2     3     4    5     6     7    8     9    10   11    12   13  14   15   16   17   18   19   20   21   22   23   24   25   26   27    28  29   30   31   32   33   34   35   36
  {"\x01\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"},// 12
    };
#elif (DisplayLogo==PuntaLOGO)
    BYTE code PuntalogoText[11][62] =
 {
  {"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"},
  {"\x01\x02\x03\x04\x04\x04\x04\x04\x04\x04\x04\x05\x06\x01\x07\x08\x01\x01\x01\x01\x01\x01\x01\x09\x0A\x0B\x01\x01\x0C\x0D\x04\x04\x04\x04\x04\x04\x0E\x0F\x01\x01\x10\x04\x11\x12\x13\x14\x04\x04\x04\x15\x01\x01\x01\x16\x17\x18\x19\x01\x01\x01\x01"},
  {"\x01\x1A\x1B\x1C\x1D\x1D\x1D\x1D\x1D\x1D\x1D\x1E\x1F\x20\x21\x1B\x01\x01\x01\x01\x01\x01\x01\x22\x1B\x23\x01\x24\x25\x26\x1D\x1D\x1D\x1D\x1D\x27\x28\x29\x2A\x01\x2B\x1D\x2C\x2D\x2E\x2F\x30\x1D\x1D\x31\x01\x32\x33\x34\x35\x36\x37\x38\x01\x01\x01"},
  {"\x01\x1A\x1B\x39\x3A\x3A\x3A\x3A\x3A\x3A\x3A\x3B\x3C\x01\x21\x1B\x3D\x01\x01\x01\x01\x01\x01\x22\x1B\x3E\x01\x3F\x40\x41\x01\x01\x01\x01\x01\x01\x42\x1B\x43\x01\x01\x01\x01\x44\x1B\x45\x01\x01\x01\x01\x46\x47\x48\x49\x01\x4A\x4B\x4C\x4D\x01\x01"},
  {"\x01\x1A\x1B\x4E\x4F\x4F\x4F\x4F\x4F\x4F\x4F\x50\x01\x01\x51\x52\x53\x54\x54\x54\x54\x54\x55\x56\x57\x58\x01\x59\x5A\x01\x01\x01\x01\x01\x01\x01\x1A\x1B\x5B\x01\x01\x01\x01\x5C\x1B\x5D\x01\x01\x01\x5E\x5F\x60\x61\x01\x01\x01\x62\x63\x64\x65\x01"},
  {"\x01\x66\x67\x68\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x69\x6A\x6B\x6B\x6B\x6B\x6B\x6B\x6C\x6D\x01\x01\x6E\x6F\x01\x01\x01\x01\x01\x01\x01\x70\x71\x72\x01\x01\x01\x01\x73\x74\x75\x01\x01\x01\x76\x77\x78\x01\x01\x01\x01\x01\x79\x7A\x7B\x01"},
  {"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"},
  {"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x7C\x7D\x01\x01\x01\x7E\x7F\x01\x80\x01\x01\x01\x01\x81\x82\x83\x01\x01\x01\x01\x01\x01\x01\x84\x01"},
  {"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x85\x86\x87\x01\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x01"},
  {"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x9C\x9D\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x9E\x9F\x01\x01\x01\x01"},

 };
#elif (DisplayLogo==AcerLOGO)
#define acM(x)        (x+1)
//#define qin(x)        (0x00+x*2)//(2+x)//logo_font_star_addrs

 WORD code straceWindow[13][55]=
{
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x01), acM(0x02), acM(0x03), acM(0x03), 
		 acM(0x03), acM(0x03), acM(0x03), acM(0x04), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x05), acM(0x06), acM(0x03), acM(0x03), acM(0x03), acM(0x07), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x08), acM(0x09), acM(0x03), acM(0x03), 
		 acM(0x0A), acM(0x0B), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x01), 
		 acM(0x0C), acM(0x03), acM(0x03), acM(0x03), acM(0x0D), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0x0E), acM(0x0F), acM(0x10), acM(0x10), acM(0x10), 
		 acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x11), acM(0x12), acM(0x00), acM(0x13), 
		 acM(0x14), acM(0x15), acM(0x16), acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x17), 
		 acM(0x18), acM(0x19), acM(0x1A), acM(0x1B), acM(0x10), acM(0x10), acM(0x10), acM(0x10), 
		 acM(0x10), acM(0x10), acM(0x11), acM(0x1C), acM(0x1D), acM(0x1E), acM(0x1F), acM(0x20), 
		 acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x21), acM(0x22), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0x23), acM(0x24), acM(0x25), acM(0x26), acM(0x27), 
		 acM(0x27), acM(0x28), acM(0x29), acM(0x2A), acM(0x10), acM(0x10), acM(0x2B), acM(0x2C), 
		 acM(0x10), acM(0x2D), acM(0x2E), acM(0x2F), acM(0x30), acM(0x31), acM(0x32), acM(0x33), 
		 acM(0x34), acM(0x10), acM(0x35), acM(0x36), acM(0x37), acM(0x38), acM(0x27), acM(0x39), 
		 acM(0x3A), acM(0x3B), acM(0x10), acM(0x10), acM(0x3C), acM(0x10), acM(0x10), acM(0x3D), 
		 acM(0x3E), acM(0x3F), acM(0x3F), acM(0x3F), acM(0x40), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x41), acM(0x42), acM(0x43), 
		 acM(0x44), acM(0x45), acM(0x46), acM(0x47), acM(0x10), acM(0x10), acM(0x10), acM(0x10), 
		 acM(0x48), acM(0x49), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x4A), 
		 acM(0x10), acM(0x10), acM(0x4B), acM(0x00), acM(0x00), acM(0x4C), acM(0x4D), acM(0x4E), 
		 acM(0x4F), acM(0x10), acM(0x10), acM(0x50), acM(0x10), acM(0x10), acM(0x51), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x00), acM(0x52), acM(0x53), acM(0x54), acM(0x55), acM(0x10), acM(0x56), acM(0x57), 
		 acM(0x58), acM(0x59), acM(0x5A), acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x10), 
		 acM(0x5B), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x5C), acM(0x10), 
		 acM(0x10), acM(0x5D), acM(0x5E), acM(0x5F), acM(0x60), acM(0x10), acM(0x10), acM(0x61), 
		 acM(0x62), acM(0x63), acM(0x64), acM(0x65), acM(0x10), acM(0x10), acM(0x66), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x67), acM(0x68), acM(0x10), acM(0x69), acM(0x6A), acM(0x6B), acM(0x6C), acM(0x00), 
		 acM(0x00), acM(0x6D), acM(0x6E), acM(0x10), acM(0x56), acM(0x10), acM(0x10), acM(0x10), 
		 acM(0x6F), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x70), acM(0x10), 
		 acM(0x10), acM(0x71), acM(0x72), acM(0x73), acM(0x74), acM(0x75), acM(0x76), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x77), acM(0x10), acM(0x10), acM(0x78), acM(0x79), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x7A), acM(0x10), acM(0x10), acM(0x7B), acM(0x7C), acM(0x7D), acM(0x7E), acM(0x7E), 
		 acM(0x7E), acM(0x7F), acM(0x10), acM(0x10), acM(0x80), acM(0x81), acM(0x10), acM(0x10), 
		 acM(0x82), acM(0x83), acM(0x84), acM(0x85), acM(0x86), acM(0x87), acM(0x88), acM(0x10), 
		 acM(0x10), acM(0x89), acM(0x8A), acM(0x8B), acM(0x85), acM(0x86), acM(0x86), acM(0x8C), 
		 acM(0x8D), acM(0x8E), acM(0x8F), acM(0x10), acM(0x10), acM(0x90), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x91), acM(0x92), acM(0x93), acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x10), 
		 acM(0x10), acM(0x10), acM(0x94), acM(0x95), acM(0x96), acM(0x97), acM(0x98), acM(0x99), 
		 acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x9A), acM(0x9B), 
		 acM(0x9C), acM(0x9D), acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x10), acM(0x10), 
		 acM(0x9E), acM(0x9F), acM(0xA0), acM(0x10), acM(0xA1), acM(0xA2), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0xA3), acM(0xA4), acM(0x27), acM(0x27), acM(0xA5), 
		 acM(0xA6), acM(0x6C), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0xA7), acM(0x27), acM(0x27), acM(0x27), acM(0xA8), acM(0xA9), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0xAA), acM(0xAB), acM(0x27), acM(0x27), acM(0x27), acM(0xA5), acM(0xA6), 
		 acM(0x00), acM(0xAC), acM(0x27), acM(0x27), acM(0xAD), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0xAE), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0xAF), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0xB0), acM(0x00), 
		 acM(0xB1), acM(0xB2), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0xB3), acM(0xB4), 
		 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0xB5), 
		 acM(0xB6), acM(0xB7), acM(0xB8), acM(0xB9), acM(0xBA), acM(0xBB), acM(0xBC), acM(0xBD), 
		 acM(0xBE), acM(0xBF), acM(0xC0), acM(0xC1), acM(0xC2), acM(0xC3), acM(0xC4), acM(0xC5), 
		 acM(0xC6), acM(0xC7), acM(0xC8), acM(0xC9), acM(0xCA), acM(0xCB), acM(0xCC), acM(0xCD), 
		 acM(0xCE), acM(0xCF), acM(0xD0), acM(0xD1), acM(0xD2), acM(0xD3), acM(0xD4), acM(0xD5), 
		 acM(0xD6), acM(0xD7), acM(0xD8), acM(0xD9), acM(0xDA), acM(0xDB), acM(0xDC), acM(0xDD), 
		 acM(0xDE), acM(0xDF), acM(0xE0), acM(0xE1), acM(0xE2), acM(0xE3), 
	 0},
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0xE4), 
		 acM(0xE5), acM(0xE6), acM(0xE7), acM(0xE8), acM(0xE9), acM(0xEA), acM(0xEB), acM(0xEC), 
		 acM(0xED), acM(0xEE), acM(0xEF), acM(0xF0), acM(0xF1), acM(0xF2), acM(0x79), acM(0xF3), 
		 acM(0xF4), acM(0xF5), acM(0xF6), acM(0xF7), acM(0xF8), acM(0xF9), acM(0xFA), acM(0xFB), 
		 acM(0xFC), acM(0xFD), acM(0xFE), acM(0xFF), acM(0x100), acM(0x101), acM(0x79), acM(0x102), 
		 acM(0x103), acM(0x104), acM(0x105), acM(0x106), acM(0x107), acM(0x108), acM(0x109), acM(0x10A), 
		 acM(0x10B), acM(0x10C), acM(0x10D), acM(0x00), acM(0x00), acM(0x00), 
	 0},
	{	 acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x00), acM(0x10E), 
		 acM(0x10F), acM(0x110), acM(0x111), acM(0x112), acM(0x113), acM(0x114), acM(0x115), acM(0x116), 
		 acM(0x117), acM(0x118), acM(0x119), acM(0x00), acM(0x11A), acM(0x11B), acM(0x00), acM(0x11C), 
		 acM(0x11D), acM(0xF0), acM(0x11E), acM(0x11F), acM(0x120), acM(0x121), acM(0x122), acM(0x123), 
		 acM(0x114), acM(0x124), acM(0x125), acM(0x126), acM(0x127), acM(0x128), acM(0x00), acM(0x129), 
		 acM(0x12A), acM(0x12B), acM(0x12C), acM(0x12D), acM(0x12E), acM(0x12F), acM(0x130), acM(0x131), 
		 acM(0x132), acM(0x133), acM(0x134), acM(0x00), acM(0x00), acM(0x00), 
	 0}
};
#elif (DisplayLogo==WearnesLOGO)
    BYTE code PuntalogoText[4][35] =
 {
  {"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F"},
  {"\x10\x0F\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30"},
  {"\x31\x32\x0F\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51"},
  {"\x52\x53\x54\x55\x56\x57\x58\x59\x0F\x5A\x5B\x5C\x5D\x0F\x5E\x58\x5F\x60\x61\x62\x63\x64\x65\x66\x67\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E"},

 };

#else
    BYTE code AOCLogoText[10][40] =
    {
        //   1   2   3   4   5   6   7   8   9   0  1	 2	 3	 4	 5	 6	 7	 8	9  0  1   2   3   4   5   6   7   8   9   0	  1	  2	  3	  4	  5	  6    7  8   9	  0
  {"\x01\x01\x01\x01\x01\x02\x03\x04\x05\x05\x05\x05\x05\x01\x01\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x01\x01\x10\x11\x12\x13\x14\x0A\x0A\x15\x16\x17\x18\x01"},
  {"\x01\x01\x01\x01\x19\x1A\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x01\x1C\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x1D\x1E\x01\x1F\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x20\x01"},
  {"\x01\x01\x01\x01\x21\x1B\x1B\x22\x23\x24\x1B\x1B\x1B\x01\x25\x1B\x1B\x26\x27\x28\x29\x2A\x2B\x1B\x1B\x2C\x2D\x2E\x1B\x1B\x2F\x30\x28\x31\x32\x1B\x1B\x33\x01"},
  {"\x01\x01\x01\x34\x35\x1B\x36\x37\x01\x38\x1B\x1B\x1B\x01\x39\x1B\x1B\x3A\x01\x01\x01\x01\x3B\x1B\x1B\x3C\x3D\x1B\x1B\x1B\x3E\x01\x01\x01\x3F\x40\x40\x41\x01"},
  {"\x01\x01\x42\x43\x1B\x1B\x44\x01\x01\x38\x1B\x1B\x1B\x01\x39\x1B\x1B\x45\x01\x01\x01\x01\x3B\x1B\x1B\x3C\x3D\x1B\x1B\x1B\x46\x01\x01\x01\x01\x01\x01\x01\x01"},
  {"\x01\x01\x47\x1B\x1B\x48\x49\x01\x01\x38\x1B\x1B\x1B\x01\x39\x1B\x1B\x4A\x01\x01\x01\x01\x4B\x1B\x1B\x4C\x3D\x1B\x1B\x1B\x4D\x01\x01\x01\x4E\x4F\x4F\x50\x01"},
  {"\x01\x51\x1B\x1B\x52\x53\x01\x01\x01\x38\x1B\x1B\x1B\x01\x54\x1B\x1B\x55\x56\x57\x58\x59\x35\x1B\x1B\x5A\x5B\x2B\x1B\x1B\x5C\x5D\x5E\x5F\x60\x1B\x1B\x61\x01"},
  {"\x62\x63\x1B\x1B\x64\x01\x01\x01\x01\x38\x1B\x1B\x1B\x01\x65\x66\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x67\x68\x01\x69\x6A\x1B\x1B\x1B\x1B\x1B\x1B\x1B\x6B\x6C\x01"},
  {"\x6D\x6E\x6E\x6F\x70\x01\x01\x01\x01\x71\x6E\x6E\x6E\x01\x01\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x01\x01\x01\x7C\x7D\x7E\x7F\x80\x81\x78\x82\x83\x01\x01"},
    };
#endif
#if (DisplayLogo!=NoBrand)
void DrawLogo(void) // Draw AOC Logo
{
    BYTE i;
    //  MonoColorSetting();
    //printMsg("Draw Logo Frame ");
    Osd_LoadLogoFontCP();
    Osd_SetTextMonoColor(0, CP_BlackColor);
    #if (DisplayLogo==EnvisionLOGO)
        Osd_SetTextMonoColor(CP_BlackColor, 0);
        for (i = 0; i < OsdWindowHeight; i++)
            Osd_DrawRealStr(0, i, EnvisionLogoText[i]);
        Osd_SetTextMonoColor(CP_BlackColor, 4);
        for (i = 0; i < OsdWindowHeight; i++)
            Osd_DrawRealStr(44, i, EnvisionLogoText_1[i]);
    #elif (DisplayLogo==StaplesLOGO)
        Osd_SetTextMonoColor(CP_RedColor, CP_BlackColor);
        for (i = 0; i < OsdWindowHeight; i++)
        {
            Osd_DrawRealStr(0, i, StaplesLogoText[i]);
        }
        Osd_SetTextMonoColor(CP_RedColor, CP_WhiteColor);
        for (i = 3; i < 9; i++)
        {
            BYTE j;
            for (j = 3; j < 34; j++)
                Osd_DrawCharDirect(j, i, StaplesLogoText[i][j]);
        }
	#elif (DisplayLogo==PuntaLOGO)
        Osd_SetTextMonoColor(CP_WhiteColor, CP_RedColor);
        for (i = 0; i < OsdWindowHeight; i++)
        {
            Osd_DrawRealStr(0, i, PuntalogoText[i]);
        }
         mStar_AdjustBackgoundColor(0);
	#elif (DisplayLogo==AcerLOGO)
	for(i=0;i<13;i++)
	{
		BYTE j;
	if(i<=8)
        Osd_SetTextMonoColor(CP_RedColor, CP_BlackColor);
	else
        Osd_SetTextMonoColor(CP_WhiteColor, CP_BlackColor);
	  for(j=0; j<54;j++)	
        {
	if(straceWindow[i][j]>=acM(0xFF))
     	{
         mStar_WriteByte(0xAE,mStar_ReadByte(0xAE)|BIT3);
	  Osd_DrawCharDirect(j, i, (straceWindow[i][j]-0xff));
	  mStar_WriteByte(0xAE,mStar_ReadByte(0xAE)&(~BIT3));
	 }
		
     else	 	
       Osd_DrawCharDirect(j, i, straceWindow[i][j]);
        }
		}
         mStar_AdjustBackgoundColor(0);
	#elif (DisplayLogo==WearnesLOGO)
        Osd_SetTextMonoColor(CP_WhiteColor, CP_BlackColor);
        for (i = 0; i < OsdWindowHeight; i++)
        {
            Osd_DrawRealStr(0, i, PuntalogoText[i]);
        }
         mStar_AdjustBackgoundColor(4);

    #else
        for (i = 0; i < OsdWindowHeight; i++)
        {
            Osd_DrawRealStr(0, i, AOCLogoText[i]);
            // printMsg("Draw Logo _1 ");
        }
    #endif
    mStar_WriteOSDByte(0x29, 0x00);//disable subwin
    //#if (ModelName == AOC_919Vwa)
    mStar_WriteOSDByte(0x2f, 0x00);
    //#endif
 #if (DisplayLogo==PuntaLOGO||DisplayLogo==WearnesLOGO)
    mStar_WriteByte(FWC, BIT0);
#else
    mStar_WriteByte(FWC, 0);
#endif
}
#endif





#if Enable_OsdEngine1
void Osd_MainFrameOSD_E1(void)
{
    BYTE i,j,k;

	Osd_SetOsdEngine(1);
	 	Osd_SetTextColor(0, 9); //Center
	for(i=iconx1_xpos-1; i<iconx3_xpos+1; i+=8)//=9)
	{
        for(j=icon1y_ypos; j<=icon3y_ypos; j+=5)
        {
            for(k=0; k<6; k++)
	        Osd_DrawCharDirect(i+k, j-1, E1_FRAME_01+k);
		 Osd_DrawCharDirect(i+6, j-1, E1_FRAME_07);

            for(k=0;k<3;k++)
             {
	        Osd_DrawCharDirect(i+6, j+k, E1_FRAME_08+k);
	        Osd_DrawCharDirect(i+0, j+k, E1_FRAME_13-k);
              }
	        Osd_DrawContinuesChar(i+1, j+2, UPMiddle, 5);
        }
   }
	Osd_SetOsdEngine(0);
}
#endif
