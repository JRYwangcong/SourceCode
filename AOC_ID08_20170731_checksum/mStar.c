#include <math.h>
#include "types.h"
#include "board.h"
#include "ms_reg.h"
#include "global.h"
#include "panel.h"
#include "adjust.h"
#include "ms_rwreg.h"
#include "debug.h"
#include "misc.h"
#include "ddc.h"
#include "power.h"
#include "detect.h"
#include "msOsd.h"
#include "gamma.h"
#include "mstar.h"
//#include "extlib.h"
#include "menu.h"
#include "menudef.h"
#include "menufunc.h"
#include "MsID_V1.h"
#if ENABLE_HDCP
    #include "MsHDCP.h"
#endif

#if ENABLE_HDMI
#include "msHDMI.h"
#include "msHDMIACE.h"
#endif

#if MWEFunction
    #include "MsDLC.h"
    #include "MsACE.h"
    #include "Userpref.h"
#endif

#if MS_OVD
#include "msovd_v03.h"
#endif

#if MS_DAC
#include "msdac.h"
#endif

#ifdef TSUMXXT		//110311 Modify
    #include <MsADC.h>  // Provides: msADC_init()

    typedef enum LPLL_INPUT_DIV_t
    {
        LPLL_INPUT_DIV_1 = 0,
        LPLL_INPUT_DIV_2 = 1,
        LPLL_INPUT_DIV_4 = 2,
        LPLL_INPUT_DIV_8 = 3,
    } LPLL_INPUT_DIV_t;

    typedef enum LPLL_OUTPUT_DIV_t
    {
      LPLL_OUTPUT_DIV_1 = 0,
      LPLL_OUTPUT_DIV_2 = 1,
      LPLL_OUTPUT_DIV_4 = 2
    } LPLL_OUTPUT_DIV_t;

    static void mStar_LPLL_GetDividers (const DWORD kHzODCLK, LPLL_INPUT_DIV_t* pInputDiv, LPLL_OUTPUT_DIV_t* pOutputDiv, BYTE* pLoopDiv);
    static void mStar_LPLL_GetFactor   (const DWORD kHzODCLK, const BYTE        loopDiv  , DWORD*             pFactor);
#endif  // end of #if( ChipID == CHIP_TSUMT )

#if PanelminiLVDS
extern void SetRSDSToTTL(BYTE on);
#endif

#ifdef PanelSSCFunction
void mStar_SetPanelSSC(void);    
#endif

#if MainBoardType==MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
void SetScalerGPIO(BYTE pinX, bit HiLow);
#endif
//*******************************************************************
//
// Constant Definition
//
//*******************************************************************
#define ADC2Sets    0//0// 1 for chip with 2 ADC(9x5x & 9x4x)
#define FixDClk     0
#define OutDClk1    108
#define OutDClk2    135
#if PanelLVDS
    //14.318 * 15 * 8 / 3.5 = 490.9, 490.9<<19=257374477.16,   490.9<<3=8=3927
    #define DClkFactor  257374477ul
    //#define DClkFactor      257650102// use 215 as base
#else
    //14.318 * 15 * 8 / 4 = 429.54, 429.54<<19=225202667.52,   429.54<<3=8=3436.32
    #define DClkFactor  225202667ul
    //#define DClkFactor      257650102// use 215 as base
#endif
#if EnablePanelServiceMenu
#define FreeRunDClk (DWORD)DClkFactor/_PanelDCLK
//#define FreeRunDClk (DWORD)DClkFactor/((PanelMinDCLK+PanelMaxDCLK)/2)
#else
#define FreeRunDClk (DWORD)DClkFactor/PanelDCLK
#endif
// 2006/8/25 11:43AM by Emily BYTE InputSclk;  //2006-07-07 Andy

#if ENABLE_HDMI
#define DIFF(a, b)                  (a>b?(a-b):(b-a))
#endif
//*******************************************************************
//
// extern declaration
//
//*******************************************************************
//*******************************************************************
//
// local declaration
//
//*******************************************************************
void mStar_SetupInputPort(void);
void mStar_SetupFreeRunMode(void);
void mStar_SetupADC(void);
Bool mStar_SetCaptureWindow(void);
#if Enable_Expansion
void mStar_SetScalingFactor(BYTE ExpansionMode);
#else
void mStar_SetScalingFactor(void);
#endif
void mStar_SetupPattern(void);
Bool mStar_SetPanelTiming(void);
void mStar_InitADC(void);
void mStar_InitTCON(void);
#if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
void mStar_SetScalingFilter(void);
#else
void mStar_SetScalingFilter(BYTE filter);
#endif
void mStar_SetUserPref(void);
void mStar_ResetDClkPLL(void);
void CheckLineBuffer(void);
#ifdef ReduceDDC
void InitVccDetectPin(void);
void GPIOPWMSelect(void);
#endif

void SetPWMFreq(WORD freq);
void UnUseFunc(void);		//110311 Modify

//*******************************************************************
//*******************************************************************
#if MS_OVD
 BYTE code tOverDrive[]=
{
#ifdef TSUMXXQX
#if (ModelName==AOC_919Vz1&&PanelType==PanelLM190E08)       //111109 Rick add OSD table for Model AOC_919Vz1
    0xD9, 0xC2, 0xEA, 0x97, 0xB1, 0x5A, 0x47, 0x6F,
    0x10, 0xD9, 0x3D, 0x37, 0x2E, 0x27, 0x26, 0x26,
    0xD9, 0xC9, 0xF3, 0xE4, 0x88, 0xB2, 0x5B, 0x40,
    0x69, 0x1C, 0x0D, 0x06, 0x34, 0x21, 0x26, 0x26,
    0xD9, 0xDF, 0xF9, 0xE3, 0x95, 0xBD, 0xA2, 0x4A,
    0x70, 0x67, 0x17, 0x02, 0x31, 0x2D, 0x26, 0x26,
    0xD9, 0xDC, 0xCC, 0xE9, 0x90, 0x87, 0xAD, 0x55,
    0x7A, 0x60, 0x12, 0x01, 0x3C, 0x2A, 0x26, 0x26,
    0xD9, 0xDC, 0xCA, 0xFA, 0x99, 0x82, 0xB6, 0x5C,
    0x45, 0x6A, 0x1D, 0x0D, 0x3A, 0x28, 0x26, 0x26,
    0xD9, 0xDA, 0xC8, 0xFA, 0xEF, 0x89, 0xB5, 0x58,
    0x4E, 0x77, 0x1A, 0x08, 0x39, 0x36, 0x26, 0x26,
    0xD9, 0xDA, 0xC9, 0xF8, 0xEA, 0x9F, 0xB9, 0xA5,
    0x4B, 0x70, 0x67, 0x16, 0x05, 0x35, 0x26, 0x26,
    0xD9, 0xDA, 0xD7, 0xF9, 0xEB, 0x9B, 0x8B, 0xA9,
    0x54, 0x79, 0x60, 0x13, 0x03, 0x33, 0x26, 0x26,
    0xD9, 0xDA, 0xD4, 0xC6, 0xF6, 0x98, 0x88, 0xBD,
    0x59, 0x43, 0x76, 0x1A, 0x00, 0x30, 0x26, 0x26,
    0xD9, 0xDB, 0xD5, 0xC3, 0xF1, 0xE1, 0x93, 0xB9,
    0xAF, 0x49, 0x7C, 0x62, 0x16, 0x3E, 0x27, 0x26,
    0xD9, 0xDB, 0xD3, 0xC1, 0xF1, 0xE1, 0x91, 0x81,
    0xB5, 0x5F, 0x79, 0x6E, 0x12, 0x3C, 0x27, 0x26,
    0xD9, 0xD8, 0xD0, 0xCF, 0xFD, 0xE9, 0xE6, 0x8A,
    0xBF, 0xA4, 0x4F, 0x69, 0x16, 0x39, 0x25, 0x26,
    0xD9, 0xD8, 0xD1, 0xCA, 0xC4, 0xF4, 0xE5, 0x95,
    0x84, 0xA9, 0x54, 0x7F, 0x19, 0x03, 0x20, 0x26,
    0xD9, 0xD9, 0xDC, 0xC9, 0xC1, 0xFA, 0xEA, 0x9A,
    0x8B, 0xBC, 0xA0, 0x44, 0x6D, 0x09, 0x37, 0x25,
    0xD9, 0xD9, 0xDD, 0xD0, 0xC9, 0xC2, 0xFC, 0xEA,
    0xE4, 0x94, 0x84, 0xA8, 0x73, 0x1F, 0x39, 0x2E,
    0xD9, 0xD9, 0xD8, 0xDF, 0xD0, 0xD6, 0xC1, 0xF8,
    0xF2, 0xEE, 0x9C, 0x8C, 0xB0, 0x64, 0x0F, 0x29,
    0xD9, 0xD9, 0xD9, 0xD9, 0xDB, 0xDC, 0xD0, 0xC9,
    0xCC, 0xFA, 0xEB, 0x9B, 0x8F, 0xA9, 0x14, 0x3D,
    0x4A, 0x61, 0x79, 0x1C, 0x31, 0xD6, 0xED, 0xF8,
    0x96, 0x98, 0xAE, 0xB5, 0xBD, 0xB8, 0xB8, 0xB8,
    0x4F, 0x5A, 0x74, 0x47, 0x19, 0x30, 0xC9, 0xE2,
    0xFC, 0x8B, 0x9D, 0xA1, 0xB5, 0xBA, 0xB8, 0xB8,
    0x44, 0x54, 0x6A, 0x04, 0x1E, 0x28, 0xC0, 0xDA,
    0xF4, 0x81, 0x92, 0xA6, 0xAA, 0xBC, 0xB8, 0xB8,
    0x45, 0x4A, 0x64, 0x7B, 0x14, 0x2F, 0x39, 0xD0,
    0xEA, 0x85, 0x96, 0x99, 0xAB, 0xBE, 0xB8, 0xB8,
    0x45, 0x4C, 0x5C, 0x75, 0x0B, 0x23, 0x3E, 0xD5,
    0xE0, 0xFC, 0x8B, 0x9C, 0xAE, 0xBF, 0xB8, 0xB8,
    0x45, 0x4D, 0x5D, 0x6A, 0x04, 0x19, 0x30, 0xCB,
    0xE5, 0xFF, 0x8E, 0x9F, 0xA0, 0xB0, 0xB8, 0xB8,
    0x45, 0x4E, 0x5F, 0x6D, 0x7A, 0x14, 0x29, 0xC1,
    0xDA, 0xF4, 0x81, 0x91, 0xA3, 0xB1, 0xB8, 0xB8,
    0x45, 0x4E, 0x51, 0x6E, 0x7D, 0x0D, 0x26, 0x39,
    0xD0, 0xEA, 0x86, 0x95, 0xA5, 0xB3, 0xB8, 0xB8,
    0x45, 0x4F, 0x53, 0x61, 0x70, 0x0E, 0x1F, 0x34,
    0xC8, 0xE3, 0xFE, 0x8A, 0xA6, 0xB4, 0xB8, 0xB8,
    0x46, 0x41, 0x55, 0x66, 0x68, 0x05, 0x12, 0x2D,
    0xC4, 0xDD, 0xF7, 0x82, 0x99, 0xB5, 0xB8, 0xB8,
    0x46, 0x41, 0x56, 0x67, 0x68, 0x07, 0x17, 0x26,
    0x3D, 0xD4, 0xEB, 0x86, 0x92, 0xB6, 0xB8, 0xB8,
    0x46, 0x42, 0x57, 0x59, 0x6E, 0x70, 0x0F, 0x1B,
    0x37, 0xCD, 0xE4, 0xF8, 0x9F, 0xA9, 0xB8, 0xB8,
    0x46, 0x43, 0x4A, 0x5F, 0x61, 0x73, 0x03, 0x13,
    0x20, 0x3C, 0xDD, 0xF4, 0x97, 0xAC, 0xB8, 0xB8,
    0x47, 0x45, 0x4C, 0x53, 0x59, 0x6C, 0x7C, 0x0C,
    0x1C, 0x28, 0xC3, 0xED, 0x84, 0x99, 0xBC, 0xB8,
    0x47, 0x46, 0x40, 0x4B, 0x51, 0x58, 0x6C, 0x70,
    0x02, 0x12, 0x21, 0x3C, 0xFD, 0x94, 0xB7, 0xBA,
    0x47, 0x47, 0x44, 0x40, 0x4C, 0x53, 0x5B, 0x62,
    0x77, 0x7A, 0x0A, 0x18, 0x3F, 0x8D, 0xA4, 0xBF,
    0x47, 0x47, 0x47, 0x46, 0x43, 0x40, 0x4B, 0x55,
    0x5C, 0x6D, 0x7D, 0x0C, 0x24, 0x38, 0x9D, 0xB4,
    0xF6, 0xE3, 0xD8, 0xB3, 0xAA, 0x80, 0x66, 0x51,
    0x4A, 0x39, 0x2A, 0x10, 0x04, 0x0D, 0x09, 0x09,
    0xF6, 0xFD, 0xD3, 0xCD, 0xB8, 0x91, 0x88, 0x60,
    0x5A, 0x37, 0x27, 0x2B, 0x1C, 0xF6, 0x09, 0x09,
    0xF6, 0xF3, 0xEC, 0xC2, 0xBF, 0x96, 0x81, 0x78,
    0x53, 0x4D, 0x3A, 0x2F, 0x10, 0x05, 0x09, 0x09,
    0xF6, 0xF3, 0xE2, 0xDF, 0xB2, 0xAA, 0x87, 0x71,
    0x69, 0x40, 0x31, 0x20, 0x12, 0x04, 0x09, 0x09,
    0xF6, 0xF2, 0xE4, 0xD5, 0xCD, 0xA2, 0x9B, 0x74,
    0x6F, 0x46, 0x35, 0x24, 0x17, 0x06, 0x09, 0x09,
    0xF6, 0xF5, 0xE6, 0xD4, 0xC2, 0xBD, 0x90, 0x89,
    0x63, 0x5D, 0x36, 0x26, 0x28, 0x1B, 0x09, 0x09,
    0xF6, 0xF5, 0xF9, 0xD6, 0xC4, 0xB2, 0xAF, 0x80,
    0x66, 0x52, 0x4D, 0x3A, 0x2D, 0x1D, 0x09, 0x09,
    0xF6, 0xF5, 0xFB, 0xE9, 0xD9, 0xB7, 0xA7, 0x9C,
    0x70, 0x6B, 0x42, 0x30, 0x2F, 0x1F, 0x09, 0x09,
    0xF6, 0xF4, 0xFA, 0xEA, 0xDD, 0xCA, 0xBB, 0x94,
    0x8D, 0x60, 0x5C, 0x49, 0x22, 0x1E, 0x08, 0x09,
    0xF6, 0xF4, 0xFD, 0xEF, 0xDE, 0xCE, 0xBC, 0xAA,
    0x86, 0x7D, 0x54, 0x4F, 0x3B, 0x10, 0x08, 0x09,
    0xF6, 0xF7, 0xFF, 0xE1, 0xD0, 0xC2, 0xB5, 0xA3,
    0x9F, 0x76, 0x6D, 0x42, 0x3B, 0x14, 0x0B, 0x09,
    0xF6, 0xF7, 0xFE, 0xE2, 0xD6, 0xD8, 0xCB, 0xB9,
    0x97, 0x80, 0x66, 0x5D, 0x30, 0x2B, 0x0C, 0x09,
    0xF6, 0xF6, 0xF0, 0xE7, 0xEC, 0xDE, 0xC1, 0xB1,
    0xA1, 0x9C, 0x75, 0x54, 0x4C, 0x23, 0x05, 0x0B,
    0xF6, 0xF6, 0xF2, 0xFA, 0xE2, 0xE9, 0xDA, 0xCD,
    0xB1, 0xAF, 0x9D, 0x71, 0x59, 0x3D, 0x11, 0x0F,
    0xF6, 0xF6, 0xF4, 0xF1, 0xFA, 0xE3, 0xE8, 0xDC,
    0xC2, 0xB4, 0xA7, 0x95, 0x7F, 0x34, 0x2D, 0x05,
    0xF6, 0xF6, 0xF6, 0xF5, 0xF3, 0xFC, 0xE6, 0xEE,
    0xD6, 0xDB, 0xCD, 0xBD, 0xA9, 0x60, 0x24, 0x1C,
    0x92, 0xB8, 0xA2, 0xCB, 0xF0, 0x1E, 0x06, 0x28,
    0x5C, 0x4F, 0x7B, 0x76, 0x6C, 0x64, 0x65, 0x65,
    0x9F, 0x82, 0xAA, 0xDF, 0xC1, 0xE8, 0x10, 0x3B,
    0x2D, 0x53, 0x4D, 0x79, 0x75, 0x66, 0x65, 0x65,
    0x98, 0x8A, 0xB2, 0xA4, 0xCC, 0xF1, 0x18, 0x9A,
    0x2A, 0x5E, 0x49, 0x45, 0x76, 0x60, 0x65, 0x65,
    0x98, 0x97, 0x85, 0xA2, 0xCA, 0xFC, 0xE1, 0x0E,
    0x30, 0x24, 0x54, 0x46, 0x70, 0x62, 0x65, 0x65,
    0x98, 0x90, 0x80, 0xB4, 0xD2, 0xFB, 0xE2, 0x15,
    0x3E, 0x23, 0x50, 0x43, 0x72, 0x6D, 0x65, 0x65,
    0x98, 0x93, 0x83, 0xB1, 0xDA, 0xC2, 0xE9, 0x13,
    0x05, 0x2F, 0x5D, 0x4D, 0x7F, 0x6C, 0x65, 0x65,
    0x98, 0x93, 0x8D, 0xB3, 0xA1, 0xD4, 0xF2, 0x18,
    0x00, 0x2A, 0x59, 0x4E, 0x79, 0x6F, 0x65, 0x65,
    0x98, 0x92, 0x8E, 0xB2, 0xAD, 0xD3, 0xC6, 0xE2,
    0x09, 0x32, 0x27, 0x55, 0x7B, 0x69, 0x65, 0x65,
    0x9B, 0x9D, 0x89, 0xB9, 0xA9, 0xDF, 0xCC, 0xF4,
    0x12, 0x05, 0x2E, 0x53, 0x45, 0x68, 0x65, 0x65,
    0x9B, 0x9C, 0x8B, 0xBA, 0xB5, 0xD8, 0xCB, 0xFF,
    0xE4, 0x02, 0x37, 0x58, 0x43, 0x6B, 0x65, 0x65,
    0x9B, 0x9F, 0x8A, 0x85, 0xB6, 0xA1, 0xD6, 0xC4,
    0xEF, 0x14, 0x32, 0x5A, 0x4D, 0x75, 0x65, 0x65,
    0x9B, 0x9E, 0x94, 0x81, 0xBD, 0xAF, 0xDC, 0xC2,
    0xF6, 0x18, 0x04, 0x22, 0x4E, 0x76, 0x65, 0x65,
    0x9A, 0x99, 0x96, 0x8C, 0xB8, 0xB5, 0xA5, 0xD5,
    0xFB, 0xEE, 0x15, 0x34, 0x52, 0x7E, 0x67, 0x65,
    0x9A, 0x9B, 0x93, 0x8A, 0x80, 0xBF, 0xA9, 0xDB,
    0xCA, 0xF8, 0xEF, 0x08, 0x24, 0x42, 0x6F, 0x64,
    0x9A, 0x9A, 0x9F, 0x93, 0x8A, 0x83, 0xB9, 0xB4,
    0xA0, 0xD3, 0xC3, 0xF7, 0x03, 0x54, 0x72, 0x60,
    0x9A, 0x9A, 0x9B, 0x9E, 0x9D, 0x97, 0x8E, 0x81,
    0xBF, 0xA9, 0xD9, 0xCF, 0xF7, 0x3E, 0x44, 0x62,
#else
    0xC0, 0xE3, 0xF5, 0x89, 0x9E, 0xB3, 0x45, 0x57,
    0x6B, 0xC0, 0x11, 0x23, 0x31, 0x3A, 0x3F, 0x3F,
    0xC0, 0xD0, 0xF3, 0x85, 0x99, 0xAC, 0xBF, 0x51,
    0x64, 0x7A, 0x0C, 0x1C, 0x2C, 0x38, 0x3F, 0x3F,
    0xC0, 0xC3, 0xE0, 0x82, 0x96, 0xA8, 0xBB, 0x4D,
    0x61, 0x76, 0x09, 0x18, 0x29, 0x37, 0x3F, 0x3F,
    0xC0, 0xC3, 0xD2, 0xF0, 0x92, 0xA6, 0xB8, 0x4B,
    0x5E, 0x71, 0x06, 0x15, 0x28, 0x34, 0x3C, 0x3F,
    0xC0, 0xC2, 0xD0, 0xE1, 0x80, 0xA3, 0xB5, 0x47,
    0x5A, 0x6E, 0x03, 0x13, 0x24, 0x33, 0x3C, 0x3F,
    0xC0, 0xC2, 0xCF, 0xDF, 0xF1, 0x90, 0xB3, 0x45,
    0x58, 0x6B, 0x00, 0x11, 0x22, 0x31, 0x3B, 0x3F,
    0xC0, 0xC1, 0xCE, 0xDD, 0xED, 0xFF, 0xA0, 0x42,
    0x56, 0x69, 0x7C, 0x0E, 0x20, 0x30, 0x3A, 0x3F,
    0xC0, 0xC1, 0xCD, 0xDC, 0xEC, 0xFD, 0x8F, 0xB0,
    0x53, 0x66, 0x79, 0x0B, 0x1E, 0x2F, 0x3A, 0x3F,
    0xC0, 0xC1, 0xCD, 0xDA, 0xEA, 0xFB, 0x8C, 0x9E,
    0x40, 0x63, 0x77, 0x08, 0x1B, 0x2C, 0x39, 0x3F,
    0xC0, 0xC0, 0xCC, 0xD9, 0xE9, 0xF8, 0x8A, 0x9B,
    0xAE, 0x50, 0x72, 0x06, 0x17, 0x2A, 0x38, 0x3F,
    0xC0, 0xC0, 0xCB, 0xD8, 0xE6, 0xF6, 0x86, 0x97,
    0xAB, 0xBE, 0x60, 0x03, 0x14, 0x27, 0x36, 0x3F,
    0xC0, 0xC0, 0xCB, 0xD6, 0xE4, 0xF3, 0x84, 0x94,
    0xA7, 0xBB, 0x4E, 0x70, 0x12, 0x25, 0x35, 0x3F,
    0xC0, 0xC0, 0xCA, 0xD6, 0xE2, 0xF1, 0x80, 0x90,
    0xA2, 0xB7, 0x49, 0x5D, 0x00, 0x21, 0x33, 0x3F,
    0xC0, 0xC0, 0xC8, 0xD4, 0xE0, 0xEE, 0xFC, 0x8C,
    0x9E, 0xB1, 0x45, 0x58, 0x6D, 0x10, 0x31, 0x3F,
    0xC0, 0xC0, 0xC6, 0xD3, 0xDE, 0xEB, 0xFA, 0x89,
    0x9A, 0xAE, 0x41, 0x52, 0x66, 0x7D, 0x20, 0x3E,
    0xC0, 0xC0, 0xC5, 0xD0, 0xDB, 0xE7, 0xF5, 0x82,
    0x92, 0xA7, 0xBA, 0x4B, 0x5F, 0x76, 0x09, 0x30,
    0xC0, 0xC0, 0xC3, 0xCC, 0xD4, 0xDE, 0xEB, 0xF6,
    0x83, 0x95, 0xA8, 0xB9, 0x4B, 0x5E, 0x75, 0x0B,
    0x54, 0x63, 0x0F, 0x1C, 0x27, 0x33, 0xC0, 0xEE,
    0xFA, 0x85, 0x95, 0xA5, 0xB9, 0xB1, 0xB0, 0xB0,
    0x4F, 0x64, 0x73, 0x4F, 0x2C, 0x39, 0xC7, 0xD4,
    0xE1, 0x8C, 0x9B, 0xAB, 0xBC, 0xB3, 0xB0, 0xB0,
    0x4F, 0x45, 0x75, 0x02, 0x10, 0x3B, 0xCB, 0xD8,
    0xE4, 0x8F, 0x9E, 0xAE, 0xBE, 0xB5, 0xB0, 0xB0,
    0x4F, 0x45, 0x55, 0x05, 0x12, 0x3F, 0xCE, 0xDB,
    0xE8, 0xF4, 0x82, 0x90, 0xBF, 0xB5, 0xB0, 0xB0,
    0x4F, 0x46, 0x57, 0x65, 0x15, 0x23, 0x31, 0xDF,
    0xEB, 0xF6, 0x85, 0x94, 0xA3, 0xB7, 0xB0, 0xB0,
    0x4F, 0x47, 0x58, 0x69, 0x75, 0x25, 0x33, 0xC1,
    0xEE, 0xF9, 0x87, 0x96, 0xA5, 0xB7, 0xB0, 0xB0,
    0x4F, 0x48, 0x59, 0x6A, 0x79, 0x06, 0x36, 0xC2,
    0xD0, 0xFC, 0x89, 0x99, 0xA7, 0xB8, 0xB0, 0xB0,
    0x4F, 0x49, 0x5B, 0x6B, 0x7A, 0x0A, 0x17, 0xC6,
    0xD3, 0xE0, 0x8C, 0x9B, 0xA8, 0xB9, 0xB0, 0xB0,
    0x4F, 0x4A, 0x5C, 0x6D, 0x7C, 0x0C, 0x1A, 0x27,
    0xD5, 0xE2, 0x8F, 0x9D, 0xAB, 0xBC, 0xB1, 0xB0,
    0x4F, 0x4B, 0x5D, 0x6E, 0x7E, 0x0F, 0x1E, 0x2A,
    0x37, 0xE6, 0xF3, 0x81, 0xAE, 0xBE, 0xB3, 0xB0,
    0x4F, 0x4B, 0x5E, 0x50, 0x61, 0x71, 0x01, 0x2F,
    0x3B, 0xC7, 0xF7, 0x83, 0x90, 0xBF, 0xB3, 0xB0,
    0x4F, 0x4B, 0x5F, 0x51, 0x63, 0x74, 0x04, 0x12,
    0x3F, 0xCB, 0xD8, 0x87, 0x93, 0xA1, 0xB4, 0xB0,
    0x4F, 0x4B, 0x41, 0x54, 0x65, 0x77, 0x07, 0x16,
    0x22, 0xCF, 0xDB, 0xE8, 0x96, 0xA4, 0xB5, 0xB0,
    0x4F, 0x4B, 0x43, 0x56, 0x68, 0x7A, 0x0B, 0x1A,
    0x26, 0x34, 0xC0, 0xEE, 0xF8, 0xA7, 0xB7, 0xB0,
    0x4F, 0x4C, 0x43, 0x58, 0x6B, 0x7D, 0x0F, 0x1E,
    0x2C, 0x37, 0xC6, 0xD2, 0xFD, 0x89, 0xB9, 0xB0,
    0x4F, 0x4D, 0x44, 0x5A, 0x6E, 0x62, 0x74, 0x06,
    0x12, 0x3F, 0xCC, 0xDB, 0xE5, 0x8F, 0x9A, 0xB0,
    0x4F, 0x4F, 0x48, 0x40, 0x55, 0x6A, 0x7F, 0x73,
    0x03, 0x11, 0x3E, 0xCD, 0xDA, 0xE4, 0x8F, 0x94,
    0xF8, 0xDB, 0xCC, 0xB0, 0xA3, 0x96, 0x7A, 0x6D,
    0x50, 0x45, 0x36, 0x18, 0x08, 0x01, 0x07, 0x07,
    0xF8, 0xFB, 0xCB, 0xBC, 0xAF, 0x91, 0x85, 0x68,
    0x5A, 0x4F, 0x32, 0x21, 0x13, 0xF8, 0x07, 0x07,
    0xF8, 0xFB, 0xEA, 0xBA, 0xAD, 0x9F, 0x82, 0x74,
    0x67, 0x4C, 0x3F, 0x2E, 0x11, 0x0D, 0x06, 0x07,
    0xF8, 0xFB, 0xE9, 0xDA, 0xAA, 0x9C, 0x8F, 0x71,
    0x64, 0x48, 0x3C, 0x2C, 0x1E, 0x0C, 0x04, 0x07,
    0xF8, 0xFA, 0xE8, 0xD8, 0xC9, 0x9A, 0x8C, 0x7E,
    0x61, 0x55, 0x39, 0x2A, 0x1B, 0x0A, 0x03, 0x07,
    0xF8, 0xF9, 0xF6, 0xE5, 0xD6, 0xB9, 0x89, 0x7C,
    0x6E, 0x52, 0x45, 0x37, 0x19, 0x09, 0x03, 0x07,
    0xF8, 0xF9, 0xF6, 0xE5, 0xD5, 0xC6, 0xAA, 0x79,
    0x6D, 0x5E, 0x43, 0x34, 0x27, 0x08, 0x02, 0x07,
    0xF8, 0xF9, 0xF5, 0xE3, 0xD3, 0xC4, 0xB5, 0x98,
    0x69, 0x5D, 0x40, 0x32, 0x24, 0x17, 0x01, 0x07,
    0xF8, 0xF8, 0xF4, 0xE2, 0xD2, 0xC2, 0xB3, 0xA5,
    0x97, 0x5A, 0x4D, 0x3F, 0x21, 0x13, 0x00, 0x07,
    0xF8, 0xF8, 0xF4, 0xE1, 0xDE, 0xCF, 0xBF, 0xA0,
    0x94, 0x87, 0x48, 0x3C, 0x2E, 0x11, 0x0F, 0x07,
    0xF8, 0xF8, 0xF3, 0xEF, 0xDD, 0xCC, 0xBD, 0xAE,
    0x90, 0x85, 0x77, 0x39, 0x2C, 0x1E, 0x0E, 0x07,
    0xF8, 0xF8, 0xF2, 0xEE, 0xDB, 0xCA, 0xBA, 0xA9,
    0x9D, 0x80, 0x74, 0x67, 0x28, 0x1B, 0x0C, 0x07,
    0xF8, 0xF8, 0xF1, 0xED, 0xD9, 0xC8, 0xC7, 0xB6,
    0x99, 0x8D, 0x7F, 0x61, 0x56, 0x18, 0x0A, 0x07,
    0xF8, 0xF8, 0xFF, 0xEB, 0xE7, 0xD4, 0xC3, 0xB1,
    0xA4, 0x88, 0x7A, 0x6E, 0x51, 0x46, 0x17, 0x07,
    0xF8, 0xF8, 0xFE, 0xEA, 0xE5, 0xD2, 0xCF, 0xBE,
    0xAF, 0x92, 0x86, 0x68, 0x5A, 0x41, 0x36, 0x05,
    0xF8, 0xF8, 0xFC, 0xF6, 0xE1, 0xDD, 0xC9, 0xC6,
    0xB6, 0x9A, 0x8C, 0x7F, 0x60, 0x49, 0x3C, 0x24,
    0x9D, 0xBE, 0xA8, 0xC4, 0xF0, 0xED, 0x1E, 0x0B,
    0x27, 0x52, 0x42, 0x72, 0x60, 0x68, 0x6A, 0x6A,
    0x95, 0x8D, 0xAF, 0xD8, 0xF5, 0xE1, 0x13, 0x0D,
    0x38, 0x54, 0x47, 0x77, 0x67, 0x6E, 0x6A, 0x6A,
    0x95, 0x9F, 0xBD, 0xDF, 0xCB, 0xE4, 0x16, 0x95,
    0x3C, 0x2B, 0x5A, 0x75, 0x64, 0x6F, 0x6A, 0x6A,
    0x95, 0x9C, 0x8C, 0xAD, 0xCE, 0xFB, 0x15, 0x06,
    0x30, 0x2F, 0x59, 0x48, 0x7B, 0x6C, 0x6A, 0x6A,
    0x95, 0x9D, 0x82, 0xBC, 0xDD, 0xFE, 0xE8, 0x1A,
    0x37, 0x22, 0x5C, 0x4F, 0x7E, 0x6D, 0x6A, 0x6A,
    0x95, 0x92, 0x83, 0xB0, 0xA2, 0xCD, 0xEE, 0x18,
    0x35, 0x21, 0x53, 0x42, 0x7C, 0x62, 0x6A, 0x6A,
    0x95, 0x92, 0x83, 0xB0, 0xA0, 0xD2, 0xFD, 0x1F,
    0x08, 0x24, 0x50, 0x40, 0x72, 0x63, 0x6A, 0x6A,
    0x95, 0x90, 0x81, 0xB6, 0xA1, 0xD0, 0xC3, 0xED,
    0x0E, 0x3B, 0x57, 0x47, 0x73, 0x61, 0x6A, 0x6A,
    0x95, 0x90, 0x86, 0xB7, 0xA7, 0xD7, 0xC6, 0xF3,
    0x1D, 0x3E, 0x2A, 0x45, 0x76, 0x67, 0x68, 0x6A,
    0x95, 0x91, 0x87, 0xB5, 0xBA, 0xAA, 0xDA, 0xF6,
    0xE3, 0x0D, 0x2E, 0x58, 0x75, 0x64, 0x69, 0x6A,
    0x95, 0x91, 0x84, 0x8A, 0xB8, 0xA9, 0xD8, 0xCA,
    0xE6, 0x13, 0x3D, 0x5E, 0x4B, 0x7A, 0x6E, 0x6A,
    0x95, 0x91, 0x9A, 0x89, 0xBE, 0xAF, 0xDC, 0xC9,
    0xFA, 0x14, 0x00, 0x2D, 0x4E, 0x78, 0x6F, 0x6A,
    0x95, 0x91, 0x98, 0x8F, 0xBC, 0xA2, 0xD3, 0xC2,
    0xFF, 0xEB, 0x05, 0x31, 0x5D, 0x7F, 0x6C, 0x6A,
    0x95, 0x91, 0x99, 0x8D, 0xB3, 0xA6, 0xD7, 0xC4,
    0xF3, 0xEC, 0x1E, 0x0A, 0x20, 0x4D, 0x62, 0x6A,
    0x95, 0x96, 0x99, 0x83, 0xB6, 0xA5, 0xAB, 0xDB,
    0xF7, 0xE6, 0x12, 0x0C, 0x25, 0x51, 0x7D, 0x6A,
    0x95, 0x94, 0x9F, 0x81, 0x8B, 0xBE, 0xAD, 0xD3,
    0xCD, 0xFE, 0xE8, 0x1A, 0x31, 0x2C, 0x45, 0x6D,   
#endif
#else
    0xD9, 0xC2, 0xEA, 0x97, 0xB1, 0x5A, 0x47, 0x6F,
    0x10, 0xD9, 0x3D, 0x37, 0x2E, 0x27, 0x26, 0x26,
    0xD9, 0xC9, 0xF3, 0xE4, 0x88, 0xB2, 0x5B, 0x40,
    0x69, 0x1C, 0x0D, 0x06, 0x34, 0x21, 0x26, 0x26,
    0xD9, 0xDF, 0xF9, 0xE3, 0x95, 0xBD, 0xA2, 0x4A,
    0x70, 0x67, 0x17, 0x02, 0x31, 0x2D, 0x26, 0x26,
    0xD9, 0xDC, 0xCC, 0xE9, 0x90, 0x87, 0xAD, 0x55,
    0x7A, 0x60, 0x12, 0x01, 0x3C, 0x2A, 0x26, 0x26,
    0xD9, 0xDC, 0xCA, 0xFA, 0x99, 0x82, 0xB6, 0x5C,
    0x45, 0x6A, 0x1D, 0x0D, 0x3A, 0x28, 0x26, 0x26,
    0xD9, 0xDA, 0xC8, 0xFA, 0xEF, 0x89, 0xB5, 0x58,
    0x4E, 0x77, 0x1A, 0x08, 0x39, 0x36, 0x26, 0x26,
    0xD9, 0xDA, 0xC9, 0xF8, 0xEA, 0x9F, 0xB9, 0xA5,
    0x4B, 0x70, 0x67, 0x16, 0x05, 0x35, 0x26, 0x26,
    0xD9, 0xDA, 0xD7, 0xF9, 0xEB, 0x9B, 0x8B, 0xA9,
    0x54, 0x79, 0x60, 0x13, 0x03, 0x33, 0x26, 0x26,
    0xD9, 0xDA, 0xD4, 0xC6, 0xF6, 0x98, 0x88, 0xBD,
    0x59, 0x43, 0x76, 0x1A, 0x00, 0x30, 0x26, 0x26,
    0xD9, 0xDB, 0xD5, 0xC3, 0xF1, 0xE1, 0x93, 0xB9,
    0xAF, 0x49, 0x7C, 0x62, 0x16, 0x3E, 0x27, 0x26,
    0xD9, 0xDB, 0xD3, 0xC1, 0xF1, 0xE1, 0x91, 0x81,
    0xB5, 0x5F, 0x79, 0x6E, 0x12, 0x3C, 0x27, 0x26,
    0xD9, 0xD8, 0xD0, 0xCF, 0xFD, 0xE9, 0xE6, 0x8A,
    0xBF, 0xA4, 0x4F, 0x69, 0x16, 0x39, 0x25, 0x26,
    0xD9, 0xD8, 0xD1, 0xCA, 0xC4, 0xF4, 0xE5, 0x95,
    0x84, 0xA9, 0x54, 0x7F, 0x19, 0x03, 0x20, 0x26,
    0xD9, 0xD9, 0xDC, 0xC9, 0xC1, 0xFA, 0xEA, 0x9A,
    0x8B, 0xBC, 0xA0, 0x44, 0x6D, 0x09, 0x37, 0x25,
    0xD9, 0xD9, 0xDD, 0xD0, 0xC9, 0xC2, 0xFC, 0xEA,
    0xE4, 0x94, 0x84, 0xA8, 0x73, 0x1F, 0x39, 0x2E,
    0xD9, 0xD9, 0xD8, 0xDF, 0xD0, 0xD6, 0xC1, 0xF8,
    0xF2, 0xEE, 0x9C, 0x8C, 0xB0, 0x64, 0x0F, 0x29,
    0xD9, 0xD9, 0xD9, 0xD9, 0xDB, 0xDC, 0xD0, 0xC9,
    0xCC, 0xFA, 0xEB, 0x9B, 0x8F, 0xA9, 0x14, 0x3D,
    0x4A, 0x61, 0x79, 0x1C, 0x31, 0xD6, 0xED, 0xF8,
    0x96, 0x98, 0xAE, 0xB5, 0xBD, 0xB8, 0xB8, 0xB8,
    0x4F, 0x5A, 0x74, 0x47, 0x19, 0x30, 0xC9, 0xE2,
    0xFC, 0x8B, 0x9D, 0xA1, 0xB5, 0xBA, 0xB8, 0xB8,
    0x44, 0x54, 0x6A, 0x04, 0x1E, 0x28, 0xC0, 0xDA,
    0xF4, 0x81, 0x92, 0xA6, 0xAA, 0xBC, 0xB8, 0xB8,
    0x45, 0x4A, 0x64, 0x7B, 0x14, 0x2F, 0x39, 0xD0,
    0xEA, 0x85, 0x96, 0x99, 0xAB, 0xBE, 0xB8, 0xB8,
    0x45, 0x4C, 0x5C, 0x75, 0x0B, 0x23, 0x3E, 0xD5,
    0xE0, 0xFC, 0x8B, 0x9C, 0xAE, 0xBF, 0xB8, 0xB8,
    0x45, 0x4D, 0x5D, 0x6A, 0x04, 0x19, 0x30, 0xCB,
    0xE5, 0xFF, 0x8E, 0x9F, 0xA0, 0xB0, 0xB8, 0xB8,
    0x45, 0x4E, 0x5F, 0x6D, 0x7A, 0x14, 0x29, 0xC1,
    0xDA, 0xF4, 0x81, 0x91, 0xA3, 0xB1, 0xB8, 0xB8,
    0x45, 0x4E, 0x51, 0x6E, 0x7D, 0x0D, 0x26, 0x39,
    0xD0, 0xEA, 0x86, 0x95, 0xA5, 0xB3, 0xB8, 0xB8,
    0x45, 0x4F, 0x53, 0x61, 0x70, 0x0E, 0x1F, 0x34,
    0xC8, 0xE3, 0xFE, 0x8A, 0xA6, 0xB4, 0xB8, 0xB8,
    0x46, 0x41, 0x55, 0x66, 0x68, 0x05, 0x12, 0x2D,
    0xC4, 0xDD, 0xF7, 0x82, 0x99, 0xB5, 0xB8, 0xB8,
    0x46, 0x41, 0x56, 0x67, 0x68, 0x07, 0x17, 0x26,
    0x3D, 0xD4, 0xEB, 0x86, 0x92, 0xB6, 0xB8, 0xB8,
    0x46, 0x42, 0x57, 0x59, 0x6E, 0x70, 0x0F, 0x1B,
    0x37, 0xCD, 0xE4, 0xF8, 0x9F, 0xA9, 0xB8, 0xB8,
    0x46, 0x43, 0x4A, 0x5F, 0x61, 0x73, 0x03, 0x13,
    0x20, 0x3C, 0xDD, 0xF4, 0x97, 0xAC, 0xB8, 0xB8,
    0x47, 0x45, 0x4C, 0x53, 0x59, 0x6C, 0x7C, 0x0C,
    0x1C, 0x28, 0xC3, 0xED, 0x84, 0x99, 0xBC, 0xB8,
    0x47, 0x46, 0x40, 0x4B, 0x51, 0x58, 0x6C, 0x70,
    0x02, 0x12, 0x21, 0x3C, 0xFD, 0x94, 0xB7, 0xBA,
    0x47, 0x47, 0x44, 0x40, 0x4C, 0x53, 0x5B, 0x62,
    0x77, 0x7A, 0x0A, 0x18, 0x3F, 0x8D, 0xA4, 0xBF,
    0x47, 0x47, 0x47, 0x46, 0x43, 0x40, 0x4B, 0x55,
    0x5C, 0x6D, 0x7D, 0x0C, 0x24, 0x38, 0x9D, 0xB4,
    0xF6, 0xE3, 0xD8, 0xB3, 0xAA, 0x80, 0x66, 0x51,
    0x4A, 0x39, 0x2A, 0x10, 0x04, 0x0D, 0x09, 0x09,
    0xF6, 0xFD, 0xD3, 0xCD, 0xB8, 0x91, 0x88, 0x60,
    0x5A, 0x37, 0x27, 0x2B, 0x1C, 0xF6, 0x09, 0x09,
    0xF6, 0xF3, 0xEC, 0xC2, 0xBF, 0x96, 0x81, 0x78,
    0x53, 0x4D, 0x3A, 0x2F, 0x10, 0x05, 0x09, 0x09,
    0xF6, 0xF3, 0xE2, 0xDF, 0xB2, 0xAA, 0x87, 0x71,
    0x69, 0x40, 0x31, 0x20, 0x12, 0x04, 0x09, 0x09,
    0xF6, 0xF2, 0xE4, 0xD5, 0xCD, 0xA2, 0x9B, 0x74,
    0x6F, 0x46, 0x35, 0x24, 0x17, 0x06, 0x09, 0x09,
    0xF6, 0xF5, 0xE6, 0xD4, 0xC2, 0xBD, 0x90, 0x89,
    0x63, 0x5D, 0x36, 0x26, 0x28, 0x1B, 0x09, 0x09,
    0xF6, 0xF5, 0xF9, 0xD6, 0xC4, 0xB2, 0xAF, 0x80,
    0x66, 0x52, 0x4D, 0x3A, 0x2D, 0x1D, 0x09, 0x09,
    0xF6, 0xF5, 0xFB, 0xE9, 0xD9, 0xB7, 0xA7, 0x9C,
    0x70, 0x6B, 0x42, 0x30, 0x2F, 0x1F, 0x09, 0x09,
    0xF6, 0xF4, 0xFA, 0xEA, 0xDD, 0xCA, 0xBB, 0x94,
    0x8D, 0x60, 0x5C, 0x49, 0x22, 0x1E, 0x08, 0x09,
    0xF6, 0xF4, 0xFD, 0xEF, 0xDE, 0xCE, 0xBC, 0xAA,
    0x86, 0x7D, 0x54, 0x4F, 0x3B, 0x10, 0x08, 0x09,
    0xF6, 0xF7, 0xFF, 0xE1, 0xD0, 0xC2, 0xB5, 0xA3,
    0x9F, 0x76, 0x6D, 0x42, 0x3B, 0x14, 0x0B, 0x09,
    0xF6, 0xF7, 0xFE, 0xE2, 0xD6, 0xD8, 0xCB, 0xB9,
    0x97, 0x80, 0x66, 0x5D, 0x30, 0x2B, 0x0C, 0x09,
    0xF6, 0xF6, 0xF0, 0xE7, 0xEC, 0xDE, 0xC1, 0xB1,
    0xA1, 0x9C, 0x75, 0x54, 0x4C, 0x23, 0x05, 0x0B,
    0xF6, 0xF6, 0xF2, 0xFA, 0xE2, 0xE9, 0xDA, 0xCD,
    0xB1, 0xAF, 0x9D, 0x71, 0x59, 0x3D, 0x11, 0x0F,
    0xF6, 0xF6, 0xF4, 0xF1, 0xFA, 0xE3, 0xE8, 0xDC,
    0xC2, 0xB4, 0xA7, 0x95, 0x7F, 0x34, 0x2D, 0x05,
    0xF6, 0xF6, 0xF6, 0xF5, 0xF3, 0xFC, 0xE6, 0xEE,
    0xD6, 0xDB, 0xCD, 0xBD, 0xA9, 0x60, 0x24, 0x1C,
    0x92, 0xB8, 0xA2, 0xCB, 0xF0, 0x1E, 0x06, 0x28,
    0x5C, 0x4F, 0x7B, 0x76, 0x6C, 0x64, 0x65, 0x65,
    0x9F, 0x82, 0xAA, 0xDF, 0xC1, 0xE8, 0x10, 0x3B,
    0x2D, 0x53, 0x4D, 0x79, 0x75, 0x66, 0x65, 0x65,
    0x98, 0x8A, 0xB2, 0xA4, 0xCC, 0xF1, 0x18, 0x9A,
    0x2A, 0x5E, 0x49, 0x45, 0x76, 0x60, 0x65, 0x65,
    0x98, 0x97, 0x85, 0xA2, 0xCA, 0xFC, 0xE1, 0x0E,
    0x30, 0x24, 0x54, 0x46, 0x70, 0x62, 0x65, 0x65,
    0x98, 0x90, 0x80, 0xB4, 0xD2, 0xFB, 0xE2, 0x15,
    0x3E, 0x23, 0x50, 0x43, 0x72, 0x6D, 0x65, 0x65,
    0x98, 0x93, 0x83, 0xB1, 0xDA, 0xC2, 0xE9, 0x13,
    0x05, 0x2F, 0x5D, 0x4D, 0x7F, 0x6C, 0x65, 0x65,
    0x98, 0x93, 0x8D, 0xB3, 0xA1, 0xD4, 0xF2, 0x18,
    0x00, 0x2A, 0x59, 0x4E, 0x79, 0x6F, 0x65, 0x65,
    0x98, 0x92, 0x8E, 0xB2, 0xAD, 0xD3, 0xC6, 0xE2,
    0x09, 0x32, 0x27, 0x55, 0x7B, 0x69, 0x65, 0x65,
    0x9B, 0x9D, 0x89, 0xB9, 0xA9, 0xDF, 0xCC, 0xF4,
    0x12, 0x05, 0x2E, 0x53, 0x45, 0x68, 0x65, 0x65,
    0x9B, 0x9C, 0x8B, 0xBA, 0xB5, 0xD8, 0xCB, 0xFF,
    0xE4, 0x02, 0x37, 0x58, 0x43, 0x6B, 0x65, 0x65,
    0x9B, 0x9F, 0x8A, 0x85, 0xB6, 0xA1, 0xD6, 0xC4,
    0xEF, 0x14, 0x32, 0x5A, 0x4D, 0x75, 0x65, 0x65,
    0x9B, 0x9E, 0x94, 0x81, 0xBD, 0xAF, 0xDC, 0xC2,
    0xF6, 0x18, 0x04, 0x22, 0x4E, 0x76, 0x65, 0x65,
    0x9A, 0x99, 0x96, 0x8C, 0xB8, 0xB5, 0xA5, 0xD5,
    0xFB, 0xEE, 0x15, 0x34, 0x52, 0x7E, 0x67, 0x65,
    0x9A, 0x9B, 0x93, 0x8A, 0x80, 0xBF, 0xA9, 0xDB,
    0xCA, 0xF8, 0xEF, 0x08, 0x24, 0x42, 0x6F, 0x64,
    0x9A, 0x9A, 0x9F, 0x93, 0x8A, 0x83, 0xB9, 0xB4,
    0xA0, 0xD3, 0xC3, 0xF7, 0x03, 0x54, 0x72, 0x60,
    0x9A, 0x9A, 0x9B, 0x9E, 0x9D, 0x97, 0x8E, 0x81,
    0xBF, 0xA9, 0xD9, 0xCF, 0xF7, 0x3E, 0x44, 0x62,
#endif    
};
#endif
//*******************************************************************
//*******************************************************************
#ifdef ReduceDDC
void ScalerResetThenPowerDown(void)
{
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(SWRST, 0x00);
    Set_ms_ResetZ(); // reset chipset
    Delay1ms(5);
    Clr_ms_ResetZ();
    // Initialize Scaler block
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(DBFC, 0);
    mStar_WriteByte(SWRST, 0x00);
    //mStar_WriteByte(PDMD, 0x00);
    mStar_WriteByte(SWRST, 0x7F); //0x3F Bit6 IIC Reset// software reset
    Delay1ms(2);
    mStar_WriteByte(SWRST, 0x00); // software reset

    InitVccDetectPin();
    // 080918
    GPIOPWMSelect();
    
#if 0//def ReduceDDC // 080918   
    mStar_InitDDC();
    mStar_SetDDC_WP();
#endif

#if MS_OVD
     msOD_MIUPowerUp(_OD_Disable);
#endif

    mStar_WriteByte(REGBK, REGBANKTCON);
    mStar_WriteByte(0x77, 0xa0);
    if((mStar_ReadByte(0x77)&BIT5) == BIT5)
    {
        Set_PowerGoodExistFlag();
        mStar_WriteByte(0x77, mStar_ReadByte(0x77)|BIT6);
        mStar_WriteByte(REGBK, REGBANKSCALER);
        mStar_WriteByte(PDMD, 0x03);
    }
    else
    {
        Clr_PowerGoodExistFlag();
        mStar_WriteByte(REGBK, REGBANKSCALER);
        mStar_WriteByte(PDMD, VDD2LOW|0x03);
    }
    mStar_WriteByte(SWRST, 0x80);
}

