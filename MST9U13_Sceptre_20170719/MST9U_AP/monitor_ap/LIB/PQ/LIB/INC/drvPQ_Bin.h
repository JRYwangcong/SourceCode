//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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
#ifndef _DRV_PQ_BIN_H_
#define _DRV_PQ_BIN_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------



#define PQ_BIN_MAX_IP       256
#define PQ_BIN_MAX_PNL      30

#define PQ_BIN_IP_ALL       0xFFFF
#define PQ_BIN_IP_NULL      0xFFFF
#define PQ_BIN_IP_COMM      0xFFFE
#define PQ_BIN_ADDR_NULL    0xFFFFFFFF

#define PQ_BIN_BANK_SIZE    1
#define PQ_BIN_ADDR_SIZE    1
#define PQ_BIN_MASK_SIZE    1

#define PQ_BIN_HEADER_LEN   16
#define PQ_BIN_VERSION_LEN  16
#define PQ_BIN_DUMMY        8

#define PQ_TEXT_BIN_HEADER_LEN   16
#define PQ_TEXT_BIN_VERSION_LEN  16
#define PQ_TEXT_BIN_DUMMY        8


//Size of each table
#define PQ_BIN_GRULE_INFO_SIZE              14
#define PQ_BIN_GRULE_LEVEL_INFO_SIZE        6
#define PQ_BIN_XRULE_INFO_SIZE              12
#define PQ_BIN_SKIPRULE_INFO_SIZE           6
#define PQ_BIN_IP_COMM_INFO_SIZE            16
#define PQ_BIN_SOURCELUT_INFO_SIZE          8


// Item Offset of each table
// GRule
#define PQ_BIN_GRULE_HEADER_LEN             (16)
#define PQ_BIN_GRULE_RULE_NUM_LEN           (2)
#define PQ_BIN_GRULE_PANEL_NUM_LEN          (2)

#define PQ_BIN_GRULE_IP_NUM_OFFSET          (0)
#define PQ_BIN_GRULE_SOURCE_NUM_OFFSET      (PQ_BIN_GRULE_IP_NUM_OFFSET+2)
#define PQ_BIN_GRULE_GROUP_NUM_OFFSET       (PQ_BIN_GRULE_SOURCE_NUM_OFFSET+2)
#define PQ_BIN_GRULE_IPOFFSET_OFFSET        (PQ_BIN_GRULE_GROUP_NUM_OFFSET+2)
#define PQ_BIN_GRULE_RULEOFFSET_OFFSET      (PQ_BIN_GRULE_IPOFFSET_OFFSET+4)

#define PQ_BIN_GRULE_LEVEL_LVL_NUM_OFFSET   (0)
#define PQ_BIN_GRULE_LEVEL_OFFSET_OFFSET    (PQ_BIN_GRULE_LEVEL_LVL_NUM_OFFSET+2)


// XRule
#define PQ_BIN_XRULE_HEADER_LEN             (16)
#define PQ_BIN_XRULE_RULE_NUM_LEN           (2)

#define PQ_BIN_XRULE_IP_NUM_OFFSET          (0)
#define PQ_BIN_XRULE_GROUP_NUM_OFFSET       (PQ_BIN_XRULE_IP_NUM_OFFSET+2)
#define PQ_BIN_XRULE_IPOFFSET_OFFSET        (PQ_BIN_XRULE_GROUP_NUM_OFFSET+2)
#define PQ_BIN_XRULE_GROUPOFFSET_OFFSET     (PQ_BIN_XRULE_IPOFFSET_OFFSET+4)


// SkipRule
#define PQ_BIN_SKIPRULE_HEADER_LEN          (16)
#define PQ_BIN_SKIPRULE_IP_NUM_OFFSET       (0)
#define PQ_BIN_SKIPRULE_OFFSET_OFFSET       (PQ_BIN_SKIPRULE_IP_NUM_OFFSET+2)


