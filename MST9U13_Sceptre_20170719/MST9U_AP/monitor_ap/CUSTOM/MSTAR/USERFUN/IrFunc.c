#include "Board.h"





#define _IRFUNC_C_

// System
//#include <intrins.h>

// Common
//#include "board.h"
//#include "define.h"
#include "datatype.h"
#include "Mcu.h"

// External
#include "Global.h"
//#include "msModeDetect.h"
//#include "uart.h"
#include "MsIR.h"
#include "Menu.h"
#include "menufunc.h"
#include "Keypad.h"
#include "msOSD.h"
#include "i2c.h"
#include "UserPref.h"
#include "Power.h"
//#include "font.h"
#include "Menu.h"
#include "menudef.h"
#include "KeyPaddef.h"
#include "FactoryMenu.h"
// Internal
#include "IrFunc.h"

#define IR_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&IR_DEBUG
    #define IR_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
    #define IR_PRINT(format, ...)
#endif

#if(IR_REMOTE_SEL != NO_REMOTE)
BYTE IRKeyToNumber( BYTE ucCode )
{
    switch( ucCode )
    {
        case IRKEY_0:   return 0;
        case IRKEY_1:   return 1;
        case IRKEY_2:   return 2;
        case IRKEY_3:   return 3;
        case IRKEY_4:   return 4;
        case IRKEY_5:   return 5;
        case IRKEY_6:   return 6;
        case IRKEY_7:   return 7;
        case IRKEY_8:   return 8;
        case IRKEY_9:   return 9;
    }
    return 0xFF;
}
#if 1
void irDecodeCommand(void)
{
    BOOL bProcessDefaultIREvent = TRUE;
    BYTE ucTmp;
    BYTE ucRepeatFlag = 0;

    if (g_bIrDetect)
    {
        g_bIrDetect = 0;
        if (g_bIRRepeat) // check repeat status
        {
        #if(!(IR_MODE_SEL==IR_RAW_DATA_POL_MODE ||IR_MODE_SEL==IR_RAW_DATA_INT_MODE))
            if (g_bIRRepeatTimeOut) // check
                g_bIRRepeatTimeOut = 0;
            else
        #endif
                return;
        }
    }
    else
    {
        return;
    }
    //IR_PRINT("\r\nir=%x \n", g_ucIrCode);

    if ((!POWERON_FLAG1) && (!(g_ucIrCode == IRKEY_POWER)
#ifdef IR_MODE_SEL_2
        ||(g_ucIrCode == IRKEY_POWER_2)
#endif			
#ifdef IR_MODE_SEL_3
        ||(g_ucIrCode == IRKEY_POWER_3)
#endif			
#ifdef IR_MODE_SEL_4
        ||(g_ucIrCode == IRKEY_POWER_4)
#endif	
#ifdef IR_MODE_SEL_5
        ||(g_ucIrCode == IRKEY_POWER_5)
#endif	
	) )
        return;

    if( !bProcessDefaultIREvent ) // Don't process default IR event
    {
        irDecodeEnd();
        return;
    }

#ifdef IR_MODE_SEL_2
if(IR_Mode_Sel_Flag)
{

#if NewIRFunction
{
    switch(g_ucIrCode)
    {
/*    
        case IRKEY_0:
        case IRKEY_1:
        case IRKEY_2:
        case IRKEY_3:
        case IRKEY_4:
        case IRKEY_5:
        case IRKEY_6:
        case IRKEY_7:
        case IRKEY_8:
        case IRKEY_9:
        case IRKEY_100:
        {
            ucTmp = IRKeyToNumber(g_ucIrCode);
            printData("IRKEY TO NUM = %d", ucTmp);
            break;
        }
*/
        case IRKEY_OSD_MENU_2:
        {
            //printMsg("MENU KEY ");
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag))
            {
             if(g_u8MenuPageIndex == FACTORY_MENU)
             {
              if(g_u8MenuItemIndex==FMI_Item_Exit)
               FactoryExecuteKeyEvent(MIA_PREV_ITEM);
		else	  
               FactoryExecuteKeyEvent(MIA_GOTO_ADJUST_ITEM);
             }
	#if ((defined(FengYuan_Project))||(defined(KunDe_Project)))
             VKeyPad=VK_MENU;	
	#else
              else if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                {
             #if LHCX_IR_Function_Enable
		  ExecuteKeyEvent(MIA_GOTO_MainMenu);	
		#else
                ExecuteKeyEvent(MIA_GOTO_NEXT);
		#endif
                 ResetOsdTimer();
                }
	#endif	
                ucRepeatFlag |= _BIT0;		 
           }
            break;
        }

        case IRKEY_OSD_UP_2:
        {
            //printMsg("VOL UP KEY ");
	           VKeyPad=VK_UP;	
                  IRkeyDelayCounter=40;  //50x10ms
        
            ucRepeatFlag |= _BIT1;
            
            break;
        }

        case IRKEY_OSD_DOWM_2:
        {
           // printMsg("VOL DOWN KEY ");

       if(IRkeyDelayCounter==0)
        {
         // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
     #if (LHCX_IR_Function_Enable&&LED_HOT_Menu_Func)       
  	       if (g_u8MenuPageIndex == MENU_ROOT)
                 ExecuteKeyEvent(MIA_HotLedMode);
             else
     #else	 
       #if (defined(LeXingEnMa_Project)||defined(FengYuan_Project))     
  	       if (g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTCROSS
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||(g_u8MenuPageIndex==FACTORY_MENU)
		   )
                 ExecuteKeyEvent(MIA_HOTCROSS);
  	       else		
	#elif (defined(KunDe_Project))
  	       if (g_u8MenuPageIndex == MENU_ROOT  )
                 ExecuteKeyEvent(MIA_HotBrightness);
  	       else		
	#endif	  
     #endif
   	        VKeyPad=VK_DOWN;		
  
               ucRepeatFlag |= _BIT1;		 
             }
              IRkeyDelayCounter=40;  //50x10ms
         }
          break;
        }

        case IRKEY_OSD_LEFT_2:
        {
              
            printMsg("CH LEFT KEY ");

            if(IRkeyDelayCounter==0)
            {
                   IRkeyDelayCounter=18;  //20x10ms
   	             VKeyPad=VK_DOWN;	                  
            }
            
 	     //VKeyPad=VK_LEFT;	
             ucRepeatFlag |= _BIT0;	
            	 
           break;
        }

        case IRKEY_OSD_RIGHT_2:
        {
            printMsg("CH RIGHT KEY ");

              if(IRkeyDelayCounter==0)
              {
                   IRkeyDelayCounter=18;  //20x10ms
   	             VKeyPad=VK_UP;	                  
             }
                
            ucRepeatFlag |= _BIT0;
           	 
           break;
        }

        case IRKEY_OSD_EXIT_2:
       {
           // printMsg("CH EXIT KEY ");
         if(g_u8MenuPageIndex == FACTORY_MENU)
         {
 	     VKeyPad=VK_EXIT;		
         }
    #if (defined(KunDe_Project))
	  else if (g_u8MenuPageIndex == MENU_ROOT )
         {
             ExecuteKeyEvent(MIA_HotInput);
         }
    #endif	
        else if (g_u8MenuPageIndex != MENU_ROOT)
        {
            //ExecuteKeyEvent(MIA_GotoPrev);
      #if Menu_Key_Use_For_Enter
 	     VKeyPad=VK_EXIT;		
      #else	  
 	     VKeyPad=VK_MENU;		
      #endif	  
        }

            ucRepeatFlag |= _BIT0;		 
           break;
        }		
				
        case IRKEY_OSD_Enter_2:
        {
            //printMsg("CH Enter KEY ");
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
              if(g_u8MenuPageIndex == FACTORY_MENU)
              {
   	        VKeyPad=VK_MENU;		
              }
              else 
	#if (defined(KunDe_Project))
  	     if (g_u8MenuPageIndex == MENU_ROOT )
             {
                 ExecuteKeyEvent(MIA_HotPictureMode);
             }
  	     else		
	#else
  	     if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
             {
           #if ENABLE_MULTI_WINDOW_SETTINGS_MENU  
               if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)||(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1))
                 ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
		 else	   
	    #endif	 	
                 ExecuteKeyEvent(MIA_NOTHING);
             }
  	     else		
	#endif	 	
  	     {
  	    #if Menu_Key_Use_For_Enter    
   	        VKeyPad=VK_MENU;		
	    #else	
   	        VKeyPad=VK_EXIT;		
	    #endif	
  	     }
               ucRepeatFlag |= _BIT0;		 
           }
           break;
        }		
