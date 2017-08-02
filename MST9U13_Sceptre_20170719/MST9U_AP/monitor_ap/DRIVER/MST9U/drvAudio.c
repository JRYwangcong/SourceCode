
#include "Global.h"

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "drvAudio.h"
#include "regAUDIO.h"
#include "datatype.h"
//#include "msScaler.h"
#include "Global.h"
#include "drvBDMA.h"
//-------------------------------------------------------------------------------------------------
#define MSAUDIO_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG && MSAUDIO_DEBUG
#define MSAUDIO_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MSAUDIO_PRINT(format, ...)
#endif

#if AudioFunc

//-------------------------------------------------------------------------------------------------
//  Control Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Customerize Defines
//-------------------------------------------------------------------------------------------------
#define AUDIO_STATE_DEBUG                       1
#define AUDIO_MUTE_DEBUG                        0




//-------------------------------------------------------------------------------------------------
//  Constant Defines
//-------------------------------------------------------------------------------------------------
#if EnableDACDePop
#define DAC_DELAY_STEP12                    1
#define DAC_DELAY_STEP23                    1550
#define DAC_DELAY_STEP34                    50
#define DAC_DELAY_STEP45                300
#define DAC_DELAY_STEP56                900
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define AUDIO_VOLUME_ARRAY_NUMBER           115

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
code RegTbl2Type DEPOP_INIT_TBL[DE_POP_Finish][9]=
{
    {   //DE_POP_STEP1
        //Address,Value,Mask
        {REG_14020A, 0x7FFF,0x7FFF},//BIT14~BIT1
        {REG_14020C, 0x001F,0x001F},//BIT4~BIT3
        {REG_14020E, 0x0000,0x07E6},//BIT10~BIT5 BIT2~BIT1
        {REG_140210, 0x0000,0x0000},//not defined in excel
        {REG_140212, 0x0000,0x0000},//not defined in excel
        {REG_140214, 0x0000,0x800F},//BIT15,BIT4 ,BIT3~0
        {REG_140216, 0x0000,0x0008},//BIT3
        {REG_140220, 0x0000,0x7E1F},//BIT14~10,BIT9,BIT4,BIT3~0
        {REG_140222, 0x0000,0x0000},//not defined in excel
    },
    {   //DE_POP_STEP2
      //Address,Value,Mask
        {REG_14020A, 0x1000,0x7FFF},//BIT14~BIT1
        {REG_14020C, 0x0000,0x001F},//BIT4~BIT3
        {REG_14020E, 0x0026,0x07E6},//BIT10~BIT5 BIT2~BIT1
        {REG_140210, 0x0000,0x0000},//not defined in excel
        {REG_140212, 0x0000,0x0000},//not defined in excel
        {REG_140214, 0x800F,0x800F},//BIT15,BIT4 ,BIT3~0
        {REG_140216, 0x0000,0x0008},//BIT3
        {REG_140220, 0x0000,0x7E1F},//BIT14~10,BIT9,BIT4,BIT3~0
        {REG_140222, 0x0000,0x0000},//not defined in excel
    },
    {   //DE_POP_STEP3
      //Address,Value,Mask
        {REG_14020A, 0x0000,0x7FFF},//BIT14~BIT1
        {REG_14020C, 0x0000,0x001F},//BIT4~BIT3
        {REG_14020E, 0x00A6,0x07E6},//BIT10~BIT5 BIT2~BIT1
        {REG_140210, 0x0000,0x0000},//not defined in excel
        {REG_140212, 0x0000,0x0000},//not defined in excel
        {REG_140214, 0x800F,0x800F},//BIT15,BIT4 ,BIT3~0
        {REG_140216, 0x0000,0x0008},//BIT3
        {REG_140220, 0x0002,0x7E1F},//BIT14~10,BIT9,BIT4,BIT3~0
        {REG_140222, 0x0000,0x0000},//not defined in excel
    },
    {   //DE_POP_STEP4
      //Address,Value,Mask
        {REG_14020A, 0x0000,0x7FFF},//BIT14~BIT1
        {REG_14020C, 0x0000,0x001F},//BIT4~BIT3
        {REG_14020E, 0x00A6,0x07E6},//BIT10~BIT5 BIT2~BIT1
        {REG_140210, 0x0000,0x0000},//not defined in excel
        {REG_140212, 0x0000,0x0000},//not defined in excel
        {REG_140214, 0x800F,0x800F},//BIT15,BIT4 ,BIT3~0
        {REG_140216, 0x0000,0x0008},//BIT3
        {REG_140220, 0x000E,0x7E1F},//BIT14~10,BIT9,BIT4,BIT3~0
        {REG_140222, 0x0000,0x0000},//not defined in excel
    },
    {   //DE_POP_STEP5
      //Address,Value,Mask
        {REG_14020A, 0x0000,0x7FFF},//BIT14~BIT1
        {REG_14020C, 0x0000,0x001F},//BIT4~BIT3
        {REG_14020E, 0x00E6,0x07E6},//BIT10~BIT5 BIT2~BIT1
        {REG_140210, 0x0000,0x0000},//not defined in excel
        {REG_140212, 0x0000,0x0000},//not defined in excel
        {REG_140214, 0x800F,0x800F},//BIT15,BIT4 ,BIT3~0
        {REG_140216, 0x0000,0x0000},//BIT3
        {REG_140220, 0x000E,0x7E1F},//BIT14~10,BIT9,BIT4,BIT3~0
        {REG_140222, 0x0000,0x0000},//not defined in excel
    },
    {   //DE_POP_STEP6
      //Address,Value,Mask
        {REG_14020A, 0x0000,0x7FFF},//BIT14~BIT1
        {REG_14020C, 0x0000,0x001F},//BIT4~BIT3
        {REG_14020E, 0x00E6,0x07E6},//BIT10~BIT5 BIT2~BIT1
        {REG_140210, 0x0000,0x0000},//not defined in excel
        {REG_140212, 0x0000,0x0000},//not defined in excel
        {REG_140214, 0x000F,0x800F},//BIT15,BIT4 ,BIT3~0
        {REG_140216, 0x0000,0x0008},//BIT3
        {REG_140220, 0x000F,0x7E1F},//BIT14~10,BIT9,BIT4,BIT3~0
        {REG_140222, 0x0000,0x0000},//not defined in excel
    },
};

const U16 AudVolumeTab[AUDIO_VOLUME_ARRAY_NUMBER] =
{
    //   0       1       2       3       4       5       6       7       8       9     OSD  Volume_Range
    0x0000, 0x0008, 0x0010, 0x0018, 0x0020, 0x0028, 0x0030, 0x0038, 0x0040, 0x0048, //  00,   0dB ~ -9dB
    0x0050, 0x0058, 0x0060, 0x0068, 0x0070, 0x0078, 0x0080, 0x0088, 0x0090, 0x0098, //  10, -10dB ~ -19dB
    0x00A0, 0x00A8, 0x00B0, 0x00B8, 0x00C0, 0x00C8, 0x00D0, 0x00D8, 0x00E0, 0x00E8, //  20, -20dB ~ -29dB
    0x00F0, 0x00F8, 0x0100, 0x0108, 0x0110, 0x0118, 0x0120, 0x0128, 0x0130, 0x0138, //  30, -30dB ~ -39dB
    0x0140, 0x0148, 0x0150, 0x0158, 0x0160, 0x0168, 0x0170, 0x0178, 0x0180, 0x0188, //  40, -40dB ~ -49dB
    0x0190, 0x0198, 0x01A0, 0x01A8, 0x01B0, 0x01B8, 0x01C0, 0x01C8, 0x01D0, 0x01D8, //  50, -50dB ~ -59dB
    0x01E0, 0x01E8, 0x01F0, 0x01F8, 0x0200, 0x0208, 0x0210, 0x0218, 0x0220, 0x0228, //  60, -60dB ~ -69dB
    0x0230, 0x0238, 0x0240, 0x0248, 0x0250, 0x0258, 0x0260, 0x0268, 0x0270, 0x0278, //  70, -70dB ~ -79dB
    0x0280, 0x0288, 0x0290, 0x0298, 0x02A0, 0x02A8, 0x02B0, 0x02B8, 0x02C0, 0x02C8, //  80, -80dB ~ -89dB
    0x02D0, 0x02D8, 0x02E0, 0x02E8, 0x02F0, 0x02F8, 0x0300, 0x0308, 0x0310, 0x0318, //  90, -90dB ~ -99dB
    0x0320, 0x0328, 0x0330, 0x0338, 0x0340, 0x0348, 0x0350, 0x0358, 0x0360, 0x0368, // 100,-100dB ~-109dB
    0x0370, 0x0378, 0x0380, 0x0388, 0x0390                                                         // 110,-110dB ~-114dB
};


//-------------------------------------------------------------------------------------------------
// Mute Defines
//-------------------------------------------------------------------------------------------------
static  BOOLEANS m_BOOLEANS_1;
#define m_bDPGA_InitAudioMute           m_BOOLEANS_1.bBOOL_0
#define m_bDPGA_SourceChangeAudioMute   m_BOOLEANS_1.bBOOL_1
#define m_bDPGA_MenuAudioMute           m_BOOLEANS_1.bBOOL_2
#define m_bDPGA_AVSyncAudioMute         m_BOOLEANS_1.bBOOL_3
#define m_bDPGA_TimingChangeAudioMute   m_BOOLEANS_1.bBOOL_4
#define m_bDPGA_ModeChangeAudioMute     m_BOOLEANS_1.bBOOL_5
#define m_bDPGA_DelayChangeAudioMute    m_BOOLEANS_1.bBOOL_6
#define m_bDPGA_Internal1AudioMute      m_BOOLEANS_1.bBOOL_7

static  BOOLEANS m_BOOLEANS_2;
#define m_bSW_MailboxAudioMute          m_BOOLEANS_2.bBOOL_0
#define m_bSW_SourceChangeAudioMute     m_BOOLEANS_2.bBOOL_1
#define m_bSW_InitAudioMute             m_BOOLEANS_2.bBOOL_2
#define m_bSW_MenuAudioMute             m_BOOLEANS_2.bBOOL_3
#define m_bSW_AVSyncAudioMute           m_BOOLEANS_2.bBOOL_4
#define m_bSW_TimingChangeAudioMute     m_BOOLEANS_2.bBOOL_5
#define m_bSW_ModeChangeAudioMute       m_BOOLEANS_2.bBOOL_6
#define m_bSW_Internal1AudioMute        m_BOOLEANS_2.bBOOL_7


//-------------------------------------------------------------------------------------------------
// Local Variable
//-------------------------------------------------------------------------------------------------
#if ENABLE_DAC
static MS_BOOL                          audio_pad_init_flag         = 0;
static MS_BOOL                          audio_dpga_mute_flag        = 0;

