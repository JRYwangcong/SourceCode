
	/******************************************************************************
	AOC_2564wx.h
	[Date]:        08-NOV-2007
	[Comment]:
	AOC_2564 header file..
	[Reversion History]:
	*******************************************************************************/
#ifndef  _AOC_731FW_
#define _AOC_731FW_
#define Model				"731FW"
#define ShowModelName 		_7,_3,_1,_F,_W
#define FWVersion			"011"
#define F_FWVersion			_0,_1,_1
#define Version				11
#define ChangeDate			_2,_0,_0,_8,_0,_8,_0,_5//"20070921"
#define MainBoardType		MainBoard_715G2575  
#define Dual					0 // for 2008osd 0
#define ENABLE_HDMI			0
#define HDCPKEY_IN_Flash	0
#define DisplayLogo			AOCLOGO//EnvisionLOGO//StaplesLOGO
#define LANGUAGE_TYPE		ASIA//EUROPE//ID2007Standard//ASIA// for 2008osd
#define Enable_Cache			1
#define Enable_OsdEngine1	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    0//support E or G Version的IC漸層和多國語言title文字置中


#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

//#define USE_EON_FLASH				0
//#define USE_EON_FLASH_SAVE_FUNC	0


#define AllInOneKey			0
#define PanelType			PanelCLAA170WA02
#define WidePanel			1
#define Panel_Inv			1 // ownbrand use 1
#define AudioFunc			0 // 0
#define DVI_RB_SWAP 		0
#define DVI_PN_SWAP		0
#define INPUT_TYPE			INPUT_1A
#define MS_OVD				0
#define DDCCI_ENABLE		1
#define Enable_Expansion		1
#define ENABLE_HDCP	 		0
#define ENABLE_NEW_HDCP_RULE	0
#define USB_Contral				0
#define DCRClearBacklight                     0
#endif