// IP & Common
#define PQ_BIN_IP_COMM_HEADER_LEN           (16)
#define PQ_BIN_IP_COMM_IP_NUM_LEN           (2)
#define PQ_BIN_IP_COMM_TABLE_TYPE_OFFSET    (0)
#define PQ_BIN_IP_COMM_COMM_REG_NUM_OFFSET  (PQ_BIN_IP_COMM_TABLE_TYPE_OFFSET+2)
#define PQ_BIN_IP_COMM_IP_REG_NUM_OFFSET    (PQ_BIN_IP_COMM_COMM_REG_NUM_OFFSET+2)
#define PQ_BIN_IP_COMM_IP_GROUP_NUM_OFFSET  (PQ_BIN_IP_COMM_IP_REG_NUM_OFFSET+2)
#define PQ_BIN_IP_COMM_COMMOFFSET_OFFSET    (PQ_BIN_IP_COMM_IP_GROUP_NUM_OFFSET+2)
#define PQ_BIN_IP_COMM_IPOFFSET_OFFSET      (PQ_BIN_IP_COMM_COMMOFFSET_OFFSET+4)


// SourceLUT
#define PQ_BIN_SOURCELUT_HEADER_LEN         (16)
#define PQ_BIN_SOURCELUT_PANEL_NUM_LEN      (2)

#define PQ_BIN_SOURCELUT_SOURCE_NUM_OFFSET  (0)
#define PQ_BIN_SOURCELUT_IP_NUM_OFFSET      (PQ_BIN_SOURCELUT_SOURCE_NUM_OFFSET+2)
#define PQ_BIN_SOURCELUT_OFFSET_OFFSET      (PQ_BIN_SOURCELUT_IP_NUM_OFFSET+2)



#define PQ_BIN_SOURCELUT_DATA_LEN           2 // 2 bytes
#define PQ_BIN_IP_COMM_DATA_LEN             1 // 1 byte


// Text Bin
//Size of each table
#define PQ_TEXT_BIN_SRCTYPE_INFO_SIZE       6
#define PQ_TEXT_BIN_IPNAME_INFO_SIZE        6
#define PQ_TEXT_BIN_GROUPNAME_INFO_SIZE     6

// SrcType
#define PQ_TEXT_BIN_SRCTYPE_HEADER_LEN      (16)
#define PQ_TEXT_BIN_SRCTYPE_SRCNUM_LEN      (2)
#define PQ_TEXT_BIN_SRCTYPE_SRC_NUM_OFFSET  (0)
#define PQ_TEXT_BIN_SRCTYPE_OFFSET_OFFSET   (PQ_TEXT_BIN_SRCTYPE_SRC_NUM_OFFSET+2)


// IPName
#define PQ_TEXT_BIN_IPNAME_HEADER_LEN       (16)
#define PQ_TEXT_BIN_IPNAME_IPNUM_LEN        (2)
#define PQ_TEXT_BIN_IPNAME_IPNUM_OFFSET     (0)
#define PQ_TEXT_BIN_IPNAME_OFFSET_OFFSET    (PQ_TEXT_BIN_IPNAME_IPNUM_OFFSET+2)

// GroupName
#define PQ_TEXT_BIN_GROUPNAME_HEADER_LEN                (16)
#define PQ_TEXT_BIN_GROUPNAME_IPNUM_LEN                 (2)
#define PQ_TEXT_BIN_GROUPNAME_IPGROUP_GROUPNUM_OFFSET   (0)
#define PQ_TEXT_BIN_GROUPNAME_IPGROUP_OFFSET_OFFSET     (PQ_TEXT_BIN_GROUPNAME_IPGROUP_GROUPNUM_OFFSET+2)

#define PQ_TEXT_BIN_GROUPNAME_GROUPOFFSET_LEN           (4)
#define PQ_TEXT_BIN_GROUPNAME_GROUPLENGTH_LEN           (2)
#define PQ_TEXT_BIN_GROUPNAME_GROUPOFFSET_OFFSET        (0)

/// ApiStatusEX version of current XC lib
#define API_STATUS_EX_VERSION                          2

//-------------------------------------------------------------------------------------------------
//  enum
//-------------------------------------------------------------------------------------------------


typedef enum
{
    PQ_BIN_DISPLAY_ONE = 0,
    PQ_BIN_DISPLAY_PIP,
    PQ_BIN_DISPLAY_POP,
    PQ_BIN_DISPLAY_NUM,
}PQ_BIN_DISPLAY_TYPE;

