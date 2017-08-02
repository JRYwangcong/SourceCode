
#include "Global.h"

void appmStar_SetPanelTiming(BYTE SCDetWin)
{
    msDrvSetTimingGen(SCDetWin);
    msDrvIP1SetOutputLockMode(FALSE, SCDetWin);//_msWriteByteMask( SC0_02, BIT7, BIT7 ); // disable output Lock mode to enable free run.
    //msDrvIP1SetOutputFreerun(TRUE,SCDetWin);
    if(g_ScalerInfo[SCDetWin].eSiDispStatus == SC_STATUS_IDLE)
    {
        msDrvScMemMRQ(SCDetWin, TRUE);
    }
}

