#include "types.h"
#include "board.h"
#include "global.h"
#include "ms_reg.h"
#include "panel.h"
#include "misc.h"
#include "ms_rwreg.h"
#include "mstar.h"
#include "debug.h"
#include "msOsd.h"
#include "menudef.h"
#include <math.h>

#if ENABLE_HDCP
    #include "MsHDCP.h"
#endif ///#if UseHDCP
#if ScanKey
#include "scankey.h"
#endif
#include "adjust.h"
#if MS_OVD
#include "msOVD_v03.h"
#endif
#if Enable_Lightsensor
#include "I2c.h"
#endif
#ifdef MS_PM
#include "MS_PM.h"
#endif
#if PANEL_VCOM_ADJUST		//121031 Modify
#include "msMiniLVDSI2c.h"
#endif
#if IT6251
#include "ITE6251.h"
#endif

#if MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
extern void SetScalerGPIO(BYTE pinX, bit HiLow);
#endif

#define TurnOffOSD      1
void Power_TurnOffLed(void);
void Power_TurnOnAmberLed(void);

void Power_TurnOffPanel(void);
void Power_PowerOffSystem(void);
void DrawOsdMenu(void);

#ifdef PowerSavingAmberLEDFlashing
    #if PSAmberLEDFlashingType == Type_Breathing
    #if MainBoardType == MainBoard_715G3329_A2 || MainBoardType == MainBoard_715G3244_TSUMU18NR|| MainBoardType ==MainBoard_712G3244_2|| MainBoardType ==MainBoard_712G3244_2 || MainBoardType == MainBoard_715G3959_T || MainBoardType == MainBoard_715G3834||MainBoardType ==MainBoard_3834_18T
	#define P06SetToPWM1    BIT5
    void InitialP06ToPWM1(Bool sel)
    {
        BYTE tempBank = mStar_ReadByte(0x00);
        mStar_WriteByte(0x00, 0x00);
        if(sel)
        {
            mStar_WriteByte(PowerSavingLEDPWM, LED_PWM_MAX);
            xfr_regs[0x54] |= P06SetToPWM1;
            bPSLEDIncToLightFlag = 0;
            LEDFlashCount = 1;
        }
        else
        {
            xfr_regs[0x54] &= ~P06SetToPWM1;
            LEDFlashCount = 0;
            bPSLEDIncToLightFlag = 0;
        }
        mStar_WriteByte(0x00, tempBank);
    }

    #define PeriodDivider   ((abs(LED_PWM_MAX-LED_PWM_MIN)>=0xA0)?(32000):(8000))
    #define CalFlashingPeriod(duty)   ((LED_PWM_MAX > LED_PWM_MIN)?\
        (((WORD)(LED_PWM_MAX-duty)*(LED_PWM_MAX-duty))/PeriodDivider+1):\
        (((WORD)(duty-LED_PWM_MAX)*(duty-LED_PWM_MAX))/PeriodDivider+1))
    void PowerSavingLEDBreathing(void)
    {
        BYTE tempDuty;
        BYTE tempBank = mStar_ReadByte(0x00);
        mStar_WriteByte(0x00, 0x00);
        tempDuty = mStar_ReadByte(PowerSavingLEDPWM);
        if(bPSLEDIncToLightFlag) // increase LED bright
        {
            if(LED_PWM_MAX > LED_PWM_MIN) // PWM value higher, LED brighter
            {
            	#if 0
                if(tempDuty == LED_PWM_MAX)
                {
                    bPSLEDIncToLightFlag = 0; // set to decrease duty
                    tempDuty--;
                }
                else
                {
                    tempDuty++;
                }
				#else	// increase LED bright immediately 
				bPSLEDIncToLightFlag = 0;
				tempDuty = LED_PWM_MIN;
				#endif
            }
            else // PWM value higher, LED darker 
            {
            	#if 0
                if(tempDuty == LED_PWM_MAX)
                {
                    bPSLEDIncToLightFlag = 0; // set to increase duty
                    tempDuty++;
                }
                else
                {
                    tempDuty--;
                }
				#else	// increase LED bright immediately 
				bPSLEDIncToLightFlag = 0;
				tempDuty = LED_PWM_MAX;
				#endif
            }
        }
        else // decrease LED bright
        {
            if(LED_PWM_MAX > LED_PWM_MIN) // PWM value higher, LED brighter
            {
                if(tempDuty == LED_PWM_MIN)
                {
                    bPSLEDIncToLightFlag = 1; // set to increase duty
                    tempDuty++;
                }
                else
                {
                    tempDuty--;
                }
            }
            else // PWM value higher, LED darker 
            {
                if(tempDuty == LED_PWM_MIN)
                {
                    bPSLEDIncToLightFlag = 1; // set to decrease duty
                    tempDuty--;
                }
                else
                {
                    tempDuty++;
                }
            }
        }
        mStar_WriteByte(PowerSavingLEDPWM, tempDuty);
        mStar_WriteByte(0x00, tempBank);

        if(tempDuty == LED_PWM_MIN)
            LEDFlashCount = 50; // 500ms
        //else if(bPSLEDIncToLightFlag)
        //    LEDFlashCount = 1;
        else
            LEDFlashCount = CalFlashingPeriod(tempDuty);
        
    }
    
    #endif
    #endif
