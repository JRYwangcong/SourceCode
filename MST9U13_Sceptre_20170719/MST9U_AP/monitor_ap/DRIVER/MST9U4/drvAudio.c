
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
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define AUDIO_VOLUME_ARRAY_NUMBER           115

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
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

static U16                              dpga8Ch0_volume_lt_store       = 0;
static U16                              dpga8Ch0_volume_rt_store       = 0;
static U16                              dpga8Ch1_volume_lt_store       = 0;
static U16                              dpga8Ch1_volume_rt_store       = 0;
static U16                              dpga8Ch2_volume_lt_store       = 0;
static U16                              dpga8Ch2_volume_rt_store       = 0;
static U16                              dpga8Ch3_volume_lt_store       = 0;
static U16                              dpga8Ch3_volume_rt_store       = 0;

static U16                              dpga2Ch0_volume_lt_store       = 0;
static U16                              dpga2Ch0_volume_rt_store       = 0;


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
/// @brief \b Function \b Name:  msDrvAudio_SRC_SetByPassEn(enable)
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_SRC_SetByPassEn(MS_BOOL enable)
{
    //Audio SRC Byass enable/disable
    if (enable)
    {
        msWrite2ByteMask(REG_112C04, BIT15, BIT15);
        #if (AUDIO_STATE_DEBUG)
            MSAUDIO_PRINT("---------- AUDIO SRC ByPass Enable --------- \r\n");
        #endif
    }
    else
    {
        msWrite2ByteMask(REG_112C04, 0x0000, BIT15);
        #if (AUDIO_STATE_DEBUG)
            MSAUDIO_PRINT("---------- AUDIO SRC ByPass Disable --------- \r\n");
        #endif
    }
}

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
        msWrite2ByteMask(REG_112D00, BIT15, BIT15);
        #if (AUDIO_STATE_DEBUG)
            MSAUDIO_PRINT("---------- AUDIO MIU ByPass Enable --------- \r\n");
        #endif
    }
    else
    {
        msWrite2ByteMask(REG_112D00, 0x0000, BIT15);
        #if (AUDIO_STATE_DEBUG)
            MSAUDIO_PRINT("---------- AUDIO MIU ByPass Disable --------- \r\n");
        #endif
    }
}

