// for CMO "
#define PanelName	_C,_M,_O,_2,_2,_0,_Z,_1,_M,_0,_1
#define PANEL_TYPE	PANEL_22_WIDE
#define PanelDither		6  //2006-07-27 Andy 8//2006-07-14 Andy 6//2006-03-02 

#define PanelTTL		0
#define PanelTCON		0
#define PanelLVDS		0
#define PanelRSDS		0xFF
#define PanelminiLVDS      0 

#define ShortLineCheck		0
#define LVDS_TIMode		    0x00
#define PanelDualPort		0xFF
#define PanelSwapPort		0x0
#define PanelSwapOddML		0x00
#define PanelSwapEvenML	    0x00
#define PanelSwapOddRB		0xFF
#define PanelSwapEvenRB	    0xFF
#define PanelSwapMLSB		0xFF

#define PanelDClkDelay		8//0
#define PanelInvDClk		       0
#define PanelInvDE		       0
#define PanelInvHSync		0
#define PanelInvVSync		0

// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
#define PanelDCLKCurrent	 1 // Dclk current
#define PanelDECurrent		 1 // DE signal current
#define PanelOddDataCurrent	 1 // Odd data current
#define PanelEvenDataCurrent 1 // Even data current


#define PanelOnTiming0	25	//time between PanelVCC and DCDC			// William 20051011
#define PanelOnTiming1	34 	// time between DCDC & TCON turn on 		// William 20051011, for VGH 2 steps go High
#define PanelOnTiming2	20 	// time between TCON & backlight 			// William 20050908
#define PanelOnTiming3	50 	// time between backlight & PWM			// WIlliam 20050908
#define PanelOnTiming4	50  	// time between PWM & data

#define PanelOffTiming0	5	// time between data off & backlight off
#define PanelOffTiming1	5 	// time between backlight off & PWM off
#define PanelOffTiming2	5 	// time between PWM off & DCDC off		// WIlliam 20050819
#define PanelOffTiming3	0 	// time between DCDC off & TCON IO 
#define PanelOffTiming4	30//60 	// time between TCON IO & PanelVCC


#define PanelHSyncWidth		43 //40 //32
#define PanelHSyncBackPorch	41

#define PanelVSyncWidth	2
#define PanelVSyncBackPorch	38

#define PanelHStart		(PanelHSyncWidth+PanelHSyncBackPorch)
#define PanelVStart		(PanelVSyncWidth+PanelVSyncBackPorch)
#define PanelWidth		1680
#define PanelHeight		1050
#define PanelHTotal		1880
#define PanelVTotal		1089

#define PanelMaxHTotal		2240//1800
#define PanelMinHTotal		1880//2006-03-02 Andy 1688
#define PanelMaxVTotal		1600
#define PanelMinVTotal		1058//2006-03-10 900//2006-03-02 Andy 1042
#define PanelDCLK		            108
#define PanelMaxDCLK		170
#define PanelMinDCLK		       100

//=================================================================
#define TCON_Format1	0x1 // OFC1: control polarity & inversion

#define ESP_AfterData 	0 // Even Start Pulse after Data
#define ESP_Offset	       1 // Even start pulse position
#define OSP_AfterData 	0 // Odd Start Pulse after Data
#define OSP_Offset	       1 // Odd start pulse position

// TCON	 setting for RSDS
// driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA


#define PanelOESPCurrent	    3 // OSP/ESP drive current
#define PanelOEGCurrent	    2 // OPOL/EPOL/GPOL drive current
#define PanelEINVCurrent	    0 // EINV drive current
#define PanelOINVCurrent	    0 // OINV drive current


#define OuputFormat		G0AT_B|DATI_B|PUA_B//(DATI_B)
#define RSBMLSW             0
#define RSAMLSW             0



//===========================
//2006-03-10 Suggest By AE
// POL  //ok     //nick 060807
#define GPO0_VStart          0x000     
#define GPO0_VEnd            0x385
#define GPO0_HStart          0x168 
#define GPO0_HEnd            0x168
#define GPO0_Control        0x92//0x12     //2006-03-15  : one line flicker:0x02  Two Line :0x12  Solve Flicker
//#define GPO0_Control        0x02     //2006-03-15  : one line flicker:0x02  Two Line :0x12  Solve Flicker
//old:0x82 

// STB   //0k
#define GPO1_VStart		0x00
#define GPO1_VEnd		0x00
#define GPO1_HStart		0x350	
#define GPO1_HEnd		0x364	
#define GPO1_Control	0x00

// STV 
#define GPO2_VStart		0x000
#define GPO2_VEnd		0x001
#define GPO2_HStart		0x180          
#define GPO2_HEnd		0x180
#define GPO2_Control	0x00

// CLKV
#define GPO3_VStart		0
#define GPO3_VEnd		0
#define GPO3_HStart		0x00//0x180           
#define GPO3_HEnd		0x00//0x2D0
#define GPO3_Control		0x00//0x01

// OE
#define GPO4_VStart		0x0 
#define GPO4_VEnd		0x0
#define GPO4_HStart		0x280
#define GPO4_HEnd		0x310		
#define GPO4_Control	0x14//0x00

//GV_OFF   //ok 060807 nick
#define GPO5_VStart        0x0 
#define GPO5_VEnd          0x0 
#define GPO5_HStart        0x198	//  0x196 
#define GPO5_HEnd          0x2B6	//0x25E           
#define GPO5_Control       0x01 

//GV_ON     // no use for AOC Cmo  panel                    
#define GPO6_VStart         0x00               // 2006-03-13 Andy Polarity Reverse From GV_OFF
#define GPO6_VEnd           0x00 
#define GPO6_HStart         0x200      
#define GPO6_HEnd           0x2EF
#define GPO6_Control        0x00 

