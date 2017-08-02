////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef  _API_SBOOT_H_
#define _API_SBOOT_H_
//-------------------------------------------------------------------------------------------------
// Version Control
//-------------------------------------------------------------------------------------------------
#define MSIF_TAG                    {'M','S','V','C'}                   // MSVC
#define MSIF_CLASS                  {'0','0'}                           // DRV/API (DDI)
#define MSIF_CUS                    {'0','0','S','3'}
#define MSIF_QUALITY                0
#define MSIF_MOD                    {'S','B','T','_'}
#define MSIF_DATE                   {'Y','Y','M','M','D','D'}
#define MSIF_SBT_CHANGELIST         {'0','0','5','3','6','7','3','5'}    //P4 ChangeList Number

#define SBT_VER                  /* Character String for SBOOT version               */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    MSIF_CUS,                       /* '00S0'                                           */  \
    MSIF_QUALITY,                   /* 0                                                */  \
    MSIF_MOD,                       /* 'SBT_'                                           */  \
    MSIF_DATE,                      /* 'TTMMDD'                                         */  \
    MSIF_SBT_CHANGELIST,            /* CL#                                              */  \
    {'0','0','0'}

typedef union _MSIF_Version
{
    struct _DDI
    {
        U8                       tag[4];
        U8                       type[2];
        U16                      customer;
        U16                      model;
        U16                      chip;
        U8                       cpu;
        U8                       name[4];
        U8                       version[2];
        U8                       build[2];
        U8                       change[8];
        U8                       os;
    } DDI;
    struct _MW
    {
        U8                                     tag[4];
        U8                                     type[2];
        U16                                    customer;
        U16                                    mod;
        U16                                    chip;
        U8                                     cpu;
        U8                                     name[4];
        U8                                     version[2];
        U8                                     build[2];
        U8                                     changelist[8];
        U8                                     os;
    } MW;
    struct _APP
    {
        U8                                     tag[4];
        U8                                     type[2];
        U8                                     id[4];
        U8                                     quality;
        U8                                     version[4];
        U8                                     time[6];
        U8                                     changelist[8];
        U8                                     reserve[3];
    } APP;
} MSIF_Version;

const MSIF_Version _sbt_version = {
    .APP = { SBT_VER }
};

#endif //_API_SBOOT_H_
