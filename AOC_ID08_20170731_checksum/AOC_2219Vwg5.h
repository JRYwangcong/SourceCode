	/*******************************************************************************/
#ifndef  _AOC_2219Vwg5_
#define _AOC_2219Vwg5_

#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3289_C2
#define PanelType 				PanelTPM215HW01H1PS1
#define DisplayLogo				AOCLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1 // for 2008osd 0
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		1
	//	#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				0
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			ASIA
#define INPUT_TYPE                        INPUT_1A1D
	//
#define DVI_RB_SWAP 		0
#define DVI_PN_SWAP		0
#define Enable_Cache		     1
#define Enable_OsdEngine1      0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    1//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited	205 // scaler ADC operating range

//#define ReduceDDC
#define EnableNewHotKey
#define OffPower

#define Panel_miniLVDS // add VCOM adjusting item in factory menu
	/*******************HDMI****************/
#define ENABLE_HDMI					0

	/****************************************/

#define USE_EON_FLASH				1
#define USE_EON_FLASH_SAVE_FUNC	1
#define SETTING_IN_FLASH_12K


#define Model 					"2219Vwg5"
#define ShowModelName 			_2,_2,_1,_9,_V,_w,_g,_5
#define FWVersion				"004"
#define F_FWVersion                      _0,_0,_4
#define Version					004
#define ChangeDate				_2,_0,_0,_9,_0,_8,_1,_1
#endif