/// Define DeInterlace mode
typedef enum
{

    MS_DEINT_OFF=0,         ///<deinterlace mode off
    MS_DEINT_2DDI_BOB,      ///<deinterlace mode: BOB
    MS_DEINT_2DDI_AVG,      ///<deinterlace mode: AVG
    MS_DEINT_3DDI_HISTORY,  ///<deinterlace mode: HISTORY // 24 bit
    MS_DEINT_3DDI,          ///<deinterlace mode:3DDI// 16 bit
} MS_DEINTERLACE_MODE;

/// Image store format in XC
typedef enum
{
    IMAGE_STORE_444_24BIT,    ///< (8+8+8)   Y Cb Cr   / B G R
    IMAGE_STORE_422_16BIT,    ///< (8+8)     Y Cb Y Cr / G B G R
    IMAGE_STORE_422_24BIT,    ///< (10+10+4) Y Cb Y Cr / G B G R -- Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0 - C5 C4 C3 C2 C1 C0 Y9 Y8 - xx xx xx xx C9 C8 C7 C6
} MS_IMAGE_STORE_FMT;

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#if 1   //$$$
/// XC API status
typedef struct
{
    MS_U32 u32ApiStatusEx_Version;   ///<Version of current structure. Please always set to "PANEL_INFO_EX_VERSION" as input
    MS_U16 u16ApiStatusEX_Length;    ///<Length of this structure, u16PanelInfoEX_Length=sizeof(XC_PANEL_INFO_EX)

    //----------------------
    // Customer setting
    //----------------------
    INPUT_SOURCE_TYPE_t enInputSourceType;      ///< Input source type

    //----------------------
    // Window
    //----------------------
    MS_WINDOW_TYPE stCapWin;       ///<Capture window
    MS_WINDOW_TYPE stDispWin;      ///<Display window
    MS_WINDOW_TYPE stCropWin;      ///<Crop window

    //----------------------
    // Timing
    //----------------------
    MS_BOOL bInterlace;             ///<Interlaced or Progressive
    MS_BOOL bHDuplicate;            ///<flag for vop horizontal duplicate, for MVD, YPbPr, indicate input double sampled or not
    MS_U16  u16InputVFreq;          ///<Input V Frequency, VFreqx10, for calculate output panel timing
    MS_U16  u16InputVTotal;         ///<Input Vertical total, for calculate output panel timing
    MS_U16  u16DefaultHtotal;       ///<Default Htotal for VGA/YPbPr input
    MS_U8   u8DefaultPhase;         ///<Obsolete, use u16DefaultPhase instead

    //----------------------
    // Post customized scaling
    //----------------------
    MS_BOOL bHCusScaling;           ///<assign H customized scaling instead of using XC scaling
    MS_U16  u16HCusScalingSrc;      ///<H customized scaling src width
    MS_U16  u16HCusScalingDst;      ///<H customized scaling dst width
    MS_BOOL bVCusScaling;           ///<assign V manuel scaling instead of using XC scaling
    MS_U16  u16VCusScalingSrc;      ///<V customized scaling src height
    MS_U16  u16VCusScalingDst;      ///<V customized scaling dst height

    //--------------
    // 9 lattice
    //--------------
    MS_BOOL bDisplayNineLattice;    ///<used to indicate where to display in panel and where to put in frame buffer

    //----------------------
    // XC internal setting
    //----------------------

    /* scaling ratio */
    MS_U16 u16H_SizeAfterPreScaling;         ///<Horizontal size after prescaling
    MS_U16 u16V_SizeAfterPreScaling;         ///<Vertical size after prescaling
    MS_BOOL bPreV_ScalingDown;      ///<define whether it's pre-Vertical scaling down

    /* real crop win in memory */
    MS_WINDOW_TYPE ScaledCropWin;

    /* others */
    MS_U32 u32Op2DclkSet;           ///<OP to Dot clock set

    /* Video screen status */
    MS_BOOL bBlackscreenEnabled;      ///<Black screen status
    MS_BOOL bBluescreenEnabled;       ///<Blue screen status
    MS_U16 u16VideoDark;            ///<Video dark

    MS_U16 u16V_Length;             ///<for MDrv_Scaler_SetFetchNumberLimit
    MS_U16 u16BytePerWord;          ///<BytePerWord in Scaler
    MS_U16 u16OffsetPixelAlignment; ///<Pixel alignment of Offset (including IPM/OPM)
    MS_U8 u8BitPerPixel;            ///<Bits number Per Pixel
    MS_DEINTERLACE_MODE eDeInterlaceMode;       ///<De-interlace mode
    MS_U8 u8DelayLines;             ///<delay lines
    MS_BOOL bMemFmt422;             ///<whether memory format is 422, for easy usage
    MS_IMAGE_STORE_FMT eMemory_FMT; ///<memory format
    MS_BOOL bForceNRoff;            ///<Force NR off
    MS_BOOL bEn3DNR;                ///<whether it's 3DNR enabled
    MS_BOOL bUseYUVSpace;           ///< color format before 3x3 matrix
    MS_BOOL bMemYUVFmt;             ///< memroy color format
    MS_BOOL bForceRGBin;            ///<Force set format in memory as RGB (only for RGB input source)
    MS_BOOL bLinearMode;            ///<Is current memory format LinearMode?

    // frame lock related
    // only main can select FB/FBL because panel output timing depends on main window, sub will always use FB
    MS_BOOL bFBL;                   ///<whether it's framebufferless case
    MS_BOOL bFastFrameLock;         ///<whether framelock is enabled or not
    MS_BOOL bDoneFPLL;              ///<whether the FPLL is done
    MS_BOOL bEnableFPLL;            ///<enable FPLL or not
    MS_BOOL bFPLL_LOCK;             ///<whether FPLL is locked (in specific threshold.)

    // Capture_Memory
    MS_U32 u32IPMBase0;             ///<IPM base 0
    MS_U32 u32IPMBase1;             ///<IPM base 1
    MS_U32 u32IPMBase2;             ///<IPM base 2
    MS_U16 u16IPMOffset;            ///<IPM offset
    MS_U32 u16IPMFetch;             ///<IPM fetch

    //----------------------
    // Pre customized scaling
    //----------------------
    MS_BOOL bPreHCusScaling;           ///<assign H customized scaling instead of using XC scaling
    MS_U16  u16PreHCusScalingSrc;      ///<H customized scaling src width
    MS_U16  u16PreHCusScalingDst;      ///<H customized scaling dst width
    MS_BOOL bPreVCusScaling;           ///<assign V manuel scaling instead of using XC scaling
    MS_U16  u16PreVCusScalingSrc;      ///<V customized scaling src height
    MS_U16  u16PreVCusScalingDst;      ///<V customized scaling dst height

    MS_BOOL bPQSetHSD;///<H_SizeAfterPreScaling is changed manually, which will skip PQ HSD sampling and filter

    MS_U16  u16DefaultPhase;           ///<Default Phase for VGA/YPbPr input
}XC_ApiStatusEx;

