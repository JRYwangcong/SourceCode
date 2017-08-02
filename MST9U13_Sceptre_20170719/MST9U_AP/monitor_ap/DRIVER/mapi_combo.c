///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mapi_combo.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPI_COMBO_C_
#define _MAPI_COMBO_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "Global.h"
#include "mapi_combo.h"
#include "mdrv_combo.h"
#include "mhal_combo.h"
#include "drvADC.h"
#include "mapi_mhl.h"
#include "mapi_tmds.h"
#include "mdrv_combo.c"
//#include "drvDPRxComboApp.h"

#if(CHIP_ID == MST9U4)
   #include "mapi_DPTx.h"
#endif
#if(ENABLE_LOAD_KEY_VIA_EFUSE)
#include "msEread.h"
#endif

#include "combo_table.h"
#include "Mdrv_DPRX.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define COMBO_API_DEBUG_MESSAGE         1

#if(COMBO_DEBUG_MESSAGE & COMBO_API_DEBUG_MESSAGE)
#define COMBO_API_DPUTSTR(str)          printMsg(str)
#define COMBO_API_DPRINTF(str, x)       printData(str, x)
#else
#define COMBO_API_DPUTSTR(str)
#define COMBO_API_DPRINTF(str, x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Combo EDID table
//-------------------------------------------------------------------------------------------------
BYTE tMHL_EDID_SIZE_PORT[] =
{
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_1PORT)
    COMBO_EDID_SIZE_PORT0,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_2PORT)
    COMBO_EDID_SIZE_PORT1,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_3PORT)
    COMBO_EDID_SIZE_PORT2,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_4PORT)
    COMBO_EDID_SIZE_PORT3,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_5PORT)
    COMBO_EDID_SIZE_PORT4,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_6PORT)
    COMBO_EDID_SIZE_PORT5,
#endif
    COMBO_EDID_SIZE_VGA,
};

BYTE *tCOMBO_EDID_TALBE_PORT[] =
{
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_1PORT)
    tEDID_TABLE_COMBO_PORT0,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_2PORT)
    tEDID_TABLE_COMBO_PORT1,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_3PORT)
    tEDID_TABLE_COMBO_PORT2,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_4PORT)
    tEDID_TABLE_COMBO_PORT3,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_5PORT)
    tEDID_TABLE_COMBO_PORT4,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_6PORT)
    tEDID_TABLE_COMBO_PORT5,
#endif
    tEDID_TABLE_COMBO_VGA,
};

BYTE *tCOMBO_EDID_TALBE_PORT_HDMIFreeSync[] =
{
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_1PORT)
    tEDID_TABLE_COMBO_PORT0_HDMIFreeSync,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_2PORT)
    tEDID_TABLE_COMBO_PORT1_HDMIFreeSync,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_3PORT)
    tEDID_TABLE_COMBO_PORT2_HDMIFreeSync,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_4PORT)
    tEDID_TABLE_COMBO_PORT3_HDMIFreeSync,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_5PORT)
    tEDID_TABLE_COMBO_PORT4_HDMIFreeSync,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_6PORT)
    tEDID_TABLE_COMBO_PORT5_HDMIFreeSync,
#endif
    tEDID_TABLE_COMBO_VGA,
};

BYTE *tMHL_EDID_TALBE_PORT[] =
{
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_1PORT)
    tEDID_TABLE_COMBO_PORT0,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_2PORT)
    tEDID_TABLE_COMBO_PORT1,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_3PORT)
    tEDID_TABLE_COMBO_PORT2,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_4PORT)
    tEDID_TABLE_COMBO_PORT3,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_5PORT)
    tEDID_TABLE_COMBO_PORT4,
#endif
#if(COMBO_IP_SUPPORT_TYPE >= COMBO_IP_SUPPORT_6PORT)
    tEDID_TABLE_COMBO_PORT5,
#endif
    tEDID_TABLE_COMBO_VGA,
};

//-------------------------------------------------------------------------------------------------
//  Combo HDCP key
//-------------------------------------------------------------------------------------------------
#if (ENABLE_USER_PREF_HDCP||(ENABLE_LOAD_KEY_VIA_EFUSE == 0x1))
BYTE tCOMBO_HDCP_BKSV[COMBO_HDCP_BKSV_SIZE] = {0x0};
BYTE tCOMBO_HDCP_KEY_TABLE[COMBO_HDCP_KEY_SIZE] = {0x0};
#else
BYTE tCOMBO_HDCP_BKSV[COMBO_HDCP_BKSV_SIZE] =
{
    0x89, 0x38, 0xae, 0x0d, 0xed,
};

BYTE tCOMBO_HDCP_KEY_TABLE[COMBO_HDCP_KEY_SIZE] =
{
    0x00, 0xE6, 0xD2, 0xD0, 0xA4, 0x0A, 0xF9, 0xAB, 0x2F, 0x9A, 0x2D, 0x3A,
    0xB5, 0xFF, 0x9D, 0x37, 0xB3, 0x63, 0x23, 0x6E, 0xA3, 0x4E, 0x58, 0xEF,
    0xD2, 0x78, 0x6D, 0x3B, 0xDE, 0xFD, 0x7E, 0x4D, 0xAA, 0xB6, 0x0C, 0x59,
    0x4F, 0x74, 0x3C, 0xF8, 0xDA, 0x65, 0x6E, 0x2F, 0xB4, 0xAB, 0xAD, 0x7F,
    0x89, 0xA1, 0x92, 0x25, 0x1A, 0xCD, 0x50, 0x93, 0x50, 0x00, 0x63, 0x88,
    0x7B, 0xD0, 0xD8, 0x4D, 0x46, 0x95, 0xA8, 0xFC, 0x9B, 0x0D, 0xD1, 0x08,
    0x06, 0xB2, 0x67, 0xBE, 0xA2, 0x90, 0xA7, 0x84, 0x2A, 0x4B, 0x28, 0xC8,
    0xDD, 0x8F, 0xC9, 0x48, 0xA3, 0x0A, 0x7D, 0xCA, 0xDA, 0xBC, 0xCE, 0xFA,
    0x9B, 0x9F, 0x61, 0x90, 0x9E, 0x46, 0x24, 0x2B, 0xA6, 0x7B, 0xB1, 0x6F,
    0xC4, 0x4C, 0x63, 0xAF, 0x44, 0xE1, 0x3A, 0xDD, 0x0D, 0xED, 0xF4, 0x45,
    0x36, 0x47, 0x0B, 0x48, 0x0A, 0xB1, 0xE6, 0xE9, 0xB6, 0xED, 0x78, 0x37,
    0xDC, 0xB0, 0x07, 0x72, 0x83, 0xDC, 0x73, 0x26, 0x31, 0x49, 0x03, 0x52,
    0x7C, 0xE6, 0xC6, 0xE8, 0x3C, 0xD1, 0x62, 0xFE, 0xF0, 0x01, 0x73, 0x23,
    0x90, 0x70, 0xA9, 0x52, 0x8C, 0x75, 0x1C, 0xE5, 0x63, 0x7C, 0xA9, 0x98,
    0x5F, 0xD9, 0x7C, 0x62, 0x25, 0x6C, 0x61, 0x6C, 0xB5, 0xF9, 0xDB, 0xD8,
    0x91, 0x2F, 0x25, 0xF6, 0x3E, 0x37, 0xBE, 0xE2, 0x92, 0x08, 0x07, 0x3C,
    0xE6, 0xC9, 0xE3, 0xB0, 0x4D, 0xF6, 0xAD, 0x54, 0x3C, 0xED, 0x92, 0x5A,
    0x97, 0x93, 0xC0, 0x48, 0xEA, 0x23, 0x6C, 0xDD, 0x9A, 0x00, 0xBA, 0xF1,
    0xB4, 0xE7, 0x7E, 0x84, 0x9A, 0xB5, 0xD5, 0xF4, 0xB0, 0x42, 0x49, 0x5E,
    0x15, 0x5A, 0x43, 0x2D, 0xA9, 0xB0, 0x0A, 0x1C, 0x75, 0xDE, 0x2E, 0x08,
    0xF7, 0xDF, 0x38, 0x03, 0xEB, 0x6A, 0x75, 0xEB, 0xDA, 0x1C, 0xA0, 0xC9,
    0x2F, 0x2A, 0x69, 0x3F, 0x13, 0xFB, 0xA4, 0x23, 0x0A, 0xE5, 0x0A, 0xA5,
    0xEF, 0xCC, 0x8D, 0xB6, 0x5E, 0x6C, 0xB6, 0xE8, 0x84, 0xEF, 0x03, 0xDC,
    0xC5, 0x8A, 0xA3, 0x4B, 0xEF, 0x44, 0x12, 0x7F,
};
#endif