static U16                              dpga0_volume_lt_store       = 0;
static U16                              dpga0_volume_rt_store       = 0;
static U16                              dpga1_volume_lt_store       = 0;
static U16                              dpga1_volume_rt_store       = 0;
static U16                              dpga2_volume_lt_store       = 0;
static U16                              dpga2_volume_rt_store       = 0;
static U16                              dpga3_volume_lt_store       = 0;
static U16                              dpga3_volume_rt_store       = 0;

DAC_INIT_STEP                           AudioDePopSequence = DE_POP_STEP1;

//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///           AUDIO DRV FUNCTION
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//******************************************************************************


/******************************************************************************/
/// @brief \b Function \b Name:  msDrvAudio_MIU_SetByPassEn(enable)
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_MIU_SetByPassEn(MS_BOOL enable)
{
    //Audio MIU Byass enable/disable
    if (enable)
    {
        msWrite2ByteMask(REG_140202, 0x0010, 0x0010);                                      // 0x2C02[4] = 1, MIU bypass
        #if (AUDIO_STATE_DEBUG)
            MSAUDIO_PRINT("---------- AUDIO MIU ByPass Enable --------- \r\n");
        #endif
    }
    else
    {
        msWrite2ByteMask(REG_140202, 0x0000, 0x0010);                                      // 0x2C02[4] = 0, MIU bypass disable
        #if (AUDIO_STATE_DEBUG)
            MSAUDIO_PRINT("---------- AUDIO MIU ByPass Disable --------- \r\n");
        #endif
    }
}

/******************************************************************************/
/// @brief \b Function \b Name:  msDrvAudio_MIU_GetByPassInfo()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
MS_BOOL msDrvAudio_MIU_GetByPassInfo(void)
{
    MS_U16  temp;
    temp = msRead2Byte(REG_140202) & 0x0010;                                                     // 0x2C02[4] = 1, MIU bypass
    if (temp == 0x0010)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}


//==============================================================================
// Audio INIT Relation
//==============================================================================
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_PLL_SetINI()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_PLL_SetINI(void)
{
       //  Audio PLL Setting
       msWrite2ByteMask(REG_162F12, 0x0022, 0x60FF);      // 0x162F12[7:4]:KM, [3:0]:KP
                                                                                // 0x162F13[6:5]:KN
        msWrite2ByteMask(REG_162F14, 0x6200, 0xFF00);      // 0x162F15[7:4]:fbdiv, [3:0]:ddiv
        msWrite2ByteMask(REG_162F06, 0x0000, 0x0100);      // 0x162F07[0]:apll power down off
        msWrite2ByteMask(REG_162F0E, 0x0050, 0x00FF);      // 0x162F0E[0]:test mode to force pll lock
        msWrite2ByteMask(REG_162F06, 0x0001, 0x0001);      // 0x162F0E[0]:test mode to force pll lock

        msWrite2ByteMask(REG_163002, 0x0010, 0x0010);      // b[4]: 192fs
        //msWrite2ByteMask(REG_14026C, 0x0010, 0x0070);      // bit clock 48kHz

        //Audio Output Pad Control
        msWrite2ByteMask(REG_1630B2, 0x0000, 0x003F);      // 0 is enable; [3:0]:i2s out en, [4]:spidf out, [5]:i2s mute
}
#if 0
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_ADC_SetINI()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_ADC_SetINI(void)
{
    MSAUDIO_PRINT("@@@@@@@@@@@@ msDrvAudio_ADC_SetINI  \r\n",0);
    msDrvAudio_PLL_SetINI();
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_HDMI_SetINI()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
//void msDrvAudio_HDMI_SetINI(void)
{
        //Audio Pad Setting
        MSAUDIO_PRINT("@@@@@@@@@@@@ msDrvAudio_HDMI_SetINI   \r\n",0);
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_DP_SetINI()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_DP_SetINI(void)
{
    //  Audio Pad Setting
    MSAUDIO_PRINT("@@@@@@@@@@@@ msDrvAudio_DP_SetINI   \r\n ",0);
        return;
        msWrite2ByteMask(REG_000406, 0x0300, 0x0F00);         // 0x407[0]:i2s_out_en; 0x407[1]:i2s_out_mck_en
                                                            // 0x407[7:0] = 0xF0 I2S is from HDMI
                                                            // 0x407[7:0] = 0x0F I2S is from Audio
        msWrite2ByteMask(REG_000406, 0x0000, 0x0F00);         // 0x407[7:0]: for Audio I2S Out
        msWrite2ByteMask(REG_000416, 0x0000, 0x00FF);         // 0x416[7:0]: i2s data out en
        //msWrite2ByteMask(REG_14026C, 0x0010, 0x0070);      // 0x14026C[6:4] REG_SEL_I2S_TX_BCK, I2S BCK clock selection.
                                                            // 000: AUPLL 32FS,  001: AUPLL 48FS, 010: AUPLL 64FS.

    //  GPIO A0 (shutdown pin setting)
        msWrite2ByteMask(REG_000406, 0x0000, 0x0020);         // 0x406[5]: disable AV mute control
        msWrite2ByteMask(REG_000406, 0x0000, 0x1000);         // 0x407[4]: disable AV mute control (au)
        msWrite2ByteMask(REG_00045C, 0x0000, 0x0001);         // 0x45C[0]: GPIO A0 OEN control
}
#endif


#if EnableDACDePop
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioPoweronDePop()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioPoweronDePop(DAC_INIT_STEP DePopSequence)
{
    BYTE uctemp=0;

    if (bAudioDePopFlag == 0)
        return;
    for(uctemp = 0; uctemp <(sizeof(DEPOP_INIT_TBL)/sizeof(RegTbl2Type)/DE_POP_Finish); uctemp++)
    {
            msWrite2Byte(DEPOP_INIT_TBL[DePopSequence][uctemp].dwReg, DEPOP_INIT_TBL[DePopSequence][uctemp].wValue);
            //MSAUDIO_PRINT(" ---------DePopSequence ==%x , ---uctemp ==%x\r\n",DePopSequence,uctemp);
            //MSAUDIO_PRINT(" ---------dwReg ==%x ---wValue==%x    \r\n",DEPOP_INIT_TBL[DePopSequence][uctemp].dwReg,DEPOP_INIT_TBL[DePopSequence][uctemp].wValue);
    }
    switch(DePopSequence)
    {
        case DE_POP_STEP1:
            MSAUDIO_PRINT("--- Audio De-Pop Step 1\r\n");
            AudioDePopSequence = DE_POP_STEP2;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP12);
            break;

        case DE_POP_STEP2:
            // Eanle DAC and OPLP and CLK(for DAC)
            MSAUDIO_PRINT("--- Audio De-Pop Step 2\r\n");

            AudioDePopSequence = DE_POP_STEP3;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP23);
            break;

        case DE_POP_STEP3:
            // Enable LP Loop and Select Channel One
            MSAUDIO_PRINT("--- Audio De-Pop Step 3\r\n");
            AudioDePopSequence = DE_POP_STEP4;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP34);
            break;

        case DE_POP_STEP4:
            // Enable SW1
            MSAUDIO_PRINT("--- Audio De-Pop Step 4\r\n");
            AudioDePopSequence = DE_POP_STEP5;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP45);
            break;

        case DE_POP_STEP5:
            // Enable SW2
            MSAUDIO_PRINT("--- Audio De-Pop Step 5\r\n");
            AudioDePopSequence = DE_POP_STEP6;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP56);

            break;

        case DE_POP_STEP6:
            // Enable Class-AB and Close vref quick start
            MSAUDIO_PRINT("--- Audio De-Pop Step 6\r\n");
            msDrvAudio_SetDPGAMuteSrc(E_DPGA_INIT_MUTE_OFF);
            AudioDePopSequence = DE_POP_Finish;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(0);
            break;

        case DE_POP_Finish:
            MSAUDIO_PRINT("--- Audio DE_POP_Finish\r\n");
            break;

        default:
            //AudioDePopSequence = DE_POP_STEP1;
            //u16AudioDePopCounter = 0;
            break;
    }
}
#else
void msDrvAudioPoweronDACwithoutDePop(void)
{
    BYTE uctemp1,uctemp2;
    for(uctemp1 = DE_POP_STEP1; uctemp1 <DE_POP_Finish; uctemp1++)
    {
        for(uctemp2 = 0; uctemp2 <(sizeof(DEPOP_INIT_TBL)/sizeof(RegTbl2Type)/DE_POP_Finish); uctemp2++)
        {
                msWrite2Byte(DEPOP_INIT_TBL[uctemp1][uctemp2].dwReg, DEPOP_INIT_TBL[uctemp1][uctemp2].wValue);
                printf(" ---------DePopSequence ==%x , ---uctemp ==%x\r\n",uctemp1,uctemp2);
                printf(" ---------dwReg ==%x ---wValue==%x    \r\n",DEPOP_INIT_TBL[uctemp1][uctemp2].dwReg,DEPOP_INIT_TBL[uctemp1][uctemp2].wValue);
        }
    }
}
#endif
void msDrvAudioDigitalHardwareInit( void )
{
        MSAUDIO_PRINT("----- Audio DHW Init  ----- \r\n");
        //======= enable ADC settings =======//
        msWrite2Byte(REG_14020C, 0x0000);//remove?
        msWrite2Byte(REG_14020E, 0x00E6);//remove?
        msWrite2Byte(REG_140212, 0x0E00);//remove?

        //======= reset =======//
        msWrite2Byte(REG_140200, 0xFFFF);             // 0x140200
        msWrite2Byte(REG_140200, 0x0000);             // 0x140200

        //======= 2nd order synthesizer =======//
        msWrite2Byte(REG_14022C, 0x1C20);  // 0x14022C     // 48K
        //msWrite2Byte(REG_AUDIO_2nd_Order_Synth_1, 0x2C30);  // 0x14022C     // 32K
        msWrite2Byte(REG_14022A, 0x4000);  // 0x14022A

        //======= I2S TX =======//
        msWrite2Byte(REG_14026C, 0x0021);       // 0x14026c   [6:4] REG_SEL_I2S_TX_BCK 010: AUPLL 64FS. [2:0] REG_SEL_I2S_TX_MCK 001: AUPLL 256FS.

        //======= SPDIF TX =======//
        msWrite2Byte(REG_140260, 0x0000);     // 0x140260

        //======= PAD control =======//
        msWrite2Byte(REG_14027C, 0xE401);          // 0x14027c
        msWrite2Byte(REG_14027E, 0x0000);          // 0x14027e

        //msWrite2ByteMask(REG_GPIO_PAD_AUD_I2S, 0x00F0, 0x00FF);         // 0x000416
        //msWrite2ByteMask(REG_GPIO_PAD_AUD_CFG1, 0x7000, 0x7700);        // 0x000406, [2]: spdif config in Init_SPIFGPIO_Pin

        //======= CH1 bypass disable =======//
        msWrite2Byte(REG_140330, 0xD0F0);       // 0x140330
        msWrite2Byte(REG_140332, 0x0000);       // 0x140332

        //======= HDMI RX singen enable =======//
        msWrite2Byte(REG_14021A, 0x0005);      // 0x14021a
        msWrite2Byte(REG_140218, 0x0004);      // 0x140218

        //======= CH1 input select =======//
        msWrite2Byte(REG_140252, 0x0000);                   // Select HDMI

        //======= Earphone Mute settings =======//

        msWrite2ByteMask(REG_140220, 0x0000, 0x0001);
        Delay1ms(1);
        msWrite2ByteMask(REG_140220, 0x0000, 0x000C);
        msWrite2ByteMask(REG_140220, 0x0000, 0x0002);
        msWrite2ByteMask(REG_140220, 0x0010, 0x0010);


        //======= DPGA settings =======//
        msWrite2Byte(REG_140302, 0x038F);
        msWrite2Byte(REG_140304, 0x038F);
        msWrite2Byte(REG_14030A, 0x038F);
        msWrite2Byte(REG_14030C, 0x038F);
        msWrite2Byte(REG_140312, 0x038F);
        msWrite2Byte(REG_140314, 0x038F);
        msWrite2Byte(REG_14031A, 0x038F);
        msWrite2Byte(REG_14031C, 0x038F);

        //[14:12][10:8]:Step size for fading function in L/R Channel
        //[6]:Zero crossing detection enable
        //[5]:All channel refer to the DPGA1 left channel(boundle)
        //[4]:Rught channel boundle with left channel
        //[3]:fading enable
        //[2]:DPGA enable
        //[1:0]:DPGA gain trigger(L/R)
        msWrite2ByteMask(REG_140300, 0x330C,0x07FFF);
        msWrite2ByteMask(REG_140308, 0x330C,0x07FFF);
        msWrite2ByteMask(REG_140310, 0x330C,0x07FFF);
        msWrite2ByteMask(REG_140318, 0x330C,0x07FFF);
        //msWrite2ByteMask(REG_140300, 0x0067,0x007F);
        //msWrite2Byte(REG_140320, 0x0017);                 // SPDIF

        msWriteByte(REG_140306, 0x01);//DPGA timeout number
        msWriteByte(REG_14030E, 0x01);
        msWriteByte(REG_140316, 0x01);
        msWriteByte(REG_14031E, 0x01);

        //msWrite2ByteMask(REG_140300, 0x0067,0x007F);
        //msWrite2Byte(REG_140320, 0x0017);                 // SPDIF


        //======= Disable init sram =======//
        msWrite2Byte(REG_1403DE, 0x0000);
        msWrite2Byte(REG_1402E8, 0x0003);

        //======= CH1 SRC enable =======//
        msWrite2Byte(REG_AUDIO_SRC_CTRL0, 0x0000);
        msWrite2Byte(REG_AUDIO_SRC_CTRL1, 0xFF00);

        //======= CH1 setting =======//
        msWrite2Byte(REG_AUDIO_CH1_CTRL, 0x0090);    //[7] enable ch1 [4] MIU Bypass
        //======= DC Offset and Dither =======//
        msWrite2ByteMask(REG_140338, 1<<5, 0x00E0);  //0.5x dither
        msWrite2Byte(REG_14033C, 0x80);  //offset
        msWrite2ByteMask(REG_140336, 0x0077, 0x0077);  //Enable dither and offset

    //msWrite2ByteMask(REG_00045C, 0x0000, 0x0001);             // 0x45C[0]: GPIO A0 OEN control
    //msWrite2ByteMask(REG_00045A, 0x0001, 0x0001);             // 0x45A[0]: shutdown = 0

        msWrite2Byte(REG_140224, 0x24);  // 20131114 default is 0xE4, Set to 0x24 to solve Earout no sound issue
        //msWrite2Byte(REG_140224, 0x001B);  //mapping ch0~3 to DAC3~0 to solve earout no sound if input is digital port.

        msWrite2Byte(REG_1403E0, 0x38C0);  //(dither clipping protect at large sound)
        msWrite2Byte(REG_1403E2, 0x3838);  //(dither clipping protect at large sound)

        //pad setting
        #if ENABLE_SPDIF
        Init_SPIFGPIO_Pin(); //SPDIF -> PAD
        #endif
        #if ENABLE_I2S //combo -> Hydan -> PAD
        Init_I2S_Switch_Pin();
        #endif
}


