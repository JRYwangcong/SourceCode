#include "types.h"
#include "board.h"
#include "global.h"
#include "power.h"
#include "debug.h"
#include "debugDef.h"
#include "ddc2bi.h"
#include "Menudef.h"
#include "ms_reg.h"

#if ENABLE_HDMI
#include "msHDMI.h"
#endif
#if ScanKey
#include "ScanKey.h"
#endif
#if MS_DAC
#include "msdac.h"
#endif
#include "DDC2Bi_Isr.h"
#if 0//USEFLASH
extern WORD xdata ucFlashSaveCount;
#endif

extern BYTE xdata DCRStepTransCnt; // 081205 dcr test

#define ReSyncTick          255//54 // 0.2 Second for resynchorize dubug process
#if UseINT
    #define EnableExt0Interrupt     1
#else
    #define EnableExt0Interrupt     0
#endif
#if 1//DDCCI_ENABLE
    #define EnableExt1Interrupt     1
    #define EnableSerialPortInterrupt   1 // 0
#else
    #define EnableExt1Interrupt     0
    #define EnableSerialPortInterrupt   1
#endif
#define EnableTime0Interrupt        1
#define EnableTime1Interrupt        0
#define EnableTime2Interrupt        0
#if EnableExt0Interrupt
    void EXT0Interrupt(void)interrupt 0
    {
        if (!SyncLossState())
        // && SrcInputType!=Input_Digital)
        {
            #if ENABLE_HDMI		
                #if MainBoardType != MainBoard_715G3603_M0A&& MainBoardType!=MainBoard_715G3663_M0A
            if (gScInfo.InputTmdsType == TMDS_HDMI)
            hw_SetMute(); // for changed mode have "po-po"
                #endif
            #endif

            if(!ProductModeFlag || SrcInputType!=Input_Digital)
        	{
            	hw_ClrBlacklit();
            	Set_InputTimingChangeFlag();
            	InputTimingStableCounter = 0;
            	SrcFlags |= SyncLoss;
		}
         }
        IE0 = 0;
    }
