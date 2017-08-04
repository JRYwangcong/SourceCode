#include "types.h"
#include "board.h"
#include "global.h"
#include "Menudef.h"
#include "menufunc.h"
#include "adjust.h"
#include <math.h>
#include "ms_reg.h"
#include "msOSD.h"
#include "autofunc.h"
#include "ms_rwreg.h"
#include "Debug.h"
#include "misc.h"
#include "mstar.h"
#include "power.h"
#include "keypaddef.h"
#include "ddc2bi.h"
#include "userpref.h"

#include "msACE.h"
#include "gamma.h"
#include "msflash.h"
#include "mcu.h"
#include "ddc.h"
#include "nvram.h"
#ifdef TSUMXXT//#if( ChipID == ChipTSUMT )		//110311 Modify
#include "msADC.h"
#endif
#include "MsDLC.H"
#if PANEL_VCOM_ADJUST		//121031 Modify
#include "msMiniLVDSI2c.h"
#endif


#define CTEMP_6500K CTEMP_Warm1
#define CTEMP_9300K CTEMP_Cool1
#define CTEMP_7500K CTEMP_Normal

#define UserPrefRedColor6500K UserPrefRedColorWarm1
#define UserPrefGreenColor6500K UserPrefGreenColorWarm1
#define UserPrefBlueColor6500K UserPrefBlueColorWarm1

#define UserPrefRedColor9300K UserPrefRedColorCool1
#define UserPrefGreenColor9300K UserPrefGreenColorCool1
#define UserPrefBlueColor9300K UserPrefBlueColorCool1

#define UserPrefRedColor7500K UserPrefRedColorNormal
#define UserPrefGreenColor7500K UserPrefGreenColorNormal
#define UserPrefBlueColor7500K UserPrefBlueColorNormal

  

extern Bool SetColor(void);
extern Bool SetColorTemp();
extern Bool CheckFactorySettingOutOfRange(void);
void CheckModeSettingRange();
void Init_FactorySetting(void);
extern Bool xdata bFlashWriteFactory;

extern void DrawOsdMenu(void);
#if HKC_ColorTemp_Adjust
Bool HKCAlignControl(void);
#endif

extern WORD CRC16(BYTE nData, WORD nCRC);
#if MainBoardType==MainBoard_715G3603_M0A||MainBoardType==MainBoard_715G3663_M0A || MainBoardType==MainBoard_715G3959_T
void SetScalerGPIO(BYTE pinX, bit HiLow);
#endif



#define SIO_ID1     SIORxBuffer[0]
#define SIO_TYPE1   SIORxBuffer[1]
#define SIO_TYPE2   SIORxBuffer[2]
#define SIO_CMD1    SIORxBuffer[3]
#define SIO_CMD2    SIORxBuffer[4]
#define SIO_DATA1   SIORxBuffer[5]
#define SIO_DATA2   SIORxBuffer[6]
#define SIO_DATA3       SIORxBuffer[7]
#define SIOData10   (SIO_DATA1-0x30)*256
#define SIOData11   ((SIO_DATA1-0x41)+10)*256
#define SIOData20   (SIO_DATA2-0x30)*16
#define SIOData21   ((SIO_DATA2-0x41)+10)*16
#define SIOData30   (SIO_DATA3-0x30)
#define SIOData31   (SIO_DATA3-0x41)+10
BYTE AlignControl(void);
#define ASSETSTARTADDR			0x600
#define ASSETLENGTH				64
//#define MaxBufLen 50
BYTE xdata rxInputPort; // Annie 2006.7.5
//BYTE xdata CP=0;
BYTE xdata DDCBuffer[MaxBufLen];
BYTE xdata rxIndex = 0;
BYTE xdata rxStatus;
BYTE xdata rxLength = 0;
BYTE xdata rxChkSum = 0;
BYTE xdata txStatus;
BYTE *txBufferPtr;
BYTE xdata txLength;
BYTE xdata sendData;

#if FDDCCI_WriteEDID

//bit ubAutoAlign=0;
WORD  idata edid_cont=0;
BYTE  idata edid_pointer=0;
BYTE idata CommandCode;
BYTE idata PageValue;
BYTE idata CPCode;
BYTE idata ValueH;
BYTE idata ValueL;
WORD idata WordValue;

#define S_EDID_VGA			             0 
#define S_EDID_DVI		                    1	
#define S_EDID_HDMI			             2

#if 0//ENABLE_HDCP
#define HDCP_L128		             128
#define HDCP_L33		             	33
WORD xdata  DDCCI_pointer=0;
BYTE xdata  DDCCI_cont=0;
BYTE xdata  Hdcpsave_offset=0 ;
WORD  Hdcp_pointer=0;
WORD  DDCCi_checksun=0;
WORD  Tchecksun=0;
WORD  checksun_verify=0;
#endif
#endif



code BYTE nullMsg1[3] =
{
    0x6e, 0x80, 0xbe
};
code BYTE nullMsg[3] =
{
    0x51, 0x80, 0xbe
};
        BYTE code MODEL_TYPE[] = 
        {
            Model
        };
extern  BYTE MenuPageIndex;
extern  BYTE MenuItemIndex;
// receive/transmit buffer syntax defines
#define LENGTH							0
#define COMMAND						1
#define DATA1							2
#define DATA2							3
#define CHKSUM							4
// DDC2Bi receive/transmit status bytes definitions
#define DDC2B_CLEAR						0
#define DDC2B_SRCADDRESS				1
#define DDC2B_COMMAND					2
#define DDC2B_RECBODY					3
#define DDC2B_WAITFORCK				4
#define DDC2B_COMPLETED				5
#define DDC2B_MSGON					6
#define DDC2B_OVER						7
// DDC2Bi standard definitions
#define DDC2B_DEST_ADDRESS				0x6e
#define DDC2B_SRC_ADDRESS				0x51
#define DDC2Bi_CONTROL_STATUS_FLAG		0x80
// checksum start values
#define INITRxCK			DDC2B_SRC_ADDRESS ^ DDC2B_DEST_ADDRESS
#define INITTxCK			0x50 ^ DDC2B_DEST_ADDRESS
#define INITNMVTxCK		0x50^DDC2B_SRC_ADDRESS
//interrupt flag
#define TXBI	 BIT4   // Annie 2006.7.6 BIT7
#define RCBI	 BIT3  // Annie 2006.7.6  BIT6
#define SlvBMI	BIT5
// maximum wait time (msec) for getting the transmit buffer
#define MAX_TXWAIT_DELAY						20
#define Response_Command_LENGTH      4

BYTE code CAP_VCP[]=
{
"("
"vcp(02 04 05 06 08 0E 10 12 14(01 05 06 08 0B) 16 18 1A 1E 20 30 3E "
#if Dual
"60(01 03) "
#endif
#if AudioFunc
"62 "
#endif
"6C 6E 70 C8 C9 "
#if LANGUAGE_TYPE==ID2007Standard
"CC(01 02 03 04 05 08 09 0A) "
#endif
"B0 B6 D6 DF F8)"
"prot(monitor)"
"type(LCD)"
"cmds(01 02 03 07 0C F3)"
"mccs_ver(2.1)"
"asset_eep(64)"
"mpu_ver("FWVersion")"
"model("Model")"
"mswhql(1)"
")"
};
#if Dual
BYTE code DVI_CAP_VCP[]=
{
"("
"vcp(02 04 05 08 10 12 14(01 05 06 08 0B) 16 18 1A 60(01 03)"
#if AudioFunc
"62 "
#endif
"6C 6E 70 C8 C9 "
#if LANGUAGE_TYPE==ID2007Standard
"CC(01 02 03 04 05 08 09 0A) "
#endif
"B0 B6 D6 DF F8)"
"prot(monitor)"
"type(LCD)"
"cmds(01 02 03 07 0C F3)"
"mccs_ver(2.1)"
"asset_eep(64)"
"mpu_ver("FWVersion")"
"model("Model")"
"mswhql(1)"
")"
};
#endif
void DDC2Bi_InitRx(void)
{
    rxStatus = DDC2B_CLEAR;
    rxIndex = 0;
}
void DDC2Bi_Init(void)
{
 /*   // initialize various flags and data items
#if 0// 2007/10/24  by KK //AOC_TSUM58BJ
  INTENA1 = 0x02;
  INTINV1 = 0x02;
  INTCTRL1 = 0x01;
  INTSTA1 = 0;
#else
    INTCTRL1 = 0x03;
    INTENA1 = 0x01;
#endif
*/
#if 0//(MainBoardType!=MainBoard_2810_B)//AOC_TSUM58BJ  	
    INTCTRL1 = 0x03;
    INTENA1 = 0x01;
#endif

    DDC2Bi_InitRx();
    txStatus = DDC2B_CLEAR;
    ADC_DDCCI_ID = 0xB7;
    INTMASK_EN_ADC = 0x18;
    ADC_INT_FLAG = 0x00;
    #if Dual
        DVI_DDCCI_ID = 0xB7;
        INTMASK_EN_DVI = 0x18;
        DVI_INT_FLAG = 0x00;
    #endif
    //DDC2BI_CTRL = 0x13;
    
	#ifdef TSUMW88MDT3_For_HDMIDDCCI
		DVI1_DDCCI_ID = 0xB7;
		INTMASK_EN_DVI1 = 0x18;
		DVI1_INT_FLAG = 0x00;
	#endif

    DDC2BI_CTRL = 0x0B; // BIT3 < 1 =Interrupt 1 >  < 0 = Interrupt 0 >
    PX1 = 1;
    EX1 = 1;
    IT1 = 0;
    txBufferPtr = &nullMsg1[0];
    txLength = sizeof(nullMsg1);
    if (rxInputPort == 0)
        ADC_RBUF_WDP =  *txBufferPtr++;
    else
    #ifdef TSUMW88MDT3_For_HDMIDDCCI
			if (SrcInputType == Input_Digital)
			DVI_RBUF_WDP = *txBufferPtr++;
		else if (SrcInputType == Input_Digital2)
			DVI1_RBUF_WDP = *txBufferPtr++;
	#else
		DVI_RBUF_WDP = *txBufferPtr++;
	#endif
       
}
void DDC2Bi_GetTxBuffer(void)
{
    BYTE i;
    for (i = MAX_TXWAIT_DELAY; i > 0; i--)
    if (txStatus == DDC2B_MSGON)
    {
        Delay1ms(1);
    }
    txStatus = DDC2B_CLEAR;
}
BYTE DDC2Bi_ComputeChecksum(BYTE count)
{
    BYTE checksum;
    BYTE i = 0;
    checksum = INITTxCK;
    for (; count > 0; count--)
        checksum ^= DDCBuffer[i++];
    return checksum;
}
void DDC2Bi_MessageReady(void)
{

#if HKC_ColorTemp_Adjust||FDDCCI_WriteEDID
if(ATEModeFlag)
{
	if(ubAutoAlign==0)
		Clr_ATEModeFlag();
  DDCBuffer[3]= 0x50^0x6E^DDCBuffer[0]^DDCBuffer[1]^DDCBuffer[2];
}
else
#endif	
{
    DDCBuffer[0] |= DDC2Bi_CONTROL_STATUS_FLAG;
    DDCBuffer[(DDCBuffer[0] &~DDC2Bi_CONTROL_STATUS_FLAG) + 1] = DDC2Bi_ComputeChecksum((DDCBuffer[0] &~DDC2Bi_CONTROL_STATUS_FLAG) + 1);
    DDC2Bi_GetTxBuffer();
}
    txBufferPtr = &DDCBuffer[0];
    #if DDCCI_DEBUG_ENABLE
    Osd_Show();
	Osd_Draw4Num(1,0,DDCBuffer[0]);
	Osd_Draw4Num(1,1,DDCBuffer[1]);
	Osd_Draw4Num(1,2,DDCBuffer[2]);
	Osd_Draw4Num(1,3,DDCBuffer[3]);
	Osd_Draw4Num(1,4,DDCBuffer[4]);
	Osd_Draw4Num(1,5,DDCBuffer[5]);
	Osd_Draw4Num(1,6,DDCBuffer[6]);
	Osd_Draw4Num(1,7,DDCBuffer[7]);
	Osd_Draw4Num(1,8,DDCBuffer[8]);
	Osd_Draw4Num(1,9,DDCBuffer[9]);
	Delay1ms(1000);
    #endif
    txLength = (DDCBuffer[0] &(~DDC2Bi_CONTROL_STATUS_FLAG)) + 2;
}
void DDC2Bi_ParseCommand(void)
{
    BYTE length; //, command;
    length = DDCBuffer[LENGTH] &0x7F;
    if (rxIndex <= 0)
        return ;
    if (length <= 0)
    {
        txBufferPtr = &nullMsg1[1];
        txLength = sizeof(nullMsg1);
		#ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028	
			if (SrcInputType==Input_Analog1)
				ADC_RBUF_WDP = DDC2B_DEST_ADDRESS;
			else if ( SrcInputType == Input_Digital )
				DVI_RBUF_WDP = DDC2B_DEST_ADDRESS;
		
			else if ( SrcInputType == Input_Digital2 )
				DVI1_RBUF_WDP = DDC2B_DEST_ADDRESS;
		#endif
        #if 0
            //Disable it,when ddcci is disable softmccs can show ddcci supported,because it return a null message to softmccs.
            // re enable interrupts
            if (rxInputPort == 0)
                ADC_RBUF_WDP = DDC2B_DEST_ADDRESS;
            else
                DVI_RBUF_WDP = DDC2B_DEST_ADDRESS;
            return ;
        #endif
    }
    //if (!UnsupportedModeFlag)
    //eshin
       // Osd_Hide();
    //MenuPageIndex = RootMenu;
    //MenuItemIndex = 0;
    length = AlignControl();
    if (length <= 0)
    {
        DDCBuffer[0] = 0x80;
        return ;
    }
    else if (length == 0xFE || length == 0xFD)
    {
        DDCBuffer[0] = 0x80;
    }
    DDC2Bi_MessageReady();
    if (rxInputPort == 0)
        ADC_RBUF_WDP = DDC2B_DEST_ADDRESS;
    else
    #ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028
		if ( rxInputPort == Input_Digital )
        DVI_RBUF_WDP = DDC2B_DEST_ADDRESS;
    else if ( rxInputPort == Input_Digital2 )
        DVI1_RBUF_WDP = DDC2B_DEST_ADDRESS;
	#else
	DVI_RBUF_WDP = DDC2B_DEST_ADDRESS;
	#endif
        
}
void DDC2Bi_CommandHandler(void)
{
    if (rxStatus == DDC2B_COMPLETED)
    {
        //Set_DDCHideOsdFlag();
        DDC2Bi_ParseCommand();
        Clr_RxBusyFlag();
        DDC2Bi_InitRx();
    }
}

