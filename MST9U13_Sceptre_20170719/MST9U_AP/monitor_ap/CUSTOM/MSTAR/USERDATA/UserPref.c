#define _USERPREF_C_
#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "NVRam.h"
#include "MDebug.h"
#include "Keypaddef.h"
#include "Common.h"
#include "menudef.h"
//#include "UserPref.h"
#include "msflash.h"
#include "misc.h"
#include "ms_reg.h"
#include "ColorFinetune_Default.h"
#include "appWindow.h"
#include "DEFAULT_DP_MHL_EDID_TABLE.h"

#define USERPREF_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&USERPREF_DEBUG
    #define USERPREF_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
    #define USERPREF_PRINT(format, ...)
#endif

Bool CheckBoard(void);
void CheckModeSettingRange(void);
void CheckMonitorSettingRange(void);
extern BYTE tEDID_TABLE_COMBO_PORT0[256];
extern BYTE tEDID_TABLE_COMBO_PORT1[256];
extern BYTE tEDID_TABLE_COMBO_PORT2[256];
extern BYTE tEDID_TABLE_COMBO_PORT3[256];
extern BYTE tEDID_TABLE_COMBO_PORT4[256];
extern BYTE tEDID_TABLE_COMBO_PORT5[256];
extern BYTE Customer_DP1_EDID_FreeSync60[256];
extern BYTE tCOMBO_USER_HDCP22_CERTRX_TABLE[1008];
extern BYTE tCOMBO_HDCP_BKSV[COMBO_HDCP_BKSV_SIZE];
extern BYTE tCOMBO_HDCP_KEY_TABLE[COMBO_HDCP_KEY_SIZE];



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void EDID_ReLoad_Func(BYTE Reload)
{
    WORD u16Index;
    u16Index=0;


    for(u16Index=0; u16Index<0x100; u16Index++)
    {
#if 0//ENABLE_USER_PREF_EDID // richard EDID
        tEDID_TABLE_COMBO_PORT0[u16Index]  = USER_DP_EDID(u16Index);
        tEDID_TABLE_COMBO_PORT1[u16Index] = USER_mDP_EDID(u16Index);
        tEDID_TABLE_COMBO_PORT2[u16Index] = USER_MHL_EDID(u16Index);
        tEDID_TABLE_COMBO_PORT3[u16Index] = USER_MHL_EDID(u16Index);
#else
#warning "Please remember set  ENABLE_USER_PREF_EDID=1!"  

#if 0//(Enable_Menu_Panel_Rotate_180)
if(USER_PREF_ROTATE_INDEX==Rotate_90||USER_PREF_ROTATE_INDEX==Rotate_270)
     tEDID_TABLE_COMBO_PORT4[u16Index]  = Customer_DP1_2160x3840_EDID[u16Index];
else
#endif	
{
 #if DP_PBP_RELOAD_EDID
     if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)//||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT)))
     	{
    #if (!Disable_4K60HZ_Timing)
	if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT)))
	{
          tEDID_TABLE_COMBO_PORT4[u16Index]  = Customer_DP1_1920x1080_EDID[u16Index];
	   Set_DP_EDID_1920x1080_Flag();	 
	   Clr_DP_EDID_1920x2160_Flag();
	}
	else if((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_DISPLAYPORT||USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_DISPLAYPORT))
	{
          tEDID_TABLE_COMBO_PORT4[u16Index]  = Customer_DP1_1920x1080_EDID[u16Index];
	   Set_DP_EDID_1920x1080_Flag();	 
	   Clr_DP_EDID_1920x2160_Flag();
	}
	else
   #endif		
	{
           tEDID_TABLE_COMBO_PORT4[u16Index]  = Customer_DP1_1920x2160_EDID[u16Index];
	    Clr_DP_EDID_1920x1080_Flag();	   
	    Set_DP_EDID_1920x2160_Flag();
	}
		
	 Clr_DP_EDID_3840x2160_Flag();	
     	}
	else
#endif
     	{
   #if (Free_Sync_Enable &&((defined(FengYuan_Project))&&(EDID_3840x2160||EDID_2560x1080_120)))//||(Free_Sync_Enable&&(defined(LianHeChuangXin_Project))&&(PanelType == PanelLTM340YP01)))
     	if(FreeSyncFlag)
        tEDID_TABLE_COMBO_PORT4[u16Index]  = Customer_DP1_EDID_FreeSync60[u16Index];
	else		
   #endif		
        tEDID_TABLE_COMBO_PORT4[u16Index]  = Customer_DP1_EDID[u16Index];
	 Set_DP_EDID_3840x2160_Flag();		
	 Clr_DP_EDID_1920x1080_Flag();	  
	 Clr_DP_EDID_1920x2160_Flag();
     	}
   }

     if(Reload)
     	{
        tEDID_TABLE_COMBO_PORT0[u16Index] = Customer_MHL_EDID[u16Index];
        tEDID_TABLE_COMBO_PORT2[u16Index] = Customer_MHL_EDID[u16Index];
    	}
#endif

    }
}

#if !ENABLE_LOAD_KEY_VIA_EFUSE
void HDCP_Read(void)
{
    WORD u16Index;
    u16Index=0;	
    ReadNVRAMBlock(ADDR_HDCP_BLOCK, HDCP_KEY_ROMSIZE);
   //msLoadHDCPKey();
#if ENABLE_USER_PREF_HDCP
    for(u16Index=0; u16Index<HDCP_KEY_SIZE1; u16Index++)
    {
        tCOMBO_HDCP_BKSV[u16Index]  = USER_HDCP_DATA(u16Index);
    }
    for(u16Index=0; u16Index<HDCP_KEY_SIZE2; u16Index++)
    {
        //printData("USER_HDCP_DATA(HDCP_KEY_SIZE1+u16Index)==%x",USER_HDCP_DATA(HDCP_KEY_SIZE1+u16Index));
		
        tCOMBO_HDCP_KEY_TABLE[u16Index]  = USER_HDCP_DATA(HDCP_KEY_SIZE1+u16Index);
    }
#else
#warning "Please remember set  ENABLE_USER_PREF_HDCP=1!"	
#endif
}
#endif


#if ENABLE_HDCP22
BYTE tCOMBO_USER_HDCP22_CERTRX_TABLE[COMBO_HDCP2_ENCODE_KEY_SIZE] =
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
};


void HDCP2_Read(Bool ucCtl)
{
    WORD u16Index;
    u16Index=0;
    #if 1//!USER_PREF_HDCP_USEFLASH	
    ReadNVRAMBlock(ADDR_HDCP22_BLOCK, HDCP22_KEY_ROMSIZE);
   #endif	
#if 0//ENABLE_USER_PREF_HDCP
    for(u16Index=0; u16Index<HDCP22_KEY_ROMSIZE; u16Index++)
    {
    #if 0//USER_PREF_HDCP_USEFLASH
        tCOMBO_USER_HDCP22_CERTRX_TABLE[u16Index]  = FlashReadByte(FDATA_HDCP22_ADDR+u16Index);
    #else
        tCOMBO_USER_HDCP22_CERTRX_TABLE[u16Index]  = USER_HDCP22_DATA(u16Index);	
    #endif	
    }
#else
#warning "Please remember set  ENABLE_USER_PREF_HDCP=1!"	
#endif

    if(ucCtl)
    		msAPI_combo_IPInsertHDCP22Key(TRUE, tCOMBO_USER_HDCP22_CERTRX_TABLE);
}

#endif


/****************************************************************
*                                                               *
*   For initialize NVRAM MAP data                               *
*                                                               *
****************************************************************/

/////////////////////////////////////////////////////////////////
//Factory Block
/////////////////////////////////////////////////////////////////

