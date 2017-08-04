#ifndef  _ENVISION_R2018WS_
#define  _ENVISION_R2018WS_

#define TSUMXXT
#define MS_OVD                               0
#define MainBoardType			MainBoard_SLB1PTR
#define PanelType 				PanelHSM195EU_RTN01//PanelM185B3LA1//////PanelAUOM215HW03//////PanelTPM200O1L02//
#define DisplayLogo				EnvisionLOGO
#define DDCCI_ENABLE			1
#define Enable_Expansion			1
#define Dual	    					0
#define WidePanel				1
#define ENABLE_HDCP	 			0
#define ENABLE_NEW_HDCP_RULE	0
#define HDCPKEY_IN_Flash		0

#define AudioFunc				0
#define USB_Contral				0
#define AllInOneKey				0
#define Panel_Inv               		1 // ownbrand use 1
#define LANGUAGE_TYPE			All14Language_AddJap//ASIA
#define INPUT_TYPE                        INPUT_1A
	
#define DVI_RB_SWAP 		    	0
#define DVI_PN_SWAP			0
#define Enable_Cache		     		1

#define Enable_OsdEngine1      		0// 1: two OSD engine,; 0: one osd engine
#define Enable_CascadeMode	    		0//support E or G Versionμ?IC?u?óoí?à???Z??title??×????D

#define EnablePanelServiceMenu		0//To Save Panel Setting to EEPROM or Flash 
#define PanelSettingInFlash			1// 1->Flash 	0->EEPROM

#define ADCLimited					165 // scaler ADC operating range

#define	DISPLAY_CHECK_VOL	                1	

#define   BlacklitTimeOnShow               1

#define PANELSSCSTEPLFUN                    1

#define   HKC_ColorTemp_Adjust             1   //色温自动调节

#define   FactoryAuto_5400K                   (1&&HKC_ColorTemp_Adjust)

#define  FDDCCI_WriteEDID		    		1

#define Enable_External_EDID          		1

#define VGA_EDID_Write_To_24C02		(1&&Enable_External_EDID)

#define F_ResetCheckEdidManueAndSN		1

#define findmodebyVwidth                    1

#define DCRClearBacklight                    1//0
#define InverseBrightness               	1
#define BURST_MODE                           1
/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/
#define Model 					"R2018WS"
#define ShowModelName 			_R,_2,_0,_1,_8,_W,_S
#define FWVersion				"001"
#define F_FWVersion             		_0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_1,_5,_1,_2,_1,_9

#endif

