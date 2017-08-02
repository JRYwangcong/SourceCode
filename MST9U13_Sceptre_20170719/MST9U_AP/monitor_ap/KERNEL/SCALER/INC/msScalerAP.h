#ifndef _MS_SCALERAP_H_
#define _MS_SCALERAP_H_

#ifdef _MS_SCALERAP_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif


typedef void ( *fpCb )              (void);
typedef void ( *fpSetupWindow )     (BYTE u8WinIndex, InputTimingType sInputTiming, ST_WINDOW_INFO sAutoWindow, BYTE *pbSupportTiming, ST_WINDOW_INFO *psCapWin, ST_WINDOW_INFO *psDispWin);
typedef void ( *fpSetupMPQ )        (BYTE u8WinIndex, InputTimingType sInputTiming, ST_WINDOW_INFO sAutoWindow, ST_WIN_PQ_MODE *psPQMode);
typedef void ( *fpSetupUserPref )   (BYTE u8WinIndex);
typedef void ( *fpSourceSwitch )    (BYTE u8InputPort, BYTE *pu8SwitchPortCnt, WORD *u16StableCnt, WORD u16SyncLossThd );

typedef void ( *fpCustomerSetPanelI2CCommand) (void);
#if !Enable_AutoInput_Menu	
typedef void ( *fpCustomerSetPMModeFlag)( BYTE ePMMode );
#endif
typedef void ( *fpCustomer1ms )     (void);
typedef void ( *fpCustomerDebug )    ( BYTE CMD1, BYTE CMD2 , BYTE CMD3  );
typedef void ( *fpCustomerDrawOSDAutoStatus)     (void);
typedef void ( *fpCustomerSetIsrTimingChangeStatus)     (BYTE u8InputPort);
typedef void ( *fpCustomerSetModeChangeAudioMuteOnOff)     (void);
typedef void ( *fpCustomerSetCurFrameMode ) ( BYTE InputPort );

#define DFT_HPDCLK_DIFF_PERIOD 100ul // 100 as suggested
#if (CHIP_ID == MST9U4)
#define RESET_FORCE_COLOR_FORMAT_SETTING(x) (msAPISrcForceInputFormat(x, DISABLE, 0x00))
#else
#define RESET_FORCE_COLOR_FORMAT_SETTING(x) (msAPISrcForceInputFormat(x, DISABLE, 0x00),\
                                            msAPISrcSetPixelRepetition(x, 0x00))
#endif

// call back function type
enum
{
    eCB_SetupWindow,
    eCB_SetupMPQ,
    eCB_SetupUserPref,
    eCB_SourceSwitch,
    eCB_CustomerSetPanelI2CCommand,
 #if !Enable_AutoInput_Menu	   
    eCB_CustomerSetPMModeFlag,  
#endif    
    eCB_CustomerSetIsrTimingChangeStatus,
#if AudioFunc
    eCB_CustomerSetModeChangeAudioMuteOnOff,
#endif
#if INPUT_SAME_OUT
    eCB_CustomerSetCurFrameMode,
#endif

    eCB_Customer1ms,
    eCB_CustomerDebug,
    eCB_CustomerDrawOSDAutoStatus,
    // etc ..
    enCBMaxNum,
};

// for detect off-line use
typedef enum
{
    INIT = 0,
    //WAIT_RESOURCE,
    ALLOCATED_RESOURCE,
    //RETURN_RESOURCE,
    READY_TO_DETECT,
    END_SCAN,
    //NOT_SCAN,
}DetectStatus;
struct DetectPortStruct
{
    DetectStatus Status;
    BYTE Scaler;
    WORD DetectTimeTotal;

    WORD DetectTimeInterval;
    //WORD ValidCount;
    //WORD DetectCounter;
    WORD DetectTimeStep;

    WORD Hperiod;
    WORD Vtotal;
};


/// Define scaler status
typedef enum
{
    WIN_STATUS_IDLE = 0,            // idle

    WIN_STATUS_FREERUN,
    WIN_STATUS_OUTPUT_DISPLAYOK,

}EN_WINDOW_STATUS;

typedef enum{
    eWIN_COLOR_RGB,         // RGB  0~255
    eWIN_COLOR_YUV,         // YUV
    eWIN_COLOR_YUV_422

}EN_WINDOW_COLOR;

typedef enum{
    eINPUT_COLOR_RGB,           // RGB 0~255
    eINPUT_COLOR_422,
    eINPUT_COLOR_444,
    eINPUT_COLOR_420,
    eINPUT_COLOR_RESERVED,     ///< Reserve
    eINPUT_COLOR_FORMAT_DEFAULT     = eINPUT_COLOR_RGB,///< Default setting

    eINPUT_COLOR_YPBPR,         // Ypbpr

}EN_INPUT_COLOR;