#if 1
        case IRKEY_INPUT_2:
        {
            //printMsg("CH IRKEY_INPUT ");
	   if(IRkeyDelayCounter==0)
	   {
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||g_u8MenuPageIndex == MENU_STANDBY
		||g_u8MenuPageIndex == MENU_ENTERPOWERSAVING
		)
                {
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
	  #if (defined(FengYuan_Project))	
	       else if(g_u8MenuPageIndex == MENU_HOTINPUT)
	       {
                VKeyPad=VK_MENU;		
	       }
              else
              {
                VKeyPad=VK_EXIT;		
              }
	  #endif
               IRkeyDelayCounter=50;  //50x10ms
               ucRepeatFlag |= _BIT0;		 
            }
           } 
        }
        break;

        case IRKEY_VOL_UP_2:
        {
            //printMsg("VOL UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_VOL_UP;		
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
        #if ENABLE_AUDIO_SETTINGS_MENU    
            if (g_u8MenuPageIndex == MENU_HOTVOLUME)
            	{
                ExecuteKeyEvent(MIA_INC_VALUE);
                IRkeyDelayCounter=18;  //50x10ms
		}
	     else 
	#endif	 	
	     if(g_u8MenuPageIndex == MENU_HOTINPUT)	  
	     	{
	     	  VKeyPad=VK_UP;	
                IRkeyDelayCounter=40;  //40x10ms
	     	}
            else
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
                   IRkeyDelayCounter=18;  //50x10ms
	        }
                 ResetOsdTimer();
            }
            ucRepeatFlag |= _BIT0;
	   }
           break;
        }

        case IRKEY_VOL_DOWM_2:
        {
            //printMsg("VOL DOWN  KEY ");
	    if(IRkeyDelayCounter==0)
	    {
 	     VKeyPad=VK_VOL_DOWN;	
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
        #if ENABLE_AUDIO_SETTINGS_MENU    
            if (g_u8MenuPageIndex == MENU_HOTVOLUME)
            	{
                ExecuteKeyEvent(MIA_DEC_VALUE);
                IRkeyDelayCounter=18;  //50x10ms
            	}
 	     else
	#endif	 	
	    if(g_u8MenuPageIndex == MENU_HOTINPUT)	  
	     	{
	     	  VKeyPad=VK_DOWN;	
                IRkeyDelayCounter=40;  //40x10ms
	     	}
           else
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
		     IRkeyDelayCounter=18;  //50x10ms		
	        }
                     ResetOsdTimer();
            }
         }
            ucRepeatFlag |= _BIT0;
		 
           break;
        }

        case IRKEY_CH_UP_2:
        {
           // printMsg("BRI UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                  ResetOsdTimer();
                  IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_CH_DOWM_2:
        {
            //printMsg("BRI DOWN  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_DOWN;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {

            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
           }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_MUTE_2:
        {
           // printMsg("IRKEY_MUTE");
	 if(IRkeyDelayCounter==0)
	 {
 	     VKeyPad=VK_REPEAT;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
      //         if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTMUTE)
	// #if SleepMode_Enable		  	
	//	||g_u8MenuPageIndex == MENU_POWEROFFMSG
      //  #endif
	//	)
		
               {
	            ExecuteKeyEvent(MIA_HotMuteOnOff);
               }
                 ResetOsdTimer();
            }
                IRkeyDelayCounter=50;  //50x10ms
	   }
            ucRepeatFlag |= _BIT1;
           break;
        }

        case IRKEY_ZOOM_2:
        {
           // printMsg("IRKEY_ZOOM");
	   if((IRkeyDelayCounter==0)
    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	   &&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
    #endif
	   )
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT||g_u8MenuPageIndex == MENU_HOTCROSS
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
	            ExecuteKeyEvent(MIA_HotPictureFormat);
            }
                IRkeyDelayCounter=100;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }
        
        case IRKEY_PIP_2:
        {
            //printMsg("IRKEY_PIP");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PXP);
             }
                 IRkeyDelayCounter=50;  //50x10ms
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIP_Swap_2:
       {
            //printMsg("IRKEY_PIP_Swap ");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
//            if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
            }
                   IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#if ENABLE_AUDIO_SETTINGS_MENU
        case IRKEY_PIP_AudioSwap_2:
        {
            //printMsg("IRKEY_PIP_AudioSwap");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_AudioSwap);
            }
                IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#endif
        case IRKEY_PIP_Position_2:
       {
            //printMsg("IRKEY_PIP_Position");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#if SleepMode_Enable		
        case IRKEY_SLEEP_2:
        {
            printMsg("IRKEY_SLEEP");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_SLEEPMODE
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
			)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_SLEEPMODE)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotSleepMode);
            }
                IRkeyDelayCounter=50;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }		
 #endif		
#endif
		
        case IRKEY_POWER_2:
        {
            //printMsg("POWER KEY ");
 	     VKeyPad=VK_POWER;		
            break;
        }
    }
}
#else

    switch(g_ucIrCode)
    {

        case IRKEY_OSD_MENU_2:
        {
            //printMsg("MENU KEY ");
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag))
            if(IRkeyDelayCounter==0)
            {
             if(g_u8MenuPageIndex == FACTORY_MENU)
             {
              if(g_u8MenuItemIndex==FMI_Item_Exit)
               FactoryExecuteKeyEvent(MIA_PREV_ITEM);
		else	  
               FactoryExecuteKeyEvent(MIA_GOTO_ADJUST_ITEM);
		 IRkeyDelayCounter=50;  //50x10ms	
             }
	      else		 
	      	{
	#if (defined(KunDe_Project))
             VKeyPad=VK_MENU;	
	#else
              if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                {
           #if LHCX_IR_Function_Enable
		  ExecuteKeyEvent(MIA_GOTO_MainMenu);	
	    #else
                ExecuteKeyEvent(MIA_GOTO_NEXT);
	    #endif	
                 ResetOsdTimer();
                }
	#endif	
	       IRkeyDelayCounter=50;  //50x10ms	
	      	}
                ucRepeatFlag |= _BIT0;		 
           }
            break;
        }
		
        case IRKEY_OSD_UP_2:
        {
            //printMsg("VOL UP KEY ");
           if(IRkeyDelayCounter==0)
           {
	           VKeyPad=VK_UP;	
                  IRkeyDelayCounter=50;  //50x10ms
            ucRepeatFlag |= _BIT0;//_BIT1;
           }
            break;
        }

        case IRKEY_OSD_DOWM_2:
        {
           // printMsg("VOL DOWN KEY ");

       if(IRkeyDelayCounter==0)
        {
   	        VKeyPad=VK_DOWN;	
		 IRkeyDelayCounter=50;  //50x10ms		 	
               ucRepeatFlag |= _BIT0;//_BIT1;		 
         }
          break;
        }

        case IRKEY_OSD_LEFT_2:
        {
              
            //printMsg("CH LEFT KEY ");

            if(IRkeyDelayCounter==0)
             {
	           VKeyPad=VK_LEFT;	
   		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=60;  
		   else
                   IRkeyDelayCounter=18;  //20x10ms
             }
            
 	     //VKeyPad=VK_LEFT;	
             ucRepeatFlag |= _BIT0;	
            	 
           break;
        }

        case IRKEY_OSD_RIGHT_2:
        {
            //printMsg("CH RIGHT KEY ");

              if(IRkeyDelayCounter==0)
               {
		 VKeyPad=VK_RIGHT;
		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=60;  
		   else
                  IRkeyDelayCounter=18;  //20x10ms
                }
                
            ucRepeatFlag |= _BIT0;
           	 
           break;
        }

        case IRKEY_OSD_EXIT_2:
       {
           // printMsg("CH EXIT KEY ");
      if(IRkeyDelayCounter==0)
      	{
         if(g_u8MenuPageIndex == FACTORY_MENU)
         {
 	     VKeyPad=VK_EXIT;		
         }
    #if (defined(KunDe_Project))
	  else if (g_u8MenuPageIndex == MENU_ROOT )
         {
             ExecuteKeyEvent(MIA_HotInput);
         }
    #endif	
        else if (g_u8MenuPageIndex != MENU_ROOT)
        {
            //ExecuteKeyEvent(MIA_GotoPrev);
         #if Menu_Key_Use_For_Enter
 	     VKeyPad=VK_EXIT;	
	  #else	 
 	     VKeyPad=VK_MENU;	
	  #endif
        }
	  IRkeyDelayCounter=50;  	 
      	}
            ucRepeatFlag |= _BIT0;		 
           break;
        }		
				
        case IRKEY_OSD_Enter_2:
        {
            //printMsg("CH Enter KEY ");
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
       if(IRkeyDelayCounter==0)
        {
              if(g_u8MenuPageIndex == FACTORY_MENU)
              {
   	        VKeyPad=VK_MENU;		
              }
	#if (defined(KunDe_Project))
  	    else if (g_u8MenuPageIndex == MENU_ROOT )
             {
                 ExecuteKeyEvent(MIA_HotPictureMode);
             }
  	     else		
	#else
  	     if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                 ExecuteKeyEvent(MIA_NOTHING);
  	     else	
	#endif	 	
	 #if Menu_Key_Use_For_Enter
   	        VKeyPad=VK_MENU;		
	 #else
   	        VKeyPad=VK_EXIT;		
        #endif
               ucRepeatFlag |= _BIT0;	
		 IRkeyDelayCounter=50;	   
           }
           break;
        }		
#if 1
        case IRKEY_INPUT_2:
        {
            //printMsg("CH IRKEY_INPUT ");
	   if(IRkeyDelayCounter==0)
	   {
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||g_u8MenuPageIndex == MENU_STANDBY
		||g_u8MenuPageIndex == MENU_ENTERPOWERSAVING
		)
                {
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
	#if (defined(FengYuan_Project))	
		else if(LR_UpDown_SwitchFlag)	   
		{
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
	        else if (g_u8MenuPageIndex != MENU_ROOT)
	        {
	   	        VKeyPad=VK_EXIT;		
	        }
       #endif
		
               IRkeyDelayCounter=50;  //50x10ms
               ucRepeatFlag |= _BIT0;		 
            }
           } 
        }
        break;

        case IRKEY_VOL_UP_2:
        {
            //printMsg("VOL UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_VOL_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
       #if ENABLE_AUDIO_SETTINGS_MENU     
          if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_INC_VALUE);
	   else	  
	#endif	  
	#if (defined(FengYuan_Project))		  
          if (g_u8MenuPageIndex == MENU_HOTINPUT)
                {
	           VKeyPad=VK_UP;	
                  IRkeyDelayCounter=50;  //50x10ms
                }
            else
        #endif		  
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
	#if (defined(FengYuan_Project))	
            if (g_u8MenuPageIndex != MENU_HOTINPUT)
	#endif
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
            ucRepeatFlag |= _BIT0;
	   }
           break;
        }
		
        case IRKEY_VOL_DOWM_2:
        {
            //printMsg("VOL DOWN  KEY ");
	    if(IRkeyDelayCounter==0)
	    {
 	     VKeyPad=VK_VOL_DOWN;	
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
       #if ENABLE_AUDIO_SETTINGS_MENU     
            if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_DEC_VALUE);
	     else		
	#endif		
	#if (defined(FengYuan_Project))	
          if (g_u8MenuPageIndex == MENU_HOTINPUT)
                {
	           VKeyPad=VK_DOWN;	
                  IRkeyDelayCounter=50;  //50x10ms

                }
            else
	#endif		
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
	#if (defined(FengYuan_Project))	
            if (g_u8MenuPageIndex != MENU_HOTINPUT)
	#endif
                     ResetOsdTimer();
             IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
		 
           break;
        }

        case IRKEY_CH_UP_2:
        {
           // printMsg("BRI UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                  ResetOsdTimer();
                  IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_CH_DOWM_2:
        {
            //printMsg("BRI DOWN  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_DOWN;		
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {

            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
           }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_MUTE_2:
        {
           // printMsg("IRKEY_MUTE");
	 if(IRkeyDelayCounter==0)
	 {
 	     VKeyPad=VK_REPEAT;		
            if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)
        #if ENABLE_AUDIO_SETTINGS_MENU
		||(g_u8MenuPageIndex == MENU_HOTMUTE)
	 #endif		   
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
               {
	            ExecuteKeyEvent(MIA_HotMuteOnOff);
               }
                 ResetOsdTimer();
            }
                IRkeyDelayCounter=50;  //50x10ms
	   }
            ucRepeatFlag |= _BIT1;
           break;
        }

        case IRKEY_ZOOM_2:
        {
           // printMsg("IRKEY_ZOOM");
	   if((IRkeyDelayCounter==0)
	 #if ENABLE_MULTI_WINDOW_SETTINGS_MENU  	
	   	&&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
        #endif
	   )
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotPictureFormat);
            }
                IRkeyDelayCounter=100;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }
        
        case IRKEY_PIP_2:
        {
            //printMsg("IRKEY_PIP");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PXP);
           }
                 IRkeyDelayCounter=50;  //50x10ms
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIP_Swap_2:
       {
            //printMsg("IRKEY_PIP_Swap ");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
            }
                   IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#if ENABLE_AUDIO_SETTINGS_MENU
        case IRKEY_PIP_AudioSwap_2:
        {
            //printMsg("IRKEY_PIP_AudioSwap");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_AudioSwap);
            }
                IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#endif
        case IRKEY_PIP_Position_2:
       {
            //printMsg("IRKEY_PIP_Position");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#if SleepMode_Enable		
        case IRKEY_SLEEP_2:
        {
            printMsg("IRKEY_SLEEP");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_SLEEPMODE
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
			)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_SLEEPMODE)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotSleepMode);
            }
                IRkeyDelayCounter=50;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }		
 #endif		
