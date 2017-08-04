/******************************************************************************
Copyright (c) 2003 MStar Semiconductor, Inc.
All rights reserved.
[Module Name]: MsDLC.c
[Date]:        26-Dec-2003
[Comment]:
MST DLC subroutines.
[Reversion History]:
 *******************************************************************************/
#include "board.h"
#include "types.h"
#include "msDLC.h"
#include "msRegTV.h"
#include "debug.h"
#include "global.h"
#include "adjust.h"
#include "Panel.h"
#include "ms_rwreg.h"
#include <global.h>
#include <math.h>
//#include "msACE.h"
//#define LOCAL_TYPE 
#define msDoubleBufferStart()
#define msDoubleBufferEnd()
extern code short tSRGB[3][3];
extern code short tNormalColorCorrectionMatrix[3][3];
XDATA BYTE g_ucLumaCurve[16];
XDATA WORD g_ucTable[16];
XDATA BYTE g_bLockDlc; // When this bit is true, DLC don't work!!
XDATA DWORD g_dwFactory;
XDATA DWORD g_dwFactory_7;
XDATA DWORD g_dwFactory_9;
XDATA DWORD g_dwLstep;
XDATA DWORD g_dwHstep;
XDATA BYTE g_LowStrength = 120;
XDATA BYTE g_HighStrength = 100;
XDATA BYTE g_bDLCOnOff = 0;
XDATA BYTE g_bNeedRequest = 0;
XDATA BYTE ucDLCCCount = 0;
XDATA BYTE BrightData = 0xff;
XDATA BYTE DarkData = 0x00;
XDATA BYTE PreBrightData = 0xff;
XDATA BYTE ReadFrameDataCnt = 1;
XDATA BYTE SetDarkestFlag = 0;
XDATA BYTE DarkSegmentData;
#define TopLimit_Upper  (0)
#define DownLimit_Upper (((PanelHeight/2)/8)-1)
#define TopLimit_Lower  ((PanelHeight/2)/8)
#define DownLimit_Lower ((PanelHeight/8)-1)
#define DarkestValue    0
#define DarkestRange    ((SrcInputType == Input_Analog1)?(16):(20))
#define BlackWhiteDifference    21    //16      //20081201
//#define TOP_LIMIT       ((WORD)wHeight/64)
//#define DOWN_LIMIT      ((WORD)wHeight*7/64)
#define TOP_LIMIT       (1)// 2004/11/10
#define DOWN_LIMIT      (255)//((WORD)wHeight/8)// 2005/01/21
#define TOTAL_PIXEL     ((DWORD)wHeight*0.04375*wWidth)
#define FACTORY_7       g_dwFactory_7    // low threshold
#define FACTORY_9       g_dwFactory_9    // high threshold
#define Lstep           g_dwLstep
#define Hstep           g_dwHstep
//#define TOP_LIMIT   (g_wHeight/64)
//#define DOWN_LIMIT  (g_wHeight*7/64)
//#define TOTAL_PIXEL ((DWORD)g_wHeight*0.04375*g_wWidth)
//#define FACTORY     (TOTAL_PIXEL / 8)
//#define FACTORY_7   (FACTORY * 7 / 8)      // low threshold
//#define FACTORY_9   (FACTORY + (FACTORY/8))  // high threshold
//#define Lstep       (FACTORY *1/16)*g_ucLstrength
//#define Hstep       (FACTORY *3/8)*g_ucHstrength
//#define FACTORY_M1  (FACTORY_7 - ((FACTORY *8/16)*g_ucLstrength))
//#define FACTORY_0   (FACTORY_7 - ((FACTORY *7/16)*g_ucLstrength))
//#define FACTORY_1   (FACTORY_7 - ((FACTORY *6/16)*g_ucLstrength))
//#define FACTORY_2   (FACTORY_7 - ((FACTORY *5/16)*g_ucLstrength))
//#define FACTORY_3   (FACTORY_7 - ((FACTORY *4/16)*g_ucLstrength))
//#define FACTORY_4   (FACTORY_7 - ((FACTORY *3/16)*g_ucLstrength))
//#define FACTORY_5   (FACTORY_7 - ((FACTORY *2/16)*g_ucLstrength))
//#define FACTORY_6   (FACTORY_7 - ((FACTORY *1/16)*g_ucLstrength))
//#define FACTORY_11  (FACTORY_9 + ((FACTORY*2/8)*g_ucHstrength))
//#define FACTORY_13  (FACTORY_9 + ((FACTORY*4/8)*g_ucHstrength)
//#define FACTORY_16  (FACTORY_9 + ((FACTORY*7/8)*g_ucHstrength)
//#define FACTORY_19  (FACTORY_9 + (((FACTORY)+(FACTORY*2/8))*g_ucHstrength))
//#define FACTORY_21  (FACTORY_9 + (((FACTORY)+(FACTORY*4/8))*g_ucHstrength))
//#define FACTORY_23  (FACTORY_9 + (((FACTORY)+(FACTORY*6/8))*g_ucHstrength))
//#define FACTORY_27  (FACTORY_9 + (((FACTORY*2)+(FACTORY*2/8))*g_ucHstrength))
//#define FACTORY_30  (FACTORY_9 + (((FACTORY*2)+(FACTORY*5/8))*g_ucHstrength))
//#define FACTORY_32  (FACTORY_9 + (((FACTORY*2)+(FACTORY*7/8))*g_ucHstrength))
void InitDLCVar()
{
    // DLC table initial table
    g_ucLumaCurve[0] = 0x07;
    g_ucLumaCurve[1] = 0x17;
    g_ucLumaCurve[2] = 0x27;
    g_ucLumaCurve[3] = 0x37;
    g_ucLumaCurve[4] = 0x47;
    g_ucLumaCurve[5] = 0x57;
    g_ucLumaCurve[6] = 0x67;
    g_ucLumaCurve[7] = 0x77;
    g_ucLumaCurve[8] = 0x87;
    g_ucLumaCurve[9] = 0x97;
    g_ucLumaCurve[10] = 0xA7;
    g_ucLumaCurve[11] = 0xB7;
    g_ucLumaCurve[12] = 0xC7;
    g_ucLumaCurve[13] = 0xD7;
    g_ucLumaCurve[14] = 0xE7;
    g_ucLumaCurve[15] = 0xF7;
}
//////////////////////////////////////////////////////////////////////////////
// <Name>: msSetDlcStrength
//
// <Description>: This function set DLC strength.
//
// <Parameter>:    -  <Flow>  -  <Description>
// ---------------------------------------------------------------------------
//  ucLowStrength  -    In    -  Strength of low level, from 1 to 255
//  ucHighStrenght -    In    -  Strength of high level from 1 to 255
// The unit is 0.01; the value is small and the effect is strong
// Lstrength :  0.50(strong)   <------------>   1.70 (weak) ; default = 1.20
// Hstrength:   0.50(strong)   <------------>   2.00 (weak) ; default = 1.00
//////////////////////////////////////////////////////////////////////////////
void msSetDlcStrength(BYTE pucLowStrength, BYTE pucHighStrenght)
{
    BYTE LOCAL_TYPE ucLowStrength;
    BYTE LOCAL_TYPE ucHighStrenght;
    ucLowStrength = pucLowStrength;
    ucHighStrenght = pucHighStrenght;
    if (0 == ucLowStrength)
        ucLowStrength = 1;
    if (0 == ucHighStrenght)
        ucHighStrenght = 1;
    g_dwLstep = g_dwFactory * 1 / 16 * ucLowStrength / 100;
    g_dwHstep = g_dwFactory * 3 / 8 * ucHighStrenght / 100;
}
void msDlcInit(WORD pwWidth, WORD pwHeight)
{
    BYTE LOCAL_TYPE ucBank;
    WORD LOCAL_TYPE wWidth;
    WORD LOCAL_TYPE wHeight;
    WORD LOCAL_TYPE vtotal;
    wWidth = pwWidth;
    wHeight = pwHeight;
    g_bLockDlc = 0;
    g_dwFactory = TOTAL_PIXEL / 8;
    g_dwFactory_7 = (g_dwFactory *7 / 8);
    g_dwFactory_9 = (g_dwFactory + (g_dwFactory / 8));
    g_dwLstep = g_dwFactory * 1 / 16 * 12 / 10;
    g_dwHstep = g_dwFactory * 3 / 8;
    //printf("TOP_LIMIT = 0x%x\r\n", TOP_LIMIT);
    //printf("DOWN_LIMIT = 0x%x\r\n", DOWN_LIMIT);
    /*
    printf("TOTAL_PIXEL = 0x%x", dwTotalPixel>>16);
    printf("%x\r\n", dwTotalPixel);
    printf("FACTORY = 0x%x", dwFactory>>16);
    printf("%x\r\n", dwFactory);
    printf("FACTORY7 = 0x%x", g_dwFactory_7>>16);
    printf("%x\r\n", g_dwFactory_7);
    printf("FACTORY9 = 0x%x", g_dwFactory_9>>16);
    printf("%x\r\n", g_dwFactory_9);
    printf("Lstep = 0x%x", g_dwLstep>>16);
    printf("%x\r\n", g_dwLstep);
    printf("Hstep = 0x%x", g_dwHstep>>16);
    printf("%x\r\n", g_dwHstep);
     */
    ucBank = msReadByte(BK0_00);
    // get vtotal// 2005/1/24
    msWriteByte(BK0_00, 0x00);
    vtotal = msReadByte(BK0_95) + (msReadByte(BK0_96) &0x07) *256;
    // 
    msWriteByte(BK0_00, 0x03);
    //msWriteByte(BK3_B6, TOP_LIMIT);
    // msWriteByte(BK3_B7, vtotal/8-1);// 2005/1/24
    msWriteByte(BK3_BC, 0x00); // xxxx 0909
    //    msWriteByte(BK3_B5, BIT0);  // Enable range for Histogram
    #if 0//EnablePanelServiceMenu
    if ((PanelWidth==1280&&PanelHeight==720)||(PanelWidth==1366&&PanelHeight==768))
    	{
	    msWriteByte(BK3_B6, 0x00);
	    msWriteByte(BK3_B7, PanelHeight/8); // 2005/1/24
    	}
    else
    	{
	    msWriteByte(BK3_B6, 0x20);
	    msWriteByte(BK3_B7, 0x70); // 2005/1/24
	}
    #else
    #if ((PanelWidth==1280&&PanelHeight==720)||(PanelWidth==1366&&PanelHeight==768))
    msWriteByte(BK3_B6, 0x00);
    msWriteByte(BK3_B7, PanelHeight/8); // 2005/1/24
    #else
    msWriteByte(BK3_B6, 0x20);
    msWriteByte(BK3_B7, 0x70); // 2005/1/24
    #endif
   #endif
    msWriteByte(BK3_B5, BIT0); // Enable range for Histogram
    msWriteByte(BK3_B9, 0x10);
    //msWriteByte(BK3_B8, _BIT6);
    msWriteByte(BK0_00, ucBank);
}
extern Bool SupportMWE(void);
void msDlcOnOff(BYTE pbSwitch)
{
    BYTE LOCAL_TYPE ucBank;
    BYTE LOCAL_TYPE bSwitch;
    WORD LOCAL_TYPE time_out;
    bSwitch = pbSwitch;
    //bWindow=SUB_WINDOW;
    #if 0
        if (!SupportMWE())
            return ;
    #endif 
    //bWindow=1;
    ucBank = msReadByte(BK0_00);
    // wait output v blank
    msWriteByte(BK0_00, 0x00); // bank 0
    time_out = 10000;
    while (!(msReadByte(0xE0) &0x01) && (time_out--))
        ;
    // waiting for V active area
    time_out = 10000;
    while ((msReadByte(0xE0) &0x01) && (time_out--))
        ;
    // waiting for V blanking
    //
    msWriteByte(BK0_00, REG_BANK_PIP);
    //if (bWindow == MAIN_WINDOW)
    //if (0)
    //{
    //	if (bSwitch == _ENABLE)
    //	   msWriteByte(BK3_B8, msReadByte(BK3_B8) | _BIT7);
    //	else
    //	   msWriteByte(BK3_B8, msReadByte(BK3_B8) & ~_BIT7);
    //}
    //else // Sub-Window
    {
        if (bSwitch == _ENABLE)
        {
            msWriteByte(BK3_B8, msReadByte(BK3_B8) | BIT6);
            g_bDLCOnOff = 1;
            g_bNeedRequest = 1;
            DLCStatus = DLC_WAIT;
            if(UserPrefDcrMode)
            {
                BrightData = 0xff;
                PreBrightData = 0xff;
                DarkData = 0x00;
                ReadFrameDataCnt = 1;
                SetDarkestFlag = 0;
            }
        }
        else
        {
            msWriteByte(BK3_B8, msReadByte(BK3_B8) &~BIT6);
            g_bDLCOnOff = 0;
        }
    }
    msWriteByte(BK0_00, ucBank);
}
static XDATA BYTE s_Slope2, s_Slope5;
#define DYNAMIC_DLC	1
BYTE XDATA DLCStatus;
#ifdef TSUMXXNX
WORD XDATA g_u32_HSLU[32];
#endif
WORD XDATA uwArray[8];
BYTE XDATA ucTmp;

