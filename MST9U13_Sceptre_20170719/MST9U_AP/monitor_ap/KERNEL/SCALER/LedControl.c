
#include "Board.h"

#if  ENABLE_LED_CONTROLLER

#define _LEDCONTROL_C_
#include <math.h>
#include "datatype.h"
#include "ms_reg.h"
#include "Mode.h"
#include "UserPrefDef.h"
#include "Global.h"
#include "Panel.h"
#include "Ms_rwreg.h"
//#include "debugMsg.h"
#include "Detect.h"
#include "mStar.h"
#include "misc.h"
#include "LedControl.h"


WORD code Stream_S[LED_STRING_NUM][2]=
{
    {LED_CH_S0, STRING_NUM_4  },
    {LED_CH_S1, STRING_NUM_5  },
    {LED_CH_S2, STRING_NUM_6  },
    {LED_CH_S3, STRING_NUM_7  },
};
WORD code Stream_D[LED_STRING_NUM][2]=
{
    {LED_CH_D0, STRING_NUM_0  },
    {LED_CH_D1, STRING_NUM_1  },
    {LED_CH_D2, STRING_NUM_2  },
    {LED_CH_D3, STRING_NUM_3  },
};


LED_INFO xdata gLEDInfo;
LED_Status xdata LEDSatus;
LED_Alert_Status xdata gLEDAlert;
static BYTE xdata g_u8PWMValue = 0;
//BYTE xdata u8States_5C=0;
//BYTE xdata u8States_5D=0;
//BYTE idata u8Inter_Count=0;
BYTE xdata u8SymState=eLEDSTATE_INITIAL;
BYTE xdata u8LEDState  =eLED_IDLE;
WORD xdata u16TotalPower=0;

#if LED_AUTO_RECOVER
BYTE xdata StringShortOpenStatus = 0;
BYTE xdata StringErrorCounter = 0;
BYTE xdata StringRecoverCounter = 0;
bit StringShortOpenChecking = 0;
bit StringShortOpenFlag = 0;
bit StringRecoverCounterFlag = 0;
bit StringRebootFlag = 0;
#define LED_AUTO_RECOVER_DEBUG      1
#endif

#define  LED_DEBUG   1
#if (ENABLE_MSTV_UART_DEBUG&&LED_DEBUG)
#define LED_printData(str, value)   printData(str, value)
#define LED_printMsg(str)               printMsg(str)
#else
#define LED_printData(str, value)
#define LED_printMsg(str)
#endif
#define  LED_DEBUG_PROTECTION 1&LED_DEBUG
#define  LED_DEBUG_CurrentAdjust 0&LED_DEBUG

//==============================================================================
void Init_LED_Setting()
{
    WORD xdata temp=0;
#if LED_CTRL_BRIGHTNESS_BY_CURRENT
        _msWriteByteMask(REG_0A84,BIT7,0x8F);    // [7]:new mode to impare tuning speed [3:0] : 256T before Neg PWM  Prepare timt
#else
    _msWriteByteMask(REG_0A84,0x01,0x0F);    // 256T before Neg PWM  Prepare timt
#endif
    _msWrite2Byte(REG_0A88, VOL_TO_SAR(NORMAL_UP));   // 1V = 0x12
    _msWrite2Byte(REG_0A8A, VOL_TO_SAR(NORMAL_LOW));  // 0.55V = 0x0A
    _msWrite2Byte(REG_0A8C, VOL_TO_SAR(SKEW_LOW));        // 1.55V = 0x1C
    _msWrite2Byte(REG_0A8E,  VOL_TO_SAR(SKWE_UP));        // 3.5V = 0x41

    _msWrite2Byte(REG_1B56,  0x00FF);        //disable GPIO_L0 ~ GPIO_L7 enable to let SAR detection is ok //20110517
}

//==============================================================================
void  msLED_ClearFlag(void)
{
    //CLR_SAR_Flag();
    _msWrite2Byte(REG_0AC4, 0xFFFF);                          // Clear LED states
    _msWrite2Byte(REG_0A4A, 0xFFFF);                        //Clear SAR INT 1 Flag
    //_msWrite2Byte(REG_0A4A, 0xFFFF);                        //Clear SAR INT 1 Flag
    _msWrite2Byte(REG_0A4C, 0xFFFF);                      //Clear SAR INT 2 Flag
    //gLEDInfo.bTime_Out =100;       //20110302
    gLEDInfo.bMEASURE_Delay =0;      //20110302                 //TIME_SAR_SAMPLE_DELAY;
}
//==============================================================================
void  Init_SAR()
{

    _msWriteByteMask(REG_0A60,0 ,BIT5|BIT7|BIT6);                              //Power On sar //One shot mode//
#if LED_CTRL_BRIGHTNESS_BY_CURRENT
    _msWriteByte(REG_0A61,0x00 );													//Sample delay time
#else
    _msWriteByte(REG_0A61,0x05 );                                                    //Sample delay time
#endif
    _msWriteByteMask(REG_0A63, BIT2|BIT1 , BIT4|BIT3|BIT2|BIT1|BIT0 );      //Sar Input Clock

    _msWrite2Byte(REG_0A40,  SAR_V_8p00);         // > 8.0 V //2 LED short protection
    _msWrite2Byte(REG_0A42,  SAR_V_4p50);         // > 4.5 V  1 led short protection
    _msWrite2Byte(REG_0A44,  SAR_V_0p10);         // <.0.1 V
}
//==============================================================================
void msLED_BoostEnable( BYTE ucStatus)
{
    if ( ucStatus == _ENABLE)
    {
        gLEDInfo.bBoostOnOff = 1;
        _msWriteByteMask(REG_0A95,BIT7,BIT7);            //Enable Boost pwm
        _msWriteByteMask(REG_0A87,BIT7,BIT7);            //Enable Boost Clk
        // 111116 coding test
        BoostCounter = BOOST_COUNT;
        BoostCounterFlag = 1;
    }
    else
    {
        gLEDInfo.bBoostOnOff = 0;
        _msWriteByteMask(REG_0A95,0 ,BIT7);              //Disable Boost pwm
        _msWriteByteMask(REG_0A87,0,BIT7);               //Disable Boost Clk
        // 111116 coding test
        BoostCounter = 0;
        BoostCounterFlag = 0;
    }
}
//==============================================================================
void msLED_DimmingEnable( BYTE ucStatus, WORD channels)
{
    if ( ucStatus == _ENABLE)
    {
        while(BoostCounterFlag);// 111116 coding test
        WAIT_V_OUTPUT_BLANKING();  //sometime  have flash  black line at  turn on panel
        LED_printData(" Dimming ON => Channel = %x " , channels);
#if LED_limitOCPtoreduceCurrentPeak //use OCP to limit current peak 20120726
        {
            BYTE xdata u8OCPVoltage;
            _msWriteByteMask(REG_0AE3,0,BIT4|BIT5);                  // OCP 10b =REG_0AE3/IFB_R Current
            //LED_printData(" Dimming ON => Channel = %x " , channels);
            _msWrite2ByteMask(REG_0A80,STREAM_VALUE_D,channels&0x000F);
            /*>>-----------------------------------
            LED dimming controller PWM0 enable0: Disable1: Enable
            LED dimming controller PWM1 enable0: Disable1: Enable
            LED dimming controller PWM2 enable0: Disable1: Enable
            LED dimming controller PWM3 enable0: Disable1: Enable
            -----------------------------------<<*/
            LED_printData(" @@@@ REG_0A80 = %x " , _msRead2Byte(REG_0A80));
            for( u8OCPVoltage = 0; u8OCPVoltage <= BOOST_CURRENT_LIMIT_LEVEL; u8OCPVoltage ++ )
            {
            ForceDelay1ms(5);
            _msWriteByteMask(REG_0AE3,(u8OCPVoltage<<4),BIT4|BIT5);                   // OCP 10b =REG_0AE3/IFB_R Current
            }
            _msWriteByteMask(REG_0AC4,BIT6,BIT6);
        }
#else
        _msWrite2ByteMask(REG_0A80,STREAM_VALUE_D,channels&0x000F);
#endif


        /*>>-----------------------------------
        LED dimming controller PWM0 enable0: Disable1: Enable
        LED dimming controller PWM1 enable0: Disable1: Enable
        LED dimming controller PWM2 enable0: Disable1: Enable
        LED dimming controller PWM3 enable0: Disable1: Enable
        -----------------------------------<<*/

#if LED_SAR_VOLTAGE_MEASURE_ENABLE
        _msWrite2ByteMask(REG_0AD0,(STREAM_VALUE_D<<4|STREAM_VALUE_D),channels&0x00FF);
        /*>>-----------------------------------
        And 68h[7:0] stand for
        68[0]: VDET0 and STR0 SAR measurement enable.
        68[1]: VDET1 and STR1 SAR measurement enable.
        68[2]: VDET2 and STR2 SAR measurement enable.
        68[3]: VDET3 and STR3 SAR measurement enable.
        68[4]: STR0 SAR measurement enable.
        68[5]: STR1 SAR measurement enable.
        68[6]: STR2 SAR measurement enable.
        68[7]: STR3 SAR measurement enable.

        In other words,
        Set 68h[7:0] = FF, all the VDET & STR channels are enable.
        Set 68h[7:0] = 0F, all the VDET channels are enable.
        Set 68h[7:0] = F0 or 00, all the channel are disable.
        -----------------------------------<<*/
#endif

#if LED_DETECT_STRN
        _msWrite2ByteMask(REG_0A68, STREAM_VALUE_S,channels&0x00F0);
        /*>>-----------------------------------
        34h[7:0]
        Are data feedback to boost calibration enable,
        The data will be considered into calibration if the channel enable is 1.
        34[0]: VDET0 channel data to boost calibration enable
        34[1]: VDET1 channel data to boost calibration enable
        34[2]: VDET2 channel data to boost calibration enable
        34[3]: VDET3 channel data to boost calibration enable
        34[4]: STR0 channel data to boost calibration enable
        34[5]: STR1 channel data to boost calibration enable
        34[6]: STR2 channel data to boost calibration enable
        34[7]: STR3 channel data to boost calibration enable
        -----------------------------------<<*/
#else
        _msWrite2ByteMask(REG_0A68, STREAM_VALUE_D,channels&0x000F);
#endif
        _msWriteByteMask(REG_0AE1,BIT6|BIT7,BIT6|BIT7);
        _msWriteByteMask(REG_0AF1,BIT6|BIT4,BIT6|BIT4);
    }
    else
    {
        //LED_printData(" Dimming OFF => Channel = %x " , channels);

        _msWrite2ByteMask(REG_0A80,0x0000,channels);
        _msWrite2ByteMask(REG_0A68,0x0000,channels);

     #if LED_SAR_VOLTAGE_MEASURE_ENABLE
        _msWrite2ByteMask(REG_0AD0,0x0000,channels);
     #endif
     _msWriteByteMask(REG_0AE1,0x00,BIT6|BIT7);
     _msWriteByteMask(REG_0AF1,0x00,BIT6|BIT4);
    }
}

