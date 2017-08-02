////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
//****************************************************
//   Quality Map CodeGen Version 4.0
// 3840
// MST9U
// 2013/6/20 13:31
// 2008/7/1 10:31
//****************************************************

#ifndef _MST9U_SUB_EX_C_
#define _MST9U_SUB_EX_C_

#if PQ_SKIPRULE_ENABLE

const U8 MST_SkipRule_IP_Sub_Ex[PQ_IP_NUM_Sub_Ex]=
{
    /* SPF_DBK */ 0, /* DMS */ 0, /* DMS_H */ 0, /* DMS_V */ 0,
    /* DMS_NMR */ 0, /* VIP_HNMR_Y_lowY */ 0, /* VIP_VNMR_Y_lowY */ 0, /* VIP_Peaking_adptive */ 0,
    /* VIP_Peaking_alpha_MIN */ 0, /* VIP_Peaking_Pcoring_ad_C */ 0, /* VIP_Peaking_gain_by_Cedge */ 0, /* VIP_Peaking_V_dering_LPF */ 0,
    /* VIP_Peaking_V_dering_Adpt */ 0, /* VIP_Peaking_V_dering_Table */ 0, /* VIP_Post_SNR */ 0, /* VIP_ICC_Target */ 0,
    /* VIP_IBC_weight */ 0, /* VIP_ACK */ 0,
};

