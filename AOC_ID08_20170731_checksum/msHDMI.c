#ifndef		_MSHDMI_C_
#define _MSHDMI_C_
#include <math.h>
#include "types.h"
#include "board.h"
#include "Global.h"
#include "misc.h"
#include "Adjust.h"
#include "msHDMIRegMap.h"
#include "msHDMI.h"
#include "Debug.h"

#if ENABLE_HDMI

#if MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
extern void SetScalerGPIO(BYTE pinX, bit HiLow);
#endif

extern void Delay1ms(WORD msNums);
extern void mStar_WriteByte(BYTE addr, BYTE value);
extern BYTE mStar_ReadByte(BYTE addr);

void mstar_HDMIInitialVariable(void)
{
	gScInfo.InputColor = INPUT_RGB;
	gScInfo.InputSource =INPUT_TMDS_A;
	gScInfo.InputTmdsType=TMDS_DVI;
	gScInfo.ColorChanged=0;
	gScInfo.ModeChange=0;
	gScInfo.wAviPktLossCnt=0;
	gScInfo.InputPacketStatus = 0;
	gScInfo.AudioOffDebunce=0;
	gScInfo.InputPacketError=0;
	gScInfo.InputVideoMode = 16; // check here
	gScInfo.InputAVIInfoReceived=0;
	gScInfo.OutputAudioState=HDMI_AUDIO_OFF;//Mute
	gScInfo.OutputDataState=HDMI_DATA_OUTPUT_OFF;
    #if CheckCTSStable
    gScInfo.InputCTSValue=0;
    gScInfo.OutputAudioCnt=AudioDeBounce;
    #endif
}
///////////////////////////////////////////////////////////////////////////////
// <Function>: msInitialHDMI
//
// <Description>: HDMI initial Audio function
//
// <Parameter>: None
//
///////////////////////////////////////////////////////////////////////////////


#ifdef AUMUTE_OSEL
#define AUMUTE_B	BIT5
#else
#define AUMUTE_B	0
#endif
#ifdef SPDIF_OSEL
#define SPDIF_B		BIT4
#else
#define SPDIF_B		0
#endif
BYTE code tblAudioInit[][2] =
{
    {REGBK,  0x05},
#if (G2_G3_HDMI_AUDIO_FADING||ENABLE_CTSNSYNC||MS_DAC)
    {BK5_05, 0x07},
#else
    {BK5_05, 0x01},
#endif
    {BK5_06, 0x86}, // 0xD6 100526 // 0x56 091007
    {BK5_07, 0x02},
    {BK5_07, 0x00},
    {BK5_08, 0x9A},
#if EN_HDMI_NEW_PIXEL_REPETITION
    {BK5_09, 0x00},
#else
    {BK5_09, 0x04},
#endif	
    {BK5_0A, 0x02}, // 080131
    {BK5_10, 0x00},	// {BK5_10, 0x0F|AUMUTE_B|SPDIF_B} 100526//{BK5_10, 0x2F},//P4_2,SPDIF  as GPIO
    {BK5_11, 0xAA},
    {BK5_12, 0xA0},
    {BK5_13, 0xAA}, // 0x8A 080131
#if G2_G3_HDMI_AUDIO_FADING
    {BK5_19, 0xC0}, //0xc4 091007// 0x40 080131
#else
    {BK5_19, 0x80},
#endif
    {BK5_1A, 0xC0},
    {BK5_1B, 0xD0},
    {BK5_1C, 0x00},
    {BK5_22, 0x10},
#if ENABLE_CTSNSYNC
    {BK5_2E, 0x03},
    {BK5_B9, 0xA1},
#endif
#if MS_DAC
    {BK5_2F, 0x05},
    {BK5_0B, 0x00},
    {BK5_0C, 0x26},
#endif
#if EN_HDMI_1_3
    {BK5_F0, 0x03},
    {BK5_F1, 0x40},
#endif
#ifdef TSUMXXQX
    {BK5_1F, 0x1A}, 	//Code_Update	  Aupll Power On Default.
    {BK5_F5, 0x00}, 	//Code_Update	Vpll  Power On Default.
#endif

};
void mstar_HDMIInitialAudio(void)
{
    BYTE i=0;
    BYTE ucBank;

    ucBank=mStar_ReadByte(0x00);
    for (i=0; i<sizeof(tblAudioInit)/2; i++)
        mStar_WriteByte(tblAudioInit[i][0], tblAudioInit[i][1]);

       mStar_WriteByte(BK5_CB,0x2F);
	
	mStar_WriteByte(0x00, ucBank);//REGBK

}

