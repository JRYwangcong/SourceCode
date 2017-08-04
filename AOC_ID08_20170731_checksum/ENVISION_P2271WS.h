#ifndef  _ENVISION_P2271WS_
#define _ENVISION_P2271WS_

#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3329_A2//MainBoard_715G3329_58N//MainBoard_715G3329_1_2
//#define PanelType 				PanelLM215WF4//PanelTPM215H1L01
#define DisplayLogo				EnvisionLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		1

#define TSUMXXNX         // Rosa Chip//101130
	//	#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				0
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap
#define INPUT_TYPE                        INPUT_1A1D
	//
#define DVI_RB_SWAP 			0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1
#ifdef TSUMXXNX
#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version¦Ì?IC?u?¨®o¨ª?¨¤???Z??title??¡Á????D
#else
#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	1//support E or G Version¦Ì?IC?u?¨®o¨ª?¨¤???Z??title??¡Á????D
#endif

#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited	205 // scaler ADC operating range

//#define ReduceDDC
#define DCRClearBacklight                     1

#if 1//ScanKey
#define PowerKeyDeBounce
#define PowerKeyDeBounceCount                   30
#endif

#define UseVGACableReadWriteAllPortsEDID

#define PowerSavingAmberLEDFlashing // 090701
#define GreenLEDDutyCtrl
#define Type_OnOff500mS          0
#define Type_Breathing              1
#define PSAmberLEDFlashingType     Type_Breathing
#if PSAmberLEDFlashingType == Type_Breathing
    #define LED_PWM_MAX     0x80
    #define LED_PWM_MIN     0xff
#endif


#define ProgramKey // scan key could program key attribute
//#define LEDFlash
#define InverseBrightness               	0
	/*******************HDMI****************/
#define ENABLE_HDMI					0

	/****************************************/
#define BURST_MODE              1
//#define USE_EON_FLASH				1
//#define USE_EON_FLASH_SAVE_FUNC	1
//#define USE_EON_FLASH_HDCP_EDID_4K_SAVE_FUNC 	0
//#define SETTING_IN_FLASH_12K

#if PanelType==PanelAUOM215HW01VC
#define Model 					"P2271WS"
#define ShowModelName 			_P,_2,_2,_7,_1,_W,_S
#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_2
#define Version					002
#define ChangeDate				_2,_0,_1,_1,_0,_5,_1,_0
#endif
#if PanelType==PanelAUOM215HW03
#define Model 					"P2271WS"
#define ShowModelName 			_P,_2,_2,_7,_1,_W,_S
#define FWVersion				"001"
#define F_FWVersion                      _0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_1,_1,_0,_9,_2,_6
#endif

#endif