void InitVccDetectPin(void)
{
    #if MainBoardType == MainBoard_2883_1B
      MCU_P0_OUT_EN &= ~BIT0;  
      MCU_P0_FMD &= ~BIT0;	
    #elif (MainBoardType == MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529)
      MCU_P1_OUT_EN &= ~BIT7;  
      MCU_P1_FMD &= ~BIT7;	
    #endif
}

void SystemVccOK(void)
{
    BYTE tempCnt = 0;
    do{
            if(hwSystemVcc_Pin)
                tempCnt ++;
            else
                tempCnt = 0;
            ForceDelay1ms(2);
    }while(tempCnt < 100);
    SystemVccDropDownCnt = 0;
    EnCheckVccFlag = 1;
}
void ScalerIntoDeepPowerDown(void)
{
    BYTE tempBank = mStar_ReadByte(REGBK);
    #if Dual
    mStar_WriteByte(REGBK, REGBANKADC);
        #ifdef TSUMXXNX
        mStar_WriteByte(TESTEN, mStar_ReadByte(TESTEN)|BIT7);
        mStar_WriteByte(TESTA5, mStar_ReadByte(TESTA5)|BIT0);
        mStar_WriteByte(BK1_D1, mStar_ReadByte(BK1_D1) |BIT5);
        mStar_WriteByte(TESTEN, mStar_ReadByte(TESTEN)&~BIT7);
        #else
        mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4) | BIT0); // power down dvi clk r-termintor
        #endif
    #endif        

#if MS_OVD
    msOD_MIUPowerUp(_OD_Disable);
#endif
    
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(PDMD, mStar_ReadByte(PDMD) | 0x03);
    mStar_WriteByte(SWRST, 0x80);
    mStar_WriteByte(REGBK, tempBank);
}
#endif


#if 1
void ScalerReset(void)
{
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(SWRST, 0x00);
    Set_ms_ResetZ(); // reset chipset
    Delay1ms(5);
    Clr_ms_ResetZ();
    // Initialize Scaler block
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(DBFC, 0);
    mStar_WriteByte(SWRST, 0x00);
    mStar_WriteByte(PDMD, 0x00);
    mStar_WriteByte(SWRST, 0x7F); // mStar_WriteByte(SWRST, 0x3F); // software reset
    Delay1ms(2);
    mStar_WriteByte(SWRST, 0x00); // software reset
#if IT6251
CLR_EDP_IT6251_RSTB();
ForceDelay1ms(5);
SET_EDP_IT6251_RSTB();
#endif

    #ifdef ReduceDDC
	#if !Enable_External_EDID
    mStar_SetDDC_WP();
	#endif
    #endif

#if AudioFunc || ENABLE_HDMI
	#if MainBoardType==MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
	hw_SetMute();
	#endif
#endif

	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
	#else
    mStar_WriteByte(REGBK, REGBANKTCON);
    mStar_WriteByte(0x77, 0xa0);
    if((mStar_ReadByte(0x77)&BIT5) == BIT5)
    {
        Set_PowerGoodExistFlag();
        mStar_WriteByte(0x77, mStar_ReadByte(0x77)|BIT6);
        mStar_WriteByte(REGBK, REGBANKSCALER);
        mStar_WriteByte(PDMD, 0x00);
    }
    else
    {
        Clr_PowerGoodExistFlag();
        mStar_WriteByte(REGBK, REGBANKSCALER);
        mStar_WriteByte(PDMD, VDD2LOW);
    }
	#endif

// 090810 coding, add setting for mini
#if PanelminiLVDS
    SetRSDSToTTL(1);  //nick 090709
    mStar_WriteByte(TRISTATE, 0xFF); // tri-state all output panel signal
#endif
    
    // 090318 modified for reducing thermal
    #if MainBoardType == MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2
    mStar_WriteByte(PDMD, mStar_ReadByte(PDMD)|BIT3);
    #endif
    
#if MS_OVD // 081124 od modified
     msOD_MIUPowerUp(_OD_Enable);
#endif

}
#else
void ScalerReset(void)
{
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(SWRST, 0x00);
    Set_ms_ResetZ(); // reset chipset
    Delay1ms(5);
    Clr_ms_ResetZ();
    // Initialize Scaler block
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(DBFC, 0);
    mStar_WriteByte(SWRST, 0x3F); // software reset
    Delay1ms(2);
    mStar_WriteByte(SWRST, 0x00); // software reset
    // power down contorl 0xF0~0xF1
       mStar_WriteByte(PDMD, 0x00 | BIT6); // power up chip      ?== new added for avoid the  flash data loss issue
       mStar_WriteByte(TEST, 0x00); // ???? important
    

mStar_WriteByte(REGBK, REGBANKTCON);
  mStar_WriteByte(0x77,BIT5);
  mStar_WriteByte(0x77,mStar_ReadByte(0x77)|BIT6);  //70us
  mStar_WriteByte(REGBK, REGBANKSCALER);
}
#endif

#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify

// set initial values of color engine: IHC
static void mStar_InitIHC (void)
{
    volatile BYTE ucBank = mStar_ReadByte( REGBK ); // store original bank

    mStar_WriteByte(REGBK, REGBANK6);
    mStar_WriteByteMask(BK6_31, 0x24,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // define_range_r
    mStar_WriteByteMask(BK6_32, 0x40,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // define_range_g
    mStar_WriteByteMask(BK6_33, 0x24,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // define_range_b
    mStar_WriteByteMask(BK6_34, 0x24,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // act_range_r
    mStar_WriteByteMask(BK6_35, 0x40,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // act_range_g
    mStar_WriteByteMask(BK6_36, 0x24,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // act_range_b
    mStar_WriteByteMask(BK6_37, 0x23,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // act_range_c
    mStar_WriteByteMask(BK6_38, 0x39,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // act_range_m
    mStar_WriteByteMask(BK6_39, 0x23,  (BIT5|BIT4|BIT3|BIT2|BIT1|BIT0));  // act_range_y

    mStar_WriteByte( REGBK, ucBank ); // restore original bank

}

// set initial values of color engine: 3D dither
static void mStar_InitDither3D (void)
{
    volatile BYTE ucBank = mStar_ReadByte( REGBK ); // store original bank

    mStar_WriteByte(REGBK, REGBANK3);
    mStar_WriteByte(BK3_D3, 0x63);  // 3d dither horizontal dither table 0
    mStar_WriteByte(BK3_D4, 0x9C);  // 3d dither horizontal dither table 1
    mStar_WriteByte(BK3_D5, 0x27);  // 3d dither horizontal dither table 2
    mStar_WriteByte(BK3_D6, 0x8D);  // 3d dither horizontal dither table 3
    mStar_WriteByte(BK3_D7, 0x93);  // 3d dither vertical   dither table 0
    mStar_WriteByte(BK3_D8, 0x8D);  // 3d dither vertical   dither table 1
    mStar_WriteByte(BK3_D9, 0x4E);  // 3d dither vertical   dither table 2
    mStar_WriteByte(BK3_DA, 0x39);  // 3d dither vertical   dither table 3

    mStar_WriteByte( REGBK, ucBank ); // restore original bank
}

#endif  // end of #if( ChipID == TSUMXXT )		//110311 Modify
//*******************************************************************
//
//*******************************************************************
// Function Name: mStar_Init(void)
//
// Description: Initialize mStar chip while 1st power on system
//
// Caller: mStar_InitADC(), mStar_InitTCON, mStar_SetupFreeRunMode()
//         mStar_SetupInputPort() in mStar.c
//         mStar_WriteByte(), mStar_WriteWord() in ms_rwreg.c
//         mStar_WriteDDC1(), mStar_WriteDDC2() in ddc.c
//         mStar_InitGamma() in gamma.c
//         Osd_InitOSD() in osd.c
// Callee: Init_Device() in main.c
//*******************************************************************
void GPIOPWMSelect(void)
{
    mStar_WriteByte(REGBK, REGBANKSCALER);
    #if MST_MCU
        #if (MainBoardType==MainBoard_2573)
            mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH) &~BIT3) | BIT4 | BIT7 | BIT1); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
        #elif (MainBoardType==MainBoard_2701)
            mStar_WriteByte(PG_SWCH, 0x05); //0x0D); // Disable PWM2
        #elif (MainBoardType==MainBoard_2186_1)
            mStar_WriteByte(PG_SWCH, 0x12); // Disable PWM2
        #elif (MainBoardType==MainBoard_2186_2A)
            mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH) &~BIT3) | BIT4 | BIT7 | BIT1); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
        #elif (MainBoardType==MainBoard_2698_2_X_X_2)
            mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)) | BIT6 | BIT7 | BIT4 | BIT2); // 128 Pin
        #elif (MainBoardType==MainBoard_2698_2_X_X_8)
            mStar_WriteByte(PG_SWCH, 0x05);
        #elif (MainBoardType==MainBoard_715G2575)
            mStar_WriteByte(PG_SWCH, ( mStar_ReadByte( PG_SWCH ) & ~BIT3 ) | BIT2 | BIT0);
        #elif (MainBoardType==MainBoard_2810_B)
            #ifdef BGJ_TO_5PWGJ
            mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)) |  BIT1 | BIT3 | BIT4); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
            #else
            mStar_WriteByte(PG_SWCH,  BIT4| BIT3);//| BIT2);
            #endif
	#elif (MainBoardType==MainBoard_715G2808||MainBoardType==MainBoard_715G2988)		
           mStar_WriteByte(PG_SWCH, 0x18); // 128 Pin        //  ivan    bit3|bit4
	#elif (MainBoardType==MainBoard_2883_B_X_X_1)//yuanfa
           mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)&~BIT1&~BIT5&~ BIT4)| BIT0 | BIT2  ); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
    #elif (MainBoardType==MainBoard_715G3329_1_2)//yuanfa
           mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)| BIT4 | BIT2)); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
       #elif (MainBoardType==MainBoard_2883_B_X_X_2||MainBoardType==MainBoard_2883_1B)//yuanfa
           mStar_WriteByte(PG_SWCH, BIT2|BIT4);
       #elif MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A
        #ifdef TSUMXXNX
            #if AudioFunc
            mStar_WriteByte(PG_SWCH, BIT2|BIT0);
            #else
            mStar_WriteByte(PG_SWCH, BIT2);
            #endif
        #else
            #if AudioFunc
            mStar_WriteByte(PG_SWCH, BIT2|BIT4);
            #else
            mStar_WriteByte(PG_SWCH, BIT2);
            #endif
        #endif            
	#elif (MainBoardType==MainBoard_715G3329_58N)
		   mStar_WriteByte(PG_SWCH, BIT0|BIT2);
	#elif (MainBoardType==MainBoard_715G3329_18N)
		   mStar_WriteByte(PG_SWCH, BIT0|BIT2);
	#elif(MainBoardType==MainBoard_712G3244_2)
	      #if AudioFunc
            mStar_WriteByte(PG_SWCH, BIT2|BIT0);
            #else
            mStar_WriteByte(PG_SWCH, BIT2);
            #endif
	#elif (MainBoardType==MainBoard_715G2564_B)
            mStar_WriteByte(PG_SWCH,0x05);
	#elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529)
        	mStar_WriteByte(PG_SWCH, BIT0|BIT2);//(mStar_ReadByte(PG_SWCH)&~BIT4&~BIT5)|BIT0 |BIT2|BIT6|BIT7);
        #elif (MainBoardType==MainBoard_715G4734)
        	mStar_WriteByte(PG_SWCH, BIT0|BIT2);//(mStar_ReadByte(PG_SWCH)&~BIT4&~BIT5)|BIT0 |BIT2|BIT6|BIT7);
	#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
        	mStar_WriteByte(PG_SWCH, BIT0|BIT2);
	#elif (MainBoardType==MainBoard_715G3959_T)		//110617 Modify
        	mStar_WriteByte(PG_SWCH, BIT0|BIT2);
	#elif (MainBoardType==MainBoard_715G5265_T||(MainBoardType == MainBoard_SLB1PTR||MainBoardType == MainBoard_JRYL1PTR_LF))	//petit 20121030	
        	mStar_WriteByte(PG_SWCH, BIT0|BIT2);
	#elif (MainBoardType==MainBoard_2670_C||MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611)   //Amy2007.12.18   9:48:35           
            mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH))|BIT1|BIT6|BIT3);
	#elif MainBoardType==MainBoard_2670_1
       	mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH))|BIT1|BIT6|BIT3); // Disable PWM2  
        #elif MainBoardType == MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2
            #if AudioFunc
            mStar_WriteByte(PG_SWCH, 0x4a);
            #else
            mStar_WriteByte(PG_SWCH, 0x0a);
            #endif
        #elif MainBoardType == MainBoard_715G3603_M0A
            #if AudioFunc||ENABLE_HDMI
            mStar_WriteByte(0x64, BIT4); // volume
            #endif
            mStar_WriteByte(PG_SWCH, BIT0);
        #elif MainBoardType==MainBoard_715G3663_M0A
            #if AudioFunc||ENABLE_HDMI
            mStar_WriteByte(0x64, BIT4); // volume
            #endif
            mStar_WriteByte(PG_SWCH, BIT0);
		#elif (MainBoardType==MainBoard_715G3598_1PLL)
			mStar_WriteByte(PG_SWCH, BIT2|BIT5);
    #elif (MainBoardType==MainBoard_715G3834)
		mStar_WriteByte(PG_SWCH, BIT0|BIT2);
	#elif (MainBoardType==MainBoard_3834_18T)//petit 20121228
		mStar_WriteByte(PG_SWCH, BIT0|BIT2);
	#elif (MainBoardType==MainBoard_715G6952_M0B) || (MainBoardType==MainBoard_715G6912_M1A)//Gino 20140522
		mStar_WriteByte(REGBK, REGBANKADC);
		mStar_WriteByte(BK1_C9, BIT4|BIT7);
    	mStar_WriteByte(REGBK, REGBANKSCALER);
	#elif MainBoardType==MainBoard_715G4175_M0A
	        mStar_WriteByte(PG_SWCH, BIT2);
	#elif (MainBoardType == MainBoard_4432)
    msWriteByte( BK0_63, BIT2 | BIT0 ); // Rex 100705 for Set mute
        #else
            mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)) | BIT0 | BIT2 | BIT4 | BIT6); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
        #endif
    #endif

    hw_SetFlashWP(); // 100802 coding addition

}
void mStar_Init(void)
{
    ChipVer = msReadID();	
    mStar_WriteByte(DBFC, 0);
    // power down contorl 0xF0~0xF1
    if(!PowerGoodExistFlag)
        mStar_WriteByte(PDMD, VDD2LOW); // power up chip
    mStar_WriteByte(TEST, 0x00); // ???? important
    //=========================================================================
    // display timing
    // output DE size // 0x10~0x17
    mStar_WriteWord(DEVST_H, 0); // Vert. DE start
    mStar_WriteWord(DEVEND_H, PanelHeight - 1); // Vert. DE end
    mStar_WriteWord(DEHST_H, PanelHStart); // Hor. DE start
    mStar_WriteWord(DEHEND_H, PanelHStart + PanelWidth - 1); // Hor. DE end
    // Scaling Image window size 0x18~0x1D
    mStar_WriteWord(SIHST_H, PanelHStart);
    mStar_WriteWord(SIHEND_H, PanelHStart + PanelWidth - 1);
    mStar_WriteWord(SIVEND_H, PanelHeight - 1);
    // Output Sync Timing //0x1E~0x26
    mStar_WriteWord(VDTOT_H, PanelVTotal - 1); // output vtotal
    mStar_WriteWord(VSST_H, PanelVTotal - PanelVStart); // vsync start
    mStar_WriteWord(VSEND_H, PanelVTotal - PanelVSyncBackPorch); //vsync end
    mStar_WriteWord(HDTOT_H, PanelHTotal - 1); // output htotal
    mStar_WriteByte(HSEND, PanelHSyncWidth - 1); //
    mStar_WriteByte(OSCCTRL1, LCM_B | HSRM_B | AHRT_B | CTRL_B); // enable auto-htotal
    mStar_WriteByte(OSCCTRL2, ATEN2_B); // setup clock mode

    #if PanelRSDS||PanelminiLVDS
    if(PanelWidth > 1440 || PanelHeight > 900)
        mStar_WriteByte(0x9f, 0x10);
    else
        mStar_WriteByte(0x9f, 0x00);
    #endif
	
    // output control // 0x42~0x44
    #if EnablePanelServiceMenu
     mStar_WriteByte(OCTRL1, PanelOutputControl1); // setup
    #else
    #if 0// " +/- " SWAP OF LVDS OUTPUT
        mStar_WriteByte(OCTRL1, (LTIM_B &LVDS_TIMode) | (PanelSwapOddRB &ORBX_B) | (PanelSwapEvenRB &ERBX_B) |  // setup output port
        (PanelSwapMLSB &MLXT_B) | (PanelSwapOddML &OMLX_B) | (PanelSwapEvenML &EMLX_B) | (PanelSwapPlusMinus &LCPS_B)); // setup
    #else
		#if ( ModelName == ENVISION_P2071l||ModelName==ENVISION_P2071l1)
        mStar_WriteByte(OCTRL1, (LTIM_B &LVDS_TIMode) | (PanelSwapOddRB &ORBX_B) | (PanelSwapEvenRB &ERBX_B) |  // setup output port
        (PanelSwapMLSB &MLXT_B) | (PanelSwapOddML &OMLX_B) | (PanelSwapEvenML &EMLX_B)|(PanelSwapPolarity & LCPS_B)); // setup
		#else
        mStar_WriteByte(OCTRL1, (LTIM_B &LVDS_TIMode) | (PanelSwapOddRB &ORBX_B) | (PanelSwapEvenRB &ERBX_B) |  // setup output port
        (PanelSwapMLSB &MLXT_B) | (PanelSwapOddML &OMLX_B) | (PanelSwapEvenML &EMLX_B)); // setup
        #endif
    #endif
    #endif
    
    #if PanelRSDS
	#if PanelType==PanelCMOM220Z1_PS3||PanelType==PanelCLAA220WA02
        mStar_WriteByte(OCTRL2,  (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B));
        #elif PanelType == PanelTPM215HW01
        mStar_WriteByte(OCTRL2, (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B) | BLKS_B);
	#else
        mStar_WriteByte(OCTRL2, TCOP_B | (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B));
	#endif
        mStar_WriteByte(OCTRL3, 0);//0x1F); // RSDS output with all GPO
	#if PanelType==PanelCMO220Z1M01
		Set_CLK();
	#endif
    #elif PanelminiLVDS
        mStar_WriteByte(OCTRL2, (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B) | BLKS_B);
        mStar_WriteByte(OCTRL3, 0);
    #elif PanelLVDS
	 #if EnablePanelServiceMenu
            mStar_WriteByte(OCTRL2, PanelOutputControl2);
        #else
        mStar_WriteByte(OCTRL2, DOT_B | (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B));
	 #endif
        mStar_WriteByte(OCTRL3, 0); // LVDS output
    #elif PanelTTL
        mStar_WriteByte(OCTRL2, (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B));
        mStar_WriteByte(OCTRL3, 0); // TTL Output
    #endif
    
    #if 0//EnablePanelServiceMenu
    //mStar_WriteByte(OSCTRL, 0x00); // output signal polarity control
    mStar_WriteByte(OSCTRL, PanelOSContol); // output signal polarity control
    // output driving current 0xF5
    mStar_WriteByte(ODRV, PanelODRriving); // setup output driving current
    #else
    // output signal control 0xF2
    //mStar_WriteByte(OSCTRL, 0x00); // output signal polarity control
    //mStar_WriteByte(OSCTRL, (PanelDClkDelay << 4) | (OCLK_B &PanelInvDClk) | (ODE_B &PanelInvDE) | (OVS_B &PanelInvVSync) | (OHS_B &PanelInvHSync)); // output signal polarity control
    // output driving current 0xF5
    ///mStar_WriteByte(ODRV, (PanelDCLKCurrent << 6) | (PanelDECurrent << 4) | (PanelOddDataCurrent << 2) | PanelEvenDataCurrent); // setup output driving current
    #endif
    //=========================================================================
    // Clock generator control 0xD0~0xDB
    mStar_WriteByte(PLLCTRL1, 0x00); //
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )  // Rose's new MPLL doesn't require settings		//110311 Modify
    mStar_WriteByte(PLLCTRL2, 0x04);   // restore power-on default value of BK0_D1
	#else
    mStar_WriteByte(PLLCTRL2, BIT7); // setup output pll & master pll
    #endif
    mStar_WriteByte( MDCTRL, BIT7); //mStar_WriteByte(MDCTRL, 0x00);
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
	mStar_WriteByte(MPLL_M, 0x0F);// set master PLL as power-on default value
	mStar_WriteByte(LPLL_M, 0x40); // output pll spread spectrum control
	#else
    // Master PLL
    mStar_WriteByte(MPLL_M, 0x6F); // set master pll as 215 MHz & drive current
    // output dclk setting
    mStar_WriteByte(LPLL_M, 0x48); //0x42); // output pll spread spectrum control
    #endif
	#ifdef TSUMXXT//#if( ChipID== TSUMXXT )  // the LPLL related settings of TSUMXXT has been placed in mStar_SetPanelTiming()		//110311 Modify
	#else
    #if PanelLVDS
        mStar_WriteByte(LPLL_CTL2, 0x1B); //0x0B); // output pll type & divider
    #else //select Output PLL type BIT5
        mStar_WriteByte(LPLL_CTL2, 0x3B); //0x2B); // output pll type & divider
    #endif
	#endif

	#if PANELSSCSTEPLFUN
    mStar_WriteByte(LPLL_STEP_L, PanelSSCSTEPL);
    mStar_WriteByte(LPLL_STEP_H, PanelSSCSTEPH);
    mStar_WriteByte(LPLL_SPAN_L, PanelSSCSPANL);
    mStar_WriteByte(LPLL_SPAN_H, PanelSSCSPANH);
	#else
	mStar_WriteWord(LPLL_STEP_H, LPLL_STEP_NUM);
	mStar_WriteWord(LPLL_SPAN_H, LPLL_SPAN_NUM);
	#endif
	
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
	mStar_WriteByte(BK0_5E, mStar_ReadByte(BK0_5E)|BIT3);	//110419 Modify
	mStar_WriteByte(REGBK, REGBANK2);//{REGBK,   REGBANK2},
	mStar_WriteByte(BK2_6B, (BD_DOUBLE_LVDS_CLK_SWING & BIT0));;//{BK2_6B, (BD_DOUBLE_LVDS_CLK_SWING & BIT0) }, // TSUMXXT supports to double swing of LVDS clock.

	mStar_WriteByte(REGBK, REGBANK4);//{REGBK,   REGBANK4},
	mStar_WriteByte(BK4_51, 0x00);//{BK4_51,	0x00},	// <Janus>2010/07/14: Restore power-on default value of BK4_51 to ensure normal detection of SoG when (PM_SUPPORT_SOG_TIME_SHARE==1)
	mStar_WriteByte(REGBK, REGBANK0);
	#endif
    // Dither control //0x36~0x38
    #if EnablePanelServiceMenu
	if(PanelSettingFlag&bPanelColorDepth)
	        mStar_WriteByte(0x36, 0x00);
	else
	        mStar_WriteByte(0x36, 0x01);
    #else
                mStar_WriteByte(DITHCTRL, 0x01);
    #endif
    #if PanelType==PanelCLAA220WA02
    mStar_WriteByte(DITHCOEF, 0xE4); 
    #else
    mStar_WriteByte(DITHCOEF, 0x2d); //0xe1 );//Benz 2007.4.14   14:37:23
    #endif
    #if PanelType==PanelHSM215EU_HTN01
    mStar_WriteByte(TRFN, 0x45);//0x07); 
    #else
    mStar_WriteByte(TRFN, 0x42); //0x13);    //Benz 2007.4.14   14:37:33
    #endif
    //====================================================================
    mStar_WriteByte(DDCCTRL, 0x00); // DDC control
    
    mStar_WriteByte(MISCFC, BIT2|BIT5|BIT7);    // 071026 RD suggest, New added for auto position error
    
    // display port
    // brightness 0x2A~0x2D
    mStar_WriteByte(BRC, BRC_B); // disable birghtness control
    
    // contrast 0x2E~0x31
    mStar_WriteByte(CNTR, 0x60 | CNTR_B); // enable contrast function and set 0 as center
        
    // border color 0x32~0x35
    mStar_WriteByte(FWC, 0); //FWC_B); // panel background color
    #if UsesRGB
        mStar_WriteByte(FTAPEN, 0x02); // enable srgb function
    #else
        mStar_WriteByte(FTAPEN, 0x00); // use bk0 5Ch / 5Dh setting for sharpness
        // Filter coefficient(sharpness) 0x65~0x69
    /*        
	#if (PanelType==PanelTPM215HW01)
        mStar_WriteByte(FTAPEN, 0x00); //2);   // enable srgb function
	#else
        mStar_WriteByte(FTAPEN, 0x01); //2);   // enable srgb function
        #endif
        mStar_WriteByte(FTAPC1, 0x00);
        mStar_WriteByte(FTAPC2, 0x40);
	#if EnablePanelServiceMenu
	if (PanelWidth==1366&&PanelHeight==768)
	{
	        mStar_WriteByte(FTAPC3, 0x00);
	        mStar_WriteByte(FTAPC4, 0x40);
	}
	else
	{
	        mStar_WriteByte(FTAPC3, 0x88);
	        mStar_WriteByte(FTAPC4, 0x48);
	}
	#else
	#if PanelWidth==1366&&PanelHeight==768
        mStar_WriteByte(FTAPC3, 0x00);
        mStar_WriteByte(FTAPC4, 0x40);
	#else
        mStar_WriteByte(FTAPC3, 0x88);
        mStar_WriteByte(FTAPC4, 0x48);
	#endif
	#endif
	        /***************************************
        If FTAPEN[0]=1,0x66,0x67,0x68,0x69 should follow the below table.
        	0x65	0x66	0x67	0x68	0x69
        0	0x01	0x54	0x14	0x54	0x14
        4	0x01	0x58 	0x18	0x58	0x18
        8	0x01	0x5C	0x1C	0x5C	0x1C
        12	0x01	0x60	0x20	0x60	0x20
        16	0x01	0x64	0x24	0x64	0x24
        20	0x01	0x68	0x28	0x68	0x28
        24	0x01	0x6C	0x2C	0x6C	0x2C
        28	0x01	0x70	0x30	0x70	0x30
        32	0x01	0x74	0x34	0x74	0x34
        36	0x01	0x78	0x38	0x78	0x38
        40	0x01	0x7C	0x3C	0x7C	0x3C
        44	0x01	0x80	0x40	0x80	0x40
        48	0x01	0x84	0x44	0x84	0x44
        52	0x01	0x88	0x48	0x88	0x48
        56	0x01	0x8C	0x4C	0x8C	0x4C
        60	0x01	0x8F	0x4F	0x8F	0x4F
        64	0x01	0x90	0x50	0x90	0x50
        68	0x01	0x94	0x54	0x94	0x54
        72	0x01	0x98	0x58	0x98	0x58
        76	0x01	0x9C	0x5C	0x9C	0x5C
        80	0x01	0xA0	0x60	0xA0	0x60
        84	0x01	0xA4	0x64	0xA4	0x64
        88	0x01	0xA8	0x68	0xA8	0x68
        92	0x01	0xAC	0x6C	0xAC	0x6C
        96	0x01	0xB0	0x70	0xB0	0x70
        100	0x01	0xB4	0x74	0xB4	0x74
        ***************************************/
    
    #endif
    // Auto Adjustment 0x78~
    mStar_WriteByte(ATOCTRL, 0x1); // enable auto position
    mStar_WriteByte(ATPCTRL, 0x19); // enable auto phase
    mStar_WriteByte(ATGCTRL, 0); //011); // enable auto Gain
    #if MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611
    mStar_WriteByte(ATPTH, 0x10); // enable auto phase
    #else
    mStar_WriteByte(ATPTH, 0x1C); // enable auto phase
    #endif
    mStar_WriteByte(AOVDV, 0x40);
    // VSync Status
    mStar_WriteByte(LHMARGIN, 0x30);
    mStar_WriteByte(LSMARGIN, 0x20);
    // interrupt control
    mStar_WriteByte(INTCTRL, 0x20);
    mStar_WriteByte(INTPULSE, 0x0F);
    mStar_WriteByte(INTENA, 0);
    mStar_WriteByte(INTENB, 0);		//110311 Modify
    mStar_WriteByte(HSTOL, 3);
    mStar_WriteByte(VSTOL, 1);
    mStar_WriteByte(HSPRD_H, 0x00); //10);
    mStar_WriteByte(0xEB, 0x1E);
	#ifdef TSUMXXQX
    mStar_WriteByte(DIMD, BIT2);
	#endif
    //ChipVer = mStar_ReadByte(0xFD); //0729
    //printData("Chip ID = %x",ChipVer);
//120907 Modify
#if defined(TSUMXXNX)||defined(TSUMXXT)
	mStar_WriteByteMask( BK0_6C, BIT7, BIT7|BIT6|BIT5 );
#elif defined(TSUMXXQX)||defined(TSUMXXMX)
	mStar_WriteByteMask( BK0_48, BIT3, BIT3 );
	mStar_WriteByteMask( BK0_6C, BIT7, BIT7|BIT6|BIT5 );
	mStar_WriteByteMask( BK0_48, 0,    BIT3 );
#endif

    #if MS_OVD
    msODLibVersionCheck(); // to prevent complier warning
    msLoadODT(0, tOverDrive);
    mStar_Load_tblOVD();
    msOD_SetMIUSSC(_MIU_SSC_40K_ONE_PERCENT);	//msOD_SetMIUSSC(_MIU_SSC_OFF);
    #endif

    #if ENABLE_HDCP
    msInitHDCPProductionKey();
    #endif

    //========================================================
    //=======================================================
    mStar_InitADC();
    mStar_SetupFreeRunMode();
    mStar_InitTCON();

#if ENABLE_HDMI
    mstar_HDMIInitialVariable();
    mstar_HDMIInitialAudio();
#endif

#if MS_DAC
    msAudioDAC_Init(); // Audio DAC init
#endif
    
    mStar_SetupInputPort();

#if PANEL_VCOM_ADJUST		//121031 Modify
	g_bACPowerOn_CheckVCOM = TRUE ;
