#include <stddef.h>
#include "board.h"

#define ProductModeOnValue 0x55AA
#define ProductModeOffValue 0xBB99

#define UseDefaultPanelValue 	0x55AA
#define UseUserPanelValue 	0xBB99

#ifdef DATA_LENGTH_20_BYTE
#define HDCP_BLOCK_LENGTH	20
#else
#define HDCP_BLOCK_LENGTH	5
#endif

#if EnablePanelServiceMenu
typedef struct
{ 
    #if 0//PanelSettingInFlash
    BYTE Ident;
    #endif
    WORD CheckSum;
    //BYTE DitherTpye;
    BYTE SettingFlag;//4//color depth,panel Vdd etc.
    BYTE OnTiming1;		//4//time between panel & data while turn on power
    WORD OnTiming2;		//4//time between data & back light while turn on power
    WORD OffTiming1;	//4//time between back light & data while turn off power
    BYTE OffTiming2;		//4//time between data & panel while turn off power

    WORD HsyncWidth;
    WORD HSyncBackPorch;
    WORD VSyncWidth;
    WORD VsyncBackPorch;
    WORD Width;
    WORD Height;
    WORD HTotal;
    WORD VTotal;
    WORD MaxHTotal;
    WORD MinHTotal;
   /* WORD MaxVTotal;
    WORD MinVTotal;*/

    BYTE TypicalDCLK;
    BYTE MaxDCLK;
    BYTE MinDCLK;
    BYTE Swing;

    BYTE OutputControl1; //4//Scaler Bank 0x42 
    BYTE OutputControl2; //4//Scaler Bank 0x43 
    //BYTE OSContol; //4//output signal control		Scaler Bank 0xF2
    //BYTE ODRriving; //4//Output DE Driving		Scaler Bank 0xF5
    
    WORD BurstModeFreq;
    BYTE DefDuty;
    BYTE MinDuty;
    BYTE MaxDuty;
    //4//ssc setting	
    BYTE SSCSTEPL;//0xD8
    BYTE SSCSTEPH;//0xD9
    BYTE SSCSPANL;//0xDA
    BYTE SSCSPANH;//0xDB
    #if PanelSettingInFlash
    WORD PanelTypeNum;
    #endif
    //BYTE CheckSum;
}PanelSettingType;

#endif
#define PanelSettingSize		sizeof(PanelSettingType)
#define PanelSettingChecksumSize		sizeof(PanelSetting.CheckSum)
#if USEFLASH
typedef struct
{
    BYTE Ident;
    DWORD Backlighttime;
    #if BlacklitTimeOnShow
    DWORD BlacklitTimeOn_H;
    BYTE BlacklitTimeOn_M;
    BYTE BlacklitTimeOn_S;
    //BYTE BlacklitTimeOn_HH;
    #endif
    BYTE CheckSum;
} MonitorSettingType2;

typedef struct
{
    BYTE Ident;
    WORD MonitorFlag; // 0
    BYTE Brightness; // 1
    BYTE Contrast; //8
    BYTE RedColor; //a
    BYTE GreenColor; //b
    BYTE BlueColor; //c
    BYTE YColor; //a
    BYTE CColor; //b
    BYTE MColor; //c
    BYTE ColorTemp; //9  
    BYTE BrightnessUser; // 1
    BYTE ContrastUser; //8
    BYTE BrightnessWarm1;
    BYTE ContrastWarm1;
    BYTE BrightnessNormal;
    BYTE ContrastNormal;
    BYTE BrightnessCool1;
    BYTE ContrastCool1;
    BYTE BrightnesssRGB;
    BYTE ContrastsRGB;
    BYTE RedColorUser; //d
    BYTE GreenColorUser; //e
    BYTE BlueColorUser; //f
    //BYTE LastMenuPage;
    #if MWEFunction
        BYTE Hue;
        BYTE Saturation;
        BYTE SubContrast;
        BYTE SubBrightness;
        // 2006/10/26 6:47PM by Emily   BYTE SubSharpness;
        //BYTE SkinTone;
        BYTE ColorFlag;
        // 2006/10/26 6:48PM by Emily   BYTE DLCModeType;
        //BYTE MWEModeType;
        BYTE BF_Size;
        BYTE BF_Hstart;
        BYTE BF_Vstart;
        WORD BF_HSize;
        WORD BF_VSize;
    #endif 
    // 2006/10/26 6:49PM by Emily   BYTE Sharpness;//13
    BYTE Volume;
    BYTE ECOMode;
    BYTE Language; //14
    BYTE OsdHStart; //15
    BYTE OsdVStart; //16
    BYTE OsdTime; //17
    BYTE BankNo; //18
    BYTE OsdTrans;
	#ifdef OffPower          //090331 xiandi.yu Auto Off Power
    BYTE AutoPowerOff;
    #endif
    BYTE Addr; //19
    BYTE Value; //1a
    BYTE GamaMode;
    BYTE DcrMode;
    BYTE InputType; //1b
    BYTE InputSelectType; //1b
    // 2006/10/24 7:17PM by Emily   BYTE BlackLevel; // 2 //1125
    BYTE RedBlackLevel;
    BYTE GreenBlackLevel;
    BYTE BlueBlackLevel;
    BYTE SavedModeIndex;
    #if Enable_Expansion
	BYTE ExpansionMode;
    #endif 
    #if Enable_Lightsensor
    BYTE iCareMode;
    #endif
    #ifdef EnableLightBar
    BYTE LightBar;
    #endif
    //======for AutoBacklight================
    BYTE Histogram1;
    BYTE Histogram2;
    BYTE ALhaA;
    BYTE BataA;


	
    //DWORD BackLightTime;
    // 2006/11/6 5:34PM by Emily   BYTE  AutoModel;
    // 2006/11/6 5:33PM by Emily   BYTE  CompareRange;
    // 2006/11/6 5:34PM by Emily   BYTE  DiffHvalue;
    BYTE checksum; //1c
}MonitorSettingType;

