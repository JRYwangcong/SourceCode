#include <math.h>
#include "types.h"
#include "ms_reg.h"
#include "ms_rwreg.h"
#include "debug.h"
#include "msosd.h"
#include "Board.h"
#include "global.h"
#include "menudef.h" 
#include "msdlc.h"
#include "msace.h"
#if AudioFunc
#include "misc.h"
#endif
#if ENABLE_HDMI
#include "msHDMI.h"
#include "msHDMIACE.h"
#endif
#if MainBoardType==MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
extern void SetScalerGPIO(BYTE pinX, bit HiLow);
#endif
#if ScanKey
#ifdef EnableLightBar
#include "scankey.h"
#endif
#endif
#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
    #include <MsACE.h>   // Provides: msAdjustPCRGB()
    BYTE _gRedContrast   = 0;
    BYTE _gGreenContrast = 0;
    BYTE _gBlueContrast  = 0;
#endif
//===================================================
// Local function declaration
#if UsesRGB
    void mStar_AdjustRedColor(BYTE rColor, BYTE gColor, BYTE bColor);
    void mStar_AdjustGreenColor(BYTE rColor, BYTE gColor, BYTE bColor);
    void mStar_AdjustBlueColor(BYTE rColor, BYTE gColor, BYTE bColor);
#else 
    void mStar_AdjustRedColor(BYTE color, BYTE contrast);
    void mStar_AdjustGreenColor(BYTE color, BYTE contrast);
    void mStar_AdjustBlueColor(BYTE color, BYTE contrast);
#endif 
void mStar_AdjustBlackLevel(BYTE BlackLevel);
#if 0
extern void NVRam_WriteByte(WORD addr, BYTE value);
    Bool EraseAllEEPROMCell(void)
    {
        WORD  i;
        #ifdef EEPRom_24C04
        for(i=0;i<0x1ff;i++)
        #else
        for(i=0;i<0x7ff;i++)
        #endif
            NVRam_WriteByte(i,0xFF);
        hw_ClrGreenLed();
        hw_SetAmberLed();
        return TRUE;
    }
#endif
#define MinBlackLevelValue	78
#define MaxBlackLevelValue	178
void mStar_AdjustUserPrefBlacklevel(BYTE Redblacklevel, BYTE Greenblacklevel, BYTE Blueblacklevel)
{
    Redblacklevel = ((WORD)Redblacklevel *(MaxBlackLevelValue - MinBlackLevelValue)) / 100+MinBlackLevelValue;
    Greenblacklevel = ((WORD)Greenblacklevel *(MaxBlackLevelValue - MinBlackLevelValue)) / 100+MinBlackLevelValue;
    Blueblacklevel = ((WORD)Blueblacklevel *(MaxBlackLevelValue - MinBlackLevelValue)) / 100+MinBlackLevelValue;

#if(PanelType == PanelM185WHB_N10)
    mStar_WriteByte(BCR, 0x80);//0x80);
    mStar_WriteByte(BCG, 0x80);//0x80);
    mStar_WriteByte(BCB, 0x80);//0x80);
#elif(PanelType == PanelLM185TT3A)
#if LM185TT3A_240MA
    mStar_WriteByte(BCR, 0x95);//0x80);
    mStar_WriteByte(BCG, 0x95);//0x80);
    mStar_WriteByte(BCB, 0x95);//0x80);
#else
    mStar_WriteByte(BCR, 0x82);//0x80);
    mStar_WriteByte(BCG, 0x82);//0x80);
    mStar_WriteByte(BCB, 0x82);//0x80);
#endif

#else
    mStar_WriteByte(BCR, 0x7C);//0x80);
    mStar_WriteByte(BCG, 0x7C);//0x80);
    mStar_WriteByte(BCB, 0x7C);//0x80);
#endif
	/*
    mStar_WriteByte(BCR, Redblacklevel);
    mStar_WriteByte(BCG, Greenblacklevel);
    mStar_WriteByte(BCB, Blueblacklevel);
	//*/
}

