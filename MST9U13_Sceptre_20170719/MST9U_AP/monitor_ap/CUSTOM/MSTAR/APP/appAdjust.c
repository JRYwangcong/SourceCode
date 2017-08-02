
#define _APPADJUST_C_
#include <string.h>
#include "Global.h"
#include "appAdjust.h"

#define APPADJUST_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && APPADJUST_DEBUG
#define APPADJUST_printData(str, value)     printData(str, value)
#define APPADJUST_printMsg(str)             printMsg(str)
#else
#define APPADJUST_printData(str, value)
#define APPADJUST_printMsg(str)
#endif

//Real Value Functions=======================================================================================================
WORD GetRealVaueFunc(WORD wOsdValue, WORD wOsdMin, WORD wOsdMid, WORD wOsdMax, WORD wRealMin, WORD wRealMid, WORD wRealMax)
{
    WORD wResult=0;

    if(wOsdValue == wOsdMid)
    {
        wResult = wRealMid;
    }
    else if(wOsdValue > wOsdMid)
    {
        wResult = LINEAR_MAPPING_VALUE(wOsdValue, \
            wOsdMid, wOsdMax, \
            wRealMid, wRealMax);
    }
    else
    {
        wResult = LINEAR_MAPPING_VALUE(wOsdValue, \
            wOsdMin, wOsdMid, \
            wRealMin, wRealMid);
    }
    return wResult;
}

//Define-----------------------------------------------------
#define BLACK_LEVEL_OSD_MAX     100
#define BLACK_LEVEL_OSD_MID     DEF_rgbBRIGHTNESS
#define BLACK_LEVEL_OSD_MIN     0

#if ADJ_BackLight_Enable
#if (defined(Bangzi_Project))
#if (COLOR_ACCURACY == COLOR_8_BIT)
#define BLACK_LEVEL_REAL_MAX    147//255
#define BLACK_LEVEL_REAL_MID    128
#define BLACK_LEVEL_REAL_MIN    96//0
#elif (COLOR_ACCURACY == COLOR_10_BIT)
#define BLACK_LEVEL_REAL_MAX    612//1023
#define BLACK_LEVEL_REAL_MID    512
#define BLACK_LEVEL_REAL_MIN    412//0
#endif
#elif (defined(K3L_CrossOver_Project))
#if (COLOR_ACCURACY == COLOR_8_BIT)
#define BLACK_LEVEL_REAL_MAX    158//178
#define BLACK_LEVEL_REAL_MID    128
#define BLACK_LEVEL_REAL_MIN    78
#elif (COLOR_ACCURACY == COLOR_10_BIT)
#define BLACK_LEVEL_REAL_MAX    612//1023
#define BLACK_LEVEL_REAL_MID    512
#define BLACK_LEVEL_REAL_MIN    412//0
#endif

#else
#if (COLOR_ACCURACY == COLOR_8_BIT)
#define BLACK_LEVEL_REAL_MAX    148//255
#define BLACK_LEVEL_REAL_MID    128
#define BLACK_LEVEL_REAL_MIN    96//0
#elif (COLOR_ACCURACY == COLOR_10_BIT)
#define BLACK_LEVEL_REAL_MAX    612//1023
#define BLACK_LEVEL_REAL_MID    512
#define BLACK_LEVEL_REAL_MIN    412//0
#endif
#endif

#else

#if (COLOR_ACCURACY == COLOR_8_BIT)
#define BLACK_LEVEL_REAL_MAX    255
#define BLACK_LEVEL_REAL_MID    128
#define BLACK_LEVEL_REAL_MIN    0
#elif (COLOR_ACCURACY == COLOR_10_BIT)
#define BLACK_LEVEL_REAL_MAX    1023
#define BLACK_LEVEL_REAL_MID    512
#define BLACK_LEVEL_REAL_MIN    0
#endif

#endif

