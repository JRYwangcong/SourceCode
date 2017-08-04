#include "types.h"
#include "board.h"
#include "global.h"
#include "keypaddef.h"
#include "menudef.h"
#include "adjust.h"
#include "ms_reg.h"
#include "debug.h"
#include "msosd.h"
#include "power.h"
#include "misc.h"
#include "autofunc.h"
#include "nvram.h"
#include "userpref.h"
#include "mstar.h"
#include "ms_rwreg.h"
#include "adjust.h"
#include "mcu.h"
#include "menustr.h"
#include "Gamma.h"
#include "ddc2bi.h"
#if MWEFunction
    #include "MsDLC.H"
    #include "MsACE.H"
    #include "MsRegTV.h"
    #include "Panel.h"
#endif
#if ENABLE_HDCP
    #include "MsHDCP.h"
#endif
#if MS_OVD
#include "msovd_v03.h"
#endif
#if ENABLE_HDMI
#include "MsHDMI.h"
#include "msHDMIACE.h"
#endif
#if 1//USEFLASH
    #include "msflash.h"
#endif 
#if ScanKey
#include "scankey.h"
#endif
#ifdef MS_PM
#include "MS_PM.h"
#endif
#include "MsID_V1.h"
#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
#include "msADC.h"
#endif
#if PANEL_VCOM_ADJUST		//121031 Modify
#include "msMiniLVDSI2c.h"
#endif
#if MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A || MainBoardType==MainBoard_715G3959_T
extern void SetScalerGPIO(BYTE pinX, bit HiLow);
#endif

void SaveUserPref(void);
void ReadWarm1Setting(void);
void ReadWarm2Setting(void);
void ReadNormalSetting(void);
void ReadCool1Setting(void);
void ReadCool2Setting(void);
void ReadSRGBSetting(void);
void RunLanguage(void);
extern Bool ExecuteKeyEvent(MenuItemActionType menuAction);

#if Enable_Expansion
    void SetExpansionMode(void);
    Bool AdjustExpansionMode(void);
#endif
WORD DecIncValue(MenuItemActionType action, WORD value, WORD minValue, WORD maxValue, BYTE step);
WORD GetScale100Value(WORD value, WORD minValue, WORD maxValue);
void SetYCMColor(MenuItemActionType action, WORD color, WORD value);
void SetECO(void);
//Bool SetBFSize(void);
Bool SetBFSize(BYTE ucSize);
void ReadColorTempSetting(void);
void ReadUserSetting(void);
extern void mStar_SetUserPref(void);
Bool WriteRegValue(void);
extern void mStar_SetupGamma(BYTE GamaMode);

//void Save9300KSetting(void);
//void Save6500KSetting(void);
BYTE* code ColorTempPtr[5][5]=
{
#if 0
  {&UserPrefBrightnessCool1, &UserPrefContrastCool1, &UserPrefRedColorCool1, &UserPrefGreenColorCool1, &UserPrefBlueColorCool1},
  {&UserPrefBrightnessNormal, &UserPrefContrastNormal, &UserPrefRedColorNormal, &UserPrefGreenColorNormal, &UserPrefBlueColorNormal},
  {&UserPrefBrightnessWarm1, &UserPrefContrastWarm1, &UserPrefRedColorWarm1, &UserPrefGreenColorWarm1, &UserPrefBlueColorWarm1},
  {&UserPrefBrightnessSRGB, &UserPrefContrastSRGB, &UserPrefRedColorSRGB, &UserPrefGreenColorSRGB, &UserPrefBlueColorSRGB},
  {&UserPrefBrightnessUser, &UserPrefContrastUser, &UserPrefRedColorUser, &UserPrefGreenColorUser, &UserPrefBlueColorUser},
#else
  {&UserPrefBrightnessWarm1, &UserPrefContrastWarm1, &UserPrefRedColorWarm1, &UserPrefGreenColorWarm1, &UserPrefBlueColorWarm1},
  {&UserPrefBrightnessNormal, &UserPrefContrastNormal, &UserPrefRedColorNormal, &UserPrefGreenColorNormal, &UserPrefBlueColorNormal},
  {&UserPrefBrightnessCool1, &UserPrefContrastCool1, &UserPrefRedColorCool1, &UserPrefGreenColorCool1, &UserPrefBlueColorCool1},
  {&UserPrefBrightnessSRGB, &UserPrefContrastSRGB, &UserPrefRedColorSRGB, &UserPrefGreenColorSRGB, &UserPrefBlueColorSRGB},
  {&UserPrefBrightnessUser, &UserPrefContrastUser, &UserPrefRedColorUser, &UserPrefGreenColorUser, &UserPrefBlueColorUser},
#endif
};
#if MWEFunction
    //////////////////////////////////////////////////////////////////////////
    // Matrix for convert to sRGB space
    //////////////////////////////////////////////////////////////////////////
    code short tSRGB[3][3] =
    {
    { 1096, -43, -28 }, // R  1.0694, -0.0424, -0.0270
    { - 21,   1063, -18 }, // G -0.0204,  1.0376, -0.0172
    { - 1,   34,   991 }  // B -0.0009,  0.0330,  0.9679

    };
    code short tNormalColorCorrectionMatrix[3][3] =
    {
	{ 1024,    0,      	0},
   {	  0,	1024,     	0},
   {    0,     0,		1024}
};
    //////////////////////////////////////////////////////////////////////////
    // Matrix for VEDIO color correcttion
    //////////////////////////////////////////////////////////////////////////
    /*
    code short tVedioColorCorrectionMatrix[3][3] =
    {
    { 992,   -88,   28 },
    { -181, 1177,  -52 },
    { -76,   -82, 1241 }
    };
    code short tBlueColorCorrectionMatrix[3][3] =
    {
    { 1100,    77,      -154},
    {	0,		1170,     -154},
    {    0,     0,     1024}
    };
    code short tGreenColorCorrectionMatrix[3][3] =
    {
    { 1100,   -154,   77 },
    { 0, 1024,  -0 },
    { 77,   -154, 1100 }
    };
    code short tRedFleshColorCorrectionMatrix[3][3] =
    {
    { 1074,    -25,      -25},
    {		-100,	1124,  0},
    {    0,     0,     1024}
    };
    code short tYellowFleshColorCorrectionMatrix[3][3] =
    {
    { 1024,    0,      0},
    {		0,	1024,     0},
    {    -154,     0,     1178}
    };
    code short t_FR_ColorCorrectionMatrix[3][3] =
    {
    { 1126,    -102,      	0},
    {	  -102,	1126,     	0},
    {    0,     0,		1024}
    };
    code short t_FY_ColorCorrectionMatrix[3][3] =
    {
    { 1024,    0,      	0},
    {	  0,	1024,     	0},
    {    -123,     0,		1147}
    };
    code short t_B_ColorCorrectionMatrix[3][3] =
    {
    { 1126,    102,      	-205},
    {	  102,	1024,     	-205},
    {    51,     0,		1024}
    };
    code short t_G_ColorCorrectionMatrix[3][3] =
    {
    { 1126,    -230,      	128},
    {	  0,	1024,     	0},
    {    128,     -230,		1126}
    };
    code short t_G_EnhanceColorCorrectionMatrix[3][3] =
    {
    { 1280,    -256,      	0},
    {	  -51,	1177,    -102},
    {    0,     -256,		1280}
    };
    code short t_B_G_ColorCorrectionMatrix[3][3] =
    {
    { 1213,    -105,      	-84},
    {	  87,	1149,     	-212},
    {    184,     -242,		1133}
    };
    code short t_B_G_EnhanceColorCorrectionMatrix[3][3] =
    {
    { 1331,    	-153,      	-153},
    {	  -51,	1331,     	-256},
    {    -51,     -256,		1331}
    };
    code short t_FR_B_ColorCorrectionMatrix[3][3] =
    {
    { 1229,    0,      	-205},
    {	  0,	1229,     	-205},
    {    51,     0,		1024}
    };
    code short t_FR_G_ColorCorrectionMatrix[3][3] =
    {
    { 1239,    -356,      	141},
    {	  -113,	1149,     	-13},
    {    128,     -230,		1126}
    };
    code short t_FR_B_G_ColorCorrectionMatrix[3][3] =
    {
    {		1326,		-230,		-72},
    {		-26,		1275,		-225},
    {		184,		-242,		1133}
    };
    code short t_FY_B_ColorCorrectionMatrix[3][3] =
    {
    {		1126,		102,		-205},
    {		102,		1126,		-205},
    {		-78,		-12,		1171}
    };
    code short t_FY_G_ColorCorrectionMatrix[3][3] =
    {
    {		1126,		-230,		128},
    {		0,		1024,		0},
    {		8,		-230,		1246}
    };
    code short t_FY_B_G_ColorCorrectionMatrix[3][3] =
    {
    {		1213,		-105,		-84},
    {		87,		1149,		-212},
    {		61,		-258,		1279}
    };
    code short t_MWEMode_EnhanceColorCorrectionMatrix1[3][3] =
    {
    { 962,    	31,      	-20},
    {	  -164,	1167,     	-31},
    {    -205,     -123,		1352}
    };
    code short t_MWEMode_EnhanceColorCorrectionMatrix[3][3] =
    {
    { 979,    	20,      	20},
    {	  -30,	1101,     	-20},
    {    -61,     -40,		1173}
    };
     */
    // MWE Table
#if  0
    BYTE code t_AutoDetect_MWEColorSettingTable[] =
    {
   0x05,0x37,0xFE,0x2D,0x00,0x9B,0x00,0x70,0x04,0x79,0xFF,0x17,0xFF,0xBA,0x00,0x0C,
   0x04,0x3A,0x00,0x00,0x00,0xAA,0x9B,0x43,0x6E,0xEB,0x87,0x7F,0xD1,0x63,0x73,0xBF,
   0xEE,0x8E,0x5C,0xD8,0xF5,0xB3,0x67,0xE0,0xAC,0x1E,0x52,0x89,0x81,0x9D,0x1C,0xBA,
    };
#endif
    BYTE code t_Skin_MWEColorSettingTable[] =
    {
   0x04,0x29,0xFF,0xCF,0x00,0x08,0xFF,0xE0,0x04,0x25,0xFF,0xFB,0xFF,0xE0,0x00,0x21,
   0x03,0xFE,0x00,0x00,0x00,0x03,0x35,0xAF,0xD7,0x5C,0x24,0x9A,0x35,0xF1,0x25,0xBF,
   0x44,0x47,0x67,0x67,0x3D,0xB4,0xB5,0xE9,0xCF,0xEC,0x76,0xD7,0x36,0xC9,0xE4,0x68,
    };
    // green4
    BYTE code t_Green_MWEColorSettingTable[] =
    {
   0x03,0xEC,0x00,0x0A,0x00,0x0A,0xFF,0xF1,0x04,0x1E,0xFF,0xF1,0x00,0x0A,0x00,0x0A,
   0x03,0xEC,0x00,0x00,0x00,0x84,0xB7,0xA0,0xF3,0x69,0x69,0xAB,0xC5,0x1B,0x70,0x12,
   0x79,0xD9,0x5F,0x77,0xB3,0x78,0xC7,0x35,0xF1,0x7A,0x01,0x9E,0xFA,0x6E,0x3A,0x74,
    };
    BYTE code t_Blue_MWEColorSettingTable[] =
    {
   0x04,0x07,0x00,0x07,0xFF,0xF2,0x00,0x0E,0x04,0x23,0xFF,0xCE,0xFF,0xE3,0x00,0x09,
   0x04,0x14,0x00,0x00,0x00,0x84,0xB7,0xA0,0xF3,0x69,0x69,0xAB,0xC5,0x1B,0x70,0x12,
   0x79,0xD9,0x5F,0x77,0xB3,0x78,0xC7,0x35,0xF1,0x7A,0x01,0x9E,0xFA,0x6E,0x3A,0x74,
    };
    BYTE code t_Full_MWEColorSettingTable[] =
    {
   0x04,0xD3,0xFE,0xC9,0x00,0x63,0x00,0x47,0x04,0x57,0xFF,0x61,0xFF,0xCE,0x00,0x0A,
   0x04,0x27,0x00,0x00,0x00,0xC5,0x01,0xCB,0xE8,0xA8,0x49,0xB3,0xE7,0xF6,0x38,0xE7,
   0xE0,0x96,0xE5,0x60,0xEF,0x4D,0x2A,0x98,0xA9,0x16,0x5C,0x79,0xF5,0xA5,0x4C,0x86,
    };
    BYTE code t_MWEColorSettingTable[] =
    {
   0x04,0x07,0x00,0x07,0xFF,0xF2,0xFF,0xF9,0x04,0x15,0xFF,0xF3,0xFF,0xF9,0x00,0x07,
   0x04,0x00,0x00,0x00,0x00,0x84,0xB7,0xA0,0xF3,0x69,0x69,0xAB,0xC5,0x1B,0x70,0x12,
   0x79,0xD9,0x5F,0x77,0xB3,0x78,0xC7,0x35,0xF1,0x7A,0x01,0x9E,0xFA,0x6E,0x3A,0x74,
    };
    //DLC Table......
    BYTE code t_MWEDLC_Linear_Table[] =
    {
   0x05,0x16,0x26,0x37,0x47,0x57,0x67,0x77,0x87,0x97,0xA7,0xB7,0xC7,0xD7,0xE7,0xF7,
   0x87,0x9B,0x0F,0xC2,0xBD,0x4E,0xC6,0xFD,0xF3,0x3E,0xE9,0x35,0x75,0xED,0x76,0x8D,
   0xC1,0x74,0xD4,0x3C,0x3A,0x18,0xE0,0x0A,0xD6,0xE3,0x33,0x0C,0x0A,0x84,0xB7,0xA0,
    };
    BYTE code t_Normal_ColorSettingTable[] =
    {
   0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x04,0x00,0x00,0x00,0x00,0xD4,0x55,0xED,0xDC,0xEA,0x20,0x81,0x11,0x1B,0x01,0xFE,
   0xDC,0xC2,0xAD,0xDE,0x23,0x63,0x0E,0x58,0x10,0x21,0xF1,0xFA,0x5D,0x7B,0xF1,0x87,
    };
    /*
    BYTE code t_G_ColorSettingTable[] =
    {
    0x04,0x7E,0xFF,0x1C,0x00,0x66,0xFF,0x98,0x04,0xD3,0xFF,0x95,0x00,0x88,0xFE,0xED,
    0x04,0x8B,0xC3,0x4B,0x29,0x96,0xFE,0x8B,0xEB,0xC9,0xDC,0x39,0x18,0x32,0xD8,0x04,
    0x27,0xEE,0xF4,0xE7,0x9F,0xCB,0x64,0xC3,0x6C,0x04,0x7A,0x3B,0x62,0x3E,0xD1,0xD5,
    };
    BYTE code t_B_ColorSettingTable[] =
    {
    0x04,0x72,0x00,0x02,0xFF,0x8B,0x00,0x2B,0x04,0x36,0xFF,0x9F,0xFF,0xD9,0x00,0x09,
    0x04,0x1D,0xF3,0x69,0x69,0xAB,0xC5,0x1B,0x70,0x12,0x79,0xD9,0x5F,0x77,0xB3,0x78,
    0xC7,0x35,0xF1,0x7A,0x01,0x9E,0xFA,0x6E,0x3A,0x74,0x86,0x87,0xBF,0xA6,0x2A,0x80,
    };
    BYTE code t_B_G_ColorSettingTable[] =
    {
    0x04,0xEF,0xFF,0x0D,0x00,0x04,0xFF,0xC8,0x04,0xFB,0xFF,0x3D,0x00,0x65,0xFE,0xF5,
    0x04,0xA5,0x75,0x99,0x95,0xB9,0x78,0xF4,0x3E,0xC7,0xC7,0xF5,0x5F,0x4F,0x4A,0xC1,
    0xF5,0xAF,0xC5,0x01,0xAB,0x71,0x0B,0xFC,0xA2,0x83,0x61,0xF7,0x80,0xB3,0x30,0xCA,
    };
    BYTE code t_FR_ColorSettingTable[] =
    {
    0x04,0x31,0xFF,0xD5,0xFF,0xFA,0xFF,0x74,0x04,0xB1,0xFF,0xDC,0xFF,0x41,0x00,0xC6,
    0x03,0xF8,0x73,0xBF,0xEE,0x8E,0x5C,0xD8,0xF5,0xB3,0x67,0xE0,0xAC,0x1E,0x52,0x89,
    0x81,0x9D,0x1C,0xBA,0xCF,0x38,0xF0,0x6E,0x70,0x36,0x58,0xD2,0x85,0x4D,0x9F,0x9A,
    };
    BYTE code t_FR_B_ColorSettingTable[] =
    {
    0x04,0x9D,0xFF,0xCF,0xFF,0x94,0xFF,0xA3,0x04,0xD8,0xFF,0x85,0xFF,0x25,0x00,0xC4,
    0x04,0x17,0xE9,0xCF,0xEC,0x76,0xD7,0x36,0xC9,0xE4,0x68,0x9A,0x5A,0x8C,0x26,0xAA,
    0x30,0x07,0xD8,0x79,0xE4,0xF7,0xB2,0x98,0xED,0x44,0xBB,0x75,0xA0,0x98,0x80,0xF4,
    };
    BYTE code t_FR_G_ColorSettingTable[] =
    {
    0x04,0xA7,0xFE,0xEB,0x00,0x6E,0xFF,0x12,0x05,0x73,0xFF,0x7C,0xFF,0xD5,0xFF,0xA4,
    0x04,0x87,0x1A,0xC5,0xD9,0xEF,0xFF,0x75,0x1D,0x37,0x60,0xD2,0x2E,0x5E,0xF8,0xEF,
    0x8B,0x8A,0x33,0x46,0x96,0xCF,0x19,0x92,0x1D,0x29,0x80,0xEF,0x26,0xBB,0x73,0x35,
    };
    BYTE code t_FR_B_G_ColorSettingTable[] =
    {
    0x05,0x19,0xFE,0xDA,0x00,0x0D,0xFF,0x3D,0x05,0xA1,0xFF,0x22,0xFF,0xB5,0xFF,0xA9,
    0x04,0xA2,0x16,0x11,0x8F,0x16,0x40,0xD9,0x68,0x18,0x09,0x1D,0x25,0xD3,0x0C,0xC1,
    0x69,0xEC,0x07,0x16,0x8A,0xD5,0x8D,0x72,0xE1,0x91,0xEC,0x11,0x51,0xAA,0x61,0x2C,
    };
     */
     
	void IndependentColorControl(void)
	{
		BYTE code tColorRangeTbl[]={0x20,0x28,0x20,0x20,0x28,0x20,0x19,0x29,0x19};
		BYTE i,j;
	
		BYTE code tColorControlTbl[][18]={ //0x3A~0x4B
	   //  3A	3B	 3C   3D   3E	3F	 40   41   42	43	 44   45   46	47	 48   49   4A	4B
		{0x08,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, //FullEnhanceFlag
		{0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, //SkinFlag
		{0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, //BlueFlag
		{0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, //GreenFlag
		{0x08,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}  //ColorAutoDetectFlag
		};
		mStar_WriteByte(REGBK, 0x06);
	
		for(i=0;i<9;i++)	//0x31~0x39
			mStar_WriteByte(0x31+i,tColorRangeTbl[i]);
	
		if (FullEnhanceFlag )
			j=0;
		else if (SkinFlag )
			j=1;
		else if(BlueFlag )
			j=2;
		else if(GreenFlag )
			j=3;
		else if ( ColorAutoDetectFlag)
			j=4;
	
		for(i=0;i<18;i++)	 //0x3A~0x4B
					mStar_WriteByte(0x3A+i,tColorControlTbl[j][i]);
		if(DemoFlag)
				mStar_WriteByte(0x30,0x07);
		else
			{
				SetBFSize(UserPrefBFSize);//SetBFSize();
				mStar_WriteByte(0x30,0x70);
			}
		mStar_WriteByte(REGBK, 0x00);
	}
#if !ENABLE_HDMI
    void SetColorBoostWindow(BYTE mode)
    {
        if (DemoFlag)
            mode = COLORBOOST_HALF;
        if (mode != COLORBOOST_OFF)
        {
            // 2006/11/10 10:43PM by KK 		msAdjustSubBrightness( UserPrefSubBrightness );
            if (DemoFlag)
                msAdjustVideoContrast(DefContrastBase + DefContrast);
            else
                msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
            if (mode == COLORBOOST_HALF)
            {
                UserPrefBFHSize = PanelWidth / 2;
                UserPrefBFVSize = PanelHeight;
                UserPrefBFHStart = 0;
                UserPrefBFVStart = 0;
            }
            else if (mode == COLORBOOST_FULL)
            {
                UserPrefBFHSize = PanelWidth;
                UserPrefBFVSize = PanelHeight;
                UserPrefBFHStart = 0;
                UserPrefBFVStart = 0;
            }
			if(DemoFlag)	
			 { 
			   msAccSetup(UserPrefBFHStart, UserPrefBFHSize, UserPrefBFVStart, UserPrefBFVSize);
			   msAccOnOff(1);
			 }

        }
        else
        // COLORBOOST_OFF
        {
            msAccOnOff(0);
        }
    }
#endif
    void SetColorEnhance(bit enable)
    {
        BYTE *pColorTable;
        enable = 1;
        if (UserPrefDcrMode)
        {
            pColorTable = t_Normal_ColorSettingTable;
            LoadACETable(pColorTable, DefHue, DefSaturation, 0x80); //  UserPrefSubContrast  );
            msAccOnOff(1);
            msDlcOnOff(1);
        }
        else if (FullEnhanceFlag || SkinFlag || BlueFlag || GreenFlag || DemoFlag || ColorAutoDetectFlag)
     
        {
        
            if (FullEnhanceFlag || ColorAutoDetectFlag || GreenFlag|| BlueFlag|| SkinFlag)
		  IndependentColorControl();
           else
           	{
			mStar_WriteByte(0x00, 0x06);
			mStar_WriteByte(0x30, 0x00);
			mStar_WriteByte(0x00, 0x00);
			//LoadACETable(pColorTable, DefHue, DefSaturation, DefContrastBase + UserPrefContrast);
           	}
		   if (GreenFlag || SkinFlag)
            {
                mStar_WriteByte(BK0_00, 0x03);
                mStar_WriteByte(BK3_72, 0x6a);
                mStar_WriteByte(BK3_73, 0x6a);
                mStar_WriteByte(BK3_74, 0x6a);
                mStar_WriteByte(BK0_00, 0x00);
            }
            else if (ColorAutoDetectFlag)
            {
                mStar_WriteByte(BK0_00, 0x03);
                mStar_WriteByte(BK3_72, 0x60);
                mStar_WriteByte(BK3_73, 0x60);
                mStar_WriteByte(BK3_74, 0x60);
                mStar_WriteByte(BK0_00, 0x00);
            }
            else
            {
                mStar_WriteByte(BK0_00, 0x03);
                mStar_WriteByte(BK3_72, 0x70);
                mStar_WriteByte(BK3_73, 0x70);
                mStar_WriteByte(BK3_74, 0x70);
                mStar_WriteByte(BK0_00, 0x00);
            }
        }
		else
          {
			mStar_WriteByte(0x00, 0x06);
			mStar_WriteByte(0x30, 0x00);
			mStar_WriteByte(0x00, 0x00);
			//LoadACETable(pColorTable, DefHue, DefSaturation, DefContrastBase + UserPrefContrast);
         }
    }

#if ENABLE_HDMI
void SetColorBoostWin( BYTE mode )
{
  if( DemoFlag)
      mode =  COLORBOOST_HALF;

  if( mode != COLORBOOST_OFF)
  {
      msAdjustVideoContrast(DefContrastBase+UserPrefContrast );

      if(mode == COLORBOOST_HALF )
      {
          UserPrefBFHSize = PanelWidth / 2;
          UserPrefBFVSize = PanelHeight;
          UserPrefBFHStart = 0;
          UserPrefBFVStart = 0;
      }
      else if(mode == COLORBOOST_FULL)
      {
          UserPrefBFHSize = PanelWidth ;
          UserPrefBFVSize = PanelHeight;
          UserPrefBFHStart = 0;
          UserPrefBFVStart = 0;
      }


	  if(DemoFlag)
	  	{
	  	msAccSetup( UserPrefBFHStart, UserPrefBFHSize, UserPrefBFVStart, UserPrefBFVSize );
		if(gScInfo.InputColor == INPUT_RGB)
			msAccOnOff(1);
		 else 
			mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)|BIT3);
	  	}
	  else
	  	{
		if(gScInfo.InputColor == INPUT_RGB)
			msAccOnOff(0);
		 else 
			mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)&~BIT3);	  	
	  	}
		
  }
  else // COLORBOOST_OFF
  {
      msAccSetup( 0, 0, 0, 0 );
    //msAccOnOff( 0 );
  }

}