//****************************************************
#define MaxBacklightNowValue	225//230//231//226  //216 //236//238
#define MinBacklightNowValue	  20//17//29  //39  //19//17
#if MainBoardType == MainBoard_4432
#define AbsMostDarkest1  ((SrcInputType == Input_Analog1)?(19):(17))  // 16  //20081201
#define AbsNearDarkest1  ((SrcInputType == Input_Analog1)?(24):(20))  // 18
#define AbsMostDarkest2  ((SrcInputType == Input_Analog1)?(19):(17))  // 17
#define AbsNearDarkest2  ((SrcInputType == Input_Analog1)?(25):(22))  // 20
#else
#define AbsMostDarkest1  ((SrcInputType == Input_Analog1)?(16):(16))  // 16  //20081201
#define AbsNearDarkest1  ((SrcInputType == Input_Analog1)?(21):(17))  // 18
#define AbsMostDarkest2  ((SrcInputType == Input_Analog1)?(16):(16))  // 17
#define AbsNearDarkest2  ((SrcInputType == Input_Analog1)?(23):(19))  // 20
#endif
//****************************************************
void mStar_AdjustDcrBrightness(void)
{
    static BYTE xdata brightness;
    static BYTE xdata DcrCutoffFlag = 0;    //DCR new rule 081128    

    msWriteByte(REGBK, REG_BANK_SCALER);
    if(BacklightNow < MinBacklightNowValue)
          BacklightNow = MinBacklightNowValue;
	    if(BacklightNow > MaxBacklightNowValue)
          BacklightNow = MaxBacklightNowValue;
#if EnablePanelServiceMenu
	//if(!DcrCutoffFlag)
		//brightness = (((DWORD)(BacklightNow - MinBacklightNowValue)*(PanelMaxDuty - RealMinDcrBrightnessValue)) / (MaxBacklightNowValue - MinBacklightNowValue)) + RealMinDcrBrightnessValue;
	if (BacklightNow >= MaxBacklightNowValue)
	{
		brightness = PanelMaxDuty;
		DcrCutoffFlag = 0;    //20081201
	} 
	else if (BacklightNow <= MinBacklightNowValue)
	{
		if(SetDarkestFlag)
		{
			SetDarkestFlag = 0;
			PreBrightData = 0; // to make sure if bright data exist, it will update brightness.
			//brightness = PanelMinDuty;  // set logical absolute dark value, // RealMinBrightnessValue;
			if(DcrCutoffFlag == 0)
			{
				if((DarkData <= AbsMostDarkest1) && (BrightData <= AbsNearDarkest1))        // >>>  logical absolute dark condition  <<< 20081128
				// set logical absolute dark value, // RealMinBrightnessValue;
				{
					//brightness = RealMinDcrBrightnessValue;  // set logical absolute dark value, // RealMinBrightnessValue;
					DcrCutoffFlag = 1;
				}
  				brightness = RealMinDcrBrightnessValue;
			}
			else
			{
				if((DarkData <= AbsMostDarkest2) && (BrightData <= AbsNearDarkest2))        // set logical absolute dark value,
				{
					if(brightness > PanelMinDuty)
						brightness--;  // set logical absolute dark value, // RealMinBrightnessValue;
					if((mStar_ReadOSDByte(IOSDC1)&MWIN_B)&&(brightness<RealMinBrightnessValue))//Hony 20090617 for high DCR,  when monitor shows OSD menu,the DCR min brightness==RealMinBrightnessValue 
						brightness=RealMinBrightnessValue;
				}
				else
				{
					DcrCutoffFlag = 0;
  					brightness = RealMinDcrBrightnessValue;
				}  
			}				
		}
		else // 081116
		{
			DcrCutoffFlag = 0;
			brightness = RealMinDcrBrightnessValue;
		}
	}
	else
	{
		DcrCutoffFlag = 0;
		brightness = (((DWORD)(BacklightNow - MinBacklightNowValue)*(PanelMaxDuty - RealMinDcrBrightnessValue)) / (MaxBacklightNowValue - MinBacklightNowValue)) + RealMinDcrBrightnessValue;
	}  

#else

#if DEBUG_ENABLE
//printData("brightness            ==%x", brightness);
//printData("DarkData            ==%x", DarkData);
printData("BacklightNow            ==%x", BacklightNow);
#endif
	//if(!DcrCutoffFlag)
		//brightness = (((DWORD)(BacklightNow - MinBacklightNowValue)*(RealMaxBrightnessValue - RealMinDcrBrightnessValue)) / (MaxBacklightNowValue - MinBacklightNowValue)) + RealMinDcrBrightnessValue;
	if (BacklightNow >= MaxBacklightNowValue)
	{
		brightness = RealMaxBrightnessValue;
		DcrCutoffFlag = 0;    //20081201
	} 
	else if (BacklightNow <= MinBacklightNowValue)
	{
		if(SetDarkestFlag)
		{
			SetDarkestFlag = 0;
			PreBrightData = 0; // to make sure if bright data exist, it will update brightness.
			//brightness = RealAbsoluteMinBrightnessValue;  // set logical absolute dark value, // RealMinBrightnessValue;
			if(DcrCutoffFlag == 0)
			{
				if((DarkData <= AbsMostDarkest1) && (BrightData <= AbsNearDarkest1))        // >>>  logical absolute dark condition  <<< 20081128
				// set logical absolute dark value, // RealMinBrightnessValue;
				{
					//brightness = RealMinDcrBrightnessValue;  // set logical absolute dark value, // RealMinBrightnessValue;
					DcrCutoffFlag = 1;
				}
				brightness = RealMinDcrBrightnessValue;  // set logical absolute dark value, // RealMinBrightnessValue;
			}
			else
			{
				if((DarkData <= AbsMostDarkest2) && (BrightData <= AbsNearDarkest2))        // set logical absolute dark value,
				{
					if(brightness > RealAbsoluteMinBrightnessValue)
						brightness=brightness-1;//-2;//10;  // set logical absolute dark value, // RealMinBrightnessValue;
					if((mStar_ReadOSDByte(IOSDC1)&MWIN_B)&&(brightness<RealMinBrightnessValue))//Hony 20090617 for high DCR,  when monitor shows OSD menu,the DCR min brightness==RealMinBrightnessValue 
						brightness=RealMinBrightnessValue;
				}
				else
				{
					DcrCutoffFlag = 0;
  					brightness = RealMinDcrBrightnessValue;
				}  
			}
		}    // if(SetDarkestFlag)
		else // 081116
		{
			DcrCutoffFlag = 0;
			brightness = RealMinDcrBrightnessValue;
		}
	}
	else
	{
		DcrCutoffFlag = 0;
		brightness = (((DWORD)(BacklightNow - MinBacklightNowValue)*(RealMaxBrightnessValue - RealMinDcrBrightnessValue)) / (MaxBacklightNowValue - MinBacklightNowValue)) + RealMinDcrBrightnessValue;
	}  
#endif	
#if DEBUG_ENABLE
printData("brightness            ==%x", brightness);
#endif
   
    #if InverseBrightness
	#if BrightnessPWM == PWM6//xiandi 20140613
	mStar_WriteByte(REGBK, REGBANKADC);
	#endif
	mStar_WriteByte(BrightnessPWM, 0xFF-brightness);
	#if BrightnessPWM == PWM6//xiandi 20140613
	mStar_WriteByte(REGBK, REGBANKSCALER);
	#endif
    #else 
		#if ModelName == AOC_e2237Fw  //091111:for LED Panel High DCR  by lizzie
		      if(brightness == RealAbsoluteMinBrightnessValue)
			{
				if((mStar_ReadByte(0x63)&BIT2)&&(MenuPageIndex == RootMenu))//091111: PWM1=>GPIO
				{
					mStar_WriteByte(0x63, (mStar_ReadByte(0x63)) &(~BIT2));
					P2_5 = 0;
				}
			}
			else
			{
				if(!(mStar_ReadByte(0x63)&BIT2)) //091111: GPIO=>PWM1
					mStar_WriteByte(0x63, (mStar_ReadByte(0x63))|BIT2);
				mStar_WriteByte(BrightnessPWM, brightness);
			}
		#else
	#if BrightnessPWM == PWM6//xiandi 20140613
	mStar_WriteByte(REGBK, REGBANKADC);
	#endif
        mStar_WriteByte(BrightnessPWM, brightness);
	#if BrightnessPWM == PWM6//xiandi 20140613
	mStar_WriteByte(REGBK, REGBANKSCALER);
	#endif		
		#endif
    #endif 
	
	#if DCRClearBacklight
			#if EnablePanelServiceMenu
				if(brightness == PanelMinDuty)
			#else
				if(brightness <= RealAbsoluteMinBrightnessValue)
			#endif
				{
					if(BackLightActiveFlag)
						hw_ClrBlacklit();
				}
				else
				{
					if(!BackLightActiveFlag)
						hw_SetBlacklit();
				}	
	#endif
}


