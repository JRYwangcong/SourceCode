#ifndef  _ENVISION_P971WALS_H_
#define  _ENVISION_P971WALS_H_

#define TSUMXXT
#define MS_OVD                               0
#define MainBoardType			MainBoard_3834_18T
#define PanelType 				PanelTPM185B1_WX3400  
#define DisplayLogo				EnvisionLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1         //Image Ratio
#define Dual	    					0
#define WidePanel				1
#define ENABLE_HDCP	 			0
#define ENABLE_NEW_HDCP_RULE	1
#define HDCPKEY_IN_Flash		0

#define AudioFunc				1
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1   //review here
#define LANGUAGE_TYPE			All14Language_AddJap
#define INPUT_TYPE                        INPUT_1A
	//
#define DVI_RB_SWAP 			0           //review here                    
#define DVI_PN_SWAP			0           //review here
#define Enable_Cache		     		1   //review here

#define Enable_OsdEngine1      	0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    	0//support E or G Version的IC漸層和多國語言title文字置中

#define EnablePanelServiceMenu		1//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited	205 // scaler ADC operating range

//#define ReduceDDC


#if 1//ScanKey
#define PowerKeyDeBounce
#define PowerKeyDeBounceCount                   30
#endif
/* Peter.Yu */
#define PowerSavingAmberLEDFlashing // 090701
#define GreenLEDDutyCtrl
#define Type_OnOff500mS          0
#define Type_Breathing              1
#define PSAmberLEDFlashingType     Type_Breathing
#if PSAmberLEDFlashingType == Type_Breathing
    #define LED_PWM_MAX     0x80
    #define LED_PWM_MIN     0xff
#endif
/* end of modify */

#define ProgramKey // scan key could program key attribute
#define BURST_MODE                              1
#define DCRClearBacklight                     1
#define UseVGACableReadWriteAllPortsEDID
#define InverseBrightness               	0
	/*******************HDMI****************/
#define ENABLE_HDMI					0

	/****************************************/



#define Model 					"P971WALS"
#define ShowModelName 			_P,_9,_7,_1,_W,_A,_L,_S
#define FWVersion				"003"
#define F_FWVersion                      _0,_0,_3
#define Version					003
#define ChangeDate				_2,_0,_1,_3,_0,_4,_1,_1
#endif