void SetColorMatrix( void )
{
    BYTE *pColorTable;

    if( UserPrefDcrMode)
    {
        pColorTable = t_Normal_ColorSettingTable;
        LoadACETable( pColorTable, UserPrefHue, UserPrefSaturation, 0x80); //  UserPrefSubContrast  );
        //msAccOnOff( 1 );
        //msDlcOnOff( 1 );
    }
    else  if ( FullEnhanceFlag || SkinFlag || BlueFlag || GreenFlag || DemoFlag ||ColorAutoDetectFlag)   //NormalSkin+GreenENhance+BlueEnhance
    {
        
            if (FullEnhanceFlag || ColorAutoDetectFlag || GreenFlag|| BlueFlag|| SkinFlag)
		  IndependentColorControl();
           else
           	{
			mStar_WriteByte(0x00, 0x06);
			mStar_WriteByte(0x30, 0x00);
			mStar_WriteByte(0x00, 0x00);
			//LoadACETable(pColorTable, DefHue, DefSaturation, DefContrastBase + UserPrefContrast);
           	}
		   if (GreenFlag || SkinFlag)
            {
                mStar_WriteByte(BK0_00, 0x03);
                mStar_WriteByte(BK3_72, 0x6a);
                mStar_WriteByte(BK3_73, 0x6a);
                mStar_WriteByte(BK3_74, 0x6a);
                mStar_WriteByte(BK0_00, 0x00);
            }
            else if (ColorAutoDetectFlag)
            {
                mStar_WriteByte(BK0_00, 0x03);
                mStar_WriteByte(BK3_72, 0x60);
                mStar_WriteByte(BK3_73, 0x60);
                mStar_WriteByte(BK3_74, 0x60);
                mStar_WriteByte(BK0_00, 0x00);
            }
            else
            {
                mStar_WriteByte(BK0_00, 0x03);
                mStar_WriteByte(BK3_72, 0x70);
                mStar_WriteByte(BK3_73, 0x70);
                mStar_WriteByte(BK3_74, 0x70);
                mStar_WriteByte(BK0_00, 0x00);
            }
        }
	else
 		{
			mStar_WriteByte(0x00, 0x06);
			mStar_WriteByte(0x30, 0x00);
			mStar_WriteByte(0x00, 0x00);
			//LoadACETable(pColorTable, DefHue, DefSaturation, DefContrastBase + UserPrefContrast);
         }
}

void SetColorSpace( void )
{
    #if ENABLE_HDMI
    BYTE XDATA ucDomain;
    #endif

    #if ENABLE_HDMI
    ucDomain = 0x00;
    if(gScInfo.InputColor!=INPUT_RGB)
        ucDomain = 0xFF;
    #endif

    if ( DemoFlag ||BlueFlag||SkinFlag||GreenFlag||ColorAutoDetectFlag||FullEnhanceFlag)
    {
        #if ENABLE_HDMI
        if (ucDomain)
        {
        
  
            if(DemoFlag)
            	{
          	SetMainWinVideoDomain(ucDomain);		//SetMainWinColorControl(ucDomain);            	
            	mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)|BIT3);
            	}
		else
		{
			mStar_WriteByte(0x02, mStar_ReadByte(0x02)|BIT3);
			mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)&~BIT3);
		}
        }
        else
        #endif
	if(DemoFlag)
        msAccOnOff( 1 );
    }
    else
    {
        #if ENABLE_HDMI
        if (ucDomain)
            mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)&~BIT3);
        else
        #endif
            msAccOnOff(0);
        #if ENABLE_HDMI
        SetMainWinVideoDomain(0);	//SetMainWinColorControl(0);
        if (ucDomain)
            mStar_WriteByte(0x02, mStar_ReadByte(0x02)|BIT3);
        #endif
    }
}
#endif

//================================================================================
Bool AdjustColorBoostMode(MenuItemActionType action)
{
    action = 0;
    if (!DemoFlag)
    {
        Clr_DemoFlag();
    }
    Clr_SkinFlag();
    Clr_GreenFlag();
    Clr_BlueFlag();
    Clr_ColorAutoDetectFlag();
    Clr_PictureBoostFlag();
    //if (UserPrefDcrMode)
        //SetECO();
    UserPrefDcrMode = 0;
	#if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
	#endif
    UserPrefECOMode = ECO_Standard;
    SetECO();
    #if ENABLE_HDMI
    msDlcOnOff(_DISABLE);
    #endif
    
    if (FullEnhanceFlag)
    // clr
    {
        Clr_FullEnhanceFlag();
	 Clr_DrawFullEnhanceFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_OFF);   
        #else
        SetColorBoostWindow(COLORBOOST_OFF);
        #endif
    }
    else
    // set
    {
        Set_FullEnhanceFlag();
	 Set_DrawFullEnhanceFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_FULL);  
        #else
        SetColorBoostWindow(COLORBOOST_FULL);
        #endif
    }
    #if ENABLE_HDMI
    SetColorMatrix();
    SetColorSpace();
    #else
    SetColorEnhance(1);
    #endif
    
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
    #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    //Benz 2007.4.24   9:50:19		Osd_Draw4Num(7,1,UserPrefColorFlags);
    return TRUE;
}

//================================================================================
Bool AdjustSkinProtectMode(MenuItemActionType action)
{
    action = action;
    if (!DemoFlag)
    {
        Clr_DemoFlag();
    }
    Clr_GreenFlag();
    Clr_FullEnhanceFlag();
    Clr_BlueFlag();
    Clr_ColorAutoDetectFlag();
    Clr_PictureBoostFlag();
    
    //if (UserPrefDcrMode)
        //SetECO();
    UserPrefDcrMode = 0;
	#if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
	#endif
    UserPrefECOMode = ECO_Standard;
    SetECO();

    #if ENABLE_HDMI
    msDlcOnOff(_DISABLE);
    #endif

    if (SkinFlag)
    {
        Clr_SkinFlag();
	 Clr_DrawSkinFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_OFF);        
        #else
        SetColorBoostWindow(COLORBOOST_OFF);
        #endif
    }
    else
    {
        Set_SkinFlag();
	 Set_DrawSkinFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_FULL);        
        #else
        SetColorBoostWindow(COLORBOOST_FULL);
        #endif
    }
    #if ENABLE_HDMI
    SetColorMatrix();
    SetColorSpace();    
    #else
    SetColorEnhance(1);
    #endif
    
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
    #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    //Benz 2007.4.24   9:50:24	Osd_Draw4Num(6,1,UserPrefColorFlags);
    return TRUE;
}
Bool AdjustGreenEnhanceMode(MenuItemActionType action)
{
    action = action;
    if (!DemoFlag)
    {
        Clr_DemoFlag();
    }
    Clr_SkinFlag();
    Clr_FullEnhanceFlag();
    Clr_BlueFlag();
    Clr_ColorAutoDetectFlag();
    Clr_PictureBoostFlag();
    //if (UserPrefDcrMode)
        //SetECO();
    UserPrefDcrMode = 0;
  	#if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
	#endif
    UserPrefECOMode = ECO_Standard;
    SetECO();
    
    #if ENABLE_HDMI
    msDlcOnOff(_DISABLE);
    #endif
    
    if (GreenFlag)
    {
        Clr_GreenFlag();
	 Clr_DrawGreenFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_OFF);
        #else
        SetColorBoostWindow(COLORBOOST_OFF);
        #endif
    }
    else
    {
        Set_GreenFlag();
	 Set_DrawGreenFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_FULL);
        #else
        SetColorBoostWindow(COLORBOOST_FULL);
        #endif
    }
    #if ENABLE_HDMI
    SetColorMatrix();
    SetColorSpace();
    #else
    SetColorEnhance(1);
    #endif
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
    #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    //Benz 2007.4.24   9:50:28		Osd_Draw4Num(5,1,UserPrefColorFlags);
    return TRUE;
}
Bool AdjustBlueEnhanceMode(MenuItemActionType action)
{
    action = action;
    if (!DemoFlag)
    {
        Clr_DemoFlag();
    }
    // color boost menu
    Clr_FullEnhanceFlag();
    Clr_GreenFlag();
    Clr_SkinFlag();
    Clr_ColorAutoDetectFlag();
    // picture boost menu
    Clr_PictureBoostFlag();
    // DCR function
    //if (UserPrefDcrMode)
        //SetECO();
    UserPrefDcrMode = 0;
    	#if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
	#endif
    UserPrefECOMode = ECO_Standard;
    SetECO();

    #if ENABLE_HDMI
    msDlcOnOff(_DISABLE);
    #endif
    
    if (BlueFlag)
    {
        Clr_BlueFlag();
	 Clr_DrawBlueFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_OFF);
        #else
        SetColorBoostWindow(COLORBOOST_OFF);
        #endif
    }
    else
    {
        Set_BlueFlag();
	 Set_DrawBlueFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_FULL);
        #else
        SetColorBoostWindow(COLORBOOST_FULL);
        #endif
    }
    #if ENABLE_HDMI
    SetColorMatrix();
    SetColorSpace();
    #else
    SetColorEnhance(1);
    #endif
    #if !USEFLASH
	 NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
    #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    //Benz 2007.4.24   9:50:33		Osd_Draw4Num(4,1,UserPrefColorFlags);
    return TRUE;
}

Bool AdjustAutoDetectMode(MenuItemActionType action)
{
    action = action;
    if (!DemoFlag)
    // when demo on, don't clear the demo flag
    {
        Clr_DemoFlag();
    }
    // color boost menu
    Clr_FullEnhanceFlag();
    Clr_GreenFlag();
    Clr_BlueFlag();
    Clr_SkinFlag();
    // picture boost menu
    Clr_PictureBoostFlag();
    // DCR function
    //if (UserPrefDcrMode)
        //SetECO();
    UserPrefDcrMode = 0;
    	#if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
	#endif
    UserPrefECOMode = ECO_Standard;
    SetECO();

    #if ENABLE_HDMI
    msDlcOnOff(_DISABLE);
    #endif
    
    if (ColorAutoDetectFlag)
    {
        Clr_ColorAutoDetectFlag();
	 Clr_DrawColorAutoDetectFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_OFF);
        #else
        msDlcOnOff(_DISABLE);
        SetColorBoostWindow(COLORBOOST_OFF);
        #endif
    }
    else
    {
        Set_ColorAutoDetectFlag();
	 Set_DrawColorAutoDetectFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_FULL);
        #else
        LoadDLCTable(t_MWEDLC_Linear_Table);
        msDlcOnOff(_DISABLE);
        SetColorBoostWindow(COLORBOOST_FULL);
        #endif
    }
    #if ENABLE_HDMI
    SetColorMatrix();
    SetColorSpace();
    #else
    SetColorEnhance(1);
    #endif
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
    #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    //Benz 2007.4.24   9:50:40		Osd_Draw4Num(3,1,UserPrefColorFlags);
    return TRUE;
}
Bool AdjustDemoMode(MenuItemActionType action)
{
    action = action;
    Clr_PictureBoostFlag();
    //if (UserPrefDcrMode)
        //SetECO();
    UserPrefDcrMode = 0;
	#if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
	#endif
    UserPrefECOMode = ECO_Standard;
    UserPrefBFSize=1;
    SetECO();

    #if ENABLE_HDMI
    msDlcOnOff(_DISABLE);
    #endif
    
    if (DemoFlag)
    {
        Clr_DemoFlag();
        if (BlueFlag || SkinFlag || GreenFlag || ColorAutoDetectFlag || FullEnhanceFlag)
        #if ENABLE_HDMI
            SetColorBoostWin(COLORBOOST_OFF);
        #else
            SetColorBoostWindow(COLORBOOST_OFF);
        #endif
        else
        #if ENABLE_HDMI
            SetColorBoostWin(COLORBOOST_OFF);
        #else
            SetColorBoostWindow(COLORBOOST_OFF);
        #endif
    }
    else
    {
        Clr_PictureBoostFlag();
        Set_DemoFlag();
        #if ENABLE_HDMI
        SetColorBoostWin(COLORBOOST_HALF);
        #else
        SetColorBoostWindow(COLORBOOST_HALF);
        #endif	
    }
    #if ENABLE_HDMI
    SetColorMatrix();
    SetColorSpace();
    #else
    SetColorEnhance(1);
    #endif
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
    #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    //Benz 2007.4.24   9:50:44		Osd_Draw4Num(2,1,UserPrefColorFlags);
    return TRUE;
}

#if 1  //2006-08-29 Andy
//Bool SetBFSize(void)
Bool SetBFSize(BYTE ucSize)
{
    #if ENABLE_HDMI
    BYTE XDATA ucDomain;
    #endif

    #if ENABLE_HDMI
    ucDomain = 0x00;
    if(gScInfo.InputColor!=INPUT_RGB)
        ucDomain = 0xFF;
    #endif

    #if 1
    if (!PictureBoostFlag)
    {
        #if !ENABLE_HDMI
        msAccOnOff(0);
        msDlcOnOff(0);
        #endif
        UserPrefBFHSize = 0;
        UserPrefBFVSize = 0;
        UserPrefBFHStart = 0;
        UserPrefBFVStart = 0;
        UserPrefBFSize= ucSize = 1; //Benz 2007.4.14   14:34:23
    }
    else
    {
        #if 0//!ENABLE_HDMI
        msAccOnOff(1);
        #endif
        if (ucSize == 1)  // H:5 V:5
        {
            UserPrefBFHSize = (DISPLAY_WIDTH+4) / 8;	//120204 Modify
            UserPrefBFVSize = PanelHeight / 8; // / 5;
        }
        else if (ucSize == 2)          // H:5 V:3
        {
            UserPrefBFHSize = DISPLAY_WIDTH / 5;
            UserPrefBFVSize = PanelHeight / 5; // / 3;
        }
        else if (ucSize == 3)     //  H4 V:4
        {
            UserPrefBFHSize = DISPLAY_WIDTH / 4;
            UserPrefBFVSize = PanelHeight / 4;
        }
        else if (ucSize == 4)       //  H3 V:3
        {
            UserPrefBFHSize = DISPLAY_WIDTH / 3;
            UserPrefBFVSize = PanelHeight / 3;
        }
        else if (ucSize == 5)        //  H2 V:2
        {
            UserPrefBFHSize = DISPLAY_WIDTH / 2;
            UserPrefBFVSize = PanelHeight / 2;
        }
        else if (ucSize == 6)        //  H2 V:1
        {
            UserPrefBFHSize = DISPLAY_WIDTH / 2;
            UserPrefBFVSize = PanelHeight;
        }
        else if (ucSize == 7)       //  Full
        {
            UserPrefBFHSize = DISPLAY_WIDTH;
            UserPrefBFVSize = PanelHeight;
        }
        UserPrefBFHStart = 0;
        UserPrefBFVStart = 0;
        msAdjustVideoContrast(UserPrefSubContrast);
        msAccSetup(UserPrefBFHStart + HSTART_OFFSET, UserPrefBFHSize + HSTART_OFFSET, UserPrefBFVStart, UserPrefBFVSize);
        #if ENABLE_HDMI
        msAdjustSubBrightness( UserPrefSubBrightness );
        #else
        msAccOnOff(1);
        #endif
    }
    #endif
    
    #if ENABLE_HDMI
    if ( PictureBoostFlag)
    {
        #if ENABLE_HDMI
        if (ucDomain)
        {
            SetMainWinVideoDomain(ucDomain);	//SetMainWinColorControl(ucDomain);
            mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)|BIT3);
        }
        else
        #endif
            msAccOnOff( 1 );
    }
    else
    {
        #if ENABLE_HDMI
        if (ucDomain)
            mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)&~BIT3);
        else
        #endif
            msAccOnOff(0);
        #if ENABLE_HDMI
        SetMainWinVideoDomain(0);	//SetMainWinColorControl(0);
        if (ucDomain)
            mStar_WriteByte(0x02, mStar_ReadByte(0x02)|BIT3);
        #endif
    }
    #endif
    return TRUE;
}

#if ENABLE_HDMI
Bool AdjustBrightFrameSize(BYTE ucSize)
{
  if ( ucSize == 1 )        // H:5 V:5
  {
      UserPrefBFHSize = DISPLAY_WIDTH /8; // / 5;
      UserPrefBFVSize = PanelHeight /8; // / 5;
  }
  else if ( ucSize == 2 ) // H:5 V:3
  {
      UserPrefBFHSize = DISPLAY_WIDTH / 5;
      UserPrefBFVSize = PanelHeight /5; // / 3;
  }
  else if ( ucSize == 3 ) //  H4 V:4
  {
      UserPrefBFHSize = DISPLAY_WIDTH / 4;
      UserPrefBFVSize = PanelHeight / 4;
  }
  else if ( ucSize == 4 ) //  H3 V:3
  {
      UserPrefBFHSize = DISPLAY_WIDTH / 3;
      UserPrefBFVSize = PanelHeight / 3;
  }
  else if ( ucSize == 5 ) //  H2 V:2
  {
      UserPrefBFHSize = DISPLAY_WIDTH / 2;
      UserPrefBFVSize = PanelHeight / 2;
  }
  else if ( ucSize == 6 ) //  H2 V:1
  {
      UserPrefBFHSize = DISPLAY_WIDTH / 2;
      UserPrefBFVSize = PanelHeight;
  }
  else if ( ucSize == 7 ) //  Full
  {
      UserPrefBFHSize = DISPLAY_WIDTH;
      UserPrefBFVSize = PanelHeight;
  }

  UserPrefBFHStart = 0;
  UserPrefBFVStart = 0;
  //msAdjustVideoContrast( UserPrefSubContrast );
  msAccSetup( UserPrefBFHStart + HSTART_OFFSET, UserPrefBFHSize + HSTART_OFFSET, UserPrefBFVStart, UserPrefBFVSize );
  //msAccOnOff( 1 );

    return TRUE;
}
#endif
//---------------------------------------------------------------
Bool AdjustBFSize(MenuItemActionType action)
{
    WORD tempValue;
    tempValue = DecIncValue(action, UserPrefBFSize, 1, 7, 1);
    if (tempValue == UserPrefBFSize)
    {
        return FALSE;
    }
    UserPrefBFSize = tempValue;
	//printData("UserPrefBFSize=%d",UserPrefBFSize);
    #if ENABLE_HDMI
    AdjustBrightFrameSize(UserPrefBFSize);	//
    #else
    SetBFSize(UserPrefBFSize);//SetBFSize();
    #endif
    return TRUE;
}
WORD GetBFSizeValue(void)
{
    return GetScale100Value(UserPrefBFSize, 0, 7);
}
        //---------------------------------------------------------------
        Bool AdjustBFHstart(MenuItemActionType action)
        {
            WORD tempValue;
            if (UserPrefBFSize == 1)
            {
                tempValue = DecIncValue(action, UserPrefBFHStart, 0, 7, 1);
            }
            else if (UserPrefBFSize == 2)
            {
                tempValue = DecIncValue(action, UserPrefBFHStart, 0, 4, 1);
            }
            else if (UserPrefBFSize == 3)
            {
                tempValue = DecIncValue(action, UserPrefBFHStart, 0, 3, 1);
            }
            else if (UserPrefBFSize == 4)
            {
                tempValue = DecIncValue(action, UserPrefBFHStart, 0, 2, 1);
            }
            else if (UserPrefBFSize == 5 || UserPrefBFSize == 6)
            {
                tempValue = DecIncValue(action, UserPrefBFHStart, 0, 1, 1);
            }
            else
            {
                tempValue = 0; //20120220 hong
            }
            if (tempValue == UserPrefBFHStart)  //20120220 hong
            {
                return FALSE;
            }
			//printData("tempValue11=%d",tempValue);

            UserPrefBFHStart = tempValue;
            tempValue = UserPrefBFHStart * UserPrefBFHSize + HSTART_OFFSET;
					//printData("UserPrefBFHStart=%d",UserPrefBFHStart);
					//printData("UserPrefBFHSize=%d",UserPrefBFHSize);
 			//printData("tempValue22=%d",tempValue);
           msAccSetup(tempValue, (UserPrefBFHSize + tempValue), UserPrefBFVStart *UserPrefBFVSize, (UserPrefBFVStart *UserPrefBFVSize + UserPrefBFVSize));
            return TRUE;
        }
        WORD GetBFHstartValue(void)
        {
            if (UserPrefBFSize == 2)
            {
                return GetScale100Value(UserPrefBFHStart, 0, 4);
            }
            else if (UserPrefBFSize == 3)
            {
                return GetScale100Value(UserPrefBFHStart, 0, 3);
            }
            else if (UserPrefBFSize == 4)
            {
                return GetScale100Value(UserPrefBFHStart, 0, 2);
            }
            else if (UserPrefBFSize == 5 || UserPrefBFSize == 6)
            {
                return GetScale100Value(UserPrefBFHStart, 0, 1);
            }
            else if (UserPrefBFSize == 1)
            {
                return GetScale100Value(UserPrefBFHStart, 0, 7);
            }
            else
            {
                return 0;
            }
        }
        //---------------------------------------------------------------
        Bool AdjustBFVstart(MenuItemActionType action)
        {
            WORD tempValue;
            if (UserPrefBFSize == 6 || UserPrefBFSize == 7)
            {
                tempValue = 0;
            }
            else if (UserPrefBFSize == 1)
            {
                tempValue = DecIncValue(action, UserPrefBFVStart, 0, 7, 1);
            }
            else if (UserPrefBFSize == 2)
            {
                tempValue = DecIncValue(action, UserPrefBFVStart, 0, 4, 1);
            }
            else if (UserPrefBFSize == 4)
            {
                tempValue = DecIncValue(action, UserPrefBFVStart, 0, 2, 1);
            }
            else if (UserPrefBFSize == 3)
            {
                tempValue = DecIncValue(action, UserPrefBFVStart, 0, 3, 1);
            }
            else if (UserPrefBFSize == 5)
            {
                tempValue = DecIncValue(action, UserPrefBFVStart, 0, 1, 1);
            }
            /*
            if ( tempValue == UserPrefVStart )
            {
            return FALSE;
            }
             */
            UserPrefBFVStart = tempValue;
            tempValue = UserPrefBFVStart * UserPrefBFVSize;
            msAccSetup(UserPrefBFHStart *UserPrefBFHSize + HSTART_OFFSET, UserPrefBFHStart *UserPrefBFHSize + UserPrefBFHSize + HSTART_OFFSET, tempValue, (tempValue + UserPrefBFVSize));
            return TRUE;
        }
        WORD GetBFVstartValue(void)
        {
            if (UserPrefBFSize == 1)
            {
                return GetScale100Value(UserPrefBFVStart, 0, 7);
            }
            else if (UserPrefBFSize == 2)
            {
                return GetScale100Value(UserPrefBFVStart, 0, 4);
            }
            else if (UserPrefBFSize == 4)
            {
                return GetScale100Value(UserPrefBFVStart, 0, 2);
            }
            else if (UserPrefBFSize == 3)
            {
                return GetScale100Value(UserPrefBFVStart, 0, 3);
            }
            else if (UserPrefBFSize == 5)
            {
                return GetScale100Value(UserPrefBFVStart, 0, 1);
            }
            else
            {
                return 0;
            }
        }
//================================================================================
Bool AdjustBF_Switch(MenuItemActionType action)
{
    action = action;
    Clr_ColorAutoDetectFlag();
    Clr_FullEnhanceFlag();
    Clr_GreenFlag();
    Clr_BlueFlag();
    Clr_SkinFlag();
    Clr_DemoFlag();
    //if (UserPrefDcrMode)
        //SetECO();
    UserPrefDcrMode = 0;
   	#if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
	#endif
    UserPrefECOMode = ECO_Standard;
    SetECO();

    #if ENABLE_HDMI
    msDlcOnOff(_DISABLE);
    #endif
    
    if (PictureBoostFlag)
    {
        Clr_PictureBoostFlag();
        #if ENABLE_HDMI
        UserPrefBFSize = 1 ;	// recall default setting=> BFSize=1, position(0,0)
        #endif
        SetBFSize(UserPrefBFSize);//SetBFSize();
    }
    else
    {
        #if !ENABLE_HDMI
        msAdjustSubBrightness(UserPrefSubBrightness);
        LoadACETable(t_Normal_ColorSettingTable, UserPrefHue, UserPrefSaturation, UserPrefSubContrast );
        #endif
        Set_PictureBoostFlag();
        #if !ENABLE_HDMI
        UserPrefBFSize = 1; // recall default setting=> BFSize=1, position(0,0)
        #endif
        SetBFSize(UserPrefBFSize);//SetBFSize();
        #if !ENABLE_HDMI
        SetColorEnhance(1);
        #endif
    }
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
     #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
     #endif
    //Benz 2007.4.24   9:50:48	Osd_Draw4Num(1,1,UserPrefColorFlags);
    return TRUE;
}
#endif
    //---------------------------------------------------------------
#if 0
    extern XDATA BYTE g_ucVideoSaturation;
    extern XDATA BYTE g_ucVideoHue;
    //================================================================================
    Bool AdjustHue(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefHue, MIN_VIDEO_HUE, MAX_VIDEO_HUE, 1);
        if (tempValue == UserPrefHue)
        {
            return FALSE;
        }
        UserPrefHue = tempValue;
        msAdjustVideoHue(UserPrefHue);
        return TRUE;
    }
    WORD GetHue100Value(void)
    {
        return GetScale100Value(UserPrefHue, MIN_VIDEO_HUE, MAX_VIDEO_HUE);
    }
    //================================================================================
    Bool AdjustSaturation(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefSaturation, MIN_VIDEO_SATURATION, MAX_VIDEO_SATURATION, 1);
        if (tempValue == UserPrefSaturation)
        {
            return FALSE;
        }
        UserPrefSaturation = tempValue;
        msAdjustVideoSaturation(UserPrefSaturation);
        return TRUE;
    }
    WORD GetSaturation100Value(void)
    {
        return GetScale100Value(UserPrefSaturation, MIN_VIDEO_SATURATION, MAX_VIDEO_SATURATION);
    }