void FactoryBlockInit(void)
{
	USER_PREF_EEPROM_CHKSUM			= BOARD_CHECKSUM;

	USER_PREF_SRGB_DEFAULT			= DEF_SRGB_BRIGHTNESS_ADJ;
	USER_PREF_ADC_PGA_GAIN			= DEF_PGA_GAIN;

	USER_PREF_ADC_RED_GAIN			= DEF_RED_GAIN;
	USER_PREF_ADC_GREEN_GAIN		= DEF_GREEN_GAIN;
	USER_PREF_ADC_BLUE_GAIN			= DEF_BLUE_GAIN;

	USER_PREF_ADC_RED_OFFSET		= DEF_RED_OFFSET;
	USER_PREF_ADC_GREEN_OFFSET		= DEF_GREEN_OFFSET;
	USER_PREF_ADC_BLUE_OFFSET		= DEF_BLUE_OFFSET;

	USER_PREF_RED_COLOR_SRGB		= DEF_RED_COLOR_SRGB;
	USER_PREF_GREEN_COLOR_SRGB		= DEF_GREEN_COLOR_SRGB;
	USER_PREF_BLUE_COLOR_SRGB		= DEF_BLUE_COLOR_SRGB;
	USER_PREF_BRIGHTNESS_SRGB		= DEF_BRIGHTNESS_COLOR_SRGB;
	USER_PREF_CONTRAST_SRGB			= DEF_CONTRAST_COLOR_SRGB;

	USER_PREF_RED_COLOR_5000K		= DEF_RED_COLOR_5000K;
	USER_PREF_GREEN_COLOR_5000K		= DEF_GREEN_COLOR_5000K;
	USER_PREF_BLUE_COLOR_5000K		= DEF_BLUE_COLOR_5000K;
	USER_PREF_BRIGHTNESS_5000K		= DEF_BRIGHTNESS_COLOR_5000K;
	USER_PREF_CONTRAST_5000K		= DEF_CONTRAST_COLOR_5000K;

	USER_PREF_RED_COLOR_5700K		= DEF_RED_COLOR_5700K;
	USER_PREF_GREEN_COLOR_5700K		= DEF_GREEN_COLOR_5700K;
	USER_PREF_BLUE_COLOR_5700K		= DEF_BLUE_COLOR_5700K;
	USER_PREF_BRIGHTNESS_5700K		= DEF_BRIGHTNESS_COLOR_5700K;
	USER_PREF_CONTRAST_5700K		= DEF_CONTRAST_COLOR_5700K;

	USER_PREF_RED_COLOR_6500K		= DEF_RED_COLOR_6500K;
	USER_PREF_GREEN_COLOR_6500K		= DEF_GREEN_COLOR_6500K;
	USER_PREF_BLUE_COLOR_6500K		= DEF_BLUE_COLOR_6500K;
	USER_PREF_BRIGHTNESS_6500K		= DEF_BRIGHTNESS_COLOR_6500K;
	USER_PREF_CONTRAST_6500K		= DEF_CONTRAST_COLOR_6500K;

	USER_PREF_RED_COLOR_7500K		= DEF_RED_COLOR_7500K;
	USER_PREF_GREEN_COLOR_7500K		= DEF_GREEN_COLOR_7500K;
	USER_PREF_BLUE_COLOR_7500K		= DEF_BLUE_COLOR_7500K;
	USER_PREF_BRIGHTNESS_7500K		= DEF_BRIGHTNESS_COLOR_7500K;
	USER_PREF_CONTRAST_7500K		= DEF_CONTRAST_COLOR_7500K;

	USER_PREF_RED_COLOR_9300K 		= DEF_RED_COLOR_9300K;
	USER_PREF_GREEN_COLOR_9300K		= DEF_GREEN_COLOR_9300K;
	USER_PREF_BLUE_COLOR_9300K		= DEF_BLUE_COLOR_9300K;
	USER_PREF_BRIGHTNESS_9300K		= DEF_BRIGHTNESS_COLOR_9300K;
	USER_PREF_CONTRAST_9300K		= DEF_CONTRAST_COLOR_9300K;

	USER_PREF_RED_COLOR_10000K		= DEF_RED_COLOR_10000K;
	USER_PREF_GREEN_COLOR_10000K	= DEF_GREEN_COLOR_10000K;
	USER_PREF_BLUE_COLOR_10000K		= DEF_BLUE_COLOR_10000K;
	USER_PREF_BRIGHTNESS_10000K		= DEF_BRIGHTNESS_COLOR_10000K;
	USER_PREF_CONTRAST_10000K		= DEF_CONTRAST_COLOR_10000K;

	USER_PREF_DEF_SUBBRIGHTNESS		= INIT_FACTORY_DEF_BRIGHTNESS;
	USER_PREF_DEF_SUBCONTRAST		= INIT_FACTORY_DEF_CONTRAST;

       USER_PREF_SSC_FREQUENCY  =  PANEL_SSC_MODULATION_DEF;
       USER_PREF_SSC_PERMILLAGE_ADJUST  =  PANEL_SSC_PERCENTAGE_DEF;
	
       USER_PREF_DDR_SSC_FREQ_MODULATION  =  DDR_SSC_MODULATION_DEF;
       USER_PREF_DDR_SSC_PERMILLAGE_ADJUST  =  DDR_SSC_PERCENTAGE_DEF;

	   
    USER_PREF_FAC_PADDING0		    = 0xFF;
    USER_PREF_FAC_PADDING1		    = 0xFF;
    USER_PREF_FAC_PADDING2	        = 0xFF;
}

void FactoryBlockRead(void)
{
    ReadNVRAMBlock(ADDR_FACTORY_BLOCK, CNT_FACTORY_BLOCK);
}
void FactoryBlockSave(void)
{
    SaveNVRAMBlock(ADDR_FACTORY_BLOCK, CNT_FACTORY_BLOCK);
}

/////////////////////////////////////////////////////////////////
//System Block
/////////////////////////////////////////////////////////////////
void SystemBlockInit(void)
{
    BYTE i;


    for (i=0;i<WIND_CNT;i++)
    {
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU		
        USER_PREF_MULTI_WIN_SIZE(i)           = DEF_PIP_SIZE;
        USER_PREF_MULTI_WIN_H_POS(i)          = 0;
        USER_PREF_MULTI_WIN_V_POS(i)          = 0;
        USER_PREF_MULTI_WIN_ROTATE(i)         = 0;
#endif		
        USER_PREF_MULTI_WINDOW_OVERSCAN(i)    = 0x00;
    }
	
    for (i=0; i<PORT_CNT; i++)
    {
        USER_PREF_WIDE_MODE(i)                = DEF_WIDE_MODE;
    }

	
#if (defined(KTC_Project))
    MONITOR_FLAGS                             = DDCCI_ENABLE_BIT|POWER_ON_BIT|BURNIN_MODE_BIT;
#elif (defined(HongTian_Project))
    #if (PanelType==PanelM238DCJ_E50)
      MONITOR_FLAGS                             = DDCCI_ENABLE_BIT|POWER_ON_BIT|OSD_LOGO_BIT|PIP_MAIN_AUDIO_BIT;
    #else
      MONITOR_FLAGS                             = DDCCI_ENABLE_BIT|POWER_ON_BIT|OSD_LOGO_BIT|PIP_MAIN_AUDIO_BIT|BURNIN_MODE_BIT|FACTORY_MODE_BIT;
    #endif
#elif ((defined(LianHeChuangXin_Project))&&(!AIO_Board)||(defined(RunFeng_Project)))
   #if(defined(Gaems_Project)||defined(GK3B_Project))
      MONITOR_FLAGS                             = DDCCI_ENABLE_BIT|POWER_ON_BIT|OSD_LOGO_BIT|PIP_MAIN_AUDIO_BIT;
   #else
      MONITOR_FLAGS                             = DDCCI_ENABLE_BIT|POWER_ON_BIT|OSD_LOGO_BIT|PIP_MAIN_AUDIO_BIT|BURNIN_MODE_BIT|FACTORY_MODE_BIT;
   #endif
#elif (defined(Vage34A1_1H1DP_Project))
    MONITOR_FLAGS                             = DDCCI_ENABLE_BIT|POWER_ON_BIT|OSD_LOGO_BIT|PIP_MAIN_AUDIO_BIT|bFreeSyncBit;
#elif (defined(GK3B_Project))
      MONITOR_FLAGS                             = DDCCI_ENABLE_BIT|POWER_ON_BIT|OSD_LOGO_BIT|PIP_MAIN_AUDIO_BIT|BURNIN_MODE_BIT|FACTORY_MODE_BIT;
#else
      MONITOR_FLAGS                             = DDCCI_ENABLE_BIT|POWER_ON_BIT|OSD_LOGO_BIT|PIP_MAIN_AUDIO_BIT|BURNIN_MODE_BIT|FACTORY_MODE_BIT;
#endif

#if HDMI_HPD_Enable
    SET_MENU_HDMI_HPD_FLAG();
#endif

#if 0//LED_HOT_Menu_Func
    CLR_LED_FUNC_ON_FLAG();
#endif


    MISC_FLAGS                                = 0;
#if (MS_BOARD_TYPE_SEL == BD_MST9UHL_A0)
    DDCCI_FUN_FLAGS                           = bLoadVGAEdidBit|bLoadDigital1EdidBit|bLoadDigital2EdidBit;
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHL_V3)	
    DDCCI_FUN_FLAGS                           = bLoadVGAEdidBit|bLoadDigital1EdidBit|bLoadDigital2EdidBit|bLoadDigital3EdidBit;
