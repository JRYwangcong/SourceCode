#ifndef _MDRV_TMDS_H_
#define _MDRV_TMDS_H_
#include "mhal_tmds.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define AviPktLossBound      500  //unit:ms
#define VS_PKT_LOSS_BOUND       500

typedef enum
{
    MsCombo_TMDS_TYPE_DVI      = 1, // Support to 165Mhz.
    MsCombo_TMDS_TYPE_DUAL     = 2, // Support to 33oMhz
    MsCombo_TMDS_TYPE_HDMI     = 4, // support HDMI packet.
    MsCombo_TMDS_TYPE_RESERVED = 8, // Not TMDS port.
} MsCombo_TMDS_TYPE;

typedef enum
{
    MSCombo_TMDS_COLOR_RGB         = 0,///< HDMI RGB 444 Color Format
    MSCombo_TMDS_COLOR_YUV_422     = 1,///< HDMI YUV 422 Color Format
    MSCombo_TMDS_COLOR_YUV_444     = 2,///< HDMI YUV 444 Color Format
    MSCombo_TMDS_COLOR_RESERVED    = 3,///< Reserve
    MSCombo_TMDS_COLOR_DEFAULT     = MSCombo_TMDS_COLOR_RGB,///< Default setting
    MSCombo_TMDS_COLOR_UNKNOWN     = 15,///< Unknow Color Format
} MSCombo_TMDS_COLOR_FORMAT;

typedef enum
{
    MSCombo_YUV_COLORIMETRY_ITU601=0,
    MSCombo_YUV_COLORIMETRY_ITU709,
    MSCombo_YUV_COLORIMETRY_NoData,
    MSCombo_YUV_COLORIMETRY_Max
} MSCombo_YPBPR_COLORIMETRY_TYPE;

typedef enum
{
    MsCombo_TMDS_POWER_ON              = 1,
    MsCombo_TMDS_POWER_STANDBY         = 2,
    MsCombo_TMDS_POWER_OFF             = 4,
    MsCombo_TMDS_POWER_UNKNOWN         = 8,
} MsCombo_TMDS_POWER_STATUS;

typedef enum
{
    TMDS_AUDIO_STATE_OFF = 0,
    TMDS_AUDIO_STATE_ON = 1,
} MsCombo_TMDS_AUD_STATUS;

typedef enum
{
    TMDS_DATA_OUTPUT_OFF = 0,
    TMDS_DATA_OUTPUT_ON = 1,
} MsCombo_TMDS_DATA_STATUS;

#ifdef _WORK_ON_PM_
typedef struct
{
    MsCombo_TMDS_TYPE              port_type;
    ST_COMBO_COLOR_FORMAT          input_color;
    MsCombo_TMDS_POWER_STATUS      port_power;
    MS_BOOL                        AudioOutputEnable;
    MsCombo_TMDS_AUD_STATUS        AudioOutputState;
    MsCombo_TMDS_DATA_STATUS       DataOutputState;
    WORD                           InputPacketStatus;
    BYTE                           InputPacketError;
    MS_BOOL                        ModeChange;
    MS_BOOL                        ColorChange;
    BYTE                           InputVideoMode;
    BYTE                           InputAVIInfoReceived;
    BYTE                           InputVSPktReceived;
    BYTE                           AVI_Colorimetry;
    WORD                           AVI_PktLossCnt;
    WORD                           VS_PktLossCnt;
    BYTE                           AudioErrorStatus;
    BYTE                           AudioOffDebunce;
} MsCombo_TMDS_Port;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void mdrv_tmds_SetConfiguration(BYTE ucDVIEnableIndex, BYTE ucDualEnableIndex, BYTE ucHDMIEnableIndex, BYTE ucDisplayEnableIndex);
void mdrv_tmds_Initial(void);
void mdrv_tmds_ConfigAudioPort(BYTE ucComboPort);
void mdrv_tmds_EnableAudioMute(MsCombo_TMDS_AUD_STATUS AudioStatus);
void mdrv_tmds_CheckAudio(BYTE ucComboPort);
void mdrv_tmds_Handler(void);
void mdrv_tmds_Set_HPD(BYTE ucComboSelect, MS_BOOL bSetHPD);
ST_COMBO_COLOR_FORMAT mdrv_tmds_Get_Color_Format(BYTE ucInputPort);
BYTE mdrv_tmds_Input_Is_HDMI(BYTE ucInputPort);
void mdrv_tmds_GetPacketContent(BYTE ucInputPort, EN_PACKET_DEFINE_TYPE ucPacketType, BYTE ucPacketLength, BYTE *pPacketData);
#endif
#if ENABLE_SYNC_CHECK_AGAIN
void mdrv_tmds_LoadHDCPKey(BYTE ucPortSelect, BYTE ucBcaps, BYTE *pHDCPKey, BYTE *pBKSV);
BYTE mdrv_tmds_GetDVIDualMainPort(BYTE ucPortSelect);
Bool mdrv_tmds_LoadKeyViaEfuse(BYTE* HDCP_KEY_TABLE, BYTE* HDCP_BKSV, EFUSE_SEL eFuse);
#endif
#if ENABLE_CABLE_DET
BYTE mdrv_tmds_DetectCablePlug(BYTE ucInputPort);
#endif

#endif //_MDRV_TMDS_H_


