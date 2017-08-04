#include "types.h"
#include "board.h"
#include "global.h"
#include "mcu.h"
#include "detect.h"
#include "menu.h"
#include "ms_reg.h"
#include "power.h"
#include "debug.h"
#include "ms_rwreg.h"
#include "msosd.h"
#include "misc.h"
#include "NVRam.h"
#include "mstar.h"
#include "UserPref.h"
#include "keypad.h"
#include "Panel.h"
#include "msflash.h"  //2006-03-20
#include "ddc2bi.h"
#include "menudef.h"//071226
#if DDCCI_ENABLE
    #include "adjust.h"
#endif
#if MWEFunction
    #include "MsDLC.h"
    #include "MsACE.h"
#endif
#if MS_OVD
#include "msovd_v03.h"
#endif
#if ENABLE_HDMI
#include "MsHDMI.h"
#endif
#ifdef ReduceDDC
#include "ddc.h"
#endif
#if ScanKey
#include "ScanKey.h"
#endif
#if Enable_Lightsensor
#include "I2c.h"
#endif
#if MS_DAC
#include "msdac.h"
#endif
#ifdef MS_PM
#include "MS_PM.h"
#endif
#if IT6251
#include "ITE6251.h"
#endif

Bool CheckInit(void);
void Init_Device(void);
void Main_SlowTimerHandler(void);
#if OSD_PIVOT_EN  //071226
void OSDPivotHandler(void);
extern void DrawOsdMenu(void);
//extern BYTE  MenuPageIndex;
#endif
extern void Init_GlobalVariables(void);
extern void i2C_Intial(void); //2006/03/20
//extern WORD ReadFlashID(DWORD address); 

extern BYTE xdata PowerDownCounter;
#if ENABLE_HDCP
    #if HDCPKEY_IN_Flash
        extern void msInitHDCPProductionKey(void);
    #endif
#endif

#if VGA_EDID_Write_To_24C02
 extern void Mstar_Load24C02EDID(void);
#endif
#ifndef ReduceDDC
extern void mStar_InitDDC(void);
#endif

#if (defined(OffPower)||ESaverPowerDownFunc)    //090331 xiandi.yu Auto Off Power
extern PowerOffSystem(void);
#endif
//extern BYTE Second;
#if Backupdata
    void GetCheckSum(void)
    {
        BYTE i, NVRAMData, checksum1; //,checksum2;
        WORD totalchecksum;
        totalchecksum = 0;
        for (i = 0; i < MonitorSettingSize; i++)
        {
            NVRam_ReadByte(i + MonitorSettingAddr, &NVRAMData);
            totalchecksum = totalchecksum + NVRAMData;
        }
        checksum1 = totalchecksum % 256;
        NVRam_WriteByte(Monitorchecksum, checksum1);
    }
