#define PanelName					_T,_P,_M,_2,_4,_0,_W,_F,_1

#define _PanelColorDepth				bPanelColorDepth//6 Bit Panel->0		8 Bit Panel->bPanelColorDepth
#define PanelTTL						0
#define PanelTCON					0
#define PanelLVDS					0xff
#define PanelRSDS					0
#define PanelminiLVDS      0 

#define _ChangeModePanelVddOff		bChangeModePanelVddOff//Change Mode Panel Vdd On->0		Change Mode Panel Vdd Off->bChangeModePanelVddOff

#define PanelDualPort				0xFF
#define PanelSwapPort				0xff

#define _PanelOutputControl1			0x10    //Debug 0x42
#define _PanelOutputControl2			(DOT_B | (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B))//Scaler Bank 0x43 Setting

#define _PanelOnTiming1				15 // time between panel & data while turn on power
#define _PanelOnTiming2				550// time between data & back light while turn on power
#define _PanelOffTiming1				250 // time between back light & data while turn off power
#define _PanelOffTiming2				15 // time between data & panel while turn off power

#define _PanelHSyncWidth			24
#define _PanelHSyncBackPorch		40
#define _PanelVSyncWidth			3
#define _PanelVSyncBackPorch		5
#define _PanelHStart					(_PanelHSyncWidth+_PanelHSyncBackPorch)
#define _PanelVStart					(_PanelVSyncWidth+_PanelVSyncBackPorch)

#define _PanelWidth					1920
#define _PanelHeight					1080
#define _PanelHTotal					2100 
#define _PanelVTotal					1130 
#define _PanelMaxHTotal				2200  
#define _PanelMinHTotal					2068
#define _PanelMaxVTotal				1818 
#define _PanelMinVTotal					1092 
#define _PanelDCLK					144  
#define _PanelMaxDCLK					188
#define _PanelMinDCLK					100


#define _DefBurstModeFreq			250
#define _DefMinDutyValue			0x1B
#define _DefMaxDutyValue			0xFF


#define _STEPL						0x70
#define _STEPH						0x00
#define _SPANL						0xCC
#define _SPANH						0x01

#define _PanelLVDSSwing1			0x15// 283mV //For old chip
#define _PanelLVDSSwing2			1// 0:242.37mV    1:339.33mV    2:387mV    3:169.67mV    //For TSUMXXN&TSUMXXQ type ic auto tune target voltage
#define _PanelLVDSSwing				(_PanelLVDSSwing1|_PanelLVDSSwing2<<6)