#if(COMBO_HDCP2_FUNCTION_SUPPORT)
BYTE tCOMBO_HDCP2_CERTRX_TABLE[1 +COMBO_HDCP2_ENCODE_KEY_SIZE] =
{
    0xD2, 0x35, 0xF5, 0xC0, 0x9A, 0xAC, 0xF5, 0xCA, 0x10, 0xB4, 0xE0, 0x38, 0x0A, 0x3F, 0xA8, 0x09,
	0x9B, 0xCF, 0x0D, 0x31, 0x63, 0x5E, 0x1B, 0x6F, 0x93, 0xAA, 0x87, 0x7D, 0x36, 0x4C, 0x12, 0x6E,
	0xFE, 0xB1, 0x02, 0xEF, 0x7D, 0xD3, 0x32, 0x86, 0x0C, 0x93, 0x8C, 0x9F, 0x6B, 0x12, 0x4B, 0x58,
	0x3C, 0xE8, 0x0F, 0x42, 0xE7, 0xEE, 0x8B, 0x6B, 0xBD, 0xB6, 0xB3, 0x6D, 0xEE, 0x55, 0x68, 0x30,
	0xA1, 0x8F, 0xB9, 0x0E, 0x9E, 0x69, 0x13, 0xC8, 0xF6, 0xDA, 0xB8, 0x5C, 0xE0, 0x53, 0xA5, 0xFD,
	0x0F, 0x0D, 0x0C, 0xD8, 0x76, 0xF3, 0xE3, 0x2B, 0xC2, 0xEB, 0x5D, 0xE5, 0xA1, 0x0B, 0x16, 0x05,
	0xA4, 0xAE, 0x67, 0xF1, 0xC9, 0x8C, 0xE8, 0x4B, 0xB2, 0x96, 0x26, 0xCB, 0x75, 0xDB, 0xAC, 0x50,
	0x5D, 0xE7, 0x2B, 0x58, 0xB8, 0x30, 0xDA, 0x9C, 0x73, 0xE5, 0xAE, 0x6A, 0xF0, 0x58, 0x46, 0x1D,
	0xF1, 0xA2, 0xD3, 0xBB, 0xAF, 0x01, 0x00, 0x01, 0x00, 0x00, 0x9B, 0xE4, 0x8C, 0xC1, 0xB4, 0x5B,
	0x76, 0x05, 0xEB, 0x21, 0x35, 0xCE, 0x4A, 0xC5, 0x13, 0x12, 0x70, 0xB8, 0xA1, 0x4D, 0x0E, 0x6C,
	0x84, 0x8B, 0x29, 0xF1, 0xDA, 0x8B, 0xCC, 0x50, 0xF4, 0x3F, 0x2B, 0xFF, 0x92, 0x31, 0xE4, 0x21,
	0x77, 0x32, 0xF7, 0xE4, 0xD5, 0xD4, 0x5B, 0xD2, 0x5E, 0xF5, 0x8E, 0xEF, 0x4A, 0xBA, 0xD0, 0x35,
	0x16, 0xB4, 0xFA, 0x38, 0xE0, 0xD1, 0x29, 0xB2, 0x58, 0xC2, 0x1B, 0x38, 0x7D, 0xD5, 0x84, 0x50,
	0x29, 0xA0, 0xDC, 0x41, 0x28, 0x8D, 0x69, 0x61, 0xFF, 0x86, 0x01, 0x72, 0x06, 0xB6, 0x25, 0x22,
	0x42, 0xFD, 0x15, 0xEA, 0xDE, 0x85, 0x89, 0x65, 0x97, 0xA1, 0x14, 0x89, 0x21, 0x1C, 0x85, 0x1C,
	0x7D, 0xC0, 0xEB, 0xF8, 0xED, 0x64, 0xD1, 0x61, 0xF6, 0xD9, 0xF5, 0x13, 0x18, 0x74, 0x9F, 0x4A,
	0x44, 0x9B, 0xC8, 0xF0, 0x6A, 0xCC, 0xCB, 0x02, 0x62, 0xCF, 0xFF, 0xF3, 0x9F, 0xB8, 0x6D, 0xCE,
	0x04, 0x80, 0x89, 0x06, 0xDB, 0xE5, 0xC7, 0x48, 0x63, 0xC2, 0x8C, 0xAD, 0x0A, 0xE4, 0xF0, 0x0D,
	0x66, 0x98, 0x82, 0x81, 0x35, 0x0F, 0x57, 0x51, 0x1B, 0x76, 0xF2, 0xA1, 0xD2, 0x39, 0xCB, 0x95,
	0xD9, 0x36, 0x86, 0x57, 0xE2, 0x54, 0x28, 0x1F, 0xEB, 0xAD, 0xCC, 0xF7, 0xD0, 0x2B, 0xD3, 0x0C,
	0x12, 0x97, 0xBB, 0xBA, 0xF1, 0x83, 0x6D, 0x0E, 0xA4, 0x47, 0x89, 0x55, 0x9C, 0x75, 0x36, 0x87,
	0x2B, 0x31, 0xCF, 0x3F, 0x20, 0x48, 0x2E, 0x47, 0x38, 0x57, 0xD1, 0xC5, 0x29, 0x99, 0x08, 0x29,
	0x1D, 0x7D, 0x39, 0x8E, 0xCB, 0x57, 0x65, 0xF4, 0x3F, 0x9C, 0x2C, 0xBE, 0xA9, 0x56, 0xAF, 0x4B,
	0x39, 0xA0, 0x21, 0x51, 0x7C, 0x7B, 0x36, 0x6E, 0xFF, 0x37, 0x58, 0x9B, 0x2F, 0x00, 0x50, 0xCE,
	0x34, 0xBC, 0xB1, 0x8B, 0xF6, 0x2C, 0xB2, 0xE5, 0x32, 0x7C, 0xC2, 0xD3, 0x94, 0x76, 0x49, 0x81,
	0xA7, 0x76, 0xD8, 0xCB, 0xBA, 0xFF, 0x80, 0xEB, 0x78, 0x1B, 0x8C, 0x34, 0xAA, 0x70, 0xF3, 0x9D,
	0xDC, 0xB2, 0x11, 0x55, 0xD1, 0x48, 0x43, 0xCA, 0xE5, 0x38, 0xFE, 0x4A, 0xD9, 0x4E, 0x3C, 0x57,
	0xBD, 0x27, 0x8B, 0x3E, 0xB9, 0x9B, 0x52, 0x19, 0xDE, 0xA0, 0x75, 0xF5, 0x17, 0x25, 0xD5, 0xDA,
	0xA6, 0x10, 0xEC, 0xBE, 0xD8, 0x41, 0x17, 0x6D, 0xDB, 0x7E, 0xAC, 0xA1, 0x69, 0x00, 0x9F, 0x2C,
	0x5B, 0x15, 0xEF, 0x23, 0xFF, 0x48, 0x70, 0xFE, 0x99, 0x06, 0x4E, 0xDC, 0x25, 0x3B, 0x2C, 0xE5,
	0xBE, 0x76, 0xB3, 0x70, 0xFF, 0x20, 0x7E, 0x2C, 0x09, 0x09, 0x24, 0xFF, 0xA6, 0xCB, 0x42, 0x55,
	0x29, 0xF8, 0x17, 0xCE, 0x0E, 0x91, 0x1F, 0x53, 0x86, 0xCA, 0x67, 0xCB, 0xD3, 0x51, 0x8F, 0xE6,
	0x8C, 0x6E, 0x26, 0xD3, 0xC7, 0x7F, 0x26, 0x0A, 0x01, 0xDD, 0xE0, 0x38, 0x0A, 0x3F, 0xA8, 0x09,
	0x9B, 0xCF, 0x0D, 0x31, 0x63, 0x5E, 0x1B, 0x6F, 0x93, 0xAA, 0x87, 0x7D, 0x36, 0x4C, 0x12, 0x6E,
	0xFE, 0xB1, 0x02, 0xEF, 0x7D, 0xD3, 0x32, 0x86, 0x0C, 0x93, 0x8C, 0x9F, 0x6B, 0x12, 0x4B, 0x58,
	0x3C, 0xE8, 0x0F, 0x42, 0xE7, 0xEE, 0x8B, 0x6B, 0xBD, 0xB6, 0xB3, 0x6D, 0xEE, 0x55, 0x68, 0x30,
	0xA1, 0x8F, 0xB9, 0x0E, 0x9E, 0x69, 0x13, 0xC8, 0xF6, 0xDA, 0xB8, 0x5C, 0xE0, 0x53, 0xA5, 0xFD,
	0x0F, 0x0D, 0x0C, 0xD8, 0x76, 0xF3, 0xE3, 0x2B, 0xC2, 0xEB, 0x5D, 0xE5, 0xA1, 0x0B, 0x16, 0x05,
	0xA4, 0xAE, 0x67, 0xF1, 0xC9, 0x8C, 0xE8, 0x4B, 0xB2, 0x96, 0x26, 0xCB, 0x75, 0xDB, 0xAC, 0x50,
	0x5D, 0xE7, 0x2B, 0x58, 0xB8, 0x30, 0xDA, 0x9C, 0x73, 0xE5, 0xAE, 0x6A, 0xF0, 0x58, 0x46, 0x1D,
	0xF1, 0xA2, 0xD3, 0xBB, 0xAF, 0x01, 0x00, 0x01, 0x00, 0x00, 0x9B, 0xE4, 0x8C, 0xC1, 0xB4, 0x5B,
	0x76, 0x05, 0xEB, 0x21, 0x35, 0xCE, 0x4A, 0xC5, 0x13, 0x12, 0x70, 0xB8, 0xA1, 0x4D, 0x0E, 0x6C,
	0x84, 0x8B, 0x29, 0xF1, 0xDA, 0x8B, 0xCC, 0x50, 0xF4, 0x3F, 0x2B, 0xFF, 0x92, 0x31, 0xE4, 0x21,
	0x77, 0x32, 0xF7, 0xE4, 0xD5, 0xD4, 0x5B, 0xD2, 0x5E, 0xF5, 0x8E, 0xEF, 0x4A, 0xBA, 0xD0, 0x35,
	0x16, 0xB4, 0xFA, 0x38, 0xE0, 0xD1, 0x29, 0xB2, 0x58, 0xC2, 0x1B, 0x38, 0x7D, 0xD5, 0x84, 0x50,
	0x29, 0xA0, 0xDC, 0x41, 0x28, 0x8D, 0x69, 0x61, 0xFF, 0x86, 0x01, 0x72, 0x06, 0xB6, 0x25, 0x22,
	0x42, 0xFD, 0x15, 0xEA, 0xDE, 0x85, 0x89, 0x65, 0x97, 0xA1, 0x14, 0x89, 0x21, 0x1C, 0x85, 0x1C,
	0x7D, 0xC0, 0xEB, 0xF8, 0xED, 0x64, 0xD1, 0x61, 0xF6, 0xD9, 0xF5, 0x13, 0x18, 0x74, 0x9F, 0x4A,
	0x44, 0x9B, 0xC8, 0xF0, 0x6A, 0xCC, 0xCB, 0x02, 0x62, 0xCF, 0xFF, 0xF3, 0x9F, 0xB8, 0x6D, 0xCE,
	0x04, 0x80, 0x89, 0x06, 0xDB, 0xE5, 0xC7, 0x48, 0x63, 0xC2, 0x8C, 0xAD, 0x0A, 0xE4, 0xF0, 0x0D,
	0x66, 0x98, 0x82, 0x81, 0x35, 0x0F, 0x57, 0x51, 0x1B, 0x76, 0xF2, 0xA1, 0xD2, 0x39, 0xCB, 0x95,
	0xD9, 0x36, 0x86, 0x57, 0xE2, 0x54, 0x28, 0x1F, 0xEB, 0xAD, 0xCC, 0xF7, 0xD0, 0x2B, 0xD3, 0x0C,
	0x12, 0x97, 0xBB, 0xBA, 0xF1, 0x83, 0x6D, 0x0E, 0xA4, 0x47, 0x89, 0x55, 0x9C, 0x75, 0x36, 0x87,
	0x2B, 0x31, 0xCF, 0x3F, 0x20, 0x48, 0x2E, 0x47, 0x38, 0x57, 0xD1, 0xC5, 0x29, 0x99, 0x08, 0x29,
	0x1D, 0x7D, 0x39, 0x8E, 0xCB, 0x57, 0x65, 0xF4, 0x3F, 0x9C, 0x2C, 0xBE, 0xA9, 0x56, 0xAF, 0x4B,
	0x39, 0xA0, 0x21, 0x51, 0x7C, 0x7B, 0x36, 0x6E, 0xFF, 0x37, 0x58, 0x9B, 0x2F, 0x00, 0x50, 0xCE,
	0x34, 0xBC, 0xB1, 0x8B, 0xF6, 0x2C, 0xB2, 0xE5, 0x32, 0x7C, 0xC2, 0xD3, 0x94, 0x76, 0x49, 0x81,
	0xA7, 0x76, 0xD8, 0xCB, 0xBA, 0xFF, 0x80, 0xEB, 0x78, 0x1B, 0x8C, 0x34, 0xAA, 0x70, 0xF3, 0x9D,
	0xDC, 0xB2, 0x11, 0x55, 0xD1, 0x48, 0x43, 0xCA, 0xE5, 0x38, 0xFE, 0x4A, 0xD9, 0x4E, 0x3C, 0x57,
	0xBD, 0x27, 0x8B, 0x3E, 0xB9, 0x9B, 0x52, 0x19, 0xDE, 0xA0, 0x75, 0xF5, 0x17, 0x25, 0xD5, 0xDA,
	0xA6, 0x10, 0xEC, 0xBE, 0xD8, 0x41, 0x17, 0x6D, 0xDB, 0x7E, 0xAC, 0xA1, 0x69, 0x00, 0x9F, 0x2C,
	0x5B, 0x15, 0xEF, 0x23, 0xFF, 0x48, 0x70, 0xFE, 0x99, 0x06, 0x4E, 0xDC, 0x25, 0x3B, 0x2C, 0xE5,
	0xBE, 0x76, 0xB3, 0x70, 0xFF, 0x20, 0x7E, 0x2C, 0x09, 0x09, 0x24, 0xFF, 0xA6, 0xCB, 0x42, 0x55,
	0x00
};

#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#if(COMBO_HDCP2_FUNCTION_SUPPORT)

static WORD gu16SecR2TimeoutCnt = 0;