#if ENABLE_TSUML_NEWOVP
void  msLED_OVP2_LevelSelection(Bool bflag)
{
    BYTE xdata u8count=0;
    BYTE code OVP2_THRESHOLD[16][2]=
    {
        { 55, 35}, //VH and VL
        { 60, 40},
        { 65, 45},
        { 70, 50},
        { 75, 55},
        { 80, 60},
        { 85, 65},
        { 90, 70},
        { 95, 75},
        { 105, 80},
        { 115, 90},
        { 125, 100},
        { 135, 105},
        { 150, 120},
        { 165, 135},
        { 180, 145}
    };
    if (bflag)
    {
        for (u8count=0; u8count<sizeof(OVP2_THRESHOLD)/2; u8count++)
        {
            //WORD wOVP2Voltage_TH = (OVP2_THRESHOLD[u8count][0] * ResistorRatio /100);
            //WORD wOVP2Voltage_TL = (OVP2_THRESHOLD[u8count][1] * ResistorRatio /100);
            WORD wOVP2Voltage_TH = ((WORD)(OVP2_THRESHOLD[u8count][0] * ResistorRatio) /100);
            WORD wOVP2Voltage_TL = ((WORD)(OVP2_THRESHOLD[u8count][1] * ResistorRatio) /100);

            if ((wOVP2Voltage_TH>OVP2_ProtVoltage)&&(wOVP2Voltage_TL<OVP2_ProtVoltage))
            {
                #if LED_DEBUG
                    LED_printData(" @@@@ u8count ===%d", u8count&0x0F);
                    LED_printData(" OVP2_TH ===%d", wOVP2Voltage_TH);
                    LED_printData(" OVP2_TL ===%d", wOVP2Voltage_TL);
                    LED_printData(" OVP2_ProtVoltage ===%d", OVP2_ProtVoltage);
                #endif

                _msWriteByteMask(REG_0AE0, BIT7,BIT7);       //Enable OVP protection
                _msWriteByteMask(REG_0AE2, u8count,0x0F);    //OVP2 level selection
                _msWriteByteMask(REG_0AFA, u8count,0x0F);		//OVP2 THR1
                _msWriteByteMask(REG_0AFA, u8count<<4,0xF0);	//OVP2 THR2
                break;
            }
        }
    }
    else
    {
        _msWriteByteMask(REG_0AE0, 0x00,BIT7);       //Disable OVP protection
        _msWriteByteMask(REG_0AE2, 0x0F,0x0F);       //OVP2 level selection
        _msWriteByteMask(REG_0AFA, 0x0F,0x0F);		//OVP2 THR1
        _msWriteByteMask(REG_0AFA, 0x0F<<4,0xF0);	//OVP2 THR2
    }
}
#endif
void msSetOCPLevel( void )
{
    DWORD xdata w16Ipeak=IPeak;
    BYTE xdata u8OCPLevel=0;

    if (w16Ipeak<eLED_OCPLV_0 && w16Ipeak>0 )
        u8OCPLevel=0;
    else if(w16Ipeak<eLED_OCPLV_1 && w16Ipeak>eLED_OCPLV_0)
        u8OCPLevel=1;
    else if(w16Ipeak<eLED_OCPLV_2 && w16Ipeak>eLED_OCPLV_1)
        u8OCPLevel=2;
    else if(w16Ipeak<eLED_OCPLV_3 && w16Ipeak>eLED_OCPLV_2)
        u8OCPLevel=3;
    else
        LED_printMsg( "@@@@@@ OCP set error");

    LED_printData("\n\  OCP  TotalCurrent = %d ", TotalCurrent);
    LED_printData("\n\r OCP GateDuty = %d ", GateDuty);
    LED_printData("\n\r OCP w16IpeakA = %d ", w16Ipeak);
    LED_printData("\n\r  OCP u8OCPLevel  = %d ", u8OCPLevel);

    // "00= 3A 01= 4V 10=5A 11= 6.5"
    _msWriteByteMask(REG_0AE3, (u8OCPLevel<<4), BIT4|BIT5);

}
//==============================================================================
void  Init_BoostPWM()
{
    _msWrite2Byte(REG_0AC0,   0xFFFF);                                   //Mask All Interrupt // LED INT Enable

#if ENABLE_TSUML_NEWOVP
    _msWrite2Byte(REG_0A90, PnlVOL_TO_SAR(PanelLightBarDefaultVoltage));         //initial boost duty = panel default voltage
#if LED_CTRL_BRIGHTNESS_BY_CURRENT
    _msWrite2Byte(REG_0A92, PnlVOL_TO_SAR(PanelLightBarDefaultVoltage*6/10));	//minimum boost duty = panel default voltage x 90%
#else
    _msWrite2Byte(REG_0A92, PnlVOL_TO_SAR(PanelLightBarDefaultVoltage*9/10));    //minimum boost duty = panel default voltage x 90%
#endif
    _msWrite2Byte(REG_0AD2, PnlVOL_TO_SAR(OVP1_ProtVoltage)<<4);                 //maximum boost duty = panel default voltage x 110% = OVP1_ProtVoltage
    _msWriteByteMask(REG_0AD3, BIT7 ,BIT7);      // Enable BST maximum setting
    //_msWriteByteMask(REG_0AD3, BIT7 ,BIT7);    // Enable BST maximum setting
    msLED_OVP2_LevelSelection(TRUE);
    #if LED_DEBUG
    LED_printData(" @@@ initial BST duty ==%x ",PnlVOL_TO_SAR(PanelLightBarDefaultVoltage));
    LED_printData(" @@@ minimum BST duty ==%x ",PnlVOL_TO_SAR(PanelLightBarDefaultVoltage*9/10));
    LED_printData(" @@@ maximum BST duty ==%x ",PnlVOL_TO_SAR(OVP1_ProtVoltage));
    #endif
#else
    _msWrite2Byte(REG_0A90, DUTY_TO_SAR(INIT_BOOST_DUTY));          //Initial PWM       // LED Initial Duty
    _msWrite2Byte(REG_0A92, DUTY_TO_SAR(MIN_BOOST_DUTY));           //Min PWM       // LED Min Duty
#endif
	_msWriteByteMask(REG_0AEF,0x00,BIT5|BIT4);	 //[5:4]CSA gain selection 2'10->3x for 33mΩ 2'b00->1x for 100mΩ

    //_msWriteByteMask(REG_0AE2,BIT5,BIT4|BIT5);                       // Boost Current limit level 10b
#if !LED_limitOCPtoreduceCurrentPeak //use OCP to limit current peak 20120726
    //_msWriteByteMask(REG_0AE3, (BOOST_CURRENT_LIMIT_LEVEL<<4), BIT4|BIT5);    //111216 set boost current limit level setting for each panel  //20111209_FOR_CAE                  //_msWriteByteMask(REG_0AE3,BIT5|BIT4,BIT4|BIT5);                  // OCP 10b =REG_0AE3/IFB_R Current
    msSetOCPLevel();    //follow formula to set OCP level 20130307
#endif
    _msWriteByteMask(REG_0AE1,   BIT0,   BIT2|BIT1|BIT0);                // Slope compensation

    //_msWriteByteMask(REG_0AE3, BIT2, BIT2);    //Enable Soft Start
    _msWriteByteMask(REG_0AEF, BIT1|BIT0, BIT1|BIT0);    //Enable Soft Start to long period

    //_msWriteByteMask(REG_0AE3,BIT5|BIT4,BIT4|BIT5);                // OCP 10b =REG_0AE3/IFB_R Current

#if LED_CTRL_BRIGHTNESS_BY_CURRENT
    _msWriteByteMask(REG_0A96,0x00,0x1F);			 	// Boost  Settle time  N // delay time for boost stable
    _msWriteByteMask(REG_0A97,0x00,0x07);			 	// Boost  Settle time  M// delay time for boost stable
#else
    _msWriteByteMask(REG_0A96,0x0B,0x1F);                        // Boost  Settle time  N // delay time for boost stable
    _msWriteByteMask(REG_0A97,0x06,0x07);                        // Boost  Settle time  M// delay time for boost stable
#endif
    if (gLEDInfo.bBoostClkODClk ==Boost_ODClk) // Panel Dclk
    {
        _msWriteByteMask(REG_0A87,BIT6,BIT6);      // Set OD clock
        _msWriteByte(REG_0A86,CLKDIV_OD_N);
    }
    else
    {
        _msWriteByteMask(REG_0A87,0,BIT6);       // Set Xtal clock
        _msWriteByteMask(REG_0A87,CLKDIV_216M_N,BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);       // Divider
        LED_printData("CLKDIV_216M_N===%x",CLKDIV_216M_N);
    }
    if (gLEDInfo.bControlModeHW == SW_Mode)
    {
        _msWriteByteMask(REG_0A95,BIT6,BIT6);                //Software Mode
    }
    else
    {
        _msWriteByteMask(REG_0A95,0,BIT6);                //Hardware Mode
        //_msWriteByte(REG_0A94, gLEDInfo.ucBoostPwmStep);    //Hardware Mode Boost Step
        msLED_Set_BoostPwmStep(gLEDInfo.ucBoostPwmStep);
    }
    msLED_ReloadBoostPWM();
    msLED_BoostEnable(_ENABLE);
}
//==============================================================================
void SetDimmingPWMFreq(WORD freq)       //111216 modified LED blacklit dimming PWMfreq setting
{
#if LED_CTRL_BRIGHTNESS_BY_CURRENT
    BYTE xdata DimmingFreq=((28000ul/(14000)) )/(1<<FQC_DIV_M);
    freq=freq;
#else
    BYTE xdata DimmingFreq=((28000ul/(freq)) )/(1<<FQC_DIV_M);
#endif
    _msWriteByte(REG_0A82,DimmingFreq);       //should be larger than preparing time of SAR(REG_0A82 >REG_0A84[3:0])        //  n

}

#if LED_CTRL_BRIGHTNESS_BY_CURRENT
void SetDimmingCurrent_INIT(BYTE current)
{
    BYTE xdata i;
    // 0~100 to 1~140

    current = (WORD)current*127/100;

    if(!current)
        current = 1;
    else if(current > 127)
        current = 127;
#if ENABLE_MSTV_UART_DEBUG
    LED_printData("@@@@@@  initial current:%d", current);
#endif

    for(i=0; i<4; i++)
        _msWriteByte(REG_0AE4+(i*2), current);
}