void msDrvAudioDACInit( void )
{
    //BYTE i=0;

    msWrite2Byte(REG_AUDIO_AUPLL_CFG1, 0x1500);                 // 0x101ED4
    msWrite2Byte(REG_AUDIO_AUPLL_CFG2, 0x1E06);                 // 0x101ED6
    msWrite2Byte(REG_AUDIO_AUPLL_CFG3, 0x00A8);                 // 0x101ED8

    //======= Clock Enable=======//
    msWrite2Byte(REG_14024C, 0xFFFF);
    msWrite2Byte(REG_14024E, 0xFFFF);

    //======= CH1 output SDM enable =======//
    msWrite2Byte(REG_140338, 0xF001);

    msDrvAudio_PLL_SetINI();

    msDrvAudioDigitalHardwareInit();

#if EnableDACDePop
    bAudioDePopFlag=1;
    msDrvAudioPoweronDePop(DE_POP_STEP1);
#else
    msDrvAudioPoweronDACwithoutDePop();
#endif
    MSAUDIO_PRINT("... Audio Initialize .777..\r\n");
}
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioSetInit()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioSetInit( void )
{
#if EnableDACDePop
        bAudioDePopFlag = 0;
#endif
        m_bDPGA_InitAudioMute           = FALSE;
        m_bDPGA_SourceChangeAudioMute   = FALSE;
        m_bDPGA_MenuAudioMute           = FALSE;
        m_bDPGA_AVSyncAudioMute         = FALSE;
        m_bDPGA_TimingChangeAudioMute   = FALSE;
        m_bDPGA_ModeChangeAudioMute     = FALSE;
        m_bDPGA_DelayChangeAudioMute     = FALSE;
        m_bDPGA_Internal1AudioMute      = FALSE;

        m_bSW_MailboxAudioMute          = FALSE;
        m_bSW_SourceChangeAudioMute     = FALSE;
        m_bSW_InitAudioMute             = FALSE;
        m_bSW_MenuAudioMute             = FALSE;
        m_bSW_AVSyncAudioMute           = FALSE;
        m_bSW_TimingChangeAudioMute     = FALSE;
        m_bSW_ModeChangeAudioMute       = FALSE;
        m_bSW_Internal1AudioMute        = FALSE;

}