static Bool bHDCP22InitProcDone = FALSE;
static MS_BOOL bSecRomCodeDone = FALSE;
#if(COMBO_HDCP2_DECODE_KEY)
static MS_BOOL bSecuStoreDone = FALSE;
#endif

#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
ST_COMBO_IP_PARAMETER stComboIPParameter = {FALSE, FALSE, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_GetComboMask()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_combo_GetComboMask(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucComboMask = 0;

    switch(ucComboSelect)
    {
        case COMBO_IP_SELECT_PORT0:
            ucComboMask = COMBO_IP_MASK_PORT0;
            break;

        case COMBO_IP_SELECT_PORT1:
            ucComboMask = COMBO_IP_MASK_PORT1;
            break;

        case COMBO_IP_SELECT_PORT2:
            ucComboMask = COMBO_IP_MASK_PORT2;
            break;

        case COMBO_IP_SELECT_PORT3:
            ucComboMask = COMBO_IP_MASK_PORT3;
            break;

        case COMBO_IP_SELECT_PORT4:
            ucComboMask = COMBO_IP_MASK_PORT4;
            break;

        case COMBO_IP_SELECT_PORT5:
            ucComboMask = COMBO_IP_MASK_PORT5;
            break;

        case COMBO_IP_SELECT_VGA:
            ucComboMask = COMBO_IP_MASK_VGA;
            break;

        default:

            break;
    };

    return ucComboMask;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_GetComboType()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
EN_COMBO_IP_TYPE mapi_combo_GetComboType(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucComboMask = mapi_combo_GetComboMask(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = COMBO_IP_TYPE_HDMI;

    if(stComboIPParameter.ucHDMIEnableIndex & ucComboMask)
    {
        ucComboIPType = COMBO_IP_TYPE_HDMI;
    }
    else if(stComboIPParameter.ucDPEnableIndex & ucComboMask)
    {
        ucComboIPType = COMBO_IP_TYPE_DP;
    }
    else if(stComboIPParameter.ucDualEnableIndex & ucComboMask)
    {
        ucComboIPType = COMBO_IP_TYPE_DUAL;
    }
    else if(stComboIPParameter.ucDVIEnableIndex & ucComboMask)
    {
        ucComboIPType = COMBO_IP_TYPE_DVI;
    }
    else if(stComboIPParameter.ucMHLEnableIndex & ucComboMask)
    {
        ucComboIPType = COMBO_IP_TYPE_MHL;
    }
    else
    {
        ucComboIPType = COMBO_IP_TYPE_VGA;
    }

    return ucComboIPType;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_GetComboEnableIndex()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_combo_GetComboEnableIndex(void)
{
    BYTE ucComboEnable = stComboIPParameter.ucHDMIEnableIndex;

    ucComboEnable |= stComboIPParameter.ucDVIEnableIndex;
    ucComboEnable |= stComboIPParameter.ucDualEnableIndex;
    ucComboEnable |= stComboIPParameter.ucDPEnableIndex;

    return ucComboEnable;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_GetComboInputPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_combo_GetComboInputPort(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucInputPort = COMBO_INPUT_SELECT_PORT0;

    switch(ucComboSelect)
    {
        case COMBO_IP_SELECT_PORT0:
            ucInputPort = COMBO_INPUT_SELECT_PORT0;
            break;

        case COMBO_IP_SELECT_PORT1:
            ucInputPort = COMBO_INPUT_SELECT_PORT1;
            break;

        case COMBO_IP_SELECT_PORT2:
            ucInputPort = COMBO_INPUT_SELECT_PORT2;
            break;

        case COMBO_IP_SELECT_PORT3:
            ucInputPort = COMBO_INPUT_SELECT_PORT3;
            break;

        case COMBO_IP_SELECT_PORT4:
            ucInputPort = COMBO_INPUT_SELECT_PORT4;
            break;

        case COMBO_IP_SELECT_PORT5:
            ucInputPort = COMBO_INPUT_SELECT_PORT5;
            break;

        case COMBO_IP_SELECT_VGA:
            ucInputPort = COMBO_INPUT_SELECT_VGA;
            break;

        default:

            break;
    };

    return ucInputPort;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_SetConfiguration()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_combo_SetConfiguration(void)
{
    BYTE ucEDIDEnableIndex = stComboIPParameter.ucDPEnableIndex;
    BYTE ucHDCPEnableIndex = stComboIPParameter.ucDPEnableIndex;

    if(stComboIPParameter.bVGAEnableIndex)
    {
        ucEDIDEnableIndex |= COMBO_IP_MASK_VGA;
    }

    ucHDCPEnableIndex |= stComboIPParameter.ucDVIEnableIndex;
    ucHDCPEnableIndex |= stComboIPParameter.ucDualEnableIndex;
    ucHDCPEnableIndex |= stComboIPParameter.ucHDMIEnableIndex;

#if(COMBO_TMDS_USE_INTERNAL_EDID)
    ucEDIDEnableIndex |= ucHDCPEnableIndex;
#endif

    mdrv_combo_SetConfiguration(ucEDIDEnableIndex, ucHDCPEnableIndex, stComboIPParameter.ucDPEnableIndex);

#if(CHIP_ID == MST9U2)
    mapi_DP_DaisychainFlag(stComboIPParameter.ucDaisyEnableIndex);
#endif

}

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_LoadHDCPKey()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_combo_LoadHDCPKey(BYTE *pHDCPKey, BYTE *pBKSV)
{
    EN_COMBO_IP_SELECT ucComboSelect = COMBO_IP_SELECT_PORT0;
	BYTE ucInputPort = COMBO_INPUT_SELECT_PORT0;
    EN_COMBO_IP_TYPE  ucComboIPType = COMBO_IP_TYPE_HDMI;

    for(ucComboSelect = COMBO_IP_SELECT_PORT0; ucComboSelect < COMBO_IP_SELECT_MASK; ucComboSelect++)
    {
        ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
		ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

        switch(ucComboIPType)
        {
            case COMBO_IP_TYPE_DVI:
                mdrv_combo_LoadHDCPKey(ucInputPort, 0x00, pHDCPKey, pBKSV);
                break;

            case COMBO_IP_TYPE_DUAL:
                mdrv_combo_LoadHDCPKey(ucInputPort, 0x00, pHDCPKey, pBKSV);
                mdrv_combo_LoadHDCPKey(mdrv_combo_GetDVIDualMainPort(ucInputPort), 0x00, pHDCPKey, pBKSV);
                break;

            case COMBO_IP_TYPE_HDMI:
            case COMBO_IP_TYPE_MHL:
                mdrv_combo_LoadHDCPKey(ucInputPort, 0x80, pHDCPKey, pBKSV);
                break;

            case COMBO_IP_TYPE_DP:
                mdrv_combo_LoadHDCPKey(ucInputPort, 0x00, pHDCPKey, pBKSV);
                break;

            default:

                break;
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_PowerConfiguration()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_combo_PowerConfiguration(void)
{
    static BYTE ucDisplayEnableIndex = 0;
    BYTE ucComboMask = 0;
    BYTE ucRtermEnableIndex = 0;
    BYTE ucRtermDisableIndex = 0;
    BYTE ucInputPort = COMBO_INPUT_SELECT_PORT0;
    EN_COMBO_IP_SELECT ucComboSelect = COMBO_IP_SELECT_PORT0;

    if(ucDisplayEnableIndex != stComboIPParameter.ucDisplayEnableIndex)
    {
        // Display change
        ucRtermDisableIndex = ucDisplayEnableIndex ^stComboIPParameter.ucDisplayEnableIndex;
        // Rtern enable
        ucRtermEnableIndex = ucRtermDisableIndex &stComboIPParameter.ucDisplayEnableIndex;
        // Rtern disable
        ucRtermDisableIndex = ucRtermDisableIndex &ucDisplayEnableIndex;
        
        for(ucComboSelect = COMBO_IP_SELECT_VGA; ucComboSelect < COMBO_IP_SELECT_MASK; ucComboSelect++)
        {
            ucComboMask = mapi_combo_GetComboMask(ucComboSelect);
            ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);

            if(ucRtermEnableIndex & ucComboMask)
            {
                switch(mapi_combo_GetComboType(ucComboSelect))
                {
                    case COMBO_IP_TYPE_VGA:
                        COMBO_API_DPRINTF("** Combo power on VGA!! %x", ucRtermEnableIndex);
                        break;
                        
                    case COMBO_IP_TYPE_DVI:
                    case COMBO_IP_TYPE_HDMI:
                    case COMBO_IP_TYPE_MHL:
#if(COMBO_TMDS_POWER_CONTROL)
	                    mdrv_combo_ClockRtermControl(ucInputPort, TRUE);
                        mapi_tmds_PowerControl(ucInputPort, TRUE, FALSE);

                        if(!mapi_mhl_GetCableDetectPort(ucInputPort))
                        {
                            mdrv_combo_DataRtermControl(ucInputPort, TRUE);
                        }
#endif
                        break;

                    case COMBO_IP_TYPE_DUAL:
#if(COMBO_TMDS_POWER_CONTROL)
						mdrv_combo_ClockRtermControl(ucInputPort, TRUE);
						mdrv_combo_ClockRtermControl(mdrv_combo_GetDVIDualMainPort(ucInputPort), TRUE);
						mapi_tmds_PowerControl(ucInputPort, TRUE, FALSE);
						mapi_tmds_PowerControl(mdrv_combo_GetDVIDualMainPort(ucInputPort), TRUE, FALSE);

                        mdrv_combo_DataRtermControl(ucInputPort, TRUE);
                        mdrv_combo_DataRtermControl(mdrv_combo_GetDVIDualMainPort(ucInputPort), TRUE);
#endif                        
                        break;

                    case COMBO_IP_TYPE_DP:
                        mdrv_combo_ClockRtermControl(ucInputPort, TRUE);
                        mdrv_combo_DataRtermControl(ucInputPort, TRUE);
                        break;

                    default:

                        break;
                };
            }
            else if(ucRtermDisableIndex & ucComboMask)
            {
                switch(mapi_combo_GetComboType(ucComboSelect))
                {
                    case COMBO_IP_TYPE_VGA:
                        COMBO_API_DPRINTF("** Combo power down VGA!! %x", ucRtermDisableIndex);
                        break;
                        
                    case COMBO_IP_TYPE_DVI:
                    case COMBO_IP_TYPE_HDMI:
                    case COMBO_IP_TYPE_MHL:
#if(COMBO_TMDS_POWER_CONTROL)
#if(COMBO_TMDS_CLOCK_DETECT)
						mapi_tmds_PowerControl(ucInputPort, FALSE, TRUE);
	                    mdrv_combo_ClockRtermControl(ucInputPort, TRUE);
#else
						mapi_tmds_PowerControl(ucInputPort, FALSE, FALSE);
	                    mdrv_combo_ClockRtermControl(ucInputPort, FALSE);
#endif
						if(!mapi_mhl_GetCableDetectPort(ucInputPort))
						{
							mdrv_combo_DataRtermControl(ucInputPort, FALSE);
						}
#endif
                        break;

                    case COMBO_IP_TYPE_DUAL:
#if(COMBO_TMDS_POWER_CONTROL)
#if(COMBO_TMDS_CLOCK_DETECT)
						mapi_tmds_PowerControl(ucInputPort, FALSE, TRUE);
						mapi_tmds_PowerControl(mdrv_combo_GetDVIDualMainPort(ucInputPort), FALSE, TRUE);
						mdrv_combo_ClockRtermControl(ucInputPort, TRUE);
						mdrv_combo_ClockRtermControl(mdrv_combo_GetDVIDualMainPort(ucInputPort), TRUE);
#else
						mapi_tmds_PowerControl(ucInputPort, FALSE, FALSE);
						mapi_tmds_PowerControl(mdrv_combo_GetDVIDualMainPort(ucInputPort), FALSE, FALSE);
						mdrv_combo_ClockRtermControl(ucInputPort, FALSE);
						mdrv_combo_ClockRtermControl(mdrv_combo_GetDVIDualMainPort(ucInputPort), FALSE);
#endif
						mdrv_combo_DataRtermControl(ucInputPort, FALSE);
						mdrv_combo_DataRtermControl(mdrv_combo_GetDVIDualMainPort(ucInputPort), FALSE);
#endif	                        
                        break;

                    case COMBO_IP_TYPE_DP:
                        //mdrv_combo_ClockRtermControl(ucInputPort, FALSE);
                        //mdrv_combo_DataRtermControl(ucInputPort, FALSE);
                        break;

                    default:

                        break;
                };
            }
        }

        ucDisplayEnableIndex = stComboIPParameter.ucDisplayEnableIndex;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_tmds_DualLink_Align()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_tmds_DualLink_Align(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    mapi_tmds_DualLink_Align(ucInputPort);
}

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_GetDaisyChainType()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mapi_combo_GetDaisyChainType(EN_COMBO_IP_SELECT ucComboSelect)
{
    Bool bDaisyChainFlag = FALSE;
    BYTE ucComboMask = mapi_combo_GetComboMask(ucComboSelect);

    if(stComboIPParameter.ucDaisyEnableIndex & ucComboMask)
    {
        bDaisyChainFlag = TRUE;
    }

    return bDaisyChainFlag;
}

#if(COMBO_HDCP2_FUNCTION_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  mapi_combo_HDCP2Initial()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_combo_HDCP2Initial(void)
{
    EN_COMBO_IP_SELECT ucComboSelect = COMBO_IP_SELECT_PORT0;
	BYTE ucInputPort = COMBO_INPUT_SELECT_PORT0;
    EN_COMBO_IP_TYPE  ucComboIPType = COMBO_IP_TYPE_HDMI;

    drv_HDCPMBX_Init();
    mdrv_combo_HDCP2Initial();
    drv_HDCPRx_Init(HDCP_PORT_ALL);
    
#if(COMBO_HDCP2_DECODE_KEY)
    drv_HDCPRx_DecodeKey(tCOMBO_HDCP2_CERTRX_TABLE);
#endif
    
    for(ucComboSelect = COMBO_IP_SELECT_PORT0; ucComboSelect < COMBO_IP_SELECT_MASK; ucComboSelect++)
    {
        ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
		ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

        switch(ucComboIPType)
        {
            case COMBO_IP_TYPE_DVI:
            case COMBO_IP_TYPE_DUAL:
            case COMBO_IP_TYPE_HDMI:
            case COMBO_IP_TYPE_MHL:
                mdrv_combo_SetHDCP2CallBackFunction(ucInputPort, mapi_tmds_HDCP2TxEventProc);
                break;

            case COMBO_IP_TYPE_DP:
                mdrv_combo_SetHDCP2CallBackFunction(ucInputPort, mapi_DP_HDCP2TxEventProc);
                break;

            default:

                break;
        }
    }
}

#if (COMBO_HDCP2_INITPROC_NEW_MODE)

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_HDCP2Initial()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool msAPI_combo_HDCP2InitHandler(void)
{
    static Bool bHDCP22TimerClr = FALSE;

    if (bHDCP22TimerClr == FALSE)
    {
        bHDCP22TimerClr = TRUE;
        gu16SecR2TimeoutCnt = 0;
    }
    
    if (gu16SecR2TimeoutCnt <= COMBO_HDCP2_SECUR2INT_WDT)
    {
    	if(mdrv_combo_HDCP2RomCodeDone())
    	{
    	    bSecRomCodeDone = TRUE;
    		mapi_combo_HDCP2Initial();
    		mapi_tmds_SetHDCP2CallBackFunction(mdrv_combo_HDCP2RxEventProc, tCOMBO_HDCP2_CERTRX_TABLE);

            #if !(COMBO_HDCP2_DECODE_KEY)
            {
                BYTE ucPortIdx = 0;
                
                for (ucPortIdx = 0; ucPortIdx < (BYTE)COMBO_IP_SELECT_PORT_END; ucPortIdx++ ) //pull high HPD after initial process is done
                {
                    msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)(ucPortIdx), TRUE);
                }
            }
            #endif
    	}

        #if(COMBO_HDCP2_DECODE_KEY)
        if(mdrv_combo_HDCP2KeyDecodeDone() && bSecRomCodeDone)
        {
            WORD i = 0;
            
    		for ( i = 0; i < sizeof(tCOMBO_HDCP2_CERTRX_TABLE); i++ ) //update from DRAM
    		{
    			tCOMBO_HDCP2_CERTRX_TABLE[i] = msMemReadByte((ULONG)tCOMBO_HDCP2_CERTRX_TABLE + i);
    			#if 0
    			printf("0x%02X ", tCOMBO_HDCP2_CERTRX_TABLE[i]);
    			if ((i+1) % 16 == 0)
    				printf("\r\n");
    			#endif
    		}
    		//printf("\r\n");

            bHDCP22InitProcDone = TRUE;
            bSecuStoreDone = TRUE;
        }

		if(bSecuStoreDone == TRUE)
		    mapi_DP_SetHDCP2CallBackFunction(mdrv_combo_HDCP2RxEventProc, tCOMBO_HDCP2_CERTRX_TABLE);
		
        #endif
    }
    else
    {
        if (bHDCP22InitProcDone == FALSE)
        {            
            bHDCP22InitProcDone = TRUE;
            printf("Wait SECU R2 Init Process Timeout!!\r\n");
        }
    }

    return bHDCP22InitProcDone;
}

#endif //#if (COMBO_HDCP2_INITPROC_NEW_MODE)

//**************************************************************************
//  [Function Name]:
//                  mapi_combo_HDCP2Initial()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_combo_HDCP2GetRecvID(BYTE *pucDataBuf)
{
    MS_BOOL bRet = FALSE;

    if ((bHDCP22InitProcDone == TRUE) && (pucDataBuf != NULL))
    {
        memcpy(pucDataBuf, tCOMBO_HDCP2_CERTRX_TABLE, sizeof(BYTE)*5);
        bRet = TRUE;
    }

    return bRet;
}

#endif //#if(COMBO_HDCP2_FUNCTION_SUPPORT)
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPSetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPSetPowerStatus(EN_COMBO_POWER_STATUS ucComboPowerStatus)
{
    if((ucComboPowerStatus == COMBO_IP_POWER_DC_ON) || (ucComboPowerStatus == COMBO_IP_POWER_PS_ON))
    {
        stComboIPParameter.bComboPowerIgnore = TRUE;
    }

    mapi_mhl_SetPowerStatus((BYTE)ucComboPowerStatus);
     mapi_DP_SetPowerStatus((BYTE)ucComboPowerStatus);
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPConfiguration()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPConfiguration(Bool bDisplayEnable, EN_COMBO_IP_SELECT ucComboSelect, EN_INPUTPORT_TYPE ucComboType)
{
    BYTE ucComboMask = mapi_combo_GetComboMask(ucComboSelect);

    if(!stComboIPParameter.bConfigFinish)
    {
        if(ucComboType &TYPE_ANALOG)
        {
            stComboIPParameter.bVGAEnableIndex = FALSE;//TRUE;
        }

        if(ucComboType &TYPE_DVI_SINGLE)
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucDVIEnableIndex, ucComboMask);
        }

        if(ucComboType &TYPE_DVI_DUAL)
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucDualEnableIndex, ucComboMask);
        }

        if(ucComboType &TYPE_HDMI)
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucHDMIEnableIndex, ucComboMask);
        }

        if(ucComboType &TYPE_HDMI_2_0)
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucHDMI2EnableIndex, ucComboMask);
        }

        if(ucComboType &TYPE_MHL)
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucMHLEnableIndex, ucComboMask);
        }

        if(ucComboType &TYPE_DP)
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucDPEnableIndex, ucComboMask);
        }

        if(ucComboType &TYPE_HBR2_0)
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucHBR20EnableIndex, ucComboMask);
        }

        if(ucComboType &TYPE_HBR2_1)
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucHBR21EnableIndex, ucComboMask);
        }