//-----------------------------------------------------------
WORD GetRealBlackLevel(WORD u16OsdValue)
{
    WORD u16Result=0;

    u16Result = GetRealVaueFunc(u16OsdValue, BLACK_LEVEL_OSD_MIN, BLACK_LEVEL_OSD_MID, BLACK_LEVEL_OSD_MAX,\
        BLACK_LEVEL_REAL_MIN, BLACK_LEVEL_REAL_MID, BLACK_LEVEL_REAL_MAX);

    return u16Result;
}

//Define-----------------------------------------------------
//#define RGB_GAIN_OSD_MAX     0xDB//255//255
//#define RGB_GAIN_OSD_MID     0xDB/2//128//0xDA/2//128
//#define RGB_GAIN_OSD_MIN     0

#if (COLOR_ACCURACY == COLOR_8_BIT)
#define RGB_GAIN_REAL_MAX    250
#define RGB_GAIN_REAL_MID    128
#define RGB_GAIN_REAL_MIN    0
#elif (COLOR_ACCURACY == COLOR_10_BIT)
#define RGB_GAIN_REAL_MAX    1023
#define RGB_GAIN_REAL_MID    512
#define RGB_GAIN_REAL_MIN    0
#endif
//-----------------------------------------------------------
WORD GetRealRgbGain(WORD u16OsdValue)
{
    WORD u16Result=0;

    //u16Result = GetRealVaueFunc(u16OsdValue, RGB_GAIN_OSD_MIN, RGB_GAIN_OSD_MID, RGB_GAIN_OSD_MAX,\
    //    RGB_GAIN_REAL_MIN, RGB_GAIN_REAL_MID, RGB_GAIN_REAL_MAX);
     u16Result= u16OsdValue;

    return u16Result;
}

//Define-----------------------------------------------------
#define CONTRAST_OSD_MAX     100
#define CONTRAST_OSD_MID     DEF_CONTRAST
#define CONTRAST_OSD_MIN     0

#if (COLOR_ACCURACY == COLOR_8_BIT)

#if (defined(LeXingEnMa_Project))
#if (PanelType==PanelLC490EQE_XGF1)
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x10
#elif (PanelType==PanelV400DK1_KS1)
#if (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x75//0x80
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x10
#else
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x10
#endif
#elif (PanelType==PanelLC430EQE_FHM2)
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x79
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x10
#elif (PanelType==PanelCMI28UltraHD)
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x6C//0x70
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x80
#define VDC_REAL_MIN    0x10
#elif (PanelType==PanelM320QAN01)
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x73//0x77//0x80
//#define PCC_REAL_MID    0x7A////20151201 Special Setting
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x10
#else
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x10

#endif

#elif (defined(KunDe_Project))
//PC Contrast
#define PCC_REAL_MAX    0x9D//0xA0
#define PCC_REAL_MID    0x7A//0x80
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x10
#elif(defined(SenYo_Project))
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x3C
//Video Contrast
#define VDC_REAL_MAX    0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x40

#elif (defined(FengYuan_Project))
#if 1//(defined(Korean_HYUNDAI_Project))
//PC Contrast
#define PCC_REAL_MAX    0x91//0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0x98//0xA0//0xAA
#define VDC_REAL_MID    0x82
#define VDC_REAL_MIN    0x10
#else
//PC Contrast
#define PCC_REAL_MAX    0x91//0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xA0//0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x10
#endif
#elif (defined(LianHeChuangXin_Project))

#if (defined(Bangzi_Project))
//PC Contrast
#define PCC_REAL_MAX    0x96//0xF8
#define PCC_REAL_MID    0x81
#define PCC_REAL_MIN    0x30
//Video Contrast
#define VDC_REAL_MAX    0x96//0xAA
#define VDC_REAL_MID    0x81
#define VDC_REAL_MIN    0x30
#else
//PC Contrast
#define PCC_REAL_MAX    0x96//0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x30
//Video Contrast
#define VDC_REAL_MAX    0x96//0xAA
#define VDC_REAL_MID    0x80
#define VDC_REAL_MIN    0x30
#endif