#endif
    //================================================================================
    //================================================================================
    Bool AdjustSubContrast(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefSubContrast, MinSubContrast, MaxSubContrast, 2);
        if (tempValue == UserPrefSubContrast)
        {
            return FALSE;
        }
        UserPrefSubContrast = tempValue;
        msAdjustVideoContrast(UserPrefSubContrast);
        //NVRam_WriteByte( nvrMonitorAddr( SubContrast ), UserPrefSubContrast);
        return TRUE;
    }
    WORD GetSubContrast100Value(void)
    {
        return GetScale100Value(UserPrefSubContrast, MinSubContrast, MaxSubContrast);
    }
    //================================================================================
    Bool AdjustSubBrightness(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefSubBrightness, MinSubBrightness, MaxSubBrightness, 2);
        if (tempValue == UserPrefSubBrightness)
        {
            return FALSE;
        }
        UserPrefSubBrightness = tempValue;
        msAdjustSubBrightness(UserPrefSubBrightness);
        return TRUE;
    }
    #if 0
        WORD GetSubBrightnessValue(void)
        {
            return UserPrefSubBrightness;
        }
    #endif
    WORD GetSubBrightness100Value(void)
    {
        return GetScale100Value(UserPrefSubBrightness, MinSubBrightness, MaxSubBrightness);
    }
#endif
//================================================================================
WORD DecIncValue(MenuItemActionType action, WORD value, WORD minValue, WORD maxValue, BYTE step)
{
    if (action == MIA_IncValue || action == MIA_InvDecValue)
    {
        if (value >= maxValue)
        {
            return value;
        }
        minValue = value + step;
        if (minValue > maxValue)
        {
            minValue = maxValue;
        }
        value = minValue;
    }
    else
    // Decrease value
    {
        if (value <= minValue)
        {
            return value;
        }
        maxValue = value - step;
        if (maxValue < minValue)
        {
            maxValue = minValue;
        }
        value = maxValue;
    }
    return value;
}
WORD GetScale100Value(WORD value, WORD minValue, WORD maxValue)
{
    maxValue = (maxValue <= minValue)?minValue:(maxValue - minValue);
    //maxValue = maxValue - minValue;
    value = value - minValue;
    if (value > 650)
    {
        value /= 2;
        maxValue /= 2;
    }
    value = ((WORD)value *100+maxValue / 2) / maxValue;
    value %=101;
   return value;
}
extern BYTE  MenuItemIndex;
extern BYTE  MenuPageIndex;
Bool ResetOsdTimer(void)
{
    if (!FactoryModeFlag)
    {
        OsdCounter = UserPrefOsdTime;
    }
#if AOC_MENU_FRAME_MODE
    if (MenuPageIndex == MainMenu )
#else
    if (MenuPageIndex == MainMenu && !FactoryModeFlag)
#endif
    {
        UserPrefLastMenuIndex = MenuItemIndex;
    }
    //OsdCounter=0;
    return TRUE;
}
Bool EnablePowerDownCounter(void)
{
    if(ProductModeFlag)
        Set_ForcePowerSavingFlag();
    else
    {
        PowerDownCounter = 3;
    }
    //Set_ForcePowerSavingFlag();
    return TRUE;
}

#if 0  //2006-08-10 Andy
    Bool PowerOffSystem(void)
    {
        printMsg("turn off");
        Power_TurnOffLed();
        Power_PowerOffSystem();
        Clr_PowerOnFlag();
        Clr_FactoryModeFlag();
        Clr_DisplayLogoFlag();
        Clr_DoModeSettingFlag();
        // printData("Power-off Monitor Flag %d",MonitorFlags);
        NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
        return TRUE;
    }
#endif
Bool PowerOffSystem(void)
{
    Clr_PowerOnFlag();
    Clr_FactoryModeFlag();
    Clr_DisplayLogoFlag();
    Clr_DoModeSettingFlag();
	Clr_BurninModeFlag();
	Clr_DoBurninModeFlag();
    mStar_WriteByte(FWC, 0); //FWC_B); // panel background color

    Power_TurnOffLed();
    // 090819 coding, dc off should set ddc wp to protect
   // hw_SetDDC_WP();

#if ESaverPowerDownFunc
    ESaverPowerDownCounter=0;
#endif

#if ScanKey
    #if Scan_Source == Cypress

    #ifdef LEDBrightCtrlByScalerPWM
        #if MainBoardType == MainBoard_715G3598_M0A || MainBoardType == MainBoard_715G3603_M0A || MainBoardType == MainBoard_715G3598_1PLL
        SKLEDStatus = SKLEDBright_Off;
        mStar_WriteByte(LEDPWM, LEDSetting_Off);
        #endif
    #else
    Clr_SK_LEDVcc();
    #endif

    #ifdef EnableLightBar
    //ScanKey_EnablePWM(0);
    ScanKey_PWMCtrl(0);        
    #endif

    #endif
#endif
    
#if ENABLE_HDMI
    mstar_HDMIAudioMute(0);
    hw_SetMute();
#endif

    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
    #else
	SaveMonitorSetting();
    #endif
    
    #if 0//ENABLE_HDCP
        msDisableHDCP();
    #endif
    
    Power_PowerOffSystem();
    {
        BYTE tempdata = mStar_ReadByte(0x00);
        #if Dual // 080819 HDMI spec.: DC off, TMDS bus/hot-plug should keep in high.
        mStar_WriteByte(REGBK, REGBANKADC);    //mStar_WriteByte(0x00, 0x01);
            #if defined(TSUMXXNX) || defined(TSUMXXT)		//110311 Modify
            mStar_WriteByte(TESTEN, mStar_ReadByte(TESTEN)|BIT7);
            mStar_WriteByte(TESTA5, mStar_ReadByte(TESTA5)|BIT0);
            mStar_WriteByte(BK1_D1, mStar_ReadByte(BK1_D1) |BIT5);
            mStar_WriteByte(TESTEN, mStar_ReadByte(TESTEN)&~BIT7);
            #else
            mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4) | (BIT0|BIT1)); // power down dvi clk r-termintor
            #endif
        #endif        
        mStar_WriteByte(REGBK, REGBANKSCALER);    //mStar_WriteByte(0x00, 0x00);
        mStar_WriteByte(SWRST, 0x80); // enable deep power down mode,Please remember to check lock OSD Function!
        mStar_WriteByte(0x00, tempdata);
    }

    #ifdef MS_PM
    msPM_SetUp(ePM_PowerOff);
    //msPM_WaitingEvent();
    #endif  
    
    return TRUE;
}
Bool PowerOnSystem(void)
{
    #ifdef MS_PM
    msPM_Reset();
    #endif

    Clr_PowerOffFlag();
    Set_PowerOnFlag();
    Clr_DoKeyCountFlag();
    PowerKeyCounter=0;
    SwitchPortCntr = 0;  //071226

		Power_TurnOnGreenLed();

    {
        BYTE tempdata = mStar_ReadByte(0x00);
        mStar_WriteByte(0x00, 0x00);
        mStar_WriteByte(SWRST, 0x00); // disable deep power down mode
        #if Dual // 080819 HDMI spec.: DC off, TMDS bus/hot-plug should keep in high.
        mStar_WriteByte(0x00, 0x01);
            #if MainBoardType == MainBoard_715G3603_M0A
            if(SrcInputType == Input_Digital) // connect A port termination
                mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4) & ~BIT0); // power on dvi clk r-termintor
            else // connect B port termination
                mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4) & ~BIT1); // power on dvi clk r-termintor
            #else
			#if defined(TSUMXXNX) || defined(TSUMXXT)		//110311 Modify
            mStar_WriteByte(TESTEN, mStar_ReadByte(TESTEN)|BIT7);
            mStar_WriteByte(TESTA5, mStar_ReadByte(TESTA5)&~BIT0);
            mStar_WriteByte(BK1_D1, mStar_ReadByte(BK1_D1)&~BIT5);
            mStar_WriteByte(TESTEN, mStar_ReadByte(TESTEN)&~BIT7);
            #else
            mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4) & ~(BIT0|BIT1)); // power on dvi clk r-termintor
            #endif
            #endif
        #endif        
        mStar_WriteByte(0x00, tempdata);
    }
    Power_PowerOnSystem();

	
#ifdef PowerSavingAmberLEDFlashing
    #if PSAmberLEDFlashingType == Type_Breathing
        #if MainBoardType == MainBoard_715G3329_A2  || MainBoardType == MainBoard_715G3244_TSUMU18NR|| MainBoardType ==MainBoard_712G3244_2 || MainBoardType == MainBoard_715G3959_T || MainBoardType == MainBoard_715G3834||MainBoardType ==MainBoard_3834_18T
			InitialP06ToPWM1(FALSE);
        #endif
    #endif
#endif

    #ifdef MS_PM
    msPM_PowerUpMacro();
    #if 1//PM_CLOCK!=XTALDOUBLE
    mStar_WriteByte ( REGBK, REGBANKADC );    
    msWriteByteMask(BK1_68, BIT6       ,  BIT6);//B.1.8 Scar, 20090424   /*JT modify it from BK1_67[7] change to BK1_68[6]*/
    msWriteByteMask(BK1_66, BIT0       ,  BIT0);//B.14.4 Scar,20090424
    ForceDelay1ms ( 1 );        
    msWriteByteMask(BK1_67, BIT4        ,  BIT5|BIT4);
    ForceDelay1ms ( 1 );
    msWriteByteMask(BK1_66, BIT3|BIT2, BIT3|BIT2|BIT1);
    ForceDelay1ms ( 1 );
    msWriteByteMask(BK1_66, 0             ,  BIT0);
    ForceDelay1ms ( 1 );
    mStar_WriteByte(REGBK, REGBANKSCALER);
    #endif
    #endif

#if ScanKey
    #if Scan_Source == Cypress
    
    #ifdef LEDBrightCtrlByScalerPWM
        #if MainBoardType == MainBoard_715G3598_M0A || MainBoardType == MainBoard_715G3603_M0A|| MainBoardType == MainBoard_715G3598_1PLL
        SKLEDStatus = SKLEDBright_Light_Immediately;
        ScanKey_LEDBrightAction();
        #endif
    #else
        Set_SK_LEDVcc();
    #endif

    #ifdef EnableLightBar
        //ScanKey_CtrlPWM(UserPrefLightBar);
        //ScanKey_EnablePWM(1);
        ScanKey_PWMCtrl(UserPrefLightBar);
    #endif
    
    #endif

    #ifdef EnableSlider
    PreKeyStatus = 0;
    PreSliderStatus = SliderFingerOff;
    SliderIncDecStep = 0;
    #endif
    
#endif

    //Set_ShowInputInfoFlag();
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
    #else
	SaveMonitorSetting();
    #endif   
    // mStar_WriteByte( REGBK, REGBANKSCALER );
    // mStar_WriteByte( FWC, 0 ); // disable background color function.
    #if DisplayPowerOnLogo
	if(!(ProductModeFlag||FactoryModeFlag))
        {
            Set_DisplayLogoFlag();
            mStar_WriteOSDByte(IOSDC2, 0x02); //Disable color bit setting
            // 091005 coding
            mStar_AdjustBrightness(100);
        }
    #endif
	#ifdef OffPower           //090331 xiandi.yu Auto Off Power
	AutoPowerOffSTime=0;
    AutoPowerOffMTime=0;
    AutoPowerOffHTime=0;
	ShowOffPowerWarningFlag = 0;
	#endif
	
    #if ESaverPowerDownFunc
	ESaverPowerDownCounter=0;
    #endif

  if(!ProductModeFlag)
  	{
	#if DisplayEnergyStar
		Set_ShowEpaAfterLogoFlag();
	#endif
  	}
    mStar_SetupFreeRunMode();
    mStar_SetupInputPort();
    return TRUE;
}
Bool EnterRootMenu(void)
{
    OsdCounter = 0;
    return TRUE;
}
//================================================================================
//================================================================================
//================================================================================
Bool AdjustContrast(MenuItemActionType action)
{
    WORD tempValue;
    tempValue = DecIncValue(action, UserPrefContrast, MinContrastValue, MaxContrastValue, 1);
    if (tempValue == UserPrefContrast)
    {
        return FALSE;
    }
    UserPrefContrast = tempValue;
    mStar_AdjustContrast(UserPrefContrast);
/*	Clr_GreenFlag();	//080103 close colorboost function when adjusting Contrast
 	Clr_SkinFlag();
	Clr_BlueFlag();
	Clr_ColorAutoDetectFlag();
 	Clr_FullEnhanceFlag();
	Clr_DemoFlag(); //eshin 070610 HotEco DemoFlag on but picture off
	SetBFSize(); // jeff add in 1112*/
    if (UserPrefColorTemp == CTEMP_USER)
    {
        UserPrefContrastUser = UserPrefContrast;
    }
    else if (UserPrefColorTemp == CTEMP_Cool1)
    {
        UserPrefContrastCool1 = UserPrefContrast;
        if (FactoryModeFlag)
            FUserPrefContrastCool1 = UserPrefContrast;
    }
    else if (UserPrefColorTemp == CTEMP_Warm1)
    {
        UserPrefContrastWarm1 = UserPrefContrast;
        if (FactoryModeFlag)
            FUserPrefContrastWarm1 = UserPrefContrast;
    }
    else if (UserPrefColorTemp == CTEMP_Normal)
    {
        UserPrefContrastNormal = UserPrefContrast;
        if (FactoryModeFlag)
            FUserPrefContrastNormal = UserPrefContrast;
    }
    else if (UserPrefColorTemp == CTEMP_SRGB)
    {
        UserPrefContrastSRGB = UserPrefContrast;
    }
    //SaveMonitorSetting(); // need check again
    return TRUE;
}

WORD GetContrastValue(void)
{
    //return GetScale100Value(UserPrefContrast, MinContrastValue, MaxContrastValue);
    WORD temp;
    temp = GetScale100Value(UserPrefContrast, MinContrastValue, MaxContrastValue);
    return temp;
}
//====================================================
Bool AdjustBrightness(MenuItemActionType action)
{
    WORD tempValue;
    tempValue = DecIncValue(action, UserPrefBrightness, 0, 100, 1);
    if (tempValue == UserPrefBrightness)
    {
        return FALSE;
    }
    UserPrefBrightness = tempValue;
    mStar_AdjustBrightness(UserPrefBrightness);
/*	Clr_GreenFlag();	//080103 close colorboost function when adjusting Brightness
    Clr_SkinFlag();
    Clr_BlueFlag();
    Clr_ColorAutoDetectFlag();
    Clr_FullEnhanceFlag();
    Clr_DemoFlag(); //eshin 070610 HotEco DemoFlag on but picture off
    SetBFSize(); // jeff add in 1112*/
    #if 0
        if (UserPrefBrightness > 50)
            mStar_AdjustBrightness(UserPrefBrightness);
        else
        {
            mStar_AdjustBrightness(0);
            mStar_AdjustBlackLevel(UserPrefBrightness);
        }
    #endif
    if (UserPrefColorTemp == CTEMP_USER)
        UserPrefBrightnessUser = UserPrefBrightness;
    else if (UserPrefColorTemp == CTEMP_Cool1)
    {
        UserPrefBrightnessCool1 = UserPrefBrightness;
        if (FactoryModeFlag)
            FUserPrefBrightnessCool1 = UserPrefBrightness;
    }
    else if (UserPrefColorTemp == CTEMP_Warm1)
    {
        UserPrefBrightnessWarm1 = UserPrefBrightness;
        if (FactoryModeFlag)
            FUserPrefBrightnessWarm1 = UserPrefBrightness;
    }
    else if (UserPrefColorTemp == CTEMP_Normal)
    {
        UserPrefBrightnessNormal = UserPrefBrightness;
        if (FactoryModeFlag)
            FUserPrefBrightnessNormal = UserPrefBrightness;
    }
    else if (UserPrefColorTemp == CTEMP_SRGB)
    {
        UserPrefBrightnessSRGB = UserPrefBrightness;
    }
    //SaveMonitorSetting(); // need changed again
    return TRUE;
}

WORD GetBrightnessValue(void)
{
    return UserPrefBrightness; //GetScale100Value(UserPrefBrightness, 0, 100);
}
//====================================================
Bool AdjustFocus(MenuItemActionType action)
{
    WORD tempValue;
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
		#if AdjustFocusOSDValueBy1Step
		tempValue=DecIncValue(action, UserPrefPhase, 0, 100, 1);
		#else
	tempValue=DecIncValue(action, UserPrefPhase, 0, 127, 1);
		#endif
	if (tempValue==UserPrefPhase)
		return FALSE;
	UserPrefPhase=tempValue;
		#if AdjustFocusOSDValueBy1Step
		msADC_SetPhaseCode_100(UserPrefPhase);
		#else
	msADC_SetPhaseCode(UserPrefPhase);
		#endif
	#else
    tempValue = DecIncValue(action, UserPrefPhase, 0, 63, 1);
    if (tempValue == UserPrefPhase)
    {
        return FALSE;
    }
    UserPrefPhase = tempValue;
    mStar_AdjustPhase(UserPrefPhase);
	#endif
    return TRUE;
}

WORD GetFocusValue(void)
{
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
		#if AdjustFocusOSDValueBy1Step
		return GetScale100Value(UserPrefPhase, 0, 100);
		#else
	return GetScale100Value(UserPrefPhase, 0, 127);
		#endif
	#else
    return GetScale100Value(UserPrefPhase, 0, 63);
	#endif
    //return UserPrefPhase;
}

//====================================================
#define MaxClock    (StandardModeHTotal+50)
#define MinClock    (StandardModeHTotal-50)
Bool AdjustClock(MenuItemActionType action)
{
    WORD tempValue;
    tempValue = DecIncValue(action, UserPrefHTotal, MinClock, MaxClock, 1);
    if (tempValue == UserPrefHTotal)
    {
        return FALSE;
    }
    UserPrefHTotal = tempValue;
    #if WaitForDataBlanking  //zjb20071219
        mStar_WaitForDataBlanking();
    #endif
	#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
	msADC_AdjustHTotal(UserPrefHTotal);
	#else
    mStar_AdjustHTotal(UserPrefHTotal);
	#endif
    return TRUE;
}

WORD GetClockValue(void) // adjust range
{
    return GetScale100Value(UserPrefHTotal, MinClock, MaxClock);
}
//#define MaxHStart   UserPrefAutoHStart+50
//#define MinHStart   UserPrefAutoHStart-50
//====================================================
Bool AdjustHPosition(MenuItemActionType action)
{
    WORD tempValue;
	if (action==MIA_IncValue)
		action=MIA_DecValue;
	else if (action==MIA_DecValue)
		action=MIA_IncValue;
    //  tempValue=DecIncValue(action, UserPrefHStart, 0, 2*StandardModeHStart, 1);
    tempValue = DecIncValue(action, UserPrefHStart, MinHStart, MaxHStart, 1);
    if (tempValue == UserPrefHStart)
    {
        return FALSE;
    }
    UserPrefHStart = tempValue;
    mStar_AdjustHPosition(UserPrefHStart);
    return TRUE;
}

WORD GetHPositionValue(void)
{
    return 100-GetScale100Value(UserPrefHStart, MinHStart, MaxHStart); //, 0);
    // return GetScale100Value(UserPrefHStart, MinHStart, MaxHStart);
}
//====================================================
//#define MaxVStart 2*UserPrefAutoVStart-1
//#define MinVStart 1  // 2006/10/26 4:18PM by Emily  0
//====================================================
Bool AdjustVPosition(MenuItemActionType action)
{
    WORD tempValue;
    if (DecVScaleFlag)
        tempValue = DecIncValue(action, UserPrefVStart, (VScaleDecValue / 2), 100+(VScaleDecValue / 2), 1);
    else
    #if 1 //2004/01/15 num11
     tempValue = DecIncValue(action, UserPrefVStart, MinVStart, MaxVStart, 1);
    #else
        tempValue = DecIncValue(action, UserPrefVStart, 0, 2 *StandardModeVStart, 1);
    #endif
    if (tempValue == UserPrefVStart)
    {
        return FALSE;
    }
    UserPrefVStart = tempValue;
    mStar_AdjustVPosition(UserPrefVStart);
    return TRUE;
}

WORD GetVPositionValue(void)
{
    //Osd_Draw4Num(11,1,UserPrefVStart);
    //Osd_Draw4Num(11,2,DecVScaleFlag);
    //Osd_Draw4Num(18,1,VScaleDecValue);
    //Osd_Draw4Num(18,2,DecVScaleFlag);
    if (DecVScaleFlag)
        return GetScale100Value(UserPrefVStart, (VScaleDecValue / 2), 100+(VScaleDecValue / 2));
    else
    #if 1 //2004/01/15 num11
            return GetScale100Value(UserPrefVStart, MinVStart, MaxVStart);
    #else
        return GetScale100Value(UserPrefVStart, 0, 2 *StandardModeVStart);
    #endif
}
//====================================================
#if AudioFunc
    Bool AdjustVolume(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefVolume, MinVolume, MaxVolume, 1);
        if (tempValue == UserPrefVolume)
        {
            return FALSE;
        }
        UserPrefVolume = tempValue;
        mStar_AdjustVolume(UserPrefVolume);
        return TRUE;
    }
    WORD GetVolumeValue(void)
    {
        return GetScale100Value(UserPrefVolume, MinVolume, MaxVolume);
    }
#endif
#if 1
    //=====================================================
    Bool AdjustOSDHPosition(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefOsdHStart, 0, 100, 5); //0729 New Spec
        if (tempValue == UserPrefOsdHStart)
        {
            return FALSE;
        }
        UserPrefOsdHStart = tempValue;
	 #if OSD_PIVOT_EN  //071226
	 if(OSDRotateFlag)
        Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);	 	
	 else	
	 #endif	
        Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
#if Enable_OsdEngine1
	 Osd_SetOsdEngine(1);
	 #if OSD_PIVOT_EN  //071226
	 if(OSDRotateFlag)
        Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);	 	
	 else	
	 #endif	
        Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
	 Osd_SetOsdEngine(0);
#endif
        return TRUE;
    }
    WORD GetOSDHPositionValue(void)
    {
        return GetScale100Value(UserPrefOsdHStart, 0, 100);
        //   return ( UserPrefOsdHStart % 101 );
    }
    //================================================================================
    Bool AdjustOSDVPosition(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefOsdVStart, 0, 100, 5); //0729 New Spec
        if (tempValue == UserPrefOsdVStart)
        {
            return FALSE;
        }
        UserPrefOsdVStart = tempValue;
	 #if OSD_PIVOT_EN   //071226
	 if(OSDRotateFlag)
        Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);	 	
	 else	
	 #endif			
        Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
#if Enable_OsdEngine1
	 Osd_SetOsdEngine(1);
	 #if OSD_PIVOT_EN	//071226
	 if(OSDRotateFlag)
        Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);	 	
	 else	
	 #endif	
        Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
	 Osd_SetOsdEngine(0);
#endif
        return TRUE;
    }
    WORD GetOSDVPositionValue(void)
    {
        return GetScale100Value(UserPrefOsdVStart, 0, 100);
        //return GetScale100Value(UserPrefOsdVStart, 0, 100);
    }
    //================================================================================
    Bool AdjustOSDTransparence(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefOsdTrans, 0, 4, 1); //0729 New Spec
        if (tempValue == UserPrefOsdTrans)
        {
            return FALSE;
        }
        UserPrefOsdTrans = tempValue;
        Osd_SetTransparence(UserPrefOsdTrans);
#if Enable_OsdEngine1
	 Osd_SetOsdEngine(1);
        Osd_SetTransparence(UserPrefOsdTrans);
	 Osd_SetOsdEngine(0);
#endif
        return TRUE;
    }
    WORD GetOSDTransparence(void)
    {
        return GetScale100Value(UserPrefOsdTrans, 0, 4);
    }
    //================================================================================
    Bool AdjustOSDTime(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefOsdTime, 5, 120, 5); // 071218 modified for osd time out
        if (tempValue == UserPrefOsdTime)
        {
            return FALSE;
        }
        UserPrefOsdTime = tempValue;
		    //mStar_WriteByte(BCR, 113+tempValue); 
		    //mStar_WriteByte(BCG, 113+tempValue); 
		    //mStar_WriteByte(BCB, 113+tempValue); 
        return TRUE;
    }
    WORD GetOSDTimeValue(void)
    {
        return UserPrefOsdTime;
    }
    WORD GetOSDTime100Value(void)
    {
        return GetScale100Value(UserPrefOsdTime, 0, 120); // 071218 modified for osd time out
    }
#endif

#ifdef OffPower          //090331 xiandi.yu Auto Off Power
Bool AdjustAutoPowerOff(MenuItemActionType action)
{
    WORD tempValue;

    tempValue = DecIncValue(action, UserPrefAutoPowerOff, MinAutoPowerOffTime, MaxAutoPowerOffTime, 1); 
    
    if (tempValue == UserPrefAutoPowerOff)
    {
        return FALSE;
    }
    UserPrefAutoPowerOff = tempValue;
    AutoPowerOffSTime=0;
    AutoPowerOffMTime=0;
    AutoPowerOffHTime=0;
	ShowOffPowerWarningFlag = 0;
    
    return TRUE;
}
    WORD GetAutoPowerOffValue(void)
    {
        return UserPrefAutoPowerOff;
    }