void SetDimmingCurrent(BYTE current)
{
    BYTE xdata i=0 ,j=0;
    // 0~100 to 1~140

    BYTE xdata PeriousCurrent;
    PeriousCurrent=_msReadByte(REG_0AE4);

    current = (WORD)current*127/100;
    if(!current)
        current = 1;
    else if(current > 127)
        current = 127;
#if LED_DEBUG_CurrentAdjust
    LED_printData("XXX current:%d", current);
    LED_printData("XXX  PeriousCurrent	==>%d", PeriousCurrent);
#endif
	if (PeriousCurrent==0)
		PeriousCurrent=1;
#if 0//ENABLE_LED_INT
        Init_LEDIRQEnable(FALSE);
#endif


	//PeriousCurrent=_msReadByte(REG_0AE4);

if (PeriousCurrent > current)
{
	for( i = PeriousCurrent; i > current; i -- )
	{
		#if LED_DEBUG_CurrentAdjust
		LED_printData("1111  I  ==>%d", i);
		#endif
		ForceDelay1ms(1);
		for(j=0; j<4; j++)
			_msWriteByte(REG_0AE4+(j*2), i);
	}
}
else
{
	#if 0//LED_DEBUG_CurrentAdjust
	LED_printData("2222  PeriousCurrent	==>%d", PeriousCurrent);
	LED_printData("2222  current	==>%d", current);
	#endif
	for( i = PeriousCurrent; i < current; i ++ )
	{
		#if LED_DEBUG_CurrentAdjust
		LED_printData("3333  I  ==>%d", i );
		#endif
		ForceDelay1ms(1);
		for(j=0; j<4; j++)
			_msWriteByte(REG_0AE4+(j*2), i);
	}
}
#if 0//ENABLE_LED_INT
	Init_LEDIRQEnable(TRUE);
#endif

}
#endif

void  Init_DimmingPWM()
{
    WORD xdata i=0;
    WORD xdata channel=0x01;
    BYTE xdata j=0;
    WORD xdata temp[5]={0};

    LED_printMsg("======Initial DimmingPWM ========");

    msLED_DimmingEnable(_DISABLE,0xFFFF);                                   //Disable Dimming PWM

    //LED dimming PWM frequency setting
    _msWriteByteMask(REG_0A83,FQC_DIV_M<<4, BIT4|BIT5|BIT6);     // m = 3
    #if !BrightFreqByVfreq
    _msWriteByte(REG_0A82,FQC_DIV_N);       //should be larger than preparing time of SAR(REG_0A82 >REG_0A84[3:0])        //  n
    #endif
    //_msWriteByteMask(REG_0A83,FQC_DIV_M<<4, BIT4|BIT5|BIT6);             // m = 3
   // _msWriteByte(REG_0A82,FQC_DIV_N);       //should be larger than preparing time of SAR(REG_0A82 >REG_0A84[3:0])           //  n

    _msWriteByteMask(REG_0AE1,((gLEDInfo.bDimCtlModeMultiPhase == MULTI_Phase)?(0):(BIT3)),BIT3); // multi phase enable

#if LED_CTRL_BRIGHTNESS_BY_CURRENT
    //SetDimmingCurrent_INIT(100);

    SetDimmingCurrent_INIT(UserPrefBrightness);
    // set Dimming duty to max
    _msWrite2Byte(REG_0AB0, 0x1FF);
    _msWrite2Byte(REG_0AB2, 0x1FF);
    _msWrite2Byte(REG_0AB4, 0x1FF);
    _msWrite2Byte(REG_0AB6, 0x1FF);
    //Reload Dimming Duty
    msLED_ReloadDimmingPWM();
#else
    for (i=1,j=0; j<LED_STRING_NUM; j++,i=i*2)
        msLED_SetDimCurrent(i, gLEDInfo.bCurrent[j]);// need to check this function
#endif

    //msLED_DimmingPWMAdjust(0x86);   // PWM duty cycle               // LED dimming controller  DUTY,  Initial Duty =50%
    if (gLEDInfo.bDimCtlModeMultiPhase == MULTI_Phase)
    {
        _msWrite2Byte(REG_0AA0,0x0010); //bit 0~9
        _msWrite2Byte(REG_0AA2,0x0040);   //45 degree
        _msWrite2Byte(REG_0AA4,0x0080);   //90 degree
        _msWrite2Byte(REG_0AA6,0x00C0);   //135 degree
    }
    else
    {
        _msWrite2Byte(REG_0AA0,0x0000); //bit 0~9
        _msWrite2Byte(REG_0AA2,0x0000);
        _msWrite2Byte(REG_0AA4,0x0000); //bit 0~9
        _msWrite2Byte(REG_0AA6,0x0000);
    }
#if 1 //20110725
    if(bPanelOnFlag)//(PANEL_ON_FLAG)
        msLED_DimmingEnable(_ENABLE,0xFFFF);                                     // Enable 16 Dimming PWM
#endif
    _msWriteByteMask(REG_0A85,BIT0,BIT0);                            // Upload PWM with Double buffer
}
//==============================================================================

#if ENABLE_LED_INT
#if LED_DEBUG_PROTECTION
bit bEnterOpen_Interrput=0;
bit bEnterShort_Interrput=0;
BYTE xdata ucShortCheckCounter=0;
WORD xdata wINT_CHECK_4F4E=0; //INT0 MASK
WORD xdata wINT_CHECK_5150=0; //INT1 MASK
WORD xdata wINT_CHECK_5352=0; //INT2 MASK

WORD xdata wINT_CHECK_5B5A=0; //INT0 STATUS
WORD xdata wINT_CHECK_5D5C=0; //INT1 STATUS
WORD xdata wINT_CHECK_5F5E=0; //INT2 STATUS

WORD xdata wINT_CHECK_6968=0; //Boost calibration Enable
WORD xdata wINT_CHECK_8180=0; //Dimming PWM Enable
WORD xdata wINT_CHECK_D1D0=0; //SAR Detection switch
#endif
#if ENABLE_TSUML_NEWOVP
    #if OVP2_INTERRUPT
    BYTE OVP2_Counter=0;
    #endif
#endif

void msLED_Interrupt(void)
{
    WORD idata i=0;
    BYTE idata Temp=0;

#if ENABLE_TSUML_NEWOVP
    #if OVP2_INTERRUPT
    if (_msRegs[REG_0AC6]&BIT5)
    {
        if(OVP2_Counter<3)
        {
            OVP2_Counter++;
            _msRegs[REG_0AC4]=_msRegs[REG_0AC4]&(~BIT5);/// clr interrupt flag
        }
        else
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(0x00);//disable dimming
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(0x00);
#if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(0x00);
#endif
            _msRegs[REG_0A52]=_msRegs[REG_0A52]|(0xFF);//disable open interrupt
            _msRegs[REG_0A4E]=_msRegs[REG_0A4E]|(0xFF);//disable two short interrupt
            gLEDInfo.bBoostOnOff = 0;
            _msRegs[REG_0A95]=_msRegs[REG_0A95]&(~BIT7);//Disable Boost pwm
            _msRegs[REG_0A87]=_msRegs[REG_0A87]&(~BIT7);//Disable Boost Clk
            OVP2_Counter=0;
            _msRegs[REG_0AC4]=_msRegs[REG_0AC4]&(~BIT5);/// clr interrupt flag
            _msRegs[REG_0AC0]=_msRegs[REG_0AC0]&(~BIT5);	// disable interrupt mask
        }
    }
    #endif
#endif
#if OCP_INTERRUPT
	else if (_msRegs[REG_0AC6]&BIT6)
	{
		_msRegs[REG_0A80]=_msRegs[REG_0A80]&(0x00);	//disable dimming
		_msRegs[REG_0A68]=_msRegs[REG_0A68]&(0x00);
#if LED_SAR_VOLTAGE_MEASURE_ENABLE
		_msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(0x00);
#endif
		_msRegs[REG_0A52]=_msRegs[REG_0A52]|(0xFF);	//disable open interrupt
		_msRegs[REG_0A4E]=_msRegs[REG_0A4E]|(0xFF);	//disable two short interrupt
		gLEDInfo.bBoostOnOff = 0;
		_msRegs[REG_0A95]=_msRegs[REG_0A95]&(~BIT7);	//Disable Boost pwm
		_msRegs[REG_0A87]=_msRegs[REG_0A87]&(~BIT7);	//Disable Boost Clk
		_msRegs[REG_0AC4]=_msRegs[REG_0AC4]&(~BIT6);	// clr interrupt flag
		_msRegs[REG_0AC0]=_msRegs[REG_0AC0]&(~BIT6);	// disable interrupt mask
	}
#endif


  #ifdef _OPEN_INTERRUPT_ENABLE_
    if ((_msRegs[REG_0A5E])|(_msRegs[REG_0A5F]))
    {
    #if LED_CH_0
        if (_msRegs[REG_0A5E]&BIT4)   // 2
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(~BIT0);
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(~BIT4);
            #if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(~(BIT0|BIT4));
            #endif
            _msRegs[REG_0A52]=_msRegs[REG_0A52]|BIT4;   //disable interrupt
        }
    #endif
    #if LED_CH_1
        if (_msRegs[REG_0A5E]&BIT5)  // 7
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(~BIT1);
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(~BIT5);
            #if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(~(BIT1|BIT5));
            #endif
            _msRegs[REG_0A52]=_msRegs[REG_0A52]|BIT5;
        }
    #endif
    #if LED_CH_2
        if (_msRegs[REG_0A5E]&BIT6)
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(~BIT2);
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(~BIT6);
            #if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(~(BIT2|BIT6));
            #endif
            _msRegs[REG_0A52]=_msRegs[REG_0A52]|BIT6;
        }
    #endif

    #if LED_CH_3
        if (_msRegs[REG_0A5E]&BIT7)
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(~(BIT3));
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(~(BIT7));
            #if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(~(BIT3|BIT7));
            #endif
            _msRegs[REG_0A52]=_msRegs[REG_0A52]|BIT7;
        }
    #endif
    }
  #endif
    if ((_msRegs[REG_0A5A])|(_msRegs[REG_0A5B]))
    {
      //  u8Inter_Count++;

        #if LED_CH_0
        if (_msRegs[REG_0A5A]&BIT0)   // 2
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(~BIT0);
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(~BIT4);
        #if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(~(BIT0|BIT4));
        #endif
            _msRegs[REG_0A4E]=_msRegs[REG_0A4E]|BIT0;   //disable interrupt
        }
        #endif
        #if LED_CH_1
        if (_msRegs[REG_0A5A]&BIT1)  // 7
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(~BIT1);
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(~BIT5);
        #if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(~(BIT1|BIT5));
        #endif
            _msRegs[REG_0A4E]=_msRegs[REG_0A4E]|BIT1;
        }
        #endif

        #if LED_CH_2
        if (_msRegs[REG_0A5A]&BIT2)
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(~BIT2);
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(~BIT6);
        #if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(~(BIT2|BIT6));
        #endif
            _msRegs[REG_0A4E]=_msRegs[REG_0A4E]|BIT2;
        }
        #endif

        #if LED_CH_3
        if (_msRegs[REG_0A5A]&BIT3)
        {
            _msRegs[REG_0A80]=_msRegs[REG_0A80]&(~(BIT3));
            _msRegs[REG_0A68]=_msRegs[REG_0A68]&(~(BIT7));
        #if LED_SAR_VOLTAGE_MEASURE_ENABLE
            _msRegs[REG_0AD0]=_msRegs[REG_0AD0]&(~(BIT3|BIT7));
        #endif
            _msRegs[REG_0A4E]=_msRegs[REG_0A4E]|BIT3;
        }
        #endif
    }


#if LED_AUTO_RECOVER
		if(StringShortOpenChecking)
		{
			StringShortOpenStatus = (_msRegs[REG_0A5A]&0x0F)|(_msRegs[REG_0A5E]&0xF0);
			if(StringShortOpenStatus)
			{
				StringShortOpenFlag = 1;

				StringRebootFlag = 0;
			}
		}