#endif





typedef struct
{
    MS_U16 u16IPNum;
    MS_U16 u16SourceNum;
    MS_U16 u16GroupNum;
    MS_U32 u32IPOffset;
    MS_U32 u32RuleOffset;
}MS_PQBin_GRule_Info;

typedef struct
{
    MS_U16 u16LvlNum;
    MS_U32 u32Offset;
}MS_PQBin_GRule_Level_Info;

typedef struct
{
    MS_U16 u16IPNum;
    MS_U16 u16GroupNum;
    MS_U32 u32IPOffset;
    MS_U32 u32GroupOffset;
}MS_PQBin_XRule_Info;


typedef struct
{
    MS_U16 u16IPNum;
    MS_U32 u32Offset;
}MS_PQBin_SkipRule_Info;


typedef struct
{
    MS_U16 u16SourceNum;
    MS_U16 u16IPNum;
    MS_U32 u32Offset;
}MS_PQBin_SourceLUT_Info;


typedef struct
{
    MS_U16 u16TableType;
    MS_U16 u16CommRegNum;
    MS_U16 u16IPRegNum;
    MS_U16 u16IPGroupNum;
    MS_U32 u32CommOffset;
    MS_U32 u32IPOffset;
}MS_PQBin_IP_Common_Info;

typedef struct
{
    MS_U8 u8BinID;
    MS_U32 u32BinStartAddress;
    char  u8Header[PQ_BIN_HEADER_LEN];
    char  u8Version[PQ_BIN_VERSION_LEN];
    MS_U32 u32StartOffset;
    MS_U32 u32EndOffset;
    char  u8Dummy[PQ_BIN_DUMMY];

    // GRule
    char   u8GRuleHader[PQ_BIN_HEADER_LEN];
    MS_U16 u16GRule_RuleNum;
    MS_U16 u16GRule_PnlNum;
    MS_U32 u32GRule_Offset;
    MS_U32 u32GRule_Pitch;
    MS_U32 u32GRule_Lvl_Offset;
    MS_U32 u32GRule_Lvl_Pitch;

    // XRule
    char   u8XRuleHader[PQ_BIN_HEADER_LEN];
    MS_U16 u16XRuleNum;
    MS_U32 u32XRule_Offset;
    MS_U32 u32XRUle_Pitch;

    // SkipRule
    char   u8SkipRuleHader[PQ_BIN_HEADER_LEN];
    MS_U16 u16SkipRule_IPNum;
    MS_U32 u32SkipRule_Offset;

    // IP & Common
    char   u8IP_Comm_Header[PQ_BIN_HEADER_LEN];
    MS_U16 u16IP_Comm_Num;
    MS_U32 u32IP_Comm_Offset;
    MS_U32 u32IP_Comm_Pitch;


    // SourceLUT
    MS_U8  u8SourceLUT_Header[PQ_BIN_HEADER_LEN];
    MS_U16 u16SourceLUT_PnlNum;
    MS_U32 u32SourceLUT_Offset;
    MS_U32 u32SourceLUT_Pitch;
} MS_PQBin_Header_Info;

