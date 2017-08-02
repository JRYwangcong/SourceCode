//#include "Board.h"
#ifndef _Mode_H_
#define _Mode_H_

#ifdef _MODE_C_
#define _MODEDEC_
#else
#define _MODEDEC_ extern
#endif


#define bVSyncNegative          BIT0
#define bHSyncNegative          BIT1
#define bHSyncLoss              BIT2
#define bVSyncLoss              BIT3
#define bInterlaceMode          BIT4
#define bUnsupportMode          BIT5
#define bUserMode               BIT6
#define bNativeMode             BIT7
//#define bShrinkMode             BIT7


#define HPeriod_Torlance            10  // for mode change
#define VTotal_Torlance             4   // 10 // for mode change
#define VTotal_Delta                50
#define HFreq_Torlance              12  // 15 // 1.5 KHz
#define VFreq_Torlance              12  // 15 // 1.5 Hz
#define HFREQ_TOLERANCE_ENLARGE     20 // 15KHz
#define VFREQ_TOLERANCE_ENLARGE     20 // 10Hz

//============================================================================================
#define StandardModeWidth(InputPort)   GetStandardModeWidth(InputPort)//StandardModeResolution[StandardModeGroup].DispWidth
#define StandardModeHeight(InputPort)  GetStandardModeHeight(InputPort)
#define StandardModeGroup(InputPort)   GetStandardModeGroup(InputPort)
#define StandardModeHStart(InputPort)  GetStandardModeHStart(InputPort)
#define StandardModeVStart(InputPort)  GetStandardModeVStart(InputPort)
#define StandardModeHTotal(InputPort)  GetStandardModeHTotal(InputPort)
#define StandardModeVTotal(InputPort)  GetStandardModeVTotal(InputPort)
#define StandardModeHFreq(InputPort)   GetStandardModeHFreq(InputPort)
#define StandardModeVFreq(InputPort)   GetStandardModeVFreq(InputPort)
//============================================================================================
#define SyncLoss            (bHSyncLoss|bVSyncLoss)
#define SyncPolarity(status)        (status&0x03)
#define GetSyncPolarity(status)     (1<<(status&0x03))

//============================================================================================
/// ~~~~~~~~~~~~~~~~~~~~~~~~

enum
{
    CLK_ADC_BUFF_DIV2,
    CLK_DVI_BUFF_DIV2,
    DP_STRM_CLK,
    CLK_DVI_BUFF,

};

#if ENABLE_VGA_INPUT
#define First_Input_Port    Input_Analog1
#else
#define First_Input_Port    Input_Digital1
#endif
enum
{
  #if (INPUT_TYPE&INPUT_1A)
    Input_Analog1,
  #endif
  #if (INPUT_TYPE >= INPUT_1C)
    Input_Digital1,
  #endif
  #if (INPUT_TYPE >= INPUT_2C)
    Input_Digital2,
  #endif
  #if (INPUT_TYPE >= INPUT_3C)
    Input_Digital3,
  #endif
  #if (INPUT_TYPE >= INPUT_4C)
    Input_Digital4,
  #endif
  #if (INPUT_TYPE >= INPUT_5C)
    Input_Digital5,
  #endif
  #if (INPUT_TYPE >= INPUT_6C)
    Input_Digital6,
  #endif
  #if (INPUT_TYPE >= INPUT_7C)
    Input_Digital7,
  #endif
  #if (INPUT_TYPE >= INPUT_8C)
    Input_Digital8,
  #endif
    Input_Nums,
///////////////////////////////
///////////////////////////////
  #if ENABLE_VGA_INPUT
    INPUT_VGA           = Input_Analog1,
  #endif
  #if ENABLE_DVI_INPUT
    INPUT_DVI           = DVI_PORT_PRIORITY,
   #if ENABLE_DVI2ND_INPUT
    INPUT_DVI2ND        = DVI2ND_PORT_PRIORITY,
    #if ENABLE_DVI3ND_INPUT
    INPUT_DVI3ND        = DVI3ND_PORT_PRIORITY,
    #endif
   #endif
  #endif
  #if ENABLE_HDMI_INPUT
    INPUT_HDMI          = HDMI_PORT_PRIORITY,
   #if ENABLE_HDMI2ND_INPUT
    INPUT_HDMI2ND       = HDMI2ND_PORT_PRIORITY,
    #if ENABLE_HDMI3ND_INPUT
    INPUT_HDMI3ND       = HDMI3ND_PORT_PRIORITY,
    #endif
    #if ENABLE_HDMI4ND_INPUT
    INPUT_HDMI4ND       = HDMI4ND_PORT_PRIORITY,
    #endif
   #endif
  #endif
  #if ENABLE_DP_INPUT
    INPUT_DISPLAYPORT   = DP_PORT_PRIORITY,
   #if ENABLE_DP2ND_INPUT
    INPUT_DISPLAYPORT2ND= DP2ND_PORT_PRIORITY,
    #if ENABLE_DP3ND_INPUT
    INPUT_DISPLAYPORT3ND= DP3ND_PORT_PRIORITY,
    #endif
    #if ENABLE_DP4ND_INPUT
    INPUT_DISPLAYPORT4ND= DP4ND_PORT_PRIORITY,
    #endif
    #if ENABLE_DP5ND_INPUT
    INPUT_DISPLAYPORT5ND= DP5ND_PORT_PRIORITY,
    #endif
    #if ENABLE_DP6ND_INPUT
    INPUT_DISPLAYPORT6ND= DP6ND_PORT_PRIORITY,
    #endif
   #endif
  #endif
  #if ENABLE_VD_INPUT
    INPUT_VD          = VD_PORT_PRIORITY,
  #endif
};

