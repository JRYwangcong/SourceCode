#define PanelName				_T,_P,_M,_2,_1,_5,_H,_W,_0,_1

#define PanelTTL    				0
#define PanelTCON   				0
#define PanelLVDS				0xff 
#define PanelRSDS				0
#define PanelminiLVDS      		0

#define _PanelColorDepth			0//bPanelColorDepth
#define _ChangeModePanelVddOff	bChangeModePanelVddOff


#define PanelDualPort   			0xff
#define PanelSwapPort   			0xff

#define _PanelOutputControl1		0x10
#define _PanelOutputControl2			(DOT_B | (PanelDualPort &DPO_B) | (PanelSwapPort &DPX_B))//Scaler Bank 0x43 Setting
//#define _PanelOSContol			0x80
//#define _PanelODRriving			0x55


#define _PanelOnTiming1    		20
#define _PanelOnTiming2    		500
#define _PanelOffTiming1 			140
#define _PanelOffTiming2 			20

#define _PanelHSyncWidth   		24
#define _PanelHSyncBackPorch 	31
#define _PanelVSyncWidth   		3
#define _PanelVSyncBackPorch 	13
#define _PanelHStart   			(_PanelHSyncWidth+_PanelHSyncBackPorch)
#define _PanelVStart   			(_PanelVSyncWidth+_PanelVSyncBackPorch)
 
#define _PanelWidth				1920
#define _PanelHeight				1080
#define _PanelHTotal				2100 
#define _PanelVTotal				1130 
#define _PanelMaxHTotal			2176 
#define _PanelMinHTotal			2048//2030//2100
#define _PanelMaxVTotal			2047
#define _PanelMinVTotal			1088


#define _PanelDCLK				140  
#define _PanelMaxDCLK			172 
#define _PanelMinDCLK			130


#define _DefBurstModeFreq		250
#define _DefMinDutyValue			0x13
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

    //=================================================================
    #define TCON_Format1  0x1 // OFC1: control polarity & inversion
    #define ESP_AfterData   0 // Even Start Pulse after Data
    #define ESP_Offset  1 // Even start pulse position
    #define OSP_AfterData   0 // Odd Start Pulse after Data
    #define OSP_Offset  1 // Odd start pulse position
    // driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
    #define PanelOESPCurrent  2 // OSP/ESP drive current
    #define PanelOEGCurrent   2 // OPOL/EPOL/GPOL drive current
    #define PanelEINVCurrent  0 // EINV drive current
    #define PanelOINVCurrent  0 // OINV drive current
    #define OuputFormat   (BIT5)//Enable Auto Toggle GPO0

    // POL
    #define GPO0_VStart   0x000
    #define GPO0_VEnd   0x000
    #define GPO0_HStart   0x200 // 090615 // 0x369//0x2B8
    #define GPO0_HEnd   0x3C8
    #define GPO0_Control    0x12 // 0x1a: 3-line toggle // 0x12: 2-line toggle // 0x02: 1-line toggle
    //LP
    #define GPO1_VStart   0
    #define GPO1_VEnd   0
    #define GPO1_HStart   0x3c8 //0x3f0 // 0x3d0 090602 // 0x3ca
    #define GPO1_HEnd   0x410 // 0x3e8 090602 // 0x3de
    #define GPO1_Control  0x00
    // STV
    #define GPO2_VStart   0x000
    #define GPO2_VEnd   0x001
    #define GPO2_HStart   0x1f8 // 0x2b0 // 090522
    #define GPO2_HEnd     0x1f0
    #define GPO2_Control  0x00
    // CLKV
    #define GPO3_VStart   0
    #define GPO3_VEnd   0x00 
    #define GPO3_HStart   0x1f8// 0x4c
    #define GPO3_HEnd   0x3b6
    #define GPO3_Control  0x01
    // OE
    #define GPO4_VStart   0
    #define GPO4_VEnd   0
    #define GPO4_HStart   0x308//0x300//0x330
    #define GPO4_HEnd   0x3e0// 0x3d8
    #define GPO4_Control  0x00// 0x01

    //GV_OFF 
    #define GPO5_VStart        0x0 
    #define GPO5_VEnd          0x0 
    #define GPO5_HStart        0x256 // 0x2c0 090223
    #define GPO5_HEnd          0x348 // 0x3c0 090223
    #define GPO5_Control       0x00//0x01 

    //PVS / FSYNC
    #define GPO9_VStart   0 //0x002
    #define GPO9_VEnd   0 //0x306
    #define GPO9_HStart   0 //0x00A
    #define GPO9_HEnd   0 //0x005
    #define GPO9_Control  0 //0x08
    //HSYNC
    #define GPOA_VStart   0
    #define GPOA_VEnd   0
    #define GPOA_HStart   0
    #define GPOA_HEnd   0
    #define GPOA_Control  0