#endif


//=============================================
// Control system power status
// caller : main()
void Power_PowerHandler(void)
{
    if (ForcePowerSavingFlag)
    {
        Clr_ForcePowerSavingFlag(); // clear force power saving flag
        if (!PowerSavingFlag)
        {
            if (PowerOnFlag)
            {
                // 071221 modified for show EnergyStarMenu
                /*if(0)//!ProductModeFlag&&!SourceChangedFlag)   //071222 for ProductMode //071225 for cancel EPA after switching source
             	{   Osd_Hide();
					Delay1ms(30);
                    MenuPageIndex = EnergyStarMenu;
                    DrawOsdMenu();
                    Delay1ms(1000);
                	MenuPageIndex = RootMenu;
           		}
					*/
                Power_TurnOnAmberLed();
				
#ifdef PowerSavingAmberLEDFlashing
    #if PSAmberLEDFlashingType == Type_Breathing
        #if MainBoardType == MainBoard_715G3329_A2  || MainBoardType == MainBoard_715G3244_TSUMU18NR|| MainBoardType ==MainBoard_712G3244_2 || MainBoardType == MainBoard_715G3959_T || MainBoardType == MainBoard_715G3834||MainBoardType ==MainBoard_3834_18T
	InitialP06ToPWM1(TRUE);
        #endif                
    #endif
#endif

#if ScanKey // led key off 071227
    #if Scan_Source == Synaptics
                if(UserPrefInputSelectType != InputSelectType_Auto)
                {
                    KeyLedCounter = 5;
                    //Set_keyLEDTurnOffFlag();
                }
    #else

        #ifdef LEDBrightCtrlByScalerPWM
                // 091020 coding for GPM request
                //SKLEDStatus = SKLEDBright_Light_Immediately;
                SKLEDStatus = SKLEDBright_Light;//091119  for SVT Keyboard //091029  only for SIT KEY BD test  by lizzie
                ScanKey_LEDBrightAction();
        #endif
    
    #endif                
#endif
            }
            
            Power_PowerOffSystem();
            if (!PowerOnFlag)
            // system is on power-off status
            {
                Power_TurnOffLed();
                {
                    BYTE tempdata = mStar_ReadByte(0x00);
                    #if Dual // 080819 HDMI spec.: DC off, TMDS bus/hot-plug should keep in high.
                    mStar_WriteByte(REGBK, REGBANKADC);    //mStar_WriteByte(0x00, 0x01);
                    #ifdef TSUMXXNX
                    mStar_WriteByte(TESTEN, mStar_ReadByte(TESTEN)|BIT7);
                    mStar_WriteByte(TESTA5, mStar_ReadByte(TESTA5)|BIT0);
                    mStar_WriteByte(BK1_D1, mStar_ReadByte(BK1_D1) |BIT5);
                    mStar_WriteByte(TESTEN, mStar_ReadByte(TESTEN)&~BIT7);
                    #else
                    mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4) | (BIT0|BIT1)); // power down dvi clk r-termintor
                    #endif
                    #endif        
                    mStar_WriteByte(0x00, 0x00);
                    mStar_WriteByte(SWRST, 0x80); // enable deep power down mode,Please remember to check lock OSD Function!
                    mStar_WriteByte(0x00, tempdata);
                }
            }

        #ifdef MS_PM
            if(!PowerOnFlag)
                msPM_SetUp(ePM_PowerOff);
            #ifdef ENABLE_STANDBYINTOPM
            else
                msPM_SetUp(ePM_Standby);
            #endif
            //msPM_WaitingEvent();
        #endif
            
        }
    }
}
#if 0
    void Power_PowerOnSystem(void)
    {
        Power_TurnOffPanel();
        mStar_PowerUp();
        Clr_PowerSavingFlag();
        Set_InputTimingChangeFlag();
        #if AOC
            if (UserPrefVolume)
                hw_ClrMute();
            hw_ClrStandby();
        #endif 
        SrcFlags |= SyncLoss;
    }
