#ifndef _MS_SCALER_H_
#define _MS_SCALER_H_

#ifdef _MS_SCALER_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif

#if ENABLE_HV_TOLERANCE_ENLARGE
#define H_IntTorlance     5      //0~0x0F
#define V_IntTorlance     5    //0~0x3F
#else
#define H_IntTorlance     5     //0~0x0F
#define V_IntTorlance     1     //0~0x3F
#endif

#define HSC_offset                  1
#define NUM_ROT_LB                  2112 // MST9U1,2
#if ((CHIP_ID==MST9U3) || (CHIP_ID==MST9U4))
#define ENABLE_SUBSC_HOLLOW_OUT     (0)
#else
#define ENABLE_SUBSC_HOLLOW_OUT     (1 && (MST9U_ASIC_1P==0)) //hollow out the overlap region by using sub scaler for BW reduction.
#endif
#define ENABLE_SUBSCIN              (1 || ENABLE_SUBSCOUT) // out implies in
#define ENABLE_SUBSCOUT             (1)
#if(ENABLE_SUBSCOUT)
#define ENABLE_MWE                  0
#else
#define ENABLE_MWE                  0
#endif

#define ENABLE_ROT_2SCIP            (1 && ( (CHIP_ID==MST9U) || (CHIP_ID==MST9U2))) // Rotate uses 2 SC to capture when H (NO H-pre scaling)  > NUM_ROT_LB
#define ENABLE_ROT_2SCOP            ((MST9U_ASIC_1P==0) && (CHIP_ID==MST9U)) // force rotate using 2sc (both L or R) for output post h down

//CSC Selection
#define IP_CSC_MATRIX           0
#define ACE_CSC_MATRIX          1

#if (ENABLE_MWE)
#define CSC_MATRIX_SEL          IP_CSC_MATRIX//ACE_CSC_MATRIX
#else
#define CSC_MATRIX_SEL          IP_CSC_MATRIX
#endif

//Y Sub 16
#define DLC_Y_ADJUST            0
#define ACE_Y_SUB               1

#define YSUB_16_SEL            DLC_Y_ADJUST


#define SC_GUARD_BAND           64      // 8 tap * 8x scaling up
#define SC_GUARD_BAND_PQ        16
#if 0//EDID_3440x1440
#define DFT_HBR2_GUARDBAND      32//   NEW 16 for 533.250 MHz
#else
#define DFT_HBR2_GUARDBAND      16//32   NEW 16 for 533.250 MHz
#endif

#define SC_FORCE_CAP_ALIGN      64      // Should be <= DFT_HBR2_GUARDBAND*2
#if (CHIP_ID==MST9U2) || (CHIP_ID==MST9U3)
#define SC_ROT_V_ALIGN          0xFFF8  // v pre down to 8 lines alignment
#else
#define SC_ROT_V_ALIGN          0xFFFF  // no v pre down in ROT ==> no alignment
#endif

#if ((SC_FORCE_CAP_ALIGN <= (DFT_HBR2_GUARDBAND*2))||EDID_3440x1440)
#define SC_FORCE_CAP_ALIGN_EN          1
#else
#define SC_FORCE_CAP_ALIGN_EN          0
#endif

typedef enum
{
    SC_FMT_IN_NORMAL,
    SC_FMT_IN_3D_PF,        /* page flipping */
    SC_FMT_IN_3D_FP,        /* frame packing */
    SC_FMT_IN_3D_FPI,       /* frame packing interlace */
    SC_FMT_IN_3D_TB,        /* top bottom */
    SC_FMT_IN_3D_SBSH,      /* side by side half */
    SC_FMT_IN_3D_LA,        /* line interleave */
    SC_FMT_IN_3D_SBSF,      /* side by side full */
    SC_FMT_IN_3D_CB,        /* chess board */
    SC_FMT_IN_3D_FA,        /* filed alternative */
    SC_FMT_IN_3D_PI,        /* pixel interleave */
}EN_3D_FMT_IN;

typedef enum
{
    SC_FMT_OUT_3D_FS,       /* frame sequential */
    SC_FMT_OUT_3D_PSV,      /* passive */
    SC_FMT_OUT_3D_PSV_4M,   /* 4M embedded passive */
    SC_FMT_OUT_3D_TB,       /* top and bottom */
    SC_FMT_OUT_3D_SBS,      /* side by side */
    SC_FMT_OUT_3D_CB,   /* chess board */
}EN_3D_FMT_OUT;

