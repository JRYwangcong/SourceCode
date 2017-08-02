/******************************************************************************
 Copyright (c) 2003 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: msACE.h
 [Date]:        01-Dec-2003
 [Comment]:
   Color adjust header file.
 [Reversion History]:
*******************************************************************************/

#ifndef _MSACE_H_
#define _MSACE_H_

#ifdef _MSACE_C_
  #define _MSACEDEC_
#else
  #define _MSACEDEC_ extern
#endif


#define MAX_DISPALY_WINDOWS     DISPLAY_MAX_NUMS  // 4
#define REG_ADDR_SC_BANK_SEL 0
#define REG_ADDR_CM_MAIN    (SC10_4C)
#define REG_ADDR_CM_CTL_MAIN (SC10_5E)
#define REG_ADDR_CM_SUB     (SC0F_3A)
#define REG_ADDR_CM_CTL_SUB (SC0F_4C)

#define REG_ADDR_SRGBCM_MAIN    (SC25_04)
#define REG_ADDR_SRGBCM_CTL_MAIN (SC25_02)
#define REG_ADDR_SRGBCM_SUB     (SC25_24)
#define REG_ADDR_SRGBCM_CTL_SUB (SC25_22)

#define REG_ADDR_PRECON_MAIN    (SC0F_A6)
#define REG_ADDR_PRECON_SUB    (SC0F_A7)

#define REG_ADDR_POSTCON_MAIN    (SC25_02)
#define REG_ADDR_POSTCON_SUB    (SC25_22)
///////////////////////////////////////////////////////////////////////////////
// Basic color control function(Library)
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Get ACE library version
///////////////////////////////////////////////////////////////////////
WORD msACE_GetVersion(void);

typedef enum
{
    ACE_INFO_TYPE_VERSION,
    ACE_INFO_TYPE_CONTRAST,
    ACE_INFO_TYPE_R_GAIN,
    ACE_INFO_TYPE_G_GAIN,
    ACE_INFO_TYPE_B_GAIN,
    ACE_INFO_TYPE_SATURATION,
    ACE_INFO_TYPE_HUE,
    ACE_INFO_TYPE_COLOR_CORRECTION_XY_R,
    ACE_INFO_TYPE_COLOR_CORRECTION_XY_G,
    ACE_INFO_TYPE_COLOR_CORRECTION_XY_B,
    ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_R,
    ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_G,
    ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_B,

    ACE_INFO_TYPE_DUMMY
} EnuACEInfoType;
WORD msACE_GetInfo( BYTE ucWinIndex, EnuACEInfoType ucInfoType );


// Initialize ACE varible
// Must call this function when power on
void InitACEVar(void);

//------------------------------------------------------------------------
// Only for Video input, not support PC input
// Input:
//      ucWinIndex: display window select
//      ucContrast: 0~FFh, (gain range x0 ~ x2, 80h is x1)
//      Mstar use: 50h~A0h
//------------------------------------------------------------------------
void msAdjustVideoContrast( BYTE ucWinIndex, BYTE ucContrast );

//------------------------------------------------------------------------
// Only for Video input, not support PC input
// Input:
//      ucWinIndex: display window select
//      ucContrast: 0~1023, (gain range x0 ~ x2, 512 is x1)
//------------------------------------------------------------------------
void msAdjustVideoContrast_10Bits( BYTE ucWinIndex, WORD wContrast );

//------------------------------------------------------------------------
// Only for Video input, not support PC input
// Input:
//      ucWinIndex: display window select
//      ucSaturation: 0~FFh, (gain range x0 ~ x2, 80h is x1)
//      Mstar use: 50h~A0h
//------------------------------------------------------------------------
void msAdjustVideoSaturation( BYTE ucWinIndex, BYTE ucSaturation );
//------------------------------------------------------------------------
// Only for Video input, not support PC input
// Input:
//      ucWinIndex: display window select
//      ucHue: 0~100, (angel -50 degree ~ angel 50 degree, 50 is 0 degree)
//      Mstar use: 0~100
//------------------------------------------------------------------------
void msAdjustVideoHue( BYTE ucWinIndex, WORD uwHue );

//------------------------------------------------------------------------
// Only for Video input, not support PC input
// Adjust vide R,G,B gain
// Input:
//      ucWinIndex: display window selects
//      ucContrast: 0~FFh, (gain range x0 ~ x2, 80h is x1)
//------------------------------------------------------------------------
void msAdjustVideoRGB( BYTE ucWinIndex, BYTE ucRCon, BYTE ucGCon, BYTE ucBCon);

//------------------------------------------------------------------------
// Only for Video input, not support PC input
// Adjust vide R,G,B gain
// Input:
//      ucWinIndex: display window selects
//      ucContrast: 0~1023, (gain range x0 ~ x2, 512 is x1)
//------------------------------------------------------------------------
void msAdjustVideoRGB_10Bits( BYTE ucWinIndex, WORD ucRCon, WORD ucGCon, WORD ucBCon);

//------------------------------------------------------------------------
// Only for PC input, not support Video input
// Input:
//      ucWinIndex: display window select
//      ucContrast: 0~FFh, (gain range x0 ~ x2, 80h is x1)
//      Mstar use: 50h~A0h
//------------------------------------------------------------------------
void msAdjustPCContrast( BYTE ucWinIndex, BYTE ucContrast );

//------------------------------------------------------------------------
// Only for PC input, not support Video input
// Input:
//      ucWinIndex: display window select
//      ucContrast: 0~1023, (gain range x0 ~ x2, 512 is x1)
//------------------------------------------------------------------------
void msAdjustPCContrast_10Bits( BYTE ucWinIndex, WORD wContrast );