#endif 
#if 1
    extern void mStar_AdjustVolume(BYTE volume);
    void Power_PowerOnSystem(void)
    { 
        #if USB_Contral
            hw_USB_HI();
        #endif 
        
        gBoolVisualKey = 0;
        gByteVisualKey = 0xff;
        //BYTE Volume;  //wmz 20051019
        Power_TurnOffPanel();
        mStar_PowerUp();
        
    #if ScanKey
        #if Scan_Source == Synaptics
        Set_StartScanKeyFlag();
        ScanKey_Init();
        #endif
    #endif
    
    #if Enable_Lightsensor
        LightSensor_On();
    #endif
    
        Clr_PowerSavingFlag();
        Set_InputTimingChangeFlag();
		#if Dual
	 Set_ShowInputInfoFlag();
		#endif
	 
        SrcFlags |= SyncLoss;
    }
    void Power_PowerOffSystem(void)
    {
        #if USB_Contral
            if (!PowerOnFlag)
                hw_USB_LOW();
        #endif 

#if 0//ScanKey // key led off 071227
        if(!PowerOnFlag)
            KeyLED_TurnOff();
#endif
        #if AudioFunc// 2006/10/18 11:57AM by Emily  for power off disable audio
        hw_SetMute();
            #if (MainBoardType!=MainBoard_2698_2_X_X_2)&&(MainBoardType!=MainBoard_2698_2_X_X_8)&&(MainBoardType!=MainBoard_2810_B)
                hw_SetStandby();
            #endif 
        #endif 

        Power_TurnOffPanel();
       #if EnablePanelServiceMenu//PanelType==PanelCLAA170WA02
       hw_ClrPanel();
       #endif
        #if Enable_Lightsensor
        LightSensor_Off();
        #endif
        mStar_PowerDown();
        Set_PowerSavingFlag();
        Clr_InputTimingChangeFlag();
        OsdCounter = 0;
        PowerDownCounter = 0;
        #if 0//ENABLE_HDMI
        bDigitalGraphicReset=1;
        #endif
        Clr_OsdTimeoutFlag();

        Clr_DoModeSettingFlag();		//110311 Modify
        Set_InputTimingStableFlag();	//110311 Modify
        
    }
#endif 
void Power_TurnOffLed(void)
{
    #if ScanKey
        #if Scan_Source == Cypress
        hw_ClrGreenLed();
        #endif
    #else
    hw_ClrGreenLed();
    hw_ClrAmberLed();
    #endif
}



void Power_TurnOnGreenLed(void)
{
    #if ScanKey
        #if Scan_Source == Cypress
        hw_SetGreenLed();
        #endif
    #else
    hw_ClrAmberLed();
    hw_SetGreenLed();
    #endif

}

//void Power_TurnOffGreenLed(void)
//{ hw_ClrGreenLed();
//}
void Power_TurnOnAmberLed(void)
{ 
    #if ScanKey
        #if Scan_Source == Cypress
        hw_SetAmberLed();
        #endif
    #else
    hw_ClrGreenLed();
    hw_SetAmberLed();
    #endif    
}
#if Enable_ProductModeAdjust
void Set_GreenLedFlash(BYTE times)
{
	BYTE i;
	hw_ClrAmberLed();
	for(i=0;i<times;i++)
	{
		hw_ClrGreenLed();
		Delay1ms(20);
		hw_SetGreenLed();
		Delay1ms(20);
	}
}
void Set_AmberLedFlash(BYTE times)
{
	BYTE i;
	hw_ClrGreenLed();
	for(i=0;i<times;i++)
	{
		hw_ClrAmberLed();
		Delay1ms(20);
		hw_SetAmberLed();
		Delay1ms(20);
	}
}
#endif
#if PanelRSDS||PanelminiLVDS
#if MainBoardType == MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2
// set vcom pwm to io when off
void VcomPWMCtrl(BYTE on)
{
    BYTE tempBank = mStar_ReadByte(0x00);
    mStar_WriteByte(0x00, 0x00);
    if(on)
    {
        mStar_WriteByte(0x63, mStar_ReadByte(0x63)|BIT3);
    }
    else
    {
        mStar_WriteByte(0x63, mStar_ReadByte(0x63)&~BIT3);
        P2_3 = 0;
    }
    mStar_WriteByte(REGBK, tempBank);
}