/// Define scaler status
typedef enum
{
    SC_STATUS_IDLE = 0,             // idle

    SC_STATUS_FREERUN,
    SC_STATUS_UI_SETUP,             // UI Init
    SC_STATUS_WINDOW_SETUP,          // , over/under scan, asp ratio calculation by Customer spec
    SC_STATUS_WINDOW_UNSUPPORT,     // Unsupport Mode
    SC_STATUS_OUTPUT_SETUP,         // msScalerSetup(), input stable sync, under setup
    SC_STATUS_OUTPUT_SETUPOK,       // msScalerSetup(), input stable sync, scaler setup ok, not display yet
    SC_STATUS_HOLLOW,
}EN_SCALER_DISP_STATUS;

// Define h guardband
typedef struct
{
    BYTE u8Left;                    // left size of guardband
    BYTE u8Right;                   // right size of guardband
}ST_GUARDBAND_INFO;

// Define HBR2 Left/Right mux
enum
{
    HBR2_LEFT = 0,//                      // HBR0 select left mux
    HBR2_RIGHT = 1,//                      // HBR0 select right mux
//    HBR1_Left = 2,                      // HBR1 select left mux
//    HBR1_Right = 3,                      // HBR1 select right mux
};

// Enum of mem rotate mode
enum
{
    ROT_NONE = 0,
    ROT_90,
    ROT_270,
    ROT_H_MIRROR,
    ROT_V_MIRROR,
    ROT_HV_MIRROR,
};

typedef enum
{
    SC_MA_MODE_OFF,
    SC_MA_MODE_ROT,         // PQ_IP_MADi_P_MODE_ROTATE_Main,   MOT_PMODE, OFF, OFF, OFF, OFF, OFF
    SC_MA_MODE_0,           // P_MODE8_444,                     MOT_PMODE, OFF, OFF, OFF, OFF, OFF
    SC_MA_MODE_1,           // P_MODE_MOT8,                     MOT_PMODE, OFF, OFF, OFF, OFF, OFF
    SC_MA_MODE_2,           // P_MODE_MC_3F,                    MOT_PMODE, OFF, OFF, OFF, OFF, OFF
    SC_MA_MODE_3,           // P_MODE_MC_6F_6R,                 MOT_PMODE, OFF, OFF, OFF, OFF, OFF
    SC_MA_MODE_4,           // P_MODE_MC_12F_8F,                MOT_PMODE, OFF, OFF, OFF, OFF, OFF
    SC_MA_MODE_5,           // 25_4R,                           MOT_4R_5,  OFF, OFF, OFF, OFF, OFF
    SC_MA_MODE_6,           // 25_MC_4R,                        MOT_4R_5_MC, M1, DFK1, SST3, OFF, OFF
    SC_MA_MODE_7,           // 25_MC_14F_6R,                    MOT_4R_5MC, M1, OFF, OFF, W2, OFF
    SC_MA_MODE_8            // 25_MC_14F_8R,                    MOT_4R_5MC, M1, OFF, OFF, W2, OFF
} EN_SCALER_MA_MODE;

typedef enum
{
    SC_EO_MODE_OFF,         // OFF, OFF, OFF, OFF
    SC_EO_MODE_0,           // SD_8, OFF, OFF, OFF
    SC_EO_MODE_1            // SD_8, EW1, OFF, OFF
} EN_SCALER_EO_MODE;

typedef enum
{
    SC_MC_MODE_OFF,         // OFF, OFF, OFF
    SC_MC_MODE_0,           // SD2, OFF, ON
    SC_MC_MODE_1,           // SD2, T12, ON_DHD
    SC_MC_MODE_2,           // HD1, OFF, ON_UCNR
    SC_MC_MODE_3            // HD, OFF, ON_UCNR
} EN_SCALER_MC_MODE;

typedef enum
{
    SC_FILM_MODE_OFF,       // OFF, OFF, OFF, OFF
    SC_FILM_MODE_1,         // SD2, OFF, OFF, OFF
    SC_FILM_MODE_2,         // SD2, OFF, SD4, OFF
    SC_FILM_MODE_3,         // SD2, SD5, SD10, SD5
    SC_FILM_MODE_4,         // HD4, OFF, HD_2, OFF
    SC_FILM_MODE_5          // HD4, HD_3_MC, HD_2, OFF
} EN_SCALER_FILM_MODE;

