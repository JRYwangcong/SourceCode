#define MS_OVD                                  0
#define MainBoardType			MainBoard_2883_1B
//#define PanelType 				PanelLTM230HT01//PanelLM230WF1TLA3
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
#define AllInOneKey				1
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap
#define INPUT_TYPE                          INPUT_1A1D
	//
#define DVI_RB_SWAP 		0
#define DVI_PN_SWAP		0
#define Enable_Cache		     1
#define Enable_OsdEngine1      0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    1//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

//#define USE_EON_FLASH				0
//#define USE_EON_FLASH_SAVE_FUNC	0


	//#define ReduceDDC
#define DCRClearBacklight                     1
#define InverseBrightness               	0
	/*******************HDMI****************/
#define ENABLE_HDMI					0

	/****************************************/


#define Model 					"2330V"
#define ShowModelName 			_2,_3,_3,_0,_V
#if PanelType==PanelLTM230HT01
#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_2
#define ChangeDate				_2,_0,_1,_0,_0,_1,_2,_7
#define Version					002
#elif PanelType==PanelLM230WF1TLA3
#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_2
#define ChangeDate				_2,_0,_1,_0,_0,_1,_2,_7
#define Version					002
#endif