#endif

    #if MST_MCU
	//******************************************************************************************************
	//Scaler Bank 0x63 Moved to GPIOPWMSelect Function.Why????
	//For Example:Flash WP Pin Uses GPIO2_4/PWM2,if we don't set it to GPIO the Register Status is unknown.
	//So we Must Set Scaler Bank 0x63 First and then use it,In other words,We must set 0x63 and then Set Flash Pin High or Low.
	//******************************************************************************************************
        #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
        mStar_WriteByte(0xC0, mStar_ReadByte(0xC0)&(~BIT1));    // for PWM mask
        #endif
        
        #if (MainBoardType==MainBoard_2573)
            mStar_WriteByte(PWMDIV2, 0xBE);
            mStar_WriteByte(PWMEXT, 0x08);
            //mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH) &~BIT3) | BIT4 | BIT7 | BIT1); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
        #elif (MainBoardType==MainBoard_2701)
            mStar_WriteByte(PWMDIV1, 0x1E); //power request 390Hz
            mStar_WriteByte(PWMEXT, 0x04);
            //mStar_WriteByte(PG_SWCH, 0x05); //0x0D); // Disable PWM2
        #elif (MainBoardType==MainBoard_2186_1)
            //mStar_WriteByte( PWMDIV2, 0xBE );
            //mStar_WriteByte( PWMEXT, 0x08);
            //Do not use burst mode
            //mStar_WriteByte(PG_SWCH, 0x12); // Disable PWM2
        #elif (MainBoardType==MainBoard_2186_2A)
            mStar_WriteByte( PWMDIV2, 0xBE );
            mStar_WriteByte( PWMEXT, PWM2BIT8);
            //mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH) &~BIT3) | BIT4 | BIT7 | BIT1); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
        #elif (MainBoardType==MainBoard_2698_2_X_X_2)
            //mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)) | BIT6 | BIT7 | BIT4 | BIT2); // 128 Pin
            mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) | PWMS); //PWM3
            mStar_WriteByte(PWMDIV2, 0xBE);
            mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) |PWM3BIT8);
        #elif (MainBoardType==MainBoard_2698_2_X_X_8)
            mStar_WriteByte(PWMDIV1, 0xBE);
            mStar_WriteByte( PWMEXT, PWM1BIT8);
            //mStar_WriteByte(PG_SWCH, 0x05);
        #elif (MainBoardType==MainBoard_715G2575)
            //mStar_WriteByte(PG_SWCH, ( mStar_ReadByte( PG_SWCH ) & ~BIT3 ) | BIT2 | BIT0);
            mStar_WriteByte(PWMDIV1,0xBE);
            mStar_WriteByte(PWMEXT, PWM1BIT8);
        #elif (MainBoardType==MainBoard_2810_B)
            #ifdef BGJ_TO_5PWGJ
            mStar_WriteByte(PWMDIV2, 0xBE);
            mStar_WriteByte(PWMEXT, 0x08);
            //mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)) |  BIT1 | BIT3 | BIT4); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
            #else
            //mStar_WriteByte(PG_SWCH,  BIT4| BIT3);//| BIT2);
            mStar_WriteByte(PWMDIV2, 0xBE);
            mStar_WriteByte(PWMEXT, PWM2BIT8);
            #endif
	#elif (MainBoardType==MainBoard_715G2808||MainBoardType==MainBoard_715G2988)		
           //mStar_WriteByte(PG_SWCH, 0x18); // 128 Pin        //  ivan    bit3|bit4
           mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) | PWM2BIT8);  //ivan
           #if MainBoardType==MainBoard_715G2988 // 080110 brightness freq.
           mStar_WriteByte(PWMDIV2, 0x9e); // 270Hz
           #else
           mStar_WriteByte(PWMDIV2, 0xbf); // 250Hz
           #endif
           mStar_WriteByte(PWMDIV1, 0x05); // 080110 audio freq.
	#elif (MainBoardType==MainBoard_2883_B_X_X_1)//yuanfa
           mStar_WriteByte(PWMDIV1, 0xAE);
           mStar_WriteByte(PWMEXT, PWM1BIT8);
    #elif (MainBoardType==MainBoard_715G3329_1_2)//yuanfa
           mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) | PWMS); //PWM3
	       mStar_WriteByte(PWMDIV2, 0xBE);
	       mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) |PWM3BIT8);
           //mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)&~BIT1&~BIT5&~ BIT4)| BIT0 | BIT2  ); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
       #elif (MainBoardType==MainBoard_2883_B_X_X_2||MainBoardType==MainBoard_2883_1B||\
                 MainBoardType==MainBoard_715G3329_A2)//MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A)//yuanfa
          // mStar_WriteByte(PG_SWCH, BIT2|BIT4);
            #ifdef TSUMXXNX
            mStar_WriteByte(PWMDIV1, 0xBE);
            mStar_WriteByte(PWMEXT, PWM1BIT8);
            #else
           mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) | PWMS); //PWM3
           mStar_WriteByte(PWMDIV2, 0xBE);
           mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) |PWM3BIT8);
            #endif    
	#elif(MainBoardType==MainBoard_715G3329_58N)
		mStar_WriteByte(PWMDIV1, 0xD0);
		mStar_WriteByte(PWMEXT,(mStar_ReadByte(PWMEXT)|BIT2));
	#elif(MainBoardType==MainBoard_715G3329_18N)
		mStar_WriteByte(PWMDIV1, 0xD0);
		mStar_WriteByte(PWMEXT,(mStar_ReadByte(PWMEXT)|BIT2));
	#elif(MainBoardType==MainBoard_712G3244_2)
	     mStar_WriteByte(PWMDIV1, 0xBE);
            mStar_WriteByte(PWMEXT, PWM1BIT8);
	#elif (MainBoardType==MainBoard_715G2564_B)
	    mStar_WriteByte(PWM0C, 0x0A);
	    mStar_WriteByte(PWM1C, 0x7F);
	    mStar_WriteByte(PWMDIV1, 0x17);
	    mStar_WriteByte(PWMEXT, 0x04);
            //mStar_WriteByte(PG_SWCH,0x05);
	#elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529)
             #if ModelName==AOC_e1620Swb||ModelName==AOC_e1621Swb_
                mStar_WriteByte(PWMDIV1, 0x2E);
        	//mStar_WriteByte( PWMEXT, PWM1BIT8);
		mStar_WriteByte(PWMDIV0A9, 0x01);
	     #else
	        mStar_WriteByte(PWMDIV1, 0xBE);
        	mStar_WriteByte( PWMEXT, PWM1BIT8);
	    #endif
        	//mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)&~BIT4&~BIT5)|BIT0 |BIT2|BIT6|BIT7);
	#elif (MainBoardType==MainBoard_715G4734)
                mStar_WriteByte(PWMDIV1, 0x2E);
        	//mStar_WriteByte( PWMEXT, PWM1BIT8);
		mStar_WriteByte(PWMDIV0A9, 0x01);
	#elif (MainBoardType==MainBoard_2670_C)   //Amy2007.12.18   9:48:35           
  	     mStar_WriteByte(PWMDIV0, 0x1F);//0xBE)//);//Amy2007.12.18   9:53:31
	     mStar_WriteByte(PWMEXT, PWM0BIT8);
            //mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH))|BIT1|BIT6|BIT3);
	     //mStar_WriteByte(0xC6,0x03); //For Vcom Init Value --->PWM2C  //nick 061129
    	     mStar_WriteByte(0xC8,0x03); //For Vcom Init Value --->for avoid VCOM error
    	     mStar_WriteByte(0xC4,0x03);//UserPref0XC7); //For Vcom Init Value --->PWM1C   //cy 1123
	     //mStar_WriteByte(0xC5,0xB5);//UserPref0XC7); //For Vcom Init Value --->PWM2C   //cy 1122
	     mStar_WriteByte(0xC5,0x95);
     	     mStar_WriteByte(0x9F, 0x10);
 	     mStar_WriteByte(0x5F, 0x00);
	#elif MainBoardType==MainBoard_2670_1
	mStar_WriteByte(REGBK, REGBANKSCALER);
       	//mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH))|BIT1|BIT6|BIT3); // Disable PWM2  
		mStar_WriteByte(0x64, 0); // Disable PWM2  
		mStar_WriteByte(0xC6,0x03); //For Vcom Init Value --->PWM2C  //nick 061129
    		mStar_WriteByte(PWMEXT,0x02); //For Vcom Init Value --->for avoid VCOM error
   		mStar_WriteByte(PWMDIV1, 0x03);
		mStar_WriteByte(0xC5,0x83);//UserPref0XC7); //For Vcom Init Value --->PWM2C 
 		 mStar_WriteByte(0x9F, 0x10);
 		 mStar_WriteByte(0x5F, 0x00);
	#elif (MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611)   //Amy2007.12.18   9:48:35           
  	     mStar_WriteByte(PWMDIV0, 0x1F);//Freq Set To 390Hz
	     mStar_WriteByte(PWMEXT, PWM0BIT8);
    	     mStar_WriteByte(PWMDIV1,0x03);//V-Com Freq Set To 27KHz,Request by manli
	     mStar_WriteByte(PWM1C,0x6D);//0x66);//0x66->5.6Volt	0x7E->5Volt 	//V-Com Init Value
     	     mStar_WriteByte(0x9F, 0x10);
 	     mStar_WriteByte(0x5F, 0x00);
        #elif MainBoardType == MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2
            mStar_WriteByte(PWMDIV1, 0x03); // 0x03: VCOM freq. 28K
            mStar_WriteByte(PWM1C, DefVcomValue); // VCOM value
            mStar_WriteByte(PWMDIV0, 0xD2); // Brightness freq. 240Hz
            mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT)|PWM0BIT8); // Brightness freq. 240Hz
            mStar_WriteByte(0x63, 0x4a);
            mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT)|PWMS);
            mStar_WriteByte(PWMDIV2, 0x00); // volume freq. 112K
        #elif MainBoardType==MainBoard_715G3598_M0A
            #ifdef TSUMXXNX
			#if ModelName==AOC_e2437Fw
		  mStar_WriteByte(PWMDIV1, 0x97);
			#else
                mStar_WriteByte(PWMDIV1, 0xBE);
			#endif
                #if ScanKey
                // LED bright PWM freq. 250Hz
                mStar_WriteByte(PWMDIV2, 0xBE); 
			#if ModelName==AOC_e2437Fw
		  mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) |PWM2BIT8);
			#else
                mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) |PWM1BIT8|PWM2BIT8);
			#endif
                #else
                mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) |PWM1BIT8);
                #endif
            #else
                mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) | PWMS); //PWM3
                mStar_WriteByte(PWMDIV2, 0xBE);
                #if ScanKey
                // LED bright PWM freq. 250Hz
                mStar_WriteByte(PWMDIV0, 0xbe); 
                mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) |PWM3BIT8|PWM0BIT8);
                #else
                mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) |PWM3BIT8);
                #endif
            #endif
        #elif MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
            // Brightness freq. 250Hz
            mStar_WriteByte(PWMDIV0, 0xbe); 
            #if ScanKey
            // LED bright PWM freq. 250Hz
            mStar_WriteByte(PWMDIV1, 0xbe); 
            mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT)|PWMS|PWM1BIT8|PWM0BIT8); // volume use pwm3
            #else
            mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT)|PWMS|PWM1BIT8|PWM0BIT8); // volume use pwm3
            #endif

            // volume freq. 110K
            mStar_WriteByte(VolumePWM-1, 0x00); 
        #elif (MainBoardType==MainBoard_715G3244_TSUMU18NR)
           mStar_WriteByte(0x63, BIT2);
           mStar_WriteByte(PWMDIV1, 0xAE); 
		#elif (MainBoardType==MainBoard_715G3598_1PLL)
		  //mStar_WriteByte(0x63, BIT2|BIT5);
          mStar_WriteByte(PWMDIV1, 0xBE);
          mStar_WriteByte(PWMEXT, PWM1BIT8); 
	#elif (MainBoardType==MainBoard_715G4175_M0A)
	     mStar_WriteByte(PWMDIV1, 0xBE);
            mStar_WriteByte(PWMEXT, PWM1BIT8);
		#elif MainBoardType == MainBoard_4432
        mStar_WriteByte( PWMDIV1, 0xBE );
         mStar_WriteByte( PWMEXT, PWM1BIT8 | BIT5 );	
		#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
		mStar_WriteByte(PWMDIV1, 0xBE);
		mStar_WriteByte(PWMEXT, PWM1BIT8);
		#elif (MainBoardType==MainBoard_715G3959_T)		//110617 Modify
		mStar_WriteByte(PWMDIV1, 0xBE);
		mStar_WriteByte(PWMEXT, PWM1BIT8);
		#elif (MainBoardType==MainBoard_715G5265_T||(MainBoardType == MainBoard_SLB1PTR||MainBoardType == MainBoard_JRYL1PTR_LF))	//petit 20121030	
		#if (Panel_NT156WHM_N12||PanelType==PanelAUOM215HW03)
		mStar_WriteByte(PWMDIV0, 0x15);
		mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) &(~BIT1));
		#endif
		mStar_WriteByte(PWMDIV1, 0xBE);
		mStar_WriteByte(PWMEXT, PWM1BIT8);
		#elif (MainBoardType==MainBoard_715G6952_M0B) || (MainBoardType==MainBoard_715G6912_M1A)	//Gino 20140522
			mStar_WriteByte(REGBK, REGBANKADC);
			mStar_WriteByte(BK1_C9, mStar_ReadByte(BK1_C9)|PWM6BIT8);
			mStar_WriteByte(BK1_C8, 0xBE);
			mStar_WriteByte(REGBK, REGBANKSCALER);
		#elif (MainBoardType==MainBoard_3834_18T)	//petit 20121228
			mStar_WriteByte(PWMEXT, PWM1BIT8);
			mStar_WriteByte(PWMDIV1, 0xBE);
		
			#if AudioFunc
            mStar_WriteByte(PWMDIV0, 0x00);
            mStar_WriteByte(PWMEXT, PWM0BIT8);
			#endif
	    #elif (MainBoardType==MainBoard_715G3834)
            mStar_WriteByte(PWMEXT, PWM1BIT8);
            mStar_WriteByte(PWMDIV1, 0xBE);
            #if AudioFunc
            #if (ModelName == ENV_P971WALS_TPM185)
            mStar_WriteByte(PWMDIV0, 0x0A);      //set the volume PWM fre to about 10K Hz.
            #endif
            #endif
        #else
            mStar_WriteByte(PWMDIV1, 0xBE);
            mStar_WriteByte(PWMEXT, PWM1BIT8);
            //mStar_WriteByte(PG_SWCH, (mStar_ReadByte(PG_SWCH)) | BIT0 | BIT2 | BIT4 | BIT6); //For P2_5  set as I-O Pin 2006-06-23 Andy For pwm2c
        #endif
	#if EnablePanelServiceMenu
		SetPWMFreq(PanelBurstModeFreq);
	#endif
    #endif
    Osd_InitOSD();

    #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
   // I_Gen_Tuning();
    Set_AC_PowerFlag(); // jeff add in 2009 1109
    #endif

    #if DisplayPowerOnLogo
    if (DisplayLogoFlag)
    {
        Menu_InitAction();
    }
    #endif
    //mStar_WriteDDC1(); // programming DDC1 inside chip
    //mStar_WriteDDC2(); // programming DDC2 inside chip
    mStar_InitGamma(); // programming gamma table
    /************
    // Zhifeng.Wu		2007-6-1 16:55:22
    Cancel By Zhifeng.Wu For Power On Audio output
    #if (AudioKeil==1)
    Delay1ms( 100 );
    mStar_AdjustVolume( UserPrefVolume );
    #endif
     ************/
    // Init_MWE();
    #if MWEFunction
    InitialPeaking();
	#ifdef TSUMXXT	// 110419 Modify
	mStar_WriteByte(REGBK, REGBANK3);
	mStar_WriteByte(BK3_3B,mStar_ReadByte(BK3_3B)&(~BIT0));
	mStar_WriteByte(REGBK, REGBANKSCALER);
	#endif
	#if defined(TSUMXXQX)||defined(TSUMXXMX)//modify 20121221
	mStar_WriteByte(REGBK, REGBANK3);
	mStar_WriteByte( BK3_75, 0x00 );
	mStar_WriteByte( BK3_76, 0x00 );
	mStar_WriteByte(REGBK, REGBANKSCALER);
	#endif
    //SetBFSize();
    InitACEVar(); // Initialize the variable at ACE.c
    InitDLCVar();
    #if HDMICOLORTEST
    InitHDMIACEVar();
    #endif

    #if EN_HDMI_NEW_PIXEL_REPETITION
    mStar_WriteByte(REGBK, 0x00);
    mStar_WriteByte(BK0_7A,(mStar_ReadByte(BK0_7A)|BIT7));
    #endif


        // 2006/10/26 6:46PM by Emily     msAdjustSharpness(UserPrefSubSharpness, 1);
        //As below function will be set in mstar_setuserpref()
        /*
        msAdjustSharpness(11, 1);
        msAdjustSubBrightness( UserPrefSubBrightness );
        msAdjustVideoContrast( UserPrefSubContrast );
        msAdjustVideoHue( UserPrefHue );
        msAdjustVideoSaturation( UserPrefSaturation );
         */
	#ifndef TSUMXXT		//110608 Modify
    mStar_WriteByte(REGBK, REGBANK3);
    mStar_WriteByte(BK3_3B, (mStar_ReadByte(BK3_3B)) | BIT7);       //Lizzie.ding      OCT-28-08   for subwin sharpness
    mStar_WriteByte(REGBK, REGBANKSCALER);
	#endif
    #endif

#ifdef TSUMW88MDT3_For_HDMIDDCCI///yuanzhi20100810_2 for VGA Port AC_On inset the HDMI cable that connect to DVD and have sync EXT0 Timingchange..
    mStar_WriteByte(REGBK, 0x0B);
    mStar_WriteByte(0xA0, 0xFF);      
    mStar_WriteByte(REGBK, REGBANKSCALER);
#endif
	
	
#ifdef PowerSavingAmberLEDFlashing
    #if PSAmberLEDFlashingType == Type_Breathing
        #if MainBoardType == MainBoard_715G3329_A2  || MainBoardType == MainBoard_715G3244_TSUMU18NR|| MainBoardType ==MainBoard_712G3244_2 || MainBoardType == MainBoard_715G3959_T || MainBoardType == MainBoard_715G3834||MainBoardType ==MainBoard_3834_18T
			InitialP06ToPWM1(FALSE);
        #endif
    #endif
#endif
	#ifdef TSUMXXT//#if ChipID == TSUMXXT		//110311 Modify
    mStar_WriteByte(REGBK, REGBANK3);
    mStar_WriteByte(BK3_3B, (mStar_ReadByte(BK3_3B)) & (~BIT7));       //110608 Modify for T-Type sharpness peaking
    mStar_WriteByte(REGBK, REGBANKSCALER);
	mStar_InitIHC();	// set initial default values of color engine: IHC
	mStar_InitDither3D();  // set initial values of color engine: 3D dither
	#endif
    mStar_SetUserPref();
	UnUseFunc();
#if DISABLE_SHORT_FRAME_TUNE	//petit 20121030
#ifdef TSUMXXT
	mStar_WriteByte(REGBK, REGBANK1);
	mStar_WriteByteMask( BK1_20, BIT7, BIT7 );	  // enable test mode to set BK1_12 and BK1_24
	mStar_WriteByte( BK1_3E, 0x10 );
	mStar_WriteByteMask( BK1_20, 0, BIT7 );	  // enable test mode to set BK1_12 and BK1_24
       mStar_WriteByte(REGBK, REGBANK0);
#endif
#endif

#if IT6251
/*******************************************************************/	//Disable Short Line 090630  Issue:DCLK Auto Change
       mStar_WriteByte(REGBK, REGBANKADC);
       mStar_WriteByte(TESTEN,TSTEN_B);
       //eDP_Flick mStar_WriteByte(0x3E,mStar_ReadByte(0x3E)|(BIT2));
       //eDP_Flick mStar_WriteByte(0x3E,mStar_ReadByte(0x3E)&(~BIT0));
       mStar_WriteByte(0x3E, 0xD4); //mStar_WriteByte(0x3E, 0xC4);//eDP_Flick
       mStar_WriteByte(TESTEN,0);
       mStar_WriteByte(REGBK, REGBANKSCALER);
       mStar_WriteByte(0x48,mStar_ReadByte(0x48)|(BIT3));
       //eDP_Flick mStar_WriteByte(0x28,mStar_ReadByte(0x28)&(~BIT6));
       mStar_WriteWord(0x6E,PanelMinHTotal-0x100);  //eDP_Flick
       mStar_WriteByte(0x6C,0x90);  //eDP_Flick
       mStar_WriteByte(0x28,mStar_ReadByte(0x28)|(BIT6)); //eDP_Flick
       #ifdef TSUMXXT
       mStar_WriteByte(BK0_27, (mStar_ReadByte(BK0_27)) & (~BIT5)); 
	#endif
/*******************************************************************/
#endif	

}

void mStar_InterlaceModeSetting(void)
{
    BYTE regBank = mStar_ReadByte(0x00);
    
    mStar_WriteByte(REGBK, REGBANKSCALER);
    if(SrcFlags&bInterlaceMode)
    {
	#ifdef NewInterlaceShiftLineRule   
	if(mStar_ReadByte(0x04)&BIT6)    //edit by yz.nie 100526
	{
		#if (INPUT_TYPE == INPUT_1A2D)
		{
			if(SrcInputType == Input_Digital2)
		{
			if(StandardModeGroup == Res_720x480)
			{
				mStar_WriteByte(INTMDS, 0x13);
			}
			else
			{
				mStar_WriteByte(INTMDS, 0x00);
			}
		}
		else
		{
			mStar_WriteByte(INTMDS, 0x00);
		}
	}
	#elif(INPUT_TYPE == INPUT_1A1H)
	{
		if(SrcInputType == Input_Digital)
		{
			if(StandardModeGroup == Res_720x480)
			{
				mStar_WriteByte(INTMDS, 0x13);
			}
			else
			{
				mStar_WriteByte(INTMDS, 0x00);
			}			
		}
		else
		{
			mStar_WriteByte(INTMDS, 0x00);
		}		
	}
	#elif(INPUT_TYPE == INPUT_1A1D)
	{
		mStar_WriteByte(INTMDS, 0x00);
	}
	#else///INPUT_1A
		{
			mStar_WriteByte(INTMDS, 0x00);
		}
		#endif
	}
	else       //edit by yz.nie 100526
	{
		mStar_WriteByte(INTMDS, 0x00);
	}
	
	#else
	
	mStar_WriteByte(INTMDS, 0x01);
	
	#endif
		
        if(SrcInputType == Input_Analog1)
        {
            mStar_WriteByte(MDCTRL, mStar_ReadByte(MDCTRL)|BIT0);
        }
    #if INPUT_TYPE != INPUT_1A        
        else
        {
            if(mStar_ReadByte(IPCTRL2)&DEON_B) // DE mode
                mStar_WriteByte(MDCTRL, mStar_ReadByte(MDCTRL)&~BIT0);
            else // HV mode
                mStar_WriteByte(MDCTRL, mStar_ReadByte(MDCTRL)|BIT0);
        }
    #endif    

    
        

    #if MS_OVD
     #if 1 // 100604 coding for interlace mode setting, wrong setting may cause OD works abnormally.
     
             mStar_WriteByte(INTMDS, 0x00);
         #ifdef TSUMXXKX
             msWriteByteMask(DIMD, BIT3|BIT1, BIT3|BIT2|BIT1);
         #else
             msWriteByteMask(LVL, BIT7, BIT7|BIT6);
             msWriteByteMask(DIMD, BIT3, BIT3);
         #endif    
     
     #else	
             mStar_WriteByte(0x00, 0x04);
             mStar_WriteByte(0xaf, 0x80);
             mStar_WriteByte(0xbf, 0x18);
     #endif		
    #endif

	
	
    }
    else
    {
	//mStar_WriteByte(INTMDS, mStar_ReadByte(INTMDS)&~BIT0);

	
	mStar_WriteByte(INTMDS, 0x00);
	mStar_WriteByte(MDCTRL, mStar_ReadByte(MDCTRL)&~BIT0);

    #if MS_OVD
       #if 1 // 100604 coding for progressive mode setting
  
        #ifdef TSUMXXKX
        msWriteByteMask(DIMD, 0, BIT3|BIT2|BIT1);
        #else
        msWriteByteMask(LVL, 0, BIT7|BIT6);
        msWriteByteMask(DIMD, 0, BIT3);
        #endif
       
       #else	
       mStar_WriteByte(0x00, 0x04);
       mStar_WriteByte(0xaf, 0xc0);
       mStar_WriteByte(0xbf, 0x00);
       #endif
	#endif
    }
    
    mStar_WriteByte(REGBK, regBank);
    
}

#define FIXCLK (172-3)
void mStar_CheckFastClock(void)
{

	DWORD factor;
	WORD dclk;
	WORD sclk;
	WORD hPeriod;
	WORD Span;
	WORD Step;
	WORD Percent_Value;
	#ifdef TSUMXXT    //110401 Modify
    volatile DWORD             u32MPLL_KHZ;
    BYTE u8LPLL_LOOP_DIV;
	#endif

	if (SrcHPeriod>511) // avoid overflow
		hPeriod=SrcHPeriod*16;
	else
	{
		sclk=GetVSyncTime()*4;///3
		mStar_WriteByte(HSPRD_H, IHDM_B); // enable 16 line line to calculate hsync period.
		Delay1ms(sclk);
		hPeriod=mStar_ReadWord(HSPRD_H)&0x1FFF;
		if (abs(hPeriod/16-SrcHPeriod)>5)
			hPeriod=SrcHPeriod*16;
		mStar_WriteByte(HSPRD_H, 0);
		Delay1ms(sclk);
	}

	if(SrcInputType>Input_Analog1)	
		sclk = ((DWORD) StandardModeHTotal*MST_HPeriod_UINT)/((DWORD)hPeriod*1000);
	else
		sclk = ((DWORD) UserPrefHTotal *MST_HPeriod_UINT)/((DWORD)hPeriod*1000);

	factor=mStar_ReadByte(LPLL_SET_H);
	factor=(factor<<16)|mStar_ReadWord(LPLL_SET_M);
	Span=mStar_ReadWord(LPLL_SPAN_H);
	Step=mStar_ReadWord(LPLL_STEP_H)&0x7FF;
	Percent_Value=((DWORD)Span*Step*10000)/(factor);
	#ifdef TSUMXXT    //110401 Modify
	//@todo Change the following computation to compile-time, not run-time
	u32MPLL_KHZ = ( mStar_ReadByte(BK0_D9) & BIT3 )? 214770UL : 429540UL;  // 214.77MHz or 429.54MHz
	u8LPLL_LOOP_DIV = (mStar_ReadByte(BK0_D9)&(BIT7|BIT6|BIT5|BIT4))>>4;
	dclk = ((float)(u32MPLL_KHZ / 7) * ((float)524288UL * (float)u8LPLL_LOOP_DIV * 2 /factor ))/1000;
	#else
	dclk=(DWORD)DClkFactor/factor;		// DClkFactor =257374477  for LVDS,  DClkFactor =225202667	for RSDS
	#endif

	dclk=dclk+((WORD)dclk*Percent_Value)/10000;

	#ifdef TSUMXXT    //110401 Modify
	if ( !(mStar_ReadByte(OCTRL2)&DPO_B))
		dclk /= 2;
	#endif

	mStar_WriteByte(REGBK, REGBANKADC);
	msWriteByteMask(BK1_68, 0, BIT5);// Disable select 214MHz
	mStar_WriteByte(REGBK, REGBANKSCALER);

	#ifdef TSUMXXT    //110401 Modify
	#else
	msWriteByteMask(BK0_E9, BIT7, BIT7);
	msWriteByteMask(PLLCTRL2, BIT7, BIT7);
	#endif

	if(sclk < FIXCLK && dclk < FIXCLK)
	{
		msWriteByteMask(VDSUSL, BIT7, BIT7|BIT6);
	}
	else if(sclk >= FIXCLK && dclk >= FIXCLK)
	{
		if(abs(sclk - dclk) <= 4)
		{
			mStar_WriteByte(REGBK, REGBANKADC);
			msWriteByteMask(BK1_68, BIT5, BIT5);// Enable select 214MHz
			mStar_WriteByte(REGBK, REGBANKSCALER);
			msWriteByteMask(BK0_5B, BIT7, BIT7|BIT6);
		}
		else if(sclk > dclk)
			msWriteByteMask(VDSUSL, BIT7|BIT6, BIT7|BIT6);
		else
			msWriteByteMask(VDSUSL, BIT6, BIT7|BIT6);
	}
	else if(sclk >= FIXCLK)
		msWriteByteMask(VDSUSL, BIT7|BIT6, BIT7|BIT6);
	else if(dclk >= FIXCLK)
		msWriteByteMask(VDSUSL, BIT6, BIT7|BIT6);

}

#ifdef TSUMXXNX
void ClrLineBuffer(void)
{
    BYTE u8Reg15=mStar_ReadOSDByte(GCCTRL);
    BYTE u8Reg16=mStar_ReadOSDByte(GRADCLR);
    BYTE u8Reg17=mStar_ReadOSDByte(HGRADCR);
    BYTE u8Reg18=mStar_ReadOSDByte(HGRADCG);
    BYTE u8Reg19=mStar_ReadOSDByte(HGRADCB);
    BYTE u8Reg1D=mStar_ReadOSDByte(VGRADCR);
    BYTE u8Reg1E=mStar_ReadOSDByte(VGRADCG);
    BYTE u8Reg1F=mStar_ReadOSDByte(VGRADCB);
    BYTE u8Reg02=mStar_ReadOSDByte(DBFC);
    WORD u16Width=mStar_ReadWord(SPRHDC_H);
    WORD u16VSyncTime=GetVSyncTime()+3;

	
    mStar_WriteOSDByte(DBFC, 0x00);
    mStar_WriteOSDByte(GRADCLR,0x00); //disable new color
    mStar_WriteOSDByte(HGRADCR,0);
    mStar_WriteOSDByte(HGRADCG,0);
    mStar_WriteOSDByte(HGRADCB,0);
    mStar_WriteOSDByte(VGRADCR,0);
    mStar_WriteOSDByte(VGRADCG,0);
    mStar_WriteOSDByte(VGRADCB,0);
    mStar_WriteOSDByte(GCCTRL,0x0C);

    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByteMask(BK1_C0,BIT4,BIT4); //Thru ADC
    mStar_WriteByte(REGBK, REGBANKSCALER);

    mStar_WriteWord(SPRHDC_H, u16Width+2);
    Delay1ms(u16VSyncTime);
    mStar_WriteWord(SPRHDC_H, u16Width);
    Delay1ms(u16VSyncTime); //must add delay here

    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByteMask(BK1_C0,0,BIT4);
    mStar_WriteByte(REGBK, REGBANKSCALER);
	
//Recover Reg
    mStar_WriteOSDByte(GCCTRL,u8Reg15);
    mStar_WriteOSDByte(GRADCLR,u8Reg16);
    mStar_WriteOSDByte(HGRADCR,u8Reg17);
    mStar_WriteOSDByte(HGRADCG,u8Reg18);
    mStar_WriteOSDByte(HGRADCB,u8Reg19);
    mStar_WriteOSDByte(VGRADCR,u8Reg1D);
    mStar_WriteOSDByte(VGRADCG,u8Reg1E);
    mStar_WriteOSDByte(VGRADCB,u8Reg1F);
    mStar_WriteOSDByte(DBFC, u8Reg02);
	
    Delay1ms(u16VSyncTime*2); //must add delay here
}
#endif

//*******************************************************************
// Function Name: mStar_SetupMode
//
// Decscription: setup registers for input timing,
// return      : TRUE,
// caller: mStar_SetupADC(), mStar_SetupCaptureWindow(),
//         mStar_SetScalingFactor(), mStar_SetPanelTiming(),
//         mStar_SetUserPref() in mstar.c
//         mStar_FineTuneDVIPhase(), mStar_ValidTimingDetect() in detect.c
//         mSar_WriteByte(), mStar_ReadByte() in ms_rwreg.c
// callee: mStar_ModeHandler() in detect.c
//*******************************************************************
Bool mStar_SetupMode(void)
{
    Clr_FreeRunModeFlag();
	#if 0//ESaverPowerDownFunc
    ESaverPowerDownCounter=0;
	#endif
    mStar_SetupADC(); // setup ADC block, including polarity & htotal, phase, vco
    if (mStar_SetCaptureWindow() == FALSE)
    // setup capture window
    {
        //      printMsg("Err_1");
        return FALSE;
    }
	
	#if Enable_Expansion
	mStar_SetScalingFactor(UserprefExpansionMode); // setup scaling factor
	#else
	mStar_SetScalingFactor(); // setup scaling factor
	#endif

#if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
    mStar_SetScalingFilter();
#else
    if (mStar_ReadWord(SPRHDC_H) == PanelWidth && mStar_ReadWord(SPRVDC_H) == PanelHeight)
    {
        mStar_SetScalingFilter(0); // programming sharpness
    }
    else
    {
    #if ModelName==ENVISION_P2271wl||ModelName==ENVISION_P2271wl1
        mStar_SetScalingFilter(3);
     #elif ModelName==ENVISION_P2471
	 mStar_SetScalingFilter(1);
     #else
        mStar_SetScalingFilter(2); //UserPrefSharpness); // programming sharpness
     #endif
    }
#endif

    if (mStar_SetPanelTiming() == FALSE)
    // set output dclk
    {
        SrcFlags |= bUnsupportMode;
        //    printMsg("Err_2");
        return TRUE;
    }
	
#if ENABLE_HDMI
    #if !EN_HDMI_NEW_PIXEL_REPETITION
			{
				BYTE reg_bank;
				// 080418
				PreHDMIColor=mstar_HDMIPacketColor();
				if((mstar_HDMITmdsGetType()==TMDS_HDMI)&&(mstar_HDMIPacketColor()==INPUT_YUV422) &&
					(StandardModeGroup == Res_720x480 || StandardModeGroup == Res_720x576)) // 480i, 576i
				{
					BYTE tempcnt=0;
					WORD tempHStart;
					EX0=0;
					reg_bank=mStar_ReadByte(0x00);
					mStar_WriteByte(0x00,0x05);
					mStar_WriteByte(0x09,0x00);
					ForceDelay1ms(100);	//100303 : for DVD 480i YUV422
					mStar_WriteByte(0x00,reg_bank);
					EX0=1;
	
					do{
							tempcnt++;
							tempHStart = mStar_ReadWord(AOHST_H) &0x7FF;
							Delay1ms(30);
					}while((abs(SrcVTotal - (mStar_ReadWord(VTOTAL_H)&0x7FF)) > 1 ||
								 abs(tempHStart - (mStar_ReadWord(AOHST_H) &0x7FF)) > 1) &&
								tempcnt < 30);	
				}
			}
    #endif
#endif

    mStar_WriteWord(SPRHST_H, UserPrefHStart); // set capture window hstart
    if (!DecVScaleFlag)
    // 2006/11/1 8:43PM by Emily for can't set vstart again while set decvscaleflag
    {
	     #ifdef NewInterlaceShiftLineRule   
		if(SrcFlags&bInterlaceMode)	 
	        {
	            if(mStar_ReadByte(0x04)&BIT6)
	            {
				#if (INPUT_TYPE == INPUT_1A2D)
				{
					if(SrcInputType == Input_Digital2)
					{
						if(StandardModeGroup == Res_720x480)
						{
							mStar_WriteWord(SPRVST_H, UserPrefVStart-2);
						}
						else
						{
							mStar_WriteWord(SPRVST_H, UserPrefVStart);
						}
					}
					else
					{
						mStar_WriteWord(SPRVST_H, UserPrefVStart);
					}
				}
				#elif(INPUT_TYPE == INPUT_1A1H)
				{
					if(SrcInputType == Input_Digital)
					{
						if(StandardModeGroup == Res_720x480)
						{
							mStar_WriteWord(SPRVST_H, UserPrefVStart-2);
						}
						else
						{
							mStar_WriteWord(SPRVST_H, UserPrefVStart);
						}				
					}
					else
					{
						mStar_WriteWord(SPRVST_H, UserPrefVStart);
					}			
				}
				#elif(INPUT_TYPE == INPUT_1A1D)
				{
					mStar_WriteWord(SPRVST_H, UserPrefVStart);
				}
				#else///INPUT_1A
				{
					mStar_WriteWord(SPRVST_H, UserPrefVStart);
				}
					#endif
			
	            }
		     else
		     {
				mStar_WriteWord(SPRVST_H, UserPrefVStart);
		     }
	    	}
		else
	     {
			mStar_WriteWord(SPRVST_H, UserPrefVStart);
	     }
	     #else /// ! NewInterlaceShiftLineRule
		 
             mStar_WriteWord(SPRVST_H, UserPrefVStart);
		 
	     #endif ///NewInterlaceShiftLineRule   end
    	
    }
    // set capture window vstart
    //  fine Tune DVI phase to get more stable DVI
#if EnableAutoDVI
    if (SrcInputType == Input_Digital  || SrcInputType==Input_Digital2)
    {
        BYTE dviPhase;
        mStar_WriteByte(REGBK, REGBANKADC); // switch to ADC bank
        // fine turn DVI phase--Red Channel
        dviPhase = mStar_FineTuneDVIPhase(DVI_PHR, RED_CHANNEL);
        if (dviPhase < 0xFF)
        {
            mStar_WriteByte(DVI_PHR, DVIPhaseTbl[dviPhase]);
        }
        // fine turn DVI phase--Green Channel
        dviPhase = mStar_FineTuneDVIPhase(DVI_PHG, GREEN_CHANNEL);
        if (dviPhase < 0xFF)
        {
            mStar_WriteByte(DVI_PHG, DVIPhaseTbl[dviPhase]);
        }
        // fine turn DVI phase--Blue Channel
        dviPhase = mStar_FineTuneDVIPhase(DVI_PHB, BLUE_CHANNEL);
        if (dviPhase < 0xFF)
        {
            mStar_WriteByte(DVI_PHB, DVIPhaseTbl[dviPhase]);
        }
        mStar_WriteByte(REGBK, REGBANKSCALER); // switch to Scaler bank
       
    }
#endif        
    
    mStar_WriteWord(VDTOT_H, 0x7FF);

    // 100513 coding, moves to here to prevent timing change causes white screen.
    mStar_CheckFastClock();
	
    #ifdef TSUMXXNX
	ClrLineBuffer(); //before double buffer enable
	#endif

    if (InputTimingChangeFlag || mStar_ValidTimingDetect())
    // check if input timing has changed
    {
        //printMsg("Err3");
        return FALSE;
    }
    #if ChipID==ChipBB
        CheckLineBuffer();
    #endif

    //mStar_CheckFastClock();

    // enable double buffer
    mStar_WriteByte(DBFC, 0x05); // enable double bufer.
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(DBFC, 0x01); // enable ADC's double bufer.
    mStar_WriteByte(REGBK, REGBANKSCALER);
    // 2006/10/16 12:38AM by Emily     mStar_SetUserPref(); // restore user setting
    // use interrupt to speedup mode changing while input timing is changing
    #if UseINT
        if (SrcInputType < Input_Digital)
        {
            mStar_WriteByte(INTENA, 0xB0);
            mStar_WriteByte(INTENB, 0x00);	//110311 Modify
        }
        else
        {
            mStar_WriteByte(INTENA, 0x30);
            mStar_WriteByte(INTENB, 0x04);	//110311 Modify		
        } // for YCbCr
    #endif

    mStar_InterlaceModeSetting();

  #if MS_OVD
	if(OverDriveOnFlag)
		msOD_OnOff(1,0x5f);	
	else
		msOD_OnOff(0,0x50);	
  #endif
if(UserPrefDcrMode)//20151225 lixw 
	Set_DcrOpenScikeFlag();
 
    mStar_SetUserPref(); // restore user setting // 2006/10/16 12:38AM by Emily
    return TRUE;
}
#ifdef TSUMXXT		//110311 Modify
//*******************************************************************
// Function Name: mStar_LPLL_GetDividers
//
// Decscription: Get output divider and loop divider of new LPLL from
//                  output clock in kHz.
//*******************************************************************
static void
mStar_LPLL_GetDividers (const DWORD        kHzODCLK,
                        LPLL_INPUT_DIV_t*  pInputDiv,
                        LPLL_OUTPUT_DIV_t* pOutputDiv,
                        BYTE*              pLoopDiv)
{
    LPLL_OUTPUT_DIV_t outputDiv;
    BYTE              loopDiv;

#if( PanelDualPort )
    if( kHzODCLK <= 50*1000UL )  // 40MHz~50MHz
    {
        outputDiv = LPLL_OUTPUT_DIV_4;
        loopDiv   = 1;    // divide by 1
    }
    else if( kHzODCLK <= 100*1000UL )    // 50MHz~100MHz
    {
        outputDiv = LPLL_OUTPUT_DIV_2;
        loopDiv   = 2;    // divide by 2
    }
    else    // 100MHz~200MHz
    {
        outputDiv = LPLL_OUTPUT_DIV_1;
        loopDiv   = 4;    // divide by 4
    }
#else
    if( kHzODCLK <= 50*1000UL )  // 40MHz~50MHz
    {
        outputDiv = LPLL_OUTPUT_DIV_2;
        loopDiv   = 2;    // divide by 2
    }
    else    // 50MHz~100MHz
    {
        outputDiv = LPLL_OUTPUT_DIV_1;
        loopDiv   = 4;    // divide by 4
    }
#endif

    if( pInputDiv != NULL )
    {
        *pInputDiv = LPLL_INPUT_DIV_1;
    }

    if( pOutputDiv != NULL )
    {
        *pOutputDiv = outputDiv;
    }

    if( pLoopDiv != NULL )
    {
        *pLoopDiv = loopDiv;
    }
}