#elif(defined(KTC_Project))
//PC Contrast
#define PCC_REAL_MAX    0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x20
//Video Contrast
#define VDC_REAL_MAX   0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x20


#else

//PC Contrast
#define PCC_REAL_MAX    0x98//0xF8
#define PCC_REAL_MID    0x80
#define PCC_REAL_MIN    0x20
//Video Contrast
#define VDC_REAL_MAX    0x90//0xAA
#define VDC_REAL_MID    0x88
#define VDC_REAL_MIN    0x20
#endif

#elif (COLOR_ACCURACY == COLOR_10_BIT)
//PC Contrast
#define PCC_REAL_MAX    0x11D
#define PCC_REAL_MID    0x100
#define PCC_REAL_MIN    0x10
//Video Contrast
#define VDC_REAL_MAX    0xF5
#define VDC_REAL_MID    0xDC
#define VDC_REAL_MIN    0x10
#endif
//-----------------------------------------------------------
WORD GetRealPcContrast(WORD u16OsdValue)
{
    WORD u16Result=0;

    u16Result = GetRealVaueFunc(u16OsdValue, CONTRAST_OSD_MIN, CONTRAST_OSD_MID, CONTRAST_OSD_MAX,\
        PCC_REAL_MIN, PCC_REAL_MID, PCC_REAL_MAX);

    return u16Result;
}
WORD GetRealVideoContrast(WORD u16OsdValue)
{
    WORD u16Result=0;

    u16Result = GetRealVaueFunc(u16OsdValue, CONTRAST_OSD_MIN, CONTRAST_OSD_MID, CONTRAST_OSD_MAX,\
        VDC_REAL_MIN, VDC_REAL_MID, VDC_REAL_MAX);

    return u16Result;
}

//Define-----------------------------------------------------
#define SATURATION_OSD_MAX     100//255//255
#define SATURATION_OSD_MID     50//128//0xDA/2//128
#define SATURATION_OSD_MIN     0

#define SATURATION_REAL_MAX    0xFF
#define SATURATION_REAL_MID    0x80
#define SATURATION_REAL_MIN    0
//-----------------------------------------------------------
WORD GetRealSaturation(WORD u16OsdValue)
{
    WORD u16Result=0;

    u16Result = GetRealVaueFunc(u16OsdValue, SATURATION_OSD_MIN, SATURATION_OSD_MID, SATURATION_OSD_MAX,\
        SATURATION_REAL_MIN, SATURATION_REAL_MID, SATURATION_REAL_MAX);

    return u16Result;
}




//////////////////////////////
//-----------------------------------------------------------
#define PWMValue(n,MinValue,MaxValue)  (MinValue+(WORD)((((MaxValue-MinValue)*n)/9)))

//#define PWMValue(n,MinValue,MaxValue)  (MinValue+((WORD) (MaxValue-MinValue)/9)*n)

#if (PanelType==PanelLTM340YP01)
#if (defined(GV3B_Project))
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xE8),
(0xFF-0xE2),
(0xFF-0xDC),
(0xFF-0xD4),
(0xFF-0xC9),
(0xFF-0xBB),
(0xFF-0xAA),
(0xFF-0x92),
(0xFF-0x70),
(0xFF-0x40),
(0xFF-0x00),

};
#elif (defined(Bangzi_Project))
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xE8),
(0xFF-0xE2),
(0xFF-0xDC),
(0xFF-0xD4),
(0xFF-0xC9),
(0xFF-0xBB),
(0xFF-0xAA),
(0xFF-0x92),
(0xFF-0x70),
(0xFF-0x40),
(0xFF-0x00),

};

#else
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xEA),
(0xFF-0xE4),
(0xFF-0xDC),
(0xFF-0xD4),
(0xFF-0xC9),
(0xFF-0xBB),
(0xFF-0xAA),
(0xFF-0x92),
(0xFF-0x70),
(0xFF-0x40),
(0xFF-0x00),


//with audio
/*
 14,
 24,
 34,
 44,
 54,
 64,//59,
 74,//69,
 98,//97,
 113,//111,
 172,//151,
 255,
*/


