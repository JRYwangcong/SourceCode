#ifndef __PanelLM230WF1TLD1__
    #define __PanelLM230WF1TLD1__
    #define PanelName			_L,_M,_2,_3,_0,_W,_F,_1,_T,_L,_D,_1
    #define PANEL_TYPE	        PANEL_22_WIDE
	
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

    #define PanelOnTiming1		20//60//20 // time between panel & data while turn on power
    #define PanelOnTiming2		550//350//220 // time between data & back light while turn on power
    #define PanelOffTiming1		250//350//220 // 2005/5/3// time between back light & data while turn off power 
    #define PanelOffTiming2		25//20//25 // 2005/5/3//10 // time between data & panel while turn off power 

    #define PanelHSyncWidth		25
    #define PanelHSyncBackPorch	35
    
    #define PanelVSyncWidth		3
    #define PanelVSyncBackPorch	13
    
    #define PanelHStart   (PanelHSyncWidth+PanelHSyncBackPorch)
    #define PanelVStart   (PanelVSyncWidth+PanelVSyncBackPorch)


    #define PanelWidth    1920
    #define PanelHeight   1080
    #define PanelHTotal   2176  
    #define PanelVTotal   1100 
    #define PanelMaxHTotal     2240
    #define PanelMinHTotal     2000//2030//2100
    #define PanelMaxVTotal     1160 
    #define PanelMinVTotal    1090

    
    #define PanelDCLK            144 
    #define PanelMaxDCLK          175
    #define PanelMinDCLK          120
    //HSYNC
    #define GPOA_VStart   0
    #define GPOA_VEnd   0
    #define GPOA_HStart   0
    #define GPOA_HEnd   0
    #define GPOA_Control  0
#endif

