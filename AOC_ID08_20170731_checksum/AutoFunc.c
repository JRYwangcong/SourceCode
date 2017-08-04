#include <math.h>
#include "types.h"
#include "board.h"
#include "global.h"
#include "ms_reg.h"
#include "adjust.h"
#include "ms_rwreg.h"
#include "misc.h"
#include "debug.h"
#include "power.h"
#include "panel.h"

#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
#include "MsADC.h"
#endif
//#include "Menudef.h"
#define UseWorstPhase   0
#define EvenClock   1
#define QuickAuto   0
#define OverFlow    BIT7
#define NoneOverFlow    BIT6
#define UnderFlow   BIT7
#define NoneUnderFlow   BIT6
//===============================================================
// local declaration
Bool mStar_AutoAdcColor(BYTE vsyncTime);
void CheckDosTextHtotal(BYTE vsyncTime);
//===============================================================
// Code
Bool CheckSyncLoss(void)
{
    #if UseINT
        if (InputTimingChangeFlag)
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
        if (abs(SrcHPeriod - mStar_ReadWord(HSPRD_H)) > 10)
        {
            result = TRUE;
        }
        else if (abs(SrcVTotal - mStar_ReadWord(VTOTAL_H)) > 10)
        {
            result = TRUE;
        }
        if (result)
        {
            Power_TurnOffPanel();
            Set_InputTimingChangeFlag();
        }
        return result;
    #endif 
}
void WaitAutoStatusReady(BYTE regAddr, BYTE regFlag)
{
    #if Enable_Cache   // jeff 12 22 for cache add it 
      BYTE waits = 2200;
	#else
    BYTE waits = 200;
	#endif
    while (waits-- && !(mStar_ReadByte(regAddr) &regFlag))
        ;
}
WORD GetAutoValue(BYTE regAddr)
{
    WaitAutoStatusReady(ATOCTRL, ATOR_B);
    return mStar_ReadWord(regAddr);
}
BYTE GetTranstionPosition(BYTE vsyncTime, BYTE regAddr)
{
    BYTE retry = 6;
    BYTE adjPhase;
    WORD pos;
    BYTE minPhase, maxPhase;
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
	maxPhase=0x7F;
	minPhase=0x0;
	msADC_SetPhaseCode(0);
	#else
    maxPhase = 0x3F;
    minPhase = 0x0;
    mStar_AdjustPhase(0);
	#endif
    Delay1ms(vsyncTime);
    pos = GetAutoValue(regAddr);
    adjPhase = (maxPhase + minPhase) / 2;
    while (retry--)
    {
		#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
		msADC_SetPhaseCode(adjPhase);
		#else
        mStar_AdjustPhase(adjPhase);
		#endif
        Delay1ms(vsyncTime);
        if (GetAutoValue(regAddr) != pos)
        {
            maxPhase = adjPhase;
        }
        else
        {
            minPhase = adjPhase;
        }
        adjPhase = (maxPhase + minPhase) / 2;
        if (CheckSyncLoss())
        {
            return FALSE;
        }
    }
    //  printData("trans phase %d", adjPhase);
    return (adjPhase);
}
#if !QuickAuto
    BYTE AutoSetDataThreshold(BYTE vsyncTime)
    {
        BYTE thr;
        BYTE adjPhase;
        WORD regWord1, regWord2;
        for (thr = 3/*4*/; thr < 0x10; thr++)
        {
            mStar_WriteByte(AOVDV, thr << 4);
			#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
			msADC_SetPhaseCode(0);
			#else
            mStar_AdjustPhase(0);
			#endif
            Delay1ms(vsyncTime);
            //      WaitAutoStatusReady(ATOCTRL, ATOR_B);       // 20050223 wumaozhong
            regWord1 = GetAutoValue(AOHST_H);
			#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
			for (adjPhase = 7; adjPhase < 0x80; adjPhase += 7)
			#else
            for (adjPhase = 4; adjPhase < 0x40; adjPhase += 4)
			#endif
            {
				#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
				msADC_SetPhaseCode(adjPhase);
				#else
                mStar_AdjustPhase(adjPhase);
				#endif
                Delay1ms(vsyncTime);
                //          WaitAutoStatusReady(ATOCTRL, ATOR_B);       // 20050223 wumaozhong          
                regWord2 = GetAutoValue(AOHST_H);
                if (abs(regWord1 - regWord2) > 3)
                {
                    break;
                }
                if (CheckSyncLoss())
                {
                    return FALSE;
                }
            }
			#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
			if (adjPhase >= 0x80)
			#else
            if (adjPhase >= 0x40)
			#endif
            {
                break;
            }
        }
		#ifdef TSUMXXT		//110311 Modify
		#else
        thr = 5;
        mStar_WriteByte(AOVDV, thr << 4);
		#endif
        return thr;
    }
#endif 
WORD SearchMaxWidth(BYTE vsyncTime)
{
    BYTE startPhase;
    WORD hStart, hEnd;
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
	msADC_SetPhaseCode(0);
	#else
    mStar_AdjustPhase(0);
	#endif
    Delay1ms(vsyncTime);
    Delay1ms(vsyncTime);
    hStart = GetAutoValue(AOHST_H);
    //  printData("hStart %d", hStart);
    startPhase = GetTranstionPosition(vsyncTime, AOHST_H);
    hEnd = GetAutoValue(AOHEND_H);
    //  printData("hEnd %d", hEnd);
    hStart = hEnd - hStart + 1;
    return hStart;
}

