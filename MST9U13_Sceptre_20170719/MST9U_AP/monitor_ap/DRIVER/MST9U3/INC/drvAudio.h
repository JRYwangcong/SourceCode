//#define _DRVDAC_H_
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   msDAC.h
/// @author MStar Semiconductor Inc.
/// @brief  Audio DAC Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MSDAC_H_
#define _MSDAC_H_




//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//#define MEM32(x)                                *((volatile unsigned long *)(x))
//#define DQMEM                                   __attribute__ ((section(".qmem")))
//#define AUDIO_DELAY_EN              1

#define ENABLE_I2S_FIX_MCLK         0   // fix MCLK output freq. ~13M

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// Audio DPGA selection
typedef enum
{
    AUDIO_DPGA0                     = 0,
    AUDIO_DPGA1                     = 1,
    AUDIO_DPGA2                     = 2,
    AUDIO_DPGA3                     = 3,
    AUDIO_DPGA_ALL                  = 4,
} AUDIO_DPGA_SEL;

// Audio channel selection
typedef enum
{
    E_AUDIO_LEFT_CH                 = 0,
    E_AUDIO_RIGHT_CH                = 1,
    E_AUDIO_LEFT_RIGHT_CH           = 2
} AUDIO_CH_SEL;

// Audio source selection
typedef enum
{
    AUDIO_SOURCE_HDMI               = 0,
    AUDIO_SOURCE_LINEIN0            = 1,
    AUDIO_SOURCE_DMA_RD     = 2,
    AUDIO_SOURCE_DP                     = 3,
    AUDIO_SOURCE_NONE                   = 4,
} AUDIO_SOURCE_SEL;

//==============================================
/// Audio Codec IN SEL (REG_SEL_CODEC_IN = 0x140252[6:4])
typedef enum
{
    AUDIO_MUX_HDMI_DP_DECODER       = 0,
    AUDIO_MUX_ADC_INPUT             = 4,
} AUDIO_DECODE_CH_SWITCH_MUX_SEL;

//==============================================
/// Internal PCM generator
typedef enum __AUDIO_PCM_FREQ
{
    PCM_250Hz,
    PCM_500HZ,
    PCM_1KHZ,
    PCM_1500HZ,
    PCM_2KHZ,
    PCM_3KHZ,
    PCM_4KHZ,
    PCM_6KHZ,
    PCM_8KHZ,
    PCM_12KHZ,
    PCM_16KHZ
} AUDIO_PCM_FREQ;

typedef enum
{
    DE_POP_STEP1                    = 0,
    DE_POP_STEP2                    = 1,
    DE_POP_STEP3                    = 2,
    DE_POP_STEP4                    = 3,
    DE_POP_STEP5                    = 4,
    DE_POP_STEP6                    = 5,
    DE_POP_STEP7                    = 6,
    DE_POP_Finish                   = 7,
    DE_POP_STEP_MAX
} DAC_INIT_STEP;

typedef enum
{
    PD_DE_POP_STEP1                    = 0,
    PD_DE_POP_STEP2                    = 1,    // Enable DAC Vref and OPLP and CLK
    PD_DE_POP_STEP3                    = 2,    // Enable LOOP and Slect Channel One
    PD_DE_POP_STEP4                    = 3,    // Enable SW1
    PD_DE_POP_STEP5                    = 4,    // Enable SW2
    PD_DE_POP_STEP6                    = 5,    // Enable CLASS_AB
    PD_DE_POP_Finish            = 6,
    PD_DE_POP_STEP_MAX
} PD_DAC_INIT_STEP;

typedef enum
{
    WKUP_DE_POP_STEP1                    = 0,
    WKUP_DE_POP_STEP2                    = 1,
    WKUP_DE_POP_STEP3                    = 2,
    WKUP_DE_POP_STEP4                    = 3,
    WKUP_DE_POP_STEP5                    = 4,
    WKUP_DE_POP_STEP6                    = 5,
    WKUP_DE_POP_STEP7                    = 6,
    WKUP_DE_POP_STEP8                    = 7,
    WKUP_DE_POP_Finish                    = 8,
    WKUP_DE_POP_STEP_MAX
} DAC_WKUP_STEP;

