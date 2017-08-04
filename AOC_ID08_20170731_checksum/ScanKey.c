
////#if Scan_Source==Cypress
#include "board.h"
#if ScanKey
#include "types.h"
#include "Ms_reg.h"
#include "KeypadDef.h"
#include "SynaOT.h"
#include "Power.h"
#include "Misc.h"
#include "scanKey.h"
#include "global.h"
#include "Ms_rwReg.h"
#include "Menudef.h"
#ifdef EnableSlider
#include <math.h>
#endif
//#include "Dell_OTConfig.h"

//extern void Scan_KeyBuzzer(BYTE ucType);
extern void NVRam_WriteByte(WORD addr, BYTE value);
extern Bool ExecuteKeyEvent(MenuItemActionType menuAction);
extern void printData(char *str, WORD value);
extern BYTE mStar_ReadOSDByte(BYTE address);

extern BYTE MenuPageIndex;
extern BYTE MenuItemIndex;


void ScanKey_SetLedStatus(BYTE LEDstatus);
void KeyLED_TurnOn(void);
void KeyLED_TurnOff(void);
void KeyLED_TurnOn_Dark(void);


#if Scan_Source==Cypress
BYTE xdata pOneTouchDataRegs[2]; // OneTouch Data Registers
void ResetKeyPower(void);
#ifdef ProgramKey
//================ for programming key attribute========================================
Bool CheckKeyRegisterValue(BYTE address, BYTE *buffer)
{
    BYTE tempBuffer = address;
    BYTE tempCnt;

    for(tempCnt = 0; tempCnt < 3; tempCnt ++)
    {
        ForceDelay1ms(5);
        if(OT_WriteI2C(OT_ADDR, &tempBuffer, 1) == OT_SUCCESS)
        {
            ForceDelay1ms(5);
            if(OT_ReadI2C(OT_ADDR, buffer, 1) == OT_SUCCESS)
            {
                return TRUE;
            }
            else if(tempCnt == 2)
            {
                *buffer = ReadFail;
                return FALSE;
            }
        }
        else if(tempCnt == 2)
        {
            *buffer = WriteFail;
            return FALSE;
        }
    }
}

void ProgramKeyInitialTable(void)
{
    ForceDelay1ms(5);
    OT_WriteI2C(OT_ADDR, SetupMode, sizeof(SetupMode));
    ForceDelay1ms(100); // wait for writing into key flash
}

Bool KeyInitialTableCorrect(void)
{
    BYTE xdata tempBuffer[123];
    BYTE tempCnt;

    // 0xa0, 0x04 read
    ForceDelay1ms(5);
    tempBuffer[0] = 0xA0;
    tempBuffer[1] = 0x04;
    if(OT_WriteI2C(OT_ADDR, tempBuffer, 2) == OT_SUCCESS)
    {
        ForceDelay1ms(5);
        if(OT_ReadI2C(OT_ADDR, tempBuffer, 123) == OT_SUCCESS)
        {
            for(tempCnt = 0; tempCnt < 123; tempCnt ++)
            {
                if(tempBuffer[tempCnt] != SetupMode[tempCnt+2])
                return FALSE;
            }
        }
    }

    return TRUE;
    
}

void LoadScanKeyTable(void)
{
    BYTE tempCnt, tempBuffer;
    bit bInitialFlag = 0;
    
    // check if iic bus is idle
    for(tempCnt = 0; tempCnt < 10; tempCnt ++)
    {
        if(SK_SCLHi() && SK_SDAHi())
            break;
        
        Delay1ms(20);
    }

    // iic bus is abnormal
    if(tempCnt >= 10)
        return;

    if(CheckKeyRegisterValue(0x4f, &tempBuffer) == FALSE && tempBuffer == WriteFail)
    {
        bInitialFlag = 1; // set flag for initialing key
        bKeyInitialFlag = 1; // 1: enlarge waiting ack period

        // separate each command for it may need to fine tune delay period for different command
        Delay1ms(5);
        OT_WriteI2C(OT_ADDR_Default, InitialAddress1, 4);
        Delay1ms(5);
        OT_WriteI2C(OT_ADDR_Default, InitialAddress2, 2);
        Delay1ms(5);
        OT_WriteI2C(OT_ADDR_Default, InitialAddress3, 4);
    }

    if(bInitialFlag) // brand-new key
    {
        for(tempCnt = 0; tempCnt < 2; tempCnt ++)
        {
            ProgramKeyInitialTable();
            ResetKeyPower();
            if(KeyInitialTableCorrect() == TRUE)
            {
                break;
            }
        }
    }
    else if(KeyInitialTableCorrect() == FALSE) // check key contents
    {
        bKeyInitialFlag = 1; // 1: enlarge waiting ack period

        for(tempCnt = 0; tempCnt < 2; tempCnt ++)
        {
            ProgramKeyInitialTable();
            ResetKeyPower();
            if(KeyInitialTableCorrect() == TRUE)
            {
                break;
            }
        }
    }

    bKeyInitialFlag = 0;
    
}
#endif
//============== for programming key attribute==========================================

