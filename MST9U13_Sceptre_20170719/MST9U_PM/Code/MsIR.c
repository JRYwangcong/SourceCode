/******************************************************************************
 Copyright (c) 2005 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: MsIr.c
 [Date]:        04-Feb-2005
 [Comment]:
   Remote control subroutines.
 [Reversion History]:
*******************************************************************************/

#define _MSIR_C_

// System
//#include <intrins.h>

// Common
//#include "define.h"
#include "Types.h"
//#include "Mcu.h"
#include "Board.h"

// External
#include "i2c.h"
//#include "uart.h"
#include "Common.h"
//#include "msio.h"
//#include "msreg.h"
#include "Ms_rwreg.h"
#include "ms_reg.h"
//#include "msmcu.h"
//#include "drvMcu.h"

// Internal
#if(IR_REMOTE_SEL != NO_REMOTE)

#define IR_DEBUG    1
#if ENABLE_DEBUG&&IR_DEBUG
    #define IR_printData(str, value)   printData(str, value)
    #define IR_printMsg(str)           printMsg(str)
#else
    #define IR_printData(str, value)
    #define IR_printMsg(str)
#endif

#include "MsIr.h"

#if(IR_MODE_SEL != IR_SOFTWARE_MODE)
void irDelayNop(BYTE ucCnt)
{
    while(ucCnt--)
    {
        _nop_();
    } // while
}
#endif
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#if(IR_MODE_SEL==IR_FULL_INT_NEC_MODE)
void irInitialize(void)
{
    DWORD xdata ucRPTimeOutTemp;
    BYTE xdata i;
    ucRPTimeOutTemp = IR_RP_TIMEOUT;
    g_ucIrCode = 0x00;
    g_bIRRepeat = 0;
    g_IRBufferEmpty = 0;
    for(i=0;i<8;i++) //clear fifo after PM wake up
        msWriteBit(REG_003DB0,TRUE,_BIT0);

    //msWriteByte(REG_038E, msReadByte(REG_003D8E)&~(_BIT0|_BIT1));//set clock gen to 1Mhz   //?
    INT_FIQ_IR_ENABLE(TRUE);    //enable FIQ mask
    msWriteBit(REG_002B28,0,BIT1);  //PM IRQ

    msWriteByteMask(REG_000407, 0x80, 0x80);    //enable GPIO as IR in
    msWrite2Byte(REG_003D80, 0x01BF);

    msWrite2Byte(REG_003D82, IR_HDC_UPB); // header code upper bound
    msWrite2Byte(REG_003D84, IR_HDC_LOB); // header code lower bound

    msWrite2Byte(REG_003D86, IR_OFC_UPB); // off code upper bound
    msWrite2Byte(REG_003D88, IR_OFC_LOB); // off code lower bound

    msWrite2Byte(REG_003D8A, IR_OFC_RP_UPB); // off code repeat upper bound
    msWrite2Byte(REG_003D8C, IR_OFC_RP_LOB); // off code repeat lower bound

    msWrite2Byte(REG_003D8E, IR_LG01H_UPB); // logical 0/1 high upper bound
    msWrite2Byte(REG_003D90, IR_LG01H_LOB); // logical 0/1 high lower bound

    msWrite2Byte(REG_003D92, IR_LG0_UPB); // logical 0 upper bound
    msWrite2Byte(REG_003D94, IR_LG0_LOB); // logical 0 lower bound

    msWrite2Byte(REG_003D96, IR_LG1_UPB); // logical 1 upper bound
    msWrite2Byte(REG_003D98, IR_LG1_LOB); // logical 1 lower bound

    msWrite2Byte(REG_003D9E, ucRPTimeOutTemp); // repeat time out [0:15]
    msWriteByteMask(REG_003DA0, ucRPTimeOutTemp>>16, 0x07); // repeat time out [16:18]
    msWriteByteMask(REG_003DA0, 0x30, 0x30); // Ir timeout clear set
    msWriteByte(REG_003DA1, 0x9F);
    msWriteByte(REG_003DA3, 0x0F); // FIFO control FIFO full enable fifo_depth = 7
    msWrite2Byte(REG_003DA4, ((IR_HEADER_CODE1 << 8) | IR_HEADER_CODE0));
    msWriteByte(REG_003DA6, 0x00); // GLHRM number[7:0]
    msWriteByte(REG_003DA7, 0x30); // [2:0]->GLHRM number[10:8], [5:4]FULL MODE
    msWriteByte(REG_003DA8, IR_CKDIV_NUM); // clock divider

}
void irInterruptProcess(void)
{
    if(!(MEM_MSREAD_BYTE_EXT1(REG_003DAD) & _BIT1)) // buffer not empty
    {
        g_ucIrCode = MEM_MSREAD_BYTE_EXT1(REG_003DA9); // read ir code
        MEM_MSWRITE_BIT_ISR(_EXT1, REG_003DB0, TRUE, _BIT0);//Read Pulse counter + 1
        g_bIrDetect = 1;      // inform main to print ir code
        irDelayNop(4);
        if(MEM_MSREAD_BYTE_EXT1(REG_003DAD) & _BIT0) // check repeat flag
        {
            g_bIRRepeat = 1;
        }
        else
        {
            g_bIRRepeat = 0;
            g_bIRRepeatTimeOut = 0;
            g_wIRRepeatTimer = 0;
        }
    }
}
void irPollingHandler(void)
{

}
#elif(IR_MODE_SEL==IR_FULL_POL_NEC_MODE)
void irInitialize(void)
{
    DWORD xdata ucRPTimeOutTemp;
    ucRPTimeOutTemp = IR_RP_TIMEOUT;

    g_ucIrCode = 0x00;
    g_bIRRepeat = 0;
    g_IRBufferEmpty = 0;
    //msWriteByte(REG_038E,msReadByte(REG_038E)&~(_BIT0|_BIT1));//set clock gen to 1Mhz   //?

    msWriteByteMask(REG_000407, 0x80, 0x80);    //enable GPIO as IR in
    msWriteByte(REG_003DA1, 0x9F);

    msWrite2Byte(REG_003D80, 0x01FF);

    msWrite2Byte(REG_003D82, IR_HDC_UPB);
    msWrite2Byte(REG_003D84, IR_HDC_LOB);

    msWrite2Byte(REG_003D86, IR_OFC_UPB);
    msWrite2Byte(REG_003D88, IR_OFC_LOB);

    msWrite2Byte(REG_003D8A, IR_OFC_RP_UPB);
    msWrite2Byte(REG_003D8C, IR_OFC_RP_LOB);

    msWrite2Byte(REG_003D8E, IR_LG01H_UPB);
    msWrite2Byte(REG_003D90, IR_LG01H_LOB);

    msWrite2Byte(REG_003D92, IR_LG0_UPB);
    msWrite2Byte(REG_003D94, IR_LG0_LOB);

    msWrite2Byte(REG_003D96, IR_LG1_UPB);
    msWrite2Byte(REG_003D98, IR_LG1_LOB);

    msWrite2Byte(REG_003D9E, ucRPTimeOutTemp);    //repeat time out [0:15]
    msWriteByteMask(REG_003DA0,ucRPTimeOutTemp>>16,0x07);    //repeat time out [16:18]
    msWriteByteMask(REG_003DA0,0x30,0x30);    //repeat time out [16:18]
    msWriteByte(REG_003DA3, 0x0F);    //FIFO Depth and Full enable

    //msWriteByte(REG_003DA7, msReadByte(REG_003DA7) | 0x30);
    msWriteByte(REG_003DA7, msReadByte(REG_003DA7) | 0x00); //IR decode mode
    //msWriteByte(REG_003DA1, 0x1F);
    msWrite2Byte(REG_003DA4, ((IR_HEADER_CODE1 << 8) | IR_HEADER_CODE0)); //customer code
    msWriteByte(REG_003DA8, IR_CKDIV_NUM);

    #if 1
    msWriteByte(REG_003DA6, 0x00);    //glitch removal
    msWriteByte(REG_003DA7, msReadByte(REG_003DA7) & 0xF8);
    #else
    msWrite2Byte(REG_003DA6, msRead2Byte(REG_003DA6) | 0x0CB0);
    #endif

}
void irInterruptProcess(void)
{

}
void irPollingHandler(void)
{
//    BYTE RegDat;
    BYTE xdata ucCodeBff, ucDummy;


//
//    RegDat = msReadByte(BK0_FC);
//    if(RegDat & _BIT6)
//        msWriteByte(BK0_FC,RegDat&~_BIT6);
//    else
//    {
//
//        return;
//    }

    ucDummy = 10;
    while(ucDummy--)
    {
        if (msReadByte(REG_003DAD) & _BIT1) // check FIFO is empty
            break;
        else
        {
            //ucCodeBff = msReadByte(BKA_99);
            ucCodeBff = msReadByte(REG_003DA9);
            msWriteBit(REG_003DB0, TRUE,_BIT0);//Read Pulse counter + 1
            IR_printData("polling IR = %x",ucCodeBff);
            g_bIrDetect = 1;
            //Delay1ms(1); // must delay over 4 machine cycle
            irDelayNop(4);
            if((msReadByte(REG_003DAD)&_BIT0))// && (ucCodeBff==g_ucIrCode)) // check repeat flag
            {
                g_bIRRepeat = 1;
              #if IR_DEBUG_EN
                IR_printData("\r\nrepeat=%x",g_ucIrCode);
              #endif
            }
            else
            {
                g_ucIrCode = ucCodeBff;
            #if IR_DEBUG_EN
                IR_printData("\r\nfirst code=%x",g_ucIrCode);
            #endif
                g_bIRRepeat = 0;
                g_bIRRepeatTimeOut = 0;
                g_wIRRepeatTimer = 0;
            }
        }
    } // while
}

