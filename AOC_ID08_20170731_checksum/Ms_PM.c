///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    Ms_PM.c
/// @author MStar Semiconductor Inc.
/// @brief  PM Function
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <intrins.h>
#include <math.h>
#include <string.h>

#include "board.h"
#include "types.h"
#ifdef MS_PM
#define _MSPM_C_
#include "ms_reg.h"
#include "global.h"
#include "Misc.h"
#include "ms_rwreg.h"
#include "Ms_PM.h"
#include "Debug.h"
#include "Power.h"
#include "DDC2Bi.h"
#include "Userpref.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define XBYTE             ((unsigned char volatile xdata *) 0)

#define	RCOSC		    0
#define 	XTAL		    1
#define   XTALDIV2	    2
#define   XTALDIV4	    3
#define   XTALDOUBLE	    4

//--------PM Option----------------
/*Choice MCU clock when enter into PM_MODE*/
#define PM_CLOCK	            RCOSC    
/*Disable MCU or not when enter into PM_MODE*/
#define PM_MCUSLEEP		1
#define PM_USE_AC2DC		1
#define PM_DVICTRL			0

#define PM_U02				

#if PM_MCUSLEEP
    #if ScanKey
    #define PM_SAR_ANYKYE	0
    #else
    #define PM_SAR_ANYKYE	1
    #endif
#else
    #define PM_SAR_ANYKYE	0
#endif
#define PM_SAR		0
#define PM_SOG		0
#define PM_CEC		0
#define PM_MCCS		0
//#define POWER_KEY		P1_4

//---------------------------------
#define HSYNC_DET_0		BIT7
#define VSYNC_DET_0		BIT6
#define SOG_DET_0		BIT5
#define HV_DET_0		BIT4
#define HSYNC_DET_1		BIT3
#define VSYNC_DET_1		BIT2
#define SOG_DET_1		BIT1
#define HV_DET_1		BIT0

#define CEC_WAKEUP		BIT5
#define D2B_WAKEUP		BIT4
#define SAR_IN_DET		BIT3
#define GPIO_0_INT		BIT2
#define GPIO_1_INT		BIT1
#define DVI_CLK_DET		BIT0
//---------------------------------
#define VCP_SET		        0x03
#define VCP_DPMS 	        0xD6
#define VCP_DPMS_ON	 0x01
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

#define printMsg(x) printData(x,0)

#if defined(TSUMXXNX)
#define REGBANKPM         0x04
#elif defined(TSUMXXSX)
#define REGBANKPM         0x0C
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static XDATA BYTE ucPM_Status;
XDATA BYTE fCheckDPMS;
BYTE code tblPM_ON[][2]=
{
  {REGBK, REGBANK4},
  //{BK4_64, AC2DC_PAD_Disable},  	
  //{BK4_60, OFF_MODE}, 
  //{BK4_28, AC2DC_RCOSC_CLKOFF}, 
  {BK4_21, PD_HSYNCCMP_Disable},		
  {BK4_26, DVIRAW_CLK_Enable},	       /*DVI RAW clock gating*/
  {BK4_27, DVI_SYNC_CLK_Enable},           /*DVI clock gating, Sync clock gating*/
  {BK4_29, DVICLK_CTLCLK_Enable},         
  {BK4_34, PKEY_WkUp_OFF},                    /*disable SAR detect*/
  {BK4_41, DVICLK_DET_Disable},             /*disable DVI0 detect*/
  {BK4_44, 0x00},                                     /*disable Hsync & Vsync detect*/
  {BK4_4A, 0x00}, 
  {BK4_4B, 0x00}, 
  //{BK4_62, 0x0E},  
  //{BK4_66, ACDET_EN},  
  {BK4_24, 0x00},
  {BK4_25, 0x00},
};