#define	MIN_BLIGHT_DISP_VALUE	0
#define	MAX_BLIGHT_DISP_VALUE	100
#define	DEF_BLIGHT_DISP_VALUE	(MIN_BLIGHT_DISP_VALUE + ((MAX_BLIGHT_DISP_VALUE-MIN_BLIGHT_DISP_VALUE)/2))


#define	MIN_BRT_DISP_VALUE	0
#define	MAX_BRT_DISP_VALUE	100
#define	DEF_BRT_DISP_VALUE	90//(MIN_BRT_DISP_VALUE + ((MAX_BRT_DISP_VALUE-MIN_BRT_DISP_VALUE)/2))

#if PanelType == PanelLGLP156WHB_DP
#define	MIN_BLIGHT_VALUE		0x50//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
#define	MAX_BLIGHT_VALUE		0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0xE0//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#else
#if Panel_N156BGE
#if Panel_N156BGE_NEW
	#if PanelType == PanelM185WHB_N10
#define	MAX_BLIGHT_VALUE		0xFF//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0xE2//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x20//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
	#elif (PanelType == PanelLM185TT3A)
#if LM185TT3A_240MA	
#define	MAX_BLIGHT_VALUE		0xFF//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0xE2//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x03//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
#else
#define	MAX_BLIGHT_VALUE		0xFF//0xFF//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0x96//0x96//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x03//0x03//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
#endif

//400限240
/*
#define	MAX_BLIGHT_VALUE		0x48//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0x40//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x03//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
*/
	#else
#define	MAX_BLIGHT_VALUE		0xFF//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0xD2//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x20//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
	#endif
#else
#define	MAX_BLIGHT_VALUE		0xDE//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0xB8//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x20//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
#endif
#elif Panel_B156xtno7 
#define	MAX_BLIGHT_VALUE		0xFF//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0xD5//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x20//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
#elif Panel_NT156WHM_N12 
#define	MAX_BLIGHT_VALUE		0xFF//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0xD5//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x20//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
#else
#define	MAX_BLIGHT_VALUE		0xFF//0xE7//0xFF//0xDE//0xCB//0xff    //  (MIN_BLIGHT_VALUE:0X00,MAX_BLIGHT_VALUE:0x9a for 19W lcd的灰界水波纹干扰澹//?/ 澹?
#define	DEF_BLIGHT_VALUE		0xD5//0xFF//0xDB//0xCB//0xB8//(MIN_BLIGHT_VALUE + ((MAX_BLIGHT_VALUE-MIN_BLIGHT_VALUE)/2))   //0x4d//// 0x40
#define	MIN_BLIGHT_VALUE		0x20//0xA0//0x95//0x00//0x20//0x33//0x2E // 0x20 // 0x10 // 0x80 // For 22" is 0x78;
#endif
#endif