typedef struct
{ 
   //#ifdef SETTING_IN_FLASH_12K
    BYTE Ident;

   //#endif
	#ifdef TSUMXXT//( ChipID == ChipTSUMT )		//110311 Modify
	BYTE AdcPgaGain;    //110401 Modify
	WORD AdcRedGain;
	WORD AdcGreenGain;
	WORD AdcBlueGain;
	WORD AdcRedOffset;
	WORD AdcGreenOffset;
	WORD AdcBlueOffset;
	#else
    BYTE AdcRedGain; // 2
    BYTE AdcGreenGain; //3
    BYTE AdcBlueGain; //4
    BYTE AdcRedOffset; // 5
    BYTE AdcGreenOffset; //6
    BYTE AdcBlueOffset; //7
	#endif
    BYTE FBrightnessWarm1;
    BYTE FContrastWarm1;
    BYTE RedColorWarm1; //d
    BYTE GreenColorWarm1; //e
    BYTE BlueColorWarm1; //f
    
    BYTE FBrightnessNormal;
    BYTE FContrastNormal;
    BYTE RedColorNormal; //10
    BYTE GreenColorNormal; //11
    BYTE BlueColorNormal; //12
    
    BYTE FBrightnessCool1;
    BYTE FContrastCool1;
    BYTE RedColorCool1; //10
    BYTE GreenColorCool1; //11
    BYTE BlueColorCool1; //12
    
    BYTE FBrightnesssRGB;
    BYTE FContrastsRGB;
    BYTE RedColorsRGB; //10
    BYTE GreenColorsRGB; //11
    BYTE BlueColorsRGB; //12
    WORD ProductModeValue;

  #if PANELSSCSTEPLFUN
    BYTE SSCSTEPL;//0xD8
    BYTE SSCSTEPH;//0xD9
    BYTE SSCSPANL;//0xDA
    BYTE SSCSPANH;//0xDB

	#endif

	
    #if EnableVcomAdjustItem||PANEL_VCOM_ADJUST		//121031 Modify
	#if VCOM_TYPE == VCOM_MAXIM
	WORD VcomValue;
	#else
	BYTE VcomValue;
	#endif
    #endif
    BYTE LogoON;
    BYTE CheckSum; //1c
} FactorySettingType;
#if 1//def SETTING_IN_FLASH_12K

typedef struct
{
   BYTE Ident;	
  BYTE Index;	
  WORD HTotal;
  WORD HStart;
  WORD VStart;
  WORD AutoHStart;
  WORD AutoVStart;  //2004/01/15 num11
  BYTE Phase;
  BYTE AutoTimes;
  WORD HerFreq;
  WORD VerFreq;
  WORD VTotal;
  BYTE Flags;

  BYTE checksum;
} ModeSettingType;