void KeyPower(bit on)
{
    if(on)
    {
        Set_SK_KeyVcc();
        ForceDelay1ms(5);
        Set_SK_SDA();
        Set_SK_SCL();
        ForceDelay1ms(150);
    }
    else
    {
        Clr_SK_KeyVcc();
        Clr_SK_SDA();
        Clr_SK_SCL();
        ForceDelay1ms(150);
    }
}

void ResetKeyPower(void)
{
    KeyPower(TurnOff);
    KeyPower(TurnOn);
}

void ScanKey_Init(void)
{
    ResetKeyPower();
    #ifdef ProgramKey
    LoadScanKeyTable();
    #endif

    #ifdef EnableSlider
    PreKeyStatus = 0;
    PreSliderStatus = SliderFingerOff;
    SliderIncDecStep = 0;
    #endif
}

#ifdef LEDBrightCtrlByScalerPWM
void ScanKey_LEDBrightAction(void)
{
    BYTE tempBank = mStar_ReadByte(0x00);

    mStar_WriteByte(0x00, 0x00);

    if(SKLEDStatus == SKLEDBright_Off)
    {
        KeyLedCounter = 0;
        if(mStar_ReadByte(LEDPWM) != LEDSetting_Off)
            mStar_WriteByte(LEDPWM, LEDSetting_Off);
    }
    else if(SKLEDStatus == SKLEDBright_Dark)
    {
        if(LEDSetting_Dark > LEDSetting_Light) // dark pwm value is larger than light pwm value
        {
            if(mStar_ReadByte(LEDPWM) < LEDSetting_Dark)
                mStar_WriteByte(LEDPWM, mStar_ReadByte(LEDPWM)+1);
        }
        else
        {
            if(mStar_ReadByte(LEDPWM) > LEDSetting_Dark)
                mStar_WriteByte(LEDPWM, mStar_ReadByte(LEDPWM)-1);
        }
    }
    else if(SKLEDStatus == SKLEDBright_Light)
    {
        if(LEDSetting_Dark > LEDSetting_Light) // dark pwm value is larger than light pwm value
        {
            if(mStar_ReadByte(LEDPWM) > LEDSetting_Light)
                mStar_WriteByte(LEDPWM, mStar_ReadByte(LEDPWM)-1);
        }
        else
        {
            if(mStar_ReadByte(LEDPWM) < LEDSetting_Light)
                mStar_WriteByte(LEDPWM, mStar_ReadByte(LEDPWM)+1);
        }
    }
    else if(SKLEDStatus == SKLEDBright_Light_Immediately)
    {
        KeyLedCounter = 0; // LED status keep in light
        if(mStar_ReadByte(LEDPWM) != LEDSetting_Light)
            mStar_WriteByte(LEDPWM, LEDSetting_Light);
    }

    mStar_WriteByte(0x00, tempBank);
}
#endif