// GVoff high -> VGH low
void GVoffCtrl(BYTE on)
{
    BYTE tempBank = mStar_ReadByte(0x00);
    mStar_WriteByte(REGBK, REGBANKTCON);
    if(on)
    {
        mStar_WriteWord(G5VST_H, GPO5_VStart);	
        mStar_WriteWord(G5VEND_H, GPO5_VEnd);
        mStar_WriteWord(G5HST_H, GPO5_HStart);
        mStar_WriteWord(G5HEND_H, GPO5_HEnd);
        mStar_WriteByte(G5CTRL, GPO5_Control);
    }
    else
    {
        mStar_WriteWord(G5VST_H, 0);	
        mStar_WriteWord(G5VEND_H, 0);
        mStar_WriteWord(G5HST_H, 0);
        mStar_WriteWord(G5HEND_H, 0);
        mStar_WriteByte(G5CTRL, 1);
    }
    mStar_WriteByte(REGBK, tempBank);
}

    // 1: set to TTL, 0: set to RSDS
    void SetRSDSToTTL(BYTE on)
    {
        BYTE tempBank = mStar_ReadByte(0x00);

        mStar_WriteByte(0x00, 0x01);
        
        if(on) // set to TTL
        {
            mStar_WriteByte(0x20, 0x80);
            mStar_WriteByte(0x2F, 0x04); // 090209 hdcp 
            mStar_WriteByte(0x2E, 0x84);
            mStar_WriteByte(0x20, 0x00);
        }
        else // set to original(RSDS)
        {
            mStar_WriteByte(0x20, 0x80);
            #if MainBoardType==MainBoard_715G3289_C2
            mStar_WriteByte(0x2E, 0xba);
            #else
            mStar_WriteByte(0x2E, 0x0f);
            #endif
            mStar_WriteByte(0x20, 0x00);
        }
            
        mStar_WriteByte(0x00, tempBank);
    }

#endif
#if PanelType!=PanelCMO220Z1M01
    void sMute(BYTE on)
    {
        mStar_WriteByte(REGBK, REGBANKTCON);
        if (on)
        {
            #if MainBoardType==MainBoard_3289_A2 || MainBoardType==MainBoard_715G3289_C2
            
            GVoffCtrl(1);
            
            mStar_WriteByte(IFCTRL, OuputFormat | PUA_B); // tcon active

            // 090223 OE should pull low 2 frame time after power on
            ForceDelay1ms(40);
            mStar_WriteWord(G4VST_H, GPO4_VStart); // turn OE
            mStar_WriteWord(G4VEND_H, GPO4_VEnd);
            mStar_WriteWord(G4HST_H, GPO4_HStart);
            mStar_WriteWord(G4HEND_H, GPO4_HEnd);
            mStar_WriteByte(G4CTRL, GPO4_Control);
            SetRSDSToTTL(0);
            #else
            
            mStar_WriteByte(IFCTRL, OuputFormat | PUA_B); // turn off OE
            Delay1ms(20);
            mStar_WriteWord(G4VST_H, GPO4_VStart); // turn OE
            mStar_WriteWord(G4VEND_H, GPO4_VEnd);
            mStar_WriteWord(G4HST_H, GPO4_HStart);
            mStar_WriteWord(G4HEND_H, GPO4_HEnd);
            mStar_WriteByte(G4CTRL, GPO4_Control);
            
            #endif
            
            // 090317
            mStar_WriteByte(0x00, 0x00);
            mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)|TCOP_B); // enable TCON GPO output
            
        }
        else
        {
            #if MainBoardType==MainBoard_3289_A2 || MainBoardType==MainBoard_715G3289_C2

            mStar_WriteByte(IFCTRL, OuputFormat); // tcon inactive

            // OE output low
            mStar_WriteWord(G4VST_H, 0); // turn OE
            mStar_WriteWord(G4VEND_H, 0);
            mStar_WriteWord(G4HST_H, 0);
            mStar_WriteWord(G4HEND_H, 0);
            mStar_WriteByte(G4CTRL, 1); // Active low
            GVoffCtrl(0);
            SetRSDSToTTL(1);
            #else
            
            mStar_WriteByte(IFCTRL, OuputFormat);
            mStar_WriteWord(G4VST_H, 0); // turn OE
            mStar_WriteWord(G4VEND_H, 0);
            mStar_WriteWord(G4HST_H, 0);
            mStar_WriteWord(G4HEND_H, 0);
            mStar_WriteByte(G4CTRL, 1); // Active low

            #endif

            // 090317
            mStar_WriteByte(0x00, 0x00);
            mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)&~TCOP_B); // disable TCON GPO output
            
        }
        mStar_WriteByte(REGBK, REGBANKSCALER);
    }