#else
typedef struct
{
   BYTE Ident;	
  BYTE Index;	
  WORD HTotal;
  WORD HStart;
  WORD VStart;
  WORD AutoHStart;
  WORD AutoVStart;  //2004/01/15 num11
  BYTE Phase;
  BYTE AutoTimes;

  BYTE checksum;
} ModeSettingType;
#endif

typedef struct
{
    BYTE Ident;	
    BYTE Index;
    WORD HerFreq;
    WORD VerFreq;
    WORD VTotal;
    BYTE Flags;
} ModeInfoType;

#if USEFLASH
#define MonitorSetting2Size	sizeof(MonitorSettingType2)
#endif

#define NVRamAddress_Base	0x0000
#define MonitorSettingSize	sizeof(MonitorSettingType)
#define FactorySettingSize	sizeof(FactorySettingType)
#define ModeSettingSize		sizeof(ModeSettingType)
#define ModeInfoSize		sizeof(ModeInfoType)

 	#define NumberOfMode		18
	#define UserModeNumber		5


#define ChecksumValue	0xaa

#else

typedef struct
{
    BYTE MonitorFlag; // 0
    BYTE AdcRedGain; // 2
    BYTE AdcGreenGain; //3
    BYTE AdcBlueGain; //4
    BYTE AdcRedOffset; // 5
    BYTE AdcGreenOffset; //6
    BYTE AdcBlueOffset; //7
    BYTE Brightness; // 1
    BYTE Contrast; //8
    BYTE RedColor; //a
    BYTE GreenColor; //b
    BYTE BlueColor; //c
    BYTE YColor; //a
    BYTE CColor; //b
    BYTE MColor; //c
    BYTE ColorTemp; //9  
    BYTE BrightnessUser; // 1
    BYTE ContrastUser; //8
    BYTE RedColorUser; //d
    BYTE GreenColorUser; //e
    BYTE BlueColorUser; //f
    BYTE BrightnessWarm1;
    BYTE ContrastWarm1;
    BYTE RedColorWarm1; //d
    BYTE GreenColorWarm1; //e
    BYTE BlueColorWarm1; //f
    BYTE BrightnessNormal;
    BYTE ContrastNormal;
    BYTE RedColorNormal; //10
    BYTE GreenColorNormal; //11
    BYTE BlueColorNormal; //12
    BYTE BrightnessCool1;
    BYTE ContrastCool1;
    BYTE RedColorCool1; //10
    BYTE GreenColorCool1; //11
    BYTE BlueColorCool1; //12
    BYTE BrightnesssRGB;
    BYTE ContrastsRGB;
    BYTE RedColorsRGB; //10
    BYTE GreenColorsRGB; //11
    BYTE BlueColorsRGB; //12
    //BYTE LastMenuPage;
    #if MWEFunction
        BYTE Hue;
        BYTE Saturation;
        BYTE SubContrast;
        BYTE SubBrightness;
        // 2006/10/26 6:47PM by Emily   BYTE SubSharpness;
        //BYTE SkinTone;
        BYTE ColorFlag;
        // 2006/10/26 6:48PM by Emily   BYTE DLCModeType;
        //BYTE MWEModeType;
        BYTE BF_Size;
        BYTE BF_Hstart;
        BYTE BF_Vstart;
        WORD BF_HSize;
        WORD BF_VSize;
    #endif 
    // 2006/10/26 6:49PM by Emily   BYTE Sharpness;//13
    BYTE Volume;
    BYTE ECOMode;
    BYTE Language; //14
    BYTE OsdHStart; //15
    BYTE OsdVStart; //16
    BYTE OsdTime; //17
	BYTE OsdTrans;
#ifdef OffPower        //090331 xiandi.yu Auto Off Power
    BYTE AutoPowerOff;
#endif
    BYTE BankNo; //18
    BYTE Addr; //19
    BYTE Value; //1a
    BYTE GamaMode;
    BYTE DcrMode;
    BYTE InputType; //1b
    BYTE InputSelectType; //1b
    // 2006/10/24 7:17PM by Emily   BYTE BlackLevel; // 2 //1125
    BYTE RedBlackLevel;
    BYTE GreenBlackLevel;
    BYTE BlueBlackLevel;
    BYTE SavedModeIndex;
    BYTE FBrightnessNormal;
    BYTE FContrastNormal;
    BYTE FBrightnessCool1;
    BYTE FContrastCool1;
    BYTE FBrightnessWarm1;
    BYTE FContrastWarm1;
    #if Enable_Expansion
        BYTE ExpansionMode;
    #endif 
    #if Enable_Lightsensor
    BYTE iCareMode;
    #endif
    #ifdef EnableLightBar
    BYTE LightBar;
    #endif
    //======for AutoBacklight================
    BYTE Histogram1;
    BYTE Histogram2;
    BYTE ALhaA;
    BYTE BataA;
    // 2006/11/6 5:34PM by Emily   BYTE  AutoModel;
    // 2006/11/6 5:33PM by Emily   BYTE  CompareRange;
    // 2006/11/6 5:34PM by Emily   BYTE  DiffHvalue;
    #if EnableVcomAdjustItem||PANEL_VCOM_ADJUST		//121031 Modify
	#if VCOM_TYPE == VCOM_MAXIM
	WORD VcomValue;
	#else
	BYTE VcomValue;
	#endif
    #endif
     BYTE LogoON;
    BYTE checksum; //1c
} MonitorSettingType;
typedef struct
{
    WORD HTotal;
    WORD HStart;
    WORD VStart;
    WORD AutoHStart;
    WORD AutoVStart;
    BYTE Phase;
    BYTE AutoTimes;
    BYTE checksum;
} ModeSettingType;
typedef struct
{
    WORD HerFreq;
    WORD VerFreq;
    WORD VTotal;
    BYTE Flags;
} ModeInfoType;
#define ChecksumValue	0x22 // 0xcc
#define BoardChecksum	0x11 // 0xaa
//==========================================================================
//==========================================================================
#ifdef EEPRom_24C04
    #define NumberOfMode		15//2006-07-07 Andy //2006-0626 Andy 55 //50  //wmz 20051012 for WidePanel