BYTE code tblPM_OFF[][2]=
{
  {REGBK, REGBANK4},
  {BK4_21, PD_HSYNCCMP|BIT7},
  
  {BK4_26, DVIRAW_CLK_Enable},                //DVI RAW clock pass
  {BK4_27, DVI_SYNC_CLK_Enable},             //DVI clock pass, Sync clock pass
  {BK4_28, AC2DC_CLKOFF}, 
  {BK4_29, DVICLK_CTLCLK_Enable},            //enable GPIO/SAR detect
  {BK4_34, Key_WkUp_EN}, 
  
  #ifdef PM_U02
   {BK4_35, 0x00},  //{BK4_35, 0x02},  	                                     //enable GPIO P13 detect
  //{BK4_C1, 0x01},  	                              //Select the method to delay the wakeup time to enable MCU and XTAL
  #endif	
  
  {BK4_41, DVICLK_DET_Disable},  	
  {BK4_44, 0x00},  	
  {BK4_4A, 0x00},  	
  {BK4_4B, DVICLK_CTL_CHEN},  	
  //{BK4_60, AC2DC_MODE},  	
  //{BK4_62, OFF_PERIOD},  	
  //{BK4_64, AC2DC_PAD},  	
  //{BK4_66, ACDET_THRD},  
  {BK4_24, 0x55},     //PM_PASSWORD
  {BK4_25, 0xAA},     //PM_PASSWORD
  
  {BK4_22, 0x02}
};
//for power test - enable Sync ,DVI,GPIO detect
BYTE code tblPM_SB[][2]=
{
  {REGBK, REGBANK4},
  {BK4_21, PD_HSYNCCMP},  
  
  {BK4_26, DVIRAW_CLK_Disable},                   /*DVI RAW clock pass*/
  {BK4_27, DVI_SYNC_CLK_Disable},	             /*DVI clock pass, Sync clock pass*/
  {BK4_28, AC2DC_CLKOFF}, 
  {BK4_29, DVICLK_CTLCLK_Disable},              /*enable GPIO/SAR detect */
  {BK4_34, Key_WkUp_EN},  	                           /*enable GPIO/SAR detect*/
  
  #ifdef PM_U02
  {BK4_35, 0x00},   //{BK4_35, 0x02},  	                                        /*enable GPIO P13 detect */
  //{BK4_C1, 0x01},  	                                 /*Select the method to delay the wakeup time to enable MCU and XTAL */
  #endif	
  
  {BK4_41, DVICLK_DET_Enable}, 
  {BK4_44, HVSYNC_SOG_DET},  
  
  #if PM_DVICTRL	
  {BK4_4A, DVICLK_CTL_AMP_SEL},  	
  {BK4_4B, DVICLK_CTL_CHEN},  	
  #else
  {BK4_4A, 0x00},  	
  {BK4_4B, 0x00},  	
  #endif	
  
  //{BK4_60, AC2DC_MODE}, 
  //{BK4_62, OFF_PERIOD},  
  //{BK4_64, AC2DC_PAD},  	
  //{BK4_66, ACDET_THRD},  
  {BK4_24, 0x55},
  {BK4_25, 0xAA},
  
  {BK4_22, 0x02}
};

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  External Variables
//-------------------------------------------------------------------------------------------------
#if !DEBUG_ENABLE
//extern BYTE xdata rxIndex;
//extern BYTE xdata DDCBuffer[MaxBufLen];
#endif
//-------------------------------------------------------------------------------------------------
//  External Functions
//-------------------------------------------------------------------------------------------------
extern Bool PowerOnSystem(void);

//**************************************************************************
//  [Function Name]:
//                  DDCCI_CheckDMPSON()
//  [Description]
//                  DDCCI_CheckDMPSON
//  [Arguments]:
//
//  [Return]:
// DMPS Power On: 6E 51 84 03 D6 00 01 CkSum
// DMPS Power Off(standby): 6E 51 84 03 D6 00 02 CkSum
// DMPS Power Off(suspend) : 6E 51 84 03 D6 00 03 CkSum
// DMPS Power Off: 6E 51 84 03 D6 00 04 CkSum
// Power Off: 6E 51 84 03 D6 00 05 CkSum

//************************************************************************** 
#if PM_MCCS
BYTE code MCCS_DPMSON[6] ={0x51, 0x84, 0x03, 0xD6, 0x00, 0x01};
	
Bool DDCCI_CheckDMPSON(void)
{
   if(rxIndex >= 5)
   {
	if(memcmp(MCCS_DPMSON, DDCBuffer, 6) == 0)
	{
		memset(DDCBuffer, 0, 6);
		rxIndex=0;
		fCheckDPMS=0;
		return TRUE;
	}
   }

   return FALSE;
}
#endif

