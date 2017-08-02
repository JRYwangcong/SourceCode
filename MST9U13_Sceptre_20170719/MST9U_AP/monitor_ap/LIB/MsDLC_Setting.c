/******************************************************************************
Copyright (c) 2003 MStar Semiconductor, Inc.
All rights reserved.
[Module Name]: MsDLC.c
[Date]:        26-Dec-2003
[Comment]:
MST DLC subroutines.
[Reversion History]:
*******************************************************************************/
#include "Board.h"
#include "datatype.h"
#include "MsDLC.h"
#include "ms_reg.h"
#include "MDebug.h"
#include "Common.h"
#include "Global.h"
#include "Adjust.h"
#include "Ms_rwreg.h"
#include <Global.h>
#include <math.h>
#include "MsDLC_Setting.h"
#include "appWindow.h"

//-------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------
#define DLC_SETTING_DEBUG 0

void msSetWinHistogramRange(void);
void msGetWinHistogramData(void);

DLC_MODE_T DlcMode;

XDATA BYTE DLC_Loop_Cnt;
XDATA BYTE DLC_Loop_Cnt_THD =20;// 64;

void msDlcInit(void)//( WORD wWidth, WORD wHeight )
{
   msSetWinHistogramRange();
}

void setWinDlcOnOff(BOOL En, BYTE u8Mode )
{
    BYTE u8count;
    for(u8count = 0 ; u8count < DISPLAY_MAX_NUMS ; u8count++)
    {
    	if (msAPIWinGetScDwEnable(u8count))
    	{
       	    msDlcOnOff(u8count, En, u8Mode);
    	}
    }
}

void msDlcOnOff(BYTE u8WinIdx, Bool bSwitch, BYTE u8MDLCode)
{
    if(bSwitch)
    {
        msDlcInit();
        g_bDLC_On = 1;
        msAPI_SetDLCEnable(u8WinIdx, _ENABLE);
        DlcMode = (DLC_MODE_T)u8MDLCode; //DLC_STATIC;
    }
    else
    {
        msAPI_SetDLCEnable(u8WinIdx, _DISABLE);
        g_bDLC_On = 0;
    }
}


void msHistogramHandler(BYTE u8WinIdx)
{
    //if(HISTOGRAM_READY_FLAG)
    {
        //CLR_HISTOGRAM_READY_FLAG(); // TBD
        msAPI_HistogramReportEnable(u8WinIdx);
    }

    //if ACK, get histogram data
    if(msAPI_HistogramCheckACK(u8WinIdx))
    {
        msAPI_HistogramUpDateData(u8WinIdx);
        //SET_HISTOGRAM_READY_FLAG();
    }
}

void msGetWinHistogramData(void)
{
   BYTE u8WinIdx;
    for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
    {
        if(msAPIWinGetScDwEnable(u8WinIdx))
            msHistogramHandler(u8WinIdx);
    }
}

void msSetWinHistogramRange(void)
{
    BYTE u8WinIdx;
    for(u8WinIdx = 0 ; u8WinIdx < DISPLAY_MAX_NUMS ; u8WinIdx++)
    {
        if(msAPIWinGetScDwEnable(u8WinIdx))
            msAPI_AutoSetHistogramRange(u8WinIdx);
    }
}

void msSetDLCCurve(BYTE u8WinIndex)
{
    unsigned long long TempVar1;
    int i;

        if (DLC_Loop_Cnt==0)
    {
        for( i = 0; i < 16; i++ )
        {
            TempVar1 = g_ucPreLumaCurve[u8WinIndex][i];
            if (g_ucLumaCurve[i]>TempVar1)
            {
                g_ucLumaCurve[i] = TempVar1+1;
            }
            else if (g_ucLumaCurve[i]<TempVar1)
            {
                g_ucLumaCurve[i] = TempVar1-1 ;
            }
        }

            msAPI_SetDLCCurve(u8WinIndex, &g_ucLumaCurve[0]);

        for( i = 0; i < 16; i++ )
            g_ucPreLumaCurve[u8WinIndex][i] = g_ucLumaCurve[i];
    }

        if (DLC_Loop_Cnt == DLC_Loop_Cnt_THD)
    {
            DLC_Loop_Cnt = 0;
    }
        else
    {
            DLC_Loop_Cnt++;
}

}

void msDLCProcessHandler(BYTE u8WinInx)
{
    msDLCAlgorithm(u8WinInx,DlcMode);
    msSetDLCCurve(u8WinInx);
}

void msDlcHandler(void)
{
    BYTE u8Temp;

    if( !g_bDLC_On )
    return ;

    //msGetWinHistogramData();

    for(u8Temp = 0 ; u8Temp < DISPLAY_MAX_NUMS ; u8Temp++)
    {
        if(msAPIWinGetScDwEnable(u8Temp))
            msDLCProcessHandler(u8Temp);
    }


#if DLC_SETTING_DEBUG
    XDATA BYTE i,j;
    printData("\r\n ===== Histogram Start window(%d) ======", 0);
    for ( j = 0; j < DISPLAY_MAX_NUMS; j++ )
        {
            printData("\r\n <<<<<<(%d)>>>>>> ", j);
            for ( i = 0; i < 32; i++ )
                printData("\r\n  %4x", u32Histogram[j][i]);
        }
    printMsg("\r\n ===== Histogram End ======");
#endif

}