#endif
		
        case IRKEY_POWER_2:
        {
            //printMsg("POWER KEY ");
 	     VKeyPad=VK_POWER;		
            break;
        }
    }
#endif

}
else
#endif
#ifdef IR_MODE_SEL_3
if(IR_Mode_Sel3_Flag)
{
    switch(g_ucIrCode)
    {

        case IRKEY_OSD_MENU_3:
        {
            //printMsg("MENU KEY ");
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag))
            {
             if(g_u8MenuPageIndex == FACTORY_MENU)
             {
              if(g_u8MenuItemIndex==FMI_Item_Exit)
               FactoryExecuteKeyEvent(MIA_PREV_ITEM);
		else	  
               FactoryExecuteKeyEvent(MIA_GOTO_ADJUST_ITEM);
             }
              else if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                {
                 ExecuteKeyEvent(MIA_GOTO_NEXT);
                 ResetOsdTimer();
                }
                ucRepeatFlag |= _BIT0;		 
           }
            break;
        }
		
        case IRKEY_OSD_UP_3:
        {
            //printMsg("VOL UP KEY ");

	           VKeyPad=VK_UP;	
                  IRkeyDelayCounter=50;  //50x10ms
            ucRepeatFlag |= _BIT1;
            
            break;
        }

        case IRKEY_OSD_DOWM_3:
        {
           // printMsg("VOL DOWN KEY ");

       if(IRkeyDelayCounter==0)
        {
   	        VKeyPad=VK_DOWN;		
  
               ucRepeatFlag |= _BIT1;		 
              IRkeyDelayCounter=50;  //50x10ms
         }
          break;
        }

        case IRKEY_OSD_LEFT_3:
        {
              
            //printMsg("CH LEFT KEY ");

            if(IRkeyDelayCounter==0)
               {
        #if ENABLE_AUDIO_SETTINGS_MENU       
                  if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTCROSS))	 
                  {
                      ExecuteKeyEvent(MIA_HotVolume);
                  }
 		   else
	#endif	   	
		   if (g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT||g_u8MenuPageIndex == MENU_HOTCROSS)
  		   {
                   VKeyPad=VK_NOTHING;	
  		   }
                  else
	           VKeyPad=VK_LEFT;	
   		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=60;  
		   else
                   IRkeyDelayCounter=18;  //20x10ms
               }
            
 	     //VKeyPad=VK_LEFT;	
             ucRepeatFlag |= _BIT0;	
            	 
           break;
        }

        case IRKEY_OSD_RIGHT_3:
        {
            //printMsg("CH RIGHT KEY ");

              if(IRkeyDelayCounter==0)
                {
        #if ENABLE_AUDIO_SETTINGS_MENU        
                  if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTCROSS))	 
                  {
                      ExecuteKeyEvent(MIA_HotVolume);
                  }
		    else
	#endif			
		   if (g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT||g_u8MenuPageIndex==MENU_HOTPICTUREMODE||g_u8MenuPageIndex == MENU_HOTCROSS)
 		   {
                    VKeyPad=VK_NOTHING;	
 		   }
                  else
	           VKeyPad=VK_RIGHT;
		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=60;  
		   else
                  IRkeyDelayCounter=18;  //20x10ms
                }
                
            ucRepeatFlag |= _BIT0;
           	 
           break;
        }

        case IRKEY_OSD_EXIT_3:
       {
           // printMsg("CH EXIT KEY ");
         if(g_u8MenuPageIndex == FACTORY_MENU)
         {
 	     VKeyPad=VK_EXIT;		
         }
        else if (g_u8MenuPageIndex != MENU_ROOT)
        {
            //ExecuteKeyEvent(MIA_GotoPrev);
         #if Menu_Key_Use_For_Enter
 	     VKeyPad=VK_EXIT;		
	  #else	 
 	     VKeyPad=VK_MENU;		
	  #endif
        }

            ucRepeatFlag |= _BIT0;		 
           break;
        }		
				
        case IRKEY_OSD_Enter_3:
        {
            //printMsg("CH Enter KEY ");
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
              if(g_u8MenuPageIndex == FACTORY_MENU)
              {
   	        VKeyPad=VK_MENU;		
              }
              else 
  	     if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                 ExecuteKeyEvent(MIA_NOTHING);
  	     else		
	#if Menu_Key_Use_For_Enter
   	        VKeyPad=VK_MENU;		
	#else
   	        VKeyPad=VK_EXIT;		
       #endif
               ucRepeatFlag |= _BIT0;		 
           }
           break;
        }		
#if 1
        case IRKEY_INPUT_3:
        {
            //printMsg("CH IRKEY_INPUT ");
	   if(IRkeyDelayCounter==0)
	   {
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||g_u8MenuPageIndex == MENU_STANDBY
		||g_u8MenuPageIndex == MENU_ENTERPOWERSAVING
		)
                {
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
               IRkeyDelayCounter=50;  //50x10ms
               ucRepeatFlag |= _BIT0;		 
            }
           } 
        }
        break;

#if (defined(TaiBei_Project))	
        case IRKEY_VOL_UP_3:
        {
            //printMsg("VOL UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_VOL_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
          if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
            ucRepeatFlag |= _BIT0;
	   }
           break;
        }
		
        case IRKEY_VOL_DOWM_3:
        {
            //printMsg("VOL DOWN  KEY ");
	    if(IRkeyDelayCounter==0)
	    {
 	     VKeyPad=VK_VOL_DOWN;	
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                     ResetOsdTimer();
             IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
		 
           break;
        }
        case IRKEY_CH_UP_3:
        {
           // printMsg("BRI UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                  ResetOsdTimer();
                  IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_CH_DOWM_3:
        {
            //printMsg("BRI DOWN  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_DOWN;		
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {

            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
#endif
        case IRKEY_MUTE_3:
        {
           // printMsg("IRKEY_MUTE");
	 if(IRkeyDelayCounter==0)
	 {
 	     VKeyPad=VK_REPEAT;		
            if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)
        #if ENABLE_AUDIO_SETTINGS_MENU
	       ||(g_u8MenuPageIndex == MENU_HOTMUTE)
	 #endif      
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
               {
	            ExecuteKeyEvent(MIA_HotMuteOnOff);
               }
                 ResetOsdTimer();
            }
                IRkeyDelayCounter=50;  //50x10ms
	   }
            ucRepeatFlag |= _BIT1;
           break;
        }

        case IRKEY_ZOOM_3:
        {
           // printMsg("IRKEY_ZOOM");
	   if((IRkeyDelayCounter==0)
     #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	   &&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
     #endif
	 )
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT||g_u8MenuPageIndex == MENU_HOTCROSS
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
	            ExecuteKeyEvent(MIA_HotPictureFormat);
            }
                IRkeyDelayCounter=100;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }
        
        case IRKEY_PIP_3:
        {
            //printMsg("IRKEY_PIP");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PXP);
           }
                 IRkeyDelayCounter=50;  //50x10ms
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIP_Swap_3:
       {
            //printMsg("IRKEY_PIP_Swap ");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
            }
                   IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_AudioSwap_3:
        {
            //printMsg("IRKEY_PIP_AudioSwap");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_AudioSwap);
            }
                IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_Position_3:
       {
            //printMsg("IRKEY_PIP_Position");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#if SleepMode_Enable		
        case IRKEY_SLEEP_3:
        {
            printMsg("IRKEY_SLEEP");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_SLEEPMODE
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
			)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_SLEEPMODE)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotSleepMode);
            }
                IRkeyDelayCounter=50;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }		
 #endif		
#endif
		
        case IRKEY_POWER_3:
        {
            //printMsg("POWER KEY ");
 	     VKeyPad=VK_POWER;		
            break;
        }
    }
}
else
#endif
#ifdef IR_MODE_SEL_4
if(IR_Mode_Sel4_Flag)
{
    switch(g_ucIrCode)
    {

        case IRKEY_OSD_MENU_4:
        {
            //printMsg("MENU KEY ");
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag))
           if(IRkeyDelayCounter==0)
            {
             if(g_u8MenuPageIndex == FACTORY_MENU)
             {
              if(g_u8MenuItemIndex==FMI_Item_Exit)
               FactoryExecuteKeyEvent(MIA_PREV_ITEM);
		else	  
               FactoryExecuteKeyEvent(MIA_GOTO_ADJUST_ITEM);
             }
              else if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                {
            #if LHCX_IR_Function_Enable
		ExecuteKeyEvent(MIA_GOTO_MainMenu);	
		#else
                 ExecuteKeyEvent(MIA_GOTO_NEXT);
		#endif
                 ResetOsdTimer();
                }
		else
		{
                  VKeyPad=VK_MENU;	
		}
		 
		   IRkeyDelayCounter=70;  //50x10ms		
           }
                  ucRepeatFlag |= _BIT0;	
          break;
        }
		
        case IRKEY_OSD_UP_4:
        {
            //printMsg("VOL UP KEY ");

	           VKeyPad=VK_UP;	
                  IRkeyDelayCounter=50;  //50x10ms
            ucRepeatFlag |= _BIT1;
            
            break;
        }

        case IRKEY_OSD_DOWM_4:
        {
           // printMsg("VOL DOWN KEY ");

       if(IRkeyDelayCounter==0)
        {
         // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
   	        VKeyPad=VK_DOWN;		
  
               ucRepeatFlag |= _BIT1;		 
             }
              IRkeyDelayCounter=50;  //50x10ms
         }
          break;
        }

        case IRKEY_OSD_LEFT_4:
        {
              
            //printMsg("CH LEFT KEY ");

            if(IRkeyDelayCounter==0)
                {
                  if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTCROSS))	 
                  {
                      ExecuteKeyEvent(MIA_HotVolume);
                  }
                  else
	           VKeyPad=VK_LEFT;	
   		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=60;  
		   else
                   IRkeyDelayCounter=18;  //20x10ms
                }
            
 	     //VKeyPad=VK_LEFT;	
             ucRepeatFlag |= _BIT0;	
            	 
           break;
        }

        case IRKEY_OSD_RIGHT_4:
        {
            //printMsg("CH RIGHT KEY ");

              if(IRkeyDelayCounter==0)
                {
                  if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTCROSS))	 
                  {
                      ExecuteKeyEvent(MIA_HotVolume);
                  }
                  else
	           VKeyPad=VK_RIGHT;
		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=60;  
		   else
                  IRkeyDelayCounter=18;  //20x10ms
                }
                
            ucRepeatFlag |= _BIT0;
           	 
           break;
        }
