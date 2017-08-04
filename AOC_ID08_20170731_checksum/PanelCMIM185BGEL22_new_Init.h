
#define PanelTpye_1366
#define PanelName	 _M,_1,_8,_5,_B,_G,_E,_L,_2,_2
#define _PanelColorDepth			bPanelColorDepth//0->6bit+FRC						bPanelColorDepth->8Bit
#define PanelTTL					0
#define PanelTCON					0
#define PanelLVDS					0xff
#define PanelRSDS					0
#define PanelminiLVDS      0
    
#define _ChangeModePanelVddOff		bChangeModePanelVddOff//0->Change Mode Panel Vdd On		bChangeModePanelVddOff->Change Mode Panel Vdd Off

#define PanelDualPort   			0
#if (MainBoardType==MainBoard_715G2904_B|| MainBoardType==MainBoard_712G3244_2||MainBoardType==MainBoard_715G3834||MainBoardType==MainBoard_3834_18T)
#define PanelSwapPort   			0xFF
#else
#define PanelSwapPort				0
#endif

#define _PanelOutputControl1			0x10//Scaler Bank 0x42 Setting
#define _PanelOutputControl2			(DOT_B | (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B))//Scaler Bank 0x43 Setting

#define _PanelOnTiming1				15// time between panel & data while turn on power
#define _PanelOnTiming2				250 // time between data & back light while turn on power
#define _PanelOffTiming1			250// time between back light & data while turn off power
#define _PanelOffTiming2			15// time between data & panel while turn off power

#define _PanelHSyncWidth			15 // 35  20111227 hong
#define _PanelHSyncBackPorch		15// 21  20111227 hong
#define _PanelVSyncWidth			3
#define _PanelVSyncBackPorch		29
#define _PanelHStart				(_PanelHSyncWidth+_PanelHSyncBackPorch)
#define _PanelVStart 				(_PanelVSyncWidth+_PanelVSyncBackPorch)

#define _PanelWidth				1366
#define _PanelHeight				768
#define _PanelHTotal				1560
#define _PanelVTotal				806
#define _PanelMaxHTotal			1570
#define _PanelMinHTotal				1512 
#define _PanelMaxVTotal			815
#define _PanelMinVTotal				800  

#define _PanelDCLK				76
#define _PanelMaxDCLK				95
#define _PanelMinDCLK				63

#define _DefBurstModeFreq			250//Hz
#define _DefMinDutyValue			0x2B
#define _DefMaxDutyValue			0xFF

#define _STEPL						0x33
#define _STEPH						0x00
#define _SPANL						0xCC
#define _SPANH						0x01

#define _PanelLVDSSwing1			0x15// 283mV //For old chip
#define _PanelLVDSSwing2			1// 0:242.37mV    1:339.33mV    2:387mV    3:169.67mV    //For TSUMXXNXX&TSUMXXQXX type ic auto tune target voltage
#define _PanelLVDSSwing				(_PanelLVDSSwing1|_PanelLVDSSwing2<<6)