//*******************************************************************
// Function Name: mStar_LPLL_GetFactor
//
// Decscription: Get LPLL factor settings.
//*******************************************************************
static void
mStar_LPLL_GetFactor (const DWORD        kHzODCLK,
                      const BYTE         loopDiv,
                      DWORD*             pFactor)
{
    DWORD factor;
    volatile BYTE  origBank = mStar_ReadByte( REGBK );
    volatile DWORD kHzMPLL;


    mStar_WriteByte( REGBK, REGBANKSCALER );

    //@todo Change the following computation to compile-time, not run-time
    kHzMPLL = ( mStar_ReadByte(BK0_D9) & BIT3 )? 214770UL : 429540UL;  // 214.77MHz or 429.54MHz

    factor = (float)(kHzMPLL / 7) * ((float)524288UL * (float)loopDiv * 2 / kHzODCLK);

#if (!PanelRSDS && !PanelDualPort)
    factor/=2;
#endif

    if( pFactor != NULL )
    {
        *pFactor = factor;
    }

    mStar_WriteByte( REGBK, origBank );   // restore original bank

}

#endif  // end of #if( ChipID == CHIP_TSUMT )
//*******************************************************************
// Function Name: mStar_SetupFreeRunMode
//
// Decscription: setup registers for free run mode without any input timing,
//
// caller: mStar_ResetDClkPLL() in mstar.c
//         mSar_WriteByte(), mStar_ReadByte(), mStar_WriteWord() in ms_rwreg.c
// callee: mStar_ModeHandler() in detect.c
//*******************************************************************
void mStar_SetupFreeRunMode(void)
{
    DWORD factor;
#ifdef TSUMXXT		//110311 Modify
	const	 DWORD			   kHzODCLK = (DWORD)PanelDCLK * 1000;
			 LPLL_INPUT_DIV_t  lpllInputDiv;
			 LPLL_OUTPUT_DIV_t lpllOutputDiv;
			 BYTE			   lpllLoopDiv;
	const	 BYTE			   LPLL_ICTL = 3;
#endif  // end of #if( ChipID == CHIP_TSUMT )

    mStar_WriteByte(ISELECT, mStar_ReadByte(ISELECT) | NIS_B);
    // disable output Lock mode to enable free run.
#ifdef TSUMXXT		//110311 Modify
	mStar_LPLL_GetDividers( kHzODCLK, &lpllInputDiv, &lpllOutputDiv, &lpllLoopDiv );  // get divider settings
	mStar_LPLL_GetFactor  ( kHzODCLK,  lpllLoopDiv,  &factor );   // compute factor from divider settings
#else
    factor = FreeRunDClk;
    #if 1//!EnablePanelServiceMenu
    #if (!PanelRSDS)// && !PanelDualPort)
	if (!(mStar_ReadByte(OCTRL2)&DPO_B))
        factor /= 2;
    #endif
    #endif
#endif
    mStar_WriteWord(LPLL_SET_M, factor &0xFFFF);
    mStar_WriteByte(LPLL_SET_H, factor >> 16); // setup output dot clock
    mStar_ResetDClkPLL(); // reset output PLL
    //  mStar_WriteByte(LPLL_SET_H, 0x24);
#ifdef TSUMXXT		//110311 Modify
	// Setting these dividors must placed after LPLL_SET, to ensure stable LPLL clock output.
	mStar_WriteByte(REGBK, REGBANKSCALER);
	mStar_WriteByteMask( BK0_D3, (lpllOutputDiv<<2)|(lpllInputDiv), (BIT3|BIT2)|(BIT1|BIT0) );   // BK0_D3[3:2]=LPLL_OUTPUT_DIV;	BK0_D3[1:0]=LPLL_INPUT_DIV
	mStar_WriteByteMask( BK0_D4, (LPLL_ICTL), (BIT2|BIT1|BIT0)	  );		  // BK0_D4[2:0] = LPLL_ICTL
	mStar_WriteByteMask( BK0_D9, (lpllLoopDiv  <<4), (BIT7|BIT6|BIT5|BIT4) );   // BK0_D9[7:4] = LPLL_LOOP_DIV
#endif  // end of #if( ChipID == CHIP_TSUMT )
    mStar_WriteWord(VDTOT_H, PanelVTotal - 1); // set output panel vtotal
    mStar_WriteWord(HDTOT_H, PanelHTotal - 1); // set output panel htotal
    Set_FreeRunModeFlag();
	
#ifdef PanelSSCFunction
	mStar_SetPanelSSC();    
#endif
    // printData("enter free run mode %d", SrcInputType);
}
//*******************************************************************
// Function Name: mStar_SetAnalogInputPort
//
// Decscription: setup registers for Separate Sync/Composite Sync/SOG,
//
// caller: mSar_WriteByte(), mStar_ReadByte() in ms_rwreg.c
//
// callee: mStar_MonitorInputTiming() in detect.c
//*******************************************************************
void mStar_SetAnalogInputPort(void)
{
    BYTE regValue = mStar_ReadByte(ISELECT);
    if (regValue &COMP_B)
    // current is SOG
    {
        regValue = regValue &0x8F; // swtich to auto-detect
    }
    else
    {
        //regValue = regValue | 0x70; // switch to SOG-detect
        regValue = regValue &0x8F; // swtich to auto-detect
    }
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT ) // TSUMXXT uses new ADC driver		//110311 Modify
	msADC_SetADCSource(ADC_TABLE_SOURCE_RGB);	// input is RGB
	#endif
    mStar_WriteByte(ISELECT, regValue);
    mStar_WriteByte(SWRST, GPR_B | ADCR_B); // enable software reset function to clear ADC & Graphic port RO register
    Delay1ms(2);
    mStar_WriteByte(SWRST, 0); // disable software reset
    Delay1ms(80); // delay over 1 frame time to wait for status register is ready
}
//*******************************************************************
// Function Name: mStar_SetupInputPort
//
// Decscription: setup input port registers for
//               Analog/Digital/YCbCr(Video) input
//
// caller: mSar_WriteByte(), mStar_ReadByte() in ms_rwreg.c
// callee: mStar_MonitorInputTiming() in detect.c
//*******************************************************************
void mStar_SetupInputPort(void)
{
    BYTE regByte;
    //  printData("switch input port %d", SrcInputType);
#if 0//DisplayInputInfo
    Set_ShowInputInfoFlag();
#endif
    // printData("Set_ShowInputInfoFlag",0);

#if 1
    regByte = mStar_ReadByte(ISELECT) & NIS_B;
#else
    #if ENABLE_HDCP
        mStar_WriteByte(BK0_BD, mStar_ReadByte(BK0_BD) | 0x80); // Eric, 20060630
    #endif
    
    #if ENABLE_HDMI
    regByte = mStar_ReadByte(ISELECT) &NIS_B;
    #else
    regByte = mStar_ReadByte(ISELECT) &(NIS_B | IHSU_B);
    #endif
#endif
    
    if (SrcInputType < Input_Digital)
    {
        regByte |= (SrcInputType == Input_Analog1) ? (ISEL_Analog1): (ISEL_Analog2);
		#ifdef TSUMXXT//#if( ChipID == TSUMXXT ) // TSUMXXT uses new ADC driver		//110311 Modify
		msADC_SetInputMux(Input_Analog1);
		mStar_SetAnalogInputPort();//mStar_SetAnalogInputPort( FALSE );	  // switch to RGB port by default
		#else
        // setup ADC bank register
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte( GCTRL, 0x2);//eshin 070621 0x6 ); // Coast Polarity as high
        mStar_WriteByte(VCOCTRL, 0x15); //D); // set VOC
        mStar_WriteByte(SOG_LVL, 0x10); // set SOG level
        // enable test mode to program
        mStar_WriteByte(TESTEN, TSTEN_B);
        //****INPORTANT must enable test mode for programming the following registers
        #if ADC2Sets
            if (PowerSavingFlag)
            {
                if (SrcInputType == Input_Analog1)
                {
                    mStar_WriteByte(TESTA5, 0x9E);
                } //mStar_ReadByte(TESTA5)&(~AMUX_B));
                else
                {
                    mStar_WriteByte(TESTA5, 0xBE);
                } //mStar_ReadByte(TESTA5)|AMUX_B);
            }
        #endif
        mStar_WriteByte(TESTA0, 0x00); //set pll charge pump current for ADC
        mStar_WriteByte(TESTEN, 0); // disable test mode
        mStar_WriteByte(PLLCTRLV, 0x95); //0513  0x00);//0x95);
		#endif	// end of #if( ChipID == TSUMXXT )		//110311 Modify
        // setup Scaler bank
        mStar_WriteByte(REGBK, REGBANKSCALER);
        mStar_WriteByte(IPCTRL2, DEON_B | VSE_B);
        mStar_WriteByte(LVL, 0);
        mStar_WriteByte(INTMDS, 0x00); // Interlace mode line shift
        mStar_WriteByte(COCTRL1, 0x01); // enable ADC coast
        mStar_WriteByte(COCTRL2, 0x03); // enable coast window start
        mStar_WriteByte(COCTRL3, 0x01); // enable coast window end
        mStar_WriteByte(ISCTRL, 0x0); // disable DE glitch removal function & more tolerance for DE
    }
    else if ( SrcInputType == Input_Digital  || SrcInputType == Input_Digital2)
    {
        regByte |= ISEL_DVI | COMP_B;
        #if ENABLE_HDMI
        mstar_HDMIInitialVariable();
        #endif			
		
        #if ADC2Sets
            if (PowerSavingFlag)
            {
                mStar_WriteByte(REGBK, REGBANKADC);
                mStar_WriteByte(TESTEN, TSTEN_B);
                mStar_WriteByte(TESTA0, 0x20); //+--- set pll charge pump current for ADC
                mStar_WriteByte(TESTA5, 0x86);
                mStar_WriteByte(TESTEN, 0);
                mStar_WriteByte(REGBK, REGBANKSCALER);
			
            //=========== To Set Up 832x624 75Hz DVI Mode ===========   
	    #if 0//2006-07-11 Andy To Set Up 832x624 75Hz DVI Mode 
	    mStar_WriteByte( 0xE8, ( mStar_ReadByte( 0xE8 ) & ~BIT3 ) | BIT2 ); 
	    #endif               
            }
        #endif
        mStar_WriteByte(IPCTRL2, DEON_B | DHSR_B | VSE_B );	//110311 Modify
        mStar_WriteByte(ISCTRL, 0xF0); //0xF8);// enable DE glitch removal function & more tolerance for DE

        mStar_WriteByte(COCTRL1, 0x0); // disable ADC coast
       
	//***************************************charles
	mStar_WriteByte(REGBK, REGBANKADC);
    
    #if MainBoardType == MainBoard_715G3603_M0A
        if(SrcInputType == Input_Digital) // DVI setting, A port, RB no swap, PN no swap
        {
            mStar_WriteByte(BK1_E4, 0x0e);
            mStar_WriteByte(GCTRL, mStar_ReadByte(GCTRL)&~DRBS);
            mStar_WriteByte(RT_CTL, mStar_ReadByte(RT_CTL)&~BIT7);
        }
        else // HDMI setting, B port, RB swap, PN swap
        {
            mStar_WriteByte(BK1_E4, 0x31);
            mStar_WriteByte(GCTRL, mStar_ReadByte(GCTRL)|DRBS); // RB swap
            mStar_WriteByte(RT_CTL, mStar_ReadByte(RT_CTL)|BIT7); // PN swap
        }
    #else
	#if DVI_RB_SWAP
	mStar_WriteByte(GCTRL, mStar_ReadByte(GCTRL)|DRBS);//DVI R B swap 
	#endif
	#if DVI_PN_SWAP
	mStar_WriteByte(RT_CTL, mStar_ReadByte(RT_CTL)|BIT7);
	#endif
    #endif
    
        mStar_WriteByte(TESTEN, TSTEN_B);
        mStar_WriteByte(TESTA0, 0x20); //+--- set pll charge pump current for ADC
        mStar_WriteByte(TESTEN, 0);
        mStar_WriteByte(REGBK, REGBANKSCALER);

#if ENABLE_HDMI
    #if MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
        // TMDS mux. is inside of scaler
    #else
        #if INPUT_TYPE == INPUT_1A2D
        if(SrcInputType==Input_Digital2)
        #elif INPUT_TYPE == INPUT_1A1H
        if(SrcInputType==Input_Digital)
        #endif
        {		
            hw_SEL_HDMI();
        }
        else
        {
            hw_SEL_DVI();			
        }
    #endif
#endif
    
    }

 
    //if ( SrcInputType == Input_Digital ||SrcInputType == Input_Digital2 )  //charles
    //    mStar_WriteByte( 0xE8, 0x04 );
    //else
        mStar_WriteByte( 0xE8, 0x00 );
       
        mStar_WriteByte(ISELECT, regByte);
  
    Delay1ms(2);
	
    if ( SrcInputType == Input_Digital ||SrcInputType == Input_Digital2 )  //charles
        mStar_WriteByte(SWRST, GPR_B); // enable software reset function to clear ADC & Graphic port RO register
    else
        mStar_WriteByte(SWRST, GPR_B | ADCR_B); // enable software reset function to clear ADC & Graphic port RO register
    Delay1ms(2);
    mStar_WriteByte(SWRST, 0); // disable software reset
    Delay1ms(20); // delay over 1 frame time to wait for status register is ready
    
    #if ENABLE_HDCP
    if(SrcInputType==Input_Digital||SrcInputType == Input_Digital2 )  //charles
            msEnableHDCP();
    #endif
   
}


//*******************************************************************
// Function Name: mStar_SetCaptureWindow
//
// Decscription: setup input capture window for display
//
// caller: mSar_WriteWord(), mStar_ReadWord() in ms_rwreg.c
///
// callee: mStar_SetupMode() in mstar.c
//*******************************************************************
Bool mStar_SetCaptureWindow(void)
{
    WORD width, height;
    #if 0//ENABLE_HDMI
    BYTE i;
    #endif
   	
  if ( SrcInputType == Input_Digital || SrcInputType==Input_Digital2) 
    {
        UserPrefHStart = mStar_ReadWord(AOHST_H); // get hsync DE start
        UserPrefVStart = mStar_ReadWord(AOVST_H); // get vsync DE start
        width = mStar_ReadWord(AOHEND_H) - UserPrefHStart + 1; // get DE width
        height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 1; // get DE Height

        // 091007
        if(SrcFlags&bInterlaceMode)
        {
            if(mStar_ReadByte(0x04)&BIT6)
            {
		#ifdef NewInterlaceShiftLineRule   
		
		#if (INPUT_TYPE == INPUT_1A2D)
		{
			if(SrcInputType == Input_Digital2)
			{
				if(StandardModeGroup == Res_720x480)
				{
					height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 4;
				}
				else
				{
					height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 2;
				}
			}
			else
			{
				height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 2;
			}
		}
		#elif(INPUT_TYPE == INPUT_1A1H)
		{
			if(SrcInputType == Input_Digital)
			{
				if(StandardModeGroup == Res_720x480)
				{
					height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 4;
				}
				else
				{
					height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 2;
				}				
			}
			else
			{
				height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 2;
			}			
		}
		#elif(INPUT_TYPE == INPUT_1A1D)
		{
			height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 2;
		}
		#else///INPUT_1A
		{
			height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 2;
		}
		#endif
		
		#else     
		
		height = mStar_ReadWord(AOVEND_H) - UserPrefVStart + 2;
		
		#endif
            }
            else
            {
                height = mStar_ReadWord(AOVEND_H) - UserPrefVStart;
            }
        }

     		if(PanelWidth == 1366)
		{
			if(width == 1360)
			{
				 UserPrefHStart -= 3;
				 width = 1366;
			}
		}

    }
    else
    {
   		 // input 1360x768 display on 1366 panel,
		// don't do sacle and get 1366 for hori. data
		if(PanelWidth == 1366)
		{
			if(StandardModeGroup == Res_1360x768)
				width = 1366;
			else
				width = StandardModeWidth;
		}
		else
		{
			width = StandardModeWidth;
		}

       
        //2006-03-30 Andy height=StandardModeHeight;
        //height = mStar_ReadWord(AOVEND_H) - mStar_ReadWord(AOVST_H);
        //2006-07-04 Andy    //2006-02-15 Andy for MAC 1152x870  60hz
        //        if ( && height > 868 )    //2006-02-15 Andy for MAC 1152x870  60hz
        if (StandardModeGroup == Res_1152x864)
        {
            height = 864;
        }
        else
        {
            height = StandardModeHeight;
        }

    }
    #if 1 // 2006/7/5 11:52PM by Emily for dec. dot clock
        if (DecVScaleFlag)
            height += VScaleDecValue;

        mStar_WriteWord(SPRHST_H, UserPrefHStart);
        if (DecVScaleFlag)
        {
            if (UserPrefVStart < (VScaleDecValue / 2))
                UserPrefVStart = (VScaleDecValue / 2);
            mStar_WriteWord(SPRVST_H, UserPrefVStart - (VScaleDecValue / 2));
        }
        else
            mStar_WriteWord(SPRVST_H, UserPrefVStart);

        // 091008
        if(SrcFlags&bInterlaceMode)
        {
            if(SrcInputType==Input_Analog1 || SrcInputType>=Input_Digital&&!(mStar_ReadByte(0x04)&BIT6))
            {
                if(UserPrefVStart%2 == 0)
                {
                    UserPrefVStart += 1;
                    mStar_WriteWord(SPRVST_H, mStar_ReadWord(SPRVST_H)+1);
                }
            }
        }
        
        #if DEBUG_CAPTUREWIN
        printData("CAPTUREWIN width=%d",width);
        #endif
        mStar_WriteWord(SPRHDC_H, width);
        //if (StandardModeGroup == Res_1920x1080 && abs(StandardModeHFreq - 337) < 10)
            //height = height + 1;
        
        #if DEBUG_CAPTUREWIN
        printData("CAPTUREWIN height=%d",height);
        #endif
        mStar_WriteWord(SPRVDC_H, height);
    #else
        mStar_WriteWord(SPRHST_H, StandardModeHStart); //UserPrefHStart); // set capture window hstart
        mStar_WriteWord(SPRVST_H, StandardModeVStart); //UserPrefVStart); // set capture window vstart
        mStar_WriteWord(SPRHDC_H, width); // set capture window width according input resolution
        mStar_WriteWord(SPRVDC_H, height); // set capture window height according input resolution
    #endif
    #if 0//PanelWidth==1680
        if ((width > PanelWidth || height > PanelHeight) && StandardModeGroup != Res_1920x1080)
        // set native mode flag  // Sheeper2005527  from PanelHeight to 1024
            SrcFlags |= bShrinkMode;
    #else
        if (width > PanelWidth)
            SrcFlags |= bShrinkMode;
    #endif
    #if Enable_Expansion
	#if (ALL_RES_SCALE_FUNC_EN)
	     Set_ExpansionFlag();
	#else
        if ((4*StandardModeWidth == 5*StandardModeHeight||3*StandardModeWidth == 4*StandardModeHeight)&&WidePanel)
            Set_ExpansionFlag();
        else
            Clr_ExpansionFlag();
	#endif
	/*
        if ((((DWORD)PanelWidth *100) / width) > (((DWORD)PanelHeight *100) / height))
            Set_ExpansionFlag();
        else
            Clr_ExpansionFlag();
      */
    #endif
    return TRUE;
}
//*******************************************************************
// Function Name: mStar_SetupADC
//
// Decscription: setup ADC bandwidth/filter, clock, phase for sampling input data
//               and R/G/B gains, offsets
// caller: mSar_WriteByte(), mStar_ReadByte() in ms_rwreg.c
///
// callee: mStar_SetupMode() in mstar.c
//*******************************************************************
#ifdef TSUMXXT		//110311 Modify
#else
BYTE code ADCWB[4][3] =  // ADC parameter table for setup ADC
{
    // HS_LVL, BWCOEF,  DCOEF
    #if defined TSUMXXNX
    {0x63, 0x86, 0x2},         // <42 MHz
    {0x43, 0x85, 0x3},         // 42< < 82
    {0x23, 0x83, 0x3},         // 82< <155
    {0x03, 0x81, 0x3},         //  >155
    #else        
    {0x65, 0x06,  0x2},    // <42 MHz // <38 MHz
    {0x45, 0x05,  0x3},   // 42< < 82 // 38< < 75
    {0x25, 0x03,  0x3},     // 0x02 wmz 20050714   // 82< <155
    {0x05, 0x01,  0x3}    //  >155
    #endif
};
#endif
void mStar_SetupADC(void)
{
    WORD tempValue;
    mStar_WriteByte(REGBK, REGBANKADC); // switch to ADC bank
    if (SrcInputType < Input_Digital || SrcInputType == Input_YPbPr)
    {
        BYTE regByte = mStar_ReadByte(GCTRL) &~HSP_B;
        tempValue = ((DWORD)MST_CLOCK_MHZ *10+SrcHPeriod / 2) / SrcHPeriod; //calculate hfreq: round 5
        tempValue = ((DWORD)tempValue *UserPrefHTotal + 5000) / 10000; //dclk= hfreq * htotal
 	#ifdef TSUMXXT//#if( ChipID == TSUMXXT ) // TSUMXXT uses new ADC driver		//110311 Modify
		
		msADC_SetModewithPLLProtection(ADC_INPUTSOURCE_RGB, tempValue, UserPrefHTotal);
		msADC_SetupHsyncPolarity((SrcFlags & bHSyncNegative) ? TRUE : FALSE); // BK0_E1[1]: 0: active low, 1: active high
		msADC_AdjustHTotal(UserPrefHTotal);
         // Set Adc Pga gain    //110401 Modify
        msADC_SetPgaGain( UserPrefAdcPgaGain ); 
		msADC_SetRGBGainCode(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);//for test	 //110314 Modify
        msADC_SetRGBOffsetCode(UserPrefAdcRedOffset, UserPrefAdcGreenOffset, UserPrefAdcBlueOffset);
		msADC_SetPhaseCode(UserPrefPhase);		//110413 Modify
		
		// When input timing is reduced blanking, we have to reduce the
		//   duration of ADC calibration to avoid dark display.
		// The amount of calibration duration must be smaller than back-porch.
		// Currently, we set the calibration duration to H_Back_Porch/2.
  		#if 1
		switch( SrcModeIndex)//StandardModeModeID )
		{
			//case Mod_1400x900_60R:  // this timing is reduced blanking
			//{
				// From VESA timing spec, back-porch of this timing is 80 pixels.
			//	msADC_AdjustCalDuring( 80/2 );  // unit: pixel
			//	break;
			//}
		  case PRESET_1128x730_60Hz://20151123 lixw
                {
                    // The power-on default value of the register.
                    msADC_AdjustCalDuring( 8 );	//drvAdcAdjustCalDuring( 8 );  // unit: pixel
                    break;
                }
		}
  		#else
		//msADC_AdjustCalDuring( 30 );	// unit: pixel	//110527 Modify
  		#endif
		  
	#else
        if (tempValue < 42)
        //38)
        {
            tempValue = 0;
        }
        else if (tempValue < 82)
        //75) //82)
        {
            tempValue = 1;
        }
        #if 0   //wmz 20050714 for T144 wring
            else if (StandardModeGroup == Res_1280x1024 && tempValue < 155 && tempValue > 100)
                tempValue = 3;
        #endif
        else if (tempValue < 155)
        {
            tempValue = 2;
        }
        else
        {
            tempValue = 3;
        }
        mStar_WriteByte(HS_LVL, ADCWB[tempValue][0]); // setup ADC bandwidth
        mStar_WriteByte(BWCOEF, ADCWB[tempValue][1]); // setup ADC
        mStar_WriteByte(DCOEF, ADCWB[tempValue][2]);
        // setup clock
        tempValue = UserPrefHTotal - 3;
        #ifdef TSUMXXNX
        mStar_WriteByte ( PLLDIVM, ( tempValue & 0xFF )  );        
        mStar_WriteByte ( PLLDIVL,  ( tempValue >> 8) & 0x1F );
        #else
        mStar_WriteByte(PLLDIVM, tempValue >> 4);
        mStar_WriteByte(PLLDIVL, (tempValue &0xF) << 4);
        #endif
        // setup phase
        mStar_WriteByte(CLKCTRL1, UserPrefPhase + 8);
        mStar_WriteByte(CLKCTRL2, UserPrefPhase);
        // setup hsync polarity
        {
            tempValue = regByte;
            if (!(SrcFlags &bHSyncNegative))
            {
                regByte |= HSP_B;
            }
            mStar_WriteByte(GCTRL, regByte);
        }
        // setup ADC gain
        mStar_WriteByte(REDGAIN, 0xFF - UserPrefAdcRedGain);
        mStar_WriteByte(GRNGAIN, 0xFF - UserPrefAdcGreenGain);
        mStar_WriteByte(BLUGAIN, 0xFF - UserPrefAdcBlueGain);
        // setup ADC offset
        mStar_WriteByte(REDOFST, 0xFF - UserPrefAdcRedOffset);
        mStar_WriteByte(GRNOFST, 0xFF - UserPrefAdcGreenOffset);
        mStar_WriteByte(BLUOFST, 0xFF - UserPrefAdcBlueOffset);
        // setup hsync polarity again
        {
            if (SrcFlags &bHSyncNegative)
            {
                tempValue |= HSP_B;
            }
            mStar_WriteByte(GCTRL, tempValue);
        }
	#endif // end of #if( ChipID == TSUMXXT )
    }
    else if (SrcInputType == Input_Digital || SrcInputType==Input_Digital2)
    {
        mStar_WriteByte(HS_LVL, 0x00); // setup ADC bandwidth
        mStar_WriteByte(BWCOEF, 0x01); // setup ADC
        mStar_WriteByte(DCOEF, 0x03);
    }
    mStar_WriteByte(REGBK, REGBANKSCALER); // switch to scaler bank
}
//*******************************************************************
// Function Name: mStar_SetScalingFactor
//
// Decscription: setup scaler hor/ver scaling factor to expand image to full screen
//
// caller: mSar_WriteByte(), mStar_ReadByte() in ms_rwreg.c
///
// callee: mStar_SetupMode() in mstar.c
//*******************************************************************
#define HSC_offset  1
#if Enable_Expansion
    void mStar_SetScalingFactor(BYTE ExpansionMode)
    {
        DWORD factor, Factor_height;
        WORD width, height;
        WORD Expansionwidth;
        WORD Hstar;
        width = mStar_ReadWord(SPRHDC_H);
        height = mStar_ReadWord(SPRVDC_H);
        Factor_height = ((DWORD)PanelHeight *100) / height;
        Expansionwidth = PanelWidth;
        if (mStar_IsStatusChanged() == TRUE)
            return ;
	#if 0//PanelType==PanelM156B1L01||PanelType==PanelSVA156WX1 || PanelType == PanelCLAA185WA01|| PanelType == PanelM185B1L01||PanelType==PanelLM185WH1TLA1||PanelType==PanelAUOM185XW01 
	//#if (ModelName ==AOC_Q156W)
	//else if (SrcModeIndex==1||SrcModeIndex==13)  // for 1360 x768 60 Hz not scaling
//	else if (StandardModeGroup == Res_1360x768 && abs(StandardModeVFreq - 600) < 10)  // for 1360 x768 60 Hz not scaling

	else if ((StandardModeGroup == Res_1360x768 && abs(StandardModeVFreq - 600) < 10)\
		||(StandardModeGroup == Res_1366x768 && abs(StandardModeVFreq - 600) < 10))  // for 1360 x768 60 Hz not scaling
         {

		BYTE ucReg;
	   	WORD HStart,Hend;

		//if (SrcModeIndex == 0 )
		{
	   		ucReg=mStar_ReadByte(AOVDV);
		   	mStar_WriteByte(AOVDV, 0x40);
		   	ForceDelay1ms(60);
		   	HStart=mStar_ReadWord(AOHST_H); // get hsync DE start
		  	Hend=mStar_ReadWord(AOHEND_H)-HStart+1; // get DE width
		   	mStar_WriteByte(AOVDV, ucReg);
		if (Hend<=1361)
			{
				Expansionwidth=1360;
			}
		      Hstar = ( PanelWidth - Expansionwidth ) / 2 + mStar_ReadWord( DEHST_H );
		      mStar_WriteWord( SIHST_H, Hstar );
		      mStar_WriteWord( SIHEND_H, Hstar + Expansionwidth - 1 ); 
		}
  
         }
		#endif
        else if (ExpansionMode == Expansion_Aspect && ExpansionFlag)
        {
        #if !ALL_RES_SCALE_FUNC_EN
	     Expansionwidth = ((DWORD)Factor_height *width) / (100);
	 #else
	     Expansionwidth = (PanelHeight*4)/3;
        #endif
            Hstar = (PanelWidth - Expansionwidth) / 2+PanelHStart;
            mStar_WriteWord(SIHST_H, Hstar);
            mStar_WriteWord(SIHEND_H, Hstar + Expansionwidth - 1);
			DispalyWidth = Expansionwidth;
			HStartOffset = ( PANEL_WIDTH - Expansionwidth ) / 2;
        }
        else
        {
            mStar_WriteWord(SIHST_H, PanelHStart);
            mStar_WriteWord(SIHEND_H, PanelHStart + PanelWidth - 1);
			DispalyWidth = PANEL_WIDTH;
			HStartOffset = 0;
        }
        if (SrcFlags &bInterlaceMode)
            height /= 2;
		#if !ALL_RES_SCALE_FUNC_EN
        // if input timing is interlace, must use 1/2 height to get scaler factor
        if (width != PanelWidth)
		#endif
        // set expansion factor
        {
            factor = width - HSC_offset; // factor=((input width-1)*2^20)/(output width-1) + 1
            factor = ((DWORD)(factor << 20)) / (Expansionwidth - HSC_offset) + 1;
            mStar_WriteWord(SRH_M, factor &0xFFFF);
            mStar_WriteByte(SRH_H, ((factor >> 16) &0xFF) | SENH_B);
        }
		#if !ALL_RES_SCALE_FUNC_EN
        else
            mStar_WriteByte(SRH_H, 0);
		#endif
        // input width=output width, then disable expansion
        // Setup vertical expansion
        if (height != PanelHeight)
        {
            factor = height - 1; // factor=((input height-1)*2^20)/(output height-1) + 1
            factor = ((DWORD)(factor << 20)) / (PanelHeight - 1) + 1;
            mStar_WriteWord(SRV_M, factor &0xFFFF);
            mStar_WriteByte(SRV_H, ((factor >> 16) &0xFF) | SENV_B);
        }
        else
            mStar_WriteByte(SRV_H, 0);
        // input height=output height, then disable expansion
    }