#endif
/*
BYTE code tDDC[128]=
{ 
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x15,0xD6,0x18,0x22,0x00,0x00,0x00,0x00,
0x00,0x18,0x01,0x03,0x68,0x30,0x1B,0x78,
0x2A,0x9D,0x80,0xA2,0x59,0x54,0x9E,0x27,
0x0E,0x50,0x54,0xBF,0xEF,0x00,0xD1,0xC0,
0xB3,0x00,0x95,0x00,0x81,0x80,0x81,0x40,
0x81,0xC0,0x01,0x01,0x01,0x01,0x02,0x3A,
0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,
0x45,0x00,0xDD,0x0C,0x11,0x00,0x00,0x1E,
0x00,0x00,0x00,0xFD,0x00,0x32,0x4C,0x1E,
0x53,0x11,0x00,0x0A,0x20,0x20,0x20,0x20,
0x20,0x20,0x00,0x00,0x00,0xFC,0x00,0x4C,
0x45,0x44,0x20,0x32,0x32,0x31,0x38,0x57,
0x0A,0x20,0x20,0x20,0x00,0x00,0x00,0xFF,
0x00,0x45,0x45,0x42,0x31,0x38,0x34,0x30,
0x30,0x5A,0x30,0x30,0x30,0x30,0x00,0x0C,
};

void mStar_InitDDC(void)
{
	BYTE xdata i;

	mStar_WriteByte(DDC_LAST2, mStar_ReadByte(DDC_LAST2)&~BIT7); // bit7=0, write
	

		for(i = 0; i < 128; i++)
			{
			mStar_WriteByte(DDCADDR, i);
			mStar_WriteByte(DDCDATA, tDDC[i]);    
			}
	
	mStar_WriteByte(DDCEN2, BIT7|BIT5);
	mStar_WriteByte(DDC_LAST2,mStar_ReadByte(DDC_LAST2)|BIT7); // bit7=1, read
}

*/
void main(void)
{
ReStart:
    
    WD_SWZ1 = 0x55;
    WD_SWZ2 = 0xAA;

#ifdef MS_PM        
    ms_PM_Chcek_Sync(); //for Check no-Sync DC level wake up PM
    msPM_Reset();
#endif

    Init_MCU();

#ifdef ReduceDDC
    ScalerResetThenPowerDown();
#else
    ForceDelay1ms(100); // 100802 coding, wait for system power stable then execute scalar setting
    ScalerReset();
    GPIOPWMSelect();    
#endif

    g_ucFlashID = ReadFlashID();

	
    #if !USEFLASH 
    i2C_Intial(); //2006-03-20	
    #endif

     LogoON=1;
    ReadMonitorSetting(); //Benz 2007.4.23   19:11:22
    mStar_Init(); //2006-03-07

#ifdef ReduceDDC
    Flash_GetBufferAddr();
    Flash_AllocateBufferAddr();
    Flash_CheckBufferType();
	
	#if Enable_External_EDID
	mStar_WriteByte(DDCEN1, 0);
	mStar_WriteByte(DDCEN2, 0);
	#else
	mStar_InitDDC();
	#endif
	
    SystemVccOK();   
    ScalerReset();
    GPIOPWMSelect();    
 #else
    Flash_GetBufferAddr();
    Flash_AllocateBufferAddr();
    Flash_CheckBufferType();
	
	#if Enable_External_EDID
	mStar_WriteByte(DDCEN1, 0);
	mStar_WriteByte(DDCEN2, 0);
	#else
		#if !Enable_External_EDID
	mStar_InitDDC();
		#endif
	#endif
  #endif


    /* for ScanKey initial takes too much time 20120726 */
    #if ScanKey
    ScanKey_Init();
        #if Scan_Source == Synaptics        
        KeyLED_TurnOff();
        #endif
    #endif

	//mStar_InitDDC();

    #ifdef MS_PM
    #if DEBUG_PM
    printData("Check1=%d",Check1);
    #endif
    msPM_Check_Power_Status();  //need excute before Init_GlobalVariables function to check PowerOnFlag
    #endif

    #if !ScanKey
    CheckFactoryKeyStatus();
	#else
	Clr_FactoryModeFlag();
    #endif
    
    #if Backupdata
        Set_ACONFlag();
        Clr_DonotchangeBackFlag();
    #endif
    Init_GlobalVariables();


if (PowerOnFlag)
{
	if (FactoryModeFlag||BurninModeFlag)
	{
		
		Power_TurnOnGreenLed();//Power_TurnOnAmberLed();		
	}
	else
		Power_TurnOnGreenLed();
}
//Set_DisplayLogoFlag();
	
    Menu_InitVariable();
    #if ENABLE_HDCP
        #if Default_HDCP_Key
            if (!Check_HDCP_CRC())
            {
                Write_Internal_Key();
            }
        #endif
    #endif
    Init_Device();
    
    //#ifdef TSUMXXNX
    //I_Gen_Tuning();
    //#endif

   // if (!FactoryModeFlag)
    {
        DDC2Bi_Init();
        #if DDCCI_ENABLE
            mStar_AdjustUserPrefBlacklevel(UserprefRedBlackLevel, UserprefGreenBlackLevel, UserprefBlueBlackLevel);
        #endif
    }
    #if DEBUG_SAVEMODE
        printData("ModeSettingAddr=%d", ModeSettingAddr);
        printData("ModeInfoAddr=%d", ModeInfoAddr);
        printData("EndAddr=%d", EndAddr);
    #endif
#if (MainBoardType==MainBoard_715G6952_M0B) || (MainBoardType==MainBoard_715G6912_M1A)//Gino 20140605
	msWriteByteMask(BK0_63,0,BIT0);
#endif

	//Set_FactoryModeFlag();
 #if IT6251
 hw_SetPanel();
 ForceDelay1ms(100);
 IT6251_DeviceLoopProcedure();
         //  CTimerDelayXms(2000);        
 #endif

    while (1)
    {
#ifdef ReduceDDC
    if(SystemVccDropDownCnt >= SystemShutDownCntr)
    {
        EnCheckVccFlag = 0;
        ScalerIntoDeepPowerDown();
        SystemVccDropDownCnt = 0;
        goto ReStart;
    }
#endif        
        #if DEBUG_ENABLE
                DebugHandler();
                  if(DebugOnlyFlag)
		        continue;
            #else
                if (ES)
                {
                    DebugHandler();
                    if(DebugOnlyFlag)
		        continue;
                }
            #endif

		DDC2Bi_CommandHandler();
/*
		#ifdef UseVGACableReadWriteAllPortsEDID
			#ifdef ReduceDDC
			if(LoadEDIDFlag)
			{
				mStar_InitDDC();
				Clr_LoadEDIDFlag();
			}
			#endif
	        if(WriteDeviceFlag)
	            continue;
		#endif
		//*/

        #if ENABLE_HDCP
            #if HDCPKEY_IN_Flash
                if (!WriteHDCPcodeFlag)
            #endif
        #endif
        {
            #if ENABLE_HDCP
                #if HDCPKEY_IN_Flash
                    if (LoadHDCPKeyFlag)
                        {
                            msInitHDCPProductionKey();
                            Clr_LoadHDCPKeyFlag();
                    	   }
                #endif
            #endif
	     Main_SlowTimerHandler();
            Power_PowerHandler();


            if (!ModeDetectCounter || InputTimingChangeFlag)
            {
                mStar_MonitorInputTiming();
		#if 0//ENABLE_HDMI  //edit by lizzie,100429.for the time that HDMI change to powersaving is too long.
              	if (SrcInputType <Input_Digital)
		   		 ModeDetectCounter = 20;
              	else
                  		ModeDetectCounter = 40;
		#else
		    		ModeDetectCounter = 20;
		#endif
                    if (CheckInit())
                    {
                        goto ReStart;
                    }
            }
#if IT6251
if(PanelOnFlag&&CheckITE6251Flag&&PowerOnFlag&&(!SyncLossState()) && InputTimingStableFlag)
{

Clr_CheckITE6251Flag(); 
IT6251_DeviceLoopProcedure();
//WriteI2C_Byte(0xbc, 0x05,0x34);		
Delay1ms(2);	

}
#endif

		#if ENABLE_HDMI
		mstar_HDMIHandler();
			#if ENABLE_CEC
			MsAPI_CecCecHandler();
			#endif
		#endif
            mStar_ModeHandler();
        #if MS_DAC
            msAudioGainToggle();
        #endif
            Menu_OsdHandler();
	     #ifdef ReduceDDC
      		mStar_CheckRAM();
	     #endif
            #if MWEFunction
                //if ((ColorAutoDetectFlag || UserPrefDcrMode) && PowerOnFlag && !FreeRunModeFlag && InputTimingStableFlag)
                if ((ColorAutoDetectFlag || UserPrefDcrMode) && PowerOnFlag && !SyncLossState() && !FreeRunModeFlag&& InputTimingStableFlag)//edit by yz.nie .for DCR ON,change timing quickly have white screen.
                {
                    msDlcHandler();
                }
            #endif
            #if Backupdata
                if (GetChecksumFlag && !ACONFlag)
                {
                    Clr_GetChecksumFlag();
                    Clr_DDCCIAadjustFlag();
                    GetCheckSum();
                }
            #endif
			
	#if VGA_EDID_Write_To_24C02&&FDDCCI_WriteEDID
	  if (!VGA_EDID_Flag&&!ATEModeFlag&&(ReloadedidCounter==0))
       	 Mstar_Load24C02EDID();
        #endif
            
	 #if OSD_PIVOT_EN
		OSDPivotHandler();
	 #endif

        #if Enable_Lightsensor
            //if((UserPrefiCareMode == LightSensorOn) && PowerOnFlag && !FreeRunModeFlag &&
              //  InputTimingStableFlag)
            if(PowerOnFlag && !DisplayLogoFlag && (UserPrefiCareMode == LightSensorOn) && 
                !SyncLossState() && InputTimingStableFlag)
                LightSensorHandler();    
        #endif

        #if USEFLASH
            UserPref_FlashDataSaving();
        #endif
        
        }
        
    }
}

