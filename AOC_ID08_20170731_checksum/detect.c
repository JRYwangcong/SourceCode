#include <math.h>
#include "types.h"
#include "board.h"
#include "global.h"
#include "ms_reg.h"
#include "ms_rwreg.h"
#include "power.h"
#include "debug.h"
#include "misc.h"
#include "menu.h"
#include "UserPref.h"
#include "msOSD.h"
#include "mStar.h"
#include "Panel.h"
#include "adjust.h"
#if ENABLE_HDCP
    #include "MsHDCP.h"
#endif
#if MWEFunction
    #include "MsACE.h"
    #include "msdlc.h"
#endif

#if ENABLE_HDMI
#include "MsHDMI.h"
#endif

#ifdef ReduceDDC
#include "ddc.h"
#endif

#if MS_DAC
#include "msdac.h"
#endif

#if ScanKey
#include "scankey.h"
#endif

#ifdef TSUMXXT//#if( ChipID == ChipTSUMT )		//110311 Modify
#include "msADC.h"
#endif

#define HPeriod_Torlance    10 // for mode change
#define VTotal_Torlance     6 // 10 // for mode change

////////////////////////////////////////////////////////////////////////
#define SyncValidCount_VGA    15//((ProductModeFlag)?(6):(10))
#define SyncLossCount_VGA     70//((ProductModeFlag)?(12):(20))
#if ModelName==AOC_e2237Fw2||ModelName==AOC_e2237Fw
#define SyncValidCount_DVI     ((ProductModeFlag)?(15):(20))
#define SyncLossCount_DVI      ((ProductModeFlag)?(30):(60))
#define SyncValidCount_HDMI   ((ProductModeFlag)?(50):(50))
#define SyncLossCount_HDMI    ((ProductModeFlag)?(60):(170))
#else
#define SyncValidCount_DVI     ((ProductModeFlag)?(6):(20))
#define SyncLossCount_DVI      ((ProductModeFlag)?(12):(60))
#define SyncValidCount_HDMI   ((ProductModeFlag)?(20):(50))
#define SyncLossCount_HDMI    ((ProductModeFlag)?(30):(170))
#endif
////////////////////////////////////////////////////////////////////////

#if INPUT_TYPE == INPUT_1A1H
#define SyncValidCount_Digital  SyncValidCount_HDMI
#define SyncLossCount_Digital  SyncLossCount_HDMI
#elif INPUT_TYPE == INPUT_1A2D || INPUT_TYPE == INPUT_2A2D
#define SyncValidCount_Digital  ((SrcInputType>Input_Digital)?(SyncValidCount_HDMI):(SyncValidCount_DVI))
#define SyncLossCount_Digital  ((SrcInputType>Input_Digital)?(SyncLossCount_HDMI):(SyncLossCount_DVI))
#else
#define SyncValidCount_Digital  SyncValidCount_DVI
#define SyncLossCount_Digital  SyncLossCount_DVI
#endif

#define ValidTimingStableCount  ((SrcInputType<Input_Digital)?(SyncValidCount_VGA):(SyncValidCount_Digital))
#define SyncLossStableCount     ((SrcInputType<Input_Digital)?(SyncLossCount_VGA):(SyncLossCount_Digital))


#ifdef PanelTpye_1366// PanelWidth==1366
#define HVStartTorlance		((StandardModeGroup==Res_1360x768)?(5):(2))
#else
#define HVStartTorlance		2
#endif

#define HFreq_Torlance     12 // 15 // 1.5 KHz
#define VFreq_Torlance     12 // 15 // 1.5 Hz
#define VTotal_Delta        50
#define MaxInputHFreq      1000 // 955 //820 // 955 // 80.0 KHz
#define MinInputHFreq       150//240 // 150//150 for YPbPr



#define MaxInputVFreq      630//800// petit 20130130  865 //   865 //765 // 865 // 76.5 Hz
#define MinInputVFreq       400 // 410 //400 // 50.0 Hz

extern InputModeType code StandardMode[];
extern BYTE xdata InputTimingStableCounter;
extern BYTE xdata TimingChangeDebunce;
Bool mStar_FindMode(void);
BYTE mStar_GetInputStatus(void);
Bool IsCableNotConnected(void);
Bool mStar_SyncLossStateDetect(void);
Bool mStar_ValidTimingDetect(void);

#if findmodebyVwidth
BYTE GetVsyncWidth(void);
#endif

#if EnableAutoDVI
void mStar_AutoDVI(void);
BYTE code DVIPhaseTbl[];
BYTE mStar_FineTuneDVIPhase(BYTE channelReg, BYTE channel);
#endif

// 071221
#if (ModelName == AOC_2230FH || MainBoardType==MainBoard_715G3603_M0A || \
	MainBoardType==MainBoard_715G3663_M0A)

#if ModelName == AOC_2230FH
#define SARDetectValue   (KEYPAD_ADC1&0x3f)
#elif MainBoardType==MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
#define SARDetectValue   (KEYPAD_ADC4&0x3f)
#endif

#define justDVI 39
#define justHDMI 24
#define DVIandHDMI 18
#define DigitalNoConnect 63

BYTE CheckDigitalCable(void)
{
    BYTE temp, temp1, retry_Key;
    BYTE status = DigitalNoConnect;

        retry_Key = 3;
        while(retry_Key)
        {
            temp = SARDetectValue;
            ForceDelay1ms(3);
            temp1 = SARDetectValue;
            if(abs(temp-temp1) < 5)
                break;
            retry_Key--;
        }

        if(temp <= 20)
            status = DVIandHDMI;
        else if(temp <= 30)
            status = justHDMI;
        else if(temp <= 50)
            status =  justDVI;
        
        
#if 0//DEBUG_ENABLE
    if(temp < 60)
        printData("ADC1:%d", temp);
#endif

        return status;

}

#endif

