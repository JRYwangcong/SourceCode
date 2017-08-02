#define _drvDPTXAPP_7_C_
#include "Board.h"
#include <string.h>
#include <math.h>
#include "datatype.h"
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
#include "drvDPTxApp.h"
#include <msScaler.h>
#include "DDC2Bi.H"
#include "msACE.h"
#include "NVRam.h"
#include "drvMcu.h"
#include "Common.h"
#include "msEread.h"

#define eDPTX_DEBUG 1
#if (eDPTX_DEBUG&&ENABLE_MSTV_UART_DEBUG)
#define eDPTX_printData(str, value)   printData(str, value)
#define eDPTX_printMsg(str)               printMsg(str)
#else
#define eDPTX_printData(str, value)
#define eDPTX_printMsg(str)
#endif


#if ENABLE_DP_OUTPUT

#define AuxWaitReplyLpCntNum 2000
#define AuxWaitRdPluseLpCntNum 200  // 200 nop delay
#define AuxWaitWtPluseLpCntNum 10    // 10 nop delay

#define eDPTXMODPhyOffSet 0

// #############   eDPTXAutoTest  Define  ########################
#define eDPTXAutoTest                 0     // must set  #define eDPTXDETECTHPD 1
#if eDPTXAutoTest
#define TEST_LINK_TRAINING      1
#define TEST_PATTERN                0
#define TEST_EDID_READ             0
#define PHY_TEST_PATTERN       1
#if PHY_TEST_PATTERN
#define TEST_D102                        1
#define TEST_SYMBERR              0
#define TEST_PRBS7                    0
#define TEST_PHY80B                  0
#define TEST_HBR2EYE               0
#endif
#endif
// #############################################################

WORD xdata WaitRdPluseDelayCount;
WORD xdata WaitReplyCount;

BYTE xdata eDPTxCalX[4][4]={{4,5,5,5},{6,6,6,6},{8,8,8,8},{8,8,8,8}};
BYTE xdata eDPTxCalY[4][4]={{0,8,14,14},{0,8,15,15},{0,20,20,20},{0,20,20,20}};

BYTE xdata eDPTxSwingCalD[4][4]=
{   {eDP_SWING_DefaultValue,eDP_SWING_DefaultValue,eDP_SWING_DefaultValue,eDP_SWING_DefaultValue},
    {eDP_SWING_DefaultValue,eDP_SWING_DefaultValue,eDP_SWING_DefaultValue,eDP_SWING_DefaultValue},
    {eDP_SWING_DefaultValue,eDP_SWING_DefaultValue,eDP_SWING_DefaultValue,eDP_SWING_DefaultValue},
    {eDP_SWING_DefaultValue,eDP_SWING_DefaultValue,eDP_SWING_DefaultValue,eDP_SWING_DefaultValue}
};
BYTE xdata eDPTxPremphasisCalD[4][4]=
{   {eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue},
    {eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue},
    {eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue},
    {eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue,eDP_PREMPHASIS_DefaultValue}
};


#if 1 //eDPTXDETECTHPD
#define eDPTXHPDUSEGPIOGroupBIT   (BIT4<<eDPTXHPDUseGPIOGroupNum)
#define eDPTXHPDP0_IRQEvent             (BIT2<<(eDPTXHPDPort0*3))
#define eDPTXHPDP0_IRQDisConnect    (BIT1<<(eDPTXHPDPort0*3))
#define eDPTXHPDP0_IRQConnect         (BIT0<<(eDPTXHPDPort0*3))
#define eDPTXHPDP1_IRQEvent             (BIT2<<(eDPTXHPDPort1*3))
#define eDPTXHPDP1_IRQDisConnect   (BIT1<<(eDPTXHPDPort1*3))
#define eDPTXHPDP1_IRQConnect         (BIT0<<(eDPTXHPDPort1*3))
#endif

void DPTxInit( void )
{

#if eDPTXSWAP
eDPTX_printMsg("## eDPTXSWAP ##\r\n");
#endif

eDPTX_printMsg("eDP TX Init \r\n");

#if eDPTXDETECTHPD
// ## Overwrite HPD
msWriteByte( REG_1408A2,0x00); // [3:0] HPD Soft Ov En   [7:4] Ov Value
// ## HPD initial setting
msWriteByteMask(REG_000417,eDPTXHPDUSEGPIOGroupBIT,BIT7|BIT6|BIT5|BIT4); // [4] en HPD Gp0(50~53) [5] enHPD Gp1(54~57) [6]en IO50 as HPD [7]en IO54 as HPD
//msWriteByte( REG_1408A3,0xFF); // [3:0] HPD GPIO Oen   [7:4] GPIO Output Value
//msWriteByteMask( REG_1408A3,eDPTXHPDPort0|eDPTXHPDPort1,eDPTXHPDPort0|eDPTXHPDPort1); // [3:0] HPD GPIO Oen   [7:4] GPIO Output Value
msWriteByteMask( REG_1408A3,(BIT0<<eDPTXHPDPort0), (BIT0<<eDPTXHPDPort0)); // [3:0] HPD GPIO Oen   [7:4] GPIO Output Value
msWriteByteMask( REG_1408A3,(BIT0<<eDPTXHPDPort1), (BIT0<<eDPTXHPDPort1)); // [3:0] HPD GPIO Oen   [7:4] GPIO Output Value
msWriteByteMask( REG_1408A5,BIT6,BIT6); //[6] En Xtal Clock
msWriteByte( REG_1408AE,0x30); //12M Xtal Count 1us [7:2] integer [1:0]Fractional
msWriteByte( REG_1408A0,0xA8); // [3:0] debounce Xtal*8   [7:4] HPD int event [5:4] lower 100us [7:6] upper 700us  // 200us per step
msWriteByte( REG_1408A1,0x00); // [3:0] disconnect 1.5ms+N*100us  [7:4] connect 1.5ms+N*100us
msWrite2Byte(REG_1408A8,0x0000); // HPD IRQ mask
msWrite2Byte(REG_1408AA,0x0000); // HPD IRQ force
msWrite2Byte(REG_1408AC,0x0FFF);  // clear IRQ
msWrite2Byte(REG_1408AC,0x0000);  // clear IRQ;
#else
// ## Overwrite HPD
msWriteByte( REG_1408A2,0xFF); // [3:0] HPD Soft Ov En   [7:4] Ov Value
#endif
// ## P0 AUXTX atop setting
msWriteByte( REG_112639,0x00 ); //wriu 0x112639  0x00
msWriteByte( REG_11263F,0x14); //wriu 0x11263F  0x14
msWriteByte( REG_11261B,0x0B);//wriu 0x11261B  0x0B  //  Sample Rate for 12M
msWriteByteMask( REG_11260A,0x07,0x7F);//wriu 0x11260A  [6:0] 0x07  // UI for 12M
// ## P1 AUXTX atop setting
msWriteByte( REG_112639+0x040,0x00 ); //wriu 0x112639  0x00
msWriteByte( REG_11263F+0x040,0x14); //wriu 0x11263F  0x14
msWriteByte( REG_11261B+0x040,0x0B);//wriu 0x11261B  0x0B  //  Sample Rate for 12M
msWriteByteMask( REG_11260A+0x040,0x07,0x7F);//wriu 0x11260A  [6:0] 0x07  // UI for 12M
//## AUXTX P/N swap
//msWriteByte( REG_112607,0xC0); //[7]:swap //wriu 0x112607  0xC0
// ## timeout+P/N swap setting
#if eDPTXAuxP0PNSWAP
msWriteByte( REG_112607,0xC8);//0x112607, 8wC8, 8wC8 PN Swap
#else
msWriteByte( REG_112607,0x48);//0x112607, 8wC8 PN no Swap
#endif
#if eDPTXAuxP1PNSWAP
msWriteByte( REG_112607+0x040,0xC8);//0x112607, 8wC8 PN Swap
#else
msWriteByte( REG_112607+0x040,0x48);//0x112607, 8wC8 PN no Swap
#endif
// ## debounce clock
msWriteByte( REG_11262F,0x0E);//0x11262E, 8w0E
msWriteByte( REG_11262F+0x040,0x0E);//0x11262E, 8w0E

// ## eDP TX Clk En
msWriteByteMask( REG_140802,0x0F,0x0F); // [3:0] eDP TX Clk En
msWriteByteMask( REG_140868,0x01,0x01); // [0]:eDP TX Fn En
msWriteByteMask( REG_140902,0x0F,0x0F); // [3:0] eDP TX Clk En
msWriteByteMask( REG_140968,0x01,0x01); // [0]:eDP TX Fn En

//eDPTX_printMsg("eDP TX Phy Init \r\n");
// ## eDP TX RBR Setting
// Clock Gen
#if 0
// Clock Gen
msWrite2Byte(REG_120F0C,0x0000);
msWrite2Byte(REG_120F0E,0x0800);
msWrite2Byte(REG_120F10,0x0000);
msWrite2Byte(REG_120F6A,0x0000);
msWrite2Byte(REG_120F46,0x0000);
#else
// Clock Gen
//msWrite2Byte(REG_120F0C,0x0000);
msWriteByte(REG_120F0F,0x08);
msWriteByte(REG_120F10,0x00);
//msWrite2Byte(REG_120F6A,0x0000);
//msWrite2Byte(REG_120F46,0x0000);
#endif

// MOD1
msWrite2Byte(REG_MOD1_20,0x0000);
msWrite2Byte(REG_MOD1_22,0x0001);
msWrite2Byte(REG_MOD1_24,0x5500); // P0 Channel output mode
msWrite2Byte(REG_MOD1_26,0x0000);
msWrite2Byte(REG_MOD1_4E,0x00ff);
// #############  O2 ############################################
//msWrite2Byte(REG_MOD1_50,0x0000);  // [0] MOD power down
msWriteByteMask(REG_MOD1_50,0x00,BIT0);  // [0] MOD power down
//##############################################################
msWrite2Byte(REG_MOD1_54,0x00F0);  //MOD1  Ch20-Ch23 Rterm
msWrite2Byte(REG_MOD2_B2,0x000F);  //MOD2  Ch24-Ch27 Rterm
msWrite2Byte(REG_MOD1_78,0x0000);
//msWrite2Byte(REG_MOD1_60,0xD080);  // [7] Set Sof Ov ModBuf enable

msWrite2Byte(REG_MOD2_14,0x0003);  //MOD2
msWrite2Byte(REG_MOD2_80,0x01ff); // MOD2
msWrite2Byte(REG_MOD2_82,0x0055); // MOD2  // P1 Channel output mode
msWrite2Byte(REG_MOD2_84,0x0000); // MOD2
msWrite2Byte(REG_MOD2_86,0x0000); //MOD2

msWrite2Byte(REG_MOD1_A0,0x0000);
msWrite2Byte(REG_MOD1_A2,0x0000);

DPTxSwingTune(0,eDP_SWING_DefaultValue);                        // swing = 0x2E
DPTxSwingTune(1,eDP_SWING_DefaultValue);                        // swing = 0x2E
DPTxPreEmphasisTune(0,eDP_PREMPHASIS_DefaultValue); // pre = 0x00
DPTxPreEmphasisTune(1,eDP_PREMPHASIS_DefaultValue); // pre = 0x00

// P0 PN swap
#if eDPTXP0PNSWAP
msWrite2Byte(REG_MOD1_46,0xFFFF);
msWrite2Byte(REG_MOD1_48,0x00FF);
#else
msWrite2Byte(REG_MOD1_46,0x0000);
msWrite2Byte(REG_MOD1_48,0x0000);
#endif

// P1 PN swap
#if eDPTXP1PNSWAP
msWrite2Byte(REG_MOD2_AE,0xFFFF);
msWrite2Byte(REG_MOD2_B0,0x00FF);
#else
msWrite2Byte(REG_MOD2_AE,0x0000);
msWrite2Byte(REG_MOD2_B0,0x0000);
#endif

#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
#if eDPTXPortSWAP
    msWriteByteMask(REG_MFT_02,0x01,BIT0);  // MFT
    msWriteByteMask(REG_MFT_21,0x80,BIT7); // L R SWAP
#endif
#endif

#if eDPTXMODPhyCalEn
DPTxSwingCal();
DPTxPreemphasisCal();
#endif

DPTxMLSignalEn(0,FALSE);
DPTxMLSignalEn(1,FALSE);
}