#if 1
    Bool mStar_AutoHTotal(BYTE vsyncTime)
    {
        WORD tempHTotal;
        WORD autoWidth;
        BYTE result;

        // 100512 coding addition
        if(InputTimingChangeFlag)
            return FALSE;
        
        // 1st to coast tune HTotal
		#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
		msADC_SetPhaseCode(0);
		#else
        mStar_AdjustPhase(0);
		#endif
        Delay1ms(vsyncTime);
        autoWidth = GetAutoValue(AOHEND_H) - GetAutoValue(AOHST_H);
        tempHTotal = StandardModeWidth / 3;
	#if DEBUG_PRINTDATA
	printData("1st AOHEND_H %d", GetAutoValue(AOHEND_H));
	printData("1st AOHST_H   %d", GetAutoValue(AOHST_H));
	 printData("1st max width %d", autoWidth);
        printData("1st StandardModeWidth %d", StandardModeWidth);
        printData("1st htotal %d", tempHTotal);
	#endif
        if (abs(autoWidth - StandardModeWidth) > tempHTotal)
        {
			#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
			msADC_SetPhaseCode(UserPrefPhase);
			#else
            mStar_AdjustPhase(UserPrefPhase); //add 20050613 wumaozhong for DOS Auto wring
            #endif
            return FALSE;
        }
        tempHTotal = UserPrefHTotal;

        if (abs(autoWidth - StandardModeWidth) > 1)
        {
            tempHTotal = (DWORD)UserPrefHTotal *StandardModeWidth / autoWidth;
            if (abs(tempHTotal - StandardModeHTotal) > ClockAdjRange)
            {
                goto AutoHTotalFailed;
            }
			#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
			msADC_AdjustHTotal(tempHTotal);
			#else
            mStar_AdjustHTotal(tempHTotal);
			#endif
            //printData("2nd htotal %d", tempHTotal);
        }
        autoWidth = SearchMaxWidth(vsyncTime);
        //printData("2nd max width %d", autoWidth);
        // autoWidth=SearchMaxWidth(vsyncTime);
        // 2nd fine tune
        if (autoWidth != StandardModeWidth)
        {
            tempHTotal = tempHTotal + (StandardModeWidth - autoWidth);
            if (abs(tempHTotal - StandardModeHTotal) > ClockAdjRange)	//110311 Modify
            {
                goto AutoHTotalFailed;
            }
			#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
			msADC_AdjustHTotal(tempHTotal);
			#else
            mStar_AdjustHTotal(tempHTotal);
			#endif
            autoWidth = SearchMaxWidth(vsyncTime);
            // printData("3rd htotal %d", tempHTotal);
            // printData("max width %d", autoWidth);
            tempHTotal = tempHTotal + (StandardModeWidth - autoWidth);
        }
        else if (tempHTotal &1)
        {
            WORD autoWidth2;
            //tempHTotal=tempHTotal-1;
			#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
			msADC_AdjustHTotal(tempHTotal - 1);
			#else
            mStar_AdjustHTotal(tempHTotal - 1);
			#endif
            autoWidth = SearchMaxWidth(vsyncTime);
			#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
			msADC_AdjustHTotal(tempHTotal + 1);
			#else
            mStar_AdjustHTotal(tempHTotal + 1);
			#endif
            autoWidth2 = SearchMaxWidth(vsyncTime);
            if (autoWidth2 == StandardModeWidth)
            {
                tempHTotal = tempHTotal + 1;
            }
            else if (autoWidth == StandardModeWidth)
            {
                tempHTotal = tempHTotal - 1;
            }
            //printData("4th htotal %d", tempHTotal);
            //printData("4th max width %d", autoWidth);
            //printData("4th max width2 %d", autoWidth2);
            #if 0
                if (autoWidth != StandardModeWidth)
                    tempHTotal = tempHTotal + (StandardModeWidth - autoWidth);
            #endif 
        }
       #if EvenClock
	   #if 1//20150901 lixw 
	if(SrcModeIndex==PRESET_1280x1024_70Hz)
		;
	else
	   #endif
	 {
            if (tempHTotal % 4)
            {
                tempHTotal = ((tempHTotal + 1) / 4) *4;
            }
	 }
        #endif 
        AutoHTotalFailed: result = FALSE;
        if (abs(tempHTotal - StandardModeHTotal) < 60)
        {
            UserPrefHTotal = tempHTotal;
            result = TRUE;
        }
		#if DEBUG_PRINTDATA
	printData("End00-- UserPrefHTotal %d", UserPrefHTotal);
		#endif
		#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		msADC_AdjustHTotal(UserPrefHTotal);
		msADC_SetPhaseCode(UserPrefPhase);
		#else
        mStar_AdjustHTotal(UserPrefHTotal);
        mStar_AdjustPhase(UserPrefPhase);
		#endif
        return result;
        #undef result
    }
#else 
    Bool mStar_AutoHTotal(BYTE vsyncTime)
    {
        WORD tempHTotal;
        WORD autoWidth;
        BYTE result;
        // 1st to coast tune HTotal
        mStar_AdjustPhase(0);
        Delay1ms(vsyncTime);
        Delay1ms(vsyncTime);
        autoWidth = GetAutoValue(AOHEND_H) - GetAutoValue(AOHST_H);
        tempHTotal = UserPrefHTotal;
        //  autoWidth=SearchMaxWidth(vsyncTime);
        if (abs(autoWidth - StandardModeWidth) > 1)
        {
            tempHTotal = (DWORD)UserPrefHTotal *StandardModeWidth / autoWidth;
            if (abs(tempHTotal - StandardModeHTotal) > 100)
            //80) //60)
            {
                goto AutoHTotalFailed;
            }
            mStar_AdjustHTotal(tempHTotal);
        }
        autoWidth = SearchMaxWidth(vsyncTime);
        // autoWidth=SearchMaxWidth(vsyncTime);
        // 2nd fine tune
        if (autoWidth != StandardModeWidth)
        {
            tempHTotal = tempHTotal + (StandardModeWidth - autoWidth);
            mStar_AdjustHTotal(tempHTotal);
            autoWidth = SearchMaxWidth(vsyncTime);
            tempHTotal = tempHTotal + (StandardModeWidth - autoWidth);
        }
        else if (tempHTotal &1)
        {
            WORD autoWidth2;
            //tempHTotal=tempHTotal-1;
            mStar_AdjustHTotal(tempHTotal - 1);
            autoWidth = SearchMaxWidth(vsyncTime);
            mStar_AdjustHTotal(tempHTotal + 1);
            autoWidth2 = SearchMaxWidth(vsyncTime);
            if (autoWidth2 == StandardModeWidth)
            {
                tempHTotal = tempHTotal + 1;
            }
            else if (autoWidth == StandardModeWidth)
            {
                tempHTotal = tempHTotal - 1;
            }
            #if 0
                if (autoWidth != StandardModeWidth)
                    tempHTotal = tempHTotal + (StandardModeWidth - autoWidth);
            #endif 
        }
        #if EvenClock
            if (StandardModeGroup != Res_1152x864)
            {
                if (tempHTotal % 4)
                {
                    tempHTotal = ((tempHTotal + 1) / 4) *4;
                }
            }
        #endif 
        /*
        if(tempHTotal%2)  // jeff 12 25
        {
        if (tempHTotal%4)
        tempHTotal=((tempHTotal+1)/4)*4;
        }
         */
        AutoHTotalFailed: result = FALSE;
        if (abs(tempHTotal - StandardModeHTotal) <= 100)
        //80) //60)
        {
            UserPrefHTotal = tempHTotal;
            result = TRUE;
        }
        mStar_AdjustHTotal(UserPrefHTotal);
        mStar_AdjustPhase(UserPrefPhase);
        printData("final htotal %d", UserPrefHTotal);
        return result;
        #undef result
    }