#ifdef EnableLightBar
/*
void ScanKey_EnablePWM(bit ctrl_on)
{
    BYTE pButtonBuffer[2];

    ForceDelay1ms(5); // cypress must delay min. 4ms 
    pButtonBuffer[0] = 0x04;

    if(PowerOnFlag)
    {
        if(ctrl_on)
        {
            if(PowerSavingFlag)
                pButtonBuffer[1] = 0x02|PWMEnableBit;
            else
                pButtonBuffer[1] = 0x01|PWMEnableBit;
        }
        else
        {
            if(PowerSavingFlag)
                pButtonBuffer[1] = 0x02;
            else
                pButtonBuffer[1] = 0x01;
        }
    }
    else
    {
            pButtonBuffer[1] = 0x00;
    }
        
    OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 2);
    
    SKReadCaseCnt = SKCase_SetButtonAddress;
    TPDebunceCounter = SKPollingInterval;
    Clr_StartScanKeyFlag();
}


void ScanKey_CtrlPWM(BYTE val)
{
    BYTE pButtonBuffer[2];
    BYTE code lightbartbl[11]=
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08, 0x09, 0x0b, 0x0e
    };

    ForceDelay1ms(5); // cypress must delay min. 4ms 
    // 090706
    ScanKey_EnablePWM(0);
    ForceDelay1ms(5); // cypress must delay min. 4ms 
    
    pButtonBuffer[0] = PWMAddress;
    pButtonBuffer[1] = lightbartbl[val/10];
    OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 2);

    // 090706
    ForceDelay1ms(5); // cypress must delay min. 4ms 
    ScanKey_EnablePWM(1);

    SKReadCaseCnt = SKCase_SetButtonAddress;
    TPDebunceCounter = SKPollingInterval;
    Clr_StartScanKeyFlag();
}
*/
void ScanKey_PWMCtrl(BYTE val)
{
    BYTE code lightbartbl[11]={0x00, 0x01, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08, 0x09, 0x0B, 0x0F   };
    BYTE pButtonBuffer[2];
    BYTE ucPWMReg=0;

    ForceDelay1ms(5); // cypress must delay min. 4ms 
    if (val)
    {
        ucPWMReg=BIT2;
        pButtonBuffer[0] = PWMAddress;
        pButtonBuffer[1] = lightbartbl[val/10];
        OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 2);
    }
                
    pButtonBuffer[0] = 0x04;    
    if(PowerOnFlag)
    {
        if(PowerSavingFlag)
            pButtonBuffer[1] = 0x02|ucPWMReg;
        else
            pButtonBuffer[1] = 0x01|ucPWMReg;
    }
    else
    {
            pButtonBuffer[1] = 0x00;
    }
    OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 2);   

    SKReadCaseCnt = SKCase_SetButtonAddress;
    TPDebunceCounter = SKPollingInterval;
    Clr_StartScanKeyFlag();
}
#endif

