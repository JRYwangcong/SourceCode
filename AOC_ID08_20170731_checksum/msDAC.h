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


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

// Audio channel selection
typedef enum _AUDIO_CH_SEL
{
    E_AUDIO_LEFT_CH = 0,
    E_AUDIO_RIGHT_CH = 1,
    E_AUDIO_LEFT_RIGHT_CH =2
} AUDIO_CH_SEL;

// Line-out source selection
typedef enum _AUDIO_LINEOUT_SOURCE_SEL
{
    E_LINEOUT_FROM_LINEIN0 = 0,
    E_LINEOUT_FROM_LINEIN1 = 1,
    E_LINEOUT_FROM_DAC = 2,
    E_LINEOUT_FROM_AVSS = 3 // ground
} AUDIO_LINEOUT_SOURCE_SEL;

// Earphone-out source selection 
typedef enum _AUDIO_EAROUT_SOURCE_SEL
{
    E_EAROUT_FROM_LINEIN0 = 0,
    E_EAROUT_FROM_LINEIN1 = 1,
    E_EAROUT_FROM_LINEOUT = 2,
    E_EAROUT_FROM_DACOUT = 3
} AUDIO_EAROUT_SOURCE_SEL;


//Internal PCM generator
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
}AUDIO_PCM_FREQ;
//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
extern bit bToggleGainFlag;
extern BYTE ToggleGainCntr;
#define ToggleGainPeriod    50 // unit: ms

extern void msAudioDAC_Init(void);
extern void msAudioLineout_Mute(void);
extern void msAudioLineout_SetVolume(AUDIO_CH_SEL chsel, int volume);
extern void msAudioLineout_SourceSel(AUDIO_LINEOUT_SOURCE_SEL src);
//extern AUDIO_LINEOUT_SOURCE_SEL msAudioLineout_GetSourceSel(void);
extern void msAudioEARout_MuteOnOff(Bool bEnable);
extern void msAudioEAR_SourceSel(AUDIO_EAROUT_SOURCE_SEL src);
//extern AUDIO_EAROUT_SOURCE_SEL msAudioEARout_GetSourceSel(void);
//extern void msAudioDACPCMGenerator(Bool bEnable, AUDIO_PCM_FREQ eFreq);
extern void msAudioDACPowerDown(Bool bPowerDN);
extern void msAudioLineOutGain(BYTE val);
extern void msAudioGainToggle(void);

#endif //_MSDAC_H_