#endif
    #if LED_DEBUG_PROTECTION
    bEnterShort_Interrput=TRUE;
    wINT_CHECK_4F4E=_msRegs[REG_0A4F]<<7|_msRegs[REG_0A4E];
    wINT_CHECK_5B5A=_msRegs[REG_0A5B]<<7|_msRegs[REG_0A5A];
    wINT_CHECK_6968=_msRegs[REG_0A69]<<7|_msRegs[REG_0A68];
    wINT_CHECK_8180=_msRegs[REG_0A81]<<7|_msRegs[REG_0A80];
    wINT_CHECK_D1D0=_msRegs[REG_0AD1]<<7|_msRegs[REG_0AD0];
    #endif

    #if LED_DEBUG_PROTECTION
    bEnterOpen_Interrput=TRUE;
    wINT_CHECK_5352=_msRegs[REG_0A53]<<7|_msRegs[REG_0A52];
    wINT_CHECK_5F5E=_msRegs[REG_0A5F]<<7|_msRegs[REG_0A5E];
    wINT_CHECK_6968=_msRegs[REG_0A69]<<7|_msRegs[REG_0A68];
    wINT_CHECK_8180=_msRegs[REG_0A81]<<7|_msRegs[REG_0A80];
    wINT_CHECK_D1D0=_msRegs[REG_0AD1]<<7|_msRegs[REG_0AD0];
    #endif
}

//==============================================================================
void Init_LEDIRQEnable(BOOL bEnable)
{
    _msWrite2Byte(REG_0A48, 0xFFFF);                      //Clear SAR INT 0 Flag
    _msWrite2Byte(REG_0A4A, 0xFFFF);                      //Clear SAR INT 1 Flag
    _msWrite2Byte(REG_0A4C, 0xFFFF);                      //Clear SAR INT 2 Flag
    //==============================mask=========
    _msWrite2Byte(REG_0A50,0xFFFF);                     //Disable SAR INT 1
    #ifndef _OPEN_INTERRUPT_ENABLE_
    _msWrite2Byte(REG_0A52,0xFFFF);                       //Disable SAR INT 2
    #endif

    _msWrite2Byte(REG_0AC0,0xFFFF);                       //disable LED status INT
    _msWrite2Byte(REG_0AC4,0xFFFF);                       //Clear LED INT


    if (BoostCounterFlag==0)   //if boost is not ready , dont need to enable interrupt
    	return;

    INT_IRQ_LEDSARINT_ENABLE(bEnable);

    if (bEnable)
    {
        LED_printMsg("=Interrupt--On=");
        _msWrite2Byte(REG_0A4E, (0x00FF-STREAM_VALUE_D));  // INT1 Mask
#ifdef _OPEN_INTERRUPT_ENABLE_
         _msWrite2Byte(REG_0A52, (0x00FF-STREAM_VALUE_S));  // INT1 Mask
#endif
#if OVP2_INTERRUPT
	_msWrite2ByteMask(REG_0AC0,	 0x00	,BIT5); //enable OVP2 interrupt
#endif
#if OCP_INTERRUPT
	_msWriteByteMask(REG_0AC4,	BIT6		,BIT6);
	_msWrite2ByteMask(REG_0AC0,	 0x00	,BIT6); //enable OCP interrupt
	LED_printData("%%%%%	REG_0AC0======>>%x",_msRead2Byte(REG_0AC0));
#endif

    }
    else
    {
        LED_printMsg("=Interrupt--Off=");
        _msWrite2Byte(REG_0A4E,0xFFFF);
#ifdef _OPEN_INTERRUPT_ENABLE_
         _msWrite2Byte(REG_0A52, 0xFFFF);  // INT1 Mask
#endif
#if OVP2_INTERRUPT
	_msWrite2ByteMask(REG_0AC0,	 BIT5	,BIT5); //enable OVP2 interrupt
#endif
#if OCP_INTERRUPT
	_msWriteByteMask(REG_0AC4,	BIT6		,BIT6);
	_msWrite2ByteMask(REG_0AC0,	 BIT6	,BIT6); //enable OCP interrupt
	LED_printData("%%%%%	REG_0AC0======>>%x",_msRead2Byte(REG_0AC0));
#endif
    }

}
#endif
//==============================================================================
void Init_LEDVariable()
{
    LED_printMsg("===Init_LEDVariable===");

    u8SymState=eLEDSTATE_INITIAL;
    u8LEDState  =eLED_IDLE;

    gLEDAlert.bBOOST_PWM_States =KEEP_PWM;
    gLEDAlert.bBOOST_Warning =0;
    gLEDAlert.wLED_TwoFail =0;
    gLEDAlert.wLED_OneFail =0;
    gLEDAlert.wLED_Open =0;
    gLEDAlert.wHiBOOST =0;
    gLEDAlert.wLowBOOST =0;
    gLEDAlert.wSkew_Error=0;
    gLEDAlert.bBOOST_Warning=0;
    gLEDAlert.bSYStemFail =0;
}
//==============================================================================
Bool msCheckGDRVVoltage(void)
{
#if LED_CTRL_BRIGHTNESS_BY_CURRENT
	return TRUE;
#else
    WORD u16DetectedGDRV;
    BYTE waits = 5;
    BOOL result = FALSE;
    _msWriteByteMask(REG_0AE1,BIT6,BIT6);    // [6]Bias Enable ;

    while (waits-- )
    {
    	if(( _msReadByte( REG_0A17 ) & BIT7 ))
    	{

        u16DetectedGDRV=_msRead2Byte(REG_0A16)&0x3FF;
        LED_printData("  $$$  u16DetectedGDRV===%x", u16DetectedGDRV);
        if (u16DetectedGDRV > GDRV_THRD)
       {
            result = TRUE;
		break;
        	}
    	}
	ForceDelay1ms(100);
    };
	return result;
#endif
}
void msLED_GDRV5VEnable( void)
{
    if (msCheckGDRVVoltage()==TRUE)
    {
        LED_printMsg("  $$$  msLED_GDRV5VEnable");
#if LED_Direct5VBoostCircuit
        _msWriteByteMask(REG_0AF1,BIT6|BIT4,         BIT6|BIT4);             //[6]:boost error amp enable; [4]:RDAC enable;
        _msWriteByteMask(REG_0AF4,BIT7|BIT4|UVLO_TH,BIT7|BIT4|UVLO_TH);   //[7]:UVLO enable; [4]:GDRV bias enable; [3:0]Set UVL TH to 4.875V；
        _msWriteByteMask(REG_0AF2,0x7F,              0x7F);                  //[7:0]:GDRV 5V enable;
#else
        _msWriteByteMask(REG_0AF1,BIT6|BIT4,         BIT6|BIT4);             //[6]:boost error amp enable; [4]:RDAC enable;
        _msWriteByteMask(REG_0AF4,BIT4,              BIT7|BIT4|UVLO_TH); //[7]:UVLO enable; [4]:GDRV bias enable; [3:0]Set UVL TH to 4.875V；
        _msWriteByteMask(REG_0AF2,0x00,              0x7F);              //[7:0]:GDRV 5V enable;
#endif
    }
    else
    {
            _msWriteByteMask(REG_0AF1,0x00,              BIT6|BIT4);             //[6]:boost error amp enable; [4]:RDAC enable;
            _msWriteByteMask(REG_0AF4,0x00,              BIT7|BIT4|UVLO_TH);  //[7]:UVLO enable; [4]:GDRV bias enable; [3:0]Set UVL TH to 4.875V；
            _msWriteByteMask(REG_0AF2,0x00,              0x7F);                  //[7:0]:GDRV 5V enable;
    }
}
//==============================================================================
void  msLED_Init(void)
{
    BYTE xdata i=0;

    Init_SAR();

#if LED_CTRL_BRIGHTNESS_BY_CURRENT
	_msWriteByteMask(REG_0A6F,0x00	,BIT7); //[7]:disable GDRV voltage measurement to enhance ADC report time
#endif

    msLED_GDRV5VEnable();
#if ENABLE_HW_SHORT_PROTECTION
    _msWriteByteMask(REG_0A83,BIT3,BIT3);
#endif
#if LED_THERMAL_SENSOR_PROTECT
    _msWriteByteMask(REG_0A65,BIT6|BIT4,BIT6|BIT4|BIT3|BIT2); //[6]:OTP enable Set to 3'b100 TEMP Range will be 133~112 degree
	#if LED_CTRL_BRIGHTNESS_BY_CURRENT
	_msWriteByteMask(REG_0A67,0x00	,BIT7); //[7]:disable Thermal  measurement to enhance ADC report time
	#endif
#endif
    _msWrite2Byte(REG_0AD0,(STREAM_VALUE_D<<4|STREAM_VALUE_D));                            // Enable all SAR

#if ENABLE_LED_INT //20110302
    Init_LEDIRQEnable(FALSE);
#endif
//    LED_printData("\n\r====Version = %d =======", Version);
    msLED_BoostEnable(_DISABLE);
    msLED_DimmingEnable(_DISABLE,0xFFFF);

    for (i=0; i<LED_STRING_NUM; i++)
#if 0//LED_CTRL_BRIGHTNESS_BY_CURRENT
        gLEDInfo.bCurrent[i] = UserPrefBrightness; // UserPrefBrightness 0~100
#else
        gLEDInfo.bCurrent[i] = LEDBL_PanelDefaultCurrent;
#endif

    gLEDInfo.bBoostClkODClk = Boost_216MClk;//Boost_ODClk; //Boost_ODClk;      // different ???
    gLEDInfo.bControlModeHW = HW_Mode;

    gLEDInfo.ucBoostPwmStep = BOOST_PWM_TUNING_STEP;//0x05;
    gLEDInfo.WCurrentPWM = MIN_BOOST_PWM;               // different ???
    gLEDInfo.bDimCtlModeMultiPhase = SINGLE_Phase;                     // different ???

    //gLEDInfo.bDimCtlModeMultiPhase = MULTI_Phase;                     // different ???



    if (gLEDInfo.bControlModeHW ==HW_Mode)
    {
        LED_printMsg("ControlMode = HW_Mode");
    }
    else
    {
        LED_printMsg("ControlMode = SW_Mode");
    }

    Init_LEDVariable();
    //Init_SAR();
    Init_LED_Setting(); // set LED Normal range and skew range

    Init_DimmingPWM(); // backlight Freq

    Init_BoostPWM();
    gLEDInfo.bPREPARE_Delay  = 0;                    // 3;    (Sec) // 120327 coding, from trunk
    u8SymState = eLEDSTATE_PREPARE; // 120327 coding, from trunk
}
//==============================================================================
void msLED_SetDimCurrent(WORD Channels, BYTE Current)// need to check this function
{
    BYTE xdata i=0,j=0;

    Current = AnalogCurrent(Current);

    if (Current >=120)
        Current = 120;

    for (i=0; i<LED_STRING_NUM; i++)
    {
        if((Channels>>i)&0x01)
        {
            _msWrite2Byte(REG_0AE4+i*2,  Current);

            //LED_printData("\n\r 11111  Channels==%x",Channels);
            //LED_printData("\n\r 22222  ((Channels&0x000F)>>i)==%x",((Channels&0x000F)>>i));
            //LED_printData("\n\r 33333   _msRead2Byte(REG_0AE4+i*2)==%x", _msRead2Byte(REG_0AE4+i*2));
        }
    }
}