/*&void DoDDCCI_A(void) // Annie 2006.7.5
{
    BYTE intFlag, rxByte;
    intFlag = ADC_INT_FLAG; //INTFLG;
    if (intFlag)
    {
        switch (intFlag &(TXBI | RCBI))
        {
            case TXBI:
                //
                txStatus = DDC2B_MSGON;
                // no characters left
                if (!(txLength))
                {
                    // clear the transmit status
                    txStatus = DDC2B_CLEAR;
                    ADC_RBUF_WDP = 0;
                    return ;
                }
                // send out the current byte
                ADC_RBUF_WDP =  *txBufferPtr++;
                txLength--;
                intFlag &= (~TXBI); //clear interrupt flag
                break;
            case RCBI:
                // read the received byte
                rxByte = ADC_WBUF_RDP;
                // depending of the message status
                switch (rxStatus)
                {
                    // in case there is nothing received yet
                case DDC2B_CLEAR:
                    if (rxByte == DDC2B_SRC_ADDRESS)
                    {
                        rxStatus++; // = DDC2B_SRCADDRESS;
                        rxChkSum = INITRxCK;
                    }
                    else
                    {
                        DDC2Bi_InitRx();
                    }
                    break;
                    //
                case DDC2B_SRCADDRESS:
                    // get the length
                    rxLength = rxByte &~DDC2Bi_CONTROL_STATUS_FLAG;
                    // put the received byte in DDCBuffer
                    DDCBuffer[rxIndex++] = rxByte;
                    rxChkSum ^= rxByte;
                    // set the receive body state
                    rxStatus++; // = DDC2B_COMMAND;
                    //if it is a NULL message
                    if (rxLength == 0 || rxLength >= MaxBufLen)
                    {
                        rxStatus = DDC2B_COMPLETED;
                        Set_RxBusyFlag();
                    }
                    break;
                    // get the command
                case DDC2B_COMMAND:
                    // save the commandbyte
                    rxStatus++; // = DDC2B_RECBODY;
                    // get the message body
                case DDC2B_RECBODY:
                    DDCBuffer[rxIndex++] = rxByte;
                    rxChkSum ^= rxByte;
                    rxLength--;
                    // the last byte in the message body
                    if (rxLength == 0)
                    {
                        rxStatus++; // = DDC2B_WAITFORCK;
                    }
                    break;
                    // ...here we are waiting for checksum...
                case DDC2B_WAITFORCK:
                    // if checksum match
                    if (rxChkSum == rxByte)
                    {
                        rxInputPort = 0;
                        rxStatus = DDC2B_COMPLETED;
                        Set_RxBusyFlag();
                    }
                    // elsechecksum error
                    else
                    {
                        // if checksum error re-initialize the receive buffer
                        DDC2Bi_InitRx();
                    }
                    break;
                default:
                    // clear the rxState and the current buffer for a new message
                    if (!RxBusyFlag)
                        DDC2Bi_InitRx();
                    break;
                }
                intFlag &= (~RCBI); //clear interrupt flag
                break;
        }
    }
}

#if Dual
    void DoDDCCI_D(void) // Annie 2006.7.5
    {
        BYTE intFlag, rxByte;		
  	#ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028
			  if ( SrcInputType == Input_Digital2 ) 		  
				  intFlag = DVI1_INT_FLAG;
			  else
	#endif	  
        intFlag = DVI_INT_FLAG;
        if (intFlag)
        {
            switch (intFlag &(TXBI | RCBI))
            {
                case TXBI:
                    //
                    txStatus = DDC2B_MSGON;
                    // no characters left
                    if (!(txLength))
                    {
                        // clear the transmit status
                        txStatus = DDC2B_CLEAR;
						#ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028
						if ( SrcInputType == Input_Digital2 )					
							DVI1_RBUF_WDP = 0;
						else		
						#endif
                        DVI_RBUF_WDP = 0;
                        return ;
                    }
                    // send out the current byte
                    #ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028
						   if ( SrcInputType == Input_Digital2 )	   
							   DVI1_RBUF_WDP = *txBufferPtr++;	   
						   else 		 
					#endif	   
                    DVI_RBUF_WDP =  *txBufferPtr++;
                    txLength--;
                    intFlag &= (~TXBI); //clear interrupt flag
                    break;
                case RCBI:
                    // read the received byte
                    #ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028
						 if (SrcInputType == Input_Digital2 )	 
							 rxByte = DVI1_WBUF_RDP;//RCBTXBBUF;
						 else		   
					#endif
                    rxByte = DVI_WBUF_RDP; //RCBTXBBUF;
                    // depending of the message status
                    switch (rxStatus)
                    {
                        // in case there is nothing received yet
                    case DDC2B_CLEAR:
                        if (rxByte == DDC2B_SRC_ADDRESS)
                        {
                            rxStatus++; // = DDC2B_SRCADDRESS;
                            rxChkSum = INITRxCK;
                        }
                        else
                        {
                            DDC2Bi_InitRx();
                        }
                        break;
                        //
                    case DDC2B_SRCADDRESS:
                        // get the length
                        rxLength = rxByte &~DDC2Bi_CONTROL_STATUS_FLAG;
                        // put the received byte in DDCBuffer
                        DDCBuffer[rxIndex++] = rxByte;
                        rxChkSum ^= rxByte;
                        // set the receive body state
                        rxStatus++; // = DDC2B_COMMAND;
                        //if it is a NULL message
                        if (rxLength == 0 || rxLength >= MaxBufLen)
                        {
                            rxStatus = DDC2B_COMPLETED;
                            Set_RxBusyFlag();
                        }
                        break;
                        // get the command
                    case DDC2B_COMMAND:
                        // save the commandbyte
                        rxStatus++; // = DDC2B_RECBODY;
                        // get the message body
                    case DDC2B_RECBODY:
                        DDCBuffer[rxIndex++] = rxByte;
                        rxChkSum ^= rxByte;
                        rxLength--;
                        // the last byte in the message body
                        if (rxLength == 0)
                        {
                            rxStatus++; // = DDC2B_WAITFORCK;
                        }
                        break;
                        // ...here we are waiting for checksum...
                    case DDC2B_WAITFORCK:
                        // if checksum match
                        if (rxChkSum == rxByte)
                        {
                        #ifdef TSUMW88MDT3_For_HDMIDDCCI
							   if (SrcInputType == Input_Digital2 )    
								   rxInputPort = Input_Digital2;
							   else    
						#endif
                            rxInputPort = 1;
                            rxStatus = DDC2B_COMPLETED;
                            Set_RxBusyFlag();
                        }
                        // elsechecksum error
                        else
                        {
                            // if checksum error re-initialize the receive buffer
                            DDC2Bi_InitRx();
                        }
                        break;
                    default:
                        // clear the rxState and the current buffer for a new message
                        if (!RxBusyFlag)
                            DDC2Bi_InitRx();
                        break;
                    }
                    intFlag &= (~RCBI); //clear interrupt flag
                    break;
            }
        }
    }
#endif*/
//===================================================
BYTE AlignControl(void)
{
    BYTE CommandCode, CPCode, ValueH, ValueL, RetValueH, RetValueL, PageValue;
    BYTE TPValue;
    WORD WordValue;
    #if DDCCI_ENABLE
        WORD WordAddr;
    #endif
	 #if HKC_ColorTemp_Adjust
    BYTE  Resault;
 #endif

    CommandCode = DDCBuffer[1];
    PageValue = 0;
   /* #if DDCCI_ENABLE
        if (!DDCciFlag)
        //We can adjust the W/B By DDC2BI When DDCCI Is Disable
    #endif //When DDCCI_ENABLE=0,we use below to disable DDCCI and Use it to adjust W/B
    {
        if (CommandCode == SETVCPFeature || CommandCode == GETVCPFeature)
        {
            CPCode = DDCBuffer[2];
            if (CPCode != 0xFE)
                return FALSE;
        }
        else
            return FALSE;
    }*/
    #if DDCCI_DEBUG_ENABLE

	Osd_Show();
	Osd_Draw4Num(15,0,DDCBuffer[1]);
	Osd_Draw4Num(15,1,DDCBuffer[2]);
	Osd_Draw4Num(15,2,DDCBuffer[3]);
	Osd_Draw4Num(15,3,DDCBuffer[4]);
	Osd_Draw4Num(15,4,DDCBuffer[5]);
	//Delay1ms(1000);
    #endif

    //111----------------------------------------------------------Start
    if (CommandCode == SETVCPFeature)
    // 1-1
    {
        CPCode = DDCBuffer[2];
        RetValueH = DDCBuffer[3];
        RetValueL = DDCBuffer[4];
        WordValue = ((WORD)DDCBuffer[3] << 8) | ((WORD)DDCBuffer[4]);
    }
    else if (CommandCode == GETVCPFeature)
    // 1-2
    {
        CPCode = DDCBuffer[2];
    }
    else if (CommandCode == ModeChangeRequest)
    // 1-3 xx
    {
        CPCode = DDCBuffer[2];
        ValueH = DDCBuffer[3];
    }
    else if (CommandCode == GETPagedVCP)
    // 1-4
    {
        PageValue = DDCBuffer[2];
        CPCode = DDCBuffer[3];
    }
    else if (CommandCode == SETPagedVCP)
    // 1-5
    {
        PageValue = DDCBuffer[2];
        CPCode = DDCBuffer[3];
        RetValueH = DDCBuffer[4];
        RetValueL = DDCBuffer[5];
        WordValue = ((WORD)DDCBuffer[4] << 8) | ((WORD)DDCBuffer[5]);
    }
    #if DDCCI_ENABLE
        else if (CommandCode == CapabilitiesRequest)
        // 1-6
        {
            RetValueH = DDCBuffer[2];
            RetValueL = DDCBuffer[3];
            WordAddr = ((WORD)DDCBuffer[2] << 8) | ((WORD)DDCBuffer[3]);
            #if Dual
                if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2)
                    WordValue = sizeof(DVI_CAP_VCP);
                else
            #endif
                WordValue = sizeof(CAP_VCP);
            if (WordAddr == WordValue)
            {
                DDCBuffer[0] = 0x83;
                DDCBuffer[1] = 0xE3;
                return 3;
            }
            #define CapLen CPCode
            CapLen = (WordAddr + 32 >= WordValue) ? (WordValue - WordAddr): 32;
            DDCBuffer[0] = CapLen + 3+0x80;
            DDCBuffer[1] = 0xE3;
            DDCBuffer[2] = RetValueH;
            DDCBuffer[3] = RetValueL;
            for (ValueL = 4; ValueL < CapLen + 4; ValueL++)
            {
                #if Dual
                    if (SrcInputType == Input_Digital || SrcInputType == Input_Digital2)
                        DDCBuffer[ValueL] = DVI_CAP_VCP[WordAddr + ValueL - 4];
                    else
                #endif
                    DDCBuffer[ValueL] = CAP_VCP[WordAddr + ValueL - 4];
            }
            #undef CapLen
            return DDCBuffer[0] &~0x80;
        }
    #endif
    else if (CommandCode == EnableApp)
    // 1-7 xx
    {
        DDCBuffer[0] = 0x80;
        return 0; //0xFE;
    }
    else if (CommandCode == AppTest)
    // 1-8 xx
    {
        DDCBuffer[0] = 0x80;
        return 0; //0xFD;
    }
    else if (CommandCode == GetTimingRequest)
    // 1-9
    {
        DDCBuffer[0] = 0x06;
        DDCBuffer[1] = 0x4E;
        DDCBuffer[2] = SyncPolarity(SrcFlags);
        if (UnsupportedModeFlag)
            DDCBuffer[2] |= 0x80;
        if (!InputTimingStableFlag)
            DDCBuffer[2] |= 0x40;
        WordValue = HFreq(SrcHPeriod) *10;
        DDCBuffer[3] = WordValue >> 8;
        DDCBuffer[4] = WordValue &0x00FF;
        WordValue = VFreq(WordValue, SrcVTotal);
        DDCBuffer[5] = WordValue >> 8;
        DDCBuffer[6] = WordValue &0x00FF;
        return 6;
    }
    if (CommandCode == GETVCPFeature || CommandCode == GETPagedVCP)
    {
        SIORxBuffer[0] = DDCBuffer[3];//CmdByte  0xE1
        SIORxBuffer[1] = DDCBuffer[4];//ExtByte	eg:0x11->ECO Mode
        SIORxBuffer[2] = DDCBuffer[5];
        SIORxBuffer[3] = DDCBuffer[6];
        SIORxBuffer[4] = DDCBuffer[7];
        SIORxBuffer[5] = DDCBuffer[8];
        SIORxBuffer[6] = DDCBuffer[9];
        SIORxBuffer[7] = DDCBuffer[10];
	}
	else if(CommandCode == SETVCPFeature)
    {//For I-Menu
    //S-6Ea-51a-PktLena-Pre1Bytea-Pre2Bytea-CmdBytea-ExtBytea-DATA1a-DATA2a-CHKa-P

    	#ifdef UseVGACableReadWriteAllPortsEDID
	  #if 1  ///yuanzhi 20100428_1 for 单口烧录不通问题.
	  if((DDCBuffer[2]==0xFE&&DDCBuffer[3]==0xEF)
		||((DDCBuffer[3]==0xE1)&&(DDCBuffer[4]==0xA2)&&(DDCBuffer[5]==0x01))
	     )// && DDCBuffer[4]==0x20)
	  #else
	  if(DDCBuffer[2]==0xFE&&DDCBuffer[3]==0xEF)// && DDCBuffer[4]==0x20)
	  #endif
		{
		  ;
		}
	  else
	#endif
        {
        SIORxBuffer[0] = DDCBuffer[3];//CmdByte  0xE1
        SIORxBuffer[1] = DDCBuffer[4];//ExtByte	eg:0x11->ECO Mode
        SIORxBuffer[2] = DDCBuffer[5];//DATA1	Value High
        SIORxBuffer[3] = DDCBuffer[6];//DATA2	Value Low
        SIORxBuffer[4] = DDCBuffer[7];
        SIORxBuffer[5] = DDCBuffer[8];
        SIORxBuffer[6] = DDCBuffer[9];
        SIORxBuffer[7] = DDCBuffer[10];
	  }
	}
    //111----------------------------------------------------------End
    //GetValue------------------------------------------------------Start
    if (CommandCode == GETVCPFeature || CommandCode == GETPagedVCP)
    {
        //Initial-----------------------Start
        if (CommandCode == GETVCPFeature)
        {
            if(CPCode==TPV_App_Command)
            	{
                DDCBuffer[0] = 0x88;
                DDCBuffer[1] = 0x02;
                DDCBuffer[2] = 0x00;
                DDCBuffer[3] = IMenu_Application;
                //DDCBuffer[4] = SIORxBuffer[1];//ExtByte
            	}
		#ifdef UseVGACableReadWriteAllPortsEDID
	     else if((CPCode==0xFE)&&(DDCBuffer[3]==0xEF))
				{
				  ;
				}
		#endif
            else
            	{
            DDCBuffer[0] = 0x88;
            DDCBuffer[1] = 0x02;
            DDCBuffer[2] = 0x00;
            DDCBuffer[3] = CPCode;
            DDCBuffer[4] = 0x01;
            ValueH = 0x00;
            ValueL = 0xFF;
            RetValueH = 0x00;
            TPValue = 0x01;
        }
        }
        else
        {
            DDCBuffer[0] = 0x89;
            DDCBuffer[1] = 0xC7;
            DDCBuffer[2] = 0x00;
            DDCBuffer[3] = PageValue;
            DDCBuffer[4] = CPCode;
            DDCBuffer[5] = 0x01;
            ValueH = 0x00;
            ValueL = 0xFF;
            RetValueH = 0x00;
            TPValue = 0x01;
        }
        //Initial-----------------------End
        //Setting---------------------------------Start
        if (CPCode == NewControl && PageValue == 0)
        {
             /*if(DDCCINewControlFlag)
            RetValueL = 2;
            else*/
            RetValueL = 1;
            ValueL = 2;
        }
        else if (CPCode == ALL_Reset && PageValue == 0)
        {
            RetValueL = 0;
            ValueL = 1;
        }
        
        else if(CPCode == BriCon_Reset && PageValue == 0)   // 2-2
        {RetValueL = 0;
        ValueL = 1;
        }
        else if (CPCode == Geometry_Reset && PageValue == 0 && SrcInputType < Input_Digital)
        // 2-2
        {
            RetValueL = 0;
            ValueL = 1;
        }
        else if (CPCode == Color_Reset && PageValue == 0)
        // 2-3
        {
            RetValueL = 0;
            ValueL = 1;
        }
        else if (CPCode == ADJ_Clock && PageValue == 0 && SrcInputType < Input_Digital)
        // 2-4
        {
            RetValueH = 0;
            RetValueL = GetClockValue();
            ValueH = 0;
            ValueL = 100;
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_Brightness && PageValue == 0)
        // 2-5
        {
            ValueL = MaxBrightnessValue - MinBrightnessValue;
            RetValueL = UserPrefBrightness - MinBrightnessValue;
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_Contrast && PageValue == 0)
        // 2-6
        {
            ValueL = ((WORD)(MaxContrastValue - MinContrastValue) *100) / (MaxContrastValue - MinContrastValue);
            RetValueL = ((WORD)(UserPrefContrast - MinContrastValue) *100) / (MaxContrastValue - MinContrastValue); //Benz 2007.4.15   20:32:03100;// UserPrefContrast - MinContrastValue;
            TPValue = 0x00;
        }
        else if (CPCode == Select_Color_Preset && PageValue == 0)
        // 2-7
        {
            //Benz 2007.4.15   12:09:45
            ValueL = 0x0D; //0x0B
            if (UserPrefColorTemp == CTEMP_SRGB)
                RetValueL = 0x01;
            else if (UserPrefColorTemp == CTEMP_6500K)
                RetValueL = 0x05;
            else if (UserPrefColorTemp == CTEMP_Normal)
                RetValueL = 0x06;
            else if (UserPrefColorTemp == CTEMP_9300K)
                RetValueL = 0x08;
            else if (UserPrefColorTemp == CTEMP_USER)
                RetValueL = 0x0B;
            TPValue = 0x00;
        }
        else if ((CPCode == ADJ_Red_Gain) && PageValue == 0)
        // 2-8
        {
            ValueL = 100;//(MaxColorValue - MinColorValue);
		#if 0
            if (UserPrefColorTemp == CTEMP_SRGB)
                RetValueL = (WORD)(UserPrefRedColorSRGB - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_USER)
                RetValueL = (WORD)(UserPrefRedColorUser - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_6500K)
                RetValueL = (WORD)(UserPrefRedColor6500K - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_Normal)
                RetValueL = (WORD)(UserPrefRedColorNormal - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_9300K)
                RetValueL = (WORD)(UserPrefRedColor9300K - MinColorValue)*100/MaxColorValue;
		#else
		RetValueL =GetRedColorValue();
		#endif
            TPValue = 0x00;
        }
        else if ((CPCode == ADJ_Green_Gain) && PageValue == 0)
        // 2-9
        {
            ValueL = 100;//(MaxColorValue - MinColorValue);
		#if 0
            if (UserPrefColorTemp == CTEMP_SRGB)
                RetValueL = (WORD)(UserPrefGreenColorSRGB - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_USER)
                RetValueL = (WORD)(UserPrefGreenColorUser - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_6500K)
                RetValueL = (WORD)(UserPrefGreenColor6500K - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_Normal)
                RetValueL = (WORD)(UserPrefGreenColorNormal - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_9300K)
                RetValueL = (WORD)(UserPrefGreenColor9300K - MinColorValue)*100/MaxColorValue;
		#else
		RetValueL =GetGreenColorValue();
		#endif
            TPValue = 0x00;
        }
        else if ((CPCode == ADJ_Blue_Gain) && PageValue == 0)
        // 2-10
        {
            ValueL = 100;//(MaxColorValue - MinColorValue);
		#if 0
            if (UserPrefColorTemp == CTEMP_SRGB)
                RetValueL = (WORD)(UserPrefBlueColorSRGB - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_USER)
                RetValueL = (WORD)(UserPrefBlueColorUser - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_6500K)
                RetValueL = (WORD)(UserPrefBlueColor6500K - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_Normal)
                RetValueL = (WORD)(UserPrefBlueColorNormal - MinColorValue)*100/MaxColorValue;
            else if (UserPrefColorTemp == CTEMP_9300K)
                RetValueL = (WORD)(UserPrefBlueColor9300K - MinColorValue)*100/MaxColorValue;
		#else
		RetValueL =GetBlueColorValue();
		#endif
            TPValue = 0x00;
        }
        else if (CPCode == Auto_Setup && PageValue == 0 && SrcInputType < Input_Digital)
        // 2-11
        {
            RetValueL = 0;
            ValueL = 1;
        }
        else if (CPCode == Auto_Color && PageValue == 0 && SrcInputType < Input_Digital)
        // 2-11
        {
            RetValueL = 0;
            ValueL = 1;
        }
        else if (CPCode == ADJ_HorizontalPos && PageValue == 0 && SrcInputType < Input_Digital)
        // 2-12
        {
        	//110527 Modify
            ValueH = 0; 
            ValueL = 100; 
            RetValueH = 0;
            RetValueL = GetHPositionValue();
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_VerticalPos && PageValue == 0 && SrcInputType < Input_Digital)
        // 2-13
        {
        #if 0
            ValueH = (MaxVStart - MinVStart) >> 8;
            ValueL = (MaxVStart - MinVStart) &0x00FF;
            RetValueH = (UserPrefVStart - MinVStart) >> 8;
            RetValueL = (UserPrefVStart - MinVStart) &0x00FF;
            TPValue = 0x00;
	#else
            RetValueH = 0x00;
            RetValueL = GetVPositionValue();
            ValueH = 0x0;
            ValueL = 100;
            TPValue = 0x00;
	#endif
        }
        else if (CPCode == ADJ_Phase && PageValue == 0 && SrcInputType < Input_Digital)
        // 2-14
        {
            RetValueH = 0x00;
            RetValueL = GetFocusValue();
            ValueH = 0x0;
            ValueL = 100;
            TPValue = 0x00;
        }
        else if (CPCode == Select_InputSource)
        {
            ValueH = 0x00;
            ValueL = 0x04;
            if ((SrcInputType != Input_Digital)||(SrcInputType != Input_Digital2))//yuanfa
                RetValueL = 0x01;
            else
                RetValueH = 0x03;
            TPValue = 0x00;
        }
        #if AudioFunc
            else if (CPCode == ADJ_SpeakerVolume && PageValue == 0)
            {
                ValueH = 0;
                ValueL = 100;
                RetValueL = UserPrefVolume;
                RetValueH = 0;
                TPValue = 0x00;
            }
        #endif
        else if (CPCode == ADJ_Red_Blacklevel && PageValue == 0)
        {
            ValueH = 0x00;
            ValueL = 100;
            RetValueL = UserprefRedBlackLevel;
            TPValue = 0;
        }
        else if (CPCode == ADJ_Green_Blacklevel && PageValue == 0)
        {
            ValueH = 0x00;
            ValueL = 100;
            RetValueL = UserprefGreenBlackLevel;
            TPValue = 0;
        }
        else if (CPCode == ADJ_Blue_Blacklevel && PageValue == 0)
        {
            ValueH = 0x00;
            ValueL = 100;
            RetValueL = UserprefBlueBlackLevel;
            TPValue = 0;
        }
        else if (CPCode == H_Frequency && PageValue == 0)
        {
            ValueH = 0xFF;
            ValueL = 0xFF;
            WordValue = HFreq(SrcHPeriod) *10;
            RetValueH = WordValue >> 8;
            RetValueL = WordValue &0x00ff;
            TPValue = 0x01;
        }
        else if (CPCode == V_Frequency && PageValue == 0)
        {
            ValueH = 0xFF;
            ValueL = 0xFF;
            WordValue = HFreq(SrcHPeriod) *10;
            WordValue = VFreq(WordValue, SrcVTotal);
            RetValueH = WordValue >> 8;
            RetValueL = WordValue &0x00ff;
            TPValue = 0x01;
        }
        else if (CPCode == FlatPanellayout)
        {
            ValueL = 8;
            RetValueL = 1;
        }
        else if (CPCode == MonitorType && PageValue == 0)
        // 2-20
        {
            RetValueH = 0;
            RetValueL = 3;
            ValueH = 0x00;
            ValueL = 0x08;
            TPValue = 0x00;
	 }
        else if (CPCode == DisplayControlType)
        // Jeff add it in 20047る13らと 04:46:32
        {
            ValueH = 0xFF;
            ValueL = 0x16;
            RetValueH = 0;
            RetValueL = 5; // Mean Mstar
        }
        else if (CPCode == FWLevel)
        {
            ValueH = 0xFF;
            ValueL = 0xFF;
            RetValueL = Version;
            RetValueH = Version>>16;
        }
        else if (CPCode == ADJ_Language && PageValue == 0)
        // 2-17
        {
            #if LANGUAGE_TYPE==ID2007Standard
                ValueL = LANG_Nums;
                if (UserPrefLanguage == LANG_English)
                    RetValueL = 2;
                else if (UserPrefLanguage == LANG_Spanish)
                    RetValueL = 10;
                else if (UserPrefLanguage == LANG_France)
                    RetValueL = 3;
                else if (UserPrefLanguage == LANG_German)
                    RetValueL = 4;
                else if (UserPrefLanguage == LANG_Italian)
                    RetValueL = 5;
                else if (UserPrefLanguage == LANG_Portugues)
                    RetValueL = 8;
                else if (UserPrefLanguage == LANG_Russia)
                    RetValueL = 9;
                else if (UserPrefLanguage == LANG_SChina)
                    RetValueL = 1;
                TPValue = 0x00;
            #endif
        }
#if OSD_PIVOT_EN
        else if ( CPCode == Screen_Orientation && PageValue == 0 )
        {
            ValueL = 0xFF;
            ValueH = 0xFF;

            RetValueL = ( hw_PnlDirtDect) ? ( 2 ) : ( 1 );

            RetValueH = 0x00;

        }
#endif
        else if (CPCode == VCPVersion && PageValue == 0)
        {
            RetValueL = 1;
            RetValueH = 2;
            ValueH = 0xFF;
            ValueL = 0xFF;
        }
            else if (CPCode == 0xFE)
            {
       if (SIORxBuffer[0]=='R')
            {
                if (SIORxBuffer[1]=='M' && SIORxBuffer[2]=='N'&&SIORxBuffer[3]=='0')
                {
                    TPValue=SIORxBuffer[3];
                    ValueH = SIORxBuffer[4];
                    if(SIORxBuffer[4]>='0'&&SIORxBuffer[4]<='3')
                    {   
                        BYTE i;
			   i=SIORxBuffer[4]-'0';
                        ValueL = MODEL_TYPE[3*i];
                        RetValueH = MODEL_TYPE[3*i+1];
                        RetValueL = MODEL_TYPE[3*i+2];
                    }
                } 
         #ifdef ReduceDDC
		else if((SIORxBuffer[1]=='E' && SIORxBuffer[2]=='D'&&SIORxBuffer[3]=='I'))//'R'	'E'	'D'	'I'	'D'	'W'	'A'	'Y'
		#ifdef OnlyVGAEDIDInFlash
                {//'R'	'E'	'D'	'I'	'D'	'V'	'G'	'A'
                    TPValue='I';
                    ValueH = 'D';
                    ValueL = 'V';
                    RetValueH = 'G';
                    RetValueL = 'A';
                }
		#else
                {//'R'	'E'	'D'	'I'	'D'	'F'	'L'	'H'
                    TPValue='I';
                    ValueH = 'D';
                    ValueL = 'F';
                    RetValueH = 'L';
                    RetValueL = 'H';
                }
		#endif
        #endif 
        #if HDCPKEY_IN_Flash
		else if((SIORxBuffer[1]=='H' && SIORxBuffer[2]=='D'&&SIORxBuffer[3]=='C'))
                {
                    ValueH = TCRCvalue / 256;
                    ValueL = TCRCvalue % 256;
                    RetValueH = 0x88;
                    RetValueL = 0x02;

		      		Clr_WriteHDCPcodeFlag();
            }
        #endif 
       	}
		#ifdef	UseVGACableReadWriteAllPortsEDID
			else if(SIORxBuffer[0]==0xEF)
			{
				BYTE i,Length;
				WORD Addr;

				Length = DDCBuffer[7];
				Addr=(DDCBuffer[5]<<8)+DDCBuffer[6];
				if((DDCBuffer[4]>=VGA1&&DDCBuffer[4] <=DP2)||(DDCBuffer[4]>=EEPROM&&DDCBuffer[4] <=PanelSettingDevice))
				{
					ReadDevice(DDCBuffer[4],Addr,Length);//Read EDID or HDCP KEY
					for(i = 0; i < Length; i++)
					{
						DDCBuffer[i+1] = SIORxBuffer[i];
					}
				}
				else
				{
					;
				}
				DDCBuffer[0]= 0x80+Length;
				return Length;
			}
		#endif
            	
	   #if HDCPKEY_IN_Flash
		else if(SIORxBuffer[0]==0xE1 &&  SIORxBuffer[1]==0xE2 &&  SIORxBuffer[2]==0x00 && SIORxBuffer[3]==0x00)
                {
                    ValueH = TCRCvalue / 256;
                    ValueL = TCRCvalue % 256;
                    RetValueH = 0x00;
                    RetValueL = 0x00;
		      		Clr_WriteHDCPcodeFlag();
            	}
        #endif 

	   //==============NewFactoryCommand=================
	#if NewFactoryCommand 
		#define TPVCMD1 	DDCBuffer[3] 		
			else if(TPVCMD1==TPV_AdjRedGain)
			{           
			}
			else if(TPVCMD1==TPV_AdjGreenGain)
			{
			}
			else if(TPVCMD1==TPV_AdjBlueGain)
			{
			}
			else if(TPVCMD1==TPV_AdjContrast)
			{
			}
			else if(TPVCMD1==TPV_AdjBrightness)
			{
			}
			else if(TPVCMD1==TPV_SetDefLanguage)
			{
			}
			else if(TPVCMD1==TPV_SelectInput)
			{
			}
			else if(TPVCMD1==TPV_SelectGamma)
			{
			}
		#undef	TPVCMD1
	#endif	
		//==========================================

        }
/***********************************************Zhifeng.Wu***********************************************/
	#define	I_Menu_Command	SIORxBuffer[1]

         //For TPV Application
        else if (CPCode == TPV_App_Command&& PageValue == 0)
        {
		if(SIORxBuffer[0]==IMenu_Application)//For AOC I-Menu Application
		{
			//For Other Luminance Menu
			if(I_Menu_Command==ECO_Mode)
			{
                         RetValueH = 0;
                         RetValueL = UserPrefECOMode;
                         ValueH = 0;
                         ValueL = ECO_Nums-1;
			}
			else if(I_Menu_Command==Gamma)
			{
                         RetValueH = 0;
                         RetValueL = UserPrefGamaMode;
                         ValueH = 0;
                         ValueL = GAMA_Nums-1;
			}
			else if(I_Menu_Command==DCR)
			{
                         RetValueH = 0;
                         RetValueL = UserPrefDcrMode;
                         ValueH = 0;
                         ValueL = 1;
			}
			//For All Color Boost Menu Items
			else if((I_Menu_Command&0xF0)==0x20)
			{	    
                         RetValueH = 0;
                         RetValueL =((UserPrefColorFlags&(1<<((I_Menu_Command&0x0F)-1))) >0)?1:0;
                         ValueH = 0;
                         ValueL = 1;
			}
			//For Picture Boost Items
			else if(I_Menu_Command==FrameSize)
			{
                         RetValueH = 0;
                         RetValueL = GetBFSizeValue();
                         ValueH = 0;
                         ValueL = 100;
			}
			else if(I_Menu_Command==PB_Brightness)
			{
                         RetValueH = 0;
                         RetValueL = GetSubBrightness100Value();
                         ValueH = 0;
                         ValueL = 100;
			}
			else if(I_Menu_Command==PB_Contrast)
			{
                         RetValueH = 0;
                         RetValueL = GetSubContrast100Value();
                         ValueH = 0;
                         ValueL = 100;
			}
			else if(I_Menu_Command==PB_Hue)
			{
			}
			else if(I_Menu_Command==PB_Saturation)
			{
			}
			else if(I_Menu_Command==PB_Hpos)
			{
                         RetValueH = 0;
                         RetValueL = GetBFHstartValue();
                         ValueH = 0;
                         ValueL = 100;
			}
			else if(I_Menu_Command==PB_Vpos)
			{
                         RetValueH = 0;
                         RetValueL = GetBFVstartValue();
                         ValueH = 0;
                         ValueL = 100;
			}
			else if(I_Menu_Command==PB_BrightFrame)
			{
                         RetValueH = 0;
                         RetValueL = ((UserPrefColorFlags&bPictureBoostBit) >0)?1:0;
                         ValueH = 0;
                         ValueL = 1;
			}
		}
        }
	#undef	I_Menu_Command
/*******************************************************************************************************/
        
        else if ( CPCode == PowerMode )
        {
            ValueL = 4;
            if ( !PowerOnFlag )
            {
                RetValueL = 4;
            }
            else if ( PowerSavingFlag )
            {
                RetValueL = 2;
            }
            else
            {
                RetValueL = 1;
            }
        }		
#if  FDDCCI_WriteEDID
	else  if(CPCode == GetUserTime&& PageValue == 0)
		{
			TPValue = 0x00;
			ValueH		=((BlacklitTime/3600)>>8)&&0xFF;
			ValueL		=(BlacklitTime/3600)&&0xFF;
			RetValueH	=(BlacklitTime/60)%60;
			RetValueL	=BlacklitTime%60;	
			Clr_ATEModeFlag();
		}
	else if(CPCode == CleanUsedTime&& PageValue == 0)
		{
			#if  BlacklitTimeOnShow

			//Reset_Time();
			MonitorBlacklitTimeOn_S=BlacklitTimeOn_S=0;
	       	MonitorBlacklitTimeOn_M=BlacklitTimeOn_M=0;  // MonitorSetting.BlacklitTimeOn_M
              	MonitorBlacklitTimeOn_H=BlacklitTimeOn_H=0;   //MonitorSetting.BlacklitTimeOn_H
              	#endif
			SaveMonitorSetting2();//SaveMonitorSetting();
			//ExecuteKeyEvent(MIA_ResetTime);
			Clr_ATEModeFlag();
			Set_DDCCIResetPowerOffFlag();
		}
#endif

        else
        {
            DDCBuffer[2] = 0x01;
            RetValueL = 0x00;
            ValueH = 0xFF;
            ValueL = 0xFF;
        }
        //Setting---------------------------------End
        //Transmission----------------------------Start
        if (CommandCode == GETVCPFeature)
        {
            if(CPCode==TPV_App_Command)
		{
			DDCBuffer[5] = ValueH;
			DDCBuffer[6] = ValueL;
			DDCBuffer[7] = RetValueH;
			DDCBuffer[8] = RetValueL;
		}
		else
		{
            DDCBuffer[4] = TPValue;
            DDCBuffer[5] = ValueH;
            DDCBuffer[6] = ValueL;
            DDCBuffer[7] = RetValueH;
            DDCBuffer[8] = RetValueL;
		}
            return 8;
        }
        else
        {
            DDCBuffer[5] = TPValue;
            DDCBuffer[6] = ValueH;
            DDCBuffer[7] = ValueL;
            DDCBuffer[8] = RetValueH;
            DDCBuffer[9] = RetValueL;
            return 9;
        }
        //Transmission----------------------------End
    }
    //GetValue------------------------------------------------------Start
    //SetValue------------------------------------------------------Start
    else if (CommandCode == SETVCPFeature || CommandCode == SETPagedVCP)
    {
        DDCBuffer[2] = 0x00; // Result Code
        ValueH = 0x00;
        ValueL = 0xFF;
        TPValue = 0x01;
        if (CPCode == NewControl && PageValue == 0)
        {
            //Clr_DDCCINewControlFlag();
            ValueL = 2;
        }
        else if (CPCode == ALL_Reset && PageValue == 0)
        // 3-1
        {
            if (WordValue != 0)
            {
                ResetAllSetting();
		#if(PanelType == PanelM185WHB_N10)
		    mStar_WriteByte(BCR, 0x80);//0x80);
		    mStar_WriteByte(BCG, 0x80);//0x80);
		    mStar_WriteByte(BCB, 0x80);//0x80);
		#elif(PanelType == PanelLM185TT3A)
			#if LM185TT3A_240MA
			    mStar_WriteByte(BCR, 0x95);//0x80);
			    mStar_WriteByte(BCG, 0x95);//0x80);
			    mStar_WriteByte(BCB, 0x95);//0x80);
			#else
			    mStar_WriteByte(BCR, 0x82);//0x80);
			    mStar_WriteByte(BCG, 0x82);//0x80);
			    mStar_WriteByte(BCB, 0x82);//0x80);
			#endif
		#else
		    mStar_WriteByte(BCR, 0x7C);//0x80);
		    mStar_WriteByte(BCG, 0x7C);//0x80);
		    mStar_WriteByte(BCB, 0x7C);//0x80);
		#endif
            }
            ValueL = 1;
        }
        
        else if(CPCode == BriCon_Reset&& PageValue == 0)   // 3-2
        { 
            if((UserPrefDcrMode == 0) && (UserPrefECOMode == ECO_Standard) && (UserPrefColorTemp != CTEMP_SRGB))
            {
                if(WordValue != 0 )
                {
                    UserPrefBrightness=DefBrightness;
                    UserPrefContrast=DefContrast;

                    UserPrefBrightnessUser = UserPrefBrightness;
    		        UserPrefContrastUser = UserPrefContrast;
    	  
                    UserPrefBrightnessCool1 = UserPrefBrightness;
    		        UserPrefContrastCool1 = UserPrefContrast;
            
                    UserPrefBrightnessWarm1 = UserPrefBrightness;
    		        UserPrefContrastWarm1 = UserPrefContrast;
            
                    UserPrefBrightnessNormal = UserPrefBrightness;
    		        UserPrefContrastNormal = UserPrefContrast;
            
                    UserPrefBrightnessSRGB = UserPrefBrightness;
    		        UserPrefContrastSRGB = UserPrefContrast;
                
                    mStar_AdjustBrightness(UserPrefBrightness);
                    mStar_AdjustContrast(UserPrefContrast);//petit 20121203
                }
            }
            ValueL = 1;
        }	 
        else if (CPCode == Geometry_Reset && PageValue == 0 && rxInputPort != Input_Digital)
        // 3-2
        {
            //Osd_Hide();
            if (WordValue != 0 && (SrcInputType != Input_Digital)&&(SrcInputType != Input_Digital2))//yuanfa
                ResetGeometry();
            ValueL = 1;
            TPValue = 0x00;
        }
        else if (CPCode == Color_Reset && PageValue == 0)
        // 3-3
        {
            UserPrefRedColorUser = DefRedColor;
            UserPrefGreenColorUser= DefGreenColor;
            UserPrefBlueColorUser= DefBlueColor;
            UserprefRedBlackLevel = 50;
            UserprefGreenBlackLevel = 50;
            UserprefBlueBlackLevel = 50;
            mStar_AdjustUserPrefBlacklevel(UserprefRedBlackLevel, UserprefGreenBlackLevel, UserprefBlueBlackLevel);
            UserPrefColorTemp = CTEMP_Warm1; //CTEMP_9300K 20051115 wmz
            if (WordValue != 0)
                SetColorTemp();//Setcolortemp();
            //Benz 2007.4.15   19:31:44SetColor();
            ValueL = 1;
        }
        else if (CPCode == ADJ_Clock && PageValue == 0 && SrcInputType < Input_Digital)
        // 3-4
        {
            //Osd_Hide();
            if (SrcInputType != Input_Digital)
            {
                UserPrefHTotal = ((WordValue *(MaxClock - MinClock) + 50) / 0x64 + MinClock);
                #if WaitForDataBlanking  //zjb20071219
                mStar_WaitForDataBlanking();
                #endif		
				#ifdef TSUMXXT//#if (ChipID == ChipTSUMT)		//110311 Modify
				msADC_AdjustHTotal(UserPrefHTotal);
				#else
                		mStar_AdjustHTotal(UserPrefHTotal);
				#endif
            }
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_Brightness && PageValue == 0)
        // 3-5
        {
            if((UserPrefDcrMode == 0) && (UserPrefECOMode == ECO_Standard) && (UserPrefColorTemp != CTEMP_SRGB))
            {
                UserPrefBrightness = RetValueL + MinBrightnessValue;
                if (UserPrefColorTemp == CTEMP_USER)
                    UserPrefBrightnessUser = UserPrefBrightness;
                else if (UserPrefColorTemp == CTEMP_Cool1)
                {
                    UserPrefBrightnessCool1 = UserPrefBrightness;
                }
                else if (UserPrefColorTemp == CTEMP_Warm1)
                {
                    UserPrefBrightnessWarm1 = UserPrefBrightness;
                }
                else if (UserPrefColorTemp == CTEMP_Normal)
                {
                    UserPrefBrightnessNormal = UserPrefBrightness;
                }
                else if (UserPrefColorTemp == CTEMP_SRGB)
                {
                    UserPrefBrightnessSRGB = UserPrefBrightness;
                }
                mStar_AdjustBrightness(UserPrefBrightness);
            }
            ValueL = MaxBrightnessValue - MinBrightnessValue;
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_Contrast && PageValue == 0)
        // 3-6
        {
            //Osd_Hide();
            if((UserPrefDcrMode == 0) && (UserPrefECOMode == ECO_Standard) && (UserPrefColorTemp != CTEMP_SRGB))
            {
                if (RetValueL <= (MaxContrastValue - MinContrastValue))
                {
                    UserPrefContrast = (WORD)(RetValueL *(MaxContrastValue - MinContrastValue)) / 100+MinContrastValue;
                    if (UserPrefColorTemp == CTEMP_USER)
                    {
                        UserPrefContrastUser = UserPrefContrast;
                    }
                    else if (UserPrefColorTemp == CTEMP_Cool1)
                    {
                        UserPrefContrastCool1 = UserPrefContrast;
                    }
                    else if (UserPrefColorTemp == CTEMP_Warm1)
                    {
                        UserPrefContrastWarm1 = UserPrefContrast;
                    }
                    else if (UserPrefColorTemp == CTEMP_Normal)
                    {
                        UserPrefContrastNormal = UserPrefContrast;
                    }
                    else if (UserPrefColorTemp == CTEMP_SRGB)
                    {
                        UserPrefContrastSRGB = UserPrefContrast;
                    }
                    mStar_AdjustContrast(UserPrefContrast);
                }
            }
            ValueL = 100;
            TPValue = 0x00;
        }
        else if (CPCode == Select_Color_Preset && PageValue == 0)
        // 3-7
        {
            if (RetValueL == 0x01)
                UserPrefColorTemp = CTEMP_SRGB;
            else if (RetValueL == 0x05)
                UserPrefColorTemp = CTEMP_6500K;
            else if (RetValueL == 0x06)
                UserPrefColorTemp = CTEMP_Normal;
            else if (RetValueL == 0x08)
                UserPrefColorTemp = CTEMP_9300K;
            else if (RetValueL == 0x0B)
                UserPrefColorTemp = CTEMP_USER;
                //SetColorTemp();//Setcolortemp();

   	   #if Enable_Lightsensor // 091029 coding
   	    if(UserPrefColorTemp==CTEMP_SRGB&&(UserPrefDcrMode||UserPrefiCareMode)) //20120216 hong for imenu when dcr on,change to sRGB,dcr cannot be off 
   	    {
			UserPrefDcrMode = 0;
			UserPrefiCareMode = LightSensorOff;
			msAccOnOff(0);
			msDlcOnOff(0);
   	    }
   	   #else
   	     if(UserPrefColorTemp==CTEMP_SRGB&&UserPrefDcrMode)
   	    	{
   		    	UserPrefDcrMode = 0;
   			msAccOnOff(0);
   			msDlcOnOff(0);
   	    	}
   	   #endif

		if (UserPrefColorTemp == CTEMP_SRGB && UserPrefECOMode != ECO_Standard)
		{
			UserPrefECOMode = ECO_Standard;
		}
			
		ReadColorTempSetting();

            ValueL = 0x0D; //0x0B
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_Red_Gain && PageValue == 0)
        // 3-8
        {
            if ((UserPrefColorTemp == CTEMP_USER) && (UserPrefDcrMode == 0))
            {
               // if(RetValueL<=10)
		    // RetValueL=10;
		        UserPrefRedColorUser=UserPrefRedColor = (((DWORD)RetValueL*(MaxColorValue-MinColorValue))/100+MinColorValue);
             
              // UserPrefRedColorUser=UserPrefRedColor = (WORD)RetValueL*MaxColorValue/100;
                //mStar_AdjustRedColor(UserPrefContrast, UserPrefRedColor);//petit 20130110
			   mStar_AdjustRedColor(UserPrefRedColor, UserPrefContrast);
            }
            ValueL = 100;//(MaxColorValue - MinColorValue);
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_Green_Gain && PageValue == 0)
        // 3-9
        {
            if ((UserPrefColorTemp == CTEMP_USER) && (UserPrefDcrMode == 0))
            {
                //if(RetValueL<=10)
		     // RetValueL=10;
		     UserPrefGreenColorUser = UserPrefGreenColor = (((DWORD)RetValueL*(MaxColorValue-MinColorValue))/100+MinColorValue);
             
               // UserPrefGreenColorUser = UserPrefGreenColor = (WORD)RetValueL*MaxColorValue/100 ;
                //mStar_AdjustGreenColor(UserPrefContrast, UserPrefGreenColor);
                mStar_AdjustGreenColor(UserPrefGreenColor, UserPrefContrast);//petit 20130110
            }
            ValueL = 100;//(MaxColorValue - MinColorValue);
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_Blue_Gain && PageValue == 0)
        // 3-10
        {
            if ((UserPrefColorTemp == CTEMP_USER) && (UserPrefDcrMode == 0))
            {
                //if(RetValueL<=10)
		     // RetValueL=10;
		       UserPrefBlueColorUser = UserPrefBlueColor = (((DWORD)RetValueL*(MaxColorValue-MinColorValue))/100+MinColorValue);
             
                //UserPrefBlueColorUser = UserPrefBlueColor = (WORD)RetValueL*MaxColorValue/100;
               // mStar_AdjustBlueColor(UserPrefContrast, UserPrefBlueColor);
		mStar_AdjustBlueColor(UserPrefBlueColor, UserPrefContrast);
            }
            ValueL = 100;//(MaxColorValue - MinColorValue);
            TPValue = 0x00;
        }
        else if (CPCode == Auto_Setup && PageValue == 0 && SrcInputType < Input_Digital)
        // 3-11
        {
            //Osd_Hide();
            if (WordValue != 0 && SrcInputType != Input_Digital)
                AutoConfig();
            ValueL = 1;
        }
        else if (CPCode == Auto_Color && PageValue == 0 && SrcInputType < Input_Digital)
        // 3-11
        {
            if (WordValue != 0 && SrcInputType != Input_Digital)
                mStar_AutoColor();
            ValueL = 1;
        }
        else if (CPCode == ADJ_HorizontalPos && PageValue == 0 && SrcInputType < Input_Digital)
        // 3-12
        {
        	//110527 Modify
            if (SrcInputType != Input_Digital)
            {
                UserPrefHStart = (WORD)(100-WordValue)*(MaxHStart-MinHStart)/100 + MinHStart;
                mStar_AdjustHPosition(UserPrefHStart);
            }
            TPValue = 0x00;
        }
        else if (CPCode == ADJ_VerticalPos && PageValue == 0 && SrcInputType < Input_Digital)
        // 3-13
        {
        #if 0
            if (SrcInputType != Input_Digital)
            {
                UserPrefVStart = WordValue + MinVStart;
                mStar_AdjustVPosition(UserPrefVStart);
            }
            ValueH = (MaxVStart - MinVStart) >> 8;
            ValueL = (MaxVStart - MinVStart) &0x00FF;
            TPValue = 0x00;
	#else
			if (SrcInputType != Input_Digital)		//110527 Modify
			{
	            UserPrefVStart = (WORD)WordValue*(MaxVStart-MinVStart)/100 + MinVStart;
	            mStar_AdjustVPosition(UserPrefVStart);
			}
            TPValue = 0x00;
	#endif
        }
        else if (CPCode == ADJ_Phase && PageValue == 0 && SrcInputType < Input_Digital)
        // 3-14
        {
            if (SrcInputType != Input_Digital)
            {
				#ifdef TSUMXXT//#if( ChipID == ChipTSUMT )		//110311 Modify
				UserPrefPhase = WordValue*128/100;
				if(UserPrefPhase>127)
					UserPrefPhase=127;
				msADC_SetPhaseCode(UserPrefPhase);
				#else
				UserPrefPhase = (WORD)WordValue *63 / 100+0.5;
				if (UserPrefPhase > 63)
					UserPrefPhase = 63;
				mStar_AdjustPhase(UserPrefPhase);
				#endif
            }
            TPValue = 0x00;
        }
        else if (CPCode == DDCCI_Settings && PageValue == 0 )
        // 3-14
        {
            if ((SrcInputType != Input_Digital)&&(SrcInputType != Input_Digital2))//yuanfa
            {
                #if USEFLASH
                UserPref_EnableFlashSaveBit(bFlashSaveModeBit);
                #else
                SaveModeSetting();
                #endif
            }

            #if USEFLASH
            UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit);
            #else
            SaveMonitorSetting();
            #endif
            
            TPValue = 0x00;
        }
        else if (CPCode == Select_InputSource && PageValue == 0)
        {
            // Osd_Hide();
            if (RetValueL == 0x01 || RetValueL == 0x02)
            {
                #if Dual
                    ChangeSourceToAnalog();
                #endif
            }
            if (RetValueL == 0x03 || RetValueL == 0x04)
            {
                #if Dual
                    ChangeSourceToDigital();
                #endif
            }
            #if  INPUT_TYPE==INPUT_1A1H	|| INPUT_TYPE == INPUT_1A2D
            if (RetValueL == 0x11 || RetValueL == 0x12)
            {
                SetInputToHDMI();
            }
            #endif
            /*
            if(SrcInputType != Input_Digital)
            ValueL = 0x01;
            else
            ValueL = 0x03;*/
            TPValue = 0x00;
        }
        #if AudioFunc
            else if (CPCode == ADJ_SpeakerVolume && PageValue == 0)
            {
                //Osd_Hide();
                UserPrefVolume = RetValueL;
                mStar_AdjustVolume(UserPrefVolume);
                TPValue = 0x00;
            }
        #endif
        else if ((CPCode == ADJ_Red_Blacklevel || CPCode == ADJ_Green_Blacklevel || CPCode == ADJ_Blue_Blacklevel) && PageValue == 0)
        {
            if (CPCode == ADJ_Red_Blacklevel)
                UserprefRedBlackLevel = RetValueL;
            else if (CPCode == ADJ_Green_Blacklevel)
                UserprefGreenBlackLevel = RetValueL;
            else if (CPCode == ADJ_Blue_Blacklevel)
                UserprefBlueBlackLevel = RetValueL;
            mStar_AdjustUserPrefBlacklevel(UserprefRedBlackLevel, UserprefGreenBlackLevel, UserprefBlueBlackLevel);
            //SaveMonitorSetting();
        }
        /*
        else if(CPCode == ADJ_Sharpness&& PageValue == 0)  // 3-22
        {UserPrefSharpness= RetValueL+MinSharpnessValue;
        mStar_SetSharpnessCoefficient(UserPrefModeSharpness);
        ValueL = MaxSharpnessValue-MinSharpnessValue;
        TPValue = 0x00;
        }*/
        else if (CPCode == ADJ_Language && PageValue == 0)
        // 3-17
        {
            //     UserPrefLanguage = RetValueL-1;
            #if LANGUAGE_TYPE==ID2007Standard
                if (RetValueL == 2)
                    UserPrefLanguage = LANG_English;
                else if (RetValueL == 10)
                    UserPrefLanguage = LANG_Spanish;
                else if (RetValueL == 3)
                    UserPrefLanguage = LANG_France;
                else if (RetValueL == 4)
                    UserPrefLanguage = LANG_German;
                else if (RetValueL == 5)
                    UserPrefLanguage = LANG_Italian;
                else if (RetValueL == 8)
                    UserPrefLanguage = LANG_Portugues;
                else if (RetValueL == 9)
                    UserPrefLanguage = LANG_Russia;
                else if (RetValueL == 1)
                    UserPrefLanguage = LANG_SChina;
                ValueL = LANG_Nums;
                TPValue = 0x00;
            #endif
        }
        else if (CPCode == SyncType && PageValue == 0)
        // 2-19
        {
            ValueL = 3;
            RetValueL = 0;
        }
        else if( CPCode == PowerMode && PageValue == 0 )	//120301 Modif
        {
            if( RetValueL == 0x01 )
            {
            	if(!PowerOnFlag)
            	{
					PowerOnSystem();
					#if USEFLASH
					UserPref_EnableFlashSaveBit( bFlashSaveMonitorBit );
					#else
					NVRam_WriteByte( nvrMonitorAddr( MonitorFlag ), MonitorFlags );
					#endif
            	}
            }
			else if( RetValueL == 0x04 )
			{
            	if(PowerOnFlag)
        		{
					PowerOffSystem();
					#if USEFLASH
					UserPref_EnableFlashSaveBit( bFlashSaveMonitorBit );
					#else
					NVRam_WriteByte( nvrMonitorAddr( MonitorFlag ), MonitorFlags );
					#endif
            	}
			}
            TPValue = 0x00;
        }
         
