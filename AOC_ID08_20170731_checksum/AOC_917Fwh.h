#define MS_OVD                          	0
#define MainBoardType			MainBoard_2810_B
#ifdef BGJ_TO_5PWGJ
#define PanelType 				PanelLTM190M2
#else
#define PanelType 				PanelLTM190M2
#endif
#define DisplayLogo				AOCLOGO//EnvisionLOGO//StaplesLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1 // for 2008osd 0
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		1
//#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				1
#define USB_Contral				0
#define AllInOneKey				1
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			ASIA
#define INPUT_TYPE                  	INPUT_1A1H

#define AOC_TSUM58BGJ	   		1
//#define BGJ_TO_5PWGJ		   	0

#define DVI_RB_SWAP 		 	1
#define DVI_PN_SWAP			1

#define Enable_Cache		     1
#define Enable_OsdEngine1      1// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    0//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

//#define USE_EON_FLASH				0
//#define USE_EON_FLASH_SAVE_FUNC	0
#define DCRClearBacklight                     0

/*******************HDMI****************/
#define ENABLE_HDMI				1

/****************************************/

#define Model 					"917Fwh"
#define ShowModelName 			_9,_1,_7,_F,_w,_h
#define FWVersion				"013"
#define F_FWVersion                	_0,_1,_3
#define ChangeDate				_2,_0,_0,_8,_0,_3,_1,_3//"20071008"
#define Version					13