//*******************************************************************
// Function Name: mStar_MonitorInputTiming
//
// Decscription: Detect input timing
//
// callee: mStar_SyncLossStateDetect(), mStar_ValidTimingDetect(), mStar_AutoDVI() in detect.c
//         mStar_SetInputPort(), mStar_SetAnalogInputPort() in mstar.c
//
// caller: main() in main.c
//*******************************************************************
void mStar_MonitorInputTiming(void)
{
    if (!PowerOnFlag || DisplayLogoFlag)
    {
        return ;
    }
    
    // detect whether input signal is changing
    if (!InputTimingChangeFlag)
    {
        if (SyncLossState())
        {
            if (!FreeRunModeFlag)
            {
                Set_InputTimingChangeFlag();
            }
            else if (mStar_SyncLossStateDetect())
            // detect mode while no Sync/suspend/standby/disconnect
            {
                Set_InputTimingChangeFlag();
				//SwitchPortCntr = 0; // 100125 coding
            }
#if Dual
            else if((InputTimingStableCounter > 0) && (SwitchPortCntr > Input_Nums) && // 100125 coding
                       (UserPrefInputSelectType == InputSelectType_Auto))
            {
            
    // 090520 add for speed up switch port        
    #if ModelName == AOC_2230FH || MainBoardType==MainBoard_715G3603_M0A ||\
	   MainBoardType==MainBoard_715G3663_M0A
                if(!ProductModeFlag && SrcInputType > Input_Analog1 && SyncLossState())
                {
                    if(InputTimingStableCounter >= SyncLossStableCount/4)
                    {
                        BYTE cablestatus = CheckDigitalCable();
                        if((SrcInputType==Input_Digital && (cablestatus==justHDMI || cablestatus==DigitalNoConnect))||
                            (SrcInputType==Input_Digital2 && (cablestatus==justDVI || cablestatus==DigitalNoConnect)))
                            InputTimingStableCounter = SyncLossStableCount;
                    }
                }
    #endif
            
                if((InputTimingStableCounter % SyncLossStableCount) == 0)
                {
                    InputTimingStableCounter = 1; // 071224
                    SrcInputType = (SrcInputType + 1) % Input_Nums;
                    mStar_SetupInputPort();
                }
            }
#endif            
        }
        else
        // valid timing
        {
            // check if input timing is changing
            if (mStar_ValidTimingDetect())
            {
                SrcFlags |= SyncLoss;
                Set_InputTimingChangeFlag();
                hw_ClrBlacklit(); // 2006/9/7 8:39PM by Emily for changed mode had garbage
            }
        }
    }
    //=============================================================================
    //=============================================================================
    if (InputTimingChangeFlag)
    // input timing is unstable
    {
        #if MWEFunction
            if (g_bAccOnOff)
            {
                //UserPrefMWE=OFF_W;
                //msAccSetup(UserPrefMWE);
                msAccOnOff(0);
            }
        #endif
        // 2006/7/6 0:8AM by Emily for dec. dot clock
        Clr_DecVScaleFlag();
        VScaleDecValue = 0;
        Clr_InputTimingChangeFlag();
        Clr_DoModeSettingFlag();
        Clr_ReadyForChangeFlag();
        // wumaozhong 20050426  modify for enter powersaving down and DVI no Picture
        #if !Dual
            Clr_InputTimingStableFlag(); // reset input timing stable and wait for timing stable
            InputTimingStableCounter = 0;
        #endif
        TimingChangeDebunce++;
        if (!SyncLossState())
        // && !InputTimingStableFlag) // prevent from SOG
        {
            if (TimingChangeDebunce <= 1)
            {
                SrcFlags |= SyncLoss;
                return ;
            }
        }
        mStar_PrepareForTimingChange();
    }
    else
    // input timing is stable
    {
        TimingChangeDebunce = 0;
        InputTimingStableCounter++; // counter timing stable times
        if(InputTimingStableCounter == 0)
            InputTimingStableCounter = 1;
        if (!InputTimingStableFlag)
        {

    // 090520 add for speed up switch port        
    #if ModelName == AOC_2230FH || MainBoardType==MainBoard_715G3603_M0A ||\
	   MainBoardType==MainBoard_715G3663_M0A
            if(!ProductModeFlag && SrcInputType > Input_Analog1 && SyncLossState())
            {
                if(InputTimingStableCounter >= SyncLossStableCount/4)
                {
                    BYTE cablestatus = CheckDigitalCable();
                    if((SrcInputType==Input_Digital && (cablestatus==justHDMI || cablestatus==DigitalNoConnect))||
                        (SrcInputType==Input_Digital2 && (cablestatus==justDVI || cablestatus==DigitalNoConnect)))
                        InputTimingStableCounter = SyncLossStableCount;
                }
            }
    #endif
        
            if((!SyncLossState() && InputTimingStableCounter >= ValidTimingStableCount) ||
                (SyncLossState() && InputTimingStableCounter >= SyncLossStableCount))
            {
                Set_InputTimingStableFlag(); // set flag to search for input mode
                Set_DoModeSettingFlag();
#if Dual
#if 0//DEBUG_ENABLE
    if(!SyncLossState())
        printData("ValidCntr:%d", InputTimingStableCounter);
    else
        printData("LossCntr:%d", InputTimingStableCounter);
    printData("SrcInputType:%d", SrcInputType);
#endif

                InputTimingStableCounter = 1; // 071224
                if(!SyncLossState())
                    SwitchPortCntr = 0;

#if ScanKey // led key off 071227 to keep led on till power saving
    #if Scan_Source == Synaptics
                if(SyncLossState())
                {
                    KeyLedCounter = 0;
                    Clr_keyLEDTurnOffFlag();
                }
    #endif                
#endif
                
                if(SyncLossState() && (UserPrefInputSelectType == InputSelectType_Auto) && 
                    (++SwitchPortCntr <= Input_Nums)) // 100125 coding
                {
    
                    Clr_InputTimingStableFlag(); // set flag to search for input mode
                    Clr_DoModeSettingFlag();
                    SrcInputType = (SrcInputType + 1) % Input_Nums;
                    mStar_SetupInputPort();

    #if 0//DEBUG_ENABLE
        printData("SwitchPortCntr:%d", SwitchPortCntr);
    #endif
                    
                }

#endif                
            }

            }
    }
}
//*******************************************************************
// Function Name: mStar_ModeHandler
//
// Decscription: Programming scaler while input timing is changing
//
// callee: mStar_SyncLossStateDetect(), mStar_ValidTimingDetect(), mStar_AutoDVI() in detect.c
//         mStar_SetInputPort(), mStar_SetAnalogInputPort() in mstar.c
//
// caller: main() in main.c
//*******************************************************************
void mStar_ModeHandler(void)
{
    if (DisplayLogoFlag)
    {
        Menu_InitAction();
    }
    if (!DoModeSettingFlag)
    // while timing change & stable
    {
        return ;
    }
    //Clr_DoModeSettingFlag();  //0606 for Diaplay LOGO
    if (!SyncLossState())
    // no sync
    {
        if (!ReadyForChangeFlag)
        {
            if(SrcInputType==Input_Digital || SrcInputType==Input_Digital2)
                CheckADC();
            if (!mStar_FindMode())
            // search mode index
            {
                Set_InputTimingChangeFlag(); // search mode faile
                return ;
            }
        }
    		if(StandardModeVFreq>760||StandardModeVFreq<400||StandardModeHFreq>950||StandardModeHFreq<220)//if (!UnsupportedModeFlag)
		//supported mode
		;
	else
        //supported mode
        {

            if (!ReadyForChangeFlag)
            {
                ReadModeSetting(); // restore user's settings from NVRAM for each mode
                Set_ReadyForChangeFlag();
            }
            #if 1
                if (DisplayLogoFlag)
                {
                    return ;
                }
            #endif
            if (mStar_SetupMode() == FALSE)
            // setup scaler
            {
                Set_InputTimingChangeFlag(); // setup mode failed
                mStar_SetupFreeRunMode(); // setup freerun mode
                SrcFlags|=SyncLoss;//edit by yz.nie .for DCR ON,change timing quickly have white screen.
                return ;
            }
            if (UnsupportedModeFlag){}
        }

    }
        Set_ReadyForChangeFlag();
    if (DisplayLogoFlag)
    {
        return ;
    }

		if (SrcModeIndex==PRESET_1600x1200_60Hz_VESA||
				SrcModeIndex==PRESET_1600x1200_65Hz_VESA||
				SrcModeIndex==PRESET_1600x1200_70Hz_VESA||
				SrcModeIndex==PRESET_1600x1200_75Hz_VESA)
			{
				SrcFlags |= bUnsupportMode;
				//Set_DLKOverFlag();
				
			}
	
	else if (SyncLossState() || (UnsupportedModeFlag&&((StandardModeVFreq>760||StandardModeVFreq<400||StandardModeHFreq>950||StandardModeHFreq<220))))
    {
        mStar_SetupFreeRunMode(); // setup freerun mode
        //      Power_TurnOffGreenLed(); // turn off green led
        //      Power_TurnOnAmberLed(); // turn on amber led
       			if (PowerSavingFlag)
				{
					Power_TurnOnAmberLed();
				}
        // printMsg("Set Free Run 1");
    }
    else
    {
        WORD vfreq, hfreq;
        hfreq = HFreq(SrcHPeriod);
        vfreq = VFreq(hfreq, SrcVTotal);
        if(SrcFlags&bInterlaceMode)            
        {
            vfreq *= 2;
        }
    #if BURST_MODE
    {

			BYTE i=4;
			while((vfreq*i)/10<220)//set pwm freq.>=220
			{
				i++;
			}
			SetPWMFreq((vfreq*4)/10);
    }
    #endif
	#if 1//petit 20130130
        if (hfreq > MaxInputHFreq || hfreq < MinInputHFreq || vfreq > MaxInputVFreq || vfreq < MinInputVFreq)
        {
            SrcFlags |= bUnsupportMode;
            //printMsg("input timing is out of range");
        }
	#endif
	 #if 1//20160123  lixw add
	 if( StandardModeHeight <= 400 || ( StandardModeHeight == 480 && ( vfreq > 575 && vfreq < 770) && SrcModeIndex != 0 ) )
        {
            UserPrefAutoTimes = 1;
        }
	 #endif
        #if 0//PanelHeight==768
		if (ShrinkModeFlag || vfreq>761 || vfreq<548 ||
			(StandardModeHeight==768 && (vfreq>761 && hfreq>613)) ||
			(StandardModeHeight<=480 && (vfreq<625 && hfreq<288)))
            {
                SrcFlags |= bUnsupportMode;
            }
        #else
#if 0
		if (ShrinkModeFlag || vfreq>765 || vfreq< MinInputVFreq ||
			(StandardModeHeight==1050 && (vfreq>758 && hfreq>834)) ||
			(StandardModeHeight<=480 && (vfreq<625 && hfreq<296)))
            {
                SrcFlags |= bUnsupportMode;
            }

#else
     #if (ModelName==AOC_2237Fw)
	if ( hfreq > (830+HFreq_Torlance) || vfreq>(750+VFreq_Torlance) )
  	{
	   	SrcFlags |= bUnsupportMode;
  	}
	else if (hfreq<(300-HFreq_Torlance) ||vfreq < (500-VFreq_Torlance) )
	{
		SrcFlags |= bUnsupportMode;
	}
     #else
	if ( hfreq > MaxInputHFreq || vfreq>765 )
  	{
	   	SrcFlags |= bUnsupportMode;
  	}
	else if (hfreq<MinInputHFreq ||vfreq < MinInputVFreq )
	{
		if (SrcInputType != Input_Analog1 )//&& (SrcFlags&bInterlaceMode))   //charles
			SrcFlags &= !bUnsupportMode;
		else
			SrcFlags |= bUnsupportMode;
	}
      #endif
	#if 0//(ModelName!=AOC_716Sw)
	
	else if (StandardModeHeight>PanelHeight)
	{
		/*
		if ((StandardModeHeight==1080) && (StandardModeWidth==1920) && (SrcInputType != Input_Analog1))
			SrcFlags &= !bUnsupportMode;
		else
			SrcFlags |= bUnsupportMode;
		*/
	#if (ModelName==AOC_917Fwh)
	      if (!(StandardModeGroup == Res_1920x1080||StandardModeGroup == Res_1280x1024||StandardModeGroup == Res_1280x960))
	#else
		if (!(StandardModeGroup == Res_1920x1080))
	#endif
			SrcFlags |= bUnsupportMode;
	}
	#else
  #if (ModelName==ENVISION_G2217||ModelName==ENVISION_G2219||ModelName==AOC_2219P2||ModelName==ENVISION_G2219_LM220WE1)// cy 1121 test //Amy2007.12.18   13:50:01
            mStar_WriteByte(0x5C,(mStar_ReadByte(0x5C)&~BIT0));
            if(StandardModeGroup == Res_1680x1050)
                {
                     mStar_WriteByte(0x00, 0x00);
                     mStar_WriteByte(0x5C,(mStar_ReadByte(0x5C)|BIT0));
                     if(abs(vfreq-750)<15)
                        SrcFlags |= bUnsupportMode;
                }
            #endif

	#endif
	
#endif
        #endif

/*
	if (StandardModeHeight<=400 ||
	  (StandardModeHeight==480 &&(StandardModeGroup!=Res_848x480)))//))//101215 Modify
        {
            UserPrefAutoTimes = 1;
        }
	//*/

	

        if (PowerOnFlag)
        // system is on power-off status
        {
          		if (FactoryModeFlag||BurninModeFlag)
			{
				Power_TurnOnAmberLed();
			}
			else
			{
				Power_TurnOnGreenLed();
			}// turn on green led
        }
		//Set_ShowInputInfoFlag();//071226
    }

    #ifdef BrightFreqByVfreq
    if(FreeRunModeFlag)
    {
        // free run set to 60hz*4
        mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT)|PWM0BIT8); // Brightness freq.
        mStar_WriteByte(PWMDIV0, 0xd1); // Brightness freq.
    }
    else
    {
        WORD vfreq, hfreq;
        hfreq = HFreq(SrcHPeriod);
        vfreq = VFreq(hfreq, SrcVTotal);
        if (SrcFlags&bInterlaceMode)
        {
            vfreq *= 2;
        }

        // calculate brightness freq.
        hfreq = 111859/(vfreq*4/10);
        if(hfreq > 0x100)
        {
            hfreq -= 0x100;
            mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT)|PWM0BIT8); // Brightness freq.
            mStar_WriteByte(PWMDIV0, hfreq-1); // Brightness freq.
        }
        else
        {
            mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) & ~PWM0BIT8); // Brightness freq.
            mStar_WriteByte(PWMDIV0, hfreq-1); // Brightness freq.
        }
    }
    #endif
    #if BURST_MODE
	if(FreeRunModeFlag)	//110715 Modify
	{
		#if 0//EnablePanelServiceMenu
		SetPWMFreq(PanelBurstModeFreq);
		#else
		SetPWMFreq(250);		
		#endif
	}
	else
    {
        WORD vfreq, hfreq;
        hfreq = HFreq(SrcHPeriod);
        vfreq = VFreq(hfreq, SrcVTotal);
		if(abs(vfreq-500)<10)//when Input Vfreq=50Hz,Set Brightness freq to 250Hz
			SetPWMFreq(250);		
		else
			SetPWMFreq((vfreq*4)/10);
    }
    #endif
	
    #if 1
        if (!CableNotConnectedFlag || !SyncLossState())
        {
            if (DoBurninModeFlag)
            {
            	mStar_WriteByte(FWC, 0);//petit 20121122
	    #if 0		
            if(!ProductModeFlag)
            	{
                //Clr_DoBurninModeFlag();
                mStar_WriteByte(FWC, 0); // disable background color function.
            	}
	    #endif
            }
        }
    #endif
    
    Clr_DoModeSettingFlag(); //0606

    TermBuffer = StandardModeGroup;

    #if ENABLE_HDMI
    if(!SyncLossState() && mstar_HDMITmdsGetType()==TMDS_HDMI && SrcInputType>=Input_Digital)
    {
        BYTE ucCount=200;

        while(ucCount--)
        {
            gScInfo.InputAVIInfoReceived=0;
            mstar_HDMIGetPktStatus();
            if(gScInfo.InputAVIInfoReceived)
            {
                gScInfo.InputColor=mstar_HDMIPacketColor();
                gScInfo.ColorChanged=1;
                mstar_HDMIHandleColor();
                break;
            }
            Delay1ms(20);
        }
        gScInfo.OutputDataState=HDMI_DATA_OUTPUT_ON;
        mstar_HDMICheckAudio();

	if( TermBuffer == Res_1920x1080 || TermBuffer == Res_1280x720)
	{
		msSetColorFomat( HDTV  );
	}
	
	else //( TermBuffer == Res_720x576	|| TermBuffer == Res_720x483	|| TermBuffer == Res_720x480)
	{
		msSetColorFomat( SDTV );
	}
		
    }
	
	#if MS_DAC
        if (gScInfo.InputTmdsType ==TMDS_HDMI && SrcInputType>=Input_Digital) 
        {
            // dac out to line out: 550mVrms
            msAudioLineout_SourceSel(E_LINEOUT_FROM_DAC);	
            msAudioLineout_SetVolume(E_AUDIO_LEFT_RIGHT_CH, 0x10);
            msAudioLineOutGain(0x28);
        }
        else // line in to line out: 550mVrms
        {
            msAudioLineout_SourceSel(E_LINEOUT_FROM_LINEIN0);
            msAudioLineOutGain(0x2b);
        }
        msAudioEARout_MuteOnOff(FALSE);
	#else		
        if (gScInfo.InputTmdsType ==TMDS_HDMI && SrcInputType>=Input_Digital)
            hw_Audio_HDMI();
        else
            hw_Audio_PC();
	#endif

	#if DEBUG_HDMI
  	printData("gScInfo.InputTmdsType=%d",gScInfo.InputTmdsType);
  	#endif

       #else // !ENABLE_HDMI 
        {
            // prevent complier warning.
            BYTE temp = 0;
            if(temp)
				#ifdef TSUMXXT	//110216 for compile error and follow dell		//110311 Modify
				;
				#else
                msSetColorFomat( SDTV );
				#endif
        }
	#endif

    Power_TurnOnPanel(); // turn on panel
    
    #if ENABLE_HDMI||AudioFunc
	//if(!FreeRunModeFlag)
        mStar_AdjustVolume(UserPrefVolume);
    #endif
    
    #if Backupdata
        Clr_ACONFlag();
    #endif
    Menu_InitAction(); // initial menu osd state
}
void mStar_PrepareForTimingChange(void)
{
#if ENABLE_HDMI
    #if EN_HDMI_NEW_PIXEL_REPETITION
    if (gScInfo.InputTmdsType == TMDS_HDMI)
        mstar_HDMIAudioMute(HDMI_AUDIO_OFF);

        #if DEBUG_TIMEINGCHANGE
        printData("mStar_PrepareForTimingChange",1);
        #endif
    #else
	BYTE bank;
	//patch
    if (gScInfo.InputTmdsType == TMDS_HDMI)
        mstar_HDMIAudioMute(HDMI_AUDIO_OFF);

    #if DEBUG_TIMEINGCHANGE
    printData("mStar_PrepareForTimingChange",1);
    #endif
	if((mstar_HDMITmdsGetType()==TMDS_HDMI)&&(mstar_HDMIPacketColor()==INPUT_YUV422) &&
						(StandardModeGroup == Res_720x480 || StandardModeGroup == Res_720x576))
	//if((SrcInputType < Input_Digital )||((mstar_HDMITmdsGetType()==TMDS_HDMI)&&(mstar_HDMIPacketColor()==INPUT_YUV444) &&
	//					(StandardModeGroup == Res_720x480 || StandardModeGroup == Res_720x576)))
    {
		bank=mStar_ReadByte(0);  // jacky
		mStar_WriteByte(0, 5);
		mStar_WriteByte(0x9,0x00);
		ForceDelay1ms(30);	//100303 : for VGA 480i 576i 
		mStar_WriteByte(0, bank);
    }
	else
	{
		bank=mStar_ReadByte(0);  // jacky
		mStar_WriteByte(0, 5);
		mStar_WriteByte(0x9,0x04);
		ForceDelay1ms(30);	//100303 : for VGA 480i 576i 
		mStar_WriteByte(0, bank);
    }
    #endif    
	//if(SyncLossState())
    {
        mstar_HDMIInitialVariable();
		mstar_HDMIResetColorSpace();
		mstar_HDMIRst();
		//ForceDelay1ms(30);
	}
	#endif

    #if ScanKey
        #ifdef LEDBrightCtrlByScalerPWM
            #if 0 // 091020 coding reserved, GPM request
            SKLEDStatus = SKLEDBright_Light_Immediately;
            ScanKey_LEDBrightAction();
            #endif
        #endif
    #endif

    Clr_ForcePowerSavingFlag();
    PowerDownCounter = 0;
    if (!DisplayLogoFlag)
    {
        if (PowerSavingFlag)
        {
            Power_PowerOnSystem();
            mStar_SetupFreeRunMode();
            SwitchPortCntr = 0; // 071226
            
		#ifdef PowerSavingAmberLEDFlashing
			#if PSAmberLEDFlashingType
				#if MainBoardType == MainBoard_715G3329_A2 || MainBoardType == MainBoard_715G3244_TSUMU18NR|| MainBoardType ==MainBoard_712G3244_2 || MainBoardType == MainBoard_715G3959_T || MainBoardType == MainBoard_715G3834||MainBoardType ==MainBoard_3834_18T
							InitialP06ToPWM1(FALSE);
				#endif	   
			#else
							Set_SK_LEDVcc();
			#endif
		#endif
        }
        else
        {
            //if (PanelOnFlag)
            mStar_SetupFreeRunMode();
            Power_TurnOffPanel();
        }
        Osd_Hide();
    }
    #if UseINT
        mStar_WriteByte(INTENA, 0);
		mStar_WriteByte(INTENB, 0);	//110311 Modify
    #endif

    if (InputTimingStableFlag)
    {
        #if ENABLE_HDCP
            if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2 )
            mStar_WriteByte(SWRST, GPR_B); // Reset Graphic port to re-counter input hfreq & vtotal
        else
            mStar_WriteByte(SWRST, GPR_B | ADCR_B); // Reset Graphic port to re-counter input hfreq & vtotal
        #else
            mStar_WriteByte(SWRST, GPR_B | ADCR_B); // Reset Graphic port to re-counter input hfreq & vtotal
        #endif
        Delay1ms(3);
        mStar_WriteByte(SWRST, 0);
        Delay1ms(20);
        if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2)
        {
            #if ENABLE_HDCP
                mStar_WriteByte(0xFB, 0x00);
            #endif
#if EnableAutoDVI
            mStar_AutoDVI();
#endif
        }
    }
    #if 0//ENABLE_HDMI
    else
    {
        if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2)    // Slove DENON DVD Player DE Data Error
        {
            if (bDigitalGraphicReset)
            {
                bDigitalGraphicReset=0;
                mStar_WriteByte(SWRST, GPR_B);  // Reset Graphic port to re-counter input hfreq & vtotal
                ForceDelay1ms( 3 );
                mStar_WriteByte( SWRST, 0 );
                ForceDelay1ms( 80 );
                #if DEBUG_DE
                printData("2222222222222",1);
                #endif
            }
        }
    }
    #endif

    // wumaozhong 20050426  modify for enter powersaving down
    #if Dual
        Clr_InputTimingStableFlag(); // reset input timing stable and wait for timing stable
        InputTimingStableCounter = 0;
    #endif
    SrcFlags &= ~(bUnsupportMode | bUserMode | bNativeMode);
    mStar_WriteByte(DBFC, 0x00); // enable double bufer.
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(DBFC, 0x00); // enable ADC's double bufer.
    mStar_WriteByte(REGBK, REGBANKSCALER);
}
//==========================================================
#if EnableAutoDVI
BYTE GetBestBandwidth(BYTE channel, BYTE *errA)
{
    BYTE errB;
    mStar_WriteByte(TESTEN, channel | ERRD_B); // set test channed
    mStar_WriteByte(TESTEN, channel | TSTEN_B); // set test channed
    mStar_WriteByte(TESTA0, 0x20); // set high bandwidth
    mStar_WriteByte(TESTEN, channel); // set test channed
    Delay1ms(1);
    mStar_WriteByte(TESTEN, channel | ERRD_B); // set test channed
    *errA = mStar_ReadByte(DVI_ERST);
    mStar_WriteByte(TESTEN, channel | TSTEN_B); // set test channed
    #if ChipID>=ChipAD
        mStar_WriteByte(TESTA0, 0x08); // set medium bandwidth
    #else
        mStar_WriteByte(TESTA0, 0x00); // set medium bandwidth
    #endif
    mStar_WriteByte(TESTEN, channel); // set test channed
    Delay1ms(1);
    mStar_WriteByte(TESTEN, channel | ERRD_B); // set test channed
    errB = mStar_ReadByte(DVI_ERST);
    mStar_WriteByte(TESTEN, channel); // set test channed
    #if ChipID>=ChipAD
        if (*errA > errB)
        {
            *errA = errB;
            return 0x8;
        }
    #else
        if (*errA > errB)
        {
            *errA = errB;
            return 0x0;
        }
    #endif
    return 0x20;
}
BYTE mStar_FineTuneBandwidth(void)
{
    BYTE errRed, bwRed;
    BYTE errGreen, bwGreen;
    BYTE errBlue, bwBlue;
    bwRed = GetBestBandwidth(RED_CHANNEL, &errRed);
    bwGreen = GetBestBandwidth(GREEN_CHANNEL, &errGreen);
    bwBlue = GetBestBandwidth(BLUE_CHANNEL, &errBlue);
    if (errRed > errGreen)
    {
        if (errGreen > errBlue)
        {
            return bwBlue;
        }
        else
        {
            return bwGreen;
        }
    }
    else
    {
        if (errRed > errBlue)
        {
            return bwBlue;
        }
        else
        {
            return bwRed;
        }
    }
}
BYTE code DVIPhaseTbl[] =
{// 0     1     2     3     4     5     6     7     8     9
  0x40, 0x01, 0x03, 0x07, 0x0F, 0x17, 0x13, 0x11, 0x10, 0x31,
  0x33, 0x37, 0x3F, 0x27, 0x23, 0x21, 0x20, 0x61, 0x63, 0x67,
  0x6F, 0x77, 0x73, 0x71, 0x70, 0x51, 0x53, 0x57, 0x5F, 0x47,
  0x43, 0x41
};
BYTE code DVIPhaseTbl2[] =
{
    // 0     4     8     12    16    20    24    28
    0x00, 0x1F, 0x30, 0x2F, 0x60, 0x7F, 0x50, 0x4F
};
BYTE GetLinearPhaseValue(BYTE value)
{
    BYTE i;
    for (i = 0; i < 0x20; i++)
    {
        if (DVIPhaseTbl[i] == value)
        {
            break;
        }
    }
    if (i >= 0x20)
    {
        for (i = 0; i < 8; i++)
        {
            if (DVIPhaseTbl2[i] == value)
            {
                break;
            }
        }
        if (i >= 8)
        {
            i = 0;
        }
        else
        {
            i = i * 4;
        }
    }
    return i;
}
BYTE mStar_FineTuneDVIPhase(BYTE channelReg, BYTE channel)
{
    BYTE phX, phY, phZ;
    BYTE errX, errY, errZ;
    mStar_WriteByte(TESTEN, channel | ERRD_B); // set test channed
    mStar_WriteByte(TESTEN, channel); // set test channed
    Delay1ms(1);
    mStar_WriteByte(TESTEN, channel | ERRD_B); // set test channed
    errX = mStar_ReadByte(DVI_ERST); // read error status X
    if (errX == 0)
    {
        return 0xFF;
    }
    phX = mStar_ReadByte(channelReg); // read phase X
    phX = GetLinearPhaseValue(phX); // convert to linear Phase;
    mStar_WriteByte(channelReg, DVIPhaseTbl[(phX + 8) % 0x20]); // write linear phase
    mStar_WriteByte(TESTEN, channel); // set test channed
    Delay1ms(1);
    mStar_WriteByte(TESTEN, channel | ERRD_B); // set test channed
    errY = mStar_ReadByte(DVI_ERST); // read error status X
    if (errY == 0)
    {
        return 0xFF;
    }
    phY = mStar_ReadByte(channelReg); // read phase X
    phY = GetLinearPhaseValue(phY); // convert to linear Phase;
    mStar_WriteByte(channelReg, DVIPhaseTbl[(phY + 8) % 0x20]); // write linear phase
    mStar_WriteByte(TESTEN, channel); // set test channed
    Delay1ms(1);
    mStar_WriteByte(TESTEN, channel | ERRD_B); // set test channed
    errZ = mStar_ReadByte(DVI_ERST); // read error status X
    phZ = mStar_ReadByte(channelReg); // read phase X
    phZ = GetLinearPhaseValue(phZ); // convert to linear Phase;
    mStar_WriteByte(TESTEN, channel); // set test channed
    if (errX > errY)
    {
        if (errY > errZ)
        {
            return phZ;
        }
        else
        {
            return phY;
        }
    }
    else
    // X<Y
    {
        if (errX > errZ)
        {
            return phZ;
        }
        else
        {
            return phX;
        }
    }
}
void mStar_AutoDVI(void)
{
    BYTE checkTimes = 16;
    BYTE rtValue = 0x10;
    BYTE regValue;
    mStar_WriteByte(REGBK, REGBANKADC);
    // auto-setting resistor impendence
    #if DVI_PN_SWAP
    mStar_WriteByte(RT_CTL, BIT7|rtValue);//BIT7 DVI P/N swap
    #else
    mStar_WriteByte(RT_CTL, rtValue);
    #endif
    /*
    mStar_WriteByte( TESTEN, 0x00 ); // normal status
    mStar_WriteByte( TESTEN, RDST_B ); // freeze/read status
    while ( checkTimes-- )
    {
    mStar_WriteByte( TESTEN, 0x00 );
    Delay1ms( 1 );
    mStar_WriteByte( TESTEN, RDST_B ); // freeze/read status
    regValue = mStar_ReadByte( RT_STATUS2 );
    if ( regValue & BIT7 )
    {
    rtValue++;
    }
    else
    {
    rtValue--;
    }
    rtValue &= 0x1F;
    mStar_WriteByte( RT_CTL, rtValue );
    }
    mStar_WriteByte( TESTEN, 0x00 ); // normal status
     */
    // set best bandwidth
    regValue = mStar_FineTuneBandwidth();
    mStar_WriteByte(TESTEN, TSTEN_B);
    mStar_WriteByte(TESTA0, regValue);
    mStar_WriteByte(TESTEN, 0);
    mStar_WriteByte(REGBK, REGBANKSCALER);
}
#endif
Bool mStar_SyncLossStateDetect(void)
{
    BYTE fStatus;
    Bool result = FALSE;
#if DisplayLogo==NoBrand
    //EDID program when cable not connected
    //if ( SyncLossState() && CableNotConnectedFlag ) //2006-02-14
    if (SyncLossState() && IsCableNotConnected()&&!ProductModeFlag)//add !ProductModeFlag For PE request 
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
            hw_SetDDC_WP();
        #endif 
    } 
