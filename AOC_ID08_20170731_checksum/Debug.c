#include "types.h"
#include "Common.h"
#include "debugDef.h"
#include "board.h"
#include "global.h"
#include "ms_reg.h"
#include "ms_rwreg.h"
#include "msosd.h"
#include "NVRam.h"
#include "i2c.h"
#include "misc.h"
#include "adjust.h"
#include "userPref.h"
#include "autofunc.h"
#include "menudef.h"
#include "menufunc.h"
#include "msace.h"
#include "msdlc.h"
#include "msflash.h"
#include "keypaddef.h"
#include "mcu.h"
#include "power.h"
//BYTE SIORxBuffer[4];

 
BYTE xdata SIORxIndex = 0;
BYTE xdata ReSyncCounter;
//void printData(char *str, WORD value);
//void printMsg(char *str);
//void putSIOChar(unsigned char sendData);
//BYTE xdata pReg[123];
extern Bool xdata bFlashWriteFactory;
extern void mStar_InitGamma(void);
extern void Power_PowerOffSystem(void);
extern BYTE OT_WriteI2C(BYTE I2CAddr7Bit, BYTE *Buffer, BYTE BytesToSend);
extern BYTE OT_ReadI2C(BYTE I2CAddr7Bit, BYTE *Buffer, BYTE BytesToRead);
BYTE AOCAlign(void);
#if ENABLE_HDCP
    #if HDCPKEY_IN_Flash
        extern WORD CRC16(BYTE nData, WORD nCRC);
    #endif
