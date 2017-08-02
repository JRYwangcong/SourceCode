//******************************************************************************
//  Copyright (c) 2003-2008 MStar Semiconductor, Inc.
//  All rights reserved.
//
//  [Module Name]:
//      DPRxApp.c
//  [Abstract]:
//      This module contains code for DisplayPort receiver's application level
//      procedure and subroutin
//  [Author(s)]:
//      Vincent Kuo
//  [Reversion History]:
//      Initial release:    06 May, 2008
//*******************************************************************************

#define _drvDPRXCOMBOAPP_7_C_
#include "Board.h"
#if ENABLE_DP_INPUT


#include <string.h>
#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "ms_reg.h"
#include "Mode.h"
#include "UserPrefDef.h"
#include "UserPref.h"
#include "Global.h"
#include "Panel.h"
#include "Ms_rwreg.h"
#include "MDebug.h"
#include "Detect.h"
#include "mStar.h"
#include "misc.h"
#include "drvDPRxComboApp.h"
#include <msScaler.h>
#include "DDC2Bi.H"
#include "msACE.h"
#include "NVRam.h"
#include "drvMcu.h"
#include "Common.h"
////////////////////////////////////
//#include "drvMcu.h"
///////////////////////////////////
#include "msflash.h"
#include "GPIO_DEF.h"
#include "Adjust.h"
#if ENABLE_DAC
#include "drvAudio.h"
#endif
#include "msEread.h"
//-------------------------------------------------------------------------------------------------
//  Local Structures & define
//-------------------------------------------------------------------------------------------------
#include "msComboRx.h"

#define    DP_Combo_DEBUG           0
#define    DP_AUXCom_XTAL           1


#if (DP_Combo_DEBUG&&ENABLE_MSTV_UART_DEBUG)
#define DP_printData(str, value)    printData(str, value)
#define DP_printMsg(str)                printMsg(str)
#else
#define DP_printData(str, value)
#define DP_printMsg(str)
#endif

DP_Combo_INFO _DPDATATYPE_ gDPComboInfo[6];
BYTE xdata DP_Combo_Connected[6] ={0};
HBR_IP_SELECT  xdata DP_HBR_0=HBR_NONUSE;
HBR_IP_SELECT  xdata DP_HBR_1=HBR_NONUSE;

void DPRxComboInit()
{

   #if DP_COMBO_PORT
      BYTE i=0;
    WORD DP_Combo_Offset=0;

     for(i=0;i<6;i+=1)
     {
            DP_Combo_Offset = DPRxComboOffset(i);
        //msWrite2Byte( REG_16065C+DP_Combo_Offset,0xFFFF);
         if(DP_Combo_Connected[i] ==0)
            continue;
        DPRxComboSet(i);
        DPRxSetCombo_0(i);
        DPRxSetCombo_1(i);
        //DPRxComboIRQEnable(DISABLE, i);
        DPRxComboIRQEnable(ENABLE, i);
        gDPComboInfo[i].ucState = eComSTATE_INITIAL;
     }
   #endif

}

