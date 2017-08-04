#ifndef _MSPM_H_
#define _MSPM_H_

#ifdef _MSPM_C_
  #define _MSPMDEC_
#else
  #define _MSPMDEC_ extern
#endif

typedef enum
{
  ePM_Standby,
  ePM_PowerOff,
  ePM_PowerON,
  ePM_Invalid
}ePM_Mode;

typedef enum
{
  ePMStatus_Non,
  ePMStatus_ADCActive,
  ePMStatus_DVIActive,
  ePMStatus_SARActive,
  ePMStatus_GPIOActive,
  ePMStatus_CECActive,
  ePMStatus_D2BActive,
	
  ePMStatus_Invaild,
  ePMStatus_ForcPowerOn
	
}ePMStatus;


//extern void msPM_Init(void); 
_MSPMDEC_ BYTE xdata Check,Check1;	
_MSPMDEC_ BYTE msPM_Reset(void);
_MSPMDEC_ void msPM_SetUp(BYTE ucMode);
_MSPMDEC_ void msPM_PowerUpMacro(void);
_MSPMDEC_ void msPM_WaitingEvent(void);
_MSPMDEC_ Bool GetPMStatus(BYTE* pucStatus);
_MSPMDEC_ void msPM_ClearStatus(void);
//_MSPMDEC_ void CheckPowerKey(void);

_MSPMDEC_ void ms_PM_Chcek_Sync(void);
_MSPMDEC_ void msPM_Check_Power_Status(void);
_MSPMDEC_ Bool msPM_Check_Not_Show_Logo(void);

#define ConvPercent(x)	( (WORD)(x)*255/100)
//=============================
// for user to set
//=============================
//#define AC2DC_PAD    	                            AC2DC_PAD_GPIOP25	/*BKC_64*/               /*BY HW SETTING */
//#define AC2DC_MODE    	                            VL_MODE	                     /*BKC_60*/               /*FW SETTING */  
//#define AC2DC_RCOSC_CLK_GATE    	       AC2DC_CLKOFF	       /*BKC_28*/               /*FW SETTING */ 
//#define PD_HSYNCCMP    	                            PD_HSYNCCMP_Enable	/*BKC_21*/               /*FW SETTING */ 
//#define DVIRAW_CLK_GATE    	                     DVIRAW_CLK_Enable    /*BKC_26*/               /*FW SETTING */ 
//#define DVI_SYNC_CLK_GATE    	              DVI_SYNC_CLK_Enable /*BKC_27*/               /*FW SETTING */ 
//#define DVICLK_CTLCLK_GATE    	              DVICLK_CTLCLK_Enable/*BKC_29*/               /*FW SETTING */ 
//#define Key_WkUp_EN    	                            P26_WkUp_EN	              /*BKC_34*/               /*BY HW SETTING */ 
//#define DVICLK_DET_SEL    	                     DVICLK_DET_Enable	/*BKC_41*/               /*FW SETTING */ 
//#define HVSYNC_SOG_DET_SEL    	              HVSYNC_SOG_DET    	/*BKC_44*/               /*FW SETTING */ 
//#define DVICLK_CTL_AMP    	                     DVICLK_CTL_EN |DVICLK_CTL_PERIOD           /*BKC_4A*/               /*BY HW SETTING */
//#define DVICLK_CTL_CHEN    	                     DVI0_EN                	/*BKC_4B*/               /*BY HW SETTING */
//#define OFF_PERIOD    	                            0x3E                        	/*BKC_62*/               /*BY HW SETTING */
//#define ACDET_THRD                                   ACDET_EN|Low_Volt_1_10|High_Volt_1_55     /*BKC_66*/               /*BY HW SETTING */

