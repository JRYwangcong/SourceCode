#ifndef __PANEL_LM190E08__
    #define __PANEL_LM190E08__
    #define PanelName               _L,_M,_1,_9,_0,_E,_0,_8
#define PANEL_TYPE	        PANEL_19

	
    #define PanelDither   8
    #define PanelTTL    0
    #define PanelTCON   0
    #define PanelLVDS   0xff
    #define PanelRSDS   0
    #define PanelminiLVDS      0 

    #define ShortLineCheck    0
    #define LVDS_TIMode   0xff
    #define PanelDualPort   0xff
    #define PanelSwapPort   0xff
    #define PanelSwapOddML    0
    #define PanelSwapEvenML   0
    #define PanelSwapOddRB    0
    #define PanelSwapEvenRB   0
    #define PanelSwapMLSB   0
    #define PanelDClkDelay    8
    #define PanelInvDE      0
    #define PanelInvDClk    0
    #define PanelInvHSync   0
    #define PanelInvVSync   0
    // driving current setting 0==>4mA, 1==>6mA, 2==>8mA ,3==>12mA
    #define PanelDCLKCurrent  1 // Dclk current
    #define PanelDECurrent    1 // DE signal current
    #define PanelOddDataCurrent 1 // Odd data current
    #define PanelEvenDataCurrent  1 // Even data current
    #define PanelOnTiming1		20 //80 // time between panel & data while turn on power
    #define PanelOnTiming2		250 //80 // time between data & back light while turn on power
    #define PanelOffTiming1		250 //250 //80 // time between back light & data while turn off power
    #define PanelOffTiming2		20 //5 //80 // time between data & panel while turn off power
    #define PanelHSyncWidth	       25//32
    #define PanelHSyncBackPorch	15//40
    #define PanelVSyncWidth		12
    #define PanelVSyncBackPorch	28
    #define PanelHStart		(PanelHSyncWidth+PanelHSyncBackPorch)
    #define PanelVStart		(PanelVSyncWidth+PanelVSyncBackPorch)
    #define PanelWidth		1280
    #define PanelHeight		1024
    #define PanelHTotal		1688
    #define PanelVTotal		1066
    #define PanelMaxHTotal		1800
    #define PanelMinHTotal		1600
    #define PanelMaxVTotal		2000
    #define PanelMinVTotal		900
    #define PanelDCLK		       108
    #define PanelMaxDCLK		135  //2004/01/13 num1
    #define PanelMinDCLK		100
    #define HV_OUTPUT_TYPE          HSRM_B
    //=================================================================
    // TCON  setting for RSDS
    // TCON  setting for RSDS
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
    #define OuputFormat   (BIT3|BIT5)//Enable Auto Toggle GPO0 & data invert
    // POL
    #define GPO0_VStart   0x000
    #define GPO0_VEnd   0x000
    #define GPO0_HStart   0x186 //0x068
    #define GPO0_HEnd   0x186 //0x222
    #define GPO0_Control    0x02 //0x02 //0x82
    //LP
    //HSYNC
    #define GPOA_VStart   0
    #define GPOA_VEnd   0
    #define GPOA_HStart   0
    #define GPOA_HEnd   0
    #define GPOA_Control  0
#endif
