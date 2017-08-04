///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   MsADC.h
/// @author MStar Semiconductor Inc.
/// @brief  ADC Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MSADC_H_
#define _MSADC_H_

#include "Mdrv_adctbl.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//#define DGAINCALWITHINPUT
//#define DGAINCALWITHOUTINPUT
#define DADCWORSTPHASE    0 // ADC find good phase criteria by worst phase
#define DADCPLLPROTECTION    1 // ADC PLL protection mechnism

#define OffsetValue         (10)    //110401 Modify
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    // Unknow
    ADC_INPUTSOURCE_UNKNOW = 0x00,
    // Single source
    ADC_INPUTSOURCE_RGB    = 0x01,
    ADC_INPUTSOURCE_YPBPR  = 0x02,
}ADC_INPUTSOURCE_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

extern void msADC_init(Bool IsShareGrd);
extern void msADC_SetInputMux(InputPortType port_type );
extern void msADC_SetADCSource(ADC_SOURCE_TYPE inputsrc_type);
extern void msADC_SetADCModeSetting(ADC_INPUTSOURCE_TYPE enADCInput, WORD u16PixelClk);
extern void msADC_AdjustHTotal(WORD htotal);
extern void msADC_SetPhaseCode(BYTE phasecode);
extern void msADC_SetRGBGainCode(WORD rcode, WORD gcode, WORD bcode);
extern void msADC_SetRGBOffsetCode(WORD rcode, WORD gcode, WORD bcode);
extern void msADC_SetPgaGain   ( BYTE ucAdcPgaGain );    //110401 Modify
extern void msADC_ADCOffsetGainMismatchCal(void);
extern Bool msADC_AutoPhaseAdjustment(BYTE vsyncTime);
extern Bool msADC_AutoAdcColor(BYTE vsyncTime);
extern void msADC_SetupHsyncPolarity(Bool bActiveHigh);
extern void msADC_AdjustCalDuring(BYTE u8CalDur);
#if AdjustFocusOSDValueBy1Step
extern void msADC_SetPhaseCode_100(BYTE phasecode);
#endif
#ifdef DADCPLLPROTECTION
extern Bool msADC_SetModewithPLLProtection(ADC_INPUTSOURCE_TYPE enADCInput, WORD u16PixelClk, WORD htotal);
#endif //#ifdef DADCPLLPROTECTION
#ifdef DGAINCALWITHOUTINPUT
extern void msADC_ADCAutoGainCalwithoutInput(void);
#endif
#ifdef DGAINCALWITHINPUT
extern void msADC_ADCAutoGainCalwithInput(void);
#endif
#if 0 	//111021 Modify
extern void drvAdcAdjustCalDuring(BYTE u8CalDur);
#endif

#endif //_MSADC_H_

