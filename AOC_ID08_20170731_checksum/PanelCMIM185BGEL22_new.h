
#define PanelName               _M,_1,_8,_5,_F,_E,_G,_L,_2,_0

#define PanelDither 		8
#define PanelTTL			0
#define PanelTCON		0
#define PanelLVDS		0xff
#define PanelRSDS		0
#define PanelminiLVDS      0 

#define LVDS_TIMode   			0xff
#define PanelDualPort   			0//0xff
#if (MainBoardType==MainBoard_715G2904_B|| MainBoardType==MainBoard_712G3244_2||MainBoardType==MainBoard_715G3834||MainBoardType==MainBoard_3834_18T)
#define PanelSwapPort   			0xFF
#else
#define PanelSwapPort   			0
#define PanelSwapOddML    		0
#define PanelSwapEvenML   		0
#define PanelSwapOddRB    		0
#define PanelSwapEvenRB   		0
#define PanelSwapMLSB   			0
#define PanelDClkDelay    			8
#define PanelInvDE      			0
#define PanelInvDClk    			0
#define PanelInvHSync   			0//0xFF
#define PanelInvVSync   			0//0xff
// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelDCLKCurrent  		1 // Dclk current
#define PanelDECurrent    			1 // DE signal current
#define PanelOddDataCurrent 		1 // Odd data current
#define PanelEvenDataCurrent  	1 // Even data current
#define PanelOnTiming1    		15 // time between panel & data while turn on power
#define PanelOnTiming2    		250 // time between data & back light while turn on power  //20050701
#define PanelOffTiming1 		250 // time between back light & data while turn off power
#define PanelOffTiming2 		15 // time between data & panel while turn off power
#define PanelHSyncWidth			15
#define PanelHSyncBackPorch		15
#define PanelVSyncWidth			3
#define PanelVSyncBackPorch		29
#define PanelHStart 		(PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart 		(PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth			1366
#define PanelHeight 		768
#define PanelHTotal			1560
#define PanelVTotal 		806
#define PanelMaxHTotal		1570
#define PanelMinHTotal		1512
#define PanelMaxVTotal		815
#define PanelMinVTotal		800
#define PanelDCLK			76
#define PanelMaxDCLK		95
#define PanelMinDCLK		63
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

