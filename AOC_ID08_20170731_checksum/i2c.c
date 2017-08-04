#include "types.h"
#include "i2cdef.h"
#include "board.h"
#include "debug.h"
#include "misc.h"
#include "global.h"
#include <math.h>
#include <intrins.h>

#define i2cSlaveAddr(deviceID, addr)    deviceID|((addr>>8)<<1)
#define i2cWordAddr(addr)       addr&0xFF

#if Enable_Lightsensor
    #if LightSensor_Source == EverLight
    extern BYTE xdata SensorErrorCnt;
    #else
    #if TSL2561
    extern BYTE xdata SensorErrorCnt;
    #endif
    #endif
#endif

#if !USEFLASH || Enable_Lightsensor ||VGA_EDID_Write_To_24C02|| (defined(UseVGACableReadWriteAllPortsEDID)&&!defined(ReduceDDC)) 
#define I2C_CHECK	1

/*
#if I2C_CHECK
#define I2C_CHECK_PIN_DUMMY	500//150//100  

void i2cSCL_Chk(void)
{
    	BYTE ucDummy = I2C_CHECK_PIN_DUMMY; // initialize dummy    	
	while (i2c_SCLLo() && (ucDummy--)) 	
			; // check SCL pull high
}
#else
#define i2cSCL_Chk()
#endif
//*/
Bool i2c_Start(void)
{ 
    Delay4us();
    Set_i2c_SDA();
  	Delay4us();
  	Set_i2c_SCL();

  	Delay4us();

   	if (i2c_SDALo() || i2c_SCLLo())
    	return FALSE;

  	Clr_i2c_SDA();
  	Delay4us();
  	Clr_i2c_SCL();
  	return TRUE;
}

void i2c_Stop(void)
{ 	
  	Clr_i2c_SCL();
  	Delay4us();
  	Clr_i2c_SDA();  
  	Delay4us();
  	
  	Set_i2c_SCL();
  	Delay4us();
  	Set_i2c_SDA();
}

Bool i2c_SendByte(BYTE value)
{ 	BYTE i;
  	Bool result;

  	for (i=0; i<8; i++) 
    { 	if (value&BIT7)
        	Set_i2c_SDA();
      	else
      		Clr_i2c_SDA();      
      	Delay4us();
      	Set_i2c_SCL();      
      	value<<=1;
      	Clr_i2c_SCL(); 
    }
  	Set_i2c_SDA();
  	Delay4us();
  	Set_i2c_SCL();
  	

  	result=i2c_SDALo(); 


  	Clr_i2c_SCL();
  	Delay4us();
  	Clr_i2c_SDA();
    
  	return result;
}

BYTE i2c_ReceiveByte(const Bool ack)
{ 	BYTE i;
  	BYTE value;
	
  	for (i=0; i<8; i++)
    { 	value<<=1;
      	Set_i2c_SDA();

      	Set_i2c_SCL();
      	Delay4us();
      	if (i2c_SDAHi())
      		value|=BIT0;
      	Clr_i2c_SCL();

    }
  	
  	if (ack)
    	Clr_i2c_SDA();
  	else
    	Set_i2c_SDA();
  	Delay4us();
  	
  	Set_i2c_SCL();
  	Delay4us();
  	Clr_i2c_SCL();
  	
  	return value;  	
}


Bool i2c_BurstWrite(BYTE count, BYTE *buffer)
{ 	while (count--)
    { 	if (i2c_SendByte(*(buffer++))==FALSE)
        	return FALSE;
    }
 	return TRUE;
}

Bool i2c_BurstRead(BYTE count, BYTE * buffer)
{ 	BYTE i;

  	for (i=0; i<count-1; i++)
    	*(buffer+i)=i2c_ReceiveByte(1);
  	*(buffer+i)=i2c_ReceiveByte(0);

  	i2c_Stop();
  
  	return TRUE;
}

Bool i2c_MasterStart(I2C_Direction direct, BYTE addr)
{ 	BYTE retry=5;

  	if (direct==I2C_READ) 
    	addr|=BIT0;
  	else
    	addr&=~BIT0;
  	
  	while (retry--)
    { 	if (i2c_Start()==FALSE)
        	continue;
  	
      	if (i2c_SendByte(addr)==TRUE) 
        	return TRUE;
      	
      	i2c_Stop();
      	Delay1ms(1);
    }
  	
  	return FALSE;
}


void i2c_WriteTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count)
{ 	BYTE slaveAddr;
  	BYTE wordAddr;
  	BYTE offset=0;
  	BYTE tempSize;
  	Bool succ=TRUE;
  	
  	while (count)
    { 	slaveAddr=i2cSlaveAddr(deviceID, addr+offset);
      	wordAddr=i2cWordAddr(addr+offset);
      	if (i2c_MasterStart(I2C_WRITE, slaveAddr)==FALSE)
      		break;
      	if (i2c_BurstWrite(1, &wordAddr)==FALSE)
      		break;
      	tempSize=0x10-(wordAddr&0xF);
      	if (count>tempSize)
      	{ 	if (i2c_BurstWrite(tempSize, (BYTE*)(buffer+offset))==FALSE)
      	    	break;
      	  	i2c_Stop();
      	  	count-=tempSize;
      	  	offset+=tempSize;
      	}
      	else if (count>0x10)
      	{ 	if (i2c_BurstWrite(0x10, (BYTE*)(buffer+offset))==FALSE)
            	break;
      	  	i2c_Stop();
      	  	count-=0x10;
      	  	offset+=0x10;
      	}
      	else
      	{ 	if (i2c_BurstWrite(count, (BYTE*)(buffer+offset))==FALSE)
      	    	break;
      	  	i2c_Stop();
          	count=0;
      	}
#if 0//ImproveDDCCISpeed
            if(BurstDataSaveDDCCIFlag)
            {
            BurstDataSaveDDCCIFlag=0;
            ForceDelay1ms(5);
            }
            else
#endif
            ForceDelay1ms(10);
            
    }
  	i2c_Stop();
}

