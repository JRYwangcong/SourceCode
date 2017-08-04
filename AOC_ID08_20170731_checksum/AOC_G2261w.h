	/*******************************************************************************/
#ifndef  _AOC_G2261w_
#define _AOC_G2261w_

#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3289_C2
#define PanelType 				PanelTPM215HW01//PanelTPM215HW01H1PS1 090623 coding
#define DisplayLogo				EnvisionLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1 // for 2008osd 0
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		1
	//	#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				0///1
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

#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			0// 1->Flash 	0->EEPROM

#define ADCLimited	205 // scaler ADC operating range

//#define ReduceDDC
#define DCRClearBacklight                     1

#define Panel_RSDS // add VCOM adjusting item in factory menu

#define ProgramKey // scan key could program key attribute
#define EnableSlider // scan key with slider

#define PowerKeyDeBounce
#define PowerKeyDeBounceCount                   20 // unit: 10ms
#define InverseBrightness               	1
/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/

//#define USE_EON_FLASH				1
//#define USE_EON_FLASH_SAVE_FUNC	1
//#define SETTING_IN_FLASH_12K


#define Model 					"G2261w"
#define ShowModelName 			_G,_2,_2,_6,_1,_w
#define FWVersion				"003"
#define F_FWVersion                      _0,_0,_3
#define Version					003
#define ChangeDate				_2,_0,_0,_9,_0,_8,_1,_0
#endif

