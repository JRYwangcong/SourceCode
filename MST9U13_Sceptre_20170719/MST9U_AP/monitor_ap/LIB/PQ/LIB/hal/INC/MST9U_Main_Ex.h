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

#ifndef _MST9U_MAIN_EX_H_
#define _MST9U_MAIN_EX_H_

#define PQ_IP_NUM_Main_Ex 18

typedef enum
{
    #if PQ_QM_YPBPR
    QM_YPbPr_480i_Main_Ex, //0
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576i_Main_Ex, //1
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_480p_Main_Ex, //2
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_576p_Main_Ex, //3
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_24hz_Main_Ex, //4
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_50hz_Main_Ex, //5
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_720p_60hz_Main_Ex, //6
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_50hz_Main_Ex, //7
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080i_60hz_Main_Ex, //8
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_24hz_Main_Ex, //9
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_50hz_Main_Ex, //10
    #endif
    #if PQ_QM_YPBPR
    QM_YPbPr_1080p_60hz_Main_Ex, //11
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_480i_Main_Ex, //12
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_576i_Main_Ex, //13
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_480p_Main_Ex, //14
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_576p_Main_Ex, //15
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_720p_24hz_Main_Ex, //16
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_720p_50hz_Main_Ex, //17
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_720p_60hz_Main_Ex, //18
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_1080i_50hz_Main_Ex, //19
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_1080i_60hz_Main_Ex, //20
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_1080p_24hz_Main_Ex, //21
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_1080p_60hz_Main_Ex, //22
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_1080p_50hz_Main_Ex, //23
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_RGB_4K2K_Main_Ex, //24
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480i_Main_Ex, //25
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576i_Main_Ex, //26
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_480p_Main_Ex, //27
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_576p_Main_Ex, //28
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_24hz_Main_Ex, //29
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_50hz_Main_Ex, //30
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_720p_60hz_Main_Ex, //31
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_50hz_Main_Ex, //32
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080i_60hz_Main_Ex, //33
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_24hz_Main_Ex, //34
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_60hz_Main_Ex, //35
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_1080p_50hz_Main_Ex, //36
    #endif
    #if PQ_QM_HMDI
    QM_HDMI_4K2K_Main_Ex, //37
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vup_Main_Ex, //38
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vdown_Main_Ex, //39
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hup_Vno_Main_Ex, //40
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vup_Main_Ex, //41
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vdown_Main_Ex, //42
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hdown_Vno_Main_Ex, //43
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vup_Main_Ex, //44
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vdown_Main_Ex, //45
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_444_PC_Hno_Vno_Main_Ex, //46
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vup_Main_Ex, //47
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vdown_Main_Ex, //48
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hup_Vno_Main_Ex, //49
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vup_Main_Ex, //50
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vdown_Main_Ex, //51
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hdown_Vno_Main_Ex, //52
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vup_Main_Ex, //53
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vdown_Main_Ex, //54
    #endif
    #if PQ_QM_HDMI_PC
    QM_HDMI_422_PC_Hno_Vno_Main_Ex, //55
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Main_Ex, //56
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Main_Ex, //57
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Main_Ex, //58
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Main_Ex, //59
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Main_Ex, //60
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Main_Ex, //61
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Main_Ex, //62
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Main_Ex, //63
    #endif
    #if PQ_QM_PC
    QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Main_Ex, //64
    #endif
    QM_INPUTTYPE_NUM_Main_Ex, // 65
} SC_QUALITY_MAP_INDEX_e_Main_Ex;

typedef enum
{
PQ_IP_SPF_DBK_Main_Ex,  //0
PQ_IP_DMS_Main_Ex,  //1
PQ_IP_DMS_H_Main_Ex,  //2
PQ_IP_DMS_V_Main_Ex,  //3
PQ_IP_DMS_NMR_Main_Ex,  //4
PQ_IP_VIP_HNMR_Y_lowY_Main_Ex,  //5
PQ_IP_VIP_VNMR_Y_lowY_Main_Ex,  //6
PQ_IP_VIP_Peaking_adptive_Main_Ex,  //7
PQ_IP_VIP_Peaking_alpha_MIN_Main_Ex,  //8
PQ_IP_VIP_Peaking_Pcoring_ad_C_Main_Ex,  //9
PQ_IP_VIP_Peaking_gain_by_Cedge_Main_Ex,  //10
PQ_IP_VIP_Peaking_V_dering_LPF_Main_Ex,  //11
PQ_IP_VIP_Peaking_V_dering_Adpt_Main_Ex,  //12
PQ_IP_VIP_Peaking_V_dering_Table_Main_Ex,  //13
PQ_IP_VIP_Post_SNR_Main_Ex,  //14
PQ_IP_VIP_ICC_Target_Main_Ex,  //15
PQ_IP_VIP_IBC_weight_Main_Ex,  //16
PQ_IP_VIP_ACK_Main_Ex,  //17
 }   PQ_IPTYPE_Main_Ex;

