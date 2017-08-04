#define PanelName	_M,_1,_9,_5,_F,_E,_G,_L,_2,_0

#define _PanelDither		8 
#define PanelTTL		0
#define PanelTCON		0
#define PanelLVDS		0xFF
#define PanelRSDS		0
#define PanelminiLVDS      0 

#define _PanelColorDepth			bPanelColorDepth
#define _ChangeModePanelVddOff		bChangeModePanelVddOff

#define DISABLE_SHORT_FRAME_TUNE	1//petit 20121031

#define PanelDualPort		0xFF
#define PanelSwapPort		0xFF

#define _PanelOutputControl1	0x10
#define _PanelOutputControl2			(DOT_B | (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B))//Scaler Bank 0x43 Setting

#define _PanelOnTiming1		10 // time between panel & data while turn on power
#define _PanelOnTiming2		300 // time between data & back light while turn on power
#define _PanelOffTiming1		300  // time between back light & data while turn off power
#define _PanelOffTiming2		10  // time between data & panel while turn off power

#define _PanelHSyncWidth		45
#define _PanelHSyncBackPorch	40
#define _PanelVSyncWidth		3
#define _PanelVSyncBackPorch	21
#define _PanelHStart					(_PanelHSyncWidth+_PanelHSyncBackPorch)
#define _PanelVStart 					(_PanelVSyncWidth+_PanelVSyncBackPorch)
#define _PanelWidth		       1600
#define _PanelHeight		       900
#define _PanelHTotal		       2112
#define _PanelVTotal		       926
#define _PanelMaxHTotal			2300
#define _PanelMinHTotal			1912
#define _PanelMaxVTotal			942
#define _PanelMinVTotal			905
#define _PanelDCLK		       116
#define _PanelMaxDCLK			160
#define _PanelMinDCLK		  	86

#define _STEPL			0x44
#define _STEPH			0x00
#define _SPANL			0xCC
#define _SPANH			0x01
#define _DefBurstModeFreq		250

#define _DefMinDutyValue			0x00
#define _DefMaxDutyValue		0xFF

#define _PanelLVDSSwing1			0x15// 283mV //For old chip
#define _PanelLVDSSwing2			1// 0:242.37mV    1:339.33mV    2:387mV    3:169.67mV    //For TSUMXXN&TSUMXXQ type ic auto tune target voltage
#define _PanelLVDSSwing				(_PanelLVDSSwing1|_PanelLVDSSwing2<<6)