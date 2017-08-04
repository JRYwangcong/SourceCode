#include "board.h"
#include "types.h"
#include "misc.h"
#include "debug.h"
#include "ms_rwreg.h"
#include "Reg52.h"
#if ENABLE_HDCP
    #include "msHDCP.h"
#endif
#include "global.h"
#include "power.h"

#if MainBoardType == MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A || MainBoardType==MainBoard_715G3959_T
extern void SetScalerGPIO(BYTE pinX, bit HiLow);
#endif

void Init_ExternalInterrupt(void);
void Init_Timer(void);
void Init_SerialPort(void);
void Init_IOPorts(void);
void MCU_EnableCache(void);

//extern xdata BYTE xfr_regs[256];
void Init_MCU(void)
{
    EA = 0;
    Init_IOPorts();
    Init_ExternalInterrupt();
    Init_Timer();
    Init_SerialPort();

    MCU_EnableCache();
    EA = 1;
}
// Initialize I/O setting
void Init_IOPorts(void)
{
    #if MST_MCU
        #if 0  //2006-05-16 Andy
            P0 = 0xC7;
            //P1 = 0x60;
            P1 = 0x64; //2006-02-27 Andy
            //2006-03-20 Andy P2= 0x10;
            P2 = 0x00;
            hw_SetDDC_WP(); //2006-03-20 Andy
            Set_EEPROM_WP(); //2006-03-20 Andy
        #endif
		#if MainBoardType == MainBoard_4432
             P0 = 0xFF;
             P1 = 0xFF;
             P2 = 0xFF;
             P4 = 0xFF;
   #elif (MainBoardType==MainBoard_715G3834)
        P0 = 0xFF;
        P1 = 0xFF;
        P2 = 0xFF;
        P4 = 0x00;	
   #elif (MainBoardType==MainBoard_715G5265_T)//petit 20121030
        P0 = 0x3F;//0xFF;
        P1 = 0xFB;//0x7F;
        P2 = 0xFF;
        P4 = 0xFF;	
#elif (MainBoardType == MainBoard_SLB1PTR||MainBoardType == MainBoard_JRYL1PTR_LF)

    

    P2 = P2_Init;
	
    P4 = P4_Init;
		
	    #else		 
        P0 = 0xFF;
        //P1 = 0x60;
        P1 = 0xFF; //2006-06-06 Andy
        //P2 = 0x10;
        //  P2 = 0x13;//2006-06-06 Andy  For DVI-DDC SCL&SDA
        P2 = 0xFF; //2006-06-06 Andy  For DSUB-DDC SCL&SDA
        P4 = 0x00;
		#endif
        DDC2BI_CTRL = 0x13;
	#if (MainBoardType==MainBoard_2573||MainBoardType==MainBoard_2186_2A||MainBoardType==MainBoard_2186_1||MainBoardType==MainBoard_2578)
		MCU_P2_OUT_EN = MCU_P2_OUT_EN | BIT2 | BIT3 | BIT5 | BIT4;
		MCU_P2_FMD = MCU_P2_FMD | BIT2 | BIT3 | BIT5 | BIT4;
		MCU_P4_OUT_EN = MCU_P4_OUT_EN | (BIT4 | BIT6 | BIT7);
		MCU_P4_FMD = MCU_P4_FMD | (BIT4 | BIT6 | BIT7);
	#elif (MainBoardType==MainBoard_2698_2_X_X_2)
		MCU_P2_OUT_EN = (BIT2 | BIT4 | BIT5);
		MCU_P2_FMD = (BIT2 | BIT4 | BIT5);
		MCU_P4_OUT_EN = (BIT6 | BIT7);
		MCU_P4_FMD = (BIT6 | BIT7);
		
#elif (MainBoardType == MainBoard_SLB1PTR||MainBoardType == MainBoard_JRYL1PTR_LF)

    MCU_P0_FMD = InitMCU_P0_FMD;
    MCU_P0_OUT_EN = InitMCU_P0_OUT_EN;
    MCU_P0_ROD_EN = InitMCU_P0_ROD_EN;

    P0 = P0_Init;
	
    MCU_P1_FMD 	  = InitMCU_P1_FMD;
    MCU_P1_OUT_EN = InitMCU_P1_OUT_EN;
    MCU_P1_ROD_EN = InitMCU_P1_ROD_EN;

	P1 = P1_Init;

    MCU_P2_FMD = InitMCU_P2_FMD;
    MCU_P2_OUT_EN = InitMCU_P2_OUT_EN;
    MCU_P2_ROD_EN = InitMCU_P2_ROD_EN;

    MCU_P3_FMD = InitMCU_P3_FMD;
    MCU_P3_OUT_EN = InitMCU_P3_OUT_EN;
    MCU_P3_ROD_EN = InitMCU_P3_ROD_EN;

    MCU_P4_FMD = InitMCU_P4_FMD;
    MCU_P4_OUT_EN = InitMCU_P4_OUT_EN;
    MCU_P4_ROD_EN = InitMCU_P4_ROD_EN;

	#elif (MainBoardType==MainBoard_2698_2_X_X_8)
		MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT6|BIT7);
		MCU_P0_FMD=MCU_P0_FMD|(BIT6|BIT7);
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2|BIT3|BIT4);
		MCU_P1_FMD=MCU_P1_FMD|(BIT2|BIT3|BIT4);
		MCU_P2_OUT_EN=MCU_P2_OUT_EN|(BIT2|BIT3|BIT4);
		MCU_P2_FMD=MCU_P2_FMD|(BIT2|BIT3|BIT4);
	#elif (MainBoardType==MainBoard_2701)
		MCU_P2_OUT_EN = MCU_P2_OUT_EN | (BIT2 | BIT3 | BIT4); //2005-04-11
		MCU_P2_FMD = MCU_P2_FMD | (BIT2 | BIT3 | BIT4); //2005-04-11
	#elif (MainBoardType==MainBoard_715G2575)
		MCU_P1_OUT_EN = MCU_P1_OUT_EN |BIT2;
		MCU_P1_FMD =  MCU_P1_FMD |BIT2;
		MCU_P2_OUT_EN = MCU_P2_OUT_EN |BIT3;
		MCU_P2_FMD =  MCU_P2_FMD |BIT3;
	#elif (MainBoardType==MainBoard_2810_B||MainBoardType==MainBoard_715G2808)
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT6);
		MCU_P1_FMD=MCU_P1_FMD|(BIT6);

		MCU_P2_OUT_EN = MCU_P2_OUT_EN|BIT5 |BIT6|BIT7;
		MCU_P2_FMD =  MCU_P2_FMD|BIT5 |BIT6|BIT7;
		MCU_P4_OUT_EN = (BIT2|BIT3 );
		MCU_P4_FMD = (BIT2|BIT3 );
	#elif (MainBoardType==MainBoard_715G2988)    
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|BIT5 |BIT6|BIT7;
		MCU_P2_FMD =  MCU_P2_FMD|BIT5 |BIT6|BIT7;
		MCU_P4_OUT_EN = (BIT2|BIT3 );
		MCU_P4_FMD = (BIT2|BIT3 );
	#elif (MainBoardType==MainBoard_2883_B_X_X_1)
		MCU_P0_OUT_EN = MCU_P0_OUT_EN|BIT7|BIT6 ;
		MCU_P0_FMD =  MCU_P0_FMD|BIT7|BIT6;
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|BIT2|BIT4|BIT5;
		MCU_P1_FMD=MCU_P1_FMD|BIT2|BIT4|BIT5;
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|BIT2|BIT3|BIT4;
		MCU_P2_FMD =  MCU_P2_FMD|BIT2|BIT3|BIT4;
	#elif (MainBoardType==MainBoard_2883_B_X_X_2)//yuanfa
		MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT7);
		MCU_P0_FMD=MCU_P0_FMD|(BIT7);
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT5);
		MCU_P1_FMD=MCU_P1_FMD|(BIT5);
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|BIT4|BIT5;
		MCU_P2_FMD =  MCU_P2_FMD|BIT4|BIT5;
		MCU_P4_OUT_EN = MCU_P4_OUT_EN|BIT7|BIT6 ;
		MCU_P4_FMD =  MCU_P4_FMD|BIT7|BIT6;
	#elif (MainBoardType==MainBoard_715G2564_B)
		P0 = 0xC7;
		P1 = 0x78;
		P2 = 0x94;	  //0x90  //XM V1.01
		MCU_P2_OUT_EN=MCU_P2_OUT_EN|(BIT2|BIT3);
		MCU_P2_FMD=MCU_P2_FMD|(BIT2|BIT3);
		DDC2BI_CTRL=0x13;
	#elif (MainBoardType==MainBoard_715G2904_B||MainBoardType==MainBoard_715G3529)
        #if ModelName==AOC_1620Sw||ModelName==AOC_1620Sw_TPM156B1L01
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2);//100915
		MCU_P1_FMD=MCU_P1_FMD|(BIT2); 
		#else
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT0|BIT2);
		MCU_P1_FMD=MCU_P1_FMD|(BIT0|BIT2); 
		#endif
		MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT6|BIT7);  
		MCU_P0_FMD=MCU_P0_FMD|(BIT6|BIT7);
             #if ModelName==AOC_e1620Swb||ModelName==AOC_e1621Swb_
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT7);
		MCU_P2_FMD =  MCU_P2_FMD|(BIT4|BIT7);
	     #else
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|BIT7;
		MCU_P2_FMD =  MCU_P2_FMD|BIT7;
	     #endif
	#elif (MainBoardType==MainBoard_715G4734)
	       MCU_P0_ROD_EN=MCU_P0_ROD_EN|BIT6|BIT7;
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT7);
		MCU_P2_FMD =  MCU_P2_FMD|(BIT4|BIT7);
	#elif (MainBoardType==MainBoard_2670_C) //Amy2007.12.18   9:28:44	
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT6|BIT5); 
		MCU_P1_FMD=MCU_P1_FMD|(BIT6|BIT5);       
		MCU_P2_OUT_EN=MCU_P2_OUT_EN|(BIT4|BIT7);//(BIT2|BIT3|BIT7|BIT4);
		MCU_P2_FMD=MCU_P2_FMD|(BIT4|BIT7);//(BIT2|BIT3|BIT7|BIT4);       
	#elif (MainBoardType==MainBoard_2670_1)
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT6);  //2005-04-11
		MCU_P1_FMD=MCU_P1_FMD|(BIT6);        //2005-04-11  
		MCU_P2_OUT_EN=MCU_P2_OUT_EN|(BIT2|BIT3|BIT4|BIT7);  //2005-04-11
		MCU_P2_FMD=MCU_P2_FMD|(BIT2|BIT3|BIT4|BIT7);        //2005-04-11  
	#elif (MainBoardType==MainBoard_2670_B) //Amy2007.12.18   9:28:44 
		MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT6|BIT7);
		MCU_P0_FMD=MCU_P0_FMD|(BIT6|BIT7);

		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT6|BIT5); 
		MCU_P1_FMD=MCU_P1_FMD|(BIT6|BIT5);       
		MCU_P2_OUT_EN=MCU_P2_OUT_EN|(BIT4|BIT7);
		MCU_P2_FMD=MCU_P2_FMD|(BIT4|BIT7);
	#elif (MainBoardType==RDG2670_C_4_X_1_080611) 
		MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT6|BIT7);
		MCU_P0_FMD=MCU_P0_FMD|(BIT6|BIT7);
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT5|BIT6); 
		MCU_P1_FMD=MCU_P1_FMD|(BIT5);       
		MCU_P2_OUT_EN=MCU_P2_OUT_EN|(BIT4);
		MCU_P2_FMD=MCU_P2_FMD|(BIT4);
		
	 #elif (MainBoardType == MainBoard_2883_1B)
		//MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT7);
		//MCU_P0_FMD=MCU_P0_FMD|(BIT7);
		//MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT5);
		//MCU_P1_FMD=MCU_P1_FMD|(BIT5);
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|BIT4|BIT5;
		MCU_P2_FMD =  MCU_P2_FMD|BIT4|BIT5;
		MCU_P4_OUT_EN = MCU_P4_OUT_EN|BIT7|BIT6 ;
		MCU_P4_FMD =  MCU_P4_FMD|BIT7|BIT6;
	 #elif (MainBoardType==MainBoard_715G3329_A2||MainBoardType==MainBoard_715G3598_M0A)
		P4 |= BIT2;
		#ifdef TSUMXXNX
		MCU_P1_OUT_EN |= BIT2;
		MCU_P1_FMD |= BIT2;
		
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|BIT7|BIT4|BIT3|BIT2;
		MCU_P2_FMD =  MCU_P2_FMD|BIT7|BIT4|BIT3|BIT2;
		MCU_P2_ROD_EN=MCU_P2_ROD_EN|BIT1|BIT0;
		
		#if AudioFunc || ENABLE_HDMI
		MCU_P1_OUT_EN |= BIT4;
		MCU_P1_FMD |= BIT4;
		#endif

		#if ScanKey
		#if ModelName==ENVISION_P2471w ||ModelName==ENVISION_P2271w||ModelName == ENVISION_P2271wl||ModelName==ENVISION_P2471||ModelName==ENVISION_P2371l||ModelName==ENVISION_P2371l2||ModelName==ENVISION_P971waL2
		MCU_P0_OUT_EN &= ~BIT7;
		MCU_P0_FMD &= ~BIT7;
		MCU_P0_ROD_EN = BIT7;
		#else
		MCU_P0_OUT_EN |= BIT7;
		MCU_P0_FMD |=  BIT7;
		#endif
		MCU_P4_OUT_EN |= BIT0;
		MCU_P4_FMD |= BIT0;
		#endif
		#else		
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|BIT4|BIT5;
		MCU_P2_FMD =  MCU_P2_FMD|BIT4|BIT5;
		MCU_P4_OUT_EN = MCU_P4_OUT_EN|BIT7|BIT6|BIT5;
		MCU_P4_FMD =  MCU_P4_FMD|BIT7|BIT6|BIT5;
		#if AudioFunc || ENABLE_HDMI
		MCU_P1_OUT_EN |= BIT3;
		MCU_P1_FMD |= BIT3;
		#endif
		#if ScanKey
		#if ModelName==ENVISION_P2471w ||ModelName==ENVISION_P2271w||ModelName == ENVISION_P2271wl||ModelName==ENVISION_P2471||ModelName==ENVISION_P2371l||ModelName==ENVISION_P2371l2||ModelName==ENVISION_P971waL2
		MCU_P0_OUT_EN &= ~BIT7;
		MCU_P0_FMD &= ~BIT7;
		MCU_P0_ROD_EN = BIT7;
		#else
		MCU_P0_OUT_EN |= BIT7;
		MCU_P0_FMD |=  BIT7;
		#endif
		MCU_P2_OUT_EN |= BIT3;
		MCU_P2_FMD |= BIT3;
		#endif
		#endif
	#elif(MainBoardType==MainBoard_715G3329_58N)
			P0 = 0x7F;
			MCU_P0_ROD_EN=MCU_P0_ROD_EN|(BIT6|BIT7); //for LED P06/P07 open drain lizzie.ding	100823
			MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2);
			MCU_P1_FMD=MCU_P1_OUT_EN|(BIT2);
			MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT2|BIT4|BIT7);
			MCU_P2_FMD =  MCU_P2_FMD|(BIT2|BIT4|BIT7);
			MCU_P4_OUT_EN = MCU_P4_OUT_EN | (BIT2);
			MCU_P4_FMD = MCU_P4_FMD & (~BIT2);
	#elif(MainBoardType==MainBoard_715G3329_18N)
			P0 = 0x7F;
			MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT6|BIT7);
			MCU_P0_FMD=MCU_P0_OUT_EN|(BIT6|BIT7);
			MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2);
			MCU_P1_FMD=MCU_P1_OUT_EN|(BIT2);
			MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT2|BIT4|BIT7);
			MCU_P2_FMD =  MCU_P2_FMD|(BIT2|BIT4|BIT7);
			MCU_P4_OUT_EN = MCU_P4_OUT_EN | (BIT2);
			MCU_P4_FMD = MCU_P4_FMD & (~BIT2);
	#elif (MainBoardType==MainBoard_712G3244_2)

		MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT6|BIT7);
        MCU_P0_FMD =  MCU_P0_FMD|(BIT6|BIT7);
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2);
		MCU_P1_FMD=MCU_P1_FMD|(BIT2);

		MCU_P1_ROD_EN=MCU_P1_ROD_EN|(BIT0);
		
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT7);
		MCU_P2_FMD =  MCU_P2_FMD|(BIT4|BIT7);
		
	#elif (MainBoardType==MainBoard_715G3598_1PLL)
		
		MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT7);
		MCU_P0_FMD =  MCU_P0_FMD|(BIT7);
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2);
		MCU_P1_FMD=MCU_P1_FMD|(BIT2);				
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT2|BIT4|BIT7);
		MCU_P2_FMD =  MCU_P2_FMD|(BIT2|BIT4|BIT7);
		

	#elif (MainBoardType==MainBoard_3289_A2||MainBoardType==MainBoard_715G3289_C2) 
            #ifdef LEDCtrlByScalerIOPin // prevent LED flash when AC on
                #if MainBoardType==MainBoard_715G3289_C2
                MCU_P0_OUT_EN=BIT0;
                MCU_P0_FMD=BIT0;
                #endif
            #else
            MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT0|BIT6|BIT7);
            MCU_P0_FMD=MCU_P0_FMD|(BIT0|BIT6|BIT7);
            #endif
            MCU_P2_OUT_EN |= BIT4; // flash wp
            MCU_P2_FMD |= BIT4; // flash wp
            MCU_P4_OUT_EN |= BIT4; 
            MCU_P4_FMD |= BIT4;
        #elif (MainBoardType == MainBoard_715G3603_M0A)
            MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT5);
        	MCU_P0_FMD =  MCU_P0_FMD|(BIT5);
            MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4);
			MCU_P2_FMD =  MCU_P2_FMD|(BIT4);
        #elif MainBoardType==MainBoard_715G3663_M0A
            MCU_P0_OUT_EN = BIT5;
            MCU_P0_FMD = BIT5;
            MCU_P2_OUT_EN = BIT4; // b4:flash wp
            MCU_P2_FMD = BIT4; // b4:flash wp
        #elif (MainBoardType==MainBoard_715G3244_TSUMU18NR)
			MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT7);
        	MCU_P0_FMD =  MCU_P0_FMD|(BIT7);

			MCU_P0_ROD_EN=MCU_P0_ROD_EN|(BIT6);
			
			MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2);
			MCU_P1_FMD=MCU_P1_FMD|(BIT2);

			MCU_P1_ROD_EN=MCU_P1_ROD_EN|(BIT0);
			
			MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT7);
			MCU_P2_FMD =  MCU_P2_FMD|(BIT4|BIT7);
	  #elif (MainBoardType==MainBoard_715G4175_M0A)

	  	MCU_P1_OUT_EN = MCU_P1_OUT_EN|(BIT2);
		MCU_P1_FMD = MCU_P1_FMD|(BIT2);
		
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT7);
		MCU_P2_FMD =  MCU_P2_FMD|(BIT4|BIT7);
		MCU_P2_ROD_EN=MCU_P2_ROD_EN|BIT1|BIT0;
		#elif MainBoardType == MainBoard_4432
		
        MCU_P0_ROD_EN=MCU_P0_ROD_EN|(BIT7);
		
        MCU_P0_OUT_EN = BIT6 ;
        MCU_P0_FMD = BIT6 ;
		//MCU_P0_OUT_EN = BIT6 | BIT7;
        //MCU_P0_FMD = BIT6 | BIT7;

		MCU_P1_OUT_EN = BIT2;
        MCU_P1_FMD = BIT2;
        MCU_P2_OUT_EN = BIT2 | BIT3 | BIT4 | BIT6 | BIT7;
        MCU_P2_FMD = BIT2 | BIT3 | BIT4 | BIT6 | BIT7;
	#elif (MainBoardType==MainBoard_715G3329_1_2||MainBoardType==MainBoard_715G3329_1_2A)
      
	  //  MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT7);
	  //  MCU_P0_FMD =  MCU_P0_FMD|(BIT7);
          MCU_P0_ROD_EN=MCU_P0_ROD_EN|(BIT7);
	      
		//MCU_P1_OUT_EN = MCU_P1_OUT_EN|(BIT5);
		//MCU_P1_FMD =  MCU_P1_FMD|(BIT5);
		
          MCU_P1_ROD_EN=MCU_P1_ROD_EN|(BIT5);
		
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT5);
		MCU_P2_FMD =  MCU_P2_FMD|(BIT4|BIT5);

		MCU_P4_OUT_EN = (MCU_P4_FMD)|(BIT2|BIT5|BIT6);
		MCU_P4_FMD =  (MCU_P4_FMD&~BIT2)|(BIT5|BIT6);

	#elif (MainBoardType==MainBoard_715G3529_T)	//110311 Modify
			MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2);
			MCU_P1_FMD=MCU_P1_FMD|(BIT2); 
			MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT6|BIT7);  
			MCU_P0_FMD=MCU_P0_FMD|(BIT6|BIT7);
			MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT7);
			MCU_P2_FMD =  MCU_P2_FMD|(BIT4|BIT7);
	#elif (MainBoardType==MainBoard_715G3959_T)		//110617 Modify
			#if !ScanKey		//110623 Modify
			MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT6|BIT7);
			MCU_P0_FMD = MCU_P0_FMD|(BIT6|BIT7);
			#endif
			MCU_P1_OUT_EN = MCU_P1_OUT_EN|BIT3;
			MCU_P1_FMD = MCU_P1_FMD|BIT3;
			MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT7);
			MCU_P2_FMD = MCU_P2_FMD|(BIT4|BIT7);
	#elif (MainBoardType==MainBoard_715G5265_T)	//petit 20121030
			MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT6|BIT7);//0;  
			MCU_P0_FMD=MCU_P0_FMD|(BIT6|BIT7);//0;
			MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT2);//MCU_P1_OUT_EN|(BIT7);
			MCU_P1_FMD=MCU_P1_FMD|(BIT2|BIT3);//MCU_P1_FMD|(BIT7); 
			MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT4|BIT7);
			MCU_P2_FMD =  MCU_P2_FMD|(BIT4|BIT7);
	#elif (MainBoardType==MainBoard_3834_18T)	//petit 20121228
		#ifdef ScanKey
		P0 = 0xFB;
		#else
		MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT6|BIT7);
		MCU_P0_FMD = MCU_P0_FMD|(BIT6|BIT7);
		#endif
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT7);
		MCU_P1_FMD=MCU_P1_FMD|(BIT7); 
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT2|BIT4|BIT7);
		MCU_P2_FMD = MCU_P2_FMD|(BIT2|BIT4|BIT7);
		MCU_P4_OUT_EN=MCU_P4_OUT_EN|(BIT0|BIT2);
		MCU_P4_FMD=MCU_P4_FMD|(BIT0|BIT2);
	#elif (MainBoardType==MainBoard_715G6952_M0B)	//Gino 20140522
		#ifdef ScanKey
		P0 = 0xFB;
		#else
		MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT6|BIT7);
		MCU_P0_FMD = MCU_P0_FMD|(BIT6|BIT7);
		#endif
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT3);
		MCU_P1_FMD=MCU_P1_FMD|(BIT3); 
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT2|BIT4|BIT7);
		MCU_P2_FMD = MCU_P2_FMD|(BIT2|BIT4|BIT7);
		MCU_P4_OUT_EN=MCU_P4_OUT_EN|(BIT0);
		MCU_P4_FMD=MCU_P4_FMD|(BIT0);
	#elif (MainBoardType==MainBoard_715G6912_M1A)	//Gino 20140522
		#ifdef ScanKey
		P0 = 0xFB;
		#else
		MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT6|BIT7);
		MCU_P0_FMD = MCU_P0_FMD|(BIT6|BIT7);
		#endif
		MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT3);
		MCU_P1_FMD=MCU_P1_FMD|(BIT3); 
		MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT2|BIT4|BIT7);
		MCU_P2_FMD = MCU_P2_FMD|(BIT2|BIT4|BIT7);
		MCU_P4_OUT_EN=MCU_P4_OUT_EN|(BIT0);
		MCU_P4_FMD=MCU_P4_FMD|(BIT0);
	#elif (MainBoardType==MainBoard_715G3834)
	   	#if ScanKey
		P0 = 0xFB;
		#endif
     	#if ModelName==ENVISION_G2218_PLUS	
	    MCU_P0_ROD_EN= MCU_P0_ROD_EN|(BIT6|BIT7);
	    #else
        MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT6|BIT7);
        MCU_P0_FMD = MCU_P0_FMD|(BIT6|BIT7);
	    #endif
        MCU_P1_OUT_EN = MCU_P1_OUT_EN|(BIT2|BIT6);
        MCU_P1_FMD = MCU_P1_FMD|(BIT2|BIT6);
        MCU_P2_OUT_EN = MCU_P2_OUT_EN|(BIT2|BIT3|BIT4|BIT7);
        MCU_P2_FMD = MCU_P2_FMD|(BIT2|BIT3|BIT4|BIT7);
        MCU_P4_OUT_EN=MCU_P4_OUT_EN|(BIT0|BIT1|BIT2);
        MCU_P4_FMD=MCU_P4_FMD|(BIT0|BIT1|BIT2);
    #endif
	
   	GT_mode=0x80;
	
	#ifdef TSUMXXT		//110311 Modify
	#else
	#ifdef AOC_TSUM58BGJ
	INTENA1 = 0x02;
	INTINV1 = 0x02;
	INTCTRL1 = 0x01;
	INTSTA1 = 0;
	P2_4=1;
	INTENA1 = 0x02;
	INTINV1 = 0x02;
	INTCTRL1 = 0x01;
	INTSTA1 = 0;
	#else
	INTCTRL1 = 0x03;
	INTENA1 = 0x01;
        #endif
	#endif
        #if (MainBoardType!=MainBoard_715G2904_B&&MainBoardType!=MainBoard_2883_B_X_X_1)
       // hw_SetDDC_WP();
        #endif

	#if AudioFunc || ENABLE_HDMI
	hw_SetStandby();
	hw_SetMute();
	#endif
		
        Set_EEPROM_WP();
        hw_ClrBlacklit();
        hw_ClrPanel();
        hw_SetFlashWP();
        #if ScanKey
            #if Scan_Source == Synaptics
            Clr_ScanKey_RST();
            #else
            Clr_SK_LEDVcc();
            #endif

            #ifdef LEDCtrlByScalerIOPin
            hw_ClrAmberLed();
            hw_ClrGreenLed();
            #if MainBoardType==MainBoard_715G3289_C2 // prevent LED flash when AC on
            MCU_P0_OUT_EN |= (BIT6|BIT7);
            MCU_P0_FMD |= (BIT6|BIT7);
            #endif
            #endif
            
        #else
        hw_ClrAmberLed();
        hw_ClrGreenLed();
        #endif