typedef enum{
    eCOLOR_RANGE_LIMIT,
    eCOLOR_RANGE_FULL,
}EN_COLOR_RANGE;


typedef enum
{
    eINPUT_YUV_COLORIMETRY_ITU601=0,
    eINPUT_YUV_COLORIMETRY_ITU709,
    eINPUT_YUV_COLORIMETRY_NODATA,
    eINPUT_YUV_COLORIMETRY_MAX
}EN_INPUT_YUV_COLORIMETRY_TYPE;

typedef enum // Inputport type, need to sync w/ EN_INPUTPORT_TYPE
{
    eAPI_SRCTYPE_ANALOG      = BIT0,
    eAPI_SRCTYPE_DVI_SINGLE  = BIT1,
    eAPI_SRCTYPE_DVI_DUAL    = BIT2,
    eAPI_SRCTYPE_HDMI        = BIT3,
    eAPI_SRCTYPE_MHL         = BIT4,
    eAPI_SRCTYPE_DP          = BIT5,
    eAPI_SRCTYPE_DPTX        = BIT6,
    eAPI_SRCTYPE_VD          = BIT7,
    eAPI_SRCTYPE_HBR2_0      = BIT8,
    eAPI_SRCTYPE_HBR2_1      = BIT9,
    eAPI_SRCTYPE_HDMI_2_0    = BIT10,
    eAPI_SRCTYPE_FAKE_DUAL   = BIT11,
    eAPI_SRCTYPE_FAKE_DUAL_S = BIT12,
    eAPI_SRCTYPE_DAISY_CHAIN = BIT13,
    eAPI_SRCTYPE_DP_MST      = BIT14,
    eAPI_SRCTYPE_VBY1        = BIT15,
    eAPI_SRCTYPE_LVDS        = BIT16,
    eAPI_SRCTYPE_RESERVED    = BIT17,
}EN_API_SRC_TYPE;

/// Define display window parameters
typedef struct
{
    BYTE                    u8DwEnable;
    EN_WINDOW_STATUS        eDwStatus;
    ST_WINDOW_INFO          sDwAutoWin;
    ST_WINDOW_INFO          sDwCaptureWin;
    ST_WINDOW_INFO          sDwDisplayWin;
    ST_WINDOW_INFO          sDwDisplayWinOri;
    BYTE                    u8DwPortIndex;          // input port index to display

    BYTE                    u8DWRotate;
    WORD                    u16DwScMaskOut;         // OP scaler mask (after mem)
    WORD                    u16DwScMaskIn;          // IP scaler mask (before mem)
    BYTE                    bDwMem12;
    BYTE                    u8DwMemFormat;
    BYTE                    u8DwMemFieldNum;
    WORD                    u16DwMemFieldMode;
    BYTE                    bDwReadCurFrame;
    // MIU balance
    BYTE                    bDwLRCross;
    BYTE                    bDwLRCenter;
    BYTE                    bDwLRCapCenter;         // Capture window centralized
    BYTE                    bDwMemBalance;
    BYTE                    bDwMemForceBase1;       // mem balance mode force using u32DwMemBase[1]

    ST_WIN_PQ_MODE          sDwPQMode;

    EN_WINDOW_COLOR         eWinColorFormat;        // window color format for data path
    EN_INPUT_COLOR          eInputColorFormat;      // input color format
    EN_COLOR_RANGE          eInputColorRange;
    // etc ...
    BYTE                    bForceYUV: 1;
    BYTE                    bDwForceRot2ScOP;       // force rotate using 2sc (both L or R) for output post h down
    BYTE                    bDwForceRot2ScIP;       // force rotate using 2sc for input catpre if H > NUM_ROT_LB. (NO H-pre scaling)
    BYTE                    bDwForce1P2ScOP;        // force MST9U_ASIC_1P using 2sc for output display.

    DWORD                   u32DwMemBase[2];        // extend for balance mode
    DWORD                   u32DwMemSize[2];
    DWORD                   u32DwFbSize[2];

    BYTE                    u8DwUnmuteCntr;         // counter for unmute, after disable freerun

    BYTE                    bDwDirectDisplay;       // display memory without input
    BYTE                    bDwMono;

    // fake capture window
    BOOL                    bDwFakeCapWinEnable;      // 1: indicate fake capture window is enabled; 0: disabled
    ST_FAKE_CAP_WIN_INFO    sDwFakeCapWin;            // user-defined fake capture window

}ST_DISPLAY_WIN;