#endif
//****************************************************
// SPF_DBK
//****************************************************
const U8 MST_SPF_DBK_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK0C_6B_L), 0x1F, 0x1F },
 { PQ_MAP_REG(REG_SC_BK0C_6B_H), 0x1F, 0x1F },
 { PQ_MAP_REG(REG_SC_BK0C_65_L), 0x1F, 0x1F },
 { PQ_MAP_REG(REG_SC_BK0C_65_H), 0x1F, 0x1F },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_SPF_DBK_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_SPF_DBK_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK0C_74_L), 0x01, 0x00/*OFF*/,
                              0x01/*LON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS
//****************************************************
const U8 MST_DMS_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK26_14_L), 0x02, 0x02 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_DMS_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_H
//****************************************************
const U8 MST_DMS_H_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK26_14_L), 0x10, 0x00 },
 { PQ_MAP_REG(REG_SC_BK26_1C_L), 0x0B, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_DMS_H_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_H_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_V
//****************************************************
const U8 MST_DMS_V_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK26_14_L), 0x20, 0x00 },
 { PQ_MAP_REG(REG_SC_BK26_1C_L), 0x30, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_DMS_V_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_V_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_NMR
//****************************************************
const U8 MST_DMS_NMR_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_DMS_NMR_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_NMR_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK26_14_H), 0x04, 0x00/*OFF*/,
                              0x04/*0x03*/, },
 { PQ_MAP_REG(REG_SC_BK26_1D_L), 0x3F, 0x00/*OFF*/,
                              0x03/*0x03*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_HNMR_Y_lowY
//****************************************************
const U8 MST_VIP_HNMR_Y_lowY_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_HNMR_Y_lowY_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_HNMR_Y_lowY_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK18_70_L), 0x10, 0x00/*OFF*/,
                              0x10/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK18_73_L), 0xFF, 0x00/*OFF*/,
                              0x10/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK18_74_L), 0x3F, 0x00/*OFF*/,
                              0x04/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK18_74_H), 0x03, 0x00/*OFF*/,
                              0x01/*S2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_VNMR_Y_lowY
//****************************************************
const U8 MST_VIP_VNMR_Y_lowY_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_VNMR_Y_lowY_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_VNMR_Y_lowY_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_7B_L), 0x10, 0x00/*OFF*/,
                              0x10/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK19_7E_L), 0xFF, 0x00/*OFF*/,
                              0x10/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK19_7F_L), 0x3F, 0x00/*OFF*/,
                              0x04/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK19_7F_H), 0x03, 0x00/*OFF*/,
                              0x01/*S2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_adptive
//****************************************************
const U8 MST_VIP_Peaking_adptive_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_Peaking_adptive_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_adptive_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_31_H), 0x9F, 0x03/*$RF_NTSC*/,
                              0x93/*$AV_NTSC*/,
                              0x03/*$AV_PAL*/,
                              0x13/*$480i*/,
                              0x11/*$720p*/,
                              0x93/*$1080i*/,
                              0x03/*$H_480i*/,
                              0x11/*$H_576i*/,
                              0x11/*$H_720p*/,
                              0x93/*$H_1080i*/,
                              0x03/*$DT_MPEG2_480is*/,
                              0x11/*$DT_MPEG2_576i*/,
                              0x03/*$DT_MPEG2_1080i*/,
                              0x03/*$DT_H264_480is*/,
                              0x11/*$DT_H264_576p*/,
                              0x11/*$DT_H264_720p*/,
                              0x03/*$DT_H264_1080i*/,
                              0x11/*$DT_H264_1080p*/,
                              0x11/*$PC_mode*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_alpha_MIN
//****************************************************
const U8 MST_VIP_Peaking_alpha_MIN_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_Peaking_alpha_MIN_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_alpha_MIN_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_6C_H), 0x37, 0x00/*$OFF*/,
                              0x31/*$m1*/,
                              0x33/*$m2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_Pcoring_ad_C
//****************************************************
const U8 MST_VIP_Peaking_Pcoring_ad_C_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK27_2F_L), 0xFF, 0x88 },//Same mark
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_Peaking_Pcoring_ad_C_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_30_H), 0x10, 0x00/*OFF*/,
                              0x10/*T1*/,
                              0x10/*T2*/,
                              0x10/*T3*/,
                              0x10/*T4*/,
                              0x10/*T5*/,
                              0x10/*T6*/, },
 { PQ_MAP_REG(REG_SC_BK19_37_H), 0x3F, 0x20/*OFF*/,
                              0x20/*T1*/,
                              0x20/*T2*/,
                              0x20/*T3*/,
                              0x20/*T4*/,
                              0x00/*T5*/,
                              0x20/*T6*/, },
 { PQ_MAP_REG(REG_SC_BK27_2E_L), 0xFF, 0x88/*$OFF*/,
                              0xA8/*$T1*/,
                              0xA8/*$T2*/,
                              0x88/*$T3*/,
                              0x88/*$T4*/,
                              0x88/*$T5*/,
                              0x88/*$T6*/, },
 { PQ_MAP_REG(REG_SC_BK27_2E_H), 0xFF, 0x88/*$OFF*/,
                              0x66/*$T1*/,
                              0x77/*$T2*/,
                              0x86/*$T3*/,
                              0x88/*$T4*/,
                              0x88/*$T5*/,
                              0x88/*$T6*/, },
 { PQ_MAP_REG(REG_SC_BK27_2F_H), 0xFF, 0x88/*$OFF*/,
                              0x85/*$T1*/,
                              0x86/*$T2*/,
                              0x87/*$T3*/,
                              0x88/*$T4*/,
                              0x88/*$T5*/,
                              0x88/*$T6*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_gain_by_Cedge
//****************************************************
const U8 MST_VIP_Peaking_gain_by_Cedge_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_Peaking_gain_by_Cedge_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_08_H), 0x77, 0x00/*$OFF*/,
                              0x33/*$T2*/, },
 { PQ_MAP_REG(REG_SC_BK19_0A_L), 0xFF, 0x00/*$OFF*/,
                              0x22/*$T2*/, },
 { PQ_MAP_REG(REG_SC_BK19_0A_H), 0xFF, 0x00/*OFF*/,
                              0xFF/*T2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_V_dering_LPF
//****************************************************
const U8 MST_VIP_Peaking_V_dering_LPF_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_Peaking_V_dering_LPF_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_LPF_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_11_H), 0x72, 0x00/*$OFF*/,
                              0x10/*$VDLPF1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_21_H), 0x30, 0x00/*$OFF*/,
                              0x10/*$VDLPF1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_V_dering_Adpt
//****************************************************
const U8 MST_VIP_Peaking_V_dering_Adpt_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK2B_30_L), 0x3F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_32_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_33_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_34_L), 0x3F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_35_L), 0x3F, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_Peaking_V_dering_Adpt_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_Adpt_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_14_L), 0x0F, 0x00/*OFF*/,
                              0x58/*VDAdpt1*/,
                              0x58/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_14_H), 0x0F, 0x00/*OFF*/,
                              0x58/*VDAdpt1*/,
                              0x58/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_20_H), 0x03, 0x00/*$OFF*/,
                              0x03/*$VDAdpt1*/,
                              0x03/*$VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_21_H), 0x07, 0x00/*$OFF*/,
                              0x07/*$VDAdpt1*/,
                              0x07/*$VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_30_H), 0x07, 0x00/*OFF*/,
                              0x05/*VDAdpt1*/,
                              0x01/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_31_L), 0xFF, 0x00/*OFF*/,
                              0x14/*VDAdpt1*/,
                              0x14/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_31_H), 0x0F, 0x00/*OFF*/,
                              0x08/*VDAdpt1*/,
                              0x08/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_32_H), 0x3F, 0x00/*OFF*/,
                              0x20/*VDAdpt1*/,
                              0x20/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_33_H), 0x0F, 0x00/*OFF*/,
                              0x08/*VDAdpt1*/,
                              0x08/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_V_dering_Table
