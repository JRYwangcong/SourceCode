	/*******************************************************************************/
#ifndef  _AOC_2219Vg_
#define _AOC_2219Vg_

#define MS_OVD                                  0
#define MainBoardType			MainBoard_2883_1B
//#define PanelType 				PanelLTM220M1L01
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
#define INPUT_TYPE                          INPUT_1A1D
	//
#define DVI_RB_SWAP 		0
#define DVI_PN_SWAP		0
#define Enable_Cache		     1
#define Enable_OsdEngine1      0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    1//support E or G Version的IC漸層和多國語言title文字置中

#if PanelType==PanelLTM220M1L01
#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#elif PanelType==PanelLM220WE1
#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#endif
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define USE_EON_FLASH				0
#define USE_EON_FLASH_SAVE_FUNC	0


//#define ReduceDDC
//#define EnableNewHotKey

	/*******************HDMI****************/
#define ENABLE_HDMI					0

	/****************************************/



#define Model 					"2219Vg"
#define ShowModelName 			_2,_2,_1,_9,_V,_g
#if PanelType==PanelLTM220M1L01
#define FWVersion				"003"
#define F_FWVersion                      _0,_0,_3
#define Version					003
#define ChangeDate				_2,_0,_0,_8,_1,_2,_1,_1
#elif PanelType==PanelLM220WE1
#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_2
#define Version					002
#define ChangeDate				_2,_0,_0,_8,_1,_2,_2,_6
#endif
#endif