/*
        case IRKEY_OSD_EXIT_4:
       {
           // printMsg("CH EXIT KEY ");
         if(g_u8MenuPageIndex == FACTORY_MENU)
         {
 	     VKeyPad=VK_EXIT;		
         }
        else if (g_u8MenuPageIndex != MENU_ROOT)
        {
            //ExecuteKeyEvent(MIA_GotoPrev);
 	     VKeyPad=VK_MENU;		
        }

            ucRepeatFlag |= _BIT0;		 
           break;
        }		
*/				
        case IRKEY_OSD_Enter_4:
        {
            //printMsg("CH Enter KEY ");
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
              if(g_u8MenuPageIndex == FACTORY_MENU)
              {
   	        VKeyPad=VK_MENU;		
              }
              else 
  	     if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                 ExecuteKeyEvent(MIA_NOTHING);
  	     else		
	#if Menu_Key_Use_For_Enter
   	        VKeyPad=VK_MENU;		
	#else
   	        VKeyPad=VK_EXIT;		
       #endif
               ucRepeatFlag |= _BIT0;		 
           }
           break;
        }		
#if 1
        case IRKEY_INPUT_4:
        {
            //printMsg("CH IRKEY_INPUT ");
	   if(IRkeyDelayCounter==0)
	   {
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||g_u8MenuPageIndex == MENU_STANDBY
		||g_u8MenuPageIndex == MENU_ENTERPOWERSAVING
		)
                {
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
               IRkeyDelayCounter=50;  //50x10ms
               ucRepeatFlag |= _BIT0;		 
            }
           } 
        }
        break;

#if (defined(TaiBei_Project))	
        case IRKEY_VOL_UP_3:
        {
            //printMsg("VOL UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_VOL_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
          if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
            ucRepeatFlag |= _BIT0;
	   }
           break;
        }
		
        case IRKEY_VOL_DOWM_3:
        {
            //printMsg("VOL DOWN  KEY ");
	    if(IRkeyDelayCounter==0)
	    {
 	     VKeyPad=VK_VOL_DOWN;	
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                     ResetOsdTimer();
             IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
		 
           break;
        }
        case IRKEY_CH_UP_3:
        {
           // printMsg("BRI UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                  ResetOsdTimer();
                  IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_CH_DOWM_3:
        {
            //printMsg("BRI DOWN  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_DOWN;		
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {

            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
#endif

        case IRKEY_MUTE_4:
        {
           // printMsg("IRKEY_MUTE");
	 if(IRkeyDelayCounter==0)
	 {
 	     VKeyPad=VK_REPEAT;		
            if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)
        #if ENABLE_AUDIO_SETTINGS_MENU
	       ||(g_u8MenuPageIndex == MENU_HOTMUTE)
	 #endif		   
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
               {
	            ExecuteKeyEvent(MIA_HotMuteOnOff);
               }
                 ResetOsdTimer();
            }
                IRkeyDelayCounter=50;  //50x10ms
	   }
            ucRepeatFlag |= _BIT1;
           break;
        }

        case IRKEY_ZOOM_4:
        {
           // printMsg("IRKEY_ZOOM");
	   if((IRkeyDelayCounter==0)
         #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	   &&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
         #endif
         )
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT||g_u8MenuPageIndex == MENU_HOTCROSS
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
	            ExecuteKeyEvent(MIA_HotPictureFormat);
            }
                IRkeyDelayCounter=100;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }
  /*      
        case IRKEY_PIP_4:
        {
            //printMsg("IRKEY_PIP");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PXP);
           }
                 IRkeyDelayCounter=50;  //50x10ms
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIP_Swap_4:
       {
            //printMsg("IRKEY_PIP_Swap ");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
            }
                   IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_AudioSwap_4:
        {
            //printMsg("IRKEY_PIP_AudioSwap");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_AudioSwap);
            }
                IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_Position_4:
       {
            //printMsg("IRKEY_PIP_Position");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
        */
#if 0//SleepMode_Enable		
        case IRKEY_SLEEP_4:
        {
            printMsg("IRKEY_SLEEP");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_SLEEPMODE
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
			)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_SLEEPMODE)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotSleepMode);
            }
                IRkeyDelayCounter=50;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }		
 #endif		
#endif
		
        case IRKEY_POWER_4:
        {
            //printMsg("POWER KEY ");
            #if LHCX_IR_Function_Enable
		ExecuteKeyEvent(MIA_POWER);	
		#else
 	     VKeyPad=VK_POWER;		
		#endif
            break;
        }
    }
}
else
#endif
#ifdef IR_MODE_SEL_5
if(IR_Mode_Sel5_Flag)
{
    switch(g_ucIrCode)
    {

        case IRKEY_OSD_MENU_5:
        {
            //printMsg("MENU KEY ");
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag))
            {
             if(g_u8MenuPageIndex == FACTORY_MENU)
             {
              if(g_u8MenuItemIndex==FMI_Item_Exit)
               FactoryExecuteKeyEvent(MIA_PREV_ITEM);
		else	  
               FactoryExecuteKeyEvent(MIA_GOTO_ADJUST_ITEM);
             }
              else if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                {
             #if LHCX_IR_Function_Enable
		  ExecuteKeyEvent(MIA_GOTO_MainMenu);	
		#else
                ExecuteKeyEvent(MIA_GOTO_NEXT);
		#endif
                 ResetOsdTimer();
                }
                //ucRepeatFlag |= _BIT0;		 
           }
            break;
        }
		
        case IRKEY_OSD_UP_5:
        {
            //printMsg("VOL UP KEY ");

       if(IRkeyDelayCounter==0)
         {
	           VKeyPad=VK_UP;	
   		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=45;  
		   else
                   IRkeyDelayCounter=18;  //20x10ms
       }
            ucRepeatFlag |= _BIT1;
            
            break;
        }

        case IRKEY_OSD_DOWM_5:
        {
           // printMsg("VOL DOWN KEY ");

        if(IRkeyDelayCounter==0)
        {
   	        VKeyPad=VK_DOWN;		
            if(LR_UpDown_SwitchFlag)	   
               IRkeyDelayCounter=43;  
            else
               IRkeyDelayCounter=18;  //20x10ms
         }
          break;
        }

        case IRKEY_OSD_LEFT_5:
        {
              
            //printMsg("CH LEFT KEY ");

            if(IRkeyDelayCounter==0)
               {
	           VKeyPad=VK_LEFT;	
   		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=43;  
		   else
                   IRkeyDelayCounter=18;  //20x10ms
               }
            
 	     //VKeyPad=VK_LEFT;	
             //ucRepeatFlag |= _BIT0;	
            	 
           break;
        }

        case IRKEY_OSD_RIGHT_5:
        {
            //printMsg("CH RIGHT KEY ");

              if(IRkeyDelayCounter==0)
                {
                  if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTCROSS))	 
                  {
                      ExecuteKeyEvent(MIA_HotVolume);
                  }
		   else if(g_u8MenuPageIndex==MENU_HOTVOLUME) 
		   {
   	             VKeyPad=VK_UP;		
		   }
                  else
	           VKeyPad=VK_RIGHT;
		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=43;  
		   else
                  IRkeyDelayCounter=18;  //20x10ms
                }
                
            //ucRepeatFlag |= _BIT0;
           	 
           break;
        }

        case IRKEY_OSD_EXIT_5:
       {
           // printMsg("CH EXIT KEY ");
         if(g_u8MenuPageIndex == FACTORY_MENU)
         {
 	     VKeyPad=VK_EXIT;		
         }
        else if (g_u8MenuPageIndex != MENU_ROOT)
        {
            //ExecuteKeyEvent(MIA_GotoPrev);
         #if Menu_Key_Use_For_Enter
 	     VKeyPad=VK_EXIT;	
	  #else	 
 	     VKeyPad=VK_MENU;	
	 #endif 
        }

            //ucRepeatFlag |= _BIT0;		 
           break;
        }		
				
        case IRKEY_OSD_Enter_5:
        {
            //printMsg("CH Enter KEY ");
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
              if(g_u8MenuPageIndex == FACTORY_MENU)
              {
   	        VKeyPad=VK_MENU;		
              }
              else 
  	     if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                 ExecuteKeyEvent(MIA_NOTHING);
  	     else		
		#if Menu_Key_Use_For_Enter
   	        VKeyPad=VK_MENU;		
		#else
   	        VKeyPad=VK_EXIT;		
             #endif
               ucRepeatFlag |= _BIT0;		 
           }
           break;
        }		
#if 1
        case IRKEY_INPUT_5:
        {
            //printMsg("CH IRKEY_INPUT ");
	   if(IRkeyDelayCounter==0)
	   {
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||g_u8MenuPageIndex == MENU_STANDBY
		||g_u8MenuPageIndex == MENU_ENTERPOWERSAVING
		)
                {
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
               IRkeyDelayCounter=5;  //50x10ms
               ucRepeatFlag |= _BIT0;		 
            }
           } 
        }
        break;

        case IRKEY_MUTE_5:
        {
           // printMsg("IRKEY_MUTE");
	 if(IRkeyDelayCounter==0)
	 {
 	     VKeyPad=VK_REPEAT;		
            if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)
        #if ENABLE_AUDIO_SETTINGS_MENU
		||(g_u8MenuPageIndex == MENU_HOTMUTE)
	 #endif		   
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
               {
	            ExecuteKeyEvent(MIA_HotMuteOnOff);
               }
                 ResetOsdTimer();
            }
                IRkeyDelayCounter=45;  //50x10ms
	   }
            //ucRepeatFlag |= _BIT1;
           break;
        }

        case IRKEY_ZOOM_5:
        {
           // printMsg("IRKEY_ZOOM");
	   if((IRkeyDelayCounter==0)
         #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	   &&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
         #endif
         )
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT||g_u8MenuPageIndex == MENU_HOTCROSS
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
	            ExecuteKeyEvent(MIA_HotPictureFormat);
            }
                IRkeyDelayCounter=100;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }
        
        case IRKEY_LOW_BLURAY_5:
        {
           // printMsg("IRKEY_LOW_BLURAY_5");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT)
		&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)
		#if Enable_HotAdjLowbluray
		||(g_u8MenuPageIndex == MENU_HOTLOWBLURAY)
		#endif
	 #if SleepMode_Enable		  	
		||(g_u8MenuPageIndex == MENU_POWEROFFMSG)
        #endif
		)
		#if Enable_HotAdjLowbluray
	            ExecuteKeyEvent(MIA_Lowbluray);
		#endif
            }
                IRkeyDelayCounter=10;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIC_MODE_5:
       {
           // printMsg("IRKEY_PIC_MODE_5");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTPICTUREMODE
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTPICTUREMODE)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotPictureMode);
            }
                IRkeyDelayCounter=5;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_DCR_5:
        {
           // printMsg("IRKEY_DCR_5");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTDCR
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTDCR)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotDCR);
            }
                IRkeyDelayCounter=5;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }

       