#if IT6251
   SET_EDP_IT6251_RSTB();
#endif
		
        // 080514 for led flash when ac on
        // after turn off led then force output
	#if !ScanKey
	#if (MainBoardType == MainBoard_2883_1B||MainBoardType==MainBoard_715G3329_A2||\
	MainBoardType==MainBoard_715G3598_M0A)
	#ifdef TSUMXXNX
	MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT7|BIT6);
	MCU_P0_FMD=MCU_P0_FMD|(BIT7|BIT6);
	#else
	MCU_P0_OUT_EN=MCU_P0_OUT_EN|(BIT7);
	MCU_P0_FMD=MCU_P0_FMD|(BIT7);
	MCU_P1_OUT_EN=MCU_P1_OUT_EN|(BIT5);
	MCU_P1_FMD=MCU_P1_FMD|(BIT5);
	#endif
	#elif MainBoardType==MainBoard_715G4175_M0A
		MCU_P0_OUT_EN = MCU_P0_OUT_EN|(BIT6|BIT7);
		MCU_P0_FMD =  MCU_P0_FMD|(BIT6|BIT7);
	#endif
	#endif

    #endif
}
/* initialize 8051 CPU timer & interrupt routine */
/* TCON.7(   TF1): Timer 1 overflow flag */
/* TCON.6(   TR1): Timer 1 stop/start bit */
/* TCON.5(   TF0): Timer 0 overflow flag */
/* TCON.4(   TR0): Timer 0 stop/start bit */
/* TCON.3(   IE1): Timer 1 INT1 interrupt flag */
/* TCON.2(   IT1): Timer 1 INT1 interrupt style setup*/
/* TCON.1(   IE0): Timer 0 /INT0 interrupt flag */
/* TCON.0(   IT0): Timer 0 /INT0 interrupt style setup */
void Init_ExternalInterrupt(void)
{
    #if UseINT
        IT0 = 0; //1;
        IE0 = 0;
        PX0 = 1;
        EX0 = 1; // disable external interrupt 0
    #else
        IT0 = 0; //1;
        IE0 = 0;
        PX0 = 0;
        EX0 = 0; // disable external interrupt 0
    #endif
    IT1 = 1; // 0;//1; // set external interrupt 0 & 1 as falling edge trigger
    IE1 = 0; // interrupt flag
    EX1 = 1; // 0;  // enable external interrupt 1
    PX1 = 1;
}
void Init_Timer(void)
{
    WORD clock_period = ClockPeriod;
    /* -------------initialize Timer 0 -----------------------------*/
    ET0 = 1; // enable timer 0 interrupt
    TH0 = clock_period >> 8;
    TL0 = clock_period &0xFF; // timer 0 counter
    TF0 = 0; // timer 1 flag
    /* -------------initialize Timer 0 -----------------------------*/
    ET1 = 0; // disable timer 1 interrupt
    TH1 = 0;
    TL1 = 0; // timer 1 counter
    TF1 = 0; // timer 1 flag
    // setup Timer mode
    TMOD = 0x21; // set timer 1 as timer(1) , timer 0 as counter(5)
    // enable/disable timer
    TR0 = 1; // disable timer 0
    TR1 = 0; // disable timer 1
}
/*----------Timer 2 -------------------*/
/* T2CON.7(   TF2): overflow flag */
/* T2CON.6(  EXF2): extern enable flag */
/* T2CON.5(  RCLK): receive clock */
/* T2CON.4(  TCLK): transfer clock */
/* T2CON.3( EXEN2): extern enable flag */
/* T2CON.2(   TR2): stop/start timer 2 */
/* T2CON.1(  C_T2): intern clock(0)/extern counter(1) switch */
/* T2CON.0(CP_RL2): capture flag */
void Init_SerialPort(void)
{
    #if MST_MCU
        WORD br_cnt = BaudRatePeriod;
        PCON = 0x80; //Double RS232 rate
        SCON = 0x52;
        //TH1=252;// 9600
        TH1 = 240; // 9600
        TR1 = 1;
        RI = 0;
        TI = 0;
        #if DEBUG_ENABLE
            ES = 1; // 20051123 1;  //2006-06-27 1;
        #else
            ES = 0;
        #endif
    #else
        WORD br_cnt = BaudRatePeriod;
        T2CON = 0x00;
        //  RCAP2H= 0xFF;//BaudRatePeriod>>8;
        //  RCAP2L= 0xDC;//BaudRatePeriod&0xff;
        RCAP2H = br_cnt >> 8;
        RCAP2L = br_cnt &0xff;
        SCON = 0x50;
        T2CON = 0x34;
        RI = 0;
        TI = 0;
        ES = 0;
    #endif
}
#if 0
    // Winbond ISP
    void EnableISP(void)
    {
        TR0 = 0;
        CHPENR = 0x87;
        CHPENR = 0x59;
        CHPCON = 0x03;
        IP = 0x00;
        IP = 0x82;
        TL0 = 0xFE;
        TH0 = 0xFF;
        TR0 = 1; //
        PCON |= 0x01;
        while (1)
            ;
    }
#endif


void MCU_EnableCache(void)
{
#if Enable_Cache
	MCU_CACHE_CTL = 0x80;
	MCU_CACHE_CTL = 0x00;
	MCU_CACHE_CTL = 0x20;
#else
	MCU_CACHE_CTL = 0x00;//0x20;
#endif
}
#if Enable_Cache
void MCU_DisableCache(void)
{
	MCU_CACHE_CTL = 0x00;
}
#endif
