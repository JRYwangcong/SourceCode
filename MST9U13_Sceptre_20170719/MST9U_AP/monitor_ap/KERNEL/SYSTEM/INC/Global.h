#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include "Board.h"
#include "msDataType.h"
#include "UserPrefDef.h"
#include "Mode.h"
#include "OsdDefault.h"

///////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>
#if 0//nop?
#include <intrins.h>
#endif
#include "datatype.h"
//#include "MDebug.h"
#include "Common.h"
#include "Ms_rwreg.h"
#include "ms_reg.h"
//#include "Reg52.h"
#include "i2cdef.h"

#include "misc.h"
#include "mStar.h"
#include "menudef.h"
#include "drvADC.h"
#include "drvGPIO.h"

#include "halRwreg.h"
#include "drvMcu.h"
#include "drvAdjust.h"
#include "drvDDC2Bi.h"
//#include "drvPower.h" // the function moves to drvmStar
#include "drvmStar.h"
#include "msScaler.h"
#include "msScalerAP.h"
#include "mapi_combo.h"

#if ENABLE_RTE
#include "msOVD.h"
#include "drvmsOVD.h"
#endif
#include "appmStar.h"
#if ENABLE_TOUCH_PANEL
#include "touchpanel.h"
#endif
#if ENABLE_TOUCH_KEY
#include "touchkey.h"
#endif
#if 1//ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif
#include "combo_config.h"

#if ENABLE_DP_OUTPUT
#include "drvDPTxApp.h"
#endif
#include "Ms_PM.h"
#include "drvWDT.h"

#if ENABLE_5707
#include "amp_i2s_tas5707.h"
#endif

#include "appWindow.h"

/////////////////////////////////////////////////////////////////////////////////////
///// XDATA MAP MUST Sync with 51 - KeilC project setting about xdata start/size/////
#define XDATA_SIZE              0x2000
#define XDATA_ADDR_START        0x4000 // keep 0x4000.

#define XDATA_MHL_CBUS_START0   0x4000 // 0x4000 ~ 0x40FF
#define XDATA_MHL_CBUS_START1   XDATA_MHL_CBUS_START0 + 256 //0x4100 ~ 0x41FF
#define XDATA_MHL_CBUS_START2   XDATA_MHL_CBUS_START1 + 256 //0x4200 ~ 0x42FF
#define XDATA_MHL_CBUS_START3   XDATA_MHL_CBUS_START2 + 256 //0x4300 ~ 0x43FF
#define XDATA_MHL_CBUS_START4   XDATA_MHL_CBUS_START3 + 256 //0x4400 ~ 0x44FF
#define XDATA_MHL_CBUS_START5   XDATA_MHL_CBUS_START4 + 256 //0x4500 ~ 0x45FF

#define XDATA_SWDB_ADDR_START   XDATA_MHL_CBUS_START5 + 0x100 // SWDB - allocate maximum 256 Bytes >=  sizeof(StuSWDBCtrl)
#define XDATA_DDC_ADDR_START    XDATA_SWDB_ADDR_START + 0x100 // DDC max = 256B*5=0x500, 0X4C00-0x0500=0x4700, In current driver code, maximum 256 Bytes needed.
#define XDATA_DDC_TX_ADDR_START  XDATA_DDC_ADDR_START + D2B_FIFO_XdataSize //
// ...
// reserved for IP that needs xdata
// ...
#define XDATA_MBX_XD2MIU_ADDR   0x4FFC // 4Bytes, xd2miu base address, won't be initialized in 51
#define XDATA_51_START          0x5000 // Setting in KeilC & startup.a51, start = 0x5000, size = 0x1000, for 51 variables
#define XDATA_ADDR_END          0x5FFF
///// XDATA MAP MUST Sync with 51 - KeilC project setting about xdata start/size/////
//////////////////////////////////////////////////////////////////////////

#if SET_BLIT_WITH_OSDFreeSyncSwitchAction  //  160729_03
extern bit bOSDFreeSyncOnOffFlag;
#endif
#if SET_BLIT_WITH_OSDFreeSyncSwitchAction  //  160729_03
extern bit bOSDFreeSyncOnOffFlag;
#endif
#if ENABLE_TIMINGCHANGE_CLRBL_ONLY  //  160714_14
extern bit bOnlyClrBL;
#elif ENABLE_TIMINGCHANGE_TURNOFFPANEL
extern bit bTurnOffPanelWhenTimingChange;
#endif

extern bit msFreeSyncTimerFlag;
#define Set_msFreeSyncTimerFlag()         (msFreeSyncTimerFlag = 1)
#define Clr_msFreeSyncTimerFlag()         (msFreeSyncTimerFlag = 0)
extern Bool g_NotSettingLPLL;  //

extern BYTE g_u8Test;

#if PANEL_VBY1()
extern WORD u8Vby1CheckLockCounter;
extern bit bVby1CheckLockFlag;
#endif
extern bit bHDMI108oiTimingFlag; //20160309

#if (VBY1_PowerSequence)
extern WORD xdata PowerOnBackLightCounter;
#endif
extern WORD xdata BlackScreenCounter;

extern BYTE xdata OSDActiveCounter;

#if DP_HPD_Test
extern BYTE TimingUnstableCNT;
#endif

#if Audio_Source_Enable
extern BYTE xdata gAudioSourceSelectableIndex;
#define GetCurAudioSourceIndex(InputPort)		(GetOSDAudioInputType(InputPort))
#endif

#if ENABLE_DEMURA_FUNCTION
extern BYTE g_MEASURE_SIZE_W;
extern BYTE g_MEASURE_SIZE_H;
#endif

#if USB_Updata_FW_Enable
extern BYTE xdata USBUpdataFwStatus;
#endif

#if New_Cross_Enable||Enable_Corss_FY||Enable_Corss_LHCX||Enable_Corss_RunFeng
extern BYTE xdata  UserPrefCrossType;
#endif

extern WORD xdata SystemFlags; // for system status flags
extern WORD xdata SystemFlags2; // for system status flags
extern WORD xdata SystemFlags3; // for system status flags
extern WORD xdata SystemFlags4; // for system status flags

extern WORD xdata OrigAdjPipHpos;
extern WORD xdata OrigAdjPipVpos;

extern WORD xdata InputWidth;
extern WORD xdata InputHeight;
extern WORD xdata InputVfreq;

extern BYTE xdata UserPrefBrightness_Temp;
extern BYTE xdata UserPrefContrast_Temp;

extern BYTE xdata DDCCIFuncCounter;
extern BYTE xdata DDCCIFuncCommand;

extern BYTE xdata UserPrefNoiseReduction;

#if Enable_Corss_LHCX_New
extern BYTE xdata GameModeTime_S;
extern BYTE xdata GameModeTime_M;
extern BYTE xdata GameModeTime_Position;
#endif

extern BYTE xdata LEDModeCounter;

#if 0//LED_HOT_Menu_Func
extern BYTE xdata UserprefLEDColorful;
extern BYTE xdata UserprefLedStrength;
extern BYTE xdata UserprefLedModeType;
#endif

#if ENABLE_HDCP
extern BYTE xdata Flash_HDCPcont;
extern BYTE xdata DDC1[];
extern bit LoadHDCPKeyFlag;
#define Set_LoadHDCPKeyFlag()    		(LoadHDCPKeyFlag=1)
#define Clr_LoadHDCPKeyFlag()     		(LoadHDCPKeyFlag=0)
#endif

//EEPROM MAP
extern BYTE u8NVRAM_MAP[];
extern BYTE u8NVRAM_MAP_BK[];
extern Bool g_bEEP_Dirty;
extern WORD g_u16EEP_SaveAddr;
extern WORD g_u16EEP_SaveCnt;
extern Bool g_BAutoSave;
extern Bool g_bBLTimeSaveFlag;
#if ENABLE_MENULOAD
extern BYTE u8MenuLoadFlag;
#endif
#if ENABLE_SW_SCALER_COMMON_WRITE
extern WORD g_u16CommonWriteSCMask;
#endif

#if (IR_REMOTE_SEL!=NO_REMOTE)
extern BYTE xdata IRkeyDelayCounter;
#endif