#endif
    fStatus = mStar_GetInputStatus();
    // printData("status %xH", fStatus);//mStar_ReadByte(INSTA));
    // printData("stable counter %d", InputTimingStableCounter);
    if (fStatus &SyncLoss)
    {
        if (SrcInputType < Input_Digital)
        {
            if(InputTimingStableCounter%ValidTimingStableCount == 0)
                mStar_SetAnalogInputPort();
            fStatus=mStar_GetInputStatus();
            if(!(fStatus&SyncLoss))
                goto SyncActive;
        }
        if (CableNotConnectedFlag)
        // no Cable
        {
            if (!IsCableNotConnected())
            {
                // 091009 coding, add condition for waking up from power saving, comfirmed to Mark
                if(PowerSavingFlag)
                {
                    if(!(fStatus&SyncLoss))
                    {
                        Clr_CableNotConnectedFlag();
			  if (!ProductModeFlag)
                        result = TRUE;
                    }
                }
                else
                {
                    Clr_CableNotConnectedFlag();
					#if Dual
					SwitchPortCntr=0;
					#endif
                    result = TRUE;
                }
            }
        }
        else if (IsCableNotConnected())
        // no Sync
        {
            Set_CableNotConnectedFlag();


            result = TRUE;
            
        }
        if (PowerSavingFlag)
        {
            if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2 )
            {
                if (!(mStar_ReadByte(INTCTRL) &0x40) )//&& !(mStar_ReadByte(STATUS2) &0xF0))
                {
                    //Delay1ms(150);
                    Delay1ms(50);
                    mStar_PowerUp();
                    Delay1ms(50);
                    fStatus = mStar_GetInputStatus();
                    if (fStatus &SyncLoss)
                    {
                        mStar_PowerDown();
                    }
                    else
                    {
                        result = TRUE;
                    }
                }
            }
            /********** wmz 20051117
            else if (SrcInputType==Input_YPbPr && mStar_ReadByte(STATUS2)&0x50)
            { if (abs(HFreq(mStar_ReadWord(HSPRD_H))-156)<20)
            { fStatus&=~SyncLoss;
            result=TRUE;
            }
            }
             ***************/
        }
    }
    else
    {
        SyncActive: result = TRUE;
    }
    if (result)
    {
        SrcFlags = (SrcFlags &0xF0) | fStatus;
    } // clear others flags, except sync informat (polarity & w/o)
    return result;
}
#if Enable_Expansion
    Bool mStar_IsStatusChanged(void)
    {
        // prevent input status change when setup mode.
        if (mStar_ReadByte(STATUS2) &INTM_B)
        {
            if (!(SrcFlags &bInterlaceMode))
            {
                Set_InputTimingChangeFlag();
                return TRUE;
            }
        }
        else if (SrcFlags &bInterlaceMode)
        {
            Set_InputTimingChangeFlag();
            return TRUE;
        }
        return FALSE;
    }
