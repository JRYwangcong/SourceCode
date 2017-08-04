	/*******************************************************************************/
#ifndef  _AOC_831S_
#define _AOC_831S_
#if PanelType == PanelCLAA185WA01
#define Model 					"831S+"
#elif PanelType == PanelM185B1L01
#define Model 					"831S"
#endif
#define ShowModelName 			_8,_3,_1,_S
#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_2
#define Version					002
#define ChangeDate				_2,_0,_0,_8,_0,_8,_0,_8//"20070921"
#define MainBoardType			MainBoard_715G2904_B
#define Dual	    					0 // for 2008osd 0
#define ENABLE_HDMI				0
#define HDCPKEY_IN_Flash		0
#define DisplayLogo				AOCLOGO//EnvisionLOGO//StaplesLOGO
//#define LANGUAGE_TYPE			ASIA//EUROPE//ID2007Standard//ASIA// for 2008osd
#define Enable_Cache		     1
#define Enable_OsdEngine1      0// 1: two OSD engine,; 0: one osd engine 
#define Enable_CascadeMode	    0//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

//#define USE_EON_FLASH				0
//#define USE_EON_FLASH_SAVE_FUNC	0

#define EnableNewHotKey
#define AllInOneKey				0
//#define PanelType 				PanelCLAA185WA01//PanelM185B1L01
#define WidePanel				1
#define Panel_Inv               		1 // ownbrand use 1
#define AudioFunc			    	0
#define DVI_RB_SWAP 		0
#define DVI_PN_SWAP		0
#define INPUT_TYPE                          INPUT_1A
#define MS_OVD                              0
#define DDCCI_ENABLE			1
#define Enable_Expansion		1
#define ENABLE_HDCP	 		0
#define ENABLE_NEW_HDCP_RULE	0
#define USB_Contral				0
//#define OffPower
#define DCRClearBacklight                     0
#endif