void i2c_ReadTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count)
{
    WORD retry = 10;
    BYTE slaveAddr;
    BYTE wordAddr;

    slaveAddr = i2cSlaveAddr( deviceID, addr );
    wordAddr = i2cWordAddr( addr );
    while ( retry-- )
    {
        if ( i2c_MasterStart( I2C_WRITE, slaveAddr ) == FALSE )
        {
            Delay1ms( 2 );
            i2c_Stop();
            continue;
        }
        if ( i2c_BurstWrite( 1, &wordAddr ) == FALSE )
      		continue;
      	if (i2c_MasterStart(I2C_READ, slaveAddr)==FALSE)
        	continue;
      	if (i2c_BurstRead(count, buffer)==FALSE)
      		continue;      
      	return;
    }  
}

#if (defined(UseVGACableReadWriteAllPortsEDID)&&(!defined(ReduceDDC)))
#define	PageWriteLength		((LoadEDIDSelectPort<EEPROM)?8:16)
#define	PageWriteDelayTime		((LoadEDIDSelectPort<EEPROM)?3:12) //edit by lizzie,100429.for the problem: use HDMI/VGA Cable write all port EDID ,the time to write 32byte is too long
//ATMEL 24C02 Page Write Buffer is 8 Bytes.
//MicroChip 24C02 Page Write Buffer is 16 Bytes.
//ST 24C02 Page Write Buffer is 16 Bytes.
#else
#define	PageWriteLength		16
#define	PageWriteDelayTime	12 //edit by lizzie,100429.for the problem: use HDMI/VGA Cable write all port EDID ,the time to write 32byte is too long
#endif
#if (defined(UseVGACableReadWriteAllPortsEDID)&&!defined(ReduceDDC)) 
void i2c_WriteTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count)
{ BYTE slaveAddr;
  BYTE wordAddr;
  BYTE offset=0;
  BYTE tempSize;
  Bool succ=TRUE;
  
  while (count)
    { slaveAddr=i2cSlaveAddr(deviceID, addr+offset);
      wordAddr=i2cWordAddr(addr+offset);
      if (i2c_MasterStart(I2C_WRITE, slaveAddr)==FALSE)
      	break;
      if (i2c_BurstWrite(1, &wordAddr)==FALSE)
      	break;
      tempSize=PageWriteLength-(wordAddr&(PageWriteLength-1));//0x10-(wordAddr&0xF);
      if (count>tempSize)
      	{ if (i2c_BurstWrite(tempSize, (BYTE*)(buffer+offset))==FALSE)
      	    break;
      	  i2c_Stop();
      	  count-=tempSize;
      	  offset+=tempSize;
      	}
      else if (count>PageWriteLength)//0x10)
      	{ if (i2c_BurstWrite(0x10, (BYTE*)(buffer+offset))==FALSE)
            break;
      	  i2c_Stop();
      	  count-=PageWriteLength;//0x10;
      	  offset+=PageWriteLength;//0x10;
      	}
      else
      	{ if (i2c_BurstWrite(count, (BYTE*)(buffer+offset))==FALSE)
      	    break;
      	  i2c_Stop();
          count=0;
      	}
      ForceDelay1ms(PageWriteDelayTime);//edit by lizzie,100429.for the problem: use HDMI/VGA Cable write all port EDID ,the time to write 32byte is too long
    }
  i2c_Stop();
}

void i2c_ReadTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count)
{  
  #if Enable_Cache    // jeff 12 22 for cache add it 
  WORD retry=100;

  #else
   WORD retry=15;
  #endif
  BYTE slaveAddr;
  BYTE wordAddr;

  slaveAddr=i2cSlaveAddr(deviceID, addr);
  wordAddr=i2cWordAddr(addr);
  while (retry--)
    { if (i2c_MasterStart(I2C_WRITE, slaveAddr)==FALSE)
        continue;
      if (i2c_BurstWrite(1, &wordAddr)==FALSE)
      	continue;
      if (i2c_MasterStart(I2C_READ, slaveAddr)==FALSE)
        continue;
      if (i2c_BurstRead(count, buffer)==FALSE)
      	continue;      
      return;
    }  
}
#endif
//2004/02/20 num4--Start
#if !USEFLASH
void i2C_Intial(void)
{
  BYTE i;
 
  Set_i2c_SDA();
  Delay4us();
  Set_i2c_SCL();
  Delay4us();
 for(i=0;i<10;i++)
   {
     Set_i2c_SCL();
     Delay4us();
     Clr_i2c_SDA();
     Delay4us();
     Clr_i2c_SCL();
     Delay4us();
     Set_i2c_SCL();
     Delay4us();
     Set_i2c_SDA();
     Delay4us();
   }
  while(i++ <100)
   {
      if(hwI2C_SDA_Pin)
       break;
 
   Set_i2c_SCL();
   Delay4us();
   Clr_i2c_SDA();
   Delay4us();
   Clr_i2c_SCL();
   Delay4us();
   Set_i2c_SCL();
   Delay4us();
   Set_i2c_SDA();
    Delay4us();
   

  	}
}
#endif
#endif


#if Enable_Lightsensor
extern mStar_AdjustLightSensorBrightness(void);

//========================================================
#define  LIGHTSENSOR_SLAVEADDRESS 0x39
#define LIGHTSENSOR_DEVICE (LIGHTSENSOR_SLAVEADDRESS<<1)
//------------------Command----------------
#if LightSensor_Source==EverLight

#define LS_PowerDown 0x80
#define LS_PowerUp      0x04
#define LS_StartIntergration      0x08
#define LS_StopIntergration      0x30

#else

#define LS_PowerDown 0x00
#define LS_PowerUp      0x03
#if TSL2561
#define Read_LS_CH1_LowByte    0x8c
#define Read_LS_CH1_HighByte    0x8d
#define Read_LS_CH2_LowByte    0x8e
#define Read_LS_CH2_HighByte    0x8f

#define Read_LS_CH1_Word    0xac
#define Read_LS_CH2_Word    0xae
#else
#define Read_LS_CH1    0x43
#define Read_LS_CH2    0x83
#define Read_LS_Valid_Bit	BIT7		//fido 20070317+
#endif