#endif
#if Enable_YMC
    // bit 7 6 5 4 3 2 1 0
    //         Y C M R G B
    void SetYCMColor(MenuItemActionType action, WORD color, WORD value)
    {
        //printData("color %d", color);
        //printData("value %d", value);
        if (color &BIT5)
        // Y
        {
            if (action == MIA_IncValue)
            // inc
            {
                if (value + UserPrefYColor >= MaxColorValue)
                    UserPrefYColor = MaxColorValue;
                else
                    UserPrefYColor += value;
            }
            else
            // dec
            {
                if (UserPrefYColor == 0 || ((UserPrefYColor - value) <= 0))
                    UserPrefYColor = 0;
                else
                    UserPrefYColor -= value;
            }
            //printData("y %d", UserPrefYColor);
        }
        if (color &BIT4)
        // C
        {
            if (action == MIA_IncValue)
            // inc
            {
                if (value + UserPrefCColor >= MaxColorValue)
                    UserPrefCColor = MaxColorValue;
                else
                    UserPrefCColor += value;
            }
            else
            // dec
            {
                if (UserPrefCColor == 0 || ((UserPrefCColor - value) <= 0))
                    UserPrefCColor = 0;
                else
                    UserPrefCColor -= value;
            }
            //printData("c %d", UserPrefCColor);
        }
        if (color &BIT3)
        // M
        {
            if (action == MIA_IncValue)
            // inc
            {
                if (value + UserPrefMColor >= MaxColorValue)
                    UserPrefMColor = MaxColorValue;
                else
                    UserPrefMColor += value;
            }
            else
            // dec
            {
                if (UserPrefMColor == 0 || ((UserPrefMColor - value) <= 0))
                    UserPrefMColor = 0;
                else
                    UserPrefMColor -= value;
            }
            //printData("m %d", UserPrefMColor);
        }
        if (color &BIT2)
        // Red
        {
            if (action == MIA_IncValue)
            // inc
            {
                if (value + UserPrefRedColor >= MaxColorValue)
                    UserPrefRedColor = MaxColorValue;
                else
                    UserPrefRedColor += value;
            }
            else
            // dec
            {
                if (UserPrefRedColor == 0 || ((UserPrefRedColor - value <= 0)))
                    UserPrefRedColor = 0;
                else
                    UserPrefRedColor -= value;
            }
            //printData("red %d", UserPrefRedColor);
            UserPrefRedColorUser = UserPrefRedColor;
            mStar_AdjustRedColor(UserPrefRedColorUser, UserPrefContrast);
        }
        if (color &BIT1)
        // Green
        {
            if (action == MIA_IncValue)
            // inc
            {
                if (value + UserPrefGreenColor >= MaxColorValue)
                    UserPrefGreenColor = MaxColorValue;
                else
                    UserPrefGreenColor += value;
            }
            else
            // dec
            {
                if (UserPrefGreenColor == 0 || ((UserPrefGreenColor - value <= 0)))
                    UserPrefGreenColor = 0;
                else
                    UserPrefGreenColor -= value;
            }
            //printData("green %d", UserPrefGreenColor);
            UserPrefGreenColorUser = UserPrefGreenColor;
            mStar_AdjustGreenColor(UserPrefGreenColorUser, UserPrefContrast);
        }
        if (color &BIT0)
        // Blue
        {
            if (action == MIA_IncValue)
            // inc
            {
                if (value + UserPrefBlueColor >= MaxColorValue)
                    UserPrefBlueColor = MaxColorValue;
                else
                    UserPrefBlueColor += value;
            }
            else
            // dec
            {
                if (UserPrefBlueColor == 0 || ((UserPrefBlueColor - value <= 0)))
                    UserPrefBlueColor = 0;
                else
                    UserPrefBlueColor -= value;
            }
            //printData("blue %d", UserPrefBlueColor);
            UserPrefBlueColorUser = UserPrefBlueColor;
            mStar_AdjustBlueColor(UserPrefBlueColorUser, UserPrefContrast);
        }
    }
    //====================================================
    Bool AdjustYColor(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefYColor, 0, MaxColorValue, 1);
        if (tempValue == UserPrefYColor)
        {
            return FALSE;
        }
        if (action == MIA_IncValue)
        // Y=R+G ; C=G+B ; M=B+R
        {
            SetYCMColor(action, 0x1e, tempValue - UserPrefYColor); // YCMRGB:01 1110
        }
        else
        {
            SetYCMColor(action, 0x1e, UserPrefYColor - tempValue); // YCMRGB:01 1110
        }
        UserPrefYColor = tempValue;
        #if UsesRGB
            mStar_AdjustContrast(UserPrefContrast);
        #endif
        return TRUE;
    }
    WORD GetYColorValue(void)
    {
        return GetScale100Value(UserPrefYColor, 0, MaxColorValue);
    }
    //====================================================
    Bool AdjustCColor(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefCColor, 0, MaxColorValue, 1);
        if (tempValue == UserPrefCColor)
        {
            return FALSE;
        }
        if (action == MIA_IncValue)
        // Y=R+G ; C=G+B ; M=B+R
            SetYCMColor(action, 0x2b, tempValue - UserPrefCColor);
        // YCMRGB:10 1011
        else
            SetYCMColor(action, 0x2b, UserPrefCColor - tempValue);
        // YCMRGB:10 1011
        UserPrefCColor = tempValue;
        #if UsesRGB
            mStar_AdjustContrast(UserPrefContrast);
        #endif
        return TRUE;
    }
    WORD GetCColorValue(void)
    {
        return GetScale100Value(UserPrefCColor, 0, MaxColorValue);
    }
    //====================================================
    Bool AdjustMColor(MenuItemActionType action)
    {
        WORD tempValue;
        tempValue = DecIncValue(action, UserPrefMColor, 0, MaxColorValue, 1);
        if (tempValue == UserPrefMColor)
        {
            return FALSE;
        }
        if (action == MIA_IncValue)
        // Y=R+G ; C=G+B ; M=B+R
            SetYCMColor(action, 0x35, tempValue - UserPrefMColor);
        // YCMRGB:11 0101
        else
            SetYCMColor(action, 0x35, UserPrefMColor - tempValue);
        // YCMRGB:11 0101
        UserPrefMColor = tempValue;
        #if UsesRGB
            mStar_AdjustContrast(UserPrefContrast);
        #endif
        return TRUE;
    }
    WORD GetMColorValue(void)
    {
        return GetScale100Value(UserPrefMColor, 0, MaxColorValue);
    }
#endif
//====================================================
//====================================================
Bool AdjustBlueColor(MenuItemActionType action)
{
    WORD tempValue;
      tempValue = DecIncValue(action, UserPrefBlueColor, MinColorValue, MaxColorValue, ((UserPrefColorTemp == CTEMP_USER)?1:1));
 
	//tempValue = DecIncValue(action, UserPrefBlueColor, 0, MaxColorValue, 1);
    if (tempValue == UserPrefBlueColor)
    {
        return FALSE;
    }
    #if Enable_YMC
        if (UserPrefColorTemp == CTEMP_USER)
        // R=Y+M B=M+C G=C+Y
        {
            if (action == MIA_IncValue)
            // Y=R+G ; C=G+B ; M=B+R
                SetYCMColor(action, 0x18, tempValue - UserPrefBlueColor);
            // YCMRGB:01 1000
            else
                SetYCMColor(action, 0x18, UserPrefBlueColor - tempValue);
            // YCMRGB:01 1000
        }
    #endif
    UserPrefBlueColor = tempValue;
    UserPrefBlueColorUser = UserPrefBlueColor;
    #if UsesRGB
        mStar_AdjustContrast(UserPrefContrast);
    #else
        mStar_AdjustBlueColor(UserPrefBlueColorUser, UserPrefContrast);
    #endif
    #if 0
        if (UserPrefDcrMode == 1)
        // jeff add in 1112
        {
            msAccOnOff(0); // 2006/11/9 11:27PM by Emily test
            UserPrefColorFlags = 0;
            UserPrefDcrMode = 0;
            ReadColorTempSetting();
        }
    #endif
    return TRUE;
}

WORD GetBlueColorValue(void)
{
    return GetScale100Value(UserPrefBlueColor, MinColorValue, MaxColorValue);
}
//====================================================
Bool AdjustGreenColor(MenuItemActionType action)
{
    WORD tempValue;
	 tempValue = DecIncValue(action, UserPrefGreenColor, MinColorValue, MaxColorValue, ((UserPrefColorTemp == CTEMP_USER)?1:1));
 
   // tempValue = DecIncValue(action, UserPrefGreenColor, 0, MaxColorValue, 1);
    if (tempValue == UserPrefGreenColor)
    {
        return FALSE;
    }
    #if Enable_YMC
        if (UserPrefColorTemp == CTEMP_USER)
        // R=Y+M B=M+C G=C+Y
        {
            if (action == MIA_IncValue)
            // Y=R+G ; C=G+B ; M=B+R
                SetYCMColor(action, 0x30, tempValue - UserPrefGreenColor);
            // YCMRGB:11 0000
            else
                SetYCMColor(action, 0x30, UserPrefGreenColor - tempValue);
            // YCMRGB:11 0000
        }
    #endif
    UserPrefGreenColor = tempValue;
    UserPrefGreenColorUser = UserPrefGreenColor;
    #if UsesRGB
        mStar_AdjustContrast(UserPrefContrast);
    #else
        mStar_AdjustGreenColor(UserPrefGreenColorUser, UserPrefContrast);
    #endif
    #if 0
        if (UserPrefDcrMode == 1)
        // jeff add in 1112
        {
            msAccOnOff(0); // 2006/11/9 11:27PM by Emily test
            UserPrefColorFlags = 0;
            UserPrefDcrMode = 0;
            ReadColorTempSetting();
        }
    #endif
    return TRUE;
}
WORD GetGreenColorValue(void)
{
    return GetScale100Value(UserPrefGreenColor, MinColorValue, MaxColorValue);
}
//====================================================
Bool AdjustRedColor(MenuItemActionType action)
{
    WORD tempValue;
   // tempValue = DecIncValue(action, UserPrefRedColor, 0, MaxColorValue, 1);
    tempValue = DecIncValue(action, UserPrefRedColor, MinColorValue, MaxColorValue, ((UserPrefColorTemp == CTEMP_USER)?1:1));
 
    if (tempValue == UserPrefRedColor)
    {
        return FALSE;
    }
    #if Enable_YMC
        if (UserPrefColorTemp == CTEMP_USER)
        // R=Y+M B=M+C G=C+Y
        {
            if (action == MIA_IncValue)
            // Y=R+G ; C=G+B ; M=B+R
            {
                SetYCMColor(action, 0x28, tempValue - UserPrefRedColor); // YCMRGB:10 1000
            }
            else
            {
                SetYCMColor(action, 0x28, UserPrefRedColor - tempValue); // YCMRGB:10 1000
            }
        }
    #endif
    UserPrefRedColor = tempValue;
    UserPrefRedColorUser = UserPrefRedColor;
    #if UsesRGB
        mStar_AdjustContrast(UserPrefContrast);
    #else
        mStar_AdjustRedColor(UserPrefRedColorUser, UserPrefContrast);
    #endif
    #if 0
        if (UserPrefDcrMode == 1)
        // jeff add in 1112
        {
            msAccOnOff(0); // 2006/11/9 11:27PM by Emily test
            UserPrefColorFlags = 0;
            UserPrefDcrMode = 0;
            ReadColorTempSetting();
        }
    #endif
    return TRUE;
}
WORD GetRedColorValue(void)
{
    return GetScale100Value(UserPrefRedColor, MinColorValue, MaxColorValue);
}

//====================================================
Bool SetColorTemp(void)
{
    ReadColorTempSetting();
    //if (UserPrefColorTemp == CTEMP_SRGB)
    //{
    //    UserPrefECOMode = ECO_Standard;
    //}
    //Set_SaveSettingFlag();//1020
    return TRUE;
}
Bool AdjustColorTempMode(MenuItemActionType action)
{
    BYTE temp;
    temp = UserPrefColorTemp;
    if (action == MIA_DecValue)
    {
        UserPrefColorTemp = (UserPrefColorTemp + CTEMP_Nums - 1) % CTEMP_Nums;
    }
    else
    {
        UserPrefColorTemp = (UserPrefColorTemp + 1) % CTEMP_Nums;
    }
    if (temp == UserPrefColorTemp)
        return FALSE;

    #if Enable_Lightsensor // 091029 coding
    if(UserPrefColorTemp==CTEMP_SRGB&&(UserPrefDcrMode||UserPrefiCareMode))
    {
        UserPrefDcrMode = 0;
        UserPrefiCareMode = LightSensorOff;
        msAccOnOff(0);
        msDlcOnOff(0);
    }
   #else
     if(UserPrefColorTemp==CTEMP_SRGB&&UserPrefDcrMode)
    	{
	    	UserPrefDcrMode = 0;
		msAccOnOff(0);
		msDlcOnOff(0);
    	}
   #endif
   
    ReadColorTempSetting();

	if (UserPrefECOMode != ECO_Standard)
		//jeff add in 1112
		UserPrefECOMode = ECO_Standard;
   
//    if (UserPrefColorTemp == CTEMP_SRGB)
    //jeff add in 1112
        ;//UserPrefECOMode = ECO_Standard;
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(ECOMode), UserPrefECOMode);
	NVRam_WriteByte(nvrMonitorAddr(ColorTemp), UserPrefColorTemp);
    #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    /*msAccOnOff(0); // 2006/11/9 11:27PM by Emily test
    UserPrefColorFlags = 0;*/
    return TRUE;
}

Bool AdjustLanguage(MenuItemActionType action)
{
    BYTE temp,i;
    temp = UserPrefLanguage;

    if (action == MIA_DecValue)
    {
        UserPrefLanguage = (UserPrefLanguage + LANG_Nums - 1) % LANG_Nums;
    }
    else
    {
        UserPrefLanguage = (UserPrefLanguage + 1) % LANG_Nums;
    }

    if (temp == UserPrefLanguage)
        return FALSE;

    RunLanguage();   //edit by lizzie.ding 100601, for new define language,don't forget define language address
     Set_LoadFontFlag();

     //LoadCommonFont1();
    // Osd_Hide();    //071222  for switch language 
	   Osd_DrawContinuesChar(0,1,SpaceFont,OsdWindowWidth);
	   Osd_DrawContinuesChar(0,2,SpaceFont,OsdWindowWidth);
	 for (i=0;i<5;i++)
	   Osd_DrawContinuesChar(0,i*2+OSD_HPosition,SpaceFont,OsdWindowWidth);
    

    return TRUE;
}

void RunLanguage(void)  //edit by lizzie.ding 100601, for new define language,don't forget define language font address
{
    #if LANGUAGE_TYPE == ASIA
      if ((UserPrefLanguage <= LANG_Russia)||UserPrefLanguage == LANG_SChina)//jeff 1222
        {
            LanguageIndex = 0;
            SecondTblAddr = 0xFF;
        }
        else if (UserPrefLanguage == LANG_TChina)
        {
            LanguageIndex = 1;
            SecondTblAddr = 0x80;
        }
        else if (UserPrefLanguage == LANG_Korea)
        {
            LanguageIndex = 2;
            SecondTblAddr= 0x7A;//SecondTblAddr = 0x80; //SecondTblAddr = 0xFF;
        }
    #elif LANGUAGE_TYPE == EUROPE
        LanguageIndex = 0;
        SecondTblAddr = 0xFF;
    #elif LANGUAGE_TYPE == ID2007Standard
        LanguageIndex = 1;
        SecondTblAddr = 0xFF;
     #elif LANGUAGE_TYPE == All13Language
		if (UserPrefLanguage == LANG_SChina || UserPrefLanguage == LANG_English)
        {
            LanguageIndex = 0;
            SecondTblAddr = 0xFF;
        }
        else if (UserPrefLanguage == LANG_TChina)
        {
            LanguageIndex = 1;
            SecondTblAddr = 0x80;
        }
        else if (UserPrefLanguage == LANG_Korea)
        {
            LanguageIndex = 2;
            SecondTblAddr = 0x7A;
        }
		else
        {
            LanguageIndex = 3;
            SecondTblAddr = 0x80;
        }
	#elif LANGUAGE_TYPE == All14Language_AddJap
		if (UserPrefLanguage == LANG_SChina || UserPrefLanguage == LANG_English)
        {
            LanguageIndex = 0;
            SecondTblAddr = 0xFF;
        }
        else if (UserPrefLanguage == LANG_TChina)
        {
            LanguageIndex = 1;
            SecondTblAddr = 0x80;
        }
        else if (UserPrefLanguage == LANG_Korea)
        {
            LanguageIndex = 2;
            SecondTblAddr = 0x7A;
        }
		else if (UserPrefLanguage == LANG_Japan)
        {
            LanguageIndex = 4;
            SecondTblAddr = 0x70;
        }
		else
        {
            LanguageIndex = 3;
            SecondTblAddr = 0x80;
        }
    #endif

}

#ifdef EnableLightBar
Bool AdjustLightBar(MenuItemActionType action)
{
    BYTE tempVal = UserPrefLightBar;

    if(action == MIA_IncValue)
        UserPrefLightBar = (UserPrefLightBar >= 100)?(100):(UserPrefLightBar+10);
    else
        UserPrefLightBar = (UserPrefLightBar >= 10)?(UserPrefLightBar-10):(0);

    if(tempVal == UserPrefLightBar)
        return FALSE;

    /*
    if(UserPrefLightBar == 0)
        ScanKey_EnablePWM(0);
    else if(UserPrefLightBar == 10) // just write one time for enable pwm output
        ScanKey_EnablePWM(1);

    if(UserPrefLightBar >= 10)
    {
       ScanKey_CtrlPWM(UserPrefLightBar);
    }
    */
    ScanKey_PWMCtrl(UserPrefLightBar);

    return TRUE;
    
}

WORD GetLightBarValue(void)
{
    return GetScale100Value(UserPrefLightBar, 0, 100);
}
#endif

/*
Bool SetToEnglish( void )
{
UserPrefLanguage = LANG_English;
Set_SaveSettingFlag();
return TRUE;
}
 */
//========== ADJUST INPUT PORT =======================
#if 1//Dual
    //WORD GetLanguageValue(void)
    //{ return UserPrefLanguage;
    //}
    #if 0
        Bool AdjustInputSource(MenuItemActionType action)
        {
            if (action == MIA_IncValue)
                UserPrefInputSelectType = (UserPrefInputSelectType + 1) % Input_Nums;
            else
                UserPrefInputSelectType = (UserPrefInputSelectType + Input_Nums - 1) % Input_Nums;
            //printData("Input Type %d",UserPrefInputSelectType);
            return TRUE;
        }
    #endif

#if Dual
 Bool AdjustSource(MenuItemActionType action)
        {
#if 1 // 071218 source select
            if (action == MIA_DecValue)
            {
                UserPrefInputSelectType = (UserPrefInputSelectType + InputSelectType_Nums - 1) % InputSelectType_Nums;
            }
            else
            {
                UserPrefInputSelectType = (UserPrefInputSelectType + 1) % InputSelectType_Nums;
            }
            return TRUE;
#else
        	action = action;
		#if ENABLE_HDMI&&(MainBoardType==MainBoard_2810_B || MainBoardType==MainBoard_715G2808)//yuanfa
		
		if(UserPrefInputType==Input_Analog1)
			UserPrefInputType=Input_Digital;//yuanfa
		else 
			UserPrefInputType=Input_Analog1;
		
		#else

		if(UserPrefInputType == Input_Digital)
			UserPrefInputType=Input_Analog1;
		else
			UserPrefInputType=Input_Digital;
		#endif
		
	//	UserPrefInputType = SrcInputType ;
		return TRUE;
#endif
		
        }
#endif

#if(!Youweipaike)
    Bool ChangeSource(void)
    {       
            // 071218 source select
        #if ScanKey
            #if Scan_Source == Cypress
                hw_SetGreenLed();
            #else
                hw_SetGreenLed();
                hw_ClrAmberLed();
            #endif
        #else
            hw_SetGreenLed();
            hw_ClrAmberLed();
        #endif            

#if AudioFunc
        mStar_AdjustVolume(0);
#endif
        
// 071221 modified for changing mode should mute audio immediately
#if ENABLE_HDMI 
        if (gScInfo.InputTmdsType == TMDS_HDMI)
            mstar_HDMIAudioMute(HDMI_AUDIO_OFF);
#endif

		Power_TurnOffPanel();
		mStar_SetupInputPort();
		Set_InputTimingChangeFlag();
		Set_ShowInputInfoFlag();
		SrcFlags |= SyncLoss;
		return TRUE;
    }
#endif
    #if DDCCI_ENABLE
        Bool AdjustDDCCI(MenuItemActionType action)
        {
            action = action;
            if (DDCciFlag)
                Clr_DDCciFlag();
            else
                Set_DDCciFlag();
            return TRUE;
        }
        Bool ExecDDC2Bi(void)
        {
            if (DDCciFlag)
                Clr_DDCciFlag();
            else
                Set_DDCciFlag();
            //DDC2Bi_Init();
            return TRUE;
        }
    #endif
#if HotInputSelect

  
		Bool SetInputToAnalog(void)
		{

			if (SrcInputType == Input_Analog1&&!FreeRunModeFlag)
			{
				return FALSE;
			}
			UserPrefInputType=SrcInputType = Input_Analog1;			
	
			if(UserPrefInputSelectType!=InputSelectType_Auto)
			{
				UserPrefInputSelectType=InputSelectType_Analog;
			}
			Set_SaveSettingFlag();
			ChangeSource();
			return TRUE;
		}
	
	#if INPUT_TYPE==INPUT_1A2D||INPUT_TYPE==INPUT_1A1D
			Bool SetInputToDigital(void)
			{
				if (SrcInputType == Input_Digital&&!FreeRunModeFlag)
				{
					return FALSE;
				}
				UserPrefInputType = SrcInputType = Input_Digital;
				
				if(UserPrefInputSelectType!=InputSelectType_Auto)
				{
			 	UserPrefInputSelectType=InputSelectType_Digital;
				}	
				Set_SaveSettingFlag();
				ChangeSource();
				return TRUE;
			}
	#endif		
	#if INPUT_TYPE==INPUT_1A1H	
			Bool SetInputToHDMI(void)
			{
				if (SrcInputType == Input_Digital&&!FreeRunModeFlag)
				{
					return FALSE;
				}
				UserPrefInputType = SrcInputType = Input_Digital;
				
				if(UserPrefInputSelectType!=InputSelectType_Auto)
				{
			 	UserPrefInputSelectType=InputSelectType_HDMI;
				}	
				Set_SaveSettingFlag();
				ChangeSource();
				return TRUE;
			}
	#elif 	INPUT_TYPE==INPUT_1A2D
			Bool SetInputToHDMI(void)
			{
				if (SrcInputType == Input_Digital2&&!FreeRunModeFlag)
				{
					return FALSE;
				}
				UserPrefInputType = SrcInputType = Input_Digital2;
				
				if(UserPrefInputSelectType!=InputSelectType_Auto)
				{
				UserPrefInputSelectType=InputSelectType_HDMI;
				}	
				Set_SaveSettingFlag();
				ChangeSource();
				return TRUE;
			}
	#endif		
		
			Bool EnterHotInputMenu( void )
			{
				//OsdCounter = 5;
			   if(PowerSavingFlag)
			   {
					   Clr_PowerSavingFlag();
					   Power_TurnOnPanel();
			   }
				return TRUE;
			}

	
#endif
#if Dual
    Bool ChangeSourceToAnalog(void)
    {
        if (SrcInputType == Input_Analog1)
        {
            return FALSE;
        }
        SrcInputType = Input_Analog1;
        UserPrefInputType = SrcInputType;

        ChangeSource();
        return TRUE;
    }
#endif
    #if Dual
        Bool ChangeSourceToDigital(void)
        {
            //Delay1ms(200);
            if (SrcInputType == Input_Digital)
            {
                return FALSE;
            }
            SrcInputType = Input_Digital;
            UserPrefInputType = SrcInputType;
            ChangeSource();
            return TRUE;
        }
    #endif // dual
