#ifndef _APPADJUST_H_
#define _APPADJUST_H_

#ifdef _APPADJUST_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif


//Calculate Real Value function=========================================
INTERFACE WORD GetRealBlackLevel(WORD u16OsdValue);
INTERFACE WORD GetRealRgbGain(WORD u16OsdValue);
INTERFACE WORD GetRealPcContrast(WORD u16OsdValue);
INTERFACE WORD GetRealVideoContrast(WORD u16OsdValue);
INTERFACE WORD GetRealSaturation(WORD u16OsdValue);
//======================================================================


INTERFACE WORD APPAdjustBrightness(BYTE u8Brightness);
#if AudioFunc
//INTERFACE void APPAdjustVolume(BYTE u8Volume);
INTERFACE WORD GetRealVolume(WORD u16OsdValue);

#endif

#undef INTERFACE
#endif