#endif
//-------------------------------------------
#if LightSensorUse2Channel
// Oct/29/2004 Alex Liu. Update the caculate-lux function from TAOS.
// Lookup table for channel ratio (i.e. channel1 / channel0)
code BYTE ratioLut[129] = {
100,100,100,100,100,100,100,100,
100,100,100,100,100,100,99,99,
99,99,99,99,99,99,99,99,
99,99,99,98,98,98,98,98,
98,98,97,97,97,97,97,96,
96,96,96,95,95,95,94,94,
93,93,93,92,92,91,91,90,
89,89,88,87,87,86,85,84,
83,82,81,80,79,78,77,75,
74,73,71,69,68,66,64,62,
60,58,56,54,52,49,47,44,
42,41,40,40,39,39,38,38,
37,37,37,36,36,36,35,35,
35,35,34,34,34,34,33,33,
33,33,32,32,32,32,32,31,
31,31,31,31,30,30,30,30,
30
};

#define kMaxLux  1846	// standard mode max	

// Lookup table to convert channel values to counts
code WORD countLut[128] = {
   0,   1,   2,   3,   4,   5,   6,   7,
   8,   9,  10,  11,  12,  13,  14,  15,
  16,  18,  20,  22,  24,  26,  28,  30,
  32,  34,  36,  38,  40,  42,  44,  46,
  49,  53,  57,  61,  65,  69,  73,  77,
  81,  85,  89,  93,  97, 101, 105, 109,
 115, 123, 131, 139, 147, 155, 163, 171,
 179, 187, 195, 203, 211, 219, 227, 235,
 247, 263, 279, 295, 311, 327, 343, 359,
 375, 391, 407, 423, 439, 455, 471, 487,
 511, 543, 575, 607, 639, 671, 703, 735,
 767, 799, 831, 863, 895, 927, 959, 991,
1039,1103,1167,1231,1295,1359,1423,1487,
1551,1615,1679,1743,1807,1871,1935,1999,
2095,2223,2351,2479,2607,2735,2863,2991,
3119,3247,3375,3503,3631,3759,3887,4015
};
#endif

#if LightSensor_Source == EverLight
WORD xdata LSch1Buffer;
#else
    #if TSL2561
    WORD xdata LSch1Buffer;
    WORD xdata LSch2Buffer;
    #else
    BYTE LSch1Buffer;
    #if LightSensorUse2Channel	//fido 20070920+
    BYTE LSch2Buffer;
    #endif
    #endif
#endif


#if LightSensor_Source == EverLight
void i2c_LightSensor_Write(BYTE Command)
{
    if(i2c_MasterStart(I2C_WRITE, LIGHTSENSOR_DEVICE) == FALSE)
    {
        i2c_Stop();
        return;
    }

    i2c_SendByte(Command);
    i2c_Stop();
}
#else
    #if TSL2561
    void i2c_LightSensor_Write(BYTE Command, BYTE value)
    {
        if(i2c_MasterStart(I2C_WRITE, LIGHTSENSOR_DEVICE) == FALSE)
        {
            i2c_Stop();
            return;
        }

        i2c_SendByte(Command);
        i2c_SendByte(value);
        i2c_Stop();
    }
    #else
    void i2c_LightSensor_Write(BYTE Command)
    {
        if(i2c_MasterStart(I2C_WRITE, LIGHTSENSOR_DEVICE) == FALSE)
        {
            i2c_Stop();
            return;
        }

        i2c_SendByte(Command);
        i2c_Stop();
    }
    #endif
#endif

#if LightSensor_Source == EverLight
Bool i2c_LightSensor_Read(BYTE *buffer)
{ 
    if(i2c_MasterStart(I2C_WRITE, LIGHTSENSOR_DEVICE) == FALSE)
    {
        i2c_Stop();
        return FALSE;
    }

    i2c_Stop();

    if(i2c_MasterStart(I2C_READ, LIGHTSENSOR_DEVICE) == FALSE)
    {
        i2c_Stop();
        return FALSE;
    }

    *buffer = i2c_ReceiveByte(1);
    *(buffer+1) = i2c_ReceiveByte(0);
    
    i2c_Stop();
    
    return TRUE;
    
}
#else
Bool i2c_LightSensor_Read(BYTE Command, BYTE *buffer)
{ 
    if(i2c_MasterStart(I2C_WRITE, LIGHTSENSOR_DEVICE) == FALSE)
    {
        i2c_Stop();
        return FALSE;
    }

    if(i2c_SendByte(Command) == FALSE)
    {
        i2c_Stop();
        return FALSE;
    }

    i2c_Stop();

    if(i2c_MasterStart(I2C_READ, LIGHTSENSOR_DEVICE) == FALSE)
    {
        i2c_Stop();
        return FALSE;
    }

    #if TSL2561
    *buffer = i2c_ReceiveByte(1);
    *(buffer+1) = i2c_ReceiveByte(0);
    #else
    *buffer = i2c_ReceiveByte(0);
    #endif
    
    i2c_Stop();
    
    return TRUE;
    
}
#endif

void  LightSensor_On(void)
{
#if LightSensor_Source == EverLight
    i2c_LightSensor_Write(LS_PowerUp);
    i2c_LightSensor_Write(LS_StartIntergration);
    SensorErrorCnt = 0;
#else
    #if TSL2561
    i2c_LightSensor_Write(0x80, LS_PowerUp);
    i2c_LightSensor_Write(0x81, 0x12); // polling interval 402mS, gain = 16
    SensorErrorCnt = 0;
    #else
    i2c_LightSensor_Write(LS_PowerUp);
    #endif
#endif
    Clr_DetectBriFlag();
}
void  LightSensor_Off(void)
{
#if LightSensor_Source == EverLight
    i2c_LightSensor_Write(LS_PowerDown);
#else
    #if TSL2561
    i2c_LightSensor_Write(0x80, LS_PowerDown);
    #else
    i2c_LightSensor_Write(LS_PowerDown);
    #endif
#endif
}