#if(CHIP_ID == MST9U4)	
	  if((ucComboType &TYPE_DAISY_CHAIN)||(ucComboType &TYPE_DP_MST))
        {
            SET_COMBO_IP_PARAMETER(stComboIPParameter.ucDaisyEnableIndex, ucComboMask);
        }
#else
#if(COMBO_DP_DAISY_CHAIN_SUPPORT)
        stComboIPParameter.ucDaisyEnableIndex = COMBO_DP_DC_SUPPORT_INDEX;
#endif
#endif
    }

    if(bDisplayEnable)
    {
        SET_COMBO_IP_PARAMETER(stComboIPParameter.ucDisplayEnableIndex, ucComboMask);
    }
    else
    {
        CLR_COMBO_IP_PARAMETER(stComboIPParameter.ucDisplayEnableIndex, ucComboMask);
    }

    mapi_combo_SetConfiguration();

#if ENABLE_VGA_INPUT
    if(ucComboType &TYPE_ANALOG)
    {
        msADC_SetConfiguration(stComboIPParameter.ucDisplayEnableIndex);
    }
#endif

    mapi_tmds_SetConfiguration(stComboIPParameter.ucDVIEnableIndex, stComboIPParameter.ucDualEnableIndex, stComboIPParameter.ucHDMIEnableIndex, stComboIPParameter.ucHDMI2EnableIndex, stComboIPParameter.ucMHLEnableIndex, stComboIPParameter.ucDisplayEnableIndex);

#if(CHIP_ID == MST9U4)	
   mapi_DP_SetConfiguration(stComboIPParameter.ucHBR20EnableIndex, stComboIPParameter.ucDaisyEnableIndex,stComboIPParameter.ucDPEnableIndex,stComboIPParameter.ucDisplayEnableIndex);
#else
   mapi_DP_SetConfiguration(stComboIPParameter.ucHBR20EnableIndex, stComboIPParameter.ucHBR21EnableIndex,stComboIPParameter.ucDPEnableIndex,stComboIPParameter.ucDisplayEnableIndex);