#elif (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7)	
  #if AIO_Board
    DDCCI_FUN_FLAGS                           = bLoadDigital2EdidBit;
  #else
    DDCCI_FUN_FLAGS                           = bLoadDigital1EdidBit|bLoadDigital2EdidBit;
  #endif
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP)
    DDCCI_FUN_FLAGS                           = bLoadDigital2EdidBit;
#elif (MS_BOARD_TYPE_SEL == BD_MST9UHD_BV1||MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1)
    DDCCI_FUN_FLAGS                           = bLoadDigital1EdidBit|bLoadDigital2EdidBit|bLoadDigital3EdidBit;
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
    DDCCI_FUN_FLAGS                           = bLoadDigital2EdidBit|bLoadDigital3EdidBit;
#else
    DDCCI_FUN_FLAGS                           = 0;
#endif	


    USER_PREF_CTRL_HPD                        = 0;
    USER_PREF_INPUT_PRIORITY                  = DEF_INPUT_PRIORITY;
    USER_PREF_DLC_MODE                        = DEF_DLC_MODE;
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
    USER_PREF_MULTI_WIN_MODE                  = DEF_MULTIWINW_MODE;

    USER_PREF_MULTI_WIN_BOARDER_COLOR         = 0x00;
    USER_PREF_PIP_POSITION	                = DEF_PIP_POSITION;
#endif	
    USER_PREF_WIN_SEL                         = DEF_MULTIWIN_SEL;
    USER_PREF_WIN_SRC(DISPLAY_WIN1)                      = DEF_INPUT_TYPE;
    USER_PREF_WIN_SRC(DISPLAY_WIN2)                      = DEF_SUBWIN_2_INPUT;
    USER_PREF_WIN_SRC(DISPLAY_WIN3)                      = DEF_SUBWIN_3_INPUT;
    USER_PREF_WIN_SRC(DISPLAY_WIN4)                      = DEF_SUBWIN_4_INPUT;
    USER_PREF_LANGUAGE                        = DEF_LANGUAGE;
    USER_PREF_TRANSPARENCY                    = DEF_TRANSPARENCY;
    USER_PREF_DPConfigType                    = DEF_DPTYPE;
#if HDMI_VerSion_Enable	
    USER_PREF_HDMIConfigType                    = DEF_HDMITYPE;
#endif	
#if Enable_Menu_Panel_Rotate_180
    USER_PREF_ROTATE_INDEX                    = Rotate_Normal;
#endif

    USER_PREF_BRIGHTNESS               = DEF_BRIGHTNESS;
    USER_PREF_BRIGHTNESS_USER               = DEF_BRIGHTNESS;

    USER_PREF_OSD_ROTATION                    = Osd_Rotation_Normal;

    USER_PREF_rgbBRIGHTNESS                           = DEF_rgbBRIGHTNESS;

    USER_PREF_DDCCI                           = DEF_DDCCI_SETTING;
    USER_PREF_OSD_H_POSITION                  = DEF_OSD_H_POSITION;
    USER_PREF_OSD_V_POSITION                  = DEF_OSD_V_POSITION;
    USER_PREF_OSD_TIMEOUT                     = DEF_OSD_TIMEOUT;
    USER_PREF_OSD_LOCK                        = DEF_OSD_LOCK;
    USER_PREF_OSD_PIVOT                       = DEF_MENU_ROTATION;

    USER_PREF_SLEEPMODE                      =  DEF_SLEEPMODE;
#if AudioFunc
    USER_PREF_VOLUME                          = DEF_VOLUME;
    USER_PREF_AUDIO_SRC(0)                       = DEF_AUDIOSOURCE_0;
    USER_PREF_AUDIO_SRC(1)                       = DEF_AUDIOSOURCE_1;
    USER_PREF_AUDIO_SRC(2)                       = DEF_AUDIOSOURCE_2;
    USER_PREF_AUDIO_SRC(3)                       = DEF_AUDIOSOURCE_3;
    USER_PREF_AUDIO_SRC(4)                       = DEF_AUDIOSOURCE_4;
    USER_PREF_AUDIO_SRC(5)                       = DEF_AUDIOSOURCE_5;
    USER_PREF_AUDIO_SRC(6)                       = AIDIOSOURCE_LINE_IN;
    USER_PREF_AUDIO_SRC(7)                       = AIDIOSOURCE_LINE_IN;
	
    //USER_PREF_AUDIO_MUTE                      = _OFF;
    USER_PREF_AUDIO_CONFIG                    = 0x00;
    USER_PREF_AUDIO_CHANNEL_OUT               = 0x00;
#if ENABLE_ADJ_AUDIO_OUTPUT	
  #if ((defined(KTC_Project)&&PanelType==PanelLSM315HP01)||(defined(ForZhouYang315HP_Project)))
    USER_PREF_AUDIO_OUTPUTTYPE               = EarPhone;
  #else
    USER_PREF_AUDIO_OUTPUTTYPE               = Speaker;
  #endif
#endif	
#endif

    USER_PREF_LOSMODE                        =Cross_Mode_1;
    USER_PREF_LEDColorful_MODE           =DEF_LED_MODE;
    USER_PREF_LedStrength           =DEF_LED_STRENGTH;
    USER_PREF_LedModeType           =DEF_LED_MODE_TYPE;
    USER_PREF_LEDMODE_INDEX    =  DEF_LED_MODE_INDEX_TYPE;
		
    USER_PREF_LEDMODE              = DEF_LEDMODE;

    USER_PREF_HOT_PICTURE_INDEX       = CT_GAME;
	
    USER_PREF_OSD_TIME                        = DEF_OSD_TIMEOUT;
    USER_PREF_RESPONSE_TIME                   = DEF_RESPONSE_TIME;
    USER_PREF_BACKLIGHT_TIME                  = 0;
    USER_PREF_DEMURA_MEASURE_COMMAND          = 0x00;
    USER_PREF_PRESET_MODE_EEPROM_INDEX        = DEF_PRESET_MODE_INDEX;
    USER_PREF_USER_MODE_EEPROM_INDEX          = DEF_USER_MODE_INDEX;
    USER_PREF_SYS_PADDING0		              = 0xFF;

	
}

void SystemBlockRead(void)
{
    ReadNVRAMBlock(ADDR_SYSTEM_BLOCK, CNT_SYSTEM_BLOCK);
}

void SystemBlockSave(void)
{
#if 1//(defined(LianHeChuangXin_Project))
{
BYTE i;
    for (i=0; i<PORT_CNT; i++)
    {
        USER_PREF_WIDE_MODE(i)   = USER_PREF_WIDE_MODE(SrcInputPortM);
    }
}
#endif
    SaveNVRAMBlock(ADDR_SYSTEM_BLOCK, CNT_SYSTEM_BLOCK);
}