//==============================================================================
void msLED_ReloadBoostPWM(void)
{
    _msWriteByteMask(REG_0A85,BIT1,BIT1);        //Reload PWM Duty
}

void msLED_ReloadDimmingPWM(void)
{
    _msWriteByteMask(REG_0A85,BIT0,BIT0);        //Reload Dimming Duty
}
//==============================================================================
void msLED_CheckFlag(void)
{
    BYTE xdata reg_value;

    reg_value = _msReadByte(REG_0AC6);                            //High Byte
    //LED_printData(" ==>REG_0AC6 =%x", reg_value);
    LEDSatus.Separate.bBoost_Under_Voltage =GET_BIT7(reg_value);            //Separate[0]
    LEDSatus.Separate.bBoost_Over_Current = GET_BIT6(reg_value);     //Separate[1]
    LEDSatus.Separate.bBoost_Over_Voltage = GET_BIT5(reg_value);     //Separate[2]
    LEDSatus.Separate.bBoost_Over_Temperature = GET_BIT4(reg_value);    //Separate[3]
    LEDSatus.Separate.bBoost_Over_Skew = GET_BIT3(reg_value);            //Separate[4]
    LEDSatus.Separate.bNonuse2=0;
    LEDSatus.Separate.bNonuse1=0;
    LEDSatus.Separate.bNonuse=0;

    //if(gLEDInfo.bControlModeHW == 1)
    {
        reg_value = _msReadByte(REG_0AC7);                                         //Low Byte
        if (reg_value!=0x06)
        LED_printData(" ==>REG_0AC7 =%x", reg_value);
        LEDSatus.Separate.bHigh_Boost = GET_BIT7(reg_value);             //Separate[0]
        LEDSatus.Separate.bLow_Boost = GET_BIT6(reg_value);              //Separate[1]
        LEDSatus.Separate.bSkew_Error = GET_BIT5(reg_value);             //Separate[2]
        LEDSatus.Separate.bCross_Boundary_Error = GET_BIT4(reg_value);       //Separate[3]
        LEDSatus.Separate.bBoost_High_low_Error = GET_BIT3(reg_value);       //Separate[4]
        LEDSatus.Separate.bBoost_Tuning_Finish= GET_BIT2(reg_value);     //Separate[5]
        LEDSatus.Separate.bBoost_Data_Fnish = GET_BIT1(reg_value);           //Separate[6]
        LEDSatus.Separate.bBoost_Clock_Fail = GET_BIT0(reg_value);           //Separate[7]
    }
}
//==============================================================================
BOOST_STATE msLED_BoostPWMAdjust(BOOST_PWM_ADJUST UpDown)
{
    WORD xdata temp=0;
    BYTE   xdata Flag=Normal_PWM;


    if (UpDown == UP_PWM)
    {
        gLEDInfo.WCurrentPWM  =  _msRead2Byte(REG_0A90) +(WORD) gLEDInfo.ucBoostPwmStep;

    }
    else if (UpDown == DOWM_PWM)
    {
        gLEDInfo.WCurrentPWM =  _msRead2Byte(REG_0A90) - (WORD) gLEDInfo.ucBoostPwmStep;
    }
    else
        gLEDInfo.WCurrentPWM =  _msRead2Byte(REG_0A90);


    if (gLEDInfo.WCurrentPWM < MIN_BOOST_PWM)
    {
        gLEDInfo.WCurrentPWM = MIN_BOOST_PWM;

        Flag= Min_PWM;
    }
    else if (gLEDInfo.WCurrentPWM > MAX_BOOST_PWM)
    {
        gLEDInfo.WCurrentPWM = MAX_BOOST_PWM;

        Flag= Max_PWM;
    }
    else
    {
        Flag= Normal_PWM;
    }
    _msWrite2Byte(REG_0A90,gLEDInfo.WCurrentPWM);
    msLED_ReloadBoostPWM();
    return Flag;
}
//==============================================================================
void msLED_DimmingPWMAdjust(BYTE PwmValue)
{
    BYTE xdata i=0;

#if ENABLE_SUPER_RESOLUTION
    _msWrite2Byte(REG_0AB0, 0x1FF);
    _msWrite2Byte(REG_0AB2, 0x1FF);
    _msWrite2Byte(REG_0AB4, 0x1FF);
    _msWrite2Byte(REG_0AB6, 0x1FF);
    msLED_ReloadDimmingPWM();  //未開有辦法 update dimmingpwm嗎？ //20110303
#elif LED_CTRL_BRIGHTNESS_BY_CURRENT
    // 0~0xFF to 0~100
    //PwmValue = (WORD)PwmValue*100/255;
	#if ENABLE_LED_INT
	Init_LEDIRQEnable(FALSE);
	#endif
	SetDimmingCurrent(PwmValue);
	#if ENABLE_LED_INT
	Init_LEDIRQEnable(TRUE);
	#endif
    PwmValue = 0xFF;
#else
    for (i = 0; i < LED_STRING_NUM ; i++)
    {
        _msWrite2Byte(REG_0AB0+i*2, (WORD)(PwmValue<<1)); //512 step
    }
    msLED_ReloadDimmingPWM();  //未開有辦法 update dimmingpwm嗎？ //20110303
#endif
    g_u8PWMValue = PwmValue;
}
//==============================================================================
void msLED_Control(void)
{
    switch (u8LEDState)
    {

        case eLED_ADJUST_BOOST_HIGH:
            gLEDAlert.bBOOST_PWM_States=msLED_BoostPWMAdjust(UP_PWM);
            break;

        case eLED_ADJUST_BOOST_LOW:
            gLEDAlert.bBOOST_PWM_States=msLED_BoostPWMAdjust(DOWM_PWM);
            break;

        case eLED_ADJUST_DIMCURRENT:
            //msLED_SetDimCurrent()
            break;
        case  eLED_IDLE:

            break;

    }
}
//==============================================================================
#if 0
void msLED_CheckLED(void)
{

        printMsg(" msLED_CheckLED");

// Check One Led Short
    if (msReadWord( REG_0A5D)&STREAM_VALUE_S)
    {
        gLEDAlert.wLED_OneFail =(msReadWord( REG_0A5B)|gLEDAlert.wLED_OneFail)&STREAM_VALUE_S;

        LED_printData(" HW:gLEDAlert.wLED_OneFail = %x " , gLEDAlert.wLED_OneFail );

        msLED_SetDimCurrent((msReadWord( REG_0A5D))&STREAM_VALUE_S, DIM_CURRENT_ONE_FAIL);
    }

// Check Two Led Short
    if (msReadWord( REG_0A5B)&STREAM_VALUE_S)
    {
        gLEDAlert.wLED_TwoFail =(msReadWord( REG_0A5B )|gLEDAlert.wLED_TwoFail)&STREAM_VALUE_S;

        LED_printData(" HW:gLEDAlert.wLED_TwoFail = %x " , gLEDAlert.wLED_TwoFail );

        msLED_DimmingEnable(_DISABLE, (msReadWord( REG_0A5B))&STREAM_VALUE_S);

        msLED_SetDimCurrent((msReadWord( REG_0A5B))&STREAM_VALUE_S, 0);
    }

// Check LED OPEN
    if (msReadWord( REG_0A5F)&STREAM_VALUE_S)
    {
        gLEDAlert.wLED_Open =(msReadWord( REG_0A5F )|gLEDAlert.wLED_Open)&STREAM_VALUE_S;

        LED_printData(" HW:gLEDAlert.wLED_Open = %x " , gLEDAlert.wLED_Open );

        msLED_DimmingEnable(_DISABLE, (msReadWord( REG_0A5F ))&STREAM_VALUE_S);
    }
}
#endif
//----------------------------------------------------------------------------------------------------
void msLED_Set_BoostPwmStep(BYTE value)
{
    _msWriteByte(REG_0A94, value);
}
WORD msLED_Get_LEDSAR_Data(WORD channels)
{
    return (_msRead2Byte(REG_0A20 + channels * 2))&0x3FF;     // gete V Data
}