//**************************************************************************
//  [Function Name]:
//                  msPM_PowerDownMacro()
//  [Description]
//                  msPM_PowerDownMacro
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void msPM_PowerDownMacro(BYTE ucMode)
{
	//BYTE ucBank;
	//ucBank = mStar_ReadByte(REGBK);
	mStar_WriteByte(REGBK, REGBANK1);
	mStar_WriteByte(BK1_20, 0x80);
	msWriteByteMask(BK1_E2, BIT7, BIT7);
	msWriteByteMask(BK1_5C, BIT5|BIT4, BIT5|BIT4);

	msWriteByteMask(BK1_CF, BIT0, BIT0);	
#if !PM_DVICTRL	
	/*enable DVI A/B T-term & clock */
	//mStar_WriteByte(BK1_E4, 0x11);        /*Only enable DVI0*/
	mStar_WriteByte(BK1_E4, 0x14);       /*enable DVI0/DVI1*/
	mStar_WriteByte(BK1_26, 0x01);
	msWriteByteMask(BK1_C9, 0, BIT7);
#endif

	mStar_WriteByte(BK1_60, 0x60);	//Power down SAR

	//mStar_WriteByte(BK1_A6, 0x80);   //Scar 20090701
	mStar_WriteByte(BK1_20, 0x00);
    
#if 0     /*Reverse for DVI cant wakeup with no-signal cable*/ 
        //20090330, DVI cable plug in without DVI CLK , then cant enter into PM_MODE
        /*Bank1 0xCF[0] = 0, link A clock and data r-term disconnected by Bank1 0xE4[0]=1*/         
	msWriteByteMask(BK1_CF, 0, BIT0);   
	mStar_WriteByte(BK1_E4, 0x03);
#endif

	//mStar_WriteByte(REGBK, REGBANK5);
	//msWriteByteMask(BK5_1F, BIT6, BIT6);
	//msWriteByteMask(BK5_F5, BIT0, BIT0);
	
	mStar_WriteByte(REGBK, REGBANK0);
	mStar_WriteByte(BK0_F4, 0x7F);
	msWriteByteMask(BK0_B4, 0, BIT6);	//Disable DDC filter 

	//mStar_WriteByte(REGBK, ucBank);

	if(ucMode == ePM_PowerOff)
	{
		MCU_P0_OUT_EN = 0x00;
		MCU_P0_FMD = 0x00;
	}
	else
        {
		MCU_P0_OUT_EN = 0x00;
		MCU_P0_FMD = 0x00;            
	}
	
	MCU_P1_OUT_EN = 0x00;
	MCU_P1_FMD = 0x00;
	MCU_P2_OUT_EN = 0x00;
	MCU_P2_FMD = 0x00;

	//hw_Set_HdcpHpd();
	//hw_Set_HdcpHpd1();

}
//**************************************************************************
//  [Function Name]:
//                  msPM_PowerUpMacro()
//  [Description]
//                  msPM_PowerUpMacro
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
extern void Init_IOPorts( void );
void msPM_PowerUpMacro(void)
{
	mStar_WriteByte(REGBK, REGBANK1);
	mStar_WriteByte(BK1_20, 0x80);
	msWriteByteMask(BK1_E2, 0, BIT7);
	msWriteByteMask(BK1_5C, 0, BIT5|BIT4);
	//mStar_WriteByte(BK1_A6, 0x00);       //Scar 20090701
	msWriteByteMask(BK1_CF, 0, BIT0);	
	mStar_WriteByte(BK1_20, 0x00);
	mStar_WriteByte(BK1_60, 0x20);
    
	//mStar_WriteByte(REGBK, REGBANK5);
	//msWriteByteMask(BK5_1F, 0, BIT6);
	//msWriteByteMask(BK5_F5, 0, BIT0);
	
	mStar_WriteByte(REGBK, REGBANK0);
	mStar_WriteByte(BK0_F4, 0x00);
    
    #if !PM_MCUSLEEP	
        Init_IOPorts();
    #endif	
	mStar_WriteByte(REGBK, REGBANK0);

}

//**************************************************************************
//  [Function Name]:
//                  StartRCOSCCal()
//  [Description]
//                  StartRCOSCCal
//  [Arguments]:
//
//  [Return]:
//  RCOSC = XTAL * Counter / 512 => Counter = RCOSC *512/XTAL = 143 =>8Fh
//************************************************************************** 
//#define RCOSC_TARGET	143	//4MHz
#define RCOSC_MAX		

