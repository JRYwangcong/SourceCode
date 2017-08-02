///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mhal_tmds.c
/// @author MStar Semiconductor Inc.
/// @brief  TMDS driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_TMDS_C_
#define _MHAL_TMDS_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include <string.h>
#include "mhal_tmds.h"
#include "mapi_combo.h"
#include "mhal_mhl.h"

#if ENABLE_SYNC_CHECK_AGAIN

#define EFUSE_DEBUG    0
#if ENABLE_DEBUG&&EFUSE_DEBUG
    #define EFUSE_printData(str, value)   printData(str, value)
    #define EFUSE_printMsg(str)           printMsg(str)
#else
    #define EFUSE_printData(str, value)
    #define EFUSE_printMsg(str)
#endif

#define _msEfuseReadByte(u16Addr)                     msReadByte(u16Addr)
#define _msEfuseWriteByte(u16Addr,u8Value)            msWriteByte(u16Addr, u8Value)
#define _msEfuseWriteByteMask(u16Addr,u8Value,u8Mask) msWriteByteMask(u16Addr, u8Value, u8Mask)

#define HDCP_EFUSE_WRITE_ADDRESS(Value)    ( _msEfuseWriteByte( REG_103A85, ((Value<<3)>>8) & 0x0F ), _msEfuseWriteByte(REG_103A84, ((Value<<3)&0xFF)) )
#define HDCP_EFUSE_READ_DATA()    (_msEfuseReadByte(REG_103A86))
#define HDCP_EFUSE_WAIT_READY()    ( (_msEfuseReadByte(REG_103A85) & BIT6) == BIT6 )
#define HDCP_EFUSE_PROTECT_CODE(En)  (_msEfuseWriteByte( REG_103A8A, En ? 0xCD : 0x00))
#define DWAITCNT    2
#endif

//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_bank_offset()
//  [Description]:
//                  use to get combo bank offset
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  comb bank offset
//**************************************************************************
WORD _mhal_tmds_bank_offset(BYTE u8ComboPort)
{
    WORD XDATA wOffset;

    switch(u8ComboPort)
    {
        default:
        case MUX_COMBO0:
            wOffset = 0x0000;
            break;
        case MUX_COMBO1:
            wOffset = 0x0600;
            break;
        case MUX_COMBO2:
            wOffset = 0x0E00;
            break;
        case MUX_COMBO3:
            wOffset = 0x1600;
            break;
        case MUX_COMBO4:
            wOffset = 0x1E00;
            break;
        case MUX_COMBO5:
            wOffset = 0x2600;
            break;
    }

    return wOffset;

}


//**************************************************************************
//  [Function Name]:
//                  _mhal_tmds_phy_offset()
//  [Description]:
//                  use to get combo_phy_top offset
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  combo_phy_top bank offset
//**************************************************************************
#if (CHIP_ID == MST9U3)
WORD _mhal_tmds_phy_offset(BYTE enInputPort)
{
    WORD XDATA wOffset;
    switch(enInputPort)
    {
        default:
        case MUX_COMBO0:
            wOffset = 0x0000;
            break;
        case MUX_COMBO1:
            wOffset = 0x0200;
            break;
        case MUX_COMBO2:
        case MUX_COMBO3:
            wOffset = 0x0400;
            break;
    }

    return wOffset;

}
#else
WORD _mhal_tmds_phy_offset(BYTE u8ComboPort)
{
    WORD XDATA wOffset;

    switch(u8ComboPort)
    {
        default:
        case MUX_COMBO0:
            wOffset = 0x0000;
            break;
        case MUX_COMBO1:
            wOffset = 0x0100;
            break;
        case MUX_COMBO2:
            wOffset = 0x0200;
            break;
        case MUX_COMBO3:
            wOffset = 0x0300;
            break;
        case MUX_COMBO4:
            wOffset = 0x0400;
            break;
        case MUX_COMBO5:
            wOffset = 0x0500;
            break;
    }

    return wOffset;

}
#endif
#ifdef _WORK_ON_PM_


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDMIInitialAudio()
//  [Description]:
//                  initial audio settings for HDMI
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  none
//**************************************************************************
void mhal_tmds_HDMIInitialAudio(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset = 0;



    //*** BK: PAD_TOP ***//
    #if 1//(ENABLE_HDMI_AUD_BYPASS_I2S_PAD == 1)  //20131227,Set it based on board define
    //msWriteByteMask(REG_000407, 0x03, 0x03); //reg_i2s_out_en [0]: 0:disable, 1:enable i2s output pad control on pad GPIO_AU1, AU2, AU3
    //msWriteByteMask(REG_000416, 0x0F, 0x0F); //[3:0]: reg_i2s_out_data_0~3_en, [7:4]: reg_i2s_out_data_0~3_au_en
    #else
    //i2s pad signal from DAC(Hydun)
    msWriteByteMask(REG_000407, 0xF0, 0xF0); //reg_i2s_out_en [0]: 0:disable, 1:enable i2s output pad control on pad GPIO_AU1, AU2, AU3
    msWriteByteMask(REG_000416, 0xF0, 0xF0); //[3:0]: reg_i2s_out_data_0~3_en, [7:4]: reg_i2s_out_data_0~3_au_en
    msWriteByteMask(REG_14026C, 0x10, 0x70); //reg_i2s_tx_ctrl_0[6:4]: Haydn wclk setting, 48fs
    #endif

    //*** BK: COMBO_GP_TOP ***//
    msWriteByteMask(REG_1630B2, 0x00, 0x3F); //reg_auoe[3:0]: i2s 0~7 out OEN, [4]:spdif out OEN, [5]: i2s out mute OEN
    //audio clock source selection
    msWriteByteMask(REG_163004, 0x30, (enInputPort<<4)); //reg_au_clk_sel[6:4]: audio engine source clock sel
    //sample source selection
    msWriteByteMask(REG_163088, enInputPort, 0x0F); //[3:0]: reg_audio_src_sel
    //gain
    msWriteByte(REG_1630A5, 0xC0); //**MUST SET** reg_gainp[7:0]: 0xC0 is 0 dB, default 0x00 is mute will cause i2s out data fade out
    //fading setting
    msWriteByteMask(0x1630A0, 0x08, 0x0C); // [3:2]: reg_fading_mode, 10: automatic fading mode
    msWriteByteMask(0x163093, 0x0C, 0x0C); // [3:2]: reg_fad_speed_sel, lfading speed select, 11: 8X
    msWrite2Byte(0x16308C, 0xFFFF); // reg_zero_cross[15:0]: zero-crossing threshold

    //clear status before enable fading event;
    wOffset = _mhal_tmds_phy_offset(enInputPort);
    msWriteByteMask(0x112064 + wOffset, 0x04, 0x04); // clear clock big change status

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    msWrite2ByteMask(0x1630B0, 0x3FF, 0x3FF); // clear all status;
    msWriteByteMask(0x1603EC + wOffset, 0x40, 0x40); // clear CTS/N over range
    msWriteByteMask(0x160380 + wOffset, 0x00, 0x01); // clear AV mute

    //enable fading events
    msWriteByte(0x6308E, 0xAF); //reg_hdmi_jmute_fad_out_en[7:0]:
                                //[0]: HDMI CTS/N over range / DP audio sample error
                                //[1]: HDMI TMDS clock big change / DP audio sample parity error
                                //[2]: HDMI AVMUTE / DP non-PCM
                                //[3]: HDMI no input clock / DP audio mute (VB_ID[4])
                                //[5]: HDMI audio channel status change / DP no input recovery clock
                                //[7]: HDMI audio sample error / DP audio FIFO error

    //*** BK: COMBO_PLL_TOP ***//
    //formula:  ( 12Mhz * 2^km * fbdiv ) / (2^kn * 2^kp * ddiv)
    msWriteByte(REG_162F12, 0x22); //[7:4]: reg_apll_km_reg, [3:0]:reg_apll_divkp_reg
    msWriteByteMask(REG_162F13, 0x00, 0x60); //[6:5]: reg_apll_kn_reg
    msWriteByte(REG_162F15, 0x62); //[3:0]:reg_aupll_ddiv, [7:4]:reg_aupll_fbdiv
    msWriteByteMask(REG_162F07, 0x00, 0x01); //reg_apll_pwdwn[0]: turn off power down
    msWriteByte(REG_162F0E, 0x50); //reg_apll_test_in: makes apll lock
    //CTS
    msWriteByteMask(REG_162F30, 0x80, 0x80); //reg_en_cts_n_synth[7]: enable Audio HDMI CTS-N synthesizer
    msWriteByteMask(REG_162F40, 0x00, 0x03); //reg_apll_src_sel[0]: APLL synthesizer DIN, FBIN input select, [1]: APLL input control and clock source select
    msWriteByteMask(REG_162F40, (enInputPort<<4), 0xF0); //reg_apll_ls_clk_sel[7:4]: APLL clock source selection
    msWriteByteMask(REG_162F11, 0x80, 0x80); //reg_apll_pdll_testd[7]: bypass DP audio DPLL test mode
    msWriteByteMask(REG_162F07, 0x10, 0x10); //[4]: reg_apll_freeze

    //BK:combo_top 0~5
    msWriteByteMask(REG_16064B + wOffset, 0x04, 0x04); //[2]: reg_i2s_mclk_192fs_sel: 192f2(MHL/HDMI only)

}

#endif

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_GetDVIDualMainPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
#if ENABLE_SYNC_CHECK_AGAIN
BYTE mhal_tmds_GetDVIDualMainPort(BYTE ucPortSelect)
{
    if(ucPortSelect %2) // Port 1/3/5
    {
        ucPortSelect = ucPortSelect -1;
    }
    else // Port 0/2/4
    {
        ucPortSelect = ucPortSelect +1;
    }

    return ucPortSelect;
}
#endif
//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Initial()
//  [Description]:
//                  initital setting for each combo port,
//                  if enInputPort is dual link port, also config the sub-link
//  [Arguments]:
//                  enInputPort : combo port
//                  bDual : dual link port or not
//  [Return]:
//                  None
//**************************************************************************
#if (CHIP_ID == MST9U3)

