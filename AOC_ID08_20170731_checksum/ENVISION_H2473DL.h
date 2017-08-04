#ifndef  _ENVISION_H2473DL_
#define _ENVISION_H2473DL_

#define TSUMXXNX
#define MS_OVD                               0
#define MainBoardType			MainBoard_4432
//#define PanelType 				PanelAUOM240HW01VD   //20130327
#define DisplayLogo				EnvisionLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					1
#define WidePanel				1
#define ENABLE_HDCP	 			1
#define ENABLE_NEW_HDCP_RULE  1
#define HDCPKEY_IN_Flash		1

#define AudioFunc				0
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap//ASIA
#define INPUT_TYPE                        INPUT_1A1D
	//
#define DVI_RB_SWAP 		    	0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1 //101227 Modify

#ifdef TSUMXXNX
#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version¦Ì?IC?u?¨®o¨ª?¨¤???Z??title??¡Á????D
#else
#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	1//support E or G Version¦Ì?IC?u?¨®o¨ª?¨¤???Z??title??¡Á????D
#endif

#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited	165 // scaler ADC operating range

#define DCRClearBacklight                     1
#define BURST_MODE					1
#define UseVGACableReadWriteAllPortsEDID
#define InverseBrightness               	0
/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/

#define Model 					"H2473DL"
#define ShowModelName 			_H,_2,_4,_7,_3,_D,_L
#if PanelType==PanelAUOM240HW01VD//20130327
#define FWVersion				"002"
#define F_FWVersion             _0,_0,_2
#define Version					002
#define ChangeDate				_2,_0,_1,_2,_1,_2,_0,_4
#elif PanelType==PanelTPM240WF1_HTN01//20130327
#define FWVersion				"001"
#define F_FWVersion             _0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_1,_3,_0,_4,_2,_4
#elif PanelType==PanelTPM240WF1_HW01//20130327
#define FWVersion				"003"
#define F_FWVersion             _0,_0,_3
#define Version					003
#define ChangeDate				_2,_0,_1,_3,_0,_7,_1,_9
#endif  
#endif  