/////////////////////////////////////////////////////////////////
//Color Parameter Block
/////////////////////////////////////////////////////////////////
void ColorParameterBlockInit(void)
{
    BYTE i,j;
    BYTE USER_PREF_WIN_SRC_BK[WIND_CNT];

    for (i=0; i<WIND_CNT; i++)
    {
        USER_PREF_WIN_SRC_BK[i] = USER_PREF_WIN_SRC(i);
    }

    for(i=0;i<PORT_CNT;i++)
    {
        for (j=0; j<WIND_CNT; j++)
        {
            USER_PREF_WIN_SRC(j) = i;

            USER_PREF_CONTRAST(j)            = DEF_CONTRAST;
            USER_PREF_CONTRAST_USER(j)            = DEF_CONTRAST;
            USER_PREF_INPUT_COLOR_FORMAT(j)  = DEF_INPUT_COLOR_FORMAT;
            USER_PREF_INPUT_COLOR_RANGE(j)  = OSD_INPUT_COLOR_RANGE_DEF;
            USER_PREF_GAMMA(j)               = DEF_GAMMA;

            USER_PREF_SATURATION(j)          = DEF_VIDEO_SATURATION;
            USER_PREF_HUE(j)                 = DEF_VIDEO_HUE;
			
            USER_PREF_COLOR_TEMP(j)          = DEF_COLOR_TEMP;
            USER_PREF_PICTURE_MODE(j)          = DEF_PICTURE_MODE;
			
            USER_PREF_SHARPNESS(j)           = DEF_SHARPNESS;
 
            USER_PREF_LOWBLUELIGHT(j)      = DEF_LOWBLUELIGHT;  
				
            USER_PREF_RED_COLOR(j)           = DEF_R_COLOR_VALUE;
            USER_PREF_GREEN_COLOR(j)         = DEF_G_COLOR_VALUE;
            USER_PREF_BLUE_COLOR(j)          = DEF_B_COLOR_VALUE;
            USER_PREF_R_HUE(j)               = DEF_R_HUE;
            USER_PREF_G_HUE(j)               = DEF_G_HUE;
            USER_PREF_B_HUE(j)               = DEF_B_HUE;
            USER_PREF_C_HUE(j)               = DEF_C_HUE;
            USER_PREF_M_HUE(j)               = DEF_M_HUE;
            USER_PREF_Y_HUE(j)               = DEF_Y_HUE;
            USER_PREF_R_SATURATION(j)        = DEF_R_SATURATION;
            USER_PREF_G_SATURATION(j)        = DEF_G_SATURATION;
            USER_PREF_B_SATURATION(j)        = DEF_B_SATURATION;
            USER_PREF_C_SATURATION(j)        = DEF_C_SATURATION;
            USER_PREF_M_SATURATION(j)        = DEF_M_SATURATION;
            USER_PREF_Y_SATURATION(j)        = DEF_Y_SATURATION;

            USER_PREF_Color_R(j)        = DEF_COLOR_R;
            USER_PREF_Color_G(j)        = DEF_COLOR_G;
            USER_PREF_Color_B(j)        = DEF_COLOR_B;
            USER_PREF_Color_C(j)        = DEF_COLOR_C;
            USER_PREF_Color_M(j)        = DEF_COLOR_M;
            USER_PREF_Color_Y(j)        = DEF_COLOR_Y;
			
#if ADJ_BackLight_Enable
            USER_PREF_RED_BLACK_LEVEL(j)     = USER_PREF_rgbBRIGHTNESS;
            USER_PREF_GREEN_BLACK_LEVEL(j)   = USER_PREF_rgbBRIGHTNESS;
            USER_PREF_BLUE_BLACK_LEVEL(j)    = USER_PREF_rgbBRIGHTNESS;
#else
            USER_PREF_RED_BLACK_LEVEL(j)     = DEF_RED_BLACK_LEVEL;
            USER_PREF_GREEN_BLACK_LEVEL(j)   = DEF_GREEN_BLACK_LEVEL;
            USER_PREF_BLUE_BLACK_LEVEL(j)    = DEF_BLUE_BLACK_LEVEL;
#endif

            USER_PREF_SUPER_RESOLUTION(j)    = DEF_SUPER_RESOLUTION;
            USER_PREF_NOISE_REDUCTION(j)     = DEF_NOISE_REDUCTION;
        }
    }

    for (i=0; i<WIND_CNT; i++)
    {
        USER_PREF_WIN_SRC(i) = USER_PREF_WIN_SRC_BK[i];
    }
    UserPrefContrast_Temp=DEF_CONTRAST;
}
void ColorParameterblockRead(void)
{
    ReadNVRAMBlock(ADDR_COLOR_BLOCK, CNT_COLOR_BLOCK);
}
void ColorParameterBlockSave(void)
{
#if 1//(defined(LianHeChuangXin_Project))
{
    BYTE i,j;
    BYTE USER_PREF_WIN_SRC_BK[WIND_CNT];

    for (i=0; i<WIND_CNT; i++)
    {
        USER_PREF_WIN_SRC_BK[i] = USER_PREF_WIN_SRC(i);
    }

   for(i=0;i<PORT_CNT;i++)
    {
        for (j=0; j<WIND_CNT; j++)
        {
            USER_PREF_WIN_SRC(j) = i;
            USER_PREF_CONTRAST(j)            = UserPrefContrast_Temp;
        }
     }
    for (i=0; i<WIND_CNT; i++)
    {
        USER_PREF_WIN_SRC(i) = USER_PREF_WIN_SRC_BK[i];
    }
}
#endif	
    SaveNVRAMBlock(ADDR_COLOR_BLOCK, CNT_COLOR_BLOCK);
printMsg("ColorParameterBlockSave");
}

/////////////////////////////////////////////////////////////////
//Mode Block
/////////////////////////////////////////////////////////////////
#if ENABLE_VGA_INPUT
BYTE code tDefaultModeBlockData[]=
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,//Reserve
};
void ModeBlockInit(void)
{
    WORD u16Counter = 0;
    BYTE u8Offset = 0;

    for(u8Offset = 0; u8Offset < CNT_MODE_SETS; u8Offset++)
    {
        for(u16Counter = 0; u16Counter< SIZEOF_ST_EEP_MODE; u16Counter++)
        {
            u8NVRAM_MAP[ADDR_MODE_BLOCK + (SIZEOF_ST_EEP_MODE * u8Offset) + u16Counter] = u8NVRAM_MAP_BK[ADDR_MODE_BLOCK + SIZEOF_ST_EEP_MODE + u16Counter] = tDefaultModeBlockData[u16Counter];
        }
    }
}
void ModeBlockRead(void)
{
    ReadNVRAMBlock(EEP_MODE_BASE_ADDR( g_u8SaveIndex ), SIZEOF_ST_EEP_MODE);
}
void ModeBlockSave(void)
{
    SaveNVRAMBlock(EEP_MODE_BASE_ADDR( g_u8SaveIndex ), SIZEOF_ST_EEP_MODE);
}
#endif

/////////////////////////////////////////////////////////////////
void NvramMapDataInit(void)
{
    FactoryBlockInit();
    SystemBlockInit();
    ColorParameterBlockInit();
#if ENABLE_VGA_INPUT
    ModeBlockInit();
#endif
}