BYTE ScanKey_GetKeypadStatus(void)
{
    BYTE keypad=0xFF;

    if(!StartScanKeyFlag)
    {
        return keypad;
    }
    else
    {
    
#if DEBUG_ENABLE && DEBUG_SCANKEY
    //LoopCnt++;
#endif
    
        switch (SKReadCaseCnt)
        {
            case SKCase_SetButtonAddress:
                
                if(OT_Set_ReadAddress(ButtonAddress) == OT_SUCCESS)
                {
                    SKReadCaseCnt = SKCase_ReadButton;
                    SKKeyErrorCnt = 0;
                }
                else // even set 89 fail, it should release power-key status.
                {
                    SKReadCaseCnt = SKCase_SetButtonAddress;
                    SKKeyErrorCnt ++;
                    Set_KeyReadyFlag();
                    #if 0//DEBUG_ENABLE && DEBUG_SCANKEY
                    printData("   Set89_Fail", 1);
                    #endif
                }

                #ifdef EnableSlider
                PreKeyStatus = 0;
                #endif
                
            break;
            
            case SKCase_ReadButton:

                if(OT_ReadI2C(OT_ADDR,(BYTE*)&pOneTouchDataRegs[0],1) == OT_SUCCESS)
                {
                    #if 0//DEBUG_ENABLE && DEBUG_SCANKEY
                    if(pOneTouchDataRegs[0])
                        printData("   SKCase_ReadButton:%x", pOneTouchDataRegs[0]);
                    #endif

                    if(pOneTouchDataRegs[0]&SK_Power)
                    {
                        #ifdef EnableSlider
                        PreKeyStatus |= KEY_POWER;
                        #else
                        keypad &= ~KEY_POWER;
                        #endif                  
                    
                        #if DEBUG_ENABLE && DEBUG_SCANKEY
                        printData("   KEY_POWER", 1);
                        #endif                                              
                    }

                    if(pOneTouchDataRegs[0]&SK_Exit)
                    {
                        #ifdef EnableSlider
                        PreKeyStatus |= KEY_EXIT;
                        #else
                        keypad &= ~KEY_EXIT;
                        #endif
                    
                        #if DEBUG_ENABLE && DEBUG_SCANKEY
                        printData("   KEY_EXIT", 1);
                        #endif
                    }

                    if(pOneTouchDataRegs[0]&SK_Plus)
                    {
                        keypad &= ~KEY_PLUS;
                    
                        #if DEBUG_ENABLE && DEBUG_SCANKEY
                        printData("   KEY_PLUS", 1);
                        #endif
                    }
                    
                    if(pOneTouchDataRegs[0]&SK_Minus)
                    {
                        keypad &= ~KEY_MINUS;
                        #if DEBUG_ENABLE && DEBUG_SCANKEY
                        printData("   KEY_MINUS", 1);
                        #endif
                    }

                    if(pOneTouchDataRegs[0]&SK_Menu)
                    {
                    #ifdef EnableSlider
                        PreKeyStatus |= KEY_MENU;
                    #else
                        keypad &= ~KEY_MENU;
                    #endif
                    
                        #if DEBUG_ENABLE && DEBUG_SCANKEY
                        printData("   KEY_MENU", 1);
                        #endif
                    }
                    
                    SKKeyErrorCnt = 0;

                }
                else
                {
                    SKKeyErrorCnt ++;
#if 0//DEBUG_ENABLE && DEBUG_SCANKEY
        printData("   Read89_Fail", 1);
#endif
                }

    #ifdef EnableSlider
                SKReadCaseCnt = SKCase_SetSliderAddress;
    #else
                Set_KeyReadyFlag();
                SKReadCaseCnt = SKCase_SetButtonAddress;
    #endif                
            break;

    #ifdef EnableSlider
            case SKCase_SetSliderAddress:

                if(OT_Set_ReadAddress(SliderAddress) == OT_SUCCESS)
                {
                    SKReadCaseCnt = SKCase_ReadSlider;
                    SKKeyErrorCnt = 0;
                }
                else // even set 89 fail, it should release power-key status.
                {
                    SKReadCaseCnt = SKCase_SetButtonAddress;
                    SKKeyErrorCnt ++;
                    keypad &= ~PreKeyStatus;
                    Set_KeyReadyFlag();
#if DEBUG_ENABLE && DEBUG_SLIDER
        printData("   SetSliderAddress_Fail", 1);
#endif
                }

            break;

            case SKCase_ReadSlider:

                if(OT_ReadI2C(OT_ADDR,(BYTE*)&pOneTouchDataRegs[0],1) == OT_SUCCESS)
                {
                    BYTE sliderstatus = pOneTouchDataRegs[0];
                    
                    if(sliderstatus < SliderFingerOff) // finger on
                    {
                        if(PreSliderStatus >= SliderFingerOff)
                        {
                            PreSliderStatus = sliderstatus;
                            FingerOnCounter = 300;

                            SliderIncDecStep = 0;

#if DEBUG_ENABLE && DEBUG_SLIDER
        printData("   FingerFirstOn:%x", PreSliderStatus);
#endif
                            
                        }
                        else if(FingerOnCounter)
                        {
                            if(abs(PreSliderStatus-sliderstatus) > 5) // finger shift
                            {
                                // record difference of slider
                                SliderIncDecStep = abs(PreSliderStatus-sliderstatus);
                            
                                if(sliderstatus > PreSliderStatus)
                                    PreKeyStatus |= KEY_PLUS;
                                else
                                    PreKeyStatus |= KEY_MINUS;
                                
                                FingerOnCounter = 0;

                                // act step by shifting distance
                                SliderIncDecStep = (SliderIncDecStep>20)?(SliderIncDecStep/10):(2);

#if DEBUG_ENABLE && DEBUG_SLIDER
        printData("   FingerShift-Cntr:%x", PreSliderStatus);
#endif
                                
                            }
                        }
                        else if(FingerOnCounter == 0)
                        {
                            if(abs(PreSliderStatus-sliderstatus) > 5) // finger shift
                            {
                                // record difference of slider
                                SliderIncDecStep = abs(PreSliderStatus-sliderstatus);

                                if(sliderstatus > PreSliderStatus)
                                    PreKeyStatus |= KEY_PLUS;
                                else
                                    PreKeyStatus |= KEY_MINUS;
                                
                                // act step by shifting distance
                                SliderIncDecStep = (SliderIncDecStep>20)?(SliderIncDecStep/10):(2);

#if DEBUG_ENABLE && DEBUG_SLIDER
        printData("   FingerShift-Cntr0:%x", PreSliderStatus);
#endif
                            
                            }
                            else // finger hold
                            {
                                PreSliderStatus = sliderstatus;
                                if(PreSliderStatus <= SliderMinusMaxValue)
                                    PreKeyStatus |= KEY_MINUS;
                                else
                                    PreKeyStatus |= KEY_PLUS;

                                SliderIncDecStep = 1;
                            }
                                
#if DEBUG_ENABLE && DEBUG_SLIDER
        printData("   FingerHold:%x", PreSliderStatus);
#endif
                        }
                        else // impossible case
                        {
                            PreSliderStatus = SliderFingerOff;
                            SliderIncDecStep = 0;
#if DEBUG_ENABLE && DEBUG_SLIDER
        printData("   Impossible", 1);
#endif
                        }

#if 0//DEBUG_ENABLE && DEBUG_SCANKEY
        printData("   SKCase_ReadSlider:%x", pOneTouchDataRegs[0]);
#endif
                    }
                    else // finger off
                    {
                        FingerOnCounter = 0;
                        PreSliderStatus = SliderFingerOff;
                        SliderIncDecStep = 0;

#if DEBUG_ENABLE && DEBUG_SLIDER
        printData("   FingerOff", 1);
#endif
                
                    }
                }
                else
                {
                    FingerOnCounter = 0;
                    PreSliderStatus = SliderFingerOff;
                    SliderIncDecStep = 0;
                    SKKeyErrorCnt ++;

#if DEBUG_ENABLE && DEBUG_SLIDER
        printData("   ReadSlider_Fail", 1);
#endif
                    
                }

                keypad &= ~PreKeyStatus;
                Set_KeyReadyFlag();
                SKReadCaseCnt = SKCase_SetButtonAddress;
                
            break;                
    #endif
            
            default:
                
                SKReadCaseCnt = SKCase_SetButtonAddress;
                SKKeyErrorCnt = 0;
            #ifdef EnableSlider
                FingerOnCounter = 0;
                PreSliderStatus = SliderFingerOff;
                SliderIncDecStep = 0;
            #endif
            
            break;    
        }

        if(SKKeyErrorCnt >= SKKeyError)
        {
#if DEBUG_ENABLE && DEBUG_SCANKEY
        printData("   keypadError:%d", SKKeyErrorCnt);
#endif
        
            SKKeyErrorCnt = 0;
            ScanKey_Init();

            if(PowerOnFlag)
            {
                if(PowerSavingFlag)
                {
                    hw_SetAmberLed();
                }
                else
                {
                    hw_SetGreenLed();
                }
            #ifdef EnableLightBar
                //ScanKey_CtrlPWM(UserPrefLightBar);
                //ScanKey_EnablePWM(1);
                ScanKey_PWMCtrl(UserPrefLightBar);
            #endif                
            }
            else // power off
            {
                hw_ClrAmberLed();
            #ifdef EnableLightBar
                //ScanKey_EnablePWM(0);
                ScanKey_PWMCtrl(0);
            #endif                
            }

            
        }

        //Clr_StartScanKeyFlag();
        //TPDebunceCounter = SKPollingInterval;

    }

    #ifdef LEDFlash
    if(LEDFlashChangeFlag && DisplayLogoFlag)
    {
        BYTE pButtonBuffer[2];

        LEDFlashStep = LEDFlashStep << 1;
        if(LEDFlashStep > BIT3)
            LEDFlashStep = BIT0;
        
        pButtonBuffer[0] = 0x04;
        pButtonBuffer[1] = ~LEDFlashStep;
        
        OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 2);
        Clr_LEDFlashChangeFlag();
        LEDFlashCounter = LEDFlashChangeInterval;
        
    }
    #endif

    return keypad;
    
}
#ifdef LEDFlash
void SK_TurnOn4LEDs(void)
{
    BYTE pButtonBuffer[2];
    pButtonBuffer[0] = 0x04;
    pButtonBuffer[1] = 0x00;
    OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 2);
}
#else
void SK_CtrlGreenLed(bit ctrl_on)
{
    #ifdef LEDCtrlByScalerIOPin

    ClrAmberLed();
    if(ctrl_on)
        SetGreenLed();
    else
        ClrGreenLed();
    
    #else
    BYTE pButtonBuffer[2];

    ForceDelay1ms(5); // cypress must delay min. 4ms 
    pButtonBuffer[0] = 0x04;
    
    #ifdef EnableLightBar
    if(ctrl_on)
    {
        if (UserPrefLightBar)
            pButtonBuffer[1] = 0x01|PWMEnableBit;
        else
            pButtonBuffer[1] = 0x01;
    }        
    else
        pButtonBuffer[1] = 0x00;    // pButtonBuffer[1] = 0x00|PWMEnableBit;
    #else
    if(ctrl_on)
        pButtonBuffer[1] = 0x01;
    else
        pButtonBuffer[1] = 0x00;
    #endif
        
    OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 2);
    
    SKReadCaseCnt = SKCase_SetButtonAddress;
    TPDebunceCounter = SKPollingInterval;
    Clr_StartScanKeyFlag();
    #endif    
}