void DPTxSetMSA(BYTE PortNum)
{
   PortNum=(PortNum>0)?1:0; // limit PortNum==0 or 1
   eDPTX_printData("eDP TX Set P%x MSA",PortNum);
    // eDP TX
    msWrite2Byte(REG_140802+PortNum*0x100,0x000F);
    msWrite2Byte(REG_1408A4+PortNum*0x100,0x6030);
    //msWrite2Byte(REG_140868+PortNum*0x100,0x00f1);

if(PortNum==0)
{
#if eDPTXP0LaneSWAP
    msWriteByte(REG_14082E+PortNum*0x100,0x1b);       // Skew
    msWrite2Byte(REG_140820+PortNum*0x100,0x4002); // Lane oder
#else
    msWriteByte(REG_14082E+PortNum*0x100,0xe4);       // Skew
    msWrite2Byte(REG_140820+PortNum*0x100,0x0002); // Lane oder
#endif
}else
{
#if eDPTXP1LaneSWAP
    msWriteByte(REG_14082E+PortNum*0x100,0x1b);        // Skew
    msWrite2Byte(REG_140820+PortNum*0x100,0xC002);  // Lane oder
#else
    msWriteByte(REG_14082E+PortNum*0x100,0xe4);        // Skew
    msWrite2Byte(REG_140820+PortNum*0x100,0x8002);  // Lane oder
#endif
}
#if eDPTX10Bits
    msWriteByte(REG_14082F+PortNum*0x100,0x02); // 10bits
#else
    msWriteByte(REG_14082F+PortNum*0x100,0x03); // 8bits
#endif

    msWrite2Byte(REG_14082A+PortNum*0x100,0x00c0);
    //msWrite2Byte(REG_140868+PortNum*0x100,0x00f1);
    msWrite2Byte(REG_140866+PortNum*0x100,0x3c30);
    msWrite2Byte(REG_140830+PortNum*0x100,0x0213);// 1L: 0x0e17 , 2L:0x0e02 , 4L:0213


    eDPTX_printData("MSA Hwidth[%d]",eDPTXMSA_Hwidth);
    eDPTX_printData("MSA Vheight[%d]",eDPTXMSA_Vheight);
    eDPTX_printData("MSA Htotal[%d]",eDPTXMSA_Htotal);
    eDPTX_printData("MSA Vtotal[%d]",eDPTXMSA_Vtotal);
    // MSA 2160P    VB=40 HB=128
    msWrite2Byte(REG_140854+PortNum*0x100,eDPTXMSA_Htotal);  // Htotal 2048
    msWrite2Byte(REG_140856+PortNum*0x100,eDPTXMSA_HStart); // HStart 40
    msWrite2Byte(REG_140858+PortNum*0x100,eDPTXMSA_HSP*0x8000+eDPTXMSA_HSW);    // Hsp|Hsw 20
    msWrite2Byte(REG_14085A+PortNum*0x100,eDPTXMSA_Vtotal); // Vtotal  2200
    msWrite2Byte(REG_14085C+PortNum*0x100,eDPTXMSA_VStart); // Vstart  4
    msWrite2Byte(REG_14085E+PortNum*0x100,eDPTXMSA_VSP*0x8000+eDPTXMSA_VSW); // Vsp|Vsw  5
    msWrite2Byte(REG_140860+PortNum*0x100,eDPTXMSA_Hwidth); //Hactive 1920
    msWrite2Byte(REG_140862+PortNum*0x100,eDPTXMSA_Vheight);    // Vactive 2160
    msWriteByteMask(REG_140864+PortNum*0x100,eDPTXMSAColorMISC,BIT7|BIT6|BIT5);  // MISC 10bits
//##################  Video PG ############################################
    msWrite2Byte(REG_140836+PortNum*0x100,eDPTXMSA_VB-eDPTXMSA_VStart); // V front porch
    msWrite2Byte(REG_140838+PortNum*0x100,eDPTXMSA_VSW); // Vsp
    msWrite2Byte(REG_14083A+PortNum*0x100,eDPTXMSA_VStart-eDPTXMSA_VSW); // Vback porch 30
    msWrite2Byte(REG_14083C+PortNum*0x100,eDPTXMSA_Vheight+1); //V active
    msWrite2Byte(REG_140844+PortNum*0x100,eDPTXMSA_HB-eDPTXMSA_HStart); // H front porch 28
    msWrite2Byte(REG_140846+PortNum*0x100,eDPTXMSA_HSW); // Hsp 40
    msWrite2Byte(REG_140848+PortNum*0x100,eDPTXMSA_HStart-eDPTXMSA_HSW); //  H back proch 60
    msWrite2Byte(REG_14084A+PortNum*0x100,eDPTXMSA_Hwidth); // H active
    msWrite2Byte(REG_14083E+PortNum*0x100,0x0000);  // [11:0]V pix step [12]pix repeat
    msWrite2Byte(REG_14084C+PortNum*0x100,0x1002); // [11:0]H pix step [12]pix repeat
    msWrite2Byte(REG_14084E+PortNum*0x100,0x0030); // H pix inc
    msWrite2ByteMask(REG_140834+PortNum*0x100,eDPTXEnPG,BIT0); // PG EN
}

void DPTxSetSCFastLPLL(BOOL IsVGA, WORD VGAHtt, WORD VGAVtt, WORD InVFreq)
{
DWORD SCinHotoal;
DWORD SCinVotoal;
DWORD SCOutHotoal;
DWORD SCOutVotoal;
DWORD SCLPLL_InDiv;
DWORD SCLPLL_OutDiv;

if(IsVGA)
{
SCinVotoal=VGAVtt; // Vtotal
SCinHotoal=VGAHtt; // Htotal
}
else
{
SCinVotoal=msRead2Byte(SCL0_01_3E); // Vtotal
SCinHotoal=msRead2Byte(SCL0_01_50); // Htotal
}

SCOutHotoal=msRead2Byte(SCL0_10_18); // Htotal
SCOutVotoal=msRead2Byte(SCL0_10_1A); // Vtotal

eDPTX_printData("[eDP] In Ht %d",SCinHotoal);
eDPTX_printData("[eDP] In Vt %d",SCinVotoal);
eDPTX_printData("[eDP] In VFeq %d",InVFreq);
eDPTX_printData("[eDP] Ot Ht %d",SCOutHotoal);
eDPTX_printData("[eDP] Ot Vt %d",SCOutVotoal);
SCLPLL_InDiv=SCinHotoal*(SCinVotoal+1);
if(IsVGA)
{SCLPLL_InDiv=SCLPLL_InDiv+0x10000;}
SCLPLL_OutDiv=(SCOutHotoal+1)*(SCOutVotoal+1)*(600/InVFreq);
eDPTX_printData("[eDP] In DivH %x",SCLPLL_InDiv>>16);
eDPTX_printData("[eDP] In DivL %x",SCLPLL_InDiv);
eDPTX_printData("[eDP] Out DivH %x",SCLPLL_OutDiv>>16);
eDPTX_printData("[eDP] Out DivL %x",SCLPLL_OutDiv);
#if CHIP_ID==MST9U4
msWrite2Byte(REG_LPLL_34,(SCLPLL_InDiv>>16)&0x1FF);
#else
msWrite2Byte(REG_LPLL_34,(SCLPLL_InDiv>>16)&0xFF);
#endif
msWrite2Byte(REG_LPLL_32,SCLPLL_InDiv);

msWrite2Byte(REG_LPLL_38,SCLPLL_OutDiv>>16);
msWrite2Byte(REG_LPLL_36,SCLPLL_OutDiv);

msWriteByteMask(REG_120F2F,BIT2,BIT2|BIT3); // LPLL Lock Main Window
//msWriteByteMask(REG_LPLL_E1,BIT7,BIT7); // enable fastlock
//msWriteByteMask(REG_LPLL_18,BIT3,BIT3); // enable FrameLock
}

Bool DPTxAuxDPCDReadByte( BYTE PortNum, DWORD  DPCDADDR , BYTE *pRdData)
{
BYTE RetryCnt;
Bool bVaildCmd=FALSE;
BYTE ReplyCmd=0xFF;
BYTE AuxPortNum=0;
#if eDPTXDETECTHPD
BYTE HPDPortNum=0;
#endif

#if eDPTXDETECTHPD
switch (PortNum)
{
  case 0: HPDPortNum=eDPTXP0UseHPDPortNum; break;
  case 1: HPDPortNum=eDPTXP1UseHPDPortNum; break;
  default: break;
}
if(DPTxCheckHPD(HPDPortNum)){}
else {eDPTX_printMsg("[AUX] HPD low"); return FALSE;}
#endif

switch (PortNum)
{
  case 0: AuxPortNum=eDPTXP0UseAUXPortNum; break;
  case 1: AuxPortNum=eDPTXP1UseAUXPortNum; break;
  default: break;
}

for(RetryCnt=0;RetryCnt<5;RetryCnt++)
{
WaitReplyCount=AuxWaitReplyLpCntNum;
msWriteByte( REG_112620+AuxPortNum*0x40,0x7F); // [6:0] Clear irq
//## AUXTX CMD
msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear Buf
msWriteByte( REG_112622+AuxPortNum*0x40,0x09); // [3:0] CMD
msWriteByte( REG_112624+AuxPortNum*0x40,DPCDADDR&0x000000FF); // [7:0] ADR[7:0]
msWriteByte( REG_112625+AuxPortNum*0x40,(DPCDADDR>>8)&0x000000FF); // [7:0] ADR[15:8]
msWriteByte( REG_112626+AuxPortNum*0x40,(DPCDADDR>>16)&0x0000000F); // [3:0] ADR[19:16]
msWriteByte( REG_112629+AuxPortNum*0x40,0x00); // [7:4] Req Len[3:0]
msWriteByte( REG_112616+AuxPortNum*0x40,0x00); // [0] No Length Command
msWriteByte( REG_112618+AuxPortNum*0x40,0x08); // [3] AUX TX Fire CMD

// ## AUXTX RXBUF
while(WaitReplyCount--)
{
  _nop_();
  if( (msReadByte( REG_11260C+AuxPortNum*0x40)&0x0F)==1) // [3:0]: Write Point (RX LENG) [7:4] Read Point
  {bVaildCmd=TRUE;break;}
}

ReplyCmd=msReadByte( REG_112612+AuxPortNum*0x40)&0x0F;
if( (ReplyCmd==0x00)&&(bVaildCmd==TRUE)){break;}
else{ForceDelay1ms(1);msWriteByte( REG_112629+AuxPortNum*0x40,0x01);} // [0] Complete CMD ->Clear RX Buf

if(bVaildCmd==FALSE){eDPTX_printData("SINK ACK TimeOut => R DPCD[0x%x] retry",DPCDADDR);}
else{eDPTX_printData("SINK DEF/NACK => R DPCD[0x%x] retry",DPCDADDR);}

}

if(ReplyCmd)
{
  //msWriteByte( REG_112629+PortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
  return FALSE;
}

if(bVaildCmd)
{
  msWriteByte( REG_112611+AuxPortNum*0x40,0x00); // [1]: set 0
  msWriteByte( REG_112611+AuxPortNum*0x40,0x01); // [0]: triger read pulse
 WaitRdPluseDelayCount=AuxWaitRdPluseLpCntNum;
  while(WaitRdPluseDelayCount--){_nop_();}
  *pRdData=msReadByte( REG_112610+AuxPortNum*0x40);          // [7:0]: Read DATA
  //eDPTX_printData("WaitReplyCount:%d",WaitReplyCount);
  //eDPTX_printData("R DPCD ADR:%x",DPCDADDR);
  //printData("REPLY CMD:%x",ReplyCmd);// [3:0]: Rx CMD
  //eDPTX_printData("D:%x",*pRdData);
}else
{
  eDPTX_printMsg("R TimeOut");
}
  msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
return bVaildCmd;
}


Bool DPTxAuxDPCDReadBytes( BYTE PortNum, DWORD  DPCDADDR , BYTE Length , BYTE *pRxBuf)
{
BYTE RetryCnt;
Bool bVaildCmd=FALSE;
BYTE ReplyCmd=0xFF;
BYTE RdCount;
BYTE AuxPortNum=0;
#if eDPTXDETECTHPD
BYTE HPDPortNum=0;
#endif

#if eDPTXDETECTHPD
switch (PortNum)
{
  case 0: HPDPortNum=eDPTXP0UseHPDPortNum; break;
  case 1: HPDPortNum=eDPTXP1UseHPDPortNum; break;
  default: break;
}
if(DPTxCheckHPD(HPDPortNum)){}
else {eDPTX_printMsg("[AUX] HPD low"); return FALSE;}
#endif

switch (PortNum)
{
  case 0: AuxPortNum=eDPTXP0UseAUXPortNum; break;
  case 1: AuxPortNum=eDPTXP1UseAUXPortNum; break;
  default: break;
}

for(RetryCnt=0;RetryCnt<5;RetryCnt++)
{
WaitReplyCount=AuxWaitReplyLpCntNum;
msWriteByte( REG_112620+AuxPortNum*0x40,0x7F); // [6:0] Clear irq
if((Length==0)||((Length>16))) {return bVaildCmd;}
//## AUXTX CMD
msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear Buf
msWriteByte( REG_112622+AuxPortNum*0x40,0x09); // [3:0] CMD
msWriteByte( REG_112624+AuxPortNum*0x40,DPCDADDR&0x000000FF); // [7:0] ADR[7:0]
msWriteByte( REG_112625+AuxPortNum*0x40,(DPCDADDR>>8)&0x000000FF); // [7:0] ADR[15:8]
msWriteByte( REG_112626+AuxPortNum*0x40,(DPCDADDR>>16)&0x0000000F); // [3:0] ADR[19:16]
msWriteByte( REG_112629+AuxPortNum*0x40,(Length-1)<<4); // [7:4] Req Len[3:0]
msWriteByte( REG_112616+AuxPortNum*0x40,0x00); // [0] No Length Command
msWriteByte( REG_112618+AuxPortNum*0x40,0x08); // [3] AUX TX Fire CMD
//eDPTX_printData("DPCD ADR:%x",DPCDADDR);

// ## AUXTX RXBUF
while(WaitReplyCount--)
{
  _nop_();
  if( (msReadByte( REG_11260C+AuxPortNum*0x40)&0x0F) ) // [3:0]: Write Point (RX LENG) [7:4] Read Point
  {bVaildCmd=TRUE;break;}
  if( (msReadByte( REG_11260D+AuxPortNum*0x40)&0x02) ) // [1]: Write Point  Buf Full
  {bVaildCmd=TRUE;break;}
}

ReplyCmd=msReadByte( REG_112612+AuxPortNum*0x40)&0x0F;
if( (ReplyCmd==0x00)&&(bVaildCmd==TRUE)){break;}
else{ForceDelay1ms(1);msWriteByte( REG_112629+AuxPortNum*0x40,0x01);} // [0] Complete CMD ->Clear RX Buf

if(bVaildCmd==FALSE){eDPTX_printData("SINK ACK TimeOut => R DPCD[0x%x] retry",DPCDADDR);}
else{eDPTX_printData("SINK DEF/NACK => R DPCD[0x%x] retry",DPCDADDR);}

}

if(ReplyCmd)
{
  //msWriteByte( REG_112629+PortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
  return FALSE;
}

if(bVaildCmd)
{
  //eDPTX_printData("R DPCD ADR:%x",DPCDADDR);
  //eDPTX_printData("REPLY CMD:%x",ReplyCmd);// [3:0]: Rx CMD
  msWriteByte( REG_112611+AuxPortNum*0x40,0x00); // [1]: set 0
  for(RdCount=0;RdCount<Length;RdCount++)
  {
    msWriteByte( REG_112611+AuxPortNum*0x40,0x01); // [0]: triger read pulse
    WaitRdPluseDelayCount=AuxWaitRdPluseLpCntNum;
    while(WaitRdPluseDelayCount--){_nop_();}
    *(pRxBuf+RdCount)=msReadByte( REG_112610+AuxPortNum*0x40);          // [7:0]: Read DATA
     //eDPTX_printData("D:%x",*(pRxBuf+RdCount));
  }
}else
{
  eDPTX_printMsg("R TimeOut");
}
  msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf

return bVaildCmd;

}