#endif
#if 1//2006-07-07 Andy
    Bool mStar_ValidTimingDetect(void)
    {
        BYTE fStatus;
        fStatus = mStar_GetInputStatus();
        if (fStatus &SyncLoss)
        // no sync
        {
            SrcFlags = (SrcFlags &0xF0) | fStatus; // clear others flags, except sync informat (polarity & w/o)
            return TRUE;
        }
        else
        {
            if (SyncPolarity(SrcFlags) != SyncPolarity(fStatus))
            // Sync polarity changed
            {
                SrcFlags = (SrcFlags &0xF0) | fStatus; // clear others flags, except sync informat (polarity & w/o)
                return TRUE;
            }
            else
            {
                WORD tempPeriod;
                SrcFlags = (SrcFlags &0xF0) | fStatus; // clear others flags, except sync informat (polarity & w/o)
                tempPeriod = mStar_ReadWord(HSPRD_H) &0x1FFF;
                //printData("HPeriod1 %d", tempPeriod);
                if (InputTimingStableCounter == 0)
                {
                    SrcHPeriod = tempPeriod;
                }
                if (abs(tempPeriod - SrcHPeriod) > HPeriod_Torlance)
                // HPeriod changed
                {
                    //printData("HPeriod2 %d", tempPeriod);
                    return TRUE;
                }
                tempPeriod = mStar_ReadWord(VTOTAL_H) &0x7FF;
                //printData("  VTotal1 %d", tempPeriod);
                if (InputTimingStableCounter == 0)
                {
                	ForceDelay1ms(100);
                	tempPeriod = mStar_ReadWord(VTOTAL_H) &0x7FF;
                    SrcVTotal = tempPeriod;
                }
                if (abs(tempPeriod - SrcVTotal) > VTotal_Torlance)
                // vtotal changed
                {
                    //printData("VTotal2 %d", tempPeriod);
                    return TRUE;
                }
                if ((SrcInputType == Input_Digital || SrcInputType == Input_Digital2) && !FreeRunModeFlag)
                {
                    tempPeriod = mStar_ReadWord(AOHST_H) &0x7FF;
                    // if (abs(tempPeriod-(UserPrefHStart+0x10))>2)
                    if (abs(tempPeriod - UserPrefHStart) >HVStartTorlance)
                    {
                        return TRUE;
                    }
                    tempPeriod = mStar_ReadWord(AOVST_H) &0x7FF;
                    if (abs(tempPeriod - UserPrefVStart) > 2)
                    {
                        return TRUE;
                    }
                }
            }
        }
        return FALSE;
    }