#endif
Bool ResetAllSetting(void)
{
    #if 1//ndef SETTING_IN_FLASH_12K
    ModeInfoType modeInfo;
    #endif
    WORD hFreq, vFreq;	
	//#if (ModelName == ENVISION_P2271w || ModelName == ENVISION_P2471w||ModelName==ENVISION_P971wl)//|| ModelName==ENVISION_P2071l)
	#ifdef GreenLEDDutyCtrl
	Set_DuringResetFlag();
	Set_SK_LEDVcc();
	#endif
    hFreq = HFreq(SrcHPeriod);
    vFreq = VFreq(hFreq, SrcVTotal);
   
    if (!FactoryModeFlag)
    {
        UserPrefOsdHStart = 50;
        UserPrefOsdVStart = 50;
    }

    #if !USEFLASH
    if(ProductModeFlag || FactoryModeFlag)
        FlashEnableWP();
    #endif
    
    #if INPUT_TYPE == INPUT_1A
    UserPrefInputSelectType = InputSelectType_Analog; 		
    #else
    UserPrefInputSelectType = InputSelectType_Auto; // 071221 modified for initial UserPrefInputSelectType
    #endif	
    #if AudioFunc
        UserPrefVolume = 50; //qx.long20061214//49;
        mStar_AdjustVolume(UserPrefVolume);
    #endif
    if (FactoryModeFlag)
    // User  R/G/B
    {
        UserPrefRedColor = 0xb2; //0xff;   // 0105 ask by LAI
        UserPrefGreenColor = 0xb2; //0xff; // 0105 ask by LAI
        UserPrefBlueColor = 0xb2; //0xff;  // 0105 ask by LAI
    }
    #if DDCCI_ENABLE
        UserprefRedBlackLevel = 50;
        UserprefGreenBlackLevel = 50;
        UserprefBlueBlackLevel = 50;
        mStar_AdjustUserPrefBlacklevel(UserprefRedBlackLevel, UserprefGreenBlackLevel, UserprefBlueBlackLevel);
    #endif
    // UserPrefLanguage=ResetLanguage;           //20050919 cancel
    #if Enable_Expansion
        UserprefExpansionMode = DefExpansion;
        //SetExpansionMode();//This Function will mute when reset all setting.
	mStar_SetScalingFactor(UserprefExpansionMode);
	#else
	mStar_SetScalingFactor();
    #endif
    UserPrefBrightnessUser = DefBrightness;
    UserPrefContrastUser = DefContrast;
    UserPrefBrightness = DefBrightness;
    UserPrefContrast = DefContrast;
    UserPrefBrightnessWarm1 = DefBrightness;
    UserPrefContrastWarm1 = DefContrast;
    UserPrefBrightnessNormal = DefBrightness;
    UserPrefContrastNormal = DefContrast;
    UserPrefBrightnessCool1 = DefBrightness;
    UserPrefContrastCool1 =DefContrast;
    UserPrefBrightnessUser = DefBrightness;
    UserPrefContrastUser = DefContrast;
    UserPrefSavedModeIndex = NumberOfMode;
    UserPrefOsdTime = 10;
    UserPrefOsdTrans = 0;
    Osd_SetTransparence(UserPrefOsdTrans);
	#ifdef OffPower           //090331 xiandi.yu Auto Off Power
	UserPrefAutoPowerOff=0;
    AutoPowerOffSTime=0;
    AutoPowerOffMTime=0;
    AutoPowerOffHTime=0;
	ShowOffPowerWarningFlag = 0;
	#endif
	
    #if 0//ESaverPowerDownFunc
	ESaverPowerDownCounter=0;
    #endif

    UserPrefColorTemp = DEF_CTEMP; //CTEMP_9300K 20051115 wmz
    SetColorTemp(); //9300K()
    UserPrefECOMode = ECO_Standard;
   if(UserPrefGamaMode != GAMA1)
   { UserPrefGamaMode = GAMA1;
    mStar_InitGamma();}
    UserPrefDcrMode = 0;
    UserPrefColorFlags = 0;
    
    #if DefaultIconExit//AOC_37_Series || ModelName==ENVISION_P2071l
    UserPrefLastMenuIndex=8;
    #else
    UserPrefLastMenuIndex=0;
    #endif
    
    #if Enable_Lightsensor
    UserPrefiCareMode = LightSensorOff;
    
    #endif

    #ifdef EnableLightBar
    UserPrefLightBar = DefLightBarValue;
	ScanKey_PWMCtrl(UserPrefLightBar);
    #endif
    
    // 2006/11/10 5:15PM by KK not need reset this item    UserPrefLanguage = LANG_English;
    #if ECO
        SetECO();
    #endif
    UserPrefYColor = DefColor;
    UserPrefCColor = DefColor;
    UserPrefMColor = DefColor;
	if(ProductModeFlag||FactoryModeFlag)
		{
    UserPrefRedColorUser = DefColor;
    UserPrefGreenColorUser = DefColor;
    UserPrefBlueColorUser = DefColor;
		}
    UserPrefHue = 50;
    UserPrefSaturation = 0x80;
    UserPrefSubContrast = DefSubContrast;
    UserPrefSubBrightness = DefSubBrightness;
    UserprefHistogram1 = 0x40;
    UserprefHistogram2 = 0xB0;
    UserprefALha = 50;
    UserprefBata = 50;
    UserPrefBFSize = 1; //eshin 070327
    // 2006/10/26 6:46PM by Emily     UserPrefSubSharpness = 11;
    // 2006/10/26 6:46PM by Emily     msAdjustSharpness(UserPrefSubSharpness, 1);
    msAdjustSharpness(11, 1);
    msAdjustSubBrightness(UserPrefSubBrightness);
    msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
    msAdjustVideoHue(UserPrefHue);
    msAdjustVideoSaturation(UserPrefSaturation);
    SetBFSize(UserPrefBFSize);//SetBFSize();
    
    #if USEFLASH
    UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #else
    SaveMonitorSetting(); //1020
    #endif
    
    if ((SrcInputType < Input_Digital))// && (!FactoryModeFlag)&&!ProductModeFlag)
    //for nw aoc factorymode
    {
    	Init_ModeSetting();	//for new spec.
        if((!FactoryModeFlag)&&!ProductModeFlag&&PowerOnFlag)//add PowerOnFlag for DC off/on close Burnin 101220 add by win
        {
            //mStar_AutoGeomtry();
            //UserPrefAutoTimes=0;
            if (mStar_AutoGeomtry() == TRUE)
            //0707
            {
                UserPrefAutoTimes = 1;
                UserPrefAutoHStart = UserPrefHStart;
                UserPrefAutoVStart = UserPrefVStart;
            }
        }
        UserPrefAutoHStart = UserPrefHStart;
        {
            BYTE modeIndex, flag;
            flag = SrcFlags; //UserModeFlag
            modeIndex = SaveIndex; //SrcModeIndex;
            SrcFlags &= ~bUserMode;
    #if !USEFLASH
            for (SaveIndex = 0; SaveIndex < NumberOfMode; SaveIndex++)
            {
                NVRam_WriteByte(nvrModeAddr(checksum), 0);
            }
            for (SaveIndex = 0; SaveIndex < NumberOfMode; SaveIndex++)
            {
                modeInfo.HerFreq = 0;
                modeInfo.VerFreq = 0;
                modeInfo.VTotal = 0;
                modeInfo.Flags = 0;
                NVRam_WriteTbl(nvrModeInfoAddr(SaveIndex), (BYTE*) &modeInfo, ModeInfoSize);
            }
            SaveIndex = 0;
            SrcFlags = flag;
            modeInfo.HerFreq = hFreq;
            modeInfo.VerFreq = vFreq;
            modeInfo.VTotal = SrcVTotal;
            modeInfo.Flags = SrcFlags &0x13;
            NVRam_WriteTbl(nvrModeInfoAddr(SaveIndex), (BYTE*) &modeInfo, ModeInfoSize);
            NVRam_WriteByte(nvrModeAddr(checksum), 0);
            NVRam_WriteByte(nvrMonitorAddr(SavedModeIndex), UserPrefSavedModeIndex);
            SaveModeSetting();
			
            #else
            	SaveIndex = SrcModeIndex;//0;
		  SrcFlags = flag;
            #if 1//ndef SETTING_IN_FLASH_12K
            	 modeInfo.HerFreq = hFreq;
            	 modeInfo.VerFreq = vFreq;
            	 modeInfo.VTotal = SrcVTotal;
            	 modeInfo.Flags = SrcFlags &0x13;
            #endif
            	Flash_ClearModeSet();
            	UserPref_EnableFlashSaveBit(bFlashSaveModeBit); 
             #endif
        }
    }
    else
    {
        Delay1ms(500);
    }
    #if OSD_PIVOT_EN  //071226
    if(OSDRotateFlag)
    Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);			
    else
    #endif	
	if(!(MenuPageIndex ==FactoryMenu))// petit 20130104
    Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
#if Enable_OsdEngine1
    Osd_SetOsdEngine(1);
    #if OSD_PIVOT_EN  //071226
    if(OSDRotateFlag)
    Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);			
    else
    #endif	
	if(!(MenuPageIndex ==FactoryMenu))// petit 20130104
    Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
    Osd_SetOsdEngine(0);
#endif
    if(FactoryModeFlag)
	Clr_FactoryModeFlag();
       Clr_BurninModeFlag();
	Clr_DoBurninModeFlag();//edit by lizzie,100429.for DC ON/OFF can't clr burnin mode the 1st time
	mStar_WriteByte(FWC, 0);
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
    #else
	SaveMonitorSetting();//UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
   
    #if 0//USEFLASH
	if(ProductModeFlag)
	{ 
		FactoryProductModeValue=ProductModeOffValue;
		SaveFactorySetting();	 
	}
    #endif	 
	Clr_DoResetFlag();
	#if DDCCI_ENABLE
	Set_DDCciFlag();
	#endif
	if(ProductModeFlag)
	{
		Clr_ProductModeFlag();
	#if USEFLASH
		MonitorBackLightTime=BlacklitTime = 0;
		UserPref_EnableFlashSaveBit(bFlashSaveMonitor2Bit); 
	#else
		BlacklitTime = 0;
		SaveBlacklitTime();
	#endif
	#if USEFLASH
		FactoryProductModeValue=ProductModeOffValue;
		SaveMonitorSetting();
		SaveFactorySetting();
	#else
		//WORD temp=ProductModeOffValue;
		//NVRam_WriteTbl(ProductModeAddr, (BYTE*)(&temp), 2);
		NVRam_WriteWord(ProductModeAddr,ProductModeOffValue);
	#endif
	   
		//hw_ClrGreenLed();
		//hw_SetAmberLed();
	}
	//#if (ModelName == ENVISION_P2271w || ModelName == ENVISION_P2471w||ModelName==ENVISION_P971wl)//|| ModelName==ENVISION_P2071l)
	#ifdef GreenLEDDutyCtrl
	Clr_DuringResetFlag();
	#endif
    return TRUE;
}
Bool AutoConfig(void)
{
    Bool result;
#if AutoProcess
    wAutoProcess0=0;	
    wAutoProcessCount=PrintAutoMsgPeriod;
#endif
    result = mStar_AutoGeomtry();
    //UserPrefAutoHStart=UserPrefHStart;
    if (result)
    {
		#if DEBUG_ENABLE
	printMsg("mStar_AutoGeomtry__PASS");
	#endif
        UserPrefAutoHStart = UserPrefHStart;
        UserPrefAutoVStart = UserPrefVStart;
        UserPrefAutoTimes = 1;
    }
        else
    {
	#if DEBUG_ENABLE
	printMsg("mStar_AutoGeomtry__FAILE");
	#endif
	
        UserPrefHTotal = StandardModeHTotal;
        UserPrefHStart = StandardModeHStart;
        UserPrefVStart = StandardModeVStart;
        UserPrefAutoHStart = UserPrefHStart;
        UserPrefAutoVStart = UserPrefVStart;
        UserPrefPhase = 18;
        UserPrefAutoTimes = 1;//for TPE austin request  petit 20121209
        		#if DEBUG_PRINTDATA
	printData("End99-- UserPrefHTotal %d", UserPrefHTotal);
	printData("H099 After auto %d",UserPrefHStart);
        printData("V999 After auto %d",UserPrefVStart);
		#endif

		#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		msADC_AdjustHTotal(UserPrefHTotal);
		msADC_SetPhaseCode(UserPrefPhase);
		#else
        mStar_AdjustHTotal(UserPrefHTotal);
        mStar_AdjustPhase(UserPrefPhase);
		#endif
        mStar_WriteWord(SPRHST_H, UserPrefHStart);
        mStar_WriteWord(SPRVST_H, UserPrefVStart);
    }
        #if USEFLASH
        UserPref_EnableFlashSaveBit(bFlashSaveModeBit); 
        #else
        SaveModeSetting();
        #endif

    return result;
}

#if 1
    /*
    Bool ResetBriConValue(void)
    {
    UserPrefBrightness=UserPrefBrightness6500K;
    UserPrefContrast=UserPrefContrast6500K;
    mStar_AdjustBrightness(UserPrefBrightness);
    mStar_AdjustContrast(UserPrefContrast);
    SaveMonitorSetting();
    return TRUE;
    }
     */
  
    Bool ResetGeometry(void)
    {

        #if !USEFLASH
        UserPrefHStart = StandardModeHStart;
        mStar_AdjustHPosition(UserPrefHStart);
        NVRam_WriteByte(nvrModeAddr(HStart), UserPrefHStart);
        UserPrefVStart = StandardModeVStart;
        mStar_AdjustVPosition(UserPrefVStart);
        NVRam_WriteByte(nvrModeAddr(VStart), UserPrefVStart);
        UserPrefHTotal = StandardModeHTotal;
		#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		msADC_AdjustHTotal(UserPrefHTotal);
		#else
        mStar_AdjustHTotal(UserPrefHTotal);
		#endif
        NVRam_WriteByte(nvrModeAddr(HTotal), UserPrefHTotal);
        UserPrefPhase = 0;
		#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		msADC_SetPhaseCode(UserPrefPhase);
		#else
        mStar_AdjustPhase(UserPrefPhase);
		#endif
        NVRam_WriteByte(nvrModeAddr(Phase), UserPrefPhase);
        AutoConfig();
       #else 
        UserPrefHStart = StandardModeHStart;
        mStar_AdjustHPosition(UserPrefHStart);
        UserPrefVStart = StandardModeVStart;
        mStar_AdjustVPosition(UserPrefVStart);
        UserPrefHTotal = StandardModeHTotal;
		#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		msADC_AdjustHTotal(UserPrefHTotal);
		#else
        mStar_AdjustHTotal(UserPrefHTotal);
		#endif
        UserPrefPhase = 0;
		#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		msADC_SetPhaseCode(UserPrefPhase);
		#else
        mStar_AdjustPhase(UserPrefPhase);
		#endif
        AutoConfig();	   
	#endif		
        return TRUE;
    }

#endif
// 090430 check
void SaveUserPref(void)
{
    #if USEFLASH
    UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    if (SrcInputType < Input_Digital)
    {
        UserPref_EnableFlashSaveBit(bFlashSaveModeBit);
    }
    #else
    SaveMonitorSetting();
    if (SrcInputType < Input_Digital)
    {
        SaveModeSetting();
    }
    #endif
}

void SaveUserPrefSetting(BYTE menuPageIndex)
{
    menuPageIndex = 0;
    SaveUserPref();
    Clr_SaveSettingFlag();
}
#if !USEFLASH
void SaveFactorySetting(BYTE itemIndex)
{
    if (itemIndex == 2)
    {
        NVRam_WriteByte(nvrMonitorAddr(AdcRedGain), UserPrefAdcRedGain);
    }
    else if (itemIndex == 4)
    {
        NVRam_WriteByte(nvrMonitorAddr(AdcGreenGain), UserPrefAdcGreenGain);
    }
    else if (itemIndex == 6)
    {
        NVRam_WriteByte(nvrMonitorAddr(AdcBlueGain), UserPrefAdcBlueGain);
    }
    else if (itemIndex == 8)
    {
        NVRam_WriteByte(nvrMonitorAddr(AdcRedOffset), UserPrefAdcRedOffset);
    }
    else if (itemIndex == 10)
    {
        NVRam_WriteByte(nvrMonitorAddr(AdcGreenOffset), UserPrefAdcGreenOffset);
    }
    else if (itemIndex == 12)
    {
        NVRam_WriteByte(nvrMonitorAddr(AdcBlueOffset), UserPrefAdcBlueOffset);
    }
    else if (itemIndex == 14)
    {
        NVRam_WriteByte(nvrMonitorAddr(RedColorCool1), UserPrefRedColorCool1);
    }
    else if (itemIndex == 16)
    {
        NVRam_WriteByte(nvrMonitorAddr(GreenColorCool1), UserPrefGreenColorCool1);
    }
    else if (itemIndex == 18)
    {
        NVRam_WriteByte(nvrMonitorAddr(BlueColorCool1), UserPrefBlueColorCool1);
    }
    else if (itemIndex == 20)
    {
        NVRam_WriteByte(nvrMonitorAddr(FBrightnessCool1), FUserPrefBrightnessCool1);
    }
    else if (itemIndex == 22)
    {
        NVRam_WriteByte(nvrMonitorAddr(FContrastCool1), FUserPrefContrastCool1);
    }
    else if (itemIndex == 24)
    {
        NVRam_WriteByte(nvrMonitorAddr(RedColorNormal), UserPrefRedColorNormal);
    }
    else if (itemIndex == 26)
    {
        NVRam_WriteByte(nvrMonitorAddr(GreenColorNormal), UserPrefGreenColorNormal);
    }
    else if (itemIndex == 28)
    {
        NVRam_WriteByte(nvrMonitorAddr(BlueColorNormal), UserPrefBlueColorNormal);
    }
    else if (itemIndex == 30)
    {
        NVRam_WriteByte(nvrMonitorAddr(FBrightnessNormal), FUserPrefBrightnessNormal);
    }
    else if (itemIndex == 32)
    {
        NVRam_WriteByte(nvrMonitorAddr(FContrastNormal), FUserPrefContrastNormal);
    }
    else if (itemIndex == 34)
    {
        NVRam_WriteByte(nvrMonitorAddr(RedColorWarm1), UserPrefRedColorWarm1);
    }
    else if (itemIndex == 36)
    {
        NVRam_WriteByte(nvrMonitorAddr(GreenColorWarm1), UserPrefGreenColorWarm1);
    }
    else if (itemIndex == 38)
    {
        NVRam_WriteByte(nvrMonitorAddr(BlueColorWarm1), UserPrefBlueColorWarm1);
    }
    else if (itemIndex == 40)
    {
        NVRam_WriteByte(nvrMonitorAddr(FBrightnessWarm1), FUserPrefBrightnessWarm1);
    }
    else if (itemIndex == 42)
    {
        NVRam_WriteByte(nvrMonitorAddr(FContrastWarm1), FUserPrefContrastWarm1);
    }
    else if (itemIndex == 44)
        NVRam_WriteByte(nvrMonitorAddr(RedColorsRGB), UserPrefRedColorSRGB);
    else if (itemIndex == 46)
        NVRam_WriteByte(nvrMonitorAddr(GreenColorsRGB), UserPrefGreenColorSRGB);
    else if (itemIndex == 48)
        NVRam_WriteByte(nvrMonitorAddr(BlueColorsRGB), UserPrefBlueColorSRGB);
    else if (itemIndex == 50)
    {
        NVRam_WriteByte(nvrMonitorAddr(BrightnesssRGB), UserPrefBrightnessSRGB);
    }
    else if (itemIndex == 52)
    {
        NVRam_WriteByte(nvrMonitorAddr(ContrastsRGB), UserPrefContrastSRGB);
    }
    else if (itemIndex == 54+Enable_ProductModeAdjust*2||itemIndex ==54+Enable_ProductModeAdjust*2+MS_OVD*2)
    {
        #if DEBUG_MSG
        printMsg("save MonitorFlags");
        #endif
        NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
    }
    else if (itemIndex == 56+Enable_ProductModeAdjust*2+EnablePanelServiceMenu*4+MS_OVD*2)
    {
        #if DEBUG_MSG
        printMsg("save MSBankNo");
        #endif
        NVRam_WriteByte(nvrMonitorAddr(BankNo), MSBankNo);
    }
    else if (itemIndex == 58+Enable_ProductModeAdjust*2+EnablePanelServiceMenu*4+MS_OVD*2)
    {
        #if DEBUG_MSG
        printMsg("save MSAddr");
        #endif
        NVRam_WriteByte(nvrMonitorAddr(Addr), MSAddr);
    }
    Clr_SaveSettingFlag();
}
#endif
#if 0// EnablePanelServiceMenu
#if 0//!PanelSettingInFlash
void SavePanelSettingByItem(BYTE itemIndex)
{
    //printData("itemIndex=%d",itemIndex);
    if (itemIndex == 1||itemIndex == 79)
    {
        NVRam_WriteByte(nvrPanelAddr(SettingFlag), PanelSettingFlag);
    }
    else if (itemIndex == 3)
    {
        NVRam_WriteByte(nvrPanelAddr(OnTiming1), PanelOnTiming1);
    }
    else if (itemIndex == 5)
    {
	 //NVRam_WriteTbl(nvrPanelAddr(OnTiming2), (BYTE*)(&PanelOnTiming2), 2);
        NVRam_WriteWord(nvrPanelAddr(OnTiming2),PanelOnTiming2);
    }
    else if (itemIndex == 7)
    {
	 //NVRam_WriteTbl(nvrPanelAddr(OffTiming1), (BYTE*)(&PanelOffTiming1), 2);
	 NVRam_WriteWord(nvrPanelAddr(OffTiming1),PanelOffTiming1);
    }
    else if (itemIndex == 9)
    {
        NVRam_WriteByte(nvrPanelAddr(OffTiming2), PanelOffTiming2);
    }
    else if (itemIndex == 11)
    {
        //NVRam_WriteTbl(nvrPanelAddr(HsyncWidth), (BYTE*)(&PanelHSyncWidth), 2);
        NVRam_WriteWord(nvrPanelAddr(HsyncWidth),PanelHSyncWidth);
    }
    else if (itemIndex == 13)
    {
        //NVRam_WriteTbl(nvrPanelAddr(HSyncBackPorch), (BYTE*)(&PanelHSyncBackPorch), 2);
        NVRam_WriteWord(nvrPanelAddr(HSyncBackPorch),PanelHSyncBackPorch);
    }
    else if (itemIndex == 15)
    {
        //NVRam_WriteTbl(nvrPanelAddr(VSyncWidth), (BYTE*)(&PanelVSyncWidth), 2);
        NVRam_WriteWord(nvrPanelAddr(VSyncWidth),PanelVSyncWidth);
    }
    else if (itemIndex == 17)
    {
        //NVRam_WriteTbl(nvrPanelAddr(VsyncBackPorch), (BYTE*)(&PanelVSyncBackPorch), 2);
        NVRam_WriteWord(nvrPanelAddr(VsyncBackPorch),PanelVSyncBackPorch);
    }
    else if (itemIndex == 19)
    {
        //NVRam_WriteTbl(nvrPanelAddr(Width), (BYTE*)(&PanelWidth), 2);
        NVRam_WriteWord(nvrPanelAddr(Width),PanelWidth);
    }
    else if (itemIndex == 21)
    {
        //NVRam_WriteTbl(nvrPanelAddr(Height), (BYTE*)(&PanelHeight), 2);
        NVRam_WriteWord(nvrPanelAddr(Height),PanelHeight);
    }
    else if (itemIndex == 23)
    {
        //NVRam_WriteTbl(nvrPanelAddr(HTotal), (BYTE*)(&PanelHTotal), 2);
        NVRam_WriteWord(nvrPanelAddr(HTotal),PanelHTotal);
    }
    else if (itemIndex == 25)
    {
        //NVRam_WriteTbl(nvrPanelAddr(VTotal), (BYTE*)(&PanelVTotal), 2);
        NVRam_WriteWord(nvrPanelAddr(VTotal),PanelVTotal);
    }
    else if (itemIndex == 27)
    {
        //NVRam_WriteTbl(nvrPanelAddr(MaxHTotal), (BYTE*)(&PanelMaxHTotal), 2);
        NVRam_WriteWord(nvrPanelAddr(MaxHTotal),PanelMaxHTotal);
    }
    else if (itemIndex == 29)
    {
        //NVRam_WriteTbl(nvrPanelAddr(MinHTotal), (BYTE*)(&PanelMinHTotal), 2);
        NVRam_WriteWord(nvrPanelAddr(MinHTotal),PanelMinHTotal);
    }
    else if (itemIndex == 31)
    {
        //NVRam_WriteTbl(nvrPanelAddr(MaxVTotal), (BYTE*)(&PanelMaxVTotal), 2);
        NVRam_WriteWord(nvrPanelAddr(MaxVTotal),PanelMaxVTotal);
    }
    else if (itemIndex == 33)
    {
        //NVRam_WriteTbl(nvrPanelAddr(MinVTotal), (BYTE*)(&PanelMinVTotal), 2);
        NVRam_WriteWord(nvrPanelAddr(MinVTotal),PanelMinVTotal);
    }
//    else if (itemIndex == 35)
//    {
//        NVRam_WriteByte(nvrPanelAddr(TypicalDCLK), PanelDCLK);
//    }
    else if (itemIndex == 35)
    {
        NVRam_WriteByte(nvrPanelAddr(MinDCLK), PanelMinDCLK);
    }
    else if (itemIndex == 37)
    {
        NVRam_WriteByte(nvrPanelAddr(MaxDCLK), PanelMaxDCLK);
    }
//    else if (itemIndex == 41||itemIndex == 43)
//    {
//        NVRam_WriteByte(nvrPanelAddr(OutputControl2), PanelOutputControl2);
//    }
    else if (itemIndex == 39||itemIndex == 41 || itemIndex == 43||itemIndex == 45\
		||itemIndex == 47||itemIndex == 49 || itemIndex == 51||itemIndex == 53)
    {
        NVRam_WriteByte(nvrPanelAddr(OutputControl1), PanelOutputControl1);
    }
    else if (itemIndex == 55||itemIndex == 57 || itemIndex == 59||itemIndex == 61)
    {
        NVRam_WriteByte(nvrPanelAddr(ODRriving), PanelODRriving);
    }
    else if (itemIndex == 63)
    {
        NVRam_WriteWord(nvrPanelAddr(BurstModeFreq), PanelBurstModeFreq);
    }
    else if (itemIndex == 65)
    {
        NVRam_WriteByte(nvrPanelAddr(DefDuty), PanelDefDuty);
    }
    else if (itemIndex == 67)
    {
        NVRam_WriteByte(nvrPanelAddr(MinDuty), PanelMinDuty);
    }
    else if (itemIndex == 69)
    {
        NVRam_WriteByte(nvrPanelAddr(MaxDuty), PanelMaxDuty);
    }
    else if (itemIndex == 71)
    {
        NVRam_WriteByte(nvrPanelAddr(SSCSTEPL), PanelSSCSTEPL);
    }
    else if (itemIndex == 73)
    {
        NVRam_WriteByte(nvrPanelAddr(SSCSTEPH), PanelSSCSTEPH);
    }
    else if (itemIndex == 75)
    {
        NVRam_WriteByte(nvrPanelAddr(SSCSPANL), PanelSSCSPANL);
    }
    else if (itemIndex == 77)
    {
        NVRam_WriteByte(nvrPanelAddr(SSCSPANH), PanelSSCSPANH);
    }

}
#endif

    Bool Test(void)
    {
        return TRUE;
    }