/*
 9,
 19,
 29,
 39,
 49,
 60,//59,
 70,//69,
 98,//97,
 113,//111,
 172,//151,
 255,
*/

};
#endif
#elif (PanelType == PanelT430QVN01)
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xEE),
(0xFF-0xE6),
(0xFF-0xDF),
(0xFF-0xD7),
(0xFF-0xCC),
(0xFF-0xC0),
(0xFF-0xB0),
(0xFF-0x94),
(0xFF-0x71),
(0xFF-0x49),
(0xFF-0x00),
};
#elif (PanelType == PanelM350DVR01_01)
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xED),
(0xFF-0xE3),
(0xFF-0xDB),
(0xFF-0xD2),
(0xFF-0xC8),
(0xFF-0xBB),
(0xFF-0xAD),
(0xFF-0x99),
(0xFF-0x7F),
(0xFF-0x52),
(0xFF-0x00),
};
#elif (PanelType==PanelM350DVA01_1)
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xED),
(0xFF-0xE3),
(0xFF-0xDB),
(0xFF-0xD2),
(0xFF-0xC8),
(0xFF-0xBB),
(0xFF-0xAD),
(0xFF-0x99),
(0xFF-0x7F),
(0xFF-0x52),
(0xFF-0x00),
};


#elif (PanelType == PanelLSM315DP01)
#if (defined(K3L_CrossOver_Project))
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xED),
(0xFF-0xE4),
(0xFF-0xDB),
(0xFF-0xD1),
(0xFF-0xC5),
(0xFF-0xB6),
(0xFF-0xA4),
(0xFF-0x8C),
(0xFF-0x48),
(0xFF-0x2D),
(0xFF-0x00),
};
#elif (defined(GK3DR_AIO_Project))
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xE0),
(0xFF-0xD0),
(0xFF-0xC0),
(0xFF-0xB0),
(0xFF-0xA0),
(0xFF-0x90),
(0xFF-0x80),
(0xFF-0x70),
(0xFF-0x60),
(0xFF-0x3D),
(0xFF-0x00),
};
#else
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xED),
(0xFF-0xE4),
(0xFF-0xDB),
(0xFF-0xD1),
(0xFF-0xC5),
(0xFF-0xB6),
(0xFF-0xA4),
(0xFF-0x8C),
(0xFF-0x6B),
(0xFF-0x3D),
(0xFF-0x00),
};
#endif
#elif (PanelType == PanelLSM270DP01)
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xE7),
(0xFF-0xE4),
(0xFF-0xDB),
(0xFF-0xD1),
(0xFF-0xC5),
(0xFF-0xB6),
(0xFF-0xA4),
(0xFF-0x8C),
(0xFF-0x6B),
(0xFF-0x3D),
(0xFF-0x00),
};
#elif (PanelType == PanelLSM315HP01)
#if (defined(GK3B_Project))
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xB0),
(0xFF-0xA2),
(0xFF-0x93),
(0xFF-0x83),
(0xFF-0x73),
(0xFF-0x63),
(0xFF-0x51),
(0xFF-0x3F),
(0xFF-0x2C),
(0xFF-0x1C),
(0xFF-0x00),
};
#else
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xFF),
(0xFF-0xF9),
(0xFF-0xEE),
(0xFF-0xC3),
(0xFF-0xA9),
(0xFF-0x99),
(0xFF-0x7D),
(0xFF-0x58),
(0xFF-0x3A),
(0xFF-0x20),
(0xFF-0x00),
};
/*
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xEE),
(0xFF-0xD8),
(0xFF-0xC3),
(0xFF-0xAD),
(0xFF-0x96),
(0xFF-0x7E),
(0xFF-0x67),
(0xFF-0x4B),
(0xFF-0x32),
(0xFF-0x17),
(0xFF-0x00),
};
*/
#endif
#elif PanelType == PanelLM270WR2
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xFF),
(0xFF-0xE0),
(0xFF-0xC3),
(0xFF-0xAD),
(0xFF-0x96),
(0xFF-0x7E),
(0xFF-0x67),
(0xFF-0x4B),
(0xFF-0x32),
(0xFF-0x17),
(0xFF-0x00),
};
#elif PanelType == PanelLSM270HP06
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
(0xFF-0xE6),
(0xFF-0xDD),
(0xFF-0xD2),
(0xFF-0xC6),
(0xFF-0xB8),
(0xFF-0xA6),
(0xFF-0x93),
(0xFF-0x7A),
(0xFF-0x59),
(0xFF-0x2D),
(0xFF-0x00),
};