Bool DPTxAuxDPCDWriteByte( BYTE PortNum, DWORD  DPCDADDR , BYTE wData)
{
BYTE RetryCnt;
Bool bVaildCmd=FALSE;
BYTE ReplyCmd=0xFF;
BYTE AuxPortNum=0;
#if eDPTXDETECTHPD
BYTE HPDPortNum=0;
#endif

#if eDPTXDETECTHPD
switch (PortNum)
{
  case 0: HPDPortNum=eDPTXP0UseHPDPortNum; break;
  case 1: HPDPortNum=eDPTXP1UseHPDPortNum; break;
  default: break;
}
if(DPTxCheckHPD(HPDPortNum)){}
else {eDPTX_printMsg("[AUX] HPD low"); return FALSE;}
#endif

switch (PortNum)
{
  case 0: AuxPortNum=eDPTXP0UseAUXPortNum; break;
  case 1: AuxPortNum=eDPTXP1UseAUXPortNum; break;
  default: break;
}

for(RetryCnt=0;RetryCnt<5;RetryCnt++)
{
WaitReplyCount=AuxWaitReplyLpCntNum;
msWriteByte( REG_112620+AuxPortNum*0x40,0x7F); // [6:0] Clear irq
//## AUXTX CMD
msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear Buf
msWriteByte( REG_112622+AuxPortNum*0x40,0x08); // [3:0] CMD
msWriteByte( REG_112624+AuxPortNum*0x40,DPCDADDR&0x000000FF); // [7:0] ADR[7:0]
msWriteByte( REG_112625+AuxPortNum*0x40,(DPCDADDR>>8)&0x000000FF); // [7:0] ADR[15:8]
msWriteByte( REG_112626+AuxPortNum*0x40,(DPCDADDR>>16)&0x0000000F); // [3:0] ADR[19:16]
msWriteByte( REG_11261A+AuxPortNum*0x40,wData); // [7:0] AUX TX Write DATA BUF
msWriteByte( REG_112629+AuxPortNum*0x40,0x00); // [7:4] Req Len[3:0]
msWriteByte( REG_112616+AuxPortNum*0x40,0x00); // [0] No Length Command
msWriteByte( REG_112618+AuxPortNum*0x40,0x08); // [3] AUX TX Fire CMD
//eDPTX_printData("DPCD ADR:%x",DPCDADDR);

// ## AUXTX RXBUF
while(WaitReplyCount--)
{
  _nop_();
  //if( (msReadByte( REG_112620+AuxPortNum*0x40)&BIT3) ) // [3] Rx Cmd irq
  if( (msReadByte( REG_112620+AuxPortNum*0x40)&BIT6) ) // [6] Rx Transaction Complete
  {bVaildCmd=TRUE;break;}
}

ReplyCmd=msReadByte( REG_112612+AuxPortNum*0x40)&0x0F;
if( (ReplyCmd==0x00)&&(bVaildCmd==TRUE)){break;}
else{ForceDelay1ms(1);msWriteByte( REG_112629+AuxPortNum*0x40,0x01);} // [0] Complete CMD ->Clear RX Buf

if(bVaildCmd==FALSE){eDPTX_printData("SINK ACK TimeOut => W DPCD[0x%x] retry",DPCDADDR);}
else{eDPTX_printData("SINK DEF/NACK => W DPCD[0x%x] retry",DPCDADDR);}

}

if(ReplyCmd)
{
  //msWriteByte( REG_112629+PortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
  return FALSE;
}

if(bVaildCmd)
{
  //eDPTX_printData("W DPCD ADR:%x",DPCDADDR);
  //eDPTX_printData("REPLY CMD:%x",ReplyCmd);// [3:0]: Rx CMD
}else
{
  eDPTX_printMsg("W TimeOut");
}

  msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
return bVaildCmd;
}

Bool DPTxAuxDPCDWriteBytes(BYTE PortNum, DWORD  DPCDADDR, BYTE Length ,BYTE *pTxBuf)
{
BYTE RetryCnt;
Bool bVaildCmd=FALSE;
BYTE ReplyCmd=0xFF;
BYTE LengthCnt=0;
BYTE AuxPortNum=0;
#if eDPTXDETECTHPD
BYTE HPDPortNum=0;
#endif

#if eDPTXDETECTHPD
switch (PortNum)
{
  case 0: HPDPortNum=eDPTXP0UseHPDPortNum; break;
  case 1: HPDPortNum=eDPTXP1UseHPDPortNum; break;
  default: break;
}
if(DPTxCheckHPD(HPDPortNum)){}
else {eDPTX_printMsg("[AUX] HPD low"); return FALSE;}
#endif

switch (PortNum)
{
  case 0: AuxPortNum=eDPTXP0UseAUXPortNum; break;
  case 1: AuxPortNum=eDPTXP1UseAUXPortNum; break;
  default: break;
}

for(RetryCnt=0;RetryCnt<5;RetryCnt++)
{
WaitReplyCount=AuxWaitReplyLpCntNum;
msWriteByte( REG_112620+AuxPortNum*0x40,0x7F); // [6:0] Clear irq
if((Length==0)||((Length>16))) {return bVaildCmd;}
//## AUXTX CMD
msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear Buf
msWriteByte( REG_112622+AuxPortNum*0x40,0x08); // [3:0] CMD
msWriteByte( REG_112624+AuxPortNum*0x40,DPCDADDR&0x000000FF); // [7:0] ADR[7:0]
msWriteByte( REG_112625+AuxPortNum*0x40,(DPCDADDR>>8)&0x000000FF); // [7:0] ADR[15:8]
msWriteByte( REG_112626+AuxPortNum*0x40,(DPCDADDR>>16)&0x0000000F); // [3:0] ADR[19:16]
for(LengthCnt=0;LengthCnt<Length;LengthCnt++)
{
    msWriteByte( REG_11261A+AuxPortNum*0x40,*(pTxBuf+LengthCnt)); // [7:0] AUX TX Write DATA BUF
   WaitRdPluseDelayCount=AuxWaitWtPluseLpCntNum;
    while(WaitRdPluseDelayCount--){_nop_();}
}
msWriteByte( REG_112629+AuxPortNum*0x40,(Length-1)<<4); // [7:4] Req Len[3:0]
msWriteByte( REG_112616+AuxPortNum*0x40,0x00); // [0] No Length Command
msWriteByte( REG_112618+AuxPortNum*0x40,0x08); // [3] AUX TX Fire CMD
//eDPTX_printData("DPCD ADR:%x",DPCDADDR);

// ## AUXTX RXBUF
while(WaitReplyCount--)
{
  _nop_();
  //if( (msReadByte( REG_112620+AuxPortNum*0x40)&BIT3) ) // [3] Rx Cmd irq
  if( (msReadByte( REG_112620+AuxPortNum*0x40)&BIT6) ) //  [6] Rx Transaction Complete
  {bVaildCmd=TRUE;break;}
}

ReplyCmd=msReadByte( REG_112612+AuxPortNum*0x40)&0x0F;

if( (ReplyCmd==0x00)&&(bVaildCmd==TRUE)){break;}
else{ForceDelay1ms(1);msWriteByte( REG_112629+AuxPortNum*0x40,0x01);} // [0] Complete CMD ->Clear RX Buf

if(bVaildCmd==FALSE){eDPTX_printData("SINK ACK TimeOut => W DPCD[0x%x] retry",DPCDADDR);}
else{eDPTX_printData("SINK DEF/NACK => W DPCD[0x%x] retry",DPCDADDR);}
}

if(ReplyCmd)
{
  //msWriteByte( REG_112629+PortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
  return FALSE;
}

if(bVaildCmd)
{
  //eDPTX_printData("W DPCD ADR:%x",DPCDADDR);
  //eDPTX_printData("REPLY CMD:%x",ReplyCmd);// [3:0]: Rx CMD
}else
{
  eDPTX_printMsg("W TimeOut");
}
  msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
return bVaildCmd;
}

Bool DPTxAuxEDIDReadNLCMD( BYTE PortNum, DWORD  DPCDADDR )
{
BYTE RetryCnt;
Bool bVaildCmd=FALSE;
BYTE ReplyCmd=0xFF;
BYTE AuxPortNum=0;
#if eDPTXDETECTHPD
BYTE HPDPortNum=0;
#endif

#if eDPTXDETECTHPD
#if eDPTXHBR2PortEn
HPDPortNum=(PortNum)?eDPTXHBR2PUseHPDPortNum : eDPTXHBR2PUseHPDPortNum;
#else
switch (PortNum)
{
  case 0: HPDPortNum=eDPTXP0UseHPDPortNum; break;
  case 1: HPDPortNum=eDPTXP1UseHPDPortNum; break;
  default: break;
}
#endif
if(DPTxCheckHPD(HPDPortNum)){}
else {eDPTX_printMsg("[AUX] HPD low"); return FALSE;}
#endif

#if eDPTXHBR2PortEn
AuxPortNum=(PortNum)?eDPTXHBR2PUseAUXPortNum : eDPTXHBR2PUseAUXPortNum;
#else
switch (PortNum)
{
  case 0: AuxPortNum=eDPTXP0UseAUXPortNum; break;
  case 1: AuxPortNum=eDPTXP1UseAUXPortNum; break;
  default: break;
}
#endif

for(RetryCnt=0;RetryCnt<5;RetryCnt++)
{
WaitReplyCount=AuxWaitReplyLpCntNum;
msWriteByte( REG_112620+AuxPortNum*0x40,0x7F); // [6:0] Clear irq
//## AUXTX CMD
msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear Buf
msWriteByte( REG_112622+AuxPortNum*0x40,0x01); // [3:0] CMD , MOT=0
msWriteByte( REG_112624+AuxPortNum*0x40,DPCDADDR&0x000000FF); // [7:0] ADR[7:0]
msWriteByte( REG_112625+AuxPortNum*0x40,(DPCDADDR>>8)&0x000000FF); // [7:0] ADR[15:8]
msWriteByte( REG_112626+AuxPortNum*0x40,(DPCDADDR>>16)&0x0000000F); // [3:0] ADR[19:16]
msWriteByte( REG_112629+AuxPortNum*0x40,0x00); // [7:4] Req Len[3:0]
msWriteByte( REG_112616+AuxPortNum*0x40,0x01); // [0] No Length Command
msWriteByte( REG_112618+AuxPortNum*0x40,0x08); // [3] AUX TX Fire CMD

// ## AUXTX RXBUF
while(WaitReplyCount--)
{
  _nop_();
  //if( (msReadByte( REG_11260C+AuxPortNum*0x40)&0x0F)==1) // [3:0]: Write Point (RX LENG) [7:4] Read Point
  //{bVaildCmd=TRUE;break;}
  // check reply cmd IRQ
  if(WaitReplyCount==1)
  {bVaildCmd=TRUE;break;}
}

ReplyCmd=msReadByte( REG_112612+AuxPortNum*0x40)&0x0F;
if( (ReplyCmd==0x00)&&(bVaildCmd==TRUE)){break;}
else{ForceDelay1ms(1);msWriteByte( REG_112629+AuxPortNum*0x40,0x01);} // [0] Complete CMD ->Clear RX Buf

if(bVaildCmd==FALSE){eDPTX_printData("SINK ACK TimeOut => R NL EDID[0x%x] retry",DPCDADDR);}
else{eDPTX_printData("SINK DEF/NACK => R NL EDID[0x%x] retry",DPCDADDR);}

}

if(ReplyCmd)
{
  //msWriteByte( REG_112629+PortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
  return FALSE;
}

if(bVaildCmd)
{
  //eDPTX_printData("WaitReplyCount:%d",WaitReplyCount);
  //eDPTX_printData("R DPCD ADR:%x",DPCDADDR);
  printData("REPLY CMD:%x",ReplyCmd);// [3:0]: Rx CMD
}else
{
  eDPTX_printMsg("R TimeOut");
}
msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
return bVaildCmd;
}