#endif 
#endif
//#if (ModelName==ENVISION_G2217||ModelName==ENVISION_G2219||ModelName ==AOC_2219P2)//Amy2007.12.20   9:41:42
#if PanelType==PanelCMOM220Z1_PS3||PanelType==PanelCLAA220WA02
void Power_TurnOnPanel(void)
{
#if MS_OVD // 081124 od modified
        msOD_Reset(_OD_Enable);
#endif
  Set_PanelOnFlag();	//110421 Modify	
  mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)|TCOP_B); // for background black
  mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)&~WHTS_B);
  mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)|BLKS_B); // for background black
  hw_SetPanel();
  ForceDelay1ms(PanelOnTiming1);
  #if PanelRSDS
    sMute(1);
    mStar_WriteByte(OCTRL3, 0x1F);// RSDS output with all GPO
  #elif PanelLVDS
    mStar_WriteByte(OCTRL3, ENRT_B); // LVDS output
  #endif

       mStar_WriteByte(TRISTATE, 0);
      #if 1//wh060706
      mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)|BLKS_B);
	if(!ProductModeFlag)
	      ForceDelay1ms(100);//ForceDelay1ms(50);
      mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)&~BLKS_B);
     #endif
	  ForceDelay1ms(PanelOnTiming2);
	  if (InputTimingChangeFlag && !DisplayLogoFlag)
	    Power_TurnOffPanel();
	  else
  	{
         hw_SetBlacklit();
  	}
}

	void Power_TurnOffPanel( void )
	{
	#if TurnOffOSD
	    mStar_WriteOSDByte( OSDDBC, 0x0 ); // disable osd double buffer
	    mStar_WriteOSDByte( IOSDC1, 0 );
	    mStar_WriteOSDByte( DBFC, 0x0 ); // disable double buffer
	    //mStar_WriteByte( OCTRL2, mStar_ReadByte( OCTRL2 ) | BLKS_B );
	    Delay4us();
	#endif

	    hw_ClrBlacklit();
	    Clr_PanelOnFlag();
	    ForceDelay1ms( PanelOffTiming1 );
	    //Timer_ForceDelay1ms(PanelOffTiming1); //2006-06-01 Andy

	    //To Reduce Flicker  wumaozhong 20050608
	    mStar_WriteByte( OCTRL2, mStar_ReadByte( OCTRL2 ) | WHTS_B );
           if(!ProductModeFlag)
               ForceDelay1ms(100);//ForceDelay1ms(80);
	    mStar_WriteByte( OCTRL2, mStar_ReadByte( OCTRL2 ) & ~WHTS_B );

	    mStar_WriteByte( TRISTATE, 0xFF );
	#if PanelRSDS
	    sMute( 0 );
	#endif

	#if 1//!PanelTTL
	    mStar_WriteByte( OCTRL3, 0 );
	#endif
	    ForceDelay1ms( PanelOffTiming2 );
	#if EnablePanelServiceMenu
	if(PanelSettingFlag&bChangeModePanelVddOff)
	#endif
	    hw_ClrPanel();

	#if TurnOffOSD
	    mStar_WriteOSDByte( OSDDBC, 0x5 ); // enable double
	#endif
	#if (PanelType==PanelCLAA220WA02)
		if(!ProductModeFlag)
	Delay1ms(320);
	#endif	
	}
	#elif PanelType==PanelCMO220Z1M01
	void hw_ClrPanel(void)  //nick add for GPO panel control
	{
	  mStar_WriteByte(REGBK, REGBANKTCON);
	  mStar_WriteByte(G6CTRL, 0x00); // 070525 modified for panel power // Origl: 1; Revse:0
	  mStar_WriteByte(REGBK, REGBANKSCALER);
	}

	void hw_SetPanel(void)
	{
	  mStar_WriteByte(REGBK, REGBANKTCON);
	  mStar_WriteByte(G6CTRL, 0x01); // 070525 modified for panel power // Origl: 0; Revse:1
	  mStar_WriteByte(REGBK, REGBANKSCALER);
	}

	void Set_CLK(void)
	{
		  mStar_WriteByte(REGBK, REGBANKTCON);
		     mStar_WriteByte(G3HEND_H, 0x00); // 070525 modified for panel power // Origl: 1; Revse:0
		     mStar_WriteByte(G3HEND_L, 0x00); // 070525 modified for panel power // Origl: 1; Revse:0
		     mStar_WriteByte(G3HST_H, 0x00); // 070525 modified for panel power // Origl: 1; Revse:0
		     mStar_WriteByte(G3HST_L, 0x00); // 070525 modified for panel power // Origl: 1; Revse:0
		     mStar_WriteByte(G3CTRL, 0x01); // 070525 modified for panel power // Origl: 1; Revse:0
		  mStar_WriteByte(REGBK, REGBANKSCALER);
	}

	void Clr_CLK(void)
	{
		  mStar_WriteByte(REGBK, REGBANKTCON);
		     mStar_WriteByte(G3HEND_H, 0x02); // 070525 modified for panel power // Origl: 1; Revse:0
		     mStar_WriteByte(G3HEND_L, 0xD0); // 070525 modified for panel power // Origl: 1; Revse:0
		     mStar_WriteByte(G3HST_H, 0x01); // 070525 modified for panel power // Origl: 1; Revse:0
		     mStar_WriteByte(G3HST_L, 0x80); // 070525 modified for panel power // Origl: 1; Revse:0
		     mStar_WriteByte(G3CTRL, 0x01); // 070525 modified for panel power // Origl: 1; Revse:0
		  mStar_WriteByte(REGBK, REGBANKSCALER);

	}
		
	void mStar_TurnOnTCON(void)
	{ //mStar_WriteByte(PLLCTRL2, 0); // PLL on -> TCON CLK on
	  mStar_WriteByte(REGBK, REGBANKTCON);
	  mStar_WriteByte(IFCTRL, OuputFormat|PUA_B); // STB, STV, CKV, POL, CLK
	  mStar_WriteByte(REGBK, REGBANKSCALER);
	  //mStar_WriteByte(OCTRL3, 0x1D); // STB, STV, CKV Bruce 0309.2004 // Single 0x15, Dual 0x1D
	}
	void mStar_TurnOffTCON(void)
	{ //mStar_WriteByte(OCTRL3, 0x00); // STB, STV, CKV Bruce 0309.2004 // Single 0x15, Dual 0x1D
	  mStar_WriteByte(REGBK, REGBANKTCON);
	  mStar_WriteByte(IFCTRL, OuputFormat); // STB, STV, CKV, POL, CLK
	  mStar_WriteByte(REGBK, REGBANKSCALER);
	  //mStar_WriteByte(PLLCTRL2, LP_PD_B); // PLL off -> TCON CLK off
	}
	void mStar_TurnOnData(void)
	{ BYTE Temp;
	   Temp=mStar_ReadByte(OCTRL2);
	   mStar_WriteByte(OCTRL2, Temp|TCOP_B | DOT_B | (PanelDualPort&DPO_B)|(PanelSwapPort&DPX_B));//Bruce 1103.2003
	   mStar_WriteByte(TRISTATE, 0x00);
	}
	void mStar_TurnOffData(void)
	{ mStar_WriteByte(OCTRL2, TCOP_B | DOT_B | BLKS_B|(PanelDualPort&DPO_B)|(PanelSwapPort&DPX_B));//Bruce 1103.2003
	  ForceDelay1ms(10);
	  mStar_WriteByte(TRISTATE, 0x7F);
	}

	void Power_TurnOnPanel(void)
	{ 

		#if MS_OVD // 081124 od modified
	        msOD_Reset(_OD_Enable);
		#endif
	
		Set_PanelOnFlag();		//110421 Modify		
	    mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) | BLKS_B);	//Set black background	//2006-03-13 Andy
           Set_CLK();
	    hw_SetPanel();	   
	    ForceDelay1ms(PanelOnTiming0);		
	    hw_SetPanel12V(); 
	    ForceDelay1ms(PanelOnTiming1);	
           Clr_CLK();
	#if RSDS
	    mStar_TurnOnTCON();
	    mStar_WriteByte(OCTRL3, 0x1f); 				//20050530, Justin for power sequence tunning
	#else
	    mStar_WriteByte(OCTRL3, ENRT_B); 			// LVDS output	//20050511, Justin
	#endif

	    ForceDelay1ms(PanelOnTiming2);					// 360ms		
	   if (InputTimingChangeFlag&&!DisplayLogoFlag)						// for mode change
	     { 
	       Power_TurnOffPanel();
	       return;
	      }
	   else
	#if RSDS  
		mStar_TurnOnData();
	#endif
	  	ForceDelay1ms(PanelOnTiming3);					// 700ms
	  	ForceDelay1ms(PanelOnTiming4);					// 30ms

		
	   mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)&~BLKS_B); 	//Clr black background   //20050119, Justin
      
	       hw_SetBlacklit();  
               ForceDelay1ms(220);            
               mStar_WaitForDataBlanking();
	   //mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)&~BLKS_B); 

	}

	void Power_TurnOffPanel(void)
	{
	 //  mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) | 0x10);	//Set black background	//20050119, Justin
	 //  Delay1ms(20);	
	#if 1 
	   mStar_TurnOffData();
	#endif
	  ForceDelay1ms(PanelOffTiming0);					//5ms
	  hw_ClrBlacklit();
	  Clr_PanelOnFlag();
         Set_CLK();
	    ForceDelay1ms(PanelOffTiming1);					// 5ms	
	    mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)|WHTS_B);
	    ForceDelay1ms(PanelOffTiming2);					// 15ms
	#if 1
	    mStar_TurnOffTCON();
	    ForceDelay1ms(30);
	#endif
	   hw_ClrPanel12V();
	#if RSDS 
	   mStar_WriteByte(OCTRL3, 0x10); 	//20050511, Justin
	#endif
	    ForceDelay1ms(PanelOffTiming4);	
	    mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)&~WHTS_B);
	    hw_ClrPanel();
		
	}	
	#else