#endif
#if 0
    Bool mStar_ValidTimingDetect(void)
    {
        BYTE fStatus;
        fStatus = mStar_GetInputStatus();
        if (fStatus &SyncLoss)
        // no sync
        {
            SrcFlags = (SrcFlags &0xF0) | fStatus; // clear others flags, except sync informat (polarity & w/o)
            return TRUE;
        }
        else
        {
            if (SyncPolarity(SrcFlags) != SyncPolarity(fStatus))
            // Sync polarity changed
            {
                SrcFlags = (SrcFlags &0xF0) | fStatus; // clear others flags, except sync informat (polarity & w/o)
                return TRUE;
            }
            else
            {
                WORD tempPeriod;
                SrcFlags = (SrcFlags &0xF0) | fStatus; // clear others flags, except sync informat (polarity & w/o)
                tempPeriod = mStar_ReadWord(HSPRD_H) &0x1FFF;
                //printData("HPeriod %d", tempPeriod);
                if (InputTimingStableCounter == 0)
                    SrcHPeriod = tempPeriod;
                if (abs(tempPeriod - SrcHPeriod) > HPeriod_Torlance)
                // HPeriod changed
                {
                    printData("HPeriod %d", tempPeriod);
                    return TRUE;
                }
                tempPeriod = mStar_ReadWord(VTOTAL_H) &0x7FF;
                //printData("VTotal %d", tempPeriod);
                if (InputTimingStableCounter == 0)
                    SrcVTotal = tempPeriod;
                if (abs(tempPeriod - SrcVTotal) > VTotal_Torlance)
                // vtotal changed
                {
                    printData("VTotal %d", tempPeriod);
                    return TRUE;
                }
                if (InputTimingStableFlag)
                {
                    if (mStar_IsStatusChanged() == TRUE)
                    {
                        printMsg("status Change");
                        return TRUE;
                    }
                }
                //================================================================
                if (SrcInputType == Input_Digital && !FreeRunModeFlag)
                {
                    // compare H-Start and active width
                    tempPeriod = mStar_ReadWord(AOHST_H) &0x7FF;
                    if (abs(tempPeriod - (UserPrefHStart + 0x10)) > 2)
                        return TRUE;
                    tempPeriod = (mStar_ReadWord(AOHEND_H) &0x7FF) - tempPeriod + 1;
                    if (abs(tempPeriod - mStar_ReadWord(SPRHDC_H)) > 2)
                        return TRUE;
                    // compare V-Start and active Height
                    tempPeriod = mStar_ReadWord(AOVST_H) &0x7FF;
                    if (abs(tempPeriod - UserPrefVStart) > 2)
                        return TRUE;
                    tempPeriod = (mStar_ReadWord(AOVEND_H) &0x7FF) - tempPeriod + 1; // compare active-width
                    if (abs(tempPeriod - mStar_ReadWord(SPRVDC_H)) > 2)
                        return TRUE;
                }
            }
        }
        return FALSE;
    }