#endif
		
        case IRKEY_POWER_5:
        {
            //printMsg("POWER KEY ");
            #if LHCX_IR_Function_Enable
		ExecuteKeyEvent(MIA_POWER);	
		#else
 	     VKeyPad=VK_POWER;		
		#endif
            break;
        }
    }
}
else
#endif

#if JieBo_IR_Function_Enable
{
    switch(g_ucIrCode)
    {
        case IRKEY_0:
        case IRKEY_1:
        case IRKEY_2:
        case IRKEY_3:
        case IRKEY_4:
        case IRKEY_5:
        case IRKEY_6:
        case IRKEY_7:
        case IRKEY_8:
        case IRKEY_9:
        case IRKEY_100:
        {
            ucTmp = IRKeyToNumber(g_ucIrCode);
            printData("IRKEY TO NUM = %d", ucTmp);
            break;
        }

        case IRKEY_OSD_MENU:
        {
            //printMsg("MENU KEY ");
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag))
            {
             if(g_u8MenuPageIndex == FACTORY_MENU)
             {
              if(g_u8MenuItemIndex==FMI_Item_Exit)
               FactoryExecuteKeyEvent(MIA_PREV_ITEM);
		else	  
               FactoryExecuteKeyEvent(MIA_GOTO_ADJUST_ITEM);
             }
              else if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                {
                 ExecuteKeyEvent(MIA_GOTO_NEXT);
                 ResetOsdTimer();
                }
                ucRepeatFlag |= _BIT0;		 
           }
            break;
        }

        case IRKEY_OSD_UP:
        {
            //printMsg("VOL UP KEY ");
	           VKeyPad=VK_UP;	
                  IRkeyDelayCounter=50;  //50x10ms
            
            ucRepeatFlag |= _BIT1;
            
            break;
        }

        case IRKEY_OSD_DOWM:
        {
           // printMsg("VOL DOWN KEY ");

       if(IRkeyDelayCounter==0)
        {
         // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
       #if (defined(LeXingEnMa_Project))     
  	       if (g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTCROSS
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		   )
                 ExecuteKeyEvent(MIA_HOTCROSS);
  	       else		
	#endif	   	
   	        VKeyPad=VK_DOWN;		
  
               ucRepeatFlag |= _BIT1;		 
             }
              IRkeyDelayCounter=50;  //50x10ms
         }
          break;
        }

        case IRKEY_OSD_LEFT:
        {
              
            //printMsg("CH LEFT KEY ");

            if(IRkeyDelayCounter==0)
                {
	           VKeyPad=VK_LEFT;	
   		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=60;  
		   else
                   IRkeyDelayCounter=18;  //20x10ms
                }
            
 	     //VKeyPad=VK_LEFT;	
             ucRepeatFlag |= _BIT0;	
            	 
           break;
        }

        case IRKEY_OSD_RIGHT:
        {
            //printMsg("CH RIGHT KEY ");

              if(IRkeyDelayCounter==0)
                {
	           VKeyPad=VK_RIGHT;
		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=60;  
		   else
                  IRkeyDelayCounter=18;  //20x10ms
                }
                
            ucRepeatFlag |= _BIT0;
           	 
           break;
        }

        case IRKEY_OSD_EXIT:
       {
           // printMsg("CH EXIT KEY ");
         if(g_u8MenuPageIndex == FACTORY_MENU)
         {
 	     VKeyPad=VK_EXIT;		
         }
        else if (g_u8MenuPageIndex != MENU_ROOT)
        {
            //ExecuteKeyEvent(MIA_GotoPrev);
      #if Menu_Key_Use_For_Enter
 	     VKeyPad=VK_EXIT;		
      #else	  
 	     VKeyPad=VK_MENU;		
      #endif	  
        }

            ucRepeatFlag |= _BIT0;		 
           break;
        }		
				
        case IRKEY_OSD_Enter:
        {
            //printMsg("CH Enter KEY ");
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
              if(g_u8MenuPageIndex == FACTORY_MENU)
              {
   	        VKeyPad=VK_MENU;		
              }
              else 
  	     if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                 ExecuteKeyEvent(MIA_NOTHING);
  	     else		
  	     {
  	    #if Menu_Key_Use_For_Enter    
   	        VKeyPad=VK_MENU;		
	    #else	
   	        VKeyPad=VK_EXIT;		
	    #endif	
  	     }
               ucRepeatFlag |= _BIT0;		 
           }
           break;
        }		