void Power_TurnOnPanel(void)
{
/*
#if MS_OVD // 081124 od modified
    msOD_Reset(_OD_Enable);
#endif

    while(bPanelDelayFlag)	//110421 Modify
    {
        if(InputTimingChangeFlag && !DisplayLogoFlag)
        {
            PanelOffOnDelayCntr = 0;
            bPanelDelayFlag = 0;
            return;
        }
    }
	//*/
   mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) & ~BLKS_B);	

	
    hw_SetPanel();

	ForceDelay1ms(PanelOnTiming1);
	
    #if PanelRSDS||PanelminiLVDS

    mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2)|BLKS_B);

    #if MainBoardType==MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2
    ForceDelay1ms(11); 
    VcomPWMCtrl(1);
    sMute(1);
    mStar_WriteByte(OCTRL3, 0x1F); // RSDS output
    #else
    sMute(1);
    mStar_WriteByte(OCTRL3, 0x1F); // RSDS output
    #endif

        
    #elif PanelLVDS
  //      ForceDelay1ms(PanelOnTiming1);
        mStar_WriteByte(OCTRL3, ENRT_B); // LVDS output
    #endif 
    mStar_WriteByte(TRISTATE, 0);
    
    #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
       if(AC_PowerFlag)  // jeff 2009 1109
        {
        Clr_AC_PowerFlag();
        I_Gen_Tuning();
        }
       else
        {
    mStar_WriteByte ( REGBK, REGBANKTCON );
    mStar_WriteByte ( G0VST_H, 0x0D );
    mStar_WriteByte ( G0VST_L, 0x0D );
    mStar_WriteByte ( REGBK, REGBANKSCALER );
	   }
    #endif   


