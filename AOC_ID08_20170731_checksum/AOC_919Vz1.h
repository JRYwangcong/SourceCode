#ifndef  _AOC_919Vz1_
#define _AOC_919Vz1_

#define TSUMXXQX

#define MS_OVD                               1
#define MainBoardType			MainBoard_715G3329_1_2
#define PanelType 				PanelLM190E08
#define DisplayLogo				AOCLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					0
#define WidePanel				0
#define ENABLE_HDCP	 			0
#define ENABLE_NEW_HDCP_RULE	0
#define HDCPKEY_IN_Flash		0
	//	#define HDCP_IN_FLASH_NEW_WAY
#define InverseVolume//add by win                     
#define AudioFunc				1
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap//ASIA
#define INPUT_TYPE                        INPUT_1A
	//
#define DVI_RB_SWAP 		    	0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1

#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version��?IC?u?��o��?��???Z??title??��????D


#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited	165 // scaler ADC operating range

//#define ReduceDDC

//#define ProgramKey // scan key could program key attribute
//#define LEDFlash

#define DCRClearBacklight                     1
#define BURST_MODE					0
//#define UseVGACableReadWriteAllPortsEDID
#define InverseBrightness               	0
/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/

//#define USE_EON_FLASH				1
//#define USE_EON_FLASH_SAVE_FUNC	1
//#define SETTING_IN_FLASH_12K


#define Model 					"919SZ"
#define ShowModelName 			_9,_1,_9,_S,_Z
#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_1,_1,_1,_0,_2,_6
#endif