#endif
#if EnableTime0Interrupt
    extern WORD ms_Counter;
    extern xdata BYTE ReSyncCounter;
    //extern BYTE Second;
    extern BYTE  OsdCounter;
    extern BYTE xdata HotKeyCounter;
    extern BYTE xdata PowerDownCounter;
    void Time0Interrupt(void)interrupt 1
    {
        TH0 = ClockPeriod >> 8;
        TL0 = ClockPeriod &0xFF; // timer 1 counter
        ms_Counter--;

        if (ms10msCounter)
        ms10msCounter--;
		
        if(DCRStepTransCnt)
            DCRStepTransCnt --;

#if ScanKey
	if(TPDebunceCounter)
	{
		TPDebunceCounter--;
		if(TPDebunceCounter==0)
			Set_StartScanKeyFlag();
	}

    #ifdef EnableSlider
        if(FingerOnCounter)
            FingerOnCounter--;
    #endif
#endif

#ifdef ReduceDDC
        if(EnCheckVccFlag && (ms_Counter%DetectInterval) == 0)
        {
            if(hwSystemVcc_Pin)
            {
                SystemVccDropDownCnt = 0;
            }
            else
            {
                if(SystemVccDropDownCnt < 10)
                    SystemVccDropDownCnt ++;
                if(SystemVccDropDownCnt >= SystemShutDownCntr)
                {
                    hw_ClrBlacklit();
                    hw_ClrPanel();
                    #if !ScanKey
                    hw_ClrAmberLed();
                    hw_ClrGreenLed();
                    #endif
                }
            }
        }
#endif            

#if 0//DEBUG_ENABLE
	if(DebugOnlyFlag)
	{
  	TF0=0; // timer 1 flag
		return;
	}
#endif

        if (ReSyncCounter)
        {
            ReSyncCounter--;
        }
        if (ModeDetectCounter)
        {
            ModeDetectCounter--;
        }

	#if ENABLE_HDMI
	//if(wSwitchPortCounter)
	//	wSwitchPortCounter--;
    if(HdmiPollingCounter)
      HdmiPollingCounter--;

	if (gScInfo.InputTmdsType==TMDS_HDMI && (!(SrcFlags&(SyncLoss|bUnsupportMode))) )
	{
		if(gScInfo.wAviPktLossCnt<AviPktLossBound)
			gScInfo.wAviPktLossCnt++;
	}
	#endif


        if(bDelayFlag)
      {
         if(DelayCounter)
       {
            if(-- DelayCounter == 0)
                bDelayFlag = 0;
       }
      }
 
    if(bPanelDelayFlag)
        if(--PanelOffOnDelayCntr == 0)
            bPanelDelayFlag = 0;
    
 #if AutoProcess	
  //zjb20071218
  if(wAutoProcessCount)
	if(--wAutoProcessCount==0)
	{	wAutoProcess0++;
			if(MenuPageIndex==AutoMenu||MenuPageIndex==ResetMenu)
				wAutoProcessCount=PrintAutoMsgPeriod;
	}
#endif

    #if 0//USEFLASH
        if(ucFlashSaveCount)
            ucFlashSaveCount --;
    #endif

    #if MS_DAC
        if(ToggleGainCntr)
            if(--ToggleGainCntr == 0)
                bToggleGainFlag = 1;
    #endif

        if (ms_Counter == 0)
        {
            Set_SecondFlag();
            ms_Counter=SystemTick;
    #if Enable_Lightsensor
	#if LightSensor_Source == EverLight
            Set_DetectBriFlag();
	#endif
    #endif

        }
        
        if ((ms_Counter % 500) == 0)
        {
            Set_ms500Flag();
    #if Enable_Lightsensor
	#if LightSensor_Source == TAOS
            Set_DetectBriFlag();
	#endif
    #endif
        }

    #if Enable_Lightsensor
        if((ms_Counter % LightSensorStepCnt) == 0)
            bLightSensorActStepFlag = 1;
    #endif

    #ifdef LEDBrightCtrlByScalerPWM
        if((ms_Counter % PowerLEDBrightStepCnt) == 0)
            bPowerLEDBrightActStepFlag = 1;
    #endif

    #ifdef LEDFlash
        if(LEDFlashCounter)
            if(--LEDFlashCounter == 0)
                Set_LEDFlashChangeFlag();
    #endif

#ifdef PowerSavingAmberLEDFlashing
        if(PowerSavingFlag)
        {
	#if PSAmberLEDFlashingType == Type_Breathing
            if(ms_Counter%10 == 0)
            {
                if(LEDFlashCount)
                    if(--LEDFlashCount == 0)
                        bLEDFlashingActFlag = 1;
            }
	#else
        
            if(ms_Counter%100 == 0)
                LEDFlash100mSFlag = 1;
            
	#endif
        }
#endif

    #ifdef PowerSavingAmberLEDFlash
        if(PowerSavingFlag && PowerOnFlag)
        {
            if(ms_Counter%100 == 0)
                LEDFlash100mSFlag = 1;
        }
    #endif
	
    #ifdef OffPower
	#if MainBoardType==MainBoard_715G3598_M0A
     if(SingalXTALFlag&&PowerOnFlag) // edit by lizzie. for MainBoard_715G3598_M0A  when power saving,system runs at 14.318MHz,and the 1s counter will be half.
        {
            if(ms_Counter%500 == 0)
                OffPower1sFlag= 1;
        }
    else
	#endif
    	{
    	    if(ms_Counter%1000== 0)
                OffPower1sFlag= 1;
    	}
    #endif
	
	//#if(ModelName == ENVISION_P2271w || ModelName == ENVISION_P2471w||ModelName==ENVISION_P971wl)//|| ModelName==ENVISION_P2071l)
	#ifdef GreenLEDDutyCtrl
	  if(PowerOnFlag && !DuringResetFlag && !PowerSavingFlag) //090921:for LED  brightness
	  {
	  #if ModelName == ENVISION_P2271w
		  if(ms_Counter%10 < 2)		//Freq:100Hz Duty:20%
	  #else
 		  if(ms_Counter%10 < 5)	//Freq:100Hz Duty:50%
	  #endif
			  Set_SK_LEDVcc();
		  else						
			  Clr_SK_LEDVcc();
	  }   
	#endif

        TF0 = 0; // timer 1 flag
    }
#endif
#if EnableExt1Interrupt
    void EXT1Interrupt(void)interrupt 2
    {
        #if Backupdata
            Clr_GetChecksumFlag();
            DDCCIAdjCounter = 0;
        #endif
        #if 1//DDCCI_ENABLE
            //  EX1=0;
            if (ADC_INT_FLAG &(BIT3 | BIT4))
            {
                DoDDCCI(Input_Analog1);
            }
            #if Dual
                else if (DVI_INT_FLAG &(BIT3 | BIT4))
                {
                   DoDDCCI(Input_Digital);
                }
            #endif
			#ifdef TSUMW88MDT3_For_HDMIDDCCI
			else if  ( DVI1_INT_FLAG & ( BIT3 | BIT4 ) )
			{
				DoDDCCI(Input_Digital2);
			}
			#endif
            IE1 = 0;
            //  EX1=1;
        #endif
    }
#endif
#if EnableTime1Interrupt
    void Time1Interrupt(void)interrupt 3{}
#endif
#if EnableSerialPortInterrupt
    void SerialPortInterrupt(void)interrupt 4
    {
        BYTE sioData;
        if (RI)
        {
            RI = 0;
            sioData = SBUF;
            if (!RxBusyFlag)
            {
                //if (SIORxIndex<4)
                if (SIORxIndex < 8)
                {
                    if (SIORxIndex == 0)
                    {
                        ReSyncCounter = ReSyncTick;
                    }
                    SIORxBuffer[SIORxIndex++] = sioData;
                }
            }
        }
        if (TI)
        {
            TI = 0;
        }
    }
#endif
#if EnableTime2Interrupt
    void Time2Interrupt(void)interrupt 5
    {
        TF2 = 0;
    }
#endif