#define IsWindowRot(WinIndex)           (g_DisplayWindow[WinIndex].u8DWRotate != ROT_NONE)
#define IsWindowRot90(WinIndex)         (g_DisplayWindow[WinIndex].u8DWRotate == ROT_90)
#define IsWindowRot270(WinIndex)        (g_DisplayWindow[WinIndex].u8DWRotate == ROT_270)
#define IsWindowRot90270(WinIndex)      ((g_DisplayWindow[WinIndex].u8DWRotate == ROT_90)||(g_DisplayWindow[WinIndex].u8DWRotate == ROT_270))
#define IsWindowHMirror(WinIndex)       ((g_DisplayWindow[WinIndex].u8DWRotate == ROT_H_MIRROR)||(g_DisplayWindow[WinIndex].u8DWRotate == ROT_HV_MIRROR))
#define IsWindowROnly(WinIndex)         (g_DisplayWindow[WinIndex].sDwDisplayWin.x>=(PanelWidth/2))
#define IsWindowLOnly(WinIndex)         ((g_DisplayWindow[WinIndex].sDwDisplayWin.x + g_DisplayWindow[WinIndex].sDwDisplayWin.width)<=(PanelWidth/2) )
#define IsWindowRotPostHDown(WinIndex)  (IsWindowRot90270(WinIndex)&&(g_DisplayWindow[WinIndex].sDwDisplayWin.width<g_DisplayWindow[WinIndex].sDwCaptureWin.height))
#define IsWindowLRCapCenter(WinIndex)   (g_DisplayWindow[WinIndex].bDwLRCapCenter)
#define IsWindowLRCenter(WinIndex)      (g_DisplayWindow[WinIndex].bDwLRCenter)
#define IsWindowLRCross(WinIndex)       (g_DisplayWindow[WinIndex].bDwLRCross)
#define IsWindowMemBal(WinIndex)        (g_DisplayWindow[WinIndex].bDwMemBalance)
#define IsWindowDirectDisplay(WinIndex) (g_DisplayWindow[WinIndex].bDwDirectDisplay)
#define IsWindowForceRot2ScOP(WinIndex) (g_DisplayWindow[WinIndex].bDwForceRot2ScOP)
#define IsWindowForceRot2ScIP(WinIndex) (g_DisplayWindow[WinIndex].bDwForceRot2ScIP)
#define IsWindowForce1P2ScOP(WinIndex)  (g_DisplayWindow[WinIndex].bDwForce1P2ScOP)

//#define IsWindowDI(WinIndex)            (g_DisplayWindow[WinIndex].sDwPQMode.u8MAMode >= SC_MA_MODE_5)

#define IsWindowDI(WinIndex)            (InterlaceModeFlag(MapWin2Port(WinIndex)) && (!IsWindowRot(WinIndex)))


INTERFACE ST_DISPLAY_WIN g_DisplayWindow[DISPLAY_MAX_NUMS];
INTERFACE fpCb CB_FuncList[enCBMaxNum];