#endif
#if 1  //2006-08-17 Andy
    Bool mStar_FindMode(void)
    {
        WORD hFreq, vFreq;
        BYTE htol, vtol;
        #define fStatus  hFreq
        fStatus = mStar_GetInputStatus();
        if (SyncPolarity(SrcFlags) != SyncPolarity(fStatus))
        // Sync polarity changed
        {
            // 2006/10/24 7:4PM by Emily         printData("false 1", 0);
            return FALSE;
        }
        SrcFlags &= 0x0F;
        #undef fStaus
        hFreq = mStar_ReadWord(HSPRD_H) &0x1FFF;
        if (abs(hFreq - SrcHPeriod) > HPeriod_Torlance)
        // HPeriod changed
        {
            // 2006/10/24 7:4PM by Emily        printData("false 2", 0);
            return FALSE;
        }
        vFreq = mStar_ReadWord(VTOTAL_H) &0x7FF;
        if (abs(vFreq - SrcVTotal) > VTotal_Torlance)
        // vtotal changed
        {
            return FALSE;
        }
        /////////////////////////////////////////////////////////
        SrcHPeriod = hFreq;
        SrcVTotal = vFreq;
        hFreq = HFreq(SrcHPeriod);
        vFreq = VFreq(hFreq, SrcVTotal);
        //======== for interlace mode
        #if 1
        #if ENABLE_HDCP
            if (mStar_ReadByte(STATUS2) &INTM_B)
        #else
                if (SrcInputType != Input_Digital && mStar_ReadByte(STATUS2) &INTM_B)
        #endif
        {
            SrcFlags |= bInterlaceMode;
            vFreq *= 2;
        }
       #endif

	#if findmodebyVwidth
	SrcVsyncwidth=GetVsyncWidth();
	#endif

	#if DEBUG_PRINTDATA
     	printData("HFreq= %d", hFreq );
     	printData("VFreq= %d", vFreq );
     	printData("SrcVTotal= %d", SrcVTotal );
     	printData("SrcVsyncwidth= %d", SrcVsyncwidth );
	#endif

        //====================================
        // check if input timing is out of range
        #if 1
        if (hFreq > MaxInputHFreq || hFreq < MinInputHFreq || vFreq > MaxInputVFreq || vFreq < MinInputVFreq)
        {
            SrcFlags |= bUnsupportMode;
            //printMsg("input timing is out of range");
            return TRUE;
        }
        #endif
        htol = HFreq_Torlance; //0527 add for getting more accurate mode
        vtol = VFreq_Torlance;
        // search input mode index
        {
            BYTE modeIndex = 0;
            InputModeType *modePtr = StandardMode;
            Bool found = FALSE;
            while (modePtr->HFreq)
            {
                if (SrcVTotal > StandardModeResolution[modePtr->ResIndex].DispHeight)
                {
               if (abs(hFreq-modePtr->HFreq)<htol && abs(vFreq-modePtr->VFreq)< vtol &&
                 GetSyncPolarity(SrcFlags)&modePtr->Flags &&
                 abs(SrcVTotal-(modePtr->VTotal))<=3&& // 2006/8/24 4:36PM by Emily
                    (SrcFlags &bInterlaceMode) == (modePtr->Flags &bInterlaceMode))
                    {
                        found = TRUE;
                        SrcModeIndex = modeIndex;
                        //   printData("find mode Polarity=%d",SrcFlags&0xF0);
                        //    printData("mode table Polarity=%d",modePtr->Flags);
                        if (modePtr->ResIndex == Res_1152x864)
                        // for 1152x864   vs 1152x870=>  60hz & 75hz
                        {
                            if (abs(SrcVTotal - (modePtr->VTotal)) > 3)
                            {
                                modePtr++;
                                modeIndex++;
                                continue;
                            }
                        }
                        #if DEBUG_ENABLE  && DEBUG_DETECT
                            printData("STDMode:%d", SrcModeIndex);
                        #endif
                        break;
                    }
                }
                modePtr++;
                modeIndex++;
                #if 1  //0419
                    //if (modeIndex == 34)
                    if(modePtr->ResIndex==Res_832x624&&abs(modePtr->VFreq-750)<10)
                    {
                        //34. 832x624 75Hz (MAC)
                        htol = HFreq_Torlance - 1;
                        vtol = VFreq_Torlance - 1;
                    }
                    //else if (modeIndex==49||modeIndex==50)  //2006-07-04 Andy
                    //else if (modeIndex == 43 || modeIndex == 50)
                    // 2006/12/8 6:41PM by KK             else if ( modeIndex == 43 || modeIndex == 49 || modeIndex == 50 )  //2006-07-04 Andy
                    else if ((modePtr->ResIndex==Res_1152x864&&abs(modePtr->VFreq-750)<10)||(modePtr->ResIndex==Res_1280x1024&&abs(modePtr->VFreq-750)<10))
                    {
                        //43. 1152x864 75Hz (VESA),  49. 1280x1024 60Hz (VESA), 50. 1280x1024 75Hz (VESA)
                        htol = HFreq_Torlance - 5;
                        vtol = VFreq_Torlance - 5;
                    }
                    else
                    {
                        htol = HFreq_Torlance;
                        vtol = VFreq_Torlance;
                    }
                #endif
            }
      if (!found)
      {

	modeIndex=0;
        modePtr=StandardMode;
        while (modePtr->HFreq)
       { if (SrcVTotal>StandardModeResolution[modePtr->ResIndex].DispHeight)
          { if (abs(hFreq-modePtr->HFreq)<(HFreq_Torlance+15) && abs(vFreq-modePtr->VFreq)<(VFreq_Torlance+15) &&
                 GetSyncPolarity(SrcFlags)&modePtr->Flags &&
                 abs(SrcVTotal-(modePtr->VTotal))<=30&&
                 (SrcFlags&bInterlaceMode)==(modePtr->Flags&bInterlaceMode))
              { found=TRUE;
                 SrcFlags|=bUserMode;
                SrcModeIndex=modeIndex;
#if DEBUG_ENABLE  && DEBUG_DETECT
        printData( "UserMode:%d", SrcModeIndex );
#endif
                break;
              }
           }
         modePtr++;
         modeIndex++;
                }
            }
            if (!found)
            // out of standard input range
            {
                #define delta   hFreq
                #define minDelta    vFreq
                modeIndex = 0;
                modePtr = StandardMode;
                minDelta = VTotal_Delta;
                while (modePtr->HFreq)
                {
                    if (SrcVTotal > StandardModeResolution[modePtr->ResIndex].DispHeight)
                    {
                        delta = abs(SrcVTotal - (modePtr->VTotal));
                        if (delta < VTotal_Delta && (SrcFlags &bInterlaceMode) == (modePtr->Flags &bInterlaceMode))
                        {
                            if (delta < minDelta)
                            {
                                minDelta = delta;
                                SrcModeIndex = modeIndex;
#if DEBUG_ENABLE  && DEBUG_DETECT
        printData( "UserMode1:%d", SrcModeIndex );
#endif
                            }
                            SrcFlags |= bUserMode;
                            found = TRUE;
                            #if 0   //2006-07-04 Andy
                                if (SrcInputType == Input_Analog1)
                                {
                                    if ((modeIndex == 20) && (abs(SrcVTotal - modePtr->VTotal) > 15))
                                    {
                                        SrcFlags |= bUnsupportMode;
                                        return TRUE;
                                    }
                                } //wmz 20051117
                            #endif
                        }
                    }
                    modePtr++;
                    modeIndex++;
                }
                #undef delta
                #undef minDelta
            } // out of standard input range


#if DEBUG_ENABLE 
        printData("SrcModeIndex    1111  ===%d", SrcModeIndex);
#endif


	if ((SrcModeIndex==PRESET_1360x768_60Hz)||(SrcModeIndex==PRESET_1280x768_60Hz_N)||(SrcModeIndex==PRESET_1366x768_60Hz_1))
	{
		if (abs(SrcVTotal-795)<=2)
			{
			if(SrcVsyncwidth<=5)
				SrcModeIndex=PRESET_1366x768_60Hz_1;
			else
				SrcModeIndex=PRESET_1360x768_60Hz;
			}
		else
			SrcModeIndex=PRESET_1280x768_60Hz_R;
	}

      if(SrcModeIndex==PRESET_1366x768_60Hz_N||SrcModeIndex==PRESET_1280x768_60Hz_R)
      	{
	      	if(SrcVTotal>=798)
			{
			if(SrcVsyncwidth>=7)
				SrcModeIndex=PRESET_1280x768_60Hz_N;
			else
		  		SrcModeIndex=PRESET_1366x768_60Hz_N;
	      		}
		else
		  SrcModeIndex =PRESET_1280x768_60Hz_R;
      	}
	else if (SrcModeIndex==PRESET_1600x900_60Hz||SrcModeIndex==PRESET_1440x900_60Hz_R)
	{
		if(SrcVsyncwidth<=5)
			SrcModeIndex=PRESET_1600x900_60Hz;
		else
			SrcModeIndex=PRESET_1440x900_60Hz_R;
	}

	if (SrcModeIndex==PRESET_1600x900_60Hz_R||SrcModeIndex==PRESET_1280x960_60Hz_VESA)
		{
		 BYTE i;
		 WORD wVS,wVE;
		 WORD SHS,SHE;
          	 for(i=0;i<10;i++)
			{
			Delay1ms(100);
			wVS = mStar_ReadWord(AOVST_H);
			wVE = mStar_ReadWord(AOVEND_H);
					
			//Delay1ms(100);
			SHS = mStar_ReadWord(AOHST_H);
			SHE = mStar_ReadWord(AOHEND_H);

			#if 0//MESSAGE_PrintData_Open
			printData("11111wVS=%d", wVS);
			printData("11111wVE=%d", wVE);
			#endif
			if((wVS > wVE)||(SHS > SHE))
			continue;
			Delay1ms(50);
			if(abs(wVS - mStar_ReadWord(AOVST_H))>2 || abs(wVE - mStar_ReadWord(AOVEND_H))>2)
			continue;
			if(abs(SHS - mStar_ReadWord(AOHST_H))>2 || abs(SHE - mStar_ReadWord(AOHEND_H))>2)
			continue;
			
			if(((wVE - wVS) > 920)&&((SHE - SHS) < 1366))
			{
			SrcModeIndex = PRESET_1280x960_60Hz_VESA;
			break;
			}
			//#else
			if(((wVE - wVS) > 890)&&((wVE - wVS) < 910))
			{
			SrcModeIndex = PRESET_1600x900_60Hz_R;
			break;
			}
			//****************************************//20151221 lixw
			else if((wVS > 102)&&(wVE  < 120))
			{
			SrcModeIndex = PRESET_1600x900_60Hz_R;
			break;
			}	
			//****************************************//20151221 lixw
			else
			{
			//SrcModeIndex = PRESET_1280x960_60Hz_VESA;
			break;
			}
			
			}

		}
	
	if(SrcModeIndex==PRESET_1024x768_60Hz_VESA)
	{
		if(SrcVTotal<806)
			
			SrcModeIndex = PRESET_1366x768_60Hz_1;
		
	}
#if ( PanelWidth==1600&&PanelHeight==900)
if ((StandardModeGroup==Res_1680x1050)||(StandardModeGroup==Res_1920x1080)||(StandardModeGroup==Res_1920x1200))
	 SrcFlags |= bUnsupportMode;
#endif
			
            if (!found)
            {
                SrcFlags |= bUnsupportMode;
#if DEBUG_ENABLE 
        printMsg("not found");
#endif
            }

#if DEBUG_ENABLE 
        printData("SrcModeIndex      ===%d", SrcModeIndex);
#endif
	#if PanelType==PanelCLAA156WB11A||PanelType==PanelCMON156B6L0B||PanelType==PanelBOEHT156WXB || (PanelType == PanelLTN156AT24_P) //|| (PanelType == PanelB156XTN02)
	  if(modePtr->ResIndex==Res_720x400&&abs(modePtr->VFreq-700)<10)// 2011.3.17   13:55:34
	  	{
	  	}
	  else if ((hFreq > MaxInputHFreq || hFreq < MinInputHFreq || vFreq >613 || vFreq < 587))
        {
            SrcFlags |= bUnsupportMode;
	#if DEBUG_ENABLE 
            printMsg("input timing is out of range");
       #endif
        }		
	 #endif
        } // search mode index
        return TRUE;
    }
