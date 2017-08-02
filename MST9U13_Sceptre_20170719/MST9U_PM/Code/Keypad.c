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
#include "Types.h"
#include "Board.h"
#include "Keypaddef.h"
#include "ms_reg.h"
#include "Global.h"
//#include "MDebug.h"
#include "Common.h"
#include "misc.h"
#include "Ms_rwreg.h"
#include "Mcu.h"
//#include "menudef.h"
#include "GPIO_DEF.h"
#include "msAPI_MailBox.h"
#include "drvMBX.h"
#include "Buzzer.h"

//#include "Menu.h"

BYTE xdata g_u8KeyDebounceCounter=0;
//BYTE xdata LastKeypadButton=0;
BYTE xdata g_u8KeypadButton=0;
WORD xdata g_u32KeyCounter=0;
extern XDATA ePM_WakeupStatus ucWakeupStatus;

#if (R2_51_DUAL_MODE)
BYTE xdata g_u8KeypadPressData _at_ 0x5F80;
#endif

BYTE xdata g_u8KeypadPressData = 0;

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
        u8Keypad &= ~KEY_INCVALUE;//KEY_MINUS;

    if (abs(u8Temp - 0x19)  <= 6)
        u8Keypad &= ~KEY_DECVALUE;//KEY_PLUS;

    if (abs(u8Temp - 0x2C)  <= 6)
        u8Keypad &= ~KEY_PLUS;//KEY_DECVALUE;

    if (abs(u8Temp - 0x48)  <= 6)
        u8Keypad &= ~KEY_MINUS;//KEY_INCVALUE;

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
	
#elif ((defined(RunFeng_Project)))

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

    if (abs(u8Temp - 0x34)  <= KEY_TOL)
        u8Keypad &= ~(KEY_POWER|KEY_MENU);

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
    BYTE XDATA u8Keypad = 0xFF;

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

