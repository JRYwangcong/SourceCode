#include "types.h"
#include "board.h"
#include "keypaddef.h"
#include "ms_reg.h"
#include "global.h"
#include "debug.h"
#include "misc.h"
#include "ms_rwreg.h"
#include "mcu.h"
#include "debug.h"
#include <math.h>    //MST_MCU
#include "menudef.h"
#if ScanKey
#include "ScanKey.h"
#endif
#if USEFLASH
#include "userpref.h"
#endif
extern void SaveMonitorSetting(void);
#ifdef PowerKeyDeBounce
bit PushPowerKeyFlag=0;
BYTE xdata PushPowerKeyDeBounce=0;
#endif
//================================================================
xdata BYTE KeypadButton = 0;
#if Encoder
    BYTE PrevKeypadStatus = 0;
#endif
//2006-07-10
#if !ScanKey
#if AllInOneKey
//AllInOneKey Key Define 
//Up Key       ->   KEY_EXIT
//Down Key   ->   KEY_MENU
//Left Key      ->   KEY_MINUS
//Right Key    ->   KEY_PLUS
#ifdef TSUMXXT		//110311 Modify
#define SAR_VALID_MASK    0xFF
#else
#define SAR_VALID_MASK    0x3F
#endif
BYTE Key_GetKeypadStatus(void)  //charles
{
    BYTE keypad = 0xFF;
#if 0//ScanKey
            keypad = ScanKey_GetKeypadStatus();
#if DEBUG_ENABLE // 071225
    if(keypad)
        printData("keypad:%x", keypad);
#endif
    return keypad;
#else
    BYTE temp, temp1, retry_Key;

    retry_Key = 3;
    while (retry_Key)
    {
    #if (MainBoardType==MainBoard_715G2575)
        temp = KEYPAD_ADC4 &SAR_VALID_MASK;			//110311 Modify
        Delay1ms(2);
        temp1 = KEYPAD_ADC4 &SAR_VALID_MASK;		//110311 Modify
    #else
        temp = KEYPAD_ADC2 &SAR_VALID_MASK;			//110311 Modify
        Delay1ms(2);
        temp1 = KEYPAD_ADC2 &SAR_VALID_MASK;		//110311 Modify
    #endif
        if (abs(temp - temp1) < 5)
            break;
        retry_Key--;
    }
    #if (MainBoardType==MainBoard_2573||MainBoardType==MainBoard_2578)
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_MINUS;
        else if (temp >= 36 && temp <= 41)
            keypad &= ~KEY_EXIT;
    #elif (ModelName==AOC_F19)
        if (temp >= 18 && temp <= 28)
            keypad &= ~KEY_PLUS;
        else if (temp >= 32 && temp <= 42)
            keypad &= ~KEY_EXIT;
    #elif (MainBoardType==MainBoard_715G2575)
        if(temp>=32 && temp<=42)
            keypad &= ~KEY_PLUS;
        else if(temp>=18 && temp<=26)
            keypad &= ~KEY_MENU;
    #elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529)
	if(temp>=32 && temp<44)
		keypad &= ~KEY_MENU;//~KEY_MINUS;
	else if(temp>=17 && temp<28)
		keypad &= ~KEY_PLUS;//~KEY_MENU;
    #elif (MainBoardType==MainBoard_715G4734)
	if(temp>=32 && temp<44)
		keypad &= ~KEY_MENU;//~KEY_MINUS;
	else if(temp>=17 && temp<28)
		keypad &= ~KEY_PLUS;//~KEY_MENU;
    #elif (ModelName==AOC_917Fwx)
        if (temp >= 18 && temp <=26)
            keypad &= ~KEY_PLUS;
        else if (temp >= 36 && temp <= 41)
           keypad &= ~KEY_MENU;
    #elif (MainBoardType==MainBoard_2698_2_X_X_2)
        if (temp >= 0 && temp <=6)
            keypad &= ~KEY_MENU;
        else if (temp >= 33 && temp <= 41)
            keypad &= ~KEY_MINUS;
    #elif (MainBoardType==MainBoard_2698_2_X_X_8)
        if(temp >= 18 && temp <= 26)
            keypad &= ~KEY_PLUS;
        else if(temp>=32 && temp<=42)
            keypad &= ~KEY_EXIT;
#elif (MainBoardType==MainBoard_2810_B||MainBoardType==MainBoard_715G2808 || MainBoardType==MainBoard_715G2988)
    #if ModelName == AOC_2230FH || ModelName == AOC_2218FH
        if (temp >= 0 && temp <=6)
            keypad &= ~KEY_EXIT;
        else if (temp >= 33 && temp <= 41)
            keypad &= ~KEY_PLUS; 
    #else
        if (temp >= 0 && temp <=6)
            keypad &= ~KEY_EXIT;
        else if (temp >= 33 && temp <= 41)
            keypad &= ~KEY_MINUS; 
    #endif
 #elif (MainBoardType==MainBoard_2883_B_X_X_2)//yuanfa
    #if ModelName == AOC_2230F||ModelName==AOC_2230Fa||ModelName==AOC_2230F_LTM220M1L01
        if (temp >= 32 && temp <=42)
            keypad &= ~KEY_MENU;
        else if (temp >= 17 && temp <= 27)
            keypad &= ~KEY_MINUS;
    #else
	    if (temp >= 0x10 && temp <= 0x20)
            keypad &= ~KEY_PLUS;
        else if (temp >= 0x21 && temp <= 0x30)
            keypad &= ~KEY_EXIT;
    #endif
 #elif (MainBoardType==MainBoard_2186_2A)//yuanfa
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_PLUS;
        else if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_MENU;
#elif (MainBoardType==MainBoard_2670_C)	    
   #if (ModelName==ENVISION_G2217)  //Amy2007.12.20   8:49:14
  	     if(temp>=30 && temp<48)
	    keypad &= ~KEY_MINUS;//for 5key
        else if(temp>=0 && temp<18)
           keypad &= ~KEY_EXIT; //for 5key
     #endif
 #elif (MainBoardType==MainBoard_2670_1)//yuanfa
  	     if(temp>=30 && temp<48)
	    keypad &= ~KEY_MINUS;//for 5key
        else if(temp>=0 && temp<18)
           keypad &= ~KEY_EXIT; //for 5key
    #elif (MainBoardType==MainBoard_2883_1B)
	if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_MENU; // circus down
	else if (temp >= 17 && temp <= 27)
		keypad &= ~KEY_MINUS; // circus left	
    #elif (MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A||\
              MainBoardType == MainBoard_715G3663_M0A)	
	if (abs(temp-0)<10) // 0V, original 22
		keypad &= ~KEY_EXIT; // circus up
	else if (abs(temp-38)<10) // 1.11V, original 48
		keypad &= ~KEY_PLUS; // circus right
	#elif (MainBoardType==MainBoard_715G3329_58N)
	if(temp>=0&& temp<=7)
	 keypad &= ~KEY_MINUS;
	else if(temp>=32 && temp<=42)
		 keypad &= ~KEY_EXIT;
	#elif (MainBoardType==MainBoard_715G3329_18N)
	if(temp>=0&& temp<=7)
	 keypad &= ~KEY_MINUS;
	else if(temp>=32 && temp<=42)
		 keypad &= ~KEY_EXIT;
	#elif(MainBoardType==MainBoard_4432)
    if( temp >= 0 && temp < 0x12 )
        keypad &= ~KEY_PLUS;
    else if( temp >= 0x12 && temp < 0x30 )
        keypad &= ~KEY_EXIT;
	#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
		 if( temp >=  0x2D && temp < 0x38 )
			keypad &= ~KEY_MENU;
		else if (temp >=  0x4B && temp < 0x5D )
			keypad &= ~KEY_MINUS;
	#elif (MainBoardType==MainBoard_715G3959_T)		//110617 Modify
		if( temp < 0x10 )
			keypad &= ~KEY_MENU;
		else if (temp < 0x65 )
			keypad &= ~KEY_MINUS;
    #else
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_MENU;
        else if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_MINUS;
    #endif
    #if 0//DEBUG_PRINTDATA // 090410
    if(temp<60)
    printData("ADC2=%d",temp);
    #endif
    retry_Key = 3;
    while (retry_Key)
    {
        temp = KEYPAD_ADC3 &SAR_VALID_MASK;			//110311 Modify
        Delay1ms(2);
        temp1 = KEYPAD_ADC3 &SAR_VALID_MASK;		//110311 Modify
        if (abs(temp - temp1) < 5)
            break;
        retry_Key--;
    }
    #if (MainBoardType==MainBoard_2573||MainBoardType==MainBoard_2578)
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_SELECT;
        else if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_MENU;
        else if (temp >= 36 && temp <= 41)
            keypad &= ~KEY_PLUS;
    #elif (ModelName==AOC_F19)
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_MENU;
        else if (temp >= 32 && temp <= 42)
            keypad &= ~KEY_MINUS;
    #elif (ModelName==AOC_917Fwx)
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_EXIT;
        else if (temp >= 36 && temp <= 41)
            keypad &= ~KEY_MINUS;
	#elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529)
		if(temp>=0 && temp<5)
			keypad &= ~KEY_EXIT;//~KEY_PLUS;
		else if(temp>=32 && temp<44)
			keypad &= ~KEY_MINUS;//~KEY_EXIT;
      #elif (MainBoardType==MainBoard_715G4734)
		if(temp>=0 && temp<5)
			keypad &= ~KEY_EXIT;//~KEY_PLUS;
		else if(temp>=32 && temp<44)
			keypad &= ~KEY_MINUS;//~KEY_EXIT;
    #elif (MainBoardType==MainBoard_715G2575)
        if(temp>=0 && temp<=6)
            keypad &= ~KEY_MINUS;
        else if(temp>=32 && temp<=42)
            keypad &= ~KEY_EXIT;
    #elif (MainBoardType==MainBoard_2698_2_X_X_2)
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_PLUS;
        else if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_SELECT;
        //2006-05-26 Andy
        else if (temp >= 36 && temp <= 41)
            keypad &= ~KEY_EXIT;
    #elif (MainBoardType==MainBoard_2698_2_X_X_8)
        if(temp >= 18 && temp <= 26)
            keypad &= ~KEY_MENU;
        else if(temp>=32 && temp<=42)
            keypad &= ~KEY_MINUS;
 #elif (MainBoardType==MainBoard_2810_B||MainBoardType==MainBoard_715G2808||MainBoardType==MainBoard_715G2988)
    #if ModelName == AOC_2230FH || ModelName == AOC_2218FH
        if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_MINUS;
        else if (temp >= 36 && temp <= 41)
            keypad &= ~KEY_MENU;
    #else
        if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_PLUS;
    //    else if (temp >= 18 && temp <= 26)
    //        keypad &= ~KEY_SELECT;
        else if (temp >= 36 && temp <= 41)
            keypad &= ~KEY_MENU;
    #endif
 #elif (MainBoardType==MainBoard_2883_B_X_X_2)//yuanfa
    #if ModelName == AOC_2230F||ModelName==AOC_2230Fa||ModelName==AOC_2230F_LTM220M1L01
        if (temp >= 0&& temp <=6)
            keypad &= ~KEY_EXIT;
        else if (temp >= 32 && temp <=42)
            keypad &= ~KEY_PLUS;
	#else
        if (temp >= 0x20 && temp <= 0x30)
            keypad &= ~KEY_MINUS;
        else if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_MENU;	
		#endif
 #elif (MainBoardType==MainBoard_2186_2A)//yuanfa
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_MINUS;
        else if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_EXIT;