#if 1
        case IRKEY_INPUT:
        {
            //printMsg("CH IRKEY_INPUT ");
	   if(IRkeyDelayCounter==0)
	   {
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||g_u8MenuPageIndex == MENU_STANDBY
		||g_u8MenuPageIndex == MENU_ENTERPOWERSAVING
		)
                {
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
               IRkeyDelayCounter=50;  //50x10ms
               ucRepeatFlag |= _BIT0;		 
            }
           } 
        }
        break;
		
        case IRKEY_VOL_UP:
        {
            //printMsg("VOL UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_VOL_UP;		
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
       #if ENABLE_AUDIO_SETTINGS_MENU     
          if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	#endif			
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
            ucRepeatFlag |= _BIT0;
	   }
           break;
        }
		
        case IRKEY_VOL_DOWM:
        {
            //printMsg("VOL DOWN  KEY ");
	    if(IRkeyDelayCounter==0)
	    {
 	     VKeyPad=VK_VOL_DOWN;	
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
         #if ENABLE_AUDIO_SETTINGS_MENU   
            if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	#endif			
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                     ResetOsdTimer();
             IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
		 
           break;
        }
        case IRKEY_CH_UP:
        {
           // printMsg("BRI UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                  ResetOsdTimer();
                  IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_CH_DOWM:
        {
            //printMsg("BRI DOWN  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_DOWN;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {

            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
           }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_MUTE:
        {
           // printMsg("IRKEY_MUTE");
	 if(IRkeyDelayCounter==0)
	 {
 	     VKeyPad=VK_REPEAT;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)
        #if ENABLE_AUDIO_SETTINGS_MENU
	       ||(g_u8MenuPageIndex == MENU_HOTMUTE)
	 #endif		   
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
               {
	            ExecuteKeyEvent(MIA_HotMuteOnOff);
               }
                 ResetOsdTimer();
            }
                IRkeyDelayCounter=50;  //50x10ms
	   }
            ucRepeatFlag |= _BIT1;
           break;
        }

        
        case IRKEY_PIP:
        {
            //printMsg("IRKEY_PIP");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP);
             }
                 IRkeyDelayCounter=50;  //50x10ms
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIP_Swap:
       {
            //printMsg("IRKEY_PIP_Swap ");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
//            if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
            }
                   IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIP_Size:
       {
            //printMsg("IRKEY_PIP_Size");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_Size);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_Position:
       {
            //printMsg("IRKEY_PIP_Position");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }


        case IRKEY_PBP_OFF:
       {
            //printMsg("IRKEY_PBP_OFF");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PBP_OFF);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
        case IRKEY_PBP_2WIN:
       {
            //printMsg("IRKEY_PBP_OFF");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PBP_2WIN);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
        case IRKEY_PBP_3WIN:
       {
            //printMsg("IRKEY_PBP_OFF");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PBP_3WIN);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
        case IRKEY_PBP_4WIN:
       {
            //printMsg("IRKEY_PBP_OFF");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PBP_4WIN);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
        case IRKEY_Source_DP:
       {
            //printMsg("IRKEY_PBP_OFF");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotInput_DP);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
        case IRKEY_Source_HDMI1:
       {
            //printMsg("IRKEY_PBP_OFF");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotInput_HDMI1);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
        case IRKEY_Source_HDMI2:
       {
            //printMsg("IRKEY_PBP_OFF");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotInput_HDMI2);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
        case IRKEY_Source_HDMI3:
       {
            //printMsg("IRKEY_PBP_OFF");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotInput_HDMI3);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }


		
#if SleepMode_Enable		
        case IRKEY_SLEEP:
        {
            printMsg("IRKEY_SLEEP");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_SLEEPMODE
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
			)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_SLEEPMODE)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotSleepMode);
            }
                IRkeyDelayCounter=50;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }		
 #endif		
#endif
		
        case IRKEY_POWER:
        {
            //printMsg("POWER KEY ");
 	     VKeyPad=VK_POWER;		
            break;
        }
    }
}
#elif NewIRFunction
{
    switch(g_ucIrCode)
    {
        case IRKEY_0:
        case IRKEY_1:
        case IRKEY_2:
        case IRKEY_3:
        case IRKEY_4:
        case IRKEY_5:
        case IRKEY_6:
        case IRKEY_7:
        case IRKEY_8:
        case IRKEY_9:
        case IRKEY_100:
        {
            ucTmp = IRKeyToNumber(g_ucIrCode);
            printData("IRKEY TO NUM = %d", ucTmp);
            break;
        }

        case IRKEY_OSD_MENU:
        {
            printMsg("MENU KEY ");
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag))
            {
             if(g_u8MenuPageIndex == FACTORY_MENU)
             {
              if(g_u8MenuItemIndex==FMI_Item_Exit)
               FactoryExecuteKeyEvent(MIA_PREV_ITEM);
		else	  
               FactoryExecuteKeyEvent(MIA_GOTO_ADJUST_ITEM);
             }
	#if (defined(FengYuan_Project)||defined(XuLi_Project))
             VKeyPad=VK_MENU;	
	#else
              else if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                {
                 ExecuteKeyEvent(MIA_GOTO_NEXT);
                 ResetOsdTimer();
                }
	#endif	
                ucRepeatFlag |= _BIT0;		 
           }
            break;
        }

        case IRKEY_OSD_UP:
        {
            //printMsg("VOL UP KEY ");
	           VKeyPad=VK_UP;	
                  IRkeyDelayCounter=38;  //50x10ms
            ucRepeatFlag |= _BIT1;
            
            break;
        }

        case IRKEY_OSD_DOWM:
        {
           // printMsg("VOL DOWN KEY ");

       if(IRkeyDelayCounter==0)
        {
         // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
       #if (defined(LeXingEnMa_Project)||defined(FengYuan_Project)||defined(XuLi_Project))     
  	       if (g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTCROSS
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||(g_u8MenuPageIndex==FACTORY_MENU)
		   )
        	{
        	#if (defined(Japan_Project)||defined(XuLi_Project))
                 ExecuteKeyEvent(MIA_HotPictureMode);
		#else	
                 ExecuteKeyEvent(MIA_HOTCROSS);
		#endif		 
        	}
  	       else		
	#endif	  
	#if 0//(!SevenKey_Function)
               if(LR_UpDown_SwitchFlag)
	#endif
   	        VKeyPad=VK_DOWN;		
  
               ucRepeatFlag |= _BIT1;		 
             }
              IRkeyDelayCounter=38;  //50x10ms
         }
          break;
        }

        case IRKEY_OSD_LEFT:
        {
              
            printMsg("CH LEFT KEY ");

            if(IRkeyDelayCounter==0)
            {
                   IRkeyDelayCounter=18;  //20x10ms
   	             VKeyPad=VK_DOWN;	                  
            }
            
 	     //VKeyPad=VK_LEFT;	
             ucRepeatFlag |= _BIT0;	
            	 
           break;
        }

        case IRKEY_OSD_RIGHT:
        {
             printMsg("CH RIGHT KEY ");

              if(IRkeyDelayCounter==0)
              {

                   IRkeyDelayCounter=18;  //20x10ms
   	             VKeyPad=VK_UP;	                  
		 printData("VKeyPad==%d",VKeyPad);
             }
                
            ucRepeatFlag |= _BIT0;
           	 
           break;
        }

        case IRKEY_OSD_EXIT:
       {
           // printMsg("CH EXIT KEY ");
         if(g_u8MenuPageIndex == FACTORY_MENU)
         {
 	     VKeyPad=VK_EXIT;		
         }
        else if (g_u8MenuPageIndex != MENU_ROOT)
        {
            //ExecuteKeyEvent(MIA_GotoPrev);
      #if Menu_Key_Use_For_Enter
 	     VKeyPad=VK_EXIT;		
      #else	  
 	     VKeyPad=VK_MENU;		
      #endif	  
        }

            ucRepeatFlag |= _BIT0;		 
           break;
        }		
				
        case IRKEY_OSD_Enter:
        {
            //printMsg("CH Enter KEY ");
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
           if(IRkeyDelayCounter==0)
            {
              if(g_u8MenuPageIndex == FACTORY_MENU)
              {
   	        VKeyPad=VK_MENU;		
		 IRkeyDelayCounter=10;	
              }
              else 
  	     if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
             {
         #if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&(!defined(Japan_Project))&&(!defined(XuLi_Project)))  
               if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)||(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1))
               {
                 ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
		   IRkeyDelayCounter=80;			 
               }
		 else
	  #endif	 	
		 {
                 ExecuteKeyEvent(MIA_NOTHING);
		   IRkeyDelayCounter=10;			 
		 }
             }
  	     else		
  	     {
  	    #if Menu_Key_Use_For_Enter    
   	        VKeyPad=VK_MENU;		
	    #else	
   	        VKeyPad=VK_EXIT;		
	    #endif	
		 IRkeyDelayCounter=10;			 

  	     }
               ucRepeatFlag |= _BIT0;		 
           }
           break;
        }		
#if 1
        case IRKEY_INPUT:
        {
            //printMsg("CH IRKEY_INPUT ");
	   if(IRkeyDelayCounter==0)
	   {
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||g_u8MenuPageIndex == MENU_STANDBY
		||g_u8MenuPageIndex == MENU_ENTERPOWERSAVING
		)
                {
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
	#if (defined(FengYuan_Project)||defined(XuLi_Project))
                else
                {
                  VKeyPad=VK_EXIT;	
                }
	#endif
               IRkeyDelayCounter=50;  //50x10ms
               ucRepeatFlag |= _BIT0;		 
            }
           } 
        }
        break;

        case IRKEY_VOL_UP:
        {
            //printMsg("VOL UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_VOL_UP;		
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
         #if ENABLE_AUDIO_SETTINGS_MENU   
          if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	 #endif			
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
            //ucRepeatFlag |= _BIT0;
	   }
           break;
        }
		
        case IRKEY_VOL_DOWM:
        {
            //printMsg("VOL DOWN  KEY ");
	    if(IRkeyDelayCounter==0)
	    {
 	     VKeyPad=VK_VOL_DOWN;	
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
        #if ENABLE_AUDIO_SETTINGS_MENU  
            if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	 #endif			
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                     ResetOsdTimer();
             IRkeyDelayCounter=18;  //50x10ms
            }
         }
            //ucRepeatFlag |= _BIT0;
		 
           break;
        }

        case IRKEY_CH_UP:
        {
           // printMsg("BRI UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                  ResetOsdTimer();
                  IRkeyDelayCounter=18;  //50x10ms
            }
         }
            //ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_CH_DOWM:
        {
            //printMsg("BRI DOWN  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_DOWN;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {

            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
           }
            //ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_MUTE:
        {
           // printMsg("IRKEY_MUTE");
	 if(IRkeyDelayCounter==0)
	 {
 	     VKeyPad=VK_REPEAT;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
      //         if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTMUTE)
	// #if SleepMode_Enable		  	
	//	||g_u8MenuPageIndex == MENU_POWEROFFMSG
      //  #endif
	//	)
		
               {
	            ExecuteKeyEvent(MIA_HotMuteOnOff);
               }
                 ResetOsdTimer();
            }
                IRkeyDelayCounter=80;  //50x10ms
	   }
            //ucRepeatFlag |= _BIT1;
           break;
        }

        case IRKEY_ZOOM:
        {
            printMsg("IRKEY_ZOOM");
	   if((IRkeyDelayCounter==0)
     #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	   &&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
     #endif
	   )
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT||g_u8MenuPageIndex == MENU_HOTCROSS
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
	            ExecuteKeyEvent(MIA_HotPictureFormat);
            }
                IRkeyDelayCounter=100;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }
        
        case IRKEY_PIP:
        {
            //printMsg("IRKEY_PIP");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PXP);
             }
                 IRkeyDelayCounter=50;  //50x10ms
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIP_Swap:
       {
            //printMsg("IRKEY_PIP_Swap ");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
//            if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
            }
                   IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_AudioSwap:
        {
            //printMsg("IRKEY_PIP_AudioSwap");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_AudioSwap);
            }
                IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_Position:
       {
            //printMsg("IRKEY_PIP_Position");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#if SleepMode_Enable		
        case IRKEY_SLEEP:
        {
            printMsg("IRKEY_SLEEP");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_SLEEPMODE
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
			)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_SLEEPMODE)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotSleepMode);
            }
                IRkeyDelayCounter=50;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }		
 #endif		
#endif
		
        case IRKEY_POWER:
        {
            //printMsg("POWER KEY ");
 	     VKeyPad=VK_POWER;		
            break;
        }
    }
}

#else
{
    switch(g_ucIrCode)
    {
        case IRKEY_0:
        case IRKEY_1:
        case IRKEY_2:
        case IRKEY_3:
        case IRKEY_4:
        case IRKEY_5:
        case IRKEY_6:
        case IRKEY_7:
        case IRKEY_8:
        case IRKEY_9:
        case IRKEY_100:
        {
            ucTmp = IRKeyToNumber(g_ucIrCode);
            printData("IRKEY TO NUM = %d", ucTmp);
            break;
        }

#if (defined(Changjia_Project))
        case IRKEY_HOME:
 #endif
        case IRKEY_OSD_MENU:			
        {
            //printMsg("MENU KEY ");
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag))
            {
             if(g_u8MenuPageIndex == FACTORY_MENU)
             {
              if(g_u8MenuItemIndex==FMI_Item_Exit)
               FactoryExecuteKeyEvent(MIA_PREV_ITEM);
		else	  
               FactoryExecuteKeyEvent(MIA_GOTO_ADJUST_ITEM);
             }
              else if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                {
           #if LHCX_IR_Function_Enable
		   ExecuteKeyEvent(MIA_GOTO_MainMenu);	
	    #else
                 ExecuteKeyEvent(MIA_GOTO_NEXT);
           #endif		
                 ResetOsdTimer();
                }
                ucRepeatFlag |= _BIT0;		 
           }
            break;
        }

        case IRKEY_OSD_UP:
        {
            //printMsg("VOL UP KEY ");

       if(IRkeyDelayCounter==0)
         {
	           VKeyPad=VK_UP;	
   		   if(LR_UpDown_SwitchFlag)	   
                  IRkeyDelayCounter=45;  
		   else
                   IRkeyDelayCounter=18;  //20x10ms
       }
            ucRepeatFlag |= _BIT1;
            
            break;
        }

        case IRKEY_OSD_DOWM:
        {
           // printMsg("VOL DOWN KEY ");

        if(IRkeyDelayCounter==0)
        {
   	        VKeyPad=VK_DOWN;		
            if(LR_UpDown_SwitchFlag)	   
               IRkeyDelayCounter=43;  
            else
               IRkeyDelayCounter=18;  //20x10ms
         }
          break;
        }

        case IRKEY_OSD_LEFT:
        {
              
            //printMsg("CH LEFT KEY ");

            if(IRkeyDelayCounter==0)
             {
                   IRkeyDelayCounter=18;  //20x10ms
   	             VKeyPad=VK_DOWN;	                  
		}
            
 	     //VKeyPad=VK_LEFT;	
             //ucRepeatFlag |= _BIT0;	
            	 
           break;
        }

        case IRKEY_OSD_RIGHT:
        {
            //printMsg("CH RIGHT KEY ");

              if(IRkeyDelayCounter==0)
                {
                   IRkeyDelayCounter=18;  //20x10ms
   	             VKeyPad=VK_UP;	                  
             }
                
            //ucRepeatFlag |= _BIT0;
           	 
           break;
        }

        case IRKEY_OSD_EXIT:
       {
           // printMsg("CH EXIT KEY ");
         if(g_u8MenuPageIndex == FACTORY_MENU)
         {
 	     VKeyPad=VK_EXIT;		
         }
        else if (g_u8MenuPageIndex != MENU_ROOT)
        {
            //ExecuteKeyEvent(MIA_GotoPrev);
      #if Menu_Key_Use_For_Enter
 	     VKeyPad=VK_EXIT;		
      #else	  
 	     VKeyPad=VK_MENU;		
      #endif	  
        }

            ucRepeatFlag |= _BIT0;		 
           break;
        }		
				
        case IRKEY_OSD_Enter:
        {
            //printMsg("CH Enter KEY ");
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)/*&&(!FACTORY_MODE_FLAG)*/)
            {
              if(g_u8MenuPageIndex == FACTORY_MENU)
              {
   	        VKeyPad=VK_MENU;		
              }
              else 
  	     if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		 )
                 ExecuteKeyEvent(MIA_NOTHING);
  	     else		
  	     {
  	    #if Menu_Key_Use_For_Enter    
   	        VKeyPad=VK_MENU;		
	    #else	
   	        VKeyPad=VK_EXIT;		
	    #endif	
  	     }
               ucRepeatFlag |= _BIT0;		 
           }
           break;
        }		