#define  CHORD_ch1_num ((LSch1Buffer&0x70)>>4)
#define  STEP_ch1_num    (LSch1Buffer&0x0F)
#define  STEP_Ch1_Value (0x01<<CHORD_ch1_num)
#define  CHORD_Ch1_Value ((WORD)165*(STEP_Ch1_Value-1))/10
#define  Count_Ch1_Value CHORD_Ch1_Value+((WORD)STEP_ch1_num*STEP_Ch1_Value)

#define  CHORD_ch2_num ((LSch2Buffer&0x70)>>4)
#define  STEP_ch2_num    (LSch2Buffer&0x0F)
#define  STEP_Ch2_Value (0x01<<CHORD_ch2_num)
#define  CHORD_Ch2_Value ((WORD)165*(STEP_Ch2_Value-1))/10
#define  Count_Ch2_Value CHORD_Ch2_Value+((WORD)STEP_ch2_num*STEP_Ch2_Value)

#define  MaxSM_BriValue 255
#define  MinSM_BriValue  130

#if LightSensor_Source == EverLight
#if 1 // 090610
//#define ErrorLux    0 // invisiable light is stronger than visiable light
#define MinLux      0
#define MaxLux     27//5001
#define MinADCValue	4
#define MaxADCValue	164//30000//32767
#else
#define ErrorLux    0 // invisiable light is stronger than visiable light
#define MinLux      1
#define MaxLux     5001
#define MinADCValue	1
#define MaxADCValue	30001//32767
#endif
// Calculate Ambient Brightness
WORD LightSensor_CalculateLux(WORD ch0)
{
    ch0 = (ch0 >= MaxADCValue)?(MaxLux):((DWORD)ch0*17/100);
    
    if(ch0 > MaxLux)
        ch0 = MaxLux;
    
    return ch0;
}
#else

#if TSL2561
#define ErrorLux    0 // invisiable light is stronger than visiable light

#define MaxCh0_6000   0
#define MaxCh0_10000 1
#define MaxCh0_20000 2
#define MaxCh0_30000 3
#define MaxCh0_40000 4
#define MaxCh0_50000 5
#define MaxCh0_60000 6

// use this define to select max. ADC range
// ex: select MaxCh0_6000, it means if ch0 reach 6000 or higher, it outputs max. lux
#define DefineMaxCh0   MaxCh0_6000

#if DefineMaxCh0 == MaxCh0_6000
#define MinCh0      300
#define MaxCh0     6000

#define MinLux      6
#define MaxLux     156
#else
#define Ch0Step    295

#define MinCh0      300
#define MaxCh0     (DefineMaxCh0*10000)

#define MinLux      5
#define MaxLux     (DefineMaxCh0*Ch0Step)
#endif

// Calculate Ambient Brightness
WORD LightSensor_CalculateLux(WORD ch0, WORD ch1)
{
    if(ch1 >= ch0) // invisiable light is stronger than visiable light
        return ErrorLux;
    else if(ch0 <= MinCh0)
        return MinLux;
    else if(ch0 >= MaxCh0)
        return MaxLux;
    else //if(ch0 > ch1)
    {
        WORD Rvalue = (DWORD)ch1*1000 / ch0; // 0~1.3x1000 = 0~1300

        if(Rvalue >= 1300)
            ch0 = ErrorLux;
        else if(Rvalue < 125) 
            ch0 = (DWORD)ch0*304/10000-(DWORD)ch1*272/10000;
        else if(Rvalue < 250)
            ch0 = (DWORD)ch0*325/10000-(DWORD)ch1*440/10000;
        else if(Rvalue < 375)
            ch0 = (DWORD)ch0*351/10000-(DWORD)ch1*544/10000;
        else if(Rvalue < 500)
            ch0 = (DWORD)ch0*381/10000-(DWORD)ch1*624/10000;
        else if(Rvalue < 610)
            ch0 = (DWORD)ch0*224/10000-(DWORD)ch1*310/10000;
        else if(Rvalue < 800)
            ch0 = (DWORD)ch0*128/10000-(DWORD)ch1*153/10000;
        else //if(Rvalue < 1300)
            ch0 = (DWORD)ch0*146/100000-(DWORD)ch1*112/100000; // note for /100000 is correct

        if(ch0 > MaxLux)
            ch0 = MaxLux;

        return ch0;
    }
    
}

#else
    #if LightSensorUse2Channel
// Calculate Ambient Brightness
WORD LightSensor_CalculateLux(WORD ch0, WORD ch1)
{
	WORD count0, count1, lux;
	BYTE ratio = 128;		// Note: the "128" is a scaling factor
	
	// lookup count from channel value
	count0 = countLut[ch0 & 0x7F];
	count1 = countLut[ch1 & 0x7F];

	//msg("count0 = %d",count0);
	//msg("count1 = %d",count1);

	// calculate ratio
	// avoid division by zero
	// and count1 cannot be greater than count0
	if ((count0) && (count1 <= count0)) 
		ratio = (BYTE)((DWORD)count1 * 128 / count0);

	//msg("ratio = %d", ratio);
	//msg("ratioLut[ratio] = %d", ratioLut[ratio]);
	// calculate lux
	// Note: the "256" is a scaling factor
	lux = (WORD)(((DWORD)(count0 - count1) * ratioLut[ratio]) >> 8); // / 256;
			
	//if( (count0 == 4015) && (count1 == 4015) )
	//	lux = kMaxLux;

	
	
	// range check lux
	if(lux > kMaxLux) lux = kMaxLux;

	return lux;
}
    #endif
#endif
#endif

