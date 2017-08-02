//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#define MAIN_C
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "main.h"
#include "datatype.h"
#include "Board.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "debug.h"
#include "MsTypes.h"
#include "drvUART.h"
#include "drvUartDebug.h"
#include "uartdebug.h"
#include "SysInit.h"


#include "Global.h"
#include "Mcu.h"
#include "Detect.h"
#include "Menu.h"
#include "menufunc.h"
#include "UserPrefAPI.h"
#include "UserPref.h"
#include "ms_reg.h"
#include "Power.h"
#include "MDebug.h"
#include "Common.h"
#include "Ms_rwreg.h"
#include "msOSD.h"
#include "misc.h"
#include "NVRam.h"
#include "mStar.h"
//#include "UserPref.h"
#include "Keypad.h"
//#include "Panel.h"
#include "msflash.h"  //2006-03-20
#include "DDC2Bi.H"
#include "GPIO_DEF.h"
#include "MsIR.h"
#include "IrFunc.h"
#if ENABLE_RTE
#include "drvmsOVD.h"
#endif
#if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif
#include "FactoryMenu.h"
#include "Ms_PM.h"
#if DDCCI_ENABLE||AudioFunc
#include "Adjust.h"
#endif
#include "MsDLC_Setting.h"
//#include "MsDLC.h"
#include "msPQ.h"
#include "msACE.h"
#include "mapi_combo.h"
#if ENABLE_DAC
#include "drvAudio.h"
#endif

#if (ZUI || GOP)
#include "MApp_Init.h"
#include "MApp_ZUI_Main.h"
#include "MApp_Key.h"
#endif

////////////////////////////////////
//#include "drvMcu.h"
///////////////////////////////////

#if ENABLE_DPS
#include "msDPS_Handler.h"
#include "UserPrefDef.h"
#include "msDPS_Setting.h"
#endif
#include "msAPI_MailBox.h"
//#include "EepromMap.h"
#include "appWindow.h"
#include "appSystem.h"
#if XMODEM_DWNLD_ENABLE
#include "xmodem.h"
#endif
#if ENABLE_VBY1_TO_EDP
#include "msVBy1ToEDP.h"
#endif
#if ENABLE_DAISY_CHAIN
#include "msDaisyChain.h"
#endif
#include "mapi_CEC.h"

#define Main_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&Main_DEBUG
    #define MAIN_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
    #define MAIN_PRINT(format, ...)
#endif


void Init_Device( void );
void Main_SlowTimerHandler(void);
extern void Init_GlobalVariables( void );
extern void i2C_Intial( void ); //2006/03/20
extern Bool ResetAllSetting( void );
extern void SaveUserPref(void);
Bool ExecuteKeyEvent(MenuItemActionType enMenuAction);

//extern BYTE xdata g_u8PowerDownCounter;
#ifdef LoadDefaultEDIDTo24C02
void Mstar_InitDDC(void);
#endif
#if AC_ON_ChecK_EDID
void Mstar_CheckDDC(void);
#endif
void BurnInHandle(void);
#if (VBY1_PowerSequence)
void BackLightHandel(void);
#endif
void EarphoneHandler(void);

WORD xdata mainloop = 0;
xdata BYTE g_u8DlcLibStatus;
extern int usb_check(BOOL bFullDL);
#if USB_Updata_FW_Enable	
void DrawOsdMenuItem_USB(void);
#endif
#if (ENABLE_USB_HOST)
void USB_Init_Configure(void);
#endif
void SwitchPort( BYTE ucWinIndex, BYTE ucInputPort );
#if Audio_Driver_Use_5707_Need_To_Check
void CheckI2CStatus(void);
#endif

