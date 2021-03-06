#include "types.h"
#include "ms_reg.h"
#include "ms_rwreg.h"
#include "Global.h"
#include "MsID_V1.h"
#include "UserPref.h"
extern void SetPWMFreq(WORD freq);
#define CheckDDCAddr1	0x10
#define CheckDDCAddr2	0x20
#if 1//ReduceDDC
#include "msflash.h"

#include "power.h"
#if Dual
	#ifdef OnlyVGAEDIDInFlash
	#define EnableDVIDDC	0
	#else
	#define EnableDVIDDC	1
	#endif
#else
	#define EnableDVIDDC	0
#endif
void mStar_WriteDDC(BYTE EDIDType);


#if EDID_215W
BYTE code tDDC[128]=
{ 
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x15,0xD6,0x18,0x22,0x00,0x00,0x00,0x00,
0x01,0x19,0x01,0x03,0x68,0x30,0x1B,0x78,
0x2A,0x9D,0x80,0xA2,0x59,0x54,0x9E,0x27,
0x0E,0x50,0x54,0xBF,0xEF,0x00,0xD1,0xC0,
0xB3,0x00,0x95,0x00,0x81,0x80,0x81,0x40,
0x81,0xC0,0x01,0x01,0x01,0x01,0x02,0x3A,
0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,
0x45,0x00,0xDD,0x0C,0x11,0x00,0x00,0x1E,
0x00,0x00,0x00,0xFD,0x00,0x32,0x4C,0x1E,
0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,
0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x4C,
0x45,0x44,0x20,0x32,0x32,0x31,0x38,0x57,
0x0A,0x20,0x20,0x20,0x00,0x00,0x00,0xFF,
0x00,0x45,0x42,0x31,0x38,0x35,0x30,0x30,
0x5A,0x30,0x30,0x30,0x30,0x30,0x00,0x1E,

};

BYTE code tDVIDDC[128]=
{
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x15,0xD6,0x18,0x22,0x00,0x00,0x00,0x00,
0x01,0x19,0x01,0x03,0x68,0x30,0x1B,0x78,
0x2A,0x9D,0x80,0xA2,0x59,0x54,0x9E,0x27,
0x0E,0x50,0x54,0xBF,0xEF,0x00,0xD1,0xC0,
0xB3,0x00,0x95,0x00,0x81,0x80,0x81,0x40,
0x81,0xC0,0x01,0x01,0x01,0x01,0x02,0x3A,
0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,
0x45,0x00,0xDD,0x0C,0x11,0x00,0x00,0x1E,
0x00,0x00,0x00,0xFD,0x00,0x32,0x4C,0x1E,
0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,
0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x4C,
0x45,0x44,0x20,0x32,0x32,0x31,0x38,0x57,
0x0A,0x20,0x20,0x20,0x00,0x00,0x00,0xFF,
0x00,0x45,0x45,0x42,0x31,0x38,0x34,0x30,
0x30,0x5A,0x30,0x30,0x30,0x30,0x00,0x0A,

};

#elif EDID_195W

BYTE code tDDC[128]=
{ 
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x15,0xD6,0x18,0x20,0x00,0x00,0x00,0x00,
0x01,0x19,0x01,0x03,0x68,0x2B,0x18,0x78,
0x2A,0x31,0x86,0xA8,0x52,0x46,0x97,0x23,
0x1B,0x4C,0x58,0xBF,0xEF,0x00,0xA9,0xC0,
0x95,0x00,0x81,0x80,0x81,0x40,0x81,0xC0,
0x01,0x01,0x01,0x01,0x01,0x01,0x2F,0x26,
0x40,0xA0,0x60,0x84,0x1A,0x30,0x30,0x20,
0x35,0x00,0xB2,0xEC,0x10,0x00,0x00,0x1E,
0x00,0x00,0x00,0xFD,0x00,0x32,0x4C,0x1E,
0x53,0x0E,0x00,0x0A,0x20,0x20,0x20,0x20,
0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x4C,
0x45,0x44,0x20,0x32,0x30,0x31,0x38,0x57,
0x53,0x0A,0x20,0x20,0x00,0x00,0x00,0xFF,
0x00,0x45,0x45,0x41,0x31,0x38,0x34,0x30,
0x30,0x5A,0x30,0x30,0x30,0x30,0x00,0x4A,

};