#endif
   mapi_mhl_SetConfiguration(stComboIPParameter.ucMHLEnableIndex, stComboIPParameter.ucDisplayEnableIndex);
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPPMIsrHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPPMIsrHandler(void)
{
#if(COMBO_MHL_BLOCK_ENABLE)
    mapi_mhl_IsrHandler(E_INT_IRQ_PM_IRQ_OUT);
#endif

#if(COMBO_DP_BLOCK_ENABLE)
    mapi_DP_PMIsrHandler(E_INT_IRQ_PM_IRQ_OUT);
#endif
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPIsrHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPIsrHandler(InterruptNum eIntNum)
{
    UNUSED(eIntNum);

#if(COMBO_DP_BLOCK_ENABLE)
    mapi_DP_IsrHandler();
#endif

#if(COMBO_TMDS_BLOCK_ENABLE)
    mapi_tmds_ISR();
#endif

    MsOS_EnableInterrupt(E_INT_IRQ_COMB);
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPInitial()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************

void msAPI_combo_LoadEDID_Func(void)
{
        mdrv_combo_LoadEDID(tMHL_EDID_SIZE_PORT, tCOMBO_EDID_TALBE_PORT);
}

void msAPI_combo_IPInitial(void)
{
    stComboIPParameter.bConfigFinish = TRUE;

    if(!stComboIPParameter.bComboPowerIgnore)
    {
#if(COMBO_EDID_BLOCK_ENABLE)
         msAPI_combo_LoadEDID_Func();
        //mdrv_combo_LoadEDID(tMHL_EDID_SIZE_PORT, tCOMBO_EDID_TALBE_PORT);
#endif

        stComboIPParameter.bComboPowerIgnore = FALSE;
    }

    mapi_combo_PowerConfiguration();

#if(COMBO_ADC_BLOCK_ENABLE && ENABLE_VGA_INPUT)
    msADC_init();
#endif


#if(COMBO_HDCP2_FUNCTION_SUPPORT)
	/*********************************************************************
		[NOTE]: HDCP22 init code is moved to msAPI_combo_IPHandler(); 
		this is due to HDCP init process needs polling security R2 ROM code done before init process start.
	**********************************************************************/
    //mapi_combo_HDCP2Initial();
    //mapi_tmds_SetHDCP2CallBackFunction(mdrv_combo_HDCP2RxEventProc, tCOMBO_HDCP2_CERTRX_TABLE);
    //mapi_DP_SetHDCP2CallBackFunction(mdrv_combo_HDCP2RxEventProc, tCOMBO_HDCP2_CERTRX_TABLE);
#endif

    mdrv_combo_ClockEnable(mapi_combo_GetComboEnableIndex());

#if(COMBO_TMDS_BLOCK_ENABLE)
    mapi_tmds_Initial();
#endif

#if(COMBO_DP_BLOCK_ENABLE)
    mapi_DP_Initial();
#endif

#if(COMBO_MHL_BLOCK_ENABLE)
    mapi_mhl_Initial(tMHL_EDID_TALBE_PORT);
    //mapi_mhl_SetSystemControlFlag(MHL_SYSTEM_FORCE_DISPLAY_ENABLE);
    
#if(COMBO_MHL_CTS_MODE_ENABLE) //For CTS 4.3.17.2  
    mapi_mhl_SetSystemControlFlag(MHL_SYSTEM_FORCE_SEND_MSG_ENABLE);
#endif

#endif

#if(COMBO_AUDIO_BLOCK_ENABLE)
    mdrv_combo_AudioInitial();
#endif
    
#if(ENABLE_LOAD_KEY_VIA_EFUSE)
    msEread_LoadKeyViaEfuse(tCOMBO_HDCP_KEY_TABLE, tCOMBO_HDCP_BKSV, EFUSE_0);
#endif

#if(COMBO_HDCP14_BLOCK_ENABLE)
    mapi_combo_LoadHDCPKey(tCOMBO_HDCP_KEY_TABLE, tCOMBO_HDCP_BKSV);
#endif


    MsOS_AttachInterrupt(E_INT_IRQ_COMB, msAPI_combo_IPIsrHandler);
    MsOS_EnableInterrupt(E_INT_IRQ_COMB);

	#if ENABLE_HDMI_INPUT && COMBO_TMDS_USE_INTERNAL_EDID
	    U8 u8PortIndex = 0;
	    for(u8PortIndex=0;u8PortIndex<Input_Nums;u8PortIndex++)
    	{
	        msAPI_LoadEDIDtoInternalEDIDSRAM(u8PortIndex, ( FreeSyncFlag?EDID_HDMI_FreeSync: EDID_HDMI_Normal ));
	        msAPISrcSetHPD(u8PortIndex, 100);  
    	}
    #endif
#if ((!COMBO_HDCP2_FUNCTION_SUPPORT) || (COMBO_HDCP2_INITPROC_NEW_MODE))
    //pull high HPD after initial process is done
    {
        BYTE ucPortIdx = 0;
        for (ucPortIdx = 0; ucPortIdx < (BYTE)COMBO_IP_SELECT_PORT_END; ucPortIdx++ )
        {
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)(ucPortIdx), TRUE);
        }
    }
#endif
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPHandler(void)
{
#if ((COMBO_HDCP2_FUNCTION_SUPPORT) && (!COMBO_HDCP2_INITPROC_NEW_MODE))
    static Bool bHDCP22TimerClr = FALSE;
#endif

    mapi_combo_PowerConfiguration();

#if ((COMBO_HDCP2_FUNCTION_SUPPORT) && (!COMBO_HDCP2_INITPROC_NEW_MODE))

    if (bHDCP22TimerClr == FALSE)
    {
        gu16SecR2TimeoutCnt = 0;
        bHDCP22TimerClr = TRUE;
    }
    
    if (gu16SecR2TimeoutCnt <= COMBO_HDCP2_SECUR2INT_WDT)
    {
    	if(mdrv_combo_HDCP2RomCodeDone())
    	{
    	    bSecRomCodeDone = TRUE;
    		mapi_combo_HDCP2Initial();
    		mapi_tmds_SetHDCP2CallBackFunction(mdrv_combo_HDCP2RxEventProc, tCOMBO_HDCP2_CERTRX_TABLE);

            #if !(COMBO_HDCP2_DECODE_KEY)
            {
                BYTE ucPortIdx = 0;
                
                for (ucPortIdx = 0; ucPortIdx < (BYTE)COMBO_IP_SELECT_PORT_END; ucPortIdx++ ) //pull high HPD after initial process is done
                {
                    msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)(ucPortIdx), TRUE);
                }
            }
            #endif
    	}

        #if(COMBO_HDCP2_DECODE_KEY)
        if(mdrv_combo_HDCP2KeyDecodeDone() && bSecRomCodeDone)
        {
            WORD i = 0;
            BYTE ucPortIdx = 0;
            
    		for ( i = 0; i < sizeof(tCOMBO_HDCP2_CERTRX_TABLE); i++ ) //update from DRAM
    		{
    			tCOMBO_HDCP2_CERTRX_TABLE[i] = msMemReadByte((ULONG)tCOMBO_HDCP2_CERTRX_TABLE + i);
    			#if 0
    			printf("0x%02X ", tCOMBO_HDCP2_CERTRX_TABLE[i]);
    			if ((i+1) % 16 == 0)
    				printf("\r\n");
    			#endif
    		}
    		//printf("\r\n");

            bHDCP22InitProcDone = TRUE;
            for (ucPortIdx = 0; ucPortIdx < (BYTE)COMBO_IP_SELECT_PORT_END; ucPortIdx++ ) //pull high HPD after initial process is done
            {
                msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)(ucPortIdx), TRUE);
            }
            bSecuStoreDone = TRUE;
        }

		if(bSecuStoreDone == TRUE)
		    mapi_DP_SetHDCP2CallBackFunction(mdrv_combo_HDCP2RxEventProc, tCOMBO_HDCP2_CERTRX_TABLE);
		
        #endif
    }
    else
    {
        if (bHDCP22InitProcDone == FALSE)
        {
            BYTE ucPortIdx = 0;
            
            bHDCP22InitProcDone = TRUE;
            printf("Wait SECU R2 Init Process Timeout!!\r\n");
            for (ucPortIdx = 0; ucPortIdx < (BYTE)COMBO_IP_SELECT_PORT_END; ucPortIdx++ ) //pull high HPD after initial process is done
            {
                msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)(ucPortIdx), TRUE);
            }
        }
    }
#endif //#if (COMBO_HDCP2_FUNCTION_SUPPORT) && (!COMBO_HDCP2_INITPROC_NEW_MODE)

    #if (COMBO_HDCP2_FUNCTION_SUPPORT) 
    if ((bSecRomCodeDone) && (bSecuStoreDone))
    {
        mdrv_combo_HDCP2Handler();
    }
    #endif


#if(COMBO_TMDS_BLOCK_ENABLE)
    mapi_tmds_Handler();
#endif

#if(COMBO_DP_BLOCK_ENABLE)
    mapi_DP_Handler();
#endif

#if(CHIP_ID == MST9U4)
#if(COMBO_DPTX_BLOCK_ENABLE)
   mapi_DPTx_Handle();	
#endif
#endif

#if ENABLE_DP_OUTPUT
    DPTxHandle();
#endif

#if(COMBO_MHL_BLOCK_ENABLE)
    #if (COMBO_HDCP2_FUNCTION_SUPPORT)
    if (bHDCP22InitProcDone == TRUE)
    #endif
        mapi_mhl_Handler();