int main( void )
{
#if ENABLE_DP_INPUT
    g_bDoDPInit = TRUE;
#endif
    mStar_InitMCU();

#if DP_HPD_Test
	TimingUnstableCNT=0;
 #endif
#if ENABLE_WATCH_DOG
    msAPI_ClearWDT();
#endif

#if ENABLE_MSTV_UART_DEBUG
    dbgVersionMessage();
    MDrv_UART_DebugInit(DEFAULT_UART_DEV);
#else
    mdrv_uart_close((MS_U32)mdrv_uart_open(DEFAULT_UART_DEV));
#endif
    UART_Clear();

    g_ucFlashID = ReadFlashID(); // FSP not support spi quad mode. Need change to FR before FSP.
#if ENABLE_SPI_2ND
	SetFlashType(SPI1,SPI1_FLASH_TYPE);
	SetFlashSelection(SPI1);
	ReadFlashID();
	SetFlashSelection(SPI0);
#endif

#if !USEFLASH
    i2C_Intial();
#if Audio_Driver_Use_5707_Need_To_Check
    CheckI2CStatus();
#endif

#endif

#if ENABLE_DP_OUTPUT
#if !eDPTXHBR2PortEn
    DPTxInit();
    DPTxSetMSA(0);
  #if (!eDPTXMODOnePort)	
    DPTxSetMSA(1);
  #endif
#endif
#endif

#ifndef MST9U_FPGA
    ReadMonitorSetting();
#else
    NvramMapDataInit();
    SET_POWER_ON_FLAG();
#endif

#if ((defined(HongTian_Project))&&(PanelType==PanelM238DCJ_E50))
    SET_POWER_ON_FLAG();
#endif

    printData("POWER_ON_FLAG==%x",POWER_ON_FLAG);

#if AC_ON_ChecK_EDID
   Mstar_CheckDDC();
#endif

#ifdef LoadDefaultEDIDTo24C02
   Mstar_InitDDC();
#endif

#if (ENABLE_USB_HOST)
    USB_Init_Configure();
#endif

    //CheckFactoryKeyStatus();
    mStar_Init();

#if (!LHCX_EnterFactoryMode)
    CheckACKeyStatus();
#endif

#if ENABLE_VBY1_TO_EDP
	msVTEInit(Vby1ToEDP_MIIC, Vby1ToEDP_MIIC_ENABLE);
	msVTESetPanelParams(PanelHTotal, PanelVTotal, PanelWidth, PanelHeight, PanelHStart, PanelVStart);
#endif

    Init_GlobalVariables();
#if (!ZUI)
    Menu_InitVariable();
#endif
#if (ZUI || GOP)
    MApp_PreInit();
#endif
    Init_Device();
    DDC2Bi_Init();


    MAIN_PRINT("  g_u8SystemSpeedMode:%d", g_u8SystemSpeedMode);

#if(IR_REMOTE_SEL != NO_REMOTE)
    irInitialize(); // initialize IR
#endif
    //APPWindow_Init();
#if ENABLE_MBX
    MApi_MBX_Init();
    MApi_MBX_Enable(TRUE);
    #if ENABLE_MBX_SAR
    MApi_MBX_RegisterMSG(E_MBX_CLASS_SAR, 32);
    #endif
#endif


#if (XMODEM_DWNLD_ENABLE)
    xmodem_init();
#endif


#if UseVsyncINT
    msDrvVsyncINTEnable();
#endif

#if (ENABLE_SECU_R2 == 1) && (ENABLE_HDCP22 == 1)
#if COMBO_HDCP2_INITPROC_NEW_MODE
    msAPISrcInitTmdsHPD(TRUE);
#endif
#endif


#if ENABLE_USB_HOST
 if(FACTORY_MODE_FLAG)
 {
   ForceDelay1ms(2000);
    usb_check(1);
 }
#endif

#if USB_Updata_FW_Enable
 USBUpdataFwStatus=USB_Idle;
#endif

    while( 1 )
    {
        Main_SlowTimerHandler();

		
#if( ENABLE_WATCH_DOG )
        msAPI_ClearWDT();
#endif

#if (ENABLE_MSTV_UART_DEBUG)
        if ( msAPI_UART_DecodeCommand() )
        {
            continue; // stop main loop for debug
        }
#endif
		DDC2Bi_CommandHandler();
		if(bRunToolFlag)
            continue;
		
#if(IR_REMOTE_SEL != NO_REMOTE)
        irPollingHandler();
        irDecodeCommand(); // ir decode command
        irDecodeNumKey(); // ir decode number key
#endif
#if ENABLE_DAISY_CHAIN
        msDCTimerHandler();
#endif

#if (HKC_PCToolCommand)
      if((DDCCIActiveFlag)&&(DDCCIFuncCounter==0))
       {
          Clr_DDCCIActiveFlag();
	#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	  
		if ( DDCCIFuncCommand == 0x01 )
		{
		   USER_PREF_PIP_POSITION = PIPPosition_Bottom_Left;
             if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
	      else		 
                 ExecuteKeyEvent(MIA_HotMulti_PIP);
		}
		else if( DDCCIFuncCommand == 0x02 )
		{
		   USER_PREF_PIP_POSITION = PIPPosition_Top_Left;
             if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
	      else		 
                 ExecuteKeyEvent(MIA_HotMulti_PIP);
		}
		else if( DDCCIFuncCommand == 0x03 )
		{
		   USER_PREF_PIP_POSITION = PIPPosition_Top_Right;
             if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
	      else		 
                 ExecuteKeyEvent(MIA_HotMulti_PIP);
		}
		else if( DDCCIFuncCommand == 0x04 )
		{
		   USER_PREF_PIP_POSITION = PIPPosition_Bottom_Right;
             if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
	      else		 
                 ExecuteKeyEvent(MIA_HotMulti_PIP);
		}
		else if( DDCCIFuncCommand == 0x05 )
		{
		if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_3)
                 ExecuteKeyEvent(MIA_HotMulti_PBP_3WIN);
		}
		else if( DDCCIFuncCommand == 0x06 )
		{
		if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_2_POP1)
                 ExecuteKeyEvent(MIA_HotMulti_PBP_2WIN);
		}
              else
	#endif		  	
              {
                 ExecuteKeyEvent(MIA_HotMulti_PBP_OFF);
              }
    	}