extern BYTE xdata VKeyPad;

#define SET_EEP_DIRTY_FLAG()    (g_bEEP_Dirty = TRUE)
#define CLR_EEP_DIRTY_FLAG()    (g_bEEP_Dirty = FALSE)
#define IS_EEP_DIRTY()          (g_bEEP_Dirty == TRUE)
#define DIFF_ADDR_START(wAddr)  (g_u16EEP_SaveAddr = wAddr)
#define DIFF_ADDR_CNT(wCnt)     (g_u16EEP_SaveAddr = wCnt)

#define SET_SYSTEM_AUTO_SAVE_FLAG() (g_BAutoSave = TRUE)
#define CLR_SYSTEM_AUTO_SAVE_FLAG() (g_BAutoSave = FALSE)
#define SYSTEM_AUTO_SAVE_FLAG    (g_BAutoSave)

#define BLTIME_SAVE_FLAG        (g_bBLTimeSaveFlag)
#define SET_BLTIME_SAVE_FLAG()  (g_bBLTimeSaveFlag = TRUE)
#define CLR_BLTIME_SAVE_FLAG()  (g_bBLTimeSaveFlag = FALSE)

//Window Settings
extern BYTE g_u8WinIndex;
extern Bool g_bY2R;

//MAILBOX
extern BYTE *u8MBX;
#if ENABLE_MBX_SAR
extern BYTE u8MailButton;
#endif

#ifdef LoadDefaultEDIDTo24C02
extern BYTE I2C_TYPE;
typedef enum
{
S_EDID_VGA,
S_EDID_Digital1,//DVI  MUX_COMBO1
S_EDID_Digital2,//HDMI1  MUX_COMBO2
S_EDID_Digital3,//HDMI2  MUX_COMBO0
S_max
}I2Ctype;
#endif

//GammaLUT format
#define GAMMA_256       0
#define GAMMA_1024      1
#define GAMMA_FORMAT    GAMMA_256

//Color Accuracy
#define COLOR_8_BIT         0x08
#define COLOR_10_BIT        0x0A
#define COLOR_ACCURACY      COLOR_8_BIT


#ifdef CUSTOMER_FUNCTION_DEFINE_SWITCH
// Move to UserPrefDef.h
#else
#define  FRAME_COLOR_BLUE                   0x03
#define  FRAME_COLOR_BLACL                 0x00
#define  FRAME_COLOR_WHITE                 0xFF

#define DEF_FRAME_COLOR_PATTERN       FRAME_COLOR_BLACL
#endif


//#if ENABLE_DPS
extern idata BOOL bRunToolFlag;
//#endif

#define LINEAR_MAPPING_VALUE(orgNow,orgMin,orgMax,newMin,newMax) \
    ((((DWORD)((orgNow)-(orgMin)))*((newMax)-(newMin))+(((orgMax)-(orgMin))>>1))/((orgMax)-(orgMin))+(newMin))
#if 0
typedef enum
{
    INPUT_RGB    = 0,   // 00
    INPUT_YUV422 = 1,   // 01
    INPUT_YUV444 = 2    // 11
};
#endif

typedef enum // Inputport mux
{
    MUX_ADC = 0,
    MUX_COMBO0, // MST9U4:H0
    MUX_COMBO1, // MST9U4:H1
    MUX_COMBO2, // MST9U4:H2
    MUX_COMBO3, // MST9U4:H3
    MUX_COMBO4, // MST9U4:DP0
    MUX_COMBO5, // MST9U4:DP1
    MUX_COMBO6, // MST9U4:DP2
    MUX_COMBO7, // MST9U4:VB1/LVDS
    MUX_VD,
    MUX_RESERVED
}EN_INPUTPORT_MUX;

enum // IP_SysFlags
{
    bInputTimingChangeBit   = BIT0,
    bInputTimingStableBit   = BIT1,
    bDoModeSettingBit       = BIT2,
    bReadyForChangeBit      = BIT3,
    bCableNotConnectedBit   = BIT4,
};

/* ///////////////////////////////////////////////
 ------------                -------- HPD
             |_______________|

 ---------------           ---------- CLK
                |__________|

 ------------1--2----------3---------
/////////////////////////////////////////////////*/

typedef struct
{
    WORD    u16HPDCnt;      //1:HPD duration counter
    WORD    u16HPDClkLow;   //2:CLK r-term low thd
    WORD    u16HPDClkHigh;  //3:CLK r-term high thd
} ST_HPD_PERIOD_CNT;

typedef struct
{
    EN_INPUTPORT_MUX            eIPMux;
    EN_INPUTPORT_TYPE           eIPType;

    EN_INPUT_STATUS             eIPStatus;

    BYTE                        u8IPScIndex;            // scaler index for detecting sync
    WORD                        u16IPScMask;           // scaler index - bit based
//    ST_WINDOW_INFO                    sIPCaptureWin;                    // input capture window

    BYTE                        u8IPModeDetectCounter;
    WORD                        u16IPInputTimingStableCounter;
    WORD                        u16IPInputSyncValidThd;
    WORD                        u16IPInputSyncLossThd;
    BYTE                        u8IPSysFlags;
    InputTimingType             sIPInputTiming;
    ST_WINDOW_INFO              sIPAutoWin;

    EN_InputColorFormatType     eIPInputColorFormat;    // indicate input color space is RGB or YUV
#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
    EN_InputRangeFormatType     eIPInputRangeFormat;    //Julian, indicate input range is Limited or Full
    EN_InputColorimetryType     eIPInputColorimetry;    //Julian, indicate input colorimetry
#endif
    BOOL                        bIPInputHBR2;           // 1: input HBR2 detected
    BOOL                        bIPInputSplitter23;           // 1: input splitter 23 instead of splitter 01
    ST_HPD_PERIOD_CNT           sIPHPDCntr;           // down counter for HPD duration
}ST_INPUT_PARAM;

////////////////////////////////////////////////////////////////////////////////////////////////////////
#define GET_FP3D_2X_VRES(u16VDE,u16VTT) (((u16VDE)<<1)-(u16VTT)) //get frame packing 3D 2x V resolution

extern ST_INPUT_PARAM g_InputPort[Input_Nums]; // define each port info(mux/type) in bd_xxx.h

///////////////////////////////////////////////////////////////////////////////////////
// Should prevent from using CURRENT_INPUT_XXX!!! since multiple/parallell input detect is allowed //
#define CURRENT_INPUT_IS_VGA()          (g_InputPort[SrcInputPortM].eIPType & TYPE_ANALOG)
#define CURRENT_INPUT_IS_DVI_S()        (g_InputPort[SrcInputPortM].eIPType & TYPE_DVI_SINGLE)
#define CURRENT_INPUT_IS_DVI_D()        (g_InputPort[SrcInputPortM].eIPType & TYPE_DVI_DUAL)
#define CURRENT_INPUT_IS_DVI()          (CURRENT_INPUT_IS_DVI_S() | CURRENT_INPUT_IS_DVI_D())
#define CURRENT_INPUT_IS_HDMI()         (g_InputPort[SrcInputPortM].eIPType & TYPE_HDMI)
#define CURRENT_INPUT_IS_DISPLAYPORT()  (g_InputPort[SrcInputPortM].eIPType & TYPE_DP)
#define CURRENT_INPUT_IS_TMDS()         (CURRENT_INPUT_IS_DVI() | CURRENT_INPUT_IS_HDMI())
#define CURRENT_INPUT_IS_VD()           (g_InputPort[SrcInputPortM].eIPType & TYPE_VD)
#define CURRENT_INPUT_IS_DC_RX()        (g_InputPort[SrcInputPortM].eIPType & TYPE_DAISY_CHAIN)                                            
//#define CURRENT_INPUT_IS_DIGITAL()      (!CURRENT_INPUT_IS_VGA())

#if ENABLE_VGA_INPUT
    #define CURRENT_INPUT_IS_ANALOG1()          (SrcInputPortM == INPUT_VGA)
#endif
#if ENABLE_DVI_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DVI()      (SrcInputPortM == INPUT_DVI)
  #if ENABLE_DVI2ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DVI2ND()   (SrcInputPortM == INPUT_DVI2ND)
  #endif
  #if ENABLE_DVI3ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DVI3ND()   (SrcInputPortM == INPUT_DVI3ND)
  #endif
