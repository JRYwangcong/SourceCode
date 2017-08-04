#include "board.h"
#define bVSyncNegative			BIT0
#define bHSyncNegative			BIT1
#define bHSyncLoss			    BIT2
#define bVSyncLoss			    BIT3
#define bInterlaceMode			BIT4
#define bUnsupportMode			BIT5
#define bUserMode		   	    BIT6
#define bNativeMode			BIT7
#define bShrinkMode			BIT7

typedef enum
{ 
#if INPUT_TYPE == INPUT_1A
Input_Analog1,
Input_Nums,
Input_Digital,
Input_Analog2,
Input_YPbPr,
Input_Digital2
#elif INPUT_TYPE == INPUT_1A1D
Input_Analog1,
Input_Digital,
Input_Nums,
Input_Analog2,
Input_YPbPr,
Input_Digital2
#elif INPUT_TYPE == INPUT_1A2D
Input_Analog1,
Input_Digital,
Input_Digital2,
Input_Nums,
Input_Analog2,
Input_YPbPr
#elif INPUT_TYPE == INPUT_1A1H
Input_Analog1,
Input_Digital,
Input_Nums,
Input_Digital2,
Input_Analog2,
Input_YPbPr
#endif
}InputPortType;

// 071218 addition for UserPrefInputSelectType
typedef enum
{ 
#if INPUT_TYPE == INPUT_1A1D
  InputSelectType_Analog,
  InputSelectType_Digital,
  InputSelectType_Auto,
#elif INPUT_TYPE == INPUT_1A2D
  InputSelectType_Analog,
  InputSelectType_Digital,
  InputSelectType_HDMI,
  InputSelectType_Auto,
#elif INPUT_TYPE == INPUT_1A
  InputSelectType_Analog,
#elif INPUT_TYPE == INPUT_1A1H
  InputSelectType_Analog,
  InputSelectType_HDMI,
  InputSelectType_Auto,
#endif
 
  InputSelectType_Nums
} InputPortSelectType;



/*
typedef enum
{ 

#if ENABLE_HDMI
  Input_Analog1,
    Input_Digital,
  Input_Digital2,
  Input_Nums,
#else
#if !Dual//(Dual == AOC_DsubOnly)
  Input_Analog1,
  Input_Nums,
  Input_Digital,
  Input_Digital2,  
#else  
  Input_Analog1,
  Input_Digital,
  Input_Nums,
  Input_Digital2,  
#endif  
#endif  

  Input_YPbPr,
 // Input_Nums,

  Input_Analog2,
  Input_Nothing
} InputPortType;
*/

// HpVp->3,  HpVn->2, HnVp->1, HnVn->0

#define SyncHpVp 		8  // Sync Polarity
#define SyncHpVn 		4
#define SyncHnVp  		2
#define SyncHnVn  		1


#if 0
typedef enum
{ Res_640x350,	
  Res_640x400, 
  Res_720x400, 
  Res_640x480, 
  Res_800x600, 
  Res_832x624,
  Res_884x930, //0909	
  Res_1024x768, 
  Res_1280x1024,
  Res_1600x1200,
  Res_1152x864, 
  Res_1152x870,
  Res_1280x720,
  Res_1280x960,
  Res_720x480,
  Res_800x1209,
  Res_1440x900,	// for Wide panel
  Res_1400x1050, 
  Res_1680x1050, 
  
  Max_Res
} ResolutionType;
#endif