///*BKC_64*/=============================
#define AC2DC_PAD    	            AC2DC_PAD_Disable   //AC2DC_PAD_GPIOP25    /*BY HW SETTING */ 
#define AC2DC_PAD_Disable         0x00
#define AC2DC_PAD_GPIOP25       BIT0
#define AC2DC_PAD_GPIOP24       BIT1
///*BKC_60*/=============================
#define AC2DC_MODE    	            VL_MODE	   
#define OFF_MODE                         0x01//4
#define VL_MODE                            0xA4
#define VH_MODE                           0x91
#define VHL_MODE                          0xB5
// /*BKC_28*/=============================
#define AC2DC_RCOSC_CLKOFF      0xBB
#define AC2DC_CLKOFF                  0x0B           
#define RCOSC_CLKOFF                   0xB0
// /*BKC_21*/=============================
#define PD_HSYNCCMP    	             PD_HSYNCCMP_Enable
#define PD_HSYNCCMP_Disable      0x02
#define PD_HSYNCCMP_Enable       0x1E           
#define RCOSC_CLKOFF                   0xB0
// /*BKC_26*/=============================
#define DVIRAW_CLK_Enable          0x0B
#define DVIRAW_CLK_Disable         0x00
// /*BKC_27*/=============================
#define DVI_SYNC_CLK_Enable       0xBB
#define DVI_SYNC_CLK_Disable      0x00
// /*BKC_29*/=============================
#define DVICLK_CTLCLK_Enable      0x0B
#define DVICLK_CTLCLK_Disable     0x00
// /*BKC_34*/=============================
#define PKEY_WkUp_OFF                 0x00
#define P14_WkUp_EN                     BIT2
#define P26_WkUp_EN                     BIT1
#define SAR_WkUp_EN                     BIT0
#define Key_WkUp_EN    	               P14_WkUp_EN  //14_WkUp_EN   /*BY HW SETTING */ 
// /*BKC_41*/=============================
#define DVICLK_DET_Enable            0x01
#define DVICLK_DET_Disable           0x00
// /*BKC_44*/=============================
#define HVSYNC_SOG_DET                0xF0
// /*BKC_4A*/=============================
#define DVICLK_CTL_AMP_SEL          DVICLK_CTL_EN |/*DVICLK_CTL_AMPEN|*/DVICLK_CTL_PERIOD|DVICLK_CTL_ON_PERIOD    /*BY HW SETTING */ 
//------------------------------------------------
#define DVICLK_CTL_EN	                BIT7      
#define DVICLK_CTL_AMPEN	         BIT6                                 /*if DVD cant wakeup, try BIT6=1*/
#define DVICLK_CTL_PERIOD	         /*BIT5|*/BIT4|BIT3         /*Switching period =(period+1)X131.2mS*/
#define DVICLK_CTL_ON_PERIOD	  BIT2|BIT1|BIT0                /*ONperiod =(period)X8.2mS, setting 0 then turnOff AMP*/
//------------------------------------------------
// /*BKC_4B*/=============================
#define DVICLK_CTL_CHEN                /*DVI0_EN|*/DVI1_EN      /*BY HW SETTING */ 
#define DVI0_EN                                BIT0
#define DVI1_EN                                BIT1
// /*BKC_62*/=============================
#define OFF_PERIOD    	                0x2E                                    /* OFF period = (OFF_PERIOD+1)X32.8mS , should >1S */   /*BY HW SETTING */ 

// /*BKC_66*/=============================
#define ACDET_THRD                          ACDET_EN|Low_Volt_1_25|High_Volt_1_55    /*BY HW SETTING */ 
//------------------------------------------------
#define ACDET_EN                              BIT7
// ACDET low voltage threshold // 0x66
#define Low_Volt_1_20 0x00<<2
#define Low_Volt_1_25 0x01<<2
#define Low_Volt_1_15 0x10<<2
#define Low_Volt_1_10 0x11<<2
// ACDET high voltage threshold // 0x66
#define High_Volt_1_40 0x00
#define High_Volt_1_45 0x01
#define High_Volt_1_50 0x10
#define High_Volt_1_55 0x11

//=============================
#define SAR_Ch 			SAR2
//------------------------------------------------
#define SAR0	 0
#define SAR1	 1
#define SAR2	 2
#define SAR3	 3
//------------------------------------------------
//=============================

//#define LEDPWM  			LEDPWM1 // 091013 coding reserved
#define LED_PWM_EN 	  	LED_PWM_Amber

#define PWMDUTY_Val   	ConvPercent(2) //%
#define PWMDIV_Val      	0xFF // 97.16Hz
//=============================
//#define SAR1	 BIT0
//#define SAR2	 BIT1
//#define SARALL SAR2|SAR1
//=============================
#define LEDPWM0	0
#define LEDPWM1	1
#define LEDPWM2	2
#define LEDPWM3	3
#define LED_PWM_SEL_P07   	LEDPWM<<5
#define LED_PWM_SEL_P06   	LEDPWM<<3  // P06 & P15 as same
#define LED_PWM_SEL		LED_PWM_SEL_P07|LED_PWM_SEL_P06

// PWM0 0xC2,0xC3 ; PWM1 0xC4,0xC5
#if (LEDPWM==LEDPWM1)
#define PWMDIV      0xC4
#define PWMDUTY   0xC5
#elif (LEDPWM==LEDPWM0)
#define PWMDIV      0xC2
#define PWMDUTY   0xC3
#elif (LEDPWM==LEDPWM2)
#define PWMDIV      0xC6
#define PWMDUTY   0xC7
#elif (LEDPWM==LEDPWM3)
#define PWMDIV      0xC6
#define PWMDUTY   0xC7
#endif
//=============================
#if 0 // TSUMW18MR
#define LED_P06_EN              BIT2
#define LED_P07_EN              BIT1
#define LED_P15_EN              BIT0
#else // TSUMW58M
#define LED_P07_EN            BIT1
#define LED_P06_EN            BIT0
#define LED_P15_EN            BIT0
#endif

#define LED_PWM_Green  	    LED_P06_EN
#define LED_PWM_Amber	    LED_P07_EN

#endif