#endif
#if ENABLE_HDMI_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_HDMI()     (SrcInputPortM == INPUT_HDMI)
  #if ENABLE_HDMI2ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_HDMI2ND()  (SrcInputPortM == INPUT_HDMI2ND)
  #endif
  #if ENABLE_HDMI3ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_HDMI3ND()  (SrcInputPortM == INPUT_HDMI3ND)
  #endif
  #if ENABLE_HDMI4ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_HDMI4ND()  (SrcInputPortM == INPUT_HDMI4ND)
  #endif

#endif
#if ENABLE_DP_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DP()       (SrcInputPortM == INPUT_DISPLAYPORT)
  #if ENABLE_DP2ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DP2ND()    (SrcInputPortM == INPUT_DISPLAYPORT2ND)
  #endif
  #if ENABLE_DP3ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DP3ND()    (SrcInputPortM == INPUT_DISPLAYPORT3ND)
  #endif
  #if ENABLE_DP4ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DP4ND()    (SrcInputPortM == INPUT_DISPLAYPORT4ND)
  #endif
  #if ENABLE_DP5ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DP5ND()    (SrcInputPortM == INPUT_DISPLAYPORT5ND)
  #endif
  #if ENABLE_DP6ND_INPUT
    #define CURRENT_INPUT_IS_DIGITAL_DP6ND()    (SrcInputPortM == INPUT_DISPLAYPORT6ND)
  #endif

#endif
// Should prevent from using CURRENT_INPUT_XXX!!! since multiple/parallell input detect is allowed //
///////////////////////////////////////////////////////////////////////////////////////

#define INPUT_IS_VGA(u8InputPort)               (g_InputPort[u8InputPort].eIPType & TYPE_ANALOG)
#define INPUT_IS_DVI_S(u8InputPort)             (g_InputPort[u8InputPort].eIPType & TYPE_DVI_SINGLE)
#define INPUT_IS_DVI_D(u8InputPort)             (g_InputPort[u8InputPort].eIPType & TYPE_DVI_DUAL)
#define INPUT_IS_DVI(u8InputPort)               (INPUT_IS_DVI_S(u8InputPort) | INPUT_IS_DVI_D(u8InputPort))
#define INPUT_IS_HDMI(u8InputPort)              (g_InputPort[u8InputPort].eIPType & TYPE_HDMI)
#define INPUT_IS_HDMI_2_0(u8InputPort)          (g_InputPort[u8InputPort].eIPType & TYPE_HDMI_2_0)
#define INPUT_IS_MHL(u8InputPort)          (g_InputPort[u8InputPort].eIPType & TYPE_MHL)

#define INPUT_IS_DISPLAYPORT(u8InputPort)       (g_InputPort[u8InputPort].eIPType & TYPE_DP)
#define INPUT_IS_TMDS(u8InputPort)              (INPUT_IS_DVI(u8InputPort) | INPUT_IS_HDMI(u8InputPort))
#define INPUT_IS_VD(u8InputPort)                (g_InputPort[u8InputPort].eIPType & TYPE_VD)
#define INPUT_IS_HBR2(u8InputPort)              (g_InputPort[u8InputPort].eIPType & (TYPE_HBR2_0|TYPE_HBR2_1))
#define INPUT_IS_HBR2_0(u8InputPort)            (g_InputPort[u8InputPort].eIPType & TYPE_HBR2_0)
#define INPUT_IS_HBR2_1(u8InputPort)            (g_InputPort[u8InputPort].eIPType & TYPE_HBR2_1)
#define INPUT_IS_FAKE_DUAL(u8InputPort)         (g_InputPort[u8InputPort].eIPType & TYPE_FAKE_DUAL)
#define INPUT_IS_FAKE_DUAL_S(u8InputPort)       (g_InputPort[u8InputPort].eIPType & TYPE_FAKE_DUAL_S)
#define INPUT_IS_FAKE_DUAL_MONO(u8InputPort)    (INPUT_IS_FAKE_DUAL(u8InputPort) && INPUT_IS_FAKE_DUAL_S(u8InputPort))
#define INPUT_IS_DC_RX(u8InputPort)             (g_InputPort[u8InputPort].eIPType & TYPE_DAISY_CHAIN)
#define INPUT_IS_DP_MST(u8InputPort)            (g_InputPort[u8InputPort].eIPType & TYPE_DP_MST)
#define INPUT_IS_VBY1_RX(u8InputPort)           (g_InputPort[u8InputPort].eIPType & TYPE_VBY1)
#define INPUT_IS_LVDS_RX(u8InputPort)           (g_InputPort[u8InputPort].eIPType & TYPE_LVDS)

#define INPUT_IS_DRR(u8InputPort)         (msAPI_combo_IPGetDPDRRFlag((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux))
//#define INPUT_IS_DP_DRR(u8InputPort)      ((INPUT_IS_DRR(u8InputPort))&&(INPUT_IS_DISPLAYPORT(u8InputPort)))
#define INPUT_IS_HDMI_DRR(u8InputPort)    ((INPUT_IS_DRR(u8InputPort))&&(INPUT_IS_HDMI(u8InputPort)))

#define INPUT_IS_MUX_ADC(u8InputPort)     (g_InputPort[u8InputPort].eIPMux == MUX_ADC)
#define INPUT_IS_MUX_C0(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO0)
#define INPUT_IS_MUX_C1(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO1)
#define INPUT_IS_MUX_C2(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO2)
#define INPUT_IS_MUX_C3(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO3)
#define INPUT_IS_MUX_C4(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO4)
#define INPUT_IS_MUX_C5(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO5)
#define INPUT_IS_MUX_C6(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO6)
#define INPUT_IS_MUX_C7(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_COMBO7)
#define INPUT_IS_MUX_VD(u8InputPort)      (g_InputPort[u8InputPort].eIPMux == MUX_VD)
#if ENABLE_VGA_INPUT
    #define INPUT_IS_ANALOG1(InputPort)         (InputPort == INPUT_VGA)
#endif
#if ENABLE_DVI_INPUT
    #define INPUT_IS_DIGITAL_DVI(InputPort)     (InputPort == INPUT_DVI)
  #if ENABLE_DVI2ND_INPUT
    #define INPUT_IS_DIGITAL_DVI2ND(InputPort)  (InputPort == INPUT_DVI2ND)
  #endif
  #if ENABLE_DVI3ND_INPUT
    #define INPUT_IS_DIGITAL_DVI3ND(InputPort)  (InputPort == INPUT_DVI3ND)
  #endif
#endif
#if ENABLE_HDMI_INPUT
    #define INPUT_IS_DIGITAL_HDMI(InputPort)    (InputPort == INPUT_HDMI)
  #if ENABLE_HDMI2ND_INPUT
    #define INPUT_IS_DIGITAL_HDMI2ND(InputPort) (InputPort == INPUT_HDMI2ND)
  #endif
  #if ENABLE_HDMI3ND_INPUT
    #define INPUT_IS_DIGITAL_HDMI3ND(InputPort) (InputPort == INPUT_HDMI3ND)
  #endif
  #if ENABLE_HDMI4ND_INPUT
    #define INPUT_IS_DIGITAL_HDMI4ND(InputPort) (InputPort == INPUT_HDMI4ND)
  #endif
#endif
#if ENABLE_DP_INPUT
    #define INPUT_IS_DIGITAL_DP(InputPort)      (InputPort == INPUT_DISPLAYPORT)
  #if ENABLE_DP2ND_INPUT
    #define INPUT_IS_DIGITAL_DP2ND(InputPort)   (InputPort == INPUT_DISPLAYPORT2ND)
  #endif
  #if ENABLE_DP3ND_INPUT
    #define INPUT_IS_DIGITAL_DP3ND(InputPort)   (InputPort == INPUT_DISPLAYPORT3ND)
  #endif
  #if ENABLE_DP4ND_INPUT
    #define INPUT_IS_DIGITAL_DP4ND(InputPort)   (InputPort == INPUT_DISPLAYPORT4ND)
  #endif
  #if ENABLE_DP5ND_INPUT
    #define INPUT_IS_DIGITAL_DP5ND(InputPort)   (InputPort == INPUT_DISPLAYPORT5ND)
  #endif
  #if ENABLE_DP6ND_INPUT
    #define INPUT_IS_DIGITAL_DP6ND(InputPort)   (InputPort == INPUT_DISPLAYPORT6ND)
  #endif

