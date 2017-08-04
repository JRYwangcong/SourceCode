	/*******************************************************************************/
#ifndef  _AOC_2238Vh_
#define _AOC_2238Vh_

#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3663_M0A
#define PanelType 				PanelCMOM215H1L01
#define DisplayLogo				AOCLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1 // for 2008osd 0
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		1
	//	#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				1
#define USB_Contral				0
#define AllInOneKey				1
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			AllLanguage//ASIA
#define INPUT_TYPE                        INPUT_1A1H
	//
#define DVI_RB_SWAP 		1
#define DVI_PN_SWAP		1
#define Enable_Cache		     1
#define Enable_OsdEngine1      0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    1//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			0// 1->Flash 	0->EEPROM

#define ADCLimited	205 // scaler ADC operating range

#define InverseVolume

//#define ReduceDDC
#define EnableNewHotKey
#define DCRClearBacklight                     0
//#define OffPower

	/*******************HDMI****************/
#define ENABLE_HDMI					1

	/****************************************/

//#define USE_EON_FLASH				1
//#define USE_EON_FLASH_SAVE_FUNC	1
//#define SETTING_IN_FLASH_12K


#define Model 					"2238Vh"
#define ShowModelName 			_2,_2,_3,_8,_V,_h
#define FWVersion				"001"
#define F_FWVersion                      _0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_0,_9,_0,_6,_0,_8
#endif