#if (MS_BOARD_TYPE_SEL==BD_MST9UHL_V3)
#if (defined(FengYuan_Project)||defined(XuLi_Project)||defined(MingCai_Project))
typedef enum
{
  #if Enable_AutoInput_Menu
    INPUT_PRIORITY_AUTO,
  #endif
  #if ENABLE_DP_INPUT
    INPUT_PRIORITY_DP       ,//=(DP_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP2ND_INPUT
    INPUT_PRIORITY_DP2ND    ,//=(DP2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP3ND_INPUT
    INPUT_PRIORITY_DP3ND    ,//=(DP3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP4ND_INPUT
    INPUT_PRIORITY_DP4ND    ,//=(DP4ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP5ND_INPUT
    INPUT_PRIORITY_DP5ND    ,//=(DP5ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP6ND_INPUT
    INPUT_PRIORITY_DP6ND    ,//=(DP6ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif

  
  #if ENABLE_DVI_INPUT
    INPUT_PRIORITY_DVI      ,//=(DVI_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DVI2ND_INPUT
    INPUT_PRIORITY_DVI2ND   ,//=(DVI2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DVI3ND_INPUT
    INPUT_PRIORITY_DVI3ND  ,// =(DVI3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI_INPUT
    INPUT_PRIORITY_HDMI     ,//=(HDMI_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI2ND_INPUT
    INPUT_PRIORITY_HDMI2ND  ,//=(HDMI2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI3ND_INPUT
    INPUT_PRIORITY_HDMI3ND  ,//=(HDMI3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI4ND_INPUT
    INPUT_PRIORITY_HDMI4ND  ,//=(HDMI4ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif

  #if ENABLE_VGA_INPUT
    INPUT_PRIORITY_VGA,
  #endif

  #if ENABLE_VD_INPUT
    INPUT_PRIORITY_VD          ,//= (VD_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif

    INPUT_PRIORITY_NUMS     ,//=(Input_Nums+ENABLE_AUTO_SELECT_SOURCE),
} InputPortPriorityType;