typedef enum
{
PQ_IP_SPF_DBK_COM_Main_Ex,  //0
PQ_IP_DMS_COM_Main_Ex,  //1
PQ_IP_DMS_H_COM_Main_Ex,  //2
PQ_IP_DMS_V_COM_Main_Ex,  //3
PQ_IP_DMS_NMR_COM_Main_Ex,  //4
PQ_IP_VIP_HNMR_Y_lowY_COM_Main_Ex,  //5
PQ_IP_VIP_VNMR_Y_lowY_COM_Main_Ex,  //6
PQ_IP_VIP_Peaking_adptive_COM_Main_Ex,  //7
PQ_IP_VIP_Peaking_alpha_MIN_COM_Main_Ex,  //8
PQ_IP_VIP_Peaking_Pcoring_ad_C_COM_Main_Ex,  //9
PQ_IP_VIP_Peaking_gain_by_Cedge_COM_Main_Ex,  //10
PQ_IP_VIP_Peaking_V_dering_LPF_COM_Main_Ex,  //11
PQ_IP_VIP_Peaking_V_dering_Adpt_COM_Main_Ex,  //12
PQ_IP_VIP_Peaking_V_dering_Table_COM_Main_Ex,  //13
PQ_IP_VIP_Post_SNR_COM_Main_Ex,  //14
PQ_IP_VIP_ICC_Target_COM_Main_Ex,  //15
PQ_IP_VIP_IBC_weight_COM_Main_Ex,  //16
PQ_IP_VIP_ACK_COM_Main_Ex,  //17
PQ_IP_COM_NUMS_Main_Ex
} PQ_IP_COM_Group_Main_Ex;

typedef enum
{
PQ_IP_SPF_DBK_OFF_Main_Ex,
PQ_IP_SPF_DBK_LON_Main_Ex,
PQ_IP_SPF_DBK_SDtoHD_Main_Ex,
PQ_IP_SPF_DBK_NUMS_Main_Ex
} PQ_IP_SPF_DBK_Group_Main_Ex;

typedef enum
{
PQ_IP_DMS_OFF_Main_Ex,
PQ_IP_DMS_ON_Main_Ex,
PQ_IP_DMS_NUMS_Main_Ex
} PQ_IP_DMS_Group_Main_Ex;

typedef enum
{
PQ_IP_DMS_H_J0_Main_Ex,
PQ_IP_DMS_H_J1_Main_Ex,
PQ_IP_DMS_H_NUMS_Main_Ex
} PQ_IP_DMS_H_Group_Main_Ex;

typedef enum
{
PQ_IP_DMS_V_J0_Main_Ex,
PQ_IP_DMS_V_J1_Main_Ex,
PQ_IP_DMS_V_NUMS_Main_Ex
} PQ_IP_DMS_V_Group_Main_Ex;