void mhal_tmds_Initial(BYTE enInputPort, Bool bDual)
{
    WORD XDATA wOffset;
    BYTE XDATA uncall=0;
    if(uncall) // for eliminate warning
    {
        wOffset = _mhal_tmds_bank_offset(enInputPort); 
    }

    wOffset = _mhal_tmds_phy_offset(enInputPort);

    //eq 3tage setting
    msWriteByteMask(REG_1120BE + wOffset, BIT6|BIT5, BIT6|BIT5); //lane0 always turn on 3 stage.
    msWriteByteMask(REG_1120C0 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane1 always turn on 3 stage.
    msWriteByteMask(REG_1120C2 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane2 always turn on 3 stage.
    msWriteByteMask(REG_1120C4 + wOffset, BIT6|BIT5, BIT6|BIT5); //lane3 always turn on 3 stage.

	msWrite2Byte(REG_163006, 0xFFFF); //Clock enable
    msWrite2Byte(REG_163008, 0x3FFF);

  //  msWrite2Byte(REG_1120DE + wOffset, 0x0F00);// overwrite
    if((enInputPort== MUX_COMBO0) || (enInputPort== MUX_COMBO1))
    {
       // if(bDual)
       // {
       //     msWrite2Byte(REG_1120D6 + wOffset, 0x0707); // [1]: reg_atop_en_clko_pix=1
       //     msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
      //      msWrite2Byte(REG_1120B4 + wOffset, 0x0000); // [2]: disable reg_atop_en_clko_tmds2x
      //  }
     //   else
    //    {
            msWrite2Byte(REG_1120D6 + wOffset, 0x0702); // [1]: reg_atop_en_clko_pix=1
            msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
            msWrite2Byte(REG_1120B4 + wOffset, 0x0000); // [2]: disable reg_atop_en_clko_tmds2x
       // }
    }
    else if((enInputPort== MUX_COMBO2) || (enInputPort== MUX_COMBO3))
    {
        msWrite2Byte(REG_1120D6 + wOffset, 0x0706); // [1]: reg_atop_en_clko_pix=1
        msWrite2Byte(REG_112014 + wOffset, 0x0040); // [6]: reg_af_ls_20out_sel=1
        msWrite2Byte(REG_1120B4 + wOffset, 0x0004); // [2]: disable reg_atop_en_clko_tmds2x
        msWrite2ByteMask(REG_163062, 0, 0xFFFF); // enable P2 all clock power
        msWriteByteMask(REG_163060, 0x05, 0x0D); // [2]: reg_pix_clk_div2_en_p2; [0]: reg_tmds_clk_div2_en_p2
        msWriteByteMask(REG_1612A8, 0x02, 0x03); // [1:0]: reg_avg_ctrl_case
    }

    if(enInputPort == MUX_COMBO0)
    {
        msWrite2ByteMask(REG_163022, 0, 0xFFFF); // enable P0 all clock power
        msWriteByteMask(REG_001724, 0, 0x40); // REG_PD_CLKIN_OFFL = 0
        msWriteByteMask(REG_112107, 0, 0x07); // [10:8]: Power on P0 data r-term
        msWriteByteMask(REG_001724, 0, 0xFF); // [7]: Power on P0 clock r-term
        msWriteByteMask(REG_001725, 0x01, 0x01); // reg_p0_pd_cmos_sw =1

    }
    else if(enInputPort == MUX_COMBO1)
    {
       msWrite2ByteMask(REG_163042, 0, 0xFFFF); // enable P1 all clock power
       msWriteByteMask(REG_001726, 0, 0x40); // REG_PD_CLKIN_OFFL = 0
       msWriteByteMask(REG_112307, 0, 0x07); // [10:8]: Power on P1 data r-term
       msWriteByteMask(REG_001726, 0, 0xFF); // [7]: Power on P1 clock r-term
       msWriteByteMask(REG_001727, 0x01, 0x01); // reg_p1_pd_cmos_sw =1
    }
    else if(enInputPort == MUX_COMBO2)
    {
        msWriteByteMask(REG_001720, 0, 0x0F); // [3:1]: Power on Port2 data r-term
        msWriteByteMask(REG_001720, 0, 0x10); // [4]: Power on Port2 clock r-term
        msWriteByteMask(REG_11251C, 0x00, 0x01); // [0]:mhl3_atop selection :0 (mhl3_atop_0)
        msWriteByteMask(REG_000461, 0x80, 0x80); // [15]: HDCP & SCDC I2C port selection :1 (PAD_GPIOX13/GPIOX14)

    }
    else if(enInputPort == MUX_COMBO3)
    {
        msWriteByteMask(REG_001722, 0, 0x0F); // [3:1]: Power on Port3 data r-term
        msWriteByteMask(REG_001722, 0, 0x10); // [4]: Power on Port3 clock r-term
        msWriteByteMask(REG_11251C, 0x01, 0x01); // [0]: mhl3_atop selection :1 (mhl3_atop_1)
        msWriteByteMask(REG_000461, 0x00, 0x80); // [15]: HDCP & SCDC I2C port selection:0 (PAD_GPIOX23/GPIOX24)

    }

    msWriteByteMask(REG_112500, 0x00, 0x0F); //[2:0]: reg_atop_pd_oscreg_atop_pd_osc; [3]: power down internal LDO
    msWriteByteMask(REG_112000 + wOffset, 0x80, 0x80); //[7]: Main link function select: HDMI/MHL0
    msWrite2Byte(REG_1120AE + wOffset, 0x010C); // [8]: enable mhl fullrate mode; [7:0]: Set Xtal freq
    msWrite2Byte(REG_112016 + wOffset, 0xFFFF); // RX Phy Clock enable
    msWrite2Byte(REG_112018 + wOffset, 0x3FFF); // RX Phy Clock enable
    msWrite2Byte(REG_1120B6 + wOffset, 0x0000); // [13]: reg_atop_pd_bg; [12]: reg_atop_pd_clkin; [11:8]: reg_atop_pd_phdac_ov; [3:0]: reg_atop_pd_lane
    msWriteByteMask(REG_1120F4 + wOffset, 0x00, 0x0F); // [3:0]: reg_atop_pd_pll
    // [0]: REG_EN_SQH ; [1]: reg_pd_clkin; [3:2]: reg_test_sqh; [7:4]: reg_sq_vth;
    // [11:8]: reg_pd_rt; [13:12]: reg_test_clkin; [14]: reg_dato_sqh;[15]: reg_clk_xtali_div2_en
    msWrite2Byte(REG_112068 + wOffset, 0x0040);
    msWrite2Byte(REG_11206A + wOffset, 0x0480); // [10]: reg_hdmi2_acdr_mode = 1
    msWrite2Byte(REG_112090 + wOffset, 0x0702); // [15:8]: reg_mainlink_crlose_thr; [15:8]: reg_mainlink_crlose_thr


#if 1 // HDMI default mode
    msWriteByteMask(REG_112020 + wOffset, 0x10, 0x10); //[4]: SWAP_DCDR_UPDN_POLITY
    msWriteByteMask(REG_112029 + wOffset, 0x00, 0x10); //[4]: PHFSM0_FAST_EN
    msWriteByteMask(REG_11202B + wOffset, 0x00, 0x10); //[4]: PHFSM1_FAST_EN
    msWriteByteMask(REG_11202D + wOffset, 0x00, 0x10); //[4]: PHFSM2_FAST_EN

#if 0
    //feedback divider for MHL mode
    msWriteByteMask(REG_1120DA + wOffset, 0, 0x10); //Disable Lane0 overwrite
    msWriteByteMask(REG_1120E1 + wOffset, 0, 0x03); //Divsel_MHL_L0 value
    msWriteByteMask(REG_1120DA + wOffset, 0, 0x20); //Disable Lane1 overwrite
    msWriteByteMask(REG_1120E1 + wOffset, 0, 0x0C); //Divsel_MHL_L1 value
    msWriteByteMask(REG_1120DA + wOffset, 0, 0x40); //Disable Lane2 overwrite
    msWriteByteMask(REG_1120E1 + wOffset, 0, 0x30); //Divsel_MHL_L2 value
    msWriteByteMask(REG_1120DA + wOffset, 0, 0x80); //Disable Lane3 overwrite
    msWriteByteMask(REG_1120E1 + wOffset, 0, 0xC0); //Divsel_MHL_L3 value
    //disable DP full rate mode
    msWriteByteMask(REG_1120DC + wOffset, 0, 0x02); //[1]: disable overwrite
    msWriteByteMask(REG_1120E5 + wOffset, 0, 0x04); //[2]: acdr_fullrate_ov

    //disable LA of each lane
    msWriteByteMask(REG_1120DC + wOffset, 0, 0x3C); //[5:2]: disable overwrite
    msWriteByteMask(REG_1120E5 + wOffset, 0, 0x78);

    //enable PHD feedback mode for HDMI new mode
    msWriteByteMask(REG_1120DD + wOffset, 0, 0x0F); //[3:0]: disable overwrite
#endif

    //R-term value over write
    msWriteByteMask(REG_1120B3 + wOffset, 0, 0x01); //[0]: overwtite
    msWriteByteMask(REG_1120B7 + wOffset, 0, 0xF0); //[7:4]: R-term value

#endif
    //CLK valid boundary
    msWrite2ByteMask(REG_112076 + wOffset, 0x1F00, 0x1FFF); // [12:0]: reg_clk_valid_u
    //CR Lock Mode
    msWrite2ByteMask(REG_11208A + wOffset, 0x1F83, 0x7FFF); // [14:8]: reg_dig_lock_time; [7]: reg_dig_lock_mode; [3:0]: reg_dig_lck_cnt_power


    //xtal divider setting, default value is ok, we don't modify here
    //msWriteByte(REG_112062 + wOffset, 0x3F);//clock_xtal_divider_HBR

    //CR lock/unlock threshold, setting default value currently
    //msWrite2Byte(0x112090 + wOffset, 0x0702); //MAINLINK_CRLOCK_THR[7:0], MAINLINK_CRLOSE_THR[8:15];

    //CR lock filter, setting default value currently
    //msWriteByteMask(0x11205D + wOffset, 0x06, 0x1F); //CR_LOCK_TH[4:0]


    //HDMI_MHL divider threshold
    if((enInputPort== MUX_COMBO0) || (enInputPort== MUX_COMBO1))
    {
        msWrite2Byte(REG_112092 + wOffset, 0x0B43); // reg_hdmi_clk_thr4, the value of cr_lock_cnt in 270Mhz
        msWrite2Byte(REG_112094 + wOffset, 0x0715); // reg_hdmi_clk_thr3, the value of cr_lock_cnt in 170Mhz
        msWrite2Byte(REG_112096 + wOffset, 0x038B); // reg_hdmi_clk_thr2 , the value of cr_lock_cnt in 85Mhz
        msWrite2Byte(REG_112098 + wOffset, 0x01C6); // reg_hdmi_clk_thr2 , the value of cr_lock_cnt in 42.5Mhz
        msWrite2Byte(REG_11209A + wOffset, 0x0716); // reg_hdmi2_clk_thr3, the value of cr_lock_cnt in 170Mhz
        msWrite2Byte(REG_11209C + wOffset, 0x01C6); // reg_hdmi2_clk_thr2, the value of cr_lock_cnt in 42.5Mhz
        msWrite2Byte(REG_11209E + wOffset, 0x012E); //r eg_hdmi2_clk_thr1, the value of cr_lock_cnt in 28.33Mhz
    }
    else if((enInputPort== MUX_COMBO2) || (enInputPort== MUX_COMBO3))
    {
        msWrite2Byte(REG_112092 + wOffset, 0x0A6B); // reg_hdmi_clk_thr4, the value of cr_lock_cnt in 250Mhz
        msWrite2Byte(REG_112094 + wOffset, 0x0715); // reg_hdmi_clk_thr3, the value of cr_lock_cnt in 170Mhz
        msWrite2Byte(REG_112096 + wOffset, 0x038B); // reg_hdmi_clk_thr2 , the value of cr_lock_cnt in 85Mhz
        msWrite2Byte(REG_112098 + wOffset, 0x01C5); // reg_hdmi_clk_thr2 , the value of cr_lock_cnt in 42.5Mhz
        msWrite2Byte(REG_11209A + wOffset, 0x0320); // reg_hdmi2_clk_thr3, the value of cr_lock_cnt in 75Mhz
        msWrite2Byte(REG_11209C + wOffset, 0x0190); // reg_hdmi2_clk_thr2, the value of cr_lock_cnt in 37.5Mhz
        msWrite2Byte(REG_11209E + wOffset, 0x0215); //r eg_hdmi2_clk_thr1, the value of cr_lock_cnt in 50Mhz
    }

    // EQ setting initialize
    msWrite2Byte(REG_11204A + wOffset, 0x0AC8);
    msWrite2Byte(REG_112056 + wOffset, 0x0018);
    msWrite2Byte(REG_112058 + wOffset, 0x0004);
    msWrite2Byte(REG_112050 + wOffset, 0x3A83);
    msWrite2Byte(REG_11204C + wOffset, 0x080A); // [5:0]: reg_phase_check_time; [13:8]: reg_eq_fine_tune_time

    // PHASE
    msWrite2ByteMask(REG_112022 + wOffset, 0x0000, 0x1FFF); // [12]: reg_dvi_ovch0; [11:0]: reg_dvi_ch0_ph
    msWrite2ByteMask(REG_112024 + wOffset, 0x0000, 0x1FFF); // [12]: reg_dvi_ovch1; [11:0]: reg_dvi_ch1_ph
    msWrite2ByteMask(REG_112026 + wOffset, 0x0000, 0x1FFF); // [12]: reg_dvi_ovch2; [11:0]: reg_dvi_ch2_ph

    msWrite2Byte(REG_1120B8 + wOffset, 0x0000); // [3:0]: reg_atop_sel_phdclk
    msWriteByteMask(REG_112020 + wOffset, 0x18, 0x18); // [4]: reg_swap_dcdr_updn_polity; [3]: reg_enable_phase_step

    // HDMI Auto Detect
    msWrite2ByteMask(REG_112160 + wOffset, 0x0026, 0x013F); // [5]: reg_hdmi_auto_det_eq_en; [2]: reg_hdmi_auto_det_en; [1]: reg_do_auto_det_hdmi; [0]: reg_tmds_bit_valid
    msWrite2Byte(REG_112170 + wOffset, 0x00FF); // reg_hdmi_symbol_det_times
    msWrite2Byte(REG_112194 + wOffset, 0x003A); // reg_hdmi_pat_det_times1
    msWrite2Byte(REG_112196 + wOffset, 0x0055); // reg_hdmi_pat_det_times2
    msWrite2Byte(REG_112198 + wOffset, 0x001A); // reg_hdmi_under_th
    msWriteByte(REG_11216E + wOffset, 0x1F); // [7:4]: reg_hdmi_auto_det_times; [3:0]: reg_hdmi_eq_sweep_times
    msWrite2Byte(REG_112166 + wOffset, 0x001F); // reg_hdmi_symbol_det_duration


    if((enInputPort== MUX_COMBO0) || (enInputPort== MUX_COMBO1))
    {
        // PD initialize
        msWrite2ByteMask(REG_112104 + wOffset, 0x0000, 0x7777);
        msWrite2ByteMask(REG_112106 + wOffset, 0x0000, 0x0777);

        // CLKPIX_2X initialize
      //  if(bDual)
       // {
        //    msWriteByteMask(REG_112108 + wOffset, 0x01, 0x03);
       // }
       // else
      //  {
       //     msWriteByteMask(REG_112108 + wOffset, 0x00, 0x03);
      //  }

        // DPLPH0/1/2 initialize
         msWrite2ByteMask(REG_112022 + wOffset, 0x000A, 0x1FFF); // [12]: reg_dvi_ovch0; [11:0]: reg_dvi_ch0_ph
         msWrite2ByteMask(REG_112024 + wOffset, 0x000A, 0x1FFF); // [12]: reg_dvi_ovch1; [11:0]: reg_dvi_ch1_ph
         msWrite2ByteMask(REG_112026 + wOffset, 0x000A, 0x1FFF); // [12]: reg_dvi_ovch2; [11:0]: reg_dvi_ch2_ph
         msWrite2ByteMask(REG_112160 + wOffset, 0x0000, 0x013F);
         msWrite2ByteMask(REG_11206A + wOffset, 0x0280, 0x17FF);
    }



    //auto clear enz for hit coundary
    msWriteByteMask(REG_11203B + wOffset, 0x00, 0x01);

    //digital lock part
    //msWriteByteMask(0x11208A + wOffset, 0x03, 0x0F); //DIG_LCK_CNT_PWR[3:0]
    //msWriteByte(0x11208E + wOffset, 0xE1); //DIG_LOSE_RANGE[7:4], DIG_LCK_RANGE[3:0]
    //msWriteByteMask(0x11208B + wOffset, 0x1F, 0x7F); //DIG_LOCK_TIME[6:0]


    // msWriteByteMask(REG_1606A3 + wOffset, 0x80, 0x80); //word boundary detect unlock check
    msWrite2Byte(0x1630EE, 0x1FFF); //clk to scaler

    if(enInputPort == MUX_COMBO0)
    {
        msWriteByteMask(REG_16042F, BIT5, BIT5);// patch for HDCP issue, reset HDCP FSM when no input clock
        msWriteByteMask(REG_16050D, 0x0C, 0x0C); // [3] : enable video mute, [2] : blanking when AVmute is active.
        msWriteByteMask(REG_16050E, 0x03, 0x03); // [1] : auto reset deep color FIFO, [0] : enable deep color mode.
        msWriteByteMask(REG_160640, 0x00, 0x00); // [1] : disable force 422 to 444.
        msWrite2Byte(REG_1605C0, 0xFFFF); // HDMI pkt irq mask
        msWriteByteMask(REG_16042A, 0xFF, 0xFF); // HDCP pkt irq mask
        msWriteByteMask(REG_160247, 0x00, 0xFF); // word boundary unlock threshold
        msWriteByteMask(REG_120FDE, 0x01, 0x01); // [0] force clk_combo_0_rep_en
        msWriteByteMask(REG_112120, 0x07, 0x07); // eq overwrite enable
//        msWrite2ByteMask(REG_112122, (TMDS_HDMI14_R_CHANNEL_EQ<<8)|(TMDS_HDMI14_G_CHANNEL_EQ<<4)|TMDS_HDMI14_B_CHANNEL_EQ, 0x0FFF);
    }
    else if(enInputPort == MUX_COMBO1)
    {
        msWriteByteMask(REG_16092F, BIT5, BIT5);// patch for HDCP issue, reset HDCP FSM when no input clock
        msWriteByteMask(REG_160A0D, 0x0C, 0x0C); // [3] : enable video mute, [2] : blanking when AVmute is active.
        msWriteByteMask(REG_160A0E, 0x03, 0x03); // [1] : auto reset deep color FIFO, [0] : enable deep color mode.
        msWriteByteMask(REG_160B40, 0x00, 0x00); // [1] : disable force 422 to 444.
        msWrite2Byte(REG_160AC0, 0xFFFF); // HDMI pkt irq mask
        msWriteByteMask(REG_16092A, 0xFF, 0xFF); // HDCP pkt irq mask
        msWriteByteMask(REG_160747, 0x00, 0xFF); // word boundary unlock threshold
        msWriteByteMask(REG_120FDE, 0x02, 0x02); // [1] force clk_combo_1_rep_en
        msWriteByteMask(REG_112320, 0x07, 0x07); // eq overwrite enable
//        msWrite2ByteMask(REG_112322, (TMDS_HDMI14_R_CHANNEL_EQ<<8)|(TMDS_HDMI14_G_CHANNEL_EQ<<4)|TMDS_HDMI14_B_CHANNEL_EQ, 0x0FFF);
    }
    else if((enInputPort == MUX_COMBO2) || (enInputPort == MUX_COMBO3))
    {
        msWriteByteMask(REG_16102F, BIT5, BIT5); // patch for HDCP issue, reset HDCP FSM when no input clock
        msWriteByteMask(REG_16110D, 0x0C, 0x0C); // [3] : enable video mute, [2] : blanking when AVmute is active.
        msWriteByteMask(REG_16110E, 0x03, 0x03); // [1] : auto reset deep color FIFO, [0] : enable deep color mode.
        msWriteByteMask(REG_161240, 0x00, 0x00); // [1] : disable force 422 to 444.
        msWrite2Byte(REG_1611C0, 0xFFFF); // HDMI pkt irq mask
        msWriteByteMask(REG_16102B, 0xFF, 0xFF); // HDCP pkt irq mask
        msWriteByteMask(REG_120FDE, 0x04, 0x04); // [2] force clk_combo_2_rep_en
        msWriteByteMask(REG_1611CF, 0x01, 0x01); // [8]: reg_cts_div2_en
        msWriteByteMask(REG_160C32, 0x08, 0x08); // [3]: reg_dbg_tst
        msWriteByteMask(REG_11257C, 0x04, 0x04); // [2]: SCDC bypass clock tmds ratio
       // msWriteByteMask(REG_112560, 0x07, 0x07);
       // msWriteByteMask(REG_11257C, 0x00, 0x07);
      //  msWriteByteMask(REG_00180F, 0x00, 0xFF); // SCDC interrupt clear = 0

        msWriteByteMask(REG_1124DC, 0xF0, 0xF0); // eq overwrite enable
//        msWrite2ByteMask(REG_1124E6, (TMDS_HDMI20_G_CHANNEL_EQ<<8)|TMDS_HDMI20_B_CHANNEL_EQ, 0x1F1F);
//        msWrite2ByteMask(REG_1124E8, (TMDS_HDMI20_CLK_CHANNEL_EQ<<8)|TMDS_HDMI20_R_CHANNEL_EQ, 0x1F1F);
    }




    //mask interrupt
   // msWriteByte(REG_112181 + wOffset, 0xFF); //combo irq mask wOffset!!!!!!





    //word boundary by RD suggest
   // msWriteByteMask(REG_16051D + wOffset, 0xFF, 0xFF);
   // msWriteByteMask(REG_160521 + wOffset, 0xFF, 0xFF);
   // msWriteByteMask(REG_160525 + wOffset, 0xFF, 0xFF);
   // msWriteByteMask(REG_160529 + wOffset, 0xFF, 0xFF);

    if(bDual)
    {
        switch (enInputPort)
        {
            case MUX_COMBO0:
                //mhal_tmds_Initial( MUX_COMBO1, FALSE);
                msWriteByteMask(REG_160764, 0x04, 0x04); // [2]: small DE
                msWriteByteMask(REG_160A0D, 0x00, 0x0C); // [11] : enable video mute = 0; [10] : blanking when AVmute is active = 0
                #if 0   //for HDCP main/sub signal sync setting
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //sub lane setting

                    wOffset = _mhal_tmds_bank_offset(enInputPort);
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //main land setting
                #endif
            break;
            case MUX_COMBO1:
                //mhal_tmds_Initial( MUX_COMBO0, FALSE);
                msWriteByteMask(REG_160264, 0x04, 0x04); // [2]: small DE
                msWriteByteMask(REG_16050D, 0x00, 0x0C); // [3] : enable video mute = 0; [2] : blanking when AVmute is active = 0
                #if 0   //for HDCP main/sub signal sync setting
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //sub lane setting

                    wOffset = _mhal_tmds_bank_offset(enInputPort);
                    msWriteByteMask(REG_160633 + wOffset, 0, BIT7); //main lane setting
                #endif
            break;
            default:
                //invalid port index, return to prevent the code from accessing illegal register address
                //return;
            break;
        }
    }


    //switch to hdmi old mode
    #if 0
    msWriteByte(0x112020 + wOffset, 0x0000); //SWAP_DCDR_UPDN_POLITY; old mode=0, default = 1;

    msWriteByteMask(0x112029 + wOffset, 0x10, 0x10); //PHFSM0_FST_EN
    msWriteByte(0x112028 + wOffset, 0x80); //PHFSM0_FAST_INTERVAL
    msWriteByteMask(0x11202B + wOffset, 0x10, 0x10); //PHFSM1_FST_EN
    msWriteByte(0x11202A + wOffset, 0x80); //PHFSM1_FAST_INTERVAL
    msWriteByteMask(0x11202D + wOffset, 0x10, 0x10); //PHFSM2_FST_EN
    msWriteByte(0x11202C + wOffset, 0x80); //PHFSM2_FAST_INTERVAL

    msWriteByteMask(0x1120DF + wOffset, 0x0F, 0x0F); //gc_en_phd_fb_ov
    msWriteByteMask(0x1120EB + wOffset, 0x00, 0x78); //gc_phd_fb_ov
    msWriteByteMask(0x1120B8 + wOffset, 0x0F, 0x0F); //REG_SEL_PHDCLK
    #endif

    //enable new DCDR low pass filter
    //for U02:
    wOffset = _mhal_tmds_phy_offset(enInputPort);
    msWriteByteMask(REG_11205D + wOffset, 0x3E, 0x3E);  //CR lock success threshold time, to avoid CLK_Valid interrupt assert by accident.


    msWriteByte(REG_112020 + wOffset, 0x1D);
    msWriteByte(REG_112021 + wOffset, 0x00);    //[0]               reg_ph_update_timer_dis
                                                //[1]               reg_dp_dcdr_eco_en
                                                //[2]               reg_early_late_no_vote
                                                //[3]               reg_enable_phase_step
                                                //[4]               reg_swap_dcdr_updn_polity

    // upper_boundary ? filter depth, larger => PHDAC code change slow
    msWriteByte(REG_112030 + wOffset, 0xFF);
    msWriteByte(REG_112031 + wOffset, 0x00);

    // lower_boundary ? inverse of upper boundary
    msWriteByte(REG_112032 + wOffset, 0x00);
    msWriteByte(REG_112033 + wOffset, 0xFF);

}
#else
void mhal_tmds_Initial(BYTE u8ComboPort, Bool bDual)
{
    WORD XDATA wOffset;
    Bool XDATA bReInit= FALSE;

    do
    {
      wOffset = _mhal_tmds_phy_offset(u8ComboPort);

       msWriteByteMask(REG_112000 + wOffset, 0x8F, 0x8F); //[7]: Main link function select: HDMI/MHL, [3:0]: PN Swap
       msWrite2Byte(REG_112016 + wOffset, 0xFFFF); //Clock enable
       msWrite2Byte(REG_112018 + wOffset, 0x3FFF); //Clock enable
       msWriteByteMask(REG_1120AE + wOffset, 0x0C, 0xFF); //Set Xtal freq
       msWrite2Byte(REG_163006, 0xFFFF); //Clock enable
       msWrite2Byte(REG_163008, 0x3FFF);

   #if 1 // HDMI default mode
       msWriteByteMask(REG_112020 + wOffset, 0x10, 0x10); //[4]: SWAP_DCDR_UPDN_POLITY
       msWriteByteMask(REG_112029 + wOffset, 0x00, 0x10); //[4]: PHFSM0_FAST_EN
       msWriteByteMask(REG_11202B + wOffset, 0x00, 0x10); //[4]: PHFSM1_FAST_EN
       msWriteByteMask(REG_11202D + wOffset, 0x00, 0x10); //[4]: PHFSM2_FAST_EN

   #if 0
       //feedback divider for MHL mode
       msWriteByteMask(REG_1120DA + wOffset, 0, 0x10); //Disable Lane0 overwrite
       msWriteByteMask(REG_1120E1 + wOffset, 0, 0x03); //Divsel_MHL_L0 value
       msWriteByteMask(REG_1120DA + wOffset, 0, 0x20); //Disable Lane1 overwrite
       msWriteByteMask(REG_1120E1 + wOffset, 0, 0x0C); //Divsel_MHL_L1 value
       msWriteByteMask(REG_1120DA + wOffset, 0, 0x40); //Disable Lane2 overwrite
       msWriteByteMask(REG_1120E1 + wOffset, 0, 0x30); //Divsel_MHL_L2 value
       msWriteByteMask(REG_1120DA + wOffset, 0, 0x80); //Disable Lane3 overwrite
       msWriteByteMask(REG_1120E1 + wOffset, 0, 0xC0); //Divsel_MHL_L3 value
       //disable DP full rate mode
       msWriteByteMask(REG_1120DC + wOffset, 0, 0x02); //[1]: disable overwrite
       msWriteByteMask(REG_1120E5 + wOffset, 0, 0x04); //[2]: acdr_fullrate_ov

       //disable LA of each lane
       msWriteByteMask(REG_1120DC + wOffset, 0, 0x3C); //[5:2]: disable overwrite
       msWriteByteMask(REG_1120E5 + wOffset, 0, 0x78);

       //enable PHD feedback mode for HDMI new mode
       msWriteByteMask(REG_1120DD + wOffset, 0, 0x0F); //[3:0]: disable overwrite
   #endif

       if(u8ComboPort == MUX_COMBO0)
       {
           msWrite2Byte(REG_163012, 0xFFFF); //Clock enable
           msWrite2Byte(REG_163014, 0x3FFF);
           msWriteByteMask(REG_0017A6, 0, 0x01); // REG_EN_SQH = 0
           msWriteByteMask(REG_0017A7, 0, 0x01); // REG_PD_CLKIN_OFFL = 0
           msWriteByteMask(REG_0017C4, 0, 0x0E); //[3:1]: Power on P0 data r-term
           msWriteByteMask(REG_001781, 0, 0x01); //[0]: Power on P0 clock r-term
       }
       else if(u8ComboPort == MUX_COMBO1)
       {
           msWrite2Byte(REG_163022, 0xFFFF); //Clock enable
           msWrite2Byte(REG_163024, 0x3FFF);
           msWriteByteMask(REG_0017A6, 0, 0x02); // REG_EN_SQH = 0
           msWriteByteMask(REG_0017A7, 0, 0x02); // REG_PD_CLKIN_OFFL = 0
           msWriteByteMask(REG_0017C4, 0, 0xE0); //[7:4]: Power on P1 data r-term
           msWriteByteMask(REG_001781, 0, 0x02); //[1]: Power on P1 clock r-term
       }
       else if(u8ComboPort == MUX_COMBO2)
       {
           msWrite2Byte(REG_163032, 0xFFFF); //Clock enable
           msWrite2Byte(REG_163034, 0x3FFF);
           msWriteByteMask(REG_0017A6, 0, 0x04); // REG_EN_SQH = 0
           msWriteByteMask(REG_0017A7, 0, 0x04); // REG_PD_CLKIN_OFFL = 0
           msWriteByteMask(REG_0017C5, 0, 0x0E); //[3:1]: Power on P2 data r-term
           msWriteByteMask(REG_001781, 0, 0x04); //[2]: Power on P2 clock r-term
       }
       else if(u8ComboPort == MUX_COMBO3)
       {
           msWrite2Byte(REG_163042, 0xFFFF); //Clock enable
           msWrite2Byte(REG_163044, 0x3FFF);
           msWriteByteMask(REG_0017A6, 0, 0x08); // REG_EN_SQH = 0
           msWriteByteMask(REG_0017A7, 0, 0x08); // REG_PD_CLKIN_OFFL = 0
           msWriteByteMask(REG_0017A2, 0, 0x0E); //[3:1]: Power on P3 data r-term
           msWriteByteMask(REG_001781, 0, 0x08); //[3]: Power on P3 clock r-term
       }
       else if(u8ComboPort == MUX_COMBO4)
       {
           msWrite2Byte(REG_163050, 0xFFFF); //Clock enable
           msWrite2Byte(REG_163052, 0x3FFF);
           msWriteByteMask(REG_0017A6, 0, 0x10); // REG_EN_SQH = 0
           msWriteByteMask(REG_0017A7, 0, 0x10); // REG_PD_CLKIN_OFFL = 0
           msWriteByteMask(REG_0017A2, 0, 0xE0); //[7:4]: Power on P4 data r-term
           msWriteByteMask(REG_001781, 0, 0x10); //[4]: Power on P4 clock r-term
       }
       else if(u8ComboPort == MUX_COMBO5)
       {
           msWrite2Byte(REG_16305E, 0xFFFF); //Clock enable
           msWrite2Byte(REG_163060, 0x3FFF);
           msWriteByteMask(REG_0017A6, 0, 0x20); // REG_EN_SQH = 0
           msWriteByteMask(REG_0017A7, 0, 0x20); // REG_PD_CLKIN_OFFL = 0
           msWriteByteMask(REG_0017A3, 0, 0x0E); //[3:1]: Power on P5 data r-term
           msWriteByteMask(REG_001781, 0, 0x20); //[5]: Power on P5 clock r-term
       }

   #if 1
       msWriteByteMask(REG_1120D6 + wOffset, 0x02, 0x02); //[1]: REG_ATOP_EN_CLKO_PIX = 1
       msWriteByteMask(REG_1120D9 + wOffset, 0, 0x01); //[0]: REG_ATOP_EN_DP
       msWriteByteMask(REG_1120D7 + wOffset, 0, 0x01); //[0]: REG_ATOP_PD_BG
       msWriteByteMask(REG_1120D6 + wOffset, 0, 0x08); //[3]: REG_ATOP_PD_CLKIN
       msWriteByteMask(REG_1120B6 + wOffset, 0x01, 0x0F); //[3:0]: REG_ATOP_PD_LANE

   #endif
       //R-term value over write
       msWriteByteMask(REG_1120B3 + wOffset, 0, 0x01); //[0]: overwtite
       msWriteByteMask(REG_1120B7 + wOffset, 0, 0xF0); //[7:4]: R-term value

       msWriteByteMask(REG_1120B8 + wOffset, 0, 0x0F); //[3:0]: REG_ATOP_SEL_PHDCLK
   #endif

       //CR Lock Mode
       msWriteByteMask(REG_11208A + wOffset, 0x80, 0x80); //[7]: DIG lock new mode, 0: DP mode, 1:MHL/HDMI mode

       //CLK valid boundary
       //msWrite2ByteMask(REG_112074 + wOffset, 0x0886, 0x1FFF);//CLK_VALID_L, keep default value currently
       msWrite2ByteMask(REG_112076 + wOffset, 0x1F00, 0x1FFF);//CLK_VALID_U

       //xtal divider setting, default value is ok, we don't modify here
       //msWriteByte(REG_112062 + wOffset, 0x3F);//clock_xtal_divider_HBR

       //CR lock/unlock threshold, setting default value currently
       //msWrite2Byte(0x112090 + wOffset, 0x0702); //MAINLINK_CRLOCK_THR[7:0], MAINLINK_CRLOSE_THR[8:15];

       //CR lock filter, setting default value currently
       //msWriteByteMask(0x11205D + wOffset, 0x06, 0x1F); //CR_LOCK_TH[4:0]

       //HDMI_MHL divider threshold
       msWrite2Byte(REG_112092 + wOffset, 0x0B43); //HDMI_CLK_THR4, the value of cr_lock_cnt in 270Mhz
       msWrite2Byte(REG_112094 + wOffset, 0x05A0); //HDMI_CLK_THR3, the value of cr_lock_cnt in 135Mhz
       msWrite2Byte(REG_112096 + wOffset, 0x02D0); //HDMI_CLK_THR2, the value of cr_lock_cnt in 67.5Mhz
       msWrite2Byte(REG_112098 + wOffset, 0x0168); //HDMI_CLK_THR1, the value of cr_lock_cnt in 33.75Mhz

       msWriteByteMask(REG_1120DC + wOffset, 0xC0, 0xC0); //atop_gc_eq_l0_ove and atop_gc_eq_l1_ove
       msWriteByteMask(REG_1120DD + wOffset, 0x03, 0x03); //atop_gc_eq_l2_ove and atop_gc_eq_l3_ove
       msWrite2Byte(REG_1120E6 + wOffset, (COMBO_HDMI_R_CHANNEL_EQ<<10)|(COMBO_HDMI_G_CHANNEL_EQ<<5)|COMBO_HDMI_B_CHANNEL_EQ);
       msWriteByteMask(REG_1120E8 + wOffset, COMBO_HDMI_CLK_CHANNEL_EQ, 0x1F);

       //auto clear enz for hit coundary
       msWriteByteMask(REG_11203B + wOffset, 0x00, 0x01);

       //digital lock part
       //msWriteByteMask(0x11208A + wOffset, 0x03, 0x0F); //DIG_LCK_CNT_PWR[3:0]
       //msWriteByte(0x11208E + wOffset, 0xE1); //DIG_LOSE_RANGE[7:4], DIG_LCK_RANGE[3:0]
       //msWriteByteMask(0x11208B + wOffset, 0x1F, 0x7F); //DIG_LOCK_TIME[6:0]

       wOffset = _mhal_tmds_bank_offset(u8ComboPort);
       msWriteByteMask(REG_1606A3 + wOffset, 0x80, 0x80); //word boundary detect unlock check
       msWrite2Byte(REG_1630EE, 0x1FFF); //clk to scaler

       msWriteByteMask(REG_160304 + wOffset, 0x03, 0x03); //[1] : auto reset deep color FIFO, [0] : enable deep color mode.
       msWriteByteMask(REG_16032A + wOffset, 0x02, 0x02); //[1] : force 422 to 444.

       //mask interrupt
       msWrite2Byte(REG_16065C + wOffset, 0xFFFF); //combo irq mask
       msWrite2Byte(REG_160318 + wOffset, 0xFFFF); //HDMI pkt irq mask
       msWriteByteMask(REG_160409 + wOffset, 0xFF, 0xFF); //HDCP pkt irq mask

       //word boundary by RD suggest
       msWriteByteMask(REG_16051D + wOffset, 0xFF, 0xFF);
       msWriteByteMask(REG_160521 + wOffset, 0xFF, 0xFF);
       msWriteByteMask(REG_160525 + wOffset, 0xFF, 0xFF);
       msWriteByteMask(REG_160529 + wOffset, 0xFF, 0xFF);
       if(bDual)
       {
           bReInit = TRUE;
           if(u8ComboPort == MUX_COMBO0)
                u8ComboPort = MUX_COMBO1;
           else if(u8ComboPort == MUX_COMBO2)
                u8ComboPort = MUX_COMBO3;
           else if(u8ComboPort == MUX_COMBO4)
                u8ComboPort = MUX_COMBO5;
           else
           {
                 //printMsg("ERROR: Port Parameter Error for Dual Link Setting");
                 bReInit = FALSE;
           }
       }
    }while(bReInit);


       //switch to hdmi old mode
       #if 0
       msWriteByte(0x112020 + wOffset, 0x0000); //SWAP_DCDR_UPDN_POLITY; old mode=0, default = 1;

       msWriteByteMask(0x112029 + wOffset, 0x10, 0x10); //PHFSM0_FST_EN
       msWriteByte(0x112028 + wOffset, 0x80); //PHFSM0_FAST_INTERVAL
       msWriteByteMask(0x11202B + wOffset, 0x10, 0x10); //PHFSM1_FST_EN
       msWriteByte(0x11202A + wOffset, 0x80); //PHFSM1_FAST_INTERVAL
       msWriteByteMask(0x11202D + wOffset, 0x10, 0x10); //PHFSM2_FST_EN
       msWriteByte(0x11202C + wOffset, 0x80); //PHFSM2_FAST_INTERVAL

       msWriteByteMask(0x1120DF + wOffset, 0x0F, 0x0F); //gc_en_phd_fb_ov
       msWriteByteMask(0x1120EB + wOffset, 0x00, 0x78); //gc_phd_fb_ov
       msWriteByteMask(0x1120B8 + wOffset, 0x0F, 0x0F); //REG_SEL_PHDCLK
       #endif

    //mhal_tmds_HDMIInitialAudio(u8ComboPort);

    }
#endif

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_LoadHDCPKey()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
#if ENABLE_SYNC_CHECK_AGAIN
#if (CHIP_ID == MST9U3)
void mhal_tmds_LoadHDCPKey(BYTE ucPortSelect, BYTE ucBcaps, BYTE *pHDCPKey, BYTE *pBKSV)
{
    //static Bool bKeyFlag = FALSE;
    //static Bool bP2LoadKey = FALSE;
    WORD ustemp = 0;
    // Load HDCP key
   // if(!bKeyFlag)
    if(ucPortSelect == COMBO_INPUT_SELECT_PORT0)
    {
        if(pHDCPKey != NULL)
        {
 //XIU
            msWriteByteMask(REG_163080, MASKBIT(3:2)| BIT0, MASKBIT(3:2)| BIT0); // REG_COMBO_GP_40[0][2][3]: enable CPU write SRAM by XIU
            
            //msWrite2ByteMask(REG_162E00, 0, MASKBIT(9:0)); // REG_COMBO_HDCP_00[9:0]: address
            
            for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
            {
                msWriteByteMask(REG_162E02, pBKSV[ustemp], 0xFF); // REG_COMBO_HDCP_01[7:0]: the data for CPU write into HDCP KEY SRAM through XIU
            }
            
            for(ustemp = 0; ustemp < COMBO_HDCP_KEY_SIZE; ustemp++)
            {
                msWriteByteMask(REG_162E02, pHDCPKey[ustemp], 0xFF); // REG_COMBO_HDCP_01[7:0]: the data for CPU write into HDCP KEY SRAM through XIU
            }
            
            msWriteByteMask(REG_163080, 0x00, MASKBIT(3:2) |BIT0); // REG_COMBO_GP_40[0][2][3]: disable CPU write SRAM by XIU
//#endif
           // bKeyFlag = TRUE;
        }
    }

    // Load Bksv and Bcaps
    if(pBKSV != NULL)
    {
        switch(ucPortSelect)
        {
            case COMBO_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_16042F, BIT2, BIT2); // REG_COMBO_DVI_HDCP_P0_17[10]: HDCP enable for DDC
                // 74 write enable
                msWriteByteMask(REG_160433, BIT7, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_P0_19[14][15]
                
                for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
                {
                    // 74 address: 0x00
                    msWrite2ByteMask(REG_16042E, ustemp, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_P0_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                    // bksv
                    msWriteByte(REG_160430, pBKSV[ustemp]); // REG_COMBO_DVI_HDCP_P0_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                  
                    msWriteByteMask(REG_160432, BIT5, BIT5); // REG_COMBO_DVI_HDCP_P0_19[5]: HDCP address load pulse generate                   
            	    msWriteByteMask(REG_160432, BIT4, BIT4); // REG_COMBO_DVI_HDCP_P0_19[4]: HDCP data write port pulse generate            		  
                }
                // 74 address: 0x40
                msWrite2ByteMask(REG_16042E, 0x40, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_P0_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                // bcaps
                msWriteByte(REG_160430, ucBcaps); // REG_COMBO_DVI_HDCP_P0_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                
                msWriteByteMask(REG_160432, BIT5, BIT5); // REG_COMBO_DVI_HDCP_P0_19[5]: HDCP address load pulse generate            
            	msWriteByteMask(REG_160432, BIT4, BIT4); // REG_COMBO_DVI_HDCP_P0_19[4]: HDCP data write port pulse generate            	
                
                // 74 write disable
                msWriteByteMask(REG_160433, 0, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_P0_19[14][15]
                //enable HDCP   
                msWrite2Byte(REG_160400, 0x0721); // REG_COMBO_DVI_HDCP_P0_00[5]: enable HDCP   
    			break;

            case COMBO_INPUT_SELECT_PORT1:
                msWriteByteMask(REG_16092F, BIT2, BIT2); // REG_COMBO_DVI_HDCP_P1_17[10]: HDCP enable for DDC
                // 74 write enable
                msWriteByteMask(REG_160933, BIT7, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_P1_19[14][15]
                
                for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
                {
                    // 74 address: 0x00
                    msWrite2ByteMask(REG_16092E, ustemp, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_P1_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                    // bksv
                    msWriteByte(REG_160930, pBKSV[ustemp]); // REG_COMBO_DVI_HDCP_P1_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                  
                    msWriteByteMask(REG_160932, BIT5, BIT5); // REG_COMBO_DVI_HDCP_P1_19[5]: HDCP address load pulse generate                   
            	    msWriteByteMask(REG_160932, BIT4, BIT4); // REG_COMBO_DVI_HDCP_P1_19[4]: HDCP data write port pulse generate            		  
                }
                // 74 address: 0x40
                msWrite2ByteMask(REG_16092E, 0x40, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_P1_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                // bcaps
                msWriteByte(REG_160930, ucBcaps); // REG_COMBO_DVI_HDCP_P1_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                
                msWriteByteMask(REG_160932, BIT5, BIT5); // REG_COMBO_DVI_HDCP_P1_19[5]: HDCP address load pulse generate            
            	msWriteByteMask(REG_160932, BIT4, BIT4); // REG_COMBO_DVI_HDCP_P1_19[4]: HDCP data write port pulse generate            	
                
                // 74 write disable
                msWriteByteMask(REG_160933, 0, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_P1_19[14][15]
                //enable HDCP   
                msWrite2Byte(REG_160900, 0x0721); // REG_COMBO_DVI_HDCP_P1_00[5]: enable HDCP   
    			break;

            case COMBO_INPUT_SELECT_PORT2:
            case COMBO_INPUT_SELECT_PORT3:  
                //if(bP2LoadKey == FALSE)
               // {
                    msWriteByteMask(REG_16102F, BIT2, BIT2); // REG_COMBO_DVI_HDCP_DUAL_17[10]: HDCP enable for DDC
                    // 74 write enable
                    msWriteByteMask(REG_161033, BIT7, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_DUAL_19[14][15]
                
                    for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
                    {
                        // 74 address: 0x00
                        msWrite2ByteMask(REG_16102E, ustemp, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_DUAL_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                        // bksv
                        msWriteByte(REG_161030, pBKSV[ustemp]); // REG_COMBO_DVI_HDCP_DUAL_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                  
                        msWriteByteMask(REG_161032, BIT5, BIT5); // REG_COMBO_DVI_HDCP_DUAL_19[5]: HDCP address load pulse generate                   
            	        msWriteByteMask(REG_161032, BIT4, BIT4); // REG_COMBO_DVI_HDCP_DUAL_19[4]: HDCP data write port pulse generate            		  
                    }
                    // 74 address: 0x40
                    msWrite2ByteMask(REG_16102E, 0x40, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_DUAL_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                    // bcaps
                    msWriteByte(REG_161030, ucBcaps); // REG_COMBO_DVI_HDCP_DUAL_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                
                    msWriteByteMask(REG_161032, BIT5, BIT5); // REG_COMBO_DVI_HDCP_DUAL_19[5]: HDCP address load pulse generate            
            	    msWriteByteMask(REG_161032, BIT4, BIT4); // REG_COMBO_DVI_HDCP_DUAL_19[4]: HDCP data write port pulse generate            	
                
                    // 74 write disable
                    msWriteByteMask(REG_161033, 0, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_DUAL_19[14][15]
                    //enable HDCP   
                    msWrite2Byte(REG_161000, 0x0721); // REG_COMBO_DVI_HDCP_DUAL_00[5]: enable HDCP  
                 //   bP2LoadKey = TRUE;                
              //  }
                
                break;

            default:

                break;
        };
    }
}
#else
void mhal_tmds_LoadHDCPKey(BYTE ucPortSelect, BYTE ucBcaps, BYTE *pHDCPKey, BYTE *pBKSV)
{
  //  static Bool bDoneFlag = 0;

    if(pHDCPKey != NULL)
    {
		BYTE ucValue;
		WORD wCount0;
		WORD wOffset;
        
        wOffset = _mhal_tmds_phy_offset(ucPortSelect);

        // clock enable
		msWrite2Byte(REG_112016 + wOffset, 0xFFFF); //Clock enable
        msWrite2Byte(REG_112018 + wOffset, 0x3FFF); //Clock enable

        //Bksv need to write in 74REG for HDMI/DVI
        wOffset = _mhal_tmds_bank_offset(ucPortSelect);

		msWriteByteMask(REG_16040D + wOffset, 0x04, 0x04); //HDCP enable  for DDC
		msWriteByteMask(REG_16040F + wOffset, 0x80, 0xC0); //enable CPU write

		msWriteByteMask(REG_16040C + wOffset, 0x00, 0xFF); //bksv address
		msWriteByteMask(REG_16040D + wOffset, 0x00, 0x03); //bksv address
        msWriteByteMask(REG_16043A + wOffset, 0x10, 0x10); //load address pulse

		for( wCount0 = 0; wCount0 < 5; wCount0++ )
        {
            ucValue = pBKSV[wCount0];
            msWriteByteMask(REG_16040E + wOffset, ucValue, 0xFF); //bksv
            msWriteByteMask(REG_16043A + wOffset, 0x08, 0x08); //write data pulse
            while(msReadByte(REG_16042E + wOffset) & 0x10);
	    }

        //Bcaps
        msWriteByteMask(REG_16040C + wOffset, 0x40, 0xFF); //bcaps address
		msWriteByteMask(REG_16040D + wOffset, 0x00, 0x03); //bcaps address
        msWriteByteMask(REG_16043A + wOffset, 0x10, 0x10); //load address pulse
        msWriteByteMask(REG_16040E + wOffset, ucBcaps, 0xFF); //bcaps
        msWriteByteMask(REG_16043A + wOffset, 0x08, 0x08); //write data pulse
        while(msReadByte(REG_16042E + wOffset) & 0x10);

        msWriteByteMask(REG_16040F + wOffset, 0x00, 0xC0); //disable CPU write
        msWrite2Byte(REG_160400 + wOffset, 0x0721); //enable HDCP
       

       // if(bDoneFlag == 0)
	    if(ucPortSelect == COMBO_INPUT_SELECT_PORT0)
        {
#if 0 //RIU
            msWriteByteMask(REG_163080, 0x0C, 0x0C); //enable CPU write SRAM
            msWriteByteMask(REG_163082, 0x00, 0xFF); //key address
            msWriteByteMask(REG_163083, 0x00, 0x03); //key address
            msWriteByteMask(REG_163080, 0x10, 0x10); //load address

			for( wCount0 = 0; wCount0 < 5; wCount0++ )
            {
                ucValue = pBKSV[wCount0];
                msWriteByteMask(REG_163084, ucValue, 0xFF); // bksv
                msWriteByteMask(REG_163080, 0x10, 0x10); // write data pulse
                while(msReadByte(REG_16042E + wOffset) & 0x10);
            }

			for( wCount0 = 0; wCount0 < 284; wCount0++ )
			{
                ucValue = pHDCPKey[wCount0];
			    msWriteByteMask(REG_163084, ucValue, 0xFF); // bksv
                msWriteByteMask(REG_163080, 0x10, 0x10); // write data pulse
                while(msReadByte(REG_16042E + wOffset) & 0x10);
            }

			msWriteByteMask(REG_163080, 0x00, 0x0C); //disable CPU write
#else //XIU

            msWriteByteMask(REG_163080, 0x0D, 0x0D); //enable CPU write SRAM by XIU
            msWrite2Byte(REG_162E00, 0x0000); //address

			for( wCount0 = 0; wCount0 < 5; wCount0++ )
			{
                ucValue = pBKSV[wCount0];
                msWriteByteMask(REG_162E02, ucValue, 0xFF);
			}
			for( wCount0 = 0; wCount0 < 284; wCount0++ )
			{
				ucValue = pHDCPKey[wCount0];
				msWriteByteMask(REG_162E02, ucValue, 0xFF);
			}
			msWriteByteMask(REG_163080, 0x00, 0x0D); //disable CPU write SRAM by XIU

#endif
           // bDoneFlag = 1;
		}
    }
}
#endif
#endif
#ifdef _WORK_ON_PM_
//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDMIGetPktStatus()
//  [Description]:
//                  use to get HDMI received packet status
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  [0] : mpeg infoframe
//                  [1] : audio infoframe
//                  [2] : SPD
//                  [3] : AVI infoframe
//                  [4] : GC packet
//                  [5] : Audio sample packet
//                  [6] : ACR packet
//                  [7] : Vendor specific infoframe
//                  [8] : null packet
//                  [9] : ISRC2
//                  [A] : ISRC
//                  [B] : ACP
//                  [C] : DSD
//                  [E] : Gamut metadata packet
//                  [F] : HBR
//**************************************************************************
WORD mhal_tmds_HDMIGetPktStatus(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset, temp;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    temp = msRead2Byte(REG_1603EA + wOffset);
    msWrite2Byte(REG_1603EA + wOffset, 0xFFFF);

    return temp;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_HDMIGetErrorStatus()
//  [Description]:
//                  use to return HDMI error status
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  [0] : BCH error (corrected)
//                  [1] : BCH error
//                  [2] : Checksum error
//                  [3] : Unsupported packet received
//                  [4] : Audio sample error
//                  [5] : Audio sample parity bit error
//                  [6] : Deep color FIFO overflow
//                  [7] : Deep color FIFO underflow
//**************************************************************************
BYTE mhal_tmds_HDMIGetErrorStatus(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    BYTE temp;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    temp = msRead2Byte(REG_1603EE + wOffset);
    msWriteByteMask(REG_1603EE + wOffset, 0xFF, 0xFF);

    return temp;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DE_Stable()
//  [Description]:
//                  check input timing is stable or not
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : Stable
//                  FALSE : Unstable
//**************************************************************************
MS_BOOL mhal_tmds_DE_Stable(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(msReadByte(REG_16045D + wOffset) & 0x40)
        return TRUE;
    else
        return FALSE;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetInputType()
//  [Description]:
//                  check input is DVI or HDMI
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : HDMI
//                  FALSE : DVI
//**************************************************************************
BYTE mhal_tmds_GetInputType(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    if(msRead2Byte(REG_160418 + wOffset) & 0x0001)
        return 1;
    else
        return 0;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_IsDualLink()
//  [Description]:
//                  check input is "Dual" or "Single" by VDE and HDE length
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : Dual link
//                  FALSE : Single link
//**************************************************************************
MS_BOOL mhal_tmds_IsDualLink(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);


    if( (msRead2Byte(REG_1603E2 + wOffset) & 0x0FFF) > (msRead2Byte(REG_1603DC + wOffset) & 0x0FFF)    )
        return 1;
    else
        return 0;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DualLink_Setting()
//  [Description]:
//                  use to config DVI dual link setting
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag :
//                  1 : enable dual link setting
//                  0 : disable dual link setting
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_DualLink_Setting(MSCombo_TMDS_PORT_INDEX enInputPort, MS_BOOL bFlag)
{
    WORD wOffset;

    wOffset = _mhal_tmds_phy_offset(enInputPort);
    msWriteByteMask(REG_1120D6 + wOffset, bFlag ? 0x01:0, 0x01); //Enable dual clock output
    msWriteByteMask(REG_1121B4 + wOffset, bFlag ? 0x02:0, 0x02); //Enable dual clock input
    //msWriteByteMask(REG_1120D6 + wOffset, bFlag ? 0x04:0, 0x04); //Enable pixel clock frequency double

    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        msWriteByteMask(REG_160513, bFlag ? 0x20:0, 0x30); // enable dual link data in
        msWriteByteMask(REG_160B13, bFlag ? 0x10:0, 0x30); // enable dual link data out
        msWriteByteMask(REG_1630F7, bFlag ? 0x08:0, 0xFC); // switch lane clock
    }
    else if(enInputPort == MSCombo_TMDS_PORT_2)
    {
        msWriteByteMask(REG_161313, bFlag ? 0x20:0, 0x30); // enable dual link data in
        msWriteByteMask(REG_161B13, bFlag ? 0x10:0, 0x30); // enable dual link data out
        msWriteByteMask(REG_1630F7, bFlag ? 0x20:0, 0xFC); // switch lane clock
    }
    else if(enInputPort == MSCombo_TMDS_PORT_4)
    {
        msWriteByteMask(REG_162313, bFlag ? 0x20:0, 0x30); // enable dual link data in
        msWriteByteMask(REG_162B13, bFlag ? 0x10:0, 0x30); // enable dual link data out
        msWriteByteMask(REG_1630F7, bFlag ? 0x80:0, 0xFC); // switch lane clock
    }

    wOffset = _mhal_tmds_bank_offset(enInputPort);
    msWriteByteMask(REG_160632 + wOffset, bFlag ? 0x10:0, 0x10); // enable of dual out FIFO

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_PowerControl()
//  [Description]:
//                  use to control TMDS clock and Data R-term
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag :
//                  1 : power on
//                  0 : power down R-term
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_PowerControl(MSCombo_TMDS_PORT_INDEX enInputPort, MS_BOOL bFlag)
{
    if(enInputPort == MSCombo_TMDS_PORT_0)
    {
        msWriteByteMask(REG_0017C4, bFlag ? 0:0x0E, 0x0E); //[3:1]: Power on P0 data r-term
        msWriteByteMask(REG_001781, bFlag ? 0:0x01, 0x01); //[0]: Power on P0 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_1)
    {
        msWriteByteMask(REG_0017C4, bFlag ? 0:0xE0, 0xE0); //[7:4]: Power on P1 data r-term
        msWriteByteMask(REG_001781, bFlag ? 0:0x02, 0x02); //[1]: Power on P1 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_2)
    {
        msWriteByteMask(REG_0017C5, bFlag ? 0:0x0E, 0x0E); //[3:1]: Power on P2 data r-term
        msWriteByteMask(REG_001781, bFlag ? 0:0x04, 0x04); //[2]: Power on P2 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_3)
    {
        msWriteByteMask(REG_0017A2, bFlag ? 0:0x0E, 0x0E); //[3:1]: Power on P3 data r-term
        msWriteByteMask(REG_001781, bFlag ? 0:0x08, 0x08); //[3]: Power on P3 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_4)
    {
        msWriteByteMask(REG_0017A2, bFlag ? 0:0xE0, 0xE0); //[7:4]: Power on P4 data r-term
        msWriteByteMask(REG_001781, bFlag ? 0:0x10, 0x10); //[4]: Power on P4 clock r-term
    }
    else if(enInputPort == MSCombo_TMDS_PORT_5)
    {
        msWriteByteMask(REG_0017A3, bFlag ? 0:0x0E, 0x0E); //[3:1]: Power on P5 data r-term
        msWriteByteMask(REG_001781, bFlag ? 0:0x20, 0x20); //[5]: Power on P5 clock r-term
    }
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Set_HPD()
//  [Description]:
//                  use to control Hot Plug pin
//  [Arguments]:
//                  enInputPort : combo port
//                  bSetHPD :
//                  1 : keep high
//                  0 : pull low
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_Set_HPD(BYTE ucComboSelect, MS_BOOL bSetHPD)
{
    if(bSetHPD)
    {
        if(ucComboSelect == 0) // combo_0
            hw_Set_HdcpHpd();
        else if(ucComboSelect == 1) // combo_1
            hw_Set_HdcpHpd1();
        else if(ucComboSelect == 2) // combo_2
            hw_Set_HdcpHpd2();
        else if(ucComboSelect == 3) // combo_3
            hw_Set_HdcpHpd3();
        else if(ucComboSelect == 4) // combo_4
            hw_Set_HdcpHpd4();
        else if(ucComboSelect == 5) // combo_5
            hw_Set_HdcpHpd5();
    }
    else
    {
        if(ucComboSelect == 0) // combo_0
            hw_Clr_HdcpHpd();
        else if(ucComboSelect == 1) // combo_1
            hw_Clr_HdcpHpd1();
        else if(ucComboSelect == 2) // combo_2
            hw_Clr_HdcpHpd2();
        else if(ucComboSelect == 3) // combo_3
            hw_Clr_HdcpHpd3();
        else if(ucComboSelect == 4) // combo_4
            hw_Clr_HdcpHpd4();
        else if(ucComboSelect == 5) // combo_5
            hw_Clr_HdcpHpd5();
    }

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_reset_status()
//  [Description]:
//                  use to reset packet content and packet status
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag : to reset packet content
//                  ucReset :
//                  [0] : reset AV mute
//                  [1] : reset color format in AVI (need clock)
//                  [2] : reset pixel repetition in AVI
//                  [3] : reset frame repetition ?
//                  [4] : reset pixel packing phase and color depth in GC packet
//                  [5] : reset deep color FIFO
//                  [6] : reserved
//                  [7] : reset packet received status
//  [Return]:
//                  None
//**************************************************************************
void mhal_tmds_reset_status(MSCombo_TMDS_PORT_INDEX enInputPort, MS_BOOL bFlag, BYTE ucReset)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(bFlag)
    {
        msWriteByteMask(REG_160310 + wOffset, 0x02, 0x02); //reset packet content
        msWriteByteMask(REG_160310 + wOffset, 0x00, 0x02);
    }

    msWriteByteMask(REG_160317 + wOffset, ucReset, 0xFF); //reset packet status
    msWriteByteMask(REG_160317 + wOffset, 0, 0xFF);

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_TimingChg()
//  [Description]:
//                  use to check timing chg
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag : to reset packet content
//  [Return]:
//                  TRUE : chg
//                  FALSE : no chg
//**************************************************************************
MS_BOOL mhal_tmds_TimingChg(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_bank_offset(enInputPort);

    if(msReadByte(REG_160657 + wOffset) & 0x02) // lose lock
    {
        msWriteByteMask(REG_16065B + wOffset, 0x02, 0x02); //clr
        msWriteByteMask(REG_16065B + wOffset, 0x00, 0x02);
        return 1;
    }
    else
        return 0;
}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_GetColor()
//  [Description]:
//                  use to check input color format from AVI packet content info
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  0 : RGB
//                  1 : YUV422
//                  2 : YUV444
//**************************************************************************
BYTE mhal_tmds_GetColor(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    WORD wOffset;
    BYTE temp;

    temp = MSCombo_TMDS_COLOR_RGB;
    wOffset = _mhal_tmds_bank_offset(enInputPort);

    switch(( msReadByte(REG_1603A2 + wOffset) & 0x60 ) >> 5 )
    {
        case 0:
            temp = MSCombo_TMDS_COLOR_RGB;
            break;
        case 1:
            temp = MSCombo_TMDS_COLOR_YUV_422;
            break;
        case 2:
            temp = MSCombo_TMDS_COLOR_YUV_444;
            break;
    }

    switch( (msReadByte(REG_1603A3 + wOffset)&(BIT6|BIT7)) >>6)
    {
        case 0:
            gScInfo.AVI_Colorimetry=MSCombo_YUV_COLORIMETRY_NoData;
            break;
        case 1:
            gScInfo.AVI_Colorimetry=MSCombo_YUV_COLORIMETRY_ITU601;
            break;
        case 2:
            gScInfo.AVI_Colorimetry=MSCombo_YUV_COLORIMETRY_ITU709;
            break;
    }

    return temp;

}
#endif

#if ENABLE_CABLE_DET
//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_DetectCablePlug()
//  [Description]:
//                  detect if HDMI/DVI cable is attached.
//  [Arguments]:
//                  ucInputPort
//  [Return]:
//                  BYTE
//
//**************************************************************************
BYTE mhal_tmds_DetectCablePlug(BYTE ucInputPort)
{
    BYTE bPortSelect = FALSE;

        switch(ucInputPort)
        {
            case 0:
                #if CABLE_DET_DVI0_SAR
                bPortSelect = ((hwDVI0Cable_Pin > 0x0A)? FALSE : TRUE);
                #else
                bPortSelect = (hwDVI0Cable_Pin ? FALSE : TRUE);
                #endif
                break;
            case 1:
                #if CABLE_DET_DVI1_SAR
                bPortSelect = ((hwDVI1Cable_Pin > 0x0A)? FALSE : TRUE);
                #else                
                bPortSelect = (hwDVI1Cable_Pin ? FALSE : TRUE);
                #endif
                break;
            case 2:
                #if CABLE_DET_DVI2_SAR
                bPortSelect = ((hwDVI2Cable_Pin > 0x0A)? FALSE : TRUE);
                #else
                bPortSelect = (hwDVI2Cable_Pin ? FALSE : TRUE);
                #endif
                break;
            case 3:
                #if CABLE_DET_DVI3_SAR
                bPortSelect = ((hwDVI3Cable_Pin > 0x0A)? FALSE : TRUE);
                #else
                bPortSelect = (hwDVI3Cable_Pin ? FALSE : TRUE);
                #endif
                break;
            case 4:
                #if CABLE_DET_DVI4_SAR
                bPortSelect = ((hwDVI4Cable_Pin > 0x0A)? FALSE : TRUE);
                #else
                bPortSelect = (hwDVI4Cable_Pin ? FALSE : TRUE);
                #endif
                break;
            case 5:
                #if CABLE_DET_DVI5_SAR
                bPortSelect = ((hwDVI5Cable_Pin > 0x0A)? FALSE : TRUE);
                #else
                bPortSelect = (hwDVI5Cable_Pin ? FALSE : TRUE);
                #endif
                break;
        };

        return bPortSelect;

}
#endif

#if ENABLE_SYNC_CHECK_AGAIN
//**************************************************************************
//  [Function Name]:
//                  msEread_IsHDCPKeyInEfuse()
//  [Description]:
//                 Check any valid HDCP key in Efuse
//  [Arguments]:
//               
//  [Return]:
//                  Bool
//
//**************************************************************************

Bool msEread_IsHDCPKeyInEfuse(void)
{
    Bool res = FALSE;

    HDCP_EFUSE_WRITE_ADDRESS(0x124); // 292 BYTE should be 0x45
	ForceDelay1ms(1);
    //msWait();
    while(!HDCP_EFUSE_WAIT_READY());
    res = (HDCP_EFUSE_READ_DATA() == 0x45) ? TRUE : FALSE;

    return res;
}
//**************************************************************************
//  [Function Name]:
//                  msEread_CompareHDCPChecksumInEfuse()
//  [Description]:
//                 Compare checksum is equal to Efuse or not
//  [Arguments]:
//               u16Value
//  [Return]:
//                  Bool
//
//**************************************************************************
Bool msEread_CompareHDCPChecksumInEfuse(WORD u16Value)
{
    Bool res = FALSE;
    BYTE ucValue;
    WORD wEfuseChecksum = 0;

    HDCP_EFUSE_WRITE_ADDRESS(0x12C);
	ForceDelay1ms(1);
    //msWait();
    while(!HDCP_EFUSE_WAIT_READY());
    ucValue = HDCP_EFUSE_READ_DATA();
    wEfuseChecksum |= ucValue << 8;
    HDCP_EFUSE_WRITE_ADDRESS(0x12D);
	ForceDelay1ms(1);
    //msWait();
    while(!HDCP_EFUSE_WAIT_READY());
    ucValue = HDCP_EFUSE_READ_DATA();
    wEfuseChecksum |= ucValue;

    //EFUSE_PRINT("checkSum:\n");
    //EFUSE_PRINT("SW = %x\nEfuse = %x\n",u16Value, wEfuseChecksum);

    res = (wEfuseChecksum == u16Value) ? TRUE : FALSE;

    return res;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_LoadKeyViaEfuse()
//  [Description]:
//                  load key via Efuse
//  [Arguments]:
//                  HDCP_KEY_TABLE, HDCP_BKSV, eFuse
//  [Return]:
//                  Bool
//
//**************************************************************************
Bool mhal_tmds_LoadKeyViaEfuse(BYTE* HDCP_KEY_TABLE, BYTE* HDCP_BKSV, EFUSE_SEL eFuse)
{
    WORD i, checkSum = 0;
    BYTE ucValue;

#if (CHIP_ID==MST9U3)
    _msEfuseWriteByteMask( REG_103A89, eFuse, 0x30);
#else
    _msEfuseWriteByteMask( REG_103A85, eFuse, 0x10);
#endif

    if (msEread_IsHDCPKeyInEfuse())
    {
        EFUSE_printMsg("BKSV = ");
	 
        //update ksv
        for ( i = 0; i < COMBO_HDCP_BKSV_SIZE; i++)
        {
            HDCP_EFUSE_WRITE_ADDRESS(i);
            ForceDelay1ms(1);
            //msWait();
            while(!HDCP_EFUSE_WAIT_READY());

            ucValue =  HDCP_EFUSE_READ_DATA();
            EFUSE_printData("i= %x", i);
            EFUSE_printData("key = %x\n", ucValue);
	 
            (*(HDCP_BKSV + i)) = ucValue;
            checkSum += ucValue;
        }

        EFUSE_printMsg("HDCP key = ");

        for ( i = COMBO_HDCP_BKSV_SIZE; i < (COMBO_HDCP_KEY_SIZE + COMBO_HDCP_BKSV_SIZE); i++)
        {
            HDCP_EFUSE_WRITE_ADDRESS(i);
            ForceDelay1ms(1);
            //msWait();
            while(!HDCP_EFUSE_WAIT_READY());

            ucValue = HDCP_EFUSE_READ_DATA();
            EFUSE_printData("i= %x", i);
            EFUSE_printData("key = %x\n", ucValue);

            *(HDCP_KEY_TABLE + i - 5) = ucValue;
            checkSum += ucValue;
        }

        if (msEread_CompareHDCPChecksumInEfuse(checkSum))
            return TRUE;
        else
            return FALSE;
    }
    else
        return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_ClockDetect()
//  [Description]:
//                   return clock detect status
//  [Arguments]:
//                  u8ComboPort
//  [Return]:
//                  True: have clock detect, False: no clock detect
//
//**************************************************************************
Bool mhal_tmds_ClockDetect(BYTE u8ComboPort)
{
    Bool bClkDetect = FALSE;

	if(u8ComboPort == MUX_COMBO0)
	{
		bClkDetect = msReadByte(REG_0003A2) & _BIT6; //reg_dvi_clk_det_0
	}
	else if(u8ComboPort == MUX_COMBO1)
	{
		bClkDetect = msReadByte(REG_0003A1) & _BIT0; //reg_dvi_clk_det_1
	}
	else if(u8ComboPort == MUX_COMBO2)
	{
		bClkDetect = msReadByte(REG_0003A1) & _BIT1; //reg_dvi_clk_det_2
	}
	else if(u8ComboPort == MUX_COMBO3)
	{
		bClkDetect = msReadByte(REG_0003A1) & _BIT2; //reg_dvi_clk_det_3
	}
	else if(u8ComboPort == MUX_COMBO4)
	{
		bClkDetect = msReadByte(REG_0003A1) & _BIT3; //reg_dvi_clk_det_4
	}
	else if(u8ComboPort == MUX_COMBO5)
	{
		bClkDetect = msReadByte(REG_0003A1) & _BIT4; //reg_dvi_clk_det_5
	}

	return bClkDetect;

}


//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_Clock_Stable()
//  [Description]:
//                  check input clock is stable or not
//  [Arguments]:
//                  enInputPort : combo port
//  [Return]:
//                  TRUE : Stable
//                  FALSE : Unstable
//**************************************************************************
Bool mhal_tmds_Clock_Stable(BYTE u8ComboPort)
{
    WORD wOffset;

    wOffset = _mhal_tmds_phy_offset(u8ComboPort);

    if(msRead2Byte(REG_112064 + wOffset) & 0x8888)
        return TRUE;
    else
        return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_tmds_InputIsHDMI2()
//  [Description]:
//                   return input signal is HDMI 2.0 or not
//  [Arguments]:
//                  u8ComboPort
//  [Return]:
//                  True: 2.0, False: 1.4
//
//**************************************************************************
Bool mhal_tmds_InputIsHDMI2(BYTE u8ComboPort)
{
    Bool bClkDetect = FALSE;

    if((u8ComboPort == MUX_COMBO2) || (u8ComboPort == MUX_COMBO3))
    {
        if(msReadByte(REG_112560) & _BIT3)
            return TRUE;
        else
            return FALSE;
    }
    else
        return FALSE;

}
#endif

#endif // _MHAL_TMDS_C_

