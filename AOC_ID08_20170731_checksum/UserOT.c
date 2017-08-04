
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// This file is specifically generated for
//   Customer: BenQ
//   Project: Platium
//   Date: 09-Feb, 2007

// OTLib 3.2 is customized with the following options:
// * Interrupt Mode of operation (Attention Line)
// * Hardware I2C on the host processor
// * Button  Features on OneTouch
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Copyright (c) 2006-2007 Synaptics, Inc.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to use, 
// copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
// Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Include all the header files you want to include here
// ***********************************************************************
// NOTE: User defined header file: Please add/ modify for your application
// ***********************************************************************
//#include <stdio.h> 

// ***************************************
// NOTE: OTLib header file: Do Not Modify
// ***************************************
// OTLib specific header files
#include "board.h"
#if ScanKey
#include <intrins.h>
#include "types.h"
#include "SynaOT.h"
#include "i2cdef.h"
#include "UserOT.h"
#include "Misc.h"
#include "Dell_OTConfig.h"
#include "global.h"

#if 1
// ==========================================================================
// * OTLib Host Processor Specific (Hardware Abstract Layer (HAL) functions *
// ==========================================================================
// Prototype defintions 
 
// *************************************************************************************
// The User has to write the following HAL functions:
//
// 1) OT_U8 OT_WriteI2C(OT_U8 I2CAddr7Bit, OT_U8 Buffer[], OT_U8 BytesToSend);
//			// Write a buffer to I2C
//
// 2) OT_U8 OT_ReadI2C(OT_U8 I2CAddr7Bit, OT_U8 Buffer[], OT_U8 BytesToRead);
//			// Read a buffere from I2C
//
// 3) OT_U8 OT_HAL_BitRead_ATTN(void);
//			//	Read the state of the Attention (ATTN) line
//	
// NOTE: Do not change the function names. Just add to the body of the HAL functions
// **************************************************************************************
// ========================================================================
// This is a function to Send a Byte sequence on the I2C bus
//
// Functions Name:
//  * OT_WriteI2C()
//
// Inputs: (
//  * I2CAddr7Bit, (7-bit I2C Device Address)
//  * Buffer (Buffer of bytes to be written)
// 		- High Byte of subaddress andthe Low Byte of subaddress
//		- are the first two bytes of pBuffer[]
//  * BytesToSend (No. of Bytes to write)
//
// Outputs:
//	Returns OT_SUCCESS or OT_FAILURE
//
// Assumptions:	
//	 The High Byte of subaddress and the Low Byte of subaddress
// 	 are the first two bytes of Buffer[]
// ========================================================================
void IICDelay(void)
{
    BYTE i;
#if Scan_Source == Cypress
	#if MainBoardType==MainBoard_715G3244_TSUMU18NR||ModelName==ENVISION_P2271wl||MainBoardType==MainBoard_715G3603_M0A||MainBoardType==MainBoard_715G3598_M0A||MainBoardType==MainBoard_715G3329_A2||\
		ModelName==ENVISION_P2271wl1||ModelName==ENVISOIN_P2271WS || MainBoardType==MainBoard_715G3959_T		//110623 Modify
	for(i = 0; i < 9; i ++) // IIC clock 90K
	#else
    for(i = 0; i < 8; i ++) // IIC clock 90K
    #endif
#else
    for(i = 0; i < 40; i ++) // IIC clock 24K
#endif
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();
    }
        
}

#if Scan_Source == Cypress
#define TimeOut     ((bKeyInitialFlag)?(3500):(35))
void IIC_AckDelay(void)
{
    WORD i;
    for(i = 0; i < TimeOut; i ++) // 20: 1.29ms, 30: 1.78ms, 35: 2ms
    {
        _nop_();_nop_();_nop_();_nop_();_nop_();

        if(SK_SDALo())
            break;
    }
}
#endif

Bool Wait_Bus_Release(void)
{
    BYTE	Counter=100;
	
	do{
		Counter--;
#if Scan_Source == Cypress
            IIC_AckDelay();
#else
		IICDelay();
#endif
	}
	while((SK_SDAHi()) && (Counter >0) );
	
	if(SK_SDALo())						
		return TRUE;
	
	return FALSE;			
}

Bool SK_i2c_Start(void)
{ Set_SK_SDA();
  IICDelay();
  Set_SK_SCL();
  IICDelay();
  if (SK_SDALo() || SK_SCLLo())
    return FALSE;
  Clr_SK_SDA();
  IICDelay();
  Clr_SK_SCL();
  return TRUE;
}
//=============================================
// Setup i2c Stop condition
void SK_i2c_Stop(void)
{ // SCL=L, SDA=L, Stop condition.
  Clr_SK_SCL();
  IICDelay();
  Clr_SK_SDA();  
  IICDelay();
  Set_SK_SCL();
  IICDelay();
  Set_SK_SDA();
}

//============================================
Bool SK_i2c_SendByte(BYTE value)
{ BYTE i;
  Bool result;
  
  for (i=0; i<8; i++) // Send data via i2c pin
    { if (value&BIT7)
        Set_SK_SDA();
      else
      	Clr_SK_SDA();      
      IICDelay();
      Set_SK_SCL();      
      value<<=1;
	  IICDelay();
      Clr_SK_SCL(); 
    }
  IICDelay();
  Set_SK_SDA();
  Wait_Bus_Release();//Kevin 20070727
  Set_SK_SCL();
  IICDelay();
  result=SK_SDALo(); // see if ACK
  Clr_SK_SCL();
  IICDelay();
  Clr_SK_SDA();
  IICDelay();
  return result;
}