typedef struct
{
    EN_SCALER_MA_MODE   u8MAMode;
    EN_SCALER_EO_MODE   u8EOMode;
    EN_SCALER_MC_MODE   u8MCMode;
    EN_SCALER_FILM_MODE u8FilmMode;
} ST_WIN_PQ_MODE;

typedef enum
{
    INPUTCOLOR_RGB,
    //INPUTCOLOR_YUV,
    INPUTCOLOR_YUV_422,
    INPUTCOLOR_YUV_444,
    INPUTCOLOR_YUV_420,
    INPUTCOLOR_MAX
}EN_InputColorFormatType;

#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
typedef enum
{
    INPUT_RANGE_LIMITED,
    INPUT_RANGE_FULL,
    INPUT_RANGE_MAX
}EN_InputRangeFormatType;

typedef enum
{
    INPUT_COLORIMETRY_ITU601,
    INPUT_COLORIMETRY_ITU709,
    INPUT_COLORIMETRY_NODATA,
    INPUT_COLORIMETRY_MAX
}EN_InputColorimetryType;
#endif

enum // SC_Flags
{
    bFreeRunModeBit   = BIT0,
};

enum{
    MEM_FMT_NO_USE,
    MEM_FMT_422,
    MEM_FMT_444_8,
    MEM_FMT_444_10,
    MEM_FMT_444_12,
    MEM_FMT_444_12_YM
};

enum
{
    SCL0_SUB    =0,                 // L0 - f1 sub det
    SCL0_MAIN     ,                 // L0 - f2 main det
    SCR0_SUB      ,                 // R0 - f1 sub det
    SCR0_MAIN     ,                 // R0 - f2 main det
#if (CHIP_ID==MST9U3)
    SCL0_OFFLINE  ,                 // L0 - f0 offline det
    SC_MAX_NUMS   ,                 // The max sc nums: non scaler
#elif (CHIP_ID==MST9U4)
    SCL0_OFFLINE  ,                 // L0 - f0 offline det
    SC_MAX_NUMS = SCL0_OFFLINE,     // The max sc nums: non scaler
#endif
    SCL1_SUB      ,                 // L1 - f1 sub det
    SCL1_MAIN     ,                 // L1 - f2 main det
    SCR1_SUB      ,                 // R1 - f1 sub det
    SCR1_MAIN     ,                 // R1 - f2 main det
#if (CHIP_ID==MST9U) || (CHIP_ID==MST9U2)
    SCL0_OFFLINE  ,                 // L0 - f0 offline det
    SC_MAX_NUMS   ,                 // The max sc nums: non scaler
#endif
};

/// Define scaler bit index
enum
{
    bSCL0_SUB    =BIT0,             // L0 - f1 sub det
    bSCL0_MAIN   =BIT1,             // L0 - f2 main det
    bSCR0_SUB    =BIT2,             // R0 - f1 sub det
    bSCR0_MAIN   =BIT3,             // R0 - f2 main det
    bSCL1_SUB    =BIT4,             // L1 - f1 sub det
    bSCL1_MAIN   =BIT5,             // L1 - f2 main det
    bSCR1_SUB    =BIT6,             // R1 - f1 sub det
    bSCR1_MAIN   =BIT7,             // R1 - f2 main det
#if (CHIP_ID==MST9U3 )
    bSCL0_OFFLINE=BIT4,             // L0 - f0 offline det
    bSC_MAX_NUMS =BIT5,             // The max sc nums: non scaler
    bSC_MAIN     =BIT1|BIT3,
    bSC_SUB      =BIT0|BIT2
#elif (CHIP_ID==MST9U4)
    bSC_MAX_NUMS =BIT4,             // The max sc nums: non scaler
    bSC_MAIN     =BIT1|BIT3,
    bSC_SUB      =BIT0|BIT2
#else
    bSCL0_OFFLINE=BIT8,             // L0 - f0 offline det
    bSC_MAX_NUMS =BIT9,             // The max sc nums: non scaler
    bSC_MAIN     =BIT1|BIT3|BIT5|BIT7,
    bSC_SUB      =BIT0|BIT2|BIT4|BIT6
#endif
};