#else
WORD code BrightnessTbl[]=
{
//0     1     2     3     4     5     6     7     8     9   
 0, 
 25,
 50,
 75,
 100,
 126,
 152,
 177,
 202,
 228,
 255,
};
#endif

WORD APPAdjustBrightness(BYTE u8Brightness)
{
    WORD u16Result=0;
  #if (defined(LianHeChuangXin_Project))
    BYTE  MaxValue ,MinValue;
 #endif
 
	if (u8Brightness > 100)
		u8Brightness = 100;
	else if (u8Brightness < 0)
		u8Brightness = 0;
	
#if (DEF_BRIGHTNESS==100)
	    u16Result=((DWORD)u8Brightness*(BRIGHTNESS_REAL_MAX-BRIGHTNESS_REAL_MIN))/(DEF_BRIGHTNESS)+BRIGHTNESS_REAL_MIN;
#else

  #if (defined(LianHeChuangXin_Project))
  
        MinValue=(BYTE)(u8Brightness/10);
        MaxValue=((BYTE)(u8Brightness/10))+1;
        u16Result=PWMValue((u8Brightness%10), BrightnessTbl[MinValue], BrightnessTbl[MaxValue]);

//printData("u16Result u16Result==%x",u16Result);

  #else
      if(u8Brightness>DEF_BRIGHTNESS)
        {
	     u16Result=((DWORD)(u8Brightness-DEF_BRIGHTNESS)*(BRIGHTNESS_REAL_MAX-BRIGHTNESS_REAL_MID))/(100-DEF_BRIGHTNESS)+BRIGHTNESS_REAL_MID;
        }
      else
       {
	    u16Result=((DWORD)u8Brightness*(BRIGHTNESS_REAL_MID-BRIGHTNESS_REAL_MIN))/(DEF_BRIGHTNESS-0)+BRIGHTNESS_REAL_MIN;
       }
   #endif	  
#endif

    //printData("u16Resultu16Resultu16Result ==%d",u16Result);

    return u16Result;

/*
    if(u8Brightness<=75)
        u8Brightness=(DWORD)u8Brightness*(USER_PREF_DEF_SUBBRIGHTNESS-FACTORY_DEF_BRIGHTNESS_MIN)/75+FACTORY_DEF_BRIGHTNESS_MIN;
    else
        u8Brightness=((DWORD)(u8Brightness-75)*(FACTORY_DEF_BRIGHTNESS_MAX-USER_PREF_DEF_SUBBRIGHTNESS)/25)+USER_PREF_DEF_SUBBRIGHTNESS;
    //APPADJUST_printData("u8Brightness = %d", u8Brightness);
*/
}