/******************************************************************************/
/// @brief \b Function \b Name:  msDrvAudio_DPGA8Ch_SetByPassEn(enable)
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_DPGA8Ch_SetByPassEn(MS_BOOL enable)
{
    //Audio SRC Byass enable/disable
    if (enable)
    {
        msWrite2ByteMask(REG_112D22, BIT15, BIT15);
        #if (AUDIO_STATE_DEBUG)
            MSAUDIO_PRINT("---------- AUDIO SRC ByPass Enable --------- \r\n");
        #endif
    }
    else
    {
        msWrite2ByteMask(REG_112D22, 0x0000, BIT15);
        #if (AUDIO_STATE_DEBUG)
            MSAUDIO_PRINT("---------- AUDIO SRC ByPass Disable --------- \r\n");
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
    temp = msRead2Byte(REG_112D00) & BIT15;                                                     // 0x112D00[15] = 1, MIU bypass
    if (temp == BIT15)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}
/******************************************************************************/
/// @brief \b Function \b Name:  msDrvAudio_HDMIRx_GetNPCM_Info()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
MS_BOOL msDrvAudio_HDMIRx_GetNPCM_Info(void)
{
    MS_U16  temp;
    temp = msRead2Byte(REG_112CE4) & BIT15;
    if (temp == BIT15)
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
    //------------------------------------------------------------------------------
    //synthesizer clock enable
    msWrite2ByteMask(REG_112D70, BIT0, BIT0);      //synthesizer clock enable
    msWrite2ByteMask(REG_112D6C, 0x00, BIT4);      //synthesizer reference clock selection ,0:432Mhz(default),1:216Mhz

    //synthesizer setting
    msWrite2ByteMask(REG_112D6A, 0x1C20, 0xFFFF);    //N.F. value 122.88Mhz
    msWrite2ByteMask(REG_112D68, BIT15, BIT15|0xFF); //synthesizer enable

    msWrite2ByteMask(REG_112D68, BIT14, BIT14);      //synthesizer N.f. Value update
    ForceDelay1ms(1);
    msWrite2ByteMask(REG_112D68, 0x00, BIT14);

    //AUPLL setting
    msWrite2ByteMask(REG_112DEA, 0x00, BIT11);      //AUPLL power down
    //------------------------------------------------------------------------------
}


void msDrvAudioDigitalHardwareInit( void )
{
    MSAUDIO_PRINT("----- Audio DHW Init  ----- \r\n");
    //I2S Rx
    //Clock enable
    msWrite2ByteMask(REG_112C08, BIT15, BIT15);     //I2S decoder clock enable
    msWrite2ByteMask(REG_112C08, BIT14, BIT14);     //Synthesizer source clock enable
    msWrite2ByteMask(REG_112C08, BIT13, BIT13);     //Master Mode clock enable
    //Format
    msWrite2ByteMask(REG_112C7A, 0x00, BIT1);       //I2S format 0:I2S ,1:left-justify format
    //Master mode
    //Sampling rate 48KHz
    msWrite2ByteMask(REG_112C78, BIT11, BIT11);      //Master Mode enable
    msWrite2ByteMask(REG_112C78, E_I2SRx_BITWIDTH_16<<12, BIT13|BIT12);//Master Mode bit width
    msWrite2ByteMask(REG_112C78, 0x4F, 0x3FF);      //Master Mode BCK divider ?????????   PS: wait RD provide formula to set
    //------------------------------------------------------------------------------
    //SPDIF Rx
    //Clock enable
    msWrite2ByteMask(REG_112C08, BIT12, BIT12);      //SPDIF decoder clock enable
    msWrite2ByteMask(REG_112C08, BIT11, BIT11);      //Synthesizer source clock enable
    msWrite2ByteMask(REG_112C88, BIT6, BIT6);        //CDR enable
    //------------------------------------------------------------------------------
    //HDMI Rx
    //Matrix configuration
    msWrite2ByteMask(REG_112B20, 0x0101, 0xFFFF);        //[7:0]channel matrix
    msWrite2ByteMask(REG_112B20, 0x0202, 0xFFFF);        //[7:0]channel matrix
    msWrite2ByteMask(REG_112B20, 0x0303, 0xFFFF);        //[7:0]channel matrix
    msWrite2ByteMask(REG_112B20, 0x0404, 0xFFFF);        //[7:0]channel matrix
    msWrite2ByteMask(REG_112B20, 0x0505, 0xFFFF);        //[7:0]channel matrix
    msWrite2ByteMask(REG_112B20, 0x0606, 0xFFFF);        //[7:0]channel matrix
    msWrite2ByteMask(REG_112B20, 0x0707, 0xFFFF);        //[7:0]channel matrix
    //NPCM Detector
    msWrite2ByteMask(REG_112CE2, BIT9, BIT9);        //Multi-channel enable
    //------------------------------------------------------------------------------
    //Sine-Gen
    //Clock Enable
     msWrite2ByteMask(REG_112C08, BIT10, BIT10);     //clock enable
    //------------------------------------------------------------------------------
    //8bit SRC
     msWrite2ByteMask(REG_112C0C, BIT15, BIT15);     //interpolation clock
     msWrite2ByteMask(REG_112C0C, BIT14, BIT14);     //MAC clock
     msWrite2ByteMask(REG_112C0C, BIT13, BIT13);     //decimation clock
     msWrite2ByteMask(REG_112C0C, BIT12, BIT12);     //Group A (48KHz)
    //CIC Enable
     msWrite2ByteMask(REG_112C50, 0xFF, 0xFF);      //Gain Compensation, enable filter
     msWrite2ByteMask(REG_112C4C, BIT11, BIT11);     //enable gain compensation
    //------------------------------------------------------------------------------
    //DownMix control Register
    msWrite2ByteMask(REG_112D58, 0x4000, 0xFFFF);     //Coef1 +1
    msWrite2ByteMask(REG_112D5A, 0x4000, 0xFFFF);     //Coef2 +1
    msWrite2ByteMask(REG_112D5C, 0x2d3f, 0xFFFF);     //Coef3 +0.707
    msWrite2ByteMask(REG_112D5E, 0x2d3f, 0xFFFF);     //Coef4 +0.707
    msWrite2ByteMask(REG_112D60, 0xd2C1, 0xFFFF);     //Coef5 -0.707
    msWrite2ByteMask(REG_112D62, 0x2d3f, 0xFFFF);     //Coef6 +0.707
    msWrite2ByteMask(REG_112D60, 0xd2C1, 0xFFFF);     //Coef7 -0.707
    msWrite2ByteMask(REG_112D62, 0x2d3f, 0xFFFF);     //Coef8 +0.707
    //------------------------------------------------------------------------------
    //GEQ
    //Sampling rate = 48KHz
    msWrite2ByteMask(REG_112B00, BIT0, BIT0);         //GEQ enable
    msWrite2ByteMask(REG_112B00, BIT1, BIT1);       //1:7 band, 0:5 Band
    msWrite2ByteMask(REG_112B00, 0x00, BIT2);     //Gain format 1:4.20,0:3.21
    //------------------------------------------------------------------------------
    //I2S Tx
    //I2S clock enable
    msWrite2ByteMask(REG_112C0C, BIT2, BIT2);     //I2S BCK clock enable
    msWrite2ByteMask(REG_112C82, BIT3, BIT3);     //I2S MCK clock enable
    //I2S Format
    msWrite2ByteMask(REG_112C80, 0x00, BIT15);     // 0 : I2S mode / 1 : Left-justified mode
    msWrite2ByteMask(REG_112C80, E_I2STx_BITWIDTH_16<<13, BIT14|BIT13); //I2S 16/24/32 bit mode setting
    msWrite2ByteMask(REG_112C82, E_I2STx_MCLK_128FS, BIT2|BIT1|BIT0);     //I2S MCK selection
    //------------------------------------------------------------------------------
    //SPDIF Tx
    //SPDIF clock enable
    msWrite2ByteMask(REG_112C0C, BIT0, BIT0);     //SPDIF output data out clock enable
    msWrite2ByteMask(REG_112C0C, BIT1, BIT1);     //SPDIF output data in clock enable
    //SPDIF initial sequence
    msWrite2ByteMask(REG_112C98, BIT1, BIT1);     //CNT reset
    msWrite2ByteMask(REG_112C98, 0x00, BIT1);     //CNT reset
    msWrite2ByteMask(REG_112C98, BIT0, BIT0);     //status clear
    msWrite2ByteMask(0x112C98, 0x00, BIT0);     //status clear
    //output SPDIF format
    msWrite2ByteMask(REG_112C98, 0x00|BIT5, BIT6|BIT5);     //0:PCM ,1:NPCM [5]channel status enable
    msWrite2ByteMask(REG_112C98, 0x00, BIT10);     //keep MSB 16bit data, dont truncate LSB 8 bit data
    msWrite2ByteMask(REG_112CB8, BIT1, BIT1);     //update channel status before frame start (default)
    msWrite2ByteMask(REG_112CB8, 0x00, BIT1);     //update channel status directly
    msWrite2ByteMask(REG_112CB8, BIT0, BIT0);     //channel status valid
    msWrite2ByteMask(REG_112CB8, 0x00, BIT0);     //write channel status register
    //------------------------------------------------------------------------------
    //SPDIF Tx2(for HDMI Rx ARC,Audio Return channel)
    //SPDIF clock enable
    msWrite2ByteMask(0x112C08, BIT1, BIT1);     //SPDIF output data out clock enable
    msWrite2ByteMask(0x112C08, BIT2, BIT2);     //SPDIF output data in clock enable
    //SPDIF initial sequence
    msWrite2ByteMask(0x112C9A, BIT1, BIT1);     //CNT reset
    msWrite2ByteMask(0x112C9A, 0x00, BIT0);     //CNT reset
    msWrite2ByteMask(0x112C9A, BIT0, BIT0);     //status clear
    msWrite2ByteMask(0x112C9A, 0x00, BIT0);     //status clear
    //output SPDIF format
    msWrite2ByteMask(0x112C9A, 0x00|BIT5, BIT6|BIT5);     //0:PCM ,1:NPCM [5]channel status enable
    msWrite2ByteMask(0x112C9A, 0x00, BIT10);     //keep MSB 16bit data, dont truncate LSB 8 bit data
    msWrite2ByteMask(REG_112CB8, BIT5, BIT5);     //update channel status before frame start (default)
    msWrite2ByteMask(REG_112CB8, 0x00, BIT5);     //update channel status directly
    msWrite2ByteMask(REG_112CB8, BIT6, BIT6);     //channel status valid
    msWrite2ByteMask(REG_112CB8, 0x00, BIT6);     //write channel status register
    //------------------------------------------------------------------------------
    //DPGA8CH
    //gain
    msWrite2ByteMask(REG_112D20, 0x038F, 0x7FF);    //DPGA ch1 gain
    msWrite2ByteMask(REG_112D22, 0x038F, 0x7FF);    //DPGA ch2 gain
    msWrite2ByteMask(REG_112D24, 0x038F, 0x7FF);    //DPGA ch3 gain
    msWrite2ByteMask(REG_112D26, 0x038F, 0x7FF);    //DPGA ch4 gain
    msWrite2ByteMask(REG_112D28, 0x038F, 0x7FF);    //DPGA ch5 gain
    msWrite2ByteMask(REG_112D2A, 0x038F, 0x7FF);    //DPGA ch6 gain
    msWrite2ByteMask(REG_112D2C, 0x038F, 0x7FF);    //DPGA ch7 gain
    msWrite2ByteMask(REG_112D2E, 0x038F, 0x7FF);    //DPGA ch8 gain
    msWrite2ByteMask(REG_112D20, BIT15  ,BIT15);    //Ch1/2 gain update
    msWrite2ByteMask(REG_112D24, BIT15  ,BIT15);    //Ch1/2 gain update
    msWrite2ByteMask(REG_112D28, BIT15  ,BIT15);    //Ch1/2 gain update
    msWrite2ByteMask(REG_112D2C, BIT15  ,BIT15);    //Ch1/2 gain update
    ForceDelay1ms(1);
    msWrite2ByteMask(REG_112D20, 0x00  ,BIT15);    //Ch1/2 gain update
    msWrite2ByteMask(REG_112D24, 0x00  ,BIT15);    //Ch1/2 gain update
    msWrite2ByteMask(REG_112D28, 0x00  ,BIT15);    //Ch1/2 gain update
    msWrite2ByteMask(REG_112D2C, 0x00  ,BIT15);    //Ch1/2 gain update
    //step
    msWrite2ByteMask(REG_112D20, DPGA8CH_STEP<<12, BIT14|BIT13|BIT12);    //ch1/2 step
    msWrite2ByteMask(REG_112D24, DPGA8CH_STEP<<12, BIT14|BIT13|BIT12);    //ch3/4 step
    msWrite2ByteMask(REG_112D28, DPGA8CH_STEP<<12, BIT14|BIT13|BIT12);    //ch5/6 step
    msWrite2ByteMask(REG_112D2C, DPGA8CH_STEP<<12, BIT14|BIT13|BIT12);    //ch7/7 step
    //fading
    msWrite2ByteMask(REG_112D22, BIT14|BIT13, BIT14|BIT13);    //ch1/2 fading ,zerocrossing (default on)
    msWrite2ByteMask(REG_112D26, BIT14|BIT13, BIT14|BIT13);    //ch3/4 fading ,zerocrossing
    msWrite2ByteMask(REG_112D2A, BIT14|BIT13, BIT14|BIT13);    //ch5/6 fading ,zerocrossing
    msWrite2ByteMask(REG_112D2E, BIT14|BIT13, BIT14|BIT13);    //ch7/8 fading ,zerocrossing
    //zero-crossing
    msWrite2ByteMask(REG_112D32, ENABLE<<14, BIT14);    //ch4/3/2/1  timeout
    msWrite2ByteMask(REG_112D34, ENABLE<<14, BIT14);    //ch8/7/6/5  timeout
    //Gain boundle
    //msWrite2ByteMask(REG_112D36, ENABLE<<14, BIT14);    //enable
    //msWrite2ByteMask(REG_112D36, 0x038F, 0x7FF);        //boundle gain
    //msWrite2ByteMask(REG_112D36, BIT15  ,BIT15);        //boundle gain update
    //ForceDelay1ms(1);
    //msWrite2ByteMask(REG_112D36, 0x00  ,BIT15);         //boundle gain update
    //------------------------------------------------------------------------------
    #if ENABLE_SPDIF
    //DPGA2CH
    msWrite2ByteMask(REG_112CA0, 0x038F, 0x7FF);    //DPGA L gain
    msWrite2ByteMask(REG_112CA2, 0x038F, 0x7FF);    //DPGA R gain
    msWrite2ByteMask(REG_112CA0, BIT15, BIT15);    //update DPGA gain
    msWrite2ByteMask(REG_112CA0, 0x00, BIT15);     //update DPGA gain
    #endif
    //------------------------------------------------------------------------------
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
    msDrvAudio_PLL_SetINI();

    msDrvAudioDigitalHardwareInit();

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
    BYTE u8Offset = eDPGA*4;
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
                dpga8Ch0_volume_lt_store = reg16Volume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga8Ch0_volume_rt_store = reg16Volume;
            }
            else
            {
                dpga8Ch0_volume_lt_store = reg16Volume;
                dpga8Ch0_volume_rt_store = reg16Volume;
            }
            break;

        case AUDIO_DPGA1:
            if (eChSelect == E_AUDIO_LEFT_CH)
            {
                dpga8Ch1_volume_lt_store = reg16Volume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga8Ch1_volume_rt_store = reg16Volume;
            }
            else
            {
                dpga8Ch1_volume_lt_store = reg16Volume;
                dpga8Ch1_volume_rt_store = reg16Volume;
            }
           break;

        case AUDIO_DPGA2:
            if (eChSelect == E_AUDIO_LEFT_CH)
            {
                dpga8Ch2_volume_lt_store = reg16Volume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga8Ch2_volume_rt_store = reg16Volume;
            }
            else
            {
                dpga8Ch2_volume_lt_store = reg16Volume;
                dpga8Ch2_volume_rt_store = reg16Volume;
            }
           break;

        case AUDIO_DPGA3:
            if (eChSelect == E_AUDIO_LEFT_CH)
            {
                dpga8Ch3_volume_lt_store = reg16Volume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga8Ch3_volume_rt_store = reg16Volume;
            }
            else
            {
                dpga8Ch3_volume_lt_store = reg16Volume;
                dpga8Ch3_volume_rt_store = reg16Volume;
            }
            break;

        case AUDIO_DPGA_ALL:
        default:
                dpga8Ch0_volume_lt_store = reg16Volume;
                dpga8Ch0_volume_rt_store = reg16Volume;
                dpga8Ch1_volume_lt_store = reg16Volume;
                dpga8Ch1_volume_rt_store = reg16Volume;
                dpga8Ch2_volume_lt_store = reg16Volume;
                dpga8Ch2_volume_rt_store = reg16Volume;
                dpga8Ch3_volume_lt_store = reg16Volume;
                dpga8Ch3_volume_rt_store = reg16Volume;
            break;
    }

    if( eDPGA == AUDIO_DPGA_ALL )
    {
        //need check behavior
        msWrite2ByteMask(REG_112D36, BIT14, BIT14);    //boundle all DPGA to DPGA1 left channel   ,enable it then fading is not useful
        eChSelect = E_AUDIO_LEFT_RIGHT_CH;
        u8Offset = 0;
    }
    else
    {
        msWrite2ByteMask(REG_112D36, 0x00, BIT14);
    }

    switch(eChSelect)
    {
        case E_AUDIO_LEFT_CH:
            msWrite2ByteMask(REG_112D20 + u8Offset, reg16Volume ,0x07FF);
            break;

        case E_AUDIO_RIGHT_CH:
            msWrite2ByteMask(REG_112D22 + u8Offset, reg16Volume , 0x07FF);
            break;

        case E_AUDIO_LEFT_RIGHT_CH:
            if( eDPGA == AUDIO_DPGA_ALL )
            {
                msWrite2ByteMask(REG_112D20, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_112D22, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_112D24, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_112D26, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_112D28, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_112D2A, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_112D2C, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_112D2E, reg16Volume, 0x07FF);
            }
            else
            {
                msWrite2ByteMask(REG_112D20 + u8Offset, reg16Volume, 0x07FF);
                msWrite2ByteMask(REG_112D22 + u8Offset, reg16Volume, 0x07FF);
            }
            break;
        default:
            break;
    }
    if( eDPGA == AUDIO_DPGA_ALL )
    {
        /*
        msWriteByteMask(REG_112D20, 0x00, BIT15);
        msWriteByteMask(REG_112D20, BIT15, BIT15);
        msWriteByteMask(REG_112D20, 0x00, BIT15);
        msWriteByteMask(REG_112D24, 0x00, BIT15);
        msWriteByteMask(REG_112D24, BIT15, BIT15);
        msWriteByteMask(REG_112D24, 0x00, BIT15);
        msWriteByteMask(REG_112D28, 0x00, BIT15);
        msWriteByteMask(REG_112D28, BIT15, BIT15);
        msWriteByteMask(REG_112D28, 0x00, BIT15);
        msWriteByteMask(REG_112D2C, 0x00, BIT15);
        msWriteByteMask(REG_112D2C, BIT15, BIT15);
        msWriteByteMask(REG_112D2C, 0x00, BIT15);
        */
        msWrite2ByteMask(REG_112D36, 0x00, BIT15);
        msWrite2ByteMask(REG_112D36, BIT15, BIT15);
        msWrite2ByteMask(REG_112D36, 0x00, BIT15);
    }
    else
    {
        msWrite2ByteMask(REG_112D20 + u8Offset, 0x00, BIT15);             // BIT0:Trigger hardware to set the left-channel DPGA's gain.
        msWrite2ByteMask(REG_112D20 + u8Offset, BIT15,BIT15);
        msWrite2ByteMask(REG_112D20 + u8Offset, 0x00,BIT15);
    }
}
#if ENABLE_SPDIF
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioDPGA2CHSetVolume()
/// @brief \b Function \b Description: Set Audio DPGA Volume
/// @param <IN>        \b eChSelect: Channel selection
/// @param <IN>        \b Volume_Integer: Volume Integer Part from 0 ~ -113dB
/// @param <IN>        \b Volume_Fraction: Volume Fraction Part, -0.125dB per unit
///                    \b        sel in AUDIO_VOLUME_FRACTION_PART
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioDPGA2CHSetVolume( AUDIO_CH_SEL eChSelect, signed short iVolume_Integer, AUDIO_VOLUME_FRACTION_PART Volume_Fraction )
{
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

    if (eChSelect == E_AUDIO_LEFT_CH)
    {
        dpga2Ch0_volume_lt_store = reg16Volume;
    }
    else if (E_AUDIO_RIGHT_CH)
    {
        dpga2Ch0_volume_rt_store = reg16Volume;
    }
    else
    {
        dpga2Ch0_volume_lt_store = reg16Volume;
        dpga2Ch0_volume_rt_store = reg16Volume;
    }

    switch(eChSelect)
    {
        case E_AUDIO_LEFT_CH:
            msWrite2ByteMask(REG_112CA0 , reg16Volume ,0x07FF);
            break;

        case E_AUDIO_RIGHT_CH:
            msWrite2ByteMask(REG_112CA2 , reg16Volume , 0x07FF);
            break;

        case E_AUDIO_LEFT_RIGHT_CH:
            msWrite2ByteMask(REG_112CA0, reg16Volume, 0x07FF);
            msWrite2ByteMask(REG_112CA2, reg16Volume, 0x07FF);
            break;

        default:
            break;
    }
    msWrite2ByteMask(REG_112CA0 , 0x00, BIT15);             // BIT0:Trigger hardware to set the left-channel DPGA's gain.
    msWrite2ByteMask(REG_112CA0 , BIT15,BIT15);
    msWrite2ByteMask(REG_112CA0 , 0x00,BIT15);
}
#endif
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
    msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0000, BIT15);
    msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, BIT15, BIT15);
    msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0000, BIT15);

    msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_LEFT, 0x0388, 0x07FF);
    msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_RIGHT, 0x0388, 0x07FF);
    msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0000, BIT15);
    msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, BIT15, BIT15);
    msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0000, BIT15);

    msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_LEFT, 0x0388, 0x07FF);
    msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_RIGHT, 0x0388, 0x07FF);
    msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0000, BIT15);
    msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, BIT15, BIT15);
    msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0000, BIT15);

    msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_LEFT, 0x0388, 0x07FF);
    msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_RIGHT, 0x0388, 0x07FF);
    msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0000, BIT15);
    msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, BIT15, BIT15);
    msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0000, BIT15);

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
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_LEFT, dpga8Ch3_volume_lt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_RIGHT, dpga8Ch3_volume_rt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0000, BIT15);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, BIT15, BIT15);
        msWrite2ByteMask(REG_AUDIO_DPGA3_VOLUME_CTRL, 0x0000, BIT15);

        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_LEFT, dpga8Ch2_volume_lt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_RIGHT, dpga8Ch2_volume_rt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0000, BIT15);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, BIT15, BIT15);
        msWrite2ByteMask(REG_AUDIO_DPGA2_VOLUME_CTRL, 0x0000, BIT15);

        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_LEFT, dpga8Ch1_volume_lt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_RIGHT, dpga8Ch1_volume_rt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0000, BIT15);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, BIT15, BIT15);
        msWrite2ByteMask(REG_AUDIO_DPGA1_VOLUME_CTRL, 0x0000, BIT15);

        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_LEFT, dpga8Ch0_volume_lt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_RIGHT, dpga8Ch0_volume_rt_store, 0x07FF);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0000, BIT15);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, BIT15, BIT15);
        msWrite2ByteMask(REG_AUDIO_DPGA0_VOLUME_CTRL, 0x0000, BIT15);
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
    }
}

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
/// @brief \b Function \b Name:  msDrvAudioGetInputSource()
/// @brief \b Function \b Description: Get audio source selection
/// @param <IN>        \b
/// @return <OUT>      \b Audio input source
//------------------------------------------------------------------------------