/// Define window index
enum
{
    DISPLAY_WIN1     =0,            // Output display window 1
    DISPLAY_WIN2     =1,            // Output display window 2
    DISPLAY_WIN3     =2,            // Output display window 3
    DISPLAY_WIN4     =3,            // Output display window 4
    DISPLAY_MAX_NUMS =4,
};

/*
/// Define window bit index
enum
{
    bDISPLAY_WIN1     =BIT0,        // Output display window 1
    bDISPLAY_WIN2     =BIT1,        // Output display window 2
    bDISPLAY_WIN3     =BIT2,        // Output display window 3
    bDISPLAY_WIN4     =BIT3,        // Output display window 4
    bDISPLAY_MAX_NUMS =BIT4,
};
*/

/// Define SC IP detection Module
enum
{
    SCIP1_0    =0,                 // IP1 Detection Module 0 (DP0)
    SCIP1_1      ,                 // IP1 Detection Module 1 (DP1)
    SCIP1_2      ,                 // IP1 Detection Module 2 (DP2)
    SCIP1_3      ,                 // IP1 Detection Module 3 (DP3)
    SCIP1_4      ,                 // IP1 Detection Module 4 (H0)
    SCIP1_5      ,                 // IP1 Detection Module 5 (H1)
    SCIP1_6      ,                 // IP1 Detection Module 6 (H2)
    SCIP1_7      ,                 // IP1 Detection Module 7 (H3)
    SCIP1_8      ,                 // IP1 Detection Module 8 (VBY1/LVDS)
    SCIP1_MAX_NUMS,                // The max IP1 Detection Modules
    SCIP1_NA   =0xFF,              // Status: IP1 Module Not Assigned on Input Ports
};

/// Define SC IP detection Module BitMask
enum
{
    bSCIP1_0    =BIT0,                 // IP1 Detection Module 0 (DP0)
    bSCIP1_1    =BIT1,                 // IP1 Detection Module 1 (DP1)
    bSCIP1_2    =BIT2,                 // IP1 Detection Module 2 (DP2)
    bSCIP1_3    =BIT3,                 // IP1 Detection Module 3 (DP3)
    bSCIP1_4    =BIT4,                 // IP1 Detection Module 4 (H0)
    bSCIP1_5    =BIT5,                 // IP1 Detection Module 5 (H1)
    bSCIP1_6    =BIT6,                 // IP1 Detection Module 6 (H2)
    bSCIP1_7    =BIT7,                 // IP1 Detection Module 7 (H3)
    bSCIP1_8    =BIT8,                 // IP1 Detection Module 8 (VBY1/LVDS)
    bSCIP1_MAX_NUMS=BIT9,              // The max IP1 Detection Modules
};

/// Define input status
typedef enum
{
    IP_STATUS_IDLE = 0,         // idle

    IP_STATUS_WAITSYNC,         // mStar_MonitorInputTiming(), wait input stable sync
    IP_STATUS_NOSYNC,            // mStar_MonitorInputTiming(),  input stable no sync
    IP_STATUS_FINDMODE,         // mStar_MonitorInputTiming(), input stable sync, start find mode
    IP_STATUS_FINDMODEOK,       // mStar_MonitorInputTiming(), input stable sync, find mode ok
}EN_INPUT_STATUS;

typedef enum // Inputport type, need to sync w/ EN_API_SRC_TYPE
{
    TYPE_ANALOG      = BIT0,
    TYPE_DVI_SINGLE  = BIT1,
    TYPE_DVI_DUAL    = BIT2,
    TYPE_HDMI        = BIT3,
    TYPE_MHL         = BIT4,
    TYPE_DP          = BIT5,
    TYPE_DPTX        = BIT6,
    TYPE_VD          = BIT7,
    TYPE_HBR2_0      = BIT8,
    TYPE_HBR2_1      = BIT9,
    TYPE_HDMI_2_0    = BIT10,
    TYPE_FAKE_DUAL   = BIT11,
    TYPE_FAKE_DUAL_S = BIT12,
    TYPE_DAISY_CHAIN = BIT13,
    TYPE_DP_MST      = BIT14,
    TYPE_VBY1        = BIT15,
    TYPE_LVDS        = BIT16,
    TYPE_RESERVED    = BIT17
}EN_INPUTPORT_TYPE;