//#if !Enable_Expansion
Bool CheckInit(void)
{
   // if (mStar_ReadByte(LPLL_M) != 0x48 || mStar_ReadWord(SIVEND_H) != (PanelHeight - 1) || mStar_ReadWord(SIHEND_H) != (PanelHStart + PanelWidth - 1))
   
	#ifndef TSUMXXT			//110311 Modify
	if (mStar_ReadByte(LPLL_M) != 0x48 || mStar_ReadWord(SIVEND_H) != (PanelHeight - 1) )
	{
		return TRUE;
	}
	#endif
    return FALSE;
}
//#endif
#if 0//MWEFunction
    extern void SetColorEnhance(bit enable);
#endif
void Init_Device()
{
    #if USB_Contral
        hw_USB_HI();
    #endif
    #if AudioFunc || ENABLE_HDMI
        mStar_AdjustVolume(0);
    #endif
    //   printMsg("init decice_1...");

    if (PowerOnFlag)
    {
        //Power_TurnOnGreenLed();
    #if Enable_Lightsensor
        LightSensor_On();
    #endif

    #if ScanKey
        #if Scan_Source == Cypress
            #ifdef LEDBrightCtrlByScalerPWM
            SKLEDStatus = SKLEDBright_Light_Immediately;
            ScanKey_LEDBrightAction();
            #else
            ForceDelay1ms(30); // wait LED turn on
            Set_SK_LEDVcc();
            #endif
        #endif
    #endif
    }
    else
    {
        Power_TurnOffLed();

    #if ScanKey
        #if Scan_Source == Cypress
            #ifdef LEDBrightCtrlByScalerPWM     
            SKLEDStatus = SKLEDBright_Off;
            ScanKey_LEDBrightAction();
            #else
            Clr_SK_LEDVcc();
            #endif
        #endif
    #endif
    }

    #if ScanKey
        #if Scan_Source == Cypress

        #ifdef LEDBrightCtrlByScalerPWM
            #if MainBoardType == MainBoard_715G3603_M0A
            mStar_WriteByte(PG_SWCH, mStar_ReadByte(PG_SWCH)|BIT2); // enable pwm1 for LED birght
            #elif MainBoardType == MainBoard_715G3598_M0A
                #ifdef TSUMXXNX
                mStar_WriteByte(PG_SWCH, mStar_ReadByte(PG_SWCH)|BIT5); // enable pwm2 for LED birght
                #else
                mStar_WriteByte(PG_SWCH, mStar_ReadByte(PG_SWCH)|BIT1); // enable pwm0 for LED birght
                #endif
			#elif MainBoardType==MainBoard_715G3598_1PLL
				mStar_WriteByte(PG_SWCH, mStar_ReadByte(PG_SWCH)|BIT5); // enable pwm2 for LED birght				
            #endif

        #endif     

        #ifdef EnableLightBar
            //ScanKey_CtrlPWM(UserPrefLightBar);
            //if(PowerOnFlag)
                //ScanKey_EnablePWM(1);
            //else
                //ScanKey_EnablePWM(0);
            ScanKey_PWMCtrl(UserPrefLightBar);                
        #endif    
        
        #endif

        #ifdef LEDFlash
        //Set_SK_LEDVcc();
        LEDFlashStep = BIT0;
        LEDFlashCounter = LEDFlashChangeInterval;
        #endif

    #endif
    
}


