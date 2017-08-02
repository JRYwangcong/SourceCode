///////////////////////////////////////////////////////////////////////////////
/// @file Keypad.c
/// @brief Keypad function.
/// @author MStarSemi Inc.
///
/// Keypad handler function.
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#include <math.h>    //MST_MCU
#include "datatype.h"
#include "Board.h"
#include "Keypaddef.h"
#include "ms_reg.h"
#include "Global.h"
#include "MDebug.h"
#include "Common.h"
#include "misc.h"
#include "Ms_rwreg.h"
#include "Mcu.h"
#include "menudef.h"
#include "GPIO_DEF.h"

#include "KEY_DEFINE_COMMON.h"

#include "Menu.h"
#include "NVRam.h"
#include "MsIR.h"

#define KEYPAD_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG&&KEYPAD_DEBUG
    #define KEYPAD_printData(str, value)   printData(str, value)
    #define KEYPAD_printMsg(str)           printMsg(str)
#else
    #define KEYPAD_printData(str, value)
    #define KEYPAD_printMsg(str)
#endif
BYTE xdata g_u8KeyDebounceCounter=0;
//BYTE xdata LastKeypadButton=0;
BYTE xdata g_u8KeypadButton=0;

BYTE xdata g_bIRTimeOut=0;
BYTE xdata g_u8KeypadCoumter=0;

extern BYTE xdata g_u8MenuPageIndex;

#ifndef SARADC_MASK
#define SARADC_MASK 0xFF
#endif