void msLED_Get_Led_Status(void)
{
    BYTE xdata i=0;
    WORD xdata SAR_VD_DATA[LED_STRING_NUM] = {0};
    WORD xdata SAR_VS_DATA[LED_STRING_NUM] = {0};
        //if ((_msReadByte(REG_0A83)&0x01)==0x01)  //Clear Flag  //?????? //20110302
        {
            gLEDAlert.wLED_TwoFail =0;  //
            gLEDAlert.wLED_OneFail =0;
            gLEDAlert.wLED_Open =0;
            gLEDAlert.wSkew_Error=0;
        }

    //---------------------------------------------------------------------
    for (i = 0; i < LED_STRING_NUM ; i++)     //Read LED string Data（SAR detected voltage）
    {
        SAR_VD_DATA[i] = msLED_Get_LEDSAR_Data(Stream_D[i][1]);
        SAR_VS_DATA[i] = msLED_Get_LEDSAR_Data(Stream_S[i][1]);
    }
    //---------------------------------------------------------------------
    if ( LEDSatus.Separate.bCross_Boundary_Error )   //REG_0AC7[4] ,step>V3-V4(Ex 0.55-0.54)
    {
        gLEDInfo.ucBoostPwmStep = 1;
        LED_printMsg("====>Set PwmStep = 1 ");
            msLED_Set_BoostPwmStep(gLEDInfo.ucBoostPwmStep);

    }
    if (LEDSatus.Separate.bBoost_Data_Fnish)  //REG_0AC7[1] , tunning
    {
        gLEDInfo.ucBoostPwmStep = BOOST_PWM_TUNING_STEP;
            msLED_Set_BoostPwmStep(gLEDInfo.ucBoostPwmStep);
        //LED_printMsg("====>Tuning_Data_Ready");
    }
    if (LEDSatus.Separate.bBoost_Tuning_Finish)  //REG_0AC7[2] , adjustment is finished
    {
        //LED_printMsg("====>Tuning_Finish(No Error ---)");
    }
#if LED_THERMAL_SENSOR_PROTECT
    //---------------------------------------------------------------------
    if (LEDSatus.Separate.bBoost_Over_Temperature)
    {
        msLED_DimmingEnable(_DISABLE, 0xFFFF);
        msLED_SetDimCurrent(0xFFFF, 0);
        LED_printMsg("====>bBoost_Over_Temperature ");
    }
#endif
#if 1
    //---------------------------------------------------------------------
    if ( LEDSatus.Separate.bSkew_Error)              //REG_0AC7[5] 0.55>Vmin>0.54 and Vmax>1.55(skew min voltage)
    {
        for (i = 0; i < LED_STRING_NUM ; i++)
        {
            if(Stream_D[i][0]==0)   // this CH is Close
                continue;

            if (SAR_VD_DATA[i] > SAR_V_8p00)                    //Two LED fail  led short
            {
                gLEDAlert.wLED_TwoFail |= Stream_D[i][0]; //
                gLEDAlert.wLED_TwoFail |= Stream_S[i][0]; //
                LED_printData("FW: gLEDAlert.wLED_TwoFail = %x " , gLEDAlert.wLED_TwoFail );
                msLED_DimmingEnable(_DISABLE, gLEDAlert.wLED_TwoFail);
                msLED_SetDimCurrent(gLEDAlert.wLED_TwoFail, 0);
            }
            if ((SAR_VD_DATA[i] < SAR_V_8p00)&&(SAR_VD_DATA[i] >SAR_V_4p50))  // 1 LED fail   led short
            {
                gLEDAlert.wLED_OneFail |= Stream_D[i][0]; //
                gLEDAlert.wLED_OneFail |= Stream_S[i][0]; //
                LED_printData("FW: gLEDAlert.wLED_OneFail = %x " , gLEDAlert.wLED_OneFail );
#if LED_CAL_POWER
                msLED_PowerCal((WORD *)SAR_VS_DATA, _DISABLE);
#else
                msLED_SetDimCurrent(gLEDAlert.wLED_OneFail, DIM_CURRENT_ONE_FAIL);
#endif
            }
            if ((SAR_VD_DATA[i] > SAR_V_1p55)&&(SAR_VD_DATA[i] < SAR_V_4p50))                     // Led Skew error
            {
                gLEDAlert.wSkew_Error |= Stream_D[i][0]; //
                gLEDAlert.wSkew_Error |= Stream_S[i][0]; //
                LED_printData("FW: gLEDAlert.wSkew_Error = %x " , gLEDAlert.wSkew_Error );
            }
        }
    }

    //---------------------------------------------------------------------
    if ( LEDSatus.Separate.bLow_Boost)// LED open
    {
        // Check LED Open
        if (_msRead2Byte( REG_0A5E)&STREAM_VALUE_D)
        {
            gLEDAlert.wLED_Open =(_msRead2Byte( REG_0A5E )|gLEDAlert.wLED_Open)&STREAM_VALUE_D;//&STREAM_VALUE_D;
            for (i = 0; i < LED_STRING_NUM ; i++)
            {
                //LED_printData("FW: SAR_DATA[i] = %x " , SAR_DATA[i] );
                if(Stream_D[i][0]==0)   // this CH is Close
                    continue;
                if(gLEDAlert.wLED_Open & Stream_D[i][0])
                {
                    gLEDAlert.wLED_Open |= Stream_S[i][0];
                }
            }
            msLED_DimmingEnable(_DISABLE, gLEDAlert.wLED_Open);
                    LED_printData("\n\r HW: bLow_Boost.wLED_Open = %x " , gLEDAlert.wLED_Open );
        }
        else   //isnt open , so increase current
        {
#if LED_CAL_POWER
            msLED_PowerCal((WORD *)SAR_VS_DATA, _DISABLE);  //??????
#else
            msLED_SetDimCurrent(gLEDAlert.wLED_OneFail, DIM_CURRENT_ONE_FAIL); //?????
#endif
                    LED_printMsg("\n\r  ====HW: bLow_Boost.Cal_Power ===== " );
        }
    }
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    if ( LEDSatus.Separate.bHigh_Boost)
    {
        // Check 2 LED SHORT
        if (_msRead2Byte( REG_0A5A)&STREAM_VALUE_D)
        {
            gLEDAlert.wLED_TwoFail =(_msRead2Byte( REG_0A5A )|gLEDAlert.wLED_TwoFail)&STREAM_VALUE_D;
            for (i = 0; i < LED_STRING_NUM ; i++)
            {
                //LED_printData("FW: SAR_DATA[i] = %x " , SAR_DATA[i] );
                if(Stream_D[i][0]==0)   // this CH is Close
                    continue;
                if(gLEDAlert.wLED_TwoFail & Stream_D[i][0])
                {
                    gLEDAlert.wLED_TwoFail |= Stream_S[i][0];
                }
            }
            msLED_DimmingEnable(_DISABLE, gLEDAlert.wLED_TwoFail);
            msLED_SetDimCurrent(gLEDAlert.wLED_TwoFail, 0);
            LED_printData("Dimming_DISABLE:  = %x " , gLEDAlert.wLED_TwoFail );
        }
        else
        {
#if LED_CAL_POWER
            msLED_PowerCal((WORD *)SAR_VS_DATA, _DISABLE);   //??????
#else
            msLED_SetDimCurrent(gLEDAlert.wLED_OneFail, DIM_CURRENT_ONE_FAIL); //??????
#endif
        }
    }
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    if ( LEDSatus.Separate.bBoost_Over_Skew)
    {
        // Check 1 LED SHORT
        if (_msRead2Byte( REG_0A5C)&STREAM_VALUE_D)
        {
            gLEDAlert.wLED_OneFail =(_msRead2Byte( REG_0A5C )|gLEDAlert.wLED_OneFail)&STREAM_VALUE_D;
            for (i = 0; i < LED_STRING_NUM ; i++)
            {
                //LED_printData("FW: SAR_DATA[i] = %x " , SAR_DATA[i] );
                if(Stream_D[i][0]==0)   // this CH is Close
                    continue;
                if(gLEDAlert.wLED_OneFail & Stream_D[i][0])
                {
                    gLEDAlert.wLED_OneFail |= Stream_S[i][0];
                }
            }
#if LED_CAL_POWER
            msLED_PowerCal((WORD *)SAR_VS_DATA, _DISABLE);
#else
            msLED_SetDimCurrent(gLEDAlert.wLED_OneFail, DIM_CURRENT_ONE_FAIL);
#endif
            LED_printData("SetDimCurrent:  = %x " , gLEDAlert.wLED_OneFail );
        }
        // Check 2 LED SHORT
        if (_msRead2Byte( REG_0A5A)&STREAM_VALUE_D)
        {
            gLEDAlert.wLED_TwoFail =(_msRead2Byte( REG_0A5A )|gLEDAlert.wLED_TwoFail)&STREAM_VALUE_D;
            for (i = 0; i < LED_STRING_NUM ; i++)
            {
                //LED_printData("FW: SAR_DATA[i] = %x " , SAR_DATA[i] );
                if(Stream_D[i][0]==0)   // this CH is Close
                    continue;
                if(gLEDAlert.wLED_TwoFail & Stream_D[i][0])
                {
                    gLEDAlert.wLED_TwoFail |= Stream_S[i][0];
                }
            }
            msLED_DimmingEnable(_DISABLE, gLEDAlert.wLED_TwoFail);
            msLED_SetDimCurrent(gLEDAlert.wLED_TwoFail, 0);
            LED_printData("Dimming_DISABLE:  = %x " , gLEDAlert.wLED_TwoFail );
        }
    }
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    if ( LEDSatus.Separate.bBoost_High_low_Error)
    {
        // Check LED Open
        if (_msRead2Byte( REG_0A5E)&STREAM_VALUE_D)
        {
            gLEDAlert.wLED_Open =(_msRead2Byte( REG_0A5E )|gLEDAlert.wLED_Open)&STREAM_VALUE_D;
            for (i = 0; i < LED_STRING_NUM ; i++)
            {
                //LED_printData("FW: SAR_DATA[i] = %x " , SAR_DATA[i] );
                if(Stream_D[i][0]==0)   // this CH is Close
                    continue;
                if(gLEDAlert.wLED_Open & Stream_D[i][0])
                {
                    gLEDAlert.wLED_Open |= Stream_S[i][0];
                }
            }
            msLED_DimmingEnable(_DISABLE, gLEDAlert.wLED_Open);
            LED_printData("Dimming_DISABLE:  = %x " , gLEDAlert.wLED_Open );
        }
        // Check 1 LED SHORT
        if (_msRead2Byte( REG_0A5C)&STREAM_VALUE_D)
        {
            gLEDAlert.wLED_OneFail =(_msRead2Byte( REG_0A5C )|gLEDAlert.wLED_OneFail)&STREAM_VALUE_D;
            for (i = 0; i < LED_STRING_NUM ; i++)
            {
                //LED_printData("FW: SAR_DATA[i] = %x " , SAR_DATA[i] );
                if(Stream_D[i][0]==0)   // this CH is Close
                    continue;
                if(gLEDAlert.wLED_OneFail & Stream_D[i][0])
                {
                    gLEDAlert.wLED_OneFail |= Stream_S[i][0];
                }
            }
#if LED_CAL_POWER
            msLED_PowerCal((WORD *)SAR_VS_DATA, _DISABLE);
#else
            msLED_SetDimCurrent(gLEDAlert.wLED_OneFail, DIM_CURRENT_ONE_FAIL);
#endif
            LED_printData("SetDimCurrent:  = %x " , gLEDAlert.wLED_OneFail );
        }
        // Check 2 LED SHORT
        if (_msRead2Byte( REG_0A5A)&STREAM_VALUE_D)
        {
            gLEDAlert.wLED_TwoFail =(_msRead2Byte( REG_0A5A )|gLEDAlert.wLED_TwoFail)&STREAM_VALUE_D;
            for (i = 0; i < LED_STRING_NUM ; i++)
            {
                //LED_printData("FW: SAR_DATA[i] = %x " , SAR_DATA[i] );
                if(Stream_D[i][0]==0)   // this CH is Close
                    continue;
                if(gLEDAlert.wLED_TwoFail & Stream_D[i][0])
                {
                    gLEDAlert.wLED_TwoFail |= Stream_S[i][0];
                }
            }
            msLED_DimmingEnable(_DISABLE, gLEDAlert.wLED_TwoFail);
            msLED_SetDimCurrent(gLEDAlert.wLED_TwoFail, 0);
            LED_printData("Dimming_DISABLE:  = %x " , gLEDAlert.wLED_TwoFail );
        }
    }
    #endif
}