typedef enum
{
    SC_SCALING_PRE_H_DOWN       = BIT0,
    SC_SCALING_PRE_V_DOWN       = BIT1,
    SC_SCALING_POST_H_UP        = BIT2,
    SC_SCALING_POST_H_DOWN      = BIT3,
    SC_SCALING_POST_V_UP        = BIT4,
    SC_SCALING_POST_V_DOWN      = BIT5
}EN_SCALING_TYPE;

// Define window info
typedef struct
{
    WORD x;                         // start x of the window
    WORD y;                         // start y of the window
    WORD width;                     // width of the window
    WORD height;                    // height of the window
}ST_WINDOW_INFO;

// Define fake capture window
typedef struct
{
    signed int x;                   // start x of the fake capture window, negative: x is less than 0
    signed int y;                   // start y of the fake capture window, negative: y is less than 0
    WORD width;                     // width of the fake capture window
    WORD height;                    // height of the fake capture window

}ST_FAKE_CAP_WIN_INFO;

// Define image DE offset info
typedef struct
{
    WORD u32OffsetHStart;
    WORD u32OffsetHEnd;
    WORD u32OffsetVStart;
    WORD u32OffsetVEnd;

}ST_IMAGE_DE_Offset;

// Define scaling filter direction
typedef enum
{
    HORIZONTAL,
    VERTICAL,
}EN_DIRECTION;

// Define scaling filter mode
typedef enum
{
    BYPASS,
    BILINEAR,
    BICUBIC,
    Y_6_TAP,
    Y_8_TAP,
}
EN_SCALING_FILTER_MODE;

// Define scaling filter SRAM
typedef enum
{
    Y_SRAM0,
    Y_SRAM1,
    C_SRAM0,
    C_SRAM1,
    C_SRAM2,
    C_SRAM3,
    NONE_SRAM,  // for non-SRAM scenarios (BYPASS and BILINEAR)
}EN_SCALING_FILTER_SRAM;

typedef struct
{
    // SC info, should be fixed initially based on spec
    BYTE                        u8SiScIndex;                // scaler index
    DWORD                       u32SiRegBase;               // register base //tmp no need?? move to another mapping table??

    // SC status
    EN_SCALER_DISP_STATUS       eSiDispStatus;              // display status
    BYTE                        u8SiFbMode;                 // 1: frame buffer, 0: frame buffer less
    BYTE                        u8SiSyncMode;
    BYTE                        u8SiRotate;                 //
    BYTE                        u8SiSCFlags;                // scaler sys flags
    BYTE                        u8SiWinIndex;               // window index

    // Input status
    //ModeSettingType             sSiIPModeSetting;

    EN_3D_FMT_IN                eSiIP3dFmtIn;
    EN_3D_FMT_OUT               eSiIP3dFmtOut;
    BOOL                        eSiIPHDMIDRRChk;

    // IP info

    BYTE                        u8SiIPPortIndex;            // input port index to display
    ST_WINDOW_INFO              sSiIPCaptureWin;            // input capture window
    BYTE                        u8SiIPHBR2LR;               // 0: L, 1:R
    BYTE                        u8SiIpCSC;
    BYTE                        u8SiIPRstCntr;              // counter after ip reset

    // SCMI
    float                       fSiMemPixByte;             // 444 or 422
    BYTE                        u8SiFieldNum;
    BYTE                        u8SiDelayLines;

    // SCF info
    BYTE                        u8SiSCFMemSyncSC;           // ipm/opm sync to this scaler
    ST_WINDOW_INFO              sSiSCFIpmWindow;            // window of Ipm needs to fill
    ST_WINDOW_INFO              sSiSCFMemWindow;            // window of whole image frame in dram
    ST_WINDOW_INFO              sSiSCFOpmWindow;            // window of Opm needs to fetch

    // OP2 info
    ST_WINDOW_INFO              sSiOP2DisplayWin;           // output capture window
    ST_GUARDBAND_INFO           sSiOP2GuardBand;            // guardband size
    BYTE                        u8SiGuardBandOut;
    BYTE                        u8SiGuardBandOutBalMode;

    // SCALING
    EN_SCALING_TYPE             u8SiScalingType;

    // facke captue window info
    BOOL                        bSiScalerRightPos;
    WORD                        u16SiLSCImgOffset;          // H start of image DE of L scaler used by R scaler
    WORD                        u16SiLSCOpmOffset;          // opm.x of L scaler used by R scaler
    ST_IMAGE_DE_Offset          sSiImgDEOffset;             // image DE offset info for each scaler
    BOOL                        bSiIP2CapEnable;            // Enable 2nd IP2 capture

}ST_SCALER_INFO;