#endif
BYTE mStar_GetInputStatus(void)
{
    BYTE fStatus = 0;
    WORD inputValue;
    BYTE status;
    inputValue = mStar_ReadWord(HSPRD_H) &0x1FFF;
    if (inputValue == 0x1FFF || inputValue < 10)
    {
        fStatus |= bHSyncLoss;
    }
    inputValue = mStar_ReadWord(VTOTAL_H) &0x7FF;
    if (inputValue == 0x7FF || inputValue < 200)
    {
        fStatus |= bVSyncLoss;
    }
    
    status = mStar_ReadByte(STATUS2);
    
    // 090430 addition for checking sync active bit
    if(SrcInputType==Input_Analog1 || !(mStar_ReadByte(0x04)&BIT6)&&SrcInputType>=Input_Digital)
    {
        if((status&(BIT6|BIT7)) != (BIT6|BIT7))
            fStatus |= SyncLoss;
    }
    
    fStatus |= (status &0x03); // Get input timing polarity
    if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2 )
    // input timing is valid while current state is no sync
    {
        if (mStar_ReadByte(INTCTRL) &0x40 /*|| (mStar_ReadByte(STATUS2) &0xC0) != 0xC0*/)
        {
            fStatus |= SyncLoss;
        }
    }
    else if (SyncLossState() && !(fStatus &SyncLoss))
    // input timing is valid while current state is no sync
    {
        #if 1  //2006-08-16 Andy
            if (SrcInputType < Input_Digital && (status & INTM_B))
            {
                mStar_WriteByte(SWRST, GPR_B);
                Delay1ms(1);
                mStar_WriteByte(SWRST, 0);
            }
        #endif
        Delay1ms(20);
        if (InputTimingChangeFlag)
        {
            return fStatus;
        }
        status = status &mStar_ReadByte(STATUS2);
        if ((status &3) != (fStatus &3))
        // polarity is stable
        {
            fStatus |= SyncLoss;
        }
        else if (status &0x30)
        // SOG or CSync input
        {
            Delay1ms(20);
            if (InputTimingChangeFlag)
            {
                return fStatus;
            }
            status = mStar_ReadByte(STATUS2);
            // Check if SOG/CSYNC is valid
            if (abs(mStar_ReadWord(VTOTAL_H) - inputValue) > 2)
            {
                fStatus |= SyncLoss;
            }
            /********** wmz 20051117
            else if (SrcInputType==Input_YPbPr)
            { if ((status&0x18)!=0x18)
            fStatus|=SyncLoss;
            }
             ***************/
        #if ChipID>=ChipAD
            #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
            else if (status &SOGD_B && (mStar_ReadWord(DVICKTH)&0x0FFF) > (mStar_ReadWord(HSPRD_H) / 8))  // Scar 20090618
            #else
            else if (status &SOGD_B && mStar_ReadByte(SOGHSPW) > mStar_ReadWord(HSPRD_H) / 6)
            #endif
            {
                fStatus |= SyncLoss;
            }
            #else // befor ChipAD
                else if ((status &0x50) == 0x50 || (status &0xA0) == 0xA0)
                {
                    fStatus |= SyncLoss;
                }
            #endif
            else if ((status &0x18) == 0x18)
            {
                fStatus |= SyncLoss;
            } // Check if SOG/CSYNC is valid
            
        }
    }
    return fStatus;
}