//
#endif

//=======================================================================================
// For Factory alignment
#if 1
    Bool AutoColor(void)
    {
        Bool result;
        if (SrcInputType == Input_Digital  || SrcInputType == Input_Digital2)
            return FALSE;// return TRUE; 20120307:problem DVI autocolor show "pass"
        result = mStar_AutoColor();
        Osd_SetTextColor(CP_RedColor, CP_WhiteColor);
        if (result)
        {
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
        }
/*
        if (UserPrefAutoTimes == 0&& !ProductModeFlag)
        {
            mStar_AutoGeomtry();
            UserPrefAutoTimes = 1;
            UserPrefAutoHStart = UserPrefHStart;
            UserPrefAutoVStart = UserPrefVStart;
            #if USEFLASH
            UserPref_EnableFlashSaveBit(bFlashSaveModeBit); 
            #else
            SaveModeSetting();
            #endif
        }
        else
        {
            Delay1ms(500);
        }
		//*/
        return result;
    }
    #if 1
        //====================================================
		#ifdef TSUMXXT		//110311 Modify
		#define ADC_RESOLUTION  0x0FFF
		#else
		#define ADC_RESOLUTION  0xFF
		#endif
        Bool AdjustRedGain(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefAdcRedGain, 0, ADC_RESOLUTION, 1);		//110311 Modify
            if (tempValue == UserPrefAdcRedGain)
            {
                return FALSE;
            }
            UserPrefAdcRedGain = tempValue;
			#ifdef TSUMXXT		//110311 Modify
			msADC_SetRGBGainCode(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
			#else
            		mStar_AdjustAdcRedGain(UserPrefAdcRedGain);
			#endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        WORD GetRedGainValue(void)
        {
        #if DEBUG_ENABLE
        printData("UserPrefAdcRedGain===   %d", UserPrefAdcRedGain);
	#endif
            return UserPrefAdcRedGain;
        }
        Bool AdjustGreenGain(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefAdcGreenGain, 0, ADC_RESOLUTION, 1);		//110311 Modify
            if (tempValue == UserPrefAdcGreenGain)
            {
                return FALSE;
            }
            UserPrefAdcGreenGain = tempValue;
			#ifdef TSUMXXT		//110311 Modify
			msADC_SetRGBGainCode(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
			#else
            mStar_AdjustAdcGreenGain(UserPrefAdcGreenGain);
			#endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        WORD GetGreenGainValue(void)
        {
            return UserPrefAdcGreenGain;
        }
        Bool AdjustBlueGain(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefAdcBlueGain, 0, ADC_RESOLUTION, 1);		//110311 Modify
            if (tempValue == UserPrefAdcBlueGain)
            {
                return FALSE;
            }
            UserPrefAdcBlueGain = tempValue;
			#ifdef TSUMXXT		//110311 Modify
			msADC_SetRGBGainCode(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
			#else
            mStar_AdjustAdcBlueGain(UserPrefAdcBlueGain);
			#endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        WORD GetBlueGainValue(void)
        {
            return UserPrefAdcBlueGain;
        }
        //====================================================
        //====================================================
        Bool AdjustAdcRedOffset(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefAdcRedOffset, 0, ADC_RESOLUTION, 1);		//110311 Modify
            if (tempValue == UserPrefAdcRedOffset)
            {
                return FALSE;
            }
            UserPrefAdcRedOffset = tempValue;
			#ifdef TSUMXXT		//110316 Modify
			#else
            mStar_AdjustAdcRedOffset(UserPrefAdcRedOffset);
			#endif
            return TRUE;
        }
        WORD GetAdcRedOffsetValue(void)
        {
            return UserPrefAdcRedOffset;
        }
        Bool AdjustAdcGreenOffset(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefAdcGreenOffset, 0, ADC_RESOLUTION, 1);		//110311 Modify
            if (tempValue == UserPrefAdcGreenOffset)
            {
                return FALSE;
            }
            UserPrefAdcGreenOffset = tempValue;
			#ifdef TSUMXXT		//110316 Modify
			#else
            mStar_AdjustAdcGreenOffset(UserPrefAdcGreenOffset);
			#endif
            return TRUE;
        }
        WORD GetAdcGreenOffsetValue(void)
        {
            return UserPrefAdcGreenOffset;
        }
        Bool AdjustAdcBlueOffset(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefAdcBlueOffset, 0, ADC_RESOLUTION, 1);		//110311 Modify
            if (tempValue == UserPrefAdcBlueOffset)
            {
                return FALSE;
            }
            UserPrefAdcBlueOffset = tempValue;
			#ifdef TSUMXXT		//110316 Modify
			#else
            mStar_AdjustAdcBlueOffset(UserPrefAdcBlueOffset);
			#endif
            return TRUE;
        }
        WORD GetAdcBlueOffsetValue(void)
        {
            return UserPrefAdcBlueOffset;
        }
    #endif
    void ReadColorTempSetting(void)
    {
        UserPrefContrast =  *ColorTempPtr[UserPrefColorTemp][1];
        UserPrefBrightness =  *ColorTempPtr[UserPrefColorTemp][0];
        UserPrefRedColor =  *ColorTempPtr[UserPrefColorTemp][2];
        UserPrefGreenColor =  *ColorTempPtr[UserPrefColorTemp][3];
        UserPrefBlueColor =  *ColorTempPtr[UserPrefColorTemp][4];
        CheckColorValueRange();
        UserPrefBrightness %= 101;
        // 2006/11/10 11:13PM by KK   if(!UserPrefDcrMode) // for under DCR mode, the picture show filter
        mStar_AdjustBrightness(UserPrefBrightness);
        mStar_AdjustContrast(UserPrefContrast);
        //msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
    }
    /*
    void ReadWarm1Setting( void )
    {
    //printMsg("Read Warm1");
    UserPrefContrast = UserPrefContrastWarm1;
    UserPrefBrightness = UserPrefBrightnessWarm1;
    //printData("Warm1 CONTRAST-->%d",UserPrefContrast);
    //printData("Warm1 BRIGHTNESS-->%d",UserPrefBrightness);
    UserPrefRedColor = UserPrefRedColorWarm1;
    UserPrefGreenColor = UserPrefGreenColorWarm1;
    UserPrefBlueColor = UserPrefBlueColorWarm1;
    CheckColorValueRange();
    UserPrefBrightness %= 101;
    }
    #if 0
    void ReadWarm2Setting(void)
    { //printMsg("Read Warm2");
    UserPrefContrast=UserPrefContrastWarm2;
    UserPrefBrightness=UserPrefBrightnessWarm2;
    //printData("Warm2 CONTRAST-->%d",UserPrefContrast);
    //printData("Warm2 BRIGHTNESS-->%d",UserPrefBrightness);
    CheckColorValueRange();
    UserPrefBrightness%=101;
    }
    #endif
    void ReadNormalSetting( void )
    {
    //printMsg("Read Normal");
    UserPrefContrast = UserPrefContrastNormal;
    UserPrefBrightness = UserPrefBrightnessNormal;
    //printData("Normal CONTRAST-->%d",UserPrefContrast);
    //printData("Normal BRIGHTNESS-->%d",UserPrefBrightness);
    UserPrefRedColor = UserPrefRedColorNormal;
    UserPrefGreenColor = UserPrefGreenColorNormal;
    UserPrefBlueColor = UserPrefBlueColorNormal;
    CheckColorValueRange();
    UserPrefBrightness %= 101;
    }
    void ReadCool1Setting( void )
    {
    //printMsg("Read Cool1");
    UserPrefContrast = UserPrefContrastCool1;
    UserPrefBrightness = UserPrefBrightnessCool1;
    //printData("Cool1 CONTRAST-->%d",UserPrefContrast);
    //printData("Cool1 BRIGHTNESS-->%d",UserPrefBrightness);
    UserPrefRedColor = UserPrefRedColorCool1;
    UserPrefGreenColor = UserPrefGreenColorCool1;
    UserPrefBlueColor = UserPrefBlueColorCool1;
    CheckColorValueRange();
    UserPrefBrightness %= 101;
    }
    #if 0
    void ReadCool2Setting(void)
    { //printMsg("Read Cool2");
    UserPrefContrast=UserPrefContrastCool2;
    UserPrefBrightness=UserPrefBrightnessCool2;
    //printData("Cool2 CONTRAST-->%d",UserPrefContrast);
    //printData("Cool2 BRIGHTNESS-->%d",UserPrefBrightness);
    CheckColorValueRange();
    UserPrefBrightness%=101;
    }
    #endif
    void ReadSRGBSetting( void )
    {
    //printMsg("Read sRGB");
    UserPrefContrast = UserPrefContrastSRGB;
    UserPrefBrightness = UserPrefBrightnessSRGB;
    //printData("srgb CONTRAST-->%d",UserPrefContrast);
    //printData("srgb BRIGHTNESS-->%d",UserPrefBrightness);
    UserPrefRedColor = UserPrefRedColorSRGB ;
    UserPrefGreenColor = UserPrefGreenColorSRGB;
    UserPrefBlueColor = UserPrefBlueColorSRGB;
    CheckColorValueRange();
    UserPrefBrightness %= 101;
    }
    void ReadUserSetting( void )
    {
    UserPrefRedColor = UserPrefRedColorUser;
    UserPrefGreenColor = UserPrefGreenColorUser;
    UserPrefBlueColor = UserPrefBlueColorUser;
    UserPrefContrast = UserPrefContrastUser;
    UserPrefBrightness = UserPrefBrightnessUser;
    //printData("user CONTRAST-->%d",UserPrefContrast);
    //printData("user BRIGHTNESS-->%d",UserPrefBrightness);
    CheckColorValueRange();
    UserPrefBrightness %= 101;
    }
     */
    #if ECO
        void SetECO(void)
        {
            //UserPrefColorFlags = 0 ;
	     //SetBFSize(); 
            SetColorTemp();
            if (UserPrefECOMode == ECO_Text)
            {
                UserPrefBrightness = 20;
                UserPrefContrast = DefContrast;//MinContrastValue + (MaxContrastValue - MinContrastValue) *0.8;
                //printMsg("SetECO_Text");
            }
            else if (UserPrefECOMode == ECO_Internet)
            {
                UserPrefBrightness = 40;
                UserPrefContrast = DefContrast;//MinContrastValue + (MaxContrastValue - MinContrastValue) *0.8;
                //printMsg("SetECO_Internest");
            }
            else if (UserPrefECOMode == ECO_Game)
            {
                UserPrefBrightness = 60;
                UserPrefContrast = DefContrast;//MinContrastValue + (MaxContrastValue - MinContrastValue) *0.8;
                //printMsg("SetECO_Game");
            }
            else if (UserPrefECOMode == ECO_Movie)
            {
                UserPrefBrightness = 80;
                UserPrefContrast = DefContrast;//MinContrastValue + (MaxContrastValue - MinContrastValue) *0.8;
                //printMsg("SetECO_Movie");
            }
            else if (UserPrefECOMode == ECO_Sports)
            {
                UserPrefBrightness = 100;
                UserPrefContrast = DefContrast;//MinContrastValue + (MaxContrastValue - MinContrastValue) *0.8;
                //printMsg("SetECO_Sports");
            }
            mStar_AdjustBrightness(UserPrefBrightness);
            mStar_AdjustContrast(UserPrefContrast);
        }
        Bool AdjustECOMode(MenuItemActionType action)
        {
            //if (UserPrefColorTemp == CTEMP_SRGB)
            //{
            //    UserPrefECOMode = ECO_Standard;
            //    return TRUE;
            //}
            #if Enable_Lightsensor
                UserPrefiCareMode = LightSensorOff;
            #endif
                UserPrefDcrMode = 0;
                Clr_GreenFlag();
                Clr_SkinFlag();
                Clr_BlueFlag();
                Clr_ColorAutoDetectFlag();
                Clr_FullEnhanceFlag();
                Clr_DemoFlag(); //eshin 070610 HotEco DemoFlag on but picture off
                Clr_PictureBoostFlag();
                SetBFSize(UserPrefBFSize);//SetBFSize(); // jeff add in 1112 
            //Benz 2007.4.23   15:35:12
            //Benz 2007.4.23   16:40:20 	Osd_Draw4Num(1,1,UserPrefECOMode);
            if (action == MIA_IncValue)
            {
                UserPrefECOMode = (UserPrefECOMode + ECO_Nums - 1) % ECO_Nums; //Benz have some bug issue
            }
            else
            {
                UserPrefECOMode = (UserPrefECOMode + 1) % ECO_Nums;
            }
            if(UserPrefColorTemp==CTEMP_SRGB)
            	{
            		UserPrefColorTemp=CTEMP_Warm1;
                     #if !USEFLASH
               		NVRam_WriteByte(nvrMonitorAddr(ColorTemp), UserPrefColorTemp);
                     #endif
            	}
            //Benz 2007.4.23   16:40:10 	Osd_Draw4Num(2,1,UserPrefECOMode);
           #if !USEFLASH
		NVRam_WriteByte(nvrMonitorAddr(ECOMode), UserPrefECOMode);
   		NVRam_WriteByte(nvrMonitorAddr(DcrMode), UserPrefDcrMode);
		NVRam_WriteByte(nvrMonitorAddr(ColorFlag), UserPrefColorFlags);
                #if Enable_Lightsensor
		NVRam_WriteByte(nvrMonitorAddr(iCareMode), UserPrefiCareMode);
                #endif
           #else
		UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
           #endif
            SetECO();
            return TRUE;
        }
    #endif
    Bool AdjustGamaMode(MenuItemActionType action)
    {
        //if (UserPrefColorTemp==CTEMP_SRGB)  //0617
        //  return FALSE;
        if (action == MIA_DecValue)
        {
            UserPrefGamaMode = (UserPrefGamaMode + GAMA_Nums - 1) % GAMA_Nums;
        }
        else
        {
            UserPrefGamaMode = (UserPrefGamaMode + 1) % GAMA_Nums;
        }
       #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(GamaMode), UserPrefGamaMode);
       #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
       #endif
        // 2006/11/9 11:44PM by Emily     if(!BlueFlag||!SkinFlag||!GreenFlag||!ColorAutoDetectFlag)
        {
            mStar_InitGamma();
            if ((UserPrefColorFlags &(bDemoBit|bPictureBoostBit)) || UserPrefDcrMode)  //yz.nie  100414 for colorboost on,adjust gammamode,have pictureboost 
            {
                /*if (!PictureBoostFlag)
                // if not BF, assign window
                {
                    UserPrefBFHSize = PanelWidth;
                    UserPrefBFVSize = PanelHeight;
                    UserPrefBFHStart = 0;
                    UserPrefBFVStart = 0;
                }
                msAccSetup(UserPrefBFHStart, UserPrefBFHSize, UserPrefBFVStart, UserPrefBFVSize);*/
                #if !ENABLE_HDMI
                msAccOnOff(1);
		 #endif
            }
        }
        return TRUE;
    }
    
