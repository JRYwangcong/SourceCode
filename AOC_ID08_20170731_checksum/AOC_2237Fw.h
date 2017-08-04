	/*******************************************************************************/
#ifndef  _AOC_2237Fw_
#define _AOC_2237Fw_

#define AOC37Series

#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3598_M0A 
#define PanelType 				PanelAUOM215HW01V7
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
#define AllInOneKey				1
#define Panel_Inv               		1 // ownbrand use 1
//#define LANGUAGE_TYPE			ASIA
#define LANGUAGE_TYPE			All14Language_AddJap
#define INPUT_TYPE                        INPUT_1A1D
	//
#define DVI_RB_SWAP 		    	0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1
#ifdef TSUMXXNX
#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version的IC漸層和多國語言title文字置中
#else
#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	1//support E or G Version的IC漸層和多國語言title文字置中
#endif

#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			0// 1->Flash 	0->EEPROM

#define ADCLimited	205 // scaler ADC operating range

//#define ReduceDDC
#define LEDBrightCtrlByScalerPWM
#define EnableLightBar

#define ProgramKey // scan key could program key attribute
//#define LEDFlash

#define DCRClearBacklight                     1
#define InverseBrightness               	0
/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/

//#define USE_EON_FLASH				1
//#define USE_EON_FLASH_SAVE_FUNC	1
//#define SETTING_IN_FLASH_12K


#define Model 					"2237Fw"
#define ShowModelName 			_2,_2,_3,_7,_F,_w
#define FWVersion				"007"
#define F_FWVersion                      _0,_0,_7
#define Version					007
#define ChangeDate				_2,_0,_0,_9,_1,_2,_1,_0
#endif