//============================================
BYTE SK_i2c_ReceiveByte(const Bool ack)
{ BYTE i, tempCnt = 0;
  BYTE value;

  for (i=0; i<8; i++)
    { value<<=1;
      Set_SK_SDA();
	  IICDelay();
      Set_SK_SCL();
      
      tempCnt = 0;
      while(SK_SCLLo() && (++tempCnt < 100)); // 080925
      
      IICDelay();
      if (SK_SDAHi())
      	value|=BIT0;
      Clr_SK_SCL();
     IICDelay();//20051101
    }
  if (ack)
    Clr_SK_SDA();
  else
    Set_SK_SDA();
  IICDelay();
  Set_SK_SCL();
  
  tempCnt = 0;
  while(SK_SCLLo() && (++tempCnt < 100)); // 080925

  IICDelay();
  Clr_SK_SCL();
  IICDelay();
  return value;  	
}

Bool SK_i2c_BurstWrite(BYTE count, BYTE *buffer)
{ while (count--)
    { if (SK_i2c_SendByte(*(buffer++))==FALSE)
        return FALSE;
    }
 return TRUE;
}
Bool SK_i2c_BurstRead(BYTE count, BYTE * buffer)
{ BYTE i;

  for (i=0; i<count-1; i++)
    *(buffer+i)=SK_i2c_ReceiveByte(1);
  *(buffer+i)=SK_i2c_ReceiveByte(0);
  SK_i2c_Stop();
  return TRUE;
}

Bool SK_i2c_MasterStart(I2C_Direction direct, BYTE addr)
{ BYTE retry=5;

  if (direct==I2C_READ) // Set I2C direction bit.
    addr|=BIT0;
  else
    addr&=~BIT0;
  
  while (retry--)
    { if (SK_i2c_Start()==FALSE)
  		{
  	    ForceDelay1ms(1);
        continue;
  		}  
      if (SK_i2c_SendByte(addr)==TRUE) // send address success
        return TRUE;
      SK_i2c_Stop();
      ForceDelay1ms(1);
    }
  return FALSE;
}
#endif

//extern Bool i2c_MasterStart(I2C_Direction direct, BYTE addr);
//extern void i2c_Stop(void);
//extern Bool i2c_BurstWrite(BYTE count, BYTE *buffer);
//extern Bool i2c_BurstRead(BYTE count, BYTE * buffer);

BYTE OT_WriteI2C(BYTE I2CAddr7Bit, BYTE *Buffer, BYTE BytesToSend)
{
// ***********************************************************************
// NOTE: HAL function: User adds/ writes to the body of this function
// ***********************************************************************	
	BYTE return_value=OT_SUCCESS;
	BYTE i[1]={0};

#if Scan_Source == Cypress // Cypress request
    if (SK_SDALo() || SK_SCLLo())
        return OT_FAILURE;
#endif
    
    if (SK_i2c_MasterStart(I2C_WRITE, I2CAddr7Bit)==FALSE)
        return OT_FAILURE;

    if (SK_i2c_BurstWrite(BytesToSend, Buffer)==FALSE)
        return_value=OT_FAILURE;

    SK_i2c_Stop();	
    return return_value;
}

// ============================================================================
// This is a function to Read a Byte sequence on the I2C bus
//
// Functions Name:
//  * OT_ReadI2C()
//
// Inputs: (
//  * I2CAddr7Bit, (7-bit I2C Device Address)
//  * Buffer (Buffer of bytes to be read)
//  * BytesToSend (No. of Bytes to be read)
//
// Outputs:
//  	Returns OT_SUCCESS or OT_FAILURE
//	
// Assumptions:
//		It is assumed the High Byte and the Low Byte of subaddress are pre-set	
// ==============================================================================

BYTE OT_ReadI2C(BYTE I2CAddr7Bit, BYTE *Buffer, BYTE BytesToRead)
{
	BYTE return_value=OT_SUCCESS;

#if Scan_Source == Cypress // Cypress request
    if (SK_SDALo() || SK_SCLLo())
        return OT_FAILURE;
#endif
    
    if (SK_i2c_MasterStart(I2C_READ, I2CAddr7Bit)==FALSE)
        return OT_FAILURE;

    if (SK_i2c_BurstRead(BytesToRead, Buffer)==FALSE)
        return_value=OT_FAILURE;

    return return_value;
}

#if (Scan_Source==Synaptics)
//	Read the state of the Attention (ATTN) line
BYTE OT_HAL_BitRead_ATTN(void)			
{
// ***********************************************************************
// NOTE: HAL function: User adds/ writes to the body of this function
// ***********************************************************************
	if(hw_SK_INT)
		return 1;
	else
		return 0;
}

// wait for attention line to be Asserted with timeout 
// If this function is not implemented then the 
// default function with no timing control is used
BYTE OT_Poll_Attn_Line_TimeOut(void)
{
	DWORD	bCounter=0;
	
	// wait until attention line is asserted or timer timeout
	do{
		bCounter++;
		Delay1ms(1);
	}
	while( ( (OT_HAL_BitRead_ATTN() ==1) && (bCounter < OT_TIMEOUT)) );
	
	if(OT_HAL_BitRead_ATTN()== 0)						
		return OT_SUCCESS;
	
	return OT_FAILURE;			
}

#endif


#endif