#else
typedef enum
{
  #if Enable_AutoInput_Menu
    INPUT_PRIORITY_AUTO,
  #endif
  #if ENABLE_VGA_INPUT
    INPUT_PRIORITY_VGA,
  #endif
  #if ENABLE_DP_INPUT
    INPUT_PRIORITY_DP       ,//=(DP_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP2ND_INPUT
    INPUT_PRIORITY_DP2ND    ,//=(DP2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP3ND_INPUT
    INPUT_PRIORITY_DP3ND    ,//=(DP3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP4ND_INPUT
    INPUT_PRIORITY_DP4ND    ,//=(DP4ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP5ND_INPUT
    INPUT_PRIORITY_DP5ND    ,//=(DP5ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP6ND_INPUT
    INPUT_PRIORITY_DP6ND    ,//=(DP6ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif

  
  #if ENABLE_DVI_INPUT
    INPUT_PRIORITY_DVI      ,//=(DVI_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DVI2ND_INPUT
    INPUT_PRIORITY_DVI2ND   ,//=(DVI2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DVI3ND_INPUT
    INPUT_PRIORITY_DVI3ND  ,// =(DVI3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI_INPUT
    INPUT_PRIORITY_HDMI     ,//=(HDMI_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI2ND_INPUT
    INPUT_PRIORITY_HDMI2ND  ,//=(HDMI2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI3ND_INPUT
    INPUT_PRIORITY_HDMI3ND  ,//=(HDMI3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI4ND_INPUT
    INPUT_PRIORITY_HDMI4ND  ,//=(HDMI4ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif

  #if ENABLE_VD_INPUT
    INPUT_PRIORITY_VD          ,//= (VD_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif

    INPUT_PRIORITY_NUMS     ,//=(Input_Nums+ENABLE_AUTO_SELECT_SOURCE),
} InputPortPriorityType;
#endif
#else
typedef enum
{
  #if Enable_AutoInput_Menu
    INPUT_PRIORITY_AUTO,
  #endif
  #if ENABLE_VGA_INPUT
    INPUT_PRIORITY_VGA,
  #endif
  #if ENABLE_DVI_INPUT
    INPUT_PRIORITY_DVI      ,//=(DVI_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DVI2ND_INPUT
    INPUT_PRIORITY_DVI2ND   ,//=(DVI2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DVI3ND_INPUT
    INPUT_PRIORITY_DVI3ND  ,// =(DVI3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI_INPUT
    INPUT_PRIORITY_HDMI     ,//=(HDMI_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI2ND_INPUT
    INPUT_PRIORITY_HDMI2ND  ,//=(HDMI2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI3ND_INPUT
    INPUT_PRIORITY_HDMI3ND  ,//=(HDMI3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_HDMI4ND_INPUT
    INPUT_PRIORITY_HDMI4ND  ,//=(HDMI4ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP_INPUT
    INPUT_PRIORITY_DP       ,//=(DP_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP2ND_INPUT
    INPUT_PRIORITY_DP2ND    ,//=(DP2ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP3ND_INPUT
    INPUT_PRIORITY_DP3ND    ,//=(DP3ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP4ND_INPUT
    INPUT_PRIORITY_DP4ND    ,//=(DP4ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP5ND_INPUT
    INPUT_PRIORITY_DP5ND    ,//=(DP5ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif
  #if ENABLE_DP6ND_INPUT
    INPUT_PRIORITY_DP6ND    ,//=(DP6ND_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif

  #if ENABLE_VD_INPUT
    INPUT_PRIORITY_VD          ,//= (VD_PORT_PRIORITY+ENABLE_AUTO_SELECT_SOURCE),
  #endif

    INPUT_PRIORITY_NUMS     ,//=(Input_Nums+ENABLE_AUTO_SELECT_SOURCE),
} InputPortPriorityType;
#endif

//------------------------------------------

typedef enum
{
    RES_640x350,
    RES_640x400,
    RES_720x400,
    RES_640x480,
    RES_640x500,
    RES_800x600,
    RES_832x624,
    RES_720x480I,
    RES_720x576I,
    RES_720x480P,
    RES_640x480P,
    RES_720x576P,
    RES_1280x720P_50,
    RES_1280x720P_60,
    RES_1920x1080_24,
    RES_1920x1080_25,
    RES_1920x1080_30,
    RES_1920x1080_50,
    RES_1920x1080_60,
    RES_1024x768,
    RES_1280x1024,
    RES_1600x1200,
    RES_1152x864,
    RES_1152x870,
    RES_1152x900,
    RES_1176x664,
    RES_1280x960,
    RES_1280x720,
    RES_1280x768,
    RES_1280x800,
    RES_1360x768,
    RES_1400x1050,
    RES_1440x900,
    RES_1680x1050,
    RES_720x576,
    RES_848x480,
    RES_1600x1000,
    RES_1920x1200,
    RES_1366x768,
    RES_1600x900,
    RES_1600x1280,
    RES_1792x1344,
    RES_1920x1440,
    RES_1856x1392,
    RES_2048x1152,
    RES_2048x1280,
    RES_2560x1440,
    RES_1800x1400,
    RES_2048x1536,
    RES_2560X1080,
    
    RES_2560X1600,
    RES_1920x2160,
    RES_3840x2160,

    RES_4096x2160,

 #if ENABLE_3D_FUNCTION
   RES_1280x720P_FP,
    RES_1920x1080_3D,
#endif
    MAX_RES
} ResolutionType;

