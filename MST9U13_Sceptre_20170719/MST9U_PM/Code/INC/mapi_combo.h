///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mapi_combo.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPI_COMBO_H_
#define _MAPI_COMBO_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define COMBO_IP_MASK_PORT0             BIT0
#define COMBO_IP_MASK_PORT1             BIT1
#define COMBO_IP_MASK_PORT2             BIT2
#define COMBO_IP_MASK_PORT3             BIT3
#define COMBO_IP_MASK_PORT4             BIT4
#define COMBO_IP_MASK_PORT5             BIT5
#define COMBO_IP_MASK_VGA               BIT6

#define SET_COMBO_IP_PARAMETER(a,b)     (a |= b)
#define CLR_COMBO_IP_PARAMETER(a,b)     (a &= (~b))

#define COMBO_TMDS_USE_INTERNAL_EDID    0

#define COMBO_HDMI_EQ_VALUE             0xA
#define COMBO_DP_EQ_VALUE               3

#define COMBO_HDMI_CLK_CHANNEL_EQ       COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI_B_CHANNEL_EQ         COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI_G_CHANNEL_EQ         COMBO_HDMI_EQ_VALUE
#define COMBO_HDMI_R_CHANNEL_EQ         COMBO_HDMI_EQ_VALUE

#define COMBO_DP_LANE0_EQ               COMBO_DP_EQ_VALUE
#define COMBO_DP_LANE1_EQ               COMBO_DP_EQ_VALUE
#define COMBO_DP_LANE2_EQ               COMBO_DP_EQ_VALUE
#define COMBO_DP_LANE3_EQ               COMBO_DP_EQ_VALUE

#define COMBO_USE_DISPLAY_ID_PORT0      0
#define COMBO_USE_DISPLAY_ID_PORT1      0
#define COMBO_USE_DISPLAY_ID_PORT2      0
#define COMBO_USE_DISPLAY_ID_PORT3      0
#define COMBO_USE_DISPLAY_ID_PORT4      0
#define COMBO_USE_DISPLAY_ID_PORT5      0

#define COMBO_EDID_SIZE_VGA             1
#if(COMBO_USE_DISPLAY_ID_PORT0)
#define COMBO_EDID_SIZE_PORT0           3
#else
#define COMBO_EDID_SIZE_PORT0           2
#endif
#if(COMBO_USE_DISPLAY_ID_PORT1)
#define COMBO_EDID_SIZE_PORT1           3
#else
#define COMBO_EDID_SIZE_PORT1           2
#endif
#if(COMBO_USE_DISPLAY_ID_PORT2)
#define COMBO_EDID_SIZE_PORT2           3
#else
#define COMBO_EDID_SIZE_PORT2           2
#endif
#if(COMBO_USE_DISPLAY_ID_PORT3)
#define COMBO_EDID_SIZE_PORT3           3
#else
#define COMBO_EDID_SIZE_PORT3           2
#endif
#if(COMBO_USE_DISPLAY_ID_PORT4)
#define COMBO_EDID_SIZE_PORT4           3
#else
#define COMBO_EDID_SIZE_PORT4           2
#endif
#if(COMBO_USE_DISPLAY_ID_PORT5)
#define COMBO_EDID_SIZE_PORT5           3
#else
#define COMBO_EDID_SIZE_PORT5           2
#endif

typedef enum
{
    COMBO_IP_SELECT_VGA = 0,
    COMBO_IP_SELECT_PORT0,
    COMBO_IP_SELECT_PORT1,
    COMBO_IP_SELECT_PORT2,
    COMBO_IP_SELECT_PORT3,
    COMBO_IP_SELECT_PORT4,
    COMBO_IP_SELECT_PORT5,
    COMBO_IP_SELECT_MASK,
}EN_COMBO_IP_SELECT;


enum COMBO_INPUT_SELECT_TYPE
{
    COMBO_INPUT_SELECT_PORT0 = 0,
    COMBO_INPUT_SELECT_PORT1,
    COMBO_INPUT_SELECT_PORT2,
    COMBO_INPUT_SELECT_PORT3,
    COMBO_INPUT_SELECT_PORT4,
    COMBO_INPUT_SELECT_PORT5,
    COMBO_INPUT_SELECT_MASK,
    COMBO_INPUT_SELECT_VGA = COMBO_INPUT_SELECT_MASK,
};