AUDIO_SOURCE_SEL msDrvAudioGetInputSource(void)
{
    BYTE u8AudioChSwitch = (msReadByte(REG_112C04) >> 4) & 0x70 ;
    if ( u8AudioChSwitch == 0 )
    {
        return AUDIO_SOURCE_HDMI;
    }
    else if ( u8AudioChSwitch == 2 )
    {
        return AUDIO_SOURCE_I2S;
    }
    else if ( u8AudioChSwitch == 3 )
    {
        return AUDIO_SOURCE_SPDIF;
    }
    else
    {
        return AUDIO_SOURCE_NONE;
    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioSourceSel()
/// @brief \b Function \b Description: Select audio source
/// @param <IN>        \b src: HDMI, NONE, I2S, SPDIF
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioSourceSel( AUDIO_SOURCE_SEL src )
{
    #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("------- Set msDrvAudioSourceSel = %x ------- \r\n",src);
    #endif

    switch(src)
    {
        case AUDIO_SOURCE_HDMI:
            msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);  //inmux switch
            MSAUDIO_PRINT("------- Set Audio Source AUDIO_SOURCE_HDMI ------- \r\n");
            break;

        case AUDIO_SOURCE_I2S:
            msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);  //inmux switch
            MSAUDIO_PRINT("------- Set Audio Source AUDIO_SOURCE_I2S------- \r\n");
            break;

        case AUDIO_SOURCE_SPDIF:
            msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);  //inmux switch
            MSAUDIO_PRINT("------- Set Audio Source AUDIO_SOURCE_SPDIF ------- \r\n");
            break;

        case AUDIO_SOURCE_EMPTY:
            MSAUDIO_PRINT("------- Set Audio Source NONE ------- \r\n");
            break;

        default:
            msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);
            break;
    }
    audio_pad_init_flag         = 0;

    msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
    msDrvAudio_SetSoftMuteSrcSel(E_SW_TIMING_CHANGE_MUTE_ON);
    //MSAUDIO_PRINT("--<< Audio Source Mute >>---\r\n");
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioApplicationPathSel()
/// @brief \b Function \b Description: Select audio source
/// @param <IN>        \b src:
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioApplicationPathSel( APPLICATION_PATH_SEL path_src )
{
    #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("------- Set msDrvAudioSourceSel = %x ------- \r\n",src);
    #endif

    switch(path_src)
    {
        case I2SRx_PCM_2CH_TO_2CH_I2STx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case I2SRx_PCM_2CH_TO_2CH_HDMITx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case I2SRx_PCM_2CH_TO_2CH_SPDIFTx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case SPDIFRx_PCM_2CH_TO_2CH_I2STx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case SPDIFRx_PCM_2CH_TO_2CH_HDMITx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case SPDIFRx_PCM_2CH_TO_2CH_SPDIFTx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case SPDIFRx_NPCM_2CH_TO_2CH_HDMITx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case SPDIFRx_NPCM_2CH_TO_2CH_SPDIFTx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_PCM_2CH_TO_2CH_I2STx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_PCM_2CH_TO_2CH_HDMITx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_PCM_2CH_TO_2CH_SPDIFTx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_PCM_8CH_TO_2CH_I2STx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_PCM_8CH_TO_2CH_HDMITx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_PCM_8CH_TO_2CH_SPDIFTx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_PCM_8CH_TO_8CH_I2STx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_PCM_8CH_TO_8CH_HDMITx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_NPCM_2CH_TO_2CH_HDMITx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_NPCM_2CH_TO_2CH_SPDIFTx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case HDMIRx_NPCM_8CH_TO_8CH_HDMITx:
            //m_bSW_InitAudioMute = FALSE;
            break;
        case APPLICATION_PATH_SEL_RESERVED:
            //m_bSW_InitAudioMute = FALSE;
            break;


        default:
              break;
    }

/*
    switch(src)
    {
        case AUDIO_SOURCE_HDMI:
            msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);  //inmux switch
            MSAUDIO_PRINT("------- Set Audio Source AUDIO_SOURCE_HDMI ------- \r\n");
            break;

        case AUDIO_SOURCE_I2S:
            msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);  //inmux switch
            MSAUDIO_PRINT("------- Set Audio Source AUDIO_SOURCE_I2S------- \r\n");
            break;

        case AUDIO_SOURCE_SPDIF:
            msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);  //inmux switch
            MSAUDIO_PRINT("------- Set Audio Source AUDIO_SOURCE_SPDIF ------- \r\n");
            break;

        case AUDIO_SOURCE_EMPTY:
            MSAUDIO_PRINT("------- Set Audio Source NONE ------- \r\n");
            break;

        default:
            msWrite2ByteMask(REG_140210, src<<5, BIT8|BIT7|BIT6|BIT5);
            break;
    }
    audio_pad_init_flag         = 0;

    msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
    msDrvAudio_SetSoftMuteSrcSel(E_SW_TIMING_CHANGE_MUTE_ON);
    //MSAUDIO_PRINT("--<< Audio Source Mute >>---\r\n");
    */
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
/// @brief \b Function \b Name:  msDrvAudioI2STxConfig()
/// @brief \b Function \b Description: Audio I2S configuration
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioI2STxConfig(AUDIO_I2S_FORMAT_SEL eI2SFormat, AUDIO_I2S_BCK_SEL eI2SBCK, AUDIO_I2S_MCK_SEL eI2SMCK )
{
    msWrite2ByteMask(REG_112C80,(eI2SFormat<<15)|(eI2SBCK<<13)|eI2SMCK,BIT15|BIT14|BIT13|BIT2|BIT1|BIT0);
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
    //sine gen path select
    msWrite2ByteMask(REG_112C14,0xFF00,0xFF00);
    //frequency & gain
    msWrite2ByteMask(REG_112C16,(u8SinGenFreqSel<<12)|(u8SinGenFreqSel<<8),0xFF00);
    msWrite2ByteMask(REG_112C16,(u8SinGenGAINSel<<4)|(u8SinGenGAINSel),0x00FF);
    //enable
    msWriteBit(REG_112C14, bEnable, BIT0);
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
    msDrvAudio_HDMI_SetChannelType(CH_STEREO);   //fix???
    msDrvAudioSourceSel(AUDIO_SOURCE_HDMI);
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
    enable=enable;
    //doesnt have earphone output
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
    #if ENABLE_SPDIF
    msDrvAudioDPGA2CHSetVolume( eChSelect, iVolume_Integer, Volume_Fraction );
    #endif
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msAPI_AudioSourceSel()
/// @brief \b Function \b Description: Select audio source
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSourceSel( BYTE InputPort )
{
    AUDIO_SOURCE_SEL eAudioSrc = AUDIO_SOURCE_HDMI;
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

    //MSAUDIO_PRINT("---> eType_InputSrc = %x\r\n",eType_InputSrc);
    switch(eType_InputSrc)
    {
        case T_VGA:
        case T_DVI_SINGLE:
        case T_DVI_DUAL:
        case T_DVI:
        case T_VD:
            eAudioSrc = AUDIO_SOURCE_EMPTY;
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

}




//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msAPI_AudioI2SConfig()
/// @brief \b Function \b Description: Audio I2S configuration
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioI2SConfig(AUDIO_I2S_FORMAT_SEL eI2SFormat, AUDIO_I2S_BCK_SEL eI2SBCK, AUDIO_I2S_MCK_SEL eI2SMCK )
{
    msDrvAudioI2STxConfig(eI2SFormat, eI2SBCK, eI2SMCK);
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

    MsOS_AttachInterrupt(E_INT_IRQ_VIVALDI_DMA_INTR1, msDrvAudioDdmaISR);
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

}

void msAPI_AudioDdmaDlyTime(WORD ucDelay, AUDIO_CHANNEL Channel)
{
    msDrvAudioDdmaDlyTime(ucDelay, Channel);
}

void msDrvAudioDdmaDlyEnable(BYTE ucEnable)
{
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
#endif  /* #define AUDIO_DELAY */

#endif


