#ifndef  _ENVISON_G2218_PLUS_
#define _ENVISON_G2218_PLUS_
#define TSUMXXNX
#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3834
#define PanelType 				PanelLTM220MT05//PanelLTM220M1L01
#define DisplayLogo				EnvisionLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		1
	//	#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				0
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap//ASIA
#define INPUT_TYPE                        INPUT_1A1D
	//
#define DVI_RB_SWAP 		    	0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1

#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version的IC漸層和多國語言title文字置中


#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited	165 // scaler ADC operating range

//#define ReduceDDC

//#define ProgramKey // scan key could program key attribute
//#define LEDFlash

#define DCRClearBacklight                     1
#define BURST_MODE					1
//#define UseVGACableReadWriteAllPortsEDID
#define InverseBrightness               	0
/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/

//#define USE_EON_FLASH				1
//#define USE_EON_FLASH_SAVE_FUNC	1
//#define SETTING_IN_FLASH_12K


#define Model 					"ENVISION_G2218_PLUS"
#define ShowModelName 			_G,_2,_2,_1,_8,_Plus	// 111114 Modify
#define FWVersion				"001"
#define F_FWVersion                      _0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_1,_1,_1,_0,_3,_1
#endif