#else
    void mStar_SetScalingFactor(void)
    {
        DWORD factor;
        WORD width, height;
        width = mStar_ReadWord(SPRHDC_H);
        height = mStar_ReadWord(SPRVDC_H);
        if (SrcFlags &bInterlaceMode)
        {
            height /= 2;
        } // if input timing is interlace, must use 1/2 height to get scaler factor
        //  if (width==640)
        //    mStar_WriteWord(SPRHDC_H, width+1);
        // Setup horizontal expansion
        if (width != PanelWidth)
        // set expansion factor
        {
            factor = width - HSC_offset; // factor=((input width-1)*2^20)/(output width-1) + 1
            factor = ((DWORD)(factor << 20)) / (PanelWidth - HSC_offset) + 1;
            mStar_WriteWord(SRH_M, factor &0xFFFF);
            mStar_WriteByte(SRH_H, ((factor >> 16) &0xFF) | SENH_B);
        }
        else
        {
            mStar_WriteByte(SRH_H, 0);
        } // input width=output width, then disable expansion
        // Setup vertical expansion
        if (height != PanelHeight)
        {
            factor = height - 1; // factor=((input height-1)*2^20)/(output height-1) + 1
            factor = ((DWORD)(factor << 20)) / (PanelHeight - 1) + 1;
            mStar_WriteWord(SRV_M, factor &0xFFFF);
            mStar_WriteByte(SRV_H, ((factor >> 16) &0xFF) | SENV_B);
        }
        else
        {
            mStar_WriteByte(SRV_H, 0);
        } // input height=output height, then disable expansion
    }
#endif
//*******************************************************************
// Function Name: mStar_ResetDClkPLL
//
// Decscription: Reset output PLL to prevent from PLL works abnormally by
//               toggle power down/on of PLL
//
// caller: mSar_WriteByte(), mStar_ReadByte() in ms_rwreg.c
///
// callee: mStar_SetupMode() in mstar.c
//*******************************************************************
void mStar_ResetDClkPLL(void)
{
#if 0
    BYTE regByte;
    regByte = mStar_ReadByte(PLLCTRL2);
    Delay1ms(10);
    mStar_WriteByte(PLLCTRL2, regByte | LP_PD_B); // force PLL power down
    Delay1ms(10);
    mStar_WriteByte(PLLCTRL2, regByte); // then re-enable PLL
#endif
}

void ShortLineTune(bit disable)
{
    BYTE regBank = mStar_ReadByte(0x00);
    
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(TESTEN, TSTEN_B); 
    mStar_WriteByte(0x3E, (mStar_ReadByte(0x3E)&~BIT0)|BIT2);
    mStar_WriteByte(TESTEN, 0); 
    
    mStar_WriteByte(REGBK, REGBANKSCALER);

    if(disable)
    {
        mStar_WriteByte(0x48, mStar_ReadByte(0x48)|BIT3);
        mStar_WriteByte(OSCCTRL2, mStar_ReadByte(OSCCTRL2)&~BIT6);
        mStar_WriteByte(0x48, mStar_ReadByte(0x48)&~BIT3);
    }
    else
    {
        mStar_WriteByte(0x48, mStar_ReadByte(0x48)|BIT3);
        mStar_WriteByte(OSCCTRL2, mStar_ReadByte(OSCCTRL2)|BIT6);
        mStar_WriteByte(0x48, mStar_ReadByte(0x48)&~BIT3);
    }

    mStar_WriteByte(REGBK, regBank);
}
//*******************************************************************
// Function Name: mStar_SetPanelTiming
//
// Decscription: Setup panel output dclk.
//
// callee: mStar_ResetDClkPLL() in mstar.c
//         mSar_ReadWord(), mStar_WriteWord(), mStar_WriteByte() in ms_rwreg.c
//
// caller: mStar_SetupMode() in mstar.c
//*******************************************************************
Bool mStar_SetPanelTiming(void)
{

    DWORD factor;
    WORD dclk;
    WORD sclk;
    WORD height;
    WORD dstHTotal;
    WORD hPeriod;
	#ifdef TSUMXXT		//110311 Modify
			 DWORD			   ODCLK_KHZ;
			 LPLL_INPUT_DIV_t  LPLL_INPUT_DIV;
			 LPLL_OUTPUT_DIV_t LPLL_OUTPUT_DIV;
			 BYTE			   LPLL_LOOP_DIV;
	const	 BYTE			   LPLL_ICTL = 3;
	#endif  // end of #if( ChipID == CHIP_TSUMT )
    #if 0
        // for getting more accurate output htotal
        sclk = GetVSyncTime() *3;
        mStar_WriteByte(HSPRD_H, IHDM_B); // enable 16 line line to calculate hsync period.
        Delay1ms(sclk);
        hPeriod = mStar_ReadWord(HSPRD_H) &0x1FFF;
        mStar_WriteByte(HSPRD_H, 0);
        Delay1ms(sclk);
    #endif
    Clr_DLKOverFlag();
    // for getting more accurate output htotal //0812 for AC speed switch
    if ( SrcHPeriod > 510 ) // prevent counter overflow when input frequency is very low
    {
        hPeriod = SrcHPeriod * 16;
    }
    else
    {
        sclk = GetVSyncTime() *4;
        mStar_WriteByte(HSPRD_H, IHDM_B); // enable 16 line line to calculate hsync period.
        Delay1ms(sclk);
        hPeriod = mStar_ReadWord(HSPRD_H) &0x1FFF;
        mStar_WriteByte(HSPRD_H, 0);
        Delay1ms(sclk);
        if ( abs( hPeriod - SrcHPeriod * 16 ) > 80 ) // prevent from getting the wrong hperiod to generate unknown panel clock
        {
            hPeriod = SrcHPeriod * 16;
        }
    }
    // calculate input timing dot clock for comparing with output dot clock
    // 090430 calculate input timing clock even if input resolution is higher than panel resolution.
    //sclk = 0;
    //if (mStar_ReadWord(SPRHDC_H) < PanelWidth || mStar_ReadWord(SPRVDC_H) < PanelHeight)
    {
        sclk = ((DWORD)UserPrefHTotal *MST_HPeriod_UINT) / ((DWORD)hPeriod *1000);
    }
#if DEBUG_ENABLE
    printData( "input sclk %d", sclk );   
#endif

    // 2006/8/25 11:43AM by Emily   InputSclk=((DWORD)UserPrefHTotal*MST_HPeriod_UINT)/((DWORD)hPeriod*1000);
    height = mStar_ReadWord(SPRVDC_H);
    //  if (height==350)
    //    height=400;

    if (SrcFlags &bInterlaceMode)
    {
        height /= 2;
    }

   /* #if ModelName==AOC_2037F||ModelName==AOC_e2037F     //edit by lizzie.ding  ,100623.for not change spec PanelMinHTotal,maybe happened something unpredictable
	if(StandardModeGroup == Res_800x600 && abs(StandardModeVFreq - 720) < 10)
	dstHTotal = PanelMinHTotal-25;
      else	
    #elif PanelType==PanelAUOM240HW01V6||PanelType==PanelAUOM240HW01V7||PanelType==PanelTPM200O1L02C1D
	if(StandardModeGroup == Res_800x600 && abs(StandardModeVFreq - 720) < 10)
	dstHTotal = PanelMinHTotal-20;
      else	
    #endif*/
   //#if(PanelType==PanelAUOM215HW01)
      #if ModelName==ENVISOIN_P2273wl
		if((StandardModeGroup == Res_800x600 && (abs(StandardModeVFreq - 720) < 10))||(StandardModeGroup == Res_832x624 && (abs(StandardModeVFreq - 750) < 10)))
		{
		dstHTotal = PanelMinHTotal;
		//printData("dstHTotal1=%d",dstHTotal);
		}
         else
	#endif
       #if ModelName==AOC_1620Sw_TPM156B1L01
		if((StandardModeGroup == Res_800x600 && (abs(StandardModeVFreq - 720) < 10))||(StandardModeGroup == Res_832x624 && (abs(StandardModeVFreq - 750) < 10)))
		{
		dstHTotal = PanelMinHTotal-15;
		//printData("dstHTotal1=%d",dstHTotal);
		}
         else
	#endif
	#if PanelType==PanelCLAA156WB11A
	   if(StandardModeGroup == Res_720x400 && (abs(StandardModeVFreq - 700) < 10))
		{
		dstHTotal = PanelMinHTotal-37;
		//printData("dstHTotal1=%d",dstHTotal);
		}
	else
        #endif
	#if PanelType==PanelCMON156B6L0B
	   if(StandardModeGroup == Res_720x400 && (abs(StandardModeVFreq - 700) < 10))
		{
		dstHTotal = PanelMinHTotal-20;
		//printData("dstHTotal1=%d",dstHTotal);
		}
	else
        #endif
	#if ModelName==AOC_919Vz1
	   if(StandardModeGroup == Res_800x600&& (abs(StandardModeVFreq - 560) < 10))
		{
		
		dstHTotal = PanelMinHTotal+64;
		
		//printData("dstHTotal1=%d",dstHTotal);
		}
	else
        #endif
	#if PanelType==PanelAUOM215HW03//||PanelType==PanelHSM215EU_HTN01
		if( StandardModeGroup == Res_720x400)
			dstHTotal = PanelMinHTotal+150;
		else if(StandardModeGroup == Res_1024x768&& (abs(StandardModeVFreq - 720) < 5))
			dstHTotal = PanelMinHTotal+130;
		else if(StandardModeGroup == Res_1920x1080&& (abs(StandardModeVFreq - 750) < 5))
			dstHTotal = PanelMinHTotal+100;
		else
        #endif

	
       dstHTotal = PanelMinHTotal+10;
   // printData("dstHTotal2=%d",dstHTotal);

    #if FixDClk // Fixed output dclk method: output clock will be fixed no matter what input timing is
        {
            // according to output clock to calculate output htotal
            dclk = OutDClk1;
            dstHTotal = ((float)hPeriod *dclk *(height - 1) *1000ul) / ((float)(PanelHeight - 1) *MST_HPeriod_UINT);

        if ( dstHTotal < PanelMinHTotal ) // if input htotal is samll than panel htotal, then set to another dclk
            {
                dclk = OutDClk2;
                dstHTotal = ((float)hPeriod *dclk *(height - 1) *1000ul) / ((float)(PanelHeight - 1) *MST_HPeriod_UINT);
            }
            dstHTotal &= 0xFFFE; // make output htotal as even
            // if the htotal is out of panel htotal, then use the panel minimum htotal
            if (dstHTotal < PanelMinHTotal || dstHTotal > PanelMaxHTotal)
            {
                dstHTotal = PanelMinHTotal;
            }
        }
    #endif
    if (sclk)
    {
        // in expansion mode, chip will assume output clock is faster than input clock,
        // if input clock is faster than output clock, must make output clock faster than input clock
        factor = ((float)dstHTotal *(PanelHeight - 1) *MST_HPeriod_UINT) / ((DWORD)hPeriod *(height - 1)); // output dclk
        dclk = (factor + 500) / 1000;
	#if  DEBUG_ENABLE
		printData( "Output Dclk %d", dclk );
	#endif
	
            if (dclk > 
				#if PanelType==PanelM216H1L01||PanelType==PanelLTM230HT01
				PanelMaxDCLK
				#elif PanelType==PanelLM230WF1TLA3||PanelType==PanelLM230WF1TLD1||PanelType==PanelLM230WF5TLA1
				(PanelMaxDCLK-5)
				#else
				ADCLimited
				#endif
				)
                Set_DLKOverFlag();
#if PanelType==PanelLGLP156WHB_DP
  #if 1
    	if (1)
            {
            	BYTE VSCALE_DECVALUE_DP;//=7;//14;//10;
	if (StandardModeGroup==Res_1366x768||StandardModeGroup==Res_1360x768)
		VSCALE_DECVALUE_DP=7;
	else if (StandardModeGroup==Res_1280x1024)
		VSCALE_DECVALUE_DP=9;
	else
		VSCALE_DECVALUE_DP=10;
	
                Set_DecVScaleFlag();
                //20120525 for PanelB156XTN02 DCLK out of range, the VSCALE_DECVALUE must be 40;
                {
                    VScaleDecValue = VSCALE_DECVALUE_DP;
                    dclk = mStar_ReadWord(VTOTAL_H);
                    if (dclk <= (height + VScaleDecValue + 20))
                    {
                      //  VScaleDecValue = 0;
                      //  Clr_DecVScaleFlag();
                    }
                }
                    

                mStar_SetCaptureWindow();
	    	    #if Enable_Expansion
        		mStar_SetScalingFactor(UserprefExpansionMode);
        		#else
        		mStar_SetScalingFactor();
        		#endif
                height += VScaleDecValue;
            }
    #endif
#else				
        #if 1
    	if ((dclk>=PanelMaxDCLK) 
			
             &&(StandardModeGroup == Res_640x400 
             || StandardModeGroup == Res_720x400
             || StandardModeGroup == Res_800x600
             || StandardModeGroup == Res_832x624
             || StandardModeGroup == Res_640x350))
            {
		    DWORD xdata oVtotal;
		  WORD wHeight;
			
                Set_DecVScaleFlag();

                //20120525 for PanelB156XTN02 DCLK out of range, the VSCALE_DECVALUE must be 40;
                #if  ((PanelType == PanelB156XTN02)||(PanelType == PanelB156XTN07)||(PanelType == PanelNT156WHM_N12)||(PanelType == PanelM185BGE_L23)||(PanelType == PanelM185WHB_N10)||(PanelType == PanelLM185TT3A))
                if((StandardModeGroup == Res_720x400) && (abs(StandardModeVFreq - 700) < 10)) 
                {
                    VScaleDecValue = 40;
                    dclk = mStar_ReadWord(VTOTAL_H);
                    if (dclk <= (height + VScaleDecValue + 8))
                    {
                        VScaleDecValue = 0;
                        Clr_DecVScaleFlag();
                    }
                } 
                else
                #endif
                {
                    VScaleDecValue = VSCALE_DECVALUE;
                    dclk = mStar_ReadWord(VTOTAL_H);
                    if (dclk <= (height + VScaleDecValue + 20))
                    {
                        VScaleDecValue = 0;
                        Clr_DecVScaleFlag();
                    }
                }
                    
            wHeight = height+VScaleDecValue;
            oVtotal = (DWORD)SrcVTotal*PanelHeight/wHeight;

            factor=((float)dstHTotal*(PanelHeight-1)*MST_HPeriod_UINT)/((DWORD)hPeriod*(wHeight-1)); // output dclk
            dclk=(factor+500)/1000;

            if((dclk > PanelMaxDCLK) || (oVtotal>PanelMaxVTotal))
            {
                SrcFlags|=bUnsupportMode;
                return FALSE;
            }



                mStar_SetCaptureWindow();
	    	    #if Enable_Expansion
        		mStar_SetScalingFactor(UserprefExpansionMode);
        		#else
        		mStar_SetScalingFactor();
        		#endif
                height += VScaleDecValue;
            }
        else if ( dclk > PanelMaxDCLK*1.2 ) // if ouput clock is out of panel, then enable unsupported flag
            {
		#if DEBUG_FINDMODE
			printData(" dclk=XXX%d", dclk);
		#endif
                SrcFlags |= bUnsupportMode;
            Set_DLKOverFlag();
                return FALSE;
            }
        #endif
#endif
    }

    //=  2003/12/01 used for support frequency over 173MHz, and native mode  =
    {
        // used for support frequency over 173MHz, and native mode

        #if MST_MCU
            #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
            #define MPLL_Divider    15
            #else
            #define MPLL_Divider    (7.5)  //Double Speed
            #endif
            // #define  MPLL_Divider    15
        #else
            #define MPLL_Divider    15
        #endif

        sclk = ((DWORD)UserPrefHTotal *MST_HPeriod_UINT) / ((DWORD)hPeriod *1000);

        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(TESTEN, TSTEN_B);
	if (SrcInputType < Input_Digital)
        {
            if (sclk < 20)
            {
                mStar_WriteByte(VCOCTRL, 0x05);
            } // set VCO range
            else if (sclk > 140)
            {
                mStar_WriteByte(VCOCTRL, 0x35);
            } // set VCO range
            else
            {
                mStar_WriteByte(VCOCTRL, 0x15);
            } // set VCO range

            if (sclk > 173)
            {
                mStar_WriteByte(TESTA3, 0xC0);
            }
            else
            {
                mStar_WriteByte(TESTA3, 0x00);
            }
        }
        else
        {
            mStar_WriteByte(TESTA3, 0x00);
        }

        mStar_WriteByte(TESTEN, 0x00);
        mStar_WriteByte(REGBK, REGBANKSCALER);

        #define regByte     dclk
        regByte = mStar_ReadByte(ISCTRL);
        if (sclk > 165)
        {
            mStar_WriteByte(ISCTRL, regByte | SCKI_B);
        }
        else
        {
            mStar_WriteByte(ISCTRL, regByte &~SCKI_B);
        }
        #undef regByte

	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
		ODCLK_KHZ = (float)XTAL_CLOCK_KHZ * 16UL / hPeriod * dstHTotal * (PanelHeight-1) / (height-1); // output dot clock in KHz
		mStar_LPLL_GetDividers( ODCLK_KHZ, &LPLL_INPUT_DIV, &LPLL_OUTPUT_DIV, &LPLL_LOOP_DIV );
		mStar_LPLL_GetFactor  ( ODCLK_KHZ,	LPLL_LOOP_DIV , &factor );
	#else
        #if PanelLVDS
            factor = ((float)hPeriod *524288ul * MPLL_Divider *(height - 1)) / ((float)dstHTotal *7 *(PanelHeight - 1)); // 2^23
        #else
            factor = ((float)hPeriod *65536ul * MPLL_Divider *(height - 1)) / ((float)dstHTotal *(PanelHeight - 1)); // 2^20
        #endif
	#endif
        #undef MPLL_Divider

        #if 0
		dclk=(14.318*15*524288*8*1)/(factor*3.5);
		printData("output dclk=XXX%d", dclk);		
        #endif

        }

        //1201 #if PanelLVDS
        //1201  factor=((float)hPeriod*524288ul*15*(height-1))/((float)dstHTotal*7*(PanelHeight-1)); // 2^23
        //1201 #else
        //1201  factor=((float)hPeriod*65536ul*15*(height-1))/((float)dstHTotal*(PanelHeight-1)); // 2^20
        //1201 #endif

        #ifndef TSUMXXT//#if 1//!EnablePanelServiceMenu  		//110311 Modify
        #if (!PanelRSDS )//&& !PanelDualPort)
		if (!(mStar_ReadByte(OCTRL2)&DPO_B))
            factor /= 2;
        #endif
        #endif

        ShortLineTune(SrcFlags&bInterlaceMode);
        
        // program LPLL parameters to generate expected output dclk
        mStar_WriteWord(LPLL_SET_M, factor &0xFFFF);
        mStar_WriteByte(LPLL_SET_H, factor >> 16);
        Delay1ms(100);
		#ifdef TSUMXXT		//110311 Modify
		// Setting these dividors must placed after LPLL_SET, to ensure stable LPLL clock output.
		mStar_WriteByte(REGBK, REGBANKSCALER);
		mStar_WriteByteMask( BK0_D3, (LPLL_OUTPUT_DIV<<2)|(LPLL_INPUT_DIV), (BIT3|BIT2)|(BIT1|BIT0) );	 // BK0_D3[3:2]=LPLL_OUTPUT_DIV;  BK0_D3[1:0]=LPLL_INPUT_DIV
		mStar_WriteByteMask( BK0_D4, (LPLL_ICTL 		), (BIT2|BIT1|BIT0) 	 );   // BK0_D4[2:0] = LPLL_ICTL
		mStar_WriteByteMask( BK0_D9, (LPLL_LOOP_DIV  <<4), (BIT7|BIT6|BIT5|BIT4) );   // BK0_D9[7:4] = LPLL_LOOP_DIV
		#endif  // end of #if( ChipID == CHIP_TSUMT )
        mStar_WriteByte(ISELECT, mStar_ReadByte(ISELECT) &(~NIS_B)); // enable lock input mode
		#ifdef	PanelSSCFunction
		mStar_SetPanelSSC();	  
		#endif
    /* // 090526 reserved
        mStar_WriteWord(VDTOT_H, 0x7FF); // 20030812

        sclk = GetVSyncTime() *4; // 20030812

        Delay1ms(sclk);

        //  FixShortLine_QDI(); // 20030812 Fix QDI17 LVDS Panel

        // #if PnlSANYO19 //Sanyo 19" 01D
        //  FixShortLine_Sanyo1901D(factor);
        // #endif
        mStar_ResetDClkPLL(); // reset output PLL to prevent PLL works abnormally //0910
    */
        return TRUE;
    }