#endif
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPTimerIsrHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPTimerIsrHandler(void)
{
#if(COMBO_MHL_BLOCK_ENABLE)
    mapi_mhl_TimerInterrupt();
#endif

#if(COMBO_TMDS_BLOCK_ENABLE)
    mapi_tmds_TimerInterrupt();
#endif

#if(COMBO_DP_BLOCK_ENABLE)
    mapi_DP_TimerInterrupt();
#endif

#if(COMBO_HDCP2_FUNCTION_SUPPORT)
    if (gu16SecR2TimeoutCnt <= COMBO_HDCP2_SECUR2INT_WDT)
        gu16SecR2TimeoutCnt++;
#endif

}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetColorFormat()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 0 = RGB, 1 = YCbCr422, 2 = YCbCr 444
//
//**************************************************************************
ST_COMBO_COLOR_FORMAT msAPI_combo_IPGetColorFormat(EN_COMBO_IP_SELECT ucComboSelect)
{
    ST_COMBO_COLOR_FORMAT ucColorFormat = {COMBO_COLOR_FORMAT_DEFAULT, COMBO_COLOR_RANGE_DEFAULT, COMBO_COLORIMETRY_NONE, COMBO_YUV_COLORIMETRY_ITU601};
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;

        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            ucColorFormat = mapi_tmds_Get_Color_Format(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            ucColorFormat = mapi_DP_GetColorFormate(ucInputPort);
            break;

        default:

            break;
    };

    return ucColorFormat;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetColorDepth()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 0 = 6bit,1 = 8bit, 2 = 10bit, 3 = 12bit, 4 = 16bit, 5 = unknow
//
//**************************************************************************
EN_COLOR_DEPTH_TYPE msAPI_combo_IPGetColorDepth(EN_COMBO_IP_SELECT ucComboSelect)
{
    EN_COLOR_DEPTH_TYPE ucColorDepth = COMBO_COLOR_DEPTH_8BIT;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            ucColorDepth = (EN_COLOR_DEPTH_TYPE)mapi_tmds_GetColorDepthInfo(ucInputPort);
            break;

        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            ucColorDepth = (EN_COLOR_DEPTH_TYPE)mapi_tmds_GetColorDepthInfo(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            ucColorDepth = (EN_COLOR_DEPTH_TYPE)mapi_DP_GetColorDepthInfo(ucInputPort);
            break;

        default:

            break;
    };

    return ucColorDepth;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetPacketContent()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPGetPacketContent(EN_COMBO_IP_SELECT ucComboSelect, EN_PACKET_DEFINE_TYPE ucPacketType, BYTE ucPacketLength, BYTE *pPacketData)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    if(ucPacketLength > 0)
    {
        switch(ucComboIPType)
        {
            case COMBO_IP_TYPE_DVI:
            case COMBO_IP_TYPE_DUAL:

                break;

            case COMBO_IP_TYPE_HDMI:
            case COMBO_IP_TYPE_MHL:
                mapi_tmds_GetPacketContent(ucInputPort, ucPacketType, ucPacketLength, pPacketData);
                break;

            case COMBO_IP_TYPE_DP:
                if(ucPacketType == COMBO_PACKET_DP_MISC)
                {
                    mapi_DP_Get_Misc(ucInputPort , pPacketData);
                }
                else  if(ucPacketType == COMBO_PACKET_AUD_INFOFRAME)
                {
			 mapi_DP_AudioInfo(ucInputPort , pPacketData);
                }

                break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetCableDetect()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
EN_COMBO_IP_CABLE msAPI_combo_IPGetCableDetect(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);
    EN_COMBO_IP_CABLE ucComboIPCableDetect = COMBO_IP_CABLE_NONE;

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            if(mapi_tmds_DetectCablePlug(ucInputPort)) // GPIO detect
            {
                ucComboIPCableDetect = COMBO_IP_CABLE_DVI;
            }

            break;

        case COMBO_IP_TYPE_HDMI:
            if(mapi_tmds_DetectCablePlug(ucInputPort)) // GPIO detect
            {
                ucComboIPCableDetect = COMBO_IP_CABLE_HDMI;
            }

            break;

        case COMBO_IP_TYPE_MHL:
            if(mapi_mhl_GetCableDetectPort(ucInputPort))
            {
                ucComboIPCableDetect = COMBO_IP_CABLE_MHL;
            }
            else if(mapi_tmds_DetectCablePlug(ucInputPort)) // SAR detect
            {
                ucComboIPCableDetect = COMBO_IP_CABLE_HDMI;
            }

            break;

        case COMBO_IP_TYPE_DP:
            if(mapi_DP_GetCableDetectPort(ucInputPort))
            {
                ucComboIPCableDetect = COMBO_IP_CABLE_DP;
            }

            break;

        default:

            break;
    };

    return ucComboIPCableDetect;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPCheckSourceAlive()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool msAPI_combo_IPCheckSourceAlive(EN_COMBO_IP_SELECT ucComboSelect)
{
    Bool bSourceAlive = FALSE;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
        case COMBO_IP_TYPE_HDMI:
            bSourceAlive = mapi_tmds_Clock_Stable(ucInputPort);
            break;

        case COMBO_IP_TYPE_MHL:
            if(mapi_mhl_GetCableDetectPort(ucInputPort))
            {
                bSourceAlive = mapi_mhl_GetCbusConnectPort(ucInputPort);
            }
            else if(mapi_tmds_DetectCablePlug(ucInputPort))
            {
                bSourceAlive = mapi_tmds_Clock_Stable(ucInputPort);
            }

            break;

        case COMBO_IP_TYPE_DP:
            bSourceAlive = mapi_DP_CheckSourceLock(ucInputPort);
            break;

        default:

            break;
    };

    return bSourceAlive;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPControlHPD()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPControlHPD(EN_COMBO_IP_SELECT ucComboSelect, Bool bSetHPD)
{
    Bool bDaisyChainFlag = mapi_combo_GetDaisyChainType(ucComboSelect);
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
        case COMBO_IP_TYPE_HDMI:
            mapi_tmds_Set_HPD(ucInputPort, bSetHPD);
			break;

        case COMBO_IP_TYPE_MHL:
            //if(!mapi_mhl_GetCableDetectPort(ucInputPort))
            if(mapi_mhl_GetCableDetectPortByInit(ucInputPort))
            {
                mapi_tmds_Set_HPD(ucInputPort, TRUE);
                mapi_mhl_CablePlugProcByInit(ucInputPort);
            }
            else

            {
                mapi_tmds_Set_HPD(ucInputPort, bSetHPD);
            }

            break;

        case COMBO_IP_TYPE_DP:
			if(bDaisyChainFlag) // Epona path
            {

            }
			else
			{
				mapi_DP_Set_HPD(ucInputPort, bSetHPD);
			}
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPCreateAudioPath()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPCreateAudioPath(EN_COMBO_IP_SELECT ucComboSelect)
{
    static BYTE ucCurrentAudioPath = COMBO_INPUT_SELECT_MASK;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    if(ucCurrentAudioPath != ucInputPort)
    {
        // When VGA or DVI must disable audio function
        mapi_tmds_ConfigAudioPort(ucInputPort, FALSE);
        mapi_DP_ConfigAudioPort(ucInputPort, FALSE);

        switch(ucComboIPType)
        {
            case COMBO_IP_TYPE_DVI:
            case COMBO_IP_TYPE_DUAL:

                break;

            case COMBO_IP_TYPE_HDMI:
            case COMBO_IP_TYPE_MHL:
                mapi_tmds_ConfigAudioPort(ucInputPort, TRUE); //for setting tmds driver global structure
                mdrv_combo_CreateTMDSAudioPath(ucInputPort);

                break;

            case COMBO_IP_TYPE_DP:
                mdrv_combo_CreateDPAudioPath(ucInputPort, mapi_DP_ComboConvertHBR2(ucInputPort));
                mapi_DP_ConfigAudioPort(ucInputPort, TRUE);
                
                break;

            default:

                break;
        };

        ucCurrentAudioPath = ucInputPort;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_ConfigGlobalMute()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPConfigGlobalMute(Bool bEnable)
{     
    mdrv_combo_ConfigGlobalMute(bEnable);
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPClockControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPClockControl(EN_COMBO_IP_SELECT ucComboSelect, Bool bClockHigh)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
        case COMBO_IP_TYPE_HDMI:
            mdrv_combo_ClockRtermControl(ucInputPort, bClockHigh);

            if(!bClockHigh)
            {
                mdrv_combo_DoSWReset(ucInputPort, COMBO_SW_RESET_HDCP |COMBO_SW_RESET_HDMI);
            }

            break;

        case COMBO_IP_TYPE_MHL:
            if(!mapi_mhl_GetCableDetectPort(ucInputPort))
            {
                mdrv_combo_ClockRtermControl(ucInputPort, bClockHigh);

                if(!bClockHigh)
                {
                    mdrv_combo_DoSWReset(ucInputPort, COMBO_SW_RESET_HDCP |COMBO_SW_RESET_HDMI);
                }
            }

            break;

        case COMBO_IP_TYPE_DP:

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPPowerOn()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPPowerOn(void)
{
    BYTE ucInputPort = COMBO_INPUT_SELECT_PORT0;
    EN_COMBO_IP_SELECT ucComboSelect = COMBO_IP_SELECT_PORT0;
    
    for(ucComboSelect = COMBO_IP_SELECT_PORT0; ucComboSelect < COMBO_IP_SELECT_MASK; ucComboSelect++)
    {
        ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);

        switch(mapi_combo_GetComboType(ucComboSelect))
        {
            case COMBO_IP_TYPE_DVI:
            case COMBO_IP_TYPE_DUAL:

                break;
                
            case COMBO_IP_TYPE_HDMI:
            case COMBO_IP_TYPE_MHL:
                
                break;

            case COMBO_IP_TYPE_DP:
                
                break;

            default:
                
                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPPowerDown()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPPowerDown(void)
{
    BYTE ucInputPort = COMBO_INPUT_SELECT_PORT0;
    EN_COMBO_IP_SELECT ucComboSelect = COMBO_IP_SELECT_PORT0;
    
    for(ucComboSelect = COMBO_IP_SELECT_PORT0; ucComboSelect < COMBO_IP_SELECT_MASK; ucComboSelect++)
    {
        ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);

        switch(mapi_combo_GetComboType(ucComboSelect))
        {
            case COMBO_IP_TYPE_DVI:
            case COMBO_IP_TYPE_DUAL:

                break;
                
            case COMBO_IP_TYPE_HDMI:
            case COMBO_IP_TYPE_MHL:
                mapi_mhl_PowerControl((BYTE)COMBO_IP_POWER_DC_OFF);
                break;

            case COMBO_IP_TYPE_DP:
                
                break;

            default:
                
                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPPowerSaving()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPPowerSaving(void)
{
    BYTE ucInputPort = COMBO_INPUT_SELECT_PORT0;
    EN_COMBO_IP_SELECT ucComboSelect = COMBO_IP_SELECT_PORT0;
    
    for(ucComboSelect = COMBO_IP_SELECT_PORT0; ucComboSelect < COMBO_IP_SELECT_MASK; ucComboSelect++)
    {
        ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);

        switch(mapi_combo_GetComboType(ucComboSelect))
        {
            case COMBO_IP_TYPE_DVI:
            case COMBO_IP_TYPE_DUAL:

                break;
                
            case COMBO_IP_TYPE_HDMI:
            case COMBO_IP_TYPE_MHL:
                mapi_mhl_PowerControl((BYTE)COMBO_IP_POWER_POWER_SAVING);
                break;

            case COMBO_IP_TYPE_DP:
                
                break;

            default:
                
                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetSplitFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPGetSplitFlag(EN_COMBO_IP_SELECT ucComboSelect)
{
    Bool bSplitFlag = FALSE;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            bSplitFlag = mapi_tmds_GetSplitFlag(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            bSplitFlag = mapi_DP_GetSplitFlag(ucInputPort);
            break;

        default:
            
            break;
    }

    return bSplitFlag;
}


//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetSplitFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPClrSplitFlag(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
             mapi_tmds_ClrSplitFlag(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
             mapi_DP_ClrSplitFlag(ucInputPort);
            break;

        default:
            
            break;
    }

}


//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPCheckAudioStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPCheckAudioStatus(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);
    Bool bAudPktReceived = FALSE;

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            bAudPktReceived = mapi_tmds_CheckAudioStatus(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            bAudPktReceived = mapi_DP_CheckAudioStatus(ucInputPort);
            break;

        default:
            
            break;
    }

    return bAudPktReceived;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPLoadEDIDPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPLoadEDIDPort(EN_COMBO_IP_SELECT ucComboSelect)
{
    Bool bDaisyChainFlag = mapi_combo_GetDaisyChainType(ucComboSelect);
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_MHL:
            mapi_mhl_LoadEDIDPort(ucInputPort, tMHL_EDID_TALBE_PORT);
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
        case COMBO_IP_TYPE_DP:
        case COMBO_IP_TYPE_VGA:
            if(bDaisyChainFlag) // Epona path
            {

            }
            else
            {
                mdrv_combo_LoadEDIDPort(ucInputPort, tMHL_EDID_SIZE_PORT, tCOMBO_EDID_TALBE_PORT);
            }
            
            break;

        default:
            
            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPChangeEDIDPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPChangeEDIDPort(EN_COMBO_IP_SELECT ucComboSelect, BYTE **pComboEDID)
{
    Bool bDaisyChainFlag = mapi_combo_GetDaisyChainType(ucComboSelect);
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_MHL:
            mapi_mhl_LoadEDIDPort(ucInputPort, pComboEDID);
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
        case COMBO_IP_TYPE_DP:
        case COMBO_IP_TYPE_VGA:
            if(bDaisyChainFlag) // Epona path
            {

            }
            else
            {
                mdrv_combo_LoadEDIDPort(ucInputPort, tMHL_EDID_SIZE_PORT, pComboEDID);
            }
            break;

        default:
            
            break;
    }
}


//**************************************************************************
//  [Function Name]:
//                  API_LoadEDIDtoInternalEDIDSRAM() for HDMI freesync on/off
//  [Description]   need enable COMBO_TMDS_USE_INTERNAL_EDID for EDID table switching
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_LoadEDIDtoInternalEDIDSRAM(BYTE u8input, EDID_INDEX edidtype)
{
	BYTE **pcomboEDID;
	if (edidtype ==  EDID_HDMI_FreeSync)
	{
		pcomboEDID = tCOMBO_EDID_TALBE_PORT_HDMIFreeSync;
	}
	else 
	{
		pcomboEDID = tCOMBO_EDID_TALBE_PORT;
	}
	msAPI_combo_IPChangeEDIDPort((EN_COMBO_IP_SELECT)g_InputPort[u8input].eIPMux, pcomboEDID);
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPInsertHDCP22Key()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPInsertHDCP22Key(Bool bExternalKey, BYTE *pKeyTable)
{
    Bool bDoneFlag = FALSE;
    WORD ustemp = 0;

    if(bExternalKey)
    {
        if(pKeyTable != NULL)
        {
#if(COMBO_HDCP2_FUNCTION_SUPPORT)
            if(mdrv_combo_HDCP2CheckEncodeKeyValid(&pKeyTable[32]))
            {
                for(ustemp = 0; ustemp < COMBO_HDCP2_ENCODE_KEY_SIZE; ustemp++)
                {
                    tCOMBO_HDCP2_CERTRX_TABLE[ustemp] = pKeyTable[ustemp];
                }
            }
            else // Encode key check fail
            {
                ustemp = COMBO_HDCP2_ENCODE_KEY_SIZE;

                COMBO_API_DPRINTF("** Combo HDCP22 encode key invalid!! %d", 0);
            }
            
#endif

            if(ustemp == COMBO_HDCP2_ENCODE_KEY_SIZE)
            {
                bDoneFlag = TRUE;
            }
            else // HDCP22 not enable
            {
                COMBO_API_DPRINTF("** Combo HDCP22 function not enable!! %d", 0);
            }
        }
        else // Key table is Null
        {
            COMBO_API_DPRINTF("** Combo HDCP22 key table is NULL!! %d", 0);
        }

#if(COMBO_HDCP2_FUNCTION_SUPPORT)
        tCOMBO_HDCP2_CERTRX_TABLE[COMBO_HDCP2_ENCODE_KEY_SIZE] = 1;
#endif
    }
    else
    {
#if(COMBO_HDCP2_FUNCTION_SUPPORT)
        tCOMBO_HDCP2_CERTRX_TABLE[COMBO_HDCP2_ENCODE_KEY_SIZE] = 0;
#endif
        
        bDoneFlag = TRUE;
    }
    
    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetTmdsModeInfo()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 0 =  DVI mode; 1 = HDMI mode
//
//**************************************************************************
BYTE msAPI_combo_IPGetTmdsModeInfo(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucTmdsInfo = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;

        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            ucTmdsInfo = mapi_tmds_Input_Is_HDMI(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:

            break;

        default:

            break;
    };

    return ucTmdsInfo;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetTmdsClkStable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 0 =  clk unstable; 1 = clk stable
//
//**************************************************************************
BYTE msAPI_combo_IPGetTmdsClkStable(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucTmdsClkStable = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            ucTmdsClkStable = mapi_tmds_Clock_Stable(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:

            break;
    };

    return ucTmdsClkStable;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetTmdsSplitterDone()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 0 =  splitter not done; 1 = splitter done
//
//**************************************************************************
BYTE msAPI_combo_IPGetTmdsSplitterDone(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucTmdsSplitterDone = 1;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:            
            break;
            
        case COMBO_IP_TYPE_HDMI:
            ucTmdsSplitterDone = mapi_tmds_Splitter_Done(ucInputPort);

            break;
            
        case COMBO_IP_TYPE_MHL:
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:

            break;
    };

    return ucTmdsSplitterDone;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPFixAudioClock()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPFixAudioClock(EN_TMDS_AUD_FREQ_INDEX enAudioFS, Bool bFlag)
{
    mapi_tmds_fix_audio_clock((MScombo_TMDS_AUD_FREQ_INDEX) enAudioFS, bFlag);
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetTmdsAudioFreq()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: EN_TMDS_AUD_FREQ_INDEX
//
//**************************************************************************
EN_TMDS_AUD_FREQ_INDEX msAPI_combo_IPGetTmdsAudioFreq(EN_COMBO_IP_SELECT ucComboSelect)
{
    EN_TMDS_AUD_FREQ_INDEX ucTmdsAudioFreq = COMBO_TMDS_AUD_FREQ_ERROR;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            break;       
            
        case COMBO_IP_TYPE_HDMI:                               
        case COMBO_IP_TYPE_MHL:      
            ucTmdsAudioFreq = (EN_TMDS_AUD_FREQ_INDEX)mapi_tmds_GetAudioFreq(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
	     ucTmdsAudioFreq = (EN_TMDS_AUD_FREQ_INDEX)mapi_DP_GetAudioFreq(ucInputPort);
        	
            break;

        default:

            break;
    };

    return ucTmdsAudioFreq;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetTmdsIsAudioFmtPCM()
//  [Description]:
//                 
//  [Arguments]:
//                  
//  [Return]:
//                  EN_TMDS_AUDIO_FORMAT: 
//                      COMBO_AUDIO_FMT_NON_PCM   = 0,
//                      COMBO_AUDIO_FMT_PCM       = 1,
//                      MCOMBO_AUDIO_FMT_UNKNOWN  = 2,
//
//**************************************************************************
EN_TMDS_AUDIO_FORMAT msAPI_combo_IPGetTmdsIsAudioFmtPCM(EN_COMBO_IP_SELECT ucComboSelect)
{
    EN_TMDS_AUDIO_FORMAT ucTmdsIsAudioFmtPCM = COMBO_AUDIO_FMT_UNKNOWN;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            break;            
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL: 
            ucTmdsIsAudioFmtPCM = (EN_TMDS_AUDIO_FORMAT)mapi_tmds_IsAudioFmtPCM(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
        	ucTmdsIsAudioFmtPCM = (EN_TMDS_AUDIO_FORMAT)mapi_DP_IsAudioFmtPCM(ucInputPort);
            break;

        default:

            break;
    };

    return ucTmdsIsAudioFmtPCM;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetPixelRepetitionInfo()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
EN_TMDS_PIXEL_REPETITION msAPI_combo_IPGetPixelRepetitionInfo(EN_COMBO_IP_SELECT ucComboSelect)
{
    EN_TMDS_PIXEL_REPETITION enPixelRepInfo = COMBO_TMDS_RESERVED_PIX_REP;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            enPixelRepInfo = (EN_TMDS_PIXEL_REPETITION)mapi_tmds_GetPixelRepetitionInfo(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:

            break;

        default:

            break;
    };

    return enPixelRepInfo;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetTmdsClockRate()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD msAPI_combo_IPGetTmdsClockRate(EN_COMBO_IP_SELECT ucComboSelect)
{
    WORD ucTmdsClockRate = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            ucTmdsClockRate = mapi_tmds_GetClockRate(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:

            break;
    };

      return ucTmdsClockRate;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPSetTmdsMonochromeMode()
//  [Description]:
//                 
//  [Arguments]:
//                  
//  [Return]:  None
//
//**************************************************************************
void msAPI_combo_IPSetTmdsMonochromeMode(EN_COMBO_IP_SELECT ucComboSelect, Bool bEnable, Bool bFlag, EN_TMDS_MONO_BIT ucBits)
{    
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:      
            mapi_tmds_SetMonochromeMode(ucInputPort, bEnable, bFlag, (MSCombo_TMDS_MONO_BIT)ucBits);
            break;
            
        case COMBO_IP_TYPE_HDMI:            
            break;
            
        case COMBO_IP_TYPE_MHL:            
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:

            break;
    };
   
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPSetTmdsDVIDeepColorSetting()
//  [Description]:
//                 
//  [Arguments]:
//                  
//  [Return]:  None
//
//**************************************************************************
void msAPI_combo_IPSetTmdsDVIDeepColorSetting(EN_COMBO_IP_SELECT ucComboSelect, Bool bEnable, EN_TMDS_DVI_DEEP_COLOR ucBits)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
            mapi_tmds_DVIDeepColorSetting(ucInputPort, bEnable, (MSCombo_TMDS_DVI_DEEP_COLOR)ucBits);
            break;            
            
        case COMBO_IP_TYPE_DUAL:           
            break;
            
        case COMBO_IP_TYPE_HDMI:            
            break;
            
        case COMBO_IP_TYPE_MHL:            
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:

            break;
    };
   
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPCheckAVMuteINT()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL msAPI_combo_IPCheckAVMuteINT(EN_COMBO_IP_SELECT ucComboSelect, Bool bClr)
{
    BOOL IsAVMute = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            break;

        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            IsAVMute = mapi_tmds_CheckAVMuteINT(ucInputPort, bClr);
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:
            break;
    };

    return IsAVMute;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetScalingInfo()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
BYTE msAPI_combo_IPGetScalingInfo(EN_COMBO_IP_SELECT ucComboSelect)
{    
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);    
    BYTE ucScalingInfo = 0;

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            ucScalingInfo = mapi_tmds_GetScaling_Info(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:
            
            break;
    }

    return ucScalingInfo;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetITContent()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPGetITContent(EN_COMBO_IP_SELECT ucComboSelect)
{    
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);    
    Bool bITContent = 0;

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            bITContent = mapi_tmds_GetITContent(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:
            
            break;
    }

    return bITContent;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetTmdsIsDVIDualLink()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPGetTmdsIsDVIDualLink(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);
    Bool bIsDualLink = FALSE;
    
    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
            break;
            
        case COMBO_IP_TYPE_DUAL:
            bIsDualLink = mapi_tmds_IsDVIDualLink(ucInputPort);
            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:         
        case COMBO_IP_TYPE_DP:
            break;

        default:
            
            break;
    }

    return bIsDualLink;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetTMDSAudioOutputState()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
EN_TMDS_AUD_STATUS msAPI_combo_IPGetTMDSAudioOutputState(EN_COMBO_IP_SELECT ucComboSelect)
{    
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);    
    EN_TMDS_AUD_STATUS ucAudioOutputState = COMBO_TMDS_AUDIO_STATE_OFF;

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            ucAudioOutputState = (EN_TMDS_AUD_STATUS)mapi_tmds_GetAudioOutputState(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:
            
            break;
    }

    return ucAudioOutputState;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetHDMISourceVersion()
//  [Description]:
//                 
//  [Arguments]:
//                  
//  [Return]:  None
//
//**************************************************************************
EN_HDMI_VERSION_TYPE msAPI_combo_IPGetHDMISourceVersion(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);
    EN_HDMI_VERSION_TYPE enHDMISourceVersion = COMBO_HDMI_VERSION_NOT_SURE;

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
            
            break;            
            
        case COMBO_IP_TYPE_DUAL: 
            
            break;
            
        case COMBO_IP_TYPE_HDMI:   
            enHDMISourceVersion = (EN_HDMI_VERSION_TYPE)mapi_tmds_GetSourceVersionType(ucInputPort);
            break;
            
        case COMBO_IP_TYPE_MHL:  
            
            break;

        case COMBO_IP_TYPE_DP:
            
            break;

        default:

            break;
    };

    return enHDMISourceVersion;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPForceColorFormat()
//  [Description]:
//
//  [Arguments]:
//                  enInputPort : combo port
//                  bFlag: true (Force color format); false (no force color format)
//                  enColor: color format
//  [Return]:  None
//
//**************************************************************************
void msAPI_combo_IPForceColorFormat(EN_COMBO_IP_SELECT ucComboSelect, Bool bFlag, EN_COLOR_FORMAT_TYPE enColor)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
            break;

        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            mapi_tmds_ForceColorFormat(ucInputPort, bFlag, enColor);
            break;

        case COMBO_IP_TYPE_DP:
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetCbusMSGKeyEvent()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
BYTE msAPI_combo_IPGetCbusMSGKeyEvent(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);    
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);
    BYTE ucKeyEvent = MHL_MSC_MSG_KEY_EVENT_NONE;

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            ucKeyEvent = mapi_mhl_GetMSGKeyEvent(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:
            
            break;

        default:
            
            break;
    };

    return ucKeyEvent;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPCbusSendRCPCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPCbusSendRCPCommand(EN_COMBO_IP_SELECT ucComboSelect, BYTE ucKeyCode)
{
    Bool bSendFinish = FALSE;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);    
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            bSendFinish = mapi_mhl_SendRCPCommand(ucInputPort, ucKeyCode);
            break;

        case COMBO_IP_TYPE_DP:
            
            break;

        default:
            
            break;
    };

    return bSendFinish;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IP_ControlMHLHPD()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IP_ControlMHLHPD(EN_COMBO_POWER_STATUS ucComboPowerStatus)
{
    BYTE ucInputPort = COMBO_INPUT_SELECT_PORT0;
    EN_COMBO_IP_SELECT ucComboSelect = COMBO_IP_SELECT_PORT0;

    if ((ucComboPowerStatus == COMBO_IP_POWER_DC_ON) || (ucComboPowerStatus == COMBO_IP_POWER_PS_ON))
    {
        for(ucComboSelect = COMBO_IP_SELECT_PORT0; ucComboSelect < COMBO_IP_SELECT_MASK; ucComboSelect++)
        {
            ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);

            switch(ucInputPort)
            {
            #if (MHL_FUNCTION_SUPPORT_PORT0)
                case COMBO_INPUT_SELECT_PORT0:
                    if(mapi_mhl_GetCableDetectPortByInit(ucInputPort))
                    {
                    }
                    else
                    {
					#if ENABLE_DP_ShortHPD_PM	 
					#if  !DP_Port_D0                          
					Init_hwHDCP_Hpd_Pin();	        
					#endif
					#else
					Init_hwHDCP_Hpd_Pin();
					#endif
                    }
                    break;
            #endif

            #if (MHL_FUNCTION_SUPPORT_PORT1)            
                case COMBO_INPUT_SELECT_PORT1:
                    if(mapi_mhl_GetCableDetectPortByInit(ucInputPort))
                    {
                    }
                    else
                    {
                        #if (ENABLE_DAISY_CHAIN == 0x1)&&((MS_BOARD_TYPE_SEL == BD_MST192A_A01A_S)\
	                ||(MS_BOARD_TYPE_SEL == BD_MST192A_A01C_S)||(MS_BOARD_TYPE_SEL == BD_MST203A_A01A_S)||(MS_BOARD_TYPE_SEL == BD_MST203B_A01A_S)||(MS_BOARD_TYPE_SEL == BD_MST203C_A01A_S))   

                        #else
                            #if ENABLE_DP_ShortHPD_PM
                        	#if  !DP_Port_D1                        	      
                            Init_hwHDCP_Hpd_Pin1();
                        #endif
			       #else
				Init_hwHDCP_Hpd_Pin1();
                            #endif   
                            
                        #endif
                     }
                    break;
            #endif

            #if (MHL_FUNCTION_SUPPORT_PORT2)            
                case COMBO_INPUT_SELECT_PORT2:
                    if(mapi_mhl_GetCableDetectPortByInit(ucInputPort))
                    {
                    }
                    else
                    {
                        Init_hwHDCP_Hpd_Pin2();
                     }
                    break;
            #endif  

            #if (MHL_FUNCTION_SUPPORT_PORT3)            
                case COMBO_INPUT_SELECT_PORT3:
                    if(mapi_mhl_GetCableDetectPortByInit(ucInputPort))
                    {
                    }
                    else
                    {
                        Init_hwHDCP_Hpd_Pin3();
                    }
                    break;
            #endif  
            
            #if (MHL_FUNCTION_SUPPORT_PORT4)                
                case COMBO_INPUT_SELECT_PORT4:
                    if(mapi_mhl_GetCableDetectPortByInit(ucInputPort))
                    {
                    }
                    else
                    {
                        Init_hwHDCP_Hpd_Pin4();
                    }
                    break;
            #endif        
            
            #if (MHL_FUNCTION_SUPPORT_PORT5)            
                case COMBO_INPUT_SELECT_PORT5:
                    if(mapi_mhl_GetCableDetectPortByInit(ucInputPort))
                    {
                    }
                    else
                    {
                        Init_hwHDCP_Hpd_Pin5();
                    }
                    break;
            #endif        
            }
        }
    }
    else//COMBO_IP_POWER_AC_ON
    {
        Init_hwHDCP_Hpd_Pin();
        Init_hwHDCP_Hpd_Pin1();
        Init_hwHDCP_Hpd_Pin2();
        Init_hwHDCP_Hpd_Pin3();
        Init_hwHDCP_Hpd_Pin4();
        Init_hwHDCP_Hpd_Pin5(); 
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPSetDPGuardBand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPSetDPGuardBand(EN_COMBO_IP_SELECT ucComboSelect, WORD usGuardBandValue)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:
            mapi_DP_SetGuardBand(ucInputPort, usGuardBandValue);
            break;

        default:
            
            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetDPHVInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPGetDPHVInformation(EN_COMBO_IP_SELECT ucComboSelect, WORD *usHTotalValue, WORD *usVTotalValue)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:
            mapi_DP_GetHVInformation(ucInputPort, usHTotalValue, usVTotalValue);
            break;

        default:
            
            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetDPInterlaceFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPGetDPInterlaceFlag(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucDPVBIDValue = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:
            ucDPVBIDValue = mapi_DP_GetInterlaceFlag(ucInputPort);
            break;

        default:
            
            break;
    }

    return ucDPVBIDValue;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetDPAstorPTGInterlacePatch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPGetDPAstorPTGInterlacePatch(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucDPVBIDValue = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:
            ucDPVBIDValue = mapi_DP_GetAstorPTGInterlacePatch(ucInputPort);
            break;

        default:
            
            break;
    }

    return ucDPVBIDValue;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetDPAstorPTGInterlaceFieldPatch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPGetDPAstorPTGInterlaceFieldPatch(EN_COMBO_IP_SELECT ucComboSelect)
{
    BYTE ucDPVBIDValue = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:
            ucDPVBIDValue = mapi_DP_GetAstorPTGInterlaceFieldPatch(ucInputPort);
            break;

        default:
            
            break;
    }

    return ucDPVBIDValue;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPDPCheckIgnoreDPMS()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPDPCheckIgnoreDPMS(EN_COMBO_IP_SELECT ucComboSelect)
{ 
    Bool bDPMSFlag = FALSE;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:
            bDPMSFlag = mapi_DP_CheckIgnoreDPMS(ucInputPort);
            break;

        default:
            
            break;
    }

    return bDPMSFlag;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPSetDPVersion()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPSetDPVersion(EN_COMBO_IP_SELECT ucComboSelect, EN_DP_VERSION ucVersion)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:
            mapi_DP_Set_Version(ucInputPort, ucVersion);
            break;

        default:
            
            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetDPDRRFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPGetDPDRRFlag(EN_COMBO_IP_SELECT ucComboSelect)
{
    Bool ucDPDRREnable = FALSE;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);
    BYTE ucSPDcontent[6] = {0};
		
    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
               
            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
                mapi_tmds_GetPacketContent(ucInputPort, COMBO_PACKET_SPD_INFOFRAME, 0x06, ucSPDcontent); 
				if((ucSPDcontent[0] == 0x1A) && (ucSPDcontent[1] == 0x00) && (ucSPDcontent[2] == 0x00))
				{
                    if(ucSPDcontent[5] & 0x01)
                        ucDPDRREnable = TRUE;
                    else
                        ucDPDRREnable = FALSE;
                }
            break;

        case COMBO_IP_TYPE_DP:		
                ucDPDRREnable = mapi_DP_GetDRRFlag(ucInputPort);
            break;

        default:
            
            break;
    }

    return ucDPDRREnable;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPSetDPDRRFunction()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_IPSetDPDRRFunction(EN_COMBO_IP_SELECT ucComboSelect, Bool bEnable)
{
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:
			    mapi_DP_EnableDRRFunction(ucInputPort, bEnable);
            break;

        default:
            
            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetDPPixelClock()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//                  DP Pixel clock = xxx MHz
//**************************************************************************
WORD msAPI_combo_IPGetDPPixelClock(EN_COMBO_IP_SELECT ucComboSelect)
{
    WORD uwDPPixelClock = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:		
                uwDPPixelClock = mapi_DP_GetPixelClock(ucInputPort);
            break;

        default:
            
            break;
    }

    return uwDPPixelClock;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetDPPixelClock_10KHz()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//                  DP Pixel clock = XXX x 10KHz, not MHz
//**************************************************************************
WORD msAPI_combo_IPGetDPPixelClock_10KHz(EN_COMBO_IP_SELECT ucComboSelect)
{
    WORD uwDPPixelClock = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:		
                uwDPPixelClock = mapi_DP_GetPixelClock_10KHz(ucInputPort);
            break;

        default:
            
            break;
    }

    return uwDPPixelClock;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPGetPixelClock_10KHz()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//                  Pixel clock = XXX x 10KHz, not MHz
//**************************************************************************
WORD msAPI_combo_IPGetPixelClock_10KHz(EN_COMBO_IP_SELECT ucComboSelect)
{
    WORD uwPixelClock = 0;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
			    uwPixelClock = mapi_tmds_GetPixelClock_10KHz(ucInputPort);
            break;

        case COMBO_IP_TYPE_DP:		
                uwPixelClock = mapi_DP_GetPixelClock_10KHz(ucInputPort);
            break;

        default:
            
            break;
    }

    return uwPixelClock;
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPDPCheckMSAChg()
//  [Description]
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
Bool msAPI_combo_IPDPCheckMSAChg(EN_COMBO_IP_SELECT ucComboSelect)
{ 
    Bool bMSAChgFlag = FALSE;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;

        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:

            break;

        case COMBO_IP_TYPE_DP:
            bMSAChgFlag = mapi_DP_GetMSAChgFlag(ucInputPort);
            break;

        default:

            break;
    }

    return bMSAChgFlag;
}
//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_IPDPClrMSAChg()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msAPI_combo_IPDPClrMSAChg(EN_COMBO_IP_SELECT ucComboSelect)
{

    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;

        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:

            break;

        case COMBO_IP_TYPE_DP:
           		mapi_DP_ClrMSAChgFlag(ucInputPort);
            break;

        default:

            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  msAPI_combo_CheckDPLock()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool msAPI_combo_CheckDPLock(EN_COMBO_IP_SELECT ucComboSelect)
{
    Bool blockFlag = FALSE;
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect);
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;

        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:

            break;

        case COMBO_IP_TYPE_DP:
           	blockFlag = mapi_DP_CheckDPLock(ucInputPort);
            break;

        default:

            break;
    }

    return blockFlag;
}
//
//  [Arguments]:
//
//  [Return]: 
//
//**************************************************************************
void msAPI_combo_PowerControl(EN_COMBO_IP_SELECT ucComboSelect, Bool  Enable)
{ 
    
    BYTE ucInputPort = mapi_combo_GetComboInputPort(ucComboSelect); 
    EN_COMBO_IP_TYPE ucComboIPType = mapi_combo_GetComboType(ucComboSelect);

    switch(ucComboIPType)
    {
        case COMBO_IP_TYPE_DVI:
        case COMBO_IP_TYPE_DUAL:

            break;
            
        case COMBO_IP_TYPE_HDMI:
        case COMBO_IP_TYPE_MHL:
            
            break;

        case COMBO_IP_TYPE_DP:       	  
         	  mapi_DP_PowerOnOff(ucInputPort,  Enable);
            break;

        default:
            
            break;
    }
  
}
#endif

