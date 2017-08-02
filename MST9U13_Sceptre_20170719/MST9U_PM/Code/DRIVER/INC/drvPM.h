#ifndef DRVPM_H
#define DRVPM_H
#include "board.h"
#if CHIP_ID == MST9U||CHIP_ID == MST9U2
#include "MST9U_DEMO_PMSET.h"
#include "drvPM_MST9U.h"
#elif CHIP_ID == MST9U3
#include "MST9U3_DEMO_PMSET.h"
#include "drvPM_MST9U3.h"

#else
#message "please implement DrvPM.h for new chip"
#endif

#if 0
BYTE code msDACNullData[] = {0};
void msDACDummy(void)
{
    BYTE xdata i = msDACNullData[0];
}
#endif
#endif
