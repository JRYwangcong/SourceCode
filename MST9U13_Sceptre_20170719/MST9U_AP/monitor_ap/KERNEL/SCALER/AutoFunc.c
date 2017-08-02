#define _AUTOFUNC_C_
#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "ms_reg.h"
#include "Adjust.h"
//#include "msADC.h"
#include "Ms_rwreg.h"
#include "misc.h"
#include "MDebug.h"
#include "Common.h"
#include "Power.h"
//#include "Panel.h"

// INTERNAL
#include "AutoFunc.h"
#include "drvADC.h"
#include "mStar.h"

//#include "EepromMap.h"
/////////////////////////////////////
//#include "drvmStar.h"
//#include "halRwreg.h"
////////////////////////////////////
#define AUTOFUNC_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && AUTOFUNC_DEBUG
#define AutoFunc_printData(str, value)   printData(str, value)
#define AutoFunc_printMsg(str)           printMsg(str)
#else
#define AutoFunc_printData(str, value)
#define AutoFunc_printMsg(str)
#endif
extern WORD GetScale100Value(WORD u16Value, WORD u16MinValue, WORD u16MaxValue);

#define UseWorstPhase       0
#define EvenClock           1       //110929 Rick enable evenclock function - B029

#define AUTO_FUNC_TOLERANCE_WIDTH(InputPort)   (StandardModeWidth(InputPort)*6/100)
#define AUTO_FUNC_TOLERANCE_HEIGHT(InputPort)  ((StandardModeHeight(InputPort)*16/100)<80 ? (StandardModeHeight(InputPort)*16/100) : 80)
#define AUTO_FUNC_TOLERANCE_HTOTAL(InputPort)  ((StandardModeHTotal(InputPort)*6/100)<100 ? (StandardModeHTotal(InputPort)*6/100) :100)
////////////////////////////////////
// Code
void WaitAutoStatusReady( DWORD u16RegAddr, BYTE regFlag, BYTE InputPort )
{
    WORD waits = 0xFFFF;

    if(msDrvIP1IsDEOnlyMode(MapPort2DetScIndex(InputPort)) == TRUE)
    {
        while( waits-- && (!( msReadByte( u16RegAddr ) &regFlag )) && !SyncLossState(InputPort));
    }
    else
    {
        while( waits-- && (!( msReadByte( u16RegAddr ) &regFlag ) || ( msReadByte( u16RegAddr )&BIT3 )) && !SyncLossState(InputPort));
    }
}
WORD GetAutoValue(DWORD u32RegLoAddr, BYTE InputPort)
{
    msDrvIP1AutoPositionResultReady(MapPort2DetScIndex(InputPort), InputPort);//WaitAutoStatusReady(SC0_7B, BIT1);

    return msRead2Byte(u32RegLoAddr);
}
#if ENABLE_VGA_INPUT
Bool CheckSyncLoss( BYTE InputPort )
{
    //DWORD u32SCRegBase = g_ScalerInfo[InputPortDetSC(InputPort)].u32SiRegBase;
#if UseINT
    if( InputTimingChangeFlag(InputPort) )
    {
        Power_TurnOffPanel();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    Bool result = FALSE;
    BYTE u8DetSCIndex = MapPort2DetScIndex(InputPort);

    if( labs( SrcHPeriod(InputPort) - msDrvIP1ReadHperiod(u8DetSCIndex) ) > 10 )//( abs( SrcHPeriod - _msRead2Byte(SC0_E4) ) > 10 )
    {
        result = TRUE;
    }
    else if( labs( SrcVTotal(InputPort) - msDrvIP1ReadVtotal(u8DetSCIndex) ) > 10 )//( abs( SrcVTotal - _msRead2Byte(SC0_E2) ) > 10 )
    {
        result = TRUE;
    }
    if( result )
    {
        Power_TurnOffPanel();
        Set_InputTimingChangeFlag(InputPort);
    }
    return result;
#endif
}
BYTE GetTranstionPosition( BYTE vsyncTime, DWORD u32RegLoAddr, BYTE InputPort )
{
    BYTE retry = 6;
    BYTE adjPhase;
    WORD pos;
    BYTE minPhase, maxPhase;

    maxPhase = MAX_PHASE_VALUE;
    minPhase = 0x00;
    drvADC_SetPhaseCode( 0 );
    Delay1ms(vsyncTime<<1); //Jison 090514,u8VSyncTime*2 for TSUMP sometime can't get correct AOHEND_H.
    pos = GetAutoValue( u32RegLoAddr, InputPort );
    adjPhase = ( maxPhase + minPhase ) / 2;
    while( retry-- )
    {
        drvADC_SetPhaseCode( adjPhase );
        Delay1ms( vsyncTime );
        if( GetAutoValue( u32RegLoAddr, InputPort ) != pos )
        {
            maxPhase = adjPhase;
        }
        else
        {
            minPhase = adjPhase;
        }
        adjPhase = ( maxPhase + minPhase ) / 2;
        if( CheckSyncLoss(InputPort) )
        {
            return FALSE;
        }
    }

    return ( adjPhase );
}

#define QuickAuto           1// 0

#if !QuickAuto
BYTE AutoSetDataThreshold( BYTE vsyncTime, BYTE InputPort)
{
    BYTE thr;
    BYTE adjPhase;
    WORD regWord1, regWord2;
    BYTE u8DetSCIndex = MapPort2DetScIndex(InputPort);
    //DWORD u32SCRegBase = SCRegBase(u8DetSCIndex);

    for( thr = 4; thr < 0x10; thr++ )
    {
        msDrvIP1AutoPositionSetValidValue(u8DetSCIndex, thr);//_msWriteByte(SC0_7C, thr << 4 );
        drvADC_SetPhaseCode( 0 );
        Delay1ms( vsyncTime );
        msDrvIP1AutoPositionResultReady(u8DetSCIndex, InputPort);//WaitAutoStatusReady(SC0_7B, BIT1);
        regWord1 = msDrvIP1ReadAutoHStart(u8DetSCIndex);//_msRead2Byte(SC0_80);
        #if (MAX_PHASE_VALUE==0x7F)
        for( adjPhase = 7; adjPhase < (MAX_PHASE_VALUE+1); adjPhase += 7 )
        #else
        for( adjPhase = 4; adjPhase < (MAX_PHASE_VALUE+1); adjPhase += 4 )
        #endif
        {
            drvADC_SetPhaseCode( adjPhase );
            Delay1ms( vsyncTime );
            msDrvIP1AutoPositionResultReady(u8DetSCIndex, InputPort);//WaitAutoStatusReady(SC0_7B, BIT1);
            regWord2 = msDrvIP1ReadAutoHStart(u8DetSCIndex);//_msRead2Byte(SC0_80);
            if( labs( (DWORD)regWord1 - regWord2 ) > 3 )
                break;
            if( CheckSyncLoss(InputPort) )
                return FALSE;
        }
        if (adjPhase>=(MAX_PHASE_VALUE+1))
            break;
    }
    AutoFunc_printData("Auto_thr %d", thr);
    return thr;
}
#endif
WORD SearchMaxWidth( BYTE vsyncTime, BYTE InputPort )
{
    BYTE startPhase;
    WORD hStart, hEnd;
    BYTE ucReg;
    BYTE u8DetSCIndex = MapPort2DetScIndex(InputPort);

    ucReg = msDrvIP1AutoPositionReadValidValue(u8DetSCIndex);//_msReadByte(SC0_7C);
    msDrvIP1AutoPositionSetValidValue(u8DetSCIndex, AOVDV_VALUE);//_msWriteByte(SC0_7C, 0x40);
    drvADC_SetPhaseCode( PHASE_CENTER );
    Delay1ms( vsyncTime );
    Delay1ms( vsyncTime );
    hStart = msDrvIP1AutoPositionReadHstart(u8DetSCIndex, InputPort);//GetAutoValue(SC0_80);
    startPhase = msDrvIP1AutoPositionReadTranstionPosition(vsyncTime, u8DetSCIndex, InputPort);//GetTranstionPosition( vsyncTime, SC0_80 );
    hEnd = msDrvIP1AutoPositionReadHend(u8DetSCIndex, InputPort);//GetAutoValue(SC0_84);
    hStart = hEnd - hStart + 1;
    msDrvIP1AutoPositionSetValidValue(u8DetSCIndex, ucReg);//_msWriteByte(SC0_7C, ucReg);
    Delay1ms(vsyncTime<<1); //Jison 090423, u8VSyncTime*2 wait the status ready because of AOVDV change(Auto Htotal issue).

    AutoFunc_printData("Width %d", hStart);
    return hStart;
}

Bool mStar_AutoHTotal( BYTE vsyncTime, BYTE SCDetWin )
{
    WORD tempHTotal;
    WORD autoWidth;
    BOOL result;
    BYTE InputPort = MapScaler2Port(SCDetWin);


    // 1st to coast tune HTotal
    drvADC_SetPhaseCode( 0 );
    Delay1ms( vsyncTime );
    Delay1ms( vsyncTime );
    autoWidth = msDrvIP1AutoPositionReadHend(SCDetWin, InputPort) - msDrvIP1AutoPositionReadHstart(SCDetWin, InputPort);//GetAutoValue(SC0_84) - GetAutoValue(SC0_80);
    tempHTotal = StandardModeWidth(InputPort) / 3;
    if( labs( (DWORD)autoWidth - StandardModeWidth(InputPort) ) > tempHTotal )
    {
        //drvADC_SetPhaseCode( USER_PREF_PHASE(InputPort) );
        return FALSE;
    }
    tempHTotal = USER_PREF_H_TOTAL(InputPort);
    autoWidth = SearchMaxWidth((vsyncTime + 3) , InputPort);

    if( labs( (DWORD)autoWidth - StandardModeWidth(InputPort) ) > 1 )
    {
        tempHTotal = ( DWORD )USER_PREF_H_TOTAL(InputPort) * StandardModeWidth(InputPort) / autoWidth;
        if( labs( (DWORD)tempHTotal - StandardModeHTotal(InputPort) ) > AUTO_FUNC_TOLERANCE_HTOTAL(InputPort) )
        {
            goto AutoHTotalFailed;
        }
        //AutoFunc_printData("tempHTotal=%d",tempHTotal);
        drvADC_AdjustHTotal( tempHTotal );
        Delay1ms(vsyncTime);
    }

    autoWidth = SearchMaxWidth((vsyncTime+3), InputPort );
    if( autoWidth != StandardModeWidth(InputPort) )
    {
        tempHTotal = tempHTotal + ( StandardModeWidth(InputPort) - autoWidth );
        drvADC_AdjustHTotal( tempHTotal );
        Delay1ms(vsyncTime);
        autoWidth = SearchMaxWidth((vsyncTime+3), InputPort );
        tempHTotal = tempHTotal + ( StandardModeWidth(InputPort) - autoWidth );
    }
    //Check Htotal
    if( tempHTotal & 1 )
    {
        WORD autoWidth2;

        drvADC_AdjustHTotal( tempHTotal - 1 );
        autoWidth = SearchMaxWidth((vsyncTime+3), InputPort );
        drvADC_AdjustHTotal( tempHTotal + 1 );
        autoWidth2 = SearchMaxWidth((vsyncTime+3), InputPort );
        if( autoWidth2 == StandardModeWidth(InputPort) )
        {
            tempHTotal = tempHTotal + 1;
        }
        else if( autoWidth == StandardModeWidth(InputPort) )
        {
            tempHTotal = tempHTotal - 1;
        }
    }
#if EvenClock
    if (StandardModeHTotal(InputPort) % 2 == 0)
    {
        if( tempHTotal % 4 )
        {
            tempHTotal = (( tempHTotal + 1 ) / 4 ) * 4;
        }
    }
#endif
AutoHTotalFailed:
    result = FALSE;
    if( labs( (DWORD)tempHTotal - StandardModeHTotal(InputPort) ) <= AUTO_FUNC_TOLERANCE_HTOTAL(InputPort) )
    {
        USER_PREF_H_TOTAL(InputPort) = tempHTotal;
        result = TRUE;
    }

    AutoFunc_printData("AutoUserPrefHTotal %d", USER_PREF_H_TOTAL(InputPort));

    drvADC_AdjustHTotal( USER_PREF_H_TOTAL(InputPort) );
    drvADC_SetPhaseCode( USER_PREF_PHASE(InputPort) );
    return result;
#undef result
}

Bool mStar_AutoPhase( BYTE vsyncTime, BYTE SCDetWin )
{
    BYTE adjPhase, i;
#if UseWorstPhase
    BYTE worsePhase, worsePhase2;
    DWORD mincksum, cksum;
    BYTE InputPort = MapScaler2Port(SCDetWin);

    mincksum = 0xFFFFFFFFul;
    for( adjPhase = 0; adjPhase < 0x80; adjPhase += 7 )
    {
        drvADC_SetPhaseCode( adjPhase );
        Delay1ms( vsyncTime );
        msDrvIP1AutoPhaseResultReady(SCDetWin);//drvADC_WaitAutoStatusReady(SC0_8B, BIT1);
        //cksum = _msRead2Byte(SC0_8E);
        //cksum = ( cksum << 16 ) | _msRead2Byte(SC0_8C);
        cksum = msDrvIP1AutoPhaseReadValue(SCDetWin);
        if( cksum < mincksum )
        {
            mincksum = cksum;
            worsePhase = adjPhase;
        }

        if( CheckSyncLoss(InputPort) )
            return FALSE;
    }

    // 2nd Search
    if (worsePhase<7)
        worsePhase=(worsePhase+127)%0x80; // 128-7
    else
        worsePhase-=7;

    mincksum = 0xFFFFFFFFul;
    for (adjPhase=worsePhase; adjPhase<worsePhase+10; adjPhase++)
    {
        i=adjPhase%0x80;
        drvADC_SetPhaseCode( i );
        Delay1ms( 2 * vsyncTime );
        msDrvIP1AutoPhaseResultReady(SCDetWin);//drvADC_WaitAutoStatusReady(SC0_8B, BIT1);
        //cksum = _msRead2Byte(SC0_8E);
        //cksum = ( cksum << 16 ) | _msRead2Byte(SC0_8C);
        cksum = msDrvIP1AutoPhaseReadValue(SCDetWin);
        if( cksum < mincksum )
        {
            mincksum = cksum;
            worsePhase2 = i;
        }

        if( CheckSyncLoss(InputPort) )
            return FALSE;
    }
    USER_PREF_PHASE(InputPort) = ( worsePhase2 + 63 ) % 0x80;
#else
    BYTE bestPhase=0, bestPhase2=0;
    DWORD maxcksum, cksum;
    BYTE InputPort = MapScaler2Port(SCDetWin);

    msDrvIP1AutoPhaseSetNoiseMask(7, SCDetWin);

    maxcksum = 0;
    for( adjPhase = 0; adjPhase < 0x80; adjPhase += 7 )
    {
        drvADC_SetPhaseCode( adjPhase );
        Delay1ms( 2 * vsyncTime );
        msDrvIP1AutoPhaseResultReady(SCDetWin);//drvADC_WaitAutoStatusReady(SC0_8B, BIT1);
        //cksum = _msRead2Byte(SC0_8E);
        //cksum = ( cksum << 16 ) | _msRead2Byte(SC0_8C);
        cksum = msDrvIP1AutoPhaseReadValue(SCDetWin);
        if( cksum > maxcksum )
        {
            maxcksum = cksum;
            bestPhase = adjPhase;
        }

        if( CheckSyncLoss(InputPort) )
            return FALSE;
    }

    // 2nd Search
    if ( bestPhase < 7 )
        bestPhase = (bestPhase+121)%0x80; // 128-7
    else
        bestPhase -= 7;
    maxcksum = 0;
    for( adjPhase = bestPhase; adjPhase < bestPhase + 10; adjPhase++)
    {
        i=adjPhase%0x80;
        drvADC_SetPhaseCode( i );
        Delay1ms( vsyncTime );
        msDrvIP1AutoPhaseResultReady(SCDetWin);//drvADC_WaitAutoStatusReady(SC0_8B, BIT1);
        //cksum = _msRead2Byte(SC0_8E);
        //cksum = ( cksum << 16 ) | _msRead2Byte(SC0_8C);
        cksum = msDrvIP1AutoPhaseReadValue(SCDetWin);
        if( cksum > maxcksum )
        {
            maxcksum = cksum;
            bestPhase2 = i;
        }

        if( CheckSyncLoss(InputPort) )
            return FALSE;
    }

    USER_PREF_PHASE(InputPort) = bestPhase2;
    //printData("@@Autophase = 0x%x", bestPhase2);

    msDrvIP1AutoPhaseSetNoiseMask(0, SCDetWin);
#endif
    drvADC_SetPhaseCode( USER_PREF_PHASE(InputPort) );
    return TRUE;
}

Bool mStar_AutoPosition(BYTE u8DispWin)
{
    WORD height, width;
    WORD hStart, vStart;
    BYTE InputPort = MapWin2Port(u8DispWin);
    BYTE SCDetWin = MapPort2DetScIndex(InputPort);

    hStart = msDrvIP1AutoPositionReadHstart(SCDetWin, InputPort);//GetAutoValue(SC0_80);
    vStart = msDrvIP1AutoPositionReadVstart(SCDetWin, InputPort);//GetAutoValue(SC0_7E);

    if(vStart<3||hStart<0x10)
    {
        if(msDrvIP1ReadSyncStatus(SCDetWin)&(SOGD_B|SOGP_B))
        {
            height=4;
            for (; height<0x10; height++)
            {
                msDrvIP1AutoPositionSetValidValue(SCDetWin, height);
                Delay1ms(10);
                hStart = msDrvIP1AutoPositionReadHstart(SCDetWin, InputPort);//GetAutoValue(SC0_80);
                vStart = msDrvIP1AutoPositionReadVstart(SCDetWin, InputPort);//GetAutoValue(SC0_7E);

                AutoFunc_printData("H start: 0x%x",hStart);
                AutoFunc_printData("V start: 0x%x",vStart);

                if(vStart>0&&hStart>0x10)
                    break;
            }
        }

        if (vStart == 0 || hStart < 0x10)
            return FALSE;
    }

    AutoFunc_printData("End H start: 0x%x",hStart);
    AutoFunc_printData("End V start: 0x%x",vStart);

    if( hStart > StandardModeWidth(InputPort) || vStart > StandardModeHeight(InputPort) )
    {
        return FALSE;
    }

    width = msDrvIP1AutoPositionReadHend(SCDetWin, InputPort) - hStart + 1;//GetAutoValue(SC0_84) - hStart + 1;
    height = msDrvIP1AutoPositionReadVend(SCDetWin, InputPort) - vStart + 1;//GetAutoValue(SC0_82) - vStart + 1;

    AutoFunc_printData("H_1: 0x%x",width);
    AutoFunc_printData("V_1: 0x%x",height);

    if( UserModeFlag(InputPort) )
    {
        USER_PREF_V_START(InputPort) = vStart;  //Real Value
    }
    else if (height < (StandardModeHeight(InputPort)-AUTO_FUNC_TOLERANCE_HEIGHT(InputPort))
          || width < (StandardModeWidth(InputPort)-AUTO_FUNC_TOLERANCE_WIDTH(InputPort)) )
    {
        return FALSE;
    }
#if 1 //force SOG 640x350 as 720x400
    else if (SrcModeIndex(InputPort)==MD_720X400_70 && height==350 && (msDrvIP1GetInputSyncType(SCDetWin)&COMP_B))
    {
        USER_PREF_V_START(InputPort) = vStart-50/2; //Real Value
    }
#endif
    else
        USER_PREF_V_START(InputPort) = vStart; //Real Value

    USER_PREF_H_START(InputPort) = hStart;

#if ENABLE_DIGITAL_INPUT
    if (!INPUT_IS_VGA(InputPort))
        USER_PREF_H_START(InputPort) -= 0x10;
#endif

    USER_PREF_AUTO_HSTART(InputPort)=USER_PREF_H_START(InputPort);
    USER_PREF_AUTO_VSTART(InputPort)=USER_PREF_V_START(InputPort); //Real Value

    SrcAutoHstart(InputPort) = USER_PREF_H_START(InputPort);
    SrcAutoVstart(InputPort) = USER_PREF_V_START(InputPort);

    msAPI_AdjustHPosition(u8DispWin , USER_PREF_H_START(InputPort));
    msAPI_AdjustVPosition(u8DispWin , USER_PREF_V_START(InputPort));


#if YPBPR_PERF_OVERSCAN //the image need to ceter on the set when separate input.
    if ((IsOverScan(InputPort)) && (!(msDrvIP1ReadSyncStatus(SCDetWin)&SOGD_B)))
    {
        msAPI_AdjustHPosition(u8DispWin, (USER_PREF_H_START(InputPort)+(StandardModeWidth(InputPort)*3+50)/100/2));
        msAPI_AdjustVPosition(u8DispWin, (USER_PREF_V_START(InputPort)+(StandardModeHeight(InputPort)*3+50)/100/2);
    }
    else
#endif
    {
        msAPI_AdjustHPosition(u8DispWin , USER_PREF_H_START(InputPort));
        msAPI_AdjustVPosition(u8DispWin , USER_PREF_V_START(InputPort));
    }
    return TRUE;
}


#if 1
void CheckHtotal( BYTE u8InputPort )
{
    WORD u16ChkSumTmp, u16HTTemp, u16MaxChkSum, u16MaxChkSumTemp;
    BYTE u8TempValue1, u8TempValue2;
    BYTE i;
    BYTE u8VSyncTime = GetVSyncTime(u8InputPort);
    BYTE u8DetSCIndex = MapPort2DetScIndex(u8InputPort);

#if MAX_PHASE_VALUE==0x7F
#define PHASE_STEP 4
#else
#define PHASE_STEP 2
#endif

    if (USER_PREF_H_TOTAL(u8InputPort) != StandardModeHTotal(u8InputPort))
    {
        u8TempValue1 = msDrvIP1AutoPositionReadValidValue(u8DetSCIndex);//msReadByte(SC0_7C);
        msDrvIP1AutoPositionSetValidValue(u8DetSCIndex, AOVDV_VALUE/*0x20*/);//msWriteByte(SC0_7C, 0x20);
        u8TempValue2=mStar_ScalerDoubleBuffer(TRUE);

        u16ChkSumTmp = 0;
        u16MaxChkSum = 0;
        u16MaxChkSumTemp = 0;
        for (i = 0; i < MAX_PHASE_VALUE; i = i + PHASE_STEP)
        {
            drvADC_SetPhaseCode(i);
            Delay1ms(u8VSyncTime);
            u16ChkSumTmp = msDrvIP1AutoPhaseReadValue(u8DetSCIndex);//msRead2Byte(SC0_8E);
            if (u16ChkSumTmp > u16MaxChkSum)
                u16MaxChkSum = u16ChkSumTmp;
        }

        u16HTTemp = USER_PREF_H_TOTAL(u8InputPort);
        u16MaxChkSumTemp = u16MaxChkSum;

        drvADC_AdjustHTotal(USER_PREF_H_TOTAL(u8InputPort)-1);
        Delay1ms(u8VSyncTime);
        for (i = 0; i < MAX_PHASE_VALUE; i = i + PHASE_STEP)
        {
            drvADC_SetPhaseCode(i);
            Delay1ms(u8VSyncTime);
            u16ChkSumTmp = msDrvIP1AutoPhaseReadValue(u8DetSCIndex);
            if (u16ChkSumTmp > u16MaxChkSumTemp)
                u16MaxChkSumTemp = u16ChkSumTmp;
        }

        if (u16MaxChkSumTemp > u16MaxChkSum)
        {
            u16HTTemp = USER_PREF_H_TOTAL(u8InputPort)-1;
            u16MaxChkSum = u16MaxChkSumTemp;
        }
        drvADC_AdjustHTotal(USER_PREF_H_TOTAL(u8InputPort)-2);
        Delay1ms(u8VSyncTime);
        for (i = 0; i < MAX_PHASE_VALUE; i = i + PHASE_STEP)
        {
            drvADC_SetPhaseCode(i);
            Delay1ms(u8VSyncTime);
            u16ChkSumTmp = msDrvIP1AutoPhaseReadValue(u8DetSCIndex);
            if (u16ChkSumTmp > u16MaxChkSumTemp)
                u16MaxChkSumTemp = u16ChkSumTmp;
        }

        if (u16MaxChkSumTemp > u16MaxChkSum)
        {
            u16HTTemp = USER_PREF_H_TOTAL(u8InputPort)-2;
            u16MaxChkSum = u16MaxChkSumTemp;
        }

        drvADC_AdjustHTotal(USER_PREF_H_TOTAL(u8InputPort)-3);
        Delay1ms(u8VSyncTime);
        for (i = 0; i < MAX_PHASE_VALUE; i = i + PHASE_STEP)
        {
            drvADC_SetPhaseCode(i);
            Delay1ms(u8VSyncTime);
            u16ChkSumTmp = msDrvIP1AutoPhaseReadValue(u8DetSCIndex);
            if (u16ChkSumTmp > u16MaxChkSumTemp)
                u16MaxChkSumTemp = u16ChkSumTmp;
        }

        if (u16MaxChkSumTemp > u16MaxChkSum)
        {
            u16HTTemp = USER_PREF_H_TOTAL(u8InputPort)-3;
            u16MaxChkSum = u16MaxChkSumTemp;
        }

        drvADC_AdjustHTotal(StandardModeHTotal(u8InputPort));
        Delay1ms(u8VSyncTime);
        for (i = 0; i < MAX_PHASE_VALUE; i = i + PHASE_STEP)
        {
            drvADC_SetPhaseCode(i);
            Delay1ms(u8VSyncTime);
            u16ChkSumTmp = msDrvIP1AutoPhaseReadValue(u8DetSCIndex);
            if (u16ChkSumTmp > u16MaxChkSumTemp)
                u16MaxChkSumTemp = u16ChkSumTmp;
        }

        if (u16MaxChkSumTemp > u16MaxChkSum)
        {
            u16HTTemp = StandardModeHTotal(u8InputPort);
        }

        USER_PREF_H_TOTAL(u8InputPort) = u16HTTemp;
        drvADC_AdjustHTotal(USER_PREF_H_TOTAL(u8InputPort));

        msDrvIP1AutoPositionSetValidValue(u8DetSCIndex, u8TempValue1);//msWriteByte(SC0_7C, u8TempValue1);
        mStar_ScalerDoubleBuffer(u8TempValue2&BIT0);
        AutoFunc_printData("CheckHtotal used=%d", USER_PREF_H_TOTAL(u8InputPort));
    }
#undef PHASE_STEP
}
#endif

Bool mStar_AutoGeomtry(BYTE u8DispWin )
{
    Bool result = TRUE;
    BYTE vsyncTime;
    BYTE u8InputPort;
    BYTE u8SCDetWin;

    if(((g_DisplayWindow[u8DispWin].eDwStatus != WIN_STATUS_OUTPUT_DISPLAYOK) /*&& (USER_PREF_AUTOTIME(Input_Analog1)&AUTO_TIMES_BIT)*/)
      ||(g_DisplayWindow[u8DispWin].u8DwPortIndex != Input_Analog1) )
    {
        return FALSE;
    }

    u8InputPort = g_DisplayWindow[u8DispWin].u8DwPortIndex;
    u8SCDetWin = MapPort2DetScIndex(u8InputPort);

    vsyncTime = GetVSyncTime(u8InputPort);

    if(INPUT_IS_VGA(u8InputPort))
    {
        mStar_ScalerDoubleBuffer(FALSE);
        // auto adjust threshold
    #if QuickAuto
        msDrvIP1AutoPositionSetValidValue(u8SCDetWin, AOVDV_VALUE);//_msWriteByte( BK0_7C, 0x40 );
    #else
        AutoSetDataThreshold( vsyncTime * 2, u8InputPort );
    #endif
        if( InputTimingChangeFlag(u8InputPort) )
        {
            result = FALSE;
            goto AutoGeomtryFailed;
        }
        // auto adjust htotal
        result = mStar_AutoHTotal( vsyncTime * 2, u8SCDetWin );

        //if( !result )
        //    goto AutoGeomtryFailed;

        if( InputTimingChangeFlag(u8InputPort) )
        {
            result = FALSE;
            goto AutoGeomtryFailed;
        }
        // auto adjust phase
        if( result )
        {
            drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN, USER_PREF_ADC_GREEN_GAIN, USER_PREF_ADC_BLUE_GAIN);
            CheckHtotal(u8InputPort);
            mStar_AutoPhase( vsyncTime * 2, u8SCDetWin );
        }
        else
        {
            USER_PREF_H_TOTAL(u8InputPort) = StandardModeHTotal(u8InputPort);
            drvADC_AdjustHTotal( USER_PREF_H_TOTAL(u8InputPort) );
        #if YPBPR_PERF_OVERSCAN //Force to do Auto Phase
            if (IsOverScan(u8InputPort))
                mStar_AutoPhase( vsyncTime * 2, u8SCDetWin );
        #endif
        }

        if( InputTimingChangeFlag(u8InputPort) )
        {
            result = FALSE;
            goto AutoGeomtryFailed;
        }

        Delay1ms(vsyncTime * 4);
    #if YPBPR_PERF_OVERSCAN
        if ((IsOverScan(u8InputPort)) && (msDrvIP1ReadSyncStatus(u8SCDetWin)&SOGD_B))
        //the image need to ceter on the set when separate input.
        {
            result=FALSE;
        }
        else
    #endif
        result = mStar_AutoPosition(u8DispWin);
        AutoFunc_printData("mStar_AutoPosition %d",result);

        if (!result) //Auto Position Failed
        {
        #if YPBPR_PERF_OVERSCAN
            if (IsOverScan(u8InputPort))
            {
                //OSD showing 50%
                USER_PREF_AUTO_HSTART(u8InputPort) = StandardModeHStart(u8InputPort);
                USER_PREF_AUTO_VSTART(u8InputPort) = StandardModeVStart(u8InputPort);
                USER_PREF_H_START(u8InputPort) = StandardModeHStart(u8InputPort);
                USER_PREF_V_START(u8InputPort)=USER_PREF_V_START(u8InputPort);
                result=TRUE;//Save Video Mode Data

                //Set H.V. Position
                msAPI_AdjustHPosition(u8DispWin, (USER_PREF_H_START(u8InputPort)+(StandardModeWidth(u8InputPort)*3+50)/100/2));
                msAPI_AdjustVPosition(u8DispWin, (USER_PREF_V_START(u8InputPort)+(StandardModeHeight(u8InputPort)*3+50)/100/2);
            }
            else
        #endif
            {
                msAPI_AdjustHPosition(u8DispWin, USER_PREF_H_START(u8InputPort));
                //AutoFunc_printData("\n\USER_PREF_AUTO_VSTART=%x",USER_PREF_AUTO_VSTART(u8InputPort));
                //AutoFunc_printData("StandardModeVStart=%x\n\r",StandardModeVStart(u8InputPort));
                //AutoFunc_printData("USER_PREF_V_START=%x\n\r",USER_PREF_V_START(u8InputPort));
                msAPI_AdjustVPosition(u8DispWin, USER_PREF_V_START(u8InputPort));
            }
        }
    }
AutoGeomtryFailed:
    msDrvIP1AutoPositionSetValidValue(u8SCDetWin, AOVDV_VALUE);//_msWriteByte(SC0_7C, AOVDV_VALUE);
    mStar_ScalerDoubleBuffer(TRUE);

    if(USER_PREF_INPUT_COLOR_FORMAT(0)==INPUT_COLOR_RGB)
    {
        mStar_YUVAutoOffset(0);
    }
    if(USER_PREF_INPUT_COLOR_FORMAT(0)==INPUT_COLOR_YPBPR)
    {
        mStar_YUVAutoOffset(0x1C);
    }

    return result;
}

Bool mStar_AutoColor( BYTE InputPort )
{
    Bool result;
    BYTE vsyncTime;
    vsyncTime = GetVSyncTime(InputPort);
    if(vsyncTime<(255/3))
        vsyncTime*=3;
    else
        vsyncTime=255;
    result = drvADC_AutoAdcColor(vsyncTime, InputPort);
    if( InputTimingChangeFlag(InputPort) )
        return FALSE;

    return result;
}

#define GCR 235
#define GCG 250
#define GCB 235
void mStar_YUVAutoOffset(BYTE u8Value)
{
    if(!u8Value)
    {
        drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN, USER_PREF_ADC_GREEN_GAIN, USER_PREF_ADC_BLUE_GAIN);
    }
    else
    {
        drvADC_SetRGBGainCode(USER_PREF_ADC_RED_GAIN-GCR, USER_PREF_ADC_GREEN_GAIN-GCG, USER_PREF_ADC_BLUE_GAIN-GCB);
    }
}
#endif