#endif


//EN_INPUT_SYS_FLAG
#define InputTimingChangeFlag(InputPort)          (g_InputPort[InputPort].u8IPSysFlags&bInputTimingChangeBit)
#define Set_InputTimingChangeFlag(InputPort)      (g_InputPort[InputPort].u8IPSysFlags|=bInputTimingChangeBit)
#define Clr_InputTimingChangeFlag(InputPort)      (g_InputPort[InputPort].u8IPSysFlags&=~bInputTimingChangeBit)

#define InputTimingStableFlag(InputPort)          (g_InputPort[InputPort].u8IPSysFlags&bInputTimingStableBit)
#define Set_InputTimingStableFlag(InputPort)      (g_InputPort[InputPort].u8IPSysFlags|=bInputTimingStableBit)
#define Clr_InputTimingStableFlag(InputPort)      (g_InputPort[InputPort].u8IPSysFlags&=~bInputTimingStableBit)

#define DoModeSettingFlag(InputPort)              (g_InputPort[InputPort].u8IPSysFlags&bDoModeSettingBit)
#define Set_DoModeSettingFlag(InputPort)          (g_InputPort[InputPort].u8IPSysFlags|=bDoModeSettingBit)
#define Clr_DoModeSettingFlag(InputPort)          (g_InputPort[InputPort].u8IPSysFlags&=~bDoModeSettingBit)

#define ReadyForChangeFlag(InputPort)             (g_InputPort[InputPort].u8IPSysFlags&bReadyForChangeBit)
#define Set_ReadyForChangeFlag(InputPort)         (g_InputPort[InputPort].u8IPSysFlags|=bReadyForChangeBit)
#define Clr_ReadyForChangeFlag(InputPort)         (g_InputPort[InputPort].u8IPSysFlags&=~bReadyForChangeBit)

#define CableNotConnectedFlag(InputPort)          (g_InputPort[InputPort].u8IPSysFlags&bCableNotConnectedBit)
#define Set_CableNotConnectedFlag(InputPort)      (g_InputPort[InputPort].u8IPSysFlags|=bCableNotConnectedBit)
#define Clr_CableNotConnectedFlag(InputPort)      (g_InputPort[InputPort].u8IPSysFlags&=~bCableNotConnectedBit)

//
#define ModeDetectCounter(InputPort)              (g_InputPort[InputPort].u8IPModeDetectCounter)
#define InputTimingStableCounter(InputPort)       (g_InputPort[InputPort].u16IPInputTimingStableCounter)
#define SyncValidThd(InputPort)                   (g_InputPort[InputPort].u16IPInputSyncValidThd)
#define SyncLossThd(InputPort)                    (g_InputPort[InputPort].u16IPInputSyncLossThd)
#define InputColorFormat(InputPort)               (g_InputPort[InputPort].eIPInputColorFormat)
#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
#define InputRangeFormat(InputPort)               (g_InputPort[InputPort].eIPInputRangeFormat)
#define InputColorimetry(InputPort)               (g_InputPort[InputPort].eIPInputColorimetry)
#endif

// current input timing information
#define SrcHPeriod(InputPort)                     (g_InputPort[InputPort].sIPInputTiming.HPeiod)
#define SrcVTotal(InputPort)                      (g_InputPort[InputPort].sIPInputTiming.VTotal)
#define SrcVfreq(InputPort)                      (g_InputPort[InputPort].sIPInputTiming.VFreq)
#define SrcModeIndex(InputPort)                   (g_InputPort[InputPort].sIPInputTiming.ModeIndex)
#define SrcFlags(InputPort)                       (g_InputPort[InputPort].sIPInputTiming.fFlags)
#define SrcAutoHstart(InputPort)                  (g_InputPort[InputPort].sIPInputTiming.AutoHstart)
#define SrcAutoVstart(InputPort)                  (g_InputPort[InputPort].sIPInputTiming.AutoVstart)


// Input Timing information flags
#define SyncLossState(InputPort)                  (SrcFlags(InputPort)&SyncLoss)
#define UserModeFlag(InputPort)                   (SrcFlags(InputPort)&bUserMode)
#define UnsupportedModeFlag(InputPort)            (SrcFlags(InputPort)&bUnsupportMode)
#define NativeModeFlag(InputPort)                 (SrcFlags(InputPort)&bNativeMode)
#define ShrinkModeFlag(InputPort)                 (SrcFlags(InputPort)&bShrinkMode)
#define InterlaceModeFlag(InputPort)              (SrcFlags(InputPort)&bInterlaceMode)

#define HBR2Flag(InputPort)                       (g_InputPort[InputPort].bIPInputHBR2)
#define DualInFlag(InputPort)                     (HBR2Flag(InputPort)||INPUT_IS_FAKE_DUAL(InputPort))

#define GET_PHYSICAL_VALUE(valuePecent,MinValue,MaxValue) (((WORD)((MaxValue)-(MinValue))*(valuePecent)+50)/100+(MinValue))

#ifndef USER_PREF_PHASE_IS_REAL_PHASE
  #if 1//CHIP_ID==CHIP_TSUMT || CHIP_ID==MST8556T // max phase value>100
  #define USER_PREF_PHASE_IS_REAL_PHASE 1   //if 0 USER_PREF_PHASE=OSD phase value(%), if 1 USER_PREF_PHASE=real phase value
  #else
  #define USER_PREF_PHASE_IS_REAL_PHASE 0
  #endif
#endif
#define OSD_PHASE_ADJ_BY_OSD    0
#define OSD_PHASE_ADJ_BY_REAL   (!OSD_PHASE_ADJ_BY_OSD)
#ifndef OSD_PHASE_ADJ_METHOD
  #if USER_PREF_PHASE_IS_REAL_PHASE
  #define OSD_PHASE_ADJ_METHOD  OSD_PHASE_ADJ_BY_REAL
  #else
  #define OSD_PHASE_ADJ_METHOD  OSD_PHASE_ADJ_BY_OSD
  #endif
#endif
//DO NOT ACCESS USER_PREF_PHASE(UPPHASE) DIRECTLY IN CODE
//ALL ACCESS TO THIS VARIABLE MUST USING FOLLOWING MACRO
#if USER_PREF_PHASE_IS_REAL_PHASE
#define UPPHASE_GET_REAL_VALUE(InputPort)             USER_PREF_PHASE(InputPort)
#define UPPHASE_GET_OSD_VALUE(InputPort)              LINEAR_MAPPING_VALUE(USER_PREF_PHASE(InputPort), 0, MAX_PHASE_VALUE, 0, 100)
#define UPPHASE_SET_BY_OSD_VALUE(u8Value, InputPort)  USER_PREF_PHASE(InputPort)=LINEAR_MAPPING_VALUE(u8Value, 0, 100, 0, MAX_PHASE_VALUE)
#define UPPHASE_SET_BY_REAL_VALUE(u8Value, InputPort) USER_PREF_PHASE(InputPort)=u8Value
#define UPPHASE_SET_DEFAULT(InputPort)                USER_PREF_PHASE(InputPort)=(MAX_PHASE_VALUE/2)
#define UPPHASE_CHECK_RANGE(InputPort)                USER_PREF_PHASE(InputPort)%=(MAX_PHASE_VALUE+1)
#else //USER_PREF_PHASE is OSD value
#define UPPHASE_GET_REAL_VALUE(InputPort)             LINEAR_MAPPING_VALUE(USER_PREF_PHASE(InputPort), 0, 100, 0, MAX_PHASE_VALUE)
#define UPPHASE_GET_OSD_VALUE(InputPort)              USER_PREF_PHASE(InputPort)
#define UPPHASE_SET_BY_OSD_VALUE(u8Value, InputPort)  USER_PREF_PHASE(InputPort)=u8Value
#define UPPHASE_SET_BY_REAL_VALUE(u8Value, InputPort) USER_PREF_PHASE(InputPort)=LINEAR_MAPPING_VALUE(u8Value, 0, MAX_PHASE_VALUE, 0, 100)
#define UPPHASE_SET_DEFAULT(InputPort)                USER_PREF_PHASE(InputPort)=50
#define UPPHASE_CHECK_RANGE(InputPort)                USER_PREF_PHASE(InputPort)%=(100+1)
#endif