void mstar_HDMIHandler(void)
{
	mstar_MonitorHDMI();
	mstar_HDMIHandleColor();
	mstar_HDMIHandleMode();
}


void mstar_HDMIRst(void)
{
	BYTE reg_bank;
	reg_bank=mStar_ReadByte(0x00);
	mStar_WriteByte(0x00,0x00);
	mStar_WriteByte(0xFB,BIT1);
	mStar_WriteByte(0xFB,0);
	mStar_WriteByte(0x00,reg_bank);
}

void mstar_HDMIHandleMode(void)
{
    BYTE reg_bank,temp;

    if(gScInfo.ModeChange)
    {
        #if DEBUG_HDMI
        printData("TMDS mode is=%d",gScInfo.InputTmdsType);
        #endif
        gScInfo.ModeChange=0;
        reg_bank=mStar_ReadByte(0x00);
        mstar_HDMIRst();

        temp=mStar_ReadByte(0x02)&0xF8;
        if(gScInfo.InputTmdsType==TMDS_DVI)
        {
            mStar_WriteByte(0x02,temp|0x02);
            //hw_SEL_DVI();  //20071017 for switch input port
        }
        else
        {
            mStar_WriteByte(0x02,temp|0x07);
            mstar_HDMIAudioMute(HDMI_AUDIO_OFF);    // Mute DVI Change to HDMI Sync
            //hw_SEL_HDMI();  //20071017 for switch input port
        }
        mStar_WriteByte(0x00,reg_bank);
    }
}

void SetMainWinVideoDomain(BYTE Domain)	//void SetMainWinColorControl(BYTE ucControl)
{
    BYTE XDATA ucBank,ucReg;

    ucBank = mStar_ReadByte(REGBK);
    mStar_WriteByte(REGBK, REG_BANK_PIP);
    ucReg = mStar_ReadByte(BK3_71);
    if (Domain)
        ucReg|=BIT3;
    else
        ucReg&=~BIT3;

    mStar_WriteByte(REGBK, REG_BANK_SCALER);
    if (Domain)
    {
        mStar_WaitForDataBlanking();
        mStar_WriteByte(BK0_2B, 0x70);
        mStar_WriteByte(BK0_2C, 0x70);
        mStar_WriteByte(BK0_2D, 0x70);
        mStar_WriteByte(BK0_02, mStar_ReadByte(BK0_02)&~BIT3);
        mStar_WriteByte(REGBK, REG_BANK_PIP);
        mStar_WriteByte(0x71, ucReg|BIT2);
        mStar_WriteByte(0x70, 0x15);
        mStar_WriteByte(0x71, ucReg);
        mStar_WriteByte(REGBK, REG_BANK_SCALER);
    }
    else
    {
        mStar_WaitForDataBlanking();
        mStar_WriteByte(BK0_2B, 0x80);
        mStar_WriteByte(BK0_2C, 0x80);
        mStar_WriteByte(BK0_2D, 0x80);
        mStar_WriteByte(REGBK, REG_BANK_PIP);
        mStar_WriteByte(0x71, ucReg|BIT2);
        mStar_WriteByte(0x70, 0x10);
        mStar_WriteByte(0x71, ucReg);
        mStar_WriteByte(REGBK, REG_BANK_SCALER);
    }
    mStar_WriteByte(REGBK, ucBank);
}

