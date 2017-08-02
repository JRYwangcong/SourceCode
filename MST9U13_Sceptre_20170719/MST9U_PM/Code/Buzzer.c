#include "buzzer.h"
#include "Ms_rwreg.h"
#include "global.h"
#include "Common.h"
#include "drvGPIO.h"

#define BUZZER_DEBUG    1
#if ENABLE_DEBUG&&BUZZER_DEBUG
    #define BUZZER_printData(str, value)   printData(str, value)
    #define BUZZER_printMsg(str)           printMsg(str)
#else
    #define BUZZER_printData(str, value)
    #define BUZZER_printMsg(str)
#endif

#if ENABLE_BUZZER
#if 0
void LowFrequencyBeep(void)
{
//3khz frequency and 50% duty and 200ms
    g_ucPWMTimer = BUZZER_TIME;
    msDrvPWMFreq(BUZZER_PWM, BUZZER_LOWFREQ_FREQ);
    msAPIPWMDuty(BUZZER_PWM, BUZZER_DUTY);
    BUZZER_printMsg("**Low Frequency Beep**");
}
void HighFrequencyBeep(void)
{
//4.6khz frequency and 50% duty and 200ms
    g_ucPWMTimer = BUZZER_TIME;
    msDrvPWMEnableSwitch(BUZZER_PWM, TRUE);
    msDrvPWMFreq(BUZZER_PWM, BUZZER_HIGHFREQ_FREQ);
    msAPIPWMDuty(BUZZER_PWM, BUZZER_DUTY);
    BUZZER_printMsg("**High Frequency Beep**");
}
#endif
void BuzzerInitial(void)
{
    BYTE u8RegShift;
    for(u8RegShift=0;u8RegShift<2;u8RegShift++)     //PWM pad control
    {
        msWrite2ByteMask(REG_000408+4*u8RegShift,BUZZER_PWM>>10*u8RegShift,0x3FF);
        msWriteByteMask(REG_000418+u8RegShift,BUZZER_PWM>>(20+6*u8RegShift),0x3F);
    }
    BUZZER_printMsg("BuzzerInitial");
}
#if 0
void MuteBuzzer(void)
{
#if (BOARD_TYPE_SEL == BD_DELL_AW2310)
       return;
#endif
    msWriteBit(REG_1B09, FALSE, _BIT4);
    BUZZER_printData("MuteBuzzer 1%x",msReadByte(REG_1B09)	);

    msSetPwmDuty(PWM4_Channel,0xFF);
}
#endif
void LowBeep(void)
{
// 800Hz frequency and 50% duty and 200ms
    g_ucPWMTimer = BUZZER_TIME;
    msDrvPWMEnableSwitch(BUZZER_PWM, TRUE);
    msDrvPWMFreq(BUZZER_PWM, BUZZER_LOW_FREQ);
    msAPIPWMDuty(BUZZER_PWM, BUZZER_DUTY);
    BUZZER_printMsg("**Low Frequency Beep**");

}
void HighBeep(void)
{
//1.92KHz frequency and 50% duty and 200ms
    g_ucPWMTimer = BUZZER_TIME;
    msDrvPWMEnableSwitch(BUZZER_PWM, TRUE);
    msDrvPWMFreq(BUZZER_PWM, BUZZER_HIGH_FREQ);
    msAPIPWMDuty(BUZZER_PWM, BUZZER_DUTY);
    BUZZER_printMsg("**High Frequency Beep**");
}
void BuzzerMute(void)
{
    //g_bTriggerBuzzer = FALSE;
    msDrvPWMEnableSwitch(BUZZER_PWM, FALSE);
    BUZZER_printMsg("**Mute Beep**");
}
#endif