#warning "Add Board Type if use audio driver IC is 5707 ,Please open this define!"
#if Audio_Driver_Use_5707_Need_To_Check
void CheckI2CStatus(void)
{
    #define RETRY_TIME1	5

    BYTE xdata u8TempValue_bk, u8TempValue_bk2;
    BYTE xdata u8TempValue, u8TempValue2, I=0;

    NVRam_ReadByte(ADDR_BOARD_CHECKSUM, &u8TempValue_bk);
    NVRam_ReadByte(ADDR_BOARD_CHECKSUM2, &u8TempValue_bk2);

    NVRam_WriteByte(ADDR_BOARD_CHECKSUM, BOARD_CHECKSUM);
    NVRam_WriteByte(ADDR_BOARD_CHECKSUM2, BOARD_CHECKSUM2);

    NVRam_ReadByte(ADDR_BOARD_CHECKSUM, &u8TempValue);
    NVRam_ReadByte(ADDR_BOARD_CHECKSUM2, &u8TempValue2);
	
   if ((u8TempValue != BOARD_CHECKSUM)||(BOARD_CHECKSUM2!=u8TempValue2))
   {
      printData("u8TempValue==%x",u8TempValue);
      printData("u8TempValue2==%x",u8TempValue2);
	  
	for (I=0; I<RETRY_TIME1; I++)
	{
             NVRam_ReadByte(ADDR_BOARD_CHECKSUM, &u8TempValue);
             NVRam_ReadByte(ADDR_BOARD_CHECKSUM2, &u8TempValue2);
		if ((u8TempValue != BOARD_CHECKSUM)||(BOARD_CHECKSUM2!=u8TempValue2))
			continue;
		break;
	}
   }
   
     if(I>=RETRY_TIME1)// Eeprom error
     {
	hw_SetAudioReset();		
	ForceDelay1ms(200);
	hw_ClrAudioReset();		
	ForceDelay1ms(50);
	printMsg("Check Eeprom error");
     }
     else//Pass
     {
       NVRam_WriteByte(ADDR_BOARD_CHECKSUM, u8TempValue_bk);
       NVRam_WriteByte(ADDR_BOARD_CHECKSUM2, u8TempValue_bk2);
	printMsg("Check Eeprom Pass");
     }
	 
#undef RETRY_TIME1
	
}
#endif


Bool CheckBoard(void)
{
    BYTE u8TempValue2;

    ReadNVRAMBlock(ADDR_BOARD_CHECKSUM, sizeof(USER_PREF_EEPROM_CHKSUM));
    NVRam_ReadByte(ADDR_BOARD_CHECKSUM2, &u8TempValue2);
	
    if ((USER_PREF_EEPROM_CHKSUM != BOARD_CHECKSUM)||(BOARD_CHECKSUM2!=u8TempValue2))
        return FALSE;
    return TRUE;
}
void CheckMonitorSettingRange(void)
{
    BYTE i;
    BYTE u8OrgInputSrc=USER_PREF_INPUT_TYPE;
    BYTE u8OrgInputSrc1=USER_PREF_WIN_SRC(DISPLAY_WIN1);
    BYTE u8OrgInputSrc2=USER_PREF_WIN_SRC(DISPLAY_WIN2);
    BYTE u8OrgInputSrc3=USER_PREF_WIN_SRC(DISPLAY_WIN3);
    BYTE u8OrgInputSrc4=USER_PREF_WIN_SRC(DISPLAY_WIN4);

    for(i=0; i<DISPLAY_MAX_NUMS; i++)
    {
        for(USER_PREF_WIN_SRC(i)=0; USER_PREF_WIN_SRC(i)<Input_Nums; USER_PREF_WIN_SRC(i)++)
        {
            //Contrast
            if (USER_PREF_CONTRAST(i) > MAX_CONTRAST_VALUE)
            {
                USER_PREF_CONTRAST(i) = DEF_CONTRAST;
            }
            if (USER_PREF_CONTRAST_USER(i) > MAX_CONTRAST_VALUE)
            {
                USER_PREF_CONTRAST_USER(i) = DEF_CONTRAST;
            }
			
            //Input Color format
            if (USER_PREF_INPUT_COLOR_FORMAT(i) >= INPUT_COLOR_MAX)
            {
                USER_PREF_INPUT_COLOR_FORMAT(i)  = DEF_INPUT_COLOR_FORMAT;
            }
            //Gamma
            if (USER_PREF_GAMMA(i) >= eGAMMA_MAX)
            {
                USER_PREF_GAMMA(i)  = DEF_GAMMA;
            }
            //Video Saturation
            if (USER_PREF_SATURATION(i) > MAX_VIDEO_SATURATION_VALUE)
            {
                USER_PREF_SATURATION(i) = MAX_VIDEO_SATURATION_VALUE;
            }
            //Video Hue
            if (USER_PREF_HUE(i) > MAX_VIDEO_HUE_VALUE)
            {
                USER_PREF_HUE(i) = MAX_VIDEO_HUE_VALUE;
            }
            //Color Temp
            if (USER_PREF_COLOR_TEMP(i) >= CT_NUMS)
            {
                USER_PREF_COLOR_TEMP(i)  = DEF_COLOR_TEMP;
            }
            //Color Temp
            if (USER_PREF_PICTURE_MODE(i) >= CT_NUMS)
            {
                USER_PREF_PICTURE_MODE(i)  = DEF_PICTURE_MODE;
            }

			
            //Sharpness
            USER_PREF_SHARPNESS(i) %= (MAX_SHARPNESS+1);
            //LOWBLUELIGHT
         #if Enable_Lowbluray_FY
            USER_PREF_LOWBLUELIGHT(i) %= (LOW_BLUE_LIGHT_MAX+1);
	  #else	 
            USER_PREF_LOWBLUELIGHT(i) %= (MaxLowblurayValue+1);
         #endif
			
            //RGB Color Gain
            if (USER_PREF_RED_COLOR(i) > MAX_USER_R_COLOR_VALUE)
            {
                USER_PREF_RED_COLOR(i) = DEF_R_COLOR_VALUE;
            }
            if (USER_PREF_GREEN_COLOR(i) > MAX_USER_G_COLOR_VALUE)
            {
                USER_PREF_GREEN_COLOR(i) = DEF_G_COLOR_VALUE;
            }
            if (USER_PREF_BLUE_COLOR(i) > MAX_USER_B_COLOR_VALUE)
            {
                USER_PREF_BLUE_COLOR(i) = DEF_B_COLOR_VALUE;
            }
			
            //RGB Black Level
            if (USER_PREF_RED_BLACK_LEVEL(i) > MAX_BLACK_LEVEL_VALUE)
            {
                USER_PREF_RED_BLACK_LEVEL(i) = MAX_BLACK_LEVEL_VALUE;
            }
            if (USER_PREF_GREEN_BLACK_LEVEL(i) > MAX_BLACK_LEVEL_VALUE)
            {
                USER_PREF_GREEN_BLACK_LEVEL(i)= MAX_BLACK_LEVEL_VALUE;
            }
            if (USER_PREF_BLUE_BLACK_LEVEL(i) > MAX_BLACK_LEVEL_VALUE)
            {
                USER_PREF_BLUE_BLACK_LEVEL(i) = MAX_BLACK_LEVEL_VALUE;
            }
            //IHC
            if (USER_PREF_R_HUE(i) > MAX_CUSTOM_HUE_VALUE)
            {
                USER_PREF_R_HUE(i) = MAX_CUSTOM_HUE_VALUE;
            }
            if (USER_PREF_G_HUE(i) > MAX_CUSTOM_HUE_VALUE)
            {
                USER_PREF_G_HUE(i) = MAX_CUSTOM_HUE_VALUE;
            }
            if (USER_PREF_B_HUE(i) > MAX_CUSTOM_HUE_VALUE)
            {
                USER_PREF_B_HUE(i) = MAX_CUSTOM_HUE_VALUE;
            }
            if (USER_PREF_C_HUE(i) > MAX_CUSTOM_HUE_VALUE)
            {
                USER_PREF_C_HUE(i) = MAX_CUSTOM_HUE_VALUE;
            }
            if (USER_PREF_M_HUE(i) > MAX_CUSTOM_HUE_VALUE)
            {
                USER_PREF_M_HUE(i) = MAX_CUSTOM_HUE_VALUE;
            }
            if (USER_PREF_Y_HUE(i) > MAX_CUSTOM_HUE_VALUE)
            {
                USER_PREF_Y_HUE(i) = MAX_CUSTOM_HUE_VALUE;
            }
            //Custom Saturation
            if (USER_PREF_R_SATURATION(i) > MAX_CUSTOM_SATURATION_VALUE)
            {
                USER_PREF_R_SATURATION(i) = MAX_CUSTOM_SATURATION_VALUE;
            }
            if (USER_PREF_G_SATURATION(i) > MAX_CUSTOM_SATURATION_VALUE)
            {
                USER_PREF_G_SATURATION(i) = MAX_CUSTOM_SATURATION_VALUE;
            }
            if (USER_PREF_B_SATURATION(i) > MAX_CUSTOM_SATURATION_VALUE)
            {
                USER_PREF_B_SATURATION(i) = MAX_CUSTOM_SATURATION_VALUE;
            }
            if (USER_PREF_C_SATURATION(i) > MAX_CUSTOM_SATURATION_VALUE)
            {
                USER_PREF_C_SATURATION(i) = MAX_CUSTOM_SATURATION_VALUE;
            }
            if (USER_PREF_M_SATURATION(i) > MAX_CUSTOM_SATURATION_VALUE)
            {
                USER_PREF_M_SATURATION(i) = MAX_CUSTOM_SATURATION_VALUE;
            }
            if (USER_PREF_Y_SATURATION(i) > MAX_CUSTOM_SATURATION_VALUE)
            {
                USER_PREF_Y_SATURATION(i) = MAX_CUSTOM_SATURATION_VALUE;
            }


            if (USER_PREF_Color_R(i) > MAX_CUSTOM_COLOR_VALUE)
            {
                USER_PREF_Color_R(i) = MAX_CUSTOM_COLOR_VALUE;
            }
            if (USER_PREF_Color_G(i) > MAX_CUSTOM_COLOR_VALUE)
            {
                USER_PREF_Color_G(i) = MAX_CUSTOM_COLOR_VALUE;
            }
            if (USER_PREF_Color_B(i) > MAX_CUSTOM_COLOR_VALUE)
            {
                USER_PREF_Color_B(i) = MAX_CUSTOM_COLOR_VALUE;
            }
            if (USER_PREF_Color_C(i) > MAX_CUSTOM_COLOR_VALUE)
            {
                USER_PREF_Color_C(i) = MAX_CUSTOM_COLOR_VALUE;
            }
            if (USER_PREF_Color_M(i) > MAX_CUSTOM_COLOR_VALUE)
            {
                USER_PREF_Color_M(i) = MAX_CUSTOM_COLOR_VALUE;
            }
            if (USER_PREF_Color_Y(i) > MAX_CUSTOM_COLOR_VALUE)
            {
                USER_PREF_Color_Y(i) = MAX_CUSTOM_COLOR_VALUE;
            }

			

        #if ENABLE_SUPER_RESOLUTION
            if (USER_PREF_SUPER_RESOLUTION(i) >= SUPER_RESOLUTION_NUMS)
            {
                USER_PREF_SUPER_RESOLUTION(i) = DEF_SUPER_RESOLUTION;
            }
        #endif
        #if ENABLE_NOISE_REDUCTION
            if (USER_PREF_NOISE_REDUCTION(i) >= NR_MAX)
            {
                USER_PREF_NOISE_REDUCTION(i) = DEF_NOISE_REDUCTION;
            }
        #endif

        }
    }

    for (USER_PREF_INPUT_TYPE=0; USER_PREF_INPUT_TYPE<PORT_CNT; USER_PREF_INPUT_TYPE++)
    {

		if (USER_PREF_WIDE_MODE(USER_PREF_INPUT_TYPE)>=eASPECT_MAX)
			USER_PREF_WIDE_MODE(USER_PREF_INPUT_TYPE)=DEF_WIDE_MODE;
    }

    u8OrgInputSrc1=u8OrgInputSrc;
    USER_PREF_BRIGHTNESS %= 101;
    USER_PREF_BRIGHTNESS_USER %= 101;
	
    USER_PREF_INPUT_TYPE=u8OrgInputSrc1;
    USER_PREF_INPUT_TYPE_SUBWIN1=u8OrgInputSrc2;
    USER_PREF_INPUT_TYPE_SUBWIN2=u8OrgInputSrc3;
    USER_PREF_INPUT_TYPE_SUBWIN3=u8OrgInputSrc4;

    for(i=0; i<DISPLAY_MAX_NUMS; i++)
    {
	  if (USER_PREF_WIN_SRC(i)>=Input_Nums)
	  {
	  	USER_PREF_WIN_SRC(i)= 0;
	  }
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	  
	  if (USER_PREF_MULTI_WIN_SIZE(i) >= ePIPSIZE_MAX)
	  {
	  	USER_PREF_MULTI_WIN_SIZE(i) = DEF_PIP_SIZE;
	  }
	  if (USER_PREF_MULTI_WIN_H_POS(i)>=PANEL_WIDTH)
	  {
	  	USER_PREF_MULTI_WIN_H_POS(i)=0;
	  }
	  if (USER_PREF_MULTI_WIN_V_POS(i)>=PANEL_HEIGHT)
	  {
	  	USER_PREF_MULTI_WIN_V_POS(i)=0;
	  }
#endif
    }

    if (USER_PREF_DLC_MODE>=MAX_DLC)
    {
		USER_PREF_DLC_MODE=DEF_DLC_MODE;
    }
    if (USER_PREF_PRESET_MODE_EEPROM_INDEX>=PRESET_MODE_NUMBER)
    {
		USER_PREF_PRESET_MODE_EEPROM_INDEX = DEF_PRESET_MODE_INDEX;
    }
    if (USER_PREF_USER_MODE_EEPROM_INDEX>=USER_MODE_NUMBER)
    {
		USER_PREF_USER_MODE_EEPROM_INDEX = DEF_USER_MODE_INDEX;
    }

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_MULTI_WIN_MODE >= eMULTIWIN_MAX)
    {
		USER_PREF_MULTI_WIN_MODE = DEF_MULTIWINW_MODE;
    }