#endif 
#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
#else
Bool mStar_AutoPhase(BYTE vsyncTime)
{
    BYTE adjPhase, i;
    #if UseWorstPhase
        BYTE worsePhase, worsePhase2;
        DWORD mincksum, cksum;
        mincksum = 0xFFFFFFFFul;
        for (adjPhase = 0; adjPhase < 0x40; adjPhase += 3)
        {
            mStar_AdjustPhase(adjPhase);
            Delay1ms(vsyncTime);
            WaitAutoStatusReady(ATPCTRL, ATPR_B);
            cksum = mStar_ReadWord(ATPV4);
            cksum = (cksum << 16) | mStar_ReadWord(ATPV2);
            if (cksum < mincksum)
            {
                mincksum = cksum;
                worsePhase = adjPhase;
            }
            if (CheckSyncLoss())
            {
                return FALSE;
            }
        }
        //  printData("coast phase %d", bestPhase);
        // 2nd Search
        if (worsePhase < 3)
        {
            worsePhase = (worsePhase + 61) % 0x40;
        } // 64-3
        else
        {
            worsePhase -= 3;
        }
        mincksum = 0xFFFFFFFFul;
        for (adjPhase = worsePhase; adjPhase < worsePhase + 5; adjPhase++)
        {
            i = adjPhase % 0x40;
            mStar_AdjustPhase(i);
            Delay1ms(vsyncTime);
            WaitAutoStatusReady(ATPCTRL, ATPR_B);
            cksum = mStar_ReadWord(ATPV4);
            cksum = (cksum << 16) | mStar_ReadWord(ATPV2);
            if (cksum < mincksum)
            {
                mincksum = cksum;
                worsePhase2 = i;
            }
            if (CheckSyncLoss())
            {
                return FALSE;
            }
        }
        UserPrefPhase = (worsePhase2 + 31) % 0x40;
    #else 
        BYTE bestPhase, bestPhase2;
        DWORD maxcksum, cksum;
        maxcksum = 0;
        for (adjPhase = 0; adjPhase < 0x40; adjPhase += 3)
        {
            mStar_AdjustPhase(adjPhase);
            Delay1ms(vsyncTime);
            WaitAutoStatusReady(ATPCTRL, ATPR_B);
            cksum = mStar_ReadWord(ATPV4);
            cksum = (cksum << 16) | mStar_ReadWord(ATPV2);
            if (cksum > maxcksum)
            {
                maxcksum = cksum;
                bestPhase = adjPhase;
            }
            if (CheckSyncLoss())
            {
                return FALSE;
            }
        }
        //  printData("coast phase %d", bestPhase);
        // 2nd Search
        if (bestPhase < 3)
        {
            bestPhase = (bestPhase + 61) % 0x40;
        } // 64-3
        else
        {
            bestPhase -= 3;
        }
        maxcksum = 0;
        for (adjPhase = bestPhase; adjPhase < bestPhase + 5; adjPhase++)
        {
            i = adjPhase % 0x40;
            mStar_AdjustPhase(i);
            Delay1ms(vsyncTime);
            WaitAutoStatusReady(ATPCTRL, ATPR_B);
            cksum = mStar_ReadWord(ATPV4);
            cksum = (cksum << 16) | mStar_ReadWord(ATPV2);
            if (cksum > maxcksum)
            {
                maxcksum = cksum;
                bestPhase2 = i;
            }
            if (CheckSyncLoss())
            {
                return FALSE;
            }
        }
        UserPrefPhase = bestPhase2;
    #endif 
    mStar_AdjustPhase(UserPrefPhase);
    return UserPrefPhase;
}
#endif
#if 0//2006-03-30 ANDY 1
    void mStar_AutoPosition(void)
    {
        WORD height, width;
        WORD actHeight;
        WORD hStart, vStart;
        hStart = GetAutoValue(AOHST_H);
        vStart = GetAutoValue(AOVST_H);
        //printData("Vstar %d",vStart);
        if (hStart > StandardModeWidth || vStart > StandardModeHeight)
        {
            //printMsg("return...");
            return ;
        }
        width = GetAutoValue(AOHEND_H) - hStart + 1;
        height = GetAutoValue(AOVEND_H) - vStart;
        actHeight = mStar_ReadWord(SPRVDC_H);
        if (UserModeFlag)
        {
            if (height < (StandardModeHeight *2 / 3) || width < (StandardModeWidth *2 / 3))
            {
                UserPrefHTotal = StandardModeHTotal;
                mStar_AdjustHTotal(UserPrefHTotal);
                UserPrefHStart = StandardModeHStart;
                UserPrefVStart = StandardModeVStart;
                UserPrefAutoHStart = StandardModeHStart;
                UserPrefAutoVStart = StandardModeVStart;
                goto AutoPositionFail;
            }
        }
        else if (height < (StandardModeHeight - 80) || width < (StandardModeWidth - 120))
        {
            UserPrefHTotal = StandardModeHTotal;
            mStar_AdjustHTotal(UserPrefHTotal);
            UserPrefHStart = StandardModeHStart;
            UserPrefVStart = StandardModeVStart;
            UserPrefAutoHStart = StandardModeHStart;
            UserPrefAutoVStart = StandardModeVStart;
            goto AutoPositionFail;
        }
        if (actHeight > height)
        {
            height = (actHeight - height) / 2;
            // printMsg("Exe_1");
            if (vStart > height)
            {
                UserPrefVStart = vStart - height;
                // printMsg("Exe_2");
                // printData("UserPrefVstar %d",UserPrefVStart);
            }
            else
            {
                UserPrefHTotal = StandardModeHTotal;
                mStar_AdjustHTotal(UserPrefHTotal);
                UserPrefHStart = StandardModeHStart;
                UserPrefVStart = StandardModeVStart;
                UserPrefAutoHStart = StandardModeHStart;
                UserPrefAutoVStart = StandardModeVStart;
                goto AutoPositionFail;
            }
        }
        else
        {
            UserPrefHTotal = StandardModeHTotal;
            mStar_AdjustHTotal(UserPrefHTotal);
            UserPrefHStart = StandardModeHStart;
            UserPrefVStart = StandardModeVStart;
            UserPrefAutoHStart = StandardModeHStart;
            UserPrefAutoVStart = StandardModeVStart;
            goto AutoPositionFail;
        }
        #if 0//PanelMinHTotal>1300 && PanelHeight==768
            else if (StandardModeHeight < actHeight && StandardModeHeight < 400)
            {
                height = (actHeight - 350) / 2;
                if (vStart > height)
                    UserPrefVStart = vStart - height;
                else
                    UserPrefVStart = 0;
            }
        #endif 
        UserPrefHStart = hStart;
        AutoPositionFail: if (SrcInputType == Input_Digital)
            UserPrefHStart -= 0x10;
        mStar_WriteWord(SPRHST_H, UserPrefHStart);
        mStar_WriteWord(SPRVST_H, UserPrefVStart);
        printData("input timing index = %d", SrcModeIndex);
        // printData("HTotal=%d",  UserPrefHTotal);//StandardModeHTotal); //sheeper 040831
        // printData("HStart=%d",  UserPrefHStart);//StandardModeHStart);
        // printData("VStart=%d",  UserPrefVStart);//StandardModeVStart);
    }