void mStar_AdjustBrightness(BYTE brightness)
{
//if (brightness==0)
//EraseAllEEPROMCell();

#if 0//EnablePanelServiceMenu
    if (brightness>90)
	    brightness = (((DWORD)brightness *(PanelMaxDuty- PanelDefDuty)) / 10) + (DWORD)10*PanelDefDuty-(DWORD)9*PanelMaxDuty;
    else
    {
           if(TestMinDutyFlag)
               brightness = (((DWORD)brightness *(PanelDefDuty- PanelMinDuty)) / 90) + PanelMinDuty;
           else
               brightness = (((DWORD)brightness *(PanelDefDuty- RealMinBrightnessValue)) / 90) + RealMinBrightnessValue;
    }
#else
if(brightness>=MAX_BLIGHT_DISP_VALUE)
     brightness=MAX_BLIGHT_DISP_VALUE;
else if(brightness<=MIN_BLIGHT_DISP_VALUE)
     brightness=MIN_BLIGHT_DISP_VALUE;

if(brightness>DEF_BRT_DISP_VALUE)
{
	brightness=((WORD)(brightness-DEF_BRT_DISP_VALUE)*(MAX_BLIGHT_VALUE-DEF_BLIGHT_VALUE))/(MAX_BRT_DISP_VALUE-DEF_BRT_DISP_VALUE)+DEF_BLIGHT_VALUE;
	//brightness=((WORD)(brightness-DEF_BRT_DISP_VALUE)*(MAX_BRT_DISP_VALUE-DEF_BRT_DISP_VALUE))/(MAX_BRT_DISP_VALUE-DEF_BRT_DISP_VALUE)+DEF_BLIGHT_VALUE;
}
else
{
	brightness=((WORD)brightness*(DEF_BLIGHT_VALUE-MIN_BLIGHT_VALUE))/(DEF_BRT_DISP_VALUE-MIN_BRT_DISP_VALUE)+MIN_BLIGHT_VALUE;
}

#endif
    BacklightNow=BacklightPrev = brightness;

	#if BrightnessPWM == PWM6
	mStar_WriteByte(REGBK, REGBANKADC);
	#endif
    #if InverseBrightness
	mStar_WriteByte(BrightnessPWM, 0xFF-brightness);
    #else 
        mStar_WriteByte(BrightnessPWM, brightness);
    #endif 
	#if BrightnessPWM == PWM6
	mStar_WriteByte(REGBK, REGBANKSCALER);
	#endif
}

#if Enable_Lightsensor
// 090413
void mStar_AdjustLightSensorBrightness(void)
{
    if(abs( SetBriValue - TempBrightness ) > LIGHT_SENSOR_Threshold)
    {
        if( SetBriValue < TempBrightness )
            TempBrightness --;
        else
            TempBrightness ++;

#if 0//DEBUG_ENABLE //&& DEBUG_LightSensor
    printData("   UserPrefBrightness-1:%d", TempBrightness);
#endif
        
    }
    else if(SetBriValue != TempBrightness && 
               (SetBriValue == MinBrightnessValue || SetBriValue == MaxBrightnessValue))
    {
        // 091002
        //TempBrightness = SetBriValue;
        if( SetBriValue < TempBrightness )
            TempBrightness --;
        else
            TempBrightness ++;
        
#if 0//DEBUG_ENABLE //&& DEBUG_LightSensor
    printData("   UserPrefBrightness-2:%d", TempBrightness);
#endif
    }
    else // threshold
    {
#if 0//DEBUG_ENABLE //&& DEBUG_LightSensor
            if(SetBriValue != UserPrefBrightness)
            {
                //printData("   Threshold_SetBriValue:%x", SetBriValue);
                printData("   UserPrefBrightness-3:%d", TempBrightness);
            }
#endif
        return;
    }
#if 0//DEBUG_ENABLE
            //if(SetBriValue != UserPrefBrightness)
                printData("   UserPrefBrightness:%x", UserPrefBrightness);
#endif
    mStar_AdjustBrightness(TempBrightness);

}
#endif


//=================================================================================
//volume=((DWORD)volume*63)/100+57;    //LM929 2W for Lai  
//volume=((DWORD)volume*84)/100+93;    //LM929 3W && Carrefour for Lai  
//volume=((DWORD)volume*92)/100+93;    //-0923-LM929 3W && Carrefour for Lai  
//volume=((DWORD)volume*170)/100+57;   //LM729 && Carrefour for Lu  
//volume = ( ( DWORD ) volume * 70) / 20 +90; //qx.long20061213
//volume = ( ( DWORD ) volume * 34) / 100 +160; //qx.long20061213   
// volume=((DWORD)volume*146)/100+51;   //1127 for Lai TESTING 19" AUDIO 	  
//=================================================================================
#if AudioFunc
    #if ModelName==AOC_917Fwh
    #define MaxVolumeValue 		184//224
    #define MinVolumeValue 		64
    #elif MainBoardType==MainBoard_715G2988 || MainBoardType==MainBoard_715G2808
    #define MaxVolumeValue 		224
    #define MinVolumeValue 		64
    #elif ModelName ==ENVISION_P971WALS//20130322  20130402update
     #define MaxVolumeValue 	170
     #define MinVolumeValue		20
    #elif MainBoardType == MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2
    #define MaxVolumeValue 		0xAF
    #define MinVolumeValue		0x60
    #elif(ModelName==ENVISION_P971waL_CMI)//petit 20121228
    #define MinVolumeValue		50
    #define MaxVolumeValue 		240
    #elif MainBoardType == MainBoard_715G3329_A2
	#ifdef TSUMXXNX 
    #define MaxVolumeValue 		(0xd3)
    #define MinVolumeValue		(0x00)
	#else
    #define MaxVolumeValue 		240
    #define MinVolumeValue		160
	#endif
	#elif MainBoardType == MainBoard_715G3329_58N
	#define MinVolumeValue		20
	#define MaxVolumeValue		246
	#elif MainBoardType == MainBoard_715G3329_18N
	#define MinVolumeValue		110
	#define MaxVolumeValue		255
    #elif MainBoardType==MainBoard_715G3598_M0A
    #define MaxVolumeValue 		(0xff-0x42)
    #define MinVolumeValue		(0xff-0xff)
    #elif MainBoardType==MainBoard_715G3603_M0A
	#if ModelName==AOC_E2437Fh
    #define MaxVolumeValue 		231
    #define MinVolumeValue		0
	#elif ModelName==AOC_e2237Fw2
    #define MaxVolumeValue 		255
    #define MinVolumeValue		0
	#else
    #define MaxVolumeValue 		191
    #define MinVolumeValue		0
	#endif
	
    #elif MainBoardType == MainBoard_715G3834
	  #if ModelName==ENVISION_P971waL2
       #define MaxVolumeValue 		0x8A
	   #define MinVolumeValue		0x01
      #elif(ModelName == ENV_P971WALS_TPM185) 
       #define MaxVolumeValue 		0xA3
	   #define MinVolumeValue		0x19    
	  #else
       #define MaxVolumeValue 		255
       #define MinVolumeValue		160
	  #endif
    #else
    #define MaxVolumeValue 		255
    #define MinVolumeValue		160
    #endif
	
 #if ModelName==ENVISION_P971waL2   //  hong add 20120201
     #define Volume50Value			((MaxVolumeValue-MinVolumeValue)*50/100+MinVolumeValue)
 #elif(ModelName == ENV_P971WALS_TPM185)
     #define Volume50Value			((MaxVolumeValue-MinVolumeValue)*75/100+MinVolumeValue)
 #elif(ModelName == ENVISION_P971waL_CMI)//petit 20121228
      #define Volume50Value			((MaxVolumeValue-MinVolumeValue)*50/100+MinVolumeValue)
 #else
     #define Volume50Value			((MaxVolumeValue-MinVolumeValue)*70/100+MinVolumeValue)
 #endif