#endif
    if (USER_PREF_WIN_SEL >= DISPLAY_MAX_NUMS)
    {
		USER_PREF_WIN_SEL=DEF_MULTIWIN_SEL;
    }

   #if AudioFunc
         if(USER_PREF_AUDIO_SRC(0) >= AIDIOSOURCE_TYPE_MAX)
         {
            USER_PREF_AUDIO_SRC(0)=DEF_AUDIOSOURCE_0;
         }
         if(USER_PREF_AUDIO_SRC(1) >= AIDIOSOURCE_TYPE_MAX)
         {
            USER_PREF_AUDIO_SRC(1)=DEF_AUDIOSOURCE_1;
         }
         if(USER_PREF_AUDIO_SRC(2) >= AIDIOSOURCE_TYPE_MAX)
         {
            USER_PREF_AUDIO_SRC(2)=DEF_AUDIOSOURCE_2;
         }
         if(USER_PREF_AUDIO_SRC(3) >= AIDIOSOURCE_TYPE_MAX)
         {
            USER_PREF_AUDIO_SRC(3)=DEF_AUDIOSOURCE_3;
         }
         if(USER_PREF_AUDIO_SRC(4) >= AIDIOSOURCE_TYPE_MAX)
         {
            USER_PREF_AUDIO_SRC(4)=DEF_AUDIOSOURCE_4;
         }
/*		 
         if(USER_PREF_AUDIO_SRC(5) >= AIDIOSOURCE_TYPE_MAX)
         {
            USER_PREF_AUDIO_SRC(5)=DEF_AUDIOSOURCE_5;
         }
*/
		 
   #endif


    if (USER_PREF_LANGUAGE >= LANG_INDEX_MAX)
        USER_PREF_LANGUAGE = DEF_LANGUAGE;
	
    if (USER_PREF_OSD_TIME < MIN_OSD_TIMEOUT || USER_PREF_OSD_TIME > MAX_OSD_TIMEOUT)
        USER_PREF_OSD_TIME = DEF_OSD_TIMEOUT;

    if (USER_PREF_SLEEPMODE >= SleepMode_Max)
        USER_PREF_SLEEPMODE = DEF_SLEEPMODE;

    USER_PREF_TRANSPARENCY %= (MAX_TRANSPARENCY+1);

    if (USER_PREF_DPConfigType >= DPconfigtype_NUMS)
        USER_PREF_DPConfigType = DEF_DPTYPE;
