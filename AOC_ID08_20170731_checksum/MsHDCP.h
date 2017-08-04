/******************************************************************************
Copyright (c) 2004 MStar Semiconductor, Inc.
All rights reserved.
[Module Name]: MsHDCP.h
[Date]:        28-Jan-2004
[Comment]:
HDCP header file.
[Reversion History]:
 *******************************************************************************/
#ifndef _MSHDCP_H_
    #define _MSHDCP_H_
    //#include "board.h"
    #if ENABLE_HDCP
        extern void msInitHDCPKeySelectorVector(void);
        extern void msInitHDCPProductionKey(void);
        extern void msEnableHDCP(void);
        extern void msDisableHDCP(void);
        void msPullLowHPD1(void);
        void msPullHighHPD1(void);
        void msPullLowHPD2(void);
        void msPullHighHPD2(void);
    #endif // #if ENABLE_HDCP
#endif