///////////////////////////////////////////////////////////////////////////////
// System status flags
#define bPowerSavingBit             BIT0    // system power saving status
#define PowerSavingFlag             (SystemFlags&bPowerSavingBit)
#define Set_PowerSavingFlag()       (SystemFlags|=bPowerSavingBit)
#define Clr_PowerSavingFlag()       (SystemFlags&=~bPowerSavingBit)

#define bForcePowerSavingBit        BIT1    // indicated system must enter power down
#define ForcePowerSavingFlag        (SystemFlags&bForcePowerSavingBit)
#define Set_ForcePowerSavingFlag()  (SystemFlags|=bForcePowerSavingBit)
#define Clr_ForcePowerSavingFlag()  (SystemFlags&=~bForcePowerSavingBit)

#define bRxBusyBit                  BIT2   // debug is busy
#define RxBusyFlag                  (SystemFlags&bRxBusyBit)
#define Set_RxBusyFlag()            (SystemFlags|=bRxBusyBit)
#define Clr_RxBusyFlag()            (SystemFlags&=~bRxBusyBit)

#if 1
extern bit SecondFlag;
#define Set_SecondFlag()            (SecondFlag = 1)
#define Clr_SecondFlag()            (SecondFlag = 0)
#else
#define bSecondBit                  BIT3
#define SecondFlag                  (SystemFlags&bSecondBit)
#define Set_SecondFlag()            (SystemFlags|=bSecondBit)
#define Clr_SecondFlag()            (SystemFlags&=~bSecondBit)
#endif

#define SAVE_DIFF_ENABLE_BIT        BIT4
#define SAVE_DIFF_ENABLE_FLAG       (SystemFlags&SAVE_DIFF_ENABLE_BIT)
#define SET_SAVE_DIFF_ENABLE_FLAG() (SystemFlags|=SAVE_DIFF_ENABLE_BIT)
#define CLR_SAVE_DIFF_ENABLE_FLAG() (SystemFlags&=~SAVE_DIFF_ENABLE_BIT)

#define bDoBurninModeBit            BIT5
#define DoBurninModeFlag            (SystemFlags&bDoBurninModeBit)
#define Set_DoBurninModeFlag()      (SystemFlags|=bDoBurninModeBit)
#define Clr_DoBurninModeFlag()      (SystemFlags&=~bDoBurninModeBit)

#define bTNROnBit                   BIT6
#define TNROnFlag                   (SystemFlags&bTNROnBit)
#define Set_TNROnFlag()             (SystemFlags|=bTNROnBit)
#define Clr_TNROnFlag()             (SystemFlags&=~bTNROnBit)

#define bSelfRefreshBootBit         BIT7
#define SelfRefreshBootFlag         (SystemFlags&bSelfRefreshBootBit)
#define Set_SelfRefreshBootFlag()   (SystemFlags|=bSelfRefreshBootBit)
#define Clr_SelfRefreshBootFlag()   (SystemFlags&=~bSelfRefreshBootBit)


#define bBacklightBit         BIT8
#define BacklightFlag         (SystemFlags&bBacklightBit)
#define Set_BacklightFlag()   (SystemFlags|=bBacklightBit)
#define Clr_BacklightFlag()   (SystemFlags&=~bBacklightBit)

#define bBlackScreenBit         BIT9
#define BlackScreenFlag         (SystemFlags&bBlackScreenBit)
#define Set_BlackScreenFlag()   (SystemFlags|=bBlackScreenBit)
#define Clr_BlackScreenFlag()   (SystemFlags&=~bBlackScreenBit)

#define bInputTimingStable_Normal_Bit               BIT10
#define InputTimingStable_NormalFlag          (SystemFlags&bInputTimingStable_Normal_Bit)
#define Set_InputTimingStable_NormalFlag()      (SystemFlags|=bInputTimingStable_Normal_Bit)
#define Clr_InputTimingStable_NormalFlag()      (SystemFlags&=~bInputTimingStable_Normal_Bit)

#define bDP_3840x2160_EDIDBit         					BIT11
#define DP_EDID_3840x2160_Flag         				(SystemFlags&bDP_3840x2160_EDIDBit)
#define Set_DP_EDID_3840x2160_Flag()       			(SystemFlags|=bDP_3840x2160_EDIDBit)
#define Clr_DP_EDID_3840x2160_Flag()       			(SystemFlags&=~bDP_3840x2160_EDIDBit)

#define bDP_1920x1080_EDIDBit         					BIT12
#define DP_EDID_1920x1080_Flag         				(SystemFlags&bDP_1920x1080_EDIDBit)
#define Set_DP_EDID_1920x1080_Flag()       			(SystemFlags|=bDP_1920x1080_EDIDBit)
#define Clr_DP_EDID_1920x1080_Flag()       			(SystemFlags&=~bDP_1920x1080_EDIDBit)

#define bOSDShowBit						       BIT13
#define OSDShowFlag        					(SystemFlags&bOSDShowBit)
#define Set_OSDShowFlag()      				(SystemFlags|=bOSDShowBit)
#define Clr_OSDShowFlag()      				(SystemFlags&=~bOSDShowBit)

#define DDC_WP_BIT     								BIT14
#define DDC_WP_FLAG         							(SystemFlags&DDC_WP_BIT)
#define SET_DDC_WP_FLAG()       						(SystemFlags|=DDC_WP_BIT)
#define CLR_DDC_WP_FLAG()       						(SystemFlags&=~DDC_WP_BIT)

#define bAdjDcrBacklight					BIT15
#define AdjDcrBacklightFlag			    	(SystemFlags&bAdjDcrBacklight)
#define Set_AdjDcrBacklightFlag()			(SystemFlags|=bAdjDcrBacklight)
#define Clr_AdjDcrBacklightFlag()			(SystemFlags&=~bAdjDcrBacklight)

//===================================================================================

#define bVBY1TrainingPassBit		BIT0
#define VBY1TrainingPassFlag			(SystemFlags2&bVBY1TrainingPassBit)
#define Set_VBY1TrainingPassFlag()	       (SystemFlags2|=bVBY1TrainingPassBit)
#define Clr_VBY1TrainingPassFlag()	       (SystemFlags2&=~bVBY1TrainingPassBit)


#define bBackLightOnBit            BIT1
#define BackLightOnFlag            (SystemFlags2&bBackLightOnBit)
#define Set_BackLightOnFlag()      (SystemFlags2|=bBackLightOnBit)
#define Clr_BackLightOnFlag()      (SystemFlags2&=~bBackLightOnBit)

#define bLR_UpDown_SwitchBit            BIT2
#define LR_UpDown_SwitchFlag            (SystemFlags2&bLR_UpDown_SwitchBit)
#define Set_LR_UpDown_SwitchFlag()      (SystemFlags2|=bLR_UpDown_SwitchBit)
#define Clr_LR_UpDown_SwitchFlag()      (SystemFlags2&=~bLR_UpDown_SwitchBit)

#define bMenuCross_RedBit            BIT3
#define MenuCross_RedFlag            (SystemFlags2&bMenuCross_RedBit)
#define Set_MenuCross_RedFlag()      (SystemFlags2|=bMenuCross_RedBit)
#define Clr_MenuCross_RedFlag()      (SystemFlags2&=~bMenuCross_RedBit)


#if 1//SleepMode_Enable
#define bForceEnterPowerOffBit            BIT4
#define ForceEnterPowerOffFlag            (SystemFlags2&bForceEnterPowerOffBit)
#define Set_ForceEnterPowerOffFlag()      (SystemFlags2|=bForceEnterPowerOffBit)
#define Clr_ForceEnterPowerOffFlag()      (SystemFlags2&=~bForceEnterPowerOffBit)

