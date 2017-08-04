#ifndef  _ENVISION_P971waL_CMI_
#define _ENVISION_P971waL_CMI_

#define MS_OVD                               0
#define MainBoardType			MainBoard_3834_18T
#define PanelType 				PanelCMIM185BGEL22_new
#define DisplayLogo				EnvisionLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					0
#define WidePanel				1
#define ENABLE_HDCP	 			0
#define ENABLE_NEW_HDCP_RULE	0
#define HDCPKEY_IN_Flash		0

#define TSUMXXT
	
#define AudioFunc				1
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap
#define INPUT_TYPE                        INPUT_1A
#define DVI_RB_SWAP 			0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1

#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version¦Ì?IC?u?¨®o¨ª?¨¤???Z??title??¡Á????D


#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited			165 // scaler ADC operating range

//#define ReduceDDC
#define DCRClearBacklight                     1

//ScanKey
#define PowerKeyDeBounce
#define PowerKeyDeBounceCount                   30

#define ProgramKey // scan key could program key attribute
//#define LEDFlash
#define InverseBrightness               	0
	/*******************HDMI****************/
#define ENABLE_HDMI					0

	/****************************************/
#define BURST_MODE                              1
#define Model 					"P971waL"
#define ShowModelName 			_P,_9,_7,_1,_w,_a,_L//petit 20130115
#define FWVersion				"002"
#define F_FWVersion                      _0,_0,_2
#define Version					2
#define ChangeDate				_2,_0,_1,_3,_0,_1,_1,_5
#endif