typedef enum
{
    MD_640X350_70,        //0
    MD_640X350_85,        ///1
    MD_640X400_70,       // 2
    MD_720X400_70,        ///3
    MD_640X480_60,        ///4 
    
    MD_640X480_66,        //5
    MD_640X480_72,        //6
    MD_640X480_75,        //7
    MD_800X600_56,        //8
    MD_800X600_60,        //9
    
    MD_800X600_72,        //10
    MD_800X600_75,        //11
    MD_832X624_75,        //12
    MD_1024X768_60,       //13
    MD_1024X768_60MAC,    //14
    
    MD_1024X768_70,       //15
    MD_1024X768_72,       //16
    MD_1024X768_75,       //17
    MD_1024X768_75HPVP,   //18
    MD_1152X864_60,       //19
    
    MD_1152X864_70_1,     //20
    MD_1152X864_70,       //21
    MD_1152X864_75,       //22
    MD_1152X864_75_1,     //23
    MD_1152X870_75_1,     //24
    
    MD_1152X870_75,       //25
    MD_1152X900_66,       //26
    MD_1152X900_76,       //27
    MD_1280X960_60,       //28
    MD_1280X1024_60,      //29

    MD_1280X1024_70,      //30
    MD_1280X1024_70_1,    //31
    MD_1280X1024_72,      //32
    MD_1280X1024_75,      //33
    MD_1280X1024_76,      //34
    
    MD_1600X1200_60,      //35
    MD_1600X1200_60R,     //36
    MD_1280X720_60,       //37
    MD_1280X720_60_1,     //38
    MD_1280X720_70,       //39
    
    MD_1280X720_72,       //40
    MD_1280X720_75,       //41
    MD_1280X768_60R,      //42
    MD_1280X768_60,       //43
    MD_1280X768_60_1,     //44
    
    MD_1280X768_75,       //45
    MD_1280X800_60_1,     //46
    MD_1360X768_60,       //47
    MD_1400X1050_60R,     //48
    MD_1400X1050_60,      //49
    
    MD_1400X1050_75,      //50
    MD_1440X900_60R,      //51
    MD_1440X900_60,       //52
    MD_1680X1050_60R,     //53
    MD_1680X1050_60,      //54
    
    MD_720X576_60,        //55
    MD_848X480_59,        //56
    MD_848X480_70,        //57
    MD_848X480_72,        //58
    MD_848X480_75,        //59
    
    MD_848X480_85,        //60
    MD_848X480_60,        //61
    MD_1280X800_60,       //62
    MD_800x600_67,        //63
    MD_720X480I_60,       //64
    
    MD_720X576I_50,       //65
    MD_720X480P_60,       //66
    MD_640X480P_60,       //67
    MD_720X576P_50,       //68
    MD_1280X720P_50,      //69
    
    MD_1280X720P_60,      //70
    MD_1920X1080I_50,     //71
    MD_1920X1080I_60,     //72
    MD_1920X1080P_50,     //73
    MD_1920X1080P_60,     //74
    
    MD_800X600_70,        //75
    MD_1280X1024_72_1,    //76
    MD_1600X1000_60R,     //77
    MD_640X500_58,        //78
    MD_1176X664_60,       //79
    
    MD_1920X1200_50R,     //80
    MD_1920X1200_60R,     //81
    MD_1440X900_75,       //82
    MD_1280X800_60R,      //83
    MD_1280X800_75,       //84
    
    MD_1600X1000_60,      //85
    MD_1280x800_70,       //86
    MD_1280X800_72,       //87
    MD_1366X768_60,       //88
    MD_1680X1050_75R,     //89
    
    MD_1920X1080_60R,     //90
    MD_1920X1080_60H,     //91
    MD_1920X1080_50,      //92
    MD_1600X900_60R,      //93
    MD_1600X900_50,       //94
    
    MD_1600X900_60_1,     //95
    MD_1600X1000_75,      //96
    MD_1600X1200_66,      //97
    MD_1600X1200_70,      //98
    MD_1600X1200_75,      //99
    
    MD_1600X1200_75_1,    //100
    MD_1600X1280_60,      //101
    MD_1680X1050_75,      //102
    MD_1792X1344_60,      //103
    MD_1920X1080_60,      //104
    
    MD_1920X1080_75,      //105
    MD_1920X1200_60,      //106
    MD_1920X1200_75,      //107
    MD_1920X1440_60R,     //108
    MD_1920X1440_60,      //109
    
    MD_1856X1392_60,      //110
    MD_1856X1392_60_1,    //111
    MD_1600X900_60,       //112
    MD_832X624_75_1,      //113
    MD_640X480_73,        //114
    
    MD_1920X1080I_50_2,   //115
    MD_640X500_58_1,      //116
    MD_640X350_50,        //117
    MD_2048X1152_60R,     //118
    MD_2048X1152_60_1,    //119
    
    MD_2048X1152_60,      //120
    MD_2048X1280_60,      //121
    MD_2560x1080_60,      // 122
    MD_2560x1440_60,      //123
    MD_2560x1600_60,      //124
    
    MD_640X400_85,        //125
    MD_640x480_85,        //126
    MD_720x400_85,        //127
    MD_1280x768_85,       //128
    MD_1920x1080_24,      //129
    
    MD_1920x1080_25,      //130
    MD_1920x1080_30,      //131
    MD_1800x1400,         //132
    MD_2048x1536,         //133
    MD_1920x2160_60,      //134
    
    MD_3840x2160_30,   //135
    MD_3840x2160_60,  //136
    MD_4096x2160_60,  //
	
#if ENABLE_3D_FUNCTION
    MD_1920x1080_24FP,    //132
    MD_1280x720p_60_FP,   //133
    MD_1280x720p_50_FP,   //134
    MD_1920x1080_120NV2D, //135
    MD_1920x1080_110NV2D, //136
    MD_1920x1080_100NV2D, //137
    MD_1920x1080_120NV3D, //138
    MD_1920x1080_110NV3D, //139
    MD_1920x1080_100NV3D, //140
#endif
MD_MAX
} StandardModeType;

