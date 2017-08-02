///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvADC.c
/// @author MStar Semiconductor Inc.
/// @brief  ADC Function
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DRVADC_C_
#define _DRVADC_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <math.h>
#include <string.h>
#include "Board.h"
//#include "datatype.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
//#include "DebugMsg.h"
#include "Global.h"
#include "drvADC.h"
#include "misc.h"
//#include "Power.h"
#if ENABLE_DEBUG
#include "GPIO_DEF.h"
#endif

//#include "msEread.h"

//#include "mapi_combo.h"
//#include "AutoFunc.h"

#if ENABLE_VGA_INPUT
// 120106 coding addition, default set adc dithering to disable
#ifndef ENABLE_ADC_DITHERING
#define ENABLE_ADC_DITHERING    1
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define ADC_DEBUG 0

#if ADC_DEBUG & ENABLE_DEBUG
    #define ADC_DPUTSTR(str)    printMsg(str)
    #define ADC_DPRINTF(str, x) printData(str, x)
#else
    #define ADC_DPUTSTR(str)
    #define ADC_DPRINTF(str, x)
#endif

#define   AUTO_GAIN_RGB_TARGET_CODE         182           //0.5/0.7*255=182.1
#define   AUTO_GAIN_TOLERANCE_CODE          0
#define   AUTO_GAIN_RGB_MAXVAL              (AUTO_GAIN_RGB_TARGET_CODE+AUTO_GAIN_TOLERANCE_CODE )
#define   AUTO_GAIN_RGB_MINVAL              (AUTO_GAIN_RGB_TARGET_CODE-AUTO_GAIN_TOLERANCE_CODE )
#define   DADCMISMATCHLOOP                  5  // ADC mismatch calibration loop

#define drvADC_Write2BytesMask  msWrite2ByteMask
#define drvADC_ReadByte         msReadByte

void drvADC_Test_Funct(BYTE tb1, BYTE tb2);
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

typedef struct
{
    BYTE *pTable;
    BYTE u8TabCols;
    BYTE u8TabRows;
    BYTE u8TabIdx;
} TAB_Info;

typedef struct
{
    BYTE PLL_Ictrl; // ADC_ATOP_09_H [1:0]: PLL Ictrl
    BYTE PLL_Mult_Mod_216m; // ADC_ATOP_0A_L [5]: 216m_en, [4:3]: PLL mod, [2:0]: PLL mult
    BYTE LPF_Rch; // low pass filter setting in Rch
    BYTE LPF_Gch; // low pass filter setting in Gch
    BYTE LPF_Bch; // low pass filter setting in Bch
    WORD Blacklevel_Rch; // Rch black level
    WORD Blacklevel_Gch; // Gch black level
    WORD Blacklevel_Bch; // Bch black level
    WORD Offset_Rch; // Rch offset level
    WORD Offset_Gch; // Gch offset level
    WORD Offset_Bch; // Bch offset level
} msADCBackupSetting;

typedef struct
{
    BYTE DBuffer; // scaler double buffer setting
    BYTE ISEL; // Scaler input select
    WORD SPRHST; // Image horizontal sample start point
    WORD SPRHDC; // Image horizontal resolution
    WORD SPRVST; // Image vertical sample start point
    WORD SPRVDC; // Image vertical resolution
} msScalerBackupSetting;

typedef enum
{
    E_Vcal_0_55V = 0,
    E_Vcal_1_05V = 1,
} msADC_InternalVcal;

typedef enum
{
    E_PG_Htotal_256 = 0,
    E_PG_Htotal_512 = 1,
    E_PG_Htotal_1024 = 2,
    E_PG_Htotal_2048 = 3,
} msADCScalerPG_HtotalSel;

typedef struct
{
    BYTE AVG_Rch;
    BYTE AVG_Gch;
    BYTE AVG_Bch;
} msADCAvgVal;

typedef struct
{
    WORD wOffsetVal_Rch0;
    WORD wOffsetVal_Rch1;
    WORD wPivot_Rch;
    WORD wOffsetVal_Gch0;
    WORD wOffsetVal_Gch1;
    WORD wPivot_Gch;
    WORD wOffsetVal_Bch0;
    WORD wOffsetVal_Bch1;
    WORD wPivot_Bch;
    WORD wGainVal_Rch0;
    WORD wGainVal_Rch1;
    WORD wGainVal_Gch0;
    WORD wGainVal_Gch1;
    WORD wGainVal_Bch0;
    WORD wGainVal_Bch1;
} msADCMismatchCalVal;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

msADCBackupSetting xdata gADCBackupSetting;
msScalerBackupSetting xdata gScalerBackupSetting;
msADCMismatchCalVal xdata gADCMismatchAvgVal;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void drvADC_LoadTable(TAB_Info* pTab_info);
//static void drvADC_Write2BytesMask(WORD u16regadr, WORD u16val, WORD u16mask);
//static BYTE drvADC_ReadByte(WORD u16regadr);
//static BYTE drvADC_WaitAutoStatusReady(WORD regAddr, BYTE regFlag);
static void drvADC_SetMux(ADC_MUX_TYPE ipmux_type);
#ifdef _WORK_ON_PM_

static BYTE drvADC_SearchFreqSetTableIndex(WORD u16OriginalPixClk);
#if CHIP_FAMILY_TYPE != CHIP_FAMILY_MST9U
static void drvADC_BackupADCSetting(void);
static void drvADC_RecoverADCSetting(void);
static void drvADC_SetFreeRunClock(void);
static void drvADC_MismatchLinearityCal(void);
static void drvADC_MismatchOffsetCal(void);
static void drvADC_MismatchGainCal(void);
#endif
static Bool drvADC_AutoAdjustAdcGain(BYTE vsyncTime, DWORD u32SCRegBase);
static BYTE drvADC_CheckPGAGain(BYTE vsyncTime);
static void drvADC_SetPGAGainCode(BYTE ucAdcPgaGain);  //Steven110329, Solve check PGA gain issue.

#if DADCPLLPROTECTION
static void drvADC_IsEnableFreeRun(Bool bflag);
static void drvADC_SetAutoProst(Bool bflag);
#endif // #ifdef DADCPLLPROTECTION

#if (defined(_DGAIN_CAL_WITHOUT_INPUT_) || defined(_DGAIN_CAL_WITH_INPUT_))
static void drvADC_BackupScalerSetting(void);
static void drvADC_RecoverScalerSetting(void);
#ifdef _DGAIN_CAL_WITHOUT_INPUT_
static void drvADC_EnableScalerPG(msADCScalerPG_HtotalSel ht_sel);
static void _msADC_MaskExternalSync(Bool bflag);
#endif
static void drvADC_SetInternalVcal(msADC_InternalVcal vcal);
static Bool drvADC_IsReadLineBuffer(Bool bflag);
static msADCAvgVal drvADC_GetRGBLineBufAverageData(WORD wLineBufAddress, BYTE LineBuf, BYTE AvgCnt);
static Bool drvADC_AutoGainAdjustment(void);
#endif

#define OffsetValue     20 // ADC offset adjustment

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
extern Bool CheckSyncLoss( BYTE SCDetWin );

//**************************************************************************
//  [Function Name]:
//                  msADC_init()
//  [Description]
//                  ADC init
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msADC_init( void )
{
    drvADC_init(FALSE);
#if CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U
    drvADC_Write2BytesMask(REG_ADC_ATOP_32_L, 0x0800, 0x0C00); // [11:10]: CLAMP_RGB_OV[1:0] = 2'b10 disable clamp
    ForceDelay1ms(10);
    drvADC_ADCLinearityOffsetGainMismatchCal();
    #if 0
    {
        WORD u16adcr_lcal_value0;
        WORD u16adcr_lcal_value1;
        WORD u16adcg_lcal_value0;
        WORD u16adcg_lcal_value1;
        WORD u16adcb_lcal_value0;
        WORD u16adcb_lcal_value1;
        WORD u16adcr_ocal_value0;
        WORD u16adcr_ocal_value1;
        WORD u16adcg_ocal_value0;
        WORD u16adcg_ocal_value1;
        WORD u16adcb_ocal_value0;
        WORD u16adcb_ocal_value1;
        WORD u16adcr_gcal_value0;
        WORD u16adcr_gcal_value1;
        WORD u16adcg_gcal_value0;
        WORD u16adcg_gcal_value1;
        WORD u16adcb_gcal_value0;
        WORD u16adcb_gcal_value1;

        // mismatch linearity
        msWriteByteMask(REG_ADC_DTOPB_0A_L, 0x00, 0x07);
        u16adcr_lcal_value0 = msRead2Byte(REG_ADC_DTOPB_45_L);
        msWriteByteMask(REG_ADC_DTOPB_0A_L, 0x01, 0x07);
        u16adcr_lcal_value1 = msRead2Byte(REG_ADC_DTOPB_45_L);
        msWriteByteMask(REG_ADC_DTOPB_0A_H, 0x00, 0x07);
        u16adcg_lcal_value0 = msRead2Byte(REG_ADC_DTOPB_46_L);
        msWriteByteMask(REG_ADC_DTOPB_0A_H, 0x01, 0x07);
        u16adcg_lcal_value1 = msRead2Byte(REG_ADC_DTOPB_46_L);
        msWriteByteMask(REG_ADC_DTOPB_0B_L, 0x00, 0x07);
        u16adcb_lcal_value0 = msRead2Byte(REG_ADC_DTOPB_47_L);
        msWriteByteMask(REG_ADC_DTOPB_0B_L, 0x01, 0x07);
        u16adcb_lcal_value1 = msRead2Byte(REG_ADC_DTOPB_47_L);

        // mismatch offset
        msWriteByteMask(REG_ADC_DTOPB_17_L, 0x03, 0x3F);
        u16adcr_ocal_value0 = msRead2Byte(REG_ADC_DTOPB_48_L);
        msWriteByteMask(REG_ADC_DTOPB_17_L, 0x13, 0x3F);
        u16adcr_ocal_value1 = msRead2Byte(REG_ADC_DTOPB_48_L);
        msWriteByteMask(REG_ADC_DTOPB_1A_L, 0x03, 0x3F);
        u16adcg_ocal_value0 = msRead2Byte(REG_ADC_DTOPB_49_L);
        msWriteByteMask(REG_ADC_DTOPB_1A_L, 0x13, 0x3F);
        u16adcg_ocal_value1 = msRead2Byte(REG_ADC_DTOPB_49_L);
        msWriteByteMask(REG_ADC_DTOPB_1D_L, 0x03, 0x3F);
        u16adcb_ocal_value0 = msRead2Byte(REG_ADC_DTOPB_4A_L);
        msWriteByteMask(REG_ADC_DTOPB_1D_L, 0x13, 0x3F);
        u16adcb_ocal_value1 = msRead2Byte(REG_ADC_DTOPB_4A_L);

        // mismatch gain
        msWriteByteMask(REG_ADC_DTOPB_25_L, 0x03, 0x3F);
        u16adcr_gcal_value0 = msRead2Byte(REG_ADC_DTOPB_4B_L);
        msWriteByteMask(REG_ADC_DTOPB_25_L, 0x13, 0x3F);
        u16adcr_gcal_value1 = msRead2Byte(REG_ADC_DTOPB_4B_L);
        msWriteByteMask(REG_ADC_DTOPB_29_L, 0x03, 0x3F);
        u16adcg_gcal_value0 = msRead2Byte(REG_ADC_DTOPB_4C_L);
        msWriteByteMask(REG_ADC_DTOPB_29_L, 0x13, 0x3F);
        u16adcg_gcal_value1 = msRead2Byte(REG_ADC_DTOPB_4C_L);
        msWriteByteMask(REG_ADC_DTOPB_2D_L, 0x03, 0x3F);
        u16adcb_gcal_value0 = msRead2Byte(REG_ADC_DTOPB_4D_L);
        msWriteByteMask(REG_ADC_DTOPB_2D_L, 0x13, 0x3F);
        u16adcb_gcal_value1 = msRead2Byte(REG_ADC_DTOPB_4D_L);

        ADC_DPRINTF("@@ MisI_R0= 0x%x", u16adcr_lcal_value0);
        ADC_DPRINTF("@@ MisI_R1= 0x%x", u16adcr_lcal_value1);
        ADC_DPRINTF("@@ MisI_G0= 0x%x", u16adcg_lcal_value0);
        ADC_DPRINTF("@@ MisI_G1= 0x%x", u16adcg_lcal_value1);
        ADC_DPRINTF("@@ MisI_B0= 0x%x", u16adcb_lcal_value0);
        ADC_DPRINTF("@@ MisI_B1= 0x%x", u16adcb_lcal_value1);

        ADC_DPRINTF("@@ MisO_R0= 0x%x", u16adcr_ocal_value0);
        ADC_DPRINTF("@@ MisO_R1= 0x%x", u16adcr_ocal_value1);
        ADC_DPRINTF("@@ MisO_G0= 0x%x", u16adcg_ocal_value0);
        ADC_DPRINTF("@@ MisO_G1= 0x%x", u16adcg_ocal_value1);
        ADC_DPRINTF("@@ MisO_B0= 0x%x", u16adcb_ocal_value0);
        ADC_DPRINTF("@@ MisO_B1= 0x%x", u16adcb_ocal_value1);

        ADC_DPRINTF("@@ MisG_R0= 0x%x", u16adcr_gcal_value0);
        ADC_DPRINTF("@@ MisG_R1= 0x%x", u16adcr_gcal_value1);
        ADC_DPRINTF("@@ MisG_G0= 0x%x", u16adcg_gcal_value0);
        ADC_DPRINTF("@@ MisG_G1= 0x%x", u16adcg_gcal_value1);
        ADC_DPRINTF("@@ MisG_B0= 0x%x", u16adcb_gcal_value0);
        ADC_DPRINTF("@@ MisG_B1= 0x%x", u16adcb_gcal_value1);
    }
    #endif
    drvADC_Write2BytesMask(REG_ADC_ATOP_32_L, 0x0000, 0x0C00); // [11:10]: CLAMP_RGB_OV[1:0] = 2'b00 enable clamp
#else
    drvADC_ADCOffsetGainMismatchCal();
#endif
#ifdef _DGAIN_CAL_WITHOUT_INPUT_
    drvADC_ADCAutoGainCalwithoutInput();
#endif
#if ENABLE_ADC_DITHERING
    msWriteByteMask(REG_ADC_DTOPB_00_L, 0x00, 0x03); // [1:0]: enable ADCA interleve H/V dither in the normal display
#else
    msWriteByteMask(REG_ADC_DTOPB_00_L, 0x03, 0x03); // [1:0]: disable ADCA interleve H/V dither in the normal display
#endif
}

//**************************************************************************
//  [Function Name]:
//                  msADC_SetConfiguration()
//  [Description]
//                  ADC set configuration
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void msADC_SetConfiguration(BYTE ucDisplayEn)
{
   // if(ucDisplayEn & COMBO_IP_MASK_VGA)
    {
        drvADC_SetADCSource(ADC_TABLE_SOURCE_RGB);
    }
}
//**************************************************************************
//  [Function Name]:
//                  drvADC_init()
//  [Description]
//                  ADC init
//  [Arguments]:
//                  IsShareGrd: is used shared negative pin with R/G/B
//  [Return]:
//
//**************************************************************************
void drvADC_init(Bool IsShareGrd)
{
    TAB_Info XDATA Tab_info;
    xdata BYTE no_use = 0;
#ifndef MST9U_FPGA
    BYTE u8ADCBandgapTrimVal = 0;
#endif

    Tab_info.pTable = (void*)MST_ADCINIT_TBL;
    Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_INIT_NUMS*REG_ADC_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCINIT_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = 0;

    ADC_DPUTSTR("ADC Tbl:init\n");
    drvADC_LoadTable(&Tab_info);

    // Set share ground bit
    msWriteByteMask( REG_ADC_ATOP_32_H, IsShareGrd ? BIT1 : 0, BIT1 ); // REG_ADC_ATOP_32_L[9]

#ifndef MST9U_FPGA
    u8ADCBandgapTrimVal = msEread_GetADCBandgapTrimValFromEfuse();
    if(u8ADCBandgapTrimVal & BIT7)
    {
        msWriteByteMask( REG_ADC_ATOP_5B_H, (u8ADCBandgapTrimVal & 0x1F)|BIT5, 0x3F); // REG_ADC_ATOP_5B_L[13]: override, [12:8]: reg_ref_trim_sel
        ADC_DPRINTF("$$ ADC Bandgap Trim value = 0x%x\n", u8ADCBandgapTrimVal);
    }
#endif

    drvADC_SetPGAGainCode(USER_PREF_ADC_PGA_GAIN);  //Steven110329, Solve check PGA gain issue.

    drvADC_SetHsyncRefLevel(ADC_HSYNC_LVL_DEF); // REFH=1.87V, REFL=0.88V // 120725 coding, moves here

    if(no_use)
    {
        drvADC_CheckPGAGain(20);
        drvADC_Test_Funct(0 ,0);
    }
}
//**************************************************************************
//  [Function Name]:
//                  drvADC_SetInputMux()
//  [Description]
//                  Setup analog input mux
//  [Arguments]:
//                  port_type: BYTE
//  [Return]:
//
//**************************************************************************
void drvADC_SetInputMux(BYTE port_type )
{
    switch(port_type)
    {
        default:
        case Input_Analog1:
            drvADC_SetMux(ADC_TABLE_MUX_RGB0_Sync);
            break;
    }
}
#endif