//*******************************************************************
// Function Name: mStar_PowerUp
//
// Decscription: Power on chip from power down mode
//
// callee: mStar_WriteByte() in ms_rwreg.c
//
// caller: Power_PowerOnSystem() in power.c
//*******************************************************************
void mStar_PowerUp(void)
{

    //mStar_WriteByte(PDMD, 0);// power on chip from power down mode
    if(!PowerGoodExistFlag)
        mStar_WriteByte(PDMD, VDD2LOW); //MST_MCU
    else
        mStar_WriteByte(PDMD, 0); //MST_MCU

#ifdef AOC37Series
    // DC off / Power Saving, system runs at 14.318MHz for power consumption
    mStar_WriteByte ( REGBK, REGBANKADC );    
#if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
    msWriteByteMask(BK1_68, BIT6       ,  BIT6);//B.1.8 Scar, 20090424   /*JT modify it from BK1_67[7] change to BK1_68[6]*/
    msWriteByteMask(BK1_66, BIT0       ,  BIT0);//B.14.4 Scar,20090424
    ForceDelay1ms ( 1 );        
    //DoubleSpeed
    msWriteByteMask(BK1_67, BIT4        ,  BIT5|BIT4);
    ForceDelay1ms ( 1 );
    msWriteByteMask(BK1_66, BIT3|BIT2, BIT3|BIT2|BIT1);
    ForceDelay1ms ( 1 );
    msWriteByteMask(BK1_66, 0             ,  BIT0);
    ForceDelay1ms ( 1 );
#else
    //DoubleSpeed
    mStar_WriteByte(BK1_66, 0x57);//0x47 // BK1 66h[7:5]: 1/mcu operating frequency
    ForceDelay1ms(1);
    mStar_WriteByte(BK1_66, 0x5A);//0x4A
    ForceDelay1ms(1);
    mStar_WriteByte(BK1_66, 0x5B); //0x4B
    ForceDelay1ms(1);
#endif
#if MainBoardType==MainBoard_715G3598_M0A // edit by lizzie. for MainBoard_715G3598_M0A  when power saving,system runs at 14.318MHz,and the 1s counter will be half.
    SingalXTALFlag=0;
#endif
    mStar_WriteByte ( REGBK, REGBANKSCALER);    

    #if ESaverPowerDownFunc
	#ifdef TSUMXXNX
	   if(ESaverPowerDownCounter)
	   	ESaverPowerDownCounter*=2;
	 #endif
	#endif
#endif
	#ifdef TSUMXXT		//110311 Modify
	// CHIP_TSUMT new MPLL shouldn't modify BK0_D1
	mStar_WriteByte(PLLCTRL2, 0x04); // restore power-on default value of BK0_D1
	// power on all ADC
	mStar_WriteByte(REGBK, REGBANKADC_ATOP );
	mStar_WriteByte(REG_ADC_ATOP_08, 0x00 );	// power-on ADC
	mStar_WriteByte(REG_ADC_ATOP_09, 0x00 );	// power-on ADC
	mStar_WriteByte(REGBK, REGBANKSCALER);
	#else
    mStar_WriteByte(PLLCTRL2, BIT7); // power on master & output PLL
    #endif

#if MS_DAC
	msAudioDACPowerDown(FALSE);
#endif

    // 090318 modified for reducing thermal
    #if MainBoardType == MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2
    mStar_WriteByte(PDMD, mStar_ReadByte(PDMD)|BIT3);
    #endif
        
    #if ADC2Sets
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(TESTEN, TSTEN_B);
        mStar_WriteByte(TESTA5, 0);
        mStar_WriteByte(TESTA6, 0);
        // mStar_WriteByte(TESTMOD, 0x10);//0x08);//0x40);
        mStar_WriteByte(TESTEN, 0);
        mStar_WriteByte(REGBK, REGBANKSCALER);
    #endif
    mStar_WriteByte(SWRST, GPR_B | ADCR_B); // reset graphic port RO register
    mStar_WriteByte(SWRST, 0);
    
#if MS_OVD // 081124 od modified
    msOD_MIUPowerUp(_OD_Enable);
    msOD_SetMIUSSC(_MIU_SSC_40K_ONE_PERCENT);
#endif
}
//*******************************************************************
// Function Name: mStar_PowerDown
//
// Decscription: Power down chip
//
// callee: mStar_WriteByte() in ms_rwreg.c
//
// caller: Power_PowerOffSystem() in power.c
//*******************************************************************
void mStar_PowerDown(void)
{
#if MS_OVD // 081124 od modified
    msOD_SetMIUSSC(_MIU_SSC_OFF);
    msOD_OnOff(_DISABLE,0x50); // disable OD function , or you can set BK4_90h= 0 directly.
    msOD_MIUPowerUp(_OD_Disable);
#endif

    mStar_WriteByte(TRISTATE, 0xFF); // tri-state all output panel signal


#if MS_DAC
	msAudioDACPowerDown(TRUE);
#endif
	
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )  // power down all ADC		//110311 Modify
	mStar_WriteByte(REGBK, REGBANKADC_ATOP );
	mStar_WriteByte(REG_ADC_ATOP_08, 0xFF );	// set power-on default value to power down ADC
	mStar_WriteByte(REG_ADC_ATOP_09, 0x7F );	// set power-on default value to power down ADC
	mStar_WriteByte(REGBK, REGBANKSCALER);
	#endif  // end of #if( ChipID == TSUMXXT )

    #if ADC2Sets
        {
            BYTE regByte = 0x9E; //OVRD_B|DPLBG_B|DMIBEX_B|PHD_B|DMUX_B;
            mStar_WriteByte(PLLCTRL2, BIT7 | MP_PD_B | LP_PD_B);
            if (SrcInputType == Input_Digital || SrcInputType==Input_Digital2)
            {
                regByte = 0x86;
            }
            else if (SrcInputType == Input_Analog2)
            {
                regByte |= AMUX_B;
            }
            mStar_WriteByte(REGBK, REGBANKADC);
            mStar_WriteByte(TESTEN, TSTEN_B);
            mStar_WriteByte(TESTA5, regByte);
            mStar_WriteByte(TESTA6, 0xF8); //0xE8);
            //  mStar_WriteByte(TESTMOD, 0x10);//0x08);//0x40);
            mStar_WriteByte(TESTEN, 0);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        #if MST_MCU
            if(!PowerGoodExistFlag)
                mStar_WriteByte(PDMD, PDDS_B | 0x42); // power down chip except mode detection
            else
                mStar_WriteByte(PDMD, PDDS_B | 0x02); // power down chip except mode detection
        #else
            mStar_WriteByte(PDMD, PDDS_B | 0x2); // power down chip except mode detection
        #endif
    #else
        //mStar_WriteByte(PWM1C, 0); //wmz 20051019  //  To prevent pwm1c be set to unknown value
        // mStar_WriteByte(PDMD, PDDS_B|0x3);// power down chip except mode detection
        #if MST_MCU
            //mStar_WriteByte(PDMD, PDDS_B|0x43);// power down chip except mode detection
            if(!PowerGoodExistFlag)
                mStar_WriteByte(PDMD, PDDS_B | 0x43); // power down chip except mode detection
            else
                mStar_WriteByte(PDMD, PDDS_B | 0x03); // power down chip except mode detection
        #else
            mStar_WriteByte(PDMD, PDDS_B | 0x3); // power down chip except mode detection
        #endif
    #endif
#ifdef AOC37Series
    // DC off / Power Saving, system runs at 14.318MHz for power consumption
    mStar_WriteByte(0x00, 0x01);
    mStar_WriteByte(BK1_66, mStar_ReadByte(BK1_66)|BIT0);
    mStar_WriteByte(0x00, 0x00);
	#if MainBoardType==MainBoard_715G3598_M0A   // edit by lizzie. for MainBoard_715G3598_M0A  when power saving,system runs at 14.318MHz,and the 1s counter will be half.
	SingalXTALFlag=1;
	#endif
	#if ESaverPowerDownFunc
	#ifdef TSUMXXNX
	   if(ESaverPowerDownCounter)
	   	ESaverPowerDownCounter/=2;
	#endif
	#endif
#endif

}
//*******************************************************************
// Function Name: mStar_InitADC
//
// Decscription: Initialize ADC bank registers
//
// callee: mStar_WriteByte() in ms_rwreg.c
//
// caller: mStar_Init() in mStar.c
//*******************************************************************
void mStar_InitADC(void)
{
    mStar_WriteByte(REGBK, REGBANKADC); // switch to ADC bank
    //===2006-06-26 Andy For P2_4 PULL HIGH WHEN ENTER ISP MODE ====
    #if 1
    mStar_WriteByte(BK1_50, mStar_ReadByte(BK1_50) | BIT7);
    #endif
    //=============================================================
    mStar_WriteByte(CLPLACE, 0x10); // set clamping start position
    mStar_WriteByte(CLDUR, 0x8); // set clamping width
    mStar_WriteByte(FCOEF, 0x9);
    mStar_WriteByte(VCOCTRL, 0x15); //D); // set VOC
    mStar_WriteByte(SOG_LVL, 0x10); // set SOG level
    mStar_WriteByte(PLLCTRLV, 0x95); //x49);//0x00);
    
    mStar_WriteByte(TESTEN, 0x80); //0x22);
    mStar_WriteByte(0x39, mStar_ReadByte(0x39) | BIT4);
    mStar_WriteByte(TESTEN, 0); //0x22);
    // DVI setting
    mStar_WriteByte(RT_CTL, 0x10);
    mStar_WriteByte(DVI_PHR, 0); //0x03);
    mStar_WriteByte(DVI_PHG, 0); //0x01);
    mStar_WriteByte(DVI_PHB, 0); //0x57);
    mStar_WriteByte(DVI_ERST, 0); //0x22);
    mStar_WriteByte(DVI_ERTH, 0x02); //0x0A);//0x20); // DVI threshold
    mStar_WriteByte(TESTEN, TSTEN_B); //0x22);
    mStar_WriteByte(TESTA0, 0x20); //0x22);
    #if EnablePanelServiceMenu
        mStar_WriteByte(TESTMOD, PanelLVDSSwing&0x1F);
    #else
         #if(PanelType==PanelLTM190M2)
             mStar_WriteByte(TESTMOD, 0x14);
         #elif PanelType == PanelTPM215HW01
             mStar_WriteByte(TESTMOD, 0x10);
     	#elif PanelType == PanelM185XW01
             mStar_WriteByte(TESTMOD, 0x1B);
     	#elif PanelType==PanelLTM230HT01
     	mStar_WriteByte(TESTMOD, 0x1B);
         #else
             mStar_WriteByte(TESTMOD, 0x15);//0x10); //0x08);//10);//0x22);
         #endif
    #endif
    #if PanelRSDS
    mStar_WriteByte(BK1_2F, 0x04);
    mStar_WriteByte(TST_REG, 0x84); // set RSDS to TTL for data pull low
    #elif PanelminiLVDS
        mStar_WriteByte(TST_REG, 0x84); // 090810 coding, different setting for mini
        mStar_WriteByte(BK1_68, BIT6);
    #else
        //mStar_WriteByte(TST_REG, 0);//0x22);
        mStar_WriteByte(TST_REG, 0x0f); //0x22);
    #endif

#if 1
    mStar_WriteByte(TESTA2, 0x00);
    mStar_WriteByte(BK1_36, 0x00);
    mStar_WriteByte(BK1_33, 0x40);
    mStar_WriteByte(BK1_3D, 0x10);
#else
    #if ChipID>=ChipBB
        mStar_WriteByte(TESTA2, 0x00);
        mStar_WriteByte(BK1_36, 0x00);///mStar_WriteByte(BK1_36, 0x10); ///yuanzhi20100524 modified to 0x00 for 10m cable.
        mStar_WriteByte(BK1_33, 0x40);
    #elif ChipID==ChipBA
        mStar_WriteByte(TESTA2, 0x04);
        mStar_WriteByte(BK1_36, 0x20);
        mStar_WriteByte(BK1_33, 0x40);
    #elif ChipID==ChipNone
        mStar_WriteByte(TESTA2, 0x80);
    #endif
#endif
    
    #if MST_MCU// 2005/5/12 ¤W¤È 10:32:35 by keng
        //2006-06-08 Andy suggest by Jeff mStar_WriteByte(0x51,0x40);
        // mStar_WriteByte(0x51,0x40);
        mStar_WriteByte(0x25, 0x00);
        #if (MainBoardType==MainBoard_2701||MainBoardType==MainBoard_2698_2_X_X_2||\
			MainBoardType==MainBoard_2698_2_X_X_8||MainBoardType==MainBoard_715G2575||\
			MainBoardType==MainBoard_2883_B_X_X_1||MainBoardType==MainBoard_2883_B_X_X_2||\
			MainBoardType==MainBoard_2670_C||MainBoardType==MainBoard_2670_1||\
			MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611||\
			MainBoardType==MainBoard_2883_1B||MainBoardType == MainBoard_3289_A2||\
			MainBoardType==MainBoard_715G3289_C2||MainBoardType==MainBoard_715G3329_1_2)
                #if MainBoardType==MainBoard_715G3289_C2 // 090623 coding for g2261w touch key
                    #if ScanKey
                    mStar_WriteByte(BK1_62, 0x08);
                    #else
                    mStar_WriteByte(BK1_62, 0x0E);
                    #endif
                #else
                mStar_WriteByte(BK1_62, 0x0E);
                #endif
        #elif MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A||\
                 MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
                #if MainBoardType == MainBoard_715G3603_M0A||MainBoardType==MainBoard_715G3598_M0A||\
                      MainBoardType==MainBoard_715G3329_A2//||\ ||MainBoardType==MainBoard_712G3244_2// 090623 coding for p2471w touch key
                    #if ScanKey
                    mStar_WriteByte(BK1_62, 0x08);
                    #else
                    mStar_WriteByte(BK1_62, 0x0E);
                    #endif
                #else
                mStar_WriteByte(BK1_62, 0x0E);
                #endif
        #elif MainBoardType==MainBoard_715G3329_58N||MainBoardType==MainBoard_715G3329_18N
            mStar_WriteByte(BK1_62, 0x0E);
        #elif MainBoardType==MainBoard_715G2564_B
            mStar_WriteByte(BK1_62, 0x0F);
		#elif MainBoardType==MainBoard_715G3598_1PLL
            mStar_WriteByte(BK1_62, 0x00);
        #elif MainBoardType == MainBoard_715G2988
            mStar_WriteByte(BK1_62, 0x00);
        #elif MainBoardType==MainBoard_715G2808
            mStar_WriteByte(BK1_62, 0x07);
		#elif MainBoardType==MainBoard_712G3244_2
			mStar_WriteByte(BK1_62, 0x00);
		#elif MainBoardType == MainBoard_715G3244_TSUMU18NR
            #if ScanKey
            mStar_WriteByte(0x62, 0x00);
            #else
            mStar_WriteByte(0x62, 0x0E);
            #endif
		#elif MainBoardType==MainBoard_715G4175_M0A
	     	mStar_WriteByte(BK1_62, 0x06); 
		#elif MainBoardType == MainBoard_4432
         	mStar_WriteByte( BK1_62, 0x06 );
		#elif MainBoardType == MainBoard_715G3959_T		//110623 Modify
			#if ScanKey
			mStar_WriteByte(0x62, 0x00);
			#else
			mStar_WriteByte(0x62, 0x06);
			#endif
	#elif (MainBoardType==MainBoard_3834_18T)		//petit 20121228
			#if ScanKey
			mStar_WriteByte(0x62, 0x00);
			#else
			mStar_WriteByte(0x62, 0x06);
			#endif
	#elif (MainBoardType==MainBoard_715G6952_M0B)	|| (MainBoardType==MainBoard_715G6912_M1A)	//Gino 20140522
			#if ScanKey
			mStar_WriteByte(0x62, 0x00);
			#else
			mStar_WriteByte(0x62, 0x06);
			#endif
        #elif (MainBoardType==MainBoard_715G3834)		//110413 Modify
			#if ScanKey
			mStar_WriteByte(0x62, 0x00);
			#else
			mStar_WriteByte(0x62, 0x06);
			#endif
	#elif (MainBoardType==MainBoard_715G5265_T||(MainBoardType == MainBoard_SLB1PTR||MainBoardType == MainBoard_JRYL1PTR_LF))	//petit 20121030	
			#if 0//ScanKey
			mStar_WriteByte(0x62, 0x00);
			#else
			mStar_WriteByte(0x62, BIT2|BIT1);
			#endif
   	 	#else
        	mStar_WriteByte(BK1_62, 0x06); //2005-04-11
    	#endif
        mStar_WriteByte(BK1_63, 0x04);
        //  mStar_WriteByte(0x66,0x37);  //Double Speed
        //  mStar_WriteByte(0x66,0x3B);  //Double Speed
		#ifdef TSUMXXT		//110311 Modify
		//<Janus>2010/07/19: Analog teams said reducing XTAL gain can reduce MPLL jitter and waterfall noise.
		//	 The minimal XTAL gain of CHIP_TSUMT is 5 times larger than CHIP_TSUMXXN and CHIP_TSUMQ.
		mStar_WriteByte(REGBK, REGBANK4);
		mStar_WriteByteMask(BK4_21, 0, (BIT6|BIT5));
		mStar_WriteByte(REGBK, REGBANK1);
		#endif  // end of #if( ChipID == CHIP_TSUMT )

        #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
            mStar_WriteByte ( REGBK, REGBANKADC );    
            msWriteByteMask(BK1_68, BIT6       ,  BIT6);//B.1.8 Scar, 20090424   /*JT modify it from BK1_67[7] change to BK1_68[6]*/
            msWriteByteMask(BK1_66, BIT0       ,  BIT0);//B.14.4 Scar,20090424
            ForceDelay1ms ( 1 );        
            #if 1//DoubleSpeed
            msWriteByteMask(BK1_67, BIT4        ,  BIT5|BIT4);
            ForceDelay1ms ( 1 );
            msWriteByteMask(BK1_66, BIT3|BIT2, BIT3|BIT2|BIT1);
            ForceDelay1ms ( 1 );
            msWriteByteMask(BK1_66, 0             ,  BIT0);
            ForceDelay1ms ( 1 );
            #else
            msWriteByteMask(BK1_66, BIT0        ,  BIT0);
            ForceDelay1ms ( 1 );
            #endif    
            //mStar_WriteByte ( REGBK, REGBANKSCALER );   
        #else
            #if 0
            mStar_WriteByte(BK1_66, 0x27); // 0519  Andy
            ForceDelay1ms(1);
            mStar_WriteByte(BK1_66, 0x2A);
            ForceDelay1ms(1);
            mStar_WriteByte(BK1_66, 0x2B); // 0519  Andy
            #else
            mStar_WriteByte(BK1_66, 0x57);//0x47 // BK1 66h[7:5]: 1/mcu operating frequency
            ForceDelay1ms(1);
            mStar_WriteByte(BK1_66, 0x5A);//0x4A
            ForceDelay1ms(1);
            mStar_WriteByte(BK1_66, 0x5B); //0x4B
            //EON\MXIC\SST Flash CS High Time Min=100ns
            //PMC Flash CS High Time Min=25ns
            #endif     
        #endif            
        mStar_WriteByte(BK1_60, 0x20);
        #if ENABLE_HDCP
            mStar_WriteByte(BK1_37, 0x80);
        #endif
    #endif
    
    #if MWEFunction
    // 090409
    //mStar_WriteByte(0x3f, 0x18); // 2004/9/27
    mStar_WriteByte(BK1_3F, 0x00);
    #endif
    
    #if MainBoardType==MainBoard_2883_B_X_X_2|| MainBoardType == MainBoard_2883_1B||\
          MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A||\
          MainBoardType==MainBoard_715G3598_1PLL
          #ifdef TSUMXXNX
            mStar_WriteByte(BK1_68, mStar_ReadByte(BK1_68)|(BIT3|BIT2));
          #else
            mStar_WriteByte(BK1_68, mStar_ReadByte(BK1_68)|(BIT0|BIT4));
          #endif
    #elif (MainBoardType==MainBoard_712G3244_2)
	   mStar_WriteByte(BK1_68, mStar_ReadByte(BK1_68)|(BIT3|BIT2));
    #elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_2883_B_X_X_1||MainBoardType==MainBoard_715G3329_1_2||\
		MainBoardType==MainBoard_715G4734)	
           mStar_WriteByte(BK1_68, mStar_ReadByte(BK1_68) | (BIT2|BIT3));	// p06/p07 output 12ma
    #elif MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611
           mStar_WriteByte(BK1_68, mStar_ReadByte(BK1_68) | (BIT1|BIT2));	// p06/p07 output 12ma
    #elif MainBoardType==MainBoard_715G4175_M0A
	     mStar_WriteByte(BK1_68, mStar_ReadByte(BK1_68) | (BIT2|BIT3));	// p06/p07 output 12ma
    #endif

    #ifdef LEDBrightCtrlByScalerPWM
        #if MainBoardType == MainBoard_715G3603_M0A
        mStar_WriteByte(BK1_69, BIT1); // enable pwm1 open-drain type for LED birght
        #elif MainBoardType == MainBoard_715G3598_M0A
            #ifdef TSUMXXNX
                #if AudioFunc
                mStar_WriteByte(BK1_69, BIT2|BIT0);
                #else
                mStar_WriteByte(BK1_69, BIT2); // enable pwm2 open-drain type for LED birght
                #endif
            #else
            mStar_WriteByte(BK1_69, BIT0); // enable pwm0 open-drain type for LED birght
            #endif	
		#elif MainBoardType == MainBoard_715G3598_1PLL	
			mStar_WriteByte(BK1_69, BIT2); // enable pwm2 open-drain type for LED birght
        #endif
    #else
		#if MainBoardType == MainBoard_JRYL1PTR_LF
		mStar_WriteByte(BK1_69, BIT0);
		#endif
    #endif

    #if ScanKey
    mStar_WriteByte(BK1_79, mStar_ReadByte(BK1_79) & ~BIT5); //disable adc function
    #else
    mStar_WriteByte(BK1_79, mStar_ReadByte(BK1_79) | BIT5); //enable adc function
    #endif

    #ifdef TSUMXXQX
    mStar_WriteByte(BK1_41, 0x10);
    mStar_WriteByte(BK1_42, 0x10);
    mStar_WriteByte(BK1_43, 0x00);
    
    mStar_WriteByte(BK1_C6, 0xE3); // B-ch: updated DVI phase per 100 lines
    mStar_WriteByte(BK1_C7, 0xE3); // G-ch: updated DVI phase per 100 lines
    mStar_WriteByte(BK1_C8, 0xE3); // R-ch: updated DVI phase per 100 lines
    mStar_WriteByte(BK1_C9, 0x10); // [4]: enable DVI Vsync filter
    // HDCP setting
    mStar_WriteByte(BK1_46, 0x70); // [6:4]: During Vsync blanking to detect ENC_EN in OESS mode
    mStar_WriteByte(BK1_1D, BIT0); // enable overwrite BK1 E4
    #endif

	#ifdef TSUMXXT//#if (ChipID == TSUMXXT) // Rose		//110311 Modify
		msADC_init(FALSE);
		msADC_ADCOffsetGainMismatchCal();
    	#ifdef DGAINCALWITHOUTINPUT
		msADC_ADCAutoGainCalwithoutInput();
    	#endif
	#endif

    mStar_WriteByte(TESTEN, 0); //0x22);
    mStar_WriteByte(REGBK, REGBANKSCALER); // switch to scaler bank
}
//*******************************************************************
// Function Name: mStar_InitTCON
//
// Decscription: Initialize TCON bank registers according panel type
//
// callee: mStar_WriteByte(), mStar_WriteWord in ms_rwreg.c
//
// caller: mStar_Init() in mStar.c
//*******************************************************************
void mStar_InitTCON(void)
{
    #if PanelLVDS // for LVDS Panel
    mStar_WriteByte(REGBK, REGBANKTCON);
    mStar_WriteByte(OFC1, 2); // Enable Early end function
    #if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
        mStar_WriteByte ( G0VST_H, 0x01 );
        mStar_WriteByte ( G0VST_L, 0x01 );
        mStar_WriteByte ( 0x6C, mStar_ReadByte(0x6C) &(~BIT6));       
       #endif//#else
	  #if 0//EnablePanelServiceMenu
        if(!(PanelOutputControl2&BIT0))////101124
            mStar_WriteByte(BK2_6B, mStar_ReadByte(BK2_6B)|BIT0);
	else
		mStar_WriteByte(BK2_6B, mStar_ReadByte(BK2_6B)&(~BIT0)); 
        #else
             #if ((MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_712G3244_2||MainBoardType==MainBoard_715G3529||MainBoardType==MainBoard_715G3529_T||MainBoardType==MainBoard_715G3959_T||\
			MainBoardType==MainBoard_715G4734)&&PanelDualPort)
             mStar_WriteByte(BK2_6B, mStar_ReadByte(BK2_6B)|BIT0);
             #endif
	 #endif
		//#if (!PanelDualPort)//101124
            //mStar_WriteByte(BK2_6B, mStar_ReadByte(BK2_6B)|BIT0);
            //#endif
	
        //  mStar_WriteWord(G9VST_H, 7);
        //  mStar_WriteWord(G9VEND_H, 0x705);
        //  mStar_WriteByte(G9CTRL, 0x4);
        #if !EnablePanelServiceMenu
        mStar_WriteWord(GAVST_H, GPOA_VStart);
        mStar_WriteWord(GAVEND_H, GPOA_VEnd);
        mStar_WriteByte(GACTRL, GPOA_Control);
        #endif    
 
    mStar_WriteByte(REGBK, REGBANKSCALER);
    #elif PanelminiLVDS
        mStar_WriteByte(REGBK, REGBANKTCON);
        mStar_WriteByte(OFC1, TCON_Format1);
        mStar_WriteByte(OFC2, (ESPP_B &ESP_AfterData) | (ESP_Offset << 4) | (OSPP_B &OSP_AfterData) | OSP_Offset);
        mStar_WriteByte(ODPC, (PanelOESPCurrent << 6) | (PanelOEGCurrent << 4)); // setup output driving current
        mStar_WriteByte(ODC, (PanelEINVCurrent << 6) | (PanelOINVCurrent << 4)); // setup output driving current
        mStar_WriteByte(IFCTRL, OuputFormat);// | BIT6); //0x28);
        
        #if MainBoardType==MainBoard_715G3289_C2
        mStar_WriteByte(BK2_D0, 0x11); // 0x13 090522
        mStar_WriteByte(BK2_D1, 0x33); // 0xaa 090602 color // 0x88 090602// 0x44 090522
        mStar_WriteByte(BK2_D2, 0x37); // 0xbf // 090522
        mStar_WriteByte(BK2_D3, 0x42);
        mStar_WriteByte(BK2_D4, 0x00);
        mStar_WriteByte(BK2_D5, 0x00);
        mStar_WriteByte(BK2_D6, 0x00);
        mStar_WriteByte(BK2_D7, 0x00);
        mStar_WriteByte(BK2_D8, 0xA7);
        mStar_WriteByte(BK2_D9, 0x32);
        mStar_WriteByte(BK2_DA, 0x00);
        mStar_WriteByte(BK2_DB, 0x00);
        mStar_WriteByte(BK2_DC, 0x02);
        mStar_WriteByte(BK2_DD, 0x04);
        mStar_WriteByte(BK2_DE, 0x06); // 0x00 090602 color
        mStar_WriteByte(BK2_DF, 0x00);
        #endif
        
        // POL
        mStar_WriteWord(G0VST_H, GPO0_VStart);
        mStar_WriteWord(G0VEND_H, GPO0_VEnd);
        mStar_WriteWord(G0HST_H, GPO0_HStart);
        mStar_WriteWord(G0HEND_H, GPO0_HEnd);
        mStar_WriteByte(G0CTRL, GPO0_Control);
        // LP
        mStar_WriteWord(G1VST_H, GPO1_VStart);
        mStar_WriteWord(G1VEND_H, GPO1_VEnd);
        mStar_WriteWord(G1HST_H, GPO1_HStart);
        mStar_WriteWord(G1HEND_H, GPO1_HEnd);
        mStar_WriteByte(G1CTRL, GPO1_Control);
        // STV
        mStar_WriteWord(G2VST_H, GPO2_VStart);
        mStar_WriteWord(G2VEND_H, GPO2_VEnd);
        mStar_WriteWord(G2HST_H, GPO2_HStart);
        mStar_WriteWord(G2HEND_H, GPO2_HEnd);
        mStar_WriteByte(G2CTRL, GPO2_Control);
        // CLKV
        mStar_WriteWord(G3VST_H, GPO3_VStart);
        mStar_WriteWord(G3VEND_H, GPO3_VEnd);
        mStar_WriteWord(G3HST_H, GPO3_HStart);
        mStar_WriteWord(G3HEND_H, GPO3_HEnd);
        mStar_WriteByte(G3CTRL, GPO3_Control);
        
	#if PanelType == PanelTPM215HW01H1PS1
        // OE
        mStar_WriteWord(G4VST_H, 0);
        mStar_WriteWord(G4VEND_H, 0);
        mStar_WriteWord(G4HST_H, 0);
        mStar_WriteWord(G4HEND_H, 0);
        mStar_WriteByte(G4CTRL, 1);

	  // GV_Off
	  mStar_WriteWord(G5VST_H, 0);	
	  mStar_WriteWord(G5VEND_H, 0);
	  mStar_WriteWord(G5HST_H, 0);
	  mStar_WriteWord(G5HEND_H, 0);
	  mStar_WriteByte(G5CTRL, 1);
	#endif        
        
        // PVS
        mStar_WriteWord(G9VST_H, GPO9_VStart);
        mStar_WriteWord(G9VEND_H, GPO9_VEnd);
        mStar_WriteWord(G9HST_H, GPO9_HStart);
        mStar_WriteWord(G9HEND_H, GPO9_HEnd);
        mStar_WriteByte(G9CTRL, GPO9_Control);
        mStar_WriteByte(REGBK, REGBANKSCALER);    
    #elif PanelRSDS
	 #if MainBoardType==MainBoard_2670_C     //Amy2007.12.19   13:49:34
	 mStar_WriteByte(REGBK, REGBANKTCON);
    	  mStar_WriteByte(OFC1, 0x01);
    	  mStar_WriteByte(OFC2, 0x11);
    	  mStar_WriteByte(ODPC, (PanelOESPCurrent<<6)|(PanelOEGCurrent<<4)); // setup output driving current
    	  mStar_WriteByte(ODC, (PanelEINVCurrent<<6)|(PanelOINVCurrent<<4)| // setup output driving current
    	                 (RSBMLSW_B&RSBMLSW)|(RSAMLSW_B&RSAMLSW)); // MSB/LSB swap Bruce 0220.2004
    	                 
    	  mStar_WriteByte(IFCTRL, OuputFormat);		//kidd0818	, don't output data
    
    	  mStar_WriteWord(G0VST_H, GPO0_VStart);
    	  mStar_WriteWord(G0VEND_H, GPO0_VEnd);
    	  mStar_WriteWord(G0HST_H, GPO0_HStart);
    	  mStar_WriteWord(G0HEND_H, GPO0_HEnd);
    	  mStar_WriteByte(G0CTRL, GPO0_Control);
    
    	  // LP	// STB
    	  mStar_WriteWord(G1VST_H, GPO1_VStart);
    	  mStar_WriteWord(G1VEND_H, GPO1_VEnd);
    	  mStar_WriteWord(G1HST_H, GPO1_HStart);
    	  mStar_WriteWord(G1HEND_H, GPO1_HEnd);
    	  mStar_WriteByte(G1CTRL, GPO1_Control);
    	  // STV
    	  mStar_WriteWord(G2VST_H, GPO2_VStart);
    	  mStar_WriteWord(G2VEND_H, GPO2_VEnd);
    	  mStar_WriteWord(G2HST_H, GPO2_HStart);
    	  mStar_WriteWord(G2HEND_H, GPO2_HEnd);
    	  mStar_WriteByte(G2CTRL, GPO2_Control);
    	  // CLKV
    	  mStar_WriteWord(G3VST_H, GPO3_VStart);
    	  mStar_WriteWord(G3VEND_H, GPO3_VEnd);
    	  mStar_WriteWord(G3HST_H, GPO3_HStart);
    	  mStar_WriteWord(G3HEND_H, GPO3_HEnd); //
    	  mStar_WriteByte(G3CTRL, GPO3_Control);
    	  // OE
    	  mStar_WriteWord(G4VST_H, GPO4_VStart);
    	  mStar_WriteWord(G4VEND_H, GPO4_VEnd);
    	  mStar_WriteWord(G4HST_H, GPO4_HStart); //
    	  mStar_WriteWord(G4HEND_H, GPO4_HEnd); //
    	  mStar_WriteByte(G4CTRL, GPO4_Control);
    	  // GV_Off
    	  mStar_WriteWord(G5VST_H, GPO5_VStart);		// William 20050922 reserve GPO5 for frame toggle
    	  mStar_WriteWord(G5VEND_H, GPO5_VEnd);
    	  mStar_WriteWord(G5HST_H, GPO5_HStart);
    	  mStar_WriteWord(G5HEND_H, GPO5_HEnd);
    	  mStar_WriteByte(G5CTRL, GPO5_Control);
          mStar_WriteByte(REGBK, REGBANKSCALER);
	#elif PanelType==PanelCMO220Z1M01
	  mStar_WriteByte(REGBK, REGBANKTCON);
	  mStar_WriteByte(OFC1, 0x01);
	  mStar_WriteByte(OFC2, 0x11);
	  mStar_WriteByte(ODPC, (PanelOESPCurrent<<6)|(PanelOEGCurrent<<4)); // setup output driving current
	  mStar_WriteByte(ODC, (PanelEINVCurrent<<6)|(PanelOINVCurrent<<4)| // setup output driving current
	                 (RSBMLSW_B&RSBMLSW)|(RSAMLSW_B&RSAMLSW)); // MSB/LSB swap Bruce 0220.2004

	  mStar_WriteByte(IFCTRL, OuputFormat);		//kidd0818	, don't output data
	  
	  mStar_WriteWord(G0VST_H, GPO0_VStart);
	  mStar_WriteWord(G0VEND_H, GPO0_VEnd);
	  mStar_WriteWord(G0HST_H, GPO0_HStart);
	  mStar_WriteWord(G0HEND_H, GPO0_HEnd);
	  mStar_WriteByte(G0CTRL, GPO0_Control);
	  
	  // LP	// STB
	  mStar_WriteWord(G1VST_H, GPO1_VStart);
	  mStar_WriteWord(G1VEND_H, GPO1_VEnd);
	  mStar_WriteWord(G1HST_H, GPO1_HStart);
	  mStar_WriteWord(G1HEND_H, GPO1_HEnd);
	  mStar_WriteByte(G1CTRL, GPO1_Control);
	  // STV
	  mStar_WriteWord(G2VST_H, GPO2_VStart);
	  mStar_WriteWord(G2VEND_H, GPO2_VEnd);
	  mStar_WriteWord(G2HST_H, GPO2_HStart);
	  mStar_WriteWord(G2HEND_H, GPO2_HEnd);
	  mStar_WriteByte(G2CTRL, GPO2_Control);
	  // CLKV
	  mStar_WriteWord(G3VST_H, GPO3_VStart);
	  mStar_WriteWord(G3VEND_H, GPO3_VEnd);
	  mStar_WriteWord(G3HST_H, GPO3_HStart);
	  mStar_WriteWord(G3HEND_H, GPO3_HEnd);
	  mStar_WriteByte(G3CTRL, GPO3_Control);
	  // OE
	  mStar_WriteWord(G4VST_H, GPO4_VStart);
	  mStar_WriteWord(G4VEND_H, GPO4_VEnd);
	  mStar_WriteWord(G4HST_H, GPO4_HStart);
	  mStar_WriteWord(G4HEND_H, GPO4_HEnd);
	  mStar_WriteByte(G4CTRL, GPO4_Control);
	  // GV_Off
	  mStar_WriteWord(G5VST_H, GPO5_VStart);		// William 20050922 reserve GPO5 for frame toggle
	  mStar_WriteWord(G5VEND_H, GPO5_VEnd);
	  mStar_WriteWord(G5HST_H, GPO5_HStart);
	  mStar_WriteWord(G5HEND_H, GPO5_HEnd);
	  mStar_WriteByte(G5CTRL, GPO5_Control);
         mStar_WriteByte(REGBK, REGBANKSCALER);
	#else
        mStar_WriteByte(REGBK, REGBANKTCON);
        mStar_WriteByte(OFC1, TCON_Format1);
        mStar_WriteByte(OFC2, (ESPP_B &ESP_AfterData) | (ESP_Offset << 4) | (OSPP_B &OSP_AfterData) | OSP_Offset);
        mStar_WriteByte(ODPC, (PanelOESPCurrent << 6) | (PanelOEGCurrent << 4)); // setup output driving current
        mStar_WriteByte(ODC, (PanelEINVCurrent << 6) | (PanelOINVCurrent << 4)); // setup output driving current
        mStar_WriteByte(IFCTRL, OuputFormat);// | BIT6); //0x28);
        // POL
        mStar_WriteWord(G0VST_H, GPO0_VStart);
        mStar_WriteWord(G0VEND_H, GPO0_VEnd);
        mStar_WriteWord(G0HST_H, GPO0_HStart);
        mStar_WriteWord(G0HEND_H, GPO0_HEnd);
        mStar_WriteByte(G0CTRL, GPO0_Control);
        // LP
        mStar_WriteWord(G1VST_H, GPO1_VStart);
        mStar_WriteWord(G1VEND_H, GPO1_VEnd);
        mStar_WriteWord(G1HST_H, GPO1_HStart);
        mStar_WriteWord(G1HEND_H, GPO1_HEnd);
        mStar_WriteByte(G1CTRL, GPO1_Control);
        // STV
        mStar_WriteWord(G2VST_H, GPO2_VStart);
        mStar_WriteWord(G2VEND_H, GPO2_VEnd);
        mStar_WriteWord(G2HST_H, GPO2_HStart);
        mStar_WriteWord(G2HEND_H, GPO2_HEnd);
        mStar_WriteByte(G2CTRL, GPO2_Control);
        // CLKV
        mStar_WriteWord(G3VST_H, GPO3_VStart);
        mStar_WriteWord(G3VEND_H, GPO3_VEnd);
        mStar_WriteWord(G3HST_H, GPO3_HStart);
        mStar_WriteWord(G3HEND_H, GPO3_HEnd);
        mStar_WriteByte(G3CTRL, GPO3_Control);
        
	#if PanelType == PanelTPM215HW01
        // OE
        mStar_WriteWord(G4VST_H, 0);
        mStar_WriteWord(G4VEND_H, 0);
        mStar_WriteWord(G4HST_H, 0);
        mStar_WriteWord(G4HEND_H, 0);
        mStar_WriteByte(G4CTRL, 1);

	  // GV_Off
	  mStar_WriteWord(G5VST_H, 0);	
	  mStar_WriteWord(G5VEND_H, 0);
	  mStar_WriteWord(G5HST_H, 0);
	  mStar_WriteWord(G5HEND_H, 0);
	  mStar_WriteByte(G5CTRL, 1);
	#else
        // OE
        mStar_WriteWord(G4VST_H, GPO4_VStart);
        mStar_WriteWord(G4VEND_H, GPO4_VEnd);
        mStar_WriteWord(G4HST_H, GPO4_HStart);
        mStar_WriteWord(G4HEND_H, GPO4_HEnd);
        mStar_WriteByte(G4CTRL, GPO4_Control);
	#endif        
        
        // PVS
        mStar_WriteWord(G9VST_H, GPO9_VStart);
        mStar_WriteWord(G9VEND_H, GPO9_VEnd);
        mStar_WriteWord(G9HST_H, GPO9_HStart);
        mStar_WriteWord(G9HEND_H, GPO9_HEnd);
        mStar_WriteByte(G9CTRL, GPO9_Control);
        mStar_WriteByte(REGBK, REGBANKSCALER);
     #endif