#elif(IR_MODE_SEL == IR_RAW_DATA_POL_MODE)
void irInitialize(void)
{
    g_ucIrCode = 0x00;
    g_bIRRepeat = 0;
    g_IRRawModeDone = 0;
    g_ucIRRawModeCount = 0;
    //msWriteByte(REG_038E,msReadByte(REG_018C)&~(_BIT0|_BIT1));//set clock gen to 1Mhz   //?
    msWriteByteMask(REG_000407, 0x80, 0x80);    //enable GPIO as IR in
    msWriteByte(REG_003DA1, 0x9F);

    msWrite2Byte(REG_003D80, 0x01FF);

    msWrite2Byte(REG_003D82, IR_HDC_UPB); // header code upper bound
    msWrite2Byte(REG_003D84, IR_HDC_LOB); // header code lower bound

    msWrite2Byte(REG_003D86, IR_OFC_UPB); // off code upper bound
    msWrite2Byte(REG_003D88, IR_OFC_LOB); // off code lower bound

    msWrite2Byte(REG_003D8A, IR_OFC_RP_UPB);  // off code repeat upper bound
    msWrite2Byte(REG_003D8C, IR_OFC_RP_LOB);  // off code repeat lower bound

    msWrite2Byte(REG_003D8E, IR_LG01H_UPB);   // logical 0/1 high upper bound
    msWrite2Byte(REG_003D90, IR_LG01H_LOB);   // logical 0/1 high lower bound

    msWrite2Byte(REG_003D92, IR_LG0_UPB); // logical 0 upper bound
    msWrite2Byte(REG_003D94, IR_LG0_LOB); // logical 0 lower bound

    msWrite2Byte(REG_003D96, IR_LG1_UPB); // logical 1 upper bound
    msWrite2Byte(REG_003D98, IR_LG1_LOB); // logical 1 lower bound

    msWriteByte(REG_003DA7, msReadByte(REG_003DA7) | 0x20); //IR decode mode

    //msWrite3Bytes(BKA_8E, IR_RP_TIMEOUT); // Ir timerout counter
    msWrite2Byte(REG_003D9A, IR_RP_TIMEOUT);
    //msWriteByte(REG_003DA1, 0x1F);
    //msWriteByte(BKA_96, IR_CKDIV_NUM); // clock divider
    msWriteByte(REG_003DA8, IR_CKDIV_NUM);

    //msWriteByte(BKA_97, 0x00); // GLHRM number
    msWriteByte(REG_003DA6, 0x00);
    msWriteByte(REG_003DA7, msReadByte(REG_003DA7) & 0xF8);


}
void irInterruptProcess(void)
{

}
void irPollingHandler(void)
{
    //if(!(msReadByte(BKA_9D) & _BIT1)) // check FIFO not empty
    if(!(msReadByte(REG_003DAD) & _BIT1)) // check FIFO not empty
    {
        g_IRRawData[g_ucIRRawModeCount] = msReadByte(REG_003DA9);
        msWriteBit(REG_003DB0, TRUE,_BIT0);//Read Pulse counter + 1
        g_ucIRRawModeCount++;
        if(g_ucIRRawModeCount==IR_RAW_DATA_NUM)
        {
            g_ucIRRawModeCount = 0;
            g_IRRawModeDone = 1;
        }
    }
    if(g_IRRawModeDone)
    {
        g_IRRawModeDone = 0;
        if(g_IRRawData[0]==IR_HEADER_CODE0)
        {
            if(g_IRRawData[1]==IR_HEADER_CODE1)
            {
                if((g_IRRawData[2]^g_IRRawData[3])== 0xFF)
                {
                    g_ucIrCode = g_IRRawData[2];
                    g_bIrDetect = 1;
                }
            }
        }
    }
}