//**************************************************************************
//  [Function Name]:
//                  drvADC_SetADCSource()
//  [Description]
//                  Setup ADC source is RGB or YPbPr or SOG
//  [Arguments]:
//                  inputsrc_type: RGB / YPbPr / SOG
//  [Return]:
//
//**************************************************************************
void drvADC_SetADCSource(ADC_SOURCE_TYPE inputsrc_type)
{
    TAB_Info XDATA Tab_info;
    Tab_info.pTable = (void*)MST_ADCSOURCE_TBL;
    Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_SOURCE_NUMS*REG_ADC_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCSOURCE_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = inputsrc_type;

    //ADC_DPRINTF("ADC Tbl:set src %d \n", inputsrc_type);
    drvADC_LoadTable(&Tab_info);
}
//**************************************************************************
//  [Function Name]:
//                  drvADC_init()
//  [Description]
//                  ADC init for PM wakeup - checkagain fucntion
//  [Arguments]:
//                  IsShareGrd: is used shared negative pin with R/G/B
//  [Return]:
//
//**************************************************************************
void drvADC_init(Bool IsShareGrd) //for PM wakeup - checkagain fucntion
{
    TAB_Info XDATA Tab_info;

    Tab_info.pTable = (void*)MST_ADCINIT_TBL;
    Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_INIT_NUMS*REG_ADC_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCINIT_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = 0;

    IsShareGrd = IsShareGrd;

    ADC_DPUTSTR("ADC Tbl:init\n");
    drvADC_LoadTable(&Tab_info);
}
#ifdef _WORK_ON_PM_
//**************************************************************************
//  [Function Name]:
//                  drvADC_SetADCModeSetting()
//  [Description]
//                  Set ADC related settings by mode
//  [Arguments]:
//                  enADCInput: RGB / YPbPr
//                  u16PixelClk: Input pixel clock(MHz)
//  [Return]:
//
//**************************************************************************
void drvADC_SetADCModeSetting(ADC_INPUTSOURCE_TYPE enADCInput, WORD u16PixelClk)
{
    TAB_Info xdata Tab_info;

    if ( enADCInput & ADC_INPUTSOURCE_YPBPR ) // YPbPr
    {
        Tab_info.pTable = (void*)MST_ADCSetModeYUV_TBL;
        Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_SetMode_NUMS*REG_ADC_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCSetModeYUV_TBL)/Tab_info.u8TabCols;

    }
    else if( enADCInput & ADC_INPUTSOURCE_RGB )// RGB
    {
        Tab_info.pTable = (void*)MST_ADCSetModeRGB_TBL;
        Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_SetMode_NUMS*REG_ADC_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCSetModeRGB_TBL)/Tab_info.u8TabCols;
    }

    Tab_info.u8TabIdx = drvADC_SearchFreqSetTableIndex(u16PixelClk);

    ADC_DPRINTF("ADC Tbl: TabIdx %d\n", Tab_info.u8TabIdx);
    drvADC_LoadTable(&Tab_info);
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_AdjustHTotal()
//  [Description]
//                  Adjust PLL divider
//  [Arguments]:
//                  htotal: Htotal -3
//  [Return]:
//
//**************************************************************************
void drvADC_AdjustHTotal(WORD u16HTotal)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_00_L, u16HTotal-3, 0x1FFF);
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_SetPhaseCode()
//  [Description]
//                  Set ADC phase code
//  [Arguments]:
//                  code: code, 0~127
//  [Return]:
//
//**************************************************************************
void drvADC_SetPhaseCode(BYTE u8PhaseCode)
{
    BYTE XDATA u8ADCPllMod = (drvADC_ReadByte(REG_ADC_ATOP_09_H) & BMASK(4:3)) >> 3; // ADC_ATOP_09_L[12:11]: reg_adc_pll_mod
    WORD XDATA u16Mask = BMASK((6+u8ADCPllMod):0) ;
    WORD XDATA u16PhaseCode = u8PhaseCode * (1<<u8ADCPllMod);

    // fix ADC phase not be set while AC/DC On/Off
    drvADC_Write2BytesMask(REG_ADC_ATOP_15_L, (WORD)u16PhaseCode-(1<<u8ADCPllMod), u16Mask);
    drvADC_Write2BytesMask(REG_ADC_ATOP_15_L, (WORD)u16PhaseCode, u16Mask);
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_SetRGBGainCode()
//  [Description]
//                  Set ADC R/G/B gain code
//  [Arguments]:
//                  rcode/gcode/bcode: R/G/B gain value, 2.12, default is 0x1000
//  [Return]:
//
//**************************************************************************
void drvADC_SetRGBGainCode(WORD u16RCode, WORD u16GCode, WORD u16BCode)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_51_L, u16RCode, 0x3FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_56_L, u16GCode, 0x3FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_5B_L, u16BCode, 0x3FFF);
}

void drvADC_SetRedGainCode(WORD u16RCode)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_51_L, u16RCode, 0x3FFF);
}

void drvADC_SetGreenGainCode(WORD u16GCode)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_56_L, u16GCode, 0x3FFF);
}

void drvADC_SetBlueGainCode(WORD u16BCode)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_5B_L, u16BCode, 0x3FFF);
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_SetRGBOffsetCode()
//  [Description]
//                  Set ADC R/G/B offset code
//  [Arguments]:
//                  rcode/gcode/bcode: R/G/B offset value, ��0~4095(S12)
//  [Return]:
//
//**************************************************************************
void drvADC_SetRGBOffsetCode(WORD u16RCode, WORD u16GCode, WORD u16BCode)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_52_L, u16RCode, 0x1FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_57_L, u16GCode, 0x1FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_5C_L, u16BCode, 0x1FFF);
}

void drvADC_SetRedOffsetCode(WORD u16RCode)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_52_L, u16RCode, 0x1FFF);
}

void drvADC_SetGreenOffsetCode(WORD u16GCode)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_57_L, u16GCode, 0x1FFF);
}

void drvADC_SetBlueOffsetCode(WORD u16BCode)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_5C_L, u16BCode, 0x1FFF);
}

#if CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U

//**************************************************************************
//  [Function Name]:
//                  drvADC_ADCLinearityOffsetGainMismatchCal()
//  [Description]
//                  Do ADC self mismatch calibration
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void drvADC_ADCLinearityOffsetGainMismatchCal(void)
{
    TAB_Info XDATA  Tab_info;

    Tab_info.pTable = (void*)MST_ADCAdcCal_TBL;
    Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_AdcCal_NUMS*REG_ADC_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCAdcCal_TBL)/Tab_info.u8TabCols;

#if ((DADCCALBYBANDGAP == 1) && (DADCNEWGAINCALMETHOD == 2))
    Tab_info.u8TabIdx = ADC_TABLE_AdcCal_BG_Fix_UG;
#elif ((DADCCALBYBANDGAP == 0) && (DADCNEWGAINCALMETHOD == 2))
    Tab_info.u8TabIdx = ADC_TABLE_AdcCal_Fix_UG;
#else
    Tab_info.u8TabIdx = ADC_TABLE_AdcCal_SW_UG;
#endif

    //ADC_DPRINTF("ADC Tbl:Mismatch cal %d \n", Tab_info.u8TabIdx);
    drvADC_LoadTable(&Tab_info);
}

#else

//**************************************************************************
//  [Function Name]:
//                  drvADC_ADCOffsetGainMismatchCal()
//  [Description]
//                  Do ADC self mismatch calibration
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void drvADC_ADCOffsetGainMismatchCal(void)
{
    BYTE XDATA loop;
    WORD XDATA wRch0diff, wRch1diff, wGch0diff, wGch1diff, wBch0diff, wBch1diff;

    memset(&gADCMismatchAvgVal, 0, sizeof(gADCMismatchAvgVal));

    drvADC_Write2BytesMask(REG_ADC_ATOP_51_L, 0x0003, 0x0003); // [1:0]: no ADCA interleve H/V dither

    drvADC_SetPGAGainCode(0);  // for ADC linearity calibration, PGA gain must be set to 0
    // Do mismatch linearity calibration
    drvADC_MismatchLinearityCal();
    drvADC_SetPGAGainCode(USER_PREF_ADC_PGA_GAIN);  //for ADC linearity calibration

    for(loop=0; loop< DADCMISMATCHLOOP ; loop++)
    {
        drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0000, 0x003F); // [5:0]:disable mismatch offsset and gain update enable

        // 1. backup ADC related settings
        drvADC_BackupADCSetting();
        // 2. Set free run clock at 720p
        drvADC_SetFreeRunClock();
        // 3. Do mismatch offset calibration
        drvADC_MismatchOffsetCal();
        // 4. Do mismatch gain calibration
        drvADC_MismatchGainCal();
        // 5. recover ADC related settings & turn off free run clock
        drvADC_RecoverADCSetting();

        Delay1ms(10);
    }

    // Disable ADC dither to fix horizontal/Vertical line noise in some specific panels
    //drvADC_Write2BytesMask(REG_ADC_ATOP_51_L, 0x0003, 0x0003); // [1:0]: ADCA interleve H/V dither in the normal display

    gADCMismatchAvgVal.wOffsetVal_Rch0 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wOffsetVal_Rch1 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wPivot_Rch /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wOffsetVal_Gch0 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wOffsetVal_Gch1 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wPivot_Gch /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wOffsetVal_Bch0 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wOffsetVal_Bch1 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wPivot_Bch /= DADCMISMATCHLOOP;
    wRch0diff = gADCMismatchAvgVal.wOffsetVal_Rch0 >= gADCMismatchAvgVal.wOffsetVal_Rch1 ? (gADCMismatchAvgVal.wOffsetVal_Rch1 - gADCMismatchAvgVal.wOffsetVal_Rch0) : 0;
    wRch1diff = gADCMismatchAvgVal.wOffsetVal_Rch1 >= gADCMismatchAvgVal.wOffsetVal_Rch0 ? (gADCMismatchAvgVal.wOffsetVal_Rch0 - gADCMismatchAvgVal.wOffsetVal_Rch1) : 0;
    wGch0diff = gADCMismatchAvgVal.wOffsetVal_Gch0 >= gADCMismatchAvgVal.wOffsetVal_Gch1 ? (gADCMismatchAvgVal.wOffsetVal_Gch1 - gADCMismatchAvgVal.wOffsetVal_Gch0) : 0;
    wGch1diff = gADCMismatchAvgVal.wOffsetVal_Gch1 >= gADCMismatchAvgVal.wOffsetVal_Gch0 ? (gADCMismatchAvgVal.wOffsetVal_Gch0 - gADCMismatchAvgVal.wOffsetVal_Gch1) : 0;
    wBch0diff = gADCMismatchAvgVal.wOffsetVal_Bch0 >= gADCMismatchAvgVal.wOffsetVal_Bch1 ? (gADCMismatchAvgVal.wOffsetVal_Bch1 - gADCMismatchAvgVal.wOffsetVal_Bch0) : 0;
    wBch1diff = gADCMismatchAvgVal.wOffsetVal_Bch1 >= gADCMismatchAvgVal.wOffsetVal_Bch0 ? (gADCMismatchAvgVal.wOffsetVal_Bch0 - gADCMismatchAvgVal.wOffsetVal_Bch1) : 0;

    gADCMismatchAvgVal.wGainVal_Rch0 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wGainVal_Rch1 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wGainVal_Gch0 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wGainVal_Gch1 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wGainVal_Bch0 /= DADCMISMATCHLOOP;
    gADCMismatchAvgVal.wGainVal_Bch1 /= DADCMISMATCHLOOP;

    // Restrict mismatch gain couldn't less than 0.94. otherwise it will make iclamp engine unstable
    // The maxi code is 1023, the middle point is 512 and iclamp code is 32
    // Input signal will be (1)-512 (2) x mismatch gain (3) +512,
    // if input signal less than iclamp code, iclamp engine will be charge and discharge iteratively then makes engine crash
    // so 32/512 = 0.0625, mismatch gain must be large than (1-0.0625), that is 0.94 = 0x3C2
    gADCMismatchAvgVal.wGainVal_Rch0 = (gADCMismatchAvgVal.wGainVal_Rch0 > 0x3C2) ? gADCMismatchAvgVal.wGainVal_Rch0 : 0x3C2;
    gADCMismatchAvgVal.wGainVal_Rch1 = (gADCMismatchAvgVal.wGainVal_Rch1 > 0x3C2) ? gADCMismatchAvgVal.wGainVal_Rch1 : 0x3C2;
    gADCMismatchAvgVal.wGainVal_Gch0 = (gADCMismatchAvgVal.wGainVal_Gch0 > 0x3C2) ? gADCMismatchAvgVal.wGainVal_Gch0 : 0x3C2;
    gADCMismatchAvgVal.wGainVal_Gch1 = (gADCMismatchAvgVal.wGainVal_Gch1 > 0x3C2) ? gADCMismatchAvgVal.wGainVal_Gch1 : 0x3C2;
    gADCMismatchAvgVal.wGainVal_Bch0 = (gADCMismatchAvgVal.wGainVal_Bch0 > 0x3C2) ? gADCMismatchAvgVal.wGainVal_Bch0 : 0x3C2;
    gADCMismatchAvgVal.wGainVal_Bch1 = (gADCMismatchAvgVal.wGainVal_Bch1 > 0x3C2) ? gADCMismatchAvgVal.wGainVal_Bch1 : 0x3C2;

    // Set mismatch offset componsate code
    drvADC_Write2BytesMask(REG_ADC_ATOP_56_L, wRch0diff, 0x07FF); // Set ADC channel 0 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_57_L, wRch1diff, 0x07FF); // Set ADC channel 1 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_75_L, gADCMismatchAvgVal.wPivot_Rch, 0x03FF); // Set ADCR pivot-point value, choose minimum one
    drvADC_Write2BytesMask(REG_ADC_ATOP_5A_L, wGch0diff, 0x07FF); // Set ADC channel 0 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_5B_L, wGch1diff, 0x07FF); // Set ADC channel 1 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_76_L, gADCMismatchAvgVal.wPivot_Gch, 0x03FF); // Set ADCG pivot-point value, choose minimum one
    drvADC_Write2BytesMask(REG_ADC_ATOP_5E_L, wBch0diff, 0x07FF); // Set ADC channel 0 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_5F_L, wBch1diff, 0x07FF); // Set ADC channel 1 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_77_L, gADCMismatchAvgVal.wPivot_Bch, 0x03FF); // Set ADCB pivot-point value, choose minimum one
    // Set mismatch gain componsate code
    drvADC_Write2BytesMask(REG_ADC_ATOP_58_L, gADCMismatchAvgVal.wGainVal_Rch0, 0x07FF); // Set ADC channel 0 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_59_L, gADCMismatchAvgVal.wGainVal_Rch1, 0x07FF); // Set ADC channel 1 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_5C_L, gADCMismatchAvgVal.wGainVal_Gch0, 0x07FF); // Set ADC channel 0 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_5D_L, gADCMismatchAvgVal.wGainVal_Gch1, 0x07FF); // Set ADC channel 1 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_60_L, gADCMismatchAvgVal.wGainVal_Bch0, 0x07FF); // Set ADC channel 0 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_61_L, gADCMismatchAvgVal.wGainVal_Bch1, 0x07FF); // Set ADC channel 1 gain compensated code

#if 0
    ADC_DPUTSTR("\nAverage result:\n");
    ADC_DPRINTF("R Ch0 offset code= 0x%x", wRch0diff);
    ADC_DPRINTF("R Ch1 offset code= 0x%x", wRch1diff);
    ADC_DPRINTF("R Pivot-point= 0x%x", gADCMismatchAvgVal.wPivot_Rch);
    ADC_DPRINTF("G Ch0 offset code= 0x%x", wGch0diff);
    ADC_DPRINTF("G Ch1 offset code= 0x%x", wGch1diff);
    ADC_DPRINTF("G Pivot-point= 0x%x", gADCMismatchAvgVal.wPivot_Gch);
    ADC_DPRINTF("B Ch0 offset code= 0x%x", wBch0diff);
    ADC_DPRINTF("B Ch1 offset code= 0x%x", wBch1diff);
    ADC_DPRINTF("B Pivot-point= 0x%x \n", gADCMismatchAvgVal.wPivot_Bch);

    ADC_DPRINTF("R Ch0 gain code= 0x%x", gADCMismatchAvgVal.wGainVal_Rch0);
    ADC_DPRINTF("R Ch1 gain code= 0x%x", gADCMismatchAvgVal.wGainVal_Rch1);
    ADC_DPRINTF("G Ch0 gain code= 0x%x", gADCMismatchAvgVal.wGainVal_Gch0);
    ADC_DPRINTF("G Ch1 gain code= 0x%x", gADCMismatchAvgVal.wGainVal_Gch1);
    ADC_DPRINTF("B Ch0 gain code= 0x%x", gADCMismatchAvgVal.wGainVal_Bch0);
    ADC_DPRINTF("B Ch1 gain code= 0x%x", gADCMismatchAvgVal.wGainVal_Bch1);