Bool AdjustDcrMode(MenuItemActionType action)
{
    #if ENABLE_HDMI
    BYTE XDATA ucDomain;
    #endif

    action = action;

    #if ENABLE_HDMI
    ucDomain = 0x00;
    if(gScInfo.InputColor!=INPUT_RGB)
        ucDomain = 0xFF;
    #endif

    if (UserPrefDcrMode)
    {
        UserPrefDcrMode = 0;
        mStar_WriteByte(0x00, 0x00);
        //mStar_AdjustBrightness(UserPrefBrightness); // 090420 reserved for it is unnecessary
        SetECO();
        #if !ENABLE_HDMI
        // 090420 reserved for it is unnecessary
        //msAdjustSubBrightness(UserPrefSubBrightness);
        //msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
        if (UserPrefColorFlags)
            msAccOnOff(1);
        else
            msAccOnOff(0);
        #endif
        msDlcOnOff(0);
    }
    else
    {
        UserPrefDcrMode = 1;
    #if Enable_Lightsensor
        UserPrefiCareMode = LightSensorOff;
    #endif
        UserPrefECOMode = ECO_Standard;
if(!DcrOpenScikeFlag)//20151225 lixw
        ReadColorTempSetting();
        #if !ENABLE_HDMI
	  #ifndef TSUMXXT
        msAdjustSubBrightness(UserPrefSubBrightness);
        msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
        #endif
	#endif
        Clr_PictureBoostFlag(); // 2006/10/20 6:21PM by Emily for aoc request
        Clr_ColorAutoDetectFlag();
        Clr_FullEnhanceFlag();
        Clr_SkinFlag();
        Clr_BlueFlag();
        Clr_GreenFlag();
        Clr_DemoFlag();
        mStar_WriteByte(0x00, 0x03);
        mStar_WriteByte(0x72, 0x79);//0x80);//0x85
        mStar_WriteByte(0x73, 0x79);//0x80);//0x85
        mStar_WriteByte(0x74, 0x79);//0x80);//0x85
        mStar_WriteByte(0x00, 0x00);
	#ifndef TSUMXXT
        LoadACETable(t_Normal_ColorSettingTable, DefHue, DefSaturation, 0x85); // /UserPrefSubContrast
        #endif
        msAccSetup(0, PanelWidth, 0, PanelHeight);
         
        #if !ENABLE_HDMI
        msAccOnOff(1);
        #endif
        #ifdef TSUMXXT
	 mStar_WriteByte(0x00, 0x03);
        mStar_WriteByte(0x70, 0x00);
        mStar_WriteByte(0x00, 0x00);
        msAdcCscFunctionCtrl(0,SUB_WINDOW);//CSC_SUB_ENABLE(0);  //151121 lixw
        #endif
		
        msDlcInit(PanelWidth, PanelHeight);

#if 1 // 090420 modified for some functions are unnecessary
        LoadDLCTable(t_MWEDLC_Linear_Table);
#else
        msSetDlcStrength(g_LowStrength, g_HighStrength);
        LoadDLCTable(t_MWEDLC_Linear_Table);
        #if !ENABLE_HDMI
        msDlcOnOff(1);
        #endif
#endif        

    }
    #if ENABLE_HDMI
    if (UserPrefDcrMode ||UserPrefColorFlags)
    {
        #if ENABLE_HDMI
        if (ucDomain)
        {
            SetMainWinVideoDomain(ucDomain);		//SetMainWinColorControl(ucDomain);
            mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)|BIT3);
        }
        else
        #endif
            msAccOnOff(_ENABLE);

        if (UserPrefDcrMode)
            msDlcOnOff(_ENABLE);
    }
    else
    {
        #if ENABLE_HDMI
        if (ucDomain)
            mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)&~BIT3);
        else
        #endif
            msAccOnOff(0);

        msDlcOnOff(_DISABLE);

        #if ENABLE_HDMI
        SetMainWinVideoDomain(0);	//SetMainWinColorControl(0);
        if (ucDomain)
            mStar_WriteByte(0x02, mStar_ReadByte(0x02)|BIT3);
        #endif
  }
    #endif
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(DcrMode), UserPrefDcrMode);
	NVRam_WriteByte(nvrMonitorAddr(ECOMode), UserPrefECOMode);
        #if Enable_Lightsensor
        NVRam_WriteByte(nvrMonitorAddr(iCareMode), UserPrefiCareMode);
        #endif
    #else
    	 UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    return TRUE;
}
#if Enable_Lightsensor
Bool AdjustiCareMode(MenuItemActionType action)
{
    #if ENABLE_HDMI
    BYTE XDATA ucDomain;
    #endif

    action = action;

    #if ENABLE_HDMI
    ucDomain = 0x00;
    if(gScInfo.InputColor!=INPUT_RGB)
        ucDomain = 0xFF;
    #endif

    if(UserPrefiCareMode)
    {
        UserPrefiCareMode = LightSensorOff;
        SetECO();
    }
    else
    {
        UserPrefiCareMode = LightSensorOn;

        UserPrefDcrMode = 0;
        UserPrefColorFlags = 0;
        UserPrefColorTemp = CTEMP_Warm1;
        UserPrefECOMode = ECO_Standard;
        
        //UserPrefBrightnessWarm1 = DefBrightness;

        mStar_WaitForDataBlanking();

        SetColorTemp();

        #if Enable_Lightsensor
        // 091002 coding, initial variable of light sensor
        // 091005 coding
        //SetBriValue = TempBrightness = UserPrefBrightness; 
        mStar_AdjustBrightness(TempBrightness);
        #endif
        
        #if !ENABLE_HDMI
        msAdjustSubBrightness(UserPrefSubBrightness);
        msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
        #endif

        #if ENABLE_HDMI
        if (ucDomain)
            mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)&~BIT3);
        else
        #endif
            msAccOnOff(0);

        msDlcOnOff(_DISABLE);

        #if ENABLE_HDMI
        SetMainWinVideoDomain(0);	//SetMainWinColorControl(0);
        if (ucDomain)
            mStar_WriteByte(0x02, mStar_ReadByte(0x02)|BIT3);
        #endif        
    }
    
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(DcrMode), UserPrefDcrMode);
	NVRam_WriteByte(nvrMonitorAddr(ECOMode), UserPrefECOMode);
        #if Enable_Lightsensor
        NVRam_WriteByte(nvrMonitorAddr(iCareMode), UserPrefiCareMode);
        #endif
    #else
    	 UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
    #endif
    
    return TRUE;
}
#endif
    //=====================================================================
    #if 1
        Bool SetFactoryColorTempCool1(void)
        {
            UserPrefColorTemp = CTEMP_Cool1;
            ReadColorTempSetting(); // 2006/11/10 6:58PM by KK ReadCool1Setting();
            mStar_AdjustBrightness(UserPrefBrightness);
            mStar_AdjustContrast(UserPrefContrast);
            return TRUE;
        }
        Bool SetFactoryColorTempWarm1(void)
        {
            UserPrefColorTemp = CTEMP_Warm1;
            ReadColorTempSetting(); // 2006/11/10 6:57PM by KK  ReadWarm1Setting();
            mStar_AdjustBrightness(UserPrefBrightness);
            mStar_AdjustContrast(UserPrefContrast);
            return TRUE;
        }
        Bool SetFactoryColorTempNormal(void)
        {
            UserPrefColorTemp = CTEMP_Normal;
            ReadColorTempSetting(); // 2006/11/10 6:58PM by KK ReadNormalSetting();
            mStar_AdjustBrightness(UserPrefBrightness);
            mStar_AdjustContrast(UserPrefContrast);
            return TRUE;
        }
        Bool SetFactoryColorTempSRGB(void)
        {
            UserPrefColorTemp = CTEMP_SRGB;
            ReadColorTempSetting(); // 2006/11/10 6:58PM by KK ReadSRGBSetting();
            mStar_AdjustBrightness(UserPrefBrightness);
            mStar_AdjustContrast(UserPrefContrast);
            return TRUE;
        }
    #endif
    #if 1
        //====================================================
        Bool AdjustRedColorCool1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefRedColorCool1, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefRedColorCool1)
            {
                return FALSE;
            }
            UserPrefRedColorCool1 = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustRedColor(UserPrefRedColorCool1, UserPrefContrastCool1);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustGreenColorCool1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefGreenColorCool1, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefGreenColorCool1)
            {
                return FALSE;
            }
            UserPrefGreenColorCool1 = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustGreenColor(UserPrefGreenColorCool1, UserPrefContrastCool1);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustBlueColorCool1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefBlueColorCool1, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefBlueColorCool1)
            {
                return FALSE;
            }
            UserPrefBlueColorCool1 = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustBlueColor(UserPrefBlueColorCool1, UserPrefContrastCool1);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustBrightnessCool1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefBrightnessCool1, 0, 100, 1);
            if (tempValue == UserPrefBrightnessCool1)
            {
                return FALSE;
            }
            UserPrefBrightnessCool1 = tempValue;
            UserPrefBrightness = UserPrefBrightnessCool1;
            if (FactoryModeFlag)
                FUserPrefBrightnessCool1 = UserPrefBrightnessCool1;
            mStar_AdjustBrightness(UserPrefBrightnessCool1);
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustContrastCool1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefContrastCool1, MinContrastValue, MaxContrastValue, 1);
            if (tempValue == UserPrefContrastCool1)
            {
                return FALSE;
            }
            UserPrefContrastCool1 = tempValue;
            UserPrefContrast = UserPrefContrastCool1;
            if (FactoryModeFlag)
                FUserPrefContrastCool1 = UserPrefContrastCool1;
            mStar_AdjustContrast(UserPrefContrast);
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        WORD GetRColorCool1Value(void)
        {
            return UserPrefRedColorCool1;
        }
        WORD GetGColorCool1Value(void)
        {
            return UserPrefGreenColorCool1;
        }
        WORD GetBColorCool1Value(void)
        {
            return UserPrefBlueColorCool1;
        }
        WORD GetBrightnessCool1Value(void)
        {
            return FUserPrefBrightnessCool1 % 101;
        }
        WORD GetContrastCool1Value(void) // WMZ 050819
        {
            WORD temp;
            temp = GetScale100Value(FUserPrefContrastCool1, MinContrastValue, MaxContrastValue);
            return temp;
        }
        //====================================================
        Bool AdjustRedColorWarm1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefRedColorWarm1, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefRedColorWarm1)
            {
                return FALSE;
            }
            UserPrefRedColorWarm1 = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustRedColor(UserPrefRedColorWarm1, UserPrefContrastWarm1);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustGreenColorWarm1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefGreenColorWarm1, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefGreenColorWarm1)
            {
                return FALSE;
            }
            UserPrefGreenColorWarm1 = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustGreenColor(UserPrefGreenColorWarm1, UserPrefContrastWarm1);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustBlueColorWarm1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefBlueColorWarm1, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefBlueColorWarm1)
            {
                return FALSE;
            }
            UserPrefBlueColorWarm1 = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustBlueColor(UserPrefBlueColorWarm1, UserPrefContrastWarm1);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustBrightnessWarm1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefBrightnessWarm1, 0, 100, 1);
            if (tempValue == UserPrefBrightnessWarm1)
            {
                return FALSE;
            }
            UserPrefBrightnessWarm1 = tempValue;
            UserPrefBrightness = UserPrefBrightnessWarm1;
            if (FactoryModeFlag)
                FUserPrefBrightnessWarm1 = UserPrefBrightnessWarm1;
            mStar_AdjustBrightness(UserPrefBrightnessWarm1);
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustContrastWarm1(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefContrastWarm1, MinContrastValue, MaxContrastValue, 1);
            if (tempValue == UserPrefContrastWarm1)
            {
                return FALSE;
            }
            UserPrefContrastWarm1 = tempValue;
            UserPrefContrast = UserPrefContrastWarm1;
            if (FactoryModeFlag)
                FUserPrefContrastWarm1 = UserPrefContrastWarm1;
            mStar_AdjustContrast(UserPrefContrast);
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        WORD GetRColorWarm1Value(void)
        {
            return UserPrefRedColorWarm1;
        }
        WORD GetGColorWarm1Value(void)
        {
            return UserPrefGreenColorWarm1;
        }
        WORD GetBColorWarm1Value(void)
        {
            return UserPrefBlueColorWarm1;
        }
        WORD GetBrightnessWarm1Value(void)
        {
            return FUserPrefBrightnessWarm1 % 101;
        }
        WORD GetContrastWarm1Value(void) // WMZ 050819
        {
            WORD temp;
            temp = GetScale100Value(FUserPrefContrastWarm1, MinContrastValue, MaxContrastValue);
            return temp;
        }
        //===============================================================
        Bool AdjustRedColorNormal(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefRedColorNormal, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefRedColorNormal)
            {
                return FALSE;
            }
            UserPrefRedColorNormal = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustRedColor(UserPrefRedColorNormal, UserPrefContrastNormal);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustGreenColorNormal(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefGreenColorNormal, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefGreenColorNormal)
            {
                return FALSE;
            }
            UserPrefGreenColorNormal = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustGreenColor(UserPrefGreenColorNormal, UserPrefContrastNormal);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustBlueColorNormal(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefBlueColorNormal, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefBlueColorNormal)
            {
                return FALSE;
            }
            UserPrefBlueColorNormal = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustBlueColor(UserPrefBlueColorNormal, UserPrefContrastNormal);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustBrightnessNormal(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefBrightnessNormal, 0, 100, 1);
            if (tempValue == UserPrefBrightnessNormal)
            {
                return FALSE;
            }
            UserPrefBrightnessNormal = tempValue;
            UserPrefBrightness = UserPrefBrightnessNormal;
            if (FactoryModeFlag)
                FUserPrefBrightnessNormal = UserPrefBrightnessNormal;
            mStar_AdjustBrightness(UserPrefBrightnessNormal);
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustContrastNormal(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefContrastNormal, MinContrastValue, MaxContrastValue, 1);
            if (tempValue == UserPrefContrastNormal)
            {
                return FALSE;
            }
            UserPrefContrastNormal = tempValue;
            UserPrefContrast = UserPrefContrastNormal;
            if (FactoryModeFlag)
                FUserPrefContrastNormal = UserPrefContrastNormal;
            mStar_AdjustContrast(UserPrefContrast);
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        WORD GetRColorNormalValue(void)
        {
            return UserPrefRedColorNormal;
        }
        WORD GetGColorNormalValue(void)
        {
            return UserPrefGreenColorNormal;
        }
        WORD GetBColorNormalValue(void)
        {
            return UserPrefBlueColorNormal;
        }
        WORD GetBrightnessNormalValue(void)
        {
            return FUserPrefBrightnessNormal % 101;
        }
        WORD GetContrastNormalValue(void) // WMZ 050819
        {
            WORD temp;
            temp = GetScale100Value(FUserPrefContrastNormal, MinContrastValue, MaxContrastValue);
            return temp;
        }
        //====================================================
        Bool AdjustRedColorSRGB(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefRedColorSRGB, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefRedColorSRGB)
            {
                return FALSE;
            }
            UserPrefRedColorSRGB = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustRedColor(UserPrefRedColorSRGB, UserPrefContrastSRGB);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustGreenColorSRGB(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefGreenColorSRGB, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefGreenColorSRGB)
            {
                return FALSE;
            }
            UserPrefGreenColorSRGB = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustGreenColor(UserPrefGreenColorSRGB, UserPrefContrastSRGB);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustBlueColorSRGB(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefBlueColorSRGB, MinColorValue, MaxColorValue, 1);
            if (tempValue == UserPrefBlueColorSRGB)
            {
                return FALSE;
            }
            UserPrefBlueColorSRGB = tempValue;
            #if UsesRGB
                mStar_AdjustContrast(UserPrefContrast);
            #else
                mStar_AdjustBlueColor(UserPrefBlueColorSRGB, UserPrefContrastSRGB);
            #endif
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustBrightnessSRGB(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefBrightnessSRGB, 0, 100, 1);
            if (tempValue == UserPrefBrightnessSRGB)
            {
                return FALSE;
            }
            UserPrefBrightnessSRGB = tempValue;
            UserPrefBrightness = UserPrefBrightnessSRGB;
            mStar_AdjustBrightness(UserPrefBrightnessSRGB);
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        Bool AdjustContrastSRGB(MenuItemActionType action)
        {
            WORD tempValue;
            tempValue = DecIncValue(action, UserPrefContrastSRGB, MinContrastValue, MaxContrastValue, 1);
            if (tempValue == UserPrefContrastSRGB)
            {
                return FALSE;
            }
            UserPrefContrastSRGB = tempValue;
            UserPrefContrast = UserPrefContrastSRGB;
            mStar_AdjustContrast(UserPrefContrast);
	#if USEFLASH
            SaveFactorySetting();//UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
	#else
            SaveMonitorSetting();
	#endif
            return TRUE;
        }
        WORD GetRColorSRGBValue(void)
        {
            return UserPrefRedColorSRGB;
        }
        WORD GetGColorSRGBValue(void)
        {
            return UserPrefGreenColorSRGB;
        }
        WORD GetBColorSRGBValue(void)
        {
            return UserPrefBlueColorSRGB;
        }
        WORD GetBrightnessSRGBValue(void)
        {
            return UserPrefBrightnessSRGB % 101;
        }
        WORD GetContrastSRGBValue(void) // WMZ 050819
        {
            WORD temp;
            temp = GetScale100Value(UserPrefContrastSRGB, MinContrastValue, MaxContrastValue);
            return temp;
        }
    #endif
    //======================================================================================
    Bool EnableBurninMode(void)
    {
        PatternNo = 0;
        mStar_WriteByte(FWC, FWC_B);
        return TRUE;
    }
    #if 1
        WORD GetBurninValue(void)
        {
            return (BurninModeFlag) ? (1): (0);
        }
        Bool AdjustBurnin(MenuItemActionType action)
        {
            action = action;
            if (BurninModeFlag)
            {
                Clr_BurninModeFlag();
                Clr_DoBurninModeFlag();
            }
            else
            {
                Set_BurninModeFlag();
                Set_DoBurninModeFlag();
            }
            Delay1ms(200);
            return TRUE;
        }
    #endif
    #if Enable_ProductModeAdjust
        WORD GetProductModeValue(void)
        {
            return (ProductModeFlag) ? (1): (0);
        }
        Bool AdjustProductMode(MenuItemActionType action)
        {
            action = action;
            if(UserPrefBrightness==0&&UserPrefContrast==MinContrastValue)
            	{
	            if (ProductModeFlag)
	            {
	                	#if USEFLASH
				FactoryProductModeValue=ProductModeOffValue;
				UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit);
				#else
				//WORD temp=ProductModeOffValue;
				//NVRam_WriteTbl(ProductModeAddr, (BYTE*)(&temp), 2);
				NVRam_WriteWord(ProductModeAddr, ProductModeOffValue);
				#endif
	                     Clr_ProductModeFlag();
				Set_AmberLedFlash(5);
	            }
	            else
	            {
	                	#if USEFLASH
				FactoryProductModeValue=ProductModeOnValue;
				UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit);
				#else
				//WORD temp=ProductModeOnValue;
				//NVRam_WriteTbl(ProductModeAddr, (BYTE*)(&temp), 2);
				NVRam_WriteWord(ProductModeAddr, ProductModeOnValue);
				#endif
	                     Set_ProductModeFlag();
				Set_GreenLedFlash(5);
	            }
            	}
            return TRUE;
        }
    #endif
    //====================================================
    /*
    Bool AdjustBankNo(MenuItemActionType action)
    {
        if (action == MIA_IncValue)
        {
            MSBankNo = (MSBankNo + 1) % 3;
        }
        else
        {
            MSBankNo = (MSBankNo + 2) % 3;
        }
        return TRUE;
    }
    Bool AdjustAddr(MenuItemActionType action)
    {
        if (action == MIA_IncValue)
        {
            MSAddr = (MSAddr + 1) % 0x100;
        }
        else
        {
            MSAddr = (MSAddr + 255) % 0x100;
        }
        return TRUE;
    }
    WORD GetAddrValue(void)
    {
        return MSAddr;
    }
    Bool AdjustRegValue(MenuItemActionType action)
    {
        if (action == MIA_IncValue)
        {
            MSValue = (MSValue + 1) % 0x100;
        }
        else
        {
            MSValue = (MSValue + 255) % 0x100;
        }
	#if AllInOneKey
		WriteRegValue();
	#endif
        return TRUE;
    }
    WORD GetAddrRegValue(void)
    {
        if (MSBankNo == 0)
        // scaler
        {
            MSValue = mStar_ReadByte(MSAddr);
        }
        else if (MSBankNo == 1)
        // adc
        {
            mStar_WriteByte(REGBK, REGBANKADC);
            MSValue = mStar_ReadByte(MSAddr);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        else if (MSBankNo == 2)
        // TCON
        {
            mStar_WriteByte(REGBK, REGBANKTCON);
            MSValue = mStar_ReadByte(MSAddr);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        return MSValue;
    }
    WORD GetRegValue(void)
    {
        return MSValue;
    }
    Bool WriteRegValue(void)
    {
        if (MSBankNo == 0)
        // scaler
        {
            mStar_WriteByte(MSAddr, MSValue);
        }
        else if (MSBankNo == 1)
        // adc
        {
            mStar_WriteByte(REGBK, REGBANKADC);
            mStar_WriteByte(MSAddr, MSValue);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        else if (MSBankNo == 2)
        // TCON
        {
            mStar_WriteByte(REGBK, REGBANKTCON);
            mStar_WriteByte(MSAddr, MSValue);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        return TRUE;
    }
	//*/
#if 0// EnablePanelServiceMenu
extern void Init_PanelSetting(void);
extern void SavePanelSetting(void);

    Bool AdjustPanelTpye(MenuItemActionType action)
    {
        action=action;
        PanelSelect= (PanelSelect+ 1) % 2;
    #if PanelSettingInFlash
	 if(PanelSelect==1)
 	{
		//Flash_ReadPanelSet(0, (BYTE*)&PanelSetting, PanelSettingSize);
		Flash_Read_Factory_KeySet(FLASH_KEY_PANEL, TRUE, 0, PANEL_SIZE, (BYTE*)&PanelSetting, PANEL_SIZE);	//110311 Modify
		{
		        //if(PanelSetting.PanelTypeNum!=UseUserPanelValue||PanelSetting.CheckSum!=FLASH_CHECKSUM||PanelSetting.Ident!=FLASH_IDENTDATA)
		        if(PanelSetting.PanelTypeNum!=UseUserPanelValue||(CalculateSettingCheckSum((BYTE *)(&PanelSetting)+PanelSettingChecksumSize, PanelSettingSize-PanelSettingChecksumSize)!=PanelSetting.CheckSum))
			{
				Init_PanelSetting();
			}
		}
		PanelSetting.PanelTypeNum=UseUserPanelValue;
 	}
	 else
 	{
		PanelSetting.PanelTypeNum=UseDefaultPanelValue;
 	}
	SavePanelSetting();
    #else
	 if(PanelSelect==1)
 	{
		NVRam_WriteWord(PanelTypeNumAddr, UseUserPanelValue);
		{
		        WORD temp;
		        NVRam_ReadTbl(PanelTypeNumAddr, (BYTE*)(&temp), 2);
		        if(temp!=UseUserPanelValue)
			{
				Init_PanelSetting();
				SavePanelSetting();
			}
		}
 	}
	 else
 	{
		NVRam_WriteWord(PanelTypeNumAddr, UseDefaultPanelValue);
 	}
    #endif
        return TRUE;
    }
#endif	
    Bool EnterDisplayLogo(void)
    {
        OsdCounter = 3;
        mStar_SetupFreeRunMode();
        #if((PanelType == PanelB156XTN02)||(PanelType == PanelB156XTN07)||(PanelType == PanelNT156WHM_N12)||(PanelType == PanelM185WHB_N10)||(PanelType == PanelLM185TT3A)/*||(PanelType == PanelM185BGE_L23)*/)
        Osd_Hide();
        Power_TurnOnPanel();   
        Osd_Show();
        #else
        Power_TurnOnPanel();
        #endif
        //printMsg("turn on");
        return TRUE;
    }
    Bool OsdCountDown(void)
    {
        OsdCounter = UserPrefOsdTime; //3;
        return TRUE;
    }


#endif
#if DDCCI_ENABLE
    #if 0
        void AdjustTempColorTemp(BYTE ctemp)
        {
            mStar_AdjustBrightness(*(ColorTempPtr[ctemp][0])); //wei1010
            mStar_AdjustRedColor(*(ColorTempPtr[ctemp][1]), *(ColorTempPtr[ctemp][2]));
            mStar_AdjustGreenColor(*(ColorTempPtr[ctemp][1]), *(ColorTempPtr[ctemp][3]));
            mStar_AdjustBlueColor(*(ColorTempPtr[ctemp][1]), *(ColorTempPtr[ctemp][4]));
        }
        Bool ResetGeometry(void)
        {
            //Reset HStart
            UserPrefHStart = StandardModeHStart;
            mStar_AdjustHPosition(UserPrefHStart);
            NVRam_WriteByte(nvrModeAddr(HStart), UserPrefHStart);
            //Reset VStart
            UserPrefVStart = StandardModeVStart;
            mStar_AdjustVPosition(UserPrefVStart);
            NVRam_WriteByte(nvrModeAddr(VStart), UserPrefVStart);
            //Reset HTotal
            UserPrefHTotal = StandardModeHTotal;
            mStar_AdjustHTotal(UserPrefHTotal);
            NVRam_WriteByte(nvrModeAddr(HTotal), UserPrefHTotal);
            //Reset Phase
            UserPrefPhase = 0; // Bruce 1211.2003
            mStar_AdjustPhase(UserPrefPhase);
            NVRam_WriteByte(nvrModeAddr(Phase), UserPrefPhase);
            Clr_SaveSettingFlag();
            return TRUE;
        }
        Bool ResetColorTemp(void)
        {
            /*
            UserPrefRedColor9300K=Def9300KRedColor;
            UserPrefGreenColor9300K=Def9300KGreenColor;
            UserPrefBlueColor9300K=Def9300KBlueColor;
            UserPrefRedColor7200K=Def7200KRedColor;
            UserPrefGreenColor7200K=Def7200KGreenColor;
            UserPrefBlueColor7200K=Def7200KBlueColor;
            UserPrefRedColor6500K=Def6500KRedColor;
            UserPrefGreenColor6500K=Def6500KGreenColor;
            UserPrefBlueColor6500K=Def6500KBlueColor;
            NVRam_WriteByte(nvrMonitorAddr(RedColor9300K), UserPrefRedColor9300K);
            NVRam_WriteByte(nvrMonitorAddr(GreenColor9300K), UserPrefGreenColor9300K);
            NVRam_WriteByte(nvrMonitorAddr(BlueColor9300K), UserPrefBlueColor9300K);
            NVRam_WriteByte(nvrMonitorAddr(RedColor7200K), UserPrefRedColor7200K);
            NVRam_WriteByte(nvrMonitorAddr(GreenColor7200K), UserPrefGreenColor7200K);
            NVRam_WriteByte(nvrMonitorAddr(BlueColor7200K), UserPrefBlueColor7200K);
            NVRam_WriteByte(nvrMonitorAddr(RedColor6500K), UserPrefRedColor6500K);
            NVRam_WriteByte(nvrMonitorAddr(GreenColor6500K), UserPrefGreenColor6500K);
            NVRam_WriteByte(nvrMonitorAddr(BlueColor6500K), UserPrefBlueColor6500K);
             */
            UserPrefRedColor = DefRedColor;
            UserPrefGreenColor = DefGreenColor;
            UserPrefBlueColor = DefBlueColor;
            NVRam_WriteByte(nvrMonitorAddr(RedColor), UserPrefRedColor);
            NVRam_WriteByte(nvrMonitorAddr(GreenColor), UserPrefGreenColor);
            NVRam_WriteByte(nvrMonitorAddr(BlueColor), UserPrefBlueColor);
            UserPrefColorTemp = CTEMP_Cool1;
            NVRam_WriteByte(nvrMonitorAddr(ColorTemp), UserPrefColorTemp);
            AdjustTempColorTemp(UserPrefColorTemp);
            return TRUE;
        }
    #endif
#endif
#if 0//AudioFunc
    Bool SetMute(void)
    {
        hw_SetMute();
        //hw_SetStandby();
        return TRUE;
    }
#endif
/*
WORD GetPanelOnTimeValue(void)
{
    WORD hour;
    if (BlacklitTime > 235926000ul)
    // 655365*3600
        hour = 0xFFFF;
    else
        hour = (DWORD)BlacklitTime / 3600;
    return hour;
}
//*/
#if Enable_Expansion
    void SetExpansionMode(void)
    {
    #if 0
	        mStar_SetScalingFactor(UserprefExpansionMode);
    #else
		#ifndef TSUMXXT
        bit bOsdshow;
        bOsdshow = (mStar_ReadOSDByte(IOSDC1) &0x01);
		#endif
	#if WaitForDataBlanking  //zjb20071219
       mStar_WaitForDataBlanking();
       #endif
        mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) | BLKS_B);
       #if WaitForDataBlanking  //zjb20071219
       mStar_WaitForDataBlanking();
       #endif
		#ifndef TSUMXXT
        Osd_Hide();
		#endif
        //mStar_WaitForDataBlanking();
        ForceDelay1ms(20);
        mStar_SetScalingFactor(UserprefExpansionMode);
        //mStar_WaitForDataBlanking();
        ForceDelay1ms(20);
		#ifndef TSUMXXT
        if (bOsdshow)
            Osd_Show();
		#endif
	#if WaitForDataBlanking  //zjb20071219
       mStar_WaitForDataBlanking();
       #endif
       mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) &(~BLKS_B));
       #if WaitForDataBlanking  //zjb20071219
       mStar_WaitForDataBlanking();
       #endif
    #endif
    }
    Bool AdjustExpansionMode(void)
    {
        if (!ExpansionFlag)
		return FALSE;
        //if (ExpansionFlag)
        {
            UserprefExpansionMode++;
            UserprefExpansionMode = (UserprefExpansionMode) % Expansion_Nums;
            SetExpansionMode();
        }
		#if Enable_Expansion&&MWEFunction	//120911 Modify
		if( PictureBoostFlag )
		{
			SetBFSize(UserPrefBFSize);
		}
		#endif
        #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(ExpansionMode), UserprefExpansionMode);
        #else
	UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
        #endif
        return TRUE;
    }
#endif

Bool DoAutoConfig(void)
{
	if(DoAutoConfigFlag)
	     AutoConfig();
    Clr_DoAutoConfigFlag();

    return TRUE;
}

Bool AdjustAutoConfig(void)
{
    if(DoAutoConfigFlag)
        Clr_DoAutoConfigFlag();
    else
	    Set_DoAutoConfigFlag();
    return TRUE;
}

void SetDefaultSetting(BYTE ucDef)
{
	if(ucDef)
	{
       	if(SrcInputType<Input_Digital)// image setting by default
      		{
        		if(UserPrefAutoTimes==0)
        		{
        			mStar_AdjustHPosition(StandardModeHStart);
        			mStar_AdjustVPosition(StandardModeVStart);
        		}
			else
			{
        			mStar_AdjustHPosition(UserPrefAutoHStart);
        			mStar_AdjustVPosition(UserPrefAutoVStart);
			}
	 			#ifdef TSUMXXT		//110311 Modify
				msADC_AdjustHTotal(StandardModeHTotal);
				msADC_SetPhaseCode(0);
				#else
	 			mStar_AdjustHTotal(StandardModeHTotal);
        		mStar_AdjustPhase(0);
				#endif
     		}		
		
		mStar_AdjustBrightness(DefBrightness);
		mStar_AdjustContrast(DefContrast);
		mStar_WriteByte(0x00, 0x06);
		mStar_WriteByte(0x30, 0x00);
		mStar_WriteByte(0x00, 0x00);
		mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)&~BIT3);//disable all sub-win setting
		#if AudioFunc
		mStar_AdjustVolume(50);
		#endif
    #if Enable_Expansion
	if(UserprefExpansionMode!=DefExpansion)
    	{
		mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) | BLKS_B);
		Delay1ms(20);
		mStar_SetScalingFactor(DefExpansion);
		Delay1ms(20);
		mStar_WriteByte(OCTRL2, mStar_ReadByte(OCTRL2) &(~BLKS_B));
		//SetExpansionMode();//This Function will mute when reset all setting.
		// mStar_SetScalingFactor(DefExpansion);
    	}
    #endif

    Osd_SetTransparence(0);
    #if OSD_PIVOT_EN  //071226
    if(OSDRotateFlag)
    Osd_SetPosition(50, 50);			
    else
    #endif		
    Osd_SetPosition(50, 50);
#if Enable_OsdEngine1
    Osd_SetOsdEngine(1);
    #if OSD_PIVOT_EN  //071226
    if(OSDRotateFlag)
    Osd_SetPosition(50, 50);		
    else
    #endif	
    Osd_SetPosition(50, 50);
    Osd_SetOsdEngine(0);
#endif
if (UserPrefGamaMode!=GAMA1)
	mStar_SetupGamma(GAMA1);
		 #ifdef EnableLightBar
	 ScanKey_PWMCtrl(DefLightBarValue);
		#endif

	}
	else
	{
     		if(SrcInputType<Input_Digital) // image setting by user
     		{
        		mStar_AdjustHPosition(UserPrefHStart);
        		mStar_AdjustVPosition(UserPrefVStart);
				#ifdef TSUMXXT		//110311 Modify
				msADC_AdjustHTotal(UserPrefHTotal);
				msADC_SetPhaseCode(UserPrefPhase);
				#else
        		mStar_AdjustHTotal(UserPrefHTotal);
        		mStar_AdjustPhase(UserPrefPhase);
				#endif
     		}			
    #if AudioFunc
    mStar_AdjustVolume(UserPrefVolume);
    #endif
    #if Enable_Expansion
        //SetExpansionMode();//This Function will mute when reset all setting.
        mStar_SetScalingFactor(UserprefExpansionMode);
    #endif
//		mStar_SetUserPref();
	mStar_AdjustBrightness( UserPrefBrightness );
	mStar_AdjustContrast(UserPrefContrast);
	if( UserPrefColorFlags )
	{
		mStar_WriteByte(0x00, 0x06);
		if(DemoFlag)
			mStar_WriteByte(0x30,0x07);
		else
			mStar_WriteByte(0x30,0x70);
		mStar_WriteByte(0x00, 0x00);
	}
    if (UserPrefDcrMode ||DemoFlag ||PictureBoostFlag)
    	{
	mStar_WriteByte(0x5C, mStar_ReadByte(0x5C) | BIT3);
    	}
    Osd_SetTransparence(UserPrefOsdTrans);
    #if OSD_PIVOT_EN  //071226
    if(OSDRotateFlag)
    Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);			
    else
    #endif		
    Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
    #if Enable_OsdEngine1
    Osd_SetOsdEngine(1);
    #if OSD_PIVOT_EN  //071226
    if(OSDRotateFlag)
    Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);			
    else
    #endif	
    Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);
    Osd_SetOsdEngine(0);