#define SC_FIELD_NUM            4

extern ST_SCALER_INFO   g_ScalerInfo[SC_MAX_NUMS];
extern const ST_WINDOW_INFO _InitWindow;
extern WORD g_u16FPLLTuneTimes;
extern WORD g_u16FPLLTuneDelay;
extern DWORD g_u32FPLLTuneMax, g_u32FPLLTuneMin;
extern BYTE g_u8SCTimerFlag10ms;

////////////////////////////////////////////////////////////////////
// software reset
//
#define SWRST_ALL       0xFFFF
// Byte1
#define RST_OSCCLK      _BIT7
#define RST_FCLK        _BIT6
#define RST_IP_OFFLINE  _BIT5
#define RST_IP          _BIT4
#define RST_OP          _BIT3
#define RST_IP_MAIN     _BIT2
#define RST_IP_SUB      _BIT1
#define RST_IP_OP_ALL   _BIT0

// Byte2: some SW Reset for ADC/DVI/PLL block
#define RST_ATOP        _BIT7 << 8
#define RST_HDMI        _BIT6 << 8
#define RST_HDCP        _BIT5 << 8
#define RST_DVI         _BIT4 << 8
#define RST_PLL_DIG_B   _BIT3 << 8
#define RST_ADC_VD      _BIT2 << 8
#define RST_PLL_DIG_A   _BIT1 << 8
#define RST_ADC_DIG_A   _BIT0 << 8

#define SCALING_H_DOWN(_SC_)                    (g_ScalerInfo[_SC_].u8SiScalingType&SC_SCALING_PRE_H_DOWN)
#define SCALING_V_DOWN(_SC_)                    (g_ScalerInfo[_SC_].u8SiScalingType&SC_SCALING_PRE_V_DOWN)
#define SCALING_H_UP(_SC_)                      (g_ScalerInfo[_SC_].u8SiScalingType&SC_SCALING_POST_H_UP)
#define SCALING_V_UP(_SC_)                      (g_ScalerInfo[_SC_].u8SiScalingType&SC_SCALING_POST_V_UP)

// mapping
#define MapWin2ScIndex(_WIN_)                   (GetLsbBitPos(g_DisplayWindow[_WIN_].u16DwScMaskOut)) // Care May Be 1:N
#define MapWin2Port(_WIN_)                      (g_DisplayWindow[_WIN_].u8DwPortIndex)
#define MapWin2PortMux(_WIN_)                   (g_InputPort[MapWin2Port(_WIN_)].eIPMux)
#define MapScaler2SyncSC(_SC_)                  (g_ScalerInfo[_SC_].u8SiSCFMemSyncSC)
#define MapScaler2Win(_SC_)                     (g_ScalerInfo[_SC_].u8SiWinIndex)
#define MapScaler2Port(_SC_)                    (g_ScalerInfo[_SC_].u8SiIPPortIndex)

//#define MapPort2Win(_PORT_)                   // Invalid, Port:Win May Be 1:N
#define MapPort2DetScIndex(_PORT_)              (g_InputPort[_PORT_].u8IPScIndex) // map to detecting scaler for input handler, since input:scaler may be 1:N
#define MapPort2ScMask(_PORT_)                  (g_InputPort[_PORT_].u16IPScMask) // map to detecting scaler for input handler, since input:scaler may be 1:N

#define SCRegBase(SCIndex)                      (g_ScalerInfo[SCIndex].u32SiRegBase)
#if (CHIP_ID == MST9U4)
#define SCIP1RegBase(SCIndex)                   msDrvIP1RegBaseMapping(SCIndex)
#else
#define SCIP1RegBase(SCIndex)                   SCRegBase(SCIndex)
#endif
//============================================================================================
// current input timing information
//#define SrcInputPort(SCIndex)                   (g_ScalerInfo[SCIndex].u8SiIPPortIndex)
#define SrcInputPortM                           (g_ScalerInfo[SCL0_MAIN].u8SiIPPortIndex) // global keep L0_MAIN !!!

