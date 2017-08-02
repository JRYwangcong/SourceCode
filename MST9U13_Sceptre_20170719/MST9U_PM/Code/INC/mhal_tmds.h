#ifndef _MHAL_TMDS_H_
#define _MHAL_TMDS_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
typedef enum
{
    MSCombo_TMDS_PORT_0      = 0,
    MSCombo_TMDS_PORT_1      = 1,
    MSCombo_TMDS_PORT_2      = 2,
    MSCombo_TMDS_PORT_3      = 3,
    MSCombo_TMDS_PORT_4      = 4,
    MSCombo_TMDS_PORT_5      = 5,
    MSCombo_TMDS_PORT_MAX      = 5,
} MSCombo_TMDS_PORT_INDEX;

typedef enum
{
#if (CHIP_ID==MST9U3)
    EFUSE_0 = 0x10,
    EFUSE_1 = 0x20,
    EFUSE_BOTH = 0x30,
#else
    EFUSE_0 = 0x00,
    EFUSE_1 = 0x10,
#endif
} EFUSE_SEL;

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

#define COMBO_HDCP_KEY_SIZE             284
#define COMBO_HDCP_BKSV_SIZE            5


//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#if ENABLE_SYNC_CHECK_AGAIN
BYTE mhal_tmds_GetDVIDualMainPort(BYTE ucPortSelect);
#endif
void mhal_tmds_Initial(BYTE u8ComboPort, Bool bDual);
#if ENABLE_SYNC_CHECK_AGAIN
void mhal_tmds_LoadHDCPKey(BYTE ucPortSelect, BYTE ucBcaps, BYTE *pHDCPKey, BYTE *pBKSV);
#endif
#ifdef _WORK_ON_PM_
WORD mhal_tmds_HDMIGetPktStatus(MSCombo_TMDS_PORT_INDEX enInputPort);
BYTE mhal_tmds_HDMIGetErrorStatus(MSCombo_TMDS_PORT_INDEX enInputPort);
MS_BOOL mhal_tmds_DE_Stable(MSCombo_TMDS_PORT_INDEX enInputPort);
BYTE mhal_tmds_GetInputType(MSCombo_TMDS_PORT_INDEX enInputPort);
MS_BOOL mhal_tmds_IsDualLink(MSCombo_TMDS_PORT_INDEX enInputPort);
void mhal_tmds_DualLink_Setting(MSCombo_TMDS_PORT_INDEX enInputPort, MS_BOOL bFlag);
void mhal_tmds_PowerControl(MSCombo_TMDS_PORT_INDEX enInputPort, MS_BOOL bFlag);
void mhal_tmds_Set_HPD(BYTE ucComboSelect, MS_BOOL bSetHPD);
void mhal_tmds_reset_status(MSCombo_TMDS_PORT_INDEX enInputPort, MS_BOOL bFlag, BYTE ucReset);
MS_BOOL mhal_tmds_TimingChg(MSCombo_TMDS_PORT_INDEX enInputPort);
BYTE mhal_tmds_GetColor(MSCombo_TMDS_PORT_INDEX enInputPort);
#endif
#if ENABLE_CABLE_DET
BYTE mhal_tmds_DetectCablePlug(BYTE ucInputPort);
#endif
#if ENABLE_SYNC_CHECK_AGAIN
Bool mhal_tmds_LoadKeyViaEfuse(BYTE* HDCP_KEY_TABLE, BYTE* HDCP_BKSV, EFUSE_SEL eFuse);
Bool mhal_tmds_ClockDetect(BYTE u8ComboPort);
Bool mhal_tmds_Clock_Stable(BYTE u8ComboPort);
Bool mhal_tmds_InputIsHDMI2(BYTE u8ComboPort);
#endif

#endif //_MHAL_TMDS_H_