void mStar_AdjustVolume(BYTE volume)
{
    if(MinVolume >= volume)
        volume = 0;
    
    if(volume)
    {
        hw_ClrMute();
#if (MainBoardType!=MainBoard_2698_2_X_X_2)&&\
	(MainBoardType!=MainBoard_2698_2_X_X_8)&&\
	(MainBoardType!=MainBoard_2810_B)&&\
	(MainBoardType!=MainBoard_715G2808)&&\
	(MainBoardType!=MainBoard_715G2988)&&\
	(MainBoardType!=MainBoard_715G3329_58N)&&\
	(MainBoardType!=MainBoard_715G3329_18N)
                    hw_ClrStandby();
#endif 
        
        if(volume >= 50)
            volume = ((DWORD)volume *(MaxVolumeValue - Volume50Value)) /50+2*Volume50Value-MaxVolumeValue;    
        else
            volume = ((DWORD)volume *(Volume50Value - MinVolumeValue)) / 50+MinVolumeValue; 
    }
    else
    {
        hw_SetMute();
    }

#ifdef InverseVolume
    mStar_WriteByte(VolumePWM, 0xff-volume); 
#else
    mStar_WriteByte(VolumePWM, volume); 
#endif
    
}
#endif 
void mStar_AdjustContrast(BYTE contrast)
{
    //contrast=0;
    BYTE redColor, greenColor, blueColor;
    #if ENABLE_HDMI && HDMICOLORTEST
    WORD ColorTemp;
    #endif
    //contrast=0;
    #if UsesRGB
    mStar_AdjustRedColor(UserPrefRedColor, UserPrefGreenColor, UserPrefBlueColor);
    mStar_AdjustGreenColor(UserPrefRedColor, UserPrefGreenColor, UserPrefBlueColor);
    mStar_AdjustBlueColor(UserPrefRedColor, UserPrefGreenColor, UserPrefBlueColor);
    #else 
    if (UserPrefColorTemp == CTEMP_Warm1)
    {
        //printData("CT93 Con:%d",UserPrefContrast);
        redColor = UserPrefRedColorWarm1;
        greenColor = UserPrefGreenColorWarm1;
        blueColor = UserPrefBlueColorWarm1;
    }
    #if 0  
    else if (UserPrefColorTemp == CTEMP_Warm2)
    {
        // printData("CT65 Con:%d",UserPrefContrast);
        redColor = UserPrefRedColorWarm2;
        greenColor = UserPrefGreenColorWarm2;
        blueColor = UserPrefBlueColorWarm2;
    }
    #endif 
    else if (UserPrefColorTemp == CTEMP_Normal)
    {
        // printData("CT65 Con:%d",UserPrefContrast);
        redColor = UserPrefRedColorNormal;
        greenColor = UserPrefGreenColorNormal;
        blueColor = UserPrefBlueColorNormal;
    }
    else if (UserPrefColorTemp == CTEMP_Cool1)
    {
        // printData("CT65 Con:%d",UserPrefContrast);
        redColor = UserPrefRedColorCool1;
        greenColor = UserPrefGreenColorCool1;
        blueColor = UserPrefBlueColorCool1;
    }
    #if 0
    else if (UserPrefColorTemp == CTEMP_Cool2)
    {
        // printData("CT65 Con:%d",UserPrefContrast);
        redColor = UserPrefRedColorCool2;
        greenColor = UserPrefGreenColorCool2;
        blueColor = UserPrefBlueColorCool2;
    }
    #endif 
    else if (UserPrefColorTemp == CTEMP_SRGB)
    {
        // printData("CTsRGB Con:%d",UserPrefContrast);
        redColor = UserPrefRedColorSRGB;
        greenColor = UserPrefGreenColorSRGB;
        blueColor = UserPrefBlueColorSRGB;
    }
    else
    //user
    {
        redColor = UserPrefRedColorUser;
        greenColor = UserPrefGreenColorUser;
        blueColor = UserPrefBlueColorUser;
	#if PanelType==PanelHSM215EU_HTN01
	 redColor -= 1;
        greenColor -= 1;
        blueColor -= 1;
	#endif
    }

	if(MenuPageIndex==ResetMenu && DoResetFlag)
	{

        redColor = UserPrefRedColorWarm1;
        greenColor = UserPrefGreenColorWarm1;
        blueColor = UserPrefBlueColorWarm1;
	}
	
	#ifdef TSUMXXT
	contrast=contrast;
	_gRedContrast = redColor;
	_gGreenContrast = greenColor;
	_gBlueContrast = blueColor;
	msAdjustPCContrastRGB( MAIN_WINDOW, contrast, _gRedContrast, _gGreenContrast, _gBlueContrast );
	#else
    #if ENABLE_HDMI && HDMICOLORTEST
	ColorTemp = ( ( DWORD ) redColor * ( contrast ) ) >> 7 ;
	if(ColorTemp>=0xff)
		redColor = 0xff;
	else
		redColor = (BYTE)ColorTemp;



	ColorTemp = ( ( DWORD ) greenColor * ( contrast ) ) >> 7 ;
	if(ColorTemp>=0xff)
		greenColor = 0xff;
	else
		greenColor = (BYTE)ColorTemp;


	ColorTemp = ( ( DWORD ) blueColor * ( contrast ) ) >> 7 ;
	if(ColorTemp>=0xff)
		blueColor = 0xff;
	else
		blueColor = (BYTE)ColorTemp;	

    mStar_WriteByte(CCR, redColor);
    mStar_WriteByte(CCG, greenColor);
    mStar_WriteByte(CCB, blueColor);
    msAdjustMainWinContrast(redColor, greenColor , blueColor);
    #else
    mStar_AdjustRedColor(redColor, contrast);
    mStar_AdjustGreenColor(greenColor, contrast);
    mStar_AdjustBlueColor(blueColor, contrast);

    #endif
	#endif
        // 2006/10/24 4:59PM by Emily for can't save subcontrast issue   	UserPrefSubContrast=DefContrastBase+UserPrefContrast;
        // 2006/11/10 10:40PM by KK     if(!UserPrefDcrMode) // for under DCR mode, the picture show filter
        // msAdjustVideoContrast(DefContrastBase+UserPrefContrast );
    #endif 
}
#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
#else
void mStar_AdjustHTotal(WORD htotal)
{
    mStar_WriteByte(REGBK, REGBANKADC);
    htotal -= 3;
    #ifdef TSUMXXNX
        mStar_WriteByte ( PLLDIVM, ( htotal & 0xFF )  );        
        mStar_WriteByte ( PLLDIVL,  ( htotal >> 8) & 0x1F );
    #else
        mStar_WriteByte(PLLDIVM, htotal >> 4);
        mStar_WriteByte(PLLDIVL, (htotal &0xF) << 4);
     #endif
    mStar_WriteByte(REGBK, REGBANKSCALER);
}
void mStar_AdjustPhase(BYTE phase)
{
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(CLKCTRL1, phase + 8);
    mStar_WriteByte(CLKCTRL2, phase);
    mStar_WriteByte(REGBK, REGBANKSCALER);
}
#endif
void mStar_AdjustHPosition(WORD position)
{
    mStar_WriteWord(SPRHST_H, position);
}

