#ifndef  _ENVISION_P2071L_T_
#define _ENVISION_P2071L_T_
#define TSUMXXT
#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3959_T
//#define PanelType 				//PanelTPM200O1L02C1D//PanelTPM200O1L02
#define DisplayLogo				EnvisionLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion		1
#define Dual	    			0
#define WidePanel				1
#define ENABLE_HDCP	 			0
#define ENABLE_NEW_HDCP_RULE	0
#define HDCPKEY_IN_Flash		0
	
//#define PanelSwapPolarity       0xFF

#define AudioFunc				0
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv            	1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap
#define INPUT_TYPE             	INPUT_1A
	//
#define DVI_RB_SWAP 			0
#define DVI_PN_SWAP				0
#define Enable_Cache		   	1

#define Enable_OsdEngine1      		0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited					165 // scaler ADC operating range

//#define ReduceDDC
#define DCRClearBacklight         	1 
#define BURST_MODE              1

#if 1//ScanKey		//110623 Modify
#define PowerKeyDeBounce
#define PowerKeyDeBounceCount     	30
#endif

//=================================
//110623 Modify
#define PowerSavingAmberLEDFlashing
#define GreenLEDDutyCtrl
#define Type_OnOff500mS          	0
#define Type_Breathing              1
#define PSAmberLEDFlashingType     Type_Breathing
#if PSAmberLEDFlashingType == Type_Breathing
    #define LED_PWM_MAX     0x80
    #define LED_PWM_MIN     0xff
#endif
//=================================

//#define PowerSavingAmberLEDFlash // 090701

#define ProgramKey // scan key could program key attribute
//#define LEDFlash
#define InverseBrightness               	0
	/*******************HDMI****************/
#define ENABLE_HDMI					0

	/****************************************/
#if PanelType==PanelLTM200KT01
#define Model 					"P2071L"
#define ShowModelName 			_P,_2,_0,_7,_1,_L
#define FWVersion				"003"
#define F_FWVersion                      _0,_0,_3
#define Version					003
#define ChangeDate				_2,_0,_1,_1,_0,_7,_1,_8
#endif

#endif