Bool DPTxAuxEDIDWriteByte( BYTE PortNum, DWORD  DPCDADDR , BYTE wData)
{
BYTE RetryCnt;
Bool bVaildCmd=FALSE;
BYTE ReplyCmd=0xFF;
BYTE AuxPortNum=0;
#if eDPTXDETECTHPD
BYTE HPDPortNum=0;
#endif

#if eDPTXDETECTHPD
#if eDPTXHBR2PortEn
HPDPortNum=(PortNum)?eDPTXHBR2PUseHPDPortNum : eDPTXHBR2PUseHPDPortNum;
#else
switch (PortNum)
{
  case 0: HPDPortNum=eDPTXP0UseHPDPortNum; break;
  case 1: HPDPortNum=eDPTXP1UseHPDPortNum; break;
  default: break;
}
#endif
if(DPTxCheckHPD(HPDPortNum)){}
else {eDPTX_printMsg("[AUX] HPD low"); return FALSE;}
#endif

#if eDPTXHBR2PortEn
AuxPortNum=(PortNum)?eDPTXHBR2PUseAUXPortNum : eDPTXHBR2PUseAUXPortNum;
#else
switch (PortNum)
{
  case 0: AuxPortNum=eDPTXP0UseAUXPortNum; break;
  case 1: AuxPortNum=eDPTXP1UseAUXPortNum; break;
  default: break;
}
#endif


for(RetryCnt=0;RetryCnt<5;RetryCnt++)
{
WaitReplyCount=AuxWaitReplyLpCntNum;
msWriteByte( REG_112620+AuxPortNum*0x40,0x7F); // [6:0] Clear irq
//## AUXTX CMD
msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear Buf
msWriteByte( REG_112622+AuxPortNum*0x40,0x04); // [3:0] CMD ,MOT=1
msWriteByte( REG_112624+AuxPortNum*0x40,DPCDADDR&0x000000FF); // [7:0] ADR[7:0]
msWriteByte( REG_112625+AuxPortNum*0x40,(DPCDADDR>>8)&0x000000FF); // [7:0] ADR[15:8]
msWriteByte( REG_112626+AuxPortNum*0x40,(DPCDADDR>>16)&0x0000000F); // [3:0] ADR[19:16]
msWriteByte( REG_11261A+AuxPortNum*0x40,wData); // [7:0] AUX TX Write DATA BUF
msWriteByte( REG_112629+AuxPortNum*0x40,0x00); // [7:4] Req Len[3:0]
msWriteByte( REG_112616+AuxPortNum*0x40,0x00); // [0] No Length Command
msWriteByte( REG_112618+AuxPortNum*0x40,0x08); // [3] AUX TX Fire CMD
//eDPTX_printData("DPCD ADR:%x",DPCDADDR);

// ## AUXTX RXBUF
while(WaitReplyCount--)
{
  _nop_();
  //if( (msReadByte( REG_112620+AuxPortNum*0x40)&BIT3) ) // [3] Rx Cmd irq
  if( (msReadByte( REG_112620+AuxPortNum*0x40)&BIT6) ) //  [6] Rx Transaction Complete
  {bVaildCmd=TRUE;break;}
}

ReplyCmd=msReadByte( REG_112612+AuxPortNum*0x40)&0x0F;
if( (ReplyCmd==0x00)&&(bVaildCmd==TRUE)){break;}
else{ForceDelay1ms(1);msWriteByte( REG_112629+AuxPortNum*0x40,0x01);} // [0] Complete CMD ->Clear RX Buf

if(bVaildCmd==FALSE){eDPTX_printData("SINK ACK TimeOut => W EDID[0x%x] retry",DPCDADDR);}
else{eDPTX_printData("SINK DEF/NACK => W EDID[0x%x] retry",DPCDADDR);}

}

if(ReplyCmd)
{
  //msWriteByte( REG_112629+PortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
  return FALSE;
}

if(bVaildCmd)
{
  //eDPTX_printData("W DPCD ADR:%x",DPCDADDR);
  //eDPTX_printData("REPLY CMD:%x",ReplyCmd);// [3:0]: Rx CMD
}else
{
  eDPTX_printMsg("W TimeOut");
}

  msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
return bVaildCmd;
}

Bool DPTxAuxEDIDReadBytes( BYTE PortNum, DWORD  DPCDADDR , BYTE Length , BYTE *pRxBuf)
{
BYTE RetryCnt;
Bool bVaildCmd=FALSE;
BYTE ReplyCmd=0xFF;
BYTE RdCount;
BYTE AuxPortNum=0;
#if eDPTXDETECTHPD
BYTE HPDPortNum=0;
#endif

#if eDPTXDETECTHPD
#if eDPTXHBR2PortEn
HPDPortNum=(PortNum)?eDPTXHBR2PUseHPDPortNum : eDPTXHBR2PUseHPDPortNum;
#else
switch (PortNum)
{
  case 0: HPDPortNum=eDPTXP0UseHPDPortNum; break;
  case 1: HPDPortNum=eDPTXP1UseHPDPortNum; break;
  default: break;
}
#endif
if(DPTxCheckHPD(HPDPortNum)){}
else {eDPTX_printMsg("[AUX] HPD low"); return FALSE;}
#endif

#if eDPTXHBR2PortEn
AuxPortNum=(PortNum)?eDPTXHBR2PUseAUXPortNum : eDPTXHBR2PUseAUXPortNum;
#else
switch (PortNum)
{
  case 0: AuxPortNum=eDPTXP0UseAUXPortNum; break;
  case 1: AuxPortNum=eDPTXP1UseAUXPortNum; break;
  default: break;
}
#endif

for(RetryCnt=0;RetryCnt<5;RetryCnt++)
{
WaitReplyCount=AuxWaitReplyLpCntNum+1000;
msWriteByte( REG_112620+AuxPortNum*0x40,0x7F); // [6:0] Clear irq
if((Length==0)||((Length>16))) {return bVaildCmd;}
//## AUXTX CMD
msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear Buf
msWriteByte( REG_112622+AuxPortNum*0x40,0x05); // [3:0] CMD , MOT=1
msWriteByte( REG_112624+AuxPortNum*0x40,DPCDADDR&0x000000FF); // [7:0] ADR[7:0]
msWriteByte( REG_112625+AuxPortNum*0x40,(DPCDADDR>>8)&0x000000FF); // [7:0] ADR[15:8]
msWriteByte( REG_112626+AuxPortNum*0x40,(DPCDADDR>>16)&0x0000000F); // [3:0] ADR[19:16]
msWriteByte( REG_112629+AuxPortNum*0x40,(Length-1)<<4); // [7:4] Req Len[3:0]
msWriteByte( REG_112616+AuxPortNum*0x40,0x00); // [0] No Length Command
msWriteByte( REG_112618+AuxPortNum*0x40,0x08); // [3] AUX TX Fire CMD
//eDPTX_printData("DPCD ADR:%x",DPCDADDR);

// ## AUXTX RXBUF
while(WaitReplyCount--)
{
  _nop_();
  if( (msReadByte( REG_11260C+AuxPortNum*0x40)&0x0F) ) // [3:0]: Write Point (RX LENG) [7:4] Read Point
  {bVaildCmd=TRUE;break;}
  if( (msReadByte( REG_11260D+AuxPortNum*0x40)&0x02) ) // [1]: Write Point  Buf Full
  {bVaildCmd=TRUE;break;}
}

ReplyCmd=msReadByte( REG_112612+AuxPortNum*0x40)&0x0F;
if( (ReplyCmd==0x00)&&(bVaildCmd==TRUE)){break;}
else{ForceDelay1ms(1);msWriteByte( REG_112629+AuxPortNum*0x40,0x01);} // [0] Complete CMD ->Clear RX Buf

if(bVaildCmd==FALSE){eDPTX_printData("SINK ACK TimeOut => R EDID[0x%x] retry",DPCDADDR);}
else{eDPTX_printData("SINK DEF/NACK => R EDID[0x%x] retry",DPCDADDR);}

}

if(ReplyCmd)
{
  //msWriteByte( REG_112629+PortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf
  return FALSE;
}

if(bVaildCmd)
{
  //eDPTX_printData("R DPCD ADR:%x",DPCDADDR);
  //eDPTX_printData("REPLY CMD:%x",ReplyCmd);// [3:0]: Rx CMD
  msWriteByte( REG_112611+AuxPortNum*0x40,0x00); // [1]: set 0
  for(RdCount=0;RdCount<Length;RdCount++)
  {
    msWriteByte( REG_112611+AuxPortNum*0x40,0x01); // [0]: triger read pulse
    WaitRdPluseDelayCount=AuxWaitRdPluseLpCntNum;
    while(WaitRdPluseDelayCount--){_nop_();}
    *(pRxBuf+RdCount)=msReadByte( REG_112610+AuxPortNum*0x40);          // [7:0]: Read DATA
     //eDPTX_printData("D:%x",*(pRxBuf+RdCount));
  }
}else
{
  eDPTX_printMsg("R TimeOut");
}
  msWriteByte( REG_112629+AuxPortNum*0x40,0x01); // [0] Complete CMD ->Clear RX Buf

return bVaildCmd;

}

Bool DPTxReadEDID(BYTE PortNum)
{
   BYTE AuxRxBuf[16];
   BYTE EDIDOffset;
   BYTE PrintCnt;
   EDIDOffset=0x00;
   DPTxAuxEDIDWriteByte(PortNum,0x00050,EDIDOffset); // write EDIDOffset
   eDPTX_printData("EDID Offset %d", EDIDOffset);
   DPTxAuxEDIDReadBytes(PortNum,0x00050,16,AuxRxBuf); // Read 16Bytes
   for(PrintCnt=0;PrintCnt<16;PrintCnt++)
   {eDPTX_printData("%x", AuxRxBuf[PrintCnt]);}
   eDPTX_printData("EDID Offset %d", EDIDOffset+0x10);
   DPTxAuxEDIDReadBytes(PortNum,0x00050,16,AuxRxBuf); // Read 16Bytes
   for(PrintCnt=0;PrintCnt<16;PrintCnt++)
   {eDPTX_printData("%x", AuxRxBuf[PrintCnt]);}
   DPTxAuxEDIDReadNLCMD(PortNum,0x00050); // read EOF
   return TRUE;
}

Bool DPTxReadDPCDLinkStatus(BYTE PortNum)
{
   BYTE AuxRxBuf[3];
   if(DPTxAuxDPCDReadBytes(PortNum,0x000202,3,AuxRxBuf))
   {
      eDPTX_printData("P%x Link Status",PortNum);
      eDPTX_printData("[%x]DPCD202",AuxRxBuf[0]); // 202
      eDPTX_printData("[%x]DPCD203",AuxRxBuf[1]); // 203
      eDPTX_printData("[%x]DPCD204",AuxRxBuf[2]); // 204
      if((AuxRxBuf[0]==0x77)&&(AuxRxBuf[1]==0x77)&&((AuxRxBuf[2]&0x01)==0x01))
      {return TRUE;}
      else
      {return FALSE;}
   }else
   {return FALSE;}
}

void DPTxConfigLinkRate(BYTE LinkRate)
{
	if(LinkRate==eDPTXRBR) // RBR
{
msWrite2Byte(REG_LPLL_04,0x1700); // for Geogre test
msWrite2Byte(REG_LPLL_90,0x2222);
msWrite2Byte(REG_LPLL_92,0x0022);
msWrite2Byte(REG_LPLL_80,0x0024);
msWrite2Byte(REG_LPLL_82,0x0203);
msWrite2Byte(REG_LPLL_84,0x0040);
msWrite2Byte(REG_LPLL_86,0x000f);
msWrite2Byte(REG_LPLL_88,0x0000);
msWrite2Byte(REG_LPLL_8A,0x0a00);
msWrite2Byte(REG_LPLL_8C,0x0400);
msWrite2Byte(REG_LPLL_94,0x0000);
msWrite2Byte(REG_LPLL_96,0x0004);
}
else // HBR
{
msWrite2Byte(REG_LPLL_90,0x9999);
msWrite2Byte(REG_LPLL_92,0x0019);
msWrite2Byte(REG_LPLL_80,0x0024);
msWrite2Byte(REG_LPLL_82,0x0502);
msWrite2Byte(REG_LPLL_84,0x0040);
msWrite2Byte(REG_LPLL_86,0x0013);
msWrite2Byte(REG_LPLL_88,0x0000);
msWrite2Byte(REG_LPLL_8A,0x0a00);
msWrite2Byte(REG_LPLL_8C,0x0400);
msWrite2Byte(REG_LPLL_94,0x0000);
msWrite2Byte(REG_LPLL_96,0x0004);
}
#if eDPTXEnSSC
DPTxSSCEnable(TRUE,LinkRate); // 0.5% , 33K
#endif
}