#endif
	  
       msDrvScHandler();
       msAPI_combo_IPHandler();
		
PM_Handler:		
        msPM_Handler();

#if ENABLE_DPS
        msDPSHandler();
#if DPS_FAD_OnOff_Mode
	 msDPS_Off_Fad_Handler ( REG_DPS_Off_FadFlag);
#endif
#endif

    if( ((USER_PREF_DLC_MODE!=OFF_DLC)||ENABLE_SmartConntrast())&&POWER_ON_FLAG&&!SyncLossState(USER_PREF_INPUT_TYPE)&&InputTimingStableFlag(USER_PREF_INPUT_TYPE)&&!UnsupportedModeFlag(USER_PREF_INPUT_TYPE))
        msDlcHandler();
	
#if AudioFunc || ENABLE_HDMI_INPUT
#if ENABLE_DAC
        msAPI_AudioHandler();
#endif
#endif

#if ENABLE_MBX
        MBX_Handler();
#endif

#if (!ZUI)
        if ((g_u8MenuPageIndex >= 0x80)||(g_stMenuFlag.bShowFactoryMenu==1))
            FactoryMenuHandler();
        else
            Menu_OsdHandler();
#else
        MApp_ZUI_MainLoop();
#endif

        BurnInHandle();	

    #if 0
        g_u8DlcLibStatus=0;//clear
        if (USER_PREF_DLC_MODE && POWER_ON_FLAG && !SyncLossState(SrcInputPortM) && InputTimingStableFlag(SrcInputPortM))
        {
            if (!FreeRunModeFlag(SCL0_MAIN))
            {
            #if ENABLE_DCR
                g_u8DlcLibStatus=msDLC_LibHandler((DCR_ENABLE_FLAG) ? (DLF_DCR_ACTIVE) : 0);
            #else
                g_u8DlcLibStatus=msDLC_LibHandler(0);
            #endif
            }
        }
    #endif

#if ENABLE_TOUCH_PANEL||ENABLE_TOUCH_PANEL_CTRL_OSD
        TouchPanelHandler();
#endif

        APPSystem_Handler();

	if(Is_PM_Mode_State())	
	 goto PM_Handler;

        APPWindow_Handler();
        //msPQ_AdaptiveTuning(USER_PREF_WIN_SEL);

#if (VBY1_PowerSequence)
        BackLightHandel();
#endif	 


	#if (ENABLE_CEC == 1)
		api_CEC_Handler();
	#endif
	
    }
    return 0;
}


#if (ENABLE_USB_HOST)
void USB_Init_Configure(void)
{
    U32 ii=0;

    MEM_MSWRITE_BIT(_REG_GPIOSAR_OUT, 0, BIT7);
	#if ((CHIP_ID == MST9U3) && ENABLE_DC_SDM_FORWARD)		
    MEM_MSWRITE_BIT(_REG_GPIO6_OEZ, 0, BIT4);	// use GPIO64 for GPIO_USB
	#else
    MEM_MSWRITE_BIT(_REG_GPIOSAR_OEZ, 0, BIT3);	// use GPIO_SAR3 for GPIO_USB
	#endif
	
    MAIN_PRINT("\n  USB_Init_Configure\n");
    MDrv_WriteByte(0x150200, 0x0A); // Disable MAC initial suspend, Reset UHC
    MDrv_WriteByte(0x150200, 0x28); // Release UHC reset, enable UHC XIU function

    MDrv_WriteRegBit(0x150300+0x3C, ENABLE, 0x01);  // set CA_START as 1
    MsOS_DelayTask(10);
    MDrv_WriteRegBit(0x150300+0x3C, DISABLE, 0x01);  // release CA_START

    while( (MDrv_ReadByte(0x150300+0x3C) & 0x02) == 0)  // polling bit <1> (CA_END)
    {
        if (ii++>10000)
        {
            MAIN_PRINT("Poll USB port0 CA_END timeout\n");
            break;
        }
    }

    MDrv_WriteByteMask(0x150200+0x02, BIT0, (BIT0|BIT1));  //UHC select enable

    MDrv_WriteRegBit(0x150500+0x40, DISABLE, BIT4); //0: VBUS On.
    MsOS_DelayTask(10);

    //improve the efficiency of USB access MIU when system is busy
    MDrv_WriteRegBit(0x150500+0x81, ENABLE, 0xF);
    //Disable battery charge mode
    MDrv_WriteRegBit(0x150400+0x0C, DISABLE, 0x40);// [6]= reg_into_host_bc_sw_tri
    MDrv_WriteRegBit(0x150400+0x03, DISABLE, 0x40);// [6]= reg_host_bc_en
    MDrv_WriteRegBit(0x150400+0x01, DISABLE, 0x40);//IREF_PDN=1!|b1. (utmi+0x01[6] )

    MAIN_PRINT("  USB_Init_Configure done...\n\n");
}
#endif

