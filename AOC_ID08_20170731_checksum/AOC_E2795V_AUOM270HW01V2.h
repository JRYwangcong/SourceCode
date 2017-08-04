
#ifndef  _AOC_E2795V_AUOM270HW01V2_H_
#define  _AOC_E2795V_AUOM270HW01V2_H_

#define TSUMXXNX
#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3329_A2
#define PanelType 				PanelAUOM270HW01V2
#define DisplayLogo				AOCLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		1
	//	#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				0
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap//ASIA
#define INPUT_TYPE                        INPUT_1A1D
	//
#define DVI_RB_SWAP 		    	0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1

#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version的IC漸層和多國語言title文字置中


#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited	165 // scaler ADC operating range

//#define ReduceDDC
#define EnableNewHotKey

//#define ProgramKey // scan key could program key attribute
//#define LEDFlash

//#define OffPower
//#define DefaultIconExit
#define DCRClearBacklight                     1
#define BURST_MODE					1
#define UseVGACableReadWriteAllPortsEDID
#define InverseBrightness               	0

/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/

//#define USE_EON_FLASH				1
//#define USE_EON_FLASH_SAVE_FUNC	1
//#define SETTING_IN_FLASH_12K


#define Model 					"E2795V"
#define ShowModelName 			_E,_2,_7,_9,_5,_V
#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_2
#define Version					001
#define ChangeDate				_2,_0,_1,_2,_0,_4,_2,_4
#endif