//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAudioDPGA_SetVolume()
/// @brief \b Function \b Description: Set Audio DPGA Volume
/// @param <IN>        \b eDPGA: DPGA channle,
///                    \b        mapping -> DPGA0:DAC0, DPGA1:DAC1, DPGA2:DAC2m DPGA3:DAC3
/// @param <IN>        \b eChSelect: Channel selection
/// @param <IN>        \b Volume_Integer: Volume Integer Part from 0 ~ -113dB
/// @param <IN>        \b Volume_Fraction: Volume Fraction Part, -0.125dB per unit
///                    \b        sel in AUDIO_VOLUME_FRACTION_PART
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioDPGASetVolume( AUDIO_DPGA_SEL eDPGA, AUDIO_CH_SEL eChSelect, signed short iVolume_Integer, AUDIO_VOLUME_FRACTION_PART Volume_Fraction )
{
    BYTE u8Offset = eDPGA*8;
    U8   voltab_index;
    U16  reg16Volume;

    if (iVolume_Integer > 0)                         // (+911) * -0.125dB = -113.875dB
    {
        iVolume_Integer = 0;                         // maximum is 0dB
    }
    else if (iVolume_Integer <= -113)
    {
        iVolume_Integer = -113;                      //minimum
    }

    voltab_index = abs(iVolume_Integer);
    reg16Volume = AudVolumeTab[voltab_index]+Volume_Fraction;
    #if 0
        MSAUDIO_PRINT("--->1. volume_Integer=%d, voltab_index= %d\r\n",iVolume_Integer, voltab_index);
        MSAUDIO_PRINT("--->2. Vol Ing =%x, Vol Fra=%x\r\n",AudVolumeTab[voltab_index],Volume_Fraction);
        MSAUDIO_PRINT("--->3. Register Vol=%x\r\n",reg16Volume);
    #endif

    switch(eDPGA)
    {
        case AUDIO_DPGA0:
            if (eChSelect == E_AUDIO_LEFT_CH)
            {
                dpga0_volume_lt_store = reg16Volume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga0_volume_rt_store = reg16Volume;
            }
            else
            {
                dpga0_volume_lt_store = reg16Volume;
                dpga0_volume_rt_store = reg16Volume;
            }
            break;

        case AUDIO_DPGA1:
            if (eChSelect == E_AUDIO_LEFT_CH)
            {
                dpga1_volume_lt_store = reg16Volume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga1_volume_rt_store = reg16Volume;
            }
            else
            {
                dpga1_volume_lt_store = reg16Volume;
                dpga1_volume_rt_store = reg16Volume;
            }
           break;

        case AUDIO_DPGA2:
            if (eChSelect == E_AUDIO_LEFT_CH)
            {
                dpga2_volume_lt_store = reg16Volume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga2_volume_rt_store = reg16Volume;
            }
            else
            {
                dpga2_volume_lt_store = reg16Volume;
                dpga2_volume_rt_store = reg16Volume;
            }
           break;

        case AUDIO_DPGA3:
            if (eChSelect == E_AUDIO_LEFT_CH)
            {
                dpga3_volume_lt_store = reg16Volume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga3_volume_rt_store = reg16Volume;
            }
            else
            {
                dpga3_volume_lt_store = reg16Volume;
                dpga3_volume_rt_store = reg16Volume;
            }
            break;

        case AUDIO_DPGA_ALL:
        default:
                dpga0_volume_lt_store = reg16Volume;
                dpga0_volume_rt_store = reg16Volume;
                dpga1_volume_lt_store = reg16Volume;
                dpga1_volume_rt_store = reg16Volume;
                dpga2_volume_lt_store = reg16Volume;
                dpga2_volume_rt_store = reg16Volume;
                dpga3_volume_lt_store = reg16Volume;
                dpga3_volume_rt_store = reg16Volume;
            break;
    }

    if( eDPGA == AUDIO_DPGA_ALL )
    {
        //msWriteByteMask(REG_140300, BIT5, BIT5);    //boundle all DPGA to DPGA1 left channel   ,enable it then fading is not useful
        eChSelect = E_AUDIO_LEFT_RIGHT_CH;
        u8Offset = 0;
    }
    else
    {
        msWriteByteMask(REG_140300, 0x00, BIT5);
    }

    switch(eChSelect)
    {
        case E_AUDIO_LEFT_CH:
            msWrite2ByteMask(REG_140302 + u8Offset,  reg16Volume    ,0x07FF);
            break;

        case E_AUDIO_RIGHT_CH:
            msWrite2ByteMask(REG_140304 + u8Offset, reg16Volume , 0x07FF);
            break;

        case E_AUDIO_LEFT_RIGHT_CH:
            if( eDPGA == AUDIO_DPGA_ALL )
            {
                msWrite2ByteMask(REG_140302, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_140304, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_14030A, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_14030C, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_140312, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_140314, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_14031A, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_14031C, reg16Volume, 0x07FF);
            }
            else
            {
                msWrite2ByteMask(REG_140302 + u8Offset, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_140304 + u8Offset, reg16Volume, 0x07FF);
            }
            break;
        default:
            break;
    }
    if( eDPGA == AUDIO_DPGA_ALL )
    {
        msWriteByteMask(REG_140300, 0x00, BIT1|BIT0);                       // BIT0:Trigger hardware to set the left-channel DPGA's gain.
        msWriteByteMask(REG_140300, BIT1|BIT0, BIT1|BIT0);
        msWriteByteMask(REG_140300, 0x00, BIT1|BIT0);
        msWriteByteMask(REG_140308, 0x00, BIT1|BIT0);                       // BIT0:Trigger hardware to set the left-channel DPGA's gain.
        msWriteByteMask(REG_140308, BIT1|BIT0, BIT1|BIT0);
        msWriteByteMask(REG_140308, 0x00, BIT1|BIT0);
        msWriteByteMask(REG_140310, 0x00, BIT1|BIT0);                       // BIT0:Trigger hardware to set the left-channel DPGA's gain.
        msWriteByteMask(REG_140310, BIT1|BIT0, BIT1|BIT0);
        msWriteByteMask(REG_140310, 0x00, BIT1|BIT0);
        msWriteByteMask(REG_140318, 0x00, BIT1|BIT0);                       // BIT0:Trigger hardware to set the left-channel DPGA's gain.
        msWriteByteMask(REG_140318, BIT1|BIT0, BIT1|BIT0);
        msWriteByteMask(REG_140318, 0x00, BIT1|BIT0);
    }
    else
    {
        msWriteByteMask(REG_140300 + u8Offset, 0x00, BIT1|BIT0);             // BIT0:Trigger hardware to set the left-channel DPGA's gain.
                                                                                                // BIT1:Trigger hardware to set the right-channel DPGA's gain.
        msWriteByteMask(REG_140300 + u8Offset, /*BIT3|*/BIT1|BIT0,/*BIT3|*/BIT1|BIT0);
        msWriteByteMask(REG_140300 + u8Offset, 0x00,BIT1|BIT0);
    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioDPGA_SetVolRegMute(eDPGA)
/// @brief \b Function \b Description: Store DPGA gain
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioDPGA_SetVolRegMute(void)
{
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_LEFT, 0x0388, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_RIGHT, 0x0388, 0x07FF);
        // Disable zero-crossing detect(Bit6 = 0) for some non zero-crossing audio pattern
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0007/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);

        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_LEFT, 0x0388, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_RIGHT, 0x0388, 0x07FF);
        // Disable zero-crossing detect(Bit6 = 0) for some non zero-crossing audio pattern
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0007/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);

        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_LEFT, 0x0388, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_RIGHT, 0x0388, 0x07FF);
        // Disable zero-crossing detect(Bit6 = 0) for some non zero-crossing audio pattern
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0007/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);

        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_LEFT, 0x0388, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_RIGHT, 0x0388, 0x07FF);
        // Disable zero-crossing detect(Bit6 = 0) for some non zero-crossing audio pattern
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0007/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        //g_bDebugProgStopFlag=1;

        MSAUDIO_PRINT("@@@@@@@@@@@> msDrvAudioDPGA_SetVolRegMute,  \r\n", 0);

        audio_dpga_mute_flag = 1;
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAudioDPGA_StoreVolume(eDPGA)
/// @brief \b Function \b Description: Store DPGA gain
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioDPGA_RecoverVolume(void)
{
    if (audio_dpga_mute_flag)
    {
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_LEFT, dpga3_volume_lt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_RIGHT, dpga3_volume_rt_store, 0x07FF);
        // Disable zero-crossing detect(Bit6 = 0) for some non zero-crossing audio pattern
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0007/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);

        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_LEFT, dpga2_volume_lt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_RIGHT, dpga2_volume_rt_store, 0x07FF);
        // Disable zero-crossing detect(Bit6 = 0) for some non zero-crossing audio pattern
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0007/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);

        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_LEFT, dpga1_volume_lt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_RIGHT, dpga1_volume_rt_store, 0x07FF);
        // Disable zero-crossing detect(Bit6 = 0) for some non zero-crossing audio pattern
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0007/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);

        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_LEFT, dpga0_volume_lt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_RIGHT, dpga0_volume_rt_store, 0x07FF);
        // Disable zero-crossing detect(Bit6 = 0) for some non zero-crossing audio pattern
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0007/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0004/*|BIT6*//*|BIT3*/, 0x0007|BIT5|BIT6/*|BIT3*/);
        MSAUDIO_PRINT("@@@@@@@@@@@> msDrvAudioDPGA_RecoverVolume,  \r\n", 0);

        audio_dpga_mute_flag = 0;
    }

}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_SetSoftMute()
/// @brief \b Function \b Description: Mute by Software from MCU to DMA Reader
/// @param <IN>        \b enable - 1: mute, 0: unmute
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_SetSoftMuteSrcSel(AUDIO_MUTE_SOURCE_SEL mute_src_type)
{
    MS_BOOL SoftMuteFlag;
    MS_BOOL  temp_bypassflag;

    temp_bypassflag = msDrvAudio_MIU_GetByPassInfo();

    if (temp_bypassflag)
    {
        //MSAUDIO_PRINT("--- Audio MIU Bypass mode don't Support Software Mute ---  \r\n");
    }
    else
    {
        switch(mute_src_type)
        {
            case E_SW_INIT_MUTE_OFF:
                m_bSW_InitAudioMute = FALSE;
                break;

            case E_SW_INIT_MUTE_ON:
                m_bSW_InitAudioMute = TRUE;
                break;

            case E_SW_SOURCE_CHANGE_MUTE_OFF:
                m_bSW_SourceChangeAudioMute = FALSE;
                break;

            case E_SW_SOURCE_CHANGE_MUTE_ON:
                m_bSW_SourceChangeAudioMute = TRUE;
                break;

            case E_SW_MENU_MUTE_OFF:
                m_bSW_MenuAudioMute = FALSE;
                break;

            case E_SW_MENU_MUTE_ON:
                m_bSW_MenuAudioMute = TRUE;
                break;

            case E_SW_AVSYNC_MUTE_OFF:
                m_bSW_AVSyncAudioMute = FALSE;
                break;

            case E_SW_AVSYNC_MUTE_ON:
                m_bSW_AVSyncAudioMute = TRUE;
                break;

            case E_SW_MAILBOX_MUTE_OFF:
                m_bSW_MailboxAudioMute = FALSE;
                break;

            case E_SW_MAILBOX_MUTE_ON:
                m_bSW_MailboxAudioMute = TRUE;
                break;

            case E_SW_TIMING_CHANGE_MUTE_OFF:
                m_bSW_TimingChangeAudioMute = FALSE;
                break;

            case E_SW_TIMING_CHANGE_MUTE_ON:
                m_bSW_TimingChangeAudioMute = TRUE;
                break;

            case E_SW_MODE_CHANGE_MUTE_OFF:
                m_bSW_ModeChangeAudioMute = FALSE;
                break;

            case E_SW_MODE_CHANGE_MUTE_ON:
                m_bSW_ModeChangeAudioMute = TRUE;
                break;

            case E_SW_INTERNAL_1_MUTE_OFF:
                m_bSW_Internal1AudioMute = FALSE;
                break;

            case E_SW_INTERNAL_1_MUTE_ON:
                m_bSW_Internal1AudioMute = TRUE;
                break;

            default:
                  break;
        }

    #if (AUDIO_MUTE_DEBUG)
        MSAUDIO_PRINT("--> audio software mute status: p:%d   \r\n", m_bSW_InitAudioMute);
        MSAUDIO_PRINT("sr:%d,   \r\n",m_bSW_SourceChangeAudioMute);
        MSAUDIO_PRINT("me:%d,   \r\n",m_bSW_MenuAudioMute);
        MSAUDIO_PRINT("av:%d,  \r\n",m_bSW_AVSyncAudioMute);
        MSAUDIO_PRINT("ma:%d,  \r\n",m_bSW_MailboxAudioMute);
        MSAUDIO_PRINT("ti:%d,  \r\n",m_bSW_TimingChangeAudioMute);
        MSAUDIO_PRINT("mo:%d,  \r\n",m_bSW_ModeChangeAudioMute);
        MSAUDIO_PRINT("i1:%d,  \r\n",m_bSW_Internal1AudioMute);
    #endif

        SoftMuteFlag =
            m_bSW_InitAudioMute|
            m_bSW_SourceChangeAudioMute|
            m_bSW_MenuAudioMute|
            m_bSW_AVSyncAudioMute|
            m_bSW_MailboxAudioMute|
            m_bSW_TimingChangeAudioMute|
            m_bSW_ModeChangeAudioMute|
            m_bSW_Internal1AudioMute;


        if (SoftMuteFlag)
        {
            msWrite2ByteMask(REG_AUDMBOX_SW_MUTE_140340, 0x0001, 0x0001);
        }
        else
        {
            msWrite2ByteMask(REG_AUDMBOX_SW_MUTE_140340, 0x0000, 0x0001);
        }

        //MSAUDIO_PRINT("REG_AUDMBOX_SW_MUTE_140340====%d,  \r\n",msRead2Byte(REG_AUDMBOX_SW_MUTE_140340));

    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_SetEarphoneMute()
/// @brief \b Function \b Description: Earphone Mute.
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_SetEarphoneMute(MS_BOOL enable)
{
    if (enable)
    {

        msWrite2ByteMask(REG_140220, 0x0000, 0x0001);
        Delay1ms(1);
        msWrite2ByteMask(REG_140220, 0x0000, 0x000C);
        Delay1ms(1);
        msWrite2ByteMask(REG_140220, 0x0000, 0x0002);
        Delay1ms(1);
        msWrite2ByteMask(REG_140220, 0x0010, 0x0010);
    }
    else
    {
        msWrite2ByteMask(REG_140220, 0x0002, 0x0002);
        Delay1ms(5);
        msWrite2ByteMask(REG_140220, 0x0000, 0x0010);
        Delay1ms(1);
        msWrite2ByteMask(REG_140220, 0x000C, 0x000C);
        Delay1ms(1);
        msWrite2ByteMask(REG_140220, 0x0001, 0x0001);
    }
}

#if 0
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_PAD_MUTE()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_PAD_MUTE(MS_BOOL enable)
{
    U16     temp;
    if (enable)
    {
        msWrite2ByteMask(REG_000416, 0x0000, 0x00FF);         // 0x416[7:0]: i2s data out disable
        msWrite2ByteMask(REG_000406, 0x0020, 0x0020);         // 0x406[5]: enable AV mute control
        msWrite2ByteMask(REG_000406, 0x1000, 0x1000);         // 0x407[4]: enable AV mute control (au)
    }
    else
    {
        temp = (msRead2Byte(REG_140252) & 0x0070) >> 4;
        if (temp == AUDIO_MUX_HDMI_DP_DECODER)
        {
            msWrite2ByteMask(REG_000416, 0x00F0, 0x00FF);         // 0x416[7:0]: i2s data out en
        }
        else if (temp == AUDIO_MUX_ADC_INPUT)
        {
            msWrite2ByteMask(REG_000416, 0x000F, 0x00FF);         // 0x416[7:0]: i2s data out en
        }
        else
        {
            msWrite2ByteMask(REG_000416, 0x0000, 0x00FF);         // 0x416[7:0]: i2s data out en
        }

        msWrite2ByteMask(REG_000406, 0x0000, 0x0020);         // 0x406[5]: disable AV mute control
        msWrite2ByteMask(REG_000406, 0x0000, 0x1000);         // 0x407[4]: disable AV mute control (au)
    }
}
#endif
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_SetDPGAMuteSrc()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_SetDPGAMuteSrc(AUDIO_MUTE_SOURCE_SEL mute_src_type)
{
        MS_BOOL SpkrMute=FALSE;

        switch(mute_src_type)
        {
            case E_DPGA_INIT_MUTE_OFF:
                m_bDPGA_InitAudioMute = FALSE;
                break;

            case E_DPGA_INIT_MUTE_ON:
                m_bDPGA_InitAudioMute = TRUE;
                break;

            case E_DPGA_SOURCE_CHANGE_MUTE_OFF:
                m_bDPGA_SourceChangeAudioMute = FALSE;
                break;

            case E_DPGA_SOURCE_CHANGE_MUTE_ON:
                m_bDPGA_SourceChangeAudioMute = TRUE;
                break;

            case E_DPGA_MENU_MUTE_OFF:
                m_bDPGA_MenuAudioMute = FALSE;
                break;

            case E_DPGA_MENU_MUTE_ON:
                m_bDPGA_MenuAudioMute = TRUE;
                break;

            case E_DPGA_AVSYNC_MUTE_OFF:
                m_bDPGA_AVSyncAudioMute = FALSE;
                break;

            case E_DPGA_AVSYNC_MUTE_ON:
                m_bDPGA_AVSyncAudioMute = TRUE;
                break;

            case E_DPGA_TIMING_CHANGE_MUTE_OFF:
                m_bDPGA_TimingChangeAudioMute = FALSE;
                break;

            case E_DPGA_TIMING_CHANGE_MUTE_ON:
                m_bDPGA_TimingChangeAudioMute = TRUE;
                break;

            case E_DPGA_MODE_CHANGE_MUTE_OFF:
                m_bDPGA_ModeChangeAudioMute = FALSE;
                break;

            case E_DPGA_MODE_CHANGE_MUTE_ON:
                m_bDPGA_ModeChangeAudioMute = TRUE;
                break;

            case E_DPGA_DELAY_CHANGE_MUTE_OFF:
                m_bDPGA_DelayChangeAudioMute = FALSE;
                break;

            case E_DPGA_DELAY_CHANGE_MUTE_ON:
                m_bDPGA_DelayChangeAudioMute = TRUE;
                break;

            case E_DPGA_INTERNAL_1_MUTE_OFF:
                m_bDPGA_Internal1AudioMute = FALSE;
                break;

            case E_DPGA_INTERNAL_1_MUTE_ON:
                m_bDPGA_Internal1AudioMute = TRUE;
                break;

            default:
                  break;
        }

    #if (AUDIO_MUTE_DEBUG)
        MSAUDIO_PRINT("--> audio DPGA mute status: p:%d,  \r\n", m_bDPGA_InitAudioMute);
        MSAUDIO_PRINT("sr:%d,  \r\n",m_bDPGA_SourceChangeAudioMute);
        MSAUDIO_PRINT("me:%d,  \r\n",m_bDPGA_MenuAudioMute);
        MSAUDIO_PRINT("av:%d,  \r\n",m_bDPGA_AVSyncAudioMute);
        MSAUDIO_PRINT("tm:%d,  \r\n",m_bDPGA_TimingChangeAudioMute);
        MSAUDIO_PRINT("mo:%d,  \r\n",m_bDPGA_ModeChangeAudioMute);
        MSAUDIO_PRINT("de:%d,  \r\n",m_bDPGA_DelayChangeAudioMute);
        MSAUDIO_PRINT("i1:%d,  \r\n",m_bDPGA_Internal1AudioMute);
    #endif

        SpkrMute =  m_bDPGA_InitAudioMute|
                    m_bDPGA_SourceChangeAudioMute|
                    m_bDPGA_MenuAudioMute|
                    m_bDPGA_AVSyncAudioMute|
                    m_bDPGA_TimingChangeAudioMute|
                    m_bDPGA_ModeChangeAudioMute|
                    m_bDPGA_DelayChangeAudioMute|
                    m_bDPGA_Internal1AudioMute;


        if(SpkrMute == TRUE)
        {
            msDrvAudioDPGA_SetVolRegMute();
            MSAUDIO_PRINT("--> audio msDrvAudioDPGA_SetVolRegMute: p:%d,  \r\n", 0);
        }
        else
        {
            msDrvAudioDPGA_RecoverVolume();
            MSAUDIO_PRINT("--> audio msDrvAudioDPGA_RecoverVolume: p:%d,  \r\n", 0);
        }

}

//==============================================================================
// Audio DMA Relation
//==============================================================================
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_HDMI_SetChannelType()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_HDMI_SetChannelType(CHANNEL_TYPE type)
{
        // REG_AUDMBOX_CH_TYPE_140342
        // 0x140342= 0: Ch_Stereo,
        //           1: CH_Multi ,
        //           2: CH_Downmix
        //           4: Mute
        switch(type)
        {
            case CH_MUTE:
                    msWrite2ByteMask(REG_AUDMBOX_CH_TYPE_140342, 0x0004, 0x000F);
                    break;

            case CH_STEREO:
                    msWrite2ByteMask(REG_AUDMBOX_CH_TYPE_140342, 0x0000, 0x000F);
                    break;

            case CH_MULTI:
                    msWrite2ByteMask(REG_AUDMBOX_CH_TYPE_140342, 0x0001, 0x000F);
                    break;

            case CH_DOWNMIX:
                    msWrite2ByteMask(REG_AUDMBOX_CH_TYPE_140342, 0x0002, 0x000F);
                    break;

            default:
                    break;
        }
}


//==============================================================================
// Audio System Relation
//==============================================================================
#if 0
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioGetInputSource()
/// @brief \b Function \b Description: Get audio source selection
/// @param <IN>        \b
/// @return <OUT>      \b Audio input source
//------------------------------------------------------------------------------
AUDIO_DECODE_CH_SWITCH_MUX_SEL msDrvAudioGetInputSource(void)
    {
    BYTE u8AudioChSwitch = (msReadByte(REG_140252) >> 4) & 0x70 ;
    if ( u8AudioChSwitch == 0 )
    {
        return AUDIO_MUX_HDMI_DP_DECODER;
    }
    else
    {
        return AUDIO_MUX_ADC_INPUT;
    }
}
#endif

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioSourceSel()
/// @brief \b Function \b Description: Select audio source
/// @param <IN>        \b src: Line-in0, Line-in1, Line-in1, DAC0, DAC1, DAC2, DAC3, HDMI
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioSourceSel( AUDIO_SOURCE_SEL src )
{
    #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("------- Set msDrvAudioSourceSel = %x ------- \r\n",src);
    #endif

    //msDrvAudioClearDramBuffer();


        switch(src)
        {
            case AUDIO_SOURCE_DAC0:
            case AUDIO_SOURCE_DAC1:
            case AUDIO_SOURCE_DAC2:
            case AUDIO_SOURCE_DAC3:
                msWrite2ByteMask(REG_140252, AUDIO_MUX_ADC_INPUT<<4, 0x0070);   //[6] REG_SEL_ADC_IN 0: refer to SEL_CODEC_IN setting 1: ADC IN
                msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);  //inmux switch
                MSAUDIO_PRINT("------- Set Audio Source NONE ------- \r\n");
                break;

            case AUDIO_SOURCE_HDMI:
                msWrite2ByteMask(REG_140252, AUDIO_MUX_HDMI_DP_DECODER<<4,0x0070);
                //msDrvAudio_HDMI_SetINI();
                MSAUDIO_PRINT("------- Set Audio Source AUDIO_SOURCE_HDMI ------- \r\n");

                break;

            case AUDIO_SOURCE_DP:
                msWrite2ByteMask(REG_140252, AUDIO_MUX_HDMI_DP_DECODER<<4,0x0070);
                //msDrvAudio_DP_SetINI();
                MSAUDIO_PRINT("------- Set Audio Source AUDIO_SOURCE_DP ------- \r\n");

                break;

            case AUDIO_SOURCE_NONE:
                //msDrvAudio_PAD_MUTE(TRUE);
                MSAUDIO_PRINT("------- Set Audio Source NONE ------- \r\n");
                break;

            case AUDIO_SOURCE_LINEIN0:
            case AUDIO_SOURCE_LINEIN1:
            case AUDIO_SOURCE_LINEIN2:
                //msDrvAudio_ADC_SetINI();

            default:

                msWrite2ByteMask(REG_140252, AUDIO_MUX_ADC_INPUT<<4, 0x0070);
                msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);
                break;
        }
        audio_pad_init_flag         = 0;

        msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
        msDrvAudio_SetSoftMuteSrcSel(E_SW_TIMING_CHANGE_MUTE_ON);
        //MSAUDIO_PRINT("--<< Audio Source Mute >>---\r\n");
}

