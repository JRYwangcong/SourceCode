#if PanelType==PanelAUOM240HW01V6
#define PanelName	_M,_2,_4,_0,_H,_W,_0,_1,_V,_6 // auo
#elif PanelType==PanelAUOM240HW01VB
#define PanelName	_M,_2,_4,_0,_H,_W,_0,_1,_V,_B
#endif

#define PanelDither		8

#define PanelTTL			0
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
#if 0//SingleBoard
#define PanelSwapLCPS		0xFF
#else
#define PanelSwapLCPS		0
#endif

#define PanelDClkDelay		0
#define PanelInvDClk			0
#define PanelInvDE			0
#define PanelInvHSync		0
#define PanelInvVSync		0

// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelDCLKCurrent		1 // Dclk current
#define PanelDECurrent		1 // DE signal current
#define PanelOddDataCurrent	1 // Odd data current
#define PanelEvenDataCurrent	1 // Even data current

#define PanelOnTiming1		15// T2 <= 20ms // time between panel & data while turn on power
#define PanelOnTiming2		350// T3 >= 200ms // time between data & back light while turn on power
#define PanelOffTiming1		250// T4 >= 200ms // time between back light & data while turn off power 
#define PanelOffTiming2		20// T5 <= 50ms // time between data & panel while turn off power 
#define PanelOffOnTiming    1050 // T7 >= 1000ms

#define PanelHSyncWidth		16//32
#define PanelHSyncBackPorch	80

#define PanelVSyncWidth		3
#define PanelVSyncBackPorch	13

 #define PanelHStart   (PanelHSyncWidth+PanelHSyncBackPorch)
 #define PanelVStart   (PanelVSyncWidth+PanelVSyncBackPorch)
 #define PanelWidth    1920
 #define PanelHeight   1080
 #define PanelHTotal   2120 
 #define PanelVTotal   1120 
 
 #define PanelMaxHTotal    4094
 #define PanelMinHTotal     2068
 #define PanelMaxVTotal     2047 
 #define PanelMinVTotal    1088
 #define PanelDCLK            150
 #define PanelMaxDCLK          180
 #define PanelMinDCLK          80

// 080514
#define PanelMaxVfreq   (750+13)
#define PanelMinVfreq   (500-13)

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