void mStar_AdjustVPosition(WORD position)
{
    if (DecVScaleFlag)
    {
        if (position <= (VScaleDecValue / 2))
            position = VScaleDecValue / 2+1;
        mStar_WriteWord(SPRVST_H, position - (VScaleDecValue / 2));
    }
    else
    {
        if (position == 0)
            position = 1;
        mStar_WriteWord(SPRVST_H, position);
    }
}
#if UsesRGB
    // Note: Contrast range should be form 0.4 to 1
    #define RedCoeff1(contrast, color)\
    (Slong)(((Slong)299*(contrast+45))/300+701)*color/500
    #define RedCoeff(contrast, color)\
    (Slong)((Slong)((Slong)299*(contrast+45))/300+701)*color
    #define RedCoeff2(contrast, color, divider)\
    (Slong)((Slong)((Slong)299*(contrast+45))/300-299)*color*128/(Slong)divider
    #define GreenCoeff1(contrast, color)\
    (Slong)((Slong)((Slong)587*(contrast+45))/300+413)*color/500
    #define GreenCoeff(contrast, color)\
    (Slong)((Slong)((Slong)587*(contrast+45))/300+413)*color
    #define GreenCoeff2(contrast, color, divider)\
    (Slong)((Slong)((Slong)587*(contrast+45))/300-587)*color*128/(Slong)divider
    #define BlueCoeff1(contrast, color)\
    (Slong)((Slong)((Slong)114*(contrast+45))/300+886)*color/500
    #define BlueCoeff(contrast, color)\
    (Slong)((Slong)((Slong)114*(contrast+45))/300+886)*color
    #define BlueCoeff2(contrast, color, divider)\
    (Slong)((Slong)((Slong)114*(contrast+45))/300-114)*color*128/(Slong)divider
    void mStar_AdjustRedColor(BYTE rColor, BYTE gColor, BYTE bColor)
    {
        Slong tempColor, tempColor1;
        tempColor1 = RedCoeff(UserPrefContrast, rColor);
        tempColor = GreenCoeff2(UserPrefContrast, gColor, tempColor1);
        if (tempColor < 0)
        {
            tempColor = abs(tempColor) | BIT7;
        }
        mStar_WriteByte(SRGB12, tempColor);
        tempColor = BlueCoeff2(UserPrefContrast, bColor, tempColor1);
        if (tempColor < 0)
        {
            tempColor = abs(tempColor) | BIT7;
        }
        mStar_WriteByte(SRGB13, tempColor);
        tempColor = (Slong)tempColor1 / 500; // RedCoeff1(UserPrefContrast, color);
        rColor = tempColor;
        if (tempColor > 0xFF)
        {
            rColor = 0xFF;
        }
        else
        {
            rColor = (BYTE)tempColor;
        }
        mStar_WriteByte(CCR, rColor); //+UserPrefContrast);
    }
    void mStar_AdjustGreenColor(BYTE rColor, BYTE gColor, BYTE bColor)
    {
        Slong tempColor, tempColor1;
        tempColor1 = GreenCoeff(UserPrefContrast, gColor);
        tempColor = RedCoeff2(UserPrefContrast, rColor, tempColor1);
        if (tempColor < 0)
        {
            tempColor = abs(tempColor) | BIT7;
        }
        mStar_WriteByte(SRGB21, tempColor);
        tempColor = BlueCoeff2(UserPrefContrast, bColor, tempColor1);
        if (tempColor < 0)
        {
            tempColor = abs(tempColor) | BIT7;
        }
        mStar_WriteByte(SRGB23, tempColor);
        tempColor = (Slong)tempColor1 / 500; // GreenCoeff1(UserPrefContrast, color);
        gColor = tempColor;
        if (tempColor > 0xFF)
        {
            gColor = 0xFF;
        }
        else
        {
            gColor = (BYTE)tempColor;
        }
        mStar_WriteByte(CCG, gColor); //+UserPrefContrast);
    }
    void mStar_AdjustBlueColor(BYTE rColor, BYTE gColor, BYTE bColor)
    {
        Slong tempColor, tempColor1;
        tempColor1 = BlueCoeff(UserPrefContrast, bColor);
        tempColor = RedCoeff2(UserPrefContrast, rColor, tempColor1);
        if (tempColor < 0)
        {
            tempColor = abs(tempColor) | BIT7;
        }
        mStar_WriteByte(SRGB31, tempColor);
        tempColor = GreenCoeff2(UserPrefContrast, gColor, tempColor1);
        if (tempColor < 0)
        {
            tempColor = abs(tempColor) | BIT7;
        }
        mStar_WriteByte(SRGB32, tempColor);
        tempColor = (Slong)tempColor1 / 500; // BlueCoeff1(UserPrefContrast, color);
        bColor = tempColor;
        if (tempColor > 0xFF)
        {
            bColor = 0xFF;
        }
        else
        {
            bColor = (BYTE)tempColor;
        }
        mStar_WriteByte(CCB, bColor); //+UserPrefContrast);
    }