#endif
}

#endif // #if CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U

#if 0 // wait for re-manege
//**************************************************************************
//  [Function Name]:
//                  drvADC_AutoPhaseAdjustment()
//  [Description]
//                  Do ADC good phase adjustment
//  [Arguments]:
//                  vsyncTime: frame rate of the current input
//  [Return]:
//                  TRUE: done, FALSE: fail
//**************************************************************************
Bool drvADC_AutoPhaseAdjustment(BYTE u8VSyncTime)
{
    BYTE u8AdjPhase, i;

    ADC_DPUTSTR("drvADC_AutoPhaseAdjustment\n");
#if (DADCWORSTPHASE)
{
    BYTE worsePhase, worsePhase2;
    DWORD mincksum, cksum;

    mincksum=0xFFFFFFFFul;
    for (u8AdjPhase=0; u8AdjPhase<0x80; u8AdjPhase+=7)
    {
        drvADC_SetPhaseCode(u8AdjPhase);
        Delay1ms(u8VSyncTime);
        drvADC_WaitAutoStatusReady(SC0_8B, BIT1);
        cksum=_msRead2Byte(SC0_8E);
        cksum=(cksum<<16)|_msRead2Byte(SC0_8C);
        if (cksum<mincksum)
        {
            mincksum=cksum;
            worsePhase=u8AdjPhase;
        }
        if (CheckSyncLoss())//||CheckPowerKeyStatus())
            return FALSE;
    }
    //  printData("coast phase %d", bestPhase);
    // 2nd Search
    if (worsePhase<7)
        worsePhase=(worsePhase+127)%0x80; // 128-7
    else
        worsePhase-=7;
    mincksum=0xFFFFFFFFul;
    for (u8AdjPhase=worsePhase; u8AdjPhase<worsePhase+10; u8AdjPhase++)
    {
        i=u8AdjPhase%0x80;
        drvADC_SetPhaseCode(i);
        Delay1ms(u8VSyncTime);
        drvADC_WaitAutoStatusReady(SC0_8B, BIT1);
        cksum=_msRead2Byte(SC0_8E);
        cksum=(cksum<<16)|_msRead2Byte(SC0_8C);
        if (cksum<mincksum)
        {
            mincksum=cksum;
            worsePhase2=i;
        }
        if (CheckSyncLoss())//||CheckPowerKeyStatus())
            return FALSE;
    }
    u8AdjPhase=(worsePhase2+63)%0x80;
    #define FINAL_PHASE u8AdjPhase
}
#else
{
    BYTE bestPhase, bestPhase2;
    DWORD maxcksum, cksum;

    maxcksum=0;
    for (u8AdjPhase=0; u8AdjPhase<0x80; u8AdjPhase+=7)
    {
        drvADC_SetPhaseCode(u8AdjPhase);
        ForceDelay1ms(u8VSyncTime);
        drvADC_WaitAutoStatusReady(SC0_8B, BIT1);
        cksum=_msRead2Byte(SC0_8E);
        cksum=(cksum<<16)|_msRead2Byte(SC0_8C);
        if (cksum>maxcksum)
        {
            maxcksum=cksum;
            bestPhase=u8AdjPhase;
        }
        if (CheckSyncLoss())//||CheckPowerKeyStatus())
            return FALSE;
    }
    //  printData("coast phase %d", bestPhase);
    // 2nd Search
    if (bestPhase<7)
        bestPhase=(bestPhase+121)%0x80; // 128-7
    else
        bestPhase-=7;
    maxcksum=0;
    for (u8AdjPhase=bestPhase; u8AdjPhase<bestPhase+10; u8AdjPhase++)
    {
        i=u8AdjPhase%0x80;
        drvADC_SetPhaseCode(i);
        ForceDelay1ms(u8VSyncTime);
        drvADC_WaitAutoStatusReady(SC0_8B, BIT1);
        cksum=_msRead2Byte(SC0_8E);
        cksum=(cksum<<16)|_msRead2Byte(SC0_8C);
        if (cksum>maxcksum)
        {
            maxcksum=cksum;
            bestPhase2=i;
        }
        if (CheckSyncLoss())//||CheckPowerKeyStatus())
            return FALSE;
    }
    u8AdjPhase=bestPhase2;
    #define FINAL_PHASE u8AdjPhase
}
#endif // DADCWORSTPHASE

    drvADC_SetPhaseCode(FINAL_PHASE);
    ADC_DPRINTF("final phase %d", FINAL_PHASE);
    //UPPHASE_SET_BY_REAL_VALUE(FINAL_PHASE);
    #undef FINAL_PHASE
    return TRUE;
}
#endif
//**************************************************************************
//  [Function Name]:
//                  drvADC_AutoAdcColor()
//  [Description]
//                  Do the ADC auto color adjustment
//  [Arguments]:
//                  vsyncTime: frame rate of the current input
//  [Return]:
//                  TRUE: done, FALSE: fail
//**************************************************************************
Bool drvADC_AutoAdcColor(BYTE u8VSyncTime, BYTE InputPort)
{
    BOOL xdata result;
    BYTE xdata u8AdcPgaGain;
    DWORD xdata u32SCRegBase = SCRegBase(MapPort2DetScIndex(InputPort));

    if (!INPUT_IS_VGA(InputPort))//(MapScaler2Port!=Input_Analog1)
        return FALSE;

    ADC_DPUTSTR("drvADC_AutoAdcColor\n");
    //drvADC_SetPGAGainCode(0);
    //if(USER_PREF_INPUT_COLOR_FORMAT==INPUT_COLOR_YPBPR) //YPbPr or YUV_H+V
    //    drvADC_SetRGBOffsetCode(0x200, 0x40, 0x200);
    //else
        drvADC_SetRGBOffsetCode(0, 0, 0);
    drvADC_SetRGBGainCode(0x1084, 0x1084, 0x1084);

    msWriteByte(u32SCRegBase+SC01_1C, 0x01); // enable auto Gain
    //u8AdcPgaGain=drvADC_CheckPGAGain(u8VSyncTime);
    u8AdcPgaGain = 0x03;

    result = drvADC_AutoAdjustAdcGain(u8VSyncTime, u32SCRegBase);

    if(result)
    {
        //ADC_DPUTSTR("$$ Auto color pass! \n");
        USER_PREF_ADC_PGA_GAIN = u8AdcPgaGain;
        //USER_PREF_ADC_RED_OFFSET = (0x800-OffsetValue);
        //USER_PREF_ADC_GREEN_OFFSET = 0x800-OffsetValue;
        //USER_PREF_ADC_BLUE_OFFSET = 0x800-OffsetValue;
        //drvADC_SetRGBOffsetCode(USER_PREF_ADC_RED_OFFSET, USER_PREF_ADC_GREEN_OFFSET, USER_PREF_ADC_BLUE_OFFSET);
    }
    else
    {
        drvADC_SetPGAGainCode( USER_PREF_ADC_PGA_GAIN );
        drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN, USER_PREF_ADC_GREEN_GAIN, USER_PREF_ADC_BLUE_GAIN);
        //drvADC_SetRGBOffsetCode(USER_PREF_ADC_RED_OFFSET, USER_PREF_ADC_GREEN_OFFSET, USER_PREF_ADC_BLUE_OFFSET);
    }

/*
    if (!result)
    {
        ADC_DPUTSTR( "Auto color failed !!");
        drvADC_SetPGAGainCode(USER_PREF_ADC_PGA_GAIN);
        drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN, USER_PREF_ADC_GREEN_GAIN, USER_PREF_ADC_BLUE_GAIN);
    }
    else
    {
        USER_PREF_ADC_PGA_GAIN=u8AdcPgaGain;
    }
*/

    msWriteByte(u32SCRegBase+SC01_1C, 0); // disable auto Gain

    #if 0
    ADC_DPRINTF("adc gain red 0x%x", USER_PREF_ADC_RED_GAIN);
    ADC_DPRINTF("adc gain green 0x%x", USER_PREF_ADC_GREEN_GAIN);
    ADC_DPRINTF("adc gain blue 0x%x", USER_PREF_ADC_BLUE_GAIN);
    ADC_DPRINTF("adc gain PGA 0x%x", USER_PREF_ADC_PGA_GAIN);
    #endif

    return result;
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_SetupHsyncPolarity()
//  [Description]
//                  Setup Hsync polarity
//  [Arguments]:
//                  bActiveHigh: TRUE -> active high, FALSE: active low
//  [Return]:
//
//**************************************************************************
void drvADC_SetupHsyncPolarity(Bool IsActiveHigh)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_07_L,(IsActiveHigh?BIT7:0),(BIT7));
}
//**************************************************************************
//  [Function Name]:
//                  drvADC_AdjustCalDuring()
//  [Description]
//                  Adjust ADC calibration during for specific reduced blanking timings
//  [Arguments]:
//                  u8CalDur: pixel
//  [Return]:
//
//**************************************************************************
void drvADC_AdjustCalDuring(BYTE u8CalDur)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_18_L, u8CalDur, 0x00FF);
}
//**************************************************************************
//  [Function Name]:
//                  drvADC_SetupADCLPF()
//  [Description]
//                  Setup ADC LPF
//  [Arguments]:
//                  val: 3dB bandwidth
//                  - VGA(RGB(YUV)_H+V): Pixel Clock/1.2 < 3dB BW
//                  - SOG(Sync on G or Y): Pixel Clock/2 < 3dB BW
//  [Return]:
//
//**************************************************************************
#if 0
void drvADC_SetupADCLPF(ADCLPFBW_LIST val)
{
    // Bch: ATOP_23[3:0]: +, ATOP_23[7:4]: -
    // Gch: ATOP_23[15:12]: +, ATOP_23[11:8]: -
    // Rch: ATOP_24[3:0]: +, ATOP_24[7:4]: -
    drvADC_Write2BytesMask(REG_ADC_ATOP_23_L, val<<12|val<<8|val<<4|val, 0xFFFF);
    drvADC_Write2BytesMask(REG_ADC_ATOP_24_L, val<<4|val, 0x00FF);
}
#endif
#if DADCPLLPROTECTION
//**************************************************************************
//  [Function Name]:
//                  drvADC_SetModewithPLLProtection()
//  [Description]
//                  Mode changed with applying ADC PLL protection mechnism
//  [Arguments]:
//                  enADCInput: RGB / YPbPr
//                  u16PixelClk: Input pixel clock(MHz)
//                  htotal: Htotal -3
//  [Return]:
//                  TRUE: done, FALSE: fail
//**************************************************************************
Bool drvADC_SetModewithPLLProtection(ADC_INPUTSOURCE_TYPE enADCInput, WORD u16PixelClk, WORD u16HTotal)
{
    Bool result;
    BYTE regval1, regval2;

    //ADC_DPUTSTR("drvADC_SetModewithPLLProtection\n");
    drvADC_IsEnableFreeRun(TRUE);
    drvADC_SetAutoProst(TRUE);
    drvADC_SetADCModeSetting(enADCInput, u16PixelClk);
    drvADC_SetAutoProst(FALSE);
    Delay1ms(1);
    drvADC_AdjustHTotal(u16HTotal);
    drvADC_IsEnableFreeRun(FALSE);
    Delay1ms(10); // PLL lock status would depend on input frequency, low frequency would take longer time to lock
    regval1 = drvADC_ReadByte(REG_ADC_ATOP_0F_H); // ADC_ATOP_0F[12]
    regval2 = drvADC_ReadByte(REG_ADC_DTOP_05_L);
    if(!(regval1 & 0x10) && (regval2 & 0x80))
    {
        result = TRUE;
    }
    else // 1st trial failed, wait 10msec to try 2nd round
    {
        //ADC_DPUTSTR("drvADC_SetModewithPLLProtection 1st failed\n");
        //ADC_DPRINTF("@@HV_flag = 0x%x", regval1);
        //ADC_DPRINTF("@@PLL_flag = 0x%x", regval2);
        Delay1ms(10);
        drvADC_IsEnableFreeRun(TRUE);
        drvADC_SetAutoProst(TRUE);
        drvADC_SetADCModeSetting(enADCInput, u16PixelClk);
        drvADC_SetAutoProst(FALSE);
        Delay1ms(1);
        drvADC_AdjustHTotal(u16HTotal);
        drvADC_IsEnableFreeRun(FALSE);
        Delay1ms(10); // PLL lock status would depend on input frequency, low frequency would take longer time to lock
        regval1 = drvADC_ReadByte(REG_ADC_ATOP_0F_H);
        regval2 = drvADC_ReadByte(REG_ADC_DTOP_05_L);
        if(!(regval1 & 0x10) && (regval2 & 0x80))
        {
            result = TRUE;
        }
        else
            result = FALSE;
    }
#if ENABLE_ADC_RESET
    // ADC software reset for ���ťի�issue
    drvADC_Write2BytesMask(REG_ADC_ATOP_07_L, BIT6, BIT6); // [6]: Iclamp reset
    drvADC_Write2BytesMask(REG_ADC_ATOP_07_L, 0, BIT6);
    Delay1ms(2);
#endif
    // trigger ADC phase dac alignment to avoid phase shift after DC off/on
    drvADC_Write2BytesMask(REG_ADC_ATOP_19_L, BIT3, BIT3);
    drvADC_Write2BytesMask(REG_ADC_ATOP_19_L, 0, BIT3);
    ADC_DPRINTF("drvADC_SetModewithPLLProtection result = %d\n", result);
    return result;
}
#endif //#ifdef DADCPLLPROTECTION

#ifdef _DGAIN_CAL_WITHOUT_INPUT_
//**************************************************************************
//  [Function Name]:
//                  drvADC_ADCAutoGainCalwithoutInput()
//  [Description]
//                  Do ADC gain calibration without input
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void drvADC_ADCAutoGainCalwithoutInput(void)
{
#if DADCNEWGAINCALMETHOD == 1

#if CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U
    USER_PREF_ADC_RED_GAIN  = 0x1084;
    USER_PREF_ADC_GREEN_GAIN = 0x1084;
    USER_PREF_ADC_BLUE_GAIN = 0x1084;
#else
    USER_PREF_ADC_RED_GAIN  = 0x421;
    USER_PREF_ADC_GREEN_GAIN = 0x421;
    USER_PREF_ADC_BLUE_GAIN = 0x421;
#endif
    drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN, USER_PREF_ADC_GREEN_GAIN, USER_PREF_ADC_BLUE_GAIN);

#elif DADCNEWGAINCALMETHOD == 2

#if CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U

    // Digital target gain: 1024/(1024-32)*PGA gain/0.7
    // Russell ADC PGA gain:
    //  00: 1.0 / 01: 1.2 / 10: 0.8 / 11: 0.96
    switch(USER_PREF_ADC_PGA_GAIN)
    {
        case 0x00: // 1.474654
            USER_PREF_ADC_RED_GAIN = 0x1798;
            USER_PREF_ADC_GREEN_GAIN = 0x1798;
            USER_PREF_ADC_BLUE_GAIN = 0x1798;
            break;
        case 0x01: // 1.769584
            USER_PREF_ADC_RED_GAIN = 0x1C50;
            USER_PREF_ADC_GREEN_GAIN = 0x1C50;
            USER_PREF_ADC_BLUE_GAIN = 0x1C50;
            break;
        case 0x02: // 1.179723
            USER_PREF_ADC_RED_GAIN = 0x12E0;
            USER_PREF_ADC_GREEN_GAIN = 0x12E0;
            USER_PREF_ADC_BLUE_GAIN = 0x12E0;
            break;
        case 0x03: // 1.415667
            USER_PREF_ADC_RED_GAIN = 0x16A6;
            USER_PREF_ADC_GREEN_GAIN = 0x16A6;
            USER_PREF_ADC_BLUE_GAIN = 0x16A6;
            break;
        default:
            break;
    }