#else 
    Bool mStar_AutoPosition(void)
    {
        WORD height, width;
        WORD actHeight;
        WORD hStart, vStart;
        hStart = GetAutoValue(AOHST_H);
        vStart = GetAutoValue(AOVST_H);
        //printData("Vstar %d",vStart);
        if (hStart > StandardModeWidth || vStart > StandardModeHeight)
        {
            // printMsg("return...");
            return FALSE;
        }
        
        if (StandardModeGroup == Res_640x350 && abs(StandardModeVFreq - 700) < 10)
        {
            if (vStart > (2 *StandardModeVStart))
            {
                UserPrefVStart = StandardModeVStart;
                //mStar_WriteWord(SPRVST_H, UserPrefVStart);
                if (GetAutoValue(AOVEND_H) < (StandardModeVStart + StandardModeHeight - 10))
                    UserPrefHStart = StandardModeHStart;
                else
                    UserPrefHStart = hStart;
            }
            else
            {
                UserPrefHStart = hStart;
                UserPrefVStart = vStart;
            }
        }
        else
        {
            width = GetAutoValue(AOHEND_H) - hStart + 1;
            height = GetAutoValue(AOVEND_H) - vStart;
            actHeight = mStar_ReadWord(SPRVDC_H);
            if (UserModeFlag)
            {
                if (height < (StandardModeHeight *2 / 3) || width < (StandardModeWidth *2 / 3))
                {
                    UserPrefHTotal = StandardModeHTotal;
					#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
					//msADC_AdjustHTotal(UserPrefHTotal);
					#else
                    mStar_AdjustHTotal(UserPrefHTotal);
					#endif
                    UserPrefHStart = StandardModeHStart;
                    //UserPrefVStart=StandardModeVStart+1;
                    UserPrefAutoHStart = StandardModeHStart;
                    #if 0   //2006-07-07 Andy   
                        if (ShrinkModeFlag)
                        {
                            UserPrefVStart = StandardModeVStart + 1;
                            UserPrefAutoVStart = StandardModeVStart + 1;
                        }
                        else
                    #endif 
                    {
                        UserPrefVStart = StandardModeVStart;
                        UserPrefAutoVStart = StandardModeVStart;
                    }
                    goto AutoPositionFail;
                }
            }
            else if (height < (StandardModeHeight - 80) || width < (StandardModeWidth - 120))
            {
                UserPrefHTotal = StandardModeHTotal;
				#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
				//msADC_AdjustHTotal(UserPrefHTotal);
				#else
                mStar_AdjustHTotal(UserPrefHTotal);
				#endif
                UserPrefHStart = StandardModeHStart;
                //UserPrefVStart=StandardModeVStart+1;
                UserPrefAutoHStart = StandardModeHStart;
                #if 0 //2006-07-07 Andy
                    if (ShrinkModeFlag)
                    {
                        UserPrefAutoVStart = StandardModeVStart + 1;
                        UserPrefVStart = StandardModeVStart + 1;
                    }
                    else
                #endif 
                {
                    UserPrefAutoVStart = StandardModeVStart;
                    UserPrefVStart = StandardModeVStart;
                }
                goto AutoPositionFail;
            }
		       if(DecVScaleFlag)
			{
				actHeight = actHeight - VScaleDecValue;
			}	
            if (actHeight > height||UserModeFlag)
            {
                height = (actHeight - height) / 2;
                if (vStart > height)
                {
                    #if 0   //2006-07-07 Andy
                        if (ShrinkModeFlag)
                            UserPrefVStart = (vStart - height) + 1;
                        else
                    #endif 
                        UserPrefVStart = (vStart - height);
                    //printMsg("Exe_2");
                    //printData("UserPrefVstar %d",UserPrefVStart);
                }
                else
                {
                    //printMsg("1 Stand H.V...");
                    UserPrefHTotal = StandardModeHTotal;
					#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
				//	msADC_AdjustHTotal(UserPrefHTotal);
					#else
                    mStar_AdjustHTotal(UserPrefHTotal);
					#endif
                    UserPrefHStart = StandardModeHStart;
                    //UserPrefVStart=StandardModeVStart+1;
                    UserPrefAutoHStart = StandardModeHStart;
                    #if 0   //2006-07-07 Andy
                        if (ShrinkModeFlag)
                        {
                            UserPrefAutoVStart = StandardModeVStart + 1;
                            UserPrefVStart = StandardModeVStart + 1;
                        }
                        else
                    #endif 
                    {
                        UserPrefAutoVStart = StandardModeVStart;
                        UserPrefVStart = StandardModeVStart;
                    }
                    goto AutoPositionFail;
                }
            }

	  #ifdef PanelTpye_1366// PanelWidth==1366&&PanelHeight==768
            // input 1360x768 display on 1366 panel 
            // input 1360 get 1366, so -3 is in center position.
            if(PanelWidth == 1366)
            {
                if(StandardModeGroup == Res_1360x768)
                    UserPrefHStart = hStart-3;
                else
                    UserPrefHStart = hStart;
            }
            else
			#endif

            UserPrefHStart = hStart;
        } //2006-02-23 Andy
        AutoPositionFail: 
	if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2)
            UserPrefHStart -= 0x10;
			#if DEBUG_PRINTDATA
        printData("H000 After auto %d",UserPrefHStart);
        printData("V000 After auto %d",UserPrefVStart);
		#endif

        #if 0
  if(StandardModeGroup==Res_720x400&& 
      ((GetAutoValue(AOVEND_H)-GetAutoValue(AOVST_H)<380)||(GetAutoValue(AOHEND_H)-GetAutoValue(AOHST_H)<700)))
  {
      //UserPrefAutoVStart=StandardModeVStart;
      //UserPrefVStart=StandardModeVStart;
     //  UserPrefHStart=StandardModeHStart;   //eshin for -1k/hz
     // UserPrefHTotal=StandardModeHTotal;
      mStar_AdjustHTotal(UserPrefHTotal);
  }
  else if(StandardModeGroup==Res_640x480&& 
      ((GetAutoValue(AOVEND_H)-GetAutoValue(AOVST_H)<460)||(GetAutoValue(AOHEND_H)-GetAutoValue(AOHST_H)<620)))
            {
                UserPrefAutoVStart = StandardModeVStart;
                UserPrefVStart = StandardModeVStart;
                UserPrefHStart = StandardModeHStart;
                UserPrefHTotal = StandardModeHTotal;
                mStar_AdjustHTotal(UserPrefHTotal);
            }
        #endif 
		//printData("UserPrefHStart==%d", UserPrefHStart);
			//	printData("UserPrefVStart==%d", UserPrefVStart);

        mStar_WriteWord(SPRHST_H, UserPrefHStart);
        if (DecVScaleFlag)
        // 2006/7/6 0:10AM by Emily 
        {
            if (UserPrefVStart <= (VScaleDecValue / 2))
                UserPrefVStart = (VScaleDecValue / 2) + 1;
            mStar_WriteWord(SPRVST_H, UserPrefVStart - (VScaleDecValue / 2));
        }
        else
            mStar_WriteWord(SPRVST_H, UserPrefVStart);

	return TRUE;
    }
#endif 
//==========================================
//========== 2008-02-12 ====================
#if 0
    void CheckDosTextHtotal(BYTE vsyncTime)
    {
        WORD hStart, vStart, height;
        //BYTE i;
        //for 640x350, 720x400, 640x480 mode
        //2006-07-04 Andy  if (SrcModeIndex==0||SrcModeIndex==2||SrcModeIndex==6) // 
        if (SrcModeIndex == 18 || SrcModeIndex == 20 || SrcModeIndex == 24)
        //
        {
            #if 1  //2006-02-23 Andy
                hStart = GetAutoValue(AOHST_H);
                vStart = GetAutoValue(AOVST_H);
                height = GetAutoValue(AOVEND_H) - vStart;
                //2006-07-04 Andy  if (height<(StandardModeHeight-1)||SrcModeIndex==2)
                if (height < (StandardModeHeight - 1) || SrcModeIndex == 20)
                {
                    //printData("height=%d",height);
                    height = vsyncTime;
                    //2006-07-04 Andy  if(SrcModeIndex==2&&abs(UserPrefHTotal-StandardModeHTotal)<5)
                    if (SrcModeIndex == 20 && abs(UserPrefHTotal - StandardModeHTotal) < 5)
                    {
                        UserPrefHTotal = UserPrefHTotal;
                    }
                    else
                    {
                        UserPrefHTotal = StandardModeHTotal;
                    }
                    mStar_AdjustHTotal(UserPrefHTotal);
                }
            #endif 
        }
    }