//------------------------------------------------------------------------
// Only for PC input, not support Video input
// Adjust vide R,G,B gain
// Input:
//      ucWinIndex: display window select
//      ucRed/ucGreen/ucBlue: 0~FFh, (gain range x0 ~ x2, 80h is x1)
//------------------------------------------------------------------------
void msAdjustPCRGB( BYTE ucWinIndex, BYTE ucRed, BYTE ucGreen, BYTE ucBlue );

//------------------------------------------------------------------------
// Only for PC input, not support Video input
// Adjust vide R,G,B gain
// Input:
//      ucWinIndex: display window select
//      wRed/wGreen/wBlue: 0~1023, (gain range x0 ~ x2, 512 is x1)
//------------------------------------------------------------------------
void msAdjustPCRGB_10Bits( BYTE ucWinIndex, WORD wRed, WORD wGreen, WORD wBlue );

///////////////////////////////////////////////////////////////////////////////
// Special color control function
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
// Only for Video input, not support PC input
// Adjust vide contrast, hue, saturation together
// Input:
//      ucWinIndex: display window select
//      ucContrast: 0~FFh, (gain range x0 ~ x4, 80h is x1)
//      ucHue: 0~100, (angel -50 degree ~ angel 50 degree, 50 is 0 degree)
//      ucSaturation: 0~FFh, (gain range x0 ~ x4, 80h is x1)
//------------------------------------------------------------------------
void msInitialVideoColorMatrix(BYTE ucWinIndex, WORD wHue, BYTE ucSaturation, WORD wContrast, WORD wRCon, WORD wGCon, WORD wBCon);

void msAdjustHSC(BYTE ucWinIndex, WORD uwHue, BYTE ucSaturation, BYTE ucContrast);

//------------------------------------------------------------------------
//  if ( SC1_04[3]: RGB -> YUV ) enable
//  else disable
//  If input video is RGB format, we need do this one for enable our ACE engine
//  Ex: DVI video input, HDMI input
//------------------------------------------------------------------------
void msACEPCForceYUVtoRGB( BYTE ucWinIndex, BOOL bEnable );

//---------------------------------------------------------------------------
// Specify the formula of YUV to RGB.
//---------------------------------------------------------------------------
#define ACE_YUV_TO_RGB_MATRIX_SDTV  0   // Use Std CSCM for SDTV
#define ACE_YUV_TO_RGB_MATRIX_HDTV  1   // Use Std CSCM for HDTV
#define ACE_YUV_TO_RGB_MATRIX_USER  2   // Use user define CSCM, specified by tUserYVUtoRGBMatrix
#define ACE_YUV_TO_RGB_MATRIX_SDTV_LIMIT  3   // Use Std CSCM for SDTV
#define ACE_YUV_TO_RGB_MATRIX_HDTV_LIMIT  4   // Use Std CSCM for HDTV
#define ACE_YUV_TO_RGB_MATRIX_SDTV_10Bit  5   // Use Std CSCM for SDTV
#define ACE_YUV_TO_RGB_MATRIX_HDTV_10Bit  6   // Use Std CSCM for HDT
#define ACE_YUV_TO_RGB_MATRIX_HDTV_YUV_LMITE 7  // Use Std CSCM for HDTV with YUV
#define ACE_YUV_TO_RGB_MATRIX_SDTV_YUV_LMITE   8// Use Std CSCM for SDTV with YUV
#define ACE_YUV_TO_RGB_MATRIX_BT2020  9  // Use Std BT2020

void msACESelectYUVtoRGBMatrix( BYTE ucWinIndex, BYTE ucMatrix, short* psUserYUVtoRGBMatrix );

//-------------------------------------------------------------------------------
// Specify the color correction table.
//-------------------------------------------------------------------------------
void msACESetColorCorrectionTable( BYTE ucWinIndex, short* psColorCorrectionTable );

//-------------------------------------------------------------------------------
// Specify the color clone table.
//-------------------------------------------------------------------------------
void msACESetColorCloneTable( BYTE ucWinIndex, short* psColorCloneTable );

//---------------------------------------------------------------------------
// Specify the sRGB table.
//---------------------------------------------------------------------------
void msACESetPCsRGBTable( BYTE ucWinIndex, short* psPCsRGBTable );

void msACESetRGBColorRange(BYTE ucWinIndex, Bool bLimitRange);

void msAdjustRGBOffset(BYTE ucWinIndex, WORD wRedOffset, WORD wGreenOffset, WORD wBlueOffset );

void msWriteColorMatrix( BYTE ucWinIndex, short* psMatrix);

void msEnableColorMatrix( BYTE ucWinIndex, BOOL benable, BOOL bRGBMode);

void msWritexvYccColorMatrix( BYTE ucWinIndex, short* psMatrix);

void msEnablexvYccColorMatrix( BYTE ucWinIndex, BOOL benable);

//---------------------------------------------------------------------------
// Use Post contrast instead Pre contrast
//---------------------------------------------------------------------------
void msUsePostContrast( BYTE ucWinIndex, BOOL bUsePostContrast);

void msEnablePerContrast( BYTE ucWinIndex,BOOL benable);

void msEnablePostContrast( BYTE ucWinIndex,BOOL benable);

void msSetRGBContrast( BYTE ucWinIndex, WORD ucRCon, WORD ucGCon, WORD ucBCon);

void msAdjustRGBContrast(BYTE ucWinIndex);


#endif