#if 1
        case IRKEY_INPUT:
        {
            //printMsg("CH IRKEY_INPUT ");
	   if(IRkeyDelayCounter==0)
	   {
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_ROOT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		||g_u8MenuPageIndex == MENU_STANDBY
		||g_u8MenuPageIndex == MENU_ENTERPOWERSAVING
		)
                {
                  ExecuteKeyEvent(MIA_HotInput);
                  ResetOsdTimer();
                }
               IRkeyDelayCounter=50;  //50x10ms
               ucRepeatFlag |= _BIT0;		 
            }
           } 
        }
        break;
		
        case IRKEY_VOL_UP:
        {
            //printMsg("VOL UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_VOL_UP;		
           // if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
       #if ENABLE_AUDIO_SETTINGS_MENU     
              if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	#endif			
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
            ucRepeatFlag |= _BIT0;
	   }
           break;
        }
		
        case IRKEY_VOL_DOWM:
        {
            //printMsg("VOL DOWN  KEY ");
	    if(IRkeyDelayCounter==0)
	    {
 	     VKeyPad=VK_VOL_DOWN;	
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
         #if ENABLE_AUDIO_SETTINGS_MENU   
            if (g_u8MenuPageIndex == MENU_HOTVOLUME)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	 #endif			
	        {
	            ExecuteKeyEvent(MIA_HotVolume);
	        }
                     ResetOsdTimer();
             IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
		 
           break;
        }
        case IRKEY_CH_UP:
        {
           // printMsg("BRI UP  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_UP;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_INC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                  ResetOsdTimer();
                  IRkeyDelayCounter=18;  //50x10ms
            }
         }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_CH_DOWM:
        {
            //printMsg("BRI DOWN  KEY ");
	   if(IRkeyDelayCounter==0)
	   {
 	     VKeyPad=VK_BRI_DOWN;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {

            if (g_u8MenuPageIndex == MENU_HOTBRIGHTNESS)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            else
	        {
	            ExecuteKeyEvent(MIA_HotBrightness);
	        }
                 ResetOsdTimer();
                IRkeyDelayCounter=18;  //50x10ms
            }
           }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_MUTE:
        {
           // printMsg("IRKEY_MUTE");
	 if(IRkeyDelayCounter==0)
	 {
 	     VKeyPad=VK_REPEAT;		
            //if ((!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)
        #if ENABLE_AUDIO_SETTINGS_MENU
		||(g_u8MenuPageIndex == MENU_HOTMUTE)
	 #endif		   
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
               {
	            ExecuteKeyEvent(MIA_HotMuteOnOff);
               }
                 ResetOsdTimer();
            }
                IRkeyDelayCounter=50;  //50x10ms
	   }
            ucRepeatFlag |= _BIT1;
           break;
        }

        case IRKEY_ZOOM:
        {
           // printMsg("IRKEY_ZOOM");
	   if((IRkeyDelayCounter==0)
         #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	   &&(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
         #endif
         )
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTPICTUREFORMAT)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotPictureFormat);
            }
                IRkeyDelayCounter=100;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }
        
        case IRKEY_PIP:
        {
            //printMsg("IRKEY_PIP");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PXP);
             }
                 IRkeyDelayCounter=50;  //50x10ms
           }
            ucRepeatFlag |= _BIT0;
           break;
        }
		
        case IRKEY_PIP_Swap:
       {
            //printMsg("IRKEY_PIP_Swap ");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
//            if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMultiView_PIP_Swap);
            }
                   IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_AudioSwap:
        {
            //printMsg("IRKEY_PIP_AudioSwap");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_AudioSwap);
            }
                IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }

        case IRKEY_PIP_Position:
       {
            //printMsg("IRKEY_PIP_Position");
	    if(IRkeyDelayCounter==0)		
	    {
 	     VKeyPad=VK_NOTHING;		
            //if ((g_u8MenuPageIndex == MENU_ROOT)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            if((!FACTORY_MODE_FLAG)&&(g_ScalerInfo[DISPLAY_WIN1].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN2].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN3].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK||g_ScalerInfo[DISPLAY_WIN4].eSiDispStatus == SC_STATUS_OUTPUT_SETUPOK))
            {
	            ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
             }
                  IRkeyDelayCounter=50;  //50x10ms
	    }
            ucRepeatFlag |= _BIT0;
           break;
        }