//==============================================================================
void msLED_CheckHW(void)
{

    //-----------------------------------------------------
    //get fail led status
    msLED_Get_Led_Status();
    //-----------------------------------------------------
    //setting by status
    //msLED_Set_Led_Setting();
    //-----------------------------------------------------

}
//==============================================================================
void msLED_PowerCal(WORD *Data, Bool value)
{
    BYTE xdata i=0,Num=0;
    BYTE xdata Fail[2]=0;
    Bool xdata Low_Current=0;
    WORD xdata Temp=0;
    WORD xdata Temp1=0;
    //WORD xdata wFail_VS_Temp=0;

    for (i = 0; i < LED_STRING_NUM ; i++)
    {
        //LED_printData(" ===>%x", i);
        //LED_printData(" %x", *(Data+i));
#if LED_SKIP_DIMMING   // skip Dimming Off
        if (!(((WORD)(Stream_S[i][0]))&_msRead2Byte(REG_0A80)))
            continue;
#endif
        if (value == _DISABLE)
        {
            if (((WORD)(Stream_S[i][0]))&(gLEDAlert.wLED_OneFail|gLEDAlert.wSkew_Error))
            {
                if (Num ==0)
                    Fail[0] = Stream_S[i][1];           // First LED fail Number
                else
                    Fail[1] = Stream_S[i][1];           // SEC LED fail Number
                Num++;
                continue;
            }
        }
        Temp = SAR_TO_VOL(*(Data+i));
        Temp1 = (Temp) * gLEDInfo.bCurrent[Stream_S[i][1]]*2 + Temp1;
        LED_printData(" VSpad Power==>%d (mW)", Temp1/10);
    }

    u16TotalPower = Temp1;

    //LED_printData(" u16TotalPower==>%d (mW)", u16TotalPower/10);

    if ((Num!=0)&&(value == _DISABLE))
    {
        if (u16TotalPower >= LIMIT_POWER)
        {
            Low_Current =1;
            LED_printData("\n\r Low_Current =1   && Leave power=%d" , Temp/10);
        }
        else
        {
            Temp = (LIMIT_POWER - u16TotalPower);
            LED_printData("\n\r Low_Current =0   && Leave power=%d" , Temp/10);
        }


        //wFail_VS_Temp = SAR_TO_VOL(*(Data+Fail[0]));
        //LED_printData("\n\r wFail_VS_Temp=%d" , wFail_VS_Temp);

        if (Temp > LEAVE_POWER) //
            Temp = LEAVE_POWER;

        gLEDInfo.bCurrent[Fail[0]] = Temp/(57); //one led short , VD=5.74V
        LED_printData("\n\r gLEDInfo.bCurrent[Fail[0]]=%d" , gLEDInfo.bCurrent[Fail[0]] );

        if (gLEDInfo.bCurrent[Fail[0]]  > LEDBL_PanelDefaultCurrent*2)
            gLEDInfo.bCurrent[Fail[0]]   = LEDBL_PanelDefaultCurrent*2;

        //LED_printData("\n\r Temp=%d" , Temp);
        //LED_printData("\n\r Num=%x" , Num);
        //LED_printData("\n\r Fail[0]=%x" , Fail[0] );
        //LED_printData("\n\r Current =%x" , gLEDInfo.bCurrent[Fail[0]] );

#if 1
        switch (Num)
        {
            case 1:
                if (Low_Current ==0)
                {
                    msLED_SetDimCurrent((WORD)(1<<Fail[0]),  gLEDInfo.bCurrent[Fail[0]] );
                    LED_printData("\n\r 1 msLED_SetDimCurrent.[Fail[0]]=%d" , gLEDInfo.bCurrent[Fail[0]] );
                }
                else
                    msLED_SetDimCurrent((WORD)(1<<Fail[0]), DIM_CURRENT_LOW);
                break;

            case 2:
                gLEDInfo.bCurrent[Fail[0]] = gLEDInfo.bCurrent[Fail[0]] /2 ;
                if (Low_Current ==0)
                {
                    msLED_SetDimCurrent((WORD)(1<<Fail[0]),  gLEDInfo.bCurrent[Fail[0]]);
                    msLED_SetDimCurrent((WORD)(1<<Fail[1]),  gLEDInfo.bCurrent[Fail[0]]);
                    LED_printData("\n\r 2 SetDimCurrent.[Fail[0]]=%d" , gLEDInfo.bCurrent[Fail[0]] );
                }
                else
                {
                    msLED_SetDimCurrent((WORD)(1<<Fail[0]),  DIM_CURRENT_LOW);
                    msLED_SetDimCurrent((WORD)(1<<Fail[1]),  DIM_CURRENT_LOW);
                }
                break;

            case 3:


            default:
                gLEDAlert.bSYStemFail = 1;
                gLEDInfo.bTurnOn_Delay = TIME_TURN_ON_DELAY;
                u8SymState = eLEDSTATE_Reset;
                break;
        }
     #if LED_SAR_VOLTAGE_MEASURE_ENABLE
    _msWrite2ByteMask(REG_0AD0,0x0000,gLEDAlert.wLED_OneFail|gLEDAlert.wSkew_Error);
    LED_printData("\n\r wSkew_Error|wLED_OneFail=%x" , gLEDAlert.wLED_OneFail|gLEDAlert.wSkew_Error );
     #endif
#endif

    }
}