BYTE code tDVIDDC[128]=
{
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x15,0xD6,0x18,0x22,0x00,0x00,0x00,0x00,
0x01,0x19,0x01,0x03,0x68,0x30,0x1B,0x78,
0x2A,0x9D,0x80,0xA2,0x59,0x54,0x9E,0x27,
0x0E,0x50,0x54,0xBF,0xEF,0x00,0xD1,0xC0,
0xB3,0x00,0x95,0x00,0x81,0x80,0x81,0x40,
0x81,0xC0,0x01,0x01,0x01,0x01,0x02,0x3A,
0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,
0x45,0x00,0xDD,0x0C,0x11,0x00,0x00,0x1E,
0x00,0x00,0x00,0xFD,0x00,0x32,0x4C,0x1E,
0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,
0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x4C,
0x45,0x44,0x20,0x32,0x32,0x31,0x38,0x57,
0x0A,0x20,0x20,0x20,0x00,0x00,0x00,0xFF,
0x00,0x45,0x45,0x42,0x31,0x38,0x34,0x30,
0x30,0x5A,0x30,0x30,0x30,0x30,0x00,0x0A,

};
#elif EDID_185W
BYTE code tDDC[128]=
{ 
#if(PanelType == PanelLM185TT3A)

0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x04,0x72,0x60,0x15,0x01,0x00,0x00,0x00,
0x17,0x1B,0x01,0x03,0x00,0x29,0x17,0x78,
0x2E,0xB1,0xA5,0xA1,0x58,0x4F,0x95,0x26,
0x16,0x50,0x54,0x23,0x08,0x00,0x81,0xC0,
0x81,0x00,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,
0x56,0xAA,0x51,0x00,0x1E,0x30,0x46,0x8F,
0x33,0x00,0x99,0xE6,0x10,0x00,0x00,0x1C,
0x00,0x00,0x00,0xFD,0x00,0x38,0x3F,0x1E,
0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,
0x20,0x20,0x0E,0x1F,0x00,0x80,0x51,0x00,
0x1E,0x30,0x40,0x80,0x37,0x00,0x70,0xCF,
0x10,0x00,0x00,0x1C,0x00,0x00,0x00,0xFC,
0x00,0x45,0x31,0x39,0x31,0x4D,0x4E,0x54,
0x0A,0x20,0x20,0x20,0x20,0x20,0x00,0x36,



#else
//限60HZ
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x5D,0xD3,0x60,0x15,0x01,0x00,0x00,0x00,
0x20,0x1A,0x01,0x03,0x00,0x22,0x13,0x78,
0x2E,0xB1,0xA5,0xA1,0x58,0x4F,0x95,0x26,
0x16,0x50,0x54,0x23,0x08,0x00,0x81,0xC0,
0x81,0x00,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,
0x56,0xAA,0x51,0x00,0x1E,0x30,0x46,0x8F,
0x33,0x00,0x99,0xE6,0x10,0x00,0x00,0x1C,
0x00,0x00,0x00,0xFD,0x00,0x38,0x3F,0x1E,
0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,
0x20,0x20,0x0E,0x1F,0x00,0x80,0x51,0x00,
0x1E,0x30,0x40,0x80,0x37,0x00,0x70,0xCF,
0x10,0x00,0x00,0x1C,0x00,0x00,0x00,0xFC,
0x00,0x56,0x47,0x41,0x0A,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x00,0xF0,
/*
//去掉60以上
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x5D,0xD3,0x60,0x15,0x01,0x00,0x00,0x00,
0x20,0x1A,0x01,0x03,0x00,0x22,0x13,0x78,
0x2E,0xB1,0xA5,0xA1,0x58,0x4F,0x95,0x26,
0x16,0x50,0x54,0x23,0x08,0x00,0x81,0xC0,
0x81,0x00,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,
0x56,0xAA,0x51,0x00,0x1E,0x30,0x46,0x8F,
0x33,0x00,0x99,0xE6,0x10,0x00,0x00,0x1C,
0x00,0x00,0x00,0xFF,0x00,0x30,0x30,0x30,
0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
0x30,0x31,0x0E,0x1F,0x00,0x80,0x51,0x00,
0x1E,0x30,0x40,0x80,0x37,0x00,0x70,0xCF,
0x10,0x00,0x00,0x1C,0x00,0x00,0x00,0xFC,
0x00,0x56,0x47,0x41,0x0A,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x40,
*/
/*
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x35,0xB8,0x60,0x15,0x01,0x00,0x00,0x00,
0x16,0x19,0x01,0x03,0x00,0x22,0x13,0x78,
0x2E,0xB1,0xA5,0xA1,0x58,0x4F,0x95,0x26,
0x16,0x50,0x54,0xAF,0xCE,0x00,0x81,0xC0,
0x81,0x00,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,
0x56,0xAA,0x51,0x00,0x1E,0x30,0x46,0x8F,
0x33,0x00,0x99,0xE6,0x10,0x00,0x00,0x1C,
0x00,0x00,0x00,0xFF,0x00,0x30,0x30,0x30,
0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
0x30,0x31,0x0E,0x1F,0x00,0x80,0x51,0x00,
0x1E,0x30,0x40,0x80,0x37,0x00,0x70,0xCF,
0x10,0x00,0x00,0x1C,0x00,0x00,0x00,0xFC,
0x00,0x00,0x0A,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x00,0xDA,
*/
/*
//不限60
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x2A,0x59,0x50,0x18,0xB8,0x22,0x00,0x00,
0x26,0x1A,0x01,0x03,0x08,0x22,0x13,0x78,
0xAE,0xB1,0xA5,0xA1,0x58,0x4F,0x95,0x26,
0x16,0x50,0x54,0xA3,0x08,0x00,0x31,0x40,
0x45,0x40,0x61,0x40,0x81,0x80,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,
0x56,0xE2,0x50,0x00,0x20,0x30,0x46,0x8F,
0x33,0x00,0xC4,0xFE,0x10,0x00,0x00,0x1E,
0x66,0x21,0x56,0xAA,0x51,0x00,0x1E,0x30,
0x46,0x8F,0x33,0x00,0xC4,0xFE,0x10,0x00,
0x00,0x1E,0x00,0x00,0x00,0xFF,0x00,0x30,
0x30,0x0A,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
0x00,0x41,0x6E,0x61,0x6C,0x6F,0x67,0x0A,
0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x06,
*/
/*
//限60
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x2A,0x59,0x50,0x18,0xB8,0x22,0x00,0x00,
0x26,0x1A,0x01,0x03,0x08,0x22,0x13,0x78,
0xAE,0xB1,0xA5,0xA1,0x58,0x4F,0x95,0x26,
0x16,0x50,0x54,0x23,0x08,0x00,0x31,0x40,
0x45,0x40,0x61,0x40,0x81,0x80,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,
0x56,0xE2,0x50,0x00,0x20,0x30,0x46,0x8F,
0x33,0x00,0xC4,0xFE,0x10,0x00,0x00,0x1E,
0x66,0x21,0x56,0xAA,0x51,0x00,0x1E,0x30,
0x46,0x8F,0x33,0x00,0xC4,0xFE,0x10,0x00,
0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x38,
0x3F,0x1E,0x53,0x11,0x00,0x0A,0x20,0x20,
0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
0x00,0x41,0x6E,0x61,0x6C,0x6F,0x67,0x0A,
0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x6F,
*/

#endif
};