typedef enum
{
    COMBO_IP_TYPE_DVI = 0,
    COMBO_IP_TYPE_DUAL,
    COMBO_IP_TYPE_HDMI,
    COMBO_IP_TYPE_MHL,
    COMBO_IP_TYPE_DP,
}EN_COMBO_IP_TYPE;

typedef enum
{
    COMBO_IP_CABLE_NONE = 0,
    COMBO_IP_CABLE_DVI,
    COMBO_IP_CABLE_HDMI,
    COMBO_IP_CABLE_MHL,
    COMBO_IP_CABLE_DP,
}EN_COMBO_IP_CABLE;

typedef enum
{
    COMBO_COLOR_FORMAT_RGB         = 0,     ///< HDMI RGB 444 Color Format
    COMBO_COLOR_FORMAT_YUV_422     = 1,     ///< HDMI YUV 422 Color Format
    COMBO_COLOR_FORMAT_YUV_444     = 2,     ///< HDMI YUV 444 Color Format
    COMBO_COLOR_FORMAT_YUV_420     = 3,     ///< HDMI YUV 420 Color Format
    COMBO_COLOR_FORMAT_RESERVED    = 4,     ///< Reserve
    COMBO_COLOR_FORMAT_DEFAULT     = COMBO_COLOR_FORMAT_RGB,///< Default setting
    COMBO_COLOR_FORMAT_UNKNOWN     = 15,    ///< Unknow Color Format
}EN_COLOR_FORMAT_TYPE;

typedef enum
{
    COMBO_COLORIMETRY_xvYCC601    = 0,    ///< HDMI xvYCC601 Colorimetry
    COMBO_COLORIMETRY_xvYCC709    = 1,    ///< HDMI xvYCC709 Colorimetry
    COMBO_COLORIMETRY_sYCC601     = 2,    ///< HDMI sYCC601 colorimetry
    COMBO_COLORIMETRY_ADOBEYCC601 = 3,    ///< HDMI Adobe YCC 601 colorimetry
    COMBO_COLORIMETRY_ADOBERBG    = 4,    ///< HDMI Adobe RGB colorimetry
    COMBO_COLORIMETRY_NONE        = 15,
}EN_COLORIMETRY_TYPE;

typedef enum
{
    COMBO_COLOR_RANGE_DEFAULT    = 0, //depend on video format;
    COMBO_COLOR_RANGE_LIMIT      = 1,
    COMBO_COLOR_RANGE_FULL       = 2,
    COMBO_COLOR_RANGE_RESERVED   = 3,
}EN_COLOR_RANGE_TYPE;

typedef enum
{
    COMBO_PACKET_NULL                       = 0x00, //depend on video format;
    COMBO_PACKET_AUDIO_CLOCK_REGENERATION   = 0x01,
    COMBO_PACKET_AUDIO_SAMPLE               = 0x02,
    COMBO_PACKET_GENERAL_CONTROL            = 0x03,
    COMBO_PACKET_ACP_PACKET                 = 0x04,
    COMBO_PACKET_ISRC1_PACKET               = 0x05,
    COMBO_PACKET_ISRC2_PACKET               = 0x06,
    COMBO_PACKET_ONE_BIT_AUDIO_SAMPLE       = 0x07,
    COMBO_PACKET_DST_AUDIO                  = 0x08,
    COMBO_PACKET_HBR_AUDIO_STREAM           = 0x09,
    COMBO_PACKET_GAMUT_METADATA             = 0x0A,
    COMBO_PACKET_VS_INFOFRAME               = 0x81,
    COMBO_PACKET_AVI_INFOFRAME              = 0x82,
    COMBO_PACKET_SPD_INFOFRAME              = 0x83,
    COMBO_PACKET_AUD_INFOFRAME              = 0x84,
    COMBO_PACKET_MJPGSRC_INFOFRAME          = 0x85,
    COMBO_PACKET_DP_MISC                    = 0xFF,
}EN_PACKET_DEFINE_TYPE;
#ifdef _WORK_ON_PM_
typedef enum
{
    COMBO_SW_RESET_NONE                 = 0,
    COMBO_SW_RESET_WORD_BOUNDARY        = BIT0,
    COMBO_SW_RESET_DP_DECODER           = BIT1,
    COMBO_SW_RESET_DVI_DECODER          = BIT2,
    COMBO_SW_RESET_MHL_AUTO_EQ          = BIT3,
    COMBO_SW_RESET_HDCP                 = BIT4,
    COMBO_SW_RESET_DVI2MUI              = BIT5,
    COMBO_SW_RESET_HDMI                 = BIT6,
    COMBO_SW_RESET_DVI_MSIC             = BIT7,
    COMBO_SW_RESET_DP_RX_SYNTHESIZER    = BIT8,
    COMBO_SW_RESET_DP_LINK_QUALITY      = BIT9,
    COMBO_SW_RESET_CLOCK_GENERATOR      = BIT10,
    COMBO_SW_RESET_DP_VPLL              = BIT11,
    COMBO_SW_RESET_DITHER               = BIT12,
    COMBO_SW_RESET_VIDEO_OUTPUT         = BIT13,
    COMBO_SW_RESET_AVG_FIFO             = BIT14,
    COMBO_SW_RESET_DP_FAST_TRAINING     = BIT15,
    COMBO_SW_RESET_DP_UNPACK            = BIT16,
    COMBO_SW_RESET_ALL                  = BIT17,
}EN_IP_SW_RESET_TYPE;
#endif