#if SleepMode_Enable		
        case IRKEY_SLEEP:
        {
            printMsg("IRKEY_SLEEP");
	   if(IRkeyDelayCounter==0)
	  {
 	     VKeyPad=VK_NOTHING;		
            if ((g_u8MenuPageIndex == MENU_ROOT||g_u8MenuPageIndex == MENU_SLEEPMODE
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
			)&&(!SyncLossState(USER_PREF_INPUT_TYPE))&&(!(UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))&&(InputTimingStable_NormalFlag)&&(!FACTORY_MODE_FLAG))
            {
               if((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_SLEEPMODE)
	 #if SleepMode_Enable		  	
		||g_u8MenuPageIndex == MENU_POWEROFFMSG
        #endif
		)
	            ExecuteKeyEvent(MIA_HotSleepMode);
            }
                IRkeyDelayCounter=50;  //50x10ms
                 ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }		
 #endif		

#if (defined(KunDe_Project))		
        case IRKEY_HotBrightness:
        {
            printMsg("IRKEY_HotBrightness");
	  if((IRkeyDelayCounter==0)&&(g_u8MenuPageIndex == MENU_ROOT))
	  {
 	      VKeyPad=VK_NOTHING;		
	       ExecuteKeyEvent(MIA_HotBrightness);
              IRkeyDelayCounter=50;  //50x10ms
              ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }	
        case IRKEY_HotVolume:
        {
            printMsg("IRKEY_HotVolume");
	  if((IRkeyDelayCounter==0)&&(g_u8MenuPageIndex == MENU_ROOT))
	  {
 	      VKeyPad=VK_NOTHING;		
	       ExecuteKeyEvent(MIA_HotVolume);
              IRkeyDelayCounter=50;  //50x10ms
              ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }	
        case IRKEY_HotKey1:
        {
            printMsg("IRKEY_HotKey1");
	  if((IRkeyDelayCounter==0)&&((g_u8MenuPageIndex == MENU_ROOT)
      #if ENABLE_AUDIO_SETTINGS_MENU
	  ||(g_u8MenuPageIndex == MENU_HOTMUTE)
      #endif
	  ))
	  {
 	      VKeyPad=VK_NOTHING;		
	       ExecuteKeyEvent(MIA_HotMuteOnOff);
              IRkeyDelayCounter=50;  //50x10ms
              ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }	
        case IRKEY_HotKey2:
        {
            printMsg("IRKEY_HotKey2");
	  if((IRkeyDelayCounter==0)&&((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTDCR)))
	  {
 	      VKeyPad=VK_NOTHING;		
	       ExecuteKeyEvent(MIA_HotDCR);
              IRkeyDelayCounter=50;  //50x10ms
              ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }	

        case IRKEY_HotKey3:
        {
            printMsg("IRKEY_HotKey2");
	  if((IRkeyDelayCounter==0)&&((g_u8MenuPageIndex == MENU_ROOT)))
	  {
 	      VKeyPad=VK_NOTHING;		
	       ExecuteKeyEvent(MIA_HotInput);
              IRkeyDelayCounter=50;  //50x10ms
              ResetOsdTimer();
	  }
	  else if((IRkeyDelayCounter==0)&&((g_u8MenuPageIndex == MENU_HOTINPUT)))
	  {
 	      VKeyPad=VK_NOTHING;		
	       ExecuteKeyEvent(MIA_EXEC_AND_GOTO_NEXT);
              IRkeyDelayCounter=50;  //50x10ms
              ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }	

        case IRKEY_HotKey4:
        {
            printMsg("IRKEY_HotKey2");
	  if((IRkeyDelayCounter==0)&&((g_u8MenuPageIndex == MENU_ROOT)||(g_u8MenuPageIndex == MENU_HOTPICTUREMODE)))
	  {
 	      VKeyPad=VK_NOTHING;		
	       ExecuteKeyEvent(MIA_HotPictureMode);
              IRkeyDelayCounter=50;  //50x10ms
              ResetOsdTimer();
	  }
            ucRepeatFlag |= _BIT0;
           break;
        }	
		
 #endif		

 
#endif
		
        case IRKEY_POWER:
        {
            //printMsg("POWER KEY ");
 	     VKeyPad=VK_POWER;		
            break;
        }
    }
}
#endif

    if(ucRepeatFlag&_BIT0)
    {
        irDecodeRepaeat(IR_DELAY_TIME0);
    }
    else if(ucRepeatFlag&_BIT1)
    {
        irDecodeRepaeat(IR_DELAY_TIME1);
    }
    else
    {
        irDecodeEnd();
    }
}
#else
void irDecodeCommand(void)
{
    BOOL bProcessDefaultIREvent = TRUE;
    BYTE ucTmp;
    BYTE ucRepeatFlag = 0;

    if (g_bIrDetect)
    {
        g_bIrDetect = 0;
        if (g_bIRRepeat) // check repeat status
        {
        #if(!(IR_MODE_SEL==IR_RAW_DATA_POL_MODE ||IR_MODE_SEL==IR_RAW_DATA_INT_MODE))
            if (g_bIRRepeatTimeOut) // check
                g_bIRRepeatTimeOut = 0;
            else
        #endif
                return;
        }
    }
    else
        return;
    IR_PRINT("\r\nir=%x ", g_ucIrCode);
#if ENABLE_CEC
    if(MsAPI_CecIrdaKeyProcess(E_CEC_CONTROLLER_0, g_ucIrCode))   return;
#endif
#if ENABLE_CEC2
    if(MsAPI_CecIrdaKeyProcess(E_CEC_CONTROLLER_1, g_ucIrCode))   return;
#endif
    if ((!g_SysSetting.PowerOn) && (g_ucIrCode != IRKEY_POWER))
        return;

    ucTmp = IRKeyToNumber( g_ucIrCode );





    if( !bProcessDefaultIREvent ) // Don't process default IR event
    {
        irDecodeEnd();
        return;
    }

	irPullFactoryKey( g_ucIrCode);
    switch (g_ucIrCode)
    {
        case IRKEY_0:
        case IRKEY_1:
        case IRKEY_2:
        case IRKEY_3:
        case IRKEY_4:
        case IRKEY_5:
        case IRKEY_6:
        case IRKEY_7:
        case IRKEY_8:
        case IRKEY_9:
		    if(!g_SysSetting.PowerOn)//POWER ON
			{
			    ExecuteKeyEvent(MIA_POWER);
                break;
            }
            #if (OSD_STYLE==OSD_DEMO)
            if(g_ucMenuPageIndex==MENU_SOURCE)
			    break;
            #endif
            if (IsTVInUse())
            {
                    irAdjustNumKey(IRKeyToNumber(g_ucIrCode));
                break;
            }
            break;
        case IRKEY_100:
            if (g_bIrNumKeyStart == 0)
            {
                g_bIr100Key = 1;
                g_ucIrNumKey = 0; // clear IR key

                g_bIrMenuControl = 1;
                g_ucIrCodeMenuAction = IR_MENU_CHANNEL1;
                {
                g_IrCodeParam[0] = '1';
                g_IrCodeParam[1] = '-';
                g_IrCodeParam[2] = '-';
                g_IrCodeParam[3] = '\0';
                }
                g_ucMenuPageIndex = MENU_INPUT_MESSAGE;
                g_ucMenuItemIndex = 0;
                msOsdHide();
                DrawOsdMenu();
                g_bIrMenuControl = 0;

                g_wIrNumKeyTimer = IR_NUMKEY_DELAY_TIME1;
            }
            break;
        case IRKEY_OSD_MENU:
            MenuProcessKey(BTN_OSD);
            break;
        case IRKEY_OSD_LEFT:
            //if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ) )
            {
                {
                    MenuProcessKey(BTN_LEFT);
                    ucRepeatFlag |= _BIT1;
                }
            }
            break;
        case IRKEY_OSD_RIGHT:
            //if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ) )
            {
                {
                    MenuProcessKey(BTN_RIGHT);
                    ucRepeatFlag |= _BIT1;
                }
            }
            break;
        case IRKEY_OSD_UP:
            //if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ) )
            {
                MenuProcessKey(BTN_UP);
                ucRepeatFlag |= _BIT0;
            }
            break;
        case IRKEY_OSD_DOWM:
            //if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ) )
            {
                MenuProcessKey(BTN_DOWN);
                ucRepeatFlag |= _BIT0;
            }
            break;
        case IRKEY_POWER:
            MenuProcessKey(BTN_POWER);
            break;
        case IRKEY_INPUT:
            /* <Janus>2009/10/15: To clarify original logic */
            if( g_SysSetting.PowerOn )    /* power has already been turned on */
            {
                MenuProcessKey(BTN_SOURCE); /* change source */
            }
            else    /* if power is not on */
            {
                ExecuteKeyEvent(MIA_POWER); /* turn on power */
            }
            break;
    #if( ENABLE_SUB_WIN )
        case IRKEY_PIP:
            MenuProcessKey(BTN_PIP);
            break;
    #endif
        case IRKEY_VOL_UP:

            if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ))
                MenuProcessKey(BTN_RIGHT);
            #if (OSD_STYLE==OSD_DEMO)
            else if (g_ucMenuPageIndex == MENU_VOLUME)
                ExecuteKeyEvent(MIA_INC_VALUE);
            #endif
            else
	        {
	            ExecuteKeyEvent(MIA_VOLUME);
				ExecuteKeyEvent(MIA_INC_VALUE);
	        }
            ucRepeatFlag |= _BIT1;
            break;
        case IRKEY_VOL_DOWM:
            if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ))
                MenuProcessKey(BTN_LEFT);
            #if (OSD_STYLE==OSD_DEMO)
            else if (g_ucMenuPageIndex == MENU_VOLUME)
                ExecuteKeyEvent(MIA_DEC_VALUE);
            #endif
            else
        	{
	            ExecuteKeyEvent(MIA_VOLUME);
				ExecuteKeyEvent(MIA_DEC_VALUE);
        	}
            ucRepeatFlag |= _BIT1;
            break;
        case IRKEY_CH_UP:
            if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ) )
            {
                MenuProcessKey(BTN_UP);
                ucRepeatFlag |= _BIT0;
                break;
            }
                ExecuteKeyEvent(MIA_TVCHINC);
            ucRepeatFlag |= _BIT0;
            break;
        case IRKEY_CH_DOWM:
            if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ) )
            {
                MenuProcessKey(BTN_DOWN);
                ucRepeatFlag |= _BIT0;
                break;
            }
                ExecuteKeyEvent(MIA_TVCHDEC);
            ucRepeatFlag |= _BIT0;
            break;
        case IRKEY_DISPLAY:
            if (g_ucMenuPageIndex == MENU_ROOT ||
                #if (OSD_STYLE==OSD_DEMO)
                g_ucMenuPageIndex == MENU_PICTURE_MODE ||
                g_ucMenuPageIndex == MENU_SCALER_MODE ||
                g_ucMenuPageIndex == MENU_SLEEP_MODE ||
                #endif
                g_ucMenuPageIndex == MENU_INPUT_MESSAGE)
                ExecuteKeyEvent(MIA_INFO);
            break;
        case IRKEY_MUTE:
            ExecuteKeyEvent(MIA_MUTE);
            break;
        case IRKEY_PICMODE:

            ExecuteKeyEvent(MIA_PICTURE_MODE);

            break;

        case IRKEY_ZOOM:

            ExecuteKeyEvent(MIA_ZOOM);

            break;
        case IRKEY_SLEEP:
            if (g_ucMenuPageIndex == MENU_ROOT ||
                #if (OSD_STYLE==OSD_DEMO)
                g_ucMenuPageIndex == MENU_PICTURE_MODE ||
                g_ucMenuPageIndex == MENU_SCALER_MODE ||
                g_ucMenuPageIndex == MENU_SLEEP_MODE ||
                #endif
                g_ucMenuPageIndex == MENU_INPUT_MESSAGE)
            ExecuteKeyEvent(MIA_SLEEP);
            break;
        case IRKEY_MTS:
            if (g_ucMenuPageIndex == MENU_ROOT ||
                #if (OSD_STYLE==OSD_DEMO)
                g_ucMenuPageIndex == MENU_PICTURE_MODE ||
                g_ucMenuPageIndex == MENU_SCALER_MODE ||
                g_ucMenuPageIndex == MENU_SLEEP_MODE ||
                #endif
                g_ucMenuPageIndex == MENU_INPUT_MESSAGE)
            ExecuteKeyEvent(MIA_MTS);
            break;



    } // End of switch

    if(ucRepeatFlag&_BIT0)
    {
        irDecodeRepaeat(IR_DELAY_TIME0);
    }
    else if(ucRepeatFlag&_BIT1)
    {
        irDecodeRepaeat(IR_DELAY_TIME1);
    }
    else
    {
        irDecodeEnd();
    }
}

/////////////////////////////////////////////////////
void irAdjustNumKey(BYTE ucPressNum)
{
    if (g_bIrNumKeyStart == 0)
    {
        g_ucIrNumKey = ucPressNum;
        g_bIrNumKeyStart = 1;
        g_bIrKeyNumEnd = 0;

        g_bIrMenuControl = 1;
        g_ucIrCodeMenuAction = IR_MENU_CHANNEL1;
		{
        	if (g_bIr100Key)
            	g_IrCodeParam[0] = OSD_0_INDEX + 1;
        	else
            	g_IrCodeParam[0] = ' ';
        	g_IrCodeParam[1] = ucPressNum + OSD_0_INDEX;
        	g_IrCodeParam[2] = '-';
        	g_IrCodeParam[3] = '\0';
		}
        g_ucMenuPageIndex = MENU_INPUT_MESSAGE;
        g_ucMenuItemIndex = 0;
        msOsdHide();
        DrawOsdMenu();
        g_bIrMenuControl = 0;

        g_wIrNumKeyTimer = IR_NUMKEY_DELAY_TIME1;
    }
    else
    {
        g_ucIrNumKey = (g_ucIrNumKey * 10) + ucPressNum; // calculate key number

        if (g_bIr100Key)
        {
            g_bIr100Key = 0;
            g_ucIrNumKey += 100;
        }
        g_bIrMenuControl = 1;
        g_ucIrCodeMenuAction = IR_MENU_CHANNEL2;
        g_ucMenuPageIndex = MENU_INPUT_MESSAGE;
        g_ucMenuItemIndex = 0;
        msOsdHide();
        DrawOsdMenu();
        g_bIrMenuControl = 0;

        g_wIrNumKeyTimer = IR_NUMKEY_DELAY_TIME2;
        g_bIrNumKeyStart = 0;
        g_bIrKeyNumEnd = 1; // ready to change channel
    }
}
///////////////////////////////////////////////////////////////
//press IRKEY_1 ->IRKEY_2 ->IRKEY_3 ->IRKEY_4 to enter menu factory
//bit0      bit1       bit2      bit3
void irPullFactoryKey(BYTE ucPressKey)
{
	if(g_ucFactoryFlag&0x80)
	   return;

	switch(ucPressKey)
    {
		case IRKEY_1:
            if((g_ucFactoryFlag&0x0F) ==0)
                g_ucFactoryFlag |=_BIT0;
            else if((g_ucFactoryFlag&0X0F) ==_BIT0)
                break;//Not care repeat
            else
                g_ucFactoryFlag &=0XF0;
            break;
		case IRKEY_2:
            if((g_ucFactoryFlag&0X0F) ==(_BIT0))
                g_ucFactoryFlag |=_BIT1;
            else if((g_ucFactoryFlag&0X0F) ==(_BIT0|_BIT1))
                break;//Not care repeat
            else
                g_ucFactoryFlag &=0XF0;
            break;
		case IRKEY_3:
            if((g_ucFactoryFlag&0X0F) ==(_BIT0|_BIT1))
            {
				g_ucFactoryFlag |=_BIT2;
		    }
            else if((g_ucFactoryFlag &0X0F)==(_BIT0|_BIT1|_BIT2))
                break;//Not care repeat
            else
                g_ucFactoryFlag &=0XF0;
            break;
		case IRKEY_4:
		    if((g_ucFactoryFlag&0X0F) ==(_BIT0|_BIT1|_BIT2))
                g_ucFactoryFlag |=_BIT3;
            else
                g_ucFactoryFlag &=0XF0;
			if((g_ucFactoryFlag&0x0F)==0x0F)
			    ExecuteKeyEvent(MIA_ENTERFACTORY);
		    break;
        default:
		    g_ucFactoryFlag&=0XF0;
            break;
    }
	//IR_PRINT("g_ucFactoryFlag=%x ", g_ucFactoryFlag);
}
#endif

/////////////////////////////////////////////////////
void irDecodeNumKey(void)
{
    if (g_bIrKeyNumEnd)
    {
        g_bIrKeyNumEnd = 0;
        g_bIr100Key = 0;
    } // if IR key end
}
#endif  //(IR_REMOTE_SEL != NO_REMOTE)