#else 
//wmz 20051017 for response time
void mStar_AdjustRedColor(BYTE color, BYTE contrast)
{
	#ifdef TSUMXXT// #if( ChipID == TSUMXXT )		//110311 Modify
	{
		_gRedContrast = color;
		//msAdjustPCRGB( MAIN_WINDOW, _gRedContrast, _gGreenContrast, _gBlueContrast );
		msAdjustPCContrastRGB( MAIN_WINDOW, contrast, _gRedContrast, _gGreenContrast, _gBlueContrast );
	}
  	#else
    WORD color1;
    //color=color+DefRGBgainBase;
    //UserPrefSubRedGain=color;
    //msAdjustVideoRGB( 1, UserPrefSubRedGain,UserPrefSubGreenGain,UserPrefSubBlueGain);
    color1 = ((DWORD)color *(contrast)) >> 7;
    if (color1 >= 0xff)
    {
        color1 = 0xff;
    }
    color = color1; //gaohp
    mStar_WriteByte(CCR, color); //+UserPrefContrast);
    
	#if ENABLE_HDMI && HDMICOLORTEST
    msAdjustMainWinRedColor(color);
	#endif
    #endif
}
void mStar_AdjustGreenColor(BYTE color, BYTE contrast)
{
	#ifdef TSUMXXT// #if( ChipID == TSUMXXT )		//110311 Modify
	{
		_gGreenContrast = color;
		//msAdjustPCRGB( MAIN_WINDOW, _gRedContrast, _gGreenContrast, _gBlueContrast );
		msAdjustPCContrastRGB( MAIN_WINDOW, contrast, _gRedContrast, _gGreenContrast, _gBlueContrast );
	}
 	#else
    WORD color1;
    //color=color+DefRGBgainBase;
    //UserPrefSubGreenGain=color;
    //msAdjustVideoRGB( 1, UserPrefSubRedGain,UserPrefSubGreenGain,UserPrefSubBlueGain);
    color1 = ((DWORD)color *(contrast)) >> 7;
    if (color1 >= 0xff)
    {
        color1 = 0xff;
    }
    color = color1; //gaohp
    mStar_WriteByte(CCG, color); //+UserPrefContrast);

    #if (ENABLE_HDMI && HDMICOLORTEST)
    msAdjustMainWinGreenColor(color);
	#endif
    #endif
}
void mStar_AdjustBlueColor(BYTE color, BYTE contrast)
{
	#ifdef TSUMXXT//  #if( ChipID == TSUMXXT )		//110311 Modify
	{
		_gBlueContrast = color;
		//msAdjustPCRGB( MAIN_WINDOW, _gRedContrast, _gGreenContrast, _gBlueContrast );
		msAdjustPCContrastRGB( MAIN_WINDOW, contrast, _gRedContrast, _gGreenContrast, _gBlueContrast );
	}
  	#else
    WORD color1;
    //color=color+DefRGBgainBase;
    //UserPrefSubBlueGain=color;
    //msAdjustVideoRGB( 1, UserPrefSubRedGain,UserPrefSubGreenGain,UserPrefSubBlueGain);
    color1 = ((DWORD)color *(contrast)) >> 7;
    if (color1 >= 0xff)
    {
        color1 = 0xff;
    }
    color = color1; //gaohp
    mStar_WriteByte(CCB, color); //+UserPrefContrast);
	
	#if (ENABLE_HDMI && HDMICOLORTEST)
	msAdjustMainWinBlueColor(color);
	#endif
    #endif
}
    /********
    void mStar_AdjustRedColor(BYTE color,BYTE contrast)
    { //color=((DWORD)color*(UserPrefContrast))>>7;
    color=((DWORD)color*(contrast))>>7;
    mStar_WriteByte(CCR, color);//+UserPrefContrast);
    }
    void mStar_AdjustGreenColor(BYTE color,BYTE contrast)
    { //color=((DWORD)color*(UserPrefContrast))>>7;
    color=((DWORD)color*(contrast))>>7;
    mStar_WriteByte(CCG, color);//+UserPrefContrast);
    }
    void mStar_AdjustBlueColor(BYTE color,BYTE contrast)
    { //color=((DWORD)color*(UserPrefContrast))>>7;
    color=((DWORD)color*(contrast))>>7;
    mStar_WriteByte(CCB, color);//+UserPrefContrast);
    }
     ************/