#if ENABLE_USB_HOST && ENABLE_FILESYSTEM
void sw_update_cb(U8 u8Percent)
{
    if(u8Percent > 100)
        u8Percent = 100;
    //MApp_BL_DisplaySystem(u8Percent);
    printf("%s: u8Percent: %u, at %d\n", __func__, u8Percent, __LINE__);
    return;
}

#include "drvGlobal.h"
#include "drvUSB.h"
#include "MApp_USBDownload.h"
#include "mw_usbdownload.h"
#include "mw_usbdownload.h"

int usb_init(void)
{
    const U32 max_count = 0x1000;
    U32 connect_count = 0;

    while(connect_count++ < max_count)
    {
        MAIN_PRINT("Connecting....\n");

        if (MDrv_UsbDeviceConnect()==1)
        {
            MAIN_PRINT("Connect\n");
            if (MDrv_Usb_Device_Enum() == 1)
            {
                MAIN_PRINT("USB enumeration success!!\n");
                break;
            }
            else
            {
                MAIN_PRINT("USB enumeration fail!!\n");
            }
        }
        else
        {
            //MAIN_PRINT("NO connect\n");
        }
    }
    return 1;
}
#if 1

int CheckUSB_existDLBinFile(void)
{
#include "SysInit.h"

    U8 u8PortEnStatus = 0;

    printf("run usb_init..\n");
    usb_init();

    printf("run usb_check..\n");
    //USB init
    MDrv_UsbDeviceConnect();

    printf("MDrv_UsbDeviceConnect done..\n");

    //MDrv_UsbDeviceConnect_Port2();
    MsOS_DelayTask(500);

    u8PortEnStatus = 1;//MDrv_USBGetPortEnableStatus();
    printf("u8PortEnStatus = %d\n", u8PortEnStatus);

    MDrv_Sys_ClearWatchDog();

    if((u8PortEnStatus & BIT0) == BIT0)
    {
        if (!MDrv_UsbDeviceConnect())
        {
                printf("USB Port1 is Not detected!\n");
        }
        else
        {
            printf("USB Port1 is detected.\n");

            MApp_UsbDownload_Init(BIT0, sw_update_cb);

            #if (ZUI)
//                FlashProgressFuncAttach( MApp_ZUI_ACT_UpdateDownloadProgress);
            #else
                FlashProgressFuncAttach( sw_update_cb);
            #endif
            if (MW_UsbDownload_Search())
            {
                printf("updated file in USB is found.\n");
                //return E_FILE_READY;
            }
            else //no sw file detected
            {
                printf("No updated file in USB is found!\n");
                //return E_NO_FILE_SKIP;
            }
        }
    }
    else
    {
        printf(".");
    }
    return 0;
}

//#else