typedef enum
{ 	Res_640x350,
	Res_640x400,
	Res_640x480,
	
	Res_700x570,       ///3
	Res_720x400,
	Res_720x480,
	Res_720x483,
	Res_720x576,          ///7
	
	Res_800x600,
	Res_800x1209,
	Res_832x624,
	Res_848x480,   //2005-07-26
	Res_884x930, //0909  12
	
	Res_1024x768,
	Res_1128x730,// 20151123 lixw
	Res_1152x864,
	Res_1152x870,

	Res_1280x600,           //16
	Res_1280x720,
	Res_1280x768,  //2005-07-26
	Res_1280x800,
	Res_1280x960,
	Res_1280x1024,

	Res_1360x768,        //22
	Res_1366x768,

	Res_1400x1050,
	Res_1440x900,          //25

	Res_1600x900,
	Res_1600x1200,
	Res_1680x1050, 
	
	Res_1792x1344,
	
	Res_1920x1080,
	Res_1920x1200,

  Max_Res
} ResolutionType;

typedef enum
{
PRESET_640x350_85Hz_VESA,			//0
PRESET_640x400_60Hz,
PRESET_640x400_85Hz_VESA,
PRESET_640x480_72Hz_VESA,
PRESET_640x480_75Hz_VESA,

PRESET_640x480_85Hz_VESA,			//5
PRESET_720x400_85Hz_VESA,
PRESET_800x600_56Hz_VESA,
PRESET_800x600_60Hz_VESA,
PRESET_800x600_70Hz,

PRESET_800x600_72Hz_VESA,			//10
PRESET_800x600_75Hz_VESA,
PRESET_800x600_85Hz_VESA,
PRESET_848x480_60Hz,
PRESET_1024x768_60Hz_VESA,

PRESET_1024x768_70Hz_VESA,			//15
PRESET_1024x768_72Hz_VESA,
PRESET_1024x768_75Hz_VESA,
PRESET_1024x768_85Hz_VESA,
PRESET_1128x730_60Hz,					//19   20151123 add

PRESET_1152x864_60Hz_VESA,			//20

PRESET_1152x864_70Hz_VESA,			//21
PRESET_1152x864_72Hz_VESA,
PRESET_1152x864_75Hz_VESA,
PRESET_1152x864_85Hz_VESA,
PRESET_1280x720_60Hz_VESA,

PRESET_1280x720_70Hz_VESA,			//26
PRESET_1280x720_72Hz_VESA,
PRESET_1280x720_75Hz_VESA,

PRESET_1280x960_60Hz_VESA,
PRESET_1280x960_70Hz_VESA,

PRESET_1280x960_72Hz_VESA,			//31
PRESET_1280x960_75Hz_VESA,
PRESET_1280x960_85Hz_VESA,
PRESET_1280x1024_60Hz_VESA,
PRESET_1280x1024_75Hz_VESA,

PRESET_1280x1024_85Hz_VESA,			//36
#if 1//((PanelType==PNL_USUAL_S1366x768) || (PanelType==PNL_USUAL_D1366x768)) 
PRESET_1360x768_60Hz_R,
PRESET_1360x768_60Hz,					//38
PRESET_1360x768_70Hz_R,
PRESET_1360x768_75Hz_R,				//40

PRESET_1366x768_60Hz_R,				//41
PRESET_1366x768_60Hz_N,				//42
PRESET_1366x768_60Hz_1,
#endif
					//45     //39
PRESET_1600x1200_60Hz_VESA,
PRESET_1600x1200_65Hz_VESA,
PRESET_1600x1200_70Hz_VESA,
PRESET_1600x1200_75Hz_VESA,				//47

PRESET_1792x1344_60Hz_VESA,			
PRESET_1792x1344_70Hz,
PRESET_1792x1344_75Hz,     		//50
PRESET_640x350_70Hz_IMB_VGA,
PRESET_720x400_70Hz_VGA,
PRESET_640x400_70Hz_IMB_VGA,

PRESET_640x480_60Hz_VESA,			//54          
PRESET_640x480_66Hz_MAC,
PRESET_640x480_70Hz,
PRESET_720x400_60Hz_VGA,


PRESET_832x624_75Hz_MAC,				
PRESET_1152x870_60Hz_MAC,
PRESET_1152x870_75Hz_MAC,		//60
PRESET_1280x768_60Hz_R,
PRESET_1280x768_60Hz_N,

PRESET_1280x768_70Hz,					//63
PRESET_1280x768_75Hz,
PRESET_1280x768_85Hz,
PRESET_1280x800_60Hz_R,
PRESET_1280x800_60Hz_N,

PRESET_1280x800_70Hz,					//68
PRESET_1280x800_75Hz,
PRESET_1280x800_76Hz,
PRESET_1280x800_85Hz,
PRESET_1280x1024_72Hz,

PRESET_1280x1024_70Hz,				//75             //69
/*
PRESET_1440x1050_60Hz_R,
PRESET_1440x1050_60Hz_VESA,
PRESET_1440x1050_70Hz_VESA,
PRESET_1440x1050_75Hz_VESA,
PRESET_1440x1050_85Hz_VESA,
//*/

PRESET_1600x900_60Hz,
PRESET_1440x900_60Hz_R,				//76            //75
PRESET_1440x900_60Hz_N,
PRESET_1440x900_70Hz,
PRESET_1440x900_75Hz,
PRESET_1440x900_85Hz,
PRESET_1600x900_60Hz_R,
PRESET_1600x900_70Hz,

PRESET_1600x900_75Hz,

PRESET_1680x1050_60Hz_R,				//81             //80
PRESET_1680x1050_60Hz,
PRESET_1680x1050_70Hz,
PRESET_1680x1050_75hz,
PRESET_1920x1080_60Hz_R,
PRESET_1920x1080_60Hz_VESA_1,
PRESET_1920x1080_60Hz_VESA,		 //86            //85
PRESET_1920x1080_70Hz,
PRESET_1920x1080_75Hz,
PRESET_1920x1200_60Hz_R,
PRESET_1920x1200_60Hz_N,

PRESET_1920x1200_70Hz,				//91            //90
PRESET_1920x1200_75Hz,
PRESET_1280x720_60Hz_VESA_R,
PRESET_1280x600_60Hz,
PRESET_1280x600_60Hz_R,

PRESET_1280x600_70Hz,					//96          //95
PRESET_700x570_50Hz,
PRESET_720x480_60Hz_YCbCr,
PRESET_720x480i_60Hz_YPbCr,
PRESET_480P_60Hz,

PRESET_576P_YCbCr,						//101          //100
PRESET_576_75Hz,
PRESET_576i_50Hz,
PRESET_800x1209_TPV,
PRESET_884x930,

PRESET_1152x864_70Hz_ATI,				//106            //105
PRESET_720P,
PRESET_1920x1080i_50Hz_YPbCr,
PRESET_1080i_NTSC,
PRESET_1080P_PAL,

PRESET_1080P_NTSC,					//111              //110
PRESET_1280x960_60Hz_R,
PRESET_1024x768_75Hz_MAC,
PRESET_1024x768_72Hz,

} PresetModeType;

typedef struct
{
    BYTE fFlags;
    BYTE InputType;
    WORD HPeiod;
    WORD VTotal;
    BYTE ModeIndex;
	BYTE Vsyncwidth; 
} InputTimingType;
typedef struct
{
    WORD DispWidth;
    WORD DispHeight;
} InputResolutionType;
typedef struct
{
    BYTE Flags;
    ResolutionType ResIndex;
    WORD HFreq;
    WORD VFreq;
    WORD HTotal;
    WORD VTotal;
    WORD HStart;
    WORD VStart;
    WORD HSyncTime;
} InputModeType;
extern WORD GetStandardModeWidth(BYTE StandardModeGroup);
extern WORD GetStandardModeHeight(BYTE StandardModeGroup);
extern WORD GetStandardModeGroup(BYTE SrcModeIndex);
extern WORD GetStandardModeHTotal(BYTE SrcModeIndex);
extern WORD GetStandardModeVFreq(BYTE SrcModeIndex);
extern WORD GetStandardModeHStart(BYTE SrcModeIndex);
extern WORD GetStandardModeVStart(BYTE SrcModeIndex);