#ifdef TSUMXXT//20110301 add please check again		//110311 Modify
    #if PanelLVDS
		  mStar_WriteByte(REGBK,REGBANK2);	   // switch to REGBANK2
		  mStar_WriteByte(BK2_08, BIT3 | BIT2 | BIT0);	// enable LVDS mode
		  mStar_WriteByte(BK2_09, BIT3 | BIT2 | BIT0);	// enable LVDS mode
		  mStar_WriteByte(REGBK,REGBANKSCALER);
    #endif
#else
		mStar_WriteByte(TRISTATE, 0);
#endif
//*/
    //mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) | BLKS_B);
   // ForceDelay1ms(50);
    ForceDelay1ms(PanelOnTiming2-50);
    //mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) &~BLKS_B);
/*
	#if(ModelName == AOC_e2237Fw || ModelName==ENVISION_P2271w || ModelName==ENVISION_P2471w  || ModelName==ENVISION_P2071l||ModelName == ENVISION_P2271wl||\
		ModelName==AOC_2330V_PLUS||ModelName==AOC_2330V_14L||ModelName==ENVISION_P2471||ModelName==ENVISION_P2371l|| ModelName==ENVISION_P2071l1||\
		ModelName == ENVISION_P2271wl1||ModelName==ENVISOIN_P2271WS||ModelName==ENVISION_P2371l2||ModelName==ENVISION_P971waL2 || (ModelName == ENV_P971WALS_TPM185) ||\
		(ModelName == ENV_P971WLS_TPM185_NO_AUDIO)||ModelName ==ENVISION_P971WALS)
	if(DisplayLogoFlag)
        mStar_AdjustBrightness(100);
	 #if Enable_Lightsensor // 100414 coding addition for that timing change caused brightness change.
    else if(UserPrefiCareMode == LightSensorOn)
        mStar_AdjustBrightness(TempBrightness);
    #endif        

    else
        mStar_AdjustBrightness(UserPrefBrightness);
	#endif
	//*/
/*
#if PANEL_VCOM_ADJUST		//121031 Modify
	if(g_bACPowerOn_CheckVCOM)
	{
	#if VCOM_TYPE == VCOM_MAXIM 	//120322 Modify
		WORD ucReg;
	#else
		BYTE ucReg;
	#endif
		ucReg = msReadMiniLVDSi2c_Vcom();

		if (ucReg!=FactorySetting.VcomValue)
		{
			msWriteMiniLVDSi2c_Vcom(FactorySetting.VcomValue);
		}
		g_bACPowerOn_CheckVCOM = FALSE;
	}
#endif
//*/
     #if IT6251