// bFullDL
//    1: download all merge.bin
//    0: download exclude sboot range
int usb_check(BOOL bFullDL)
{
#include "SysInit.h"

    U8 u8PortEnStatus = 0;

    printf("run usb_init..\n");
    usb_init();

    printf("run usb_check..\n");
    //USB init
    MDrv_UsbDeviceConnect();

    printf("MDrv_UsbDeviceConnect done..\n");

    //MDrv_UsbDeviceConnect_Port2();
    MsOS_DelayTask(500);

    u8PortEnStatus = 1;//MDrv_USBGetPortEnableStatus();
    printf("u8PortEnStatus = %d\n", u8PortEnStatus);

    MDrv_Sys_ClearWatchDog();

    if((u8PortEnStatus & BIT0) == BIT0)
    {
        if (!MDrv_UsbDeviceConnect())
        {
            //if((u8PortEnStatus & BIT1) != BIT1)
            {
                printf("USB Port1 is Not detected!\n");
            }
	      #if USB_Updata_FW_Enable		
		      USBUpdataFwStatus=USB_Idle;
		      DrawOsdMenuItem_USB();
             #endif		
			
        }
        else
        {
            printf("USB Port1 is detected.\n");

           hw_ClrGreenLed();
           hw_SetAmberLed();
           ForceDelay1ms(300);
           hw_ClrAmberLed();
           ForceDelay1ms(300);
           hw_SetAmberLed();
           ForceDelay1ms(300);
           hw_ClrAmberLed();
           ForceDelay1ms(300);
           hw_SetAmberLed();


            MApp_UsbDownload_Init(BIT0, sw_update_cb);

            #if (ZUI)
//                FlashProgressFuncAttach( MApp_ZUI_ACT_UpdateDownloadProgress);
            #else
                FlashProgressFuncAttach( sw_update_cb);
            #endif

            if (MW_UsbDownload_Search())
            {
	      #if USB_Updata_FW_Enable		
		      USBUpdataFwStatus=USB_Writting;
                    DrawOsdMenuItem_USB();
             #endif		
            
                printf("updated file in USB is found.\n");
                //Thomas update for BOOTLOADER
                //MApp_BL_DisplaySystem_clear(); //clear screen
                //MApp_BL_DisplaySystem_setStatus((S8*)"Software Update(USB)");

                //MApp_BL_DisplaySystem(0);  //leo.chou

                MW_UsbDownload_Start(bFullDL);

                return 0;

                //MDrv_MIU_SPI_SetOffset(0LU);
                MDrv_Sys_SetWatchDogTimer(0);
                MDrv_Sys_EnableWatchDog();

                while(1);   // Necessary because of using watch dog reset
                //break;
            }
            else //no sw file detected
            {
                //if((u8PortEnStatus & BIT1) != BIT1)
                {
                    printf("No updated file in USB is found!\n");
                }
	      #if USB_Updata_FW_Enable		
		      USBUpdataFwStatus=USB_Idle;
		      DrawOsdMenuItem_USB();
             #endif		

				
            }
        }
    }
    else
    {
        printf(".");
    }
    return 0;
}
#endif
#endif

void Init_Device()
{
#if ENABLE_TOUCH_KEY
    TouchKeyRestortCounter = TouchKey_Init();
#endif

#if ENABLE_5707
//AMP_Init();
#endif


#if (AudioFunc/* && ENABLE_VOLUME_ADJ*/)
    msAPI_AdjustVolume( 0 );
#endif

#if ENABLE_TOUCH_PANEL||ENABLE_TOUCH_PANEL_CTRL_OSD
    TPL_InitSetting();
#endif



#if PANEL_VBY1()
#ifdef PanelI2CCommandSetting
    if(!VBY1_Check_LockN_Flag)
    {	
      printMsg("333333333333333");
      Set_VBY1_Check_LockN_Flag();
      msWriteByteMask(REG_140600, 0x00, 0x01); // force at 1st time, especially for HW mode that might show lock(but display error) before we doing the training.
      msDrvCheckVBY1LockN(0);
      
      ForceDelay1ms(150);
      
      if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetPanelI2CCommand]!=NULL)
        ((fpCustomerSetPanelI2CCommand)CB_FuncList[eCB_CustomerSetPanelI2CCommand])();
    }
#endif
#endif

}