#endif
#if 1//Benz 2007.4.16   16:26:29DEBUG_ENABLE
    void ProcessCommand(void)
    {
        short xdata m_value = 0;
        if (DebugCommand == Read_mStar)
        {
            DebugData = mStar_ReadByte(DebugAddress);
        }
        else if (DebugCommand == Write_mStar)
        {
            mStar_WriteByte(DebugAddress, DebugData);
        }
        else if (DebugCommand == Read_mStarADC)
        {
            mStar_WriteByte(REGBK, REGBANKADC);
            DebugData = mStar_ReadByte(DebugAddress);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        else if (DebugCommand == Write_mStarADC)
        {
            mStar_WriteByte(REGBK, REGBANKADC);
            mStar_WriteByte(DebugAddress, DebugData);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        else if (DebugCommand == Read_mStarTCON)
        {
            mStar_WriteByte(REGBK, REGBANKTCON);
            DebugData = mStar_ReadByte(DebugAddress);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        else if (DebugCommand == Write_mStarTCON)
        {
            mStar_WriteByte(REGBK, REGBANKTCON);
            mStar_WriteByte(DebugAddress, DebugData);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        else if (DebugCommand == Read_mStarSubWin)
        {
            mStar_WriteByte(REGBK, REGBANK3);
            DebugData = mStar_ReadByte(DebugAddress);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        else if (DebugCommand == Write_mStarSubWin)
        {
            mStar_WriteByte(REGBK, REGBANK3);
            mStar_WriteByte(DebugAddress, DebugData);
            mStar_WriteByte(REGBK, REGBANKSCALER);
        }
        else if (DebugCommand == Read_mStarOSD)
        {
            DebugData = mStar_ReadOSDByte(DebugAddress);
        }
        else if (DebugCommand == Write_mStarOSD)
        {
            mStar_WriteOSDByte(DebugAddress, DebugData);
        }
	#if !USEFLASH
        else if (DebugCommand == Read_NVRam)
        {
            NVRam_ReadByte(DebugAddress | ((WORD)DebugAddressBank << 8), &DebugData);
        }
        else if (DebugCommand == Write_NVRam)
        {
            #if Backupdata
                Set_DonotchangeBackFlag();
            #endif
            NVRam_WriteByte(DebugAddress | ((WORD)DebugAddressBank << 8), DebugData);
            #if Backupdata
                Clr_DonotchangeBackFlag();
            #endif
        }
	#endif
        else if (DebugCommand == Read_Device)
        {
	#if DEBUG_IOPORT
           	if ( DebugDeviceAddr == 0x88 )
	{
		if ( DebugAddress == 0x00 )
			DebugData = P0;
		else if ( DebugAddress == 0x01 )
			DebugData = P1;
		else if ( DebugAddress == 0x02 )
			DebugData = P2;
		else if ( DebugAddress == 0x03 )
			DebugData = P3;
		else if ( DebugAddress == 0x04 )
			DebugData = P4;
	}
	else
	#endif	
	#if DEBUG_MSTMCU
	if ( DebugDeviceAddr == 0x99 )
	{
       	DebugData = xfr_regs[ DebugAddress ];	// Mask for reduce xdata. Louis070124. follow Karl Suggest.
	}
	else
	#endif        
#if IT6251
	if ( DebugDeviceAddr == 0xAA )
	{
       	//DebugData = xfr_regs[ DebugAddress ];	// Mask for reduce xdata. Louis070124. follow Karl Suggest.
		DebugData=ReadI2C_Byte(DP_I2C_ADDR, DebugAddress);
	}
	else
#endif        
      #if DEBUG_ALLBANK  
	if ( DebugDeviceAddr <= 0x10 )
	{
		mStar_WriteByte(REGBK, DebugDeviceAddr);
      		DebugData=mStar_ReadByte(DebugAddress);
      		mStar_WriteByte(REGBK, REGBANKSCALER);
	}
	else
     #endif		
	#if DEBUG_FLASH
	if ( (DebugDeviceAddr&0xF0) == 0x70 )
	{
       	DebugData = FlashReadByte(HDCPKey_START+((DebugDeviceAddr&0x0F)*256)+DebugAddress);//DebugData = xfr_regs[ DebugAddress ];	// Mask for reduce xdata. Louis070124. follow Karl Suggest.
	}
	else	
	#endif
		#if !USEFLASH
            i2c_ReadTBL(DebugDeviceAddr, DebugAddress, &DebugData, 1);
	    #else
		;
	    #endif
        }
        else if (DebugCommand == Write_Device)
        {
    	#if DEBUG_IOPORT
      	if ( DebugDeviceAddr == 0x88 )
	{
		if ( DebugAddress == 0x00 )
			P0 = DebugData;
		else if ( DebugAddress == 0x01 )
			P1 = DebugData;
		else if ( DebugAddress == 0x02 )
			P2 = DebugData;
		else if ( DebugAddress == 0x03 )
			P3 = DebugData;
		else if ( DebugAddress == 0x04 )
			P4 = DebugData;
	}
	else
	#endif
	#if DEBUG_MSTMCU
	if ( DebugDeviceAddr == 0x99 )
	{
       	xfr_regs[ DebugAddress ] = DebugData;// Mask for reduce xdata. Louis070124. follow Karl Suggest.
	}
	else
	#endif
#if IT6251
	 if ( DebugDeviceAddr == 0xAA )
	{
       	//xfr_regs[ DebugAddress ] = DebugData;
		WriteI2C_Byte(DP_I2C_ADDR, DebugAddress, DebugData);
	}
	else 
#endif		
	#if DEBUG_ALLBANK
	if ( DebugDeviceAddr <= 0x10 )
	{
		mStar_WriteByte(REGBK, DebugDeviceAddr);
      		mStar_WriteByte(DebugAddress, DebugData);
      		mStar_WriteByte(REGBK, REGBANKSCALER);
	}
	else
	#endif
	#if DEBUG_FLASH
	if ( (DebugDeviceAddr&0xF0) == 0x70 )
	{
		FlashDisableWP();
       	FlashWriteByte(HDCPKey_START+((DebugDeviceAddr&0x0F)*256)+DebugAddress,DebugData);
		FlashEnableWP();
	}
	else	
	#endif
	#if 0//ndef ReduceDDC
            i2c_WriteTBL(DebugDeviceAddr, DebugAddress, &DebugData, 1);
	#else
	    ;
	#endif
        }
        else if (DebugCommand == Read_Matrix)
        {
            if (DebugAddress < 9)
            {
                DebugData = g_sCommandMatrix[DebugAddress / 3][DebugAddress % 3];
            }
        }
        else if (DebugCommand == Write_Matrix)
        {
            if (DebugAddress < 18)
            {
                if ((DebugAddress % 2) == 0)
                {
                    m_value = (short)DebugData << 8;
                }
                else
                {
                    m_value = m_value + DebugData;
                    g_sCommandMatrix[DebugAddress / 6][(DebugAddress % 6) / 2] = m_value;
                }
            }
            else
            {
                tColorCorrectionMatrix = &g_sCommandMatrix[0][0];
                msAdjustHSC(UserPrefHue, UserPrefSaturation, UserPrefSubContrast);
            }
        }
        else if (DebugCommand == Read_DLC)
        {
            if (DebugAddress < 16)
            {
                DebugData = g_ucLumaCurve[DebugAddress % 16];
            }
            else if (DebugAddress == 16)
            {
                DebugData = g_LowStrength;
            }
            else if (DebugAddress == 17)
            {
                DebugData = g_HighStrength;
            }
        }
        else if (DebugCommand == Write_DLC)
        {
            if (DebugAddress < 16)
            {
                g_ucLumaCurve[DebugAddress % 16] = DebugData;
            }
            else if (DebugAddress == 16)
            {
                g_LowStrength = DebugData;
            }
            else if (DebugAddress == 17)
            {
                g_HighStrength = DebugData;
            }
            else
            {
                // 2006/10/26 6:48PM by Emily            UserPrefDLCMode = MODE1_DLC;
                msSetDlcStrength(g_LowStrength, g_HighStrength);
                msDlcOnOff(_ENABLE);
            }
        }
        else if (DebugCommand==Send_DebugOnly)
        {
            if (DebugData==1)
                Set_DebugOnlyFlag();
            else if (DebugData==2)
                Clr_DebugOnlyFlag();
        }
        if (!(DebugCommand &BIT0))
        {
            DebugData = ((WORD)DebugData + DebugAddress) &0xFF;
        }
        #if 1//DEBUG_ENABLE
            putSIOChar(DebugData);
        #endif
    }
#endif
Bool GetCommand(void)
{
    if (ReSyncCounter)
    {
        if (SIORxIndex > 2 && SIORxIndex == CommandLength)
        {
            Set_RxBusyFlag();
            return TRUE;
        }
        if (ReSyncCounter < 5)
        {
            Clr_RxBusyFlag();
            SIORxIndex = 0;
	#if 0//DEBUG_ENABLE
		if(!DebugOnlyFlag)
	#endif
            ReSyncCounter = 0;
        }
    }
    else if (SIORxIndex)
    {
        SIORxIndex = 0;
    }
    return FALSE;
}
void DebugHandler(void)
{
    //ES = 1; // 20051123 for factory mode
    if (GetCommand())
    {
        #if 1//Benz 2007.4.16   16:26:38DEBUG_ENABLE
            ProcessCommand();
        #endif
        SIORxIndex = 0;
 	  #if 0//DEBUG_ENABLE
		if(!DebugOnlyFlag)
	#endif
        ReSyncCounter = 0;
        Clr_RxBusyFlag();
        //hw_ClrAmberLed();
    }
    else if (SIORxIndex > 7)
    {
        #if Backupdata
            Clr_GetChecksumFlag();
            DDCCIAdjCounter = 0;
        #endif
        AOCAlign();
        Clr_RxBusyFlag();
        SIORxIndex = 0;
    }
}



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
//extern void Read6500KSetting(void);    //20050921  WMZ
//extern void Read9300KSetting(void);
//extern void ReadSRGBSetting(void);
//extern Bool ResetAllSetting(void);
//extern Bool SetToEnglish(void);
//extern Bool SetToSimpleChinese(void);
BYTE AOCAlign(void)
{
    WORD sioData;
    if (SIO_ID1 == 'W')
    {
        // adjust
        if ((SIO_TYPE1 == '9') && (SIO_TYPE2 == '8'))
        {
            // ascii to integer
            sioData = ((SIO_DATA1 >= '0') && (SIO_DATA1 <= '9')) ? SIOData10 : SIOData11;
            sioData += ((SIO_DATA2 >= '0') && (SIO_DATA2 <= '9')) ? SIOData20 : SIOData21;
            sioData += ((SIO_DATA3 >= '0') && (SIO_DATA3 <= '9')) ? SIOData30 : SIOData31;
            // red
            if ((SIO_CMD1 == '0') && (SIO_CMD2 == '1'))
            // R Gain
            {
                UserPrefRedColor = sioData; // for CT
                // mStar_AdjustRedColor(UserPrefRedColor);
                mStar_AdjustRedColor(UserPrefRedColor, UserPrefContrast);
            }
            // green
            else if ((SIO_CMD1 == '0') && (SIO_CMD2 == '4'))
            // G Gain
            {
                UserPrefGreenColor = sioData; // for CT
                //mStar_AdjustGreenColor(UserPrefGreenColor);
                mStar_AdjustGreenColor(UserPrefGreenColor, UserPrefContrast);
            }
            // blue
            else if ((SIO_CMD1 == '0') && (SIO_CMD2 == '7'))
            // B Gain
            {
                UserPrefBlueColor = sioData; // for CT
                // mStar_AdjustBlueColor(UserPrefBlueColor);
                //mStar_AdjustBlueColor(UserPrefGreenColor,UserPrefContrast);
                mStar_AdjustBlueColor(UserPrefBlueColor, UserPrefContrast); //0727
            }
            // contrast
            else if ((SIO_CMD1 == '1') && (SIO_CMD2 == '4'))
            // Contrast
            {
                UserPrefContrast = sioData;
                //UserPrefContrast=((WORD)(MaxContrastValue-MinContrastValue))*UserPrefContrast/100+MinContrastValue;
                UserPrefContrast = UserPrefContrast + MinContrastValue;
                mStar_AdjustContrast(UserPrefContrast);
            }
            // black level
            else if ((SIO_CMD1 == '1') && (SIO_CMD2 == '5'))
            // Black level
            {}
            // brightness
            else if ((SIO_CMD1 == '1') && (SIO_CMD2 == '6'))
            // Brightness
            {
                UserPrefBrightness = sioData;
                mStar_AdjustBrightness(UserPrefBrightness);
            }
            // reset language
            else if ((SIO_CMD1 == 'R') && (SIO_CMD2 == 'L'))
            // Black level
            {
                #if LANGUAGE_TYPE == ASIA
                    if (sioData == 1)
                        UserPrefLanguage = LANG_France;
                    else if (sioData == 7)
                        UserPrefLanguage = LANG_TChina;
                    else if (sioData == 10)
                        UserPrefLanguage = LANG_Korea;
                    else if (sioData == 4)
                        UserPrefLanguage = LANG_Spanish;
                    else if (sioData == 8)
                        UserPrefLanguage = LANG_Portugues;
                    else if (sioData == 5)
                        UserPrefLanguage = LANG_Russia;
                    else if (sioData == 6)
                        UserPrefLanguage = LANG_SChina;
                    else
                    // default
                        UserPrefLanguage = LANG_English;
		 //For Select SChinese/TChinese/Korean Language Font Error Issue
            /* if ((UserPrefLanguage <= LANG_Russia)||UserPrefLanguage == LANG_SChina)//jeff 1222
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
                        SecondTblAddr= 0x7A;//SecondTblAddr = 0x80; //SecondTblAddr = 0xFF;
                    }
                    */
                      RunLanguage();
                #elif LANGUAGE_TYPE == EUROPE
                        if (sioData == 1)
                            UserPrefLanguage = LANG_France;
                        else if (sioData == 2)
                            UserPrefLanguage = LANG_German;
                        else if (sioData == 3)
                            UserPrefLanguage = LANG_Italian;
                        else if (sioData == 4)
                            UserPrefLanguage = LANG_Spanish;
                        else if (sioData == 5)
                            UserPrefLanguage = LANG_Russia;
                        else if (sioData == 6)
                            UserPrefLanguage = LANG_Swedish;
                        else if (sioData == 7)
                            UserPrefLanguage = LANG_Polish;
                        else if (sioData == 8)
                            UserPrefLanguage = LANG_Czech;
                        else
                        // default
                            UserPrefLanguage = LANG_English;
                #elif LANGUAGE_TYPE == ID2007Standard
                    if (sioData == 1)
                        UserPrefLanguage = LANG_France;
                    else if (sioData == 2)
                        UserPrefLanguage = LANG_German;
                    else if (sioData == 3)
                        UserPrefLanguage = LANG_Italian;
                    else if (sioData == 4)
                        UserPrefLanguage = LANG_Spanish;
                    else if (sioData == 8)
                        UserPrefLanguage = LANG_Portugues;
                    else if (sioData == 5)
                        UserPrefLanguage = LANG_Russia;
                    else if (sioData == 6)
                        UserPrefLanguage = LANG_SChina;
                    else
                    // default
                        UserPrefLanguage = LANG_English;
				#elif LANGUAGE_TYPE == All13Language
					if (sioData == 0x03)
                        UserPrefLanguage = LANG_France;
                    else if (sioData == 0x04)
                        UserPrefLanguage = LANG_German;
                    else if (sioData == 0x05)
                        UserPrefLanguage = LANG_Italian;
                    else if (sioData == 0x0A)
                        UserPrefLanguage = LANG_Spanish;
                    else if (sioData == 0x0B)
                        UserPrefLanguage = LANG_Swedish;
                    else if (sioData == 0x0D)
                        UserPrefLanguage = LANG_SChina;
                    else if (sioData == 0x1E)
                        UserPrefLanguage = LANG_Polish;
			else if (sioData ==0x08)
                        UserPrefLanguage = LANG_Portugues;
                    else if (sioData == 0x09)
                        UserPrefLanguage = LANG_Russia;
                    else if (sioData == 0x07)
                        UserPrefLanguage = LANG_Korea;
                    else if (sioData == 0x01)
                        UserPrefLanguage = LANG_TChina;
			else if (sioData == 0x12)
                        UserPrefLanguage = LANG_Czech;
                    else
                    // default
                        UserPrefLanguage = LANG_English;

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
					else
			        {
			            LanguageIndex = 3;
			            SecondTblAddr = 0x80;
			        }*/
			        RunLanguage();
				#elif LANGUAGE_TYPE == All14Language_AddJap
					if (sioData == 0x03)
                        UserPrefLanguage = LANG_France;
                    else if (sioData == 0x04)
                        UserPrefLanguage = LANG_German;
                    else if (sioData == 0x05)
                        UserPrefLanguage = LANG_Italian;
                    else if (sioData == 0x0A)
                        UserPrefLanguage = LANG_Spanish;
                    else if (sioData == 0x0B)
                        UserPrefLanguage = LANG_Swedish;
                    else if (sioData == 0x0D)
                        UserPrefLanguage = LANG_SChina;
                    else if (sioData == 0x1E)
                        UserPrefLanguage = LANG_Polish;
			else if (sioData ==0x0E)
                        UserPrefLanguage = LANG_Portugues;
                    else if (sioData == 0x09)
                        UserPrefLanguage = LANG_Russia;
                    else if (sioData == 0x07)
                        UserPrefLanguage = LANG_Korea;
                    else if (sioData == 0x01)
                        UserPrefLanguage = LANG_TChina;
			else if (sioData == 0x12)
                        UserPrefLanguage = LANG_Czech;
                    else
                    // default
                        UserPrefLanguage = LANG_English;

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
                                    UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit);                                    
                 #endif
            }
        }
        // save & recall color temperature
        else if ((SIO_TYPE1 == 'A') && (SIO_TYPE2 == '0'))
        {
            //  save cool
            if ((SIO_CMD1 == 'C') && (SIO_CMD2 == '1'))
            {
                UserPrefColorTemp = CTEMP_Cool1;
                UserPrefRedColorCool1 = UserPrefRedColor;
                UserPrefGreenColorCool1 = UserPrefGreenColor;
                UserPrefBlueColorCool1 = UserPrefBlueColor;
                UserPrefBrightnessCool1 = UserPrefBrightness;
                UserPrefContrastCool1 = UserPrefContrast;
                FUserPrefBrightnessCool1 = UserPrefBrightness;
                FUserPrefContrastCool1 = UserPrefContrast;
                
                #if USEFLASH
                UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit|bFlashSaveFactoryBit);                    
                #else
                SaveMonitorSetting();
                #endif
                
                //NVRam_WriteByte(Factory9300KContrastAddr, UserPrefContrast);
                //NVRam_WriteByte(Factory9300KBrightnessAddr, UserPrefBrightness);
            }
            // save warm
            else if ((SIO_CMD1 == 'A') && (SIO_CMD2 == '8'))
            {
                UserPrefColorTemp = CTEMP_Warm1;
                UserPrefRedColorWarm1 = UserPrefRedColor;
                UserPrefGreenColorWarm1 = UserPrefGreenColor;
                UserPrefBlueColorWarm1 = UserPrefBlueColor;
                UserPrefBrightnessWarm1 = UserPrefBrightness;
                UserPrefContrastWarm1 = UserPrefContrast;
                FUserPrefBrightnessWarm1 = UserPrefBrightness;
                FUserPrefContrastWarm1 = UserPrefContrast;
                
                #if USEFLASH
                UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit|bFlashSaveFactoryBit);                    
                #else
                SaveMonitorSetting();
                #endif
                
                //NVRam_WriteByte(Factory6500KContrastAddr, UserPrefContrast);
                //NVRam_WriteByte(Factory6500KBrightnessAddr, UserPrefBrightness);
            }
            // save normal:
            else if ((SIO_CMD1 == 'A') && (SIO_CMD2 == '5'))
            {
                UserPrefColorTemp = CTEMP_Normal;
                UserPrefRedColorNormal = UserPrefRedColor;
                UserPrefGreenColorNormal = UserPrefGreenColor;
                UserPrefBlueColorNormal = UserPrefBlueColor;
                UserPrefBrightnessNormal = UserPrefBrightness;
                UserPrefContrastNormal = UserPrefContrast;
                FUserPrefBrightnessNormal = UserPrefBrightness;
                FUserPrefContrastNormal = UserPrefContrast;
                
                #if USEFLASH
                UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit|bFlashSaveFactoryBit);                    
                #else
                SaveMonitorSetting();
                #endif
                
                //NVRam_WriteByte(FactorySRGBContrastAddr, UserPrefContrast);
                //NVRam_WriteByte(FactorySRGBBrightnessAddr, UserPrefBrightness);
            }
            // save sRGB:
            else if ((SIO_CMD1 == 'C') && (SIO_CMD2 == 'S'))
            {
                UserPrefColorTemp = CTEMP_SRGB;
                UserPrefRedColorSRGB = UserPrefRedColor;
                UserPrefGreenColorSRGB = UserPrefGreenColor;
                UserPrefBlueColorSRGB = UserPrefBlueColor;
                UserPrefBrightnessSRGB = UserPrefBrightness;
                UserPrefContrastSRGB = UserPrefContrast;
                
                #if USEFLASH
                UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit|bFlashSaveFactoryBit);                    
                #else
                SaveMonitorSetting();
                #endif
                
                //NVRam_WriteByte(FactorySRGBContrastAddr, UserPrefContrast);
                //NVRam_WriteByte(FactorySRGBBrightnessAddr, UserPrefBrightness);
            }
            // recall cool
            else if ((SIO_CMD1 == 'B') && (SIO_CMD2 == '9'))
            {
                UserPrefColorTemp = CTEMP_Cool1;
                UserPrefRedColor = UserPrefRedColorCool1;
                UserPrefGreenColor = UserPrefGreenColorCool1;
                UserPrefBlueColor = UserPrefBlueColorCool1;
                ReadColorTempSetting(); // 2006/11/10 6:59PM by KK  ReadCool1Setting();
                mStar_AdjustBrightness(UserPrefBrightness);
                mStar_AdjustContrast(UserPrefContrast);
            }
            // recall warm
            else if ((SIO_CMD1 == 'B') && (SIO_CMD2 == '8'))
            {
                UserPrefColorTemp = CTEMP_Warm1;
                UserPrefRedColor = UserPrefRedColorWarm1;
                UserPrefGreenColor = UserPrefGreenColorWarm1;
                UserPrefBlueColor = UserPrefBlueColorWarm1;
                ReadColorTempSetting(); // 2006/11/10 6:59PM by KK ReadWarm1Setting();
                mStar_AdjustBrightness(UserPrefBrightness);
                mStar_AdjustContrast(UserPrefContrast);
            }
            // recall normal
            else if ((SIO_CMD1 == 'B') && (SIO_CMD2 == '6'))
            {
                UserPrefColorTemp = CTEMP_Normal;
                UserPrefRedColor = UserPrefRedColorNormal;
                UserPrefGreenColor = UserPrefGreenColorNormal;
                UserPrefBlueColor = UserPrefBlueColorNormal;
                ReadColorTempSetting(); // 2006/11/10 6:59PM by KK ReadNormalSetting();
                mStar_AdjustBrightness(UserPrefBrightness);
                mStar_AdjustContrast(UserPrefContrast);
            }
            // 20050114 wumaozhong
            #if SRGB
                else if ((SIO_CMD1 == 'B') && (SIO_CMD2 == 'S'))
                {
                    UserPrefColorTemp = CTEMP_SRGB;
                    UserPrefRedColor = UserPrefRedColorSRGB;
                    UserPrefGreenColor = UserPrefGreenColorSRGB;
                    UserPrefBlueColor = UserPrefBlueColorSRGB;
                    ReadColorTempSetting(); // 2006/11/10 6:59PM by KK ReadSRGBSetting();
                    mStar_AdjustBrightness(UserPrefBrightness);
                    mStar_AdjustContrast(UserPrefContrast);
                }
            #endif
            else if (SIO_CMD1 == 'B' && SIO_CMD2 == '5')
            // Reset
            {
                ResetAllSetting();
            }
        }
        else if (SIO_TYPE1 == 'F')
        {
            sioData = ((SIO_DATA1 >= '0') && (SIO_DATA1 <= '9')) ? SIOData10 : SIOData11;
            sioData += ((SIO_DATA2 >= '0') && (SIO_DATA2 <= '9')) ? SIOData20 : SIOData21;
            sioData += ((SIO_DATA3 >= '0') && (SIO_DATA3 <= '9')) ? SIOData30 : SIOData31;
            if (SIO_TYPE2 == '0')
            {
                if (SIO_CMD1 == '0' && SIO_CMD2 == '1')
                {
                    if (mStar_AutoColor())
                    {
                        #if USEFLASH
                            UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit);     
                        #else
                             SaveMonitorSetting();
                       #endif
                    }
                }
                else if (SIO_CMD1 == '0' && SIO_CMD2 == '2')
                {
                    if (mStar_AutoGeomtry())
                    {
                        #if USEFLASH
                        UserPref_EnableFlashSaveBit(bFlashSaveModeBit); 
                        #else
                        SaveModeSetting();
                        #endif
                    }
                }
            }
            else if (SIO_TYPE2 == '3')
            {
                if (SIO_CMD1 == '0' && SIO_CMD2 == '0')
                {
                    Clr_BurninModeFlag();
                    Clr_DoBurninModeFlag();
                 #if !USEFLASH
                 						  NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
                 #else
                 						  UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit);   
                 #endif
                }
                else if (SIO_CMD1 == '0' && SIO_CMD2 == '1')
                {
                    Set_BurninModeFlag();
                    Set_DoBurninModeFlag();
                 #if !USEFLASH
                 						  NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
                 #else
                 						  UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
                 #endif
                }
                else if (SIO_CMD1 == '0' && SIO_CMD2 == '4')
                {
                    #if LANGUAGE_TYPE == ASIA
                        if (sioData == 1)
                            UserPrefLanguage = LANG_France;
                        else if (sioData == 7)
                            UserPrefLanguage = LANG_TChina;
                        else if (sioData == 10)
                            UserPrefLanguage = LANG_Korea;
                        else if (sioData == 4)
                            UserPrefLanguage = LANG_Spanish;
                        else if (sioData == 8)
                            UserPrefLanguage = LANG_Portugues;
                        else if (sioData == 5)
                            UserPrefLanguage = LANG_Russia;
                        else if (sioData == 6)
                            UserPrefLanguage = LANG_SChina;
                        else
                        // default
                            UserPrefLanguage = LANG_English;

     		          //For Select SChinese/TChinese/Korean Language Font Error Issue
                     /* if ((UserPrefLanguage <= LANG_Russia)||UserPrefLanguage == LANG_SChina)//jeff 1222
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
                            SecondTblAddr= 0x7A;//SecondTblAddr = 0x80; //SecondTblAddr = 0xFF;
                        }*/
                          RunLanguage();
                    #elif LANGUAGE_TYPE == EUROPE
                        if (sioData == 1)
                            UserPrefLanguage = LANG_France;
                        else if (sioData == 2)
                            UserPrefLanguage = LANG_German;
                        else if (sioData == 3)
                            UserPrefLanguage = LANG_Italian;
                        else if (sioData == 4)
                            UserPrefLanguage = LANG_Spanish;
                        else if (sioData == 5)
                            UserPrefLanguage = LANG_Russia;
                        else if (sioData == 6)
                            UserPrefLanguage = LANG_Swedish;
                        else if (sioData == 7)
                            UserPrefLanguage = LANG_Polish;
                        else if (sioData == 8)
                            UserPrefLanguage = LANG_Czech;
                        else
                        // default
                            UserPrefLanguage = LANG_English;
                    #elif LANGUAGE_TYPE == ID2007Standard
                        if (sioData == 1)
                            UserPrefLanguage = LANG_France;
                        else if (sioData == 2)
                            UserPrefLanguage = LANG_German;
                        else if (sioData == 3)
                            UserPrefLanguage = LANG_Italian;
                        else if (sioData == 4)
                            UserPrefLanguage = LANG_Spanish;
                        else if (sioData == 8)
                            UserPrefLanguage = LANG_Portugues;
                        else if (sioData == 5)
                            UserPrefLanguage = LANG_Russia;
                        else if (sioData == 6)
                            UserPrefLanguage = LANG_SChina;
                        else
                        // default
                            UserPrefLanguage = LANG_English;
			#elif LANGUAGE_TYPE == All13Language
					if (sioData == 0x03)
                        UserPrefLanguage = LANG_France;
                    else if (sioData == 0x04)
                        UserPrefLanguage = LANG_German;
                    else if (sioData == 0x05)
                        UserPrefLanguage = LANG_Italian;
                    else if (sioData == 0x0A)
                        UserPrefLanguage = LANG_Spanish;
                    else if (sioData == 0x0B)
                        UserPrefLanguage = LANG_Swedish;
                    else if (sioData == 0x0D)
                        UserPrefLanguage = LANG_SChina;
                    else if (sioData == 0x1E)
                        UserPrefLanguage = LANG_Polish;
			else if (sioData ==0x08)
                        UserPrefLanguage = LANG_Portugues;
                    else if (sioData == 0x09)
                        UserPrefLanguage = LANG_Russia;
                    else if (sioData == 0x07)
                        UserPrefLanguage = LANG_Korea;
                    else if (sioData == 0x01)
                        UserPrefLanguage = LANG_TChina;
			else if (sioData == 0x12)
                        UserPrefLanguage = LANG_Czech;
                    else
                    // default
                        UserPrefLanguage = LANG_English;

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
					else
			        {
			            LanguageIndex = 3;
			            SecondTblAddr = 0x80;
			        }
			        */
			          RunLanguage();
					#elif LANGUAGE_TYPE == All14Language_AddJap
					if (sioData == 0x03)
                        UserPrefLanguage = LANG_France;
                    else if (sioData == 0x04)
                        UserPrefLanguage = LANG_German;
                    else if (sioData == 0x05)
                        UserPrefLanguage = LANG_Italian;
                    else if (sioData == 0x0A)
                        UserPrefLanguage = LANG_Spanish;
                    else if (sioData == 0x0B)
                        UserPrefLanguage = LANG_Swedish;
                    else if (sioData == 0x0D)
                        UserPrefLanguage = LANG_SChina;
                    else if (sioData == 0x1E)
                        UserPrefLanguage = LANG_Polish;
			else if (sioData ==0x08)
                        UserPrefLanguage = LANG_Portugues;
                    else if (sioData == 0x09)
                        UserPrefLanguage = LANG_Russia;
                    else if (sioData == 0x07)
                        UserPrefLanguage = LANG_Korea;
                    else if (sioData == 0x01)
                        UserPrefLanguage = LANG_TChina;
			else if (sioData == 0x12)
                        UserPrefLanguage = LANG_Czech;
                    else
                    // default
                        UserPrefLanguage = LANG_English;

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
			        }
			        */
			          RunLanguage();
                    #endif
               #if !USEFLASH
               						NVRam_WriteByte(nvrMonitorAddr(Language), UserPrefLanguage);
               #else
               						UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
               #endif
                }
		else if (SIO_CMD1 == 'P' && SIO_CMD2 == 'M'&& SIO_DATA1 =='O')//ProductModeFlag WF3PMODE
              {
             	 	Set_ProductModeFlag();
			#if AudioFunc
			UserPrefVolume=90;//PE Request petit 20130102
			#endif
                	#if USEFLASH
			FactoryProductModeValue=ProductModeOnValue;
			UserPref_EnableFlashSaveBit(bFlashSaveFactoryBit); 
			UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
			#else
			//WORD temp=ProductModeOnValue;
			//NVRam_WriteTbl(ProductModeAddr, (BYTE*)(&temp), 2);
			NVRam_WriteWord(ProductModeAddr,ProductModeOnValue);
                     NVRam_WriteByte(nvrMonitorAddr(Volume), UserPrefVolume);
			#endif
                     
              }
            }
            else if (SIO_TYPE2 == '5')
            {
                if (SIO_CMD1 == '0' && SIO_CMD2 == '0')
                {
                 #if USEFLASH
			BlacklitTime = 0;
			MonitorBackLightTime= 0;
			UserPref_EnableFlashSaveBit(bFlashSaveMonitor2Bit); 
                 #else
			BlacklitTime = 0;
			SaveBlacklitTime();
                 #endif
                }
            }
            else if (SIO_TYPE2 == '7')
            {
                if (SIO_CMD1 == '0' && SIO_CMD2 == '1')
                {
                    //Set_ISPFlag();
                }
            }
            else if (SIO_TYPE2 == '9')
            {
                if (SIO_CMD1 == 'E' && SIO_CMD2 == 'N')
                {
                    Set_FactoryModeFlag();
                    Set_AutoColorFlag();
                    UserPrefContrastSRGB=FUserPrefContrastCool1=FUserPrefContrastWarm1=FUserPrefContrastNormal=UserPrefContrastCool1=UserPrefContrastWarm1=UserPrefContrastNormal=UserPrefContrast=DefContrast;
                    UserPrefBrightnessSRGB=FUserPrefBrightnessCool1=FUserPrefBrightnessWarm1=FUserPrefBrightnessNormal=UserPrefBrightnessCool1=UserPrefBrightnessWarm1=UserPrefBrightnessNormal=UserPrefBrightness = DefBrightness;
                    UserPrefECOMode = ECO_Standard;
                    UserPrefGamaMode = GAMA1;
                    mStar_InitGamma();
                    UserPrefDcrMode = 0;
                    UserPrefColorFlags = 0;
                    UserPrefYColor = DefColor;
                    UserPrefCColor = DefColor;
                    UserPrefMColor = DefColor;
                    UserPrefRedColorUser = DefColor;
                    UserPrefGreenColorUser = DefColor;
                    UserPrefBlueColorUser = DefColor;
                    UserPrefHue = 50;
                    UserPrefSaturation = 0x80;
                    UserPrefSubContrast = DefSubContrast;
                    UserPrefSubBrightness = DefSubBrightness;
                    UserprefHistogram1 = 0x40;
                    UserprefHistogram2 = 0xB0;
                    UserprefALha = 50;
                    UserprefBata = 50;
                    UserPrefBFSize = 1;
                    msAdjustSharpness(11, 1);
                    msAdjustSubBrightness(UserPrefSubBrightness);
                    msAdjustVideoContrast(DefContrastBase + UserPrefContrast);
                    msAdjustVideoHue(UserPrefHue);
                    msAdjustVideoSaturation(UserPrefSaturation);
                    SetBFSize(UserPrefBFSize);//SetBFSize();
                    mStar_AdjustContrast(UserPrefContrast);
                    mStar_AdjustBrightness(UserPrefBrightness);
			#if USEFLASH
                    UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
			#else
			SaveMonitorSetting();
			#endif
                }
                else if (SIO_CMD1 == '0' && SIO_CMD2 == 'E')
                {
                    Clr_FactoryModeFlag();
                }
            }
            else if (SIO_TYPE2 == 'B')//  //'W'	'F'	'B'	'P'	'D'	'0'	'0'	'0'
            {
                if (SIO_CMD1 == 'P' && SIO_CMD2 == 'D'&& SIO_DATA1 =='0')//Power Down
                {
		    Power_PowerOffSystem();
                  Power_TurnOnAmberLed();
                }
            }

        }
        #if ENABLE_HDCP
            #if HDCPKEY_IN_Flash//Zhifeng.Wu 20071127
                else if (SIO_TYPE1 == 'H' && SIO_TYPE2 == 'S')
                {
                    if ((SIO_CMD1 == 'T') && (SIO_CMD2 == 'R'))
                    {
                	// HDCP WRITE START
			#if 0//DetectSystemVcc
                        #if UseWDT
                        DisableWDT();
                        #endif
			#endif
                        Set_WriteHDCPcodeFlag();

			TCRCvalue = 0;
			//Flash_Erase_FreeBuffer(FLASH_KEY_HDCP,0); // jeff add it in 2009 0907
			bFlashWriteFactory = TRUE;
			
			   #if 0//Enable_Cache
                        MCU_DisableCache();
                        #endif

			FlashSectorErase(TRUE, HDCPKEYSET_START);
			FlashDisableWP(HDCPKEYSET_START);
                        
                        hw_ClrAmberLed();
                        hw_ClrGreenLed();
						
                    }
                }
                else if (SIO_TYPE1 == 'H' && WriteHDCPcodeFlag)
                // WRITE HDCP DATA
                {
                #if 0
                    WORD wTemp;
                    if(SIO_TYPE2 < (HDCPKEY_SIZE/HDCP_BLOCK_LENGTH))
                        Flash_Write_Factory_KeySet(FLASH_KEY_HDCP, SIO_TYPE2, HDCP_BLOCK_LENGTH,(BYTE*) &SIORxBuffer[3], HDCP_BLOCK_LENGTH);
                    else if(SIO_TYPE2 == (HDCPKEY_SIZE/HDCP_BLOCK_LENGTH))
                    {
                        Flash_Write_Factory_KeySet(FLASH_KEY_HDCP, SIO_TYPE2, HDCP_BLOCK_LENGTH,(BYTE*) &SIORxBuffer[3], HDCPKEY_SIZE%HDCP_BLOCK_LENGTH);
                        for(wTemp=0; wTemp<HDCPKEY_SIZE; wTemp++)
                        {
                            TCRCvalue = CRC16(FlashReadByte(HDCPKEYSET_START+wTemp), TCRCvalue);
                        }
    					Set_LoadHDCPKeyFlag();
    					hw_SetGreenLed();
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
			  #if 0//Enable_Cache
                       MCU_EnableCache();
                       #endif

                        hw_SetGreenLed();
                    }
		  #endif
                }
            #endif
        #endif
    }
    else if ((SIO_TYPE1 == 'V') && (SIO_TYPE2 == 'K'))
    {
        sioData = ((SIO_DATA1 >= '0') && (SIO_DATA1 <= '9')) ? SIOData10 : SIOData11;
        sioData += ((SIO_DATA2 >= '0') && (SIO_DATA2 <= '9')) ? SIOData20 : SIOData21;
        sioData += ((SIO_DATA3 >= '0') && (SIO_DATA3 <= '9')) ? SIOData30 : SIOData31;
        gByteVisualKey = 0xff;
        if ((SIO_CMD1 == '0') && (SIO_CMD2 == '0'))
        {
            if (sioData < 16)
            {
                if (((SIO_DATA3 >= '0') && (SIO_DATA3 <= '9')) || ((SIO_DATA3 >= 'A') && (SIO_DATA3 >= 'F')))
                {
                    if (SIO_DATA3 == '0')
                    {
                        gByteVisualKey &= ~KEY_EXIT;
                    }
                    else if (SIO_DATA3 == '1')
                    {
                        gByteVisualKey &= ~KEY_PLUS;
                    }
                    else if (SIO_DATA3 == '2')
                    {
                        gByteVisualKey &= ~KEY_MINUS;
                    }
                    else if (SIO_DATA3 == '3')
                    {
                        gByteVisualKey &= ~KEY_MENU;
                    }
                    else if (SIO_DATA3 == '4')
                    {
                        gByteVisualKey &= ~KEY_POWER;
                    }
                    else
                        gByteVisualKey = 0xff;
                }
            }
            else if ((SIO_DATA1 == '0') && (sioData > 16)){}
            gBoolVisualKey = 1;
        } //End Virual  key
    }
    return SIORxIndex;
}