typedef enum
{
    STDBY_DE_POP_STEP1                    = 0,
    STDBY_DE_POP_STEP2                    = 1,
    STDBY_DE_POP_STEP3                    = 2,
    STDBY_DE_POP_STEP4                    = 3,
    STDBY_DE_POP_STEP5                    = 4,
    STDBY_DE_POP_STEP6                    = 5,
    STDBY_DE_POP_Finish                    = 6,
    STDBY_DE_POP_STEP_MAX
} DAC_STDBY_STEP;

typedef enum
{
    AUDIO_I2S_FORMAT_I2S_JUSTIFY,
    AUDIO_I2S_FORMAT_LEFT_JUSTIFY,
}AUDIO_I2S_FORMAT_SEL;

typedef enum
{
    AUDIO_I2S_BCK_AU32FS,           // AUPLL 32FS
    AUDIO_I2S_BCK_AU48FS,           // AUPLL 48FS
    AUDIO_I2S_BCK_AU64FS,           // AUPLL 64FS
    AUDIO_I2S_BCK_RESERVED,
    AUDIO_I2S_BCK_SYN32FS,          // Synthesizer 32FS
    AUDIO_I2S_BCK_SYN64FS,          // Synthesizer 64FS
    AUDIO_I2S_BCK_SYN128FS,         // Synthesizer 128FS
    AUDIO_I2S_BCK_RESERVED1,
}AUDIO_I2S_BCK_SEL;

typedef enum
{
    AUDIO_I2S_MCK_AU128FS,          // AUPLL 128FS
    AUDIO_I2S_MCK_AU256FS,          // AUPLL 256FS
    AUDIO_I2S_MCK_AU192FS,          // AUPLL 192FS
    AUDIO_I2S_MCK_AU384FS,          // AUPLL 384FS
    AUDIO_I2S_MCK_SYN128FS,         // Synthesizer 128FS
    AUDIO_I2S_MCK_SYN256FS,         // Synthesizer 256FS
    AUDIO_I2S_MCK_SYN192FS,         // Synthesizer 192FS
    AUDIO_I2S_MCK_SYN384FS,         // Synthesizer 384FS
}AUDIO_I2S_MCK_SEL;


typedef enum
{
    T_VGA                           = 0,
    T_DVI_SINGLE                    = 1,
    T_DVI_DUAL                      = 2,
    T_DVI                           = 3,
    T_VD                            = 4,
    T_HDMI                          = 5,
    T_MHL                           = 6,
    T_DP                            = 7,
    T_HBR2_0                        = 8,
    T_HBR2_1                        = 9,
    T_HBR2                          = 10,
    T_RESERVED                      = 11
} INPUT_SRC_TYPE;


typedef enum
{
    CH_MUTE                         = 0,
    CH_STEREO                       = 1,
    CH_MULTI                        = 2,
    CH_DOWNMIX                      = 3,
    CH_TONE                         = 4,
} CHANNEL_TYPE;

typedef enum
{
    AUDIO_DPGA_MUTE                 = 0,
    AUDIO_SOFTWARE_MUTE             = 1,
} AUDIO_MUTE_TYPE;