void SK_CtrlAmberLed(bit ctrl_on)
{
    #ifdef LEDCtrlByScalerIOPin

    ClrGreenLed();
    if(ctrl_on)
        SetAmberLed();
    else
        ClrAmberLed();
    
    #else
    BYTE pButtonBuffer[2];
    
    ForceDelay1ms(5); // cypress must delay min. 4ms 
    pButtonBuffer[0] = 0x04;

    #ifdef EnableLightBar
    if(ctrl_on)
        pButtonBuffer[1] = 0x02;   //pButtonBuffer[1] = 0x02|PWMEnableBit;
    else
        pButtonBuffer[1] = 0x00;   //pButtonBuffer[1] = 0x00|PWMEnableBit;
    #else    
    if(ctrl_on)
        pButtonBuffer[1] = 0x02;
    else
        pButtonBuffer[1] = 0x00;
    #endif    

    OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 2);
    
    SKReadCaseCnt = SKCase_SetButtonAddress;
    TPDebunceCounter = SKPollingInterval;
    Clr_StartScanKeyFlag();
    #endif    
}
#endif
//===============================================================

#else //if Scan_Source==Synaptics

#define		OT_LED_REG_START_ADDR_HIGH		0x01
#define		OT_LED_REG_START_ADDR_LOW		0x08
extern BYTE OT_Set_ReadAddress(void);
BYTE xdata pOneTouchDataRegs[OT_NUM_DATA_REG_BYTES]; // OneTouch Data Registers
BYTE xdata Scan_LEDStatus=0;
/*	void OT_SWReset(void)
{
	BYTE	pButtonBuffer[4]={0x03, 0x00, 0x00, 0x01};
	OT_WriteI2C(OT_ADDR,(BYTE*)&pButtonBuffer, 4);
}*/
void ScanKey_Init(void)
{
    BYTE i=0;
	if( OT_Init()==OT_FAILURE )
	{   
	    // 071227
	    //hw_SK_INT = 0; // 071228
		if(OT_HAL_BitRead_ATTN() ==0)
	    {
                for(i = 0; i < 3; i ++)
                {
                    if(OT_Init() == OT_SUCCESS)
                        break;
                }
	    }
	}
        // 071227
#if 0//DEBUG_ENABLE
    if(i<3)
    printData("KeyInitOK", 1);
    else
    printData("KeyInitError", 1);
#endif
        
}
//=============================================================================

