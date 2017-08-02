///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mhal_combo.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_COMBO_H_
#define _MHAL_COMBO_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
enum COMBO_DP_HBR2_ENGINE_TYPE
{
    COMBO_DP_HBR2_ENGINE_0 = 0,
    COMBO_DP_HBR2_ENGINE_1,
    COMBO_DP_HBR2_ENGINE_2,
    COMBO_DP_HBR2_ENGINE_3,
    COMBO_DP_HBR2_ENGINE_NONE,
};

#define COMBO_DP_HBR2_OFFSET(a)             (a *0x600)
#define COMBO_DP_PHY_OFFSET(a)              (a *0x100)
#define COMBO_DP_LINK_OFFSET(a)             (a *0x600)

#if 1
#define COMBO_HDCP_CPU_WRITE_ENABLE(Offset, Value, Mask)    (msWriteByteMask(REG_16040F+Offset, Value, Mask))
#define COMBO_HDCP_CPU_RW_ADDRESS(Offset, Value)            (msWrite2ByteMask(REG_16040C+Offset, Value, 0x03FF))
#define COMBO_HDCP_LOAD_ADDR_GEN_PULSE(Offset)              (msWriteByte(REG_16043A+Offset, 0x10))
#define COMBO_HDCP_CPU_WRITE_BKSV_START(Offset)             (COMBO_HDCP_CPU_WRITE_ENABLE(Offset, 0x80, 0xC0), COMBO_HDCP_CPU_RW_ADDRESS(Offset, 0), COMBO_HDCP_LOAD_ADDR_GEN_PULSE(Offset))

#define COMBO_HDCP_XIU_WRITE_KEY_ENABLE()   (msWriteByteMask(REG_160408,BIT5,BIT5), msWrite2ByteMask(REG_162E00, 0, 0x03FF))
#define COMBO_HDCP_XIU_WRITE_KEY_DISABLE()  (msWriteByteMask(REG_160408,0,BIT5))
#define COMBO_HDCP_XIU_WRITE_KEY(Value)     (msWriteByte(REG_162E02,Value))

#define COMBO_HDCP_CPU_WRITE_DATA(Offset, Value)           (msWriteByte( REG_16040E+Offset, Value ), msWriteByte(REG_16043A+Offset, 0x08))
#define COMBO_HDCP_CPU_WRITE_BKSV(Offset, Address, Value)  (COMBO_HDCP_CPU_WRITE_DATA(Offset, Value))
#define COMBO_HDCP_CPU_WR_BUSY(Offset)                     (_bit4_(MEM_MSREAD_BYTE(REG_16042E+Offset)))
#define COMBO_HDCP_CPU_WRITE_BCAPS(Offset, Value)          {msWriteByteMask(REG_16040F+Offset, 0x80, 0xC0);\
                                                                    msWrite2ByteMask(REG_16040C+Offset, 0x0040, 0x03FF);\
                                                                    msWriteByte(REG_16043A+Offset, 0x10);\
                                                                    msWriteByte(REG_16040E+Offset, Value);\
                                                                    msWriteByte(REG_16043A+Offset, 0x08);}
#define COMBO_HDCP_MISC_SETTING(Offset, Value)             (msWrite2Byte( REG_160400+Offset, Value ))
#define COMBO_HDCP_ENABLE_DDC(Offset)                      (msWriteByteMask( REG_16040D+Offset, BIT2, BIT2 ))//(_msWriteByteMask( REG_3E74, BIT5, BIT5 ))

// dvi
#define COMBO_DVI_A_RCK_CTRL(On)  (msWriteByteMask( REG_001781, ((On) ? 0 : BIT0), BIT0 )) //PortA clk
#define COMBO_DVI_B_RCK_CTRL(On)  (msWriteByteMask( REG_001781, ((On) ? 0 : BIT1), BIT1 )) //PortB clk
#define COMBO_DVI_C_RCK_CTRL(On)  (msWriteByteMask( REG_001781, ((On) ? 0 : BIT2), BIT2 )) //PortC clk
#define COMBO_DVI_D_RCK_CTRL(On)  (msWriteByteMask( REG_001781, ((On) ? 0 : BIT3), BIT3 )) //PortD clk
#define COMBO_DVI_E_RCK_CTRL(On)  (msWriteByteMask( REG_001781, ((On) ? 0 : BIT4), BIT4 )) //PortE clk
#define COMBO_DVI_F_RCK_CTRL(On)  (msWriteByteMask( REG_001781, ((On) ? 0 : BIT5), BIT5 )) //PortF clk
#endif
//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void mhal_combo_LoadEDID(BYTE ucPortSelect, Bool bDPInput, BYTE ucEDIDSize, BYTE ucEDIDAddr, BYTE *pComboEDID);
void mhal_combo_LoadHDCPKey(BYTE ucPortSelect, Bool bDPInput, BYTE ucBcaps, BYTE *pHDCPKey, BYTE *pBKSV);
void mhal_combo_ClockEnable(BYTE ucPortSelect, Bool bMainSwitch);
void mhal_combo_AudioInitial(void);
void mhal_combo_ClockRtermControl(BYTE ucPortSelect, Bool bRtermEnable);
void mhal_combo_DataRtermControl(BYTE ucPortSelect, Bool bRtermEnable);
void mhal_combo_DoSWReset(BYTE ucPortSelect, DWORD ulSWResetValue);
void mhal_combo_ConfigAudioSource(Bool bIsTMDS, BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_combo_ConfigAPLLSynthesizer(Bool bIsTMDS, BYTE ucPortSelect, BYTE ucHBR2Select);
void mhal_combo_ConfigFadingEvent(Bool bEnable, BYTE ucEvents);
void mhal_combo_ConfigMuteEvent(Bool bEnable, BYTE ucEvents);
void mhal_combo_ConfigGlobalMute(Bool bEnable);
BYTE mhal_combo_GetDVIDualMainPort(BYTE ucPortSelect);

#endif