//****************************************************
const U8 MST_VIP_Peaking_V_dering_Table_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_Peaking_V_dering_Table_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_Table_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_1C_L), 0x77, 0x00/*$OFF*/,
                              0x31/*$VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1C_H), 0x07, 0x00/*OFF*/,
                              0x04/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_37_L), 0x77, 0x00/*$OFF*/,
                              0x31/*$VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_37_H), 0x07, 0x00/*OFF*/,
                              0x04/*VDTB1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Post_SNR
//****************************************************
const U8 MST_VIP_Post_SNR_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_Post_SNR_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Post_SNR_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_61_L), 0x02, 0x00/*OFF*/,
                              0x02/*ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_ICC_Target
//****************************************************
const U8 MST_VIP_ICC_Target_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_ICC_Target_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_ICC_Target_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK18_30_H), 0x04, 0x00/*OFF*/,
                              0x04/*ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_IBC_weight
//****************************************************
const U8 MST_VIP_IBC_weight_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_IBC_weight_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_IBC_weight_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_ACK
//****************************************************
const U8 MST_VIP_ACK_COM_Sub_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK18_38_L), 0x0C, 0x00 },//Same mark
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

const U8 MST_VIP_ACK_Sub_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_ACK_NUMS_Sub_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};


code EN_IPTAB_INFO PQ_IPTAB_INFO_Sub_Ex[]=
{
{*MST_SPF_DBK_COM_Sub_Ex, *MST_SPF_DBK_Sub_Ex, PQ_IP_SPF_DBK_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_COM_Sub_Ex, *MST_DMS_Sub_Ex, PQ_IP_DMS_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_H_COM_Sub_Ex, *MST_DMS_H_Sub_Ex, PQ_IP_DMS_H_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_V_COM_Sub_Ex, *MST_DMS_V_Sub_Ex, PQ_IP_DMS_V_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_NMR_COM_Sub_Ex, *MST_DMS_NMR_Sub_Ex, PQ_IP_DMS_NMR_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_HNMR_Y_lowY_COM_Sub_Ex, *MST_VIP_HNMR_Y_lowY_Sub_Ex, PQ_IP_VIP_HNMR_Y_lowY_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_VNMR_Y_lowY_COM_Sub_Ex, *MST_VIP_VNMR_Y_lowY_Sub_Ex, PQ_IP_VIP_VNMR_Y_lowY_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_adptive_COM_Sub_Ex, *MST_VIP_Peaking_adptive_Sub_Ex, PQ_IP_VIP_Peaking_adptive_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_alpha_MIN_COM_Sub_Ex, *MST_VIP_Peaking_alpha_MIN_Sub_Ex, PQ_IP_VIP_Peaking_alpha_MIN_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_Pcoring_ad_C_COM_Sub_Ex, *MST_VIP_Peaking_Pcoring_ad_C_Sub_Ex, PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_gain_by_Cedge_COM_Sub_Ex, *MST_VIP_Peaking_gain_by_Cedge_Sub_Ex, PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_LPF_COM_Sub_Ex, *MST_VIP_Peaking_V_dering_LPF_Sub_Ex, PQ_IP_VIP_Peaking_V_dering_LPF_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_Adpt_COM_Sub_Ex, *MST_VIP_Peaking_V_dering_Adpt_Sub_Ex, PQ_IP_VIP_Peaking_V_dering_Adpt_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_Table_COM_Sub_Ex, *MST_VIP_Peaking_V_dering_Table_Sub_Ex, PQ_IP_VIP_Peaking_V_dering_Table_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Post_SNR_COM_Sub_Ex, *MST_VIP_Post_SNR_Sub_Ex, PQ_IP_VIP_Post_SNR_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ICC_Target_COM_Sub_Ex, *MST_VIP_ICC_Target_Sub_Ex, PQ_IP_VIP_ICC_Target_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_IBC_weight_COM_Sub_Ex, *MST_VIP_IBC_weight_Sub_Ex, PQ_IP_VIP_IBC_weight_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ACK_COM_Sub_Ex, *MST_VIP_ACK_Sub_Ex, PQ_IP_VIP_ACK_NUMS_Sub_Ex, PQ_TABTYPE_SCALER},
};

#endif