BYTE code tDVIDDC[128]=
{
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x05,0x2F,0x50,0x18,0xB8,0x22,0x00,0x00,
0x31,0x19,0x01,0x03,0x80,0x29,0x17,0x78,
0xAE,0xB1,0xA5,0xA1,0x58,0x4F,0x95,0x26,
0x16,0x50,0x54,0xAD,0xCE,0x00,0x31,0x40,
0x45,0x40,0x61,0x40,0x81,0x80,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x66,0x21,
0x56,0xAA,0x51,0x00,0x1E,0x30,0x46,0x8F,
0x33,0x00,0xC4,0xFE,0x10,0x00,0x00,0x1E,
0x66,0x21,0x56,0xAA,0x51,0x00,0x1E,0x30,
0x46,0x8F,0x33,0x00,0xC4,0xFE,0x10,0x00,
0x00,0x1E,0x00,0x00,0x00,0xFC,0x00,0x0A,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
0x00,0x41,0x6E,0x61,0x6C,0x6F,0x67,0x0A,
0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x54,

};
#endif
#if !Enable_External_EDID

#if  FDDCCI_WriteEDID
Bool CheckFlashEDIDData(void)
{
	if(DDC1[0]==0x00&&DDC1[1]==0xFF&&DDC1[2]==0xFF&&DDC1[3]==0xFF&&DDC1[4]==0xFF&&DDC1[5]==0xFF&&DDC1[6]==0xFF&&DDC1[7]==0x00)
	{
		return TRUE;
	}
	else
		return FALSE;	
}
void mStar_InitDDC(void)
{
	BYTE i;
	//BYTE dd;
    	 
#if MainBoardType == MainBoard_SLB1PTR||MainBoardType == MainBoard_JRYL1PTR_LF
	#if USEFLASH
	Flash_ReadTbl(DDCAKEYSET_START, &DDC1, 128);
	//Flash_Read_Factory_KeySet(FLASH_KEY_DDCA, TRUE, 0, DDCAKEY_SIZE, DDC1, DDCAKEY_SIZE);
	#else
	NVRam_ReadTbl( DDCSaveAddr , &DDC1, 128 );
	#endif
	mStar_WriteByte(DDC_LAST2, mStar_ReadByte(DDC_LAST2)&~BIT7); // bit7=0, write
	
	if(CheckFlashEDIDData())
		{	
		for(i = 0; i < 128; i++)
			{
			mStar_WriteByte(DDCADDR, i);
			mStar_WriteByte(DDCDATA, DDC1[i]);     
			}
		}
	else
		{
		for(i = 0; i < 128; i++)
			{
			mStar_WriteByte(DDCADDR, i);
			mStar_WriteByte(DDCDATA, tDDC[i]);    
			}
		}
	
	mStar_WriteByte(DDCEN2, BIT7|BIT5);
	mStar_WriteByte(DDC_LAST2,mStar_ReadByte(DDC_LAST2)|BIT7); // bit7=1, read
	
#elif  MainBoardType == MainBoard_JRY58NWHL
	#if USEFLASH
       Flash_ReadTbl(DDCAKEYSET_START, &DDC1, 128);
	//Flash_Read_Factory_KeySet(FLASH_KEY_DDCA, TRUE, 0, DDCAKEY_SIZE, &DDC1, DDCAKEY_SIZE);
	#else
	NVRam_ReadTbl( DDCSaveAddr , &DDC1, 128 );
	#endif
	mStar_WriteByte(DDC_LAST2,mStar_ReadByte(DDC_LAST2)&~BIT7);
	mStar_WriteByte(DDC_LAST1,mStar_ReadByte(DDC_LAST1)&~BIT7);
	
	if(CheckFlashEDIDData())
		{	
		for(i = 0; i < 128; i++)
			{
			mStar_WriteByte(DDCADDR, i);
			mStar_WriteByte(DDCDATA, DDC1[i]);     
			}
		}
	else
		{
		for(i = 0; i < 128; i++)
			{
			mStar_WriteByte(DDCADDR, i);
			mStar_WriteByte(DDCDATA, tDDC[i]);    
			}
		}
 #if Dual			  
   	#if USEFLASH
       Flash_ReadTbl(DDCAKEYSET_START, &DDC1, 128);
   	//Flash_Read_Factory_KeySet(FLASH_KEY_DDCD, TRUE, 0, DDCDKEY_SIZE, &DDC1, DDCDKEY_SIZE);
   	#else
   	NVRam_ReadTbl( DDCSaveAddr+128 , &DDC1, 128 );
  	#endif
   
	if(CheckFlashEDIDData())
    	{		
		for(i = 0; i < 128; i++)
		{
		mStar_WriteByte(DDCADDR, i+0x80);
		mStar_WriteByte(DDCDATA, DDC1[i]);
		}
   	}
   	else
    	{
		for(i = 0; i < 128; i++)
		{
		mStar_WriteByte(DDCADDR, i+0x80);
		mStar_WriteByte(DDCDATA, tDVIDDC[i]);
		}
   	}
     #endif	
	mStar_WriteByte(DDCEN1, BIT7);
	mStar_WriteByte(DDCEN2, BIT7|BIT5);
    mStar_WriteByte(DDC_LAST1,mStar_ReadByte(DDC_LAST1)|BIT7);	//0XB5
    mStar_WriteByte(DDC_LAST2,mStar_ReadByte(DDC_LAST2)|BIT7);	//0XB9
#else

	#if USEFLASH
       Flash_ReadTbl(DDCAKEYSET_START, &DDC1, 128);
	//Flash_Read_Factory_KeySet(FLASH_KEY_DDCA, TRUE, 0, DDCAKEY_SIZE, &DDC1, DDCAKEY_SIZE);
	#else
	NVRam_ReadTbl( DDCSaveAddr , &DDC1, 128 );
	#endif

 if(CheckFlashEDIDData())
    {	
	for(i = 0; i < 128; i++)
	{
		mStar_WriteByte(DDCADDR2, i);
		mStar_WriteByte(DDCDATA2, DDC1[i]);
	}
   }
   else
   {
	for(i = 0; i < 128; i++)
	{
		mStar_WriteByte(DDCADDR2, i);
		mStar_WriteByte(DDCDATA2, tDDC[i]);
	}
   }
	mStar_WriteByte(DDCEN2, D_EN_B);
	mStar_WriteByte(DDCADDR2, 0x81);   //VGA  //for DDC 2005.11.08
	mStar_WriteByte(DDC_LAST2,BIT7);//oym 20060920 mStar_WriteByte(DDC_LAST1,BIT7);//by beary 2006.6.28
#if Dual		
   	#if USEFLASH
       Flash_ReadTbl(DDCAKEYSET_START, &DDC1, 128);
   	//Flash_Read_Factory_KeySet(FLASH_KEY_DDCD, TRUE, 0, DDCDKEY_SIZE, &DDC1, DDCDKEY_SIZE);
   	#else
   	NVRam_ReadTbl( DDCSaveAddr+128 , &DDC1, 128 );
  	 #endif

   if(CheckFlashEDIDData())
    {		
	for(i = 0; i < 128; i++)
	{
		//dd = FlashReadByte(DIGITAL_DDC_START+i);
		mStar_WriteByte(DDCADDR, i);
		mStar_WriteByte(DDCDATA, DDC1[i]);
	}
   }
   else
    {
	for(i = 0; i < 128; i++)
	{
		//dd = FlashReadByte(DIGITAL_DDC_START+i);
		mStar_WriteByte(DDCADDR, i);
		mStar_WriteByte(DDCDATA, tDVIDDC[i]);
	}
   }
   #endif
	mStar_WriteByte(DDCEN1, BIT5|D_EN_B);//oym 20060928 D_EN_B);
	mStar_WriteByte(DDCADDR, 0x81);   //dvi//for DDC 2005.11.08
	mStar_WriteByte(DDC_LAST1,BIT7);    //oym 20060920 
		
	EX1	= 1;
	
#endif
}
#else
void mStar_ClrDDC_WP(void)    //turn off Ram WP
{
	mStar_WriteByte(DDCEN2, BIT7); // B8[5] 0->disable write protect
#if EnableDVIDDC
	mStar_WriteByte(DDCEN1, BIT7|BIT5); // B4[5] 1->disable write protect
#endif
}
void mStar_SetDDC_WP(void)    //turn on Ram WP
{
    mStar_WriteByte(DDCEN2, BIT7|BIT5); // B8[5] 1->enable write protect
#if EnableDVIDDC
    mStar_WriteByte(DDCEN1, BIT7); // B4[5] 0->enable write protect
#endif
}
Bool CheckFlashEDIDData(void)
{
	if(DDC1[0]==0x00&&DDC1[1]==0xFF&&DDC1[2]==0xFF&&DDC1[3]==0xFF&&DDC1[4]==0xFF&&DDC1[5]==0xFF&&DDC1[6]==0xFF&&DDC1[7]==0x00)
	{
		return TRUE;
	}
	else
		return FALSE;	
}
void mStar_InitDDC(void)
{
	BYTE xdata i;
 /*
	BYTE xdata dd;

	mStar_WriteByte(DDCEN2, BIT7);
	mStar_WriteByte(DDC_LAST2, BIT7);
	
	for(i = 0; i < 128; i++)
	{
		dd = FlashReadByte(DDCAKEYSET_START+i);
		mStar_WriteByte(DDCADDR2, i);
		mStar_WriteByte(DDCDATA2, dd);
	}
	#if EnableDVIDDC
	mStar_WriteByte(DDCEN1, BIT7);
	mStar_WriteByte(DDC_LAST1, BIT7);
	for(i = 0; i < 128; i++)
	{
		dd = FlashReadByte(DDCDKEYSET_START+i);
		mStar_WriteByte(DDCADDR, i);
		mStar_WriteByte(DDCDATA, dd);
	}
       #endif
       
	mStar_SetDDC_WP();
*/

	mStar_ClrDDC_WP();
	mStar_WriteByte(DDC_LAST2, mStar_ReadByte(DDC_LAST2)&~BIT7); // bit7=0, write
	//Flash_ReadTbl(DDCAKEYSET_START, &DDC1, 128);
	if(CheckFlashEDIDData())
		{	
		for(i = 0; i < 128; i++)
			{
			mStar_WriteByte(DDCADDR, i);
			mStar_WriteByte(DDCDATA, DDC1[i]);     
			}
		}
	else
		{
		for(i = 0; i < 128; i++)
			{
			mStar_WriteByte(DDCADDR, i);
			mStar_WriteByte(DDCDATA, tDDC[i]);    
			}
		}
	
	mStar_SetDDC_WP();//mStar_WriteByte(DDCEN2, BIT7|BIT5);
	mStar_WriteByte(DDC_LAST2,mStar_ReadByte(DDC_LAST2)|BIT7); // bit7=1, read
}
#endif
#endif