/***********************************************Zhifeng.Wu***********************************************/
	#define	I_Menu_Command	    SIORxBuffer[1]
	#define I_Menu_Set_Value_H  SIORxBuffer[2]
	#define	I_Menu_Set_Value	SIORxBuffer[3]
         //For TPV Application
	else if (CPCode == TPV_App_Command&& PageValue == 0)
	{
		if(SIORxBuffer[0]==IMenu_Application)//For AOC I-Menu Application
		{
			//For Other Luminance Menu
			if((I_Menu_Command==ECO_Mode)&&(UserPrefColorTemp!=CTEMP_SRGB)&&(UserPrefDcrMode != 1))
			{
				UserPrefECOMode=I_Menu_Set_Value;
				SetECO();						 
			}
			else if((I_Menu_Command==Gamma)&&(UserPrefColorTemp!=CTEMP_SRGB)&&(UserPrefDcrMode != 1))
			{
				UserPrefGamaMode=I_Menu_Set_Value;
				mStar_InitGamma();						 
			}
			else if(I_Menu_Command==DCR)
			{
				if (UserPrefColorTemp == CTEMP_SRGB)
					UserPrefColorTemp = CTEMP_Warm1;
				if(UserPrefDcrMode!=I_Menu_Set_Value)//duqq080806 test
					AdjustDcrMode(1);						 
			}
			//For All Color Boost Menu Items
			else if((I_Menu_Command&0xF0)==0x20)
			{
                         RetValueL =((UserPrefColorFlags&(1<<((I_Menu_Command&0x0F)-1))) >0)?1:0;
			    //RetValueL =Get Current Status Of Color Boost Item
				if(RetValueL!=I_Menu_Set_Value)
				{
					if(I_Menu_Set_Value)//Current Status if Off,It Will Be Set "On"
					{
						UserPrefColorFlags&=~(1<<((I_Menu_Command&0x0F)-1));
					}
					else//Off
					{
						UserPrefColorFlags|=(1<<((I_Menu_Command&0x0F)-1));
					}
					if(I_Menu_Command==Full_Enhance)
					{
						AdjustColorBoostMode(1);						 
					}
					else if(I_Menu_Command==Nature_Skin)
					{
						AdjustSkinProtectMode(1);						 
					}
					else if(I_Menu_Command==Sky_Blue)
					{
						AdjustBlueEnhanceMode(1);						 
					}
					else if(I_Menu_Command==Green_Field)
					{
						AdjustGreenEnhanceMode(1);						 
					}
					else if(I_Menu_Command==Auto_Detect)
					{
						AdjustAutoDetectMode(1);						 
					}
					else if(I_Menu_Command==Demo)
					{
						AdjustDemoMode(1);						 
					}
				}
			}
			//For Picture Boost Items
			else if(I_Menu_Command==FrameSize)
			{
				#if 0
				RetValueL=(I_Menu_Set_Value>GetBFSizeValue())?  MIA_IncValue : MIA_DecValue;
				if(PictureBoostFlag)
					AdjustBFSize(RetValueL);
				#else
					UserPrefBFSize=((WORD)I_Menu_Set_Value*7+5)/100;
					#if ENABLE_HDMI
					AdjustBrightFrameSize(UserPrefBFSize);	//
					#else
					SetBFSize(UserPrefBFSize);//SetBFSize();
					#endif
				#endif
			}
			else if(I_Menu_Command==PB_Brightness)
			{
				if(PictureBoostFlag)
				{
					UserPrefSubBrightness=I_Menu_Set_Value*2+MinSubBrightness;//UserPrefSubBrightness=I_Menu_Set_Value*2.55;
					msAdjustSubBrightness(UserPrefSubBrightness);				
				}
			}
			else if(I_Menu_Command==PB_Contrast)
			{
				if(PictureBoostFlag)
				{
					UserPrefSubContrast=I_Menu_Set_Value*2+MinSubContrast;//UserPrefSubContrast=I_Menu_Set_Value*2.55;
					msAdjustVideoContrast(UserPrefSubContrast);	
				}
			}
			else if(I_Menu_Command==PB_Hpos)
			{
				#if 0
				RetValueL=(I_Menu_Set_Value>GetBFHstartValue())?  MIA_IncValue : MIA_DecValue;
				if(PictureBoostFlag)
					AdjustBFHstart(RetValueL);		
				#else
		            if (UserPrefBFSize == 1)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*7+5)/100;
		            }
		            else if (UserPrefBFSize == 2)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*4+5)/100;
		            }
		            else if (UserPrefBFSize == 3)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*3+5)/100;
		            }
		            else if (UserPrefBFSize == 4)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*2+5)/100;
		            }
		            else if (UserPrefBFSize == 5 || UserPrefBFSize == 6)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*1+5)/100;
		            }
				if(PictureBoostFlag)
				{
			            UserPrefBFHStart = RetValueL;
						WordValue = UserPrefBFHStart * UserPrefBFHSize + HSTART_OFFSET;	//120112 Modify
			            msAccSetup(WordValue, (UserPrefBFHSize + WordValue), UserPrefBFVStart *UserPrefBFVSize, (UserPrefBFVStart *UserPrefBFVSize + UserPrefBFVSize));
				}
				#endif
			}
			else if(I_Menu_Command==PB_Vpos)
			{
				#if 0
				RetValueL=(I_Menu_Set_Value>GetBFVstartValue())?  MIA_IncValue : MIA_DecValue;
				if(PictureBoostFlag)
					AdjustBFVstart(RetValueL);						 
				#else
		            if (UserPrefBFSize == 1)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*7+5)/100;
		            }
		            else if (UserPrefBFSize == 2)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*4+5)/100;
		            }
		            else if (UserPrefBFSize == 3)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*3+5)/100;
		            }
		            else if (UserPrefBFSize == 4)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*2+5)/100;
		            }
		            else if (UserPrefBFSize == 5 || UserPrefBFSize == 6)
		            {
		                RetValueL = ((WORD)I_Menu_Set_Value*1+5)/100;
		            }
				if(PictureBoostFlag)
				{
			            UserPrefBFVStart = RetValueL;
			            WordValue = UserPrefBFVStart * UserPrefBFVSize;
			            msAccSetup(UserPrefBFHStart *UserPrefBFHSize + HSTART_OFFSET, UserPrefBFHStart *UserPrefBFHSize + UserPrefBFHSize+ HSTART_OFFSET, WordValue, (WordValue + UserPrefBFVSize));
				}
				#endif
			}
			else if(I_Menu_Command==PB_BrightFrame)
			{
                         RetValueL =(PictureBoostFlag >0)?1:0;
			    //RetValueL =Get Current Status Of Color Boost Item
				if(RetValueL!=I_Menu_Set_Value)
				{
					if(I_Menu_Set_Value)//Current Status if Off,It Will Be Set "On"
					{
						Clr_PictureBoostFlag();
					}
					else//Off
					{
						Set_PictureBoostFlag();
					}
					AdjustBF_Switch(1);	
				}
			}
		#if ESaverPowerDownFunc
			else if(I_Menu_Command==PB_AutoPowerDown)
			{
			    ESaverPowerDownCounter=0;
				if((!I_Menu_Set_Value_H)&&(!I_Menu_Set_Value))
				{
                    ESaverPowerDownCounter=0;
				}
				else
				{   
					ESaverPowerDownCounter=((WORD)I_Menu_Set_Value_H << 8) | ((WORD)I_Menu_Set_Value);
				}
				//Osd_Show();
				//Osd_Draw4Num(1,1,ESaverPowerDownCounter);
			}
		#endif
		}
		
	}
	#undef	I_Menu_Command
	#undef	I_Menu_Set_Value_H
	#undef	I_Menu_Set_Value