#define bForceEnterPowerOffMsgBit            BIT5
#define ForceEnterPowerOffMsgFlag            (SystemFlags2&bForceEnterPowerOffMsgBit)
#define Set_ForceEnterPowerOffMsgFlag()      (SystemFlags2|=bForceEnterPowerOffMsgBit)
#define Clr_ForceEnterPowerOffMsgFlag()      (SystemFlags2&=~bForceEnterPowerOffMsgBit)

#define bEnterPowerOffMsgChangeNumBit            BIT6
#define EnterPowerOffMsgChangeNumFlag            (SystemFlags2&bEnterPowerOffMsgChangeNumBit)
#define Set_EnterPowerOffMsgChangeNumFlag()      (SystemFlags2|=bEnterPowerOffMsgChangeNumBit)
#define Clr_EnterPowerOffMsgChangeNumFlag()      (SystemFlags2&=~bEnterPowerOffMsgChangeNumBit)
#endif

#define bHDMI1HPDBit            BIT7
#define HDMI1HPDFlag            (SystemFlags2&bHDMI1HPDBit)
#define Set_HDMI1HPDFlag()      (SystemFlags2|=bHDMI1HPDBit)
#define Clr_HDMI1HPDFlag()      (SystemFlags2&=~bHDMI1HPDBit)

#ifdef IR_MODE_SEL_2
#define bIR_Mode_SelBit         				BIT8
#define IR_Mode_Sel_Flag       			(SystemFlags2&bIR_Mode_SelBit)
#define Set_IR_Mode_Sel_Flag()			(SystemFlags2|=bIR_Mode_SelBit)
#define Clr_IR_Mode_Sel_Flag()			(SystemFlags2&=~bIR_Mode_SelBit)
#endif

#ifdef PanelI2CCommandSetting
#define bVBY1_Check_LockNBit         				BIT9
#define VBY1_Check_LockN_Flag       			(SystemFlags2&bVBY1_Check_LockNBit)
#define Set_VBY1_Check_LockN_Flag()			(SystemFlags2|=bVBY1_Check_LockNBit)
#define Clr_VBY1_Check_LockN_Flag()			(SystemFlags2&=~bVBY1_Check_LockNBit)
#endif


#define bHDMI_3840x2160_EDIDBit         					BIT10
#define HDMI_EDID_3840x2160_Flag         				(SystemFlags2&bHDMI_3840x2160_EDIDBit)
#define Set_HDMI_EDID_3840x2160_Flag()       			(SystemFlags2|=bHDMI_3840x2160_EDIDBit)
#define Clr_HDMI_EDID_3840x2160_Flag()       			(SystemFlags2&=~bHDMI_3840x2160_EDIDBit)

#define bHDMI_1920x1080_EDIDBit         					BIT11
#define HDMI_EDID_1920x1080_Flag         				(SystemFlags2&bHDMI_1920x1080_EDIDBit)
#define Set_HDMI_EDID_1920x1080_Flag()       			(SystemFlags2|=bHDMI_1920x1080_EDIDBit)
#define Clr_HDMI_EDID_1920x1080_Flag()       			(SystemFlags2&=~bHDMI_1920x1080_EDIDBit)

#define bHDMI_1920x2160_EDIDBit         					BIT12
#define HDMI_EDID_1920x2160_Flag         				(SystemFlags2&bHDMI_1920x2160_EDIDBit)
#define Set_HDMI_EDID_1920x2160_Flag()       			(SystemFlags2|=bHDMI_1920x2160_EDIDBit)
#define Clr_HDMI_EDID_1920x2160_Flag()       			(SystemFlags2&=~bHDMI_1920x2160_EDIDBit)


#define bDP_1920x2160_EDIDBit         					BIT13
#define DP_EDID_1920x2160_Flag         				(SystemFlags2&bDP_1920x2160_EDIDBit)
#define Set_DP_EDID_1920x2160_Flag()       			(SystemFlags2|=bDP_1920x2160_EDIDBit)
#define Clr_DP_EDID_1920x2160_Flag()       			(SystemFlags2&=~bDP_1920x2160_EDIDBit)


#define EARPHONE_EN_BIT     							BIT14
#define EARPHONE_EN_FLAG         						(SystemFlags2&EARPHONE_EN_BIT)
#define SET_EARPHONE_EN_FLAG()       				(SystemFlags2|=EARPHONE_EN_BIT)
#define CLR_EARPHONE_EN_FLAG()       					(SystemFlags2&=~EARPHONE_EN_BIT)

#define DISPLAY_UNSUPPORT_MODE_BIT     			BIT15
#define DISPLAY_UNSUPPORT_MODE_FLAG         		(SystemFlags2&DISPLAY_UNSUPPORT_MODE_BIT)
#define SET_DISPLAY_UNSUPPORT_MODE_FLAG()       	(SystemFlags2|=DISPLAY_UNSUPPORT_MODE_BIT)
#define CLR_DISPLAY_UNSUPPORT_MODE_FLAG()       	(SystemFlags2&=~DISPLAY_UNSUPPORT_MODE_BIT)

//===================================================================================

#define FACTORY_RESET_BIT                BIT0
#define FACTORY_RESET_FLAG               (SystemFlags3&FACTORY_RESET_BIT)
#define SET_FACTORY_RESET_FLAG()         (SystemFlags3|=FACTORY_RESET_BIT)
#define CLR_FACTORY_RESET_FLAG()         (SystemFlags3&=~FACTORY_RESET_BIT)

#ifdef IR_MODE_SEL_3
#define bIR_Mode_Sel3Bit         				BIT1
#define IR_Mode_Sel3_Flag       			(SystemFlags3&bIR_Mode_Sel3Bit)
#define Set_IR_Mode_Sel3_Flag()			(SystemFlags3|=bIR_Mode_Sel3Bit)
#define Clr_IR_Mode_Sel3_Flag()			(SystemFlags3&=~bIR_Mode_Sel3Bit)
#endif

#define bDVI_3840x2160_EDIDBit         					BIT2
#define DVI_EDID_3840x2160_Flag         				(SystemFlags3&bDVI_3840x2160_EDIDBit)
#define Set_DVI_EDID_3840x2160_Flag()       			(SystemFlags3|=bDVI_3840x2160_EDIDBit)
#define Clr_DVI_EDID_3840x2160_Flag()       			(SystemFlags3&=~bDVI_3840x2160_EDIDBit)

#define bDVI_1920x1080_EDIDBit         					BIT3
#define DVI_EDID_1920x1080_Flag         				(SystemFlags3&bDVI_1920x1080_EDIDBit)
#define Set_DVI_EDID_1920x1080_Flag()       			(SystemFlags3|=bDVI_1920x1080_EDIDBit)
#define Clr_DVI_EDID_1920x1080_Flag()       			(SystemFlags3&=~bDVI_1920x1080_EDIDBit)

#define bDVI_1920x2160_EDIDBit         					BIT4
#define DVI_EDID_1920x2160_Flag         				(SystemFlags3&bDVI_1920x2160_EDIDBit)
#define Set_DVI_EDID_1920x2160_Flag()       			(SystemFlags3|=bDVI_1920x2160_EDIDBit)
#define Clr_DVI_EDID_1920x2160_Flag()       			(SystemFlags3&=~bDVI_1920x2160_EDIDBit)

#define bVGA_3840x2160_EDIDBit         					BIT5
#define VGA_EDID_3840x2160_Flag         				(SystemFlags3&bVGA_3840x2160_EDIDBit)
#define Set_VGA_EDID_3840x2160_Flag()       			(SystemFlags3|=bVGA_3840x2160_EDIDBit)
#define Clr_VGA_EDID_3840x2160_Flag()       			(SystemFlags3&=~bVGA_3840x2160_EDIDBit)

#define bVGA_1920x1080_EDIDBit         					BIT6
#define VGA_EDID_1920x1080_Flag         				(SystemFlags3&bVGA_1920x1080_EDIDBit)
#define Set_VGA_EDID_1920x1080_Flag()       			(SystemFlags3|=bVGA_1920x1080_EDIDBit)
#define Clr_VGA_EDID_1920x1080_Flag()       			(SystemFlags3&=~bVGA_1920x1080_EDIDBit)

