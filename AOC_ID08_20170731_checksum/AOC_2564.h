
/******************************************************************************
AOC_2564wx.h
[Date]:        08-NOV-2007
[Comment]:
AOC_2564 header file..
[Reversion History]:
*******************************************************************************/
#ifndef  _AOC_2564_
#define _AOC_2564_
// Module Name
#define Model 					"AOC_2564"
#define ShowModelName 			_2,_5,_6,_4
//Version
#define FWVersion				"003"
#define F_FWVersion                      _0,_0,_3
#define Version					003
//Date
#define ChangeDate				_2,_0,_0,_7,_1,_1,_1,_6//"20070921"

//MainBoard
#define MainBoardType			MainBoard_715G2564_B
//Input
#define Dual	    					1 // for 2008osd 0
#define ENABLE_HDMI				0
//DDC
//#define DDC_In_Flash				1
//EEPRom
//  #define EEPRom_24C04			0

//flash
//	#define  SST						0
//	#define  PMC						0
#define HDCPKEY_IN_Flash		0
//	 #define Data_IN_Flash				1

//Display  OSD
#define DisplayLogo				AOCLOGO//EnvisionLOGO//StaplesLOGO
#define LANGUAGE_TYPE			ASIA//EUROPE//ID2007Standard//ASIA// for 2008osd
//ADCkey
#define AllInOneKey				0
//panel
#define PanelType 				PanelCMO190
#define WidePanel				0
#define Panel_Inv               		1 // ownbrand use 1
//Audio
#define AudioFunc			    	1 // 0
//other

//
#define DVI_RB_SWAP 		0
#define DVI_PN_SWAP		0
///

#define Enable_Cache		     1
#define Enable_OsdEngine1      0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    0//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

//#define USE_EON_FLASH				0
//#define USE_EON_FLASH_SAVE_FUNC	0


#define INPUT_TYPE                          INPUT_1A1D

#define MS_OVD                              0
#define DDCCI_ENABLE			1
#define Enable_Expansion		1
#define ENABLE_HDCP	 		0
#define ENABLE_NEW_HDCP_RULE	0
#define USB_Contral				0
#define DCRClearBacklight                     0
#endif