/*******************************************************************************************************/
        else if (CPCode == 0xFE && PageValue == 0)
        {
        	#ifdef UseVGACableReadWriteAllPortsEDID
			if(DDCBuffer[3]==0xEF)
			{
				BYTE i,Length;
				WORD Addr;

				Addr=(DDCBuffer[5]<<8)+DDCBuffer[6];
				Length=DDCBuffer[0]-0x86;

				for(i=0;i<Length;i++)
				{
					SIORxBuffer[i] = DDCBuffer[i+7];
				}
				if((DDCBuffer[4]>=VGA1&&DDCBuffer[4] <=DP2)||(DDCBuffer[4]>=EEPROM&&DDCBuffer[4] <=PanelSettingDevice))
					WriteDevice(DDCBuffer[4] ,Addr,Length);//Write EDID or HDCP KEY
				else
				{
					return 0;
				}
			}
			else if((DDCBuffer[3]==0xE1)&&(DDCBuffer[4]==0xA2)&&(DDCBuffer[5]==0x01))
			{
				if(DDCBuffer[6]==0x01)
				{
					hw_ClrDDC_WP();//EDID In Flash should not disable ddc ram.
				#ifdef AOC37Series  //100412  edit by lizzie. for power saving can't receive DDC COMMAND completely
				            #if MainBoardType==MainBoard_715G3598_M0A
						// DC off / Power Saving, system runs at 14.318MHz for power consumption
						mStar_WriteByte ( REGBK, REGBANKADC );	  
						msWriteByteMask(BK1_68, BIT6	   ,  BIT6);//B.1.8 Scar, 20090424	 /*JT modify it from BK1_67[7] change to BK1_68[6]*/
						msWriteByteMask(BK1_66, BIT0	   ,  BIT0);//B.14.4 Scar,20090424
						ForceDelay1ms ( 1 );		
						//DoubleSpeed
						msWriteByteMask(BK1_67, BIT4		,  BIT5|BIT4);
						ForceDelay1ms ( 1 );
						msWriteByteMask(BK1_66, BIT3|BIT2, BIT3|BIT2|BIT1);
						ForceDelay1ms ( 1 );
						msWriteByteMask(BK1_66, 0			  ,  BIT0);
						mStar_WriteByte ( REGBK, REGBANKSCALER);	
						#endif
				#endif
					Set_WriteDeviceFlag();
					#if 0//def ReduceDDC
						FlashErase(ANALOG_DDC_START);
						#if Dual
						FlashErase(DIGITAL_DDC_START);
						#endif
					#endif
				}
				else if(DDCBuffer[6]==0x00)
				{
					
					#ifdef AOC37Series//100412 
					       #if MainBoardType==MainBoard_715G3598_M0A
						// DC off / Power Saving, system runs at 14.318MHz for power consumption
						mStar_WriteByte(0x00, 0x01);
						mStar_WriteByte(BK1_66, mStar_ReadByte(BK1_66)|BIT0);
						mStar_WriteByte(0x00, 0x00);
						#endif
					#endif
					#ifdef ReduceDDC
					#if !Enable_External_EDID
						mStar_SetDDC_WP();
					#endif
						Set_LoadEDIDFlag();//Load EDID to DDC RAM From Flash
					#else
						hw_SetDDC_WP();
					#endif
					Clr_WriteDeviceFlag();
				}
			}
			else
			#endif
        
        	{
        	#if NewFactoryCommand
        	   BYTE tempValue;
		#endif
                 
        #ifdef DATA_LENGTH_20_BYTE
            BYTE i;
            for(i=0;i<23;i++)
                SIORxBuffer[i] = DDCBuffer[i+3];
        #else
            SIORxBuffer[0] = DDCBuffer[3];
            SIORxBuffer[1] = DDCBuffer[4];
            SIORxBuffer[2] = DDCBuffer[5];
            SIORxBuffer[3] = DDCBuffer[6];
            SIORxBuffer[4] = DDCBuffer[7];
            SIORxBuffer[5] = DDCBuffer[8];
            SIORxBuffer[6] = DDCBuffer[9];
            SIORxBuffer[7] = DDCBuffer[10];
        #endif

				#if NewFactoryCommand
//==========================NewFactoryCommand====================
			#define TPVCMD1    DDCBuffer[3]
			#define TPVCMD2    DDCBuffer[4]
			#define TPV_ValueH DDCBuffer[5]
			#define TPV_ValueL DDCBuffer[6]
			
			if(TPVCMD1==TPV_FactoryAutoAlignment)
			{
				if(TPVCMD2==SetFactoryMode)
				{
					if(TPV_ValueL==SetOn)
					{
						Set_FactoryModeFlag();	
						if(UserPrefECOMode != ECO_Standard)
						{
							UserPrefECOMode = ECO_Standard;
							SetECO();
						}
						if( UserPrefDcrMode == 1 )
						{
							AdjustDcrMode(0);
							ReadColorTempSetting();
						}
						#if MWEFunction
						if(UserPrefColorFlags)
						{
							UserPrefColorFlags = 0;
							UserPrefHue = 50;
							UserPrefSaturation = 0x80;
							UserPrefSubContrast = DefSubContrast;
							UserPrefSubBrightness = DefSubBrightness;
							UserPrefBFSize = 1;
							msAdjustSubBrightness(UserPrefSubBrightness);
							msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
							msAdjustVideoHue(UserPrefHue);
							msAdjustVideoSaturation(UserPrefSaturation);
							SetBFSize(UserPrefBFSize);//SetBFSize();
						}
						#endif
						if(UserPrefGamaMode != GAMA1)
						{
							UserPrefGamaMode = GAMA1;
							mStar_InitGamma();
						}
						UserPrefBrightness = DefBrightness;
						UserPrefContrast = DefContrast;
						UserPrefBrightnessWarm1 = DefBrightness;
						UserPrefContrastWarm1 = DefContrast;
						UserPrefBrightnessNormal = DefBrightness;
						UserPrefContrastNormal = DefContrast;
						UserPrefBrightnessCool1 = DefBrightness;
						UserPrefContrastCool1 =DefContrast;
						UserPrefBrightnessUser = DefBrightness;
						UserPrefContrastUser = DefContrast;
						mStar_AdjustBrightness(UserPrefBrightness);
						mStar_AdjustContrast(UserPrefContrast);
						DDCBuffer[1] = 0x02;	
					}
					else if(TPV_ValueL==SetOff)
					{
						Clr_FactoryModeFlag();
						DDCBuffer[1] = 0x04;	
					}
				}
				else if(TPVCMD2==NVMInit)
				{
				}
				else if(TPVCMD2==SetBurnMode)
				{
					if(TPV_ValueL==SetOn)
					{
						Set_BurninModeFlag();	
                        Set_DoBurninModeFlag();
					}
					else if(TPV_ValueL==SetOff)
					{
						Clr_BurninModeFlag();
                        Clr_DoBurninModeFlag();
					}
					else if(TPV_ValueL==2)//burnin room checklater
						;
				#if !USEFLASH
                    NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
				#else
                    SaveMonitorSetting();
				#endif
                      		#if USEFLASH
					        Flash_ReadMonitorSet((BYTE*)&MonitorSetting, MonitorSettingSize);
				          if(MonitorSetting.MonitorFlag==MonitorFlags)
                      		     {
                      		     	DDCBuffer[1]=0x02;
                      		     	}
                      		    else
                      		     {
                      		     	DDCBuffer[1]=0x04;
                      		     	}	
                                         
                                  #else
                                        NVRam_ReadByte(nvrMonitorAddr(MonitorFlag), &tempValue);
                      		     if(tempValue== MonitorFlags)
                      		     	{
                      		     	DDCBuffer[1]=0x02;
                      		     	}
                      		    else
                      		     {
                      		     	DDCBuffer[1]=0x04;
                      		     	}
                      	#endif
				
				}
				else if(TPVCMD2==ClearBacklitTime)
				{
            				#if USEFLASH
                                        BlacklitTime = 0;
                                        MonitorBackLightTime= 0;
                                        SaveMonitorSetting2();
								Flash_ReadMonitorSet((BYTE*)&MonitorSetting, MonitorSettingSize);
                      		    if(MonitorSetting2.Backlighttime==0)
                      		     {
                      		     	DDCBuffer[1]=0x02;
                      		     	}
                      		    else
                      		     {
                      		     	DDCBuffer[1]=0x04;
                      		     	}	
                    			#else
                                        BlacklitTime = 0;
                                        SaveBlacklitTime();
            				
                                        NVRam_ReadTbl(BlacklitTimeAddr, (BYTE*)(&tempValue), 4);
            				   if(tempValue==0)
            				      {
                        		     	DDCBuffer[1]=0x02;
                        		     	}
                        		   else
                        		     {
                        		     	DDCBuffer[1]=0x04;
                        		     	}
            				#endif
				
					
				}		
				else if(TPVCMD2==SetProduceMode)
				{
        				#if !USEFLASH
        				WORD temp,temp1;
        				#endif
     					if(TPV_ValueL==SetOn)
     					{
						Set_ProductModeFlag();
						#if AudioFunc
						UserPrefVolume=90;//PE Request petit 20130102
						#endif
						#if USEFLASH
						FactoryProductModeValue=ProductModeOnValue;
						SaveFactorySetting();
						SaveMonitorSetting();
						#else
						{
						temp=ProductModeOnValue;
						NVRam_WriteTbl(ProductModeAddr, (BYTE*)(&temp), 2);
						NVRam_WriteByte(nvrMonitorAddr(Volume), UserPrefVolume);
						}
						#endif
     					}
 					else if(TPV_ValueL==SetOff) 
 					{
 						 Clr_ProductModeFlag();
                                   	#if USEFLASH
                                            FactoryProductModeValue=ProductModeOffValue;
                                            SaveFactorySetting();
                   		              #else
                   			   	{
                                           temp=ProductModeOffValue;
                                            NVRam_WriteTbl(ProductModeAddr, (BYTE*)(&temp), 2);
 			   			}
 					        #endif
 					}
                     		    #if USEFLASH
								Flash_ReadFactorySet((BYTE*)&FactorySetting, FactorySettingSize);
                     		    if(FactorySetting.ProductModeValue==FactoryProductModeValue)
                     		     {
                     		     	DDCBuffer[1]=0x02;
                     		     	}
                     		    else
                     		     {
                     		     	DDCBuffer[1]=0x04;
                     		     	}
								if(ProductModeFlag)
							   		ForceDelay1ms(50);
                                       #else
					     NVRam_ReadTbl(ProductModeAddr, (BYTE*)(&temp1), 2);
                     		     if(temp1==temp)
                     		     	{
                     		     	DDCBuffer[1]=0x02;
                     		     	}
                     		    else
                     		     {
                     		     	DDCBuffer[1]=0x04;
                     		     	}
                              if(ProductModeFlag)
							   ForceDelay1ms(100);  
                     	       #endif
							   

				}
				else if(TPVCMD2==LogoONOFF)//  2011.8.3   
				   {
				     if(TPV_ValueH==0x06)
				     	{
				     	  if(TPV_ValueL==0x01)
						{
						  LogoON=1;
						 	#if USEFLASH
                                           	        SaveFactorySetting();
							#else
			   		 		NVRam_WriteByte(nvrMonitorAddr(LogoON), LogoON);
			   				#endif

				     	  	}
					  else
					  	{
						  LogoON=0;
						  	#if USEFLASH
                                           	        SaveFactorySetting();
							#else
			   		 		NVRam_WriteByte(nvrMonitorAddr(LogoON), LogoON);
			   				#endif

				     	  	}
					 
				     	}
                        else if( TPV_ValueH == 0x03 )
    					{
    						if( TPV_ValueL == SetOn )
    						{
    							if(!UserPrefDcrMode)
    							{
    								AdjustDcrMode(1);
    								#if !USEFLASH
    								NVRam_WriteByte( nvrMonitorAddr( DcrMode ), UserPrefDcrMode );
    								#else
    								UserPref_EnableFlashSaveBit( bFlashSaveMonitorBit );
    								#endif
    							}
    						}
    						else if( TPV_ValueL == SetOff )
    						{
    							if(UserPrefDcrMode)
    							{
    								AdjustDcrMode(1);
    								#if !USEFLASH
    								NVRam_WriteByte( nvrMonitorAddr( DcrMode ), UserPrefDcrMode );
    								#else
    								UserPref_EnableFlashSaveBit( bFlashSaveMonitorBit );
    								#endif
    							}
    						}
    					}
				    }
		
				#if PANEL_VCOM_ADJUST		//121031 Modify
					else if(TPVCMD2==AdjPanelVCOM)
					{
						#if VCOM_TYPE == VCOM_MAXIM
						WordValue = (( TPV_ValueH << 8 ) | TPV_ValueL );
						WordValue = WordValue + VCOM_VALUE_OFFSET;
						if( WordValue >= MAX_VCOM_VALUE )
							FactorySetting.VcomValue = MAX_VCOM_VALUE;
						else
							FactorySetting.VcomValue = WordValue & 0x3FF;
						#else
						if( TPV_ValueL >= 0x7F )
							FactorySetting.VcomValue = 0x7F;
						else
							FactorySetting.VcomValue = TPV_ValueL;
						#endif
						msWriteMiniLVDSi2c_Vcom_DAC(FactorySetting.VcomValue);
						#if USEFLASH
								SaveFactorySetting();
						#else
						#if VCOM_TYPE == VCOM_MAXIM
								NVRam_WriteWord( nvrMonitorAddr( VcomValue ), FactorySetting.VcomValue );
						#else
								NVRam_WriteByte( nvrMonitorAddr( VcomValue ), FactorySetting.VcomValue );
						#endif
						#endif
					}
				#endif
				#if HDCPKEY_IN_Flash
				else if(TPVCMD2==HDCPReadWriteStart)
				{
					Set_WriteHDCPcodeFlag();

					TCRCvalue = 0;
					hw_ClrAmberLed();
					hw_ClrGreenLed();
					//Flash_Erase_FreeBuffer(FLASH_KEY_HDCP,0); // jeff add it in 2009 0907
					bFlashWriteFactory = TRUE;
					
			            #if 0 // Enable_Cache
                                       MCU_DisableCache();
                                 #endif
			
					FlashSectorErase(TRUE, HDCPKEYSET_START);
					FlashDisableWP(HDCPKEYSET_START);
				}
				#endif
			}	
			
			else if(TPVCMD1==TPV_AdjRedGain)
			{
			    UserPrefRedColor = TPV_ValueL; // for CT
                mStar_AdjustRedColor(UserPrefRedColor, UserPrefContrast);
			}
			else if(TPVCMD1==TPV_AdjGreenGain)
			{
			    UserPrefGreenColor = TPV_ValueL; // for CT
                mStar_AdjustGreenColor(UserPrefGreenColor, UserPrefContrast);
			
			}
			else if(TPVCMD1==TPV_AdjBlueGain)
			{
			    UserPrefBlueColor = TPV_ValueL; // for CT
                mStar_AdjustBlueColor(UserPrefBlueColor, UserPrefContrast);
			}
			else if(TPVCMD1==TPV_AdjContrast)	
			{
                UserPrefContrast = TPV_ValueL;
                UserPrefContrast = UserPrefContrast + MinContrastValue;//checklater
                mStar_AdjustContrast(UserPrefContrast);				
			
			}
			else if(TPVCMD1==TPV_AdjBrightness)	
			{
                UserPrefBrightness = TPV_ValueL;
                mStar_AdjustBrightness(UserPrefBrightness);			
			}
			else if(TPVCMD1==TPV_SetDefLanguage)	
			{
			if(!PowerOnFlag)
			{
				MenuPageIndex = PowerOnMenu;
				PowerOnSystem();
			}
			else
			{
		#if F_ResetCheckEdidManueAndSN
			if(FactoryCheckEdidManufactureAndSN())
		#endif
			{
                #if LANGUAGE_TYPE == ASIA
                    if (TPV_ValueL == LanFrenchCode)
                        UserPrefLanguage = LANG_France;
					
                    else if (TPV_ValueL == LanTChineseCode)
                        UserPrefLanguage = LANG_TChina;
					
                    else if (TPV_ValueL == LanKoreanCode)
                        UserPrefLanguage = LANG_Korea;
					
                    else if (TPV_ValueL == LanSpanishCode)
                        UserPrefLanguage = LANG_Spanish;
					
                    else if (TPV_ValueL == LanPortugueseCode_Brazil)
                        UserPrefLanguage = LANG_Portugues;
					
                    else if (TPV_ValueL == LanRussianCode)
                        UserPrefLanguage = LANG_Russia;
					
                    else if (TPV_ValueL == LanSChineseCode)
                        UserPrefLanguage = LANG_SChina;
					
                    else if (TPV_ValueL ==LanEnglistCode)
                    // default
                        UserPrefLanguage = LANG_English;
		 //For Select SChinese/TChinese/Korean Language Font Error Issue
		        /*if (UserPrefLanguage == LANG_TChina)
		        {
		            LanguageIndex = 1;
		            SecondTblAddr = 0x80;
		        }
		        else if (UserPrefLanguage == LANG_Korea)
		        {
		            LanguageIndex = 2;
		            SecondTblAddr = 0x80; //SecondTblAddr = 0xFF;
		        }
		        else
		        {
		            LanguageIndex = 0;
		            SecondTblAddr = 0xFF;
		        }
                */
                      RunLanguage();
                #elif LANGUAGE_TYPE == EUROPE
                    if (TPV_ValueL ==LanSpanishCode)
                        UserPrefLanguage = LANG_Spanish;
					
                    else if (TPV_ValueL == LanFrenchCode)
                        UserPrefLanguage = LANG_France;
					
                    else if (TPV_ValueL == LanGermanCode)
                        UserPrefLanguage = LANG_German;
					
                    else if (TPV_ValueL == LanItalianCode)
                        UserPrefLanguage = LANG_Italian;
					
                    else if (TPV_ValueL == LanRussianCode)
                        UserPrefLanguage = LANG_Russia;
					
                    else if (TPV_ValueL == LanSwedishCode)
                        UserPrefLanguage = LANG_Swedish;
					
                    else if (TPV_ValueL == LanPolishCode)
                        UserPrefLanguage = LANG_Polish;
					
                    else if (TPV_ValueL == LanCzechCode)
                        UserPrefLanguage = LANG_Czech;
					
                    else if (TPV_ValueL ==LanEnglistCode)
                    // default
                        UserPrefLanguage = LANG_English;
					
		   #elif LANGUAGE_TYPE == All13Language
		      if (TPV_ValueL == LanSpanishCode)
                        UserPrefLanguage = LANG_Spanish;
					
                    else if (TPV_ValueL == LanFrenchCode)
                        UserPrefLanguage = LANG_France;
					
                    else if (TPV_ValueL == LanGermanCode)
                        UserPrefLanguage = LANG_German;
					
                    else if (TPV_ValueL == LanItalianCode)
                        UserPrefLanguage = LANG_Italian;
					
                    else if (TPV_ValueL == LanRussianCode)
                        UserPrefLanguage = LANG_Russia;
					
                    else if (TPV_ValueL == LanSwedishCode)
                        UserPrefLanguage = LANG_Swedish;
					
                    else if (TPV_ValueL == LanPolishCode)
                        UserPrefLanguage = LANG_Polish;
					
                    else if (TPV_ValueL == LanCzechCode)
                        UserPrefLanguage = LANG_Czech;
						  
		      else if (TPV_ValueL == LanPortugueseCode_Brazil)
                        UserPrefLanguage = LANG_Portugues;
			  
                    else if (TPV_ValueL == LanSChineseCode)
                        UserPrefLanguage = LANG_SChina;
					
                    else if (TPV_ValueL == LanTChineseCode)
                        UserPrefLanguage = LANG_TChina;
					
                    else if (TPV_ValueL == LanKoreanCode)
                        UserPrefLanguage = LANG_Korea;
					
                    else if (TPV_ValueL ==LanEnglistCode)
                    // default
                        UserPrefLanguage = LANG_English; 
			//For Select SChinese/TChinese/Korean Language Font Error Issue
                   /* if (UserPrefLanguage == LANG_SChina || UserPrefLanguage == LANG_English)
			        {
			            LanguageIndex = 0;
			            SecondTblAddr = 0xFF;
			        }
			        else if (UserPrefLanguage == LANG_TChina)
			        {
			            LanguageIndex = 1;
			            SecondTblAddr = 0x80;
			        }
			        else if (UserPrefLanguage == LANG_Korea)
			        {
			            LanguageIndex = 2;
			            SecondTblAddr = 0x7A;
			        }
					else
			        {
			            LanguageIndex = 3;
			            SecondTblAddr = 0x80;
			        }*/
			        RunLanguage();
			#elif LANGUAGE_TYPE == All14Language_AddJap
		      if (TPV_ValueL == LanSpanishCode)
                        UserPrefLanguage = LANG_Spanish;
					
                    else if (TPV_ValueL == LanFrenchCode)
                        UserPrefLanguage = LANG_France;
					
                    else if (TPV_ValueL == LanGermanCode)
                        UserPrefLanguage = LANG_German;
					
                    else if (TPV_ValueL == LanItalianCode)
                        UserPrefLanguage = LANG_Italian;
					
                    else if (TPV_ValueL == LanRussianCode)
                        UserPrefLanguage = LANG_Russia;
					
                    else if (TPV_ValueL == LanSwedishCode)
                        UserPrefLanguage = LANG_Swedish;
					
                    else if (TPV_ValueL == LanPolishCode)
                        UserPrefLanguage = LANG_Polish;
					
                    else if (TPV_ValueL == LanCzechCode)
                        UserPrefLanguage = LANG_Czech;
						  
		      else if (TPV_ValueL == LanPortugueseCode_Brazil)
                        UserPrefLanguage = LANG_Portugues;
			  
                    else if (TPV_ValueL == LanSChineseCode)
                        UserPrefLanguage = LANG_SChina;
					
                    else if (TPV_ValueL == LanTChineseCode)
                        UserPrefLanguage = LANG_TChina;
					
                    else if (TPV_ValueL == LanKoreanCode)
                        UserPrefLanguage = LANG_Korea;
					
                    else if (TPV_ValueL == LanJapaneseCode)
                        UserPrefLanguage = LANG_Japan;
								
                    else if (TPV_ValueL ==LanEnglistCode)
                    // default
                        UserPrefLanguage = LANG_English; 
			//For Select SChinese/TChinese/Korean Language Font Error Issue
                    /*if (UserPrefLanguage == LANG_SChina || UserPrefLanguage == LANG_English)
			        {
			            LanguageIndex = 0;
			            SecondTblAddr = 0xFF;
			        }
			        else if (UserPrefLanguage == LANG_TChina)
			        {
			            LanguageIndex = 1;
			            SecondTblAddr = 0x80;
			        }
			        else if (UserPrefLanguage == LANG_Korea)
			        {
			            LanguageIndex = 2;
			            SecondTblAddr = 0x7A;
			        }
				else if (UserPrefLanguage == LANG_Japan)
                               {
                                   LanguageIndex = 4;
                                   SecondTblAddr = 0x70;
                               }
					else
			        {
			            LanguageIndex = 3;
			            SecondTblAddr = 0x80;
			        }*/
			   RunLanguage();
                #endif
                #if !USEFLASH
                    NVRam_WriteByte(nvrMonitorAddr(Language), UserPrefLanguage);
                #else
                    SaveMonitorSetting();
                #endif
		 #if USEFLASH
		 			Flash_ReadMonitorSet((BYTE*)&MonitorSetting, MonitorSettingSize);
                     if(MonitorSetting.Language==UserPrefLanguage)
           		     {
           		     	DDCBuffer[1]=0x02;
           		     	}
           		    else
           		     {
           		     	DDCBuffer[1]=0x04;
           		     	}	
                  
              #else
                           NVRam_ReadByte(nvrMonitorAddr(Language), &tempValue);
       		     if(tempValue==UserPrefLanguage)
       		     	{
       		     	DDCBuffer[1]=0x02;
       		     	}
       		    else
       		     {
       		     	DDCBuffer[1]=0x04;
       		     	}
		    
	       #endif
		   	MenuPageIndex = PowerOffMenu;
			PowerOffSystem();
			} 
			}
			}
			else if(TPVCMD1==TPV_SelectInput)	
			{
			
			}
			else if(TPVCMD1==TPV_SelectGamma)	
			{
			
			}			
			else if(TPVCMD1==TPV_RestorFactory)	
			{
			if(!PowerOnFlag)
			{
				MenuPageIndex = PowerOnMenu;
				PowerOnSystem();
			}
			else
			{
		#if F_ResetCheckEdidManueAndSN
			if(FactoryCheckEdidManufactureAndSN())
		#endif
			{
					if(ProductModeFlag)
						{
              			   if( ResetAllSetting())
              			   	{
              			   	DDCBuffer[1]=0x02;
						hw_ClrGreenLed();
					       hw_SetAmberLed();
              			   	}
              			    else
              			      DDCBuffer[1]=0x04;
						}
					else
						{
					     if( ResetAllSetting())
              			   	DDCBuffer[1]=0x02;
					     else
              			      DDCBuffer[1]=0x04;
						}
			#if  BlacklitTimeOnShow
				//ForceDelay1ms(20);
             		 //////////////////////////////////////Reset->>poweroff
              	MonitorBlacklitTimeOn_S=BlacklitTimeOn_S=0;
	       	MonitorBlacklitTimeOn_M=BlacklitTimeOn_M=0;  
              	MonitorBlacklitTimeOn_H=BlacklitTimeOn_H=0;   
			#endif
			SaveMonitorSetting2();
			  MenuPageIndex = PowerOffMenu;
			  PowerOffSystem();
			}
			}
			//////////////////////////////////////
              	}			
			else if(TPVCMD1==TPV_AutoColor)
			{
              #if 1
                if (mStar_AutoColor())
				{
					if (CheckFactorySettingOutOfRange())
					{
						#if USEFLASH
                        Init_FactorySetting();
                        SaveFactorySetting();
					    #else
					    Init_MonitorSetting();
                        SaveMonitorSetting();
						#endif

						DDCBuffer[1]=0x04;
                        
                        Delay1ms(200);
					}
					else
					{
						DDCBuffer[1]=0x02;

        				#if USEFLASH
                        SaveFactorySetting();
        				#else
        				SaveMonitorSetting();
        				#endif
                        
                        Delay1ms(200);

                        MenuPageIndex = FactoryMenu;
                        MenuItemIndex = 0;
                        DrawOsdMenu();
                       Osd_SetPosition(0, 100);
					}					
				}	
				else
				{
					DDCBuffer[1]=0x04;
                    
                    #if USEFLASH
                    SaveFactorySetting();
    				#else
    				SaveMonitorSetting();
    				#endif
                    
                    Delay1ms(200);
				}
              #else
			
                if (mStar_AutoColor())
                {
                    if (CheckFactorySettingOutOfRange())
                    {
                        #if USEFLASH
                        Init_FactorySetting();
					    #else
					    Init_MonitorSetting();
						#endif
                        DDCBuffer[1]=0x04;
                    }
                    else
                        DDCBuffer[1]=0x02;
                }	
				else
                    DDCBuffer[1]=0x04;
				#if USEFLASH
                SaveFactorySetting();
				#else
				SaveMonitorSetting();
				#endif

                Delay1ms(200);
              #endif
			}
			else if(TPVCMD1==TPV_AutoConfig)
			{
				if (mStar_AutoGeomtry())
				{
				       CheckModeSettingRange();
                                 
					DDCBuffer[1]=0x02;
				}
				else
					DDCBuffer[1]=0x04;

				  SaveModeSetting();
			}
			else if(TPVCMD1==TPV_SaveOrRecallColTmp)	
			{
				//#define ColTemp		DDCBuffer[4]
				//#define InpSrc	  DDCBuffer[5]
				//#define ActFlag	  DDCBuffer[6]

				if(TPV_ValueL)
				{
					switch(TPVCMD2)
					{
						case ColorTempSRGB:
							
							UserPrefColorTemp = CTEMP_SRGB;
							UserPrefRedColor = UserPrefRedColorSRGB;
							UserPrefGreenColor = UserPrefGreenColorSRGB;
							UserPrefBlueColor = UserPrefBlueColorSRGB;
							ReadColorTempSetting(); // 2006/11/10 6:59PM by KK ReadSRGBSetting();
							mStar_AdjustBrightness(UserPrefBrightness);
							mStar_AdjustContrast(UserPrefContrast);


							break;
						case ColorTempNative:

							break;
						case ColorTemp4000K:

							break;
						case ColorTemp5000K:

							break;
						case ColorTemp6500K:
							
							UserPrefColorTemp = CTEMP_Warm1;
							UserPrefRedColor = UserPrefRedColorWarm1;
							UserPrefGreenColor = UserPrefGreenColorWarm1;
							UserPrefBlueColor = UserPrefBlueColorWarm1;
							ReadColorTempSetting(); // 2006/11/10 6:59PM by KK ReadWarm1Setting();
							mStar_AdjustBrightness(UserPrefBrightness);
							mStar_AdjustContrast(UserPrefContrast);


							break;
						case ColorTemp7500K:
							UserPrefColorTemp = CTEMP_Normal;
							UserPrefRedColor = UserPrefRedColorNormal;
							UserPrefGreenColor = UserPrefGreenColorNormal;
							UserPrefBlueColor = UserPrefBlueColorNormal;
							ReadColorTempSetting(); // 2006/11/10 6:59PM by KK ReadNormalSetting();
							mStar_AdjustBrightness(UserPrefBrightness);
							mStar_AdjustContrast(UserPrefContrast);

							break;
						case ColorTemp8200K:

							break;
						case ColorTemp9300K:
							
							UserPrefColorTemp = CTEMP_Cool1;
							UserPrefRedColor = UserPrefRedColorCool1;
							UserPrefGreenColor = UserPrefGreenColorCool1;
							UserPrefBlueColor = UserPrefBlueColorCool1;
							ReadColorTempSetting(); // 2006/11/10 6:59PM by KK	ReadCool1Setting();
							mStar_AdjustBrightness(UserPrefBrightness);
							mStar_AdjustContrast(UserPrefContrast);


							break;
						case ColorTemp10000K:

							break;
						case ColorTemp11500K:

							break;
						case ColorTempUser:

							break;
						case SaveAllColorTemp: 					
							#if USEFLASH
							SaveFactorySetting();
							#else
							SaveMonitorSetting();
							#endif						
							break;
						
						default:
							break;							
					}
				}
				else
				{
					switch(TPVCMD2)
					{
						case ColorTempSRGB:
							UserPrefColorTemp = CTEMP_SRGB;
							UserPrefRedColorSRGB = UserPrefRedColor;
							UserPrefGreenColorSRGB = UserPrefGreenColor;
							UserPrefBlueColorSRGB = UserPrefBlueColor;
							#if USEFLASH
							FUserPrefBrightnessSRGB = UserPrefBrightness;
							FUserPrefContrastSRGB = UserPrefContrast;
							#else
							UserPrefBrightnessSRGB = UserPrefBrightness;
							UserPrefContrastSRGB = UserPrefContrast;
							#endif
							break;
						case ColorTempNative:

							break;
						case ColorTemp4000K:

							break;
						case ColorTemp5000K:

							break;
						case ColorTemp6500K:
							UserPrefColorTemp = CTEMP_Warm1;
							UserPrefRedColorWarm1 = UserPrefRedColor;
							UserPrefGreenColorWarm1 = UserPrefGreenColor;
							UserPrefBlueColorWarm1 = UserPrefBlueColor;
							UserPrefBrightnessWarm1 = UserPrefBrightness;
							UserPrefContrastWarm1 = UserPrefContrast;
							FUserPrefBrightnessWarm1 = UserPrefBrightness;
							FUserPrefContrastWarm1 = UserPrefContrast;

							break;
						case ColorTemp7500K:
							UserPrefColorTemp = CTEMP_Normal;
							UserPrefRedColorNormal= UserPrefRedColor;
							UserPrefGreenColorNormal= UserPrefGreenColor;
							UserPrefBlueColorNormal= UserPrefBlueColor;
							UserPrefBrightnessNormal= UserPrefBrightness;
							UserPrefContrastNormal= UserPrefContrast;
							FUserPrefBrightnessNormal= UserPrefBrightness;
							FUserPrefContrastNormal= UserPrefContrast;

							break;
						case ColorTemp8200K:

							break;
						case ColorTemp9300K:
							
							
							UserPrefColorTemp = CTEMP_Cool1;
							UserPrefRedColorCool1 = UserPrefRedColor;
							UserPrefGreenColorCool1 = UserPrefGreenColor;
							UserPrefBlueColorCool1 = UserPrefBlueColor;
							UserPrefBrightnessCool1 = UserPrefBrightness;
							UserPrefContrastCool1 = UserPrefContrast;
							FUserPrefBrightnessCool1 = UserPrefBrightness;
							FUserPrefContrastCool1 = UserPrefContrast;

							//Osd_Draw4Num(0,1,UserPrefRedColor);
							//Osd_Draw4Num(0,2,UserPrefRedColorCool1);
							break;
						case ColorTemp10000K:

							break;
						case ColorTemp11500K:

							break;
						case ColorTempUser:

							break;
						case SaveAllColorTemp: 						
							#if USEFLASH
							SaveFactorySetting();
							#else
							SaveMonitorSetting();
							#endif												
							break;
						default:
							break;							
					}
					//Osd_Draw4Num(0,0,11);
					//Osd_Show();				
				#if USEFLASH
					SaveFactorySetting();
				#else
					SaveMonitorSetting();
				#endif	
				
				#if USEFLASH
      		      //Flash_ReadMonitorSet(0, (BYTE*)&MonitorSetting, MonitorSettingSize);	
      		      Flash_ReadMonitorSet((BYTE*)&MonitorSetting, MonitorSettingSize);
      			 tempValue=MonitorSetting.ColorTemp;
      			//Flash_ReadFactorySet(0, (BYTE*)&FactorySetting, FactorySettingSize);	
      			Flash_ReadFactorySet((BYTE*)&FactorySetting, FactorySettingSize);
				#else
				NVRam_ReadByte(nvrMonitorAddr(ColorTemp), &tempValue);
				NVRam_ReadTbl(MonitorSettingAddr, (BYTE*) &MonitorSetting, MonitorSettingSize);
		              //NVRam_ReadTbl(FactorySettingAddr, (BYTE*) &FactorySetting, FactorySettingSize);
		       	#endif
				switch(tempValue)
				{
						case  CTEMP_SRGB:
							#if USEFLASH
       						if( MonitorSetting.ColorTemp==UserPrefColorTemp&&
       							FactorySetting.RedColorsRGB== UserPrefRedColor&&
       							FactorySetting.GreenColorsRGB== UserPrefGreenColor&&
       							FactorySetting.BlueColorsRGB == UserPrefBlueColor&&
       							FactorySetting.FBrightnesssRGB == UserPrefBrightness&&
       							FactorySetting.FContrastsRGB == UserPrefContrast)
       						#else
							if( MonitorSetting.ColorTemp==UserPrefColorTemp&&
       							MonitorSetting.RedColorsRGB== UserPrefRedColor&&
       							MonitorSetting.GreenColorsRGB== UserPrefGreenColor&&
       							MonitorSetting.BlueColorsRGB == UserPrefBlueColor&&
       							MonitorSetting.BrightnesssRGB == UserPrefBrightness&&
       							MonitorSetting.ContrastsRGB == UserPrefContrast)
							#endif
       						        DDCBuffer[1]=0x02;
       						else
       							DDCBuffer[1]=0x04;

							break;
						
						case CTEMP_Warm1:
							#if USEFLASH
         						if( MonitorSetting.ColorTemp==UserPrefColorTemp&&
         							FactorySetting.RedColorWarm1 == UserPrefRedColor&&
         							FactorySetting.GreenColorWarm1 == UserPrefGreenColor&&
         							FactorySetting.BlueColorWarm1 == UserPrefBlueColor&&
         							MonitorSetting.BrightnessWarm1 == UserPrefBrightness&&
         							MonitorSetting.ContrastWarm1 == UserPrefContrast&&
         							FactorySetting.FBrightnessWarm1 == UserPrefBrightness&&
         							FactorySetting.FContrastWarm1 == UserPrefContrast)
         						#else
								if( MonitorSetting.ColorTemp==UserPrefColorTemp&&
         							MonitorSetting.RedColorWarm1 == UserPrefRedColor&&
         							MonitorSetting.GreenColorWarm1 == UserPrefGreenColor&&
         							MonitorSetting.BlueColorWarm1 == UserPrefBlueColor&&
         							MonitorSetting.BrightnessWarm1 == UserPrefBrightness&&
         							MonitorSetting.ContrastWarm1 == UserPrefContrast)
								#endif
         						        DDCBuffer[1]=0x02;
         						else
         							DDCBuffer[1]=0x04;
						
							break;
						case CTEMP_Normal:
								#if USEFLASH
         						if( MonitorSetting.ColorTemp==UserPrefColorTemp&&
         							FactorySetting.RedColorNormal== UserPrefRedColor&&
         							FactorySetting.GreenColorNormal== UserPrefGreenColor&&
         							FactorySetting.BlueColorNormal== UserPrefBlueColor&&
         							MonitorSetting.BrightnessNormal== UserPrefBrightness&&
         							MonitorSetting.ContrastNormal== UserPrefContrast&&
         							FactorySetting.FBrightnessNormal== UserPrefBrightness&&
         							FactorySetting.FContrastNormal== UserPrefContrast)
         						#else
								if( MonitorSetting.ColorTemp==UserPrefColorTemp&&
         							MonitorSetting.RedColorNormal== UserPrefRedColor&&
         							MonitorSetting.GreenColorNormal== UserPrefGreenColor&&
         							MonitorSetting.BlueColorNormal== UserPrefBlueColor&&
         							MonitorSetting.BrightnessNormal== UserPrefBrightness&&
         							MonitorSetting.ContrastNormal== UserPrefContrast)
								#endif
         						        DDCBuffer[1]=0x02;
         						else
         							DDCBuffer[1]=0x04;
						
							break;
						
						case CTEMP_Cool1:
							#if USEFLASH
							if( MonitorSetting.ColorTemp==UserPrefColorTemp&&				
							FactorySetting.RedColorCool1 == UserPrefRedColor&&
							FactorySetting.GreenColorCool1 == UserPrefGreenColor&&
							FactorySetting.BlueColorCool1 == UserPrefBlueColor&&
							MonitorSetting.BrightnessCool1 == UserPrefBrightness&&
							MonitorSetting.ContrastCool1 == UserPrefContrast&&
							FactorySetting.FBrightnessCool1 == UserPrefBrightness&&
							FactorySetting.FContrastCool1 == UserPrefContrast)
							#else
							if( MonitorSetting.ColorTemp==UserPrefColorTemp&&				
							MonitorSetting.RedColorCool1 == UserPrefRedColor&&
							MonitorSetting.GreenColorCool1 == UserPrefGreenColor&&
							MonitorSetting.BlueColorCool1 == UserPrefBlueColor&&
							MonitorSetting.BrightnessCool1 == UserPrefBrightness&&
							MonitorSetting.ContrastCool1 == UserPrefContrast)
							#endif
                                                        DDCBuffer[1]=0x02;
         						else
         							DDCBuffer[1]=0x04;
							//Osd_Draw4Num(0,1,UserPrefRedColor);
							//Osd_Draw4Num(0,2,UserPrefRedColorCool1);
							break;
						
						default:
							break;							
					}
					
				}
				//#undef ColTemp
				//#undef InpSrc
				//#undef ActFlag
			
			}
			#if HDCPKEY_IN_Flash
			else if(TPVCMD1==TPV_HDCPReadWriteStart&&WriteHDCPcodeFlag)	
			{
			#if 0
                WORD wTemp;
                if(SIO_TYPE2 < (HDCPKEY_SIZE/HDCP_BLOCK_LENGTH))
                    Flash_Write_Factory_KeySet(FLASH_KEY_HDCP, SIO_TYPE2, HDCP_BLOCK_LENGTH, &SIORxBuffer[3], HDCP_BLOCK_LENGTH);
                else if(SIO_TYPE2 == (HDCPKEY_SIZE/HDCP_BLOCK_LENGTH))
                {   Flash_Write_Factory_KeySet(FLASH_KEY_HDCP, SIO_TYPE2, HDCP_BLOCK_LENGTH, &SIORxBuffer[3], HDCPKEY_SIZE%HDCP_BLOCK_LENGTH);
                    for(wTemp=0; wTemp<HDCPKEY_SIZE; wTemp++)
                    {
                        TCRCvalue = CRC16(FlashReadByte(HDCPKEYSET_START+wTemp), TCRCvalue);
                    }
					Set_LoadHDCPKeyFlag();
					Power_TurnOnGreenLed();
                }
			#else
				      BYTE i;
		                    for (i = 0; i < HDCP_BLOCK_LENGTH; i++)
		                    {//SIO_TYPE2==HDCP BANK NO	  SIORxBuffer[3+i]==HDCP DATA
		                        if(HDCP_BLOCK_LENGTH*SIO_TYPE2+i<HDCPKEY_SIZE)
		                        {
		                            FlashHDCPWriteByte((HDCPKEYSET_START+HDCP_BLOCK_LENGTH*SIO_TYPE2+i),SIORxBuffer[3+i]);
						 #if ENABLE_HDCP
		                            TCRCvalue = CRC16(FlashReadByte(HDCPKEYSET_START+HDCP_BLOCK_LENGTH*SIO_TYPE2+i), TCRCvalue);
						 #endif
												
		                        }
								
		                    }
		                   
		                    if (SIO_TYPE2==HDCPKEY_SIZE/HDCP_BLOCK_LENGTH)//HDCP WRITE END
		                    {
		                        Set_LoadHDCPKeyFlag();
						bFlashWriteFactory = FALSE;
						FlashEnableWP();

					   #if 0 //Enable_Cache
                                     MCU_EnableCache();
                                     #endif

		                        hw_SetGreenLed();
		                    }
			#endif
			}	
			#endif
			else if(TPVCMD1==TPV_SelectGamma)	
			{
			}	


			

			
			#undef TPV_ValueH
			#undef TPV_ValueL			
			#undef TPVCMD1
//==========================NewFactoryCommand=================

			#endif
			{
	            	AOCAlign();

			}

		}   
        }