#define bVGA_1920x2160_EDIDBit         					BIT7
#define VGA_EDID_1920x2160_Flag         				(SystemFlags3&bVGA_1920x2160_EDIDBit)
#define Set_VGA_EDID_1920x2160_Flag()       			(SystemFlags3|=bVGA_1920x2160_EDIDBit)
#define Clr_VGA_EDID_1920x2160_Flag()       			(SystemFlags3&=~bVGA_1920x2160_EDIDBit)

#ifdef IR_MODE_SEL_4
#define bIR_Mode_Sel4Bit         				BIT8
#define IR_Mode_Sel4_Flag       			(SystemFlags3&bIR_Mode_Sel4Bit)
#define Set_IR_Mode_Sel4_Flag()			(SystemFlags3|=bIR_Mode_Sel4Bit)
#define Clr_IR_Mode_Sel4_Flag()			(SystemFlags3&=~bIR_Mode_Sel4Bit)
#endif

#define bLeft_EnterExit_SwitchBit            BIT9
#define Left_EnterExit_SwitchFlag            (SystemFlags3&bLeft_EnterExit_SwitchBit)
#define Set_Left_EnterExit_SwitchFlag()      (SystemFlags3|=bLeft_EnterExit_SwitchBit)
#define Clr_Left_EnterExit_SwitchFlag()      (SystemFlags3&=~bLeft_EnterExit_SwitchBit)

#define bRight_EnterExit_SwitchBit            BIT10
#define Right_EnterExit_SwitchFlag            (SystemFlags3&bRight_EnterExit_SwitchBit)
#define Set_Right_EnterExit_SwitchFlag()      (SystemFlags3|=bRight_EnterExit_SwitchBit)
#define Clr_Right_EnterExit_SwitchFlag()      (SystemFlags3&=~bRight_EnterExit_SwitchBit)

#define bHotCrossMenuBit            BIT11
#define HotCrossMenuFlag            (SystemFlags3&bHotCrossMenuBit)
#define Set_HotCrossMenuFlag()      (SystemFlags3|=bHotCrossMenuBit)
#define Clr_HotCrossMenuFlag()      (SystemFlags3&=~bHotCrossMenuBit)

#ifdef IR_MODE_SEL_5
#define bIR_Mode_Sel5Bit         				BIT12
#define IR_Mode_Sel5_Flag       			(SystemFlags3&bIR_Mode_Sel5Bit)
#define Set_IR_Mode_Sel5_Flag()			(SystemFlags3|=bIR_Mode_Sel5Bit)
#define Clr_IR_Mode_Sel5_Flag()			(SystemFlags3&=~bIR_Mode_Sel5Bit)
#endif

#define bHDMI2HPDBit            BIT13
#define HDMI2HPDFlag            (SystemFlags3&bHDMI2HPDBit)
#define Set_HDMI2HPDFlag()      (SystemFlags3|=bHDMI2HPDBit)
#define Clr_HDMI2HPDFlag()      (SystemFlags3&=~bHDMI2HPDBit)

#define bHDMI3HPDBit            BIT14
#define HDMI3HPDFlag            (SystemFlags3&bHDMI3HPDBit)
#define Set_HDMI3HPDFlag()      (SystemFlags3|=bHDMI3HPDBit)
#define Clr_HDMI3HPDFlag()      (SystemFlags3&=~bHDMI3HPDBit)


#define bATEModeBit							BIT15
#define ATEModeFlag					(SystemFlags3&bATEModeBit)
#define Set_ATEModeFlag()				(SystemFlags3|=bATEModeBit)
#define Clr_ATEModeFlag()				(SystemFlags3&=~bATEModeBit)

//===================================================================================
#define bDDCCIActiveBit					BIT0
#define DDCCIActiveFlag					(SystemFlags4&bDDCCIActiveBit)
#define Set_DDCCIActiveFlag()			(SystemFlags4|=bDDCCIActiveBit)
#define Clr_DDCCIActiveFlag()				(SystemFlags4&=~bDDCCIActiveBit)

#define bFreeSyncMsgBit					BIT1
#define FreeSyncMsgFlag					(SystemFlags4&bFreeSyncMsgBit)
#define Set_FreeSyncMsgFlag()			(SystemFlags4|=bFreeSyncMsgBit)
#define Clr_FreeSyncMsgFlag()			(SystemFlags4&=~bFreeSyncMsgBit)


#define bHotGameTimeShowBit                   BIT2
#define HotGameTimeShowFlag                  (SystemFlags4&bHotGameTimeShowBit)
#define Set_HotGameTimeShowFlag()	     (SystemFlags4|=bHotGameTimeShowBit)
#define Clr_HotGameTimeShowFlag()	     (SystemFlags4|=bHotGameTimeShowBit)

#define bHotGameTimeUpdateBit                BIT3
#define HotGameTimeUpdateFlag                  (SystemFlags4&bHotGameTimeUpdateBit)
#define Set_HotGameTimeUpdateFlag()	     (SystemFlags4|=bHotGameTimeUpdateBit)
#define Clr_HotGameTimeUpdateFlag()	     (SystemFlags4|=bHotGameTimeUpdateBit)

#define bOSDActionBit					BIT4
#define OSDActionFlag					(SystemFlags4&bOSDActionBit)
#define Set_OSDActionFlag()			(SystemFlags4|=bOSDActionBit)
#define Clr_OSDActionFlag()			(SystemFlags4&=~bOSDActionBit)

#if (LED_HOT_Menu_Func)
#define bLEDActiveOnBit					BIT5
#define LEDActiveOnFlag					(SystemFlags4&bLEDActiveOnBit)
#define Set_LEDActiveOnFlag()			(SystemFlags4|=bLEDActiveOnBit)
#define Clr_LEDActiveOnFlag()			(SystemFlags4&=~bLEDActiveOnBit)
#endif

#define bOSDFreeSyncActionBit			BIT6
#define OSDFreeSyncActionFlag			(SystemFlags4&bOSDFreeSyncActionBit)
#define Set_OSDFreeSyncActionFlag()		(SystemFlags4|=bOSDFreeSyncActionBit)
#define Clr_OSDFreeSyncActionFlag()		(SystemFlags4&=~bOSDFreeSyncActionBit)

#define bLedSlideBit					BIT7
#define LedSlideFlag					(SystemFlags4&bLedSlideBit)
#define Set_LedSlideFlag()                           	(SystemFlags4|=bLedSlideBit)
#define Clr_ledSlideFlag()				       (SystemFlags4&=~bLedSlideBit)

//===================================================================================
typedef enum
{
    eBOOT_STATUS_NONE,  // Not valid
    eBOOT_STATUS_ACON,  // AC
    eBOOT_STATUS_DCOFF, // AC -> enter DC off
    eBOOT_STATUS_DCON,  // DC wakeup
    eBOOT_STATUS_DPMSON,// DPMS wakeup
}eBOOT_STATUS_TYPE;
extern eBOOT_STATUS_TYPE g_eBootStatus;
//===================================================================================
extern bit   g_bInputSOGFlag;
extern BYTE g_ucTimer0_TH0;
extern BYTE g_ucTimer0_TL0;
extern BYTE xdata g_u8SystemSpeedMode;
extern volatile bit g_bMcuPMClock;

extern bit ms10Flag;
#define Set_ms10Flag()          (ms10Flag = 1)
#define Clr_ms10Flag()          (ms10Flag = 0)

extern bit ms20Flag;
#define Set_ms20Flag()          (ms20Flag = 1)
#define Clr_ms20Flag()          (ms20Flag = 0)

extern bit ms50Flag;
#define Set_ms50Flag()          (ms50Flag = 1)
#define Clr_ms50Flag()          (ms50Flag = 0)

extern bit ms500Flag;
#define Set_ms500Flag()         (ms500Flag = 1)
#define Clr_ms500Flag()         (ms500Flag = 0)

#if ENABLE_DP_INPUT
extern bit g_bDoDPInit;
#endif

#if ENABLE_DAISY_CHAIN
extern BYTE xdata g_u8DCMSTFlag;
extern BYTE xdata g_u8DCMSTCheckFlag;
#endif

extern bit g_bAPPWindow_Init;

extern BYTE xdata g_u8DDCCIAControl;