#define FreeRunModeFlag(SCIndex)                (g_ScalerInfo[SCIndex].u8SiSCFlags&bFreeRunModeBit)
#define Set_FreeRunModeFlag(SCIndex)            (g_ScalerInfo[SCIndex].u8SiSCFlags|=bFreeRunModeBit)
#define Clr_FreeRunModeFlag(SCIndex)            (g_ScalerInfo[SCIndex].u8SiSCFlags&=~bFreeRunModeBit)

//=================================================================================
// sclaer software reset control bit
#define SCALER_RESET_IP_F1  _BIT1
#define SCALER_RESET_IP_F2  _BIT2
#define SCALER_RESET_OP     _BIT3
#define SCALER_RESET_IP     _BIT4
#define SCALER_FCLK         _BIT6
#define SCALER_OSCCLK       _BIT7

//=================================================================================
#define IsScalerLeft(SCIndex)                   (!(SCIndex&BIT1))
#define IsScalerRight(SCIndex)                  (SCIndex&BIT1)
#define IsScalerLeftPos(SCIndex)                (!(IsScalerRightPos(SCIndex)))
#define IsScalerRightPos(SCIndex)               (g_ScalerInfo[SCIndex].bSiScalerRightPos)//g_ScalerInfo[SCIndex].sSiSCFOpmWindow.x
#define IsScaler3DDI(SCIndex)                   ((g_ScalerInfo[SCIndex].u8SiDeInterlaceMode == DEINT_3DDI)\
                                                ||(g_ScalerInfo[SCIndex].u8SiDeInterlaceMode == DEINT_3DDI_HISTORY))

//=================================================================================
#define SCALING_RATIO_CB(_IN_, _OUT_)           ((_OUT_ * 1048576) / _IN_ + 1)
#define SCALING_RATIO_BI(_IN_, _OUT_)           ((((_OUT_)%(_IN_))||(((_OUT_)/(_IN_))>=4))?(SCALING_RATIO_BI_M1(_IN_, _OUT_)):(SCALING_RATIO_BI_NM1(_IN_, _OUT_)))
#define SCALING_RATIO_BI_M1(_IN_, _OUT_)        ((_IN_-1) * 1048576 / (_OUT_ - 1) + 1)
#define SCALING_RATIO_BI_NM1(_IN_, _OUT_)       ((_IN_) * 1048576 / (_OUT_))

//=================================================================================
#if ((CHIP_ID==MST9U3) || (CHIP_ID==MST9U4))
#define SCALER_MASK_MAIN        (bSCL0_MAIN|bSCR0_MAIN)
#define SCALER_MASK_SUB         (bSCL0_SUB |bSCR0_SUB )
#else
#define SCALER_MASK_MAIN        (bSCL0_MAIN|bSCR0_MAIN|bSCL1_MAIN|bSCR1_MAIN)
#define SCALER_MASK_SUB         (bSCL0_SUB |bSCR0_SUB |bSCL1_SUB |bSCR1_SUB)
#endif
#define SCALER_MASK_ALL        (SCALER_MASK_MAIN|SCALER_MASK_SUB)
///////////////////////////////////////////////////////////////////////
////////////////////// Drv functions /////////////////////////////////////
///////////////////////////////////////////////////////////////////////
INTERFACE void msDrvWinMemAlloc(void);
INTERFACE void msDrvScMemDrawLine(BYTE SCDetWin, DWORD u32BaseAddr, WORD u16PositionV, WORD u16PositionH, WORD u16PixCount, BYTE *pDataBGR, BYTE *pDataLSB);
INTERFACE void msDrvScMemMRQ(BYTE u8ScIndex, BOOL bForceBlock);
INTERFACE void msDrvScWindowSetup( BYTE SCDetWin, BOOL bSkipAutoWin );
INTERFACE void msDrvScSetMemFreeze(BYTE ucWinIndex, BOOL bEnable);
INTERFACE void msDrvScSetupPathInit(void);
INTERFACE void msDrvScSetupPath(BYTE SCDetWin);
INTERFACE void msDrvScSetScalingFilter(BYTE SCDetWin);
INTERFACE void msDrvScSetScalingFactor(BYTE SCDetWin);
INTERFACE void msDrvScSetDefaultFiclk(BYTE SCDetWin);
INTERFACE void msDrvScSetFieldDetect(BYTE SCDetWin);