#else

    // Digital target gain: 1024/(1024-32)*PGA gain/0.7
    // Russell ADC PGA gain:
    //  00: 0.861538 / 01: 0.969231 / 10: 1.076923 / 11: 1.184615
    switch(USER_PREF_ADC_PGA_GAIN)
    {
        case 0x00:
            USER_PREF_ADC_RED_GAIN = 0x514;
            USER_PREF_ADC_GREEN_GAIN = 0x514;
            USER_PREF_ADC_BLUE_GAIN = 0x514;
            break;
        case 0x15:
            USER_PREF_ADC_RED_GAIN = 0x5B7;
            USER_PREF_ADC_GREEN_GAIN = 0x5B7;
            USER_PREF_ADC_BLUE_GAIN = 0x5B7;
            break;
        case 0x2A:
            USER_PREF_ADC_RED_GAIN = 0x65A;
            USER_PREF_ADC_GREEN_GAIN = 0x65A;
            USER_PREF_ADC_BLUE_GAIN = 0x65A;
            break;
        case 0x3F:
            USER_PREF_ADC_RED_GAIN = 0x6FC;
            USER_PREF_ADC_GREEN_GAIN = 0x6FC;
            USER_PREF_ADC_BLUE_GAIN = 0x6FC;
            break;
        default:
            break;
    }
#endif // #if CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U
    drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN, USER_PREF_ADC_GREEN_GAIN, USER_PREF_ADC_BLUE_GAIN);

#else

    // 1. backup ADC and Scaler related settings
    drvADC_BackupADCSetting();
    drvADC_BackupScalerSetting();

    // 2. Set free run clock at 720p
    _msADC_MaskExternalSync(TRUE);
    drvADC_SetFreeRunClock();

    // 3. Enable scaler pattern generator to generate Hsync and Vsync for line buffer
    drvADC_EnableScalerPG(E_PG_Htotal_256);

    // 4. Set ADC input LPF to lowest bandwidth
    drvADC_Write2BytesMask(REG_ADC_ATOP_23_L, 0xFFFF, 0xFFFF);
    drvADC_Write2BytesMask(REG_ADC_ATOP_24_L, 0x00FF, 0x00FF);

    // 5. Calibrate ADC R/G/B gain setting value
    // Set R/G/B gain initial setting value at 0x0400
    // 6. Gain calibration target value for RGB color space mode is 182 (8bit data) 255 * 0.5 / 0.7 = 182
    // 7. Get the ADC digital output with Turn-On LDO and Set VCAL to 1.05v
    // 8. Get the ADC digital output with VCAL to 0.55v
    // 9. Calculate the difference between 1.05V and 0.55V of these 3 channels.
    //    Compare the difference value with target value (182 at RGB color space) to adjust R/G/B channel gain register
    drvADC_AutoGainAdjustment();

    // 10. recover ADC and scaler related settings & turn off free run clock & turn off internal Vcal
    _msADC_MaskExternalSync(FALSE);
    drvADC_RecoverADCSetting();
    drvADC_RecoverScalerSetting();

#endif // #if DADCNEWGAINCALMETHOD
}
#endif

#ifdef _DGAIN_CAL_WITH_INPUT_
//**************************************************************************
//  [Function Name]:
//                  drvADC_ADCAutoGainCalwithInput()
//  [Description]
//                  Do ADC gain calibration with input
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void drvADC_ADCAutoGainCalwithInput(void)
{
    // 1. backup ADC and Scaler related settings
    drvADC_BackupADCSetting();
    drvADC_BackupScalerSetting();

    // 2. Set free run clock at 720p
    //drvADC_SetFreeRunClock();

    // 3. Enable scaler pattern generator to generate Hsync and Vsync for line buffer
    //drvADC_EnableScalerPG(E_PG_Htotal_256);

    // 4. Set ADC input LPF to lowest bandwidth
    drvADC_Write2BytesMask(REG_ADC_ATOP_23_L, 0xFFFF, 0xFFFF);
    drvADC_Write2BytesMask(REG_ADC_ATOP_24_L, 0x00FF, 0x00FF);

    // 5. Calibrate ADC R/G/B gain setting value
    // Set R/G/B gain initial setting value at 0x0400
    // 6. Gain calibration target value for RGB color space mode is 182 (8bit data) 255 * 0.5 / 0.7 = 182
    // 7. Get the ADC digital output with Turn-On LDO and Set VCAL to 1.05v
    // 8. Get the ADC digital output with VCAL to 0.55v
    // 9. Calculate the difference between 1.05V and 0.55V of these 3 channels.
    //    Compare the difference value with target value (182 at RGB color space) to adjust R/G/B channel gain register
    drvADC_AutoGainAdjustment();

    // 10. recover ADC and scaler related settings & turn off free run clock & turn off internal Vcal
    drvADC_RecoverADCSetting();
    drvADC_RecoverScalerSetting();
}

#endif

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

#if 0
//**************************************************************************
//  [Function Name]:
//                  drvADC_Write2BytesMask()
//  [Description]:
//                  Load ADC driver table setting
//  [Precondition]:
//
//  [Arguments]:
//                 u16regadr : 16 bits register address
//                 u16val : 16 bits register value
//                 u16mask : 16 bits register mask
//  [Return]:
//
//**************************************************************************
static void drvADC_Write2BytesMask(WORD u16regadr, WORD u16val, WORD u16mask)
{
#if 1//CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMU
    _msWrite2ByteMask( u16regadr, u16val, u16mask );
#else

    BYTE ucBank, ucCurBank, ucCurReg;

    ucBank = mStar_ReadByte(REGBK); // backup bank

    ucCurBank = (BYTE)(u16regadr >> 8);
    ucCurReg = (BYTE)(u16regadr & 0xFF);

    if(ucCurBank != ucBank)
        mStar_WriteByte(REGBK, ucCurBank);

    if(u16mask & 0x00FF)
        mStar_WriteByteMask(ucCurReg, (BYTE)(u16val&0xFF), (BYTE)(u16mask&0xFF));
    if(u16mask & 0xFF00)
        mStar_WriteByteMask(ucCurReg+1, (BYTE)(u16val>>8), (BYTE)(u16mask>>8));

    mStar_WriteByte(REGBK, ucBank);        // Switch to Original bank
#endif
}
#endif
#if 0
//**************************************************************************
//  [Function Name]:
//                  drvADC_ReadByte()
//  [Description]:
//                  Read ADC register value
//  [Precondition]:
//
//  [Arguments]:
//                 u16regadr : 16 bits register address
//  [Return]:
//                 register value(1 byte)
//**************************************************************************
static BYTE drvADC_ReadByte(WORD u16regadr)
{
#if 1//CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMU
    return _msReadByte( u16regadr );
#else

    BYTE ucBank, ucCurBank, ucCurReg, retval;

    ucBank = mStar_ReadByte(REGBK); // backup bank

    ucCurBank = (BYTE)(u16regadr >> 8);
    ucCurReg = (BYTE)(u16regadr & 0xFF);

    if(ucCurBank != ucBank)
        mStar_WriteByte(REGBK, ucCurBank);
    retval = mStar_ReadByte(ucCurReg);

    mStar_WriteByte(REGBK, ucBank);        // Switch to Original bank

    return retval;
#endif
}
#endif
//**************************************************************************
//  [Function Name]:
//                  drvADC_WaitAutoStatusReady()
//  [Description]:
//                  Wait ready bit is ready
//  [Precondition]:
//
//  [Arguments]:
//                 regAddr: 16 bit register address
//                 regFlag: Ready bit
//  [Return]:
//
//**************************************************************************
BYTE drvADC_WaitAutoStatusReady(DWORD regAddr, BYTE regFlag)
{
#if 0
    WORD waits=5000; // MCU 1x: 1500, 2x:3000, 3x: 4500
    while (waits-- && !(mStar_ReadByte(regAddr)&regFlag));
#else
    BYTE u8Value=0;
    TimeOutCounter=50;// 50 msec, at least two frames
    while( TimeOutCounter && !( (u8Value=msReadByte( regAddr ))&regFlag ) );
#endif
    //ADC_DPRINTF("drvADC_WaitAutoStatusReady= %d\n", TimeOutCounter);
    return u8Value;
}
#endif
//**************************************************************************
//  [Function Name]:
//                  Hal_ADC_LoadTable()
//  [Description]:
//                  Load ADC driver table setting
//  [Precondition]:
//
//  [Arguments]:
//                 pTab_info
//  [Return]:
//
//**************************************************************************
static void drvADC_LoadTable(TAB_Info* pTab_info)
{
    BYTE XDATA i;
    DWORD XDATA u32Addr;
    BYTE XDATA u8Mask, u8Value, u8DoNotSet;

    if (pTab_info->pTable == NULL || pTab_info->u8TabRows == 0 || pTab_info->u8TabRows == 1)
        return;

    if (REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+pTab_info->u8TabIdx*REG_ADC_DATA_SIZE >= pTab_info->u8TabCols)
    {
        ADC_DPUTSTR("Tab_info error\n");
        return;
    }

    for (i=0; i< (BYTE)(pTab_info->u8TabRows-1); i++)
    {
         u32Addr =  (DWORD)( ((DWORD)pTab_info->pTable[0]<<16) + ((DWORD)pTab_info->pTable[1]<<8) + pTab_info->pTable[2] );

         if(u32Addr == REG_ADC_DTOPB_FE_L)
         {
            // delay only, skip to write next register
            u8Value = pTab_info->pTable[REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+pTab_info->u8TabIdx*REG_ADC_DATA_SIZE + 1];
            //ADC_DPRINTF("ADC tbl delay (%d) ms \n",u8Value);
            ForceDelay1ms(u8Value);
            goto NEXT;
         }

         u8Mask  = pTab_info->pTable[3];
         u8DoNotSet = pTab_info->pTable[REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+pTab_info->u8TabIdx*REG_ADC_DATA_SIZE];
         u8Value = pTab_info->pTable[REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+pTab_info->u8TabIdx*REG_ADC_DATA_SIZE + 1];

         //printf("[addr=%06lx, msk=%02x, enb=%02x val=%02x]\n", u32Addr, u8Mask,u8DoNotSet, u8Value);

         if ( !u8DoNotSet )
         {
            //ADC_DPUTSTR("@@ register value Mask = ");
            //ADC_DPRINTF("0x%x", u32Addr>>16);
            //ADC_DPRINTF("0x%x", u32Addr);
            //ADC_DPRINTF("0x%x", u8Value);
            //ADC_DPRINTF("0x%x", u8Mask);
            msWriteByteMask( u32Addr, u8Value, u8Mask );
         }
NEXT:
         pTab_info->pTable+=pTab_info->u8TabCols; // next
    }
}
#ifdef _WORK_ON_PM_
//**************************************************************************
//  [Function Name]:
//                  drvADC_SetMux()
//  [Description]
//                  ADC set sync and data mux
//  [Arguments]:
//                  ipmux_type: sync / data
//  [Return]:
//
//**************************************************************************
static void drvADC_SetMux(ADC_MUX_TYPE ipmux_type)
{
    TAB_Info XDATA Tab_info;
    Tab_info.pTable = (void*)MST_ADCMUX_TBL;
    Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_MUX_NUMS*REG_ADC_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCMUX_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = ipmux_type;

    ADC_DPRINTF("ADC Tbl:set mux %d \n",ipmux_type);
    drvADC_LoadTable(&Tab_info);

}

//**************************************************************************
//  [Function Name]:
//                  drvADC_SearchFreqSetTableIndex()
//  [Description]:
//                  Search the Frequncy table index
//  [Precondition]:
//
//  [Arguments]:
//                 u16OriginalPixClk: xxxMHz
//  [Return]:
//
//**************************************************************************
static BYTE drvADC_SearchFreqSetTableIndex(WORD u16OriginalPixClk)
{
    BYTE  u8ClkIndex;

    for(u8ClkIndex=0; u8ClkIndex<sizeof(MST_ADC_FreqRange_TBL)/sizeof(ADC_FREQ_RANGE); u8ClkIndex++)
    {
        if((u16OriginalPixClk < MST_ADC_FreqRange_TBL[u8ClkIndex].FreqHLimit) &&
            (u16OriginalPixClk >= MST_ADC_FreqRange_TBL[u8ClkIndex].FreqLLimit))
            break;
    }

    return u8ClkIndex;
}

#if CHIP_FAMILY_TYPE != CHIP_FAMILY_MST9U

//**************************************************************************
//  [Function Name]:
//                  drvADC_BackupADCSetting()
//  [Description]:
//                  Backup ADC settings
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void drvADC_BackupADCSetting(void)
{
    memset(&gADCBackupSetting, 0, sizeof(gADCBackupSetting));

    gADCBackupSetting.PLL_Ictrl = drvADC_ReadByte(REG_ADC_ATOP_09_H) & 0x03;
    gADCBackupSetting.PLL_Mult_Mod_216m = drvADC_ReadByte(REG_ADC_ATOP_0A_L) & 0x3F;
    gADCBackupSetting.LPF_Rch = drvADC_ReadByte(REG_ADC_ATOP_24_L) & 0x0F;
    gADCBackupSetting.LPF_Gch = drvADC_ReadByte(REG_ADC_ATOP_23_H) & 0x0F;
    gADCBackupSetting.LPF_Bch = drvADC_ReadByte(REG_ADC_ATOP_23_L) & 0x0F;
    gADCBackupSetting.Blacklevel_Rch = (drvADC_ReadByte(REG_ADC_DTOP_44_H) << 8 | drvADC_ReadByte(REG_ADC_DTOP_44_L)) & 0x03FF;
    gADCBackupSetting.Blacklevel_Gch = (drvADC_ReadByte(REG_ADC_DTOP_47_H) << 8 | drvADC_ReadByte(REG_ADC_DTOP_47_L)) & 0x03FF;
    gADCBackupSetting.Blacklevel_Bch = (drvADC_ReadByte(REG_ADC_DTOP_50_H) << 8 | drvADC_ReadByte(REG_ADC_DTOP_50_L)) & 0x03FF;
    gADCBackupSetting.Offset_Rch = (drvADC_ReadByte(REG_ADC_DTOP_46_H) << 8 | drvADC_ReadByte(REG_ADC_DTOP_46_L)) & 0x07FF;
    gADCBackupSetting.Offset_Gch = (drvADC_ReadByte(REG_ADC_DTOP_49_H) << 8 | drvADC_ReadByte(REG_ADC_DTOP_49_L)) & 0x07FF;
    gADCBackupSetting.Offset_Bch = (drvADC_ReadByte(REG_ADC_DTOP_52_H) << 8 | drvADC_ReadByte(REG_ADC_DTOP_52_L)) & 0x07FF;
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_RecoverADCSetting()
//  [Description]:
//                  Recover ADC settings
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void drvADC_RecoverADCSetting(void)
{
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0x0000, 0xFFFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_06_L, 0x0000, 0x0080); // PLL Frequency Force Disable
    drvADC_Write2BytesMask(REG_ADC_DTOP_02_L, 0x0982, 0xFFFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_03_L, 0x0005, 0xFFFF);
    drvADC_Write2BytesMask(REG_ADC_ATOP_0A_L, gADCBackupSetting.PLL_Mult_Mod_216m, 0x003F);
    drvADC_Write2BytesMask(REG_ADC_ATOP_09_L, gADCBackupSetting.PLL_Ictrl << 8, 0x0300);
    drvADC_Write2BytesMask(REG_ADC_ATOP_23_L, (gADCBackupSetting.LPF_Gch << 8)|gADCBackupSetting.LPF_Bch , 0x0F0F);
    drvADC_Write2BytesMask(REG_ADC_ATOP_24_L, gADCBackupSetting.LPF_Rch, 0x000F);
    drvADC_Write2BytesMask(REG_ADC_DTOP_44_L, gADCBackupSetting.Blacklevel_Rch, 0x03FF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_47_L, gADCBackupSetting.Blacklevel_Gch, 0x03FF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_50_L, gADCBackupSetting.Blacklevel_Bch, 0x03FF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_46_L, gADCBackupSetting.Offset_Rch, 0x07FF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_49_L, gADCBackupSetting.Offset_Gch, 0x07FF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_52_L, gADCBackupSetting.Offset_Bch, 0x07FF);
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_SetFreeRunClock()
//  [Description]:
//                  Set ADC free-run clock to 40MHz
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void drvADC_SetFreeRunClock(void)
{
    //drvADC_Write2BytesMask(REG_ADC_ATOP_01_L, 0x4001, 0xFFFF); // ADC enable
    //drvADC_Write2BytesMask(REG_ADC_ATOP_04_L, 0, 0xFFFF); // Turn on analog power down
    //drvADC_Write2BytesMask(REG_ADC_ATOP_06_L, 0, 0xFFFF); // Turn on gated
#if 0
    drvADC_Write2BytesMask(REG_ADC_ATOP_0A_L, 0x0008, 0x003F); // PLL_MULT = 0, DIV2_EN = 1'b1
    drvADC_Write2BytesMask(REG_ADC_ATOP_09_L, 0x0300, 0x0300); // ADCPLL_ICTRL = 2'b11
    drvADC_Write2BytesMask(REG_ADC_DTOP_02_L, 0xB82E, 0xFFFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_03_L, 0x0052, 0xFFFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_06_L, 0x0080, 0x0080); // PLL Frequency Force Enable
#else // revise the correct free-run 40MHz setting
    drvADC_Write2BytesMask( REG_ADC_ATOP_0A_L, 0x0010, 0x003F ); // [5]:DIV2_EN = 1'b0, [4:3]:APLL_MOD=2'b10, [2:0]: APLL_MULT = 0
    drvADC_Write2BytesMask( REG_ADC_ATOP_09_L, 0x0200, 0x0700 ); // ADCPLL_ICTRL = 2'b010
    drvADC_Write2BytesMask( REG_ADC_DTOP_02_L, 0x6656, 0xFFFF );
    drvADC_Write2BytesMask( REG_ADC_DTOP_03_L, 0x0066, 0xFFFF );
    drvADC_Write2BytesMask( REG_ADC_DTOP_06_L, 0x0080, 0x0080 ); // PLL Frequency Force Enable
#endif
}


//**************************************************************************
//  [Function Name]:
//                  drvADC_MismatchOffsetCal()
//  [Description]:
//                  ADC mismatch offset calibration
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void drvADC_MismatchLinearityCal(void)
{
    TimeOutCounter = 10; // 10 msec
    // Set Vcal
    //drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0xF800, 0xF800); // [15]:ADC VCAL force enable, [14:11]: set Vcal
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0xF600, 0xFF00); // [15]:ADC VCAL force enable, [14:11]: set Vcal, [10:8]: 3'b 110, reg_rgb_self_gcal_pulse_sw

    do
    {
        // Enable linearity calibration
        drvADC_Write2BytesMask(REG_ADC_DTOP_66_L, 0, 0x0070); // [6:4]: Enable linearity calibration for R/G/B
        drvADC_Write2BytesMask(REG_ADC_DTOP_66_L, 0x0070, 0x0070); // [6:4]: Enable linearity calibration for R/G/B
        // Trigger
        drvADC_Write2BytesMask(REG_ADC_DTOP_68_L, 0x0000, 0x0700); // [10:8]: Rising trigger for R/G/B (Auto/Manual mode)
        drvADC_Write2BytesMask(REG_ADC_DTOP_68_L, 0x0700, 0x0700); // [10:8]: Rising trigger for R/G/B (Auto/Manual mode)
        drvADC_Write2BytesMask(REG_ADC_DTOP_68_L, 0x0000, 0x0700); // [10:8]: Rising trigger for R/G/B (Auto/Manual mode)
        // wait done
        ForceDelay1ms(1);
    } while( TimeOutCounter && ((drvADC_ReadByte(REG_ADC_DTOP_69_H) & 0x70) != 0x70) ); // [14:12]: linearity calibration done
    // Set Vcal
    //drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0, 0xF800); // [15]:ADC VCAL force enable, [14:11]: set Vcal
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0, 0xFF00); // [15]:ADC VCAL force enable, [14:11]: set Vcal, [10:8]: 3'b 110, reg_rgb_self_gcal_pulse_sw