#if HKC_ColorTemp_Adjust
       else if(CPCode == 0x7B && PageValue == 0)
       {
            Resault=HKCAlignControl();
       }
#endif	

        else
        {
            DDCBuffer[2] = 0x01;
        }
        //Setting----------------------------------End
        //Transmission-----------------------------Start
        if (CommandCode == SETVCPFeature)
        {
        	/*
			if(CPCode==0xFE)
			{
			#ifdef UseVGACableReadWriteAllPortsEDID
				if(DDCBuffer[3]==0xEF)
				{
					//Monitor Feedback : 6f 6e 51 83 HiAddr LowAddr ByteNum chksum ;     ByteNum= Length - 0x80 - 6 
					DDCBuffer[1] = DDCBuffer[5];
					DDCBuffer[2] = DDCBuffer[6];
					DDCBuffer[3] = (DDCBuffer[0]-0x86);
					DDCBuffer[0] = 0x83;
					return 3;
				}
				else if((DDCBuffer[3]==0xE1)&&(DDCBuffer[4]==0xA2)&&(DDCBuffer[5]==0x01))
				{
					//Monitor Feedback: 6f 6e 83 02 EDIDPort Status chksum
					DDCBuffer[0] = 0x83;
					DDCBuffer[1] = 0x02;
							
					//Return Input Types Information
					#if INPUT_TYPE==INPUT_1A
					DDCBuffer[2] = VGA_Port1;
					#elif INPUT_TYPE==INPUT_1A1D
					DDCBuffer[2]  = VGA_Port1|DVI_Port1;
					#elif INPUT_TYPE==INPUT_1A2D
					DDCBuffer[2]  = VGA_Port1|DVI_Port1|HDMI_Port1;
					#else
					DDCBuffer[2]  = 0x00;
					#endif
				
					#ifdef ReduceDDC
						DDCBuffer[3]=((DDCBuffer[6]==0x01)?BIT1:0)|BIT0;
						//BIT0=0,EDID Saved in EEPRom;BIT0=1,EDID Saved in Flash
						//BIT1:(0:Write Disable;1:Write Enable)
					#else
						DDCBuffer[3]=((DDCBuffer[6]==0x01)?BIT1:0);
					#endif
					return 3;
				}
				else
				#endif
				{ //////yuanzhi 20100428_1 for 单口烧录不通问题.
				BYTE j;
				WORD DFM_TCRCvalue=0;

				DDCBuffer[0] = 0x83;
				//DDCBuffer[1] = 0x02;
				for(j = 0; j< Response_Command_LENGTH; j++ )			 
				DFM_TCRCvalue = CRC16(SIORxBuffer[j],DFM_TCRCvalue);

				DDCBuffer[2] = (BYTE)((DFM_TCRCvalue&0xff00)>>8);
				DDCBuffer[3] = (BYTE)(DFM_TCRCvalue&0x00ff);
				return 3;
			         }
		}
//*/
	  #if HKC_ColorTemp_Adjust
	   if(CPCode == 0x7B)	
            return Resault;
	   else
	#endif
		//else
		     return 0;
        }
        else
        {
            DDCBuffer[0] = 0x89;
            DDCBuffer[1] = 0xC7;
            DDCBuffer[3] = PageValue;
            DDCBuffer[4] = CPCode;
            DDCBuffer[5] = TPValue;
            DDCBuffer[6] = ValueH;
            DDCBuffer[7] = ValueL;
            DDCBuffer[8] = RetValueH;
            DDCBuffer[9] = RetValueL;
            return 9;
        }
        //Transmission-------------------------------End
    }
    //SetValue------------------------------------------------------End
    else if (CommandCode == SaveCurrentSetting)
    // 1-10
    {
    	 #if HKC_ColorTemp_Adjust
    		if(FactoryModeFlag)
		{	
			Clr_BurninModeFlag();
			Clr_ATEModeFlag();

			Clr_FactoryModeFlag();
			// Set_HideFactoryRootMenuFlag();
			 SaveFactorySetting();
                      SaveMonitorSetting();
			 MenuPageIndex=RootMenu;
			 MenuItemIndex=0; 
			 Osd_Hide();
			 //bColorTempAdjustFlag=0;
		}
		else
	#endif		
		{
        //SaveUserPref();
        #if USEFLASH
        UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit|bFlashSaveModeBit);
        #else
        SaveMonitorSetting();
        SaveModeSetting();
        #endif
		}
        return 0;
    }
    else if (CommandCode == GETVCPPage)
    // 1-11
    {
        DDCBuffer[0] = 0x83;
        DDCBuffer[1] = 0xC5;
        DDCBuffer[2] = 0;
        DDCBuffer[3] = 3;
        return 3;
    }
    DDCBuffer[0] = 0x80;
    return 0;
}