///////////////////////////////////////////////////////////////////////
////////////////////// API functions /////////////////////////////////////
///////////////////////////////////////////////////////////////////////
INTERFACE void msAPIWinGetDisplayWin( BYTE ucWinIndex,  ST_WINDOW_INFO* ptDisplayWin );
INTERFACE void msAPIWinSetDisplayWin( BYTE ucWinIndex,  ST_WINDOW_INFO* ptDisplayWin );
INTERFACE void msAPIWinGetCaptureWin( BYTE ucWinIndex,  ST_WINDOW_INFO* ptCaptureWin );
INTERFACE void msAPIWinSetCaptureWin( BYTE ucWinIndex,  ST_WINDOW_INFO* ptCaptureWin );
INTERFACE void msAPIWinSetMemFakeCapWin(BYTE ucWinIndex, ST_FAKE_CAP_WIN_INFO sUserFakeCapWin, BOOL bUserFakeCapEable);
INTERFACE void msAPIWinGetColorSpaceInput( BYTE ucWinIndex,  EN_INPUT_COLOR* peColor, EN_COLOR_RANGE* peRange );
INTERFACE void msAPIWinSetColorSpaceInput( BYTE ucWinIndex,  EN_INPUT_COLOR eInputColor, EN_COLOR_RANGE eInputRange);
INTERFACE void msAPIWinGetColorSpaceWindow( BYTE ucWinIndex,  EN_WINDOW_COLOR* peColor );
INTERFACE void msAPIWinSetColorSpaceWindow( BYTE ucWinIndex,  EN_WINDOW_COLOR eWinColor);
INTERFACE void msAPIWinGetRotateMode( BYTE ucWinIndex,  BYTE* pucRotateMode );
INTERFACE void msAPIWinSetRotateMode( BYTE ucWinIndex,  BYTE u8Rotate );
INTERFACE void msAPIWinGetMemFormat( BYTE ucWinIndex,  BYTE* u8MemFormat );
INTERFACE void msAPIWinSetMemFormat( BYTE ucWinIndex,  BYTE u8MemFormat );
INTERFACE void msAPIWinGetMemFieldNum( BYTE ucWinIndex,  BYTE* u8MemFieldNum, WORD* u16MemFieldMode );
INTERFACE void msAPIWinSetMemFieldNum( BYTE ucWinIndex,  BYTE u8MemFieldNum, WORD u16MemFieldMode );
INTERFACE void msAPIWinGetReadCurFrameMode( BYTE ucWinIndex,  BOOL* bReadCurFrame );
INTERFACE void msAPIWinSetReadCurFrameMode( BYTE ucWinIndex,  BOOL bReadCurFrame );
INTERFACE void msAPIWinGetMem12( BYTE ucWinIndex,  BYTE* bMem12 );
INTERFACE void msAPIWinSetMem12( BYTE ucWinIndex,  BYTE bMem12 );
INTERFACE BOOL msAPIWinGetDisplayStatus( BYTE ucWinIndex);
INTERFACE WORD msAPIWinGetScMaskOut( BYTE ucWinIndex );
INTERFACE WORD msAPIWinGetScMaskIn( BYTE ucWinIndex );
INTERFACE BOOL msAPIWinGetScDwEnable( BYTE ucWinIndex );
INTERFACE void msAPIWinGetInputPort( BYTE ucWinIndex,  BYTE* pu8PortIndex );
INTERFACE void msAPIWinSetDirectDisplay( BYTE ucWinIndex, BYTE bDirectDisplay, ST_WINDOW_INFO* pCapWin );
INTERFACE void msAPIWinSetMonoDisplay( BYTE ucWinIndex, BYTE bDwMono );

INTERFACE Bool msAPIWinColorIsYUV(BYTE ucWinIndex);

INTERFACE void msAPIWinEnable( BYTE ucWinIndex, BYTE ucPortIndex, BYTE ucRotateMode );
INTERFACE void msAPIWinDisable( BYTE ucWinIndex );
INTERFACE void msAPIWinAdd( BYTE ucWinIndex, BYTE ucPortIndex, BYTE ucRotateMode, ST_WINDOW_INFO stDisplayWin );
INTERFACE void msAPIWinDel( BYTE ucWinIndex );
INTERFACE BOOL msAPIWinApply( void );
INTERFACE void msAPIWinFreerun(BYTE u8IdxWin, BYTE bEnable);
INTERFACE BOOL msAPIWinReSetup( BYTE u8IdxWin, BOOL bReDispatch, BOOL bEnFreeRun );
INTERFACE void msAPIWinSwitchPort( BYTE ucWinIndex, BYTE ucInputPort );
#if (CHIP_ID == MST9U) || (CHIP_ID == MST9U2)
INTERFACE void msAPIWinDisplayOnTop(BYTE u8IdxWin);
#endif

INTERFACE void msAPICbAttach(BYTE CbType, void *CbFunc);
INTERFACE void msAPICbInit( void );


