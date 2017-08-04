/*******************************************************************************/
#ifndef  _AOC_Q156W_
#define _AOC_Q156W_
#define MainBoardType			MainBoard_715G2904_B
#define Dual	    					0 // for 2008osd 0
#define ENABLE_HDMI				0
#define HDCPKEY_IN_Flash		0
#define DisplayLogo				AOCLOGO//EnvisionLOGO//StaplesLOGO
//#define LANGUAGE_TYPE			ASIA//EUROPE//ID2007Standard//ASIA// for 2008osd
#define Enable_Cache		     1
#define Enable_OsdEngine1      0// 1: two OSD engine,; 0: one osd engine 
#define Enable_CascadeMode	    0//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

//#define USE_EON_FLASH				0
//#define USE_EON_FLASH_SAVE_FUNC	0
#define DCRClearBacklight                     0

#define AllInOneKey				0
//#define PanelType 				PanelM156B1L01
#define WidePanel				1
#define Panel_Inv               		1 // ownbrand use 1
#define AudioFunc			    	1//0
#define DVI_RB_SWAP 		0
#define DVI_PN_SWAP		0
#define INPUT_TYPE                          INPUT_1A
#define MS_OVD                              0
#define DDCCI_ENABLE			1
#define Enable_Expansion		1
#define ENABLE_HDCP	 		0
#define ENABLE_NEW_HDCP_RULE	0
#define USB_Contral				0
#define Model 					"1619Swa"
#define ShowModelName 			_1,_6,_1,_9,_S,_w,_a
#if PanelType==PanelM156B1L01
#define FWVersion				"005"
#define F_FWVersion                      _0,_0,_5
#define Version					005
#define ChangeDate				_2,_0,_0,_8,_0,_3,_2,_8//"20070921"
#elif PanelType==PanelSVA156WX1

#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_2
#define Version					002
#define ChangeDate				_2,_0,_0,_8,_0,_6,_3,_0
#endif
#endif