void mstar_HDMIHandleColor(void)
{
    BYTE XDATA ucBank,ucReg,ucDomain,ucColorEhance;
    
    if(gScInfo.ColorChanged)
    {
	    if(mstar_HDMIPacketColor()!=PreHDMIColor)
		{
			Set_InputTimingChangeFlag();
			PreHDMIColor=mstar_HDMIPacketColor();
			if(!DisplayLogoFlag)
			   hw_ClrBlacklit();
		}
        #if DEBUG_HDMI
        printData("Handle HDMI CSC%d",gScInfo.InputColor);
        #endif
        gScInfo.ColorChanged=0;
        ucDomain = 0x00;
        ucColorEhance =0x00;
        ucBank = mStar_ReadByte(REGBK);
        mStar_WriteByte(REGBK,REG_BANK_SCALER);

        if(mStar_ReadByte(BK0_5C)&BIT3)
            ucColorEhance = 0xFF;

        if(gScInfo.InputColor!=INPUT_RGB)
            ucDomain = 0xFF;

        if(ucColorEhance)
        {
            #if DEBUG_HDMI
            printData("With Color Enhance XXX=%x",ucDomain);
            #endif
            ucReg=mStar_ReadByte(BK0_02)&(~BIT3);
            mStar_WriteByte(BK0_02,ucReg);
            ucReg = mStar_ReadByte(BK0_5A)&0x3F;
            mStar_WriteByte(BK0_5A, ucReg|((~ucDomain) & BIT6));
            SetMainWinVideoDomain(ucDomain);
        }
        else
        {
            #if DEBUG_HDMI
            printData("Without Color Enhance YYY=%x",ucDomain);
            #endif
            ucReg=mStar_ReadByte(BK0_02)&(~BIT3);
            mStar_WriteByte(0x02,ucReg|(ucDomain&BIT3));
            ucReg=mStar_ReadByte(BK0_5A)&0x3F;
            mStar_WriteByte(BK0_5A,ucReg);
            SetMainWinVideoDomain(0x00);
        }
        mStar_WriteByte(REGBK,ucBank);
    }
}

void mstar_HDMIResetColorSpace(void)
{
    BYTE temp,reg_bank;
    
    #if DEBUG_HDMI
    printData("HDMIResetColorSpace ",1);
    #endif
    reg_bank=mStar_ReadByte(0x00);
    mStar_WriteByte(0x00,0x00);

/*    if(mStar_ReadByte(0x5C)&BIT3)
    {
        temp=mStar_ReadByte(0x5A)|BIT7|BIT6;
        mStar_WriteByte(0x5A,temp);
    }*/
    temp=mStar_ReadByte(0x02)&(~BIT3);
    mStar_WriteByte(0x02,temp);
    SetMainWinVideoDomain(INPUT_RGB);		//MainWin Color Space Change YUV -> RGB
    mStar_WriteByte(0x00,reg_bank);
}

void mstar_MonitorHDMI(void)
{
	BYTE temp,curr_color;
	if(HdmiPollingCounter==0)
	{
		HdmiPollingCounter=20;

		if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2)
		{
			if(SrcFlags&(SyncLoss|bUnsupportMode)) // unsupport mode
			{
				if(gScInfo.OutputAudioState!=HDMI_AUDIO_OFF)
					mstar_HDMIAudioMute(HDMI_AUDIO_OFF);  // mute audio
			}
			else
			{
				temp=mstar_HDMITmdsGetType();
				if(temp!=gScInfo.InputTmdsType)
				{
					gScInfo.InputTmdsType=temp;
					gScInfo.InputPacketStatus=0;
					gScInfo.ModeChange=1;
					return;
				}

				mstar_HDMIGetPktStatus();
                if (gScInfo.InputTmdsType ==TMDS_HDMI && InputTimingStableFlag && PowerOnFlag)
				mstar_HDMICheckAudio();


				if(gScInfo.InputPacketStatus&BIT3)
				{
					gScInfo.wAviPktLossCnt=0;
					curr_color=mstar_HDMIPacketColor();
					if(curr_color!=gScInfo.InputColor)
					{
						gScInfo.InputColor=curr_color;
						gScInfo.ColorChanged=1;
					}
				}
				else
				{
					if(gScInfo.wAviPktLossCnt>=AviPktLossBound)
					{
						if(gScInfo.InputColor!=INPUT_RGB)
						{
							gScInfo.InputColor=INPUT_RGB;
							mstar_HDMIRst();
							gScInfo.ColorChanged = 1;
						}
					}
				}
			}
		}
	}
}