#elif(IR_MODE_SEL == IR_RAW_DATA_INT_MODE)
void irInitialize(void)
{
    g_ucIrCode = 0xFF;
    g_bIRRepeat = 0;
    g_IRRawModeDone = 0;
    g_ucIRRawModeCount = 0;
    //msWriteByte(REG_038E,msReadByte(REG_018C)&~(_BIT0|_BIT1));//set clock gen to 1Mhz   //?
    INT_FIQ_IR_ENABLE(TRUE);    //enable FIQ mask
    msWriteBit(REG_002B28,0,BIT1);  //PM IRQ

    msWriteByteMask(REG_000407, 0x80, 0x80);    //enable GPIO as IR in
    msWrite2Byte(REG_003D80, 0x01BF);

    msWrite2Byte(REG_003D82, IR_HDC_UPB); // header code upper bound
    msWrite2Byte(REG_003D84, IR_HDC_LOB); // header code lower bound

    msWrite2Byte(REG_003D86, IR_OFC_UPB); // off code upper bound
    msWrite2Byte(REG_003D88, IR_OFC_LOB); // off code lower bound

    msWrite2Byte(REG_003D8A, IR_OFC_RP_UPB);  // off code repeat upper bound
    msWrite2Byte(REG_003D8C, IR_OFC_RP_LOB);  // off code repeat lower bound

    msWrite2Byte(REG_003D8E, IR_LG01H_UPB);   // logical 0/1 high upper bound
    msWrite2Byte(REG_003D90, IR_LG01H_LOB);   // logical 0/1 high lower bound

    msWrite2Byte(REG_003D92, IR_LG0_UPB); // logical 0 upper bound
    msWrite2Byte(REG_003D94, IR_LG0_LOB); // logical 0 lower bound

    msWrite2Byte(REG_003D96, IR_LG1_UPB); // logical 1 upper bound
    msWrite2Byte(REG_003D98, IR_LG1_LOB); // logical 1 lower bound

    msWriteByte(REG_003DA7, msReadByte(REG_003DA7) | 0x20); //IR decode mode
    //msWrite3Bytes(BKA_8E, IR_RP_TIMEOUT); // Ir timerout counter
    msWrite2Byte(REG_003D9A, IR_RP_TIMEOUT);
    msWriteByte(REG_003DA1, 0x9F);
    //msWriteByte(BKA_96, IR_CKDIV_NUM); // clock divider
    msWriteByte(REG_003DA8, IR_CKDIV_NUM);

    //msWriteByte(BKA_97, 0x00); // GLHRM number
    msWriteByte(REG_003DA6, 0x00);
    msWriteByte(REG_003DA7, msReadByte(REG_003DA7) & 0xF8);


}
void irInterruptProcess(void)
{
    {
        BYTE i;//,ucBank;
        for(i=0;i!=IR_RAW_DATA_NUM;i++)
        {
            g_IRRawData[i] = MEM_MSREAD_BYTE_EXT1(REG_003DA9);
            MEM_MSWRITE_BIT_ISR(_EXT1, REG_003DB0, TRUE, _BIT0);//Read Pulse counter + 1
            irDelayNop(1);
        }
        g_IRRawModeDone = 1;
    }
}
void irPollingHandler(void)
{
    if(g_IRRawModeDone)
    {
        g_IRRawModeDone = 0;

//printData("g_IRRawData[0]==%d",g_IRRawData[0]);
//printData("g_IRRawData[1]==%d",g_IRRawData[1]);
//printData("g_IRRawData[2]==%d",g_IRRawData[2]);
//printData("g_IRRawData[3]==%d",g_IRRawData[3]);

        if(g_IRRawData[0]==IR_HEADER_CODE0
#ifdef IR_MODE_SEL_4_Customer
  ||g_IRRawData[0]==IR_HEADER_CODE6_Customer
#endif
	)
        {
            if(g_IRRawData[1]==IR_HEADER_CODE1
#ifdef IR_MODE_SEL_4_Customer
  ||g_IRRawData[1]==IR_HEADER_CODE7_Customer
#endif
	      )
            {
                if(g_IRRawData[2]==(BYTE)~g_IRRawData[3])
                {

                #ifdef IR_MODE_SEL_2
                    Clr_IR_Mode_Sel_Flag();
		  #endif
                #ifdef IR_MODE_SEL_3
                    Clr_IR_Mode_Sel3_Flag();
		  #endif
                #ifdef IR_MODE_SEL_4
                    Clr_IR_Mode_Sel4_Flag();
		  #endif
                #ifdef IR_MODE_SEL_5
                    Clr_IR_Mode_Sel5_Flag();
		  #endif
                    g_ucIrCode = g_IRRawData[2];
                    g_bIrDetect = 1;
                }
                #if IR_DEBUG_EN
                else
                    IR_printMsg("\r\nData checksum FAIL!");
                #endif
            }
            #if IR_DEBUG_EN
            else
                IR_printMsg("\r\nHEADER1 FAIL!");
            #endif
         }
#ifdef IR_MODE_SEL_2
        else if(g_IRRawData[0]==IR_HEADER_CODE2)
        {
            if(g_IRRawData[1]==IR_HEADER_CODE3)
            {
                if(g_IRRawData[2]==(0xFF&(~g_IRRawData[3])))
					
                {
                    Set_IR_Mode_Sel_Flag();
                    g_ucIrCode = g_IRRawData[2];
                    g_bIrDetect = 1;
                }
                #if IR_DEBUG_EN
                else
                    IR_printMsg("\r\nData checksum FAIL!");
                #endif
            }
            #if IR_DEBUG_EN
            else
                IR_printMsg("\r\nHEADER1 FAIL!");
            #endif
         }
#endif		
#ifdef IR_MODE_SEL_3
        else if(g_IRRawData[0]==IR_HEADER_CODE4)
        {
            if(g_IRRawData[1]==IR_HEADER_CODE5)
            {
                if(g_IRRawData[2]==(0xFF&(~g_IRRawData[3])))
					
                {
                    Set_IR_Mode_Sel3_Flag();
                    Clr_IR_Mode_Sel_Flag();
                  g_ucIrCode = g_IRRawData[2];
                    g_bIrDetect = 1;
                }
                #if IR_DEBUG_EN
                else
                    IR_printMsg("\r\nData checksum FAIL!");
                #endif
            }
            #if IR_DEBUG_EN
            else
                IR_printMsg("\r\nHEADER1 FAIL!");
            #endif
         }
#endif		
#ifdef IR_MODE_SEL_4
        else if(g_IRRawData[0]==IR_HEADER_CODE6||g_IRRawData[0]==IR_HEADER_CODE6_2)
        {
            if(g_IRRawData[1]==IR_HEADER_CODE7||g_IRRawData[1]==IR_HEADER_CODE7_2)
            {
                if(g_IRRawData[2]==(0xFF&(~g_IRRawData[3])))
					
                {
                    Set_IR_Mode_Sel4_Flag();
                    Clr_IR_Mode_Sel3_Flag();
                    Clr_IR_Mode_Sel_Flag();
                  g_ucIrCode = g_IRRawData[2];
                    g_bIrDetect = 1;
                }
                #if IR_DEBUG_EN
                else
                    IR_printMsg("\r\nData checksum FAIL!");
                #endif
            }
            #if IR_DEBUG_EN
            else
                IR_printMsg("\r\nHEADER1 FAIL!");
            #endif
         }
#endif		
#ifdef IR_MODE_SEL_5
        else if(g_IRRawData[0]==IR_HEADER_CODE8_Low)
        {
            if(g_IRRawData[1]==IR_HEADER_CODE9_High)
            {
                if(g_IRRawData[2]==(0xFF&(~g_IRRawData[3])))
                {
                    Set_IR_Mode_Sel5_Flag();
		      Clr_IR_Mode_Sel4_Flag();
                    Clr_IR_Mode_Sel3_Flag();
                    Clr_IR_Mode_Sel_Flag();
                    g_ucIrCode = g_IRRawData[2];
                    g_bIrDetect = 1;
                }
                #if IR_DEBUG_EN
                else
                    IR_printMsg("\r\nData checksum FAIL!");
                #endif
            }
            #if IR_DEBUG_EN
            else
                IR_printMsg("\r\nHEADER1 FAIL!");
            #endif
         }
#endif		

		
        #if IR_DEBUG_EN
        else
            IR_printMsg("\r\nHEADER0 FAIL!");
        #endif
    }
}