#if findmodebyVwidth
#include "mwe_msregtv.h"
BYTE GetVsyncWidth(void)
{
	BYTE VsyncWidth=0;
	BYTE ScalerBank=0;
	
	ScalerBank=mStar_ReadByte(REGBK);
	mStar_WriteByte(REGBK, REG_BANK_SCALER);
	mStar_WriteByte(0xFA, mStar_ReadByte(0xFA)|BIT0);
	Delay1ms(25);
	VsyncWidth=mStar_ReadByte(BK0_E2);
	mStar_WriteByte(0xFA, mStar_ReadByte(0xFA)&(~BIT0));
	mStar_WriteByte(REGBK, ScalerBank);
	
	return VsyncWidth;
}
#endif


#if (MainBoardType==MainBoard_2701||MainBoardType==MainBoard_2698_2_X_X_2||\
MainBoardType==MainBoard_2698_2_X_X_8||MainBoardType==MainBoard_2883_B_X_X_1||\
MainBoardType==MainBoard_2883_B_X_X_2||MainBoardType==MainBoard_2670_C||\
MainBoardType==MainBoard_2670_1||MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611||\
MainBoardType==MainBoard_2883_1B||MainBoardType==MainBoard_715G3329_A2||\
MainBoardType==MainBoard_715G3598_M0A||MainBoardType==MainBoard_715G3329_1_2||\
MainBoardType==MainBoard_715G3329_58N||MainBoardType==MainBoard_715G3329_18N)
Bool IsCableNotConnected(void)
{
    #if 0 //temp Dual
    return (hwDSUBCable_Pin);
    #else
    BYTE retry_Key, temp, temp1;
    retry_Key = 3;
    while (retry_Key)
    {
        temp = KEYPAD_ADC4 &0x3F;
        Delay1ms(2);
        temp1 = KEYPAD_ADC4 &0x3F;
        if (abs(temp - temp1) < 5)
            break;
        retry_Key--;
        Delay1ms(2);
    }
    #if DEBUG_ENABLE  && DEBUG_DETECT
    // 2007/9/13  by KK                 printData("temp=%d", temp);
    #endif
    if (temp > 60)
        return TRUE;
        //TRUE;
    else
        return FALSE;
    //TRUE;
    #endif
}
#elif(MainBoardType==MainBoard_715G2575)
    Bool IsCableNotConnected(void)
    {
        BYTE retry_Key, temp, temp1;
        retry_Key = 3;
        while (retry_Key)
        {
            temp = KEYPAD_ADC2 &0x3F;
            Delay1ms(2);
            temp1 = KEYPAD_ADC2 &0x3F;
            if (abs(temp - temp1) < 5)
                break;
            retry_Key--;
        }
        #if DEBUG_ENABLE  && DEBUG_DETECT
// 2007/9/13  by KK             printData("temp=%d", temp);
        #endif
        Delay1ms(3);
        if (temp > 60)
            return TRUE;
        else
            return FALSE;
    }
#elif MainBoardType==MainBoard_715G2808 || MainBoardType==MainBoard_715G2988
Bool IsCableNotConnected( void )
{
    BYTE temp;

    temp=KEYPAD_ADC1&0x3F;
    if (temp<0x30)
        return	FALSE;
    else
        return	TRUE;

}
#elif MainBoardType==MainBoard_715G3603_M0A||MainBoardType==MainBoard_715G3663_M0A
Bool IsCableNotConnected( void )
{
            BYTE retry_Key, temp, temp1;
            retry_Key = 3;
            while (retry_Key)
            {
                temp = KEYPAD_ADC4 &0x3F;
                Delay1ms(2);
                temp1 = KEYPAD_ADC4 &0x3F;
                if (abs(temp - temp1) < 5)
                    break;
                retry_Key--;
                Delay1ms(2);
            }

            if (temp > 60 && hwDSUBCable_Pin)
                return TRUE;
            else
                return FALSE;
    }
#else
    Bool IsCableNotConnected(void)
    {
        #if DEBUG_ENABLE  && DEBUG_DETECT
            printData("hwDSUBCable_Pin %d", hwDSUBCable_Pin);
       //     printData("hwDVICable_Pin %d", hwDVICable_Pin);
        #endif
        #if Dual
        //20050919
        // return  (hwDSUBCable_Pin && hwDVICable_Pin);
        // return   (hwDVICable_Pin);
        {
            return (hwDSUBCable_Pin && hwDVICable_Pin);//&& hwDVICable_Pin);
        }
        #else
        {
            return (hwDSUBCable_Pin);
        }
	 #endif
    }
#endif
