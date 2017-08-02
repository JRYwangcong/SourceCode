#ifndef _APIDDC2BI_H_
#define _APIDDC2BI_H_

#ifdef _APIDDC2BI_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif


INTERFACE void msAPI_DDC2BiEnable(BYTE u8InputPort, BOOL bEnable);

#undef INTERFACE
#endif