void LightSensorHandler(void)
{
    #define BrightnessMin		MinBrightnessValue
    #define BrightnessMax		MaxBrightnessValue

    #if LightSensor_Source == EverLight
        #define ErrorCounter        2
        BYTE tempBuffer[2];
    #else
        #if TSL2561
        #define ErrorCounter        2
        BYTE tempBuffer[2];
        #else
        #define wLightSensorADCMin	LIGHT_SENSOR_ADC_MIN
        #define wLightSensorADCMax	LIGHT_SENSOR_ADC_MAX
        WORD wLightSensorADC;
        WORD wSetBriValue;	//fido 20070614+
        #endif    
    #endif

	if( DetectBriFlag )
	{
#if LightSensor_Source == EverLight
            LSch1Buffer = 0;

    #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   ===========", 1);
    #endif

            i2c_LightSensor_Write(LS_StopIntergration);
    
            if(i2c_LightSensor_Read(&tempBuffer) == FALSE)
            {
    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   ReadLightSensorError1", 1);
    #endif

                SensorErrorCnt ++;
    
                goto SensorError;
            }

            if(!(tempBuffer[1]&BIT7)) // data is invalid
            {
                SensorErrorCnt ++;
    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LightSensorDataInvalid", 1);
    #endif
                goto SensorError;
            }

            // 15-bit ADC
            tempBuffer[1] &= 0x7f;
            LSch1Buffer = (WORD)tempBuffer[1]*256+tempBuffer[0];
            LSch1Buffer = (DWORD)LSch1Buffer*35/10;

            i2c_LightSensor_Write(LS_StartIntergration);

            Clr_DetectBriFlag();
/* // 090610 reserved for cannot ident shut down mode or ADC return 0            
            if(LSch1Buffer == 0) // power off
            {
                SensorErrorCnt ++;
    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LightSensorDatais0", 1);
    #endif
                goto SensorError;
            }
*/                
    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LSch1Buffer:%d", LSch1Buffer);
    #endif

	    
            if(abs(LSch1Buffer- 48424)<50) // data is invalid
            {
                SensorErrorCnt ++;
    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LightSensor_For_ESD", 1);
    #endif
                goto SensorError;
            }

            LightSensorAdcOld = LightSensorAdcNew;
		
            // record current lux
#if 1 // 090610
            LightSensorAdcNew = LightSensor_CalculateLux(LSch1Buffer);								
#else
            if((LightSensorAdcNew = LightSensor_CalculateLux(LSch1Buffer)) == ErrorLux)
            {
    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   xxxInvisiableLightxxx", 1);
    #endif
                return;
            }
#endif
            if(abs( LightSensorAdcOld - LightSensorAdcNew ) > LIGHT_SENSOR_ERROR_RANGE)
            {
    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LIGHT_SENSOR_ERROR_RANGE:%d", abs( LightSensorAdcOld - LightSensorAdcNew ));
    #endif
                return;
            }

    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   CurrentLux:%d", LightSensorAdcNew);
    #endif
            
            SetBriValue= (DWORD)(BrightnessMax-BrightnessMin)*
                                                (LightSensorAdcNew-MinLux)/(MaxLux-MinLux);

    #if DEBUG_ENABLE && DEBUG_LightSensor
                //printData("   LSch1Buffer:%d", LSch1Buffer);
                //printData("   LSch2Buffer:%d", LSch2Buffer);
                //printData("   Lux:%d", LightSensor_CalculateLux(LSch1Buffer, LSch2Buffer));
                printData("   SetBriValue:%d", SetBriValue);
                //printData("   ===========", 1);
    #endif

    SensorError:
                    if(SensorErrorCnt > ErrorCounter)
                    {
    #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LightSensorReadError", 1);
    #endif
                        LightSensor_Off();
                        LightSensor_On();
                        SensorErrorCnt = 0;
                    }
                    return;
#else

    #if TSL2561
            LSch1Buffer = 0;
            LSch2Buffer = 0;

        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   ===========", 1);
        #endif

            if(i2c_LightSensor_Read(Read_LS_CH1_Word, &tempBuffer) == FALSE)
            {
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   ReadLightSensorError1", 1);
        #endif

                SensorErrorCnt ++;
    
                goto SensorError;
            }
            
            LSch1Buffer = (WORD)tempBuffer[1]*256+tempBuffer[0];

            if(i2c_LightSensor_Read(Read_LS_CH2_Word, &tempBuffer) == FALSE)
            {
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   ReadLightSensorError2", 1);
        #endif

                SensorErrorCnt ++;
    
                goto SensorError;
            }
            
            LSch2Buffer = (WORD)tempBuffer[1]*256+tempBuffer[0];

            Clr_DetectBriFlag();

            if(LSch1Buffer == 0 && LSch2Buffer == 0) // power off
            {
                SensorErrorCnt ++;
                goto SensorError;
            }
            
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LSch1Buffer:%d", LSch1Buffer);
                printData("   LSch2Buffer:%d", LSch2Buffer);
        #endif

            LightSensorAdcOld = LightSensorAdcNew;

            // record current lux
            if((LightSensorAdcNew = LightSensor_CalculateLux(LSch1Buffer, LSch2Buffer)) == ErrorLux)
            {
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   xxxInvisiableLightxxx", 1);
        #endif
                return;
            }

            if(abs( LightSensorAdcOld - LightSensorAdcNew ) > LIGHT_SENSOR_ERROR_RANGE)
            {
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LIGHT_SENSOR_ERROR_RANGE:%d", abs( LightSensorAdcOld - LightSensorAdcNew ));
        #endif
                return;
            }

        #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   CurrentLux:%d", LightSensorAdcNew);
        #endif

            
            
            SetBriValue= (DWORD)(BrightnessMax-BrightnessMin)*
                                                (LightSensorAdcNew-MinLux)/(MaxLux-MinLux);

        #if DEBUG_ENABLE && DEBUG_LightSensor
                //printData("   LSch1Buffer:%d", LSch1Buffer);
                //printData("   LSch2Buffer:%d", LSch2Buffer);
                //printData("   Lux:%d", LightSensor_CalculateLux(LSch1Buffer, LSch2Buffer));
                printData("   SetBriValue:%d", SetBriValue);
                //printData("   ===========", 1);
        #endif

    SensorError:
                    if(SensorErrorCnt > ErrorCounter)
                    {
                        if(i2c_LightSensor_Read(0x80, &tempBuffer) == TRUE)
                        {
                            if((tempBuffer[0]&0x03) == 0x00)
                            {
                                LightSensor_On();
                            }
                            else
                            {
                                LightSensor_Off();
                                LightSensor_On();
                            }

                            SensorErrorCnt = 0;
                            
                        }
                    }
                    return;
            
    #else

        #if LightSensorUse2Channel
		LSch1Buffer = 0;
		LSch2Buffer = 0;
		i2c_LightSensor_Read(Read_LS_CH1, &LSch1Buffer);
		i2c_LightSensor_Read(Read_LS_CH2, &LSch2Buffer);
		if( (LSch1Buffer & Read_LS_Valid_Bit) && (LSch2Buffer & Read_LS_Valid_Bit) )		// if data is validable
        #else

                Clr_DetectBriFlag();
    
                if(i2c_LightSensor_Read(Read_LS_CH1, &LSch1Buffer) == FALSE)
                {
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   ReadLightSensorError", 1);
        #endif
                    return;
                }
		
		if( LSch1Buffer & Read_LS_Valid_Bit )		// if data is validable
        #endif
		{
			LightSensorAdcOld = LightSensorAdcNew;

        #if LightSensorUse2Channel
			LightSensorAdcNew = LightSensor_CalculateLux(LSch1Buffer, LSch2Buffer);
        #else
			LightSensorAdcNew = Count_Ch1_Value;
        #endif

                    if( abs( LightSensorAdcOld - LightSensorAdcNew ) > LIGHT_SENSOR_ERROR_RANGE )
                    {
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   OverErrorRange_OER: %x", abs( LightSensorAdcOld - LightSensorAdcNew ));
        #endif
                        return;
                    }

        #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   CurrentLux:%d", LightSensorAdcNew);
        #endif
            
			wLightSensorADC = ( ( LightSensorAdcOld + LightSensorAdcNew ) >> 1 ) + LIGHT_SENSOR_ADC_MIN;

                    if( ( wLightSensorADC & BIT15 ) != 0 )
                    {
                        wLightSensorADC = 0;
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   wLightSensorADC = 0", 1);
        #endif
                    }
            
			wSetBriValue = ( ( (DWORD)( BrightnessMax - BrightnessMin )
							* ( wLightSensorADC - wLightSensorADCMin ) )
							/ ( wLightSensorADCMax - wLightSensorADCMin ) ) + BrightnessMin;
            
			SetBriValue = ( ( wSetBriValue > 100 ) ? 100 : ( (BYTE)wSetBriValue ) );

        #if DEBUG_ENABLE && DEBUG_LightSensor
                printData("   SetBriValue:%d", SetBriValue);
                //printData("   ===========", 1);
        #endif
            
		}
		else
		{
        #if 0//DEBUG_ENABLE && DEBUG_LightSensor
                printData("   LightSensorReadError", 1);
        #endif
			// light sensor read error
			return;	
		}

    #endif        
#endif    
	}

    if(bLightSensorActStepFlag)
    {
        mStar_AdjustLightSensorBrightness(); // 090417
        bLightSensorActStepFlag = 0;
    }

}
#endif