#if VGA_EDID_Write_To_24C02

extern void Delay1ms(WORD msNums);
extern void ForceDelay1ms(WORD msNums);


#define NVRAM_ADDR_EDID          0x00
extern void NVRam_WriteByte(WORD addr, BYTE value);
extern void NVRam_ReadByte(WORD addr, BYTE *value);
extern void SaveMonitorSetting(void);
extern void printData(char *str, WORD value);
extern void  printMsg(char *str);
Bool CheckFlashEDIDData(void)
{
	if(DDC1[0]==0x00&&DDC1[1]==0xFF&&DDC1[2]==0xFF&&DDC1[3]==0xFF&&DDC1[4]==0xFF&&DDC1[5]==0xFF&&DDC1[6]==0xFF&&DDC1[7]==0x00)
	{
		return TRUE;
	}
	else
		return FALSE;	
}
void ClearDDCBuffer(void)
{
	BYTE xdata i;
	
	for(i=0; i<128; i++)
	{
		DDC1[i]=0xFF;
	}
}
void Mstar_Load24C02EDID(void)
{
      WORD i;
	BYTE RamCheckSum=0, dd=0;
	bit CheckDoubleFlag=0;
     if (!VGA_EDID_Flag)
      {
	#if 0//Debug_En
	printMsg(" Load 24C02 VGA_EDID");
	#endif
       hw_ClrAmberLed();

	  #if USEFLASH
	   Flash_Read_Factory_KeySet(FLASH_KEY_DDCA, TRUE, 0, DDCAKEY_SIZE, DDC1, DDCAKEY_SIZE);
	  //Flash_ReadTbl1(FLASH_DDCA_START_ADDR, &DDC1, 128);
	  #else
	  NVRam_ReadTbl( DDCSaveAddr , &DDC1, 128 );
	  #endif
	
       //   hw_ClrDDC_WP();
 	 Delay1ms(5); // ForceDelay1ms(5);	 
	if(CheckFlashEDIDData())
	     {	
	          for(i=0; i<128; i++)
	          {
	           dd=DDC1[i];
	            NVRam_WriteByte(NVRAM_ADDR_EDID+i,dd);
				//printMsg("EEEEEEEEEEEEEEEEEEEEEE");
	          }	 
		}
	else
		{
	          for(i=0; i<128; i++)
	          {
	           dd=tDDC[i];
	            NVRam_WriteByte(NVRAM_ADDR_EDID+i,dd);
			//printMsg("DDDDDDDDDDDDDDDD");
	          }	 
		}
         // hw_SetDDC_WP();
         
          ClearDDCBuffer();

           for(i=0; i<128; i++)
           {
              NVRam_ReadByte(NVRAM_ADDR_EDID+i, &dd);
              DDC1[i]=dd;  
		//printData("DDC1[i]==%x", DDC1[i]);
           } 

	////////Check
	RamCheckSum=0;
       CheckDoubleFlag=0;
	if((DDC1[0]==0x00)&&(DDC1[1]==0xFF)&&(DDC1[2]==0xFF)&&(DDC1[3]==0xFF)&&(DDC1[4]==0xFF)
	&&(DDC1[5]==0xFF)&&(DDC1[6]==0xFF)&&(DDC1[7]==0x00))//&&(DDC1[0x7F]==tDDC[0x7F]))
	{
           for(i=0; i<128; i++)
           {
           	RamCheckSum += ( DDC1[i] );
           }
           if((RamCheckSum==0)&&((DDC1[0x14]&0x80)==0x00))//
           CheckDoubleFlag=1;	
           else 
           CheckDoubleFlag=0;
	}

	if(CheckDoubleFlag)
	{
	   Set_VGA_EDID_Flag();
             
        #if USEFLASH
	   SaveMonitorSetting();
        #else	
	  NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
       #endif

         #if 0//Debug_En
	    printMsg("Check 24C02 VGA DDC Pass");
          #endif
	}
	else//DDC Error Reloads 
 	{
 	   ////////Load    
         #if 0//Debug_En
 	   printMsg("Check 24C02 VGA DDC Fail");
        #endif
 
 	Clr_VGA_EDID_Flag();	//Check Next Time
 	       
        #if USEFLASH
	   SaveMonitorSetting();
        #else	
	  NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
       #endif

 	}
          hw_SetAmberLed();
     }
}
#endif