typedef struct
{
    Bool bConfigFinish;
    Bool bVGAEnableIndex;
    BYTE ucDVIEnableIndex;
    BYTE ucHDMIEnableIndex;
    BYTE ucDualEnableIndex;
    BYTE ucDPEnableIndex;
    BYTE ucHBR0EnableIndex;
    BYTE ucHBR1EnableIndex;
    BYTE ucMHLEnableIndex;
    BYTE ucDisplayEnableIndex;
}ST_COMBO_IP_PARAMETER;

typedef struct
{
    EN_COLOR_FORMAT_TYPE    ucColorType;
    EN_COLOR_RANGE_TYPE     ucColorRange;
    EN_COLORIMETRY_TYPE     ucColorimetry;
}ST_COMBO_COLOR_FORMAT;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef _WORK_ON_PM_
void msAPI_combo_IPConfiguration(Bool bDisplayEnable, EN_COMBO_IP_SELECT ucComboSelect, EN_INPUTPORT_TYPE ucComboType);
void msAPI_combo_IPInitial(void);
void msAPI_combo_IPHandler(void);
void msAPI_combo_IPTimerIsrHandler(void);
void msAPI_combo_IPPMIsrHandler(void);
void msAPI_combo_IPIsrHandler(InterruptNum eIntNum);
ST_COMBO_COLOR_FORMAT msAPI_combo_IPGetColorFormat(EN_COMBO_IP_SELECT ucComboSelect);
void msAPI_combo_IPGetPacketContent(EN_COMBO_IP_SELECT ucComboSelect, EN_PACKET_DEFINE_TYPE ucPacketType, BYTE ucPacketLength, BYTE *pPacketData);
#endif
#if ENABLE_SYNC_CHECK_AGAIN
void mapi_combo_LoadHDCPKey(void);
#endif
#if ENABLE_CABLE_DET
EN_COMBO_IP_CABLE msAPI_combo_IPGetCableDetect(EN_COMBO_IP_SELECT ucComboSelect);
#endif
#ifdef _WORK_ON_PM_
void msAPI_combo_IPControlHPD(EN_COMBO_IP_SELECT ucComboSelect, Bool bSetHPD);
void msAPI_combo_IPCreateAudioPath(EN_COMBO_IP_SELECT ucComboSelect);
void msAPI_combo_IPClockControl(EN_COMBO_IP_SELECT ucComboSelect, Bool bClockHigh);
void msAPI_combo_IPPowerOn(void);
void msAPI_combo_IPPowerDown(void);
void msAPI_combo_IPPowerSaving(void);
#endif
BYTE mapi_combo_GetComboMask(EN_COMBO_IP_SELECT ucComboSelect);
BYTE mapi_combo_GetComboInputPort(EN_COMBO_IP_SELECT ucComboSelect);
EN_COMBO_IP_TYPE mapi_combo_GetComboType(EN_COMBO_IP_SELECT ucComboSelect);
#endif