#if EnableDACPowerDown //need update
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioPowerDown()
/// @brief \b Function \b Description: Audio DAC Power Down Control
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioPowerDown( void )
{
    msDrvAudioDPGA_SetVolRegMute();
    ForceDelay1ms(50);
    //msDrvAudio_SetEarphoneMute(TRUE);
    ForceDelay1ms(1);
    //disable class-AB
    msWrite2Byte( REG_14020A, 0x0000);
    msWrite2Byte( REG_14020C, 0x0007);
    msWrite2Byte( REG_14020E, 0x00E0);
    msWrite2Byte( REG_140210, 0x0000);
    msWrite2Byte( REG_140212, 0x0000);
    msWrite2Byte( REG_140214, 0x000F);
    msWrite2Byte( REG_140216, 0x0000);
    msWrite2Byte( REG_140220, 0x000E);   // disable lp
    msWrite2Byte( REG_140222, 0x0000);
    ForceDelay1ms(100);
//disable sw2 and channel select
    msWrite2Byte( REG_14020A, 0x0000);
    msWrite2Byte( REG_14020C, 0x0007);
    msWrite2Byte( REG_14020E, 0x0000); //disable sw2 and sel_earin
    msWrite2Byte( REG_140210, 0x0000);
    msWrite2Byte( REG_140212, 0x0000);
    msWrite2Byte( REG_140214, 0x000F);
    msWrite2Byte( REG_140216, 0x0000);
    msWrite2Byte( REG_140220, 0x000E);   // disable lp
    msWrite2Byte( REG_140222, 0x0000);
    ForceDelay1ms(100);
// disable DAC and VREF
    msWrite2Byte( REG_14020A, 0x07FF);  //disable ADC DAC
    msWrite2Byte( REG_14020C, 0x0017);  // disable vref
    msWrite2Byte( REG_14020E, 0x0020);
    msWrite2Byte( REG_140210, 0x0000);
    msWrite2Byte( REG_140212, 0x0000);
    msWrite2Byte( REG_140214, 0x800F);   //disable dac clk
    msWrite2Byte( REG_140216, 0x0000);
    msWrite2Byte( REG_140220, 0x000E);
    msWrite2Byte( REG_140222, 0x0000);
    ForceDelay1ms(2000);
//disable all
    msWrite2Byte( REG_14020A, 0x07FF);  //disable ADC DAC
    msWrite2Byte( REG_14020C, 0x001F);  // disable vref
    msWrite2Byte( REG_14020E, 0x0000);
    msWrite2Byte( REG_140210, 0x0000);
    msWrite2Byte( REG_140212, 0x0000);
    msWrite2Byte( REG_140214, 0x0000);   //disable dac clk
    msWrite2Byte( REG_140216, 0x0008);
    msWrite2Byte( REG_140220, 0x0000);
    msWrite2Byte( REG_140222, 0x0000);
    //ForceDelay1ms(2000);
}