typedef enum
{
PQ_IP_DMS_NMR_OFF_Main_Ex,
PQ_IP_DMS_NMR_ON_Main_Ex,
PQ_IP_DMS_NMR_NUMS_Main_Ex
} PQ_IP_DMS_NMR_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_HNMR_Y_lowY_OFF_Main_Ex,
PQ_IP_VIP_HNMR_Y_lowY_S2_Main_Ex,
PQ_IP_VIP_HNMR_Y_lowY_NUMS_Main_Ex
} PQ_IP_VIP_HNMR_Y_lowY_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_VNMR_Y_lowY_OFF_Main_Ex,
PQ_IP_VIP_VNMR_Y_lowY_S2_Main_Ex,
PQ_IP_VIP_VNMR_Y_lowY_NUMS_Main_Ex
} PQ_IP_VIP_VNMR_Y_lowY_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_Peaking_adptive_RF_NTSC_Main_Ex,
PQ_IP_VIP_Peaking_adptive_AV_NTSC_Main_Ex,
PQ_IP_VIP_Peaking_adptive_AV_PAL_Main_Ex,
PQ_IP_VIP_Peaking_adptive_480i_Main_Ex,
PQ_IP_VIP_Peaking_adptive_720p_Main_Ex,
PQ_IP_VIP_Peaking_adptive_1080i_Main_Ex,
PQ_IP_VIP_Peaking_adptive_H_480i_Main_Ex,
PQ_IP_VIP_Peaking_adptive_H_576i_Main_Ex,
PQ_IP_VIP_Peaking_adptive_H_720p_Main_Ex,
PQ_IP_VIP_Peaking_adptive_H_1080i_Main_Ex,
PQ_IP_VIP_Peaking_adptive_DT_MPEG2_480is_Main_Ex,
PQ_IP_VIP_Peaking_adptive_DT_MPEG2_576i_Main_Ex,
PQ_IP_VIP_Peaking_adptive_DT_MPEG2_1080i_Main_Ex,
PQ_IP_VIP_Peaking_adptive_DT_H264_480is_Main_Ex,
PQ_IP_VIP_Peaking_adptive_DT_H264_576p_Main_Ex,
PQ_IP_VIP_Peaking_adptive_DT_H264_720p_Main_Ex,
PQ_IP_VIP_Peaking_adptive_DT_H264_1080i_Main_Ex,
PQ_IP_VIP_Peaking_adptive_DT_H264_1080p_Main_Ex,
PQ_IP_VIP_Peaking_adptive_PC_mode_Main_Ex,
PQ_IP_VIP_Peaking_adptive_NUMS_Main_Ex
} PQ_IP_VIP_Peaking_adptive_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_Peaking_alpha_MIN_OFF_Main_Ex,
PQ_IP_VIP_Peaking_alpha_MIN_m1_Main_Ex,
PQ_IP_VIP_Peaking_alpha_MIN_m2_Main_Ex,
PQ_IP_VIP_Peaking_alpha_MIN_NUMS_Main_Ex
} PQ_IP_VIP_Peaking_alpha_MIN_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_Peaking_Pcoring_ad_C_OFF_Main_Ex,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T1_Main_Ex,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T2_Main_Ex,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T3_Main_Ex,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T4_Main_Ex,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T5_Main_Ex,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T6_Main_Ex,
PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_Main_Ex
} PQ_IP_VIP_Peaking_Pcoring_ad_C_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_Peaking_gain_by_Cedge_OFF_Main_Ex,
PQ_IP_VIP_Peaking_gain_by_Cedge_T2_Main_Ex,
PQ_IP_VIP_Peaking_gain_by_Cedge_NUMS_Main_Ex
} PQ_IP_VIP_Peaking_gain_by_Cedge_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_Peaking_V_dering_LPF_OFF_Main_Ex,
PQ_IP_VIP_Peaking_V_dering_LPF_VDLPF1_Main_Ex,
PQ_IP_VIP_Peaking_V_dering_LPF_NUMS_Main_Ex
} PQ_IP_VIP_Peaking_V_dering_LPF_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_Peaking_V_dering_Adpt_OFF_Main_Ex,
PQ_IP_VIP_Peaking_V_dering_Adpt_VDAdpt1_Main_Ex,
PQ_IP_VIP_Peaking_V_dering_Adpt_VDAdpt2_Main_Ex,
PQ_IP_VIP_Peaking_V_dering_Adpt_NUMS_Main_Ex
} PQ_IP_VIP_Peaking_V_dering_Adpt_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_Peaking_V_dering_Table_OFF_Main_Ex,
PQ_IP_VIP_Peaking_V_dering_Table_VDTB1_Main_Ex,
PQ_IP_VIP_Peaking_V_dering_Table_NUMS_Main_Ex
} PQ_IP_VIP_Peaking_V_dering_Table_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_Post_SNR_OFF_Main_Ex,
PQ_IP_VIP_Post_SNR_ON_Main_Ex,
PQ_IP_VIP_Post_SNR_NUMS_Main_Ex
} PQ_IP_VIP_Post_SNR_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_ICC_Target_OFF_Main_Ex,
PQ_IP_VIP_ICC_Target_ON_Main_Ex,
PQ_IP_VIP_ICC_Target_NUMS_Main_Ex
} PQ_IP_VIP_ICC_Target_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_IBC_weight_W6_Main_Ex,
PQ_IP_VIP_IBC_weight_W8_Main_Ex,
PQ_IP_VIP_IBC_weight_NUMS_Main_Ex
} PQ_IP_VIP_IBC_weight_Group_Main_Ex;

typedef enum
{
PQ_IP_VIP_ACK_OFF_Main_Ex,
PQ_IP_VIP_ACK_NUMS_Main_Ex
} PQ_IP_VIP_ACK_Group_Main_Ex;

extern code U8 MST_SkipRule_IP_Main_Ex[PQ_IP_NUM_Main_Ex];
extern code EN_IPTAB_INFO PQ_IPTAB_INFO_Main_Ex[];
extern code U8 QMAP_3840_Main_Ex[QM_INPUTTYPE_NUM_Main_Ex][PQ_IP_NUM_Main_Ex];
extern code U8 QMAP_3840_PIP_Main_Ex[QM_INPUTTYPE_NUM_Main_Ex][PQ_IP_NUM_Main_Ex];
extern code U8 QMAP_3840_POP_Main_Ex[QM_INPUTTYPE_NUM_Main_Ex][PQ_IP_NUM_Main_Ex];
extern code U8 QMAP_2560_Main_Ex[QM_INPUTTYPE_NUM_Main_Ex][PQ_IP_NUM_Main_Ex];
extern code U8 QMAP_2560_PIP_Main_Ex[QM_INPUTTYPE_NUM_Main_Ex][PQ_IP_NUM_Main_Ex];
extern code U8 QMAP_2560_POP_Main_Ex[QM_INPUTTYPE_NUM_Main_Ex][PQ_IP_NUM_Main_Ex];

#endif