#if HDMI_VerSion_Enable
    if (USER_PREF_HDMIConfigType >= HDMIconfigtype_NUMS)
        USER_PREF_HDMIConfigType = DEF_HDMITYPE;
#endif
  #if Enable_Menu_Panel_Rotate_180
     USER_PREF_ROTATE_INDEX     %= RotateIndex_NUMS;
  #endif

      USER_PREF_LOSMODE                        %=Cross_Mode_Max;

      USER_PREF_LEDColorful_MODE            %=LedColor_max;
      USER_PREF_LedStrength            %=LedStrength_max;
      USER_PREF_LedModeType            %=LedMode_Max;
      USER_PREF_LEDMODE_INDEX            %=LedModeIndex_Max;
	  
     USER_PREF_OSD_ROTATION     %= Osd_RotationIndex_NUMS;

    if (USER_PREF_SRGB_DEFAULT<0x70)
        USER_PREF_SRGB_DEFAULT = DEF_SRGB_BRIGHTNESS_ADJ;
    if (USER_PREF_RED_COLOR_SRGB > MAX_COLOR_VALUE)
        USER_PREF_RED_COLOR_SRGB = MAX_COLOR_VALUE;
    if (USER_PREF_GREEN_COLOR_SRGB > MAX_COLOR_VALUE)
        USER_PREF_GREEN_COLOR_SRGB = MAX_COLOR_VALUE;
    if (USER_PREF_BLUE_COLOR_SRGB > MAX_COLOR_VALUE)
        USER_PREF_BLUE_COLOR_SRGB = MAX_COLOR_VALUE;

    if (USER_PREF_RED_COLOR_9300K > MAX_COLOR_VALUE)
        USER_PREF_RED_COLOR_9300K = MAX_COLOR_VALUE;
    if (USER_PREF_GREEN_COLOR_9300K > MAX_COLOR_VALUE)
        USER_PREF_GREEN_COLOR_9300K = MAX_COLOR_VALUE;
    if (USER_PREF_BLUE_COLOR_9300K > MAX_COLOR_VALUE)
        USER_PREF_BLUE_COLOR_9300K = MAX_COLOR_VALUE;

    if (USER_PREF_RED_COLOR_6500K > MAX_COLOR_VALUE)
        USER_PREF_RED_COLOR_6500K = MAX_COLOR_VALUE;
    if (USER_PREF_GREEN_COLOR_6500K > MAX_COLOR_VALUE)
        USER_PREF_GREEN_COLOR_6500K = MAX_COLOR_VALUE;
    if (USER_PREF_BLUE_COLOR_6500K > MAX_COLOR_VALUE)
        USER_PREF_BLUE_COLOR_6500K = MAX_COLOR_VALUE;

    if (USER_PREF_RED_COLOR_5700K > MAX_COLOR_VALUE)
        USER_PREF_RED_COLOR_5700K = MAX_COLOR_VALUE;
    if (USER_PREF_GREEN_COLOR_5700K > MAX_COLOR_VALUE)
        USER_PREF_GREEN_COLOR_5700K = MAX_COLOR_VALUE;
    if (USER_PREF_BLUE_COLOR_5700K > MAX_COLOR_VALUE)
        USER_PREF_BLUE_COLOR_5700K = MAX_COLOR_VALUE;

    if (USER_PREF_RED_COLOR_10000K > MAX_COLOR_VALUE)
        USER_PREF_RED_COLOR_10000K = MAX_COLOR_VALUE;
    if (USER_PREF_GREEN_COLOR_10000K > MAX_COLOR_VALUE)
        USER_PREF_GREEN_COLOR_10000K = MAX_COLOR_VALUE;
    if (USER_PREF_BLUE_COLOR_10000K > MAX_COLOR_VALUE)
        USER_PREF_BLUE_COLOR_10000K = MAX_COLOR_VALUE;

    if (USER_PREF_RED_COLOR_5000K > MAX_COLOR_VALUE)
        USER_PREF_RED_COLOR_5000K = MAX_COLOR_VALUE;
    if (USER_PREF_GREEN_COLOR_5000K > MAX_COLOR_VALUE)
        USER_PREF_GREEN_COLOR_5000K = MAX_COLOR_VALUE;
    if (USER_PREF_BLUE_COLOR_5000K > MAX_COLOR_VALUE)
        USER_PREF_BLUE_COLOR_5000K = MAX_COLOR_VALUE;

    if (USER_PREF_RED_COLOR_7500K > MAX_COLOR_VALUE)
        USER_PREF_RED_COLOR_7500K = MAX_COLOR_VALUE;
    if (USER_PREF_GREEN_COLOR_7500K > MAX_COLOR_VALUE)
        USER_PREF_GREEN_COLOR_7500K = MAX_COLOR_VALUE;
    if (USER_PREF_BLUE_COLOR_7500K > MAX_COLOR_VALUE)
        USER_PREF_BLUE_COLOR_7500K = MAX_COLOR_VALUE;

    if (USER_PREF_ADC_PGA_GAIN > 0x03)
        USER_PREF_ADC_PGA_GAIN = 0x00;

    if (USER_PREF_ADC_RED_GAIN > ADC_MAX_LIMIT || USER_PREF_ADC_RED_GAIN < ADC_MIN_LIMIT)
        USER_PREF_ADC_RED_GAIN=DEF_RED_GAIN;
    if (USER_PREF_ADC_GREEN_GAIN > ADC_MAX_LIMIT || USER_PREF_ADC_GREEN_GAIN < ADC_MIN_LIMIT)
        USER_PREF_ADC_GREEN_GAIN=DEF_GREEN_GAIN;
    if (USER_PREF_ADC_BLUE_GAIN > ADC_MAX_LIMIT || USER_PREF_ADC_BLUE_GAIN < ADC_MIN_LIMIT)
        USER_PREF_ADC_BLUE_GAIN=DEF_BLUE_GAIN;
    /*
    if (USER_PREF_ADC_RED_OFFSET > ADC_MAX_LIMIT || USER_PREF_ADC_RED_OFFSET < ADC_MIN_LIMIT)
        USER_PREF_ADC_RED_OFFSET=DEF_RED_OFFSET;
    if (USER_PREF_ADC_GREEN_OFFSET > ADC_MAX_LIMIT || USER_PREF_ADC_GREEN_OFFSET < ADC_MIN_LIMIT)
        USER_PREF_ADC_GREEN_OFFSET=DEF_GREEN_OFFSET;
    if (USER_PREF_ADC_BLUE_OFFSET > ADC_MAX_LIMIT || USER_PREF_ADC_BLUE_OFFSET < ADC_MIN_LIMIT)
        USER_PREF_ADC_BLUE_OFFSET=DEF_BLUE_OFFSET;
    */

    if(USER_PREF_DEF_SUBBRIGHTNESS<FACTORY_DEF_BRIGHTNESS_MIN)
        USER_PREF_DEF_SUBBRIGHTNESS=INIT_FACTORY_DEF_BRIGHTNESS;

    USER_PREF_INPUT_TYPE %= Input_Nums;
    USER_PREF_INPUT_PRIORITY %= INPUT_PRIORITY_NUMS;

#if ENABLE_ADJ_AUDIO_OUTPUT
    USER_PREF_AUDIO_OUTPUTTYPE %= OutputNums;