#elif (MainBoardType==MainBoard_2670_C)
	#if   (ModelName==ENVISION_G2217)  //Amy2007.12.20   8:49:42
  	     if(temp>=18 && temp<26)
	     keypad &= ~KEY_PLUS;  //for 5key
	     else if(temp>=32 && temp<42)
	     keypad &= ~KEY_MENU; //for 5key
        #endif
 #elif (MainBoardType==MainBoard_2670_1)//yuanfa
        if (temp >= 36 && temp <= 41)
            keypad &= ~KEY_MENU;
        else if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_PLUS;
    #elif (MainBoardType==MainBoard_2883_1B)
        if (temp >= 32 && temp <= 42)
            keypad &= ~KEY_PLUS; // circus right
        else if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_EXIT; // circus up	
    #elif (MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A||\
              MainBoardType == MainBoard_715G3663_M0A) 
        if (abs(temp-22)<7)  // 0.68V // (temp >= 32 && temp <= 42) // 37
            keypad &= ~KEY_MINUS; // circus left
        else if (abs(temp-38)<7) // 1.11V// (temp >= 45 && temp <= 51) // 48
            keypad &= ~KEY_MENU; // circus down
	#elif (MainBoardType==MainBoard_715G3329_58N)
		if(temp>=17&& temp<=27)
		   keypad &= ~KEY_MENU;
		else if(temp>=32 && temp<=42)
			keypad &= ~KEY_PLUS;
		else if(temp>=0 && temp<=6)
			keypad &= ~KEY_SELECT;
	#elif (MainBoardType==MainBoard_715G3329_18N)
		if(temp>=17&& temp<=27)
		   keypad &= ~KEY_MENU;
		else if(temp>=32 && temp<=42)
			keypad &= ~KEY_PLUS;
		else if(temp>=0 && temp<=6)
			keypad &= ~KEY_SELECT;
	#elif(MainBoardType==MainBoard_4432)
       if( temp >= 0x0B && temp < 0x1D ) // Rex 100726 change the power key
        keypad &= ~KEY_MENU;
       else if( temp >= 0x1D && temp < 0x30 )
        keypad &= ~KEY_MINUS;
	#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
		if( temp >= 0x00 && temp < 0x17 )
			keypad &= ~KEY_PLUS;
		else if( temp >= 0x2D && temp < 0x8B)
			keypad &= ~KEY_EXIT;
	#elif (MainBoardType==MainBoard_715G3959_T)		//110617 Modify
		if( temp < 0x10 )
			keypad &= ~KEY_PLUS;
		else if (temp < 0x65 )
			keypad &= ~KEY_EXIT;
	#else
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_EXIT;
        else if (temp >= 18 && temp <= 26)
            keypad &= ~KEY_PLUS;
        else if (temp >= 36 && temp <= 41)
           keypad &= ~KEY_SELECT;

        //2006-05-26 Andy
    #endif
    #if 0//DEBUG_PRINTDATA
    if(temp<60)
    printData("ADC3=%d",temp);
    #endif
    #if (MainBoardType==MainBoard_2701||MainBoardType==MainBoard_2578||MainBoardType==MainBoard_2698_2_X_X_8||MainBoardType==MainBoard_2883_B_X_X_1)
        if ((P1_6 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_2698_2_X_X_2)
        if ((P1_4 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_715G2575)
        if ((P0_6 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_2810_B||MainBoardType==MainBoard_715G2808||MainBoardType==MainBoard_715G2988)
        if ((P0_3 &BIT0) == 0)
	#elif (MainBoardType==MainBoard_2883_B_X_X_2||MainBoardType==MainBoard_2883_1B)//yuanfa
	    if ((P1_4 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529||MainBoardType==MainBoard_715G4734)
        if ((P1_3&BIT0) == 0)
    #elif (MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A||MainBoardType==MainBoard_715G3329_58N||\
			MainBoardType==MainBoard_715G3329_18N)
        if((P4&BIT2) == 0)					
    #elif (MainBoardType == MainBoard_715G3663_M0A)
        if ((P1_4 &BIT0) == 0)
	#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
		if ((P1_3&BIT0) == 0)
	#elif (MainBoardType==MainBoard_715G3959_T||MainBoardType==MainBoard_715G5265_T)		//110617 Modify
		if ((P1_4&BIT0) == 0)
    #else
        if ((P0_0 &BIT0) == 0)
    #endif
                keypad &= ~KEY_POWER;

    return keypad;
#endif    
}

#elif ModelName==ENVISION_JRY1PTRL//MainBoard_SLB1PTR
BYTE Key_GetKeypadStatus(void)
{
BYTE keypad = 0xFF;
	BYTE temp;


    temp = KEYPAD_ADC2&0xFF;
	if (temp!=0xff)
	{
	Delay1ms(15);
	while(abs(temp-(KEYPAD_ADC2&0xFF)) > 5)
	{
		Delay1ms(15);
		temp = KEYPAD_ADC2&0xFF;
	}
	}

	if(temp<0x10)
		keypad &= ~KEY_MINUS; // KEY_DOWN;
	
	else if(temp>0x40 && temp<0x50)
		keypad &= ~KEY_PLUS; // KEY_MINUS;
		
	else if(temp>0x60 && temp<0x7A)
		keypad &= ~KEY_EXIT; // KEY_MENU;
		
	else if(temp>0xBA && temp<0xD0)
		keypad &=~KEY_MENU; // KEY_PLUS;

 temp = KEYPAD_ADC3&0xFF;
if (temp!=0xff)
	{
		Delay1ms(15);
		while(abs(temp-(KEYPAD_ADC3&0xFF)) > 5)
		{
			Delay1ms(15);
			temp = KEYPAD_ADC3&0xFF;
		}
	}

	//--{
	if(temp<0x10)
		keypad &= ~KEY_SELECT; // KEY_UP;
	
	//else if(temp>0x40 && temp<0x50)
	//	keypad &= ~KEY_SELECT; // KEY_MENU;
	
	//else if(temp>0x65 && temp<0x7A)
	//	keypad &=~KEY_UP; // KEY_EXIT;
		
if ((P1_4&BIT0) == 0)
	keypad &= ~KEY_POWER;
  return keypad;
}
#elif ModelName==ENVISION_JRYL1PTR_EV1
BYTE Key_GetKeypadStatus(void)
{
	BYTE keypad = 0xFF;
	BYTE temp;

    temp = KEYPAD_ADC2&0xFF;
	if (temp!=0xff)
	{
	Delay1ms(15);
	while(abs(temp-(KEYPAD_ADC2&0xFF)) > 5)
	{
		Delay1ms(15);
		temp = KEYPAD_ADC2&0xFF;
	}
	}

	if(temp<0x10)
		keypad &= ~KEY_PLUS; // ;
	
	else if(temp>0x40 && temp<0x50)
		keypad &= ~KEY_MINUS; // ;
		
	else if(temp>0x60 && temp<0x7A)
		keypad &= ~KEY_EXIT; // ;
		
	else if(temp>0xBA && temp<0xD0)
		keypad &=~KEY_MENU; // ;

 temp = KEYPAD_ADC3&0xFF;
if (temp!=0xff)
	{
		Delay1ms(15);
		while(abs(temp-(KEYPAD_ADC3&0xFF)) > 5)
		{
			Delay1ms(15);
			temp = KEYPAD_ADC3&0xFF;
		}
	}

	//--{
	if(temp<0x10)
		keypad &= ~KEY_SELECT; // KEY_UP;
	
	//else if(temp>0x40 && temp<0x50)
	//	keypad &= ~KEY_SELECT; // KEY_MENU;
	
	//else if(temp>0x65 && temp<0x7A)
	//	keypad &=~KEY_UP; // KEY_EXIT;
		
if ((P1_4&BIT0) == 0)
	keypad &= ~KEY_POWER;
  return keypad;
}
#elif (ModelName==ENVISION_R2218WS||ModelName==ENVISION_R2018WS)
BYTE Key_GetKeypadStatus(void)
{
	BYTE keypad = 0xFF;
	BYTE temp;

    	temp = KEYPAD_ADC2&0xFF;
	if (temp!=0xff)
	{
	Delay1ms(15);
	while(abs(temp-(KEYPAD_ADC2&0xFF)) > 5)
	{
		Delay1ms(15);
		temp = KEYPAD_ADC2&0xFF;
	}
	}

	if(temp<0x10)
		keypad &= ~KEY_EXIT; // KEY_DOWN;
	else if(temp>0x40 && temp<0x50)
		keypad &= ~KEY_SELECT; // KEY_MINUS;
	else if(temp>0xB0 && temp<0xBF)
		keypad &=~KEY_MENU; // KEY_PLUS;

	temp = KEYPAD_ADC3&0xFF;
	if (temp!=0xff)
	{
		Delay1ms(15);
		while(abs(temp-(KEYPAD_ADC3&0xFF)) > 5)
		{
			Delay1ms(15);
			temp = KEYPAD_ADC3&0xFF;
		}
	}

	if(temp<0x10)
		keypad &= ~KEY_MINUS; // KEY_UP;
	else if(temp>0x40 && temp<0x50)
		keypad &= ~KEY_PLUS; // KEY_MENU;
/*	
	else if(temp>0x65 && temp<0x7A)
		keypad &=~KEY_UP; // KEY_EXIT;
//*/
if ((P1_4&BIT0) == 0)
	keypad &= ~KEY_POWER;
  return keypad;
}

#else
#ifdef TSUMXXT		//110311 Modify
#define SAR_VALID_MASK    0xFF
#else
#define SAR_VALID_MASK    0x3F
#endif
BYTE Key_GetKeypadStatus(void)
{
    BYTE keypad = 0xFF;
#if 0//ScanKey
            keypad = ScanKey_GetKeypadStatus();
#if DEBUG_ENABLE // 071225
    if(keypad != 0xff)
        printData("keypad:%x", keypad);
#endif
    return keypad;
#else
    BYTE temp, temp1, retry_Key;
    retry_Key = 3;
    while (retry_Key)
    {
    #if (MainBoardType==MainBoard_715G2575)
        temp = KEYPAD_ADC4 &SAR_VALID_MASK;			//110311 Modify
        Delay1ms(2);
        temp1 = KEYPAD_ADC4 &SAR_VALID_MASK;		//110311 Modify
       #else
        temp = KEYPAD_ADC2 &SAR_VALID_MASK;			//110311 Modify
        Delay1ms(2);
        temp1 = KEYPAD_ADC2 &SAR_VALID_MASK;		//110311 Modify
    #endif
        if (abs(temp - temp1) < 5)
            break;
        retry_Key--;
    }
	#if (MainBoardType==MainBoard_2883_B_X_X_1||ModelName==AOC_919Pwz_Asia||ModelName==AOC_919Pwz_Europe||\
		ModelName==AOC_919P2_Asia||ModelName==AOC_919P2_Europe||ModelName==AOC_919Va2)//yuanfa
	#if ModelName==ENVISION_P851
	if (temp >= 0x10 && temp <= 0x20)
		keypad &= ~KEY_EXIT;
	else if (temp >= 0x21 && temp <= 0x30)
            keypad &= ~KEY_MINUS;
	#else
	if (temp >= 0x10 && temp <= 0x20)
		keypad &= ~KEY_MENU;
	else if (temp >= 0x21 && temp <= 0x30)
            keypad &= ~KEY_MINUS;
	#endif
    #elif (MainBoardType==MainBoard_715G3329_1_2||MainBoardType==MainBoard_715G3329_1_2A)
	if (temp >= 0x10 && temp <= 0x20)
		keypad &= ~KEY_PLUS;
	else if (temp >= 0x28 && temp <= 0x36)
            keypad &= ~KEY_EXIT;
    #elif (ModelName==AOC_931Fwz)
	if (temp >= 0x10 && temp <= 0x20)
		keypad &= ~KEY_MENU;
	else if (temp >= 0x21 && temp <= 0x30)
            keypad &= ~KEY_MINUS;
    #elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529)
	#if(ModelName==ENVISION_H716Wx||ModelName==AOC_831S_Plus)
	if(temp>=32 && temp<44)
		keypad &= ~KEY_PLUS;
	else if(temp>=17 && temp<28)
		keypad &= ~KEY_MENU;
	#else
	if(temp>=32 && temp<44)
		keypad &= ~KEY_MINUS;
	else if(temp>=17 && temp<28)
		keypad &= ~KEY_MENU;
	#endif
    #elif (MainBoardType==MainBoard_715G4734)
        #if(ModelName == AOC_e1620Swb_2)
        if(temp>=15 && temp<69)
    		keypad &= ~KEY_MENU;
    	else if(temp>=70 && temp<115)
    		keypad &= ~KEY_MINUS;
        #else
    	if(temp>=32 && temp<44)
    		keypad &= ~KEY_MINUS;
    	else if(temp>=17 && temp<28)
    		keypad &= ~KEY_MENU;
        #endif
    #elif (MainBoardType==MainBoard_715G2575)
        if(temp>=32 && temp<=42)
            keypad &= ~KEY_PLUS;
        else if(temp>=18 && temp<=26)
            keypad &= ~KEY_MENU;
	
	#elif (MainBoardType==MainBoard_715G2808||MainBoardType==MainBoard_715G2988)
                if(temp>=32 && temp<42)
                    keypad &= ~KEY_EXIT;
                else if (temp>=0 && temp<5)
                    keypad &= ~KEY_MINUS;
    #elif (MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611)	
	#if ( ModelName  == AOC_2219Vwg)//080624
	        if (temp >= 0 && temp <= 0x06)
            keypad &= ~KEY_MINUS;
        else if (temp >= 0x21 && temp <= 0x29)
			keypad &= ~KEY_EXIT;
	#else
	
        if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_PLUS;
        else if (temp >= 32 && temp <= 42)
            keypad &= ~KEY_EXIT;
     #endif
     #elif (MainBoardType==MainBoard_2670_C)
	    #if (ModelName==ENVISION_G2219||AOC_2219P2)//Amy2007.12.26   20:38:35
               if(temp>=30 && temp<48)
	          keypad &= ~KEY_EXIT; //for 6key
               else if(temp>=0 && temp<18)
                keypad &= ~KEY_MINUS; //for 6key
              #endif
	#elif (MainBoardType==MainBoard_2883_B_X_X_2)
	#if ModelName==ENVISION_G2219_LM220WE1||ModelName==AOC_2219P2_2883||ModelName==ENVISION_G2460WG
	    if(temp>=17 && temp<=27)
	    keypad &= ~KEY_MENU; //for 6key
    else if(temp>=33 && temp<=43)
        keypad &= ~KEY_PLUS; //for 6key
    else if(temp>=0 && temp<=6)
	    keypad &= ~KEY_SELECT;
	#else
    if(temp>=17 && temp<=27)
	    keypad &= ~KEY_MENU; //for 6key
    else if(temp>=33 && temp<=43)
        keypad &= ~KEY_MINUS; //for 6key
    #endif
    #elif (MainBoardType==MainBoard_2883_1B||MainBoardType==MainBoard_715G3329_A2||\
              MainBoardType==MainBoard_715G3598_M0A)
	#if ModelName==AOC_2219Vg
	if (temp >= 0 && temp <= 5)
		keypad &= ~KEY_SELECT;
	else if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_PLUS; 
	else if (temp >= 17 && temp <= 27)
		keypad &= ~KEY_MENU; 
	#elif ModelName==AOC_2219P2_3329
	if(temp >= 43 && temp <=53)
		keypad &= ~KEY_EXIT; 
	else if (temp >= 17 && temp <= 27)
		keypad &= ~KEY_MINUS; 
	#elif ModelName==ENVISION_G2360g
	if(temp >= 43 && temp <=53)
		keypad &= ~KEY_SELECT; 
	else if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_EXIT; 
	else if (temp >= 0 && temp <= 6)
		keypad &= ~KEY_MINUS; 
	#elif ModelName==ENVISION_G2470
	if (temp >= 0 && temp <= 5)
		keypad &= ~KEY_PLUS;
	else if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_EXIT; 
       #elif ModelName==ENVISION_G2770||ModelName==AOC_e2795v||ModelName==ENVISION_G2770L 
	if (temp >= 17 && temp <= 27)
		keypad &= ~KEY_PLUS;
	else if (temp >= 43 && temp <= 53)
		keypad &= ~KEY_EXIT;   
       #elif ModelName==ENVISION_G2218
	if(temp>=0&& temp<=7)
		 keypad &= ~KEY_MINUS;
    	else if(temp>=32 && temp<=42)
    		 keypad &= ~KEY_EXIT;
    #elif ((ModelName == AOC_E2795V_AUOM270HW01V2) || (ModelName == ENVISION_G2770L_L20) || (ModelName == AOC_G2770L_AUOM270)|| (ModelName == AOC_E2795V_L20))
        if(temp >= 43 && temp <= 53)  
			 keypad &= ~KEY_EXIT;
		else if(temp >= 17 && temp <= 27)
			 keypad &= ~KEY_PLUS;	
	#else
	if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_MINUS; 
	else if (temp >= 17 && temp <= 27)
		keypad &= ~KEY_EXIT; 
	#endif
	#elif (MainBoardType==MainBoard_715G3329_58N)
	 #if ModelName==ENVISION_G2218WG1
	if(temp>=0&& temp<=7)
	 keypad &= ~KEY_MINUS;
	else if(temp>=51&& temp<=59)
		 keypad &= ~KEY_EXIT;
	#else
	if(temp>=0&& temp<=7)
	 keypad &= ~KEY_MINUS;
	else if(temp>=32 && temp<=42)//42
		 keypad &= ~KEY_EXIT;
	#endif
	#elif (MainBoardType==MainBoard_715G3329_18N)
		if(temp>=17&& temp<=27)
		 keypad &= ~KEY_PLUS;
		else if(temp>=43 && temp<=53)
			 keypad &= ~KEY_EXIT;
    #elif MainBoardType==MainBoard_715G4175_M0A
	if(temp >= 32 && temp <=42)
		keypad &= ~KEY_MINUS; 
	else if (temp >= 17 && temp <= 27)
		keypad &= ~KEY_MENU; 
	#elif(MainBoardType==MainBoard_4432)//add by win 101207
    if( temp >= 0 && temp < 0x12 )
        keypad &= ~KEY_PLUS;
    else if( temp >= 0x12 && temp < 0x30 )
        keypad &= ~KEY_EXIT;
	#elif(MainBoardType==MainBoard_715G3834)
        #if(ModelName == ENVISION_H2276DL)
    		if (temp >= 22 && temp <= 44)
		        keypad &= ~KEY_EXIT;
		    else if (temp >= 46 && temp <= 60)
		        keypad &= ~KEY_PLUS;
        #else
            if(temp>=0&& temp<=7)
    		    keypad &= ~KEY_MINUS;
        	else if(temp>=32 && temp<=42)
        		keypad &= ~KEY_EXIT;
        #endif 
    #elif (MainBoardType == MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2)
        if(temp>=0 && temp<=4)
            keypad &= ~KEY_MINUS;//~KEY_PLUS;
        else if(temp>=34 && temp<=42)
            keypad &= ~KEY_EXIT;
	#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
		 if( temp >=  0x2D && temp < 0x38 )
			keypad &= ~KEY_MENU;
		else if (temp >=  0x4B && temp < 0x5D )
			keypad &= ~KEY_MINUS;
	#elif (MainBoardType==MainBoard_715G3959_T)		//110617 Modify
		if( temp < 0x10 )
			keypad &= ~KEY_MENU;
		else if (temp < 0x65 )
			keypad &= ~KEY_MINUS;
#elif (MainBoardType == MainBoard_715G5265_T)//petit 20121031
    if( temp >= 0x6B && temp < 0x75 )
        keypad &= ~KEY_MINUS;
    else if( temp >= 0x52 && temp < 0x5C )
        keypad &= ~KEY_MENU;
 #elif (MainBoardType == MainBoard_715G6952_M0B)//petit 20121031
                   if(temp< 0x10)
		 keypad &= ~KEY_PLUS;
		else if( temp< 0x65)
			 keypad &= ~KEY_EXIT;
    #elif (MainBoardType == MainBoard_715G6912_M1A)//petit 20121031
                   if(temp< 0x10)
		 keypad &= ~KEY_PLUS;
		else if( temp< 0x65)
			 keypad &= ~KEY_EXIT;
	#endif 
    #if DEBUG_PRINTDATA
    if(temp<60)
    printData("ADC2=%d",temp);
    #endif
    retry_Key = 3;
    while (retry_Key)
    {
        temp = KEYPAD_ADC3 &SAR_VALID_MASK;			//110311 Modify
        Delay1ms(1);
        temp1 = KEYPAD_ADC3 &SAR_VALID_MASK;		//110311 Modify
        if (abs(temp - temp1) < 5)
            break;
        retry_Key--;
    }
	#if (MainBoardType==MainBoard_2883_B_X_X_1||ModelName==AOC_919Pwz_Asia||ModelName==AOC_919Pwz_Europe||\
		ModelName==AOC_919P2_Asia||ModelName==AOC_919P2_Europe||ModelName==AOC_919Va2)//yuanfa
	#if ModelName==ENVISION_P851
        if (temp >= 0x20 && temp <= 0x30)
            keypad &= ~KEY_MENU;
        else if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_PLUS;	
	#else
        if (temp >= 0x20 && temp <= 0x30)
            keypad &= ~KEY_EXIT;
        else if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_PLUS;	
	#endif
    #elif (MainBoardType==MainBoard_715G3329_1_2||MainBoardType==MainBoard_715G3329_1_2A)
	if (temp >= 0x20 && temp <= 0x29)
		keypad &= ~KEY_MENU;
	else if (temp >= 0x2C && temp <= 0x36)
            keypad &= ~KEY_MINUS;
    #elif (ModelName==AOC_931Fwz)
        if (temp >= 0x20 && temp <= 0x30)
            keypad &= ~KEY_EXIT;
        else if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_PLUS;	
#elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529)
	#if(ModelName==ENVISION_H716Wx||ModelName==AOC_831S_Plus)
	if(temp>=0 && temp<5)
		keypad &= ~KEY_MINUS;
	else if(temp>=32 && temp<44)
		keypad &= ~KEY_EXIT;
	#else
	if(temp>=0 && temp<5)
		keypad &= ~KEY_PLUS;
	else if(temp>=32 && temp<44)
		keypad &= ~KEY_EXIT;
	#endif
#elif (MainBoardType==MainBoard_715G4734)
    #if(ModelName == AOC_e1620Swb_2)
    if(temp>=0 && temp<46)
		keypad &= ~KEY_PLUS;
	else if(temp>=47 && temp<115)
		keypad &= ~KEY_EXIT;
    #else
	if(temp>=0 && temp<5)
		keypad &= ~KEY_PLUS;
	else if(temp>=32 && temp<44)
		keypad &= ~KEY_EXIT;
    #endif
 #elif (MainBoardType==MainBoard_715G2575)
        if(temp>=0 && temp<=6)
            {keypad &= ~KEY_MINUS;}
        else if(temp>=32 && temp<=42)
            keypad &= ~KEY_EXIT;   
    #elif (MainBoardType==MainBoard_715G2808||MainBoardType==MainBoard_715G2988)
          	if(temp>=0 && temp<6)
              		keypad &= ~KEY_SELECT;
          	else if(temp>=32 && temp<42)
              		keypad &= ~KEY_PLUS;
          	else if(temp>=17 && temp<27)  	
               	keypad &= ~KEY_MENU;
    #elif (MainBoardType==MainBoard_2670_B||MainBoardType==RDG2670_C_4_X_1_080611)	
      #if ( ModelName  == AOC_2219Vwg)//080624
                  if (temp >= 0 && temp <= 6)
          keypad &= ~KEY_SELECT;//  keypad &= ~KEY_MENU;
        else if (temp >= 0x12 && temp <= 0x20)
            keypad &= ~KEY_MENU; 
		        else if (temp >= 0x21 && temp <= 0x29)
            keypad &= ~KEY_PLUS;
	  #else
		if (temp >= 0 && temp <= 6)
            keypad &= ~KEY_MENU;
        else if (temp >= 32 && temp <= 42)
            keypad &= ~KEY_MINUS; 
	  #endif
     #elif (MainBoardType==MainBoard_2670_C)
	  #if  (ModelName==ENVISION_G2219||ModelName ==AOC_2219P2)  //cy 1116//Amy2007.12.26   20:39:44
              if(temp>=18 && temp<26)
	     keypad &= ~KEY_MENU;  //for 6key
	     else if(temp>=32 && temp<42)
	     keypad &= ~KEY_PLUS; //for 6key
	     else if(temp>=0 && temp<6)
	     keypad &= ~KEY_SELECT;
         #endif
	#elif (MainBoardType==MainBoard_2883_B_X_X_2)
	#if ModelName==ENVISION_G2219_LM220WE1||ModelName==AOC_2219P2_2883||ModelName==ENVISION_G2460WG
    if(temp>=0 && temp<=5)
	    keypad &= ~KEY_MINUS; //for 6key
    else if(temp>=33 && temp<=43)
        keypad &= ~KEY_EXIT; //for 6key
	#else
    if(temp>=0 && temp<=5)
	    keypad &= ~KEY_PLUS; //for 6key
    else if(temp>=33 && temp<=43)
        keypad &= ~KEY_EXIT; //for 6key
    #endif
    #elif (MainBoardType==MainBoard_2883_1B||MainBoardType==MainBoard_715G3329_A2||\
              MainBoardType==MainBoard_715G3598_M0A)
	#if ModelName==AOC_2219Vg
	if (temp >= 0 && temp <= 5)
		keypad &= ~KEY_MINUS;
	else if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_EXIT;
	#elif ModelName==AOC_2219P2_3329
		if (temp >= 17 && temp <= 27)
		keypad &= ~KEY_SELECT;
	else if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_MENU;
	else if (temp >= 43 && temp <= 53)
		keypad &= ~KEY_PLUS;
	#elif ModelName==ENVISION_G2360g
	if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_MENU;
	else if (temp >= 43 && temp <= 53)
		keypad &= ~KEY_PLUS;
	#elif ModelName==ENVISION_G2470
	if (temp >= 17 && temp <= 27)
		keypad &= ~KEY_MENU;
	else if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_MINUS;
	#elif ModelName==ENVISION_G2770||ModelName==AOC_e2795v||ModelName==ENVISION_G2770L 
	if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_MENU;
	else if (temp >= 43 && temp <= 53)
		keypad &= ~KEY_MINUS;
	#elif ModelName==ENVISION_G2218
	if(temp>=17&& temp<=27)
		keypad &= ~KEY_MENU;
    else if(temp>=32 && temp<=42)
    		keypad &= ~KEY_PLUS;
	else if(temp>=0 && temp<=6)
		keypad &= ~KEY_SELECT;
    #elif ((ModelName == AOC_E2795V_AUOM270HW01V2) || (ModelName == ENVISION_G2770L_L20) || (ModelName == AOC_G2770L_AUOM270)|| (ModelName == AOC_E2795V_L20))
        if(temp >= 32 && temp <= 42)
			keypad &= ~KEY_MENU;
		else if(temp >= 43 && temp <= 53)
			keypad &= ~KEY_MINUS;
	#else
	if (temp >= 0 && temp <= 5)
		keypad &= ~KEY_PLUS;
	else if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_MENU;	
	#endif
	#elif (MainBoardType==MainBoard_715G3329_58N)
	 #if ModelName==ENVISION_G2218WG1
		if(temp>=18&& temp<=25)
		 keypad &= ~KEY_MENU;
		else if(temp>=51 && temp<=59)
			keypad &= ~KEY_PLUS;
		else if(temp>=0 && temp<=7)
			keypad &= ~KEY_SELECT;
		#else
		if(temp>=17&& temp<=27)
		   keypad &= ~KEY_MENU;
		else if(temp>=32 && temp<=42)
			keypad &= ~KEY_PLUS;
		else if(temp>=0 && temp<=6)
			keypad &= ~KEY_SELECT;
		#endif
	#elif (MainBoardType==MainBoard_715G3329_18N)
		if(temp>=44&& temp<=53)
		   keypad &= ~KEY_MINUS;
		else if(temp>=32 && temp<=42)
			keypad &= ~KEY_MENU;
    #elif MainBoardType==MainBoard_715G4175_M0A
	if (temp >= 32 && temp <= 42)
		keypad &= ~KEY_EXIT;
	else if (temp >= 0 && temp <= 5)
		keypad &= ~KEY_PLUS;
	#elif(MainBoardType==MainBoard_715G3834)
        #if(ModelName == ENVISION_H2276DL)
    	    if (temp >= 22 && temp <= 44)
	            keypad &= ~KEY_MENU;
	        else if (temp >= 46 && temp <= 60)
	            keypad &= ~KEY_MINUS;
        #else
            if(temp>=17&& temp<=27)
		        keypad &= ~KEY_MENU;
            else if(temp>=32 && temp<=42)
    		    keypad &= ~KEY_PLUS;
	        else if(temp>=0 && temp<=6)
		        keypad &= ~KEY_SELECT;
        #endif 
    
	#elif(MainBoardType==MainBoard_4432)//add by win 101297
          #if ((ModelName==ENVISOIN_P2273wl&&PanelType==PanelCLAA215FA04)||ModelName==ENVISOIN_P2273wlTPM)
               if( temp >= 0x00 && temp < 0x12 ) // Rex 100726 change the power key
                keypad &= ~KEY_MENU;
             else if( temp >= 0x12 && temp < 0x30 )
               keypad &= ~KEY_MINUS;
	#else
	 if( temp >= 0x0B && temp < 0x1D ) // Rex 100726 change the power key
        keypad &= ~KEY_MENU;
       else if( temp >= 0x1D && temp < 0x30 )
        keypad &= ~KEY_MINUS;
	   #endif
    #elif (MainBoardType == MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2)
        if(temp>=0 && temp<=4)
        	keypad &= ~KEY_SELECT;
        else if(temp>=18 && temp<=27)  	
        	keypad &= ~KEY_MENU;	
        else if(temp>=34 && temp<=42)
        	keypad &= ~KEY_PLUS;//~KEY_MINUS;
	#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
	if( temp >= 0x00 && temp < 0x17 )
			keypad &= ~KEY_PLUS;
		else if( temp >= 0x2D && temp < 0x8B)
			keypad &= ~KEY_EXIT;
   #elif (MainBoardType == MainBoard_715G5265_T)//petit 20121031
        if( temp >= 0x6B && temp < 0x75 )
              keypad &= ~KEY_PLUS;
       else if( temp >= 0x50 && temp < 0x5C )
             keypad &= ~KEY_EXIT;
	#elif (MainBoardType==MainBoard_715G3959_T)	//110617 Modify
		if( temp < 0x10 )
			keypad &= ~KEY_PLUS;
		else if (temp < 0x65 )
			keypad &= ~KEY_EXIT;
	#elif (MainBoardType == MainBoard_715G6952_M0B)//petit 20121031
		if( temp < 0x40 ) 
                keypad &= ~KEY_MENU;
             else if(  temp < 0x65)
               keypad &= ~KEY_MINUS;
	#elif (MainBoardType == MainBoard_715G6912_M1A)//petit 20121031
    	if( temp < 0x40 ) 
                keypad &= ~KEY_MENU;
             else if(  temp < 0x65)
               keypad &= ~KEY_MINUS;
    #endif
    #if DEBUG_PRINTDATA
    if(temp<60)
    printData("ADC3=%d",temp);
    #endif
    #if (MainBoardType==MainBoard_2701||MainBoardType==MainBoard_2578||\
           MainBoardType==MainBoard_2698_2_X_X_8||MainBoardType==MainBoard_2883_B_X_X_1||\
           MainBoardType==RDG2670_C_4_X_1_080611||MainBoardType==MainBoard_3289_A2||\
           MainBoardType==MainBoard_715G3289_C2)
        if ((P1_6 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_2698_2_X_X_2)
        if ((P1_4 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_715G2575)
        if ((P0_6 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_2810_B||MainBoardType==MainBoard_715G2808||MainBoardType==MainBoard_715G2988)
        if ((P0_3 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_2883_B_X_X_2||MainBoardType==MainBoard_2883_1B)//yuanfa
        if ((P1_4 &BIT0) == 0)
    #elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G4175_M0A||MainBoardType==MainBoard_715G3529||\
		MainBoardType==MainBoard_715G4734)
        if ((P1_3&BIT0) == 0)
    #elif (MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A||MainBoardType==MainBoard_715G3329_1_2||\
		  MainBoardType==MainBoard_715G3329_58N||MainBoardType==MainBoard_715G3329_18N)
        if((P4&BIT2) == 0)
	#elif (MainBoardType==MainBoard_4432)
        if ((P1_4 &BIT0) == 0)
	#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
		if ((P1_3&BIT0) == 0)
	#elif (MainBoardType==MainBoard_715G3959_T||MainBoardType==MainBoard_715G5265_T)		//110617 Modify
		if ((P1_4&BIT0) == 0)
	#elif (MainBoardType==MainBoard_715G3834)		//110617 Modify
		if ((P1_4&BIT0) == 0)
	#elif (MainBoardType == MainBoard_715G6952_M0B || MainBoardType == MainBoard_715G6912_M1A)//petit 20121031
	       if ((P2_6&BIT0) == 0)
    #else
        if ((P0_0 &BIT0) == 0)
    #endif
                keypad &= ~KEY_POWER;

    return keypad;
#endif    

}
#endif


#endif

#if !ScanKey
void CheckFactoryKeyStatus(void)
{
    BYTE keypadStatus;
    keypadStatus = (Key_GetKeypadStatus() ^ KeypadMask) &KeypadMask;
   // Set_FactoryModeFlag();
    #if AllInOneKey
    if (keypadStatus == KEY_POWER)
    #else
    if (keypadStatus == KEY_MENU)
    #endif
    {
    #if AllInOneKey
    Set_FactoryPowerOnFlag();
    #endif
        Set_FactoryModeFlag();
	Set_BurninModeFlag();
	Set_DoBurninModeFlag();
        Set_PowerOnFlag();
        Set_AutoColorFlag();
    #if !AllInOneKey
	// ES = 1;	
     #endif
        //printData("ES=%d",ES);
        //printMsg("ES");
    #if !USEFLASH
	NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
    #else
	SaveMonitorSetting();
    #endif   

    }
/*    else if (keypadStatus == KEY_ISP)
    {
        Set_PowerOnFlag();
        Set_ISPFlag();
    }*/
}
#endif

#if 0
    void Key_ScanKeypad(void)
    {
        BYTE keypadStatus;
        keypadStatus = (Key_GetKeypadStatus() ^ KeypadMask) &KeypadMask;
        if (keypadStatus)
        {
            if (keypadStatus == KEY_PLUS)
                KeypadButton = BTN_Plus;
            else if (keypadStatus == KEY_MINUS)
                KeypadButton = BTN_Minus;
            else if (keypadStatus == KEY_MENU)
            {
                if (KeypadButton == BTN_Menu || KeypadButton == BTN_Repeat)
                    KeypadButton = BTN_Repeat;
                else
                    KeypadButton = BTN_Menu;
            }
            else if (keypadStatus == KEY_EXIT)
            {
                if (KeypadButton == BTN_Repeat)
                    KeypadButton = BTN_Repeat;
                else
                    KeypadButton = BTN_Exit;
            }
            else if (keypadStatus == KEY_POWER)
            {
                if (KeypadButton == BTN_Power || KeypadButton == BTN_Repeat)
                    KeypadButton = BTN_Repeat;
                else
                    KeypadButton = BTN_Power;
            }
            else if (keypadStatus == (KEY_ISP | KEY_POWER))
            {
                Set_PowerOnFlag();
                Set_ISPFlag();
                KeypadButton = BTN_Power;
            }
            else
                KeypadButton = BTN_Nothing;
        }
        else
            KeypadButton = BTN_Nothing;
    }
#endif


Bool CheckSourceKeyStatus(void)
{ BYTE keypadStatus;
  int i;
if(UnsupportedModeFlag)
	 return FALSE;

  for(i=0;i<33;i++)//  for(i=0;i<50;i++)
  	{
        #if ScanKey // 090517 coding
    SK_CheckAutoKey:      

        // 091020 coding for GPM request
        #if Scan_Source == Cypress
        #ifdef LEDBrightCtrlByScalerPWM
        if(bPowerLEDBrightActStepFlag)
        {
            ScanKey_LEDBrightAction();
            bPowerLEDBrightActStepFlag = 0;
        }
        #endif
        #endif
    
        while(!StartScanKeyFlag);
  	  keypadStatus=(ScanKey_GetKeypadStatus()^KeypadMask)&KeypadMask;
        Clr_StartScanKeyFlag();
        TPDebunceCounter = SKPollingInterval;
        if(!KeyReadyFlag)
            goto SK_CheckAutoKey;
        Clr_KeyReadyFlag();
        #else
  	  Delay1ms(50);
  	  keypadStatus=(Key_GetKeypadStatus()^KeypadMask)&KeypadMask;
        #endif
        
	  #if AllInOneKey     //071222
            if (keypadStatus==KEY_MENU)
	  #else
	     if (keypadStatus==KEY_EXIT)
	  #endif	 	
         	{
         	    SourceKeyCounter++;
		 if(SourceKeyCounter>30)//50
		     break;		 
		 else 	 
		    continue;
         	}
	   else
	   	break;

  	}
  //printData("SourceKeyCounter  %d", SourceKeyCounter);

 return FALSE;
}


extern Bool ExecuteKeyEvent(MenuItemActionType menuAction);
extern void Osd_Hide(void);
extern BYTE  MenuPageIndex;
void Key_ScanKeypad(void)
{
    BYTE keypadStatus=0;
    #if Encoder
        BYTE curKeyStatue;
        curKeyStatue = keypadStatus = Key_GetKeypadStatus() &KeypadMask;

        if (PrevKeypadStatus &KEY_PLUS)
        {
            if ((PrevKeypadStatus ^ curKeyStatue) &KEY_PLUS)
            //Plus 1->0
            {
                if (curKeyStatue &KEY_MINUS)
                {
                    if (PrevKeypadStatus &KEY_MINUS)
                    // minus 1-->1
                    {
                        keypadStatus = KeypadMask &(~KEY_PLUS);
                        //	printMsg("minus 1-->1");
                    }
                    else
                    // minus 0-->1
                    {
                        keypadStatus = keypadStatus | (KEY_PLUS | KEY_MINUS);
                        //	printMsg("minus 0-->1");
                    }
                }
                else
                //current minus 0
                {
                    if (PrevKeypadStatus &KEY_MINUS)
                    // minus 0-->0
                    {
                        keypadStatus = keypadStatus | (KEY_PLUS | KEY_MINUS);
                        //	printMsg("minus 1-->0");
                    }
                    else
                    // minus 1-->0
                    {
                        keypadStatus = KeypadMask &(~KEY_MINUS);
                        //	printMsg("minus 0-->0");
                    }
                }
            }
            else
                keypadStatus = keypadStatus | (KEY_PLUS | KEY_MINUS);
        }
        else
            keypadStatus = keypadStatus | (KEY_PLUS | KEY_MINUS);
        PrevKeypadStatus = curKeyStatue;
        keypadStatus = (keypadStatus ^ KeypadMask) &KeypadMask;
    #else
        #if ScanKey
            if (gBoolVisualKey == 0)
                keypadStatus = (ScanKey_GetKeypadStatus() ^ KeypadMask) &KeypadMask;                                       
            else if (gBoolVisualKey == 1)
            {
                gBoolVisualKey = 0;
                keypadStatus = (gByteVisualKey ^ KeypadMask) &KeypadMask;
            }
#if 0//DEBUG_ENABLE
if(keypadStatus)
printData("keypadStatus%d", keypadStatus);
#endif
        #else
        if (gBoolVisualKey == 0)
            keypadStatus = (Key_GetKeypadStatus() ^ KeypadMask) &KeypadMask;
        else if (gBoolVisualKey == 1)
        {
            gBoolVisualKey = 0;
            keypadStatus = (gByteVisualKey ^ KeypadMask) &KeypadMask;
        }
        #endif
		#if 0//DEBUG_ENABLE
		printData("keypadStatus===%d", keypadStatus);
		#endif
    #endif

    #if ScanKey // 090515 coding, here cause
        #if Scan_Source == Cypress
        if(!KeyReadyFlag)
        {   
            return;
        }
        #endif
    #endif
    
    if (keypadStatus)
    {

#if 0//DEBUG_ENABLE&&DEBUG_SCANKEY// 090624 coding test
printData("  keypadStatus:%x", keypadStatus);
#endif
    
	#if (!AllInOneKey && ModelName!=ENVISION_P2071l&&ModelName!=ENVISION_P2271wl&&ModelName!=ENVISION_P971wl&&ModelName!=ENVISION_P971l)
           if (keypadStatus == KEY_PRODUCT&&ProductModeFlag)//Zhifeng.WU Design For Manufacture When Press "-"&"+"Auto Switch Color Temp.
            {
                      if (MenuPageIndex==RootMenu||
                    MenuPageIndex==HotKeyECOMenu
                    #if AudioFunc
                    ||MenuPageIndex==HotKeyVolMenu
                    #else
                    ||MenuPageIndex==HotKeyDCRMenu
                    #endif
			)
                      {
                          BYTE i;
                          UserPrefLastMenuIndex=2;		  
                          MenuPageIndex=ColorTempMenu;
                          MenuItemIndex=0;
                          ExecuteKeyEvent(MIA_RedrawMenu);     
                           ExecuteKeyEvent(MIA_AdjustValue);     
                          for (i=0;i<5;i++)				   
                          {
                              Delay1ms(500);
                              ExecuteKeyEvent(MIA_DecValue);     
                              if(i==4)
                                  ExecuteKeyEvent(MIA_AdjustValue);     
                          }                         
                         KeypadButton = BTN_Nothing;
                      }
                      else if(MenuPageIndex==ColorTempMenu)
                      {
				Osd_Hide();
				MenuPageIndex=ResetWaitMenu;
                        #if DefaultIconExit//AOC_37_Series || ModelName==ENVISION_P2071l
				MenuItemIndex=8;
                        #else
				MenuItemIndex=0;
                        #endif
				Set_DoResetFlag();  
				ExecuteKeyEvent(MIA_RedrawMenu);   
				Clr_DoResetFlag(); 
				ExecuteKeyEvent(MIA_Power);     
				KeypadButton = BTN_Nothing;
                      }
            else
                KeypadButton = BTN_Nothing;
            return;
            }
	#endif

    #if ScanKey
        if (KeypadButton == BTN_Repeat && 
        (keypadStatus != KEY_LOCK && keypadStatus != KEY_FACTORY))  //(keypadStatus != KEY_LOCK || keypadStatus != KEY_FACTORY))
            return ;
    #else
        if (KeypadButton == BTN_Repeat && keypadStatus != KEY_LOCK)
        // 0105
            return ;
    #endif        
    
        //0105
        {
        #if !OSD_LOCK_FUNCTION_EN
            if (keypadStatus == KEY_LOCK )
            {
                // 091019
                #if DEBUG_ENABLE
                printData("  keypadStatus == KEY_LOCK", 1);
                #endif
            #if AllInOneKey
		if(!PowerOnFlag)
			{
                if(KeypadButton == BTN_Power || KeypadButton == BTN_Repeat)
                {
                    KeypadButton = BTN_Repeat;
                }
                else
                {
                    if(OsdLockModeFlag)
                    {
                        Clr_OsdLockModeFlag();
                        Clr_ShowOsdLockFlag();
                    }
                    else
                    {
                        Set_OsdLockModeFlag();
                        Set_ShowOsdLockFlag();
                    }
                    KeypadButton = BTN_Power;
                #if AllInOneKey                    
                    Clr_DoKeyCountFlag();
                    Set_PowerKeyFlag();
                    Delay1ms(50);
                #endif  
			#if USEFLASH
                    UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit); 
			#else
			SaveMonitorSetting();
			#endif
                }
			}
            #else
                if (!PowerOnFlag && !(MonitorFlags &OsdLockModeFlag))
                {
                    Set_OsdLockModeFlag();
                    //Set_PowerOnFlag();
                    Set_ShowOsdLockFlag();
                    KeypadButton = BTN_Power;
                }
                else if ((!PowerOnFlag) && (MonitorFlags &OsdLockModeFlag))
                {
                    Clr_OsdLockModeFlag();
                    //Set_PowerOnFlag();
                    Clr_ShowOsdLockFlag();
                    KeypadButton = BTN_Power;
                }
		  else if(PowerOnFlag)
		     KeypadButton = BTN_Repeat;

	     #if USEFLASH
            UserPref_EnableFlashSaveBit(bFlashSaveMonitorBit);
            #else
            SaveMonitorSetting();
            #endif

            #endif
                
            }
	#else
	     if (keypadStatus == KEY_LOCK )
		return;
	#endif

        #if 1//ScanKey
            else if(keypadStatus == KEY_FACTORY && (!PowerOnFlag))
            {
            	 #if DEBUG_ENABLE
		printMsg("KEY_FACTORY---->");
	     #endif

            		if(FactoryModeFlag)   
		    	KeypadButton=BTN_Repeat;
			else
	    		KeypadButton=BTN_Power;

			if(!PowerOnFlag)//rafe lau add 030606
	          	{
	          		Set_FactoryModeFlag();
				if (SyncLossState())
				{
					Set_DoBurninModeFlag();   //20091223
					Set_BurninModeFlag();
				}				
	          		hw_ClrDDC_WP();
			}
            /*
            #if DEBUG_ENABLE
		printMsg("KEY_FACTORY---->");
	     #endif
                Set_FactoryModeFlag();
                Clr_OsdLockModeFlag();
                Clr_ShowOsdLockFlag();
                KeypadButton = BTN_Power;
                */
            }
        #endif            
            else if (keypadStatus == KEY_PLUS)
            {   
            #if DEBUG_ENABLE
		printMsg("KEY_PLUS---->");
	     #endif

				
				#if 0//ScanKey//ModelName==AOC_G2261w || ModelName==AOC_2237Fw  || ModelName==AOC_2037F||ModelName==AOC_e2237Fw||ModelName==AOC_e2437Fw||ModelName==AOC_e2037F||ModelName==ENVISION_P971wl
				if (KeypadButton == BTN_Repeat || ((KeypadButton == BTN_Plus) && (MenuPageIndex == HotExpansionMenu)))
					KeypadButton = BTN_Repeat;
				#else
				if (KeypadButton == BTN_Repeat)
					KeypadButton = BTN_Repeat;
				#endif
				else
				{
					#if EnableVcomAdjustItem
					if(ProductModeFlag && (MenuPageIndex != FactoryMenu))
					{
						KeypadButton = BTN_VCOM;
					}
					else
					#endif
					KeypadButton = BTN_Plus;
				}
            }
            else if (keypadStatus == KEY_MINUS)
            {
                if (MenuPageIndex == RootMenu)
                {
                    if (KeypadButton == BTN_Minus || KeypadButton == BTN_Repeat)
                            KeypadButton = BTN_Repeat;
                    else
                        KeypadButton = BTN_Minus;
                    Delay1ms(100);
                }
                else
                    KeypadButton = BTN_Minus;
             //printMsg("press KEY_MINUS");
            }
            else if (keypadStatus == KEY_MENU)
            {

	#if 0//DEBUG_ENABLE
	printData("keypadStatus=%x", keypadStatus);
      #endif
            #if  1//!AllInOneKey
                if ((KeypadButton == BTN_Menu || KeypadButton == BTN_Repeat)
					#if AllInOneKey
					&&MenuPageIndex==RootMenu
					#endif
			)
                    KeypadButton = BTN_Repeat;
                else
	     #endif
	                KeypadButton = BTN_Menu;
          //printMsg("press KEY_MENU");
            }
            else if (keypadStatus == KEY_EXIT)
            {
            #if !AllInOneKey
                if ((KeypadButton == BTN_Exit || KeypadButton == BTN_Repeat)
					#if HotInputSelect
					&&MenuPageIndex != RootMenu
					#endif
					)
                    KeypadButton = BTN_Repeat;
                else
	     #endif
                {
                    #if 1//Dual
                        // use auto/exit hot key
                    #if  1 // (ModelName!=AOC_2016SW)
                        if (MenuPageIndex == RootMenu)
                        {
                            if (!PressExitFlag)
                            {
                                Set_PressExitFlag();
                                HotKeyCounter = 1; // 4;
                            }
                            KeypadButton =BTN_Exit;// BTN_Nothing;
                        }
                        else
		      #endif
                            KeypadButton = BTN_Exit;
                    #else
                        // user auto hot key
                        KeypadButton = BTN_Exit;
                    #endif
                }
         //printMsg("press KEY_EXIT");
            }
            else if (keypadStatus == KEY_SELECT)
            //2006-02-22
            {
                if (KeypadButton == BTN_Repeat||KeypadButton == BTN_Select)
                    KeypadButton = BTN_Repeat;
                else
                {
                        KeypadButton = BTN_Select;
                }
            }
    // 091019 coding, add condition for dc power off
    #if AllInOneKey
        #if ScanKey
            else if((keypadStatus&KEY_POWER) && (keypadStatus!=KEY_POWER))
            {
                if (KeypadButton == BTN_Power || KeypadButton == BTN_Repeat)
                {
                    KeypadButton = BTN_Repeat;
                    Set_DoKeyCountFlag();
                }
                else
                {
                    Clr_DoKeyCountFlag();
                    Set_PowerKeyFlag();
                    KeypadButton = BTN_Power;
                    Delay1ms(50);
                }
            }
        #endif
    #endif
            else if (keypadStatus == KEY_POWER)
            {

                #if AllInOneKey//(ModelName==AOC_2883)&&( MainBoardType == MainBoard_715G2883_A)

                if (KeypadButton == BTN_Power || KeypadButton == BTN_Repeat)
                {
                    KeypadButton = BTN_Repeat;
				#if !ScanKey
			if(!FactoryPowerOnFlag)
                    Set_DoKeyCountFlag();
		      Clr_FactoryPowerOnFlag();
			       #else
			 Set_DoKeyCountFlag();
				#endif
                }
                else
                {
                    Clr_DoKeyCountFlag();
                    Set_PowerKeyFlag();
                    KeypadButton = BTN_Power;
                    Delay1ms(50);
                }
                #else		

                #ifdef PowerKeyDeBounce
                PushPowerKeyFlag=1;                                 
                if (PushPowerKeyFlag && PushPowerKeyDeBounce < PowerKeyDeBounceCount)
                {
                    //printData("PushPowerKeyDeBounce=%d",PushPowerKeyDeBounce);
                    KeypadButton = BTN_Nothing;			//130111_1   Henry  add
                }   
                else
                #endif                    
                {
                    if (KeypadButton == BTN_Power || KeypadButton == BTN_Repeat)
                    {
                    	    KeypadButton = BTN_Repeat;
                    }
                    else		
                        KeypadButton = BTN_Power;
                }                    
		#endif
            }
            else
            {
                KeypadButton = BTN_Nothing;
                #ifdef PowerKeyDeBounce                
                PushPowerKeyFlag=0;
                PushPowerKeyDeBounce=0;
                #endif
            }                                
        }
    }
    else //check when key release
    {
        if (PressExitFlag)
        {
            if (MenuPageIndex == RootMenu)
            {
                #if Dual
                    // use source select hot key
                    KeypadButton = BTN_Exit;
                #else
                    // user exit hot key
                    KeypadButton = BTN_Select;
                #endif
                 HotKeyCounter = 0;
                Clr_EnableShowDDCFlag();
            }
        }

        if (PowerKeyFlag)
        {
            Set_MainOSDFlag();
	        KeypadButton = BTN_Power;
        }
        else
        {
            Clr_DoKeyCountFlag();
            PowerKeyCounter=0;
        }

        KeypadButton = BTN_Nothing;
        #ifdef PowerKeyDeBounce
        PushPowerKeyFlag=0;
        PushPowerKeyDeBounce=0;
        #endif
    }
}