BYTE Key_GetKeypadStatus( void )
{
//SAR_KEY
#if SARKEY_EN
    BYTE u8Keypad = 0xFF;
    BYTE u8Temp = 0;
    BYTE u8Temp1 = 0;
    BYTE retry_Key=3;

#if Special_One_SAR_Key_Func
   #if (defined(TaiBei_Project))
	u8Temp = KEYPAD_ADC_A  & SARADC_MASK; //  SAR0

	while(retry_Key&&(u8Temp<(SARADC_MASK-5)))
	{
		u8Temp = KEYPAD_ADC_A  & SARADC_MASK; //  SAR0
		Delay1ms(15);
		u8Temp1 =KEYPAD_ADC_A  & SARADC_MASK;
		if(abs(u8Temp-u8Temp1)<5)
			break;
		retry_Key--;
	}
   // u8Temp = KEYPAD_ADC_A  & SARADC_MASK;


    if (abs(u8Temp - 0)  <= 4)
        u8Keypad &= ~KEY_POWER;

    if (abs(u8Temp - 0x0B)  <= 6)
        u8Keypad &= ~KEY_PLUS;

    if (abs(u8Temp - 0x19)  <= 6)
        u8Keypad &= ~KEY_MINUS;

    if (abs(u8Temp - 0x2C)  <= 6)
        u8Keypad &= ~KEY_DECVALUE;

    if (abs(u8Temp - 0x48)  <= 6)
        u8Keypad &= ~KEY_INCVALUE;

    if (abs(u8Temp - 0x72)  <= 6)
        u8Keypad &= ~KEY_EXIT;
	
    if (abs(u8Temp - 0xB8)  <= 6)
        u8Keypad &= ~KEY_MENU;
 #elif (defined(RunFeng_Project))
	u8Temp = KEYPAD_ADC_A  & SARADC_MASK; //  SAR0

	while(retry_Key&&(u8Temp<(SARADC_MASK-5)))
	{
		u8Temp = KEYPAD_ADC_A  & SARADC_MASK; //  SAR0
		Delay1ms(15);
		u8Temp1 =KEYPAD_ADC_A  & SARADC_MASK;
		if(abs(u8Temp-u8Temp1)<5)
			break;
		retry_Key--;
	}
   // u8Temp = KEYPAD_ADC_A  & SARADC_MASK;


    if (abs(u8Temp - 0x2C)  <= 8)
        u8Keypad &= ~KEY_MINUS;

    if (abs(u8Temp - 0x56)  <= 8)
        u8Keypad &= ~KEY_PLUS;

    if (abs(u8Temp - 0x7B)  <= 8)
        u8Keypad &= ~KEY_MENU;

    if (abs(u8Temp - 0x9D)  <= 8)
        u8Keypad &= ~KEY_EXIT;
	
    if (abs(u8Temp - 0xE0)  <= 8)
        u8Keypad &= ~KEY_POWER;
 #endif

#elif ((defined(RunFeng_Project))&&(PanelType==PanelM350DVR01_01||PanelType==PanelT430QVN01))

	u8Temp = KEYPAD_ADC_A  & SARADC_MASK; //  SAR0
       retry_Key=5;

	while(retry_Key&&(u8Temp<(SARADC_MASK-5)))
	{
		u8Temp = KEYPAD_ADC_A  & SARADC_MASK; //  SAR0
		Delay1ms(8);
		u8Temp1 =KEYPAD_ADC_A  & SARADC_MASK;
		if(abs(u8Temp-u8Temp1)<5)
			break;
		retry_Key--;
	}
   // u8Temp = KEYPAD_ADC_A  & SARADC_MASK;


    if (abs(u8Temp - ADCKEY_A0_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_A0;

    if (abs(u8Temp - ADCKEY_A1_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_A1;

    if (abs(u8Temp - ADCKEY_A2_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_A2;

       retry_Key=5;
	u8Temp = KEYPAD_ADC_B  & SARADC_MASK; //  SAR0
	
	while(retry_Key&&(u8Temp<(SARADC_MASK-5)))
	{
		u8Temp = KEYPAD_ADC_B  & SARADC_MASK; //  SAR1
		Delay1ms(8);
		u8Temp1 =KEYPAD_ADC_B  & SARADC_MASK;
		if(abs(u8Temp-u8Temp1)<5)
			break;
		retry_Key--;
	}
    //u8Temp1 = KEYPAD_ADC_B  & SARADC_MASK;


    if (abs(u8Temp - ADCKEY_B0_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_B0;

    if (abs(u8Temp - ADCKEY_B1_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_B1;
	
    if (abs(u8Temp - ADCKEY_B2_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_B2;

    if (HW_GPIO_KEY == 0)
         u8Keypad &= ~KEY_EXIT;

#else
	u8Temp = KEYPAD_ADC_A  & SARADC_MASK; //  SAR0

	while(retry_Key&&(u8Temp<(SARADC_MASK-5)))
	{
		u8Temp = KEYPAD_ADC_A  & SARADC_MASK; //  SAR0
		Delay1ms(15);
		u8Temp1 =KEYPAD_ADC_A  & SARADC_MASK;
		if(abs(u8Temp-u8Temp1)<5)
			break;
		retry_Key--;
	}
   // u8Temp = KEYPAD_ADC_A  & SARADC_MASK;


    if (abs(u8Temp - ADCKEY_A0_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_A0;

    if (abs(u8Temp - ADCKEY_A1_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_A1;

    if (abs(u8Temp - ADCKEY_A2_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_A2;

       retry_Key=3;
	u8Temp = KEYPAD_ADC_B  & SARADC_MASK; //  SAR0
	
	while(retry_Key&&(u8Temp<(SARADC_MASK-5)))
	{
		u8Temp = KEYPAD_ADC_B  & SARADC_MASK; //  SAR1
		Delay1ms(15);
		u8Temp1 =KEYPAD_ADC_B  & SARADC_MASK;
		if(abs(u8Temp-u8Temp1)<5)
			break;
		retry_Key--;
	}
    //u8Temp1 = KEYPAD_ADC_B  & SARADC_MASK;


    if (abs(u8Temp - ADCKEY_B0_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_B0;

    if (abs(u8Temp - ADCKEY_B1_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_B1;
	
    if (abs(u8Temp - ADCKEY_B2_VALUE)  <= KEY_TOL)
        u8Keypad &= ~ADCKEY_B2;
  #endif
//GPIO_KEY
#else
    BYTE u8Keypad = 0xFF;

    if (HW_KEY_PLUS==0)
        u8Keypad &= ~KEY_PLUS;
    if (HW_KEY_MINUS==0)
        u8Keypad &= ~KEY_MINUS;
    if (HW_KEY_MENU==0)
        u8Keypad &= ~KEY_MENU;
    if (HW_KEY_TICK==0)
        u8Keypad &= ~KEY_EXIT;
#endif
    //POWER_KEY
    if (HW_POWER_KEY == 0)
         u8Keypad &= ~KEY_POWER;
    return u8Keypad;
}
#if 0 //move to APPSystem_CheckPowerStatus function
void CheckFactoryKeyStatus( void )
{
    CLR_FACTORY_MODE_FLAG();

    if (POWER_ON_FLAG && !FACTORY_MODE_FLAG)
    {
        if (BURNIN_MODE_FLAG)
            SET_FACTORY_MODE_FLAG();
        else
        {
#if (!ZUI)
            g_stMenuFlag.bDisplayLogo=1;
#endif
        }
    }
}
#endif
#if (!LHCX_EnterFactoryMode)
void CheckACKeyStatus( void )
{
    BYTE u8KeypadStatus=0;
    //wait REG_003A62 setting to stable
    #if 0//R2_51_DUAL_MODE
    u8KeypadStatus = R2_XSRAM_READ(0x5F80);
    #else
    u8KeypadStatus=(Key_GetKeypadStatus()^KEYPAD_MASK)&KEYPAD_MASK;
    #endif


    if (POWER_ON_FLAG)
    {
    #if (defined(KunDe_Project))
	if (u8KeypadStatus == KEY_DECVALUE)
    #elif ((defined(KTC_Project))&&(PanelType==PanelLSM315HP01))
	if (u8KeypadStatus == KEY_EXIT)
    #else	
	if (u8KeypadStatus == KEY_MENU)
    #endif		
        {
            SET_FACTORY_MODE_FLAG();
	     SET_BURNIN_MODE_FLAG();		
        }
	else
	{
            //CLR_FACTORY_MODE_FLAG();
	     //CLR_BURNIN_MODE_FLAG();		
	}
        SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));

    }




}
#endif

Bool HoldMenuKeyCheck(void)
{
#if ENABLE_MULTI_INPUT
    if (g_u8MenuPageIndex == MENU_INPUT)
        return FALSE;
    else
#endif
    if (g_stMenuFlag.bOsdTimeout)     //Jison 080828
        return FALSE;
    return TRUE;
}
#if POWER_KEY_FUNC
Bool HoldPowerKeyCheck(void)
{
    if (g_stMenuFlag.bOsdTimeout)   
        return FALSE;
    return TRUE;
}
#endif

#if ENABLE_MBX_SAR
void Key_ScanKeypad( void )
{
#if ENABLE_MSTV_UART_DEBUG
    if(VKeyPad != VK_NOTHING)
    {
        g_u8KeypadButton=BTN_NOTHING;
        if(VKeyPad==VK_SOURCE)
            u8MailButton=BTN_EXIT;
        else if(VKeyPad==VK_MENU)
            u8MailButton=BTN_MENU;
        else if(VKeyPad==VK_LEFT || VKeyPad==VK_UP)
            u8MailButton=BTN_PLUS;
        else if(VKeyPad==VK_RIGHT || VKeyPad==VK_DOWN)
            u8MailButton=BTN_MINUS;
        else if(VKeyPad==VK_POWER)
            u8MailButton=BTN_POWER;
        else if(VKeyPad==VK_PIP)
            u8MailButton=BTN_NOTHING;
        VKeyPad = 0;
    }
#endif

    if((u8MailButton != BTN_NOTHING)&&(u8MailButton!= 0xFF))
    {
        //printData("u8MailButton: 0x%x", u8MailButton);
        if(u8MailButton != BTN_REPEAT)
        {
            g_u8KeypadButton = u8MailButton;
        }
        else
        {
            g_u8KeypadButton = BTN_NOTHING;
        }
        KEYPAD_printData("g_u8KeypadButton: 0x%x", g_u8KeypadButton);

        u8MailButton = 0xFF;
    }
}
#else
void Key_ScanKeypad( void )
{
    BYTE u8KeypadStatus=0;

    if(!g_u8KeyDebounceCounter)
    {
        #if 0//R2_51_DUAL_MODE
        u8KeypadStatus = R2_XSRAM_READ(0x5F80);
        #else
        u8KeypadStatus=(Key_GetKeypadStatus()^KEYPAD_MASK)&KEYPAD_MASK;
        #endif


#if (IR_REMOTE_SEL != NO_REMOTE)
    #ifdef MST9U_FPGA
        u8KeypadStatus=((0xFF)^KEYPAD_MASK)&KEYPAD_MASK;
    #endif
        if(!u8KeypadStatus)
        {
            if(VKeyPad != VK_NOTHING)
            {
             //printData("VKeyPad9999==%d",VKeyPad);
			 
                if(((VKeyPad==VK_SOURCE)&&(g_u8MenuPageIndex == MENU_ROOT))||(VKeyPad==VK_EXIT))
                {
                    u8KeypadStatus=KEY_EXIT;
                }
                else if(VKeyPad==VK_MENU)
                    u8KeypadStatus=KEY_MENU;
                else if(VKeyPad==VK_UP)
                    u8KeypadStatus=KEY_PLUS;
                else if(VKeyPad==VK_DOWN)
                    u8KeypadStatus=KEY_MINUS;
		#if (SevenKey_Function)		
                 else if(VKeyPad==VK_RIGHT)
                    u8KeypadStatus=KEY_INCVALUE;
               else if(VKeyPad==VK_LEFT)
                    u8KeypadStatus=KEY_DECVALUE;
		#endif	   
                else if(VKeyPad==VK_POWER)
                    u8KeypadStatus=KEY_POWER;

		//if(!g_bIRRepeat)		
                //VKeyPad = 0;
                //LastKeypadButton=u8KeypadStatus;
            }

			
        }
   #if OSD_LOCK_Enable
	if((g_u8MenuPageIndex==MENU_ROOT||g_u8MenuPageIndex==MENU_MAIN||g_u8MenuPageIndex==MENU_INPUT_SOURCE||g_u8MenuPageIndex==MENU_HOT_MENU_LOCK)&&(VKeyPad == VK_MENU))//for IR OSD Lock 
	{
         g_bIRTimeOut++;
        if((g_bIRTimeOut>=10))
        {
           VKeyPad = 0;
         }
        }
	else
    #endif		
           VKeyPad = 0;
#endif


        if (u8KeypadStatus)
        {
            
     /*   
            if(LastKeypadButton!=u8KeypadStatus)
            {
                LastKeypadButton=u8KeypadStatus;
                g_u8KeypadButton=BTN_NOTHING;
                goto KeyEnd;
            }
    */
            if(!POWER_ON_FLAG)
            {
                if(u8KeypadStatus!=KEY_POWER)
                {
                    g_u8KeypadButton=BTN_NOTHING;
                    goto KeyEnd;
                }
            }

            //printf("u8KeypadStatus=%x\n",u8KeypadStatus);
            if (u8KeypadStatus==KEY_MINUS)
            {
                if(g_u8KeypadButton==BTN_REPEAT)
                    g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_MINUS;
                }

		  #if (LHCX_EnterFactoryMode&&(!(defined(RunFeng_Project))))		
                    if (!g_stMenuFlag.bHoldMenuKey && InputTimingStableFlag(SrcInputPortM/*//tmp*/) && !UnsupportedModeFlag(SrcInputPortM/*//tmp*/))
                    {
                        if(HoldMenuKeyCheck())
                        {
                            g_stMenuFlag.bHoldMenuKey=1;
                            g_u8HoldKeyCounter=HOT_DOWN_HOLD_TIME;
                        }
                    }
		 #endif			
				
            }
            else if (u8KeypadStatus==KEY_PLUS)
            {
                if(g_u8KeypadButton==BTN_REPEAT)
                    g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_PLUS;
                }
		  #if (LHCX_EnterFactoryMode&&(defined(RunFeng_Project)))	
                    if (!g_stMenuFlag.bHoldMenuKey && InputTimingStableFlag(SrcInputPortM/*//tmp*/) && !UnsupportedModeFlag(SrcInputPortM/*//tmp*/))
                    {
                        if(HoldMenuKeyCheck())
                        {
                            g_stMenuFlag.bHoldMenuKey=1;
                            g_u8HoldKeyCounter=HOT_DOWN_HOLD_TIME;
                        }
                    }
		 #endif			
				
            }
            else if (u8KeypadStatus==KEY_INCVALUE)
            {
                if(g_u8KeypadButton==BTN_REPEAT)
                    g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_INCVALUE;
                }
            }
            else if (u8KeypadStatus==KEY_DECVALUE)
            {
                if(g_u8KeypadButton==BTN_REPEAT)
                    g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_DECVALUE;
                }
            }
            else if (u8KeypadStatus==KEY_MENU)
            {
                 //printMsg("Press Menu Key");
                if (g_u8KeypadButton==BTN_REPEAT
           #if 1//(!defined(KTC_Project))
             ||g_u8KeypadButton==BTN_MENU
	    #endif	   
		 )
                {
                    g_u8KeypadButton=BTN_REPEAT;
		  #if OSD_LOCK_Enable			
                    if (!g_stMenuFlag.bHoldMenuKey && InputTimingStableFlag(SrcInputPortM/*//tmp*/) && !UnsupportedModeFlag(SrcInputPortM/*//tmp*/))
                    {
                        if(HoldMenuKeyCheck())
                        {
                            g_stMenuFlag.bHoldMenuKey=1;
                            g_u8HoldKeyCounter=HOT_MENU_LOCK_HOLD_TIME;
                        }
                    }
		 #endif			
                }
                else
                    g_u8KeypadButton=BTN_MENU;
			//printData("g_u8KeypadButton==%d",g_u8KeypadButton);

		#if HongTian_MenuToFactoryMode
                    if (!g_stMenuFlag.bHoldMenuKey && g_u8MenuPageIndex==MENU_ROOT)
                    {
                        if(HoldMenuKeyCheck())
                        {
                            g_stMenuFlag.bHoldMenuKey=1;
                            g_u8HoldKeyCounter=HOT_DOWN_HOLD_TIME;
                        }
                    }
		 #endif

            }
            else if (u8KeypadStatus==KEY_EXIT)
            {
                if ( g_u8KeypadButton==BTN_REPEAT
              #if 1//(!defined(KTC_Project))
		  ||g_u8KeypadButton==BTN_EXIT	  
		#endif	  
		 )
                     g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_EXIT;
                }

		  #if ((defined(RunFeng_Project))&&(!Special_One_SAR_Key_Func))		
                    if ((!g_stMenuFlag.bHoldMenuKey) && (InputTimingStableFlag(SrcInputPortM)) && (!UnsupportedModeFlag(SrcInputPortM)))
                    {
                        if((HoldMenuKeyCheck()))
                        {
                            g_stMenuFlag.bHoldMenuKey=1;
                            g_u8HoldKeyCounter=HOT_DOWN_HOLD_TIME;
                        }
                    }
		 #endif			

				
            }
#if (!AIO_Board)			
            else if (u8KeypadStatus==KEY_POWER)
            {
                if (g_u8KeypadButton==BTN_POWER || g_u8KeypadButton==BTN_REPEAT)
                {
                    g_u8KeypadButton=BTN_REPEAT;
                }
                else
                {
                    g_u8KeypadButton=BTN_POWER;
                }

		#if POWER_KEY_FUNC			
                    if (!g_stMenuFlag.bHoldPowerKey )
                    {
                        if(HoldPowerKeyCheck())
                        {
                            g_stMenuFlag.bHoldPowerKey=1;
                            g_u8HoldKeyCounter=POWER_HOLD_TIME;
                        }
                    }
		#endif			
				
            }
#endif
            else if (u8KeypadStatus==KEY_DIAG && InputTimingStableFlag(SrcInputPortM/*//tmp*/))
            {
                if ((g_u8MenuPageIndex==MENU_CABLE_NOT_CONNECTED || g_u8MenuPageIndex==MENU_ROOT)
                    && !g_stMenuFlag.bHoldDiagPatternKey && !(HOT_OSD_LOCK_FLAG || OSD_LOCK_FLAG))
                {
                    g_stMenuFlag.bHoldDiagPatternKey=1;
                    g_u8HoldKeyCounter=2; //rename for menu key and diag key
                    KEYPAD_printMsg("Press Diag Key");
                }
                g_u8KeypadButton=BTN_REPEAT;
            }
            else
                g_u8KeypadButton=BTN_NOTHING;
        }
        else
        {
            #if HongTian_MenuToFactoryMode
            if (g_stMenuFlag.bHoldMenuKey && g_u8MenuPageIndex==MENU_ROOT)
            {
			g_stMenuFlag.bHoldMenuKey=0;
			g_u8HoldKeyCounter=0;	
			g_u8KeypadButton=BTN_MENU;
            }
	     else
	     #endif
            g_u8KeypadButton=BTN_NOTHING;
            //LastKeypadButton=u8KeypadStatus;
        }

        /// Released Menu Key before 10" timeout
    #if OSD_LOCK_Enable    
        if (g_stMenuFlag.bHoldMenuKey && g_u8KeypadButton==BTN_NOTHING)
        {
            g_stMenuFlag.bHoldMenuKey=0;
            g_u8HoldKeyCounter=0;
            KEYPAD_printMsg("Release Menu Key");
            //ResetOsdTimer();
        }
    #endif	
    #if LHCX_EnterFactoryMode
        if (g_stMenuFlag.bHoldMenuKey && g_u8KeypadButton==BTN_NOTHING)
        {
            g_stMenuFlag.bHoldMenuKey=0;
            g_u8HoldKeyCounter=0;
            KEYPAD_printMsg("Release Menu Key");
            //ResetOsdTimer();
        }
    #endif
	
    #if ((defined(RunFeng_Project))&&(!Special_One_SAR_Key_Func))
        if (g_stMenuFlag.bHoldMenuKey && (g_u8KeypadButton==BTN_NOTHING||g_u8KeypadButton==BTN_EXIT||g_u8KeypadButton==BTN_REPEAT))
        {
          if((g_u8KeypadButton==BTN_NOTHING)&&(g_u8MenuPageIndex != MENU_HOT_AUTO))
          {
            g_stMenuFlag.bHoldMenuKey=0;
            g_u8HoldKeyCounter=0;
	     g_u8KeypadButton=BTN_EXIT;
          }
	   else	  
	     g_u8KeypadButton=BTN_NOTHING;
            KEYPAD_printMsg("Release Exit Key");
            //ResetOsdTimer();
        }
    #endif	

    #if POWER_KEY_FUNC	
        if (g_stMenuFlag.bHoldPowerKey)
        {
          if(g_u8KeypadButton==BTN_POWER||g_u8KeypadButton==BTN_REPEAT)
          {
            if(g_u8MenuPageIndex<=MENU_ROOT)
              g_u8KeypadButton=BTN_NOTHING;
          }
          else //if(g_u8KeypadButton==BTN_NOTHING)
          {
            g_stMenuFlag.bHoldPowerKey=0;
            g_u8HoldKeyCounter=0;
          }
	   	
            KEYPAD_printMsg("Release Power Key");
            //ResetOsdTimer();
        }
    #endif		

    #if HongTian_MenuToFactoryMode
        if (g_stMenuFlag.bHoldMenuKey)
        {
          if(g_u8KeypadButton==BTN_MENU||g_u8KeypadButton==BTN_REPEAT)
          {
            if(g_u8MenuPageIndex<=MENU_ROOT)
              g_u8KeypadButton=BTN_NOTHING;
          }
          else //if(g_u8KeypadButton==BTN_NOTHING)
          {
            g_stMenuFlag.bHoldMenuKey=0;
            g_u8HoldKeyCounter=0;
          }
	   	
            KEYPAD_printMsg("Release MENU Key");
            //ResetOsdTimer();
        }
    #endif	
	
        if ((g_u8MenuPageIndex==MENU_CABLE_NOT_CONNECTED || g_u8MenuPageIndex==MENU_ROOT)
            && g_stMenuFlag.bHoldDiagPatternKey && g_u8KeypadButton==BTN_NOTHING)
        {
            g_stMenuFlag.bHoldDiagPatternKey=0;
            g_u8HoldKeyCounter=0; //White111004 rename for menu key and diag key
            KEYPAD_printMsg("Release Diag Key");
        }
    KeyEnd:
        g_u8KeyDebounceCounter=25;
    }
}
#endif
//================================================================
