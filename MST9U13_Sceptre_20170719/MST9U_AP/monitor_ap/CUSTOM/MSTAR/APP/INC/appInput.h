#ifndef _APPINPUT_H_
#define _APPINPUT_H_

#ifdef _APPINPUT_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif

#define DFT_HPD_PERIOD 500 // set reasonable value, e.g. >= 300ms

INTERFACE void APPInput_Init( void );
INTERFACE void APPInput_DetectSyncStableCntInit( void );

#undef INTERFACE
#endif



