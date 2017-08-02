#ifndef _APPWINDOW_H_
#define _APPWINDOW_H_

#ifdef _APPWINDOW_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif

#define APP_INPUT_TIMING_TOL_DE         5
#define APP_INPUT_TIMING_TOL_VFREQ      20

typedef enum
{
    eMULTIWIN_OFF,
    eMULTIWIN_2_PIP,        //PIP
    eMULTIWIN_2_POP1,       //PBP side by side
#if Enable_Multi_Win3
    eMULTIWIN_3,            // 3 window
#endif    
#if Enable_Multi_Win4    
    eMULTIWIN_4,            // 4 window
#endif
    eMULTIWIN_MAX,
    eMULTIWIN_2_POP2,       // PBP Top bottom
#if !Enable_Multi_Win3
    eMULTIWIN_3,            // 3 window
#endif    
#if !Enable_Multi_Win4    
    eMULTIWIN_4,            // 4 window
#endif
    
}eMULTIWIN_Type;

typedef enum
{
    ePIPSIZE_SMALL,
    ePIPSIZE_MIDDLE,
    ePIPSIZE_LARGE,

    ePIPSIZE_MAX,
    ePIPSIZE_HUGE,
    
}ePIPSIZE_Type;

typedef enum
{
    PIPPosition_Top_Right,
    PIPPosition_Top_Left,
    PIPPosition_Bottom_Right,
    PIPPosition_Bottom_Left,
    PIPPosition_Max,
    #if MultiwindowON_NearCenter
   	 PIPPosition_Offset=50,
    #else
    	PIPPosition_Offset=10,
    #endif
}PIPPosition;



typedef enum
{
    eASPECT_FULL,
    eASPECT_4_3,
#if Aspect_Auto_Enable
    eASPECT_NATIVE,
#endif
#if Aspect_1_1_Enable
    eASPECT_1_1,
#endif
    eASPECT_MAX,

    eASPECT_5_4,
    eASPECT_16_9,
#if !Aspect_Auto_Enable
    eASPECT_NATIVE,
#endif
#if !Aspect_1_1_Enable
    eASPECT_1_1,
#endif    
	
}eASPECT_TYPE;



enum
{
    EarPhone,
#if Enable_SPDIF		
    SPDIF_OUT,
#endif    
    Speaker,
    OutputNums,
#if !Enable_SPDIF		
    SPDIF_OUT,
#endif    
};


typedef struct
{
    WORD    dummy0;         /* dummy for alignment */
    WORD    magic_num;
    DWORD   file_size;
    WORD    dummy1;
    WORD    dummy2;
    DWORD   offset;
    DWORD   header_size;
    DWORD   width;
    DWORD   height;
    WORD    plane;
    WORD    bp;
    DWORD   compression;
    DWORD   data_size;
    DWORD   h_resolution;
    DWORD   v_resolution;
    DWORD   palette_num;
    DWORD   important;

} _sBMPType;

INTERFACE void APPWindow_Init( void );
INTERFACE void APPWindow_Handler( void );
INTERFACE void APPWindow_Configure(eMULTIWIN_Type eType, ePIPSIZE_Type ePipType);
INTERFACE void APPWindow_SetPosition(BYTE u8DispWin, WORD u16HP, WORD u16VP);
INTERFACE void APPWindow_SetSize(BYTE u8DispWin, WORD u16HP, WORD u16VP, WORD u16Width, WORD u16Heigh);
INTERFACE void APPWindow_SetColorSpace(BYTE u8DispWin);
INTERFACE void APPWindow_CheckColorSpaceChange( void );
#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
INTERFACE void APPWindow_CheckRangeFormatChange(void);
INTERFACE void APPWindow_CheckColorimetryChange(void);
#endif
INTERFACE void APPWindow_FlyWindow(void);
INTERFACE void APPWindow_FadingWindow(void);
INTERFACE void APPWindow_DrawBMP(BYTE u8WinIndex);
INTERFACE void APPWindow_DrawJPG(BYTE u8WinIndex);
INTERFACE WORD GetPipWindowWidth(BYTE u8PipSize);
INTERFACE WORD GetPipWindowHeight(BYTE u8PipSize);
INTERFACE void APPWindow_SetCurFrameMode( BYTE ucWinIndex);

#undef INTERFACE
#endif