#endif
	


}
void SaveFactorySetting(void)
{
    FactoryBlockSave();
}
void SaveMonitorSetting(void)
{
    //FactoryBlockSave();
    SystemBlockSave();
    ColorParameterBlockSave();
}
void ReadMonitorSetting(void)
{
    BYTE u8ModeIndex_bk;
#if ENABLE_VGA_INPUT
    BYTE u8Flag_bk=SrcFlags(Input_Analog1);
#endif
    //Not diff the save data at this initialize function.
    CLR_SAVE_DIFF_ENABLE_FLAG();
    u8ModeIndex_bk=g_u8SaveIndex;

    if(CheckBoard())
    {
        USERPREF_PRINT("ReadMonitorSetting...\n");
        SystemBlockRead();
        FactoryBlockRead();
        ColorParameterblockRead();
#if ENABLE_VGA_INPUT
        SrcFlags(Input_Analog1)&=~bUserMode;
        for (g_u8SaveIndex=0; g_u8SaveIndex<PRESET_MODE_NUMBER; g_u8SaveIndex++)
            ModeBlockRead();
        
        SrcFlags(Input_Analog1)|=bUserMode;
        for (g_u8SaveIndex=0; g_u8SaveIndex<USER_MODE_NUMBER; g_u8SaveIndex++)
            ModeBlockRead();
#endif
    }
    else
    {
        FactoryBlockInit();
        FactoryBlockSave();

        SystemBlockInit();
        SystemBlockSave();

        ColorParameterBlockInit();
        ColorParameterBlockSave();
        NVRam_WriteByte(ADDR_BOARD_CHECKSUM, BOARD_CHECKSUM);
        NVRam_WriteByte(ADDR_BOARD_CHECKSUM2, BOARD_CHECKSUM2);

#if ENABLE_VGA_INPUT
        ModeBlockInit();

        SrcFlags(Input_Analog1)&=~bUserMode;
        for (g_u8SaveIndex=0; g_u8SaveIndex<PRESET_MODE_NUMBER; g_u8SaveIndex++)
            ModeBlockSave();
        
        SrcFlags(Input_Analog1)|=bUserMode;
        for (g_u8SaveIndex=0; g_u8SaveIndex<USER_MODE_NUMBER; g_u8SaveIndex++)
            ModeBlockSave();
#endif
		
    }

    g_u8SaveIndex=u8ModeIndex_bk;
#if ENABLE_VGA_INPUT	
    SrcFlags(Input_Analog1)=u8Flag_bk;
#endif

    CheckMonitorSettingRange();

    SET_SAVE_DIFF_ENABLE_FLAG();

    EDID_ReLoad_Func(1);	

    #if !ENABLE_LOAD_KEY_VIA_EFUSE
    HDCP_Read();	
   #endif
    #if (ENABLE_HDCP22 && ENABLE_USER_PREF_HDCP)
    HDCP2_Read(0);	
    #endif

	
}

#if ENABLE_VGA_INPUT
void Init_ModeSetting(void)
{
    USER_PREF_H_TOTAL(Input_Analog1)    = StandardModeHTotal(Input_Analog1);
    USER_PREF_H_START(Input_Analog1)    = StandardModeHStart(Input_Analog1);
    USER_PREF_V_START(Input_Analog1)    = StandardModeVStart(Input_Analog1);
    USER_PREF_AUTO_HSTART(Input_Analog1)= StandardModeHStart(Input_Analog1);
    USER_PREF_AUTO_VSTART(Input_Analog1)= StandardModeVStart(Input_Analog1);
    UPPHASE_CHECK_RANGE(Input_Analog1);
    USER_PREF_AUTOTIME(Input_Analog1)   = 0;
    USER_PREF_MODE_CHECKSUM(Input_Analog1)=MODE_CHECKSUM_VALUE;
    USER_PREF_MODE_INDEX(Input_Analog1)=SrcModeIndex(Input_Analog1);
}
void CheckModeSettingRange(void)        //avoid value over range
{
    if (abs(USER_PREF_H_TOTAL(Input_Analog1) - StandardModeHTotal(Input_Analog1)) > 100)
        USER_PREF_H_TOTAL(Input_Analog1) = StandardModeHTotal(Input_Analog1);

    if (USER_PREF_H_START(Input_Analog1) > 2 * StandardModeHStart(Input_Analog1))
        USER_PREF_H_START(Input_Analog1) = StandardModeHStart(Input_Analog1);

    if (USER_PREF_V_START(Input_Analog1) > 2 * StandardModeVStart(Input_Analog1))
        USER_PREF_V_START(Input_Analog1) = StandardModeVStart(Input_Analog1);

    UPPHASE_CHECK_RANGE(Input_Analog1);

    if (!(USER_PREF_AUTOTIME(Input_Analog1)&AUTO_TIMES_BIT))
    {
        USER_PREF_AUTO_HSTART(Input_Analog1)=StandardModeHStart(Input_Analog1);
        USER_PREF_AUTO_VSTART(Input_Analog1)=StandardModeVStart(Input_Analog1);
    }
}
void SaveModeSetting(void)
{
    if ((CURRENT_INPUT_IS_VGA())
#if (ENABLE_MULTI_WINDOW_SETTINGS_MENU&&PXP_VGA_Auto_Enable)		
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN2)))||((USER_PREF_INPUT_TYPE_SUBWIN3==INPUT_VGA)&&(ReadyForChangeFlag(USER_PREF_INPUT_TYPE_SUBWIN3)))))
#endif
)	
        ModeBlockSave();
}
Bool GetPrestModeSaveIndex(void)
{
    Bool NewMode = 1;

    for ( g_u8SaveIndex = 0; g_u8SaveIndex < PRESET_MODE_NUMBER; g_u8SaveIndex++ )
    {
       if ( USER_PREF_MODE_CHECKSUM(Input_Analog1) == MODE_CHECKSUM_VALUE )
       {
	       if ( USER_PREF_MODE_INDEX(Input_Analog1) == SrcModeIndex(Input_Analog1) )
           {
                NewMode=0;
                break;
           }
       }
    }

    if ( g_u8SaveIndex >= PRESET_MODE_NUMBER )
    {
        USER_PREF_PRESET_MODE_EEPROM_INDEX = ( USER_PREF_PRESET_MODE_EEPROM_INDEX + 1 ) % PRESET_MODE_NUMBER;

        g_u8SaveIndex = USER_PREF_PRESET_MODE_EEPROM_INDEX;
        SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR( u8SYS_Preset_Mode_Eeprom_Index ), sizeof(USER_PREF_PRESET_MODE_EEPROM_INDEX));
    }


    return NewMode;
}
BYTE GetUserModeSaveIndex(void)
{
    Bool NewMode = 1;

    for ( g_u8SaveIndex = 0; g_u8SaveIndex < USER_MODE_NUMBER; g_u8SaveIndex++ )
    {
        if ( USER_PREF_MODE_CHECKSUM(Input_Analog1) == MODE_CHECKSUM_VALUE )
        {
	        if ( USER_PREF_MODE_INDEX(Input_Analog1) == SrcModeIndex(Input_Analog1) )
            {
                NewMode=0;
                break;
		    }
        }
    }

    if ( g_u8SaveIndex >= USER_MODE_NUMBER )
    {
        USER_PREF_USER_MODE_EEPROM_INDEX = ( USER_PREF_USER_MODE_EEPROM_INDEX + 1 ) % USER_MODE_NUMBER;

        g_u8SaveIndex = USER_PREF_USER_MODE_EEPROM_INDEX;
        SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR( u8SYS_User_Mode_Eeprom_Index ), sizeof(USER_PREF_USER_MODE_EEPROM_INDEX));
    }


    return NewMode;
}
void ReadModeSetting(void)
{
    Bool NewMode = 1;

    if ((CURRENT_INPUT_IS_VGA())
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU		
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1)))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN2)))))
       ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==INPUT_VGA)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==INPUT_VGA)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN2)))||((USER_PREF_INPUT_TYPE_SUBWIN3==INPUT_VGA)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN3)))))
#endif
	)	
	;
    else	
       return;

    USERPREF_PRINT("ReadModeSetting\n");
    if (UserModeFlag(Input_Analog1))
    {
        NewMode = GetUserModeSaveIndex();
    }
    else
    {
        NewMode = GetPrestModeSaveIndex();
    }

    if (NewMode)
    {
        Init_ModeSetting();

        SaveModeSetting();
        USERPREF_PRINT("mode setting found fail\n");
    }
    else
    {
        USERPREF_PRINT("mode setting found\n");
    }
    CheckModeSettingRange();
}
#endif