#if IT6251
#define _1PORT_
#define _Demo_2RX_Only_

#define _ENMULTICH_

//#define DVI2DP

#define ENA_EDID_UPDATE

#ifndef DVI2DP
#define ENA_DP_AUDIO
#endif

///////////////////////////////////////////////////////////////////////////////
// Constant Definition
///////////////////////////////////////////////////////////////////////////////
#define TX0DEV			0
#define TX1DEV			1
#define TX2DEV			2
#define TX3DEV			3
#define TX4DEV			4
#define TX5DEV			5
#define TX6DEV			6
#define TX7DEV			7
#define RXDEV			8
#define EDID0DEV        9
#define EDID1DEV        10

#define IT6505DEV		0
//#define RXDEV			8

#define RXADR			0x90

#ifdef _1PORT_

#define IT6505ADR	0xB8		 // Clive


#endif

#ifdef _2PORT_
	    #define TX0ADR	0x98
        #define TX1ADR	0x9A
#endif

#ifdef _3PORT_
        #define TX0ADR	0x98
	    #define TX1ADR	0x9A
    	#define TX2ADR	0x98
#endif

#ifdef _4PORT_
        #define TX0ADR	0x98
	    #define TX1ADR	0x9A
    	#define TX2ADR	0x98
    	#define TX3ADR	0x9A
#endif

#ifdef _8PORT_
	#define TX0ADR		0x98
    #define TX1ADR		0x9A
    #define TX2ADR		0x98
    #define TX3ADR		0x9A
    #define TX4ADR		0x98
    #define TX5ADR		0x9A
    #define TX6ADR		0x98
    #define TX7ADR		0x9A
#endif





#define HIGH			1
#define LOW				0

#ifndef NULL
#define NULL 0
#endif

///////////////////////////////////////////////////////////////////////////////
// 8051 Definition
///////////////////////////////////////////////////////////////////////////////



#ifdef _1PORT_
    #define DevNum	1
	#define LOOPMS	20

    //#define TX0_SDA_PORT	( MCU_PORT_PB5_PIN_REG_FF8A)
    //#define RX_SDA_PORT	(MCU_PORT_PB5_PIN_REG_FF8A)
    //#define SCL_PORT		(MCU_PORT_PB6_PIN_REG_FF89)

	sbit TX0_SDA_PORT 	= P1^1;
	sbit RX_SDA_PORT 	= P1^1;
	sbit SCL_PORT 		= P1^0;
	//sbit HW_RSTN		= P3^5;

	sbit EDID_SCL 		= P1^3;
	sbit EDID_SDA 		= P1^2;
	sbit EDID1_SCL 		= P1^7;
	sbit EDID1_SDA 		= P1^4;



#endif


#ifdef _2PORT_
    #define DevNum	2
	#define LOOPMS	20
        sbit TX0_SDA_PORT	= P1^4;
        sbit TX1_SDA_PORT	= P1^4;
    	sbit RX_SDA_PORT	= P1^4;
    	sbit SCL_PORT		= P0^2;
#endif

#ifdef _3PORT_
    #define DevNum	3
	#define LOOPMS	30
        sbit TX0_SDA_PORT	= P1^4;
        sbit TX1_SDA_PORT	= P1^4;
	    sbit TX2_SDA_PORT	= P0^5;
    	sbit RX_SDA_PORT	= P1^4;
    	sbit SCL_PORT		= P0^2;
#endif