#elif(IR_MODE_SEL == IR_SOFTWARE_MODE)
void irInitialize(void)
{
    g_ucIrCode = 0x00;
    g_bIRRepeat = 0;
    g_ucIRSWModeCount = 0;
    g_bIRSWDone = 0;
    g_bIRStart = 0;
    g_bIrDetect = 0;
    g_wIRRepeatTimer = 0;
    g_bIRRepeatTimeOut = 0;
    g_bIRSWDecoding = 0;
    //msWriteByte(REG_038E,msReadByte(REG_018C)&~(_BIT0|_BIT1));//set clock gen to 1Mhz   //?
    INT_FIQ_IR_ENABLE(TRUE);
    msWriteBit(REG_002B28,0,BIT1);  //PM IRQ
    msWriteByteMask(REG_000407, 0x80, 0x80);    //enable GPIO as IR in

    msWrite2Byte(REG_003D80, 0x01BF);
    //msWriteByte(BKA_95, IR_SW_INT_NEG_TRIG); //NEG
    msWriteByte(REG_003DA3, 0x07 | _BIT5);    //internal counter select P/N shot

    //msWriteByte(BKA_98, 0x10);

    msWriteByte(REG_003DA7, msReadByte(REG_003DA7) | 0x10);
    //msWriteByte(BKA_96, IR_CKDIV_NUM); // clock divider
    msWriteByte(REG_003DA8, SW_IR_CKDIV_NUM);

    //msWriteByte(BKA_97, 0x00); // GLHRM number
    msWriteByte(REG_003DA6, 0x00);
    msWriteByte(REG_003DA7, msReadByte(REG_003DA7) & 0xF8);


}
void irInterruptProcess(void)
{
    if(g_bIRSWDecoding)
        return;

    g_wIRCounter[g_ucIRSWModeCount] = ((DWORD)(MEM_MSREAD_BYTE_EXT1(REG_003DAC)&0x07) << 16) + ((DWORD)MEM_MSREAD_BYTE_EXT1(REG_003DAB) << 8) + MEM_MSREAD_BYTE_EXT1(REG_003DAA);

    if(g_ucIRSWModeCount==0)
    {
        if(!(g_wIRCounter[g_ucIRSWModeCount]< HEADER_UPPER_BOND && g_wIRCounter[g_ucIRSWModeCount] > HEADER_LOWER_BOND))
        {
            //printMsg("IRCounter is not between high/low bound");
            //printData("g_ucIRSWModeCount = %d",g_ucIRSWModeCount);
            //printData("g_wIRCounter[]=%x", g_wIRCounter[g_ucIRSWModeCount]);
            g_ucIRSWModeCount = 0;
            return;
        }
    }
    g_ucIRSWModeCount++;

    if(g_ucIRSWModeCount == IR_SW_DATA_NUM+1)
    {
       g_bIRSWDone = 1;
       g_ucIRSWModeCount = 0;
    }
}
void irPollingHandler(void)
{

    BYTE xdata i;
    if(g_bIRSWDone)
    {
        g_bIRSWDone = 0;
        g_bIRSWDecoding = 1;
        for(i=1;i<IR_SW_DATA_NUM+1;i++)
        {
            g_ucKeyData >>= 1;
            if(g_wIRCounter[i]>= ZERO_LB && g_wIRCounter[i]<=ZERO_UB)
                g_ucKeyData &= ~(_BIT7);
            else if(g_wIRCounter[i]>= ONE_LB && g_wIRCounter[i]<=ONE_UB)
                g_ucKeyData |= _BIT7;

            switch(i)
            {
                case 0x08:
                    if(g_ucKeyData != IR_HEADER_CODE0)
                    {
                        #if IR_DEBUG_EN
                        IR_printMsg("\r\n custom code 0 error");
                        #endif
                        g_ucIRSWModeCount = 0;
                        g_bIRSWDecoding = 0;
                        return;
                    }
                    break;
                case 0x10:
                    if(g_ucKeyData != IR_HEADER_CODE1)
                    {
                        #if IR_DEBUG_EN
                        IR_printData("\r\nucKeyData %x ",g_ucKeyData);
                        IR_printMsg("\r\n custom code 1 error");
                        g_ucIRSWModeCount = 0;
                        g_bIRSWDecoding = 0;
                        return;
                        #endif
                        return;
                    }
                    break;
                case 0x18:
                    g_ucIrCode = g_ucKeyData;
                    IR_printData("\r\nucKeyData %x ",g_ucKeyData);
                    break;
                case 0x20:
                    if(g_ucIrCode == (BYTE)~g_ucKeyData)
                    {
                        g_bIrDetect = 1;
                        IR_printData("\r\nucKeyData %x ",g_ucKeyData);
                        IR_printData("\r\n s/w code %x",g_ucIrCode);
                    }
                    #if IR_DEBUG_EN
                    else
                    {
                        g_bIRSWDecoding = 0;
                        g_ucIRSWModeCount = 0;
                        IR_printMsg("\r\nnot match");
                        IR_printData("\r\nucKeyData %x ",g_ucKeyData);
                        IR_printData("\r\n~ucKeyData %x ",(BYTE)~g_ucKeyData);
                        IR_printData("\r\n s/w code %x",g_ucIrCode);
                        return;
                    }
                    #endif
                    break;
            }
        }
        g_bIRSWDecoding = 0;
    }

}
#elif(IR_MODE_SEL == IR_FULL_INT_RC5_EXT_MODE || IR_MODE_SEL == IR_FULL_INT_RC5_MODE)
void irInitialize(void)
{
    //msWriteByte(REG_038E,msReadByte(REG_018C)&~(_BIT0|_BIT1));//set clock gen to 1Mhz   //?
    INT_FIQ_IR_RC_ENABLE(TRUE);    //enable FIQ mask
    msWriteBit(REG_002B28,0,BIT1);  //PM IRQ

    msWriteByteMask(REG_000407, 0x80, 0x80);    //enable GPIO as IR in
    msWrite2Byte(REG_003D80, 0x0080);// [0]disable Ray IR,[7]Eanble IR Inverse,[6]IR Int Mask

    #if(IR_MODE_SEL == IR_FULL_INT_RC5_MODE)
    msWriteByte(REG_003D01,0x01);
    #else
    msWriteByte(REG_003D01,0x05);
    #endif
    //long pulse detection
    msWrite2Byte(REG_003D02, LONG_BIT_BOUNDARY);
    //high keep time
    msWriteByteMask(REG_003D06, H_KEEP_TIME<<1, 0xFE);
    //clk divisor ratio
    msWriteByte(REG_003D07, 0x08);
    //watch dog timer
    msWriteByte(REG_003D08,WATCH_DOG_COUNT);
    //time out
    msWriteByte(REG_003D09,TIME_OUT_COUNT);
}
void irInterruptProcess(void)
{
    BYTE xdata ucAddress;
    if(!(MEM_MSREAD_BYTE_EXT1(REG_003D10) & _BIT0)) // buffer not empty
    {
        ucAddress = MEM_MSREAD_BYTE_EXT1(REG_003D0C)& 0x1F;
        if(ucAddress == IR_RC5_ADDR1 || ucAddress == IR_RC5_ADDR2)
        {
            #if(IR_MODE_SEL == IR_FULL_INT_RC5_MODE)
            g_ucIrCode = MEM_MSREAD_BYTE_EXT1(REG_003D0D)& 0x3F;    //? 0x3F
            #else
            g_ucIrCode = MEM_MSREAD_BYTE_EXT1(REG_003D0D)& 0x7F;
            #endif
            MEM_MSWRITE_BYTE(REG_003D12, _BIT0);//Read Pulse counter + 1

            if(g_ucIrCode & 0x80)
                g_bIRRepeat = 1;
            else
            {
                g_bIRRepeat = 0;
                g_bIRRepeatTimeOut = 0;
                g_wIRRepeatTimer = 0;
            }
            g_bIrDetect = 1;
        }
        irDelayNop(4);
//for future use -> toggle = g_ucIrCode & 0x20;
    }
}
void irPollingHandler(void)
{

}
#elif(IR_MODE_SEL == IR_FULL_INT_RC6_HEAD888MS_MODE || IR_MODE_SEL == IR_FULL_INT_RC6_HEAD444MS_MODE)
void irInitialize(void)
{
    //msWriteByte(REG_038E,msReadByte(REG_018C)&~(_BIT0|_BIT1));//set clock gen to 1Mhz   //?
    //msWriteByte(REG_2B03, msReadByte(REG_2B03) & ~(_BIT6));//enable IR interrupt  //?
    INT_FIQ_IR_RC_ENABLE(TRUE);    //enable FIQ mask
    msWriteBit(REG_002B28,0,BIT1);  //PM IRQ

    msWrite2Byte(REG_003D80, 0x0080);// [0]disable Ray IR,[7]Eanble IR Inverse,[6]IR Int Mask
    msWriteByte(REG_003D01,0x03);   //[7]=Jerry Inverse, please disable Jerry Invert,Enable Ray Inverse.
    msWriteByteMask(REG_000407, 0x80, 0x80);    //enable GPIO as IR in

    //if REG3D_00[5] = 0,then we muse manual set long pulse detection
    //msWrite2Byte(REG_0302, LONG_BIT_BOUNDARY); = 1766*3/4
    //else if REG3D_00[5] = 1
    //automatically 2666 / 6 = 444
    //if REG_0306[0] = 1 -> long pulse = 444 * 3 / 4
    //if REG_0306[0] = 0 -> long pulse = 444 * 2 * 3 / 4
    #if(IR_MODE_SEL == IR_FULL_INT_RC6_HEAD444MS_MODE)
        msWriteBit(REG_003D06,TRUE,_BIT0);
    #else
        msWriteBit(REG_003D06,FALSE,_BIT0);
    #endif

    //rc-6 traler bit long pulse calibration, (444+888) - LONG_PULSE_CALIBRATION_COUNT
    //msWrite2Byte(REG_0304,LONG_PULSE_CALIBRATION_COUNT);
    msWriteByte(REG_003D04, LONG_PULSE_CALIBRATION_COUNT);    //?
    msWriteByteMask(REG_003D05, LONG_PULSE_CALIBRATION_COUNT>>8, _BIT1|_BIT0);
    //high keep time
    msWriteByteMask(REG_003D06, H_KEEP_TIME<<1, 0xFE);
    //clk divisor ratio
    msWriteByte(REG_003D07, 0x08);
    //watch dog timer
    msWriteByte(REG_003D08,WATCH_DOG_COUNT);
    //time out
    msWriteByte(REG_003D09,TIME_OUT_COUNT);
}
void irInterruptProcess(void)
{
    BYTE ucAddress,ucRC6Misc;

    if(!(MEM_MSREAD_BYTE_EXT1(REG_003D10) & _BIT0)) // buffer not empty
    {
        ucAddress = MEM_MSREAD_BYTE_EXT1(REG_003D0C);
        if(ucAddress == IR_RC6_ADDR)
        {
            g_ucIrCode = MEM_MSREAD_BYTE_EXT1(REG_003D0D);
            ucRC6Misc = MEM_MSREAD_BYTE_EXT1(REG_003D0E);
            MEM_MSWRITE_BYTE(REG_003D12, _BIT0);//Read Pulse counter + 1
            if(ucRC6Misc & 0x10)
                g_bIRRepeat = 1;
            else
            {
                g_bIRRepeat = 0;
                g_bIRRepeatTimeOut = 0;
                g_wIRRepeatTimer = 0;
            }
            g_bIrDetect = 1;
        }
        irDelayNop(4);
//for future use -> mode = ucRC6Misc & 0x07;
//for future use -> toggle = ucRC6Misc & 0x08;
    }
}
void irPollingHandler(void)
{

}