INTERFACE void msAPISrcStatusUpdate( BYTE InputPort, EN_INPUT_STATUS InputStatus);
INTERFACE void msAPISrcSwitchUpdate( BYTE InputPort );
INTERFACE void msAPISrcFreerunFuncUpdate( BYTE InputPort );
INTERFACE void msAPISrcGetInputTiming(BYTE InputPort, InputTimingType *pType);
INTERFACE BYTE msAPISrcGetVsyncWidth(BYTE u8InputPort);
INTERFACE BYTE msAPISrcGetHsyncWidth(BYTE u8InputPort);
INTERFACE WORD msAPISrcGetHFrontPorchWidth(BYTE u8InputPort);
INTERFACE WORD msAPISrcGetVFrontPorchWidth(BYTE u8InputPort);
INTERFACE void msAPISrcSetSyncStableCnt(BYTE u8InputPort, WORD uwValidCnt, WORD uwLossCnt);
INTERFACE void msAPISrcInitTmdsHPD( BOOL bSetHPD );
INTERFACE void msAPISrcSetHPD(BYTE u8InputPort, WORD uwDurationMs);
INTERFACE void msAPISrcRterClockCtrl(BYTE u8InputPort, WORD uwDurationMs);
INTERFACE Bool msAPISrcIsCableConnected( BYTE InputPort );
INTERFACE EN_API_SRC_TYPE msAPISrcGetType( BYTE InputPort );
INTERFACE void msAPISrcSetType( BYTE InputPort, EN_API_SRC_TYPE eType, BYTE bEnable );
INTERFACE void msAPISrcFakeDualCfg(BYTE InputPortM, BYTE InputPortS, BYTE bEnable);
INTERFACE EN_INPUT_COLOR msAPISrcGetCurColorSpace( BYTE ucWinIndex );
INTERFACE Bool msAPISrcCheckHDMI422( BYTE ucInputPort );
INTERFACE EN_INPUT_COLOR msAPISrcGetPktColorSpace( BYTE ucWinIndex );
INTERFACE EN_COLOR_RANGE msAPISrcGetPktColorRange( BYTE ucWinIndex );
#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
INTERFACE void msAPISrcSetCurColorRange( BYTE ucWinIndex, EN_COLOR_RANGE eColorRange );
INTERFACE EN_COLOR_RANGE msAPISrcGetCurColorRange( BYTE ucWinIndex );
INTERFACE void msAPISrcSetCurColorimetry( BYTE ucWinIndex, EN_INPUT_YUV_COLORIMETRY_TYPE eColorimetry );
INTERFACE EN_INPUT_YUV_COLORIMETRY_TYPE msAPISrcGetCurColorimetry( BYTE ucWinIndex );
#endif

INTERFACE EN_INPUT_YUV_COLORIMETRY_TYPE msAPISrcGetPktYuvColorimetry( BYTE ucWinIndex );
INTERFACE void msAPISrcSetCurColorSpace( BYTE ucWinIndex, EN_INPUT_COLOR eColor );
INTERFACE Bool msAPISrcCheckHDMIMode( BYTE ucWinIndex );
INTERFACE WORD msAPISrcOfflineDetect(WORD u16InputPortMask,WORD u16DetectTimeEach,WORD u16DetectCount);
INTERFACE void msAPISrcOfflineDetectPort(BYTE u8InputPort);
INTERFACE BOOL msAPISrcOfflineDetectStatus(void);
INTERFACE BYTE msAPISrcGetWinMask( BYTE InputPort );
INTERFACE Bool msAPISrcCheckStatusIdle( BYTE InputPort );
INTERFACE void msAPIWinEnableFrameColor(BYTE u8IdxWin, BYTE bEnable);
INTERFACE void msAPIWinSetBackgoundColor(BYTE u8Red, BYTE u8Green, BYTE u8Blue);
INTERFACE void msAPIWinFrameColorRGB(BYTE u8IdxWin, BYTE u8Pattern);
INTERFACE void msAPIWinDrawLine(BYTE u8IdxWin, WORD u16PositionV, WORD u16PositionH, WORD u16PixCount, BYTE *pDataBGR, BYTE *pDataLSB);

INTERFACE void msAPIOSDBlendWindowPos(WORD u16HPos, WORD u16YPos);
INTERFACE void msAPIOSDBlendWindowPosRatio(BYTE u16HRatio, WORD u16VRatio);
INTERFACE void msAPIOSDBlendWindowSize(WORD u16Width, WORD u16Height);
INTERFACE void msAPIOSDBlendWindowReconfig(void);
INTERFACE void msAPIOSDBlendEnable(BOOL bEnable);
INTERFACE void msAPIOSDBlendInit(BYTE source);
INTERFACE void msAPIOSDBlendFBSwitch(BOOL bEnable);
INTERFACE BYTE msAPISrcGetDRRFPS(void);
#if ENABLE_HDMI_DRR
INTERFACE void msAPISrcHDMIDRRChk(void);
#endif
INTERFACE WORD msAPISrcGetHFreqCombo10X(BYTE u8InputPort);
#if (ENABLE_MWE)
INTERFACE void msAPIMWEWinConfig(BYTE u8IdxWin, ST_WINDOW_INFO sMWEWin, BOOL bEnableMWE);
#endif

INTERFACE void msAPISrcForceInputFormat(BYTE u8IdxWin, Bool bForceEn, WORD u16DataFormat);
#if (CHIP_ID != MST9U4)
INTERFACE void msAPISrcSetPixelRepetition(BYTE u8IdxWin, BYTE u8AssignValue);
#endif
INTERFACE BOOL msAPISrcGetPktSPDNativeColorActive(BYTE u8InputPort);
INTERFACE BOOL msAPISrcGetPktSPDLocalDimmingDisableControl(BYTE u8InputPort);

#undef INTERFACE
#endif