//==============================================================================
void msLED_CheckSW(void)
{
    WORD xdata SAR_DATA[LED_STRING_NUM] = {0};
    BYTE xdata Flag=5;
    BYTE xdata i =0;
    WORD xdata j=0;
    WORD xdata Min = 0x3FF;

    //Read 16 SAR Data
    //LED_printData("   2: PWM=%x", msReadWord(REG_0A99));

    for (i = 0; i < LED_STRING_NUM ; i++)
    {
        SAR_DATA[i] = _msRead2Byte(REG_0A20 + i * 2)&0x3FF;

#if LED_SKIP_DIMMING
        if (!(((WORD)(1<<i))&_msRead2Byte(REG_0A80)))
            continue;
#endif

        LED_printData("i = %x " , i);

        LED_printData("SAR_DATA[i] = %x " , SAR_DATA[i] );

#if 0
        LED_printData("=>%x" ,i);
#endif

        if (SAR_DATA[i] < Min)
        {
            Min = SAR_DATA[i];

            u8LED_Channel = i;
        }
    }

    LED_printData(" String Min = (%x) " , Min);

    for (i = 0; i < LED_STRING_NUM ; i++,Flag=5)
    {

        LED_printData("i = %x " , i);

        LED_printData("SAR_DATA[i] = %x " , SAR_DATA[i] );

#if LED_SKIP_DIMMING
        if (!(((WORD)(1<<i))&_msRead2Byte(REG_0A80)))
            continue;
#endif

#if LED_SKIP_LOW_BOOST
        if (((WORD)(1<<i))&gLEDAlert.wLowBOOST)
            continue;
#endif


        if (SAR_DATA[i]  < SAR_V_0p54 )
        {
            Flag = 1;                                                    // Error
        }

        if (Flag !=1)
        {

            if ((Min<= (SAR_V_0p55 + TOLERANCE_SAR))&&(Min>=(SAR_V_0p54 - TOLERANCE_SAR)))    ///Min = { SAR_V_0p54 ; SAR_V_0p55 }
            {
#if 0
                LED_printData("+++> = %x " , i);

                LED_printData("SAR_DATA[i] = %x " , SAR_DATA[i] );
#endif

                if (SAR_DATA[i] > SAR_V_4p50)                         // 1 or 2 LED Error
                {
                    Flag = 2;

                }
                else if (SAR_DATA[i] > SAR_V_1p55)                    // Led Skew error
                {
                    Flag = 4;
                }
                else
                {
                    Flag = 5;
                }

            }
            else
            {
                Flag = 3;
            }

        }

        LED_printData("  Flag=>%x", Flag );


        if (Flag ==1)  //SAR_DATA[i]  < SAR_V_0p54
        {

            if (SAR_DATA[i] > SAR_V_5p00)                                   //Two LED fail
            {
                gLEDAlert.wLED_TwoFail =SET_BIT(gLEDAlert.wLED_TwoFail,i);

                msLED_DimmingEnable(_DISABLE, (1<<i));

                msLED_SetDimCurrent(i, 0);

                u8LEDState = eLED_IDLE;

            }
            if ((SAR_DATA[i] < SAR_V_5p00)&&(SAR_DATA[i] >SAR_V_4p50))  // 1 LED fail
            {
                gLEDAlert.wLED_OneFail =SET_BIT(gLEDAlert.wLED_OneFail,i);

#if LED_CAL_POWER
                msLED_PowerCal((WORD *)SAR_DATA, _DISABLE);
#else
                msLED_SetDimCurrent((WORD)(1<<i), DIM_CURRENT_ONE_FAIL);
#endif

                u8LEDState = eLED_IDLE;

            }

            if (gLEDAlert.bBOOST_PWM_States == Max_PWM)
            {

                if (SAR_DATA[i] < SAR_V_0p10)                   //LED Open
                {

                    LED_printData("=Led Open==>%x", i );

                    LED_printData("=>%x", SAR_DATA[i] );

                    gLEDAlert.wLED_Open = SET_BIT(gLEDAlert.wLED_Open , i);

                    msLED_DimmingEnable(_DISABLE, (1<<i));

                }
                else
                {

                    LED_printData("=Low Boost==>%x", i );

                    LED_printData("=>%x", SAR_DATA[i] );

                    gLEDAlert.wLowBOOST =SET_BIT(gLEDAlert.wLowBOOST,i);
                }

            }
            else
            {
                if (u8LED_Channel ==i)
                {
                    u8LEDState = eLED_ADJUST_BOOST_HIGH;
                }
                else
                {
                    u8LEDState = eLED_IDLE;
                }
            }



        }
        else if (Flag ==3)
        {
            LED_printData(" bBOOST_PWM_States==>%x " , gLEDAlert.bBOOST_PWM_States );

            if (gLEDAlert.bBOOST_PWM_States == Min_PWM)
            {
                gLEDAlert.wHiBOOST =    SET_BIT(gLEDAlert.wHiBOOST,i);

                u8LEDState = eLED_IDLE;

                LED_printMsg("----1----");
            }
            else
            {

                if (u8LED_Channel ==i)
                {
                    u8LEDState = eLED_ADJUST_BOOST_LOW;
                }
                else
                {
                    u8LEDState = eLED_IDLE;
                }
                LED_printMsg("----2----");
            }

        }
        else if (Flag == 2)
        {

            if (SAR_DATA[i] > SAR_V_5p00)    //Two LED fail
            {
                gLEDAlert.wLED_TwoFail =SET_BIT(gLEDAlert.wLED_TwoFail,i);

                msLED_DimmingEnable(_DISABLE, (1<<i));

                msLED_SetDimCurrent(i, 0);

                u8LEDState = eLED_IDLE;

            }

            if ((SAR_DATA[i] < SAR_V_5p00)&&(SAR_DATA[i] >SAR_V_4p50))  // 1 LED fail
            {
                gLEDAlert.wLED_OneFail =SET_BIT(gLEDAlert.wLED_OneFail,i);

                msLED_PowerCal((WORD *)SAR_DATA, _DISABLE);

                u8LEDState = eLED_IDLE;

            }

        }
        else if (Flag == 4) // Skew Error
        {
            gLEDAlert.wSkew_Error =SET_BIT(gLEDAlert.wSkew_Error,i);

            msLED_SetDimCurrent((WORD)(1<<i), DIM_CURRENT_OVERSKEW);

            LED_printMsg(" =Skew_Error=");

            u8LEDState = eLED_IDLE;

        }
        else if (Flag == 5) // Normal
        {
            LED_printMsg(" =Normal=");

            u8LEDState = eLED_IDLE;
        }


#if 1
        if (u8LEDState ==0)
            LED_printMsg("=======> PWM Normal");
        else if (u8LEDState ==1)
            LED_printMsg("=======> PWM Up");
        else if (u8LEDState ==2)
            LED_printMsg("=======> PWM Low");
#endif

        msLED_Control();

        LED_printData("=======> PWM Value = %x " ,_msRead2Byte(REG_0A90));

    }
}
//==============================================================================
void msLED_CheckAlert(void)
{
    WORD xdata temp = 1;

    if (++gLEDInfo.bPrint_Delay ==0)
    {
        LEDSatus.Separate.bBoost_Tuning_Finish = 0;
        LEDSatus.Separate.bBoost_Data_Fnish =0;
        temp = LEDSatus.total;
#if 0
        LED_printData("u8States_5C= %x ", u8States_5C);
        LED_printData("u8States_5D= %x ", u8States_5D);
        LED_printData("u8Inter_Count= %x ", u8Inter_Count);
#endif

        if (gLEDInfo.bControlModeHW ==1)
        {
            if (temp)
            {
                if (LEDSatus.Separate.bBoost_Under_Voltage)
                    LED_printMsg("==>Boost_Under_Voltage");
                if (LEDSatus.Separate.bBoost_Over_Current)
                    LED_printMsg("==>Boost_Over_Current");
                if (LEDSatus.Separate.bBoost_Over_Voltage)
                    LED_printMsg("==>Boost_Over_Voltage");
                if (LEDSatus.Separate.bBoost_Over_Temperature)
                    LED_printMsg("==>Boost_Over_Temperature");
                if (LEDSatus.Separate.bBoost_Over_Skew)
                    LED_printMsg("==>Boost_Over_Skew");
                if (LEDSatus.Separate.bHigh_Boost )
                    LED_printMsg("==>High_Boost");
                if ( LEDSatus.Separate.bLow_Boost )
                    LED_printMsg("==>Low_Boost ");
                if (LEDSatus.Separate.bSkew_Error)
                    LED_printMsg("==>Skew_Error");
                if ( LEDSatus.Separate.bCross_Boundary_Error )
                    LED_printMsg("==>Cross_Boundary_Error");
                if (LEDSatus.Separate.bBoost_High_low_Error )
                    LED_printMsg("==>Boost_High_low_Error");
                if (LEDSatus.Separate.bBoost_Clock_Fail)
                    LED_printMsg("==>Boost_Clock_Fail");
            }
        }
        else
        {
            if (gLEDAlert.wLED_TwoFail ||gLEDAlert.wLED_OneFail ||gLEDAlert.wLED_Open||
                    gLEDAlert.wHiBOOST||gLEDAlert.wLowBOOST||gLEDAlert.wSkew_Error||
                    gLEDAlert.bBOOST_Warning||gLEDAlert.bSYStemFail)
            {
                LED_printData(" => TwoFail = %x ", gLEDAlert.wLED_TwoFail );
                LED_printData(" => OneFail = %x ",  gLEDAlert.wLED_OneFail );
                LED_printData(" => LED_Open = %x ", gLEDAlert.wLED_Open);
                LED_printData(" => HiBOOST = %x ", gLEDAlert.wHiBOOST);
                LED_printData(" => LowBOOST = %x ", gLEDAlert.wLowBOOST);
                LED_printData(" => Skew_Error = %x ", gLEDAlert.wSkew_Error);
                LED_printData(" => BOOST_Warning = %x ", gLEDAlert.bBOOST_Warning);
            }
        }

        if ((_msReadByte(REG_0A83)&0x01)==0x01)  //Clear Flag  //?????? //20110302
        {
            gLEDAlert.wLED_TwoFail =0;
            gLEDAlert.wLED_OneFail =0;
            gLEDAlert.wLED_Open =0;
            gLEDAlert.wSkew_Error=0;
            gLEDAlert.wHiBOOST =0;
            gLEDAlert.wLowBOOST =0;
            gLEDAlert.bBOOST_Warning=0;

            msLED_DimmingEnable(_ENABLE,0xFFFF);

            //_msWriteByteMask(REG_0A83, 0, BIT0);
        }
    }
}
//==============================================================================
void msLED_Hander(void)
{
    //BYTE temp=1; //20110302
    //LED_printData("\n\rSymState===%d",u8SymState);
#if ENABLE_LED_INT
    #if LED_DEBUG_PROTECTION
        if (bEnterShort_Interrput==TRUE)
        {
            LED_printData("1111 SHORT_MASK_27===%x",wINT_CHECK_4F4E);
            LED_printData("2222 SHORT_STATUS_2D===%x",wINT_CHECK_5B5A);
            LED_printData("3333 BSTCAL_EN_34===%x",wINT_CHECK_6968);
            LED_printData("4444 DIM_ENABLE_40===%x",wINT_CHECK_8180);
            LED_printData("5555 Which STRING is SHORT===%x",wINT_CHECK_5B5A&0x00F);
            bEnterShort_Interrput=FALSE;
        }
        else if (bEnterOpen_Interrput==TRUE)
        {
            LED_printData("1111 OPEN_MASK_29===%x",wINT_CHECK_5352);
            LED_printData("2222 OPEN_STATUS_2F===%x",wINT_CHECK_5F5E);
            LED_printData("3333 BSTCAL_EN_34===%x",wINT_CHECK_6968);
            LED_printData("4444 DIM_ENABLE_40===%x",wINT_CHECK_8180);
            LED_printData("5555 Which STRING is OPEN===%x",wINT_CHECK_5F5E&0x00F);
            bEnterOpen_Interrput=FALSE;
        }
    #endif
#endif

    switch (u8SymState)
    {
        case eLEDSTATE_INITIAL:  //0
            msLED_Init();
            gLEDInfo.bPREPARE_Delay  = 3;                    // 3;    (Sec)
            u8SymState = eLEDSTATE_PREPARE;
            //printMsg("eLEDSTATE_INITIAL");

            break;

        case eLEDSTATE_PREPARE: // 1
            #if 0//def _OPEN_INTERRUPT_ENABLE_
            if(gLEDInfo.bPREPARE_Delay ==2)
            {
                msLED_CheckStringOpen();
            }
            #endif
            if (gLEDInfo.bPREPARE_Delay ==0)
            {
            //printMsg("eLEDSTATE_PREPARE");

                msLED_ClearFlag(); // clear LED&SAR INT Flag
                msLED_ReloadBoostPWM();                // update flag
                #if 0//ENABLE_LED_INT
                Init_LEDIRQEnable(TRUE);                   //  TRUE  FALSE
                #endif
                gLEDInfo.bMEASURE_Delay =1;               //  1(Sec)
                u8SymState = eLEDSTATE_MEASURE;
            }
            break;

        case eLEDSTATE_MEASURE:    // 2
            if (gLEDInfo.bMEASURE_Delay ==0)
            {
            //printMsg("eLEDSTATE_MEASURE");

                if (GET_BIT1(_msReadByte(REG_0AC7)))  //[9]: Boost sample data compare finish interrupt flag.
                {
                    msLED_CheckFlag(); // get LED INT Status
                    if (gLEDInfo.bControlModeHW ==HW_Mode)
                    {
                        //BYTE temp=0;
                        //if (temp)
                            msLED_CheckHW();
                    }
                    else
                    {
                        msLED_CheckSW();
                    }
                    u8SymState = eLEDSTATE_Check;

		#if LED_AUTO_RECOVER
			if(LEDSatus.Separate.bBoost_Data_Fnish && LEDSatus.Separate.bBoost_Tuning_Finish)
			{
				StringShortOpenChecking = 1;
			}
		#endif
                }
                else
                {
                    u8SymState = eLEDSTATE_MEASURE;
                    break;
                }
            }
            //if (u8SymState != eLEDSTATE_Warn)   //20110302
            //    u8SymState = eLEDSTATE_Check;
            break;

        case eLEDSTATE_NORMAL:     // 3
            //printMsg("eLEDSTATE_NORMAL");

            u8SymState = eLEDSTATE_NORMAL;
            break;

        case eLEDSTATE_Check:          // 4
            //printMsg("eLEDSTATE_Check");

            msLED_CheckAlert();
            msLED_ClearFlag(); // clear SAR & LED INT Flag
            msLED_ReloadBoostPWM();         // update flag
            u8SymState = eLEDSTATE_MEASURE;
            break;

        case eLEDSTATE_Warn:       //5
                    //printMsg("eLEDSTATE_Warn");

            gLEDInfo.bTurnOn_Delay = TIME_TURN_ON_DELAY;
            msLED_BoostEnable(_DISABLE);
            msLED_DimmingEnable(_DISABLE,0xFFFF);
            LED_printMsg("=========System Restert=========");
            u8SymState = eLEDSTATE_Reset;
            break;

        case eLEDSTATE_Reset:          // 6
                    //printMsg("eLEDSTATE_Reset");

            msLED_CheckAlert();
            if (gLEDInfo.bTurnOn_Delay == 0)
                u8SymState = eLEDSTATE_INITIAL;
            break;
#if 1 //20110302
        case eLEDSTATE_Off: // 7
            //msLED_Off(); //20110725
            break;
#endif
    }
}
//==============================================================================
void msLED_Off(void)
{
    LED_printMsg("\r\n  ========msLED_Off=========");

    #if ENABLE_LED_INT
        Init_LEDIRQEnable(FALSE);
    #endif
    msLED_DimmingEnable(_DISABLE,0xFFFF);
    #if 0//CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMC
        _msWriteByteMask(REG_0AE1,   0x00,   BIT6|BIT7);
        _msWriteByteMask(REG_0AF1,   0x00,   BIT6|BIT4);
    #endif
    #if ENABLE_LED_INT
        Init_LEDIRQEnable(FALSE);
    #endif
    u8SymState = eLEDSTATE_Off;
}
void msLED_On(void)
{
    LED_printMsg("\r\n =========msLED_On=======");
    msLED_DimmingEnable(_ENABLE,0xFFFF);
    #if 0//CHIP_FAMILY_TYPE==CHIP_FAMILY_TSUMC
        _msWriteByteMask(REG_0AE1,BIT6|BIT7,BIT6|BIT7);
        _msWriteByteMask(REG_0AF1,BIT6|BIT4,BIT6|BIT4);
    #endif
    u8SymState = eLEDSTATE_MEASURE;//eLEDSTATE_INITIAL;
    #if ENABLE_LED_INT
        Init_LEDIRQEnable(TRUE);
    #endif
}


#if LED_AUTO_RECOVER
void msLED_InitRecoveryVariables(void)
{
    StringShortOpenChecking = 0;
    StringShortOpenFlag = 0;
    StringRecoverCounterFlag = 0;
    StringRebootFlag = 0;
    StringErrorCounter = 0;
    StringRecoverCounter = 0;
    StringRebootNormalCounter = 0;
}

void msLED_FastReboot(void)
{
    if(InputTimingChangeFlag)
        return;

    EA = 0;
    _msWriteByte(REG_0A4E, 0xFF); // enable int0 mask
    _msWriteByte(REG_0A52, 0xFF); // enable int2 mask
    _msWriteByte(REG_0A48, 0xFF); // clear int0 status
    _msWriteByte(REG_0A4C, 0xFF); // clear int2 status
    msLED_DimmingEnable(_ENABLE,0xFFFF);

    _msWriteByte(REG_0A4E, (0xFF-STREAM_VALUE_D)); // disable int0 mask
    _msWriteByte(REG_0A52, (0xFF-STREAM_VALUE_S)); // disable int2 mask
    StringRebootFlag = 1;
    StringRebootNormalCounter = 0;
    EA = 1;
}
#endif
#endif