#endif
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void irDetectTimer0( void )
{
#if(IR_MODE_SEL == IR_SOFTWARE_MODE || IR_MODE_SEL==IR_FULL_POL_NEC_MODE || IR_MODE_SEL==IR_FULL_INT_NEC_MODE)
    if(g_wIRRepeatTimer)
        if((--g_wIRRepeatTimer)==0)
            g_bIRRepeatTimeOut = 1;
#endif

    //--- IR press 0-9 key ending timer for TUNER Channel---
    if ( g_bIrNumKeyStart || g_bIr100Key ) // IR key start
    {
        g_wIrNumKeyTimer--;
        if ( g_wIrNumKeyTimer == 0 ) // time out
        {
            if ( g_bIr100Key )
            {
                g_bIr100Key = 0;
                g_ucIrNumKey = ( g_ucIrNumKey * 10 ) + 100;
            }
            g_bIrNumKeyStart = 0;
            g_bIrKeyNumEnd = 1;
            g_wIrNumKeyTimer = 200;
        }
    } // if IR key start
    //if (g_bIrKeyNumEnd && g_wIrNumKeyTimer)
    //    if ((--g_wIrNumKeyTimer) == 0)
    //        g_bIrKeyNumEndDelay = 1;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void irDecodeRepaeat(WORD wRepeatTime)
{
#if(!(IR_MODE_SEL==IR_RAW_DATA_POL_MODE || IR_MODE_SEL==IR_RAW_DATA_INT_MODE))

    if (g_bIRRepeat)
        g_wIRRepeatTimer = wRepeatTime;
    else
        g_wIRRepeatTimer = IR_REPEAT_START_TIME;
#else
    wRepeatTime=wRepeatTime;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void irDecodeEnd( void )
{
}
#endif  //(IR_REMOTE_SEL != NO_REMOTE)