#ifdef _4PORT_
    #define DevNum	4
	#define LOOPMS	40
	#ifdef Unixtar
        sbit TX0_SDA_PORT	= P0^6;
        sbit TX1_SDA_PORT	= P0^6;
	    sbit TX2_SDA_PORT	= P1^7;
	    sbit TX3_SDA_PORT	= P1^7;
    	sbit RX_SDA_PORT	= P0^6;
	#else
        sbit TX0_SDA_PORT	= P1^4;
        sbit TX1_SDA_PORT	= P1^4;
	    sbit TX2_SDA_PORT	= P0^5;
	    sbit TX3_SDA_PORT	= P0^5;
    	sbit RX_SDA_PORT	= P1^4;
	#endif
    	sbit SCL_PORT		= P0^2;
#endif

#ifdef _8PORT_
    #define DevNum	8
	#define LOOPMS	80
        sbit TX0_SDA_PORT	= P1^4;
        sbit TX1_SDA_PORT	= P1^4;
	    sbit TX2_SDA_PORT	= P0^5;
	    sbit TX3_SDA_PORT	= P0^5;
        sbit TX4_SDA_PORT	= P0^6;
        sbit TX5_SDA_PORT	= P0^6;
	    sbit TX6_SDA_PORT	= P1^7;
	    sbit TX7_SDA_PORT	= P1^7;
    	sbit RX_SDA_PORT	= P1^4;
    	sbit SCL_PORT		= P0^2;
#endif

//sbit RX_LED		= P4^0;

#ifdef _1PORT_
    sbit TX0_LED	= P2^0;
#endif

#ifdef _2PORT_
    sbit TX0_LED	= P2^0;
    sbit TX1_LED	= P2^1;
#endif

#ifdef _3PORT_
    sbit TX0_LED	= P2^0;
    sbit TX1_LED	= P2^1;
    sbit TX2_LED	= P2^2;
#endif

#ifdef _4PORT_
    sbit TX0_LED	= P2^0;
    sbit TX1_LED	= P2^1;
    sbit TX2_LED	= P2^2;
    sbit TX3_LED	= P2^3;
#endif

#ifdef _8PORT_
    sbit TX0_LED	= P2^0;
    sbit TX1_LED	= P2^1;
    sbit TX2_LED	= P2^2;
    sbit TX3_LED	= P2^3;
    sbit TX4_LED	= P2^4;
    sbit TX5_LED	= P2^5;
    sbit TX6_LED	= P2^6;
    sbit TX7_LED	= P2^7;
#endif

    sbit TX0_INT    = P2^1 ;

void set_8051_scl(BYTE bit_value,BYTE device  )
{
	switch( device )
	{

	case EDID0DEV:
		 EDID_SCL  =   bit_value;
		 break;

	case EDID1DEV:
	     EDID1_SCL = bit_value;
		 break;
	default:
     	 SCL_PORT = bit_value;
    }
}

void set_8051_sda(BYTE bit_value, BYTE device )
{
     switch( device ) {
     	case 0:
     	    TX0_SDA_PORT = bit_value;
            break;
		case EDID0DEV:
		 	EDID_SDA =   bit_value;
		 	break;

		case EDID1DEV:
	     	EDID1_SDA = bit_value;
		 	break;

#ifdef _2PORT_
     	case 1:
     	    TX1_SDA_PORT = bit_value;
            break;
#endif

#ifdef _3PORT_
     	case 1:
     	    TX1_SDA_PORT = bit_value;
            break;
     	case 2:
     	    TX2_SDA_PORT = bit_value;
            break;
#endif

#ifdef _4PORT_
     	case 1:
     	    TX1_SDA_PORT = bit_value;
            break;
     	case 2:
     	    TX2_SDA_PORT = bit_value;
            break;
     	case 3:
     	    TX3_SDA_PORT = bit_value;
            break;
#endif

#ifdef _8PORT_
     	case 1:
     	    TX1_SDA_PORT = bit_value;
            break;
     	case 2:
     	    TX2_SDA_PORT = bit_value;
            break;
     	case 3:
     	    TX3_SDA_PORT = bit_value;
            break;
     	case 4:
     	    TX4_SDA_PORT = bit_value;
            break;
     	case 5:
     	    TX5_SDA_PORT = bit_value;
            break;
     	case 6:
     	    TX6_SDA_PORT = bit_value;
            break;
     	case 7:
     	    TX7_SDA_PORT = bit_value;
            break;
#endif

     	default:
     	    RX_SDA_PORT = bit_value;
     }
//	DelayUS(DELAY_TIME);
}

BYTE get_8051_sda( BYTE device )
{
     switch( device ) {
     	case 0:
     	    TX0_SDA_PORT = 1;
     	    return TX0_SDA_PORT;
            break;

#ifdef _2PORT_
     	case 1:
     	    TX1_SDA_PORT = 1;
     	    return TX1_SDA_PORT;
            break;
#endif

#ifdef _3PORT_
     	case 1:
     	    TX1_SDA_PORT = 1;
     	    return TX1_SDA_PORT;
            break;
     	case 2:
     	    TX2_SDA_PORT = 1;
     	    return TX2_SDA_PORT;
            break;
#endif

#ifdef _4PORT_
     	case 1:
     	    TX1_SDA_PORT = 1;
     	    return TX1_SDA_PORT;
            break;
     	case 2:
     	    TX2_SDA_PORT = 1;
     	    return TX2_SDA_PORT;
            break;
     	case 3:
     	    TX3_SDA_PORT = 1;
     	    return TX3_SDA_PORT;
            break;
#endif

#ifdef _8PORT_
     	case 1:
     	    TX1_SDA_PORT = 1;
     	    return TX1_SDA_PORT;
            break;
     	case 2:
     	    TX2_SDA_PORT = 1;
     	    return TX2_SDA_PORT;
            break;
     	case 3:
     	    TX3_SDA_PORT = 1;
     	    return TX3_SDA_PORT;
            break;
     	case 4:
     	    TX4_SDA_PORT = 1;
     	    return TX4_SDA_PORT;
            break;
     	case 5:
     	    TX5_SDA_PORT = 1;
     	    return TX5_SDA_PORT;
            break;
     	case 6:
     	    TX6_SDA_PORT = 1;
     	    return TX6_SDA_PORT;
            break;
     	case 7:
     	    TX7_SDA_PORT = 1;
     	    return TX7_SDA_PORT;
            break;
#endif

	    case EDID0DEV:
		   EDID_SDA =   1;
		   return EDID_SDA;
		   break;

	    case EDID1DEV:
	       EDID1_SDA = 1;
		   return  EDID1_SDA;
		   break;
     	default:
     	    RX_SDA_PORT = 1;
     	    return RX_SDA_PORT;
     }
}