CLR_EDP_IT6251_RSTB();
ForceDelay1ms(5);
SET_EDP_IT6251_RSTB();	 
IT6251_DeviceLoopProcedure();
     #endif

    if (InputTimingChangeFlag && !DisplayLogoFlag)
    {
        Power_TurnOffPanel();
    }
    else
    	{
        hw_SetBlacklit();
	 Set_PanelOnFlag();
    	}
	#if PanelType==PanelM185B3LA1//20160121 lixw
	   mStar_WriteByte(REGBK, REGBANKTCON);
	   mStar_WriteByte(BK2_6D, mStar_ReadByte(BK2_6D) & ~BIT4);	
	   mStar_WriteByte(REGBK, REGBANKSCALER);
	#endif
}
#ifdef  TURNOFF_PANEL_BEFORE_DISABLE_LVDS_OUTPUT//petit 20121101
void Power_TurnOffPanel(void)
{
    // 080123
    if(!PanelOnFlag)
        return;

    Clr_PanelOnFlag();

    #if TurnOffOSD
    mStar_WriteOSDByte(OSDDBC, 0x0); // disable osd double buffer
    mStar_WriteOSDByte(IOSDC1, 0);
    mStar_WriteOSDByte(DBFC, 0x0); // disable double buffer
    //mStar_WriteByte( OCTRL2, mStar_ReadByte( OCTRL2 ) | BLKS_B );
    Delay4us();
    #endif 
    hw_ClrBlacklit();
    ForceDelay1ms(PanelOffTiming1);
    
    mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) | WHTS_B);

    #if PanelLVDS
    ForceDelay1ms(PanelOffTiming2);
    #endif

    #if EnablePanelServiceMenu
    if(PanelSettingFlag&bChangeModePanelVddOff)
    #endif
    hw_ClrPanel();

    ForceDelay1ms(TurnOffDataAfterTurnOffVCC);//petit 20121101

    //To Reduce Flicker  wumaozhong 20050608
    //ForceDelay1ms(300);

    //mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) & ~WHTS_B);

    #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
    mStar_WriteByte ( REGBK, REGBANKTCON );
    mStar_WriteByte ( G0VST_H, 0x01 );
    mStar_WriteByte ( G0VST_L, 0x01 );
    mStar_WriteByte ( REGBK, REGBANKSCALER );
    #endif

    mStar_WriteByte(TRISTATE, 0xFF);

        mStar_WriteByte(OCTRL3, 0);

    mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) & ~WHTS_B);


    //#if PanelType!=PanelCLAA170WA02
    //#endif
    #if TurnOffOSD
        mStar_WriteOSDByte(OSDDBC, 0x5); // enable double
    #endif 
    
    PanelOffOnDelayCntr = 1200;
    
    bPanelDelayFlag = 1;
}

#else
void Power_TurnOffPanel(void)
{
    // 080123
    if(!PanelOnFlag)
        return;
	
 	hw_ClrBlacklit();
	ForceDelay1ms(30);
       #if IT6251
       IT6251_PowerDown();
       #endif

	mStar_WriteByte(OCTRL2, (mStar_ReadByte(OCTRL2)|BLKS_B));

    #if TurnOffOSD
    mStar_WriteOSDByte(OSDDBC, 0x0); // disable osd double buffer
    mStar_WriteOSDByte(IOSDC1, 0);
    mStar_WriteOSDByte(DBFC, 0x0); // disable double buffer
    //mStar_WriteByte( OCTRL2, mStar_ReadByte( OCTRL2 ) | BLKS_B );
    Delay4us();
    #endif 
    Clr_PanelOnFlag();
    ForceDelay1ms(PanelOffTiming1);

    #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
    mStar_WriteByte ( REGBK, REGBANKTCON );
    mStar_WriteByte ( G0VST_H, 0x01 );
    mStar_WriteByte ( G0VST_L, 0x01 );
    mStar_WriteByte ( REGBK, REGBANKSCALER );
    #else
    mStar_WriteByte(TRISTATE, 0xFF);
    #endif

   
    #if PanelRSDS
        sMute(0);
    #endif 
	
    #if !PanelTTL
        mStar_WriteByte(OCTRL3, 0);
    #endif 
	
    #if PanelLVDS
	#if(PanelType!=PanelM185BGE_L23)
    		ForceDelay1ms(PanelOffTiming2);
	#endif
    #endif
	

    hw_ClrPanel();
    //#endif
    #if TurnOffOSD
        mStar_WriteOSDByte(OSDDBC, 0x5); // enable double
    #endif 
/*	
    #if( ModelName==ENVISION_P2271w||ModelName==ENVISION_P2471w||ModelName==ENVISION_P971wl||ModelName == ENVISION_P2271wl||ModelName==ENVISION_P971l||ModelName==ENVISION_P971wl1)
     PanelOffOnDelayCntr = 1000;
	#elif (ModelName==AOC_e2237Fw||ModelName==AOC_2237Fw||ModelName==AOC_2037F||ModelName==AOC_e937Fw||ModelName==AOC_e2037F||\
		ModelName==AOC_2330V_PLUS||ModelName==AOC_2330V_14L||ModelName==AOC_e2037F1)
     PanelOffOnDelayCntr = 1500;
	#elif PanelType==PanelTPM200O1L02C1D
    PanelOffOnDelayCntr = 800;
	#elif ModelName==AOC_931Fwa
    PanelOffOnDelayCntr = 1800;
        #elif ModelName==AOC_931Fwa_TMS190
    PanelOffOnDelayCntr = 1800;
	#else
    PanelOffOnDelayCntr = 1200;
	#endif
    bPanelDelayFlag = 1;
	//*/
}
#endif
#endif