void Main_SlowTimerHandler(void)
{
    if(ms10Flag)
    {
	
        g_u8SCTimerFlag10ms = 1;

        if(TPDebunceCounter)
        {
            if(TPDebunceCounter > 10)
                TPDebunceCounter -= 10;
            else
            {
                TPDebunceCounter = 0;
            }
        }
        if(!FreeRunModeFlag(SCL0_MAIN/*//tmp*/))
        {
        #if ENABLE_TOUCH_PANEL || ENABLE_TOUCH_PANEL_CTRL_OSD
            if(TPL_PollingCounter)
            {
                if(TPL_PollingCounter > 10)
                    TPL_PollingCounter -= 10;
                else
                    TPL_PollingCounter = 0;
            }
        #endif
        }
#if ENABLE_DC_SDM_FORWARD
        g_SDMCommandFlagDetect = TRUE;
#endif

#if HKC_PCToolCommand
        if(DDCCIFuncCounter)
        {
           DDCCIFuncCounter--;
        }
#endif		

     if(BlackScreenCounter)
     if(--BlackScreenCounter==0)
	 {
           if(OSDActionFlag&&BlackScreenFlag)
              mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);  
		Clr_OSDActionFlag();   
		
     	}

   if((!InputTimingChangeFlag(USER_PREF_INPUT_TYPE))&&(InputTimingStable_NormalFlag)&&(!SyncLossState(USER_PREF_INPUT_TYPE)))
   {
     if(OSDActiveCounter)
     if(--OSDActiveCounter==0)
	 {
	      Clr_OSDFreeSyncActionFlag();	  
     	}
    }

        Clr_ms10Flag();
    }
    if(ms20Flag)
    {
        Clr_ms20Flag();
    		
    }		
    if(ms50Flag)
    {
      #if ENABLE_MMD_DCR
        if(ENABLE_SmartConntrast())
        	MF_DCRHandler();
      #endif		

        if(g_u8SyncLossOsdCounter)
        {
            if( --g_u8SyncLossOsdCounter == 0 )
            {
                 g_stMenuFlag.bShowChangeMessage=1;
 	    //printData("g_u8SyncLossOsdCounter==%d", g_u8SyncLossOsdCounter);
           }		 

        }



        Clr_ms50Flag();
    }
    if(SecondFlag)
    {

#if PANEL_VBY1()
        g_u8VBY1CheckFlag = 1;
#endif

#if ENABLE_DAISY_CHAIN
        if((g_u8Second % DAISY_CHAIN_MST_CHECK_TIME) == 0)
            g_u8DCMSTCheckFlag = 1;
#endif


            g_stMenuFlag.bChangePattern=1;
        
        g_u8Second++;

#if SleepMode_Enable
    if((SleepModeCounter)&&(BacklightFlag)
	#if ENABLE_MULTI_WINDOW_SETTINGS_MENU	
		&&(!((Is_MultiWin_SyncLoss()&&USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)||((SyncLossState(USER_PREF_INPUT_TYPE)) && USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)))
       #else
		&&(!(SyncLossState(USER_PREF_INPUT_TYPE)))
	#endif   
	)
    {
         SleepModeCounter--;
	 if(SleepModeCounter==30)
	 	Set_ForceEnterPowerOffMsgFlag();
	  Set_EnterPowerOffMsgChangeNumFlag();
	if(SleepModeCounter==0)	
	{
           Set_ForceEnterPowerOffFlag();
	    Clr_ForceEnterPowerOffMsgFlag();	   
	}
    }
#endif

        #if ENABLE_MMD_DCR//Check BL status
        if(ENABLE_SmartConntrast())		
        	SmartConntrastTimerHandler();
        #endif

#if 0//USEFLASH
        UserPref_FlashSaveFlagCheck();
#endif

       g_stMenuFlag.bMoveOsd=1;

#if Enable_Corss_LHCX_New
	if(HotGameTimeShowFlag)
	{	
	
		if (GameModeTime_S>0||GameModeTime_M>0)
	    	{
	    	    Set_HotGameTimeUpdateFlag();
			if(GameModeTime_S)
			{
				GameModeTime_S--;
			}
			else if(GameModeTime_M)
			{
				GameModeTime_M--;
				GameModeTime_S=59;
			}
	    	}
		else
		{
			GameModeTime_M=0;
			GameModeTime_S=0;
		}
	}
#endif

        if (g_u8HoldKeyCounter)
        {
            if (--g_u8HoldKeyCounter == 0)
            	{
                g_stMenuFlag.bOsdTimeout=1;
            	}
            else if (g_u16OsdCounter>g_u8HoldKeyCounter)
                g_u16OsdCounter--;
        }
        else if (g_u16OsdCounter) //g_u8HoldKeyCounter hi priority
        {
            //Main_printData("OsdCounter=%d\r\n",g_u16OsdCounter);
            if (--g_u16OsdCounter == 0)
            	{
                g_stMenuFlag.bOsdTimeout=1;
            	}
        }

	 	
        if(g_u8PowerDownCounter
 #if ((OSD_LOCK_Enable)||(LHCX_EnterFactoryMode)||(defined(RunFeng_Project)))
		&& !g_stMenuFlag.bHoldMenuKey
 #endif
        )
        {
            if( --g_u8PowerDownCounter == 0 )
                Set_ForcePowerSavingFlag();
        }


        if( POWER_ON_FLAG && !PowerSavingFlag )
        {
            USER_PREF_BACKLIGHT_TIME++;
            if(FACTORY_MODE_FLAG||DoBurninModeFlag)
            {
                if((USER_PREF_BACKLIGHT_TIME%600) == 0) //10 Min
                {
                #if (!USEFLASH)
                    SystemBlockSave();
                #endif
                }
            }
            else
            {
                if((USER_PREF_BACKLIGHT_TIME%900) == 0) //15 min
                {
                #if (!USEFLASH)
                    SystemBlockSave();
                #endif
                }
            }
        }

#if 0//ENABLE_TOUCH_KEY
    #if ENABLE_LOW_CONTACT
        if(TouchKeyLowContactDetectCntr)
            TouchKeyLowContactDetectCntr--;
    #endif
        if((g_u8Second%2) == 0)
        {
            if(!TouchKeyRestortCounter) // 120417 coding addition
                TouchKeyRestortCounter = TouchKey_Check_ESD();
        }
#endif

   #if SwitchPortByDP_Detect
     if(USER_PREF_INPUT_TYPE==INPUT_HDMI)
     {
       if(!hw_DP_DET_Pin)
       {
          USER_PREF_INPUT_TYPE=INPUT_DISPLAYPORT;
          SwitchPort( USER_PREF_WIN_SEL, USER_PREF_INPUT_TYPE );
          APPInput_DetectSyncStableCntInit();
       }
	else
	{
           ;
	}
     }
    else	
    {
      if(hw_DP_DET_Pin)
      {
          USER_PREF_INPUT_TYPE=INPUT_HDMI;
          SwitchPort( USER_PREF_WIN_SEL, USER_PREF_INPUT_TYPE );
          APPInput_DetectSyncStableCntInit();
      }
    }
   #endif



        if (g_u8DDCCISaveCounter>=1)
            g_u8DDCCISaveCounter++;
        if(g_u8DDCCISaveCounter>2 && (!FACTORY_MODE_FLAG )) //JL 20070726
        {
            SaveUserPref();
            g_u8DDCCISaveCounter=0;//CLR_DDCCI_SAVE_FLAG() ;
        }
#if ENABLE_AUDIO_SETTINGS_MENU
#if ((!ENABLE_ADJ_AUDIO_OUTPUT)&&(!Audio_Input_Jack_Only_For_Line_In)&&(!Audio_Input_Jack_Only_For_Output))
       EarphoneHandler();
#endif
#endif		


 #if EnableDdcLed
 if(USER_PREF_LEDMODE==OffLedMode)
 {
	hw_ClrGreenLed();
	hw_ClrAmberLed();
 }
 else
 {
 if(POWER_ON_FLAG&&InputTimingStableFlag(SrcInputPortM)&&(!InputTimingChangeFlag(SrcInputPortM))&&(g_u8MenuPageIndex!=MENU_POWER_ON&&g_u8MenuPageIndex!=MENU_POWER_OFF)&&(PowerSavingFlag||(BURNIN_MODE_FLAG&&SyncLossState(USER_PREF_INPUT_TYPE))))
 {

   
     if(LEDModeCounter>=200)
	 	LEDModeCounter=0;   
     if (LedSlideFlag)
     	{
         Clr_ledSlideFlag(); 
         hw_ClrGreenLed();
         hw_ClrAmberLed();
     	}
     else
     	{
         Set_LedSlideFlag();
         LEDModeCounter++;	 

	if(LEDModeCounter%2==0)
	{
         hw_SetGreenLed();
	}
	else
	{
         hw_SetAmberLed();
	}

      }


   
 }
else if(POWER_ON_FLAG&&(g_u8MenuPageIndex!=MENU_POWER_ON&&g_u8MenuPageIndex!=MENU_POWER_OFF)&&(!SyncLossState(USER_PREF_INPUT_TYPE)))
 {
          if(!LedSlideFlag)
          {
                  Set_LedSlideFlag();
                  hw_SetGreenLed();
		    hw_ClrAmberLed();		  
		    LEDModeCounter=0;		  
          }
 }
 }
 #else
 if(POWER_ON_FLAG&&InputTimingStableFlag(SrcInputPortM)&&(!InputTimingChangeFlag(SrcInputPortM))&&(g_u8MenuPageIndex!=MENU_POWER_ON&&g_u8MenuPageIndex!=MENU_POWER_OFF)&&(PowerSavingFlag||(BURNIN_MODE_FLAG&&SyncLossState(USER_PREF_INPUT_TYPE))))
 {
     if (LedSlideFlag)
     	{
         Clr_ledSlideFlag(); 
         hw_ClrGreenLed();
     	}
     else
     	{
         Set_LedSlideFlag();
         hw_SetGreenLed();
       }
 }
else if(POWER_ON_FLAG&&(g_u8MenuPageIndex!=MENU_POWER_ON&&g_u8MenuPageIndex!=MENU_POWER_OFF)&&(!SyncLossState(USER_PREF_INPUT_TYPE)))
 {
          if(!LedSlideFlag)
          {
                  Set_LedSlideFlag();
                  hw_SetGreenLed();
          }
 }
#endif






        Clr_SecondFlag();


    }