typedef struct
{
    MS_U8 u8BinID;
    MS_U32 u32BinStartAddress;
    char  u8Header[PQ_TEXT_BIN_HEADER_LEN];
    char  u8Version[PQ_TEXT_BIN_HEADER_LEN];
    MS_U32 u32StartOffset;
    MS_U32 u32EndOffset;
    MS_U8  u8Dummy[PQ_TEXT_BIN_HEADER_LEN];

    //SourceType
    char   u8SrcTypeHeader[PQ_TEXT_BIN_HEADER_LEN];
    MS_U16 u16SrcNum;
    MS_U32 u32SrcType_Offset;
    MS_U32 u32SrcType_Pitch;

    //IPName
    char   u8IPNameHeader[PQ_TEXT_BIN_HEADER_LEN];
    MS_U16 u16IPNum;
    MS_U32 u32IPName_Offset;
    MS_U32 u32IPName_Pitch;

    //GroupName
    char   u8GroupNameHeader[PQ_TEXT_BIN_HEADER_LEN];
    MS_U16 u16GroupIPNum;
    MS_U32 u32IP_Group_Offset;
    MS_U32 u32IP_Group_pitch;
}MS_PQTextBin_Header_Info;

typedef struct
{
   MS_U32 u32TabOffset;
   MS_U16 u16RegNum;
   MS_U16 u16TableType;
   MS_U16 u16GroupNum;
   MS_U16 u16GroupIdx;
} MS_PQBin_IP_Table_Info;

MS_U8 MDrv_PQBin_GetByteData(DWORD pAddrVirt, MS_U32 *u32Offset);
MS_U16 MDrv_PQBin_Get2ByteData(DWORD pAddrVirt, MS_U32 *u32Offset);