//////////////////////////////
#if 0//AudioFunc
#if AdjustVolume_UseTable
  #if USE_DAC_ADJ
    BYTE code VolumeValueTable[]=
    {
    0x00,0x05,0x05,0x05,0x06,0x06,0x07,0x07,0x07,0x08,
    0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,  //10
    0x12,0x12,0x12,0x13,0x14,0x15,0x16,0x17,0x17,0x18,  //20
    0x18,0x18,0x19,0x19,0x19,0x1A,0x1A,0x1A,0x1A,0x1B,  //30
    0x1B,0x1B,0x1C,0x1C,0x1C,0x1D,0x1D,0x1E,0x1F,0x20,  //40
    0x20,0x21,0x21,0x21,0x22,0x22,0x22,0x23,0x23,0x23,  //50
    0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2C,  //60
    0x2D,0x2D,0x2E,0x2E,0x2F,0x30,0x31,0x31,0x32,0x33,  //70
    0x33,0x34,0x34,0x35,0x35,0x36,0x37,0x38,0x38,0x39,  //80
    0x3A,0x3B,0x3B,0x3C,0x3C,0x3D,0x3D,0x3E,0x3E,0x3F,  //90
    0x3F,                                               //100
    };
  #else
    BYTE code VolumeValueTable[]=
    {
    0x00,0x20,0x23,0x24,0x26,0x27,0x29,0x2A,0x2C,0x2D,
    0x30,0x32,0x35,0x38,0x3B,0x3E,0x41,0x44,0x47,0x4A,  //10
    0x4C,0x50,0x55,0x59,0x5E,0x62,0x67,0x6C,0x70,0x75,  //20
    0x79,0x7C,0x7F,0x81,0x84,0x86,0x89,0x8B,0x8E,0x90,  //30
    0x94,0x97,0x9A,0x9D,0xA0,0xA3,0xA6,0xA9,0xAB,0xAD,  //40
    0xB0,0xB1,0xB3,0xB4,0xB6,0xB7,0xB9,0xBA,0xBC,0xBE,  //50
    0xC0,0xC1,0xC3,0xC4,0xC6,0xC7,0xC9,0xCA,0xCC,0xCE,  //60
    0xD0,0xD1,0xD3,0xD4,0xD5,0xD7,0xD9,0xDA,0xDC,0xDE,  //70
    0xDB,0xDC,0xDD,0xDE,0xDF,0xE0,0xE1,0xE2,0xE3,0xE4,  //80
    0xE5,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,  //90
    0xEE,                                               //100
    };
  #endif
#endif

void APPAdjustVolume(BYTE u8Volume)
{
#if AdjustVolume_UseTable
    u8Volume=VolumeValueTable[u8Volume];
#else
  #define Volume50Value     ((MaxVolumeValue-MinVolumeValue)*70/100+MinVolumeValue)
    if(u8Volume >= 50)
        u8Volume=((DWORD)u8Volume * (MaxVolumeValue - Volume50Value)) / 50 + 2 * Volume50Value - MaxVolumeValue;
    else
        u8Volume=((DWORD)u8Volume * (Volume50Value - MinVolumeValue)) / 50 + MinVolumeValue;
  #undef Volume50Value
#endif
    //APPADJUST_printData("u8Volume = %d", u8Volume);
}
#endif
//////////////////////////////

//Define-----------------------------------------------------
#if (AudioFunc&&(!AudioNewCurve))
#define VOLUME_REAL_MAX    100
#define VOLUME_REAL_MID    50
#define VOLUME_REAL_MIN    0
//-----------------------------------------------------------
WORD GetRealVolume(WORD u16OsdValue)
{
    WORD u16Result=0;

    u16Result = GetRealVaueFunc(u16OsdValue, VOLUME_OSD_MIN, VOLUME_OSD_MID, VOLUME_OSD_MAX,\
    VOLUME_REAL_MIN, VOLUME_REAL_MID, VOLUME_REAL_MAX);

    return VOLUME_REAL_MAX-u16Result;
}
#endif

#define VOLUME_REAL_PWM_MAX    0xFF
#define VOLUME_REAL_PWM_MID    0x80
#define VOLUME_REAL_PWM_MIN    0
//-----------------------------------------------------------
WORD GetRealPWMVolume(WORD u16OsdValue)
{
    WORD u16Result=0;

    u16Result = GetRealVaueFunc(u16OsdValue, VOLUME_OSD_MIN, VOLUME_OSD_MID, VOLUME_OSD_MAX,\
    VOLUME_REAL_PWM_MIN, VOLUME_REAL_PWM_MID, VOLUME_REAL_PWM_MAX);

    return u16Result;
}