// ###############################################################
// function Name: DPTxSSCEnable(BYTE bEnable, BYTE LinkRate)
// Amp < 0.5% {RBR,HBR}
// 30Khz< MFrq(ModulationFrq) < 33Khz
// ###############################################################
void DPTxSSCEnable(BYTE bEnable, BYTE LinkRate)
{

  if(bEnable)
  {
    eDPTX_printMsg("eDPTX SSC Enable");
    //eDPTX_printMsg("Amp=0.43");
    eDPTX_printMsg("Amp=0.20");
    eDPTX_printMsg("MFrq=32Khz");
    if(LinkRate==eDPTXRBR) // RBR
    {
      //msWrite2Byte(REG_LPLL_90,0x3800); // 1.62G -0.25%  = 161.6M
      //msWrite2Byte(REG_LPLL_92,0x0022);
      msWrite2Byte(REG_LPLL_90,0x3300); // 1.62G -0.20%  = 161.7M
      msWrite2Byte(REG_LPLL_92,0x0022);
      //msWrite2Byte(REG_LPLL_9C,0x0007); // Step     +/- 0.25%   33K    162*0.0025=0.40M
      //msWrite2Byte(REG_LPLL_9E,0x02FD); // Spen   +/- 0.25%    33K
      msWrite2Byte(REG_LPLL_9C,0x0005); // Step     +/- 0.20%   (32K,4100ppm)    162*0.002=0.32M
      msWrite2Byte(REG_LPLL_9E,0x0315); // Spen   +/- 0.20%    (32K,4100ppm)
    }
    else  // HBR
    {
      //msWrite2Byte(REG_LPLL_90,0xAA00);// 2.7G -0.25%  269.33M
      //msWrite2Byte(REG_LPLL_92,0x0019);
      //msWrite2Byte(REG_LPLL_90,0xA600);// 2.7G -0.20%  269.49M
      //msWrite2Byte(REG_LPLL_92,0x0019);
      msWrite2Byte(REG_LPLL_90,0xA000);// 2.7G -0.10%  269.74M
      msWrite2Byte(REG_LPLL_92,0x0019);
      //msWrite2Byte(REG_LPLL_9C,0x0004); // Step   +/- 0.25%   33K  270*0.0025=0.67M
      //msWrite2Byte(REG_LPLL_9E,0x03FC); // Spen  +/- 0.25%   33K
      //msWrite2Byte(REG_LPLL_9C,0x0003); // Step   +/- 0.20%   (32K,4300ppm)  270*0.002= 0.54M
      //msWrite2Byte(REG_LPLL_9E,0x041C); // Spen  +/- 0.20%   (32K,4300ppm)
      msWrite2Byte(REG_LPLL_9C,0x0001); // Step   +/- 0.10%   (32K,4300ppm)  270*0.001= 0.27M
      msWrite2Byte(REG_LPLL_9E,0x041D); // Spen  +/- 0.10%   (32K,4300ppm)
    }
    msWriteByteMask(REG_LPLL_87,BIT0,BIT0); // SSC EN
  }else
  {
    eDPTX_printMsg("eDPTX SSC Disable");
    if(LinkRate==eDPTXRBR) // RBR
    {
      msWrite2Byte(REG_LPLL_90,0x2222);// 162M
      msWrite2Byte(REG_LPLL_92,0x0022);
    }
    else  // HBR
    {
      msWrite2Byte(REG_LPLL_90,0x9999);// 270M
      msWrite2Byte(REG_LPLL_92,0x0019);
    }
    msWriteByteMask(REG_LPLL_87,0,BIT0); // SSC DISEN
  }

}

// ###############################################################
// function Name: DPTxMLSignalEn(BYTE PortNum, BYTE bEnable)
// ###############################################################
void DPTxMLSignalEn(BYTE PortNum, BYTE bEnable)
{
  //msWriteByteMask(REG_MOD1_50,0x00,BIT0);  // [0] 1= MOD power down
  if(bEnable==0)
  {
      if(PortNum==0)
      {
        msWriteByte(REG_MOD1_25,0x00); // ch20~23 TTL/standy mode
        //msWriteByteMask(REG_MOD1_54,0x00,BIT7|BIT6|BIT5|BIT4);  // MOD1  Ch20-Ch23 Rterm off
      }
      else
      {
        msWriteByte(REG_MOD2_82,0x00); // ch24~27 TTL/standy mode
        //msWriteByteMask(REG_MOD2_B2,0x00,BIT3|BIT2|BIT1|BIT0);  //MOD2  Ch24-Ch27 Rterm off
      }
      eDPTX_printData("## eDPTX MOD PORT#[%x] ML Signal Off ##\r\n",PortNum);
   }else
   {
      if(PortNum==0)
      {
        msWriteByte(REG_MOD1_25,0x55); // ch20~23 LVDS mode
        //msWriteByteMask(REG_MOD1_54,0xF0,BIT7|BIT6|BIT5|BIT4);  // MOD1  Ch20-Ch23 Rterm On
      }
      else
      {
        msWriteByte(REG_MOD2_82,0x55); // ch24~27  LVDS mode
        //msWriteByteMask(REG_MOD2_B2,0x0F,BIT3|BIT2|BIT1|BIT0);  //MOD2  Ch24-Ch27 Rterm On
      }
      eDPTX_printData("## eDPTX MOD PORT#[%x] ML Signal On ##\r\n",PortNum);
   }
}


Bool DPTxTraining(BYTE PortNum, BYTE LinkRate, BYTE LaneCount)
{
BYTE AuxRxBuf[8];
BYTE AuxTxBuf[8];
BYTE VswingLevel=0;
BYTE VPremphasisLevel=0;
BYTE VMaxSwMaxPreMask=0;
BYTE TrainingLoop=0;
BYTE EnhanceFrame=0x80;
BYTE bCDR_LOCK=FALSE;
BYTE bEQ_LOCK=FALSE;
#if eDPTXDETECTHPD
BYTE HPDPortNum=0;
switch (PortNum)
{
  case 0: HPDPortNum=eDPTXP0UseHPDPortNum; break;
  case 1: HPDPortNum=eDPTXP1UseHPDPortNum; break;
  default: break;
}
if(DPTxCheckHPD(HPDPortNum)){}else {eDPTX_printData("[Training P%d Abort] HPD low",PortNum); DPTxMLSignalEn(PortNum,FALSE);return FALSE;}
if(DPTxCheckHPDIRQ(HPDPortNum)){eDPTX_printData("[Training P%d Abort] HPDIRQ event",PortNum); DPTxMLSignalEn(PortNum,FALSE);return FALSE;}
#endif

#if 1
    msWrite2Byte(REG_MOD1_00,0x0601);  // dis reset
#endif

eDPTX_printData("######## eDP TX PORT#[%x] Training ######",PortNum);
DPTxMLSignalEn(PortNum,TRUE);
DPTxConfigLinkRate(LinkRate);

#if 1
if(DPTxAuxDPCDReadBytes(PortNum,0x000700,1,AuxRxBuf))
{eDPTX_printData("[1.%d]eDP Version",AuxRxBuf[0]);}
if(DPTxAuxDPCDReadBytes(PortNum,0x00000E,1,AuxRxBuf))
{eDPTX_printData("[%x] TRAINING_AUX_RD_INTERVAL * 4ms",AuxRxBuf[0]);}
#endif

#if eDPTXMODPhyCalEn
   DPTxSwingTune(PortNum,eDPTxSwingCalD[0][0]);
   DPTxPreEmphasisTune(PortNum,eDPTxPremphasisCalD[0][0]);
#else
DPTxSwingTune(PortNum,eDP_SWING_DefaultValue);                        // swing = 0x2E
DPTxPreEmphasisTune(PortNum,eDP_PREMPHASIS_DefaultValue); // pre = 0x00
#endif

#if eDPTXEnASSR // method #1
DPTxAuxDPCDReadBytes(PortNum,0x00000D,1,AuxRxBuf);
eDPTX_printData("[%x]eDP Config Cap",AuxRxBuf[0]);
if(AuxRxBuf[0]&BIT0)
{
   eDPTX_printMsg("SINK Support ASSR");
   DPTxAuxDPCDWriteByte(PortNum,0x00010A,BIT0);
   msWriteByteMask(REG_140832+PortNum*0x100,BIT0,BIT0);
}else
{
   eDPTX_printMsg("SINK Not Support ASSR");
   DPTxAuxDPCDWriteByte(PortNum,0x00010A,0);
   msWriteByteMask(REG_140832+PortNum*0x100,0,BIT0);
}
#elif eDPTXEnAFR  //method #2
DPTxAuxDPCDReadBytes(PortNum,0x00000D,1,AuxRxBuf);
eDPTX_printData("[%x]eDP Config Cap",AuxRxBuf[0]);
if(AuxRxBuf[0]&BIT1)
{
   eDPTX_printMsg("SINK Support AFR");
   EnhanceFrame=0;
   DPTxAuxDPCDWriteByte(PortNum,0x00010A,BIT1);
   msWriteByteMask(REG_14082A+PortNum*0x100,BIT7,BIT7);  // enhanceFrame Mode enable
   msWriteByteMask(REG_140832+PortNum*0x100,BIT1,BIT1);
}else
{
   eDPTX_printMsg("SINK Not Support AFR");
   EnhanceFrame=0x80;
   DPTxAuxDPCDWriteByte(PortNum,0x00010A,0);
   msWriteByteMask(REG_14082A+PortNum*0x100,0,BIT7);  // enhanceFrame Mode disable
   msWriteByteMask(REG_140832+PortNum*0x100,0,BIT1);
}
#endif

#if eDPTXAutoTest
// read DPCD 00003 BIT0 => Enable SSC
DPTxAuxDPCDReadBytes(PortNum,0x000003,1,AuxRxBuf);
eDPTX_printData("[%x]DPCD 003",AuxRxBuf[0]);
if(AuxRxBuf[0]==0x01)
{ForceDelay1ms(1);DPTxAuxDPCDWriteByte(PortNum,0x000107,0x10);ForceDelay1ms(1);DPTxSSCEnable(TRUE,LinkRate);}
else
{ForceDelay1ms(1);DPTxAuxDPCDWriteByte(PortNum,0x000107,0x00);ForceDelay1ms(1);DPTxSSCEnable(FALSE,LinkRate);}
#endif

//Read DPCD 600h
DPTxAuxDPCDReadBytes(PortNum,0x000600,1,AuxRxBuf);
eDPTX_printData("[%x]DPCD600",AuxRxBuf[0]);
if(AuxRxBuf[0]!=0x01)
{ForceDelay1ms(1);DPTxAuxDPCDWriteByte(PortNum,0x000600,0x01);ForceDelay1ms(1);}

//Read DevCap
if(DPTxAuxDPCDReadBytes(PortNum,0x000000,4,AuxRxBuf))
{
  eDPTX_printMsg("Read Dev Cap");
  eDPTX_printData("[%x]DPCD Ver.",AuxRxBuf[0]);
  eDPTX_printData("[%x]MAX LinkRate",AuxRxBuf[1]);
  eDPTX_printData("[%x]MAX LaneCount",AuxRxBuf[2]);
  eDPTX_printData("[%x]MAX DownSpread",AuxRxBuf[3]);
}

// DPCD Training Configure
DPTxAuxDPCDWriteByte(PortNum,0x000100,LinkRate); // DPCD 100 HBR=0x0A ,RBR=0x06
ForceDelay1ms(1);
DPTxAuxDPCDWriteByte(PortNum,0x000101,((EnhanceFrame) |(LaneCount))); // DPCD 101 Enhance Frame , LaneCount
ForceDelay1ms(1);
DPTxAuxDPCDReadBytes(PortNum,0x000101,1,AuxRxBuf);
msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1));// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L

#if eDPTXEnSSC
DPTxAuxDPCDWriteByte(PortNum,0x000107,BIT4);// DPCD 107 [4] = SPREAD_AMP   <0.5% , 30~33Khz
#endif


#if eDPTX_NO_AUX_HANDSHAKE_LINK_TRAINING
DPTxAuxDPCDReadBytes(PortNum,0x000003,1,AuxRxBuf);
eDPTX_printData("[%x]DPCD 003",AuxRxBuf[0]);
if(AuxRxBuf[0]&BIT6)
{
  eDPTX_printMsg("SINK Support No_AUX_HandShake_Training");

  AuxTxBuf[0]=(0x00<<3) |0x01; // DPCD 103 L0 = SW[2:0]  / Pre [5:3]
  AuxTxBuf[1]=(0x00<<3) |0x01; // DPCD 104 L1 = SW[2:0]  / Pre [5:3]
  AuxTxBuf[2]=(0x00<<3) |0x01; // DPCD 105 L2 = SW[2:0]  / Pre [5:3]
  AuxTxBuf[3]=(0x00<<3) |0x01; // DPCD 106 L3 = SW[2:0]  / Pre [5:3]
  DPTxAuxDPCDWriteBytes(PortNum,0x000103,4,AuxTxBuf);
#if eDPTXMODPhyCalEn
   DPTxSwingTune(PortNum,eDPTxSwingCalD[1][0]);
   DPTxPreEmphasisTune(PortNum,eDPTxPremphasisCalD[1][0]);
#else
   DPTxSwingTune(PortNum,eDP_SWING_DefaultValue);
   DPTxPreEmphasisTune(PortNum,eDP_PREMPHASIS_DefaultValue); // pre = 0x00
#endif
  msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1)|BIT2);ForceDelay1ms(1);
  msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1)|BIT3);ForceDelay1ms(1);
  msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1));// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L
  DPTxVDClkRst();
  return TRUE;
}else
{eDPTX_printMsg("SINK Not Support No_AUX_HandShake_Training");}
#endif


VswingLevel=0;
VPremphasisLevel=0;
// ##############  Training TP1 ######################
eDPTX_printMsg("Start Training TP1");
DPTxAuxDPCDWriteByte(PortNum,0x000102,0x21);// DPCD 102  Training Pattern  [1:0]  Scramble disable [5]
msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1)|BIT2);// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L
//DPTxAuxDPCDWriteByte(PortNum,0x000102,0x21);// DPCD 102  Training Pattern  [1:0]  Scramble disable [5]