#if 0
    if((drvADC_ReadByte(REG_ADC_DTOP_69_H) & 0x70) == 0x70)
    {
        ADC_DPRINTF("@@ L_T= %d\n", TimeOutCounter);
        ADC_DPRINTF("@@ ADC R_ACCU_CH0 = 0x%x\n", _msRead2Byte(REG_ADC_DTOP_6A_L));
        ADC_DPRINTF("@@ ADC R_ACCU_CH1 = 0x%x\n", _msRead2Byte(REG_ADC_DTOP_6B_L));
        ADC_DPRINTF("@@ ADC G_ACCU_CH0 = 0x%x\n", _msRead2Byte(REG_ADC_DTOP_6C_L));
        ADC_DPRINTF("@@ ADC G_ACCU_CH1 = 0x%x\n", _msRead2Byte(REG_ADC_DTOP_6D_L));
        ADC_DPRINTF("@@ ADC B_ACCU_CH0 = 0x%x\n", _msRead2Byte(REG_ADC_DTOP_6E_L));
        ADC_DPRINTF("@@ ADC B_ACCU_CH1 = 0x%x\n", _msRead2Byte(REG_ADC_DTOP_6F_L));
    }
#endif
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_MismatchOffsetCal()
//  [Description]:
//                  ADC mismatch offset calibration
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void drvADC_MismatchOffsetCal(void)
{
    WORD XDATA wTimeOutCnt = 300;
    WORD XDATA wRch0val, wRch1val, wGch0val, wGch1val, wBch0val, wBch1val;
    WORD XDATA wRch0diff, wRch1diff, wGch0diff, wGch1diff, wBch0diff, wBch1diff;
    WORD XDATA wRchPivot, wGchPivot, wBchPivot;

    // 1. Turn on ADC offset calibration pluse
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0x0030, 0x0030);

    // 2. Enable ADC data low Vcal accumulator
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0080, 0x0080);

    ForceDelay1ms(1); // delay 1 msec

    // 3. Trigger ADC data low Vcal accumulator start
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0100, 0x0100);

    // 4. Wait ADC data accumulation done, ADC_ATO_74_L[2:0] = 3'b 111
    while ((wTimeOutCnt>0) && (!(drvADC_ReadByte(REG_ADC_ATOP_74_L) & 0x07)))
    {
        wTimeOutCnt--;
    }
    if(wTimeOutCnt == 0)
    {
        ADC_DPUTSTR("ADC data accumulation time out\n");
    }

    // 5. Read ADC channel 0 & 1 average value
    wRch0val = ((drvADC_ReadByte(REG_ADC_ATOP_62_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_62_L);
    wRch1val = ((drvADC_ReadByte(REG_ADC_ATOP_65_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_65_L);
    wGch0val = ((drvADC_ReadByte(REG_ADC_ATOP_68_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_68_L);
    wGch1val = ((drvADC_ReadByte(REG_ADC_ATOP_6B_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_6B_L);
    wBch0val = ((drvADC_ReadByte(REG_ADC_ATOP_6E_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_6E_L);
    wBch1val = ((drvADC_ReadByte(REG_ADC_ATOP_71_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_71_L);

#if 0
    // Aaron test
    wRch0val = 2000;
    wRch1val = 1889;
    wGch0val = 1776;
    wGch1val = 1888;
    wBch0val = 2;
    wBch1val = 400;
#endif

    // 6. Set ADC compensated codes and Pivot-point value
    // R-ch
    if(wRch0val > wRch1val)
    {
        wRch0diff = (WORD)(wRch1val-wRch0val); // negative number, 2's complement
        wRch1diff = 0x0000;
        wRchPivot = wRch1val; // minimum one
    }
    else if (wRch0val < wRch1val)
    {
        wRch0diff = 0x0000;
        wRch1diff = (WORD)(wRch0val-wRch1val); // negative number, 2's complement
        wRchPivot = wRch0val; // minimum one
    }
    else // wRch0val = wRch1val
    {
        wRch0diff = 0x0000;
        wRch1diff = 0x0000;
        wRchPivot = wRch0val;
    }
    // G-ch
    if(wGch0val > wGch1val)
    {
        wGch0diff = (WORD)(wGch1val-wGch0val); // negative number, 2's complement
        wGch1diff = 0x0000;
        wGchPivot = wGch1val; // minimum one
    }
    else if (wGch0val < wGch1val)
    {
        wGch0diff = 0x0000;
        wGch1diff = (WORD)(wGch0val-wGch1val); // negative number, 2's complement
        wGchPivot = wGch0val; // minimum one
    }
    else // wGch0val = wGch1val
    {
        wGch0diff = 0x0000;
        wGch1diff = 0x0000;
        wGchPivot = wGch0val;
    }
    // B-ch
    if(wBch0val > wBch1val)
    {
        wBch0diff = (WORD)(wBch1val-wBch0val); // negative number, 2's complement
        wBch1diff = 0x0000;
        wBchPivot = wBch1val; // minimum one
    }
    else if (wBch0val < wBch1val)
    {
        wBch0diff = 0x0000;
        wBch1diff = (WORD)(wBch0val-wBch1val); // negative number, 2's complement
        wBchPivot = wBch0val; // minimum one
    }
    else // wBch0val = wBch1val
    {
        wBch0diff = 0x0000;
        wBch1diff = 0x0000;
        wBchPivot = wBch0val;
    }

    gADCMismatchAvgVal.wOffsetVal_Rch0  += wRch0val;
    gADCMismatchAvgVal.wOffsetVal_Rch1  += wRch1val;
    gADCMismatchAvgVal.wPivot_Rch       += wRchPivot;
    gADCMismatchAvgVal.wOffsetVal_Gch0  += wGch0val;
    gADCMismatchAvgVal.wOffsetVal_Gch1  += wGch1val;
    gADCMismatchAvgVal.wPivot_Gch       += wGchPivot;
    gADCMismatchAvgVal.wOffsetVal_Bch0  += wBch0val;
    gADCMismatchAvgVal.wOffsetVal_Bch1  += wBch1val;
    gADCMismatchAvgVal.wPivot_Bch       += wBchPivot;
#if 0
    drvADC_Write2BytesMask(REG_ADC_ATOP_56_L, wRch0diff, 0x07FF); // Set ADC channel 0 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_57_L, wRch1diff, 0x07FF); // Set ADC channel 1 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_75_L, wRchPivot, 0x03FF); // Set ADCR pivot-point value, choose minimum one
    drvADC_Write2BytesMask(REG_ADC_ATOP_5A_L, wGch0diff, 0x07FF); // Set ADC channel 0 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_5B_L, wGch1diff, 0x07FF); // Set ADC channel 1 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_76_L, wGchPivot, 0x03FF); // Set ADCG pivot-point value, choose minimum one
    drvADC_Write2BytesMask(REG_ADC_ATOP_5E_L, wBch0diff, 0x07FF); // Set ADC channel 0 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_5F_L, wBch1diff, 0x07FF); // Set ADC channel 1 offset compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_77_L, wBchPivot, 0x03FF); // Set ADCB pivot-point value, choose minimum one
#endif
#if 0
    ADC_DPRINTF("\nR Ch0 code= 0x%x", wRch0diff);
    ADC_DPRINTF("R Ch1 code= 0x%x", wRch1diff);
    ADC_DPRINTF("R Pivot-point= 0x%x", wRchPivot);
    ADC_DPRINTF("G Ch0 code= 0x%x", wGch0diff);
    ADC_DPRINTF("G Ch1 code= 0x%x", wGch1diff);
    ADC_DPRINTF("G Pivot-point= 0x%x", wGchPivot);
    ADC_DPRINTF("B Ch0 code= 0x%x", wBch0diff);
    ADC_DPRINTF("B Ch1 code= 0x%x", wBch1diff);
    ADC_DPRINTF("B Pivot-point= 0x%x \n", wBchPivot);
#endif
    // 7. Turn on enable offset mismatch compensated codes
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0015, 0x0095); // [7]: acc low disable, [4]: B mismatch offset enable, [2]: G mismatch offset enable, [0]: R mismatch offset enable,

    // 8. Turn off ADC offset calibration pulse
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0x0000, 0x0030);
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_MismatchGainCal()
//  [Description]:
//                  ADC mismatch gain calibration
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void drvADC_MismatchGainCal(void)
{
    WORD XDATA wTimeOutCnt = 300;
    WORD XDATA wRch0val, wRch1val, wGch0val, wGch1val, wBch0val, wBch1val;
    WORD XDATA wRch0Div, wRch1Div, wGch0Div, wGch1Div, wBch0Div, wBch1Div;
#if DADCNEWGAINCALMETHOD
    WORD XDATA wTargetCode;
#endif

    // 1. Turn on LDO & select VCAL 1.05V, Turn on ADC gain calibration pulse
#if DADCCALBYBANDGAP
    drvADC_Write2BytesMask(REG_ADC_ATOP_34_L, 0, BIT0); // turn off LDO
    drvADC_Write2BytesMask(REG_ADC_ATOP_34_L, 0, BIT2); // Bandgap
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0, 0x000F); // disable LDO overwrite
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0xF800, 0xF800); // [15]:ADC VCAL force enable, [14:11]: set Vcal = 1.05V,
#else
    drvADC_Write2BytesMask(REG_ADC_ATOP_34_L, BIT0, BIT0); // turn on LDO
    drvADC_Write2BytesMask(REG_ADC_ATOP_34_L, BIT2, BIT2); // LDO
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0x0003, 0x0003); // set Vcal = 1.05V
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0x000C, 0x000C); // set Vcal from LDO
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0, 0xF800); // disable bandgap overwrite
#endif // DADCCALBYBANDGAP
    // Turn on self Gcal pulse
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0x0600, 0x0700); // [10:8]: 3'b 110, reg_rgb_self_gcal_pulse_sw

    // 2. Enable ADC data high Vcal accumulator
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0040, 0x0040);

    ForceDelay1ms(1); // delay 1 msec

    // 3. Trigger ADC data high Vcal accumulator start
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0100, 0x0100);

    // 4. Wait ADC data accumulation done, ADC_ATO_74_L[2:0] = 3'b 111
    while ((wTimeOutCnt>0) && (!(drvADC_ReadByte(REG_ADC_ATOP_74_L) & 0x07)))
    {
        wTimeOutCnt--;
    }
    if(wTimeOutCnt == 0)
    {
        ADC_DPUTSTR("ADC data high accumulation time out\n");
    }

    // 5. Select Vcal 0.55V
#if DADCCALBYBANDGAP
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0xD800, 0xF800); // [15]:ADC VCAL force enable, [14:11]: set Vcal = 0.55V,
#else
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0x0002, 0x0003); // reg_rgb_cal_ldo_sel_sw
#endif // DADCCALBYBANDGAP

    // 6. Disable ADC data high Vcal accumulator & Enable ADC data low Vcal accumulator
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0000, 0x0040);
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0080, 0x0080);

    ForceDelay1ms(1); // delay 1 msec

    // 7. Trigger ADC data low Vcal accumulator start
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x0100, 0x0100);

    // 8. Wait ADC data accumulation done, ADC_ATO_74_L[2:0] = 3'b 111
    wTimeOutCnt = 300;
    while ((wTimeOutCnt>0) && (!(drvADC_ReadByte(REG_ADC_ATOP_74_L) & 0x07)))
    {
        wTimeOutCnt--;
    }
    if(wTimeOutCnt == 0)
    {
        ADC_DPUTSTR("ADC data low accumulation time out\n");
    }

    // 9. Read ADC channel 0 & 1 difference value(Vcal_high - Vcal_low)
    wRch0val = ((drvADC_ReadByte(REG_ADC_ATOP_64_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_64_L);
    wRch1val = ((drvADC_ReadByte(REG_ADC_ATOP_67_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_67_L);
    wGch0val = ((drvADC_ReadByte(REG_ADC_ATOP_6A_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_6A_L);
    wGch1val = ((drvADC_ReadByte(REG_ADC_ATOP_6D_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_6D_L);
    wBch0val = ((drvADC_ReadByte(REG_ADC_ATOP_70_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_70_L);
    wBch1val = ((drvADC_ReadByte(REG_ADC_ATOP_73_H)&0x03) << 8) | drvADC_ReadByte(REG_ADC_ATOP_73_L);

#if 0
    // Aaron test
    wRch0val = 3;
    wRch1val = 5;
    wGch0val = 2010;
    wGch1val = 1888;
    wBch0val = 1000;
    wBch1val = 1000;
#endif

    // 10. Set ADC compensated codes (1.10)
#if DADCNEWGAINCALMETHOD == 1

    // target code : 1024 * 0.5 / 0.7 = 731
    //wTargetCode = 731;
    // target code : (1024-32) * 0.5 / 0.7 = 708
    wTargetCode = 708;

    //ADC_DPRINTF("Target Code = %d\n", wTargetCode);
    // R-ch
    wRch0Div = ((wTargetCode / wRch0val) << 10) | (WORD)(((DWORD)(wTargetCode%wRch0val)<<10)/wRch0val);
    wRch1Div = ((wTargetCode / wRch1val) << 10) | (WORD)(((DWORD)(wTargetCode%wRch1val)<<10)/wRch1val);
    // G-ch
    wGch0Div = ((wTargetCode / wGch0val) << 10) | (WORD)(((DWORD)(wTargetCode%wGch0val)<<10)/wGch0val);
    wGch1Div = ((wTargetCode / wGch1val) << 10) | (WORD)(((DWORD)(wTargetCode%wGch1val)<<10)/wGch1val);
    // B-ch
    wBch0Div = ((wTargetCode / wBch0val) << 10) | (WORD)(((DWORD)(wTargetCode%wBch0val)<<10)/wBch0val);
    wBch1Div = ((wTargetCode / wBch1val) << 10) | (WORD)(((DWORD)(wTargetCode%wBch1val)<<10)/wBch1val);

#elif DADCNEWGAINCALMETHOD == 2

    // Russell ADC PGA gain:
    //  00: 0.861538 / 01: 0.969231 / 10: 1.076923 / 11: 1.184615
    //  target code : (1024-32)*0.5/PGA gain
    switch(USER_PREF_ADC_PGA_GAIN)
    {
        case 0x00:
            wTargetCode = 576;
            break;
        case 0x15:
            wTargetCode = 512;
            break;
        case 0x2A:
            wTargetCode = 461;
            break;
        case 0x3F:
            wTargetCode = 419;
            break;
        default:
            break;
    }

    //ADC_DPRINTF("Target Code = %d\n", wTargetCode);
    // R-ch
    wRch0Div = ((wTargetCode / wRch0val) << 10) | (WORD)(((DWORD)(wTargetCode%wRch0val)<<10)/wRch0val);
    wRch1Div = ((wTargetCode / wRch1val) << 10) | (WORD)(((DWORD)(wTargetCode%wRch1val)<<10)/wRch1val);
    // G-ch
    wGch0Div = ((wTargetCode / wGch0val) << 10) | (WORD)(((DWORD)(wTargetCode%wGch0val)<<10)/wGch0val);
    wGch1Div = ((wTargetCode / wGch1val) << 10) | (WORD)(((DWORD)(wTargetCode%wGch1val)<<10)/wGch1val);
    // B-ch
    wBch0Div = ((wTargetCode / wBch0val) << 10) | (WORD)(((DWORD)(wTargetCode%wBch0val)<<10)/wBch0val);
    wBch1Div = ((wTargetCode / wBch1val) << 10) | (WORD)(((DWORD)(wTargetCode%wBch1val)<<10)/wBch1val);

#else

    // R-ch
    if(wRch0val > wRch1val)
    {
        if((wRch0val / wRch1val) > 1)
        {
            wRch1Div = 0x0400; // ratio is 1, 1<<10
            //ADC_DPRINTF("drvADC_MismatchGainCal: ADC Rch1 overflow = %f\n", (float)wRch0val/wRch1val);
        }
        else
        {
            wRch1Div = ((wRch0val / wRch1val) << 10) | (WORD)(((DWORD)(wRch0val%wRch1val)<<10)/wRch1val);
        }
        wRch0Div = 0x0400; // ratio is 1, 1<<10
    }
    else if (wRch0val < wRch1val)
    {
        if((wRch1val / wRch0val) > 1)
        {
            wRch0Div = 0x0400; // ratio is 1, 1<<10
            //ADC_DPRINTF("drvADC_MismatchGainCal: ADC Rch0 overflow = %f\n", (float)wRch1val/wRch0val);
        }
        else
        {
            wRch0Div = ((wRch1val / wRch0val) << 10) | (WORD)(((DWORD)(wRch1val%wRch0val)<<10)/wRch0val);
        }
        wRch1Div = 0x0400;
    }
    else // wRch0val = wRch1val
    {
        wRch0Div = 0x0400;
        wRch1Div = 0x0400;
        //ADC_DPUTSTR("ADC R Ch 0 and ch1's gain Componsated code are equal\n");
    }
    // G-ch
    if(wGch0val > wGch1val)
    {
        if((wGch0val / wGch1val) > 1)
        {
            wGch1Div = 0x0400; // ratio is 1, 1<<10
            //ADC_DPRINTF("drvADC_MismatchGainCal: ADC Gch1 overflow = %f\n", (float)wGch0val/wGch1val);
        }
        else
        {
            wGch1Div = ((wGch0val / wGch1val) << 10) | (WORD)(((DWORD)(wGch0val%wGch1val)<<10)/wGch1val);
        }
        wGch0Div = 0x0400;
    }
    else if (wGch0val < wGch1val)
    {
        if((wGch1val / wGch0val) > 1)
        {
            wGch0Div = 0x0400; // ratio is 1, 1<<10
            //ADC_DPRINTF("drvADC_MismatchGainCal: ADC Gch0 overflow = %f\n", (float)wGch1val/wGch0val);
        }
        else
        {
            wGch0Div = ((wGch1val / wGch0val) << 10) | (WORD)(((DWORD)(wGch1val%wGch0val)<<10)/wGch0val);
        }
        wGch1Div = 0x0400;
    }
    else // wGch0val = wGch1val
    {
        wGch0Div = 0x0400;
        wGch1Div = 0x0400;
    }
    // B-ch
    if(wBch0val > wBch1val)
    {
        if((wBch0val / wBch1val) > 1)
        {
            wBch1Div = 0x0400; // ratio is 1, 1<<10
            //ADC_DPRINTF("drvADC_MismatchGainCal: ADC Bch1 overflow = %f\n", (float)wBch0val/wBch1val);
        }
        else
        {
            wBch1Div = ((wBch0val / wBch1val) << 10) | (WORD)(((DWORD)(wBch0val%wBch1val)<<10)/wBch1val);
        }
        wBch0Div = 0x0400;
    }
    else if (wBch0val < wBch1val)
    {
        if((wBch1val / wBch0val) > 1)
        {
            wBch0Div = 0x0400; // ratio is 1, 1<<10
            //ADC_DPRINTF("drvADC_MismatchGainCal: ADC Bch0 overflow = %f\n", (float)wBch1val/wBch0val);
        }
        else
        {
            wBch0Div = ((wBch1val / wBch0val) << 10) | (WORD)(((DWORD)(wBch1val%wBch0val)<<10)/wBch0val);
        }
        wBch1Div = 0x0400;
    }
    else // wBch0val = wBch1val
    {
        wBch0Div = 0x0400;
        wBch1Div = 0x0400;
    }

#endif // #if DADCNEWGAINCALMETHOD

    gADCMismatchAvgVal.wGainVal_Rch0 += (wRch0Div & 0x07FF);
    gADCMismatchAvgVal.wGainVal_Rch1 += (wRch1Div & 0x07FF);
    gADCMismatchAvgVal.wGainVal_Gch0 += (wGch0Div & 0x07FF);
    gADCMismatchAvgVal.wGainVal_Gch1 += (wGch1Div & 0x07FF);
    gADCMismatchAvgVal.wGainVal_Bch0 += (wBch0Div & 0x07FF);
    gADCMismatchAvgVal.wGainVal_Bch1 += (wBch1Div & 0x07FF);
#if 0
    drvADC_Write2BytesMask(REG_ADC_ATOP_58_L, wRch0Div, 0x07FF); // Set ADC channel 0 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_59_L, wRch1Div, 0x07FF); // Set ADC channel 1 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_5C_L, wGch0Div, 0x07FF); // Set ADC channel 0 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_5D_L, wGch1Div, 0x07FF); // Set ADC channel 1 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_60_L, wBch0Div, 0x07FF); // Set ADC channel 0 gain compensated code
    drvADC_Write2BytesMask(REG_ADC_ATOP_61_L, wBch1Div, 0x07FF); // Set ADC channel 1 gain compensated code
#endif
#if 0
    ADC_DPRINTF("R Ch0 gain val= 0x%x", wRch0val);
    //ADC_DPRINTF("R Ch1 gain val= 0x%x", wRch1val);
    ADC_DPRINTF("G Ch0 gain val= 0x%x", wGch0val);
    //ADC_DPRINTF("G Ch1 gain val= 0x%x", wGch1val);
    ADC_DPRINTF("B Ch0 gain val= 0x%x", wBch0val);
    //ADC_DPRINTF("B Ch1 gain val= 0x%x", wBch1val);
    ADC_DPRINTF("R Ch0 gain code= 0x%x", wRch0Div);
    ADC_DPRINTF("R Ch1 gain code= 0x%x", wRch1Div);
    ADC_DPRINTF("G Ch0 gain code= 0x%x", wGch0Div);
    ADC_DPRINTF("G Ch1 gain code= 0x%x", wGch1Div);
    ADC_DPRINTF("B Ch0 gain code= 0x%x", wBch0Div);
    ADC_DPRINTF("B Ch1 gain code= 0x%x", wBch1Div);
#endif
    // 11. Turn on enable gain mismatch compensated codes
    drvADC_Write2BytesMask(REG_ADC_ATOP_55_L, 0x002A, 0x00AA); // [7]: acc low disable, [5]: B mismatch gain enable, [3]: G mismatch gain enable, [1]: R mismatch gain enable,

    // 12. Turn off ADC offset calibration pulse
    drvADC_Write2BytesMask(REG_ADC_ATOP_54_L, 0x0000, 0xFF0F); // [15]:ADC VCAL force enable, [14:11]: set Vcal, [10:8]: Turn off self Gcal pulse, [3:2]: reg_rgb_cal_trim_ldo_sel_sw, [1:0]: reg_rgb_cal_ldo_sel_sw
}

#endif // #if CHIP_FAMILY_TYPE != CHIP_FAMILY_MST9U

#define DGain_OverFlow_R_ch         BIT7
#define DGain_NoneOverFlow_R_ch     BIT6
#define DGain_OverFlow_G_ch         BIT5
#define DGain_NoneOverFlow_G_ch     BIT4
#define DGain_OverFlow_B_ch         BIT3
#define DGain_NoneOverFlow_B_ch     BIT2
#ifndef ADCRB_SWAP
#define ADCRB_SWAP 0
#endif
#ifndef ADJ_GAIN_RATIO
#define ADJ_GAIN_RATIO      1
#endif

//**************************************************************************
//  [Function Name]:
//                  drvADC_AutoAdjustAdcGain()
//  [Description]:
//                  Check input sync is lost or not
//  [Precondition]:
//                  Do auto gain adjustment with input source
//  [Arguments]:
//                  vsyncTime: frame rate of the current input
//  [Return]:
//                  TRUE: done, FALSE: fail
//**************************************************************************
static Bool drvADC_AutoAdjustAdcGain(BYTE u8VSyncTime, DWORD u32SCRegBase)
{
    WORD XDATA rGain, gGain, bGain;
    WORD XDATA maxRGain, maxGGain, maxBGain;
    WORD XDATA minRGain, minGGain, minBGain;
    BYTE XDATA retry=12;
    BYTE XDATA adcStatus;
    BYTE XDATA atgFlags=0;

    maxRGain=maxGGain=maxBGain=0x4000;
    minRGain=minGGain=minBGain=0;

    while (retry--)
    {
        rGain=(maxRGain+minRGain)/2;
        gGain=(maxGGain+minGGain)/2;
        bGain=(maxBGain+minBGain)/2;
        drvADC_SetRGBGainCode(rGain, gGain, bGain);
        Delay1ms(u8VSyncTime);
        // Wait scaler auto gain is done, SC0_78[1] = 1
        adcStatus=drvADC_WaitAutoStatusReady(u32SCRegBase+SC01_1C, BIT1);
        //adcStatus=msReadByte(SC01_1C);

        if( adcStatus & BIT7 )
        {
            rGain--;
            atgFlags|=DGain_OverFlow_R_ch;
        }
        else
        {
            rGain++;
            atgFlags|=DGain_NoneOverFlow_R_ch;
        }

        if( adcStatus & BIT6 )
        {
            gGain--;
            atgFlags|=DGain_OverFlow_G_ch;
        }
        else
        {
            gGain++;
            atgFlags|=DGain_NoneOverFlow_G_ch;
        }

        if( adcStatus & BIT5 )
        {
            bGain--;
            atgFlags|=DGain_OverFlow_B_ch;
        }
        else
        {
            bGain++;
            atgFlags|=DGain_NoneOverFlow_B_ch;
        }

        drvADC_SetRGBGainCode(rGain, gGain, bGain);
        Delay1ms(u8VSyncTime);
        // Wait scaler auto gain is done, SC0_78[1] = 1
        adcStatus=drvADC_WaitAutoStatusReady(u32SCRegBase+SC01_1C, BIT1);
        //adcStatus=msReadByte(SC01_1C);

        if( adcStatus & BIT7 )
        {
            maxRGain=rGain+1;
            atgFlags|=DGain_OverFlow_R_ch;
        }
        else
        {
            minRGain=rGain-1;
            atgFlags|=DGain_NoneOverFlow_R_ch;
        }

        if( adcStatus & BIT6 )
        {
            maxGGain=gGain+1;
            atgFlags|=DGain_OverFlow_G_ch;
        }
        else
        {
            minGGain=gGain-1;
            atgFlags|=DGain_NoneOverFlow_G_ch;
        }

        if( adcStatus & BIT5 )
        {
            maxBGain=bGain+1;
            atgFlags|=DGain_OverFlow_B_ch;
        }
        else
        {
            minBGain=bGain-1;
            atgFlags|=DGain_NoneOverFlow_B_ch;
        }

        if (CheckSyncLoss(INPUT_VGA))
            return FALSE;
    }

    if ((atgFlags&0xFC)!=0xFC)
        return FALSE;

    if (rGain>0x3000 || gGain>0x3000 || bGain>0x3000)
        return FALSE;

#if 0  //Steven101220
    USER_PREF_ADC_RED_GAIN = ((DWORD)rGain-1)*(100+ADJ_GAIN_RATIO)/100;
    USER_PREF_ADC_GREEN_GAIN = ((DWORD)gGain-1)*(100+ADJ_GAIN_RATIO)/100;
    USER_PREF_ADC_BLUE_GAIN = ((DWORD)bGain-1)*(100+ADJ_GAIN_RATIO)/100;
#else
    USER_PREF_ADC_RED_GAIN  = rGain-1;
    USER_PREF_ADC_GREEN_GAIN = gGain-1;
    USER_PREF_ADC_BLUE_GAIN = bGain-1;
#endif
    drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN, USER_PREF_ADC_GREEN_GAIN, USER_PREF_ADC_BLUE_GAIN);

    return TRUE;
}

static void drvADC_SetPGAGainCode(BYTE ucAdcPgaGain)
{
    drvADC_Write2BytesMask(REG_ADC_DTOP_65_L, (ucAdcPgaGain<<8)|(ucAdcPgaGain<<4)|ucAdcPgaGain , 0x7FF);
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_CheckPGAGain()
//  [Description]:
//                  Check input signal is over PGA gain or not
//  [Precondition]:
//                  00: 1.0V, 01: 1.2V, 10: 0.8V, 11: 0.96V
//                  The default PGA gain in Rose is 0.8V. To avoid the input signal is over 0.8V, this function is used to
//                  detect and adjust PGA gain. The maximum PGA gain would be limited in 1.0V
//  [Arguments]:
//                  vsyncTime: frame rate of the current input
//  [Return]:
//                  TRUE: done, FALSE: fail
//**************************************************************************
static BYTE drvADC_CheckPGAGain(BYTE vsyncTime)
{
    BYTE adcStatus;
    BYTE u8CurPGA = 2;

    drvADC_Write2BytesMask(REG_ADC_DTOP_65_L, (u8CurPGA<<8)|(u8CurPGA<<4)|u8CurPGA , 0xFFF);
    msWriteByteMask(SC01_1C, BIT3, BIT3); // output is overflow or underflow result
    Delay1ms(vsyncTime);
    // Wait scaler auto gain is done, BK0_78[1] = 1
    adcStatus=drvADC_WaitAutoStatusReady( SC01_1C, BIT1 );
    ADC_DPRINTF("adcStatus = 0x%x\n", adcStatus);
    ADC_DPRINTF("--adcStatus = 0x%x\n", msReadByte(REG_ADC_DTOP_65_L) & 0xFFF);

    while( adcStatus & ( BIT5 | BIT6 | BIT7 ) )
    {
        if(u8CurPGA == 0x00) // limited the maxi VPP = 1.0V
            break;
        else
        {
            u8CurPGA = (u8CurPGA + 1)%4;
            drvADC_Write2BytesMask(REG_ADC_DTOP_65_L, (u8CurPGA<<8)|(u8CurPGA<<4)|u8CurPGA , 0xFFF);
            Delay1ms(vsyncTime);
            // Wait scaler auto gain is done, SC0_78[1] = 1
            adcStatus=drvADC_WaitAutoStatusReady( SC01_1C, BIT1 );
            //adcStatus=_msReadByte(SC0_78);
        }
        ADC_DPRINTF("adcStatus = 0x%x\n", adcStatus);
    }
    ADC_DPRINTF("PGA Gain = 0x%x\n", u8CurPGA);
    msWriteByteMask(SC01_1C, 0, BIT3);

    return u8CurPGA;
}
#if DADCPLLPROTECTION
//**************************************************************************
//  [Function Name]:
//                  drvADC_IsEnableFreeRun()
//  [Description]:
//                  Enable/Disable ADC free run mode
//  [Precondition]:
//                  Use to protect ADC PLL while mode changing
//  [Arguments]:
//                  bflag: TRUE => enable, FALSE => disable
//  [Return]:
//
//**************************************************************************
static void drvADC_IsEnableFreeRun(Bool bflag)
{
    TAB_Info XDATA Tab_info;

    Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_FreeRunEn_NUMS*REG_ADC_DATA_SIZE;
    Tab_info.u8TabIdx = 0;
    if(bflag) // enable
    {
        Tab_info.pTable = (void*)MST_ADCFreeRunEn_TBL;
        Tab_info.u8TabRows = sizeof(MST_ADCFreeRunEn_TBL)/Tab_info.u8TabCols;
    }
    else // disable
    {
        Tab_info.pTable = (void*)MST_ADCFreeRunDis_TBL;
        Tab_info.u8TabRows = sizeof(MST_ADCFreeRunDis_TBL)/Tab_info.u8TabCols;
    }
    drvADC_LoadTable(&Tab_info);

    //ADC_DPRINTF("ADC Tbl:drvADC_IsEnableFreeRun %d\n", bflag);
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_SetAutoProst()
//  [Description]:
//                  Set/Release small loop auto porst
//  [Precondition]:
//                  Use to protect ADC PLL while mode changing
//  [Arguments]:
//                  bflag: TRUE => enable, FALSE => disable
//  [Return]:
//
//**************************************************************************
static void drvADC_SetAutoProst(Bool bflag)
{
    TAB_Info XDATA Tab_info;

    Tab_info.u8TabCols = REG_ADC_ADDR_SIZE+REG_ADC_MASK_SIZE+ADC_TABLE_PorstEn_NUMS*REG_ADC_DATA_SIZE;
    Tab_info.u8TabIdx = 0;
    if(bflag) // enable
    {
        Tab_info.pTable = (void*)MST_ADCPorstEn_TBL;
        Tab_info.u8TabRows = sizeof(MST_ADCPorstEn_TBL)/Tab_info.u8TabCols;
    }
    else // disable
    {
        Tab_info.pTable = (void*)MST_ADCPorstDis_TBL;
        Tab_info.u8TabRows = sizeof(MST_ADCPorstDis_TBL)/Tab_info.u8TabCols;
    }

    drvADC_LoadTable(&Tab_info);

    //ADC_DPRINTF("ADC Tbl:drvADC_SetAutoProst %d\n", bflag);
}
#endif // #ifdef DADCPLLPROTECTION


#ifdef _DGAIN_CAL_WITHOUT_INPUT_ //????
//**************************************************************************
//  [Function Name]:
//                  drvADC_EnableScalerPG()
//  [Description]:
//                  Enable Scaler pattern generatot to generate Htotal = 256/512/1024/2048 and Vtotal=64(fixed)
//  [Precondition]:
//
//  [Arguments]:
//
//                  ht_sel: 0:256, 1:512, 2:1024, 3:2048
//  [Return]:
//
//**************************************************************************
static void drvADC_EnableScalerPG(msADCScalerPG_HtotalSel ht_sel)
{
    return;

    WORD whact;

    //_msWriteByteMask(SC0_02, 0x00, 0x80); // disable free-run
    msWriteByteMask(SC00_02, 0, 0x01); // disable double buffer
    msWriteByteMask(SC01_04, 0x00|BIT2, 0x07); // [2]:use Hsync only, [1:0]:ISEL = analog1
    msWrite2ByteMask(SC01_0A, 0x0010, SC_MASK_H); // SPRHST = 0x10
    whact = 256*(ht_sel+1) - 16;
    msWrite2ByteMask(SC01_0E, whact,SC_MASK_H); // SPRHDC = htsel - 0x10
    // Vtotal is 64 lines, Vstart = 0x10, Vactive = 0x30
    msWrite2ByteMask(SC01_08, 0x0010, SC_MASK_V); // SPRVST = 0x10
    msWrite2ByteMask(SC01_0C, 0x0030,SC_MASK_V); // SPRVDC = 0x30
    msWrite2ByteMask(REG_2E24, whact, 0x0FFF); // reg_hde
    msWriteByteMask(REG_2E26, 0x05, 0xFF); // reg_hfp_w
    msWriteByteMask(REG_2E28, 0x06, 0xFF); // reg_hsync_w
    msWrite2ByteMask(REG_2E2A, 256*(ht_sel+1), 0x0FFF); // reg_htotal
    msWrite2ByteMask(REG_2E2C, 0x30, 0x0FFF); // reg_vde
    msWriteByteMask(REG_2E2E, 0x05, 0x7F); // reg_vfp_w
    msWriteByteMask(REG_2E30, 0x06, 0x3F); // reg_vsync_w
    msWrite2ByteMask(REG_2E32, 0x40, 0x0FFF); // reg_vtotal
    msWriteByteMask(REG_2E20, BIT0, BIT0); // Timing gen enable
    msWriteByteMask(REG_2E21, BIT7, BIT7); // REG_2E10[15]: Data����ADC data
}

#if 1
//**************************************************************************
//  [Function Name]:
//                  _msADC_MaskExternalSync()
//  [Description]:
//                  Mask external sync when connecting with source
//  [Precondition]:
//
//  [Arguments]:
//                 bflag: TRUE: Mask, FALSE: non-Mask
//
//  [Return]:
//
//**************************************************************************
static void _msADC_MaskExternalSync(Bool bflag)
{
    if(bflag)
        msWriteByteMask(REG_ADC_ATOP_05_H, 0x07, 0x07); // power down Hsync_Cmp, REG_ADC_ATOP_05_L[10:8]
    else
        msWriteByteMask(REG_ADC_ATOP_05_H, 0, 0x07); // power on Hsync_Cmp
}
#endif
#endif //#ifdef _DGAIN_CAL_WITHOUT_INPUT_

#if (defined(_DGAIN_CAL_WITH_INPUT_) || defined(_DGAIN_CAL_WITHOUT_INPUT_))
//**************************************************************************
//  [Function Name]:
//                  drvADC_BackupScalerSetting()
//  [Description]:
//                  Backup scaler settings
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void drvADC_BackupScalerSetting(void)
{
    return;

    memset(&gScalerBackupSetting, 0, sizeof(gScalerBackupSetting));
    gScalerBackupSetting.DBuffer = msReadByte(SC00_02);
    gScalerBackupSetting.ISEL = msReadByte( SC01_04 );
    gScalerBackupSetting.SPRHST = msRead2Byte( SC01_0A ) & SC_MASK_H; //???? 0xFFF
    gScalerBackupSetting.SPRHDC = msRead2Byte( SC01_0E ) & SC_MASK_H;
    gScalerBackupSetting.SPRVST = msRead2Byte( SC01_08 ) & SC_MASK_V; //???? 0xFFF
    gScalerBackupSetting.SPRVDC = msRead2Byte( SC01_0C ) & SC_MASK_V;
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_RecoverScalerSetting()
//  [Description]:
//                  Recover scaler settings
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void drvADC_RecoverScalerSetting(void)
{
    return;

    msWriteByteMask(SC00_02, gScalerBackupSetting.DBuffer, 0xFF);
    msWriteByteMask( SC01_04, gScalerBackupSetting.ISEL, 0xFF );
    msWrite2ByteMask( SC01_0A, gScalerBackupSetting.SPRHST, SC_MASK_H ); //????
    msWrite2ByteMask( SC01_0E, gScalerBackupSetting.SPRHDC, SC_MASK_H );
    msWrite2ByteMask( SC01_08, gScalerBackupSetting.SPRVST, SC_MASK_V );  //????
    msWrite2ByteMask( SC01_0C, gScalerBackupSetting.SPRVDC, SC_MASK_V );
    //_msWriteByteMask(REG_2FC0, 0x00, 0x1F); //[4]: OSD to IP disable, [3]: disable PG's Hsync/Vsync/Hde/Vde to IP, [2]: Pattern gen engine disable, [1:0]: Htotal is 256
    msWriteByteMask(REG_2E20, 0, BIT0); // Timing gen enable
    msWriteByteMask(REG_2E21, 0, BIT7); // REG_2E10[15]: Data����ADC data
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_SetInternalVcal()
//  [Description]:
//                  Enable internal Vcal(0.55V or 1.05V) for gain calibration
//  [Precondition]:
//
//  [Arguments]:
//                 vcal: 0.55V / 1.05V
//
//  [Return]:
//
//**************************************************************************
static void drvADC_SetInternalVcal(msADC_InternalVcal vcal)
{
    msWriteByteMask(REG_ADC_DTOPB_04_H, 0x18, 0x1C); // [12]: ADCA SELF_CAL_G &  CAL_G pulse force enable, [11]: ADCA SELF_CAL_G pulse force value
    switch(vcal)
    {
        case E_Vcal_0_55V:
        #if DADCCALBYBANDGAP
            msWriteByteMask(REG_ADC_ATOP_5A_L, 0, BIT0); // turn off LDO
            msWriteByteMask(REG_ADC_DTOPB_05_L, 0, 0x0F); // turn off LDO
            msWriteByteMask(REG_ADC_DTOPB_05_H, 0x1B, 0x1F); // set Vcal = 0.55V
        #else
            msWriteByteMask(REG_ADC_ATOP_5A_L, BIT0, BIT0); // turn on LDO
            msWriteByteMask(REG_ADC_DTOPB_05_L, 0x0E, 0x0F); // set Vcal = 0.55V
        #endif // DADCCALBYBANDGAP

            break;

        case E_Vcal_1_05V:
        #if DADCCALBYBANDGAP
            msWriteByteMask(REG_ADC_ATOP_5A_L, 0, BIT0); // turn off LDO
            msWriteByteMask(REG_ADC_DTOPB_05_L, 0, 0x0F); // turn off LDO
            msWriteByteMask(REG_ADC_DTOPB_05_H, 0x1F, 0x1F); // set Vcal = 1.05V
        #else
            msWriteByteMask(REG_ADC_ATOP_5A_L, BIT0, BIT0); // turn on LDO
            msWriteByteMask(REG_ADC_DTOPB_05_L, 0x0F, 0x0F); // set Vcal = 1.05V
        #endif // DADCCALBYBANDGAP
            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_IsReadLineBuffer()
//  [Description]:
//                  It's used to read scaler line buffer
//  [Precondition]:
//
//  [Arguments]:
//                 bflag: TRUE: reading, FALSE: recover to the normal state
//  [Return]:
//
//**************************************************************************
static Bool drvADC_IsReadLineBuffer(Bool bflag)
{
    Bool result=TRUE;
    BYTE retry=5;

    return TRUE;

    if(bflag)
    {
        _msWriteByteMask( SC5_32, BIT6, BIT6 ); // latch data in line buffer
        while(retry--)
        {
            ForceDelay1ms( 20 );
            if( _msReadByte( SC5_32 ) & BIT7 ) // Check ready bit for reading line buffer
            {
                //ADC_DPUTSTR("Line buffer ready for read\n");
                break;
            }
        }
        if(!retry)
        {
            result = FALSE;
            ADC_DPUTSTR("Reading Line buffer fail !!\n");
        }
    }
    else
    {
        _msWriteByteMask( SC5_32, 0, BIT6 ); // disable latch data in line buffer
        //ADC_DPUTSTR("Turn off reading Line buffer \n");
    }
    return result;
}

//                  Adjust gain to approach target value 182(8 bits) => 255 * 0.5/0.7 = 182
//**************************************************************************
//  [Function Name]:
//                  drvADC_GetRGBLineBufAverageData()
//  [Description]:
//                  Get the average line buffer data for R/G/B gain calibration
//                  SC5_32[4:0]31[7:0] = 0 ~ 2560(total is 2560), each has two pixels and format is following
//                  SC5_35[7:0]: R0, SC5_36[7:0]: G0, SC5_37[7:0]: B0
//                  SC5_38[7:0]: R1, SC5_39[7:0]: G1, SC5_3A[7:0]: B1
//  [Precondition]:
//
//  [Arguments]:
//                 wLineBufAddress: line buffer address, it should be less than PG Htotal
//                 LineBuf: choose one line to read, 0 ~ 3
//                 AvgCnt: average pixel count
//  [Return]:
//                 msADCAvgVal: R/G/B average data
//**************************************************************************
static msADCAvgVal drvADC_GetRGBLineBufAverageData(WORD wLineBufAddress, BYTE LineBuf, BYTE AvgCnt)
{
    BYTE XDATA i;
    WORD XDATA wAvgRdata=0, wAvgGdata=0, wAvgBdata=0;
    msADCAvgVal XDATA pAutoAdc;

    memset(&pAutoAdc, 0 , sizeof(pAutoAdc));

    return pAutoAdc;

    for( i = 0; i < AvgCnt; i++ )
    {
        _msWrite2ByteMask( SC5_31, LineBuf*1500+wLineBufAddress + ( i / 3 ), 0x7FF ); // config line buffer address
        _msWriteByteMask( SC5_33, i%3, 0x03 ); // [1:0] : LB packed pixel selection, 00: 1st pixel, 01: 2nd pixel, 10: 3rd pixel
        wAvgRdata += _msReadByte( SC5_35 ); //read R0 line buffer data
        wAvgGdata += _msReadByte( SC5_36 ); //read G0 line buffer data
        wAvgBdata += _msReadByte( SC5_37 ); //read B0 line buffer data
    }


    pAutoAdc.AVG_Bch = wAvgBdata/AvgCnt;
    pAutoAdc.AVG_Gch = wAvgGdata/AvgCnt;
    pAutoAdc.AVG_Rch = wAvgRdata/AvgCnt;

    return pAutoAdc;
}

//**************************************************************************
//  [Function Name]:
//                  drvADC_AutoGainAdjustment()
//  [Description]:
//                  Adjust gain to approach target value 182(8 bits) => 255 * 0.5/0.7 = 182
//  [Precondition]:
//
//  [Arguments]:
//
//  [Return]:
//                  status
//**************************************************************************
static Bool drvADC_AutoGainAdjustment(void)
{
    xdata BYTE retry=11;
    xdata BYTE CheckFlag=0x00;
    xdata BYTE Diff;
    xdata msADCAvgVal pAutoAdc_1_05V;
    xdata msADCAvgVal pAutoAdc_0_55V;
    xdata WORD InitADCGainVal = 0x1000;
    xdata WORD rGain = InitADCGainVal;
    xdata WORD gGain = InitADCGainVal;
    xdata WORD bGain = InitADCGainVal;
    xdata WORD GainOffset_R = InitADCGainVal >> 1;
    xdata WORD GainOffset_G = InitADCGainVal >> 1;
    xdata WORD GainOffset_B = InitADCGainVal >> 1;
    xdata BYTE Direction_R = 0;
    xdata BYTE Direction_G = 0;
    xdata BYTE Direction_B = 0;


    // 5. Calibrate ADC R/G/B gain setting value
    // Set R/G/B gain initial setting value at 0x0400
    drvADC_Write2BytesMask(REG_ADC_DTOP_51_L, InitADCGainVal, 0x3FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_56_L, InitADCGainVal, 0x3FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_5B_L, InitADCGainVal, 0x3FFF);
    // Set ADC R/G/B Black and offset level to 0.8V(511)
    drvADC_Write2BytesMask(REG_ADC_DTOP_50_L, 0x07FF, 0x0FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_55_L, 0x07FF, 0x0FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_5A_L, 0x07FF, 0x0FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_52_L, 0x07FF, 0x1FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_57_L, 0x07FF, 0x1FFF);
    drvADC_Write2BytesMask(REG_ADC_DTOP_5C_L, 0x07FF, 0x1FFF);

    // 6. Gain calibration target value for RGB color space mode is 182 (8bit data) 255 * 0.5 / 0.7 = 182
    // 7. Get the ADC digital output with Turn-On LDO and Set VCAL to 1.05v
    // 8. Get the ADC digital output with VCAL to 0.55v
    // 9. Calculate the difference between 1.05V and 0.55V of these 3 channels.
    //    Compare the difference value with target value (182 at RGB color space) to adjust R/G/B channel gain register
    //_msADC_MaskExternalSync(TRUE);

    while (retry--)
    {
        // Vcal = 1.05V
        drvADC_SetInternalVcal(E_Vcal_1_05V);
        ForceDelay1ms(16);
        drvADC_IsReadLineBuffer(TRUE);
        pAutoAdc_1_05V = drvADC_GetRGBLineBufAverageData(0x40, 0, 32); // (2*line buffer address + average data) < SPRHDC: (2*64+32) < 240
        drvADC_IsReadLineBuffer(FALSE);
        // Vcal = 0.55V
        drvADC_SetInternalVcal(E_Vcal_0_55V);
        ForceDelay1ms(16);
        drvADC_IsReadLineBuffer(TRUE);
        pAutoAdc_0_55V = drvADC_GetRGBLineBufAverageData(0x40, 0, 32); // (2*line buffer address + average data) < SPRHDC: (2*64+32) < 240
        drvADC_IsReadLineBuffer(FALSE);

        if ( (CheckFlag & BIT0) == 0 ) // Check Rch gain is approached target value or not
        {
            Diff = pAutoAdc_1_05V.AVG_Rch - pAutoAdc_0_55V.AVG_Rch;
            //ADC_DPRINTF("R gain Diff value=%d\n", Diff);
            if( (Diff >= AUTO_GAIN_RGB_MINVAL) && (Diff <= AUTO_GAIN_RGB_MAXVAL) )
            {
                CheckFlag|=BIT0; // Rch is calibrated completely
                //ADC_DPUTSTR("Rch auto gain complete!!\n");
            }
            else
            {
                if (Diff < AUTO_GAIN_RGB_MINVAL )
                {
                    rGain += GainOffset_R;
                    if ( retry == 2)
                    {
                        Direction_R = 1; //DownToUp
                    }
                }
                else   //(u16Diff>AUTO_GAIN_MAX )
                {
                    rGain-=GainOffset_R;
                    if ( retry == 2)
                    {
                        Direction_R = 2; //DownToUp
                    }
                }
                GainOffset_R=GainOffset_R>>1;
            }
        }
        if ( (CheckFlag & BIT1) == 0x00 ) // Check Gch gain is approached target value or not
        {
            Diff = pAutoAdc_1_05V.AVG_Gch - pAutoAdc_0_55V.AVG_Gch;
            //ADC_DPRINTF("G gain Diff value=%d\n", Diff);
            if( (Diff >= AUTO_GAIN_RGB_MINVAL) && (Diff <= AUTO_GAIN_RGB_MAXVAL) )
            {
                CheckFlag|=BIT1;
                //ADC_DPUTSTR("Gch auto gain complete !!\n");
            }
            else
            {
                if (Diff< AUTO_GAIN_RGB_MINVAL )
                {
                    gGain+=GainOffset_G;
                    if ( retry == 2)
                    {
                        Direction_G = 1; //DownToUp
                    }
                }
                else   //(u16Diff>AUTO_GAIN_MAX )
                {
                   gGain-=GainOffset_G;
                   if ( retry == 2)
                   {
                       Direction_G = 2; //DownToUp
                   }
                }
                GainOffset_G=GainOffset_G>>1;
            }
        }
        if ( (CheckFlag & BIT2) == 0x00 )
        {
            Diff = pAutoAdc_1_05V.AVG_Bch - pAutoAdc_0_55V.AVG_Bch;
            //ADC_DPRINTF("B gain Diff value=%d\n", Diff);
            if( (Diff >= AUTO_GAIN_RGB_MINVAL) && (Diff <= AUTO_GAIN_RGB_MAXVAL) )
            {
                CheckFlag|=BIT2;
                //ADC_DPUTSTR("Bch auto gain complete !!\n");
            }
            else
            {
                if (Diff< AUTO_GAIN_RGB_MINVAL )
                {
                    bGain+=GainOffset_B;
                    if ( retry == 2)
                    {
                        Direction_B = 1; //DownToUp
                    }
                }
                else   //(u16Diff>AUTO_GAIN_MAX )
                {
                    bGain-=GainOffset_B;
                    if ( retry == 2)
                    {
                        Direction_B = 2; //DownToUp
                    }
                }
                GainOffset_B=GainOffset_B>>1;
            }
        }
        if(retry==1)
        {
            if (Direction_R == 1)
            {
                rGain-=1;
            }
            else if (Direction_R == 2)
            {
                rGain+=1;
            }
            if (Direction_G == 1)
            {
                gGain-=1;
            }
            else if (Direction_G == 2)
            {
                gGain+=1;
            }
            if (Direction_B == 1)
            {
                bGain-=1;
            }
            else if (Direction_B == 2)
            {
                bGain+=1;
            }
            drvADC_Write2BytesMask(REG_ADC_DTOP_51_L, rGain, 0x3FFF);
            drvADC_Write2BytesMask(REG_ADC_DTOP_56_L, gGain, 0x3FFF);
            drvADC_Write2BytesMask(REG_ADC_DTOP_5B_L, bGain, 0x3FFF);
        }
        else
        {
            drvADC_Write2BytesMask(REG_ADC_DTOP_51_L, rGain, 0x3FFF);
            drvADC_Write2BytesMask(REG_ADC_DTOP_56_L, gGain, 0x3FFF);
            drvADC_Write2BytesMask(REG_ADC_DTOP_5B_L, bGain, 0x3FFF);
        }
        if((CheckFlag&0x07)==0x07)
            break;
    }

    if ((CheckFlag&0x07)==0x07)
    {
        ADC_DPUTSTR("Auto gain OK");
        USER_PREF_ADC_RED_GAIN = rGain;
        USER_PREF_ADC_GREEN_GAIN = gGain;
        USER_PREF_ADC_BLUE_GAIN = bGain;
        ADC_DPRINTF("Rgain = 0x%x", rGain);
        ADC_DPRINTF("Ggain = 0x%x", gGain);
        ADC_DPRINTF("Bgain = 0x%x", bGain);
        return TRUE;
    }
    else
    {
        ADC_DPUTSTR("Auto gain fail !!\n");
        ADC_DPRINTF("Rgain = 0x%x", rGain);
        ADC_DPRINTF("Ggain = 0x%x", gGain);
        ADC_DPRINTF("Bgain = 0x%x", bGain);
        return FALSE;
    }
}
#endif //#if (defined(_DGAIN_CAL_WITH_INPUT_) || defined(_DGAIN_CAL_WITHOUT_INPUT_))

void drvADC_SetHsyncRefLevel(ADC_HSYNCLVL_T level)
{
    msWriteByteMask(REG_ADC_ATOP_30_L, level, 0x07);
}

#if 0

void drvADC_Test_Funct(BYTE tb1, BYTE tb2)
{
//uncall    BYTE uncallvalue = tb2;
    BYTE ucInputPort;

    ucInputPort = SrcInputPortM;

    switch(tb1)
    {
        case 0: // mismatch calibration
            //drvADC_ADCOffsetGainMismatchCal();
            break;
#if 0
        case 1: // auto gain with input
            msADC_ADCAutoGainCalwithInput();
            break;
        case 2: // auto gain without input
            switch(tb2)
            {
                case 0:
                    // 1. backup ADC and Scaler related settings
                    _msADC_BackupADCSetting();
                    _msADC_BackupScalerSetting();

                    // 2. Set free run clock at 720p
                    _msADC_SetFreeRunClock();
                    break;
                case 1:
                    // 3. Enable scaler pattern generator to generate Hsync and Vsync for line buffer
                    _msADC_EnableScalerPG( E_PG_Htotal_256 );
                    break;
                case 2:
                    // 4. Set ADC input LPF to lowest bandwidth
                    _msADC_Write2BytesMask( REG_ADC_ATOP_23_L, 0xFFFF, 0xFFFF );
                    _msADC_Write2BytesMask( REG_ADC_ATOP_24_L, 0x00FF, 0x00FF );
                    break;
                case 3:
                    // 5. Calibrate ADC R/G/B gain setting value
                    // Set R/G/B gain initial setting value at 0x0400
                    // 6. Gain calibration target value for RGB color space mode is 182 (8bit data) 255 * 0.5 / 0.7 = 182
                    // 7. Get the ADC digital output with Turn-On LDO and Set VCAL to 1.05v
                    // 8. Get the ADC digital output with VCAL to 0.55v
                    // 9. Calculate the difference between 1.05V and 0.55V of these 3 channels.
                    //    Compare the difference value with target value (182 at RGB color space) to adjust R/G/B channel gain register
                    _msADC_AutoGainAdjustment();
                    break;
                case 4:
                    // 10. recover ADC and scaler related settings & turn off free run clock & turn off internal Vcal
                    _msADC_RecoverADCSetting();
                    _msADC_RecoverScalerSetting();
                    break;

                case 5:
                    msADC_ADCAutoGainCalwithoutInput();
                    break;
            }
            break;
#endif
        case 3:
            drvADC_AutoAdcColor(20, ucInputPort);
            break;
        case 4:
            mStar_AutoPhase(50, MapPort2DetScIndex(INPUT_VGA));
            break;
#ifdef _DGAIN_CAL_WITHOUT_INPUT_
        case 5:
                drvADC_AutoGainAdjustment();
            break;

        case 6:
            drvADC_ADCAutoGainCalwithoutInput();
            break;

        case 7:
#if 0
            ADC_DPUTSTR("LDO:");
            //drvADC_ADCAutoGainCalwithoutInput();
            //_msADC_SetInternalVcal(E_Vcal_1_05V);
            {
            msADCAvgVal ptmp;
            msADCAvgVal pAutoAdc_val;

            // 1. backup ADC and Scaler related settings
            drvADC_BackupADCSetting();
            drvADC_BackupScalerSetting();

            // 2. Set free run clock at 720p
            drvADC_SetFreeRunClock();
            _msADC_MaskExternalSync(TRUE);

            // 3. Enable scaler pattern generator to generate Hsync and Vsync for line buffer
            drvADC_EnableScalerPG(E_PG_Htotal_256);

            // 4. Set ADC input LPF to lowest bandwidth
            drvADC_Write2BytesMask(REG_ADC_ATOP_23_L, 0xFFFF, 0xFFFF);
            drvADC_Write2BytesMask(REG_ADC_ATOP_24_L, 0x00FF, 0x00FF);

            _msWrite2ByteMask(REG_ADC_ATOP_34_L, BIT0, BIT0); // turn on LDO
            _msWrite2ByteMask(REG_ADC_ATOP_54_L, 0, 0xF800); // disable bandgap overwrite
            _msWrite2ByteMask(REG_ADC_ATOP_34_L, BIT2, BIT2); // LDO, Aaron test
            _msWrite2ByteMask(REG_ADC_ATOP_54_L, 0x0003, 0x0003); // set Vcal = 1.05V
            _msWrite2ByteMask(REG_ADC_ATOP_54_L, 0x000C, 0x000C); // set Vcal from LDO
            _msWrite2ByteMask(REG_ADC_ATOP_54_L, 0x0600, 0x0700); // turn on Gcal pulse

            Delay1ms(1500);
            drvADC_IsReadLineBuffer(TRUE);
            ptmp = drvADC_GetRGBLineBufAverageData(0x40, 0, 32); // (2*line buffer address + average data) < SPRHDC: (2*64+32) < 240
            drvADC_IsReadLineBuffer(FALSE);

            _msWrite2ByteMask(REG_ADC_ATOP_34_L, BIT0, BIT0); // turn on LDO
            _msWrite2ByteMask(REG_ADC_ATOP_34_L, BIT2, BIT2); // LDO, Aaron test
            _msWrite2ByteMask(REG_ADC_ATOP_54_L, 0x0002, 0x0003); // set Vcal = 0.55V
            _msWrite2ByteMask(REG_ADC_ATOP_54_L, 0x000C, 0x000C); // set Vcal from LDO
            _msWrite2ByteMask(REG_ADC_ATOP_54_L, 0x0600, 0x0700); // turn on Gcal pulse

            Delay1ms(1500);
            drvADC_IsReadLineBuffer(TRUE);
            pAutoAdc_val = drvADC_GetRGBLineBufAverageData(0x40, 0, 32); // (2*line buffer address + average data) < SPRHDC: (2*64+32) < 240
            drvADC_IsReadLineBuffer(FALSE);

            // 10. recover ADC and scaler related settings & turn off free run clock & turn off internal Vcal
            _msADC_MaskExternalSync(FALSE);
            drvADC_RecoverADCSetting();
            drvADC_RecoverScalerSetting();

            ADC_DPRINTF("AVG_R_1.05V= 0x%x", ptmp.AVG_Rch);
            ADC_DPRINTF("AVG_G_1.05V = 0x%x", ptmp.AVG_Gch);
            ADC_DPRINTF("AVG_B_1.05V = 0x%x", ptmp.AVG_Bch);
            ADC_DPRINTF("AVG_R_0.55V = 0x%x", pAutoAdc_val.AVG_Rch);
            ADC_DPRINTF("AVG_G_0.55V = 0x%x", pAutoAdc_val.AVG_Gch);
            ADC_DPRINTF("AVG_B_0.55V = 0x%x", pAutoAdc_val.AVG_Bch);
            ADC_DPRINTF("AVG_R_diff = 0x%x", ptmp.AVG_Rch - pAutoAdc_val.AVG_Rch);
            ADC_DPRINTF("AVG_G_diff = 0x%x", ptmp.AVG_Gch - pAutoAdc_val.AVG_Gch);
            ADC_DPRINTF("AVG_B_diff = 0x%x", ptmp.AVG_Bch - pAutoAdc_val.AVG_Bch);
            }
#endif
            break;
        case 8:
            ADC_DPUTSTR("Get:");
            //drvADC_ADCAutoGainCalwithoutInput();
            //_msADC_SetInternalVcal(E_Vcal_1_05V);
            {
            msADCAvgVal ptmp;

            drvADC_IsReadLineBuffer(TRUE);
            ptmp = drvADC_GetRGBLineBufAverageData(0x40, 0, 32); // (2*line buffer address + average data) < SPRHDC: (2*64+32) < 240
            drvADC_IsReadLineBuffer(FALSE);

            ADC_DPRINTF("AVG_R = 0x%x", ptmp.AVG_Rch);
            ADC_DPRINTF("AVG_G = 0x%x", ptmp.AVG_Gch);
            ADC_DPRINTF("AVG_B = 0x%x", ptmp.AVG_Bch);
            }
            break;
#endif
        default:
            tb1=tb2; // unused param
            break;
    }
}
#endif // #if DEBUG_EN
#endif

#else

#if 1//DUMMY_CODE_4_LINK_FILE
BYTE code DrvADCDummy[]={0xFF};
#endif

void DrvADCNoUse(void)
{
BYTE i;
  i=DrvADCDummy[0];
}

#endif //ENABLE_VGA_INPUT

void drvADC_PowerCtrl(BYTE u8State)
{
    if(u8State==ADC_POWER_ON)
    {
        msWrite2Byte( REG_ADC_ATOP_04_L, 0x0000 );  // set power-on default value to power down ADC
        msWrite2ByteMask( REG_ADC_ATOP_05_L, 0x0000, 0x1FFF);  // set power-on default value to power down ADC
        msWrite2Byte(REG_ADC_ATOP_06_L, 0x0000); // ADC clock power on
        msWriteByteMask(REG_ADC_ATOP_3F_L, 0, BIT0); // on-line SOG MUX power on
#if _NEW_SOG_DET_
        msWrite2Byte( REG_ADC_PMATOP_78_L, 0x0002 );  // [15:4]: reg_isog_rst_period = 0, [3:0]: reg_sog_high_num = 2
        msWrite2Byte(REG_ADC_PMATOP_79_L, 0x00C8); // [15:12]: reg_isog_rst_width = 0, [11:0]: reg_sog_cnt_limit = 'h C8
        msWriteByteMask(REG_ADC_PMATOP_7A_L, BIT2, BIT2|BIT1); // on-line SOG MUX power on
#endif
        Delay1ms(1);
        msWriteByteMask(REG_ADC_ATOP_19_L, BIT3, BIT3); // ADC phase alignment trigger
        msWriteByteMask(REG_ADC_ATOP_19_L, 0, BIT3);    // ADC phase alignment trigger
    }
    else if(u8State==ADC_POWER_STANDBY)
    {
#if _NEW_SOG_DET_
        msWrite2Byte( REG_ADC_PMATOP_78_L, 0x0002 );  // set power-on default value to power down ADC
        msWrite2Byte(REG_ADC_PMATOP_79_L, 0x00C8); // ADC clock power down
        msWriteByteMask(REG_ADC_PMATOP_7A_L, BIT2, BIT2|BIT1); // on-line SOG MUX power on
#endif
        //saperate & SOG support
        msWrite2Byte(REG_ADC_ATOP_06_L, 0x0000); // ADC clock power on
        msWriteByteMask(REG_ADC_ATOP_3F_L, 0, BIT0); // on-line SOG MUX power on
    }
    else //ADC_POWER_DOWN
    {
#if _NEW_SOG_DET_
        msWrite2Byte( REG_ADC_PMATOP_78_L, 0x9280 );  // [15:4]: reg_isog_rst_period = h��928(200 msec), [3:0]: reg_sog_high_num = 0
        msWrite2Byte(REG_ADC_PMATOP_79_L, 0x1000); // [15:12]: reg_isog_rst_width = ��h1, [11:0]: reg_sog_cnt_limit = 0
        msWriteByteMask(REG_ADC_PMATOP_7A_L, 0, BIT2|BIT1); // on-line SOG MUX power on
#endif
        msWrite2Byte( REG_ADC_ATOP_04_L, 0xFFFF );  // set power-on default value to power down ADC
        msWrite2ByteMask( REG_ADC_ATOP_05_L, 0x1FFF, 0x1FFF);  // set power-on default value to power down ADC
        msWrite2Byte(REG_ADC_ATOP_06_L, 0xFFFF); // ADC clock power on
        msWriteByteMask(REG_ADC_ATOP_3F_L, BIT0, BIT0); // on-line SOG MUX power on
    }
}

#endif // _DRVADC_C_
