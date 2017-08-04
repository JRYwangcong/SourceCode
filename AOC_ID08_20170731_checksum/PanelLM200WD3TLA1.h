#define PanelType_1600x900
#define PanelName               _L,_M,_2,_0,_0,_W,_D,_3,_T,_L,_A,_1

#define PanelDither		8

#define PanelTTL		0
#define PanelTCON		0
#define PanelLVDS		0xFF
#define PanelRSDS		0
#define PanelminiLVDS      0

#define LVDS_TIMode		0xff
#define PanelDualPort		0xFF
#define PanelSwapPort		0xFF
#define PanelSwapOddML		0
#define PanelSwapEvenML		0

#define PanelSwapOddRB		0//xFF
#define PanelSwapEvenRB		0//xFF
#define PanelSwapMLSB		0

#define PanelDClkDelay		0
#define PanelInvDE		0
#define PanelInvDClk		0
#define PanelInvHSync		0
#define PanelInvVSync		0

// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelDCLKCurrent		3 // Dclk current
#define PanelDECurrent		3 // DE signal current
#define PanelOddDataCurrent	3 // Odd data current
#define PanelEvenDataCurrent	3 // Even data current

#define PanelOnTiming1		30 //80 // time between panel & data while turn on power
#define PanelOnTiming2		550//80 // time between data & back light while turn on power
#define PanelOffTiming1		250 // time between back light & data while turn off power
#define PanelOffTiming2		20 // time between data & panel while turn off power

#define PanelHSyncWidth		16
#define PanelHSyncBackPorch	40

#define PanelVSyncWidth		5
#define PanelVSyncBackPorch	18

#define PanelHStart		(PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart		(PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth		1600
#define PanelHeight		900
#define PanelHTotal		1760
#define PanelVTotal		926

#define PanelMaxHTotal		2400
#define PanelMinHTotal		1680
#define PanelMaxVTotal		1200
#define PanelMinVTotal		907
#define PanelDCLK			98
#define PanelMaxDCLK		124
#define PanelMinDCLK			79



//=================================================================
// TCON	 setting for LVDS
//PVS / FSYNC
#define GPO9_VStart		0x7
#define GPO9_VEnd		0x705
#define GPO9_HStart		0x0
#define GPO9_HEnd		0x0
#define GPO9_Control		0x7

//HSYNC
#define GPOA_VStart		0x006
#define GPOA_VEnd		0x408
#define GPOA_HStart		0
#define GPOA_HEnd		0
#define GPOA_Control		0x4