for(TrainingLoop=0;TrainingLoop<5;TrainingLoop++)
{
#if eDPTXDETECTHPD
  if(DPTxCheckHPD(HPDPortNum)){}else{eDPTX_printData("[Training P%d Abort] HPD low",PortNum); DPTxMLSignalEn(PortNum,FALSE);return FALSE;}
  if(DPTxCheckHPDIRQ(HPDPortNum)){eDPTX_printData("[Training P%d Abort] HPDIRQ event",PortNum); DPTxMLSignalEn(PortNum,FALSE);return FALSE;}
#endif
   eDPTX_printData("TP1 TrainingLoop Count %d",TrainingLoop);
//>>>>>> Read Swing Request <<<<<<
  if(DPTxAuxDPCDReadBytes(PortNum,0x000206,2,AuxRxBuf))
  {
     eDPTX_printMsg("Read Swing Req");
     eDPTX_printData("[%x]DPCD 206",AuxRxBuf[0]); // L0 : sw=[1:0] pre=[3:2]
     eDPTX_printData("[%x]DPCD 207",AuxRxBuf[1]);
     VswingLevel=AuxRxBuf[0]&0x03;
     if(VswingLevel>1){VMaxSwMaxPreMask=BIT2;}else{VMaxSwMaxPreMask=0;}; // MAX Swing = BIT2
     eDPTX_printData("VswingLevel=%x",VswingLevel);
   }
#if eDPTXMODPhyCalEn
   DPTxSwingTune(PortNum,eDPTxSwingCalD[VswingLevel][VPremphasisLevel]);
#else
   DPTxSwingTune(PortNum,eDP_SWING_DefaultValue);
#endif
   AuxTxBuf[0]=VMaxSwMaxPreMask|(VPremphasisLevel<<3) |VswingLevel; // DPCD 103 L0 = SW[2:0]  / Pre [5:3]
   AuxTxBuf[1]=VMaxSwMaxPreMask|(VPremphasisLevel<<3) |VswingLevel; // DPCD 104 L1 = SW[2:0]  / Pre [5:3]
   AuxTxBuf[2]=VMaxSwMaxPreMask|(VPremphasisLevel<<3) |VswingLevel; // DPCD 105 L2 = SW[2:0]  / Pre [5:3]
   AuxTxBuf[3]=VMaxSwMaxPreMask|(VPremphasisLevel<<3) |VswingLevel; // DPCD 106 L3 = SW[2:0]  / Pre [5:3]
   DPTxAuxDPCDWriteBytes(PortNum,0x000103,4,AuxTxBuf);
   ForceDelay1ms(1);

// Check CDR Lock
   if(DPTxAuxDPCDReadBytes(PortNum,0x000202,3,AuxRxBuf))
   {
      eDPTX_printMsg("Link Status");
      eDPTX_printData("[%x]DPCD202",AuxRxBuf[0]); // 202
      eDPTX_printData("[%x]DPCD203",AuxRxBuf[1]); // 203
      eDPTX_printData("[%x]DPCD204",AuxRxBuf[2]); // 204
      bCDR_LOCK=FALSE;
      switch(LaneCount)
      {
          case 0x01:
		  	if((AuxRxBuf[0]&0x01)==0x01) {bCDR_LOCK=TRUE;}
          break;
          case 0x02:
		  	if((AuxRxBuf[0]&0x11)==0x11) {bCDR_LOCK=TRUE;}
          break;
          case 0x04:
                     if(((AuxRxBuf[0]&0x11)==0x11)&&((AuxRxBuf[1]&0x11)==0x11)){bCDR_LOCK=TRUE;}
          break;
          default:
		  	bCDR_LOCK=FALSE;
          break;
      }
      //if(((AuxRxBuf[0]&0x11)==0x11)&&((AuxRxBuf[1]&0x11)==0x11))
      if(bCDR_LOCK){eDPTX_printMsg("Link Training CDR Lock!");break;}
   }
   //#####################################################################
   // Check Loop>5 => End Training
   //#####################################################################
   if((bCDR_LOCK==FALSE)&&(TrainingLoop==4))
   {
        DPTxAuxDPCDWriteByte(PortNum,0x000102,0x00);// DPCD 102  Training Pattern  [1:0]
        msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1));// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L
        //DPTxAuxDPCDWriteByte(PortNum,0x000102,0x00);// DPCD 102  Training Pattern  [1:0]
        eDPTX_printMsg("Training Fail");
        eDPTX_printMsg("End Training");
        DPTxMLSignalEn(PortNum,FALSE);
        return FALSE;
    }
}

// ##############  Training TP2 ######################
eDPTX_printMsg("Start Training TP2");
DPTxAuxDPCDWriteByte(PortNum,0x000102,0x22);// DPCD 102  Training Pattern  [1:0]  Scramble disable [5]
msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1)|BIT3);// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L
//DPTxAuxDPCDWriteByte(PortNum,0x000102,0x22);// DPCD 102  Training Pattern  [1:0]  Scramble disable [5]

for(TrainingLoop=0;TrainingLoop<5;TrainingLoop++)
{
#if eDPTXDETECTHPD
if(DPTxCheckHPD(HPDPortNum)){}else{eDPTX_printData("[Training P%d Abort] HPD low",PortNum); DPTxMLSignalEn(PortNum,FALSE);return FALSE;}
if(DPTxCheckHPDIRQ(HPDPortNum)){eDPTX_printData("[Training P%d Abort] HPDIRQ event",PortNum); DPTxMLSignalEn(PortNum,FALSE);return FALSE;}
#endif
   eDPTX_printData("TP2 TrainingLoop Count %d",TrainingLoop);
// >>>>> Read PreEmphasis Request  <<<<<
   if(DPTxAuxDPCDReadBytes(PortNum,0x000206,2,AuxRxBuf))
   {
      eDPTX_printMsg("Read PreEmphasis  Req");
      eDPTX_printData("[%x]DPCD 206",AuxRxBuf[0]);
      eDPTX_printData("[%x]DPCD 207",AuxRxBuf[1]);
      VPremphasisLevel=((AuxRxBuf[0]&0x0C)>>2)&0x03;
      if(VPremphasisLevel>1){VMaxSwMaxPreMask|=BIT5;}else{VMaxSwMaxPreMask&=(~BIT5);};	 // MAX Pre=BIT5
      eDPTX_printData("VPremphasisLevel=%x",VPremphasisLevel);
    }
#if eDPTXMODPhyCalEn
   DPTxPreEmphasisTune(PortNum,eDPTxPremphasisCalD[VswingLevel][VPremphasisLevel]);
#else
   DPTxPreEmphasisTune(PortNum,eDP_PREMPHASIS_DefaultValue); // pre = 0x00
#endif
   AuxTxBuf[0]=VMaxSwMaxPreMask|(VPremphasisLevel<<3) |VswingLevel; // DPCD 103 L0 = SW[2:0]  / Pre [5:3]
   AuxTxBuf[1]=VMaxSwMaxPreMask|(VPremphasisLevel<<3) |VswingLevel; // DPCD 104 L1 = SW[2:0]  / Pre [5:3]
   AuxTxBuf[2]=VMaxSwMaxPreMask|(VPremphasisLevel<<3) |VswingLevel; // DPCD 105 L2 = SW[2:0]  / Pre [5:3]
   AuxTxBuf[3]=VMaxSwMaxPreMask|(VPremphasisLevel<<3) |VswingLevel; // DPCD 106 L3 = SW[2:0]  / Pre [5:3]
   DPTxAuxDPCDWriteBytes(PortNum,0x000103,4,AuxTxBuf);
   ForceDelay1ms(1);

// Check EQ&Symbol Lock , Aligned
   if(DPTxAuxDPCDReadBytes(PortNum,0x000202,3,AuxRxBuf))
   {
      eDPTX_printMsg("Link Status");
      eDPTX_printData("[%x]DPCD202",AuxRxBuf[0]); // 202
      eDPTX_printData("[%x]DPCD203",AuxRxBuf[1]); // 203
      eDPTX_printData("[%x]DPCD204",AuxRxBuf[2]); // 204
      bEQ_LOCK=FALSE;
      switch(LaneCount)
      {
          case 0x01:
		  	if(((AuxRxBuf[0]&0x07)==0x07) &&((AuxRxBuf[2]&0x01)==0x01)){bEQ_LOCK=TRUE;}
          break;
          case 0x02:
		  	if(((AuxRxBuf[0]&0x77)==0x77) &&((AuxRxBuf[2]&0x01)==0x01)){bEQ_LOCK=TRUE;}
          break;
          case 0x04:
                     if(((AuxRxBuf[0]&0x77)==0x77)&&((AuxRxBuf[1]&0x77)==0x77)&&((AuxRxBuf[2]&0x01)==0x01)){bEQ_LOCK=TRUE;}
          break;
          default:
		  	bEQ_LOCK=FALSE;
          break;
      }
      //if((AuxRxBuf[0]==0x77)&&(AuxRxBuf[1]==0x77)&&((AuxRxBuf[2]&0x01)==0x01))
      if(bEQ_LOCK){eDPTX_printMsg("Link Training PASS!");break;}
   }
   //#####################################################################
   // Check Loop>5 => End Training
   //#####################################################################
   if((bEQ_LOCK==FALSE)&&(TrainingLoop==4))
   {
        DPTxAuxDPCDWriteByte(PortNum,0x000102,0x00);// DPCD 102  Training Pattern  [1:0]
        msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1));// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L
        //DPTxAuxDPCDWriteByte(PortNum,0x000102,0x00);// DPCD 102  Training Pattern  [1:0]
        eDPTX_printMsg("Training Fail");
        eDPTX_printMsg("End Training");
        DPTxMLSignalEn(PortNum,FALSE);
        return FALSE;
    }
}

// End Training
DPTxAuxDPCDWriteByte(PortNum,0x000102,0x00);// DPCD 102  Training Pattern  [1:0]  Scramble disable [5]
msWriteByte(REG_140820+PortNum*0x100,(LaneCount>>1));// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L
//DPTxAuxDPCDWriteByte(PortNum,0x000102,0x00);// DPCD 102  Training Pattern  [1:0]  Scramble disable [5]

eDPTX_printMsg("End Training");

// ########################
DPTxVDClkRst();
// ########################

return TRUE;
}

void DPTxSwingTune(BYTE PortNum, BYTE SwingValue)
{
WORD SwingTune;
SwingTune=SwingValue&0x3F;
//Control swing of channel
//6'h0: 40mV +/- 15%
//6'h1: 50mV +/- 15%
//6'h2: 60mV +/- 15%
//6'h3f: 670mV +/- 15%
//Output Swing Formula:
//Vout_swing= 40 + 10 * ICON[5:0] mV +/- 15%
// #### DP spec swing Level ##########
// Lv0= Vpp:400mv . set single pair 200mv = 0x0E/0x10/0x12
// Lv1= Vpp:600mv . set single pair 300mv = 0x18/0x1A/0x1D
// Lv2= Vpp:800mv . set single pair 400mv = 0x22/0x24/0x26
// Lv3= Vpp:1200mv . set single pair 600mv = 0x36/0x38/0x3A
// eDP TX Port#0  L0(ch20) / L1(ch21) / L2(ch22) / L3(ch23)

#if eDPTXMODPhyCalEn
if(SwingValue&BIT7)
msWriteByteMask(REG_MOD1_51,BIT0,BIT0);
else
msWriteByteMask(REG_MOD1_51,0,BIT0);
#endif

if( (PortNum==0)||(PortNum==0x80))
{
msWrite2Byte(REG_MOD1_8E,(SwingTune<<8)|(SwingTune<<14)); // Swing ch20~23 6bits :  ch20=[13:8] ch21=[3:0]+[15:14]
msWrite2Byte(REG_MOD1_90,(SwingTune>>2)|(SwingTune<<4)|(SwingTune<<10)); // Swing ch20~23 6bits : ch21=[3:0]+[15:14] , ch22=[9:4] , ch23=[15:10]
}
// eDP TX Port#1  L0(ch24) / L1(ch25) / L2(ch26) / L3(ch27)
if( (PortNum==1)||(PortNum==0x80))
{
msWrite2Byte(REG_MOD2_8A,SwingTune); // Swing ch24 6bits :  ch24=[5:0]
msWrite2Byte(REG_MOD2_8C,(SwingTune<<6)|(SwingTune<<12)); // Swing ch25~26 6bits : ch25=[11:6] , ch26=[1:0]+[15:12]
msWrite2Byte(REG_MOD2_8E,(SwingTune>>4)|(SwingTune<<2)); // Swing ch26~27 6bits : ch26=[1:0]+[15:12] , ch27=[7:2]
}
}

void DPTxPreEmphasisTune(BYTE PortNum, BYTE PremphasisValue)
{
WORD PreTune;
PreTune=PremphasisValue&0x07;
// preemphasis adjust
//Differential output data/clock pre-emphasis level adjust of channel0
//3'b000: 0mV +/- 15%
//3'b001: 20mV +/- 15 %
//3'b010: 40mV +/- 15%
//¡K
//3'b111: 140mV +/- 15%
//Pre-emphasis Voltage Formula:
//Vpem = 20* ADJ[2:0] mV +/- 15%

// #### DP spec pre Level ##########
// Lv0= 0.0dB(1x):   support swing Lv 0:1:2:3 => all set Value=0
// Lv1= 3.5dB(1.5x):support swing Lv 0:1:2 => swLv0 set 100mv / swLv1 set 150mv / swLv2 set 200mv
// Lv2= 6.0dB(2x):): support swing Lv 0:1 => swLv0 set 200mv / swLv1 set 300mv
// Lv3= 9.5dB(3x):   support swing Lv 0 => swLv0 set 400mv
// 0mv=0x00,100mv=0x05
// eDP TX Port#0  L0(ch20) / L1(ch21) / L2(ch22) / L3(ch23)

#if eDPTXMODPhyCalEn
if(PremphasisValue&BIT7)
msWriteByteMask(REG_MOD2_81,BIT1,BIT1);
else
msWriteByteMask(REG_MOD2_81,0,BIT1);
#endif

if( (PortNum==0)||(PortNum==0x80))
{
msWriteByteMask(REG_MOD1_44,BIT7|BIT6|BIT5|BIT4,BIT7|BIT6|BIT5|BIT4); // pe enable  ch20~23
msWrite2Byte(REG_MOD1_9A,(PreTune<<12)|(PreTune<<15)); // Pe ch20~21 3bits  ch20=[14:12] ch21=[15]
msWrite2Byte(REG_MOD1_9C,(PreTune>>1)|(PreTune<<2)|(PreTune<<5)); // Pe ch21~23 3bits  ch21=[1:0]+[15] , ch22=[4:2] , ch23=[7:5]
}

// eDP TX Port#1  L0(ch24) / L1(ch25) / L2(ch26) / L3(ch27)
if( (PortNum==1)||(PortNum==0x80))
{
msWriteByteMask(REG_MOD2_AA,BIT3|BIT2|BIT1|BIT0,BIT3|BIT2|BIT1|BIT0); // pe enable ch24~ch27
msWrite2Byte(REG_MOD2_A0,(PreTune)|(PreTune<<3)|(PreTune<<6)|(PreTune<<9)); // gcr_pe_adjch24~47 1.4mA  , ch27=[11:9] , ch26=[8:6] , ch25=[5:3] ,ch24=[2:0]
}
}

