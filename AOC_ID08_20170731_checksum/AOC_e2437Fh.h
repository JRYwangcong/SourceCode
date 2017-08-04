	/*******************************************************************************/
#ifndef  _AOC_E2437Fh_
#define _AOC_E2437Fh_

#define AOC37Series
#define TSUMXXMX	//120907 Modify

#define MS_OVD                               0
#define MainBoardType			MainBoard_715G3603_M0A
#define PanelType 				PanelAUOM240HW01VB
#define DisplayLogo				AOCLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1 // for 2008osd 0
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		1
	//	#define HDCP_IN_FLASH_NEW_WAY

#define AudioFunc				1
#define USB_Contral				0
#define AllInOneKey				1
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap
#define INPUT_TYPE                        INPUT_1A2D

#define NewInterlaceShiftLineRule       ///yuanzhi20100424_1 for HDMI Interlacemode ShiftLine 
	//
#define DVI_RB_SWAP 		0
#define DVI_PN_SWAP		0
#define Enable_Cache		     1
#define Enable_OsdEngine1      0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    1//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		1//0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1//0// 1->Flash 	0->EEPROM

#define ADCLimited	205 // scaler ADC operating range

//#define ReduceDDC
#define LEDBrightCtrlByScalerPWM
#define EnableLightBar

#define ProgramKey // scan key could program key attribute
//#define LEDFlash

#define DCRClearBacklight                     1
#define BURST_MODE				1
//#define UseVGACableReadWriteAllPortsEDID
#define InverseBrightness               	0
/*******************HDMI****************/
#define ENABLE_HDMI					1
/****************************************/



#define Model 					"E2437Fh"
#define ShowModelName 			_E,_2,_4,_3,_7,_F,_h
#define FWVersion				"007"
#define F_FWVersion                      _0,_0,_7
#define Version					007
#define ChangeDate				_2,_0,_1,_0,_0,_9,_1,_7
#endif