typedef enum
{
    E_DPGA_INIT_MUTE_OFF            = 0x00,
    E_DPGA_INIT_MUTE_ON             = 0x01,
    E_DPGA_MENU_MUTE_OFF            = 0x02,
    E_DPGA_MENU_MUTE_ON             = 0x03,
    E_DPGA_AVSYNC_MUTE_OFF          = 0x04,
    E_DPGA_AVSYNC_MUTE_ON           = 0x05,
    E_DPGA_SOURCE_CHANGE_MUTE_OFF   = 0x06,
    E_DPGA_SOURCE_CHANGE_MUTE_ON    = 0x07,
    E_DPGA_TIMING_CHANGE_MUTE_OFF   = 0x08,
    E_DPGA_TIMING_CHANGE_MUTE_ON    = 0x09,
    E_DPGA_MODE_CHANGE_MUTE_OFF     = 0x0A,
    E_DPGA_MODE_CHANGE_MUTE_ON      = 0x0B,
    E_DPGA_DELAY_CHANGE_MUTE_OFF    = 0x0C,
    E_DPGA_DELAY_CHANGE_MUTE_ON     = 0x0D,
    E_DPGA_INTERNAL_1_MUTE_OFF      = 0x0E,
    E_DPGA_INTERNAL_1_MUTE_ON       = 0x0F,

    E_SW_INIT_MUTE_OFF              = 0x10,
    E_SW_INIT_MUTE_ON               = 0x11,
    E_SW_MENU_MUTE_OFF              = 0x12,
    E_SW_MENU_MUTE_ON               = 0x13,
    E_SW_AVSYNC_MUTE_OFF            = 0x14,
    E_SW_AVSYNC_MUTE_ON             = 0x15,
    E_SW_SOURCE_CHANGE_MUTE_OFF     = 0x16,
    E_SW_SOURCE_CHANGE_MUTE_ON      = 0x17,
    E_SW_MAILBOX_MUTE_OFF           = 0x18,
    E_SW_MAILBOX_MUTE_ON            = 0x19,
    E_SW_TIMING_CHANGE_MUTE_OFF     = 0x1A,
    E_SW_TIMING_CHANGE_MUTE_ON      = 0x1B,
    E_SW_MODE_CHANGE_MUTE_OFF       = 0x1C,
    E_SW_MODE_CHANGE_MUTE_ON        = 0x1D,
    E_SW_INTERNAL_1_MUTE_OFF        = 0x1E,
    E_SW_INTERNAL_1_MUTE_ON         = 0x1F,

} AUDIO_MUTE_SOURCE_SEL;


// Audio Volume Fraction Part
typedef enum
{
    VOLUME_FRACTION_MINUS_dot0dB    = 0,
    VOLUME_FRACTION_MINUS_dot125dB  = 1,
    VOLUME_FRACTION_MINUS_dot25dB   = 2,
    VOLUME_FRACTION_MINUS_dot375dB  = 3,
    VOLUME_FRACTION_MINUS_dot5dB    = 4,
    VOLUME_FRACTION_MINUS_dot625dB  = 5,
    VOLUME_FRACTION_MINUS_dot75dB   = 6,
    VOLUME_FRACTION_MINUS_dot875dB  = 7,
} AUDIO_VOLUME_FRACTION_PART;

typedef enum
{
    CHANNEL_2 = 0,
    CHANNEL_4 = 1,
    CHANNEL_6 = 2,
    CHANNEL_8 = 3,
} AUDIO_CHANNEL;


//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
//=============================================================
// AUDIO Drv FUNCTION
//=============================================================
extern void msDrvAudioSineGen(Bool bEnable, WORD u16SinGenFreq, BYTE u8SinGenGAIN);
extern void msDrvAudioI2SConfig(AUDIO_I2S_FORMAT_SEL eI2SFormat, AUDIO_I2S_BCK_SEL eI2SBCK, AUDIO_I2S_MCK_SEL eI2SMCK );
extern void msDrvAudioDPGASetVolume( AUDIO_DPGA_SEL eDPGA, AUDIO_CH_SEL eChSelect, signed short iVolume_Integer, AUDIO_VOLUME_FRACTION_PART Volume_Fraction );
extern void msDrvAudioSourceSel( AUDIO_SOURCE_SEL src );

//=============================================================
// AUDIO API FUNCTION
//=============================================================
extern void msAPI_AudioInit( void );
extern void msAPI_AudioHandler( void );
extern void msAPI_AudioPowerDown(void );

extern void msAPI_AudioSetMute( AUDIO_MUTE_TYPE mute_type_sel, AUDIO_MUTE_SOURCE_SEL mute_src_type_sel );
extern void msAPI_AudioSetDpgaMute( AUDIO_DPGA_SEL eDPGA, MS_BOOL ebable );
extern void msAPI_AudioSetEarphoneMute( MS_BOOL enable );

extern void msAPI_AudioSourceSel( BYTE InputPort );
extern void msAPI_AudioSetVolume( AUDIO_DPGA_SEL eDPGA, AUDIO_CH_SEL eChSelect, int iVolume_Integer, AUDIO_VOLUME_FRACTION_PART Volume_Fraction );
extern void msDrvAudio_SetDPGAMuteSrc(AUDIO_MUTE_SOURCE_SEL mute_src_type);

#endif //_MSDAC_H_