extern volatile WORD ms_Counter; // for System timer
extern volatile WORD PanelOffOnCounter;

extern BYTE xdata g_u8SyncLossOsdCounter;

extern xdata WORD CFontData[18];
extern WORD xdata font_shift;
extern WORD xdata font_offset;
extern WORD xdata TotalFontWidth;
extern BYTE xdata PropFontRamWidth;
extern BYTE xdata FontUsedPixel;
extern BYTE xdata font_Norm_Up_Down ; // 0: norm  , 1: half of up, 2:half of down.

extern BYTE xdata PropFontIndex;

extern BYTE xdata ePMWakeupStatus;
#if 1//SleepMode_Enable
extern WORD xdata SleepModeCounter;
#endif
extern BYTE xdata g_u8Second;
extern WORD xdata g_u16OsdCounter;
extern WORD xdata g_u16DDCCIFunFlags;
extern BYTE xdata g_u8SwitchPortCounter;
extern BYTE xdata g_u8PowerDownCounter;
extern volatile bit bPanelOffOnFlag;
extern bit bPanelOnFlag;
extern BYTE xdata TPDebunceCounter;

extern BYTE xdata g_u8SaveIndex;
extern BYTE xdata g_u8HoldKeyCounter;
extern xdata BYTE KeypadButton;
extern BYTE xdata g_u8PositionNo;
extern BYTE xdata g_u8PatternNo;

#if ENABLE_MMD_DCR
extern BYTE xdata DCRBacklight;
extern BYTE xdata DCRDarkBacklight;
extern BYTE xdata DCRMinBacklight;
extern BYTE xdata DCRMaxBacklight;
extern BYTE xdata DCR_TargetValue;

extern WORD xdata HighContCounter;
extern BYTE xdata BacklightOffCounter;
extern WORD xdata DCRWaitBLReadyCounter;
extern BYTE xdata DCRWaitBLOffCounter;
extern BYTE xdata DCR_Status;
#endif


extern volatile WORD DelayCounter;
extern volatile bit bDelayFlag;
extern volatile WORD xdata TimeOutCounter;

extern BYTE xdata g_u8SaveUserData;
#if PANEL_VBY1()
extern BYTE xdata g_u8VBY1CheckFlag;
#endif
#if ENABLE_DAC
#if EnableDACDePop
extern volatile WORD u16AudioDePopCounter;
extern bit bDAC_DePopCountFlag;
extern volatile bit bAudioDePopFlag;

#if (CHIP_ID == MST9U3) || (CHIP_ID == MST9U4)
extern volatile bit bAudioWakeUpDePopFlag;
#endif

#define SetDAC_DePopCounter(x) ((x)?(bDAC_DePopCountFlag=0,u16AudioDePopCounter=x,bDAC_DePopCountFlag=1):(bDAC_DePopCountFlag=0))
#endif
extern volatile BYTE xdata audio_EnableAudioAfterSignalLock_Flag;
#endif
#if (ENABLE_MSTV_UART_DEBUG && ENABLE_TIME_MEASUREMENT)
extern WORD volatile g_u16TimeMeasurementCounter;
extern bit bTimeMeasurementFlag;
 #define SetTimeMeasurementCounter(x) ((x)?(bTimeMeasurementFlag=0,g_u16TimeMeasurementCounter=x,bTimeMeasurementFlag=1):(bTimeMeasurementFlag=0))

extern WORD g_u16Time1;
extern WORD g_u16Time2;
#define INIT_TIMER()           SetTimeMeasurementCounter(0xFFFF);
#define GET_T1()                g_u16Time1=g_u16TimeMeasurementCounter
#define GET_T2()                g_u16Time2=g_u16TimeMeasurementCounter
#define PRINT_DT()              printf(" Delta T=%d\r\n",(g_u16Time1-g_u16Time2))
#define TIME_MEASURE_BEGIN()    {INIT_TIMER();GET_T1();}
#define TIME_MEASURE_END()      {GET_T2();PRINT_DT();}
#else
#define TIME_MEASURE_BEGIN()
#define TIME_MEASURE_END()
#endif

#if (ZUI)
extern U16 g_u16ZUIDebounceTime;
extern U16 g_u16DTotalTime;

#define ZUI_DEBOUNCE_TIME 160 //unit:ms
#define ZUI_DEBOUNCE_STEP_DURATION 20 //unit:step
#endif

extern BYTE xdata LanguageIndex;
extern BYTE xdata SecondTblAddr;
//extern BYTE xdata u8BacklightNow;
//extern BYTE xdata u8BacklightPrev;
extern BYTE g_u8NoUse;
#define SKPollingInterval   50 // unit: ms

#if 0//ENABLE_TOUCH_KEY
#if TOUCH_KEY_POWER_KEY_DEBOUNCE
extern bit bPowerKeyPressed;
extern BYTE xdata PowerKeyDebounceCntr;
#endif
extern BYTE xdata TouchKeyRestortCounter;

    #if ENABLE_LOW_CONTACT
    extern BYTE xdata TouchKeyLowContactDelay; // unit: 50mS
    extern BYTE xdata TouchKeyLowContactDetectCntr; // unit: 1S, use to detect low contact peroid
    #define TOUCH_KEY_LOW_CONTACT_DETECT_PEROID     20 // unit: 1S
    #endif


    #if TOUCH_KEY_CTRL_LED
    //extern BYTE xdata TouchKeySetLEDStatus;
    typedef enum
    {
        TouchKeyLEDStatus_OFF,
        TouchKeyLEDStatus_AMBER_ON,
        TouchKeyLEDStatus_GREEN_ON,
    }TouchKeyLEDStatusType;
    #endif
#endif
//===================================================================================
extern InputModeType code StandardMode[];
extern InputResolutionType code StandardModeResolution[];

//extern PanelSettingType xdata PanelSetting;

extern Bool  INPUT_IS_DP_DRR(BYTE InputPort);
extern BYTE GetPow2(BYTE expvalue);
extern BYTE GetLog(DWORD value);
extern BYTE GetLsbBitPos(DWORD value); // 0:0, 0:BIT0, 1:BIT1, 2:BIT2, ...
extern BYTE TransNumToBit(BYTE value);
extern BYTE GetMulWindowInputPortNums(void);

#define WAIT_V_OUTPUT_BLANKING()                WaitVOutputBlankling()
#define WAIT_V_OUTPUT_BLANKING_START()          WaitVOutputBlanklingStart()
#define WAIT_V_INPUT_BLANKING(SCDetWin)         WaitVInputBlankling(SCDetWin)
#define WAIT_V_INPUT_BLANKING_START(SCDetWin)   WaitVInputBlanklingStart(SCDetWin)


#ifndef DDC_BUFFER_LENGTH
#define DDC_BUFFER_LENGTH   50//MaxBufLen 50 // ddc buffer length
#endif
////////////////////////////////////////////////////////////////////////////////

//=======================================================================================//
#if 0//ENABLE_OSD_3D_CONTROL
#define Def2Dto3D             0   //on off
#define Def2Dto3DGameMode     0
#define Def2Dto3DGain             2  //weakness to strongness 0~4
#define Def2Dto3DOffset           2//weakness to strongness 0~4
#define MAX_T3DOFFSET        4//weakness to strongness 0~4
#define MAX_T3DGAIN          4//weakness to strongness 0~4
#endif

//============================================================================================
extern DWORD XDATA g_u16Total_Weight[DISPLAY_MAX_NUMS], g_u16Total_Count[DISPLAY_MAX_NUMS];
extern BYTE XDATA g_u8Ymax[DISPLAY_MAX_NUMS], g_u8Ymin[DISPLAY_MAX_NUMS];
#if ENABLE_DPS
extern xdata WORD  u16DPSCurYgain ;
extern xdata BYTE  u8DPSCurBacklight;
#endif

//====================== Config for GOP OSD use ===========================
typedef enum
{
	E_GOP_OSD = 0,
#if (CHIP_ID == MST9U4)	
	E_GOP_OSD_RIGHT = 1,
#else
	E_GOP_OSD_RIGHT = 2,
#endif
} EN_GOP_OSD_TYPE;

#define GOP_GUARD_BAND       16

#endif