/*
Bool mStar_ReadDDC(BYTE EDIDType)   //read DDC from RAM and prepare for write to flash
{
	BYTE i;
	BYTE checksum=0;
	if(EDIDType==VGA_EDID)	
	{
		mStar_WriteByte(DDCADDR2, 0x80);
		for(i = 0; i < 128; i++)
		{
			DDC1[i]=mStar_ReadByte(DDCDATA2);
			checksum += ( ~DDC1[i] );
		}
	}
	#if EnableDVIDDC
	else if(EDIDType==DVI_EDID)	
	{
		mStar_WriteByte(DDCADDR, 0x80);
		for(i = 0; i < 128; i++)
		{
			DDC1[i]=mStar_ReadByte(DDCDATA);
			checksum += ( ~DDC1[i] );
		}
	}
	#endif
	if( checksum == 0)//EDID Data is right
		return TRUE;
	else
		return FALSE;
}

Bool TPV_CheckEDIDData(BYTE EDIDType)
{
	BYTE i;
	BYTE checksum=0;
	if(EDIDType==VGA_EDID)	
	{
		mStar_WriteByte(DDCADDR2, 0x80);
		for(i = 0; i < 8; i++)
		{
			DDC1[i]=mStar_ReadByte(DDCDATA2);
			checksum += ( ~DDC1[i] );
		}
	}
	#if EnableDVIDDC
	else if(EDIDType==DVI_EDID)	
	{
		mStar_WriteByte(DDCADDR, 0x80);
		for(i = 0; i < 8; i++)
		{
			DDC1[i]=mStar_ReadByte(DDCDATA);
			checksum += ( ~DDC1[i] );
		}
	}
	#endif

	if(DDC1[0]==0x00&&DDC1[1]==0xFF&&DDC1[2]==0xFF&&DDC1[3]==0xFF&&DDC1[4]==0xFF&&DDC1[5]==0xFF&&DDC1[6]==0xFF&&DDC1[7]==0x00)
	{
		mStar_WriteDDC(EDIDType);
	}
	//mStar_ReadDDC(EDIDType);
	if(DDC1[0]==0xFF&&DDC1[1]==0x00&&DDC1[2]==0x00&&DDC1[3]==0x00&&DDC1[4]==0x00&&DDC1[5]==0x00&&DDC1[6]==0x00&&DDC1[7]==0xFF)
		return TRUE;
	else
		return FALSE;
}
void mStar_WriteDDC(BYTE EDIDType)
{
	BYTE i;
	BYTE dd;

	if(EDIDType==VGA_EDID)	
	{mStar_WriteByte(DDCEN2, BIT7);	
	  for(i = 0; i < 128; i++)
	  {dd = FlashReadByte(DDCAKEYSET_START+i);
	    mStar_WriteByte(DDCADDR2, i);
	    mStar_WriteByte(DDCDATA2, dd);
	  }
	}
	#if EnableDVIDDC
	else if(EDIDType==DVI_EDID)	
	{mStar_WriteByte(DDCEN1, (mStar_ReadByte(DDCEN1)|BIT7));
	  for(i = 0; i < 128; i++)
	  {
		dd = FlashReadByte(DDCDKEYSET_START+i);
		mStar_WriteByte(DDCADDR, i);
		mStar_WriteByte(DDCDATA, dd);
	  }
      }
      #endif
	//mStar_WriteByte(DDCEN2, (mStar_ReadByte(DDCEN2)&~BIT0)|BIT7|BIT5);
}

void mStar_CheckRAM(void)
{
bit WriteVGAEDID=0;
#if EnableDVIDDC
bit WriteDVIEDID=0;
#endif
//printData("DDCEN2_2=%d", (!(mStar_ReadByte(DDCEN2)&BIT5)));

  if(
  	((mStar_ReadByte(DDCEN2)&BIT0)&&(!(mStar_ReadByte(DDCEN2)&BIT5)))
  	#if EnableDVIDDC
  	||(mStar_ReadByte(DDCEN1)&BIT0)
  	#endif
  	) //ivan 070228
  {
	if(TPV_CheckEDIDData(VGA_EDID))
		WriteVGAEDID=1;
	#if EnableDVIDDC
	if(TPV_CheckEDIDData(DVI_EDID))
	  	WriteDVIEDID=1;
	#endif
	if(WriteVGAEDID
	#if EnableDVIDDC
		||WriteDVIEDID
	#endif
		)
	ForceDelay1ms(1000);//Need to try this value
	else
		return;
	if(WriteVGAEDID)//TPV_CheckEDIDData(VGA_EDID))
	{
		//ForceDelay1ms(900);
		hw_ClrGreenLed();
		hw_ClrAmberLed(); 
		if(mStar_ReadDDC(VGA_EDID))
		{
			//Flash_Write_EDID(VGA_EDID);
			Flash_Write_Factory_KeySet(FLASH_KEY_DDCA, 0, DDCAKEY_SIZE, DDC1, DDCAKEY_SIZE);
		}
		mStar_WriteDDC(VGA_EDID);
		hw_SetGreenLed();
	}
	#if EnableDVIDDC
	#ifndef OnlyVGAEDIDInFlash
	if(WriteDVIEDID)//TPV_CheckEDIDData(DVI_EDID))
	{
		//ForceDelay1ms(900);
		hw_ClrGreenLed();
		hw_ClrAmberLed(); 
		if(mStar_ReadDDC(DVI_EDID))
		{
			//Flash_Write_EDID(DVI_EDID);
			Flash_Write_Factory_KeySet(FLASH_KEY_DDCD, 0, 128, DDC1, 128);
		}
		mStar_WriteDDC(DVI_EDID);
		//mStar_WriteByte(DDCEN1, (mStar_ReadByte(DDCEN1)|BIT5)); // Zhifeng.Wu Disalbe DVI WP
		hw_SetGreenLed();
	}
	#endif
	#endif
   }
	
  #if 0//Dual
  else if(mStar_ReadByte(DDCEN1)&BIT0) //ivan 070228
  {     
	  ForceDelay1ms(900);
	  //Flash_Write_EDID(DVI_EDID);
         hw_ClrGreenLed();
         hw_ClrAmberLed();  
	  mStar_ReadDDC(DVI_EDID);
	  Flash_Write_EDID(DVI_EDID);
	  mStar_WriteDDC(DVI_EDID);
	  hw_SetGreenLed();
  	  //hw_SetAmberLed();
   }
  #endif
}//*/
#endif