#if ENABLE_MBX_SAR
void Key_ScanKeypad( void )
{
    BYTE u8KeypadStatus=0;
    MBX_Msg mbox;
    MBX_Result stResult;

    if(!g_u8KeyDebounceCounter)
    {
        u8KeypadStatus=(Key_GetKeypadStatus()^KEYPAD_MASK)&KEYPAD_MASK;

#if 0//ENABLE_DEBUG
        if(!u8KeypadStatus)
        {
            if(VKeyPad != VK_NOTHING)
            {
                g_u8KeypadButton=BTN_NOTHING;
                if(VKeyPad==VK_SOURCE)
                    u8KeypadStatus=KEY_EXIT;
                else if(VKeyPad==VK_MENU)
                    u8KeypadStatus=KEY_MENU;
                else if(VKeyPad==VK_LEFT || VKeyPad==VK_UP)
                    u8KeypadStatus=KEY_PLUS;
                else if(VKeyPad==VK_RIGHT || VKeyPad==VK_DOWN)
                    u8KeypadStatus=KEY_MINUS;
                else if(VKeyPad==VK_POWER)
                    u8KeypadStatus=KEY_POWER;
                else if(VKeyPad==VK_PIP)
                    u8KeypadStatus=KEY_FACTORY;
                VKeyPad = 0;
                LastKeypadButton=u8KeypadStatus;
            }
        }
#endif
        if (u8KeypadStatus)
        {
            //printf("u8KeypadStatus=%x\n",u8KeypadStatus);
    /*        
            if(LastKeypadButton!=u8KeypadStatus)
            {
                LastKeypadButton=u8KeypadStatus;
                g_u8KeypadButton=BTN_NOTHING;
                goto KeyEnd;
            }
   */
            if (u8KeypadStatus==KEY_MINUS)
            {
                if(g_u8KeypadButton==BTN_MINUS)
                {
                    g_u8KeypadButton=BTN_REPEAT;
                    if(g_u32KeyCounter == 0)
                        g_u32KeyCounter = 75;
                }
                else if(g_u8KeypadButton==BTN_REPEAT)
                {
                    if(g_u32KeyCounter==0)
                        g_u8KeypadButton = BTN_MINUS;
                }
                else
                {
                    g_u8KeypadButton=BTN_MINUS;
                    g_u32KeyCounter = 0;
                }
            }
            else if (u8KeypadStatus==KEY_PLUS)
            {
                if(g_u8KeypadButton==BTN_PLUS)
                {
                    g_u8KeypadButton=BTN_REPEAT;
                    if(g_u32KeyCounter == 0)
                        g_u32KeyCounter = 75;
                }
                else if(g_u8KeypadButton==BTN_REPEAT)
                {
                    if(g_u32KeyCounter==0)
                        g_u8KeypadButton = BTN_PLUS;
                }
                else
                {
                    g_u8KeypadButton=BTN_PLUS;
                    g_u32KeyCounter = 0;
                }
            }
            else if (u8KeypadStatus==KEY_MENU)
            {
                if (g_u8KeypadButton==BTN_MENU||g_u8KeypadButton==BTN_REPEAT)
                {
                    g_u8KeypadButton=BTN_REPEAT;
                }
                else
                    g_u8KeypadButton=BTN_MENU;
            }

            else if (u8KeypadStatus==KEY_POWER)
            {
                if (g_u8KeypadButton==BTN_POWER || g_u8KeypadButton==BTN_REPEAT)
                    g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_POWER;
                    g_u32KeyCounter = 0;
                    if((sPMInfo.ucPMMode == ePM_POWER_DPMS) || (sPMInfo.ucPMMode == ePM_POWER_DCOFF))
                    {
                        sPMInfo.ePMState = ePM_EXIT_PM;
                    }
                }
            }
            else
            {
                g_u8KeypadButton=BTN_NOTHING;
                g_u32KeyCounter = 0;
            }

            if( (g_u32KeyCounter == 0) && (sPMInfo.ucPMMode == ePM_POWER_ON))
            {
                MailBox[0] = g_u8KeypadButton;
                mbox.eMsgType             = E_MBX_MSG_TYPE_NORMAL;
                mbox.u8MsgClass           = E_MBX_CLASS_SAR;
                mbox.u8Index              = MB_SAR_CMD_READ_DRAM;
                mbox.u8ParameterCount     = 0x04;
                mbox.u8Parameters[0]      = ((XDATA_TO_DRAM_ADDR+((WIN0_ADDR_START)<<10))>>24)&0xFF;
                mbox.u8Parameters[1]      = ((XDATA_TO_DRAM_ADDR+((WIN0_ADDR_START)<<10))>>16)&0xFF;
                mbox.u8Parameters[2]      = ((XDATA_TO_DRAM_ADDR+((WIN0_ADDR_START)<<10))>>8)&0xFF;
                mbox.u8Parameters[3]      = ((XDATA_TO_DRAM_ADDR+((WIN0_ADDR_START)<<10)))&0xFF;

                stResult = MApi_MBX_SendMsg(&mbox);
            }
        }
        else
        {
            g_u8KeypadButton=BTN_NOTHING;
            //LastKeypadButton=u8KeypadStatus;
        }

    KeyEnd:
        g_u8KeyDebounceCounter=25;
    }
}
#elif 1//R2_51_DUAL_MODE //R2_51_DUAL_MODE without using mailbox
void Key_ScanKeypad( void )
{
    g_u8KeypadPressData=(Key_GetKeypadStatus()^KEYPAD_MASK)&KEYPAD_MASK;
    if(g_u8KeypadPressData == KEY_POWER)
    {
        if((sPMInfo.ucPMMode == ePM_POWER_DPMS) || (sPMInfo.ucPMMode == ePM_POWER_DCOFF))
        {
            sPMInfo.ePMState = ePM_EXIT_PM;
	     ucWakeupStatus =ePMSTS_POWERGPIO_ACT;		
        }
    }
#if InputKeyWakeupEnable	
 #if (defined(FengYuan_Project)||(defined(KTC_Project))||(defined(XuLi_Project))||(defined(MingCai_Project)))
   else if(g_u8KeypadPressData == KEY_EXIT)	
 #else	   
   else if(g_u8KeypadPressData == KEY_MENU||g_u8KeypadPressData == KEY_EXIT||g_u8KeypadPressData == KEY_PLUS||g_u8KeypadPressData == KEY_MINUS||g_u8KeypadPressData == KEY_INCVALUE||g_u8KeypadPressData == KEY_DECVALUE)	
 #endif
   {
        if((sPMInfo.ucPMMode == ePM_POWER_DPMS) )
        {
            sPMInfo.ePMState = ePM_EXIT_PM;
	  if(g_u8KeypadPressData == KEY_EXIT)		
	     ucWakeupStatus =ePMSTS_KEYWAKEUP_ACT;		
	  else
	     ucWakeupStatus=ePMSTS_SAR_ACT	;
        }
   }
#endif   
	
}
#else
void Key_ScanKeypad( void )
{
    BYTE u8KeypadStatus=0;

    if(!g_u8KeyDebounceCounter)
    {
        u8KeypadStatus=(Key_GetKeypadStatus()^KEYPAD_MASK)&KEYPAD_MASK;

#if DEBUG_PRINT_ENABLE
    #ifdef FPGA
        u8KeypadStatus=((0xFF)^KEYPAD_MASK)&KEYPAD_MASK;
    #endif
    #if 0
        if(!u8KeypadStatus)
        {
            if(VKeyPad != VK_NOTHING)
            {
                g_u8KeypadButton=BTN_NOTHING;
                if(VKeyPad==VK_SOURCE)
                    u8KeypadStatus=KEY_EXIT;
                else if(VKeyPad==VK_MENU)
                    u8KeypadStatus=KEY_MENU;
                else if(VKeyPad==VK_LEFT || VKeyPad==VK_UP)
                    u8KeypadStatus=KEY_PLUS;
                else if(VKeyPad==VK_RIGHT || VKeyPad==VK_DOWN)
                    u8KeypadStatus=KEY_MINUS;
                else if(VKeyPad==VK_POWER)
                    u8KeypadStatus=KEY_POWER;
                else if(VKeyPad==VK_PIP)
                    u8KeypadStatus=KEY_FACTORY;
                VKeyPad = 0;
                LastKeypadButton=u8KeypadStatus;
            }
        }
    #endif
#endif

        if (u8KeypadStatus)
        {
            //printf("u8KeypadStatus=%x\n",u8KeypadStatus);
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

            if (u8KeypadStatus==KEY_MINUS)
            {
                if(g_u8KeypadButton==BTN_REPEAT)
                    g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_MINUS;

                    #if (R2_51_DUAL_MODE)
                    //if(g_u8KeypadPressData==0)
                    {
                        printMsg("Press MINUS!");
                        g_u8KeypadPressData = 0xAA;
                    }
                    #endif
                }
            }
            else if (u8KeypadStatus==KEY_PLUS)
            {
                if(g_u8KeypadButton==BTN_REPEAT)
                    g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_PLUS;

                    #if (R2_51_DUAL_MODE)
                    //if(g_u8KeypadPressData==0)
                    {
                        printMsg("Press PLUS!");
                        g_u8KeypadPressData = 0xBB;
                    }
                    #endif
                }
            }
            else if (u8KeypadStatus==KEY_MENU)
            {
                if (g_u8KeypadButton==BTN_MENU||g_u8KeypadButton==BTN_REPEAT)
                {
                    g_u8KeypadButton=BTN_REPEAT;
                }
                else
                {
                    g_u8KeypadButton=BTN_MENU;

                    #if (R2_51_DUAL_MODE)
                    //if(g_u8KeypadPressData==0)
                    {
                        printMsg("Press MENU!");
                        g_u8KeypadPressData = 0xCC;
                    }
                    #endif
                }
            }
            else if (u8KeypadStatus==KEY_EXIT)
            {
                if (g_u8KeypadButton==BTN_EXIT || g_u8KeypadButton==BTN_REPEAT)
                     g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_EXIT;

                    #if (R2_51_DUAL_MODE)
                    //if(g_u8KeypadPressData==0)
                    {
                        printMsg("Press TICK!");
                        g_u8KeypadPressData = 0xDD;
                    }
                    #endif
                }
            }
            else if (u8KeypadStatus==KEY_POWER)
            {
                if (g_u8KeypadButton==BTN_POWER || g_u8KeypadButton==BTN_REPEAT)
                    g_u8KeypadButton=BTN_REPEAT;
                else
                {
                    g_u8KeypadButton=BTN_POWER;
                    #if (R2_51_DUAL_MODE)
                    //if(g_u8KeypadPressData==0)
                    {
                        printMsg("Press Power!");
                        g_u8KeypadPressData = 0xEE;
                    }
                    #endif
                }
                if((sPMInfo.ucPMMode == ePM_POWER_DPMS) || (sPMInfo.ucPMMode == ePM_POWER_DCOFF))
                {
                    sPMInfo.ePMState = ePM_EXIT_PM;
                }
            }
            else
            {
                g_u8KeypadButton=BTN_NOTHING;
                #if R2_51_DUAL_MODE
                g_u8KeypadPressData = 0xFF;
                #endif
            }
        }
        else
        {
            g_u8KeypadButton=BTN_NOTHING;
            //LastKeypadButton=u8KeypadStatus;

            #if (R2_51_DUAL_MODE)
            //g_u8KeypadPressData = 0;
            #endif
        }
    KeyEnd:
        g_u8KeyDebounceCounter=25;

    }
}
#endif
//================================================================