#define TPREG_MENU			0x01
#define TPREG_MINUS		       0x08 // TPREG_SELECT
#define TPREG_EXIT			0x02 // TPREG_PLUS
#define TPREG_SELECT		0x10 // TPREG_MINUS
#define TPREG_PLUS			0x20 // TPREG_EXIT
#define TPREG_POWER		0x04 // TPREG_POWER

#if ModelName == AOC_2218FH
BYTE ScanKey_GetKeypadStatus(void)
{
	BYTE keypad=0xFF, tempbyte=0;

        if ((P0_3 & BIT0) == 0)
            keypad &= ~KEY_POWER;
	else if(PowerOnFlag/* && StartScanKeyFlag*/)
	{
	     BYTE i;
            for(i = 0; i < 3; i ++)
            {
                Delay1ms(2);
                if(OT_ReadI2C(OT_ADDR,(BYTE*)&pOneTouchDataRegs,2) == OT_SUCCESS)
                {
                    if(pOneTouchDataRegs[1] == TPREG_EXIT)
                    {
                        keypad &= ~KEY_MINUS;
#if 0//DEBUG_ENABLE
    printData("KEY_MINUS", i);
#endif
                    }
                    else if(pOneTouchDataRegs[1] ==  TPREG_SELECT)
                    {
                        keypad &= ~KEY_PLUS;
#if 0//DEBUG_ENABLE
    printData("KEY_PLUS", i);
#endif
                    }
                    else if(pOneTouchDataRegs[1] == TPREG_PLUS)
                    {
                        keypad &= ~KEY_MENU;
#if 0//DEBUG_ENABLE
    printData("KEY_MENU", i);
#endif
                    }
                    else if(pOneTouchDataRegs[1] ==TPREG_MINUS)
                    {
                        keypad &= ~KEY_EXIT;
#if 0//DEBUG_ENABLE
    printData("KEY_EXIT", i);
#endif
                    }
                    break;
                }
            }

            if(i >= 3)
            {
#if 0//DEBUG_ENABLE
    printData("KeyError", i);
#endif
                ScanKey_Init();
                //OT_Set_ReadAddress();
                if(KeyLEDStatus == KeyLEDStatus_On)
                    Set_keyLEDTurnOnFlag();
            }
            
	}

#if 0 // 080122
        if(PowerOnFlag &&
           (MenuPageIndex == RootMenu || (MenuPageIndex == StandbyMenu)) && 
           (/*keypad == ~KEY_MENU || */keypad == ~KEY_MINUS || keypad == ~KEY_EXIT ||
            keypad == ~KEY_SELECT || keypad == ~KEY_PLUS/* || keypad == ~KEY_POWER*/))
        {
            Set_keyLEDTurnOnFlag();
            tempbyte = 0xff;
        }
#endif

        if(KeyLEDTurnOffFlag)
        {
            Clr_keyLEDTurnOffFlag();
            KeyLED_TurnOn_Dark();
        }
        else if(keyLEDTurnOnFlag)
        {
            // 080122
            //if((KeyLEDStatus == KeyLEDStatus_Off) && (tempbyte == 0xff))
            //    keypad = 0xff;
            Clr_keyLEDTurnOnFlag();
            KeyLED_TurnOn();
        }
    

	return keypad;


}
#else
BYTE ScanKey_GetKeypadStatus(void)
{
	BYTE keypad=0xFF, tempbyte=0;

        if ((P0_3 & BIT0) == 0)
            keypad &= ~KEY_POWER;
	else if(PowerOnFlag/* && StartScanKeyFlag*/)
	{
	     BYTE i;
            for(i = 0; i < 3; i ++)
            {
                Delay1ms(2);
                if(OT_ReadI2C(OT_ADDR,(BYTE*)&pOneTouchDataRegs,2) == OT_SUCCESS)
                {
#if AllInOneKey
                    if(pOneTouchDataRegs[1] ==TPREG_MINUS)
#else
                    if(pOneTouchDataRegs[1]== TPREG_MENU)
                        {
                        keypad &= ~KEY_MENU;
#if 0//DEBUG_ENABLE
    printData("KEY_MENU", i);
#endif
                        }
                    else if(pOneTouchDataRegs[1] ==TPREG_MINUS)
#endif
                        {
                        keypad &= ~KEY_MINUS;
#if 0//DEBUG_ENABLE
    printData("KEY_MINUS", i);
#endif
                        }
                    else if(pOneTouchDataRegs[1] == TPREG_EXIT)
                        {
                        keypad &= ~KEY_EXIT;
#if 0//DEBUG_ENABLE
    printData("KEY_EXIT", i);
#endif
                        }
#if AllInOneKey
                    else if(pOneTouchDataRegs[1] ==  TPREG_SELECT)
                        {
         		  keypad &= ~KEY_MENU;
#if 0//DEBUG_ENABLE
    printData("KEY_MENU", i);
#endif
                        }
#else
                    else if(pOneTouchDataRegs[1] ==  TPREG_SELECT)
                        {
         		  keypad &= ~KEY_SELECT;
#if 0//DEBUG_ENABLE 
    printData("KEY_SELECT", i);
#endif
                        }
#endif                    
                    else if(pOneTouchDataRegs[1] == TPREG_PLUS)
                        {
        		  keypad &= ~KEY_PLUS;
#if 0//DEBUG_ENABLE
    printData("KEY_PLUS", i);
#endif
                        }
                    break;
                }
            }
            
            //Clr_StartScanKeyFlag();
            //TPDebunceCounter=1;//DefTPDebunceCounter;

            if(i >= 3)
            {
#if 0//DEBUG_ENABLE
    printData("KeyError", i);
#endif
                ScanKey_Init();
                if(KeyLEDStatus == KeyLEDStatus_On)
                    Set_keyLEDTurnOnFlag();
            }
            
	}

        // 071228 key led
#if 0        
        {
            BYTE tempBank;
            tempBank = mStar_ReadByte(0x00);
            mStar_WriteByte(0x00, 0x00);
            if((KeyLEDStatus == KeyLEDStatus_On) && (mStar_ReadOSDByte(IOSDC1) & BIT0))
            {
                KeyLedCounter = 5;
                Clr_keyLEDTurnOffFlag();
            }
            mStar_WriteByte(0x00, tempBank);    
        }
#endif

#if AllInOneKey
        if(PowerOnFlag && 
           (MenuPageIndex == RootMenu || (MenuPageIndex == EnergyStarMenu)) && 
           (keypad == ~KEY_MINUS || keypad == ~KEY_EXIT || keypad == ~KEY_POWER ||
            keypad == ~KEY_SELECT || keypad == ~KEY_PLUS))
#else
        if(PowerOnFlag && (keypad != ~KEY_POWER) && 
           (MenuPageIndex == RootMenu || (MenuPageIndex == EnergyStarMenu)) && 
           (keypad == ~KEY_MENU || keypad == ~KEY_MINUS || keypad == ~KEY_EXIT ||
            keypad == ~KEY_SELECT || keypad == ~KEY_PLUS))
#endif            
        {
            Set_keyLEDTurnOnFlag();
            tempbyte = 0xff;
        }

        if(KeyLEDTurnOffFlag)
        {
            Clr_keyLEDTurnOffFlag();
            //KeyLedCounter = 0;
            //KeyLED_TurnOff();
            KeyLED_TurnOn_Dark();
        }
        else if(keyLEDTurnOnFlag)
        {
            if((KeyLEDStatus == KeyLEDStatus_Off) && (tempbyte == 0xff))
                keypad = 0xff;
            Clr_keyLEDTurnOnFlag();
            KeyLED_TurnOn();
            //KeyLedCounter = 0;//UserPrefOsdTime
        }
    

	return keypad;


}
#endif