// HpVp->3,  HpVn->2, HnVp->1, HnVn->0
#define SYNC_HP_VP      8  // Sync Polarity
#define SYNC_HP_VN      4
#define SYNC_HN_VP      2
#define SYNC_HN_VN      1

typedef struct
{
    BYTE fFlags;
    WORD HPeiod;
    WORD VTotal;
    WORD VFreq;
    BYTE ModeIndex;
    WORD AutoHstart; // for digital port valid timing detect
    WORD AutoVstart; // for digital port valid timing detect
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
} InputModeType;

_MODEDEC_ WORD GetStandardModeWidth( BYTE InputPort );
_MODEDEC_ WORD GetStandardModeHeight( BYTE InputPort );
_MODEDEC_ WORD GetStandardModeGroup( BYTE InputPort );
_MODEDEC_ WORD GetStandardModeHStart( BYTE InputPort );
_MODEDEC_ WORD GetStandardModeVStart( BYTE InputPort );
_MODEDEC_ WORD GetStandardModeHTotal( BYTE InputPort );
_MODEDEC_ WORD GetStandardModeVTotal( BYTE InputPort );
_MODEDEC_ WORD GetStandardModeHFreq( BYTE InputPort );
_MODEDEC_ WORD GetStandardModeVFreq( BYTE InputPort );
_MODEDEC_ Bool mStar_FindMode( BYTE InputPort );
_MODEDEC_ void msFindMode( BYTE InputPort );
_MODEDEC_ Bool IsVideoTiming( BYTE InputPort );

#define MAX_CLOCK(InputPort)   (GetStandardModeHTotal(InputPort)+100) //(STD_MODE_H_TOTAL+100)// original =100
#define MIN_CLOCK(InputPort)   (GetStandardModeHTotal(InputPort)-100) //(STD_MODE_H_TOTAL-100)
//Jison 080731 enabled for all SI
#if 1//(ODM_NAME==ODM_BENQ)||(ODM_NAME==ODM_AOC)   //Jonson. //Super 20080701 for 640x350/70Hz Hposition adjust issue
#define MAX_H_START(InputPort) ((USER_PREF_AUTO_HSTART(InputPort)>50)?(USER_PREF_AUTO_HSTART(InputPort)+50) : USER_PREF_AUTO_HSTART(InputPort)*2)
#define MIN_H_START(InputPort) ((USER_PREF_AUTO_HSTART(InputPort)>50)?(USER_PREF_AUTO_HSTART(InputPort)-50) : 0)
#else
#define MAX_H_START UserPrefAutoHStart+50
#define MIN_H_START UserPrefAutoHStart-50
#endif

#define MAX_V_START(InputPort) (2*USER_PREF_AUTO_VSTART(InputPort)) // Jeff add it in 2006年3月6日下午 03:05:59
#define MIN_V_START(InputPort) 0

#endif

