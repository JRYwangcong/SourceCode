#ifndef  _AOC_1620Swa_
#define _AOC_1620Swa_

#define MS_OVD                               0
#define MainBoardType			MainBoard_715G2904_B
#define PanelType 				PanelAUOM156XW01V0
#define DisplayLogo				AOCLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					0
#define WidePanel				1
#define ENABLE_HDCP	 			0
#define ENABLE_NEW_HDCP_RULE	0
#define HDCPKEY_IN_Flash		0
	//	#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				1
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap//ASIA
#define INPUT_TYPE                        INPUT_1A
	//
#define DVI_RB_SWAP 		    	0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1

#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version的IC漸層和多國語言title文字置中


#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited	165 // scaler ADC operating range

//#define ReduceDDC

#define ProgramKey // scan key could program key attribute
//#define LEDFlash

#define DCRClearBacklight                     0
#define InverseBrightness               	1

/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/

//#define USE_EON_FLASH				0
//#define USE_EON_FLASH_SAVE_FUNC	0



#define Model 					"1620Swa"
#define ShowModelName 			_1,_6,_2,_0,_S,_w,_a
#define FWVersion				"001"
#define F_FWVersion                      _0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_1,_0,_0,_1,_2,_1
#endif

