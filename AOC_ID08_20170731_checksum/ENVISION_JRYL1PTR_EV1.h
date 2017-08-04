#ifndef  _ENVISION_JRYL1PTR_EV1_
#define  _ENVISION_JRYL1PTR_EV1_

#define TSUMXXT
#define MS_OVD                               0
#define MainBoardType			MainBoard_JRYL1PTR_LF
#define PanelType 				PanelLM185TT3A//PanelAUOM215HW03//PanelM185WHB_N10//PanelM185BGE_L23//PanelAUOM215HW03//PanelNT156WHM_N12//PanelAUOM215HW03//PanelB156XTN07//PanelB156XTN02//PanelAUOM215HW03//PanelAUOM215HW03//PanelLGLP156WHB_DP////////PanelTPM200O1L02//
#define IT6251           				0//(PanelType==PanelB156XTN02||PanelType==PanelAUOM215HW03)//for eDP Panel
#define DisplayLogo				AcerLOGO//NoBrand//PuntaLOGO//WearnesLOGO//PuntaLOGO//EnvisionLOGO
#define DDCCI_ENABLE			1//0
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

#define   BlacklitTimeOnShow               0

#define PANELSSCSTEPLFUN                   0

#define   HKC_ColorTemp_Adjust             0   //色温自动调节

#define   FactoryAuto_5400K                   (1&&HKC_ColorTemp_Adjust)

//#define  ReduceDDC   ////OPEN 内置
#define  FDDCCI_WriteEDID		    		0

#define Enable_External_EDID          		0

#define VGA_EDID_Write_To_24C02		(0&&Enable_External_EDID)

#define findmodebyVwidth                    1
// Auto color item in menu enable define; 	// JueryWu 100909;
#define	AUTO_COLOR_ITEM_EN		1	// 1: enable; 0: Disable;

// 菜单显示样式定义;
#define	AOC_MENU_FRAME_MODE		1	// 0: 菜单上/ 下有两条边;  // 1: 菜单下无边;

// Enable 4:3/16:9 scale function at all resolution;
#define	ALL_RES_SCALE_FUNC_EN	1	// 1: All resolution can be scaled to 4:3 at wide panel;

// Enable osd lock function;
#define	OSD_LOCK_FUNCTION_EN		1	// 1:Disable  0:enable

// Enable picture boost  function;
#define	PICTUREBOOST_FUN_EN		0	// 1:Disable  0:enable

#define DCRClearBacklight                    1//0
#define InverseBrightness               	0///1
#define BURST_MODE                           0
/*******************HDMI****************/
#define ENABLE_HDMI					0
/****************************************/

#define LM185TT3A_240MA				1

#if (PanelType == PanelM185BGE_L23)
#define Model 					"JRY185EDP"
#define ShowModelName 			_J,_R,_Y,_1,_8,_5,_E,_D,_P
#define FWVersion				"001"
#define F_FWVersion             		_0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_1,_7,_0,_2,_2,_0
#elif (PanelType == PanelM185WHB_N10||PanelType == PanelLM185TT3A)
#define Model 					"JRY185"
#define ShowModelName 			_J,_R,_Y,_1,_8,_5
#define FWVersion				"001"
#define F_FWVersion             		_0,_0,_1
#define Version					001
#define ChangeDate				_2,_0,_1,_7,_0,_8,_0,_3
#else
#define Model 					"JRY156EDP"
#define ShowModelName 			_J,_R,_Y,_1,_5,_6,_E,_D,_P
#define FWVersion				"001"
#define F_FWVersion             		_0,_0,_2
#define Version					002
#define ChangeDate				_2,_0,_1,_7,_0,_2,_0,_4
#endif
#endif