#endif		
if (UserPrefGamaMode!=GAMA1)
	mStar_SetupGamma(UserPrefGamaMode);
	 #if Enable_Lightsensor // 100414 coding addition for that timing change caused brightness change.
     if(UserPrefiCareMode == LightSensorOn)
        mStar_AdjustBrightness(TempBrightness);
    #endif        
    
	    #ifdef EnableLightBar
      ScanKey_PWMCtrl(UserPrefLightBar);
		#endif

	}
}

Bool AdjustResetSwitch(void)
{
    if(DoResetFlag)
{
        Clr_DoResetFlag();
	 SetDefaultSetting(0);//user	
}
    else
{
        Set_DoResetFlag();
	 SetDefaultSetting(1);//default

	   
}	
	

  	
    return TRUE;
}

Bool DoResetConfig(void)
{
	if(DoResetFlag)
     	ResetAllSetting();
    return TRUE;
}

#if MS_OVD
        WORD GetOverDriveValue(void)
        {
            return (OverDriveOnFlag) ? (1): (0);
        }
	Bool AdjustOverDrive(MenuItemActionType action)
	{
		action = action;
		if (OverDriveOnFlag)
		{
			Clr_OverDriveOnFlag();
			msOD_OnOff(0,0x50);
		}
		else
		{
			Set_OverDriveOnFlag();
			msOD_OnOff(1,0x5f);		
		}
		Delay1ms(200);
		return TRUE;
	}
#endif

#if PANELSSCSTEPLFUN
       Bool AdjustSSCSTEPL(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelSSCSTEPL, 0, 0xFF, 1);
                if (tempValue == PanelSSCSTEPL)
                {
                    return FALSE;
                }
                PanelSSCSTEPL = tempValue;
                mStar_WriteByte(LPLL_STEP_L, PanelSSCSTEPL);
                return TRUE;
        }
        WORD GetSSCSTEPL(void)
        {
            return PanelSSCSTEPL;
        }
        Bool AdjustSSCSTEPH(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelSSCSTEPH, 0, 7, 1);
                if (tempValue == PanelSSCSTEPH)
                {
                    return FALSE;
                }
                PanelSSCSTEPH = tempValue;
                mStar_WriteByte(LPLL_STEP_H, PanelSSCSTEPH);
                return TRUE;
        }
        WORD GetSSCSTEPH(void)
        {
            return PanelSSCSTEPH;
        }
        Bool AdjustSSCSPANL(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelSSCSPANL, 0, 0xFF, 1);
                if (tempValue == PanelSSCSPANL)
                {
                    return FALSE;
                }
                PanelSSCSPANL = tempValue;
                mStar_WriteByte(LPLL_SPAN_L, PanelSSCSPANL);
                return TRUE;
        }
        WORD GetSSCSPANL(void)
        {
            return PanelSSCSPANL;
        }
        Bool AdjustSSCSPANH(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelSSCSPANH, 0, 7, 1);
                if (tempValue == PanelSSCSPANH)
                {
                    return FALSE;
                }
                PanelSSCSPANH = tempValue;
                mStar_WriteByte(LPLL_SPAN_H, PanelSSCSPANH);
                return TRUE;
        }
        WORD GetSSCSPANH(void)
        {
            return PanelSSCSPANH;
        }
#endif



#if 0//EnablePanelServiceMenu
    Bool AdjustPanelDither(MenuItemActionType action)
    {
    action=action;
    if(PanelSettingFlag&bPanelColorDepth)
    	{
		mStar_WriteByte(DITHCTRL, DITH_B);
		PanelSettingFlag&=~bPanelColorDepth;
    	}
    else
    	{
		mStar_WriteByte(DITHCTRL, 0x00);
		PanelSettingFlag|=bPanelColorDepth;
    	}
        return TRUE;
    }
/*    WORD GetPanelDither(void)
    {
        return PanelSettingFlag&bPanelDither;
    }*/
    Bool AdjustPanelOnTiming1(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelOnTiming1, 0, MaxPanelOnTiming1, 1);
            if (tempValue == PanelOnTiming1)
            {
                return FALSE;
            }
            PanelOnTiming1 = tempValue;
            return TRUE;
    }
    WORD GetPanelOnTiming1(void)
    {
        return PanelOnTiming1;
    }

    Bool AdjustPanelOnTiming2(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelOnTiming2, 0, MaxPanelOnTiming2, 1);
            if (tempValue == PanelOnTiming2)
            {
                return FALSE;
            }
            PanelOnTiming2 = tempValue;
            return TRUE;
    }
    WORD GetPanelOnTiming2(void)
    {
        return PanelOnTiming2;
    }
	
    Bool AdjustPanelOffTiming1(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelOffTiming1, 0, MaxPanelOffTiming1, 1);
            if (tempValue == PanelOffTiming1)
            {
                return FALSE;
            }
            PanelOffTiming1 = tempValue;
            return TRUE;
    }
    WORD GetPanelOffTiming1(void)
    {
        return PanelOffTiming1;
    }
	
    Bool AdjustPanelOffTiming2(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelOffTiming2, 0, MaxPanelOffTiming2, 1);
            if (tempValue == PanelOffTiming2)
            {
                return FALSE;
            }
            PanelOffTiming2 = tempValue;
            return TRUE;
    }
    WORD GetPanelOffTiming2(void)
    {
        return PanelOffTiming2;
    }

    Bool AdjustPanelHSyncWidth(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelHSyncWidth, 0, MaxPanelHSyncWidth, 1);
            if (tempValue == PanelHSyncWidth)
            {
                return FALSE;
            }
            PanelHSyncWidth = tempValue;
            return TRUE;
    }
    WORD GetPanelHSyncWidth(void)
    {
        return PanelHSyncWidth;
    }
	
    Bool AdjustPanelHSyncBackPorch(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelHSyncBackPorch, 0, MaxPanelHSyncBackPorch, 1);
            if (tempValue == PanelHSyncBackPorch)
            {
                return FALSE;
            }
            PanelHSyncBackPorch = tempValue;
            return TRUE;
    }
    WORD GetPanelHSyncBackPorch(void)
    {
        return PanelHSyncBackPorch;
    }	
    Bool AdjustPanelVSyncWidth(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelVSyncWidth, 0, MaxPanelVSyncWidth, 1);
            if (tempValue == PanelVSyncWidth)
            {
                return FALSE;
            }
            PanelVSyncWidth = tempValue;
            return TRUE;
    }
    WORD GetPanelVSyncWidth(void)
    {
        return PanelVSyncWidth;
    }	
    Bool AdjustPanelVSyncBackPorch(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelVSyncBackPorch, 0, MaxPanelVSyncBackPorch, 1);
            if (tempValue == PanelVSyncBackPorch)
            {
                return FALSE;
            }
            PanelVSyncBackPorch = tempValue;
            return TRUE;
    }
    WORD GetPanelVSyncBackPorch(void)
    {
        return PanelVSyncBackPorch;
    }	
   /* Bool AdjustPanelWidth(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelWidth, _PanelWidth-50, _PanelWidth+50, 1);
            if (tempValue == PanelWidth)
            {
                return FALSE;
            }
            PanelWidth = tempValue;
            return TRUE;
    }*/
    WORD GetPanelWidth(void)
    {
        return PanelWidth;
    }	
    /*Bool AdjustPanelHeight(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelHeight, _PanelHeight-50, _PanelHeight+50, 1);
            if (tempValue == PanelHeight)
            {
                return FALSE;
            }
            PanelHeight = tempValue;
            return TRUE;
    }*/
    WORD GetPanelHeight(void)
    {
        return PanelHeight;
    }	
	
    Bool AdjustPanelHTotal(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelHTotal, _PanelHTotal-300, _PanelHTotal+300, 1);
            if (tempValue == PanelHTotal)
            {
                return FALSE;
            }
            PanelHTotal = tempValue;
            return TRUE;
    }
    WORD GetPanelHTotal(void)
    {
        return PanelHTotal;
    }	
    Bool AdjustPanelVTotal(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelVTotal, _PanelVTotal-300, _PanelVTotal+300, 1);
            if (tempValue == PanelVTotal)
            {
                return FALSE;
            }
            PanelVTotal = tempValue;
            return TRUE;
    }
    WORD GetPanelVTotal(void)
    {
        return PanelVTotal;
    }	
	
    Bool AdjustPanelMaxHTotal(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelMaxHTotal, _PanelMaxHTotal-300, _PanelMaxHTotal+300, 1);
            if (tempValue == PanelMaxHTotal)
            {
                return FALSE;
            }
            PanelMaxHTotal = tempValue;
            return TRUE;
    }
    WORD GetPanelMaxHTotal(void)
    {
        return PanelMaxHTotal;
    }	
    Bool AdjustPanelMinHTotal(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelMinHTotal, _PanelMinHTotal-300, _PanelMinHTotal+300, 1);
            if (tempValue == PanelMinHTotal)
            {
                return FALSE;
            }
            PanelMinHTotal = tempValue;
            return TRUE;
    }
    WORD GetPanelMinHTotal(void)
    {
        return PanelMinHTotal;
    }	

    /*Bool AdjustPanelMaxVTotal(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelMaxVTotal, _PanelMaxVTotal-300, _PanelMaxVTotal+300, 1);
            if (tempValue == PanelMaxVTotal)
            {
                return FALSE;
            }
            PanelMaxVTotal = tempValue;
            return TRUE;
    }
    WORD GetPanelMaxVTotal(void)
    {
        return PanelMaxVTotal;
    }	
    Bool AdjustPanelMinVTotal(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelMinVTotal, _PanelMinVTotal-300, _PanelMinVTotal+300, 1);
            if (tempValue == PanelMinVTotal)
            {
                return FALSE;
            }
            PanelMinVTotal = tempValue;
            return TRUE;
    }
    WORD GetPanelMinVTotal(void)
    {
        return PanelMinVTotal;
    }*/	
	
    Bool AdjustPanelDCLK(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelDCLK, _PanelDCLK-50, _PanelDCLK+50, 1);
            if (tempValue == PanelDCLK)
            {
                return FALSE;
            }
            PanelDCLK = tempValue;
            return TRUE;
    }
    WORD GetPanelDCLK(void)
    {
        return PanelDCLK;
    }	

    Bool AdjustPanelMaxDCLK(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelMaxDCLK, _PanelMaxDCLK-50, _PanelMaxDCLK+50, 1);
            if (tempValue == PanelMaxDCLK)
            {
                return FALSE;
            }
            PanelMaxDCLK = tempValue;
            return TRUE;
    }
    WORD GetPanelMaxDCLK(void)
    {
        return PanelMaxDCLK;
    }	
    Bool AdjustPanelMinDCLK(MenuItemActionType action)
    {
            WORD tempValue;
            tempValue = DecIncValue(action, PanelMinDCLK, _PanelMinDCLK-50, _PanelMinDCLK+50, 1);
            if (tempValue == PanelMinDCLK)
            {
                return FALSE;
            }
            PanelMinDCLK = tempValue;
            return TRUE;
    }
    WORD GetPanelMinDCLK(void)
    {
        return PanelMinDCLK;
    }	
/*
        Bool AdjustABPortSwap(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl2&BIT1)
			PanelOutputControl2&=(~BIT1);
		else
		            PanelOutputControl2 |= BIT1;
		mStar_WriteByte(OCTRL2,PanelOutputControl2);
            return TRUE;
        }
        Bool AdjustDualPixelOutput(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl2&BIT0)
			PanelOutputControl2&=(~BIT0);
		else
		            PanelOutputControl2|= BIT0;
		mStar_WriteByte(OCTRL2,PanelOutputControl2);
            return TRUE;
        }
*/
     Bool AdjustSwing(MenuItemActionType action)
        {
                WORD tempValue;
                BYTE tempSwingValue;
		if(ChipVer == TSUM_N || ChipVer == TSUM_Q)
                {
                    tempSwingValue=(PanelLVDSSwing&0xC0)>>6;
                    tempValue = DecIncValue(action, tempSwingValue, MinSwingAutoTuneValue, MaxSwingAutoTuneValue, 1);
                    if (tempValue == tempSwingValue)
                    {
                        return FALSE;
                    }
                    PanelLVDSSwing= (PanelLVDSSwing&0x1F)|(tempValue<<6);
					
                }
                else
                {
                    tempSwingValue=PanelLVDSSwing&0x1F;
                    tempValue = DecIncValue(action, tempSwingValue, MinSwingValue, MaxSwingValue, 1);
                    if (tempValue == tempSwingValue)
                    {
                        return FALSE;
                    }
                    PanelLVDSSwing= (PanelLVDSSwing&0xC0)|tempValue;
                    mStar_WriteByte(REGBK, REGBANKADC); // switch to ADC bank
                    mStar_WriteByte(TESTMOD, PanelLVDSSwing&0x1F);
                    mStar_WriteByte(REGBK, REGBANKSCALER); // switch to Scaler bank
                }
		
                return TRUE;
        }
        WORD GetSwingValue(void)
        {
            WORD tempValue;
           if(ChipVer == TSUM_N || ChipVer == TSUM_Q)
            {
                BYTE temp=(PanelLVDSSwing&0xC0)>>6;
                if(temp==0)
                    tempValue=242;
                else if(temp==1)
                    tempValue=339;
                else if(temp==2)
                    tempValue=387;
                else if(temp==3)
                    tempValue=170;
            }
            else
            {
                if((PanelLVDSSwing&0x1F)>=0x10)
                    tempValue=200+(((PanelLVDSSwing&0x1F)-0x10)*100+3)/6;
                else
                    tempValue=(((PanelLVDSSwing&0x1F)-0x00+4)*100+3)/6;
            }
	   return tempValue;
        }

        Bool AdjustABPortSwap(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl2&BIT1)
			PanelOutputControl2&=(~BIT1);
		else
		            PanelOutputControl2 |= BIT1;
		mStar_WriteByte(OCTRL2,PanelOutputControl2);
            return TRUE;
        }
        Bool AdjustDualPixelOutput(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl2&BIT0)
			PanelOutputControl2&=(~BIT0);
		else
		            PanelOutputControl2|= BIT0;
		mStar_WriteByte(OCTRL2,PanelOutputControl2);
            return TRUE;
        }

        Bool AdjustPNSwap(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl1&BIT7)
			PanelOutputControl1&=(~BIT7);
		else
		            PanelOutputControl1 |= BIT7;
		mStar_WriteByte(OCTRL1,PanelOutputControl1);
            return TRUE;
        }
        Bool AdjustChannelSwap(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl1&BIT6)
			PanelOutputControl1&=(~BIT6);
		else
		            PanelOutputControl1 |= BIT6;
		mStar_WriteByte(OCTRL1,PanelOutputControl1);
            return TRUE;
        }
        Bool AdjustMSBLSBExchange(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl1&BIT5)
			PanelOutputControl1&=(~BIT5);
		else
		            PanelOutputControl1 |= BIT5;
		mStar_WriteByte(OCTRL1,PanelOutputControl1);
            return TRUE;
        }
        Bool AdjustTIMode(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl1&BIT4)
			PanelOutputControl1&=(~BIT4);
		else
		            PanelOutputControl1 |= BIT4;
		mStar_WriteByte(OCTRL1,PanelOutputControl1);
            return TRUE;
        }
        Bool AdjustOddMSBLSBExchange(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl1&BIT3)
			PanelOutputControl1&=(~BIT3);
		else
		            PanelOutputControl1 |= BIT3;
		mStar_WriteByte(OCTRL1,PanelOutputControl1);
            return TRUE;
        }
        Bool AdjustEvenMSBLSBExchange(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl1&BIT2)
			PanelOutputControl1&=(~BIT2);
		else
		            PanelOutputControl1 |= BIT2;
		mStar_WriteByte(OCTRL1,PanelOutputControl1);
            return TRUE;
        }
        Bool AdjustOddRBExchange(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl1&BIT1)
			PanelOutputControl1&=(~BIT1);
		else
		            PanelOutputControl1 |= BIT1;
		mStar_WriteByte(OCTRL1,PanelOutputControl1);
            return TRUE;
        }
        Bool AdjustEvenRBExchange(MenuItemActionType action)
        {
            action = action;
		if(PanelOutputControl1&BIT0)
			PanelOutputControl1&=(~BIT0);
		else
		            PanelOutputControl1 |= BIT0;
		mStar_WriteByte(OCTRL1,PanelOutputControl1);
            return TRUE;
        }
       /* Bool AdjustDECurrent(MenuItemActionType action)
        {
            BYTE tempValue;
            tempValue = DecIncValue(action, (PanelODRriving>>6)&0x03, 0, 3, 1);
            if (tempValue == (PanelODRriving>>6)&0x03)
            {
                return FALSE;
            }
            PanelODRriving =(PanelODRriving&0x3F) |(tempValue<<6);
            mStar_WriteByte(ODRV,PanelODRriving);
            return TRUE;
        }
        Bool AdjustClockCurrent(MenuItemActionType action)
        {
            BYTE tempValue;
            tempValue = DecIncValue(action, (PanelODRriving>>4)&0x03, 0, 3, 1);
            if (tempValue == (PanelODRriving>>4)&0x03)
            {
                return FALSE;
            }
            PanelODRriving =(PanelODRriving&0xCF) |(tempValue<<4);
            mStar_WriteByte(ODRV,PanelODRriving);
            return TRUE;
        }
        Bool AdjustOddCurrent(MenuItemActionType action)
        {
            BYTE tempValue;
            tempValue = DecIncValue(action, (PanelODRriving>>2)&0x03, 0, 3, 1);
            if (tempValue == (PanelODRriving>>2)&0x03)
            {
                return FALSE;
            }
            PanelODRriving =(PanelODRriving&0xF3) |(tempValue<<2);
            mStar_WriteByte(ODRV,PanelODRriving);
            return TRUE;
        }
        Bool AdjustEvenCurrent(MenuItemActionType action)
        {
            BYTE tempValue;
            tempValue = DecIncValue(action, PanelODRriving&0x03, 0, 3, 1);
            if (tempValue == PanelODRriving&0x03)
            {
                return FALSE;
            }
            PanelODRriving =(PanelODRriving&0xFC) |tempValue;
            mStar_WriteByte(ODRV,PanelODRriving);
            return TRUE;
        }
	*/	
        Bool AdjustBurstModeFreq(MenuItemActionType action)
        {
		WORD tempValue;
		tempValue = DecIncValue(action, PanelBurstModeFreq, MinBurstModeFreqValue, MaxBurstModeFreqValue, 1);
		if (tempValue == PanelBurstModeFreq)
		{
			return FALSE;
		}
		PanelBurstModeFreq = tempValue;
		SetPWMFreq(PanelBurstModeFreq);
		return TRUE;
        }
        WORD GetBurstModeFreq(void)
        {
            return PanelBurstModeFreq;
        }
        Bool AdjustDefDuty(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelDefDuty, PanelMinDuty, PanelMaxDuty, 1);
                if (tempValue == PanelDefDuty)
                {
                    return FALSE;
                }
                PanelDefDuty = tempValue;
                mStar_AdjustBrightness(UserPrefBrightness);		
                return TRUE;
        }
        WORD GetDefDuty(void)
        {
            return PanelDefDuty;
        }
        Bool AdjustMinDuty(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelMinDuty, MinDutyValue, PanelDefDuty, 1);
                if (tempValue == PanelMinDuty)
                {
                    return FALSE;
                }
                PanelMinDuty = tempValue;
                Set_TestMinDutyFlag();
                mStar_AdjustBrightness(0);		
                Clr_TestMinDutyFlag();
                return TRUE;
        }
        WORD GetMinDuty(void)
        {
            return PanelMinDuty;
        }
        Bool AdjustMaxDuty(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelMaxDuty, PanelDefDuty, MaxDutyValue, 1);
                if (tempValue == PanelMaxDuty)
                {
                    return FALSE;
                }
                PanelMaxDuty = tempValue;
                mStar_AdjustBrightness(100);		
                return TRUE;
        }
        WORD GetMaxDuty(void)
        {
            return PanelMaxDuty;
        }
	Bool AdjustChangeModePanelVddOnOff(MenuItemActionType action)
	{
		action=action;
		if(PanelSettingFlag&bChangeModePanelVddOff)
		{
			PanelSettingFlag&=~bChangeModePanelVddOff;
		}
		else
		{
			PanelSettingFlag|=bChangeModePanelVddOff;
		}
	    return TRUE;
	}

        Bool AdjustSSCSTEPL(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelSSCSTEPL, 0, 0xFF, 1);
                if (tempValue == PanelSSCSTEPL)
                {
                    return FALSE;
                }
                PanelSSCSTEPL = tempValue;
                mStar_WriteByte(LPLL_STEP_L, PanelSSCSTEPL);
                return TRUE;
        }
        WORD GetSSCSTEPL(void)
        {
            return PanelSSCSTEPL;
        }
        Bool AdjustSSCSTEPH(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelSSCSTEPH, 0, 7, 1);
                if (tempValue == PanelSSCSTEPH)
                {
                    return FALSE;
                }
                PanelSSCSTEPH = tempValue;
				#ifdef TSUMXXT		//110311 Modify
				mStar_WriteByteMask(LPLL_STEP_H, PanelSSCSTEPH, (BIT2|BIT1|BIT0));
				#else
                mStar_WriteByte(LPLL_STEP_H, PanelSSCSTEPH);
				#endif
                return TRUE;
        }
        WORD GetSSCSTEPH(void)
        {
            return PanelSSCSTEPH;
        }
        Bool AdjustSSCSPANL(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelSSCSPANL, 0, 0xFF, 1);
                if (tempValue == PanelSSCSPANL)
                {
                    return FALSE;
                }
                PanelSSCSPANL = tempValue;
                mStar_WriteByte(LPLL_SPAN_L, PanelSSCSPANL);
                return TRUE;
        }
        WORD GetSSCSPANL(void)
        {
            return PanelSSCSPANL;
        }
        Bool AdjustSSCSPANH(MenuItemActionType action)
        {
                WORD tempValue;
                tempValue = DecIncValue(action, PanelSSCSPANH, 0, 7, 1);
                if (tempValue == PanelSSCSPANH)
                {
                    return FALSE;
                }
                PanelSSCSPANH = tempValue;
                mStar_WriteByte(LPLL_SPAN_H, PanelSSCSPANH);
                return TRUE;
        }
        WORD GetSSCSPANH(void)
        {
            return PanelSSCSPANH;
        }
	Bool InitialPanelSetting(void)
	{
		Init_PanelSetting();
		#if USEFLASH//PanelSettingInFlash
		PanelSetting.PanelTypeNum=UseUserPanelValue;
		#endif
		SetPWMFreq(PanelBurstModeFreq);
		mStar_AdjustBrightness(UserPrefBrightness);		

		mStar_WriteByte(OCTRL1,PanelOutputControl1);
		mStar_WriteByte(OCTRL2,PanelOutputControl2);
		//mStar_WriteByte(ODRV,PanelODRriving);

		mStar_WriteByte(LPLL_STEP_L, PanelSSCSTEPL);
		#ifdef TSUMXXT		//110311 Modify
		mStar_WriteByteMask(LPLL_STEP_H, PanelSSCSTEPH, (BIT2|BIT1|BIT0));
		#else
		mStar_WriteByte(LPLL_STEP_H, PanelSSCSTEPH);
		#endif
		mStar_WriteByte(LPLL_SPAN_L, PanelSSCSPANL);
		mStar_WriteByte(LPLL_SPAN_H, PanelSSCSPANH);

		return TRUE;
	}
	Bool SaveCurrentPanelSetting(void)
	{
		SavePanelSetting();
		return TRUE;
	}
	
#endif
#if PANEL_VCOM_ADJUST		//121031 Modify
WORD GetVCOMValue(void)
{
     return FactorySetting.VcomValue;
}

Bool AdjustVCOMValue(MenuItemActionType action)
{
	BYTE tempValue;
	
	tempValue = DecIncValue( action, FactorySetting.VcomValue, MIN_VCOM_VALUE, MAX_VCOM_VALUE, 1 );
	
	if( tempValue == FactorySetting.VcomValue )
	{
		return FALSE;
	}
	
	FactorySetting.VcomValue = tempValue;
	
	msWriteMiniLVDSi2c_Vcom_DAC(FactorySetting.VcomValue);

	return TRUE;
}

#elif EnableVcomAdjustItem//def Panel_RSDS
WORD GetVCOMValue(void)
{
     return UserPrefVcomValue;
}

Bool AdjustVCOMValue(MenuItemActionType action)
{
    BYTE tempBank = mStar_ReadByte(0x00);
    mStar_WriteByte(0x00, 0x00);

    if (action == MIA_IncValue)
    {
        UserPrefVcomValue = UserPrefVcomValue + 1;
    }
    else
    {
        UserPrefVcomValue = UserPrefVcomValue + 255;
    }
    mStar_WriteByte(PWM1C, UserPrefVcomValue);
    
    mStar_WriteByte(0x00, tempBank);
    return TRUE;
}

#endif

