#define PanelName					_H,_S,_M,_1,_9,_5

#define _PanelColorDepth				bPanelColorDepth//6 Bit Panel->0		8 Bit Panel->bPanelColorDepth
#define PanelTTL						0
#define PanelTCON					0
#define PanelLVDS					0xff
#define PanelRSDS					0
#define PanelminiLVDS      			0 
#define PanelSwapOddRB				0
#define PanelSwapEvenRB		             0
#define PanelSwapMLSB				0
#define PanelSwapOddML				0
#define PanelSwapEvenML				0
#define PanelDither                              8

#define _ChangeModePanelVddOff		bChangeModePanelVddOff//Change Mode Panel Vdd On->0		Change Mode Panel Vdd Off->bChangeModePanelVddOff

#define PanelDualPort					0xFF
#define PanelSwapPort				0xff
#define LVDS_TIMode                     	0xFF

#define _PanelOutputControl1			0x10    //Debug 0x42
#define _PanelOutputControl2			(DOT_B | (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B))//Scaler Bank 0x43 Setting
//#define _PanelOSContol				0x80        //Debug 0xF2
//#define _PanelODRriving				0x55        //Debug 0xF5


#define PanelOnTiming1		   		100  // 20  // time between panel & data while turn on power
#define PanelOnTiming2		  		400  //250 // 255 // time between data & back light while turn on power
#define PanelOffTiming1		  		300// 500 // time between back light & data while turn off power
#define PanelOffTiming2		   		25  // time between data & panel while turn off power

#define PanelHSyncWidth		40 // 35
#define PanelHSyncBackPorch	20

#define PanelVSyncWidth		6
#define PanelVSyncBackPorch	20

#define PanelHStart		(PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart		(PanelVSyncWidth+PanelVSyncBackPorch)

#define PanelWidth		1600//YML
#define PanelHeight		900//YML

#define PanelHTotal		2112 // 2128
#define PanelVTotal		934 // 932

#define PanelMaxHTotal		2144 // 2160//YML
#define PanelMinHTotal		2080 // 1800//YML20090211//2000

#define PanelMaxVTotal		942 // 1880//YML
#define PanelMinVTotal		929 // 1852//YML

#define PanelDCLK			118 // 120//YML
#define PanelMaxDCLK		151//YML
#define PanelMinDCLK			97 // 97//YML


#define _DefBurstModeFreq			259
#define _DefMinDutyValue				0x0a// 0x35//0x39
#define _DefMaxDutyValue			0xFF


#define _STEPL						0x44
#define _STEPH						0x00
#define _SPANL						0xCC
#define _SPANH						0x01

#define _PanelLVDSSwing1			0x15// 283mV //For old chip

#define GPO9_VStart		0x7
#define GPO9_VEnd		0x705
#define GPO9_HStart		0x0
#define GPO9_HEnd		0x0
#define GPO9_Control	0x7

//HSYNC
#define GPOA_VStart		0x006
#define GPOA_VEnd		0x408
#define GPOA_HStart		0 
#define GPOA_HEnd		0 
#define GPOA_Control	0x4

#define _PanelLVDSSwing2			0x03// 0:242.37mV    1:339.33mV    2:387mV    3:169.67mV    //For TSUMXXN&TSUMXXQ type ic auto tune target voltage
#define _PanelLVDSSwing				(_PanelLVDSSwing1|_PanelLVDSSwing2<<6)