#endif
}

#if 0
    BYTE code Pattern0[] =
    {
        0x0C, 0x00, 0x10, 0x10, 0x10, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x00
    };
    void mStar_SetupPattern(void)
    {
        BYTE i;
        mStar_WriteByte(OSDRA, 0x15);
        for (i = 0; i < sizeof(Pattern0); i++)
            mStar_WriteByte(OSDRD, Pattern0[i]);
    }
#endif
//*******************************************************************
// Function Name: mStar_SetScalingFilter
//
// Decscription: Setup scaling filter for sharpness function
//
// callee: mStar_WriteByte() in ms_rwreg.c
//
// caller: mStar_SetupMode() in mStar.c
//*******************************************************************
#if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
//@brief  Set vertical scaling filter for sharpness.
//@param  u8Filter ranges from 0(no-scaling), 1(scaling up), 2(scaling down)
void mStar_SetVScalingFilter(BYTE u8Filter)
{
#define V_COEF_NUM (5)
    BYTE code tVScalingCoeff[ 3 ][ V_COEF_NUM ] =
    {
    /*0x5A*/   { 0x00, 0x04, 0x08, 0x0C, 0x10 }, /* VDSUSG */
    /*0x57*/   { 0x00, 0x02, 0x0A, 0x27, 0x51 }, /* CB(0) */   /*VSPC*/    /*Scaling up */
    /*0x57*/   { 0x1F, 0x2B, 0x37, 0x4F, 0x67 }, /* BS(0.75) */ /*VSPC*/   /*Scaling down*/
    };

    //v filter
    if( (u8Filter&0x0F)==0 )
    {
        msWriteByteMask( BK0_5A,  0, (BIT1|BIT0) ); /* Vertical scaling filter is Bilinear mode */
    }
    else
    {
        BYTE i;
        msWriteByteMask( BK0_5A,  1, (BIT1|BIT0) ); /* Vertical scaling filter is  Register define mode */
        for ( i = 0; i < V_COEF_NUM; i++ )
        {
            msWriteByteMask( BK0_5A,  (tVScalingCoeff[ 0 ][ i ]), (BIT4|BIT3|BIT2) );     /*Bit4~2*/
            mStar_WriteByte( BK0_57 ,  tVScalingCoeff[ u8Filter&0x0F ][ i ] );
        }
    }
}
//@brief  Set horizontal scaling filter for sharpness.
//@param  u8Filter ranges from 0(no-scaling), 1(scaling up), 2(scaling down)
void mStar_SetHScalingFilter(BYTE u8Filter)
{
    BYTE code tHFilterTbl[]=
    {
        0x00,   // no-scaling
        0x02,   // upscaling
        0x04    // downscaling
    };

    //printData("  u8Filter=%x",u8Filter);
    mStar_WriteByte( BK0_56, tHFilterTbl[u8Filter] );
}
void mStar_SetScalingFilter(void)
{
    WORD nPxlImgHDE = mStar_ReadWord( SPRHDC_H ); // image horizontal DE in pixel
    WORD nPxlImgVDE = mStar_ReadWord( SPRVDC_H ); // image vertical   DE in pixel

    // horizontal
    if( nPxlImgHDE == PanelWidth )
    {
        mStar_SetHScalingFilter( 0 );  // no-scaling
    }
    else if( nPxlImgHDE < PanelWidth )
    {
        mStar_SetHScalingFilter( 1 );  // scaling-up
    }
    else    // scaling-down
    {
        mStar_SetHScalingFilter( 2 );  // scaling-down
    }

    // vertical
    if( nPxlImgVDE == PanelHeight )
    {
        mStar_SetVScalingFilter( 0 );   // no-scaling
    }
    else if( nPxlImgVDE < PanelHeight )
    {
        mStar_SetVScalingFilter( 1 );   // scaling-up
    }
    else
    {
        mStar_SetVScalingFilter( 2 );   // scaling-down
    }
    
    // 100604 coding for sharpness
    mStar_WriteByte(PFEN, 0x01);
	#ifdef TSUMXXT	//110608 Modify
	//#if (ModelName == ENVISION_H2030WS)
    //mStar_WriteByte(PFCOEF, 0x08);
	//#else
    mStar_WriteByte(PFCOEF, 0x09);
	//#endif
	#else
    mStar_WriteByte(PFCOEF, 0x99);
	#endif
   
}

#else // !TSUMXXNX && !TSUMXXQX

#if ChipID==ChipNone
BYTE code FilterTbl[4][6] =
{
    // SHF  SFV   HDSUSG  HDSUSL   VDSUSG  VDSUSL
    // 56    57    58      59       5A      5B     5C       5D
    { 0x00, 0x00, 0x00,   0x00,    0x00,   0x00}, // 0
    { 0xAA, 0x11, 0x44,   0x06,    0x22,   0x01}, // U
    { 0xAA, 0x11, 0x33,   0x06,    0x22,   0x01}, // Win1
    { 0x99, 0x11, 0x66,   0x06,    0x22,   0x01}, // Win2

};
#else
BYTE code FilterTbl[4][8] =
{
    // SHF  SFV   HDSUSG  HDSUSL   VDSUSG  VDSUSL
    // 56    57    58      59       5A      5B     5C       5D
    { 0x00, 0x00, 0x00,   0x00,    0x00,   0x00,   0x01,   0x99}, // 0
    
#if (ModelName==AOC_919P2_Asia||ModelName==AOC_919P2_Europe||ModelName==AOC_930FWz)
    { 0x11, 0x0, 0x00,   0x00,    0x0,   0x00,   0x00,   0x00}, // U
    { 0x11, 0x0, 0x00,   0x00,    0x0,   0x00,   0x00,   0x00}, // Win1
    { 0x11, 0x0, 0x00,   0x00,    0x0,   0x00,   0x01,   0xAA}, // Win2
#else
    { 0x55, 0x11, 0x00,   0x06,    0x22,   0x01,   0x01,   0x99}, // U
    { 0x55, 0x11, 0x11,   0x06,    0x22,   0x01,   0x01,   0x99}, // Win1
    { 0x33, 0x11, 0x00,   0x06,    0x22,   0x01,   0x01,   0x99}, // Win2
#endif

};
#endif
void mStar_SetScalingFilter(BYTE filter)
{
    BYTE i;
    for (i = 0; i < sizeof(FilterTbl) / 4; i++)
    {
        if((SFH+i)==BK0_5A)                    //EDIT BY lizzie.ding,100524  from AP NOTE
            mStar_WriteByte(SFH+i, (mStar_ReadByte(SFH+i)&0xC0) |FilterTbl[filter][i]);
        else if((SFH+i)==BK0_5C)
            mStar_WriteByte(SFH+i, (mStar_ReadByte(SFH+i)&BIT3) |FilterTbl[filter][i]);
        else if((SFH+i)==BK0_5B)
            mStar_WriteByte(SFH+i, (mStar_ReadByte(SFH+i)&0xC0) |FilterTbl[filter][i]);
        else
        mStar_WriteByte(SFH + i, FilterTbl[filter][i]);
    }
}
#endif
//==================================================================
extern void SetColorBoostWindow(BYTE mode);
extern void SetColorBoostWin(BYTE mode);
extern BYTE code t_MWEDLC_Linear_Table[];
extern BYTE code t_Normal_ColorSettingTable[];
void ColorBoost(void)
{
    if (ColorAutoDetectFlag)
    {
        Set_ColorAutoDetectFlag();
        LoadDLCTable(t_MWEDLC_Linear_Table);
        msDlcOnOff(_DISABLE);
    }
    else
    {
        Clr_ColorAutoDetectFlag();
        msDlcOnOff(_DISABLE);
    }
#if ENABLE_HDMI
    if (BlueFlag || SkinFlag || GreenFlag || ColorAutoDetectFlag || FullEnhanceFlag)
        SetColorBoostWin(COLORBOOST_FULL);
    else
        SetColorBoostWin(COLORBOOST_OFF);
#else
    if (BlueFlag || SkinFlag || GreenFlag || ColorAutoDetectFlag || FullEnhanceFlag)
        SetColorBoostWindow(COLORBOOST_FULL);
    else
        SetColorBoostWindow(COLORBOOST_OFF);
#endif
}

//*******************************************************************
// Function Name: mStar_SetUserPref
//
// Decscription: Restore user setting from NVRAM
//
// callee: mStar_WriteByte() in ms_rwreg.c
//
// caller: mStar_SetupMode() in mStar.c
//*******************************************************************
void mStar_SetUserPref(void)
{
    // Restore contrast & color temperature setting
    //mStar_AdjustContrast(UserPrefContrast);
    //mStar_AdjustBrightness(UserPrefBrightness);
    // 2006/10/26 6:47PM by Emily     msAdjustSharpness(UserPrefSubSharpness, 1);
    //SetECO(); Move down By zhifeng.wu
    if (UserPrefDcrMode)
    {
    #if 1
	 UserPrefDcrMode = 0;
	 AdjustDcrMode(0);
	 
        Clr_DcrOpenScikeFlag();//20151225 lixw
        // 091005 coding
        if(FreeRunModeFlag)
            mStar_AdjustBrightness(100);
           
    #else
        UserPrefDcrMode = 1;
        UserPrefECOMode = ECO_Standard;
        ReadColorTempSetting();
        msAdjustSubBrightness(UserPrefSubBrightness);
        msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
        mStar_WriteByte(0x00, 0x03);
        mStar_WriteByte(0x72, 0x70);
        mStar_WriteByte(0x73, 0x70);
        mStar_WriteByte(0x74, 0x70);
        mStar_WriteByte(0x00, 0x00);
        msDlcInit(PanelWidth, PanelHeight);
        msSetDlcStrength(g_LowStrength, g_HighStrength);
        LoadDLCTable(t_MWEDLC_Linear_Table);
        msDlcOnOff(1);
        msAccSetup(0, PanelWidth, 0, PanelHeight);
        SetColorEnhance(1);
    #endif
        return ;
    }
    else if (UserPrefColorFlags)
    {
        if (DemoFlag)
        {
        	#if 0
            UserPrefBFHSize = PanelWidth / 2;
            UserPrefBFVSize = PanelHeight;
            UserPrefBFHStart = 0;
            UserPrefBFVStart = 0;
            msAccSetup(UserPrefBFHStart, UserPrefBFHSize, UserPrefBFVStart, UserPrefBFVSize);
    	#if ENABLE_HDMI
	    if(gScInfo.InputColor == INPUT_RGB)
			msAccOnOff(1);
	    else 
		mStar_WriteByte(0x5C, mStar_ReadByte(0x5C)|BIT3);
		#else
            msAccOnOff(1);
		#endif
		#else
		Clr_DemoFlag();
		AdjustDemoMode(0);
		#endif
        }
        else if (PictureBoostFlag)
            {
                SetBFSize(UserPrefBFSize);//SetBFSize();
                msAdjustSharpness(11, 1);
                msAdjustVideoHue(UserPrefHue);
                msAdjustVideoSaturation(UserPrefSaturation);
                msAdjustSubBrightness(UserPrefSubBrightness);
                msAdjustVideoContrast(UserPrefSubContrast);
            }
        else if (BlueFlag || SkinFlag || GreenFlag || ColorAutoDetectFlag || FullEnhanceFlag)
            ColorBoost();
        SetColorEnhance(1);
        if (ColorAutoDetectFlag)
            msDlcOnOff(1);
        else
            msDlcOnOff(0);
    }
    else
        msAccOnOff(0);

#if Enable_Lightsensor
    // 091002 coding, initial variable of light sensor
    // 091005 coding
    if(UserPrefiCareMode == LightSensorOff)
    	{
        SetECO();
    	}
    else
    {
        if(FreeRunModeFlag)
        {
            mStar_AdjustBrightness(100);
        }
        else
        {
           SetColorTemp();// 091028 for AC OFF/OFF,Contrast error    by lizzie
            mStar_AdjustBrightness(TempBrightness);
	   
        }
    }
#else
    // 091005 coding
    if(FreeRunModeFlag)
        mStar_AdjustBrightness(100);
    else
        SetECO(); //zhifeng.wu
#endif    

    // 2006/10/16 12:39AM by Emily     mStar_InitGamma(); // programming gamma table
    // Restore other settings here
}
void CheckADC(void) // check DVI phase overflow
{
    BYTE i,j,temp,ErrorCount=0;
    BYTE reg_bank;

    reg_bank=mStar_ReadByte(REGBK);

    mStar_WriteByte(REGBK, REGBANKADC);
    temp=mStar_ReadByte(TESTEN);
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            mStar_WriteByte(TESTEN,j);
            if((mStar_ReadByte(0x1A)==0x7f)||(mStar_ReadByte(0x1A)==0x80))
                ErrorCount++;
        }
    }
    mStar_WriteByte(TESTEN, temp);
    mStar_WriteByte(REGBK, REGBANKSCALER);
    if(ErrorCount>=i)
    {
        mStar_WriteByte(SWRST, ADCR_B);
        mStar_WriteByte(SWRST, 0);
    }
    mStar_WriteByte(REGBK, reg_bank);
}
#if MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A  || MainBoardType==MainBoard_715G3959_T
// bk1 a0/a3 (r/w): output enable
// bk1 a1/a4 (r/w): output value
// bk1 a2/a5 (r): input value
void SetScalerGPIO(BYTE pinX, bit HiLow)
{
    BYTE xdata tempBank = mStar_ReadByte(0x00);
    mStar_WriteByte(REGBK, 0x01);

    if(pinX >= 11) // GPIO 11/12 located at bit 5/6
    {
        pinX -= 6;
        if(HiLow)
            mStar_WriteByte(0xa4, mStar_ReadByte(0xa4) | (1<<pinX));
        else
            mStar_WriteByte(0xa4, mStar_ReadByte(0xa4) & ~(1<<pinX));

        mStar_WriteByte(0xa3, mStar_ReadByte(0xa3) & ~(1<<pinX));
    }
    else if(pinX >= 8) // GPIO 8~10 located at bit 0~2
    {
        pinX -= 8;
        if(HiLow)
            mStar_WriteByte(0xa4, mStar_ReadByte(0xa4) | (1<<pinX));
        else
            mStar_WriteByte(0xa4, mStar_ReadByte(0xa4) & ~(1<<pinX));

        mStar_WriteByte(0xa3, mStar_ReadByte(0xa3) & ~(1<<pinX));
    }
    else
    {
        if(HiLow)
            mStar_WriteByte(0xa1, mStar_ReadByte(0xa1) | (1<<pinX));
        else
            mStar_WriteByte(0xa1, mStar_ReadByte(0xa1) & ~(1<<pinX));

        mStar_WriteByte(0xa0, mStar_ReadByte(0xa0) & ~(1<<pinX));
    }
    
    mStar_WriteByte(REGBK, tempBank);
}
#endif
#if ChipID==ChipBB
    void CheckLineBuffer(void)
    {
        WORD lineLength;
        BYTE regByte = mStar_ReadByte(LVSST_H);
        BYTE regByte2 = mStar_ReadByte(OSCCTRL1);
        mStar_WriteByte(LVSST_H, regByte | BIT7);
        Delay1ms(40);
        lineLength = mStar_ReadWord(LSLW_H);
        if (lineLength > 0x7F0)
        {
            lineLength = mStar_ReadWord(LFRST_H) + lineLength - 0x7F0;
            mStar_WriteByte(OSCCTRL1, regByte2 &0xFC);
            mStar_WriteWord(LRSV_H, 0x7F0);
            mStar_WriteWord(LFRST_H, lineLength);
            Delay1ms(40);
            mStar_WriteByte(OSCCTRL1, regByte2);
        }
        mStar_WriteByte(LVSST_H, regByte);
    }
#endif
#if EnablePanelServiceMenu||BURST_MODE
#if BrightnessPWM==PWM0C
#define PWMBIT	PWM0BIT8
#define PWMBIT2  PWMS
#elif BrightnessPWM==PWM1C
#define PWMBIT	PWM1BIT8
#define PWMBIT2  PWM1BIT8
#elif BrightnessPWM==PWM2C
	#ifndef USEPWM3
		#define PWMBIT	PWM2BIT8
		#define PWMBIT2	BIT6
	#else
		#define PWMBIT	PWM3BIT8
		#define PWMBIT2	BIT4
	#endif
#elif BrightnessPWM==PWM6
#define PWMBIT	PWM6BIT8
#define PWMBIT2	PWM6BIT9
#endif

void SetPWMFreq(WORD freq)
{
		WORD tempValue;
		tempValue=(WORD)(14318000*2/256/freq)-1;
	#if BrightnessPWM == PWM6
		mStar_WriteByte(REGBK, REGBANKADC);
		mStar_WriteByte(BK1_C8, tempValue&0xFF);
		if(tempValue&0x0100)
			mStar_WriteByte(BK1_C9, mStar_ReadByte(BK1_C9)|PWMBIT);
		else
			mStar_WriteByte(BK1_C9, mStar_ReadByte(BK1_C9) &(~PWMBIT));
		if(tempValue&0x0200)
		{
			mStar_WriteByte(BK1_C9, mStar_ReadByte(BK1_C9)|PWMBIT2);
		}
		else
			mStar_WriteByte(BK1_C9, mStar_ReadByte(BK1_C9) &(~PWMBIT2));
		mStar_WriteByte(REGBK, REGBANKSCALER);
	#else
		mStar_WriteByte(BrightnessPWM-1, tempValue&0xFF);
		if(tempValue&0x0100)
			{
			mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT)|PWMBIT);
			}
		else
			mStar_WriteByte(PWMEXT, mStar_ReadByte(PWMEXT) &(~PWMBIT));
	#if defined TSUMXXNX || defined TSUMXXQX
		if(tempValue&0x0200)
			{
			mStar_WriteByte(GPIO_INT,mStar_ReadByte(GPIO_INT)|PWMDIV_HIGH_EN);
			mStar_WriteByte(PWMDIV0A9, mStar_ReadByte(PWMDIV0A9)|PWMBIT2);
			}
		else
			mStar_WriteByte(PWMDIV0A9, mStar_ReadByte(PWMDIV0A9) &(~PWMBIT2));
        #endif   
	#endif
}
#endif

#if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
// 100527 coding
#ifdef TSUMXXT//#if( ChipID == TSUMXXT )  // TSUMXXT supports to HW MOD calibration
#define USE_MOD_HW_CAL        (1)   // choose hardware(1) or software(0) MOD calibration
#else
#define USE_MOD_HW_CAL       0
#endif

#if( USE_MOD_HW_CAL )
// MOD hardware current calibration
// @pre MOD is not in power-down (BK1_26[7]==0)
// @pre LPLL is not in power-down
void I_Gen_Tuning(void)
{
	//printMsg("I_Gen_Tuning_Start");

	mStar_WriteByte(REGBK, REGBANKTCON);

	mStar_WriteByteMask(BK2_6C, BIT6, BIT6);	   /* Enable MOD self current bias.*/
	mStar_WriteByteMask(BK2_6D,    0, BIT4);	   /* disable MOD HW Calibration */
	mStar_WriteByteMask(BK2_6B,    0, BIT6);	   /* Select calibration source: (0)PADA_OUT[PN]_CH[4](PAD_LHSYNC(LVB3P)/PAD_LVSYNC(LVB3M))    (1)PADA_OUT[PN]_CH[5](PAD_R_ODD_6(LVA0P)/PAD_R_ODD_7(LVA0M)) */
      #if PanelType==PanelLTM200KT01
	mStar_WriteByteMask(BK2_6B, 0x20, BIT5|BIT4); 
      #elif  PanelType==PanelM185B3LA1//20151219 lixw
	 mStar_WriteByteMask(BK2_6C, BIT4, BIT4);//20160121 lixw	
	 mStar_WriteByteMask(BK2_6B, BIT0, BIT0); 
      #else
	mStar_WriteByteMask(BK2_6B, 0x00, BIT5|BIT4);  /* Select calibration target: (00)247mV (01)296mV (10)197mV (11)345mV */
      #endif
	mStar_WriteByteMask(BK2_6D, BIT4, (BIT4|BIT3|BIT1|BIT0));	/*MOD Calibration enable. Stop gatting HW calibration.	No invert HW MOD calibration.  Select clock source for HW MOD calibration. (0):1MHz  (1):0.5MHz  (2):0.25MHz  (3):XTAL */

	while(! ( mStar_ReadByte( BK2_6E ) & BIT7 ) );				// wait for MOD HW calibration done

#if DEBUG_ENABLE
	printData("-----BK2_6E=0x%x\n\r", mStar_ReadByte(BK2_6E));	// BK2_6E[5:0] is calibration result
#endif

	// if you want to modify the output swing, please BK2_6C[5:0] = BK2_6E[5:0].

	mStar_WriteByte(REGBK, REGBANKSCALER);

	//printMsg("I_Gen_Tuning_End");
}
#else
void I_Gen_Tuning(void)
{
    BYTE i, bk0_ab;

    mStar_WriteByte(REGBK, REGBANKTCON);
    mStar_WriteByte (G0VST_H, 0x0D );
    mStar_WriteByte (G0VST_L, 0x0D );
    mStar_WriteByte(BK2_6C, BIT6|BIT5); /*Enable MOD self current bias.*/ /*Tuning output current of self-igen=6b'100000.*/
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(BK0_D0, mStar_ReadByte(BK0_D0)|BIT0); /*Enable LVDS/RSDS Test Mode enable.*/
    bk0_ab = mStar_ReadByte(BK0_AB);
    mStar_WriteByte(BK0_AB, 0xFF);   /*LVDS/RSDS Test mode Data=0xFF.*/
    mStar_WriteByte(BK0_D1, mStar_ReadByte(BK0_D1)|BIT4); //power down LPLL
    mStar_WriteByte(REGBK, REGBANKTCON);
    mStar_WriteByte(BK2_6B, mStar_ReadByte(BK2_6B)|BIT1);    /*MOD Calibration enable.*/
    #if EnablePanelServiceMenu
    mStar_WriteByte(BK2_6B, mStar_ReadByte(BK2_6B)|(PanelLVDSSwing&0xC0)>>2);
    #else
    msWriteByteMask(BK2_6B, BIT4 , BIT5 |BIT4); // 00/01/10/11: 242.37mV/339.33mV/387mV/169.67mV
    #endif
    for(i=0;i<=0x3F;i++)   /*if LVDS swing > 233mV , then BK2_6C[7](C_DDA_OUT)=1*/
    {
        mStar_WriteByte(BK2_6C, (BIT6|i));
        if(mStar_ReadByte(BK2_6C)&BIT7)
        {
            mStar_WriteByte(BK2_6C, ((mStar_ReadByte(BK2_6C)&0x3F)|BIT6));   /* add offset to let lvds swing change to 250mV */
            break;
        }
    }

    if(!(mStar_ReadByte(BK2_6C)&0x3F))
    {
        msWriteByteMask(BK2_6C, 0x28, 0x3F);// 2011.4.1   8:27:22 0x10 for require
    }

    msWriteByteMask(BK2_6B, 0, BIT1);    /*MOD Calibration disable.*/
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(BK0_D1, mStar_ReadByte(BK0_D1)&~BIT4);//enable clock
    mStar_WriteByte(BK0_AB, bk0_ab);   /* restore original value of BK0_AB */
    mStar_WriteByte(BK0_D0, mStar_ReadByte(BK0_D0)&~BIT0); /*Disable MOD self current bias.*/
}
#endif
#endif

#if !IT6251
void ITENonUse(void);
#endif

void UnUseFunc(void)		//110311 Modify for compiler warning
{
	BYTE Temp=0;
	if(Temp)
	{
              #if !IT6251
              ITENonUse();
              #endif
		msAdjustVideoRGB(0,0,0);
		#ifdef TSUMXXT
		msAdcCscFunctionCtrl(0,0);
		msACESetHDTVMode(0);
		msAdjustPCContrast(0,0);
		msAdjustPCRGB( 0,0,0,0 );
		#endif
		;
	}
}

#ifdef PanelSSCFunction   
#define PANEL_F_MODULATION    40 // 30~40KHz
#define PANEL_PERCENTAGE      2  // 0.5~3%

void mStar_SetPanelSSC(void)
{
    DWORD xdata dwFactor;
    WORD xdata wStep;
    WORD xdata wSpan;

    dwFactor = mStar_ReadByte(LPLL_SET_H);
    dwFactor <<= 16;
    dwFactor |= mStar_ReadWord(LPLL_SET_M);
    wSpan = ((14318ul*15/PANEL_F_MODULATION) * 131072ul) / dwFactor;
    wStep = (float)(((dwFactor*PANEL_F_MODULATION*PANEL_PERCENTAGE)/14318ul)*(dwFactor/1500))/131072;

	#ifdef TSUMXXT
    mStar_WriteByte(LPLL_STEP_L, (wStep&0x00FF));
	mStar_WriteByteMask(LPLL_STEP_H, ( wStep >> 8 ), (BIT2|BIT1|BIT0));
	#else
    mStar_WriteWord(LPLL_STEP_H, wStep);
	#endif
    mStar_WriteWord(LPLL_SPAN_H, wSpan);
}

#endif