#if HKC_ColorTemp_Adjust

//bit ubAutoAlign=0;
extern void SaveUserPrefSetting(BYTE menuPageIndex);
//extern Bool ExecuteKeyEvent(MenuItemActionType menuAction);
Bool HKCAlignControl(void)
{
    //BYTE CommandCode, CPCode, ValueH, ValueL, RetValueH, RetValueL, PageValue;   
    //WORD WordValue;




		

//-----------------------------------tudar add for hkc autoalign start-----------------------
// if(CPCode == 0x7B && PageValue == 0)
//{
        //bColorTempAdjustFlag=1;
	if(ubAutoAlign==0)
	{
		if((DDCBuffer[3]==0x35)&&(DDCBuffer[4]==0x1f)&&(DDCBuffer[5]==0))//enter align mode
		{
			Set_ATEModeFlag();
			Set_FactoryModeFlag();

            		mStar_WriteByte(DDCEN2, D_EN_B);//20051112mStar_WriteByte(DDCEN2, D_EN_B|BIT5);
			mStar_WriteByte(DDCEN1, D_EN_B|BIT5);

			MenuPageIndex=RootMenu;
			MenuItemIndex=0;  
			//ExecuteKeyEvent(MIA_RedrawMenu);
			
			ubAutoAlign=1;
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
		}
		else
		{
			return 0;
		}
	}
	

if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x13)&&(DDCBuffer[5]==0))//auto color
{
		 if(AutoColor()!=0)
		 {
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
		 }
		 else
		 {
			DDCBuffer[1]=0x03;
			DDCBuffer[2]=0x58;
		 }	
			DDCBuffer[0]=0x84;

			#if USEFLASH
			#if  BlacklitTimeOnShow
			BlacklitTimeOn_S=0;
			BlacklitTimeOn_M=0;
			BlacklitTimeOn_H=0;
			//BlacklitTimeOn_HH=0;
		//	BlacklitTimeOn_SShift=0;
			//BlacklitTime=0;
			//*/
			//MonitorBlacklitTimeOn_HH=0;
			MonitorBlacklitTimeOn_H=0;
			MonitorBlacklitTimeOn_M=0;
			MonitorBlacklitTimeOn_S=0;
			//MonitorBackLightTime=0;

			//SaveMonitorSetting();
			#endif
			#endif
			
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x14)&&(DDCBuffer[5]==0))//set CT
{
			//SetColorTempDDCCI(CTEMP_9300K);
			SetFactoryColorTempCool1();
			//UserPrefColorTemp=CTEMP_9300K;
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x14)&&(DDCBuffer[5]==1))//set CT
{
			//SetColorTempDDCCI(CTEMP_6500K);
			SetFactoryColorTempWarm1();
			//UserPrefColorTemp=CTEMP_6500K;
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
#if FactoryAuto_5400K	
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x14)&&(DDCBuffer[5]==2))//set CT
{
		
		       SetFactoryColorTempNormal();
			
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
#endif
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x15))//read R gain
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
		if(UserPrefColorTemp==CTEMP_6500K)
			DDCBuffer[2]=UserPrefRedColor6500K;//0x4F;
		else if(UserPrefColorTemp==CTEMP_9300K)
			DDCBuffer[2]=UserPrefRedColor9300K;//0x4F;
		#if FactoryAuto_5400K	
		else if(UserPrefColorTemp==CTEMP_7500K)
			DDCBuffer[2]=UserPrefRedColor7500K;//0x4F;
		#endif
		else 
			DDCBuffer[2]=UserPrefRedColor;//0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x16))//read G gain
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
		if(UserPrefColorTemp==CTEMP_6500K)
			DDCBuffer[2]=UserPrefGreenColor6500K;//0x4F;
		else if(UserPrefColorTemp==CTEMP_9300K)
			DDCBuffer[2]=UserPrefGreenColor9300K;//0x4F;
		#if FactoryAuto_5400K	
		else if(UserPrefColorTemp==CTEMP_7500K)
			DDCBuffer[2]=UserPrefGreenColor7500K;//0x4F;
		#endif
		else 
			DDCBuffer[2]=UserPrefGreenColor;//0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x17))//read B gain
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
		if(UserPrefColorTemp==CTEMP_6500K)
			DDCBuffer[2]=UserPrefBlueColor6500K;//0x4F;
		else if(UserPrefColorTemp==CTEMP_9300K)
			DDCBuffer[2]=UserPrefBlueColor9300K;//0x4F;
		#if FactoryAuto_5400K	
		else if(UserPrefColorTemp==CTEMP_7500K)
			DDCBuffer[2]=UserPrefBlueColor7500K;//0x4F;
		#endif
		else 
			DDCBuffer[2]=UserPrefBlueColor;//0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x15))//write R gain
{
			BYTE ucR_Gain;
			ucR_Gain=DDCBuffer[5];
			if((ucR_Gain >= 0) && (ucR_Gain <= 255))
			{
				if(UserPrefColorTemp == CTEMP_6500K) 
				{
					UserPrefRedColor6500K=ucR_Gain;
					UserPrefRedColorSRGB=ucR_Gain;
				}
				else if(UserPrefColorTemp == CTEMP_9300K) 
					UserPrefRedColor9300K=ucR_Gain;
			       #if FactoryAuto_5400K	
				else if(UserPrefColorTemp == CTEMP_7500K) 
					UserPrefRedColor7500K=ucR_Gain;
			       #endif
				else if(UserPrefColorTemp == CTEMP_USER) 
					UserPrefRedColor=ucR_Gain;
			       mStar_AdjustRedColor(ucR_Gain,UserPrefContrast);
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x16))//write G gain
{
			BYTE ucG_Gain;
			ucG_Gain=DDCBuffer[5];
			if((ucG_Gain >= 0) && (ucG_Gain <= 255))
			{
				if(UserPrefColorTemp == CTEMP_6500K) 
				{
					UserPrefGreenColor6500K=ucG_Gain;
					UserPrefGreenColorSRGB=ucG_Gain;
				}
				else if(UserPrefColorTemp == CTEMP_9300K) 
					UserPrefGreenColor9300K=ucG_Gain;
				#if FactoryAuto_5400K	
				else if(UserPrefColorTemp == CTEMP_7500K) 
					UserPrefGreenColor7500K=ucG_Gain;
				#endif
				else if(UserPrefColorTemp == CTEMP_USER) 
					UserPrefGreenColor=ucG_Gain;
  			       mStar_AdjustGreenColor(ucG_Gain,UserPrefContrast);
				
				
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x17))//write B gain
{
			BYTE ucB_Gain;
			ucB_Gain=DDCBuffer[5];
			if((ucB_Gain >= 0) && (ucB_Gain <= 255))
			{
				if(UserPrefColorTemp == CTEMP_6500K) 
				{
					UserPrefBlueColor6500K=ucB_Gain;
					UserPrefBlueColorSRGB=ucB_Gain;
				}
				else if(UserPrefColorTemp == CTEMP_9300K) 
					UserPrefBlueColor9300K=ucB_Gain;
				#if FactoryAuto_5400K	
				else if(UserPrefColorTemp == CTEMP_7500K) 
					UserPrefBlueColor7500K=ucB_Gain;
				#endif
				else if(UserPrefColorTemp == CTEMP_USER) 
					UserPrefBlueColor=ucB_Gain;
			       mStar_AdjustBlueColor(ucB_Gain,UserPrefContrast);
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x18)&&(DDCBuffer[5]==0))//save CT
{
			#if USEFLASH
	               //Set_FactorySettingSaveFlag(); 	              
			SaveFactorySetting();
			#else
                      SaveMonitorSetting();
			#endif
								 
			UserPrefColorTemp=CTEMP_9300K;
			 #if USEFLASH
                      SaveMonitorSetting();
                     #else	
                     NVRam_WriteByte(NVR_MONITOR_ADDR(MonitorFlag), MonitorFlags);
			#endif
			
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x18)&&(DDCBuffer[5]==1))//save CT
{
			#if USEFLASH
	               //Set_FactorySettingSaveFlag();	              
			SaveFactorySetting();			
			#else
                    SaveMonitorSetting();
			#endif
			UserPrefColorTemp=CTEMP_6500K;
			#if USEFLASH
                      SaveMonitorSetting();
                     #else	
                     NVRam_WriteByte(NVR_MONITOR_ADDR(MonitorFlag), MonitorFlags);
			#endif
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
#if  FactoryAuto_5400K	
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x18)&&(DDCBuffer[5]==2))//save CT
{
			#if USEFLASH
	               //Set_FactorySettingSaveFlag(); 	             
			SaveFactorySetting();
			#else
                    SaveMonitorSetting();
			#endif
		
			UserPrefColorTemp=CTEMP_7500K;
		
			#if USEFLASH
                      SaveMonitorSetting();
                     #else	
                     NVRam_WriteByte(NVR_MONITOR_ADDR(MonitorFlag), MonitorFlags);
			#endif
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
#endif
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x19)&&(DDCBuffer[5]==0))//set brightness and contrast default
{
			UserPrefBrightness=DefBrightness;
			UserPrefContrast=DefContrast;
			mStar_AdjustBrightness(UserPrefBrightness);
			#ifdef TSUMXXT
			mStar_AdjustContrast( UserPrefContrast );
			#else			
			mStar_AdjustContrast(UserPrefContrast);
			#endif
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x1A))//read adjust brightness 
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=UserPrefBrightness;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x1B))//read adjust contrast
{
            UserPrefContrast=GetScale100Value(UserPrefContrast, MinContrastValue, MaxContrastValue);
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=UserPrefContrast;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1A))//write adjust brightness 
{
			BYTE uctempData;
			uctempData=DDCBuffer[5];
			if((uctempData >= 0) && (uctempData <= 100))
			{
				UserPrefBrightness=uctempData;
				mStar_AdjustBrightness(UserPrefBrightness);
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			DDCBuffer[0]=0x84;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1B))//write adjust contrast
{
			BYTE uctempData;
			uctempData=DDCBuffer[5];
			if((uctempData >= 0) && (uctempData <= 100))
			{
			      // ContrastTempValue=uctempData;
				UserPrefContrast=uctempData;
				//UserPrefContrast=(MIN_CONTRAST_VALUE+((MAX_CONTRAST_VALUE-MIN_CONTRAST_VALUE)/100)*ContrastTempValue);
				#ifdef TSUMXXT
				mStar_AdjustContrast( UserPrefContrast );
				#else
				mStar_AdjustContrast(UserPrefContrast);
				#endif
				
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			DDCBuffer[0]=0x84;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1C))//set default language or select reset language
{
			UserPrefLanguage=DEF_OSD_LANG;
			//LoadLanguageFont(UserPrefLanguage);
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1D)&&(DDCBuffer[5]==0))//enter burn mode
{
			return 0;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1D)&&(DDCBuffer[5]==1))//exit burn mode
{
			return 0;
}
else if((DDCBuffer[3]==0x35)&&(DDCBuffer[4]==0x1F)&&(DDCBuffer[5]==0))//enter align mode
{
			ubAutoAlign=1;
			Set_ATEModeFlag();
			Set_FactoryModeFlag();

			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
/*
else if((DDCBuffer[3]==0x35)&&(DDCBuffer[4]==0x1F)&&(DDCBuffer[5]==1))//exit align mode
{
			ubAutoAlign=0;
			//Clr_ATEModeFlag();
			Clr_FactoryModeFlag();
			//SetColorTempDDCCI(CTEMP_6500K);
			//UserPrefColorTemp=CTEMP_6500K;


			if (DEF_CTEMP==CTEMP_Warm1)
			  	SetFactoryColorTempWarm1();
			else if (DEF_CTEMP==CTEMP_Normal)
				SetFactoryColorTempNormal();
			else if(DEF_CTEMP==CTEMP_SRGB)
			 	SetFactoryColorTempSRGB();			
			else
                       	SetFactoryColorTempCool1();

			//SaveMonitorSetting();
			//SaveFactorySetting();
			MenuPageIndex=FactoryMenu;
			MenuItemIndex=0;  
			//ExecuteKeyEvent(MIA_RedrawMenu);
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
*/
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x21)&&(DDCBuffer[5]==0))//factory mode on
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x21)&&(DDCBuffer[5]==1))//factory mode off
{
		ubAutoAlign=0;
		if(FactoryModeFlag)
		{	
			//Clr_ATEModeFlag();
			Clr_FactoryModeFlag();
			//Set_HideFactoryRootMenuFlag();//20090521
			 #if USEFLASH
                      SaveMonitorSetting();
                     #else	
                     NVRam_WriteByte(NVR_MONITOR_ADDR(MonitorFlag), MonitorFlags);
			#endif
			 MenuPageIndex=RootMenu;
			 MenuItemIndex=0; 
			 Osd_Hide();			
			 SaveUserPrefSetting(0);
			
		}
		else
		{
			SaveUserPrefSetting(0);//SaveUserPref();
		}
			ResetAllSetting();
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x23)&&(DDCBuffer[5]==0))//change source vga
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x23)&&(DDCBuffer[5]==1))//change source dvi
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}