void mstar_HDMICheckAudio(void)
{
    BYTE temp;

    if(gScInfo.InputPacketError&BIT6) //audio sample error
        temp=HDMI_AUDIO_OFF;
    else if((gScInfo.InputPacketStatus&(BIT6|BIT5))==(BIT6|BIT5))
    {
      #if 1 
        if (UserPrefVolume==0)
            temp = HDMI_AUDIO_OFF;
        else
      #endif 
		if(gScInfo.OutputDataState==HDMI_DATA_OUTPUT_ON)
			temp=HDMI_AUDIO_ON;
		else
			temp=HDMI_AUDIO_OFF;


        #if CheckCTSStable
        if (gScInfo.OutputAudioCnt)     // Check CTS Value Stable
            temp=HDMI_AUDIO_OFF;
        #endif
	}
	else
		temp=HDMI_AUDIO_OFF;
#if 1
	if(gScInfo.OutputAudioState==HDMI_AUDIO_ON && temp==HDMI_AUDIO_OFF)
	{
		if(++gScInfo.AudioOffDebunce<20)
			return ;
	}
	else
		gScInfo.AudioOffDebunce=0;
#endif
	if(temp!=gScInfo.OutputAudioState)
	{
		mstar_HDMIAudioMute(temp);
	}
}

BYTE mstar_HDMIPacketColor(void)
{
	BYTE temp,reg_bank;
	reg_bank=mStar_ReadByte(0x00);
	mStar_WriteByte(0x00,0x05);
	temp=INPUT_RGB;
	switch((mStar_ReadByte(0x37)&0x60)>>5)
	{
		case 0:
			temp=INPUT_RGB;
			break;
		case 1:
			temp=INPUT_YUV422;
			break;
		case 2:
			temp=INPUT_YUV444;
			break;
	}
	mStar_WriteByte(0x00,reg_bank);
	return temp;
}


void mstar_HDMIAudioMute(BYTE sw)
{
    BYTE temp,reg_bank;
    reg_bank=mStar_ReadByte(0x00);
    mStar_WriteByte(0x00,0x05);
    gScInfo.OutputAudioState=sw;
    #if DEBUG_HDMI
    printData("--gScInfo.OutputAudioState %d", gScInfo.OutputAudioState);
    #endif

    if(sw==HDMI_AUDIO_ON)
    {
    	 #if MainBoardType != MainBoard_715G3603_M0A && MainBoardType!=MainBoard_715G3663_M0A
    	 hw_Clr_AV_Mute();
        #endif         
        hw_ClrMute();
    }
    else
    {
        #if MainBoardType != MainBoard_715G3603_M0A && MainBoardType!=MainBoard_715G3663_M0A
        hw_Set_AV_Mute();
        #endif
    	 mStar_AdjustVolume(0);
    }
    temp=mStar_ReadByte(0x07)&(~BIT0);
    mStar_WriteByte(0x07,temp|sw); // mute audio
    mStar_WriteByte(0x00,reg_bank);
}

void mstar_HDMIGetPktStatus(void)
{
    #if CheckCTSStable
    BYTE ucBank,ucReg;
    #else
    BYTE ucBank;
    #endif

    ucBank=mStar_ReadByte(0x00);
	mStar_WriteByte(0x00,0x05);
	gScInfo.InputPacketStatus=(mStar_ReadByte(0x02)<<8)+mStar_ReadByte(0x01);
	gScInfo.InputPacketError=mStar_ReadByte(0x04);
    #if CheckCTSStable
    ucReg=mStar_ReadByte(BK5_30);
    if (abs(ucReg - gScInfo.InputCTSValue)<CTSRange||ucReg <= 5)    // for Check PC CTS Stable
    {
        if (gScInfo.OutputAudioCnt)
            gScInfo.OutputAudioCnt--;
    }
    else
        gScInfo.OutputAudioCnt = AudioDeBounce;

   if( ucReg  >  5)
    gScInfo.InputCTSValue = ucReg;
    #endif

    mStar_WriteByte(0x00,ucBank);

	if(gScInfo.InputPacketStatus&BIT3)
		gScInfo.InputAVIInfoReceived=1;
}


BYTE mstar_HDMITmdsGetType(void)
{
	BYTE ret_type, temp,reg_bank;
	ret_type = TMDS_DVI;;    //default DVI

	reg_bank=mStar_ReadByte(0x00);
	mStar_WriteByte(0x00,0x01);
	temp = mStar_ReadByte(0x59);
	mStar_WriteByte(0x00,reg_bank);

	if (temp & BIT0)
		ret_type = TMDS_HDMI;
	return ret_type;
}



#else

BYTE code nullData[1]=
{
    0xFF
};

#endif//#if ENABLE_HDMI








#endif