#else 
    #define NumberOfMode		50
#endif 
// NVRAM address configuration
#define NVRamAddress_Base	0
#define MonitorSettingSize	sizeof(MonitorSettingType)
#define ModeSettingSize		sizeof(ModeSettingType)
#define ModeInfoSize		sizeof(ModeInfoType)
#if 1//ENABLE_HDCP
    #define HDCP_KEY_ROMSIZE      304
    #define HDCP_KEY_SIZE1        5       // HDCP key BKSV
    #define HDCP_KEY_SIZE2        284     // HDCP key
    #define HDCPAddress_Base	0
#endif 
typedef enum
{
    ProductModeAddr=NVRamAddress_Base,
  #if EnablePanelServiceMenu
    PanelTypeNumAddr=ProductModeAddr+2, 
    PanelSettingAddr = PanelTypeNumAddr+2, 
    MonitorSettingAddr =  PanelSettingAddr+PanelSettingSize,
  #else
    MonitorSettingAddr = ProductModeAddr+2, 
  #endif
  #if Backupdata 
    BackupdataAddress=MonitorSettingAddr+MonitorSettingSize,
    Monitorchecksum=BackupdataAddress+MonitorSettingSize,
    ModeSettingAddr=Monitorchecksum+1,
  #else
    ModeSettingAddr=MonitorSettingAddr+MonitorSettingSize,
  #endif
  
    ModeInfoAddr=ModeSettingAddr+ModeSettingSize*NumberOfMode,
    EndAddr=ModeInfoAddr+ModeInfoSize*NumberOfMode,
#ifdef EEPRom_24C04
    BlacklitTimeAddr	=0x1E0,
    CheckBoardAddr=0x1F0
#else
    BlacklitTimeAddr	=0x7E0,
    CheckBoardAddr=0x7F0
#endif
} NVRamAddress;
typedef enum
{
    RM_HDCP_KEY_BASEADDR1  =HDCPAddress_Base,
    RM_HDCP_KEY_BASEADDR2  =HDCPAddress_Base+HDCP_KEY_SIZE1
} NVRamAddress1;
#define adrMonitorSetting_Base		NVRamAddress_Base
#define adrModeSetting_Base		NVRamAddress_Base+MonitorSettingSize
#define nvrMonitorAddr(member)		MonitorSettingAddr+offsetof(MonitorSettingType, member)
#define nvrModeBaseAddr(modeIndex)	ModeSettingAddr+(WORD)modeIndex*ModeSettingSize
#define nvrModeAddr(member)		nvrModeBaseAddr(SaveIndex)+offsetof(ModeSettingType, member)
#define nvrModeInfoAddr(modeIndex)	ModeInfoAddr+(WORD)modeIndex*ModeInfoSize
#if EnablePanelServiceMenu
#define nvrPanelAddr(member)		PanelSettingAddr+offsetof(PanelSettingType, member)
#endif
#endif