#endif 
//=================================================================================
//=================================================================================
//=================================================================================
// Auto Color Setting
#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
#else
Bool mStar_AutoAdjustAdcGain(BYTE vsyncTime)		//110527 Modify
{
    BYTE rGain, gGain, bGain;
    BYTE maxRGain, maxGGain, maxBGain;
    BYTE minRGain, minGGain, minBGain;
    BYTE retry = 8;
    BYTE adcStatus;
    BYTE atgFlags = 0;
    maxRGain = maxGGain = maxBGain = 0xFF;
    minRGain = minGGain = minBGain = 0;
    while (retry--)
    {
        rGain = ((WORD)maxRGain + minRGain) / 2;
        gGain = ((WORD)maxGGain + minGGain) / 2;
        bGain = ((WORD)maxBGain + minBGain) / 2;
        mStar_AdjustAdcGain(rGain, gGain, bGain);
        Delay1ms(vsyncTime);
        WaitAutoStatusReady(ATGCTRL, ATGR_B);
        adcStatus = mStar_ReadByte(ATGCTRL);
        if (adcStatus &MAXR_B)
        {
            rGain--;
            atgFlags |= BIT7;
        }
        else
        {
            rGain++;
            atgFlags |= BIT6;
        }
        if (adcStatus &MAXG_B)
        {
            gGain--;
            atgFlags |= BIT5;
        }
        else
        {
            gGain++;
            atgFlags |= BIT4;
        }
        if (adcStatus &MAXB_B)
        {
            bGain--;
            atgFlags |= BIT3;
        }
        else
        {
            bGain++;
            atgFlags |= BIT2;
        }
        mStar_AdjustAdcGain(rGain, gGain, bGain);
        Delay1ms(vsyncTime);
        WaitAutoStatusReady(ATGCTRL, ATGR_B);
        adcStatus = mStar_ReadByte(ATGCTRL);
        if (adcStatus &MAXR_B)
        {
            maxRGain = rGain + 1;
            atgFlags |= BIT7;
        }
        else
        {
            minRGain = rGain - 1;
            atgFlags |= BIT6;
        }
        if (adcStatus &MAXG_B)
        {
            maxGGain = gGain + 1;
            atgFlags |= BIT5;
        }
        else
        {
            minGGain = gGain - 1;
            atgFlags |= BIT4;
        }
        if (adcStatus &MAXB_B)
        {
            maxBGain = bGain + 1;
            atgFlags |= BIT3;
        }
        else
        {
            minBGain = bGain - 1;
            atgFlags |= BIT2;
        }
        if (CheckSyncLoss())
        {
            return FALSE;
        }
    }
    if ((atgFlags &0xFC) != 0xFC)
    {
        return FALSE;
    }
    UserPrefAdcRedGain = rGain - 1;
    UserPrefAdcGreenGain = gGain - 1;
    UserPrefAdcBlueGain = bGain - 1;
    mStar_AdjustAdcGain(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
    return TRUE;
}

#if defined TSUMXXNX || defined TSUMXXQX

#ifdef TSUMXXQX
#define REGBANK_ADC_DTOP    0x09
#else
#define REGBANK_ADC_DTOP    0x07
#endif

#define OffsetValue     3
Bool mStart_ADCOffset_AtuoCalibraton(void)
{
    BYTE BKE_0C_Value,BKE_0B_Value;
    BYTE R_OFFSET,G_OFFSET,B_OFFSET;
    BYTE u8ReTry=0;

    //ReSetOffset:

    if(SrcInputType>=Input_Digital)
	return FALSE;
    
    while(u8ReTry++<100)
    {
        mStar_AdjustAdcOffset(0x7D, 0x7D, 0x7D);
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(BK1_3F, 0x18);
        mStar_WriteByte(BK1_40, mStar_ReadByte(BK1_40)|BIT5);
        mStar_WriteByte(BK1_3F, 0x1C);

        mStar_WriteByte(REGBK, REGBANK_ADC_DTOP);
        BKE_0C_Value=mStar_ReadByte(0x0C);
        BKE_0B_Value=mStar_ReadByte(0x0B);

        mStar_WriteByte(0x0C, BKE_0C_Value|BIT3);
        mStar_WriteByte(0x0B, BKE_0B_Value|0x01);
        Delay1ms(80);
        mStar_WriteByte(REGBK, REGBANKADC);
        R_OFFSET= mStar_ReadByte(BK1_3C);
        #if 0//DEBUG_EN
        printData("ROFF = %x", R_OFFSET);
        #endif

        mStar_WriteByte(REGBK, REGBANK_ADC_DTOP);
        mStar_WriteByte(0x0B, BKE_0B_Value|0x02);
        Delay1ms(80);
        mStar_WriteByte(REGBK, REGBANKADC);
        G_OFFSET= mStar_ReadByte(BK1_3C);
        #if 0//DEBUG_EN
        printData("GOFF = %x", G_OFFSET);
        #endif

        mStar_WriteByte(REGBK, REGBANK_ADC_DTOP);
        mStar_WriteByte(0x0B, BKE_0B_Value|0x03);
        Delay1ms(80);
        mStar_WriteByte(REGBK, REGBANKADC);
        B_OFFSET= mStar_ReadByte(BK1_3C);
        #if 0//DEBUG_EN
        printData("BOFF = %x", B_OFFSET);
        #endif

        mStar_WriteByte(REGBK, REGBANK_ADC_DTOP);
        mStar_WriteByte(0x0C, BKE_0C_Value);
        mStar_WriteByte(0x0B, BKE_0B_Value);

        mStar_WriteByte(REGBK, REGBANKSCALER);

        if(!(R_OFFSET==0x7F&&G_OFFSET==0x7F&&B_OFFSET==0x7F))
            break;
        //if((R_OFFSET==0x7F&&G_OFFSET==0x7F&&B_OFFSET==0x7F)&&ReTry<100)
        //goto ReSetOffset;
    }      
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(BK1_40, mStar_ReadByte(BK1_40)&~BIT5);
    mStar_WriteByte(BK1_3F, 0x00);
    mStar_WriteByte(REGBK, REGBANKSCALER);

    if (u8ReTry<100)
    {
       #if ModelName==ENVISION_G2770
        UserPrefAdcRedOffset  =R_OFFSET-5;
        UserPrefAdcGreenOffset=G_OFFSET-5;
        UserPrefAdcBlueOffset =B_OFFSET-5;
	#elif ModelName==AOC_1620Sw//100908
	    UserPrefAdcRedOffset  =R_OFFSET-5;
        UserPrefAdcGreenOffset=G_OFFSET-5;
        UserPrefAdcBlueOffset =B_OFFSET-5;
	#elif ModelName==ENVISOIN_P2473wl
		UserPrefAdcRedOffset  =R_OFFSET-5;
		UserPrefAdcGreenOffset=G_OFFSET-5;
		UserPrefAdcBlueOffset =B_OFFSET-5;
        #elif ModelName==AOC_e2795v||ModelName==ENVISION_G2770L  
		UserPrefAdcRedOffset  =R_OFFSET-5;
		UserPrefAdcGreenOffset=G_OFFSET-5;
		UserPrefAdcBlueOffset =B_OFFSET-5;
	#else
        UserPrefAdcRedOffset  =R_OFFSET-OffsetValue;
        UserPrefAdcGreenOffset=G_OFFSET-OffsetValue;
        UserPrefAdcBlueOffset =B_OFFSET-OffsetValue;
	#endif

        mStar_AdjustAdcOffset(UserPrefAdcRedOffset , UserPrefAdcGreenOffset, UserPrefAdcBlueOffset);
        return TRUE;
    }
    return FALSE;
}
#else
Bool mStar_AutoAdjustAdcOffset(BYTE vsyncTime)
{
    BYTE rOffset, gOffset, bOffset;
    BYTE maxROffset, maxGOffset, maxBOffset;
    BYTE minROffset, minGOffset, minBOffset;
    BYTE retry = 8;
    BYTE adcStatus;
    BYTE atgFlags = 0;
    maxROffset = maxGOffset = maxBOffset = 0xFF;
    minROffset = minGOffset = minBOffset = 0;
    while (retry--)
    {
        rOffset = ((WORD)maxROffset + minROffset) / 2;
        gOffset = ((WORD)maxGOffset + minGOffset) / 2;
        bOffset = ((WORD)maxBOffset + minBOffset) / 2;
        mStar_AdjustAdcOffset(rOffset, gOffset, bOffset);
        Delay1ms(vsyncTime);
        WaitAutoStatusReady(ATGCTRL, ATGR_B);
        adcStatus = mStar_ReadByte(ATGST);
        if (adcStatus &MINR_B)
        {
            rOffset++;
            atgFlags |= UnderFlow;
        }
        else
        {
            rOffset--;
            atgFlags |= NoneUnderFlow;
        }
        if (adcStatus &MING_B)
        {
            gOffset++;
            atgFlags |= UnderFlow;
        }
        else
        {
            gOffset--;
            atgFlags |= NoneUnderFlow;
        }
        if (adcStatus &MINB_B)
        {
            bOffset++;
            atgFlags |= UnderFlow;
        }
        else
        {
            bOffset--;
            atgFlags |= NoneUnderFlow;
        }
        mStar_AdjustAdcOffset(rOffset, gOffset, bOffset);
        Delay1ms(vsyncTime);
        WaitAutoStatusReady(ATGCTRL, ATGR_B);
        adcStatus = mStar_ReadByte(ATGST);
        if (adcStatus &MINR_B)
        {
            minROffset = rOffset - 1;
            atgFlags |= UnderFlow;
        }
        else
        {
            maxROffset = rOffset + 1;
            atgFlags |= NoneUnderFlow;
        }
        if (adcStatus &MING_B)
        {
            minGOffset = gOffset - 1;
            atgFlags |= UnderFlow;
        }
        else
        {
            maxGOffset = gOffset + 1;
            atgFlags |= NoneUnderFlow;
        }
        if (adcStatus &MINB_B)
        {
            minBOffset = bOffset - 1;
            atgFlags |= UnderFlow;
        }
        else
        {
            maxBOffset = bOffset + 1;
            atgFlags |= NoneUnderFlow;
        }
        if (CheckSyncLoss())
        {
            return FALSE;
        }
    }
    if ((atgFlags &0xC0) != 0xC0)
    {
        return FALSE;
    }
    #if PanelType==PanelCLAA220WA01||PanelType==PanelLM220WE1
    UserPrefAdcRedOffset = rOffset -2;
    UserPrefAdcGreenOffset = gOffset -2;
    UserPrefAdcBlueOffset = bOffset -2;
    #elif PanelType==PanelLM230WF1TLA3||PanelType==PanelTPM215HW01
    UserPrefAdcRedOffset = rOffset -4;
    UserPrefAdcGreenOffset = gOffset -4;
    UserPrefAdcBlueOffset = bOffset -4;
    #elif MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A||PanelType==PanelType==PanelLM230WF1TLD1
    UserPrefAdcRedOffset = rOffset -3;
    UserPrefAdcGreenOffset = gOffset -3;
    UserPrefAdcBlueOffset = bOffset -3;
    #else
    UserPrefAdcRedOffset = rOffset;
    UserPrefAdcGreenOffset = gOffset;
    UserPrefAdcBlueOffset = bOffset;
    #endif	
    mStar_AdjustAdcOffset(UserPrefAdcRedOffset, UserPrefAdcGreenOffset, UserPrefAdcBlueOffset);
    return TRUE;
}
#endif
//==================================================================
Bool mStar_AutoAdcColor(BYTE vsyncTime)
{
    BYTE result;
    if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2)
    {
        return FALSE;
    }
    mStar_WriteByte(ATGCTRL, 0x11); // enable auto Gain
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(DBFC, 0);
    mStar_WriteByte(REGBK, REGBANKSCALER);
    //  vsyncTime=GetVSyncTime();
    #if defined TSUMXXNX || defined TSUMXXQX
    result = mStart_ADCOffset_AtuoCalibraton();
    #else
    result = mStar_AutoAdjustAdcOffset(vsyncTime);
    #endif
    if (!result)
    {
        mStar_AdjustAdcOffset(UserPrefAdcRedOffset, UserPrefAdcGreenOffset, UserPrefAdcBlueOffset);
    }
    result = mStar_AutoAdjustAdcGain(vsyncTime);
    if (!result)
    {
        mStar_AdjustAdcGain(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
    }
    #if 0
        printData("adc gain red %d", UserPrefAdcRedGain);
        printData("adc gain green %d", UserPrefAdcGreenGain);
        printData("adc gain blue %d", UserPrefAdcBlueGain);
        printData("adc offset red %d", UserPrefAdcRedOffset);
        printData("adc offset green %d", UserPrefAdcGreenOffset);
        printData("adc offset blue %d", UserPrefAdcBlueOffset);
    #endif 
    mStar_WriteByte(REGBK, REGBANKADC);
    mStar_WriteByte(DBFC, 1);
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(ATGCTRL, 0); // enable auto Gain
    return result;
}
#endif
#ifdef PanelTpye_1366// (PanelWidth == 1366)//#if PanelType==PanelM156B1L01||PanelType==PanelSVA156WX1 || PanelType == PanelCLAA185WA01|| PanelType == PanelM185B1L01||PanelType==PanelLM185WH1TLA1||PanelType==PanelAUOM185XW01
void CheckHtotal(void)
{
#if 1
      WORD cksum,cksum1,maxchsum,maxchsum1,tempHtotal, temp1, temp2, temp3;
      BYTE TempValue1,TempValue2 , i;
      BYTE retry=2;
	  temp1 =0;
	  temp2=0;
	  temp3=0;
	  
     
      TempValue1=mStar_ReadByte(AOVDV);
      TempValue2=mStar_ReadByte(DBFC);
      
    mStar_WriteByte(AOVDV, 0x20);
    mStar_WriteByte(DBFC, 5);

do//while (retry--)
{

#if DEBUG_PRINTDATA
        printData("retry111 ==%d", retry);

#endif


	if(retry==0)
	{
		tempHtotal=StandardModeHTotal-6;
#if DEBUG_PRINTDATA
        printData("retry ==%d", retry);
	printData("aaaa mode = %d", tempHtotal);
	printData("bbbb user = %d", UserPrefHTotal);	
#endif		
	}
	else if(retry==1)
	{
		tempHtotal=StandardModeHTotal+2;
#if DEBUG_PRINTDATA
        printData("retry ==%d", retry);
	printData("aaaa mode = %d", tempHtotal);
	printData("bbbb user = %d", UserPrefHTotal);	
#endif
		
	}

	else if(retry==2)
	{
		tempHtotal=StandardModeHTotal;

#if DEBUG_PRINTDATA
        printData("retry ==%d", retry);
	printData("aaaa mode = %d", tempHtotal);
	printData("bbbb user = %d", UserPrefHTotal);	
#endif
		
	}
	

// if(UserPrefHTotal!=tempHtotal)
 {

#if DEBUG_PRINTDATA
	printData("aaaa mode = %d", tempHtotal);
	printData("bbbb user = %d", UserPrefHTotal);	
#endif
 	 maxchsum=0;
	 maxchsum1=0;
	 #ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
	 for(i=0;i<127;i=i+2)//or    for (i=0; i<63; i=i+2)??
	 #else
 	 for(i=0;i<63;i=i+2)
	 #endif
 	 { 
		#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		msADC_SetPhaseCode(i);
		#else
		mStar_AdjustPhase(i);
		#endif
 	    Delay1ms(10);
 	 	
	    WaitAutoStatusReady(ATPCTRL, ATPR_B);
	    cksum=mStar_ReadWord(ATPV4);
	    if (cksum>maxchsum)
	  	 maxchsum=cksum;
 	  }
	 
		#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		msADC_AdjustHTotal(tempHtotal);
		#else
		mStar_AdjustHTotal(tempHtotal);
		#endif
		Delay1ms(30);
	  #ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
	  for(i=0; i<127; i=i+2)//or	for (i=0; i<63; i=i+2)??
	  #else
	  for(i=0;i<63;i=i+2)
	  #endif
 	  { 
		 #ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
		 msADC_SetPhaseCode(i);
		 #else
 	     mStar_AdjustPhase(i);
		 #endif
 	     Delay1ms(10);
	     WaitAutoStatusReady(ATPCTRL, ATPR_B);
	     cksum1=mStar_ReadWord(ATPV4);
	     if (cksum1>maxchsum1)
	  	 maxchsum1=cksum1;
          }

       if(maxchsum1>maxchsum)
	  	UserPrefHTotal=tempHtotal;

	if(retry==0)
		temp1=maxchsum1;
	else if(retry==1)
		temp2=maxchsum1;
	else //if(retry==3)
		temp3=maxchsum1;

#if DEBUG_PRINTDATA
        printData("retry ==%d", retry);
        printData("!!!!!temp1 %d", temp1);
        printData("!!!!!temp2 %d", temp2);
        printData("!!!!!temp3 %d", temp3);
#endif
	#ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
	  msADC_AdjustHTotal(UserPrefHTotal);
	#else
	  mStar_AdjustHTotal(UserPrefHTotal);
	#endif

}

}while (retry--);//while

if(temp1>temp2)
{
	if(temp1>temp3)
		UserPrefHTotal=StandardModeHTotal-6;
	else if( temp2>temp3 )
		UserPrefHTotal=StandardModeHTotal+2;
	else
		UserPrefHTotal=StandardModeHTotal;
}
else
{
	if(temp2>temp3)
		UserPrefHTotal=StandardModeHTotal+2;
	else if( temp3 >temp1 )
		UserPrefHTotal=StandardModeHTotal;
	else
		UserPrefHTotal=StandardModeHTotal-6;
}
	  #ifdef TSUMXXT//#if (ChipID == TSUMXXT)		//110311 Modify
	  msADC_AdjustHTotal(UserPrefHTotal);
	  #else
	  mStar_AdjustHTotal(UserPrefHTotal);
	  #endif

#else

      WORD cksum,cksum1,maxchsum,maxchsum1;
      BYTE TempValue1,TempValue2 , i;
     
      TempValue1=mStar_ReadByte(AOVDV);
      TempValue2=mStar_ReadByte(DBFC);
      
    mStar_WriteByte(AOVDV, 0x20);
    mStar_WriteByte(DBFC, 5);
 if(UserPrefHTotal!=StandardModeHTotal)
 {
 	 maxchsum=0;
	 maxchsum1=0;
 	 for(i=0;i<63;i=i+2)
 	 { mStar_AdjustPhase(i);
 	    Delay1ms(10);
 	 	
	    WaitAutoStatusReady(ATPCTRL, ATPR_B);
	    cksum=mStar_ReadWord(ATPV4);
	    if (cksum>maxchsum)
	  	 maxchsum=cksum;
 	  }
	 
	   mStar_AdjustHTotal(StandardModeHTotal);
	   Delay1ms(20);
	  for(i=0;i<63;i=i+2)
 	  { 
 	     mStar_AdjustPhase(i);
 	     Delay1ms(10);
	     WaitAutoStatusReady(ATPCTRL, ATPR_B);
	     cksum1=mStar_ReadWord(ATPV4);
	     if (cksum1>maxchsum1)
	  	 maxchsum1=cksum1;
          }

       if(maxchsum1>maxchsum)
	  	UserPrefHTotal=StandardModeHTotal;

	  mStar_AdjustHTotal(UserPrefHTotal);
	
}
#endif
    mStar_WriteByte(AOVDV, TempValue1);
    mStar_WriteByte(DBFC, TempValue2);
}
#endif
//==================================================================
#if 0 //2006-03-30 Andy 
    Bool mStar_AutoGeomtry(void)
    {
        Bool result = TRUE;
        BYTE vsyncTime;
        vsyncTime = GetVSyncTime();
        #if 0
            if (FactoryModeFlag)
            {
                mStar_AutoAdcColor(vsyncTime);
                if (InputTimingChangeFlag)
                    return FALSE;
            }
        #endif 
        //mStar_WriteByte(DBFC, 0);// 0708  -> Prevent image disappear when V.P is set to almost biggest. 
        if (SrcInputType != Input_Digital)
        {
            // Automatically adjusting Data threshold
            mStar_AdjustAdcGain(0x80, 0x80, 0x80);
            mStar_AdjustAdcOffset(0x90, 0x90, 0x90);
            // auto adjust threshold
            #if QuickAuto
                mStar_WriteByte(AOVDV, 0x40);
                //  mStar_WriteByte(AOVDV, 0x60);
            #else 
                AutoSetDataThreshold(vsyncTime + 3);
            #endif 
            if (InputTimingChangeFlag)
            {
                result = FALSE;
                goto AutoGeomtryFailed;
            }
            // auto adjust htotal
            result = mStar_AutoHTotal(vsyncTime *2);
            if (InputTimingChangeFlag)
            {
                result = FALSE;
                goto AutoGeomtryFailed;
            }
            // auto adjust phase
            if (result)
            {
                mStar_AdjustAdcGain(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
                mStar_AdjustAdcOffset(UserPrefAdcRedOffset, UserPrefAdcGreenOffset, UserPrefAdcBlueOffset);
                mStar_AutoPhase(vsyncTime + 3);
            }
            if (InputTimingChangeFlag)
            {
                result = FALSE;
                goto AutoGeomtryFailed;
            }
            Delay1ms(vsyncTime *2);
            if (result)
            //add 20050613 wumaozhong for DOS Auto wring
                mStar_AutoPosition();
        }
        AutoGeomtryFailed: 
        mStar_WriteByte(AOVDV, 0x40);
        mStar_WriteByte(DBFC, 5);
        mStar_AdjustAdcGain(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
        mStar_AdjustAdcOffset(UserPrefAdcRedOffset, UserPrefAdcGreenOffset, UserPrefAdcBlueOffset);
        return result;
    }
#else 
    //2006-03-30 Andy
    Bool mStar_AutoGeomtry(void)
    {
        Bool result = TRUE;
        BYTE vsyncTime;

        // 100512 coding addition
        if(InputTimingChangeFlag)
            return FALSE;
        
        vsyncTime = GetVSyncTime();
        #if 0
            if (FactoryModeFlag)
            {
                mStar_AutoAdcColor(vsyncTime);
                if (InputTimingChangeFlag)
                    return FALSE;
            }
        #endif 
        //mStar_WriteByte(DBFC, 0);// 0708  -> Prevent image disappear when V.P is set to almost biggest.
        mStar_WriteByte(DBFC, 0); // 2008-02-12

        if ((SrcInputType != Input_Digital) && (SrcInputType != Input_Digital2))//yuanfa
        {
			#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
                      mStar_WriteByteMask(BK0_0F, 0x00, BIT7);    //110401 Modify
			msADC_SetRGBGainCode(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
			#else
            // Automatically adjusting Data threshold
            mStar_AdjustAdcGain(0x80, 0x80, 0x80);
            //mStar_AdjustAdcOffset(0x90, 0x90, 0x90);
            mStar_AdjustAdcOffset(0x80, 0x80, 0x80);
			#endif
            // auto adjust threshold
            #if QuickAuto
                mStar_WriteByte(AOVDV, 0x40);
                //  mStar_WriteByte(AOVDV, 0x60);
            #else 
                //2006-02-12 AutoSetDataThreshold(vsyncTime+3);
                AutoSetDataThreshold(vsyncTime *2);
            #endif 
            if (InputTimingChangeFlag)
            {
                result = FALSE;
                goto AutoGeomtryFailed;
            }
            // auto adjust htotal
            result = mStar_AutoHTotal(vsyncTime *2);
            //2006-07-04 Andy   if (!result && )  //2006-02-12 
            //Benz 2007.4.13   21:59:12     if (!result)  //2006-02-12 
            //Benz 2007.4.13   21:59:16        result=TRUE;
            if (!result)
            //2006-02-12
                goto AutoGeomtryFailed;
            //     CheckDosTextHtotal(vsyncTime*2); //2006-02-12
            if (InputTimingChangeFlag)
            {
                result = FALSE;
                goto AutoGeomtryFailed;
            }
            // auto adjust phase
            if (result)
            {
				#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
				msADC_SetRGBGainCode(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
				#else
                mStar_AdjustAdcGain(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
                mStar_AdjustAdcOffset(UserPrefAdcRedOffset, UserPrefAdcGreenOffset, UserPrefAdcBlueOffset);
				#endif
		#ifdef PanelTpye_1366// (PanelWidth == 1366)//PanelType==PanelM156B1L01||PanelType==PanelSVA156WX1 || PanelType == PanelCLAA185WA01|| PanelType == PanelM185B1L01||PanelType==PanelLM185WH1TLA1||PanelType==PanelAUOM185XW01
			if (StandardModeGroup == Res_1366x768 && abs(StandardModeVFreq - 600) < 10)  // Check 1366 x768 60 Hz HTotal
		            CheckHtotal();
		#endif
				#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
				msADC_AutoPhaseAdjustment(vsyncTime*2);
				#else
                mStar_AutoPhase(vsyncTime *2); //2006-02-12 +3);
                #endif
            }
            if (InputTimingChangeFlag)
            {
                result = FALSE;
                goto AutoGeomtryFailed;
            }
            // 2006/10/24 2:42PM by Emily       if(DecVScaleFlag) // 2006/8/1 4:10PM by Emily 
            // 2006/10/24 2:42PM by Emily           Clr_DecVScaleFlag();
            //Delay1ms(vsyncTime*2);
            //zjb20071218
         #if AutoProcess
            wAutoProcessCount=0;	//Stop wAutoProcess increase
            wAutoProcess0=100;
            wAutoProcess1=99;
	#endif
            Delay1ms(vsyncTime *3);
            //2006-02-12 mStar_AutoPosition();
            if (mStar_AutoPosition())
            //Clock fail, but auto positon success still return true
            {
                // printMsg("Do auto position ok");
                //2006-07-04 Andy   if (!result && )
                //Benz 2007.4.13   21:59:21            if 
                //Benz 2007.4.13   21:59:22                result=TRUE;
            }
        }
        AutoGeomtryFailed: 
			
#if AutoProcess		//120703 Modify
    if(((SrcInputType != Input_Digital) && (SrcInputType != Input_Digital2)))
    {
		if(wAutoProcess0<100)
		{
			WORD retry=2000;
			while((retry--)&&(wAutoProcess0<=100))
				Delay1ms(1);
			wAutoProcessCount = 0;	//Stop wAutoProcess increase
		}
    }
#endif
		//hw_ClrAmberLed();
        mStar_WriteByte(AOVDV, 0x40);
        mStar_WriteByte(DBFC, 5);
		#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
		msADC_SetRGBGainCode(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
        mStar_WriteByteMask(BK0_0F, BIT7, BIT7);    //110401 Modify
		#else
        mStar_AdjustAdcGain(UserPrefAdcRedGain, UserPrefAdcGreenGain, UserPrefAdcBlueGain);
        mStar_AdjustAdcOffset(UserPrefAdcRedOffset, UserPrefAdcGreenOffset, UserPrefAdcBlueOffset);
		#endif
        return result;
    }
#endif 
Bool mStar_AutoColor(void)
{
    Bool result;
    BYTE vsyncTime;
    vsyncTime = GetVSyncTime();
	#ifdef TSUMXXT//#if( ChipID == TSUMXXT )		//110311 Modify
	result = msADC_AutoAdcColor(vsyncTime*2);
	#else
    result = mStar_AutoAdcColor(vsyncTime*2);
	#endif
    if (InputTimingChangeFlag)
    {
        return FALSE;
    }
    return result;
}