Bool StartRCOSCCal(void)
{
    BYTE ucBank;
    //WORD ucCounter;
    BYTE ucTemp;
    //BYTE i;
    BYTE ucSetOld = 0x08;
    BYTE ucSetNow = 0x08;
    int iDeltaOld =100;
    int iDeltaNew = 0;
	
    ucBank = mStar_ReadByte(REGBK);
    mStar_WriteByte(REGBK, REGBANK4);
    mStar_WriteByte(BK4_28, RCOSC_CLKOFF);
    ucTemp = mStar_ReadByte(BK4_20)&0x07;
    #ifdef RCOSC_MAX
    ucSetNow = 0x00;//0x1F;
    mStar_WriteByte(BK4_20, (ucSetNow<<3)|ucTemp);
    #else
    for ( i=0; i< 31; i++)
    {
        mStar_WriteByte(BK4_20, (ucSetNow<<3)|ucTemp);
        mStar_WriteByte(BK4_4C, 0x80);
        mStar_WriteByte(BK4_4C, 0x03);
        while(!mStar_ReadByte(BK4_4F)&BIT4)
        {			
        }			
        ucCounter = mStar_ReadWord(BK4_4F)&0x03FF;
        iDeltaNew = (int)ucCounter - RCOSC_TARGET;
        if(abs(iDeltaNew) < abs(iDeltaOld))
        {
            ucSetOld = ucSetNow;
            if( ucCounter > RCOSC_TARGET)
            {
                ucSetNow--;
            }
            else 
            {
                ucSetNow++;
            }
            iDeltaOld = iDeltaNew;
        //printData("RCOSC Set(%x)", ucSetNow);
        }
        else
        {
            mStar_WriteByte(BK4_20, (ucSetOld<<3)|ucTemp);
            //printData("RCOSC Set(%x)", ucSetOld);
            mStar_WriteByte(BK4_4C, 0x00);
            mStar_WriteByte(BK4_28, AC2DC_RCOSC_CLKOFF);
            mStar_WriteByte(REGBK, ucBank);
            return TRUE;
        }
		
    }
    #endif	
    mStar_WriteByte(BK4_28, AC2DC_RCOSC_CLKOFF);
    mStar_WriteByte(REGBK, ucBank);
    return FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  msPM_EnableAC2DC()
//  [Description]
//                  msPM_EnableAC2DC
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void msPM_EnableAC2DC(Bool bEnable)
{
    mStar_WriteByte(REGBK, REGBANK4);
    if(bEnable)   /*Turn On PM*/
    {
        msWriteByteMask(BK4_28, 0x00, 0xF0);	      /*AC2DC controler clock bypass*/
        mStar_WriteByte(BK4_60, AC2DC_MODE);		      /*AC2DC controler enable		*/
        mStar_WriteByte(BK4_62, OFF_PERIOD);		      /*AC2DC Off period*/
        mStar_WriteByte(BK4_64, AC2DC_PAD); 	             /*AC2DC pad function enable		*/        
        mStar_WriteByte(BK4_66, 0x84);		                    /*AC detect voltage	*/
    }
    else              /*Turn Off PM*/
    {
        mStar_WriteByte(BK4_64, 0x00);		                    /*AC2DC pad function disable*/
        mStar_WriteByte(BK4_60, OFF_MODE);		             /*AC2DC controler disable*/
        mStar_WriteByte(BK4_62, 0x00);
        mStar_WriteByte(BK4_66, 0x00);				      /*Off period*/
        msWriteByteMask(BK4_28, 0xB0, 0xF0);	      /*AC2DC controler clock gating*/
    }

    mStar_WriteByte(REGBK, REGBANK0);
}
//**************************************************************************
//  [Function Name]:
//                  GetPMStatus()
//  [Description]
//                  GetPMStatus
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
Bool GetPMStatus(BYTE* pucStatus)
{
	BYTE ucBank;
	Bool bResault = FALSE;
	BYTE ucStatus_3E;
	BYTE ucStatus_3F;
    #ifdef PM_U02
	BYTE ucStatus_35;
    #endif
    
	*pucStatus = ePMStatus_Non;
	
	ucBank = mStar_ReadByte(REGBK);
	mStar_WriteByte(REGBK, REGBANK4);
	
	ucStatus_3E = mStar_ReadByte(BK4_3E);
	ucStatus_3F = mStar_ReadByte(BK4_3F);
    
    #if DEBUG_PM
    	printData("\r\nucStatus_3E (%x) !", ucStatus_3E);
	printData("\r\ucStatus_3F (%x) !", ucStatus_3F);
        #ifdef PM_U02
            printData("\r\ucStatus_35 (%x) !", ucStatus_35);
        #endif
    #endif

    #ifdef PM_U02
	ucStatus_35 = mStar_ReadByte(BK4_35);
	if(ucStatus_35 &( BIT5|BIT4))
	{
		*pucStatus = ePMStatus_GPIOActive;
		bResault = TRUE;
	}
    #endif
    
	if(ucStatus_3E & (HSYNC_DET_0|VSYNC_DET_0|SOG_DET_0|HV_DET_0))
	{
		*pucStatus = ePMStatus_ADCActive;
		bResault = TRUE;
	}
	else 
        {
		switch(ucStatus_3F)
		{
			case CEC_WAKEUP:
				*pucStatus = ePMStatus_CECActive;
				bResault = TRUE;
				break;
			case D2B_WAKEUP:
				*pucStatus = ePMStatus_D2BActive;
				bResault = TRUE;			
				break;
			case SAR_IN_DET:
				*pucStatus = ePMStatus_SARActive;
				bResault = TRUE;
				break;
			case GPIO_0_INT:
			case GPIO_1_INT:
				*pucStatus = ePMStatus_GPIOActive;
				bResault = TRUE;
				break;	
			case DVI_CLK_DET:
				*pucStatus = ePMStatus_DVIActive;
				bResault = TRUE;
				break;				
		}
	}	
	mStar_WriteByte(REGBK, ucBank);
    #if DEBUG_PM
       printData(" CheckAAAAAA %d \n",*pucStatus);
    #endif
	return  bResault;/* *pucStatus*/ /*ucStatus_3F*//*bResault*/
}

//**************************************************************************
//  [Function Name]:
//                  EnableSAREvent()
//  [Description]
//                  EnableSAREvent
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
#define SAR_33			1<<7
#define SAR_18			0<<7
#define SAR_VOLTAGE			SAR_18

void EnableSAREvent(Bool benable, Bool bAnyKey)
{
    if(benable)
    {
        if(bAnyKey)
        {
            mStar_WriteByte(REGBK, REGBANK1);	
            mStar_WriteByte(BK1_64, SAR_VOLTAGE|BIT6);	
            mStar_WriteByte(REGBK, REGBANK4);				
            mStar_WriteByte(BK4_C0, 0xF0);	                    //SAR_PM Setting
            msWriteByteMask(BK4_34, BIT0, BIT0);	    //ENABLE SAR Detect
        }
        else
        {
            mStar_WriteByte(REGBK, REGBANK1);	
            mStar_WriteByte(BK1_64, SAR_VOLTAGE);	
            mStar_WriteByte(REGBK, REGBANK4);	
            msWriteByteMask(BK4_34, BIT0, BIT0);	
        }		
    }
    else 
    {
        mStar_WriteByte(REGBK, REGBANK4);	
        msWriteByteMask(BK4_34, 0, BIT0);				
        mStar_WriteByte(BK4_C0, 0x00);	
    }
}
//**************************************************************************
//  [Function Name]:
//                  msPM_MCCSReset()
//  [Description]
//                  msPM_MCCSReset
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void msPM_MCCSReset(void)
{
    #ifdef TSUMXXNX
    BYTE ucBank;

    ucBank=mStar_ReadByte(REGBK);
    mStar_WriteByte(REGBK, REGBANKSCALER);
    mStar_WriteByte(BK0_B3,mStar_ReadByte(BK0_B3)|(BIT0|BIT1));
    mStar_WriteByte(BK0_B3,mStar_ReadByte(BK0_B3)&(~BIT0|BIT1)); //Clear DDC2BI D6 Comamnd 04,05  flag

    mStar_WriteByte(BK0_BA,mStar_ReadByte(BK0_BA)|(BIT6));
    mStar_WriteByte(BK0_BA,mStar_ReadByte(BK0_BA)&(~(BIT6)));//Clear DDC2BI wake up flag.
    mStar_WriteByte(REGBK, ucBank);
    #else // for TSUMXXMX
    XBYTE[0xc1B0] = XBYTE[0xc1b0] |(BIT4|BIT3);         //ADC_o
    XBYTE[0xc1B0] = XBYTE[0xc1b0] &(~(BIT4|BIT3));    //ADC_o
    XBYTE[0xc1B2] = XBYTE[0xc1b2] |(BIT4|BIT3);
    XBYTE[0xc1B2] = XBYTE[0xc1b2] &(~(BIT4|BIT3));
    XBYTE[0xc1B3] = XBYTE[0xc1b3] |(BIT4|BIT3);
    XBYTE[0xc1B3] = XBYTE[0xc1b3] &(~(BIT4|BIT3));

    XBYTE[0xc1B4] =0x80;
    XBYTE[0xc1B4] =0x00;        
    #endif

    //XBYTE[0xc1B0] = XBYTE[0xc1b0] |(BIT4|BIT3);
    //XBYTE[0xc1B0] = XBYTE[0xc1b0] &(~(BIT4|BIT3));

    //XBYTE[0xc1B4] =0x80;
    //XBYTE[0xc1B4] =0x00;
}
//**************************************************************************
//  [Function Name]:
//                  EnableMCCSPM()
//  [Description]
//                  EnableMCCSPM
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void EnableMCCSPM(Bool benable)
{
    //BYTE ucloop;
    
    if(benable)
    {
        //mStar_WriteByte(REGBK, REGBANK0);	
        //mStar_WriteByte(BK0_B6, 0x00);	
        //for(ucloop=0; ucloop < 20; ucloop++)
            //mStar_WriteByte(BK0_B7, 0x00);	
        //mStar_WriteByte(BK0_BA, 0x00);	
        //for(ucloop=0; ucloop < 20; ucloop++)
            //mStar_WriteByte(BK0_BB, 0x00);	
		
        XBYTE[0xc019] = 0xB7;
        XBYTE[0xc01A] = 0xB7;
        XBYTE[0xc07A] = 0xB7;

        #ifdef TSUMXXNX
        mStar_WriteByte(REGBK, REGBANKSCALER);
        mStar_WriteByte(BK0_B3,mStar_ReadByte(BK0_B3)|(BIT4|BIT3));//MCCS wake up power key 05 Enable , MCCS wake up function Enable
        #else
        XBYTE[0xc1B0] = 0xE0;
        XBYTE[0xc1B2] = 0xE0;
        XBYTE[0xc1B3] = 0xE0;

        XBYTE[0xc1B5] = 0xC0;
        #endif
    }
    else
    {
        #ifdef TSUMXXNX
        mStar_WriteByte(BK0_B3,0x00);
        #else
        XBYTE[0xc1b0] = 0x00;
        XBYTE[0xc1b5] = 0x00;
        #endif
    }
}
//**************************************************************************
//  [Function Name]:
//                  SetPMClock()
//  [Description]
//                  SetPMClock
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void SetPMClock(void)
{	
    BYTE ucStatus;
    // 091013
    //#if ScalerType==TSUMU58N
    #ifdef TSUMXXNX
        mStar_WriteByte(REGBK, REGBANK1);		
        #if PM_CLOCK == RCOSC
        
        #if DEBUG_PM
        printMsg(" PM Clock =>RCOSC \n");
        #endif
        msWriteByteMask(BK1_66, BIT0       ,  BIT0);//B.14.4 Scar,20090424
        ForceDelay1ms ( 1 );	            
        msWriteByteMask(BK1_67, BIT1|BIT0  ,  BIT1|BIT0);
        ForceDelay1ms ( 1 );		
        msWriteByteMask(BK1_67, BIT5|BIT4  ,  BIT5|BIT4);
        ForceDelay1ms ( 1 );		
        msWriteByteMask(BK1_66, 0	          ,  BIT0);

        #elif PM_CLOCK == XTALDIV4
        #if DEBUG_PM
        printMsg(" PM Clock =>XTALDIV4 \n");
        #endif
        msWriteByteMask(BK1_67, BIT1           ,  BIT1|BIT0);
        ForceDelay1ms ( 1 );		
        msWriteByteMask(BK1_67, BIT5|BIT4  ,  BIT5|BIT4);
        ForceDelay1ms ( 1 );		
        msWriteByteMask(BK1_66, 0	          ,  BIT0);
            
        #elif PM_CLOCK == XTALDIV2
        #if DEBUG_PM
        printMsg(" PM Clock =>XTALDIV2 \n");
        #endif
        msWriteByteMask(BK1_67,         BIT0  ,  BIT1|BIT0);
        ForceDelay1ms ( 1 );		
        msWriteByteMask(BK1_67, BIT5|BIT4  ,  BIT5|BIT4);
        ForceDelay1ms ( 1 );		
        msWriteByteMask(BK1_66, 0	          ,  BIT0);
        #elif PM_CLOCK==XTALDOUBLE
        
        #else
        #if DEBUG_PM
        printMsg(" PM Clock =>XTAL \n");
        #endif
        msWriteByteMask(BK1_66, BIT0		,  BIT0);
        ForceDelay1ms ( 1 );
        #endif

    #else
	mStar_WriteByte(REGBK, REGBANK1);		
	#if PM_CLOCK == RCOSC
       //printMsg(" PM Clock =>RCOSC ");
	mStar_WriteByte(BK1_67, 0xC3);	// power down MCU PLL
	#elif PM_CLOCK == XTALDIV4
	//printMsg(" PM Clock =>XTALDIV4 ");	
	mStar_WriteByte(BK1_67, 0x82);
	#elif PM_CLOCK == XTALDIV2
	//printMsg(" PM Clock =>XTALDIV2 ");	
	mStar_WriteByte(BK1_67, 0x81);
	#else
	//printMsg(" PM Clock =>XTAL ");	
	mStar_WriteByte(BK1_67, 0x80);
	#endif	//PM_CLOCK
    #endif

    #if PM_USE_AC2DC
    msPM_EnableAC2DC(TRUE);
    #endif
    

    /*Set power down mode*/
    mStar_WriteByte(REGBK, REGBANK0);
    msWriteByteMask(BK0_F0, 0, BIT1|BIT0);
    mStar_WriteByte(SWRST, 0x01);
    mStar_WriteByte(SWRST, 0x00);
    msWriteByteMask(BK0_F0, BIT1|BIT0, BIT1|BIT0);
	
    #if PM_CLOCK == RCOSC
    mStar_WriteByte(REGBK, REGBANK4);
    msWriteByteMask(BK4_21, 0, BIT1);       /*disable XTAL*/
    #endif

    /*Set deep power down mode*/
    mStar_WriteByte(REGBK, REGBANK0);
    mStar_WriteByte(SWRST, 0x80);
	
    if(GetPMStatus(&ucStatus))
    {
        #if 0//DEBUG_PM
        printData("\r\nPM Wakeup Event (%d) !", ucStatus);
        #endif
        msPM_ClearStatus();
    }
    
    #if PM_MCUSLEEP
    #if 0//Enable_Cache
    MCU_CACHE_CTL = 0x00;
    #endif
    #if DEBUG_PM
    printData(" PM_MCUSLEEP",1);
    #endif
    _nop_();
    _nop_();
    _nop_();
    /*Set MCU sleep mode*/
    mStar_WriteByte(REGBK, REGBANK1);
    mStar_WriteByte(BK1_BA, 0x00);
    mStar_WriteByte(BK1_BA, 0x55);
    mStar_WriteByte(REGBK, REGBANK0);
    #endif//PM_MCUSLEEP
	
}

//**************************************************************************
//  [Function Name]:
//                  msPM_Init()
//  [Description]
//                  msPM_Init
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
#if 0
void msPM_Init(void)
{
	BYTE  i;
	for (i=0; i<sizeof(tblPM_INIT)/2; i++)
  		mStar_WriteByte(tblPM_INIT[i][0], tblPM_INIT[i][1]);

	EnableMCCSPM(TRUE);
   #if 0//LED_PWM // 090226
   	mStar_WriteByte(REGBK, REGBANK1);
	msWriteByteMask(BK1_79, BIT5, BIT5);   
	//mStar_WriteByte(BK1_98, LED_PWM_SEL_P07|LED_PWM_SEL_P06|LED_PWM_MODE);
	mStar_WriteByte(BK1_98, LED_PWM_SEL_P07|LED_PWM_SEL_P06|LED_PWM_Amber);   
	mStar_WriteByte(REGBK, REGBANK0);
 	mStar_WriteByte(PWMDUTY, 0xFF);   	
   #endif
}
#endif
//**************************************************************************
//  [Function Name]:
//                  msPM_Init()
//  [Description]
//                  msPM_Init
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void msPM_ClearStatus(void)
{
	mStar_WriteByte(REGBK, REGBANK4);

	//Clear Flag
	mStar_WriteByte(BK4_3F, BIT6);
	mStar_WriteByte(BK4_3F, 0);
	//PM Reset
	mStar_WriteByte(BK4_22, 0x03);
	mStar_WriteByte(BK4_22, 0x02);
	
	mStar_WriteByte(REGBK, REGBANK0);
	
}
//**************************************************************************
//  [Function Name]:
//                  msPM_Reset()
//  [Description]
//                  msPM_Reset
//  [Arguments]:
//
//  [Return]:
//		PM status
//
//************************************************************************** 
BYTE msPM_Reset(void)
{
	BYTE ucStatus = ePMStatus_Invaild;

	msPM_EnableAC2DC(FALSE);
    
    #if 0//DEBUG_PM	
	if(GetPMStatus(&ucStatus))
		printData("\r\nPM Wakeup Event (%d) !", ucStatus);
    #endif
    
	msPM_SetUp(ePM_PowerON);
    
	mStar_WriteByte(REGBK, REGBANK4);
	mStar_WriteByte(BK4_34, 0x00);	        /*disable GPIO detect*/
	mStar_WriteByte(BK4_35, 0x00);	        /*disable GPIO detect*/
    
	mStar_WriteByte(BK4_21, 0x02);	        /*Enable XTAL*/
	mStar_WriteByte(REGBK, REGBANKADC);
	mStar_WriteByte(BK1_67, 0x00);            /* Selcet XTAL*/
	//mStar_WriteByte(BK1_A6, 0x00);            /* */
    
	mStar_WriteByte(BK1_BA, 0x00);           /*Disable MCU Power down*/
    
    mStar_WriteByte ( REGBK, REGBANKSCALER );    

	//msPM_PowerUpMacro();

	msPM_ClearStatus();
	msPM_MCCSReset();

	#ifdef PM_U02				
            EnableSAREvent(FALSE, FALSE);	
	#endif
    
      #if DEBUG_PM
            printData("Enter PM ePM_PowerON mode!",0);
      #endif

	return ucStatus;
}
/*
void msPM_InterruptEnable(Bool benable)
{
	if(benable)
	{
		//ET0 =0;
		//ET1 =0;
		
	}
	else
       {
		//ET0 = 1;
		//ET1 = 1;

	}
}

//**************************************************************************
//  [Function Name]:
//                  msPM_WaitingEvent()
//  [Description]
//                  msPM_WaitingEvent
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
#define SAR_KEY_TH		0x30
void msPM_WaitingEvent(void)
{
	BYTE ucStatus =0;
	
	msPM_InterruptEnable(FALSE);
    	msPM_ClearStatus(); 
    #if DEBUG_PM
	printMsg("msPM_WaitingEvent.....");
    #endif    
	while((!GetPMStatus(&ucStatus)))
	{
		#if EnableWDT
			ResetWDTimer();
		#endif
		
		#if !DEBUG_PM
		if(DDCCI_CheckDMPSON())
			break;
		#endif	
	
		#if PM_SAR_ANYKYE
		if((KEYPAD_ADC2 < SAR_KEY_TH) ||(KEYPAD_ADC3 < SAR_KEY_TH))
			break;
		#endif
         if(ES)
         {
            #if DEBUG_PM
            	printData("\r\nucStatus_3E (%x) !", mStar_ReadByte(BK4_3E));
        	printData("\r\ucStatus_3F (%x) !", mStar_ReadByte(BK4_3F));
            #endif
          DebugHandler();	
         }
	}
    #if DEBUG_PM
      	printData("\r\nPM Wakeup Event1 (%d) !", ucStatus);
    #endif
	msPM_Reset();	
	mStar_WriteByte(REGBK, REGBANK0);
	PowerOnSystem();
	msPM_PowerUpMacro();
	
	mStar_WriteByte(REGBK, REGBANK1);
	msWriteByteMask(BK1_66, BIT0, BIT0);
	mStar_WriteByte(REGBK, REGBANK0);
	
	msPM_InterruptEnable(TRUE);
}
*/
//**************************************************************************
//  [Function Name]:
//                  msPM_SetUp()
//  [Description]
//                  msPM_SetUp
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void msPM_SetUp(BYTE ucMode)
{
	BYTE i;
		
	switch(ucMode)
	{
		case ePM_Standby:
			ucPM_Status = ePM_Standby;            
                    #if DEBUG_PM
			    printMsg("Enter PM Standby mode!");
                    #endif
            
			mStar_WriteByte(REGBK, REGBANK1);				
			#if PM_SAR_ANYKYE
				msWriteByteMask(BK1_60, 0x20, 0x33);
			#endif
            
            
			#if PM_SOG
        			msWriteByteMask(BK1_CF, BIT5, BIT5);    /*OVRD_PDNADCVREF_EN For SOG offline detection in PM mode.*/
        			msWriteByteMask(BK1_E2, BIT7, BIT7);			
			#endif
            
			msWriteByteMask(BK1_66, 0, BIT0);
            
			#if PM_MCCS
			    EnableMCCSPM(TRUE);
			#endif
            
			msPM_PowerDownMacro(ePM_Standby);
            
			if(!StartRCOSCCal())
                    #if DEBUG_PM                
				printMsg("CALIBARYION RCOSC FAIL!");
                    #else
                            ;
                    #endif

			for (i=0; i<sizeof(tblPM_SB)/2; i++)
		  		mStar_WriteByte(tblPM_SB[i][0], tblPM_SB[i][1]);

			#ifdef PM_U02				
                        #if PM_SAR
				EnableSAREvent(TRUE, TRUE);
                        #else
				EnableSAREvent(FALSE, FALSE);
                        #endif
			#endif
                     
			SetPMClock();
			break;
			
		case ePM_PowerOff:
			ucPM_Status = ePM_PowerOff;			            
                    //printData("Enter PM PowerOff mode!",0);

			msPM_PowerDownMacro(ePM_PowerOff);
			if(!StartRCOSCCal())
                        #if DEBUG_PM                
				printMsg("CALIBARYION RCOSC FAIL!");
                        #else
                            ;
                        #endif
			
			mStar_WriteByte(REGBK, REGBANK1);
			//mStar_WriteByte(BK1_E4, 0x03);			
			msWriteByteMask(BK1_66, 0, BIT0);
			#if PM_MCCS
			    EnableMCCSPM(TRUE);
			#endif
			//while(!POWER_KEY)
			//{ }			
			for (i=0; i<sizeof(tblPM_OFF)/2; i++)
		  		mStar_WriteByte(tblPM_OFF[i][0], tblPM_OFF[i][1]);	

			#ifdef PM_U02				
                        EnableSAREvent(FALSE, FALSE);
			#endif

			SetPMClock();
			break;
			
		case ePM_PowerON:
			ucPM_Status = ePM_PowerON;						            
                    #if DEBUG_PM
                        printData("Enter PM PowerON mode!",0);
                    #endif            
			for (i=0; i<sizeof(tblPM_ON)/2; i++)
		  		mStar_WriteByte(tblPM_ON[i][0], tblPM_ON[i][1]);	

			EnableMCCSPM(FALSE);
                        
			break;	
		default:
			break;			
	}
	
	mStar_WriteByte(REGBK, REGBANK0);
}
//================================================
//====================================================================
// /*1.∵無信號VGA cable接上，Vsync會由low到high則IC回報Vsync detected */
// /*為了避免原先在Standby 時，因為無信號cable插上喚醒，因此再檢查一次是否有sync detected*/
// /*若第二次檢查並沒有sync，則判定為無信號cable插入，則keep在PM_standby mode*/
//
// /*2.After the chip is wakeup, reg_mcupd_key (BK1_BA[7:0]) needs to be cleared to 0x00 */
// /*before we enable AC2DC function (set reg_ac2dc_en=1 (BKC_64[7:0]) to enter in PM mode again.*/
//=====================================================================
BYTE xdata Check,Check1;	
extern Bool PowerOffSystem(void);
void ms_PM_Chcek_Sync(void)
{       
    mStar_WriteByte(REGBK, REGBANK1);
    mStar_WriteByte(BK1_BA, 0x00);

    GetPMStatus(&Check);
    #ifdef ENABLE_STANDBYINTOPM
    if(Check==ePMStatus_ADCActive)
    {
        msPM_ClearStatus(); 
        ForceDelay1ms(5);
        GetPMStatus(&Check1);		  
        ForceDelay1ms(5);
        msPM_ClearStatus(); 
        if(Check1 != ePMStatus_ADCActive)   
        { 
            Delay1ms(25);
            msPM_SetUp(ePM_Standby);
        }
        else if(Check==ePMStatus_ADCActive)    //For test
        { 
            Check1=ePMStatus_ForcPowerOn;
        }  
    }
    #endif
}
//====================================================================
// /*Case1：For test */
// /*Case2：if GPIO or SAR detected , check "poweronflag" to know what the mode is(should PM_ON or PM_standby)  */
// /*Case3：plug AC off and AC on when set is DC off , should keep DC off*/
//=====================================================================
void msPM_Check_Power_Status(void)
{
    if(Check1==ePMStatus_ForcPowerOn)          //For test
    {
       Set_PowerOnFlag();
    }    
    
    else if(Check==ePMStatus_GPIOActive ||Check==ePMStatus_SARActive 
    #if PM_MCCS        
    ||Check==ePMStatus_D2BActive
    #endif
    )
    {
        #ifdef ENABLE_STANDBYINTOPM
        if(PowerOnFlag) 
        {
            //CheckPowerKey();
            PowerOffSystem();	   
        }
        else
        #endif
        Set_PowerOnFlag();
        #ifdef MS_PM	
            #if USEFLASH//UserData_InFlash // 091013
            SaveMonitorSetting();
            #else  
            NVRam_WriteByte(nvrMonitorAddr(MonitorFlag), MonitorFlags);
            #endif
        #endif           
    }    
    else if(Check==ePMStatus_Non)  //DC off thenAC off and AC on , should keep DC off
    {
       if(!PowerOnFlag)  
       {
           //CheckPowerKey();
  	    PowerOffSystem();	   
       }	   
    }	 
}
/*
//====================================================================
// Case1：use to clear DisplayLogoFlag when wakeup event is HVsync,SOG ,DVI or MCCS.
// 用來擋掉Standby到ON時的LOGO
//=====================================================================
Bool msPM_Check_Not_Show_Logo(void)
{
    if(Check==ePMStatus_ADCActive ||Check==ePMStatus_DVIActive||Check==ePMStatus_D2BActive)
    {
       return TRUE;
    }
    return FALSE; 
}
*/
#endif