#if SleepMode_Enable
 if(ForceEnterPowerOffFlag)
 {
     Clr_ForceEnterPowerOffFlag();
     Clr_ForceEnterPowerOffMsgFlag();	   
     ExecuteKeyEvent(MIA_POWER);
 }
#endif

#if 0
    if( SaveBlacklitTimeFlag )
    {
        SaveBlacklitTimeFlag = 0;
    #if 0//USEFLASH
        UserprefBacklighttime = BlacklitTime;
        UserPref_EnableFlashSaveBit( bFlashForceSaveMonitor2Bit );
    #else
        SaveBlacklitTime();
    #endif
    }
#endif
#if 0//ENABLE_TOUCH_KEY
#if TOUCH_KEY_CTRL_LED
    if(TouchKey_GetLEDStatus() != TouchKeySetLEDStatus)
        TouchKey_CtrlLED(TouchKeySetLEDStatus);
#endif
#endif

}




#define BackLightCounter                  2000
#define SCREENBlackCounter             1200


#if (VBY1_PowerSequence)
void BackLightHandel(void)
{
    if(BackLightOnFlag&&(( SyncLossState(USER_PREF_INPUT_TYPE)||UnsupportedModeFlag(USER_PREF_INPUT_TYPE))||((g_DisplayWindow[DISPLAY_WIN1].eDwStatus==WIN_STATUS_OUTPUT_DISPLAYOK)&&( !SyncLossState(USER_PREF_INPUT_TYPE)&&!UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))
#if LOGO_ENABLE
    ||(g_u8MenuPageIndex==MENU_LOGO)
#endif    
	))
 {

  if( SyncLossState(USER_PREF_INPUT_TYPE)||UnsupportedModeFlag(USER_PREF_INPUT_TYPE))
        PowerOnBackLightCounter=500;
  else
  {
     if((g_DisplayWindow[DISPLAY_WIN1].eDwStatus==WIN_STATUS_OUTPUT_DISPLAYOK)&&( !SyncLossState(USER_PREF_INPUT_TYPE)&&!UnsupportedModeFlag(USER_PREF_INPUT_TYPE)))
        PowerOnBackLightCounter=(BackLightCounter-200);
     else
        PowerOnBackLightCounter=BackLightCounter;
  }
	 Clr_BackLightOnFlag();	

	 printData("PowerOnBackLightCounter==%d", PowerOnBackLightCounter);

}

	if (PowerOnBackLightCounter)
	  if (--PowerOnBackLightCounter == 0)
	  {
	  #if PANEL_VBY1()
	     if(!VBY1TrainingPassFlag)
	     {
		 PowerOnBackLightCounter=100;	
		 return;
	     }
	 #endif	 
            if((!BacklightFlag))
            	{
               #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
              if((USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)&&(BURNIN_MODE_FLAG))
              {
                      BYTE u8win;
                      for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                      {
              	     msAPIWinFrameColorRGB(u8win, DEF_FRAME_COLOR_PATTERN);		
                      }
              }
              #endif
           	
            	    printMsg("hw_SetBlacklit 0000");

            	    mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);

            #if 0//((LOGO_ENABLE)&&(MS_PANEL_TYPE==PanelHK395WLEDM_CH16H))
 		if(g_u8MenuPageIndex==MENU_LOGO)
                 ForceDelay1ms(1200);// need check
 	       else
	     #endif
		    ForceDelay1ms(10);
                  hw_SetBlacklit();

            	}
	  }