void ScanKey_SetLedStatus(BYTE LEDstatus)
{
	BYTE	pButtonBuffer[4]={0x00, 0x0E, 0x7F, 0x00};
	pButtonBuffer[3]|=LEDstatus;
	Scan_LEDStatus=LEDstatus;
	OT_WriteI2C(OT_ADDR, (BYTE*)&pButtonBuffer, 4);
	OT_Set_ReadAddress();

}

void KeyLED_TurnOn(void)
{
        //Set_keyLEDOnFlag();
    	 //ScanKey_Init();
        BYTE i;
    	 BYTE code LEDBrit[]={
        0x00, 0x24, 0x00, 0x1e, // menu and exit   
        //0x00, 0x24, 0x1f, 0x1f, // menu and exit   
        //0x00, 0x25, 0x00, 0x00, //   
        0x00, 0x26, 0x1e, 0x1e, // +and-   
        0x00, 0x27, 0x00, 0x18, // select
        };
        for(i=0;i<3;i++)
        {
            OT_WriteI2C(OT_ADDR, &(LEDBrit+(i*4)), 4);
            Delay1ms(2);
        }
        
        ScanKey_SetLedStatus(ExitLed|InputSelectLed|PlusLed|MinsLed|MenuLed);
        KeyLEDStatus = KeyLEDStatus_On;
}

void KeyLED_TurnOff(void)
{
        //Clr_keyLEDOnFlag();
        //ScanKey_Init();
        ScanKey_SetLedStatus(0);
        KeyLEDStatus = KeyLEDStatus_Off;
}

void KeyLED_TurnOn_Dark(void)
{
        BYTE i;
        //Set_keyLEDOnFlag();
    	 //ScanKey_Init();
    	 BYTE code LEDBrit[]={
        0x00, 0x24, 0x00, 0x0b,    
        //0x00, 0x25, 0x00, 0x00,    
        0x00, 0x26, 0x0b, 0x0b,   // +and-
        0x00, 0x27, 0x00, 0x06, // select
        };

        KeyLEDStatus = KeyLEDStatus_Dark;
        
        for(i=0;i<3;i++)
        {
            OT_WriteI2C(OT_ADDR, &(LEDBrit+(i*4)), 4);
            Delay1ms(2);
        }
        
	 OT_Set_ReadAddress();
}
#endif








#endif