#if 1
void Main_SlowTimerHandler(void)
{

if(SecondFlag)
{

        //Second++;
  		if (!PowerSavingFlag)   //xujuanjuan20120201 
		{	
		#if  BlacklitTimeOnShow
			if (BlacklitTimeOn_S < 0xFFFFFFFFul)
			{
				
				BlacklitTimeOn_S++;
                        
			if ((BlacklitTimeOn_S%60)==0)				
  			{	
  			  	BlacklitTimeOn_S=0;
				BlacklitTimeOn_M+=1;
				if ((BlacklitTimeOn_M%60)==0)
  				{	
  					BlacklitTimeOn_M=0;
        				if (BlacklitTimeOn_H>=0xFFFF)
         				{	
        					;//BlacklitTimeOn_HH+=1;
        				}
        				else
	           				BlacklitTimeOn_H+=1;
   				}
			}
			
			}
			else
				BlacklitTimeOn_S = 0;
			#endif
		}

		#if ESaverPowerDownFunc
		if(ESaverPowerDownCounter)
		  {
             if(--ESaverPowerDownCounter == 0)
			   {
			   	  MenuPageIndex=PowerOffMenu;
                  MenuItemIndex=0;
                  PowerOffSystem();
			   }
			// Osd_Draw4Num(1,2,ESaverPowerDownCounter);
		  }
		#endif
        
        if (OsdCounter)
        {
            if (--OsdCounter == 0)
            {
                Set_OsdTimeoutFlag();
            }
        }
            
        if (HotKeyCounter)
        {
            if (--HotKeyCounter == 0)
            {
                if (PressExitFlag)
                    Set_EnableShowDDCFlag();
            }
        }
            
        if (DoKeyCountFlag && PowerOnFlag)
        {
            PowerKeyCounter++;
#if AllInOneKey
            if(ProductModeFlag)
                PowerKeyCounter%=1;
            else
#endif
            PowerKeyCounter%=2;// /3;For GPM Request
            if(PowerKeyCounter==0)
                Set_PowerOffFlag();
        }


        if (PowerDownCounter)
        {
            if (--PowerDownCounter == 0)
            {
                Set_ForcePowerSavingFlag();
            }
        }
            
#if AllInOneKey
        if(SourceKeyCnter)
            SourceKeyCnter--;
#endif

#if Backupdata
        if (DDCCIAdjCounter)
            if ((--DDCCIAdjCounter) == 0)
                Set_GetChecksumFlag();
#endif

        

#if ScanKey
        if(KeyLedCounter)
        {
            if(--KeyLedCounter == 0)
    #if Scan_Source == Synaptics
                Set_keyLEDTurnOffFlag();
    #else
        #ifdef LEDBrightCtrlByScalerPWM
                SKLEDStatus = SKLEDBright_Dark;
        #else
                ;
        #endif
    #endif        
        }

    #if 0//def PowerSavingAmberLEDFlash
        if(PowerSavingFlag)
        {   
            LEDFlashCount++;
            
            if(LEDFlashCount%2) // on 1000ms
                hw_SetAmberLed();
            else // off 1000ms
                hw_ClrAmberLed();
        }
    #endif
        
#endif

#if FDDCCI_WriteEDID
	if(ReloadedidCounter != 0)
		ReloadedidCounter--;
#endif

#if IT6251
Set_CheckITE6251Flag();
#endif

    #if USEFLASH
	UserPref_FlashSaveFlagCheck();//edit by lizzie,100429.for FlashSave every 1s,suggest by hungyi.cai 
    #endif


        Set_ChangePatternFlag();
        Clr_SecondFlag();
        
}

    if (ms10msCounter==0)
    {
        ms10msCounter=10;
        #ifdef PowerKeyDeBounce
        if (PushPowerKeyFlag)
        {
            if (PushPowerKeyDeBounce<0xFF)
                PushPowerKeyDeBounce++;
        }            
        #endif
    }

#if ScanKey
    #ifdef PowerSavingAmberLEDFlash
    if(LEDFlash100mSFlag)
    {
        LEDFlash100mSFlag = 0;
        LEDFlashCount++;
        if(LEDFlashCount == 10) // off 500ms
            hw_ClrAmberLed();
        else if(LEDFlashCount == 15) // on 1000ms
        {
            hw_SetAmberLed();
            LEDFlashCount = 0;
        }
    }
    #endif

	
#ifdef PowerSavingAmberLEDFlashing
	#if PSAmberLEDFlashingType == Type_Breathing
            if(bLEDFlashingActFlag)
            {
                bLEDFlashingActFlag = 0;
                PowerSavingLEDBreathing();
            }
	#else
        if(LEDFlash100mSFlag)
        {
            LEDFlash100mSFlag = 0;
            LEDFlashCount++;
            if(LEDFlashCount == 10) // off 500ms
            {
	#if ScanKey
                Clr_SK_LEDVcc();
	#else
                hw_ClrAmberLed();
	#endif
            }
            else if(LEDFlashCount == 15) // on 1000ms
            {
	#if ScanKey
                Set_SK_LEDVcc();
	#else
                hw_SetAmberLed();
	#endif				  
                LEDFlashCount = 0;
            }
        }
	#endif
#endif

#endif    

#ifdef OffPower          //090331 xiandi.yu Auto Off Power
    if(OffPower1sFlag)
    	{
	if (UserPrefAutoPowerOff && PowerOnFlag)
        {
                AutoPowerOffSTime++;
            if ((AutoPowerOffSTime%60)==0)
            { 
                AutoPowerOffSTime=0;
           		AutoPowerOffMTime++;

				if (AutoPowerOffHTime == (UserPrefAutoPowerOff-1))
				{
					if(AutoPowerOffMTime == 50)
						ShowOffPowerWarningFlag = 1;
				}
					
                if ((AutoPowerOffMTime%60)==0)
                {
                    AutoPowerOffMTime=0;
                    if (AutoPowerOffHTime<UserPrefAutoPowerOff)
                    {
                        AutoPowerOffHTime++;
                        if (AutoPowerOffHTime==UserPrefAutoPowerOff)
                        {
                            MenuPageIndex=PowerOffMenu;
                            MenuItemIndex=0;
                            PowerOffSystem();
                        }
                    }                        
                }
            }                
        }
	OffPower1sFlag=0;
    	}
        #endif


}
#endif
#if OSD_PIVOT_EN

bit OSDRotateOn()
{
   return( ( mStar_ReadOSDByte ( IOSDC1 ) & Rotate )&&(mStar_ReadOSDByte ( IOSDC1 )& MWIN_B));
}

void OSDPivotChange(void)
{

	Osd_Hide();

	if(OSDRotateFlag)
 		Osd_SetPosition(100-UserPrefOsdHStart, 100-UserPrefOsdVStart);
 	else
  		Osd_SetPosition(UserPrefOsdHStart, UserPrefOsdVStart);

       DrawOsdMenu();

}

void OSDPivotHandler(void)
{
	if(hw_PnlDirtDect == 0) //--
	{
		PivotStatusDebunce=0;
		Clr_OSDRotateFlag();
	 	if(OSDRotateOn() && MenuPageIndex!=RootMenu)
		{
				OSDPivotChange();
		}	
	}
	else //||
	{
		PivotStatusDebunce++;
		Set_OSDRotateFlag(); 
	 	if((!OSDRotateOn()) && (MenuPageIndex!=RootMenu) && PivotStatusDebunce>50)
		{
				PivotStatusDebunce=0;
				OSDPivotChange();
		}			
		
	}	

}


#endif