//**************************************************************************
//  [Function Name]:
//                  DPRxIRQComboEnable(Bool bEnable)
//  [Description]
//                  DPRxIRQComboEnable
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComboIRQEnable( Bool bEnable, BYTE ucComboPort )
{
    WORD DP_Combo_Offset=0;

       DP_Combo_Offset = DPRxComboOffset(ucComboPort);
    msWrite2Byte( REG_16065A+DP_Combo_Offset,0xFFFF);       //Clear
    msWrite2Byte( REG_16065A+DP_Combo_Offset,0x0000);       //Clear
    msWrite2Byte( REG_16065C+DP_Combo_Offset,0xFFFF);       //Mask

    if( bEnable )
    {

       msWriteByteMask(REG_16065C+DP_Combo_Offset , 0 ,BIT0 );            // APLL
       msWriteByteMask(REG_16065C+DP_Combo_Offset , 0 ,BIT1 );            // VPLL
          msWriteByteMask(REG_16065C+DP_Combo_Offset , BIT2 ,BIT2 );      // No Input Detect
          msWriteByteMask(REG_16065C+DP_Combo_Offset , BIT3 ,BIT3 );      // HDCP
          msWriteByteMask(REG_16065C+DP_Combo_Offset , BIT4 ,BIT4 );      // LS Clock Big Change
          msWriteByteMask(REG_16065C+DP_Combo_Offset , BIT5 ,BIT5 );      // OP Ring Buffer
          msWriteByteMask(REG_16065C+DP_Combo_Offset , 0 ,BIT6 );           // TP2
          msWriteByteMask(REG_16065C+DP_Combo_Offset , 0 ,BIT7 );           // TP1

          msWriteByteMask(REG_16065D+DP_Combo_Offset , 0 ,BIT1 );           // Interskew Lose
          msWriteByteMask(REG_16065D+DP_Combo_Offset , 0 ,BIT2 );           // CR Loss
       #if 0
          msWriteByteMask(REG_16065D+DP_Combo_Offset , 0 ,BIT0 );           // Link Rate Change
          msWriteByteMask(REG_16065D+DP_Combo_Offset , 0 ,BIT1 );           // Interskew Lose
          msWriteByteMask(REG_16065D+DP_Combo_Offset , 0 ,BIT2 );           // CR Loss
          msWriteByteMask(REG_16065D+DP_Combo_Offset , BIT3 ,BIT3 );      // Symbol Loss
          msWriteByteMask(REG_16065D+DP_Combo_Offset , BIT4 ,BIT4 );      // Video Change mode
      #endif


         msWriteByteMask( REG_163070, (BIT0 << ucComboPort), (BIT0 << ucComboPort));
        DP_printMsg( " DP Combo IRQ Enable!");
        // Enable interrupt
    }
    else
    {
          msWriteByteMask( REG_163070, 0, (BIT0 << ucComboPort));
         DP_printMsg( " DP Combo IRQ Disable!");
       // Disable interrupt
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPComboInit(Byte ucComboPort)
//  [Description]
//                  DPComboInit
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComboSet(BYTE ucComboPort)
{
    WORD DP_Combo_Offset=0;

     DP_Combo_Offset = DPRxComboOffset(ucComboPort);


  msWriteByte( REG_001100+ucComboPort*COMBO_PHY_OFFSET, 0x11);     // DP11 
  msWriteByte( REG_001101+ucComboPort*COMBO_PHY_OFFSET, 0x0A );    //cyc : 2.7G 			
  msWriteByte( REG_001103+ucComboPort*COMBO_PHY_OFFSET, 0x89);     // Not supprot TP3
  
  //**************************************************************************
  //                    PHY set for Combo
  //**************************************************************************
    msWriteByteMask( REG_112014+ucComboPort*COMBO_PHY_OFFSET, 0, BIT6);         //20/10 bits mode
    msWriteByteMask( REG_11204A+ucComboPort*COMBO_PHY_OFFSET, 0, BIT2);         //AUTO EQ OFF
    msWrite2Byte( REG_1120DC+ucComboPort*COMBO_PHY_OFFSET, 0x03C0 );                 //Override EQ
   // msWrite2Byte( REG_1120E6+ucComboPort*COMBO_PHY_OFFSET, 0x1CE7);                  // EQ=7
   // msWriteByteMask(REG_1120E8+ucComboPort*COMBO_PHY_OFFSET, 0x7, 0x1F);
  //  msWrite2Byte( REG_1120E6+ucComboPort*COMBO_PHY_OFFSET, 0x5294);     	          //  EQ= 20
  //  msWriteByteMask(REG_1120E8+ucComboPort*COMBO_PHY_OFFSET, 0x14, 0x1F);
    msWrite2Byte( REG_1120E6+ucComboPort*COMBO_PHY_OFFSET, ((COMBO_DP_LANE2_EQ<<10)|(COMBO_DP_LANE1_EQ << 5)|COMBO_DP_LANE0_EQ) );                  // EQ=20
    msWriteByteMask(REG_1120E8+ucComboPort*COMBO_PHY_OFFSET, COMBO_DP_LANE3_EQ, 0x1F);
    
    //**************************************************************************
    //                  Combo_Demap
    //**************************************************************************
    msWriteByte( REG_160220+DP_Combo_Offset, 0x21 );                                                             // Htotal Tolerance
    msWriteByte( REG_160207+DP_Combo_Offset, 0x3F );
    msWriteByte( REG_160208+DP_Combo_Offset, 0x1F );
    msWriteByte( REG_160204+DP_Combo_Offset, 0xCA );                                                                //DP Interlace mode // BIT0 =0 (640X480 issue)
    msWriteByteMask( REG_160226+DP_Combo_Offset, BIT7|BIT6, BIT7|BIT6|BIT0 );
    msWriteByte( REG_160201+DP_Combo_Offset, 0x61);                                                                 //(bit3=0 use VB_ID rising as frame flag (end of active))
    msWriteByte( REG_160202+DP_Combo_Offset, 0x07);
    msWriteByte( REG_160203+DP_Combo_Offset, 0x05);									// Bit2 : HB width , bit 0
    msWriteByteMask( REG_160204+DP_Combo_Offset, 0, BIT4);
    msWriteByteMask( REG_160222+DP_Combo_Offset, BIT5, BIT5 );
    msWriteByteMask( REG_160225+DP_Combo_Offset, BIT5, BIT5 );
    msWriteByteMask( REG_160275+DP_Combo_Offset,BIT7,BIT7);                                             //FIFO Reset issue
    //**************************************************************************
    //                  Combo_Link
    //**************************************************************************
    msWriteByte( REG_160318+DP_Combo_Offset, 0xFF );  // mask INT
    msWriteByteMask( REG_160319+DP_Combo_Offset, 0xFF,0xFF );  // mask INT
    msWriteByteMask( REG_16036C+DP_Combo_Offset, BIT2|BIT3|BIT4, BIT2|BIT3|BIT4);
    msWriteByteMask(REG_160310+DP_Combo_Offset,BIT0,BIT0);                                                  //Set Video M/N
    msWriteByteMask(REG_160371+DP_Combo_Offset, BIT4|BIT5, BIT4|BIT5);                              //A/V Big change update to syn
    msWriteByte( REG_160373+DP_Combo_Offset, 0x07);                                                                //use VBID Config
    msWriteByte( REG_160374+DP_Combo_Offset, 0x61);                                                                //use VBID Config
    msWriteByte( REG_160375+DP_Combo_Offset, 0x0F);                                                                //use VBID Config

    msWriteByte( REG_160348+DP_Combo_Offset, 0x68 );           //MVID_CENTER[7:0]
    msWriteByte( REG_160349+DP_Combo_Offset, 0x46 );           //MVID_CENTER[15:8]
    msWriteByte( REG_16034A+DP_Combo_Offset, 0x00 );           //MVID_CENTER[23:16]

    msWriteByte( REG_16034C+DP_Combo_Offset, 0x10 );           //MVID_Range[7:0]
    msWriteByte( REG_16034D+DP_Combo_Offset, 0x00 );           //MVID_Range[15:8]
    msWriteByte( REG_16034E+DP_Combo_Offset, 0x00 );           //MVID_Range[23:16]

    msWriteByte( REG_160350+DP_Combo_Offset, 0x00 );           //NVID_CENTER[7:0]
    msWriteByte( REG_160351+DP_Combo_Offset, 0x80 );           //NVID_CENTER[15:8]
    msWriteByte( REG_160352+DP_Combo_Offset, 0x00 );           //NVID_CENTER[23:16]

    msWriteByte( REG_160354+DP_Combo_Offset, 0x10 );           //NVID_Range[7:0]
    msWriteByte( REG_160355+DP_Combo_Offset, 0x00 );           //NVID_Range[15:8]
    msWriteByte( REG_160356+DP_Combo_Offset, 0x00 );           //NVID_Range[23:16]

    msWriteByte( REG_160358+DP_Combo_Offset, 0x00 );           //MAUD_CENTER[7:0]
    msWriteByte( REG_160359+DP_Combo_Offset, 0x40 );           //MAUD_CENTER[15:8]
    msWriteByte( REG_16035A+DP_Combo_Offset, 0x00 );           //MAUD_CENTER[23:16]

    msWriteByte( REG_16035C+DP_Combo_Offset, 0x10 );           //MAUD_Range[7:0]
    msWriteByte( REG_16035D+DP_Combo_Offset, 0x00 );           //MAUD_Range[15:8]
    msWriteByte( REG_16035E+DP_Combo_Offset, 0x00 );           //MAUD_Range[23:16]

    msWriteByte( REG_160360+DP_Combo_Offset, 0x00 );           //NAUD_CENTER[7:0]
    msWriteByte( REG_160361+DP_Combo_Offset, 0x80 );           //NAUD_CENTER[15:8]
    msWriteByte( REG_160362+DP_Combo_Offset, 0x00 );           //NAUD_CENTER[23:16]
    msWriteByte( REG_160365+DP_Combo_Offset, 0x10 );           //NAUD_Range[7:0]
    msWriteByte( REG_160366+DP_Combo_Offset, 0x00 );           //NAUD_Range[15:8]
    msWriteByte( REG_160367+DP_Combo_Offset, 0x00 );           //NAUD_Range[23:16]
    //**************************************************************************
    //                  Combo_HDCP
    //**************************************************************************
    msWriteByte( REG_160406+DP_Combo_Offset, 0x04 );           //HDCP

    //**************************************************************************
    //                  Combo_Decode
    //**************************************************************************
    msWriteByteMask( REG_160549+DP_Combo_Offset, BIT6|BIT5|BIT3|BIT2, BIT6|BIT5|BIT3|BIT2|BIT1 );
    msWriteByte( REG_160544+DP_Combo_Offset, 0x77);                                                                                 //EQ done is equal to symbol lock
    msWrite2Byte( REG_1605C0+DP_Combo_Offset,0x0168);                                                                                     //CR lock value for RBR
    msWrite2Byte( REG_160500+DP_Combo_Offset,0x1D00);

    msWrite2Byte( REG_160502+DP_Combo_Offset,0x020A);
    msWrite2Byte( REG_160506+DP_Combo_Offset,0x0DA8);
    msWrite2Byte( REG_160508+DP_Combo_Offset,0x0AD4);
    msWrite2Byte( REG_16050A+DP_Combo_Offset,0x096A);
    msWrite2Byte( REG_160512+DP_Combo_Offset,0x0530);
    msWrite2Byte( REG_16051C+DP_Combo_Offset,0x0700);
    msWrite2Byte( REG_160520+DP_Combo_Offset,0x0700);
    msWrite2Byte( REG_160524+DP_Combo_Offset,0x0700);
    msWrite2Byte( REG_160528+DP_Combo_Offset,0x0700);
    msWrite2Byte( REG_160546+DP_Combo_Offset,0x003F);
    msWrite2Byte( REG_16055C+DP_Combo_Offset,0x0009);
    msWrite2Byte( REG_16053C+DP_Combo_Offset,0x030F);
    msWriteByte( REG_160544+DP_Combo_Offset,0x77);
    msWrite2Byte( REG_160548+DP_Combo_Offset,0x1DAA);
    msWriteByte( REG_16053B+DP_Combo_Offset,0x80);
    msWriteByte( REG_1605AA+DP_Combo_Offset,0x80);

    //**************************************************************************
    //                  Combo_Top
    //**************************************************************************
    msWriteByteMask( REG_160660+DP_Combo_Offset, 0x03, 0x0F);     // Disable VBLK override
    msWriteByteMask( REG_160607+DP_Combo_Offset, 0, BIT0);          // AUPLL_Power on
    msWrite2Byte( REG_160600+DP_Combo_Offset,0x7040);
    msWrite2Byte( REG_160610+DP_Combo_Offset,0x0000);
    msWrite2Byte( REG_160614+DP_Combo_Offset,0x0000);
    msWrite2Byte( REG_160630+DP_Combo_Offset,0x0000);
    msWrite2Byte( REG_160652+DP_Combo_Offset,0x0044);             ///20140508
    msWrite2Byte( REG_160660+DP_Combo_Offset,0x0003);
    msWrite2Byte( REG_1606A2+DP_Combo_Offset,0x8001);
    msWrite2Byte( REG_1606AE+DP_Combo_Offset,0x4000);
    msWrite2Byte( REG_160682+DP_Combo_Offset,0x8000);
    msWrite2Byte( REG_1606A8+DP_Combo_Offset,0x0210);
    msWrite2Byte( REG_1606AA+DP_Combo_Offset,0x4334);
    msWrite2Byte( REG_1606AC+DP_Combo_Offset,0x02F1);
    msWrite2Byte( REG_1606A4+DP_Combo_Offset,0x3100);
    msWrite2Byte( REG_160640+DP_Combo_Offset,0x0006);
    msWrite2Byte( REG_1606B4+DP_Combo_Offset,0x4010);
    msWrite2Byte( REG_16064C+DP_Combo_Offset,0xFFFF);
    msWrite2Byte( REG_16064E+DP_Combo_Offset,0x3FFF);
    msWrite2Byte( REG_1606BE+DP_Combo_Offset,0x0000);
    msWrite2Byte( REG_16065A+DP_Combo_Offset,0xFFFE);
    msWrite2Byte( REG_16065A+DP_Combo_Offset,0x0000);
    msWrite2Byte( REG_16065C+DP_Combo_Offset,0xFFFF);
    //**************************************************************************
    //                  Combo_DPCD
    //**************************************************************************
    msWriteByte( REG_160700+DP_Combo_Offset, 0x03);                                                                // [0]local DPCD 1.2 En, [1] CP_ready
    msWriteByte( REG_160702+DP_Combo_Offset, 0x00);                                                                // Set Level 0
    msWriteByte( REG_160703+DP_Combo_Offset, 0x00);                                                                // Set Level 0
    msWriteByteMask( REG_1607E4+DP_Combo_Offset, 0 , BIT3 );                                                 //  Enable SQ check in fast training
    msWriteByte( REG_1607E6+DP_Combo_Offset,0x0E );                                                               //   Xtal clk for Fast train timer


}
void DPRxComboHandle( void )
{
	#if DP_COMBO_PORT	
	
    BYTE ComboPort=0;

     for(ComboPort = 0; ComboPort < 6; ComboPort++ )  // Scaler Scan Port
     {

	if( !(ucDisplayEnableIndex_Temp & (1<<DP_HBR_0)) )
	{
		continue;
	}	

            if(DP_Combo_Connected[ComboPort] ==0)
            continue;

                 DPRxComboHdcpReAuthen(ComboPort);
                 DPRxComboCDRLossHander(ComboPort);
#if LOOPOUT_DPNOSIGNAL_SOLUTION
          		 DPRxComReChkTiming(ComboPort);        //  CCJ 20140528 (invaild Packet) // 140528 DMD Loop Out에서 간헐적 화면 안나옴 개선을 위한 Spot 솔루션 적용
#endif

              if( gDPComboInfo[ComboPort].DPState !=  gDPComboInfo[ComboPort].ucState )
                 {
                        DPComboState( gDPComboInfo[ComboPort].ucState);
                  }

                gDPComboInfo[ComboPort].DPState  =  gDPComboInfo[ComboPort].ucState;

                msWriteByte( REG_16150A, gDPComboInfo[2].ucState);

                switch(gDPComboInfo[ComboPort].ucState )
                   {

                case eComSTATE_INITIAL:
                DPRxComClrSQTime(ComboPort);
                gDPComboInfo[ComboPort].ucState = eComSTATE_CKECKLOCK;
                break;
                case eComSTATE_CKECKLOCK:
                 DPRxComCheckSQTime(1500,ComboPort);
                if( gDPComboInfo[ComboPort].bDPTrainingP0T || gDPComboInfo[ComboPort].bDPTrainingP1T || (DPRxComboCheckLock(ComboPort)) )
                        {
                    DPRxComFastTraining2Enable(FALSE, ComboPort);

                if(DPRxComboCheckLock(ComboPort))
		{
			gDPComboInfo[ComboPort].bDPTrainingP0T = 0;
			gDPComboInfo[ComboPort].bDPTrainingP1T = 0;
			gDPComboInfo[ComboPort].ucState = eComSTATE_MEASURE;
		}
                            else
                            {
                         Delay1ms( 1 );
                         gDPComboInfo[ComboPort].bLockFailCnt++;
                         if(gDPComboInfo[ComboPort].bLockFailCnt > 10)
                         {
                             gDPComboInfo[ComboPort].bDPTrainingP0T = 0;
                             gDPComboInfo[ComboPort].bDPTrainingP1T = 0;
                         }
                            }

                }
                 else if( gDPComboInfo[ComboPort].bSquelchStable && ( !(  gDPComboInfo[ComboPort].bDPTrainingP0T  || gDPComboInfo[ComboPort].bDPTrainingP1T ) )&&( !DPRxComboCheckLock(ComboPort) ) )
                {
                        DPRxComClrSQTime(ComboPort);
                        if(DPComFastLink(ComboPort))
						gDPComboInfo[ComboPort].ucState  = eComSTATE_MEASURE;

                }
                break;
                case eComSTATE_MEASURE:
                gDPComboInfo[ComboPort].ucState = eComSTATE_NORMAL;
				DPRxComboHVFSMCheck(ComboPort);
                break;
                case eComSTATE_NORMAL:
                  if( !DPRxComboCheckLock(ComboPort) )
                     {
				#if DPSSC
				DPAudioSSCInit(ComboPort);
				#endif
				
					gDPComboInfo[ComboPort].bDPTrainingP0T = 0;
					gDPComboInfo[ComboPort].bDPTrainingP1T = 0;
                        DPRxComClrSQTime(ComboPort);
                        gDPComboInfo[ComboPort].ucState = eComSTATE_CKECKLOCK;
                  }
                  else
                  {
				DPRxComboAudioHandle(ComboPort);
				DPRxComboTiming(ComboPort);
				DPRxComboCheckInputFormat(ComboPort);
                  }
                break;
                case eComSTATE_POWERSAVE:

                break;
                case eComSTATE_DPIDLE:

                break;



                }


     }

	#endif
}




//**************************************************************************
//  [Function Name]:
//                  DPRxReChkTiming()
//  [Description]
//                  DPRxReChkTiming
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
// 140528 DMD Loop Out에서 간헐적 화면 안나옴 개선을 위한 Spot 솔루션 적용
#if LOOPOUT_DPNOSIGNAL_SOLUTION
void DPRxComReChkTiming(BYTE ComboPort)
{
	
	if(gDPComboInfo[ComboPort].ucState != eComSTATE_NORMAL)
	{
		if(DPRxComboCheckLock(ComboPort)&&!DPRxComCheckTiming(ComboPort,5)&& !gDPComboInfo[ComboPort].bReChkEnable)
		{									
			 gDPComboInfo[ComboPort].bReChkEnable = 1;
			 gDPComboInfo[ComboPort].uwReChkTiming = 3000;
			 
		}		
		if( gDPComboInfo[ComboPort].bReChkEnable && gDPComboInfo[ComboPort].uwReChkTiming ==0)
		{	
			gDPComboInfo[ComboPort].bReChkEnable =0;
			if(DPRxComboCheckLock(ComboPort)&&!DPRxComCheckTiming(ComboPort,5))
			{
				DP_printMsg("------------------------------------ComboPort==========================Re------check \n");
				DPComRxHPDEnable(DISABLE, ComboPort);
				ForceDelay1ms(100); // delay 1 msec				
				DPComRxHPDEnable(ENABLE, ComboPort);		
	
			}
		}
	}	
}
#endif

//**************************************************************************
//  [Function Name]:
//                  DPRxCheckTiming()
//  [Description]
//                  DPRxCheckTiming
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPRxComCheckTiming(BYTE ComboPort, BYTE Count )
{
    Bool xdata found = FALSE;
    WORD xdata LS_Clock=0;
    WORD xdata Pixel_Clock=0;
    DWORD xdata Base_N=0;
    DWORD xdata Base_M=0;
    WORD DP_Combo_Offset=0;
    DP_Combo_Offset = DPRxComboOffset(ComboPort);
    
    while(Count--)
    {
                 #if  !DP_Combo_DEBUG
                 ForceDelay1ms( 20 );
                 #endif
		LS_Clock = 27 * msReadByte(REG_160780+DP_Combo_Offset);
		DP_printData( "DP Input : LS_Clock %d ", LS_Clock );
		DP_printData( "DP Input : Lane %d ", msReadByte(REG_160781+DP_Combo_Offset)&0x0F);
		Base_M = ((DWORD) msRead2Byte( REG_1603C1+DP_Combo_Offset ) <<8) + msReadByte(REG_1603C0+DP_Combo_Offset);
		DP_printData( "DP Input : Base_M %x ", Base_M );
		Base_N = ((DWORD)msRead2Byte(REG_1603C4+DP_Combo_Offset ) <<8) + msReadByte(REG_1603C3+DP_Combo_Offset);
		DP_printData( "DP Input : Base_N %x ", Base_N );
		Pixel_Clock = ((DWORD) Base_M * LS_Clock ) / Base_N ;
		DP_printData( "DP Input : Pixel_Clock %d ", Pixel_Clock );

                 gDPComboInfo[ComboPort].bDPInterlace = ( msReadByte(REG_16026B+DP_Combo_Offset ) & 0x04 ) ? TRUE : FALSE; // Read BK36 A5 Bit2 Interlance mode


                 gDPComboInfo[ComboPort].uwDPHtotal = msRead2Byte( REG_1603B8+DP_Combo_Offset ) & 0x7FFF;

                if(  gDPComboInfo[ComboPort].bDPInterlace )
                     gDPComboInfo[ComboPort].uwDPVtotal = ( msRead2Byte( REG_1603B6+DP_Combo_Offset ) & 0x7FFF ) << 1;
                else
                     gDPComboInfo[ComboPort].uwDPVtotal = msRead2Byte( REG_1603B6+DP_Combo_Offset ) & 0x7FFF;

                     gDPComboInfo[ComboPort].uwDPHWidth = msRead2Byte( REG_160266+DP_Combo_Offset ) & 0x7FFF;


                if(  gDPComboInfo[ComboPort].bDPInterlace )
                {
                     gDPComboInfo[ComboPort].uwDPVWidth = ( msRead2Byte( REG_1603BE+DP_Combo_Offset ) & 0x7FFF ) << 1;
                }
                else
                {
                     gDPComboInfo[ComboPort].uwDPVWidth = msRead2Byte( REG_1603BE +DP_Combo_Offset) & 0x7FFF;
                     gDPComboInfo[ComboPort].uwDPHStart = msRead2Byte( REG_1603BA+DP_Combo_Offset ) & 0x7FFF;
                     gDPComboInfo[ComboPort].uwDPVStart = msRead2Byte( REG_1603BC+DP_Combo_Offset) & 0x7FFF;
                     gDPComboInfo[ComboPort].uwDPHPWS = msRead2Byte( REG_1603B2+DP_Combo_Offset ) & 0x7FFF;
                     gDPComboInfo[ComboPort].bDPHPol = ( msReadByte( REG_1603B2+DP_Combo_Offset ) & 0x80 ) ? 1 : 0;
                     gDPComboInfo[ComboPort].uwDPVPWS = msRead2Byte( REG_1603B4+DP_Combo_Offset ) & 0x7FFF;
                     gDPComboInfo[ComboPort].bDPVPol = ( msReadByte( REG_1603B4+DP_Combo_Offset ) & 0x80 ) ? 1 : 0;
                }

                  DP_printData( "DP Input : DPHtotal   %d ",  gDPComboInfo[ComboPort].uwDPHtotal );
                  DP_printData( "DP Input : DPVtotal   %d ",  gDPComboInfo[ComboPort].uwDPVtotal );
                  DP_printData( "DP Input : DPHWidth   %d ",  gDPComboInfo[ComboPort].uwDPHWidth );
                  DP_printData( "DP Input : DPVWidth   %d ",  gDPComboInfo[ComboPort].uwDPVWidth );




              if(( gDPComboInfo[ComboPort].uwDPHWidth < 5000 ) && (  gDPComboInfo[ComboPort].uwDPVWidth < 3000 ) && ( Pixel_Clock > 10) && ( Pixel_Clock < 500)
              &&( gDPComboInfo[ComboPort].uwDPHtotal >  gDPComboInfo[ComboPort].uwDPHWidth) && (  gDPComboInfo[ComboPort].uwDPVtotal >  gDPComboInfo[ComboPort].uwDPVWidth))
              {
                    found = TRUE;
                    DP_printMsg( "===> DP timing Pass:" );
                }
                else
                {   DP_printMsg( "===> DP timing Fail:" );
                    found = FALSE;
                 break;
                }
    }
     return found;

}




//**************************************************************************
//  [Function Name]:
//                  DPRxComboCDRLossHander()
//  [Description]
//                  DPRxComboCDRLossHander
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComboCDRLossHander( BYTE ComboPort )
{
      WORD DP_Combo_Offset=0;
        DP_Combo_Offset = DPRxComboOffset(ComboPort);

   if( gDPComboInfo[ComboPort].bDPLoseCDRLockIRQ ) // Lock fail have occured
    {
        if( !(DPRxComboCheckLock(ComboPort)))
        {
             if(( gDPComboInfo[ComboPort].uwCDRHPDCnt== 0 ) && ( (msReadByte( REG_160782 + DP_Combo_Offset  )&0x03) == 0 ) )
            {

              DPComRxHPDEnable(DISABLE, ComboPort);
              Delay1ms( 1);
              DPComRxHPDEnable(ENABLE, ComboPort);                                                                             //CCJ mark 20090925
                       gDPComboInfo[ComboPort].bDPLoseCDRLockIRQ  = FALSE;                                                       //clear flag
                       DP_printMsg("CDR_Loss");
            }
        }
        else
        {
                    gDPComboInfo[ComboPort].bDPLoseCDRLockIRQ  = FALSE;     //clear flag
        }

    }
    if( gDPComboInfo[ComboPort].bDPInterlaneSkewIRQ ) // Lock fail have occured
    {
        if( DPRxComboCheckLock(ComboPort) )
        {
            if(( gDPComboInfo[ComboPort].uwSkewHPDCnt == 0 ) && ((msReadByte( REG_160782 + DP_Combo_Offset  )&0x03) == 0 ) )
            {
                if( ! ( msReadByte( REG_1607C4+ DP_Combo_Offset  )&_BIT7 ) ) // now is not in traning stage
                {

                       DPComRxHPDEnable(DISABLE, ComboPort);
              Delay1ms( 1);
              DPComRxHPDEnable(ENABLE, ComboPort);
                       DP_printMsg("Interlane_Loss");

                     gDPComboInfo[ComboPort].bDPInterlaneSkewIRQ = FALSE; //clear flag
                }
                else
                {
                     gDPComboInfo[ComboPort].bDPInterlaneSkewIRQ = FALSE; //clear flag

                }
            }
        }
        else
        {
              gDPComboInfo[ComboPort].bDPInterlaneSkewIRQ = FALSE; //clear flag

        }
    }




}
//**************************************************************************
//  [Function Name]:
//                  DPRxComboHdcpReAuthen()
//  [Description]
//                  DPRxComboHdcpReAuthen
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComboHdcpReAuthen( BYTE ComboPort )
{
      WORD DP_Combo_Offset=0;
          DP_Combo_Offset = DPRxComboOffset(ComboPort);

	   if(((msReadByte(REG_160782+DP_Combo_Offset)&0x03)==0) && DPRxCheckLock()&&(msReadByte(REG_1607C0+DP_Combo_Offset)&BIT2))
          {
                if(gDPComboInfo[ComboPort].bDPHDCPIRQ ==0)
               {
                    gDPComboInfo[ComboPort].DPHDCPCnt = 1000;
                    gDPComboInfo[ComboPort].bDPHDCPIRQ = 1;
               }

               if(gDPComboInfo[ComboPort].bDPHDCPIRQ && ( gDPComboInfo[ComboPort].DPHDCPCnt ==0))
              {
                    DPComRxHPDEnable(DISABLE, ComboPort);
                    Delay1ms( 5);
                    DPComRxHPDEnable(ENABLE, ComboPort);
                    gDPComboInfo[ComboPort].DPHDCPCnt  = 0;
                }
        }
       else
      {
               gDPComboInfo[ComboPort].DPHDCPCnt  = 0;

      }


}
//**************************************************************************
//  [Function Name]:
//                  DPRxAudioHandle()
//  [Description]
//                  DPRxAudioHandle
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************

#define On_Debunce  3
#define Off_Debunce  80
void DPRxComboAudioHandle( BYTE ComboPort )
{
    #if 1
    //BYTE _DPDATATYPE_ DP_AudioDebunce_On = 0;
    //BYTE _DPDATATYPE_ DP_AudioDebunce_Off = 0;
    DWORD _DPDATATYPE_ DPAUM0 = 0;
    DWORD _DPDATATYPE_ DPAUM1 = 0;
    DWORD _DPDATATYPE_ DPAUM2 = 0;
    DWORD _DPDATATYPE_ DPAUN0 = 0;
    DWORD _DPDATATYPE_ DPAUN1 = 0;
    DWORD _DPDATATYPE_ DPAUN2 = 0;
    BYTE _DPDATATYPE_ DPFBDIV0 = 0;
    BYTE _DPDATATYPE_ DPFBDIV1 = 0;
    BYTE _DPDATATYPE_ DPKP = 0;
    BYTE _DPDATATYPE_ DPKM = 0;
    BYTE _DPDATATYPE_ DPFREQ0 = 0;
    BYTE _DPDATATYPE_ DPFREQ1 = 0;
    BYTE _DPDATATYPE_ DPFREQ2 = 0;
    DWORD _DPDATATYPE_ DPAUM = 0;
    DWORD _DPDATATYPE_ DPAUN = 0;
    DWORD _DPDATATYPE_ DPFREQ = 0;
    DWORD _DPDATATYPE_ DPFBDIV = 0;

    //DWORD _DPDATATYPE_ DPAUM_Temp = 0;
    //DWORD _DPDATATYPE_ DPAUN_Temp= 0;
    //BYTE   _DPDATATYPE_ DPAudio_Cnt=0;
    WORD   HLBR=0 ,LinkRate =0;
	
    #endif


     WORD DP_Combo_Offset=0;
     DP_Combo_Offset = DPRxComboOffset(ComboPort);

      #if DPPacketMute	
	if(gDPComboInfo[ComboPort].bPacketTime ==0)
	{
		gDPComboInfo[ComboPort].bPacket =   msReadByte(REG_1603F9 + DP_Combo_Offset);
		gDPComboInfo[ComboPort].bPacketTime =100;
	       msWriteByteMask(REG_16033A+ DP_Combo_Offset,BIT0,BIT0);
	}
    #endif		

      if(!gDPCombo[DP_HBR_0].AudioEn)
      return;

       #if DPPacketMute		
	if((gDPComboInfo[ComboPort].bPacket & 0x06)== 0x06)
	{
			#if !DPSSC
			DisableDPMute();
   	              DPRxAudioMuteEn =0;
			#endif
			//DP_printMsg("\r\n=====>   Unmute ");
	}
	else
	{	
			if(msReadByte(REG_1630B1)&(BIT0|BIT1))
			{	
				EnableDPMute();
				 DPRxAudioMuteEn = 1;
				#if DPSSC  
				DPAudioSSCInit(ComboPort);
				#endif
				//DP_printMsg("\r\n=====>   ++mute ");
			}		
	}
     #endif	

#if DPSSC     
    if((DPRxAudioCnt ==0)&&((gDPComboInfo[ComboPort].bPacket & 0x06)== 0x06))
    {
    	DPRxAudioCnt =5;
	DPAudioSSCAdjust();
    }	
 #endif	    
      DPComboAudioMuteCheck(ComboPort);

      gDPComboInfo[ComboPort].bDPNoAudioStrm = ( msReadByte( REG_16026B+ DP_Combo_Offset ) & 0x10 ) ? 1 : 0; // Read BK36 A5 Bit4:1 NoAudioStrm

    if(gDPComboInfo[ComboPort].bDPLANELOCK  == TRUE )
    {
              HLBR = msReadByte( REG_160780 + DP_Combo_Offset);

            if(( HLBR & 0x0F ) == 0x06 )
            {
                LinkRate = 162;
            }
            else
            {
                LinkRate = 270;
            }
         //################  Audio M/N example ###################
            //M=512*fs*N/LinkRate
            //N=0x8000=32768
            //32K,2.7G   => M=512*32K*32768/270000K=1988.4=0x07_C4
            //48K,2.7G   => M=512*48K*32768/270000K=2982.6=0x0B_A6
            //32K,1.62G => M=512*32K*32768/162000K=3314.0=0x0C_F2
            //48K,1.62G => M=512*48K*32768/162000K=4971.0=0x13_6B
            // ################################################

            msWriteByteMask(REG_160310+ DP_Combo_Offset,0,BIT0);
            DPAUM0 = msReadByte( REG_1603C0+ DP_Combo_Offset ); //MSA MAUD_CENTER[7:0]
            DPAUM1 = msReadByte( REG_1603C1+ DP_Combo_Offset ); //MSA MAUD_CENTER[15:8]
            DPAUM2 = msReadByte( REG_1603C2+ DP_Combo_Offset ); //MSA MAUD_CENTER[23:16]
            DPAUM = ( DPAUM2 << 16 ) + ( DPAUM1 << 8 ) + ( DPAUM0 );
            DPAUN0 = msReadByte( REG_1603C3+ DP_Combo_Offset ); //MSA NAUD_CENTER[7:0]
            DPAUN1 = msReadByte( REG_1603C4+ DP_Combo_Offset ); //MSA NAUD_CENTER[15:8]
            DPAUN2 = msReadByte( REG_1603C5+ DP_Combo_Offset ); //MSA NAUD_CENTER[23:16]
            DPAUN = ( DPAUN2 << 16 ) + ( DPAUN1 << 8 ) + ( DPAUN0 );
            msWriteByteMask(REG_160310+ DP_Combo_Offset,BIT0,BIT0);
         DPFBDIV0 = msReadByte(  REG_162F04); //FBDIV[7:0]
         DPFBDIV1 = msReadByte(  REG_162F05 ); //FBDIV[15:8]
         DPFBDIV = ( DPFBDIV1 << 8 ) + DPFBDIV0;
         DPKM = GetPow2( msReadByte( REG_162F01) & 0x0F ); //KM
         DPKP = GetPow2((msReadByte( REG_162F00 ) >> 4 ) & 0x0F ); //Kp
            gDPComboInfo[ComboPort].DPAUM_OLD =  (msReadByte( REG_16035A+ DP_Combo_Offset ) <<16)  + (msReadByte( REG_160359+ DP_Combo_Offset )<<8) + msReadByte( REG_160358+ DP_Combo_Offset );
            gDPComboInfo[ComboPort].DPAUN_OLD =  (msReadByte(REG_160362+ DP_Combo_Offset ) <<16)  + (msReadByte( REG_160361+ DP_Combo_Offset )<<8) + msReadByte( REG_160360+ DP_Combo_Offset );


            if((abs( DPAUM - gDPComboInfo[ComboPort].DPAUM_OLD) > 0x80)||(DPKM !=  gDPComboInfo[ComboPort].DPAudioKM_OLD)||(DPKP !=  gDPComboInfo[ComboPort].DPAudioKP_OLD)||(DPFBDIV1 != gDPComboInfo[ComboPort].DPAudioFBDIV_OLD))
             {
                     gDPComboInfo[ComboPort].bDPAUPLLBIGChange = TRUE;
                     DP_printData( "\r\nDP_Audio_bigchange_Manual %x ", 0 );
             }

        gDPComboInfo[ComboPort].DPAudioKM_OLD = DPKM;
        gDPComboInfo[ComboPort].DPAudioKP_OLD = DPKP;
        gDPComboInfo[ComboPort].DPAudioFBDIV_OLD = DPFBDIV1;


              if( gDPComboInfo[ComboPort].bDPAUPLLBIGChange  && ( !gDPComboInfo[ComboPort].bDPNoAudioStrm ) )
                {

                            msWriteByte( REG_160358+ DP_Combo_Offset, DPAUM0 ); //UPDATE MAUD_CENTER[7:0]
                            msWriteByte( REG_160359+ DP_Combo_Offset, DPAUM1 ); //UPDATE MAUD_CENTER[15:8]
                            msWriteByte( REG_16035A+ DP_Combo_Offset, DPAUM2 ); //UPDATE MAUD_CENTER[23:16]
                            msWriteByte( REG_160360+ DP_Combo_Offset, DPAUN0 ); //UPDATE NAUD_CENTER[7:0]
                            msWriteByte( REG_160361+ DP_Combo_Offset, DPAUN1 ); //UPDATE NAUD_CENTER[15:8]
                            msWriteByte( REG_160362+ DP_Combo_Offset, DPAUN2 ); //UPDATE NAUD_CENTER[23:16]

                           HW_SET_AUDIO_MUTE_ENABLE();

                          DP_printData( "\r\nDP_Audio_bigchange%x ", 0 );

                            gDPComboInfo[ComboPort].bDPAUPLLBIGChange = FALSE;        // claer IRQ flag

                #if 0
                            DPFBDIV0 = msReadByte( REG_160604 + DP_Combo_Offset); //FBDIV[7:0]
                            DPFBDIV1 = msReadByte( REG_160605 + DP_Combo_Offset); //FBDIV[15:8]
                            DPFBDIV = ( DPFBDIV1 << 8 ) + DPFBDIV0;
                            DPKP = GetPow2(( msReadByte( REG_160600+ DP_Combo_Offset ) >> 4 ) & 0x0F ); //KP
                            DPKM = GetPow2((msReadByte( REG_160601+ DP_Combo_Offset )  & 0x0F )); //KM
                #endif


                            DPFREQ = (64)  * ( 214.77 / LinkRate ) * ( DPKM * DPAUN ) / ( DPKP * DPAUM ) * DPFBDIV;

                            DPFREQ0 = ( DPFREQ & 0x000000FF );
                            DPFREQ1 = ( DPFREQ & 0x0000FF00 ) >> 8;
                            DPFREQ2 = ( DPFREQ & 0x00FF0000 ) >> 16;

                //  msWriteByte( REG_160608+ DP_Combo_Offset, DPFREQ0 );           //Freq
                //  msWriteByte( REG_160609+ DP_Combo_Offset, DPFREQ1 );           //Freq
                //  msWriteByte( REG_16060A+ DP_Combo_Offset, DPFREQ2 );           //Freq

                //   msWriteByte( REG_160607+ DP_Combo_Offset, 0x20 );           //update Freq
                //   msWriteByte( REG_160607+ DP_Combo_Offset, 0x00 );           //update Freq
                //   msWriteByte( REG_16060B+ DP_Combo_Offset, 0x0A );                  //set BW

                msWriteByte( REG_162F08, DPFREQ0 );           //Freq
                msWriteByte( REG_162F09, DPFREQ1 );           //Freq
                msWriteByte( REG_162F0A, DPFREQ2 );           //Freq
                   msWriteByte( REG_162F0B, 0x2A );                  //set BW 0x2A
                            msWriteByteMask( REG_162F07, BIT5,BIT5 );           //update Freq
                       msWriteByteMask( REG_162F07, 0,BIT5);                 //update Freq


                            HW_SET_AUDIO_MUTE_DISABLE();
                #if 1
                            DP_printData( "AUD M0 %x", DPAUM0 );
                            DP_printData( "AUD M1 %x", DPAUM1 );
                            DP_printData( "AUD M2 %x", DPAUM2 );
                            DP_printData( "AUD N0 %x", DPAUN0 );
                            DP_printData( "AUD N1 %x", DPAUN1 );
                            DP_printData( "AUD N2 %x", DPAUN2 );
                            DP_printData( "DPFBDIV %x", DPFBDIV );
                            DP_printData( "DPKP %x", DPKP );
                            DP_printData( "DPKM %x", DPKM );
                            DP_printData( "AUD Freq0 %x", DPFREQ0 );
                            DP_printData( "AUD Freq1 %x", DPFREQ1 );
                            DP_printData( "AUD Freq2 %x", DPFREQ2 );
                #endif
		  #if DPSSC
			DPAudioSSCInit(ComboPort);
		  #endif
               }


          }




    }

//**************************************************************************
//  [Function Name]:
//                  DPRxHPDEnable()
//  [Description]
//                  DPRxHPDEnable()
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPComRxHPDEnable(Bool bEnable,  BYTE ComboPort )
{
    if(bEnable)
    {
        if(ComboPort == Combo_PORT0)
        {
                   hw_Set_DPHpd0();

         }
        else if(ComboPort == Combo_PORT1)
        {
                hw_Set_DPHpd1();

        }
        else if(ComboPort == Combo_PORT2)
        {
                    hw_Set_DPHpd2();

        }
        else if(ComboPort == Combo_PORT3)
        {
                 hw_Set_DPHpd3();

        }
        else if(ComboPort == Combo_PORT4)
        {
                    hw_Set_DPHpd4();

        }
        else if(ComboPort == Combo_PORT5)
        {
                    hw_Set_DPHpd5();

        }
    }
    else
    {
        if(ComboPort == Combo_PORT0)
        {
                    hw_Clr_DPHpd0();

        }
        else if(ComboPort == Combo_PORT1)
        {
                 hw_Clr_DPHpd1();

        }
        else if(ComboPort == Combo_PORT2)
        {
                 hw_Clr_DPHpd2();

        }
        else if(ComboPort == Combo_PORT3)
        {
                 hw_Clr_DPHpd3();

        }
        else if(ComboPort == Combo_PORT4)
        {
                    hw_Clr_DPHpd4();

        }
        else if(ComboPort == Combo_PORT5)
        {
                    hw_Clr_DPHpd5();

            }
}

}

//**************************************************************************
//  [Function Name]:
//                  DPComboAudioMuteCheck()
//  [Description]
//                  DPComboAudioMuteCheck
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void  DPComboAudioMuteCheck(BYTE ComboPort)
{
     WORD DP_Combo_Offset=0;
     DP_Combo_Offset = DPRxComboOffset(ComboPort);


    if(msReadByte(REG_1630B0)&(BIT0|BIT1))
    {
	 //msWriteByteMask( REG_162F07, BIT5,BIT5 );           //update Freq
	 //msWriteByteMask( REG_162F07, 0,BIT5);                 //update Freq
        //DP_printMsg("\r\nAudio Sample / Parity Error ");
        msWriteByteMask(REG_1630B0,BIT0|BIT1,BIT0|BIT1);
    }
#if !DPSSC
    if(msReadByte(REG_1630B1)&(BIT0|BIT1))
    {
     msWriteByteMask(REG_1630B1,BIT0|BIT1,BIT0|BIT1);
    }
#endif
     if((msReadByte(REG_160656+ DP_Combo_Offset))&(BIT2))
    {
            //DP_printMsg("\r\nNo Ls Clock");
            msWriteByte((REG_16065A+ DP_Combo_Offset), BIT2);
    }
    if((msReadByte(REG_160657+ DP_Combo_Offset))&(BIT0))
    {
            //DP_printMsg("\r\n Linke Rate Change");
            msWriteByte((REG_16065B+ DP_Combo_Offset) ,BIT0);
    }


}
//**************************************************************************
//  [Function Name]:
//                  DPRxCheckInputFormat()
//  [Description]
//                  DPRxCheckInputFormat
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComboCheckInputFormat( BYTE  ComboPort)
{
    // DP MSA MISC[7:0]
    // [2:1]        00:RGB              ,             01:YCbCr 422        ,   10:YCbCr 444
    // [3]          0:VESA Range     ,               1:CEA Range
    // [4]          0:BT601              ,               1:BT709
    // [7:5]     000:6bits
    //             001:8bits
    //             010:10bits
    //             011:12bits
    //             100:16bits
     WORD DP_Combo_Offset=0;
     BYTE u8Temp=DPRxComPacketColor(ComboPort);
     BYTE u8Temp1 = DPRxComPacketType(ComboPort);
     BYTE u8Temp2 = DPRxComPacketRange(ComboPort);
     BYTE u8Temp3 = DPRxComColorDepth(ComboPort);

      DP_Combo_Offset = DPRxComboOffset(ComboPort);

    if( u8Temp != gDPComboInfo[ComboPort].ucDPColorFormat )
    {
        gDPComboInfo[ComboPort].ucDPColorFormat = u8Temp;
    }

   if( u8Temp1 != gDPComboInfo[ComboPort].bColorType )
    {      
        gDPComboInfo[ComboPort].bColorType = u8Temp1;           
    }

     if( u8Temp2 != gDPComboInfo[ComboPort].ucDPRangeFormat )
    {
        gDPComboInfo[ComboPort].ucDPRangeFormat = u8Temp2;
    }

    if( u8Temp3 != gDPComboInfo[ComboPort].ucDPColorDepth )
    {
        gDPComboInfo[ComboPort].ucDPColorDepth = u8Temp3;
    }


}

//**************************************************************************
//  [Function Name]:
//                  DPRxComPacketType()
//  [Description]
//                  DPRxComPacketType
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxComPacketType(BYTE ComboPort )
{
   
    WORD DP_Combo_Offset=0;
    DP_Combo_Offset = DPRxComboOffset(ComboPort);
   
    BYTE  temp= BT601;
   
    if( msReadByte(REG_160269 + DP_Combo_Offset) & BIT4 )
    {
        temp = BT709;
    }
   
    return temp;   
    
}


//**************************************************************************
//  [Function Name]:
//                  DPRxComColorDepth()
//  [Description]
//                  DPRxComColorDepth
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxComColorDepth(BYTE ComboPort )
{
     BYTE  temp;
    WORD DP_Combo_Offset=0;
     DP_Combo_Offset = DPRxComboOffset(ComboPort);

      switch(msReadByte(REG_160269 + DP_Combo_Offset) >>5 )
    {
        case 0:
            temp = Combo_DP_COLOR_DEPTH_6_BIT;
            break;
        case 1:
            temp = Combo_DP_COLOR_DEPTH_8_BIT;
            break;
        case 2:
            temp = Combo_DP_COLOR_DEPTH_10_BIT;
            break;
      case 3:
            temp = Combo_DP_COLOR_DEPTH_12_BIT;
            break;
          case 4:
            temp = Combo_DP_COLOR_DEPTH_16_BIT;
            break;
          default:
           temp = Combo_DP_COLOR_DEPTH_UNKNOWN;
            break;

    }
    return temp;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxComPacketRange()
//  [Description]
//                  DPRxComPacketRange
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxComPacketRange(BYTE ComboPort )
{
    BYTE  temp;
    WORD DP_Combo_Offset=0;
     DP_Combo_Offset = DPRxComboOffset(ComboPort);

    temp = InputRange_Full;

    if( msReadByte(REG_160269 + DP_Combo_Offset) & BIT3 )
    {
        temp = InputRange_Limited;
    }
    else
    {
        temp = InputRange_Full;
    }

    return temp;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxComPacketColor()
//  [Description]
//                  DPRxComPacketColor
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE DPRxComPacketColor( BYTE ComboPort )
{
    BYTE xdata temp;
    WORD DP_Combo_Offset=0;
     DP_Combo_Offset = DPRxComboOffset(ComboPort);

    temp = InputColor_RGB;
    switch(( msReadByte( REG_160269 + DP_Combo_Offset) & 0x06 ) >> 1 )
    {
        case 0:
            temp = InputColor_RGB;
            break;
        case 1:
            temp = InputColor_YPBPR_422;
            break;
        case 2:
            temp = InputColor_YPBPR_444;
            break;
    }

    return temp;
}

//**************************************************************************
//  [Function Name]:
//                  DPFastLink()
//  [Description]
//                  DPFastLink
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPComFastLink(BYTE ComboPort)
{
          BYTE xdata i=0,j = 0;
          BYTE xdata FastLane[3] ={0x04,0x02,0x01};
          BYTE xdata FastRate[3] = {0x0A,0x06,0x1};
       WORD DP_Combo_Offset=0;
       DP_Combo_Offset = DPRxComboOffset(ComboPort);

          DP_printMsg( "------>>  <Fast> " );
                for(j=0;j<3;j++)
                {
                        if((j==2)||  gDPComboInfo[ComboPort].bDPTrainingP0T ||  gDPComboInfo[ComboPort].bDPTrainingP1T )
                        {
                                goto FastTraining_Exit;
                        }
                            DP_printData( "---Rate--->>%x ", FastRate[j]);
                            if( FastRate[j] == 0x06 )
                                  DP_ComSetSyntheizer( DP_LBR, ComboPort );
                            else
                                     DP_ComSetSyntheizer( DP_HBR, ComboPort );

                                            for(i=0;i<3;i++)
                                            {
                                            	ForceDelay1ms( 5 );
						       //DP_printData( "---i= %x ",i );
                                                      msWriteByte( REG_160781+DP_Combo_Offset, 0x80 + FastLane[i] );
                                                      DP_printData( "---Lane--->>%x ",FastLane[i] );
                                                       if( (msReadByte( REG_160782+DP_Combo_Offset )&0x03) == 0x00 )
                                                        {
                                                        		DP_printData( "---Lane==%x ",msReadByte( REG_160781+DP_Combo_Offset ) );
                                                                      if(msReadByte( REG_160782+DP_Combo_Offset)&BIT5)
                                                                      msWriteByteMask(REG_160782+DP_Combo_Offset, 0 , BIT5  );
                                                                      DPRxComFastTraining2Enable( TRUE , ComboPort);

                                                         }
                                                                     ForceDelay1ms(200);

                                                     if( !DPRxComboCheckLock(ComboPort) )
                                                      {
                                                              DPRxComFastTraining2Enable(FALSE, ComboPort);
                                                      }
                                                     else
                                                     {
                                                            BYTE xdata Counter=0;

                                                            while(DPRxComboCheckLock(ComboPort))
                                                            {
                                                                if(++Counter>5)
                                                                {
                                                                            goto FastTraining_OK;
                                                                }
                                                            }
                                                     }
                                           }
                         }


                          goto FastTraining_Exit;


                FastTraining_OK:
                                     ForceDelay1ms( 50 );
                                     msWriteByte( REG_160346+DP_Combo_Offset, 0xFF);
                                       DP_printData( "---Lane---==>>%x ",msReadByte( REG_160781+DP_Combo_Offset ) );
                                     DP_printMsg( " <Fast> - DP Lock!" );
                                    if(!DPRxComCheckTiming(ComboPort, MSACnt))
                                    {      
						msWriteByteMask( REG_160782+DP_Combo_Offset, BIT5, BIT5  );
                                               ForceDelay1ms( 50 );
						if(!DPRxComCheckTiming(ComboPort, MSACnt))
						msWriteByteMask( REG_160782+DP_Combo_Offset, 0, BIT5  );
                                    }
                                     DPRxComFastTraining2Enable( FALSE  , ComboPort);
                                    return TRUE;
                FastTraining_Exit:
                                    DP_printMsg( " <Fast> - DP Lose Lock!" );
                                    DPRxComFastTraining2Enable( FALSE , ComboPort );

                                   return FALSE;

}


//**************************************************************************
//  [Function Name]:
//                  DP_ComSetSyntheizer()
//  [Description]
//                  DP_ComSetSyntheizer
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DP_ComSetSyntheizer( E_DP_Rate bRate,  BYTE  ComboPort)
{
    WORD DP_Combo_Offset=0;
    DP_Combo_Offset = DPRxComboOffset(ComboPort);

    if( bRate == DP_LBR )
    {

        msWriteByteMask( REG_1606A8+DP_Combo_Offset, BIT4, BIT3|BIT4|BIT5);                          //  DCDR KP Value
	 msWrite2Byte( REG_112004 + ComboPort*COMBO_PHY_OFFSET, 0xaaab );
        msWriteByte( REG_112006 + ComboPort*COMBO_PHY_OFFSET, 0x2a );   
        msWriteByte( REG_160780+DP_Combo_Offset, 0x06 );

    }
    else
    {
    	
         msWriteByteMask( REG_1606A8+DP_Combo_Offset, BIT3|BIT4, BIT3|BIT4|BIT5);                 //  DCDR KP Value
	  msWrite2Byte( REG_112004 + ComboPort*COMBO_PHY_OFFSET, 0x999a );
         msWriteByte( REG_112006 + ComboPort*COMBO_PHY_OFFSET, 0x19);         
         msWriteByte( REG_160780+DP_Combo_Offset, 0x0A );
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxComClrSQTime()
//  [Description]
//                  DPRxComClrSQTime
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComClrSQTime(BYTE  ComboPort)
{
     gDPComboInfo[ComboPort].bSquelchWaitStable=0;
     gDPComboInfo[ComboPort].bSquelchStable=0;
     gDPComboInfo[ComboPort].uwDPRxStableTimeout=0;
     gDPComboInfo[ComboPort].uwFastTrainingCntDown=0;
}
//**************************************************************************
//  [Function Name]:
//                  DPRxComCheckSQTime()
//  [Description]
//                  DPRxComCheckSQTime
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPRxComCheckSQTime(WORD Times, BYTE  ComboPort)
{
                   if (DP_ComGetSquelch(1, ComboPort) )
                    {
                        if ( gDPComboInfo[ComboPort].uwDPRxStableTimeout==0 && !gDPComboInfo[ComboPort].bSquelchStable)
                        {
                            if (!gDPComboInfo[ComboPort].bSquelchWaitStable)
                            {
                                gDPComboInfo[ComboPort].uwDPRxStableTimeout=Times;
                                gDPComboInfo[ComboPort].bSquelchWaitStable=1;
                                gDPComboInfo[ComboPort].bSquelchStable=0;
                            }
                            else
                            {

                                gDPComboInfo[ComboPort].bSquelchWaitStable=0;
                                gDPComboInfo[ComboPort].bSquelchStable=1;
                                gDPComboInfo[ComboPort].uwFastTrainingCntDown=0;
                            }
                        }
                        return TRUE;
                    }
                    else
                    {
                        gDPComboInfo[ComboPort].bSquelchWaitStable=0;
                        gDPComboInfo[ComboPort].bSquelchStable=0;
                        gDPComboInfo[ComboPort].uwDPRxStableTimeout=0;
                  	   gDPComboInfo[ComboPort].uwFastTrainingCntDown=0;

                        return FALSE;
                    }

}
//**************************************************************************
//  [Function Name]:
//                  DP_GetSquelch()
//  [Description]
//                  DP_GetSquelch
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool DP_ComGetSquelch( DWORD Times, BYTE  ComboPort)
{
   BYTE xdata Temp=1;

    Temp = (Temp << ComboPort);

    while( Times-- )
    {
        if( !( msReadByte( REG_0017B0 )&Temp ) )
        {
            return FALSE;
        }

    }  //DP_printMsg( "SQ Active" );

    return TRUE;

}



//**************************************************************************
//  [Function Name]:
//                  DPRxComFastTraining2Enable()
//  [Description]
//                  DPRxComFastTraining2Enable
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComFastTraining2Enable( Bool bEnable , BYTE ComboPort)
{
    WORD DP_Combo_Offset=0;
    DP_Combo_Offset = DPRxComboOffset(ComboPort);
    UNUSED(ComboPort);
  
    if( bEnable )
    {
        DP_printMsg( " <Fast> - En " );       
       // msWriteByte( REG_1644, 0x00);
        msWriteByteMask( REG_1606B5+DP_Combo_Offset, BIT6, BIT6 );
        msWriteByteMask( REG_1607E0+DP_Combo_Offset,  BIT1|BIT2, BIT1|BIT2 );
   	 msWriteByteMask( REG_1120D7+ComboPort*COMBO_PHY_OFFSET, 0x0E,0x0E );
    }
    else
    {
        return;
        DP_printMsg( " <Fast> - Dis" );
        msWriteByte( REG_160544+DP_Combo_Offset, 0x77);
        msWriteByteMask( REG_1606B5+DP_Combo_Offset, 0, BIT6 );
        msWriteByteMask( REG_1607E0+DP_Combo_Offset, 0, BIT1|BIT2 );
        msWriteByteMask( REG_1120D7+ComboPort*COMBO_PHY_OFFSET, 0x00,0x0E );

    }
}



//**************************************************************************
//  [Function Name]:
//                  DPRxCheckLock()
//  [Description]
//                  DPRxCheckLock
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BOOL DPRxComboCheckLock( BYTE ucComboPort  )
{

    BYTE xdata ucLans = 0;
    BOOL xdata bStstus = FALSE;
    BYTE xdata ucStatus0 = 0;
    BYTE xdata ucStatus1 = 0;
    BYTE xdata uctemp = 0;
    WORD DP_Combo_Offset=0;

    DP_Combo_Offset = DPRxComboOffset(ucComboPort);
    uctemp =  (msReadByte( REG_1607C3+DP_Combo_Offset )&0x38 ) << 1;
    ucStatus0 =  (uctemp | (msReadByte( REG_1607C3+DP_Combo_Offset )&0x07));

    uctemp =  (msReadByte(REG_1607C4+DP_Combo_Offset )&0x38 ) << 1;
    ucStatus1 =  (uctemp | (msReadByte( REG_1607C4+DP_Combo_Offset  )&0x07));

   gDPComboInfo[ucComboPort].bDPLANELOCK = FALSE;
   gDPComboInfo[ucComboPort].bDPCDRLOCK = FALSE;

    ucLans = msReadByte( REG_160781+DP_Combo_Offset ) & 0x07;

#if 1
   // DP_printData( " ucStatus0=%x", msReadByte( REG_1F10 ) );
  //  DP_printData( " ucStatus1=%x", msReadByte( REG_36A8 ) );
  //  DP_printData( " 8E=%x", msReadByte( REG_368E ) );
#endif

    switch( ucLans )
    {
        case 0x01:
            if( (ucStatus0 & 0x07) == 0x07 )
            {
                bStstus = TRUE;
          gDPComboInfo[ucComboPort].bDPLANELOCK = TRUE;
          gDPComboInfo[ucComboPort].bDPCDRLOCK = TRUE;

            }
            break;
        case 0x02:
            if( ucStatus0 == 0x77 )
            {
                bStstus = TRUE;
                gDPComboInfo[ucComboPort].bDPLANELOCK = TRUE;
          gDPComboInfo[ucComboPort].bDPCDRLOCK = TRUE;
            }
            break;
        case 0x04:
            if(( ucStatus0 == 0x77 ) && ( ucStatus1 == 0x77 ) )
            {
                bStstus = TRUE;
                gDPComboInfo[ucComboPort].bDPLANELOCK = TRUE;
          gDPComboInfo[ucComboPort].bDPCDRLOCK = TRUE;
            }
            break;
        default:
        gDPComboInfo[ucComboPort].bDPLANELOCK = FALSE;
        gDPComboInfo[ucComboPort].bDPCDRLOCK = FALSE;

            break;
     }

    if( !( msReadByte( REG_1607C4+DP_Combo_Offset )&_BIT7 ) ) // lost alignment
    {
               gDPComboInfo[ucComboPort].bDPLANELOCK = FALSE;
    }

    return bStstus;
}



//**************************************************************************
//  [Function Name]:
//                  DPPrintState(BYTE state)
//  [Description]
//                  DPPrintState
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPComboState( BYTE state )
{
    switch( state )
    {
        case eComSTATE_INITIAL:
            DP_printMsg( "##  eComSTATE_INITIAL! ##" );
            break;
        case eComSTATE_CKECKLOCK:
            DP_printMsg( "## eComSTATE_CKECKLOCK! ##" );
            break;
        case eComSTATE_MEASURE:
            DP_printMsg( "## eComSTATE_MEASURE!##" );
            break;
        case eComSTATE_NORMAL:
            DP_printMsg( "## eComSTATE_NORMAL! ##" );
            break;
        case  eComSTATE_POWERSAVE:
            DP_printMsg( "## eComSTATE_POWERSAVE! ##" );
            break;
        case eComSTATE_DPIDLE:
            DP_printMsg( "## eComSTATE_DPIDLE! ##" );
            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  DPRxSetCombo_0(Byte ucComboPort)
//  [Description]
//                  DPRxSetCombo_0
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************

void DPRxSetCombo_0(BYTE ucComboPort)
{
     WORD DP_Combo_Offset=0;

     DP_Combo_Offset = DPRxComboOffset(ucComboPort);
      DP_printData("===%x", ucComboPort);
     msWriteByteMask(REG_163018+ucComboPort * 0x10 ,  ucComboPort, BIT0|BIT1);                             // Clock_pixel
     msWrite2Byte( REG_163006, 0xFFFF );
     msWrite2Byte( REG_163008, 0x3FFF );

     msWrite2Byte( REG_163012+ucComboPort * 0x10, 0xFFFF );
     msWrite2Byte( REG_163014+ucComboPort * 0x10, 0x3FFF );
        msWriteByteMask( REG_163010 + ucComboPort * 0x10, 0,BIT0 );         // VPLL2 choose Combo2 (CCJ)
//===========================================================================
     msWriteByteMask(REG_162F82+ucComboPort * 0x10 ,0,BIT0);                                // kp/kn/km ov enable  VPLL2
     msWriteByteMask(REG_162F83+ucComboPort * 0x10 ,0,BIT0);                                // power down             VPLL2
     msWriteByteMask(REG_162F97+ucComboPort * 0x10,0,BIT0 );                                                    // vpll vco frequency selection
//===========================================================================
    #if DP_AUXCom_XTAL
     msWriteByteMask( REG_163010 +ucComboPort * 0x10 , BIT1 ,BIT1|BIT2 );                          // Crystal
    #else
     msWriteByteMask( REG_163010 +ucComboPort * 0x10, 0 ,BIT1|BIT2 );
    #endif
     msWriteByteMask( REG_1630ED, ucComboPort<<2, BIT2|BIT3|BIT4);                                      //VPLL2

     msWriteByteMask(REG_1607E4+DP_Combo_Offset, BIT3, BIT3);                            // Disable SQ
     msWriteByteMask(REG_1607E1+DP_Combo_Offset , BIT1|BIT0, BIT1|BIT0);                     //  SQ override

     msWriteByteMask(REG_160640+DP_Combo_Offset , 0, BIT0);                               // Select DP

     msWrite2Byte( REG_1630F8, 0xFFFF );                                      // PLL Clock Gen
     msWriteByte( REG_1630FA, 0xFF );

    msWriteByteMask(REG_162FD6 ,  (DP_HBR_0)<<4, 0xF0);                               // VPLL2 HBR CLK  SRC
    msWriteByteMask(REG_1630ED ,  (DP_HBR_0)<<2, 0x1C);                   // M/N/FIN
       msWriteByteMask( REG_162FD7, 0 ,BIT0 );                                // VCO Fre for Combo

     msWriteByteMask(REG_162FC2, 0, BIT0);                                // KpknKm Enable
     msWriteByteMask(REG_162FC3, 0, BIT0);                                // Power Down

    //=======================
     msWriteByteMask( REG_161440, 0, BIT0);
     msWriteByteMask( REG_1630E2, 0, BIT3);
     msWriteByteMask( REG_1630E6, 0, BIT3);
    #if (CHIP_ID==MST9U2)
    msWriteByteMask( REG_000392, BIT1, BIT1);
    #endif
    //========================

}
//**************************************************************************
//  [Function Name]:
//                  DPRxSetCombo_1(Byte ucComboPort)
//  [Description]
//                  DPRxSetCombo_1
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxSetCombo_1(BYTE ucComboPort)
{
    UNUSED(ucComboPort);


}
//**************************************************************************
//  [Function Name]:
//                  DPRxSetCombo_1(Byte ucComboPort)
//  [Description]
//                  DPRxSetCombo_1
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComboTiming( BYTE ucComboPort )
{
        WORD xdata Blanking = 0;
        WORD xdata Width, Front_Porch = 0, Back_Porch = 0;
     WORD DP_Combo_Offset=0;

     DP_Combo_Offset = DPRxComboOffset(ucComboPort);
       // DP_printData("===%x", ucComboPort);

        gDPComboInfo[ucComboPort].bDPInterlace = ( msReadByte( REG_16026B+DP_Combo_Offset ) & 0x04 ) ? TRUE : FALSE; 
   
        
        gDPComboInfo[ucComboPort].uwDPVtotal = msRead2Byte( REG_1603B6 +DP_Combo_Offset) & 0x7FFF;
        gDPComboInfo[ucComboPort].uwDPVWidth = msRead2Byte( REG_1603BE+DP_Combo_Offset) & 0x7FFF;

        msWriteByteMask( REG_160210+DP_Combo_Offset, BIT1 | BIT0, BIT1 | BIT0 );
        Blanking = gDPComboInfo[ucComboPort].uwDPVtotal - gDPComboInfo[ucComboPort].uwDPVWidth;
        Width =  Blanking / 2;
        Front_Porch = ( Blanking - Width ) / 2;
        Back_Porch = Blanking - Width - Front_Porch ;
        msWrite2Byte( REG_160214+DP_Combo_Offset, Width );
        msWriteByte( REG_16020B+DP_Combo_Offset, Front_Porch );
        msWriteByte( REG_16020E+DP_Combo_Offset, ( Back_Porch ));
        msWrite2Byte( REG_160212+DP_Combo_Offset, msRead2Byte( REG_160212+DP_Combo_Offset) );
    gDPComboInfo[ucComboPort].uwDPHWidth = msRead2Byte( REG_160266+DP_Combo_Offset) & 0x7FFF;
    if(gDPComboInfo[ucComboPort].uwDPHWidth >3000)
    {
    msWrite2Byte( REG_160212+DP_Combo_Offset, 0x34 );
    msWriteByteMask( REG_160220+DP_Combo_Offset, 0, BIT1 | BIT0 );
    }
    else
    {
    msWrite2Byte( REG_160212+DP_Combo_Offset, 0x2C );
    msWriteByteMask( REG_160220+DP_Combo_Offset, BIT1, BIT1 | BIT0 );
    }


  if( gDPComboInfo[ucComboPort].bDPInterlace)     
 {
             DWORD temp=0, temp1=0;      
            temp = ((msReadByte( REG_160284+DP_Combo_Offset )&0x0F) <<16)  + msRead2Byte( REG_160282+DP_Combo_Offset );
            temp1 = ((msReadByte(REG_160288+DP_Combo_Offset )&0x0F) <<16)  + msRead2Byte( REG_160286+DP_Combo_Offset );
            if(!(msReadByte( REG_16020A+DP_Combo_Offset)&BIT0))
            {
                    if(temp1 < temp ) 
                    msWriteByteMask( REG_16020A+DP_Combo_Offset, BIT0, BIT0);
            }
            else
            {
                    if(temp1 < temp ) 
                    msWriteByteMask( REG_16020A+DP_Combo_Offset, 0, BIT0 ); 
            }       
 }
 else
 {
            msWriteByteMask( REG_16020A+DP_Combo_Offset, 0, BIT0 );
 } 




}
//**************************************************************************
//  [Function Name]:
//                  mapi_DPCombo_ISR(Byte ucComboPort)
//  [Description]
//                  mapi_DPCombo_ISR
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD DPRxComboOffset(BYTE ComboPort)
{
    WORD Combo[6] = {0 , 0x600, 0xE00 , 0x1600 , 0x1E00 , 0x2600};
    return Combo[ComboPort];
}

//**************************************************************************
//  [Function Name]:
//                  DPRXComCheckDPMS(Byte ucComboPort)
//  [Description]
//                  DPRXComCheckDPMS
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool DPRXComCheckDPMS(BYTE InputPort)
{

    BOOL bIgnoreDPMS = FALSE; 
    WORD DP_Combo_Offset=0;
    DP_Combo_Offset = DPRxComboOffset(InputPort);   
    
    if(((msReadByte(REG_16026B+DP_Combo_Offset ) & 0x08 ) ? 1 : 0)&&(DPRxComboCheckLock(InputPort))&&(msReadByte(REG_001172+InputPort*COMBO_PHY_OFFSET )&0x01))
    {
        bIgnoreDPMS = TRUE;
    }
 

    return bIgnoreDPMS;
}

//**************************************************************************
//  [Function Name]:
//                  DPRxComboHVFSMCheck(BYTE InputPort)
//  [Description]
//                  DPRxComboHVFSMCheck
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void DPRxComboHVFSMCheck(BYTE InputPort)
{
	BYTE ubHs_gen_fsm_status;
	BYTE ubCheck_fsm_Times;
    WORD DP_Combo_Offset=0;		

	ubHs_gen_fsm_status = 0x0;
	ubCheck_fsm_Times = 0x2;  //RD suggest we should check FSM 2 times

    DP_Combo_Offset = DPRxComboOffset(InputPort);   
	
	//make sure hs_gen_fsm_status[3:0] only has 1 bit as 1, otherwise reset reg_hs_gen_fsm_rst
	while(ubCheck_fsm_Times > 0)
	{	   
		ubHs_gen_fsm_status = (msReadByte(REG_160250+DP_Combo_Offset) & 0x80) >> 7;
		ubHs_gen_fsm_status = ((msReadByte(REG_160251+DP_Combo_Offset) & 0x07) << 1) | ubHs_gen_fsm_status;
	
		if(((ubHs_gen_fsm_status & (ubHs_gen_fsm_status - 1)) != 0x0) || (ubHs_gen_fsm_status == 0x0))
		{
			msWriteByteMask((REG_16020D+DP_Combo_Offset), BIT0, BIT0); //reg_hs_gen_fsm_rst
			Delay2us();
			msWriteByteMask((REG_16020D+DP_Combo_Offset), 0, BIT0);			
		}
		
		ubCheck_fsm_Times --;
	}

}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ComGetColorDepth(BYTE InputPort)
//  [Description]
//                  mapi_DP_ComGetColorDepth
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_ComGetColorDepth(BYTE InputPort)
{

    return gDPComboInfo[InputPort].ucDPColorDepth;
}


//**************************************************************************
//  [Function Name]:
//                  mapi_DP_GetColorSpace(BYTE InputPort)
//  [Description]
//                  mapi_DP_GetColorSpace
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_ComGetColorSpace(BYTE InputPort)
{

    return gDPComboInfo[InputPort].ucDPColorFormat;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DP_ComboGetRangeFormat(BYTE InputPort)
//  [Description]
//                  mapi_DP_ComboGetRangeFormat
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mapi_DP_ComboGetRangeFormat(BYTE InputPort)
{
    return gDPComboInfo[InputPort].ucDPRangeFormat;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_DPCom_TimerInterrupt(Byte ucComboPort)
//  [Description]
//                  mapi_DPCom_TimerInterrupt
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_DPCom_TimerInterrupt(void)
{
    BYTE ComboPort=0;

     for(ComboPort = 0; ComboPort < 6; ComboPort++ )  // Scaler Scan Port
     {
             if(DP_Combo_Connected[ComboPort] ==0)
             continue;

             if(gDPComboInfo[ComboPort].uwDPRxStableTimeout)
              gDPComboInfo[ComboPort].uwDPRxStableTimeout--;

              if(gDPComboInfo[ComboPort].DPHDCPCnt )
              gDPComboInfo[ComboPort].DPHDCPCnt --;

              if(gDPComboInfo[ComboPort].uwCDRHPDCnt )
              gDPComboInfo[ComboPort].uwCDRHPDCnt --;

              if(gDPComboInfo[ComboPort].uwSkewHPDCnt)
              gDPComboInfo[ComboPort].uwSkewHPDCnt--;
#if LOOPOUT_DPNOSIGNAL_SOLUTION
			  if(gDPComboInfo[ComboPort].uwReChkTiming)
			  gDPComboInfo[ComboPort].uwReChkTiming--;
#endif
		#if DPPacketMute
		   	  if(gDPComboInfo[ComboPort].bPacketTime)
		   	  gDPComboInfo[ComboPort].bPacketTime--;	
		#endif
			
     }
     
}
//**************************************************************************
//  [Function Name]:
//                  mapi_DPCombo_ISR(Byte ucComboPort)
//  [Description]
//                  mapi_DPCombo_ISR
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
  void mapi_DPCombo_ISR(void)
  {
     WORD DP_Combo_Offset=0;
     BYTE ComboPort=0;
     WORD  DPTP1Cnt = 0;
        WORD  DPTP2Cnt = 0;

     for(ComboPort = 0; ComboPort < 6; ComboPort++ )  // Scaler Scan Port
     {
         if(DP_Combo_Connected[ComboPort] ==0)
         continue;
          DP_Combo_Offset = DPRxComboOffset(ComboPort);

        // ***************************************************
        // ****************  TP1                             **************
        // ***************************************************
          if( msRegs(REG_160656+DP_Combo_Offset)& BIT7 ) //  // Training P1
          {
			msRegs(REG_1607E0+DP_Combo_Offset) =(msRegs(REG_1607E0+DP_Combo_Offset)&(~(BIT2|BIT1)));          //	
			msRegs(REG_1606B5+DP_Combo_Offset) =(msRegs(REG_1606B5+DP_Combo_Offset)&(~(BIT6)));                 //	
		
            gDPComboInfo[ComboPort].bDPTrainingP0T = TRUE;
            gDPComboInfo[ComboPort].CTSLevel = 0;
			gDPComboInfo[ComboPort].bDPLoseCDRLockIRQ = FALSE;
			gDPComboInfo[ComboPort].bDPInterlaneSkewIRQ = FALSE;

            msRegs(REG_160702 + DP_Combo_Offset) = 0x00;
            msRegs(REG_160703 + DP_Combo_Offset) = 0x00;
            #if 1
              while((( msRegs(REG_160782 + DP_Combo_Offset) & 0x03 ) == 0x01 ) && ( DPTP1Cnt < 50000 ) ) // while pattern 1 is On-Going
                    {
                        if(DPTP1Cnt > 49990)
                        msRegs(REG_160702 + DP_Combo_Offset) = 0x00;

                        DPTP1Cnt++;
                        switch( msRegs(REG_160781 + DP_Combo_Offset) & 0x07 )
                        {
                            case 0x01:  // 1 lane
                                if(( msRegs(REG_160783 + DP_Combo_Offset)& 0x03 ) == ( msRegs(REG_160702 + DP_Combo_Offset) & 0x03 ) ) // request reach
                                    {
                                        if(( msRegs(REG_1607C3 + DP_Combo_Offset) & 0x01 ) != 0x01 ) // 1 lane LOCK Fail
                                        {
                                            {
                                                if( gDPComboInfo[ComboPort].CTSLevel  <3 )
                                                    gDPComboInfo[ComboPort].CTSLevel ++;  // update swing level
                                                msRegs(REG_160702 + DP_Combo_Offset) =( msRegs(REG_160702 + DP_Combo_Offset) & 0x0C) | ( gDPComboInfo[ComboPort].CTSLevel  & 0x03 );
                                            }
                                        }
                                    }
                                break;
                            case 0x02:  // 2 lanes
                                if(( msRegs(REG_160783 + DP_Combo_Offset) & 0x03 ) == ( msRegs(REG_160702 + DP_Combo_Offset) & 0x03 ) ) // request reach
                                    {
                                    if(( msRegs(REG_160784 + DP_Combo_Offset) & 0x03 ) == (( msRegs(REG_160702 + DP_Combo_Offset ) & 0x30 ) >> 4 ) ) // request reach
                                     {
                                        if(( msRegs(REG_1607C3 + DP_Combo_Offset) & 0x09 ) != 0x09 ) // 2 lanes CDR LOCK Fail
                                        {
                                            {
                                                if( gDPComboInfo[ComboPort].CTSLevel  < 3 )
                                                    gDPComboInfo[ComboPort].CTSLevel ++;  // update swing level
                                                msRegs(REG_160702 + DP_Combo_Offset) =( msRegs(REG_160702 + DP_Combo_Offset)& 0xCC) | ((( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 4 ) | ( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) );

                                            }
                                        }
                                        }
                                    }
                                break;
                            case 0x04:  // 4 lanes

                                if(( msRegs(REG_160783 + DP_Combo_Offset) & 0x03 ) == ( msRegs(REG_160702 + DP_Combo_Offset) & 0x03 ) ) // request reach
                                  {
                                    if(( msRegs(REG_160784 + DP_Combo_Offset) & 0x03 ) == (( msRegs(REG_160702 + DP_Combo_Offset) & 0x30 ) >> 4 ) ) // request reach
                                      {
                                        if(( msRegs(REG_160785 + DP_Combo_Offset) & 0x03 ) == ( msRegs(REG_160703 + DP_Combo_Offset ) & 0x03 ) ) // request reach
                                          {
                                            if(( msRegs(REG_160786 + DP_Combo_Offset) & 0x03 ) == (( msRegs(REG_160703 + DP_Combo_Offset) & 0x30 ) >> 4 ) ) // request reach
                                              {
                                                if((( msRegs(REG_1607C3 + DP_Combo_Offset) & 0x09 ) != 0x09 ) | (( msRegs(REG_1607C4 + DP_Combo_Offset) & 0x09 ) != 0x09 ) ) // 4 lanes CDR LOCK Fail
                                                {

                                                        if( gDPComboInfo[ComboPort].CTSLevel  < 3)
                                                            gDPComboInfo[ComboPort].CTSLevel ++;  // update swing level
                                                        msRegs(REG_160702 + DP_Combo_Offset) = (msRegs(REG_160702 + DP_Combo_Offset) & 0xCC) | ((( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 4 ) | ( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) );
                                                        msRegs(REG_160703 + DP_Combo_Offset) = (msRegs(REG_160703 + DP_Combo_Offset) & 0xCC) | ((( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 4 ) | ( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) );

                                                    }
                                                }
                                             }
                                         }
                                      }
                                break;
                            default:

                                break;
                        }
                    }
            #endif
              // msRegs(REG_113332)= msRegs(REG_113332)+1;
            msRegs(REG_16065A+DP_Combo_Offset)= BIT7;  // clear IRQ
                     msRegs(REG_16065A+DP_Combo_Offset) = 0;       // clear IRQ

          }
        // ***************************************************
        // ****************  TP2                             **************
        // ***************************************************
          if( msRegs(REG_160656+DP_Combo_Offset)& BIT6 ) //  // Training P2
          {
            gDPComboInfo[ComboPort].bDPTrainingP1T = TRUE;
            gDPComboInfo[ComboPort].CTSLevel = 0;
            DPTP2Cnt = 0;

                    msRegs(REG_160702 + DP_Combo_Offset)  = msRegs(REG_160702 + DP_Combo_Offset) & 0x33;
                    msRegs(REG_160703 + DP_Combo_Offset)  = msRegs(REG_160703 + DP_Combo_Offset) & 0x33;
                    #if 1
                    while((( msRegs(REG_160782 + DP_Combo_Offset) & 0x03 ) == 0x02 ) && ( DPTP2Cnt < 50000 ) ) // while pattern 2 is On-Going
                    {
                        if(DPTP2Cnt > 49990)
                        msRegs(REG_160782 + DP_Combo_Offset) = 0x00;

                        DPTP2Cnt++;
                        switch( msRegs(REG_160781 + DP_Combo_Offset) & 0x07 )
                        {
                            case 0x01:  // 1 lane
                                if((( msRegs(REG_160783 + DP_Combo_Offset) & 0x18 ) >> 3 ) == (( msRegs(REG_160702 + DP_Combo_Offset) & 0x0C ) >>2) ) // request reach
                                  {
                                        if(( msRegs(REG_1607C3 + DP_Combo_Offset) & 0x06 ) != 0x06 ) // 1 lane SYMBOL & EQ LOCK Fail
                                        {
                                            if( gDPComboInfo[ComboPort].CTSLevel  < 3 )
                                                gDPComboInfo[ComboPort].CTSLevel ++;  // update swing level
                                                msRegs(REG_160702 + DP_Combo_Offset) = (msRegs(REG_160702 + DP_Combo_Offset) & 0x03) | (( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 2 );

                                        }
                                    }
                                break;
                            case 0x02:  // 2 lanes
                                if((( msRegs(REG_160783 + DP_Combo_Offset) & 0x18 ) >> 3 ) == (( msRegs(REG_160702 + DP_Combo_Offset) & 0x0C )>>2) ) // request reach
                                 {
                                        if((( msRegs(REG_160784 + DP_Combo_Offset) & 0x18 ) >> 3 ) == (( msRegs(REG_160702 + DP_Combo_Offset) & 0xC0 ) >> 6 ) ) // request reach
                                          {
                                            if(( msRegs(REG_1607C3 + DP_Combo_Offset) & 0x36 ) != 0x36 ) // 2 lanes SYMBOL & EQ LOCK Fail
                                            {
                                                if( gDPComboInfo[ComboPort].CTSLevel  < 3 )
                                                    gDPComboInfo[ComboPort].CTSLevel ++;  // update swing level
                                                    msRegs(REG_160702 + DP_Combo_Offset) = (msRegs(REG_160702 + DP_Combo_Offset) & 0x33) | ((( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 6 ) | (( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 2 ) );
                                            }
                                        }
                                    }
                                break;
                            case 0x04:  // 4 lanes
                                if((( msRegs(REG_160783 + DP_Combo_Offset) & 0x18 ) >> 3 ) == (( msRegs(REG_160702 + DP_Combo_Offset) & 0x0C )>>2 )) // request reach
                                 {
                                    if((( msRegs(REG_160784 + DP_Combo_Offset) & 0x18 ) >> 3 ) == (( msRegs(REG_160702 + DP_Combo_Offset) & 0xC0 ) >> 6 ) ) // request reach
                                      {
                                        if((( msRegs(REG_160785 + DP_Combo_Offset) & 0x18 ) >> 3 ) == (( msRegs(REG_160703 + DP_Combo_Offset) & 0x0C )>>2 )) // request reach
                                          {
                                            if((( msRegs(REG_160786 + DP_Combo_Offset) & 0x18 ) >> 3 ) == (( msRegs(REG_160703 + DP_Combo_Offset) & 0xC0 ) >> 6 ) ) // request reach
                                              {
                                                if((( msRegs(REG_1607C3 + DP_Combo_Offset)&0x36 ) != 0x36 ) || (( msRegs(REG_1607C4 + DP_Combo_Offset)&0x36) != 0x36 ) ) // 4 lanes SYMBOL & EQ LOCK Fail
                                                {
                                                    if( gDPComboInfo[ComboPort].CTSLevel  < 3 )
                                                        gDPComboInfo[ComboPort].CTSLevel ++;  // update swing level
                                                     msRegs(REG_160702 + DP_Combo_Offset) = (msRegs(REG_160702 + DP_Combo_Offset) & 0x33) | ((( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 6 ) | (( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 2 ) );
                                                     msRegs(REG_160703 + DP_Combo_Offset) = (msRegs(REG_160703 + DP_Combo_Offset) & 0x33) | ((( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 6 ) | (( gDPComboInfo[ComboPort].CTSLevel  & 0x03 ) << 2 ) );
                                                }
                                           }
                                         }
                                     }
                                    }
                                break;
                            default:

                                break;
                        }
                    }
            #endif

              // msRegs(REG_113333)= msRegs(REG_113333)+1;
            msRegs(REG_16065A+DP_Combo_Offset)= BIT6;  // clear IRQ
                     msRegs(REG_16065A+DP_Combo_Offset) = 0;       // clear IRQ

          }
        // ***************************************************
        // ****************   Audio MN Big Change  **************
        // ***************************************************
        if(msRegs(REG_160656+DP_Combo_Offset)& BIT0 ) // AU MN BIG CHANGE
        {

            gDPComboInfo[ComboPort].bDPAUPLLBIGChange = TRUE;
            msRegs(REG_16065A+DP_Combo_Offset)= BIT0;  // clear IRQ
            msRegs(REG_16065A+DP_Combo_Offset) = 0;       // clear IRQ
        }
        // ***************************************************
        // ****************   Video MN Big Change  ***************
        // ***************************************************
        if(msRegs(REG_160656+DP_Combo_Offset)& BIT1 ) // AU MN BIG CHANGE
        {
            gDPComboInfo[ComboPort].bDPVPLLBIGChange = TRUE;
            msRegs(REG_16065A+DP_Combo_Offset)= BIT1;  // clear IRQ
            msRegs(REG_16065A+DP_Combo_Offset) = 0;       // clear IRQ
        }
        // ***************************************************
        // ****************   DPENABLELOSECDRLOCK **************
        // ***************************************************
        if(msRegs(REG_160657+DP_Combo_Offset)& BIT2 )
        {
           // msRegs(REG_113333)= msRegs(REG_113333)+1;
            gDPComboInfo[ComboPort].bDPLoseCDRLockIRQ = TRUE;
            gDPComboInfo[ComboPort].uwCDRHPDCnt =1000;
            msRegs(REG_16065B+DP_Combo_Offset)= BIT2;  // clear IRQ
            msRegs(REG_16065B+DP_Combo_Offset) = 0;       // clear IRQ
        }
         //****************************************************
            // ***************************************************
            // ****************   DPENABLEINTERLANESKEW ***********
            // ***************************************************
           if(msRegs(REG_160657+DP_Combo_Offset)& BIT1 ) // InterlaneSkew
            {
           // msRegs(REG_113334)= msRegs(REG_113334)+1;
            gDPComboInfo[ComboPort].bDPInterlaneSkewIRQ = TRUE;
            gDPComboInfo[ComboPort].uwSkewHPDCnt = 1000;
            msRegs(REG_16065B+DP_Combo_Offset)= BIT1;  // clear IRQ
            msRegs(REG_16065B+DP_Combo_Offset) = 0;       // clear IRQ
             }




     }
  }
#endif