void msAPI_AudioPowerDown(void )
{
    msDrvAudioPowerDown();
}
#endif
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioI2SConfig()
/// @brief \b Function \b Description: Audio I2S configuration
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioI2SConfig(AUDIO_I2S_FORMAT_SEL eI2SFormat, AUDIO_I2S_BCK_SEL eI2SBCK, AUDIO_I2S_MCK_SEL eI2SMCK )
{
    msWrite2ByteMask(REG_14026C,(eI2SFormat<<8)|(eI2SBCK<<4)|eI2SMCK,BIT8|BIT6|BIT5|BIT4|BIT2|BIT1|BIT0);
}


#if ENABLE_MSTV_UART_DEBUG
//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msDrvAudioSineGen()
/// @brief \b Function \b Description: Audio internal sine gen
/// @param <IN>        \b bEnable, u8SinGenFreq(250Hz~12000Hz), u8SinGenGAIN(-90~0db)
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioSineGen(Bool bEnable, WORD u8SinGenFreq, BYTE u8SinGenGAIN) //high nibble for LCH,low nibble for RCH
{
    BYTE u8SinGenFreqSel;
    BYTE u8SinGenGAINSel = (~(u8SinGenGAIN/6))+1;

    if(u8SinGenFreq<=250)
    {
        u8SinGenFreqSel = PCM_250Hz;
    }
    else if(u8SinGenFreq<=500)
    {
        u8SinGenFreqSel = PCM_500HZ;
    }
    else if(u8SinGenFreq<=1000)
    {
        u8SinGenFreqSel = PCM_1KHZ;
    }
    else if(u8SinGenFreq<=1500)
    {
        u8SinGenFreqSel = PCM_1500HZ;
    }
    else if(u8SinGenFreq<=2000)
    {
        u8SinGenFreqSel = PCM_2KHZ;
    }
    else if(u8SinGenFreq<=3000)
    {
        u8SinGenFreqSel = PCM_3KHZ;
    }
    else if(u8SinGenFreq<=4000)
    {
        u8SinGenFreqSel = PCM_4KHZ;
    }
    else if(u8SinGenFreq<=6000)
    {
        u8SinGenFreqSel = PCM_6KHZ;
    }
    else if(u8SinGenFreq<=8000)
    {
        u8SinGenFreqSel = PCM_8KHZ;
    }
    else if(u8SinGenFreq<=12000)
    {
        u8SinGenFreqSel = PCM_12KHZ;
    }
    else
    {
        u8SinGenFreqSel = PCM_16KHZ;
    }

    msWrite2ByteMask(REG_140218,(u8SinGenGAINSel<<4)|(u8SinGenFreqSel),BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);
    msWrite2ByteMask(REG_14021A,(u8SinGenGAINSel<<4)|(u8SinGenFreqSel),BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);
    msDrvAudioSourceSel( AUDIO_SOURCE_HDMI );
    msWriteBit(REG_140219, bEnable, BIT0);
    msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_OFF);
    msDrvAudio_SetSoftMuteSrcSel(E_SW_TIMING_CHANGE_MUTE_OFF);

}
#endif



