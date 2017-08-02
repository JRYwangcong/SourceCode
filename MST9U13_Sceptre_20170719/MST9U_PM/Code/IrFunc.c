#include "Board.h"





#define _IRFUNC_C_

// System
//#include <intrins.h>

// Common
//#include "board.h"
//#include "define.h"
#include "Types.h"
//#include "Mcu.h"

// External
#include "Global.h"
//#include "msModeDetect.h"
//#include "uart.h"
#include "MsIR.h"
#include "Keypad.h"
#include "i2c.h"
#include "KeyPaddef.h"

// Internal
#include "IrFunc.h"

#define IRFunc_DEBUG    1
#if ENABLE_DEBUG&&IRFunc_DEBUG
    #define IRF_printData(str, value)   printData(str, value)
    #define IRF_printMsg(str)           printMsg(str)
#else
    #define IRF_printData(str, value)
    #define IRF_printMsg(str)
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
    BOOL xdata bProcessDefaultIREvent = TRUE;
    BYTE xdata ucTmp;
    BYTE xdata ucRepeatFlag = 0;

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

    IRF_printData("\r\nir=%x \n", g_ucIrCode);

    //if ((!POWERON_FLAG1) && (g_ucIrCode != IRKEY_POWER))    //?
    //    return;

    if( !bProcessDefaultIREvent ) // Don't process default IR event
    {
        irDecodeEnd();
        return;
    }


#ifdef IR_MODE_SEL_2
if(IR_Mode_Sel_Flag)
{
    switch(g_ucIrCode)
    {

        case IRKEY_OSD_MENU_2:
     {
            IRF_printMsg("MENU KEY ");
            break;
        }

        case IRKEY_VOL_UP_2:
        {
            IRF_printMsg("VOL UP KEY ");
            break;
        }

        case IRKEY_VOL_DOWM_2:
        {
            IRF_printMsg("VOL DOWN KEY ");
            break;
        }

        case IRKEY_CH_UP_2:
        {
            IRF_printMsg("CH UP KEY ");
            break;
        }
        case IRKEY_CH_DOWM_2:
        {
            IRF_printMsg("CH DOWN KEY ");
            break;
        }

        case IRKEY_POWER_2:
        {
            IRF_printMsg("POWER KEY ");
            break;
        }
    }
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
            IRF_printMsg("MENU KEY ");
            break;
        }
        case IRKEY_OSD_LEFT_3:
        {
            IRF_printMsg("VOL UP KEY ");
            break;
        }

        case IRKEY_OSD_RIGHT_3:
        {
            IRF_printMsg("VOL DOWN KEY ");
            break;
        }

        case IRKEY_OSD_UP_3:
        {
            IRF_printMsg("CH UP KEY ");
            break;
        }
        case IRKEY_OSD_DOWM_3:
        {
            IRF_printMsg("CH DOWN KEY ");
            break;
        }

        case IRKEY_POWER_3:
        {
            IRF_printMsg("POWER KEY ");
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
            IRF_printMsg("MENU KEY ");
            break;
        }
        case IRKEY_OSD_LEFT_4:
        {
            IRF_printMsg("VOL UP KEY ");
            break;
        }

        case IRKEY_OSD_RIGHT_4:
        {
            IRF_printMsg("VOL DOWN KEY ");
            break;
        }

        case IRKEY_OSD_UP_4:
        {
            IRF_printMsg("CH UP KEY ");
            break;
        }
        case IRKEY_OSD_DOWM_4:
        {
            IRF_printMsg("CH DOWN KEY ");
            break;
        }

        case IRKEY_POWER_4:
        {
            IRF_printMsg("POWER KEY ");
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
            IRF_printMsg("MENU KEY ");
            break;
        }
        case IRKEY_OSD_LEFT_5:
        {
            IRF_printMsg("VOL UP KEY ");
            break;
        }

        case IRKEY_OSD_RIGHT_5:
        {
            IRF_printMsg("VOL DOWN KEY ");
            break;
        }

        case IRKEY_OSD_UP_5:
        {
            IRF_printMsg("CH UP KEY ");
            break;
        }
        case IRKEY_OSD_DOWM_5:
        {
            IRF_printMsg("CH DOWN KEY ");
            break;
        }

        case IRKEY_POWER_5:
        {
            IRF_printMsg("POWER KEY ");
            break;
        }
    }
}
else
#endif

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
            IRF_printData("IRKEY TO NUM = %d", ucTmp);
            break;
        }

        case IRKEY_OSD_MENU:
     {
            IRF_printMsg("MENU KEY ");
            break;
        }

        case IRKEY_VOL_UP:
        {
            IRF_printMsg("VOL UP KEY ");
            break;
        }

        case IRKEY_VOL_DOWM:
        {
            IRF_printMsg("VOL DOWN KEY ");
            break;
        }

        case IRKEY_CH_UP:
        {
            IRF_printMsg("CH UP KEY ");
            break;
        }
        case IRKEY_CH_DOWM:
        {
            IRF_printMsg("CH DOWN KEY ");
            break;
        }

        case IRKEY_POWER:
        {
            IRF_printMsg("POWER KEY ");
            break;
        }
    }
}

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
    printf("\r\nir=%x ", g_ucIrCode);
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
                ExecuteKeyEvent(MIA_INCVALUE);
            #endif
            else
	        {
	            ExecuteKeyEvent(MIA_VOLUME);
				ExecuteKeyEvent(MIA_INCVALUE);
	        }
            ucRepeatFlag |= _BIT1;
            break;
        case IRKEY_VOL_DOWM:
            if ((g_ucMenuPageIndex != MENU_ROOT) && (g_ucMenuPageIndex != MENU_INPUT_MESSAGE ))
                MenuProcessKey(BTN_LEFT);
            #if (OSD_STYLE==OSD_DEMO)
            else if (g_ucMenuPageIndex == MENU_VOLUME)
                ExecuteKeyEvent(MIA_DECVALUE);
            #endif
            else
        	{
	            ExecuteKeyEvent(MIA_VOLUME);
				ExecuteKeyEvent(MIA_DECVALUE);
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
	//printf("g_ucFactoryFlag=%x ", g_ucFactoryFlag);
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