#if F_ResetCheckEdidManueAndSN
Bool FactoryCheckEdidManufactureAndSN( void )
{
    Bool result=TRUE;
    BYTE i,dd=0,RamCheckSum=0; 

    for(i=0; i<128; i++)
    {
         NVRam_ReadByte(0x00+i, &dd);
         DDC1[i]=dd;  
         RamCheckSum += ( DDC1[i] );
     }   
	if((RamCheckSum !=0)||(DDC1[0x10]==0x01&&DDC1[0x11]==0x19))//2015 / 1 Week
	    result=FALSE;

    return result;
}
#endif


#ifdef  UseVGACableReadWriteAllPortsEDID

extern void i2c_WriteTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count);
extern void i2c_ReadTBL(BYTE deviceID,WORD addr,BYTE * buffer,BYTE count);

void WriteDevice(BYTE DeviceType,WORD Address, BYTE Length)//Write Data To Device
{
	#if defined(ReduceDDC)
		BYTE i;
		DWORD StartAddr;
		if(DeviceType==VGA1)
			StartAddr=DDCAKEYSET_START;
		else if(DeviceType==DVI1)
			StartAddr=DDCDKEYSET_START;
		else if(DeviceType==HDMI1)
			StartAddr=DDCHKEYSET_START;
		else if(DeviceType==PanelSettingDevice)
			StartAddr=HDCPKEYSET_START;
		else
			return;
		for(i=0;i < Length;i++)
		{	
			FlashWriteByte(StartAddr+Address+i,*(&SIORxBuffer+i));
		}
		//Flash_Write_Factory_KeySet(ucType, Address/Length, Length, SIORxBuffer, Length);//it takes about 112ms for flash to write 32 Bytes.
	#else
	   if (DeviceType==PanelSettingDevice)
	    {
			if(Address==0&&SIORxBuffer[0]!=0)//SIORxBuffer[0] mstar:0,novatek:1,realtek:2,himax:3,st:4,other:reserved
				return;
			PanelSelect=1;
			PanelSetting.PanelTypeNum=UseUserPanelValue;
			if(Address==0)
			{

				if(SIORxBuffer[1]==6)
					PanelSettingFlag=PanelSettingFlag&(~bPanelColorDepth);
				else if(SIORxBuffer[1]==8)
					PanelSettingFlag=PanelSettingFlag|bPanelColorDepth;

				PanelOnTiming1=SIORxBuffer[2];
				PanelOnTiming2=(WORD)(SIORxBuffer[3]<< 8)+SIORxBuffer[4];
				PanelOffTiming1=(WORD)(SIORxBuffer[5]<< 8)+SIORxBuffer[6];
				PanelOffTiming2=SIORxBuffer[7];
				PanelHSyncWidth=(WORD)(SIORxBuffer[8]<< 8)+SIORxBuffer[9];
				PanelHSyncBackPorch=(WORD)(SIORxBuffer[10]<< 8)+SIORxBuffer[11];
				PanelVSyncWidth=(WORD)(SIORxBuffer[12]<< 8)+SIORxBuffer[13];
				PanelVSyncBackPorch=(WORD)(SIORxBuffer[14]<< 8)+SIORxBuffer[15];
				PanelWidth=(WORD)(SIORxBuffer[16]<< 8)+SIORxBuffer[17];
				PanelHeight=(WORD)(SIORxBuffer[18]<< 8)+SIORxBuffer[19];
				PanelHTotal=(WORD)(SIORxBuffer[20]<< 8)+SIORxBuffer[21];
				PanelVTotal=(WORD)(SIORxBuffer[22]<< 8)+SIORxBuffer[23];
				PanelMaxHTotal=(WORD)(SIORxBuffer[24]<< 8)+SIORxBuffer[25];
				PanelMinHTotal=(WORD)(SIORxBuffer[26]<< 8)+SIORxBuffer[27];
				PanelMinHTotal=(WORD)(SIORxBuffer[26]<< 8)+SIORxBuffer[27];
				PanelMinHTotal=(WORD)(SIORxBuffer[26]<< 8)+SIORxBuffer[27];
				//PanelMaxVTotal=(WORD)SIORxBuffer[28]<< 8+SIORxBuffer[29];			
				//PanelMinVTotal=(WORD)SIORxBuffer[30]<< 8+SIORxBuffer[31];			
			}
			else if(Address==0x20)
			{
				WORD temp;
				PanelMinDCLK=SIORxBuffer[0];			
				PanelDCLK=SIORxBuffer[1];
				PanelMaxDCLK=SIORxBuffer[2];
				temp=(WORD)(SIORxBuffer[3]<< 8)+SIORxBuffer[4];
				if(ChipVer == TSUM_N || ChipVer == TSUM_Q)
				{
					if(temp<(170+(242-172)/2))
						temp=0x03;
					else if((temp>=(170+(242-172)/2))&&(temp<=(242+(339-242)/2)))
						temp=0x00;
					else if((temp>(242+(339-242)/2))&&(temp<=(339+(387-339)/2)))
						temp=0x01;
					else if(temp>(339+(387-339)/2))
						temp=0x02;
					PanelLVDSSwing= (PanelLVDSSwing&0x1F)|(temp<<6);
				}
				else
				{
					if(temp>=200&&temp<=450)
					{
						temp=(temp-200+((100+3)/12))/((100+3)/6)+0x10;
					}
					else//temp<200
					{
						temp=(temp-67+((100+3)/12))/((100+3)/6);
					}
					PanelLVDSSwing=(PanelLVDSSwing&0xC0)|temp;
				}
				PanelOutputControl2=SIORxBuffer[5]|(SIORxBuffer[6]<<1);
				PanelOutputControl1=(SIORxBuffer[7]<<7)|(SIORxBuffer[8]<<6)|(SIORxBuffer[9]<<5)|(SIORxBuffer[10]<<4)|(SIORxBuffer[11]<<3)|(SIORxBuffer[12]<<2)|(SIORxBuffer[13]<<1)|SIORxBuffer[14];
				PanelBurstModeFreq=(WORD)(SIORxBuffer[15]<< 8)+SIORxBuffer[16];
				PanelDefDuty=SIORxBuffer[17];
				PanelMinDuty=SIORxBuffer[18];
				PanelMaxDuty=SIORxBuffer[19];
				PanelSSCSTEPL=SIORxBuffer[20];
				PanelSSCSTEPH=SIORxBuffer[21];
				PanelSSCSPANL=SIORxBuffer[22];
				PanelSSCSPANH=SIORxBuffer[23];
				SetPWMFreq(PanelBurstModeFreq);

				mStar_WriteByte(OCTRL1,PanelOutputControl1);
				mStar_WriteByte(OCTRL2,PanelOutputControl2);

				mStar_WriteByte(LPLL_STEP_L, PanelSSCSTEPL);
				#ifdef TSUMXXT		//110311 Modify
				mStar_WriteByteMask(LPLL_STEP_H, PanelSSCSTEPH, (BIT2|BIT1|BIT0));
				#else
				mStar_WriteByte(LPLL_STEP_H, PanelSSCSTEPH);
				#endif
				mStar_WriteByte(LPLL_SPAN_L, PanelSSCSPANL);
				mStar_WriteByte(LPLL_SPAN_H, PanelSSCSPANH);
				
				if(SIORxBuffer[24]==0)//off
					PanelSettingFlag=PanelSettingFlag|bChangeModePanelVddOff;
				else if(SIORxBuffer[24]==1)//on
					PanelSettingFlag=PanelSettingFlag&(~bChangeModePanelVddOff);
				//SIORxBuffer[25]~SIORxBuffer[31] Reserved.Ignored.
				SavePanelSetting();
			}
		}
	      else
	      	{
		LoadEDIDSelectPort = DeviceType;//Select Current IIC Port
		i2c_WriteTBL(0xA0, Address, SIORxBuffer, Length);//it takes about 48ms for EEPROM to write 32 Bytes.
		LoadEDIDSelectPort = 0xFF;//Set IIC Port To UserData EEPRom
	      	}
	#endif
}
void ReadDevice(BYTE DeviceType,WORD Address, BYTE Length)//Read Data From Device
{
	#if defined(ReduceDDC)
		BYTE i;
		DWORD StartAddr;
		if(DeviceType==VGA1)
			StartAddr=DDCAKEYSET_START;
		else if(DeviceType==DVI1)
			StartAddr=DDCDKEYSET_START;
		else
			return;

		for(i = 0; i < Length; i++)
		{
			SIORxBuffer[i] = FlashReadByte((StartAddr+Address)+i);//Read Data From Flash Bank 3
		}
	#else
		LoadEDIDSelectPort = DeviceType;//Select Current IIC Port
		i2c_ReadTBL(0xA0, Address, SIORxBuffer, Length);//Read Data From EEProm
		LoadEDIDSelectPort = 0xFF;//Set IIC Port To UserData EEPRom
	#endif
}
#endif
#if 0
BYTE code tDDC1[] = 
{
    // 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x52, 0xD4, 0x0D, 0x00, 0x01, 0x00, 0x00, 0x00,  //0
    0x08, 0x0A, 0x01, 0x01, 0x6C, 0x1E, 0x17, 0x78, 0xEC, 0xA8, 0xE0, 0x99, 0x57, 0x4B, 0x92, 0x25,  //1
    0x1C, 0x50, 0x54, 0xBF, 0xEE, 0x00, 0x31, 0xCA, 0x61, 0x46, 0x61, 0x4A, 0x01, 0x01, 0x01, 0x01,  //2
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xD8, 0x09, 0x80, 0xA0, 0x20, 0x90, 0x31, 0x10, 0x10, 0x40,  //3
    0xD2, 0x00, 0x30, 0xE4, 0x10, 0x00, 0x00, 0x18, 0x40, 0x1F, 0x00, 0x30, 0x41, 0x00, 0x25, 0x30,  //4
    0x20, 0x60, 0x33, 0x00, 0x67, 0x1F, 0x11, 0x00, 0x00, 0x18, 0x0D, 0x19, 0x00, 0x30, 0x41, 0x00,  //5
    0x23, 0x30, 0x20, 0x60, 0x33, 0x00, 0x30, 0xE4, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xFC,  //6
    0x00, 0x46, 0x50, 0x54, 0x31, 0x35, 0x30, 0x33, 0x4E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x00, 0x4c  //7
};
void mStar_WriteDDC(BYTE ddcNo, BYTE *dataTbl, BYTE num)
{
    WORD i;
    BYTE ddcValue;
    mStar_WriteByte(ddcNo, 0); //DDCEN1/2 disable ddc function
    // Check DDC has been programmed
    mStar_WriteByte(ddcNo + 2, CheckDDCAddr1 | 0x80); //DDCADDR1/2
    ddcValue = mStar_ReadByte(ddcNo + 3); // DDCDATA1/2
    if (ddcValue == *(dataTbl + CheckDDCAddr1))
    {
        mStar_WriteByte(ddcNo + 2, CheckDDCAddr2 | 0x80); //DDCADDR1/2
        ddcValue = mStar_ReadByte(ddcNo + 3); // DDCDATA1/2
        if (ddcValue == *(dataTbl + CheckDDCAddr2))
            return ;
    }
    // program ddc content
    for (i = 0; i < num; i++)
    {
        mStar_WriteByte(ddcNo + 2, i); //DDCADDR1/2
        mStar_WriteByte(ddcNo + 3, *(dataTbl + i)); // DDCDATA1/2
    }
    mStar_WriteByte(ddcNo, 0); //D_EN_B);// enable ddc function
}
void mStar_WriteDDC1(void)
{
    mStar_WriteDDC(DDCEN1, &tDDC1, sizeof(tDDC1));
}

void mStar_WriteDDC2(void)
{
    mStar_WriteDDC(DDCEN2, &tDDC1, sizeof(tDDC1));
}
#endif