void DPTxSwingCal(void)
{
BYTE swLv,preLv;
BYTE eFuseValid,eFuse;
BYTE ICON;
// swLv  preLv   X   Y
// 0        0         4    0
// 0        1         5    0.8
// 0        2         5    1.4
// swLv  preLv   X   Y
// 1        0         6    0
// 1        1         6    0.8
// 1        2         6    1.5
// swLv  preLv   X   Y
// 2        0         8    0
// 2        1         8    2

// Icon[5:0]={(eFuse[5:0]+4)*X/6}-4     Fn#1
// Icon[5:0]={(eFuse[5:0]+4)*X/6}-41   Fn#2  (I>63 => REG_MOD1_51[0] =1)
eDPTX_printMsg("##### DPTxSwingCal #####");
eFuse=msEread_GetDataFromEfuse(EFUSE_0, 0x133);
//eFuse=eDP_SWING_DefaultValue;
eDPTX_printData("eFuseData %x",eFuse);
eFuseValid=(eFuse&BIT7)?TRUE:FALSE;
eFuse=eFuse&0x3F;

if(eFuseValid)
{
for(swLv=0;swLv<4;swLv++)
{
  for(preLv=0;preLv<4;preLv++)
  {
    ICON=(((eFuse+4)*eDPTxCalX[swLv][preLv])/6)-4;
    if(ICON>63)
    {
#if eDPTXMODPhyOffSet
      eDPTxSwingCalD[swLv][preLv]=0x80|((((eFuse+4)*eDPTxCalX[swLv][preLv])/6)-41);
#else
      eDPTxSwingCalD[swLv][preLv]=0x3F;//((((eFuse+4)*eDPTxCalX[swLv][preLv])/6)-41);
#endif
    }
    else
    {eDPTxSwingCalD[swLv][preLv]=ICON;}

    eDPTX_printData("SwCal %x",eDPTxSwingCalD[swLv][preLv]);
  }
}
}else
{
	for(swLv=0;swLv<4;swLv++)
	{
	  for(preLv=0;preLv<4;preLv++)
	  {
            eDPTxSwingCalD[swLv][preLv]=eDP_SWING_DefaultValue;
	  }
	}
}

}

void DPTxPreemphasisCal(void)
{
BYTE swLv,preLv;
BYTE eFuseValid,eFuse;
BYTE PeAdj;
// swLv  preLv   X   Y
// 0        0         4    0
// 0        1         5    0.8
// 0        2         5    1.4
// swLv  preLv   X   Y
// 1        0         6    0
// 1        1         6    0.8
// 1        2         6    1.5
// swLv  preLv   X   Y
// 2        0         8    0
// 2        1         8    2

// pe_adj[2:0]=(eFuse[5:0]+4)*Y/6         Fn#3
// pe_adj[2:0]={(eFuse[5:0]+4)*Y/6}-12  Fn#4  (I>7 => REG_MOD2_81[1] =1)
eDPTX_printMsg("### DPTxPremphasisCal ###");
eFuse=msEread_GetDataFromEfuse(EFUSE_0, 0x133);
//eFuse=eDP_PREMPHASIS_DefaultValue;
eDPTX_printData("eFuseData %x",eFuse);
eFuseValid=(eFuse&BIT7)?TRUE:FALSE;
eFuse=eFuse&0x3F;
if(eFuseValid)
{
for(swLv=0;swLv<4;swLv++)
{
  for(preLv=0;preLv<4;preLv++)
  {
    PeAdj=((((eFuse+4)*eDPTxCalY[swLv][preLv])+30)/60);
    if(PeAdj>7)
    {
#if eDPTXMODPhyOffSet
       if(PeAdj<13) // PeAdj=8~12
       {eDPTxPremphasisCalD[swLv][preLv]=0x80;}
	else
    {eDPTxPremphasisCalD[swLv][preLv]=0x80|((((eFuse+4)*eDPTxCalY[swLv][preLv])/60)-12);}
#else
	eDPTxPremphasisCalD[swLv][preLv]=0x07;//((((eFuse+4)*eDPTxCalY[swLv][preLv])/60)-12);
#endif
    }
    else
    {eDPTxPremphasisCalD[swLv][preLv]=PeAdj;}

    eDPTX_printData("PreCal %x",eDPTxPremphasisCalD[swLv][preLv]);
  }
}
}else
{
	for(swLv=0;swLv<4;swLv++)
	{
	  for(preLv=0;preLv<4;preLv++)
	  {
	    eDPTxPremphasisCalD[swLv][preLv]=eDP_PREMPHASIS_DefaultValue;
	  }
	}
}
}


void DPTxPGSyncRst(void)
{
msWrite2Byte(REG_140834,0x0003); // PG EN +Rst
msWrite2Byte(REG_140934,0x0003); // PG EN +Rst
msWriteByteMask(REG_1408A5,0x00,BIT5); // Video clk disable
msWrite2Byte(REG_140834,0x0001); // PG EN
msWrite2Byte(REG_140934,0x0001); // PG EN
msWriteByteMask(REG_1408A5,BIT5,BIT5); // Video clk en  able
}

void DPTxVDClkRst(void)
{
msWriteByteMask(REG_1408A5,0x00,BIT5); // Video clk disable
msWriteByteMask(REG_1408A5,BIT5,BIT5); // Video clk en  able
}

void DPTxHPDIRQRst(void)
{
eDPTX_printMsg("DPTxHPDIRQRst");
msWrite2Byte(REG_1408AC,0x0FFF);  // clear  HPD0~3 IRQ
msWrite2Byte(REG_1408AC,0x0000);  // clear IRQ
}

void DPTxATSTPrintErrCNT(BYTE PortNum)
{
BYTE DPCDRDERRCNTBUF[8];
// ####################### Read & PrintOut Error Cnt Value => verify [Value== #N] #################
// L0: DPCD 211+210 [15]=Valid ,[14:0]=ErrValue
// L1: DPCD 213+212 [15]=Valid ,[14:0]=ErrValue
// L2: DPCD 215+214 [15]=Valid ,[14:0]=ErrValue
// L3: DPCD 217+216 [15]=Valid ,[14:0]=ErrValue

                          DPTxAuxDPCDReadBytes(PortNum,0x00210,8,DPCDRDERRCNTBUF);
                          eDPTX_printData("[DPTX]: L0 error %x",(DPCDRDERRCNTBUF[1]<<8)|DPCDRDERRCNTBUF[0]);
                          eDPTX_printData("[DPTX]: L1 error %x",(DPCDRDERRCNTBUF[3]<<8)|DPCDRDERRCNTBUF[2]);
                          eDPTX_printData("[DPTX]: L2 error %x",(DPCDRDERRCNTBUF[5]<<8)|DPCDRDERRCNTBUF[4]);
                          eDPTX_printData("[DPTX]: L3 error %x",(DPCDRDERRCNTBUF[7]<<8)|DPCDRDERRCNTBUF[6]);
// #######################################################################################
}

#if eDPTXAutoTest

void DPTxATSTReadSwPreReq(BYTE PortNum,BYTE LaneCount)
{
  BYTE AuxRxBuf[2];
  BYTE AuxTxBuf[4];
  BYTE VswingLevel[4]={0,0,0,0};
  BYTE VPremphasisLevel[4]={0,0,0,0};

//>>>>>> Read Swing Request <<<<<<
  if(DPTxAuxDPCDReadBytes(PortNum,0x000206,2,AuxRxBuf))
  {
     eDPTX_printMsg("Read Sw/Pre Req");
     eDPTX_printData("[%x]DPCD 206",AuxRxBuf[0]); // L0 : sw=[1:0] pre=[3:2]
     eDPTX_printData("[%x]DPCD 207",AuxRxBuf[1]);
     VswingLevel[0]=AuxRxBuf[0]&0x03;
     VswingLevel[1]=(AuxRxBuf[0]>>4)&0x03;
     VswingLevel[2]=AuxRxBuf[1]&0x03;
     VswingLevel[3]=(AuxRxBuf[1]>>4)&0x03;
     VPremphasisLevel[0]=(AuxRxBuf[0]>>2)&0x03;
     VPremphasisLevel[1]=(AuxRxBuf[0]>>6)&0x03;
     VPremphasisLevel[2]=(AuxRxBuf[1]>>2)&0x03;
     VPremphasisLevel[3]=(AuxRxBuf[1]>>6)&0x03;
   }
#if eDPTXMODPhyCalEn
   DPTxSwingTune(PortNum,eDPTxSwingCalD[VswingLevel[0]][VPremphasisLevel[0]]);
   DPTxPreEmphasisTune(PortNum,eDPTxPremphasisCalD[VswingLevel[0]][VPremphasisLevel[0]]);
#else
   DPTxSwingTune(PortNum,eDP_SWING_DefaultValue);
   DPTxPreEmphasisTune(PortNum,eDP_PREMPHASIS_DefaultValue); // pre = 0x00
#endif

   AuxTxBuf[0]=(VPremphasisLevel[0]<<3) |VswingLevel[0];  // DPCD 103 L0 = SW[2:0]  / Pre [5:3]
if(LaneCount>1)
   {AuxTxBuf[1]=(VPremphasisLevel[1]<<3) |VswingLevel[1];} // DPCD 104 L1 = SW[2:0]  / Pre [5:3]
if(LaneCount>2)
  {AuxTxBuf[2]=(VPremphasisLevel[2]<<3) |VswingLevel[2];  // DPCD 105 L2 = SW[2:0]  / Pre [5:3]
   AuxTxBuf[3]=(VPremphasisLevel[3]<<3) |VswingLevel[3];} // DPCD 106 L3 = SW[2:0]  / Pre [5:3]

   DPTxAuxDPCDWriteBytes(PortNum,0x000103,LaneCount&0x07,AuxTxBuf);
   ForceDelay1ms(1);
}