#if 0	  
   if(BW_SCREEN_ONOFFBitFlag)
   {

      Clr_BW_SCREEN_ONOFFBitFlag();
	  
     if(TIMING_CHANGE_FLAG)	  
     {
       BW_SCREEN_ONOFFCounter=SCREENBlackCounter;
	CLR_TIMING_CHANGE_FLAG();	 
	printMsg("CLR_TIMING_CHANGE_FLAG");
     }
    else	 
     {

     if((g_DisplayWindow[DISPLAY_WIN1].eDwStatus==WIN_STATUS_OUTPUT_DISPLAYOK)&&( InputTimingStableFlag(USER_PREF_INPUT_TYPE))&&( !SyncLossState(USER_PREF_INPUT_TYPE)))
     	{
     	   printMsg("111111111");
          BW_SCREEN_ONOFFCounter=SCREENBlackCounter;
     	}
     else
     	{
     	    printMsg("000000000000000");
	  if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)		
          BW_SCREEN_ONOFFCounter=(SCREENBlackCounter+600);
	  else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1)
          BW_SCREEN_ONOFFCounter=(SCREENBlackCounter-200);
         else
          BW_SCREEN_ONOFFCounter=(SCREENBlackCounter-200);
     	}
	 
     }
   }

       if(0)//((g_DisplayWindow[DISPLAY_WIN1].eDwStatus==WIN_STATUS_OUTPUT_DISPLAYOK)&&(BW_SCREEN_ONOFFCounter!=0))
       {
          //printMsg("33333");
	   mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
          Clr_BW_SCREEN_ONOFFBitFlag();		
          BW_SCREEN_ONOFFCounter=0;	
       }
	else
	{
	
	if (BW_SCREEN_ONOFFCounter)
	  if (--BW_SCREEN_ONOFFCounter == 0)
	  {

	         printMsg("444444444444");

              #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
              if((USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)&&(BURNIN_MODE_FLAG))
              {
                      BYTE u8win;
                      for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                      {
              	     msAPIWinFrameColorRGB(u8win, DEF_FRAME_COLOR_PATTERN);		
                      }
              }
              #endif
			 
           	   mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
	  }
	  
	}
#endif

}
#endif



#undef MAIN_C

