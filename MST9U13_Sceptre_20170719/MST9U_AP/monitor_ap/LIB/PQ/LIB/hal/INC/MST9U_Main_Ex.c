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
// 2013/10/25 22:03
// 2008/7/1 10:31
//****************************************************

#ifndef _MST9U_MAIN_EX_C_
#define _MST9U_MAIN_EX_C_

#if PQ_SKIPRULE_ENABLE

code U8 MST_SkipRule_IP_Main_Ex[PQ_IP_NUM_Main_Ex]=
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
code U8 MST_SPF_DBK_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK0C_48_L), 0xFF, 0xEF },
 { PQ_MAP_REG(REG_SC_BK0C_48_H), 0xFF, 0xCD },
 { PQ_MAP_REG(REG_SC_BK0C_49_L), 0xFF, 0xAB },
 { PQ_MAP_REG(REG_SC_BK0C_49_H), 0xFF, 0x89 },
 { PQ_MAP_REG(REG_SC_BK0C_4A_L), 0xFF, 0x67 },
 { PQ_MAP_REG(REG_SC_BK0C_4A_H), 0xFF, 0x45 },
 { PQ_MAP_REG(REG_SC_BK0C_4B_L), 0xFF, 0x23 },
 { PQ_MAP_REG(REG_SC_BK0C_4B_H), 0xFF, 0x01 },
 { PQ_MAP_REG(REG_SC_BK0C_4C_L), 0xFF, 0xEF },
 { PQ_MAP_REG(REG_SC_BK0C_4C_H), 0xFF, 0xCD },
 { PQ_MAP_REG(REG_SC_BK0C_4D_L), 0xFF, 0xAB },
 { PQ_MAP_REG(REG_SC_BK0C_4D_H), 0xFF, 0x89 },
 { PQ_MAP_REG(REG_SC_BK0C_4E_L), 0xFF, 0x67 },
 { PQ_MAP_REG(REG_SC_BK0C_4E_H), 0xFF, 0x45 },
 { PQ_MAP_REG(REG_SC_BK0C_4F_L), 0xFF, 0x23 },
 { PQ_MAP_REG(REG_SC_BK0C_4F_H), 0xFF, 0x01 },
 { PQ_MAP_REG(REG_SC_BK0C_63_L), 0x1F, 0x1F },
 { PQ_MAP_REG(REG_SC_BK0C_63_H), 0x1F, 0x1F },
 { PQ_MAP_REG(REG_SC_BK0C_64_L), 0x1F, 0x1F },
 { PQ_MAP_REG(REG_SC_BK0C_64_H), 0x1F, 0x1F },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_SPF_DBK_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_SPF_DBK_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK0C_71_L), 0xC1, 0x40/*$OFF*/, 
                              0x41/*$LON*/, 
                              0x81/*$SDtoHD*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS
//****************************************************
code U8 MST_DMS_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_H
//****************************************************
code U8 MST_DMS_H_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK26_19_L), 0x0B, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_H_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_H_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK26_10_L), 0x10, 0x00/*J0*/, 
                              0x10/*J1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_V
//****************************************************
code U8 MST_DMS_V_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK26_19_L), 0x30, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_V_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_V_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK26_10_L), 0x20, 0x00/*J0*/, 
                              0x20/*J1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// DMS_NMR
//****************************************************
code U8 MST_DMS_NMR_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_DMS_NMR_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_DMS_NMR_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK26_10_L), 0x08, 0x00/*OFF*/, 
                              0x08/*ON*/, },
 { PQ_MAP_REG(REG_SC_BK26_1A_L), 0x3F, 0x00/*OFF*/, 
                              0x03/*ON*/, },
 { PQ_MAP_REG(REG_SC_BK26_1A_H), 0xFF, 0xFF/*$OFF*/, 
                              0x22/*$ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_HNMR_Y_lowY
//****************************************************
code U8 MST_VIP_HNMR_Y_lowY_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_HNMR_Y_lowY_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_HNMR_Y_lowY_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK18_70_L), 0x01, 0x00/*OFF*/, 
                              0x01/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK18_71_L), 0xFF, 0x00/*OFF*/, 
                              0x10/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK18_72_L), 0x3F, 0x00/*OFF*/, 
                              0x04/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK18_72_H), 0x03, 0x00/*OFF*/, 
                              0x01/*S2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_VNMR_Y_lowY
//****************************************************
code U8 MST_VIP_VNMR_Y_lowY_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_VNMR_Y_lowY_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_VNMR_Y_lowY_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_7B_L), 0x01, 0x00/*OFF*/, 
                              0x01/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK19_7C_L), 0xFF, 0x00/*OFF*/, 
                              0x10/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK19_7D_L), 0x3F, 0x00/*OFF*/, 
                              0x04/*S2*/, },
 { PQ_MAP_REG(REG_SC_BK19_7D_H), 0x03, 0x00/*OFF*/, 
                              0x01/*S2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_adptive
//****************************************************
code U8 MST_VIP_Peaking_adptive_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK19_32_L), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK19_32_H), 0xFF, 0x44 },//Same mark
 { PQ_MAP_REG(REG_SC_BK19_46_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_46_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_47_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_47_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_4E_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_4E_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_4F_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK19_4F_H), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_adptive_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_adptive_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_31_L), 0x9F, 0x03/*$RF_NTSC*/, 
                              0x93/*$AV_NTSC*/, 
                              0x03/*$AV_PAL*/, 
                              0x03/*$480i*/, 
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
 { PQ_MAP_REG(REG_SC_BK19_40_L), 0x3F, 0x1C/*RF_NTSC*/, 
                              0x1C/*AV_NTSC*/, 
                              0x1C/*AV_PAL*/, 
                              0x1C/*480i*/, 
                              0x1C/*720p*/, 
                              0x1C/*1080i*/, 
                              0x1C/*H_480i*/, 
                              0x1C/*H_576i*/, 
                              0x1C/*H_720p*/, 
                              0x1C/*H_1080i*/, 
                              0x1C/*DT_MPEG2_480is*/, 
                              0x1C/*DT_MPEG2_576i*/, 
                              0x1C/*DT_MPEG2_1080i*/, 
                              0x1C/*DT_H264_480is*/, 
                              0x1C/*DT_H264_576p*/, 
                              0x1C/*DT_H264_720p*/, 
                              0x1C/*DT_H264_1080i*/, 
                              0x1C/*DT_H264_1080p*/, 
                              0x2B/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_40_H), 0x3F, 0x1E/*RF_NTSC*/, 
                              0x1E/*AV_NTSC*/, 
                              0x1E/*AV_PAL*/, 
                              0x1E/*480i*/, 
                              0x1E/*720p*/, 
                              0x1E/*1080i*/, 
                              0x1E/*H_480i*/, 
                              0x1E/*H_576i*/, 
                              0x1E/*H_720p*/, 
                              0x1E/*H_1080i*/, 
                              0x1E/*DT_MPEG2_480is*/, 
                              0x1E/*DT_MPEG2_576i*/, 
                              0x1E/*DT_MPEG2_1080i*/, 
                              0x1E/*DT_H264_480is*/, 
                              0x1E/*DT_H264_576p*/, 
                              0x1E/*DT_H264_720p*/, 
                              0x1E/*DT_H264_1080i*/, 
                              0x1E/*DT_H264_1080p*/, 
                              0x2A/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_41_L), 0x3F, 0x20/*RF_NTSC*/, 
                              0x20/*AV_NTSC*/, 
                              0x20/*AV_PAL*/, 
                              0x20/*480i*/, 
                              0x20/*720p*/, 
                              0x20/*1080i*/, 
                              0x20/*H_480i*/, 
                              0x20/*H_576i*/, 
                              0x20/*H_720p*/, 
                              0x20/*H_1080i*/, 
                              0x20/*DT_MPEG2_480is*/, 
                              0x20/*DT_MPEG2_576i*/, 
                              0x20/*DT_MPEG2_1080i*/, 
                              0x20/*DT_H264_480is*/, 
                              0x20/*DT_H264_576p*/, 
                              0x20/*DT_H264_720p*/, 
                              0x20/*DT_H264_1080i*/, 
                              0x20/*DT_H264_1080p*/, 
                              0x28/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_41_H), 0x3F, 0x22/*RF_NTSC*/, 
                              0x22/*AV_NTSC*/, 
                              0x22/*AV_PAL*/, 
                              0x22/*480i*/, 
                              0x22/*720p*/, 
                              0x22/*1080i*/, 
                              0x22/*H_480i*/, 
                              0x22/*H_576i*/, 
                              0x22/*H_720p*/, 
                              0x22/*H_1080i*/, 
                              0x22/*DT_MPEG2_480is*/, 
                              0x22/*DT_MPEG2_576i*/, 
                              0x22/*DT_MPEG2_1080i*/, 
                              0x22/*DT_H264_480is*/, 
                              0x22/*DT_H264_576p*/, 
                              0x22/*DT_H264_720p*/, 
                              0x22/*DT_H264_1080i*/, 
                              0x22/*DT_H264_1080p*/, 
                              0x27/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_42_L), 0x3F, 0x24/*RF_NTSC*/, 
                              0x24/*AV_NTSC*/, 
                              0x24/*AV_PAL*/, 
                              0x24/*480i*/, 
                              0x24/*720p*/, 
                              0x24/*1080i*/, 
                              0x24/*H_480i*/, 
                              0x24/*H_576i*/, 
                              0x24/*H_720p*/, 
                              0x24/*H_1080i*/, 
                              0x24/*DT_MPEG2_480is*/, 
                              0x24/*DT_MPEG2_576i*/, 
                              0x24/*DT_MPEG2_1080i*/, 
                              0x24/*DT_H264_480is*/, 
                              0x24/*DT_H264_576p*/, 
                              0x24/*DT_H264_720p*/, 
                              0x24/*DT_H264_1080i*/, 
                              0x24/*DT_H264_1080p*/, 
                              0x26/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_42_H), 0x3F, 0x26/*RF_NTSC*/, 
                              0x26/*AV_NTSC*/, 
                              0x26/*AV_PAL*/, 
                              0x26/*480i*/, 
                              0x26/*720p*/, 
                              0x26/*1080i*/, 
                              0x26/*H_480i*/, 
                              0x26/*H_576i*/, 
                              0x26/*H_720p*/, 
                              0x26/*H_1080i*/, 
                              0x26/*DT_MPEG2_480is*/, 
                              0x26/*DT_MPEG2_576i*/, 
                              0x26/*DT_MPEG2_1080i*/, 
                              0x26/*DT_H264_480is*/, 
                              0x26/*DT_H264_576p*/, 
                              0x26/*DT_H264_720p*/, 
                              0x26/*DT_H264_1080i*/, 
                              0x26/*DT_H264_1080p*/, 
                              0x25/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_43_L), 0x3F, 0x28/*RF_NTSC*/, 
                              0x28/*AV_NTSC*/, 
                              0x28/*AV_PAL*/, 
                              0x28/*480i*/, 
                              0x28/*720p*/, 
                              0x28/*1080i*/, 
                              0x28/*H_480i*/, 
                              0x28/*H_576i*/, 
                              0x28/*H_720p*/, 
                              0x28/*H_1080i*/, 
                              0x28/*DT_MPEG2_480is*/, 
                              0x28/*DT_MPEG2_576i*/, 
                              0x28/*DT_MPEG2_1080i*/, 
                              0x28/*DT_H264_480is*/, 
                              0x28/*DT_H264_576p*/, 
                              0x28/*DT_H264_720p*/, 
                              0x28/*DT_H264_1080i*/, 
                              0x28/*DT_H264_1080p*/, 
                              0x24/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_43_H), 0x3F, 0x2A/*RF_NTSC*/, 
                              0x2A/*AV_NTSC*/, 
                              0x2A/*AV_PAL*/, 
                              0x2A/*480i*/, 
                              0x2A/*720p*/, 
                              0x2A/*1080i*/, 
                              0x2A/*H_480i*/, 
                              0x2A/*H_576i*/, 
                              0x2A/*H_720p*/, 
                              0x2A/*H_1080i*/, 
                              0x2A/*DT_MPEG2_480is*/, 
                              0x2A/*DT_MPEG2_576i*/, 
                              0x2A/*DT_MPEG2_1080i*/, 
                              0x2A/*DT_H264_480is*/, 
                              0x2A/*DT_H264_576p*/, 
                              0x2A/*DT_H264_720p*/, 
                              0x2A/*DT_H264_1080i*/, 
                              0x2A/*DT_H264_1080p*/, 
                              0x23/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_44_L), 0x3F, 0x28/*RF_NTSC*/, 
                              0x28/*AV_NTSC*/, 
                              0x28/*AV_PAL*/, 
                              0x28/*480i*/, 
                              0x28/*720p*/, 
                              0x28/*1080i*/, 
                              0x28/*H_480i*/, 
                              0x28/*H_576i*/, 
                              0x28/*H_720p*/, 
                              0x28/*H_1080i*/, 
                              0x28/*DT_MPEG2_480is*/, 
                              0x28/*DT_MPEG2_576i*/, 
                              0x28/*DT_MPEG2_1080i*/, 
                              0x28/*DT_H264_480is*/, 
                              0x28/*DT_H264_576p*/, 
                              0x28/*DT_H264_720p*/, 
                              0x28/*DT_H264_1080i*/, 
                              0x28/*DT_H264_1080p*/, 
                              0x22/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_44_H), 0x3F, 0x26/*RF_NTSC*/, 
                              0x26/*AV_NTSC*/, 
                              0x26/*AV_PAL*/, 
                              0x26/*480i*/, 
                              0x26/*720p*/, 
                              0x26/*1080i*/, 
                              0x26/*H_480i*/, 
                              0x26/*H_576i*/, 
                              0x26/*H_720p*/, 
                              0x26/*H_1080i*/, 
                              0x26/*DT_MPEG2_480is*/, 
                              0x26/*DT_MPEG2_576i*/, 
                              0x26/*DT_MPEG2_1080i*/, 
                              0x26/*DT_H264_480is*/, 
                              0x26/*DT_H264_576p*/, 
                              0x26/*DT_H264_720p*/, 
                              0x26/*DT_H264_1080i*/, 
                              0x26/*DT_H264_1080p*/, 
                              0x21/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_45_L), 0x3F, 0x24/*RF_NTSC*/, 
                              0x24/*AV_NTSC*/, 
                              0x24/*AV_PAL*/, 
                              0x24/*480i*/, 
                              0x24/*720p*/, 
                              0x24/*1080i*/, 
                              0x24/*H_480i*/, 
                              0x24/*H_576i*/, 
                              0x24/*H_720p*/, 
                              0x24/*H_1080i*/, 
                              0x24/*DT_MPEG2_480is*/, 
                              0x24/*DT_MPEG2_576i*/, 
                              0x24/*DT_MPEG2_1080i*/, 
                              0x24/*DT_H264_480is*/, 
                              0x24/*DT_H264_576p*/, 
                              0x24/*DT_H264_720p*/, 
                              0x24/*DT_H264_1080i*/, 
                              0x24/*DT_H264_1080p*/, 
                              0x20/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_45_H), 0x3F, 0x22/*RF_NTSC*/, 
                              0x22/*AV_NTSC*/, 
                              0x22/*AV_PAL*/, 
                              0x22/*480i*/, 
                              0x22/*720p*/, 
                              0x22/*1080i*/, 
                              0x22/*H_480i*/, 
                              0x22/*H_576i*/, 
                              0x22/*H_720p*/, 
                              0x22/*H_1080i*/, 
                              0x22/*DT_MPEG2_480is*/, 
                              0x22/*DT_MPEG2_576i*/, 
                              0x22/*DT_MPEG2_1080i*/, 
                              0x22/*DT_H264_480is*/, 
                              0x22/*DT_H264_576p*/, 
                              0x22/*DT_H264_720p*/, 
                              0x22/*DT_H264_1080i*/, 
                              0x22/*DT_H264_1080p*/, 
                              0x20/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_48_L), 0x3F, 0x1C/*RF_NTSC*/, 
                              0x1C/*AV_NTSC*/, 
                              0x1C/*AV_PAL*/, 
                              0x1C/*480i*/, 
                              0x1C/*720p*/, 
                              0x1C/*1080i*/, 
                              0x1C/*H_480i*/, 
                              0x1C/*H_576i*/, 
                              0x1C/*H_720p*/, 
                              0x1C/*H_1080i*/, 
                              0x1C/*DT_MPEG2_480is*/, 
                              0x1C/*DT_MPEG2_576i*/, 
                              0x1C/*DT_MPEG2_1080i*/, 
                              0x1C/*DT_H264_480is*/, 
                              0x1C/*DT_H264_576p*/, 
                              0x1C/*DT_H264_720p*/, 
                              0x1C/*DT_H264_1080i*/, 
                              0x1C/*DT_H264_1080p*/, 
                              0x2B/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_48_H), 0x3F, 0x1E/*RF_NTSC*/, 
                              0x1E/*AV_NTSC*/, 
                              0x1E/*AV_PAL*/, 
                              0x1E/*480i*/, 
                              0x1E/*720p*/, 
                              0x1E/*1080i*/, 
                              0x1E/*H_480i*/, 
                              0x1E/*H_576i*/, 
                              0x1E/*H_720p*/, 
                              0x1E/*H_1080i*/, 
                              0x1E/*DT_MPEG2_480is*/, 
                              0x1E/*DT_MPEG2_576i*/, 
                              0x1E/*DT_MPEG2_1080i*/, 
                              0x1E/*DT_H264_480is*/, 
                              0x1E/*DT_H264_576p*/, 
                              0x1E/*DT_H264_720p*/, 
                              0x1E/*DT_H264_1080i*/, 
                              0x1E/*DT_H264_1080p*/, 
                              0x2A/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_49_L), 0x3F, 0x20/*RF_NTSC*/, 
                              0x20/*AV_NTSC*/, 
                              0x20/*AV_PAL*/, 
                              0x20/*480i*/, 
                              0x20/*720p*/, 
                              0x20/*1080i*/, 
                              0x20/*H_480i*/, 
                              0x20/*H_576i*/, 
                              0x20/*H_720p*/, 
                              0x20/*H_1080i*/, 
                              0x20/*DT_MPEG2_480is*/, 
                              0x20/*DT_MPEG2_576i*/, 
                              0x20/*DT_MPEG2_1080i*/, 
                              0x20/*DT_H264_480is*/, 
                              0x20/*DT_H264_576p*/, 
                              0x20/*DT_H264_720p*/, 
                              0x20/*DT_H264_1080i*/, 
                              0x20/*DT_H264_1080p*/, 
                              0x28/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_49_H), 0x3F, 0x22/*RF_NTSC*/, 
                              0x22/*AV_NTSC*/, 
                              0x22/*AV_PAL*/, 
                              0x22/*480i*/, 
                              0x22/*720p*/, 
                              0x22/*1080i*/, 
                              0x22/*H_480i*/, 
                              0x22/*H_576i*/, 
                              0x22/*H_720p*/, 
                              0x22/*H_1080i*/, 
                              0x22/*DT_MPEG2_480is*/, 
                              0x22/*DT_MPEG2_576i*/, 
                              0x22/*DT_MPEG2_1080i*/, 
                              0x22/*DT_H264_480is*/, 
                              0x22/*DT_H264_576p*/, 
                              0x22/*DT_H264_720p*/, 
                              0x22/*DT_H264_1080i*/, 
                              0x22/*DT_H264_1080p*/, 
                              0x27/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4A_L), 0x3F, 0x24/*RF_NTSC*/, 
                              0x24/*AV_NTSC*/, 
                              0x24/*AV_PAL*/, 
                              0x24/*480i*/, 
                              0x24/*720p*/, 
                              0x24/*1080i*/, 
                              0x24/*H_480i*/, 
                              0x24/*H_576i*/, 
                              0x24/*H_720p*/, 
                              0x24/*H_1080i*/, 
                              0x24/*DT_MPEG2_480is*/, 
                              0x24/*DT_MPEG2_576i*/, 
                              0x24/*DT_MPEG2_1080i*/, 
                              0x24/*DT_H264_480is*/, 
                              0x24/*DT_H264_576p*/, 
                              0x24/*DT_H264_720p*/, 
                              0x24/*DT_H264_1080i*/, 
                              0x24/*DT_H264_1080p*/, 
                              0x26/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4A_H), 0x3F, 0x26/*RF_NTSC*/, 
                              0x26/*AV_NTSC*/, 
                              0x26/*AV_PAL*/, 
                              0x26/*480i*/, 
                              0x26/*720p*/, 
                              0x26/*1080i*/, 
                              0x26/*H_480i*/, 
                              0x26/*H_576i*/, 
                              0x26/*H_720p*/, 
                              0x26/*H_1080i*/, 
                              0x26/*DT_MPEG2_480is*/, 
                              0x26/*DT_MPEG2_576i*/, 
                              0x26/*DT_MPEG2_1080i*/, 
                              0x26/*DT_H264_480is*/, 
                              0x26/*DT_H264_576p*/, 
                              0x26/*DT_H264_720p*/, 
                              0x26/*DT_H264_1080i*/, 
                              0x26/*DT_H264_1080p*/, 
                              0x25/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4B_L), 0x3F, 0x28/*RF_NTSC*/, 
                              0x28/*AV_NTSC*/, 
                              0x28/*AV_PAL*/, 
                              0x28/*480i*/, 
                              0x28/*720p*/, 
                              0x28/*1080i*/, 
                              0x28/*H_480i*/, 
                              0x28/*H_576i*/, 
                              0x28/*H_720p*/, 
                              0x28/*H_1080i*/, 
                              0x28/*DT_MPEG2_480is*/, 
                              0x28/*DT_MPEG2_576i*/, 
                              0x28/*DT_MPEG2_1080i*/, 
                              0x28/*DT_H264_480is*/, 
                              0x28/*DT_H264_576p*/, 
                              0x28/*DT_H264_720p*/, 
                              0x28/*DT_H264_1080i*/, 
                              0x28/*DT_H264_1080p*/, 
                              0x24/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4B_H), 0x3F, 0x2A/*RF_NTSC*/, 
                              0x2A/*AV_NTSC*/, 
                              0x2A/*AV_PAL*/, 
                              0x2A/*480i*/, 
                              0x2A/*720p*/, 
                              0x2A/*1080i*/, 
                              0x2A/*H_480i*/, 
                              0x2A/*H_576i*/, 
                              0x2A/*H_720p*/, 
                              0x2A/*H_1080i*/, 
                              0x2A/*DT_MPEG2_480is*/, 
                              0x2A/*DT_MPEG2_576i*/, 
                              0x2A/*DT_MPEG2_1080i*/, 
                              0x2A/*DT_H264_480is*/, 
                              0x2A/*DT_H264_576p*/, 
                              0x2A/*DT_H264_720p*/, 
                              0x2A/*DT_H264_1080i*/, 
                              0x2A/*DT_H264_1080p*/, 
                              0x23/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4C_L), 0x3F, 0x28/*RF_NTSC*/, 
                              0x28/*AV_NTSC*/, 
                              0x28/*AV_PAL*/, 
                              0x28/*480i*/, 
                              0x28/*720p*/, 
                              0x28/*1080i*/, 
                              0x28/*H_480i*/, 
                              0x28/*H_576i*/, 
                              0x28/*H_720p*/, 
                              0x28/*H_1080i*/, 
                              0x28/*DT_MPEG2_480is*/, 
                              0x28/*DT_MPEG2_576i*/, 
                              0x28/*DT_MPEG2_1080i*/, 
                              0x28/*DT_H264_480is*/, 
                              0x28/*DT_H264_576p*/, 
                              0x28/*DT_H264_720p*/, 
                              0x28/*DT_H264_1080i*/, 
                              0x28/*DT_H264_1080p*/, 
                              0x22/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4C_H), 0x3F, 0x26/*RF_NTSC*/, 
                              0x26/*AV_NTSC*/, 
                              0x26/*AV_PAL*/, 
                              0x26/*480i*/, 
                              0x26/*720p*/, 
                              0x26/*1080i*/, 
                              0x26/*H_480i*/, 
                              0x26/*H_576i*/, 
                              0x26/*H_720p*/, 
                              0x26/*H_1080i*/, 
                              0x26/*DT_MPEG2_480is*/, 
                              0x26/*DT_MPEG2_576i*/, 
                              0x26/*DT_MPEG2_1080i*/, 
                              0x26/*DT_H264_480is*/, 
                              0x26/*DT_H264_576p*/, 
                              0x26/*DT_H264_720p*/, 
                              0x26/*DT_H264_1080i*/, 
                              0x26/*DT_H264_1080p*/, 
                              0x21/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4D_L), 0x3F, 0x24/*RF_NTSC*/, 
                              0x24/*AV_NTSC*/, 
                              0x24/*AV_PAL*/, 
                              0x24/*480i*/, 
                              0x24/*720p*/, 
                              0x24/*1080i*/, 
                              0x24/*H_480i*/, 
                              0x24/*H_576i*/, 
                              0x24/*H_720p*/, 
                              0x24/*H_1080i*/, 
                              0x24/*DT_MPEG2_480is*/, 
                              0x24/*DT_MPEG2_576i*/, 
                              0x24/*DT_MPEG2_1080i*/, 
                              0x24/*DT_H264_480is*/, 
                              0x24/*DT_H264_576p*/, 
                              0x24/*DT_H264_720p*/, 
                              0x24/*DT_H264_1080i*/, 
                              0x24/*DT_H264_1080p*/, 
                              0x20/*PC_mode*/, },
 { PQ_MAP_REG(REG_SC_BK19_4D_H), 0x3F, 0x22/*RF_NTSC*/, 
                              0x22/*AV_NTSC*/, 
                              0x22/*AV_PAL*/, 
                              0x22/*480i*/, 
                              0x22/*720p*/, 
                              0x22/*1080i*/, 
                              0x22/*H_480i*/, 
                              0x22/*H_576i*/, 
                              0x22/*H_720p*/, 
                              0x22/*H_1080i*/, 
                              0x22/*DT_MPEG2_480is*/, 
                              0x22/*DT_MPEG2_576i*/, 
                              0x22/*DT_MPEG2_1080i*/, 
                              0x22/*DT_H264_480is*/, 
                              0x22/*DT_H264_576p*/, 
                              0x22/*DT_H264_720p*/, 
                              0x22/*DT_H264_1080i*/, 
                              0x22/*DT_H264_1080p*/, 
                              0x20/*PC_mode*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_alpha_MIN
//****************************************************
code U8 MST_VIP_Peaking_alpha_MIN_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_alpha_MIN_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_alpha_MIN_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_6C_L), 0x37, 0x00/*$OFF*/, 
                              0x31/*$m1*/, 
                              0x33/*$m2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_Pcoring_ad_C
//****************************************************
code U8 MST_VIP_Peaking_Pcoring_ad_C_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK27_2D_L), 0xFF, 0x88 },//Same mark
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_Pcoring_ad_C_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_30_H), 0x80, 0x00/*OFF*/, 
                              0x00/*T1*/, 
                              0x80/*T2*/, 
                              0x80/*T3*/, 
                              0x80/*T4*/, 
                              0x80/*T5*/, 
                              0x80/*T6*/, },
 { PQ_MAP_REG(REG_SC_BK19_37_L), 0x3F, 0x20/*OFF*/, 
                              0x20/*T1*/, 
                              0x20/*T2*/, 
                              0x20/*T3*/, 
                              0x20/*T4*/, 
                              0x00/*T5*/, 
                              0x20/*T6*/, },
 { PQ_MAP_REG(REG_SC_BK27_2C_L), 0xFF, 0x88/*$OFF*/, 
                              0xA8/*$T1*/, 
                              0xA8/*$T2*/, 
                              0x88/*$T3*/, 
                              0x88/*$T4*/, 
                              0x88/*$T5*/, 
                              0x88/*$T6*/, },
 { PQ_MAP_REG(REG_SC_BK27_2C_H), 0xFF, 0x88/*$OFF*/, 
                              0x66/*$T1*/, 
                              0x77/*$T2*/, 
                              0x86/*$T3*/, 
                              0x88/*$T4*/, 
                              0x88/*$T5*/, 
                              0x88/*$T6*/, },
 { PQ_MAP_REG(REG_SC_BK27_2D_H), 0xFF, 0x88/*$OFF*/, 
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
code U8 MST_VIP_Peaking_gain_by_Cedge_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_gain_by_Cedge_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_08_L), 0x77, 0x00/*$OFF*/, 
                              0x33/*$T2*/, },
 { PQ_MAP_REG(REG_SC_BK19_09_L), 0xFF, 0x00/*$OFF*/, 
                              0x22/*$T2*/, },
 { PQ_MAP_REG(REG_SC_BK19_09_H), 0xFF, 0x00/*OFF*/, 
                              0xFF/*T2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_V_dering_LPF
//****************************************************
code U8 MST_VIP_Peaking_V_dering_LPF_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_V_dering_LPF_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_LPF_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_11_L), 0x72, 0x00/*$OFF*/, 
                              0x10/*$VDLPF1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_21_L), 0x30, 0x00/*$OFF*/, 
                              0x10/*$VDLPF1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_V_dering_Adpt
//****************************************************
code U8 MST_VIP_Peaking_V_dering_Adpt_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK2B_22_L), 0x3F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_24_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_25_L), 0xFF, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_26_L), 0x3F, 0x00 },
 { PQ_MAP_REG(REG_SC_BK2B_27_L), 0x3F, 0x00 },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_V_dering_Adpt_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_Adpt_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_12_L), 0x0F, 0x00/*OFF*/, 
                              0x58/*VDAdpt1*/, 
                              0x58/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_12_H), 0x0F, 0x00/*OFF*/, 
                              0x58/*VDAdpt1*/, 
                              0x58/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_20_L), 0x03, 0x00/*$OFF*/, 
                              0x03/*$VDAdpt1*/, 
                              0x03/*$VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_21_L), 0x07, 0x00/*$OFF*/, 
                              0x07/*$VDAdpt1*/, 
                              0x07/*$VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_22_H), 0x07, 0x00/*OFF*/, 
                              0x05/*VDAdpt1*/, 
                              0x01/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_23_L), 0xFF, 0x00/*OFF*/, 
                              0x14/*VDAdpt1*/, 
                              0x14/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_23_H), 0x0F, 0x00/*OFF*/, 
                              0x08/*VDAdpt1*/, 
                              0x08/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_24_H), 0x3F, 0x00/*OFF*/, 
                              0x20/*VDAdpt1*/, 
                              0x20/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_SC_BK2B_25_H), 0x0F, 0x00/*OFF*/, 
                              0x08/*VDAdpt1*/, 
                              0x08/*VDAdpt2*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Peaking_V_dering_Table
//****************************************************
code U8 MST_VIP_Peaking_V_dering_Table_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Peaking_V_dering_Table_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Peaking_V_dering_Table_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK2B_1B_L), 0x77, 0x00/*$OFF*/, 
                              0x31/*$VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1B_H), 0x07, 0x00/*OFF*/, 
                              0x04/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1D_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1D_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1E_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1E_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1F_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_1F_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_29_L), 0x77, 0x00/*$OFF*/, 
                              0x31/*$VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_29_H), 0x07, 0x00/*OFF*/, 
                              0x04/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2D_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2D_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2E_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2E_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2F_L), 0x3F, 0x00/*OFF*/, 
                              0x1A/*VDTB1*/, },
 { PQ_MAP_REG(REG_SC_BK2B_2F_H), 0x3F, 0x00/*OFF*/, 
                              0x13/*VDTB1*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_Post_SNR
//****************************************************
code U8 MST_VIP_Post_SNR_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_Post_SNR_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_Post_SNR_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK19_60_L), 0x02, 0x00/*OFF*/, 
                              0x02/*ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_ICC_Target
//****************************************************
code U8 MST_VIP_ICC_Target_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_ICC_Target_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_ICC_Target_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK18_30_H), 0x02, 0x00/*OFF*/, 
                              0x02/*ON*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_IBC_weight
//****************************************************
code U8 MST_VIP_IBC_weight_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_IBC_weight_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_IBC_weight_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_SC_BK18_48_H), 0xFF, 0x66/*$W6*/, 
                              0x88/*$W8*/, },
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