INTERFACE void msDrvScHandler(void);

INTERFACE void msDrvScStatusClr( WORD u16SCMask, BOOL bFreeRun );
INTERFACE void msDrvScPostDispatch( BYTE ucWinIndex, ST_WINDOW_INFO sAutoWindow );
INTERFACE BOOL msDrvScPreDispatch( BYTE ucWinIndex, WORD uwPreScMask );
INTERFACE void msDrvScPreAttach( BYTE ucSCIndex, BYTE ucWinIndex );
INTERFACE void msDrvScSetRotateMode( WORD uwScMask, BYTE ucRotate);
INTERFACE void msDrvScSetStatus( WORD uwScMask, EN_SCALER_DISP_STATUS eScalerStatus);
INTERFACE void msDrvScMuteScreen( WORD uwScMask, BOOL bEnableMute);
#if ENABLE_SUBSCIN
INTERFACE BYTE msDrvScGetIdleScalerS( void );
INTERFACE BYTE msDrvScGetIdleScalerSS( BYTE ucSCIndex );
#endif
INTERFACE BYTE msDrvScGetIdleScalerL( void );
INTERFACE BYTE msDrvScGetIdleScalerR( void );
INTERFACE BYTE msDrvScGetIdleScalerLM( void );
INTERFACE BYTE msDrvScGetIdleScalerRM( void );
INTERFACE BYTE msDrvScGetIPStableScaler( WORD u16SCMask );
INTERFACE DWORD msDrvScGetRegBase( BYTE ucSCIndex );
INTERFACE BYTE msDrvSrcMapIPMuxToPort( BYTE eIPMux);
#if 0 // now always 2p
INTERFACE BOOL msDrvScCheckOutputDualMode( void );
INTERFACE void msDrvScDispHStartUpdate( void );
#endif
INTERFACE BOOL msDrvCheckMultiWin( void );
INTERFACE void msDrvSrcStatusClr( void );
INTERFACE void msDrvSrcAttach( BYTE InputPort, BYTE SCDetWin );
INTERFACE void msDrvSrcDetach( BYTE InputPort, BYTE SCDetWin );
INTERFACE void msDrvSrcInOnlySCUpdate(BYTE u8InputPort, BYTE u8WinIndex);
INTERFACE void msDrvSrcSetHBR2FlagMux(BYTE u8InputPort, BOOL bHBR2In, EN_INPUTPORT_TYPE eIPType);
INTERFACE void msDrvSrcConfigInit( void );
INTERFACE void msDrvSrcSyncStatusToSC( BYTE InputPort, BYTE SCDetWin);
INTERFACE void msDrvSoftwareReset(DWORD ucParam, BYTE SCDetWin);
INTERFACE void msDrvSetScalingPreVCtrlLineIn( BYTE SCDetWin, WORD u16UserLineCount, Bool bEnable);
INTERFACE void msDrvSetScalingPreVCtrlLineOut( BYTE SCDetWin, WORD u16UserLineCount, Bool bEnable);
INTERFACE void msDrv_SDHD_DETECT(BYTE SCDetWin);
INTERFACE void msDrvSetChromaScalingFilter(BYTE ucWinIndex, EN_DIRECTION eDirection, EN_SCALING_FILTER_MODE eMode, EN_SCALING_FILTER_SRAM eSRAM);
INTERFACE void msDrvSetLumaScalingFilter(BYTE ucWinIndex, EN_DIRECTION eDirection, EN_SCALING_FILTER_MODE eMode, EN_SCALING_FILTER_SRAM eSRAM);
#if ENABLE_SUBSC_HOLLOW_OUT
INTERFACE void msDrvSubSCHollowOut( BOOL bL1Top, BOOL bR1Top );
#endif
INTERFACE BYTE msDrvSrcFakeDualGetM( void );
INTERFACE BYTE msDrvSrcFakeDualGetS( void );
INTERFACE BYTE msDrvScFakeDualGetM( BYTE u8IdxWin );
INTERFACE BYTE msDrvScFakeDualGetS( BYTE u8IdxWin );
#undef INTERFACE
#endif