#if  FDDCCI_WriteEDID
    else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x30))//DDC长度
		{
	          edid_cont=((DDCBuffer[5])*128);
		   edid_pointer=0;
		    if((DDCBuffer[5])<3)
			{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
			}
		    else
			{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x03;
			DDCBuffer[2]=0x58;
			return 2;				
			}
		}

	else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x31))//DDC接收
		{
			XDATA BYTE j,i=0;
		   	j=DDCBuffer[5];
			if (edid_pointer<edid_cont)
			 {
	                 for(i=0;i<j;i++)
			      {
				 DDC1[edid_pointer]=DDCBuffer[6+i];
				 edid_pointer++;
			      }
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
				return 2;
			 }
			else
			  {
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				return 2;
  		         }				
	
		}
	 else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x32))   //2    ddC 校验
		{
			XDATA BYTE checksun=0;
			XDATA WORD i;
			if ((edid_cont==128)||(edid_cont==256))
				{
				for(i=0;i<edid_cont;i++)
					{
					checksun+=DDC1[i];
					
					}
					if(checksun==0)
						{
						DDCBuffer[0]=0x84;
						DDCBuffer[1]=0x00;
						DDCBuffer[2]=0x4F;
						return 2;
						}

				}

				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				return 2;

		}
	 else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x33))//ddc保存
		{
			XDATA BYTE checksun;
			BYTE  EDID_TYPE=0;
			   #if USEFLASH
 			  WORD i;
 			  #endif

 			//Osd_Show();
			if(DDCBuffer[5]==0)				
			   EDID_TYPE=S_EDID_VGA;				
			else if (DDCBuffer[5]==1)
			   EDID_TYPE=S_EDID_DVI;
			else if (DDCBuffer[5]==2)
			   EDID_TYPE=S_EDID_HDMI;
			
			#if USEFLASH
			
			  if(EDID_TYPE==S_EDID_VGA)
			  	{
			  	//FlashErase( FLASH_DDCA_START_ADDR);            
                      //	Flash_WriteTbl1(FLASH_DDCA_START_ADDR, DDC1, 128); 
			      //Flash_Write_/actory_KeySet(FLASH_KEY_DDCA, 0x00, DDCAKEY_SIZE, DDC1, DDCAKEY_SIZE);
                        	Flash_Write_Factory_KeySet(FLASH_KEY_DDCA, FALSE,  DDCAKEY_SIZE, DDC1, DDCAKEY_SIZE);	
			  	}
                       // else if (EDID_TYPE==S_EDID_DVI)
                        //	{
                        //	FlashErase( FLASH_DDCA_START_ADDR);  
				//Flash_WriteTbl1( FLASH_DDCD_START_ADDR, DDC1, 128);			
			   	//Flash_Write_Factory_KeySet(FLASH_KEY_DDCD, FALSE,  DDCDKEY_SIZE, DDC1, DDCDKEY_SIZE);		
                        //	}
                        //else if (EDID_TYPE==S_EDID_HDMI)
				 //Flash_WriteTbl1(FLASH_DDCDP_START_ADDR, DDC1, 256);
                          //Flash_Write_Factory_KeySet(FLASH_KEY_DDCDP, FALSE,  DDCDPKEY_SIZE, DDC1, DDCDPKEY_SIZE);
                        
                        checksun=DDC1[edid_cont-1];
						
                       for(i=0;i++;i<edid_cont)
				DDC1[i]=0;
                       
                        if(EDID_TYPE==S_EDID_VGA)
				//Flash_ReadTbl1(FLASH_DDCA_START_ADDR, &DDC1, 128);
                        //	Flash_Read_Factory_KeySet(FLASH_KEY_DDCA, TRUE, 0,  DDCAKEY_SIZE, DDC1, DDCAKEY_SIZE);	
                        Flash_Read_Factory_KeySet(FLASH_KEY_DDCA, TRUE, 0, DDCAKEY_SIZE, DDC1, DDCAKEY_SIZE);
                       // else if (EDID_TYPE==S_EDID_DVI)
			//	Flash_ReadTbl1(FLASH_DDCD_START_ADDR, &DDC1, 128);
			   	//Flash_Read_Factory_KeySet(FLASH_KEY_DDCD, TRUE, 0,   DDCDKEY_SIZE, DDC1, DDCDKEY_SIZE);	
			   //else if (EDID_TYPE==S_EDID_HDMI)
			   //	Flash_ReadTbl1(FLASH_DDCDP_START_ADDR, &DDC1, 256);
			   	//Flash_Read_Factory_KeySet(FLASH_KEY_DDCDP, TRUE, 0,   DDCDPKEY_SIZE, DDC1, DDCDPKEY_SIZE);	
               
			#else
			
				 if(EDID_TYPE==S_EDID_VGA)
				 NVRam_WriteTbl( RM_MHL_EDID_BASEADDR , DDC1, 128 );
				 else if (EDID_TYPE==S_EDID_DVI)
				 NVRam_WriteTbl( RM_MHL_EDID_BASEADDR+128 , DDC1, 128 );

				 checksun=DDC1[edid_cont-1];
				 DDC1[edid_cont-1]=0;
				 
				if(EDID_TYPE==S_EDID_VGA)
				 NVRam_ReadTbl( RM_MHL_EDID_BASEADDR , &DDC1, 128 );
				else if (EDID_TYPE==S_EDID_DVI)
				NVRam_ReadTbl( RM_MHL_EDID_BASEADDR+128 , &DDC1, 128 );
	               #endif     
					 
			     if(DDC1[(edid_cont-1)]==checksun)
				{	
				#if VGA_EDID_Write_To_24C02
				Clr_VGA_EDID_Flag();
				#endif
	 			//Osd_Hide();                            
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
				return 2;
				}
			      else
				{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				return 2;
				}
			
		}
	
         else if((DDCBuffer[3]==0x35)&&(DDCBuffer[4]==0x1F)&&(DDCBuffer[5]==1))//exit align mode
		{
			ubAutoAlign=0;	
			//Clr_ATEModeFlag();
			#if !Enable_External_EDID
			mStar_InitDDC();
			#endif
			hw_ClrAmberLed();
			ReloadedidCounter=2; 
			if(FactoryModeFlag)
			{
				Osd_Hide();
				Clr_FactoryModeFlag();	
				MenuPageIndex=RootMenu;
			}		
		#if USEFLASH
			SaveMonitorSetting();
		#else	
			NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
		#endif
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
            }
#endif
else
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x03;
			DDCBuffer[2]=0x58;
			return 2;
}


//}
 



			return 2;

}
#endif