void MDrv_PQBin_Parsing(MS_PQBin_Header_Info *pPQBinHeader);
void MDrv_PQBin_LoadCommTable(MS_U16 u16PnlIdx, MS_PQBin_Header_Info *pPQBinHeader);
MS_U16 MDrv_PQBin_GetIPNum(const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetCurrentTableIndex(MS_U16 u16PnlIdx, MS_U16 u16PQIPIdx);
MS_U16 MDrv_PQBin_GetTableNum(MS_U16 u16PQIPIdx, const MS_PQBin_Header_Info* const pPQBinHeader);
void MDrv_PQBin_LoadTableData(MS_U16 u16PnlIdx, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_PQBin_Header_Info *pPQBinHeader, MS_U8 *pTable, MS_U16 u16TableSize);
void MDrv_PQBin_LoadTable(MS_U16 u16PnlIdx, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_PQBin_Header_Info *pPQBinHeader);
void MDrv_PQBin_GetTable(MS_U16 u16PnlIdx, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_PQBin_IP_Table_Info *pTableInfo, const MS_PQBin_Header_Info* const pPQBinHeader);
void MDrv_PQBin_DumpTable(MS_PQBin_IP_Table_Info* pTabInfo);
MS_U16 MDrv_PQBin_GetTableIndex(MS_U16 u16PQSrcType, MS_U16 u16PQIPIdx, MS_U16 u16PQPnlIdx, const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetSkipRule(MS_U16 u16PQIPIdx, const MS_PQBin_Header_Info* const pPQBinHeader);
void MDrv_PQBin_LoadTableBySrcType(MS_U16 u16PQSrcType, MS_U16 u16PQIPIdx, MS_U16 u16PQPnlIdx, MS_PQBin_Header_Info *pPQBinHeader, PQ_WIN eWindow, MS_BOOL isWinRot);


MS_U16 MDrv_PQBin_GetXRuleTableIndex(MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP, const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetXRuleIPIndex(MS_U16 u16XRuleType, MS_U16 u16XRuleIP, const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetXRuleIPNum(MS_U16 u16XRuleType, const MS_PQBin_Header_Info* const pPQBinHeader);


MS_U16 MDrv_PQBin_GetGRule_LevelIndex(MS_U16 u16PnlIdx, MS_U16 u16GRuleType, MS_U16 u16GRuleLvlIndex, const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetGRule_IPIndex(MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex, const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetGRule_TableIndex(MS_U16 u16GRuleType, MS_U16 u16PQSrcType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex, const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetGRule_GroupNum(MS_U16 u16GRuleType, const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetGRule_IPNum(MS_U16 u16GRuleType, const MS_PQBin_Header_Info* const pPQBinHeader);
MS_U16 MDrv_PQBin_GetGRule_LvlNum(    MS_U16 u16PnlIdx,    MS_U16 u16GRuleType,    const MS_PQBin_Header_Info* const pPQBinHeader);


void MDrv_PQBin_SetDisplayType(PQ_BIN_DISPLAY_TYPE enDisplayType);
PQ_BIN_DISPLAY_TYPE MDrv_PQBin_GetDisplayType(void);

void  MDrv_PQBin_SetPanelID(MS_U8 u8PnlID);
MS_U8 MDrv_PQBin_GetPanelIdx(void);

void MDrv_PQBin_Set_MLoadEn(MS_BOOL bEn);

void MDrv_PQBin_CheckCommTable(MS_U16 u16PnlIdx, MS_PQBin_Header_Info * pPQBinHeader);
void MDrv_PQBin_CheckTableBySrcType(MS_U16 u16PQSrcType, MS_U16 u16PQIPIdx, MS_U16 u16PQPnlIdx, MS_PQBin_Header_Info * pPQBinHeader, PQ_WIN eWindow);


void MDrv_PQTextBin_Parsing(MS_PQTextBin_Header_Info *pPQTextBinHeader);
MS_U32 MDrv_PQTextBin_GetSrcType(MS_U16 u16SrcIdx, const MS_PQTextBin_Header_Info* const pPQTextBinHeader);
MS_U32 MDrv_PQTextBin_GetIPName(MS_U16 u16PQIPIdx, const MS_PQTextBin_Header_Info* const pPQTextBinHeader);
MS_U32 MDrv_PQTextBin_GetTableName(MS_U16 u16PQIPIdx, MS_U16 u16TabIdx, const MS_PQTextBin_Header_Info* const pPQTextBinHeader);



#endif


