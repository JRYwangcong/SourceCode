 #include "board.h"
#if ENABLE_HDCP
    #include "types.h"
    #include "ms_reg.h"
    #include "debug.h"
    #include "global.h"
    #include "userpref.h"
    #include "msHDCP.h"
    #include "msflash.h"
    #include "misc.h"
    //#include "userprefDef.h"
    // external declaration
    extern void mStar_WriteByte(BYTE addr, BYTE value);
    extern void mStar_WriteWord(BYTE addr, WORD value);
    extern BYTE mStar_ReadByte(BYTE addr);
    extern WORD mStar_ReadWord(BYTE addr);
    extern void NVRam_ReadByte(WORD addr, BYTE *value);
    extern void Delay1ms(WORD msNums);
    extern void printMsg(char *str);
    //Benz 2007.4.23   20:22:16 extern void printData( char *str, WORD value );
    #if (!Init_HDCPKey)
        BYTE code BKSV[5] = 
        {
            0x89, 0x38, 0xae, 0x0d, 0xed
        };
        //////////////////////////////////////////////////////////////
        // HDCP production key
        //////////////////////////////////////////////////////////////
BYTE code HDCPkey[] =
{
    0x00,0xE6,0xD2,0xD0,0xA4,0x0A,0xF9,0xAB,0x2F,0x9A,0x2D,0x3A,
    0xB5,0xFF,0x9D,0x37,0xB3,0x63,0x23,0x6E,0xA3,0x4E,0x58,0xEF,
    0xD2,0x78,0x6D,0x3B,0xDE,0xFD,0x7E,0x4D,0xAA,0xB6,0x0C,0x59,
    0x4F,0x74,0x3C,0xF8,0xDA,0x65,0x6E,0x2F,0xB4,0xAB,0xAD,0x7F,
    0x89,0xA1,0x92,0x25,0x1A,0xCD,0x50,0x93,0x50,0x00,0x63,0x88,
    0x7B,0xD0,0xD8,0x4D,0x46,0x95,0xA8,0xFC,0x9B,0x0D,0xD1,0x08,
    0x06,0xB2,0x67,0xBE,0xA2,0x90,0xA7,0x84,0x2A,0x4B,0x28,0xC8,
    0xDD,0x8F,0xC9,0x48,0xA3,0x0A,0x7D,0xCA,0xDA,0xBC,0xCE,0xFA,
    0x9B,0x9F,0x61,0x90,0x9E,0x46,0x24,0x2B,0xA6,0x7B,0xB1,0x6F,
    0xC4,0x4C,0x63,0xAF,0x44,0xE1,0x3A,0xDD,0x0D,0xED,0xF4,0x45,
    0x36,0x47,0x0B,0x48,0x0A,0xB1,0xE6,0xE9,0xB6,0xED,0x78,0x37,
    0xDC,0xB0,0x07,0x72,0x83,0xDC,0x73,0x26,0x31,0x49,0x03,0x52,
    0x7C,0xE6,0xC6,0xE8,0x3C,0xD1,0x62,0xFE,0xF0,0x01,0x73,0x23,
    0x90,0x70,0xA9,0x52,0x8C,0x75,0x1C,0xE5,0x63,0x7C,0xA9,0x98,
    0x5F,0xD9,0x7C,0x62,0x25,0x6C,0x61,0x6C,0xB5,0xF9,0xDB,0xD8,
    0x91,0x2F,0x25,0xF6,0x3E,0x37,0xBE,0xE2,0x92,0x08,0x07,0x3C,
    0xE6,0xC9,0xE3,0xB0,0x4D,0xF6,0xAD,0x54,0x3C,0xED,0x92,0x5A,
    0x97,0x93,0xC0,0x48,0xEA,0x23,0x6C,0xDD,0x9A,0x00,0xBA,0xF1,
    0xB4,0xE7,0x7E,0x84,0x9A,0xB5,0xD5,0xF4,0xB0,0x42,0x49,0x5E,
    0x15,0x5A,0x43,0x2D,0xA9,0xB0,0x0A,0x1C,0x75,0xDE,0x2E,0x08,
    0xF7,0xDF,0x38,0x03,0xEB,0x6A,0x75,0xEB,0xDA,0x1C,0xA0,0xC9,
    0x2F,0x2A,0x69,0x3F,0x13,0xFB,0xA4,0x23,0x0A,0xE5,0x0A,0xA5,
    0xEF,0xCC,0x8D,0xB6,0x5E,0x6C,0xB6,0xE8,0x84,0xEF,0x03,0xDC,
    0xC5,0x8A,0xA3,0x4B,0xEF,0x44,0x12,0x7F
};
    #endif 
    // 2005/6/3 0:11AM by Emily 
    //*********************************************************
    // HDCP BKSV key
    //*********************************************************
    void msInitHDCPProductionKey(void)
    {
        BYTE ucBank;
        #if Init_HDCPKey
            BYTE TmpValue;
        #endif 
        WORD wCount;
        ucBank = mStar_ReadByte(REGBK); // store bank
        mStar_WriteByte(REGBK, REGBANKSCALER); // select register bank scaler
        // Load HDCP BKSV to register
        mStar_WriteByte(0xBD, 0x00); // HDCP select(I2C Register ID = 74)
        mStar_WriteByte(0xBE, 0x00); // HDCP address port
        for (wCount = 0; wCount < 5; wCount++)
        {
            #if Init_HDCPKey  //0
                #if ENABLE_HDCP
                    #if HDCPKEY_IN_Flash
                        //Flash_ReadHDCPKey(BKSVDataAddr + wCount, &TmpValue, 1);
                        TmpValue = FlashReadByte(HDCPKEYSET_START+BKSVDataAddr+ wCount);
                    #else 
                        Set_HdcpNVRamFlag();
			#if !USEFLASH
                        NVRam_ReadByte(RM_HDCP_KEY_BASEADDR1 + wCount, &TmpValue);
			#endif
                        Clr_HdcpNVRamFlag();
                    #endif 
                #endif 
                mStar_WriteByte(0xBF, TmpValue);
            #else 
                mStar_WriteByte(0xBF, BKSV[wCount]);
            #endif 
        }
        mStar_WriteByte(0xBD, 0x08);
        // Load HDCP key to register
        mStar_WriteByte(0xBD, BIT2); // HDCP internal SRAM
        mStar_WriteByte(0xBE, 0x00); // HDCP address port
        for (wCount = 0; wCount < 284; wCount++)
        {
            #if Init_HDCPKey  //0
                #if ENABLE_HDCP
                    #if HDCPKEY_IN_Flash
                        //Flash_ReadHDCPKey(HDCPKetAddr + wCount, &TmpValue, 1);
                        TmpValue = FlashReadByte(HDCPKEYSET_START+HDCPKetAddr+ wCount);
                    #else 
                        Set_HdcpNVRamFlag();
			#if !USEFLASH
                        NVRam_ReadByte(RM_HDCP_KEY_BASEADDR2 + wCount, &TmpValue);
			#endif
                        Clr_HdcpNVRamFlag();
                    #endif 
                #endif 
                mStar_WriteByte(0xBF, TmpValue);
            #else 
                mStar_WriteByte(0xBF, HDCPkey[wCount]);
            #endif 
        }
        // HDCP
        mStar_WriteByte(BK0_BD, 0x00); // HDCP select(I2C Register ID = 74)
        mStar_WriteByte(BK0_BE, 0x40); // HDCP address port
        //mStar_WriteByte(BK0_BF, 0x00);

	#if ENABLE_HDMI
	mStar_WriteByte(BK0_BF, 0x80);
	#else
        mStar_WriteByte(BK0_BF, 0x00);
	#endif
		
        mStar_WriteByte(REGBK, ucBank); // recovery bank
    }
    ///////////////////////////////////////////////////////////////////////////////
    // <Function>: msPullLowHPD1
    //
    // <Description>: Pull low DVI Hot-Plug-Detection pin
    //
    // <Parameter>: None
    //
    //
    ///////////////////////////////////////////////////////////////////////////////
    void msPullLowHPD1(void)
    {
        hw_Clr_HdcpHpd();
    }
    ///////////////////////////////////////////////////////////////////////////////
    // <Function>: msPullLowHPD1
    //
    // <Description>: Pull high DVI Hot-Plug-Detection pin
    //
    // <Parameter>: None
    //
    //
    ///////////////////////////////////////////////////////////////////////////////
    void msPullHighHPD1(void)
    {
        hw_Set_HdcpHpd();
    }
    ///////////////////////////////////////////////////////////////////////////////
    // <Function>: msPullLowHPD2
    //
    // <Description>: Cut off DVI receiver clock current
    //
    // <Parameter>: None
    //
    ///////////////////////////////////////////////////////////////////////////////
    void msPullLowHPD2(void)
    {
        BYTE ucBank;
        ucBank = mStar_ReadByte(REGBK); // store bank        
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(TESTEN, 0x80);
        mStar_WriteByte(TESTA5, 0x01);
        #ifdef TSUMXXNX
        mStar_WriteByte(BK1_D1, mStar_ReadByte(BK1_D1) |BIT5);
        #else
        mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4) | (BIT0 | BIT1));
        #endif
        //  mStar_WriteByte(TESTA6, 0x00);
        mStar_WriteByte(TESTEN, 0x00);
        mStar_WriteByte(REGBK, ucBank); // recovery bank
        mStar_WriteByte(BK0_BD, mStar_ReadByte(BK0_BD)|BIT7);
    }
    ///////////////////////////////////////////////////////////////////////////////
    // <Function>: msPullHighHPD2
    //
    // <Description>: Connect DVI receiver clock current
    //
    // <Parameter>: None
    //
    ///////////////////////////////////////////////////////////////////////////////
    void msPullHighHPD2(void)
    {
        BYTE ucBank;
        ucBank = mStar_ReadByte(REGBK);
        mStar_WriteByte(REGBK, REGBANKADC);
        mStar_WriteByte(TESTEN, 0x80);
        mStar_WriteByte(BK1_37, BIT7);        
        #if MainBoardType == MainBoard_715G3603_M0A
        if(SrcInputType == Input_Digital)
            mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4)&~BIT0); // connect A port termination
        else
            mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4)&~BIT1); // connect B port termination
        #else
            #ifdef TSUMXXNX
            mStar_WriteByte(BK1_D1, mStar_ReadByte(BK1_D1) &~BIT5);
            #else
            mStar_WriteByte(BK1_E4, mStar_ReadByte(BK1_E4) &~(BIT0 | BIT1));
            #endif
        #endif
        mStar_WriteByte(TESTA5, 0x00);
        mStar_WriteByte(TESTEN, 0x00);
        mStar_WriteByte(REGBK, ucBank);
        mStar_WriteByte(BK0_BD, mStar_ReadByte(BK0_BD) &(~BIT7));
    }
    void msDisableHDCP(void)
    {
        msPullLowHPD1();
        msPullLowHPD2();
    }
    void msEnableHDCP(void)
    {
        if(!PowerSavingFlag)
        {
            msDisableHDCP();
            ForceDelay1ms(300);
        }
        
        msPullHighHPD2();
        msPullHighHPD1();
    }
#endif //ENABLE_HDCP
