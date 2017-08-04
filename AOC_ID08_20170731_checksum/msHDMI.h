/******************************************************************************
 Copyright (c) 2004 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: MsHDMI.h
 [Date]:        28-Jan-2004
 [Comment]:
   HDCP header file.
 [Reversion History]:
*******************************************************************************/
#ifndef		MSHDMI_H
#define		MSHDMI_H

#ifdef _MSHDMI_C_
#define _MSHDMIDEC_
#else
#define _MSHDMIDEC_ extern
#endif

typedef enum
{
    INPUT_RGB    = 0,   // 00
    INPUT_YUV422 = 1,   // 01
    INPUT_YUV444 = 2    // 11
};

typedef enum
{
    INPUT_NONE,
    INPUT_ADC_0,
    INPUT_TMDS_A,
    INPUT_MAX
};
typedef enum
{
    TMDS_DVI,
    TMDS_HDMI 
};
typedef enum
{
    HDMI_AUDIO_OFF,
    HDMI_AUDIO_ON 
};
typedef enum
{
    HDMI_DATA_OUTPUT_OFF,
    HDMI_DATA_OUTPUT_ON 
};

typedef struct
{
    BYTE	ColorChanged;
    BYTE    ModeChange;
    WORD    wAviPktLossCnt;
    BYTE    InputSource;
    BYTE    InputColor;
    BYTE    InputVideoMode;
    BYTE    InputTmdsType;
    WORD  InputPacketStatus;
    BYTE    AudioOffDebunce;
    BYTE    InputPacketError;
    #if CheckCTSStable
    BYTE    InputCTSValue;
    BYTE    OutputAudioCnt;
    #endif

    BYTE    OutputVideoFormat;
    //BYTE    OutputAudioType;
    BYTE    OutputAudioState;
    BYTE    OutputDataState;
    BYTE    InputAVIInfoReceived;
} SC_INFO;


_MSHDMIDEC_ SC_INFO xdata gScInfo;
_MSHDMIDEC_ BYTE idata HdmiPollingCounter;
//_MSHDMIDEC_ WORD xdata wSwitchPortCounter;
#define AviPktLossBound      500  //unit:ms
#define AviPktLossBound         500  //unit:ms
#define AudioDeBounce           5
#define CTSRange                    0x10


_MSHDMIDEC_ void mstar_HDMIInitialVariable(void);
_MSHDMIDEC_ void mstar_HDMIInitialAudio(void);
_MSHDMIDEC_ void mstar_HDMIHandler(void);
_MSHDMIDEC_ void mstar_HDMIRst(void);
_MSHDMIDEC_ void mstar_HDMIHandleMode(void);
_MSHDMIDEC_ void mstar_HDMIHandleColor(void);
_MSHDMIDEC_ void mstar_HDMIResetColorSpace(void);
_MSHDMIDEC_ void mstar_MonitorHDMI(void);
_MSHDMIDEC_ void mstar_HDMICheckAudio(void);
_MSHDMIDEC_ BYTE mstar_HDMIPacketColor(void);
_MSHDMIDEC_ void mstar_HDMIAudioMute(BYTE sw);
_MSHDMIDEC_ void mstar_HDMIGetPktStatus(void);
_MSHDMIDEC_ BYTE mstar_HDMITmdsGetType(void);

_MSHDMIDEC_ void SetMainWinVideoDomain(BYTE Domain);
#endif