#endif 
#if 0 // 2006/9/18 5:58PM by Emily temp
    void mStar_AdjustBlackLevel(BYTE BlackLevel)
    {
        #if WidePanel
            BlackLevel = BlackLevel + 77;
        #else 
            BlackLevel = (BlackLevel *0.54) + 0x65;
        #endif 
        //printData("Black Level %d",BlackLevel);
        mStar_WriteByte(BCR, BlackLevel);
        mStar_WriteByte(BCG, BlackLevel);
        mStar_WriteByte(BCB, BlackLevel);
    }
#endif 
#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
#else
void mStar_AdjustAdcGain(BYTE rGain, BYTE gGain, BYTE bGain)
{
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(REDGAIN, 0xFF - rGain);
    mStar_WriteByte(GRNGAIN, 0xFF - gGain);
    mStar_WriteByte(BLUGAIN, 0xFF - bGain);
    mStar_WriteByte(REGBK, REGBANKSCALER);
}

void mStar_AdjustAdcOffset(BYTE rOffset, BYTE gOffset, BYTE bOffset)
{
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(REDOFST, 0xFF - rOffset);
    mStar_WriteByte(GRNOFST, 0xFF - gOffset);
    mStar_WriteByte(BLUOFST, 0xFF - bOffset);
    mStar_WriteByte(REGBK, REGBANKSCALER);
}

    void mStar_AdjustAdcRedGain(BYTE gain)
    {
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(REDGAIN, 0xFF - gain);
        mStar_WriteByte(REGBK, REGBANKSCALER);
    }
    void mStar_AdjustAdcGreenGain(BYTE gain)
    {
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(GRNGAIN, 0xFF - gain);
        mStar_WriteByte(REGBK, REGBANKSCALER);
    }
    void mStar_AdjustAdcBlueGain(BYTE gain)
    {
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(BLUGAIN, 0xFF - gain);
        mStar_WriteByte(REGBK, REGBANKSCALER);
    }
    void mStar_AdjustAdcRedOffset(BYTE offset)
    {
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(REDOFST, 0xFF - offset);
        mStar_WriteByte(REGBK, REGBANKSCALER);
    }
    void mStar_AdjustAdcGreenOffset(BYTE offset)
    {
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(GRNOFST, 0xFF - offset);
        mStar_WriteByte(REGBK, REGBANKSCALER);
    }
    void mStar_AdjustAdcBlueOffset(BYTE offset)
    {
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(BLUOFST, 0xFF - offset);
        mStar_WriteByte(REGBK, REGBANKSCALER);
    }
#endif 


// 100802 coding modify
#define RETRY_COUNT 0xffff

void mStar_WaitForDataBlanking(void)
{
    WORD retry;

    if(SyncLossState()||!PanelOnFlag)
        return;

    retry = RETRY_COUNT;        
  
    while (retry-- && !InputTimingChangeFlag && (mStar_ReadByte(ASCTRLL) &IVB_B));

    if(SyncLossState()||!PanelOnFlag)
        return;

    retry = RETRY_COUNT;        

    while (retry-- && !InputTimingChangeFlag && !(mStar_ReadByte(ASCTRLL) &IVB_B));
    
}
void mStar_AdjustBackgoundColor(BYTE pattern)
{
    BYTE rColor = 0;
    BYTE gColor = 0;
    BYTE bColor = 0;
    if (pattern == 0)
    {
        rColor = 0xFF;
    }
    else if (pattern == 1)
    {
        gColor = 0xFF;
    }
    else if (pattern == 2)
    {
        bColor = 0xFF;
    }
    else if (pattern == 4)
    {
    	#if(DisplayLogo==WearnesLOGO)
        rColor = 0x00;
        gColor = 0x00;
        bColor = 0x00;
	#else
        rColor = 0xFF;
        gColor = 0xFF;
        bColor = 0xFF;
	 #endif
    }
    mStar_WaitForDataBlanking();
    mStar_WriteByte(FCR, rColor);
    mStar_WriteByte(FCG, gColor);
    mStar_WriteByte(FCB, bColor);
}
