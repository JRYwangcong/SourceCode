#define PanelName				_T,_M,_S,_1,_9,_0

#define PanelTTL    				0
#define PanelTCON   				0
#define PanelLVDS				0xff
#define PanelRSDS				0
#define PanelminiLVDS      0 


#define _PanelColorDepth			bPanelColorDepth
#define _ChangeModePanelVddOff	bChangeModePanelVddOff


#define PanelDualPort   			0xff
#define PanelSwapPort   			0xff

#define _PanelOutputControl1		0x10
#define _PanelOutputControl2			(DOT_B | (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B))//Scaler Bank 0x43 Setting
//#define _PanelOSContol			0x80
//#define _PanelODRriving			0x55


#define _PanelOnTiming1    		25
#define _PanelOnTiming2    		250
#define _PanelOffTiming1 			250
#define _PanelOffTiming2 			25

#define _PanelHSyncWidth   		15
#define _PanelHSyncBackPorch 	39
#define _PanelVSyncWidth   		3
#define _PanelVSyncBackPorch 	21
#define _PanelHStart   			(_PanelHSyncWidth+_PanelHSyncBackPorch)
#define _PanelVStart   			(_PanelVSyncWidth+_PanelVSyncBackPorch)


#define _PanelWidth				1440
#define _PanelHeight				900
#define _PanelHTotal				1600 
#define _PanelVTotal				926 
#define _PanelMaxHTotal			1800
#define _PanelMinHTotal			1520
#define _PanelMaxVTotal			1100
#define _PanelMinVTotal			912


#define _PanelDCLK				88 
#define _PanelMaxDCLK			148 
#define _PanelMinDCLK			69


#define _DefBurstModeFreq		250
#define _DefMinDutyValue			0x30
#define _DefMaxDutyValue		0xFF


#define _STEPL					0x44
#define _STEPH					0x00
#define _SPANL					0xCC
#define _SPANH					0x01

#define _PanelLVDSSwing1			0x15// 283mV //For old chip
/*
BK1_2D        BK0_43[6]=0      BK0_43[6]=1 
0x00                133mV                67mV  
0x01                167mV                83mV  
0x02                200mV                100mV  
0x03                233mV                117mV  
0x04                267mV                133mV   
0x05                300mV                150mV  
0x06                333mV                167mV  
0x07                367mV                183mV  
0x08                400mV                200mV  
0x09                433mV                217mV  
0x0A                467mV                233mV  
0x0B                500mV                250mV  
0x0C                533mV                267mV  
0x0D                567mV                283mV  
0x0E                600mV                300mV  
0x0F                633mV                317mV  
0x10                400mV                200mV 
0x11                433mV                217mV 
0x12                467mV                233mV 
0x13                500mV                250mV 
0x14                533mV                267mV
0x15                567mV                283mV 
0x16                600mV                300mV 
0x17                633mV                317mV 
0x18                667mV                333mV 
0x19                700mV                350mV 
0x1A                733mV                367mV 
0x1B                767mV                383mV 
0x1C                800mV                400mV 
0x1D                833mV                417mV 
0x1E                867mV                433mV 
0x1F                900mV                450mV 
*/
#define _PanelLVDSSwing2			1// 0:242.37mV    1:339.33mV    2:387mV    3:169.67mV    //For TSUMXXN&TSUMXXQ type ic auto tune target voltage
#define _PanelLVDSSwing				(_PanelLVDSSwing1|_PanelLVDSSwing2<<6)