Bool DPTxAutoTest(BYTE PortNum)
{

Bool bAutoTestIRQ=FALSE;
BYTE DPCD_201;
BYTE DPCD_218;
#if PHY_TEST_PATTERN
BYTE DPCD_248;
#if TEST_SYMBERR
WORD SYMERRCNT_N;          // for sym Error Count
#endif
#if TEST_PHY80B
BYTE DPCDRDBUF[10];          // for  1.sym Error Count (DPCD0210-0217)  / 2.PHY 80b(DPCD_0250-0259)
#endif
#endif
#if TEST_LINK_TRAINING
BYTE TEST_LINK_RATE;       // DPCD_219
#endif
BYTE TEST_LANE_COUNT;  // DPCD_220

//Step 1:SINK do HPD Short Pulse (200us ~1.5ms) => trigger DPTX to Read [Link Status] DPCD 0x00200~0x00205
//Step 2:Read DPCD 0x00201[1] (DEVICE_SERVICE_IRQ_VECTOR)
//       =>AUTOMATED_TEST_REQUES == 1 ? Yes=>do AUTO TEST(Read 0x218~0x27F)
DPTxAuxDPCDReadByte(PortNum,0x00201,&DPCD_201);
if(DPCD_201&BIT1)
{
  bAutoTestIRQ=TRUE;
  eDPTX_printMsg("[DPTX] Check DPCD201 AUTO TEST IRQ=1");
//Step 3:Write DPCD 0x00201[1]=1 (DEVICE_SERVICE_IRQ_VECTOR) =>Write ¡¥1¡¦ Clear IRQ.
  DPTxAuxDPCDWriteByte(PortNum,0x00201,0x02);
//Step 4: Read DPCD 0x00218 TEST_REQUEST [RO] (Test requested by the Sink Device)??
//       [0]TEST_LINK_TRAINING
//       [1]TEST_PATTERN
//       [2]TEST_EDID_READ
//       [3]PHY_TEST_PATTERN = ? See next page for Step 4-1  check DPCD0248
  ForceDelay1ms(1);
  DPTxAuxDPCDReadByte(PortNum,0x00218,&DPCD_218);
  switch(DPCD_218&0x0F)
  {
     case 0x01:
	 	eDPTX_printMsg("[DPTX] TEST_LINK_TRAINING");
#if TEST_LINK_TRAINING
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
              DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
              eDPTX_printMsg("[DPTX] Start Do Auto Test");
              ForceDelay1ms(1);
		DPTxAuxDPCDReadByte(PortNum,0x00219,&TEST_LINK_RATE);
		eDPTX_printData("[DPTX] TEST_LINK_RATE %x",TEST_LINK_RATE);
		DPTxAuxDPCDReadByte(PortNum,0x00220,&TEST_LANE_COUNT);
		eDPTX_printData("[DPTX] TEST_LANE_COUNT %x",TEST_LANE_COUNT);
// ####################  Call Training Function  ########################
		DPTxTraining(PortNum, TEST_LINK_RATE, TEST_LANE_COUNT);
// #################################################################
#else
		eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
     break;
     case 0x02:
	 	eDPTX_printMsg("[DPTX] TEST_PATTERN");
#if TEST_PATTERN
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
              DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
              eDPTX_printMsg("[DPTX] Start Do Auto Test");
// #################  Call TEST PATTERN FLOW  ######################

// #################################################################
#else
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
              DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
		eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
     break;
     case 0x04:
	 	eDPTX_printMsg("[DPTX] TEST_EDID_READ");
#if TEST_EDID_READ
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
              DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
              eDPTX_printMsg("[DPTX] Start Do Auto Test");
// ####################  Call READ EDID Function ######################

// #################################################################
#else
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
              DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
		eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
     break;
     case 0x08:
	 	eDPTX_printMsg("[DPTX] PHY_TEST_PATTERN");
#if PHY_TEST_PATTERN
		DPTxAuxDPCDReadByte(PortNum,0x00248,&DPCD_248);
		eDPTX_printMsg("[DPTX] DPCD248 PHY_TEST_PATTERN_SEL");
// ####################### Read DPCD to Set TESTLANECOUNT&SW&PRE ###################
		DPTxAuxDPCDReadByte(PortNum,0x00220,&TEST_LANE_COUNT);
	       DPTxATSTReadSwPreReq(PortNum,TEST_LANE_COUNT);
// #####################################################################################
	       switch(DPCD_248&0x07)
	       {
                 case 0x00:
			     eDPTX_printMsg("[DPTX] No Test Pattern selected");
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
		   break;
                 case 0x01:
			     eDPTX_printMsg("[DPTX] D10.2 without scrambling");
#if TEST_D102
// ####################### Set TX D10.2 transmitter ########################################
			     msWriteByte(REG_140820+PortNum*0x100,(TEST_LANE_COUNT>>1)|BIT2);// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L
// #####################################################################################
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
                          eDPTX_printMsg("[DPTX] Start Do Auto Test");
		   break;
#else
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
			     eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
                 case 0x02:
			     eDPTX_printMsg("[DPTX] Symbol Err Measure Count");
#if TEST_SYMBERR

// ####################### Config TX to transmitt SYMBERR ###################################
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
                          eDPTX_printMsg("[DPTX] Start Do Auto Test");
// ####################### Trigger TX transmitt Error Cnt #N ####################################
                          SYMERRCNT_N=0x0005;
                          eDPTX_printData("[DPTX]: Set L0 error = %x",SYMERRCNT_N);
// ####################### Read & PrintOut Error Cnt Value => verify [Value== #N] #################
                          ForceDelay1ms(1000); // Delay 1s
                          DPTxATSTPrintErrCNT(PortNum);
// #######################################################################################
#else
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
			     eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
		   break;
                 case 0x03:
			     eDPTX_printMsg("[DPTX] PRBS7");
#if TEST_PRBS7
// ############################ Config TX to transmitt PRBS7 ##################################
			     //msWriteByte(REG_140820+PortNum*0x100,(TEST_LANE_COUNT>>1)|BIT2);// [3:2]: pattern [1:0] Lane Count 0:1L /1:2L /2:4L
			     //msWriteByteMask(REG_1408A4,BIT6,BIT6);	 // Enable PRBS7
// ########################################################################################
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
                          eDPTX_printMsg("[DPTX] Start Do Auto Test");
// ####################### Read & PrintOut Error Cnt Value #####################################
                          ForceDelay1ms(1000); // Delay 1s
                          DPTxATSTPrintErrCNT(PortNum);
// #######################################################################################
#else
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
			     eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
		   break;
                 case 0x04:
			     eDPTX_printMsg("[DPTX] 80 bit custom pattern transmitted");
#if TEST_PHY80B
// ############################ Read 80b Pattern from DPCD 0250~0259 #########################
                          DPTxAuxDPCDReadBytes(PortNum,0x00250,10,DPCDRDBUF);
// ############################ Config TX to transmitt 80bit ####################################

// ########################################################################################
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
                          eDPTX_printMsg("[DPTX] Start Do Auto Test");
// ####################### Read & PrintOut Error Cnt Value #####################################
                          ForceDelay1ms(1000); // Delay 1s
                          DPTxATSTPrintErrCNT(AuxPortNum);
// #######################################################################################
#else
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
			     eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
		   break;
                 case 0x05:
			     eDPTX_printMsg("[DPTX] HBR2 Compliance EYE pattern");
#if TEST_HBR2EYE
// ############################ Read HBR2 Configure from DPCD ##############################

// ############################ Set TX to transmitt HBR2 EYE Pattern ##########################

// ########################################################################################
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
                          eDPTX_printMsg("[DPTX] Start Do Auto Test");
// ####################### Read & PrintOut Error Cnt Value  ####################################
                          DPTxATSTPrintErrCNT(PortNum);
// #######################################################################################
#else
//Step 5:Write DPCD 0x00260[0]=1 (TEST_ACK) => means [DP TX] tell [DP RX] Start do AUTO TEST.
                          DPTxAuxDPCDWriteByte(PortNum,0x00260,0x01);
			     eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
		   break;
                 default:
			     eDPTX_printMsg("[DPTX] RESERVED");
		   break;
		}
#else
		eDPTX_printMsg("[DPTX] NOT SUPPORT NOW!!");
#endif
     break;
     default:eDPTX_printMsg("[DPTX] DPCD218 No Request");
     break;
  }

}
else
{
  bAutoTestIRQ=FALSE;
  eDPTX_printMsg("[DPTX] DPCD_201 AUTO TEST IRQ=0");
  eDPTX_printMsg("[DPTX] END AUTO TEST FLOW!!");
}

return  bAutoTestIRQ;

}

#endif

#if eDPTXDETECTHPD
void DPTxHPDP0Handle(BYTE PortNum)
{
  WORD HPDStatus=0;
  HPDStatus=msRead2Byte(REG_1408A6); //HPD IRQ [0] Con [1]dis-con [2] interrupt event  3bits/Group

  if(HPDStatus&eDPTXHPDP0_IRQConnect) // P0 HPD Connect
  {
     //eDPTX_printMsg("HPD P0 Connect");
     if(msReadByte(REG_1408A4)&(BIT0<<eDPTXHPDPort0)) // [3:0] HPD0~3 in value
     {
       msWrite2ByteMask(REG_1408AC,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);  // clear IRQ
       msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);
       eDPTX_printMsg("HPD P0 Connect");
#if eDPTXAutoTest
       if(DPTxAutoTest(PortNum)) {}
	else
	{
#endif
       DPTxTraining(PortNum,eDPTXLinkRate,eDPTXLaneCount);
#if eDPTXAutoTest
       }
#endif
       //msWrite2ByteMask(REG_1408AC,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);  // clear IRQ
       //msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);
     }else
     { eDPTX_printMsg("HPD P0 is Low !");}

  }
  else if(HPDStatus&eDPTXHPDP0_IRQEvent) // P0 HPD Event
  {
     eDPTX_printMsg("HPD P0 Event");
     if(msReadByte(REG_1408A4)&(BIT0<<eDPTXHPDPort0)) // [3:0] HPD0~3 in value
     {

#if eDPTXAutoTest
       if(DPTxAutoTest(PortNum))
	{
         msWrite2ByteMask(REG_1408AC,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);  // clear IRQ
         msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);
	}
	else{
#endif

       if(DPTxReadDPCDLinkStatus(PortNum))
       {
         eDPTX_printMsg("P0 Link Status OK!");
         msWrite2ByteMask(REG_1408AC,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);  // clear IRQ
         msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);
	}
       else
       {
         eDPTX_printMsg("P0 ReTraining!");
         msWrite2ByteMask(REG_1408AC,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);  // clear IRQ
         msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQDisConnect|eDPTXHPDP0_IRQEvent);
	  DPTxTraining(PortNum,eDPTXLinkRate,eDPTXLaneCount);
	}
#if eDPTXAutoTest
      }
#endif
     }else
     { eDPTX_printMsg("HPD P0 is Low !");}

     msWrite2ByteMask(REG_1408AC,eDPTXHPDP0_IRQEvent,eDPTXHPDP0_IRQEvent);  // clear IRQ
     msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP0_IRQEvent);

  }

  if(HPDStatus&eDPTXHPDP0_IRQDisConnect) // P0 HPD DisConnect
  {
     eDPTX_printMsg("HPD P0 Disconnect");
     msWrite2ByteMask(REG_1408AC,eDPTXHPDP0_IRQDisConnect,eDPTXHPDP0_IRQDisConnect);  // clear IRQ
     msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP0_IRQDisConnect);
  }
}

void DPTxHPDP1Handle(BYTE PortNum)
{
   WORD HPDStatus=0;
   HPDStatus=msRead2Byte(REG_1408A6); //HPD IRQ [0] Con [1]dis-con [2] interrupt event  3bits/Group

  if(HPDStatus&eDPTXHPDP1_IRQConnect) // P1 HPD Connect
  {
     //eDPTX_printMsg("HPD P1 Connect");
     if(msReadByte(REG_1408A4)&(BIT0<<eDPTXHPDPort1)) // [3:0] HPD0~3 in value
     {
       msWrite2ByteMask(REG_1408AC,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);  // clear IRQ
       msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);
       eDPTX_printMsg("HPD P1 Connect");
#if eDPTXAutoTest
       if(DPTxAutoTest(PortNum)) {}
	else
	{
#endif
       DPTxTraining(PortNum,eDPTXLinkRate,eDPTXLaneCount);
#if eDPTXAutoTest
       }
#endif
       //msWrite2ByteMask(REG_1408AC,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);  // clear IRQ
       //msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);
     }else
     {eDPTX_printMsg("HPD P1 is Low !");}
  }
  else if(HPDStatus&eDPTXHPDP1_IRQEvent) // P1 HPD Event
  {
     eDPTX_printMsg("HPD P1 Event");
     if(msReadByte(REG_1408A4)&(BIT0<<eDPTXHPDPort1)) // [3:0] HPD0~3 in value
     {

#if eDPTXAutoTest
       if(DPTxAutoTest(PortNum))
	{
         msWrite2ByteMask(REG_1408AC,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);  // clear IRQ
         msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);
	}
	else
	{
#endif
       if(DPTxReadDPCDLinkStatus(PortNum))
       {
         eDPTX_printMsg("P1 Link Status OK!");
         msWrite2ByteMask(REG_1408AC,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);  // clear IRQ
         msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);
	}
       else
       {
         eDPTX_printMsg("P1 ReTraining!");
         msWrite2ByteMask(REG_1408AC,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);  // clear IRQ
         msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQDisConnect|eDPTXHPDP1_IRQEvent);
         DPTxTraining(PortNum,eDPTXLinkRate,eDPTXLaneCount);
	}
#if eDPTXAutoTest
       }
#endif

     }else
     {eDPTX_printMsg("HPD P1 is Low !");}

     msWrite2ByteMask(REG_1408AC,eDPTXHPDP1_IRQEvent,eDPTXHPDP1_IRQEvent);  // clear IRQ
     msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP1_IRQEvent);
  }

  if(HPDStatus&eDPTXHPDP1_IRQDisConnect) // P1 HPD DisConnect
  {
     eDPTX_printMsg("HPD P1 Disconnect");
     msWrite2ByteMask(REG_1408AC,eDPTXHPDP1_IRQDisConnect,eDPTXHPDP1_IRQDisConnect);  // clear IRQ
     msWrite2ByteMask(REG_1408AC,0,eDPTXHPDP1_IRQDisConnect);
  }

}
#endif


Bool DPTxCheckHPD(BYTE HPDPortNum)
{
  Bool HPDST0=FALSE;
  Bool HPDST1=FALSE;
  Bool Status=FALSE;

  switch(HPDPortNum)
  {
    case 0x00:
                    HPDST0=(msReadByte(REG_1408A4)&(BIT0<<eDPTXHPDPort0))?TRUE:FALSE;
		      HPDST1=(msReadByte(REG_1408A4)&(BIT0<<eDPTXHPDPort0))?TRUE:FALSE;
                    Status=(HPDST0|HPDST1)?TRUE:FALSE;  // twice low status return false
	  	      break;
    case 0x01:
                    HPDST0=(msReadByte(REG_1408A4)&(BIT0<<eDPTXHPDPort1))?TRUE:FALSE;
		      HPDST1=(msReadByte(REG_1408A4)&(BIT0<<eDPTXHPDPort1))?TRUE:FALSE;
                    Status=(HPDST0|HPDST1)?TRUE:FALSE;  // twice low status return false
		      break;
    default:
  	             break;
  }

  return Status;
}

Bool DPTxCheckHPDIRQ(BYTE HPDPortNum)
{
  Bool Status=FALSE;

  switch(HPDPortNum)
  {
    case 0x00:
                    Status=(msRead2Byte(REG_1408A6)&(eDPTXHPDP0_IRQConnect|eDPTXHPDP0_IRQEvent|eDPTXHPDP0_IRQDisConnect))?TRUE:FALSE;
	  	      break;
    case 0x01:
                    Status=(msRead2Byte(REG_1408A6)&(eDPTXHPDP1_IRQConnect|eDPTXHPDP1_IRQEvent|eDPTXHPDP1_IRQDisConnect))?TRUE:FALSE;
		      break;
    default:
  	             break;
  }
  return Status;
}

void DPTxHandle(void)
{
#if eDPTXDETECTHPD
DPTxHPDP0Handle(0);
DPTxHPDP1Handle(1);
#endif
}

#endif