void i2c_8051_start( BYTE device )
{
    set_8051_sda( HIGH, device );
    set_8051_scl( HIGH, device );
    set_8051_sda( LOW,  device );
    set_8051_scl( LOW,  device );
}

void i2c_8051_write( BYTE byte_data, BYTE device )
{
 BYTE data bit_cnt, tmp;
 BYTE data bit_value;

     for(bit_cnt=0; bit_cnt<8; bit_cnt++) {
         tmp = (byte_data << bit_cnt) & 0x80;
		 bit_value = tmp && 0x80;

         set_8051_sda( bit_value, device );
         set_8051_scl( HIGH , device );
         set_8051_scl( LOW , device );
     }
}

BYTE i2c_8051_wait_ack( BYTE device )
{
 BYTE data ack_bit_value;

    
    set_8051_scl( HIGH , device );
    //Delay5us(4);
    set_8051_sda( HIGH, device );
    //Delay5us(4);
    ack_bit_value = get_8051_sda( device );
    set_8051_scl( LOW , device );

    return ack_bit_value;
}

BYTE i2c_8051_read( BYTE device )
{
 BYTE data bit_cnt, byte_data;
 BYTE data bit_value;

     byte_data = 0;
     for(bit_cnt=0; bit_cnt<8; bit_cnt++) {
         set_8051_scl( HIGH , device );

	     bit_value = get_8051_sda( device );

	     byte_data = (byte_data << 1) | bit_value;

	     set_8051_scl( LOW, device  );
     }

     return byte_data;
}

void i2c_8051_send_ack( BYTE bit_value, BYTE device )
{
     set_8051_sda( bit_value, device );
     set_8051_scl( HIGH , device );
     set_8051_scl( LOW , device );
     set_8051_sda( HIGH, device );
}

void i2c_8051_end( BYTE device )
{
     set_8051_sda( LOW, device );
     set_8051_scl( HIGH , device );
     set_8051_sda( HIGH, device );
}

BYTE i2c_write_byte( BYTE address, BYTE offset, BYTE byteno, BYTE *p_data, BYTE device )
{
 BYTE data i;

     i2c_8051_start(device);				// S

     i2c_8051_write(address&0xFE, device);		// slave address (W)
     if( i2c_8051_wait_ack(device)==1 )	

    {		// As
         i2c_8051_end(device);
	 return 0;
      }

     i2c_8051_write(offset, device);			// offset
     if( i2c_8051_wait_ack(device)==1 )	
   {		// As
         i2c_8051_end(device);
	 return 0;
     }

     for(i=0; i<byteno-1; i++) 
	 {
     	// i2c_8051_write(, device);		// write d
     	   i2c_8051_write(*p_data, device);	
     	 if( i2c_8051_wait_ack(device)==1 ) {		// As
     	     i2c_8051_end(device);
	     return 0;
         }
         p_data++;
     }

     i2c_8051_write(*p_data, device);			// write last d
     if( i2c_8051_wait_ack(device)==1 )	
   {		// As
     	 i2c_8051_end(device);
	 return 0;
     }
     else 
   {
     	 i2c_8051_end(device);
	 return 1;
     }
}


BYTE i2c_read_byte( BYTE address, BYTE offset, BYTE byteno, BYTE *p_data, BYTE device )
{
 BYTE data i;

     i2c_8051_start(device);				// S

     i2c_8051_write(address&0xFE, device);		// slave address (W)
     if( i2c_8051_wait_ack(device)==1 ) {		// As
         i2c_8051_end(device);
         return 0;
     }

     i2c_8051_write(offset, device);			// offset
     if( i2c_8051_wait_ack(device)==1 ) {		// As
         i2c_8051_end(device);
         return 0;
     }

     i2c_8051_start(device);

     i2c_8051_write(address|0x01, device);		// slave address (R)
     if( i2c_8051_wait_ack(device)==1 ) {		// As
         i2c_8051_end(device);
         return 0;
     }

     for(i=0; i<byteno-1; i++) {
         *p_data = i2c_8051_read(device);		// read d
         i2c_8051_send_ack(LOW, device);		// Am

         p_data++;
     }

     *p_data = i2c_8051_read(device);			// read last d
     i2c_8051_send_ack(HIGH, device);			// NAm
     i2c_8051_end(device);

    return 1;
}
///////////////////////////////////////////////////////////////////////////////
// I2C for original function call
///////////////////////////////////////////////////////////////////////////////


BYTE ReadI2C_Byte(BYTE addr, BYTE offset)
{
    BYTE data p_data=0;

    i2c_read_byte(addr, offset, 1, &p_data, TX0DEV);

    return p_data;
}

BYTE WriteI2C_Byte(BYTE addr, BYTE offset, BYTE d)
{
    BYTE data flag;

    flag = i2c_write_byte(addr, offset, 1, &d, TX0DEV);

    return flag;
}
/*
#else

void i2c_WriteByte(BYTE deviceID, WORD addr, BYTE *value)
{ i2c_WriteTBL(deviceID, addr, value, 1);
}
void i2c_ReadByte(BYTE deviceID, WORD addr, BYTE *value)
{ i2c_ReadTBL(deviceID, addr, value, 1);
}


BYTE ReadI2C_Byte(BYTE addr, BYTE offset)
{
    BYTE data p_data=0;

    i2c_ReadByte(addr, offset,&p_data);

    return p_data;
}

BYTE WriteI2C_Byte(BYTE addr, BYTE offset, BYTE d)
{
    BYTE data flag=0;

    i2c_WriteByte(addr, offset, &d);

    return flag;
}

*/
#endif