//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///           AUDIO API FUNCTION
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//******************************************************************************
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioInit()
/// @brief \b Function \b Description: Audio Init
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioInit( void )
{
    //MSAUDIO_PRINT("... Audio Initialize ...\r\n");
    msDrvAudioSetInit();
    msDrvAudioDACInit();
    msDrvAudio_HDMI_SetChannelType(CH_STEREO);
    msDrvAudioSourceSel(AUDIO_SOURCE_LINEIN0);
    msDrvAudio_SetEarphoneMute(TRUE);
    msDrvAudio_SetDPGAMuteSrc(E_DPGA_INIT_MUTE_ON);
    msDrvAudioDdmaDlyInit();
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioPoweronDePop()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioPoweronDePop( void )
{
    #if EnableDACDePop
        msDrvAudioPoweronDePop(AudioDePopSequence);
    #endif
}
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioHandler()
/// @brief \b Function \b Description: Audio handler
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioHandler( void )
{
    //U16  temp;
    msAPI_AudioPoweronDePop();

}


//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioSetMute()
/// @brief \b Function \b Description: Mute by Software from MCU to DMA Reader
/// @param <IN>        \b enable - 1: mute, 0: unmute
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSetMute( AUDIO_MUTE_TYPE mute_type_sel, AUDIO_MUTE_SOURCE_SEL mute_src_type_sel )
{
    #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("------------------------------- msAPI_AudioSetMute ==%x ------- \r\n",mute_src_type_sel);
    #endif

        switch(mute_type_sel)
        {
            case AUDIO_DPGA_MUTE:
                msDrvAudio_SetDPGAMuteSrc(mute_src_type_sel);
                break;

            case AUDIO_SOFTWARE_MUTE:
                msDrvAudio_SetSoftMuteSrcSel(mute_src_type_sel);
                break;

            default:
                break;
        }
}
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioSetDpgaMute()
/// @brief \b Function \b Description: Mute from DPGA hardware block
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSetDpgaMute( AUDIO_DPGA_SEL eDPGA, MS_BOOL enable )
{
    U16 temp_eDPGA;
    temp_eDPGA = eDPGA;

    if (enable)
    {
        msDrvAudioDPGA_SetVolRegMute();
        //msDrvAudioDPGASetVolume(eDPGA, E_AUDIO_LEFT_RIGHT_CH, -113, VOLUME_FRACTION_MINUS_dot0dB);
        #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("...............1 msDrvAudioDPGA_SetVolRegMute==>%x    \r\n",eDPGA);
        #endif
    }
    else
    {
        msDrvAudioDPGA_RecoverVolume();
        #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("................2 msDrvAudioDPGA_RecoverVolume==>%x    \r\n",eDPGA);
        #endif
    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioSetSoftMute()
/// @brief \b Function \b Description: Mute by Software from MCU to DMA Reader
/// @param <IN>        \b enable - 1: mute, 0: unmute
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSetSoftMute( MS_BOOL enable )
{
#if (AUDIO_STATE_DEBUG)
    MSAUDIO_PRINT("------- msAPI_AudioSetSoftMute Enable = %x ------- \r\n",enable);
#endif

    if (enable)
    {
        msDrvAudio_SetSoftMuteSrcSel(E_SW_MAILBOX_MUTE_ON);
    }
    else
    {
        msDrvAudio_SetSoftMuteSrcSel(E_SW_MAILBOX_MUTE_OFF);
    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioSetEarphoneMute()
/// @brief \b Function \b Description: Earphone Mute.
/// @param <IN>        \b enable - 1: mute, 0: unmute
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSetEarphoneMute( MS_BOOL enable )
{
    msDrvAudio_SetEarphoneMute(enable);

    //printData(" %%%%%%%%%%%%%%%%%%% msAPI_AudioSetEarphoneMute===%x",enable);
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msAPI_AudioSetVolume()
/// @brief \b Function \b Description: Set DPGA gain
/// @param <IN>        \b chsel: LR channel selection
///                      volume: volume control(0~ -113.875dB)
/// @return <OUT>      \b
//------------------------------------------------------------------------------
//void msAPI_AudioSetVolume( AUDIO_DPGA_SEL eDPGA, AUDIO_CH_SEL eChSelect, BYTE iVolume_Integer, AUDIO_VOLUME_FRACTION_PART Volume_Fraction )

void msAPI_AudioSetVolume( AUDIO_DPGA_SEL eDPGA, AUDIO_CH_SEL eChSelect, int iVolume_Integer, AUDIO_VOLUME_FRACTION_PART Volume_Fraction )
{
    msDrvAudioDPGASetVolume( eDPGA, eChSelect, iVolume_Integer, Volume_Fraction );
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msAPI_AudioSourceSel()
/// @brief \b Function \b Description: Select audio source
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSourceSel( BYTE InputPort )
{
    AUDIO_SOURCE_SEL eAudioSrc = AUDIO_SOURCE_LINEIN0;
    INPUT_SRC_TYPE  eType_InputSrc;

    MSAUDIO_PRINT(" ---1. Audio InputPort = %x\n ",InputPort);
    //MSAUDIO_PRINT(" ---2. Audio Src = %x ---- \r\n",g_InputPort[InputPort].eIPType);

        //msDrvAudioDPGA_SetVolRegMute();

    if (INPUT_IS_VGA(InputPort))
    {
        eType_InputSrc = T_VGA;
        MSAUDIO_PRINT(" --- InputPort is VGA\n");
    }
    else if(INPUT_IS_DVI_S(InputPort))
    {
        eType_InputSrc = T_DVI_SINGLE;
        MSAUDIO_PRINT(" --- InputPort is DVI_1\n");
    }
    else if(INPUT_IS_DVI_D(InputPort))
    {
        eType_InputSrc = T_DVI_DUAL;
        MSAUDIO_PRINT(" --- InputPort is DVI_2\n");
    }
    else if((INPUT_IS_DVI_S(InputPort) | INPUT_IS_DVI_D(InputPort)))
    {
        eType_InputSrc = T_DVI;
        MSAUDIO_PRINT(" --- InputPort is DVI_3\n");
    }
    else if(INPUT_IS_VD(InputPort))
    {
        eType_InputSrc = T_VD;
        MSAUDIO_PRINT(" --- InputPort is VD\n");
    }
    else if(INPUT_IS_HDMI(InputPort))
    {
        eType_InputSrc = T_HDMI;
        MSAUDIO_PRINT(" --- InputPort is HDMI\n");
    }
    else if(INPUT_IS_DVI(InputPort) | INPUT_IS_HDMI(InputPort))
    {
        eType_InputSrc = T_MHL;
        MSAUDIO_PRINT(" --- InputPort is MHL\n");
    }
    else if(INPUT_IS_DISPLAYPORT(InputPort))
    {
        eType_InputSrc = T_DP;
        MSAUDIO_PRINT(" --- InputPort is DP\n");
    }
    else if(INPUT_IS_HBR2_0(InputPort))
    {
        eType_InputSrc = T_HBR2_0;
        MSAUDIO_PRINT(" --- InputPort is HBR\n");
    }
    else if(INPUT_IS_HBR2_1(InputPort))
    {
        eType_InputSrc = T_HBR2_1;
        MSAUDIO_PRINT(" --- InputPort is HBR\n");
    }
    else if(INPUT_IS_HBR2(InputPort))
    {
        eType_InputSrc = T_HBR2;
        MSAUDIO_PRINT(" --- InputPort is HBR\n");
    }
    else
    {
        eType_InputSrc = T_RESERVED;
        MSAUDIO_PRINT(" --- InputPort is RESERVED\n");
    }

    msWriteByteMask(REG_140337, 0X00, 0x0F); //DONT inverse ch0's SDM
    //MSAUDIO_PRINT("---> eType_InputSrc = %x\r\n",eType_InputSrc);
    switch(eType_InputSrc)
    {
        case T_VGA:
        case T_DVI_SINGLE:
        case T_DVI_DUAL:
        case T_DVI:
        case T_VD:
            eAudioSrc = AUDIO_SOURCE_LINEIN0;
            msWriteByteMask(REG_140337, BIT0, 0x0F); //inverse ch0
            msAPI_combo_IPCreateAudioPath((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);   //disable audio setting of combo part
            //MSAUDIO_PRINT(" --- Audio is Line-IN\n");
            break;

        case T_HDMI:
        case T_MHL:
            eAudioSrc = AUDIO_SOURCE_HDMI;
            msAPI_combo_IPCreateAudioPath((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);
            //MSAUDIO_PRINT(" --- Audio is HDMI\n");
            break;

        case T_DP:
        case T_HBR2_0:
        case T_HBR2_1:
        case T_HBR2:
            eAudioSrc = AUDIO_SOURCE_DP;
            msAPI_combo_IPCreateAudioPath((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);
            //MSAUDIO_PRINT(" --- Audio is DP\n");
            break;

        case T_RESERVED:
        default:
            eAudioSrc = AUDIO_SOURCE_NONE;
            //MSAUDIO_PRINT(" --- Audio is Reserved\n");
            break;
    }

    msDrvAudioSourceSel(eAudioSrc);

#if (ENABLE_CP_R2)
    msDrvAudioClearDramBuffer();
#endif
}




//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msAPI_AudioI2SConfig()
/// @brief \b Function \b Description: Audio I2S configuration
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioI2SConfig(AUDIO_I2S_FORMAT_SEL eI2SFormat, AUDIO_I2S_BCK_SEL eI2SBCK, AUDIO_I2S_MCK_SEL eI2SMCK )
{
    msDrvAudioI2SConfig(eI2SFormat, eI2SBCK, eI2SMCK);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Direct DMA RD/WR Audio Delay
#define AUDIO_WRITEBASE_L               (AudioPCM_ADR>>4)
#define AUDIO_WRITEBASE_H               (AudioPCM_ADR>>16)
#define AUDIO_READBASE_L                (AudioPCM_ADR>>4)
#define AUDIO_READBASE_H                (AudioPCM_ADR>>16)
#define AUDIO_BUFFER_SIZE               ((AudioPCM_LEN-1)>>4)
static WORD g_wAudioTHOverRun;

void msDrvAudioDdmaDlyInit(void)
{
    MSAUDIO_PRINT("Audio Delay Init\r\n");

    /* Reader Init */
    msWrite2ByteMask(DMA2_CTRL_0_L, 0x30C4, 0x71C4);    // default 8-ch
    msWrite2ByteMask(DMA2_CTRL_1_L, (WORD)AUDIO_WRITEBASE_L, 0xFFF);
    msWrite2Byte(DMA2_CTRL_2_L, AUDIO_WRITEBASE_H);
    msWrite2Byte(DMA2_CTRL_3_L, (WORD)AUDIO_BUFFER_SIZE);

    msWriteBit(DMA2_CTRL_1_H, TRUE, _BIT6);
    msWriteBit(DMA2_CTRL_1_H, FALSE, _BIT6);

    /* Write Init */
    msWrite2Byte(DMA2_CTRL_9_H, 0x00);
    msWrite2ByteMask(DMA2_CTRL_9_L, (WORD)AUDIO_READBASE_L, 0xFFF);
    msWrite2Byte(DMA2_CTRL_10_L, AUDIO_READBASE_H);
    msWrite2Byte(DMA2_CTRL_11_L, (WORD)AUDIO_BUFFER_SIZE);

    msWriteBit(DMA2_CTRL_9_H, TRUE, _BIT6);
    msWriteBit(DMA2_CTRL_9_H, FALSE, _BIT6);

#if (ENABLE_CP_R2)
        msWrite2Byte(REG_AUDMBOX_DELAY_VALUE_14034E,0x1000);
        msWriteBit(REG_AUDMBOX_AUDIO_CP_START, TRUE, _BIT0);
#else
    MsOS_AttachInterrupt(E_INT_IRQ_VIVALDI_DMA_INTR1, msDrvAudioDdmaISR);
#endif
}

void msDrvAudioDdmaDlyTime(WORD ucDelay, AUDIO_CHANNEL Channel)
{
    BYTE Channel_Num;

    if (Channel > CHANNEL_8)
    {
        Channel = CHANNEL_8;
        Channel_Num = 8;
        MSAUDIO_PRINT("Maximum Audio channels: 8-channels\r\n");
    }
    else
        Channel_Num =  (Channel +1)*2;  // 2 , 4 , 6, 8

    if( ucDelay > (4800/Channel_Num ) )
    {
        ucDelay = 4800/Channel_Num;
        MSAUDIO_PRINT("Maximum Audio Delay %d ms for %d-channels audio\r\n",4800/Channel_Num, Channel_Num );
    }
    else if( ucDelay < 10 )
    {
        ucDelay = 10;
        MSAUDIO_PRINT("Miminum Audio Delay 10 ms\r\n");
    }
    msWrite2ByteMask(DMA2_CTRL_0_L, (Channel<<12) | (Channel<<6) , 0x71C0); // [14:12] DMA2_WR mode, [8:6] DMA2_RD mode

    MSAUDIO_PRINT("Audio Delay: %d ms for %d-channels audio\r\n", ucDelay, Channel_Num );

    g_wAudioTHOverRun = (WORD)((DWORD)12 * Channel_Num * ucDelay);      // wRate * ucChannel * ucDelay * 4 /16
// 12*2*2400 = 57600 = 0xE100
// 12*4*1200 = 57600 = 0xE100
// 12*6* 800 = 57600 = 0xE100
// 12*8* 600 = 57600 = 0xE100
    msWrite2Byte(DMA2_CTRL_13_L, g_wAudioTHOverRun);       // DMA2_RD_OVERRUN_TH
    msWrite2Byte(DMA2_CTRL_6_L, g_wAudioTHOverRun/2);      // DMA2_WR_UNDERRUN_TH

#if (ENABLE_CP_R2)
        msWrite2Byte(REG_AUDMBOX_DELAY_VALUE_14034E, g_wAudioTHOverRun/2);
#endif
}

void msAPI_AudioDdmaDlyTime(WORD ucDelay, AUDIO_CHANNEL Channel)
{
    msDrvAudioDdmaDlyTime(ucDelay, Channel);
}

void msDrvAudioDdmaDlyEnable(BYTE ucEnable)
{
#if (ENABLE_CP_R2)
    if( ucEnable )
    {
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, TRUE, _BIT7);
        MSAUDIO_PRINT("Audio Delay Enable\r\n");
    }
    else
    {
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, FALSE, _BIT7);
        MSAUDIO_PRINT("Audio Delay Disable\r\n");
    }
#else
    if( ucEnable )
    {
        msWriteBit(REG_140202, FALSE, _BIT4);

        msWriteBit(DMA2_CTRL_1_H, TRUE, _BIT6);             // DMA2_RD init
        msWriteBit(DMA2_CTRL_1_H, FALSE, _BIT6);

        msWriteBit(DMA2_CTRL_9_H, TRUE, _BIT6);             // DMA2_WR init
        msWriteBit(DMA2_CTRL_9_H, FALSE, _BIT6);

        msWriteBit(DMA2_CTRL_0_L, TRUE, _BIT1);             // DMA2 enable
        //msWriteBit(DMA2_CTRL_1_H, TRUE, _BIT7);             // DMA2_RD enable

        msWrite2Byte(DMA2_CTRL_4_L, g_wAudioTHOverRun);   // MCU_WR_SIZE
        msWriteBit(DMA2_CTRL_9_H, TRUE, _BIT7);             // DMA2_WR enable
        while( !(msReadByte(DMA2_CTRL_8_L)&_BIT1) );        // DMA2_RD_OVERRUN
        msWriteBit(DMA2_CTRL_1_H, TRUE, _BIT5);             // MCU_WR_TRIG
        msWriteBit(DMA2_CTRL_1_H, FALSE, _BIT5);

        msWriteBit(DMA2_CTRL_1_H, TRUE, _BIT7);             // DMA2_RD enable

        msWrite2Byte(DMA2_CTRL_4_L, g_wAudioTHOverRun/2);
        msWriteBit(REG_1402FC, TRUE, _BIT6);

        MsOS_EnableInterrupt(E_INT_IRQ_VIVALDI_DMA_INTR1);

        MSAUDIO_PRINT("Audio Delay Enable\r\n");
    }
    else
    {
        MsOS_DisableInterrupt(E_INT_IRQ_VIVALDI_DMA_INTR1);

        msWriteBit(REG_1402FC, FALSE, _BIT6);

        msWriteBit(DMA2_CTRL_0_L, FALSE, _BIT1);
        msWriteBit(DMA2_CTRL_1_H, FALSE, _BIT7);
        msWriteBit(DMA2_CTRL_9_H, FALSE, _BIT7);

        msWriteBit(REG_140202, TRUE, _BIT4);

        MSAUDIO_PRINT("Audio Delay Disable\r\n");
    }
#endif
}

void msAPI_AudioDdmaDlyEnable(BYTE ucEnable)
{
    msDrvAudioDdmaDlyEnable(ucEnable);
}

void msDrvAudioDdmaISR(InterruptNum eIntNum)
{
    BYTE ucStatus;
    BYTE ucReg;

    eIntNum = eIntNum;

    ucStatus = msReadByte(REG_1402FE);

    if( ucStatus & _BIT6 )
    {
        ucReg = msReadByte(DMA2_CTRL_1_H);
        msWriteByte(DMA2_CTRL_1_H, ucReg|_BIT5);
        msWriteByte(DMA2_CTRL_1_H, ucReg);
    }

    MsOS_EnableInterrupt(E_INT_IRQ_VIVALDI_DMA_INTR1);
}

#if (ENABLE_CP_R2)
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_ClearAudioDramBuffer()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioClearDramBuffer(void)
{
    MDrv_BDMA_PatternFill(MIU_AUDIO_BUFFER_BASE_ADDR, AudioPCM_LEN, 0x00, E_BDMA_DSTDEV_MIU0);
}

void msDrvAudioEQGain1(BYTE Gain1, BYTE Gain2, BYTE Gain3)
{
    if(Gain1 >= 0x10 ) // minus gain
    {
        if (Gain1 < 0xF4) // -12 dB
        {
            Gain1 = 0xF4;
            MSAUDIO_PRINT("Audio EQ 120HZ Minimum Gain = -12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 120HZ Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    else
    {
        if (Gain1 > 0x0C) // +12 dB
        {
            Gain1 = 0x0C;
            MSAUDIO_PRINT("Audio EQ 120HZ Maximium Gain = +12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 120HZ Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    msWrite2Byte(REG_AUDMBOX_EQ_Gain1_140350, Gain1<<8);   // 120Hz

    if(Gain1 >= 0x10 ) // minus gain
    {
        MSAUDIO_PRINT("Audio EQ_120Hz Gain = -%d dB\r\n",0x100 - Gain1 );
    }
    else
    {
        MSAUDIO_PRINT("Audio EQ_120Hz Gain = %d dB\r\n",Gain1 );
    }

    if(Gain2 >= 0x10 ) // minus gain
    {
        if (Gain2 < 0xF4) // -12 dB
        {
            Gain2 = 0xF4;
            MSAUDIO_PRINT("Audio EQ 500Hz Minimum Gain = -12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 500Hz Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    else
    {
        if (Gain2 > 0x0C) // +12 dB
        {
            Gain2 = 0x0C;
            MSAUDIO_PRINT("Audio EQ 500Hz Maximium Gain = +12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 500Hz Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    msWrite2Byte(REG_AUDMBOX_EQ_Gain2_140352, Gain2<<8);   // 500Hz

    if(Gain2 >= 0x10 ) // minus gain
        MSAUDIO_PRINT("Audio EQ_500Hz Gain = -%d dB\r\n",0x100 - Gain2 );
    else
    {
        MSAUDIO_PRINT("Audio EQ_500Hz Gain = %d dB\r\n",Gain2 );
    }

    if(Gain3 >= 0x10 ) // minus gain
    {
        if (Gain3 < 0xF4) // -12 dB
        {
            Gain3 = 0xF4;
            MSAUDIO_PRINT("Audio EQ 1.5K Hz Minimum Gain = -12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 1.5K Hz Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    else
    {
        if (Gain3 > 0x0C) // +12 dB
        {
            Gain3 = 0x0C;
            MSAUDIO_PRINT("Audio EQ 1.5K Hz Maximium Gain = +12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 1.5K Hz Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    msWrite2Byte(REG_AUDMBOX_EQ_Gain3_140354, Gain3<<8);   // 1.5KHz

    if(Gain3 >= 0x10 ) // minus gain
        MSAUDIO_PRINT("Audio EQ_1.5KHz Gain = -%d dB\r\n",0x100 - Gain3 );
    else
    {
        MSAUDIO_PRINT("Audio EQ_1.5KHz Gain = %d dB\r\n",Gain3 );
    }
}

void msAPI_AudioEQGain1(BYTE Gain1, BYTE Gain2, BYTE Gain3)
{
    msDrvAudioEQGain1(Gain1,  Gain2, Gain3);
}

void msDrvAudioEQGain2(BYTE Gain4, BYTE Gain5)
{
    if(Gain4 >= 0x10 ) // minus gain
    {
        if (Gain4 < 0xF4) // -12 dB
        {
            Gain4 = 0xF4;
            MSAUDIO_PRINT("Audio EQ 5K Hz Minimum Gain = -12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 5K Hz Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    else
    {
        if (Gain4 > 0x0C) // +12 dB
        {
            Gain4 = 0x0C;
            MSAUDIO_PRINT("Audio EQ 5K Hz Maximium Gain = +12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 5K Hz Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    msWrite2Byte(REG_AUDMBOX_EQ_Gain4_140356, Gain4<<8);   // 5KHz

    if(Gain4 >= 0x10 ) // minus gain
        MSAUDIO_PRINT("Audio EQ_5KHz = -%d dB\r\n",0x100 - Gain4 );
    else
    {
        MSAUDIO_PRINT("Audio EQ_5KHz Gain = %d dB\r\n",Gain4 );
    }

    if(Gain5 >= 0x10 ) // minus gain
    {
        if (Gain5 < 0xF4) // -12 dB
        {
            Gain5 = 0xF4;
            MSAUDIO_PRINT("Audio EQ 10K Hz Minimum Gain = -12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 10K Hz Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    else
    {
        if (Gain5 > 0x0C) // +12 dB
        {
            Gain5 = 0x0C;
            MSAUDIO_PRINT("Audio EQ 10K Hz Maximium Gain = +12 dB\r\n");
            MSAUDIO_PRINT("Audio EQ 10K Hz Gain range: Gain <= 0x0C; Gain >= 0xF400 \r\n");
        }
    }
    msWrite2Byte(REG_AUDMBOX_EQ_Gain5_140358, Gain5<<8);   // 10KHz

    if(Gain5 >= 0x10 ) // minus gain
        MSAUDIO_PRINT("Audio EQ_10KHz = -%d dB\r\n",0x100 - Gain5 );
    else
    {
        MSAUDIO_PRINT("Audio EQ_10KHz Gain = %d dB\r\n",Gain5 );
    }
}

void msAPI_AudioEQGain2(BYTE Gain4, BYTE Gain5)
{
    msDrvAudioEQGain2(Gain4,  Gain5);
}

void msDrvAudioEQEnable(BYTE ucEnable)
{
    if( ucEnable )
    {
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, FALSE, _BIT5);
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, FALSE, _BIT4);
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, TRUE, _BIT6);
        MSAUDIO_PRINT("Audio EQ Enable\r\n");
    }
    else
    {
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, FALSE, _BIT6);
        MSAUDIO_PRINT("Audio EQ Disable\r\n");
    }
}

void msAPI_AudioEQEnable(BYTE ucEnable)
{
    msDrvAudioEQEnable(ucEnable);
}

void msDrvAudioBassEnable(BYTE ucEnable, BYTE Gain)
{
    if( ucEnable )
    {
        if (Gain > 0x1F)
        {
            Gain = 0x08;
            MSAUDIO_PRINT("Audio Bass Maximum gain = +15 dB\r\n");
            MSAUDIO_PRINT("Audio Bass Minimum gain = -16 dB\r\n");
            MSAUDIO_PRINT("Audio Bass Gain range: 0x00 ~ 0x1F\r\n");
        }

        msWrite2Byte(REG_AUDMBOX_BASS_Gain_14035A, Gain);

        if(Gain >= 0x10 ) // minus gain
            MSAUDIO_PRINT("Audio Bass Gain = -%d dB\r\n",0x20 - Gain );
        else
        {
            MSAUDIO_PRINT("Audio Bass Gain = %d dB\r\n",Gain );
        }

        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, FALSE, _BIT6);
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, TRUE, _BIT5);

        MSAUDIO_PRINT("Audio Bass Enable\r\n");
    }
    else
    {
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, FALSE, _BIT5);
        MSAUDIO_PRINT("Audio Bass Disable\r\n");
    }
}

void msAPI_AudioBassEnable(BYTE ucEnable, BYTE Gain)
{
    msDrvAudioBassEnable(ucEnable, Gain);
}

void msDrvAudioTrbleEnable(BYTE ucEnable, BYTE Gain)
{
    if( ucEnable )
    {
        if (Gain > 0x1F)
        {
            Gain = 0x08;
            MSAUDIO_PRINT("Audio Treble Maximum gain = +15 dB\r\n");
            MSAUDIO_PRINT("Audio Treble Minimum gain = -16 dB\r\n");
            MSAUDIO_PRINT("Audio Treble Gain range: 0x00 ~ 0x1F\r\n");
        }

        msWrite2Byte(REG_AUDMBOX_TREBLE_Gain_14035C, Gain);

        if(Gain >= 0x10 ) // minus gain
            MSAUDIO_PRINT("Audio Treble Gain = -%d dB\r\n",0x20 - Gain );
        else
        {
            MSAUDIO_PRINT("Audio Treble Gain = %d dB\r\n",Gain );
        }
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, FALSE, _BIT6);
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, TRUE, _BIT4);

        MSAUDIO_PRINT("Audio Trble Enable\r\n");

    }
    else
    {
        msWriteBit(REG_AUDMBOX_SW_MUTE_140340, FALSE, _BIT4);
        MSAUDIO_PRINT("Audio TrbleDisable\r\n");
    }
}

void msAPI_AudioTrbleEnable(BYTE ucEnable, BYTE Gain)
{
    msDrvAudioTrbleEnable(ucEnable, Gain);
}
#endif

#endif  /* #define AUDIO_DELAY */

#endif

