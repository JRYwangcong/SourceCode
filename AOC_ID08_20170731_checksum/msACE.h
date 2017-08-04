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
    // sRGB matrix for PC
    _MSACEDEC_ XDATA short *tSrgbMatrix;
    // Color correction matrix for Video
    _MSACEDEC_ XDATA short *tColorCorrectionMatrix;
    //for tool
    extern XDATA short g_sCommandMatrix[3][3];
	#ifdef TSUMXXT		//110311 Modify
	extern XDATA BYTE g_bAccOnOff;
	#else
    extern bit g_bAccOnOff;
	#endif
    ///////////////////////////////////////////////////////////////////////////////
    // Basic color control function(Library)
    ///////////////////////////////////////////////////////////////////////////////



// ===Move to MSACE.h======================>>
#define SDTV  0   
#define HDTV  1   

//msSetColorFomat( SDTV );
 
 /*
   If input is SDTV (ex. AV/SV/TV/480i/480p/476i/576p),
   call  msSetColorFomat( SDTV );
   before any color matrix change (ex. Con., Sat., Hue, RGB-Gain adjustment)
 
   //If input is HDTV (ex. 720p/1080i/1080p),
   call  msSetColorFomat( HDTV );
   before any color matrix change (ex. Con., Sat., Hue, RGB-Gain adjustment)
*/
 
//<<=====================================

	extern void msSetColorFomat(bit ColorFormat);  
    // Must call this function before using other function
    void InitACEVar();
    #define NO_CHANGE_SHARPNESS 0xFF
    #define NO_CHANGE_STEP      0x80
    void InitialPeaking(void);
    void msAdjustSharpness(BYTE ucSharpness, BYTE ucStep); // 2004/12/10
    void msAdjustSubBrightness(BYTE brightness);
    void msAdjustVideoContrast(BYTE ucContrast); // 2004/12/10
    void msAdjustVideoSaturation(BYTE ucSaturation); // 2004/12/10
    void msAdjustVideoHue(BYTE ucHue); // 2004/12/10
    void msAdjustHSC(BYTE ucHue, BYTE ucSaturation, BYTE ucContrast); // 2004/12/10
    ///////////////////////////////////////////////////////////////////////////////
    // Special color control function
    ///////////////////////////////////////////////////////////////////////////////
    //void msAdjustVideoRGB( BYTE bScalerWin, BYTE ucRCon, BYTE ucGCon, BYTE ucBCon);
    //void msAdjustPCContrast( BYTE bScalerWin, BYTE ucContrast );
    //void msAdjustPCRGB( BYTE bScalerWin, BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
    //extern void msAdjustPCContrastRGB( BYTE bScalerWin, BYTE ucContrast, BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
    //#define ADJUST_PC_CONTRAST_RGB(bScalerWin)  msAdjustPCContrastRGB( bScalerWin, g_ColorSetting.Contrast, g_ColorTempSetting.RedColor, g_ColorTempSetting.GreenColor, g_ColorTempSetting.BlueColor )
    //extern void msACEPCsRGBCtl( BYTE bEnable );
    //extern void msACEPCYUV2RGBCtl( BYTE bEnable );
    ///////////////////////////////////////////////////////////////////////////////
    // ACC
    ///////////////////////////////////////////////////////////////////////////////
    void msAccSetup(WORD hstart_point, WORD hend_point, WORD vstart_point, WORD vend_point);
    void msAccOnOff(BYTE on);
    void LoadACETable(BYTE *pColorTable, BYTE ucHue, BYTE ucSaturation, BYTE ucSubContrast); // 2004/11/2
	void msAdjustVideoRGB( BYTE ucRCon, BYTE ucGCon, BYTE ucBCon);		//110311 Modify
#ifdef TSUMXXT//( MSACE_CHIP_ID == MSACE_CHIP_TSUMT )		//110311 Modify
	extern void msAdjustPCContrast( BYTE bScalerWin, BYTE ucContrast );
	extern void msAdjustPCRGB( BYTE bScalerWin, BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
	extern void msAdjustPCContrastRGB( BYTE bScalerWin, BYTE ucContrast, BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
	extern void msAdcCscFunctionCtrl( Bool bCtrl, Bool bWindow );
	extern void msACESetHDTVMode(BYTE isHDTV);	
#define ADJUST_PC_CONTRAST_RGB(bScalerWin)	msAdjustPCContrastRGB( bScalerWin, g_ColorSetting.Contrast, g_ColorTempSetting.RedColor, g_ColorTempSetting.GreenColor, g_ColorTempSetting.BlueColor )
#endif	// end of #if( MSACE_CHIP_ID == MSACE_CHIP_TSUMT )
#endif
