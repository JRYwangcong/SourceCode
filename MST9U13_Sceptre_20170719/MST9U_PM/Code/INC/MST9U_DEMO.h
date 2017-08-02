#ifndef MST9U_DEMO_H
#define MST9U_DEMO_H
////////////////////////////////////////////////////////////
// BOARD SELECTION
////////////////////////////////////////////////////////////
#define CHIP_FAMILY_TYPE     CHIP_FAMILY_MST9U
#define MS_BOARD_TYPE_SEL   BD_MST9UHL_A0
//#define FPGA   //20140218 enable this define to verify 51 with FPGA system
////////////////////////////////////////////////////////////
// INPUT_TYPE SELECTION
////////////////////////////////////////////////////////////
#if MS_BOARD_TYPE_SEL == BD_MST9U
#define INPUT_TYPE                      (INPUT_1A|INPUT_5C)
#define ENABLE_VGA_INPUT        1
#define ENABLE_DVI_INPUT        0
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    1
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI_1_4           1
#define ENABLE_MHL_C1             0// 1
#define ENABLE_MHL_C2             0// 1
#define ENABLE_MHL_C3             0// 1
#define ENABLE_MHL                1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_INPUT           1//((Input_Displayport_C2 != Input_Nothing)||(Input_Displayport_C3 != Input_Nothing))
#define ENABLE_DP2ND_INPUT        0
#define ENABLE_DP3ND_INPUT        0
#define ENABLE_LED_CONTROLLER     0
#define DISABLE_AUTO_SWITCH       0

#elif MS_BOARD_TYPE_SEL == BD_MST9UHL_A0
#define INPUT_TYPE                      (INPUT_1A|INPUT_3C)
#define ENABLE_VGA_INPUT        1
#define ENABLE_DVI_INPUT         1
#define ENABLE_DVI2ND_INPUT     0
#define ENABLE_DVI3ND_INPUT     0
#define ENABLE_HDMI_INPUT       1
#define ENABLE_HDMI2ND_INPUT    0
#define ENABLE_HDMI3ND_INPUT    0
#define ENABLE_HDMI_1_4           1
#define ENABLE_MHL_C1             0// 1
#define ENABLE_MHL_C2             0// 1
#define ENABLE_MHL_C3             0// 1
#define ENABLE_MHL                1 //  (ENABLE_MHL_C1|ENABLE_MHL_C2|ENABLE_MHL_C3)
#define ENABLE_DP_INPUT           1//((Input_Displayport_C2 != Input_Nothing)||(Input_Displayport_C3 != Input_Nothing))
#define ENABLE_DP2ND_INPUT        0
#define ENABLE_DP3ND_INPUT        0
#define ENABLE_LED_CONTROLLER     0
#define DISABLE_AUTO_SWITCH       0

#else
#message "please define MST9U new board type for Port_Func_Enable"
#endif
////////////////////////////////////////////////////////////
// PANEL SELECTION
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// BRIGHTNESS CONTROL
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// HDMI
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// AUDIO
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// OSD
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// SYSTEM
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// POWERMANAGEMENT
////////////////////////////////////////////////////////////
#define ENABLE_VBY1_TO_EDP	0

#if ENABLE_VBY1_TO_EDP
#define Vby1ToEDP_1_15_PowerOn() 	(MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, 1, BIT7))	// Set input mode or set Hi: On 
#define Vby1ToEDP_1_15_PowerOff()   (MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, 0, BIT7), MEM_MSWRITE_BIT(_REG_GPIO6_OUT, 0, BIT7))	// Lo: Off
#define Vby1ToEDP_3_3_PowerOn() 	do {\																					
											MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, 1, BIT6);\ 
											ForceDelay1ms(5);\
										} while(0)
#define Vby1ToEDP_3_3_PowerOff()   	(MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, 0, BIT6), MEM_MSWRITE_BIT(_REG_GPIO6_OUT, 1, BIT6))	// Hi: Off
#else
#define Vby1ToEDP_1_15_PowerOn()	
#define Vby1ToEDP_1_15_PowerOff()  
#define Vby1ToEDP_3_3_PowerOn()	
#define Vby1ToEDP_3_3_PowerOff()
#endif

#define DISABLE_R2CORE_POWER() 		do {\
                                       g_bCorePowerStatus=0;\
                                       msWriteByteMask(REG_000420, 0 ,BIT2);\
                                       msWriteByteMask(REG_000422, 0 ,BIT2);\
                                       Vby1ToEDP_1_15_PowerOff();\
                                       Vby1ToEDP_3_3_PowerOff();\
                                  	} while(0)

#define ENABLE_R2CORE_POWER()    	do {\
	                                   g_bCorePowerStatus=1;\
	                                   msWriteByteMask(REG_000422, BIT2 ,BIT2);\
	                                   msWriteByteMask(REG_000420, BIT2 ,BIT2);\
	                                   Vby1ToEDP_3_3_PowerOn();\
	                                   Vby1ToEDP_1_15_PowerOn();\
	                              	} while(0)

////////////////////////////////////////////////////////////
// INFORMATION
////////////////////////////////////////////////////////////
#define ChipName            "TSUMD"
#define Model               "XXXXXXXXX"
#define FWVersion           "000"
#define ChangeDate          "20120612"
///////////////////////////////////////////////////////////////
// SYSTEM DEBUG
//////////////////////////////////////////////////////////////
#define ENABLE_DEBUG                   0

#define R2_51_DUAL_MODE            1 // need sync with R2 setting
#define ENABLE_MBX                 0 // need sync with R2 setting
#define ENABLE_CABLE_DET           0
#define ENABLE_DRAM_SELFREFRESH    0
#define ENABLE_DAISY_CHAIN		   0 // need sync with R2 setting
#define ENABLE_DC_SDM_FORWARD      (0 && ENABLE_DAISY_CHAIN)	// need sync with R2 setting
#endif