#define DCRNew  1   // 081204
#define DCRMotionTest   1 // 081205
#if DCRNew
BYTE xdata PreAveLumi, PreAveLumi2;
#endif
#if DCRMotionTest
#define DCRStep     12//8 // DCR increasement/decreasement step
#define DCRStepTransPeriod  2 // unit: 10ms
BYTE xdata DCRStepTransValue; // 
BYTE xdata BackLightTargetValue; // 
BYTE xdata DCRStepTransCnt; // unit: DCRStepTransPeriod*10ms
bit bDCRSmoothMotionFlag = 0;
#endif

#ifdef TSUMXXNX
void msDlcHandler()
{
    BYTE LOCAL_TYPE ucBank; //, ucTmp;
    //WORD retry = 500;

    if (!g_bDLCOnOff)
    {
        return ;
    }
    
    ucBank = msReadByte(BK0_00);
    msWriteByte(BK0_00, REG_BANK_PIP);

    
    //if( MAIN_WINDOW == bWindow ) // Main window
    //if( 0 ) // Main window
    //{
    //    msWriteByte(BK3_B8, msReadByte(BK3_B8) | _BIT2 | _BIT1);
    //}
    //else // Sub window
    {
        if (g_bNeedRequest)
        {
            msWriteByte(BK3_B8, msReadByte(BK3_B8) | BIT4 | BIT2 | BIT0);	//msWriteByte(BK3_B8, msReadByte(BK3_B8) | BIT4 | BIT2 | BIT0);
            g_bNeedRequest = 0;
        }
    }
    
    if(UserPrefDcrMode)
    {
        if(ReadFrameDataCnt%2 == 0) // set to lower of panel height
        {
            mStar_WriteByte(BK3_B6, TopLimit_Lower);
            mStar_WriteByte(BK3_B7, DownLimit_Lower);
        }
        else // set to upper of panel height
        {
            mStar_WriteByte(BK3_B6, TopLimit_Upper);
            mStar_WriteByte(BK3_B7, DownLimit_Upper);
        }
    }
    
    if (!(msReadByte(BK3_B8) &BIT3))                // 2004/11/19
    {
        if(!(msReadByte(BK3_B8)&BIT2))
            msWriteByte(BK3_B8, msReadByte(BK3_B8)|BIT4|BIT2|BIT0);  // Sub window
            
        msWriteByte(BK0_00, ucBank);
        return ;
    }

    #ifdef TSUMXXNX	
    msWriteByte(BK3_A9, 0);

    for (ucTmp = 0; ucTmp < 32; ucTmp++)
                g_u32_HSLU[ucTmp] = ((WORD) msReadByte(BK3_AA)) * 0x100 + (WORD) msReadByte(BK3_AA);

    for (ucTmp=0; ucTmp<8; ucTmp++)
    {	
        DWORD ucPixels;

        ucPixels=0;
        ucPixels=((DWORD)g_u32_HSLU[(ucTmp*4)]+g_u32_HSLU[(ucTmp*4)+1]+g_u32_HSLU[(ucTmp*4)+2]+g_u32_HSLU[(ucTmp*4)+3]);

        if (ucPixels<=0xFFFF)
            uwArray[ucTmp]=ucPixels;
        else
            uwArray[ucTmp]=0xFFFF;
    }
    #else
    msWriteByte(BK3_B9, 0);
	  
    for (ucTmp = 0; ucTmp < 8; ucTmp++)
        uwArray[ucTmp] = ((WORD) msReadByte(BK3_BA)) * 0x100 + (WORD) msReadByte(BK3_BA); // Reading table    
    #endif
    
    msWriteByte(BK3_B8, msReadByte(BK3_B8) & 0xF0);   

    {     
        DWORD PixelCount,LumCount;
        BYTE AveLumi ,AveLumi2;             

        // 081117
        if(ReadFrameDataCnt%2 ==0) // get all display data
        {
            BYTE tempVal;
            
            if((tempVal = mStar_ReadByte(0x3a)) > BrightData) // get max bright data
                BrightData = tempVal;
            if((tempVal = mStar_ReadByte(0x39)) < DarkData) // get min dark data
                DarkData = tempVal;
            if(abs(BrightData - DarkData) <= BlackWhiteDifference) 
            {
                if(abs(DarkData - DarkestValue) <= DarkestRange) // all display data are dark
                {
                    SetDarkestFlag = 1;
                }
            }

            PixelCount=mStar_ReadWord(0x35);
            LumCount=mStar_ReadWord(0x37);
            AveLumi=(LumCount*256)/PixelCount;
            AveLumi = ((WORD)PreAveLumi + AveLumi)/2;

            
            PixelCount=mStar_ReadWord(0x35);
            LumCount=mStar_ReadWord(0x37);
            AveLumi2=(LumCount*256)/PixelCount;
            AveLumi2 = ((WORD)PreAveLumi2+AveLumi2)/2;
            
            if(abs(AveLumi2-AveLumi)>10)
            {
                //msWriteByte(BK3_B8, msReadByte(BK3_B8) &0xF0);
                //g_bNeedRequest = 1;
                //DLCStatus = DLC_WAIT;
                msWriteByte(BK0_00, ucBank);
                ReadFrameDataCnt ++; // 090504 addition
                return;
            }

            BackLightTargetValue = AveLumi;
            
            if(abs(BackLightTargetValue-BacklightPrev) >= DCRStep)
            {
                bDCRSmoothMotionFlag = 1;
                DCRStepTransValue = abs(BackLightTargetValue-BacklightPrev)/DCRStep;
            }
            else
            {
                bDCRSmoothMotionFlag = 0;
            }

            if(bDCRSmoothMotionFlag) // DCR operating range is over DCRStep
            {
                if(BackLightTargetValue >= BacklightPrev) // increasement
                {
                    BacklightNow = (BacklightNow+DCRStepTransValue >= BackLightTargetValue)?(BackLightTargetValue):(BacklightNow+DCRStepTransValue);
                }
                else // decreasement
                {
                    BacklightNow = (BacklightNow-DCRStepTransValue <= BackLightTargetValue)?(BackLightTargetValue):(BacklightNow-DCRStepTransValue);
                }
            }
            else
            {
                BacklightNow = BackLightTargetValue;
            }           

            //=====Get the Histogram value=============   
            #if DCRMotionTest
            if(!DCRStepTransCnt)
            #endif
            {
                if((BacklightNow != BacklightPrev) || (BrightData != PreBrightData))
                {
                    BacklightPrev = BacklightNow;
                    PreBrightData = BrightData;
                    mStar_AdjustDcrBrightness();
                    #if DCRMotionTest
                    DCRStepTransCnt = DCRStepTransPeriod;
                    #endif
                }
            }                
            msWriteByte(0x00, 0x03);
            HistogramPrev = HistogramNow;
            //BacklightPrev = BacklightNow;
            //=============================================================
        }
        else // get upper dark / bright data
        {                
            DarkData = mStar_ReadByte(0x39);
            BrightData = mStar_ReadByte(0x3a);

            PixelCount=mStar_ReadWord(0x35);
            LumCount=mStar_ReadWord(0x37);
            PreAveLumi=(LumCount*256)/PixelCount;
            PixelCount=mStar_ReadWord(0x35);
            LumCount=mStar_ReadWord(0x37);
            PreAveLumi2=(LumCount*256)/PixelCount;
        }
        
        ReadFrameDataCnt ++;
    }

    msWriteByte(BK0_00, ucBank);
    
    if(UserPrefDcrMode)
    {
        //msWriteByte(BK3_B8, msReadByte(BK3_B8) &0xF0);
        //g_bNeedRequest = 1;
        //DLCStatus = DLC_WAIT;
        return;
    }
    //else
        //DLCStatus = DLC_READ2;
                    		                 
    //////	
    uwArray[3] = ((uwArray[0] / 2) + (uwArray[1] / 4) + (uwArray[2] / 4));
    uwArray[4] = ((uwArray[5] / 4) + (uwArray[6] / 4) + (uwArray[7] / 2));

        if (uwArray[3] < FACTORY_7)
        {
            ucTmp = ((FACTORY_7 - uwArray[3])/Lstep)+1;
            if (ucTmp > 10)
                ucTmp = 10;
            g_ucTable[5] = 0x57 - ucTmp;
        }
        else if (uwArray[3] > FACTORY_9)
        {
            ucTmp = ((uwArray[3] - FACTORY_9)/Hstep)+1;
            if (ucTmp > 10)
                ucTmp = 10;
            g_ucTable[5] = 0x57 + ucTmp;
        }
        else
            g_ucTable[5] = 0x57;
    
        //flicker avoid    
        if (s_Slope5==0)
            s_Slope5=0x57;           
			
        if ((g_ucTable[5]> s_Slope5)&&((g_ucTable[5]- s_Slope5)>=2))
            g_ucTable[5]=g_ucTable[5]-1;
        else if ((s_Slope5> g_ucTable[5])&&((s_Slope5- g_ucTable[5])>=2)) 
            g_ucTable[5]=g_ucTable[5]+1;
        else
            g_ucTable[5]= s_Slope5;

        uwArray[3] = uwArray[0];

        g_ucTable[2] = ((g_ucTable[5] - 0x07) * 2 / 5) + 0x07;

        if (uwArray[3] < FACTORY_7)
        {
            ucTmp = ((FACTORY_7 - uwArray[3])/Lstep)+1;
            if (ucTmp > 10)
                ucTmp = 10;
            g_ucTable[2] = g_ucTable[2] - ucTmp;
        }
        else if (uwArray[3] > FACTORY_9)
        {
            ucTmp = ((uwArray[3] - FACTORY_9)/Hstep)+1;
            if (ucTmp > 10)
                ucTmp = 10;
            g_ucTable[2] = g_ucTable[2] + ucTmp;
        }

        //flicker avoid
        if (s_Slope2==0)
            s_Slope2=0x27;
           
        if ((g_ucTable[2]> s_Slope2)&&((g_ucTable[2]- s_Slope2)>=2))
            g_ucTable[2]=g_ucTable[2]-1;
        else if ((s_Slope2> g_ucTable[2])&&((s_Slope2- g_ucTable[2])>=2)) 
            g_ucTable[2]=g_ucTable[2]+1;
        else
            g_ucTable[2]= s_Slope2;

        if (uwArray[4] < FACTORY_7)
        {
            ucTmp = ((FACTORY_7 - uwArray[4])/Lstep)+1;
            if (ucTmp > 8)
                ucTmp = 8;
            g_ucTable[10] = 0xA7 + ucTmp;
        }
        else if (uwArray[4] > FACTORY_9)
        {
            ucTmp = ((uwArray[4] - FACTORY_9)/Hstep)+1;
            if (ucTmp > 8)
                ucTmp = 8;
            g_ucTable[10] = 0xA7 - ucTmp;
        }
        else
            g_ucTable[10] = 0xA7;

        uwArray[4] = uwArray[7];

        g_ucTable[13] = 0xf7 - ((0xf7 - g_ucTable[10]) * 2 / 5);

        if (uwArray[4] < FACTORY_7)
        {
            ucTmp = ((FACTORY_7 - uwArray[4])/Lstep)+1;
            if (ucTmp > 8)
                ucTmp = 8;
            g_ucTable[13] =g_ucTable[13]+ ucTmp;
        }
        else if (uwArray[4] > FACTORY_9)
        {
            ucTmp = ((uwArray[4] - FACTORY_9)/Hstep)+1;
            if (ucTmp > 8)
                ucTmp = 8;
            g_ucTable[13] =g_ucTable[13]- ucTmp;
        }

        s_Slope5 = g_ucTable[5];
        s_Slope2 = g_ucTable[2];

        g_ucTable[1] = (0x07 + g_ucTable[2]) / 2;
        g_ucTable[3] = g_ucTable[5] - ((g_ucTable[5] - g_ucTable[2]) * 2 / 3);
        g_ucTable[4] = g_ucTable[2] + ((g_ucTable[5] - g_ucTable[2]) * 2 / 3);
        g_ucTable[7] = g_ucTable[5] + ((g_ucTable[10] - g_ucTable[5]) * 2 / 5);
        g_ucTable[6] = (g_ucTable[5] / 2) + (g_ucTable[7] / 2);
        g_ucTable[8] = g_ucTable[10] - ((g_ucTable[10] - g_ucTable[5]) * 2 / 5);
        g_ucTable[9] = (g_ucTable[8] / 2) + (g_ucTable[10] / 2);
        g_ucTable[11] = g_ucTable[13] - ((g_ucTable[13] - g_ucTable[10]) * 2 / 3);
        g_ucTable[14] = (g_ucTable[13] / 2) + (0xf7 / 2);
        g_ucTable[12] = g_ucTable[10] + ((g_ucTable[13] - g_ucTable[10]) * 2 / 3);

        g_ucTable[0] = (0x07 + (g_ucTable[1] / 3)) / 2 ; // fine tune lowest level
	 
	 g_ucTable[15] = 0xff-((0xff-g_ucTable[14])/3);
        g_ucTable[15]= ((g_ucTable[15])/2)+(0xff/2) ; // fine tune highest level

        g_ucTable[0] = g_ucTable[0]+g_ucLumaCurve[0]-0x07;
        g_ucTable[1] = g_ucTable[1]+g_ucLumaCurve[1]-0x17;
        g_ucTable[2] = g_ucTable[2]+g_ucLumaCurve[2]-0x27;
        g_ucTable[3] = g_ucTable[3]+g_ucLumaCurve[3]-0x37;
        g_ucTable[4] = g_ucTable[4]+g_ucLumaCurve[4]-0x47;
        g_ucTable[5] = g_ucTable[5]+g_ucLumaCurve[5]-0x57;
        g_ucTable[6] = g_ucTable[6]+g_ucLumaCurve[6]-0x67;
        g_ucTable[7] = g_ucTable[7]+g_ucLumaCurve[7]-0x77;
        g_ucTable[8] = g_ucTable[8]+g_ucLumaCurve[8]-0x87;
        g_ucTable[9] = g_ucTable[9]+g_ucLumaCurve[9]-0x97;
        g_ucTable[10] = g_ucTable[10]+g_ucLumaCurve[10]-0xa7;
        g_ucTable[11] = g_ucTable[11]+g_ucLumaCurve[11]-0xb7;
        g_ucTable[12] = g_ucTable[12]+g_ucLumaCurve[12]-0xc7;
        g_ucTable[13] = g_ucTable[13]+g_ucLumaCurve[13]-0xd7;
        g_ucTable[14] = g_ucTable[14]+g_ucLumaCurve[14]-0xe7;
        g_ucTable[15] = g_ucTable[15]+g_ucLumaCurve[15]-0xf7;
    
        for(ucTmp=0; ucTmp<16; ucTmp++)
        {
            if(g_ucTable[ucTmp]>0x1FF)
                g_ucTable[ucTmp]=0x00;
            else if(g_ucTable[ucTmp]>0xFF)
                g_ucTable[ucTmp]=0xFF;
        }

        #if !DYNAMIC_DLC// 2004/11/10
        g_ucTable[0] = g_ucLumaCurve[0];
        g_ucTable[1] = g_ucLumaCurve[1];
        g_ucTable[2] = g_ucLumaCurve[2];
        g_ucTable[3] = g_ucLumaCurve[3];
        g_ucTable[4] = g_ucLumaCurve[4];
        g_ucTable[5] = g_ucLumaCurve[5];
        g_ucTable[6] = g_ucLumaCurve[6];
        g_ucTable[7] = g_ucLumaCurve[7];
        g_ucTable[8] = g_ucLumaCurve[8];
        g_ucTable[9] = g_ucLumaCurve[9];
        g_ucTable[10] = g_ucLumaCurve[10];
        g_ucTable[11] = g_ucLumaCurve[11];
        g_ucTable[12] = g_ucLumaCurve[12];
        g_ucTable[13] = g_ucLumaCurve[13];
        g_ucTable[14] = g_ucLumaCurve[14];
        g_ucTable[15] = g_ucLumaCurve[15];
	#endif
		
        msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
        msWriteByte(BK3_B9, 0x10);
        for(ucTmp=0; ucTmp<=0x0f; ucTmp++)
        {
            msWriteByte(BK3_BA, g_ucTable[ucTmp]);
        }

        msWriteByte(BK0_00, REG_BANK_PIP);

        //if (bWindow==MAIN_WINDOW)
            //msWriteByte(BK3_B8, msReadByte(BK3_B8)|_BIT4|_BIT2|_BIT1);  // Main window
        //else
            msWriteByte(BK3_B8, msReadByte(BK3_B8)|BIT4|BIT2|BIT0);  //msWriteByte(BK3_B8, msReadByte(BK3_B8)|_BIT4|_BIT2|_BIT0);

        msWriteByte(BK0_00, ucBank);	
}
#else
void msDlcHandler()
{
    BYTE LOCAL_TYPE ucBank; //, ucTmp;
    WORD retry = 500;
    BYTE tempvalue;
    //BYTE Backlight_Final;
    #if 0
        if (!SupportMWE())
            return ;
    #endif 
    if (!g_bDLCOnOff)
    {
        return ;
    }
    //bWindow=1;
    ucBank = msReadByte(BK0_00);
    msWriteByte(BK0_00, REG_BANK_PIP);
    //if( MAIN_WINDOW == bWindow ) // Main window
    //if( 0 ) // Main window
    //{
    //    msWriteByte(BK3_B8, msReadByte(BK3_B8) | _BIT2 | _BIT1);
    //}
    //else // Sub window
    {
        if (g_bNeedRequest)
        {
            msWriteByte(BK3_B8, msReadByte(BK3_B8) | BIT4 | BIT2 | BIT0);
            g_bNeedRequest = 0;
        }
    }
    
    switch (DLCStatus)
    {
        case DLC_WAIT:
            if(UserPrefDcrMode)
            {
                if(ReadFrameDataCnt%2 == 0) // set to lower of panel height
                {
                    mStar_WriteByte(BK3_B6, TopLimit_Lower);
                    mStar_WriteByte(BK3_B7, DownLimit_Lower);
                }
                else // set to upper of panel height
                {
                    mStar_WriteByte(BK3_B6, TopLimit_Upper);
                    mStar_WriteByte(BK3_B7, DownLimit_Upper);
                }
            }
            #if DYNAMIC_DLC// 2004/11/10
            if (!(msReadByte(BK3_B8) &BIT3))                // 2004/11/19
            {
                msWriteByte(BK0_00, ucBank);
                return ;
            }
                    //while (!(msReadByte(BK3_B8) & _BIT3));  // Wait until ready// 2004/11/3
            #endif 
            DLCStatus = DLC_READ1;
            msWriteByte(BK0_00, ucBank);
            return ;
        case DLC_READ1:
            msWriteByte(BK3_B9, 0);
            #if DYNAMIC_DLC// 2004/11/10
            // 8 segment pixel count extraction
            // Sequencial read
            //=======Define the capture area===========
            // msWriteByte(BK3_B5,0x01);
            // msWriteByte(BK3_B6,0x030);// start
            // msWriteByte(BK3_B6,0x070);// start
            //===================================AutoBacklight function Start=====================
            tempvalue = msReadByte(BK3_B8);
            msWriteByte(BK3_B8, 0x16);
            while (retry-- && !(mStar_ReadByte(BK3_B8) &BIT3))
                ;
            for (ucTmp = 0; ucTmp < 1; ucTmp++)                // 2004/11/3
                uwArray[ucTmp] = ((WORD)msReadByte(BK3_BA)) *0x100 + (WORD)msReadByte(BK3_BA);
            #endif 
            {
                #if DCRNew        
                DWORD PixelCount,LumCount;
                BYTE AveLumi ,AveLumi2;
                #else //!DCRNew
        
                WORD reduceAry;
                BYTE CurrentVal;
                BYTE OffsetlVal, OffsetlBL;
                BYTE noise_th = 3;//8;   //20151218 lixw
                #if 1
                BYTE BL_HIGH = 0xFF;
                BYTE BL_LOW = 0;
                #else 
                BYTE BL_HIGH = UserprefAutolightHigh; // 0xFF;
                BYTE BL_LOW = UserprefAutolightLow; //0x00;
                #endif 
                UserprefALha = 60;
                reduceAry = uwArray[0];                
                #endif // end of DCRNew                

                // 081117
                if(ReadFrameDataCnt%2 ==0) // get all display data
                {
                    BYTE tempVal;
                    
                    if((tempVal = mStar_ReadByte(0x3a)) > BrightData) // get max bright data
                        BrightData = tempVal;
                    if((tempVal = mStar_ReadByte(0x39)) < DarkData) // get min dark data
                        DarkData = tempVal;
                    if(abs(BrightData - DarkData) <= BlackWhiteDifference) 
                    {
                        if(abs(DarkData - DarkestValue) <= DarkestRange) // all display data are dark
                        {
                            SetDarkestFlag = 1;
                        }
                    }

                    #if DCRNew // 081204        
                    PixelCount=mStar_ReadWord(0x35);
                    LumCount=mStar_ReadWord(0x37);
                    AveLumi=(LumCount*256)/PixelCount;
                    AveLumi = ((WORD)PreAveLumi + AveLumi)/2;

                    
                    PixelCount=mStar_ReadWord(0x35);
                    LumCount=mStar_ReadWord(0x37);
                    AveLumi2=(LumCount*256)/PixelCount;
                    AveLumi2 = ((WORD)PreAveLumi2+AveLumi2)/2;
                    
                    if(abs(AveLumi2-AveLumi)>10)
                    {
                        msWriteByte(BK3_B8, msReadByte(BK3_B8) &0xF0);
                        g_bNeedRequest = 1;
                        DLCStatus = DLC_WAIT;
                        msWriteByte(BK0_00, ucBank);
                        ReadFrameDataCnt ++; // 090504 addition
                        break;
                    }

                    #if DCRMotionTest
                    BackLightTargetValue = AveLumi;
                    
                    if(abs(BackLightTargetValue-BacklightPrev) >= DCRStep)
                    {
                        bDCRSmoothMotionFlag = 1;
                        DCRStepTransValue = abs(BackLightTargetValue-BacklightPrev)/DCRStep;
                    }
                    else
                    {
                        bDCRSmoothMotionFlag = 0;
                    }

                    if(bDCRSmoothMotionFlag) // DCR operating range is over DCRStep
                    {
                        if(BackLightTargetValue >= BacklightPrev) // increasement
                        {
                            BacklightNow = (BacklightNow+DCRStepTransValue >= BackLightTargetValue)?(BackLightTargetValue):(BacklightNow+3/*DCRStepTransValue*/);
                        }
                        else // decreasement
                        {
                            BacklightNow = (BacklightNow-DCRStepTransValue <= BackLightTargetValue)?(BackLightTargetValue):(BacklightNow-DCRStepTransValue);
                        }
                    }
                    else
                    {
                        BacklightNow = BackLightTargetValue;
                    }
                    
                    #else // !DCRMotionTest
                    BacklightNow = AveLumi;
                    #endif
                        
                    if(0)//(abs(BacklightNow - BacklightPrev) < 3)
                    {
                        msWriteByte(BK3_B8, msReadByte(BK3_B8) &0xF0);
                        g_bNeedRequest = 1;
                        DLCStatus = DLC_WAIT;
                        msWriteByte(BK0_00, ucBank);
                        break;
                    }
                    
                    #else // !DCRNew

                    CurrentVal = (reduceAry >> 8) &0xFF;
                    CurrentVal = ((WORD)(CurrentVal) + DarkSegmentData)/2;
                    HistogramNow = CurrentVal;
                    HistogramNow = (WORD)(UserprefALha *HistogramNow + (100-UserprefALha) *HistogramPrev) / 100;
                    OffsetlVal = abs(HistogramNow - HistogramPrev) / 2;
                    if (HistogramNow > HistogramPrev)
                        HistogramNow = HistogramPrev + OffsetlVal;
                    else if (HistogramNow < HistogramPrev)
                        HistogramNow = HistogramPrev - OffsetlVal;
                    //printData("UserprefALha = %d\r\n", UserprefALha);
                    //printData("HistogramNow = %d\r\n",HistogramNow);
                    if (UserprefHistogram1 > HistogramNow)
                        BacklightNow = BL_HIGH;                //  LIGHT
                    else if (UserprefHistogram2 < HistogramNow)
                        BacklightNow = BL_LOW;                // DARK 
                    else
                    BacklightNow = (WORD)((UserprefHistogram2 - HistogramNow)*(BL_HIGH - BL_LOW) / abs(UserprefHistogram2 - UserprefHistogram1)) + BL_LOW;
                    OffsetlBL = abs(BacklightNow - BacklightPrev) / 2;   //2         //20081201  
                    if (BacklightNow > BacklightPrev)
                        BacklightNow = BacklightPrev + OffsetlBL;
                    else if (BacklightNow < BacklightPrev)
                        BacklightNow = BacklightPrev - OffsetlBL;
                    if ((BacklightNow > BacklightPrev) && ((BacklightNow - BacklightPrev) >= noise_th))
                        BacklightNow = BacklightNow - noise_th;
                    else if ((BacklightPrev > BacklightNow) && ((BacklightPrev - BacklightNow) >= noise_th))
                        BacklightNow = BacklightNow + noise_th;
                    else
                        BacklightNow = BacklightPrev;
                    #endif // end of DCRNew               
        
                    //=====Get the Histogram value=============   
                    #if DCRMotionTest
                    if(!DCRStepTransCnt)
                    #endif
                    if((BacklightNow != BacklightPrev) || (BrightData != PreBrightData))
                    {
                        BacklightPrev = BacklightNow;
                        PreBrightData = BrightData;
                        mStar_AdjustDcrBrightness();
                        #if DCRMotionTest
                        DCRStepTransCnt = DCRStepTransPeriod;
                        #endif
                    }
                    msWriteByte(0x00, 0x03);
                    HistogramPrev = HistogramNow;
                    //BacklightPrev = BacklightNow;
                    //=============================================================
                }
                else // get upper dark / bright data
                {
                    #if !DCRNew
                    DarkSegmentData = (reduceAry >> 8) &0xFF;
                    #endif
                        
                    DarkData = mStar_ReadByte(0x39);
                    BrightData = mStar_ReadByte(0x3a);

                    PixelCount=mStar_ReadWord(0x35);
                    LumCount=mStar_ReadWord(0x37);
                    PreAveLumi=(LumCount*256)/PixelCount;
                    PixelCount=mStar_ReadWord(0x35);
                    LumCount=mStar_ReadWord(0x37);
                    PreAveLumi2=(LumCount*256)/PixelCount;
                }
                
                ReadFrameDataCnt ++;
            }
            // 081109 modified for it's unnecessary to execute others case below when DCR on.
            //DLCStatus = DLC_READ2;
            if(UserPrefDcrMode)
            {
                msWriteByte(BK3_B8, msReadByte(BK3_B8) &0xF0);
                g_bNeedRequest = 1;
                DLCStatus = DLC_WAIT;
            }
            else
                DLCStatus = DLC_READ2;
            
            msWriteByte(BK0_00, ucBank);
            //  printData("C7 %d",msReadByte(0xC7));
            return ;
            break;
        case DLC_READ2:
            msWriteByte(BK3_B9, 2);
            #if DYNAMIC_DLC// 2004/11/10
                // 8 segment pixel count extraction
                // Sequencial read
                for (ucTmp = 1; ucTmp < 2; ucTmp++)
                // 2004/11/3
                    uwArray[ucTmp] = ((WORD)msReadByte(BK3_BA)) *0x100 + (WORD)msReadByte(BK3_BA);
                // 2004/11/3
                //for(ucTmp=0; ucTmp<8; ucTmp++)
                //     printData("count = 0x%x\r\n", uwArray[ucTmp]);
            #endif 
            DLCStatus = DLC_READ3;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_READ3:
            msWriteByte(BK3_B9, 4);
            #if DYNAMIC_DLC// 2004/11/10
                // 8 segment pixel count extraction
                // Sequencial read
                for (ucTmp = 2; ucTmp < 3; ucTmp++)
                // 2004/11/3
                    uwArray[ucTmp] = ((WORD)msReadByte(BK3_BA)) *0x100 + (WORD)msReadByte(BK3_BA);
                // 2004/11/3
                //for(ucTmp=0; ucTmp<8; ucTmp++)
                //    printf("count = 0x%x\r\n", uwArray[ucTmp]);
            #endif 
            DLCStatus = DLC_READ4;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_READ4:
            msWriteByte(BK3_B9, 6);
            #if DYNAMIC_DLC// 2004/11/10
                // 8 segment pixel count extraction
                // Sequencial read
                for (ucTmp = 3; ucTmp < 4; ucTmp++)
                // 2004/11/3
                    uwArray[ucTmp] = ((WORD)msReadByte(BK3_BA)) *0x100 + (WORD)msReadByte(BK3_BA);
                // 2004/11/3
                //for(ucTmp=0; ucTmp<8; ucTmp++)
                //    printf("count = 0x%x\r\n", uwArray[ucTmp]);
            #endif 
            DLCStatus = DLC_READ5;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_READ5:
            msWriteByte(BK3_B9, 8);
            #if DYNAMIC_DLC// 2004/11/10
                // 8 segment pixel count extraction
                // Sequencial read
                for (ucTmp = 4; ucTmp < 5; ucTmp++)
                // 2004/11/3
                    uwArray[ucTmp] = ((WORD)msReadByte(BK3_BA)) *0x100 + (WORD)msReadByte(BK3_BA);
                // 2004/11/3
                //for(ucTmp=0; ucTmp<8; ucTmp++)
                //    printf("count = 0x%x\r\n", uwArray[ucTmp]);
            #endif 
            DLCStatus = DLC_READ6;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_READ6:
            msWriteByte(BK3_B9, 10);
            #if DYNAMIC_DLC// 2004/11/10
                // 8 segment pixel count extraction
                // Sequencial read
                for (ucTmp = 5; ucTmp < 6; ucTmp++)
                // 2004/11/3
                    uwArray[ucTmp] = ((WORD)msReadByte(BK3_BA)) *0x100 + (WORD)msReadByte(BK3_BA);
                // 2004/11/3
                //for(ucTmp=0; ucTmp<8; ucTmp++)
                //    printf("count = 0x%x\r\n", uwArray[ucTmp]);
            #endif 
            DLCStatus = DLC_READ7;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_READ7:
            msWriteByte(BK3_B9, 12);
            #if DYNAMIC_DLC// 2004/11/10
                // 8 segment pixel count extraction
                // Sequencial read
                for (ucTmp = 6; ucTmp < 7; ucTmp++)
                // 2004/11/3
                    uwArray[ucTmp] = ((WORD)msReadByte(BK3_BA)) *0x100 + (WORD)msReadByte(BK3_BA);
                // 2004/11/3
                //for(ucTmp=0; ucTmp<8; ucTmp++)
                //    printf("count = 0x%x\r\n", uwArray[ucTmp]);
            #endif 
            DLCStatus = DLC_READ8;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_READ8:
            msWriteByte(BK3_B9, 14);
            #if DYNAMIC_DLC// 2004/11/10
                // 8 segment pixel count extraction
                // Sequencial read
                for (ucTmp = 7; ucTmp < 8; ucTmp++)
                // 2004/11/3
                    uwArray[ucTmp] = ((WORD)msReadByte(BK3_BA)) *0x100 + (WORD)msReadByte(BK3_BA);
                // 2004/11/3
                //for(ucTmp=0; ucTmp<8; ucTmp++)
                //    printf("count = 0x%x\r\n", uwArray[ucTmp]);
            #endif 
            DLCStatus = DLC_CAL1;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_CAL1:
            uwArray[3] = ((uwArray[0] / 2) + (uwArray[1] / 4) + (uwArray[2] / 4));
            uwArray[4] = ((uwArray[5] / 4) + (uwArray[6] / 4) + (uwArray[7] / 2));
            //if ((uwArray[3]>tpArray[8]) && ((uwArray[3]-uwArray[8])> FACTORY_19))
            //    g_fcount=2;
            //printf("uwArray[3] = 0x%x\r\n", uwArray[3]);
            //printf("uwArray[8] = 0x%x\r\n", uwArray[8]);
            // g_ucTable[5] value LUT
            //if (g_ucTable[5] == 0)        // this line should be removed!
            //    g_ucTable[5] = 0x57;
            if (uwArray[3] < FACTORY_7)
            {
                ucTmp = ((FACTORY_7 - uwArray[3]) / Lstep) + 1;
                if (ucTmp > 10)
                    ucTmp = 10;
                g_ucTable[5] = 0x57 - ucTmp;
            }
            else if (uwArray[3] > FACTORY_9)
            {
                ucTmp = ((uwArray[3] - FACTORY_9) / Hstep) + 1;
                if (ucTmp > 10)
                    ucTmp = 10;
                g_ucTable[5] = 0x57 + ucTmp;
            }
            else
                g_ucTable[5] = 0x57;
            //flicker avoid    
            if (s_Slope5 == 0)
                s_Slope5 = 0x57;
            DLCStatus = DLC_CAL2;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_CAL2:
            if ((g_ucTable[5] > s_Slope5) && ((g_ucTable[5] - s_Slope5) >= 2))
                g_ucTable[5] = g_ucTable[5] - 1;
            else if ((s_Slope5 > g_ucTable[5]) && ((s_Slope5 - g_ucTable[5]) >= 2))
                g_ucTable[5] = g_ucTable[5] + 1;
            else
                g_ucTable[5] = s_Slope5;
            //uwArray[8] =  uwArray[3];
            //printf("uwArray[3] = 0x%x\r\n",uwArray[3]);
            //printf("g_ucTable[5] = 0x%x\r\n",g_ucTable[5]);
            // g_ucTable[2] value LUT
            uwArray[3] = uwArray[0];
            //if (g_ucTable[2] == 0)  // Can this line be removed
            //    g_ucTable[2] = 0x27;// Can this line be removed
            g_ucTable[2] = ((g_ucTable[5] - 0x07) *2 / 5) + 0x07;
            if (uwArray[3] < FACTORY_7)
            {
                ucTmp = ((FACTORY_7 - uwArray[3]) / Lstep) + 1;
                if (ucTmp > 10)
                    ucTmp = 10;
                g_ucTable[2] = g_ucTable[2] - ucTmp;
            }
            else if (uwArray[3] > FACTORY_9)
            {
                ucTmp = ((uwArray[3] - FACTORY_9) / Hstep) + 1;
                if (ucTmp > 10)
                    ucTmp = 10;
                g_ucTable[2] = g_ucTable[2] + ucTmp;
            }
            //else
            //    g_ucTable[2] = g_ucTable[2];
            //flicker avoid
            if (s_Slope2 == 0)
                s_Slope2 = 0x27;
            DLCStatus = DLC_CAL3;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_CAL3:
            if ((g_ucTable[2] > s_Slope2) && ((g_ucTable[2] - s_Slope2) >= 2))
                g_ucTable[2] = g_ucTable[2] - 1;
            else if ((s_Slope2 > g_ucTable[2]) && ((s_Slope2 - g_ucTable[2]) >= 2))
                g_ucTable[2] = g_ucTable[2] + 1;
            else
                g_ucTable[2] = s_Slope2;
            //printf("uwArray[3] = 0x%x\r\n",uwArray[3]);
            //printf("g_ucTable[2] = 0x%x\r\n",g_ucTable[2]);
            //printf("g_ucTable[6] = 0x%x\r\n", g_ucTable[6]);
            //printf("uwArray[3] = 0x%x\r\n", uwArray[3]);
            //printf("g_ucTable[6] = 0x%x\r\n", g_ucTable[4]);
            //printf("ucSlope[2] = 0x%x\r\n", ucSlope[2]);
            //printf("g_ucTable[6] = 0x%x\r\n", g_ucTable[2]);
            //printf("ucSlope[1] = 0x%x\r\n", ucSlope[1]);
            // g_ucTable[10] value LUT
            //if (g_ucTable[10] == 0)       // this line should be removed
            //    g_ucTable[10] = 0xA7;
            if (uwArray[4] < FACTORY_7)
            {
                ucTmp = ((FACTORY_7 - uwArray[4]) / Lstep) + 1;
                if (ucTmp > 8)
                    ucTmp = 8;
                g_ucTable[10] = 0xA7 + ucTmp;
            }
            else if (uwArray[4] > FACTORY_9)
            {
                ucTmp = ((uwArray[4] - FACTORY_9) / Hstep) + 1;
                if (ucTmp > 8)
                    ucTmp = 8;
                g_ucTable[10] = 0xA7 - ucTmp;
            }
            else
                g_ucTable[10] = 0xA7;
            //printf("uwArray[4] = 0x%x\r\n",uwArray[4]);
            //printf("g_ucTable[10] = 0x%x\r\n",g_ucTable[10]);
            // g_ucTable[13] value LUT
            uwArray[4] = uwArray[7];
            //if (g_ucTable[13] == 0)       // this line should be removed
            //    g_ucTable[13] = 0xd7;
            DLCStatus = DLC_CAL4;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_CAL4:
            g_ucTable[13] = 0xf7 - ((0xf7 - g_ucTable[10]) *2 / 5);
            if (uwArray[4] < FACTORY_7)
            {
                ucTmp = ((FACTORY_7 - uwArray[4]) / Lstep) + 1;
                if (ucTmp > 8)
                    ucTmp = 8;
                g_ucTable[13] = g_ucTable[13] + ucTmp;
            }
            else if (uwArray[4] > FACTORY_9)
            {
                ucTmp = ((uwArray[4] - FACTORY_9) / Hstep) + 1;
                if (ucTmp > 8)
                    ucTmp = 8;
                g_ucTable[13] = g_ucTable[13] - ucTmp;
            }
            //else
            //    g_ucTable[13] =g_ucTable[13];
            s_Slope5 = g_ucTable[5];
            s_Slope2 = g_ucTable[2];
            DLCStatus = DLC_CAL5;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_CAL5:
            #if 0
                if (g_bLockDlc == 1)
                // For test
                {
                    g_ucTable[2] = 0x27;
                    g_ucTable[5] = 0x57;
                    g_ucTable[10] = 0xA7;
                    g_ucTable[13] = 0xd7;
                }
            #endif 
            g_ucTable[1] = (0x07 + g_ucTable[2]) / 2;
            g_ucTable[3] = g_ucTable[5] - ((g_ucTable[5] - g_ucTable[2]) *2 / 3);
            g_ucTable[4] = g_ucTable[2] + ((g_ucTable[5] - g_ucTable[2]) *2 / 3);
            g_ucTable[7] = g_ucTable[5] + ((g_ucTable[10] - g_ucTable[5]) *2 / 5);
            g_ucTable[6] = (g_ucTable[5] / 2) + (g_ucTable[7] / 2);
            g_ucTable[8] = g_ucTable[10] - ((g_ucTable[10] - g_ucTable[5]) *2 / 5);
            g_ucTable[9] = (g_ucTable[8] / 2) + (g_ucTable[10] / 2);
            g_ucTable[11] = g_ucTable[13] - ((g_ucTable[13] - g_ucTable[10]) *2 / 3);
            g_ucTable[14] = (g_ucTable[13] / 2) + (0xf7 / 2);
            g_ucTable[12] = g_ucTable[10] + ((g_ucTable[13] - g_ucTable[10]) *2 / 3);
            DLCStatus = DLC_CAL6;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_CAL6:
            g_ucTable[0] = (0x07 + (g_ucTable[1] / 3)) / 2; // fine tune lowest level
            //if(DLCMode==MODE2_DLC)  // xxxx 0909
            //   g_ucTable[0] = 0x05;
            g_ucTable[15] = 0xff - ((0xff - g_ucTable[14]) / 3);
            g_ucTable[15] = ((g_ucTable[15]) / 2) + (0xff / 2); // fine tune highest level
            g_ucTable[0] = g_ucTable[0] + g_ucLumaCurve[0] - 0x07;
            g_ucTable[1] = g_ucTable[1] + g_ucLumaCurve[1] - 0x17;
            g_ucTable[2] = g_ucTable[2] + g_ucLumaCurve[2] - 0x27;
            g_ucTable[3] = g_ucTable[3] + g_ucLumaCurve[3] - 0x37;
            g_ucTable[4] = g_ucTable[4] + g_ucLumaCurve[4] - 0x47;
            g_ucTable[5] = g_ucTable[5] + g_ucLumaCurve[5] - 0x57;
            g_ucTable[6] = g_ucTable[6] + g_ucLumaCurve[6] - 0x67;
            g_ucTable[7] = g_ucTable[7] + g_ucLumaCurve[7] - 0x77;
            g_ucTable[8] = g_ucTable[8] + g_ucLumaCurve[8] - 0x87;
            g_ucTable[9] = g_ucTable[9] + g_ucLumaCurve[9] - 0x97;
            g_ucTable[10] = g_ucTable[10] + g_ucLumaCurve[10] - 0xa7;
            g_ucTable[11] = g_ucTable[11] + g_ucLumaCurve[11] - 0xb7;
            g_ucTable[12] = g_ucTable[12] + g_ucLumaCurve[12] - 0xc7;
            g_ucTable[13] = g_ucTable[13] + g_ucLumaCurve[13] - 0xd7;
            g_ucTable[14] = g_ucTable[14] + g_ucLumaCurve[14] - 0xe7;
            g_ucTable[15] = g_ucTable[15] + g_ucLumaCurve[15] - 0xf7;
            //printf("ucSlope[6] = 0x%x\r\n",ucSlope[6]);
            //printf("ucSlope[4] = 0x%x\r\n",ucSlope[4]);
            //printf("ucSlope[2] = 0x%x\r\n",ucSlope[2]);
            //printf("ucSlope[8] = 0x%x\r\n",ucSlope[8]);
            //printf("ucSlope[5] = 0x%x\r\n",ucSlope[5]);
            //printf("ucSlope[3] = 0x%x\r\n",ucSlope[3]);
            /*if(DLCMode==MODE3_DLC)  // xxxx 0909
            {
            g_ucTable[0] = 0x05;
            g_ucTable[1] = 0x0f;
            g_ucTable[2] = 0x19;
            g_ucTable[3] = 0x28;
            g_ucTable[4] = 0x36;
            g_ucTable[5] = 0x4a;
            g_ucTable[6] = 0x5f;
            g_ucTable[7] = 0x72;
            g_ucTable[8] = 0x8d;
            g_ucTable[9] = 0xa0;
            g_ucTable[10] = 0xb8;
            g_ucTable[11] = 0xc9;
            g_ucTable[12] = 0xd8;
            g_ucTable[13] = 0xe4;
            g_ucTable[14] = 0xef;
            g_ucTable[15] = 0xf7;
            }*/
            for (ucTmp = 0; ucTmp < 16; ucTmp++)
            {
                if (g_ucTable[ucTmp] > 0x1FF)
                    g_ucTable[ucTmp] = 0x00;
                else if (g_ucTable[ucTmp] > 0xFF)
                    g_ucTable[ucTmp] = 0xFF;
            }
            #if !DYNAMIC_DLC// 2004/11/10
                g_ucTable[0] = g_ucLumaCurve[0];
                g_ucTable[1] = g_ucLumaCurve[1];
                g_ucTable[2] = g_ucLumaCurve[2];
                g_ucTable[3] = g_ucLumaCurve[3];
                g_ucTable[4] = g_ucLumaCurve[4];
                g_ucTable[5] = g_ucLumaCurve[5];
                g_ucTable[6] = g_ucLumaCurve[6];
                g_ucTable[7] = g_ucLumaCurve[7];
                g_ucTable[8] = g_ucLumaCurve[8];
                g_ucTable[9] = g_ucLumaCurve[9];
                g_ucTable[10] = g_ucLumaCurve[10];
                g_ucTable[11] = g_ucLumaCurve[11];
                g_ucTable[12] = g_ucLumaCurve[12];
                g_ucTable[13] = g_ucLumaCurve[13];
                g_ucTable[14] = g_ucLumaCurve[14];
                g_ucTable[15] = g_ucLumaCurve[15];
            #endif 
            DLCStatus = DLC_WRITE1;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_WRITE1:
            msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
            msWriteByte(BK3_B9, 0x10);
            for (ucTmp = 0; ucTmp <= 1; ucTmp++)
            {
                msWriteByte(BK3_BA, g_ucTable[ucTmp]);
            }
            //msWriteByte(BK3_BA, g_ucLumaCurve[ucTmp][uc2]);
            DLCStatus = DLC_WRITE2;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_WRITE2:
            msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
            msWriteByte(BK3_B9, 0x12);
            for (ucTmp = 2; ucTmp <= 3; ucTmp++)
            {
                msWriteByte(BK3_BA, g_ucTable[ucTmp]);
            }
            //msWriteByte(BK3_BA, g_ucLumaCurve[ucTmp][uc2]);
            DLCStatus = DLC_WRITE3;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_WRITE3:
            msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
            msWriteByte(BK3_B9, 0x14);
            for (ucTmp = 4; ucTmp <= 5; ucTmp++)
            {
                msWriteByte(BK3_BA, g_ucTable[ucTmp]);
            }
            //msWriteByte(BK3_BA, g_ucLumaCurve[ucTmp][uc2]);
            DLCStatus = DLC_WRITE4;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_WRITE4:
            msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
            msWriteByte(BK3_B9, 0x16);
            for (ucTmp = 6; ucTmp <= 7; ucTmp++)
            {
                msWriteByte(BK3_BA, g_ucTable[ucTmp]);
            }
            //msWriteByte(BK3_BA, g_ucLumaCurve[ucTmp][uc2]);
            DLCStatus = DLC_WRITE5;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_WRITE5:
            msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
            msWriteByte(BK3_B9, 0x18);
            for (ucTmp = 8; ucTmp <= 9; ucTmp++)
            {
                msWriteByte(BK3_BA, g_ucTable[ucTmp]);
            }
            //msWriteByte(BK3_BA, g_ucLumaCurve[ucTmp][uc2]);
            DLCStatus = DLC_WRITE6;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_WRITE6:
            msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
            msWriteByte(BK3_B9, 0x1A);
            for (ucTmp = 0x0a; ucTmp <= 0x0b; ucTmp++)
            {
                msWriteByte(BK3_BA, g_ucTable[ucTmp]);
            }
            //msWriteByte(BK3_BA, g_ucLumaCurve[ucTmp][uc2]);
            DLCStatus = DLC_WRITE7;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_WRITE7:
            msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
            msWriteByte(BK3_B9, 0x1C);
            for (ucTmp = 0x0c; ucTmp <= 0x0d; ucTmp++)
            {
                msWriteByte(BK3_BA, g_ucTable[ucTmp]);
            }
            //msWriteByte(BK3_BA, g_ucLumaCurve[ucTmp][uc2]);
            DLCStatus = DLC_WRITE8;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        case DLC_WRITE8:
            if (ucDLCCCount < 255)
                ucDLCCCount++;
            msWriteByte(BK0_00, REG_BANK_PIP); // bank 3
            msWriteByte(BK3_B9, 0x1E);
            for (ucTmp = 0x0e; ucTmp <= 0x0f; ucTmp++)
            {
                msWriteByte(BK3_BA, g_ucTable[ucTmp]);
            }
            //msWriteByte(BK3_BA, g_ucLumaCurve[ucTmp][uc2]);
            msWriteByte(BK0_00, REG_BANK_PIP);
            msWriteByte(BK3_B8, msReadByte(BK3_B8) &0xF0);
            //printMsg("reset");
            g_bNeedRequest = 1; // 2004/11/19
            msWriteByte(BK0_00, ucBank);
            DLCStatus = DLC_WAIT;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
        default:
            g_bNeedRequest = 1;
            DLCStatus = DLC_WAIT;
            msWriteByte(BK0_00, ucBank);
            return ;
            break;
    }
}
#endif
void LoadDLCTable(BYTE *pDLCtable) // 2004/10/15
{
    BYTE LOCAL_TYPE index;
    LOCAL_TYPE BYTE *DLCtable;
    DLCtable = pDLCtable;
    // dlc function
    for (index = 0; index < 16; index++)
    {
        g_ucLumaCurve[index] = DLCtable[index];
    }
    g_LowStrength = DLCtable[16];
    g_HighStrength = DLCtable[17];
    msSetDlcStrength(g_LowStrength, g_HighStrength);
    msDlcOnOff(_ENABLE);
}