//****************************************************
// VIP_ACK
//****************************************************
code U8 MST_VIP_ACK_COM_Main_Ex[][4] =
{      // Reg           Mask  Value
 { PQ_MAP_REG(REG_SC_BK18_38_L), 0xD0, 0x00 },//Same mark
 { PQ_MAP_REG(REG_SC_BK18_39_L), 0x3F, 0x20 },
 { PQ_MAP_REG(REG_SC_BK18_39_H), 0x3F, 0x0E },
 { PQ_MAP_REG(REG_SC_BK18_3A_L), 0xFF, 0xC0 },//Same mark
 { PQ_MAP_REG(REG_SC_BK18_3A_H), 0x1F, 0x01 },
 { PQ_MAP_REG(REG_SC_BK18_3B_L), 0xFF, 0x3C },//Same mark
 { PQ_MAP_REG(REG_SC_BK18_3B_H), 0x6F, 0x02 },//Same mark
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 MST_VIP_ACK_Main_Ex[][REG_ADDR_SIZE+REG_MASK_SIZE+PQ_IP_VIP_ACK_NUMS_Main_Ex]=
{
 { PQ_MAP_REG(REG_TABLE_END), 0x00, 0x00 }
};


code EN_IPTAB_INFO PQ_IPTAB_INFO_Main_Ex[]=
{
{*MST_SPF_DBK_COM_Main_Ex, *MST_SPF_DBK_Main_Ex, PQ_IP_SPF_DBK_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_COM_Main_Ex, *MST_DMS_Main_Ex, PQ_IP_DMS_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_H_COM_Main_Ex, *MST_DMS_H_Main_Ex, PQ_IP_DMS_H_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_V_COM_Main_Ex, *MST_DMS_V_Main_Ex, PQ_IP_DMS_V_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_DMS_NMR_COM_Main_Ex, *MST_DMS_NMR_Main_Ex, PQ_IP_DMS_NMR_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_HNMR_Y_lowY_COM_Main_Ex, *MST_VIP_HNMR_Y_lowY_Main_Ex, PQ_IP_VIP_HNMR_Y_lowY_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_VNMR_Y_lowY_COM_Main_Ex, *MST_VIP_VNMR_Y_lowY_Main_Ex, PQ_IP_VIP_VNMR_Y_lowY_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_adptive_COM_Main_Ex, *MST_VIP_Peaking_adptive_Main_Ex, PQ_IP_VIP_Peaking_adptive_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_alpha_MIN_COM_Main_Ex, *MST_VIP_Peaking_alpha_MIN_Main_Ex, PQ_IP_VIP_Peaking_alpha_MIN_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_Pcoring_ad_C_COM_Main_Ex, *MST_VIP_Peaking_Pcoring_ad_C_Main_Ex, PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_gain_by_Cedge_COM_Main_Ex, *MST_VIP_Peaking_gain_by_Cedge_Main_Ex, PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_LPF_COM_Main_Ex, *MST_VIP_Peaking_V_dering_LPF_Main_Ex, PQ_IP_VIP_Peaking_V_dering_LPF_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_Adpt_COM_Main_Ex, *MST_VIP_Peaking_V_dering_Adpt_Main_Ex, PQ_IP_VIP_Peaking_V_dering_Adpt_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Peaking_V_dering_Table_COM_Main_Ex, *MST_VIP_Peaking_V_dering_Table_Main_Ex, PQ_IP_VIP_Peaking_V_dering_Table_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_Post_SNR_COM_Main_Ex, *MST_VIP_Post_SNR_Main_Ex, PQ_IP_VIP_Post_SNR_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ICC_Target_COM_Main_Ex, *MST_VIP_ICC_Target_Main_Ex, PQ_IP_VIP_ICC_Target_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_IBC_weight_COM_Main_Ex, *MST_VIP_IBC_weight_Main_Ex, PQ_IP_VIP_IBC_weight_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
{*MST_VIP_ACK_COM_Main_Ex, *MST_VIP_ACK_Main_Ex, PQ_IP_VIP_ACK_NUMS_Main_Ex, PQ_TABTYPE_SCALER},
};

#endif
