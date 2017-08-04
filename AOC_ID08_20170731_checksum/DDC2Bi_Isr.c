#include "types.h"
#include "global.h"
#define DDC2B_CLEAR						0
#define DDC2B_SRCADDRESS				1
#define DDC2B_COMMAND					2
#define DDC2B_RECBODY					3
#define DDC2B_WAITFORCK					4
#define DDC2B_COMPLETED					5
#define DDC2B_MSGON						6
#define DDC2B_DEST_ADDRESS				0x6e
#define DDC2B_SRC_ADDRESS				0x51
#define DDC2Bi_CONTROL_STATUS_FLAG		0x80
#define INITRxCK			DDC2B_SRC_ADDRESS ^ DDC2B_DEST_ADDRESS
#define TXBI	 BIT4   // Annie 2006.7.6 BIT7
#define RCBI	 BIT3  // Annie 2006.7.6  BIT6
//#define MaxBufLen 50
extern BYTE xdata rxInputPort; // Annie 2006.7.5
extern BYTE xdata DDCBuffer[MaxBufLen];
extern BYTE xdata rxIndex;
extern BYTE xdata rxStatus;
extern BYTE xdata rxLength;
extern BYTE xdata rxChkSum;
extern BYTE xdata txStatus;
extern BYTE *txBufferPtr;
extern BYTE xdata txLength;

void DoDDCCI(BYTE Source) // Annie 2006.7.5
{
    BYTE intFlag, rxByte;
#ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028
if ( Source == Input_Digital2 )			
	intFlag = DVI1_INT_FLAG;
else 
#endif
{
if (Source == Input_Digital)		
    	intFlag = DVI_INT_FLAG;
else
	intFlag = ADC_INT_FLAG;
}
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
		if (Source == Input_Digital2)					
			DVI1_RBUF_WDP = 0;
		else		
		#endif
		if (Source == Input_Digital)				
                    	DVI_RBUF_WDP = 0;
		else
			ADC_RBUF_WDP = 0;			
                    return ;
                }
                // send out the current byte
	    #ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028
	    if (Source == Input_Digital2)		
	    	DVI1_RBUF_WDP = *txBufferPtr++; 	
	    else		  
	    #endif	
	    if (Source == Input_Digital) 	
                	DVI_RBUF_WDP =  *txBufferPtr++;
	    else
            	ADC_RBUF_WDP =  *txBufferPtr++;		    
                txLength--;
                intFlag &= (~TXBI); //clear interrupt flag
                break;
            case RCBI:
                // read the received byte
	    #ifdef TSUMW88MDT3_For_HDMIDDCCI//20091028
	    if (Source == Input_Digital2)	
		rxByte = DVI1_WBUF_RDP;
	    else		  
	    #endif
	    if (Source == Input_Digital)		    
                	rxByte = DVI_WBUF_RDP;
	    else
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
                    	#if 1
						rxStatus = DDC2B_CLEAR;
						rxIndex = 0;
						#else
                        DDC2Bi_InitRx();
						#endif
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
                    // ...here we are waiting for CheckSum...
                case DDC2B_WAITFORCK:
                    // if CheckSum match
                    if (rxChkSum == rxByte)
                    {
                        rxInputPort = Source;
                        rxStatus = DDC2B_COMPLETED;
                        Set_RxBusyFlag();
                    }
                    // elsechecksum error
                    else
                    {
                        // if CheckSum error re-initialize the receive buffer
						#if 1
						rxStatus = DDC2B_CLEAR;
						rxIndex = 0;
						#else
                        DDC2Bi_InitRx();
						#endif
                    }
                    break;
                default:
                    // clear the rxState and the current buffer for a new message
                    if (!RxBusyFlag)
                    {
						#if 1
						rxStatus = DDC2B_CLEAR;
						rxIndex = 0;
						#else
                        DDC2Bi_InitRx();
						#endif
                    }
                    break;
                }
                intFlag &= (~RCBI); //clear interrupt flag
                break;
					// 100929 coding addition
					default:
						#ifdef TSUMW88MDT3_For_HDMIDDCCI
						DVI1_RBUF_WDP = DVI1_WBUF_RDP;
						#endif
						DVI_RBUF_WDP = DVI_WBUF_RDP;
						ADC_RBUF_WDP = ADC_WBUF_RDP;
						txStatus = DDC2B_CLEAR;
						#if 1
						rxStatus = DDC2B_CLEAR;
						rxIndex = 0;
						#else
                        DDC2Bi_InitRx();
						#endif
					break;					  
        }
    }
}

