#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "menudef.h"
#include "menufunc.h"
#include "Menu.h"
#include "Adjust.h"
//#include "msADC.h"
#include "drvADC.h"
#include "ms_reg.h"
#include "msOSD.h"
//#include "userPref.h"
#include "AutoFunc.h"
#include "Ms_rwreg.h"
#include "MDebug.h"
#include "Common.h"
#include "misc.h"
#include "mStar.h"
#include "Power.h"
#include "Keypaddef.h"
#include "DDC2Bi.H"
//#include "userpref.h"
#include "msflash.h"
#include "NVRam.h"
#include "GPIO_DEF.h"
#include "FMenuFunc.h"
#include "Mode.h"

#if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif

#if ENABLE_DPS
#include "drvOSD.h"
#include "drvGPIO.h"
#endif

#define HKC_SETSUBWINDOW		  0
////////////////////////////////////////////////////
#include "drvDDC2Bi.h"
//#include "halRwreg.h"
///////////////////////////////////////////////////
Bool ResetAllSetting(void);
void SetColorTempDDCCI(void);
Bool SetFactoryColorTemp9300K(void);
Bool SetFactoryColorTemp5700K(void);
Bool SetFactoryColorTemp6500K(void);
void SaveModeSetting(void);
void SaveFactorySetting(void);
void SaveUserPref(void);
extern Bool ExecuteKeyEvent(MenuItemActionType menuAction);
extern WORD GetClockValue(void); // adjust range
extern WORD GetPhaseValue(void);
extern WORD GetScale100Value(WORD u16Value, WORD u16MinValue, WORD u16MaxValue); //foxconn alan 061214-40 061217
#if (ODM_NAME==ODM_WISTRON)
extern BYTE  WistronAlignControl(void);
#endif
#if HKC_ColorTemp_Adjust
extern BYTE HKCAlignControl(void);
#endif

#if Disable_DDCCI
#if ENABLE_VGA_INPUT
BYTE code CAP_VCP[] =
{
    "("
    ")"
};
#endif
#if ENABLE_DIGITAL_INPUT
BYTE code DVI_CAP_VCP[] =
{
    "("
    ")"
};
#endif

#else

#if ENABLE_VGA_INPUT
BYTE code CAP_VCP[] =
{
#ifdef _MCCS_VER3_
    "(prot(display)type(lcd)"
#else
    "(prot(monitor)type(lcd)"
#endif
    MCCS_MODEL_NAME
    "cmds(01 02 03 07 0C E3 F3)vcp"

    "(02 04 05 06 08 0E 10 12 "

    "14(05 08 0B 0C) "  //remove sRGB

    "16 18 1A 1E 20 30 3E 52 "

    "60("
#if ENABLE_VGA_INPUT
    "01"
#endif
#if ENABLE_DVI_INPUT
    " 03"
  #if ENABLE_DVI2ND_INPUT
    " 04"
  #endif
#endif
#if ENABLE_HDMI_INPUT
    " 11"
  #if ENABLE_HDMI2ND_INPUT
    " 12"
  #endif
#endif
#if ENABLE_DP_INPUT
    " 0F"
  #if ENABLE_DP2ND_INPUT
    " 10"
  #endif
#endif
    ") "
#if 0//ENABLE_OSD_ROTATION
    "AA(01 02) "
#endif
    "AC AE B2 B6 "
#if !defined(_DISABLE_AM_SUPPORT_)
    "C6 "
#endif
#if 0
    "C8 C9 D6(01 05)"
#else
    "C8 C9 D6(01 05) DC(00 03 "
    "04 "
    "05 "
    "06 "
    "09 "
    ")"
#endif    
    " DF"
#if !defined(_DISABLE_MANUFACTURER_ID_)
    " FD"
#endif
    ")mccs_ver("
#ifdef _MCCS_VER3_
    "3.0"
#else
    "2.1"
#endif
    ")mswhql(1))"
};
#endif
#if ENABLE_DIGITAL_INPUT
BYTE code DVI_CAP_VCP[] =
{
#ifdef _MCCS_VER3_
    "(prot(display)type(lcd)"
#else
    "(prot(monitor)type(lcd)"
#endif
    MCCS_MODEL_NAME
    "cmds(01 02 03 07 0C E3 F3)vcp"

    "(02 04 05 08 10 12 "

    "14(05 08 0B 0C) "

    "16 18 1A 52 "

    "60("
#if ENABLE_VGA_INPUT
    "01"
#endif
#if ENABLE_DVI_INPUT
    " 03"
  #if ENABLE_DVI2ND_INPUT
    " 04"
  #endif
#endif
#if (ENABLE_HDMI_INPUT)
    " 11"
  #if ENABLE_HDMI2ND_INPUT
    " 12"
  #endif
#endif
#if (ENABLE_DP_INPUT)
    " 0F"
  #if ENABLE_DP2ND_INPUT
    " 10"
  #endif
#endif
    ") "
#if 0//ENABLE_OSD_ROTATION
    "AA(01 02) "
#endif
    "AC AE B2 B6 "
#if !defined(_DISABLE_AM_SUPPORT_)
    "C6 "
#endif
#if 0
    "C8 C9 D6(01 05)"
#else
    "C8 C9 D6(01 05) DC(00 03 "
    "04 "
    "05 "
    "06 "
    "09 "
    ")"
#endif    
    " DF"
#if !defined(_DISABLE_MANUFACTURER_ID_)
    " FD"
#endif
    ")mccs_ver("
#ifdef _MCCS_VER3_
    "3.0"
#else
    "2.2"
#endif
    ")mswhql(1))"
};
#endif
#endif
BYTE code tVersionString[]=VERSION_STRING;
static BYTE ASCII2Value(BYTE u8Char)
{
    if (u8Char<='9' && u8Char>='0')
        return u8Char-'0';
#if 0
    else if (u8Char<='F' && u8Char>='A')
        return u8Char-'A'+10;
#endif
    return 0;
}
static BYTE GetMajorVer(void)
{
    return ASCII2Value(tVersionString[3]);
}
static BYTE GetMinorVer(void)
{
    BYTE u8MinorVer;
    u8MinorVer=ASCII2Value(tVersionString[4])*10;
    u8MinorVer+=ASCII2Value(tVersionString[5]);
    return u8MinorVer;
}
/////////////////////////////////////////////////////////////
//====================================================================
//
//====================================================================
void force_update_user_color(void)
{
/*
    if(CURRENT_COLOR_TEMP==PCT_5700K)
    {
        CURRENT_RED_COLOR=USER_PREF_RED_COLOR_5700K;
        CURRENT_GREEN_COLOR=USER_PREF_GREEN_COLOR_5700K;
        CURRENT_BLUE_COLOR=USER_PREF_BLUE_COLOR_5700K;
    }
    else if(CURRENT_COLOR_TEMP==PCT_9300K)
    {
        CURRENT_RED_COLOR=USER_PREF_RED_COLOR_9300K;
        CURRENT_GREEN_COLOR=USER_PREF_GREEN_COLOR_9300K;
        CURRENT_BLUE_COLOR=USER_PREF_BLUE_COLOR_9300K;
    }
    else
    {
        CURRENT_RED_COLOR=USER_PREF_RED_COLOR_6500K;
        CURRENT_GREEN_COLOR=USER_PREF_GREEN_COLOR_6500K;
        CURRENT_BLUE_COLOR=USER_PREF_BLUE_COLOR_6500K;
    }
*/
    CURRENT_COLOR_TEMP=PCT_USER;
}
void SetColorTempDDCCI(void)
{
    Menu_AdjustColorSetting();
    //Charles
#if 0
    g_u8DDCCIAControl=0;
#endif
//if (!FACTORY_MODE_FLAG) ////JL 20070726
 //   SaveMonitorSetting(); //JL 20070726
}
BYTE xdata u8CommandCode;
BYTE xdata u8CPCode;
BYTE xdata u8ValueH;
BYTE xdata u8ValueL;
BYTE xdata u8RetValueH;
BYTE xdata u8RetValueL;
BYTE xdata u8PageValue;
WORD xdata u16Value;
DWORD xdata DWordValue;
extern void SetLedFunc(BYTE LedNumber, BYTE LedColor,BYTE LedStrength,BYTE LedColorMode);


extern WORD DdcciCheck(unsigned int index);
BYTE AlignControl( void )
{
    WORD u16TempValue;
    BYTE u8TPValue;
    BYTE i;
    BYTE SCDetWin;
#if HKC_ColorTemp_Adjust	
    BYTE Resault=0;	
#endif

    u8CommandCode = DDCBuffer[1];
   u8CPCode = DDCBuffer[2];
 #if HKC_ColorTemp_Adjust
    if((u8CommandCode==SETVCPFeature)&&(u8CPCode==0x7B))
	;
    else	
 #endif
 {
    if(u8CommandCode != MStar_COMMAND && !(u8CommandCode == MCCS_COLORAP_COMMAND_READ || u8CommandCode == MCCS_COLORAP_COMMAND_WRITE))
    {
        SCDetWin = MapPort2DetScIndex(rxInputPort);
        if(SCDetWin==SC_MAX_NUMS)
        {
            return FALSE; //depends on spec ?? prevent if SCDetWin == SC_MAX_NUMS, out of arrar index access(if input source connected but no scaler assigned to it)
        }
    }
   #if Disable_DDCCI	
      #if ENABLE_HDMI_DRR_MCCS
	 if((u8CommandCode == GETVCPFeature || u8CommandCode == GETPagedVCP)&&(u8CPCode == HDMI_FREESYNC_VCP))
	 	;
	 else
      #endif
	return FALSE; 
   #endif 
 }

    //*******************************************************************************************
    //MSTART COMMAND***************************************************************************
    //*******************************************************************************************
    if(u8CommandCode == MStar_COMMAND || u8CommandCode == MCCS_COLORAP_COMMAND_READ || u8CommandCode == MCCS_COLORAP_COMMAND_WRITE)
    {
        return ColorCalibrationHandler(u8CommandCode);
    }

    u8PageValue = 0;
    if(u8CommandCode == SETVCPFeature)
    {
        u8CPCode = DDCBuffer[2];
        u8ValueH = DDCBuffer[3];
        u8ValueL = DDCBuffer[4];
        u16Value = ((WORD)DDCBuffer[3]<<8) | ((WORD)DDCBuffer[4]);
    }
    else if(u8CommandCode == GETVCPFeature)
    {
        u8CPCode = DDCBuffer[2];
    }
    else if(u8CommandCode == GETPagedVCP)
    {
        u8PageValue = DDCBuffer[2];
        u8CPCode = DDCBuffer[3];
    }
    else if(u8CommandCode == SETPagedVCP)
    {
        u8PageValue = DDCBuffer[2];
        u8CPCode = DDCBuffer[3];
        u8ValueH = DDCBuffer[4];
        u8ValueL = DDCBuffer[5];
        u16Value = ((WORD)DDCBuffer[4]<<8) | ((WORD)DDCBuffer[5]);
    }
    else if(u8CommandCode == CapabilitiesRequest)
    {
        if (g_u8DDCCISaveCounter)
            g_u8DDCCISaveCounter=1;//ResetMsCounter();   //for Vista issue, Let main loop not to save data till all test command stop. //JL
        u8RetValueH = DDCBuffer[2];
        u8RetValueL = DDCBuffer[3];
        u16Value = ((WORD)DDCBuffer[2]<<8) | ((WORD)DDCBuffer[3]);
    #if ENABLE_VGA_INPUT
        if(INPUT_IS_VGA(rxInputPort))
            u16TempValue = sizeof(CAP_VCP);
    #endif
    #if ENABLE_DIGITAL_INPUT
        if(INPUT_IS_TMDS(rxInputPort)
       #if ENABLE_DP_INPUT
           ||INPUT_IS_DISPLAYPORT(rxInputPort)
       #endif
           )
            u16TempValue = sizeof(DVI_CAP_VCP);
    #endif
        if(u16Value >= u16TempValue)
        {
            DDCBuffer[0]=0x83;
            DDCBuffer[1]=0xE3;
            return 3;
        }
    #define CAP_LEN u8CPCode
        CAP_LEN = (u16Value+32 >= u16TempValue)?(u16TempValue-u16Value):32;
        DDCBuffer[0]=CAP_LEN+3+0x80;
        DDCBuffer[1]=0xE3;
        DDCBuffer[2]=u8RetValueH;
        DDCBuffer[3]=u8RetValueL;
        for(u8ValueL = 4;u8ValueL<CAP_LEN+4;u8ValueL++)
        {   //Jison 080320
        #if ENABLE_VGA_INPUT
            if(INPUT_IS_VGA(rxInputPort))
                DDCBuffer[u8ValueL] = CAP_VCP[u16Value+u8ValueL-4];
        #endif
        #if ENABLE_DIGITAL_INPUT
            if(INPUT_IS_TMDS(rxInputPort)
          #if ENABLE_DP_INPUT
               ||INPUT_IS_DISPLAYPORT(rxInputPort)
          #endif
               )
                DDCBuffer[u8ValueL] = DVI_CAP_VCP[u16Value+u8ValueL-4];
        #endif
        }
    #undef CAP_LEN
        return DDCBuffer[0]&~0x80;
    }


    else if(u8CommandCode == EnableApp)
    {
        return 0xFE;
    }
    else if(u8CommandCode == AppTest)
    {
        return 0xFD;
    }
    else if(u8CommandCode == GetTimingRequest)
    {
        DDCBuffer[0] = 0x06;
        DDCBuffer[1] = 0x4E;
        DDCBuffer[2] = SyncPolarity(SrcFlags(rxInputPort));
        if(UnsupportedModeFlag(rxInputPort))
            DDCBuffer[2] |= 0x80;
        if(!InputTimingStableFlag(rxInputPort))
            DDCBuffer[2] |= 0x40;
        u16Value = HFREQ( SrcHPeriod(rxInputPort))*10;
        DDCBuffer[3] = u16Value >> 8;
        DDCBuffer[4] = u16Value & 0x00FF;
        u16Value = VFREQ(u16Value,SrcVTotal(rxInputPort));
        DDCBuffer[5] = u16Value >> 8;
        DDCBuffer[6] = u16Value & 0x00FF;
        return 6;
    }

    //preliminary judge cmd finish, start detail process--------------------------------------------------
    //preliminary judge cmd finish, start detail process--------------------------------------------------
#if  (ODM_NAME==ODM_WISTRON)    //061129
    i= WistronAlignControl() ;
    if(i!=0x55)
        return i;
#endif
    if(u8CommandCode == GETVCPFeature || u8CommandCode == GETPagedVCP)
    {
        if (g_u8DDCCISaveCounter)
            g_u8DDCCISaveCounter=1;//ResetMsCounter();   //for Vista issue, Let main loop not to save data till all test command stop. //JL
        if(u8CommandCode == GETVCPFeature)
        {
          #if HKC_PCToolCommand
    		 if (u8CPCode == HKC_COMMUNICATION_COMMAND_CHECK)                
    			u16Value = ((WORD)DDCBuffer[3] << 8) | ((WORD)DDCBuffer[4]);
	   #endif
        
            u8ValueH = 0x00;
            u8ValueL = 0xFF;    // one
            u8RetValueH = 0x00;

            DDCBuffer[0] = 0x88;
            DDCBuffer[1] = 0x02;
            DDCBuffer[2] = 0x00;
            DDCBuffer[3] = u8CPCode;
            DDCBuffer[4] = 0x01;
            DDCBuffer[5] = 0x00;
            //u8ValueH = 0x00;
            //u8ValueL = 0xFF;   // one
            //u8RetValueH = 0x00;

            u8TPValue = 0x01;
        }
        else
        {
            DDCBuffer[0] = 0x89;
            DDCBuffer[1] = 0xC7;
            DDCBuffer[2] = 0x00;
            DDCBuffer[3] = u8PageValue;
            DDCBuffer[4] = u8CPCode;
            DDCBuffer[5] = 0x01;
            DDCBuffer[6] = 0x00;
            //DDCBuffer[7] = 0xFF;
            u8ValueH = 0x00;
            u8ValueL = 0xFF;
            u8RetValueH = 0x00;
            u8TPValue = 0x01;
        }
        //GETVCPFeature or GETPagedVCP, start u8CPCode process-------------------------------------
        if((u8CPCode == ALL_Reset||u8CPCode==Geometry_Reset||u8CPCode==Color_Reset) && u8PageValue == 0)
        {
            u8RetValueL = 0;
        }
	#if ENABLE_HDMI_DRR_MCCS
	else if(u8CPCode == HDMI_FREESYNC_VCP)
	{			
		u8TPValue = 0x00;
		u8ValueH = 0x00;
              u8ValueL = 0x01;
              u8RetValueH = 0x00;
		u8RetValueL = (FreeSyncFlag?1:0);
	}
	#endif
        else if(u8CPCode == NewControl&& u8PageValue == 0)
        {
            if(u8ButtonFIFO[0]!=0)
                u8RetValueL = 2;
            else
                u8RetValueL = 1;
            u8ValueL=2;
        }
        else if(u8CPCode == RestoryFactory && u8PageValue == 0)
        {
            u8RetValueL = 0;
            u8ValueL = 1;
        }
        else if(u8CPCode == Auto_Setup&& u8PageValue == 0)
        {
            u8RetValueL = 0;
            u8ValueL = 2;
        }
        else if(u8CPCode == ADJ_Brightness && u8PageValue == 0)
        {
            u8RetValueL = USER_PREF_BRIGHTNESS;
            u8ValueL = 100;
            u8TPValue = 0x00;
        }
        else if(u8CPCode==ColorTempInc && u8PageValue == 0)
        {
            u8ValueL = 1500&0xFF;
            u8ValueH = 1500>>4;
        }
        else if(u8CPCode==ColorTempRequest&& u8PageValue == 0)
        {
            u8ValueL = 0xFF;
            u8ValueH = 0;
            if (CURRENT_COLOR_TEMP == PCT_6500K)
                u8RetValueL=0x01;
            else
                u8RetValueL=0x02;
        }
        else if(u8CPCode==ActiveControl)
        {
            u8ValueL = 0xFF;
            u8ValueH = 0;
            u8RetValueH=0x00;
            //u8RetValueL=g_u8DDCCIAControl;
            u8RetValueL=u8ButtonFIFO[0];
            //u8ButtonFIFO[0]=0;
        }
        else if(u8CPCode == ADJ_Contrast && u8PageValue == 0)
        {
            u8ValueL = (MAX_CONTRAST_VALUE-MIN_CONTRAST_VALUE);
         if(CURRENT_PICTURE_MODE==CT_USER)
            u8RetValueL = GET_USER_PREF_CONTRAST_USER(USER_PREF_WIN_SEL) - MIN_CONTRAST_VALUE;
	  else	  
            u8RetValueL = GET_USER_PREF_CONTRAST(USER_PREF_WIN_SEL) - MIN_CONTRAST_VALUE;
            u8TPValue = 0x00;
        }
        else if(u8CPCode == Select_Color_Preset && u8PageValue == 0)
        {
            u8ValueL = 0x0C;//0x0B;

            if(CURRENT_COLOR_TEMP== PCT_USER)
                u8RetValueL = 0x0C;
            else if(CURRENT_COLOR_TEMP== PCT_5700K)
                u8RetValueL = 0x0B;
            else if(CURRENT_COLOR_TEMP== PCT_6500K)
                u8RetValueL = 0x05;
            else if(CURRENT_COLOR_TEMP== PCT_9300K)
                u8RetValueL = 0x08;
            //else if(CURRENT_COLOR_TEMP== CTG_SRGB)
                //u8RetValueL = 0x01;
            else
                u8RetValueL = 0x05;//0x0C;
            u8TPValue = 0x00;
        }
        else if(u8CPCode == DisplayApplication && u8PageValue == 0)
        {
            if (CURRENT_COLOR_TEMP==CT_USER)
                u8RetValueL = 0x04;
            else if(CURRENT_COLOR_TEMP== CT_MOVIE)
                u8RetValueL = 0x03;
            else if(CURRENT_COLOR_TEMP== CT_VIVID)
                u8RetValueL = 0x09;
            else if(CURRENT_COLOR_TEMP== CT_FPS)
                u8RetValueL = 0x06;
            else if(CURRENT_COLOR_TEMP== CT_RTS)
                u8RetValueL = 0x05;
            else
                u8RetValueL = 0x00;  //CT_STANDARD
            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_Red_Gain && u8PageValue == 0)
        {
            u8ValueH = 0x00;
            u8ValueL = 100;//DEF_RED_COLOR;//0xFF;

            if(CURRENT_COLOR_TEMP== PCT_USER)
                u8RetValueL = GetScale100Value(CURRENT_RED_COLOR,0,MAX_USER_R_COLOR_VALUE);//(WORD)(USER_PREF_RED_COLOR*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_6500K)
                u8RetValueL = GetScale100Value(USER_PREF_RED_COLOR_6500K,0,MAX_COLOR_VALUE);//(WORD)(USER_PREF_RED_COLOR_6500K*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_9300K)
                u8RetValueL = GetScale100Value(USER_PREF_RED_COLOR_9300K,0,MAX_COLOR_VALUE);//USER_PREF_RED_COLOR_6500K(WORD)(USER_PREF_RED_COLOR_9300K*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_5700K)
                u8RetValueL = GetScale100Value(USER_PREF_RED_COLOR_5700K,0,MAX_COLOR_VALUE);
            //else if(CURRENT_COLOR_TEMP== CTG_SRGB)
            //    u8RetValueL = GetScale100Value(USER_PREF_RED_COLOR_SRGB,0,MAX_COLOR_VALUE);//(WORD)(USER_PREF_RED_COLOR_SRGB*100)/MAX_COLOR_VALUE;//*255)/100;
            else
                u8RetValueL = GetScale100Value(CURRENT_RED_COLOR,0,MAX_USER_R_COLOR_VALUE);//(WORD)(USER_PREF_RED_COLOR*100)/MAX_COLOR_VALUE;//*255)/100;

            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_Green_Gain && u8PageValue == 0)
        {
            u8ValueH = 0x00;
            u8ValueL = 100;

            if(CURRENT_COLOR_TEMP== PCT_USER)
                u8RetValueL = GetScale100Value(CURRENT_GREEN_COLOR,0,MAX_USER_G_COLOR_VALUE);//(WORD)(USER_PREF_GREEN_COLOR*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_6500K)
                u8RetValueL = GetScale100Value(USER_PREF_GREEN_COLOR_6500K,0,MAX_COLOR_VALUE);//(WORD)(USER_PREF_GREEN_COLOR_6500K*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_9300K)
                u8RetValueL = GetScale100Value(USER_PREF_GREEN_COLOR_9300K,0,MAX_COLOR_VALUE);//(WORD)(USER_PREF_GREEN_COLOR_9300K*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_5700K)
                u8RetValueL = GetScale100Value(USER_PREF_GREEN_COLOR_5700K,0,MAX_COLOR_VALUE);
            //else if(CURRENT_COLOR_TEMP == CTG_SRGB)
            //    u8RetValueL = GetScale100Value(USER_PREF_GREEN_COLOR_SRGB,0,MAX_COLOR_VALUE);//(WORD)(USER_PREF_GREEN_COLOR_SRGB*100)/MAX_COLOR_VALUE;//*255)/100;
            else
                u8RetValueL = GetScale100Value(CURRENT_GREEN_COLOR,0,MAX_USER_G_COLOR_VALUE);//(WORD)(USER_PREF_GREEN_COLOR*100)/MAX_COLOR_VALUE;//*255)/100;

            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_Blue_Gain && u8PageValue == 0)
        {
            u8ValueH = 0x00;
            u8ValueL = 100;//DEF_BLUE_COLOR;//0xFF;

            if(CURRENT_COLOR_TEMP== PCT_USER)
                u8RetValueL = GetScale100Value(CURRENT_BLUE_COLOR,0,MAX_USER_B_COLOR_VALUE);//(WORD)(USER_PREF_BLUE_COLOR*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_6500K)
                u8RetValueL = GetScale100Value(USER_PREF_BLUE_COLOR_6500K,0,MAX_COLOR_VALUE);//(WORD)(USER_PREF_BLUE_COLOR_6500K*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_9300K)
                u8RetValueL = GetScale100Value(USER_PREF_BLUE_COLOR_9300K,0,MAX_COLOR_VALUE);//(WORD)(USER_PREF_BLUE_COLOR_9300K*100)/MAX_COLOR_VALUE;//*255)/100;
            else if(CURRENT_COLOR_TEMP== PCT_5700K)
                u8RetValueL = GetScale100Value(USER_PREF_BLUE_COLOR_5700K,0,MAX_COLOR_VALUE);
            //else if(CURRENT_COLOR_TEMP == CTG_SRGB)
            //    u8RetValueL = GetScale100Value(USER_PREF_BLUE_COLOR_SRGB,0,MAX_COLOR_VALUE);//(WORD)(USER_PREF_BLUE_COLOR_SRGB*100)/MAX_COLOR_VALUE;//*255)/100;
            else
                u8RetValueL = GetScale100Value(CURRENT_BLUE_COLOR,0,MAX_USER_B_COLOR_VALUE);//(WORD)(USER_PREF_BLUE_COLOR*100)/MAX_COLOR_VALUE;//*255)/100;

            u8TPValue = 0x00;
        }
#if ENABLE_VGA_INPUT		
        else if(u8CPCode == ADJ_HorizontalPos && u8PageValue == 0)
        {
        #if ENABLE_VGA_INPUT
            u8ValueH = 0;//(MAX_H_START - MIN_H_START)>> 8;
            u8ValueL = 100;//(MAX_H_START - MIN_H_START) & 0x00FF;
            u8RetValueH =0;// (MAX_H_START - USER_PREF_H_START) >> 8;
            u8RetValueL = (MAX_H_START(rxInputPort) - USER_PREF_H_START(rxInputPort))&0x00FF;
        #else
            u8ValueH = 0;
            u8ValueL = 100;
            u8RetValueH = 0;
            u8RetValueL = 0;
        #endif
            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_VerticalPos && u8PageValue == 0)
        {
        #if ENABLE_VGA_INPUT
            u8ValueH = 0;//(MAX_V_START) >> 8;//0;//(MAX_V_START) >> 8;
            u8ValueL = 100;//(MAX_V_START) & 0x00FF;//100;//(MAX_V_START) & 0x00FF;
            u8RetValueH = 0;//USER_PREF_V_START_P >> 8;//0;//USER_PREF_V_START_P >> 8;
            u8RetValueL = (USER_PREF_V_START(rxInputPort));
        #else
            u8ValueH = 0;
            u8ValueL = 100;
            u8RetValueH = 0;
            u8RetValueL = 0;
        #endif
            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_Clock && u8PageValue == 0)
        {
            u8ValueH = 0; //(MAX_CLOCK-MIN_CLOCK)>>8;
            u8ValueL =100;// 200 // (MAX_CLOCK-MIN_CLOCK) & 0x00FF;
            u8RetValueH =0;// (USER_PREF_H_TOTAL-(g_u16TempStandarHtotal-100))>> 8;
            #if ENABLE_VGA_INPUT
            u8RetValueL =GetClockValue();
            #else
            u8RetValueL =50;
            #endif
            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_Phase && u8PageValue == 0)
        {
            u8ValueH = 0x00;
            u8ValueL = 100;//63;
            #if ENABLE_VGA_INPUT
            u8RetValueL =UPPHASE_GET_OSD_VALUE(rxInputPort);
            #else
            u8RetValueL = 50;
            #endif
            u8RetValueH=0;
            u8TPValue = 0x00;
        }
#endif		
        else if(u8CPCode == ADJ_Language && u8PageValue == 0)
        {
            u8ValueL = 5;
            u8RetValueL = USER_PREF_LANGUAGE;
            u8TPValue = 0x00;
        }
        else if(u8CPCode ==Select_InputSource && u8PageValue == 0)
        {
            u8ValueL = 14;
#if HKC_PCToolCommand
        #if ENABLE_VGA_INPUT
            if (INPUT_IS_ANALOG1(rxInputPort))
                u8RetValueL = 0x01;    //analog #1
        #endif
        #if ENABLE_DVI_INPUT
            if (INPUT_IS_DIGITAL_DVI(rxInputPort))
                u8RetValueL = 0x03;    //DVI #1
          #if ENABLE_DVI2ND_INPUT
            if (INPUT_IS_DIGITAL_DVI(rxInputPort))
                u8RetValueL = 0x04;    //DVI #2
          #endif
        #endif
        #if ENABLE_HDMI_INPUT
            if (INPUT_IS_DIGITAL_HDMI(rxInputPort))
                u8RetValueL = 0x05;    //HDMI  #1
          #if ENABLE_HDMI2ND_INPUT
            if (INPUT_IS_DIGITAL_HDMI2ND(rxInputPort))
                u8RetValueL = 0x06;    //HDMI2 #2
          #endif
        #endif
        #if ENABLE_DP_INPUT
            if (INPUT_IS_DIGITAL_DP(rxInputPort))
                u8RetValueL = 0x07;    //DisplayPort #1
          #if ENABLE_DP2ND_INPUT
            if (INPUT_IS_DIGITAL_DP2ND(rxInputPort))
                u8RetValueL = 0x08;    //DisplayPort #2
          #endif
        #endif


#else			
        #if ENABLE_VGA_INPUT
            if (INPUT_IS_ANALOG1(rxInputPort))
                u8RetValueL = 0x01;    //analog #1
        #endif
        #if ENABLE_DVI_INPUT
            if (INPUT_IS_DIGITAL_DVI(rxInputPort))
                u8RetValueL = 0x03;    //DVI #1
          #if ENABLE_DVI2ND_INPUT
            if (INPUT_IS_DIGITAL_DVI(rxInputPort))
                u8RetValueL = 0x04;    //DVI #2
          #endif
        #endif
        #if ENABLE_HDMI_INPUT
            if (INPUT_IS_DIGITAL_HDMI(rxInputPort))
                u8RetValueL = 0x11;    //HDMI  #1
          #if ENABLE_HDMI2ND_INPUT
            if (INPUT_IS_DIGITAL_HDMI2ND(rxInputPort))
                u8RetValueL = 0x12;    //HDMI2 #2
          #endif
        #endif
        #if ENABLE_DP_INPUT
            if (INPUT_IS_DIGITAL_DP(rxInputPort))
                u8RetValueL = 0x0F;    //DisplayPort #1
          #if ENABLE_DP2ND_INPUT
            if (INPUT_IS_DIGITAL_DP2ND(rxInputPort))
                u8RetValueL = 0x10;    //DisplayPort #2
          #endif
        #endif
#endif		
            u8TPValue = 0x00;
        }
#if 0        //Do not allow to adjust OSD position for long bar OSD
        else if(u8CPCode == ADJ_OSD_HorizontalPos && u8PageValue == 2)
        {
            u8ValueL = 100;
            u8RetValueL = USER_PREF_OSD_H_START;
            u8TPValue = 0x00;// Jeff add it in 20064る12らと 11:44:32
        }
        else if(u8CPCode == ADJ_OSD_VerticalPos && u8PageValue == 2)
        {
            u8ValueL = 100;
            u8RetValueL = USER_PREF_OSD_V_START;
            u8TPValue = 0x00;// Jeff add it in 20064る12らと 11:44:32
        }
#endif
        else if(u8CPCode == ADJ_Red_Blacklevel && u8PageValue == 0)
        {
            u8ValueH = 0x00;
            u8ValueL = 255; //100
            u8RetValueL = CURRENT_RED_BLACK_LEVEL;
            u8TPValue=0;
        }
        else if(u8CPCode == ADJ_Green_Blacklevel && u8PageValue == 0)
        {
            u8ValueH = 0x00;
            u8ValueL = 255; //100
            u8RetValueL = CURRENT_GREEN_BLACK_LEVEL;
            u8TPValue=0;
        }
        else if(u8CPCode == ADJ_Blue_Blacklevel && u8PageValue == 0)
        {
            u8ValueH = 0x00;
            u8ValueL = 255; //100
            u8RetValueL = CURRENT_BLUE_BLACK_LEVEL;
            u8TPValue=0;
        }
        else if(u8CPCode == SyncType)
        {
            u8ValueL = 3;
            u8RetValueL = 0;
        }
        else if(u8CPCode == RGBOrdering)
        {
            u8ValueL = 2;
            u8RetValueL = 1;
        }
        else if(u8CPCode == MonitorType)
        {
            u8ValueL = 5;
            u8RetValueL = 3;
        }
        else if(u8CPCode == HoursRunning)
        {
            u8ValueH = 0xFF;
            u8ValueL = 0xFF;
        }
        else if(u8CPCode == DisplayTime)
        {
            u8ValueH = 0xFF;
            u8ValueL = 0xFF;
        }
        else if(u8CPCode == ADJ_ControlLock)
        {
            u8ValueL = 1;
            u8RetValueL = 0;
            u8TPValue = 0x00;
        }
        else if(u8CPCode == FlatPanellayout)
        {
            u8ValueL = 8;
            u8RetValueL = 1;
        }
#if ENABLE_OSD_ROTATION
        else if(u8CPCode==ScreenOrient && u8PageValue == 0)
        {
            u8ValueL = 0xFF;
            u8ValueH = 0x00;
            u8RetValueH = 0x00;
            if (USER_PREF_OSD_ROTATION!=Osd_Rotation_Normal)
                u8RetValueL= 0x02;//Screen portrait (rotate clockwise 90'degree)
            else
                u8RetValueL= 0x01;//Screen landscape
        }
#endif
        else if(u8CPCode == H_Frequency)
        {
            DWordValue =(DWORD)HFREQ(SrcHPeriod(rxInputPort))*100;//HFREQ(g_u16tempHperiod)/10;// HFREQ(SRC_H_PERIOD)/10;
            u8ValueH = 0x00;
            u8ValueL = DWordValue>>16;
            u8RetValueH=DWordValue>>8;
            u8RetValueL=DWordValue & 0x00FF;
        }
        else if(u8CPCode == V_Frequency)
        {
            u16Value = HFREQ(SrcHPeriod(rxInputPort)) * 10;
            u16Value = VFREQ( u16Value, SrcVTotal(rxInputPort));
            //u16Value = VFREQ( u16Value, SrcVTotal )*10 ;//VFREQ(HFREQ(g_u16SavehPeriod), SRC_V_TOTAL)/10;
            u8ValueH = 0x00;
            u8ValueL = 0x00;
            u8RetValueH=u16Value>>8;
            u8RetValueL=u16Value & 0x00FF;
        }
        else if (u8CPCode== DisplayControlType)
        {
            u8ValueH = 0x00;//FF;
            u8ValueL = 0x00;//FF;
            u8RetValueH=0x56;
            u8RetValueL=5; // Mean Mstar
        }
        else if (u8CPCode == FWLevel)
        {
            u8ValueH = 0xFF;
            u8ValueL = 0xFF;
            u8RetValueH=GetMajorVer();
            u8RetValueL=GetMinorVer();
        }
        else if (u8CPCode == OSDEnable)
        {
            u8ValueH = 0x00;
            u8ValueL = 0x02;
            if(OSD_LOCK_FLAG)
                u8RetValueL=1;  //F/W Version
            else
                u8RetValueL=2;  //F/W Version
        }
        else if(u8CPCode==DisplayUsageTime)
        {
            u8ValueH = 0xFF;
            u8ValueL = 0xFF;
            u8RetValueH = (DWORD)USER_PREF_BACKLIGHT_TIME/3600/256;
            u8RetValueL = (DWORD)USER_PREF_BACKLIGHT_TIME/3600%256;
        }
        else if(u8CPCode == APEnableKey)
        {
            u8ValueL = 0xFF;      //
            u8ValueH = 0xFF;
            #if defined(_DISABLE_AM_SUPPORT_)
            u8RetValueH = 0xFF;
            u8RetValueL = 0xFF;
            #else
            u8RetValueH = 0x45;
            u8RetValueL = 0xCC;
            #endif
        }
        else if(u8CPCode == PowerMode)
        {
            if ((!POWER_ON_FLAG))
                u8RetValueL = PowerMode_SwitchOff;
            else if (PowerSavingFlag)
                u8RetValueL = PowerMode_Off;
            else
                u8RetValueL = PowerMode_On;
        }
        else if(u8CPCode == VCPVersion)
        {
        #ifdef _MCCS_VER3_
            u8RetValueH = 0x03;
            u8RetValueL = 0x00;
        #else
            u8RetValueH = 0x02;
            u8RetValueL = 0x01;
        #endif
        }
        else if(u8CPCode == ManufacturerID)
        {
            u8ValueH = 0xFF;
            u8ValueL = 0xFF;
            u8RetValueH=0;
        #if (ODM_NAME==ODM_INL)  //Foxconn
            u8RetValueL=0x66;
        #elif (ODM_NAME==ODM_WISTRON)  // Wistron
            u8RetValueL=0x74;
        #elif (ODM_NAME==ODM_AOC)  // AOC
            u8RetValueL=0x63;
        #elif (ODM_NAME==ODM_BENQ)  // Qisda
            u8RetValueL=0x62;
            u8RetValueH=0x00;
            u8TPValue=0x00;
        #endif
        }
	#if HKC_PCToolCommand
	else if ( u8CPCode == HKC_COMMUNICATION_COMMAND_CHECK)
	{
	 u16TempValue=DdcciCheck(u16Value);
	  /////////////校验方式计算
	 //u32TempValue = ((((u16Value*3+0x66)^0x9FF^0x88)-0xAA)&0xFFFF);
	  ////////////
        u8RetValueH= u16TempValue >> 8;
        u8RetValueL= u16TempValue & 0x00FF;
        
	//DrawNumUnsigned ( 8, 4,  4,u16Value);
	//DrawNumUnsigned ( 8, 5, 4 ,u16TempValue);
        u8ValueL = 100;
        u8TPValue = 0x00;
	}
	else if ( u8CPCode == ADJ_ECOmode)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
		switch (CURRENT_PICTURE_MODE)
            {
                case CT_STANDARD:
                    u8RetValueL =0x01 ;
                break;
                case CT_VIVID:
                    u8RetValueL =0x04;  
                break;
                case CT_MOVIE:
                    u8RetValueL =0x02;  
                break;
		#if 1//Enable_ECOGameMode
                case CT_FPS://FPS
                    u8RetValueL =0x06;  
                break;
                case CT_RTS://RTS
                    u8RetValueL =0x05;  
                break;
                 case CT_GAME:
                     u8RetValueL =0x03;  
                break;
		#endif
                case CT_USER:
                    u8RetValueL =0x07;  
                break;
            }
        }
	else if ( u8CPCode == ADJ_LowBluRay)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
		switch (USERPREFLOWBLURAY)
            {
                case 0:
                    u8RetValueL =0x01 ;
                break;
                case 1:
                    u8RetValueL =0x02;  
                break;
                case 2:
                    u8RetValueL =0x03;  
                break;
                case 3:
                    u8RetValueL =0x04;  
                break;
                case 4:
                    u8RetValueL =0x05;  
                break;
            }
        }	
	else if ( u8CPCode == ADJ_AspectRatiomode)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
		switch (USER_PREF_WIDE_MODE(SrcInputPortM))
            {
                case eASPECT_FULL:
                    u8RetValueL =0x04 ;
                break;
                case eASPECT_4_3:
                    u8RetValueL =0x06;  
                break;
                case eASPECT_NATIVE:
                    u8RetValueL =0x02;  
                break;
            }
        }
	else if ( u8CPCode == ADJ_Gammamode)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	#if 	Enable_Gamma
	if (  CURRENT_GAMMA == eGAMMA_20)
		u8RetValueL = 0x02;
	else if ( CURRENT_GAMMA == eGAMMA_22 )
		u8RetValueL = 0x04;
	else if ( CURRENT_GAMMA == eGAMMA_24 )
		u8RetValueL = 0x06;
	#endif
        }
	else if ( u8CPCode == ADJ_PIP_PBP_Mode)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
	{
	   u8RetValueL = 0x07;
	/*
           if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP)  
           {
               if(USER_PREF_PIP_POSITION==PIPPosition_Top_Left)
           }
	    else if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_POP1)
	    {

	    }
	    else
	    {

	    }
	   */ 
	}
	else
	u8RetValueL = 0x01;
	#endif
        }
	 else if ( u8CPCode == ADJ_PIP_Win_Size)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	     u8RetValueL = 0x00;
        }
	
	 else if ( u8CPCode == DCRMode)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	#if ENABLE_DCR
	if (DCR_ENABLE_FLAG )
		u8RetValueL = 0x01;
	else
	#endif
		u8RetValueL = 0x00;
        }
	else if ( u8CPCode == ADJ_AUDIO_VOLUME)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	#if AudioFunc
	    u8RetValueL = USER_PREF_VOLUME;
	#else
	    u8RetValueL = 50;
	#endif
        }
	else if ( u8CPCode == ADJ_AUDIO_MUTE)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	#if AudioFunc
	    u8RetValueL = ((AUDIO_MUTE_FLAG)?0:1);
	#else
	    u8RetValueL = 0;
	#endif
        }
  #if Enable_Corss_LHCX	
	else if ( u8CPCode == ADJ_GameCross_Mode)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
         if(UserPrefCrossType==Cross_Mode1_G)
	    u8RetValueL = 1;
         else if(UserPrefCrossType==Cross_Mode1_R)
	    u8RetValueL = 2;
         else if(UserPrefCrossType==Cross_Mode2_G)
	    u8RetValueL = 4;
         else if(UserPrefCrossType==Cross_Mode2_R)
	    u8RetValueL = 5;

        }
   #endif	
	else if ( u8CPCode == ADJ_OverDrive)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
		u8RetValueL = 0x00;	
        }
	else if ( u8CPCode == ADJ_LedMode)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
		if(USER_PREF_LEDMODE==OffLedMode)
		{
			u8RetValueL = OffLedMode;
		}
		else if(USER_PREF_LEDMODE==GreenLedMode)
		{
			u8RetValueL = GreenLedMode;
		}
		else
		{
			u8RetValueL = RedLedMode;
		}
        }
	#if LED_HOT_Menu_Func
            else if (u8CPCode == ADJ_LedColor)
            {
			u8ValueH = 0xFF; 
			u8ValueL = 0xFF;
			u8RetValueH = 0x00; 
			u8RetValueL =USER_PREF_LEDColorful_MODE ;
            }

            else if (u8CPCode == ADJ_LedLedStrength)
            {
			u8ValueH = 0xFF; 
			u8ValueL = 0xFF;
			u8RetValueH = 0x00; 
			u8RetValueL =USER_PREF_LedStrength&0x0F;
            }

            else if (u8CPCode == ADJ_LedModeType)
            {
			u8ValueH = 0xFF; 
			u8ValueL = 0xFF;
			u8RetValueH = 0x00; 
			u8RetValueL =USER_PREF_LedModeType ;
            }
            else if (u8CPCode == ADJ_LedOffType)
            {
			u8ValueH = 0xFF; 
			u8ValueL = 0xFF;
			u8RetValueH = 0x00; 

		  if(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)
		  {
		       u8RetValueL =0;
		  }
		  else
		  {
		       u8RetValueL =1;
		   }

            }
		#endif

	else if ( u8CPCode == ADJ_StandbyTime)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	    u8RetValueL = 0x10;
        }
	else if ( u8CPCode == ADJ_SubBrightness)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	#if HKC_SETSUBWINDOW
	    u8RetValueL = UserPrefAccSetupWinBri;
	#endif
        }
	else if ( u8CPCode == ADJ_SubContrast)
        {
            u8ValueH = 0xFF; 
            u8ValueL = 0xFF;
            u8RetValueH = 0x00; 
	#if HKC_SETSUBWINDOW
	    u8RetValueL = UserPrefAccSetupWinCon;
	#endif
        }
    else if ( u8CPCode == GetTimingHDisplay)
    	{
       	 //WORD xdata HDisplay = SC0_READ_AUTO_WIDTH();
	   if(INPUT_IS_VGA(rxInputPort))
		{
		u16Value=StandardModeWidth(rxInputPort);
           	u8RetValueH = u16Value >> 8;
            	u8RetValueL = u16Value & 0x00ff;
		}
	else if(INPUT_IS_TMDS(rxInputPort)||INPUT_IS_DISPLAYPORT(rxInputPort))
		{
		u16Value=msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN1)));//mStar_ReadWord(SC0_0C);
           	u8RetValueH = u16Value >> 8;
            	u8RetValueL = u16Value & 0x00ff;
		}
            u8ValueH = 0xFF;
            u8ValueL = 0xFF;

    	}
   else if ( u8CPCode == GetTimingVDisplay)
    	{
       		 //WORD xdata VDisplay = SC0_READ_AUTO_HEIGHT();
	if(INPUT_IS_VGA(rxInputPort))
		{
		u16Value=StandardModeHeight(rxInputPort);
           	u8RetValueH = u16Value >> 8;
            	u8RetValueL = u16Value & 0x00ff;
		}
	else if(INPUT_IS_TMDS(rxInputPort)||INPUT_IS_DISPLAYPORT(rxInputPort))
		{
		u16Value=msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(DISPLAY_WIN1)));//mStar_ReadWord(SC0_0A);
           	u8RetValueH = u16Value >> 8;
            	u8RetValueL = u16Value & 0x00ff;
		}
            u8ValueH = 0xFF;
            u8ValueL = 0xFF;
    	}
	#endif		
        else
        {
            DDCBuffer[2] = 0x01;
            u8RetValueL = 0x00;
            u8ValueH = 0xFF;
            u8ValueL = 0xFF;
        }
        //End GETVCPFeature or GETPagedVCP--------------------------------------------------
        if(u8CommandCode == GETVCPFeature)
        {
            DDCBuffer[4] = u8TPValue;
            DDCBuffer[5] = u8ValueH;
            DDCBuffer[6] = u8ValueL;
            DDCBuffer[7] = u8RetValueH;
            DDCBuffer[8] = u8RetValueL;
            return 8;
        }
        else
        {
            DDCBuffer[5] = u8TPValue;
            DDCBuffer[6] = u8ValueH;
            DDCBuffer[7] = u8ValueL;
            DDCBuffer[8] = u8RetValueH;
            DDCBuffer[9] = u8RetValueL;
            return 9;
        }
    }
    //--------------------------------------------------
    //--------------------------------------------------
    else if(u8CommandCode == SETVCPFeature || u8CommandCode == SETPagedVCP)
    {
        DDCBuffer[2] = 0x00; // Result Code
        u8RetValueL = 0xFF;
        g_u8DDCCISaveCounter=1;//SET_DDCCI_SAVE_FLAG();
        //ResetMsCounter();   //for Vista issue, Let main loop not to save data till all test command stop.
        //**********************************************
        #if 1
        if( (u8CPCode!=NewControl)&&(u8CPCode!=0x7B))// && g_u8MenuPageIndex >= MENU_MAIN)
        {
            #if LOGO_ENABLE&&Free_Sync_Enable			
            #if (!ZUI)
            	if((!g_stMenuFlag.bDisplayLogo)&&(g_u8MenuPageIndex<MENU_ROOT))
            #else
            	if(!g_stZuiMenuFlag.bDisplayLogo)
            #endif
            #endif
            	{
            if (g_u16OsdCounter)
                g_u16OsdCounter=0;
                g_stMenuFlag.bOsdTimeout=1;
		}
        }
        #endif
        //**********************************************

        //g_u8DDCCIAControl=u8CPCode;
        if(u8CPCode == ALL_Reset &&u8PageValue == 0)
        {
            g_u8DDCCISaveCounter=0;//CLR_DDCCI_SAVE_FLAG();//main loop needn't  save
            if(u16Value != 0)
            {
                g_u8MenuTempValue=1;
		  Menu_Hide();		
                ResetAllSetting();
                if (!PowerSavingFlag)
                    SET_DDCCI_AUTO_ADJUST_FLAG();
            }
            u8ValueL = 1;
        }
        else if(u8CPCode == RestoryFactory&& u8PageValue == 0)
        {
            if(u16Value != 0)
            {
            if(CURRENT_PICTURE_MODE==CT_USER)
            	{
                USER_PREF_BRIGHTNESS_USER=DEF_BRIGHTNESS;
                CURRENT_CONTRAST_USER=DEF_CONTRAST;
               msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS_USER);
                msAPI_AdjustContrast(USER_PREF_WIN_SEL , GET_USER_PREF_CONTRAST_USER(USER_PREF_WIN_SEL));
            	}
		else
		{
                USER_PREF_BRIGHTNESS=DEF_BRIGHTNESS;
                CURRENT_CONTRAST=DEF_CONTRAST;
                msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
                msAPI_AdjustContrast(USER_PREF_WIN_SEL , GET_USER_PREF_CONTRAST(USER_PREF_WIN_SEL));
		}
                //SaveMonitorSetting();
            }
        }
        else if(u8CPCode == Geometry_Reset&& u8PageValue == 0)
        {
        #if ENABLE_VGA_INPUT
            g_u8DDCCISaveCounter=0;//CLR_DDCCI_SAVE_FLAG()  ;//main loop needn't  save
            if(u16Value != 0 && INPUT_IS_VGA(rxInputPort))
            {
                SET_DDCCI_AUTO_GEOMTRY_FLAG();
            }
        #endif
        }
        else if(u8CPCode==ColorTempRequest&& u8PageValue == 0)
        {
            u16TempValue=u8ValueL;
            u16TempValue=4000+u16TempValue*1500;
            if(u16TempValue<7100)
            {
                CURRENT_COLOR_TEMP=PCT_6500K;
                SetColorTempDDCCI();
            }
            else
            {
                CURRENT_COLOR_TEMP=PCT_9300K;
                SetColorTempDDCCI();
            }
            u8RetValueL = 0xFF;
        }
        else if(u8CPCode == Color_Reset&& u8PageValue == 0)
        {
            if(u16Value != 0)
            {
                ResetColorSettingAll();
            }
        //SaveMonitorSetting();
        }
        else if(u8CPCode == ADJ_Brightness&& u8PageValue == 0)
        {
        #if ENABLE_DCR
            if (!DCR_ENABLE_FLAG)
        #endif
            {
                if (u8ValueL>100)       //while USER_PREF_BRIGHTNESS=0 and run SoftMCCS Compliance Test
                    u8ValueL=100;       //,u8ValueL=USER_PREF_BRIGHTNESS-1=0xFF
                USER_PREF_BRIGHTNESS = u8ValueL;
		  UserPrefBrightness_Temp=USER_PREF_BRIGHTNESS;	
                msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
            }
            u8TPValue=0;
        }
        else if(u8CPCode == ADJ_Contrast&& u8PageValue == 0)
        {
            BYTE u8TempContrast = GET_USER_PREF_CONTRAST(USER_PREF_WIN_SEL);

            if(u8ValueL < 25)
            {
                u8TempContrast = u8ValueL;
                //msAPI_AdjustContrast(DISPLAY_WIN1 , CURRENT_CONTRAST);
            }
            else if(u8ValueL <= (MAX_CONTRAST_VALUE-MIN_CONTRAST_VALUE))
            {
                u8TempContrast = u8ValueL + MIN_CONTRAST_VALUE;
                //msAPI_AdjustContrast(DISPLAY_WIN1 , CURRENT_CONTRAST);
            }
            CURRENT_CONTRAST=u8TempContrast;
            UserPrefContrast_Temp=CURRENT_CONTRAST;
        #if 0//ENABLE_ACE
            if (CURRENT_COLOR_TEMP > PCT_6500K)
                msAdjustVideoContrast(SUB_WINDOW,GetSubWndRealContrast(CURRENT_CONTRAST));
            else
        #endif
                msAPI_AdjustContrast(USER_PREF_WIN_SEL , GET_USER_PREF_CONTRAST(USER_PREF_WIN_SEL));

            u8RetValueL = (MAX_CONTRAST_VALUE-MIN_CONTRAST_VALUE);
            u8TPValue=0;
        }
        else if(u8CPCode == Select_Color_Preset && u8PageValue == 0)
        {
            if(u8ValueL == 0x0C)
            {
                CURRENT_COLOR_TEMP=PCT_USER;
                SetColorTempDDCCI();
            }
            else if(u8ValueL == 0x0B)
            {
                CURRENT_COLOR_TEMP=PCT_5700K;
                SetColorTempDDCCI();
            }
            else if(u8ValueL == 0x05)
            {
                CURRENT_COLOR_TEMP=PCT_6500K;
                SetColorTempDDCCI();
            }
            else if(u8ValueL == 0x08)
            {
                CURRENT_COLOR_TEMP=PCT_9300K;
                SetColorTempDDCCI();
            }

            u8RetValueL = 0xFF;
            u8TPValue=0;
        }
        else if(u8CPCode == DisplayApplication && u8PageValue == 0)
        {
            if(u8ValueL == 0x00)
            {
                CURRENT_PICTURE_MODE=CT_STANDARD;
                APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
                Menu_AdjustColorSetting();
            }
            else if(u8ValueL == 0x03)
            {
                CURRENT_PICTURE_MODE=CT_MOVIE;
                APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
                Menu_AdjustColorSetting();
            }
            else if(u8ValueL == 0x04)
            {
                CURRENT_PICTURE_MODE=CT_USER;
                APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
                Menu_AdjustColorSetting();
            }
            else if(u8ValueL==0x05)
            {
                CURRENT_PICTURE_MODE=CT_RTS;
                APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
                Menu_AdjustColorSetting();
            }
            else if(u8ValueL == 0x06)
            {
                CURRENT_PICTURE_MODE=CT_FPS;
                APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
                Menu_AdjustColorSetting();
            }
            else if(u8ValueL == 0x09)
            {
                CURRENT_PICTURE_MODE=CT_VIVID;
                APPWindow_SetColorSpace(USER_PREF_WIN_SEL);
                Menu_AdjustColorSetting();
            }
            //SetColorTempDDCCI();
            u8RetValueL = 0xFF;
            u8TPValue=0;
        }
        else if(u8CPCode == ADJ_Red_Gain && u8PageValue == 0)
        {
            if(CURRENT_COLOR_TEMP!=PCT_USER)
            {
                force_update_user_color();
                SetColorTempDDCCI();
                g_u8DDCCIAControl = 0x14;
            }
            if (CURRENT_COLOR_TEMP == PCT_USER)
            {

                if(u8ValueL<30)
                {
                    CURRENT_RED_COLOR = (WORD)(30*MAX_USER_R_COLOR_VALUE)/100;
                }
                else
                {
                    CURRENT_RED_COLOR = (WORD)(u8ValueL*MAX_USER_R_COLOR_VALUE)/100;//*100)/255;
                }
                msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
            }
            u8RetValueL =100;
            u8TPValue=0;
        }
        else if(u8CPCode == ADJ_Green_Gain && u8PageValue == 0)
        {
            if(CURRENT_COLOR_TEMP!=PCT_USER)
            {
                force_update_user_color();
                SetColorTempDDCCI();
                g_u8DDCCIAControl = 0x14;
            }
            if (CURRENT_COLOR_TEMP == PCT_USER)
            {
                if(u8ValueL<30)
                {
                    CURRENT_GREEN_COLOR = (WORD)(30*MAX_USER_G_COLOR_VALUE)/100;
                }
                else
                {
                    CURRENT_GREEN_COLOR = (WORD)(u8ValueL*MAX_USER_G_COLOR_VALUE)/100;//*100)/255;
                }
                msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
            }
            u8RetValueL =100;
            u8TPValue=0;
        }
        else if(u8CPCode == ADJ_Blue_Gain && u8PageValue == 0)
        {
            if(CURRENT_COLOR_TEMP!=PCT_USER)
            {
                force_update_user_color();
                SetColorTempDDCCI();
                g_u8DDCCIAControl = 0x14;
            }
            if(CURRENT_COLOR_TEMP == PCT_USER)
            {

                if(u8ValueL<30)
                {
                    CURRENT_BLUE_COLOR = (WORD)(30*MAX_USER_B_COLOR_VALUE)/100;
                }
                else
                {
                    CURRENT_BLUE_COLOR = (WORD)(u8ValueL*MAX_USER_B_COLOR_VALUE)/100;//*100)/255;
                }

                msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
            }
            u8RetValueL =100;
            u8TPValue=0;
        }
        else if((u8CPCode == ADJ_Red_Blacklevel||u8CPCode==ADJ_Green_Blacklevel||u8CPCode==ADJ_Blue_Blacklevel)&& u8PageValue == 0)
        {
            if(u8CPCode == ADJ_Red_Blacklevel)
                CURRENT_RED_BLACK_LEVEL= u8ValueL;
            else  if(u8CPCode == ADJ_Green_Blacklevel)
                CURRENT_GREEN_BLACK_LEVEL= u8ValueL;
            else  if(u8CPCode == ADJ_Blue_Blacklevel)
                CURRENT_BLUE_BLACK_LEVEL= u8ValueL;
            msAPI_AdjustBlacklevel(USER_PREF_WIN_SEL , CURRENT_RED_BLACK_LEVEL,CURRENT_GREEN_BLACK_LEVEL,CURRENT_BLUE_BLACK_LEVEL);
            //SaveMonitorSetting();
        }
        else if(u8CPCode == ADJ_HorizontalPos && u8PageValue == 0)
        {
        #if ENABLE_VGA_INPUT
            if(INPUT_IS_VGA(rxInputPort))
            {
                if ( u16Value >= MAX_H_START(rxInputPort))
                    u16Value = MAX_H_START(rxInputPort);
                if ( u16Value <= MAX_H_START(rxInputPort))
                {
                    USER_PREF_H_START(rxInputPort) = (MAX_H_START(rxInputPort) - u16Value);
                    msAPI_AdjustHPosition(USER_PREF_WIN_SEL , USER_PREF_H_START(rxInputPort));
                    //NVRam_WriteWord(NVR_MODE_ADDR(u16HStart), USER_PREF_H_START);
                }
            }
        #endif
            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_VerticalPos && u8PageValue == 0)
        {
        #if ENABLE_VGA_INPUT
            if(INPUT_IS_VGA(rxInputPort))
            {
                //if ( u16Value >=100 )
                //    u16Value = 100;
                //if (u16Value<=0)
                //    u16Value=0;
                USER_PREF_V_START(rxInputPort) = u16Value;//( ( WORD ) u16Value * ( MAX_V_START) ) / 100;//u16Value;
                msAPI_AdjustVPosition( USER_PREF_WIN_SEL , USER_PREF_V_START(rxInputPort));
                //NVRam_WriteWord(NVR_MODE_ADDR(u16VStart), USER_PREF_V_START_P);
                //SaveModeSetting();
            }
        #endif
            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_Clock&& u8PageValue == 0)
        {
        #if ENABLE_VGA_INPUT
            if(INPUT_IS_VGA(rxInputPort))
            {   WORD u16TempValue1;
                u16TempValue1=u16Value;
                USER_PREF_H_TOTAL(rxInputPort) =((u16TempValue1*200+50)/0x64+StandardModeHTotal(rxInputPort)-100);
                //USER_PREF_H_TOTAL =((u16TempValue1*200+50)/0x64+g_u16TempStandarHtotal-100);
                drvADC_AdjustHTotal(USER_PREF_H_TOTAL(rxInputPort));
                //SaveModeSetting();
            }
        #endif
            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_Phase && u8PageValue == 0)
        {
        #if ENABLE_VGA_INPUT
            if(INPUT_IS_VGA(rxInputPort))
            {
                UPPHASE_SET_BY_OSD_VALUE(u16Value, rxInputPort);
                drvADC_SetPhaseCode(UPPHASE_GET_REAL_VALUE(rxInputPort));
                //SaveModeSetting();
            }
        #endif
            u8TPValue = 0x00;
        }
        else if(u8CPCode == ADJ_Language&& u8PageValue == 0)
        {
            USER_PREF_LANGUAGE = u8ValueL%LANG_INDEX_MAX;
            u8TPValue = 0x00;
        }
        else if(u8CPCode == Auto_Setup&& u8PageValue == 0)
        {
            if(u16Value != 0 && INPUT_IS_VGA(rxInputPort))
            //AutoConfig();
                SET_DDCCI_AUTO_ADJUST_FLAG();
        }
#if 0        //Do not allow to adjust OSD position for long bar OSD
        else if(u8CPCode == ADJ_OSD_HorizontalPos && u8PageValue == 2)
        {
            USER_PREF_OSD_H_START = u8ValueL;
            Osd_SetPosition(USER_PREF_OSD_H_START,USER_PREF_OSD_V_START);
            u8TPValue = 0x00;// Jeff add it in 20064る12らと 11:44:32
        }
        else if(u8CPCode == ADJ_OSD_VerticalPos && u8PageValue == 2)
        {
            USER_PREF_OSD_V_START = u8ValueL;
            Osd_SetPosition(USER_PREF_OSD_H_START,USER_PREF_OSD_V_START);
            u8TPValue = 0x00;// Jeff add it in 20064る12らと 11:44:32
        }
#endif
        else if(u8CPCode == OSDEnable&& u8PageValue == 0)
        {
            if(u8ValueL==1)
                SET_OSD_LOCK_FLAG();
            if(u8ValueL==2)
                CLR_OSD_LOCK_FLAG();
            u8TPValue = 0x00;
        }
        else if(u8CPCode == NewControl && u8PageValue == 0)
        {
            if(u8ValueL==01 && u8ButtonFIFOIndex)
            {
                //CLR_NEW_CONTROL_FLAG();
                for (i=0;i<FIFO_SIZE-1;i++)
                    u8ButtonFIFO[i]=u8ButtonFIFO[i+1];

                u8ButtonFIFO[FIFO_SIZE-1]=0;
                u8ButtonFIFOIndex--;
                g_u8DDCCIAControl=0x00;
            }
        }
        else if(u8CPCode == Contrast_Auto && u8PageValue == 2)
        {
        #if ENABLE_VGA_INPUT
            g_u8DDCCISaveCounter=0;//CLR_DDCCI_SAVE_FLAG()  ;//main loop needn't  save
            if(u16Value != 0 && CURRENT_INPUT_IS_VGA())
                AutoColor();
            u8RetValueL =1;
        #endif
            u8TPValue=0;
        }
        else if(u8CPCode == TEST_SaveCurrentSetting && u8PageValue == 0)
        {
            g_u8DDCCISaveCounter=0;//CLR_DDCCI_SAVE_FLAG()  ;//main loop needn't  save
            if(u16Value != 0)
            {
                SaveUserPref();
            }
        }
        else if(u8CPCode == PowerMode)
        {
            /*if(u8ValueL == PowerMode_On)
            {   if (!POWER_ON_FLAG)
                    ExecuteKeyEvent(MIA_POWER);
            }
            else if (u8ValueL>=PowerMode_Standby && u8ValueL>=PowerMode_Off)
            {   if (POWER_ON_FLAG)
                ExecuteKeyEvent(MIA_POWER);
            }
            */
            g_u8DDCCISaveCounter=0;//CLR_DDCCI_SAVE_FLAG()  ;//main loop needn't  save
            if (u8ValueL == PowerMode_SwitchOff)
                //ExecuteKeyEvent(MIA_POWER);
				PowerOffSystem();
            else
            {
                if (POWER_ON_FLAG)
                {
                    if (u8ValueL>=PowerMode_Standby && u8ValueL<=PowerMode_Off)
                    {
                        if (!PowerSavingFlag)
                        {
                            Set_ForcePowerSavingFlag();
                            SET_DDCCI_FORCE_POWER_SAVE_FLAG(); //for DVI input,reduced power off,it wake up immediately.
                        }
                    }
                    else if (PowerSavingFlag)
                    {
                        Set_InputTimingChangeFlag(rxInputPort);
                    }
                }
            #if 1
                else if (u8ValueL==PowerMode_On) //power on if POWER_MODE_ON only
                    ExecuteKeyEvent(MIA_POWER);
            #endif
            }
        }
        else if(u8CPCode ==Select_InputSource)
        {
            g_u8DDCCISaveCounter=0;//CLR_DDCCI_SAVE_FLAG()  ;//main loop needn't  save
#if HKC_PCToolCommand
        #if ENABLE_VGA_INPUT
            if (u8ValueL==0x01)
            {
              if(USER_PREF_INPUT_TYPE!=INPUT_VGA)
                SwitchPort(USER_PREF_WIN_SEL, INPUT_VGA);
            }
        #endif
        #if ENABLE_DVI_INPUT
            if (u8ValueL==0x03)
            {
              if(USER_PREF_INPUT_TYPE!=INPUT_DVI)
                SwitchPort(USER_PREF_WIN_SEL, INPUT_DVI);
            }
          #if ENABLE_DVI2ND_INPUT
            if (u8ValueL==0x04)
            {
              if(USER_PREF_INPUT_TYPE!=INPUT_DVI2ND)
                SwitchPort(USER_PREF_WIN_SEL, INPUT_DVI2ND);
            }
          #endif
        #endif
        #if ENABLE_HDMI_INPUT
            if (u8ValueL==0x05)
            {
              if(USER_PREF_INPUT_TYPE!=INPUT_HDMI)
                SwitchPort(USER_PREF_WIN_SEL, INPUT_HDMI);
            }
          #if ENABLE_HDMI2ND_INPUT
            if (u8ValueL==0x06)
            {
              if(USER_PREF_INPUT_TYPE!=INPUT_HDMI2ND)
                SwitchPort(USER_PREF_WIN_SEL, INPUT_HDMI2ND);
            }
          #endif
        #endif
        #if ENABLE_DP_INPUT
            if (u8ValueL==0x07)
            {
              if(USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT)
                SwitchPort(USER_PREF_WIN_SEL, INPUT_DISPLAYPORT);
            }
          #if ENABLE_DP2ND_INPUT
            if (u8ValueL==0x08)
            {
              if(USER_PREF_INPUT_TYPE!=INPUT_DISPLAYPORT2ND)
                SwitchPort(USER_PREF_WIN_SEL, INPUT_DISPLAYPORT2ND);
            }
          #endif
        #endif


#else
        #if ENABLE_VGA_INPUT
            if (u8ValueL==0x01)
            {
                msAPIWinSwitchPort(USER_PREF_WIN_SEL, INPUT_VGA);
            }
        #endif
        #if ENABLE_DVI_INPUT
            if (u8ValueL==0x03)
            {
                msAPIWinSwitchPort(USER_PREF_WIN_SEL, INPUT_DVI);
            }
          #if ENABLE_DVI2ND_INPUT
            if (u8ValueL==0x04)
            {
                msAPIWinSwitchPort(USER_PREF_WIN_SEL, INPUT_DVI2ND);
            }
          #endif
        #endif
        #if ENABLE_HDMI_INPUT
            if (u8ValueL==0x11)
            {
                msAPIWinSwitchPort(USER_PREF_WIN_SEL, INPUT_HDMI);
            }
          #if ENABLE_HDMI2ND_INPUT
            if (u8ValueL==0x12)
            {
                msAPIWinSwitchPort(USER_PREF_WIN_SEL, INPUT_HDMI2ND);
            }
          #endif
        #endif
        #if ENABLE_DP_INPUT
            if (u8ValueL==0x0F)
            {
                msAPIWinSwitchPort(USER_PREF_WIN_SEL, INPUT_DISPLAYPORT);
            }
          #if ENABLE_DP2ND_INPUT
            if (u8ValueL==0x10)
            {
                msAPIWinSwitchPort(USER_PREF_WIN_SEL, INPUT_DISPLAYPORT2ND);
            }
          #endif
        #endif
            Set_InputTimingChangeFlag(rxInputPort);
  #endif		
        }
#if HKC_PCToolCommand
	else if (u8CPCode == ADJ_ECOmode && u8PageValue == 0)
	{
		u8TPValue=CURRENT_PICTURE_MODE;
           switch (u8ValueL)
            {
                case 0x01:
                    CURRENT_PICTURE_MODE = CT_STANDARD;
                break;
                
                case 0x02:
                    CURRENT_PICTURE_MODE = CT_MOVIE;
                break;

                case 0x04:
                    CURRENT_PICTURE_MODE =CT_VIVID ;
                break;
	#if 1//Enable_ECOGameMode
                case 0x06:
                    CURRENT_PICTURE_MODE = CT_FPS;//FPS
                break;
                case 0x05:
                    CURRENT_PICTURE_MODE = CT_RTS;//RTS
                break;
                 case 0x03:
                     CURRENT_PICTURE_MODE = CT_GAME;
                 break;
	#endif
                case 0x07:
                    CURRENT_PICTURE_MODE = CT_USER;
                break;
            }
            u8ValueL = 0x00;
            u8ValueH = 0x00;
           // u8TPValue = 0x00;
	if(CURRENT_PICTURE_MODE!=u8TPValue)
	{
	      Menu_AdjustColorSetting();
	}
	}
	#if 1//Enable_AdjLowbluray
	else if (u8CPCode == ADJ_LowBluRay && u8PageValue == 0)
	{
		u8TPValue=USERPREFLOWBLURAY;
           	switch (u8ValueL)
            {
                case 0x01:
                    USERPREFLOWBLURAY = 0;
                break;
                
                case 0x02:
                    USERPREFLOWBLURAY = 1;
                break;

                case 0x03:
                    USERPREFLOWBLURAY = 2;
                break;
				
                case 0x04:
                    USERPREFLOWBLURAY =3 ;
                break;
				
                case 0x05:
                    USERPREFLOWBLURAY = 4;
                break;
				
            }
            u8ValueL = 0x00;
            u8ValueH = 0x00;
		//if(USERPREFLOWBLURAY!=u8TPValue)
		{
			 AdjustLowbluray(MIA_SET_VALUE);
		}
	}
#endif
    else if (u8CPCode == ADJ_AspectRatiomode && u8PageValue == 0)
      {
			u8TPValue=USER_PREF_WIDE_MODE(SrcInputPortM);
			if ( u8ValueL == 0x04 )
				USER_PREF_WIDE_MODE(SrcInputPortM) = eASPECT_FULL;
			else if ( u8ValueL == 0x06 )
				USER_PREF_WIDE_MODE(SrcInputPortM) = eASPECT_4_3;
			else if ( u8ValueL == 0x02 )
				USER_PREF_WIDE_MODE(SrcInputPortM) = eASPECT_NATIVE;
            		u8ValueL = 0x00;
            		u8ValueH = 0x00;

				
				 if(USER_PREF_WIDE_MODE(SrcInputPortM)!=u8TPValue)	
				{
                                SetWideMode();
				 }

	}
     else if (u8CPCode == ADJ_Gammamode && u8PageValue == 0)
	{
		#if Enable_Gamma
		    u8TPValue=CURRENT_GAMMA;
			if ( u8ValueL == 0x02 )
				CURRENT_GAMMA = eGAMMA_20;
			else if ( u8ValueL == 0x04 )
				CURRENT_GAMMA = eGAMMA_22;
			else if ( u8ValueL == 0x06 )
				CURRENT_GAMMA = eGAMMA_24;
            		u8ValueL = 0x00;
            		u8ValueH = 0x00;

		   if(u8TPValue!=CURRENT_GAMMA)
		   {
		           LoadGammaFunc(USER_PREF_WIN_SEL);
		   }
		#endif
	}
     else if (u8CPCode == ADJ_PIP_PBP_Mode && u8PageValue == 0)
	{

	   #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
	         DDCCIFuncCommand=u8ValueL;
		  DDCCIFuncCounter=50;
		  Set_DDCCIActiveFlag();
	   /*
		if ( u8ValueL == 0x01 )
		{
		   USER_PREF_PIP_POSITION = PIPPosition_Bottom_Left;
             if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
	      else		 
                 ExecuteKeyEvent(MIA_HotMulti_PIP);
		}
		else if( u8ValueL == 0x02 )
		{
		   USER_PREF_PIP_POSITION = PIPPosition_Top_Left;
             if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
	      else		 
                 ExecuteKeyEvent(MIA_HotMulti_PIP);
		}
		else if( u8ValueL == 0x03 )
		{
		   USER_PREF_PIP_POSITION = PIPPosition_Top_Right;
             if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
	      else		 
                 ExecuteKeyEvent(MIA_HotMulti_PIP);
		}
		else if( u8ValueL == 0x04 )
		{
		   USER_PREF_PIP_POSITION = PIPPosition_Bottom_Right;
             if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Position);
	      else		 
                 ExecuteKeyEvent(MIA_HotMulti_PIP);
		}
		else if( u8ValueL == 0x05 )
		{
		if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_3)
                 ExecuteKeyEvent(MIA_HotMulti_PBP_3WIN);
		}
		else if( u8ValueL == 0x06 )
		{
		if(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_2_POP1)
                 ExecuteKeyEvent(MIA_HotMulti_PBP_2WIN);
		}
              else
              {
                 ExecuteKeyEvent(MIA_HotMulti_PBP_OFF);
              }
           */
	  #endif
	}
     else if (u8CPCode == ADJ_PIP_Win_Size && u8PageValue == 0)
	{
	#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
             OrigAdjPipHpos=(100-GetPIPHPositionValue());
             OrigAdjPipVpos=GetPIPVPositionValue();

		if ( u8ValueL == 0x00 )
		{
		   USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) = ePIPSIZE_SMALL;
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Size);
		}
		else if( u8ValueL == 0x01 )
		{
		   USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) = ePIPSIZE_MIDDLE;
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Size);
		}
		else if( u8ValueL == 0x02 )
		{
		   USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2) = ePIPSIZE_LARGE;
                 ExecuteKeyEvent(MIA_HotMulti_PIP_Size);
		}
        #endif
     	}	 
	else if (u8CPCode == DCRMode && u8PageValue == 0)
	{
		u8TPValue=0;//DCR_ENABLE_FLAG;
		#if ENABLE_DCR
			if ( u8ValueL == 0x01 )
				CLR_DCR_ENABLE_FLAG();
			else if ( u8ValueL == 0x00 )
				SET_DCR_ENABLE_FLAG();
	//if(u8TPValue!=DCR_ENABLE_FLAG)
		//{
		if(DCR_ENABLE_FLAG)
		{
			CLR_DCR_ENABLE_FLAG();
			Menu_AdjustColorSetting();
		}
		else
		{
			SET_DCR_ENABLE_FLAG();
                     CURRENT_PICTURE_MODE = CT_STANDARD;
                     Menu_AdjustColorSetting();        
                    #if ENABLE_DCR
                       MF_DCRCheckBrightness();
                   #else //080905
                       msAPI_AdjustBrightness(UserPrefBrightness_Temp);
                   #endif
		}
		#endif		
		SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));
	}
	else if (u8CPCode == ADJ_AUDIO_VOLUME && u8PageValue == 0)
        {
              u8TPValue=0;
       #if AudioFunc			  
              USER_PREF_VOLUME=u8ValueL;
		msAPI_AdjustVolume(USER_PREF_VOLUME);	  
	#endif	
        }
	else if (u8CPCode == ADJ_AUDIO_MUTE && u8PageValue == 0)
        {
              u8TPValue=0;
       #if AudioFunc			  
              if(u8ValueL==1)
              {
                 CLR_AUDIO_MUTE_FLAG();
              }
		else
		{
                 SET_AUDIO_MUTE_FLAG();
		}

		 SetAudioMuteOnOff();	  
	#endif	 
        }	
#if Enable_Corss_LHCX	
	else if (u8CPCode == ADJ_GameCross_Mode && u8PageValue == 0)
	{
              u8TPValue=0;

              if(u8ValueL==1)
              {
                Set_DDCCIActiveFlag();
		{
		  UserPrefCrossType=Cross_Mode1_G;	
                ExecuteKeyEvent(MIA_HOTCROSS);
		}
                Clr_DDCCIActiveFlag();
              }
		else if(u8ValueL==2)
		{
                Set_DDCCIActiveFlag();
		{
		  UserPrefCrossType=Cross_Mode1_R;	
                ExecuteKeyEvent(MIA_HOTCROSS);
		}
                Clr_DDCCIActiveFlag();
		}
		else if(u8ValueL==4)
		{
                Set_DDCCIActiveFlag();
		{
		  UserPrefCrossType=Cross_Mode2_G;	
                ExecuteKeyEvent(MIA_HOTCROSS);
		}
                Clr_DDCCIActiveFlag();
		}
		else if(u8ValueL==5)
		{
                Set_DDCCIActiveFlag();
		{
		  UserPrefCrossType=Cross_Mode2_R;	
                ExecuteKeyEvent(MIA_HOTCROSS);
		}
                Clr_DDCCIActiveFlag();
		}
		else
		{
		  g_u8MenuPageIndex = MENU_ROOT;
                ExecuteKeyEvent(MIA_REDRAW_MENU);
		}
		
	}
#endif	
	else if (u8CPCode == ADJ_OverDrive && u8PageValue == 0)
	{
   	  #if ENABLE_RTE
   		u8TPValue=0;
          if(u8ValueL==3)
          {
             USER_PREF_RESPONSE_TIME=RT_OFF;
          }
   	else 
          {
             USER_PREF_RESPONSE_TIME=RT_ON;
          }
   
   	   mStar_SetRTE();
   
   	 #endif
	}
	else if (u8CPCode == ADJ_LedMode && u8PageValue == 0)
	{
		u8TPValue=0;//CURRENT_GAMMA;
		if ( u8ValueL == OffLedMode )
		{
				hw_ClrGreenLed();
				hw_ClrAmberLed();
		}
		else if ( u8ValueL == GreenLedMode )
		{
				hw_SetGreenLed();
				hw_ClrAmberLed();
		}
		else if ( u8ValueL == RedLedMode )
		{
				hw_ClrGreenLed();
				hw_SetAmberLed();
				
		}
		if(USER_PREF_LEDMODE!=u8ValueL)
		{
			USER_PREF_LEDMODE=u8ValueL;
			SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u8SYS_LedMode), sizeof(USER_PREF_LEDMODE));
		}
            	u8ValueL = 0x00;
            	u8ValueH = 0x00;
	}
	#if LED_HOT_Menu_Func
            else if (u8CPCode == ADJ_LedColor && u8PageValue == 0)
            {
            //BYTE Retry;
			USER_PREF_LEDColorful_MODE = u8ValueL;
			if(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)
		       SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);

            }

            else if (u8CPCode == ADJ_LedLedStrength && u8PageValue == 0)
            {
            //BYTE Retry;
		USER_PREF_LedStrength=u8ValueL;
			if(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)
		       SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
		
           }

            else if (u8CPCode == ADJ_LedModeType && u8PageValue == 0)
            {
            //BYTE Retry;
		USER_PREF_LedModeType=u8ValueL ;
			if(USER_PREF_LEDMODE_INDEX!=LedModeIndex_Off)
		       SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
		
            }
            else if (u8CPCode == ADJ_LedOffType && u8PageValue == 0)
            {
            //BYTE Retry;
		if(u8ValueL)
		{
		       USER_PREF_LEDMODE_INDEX=LedModeIndex_Off;
			//CLR_LED_FUNC_ON_FLAG();
		}
		else
		{
		       USER_PREF_LEDMODE_INDEX=LedModeIndex_Breath;
			//SET_LED_FUNC_ON_FLAG();
		}

		if(!IS_LED_Power())
		 {
                   Set_LED_Power();
        
                  if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)
                     ForceDelay1ms(53);//(49);
                  else//LedModeIndex_User
                  {
           	     if((USER_PREF_LedModeType==LedBreath)	)  
                     ForceDelay1ms(53);
                  else    
                     ForceDelay1ms(55);//55//53
                  }
			//SET_LED_FUNC_ON_FLAG();
		  }

		  if(USER_PREF_LEDMODE_INDEX==LedModeIndex_Breath)
		  {
		       Set_LEDActiveOnFlag();	
		  	SetLedFunc(LED_Number, LedColorful, LedMax, LedBreath);
		  }
		  else if(USER_PREF_LEDMODE_INDEX==LedModeIndex_User)//(LED_FUNC_ON_FLAG)
		  {
		       Set_LEDActiveOnFlag();	
		       SetLedFunc(LED_Number, USER_PREF_LEDColorful_MODE, USER_PREF_LedStrength, USER_PREF_LedModeType);
		  }
		  else
		  {
		       Clr_LEDActiveOnFlag();	
		       SetLedFunc(0xFF, 0xFF, 0xFF, 0xFF);
		   }
	   
            }
	#endif
	else if (u8CPCode == ADJ_StandbyTime && u8PageValue == 0)
	{
		#if 1
		//if(u8TPValue!=UserPrefOverDriveStep)
			//{
    			u8TPValue = u8ValueL;
            		u8ValueL = 0x00;
            		u8ValueH = 0x00;
			//#if UseFlashSave
		    	//SaveMonitorSetting();
			//#else
		    	//NVRam_WriteByte(NVR_MONITOR_ADDR(OverDriveStep), UserPrefOverDriveStep);
			//#endif	
			//}
		#endif
	}
	else if (u8CPCode == ADJ_AccSetupWinEnabled && u8PageValue == 0)
	{
		//u8TPValue = u8RetValueL;
		if (!u8RetValueL)
			{
		#if 1//MWE_AP_Enable
			#if HKC_SETSUBWINDOW
			UserPrefAccSetupWinXpos=0;
			UserPrefAccSetupWinYpos=0;
			UserPrefAccSetupWinWidth=0;
			UserPrefAccSetupWinHeight=0;
			#endif
			CLR_DCR_ENABLE_FLAG();
                     CURRENT_PICTURE_MODE = CT_STANDARD;
                     Menu_AdjustColorSetting();        
		
			#if 0//UseFlashSave
		    	SaveMonitorSetting();
			#else
			#if HKC_SETSUBWINDOW
		    	NVRam_WriteByte(NVR_MONITOR_ADDR(MonitorFlag2), MonitorFlags2);
			NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinXpos), UserPrefAccSetupWinXpos);
			NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinYpos), UserPrefAccSetupWinYpos);
			NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinWidth), UserPrefAccSetupWinWidth);
			NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinHeight), UserPrefAccSetupWinHeight);
			#endif
			#endif	
		#endif
			}
			u8RetValueH = 0x00;
			u8ValueL = 0x00;
            		u8ValueH = 0x00;

	}
	else if (u8CPCode == ADJ_SubBrightness && u8PageValue == 0)
	{
		#if HKC_SETSUBWINDOW
			u8TPValue=UserPrefAccSetupWinBri;
    			UserPrefAccSetupWinBri = u8RetValueL;
				
			if(u8TPValue!=UserPrefAccSetupWinBri)
			{
			//Osd_Draw4Num ( 15, 12, UserPrefAccSetupWinBri ,0);
			mStar_AdjAccSetupWinBrightness(UserPrefAccSetupWinBri);

			#if UseFlashSave
		    	SaveMonitorSetting();
			#else
		    	NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinBri), UserPrefAccSetupWinBri);
			#endif	
			}
		#endif
		       u8ValueL = 0x00;
            		u8ValueH = 0x00;
	}
	else if (u8CPCode == ADJ_SubContrast && u8PageValue == 0)
	{
		#if HKC_SETSUBWINDOW
			u8TPValue=UserPrefAccSetupWinCon;
    			UserPrefAccSetupWinCon = u8RetValueL;

			if(u8TPValue!=UserPrefAccSetupWinCon)
			{
			//Osd_Draw4Num ( 15, 13, UserPrefAccSetupWinCon ,0);
			mStar_AdjAccSetupWintContrast(UserPrefAccSetupWinCon);

			#if UseFlashSave
		    	SaveMonitorSetting();
			#else
		    	NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinCon), UserPrefAccSetupWinCon);
			#endif	
			}
		#endif
		       u8ValueL = 0x00;
            		u8ValueH = 0x00;
	}
	else if (u8CPCode == ADJ_WindowRect && u8PageValue == 0)
	{
		WORD xdata u16WordAddr1;
		WORD xdata u16WordAddr2;

		u16TempValue = ((WORD)DDCBuffer[5] << 8) | ((WORD)DDCBuffer[6]);
		u16WordAddr1 = ((WORD)DDCBuffer[7] << 8) | ((WORD)DDCBuffer[8]);
		u16WordAddr2 = ((WORD)DDCBuffer[9] << 8) | ((WORD)DDCBuffer[10]);

		//Osd_Draw4Num ( 15, 10, u16Value ,0);
		//Osd_Draw4Num ( 15, 11, u16WordAddr ,0);
		//Osd_Draw4Num ( 15, 12, u16WordAddr1 ,0);
		//Osd_Draw4Num ( 15, 13, u16WordAddr2 ,0);
		//Osd_Draw4Num ( 15, 14, H_display ,0);
		//Osd_Draw4Num ( 15, 15, V_display ,0);
		//Osd_Draw4Num ( 15, 16, (u16WordAddr*v_Factor) ,0);
		//Osd_Draw4Num ( 15, 17, ((u16WordAddr+u16WordAddr2)*v_Factor) ,0);
		#if HKC_SETSUBWINDOW
		UserPrefAccSetupWinXpos=u16Value;
		UserPrefAccSetupWinYpos=u16TempValue;
		UserPrefAccSetupWinWidth=u16WordAddr1;
		UserPrefAccSetupWinHeight=u16WordAddr2;
		DDCCI_AccSetupWin( UserPrefAccSetupWinXpos, UserPrefAccSetupWinYpos, UserPrefAccSetupWinWidth,UserPrefAccSetupWinHeight);
		#if 0//UseFlashSave
		SaveMonitorSetting();
		#else
		NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinXpos), UserPrefAccSetupWinXpos);
		NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinYpos), UserPrefAccSetupWinYpos);
		NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinWidth), UserPrefAccSetupWinWidth);
		NVRam_WriteByte(NVR_MONITOR_ADDR(AccSetupWinHeight), UserPrefAccSetupWinHeight);
		#endif	
		#endif
    		u8ValueL = 0x00;
    		u8ValueH = 0x00;
	}
#endif
#if HKC_ColorTemp_Adjust
       else if(u8CPCode == 0x7B && u8PageValue == 0)
       {
            Resault=HKCAlignControl();
       }
#endif	
        else
        {
            DDCBuffer[2] = 0x01;
        }

        if(u8CommandCode == SETVCPFeature)
        {
       #if HKC_ColorTemp_Adjust
	   if(u8CPCode == 0x7B)	
            return Resault;
	   else
	#endif
            return 0;
        }
        else
        {
            DDCBuffer[0] = 0x89;
            DDCBuffer[1] = 0xC7;
            DDCBuffer[3] = u8PageValue;
            DDCBuffer[4] = u8CPCode;
            DDCBuffer[5] = u8TPValue;
            DDCBuffer[6] = 0x00;
            DDCBuffer[7] = u8RetValueL;
            DDCBuffer[8] = u8ValueH;
            DDCBuffer[9] = u8ValueL;
            return 9;
        }
    }
    else if(u8CommandCode == SaveCurrentSetting)
    {
        //if( FACTORY_MODE_FLAG)
            //SaveFactorySetting();
        SaveUserPref();
        return 0;
    }
    return 0;
}


#if HKC_ColorTemp_Adjust//||HKC_DDCCI_WRITEEDID

bit ubAutoAlign=0;
#if ENABLE_HDCP
#define HDCP_L128		             128
#define HDCP_L33		             	33
WORD   DDCCI_pointer=0;
BYTE   DDCCI_cont=0;
BYTE   Hdcpsave_offset=0 ;
WORD  Hdcp_pointer=0;
WORD  DDCCi_checksun=0;
WORD  Tchecksun=0;
WORD  checksun_verify=0;
#endif

extern void mStar_AdjustBackgoundColor(BYTE u8Pattern);

#if ENABLE_VGA_INPUT
extern Bool AutoColor(void);
#endif
#if HKC_DDCCI_WRITEEDID
//extern void ClearDDCBuffer(void);
//extern BYTE xdata DDCBuf[128];
  extern void  HKC_ReadDefaultDDC(void);
#endif 
#ifdef Adj_DefBri_PWM
extern BYTE xdata  DefBriPwmFlag;
#endif

#if FDDCCI_JRY_WriteEDID
WORD  edid_cont=0;
WORD edid_pointer=0;

#define S__VGA			             0 
#define S__DVI		                    1	
#define S__HDMI			             2
#define S__DP			             3
#endif


BYTE HKCAlignControl(void)
{
#if HKC_DDCCI_WRITEEDID
XDATA BYTE DdcDataChksum=0;
//XDATA BYTE DdcDataLen=0;
XDATA BYTE DdcDataLoop=0;
//XDATA BYTE i=0;//=128;
#endif


//-----------------------------------tudar add for hkc autoalign start-----------------------
{
        //bColorTempAdjustFlag=1;
	if(ubAutoAlign==0)
	{
		if((DDCBuffer[3]==0x35)&&(DDCBuffer[4]==0x1f)&&(DDCBuffer[5]==0))//enter align mode
		{
			Set_ATEModeFlag();
			SET_FACTORY_MODE_FLAG();

                     if(BURNIN_MODE_FLAG && (SyncLossState(USER_PREF_INPUT_TYPE)))
                        g_u8MenuPageIndex=BURNIN_MENU;
            	       //else	 if(FACTORY_MODE_FLAG||BURNIN_MODE_FLAG)
			//  g_u8MenuPageIndex=MENU_FACTORYROOT;
                     else
			  g_u8MenuPageIndex=MENU_ROOT;
					   
			g_u8MenuItemIndex=0;  
			//ExecuteKeyEvent(MIA_REDRAW_MENU);

			ubAutoAlign=1;
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
		}
		else
		{
			return 0;
		}
	}
	

if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x13)&&(DDCBuffer[5]==0))//auto color
{ 
     #if ENABLE_VGA_INPUT
		 if(AutoColor())
		 {
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
		 }
		 else
     #endif		 	
		 {
			DDCBuffer[1]=0x03;
			DDCBuffer[2]=0x58;
		 }	
			DDCBuffer[0]=0x84;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x14)&&(DDCBuffer[5]==0))//set CT
{
			CURRENT_COLOR_TEMP=PCT_9300K;
			SetColorTempDDCCI();
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x14)&&(DDCBuffer[5]==1))//set CT
{
			CURRENT_COLOR_TEMP=PCT_6500K;
			SetColorTempDDCCI();
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x14)&&(DDCBuffer[5]==2))//set CT
{
		#if ((defined(RunFeng_Project))||(defined(KTC_Project)))
			CURRENT_COLOR_TEMP=PCT_5700K;
			SetColorTempDDCCI();
		#else
			CURRENT_COLOR_TEMP=PCT_6500K;
			SetColorTempDDCCI();
		#endif	
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x15))//read R gain
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
		if(CURRENT_COLOR_TEMP==PCT_6500K)
			DDCBuffer[2]=USER_PREF_RED_COLOR_6500K;//0x4F;
		else if(CURRENT_COLOR_TEMP==PCT_9300K)
			DDCBuffer[2]=USER_PREF_RED_COLOR_9300K;//0x4F;
		else if(CURRENT_COLOR_TEMP==PCT_5700K)
			DDCBuffer[2]=USER_PREF_RED_COLOR_5700K;//0x4F;
		else 
			DDCBuffer[2]=CURRENT_RED_COLOR;//0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x16))//read G gain
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
		if(CURRENT_COLOR_TEMP==PCT_6500K)
			DDCBuffer[2]=USER_PREF_GREEN_COLOR_6500K;//0x4F;
		else if(CURRENT_COLOR_TEMP==PCT_9300K)
			DDCBuffer[2]=USER_PREF_GREEN_COLOR_9300K;//0x4F;
		else if(CURRENT_COLOR_TEMP==PCT_5700K)
			DDCBuffer[2]=USER_PREF_GREEN_COLOR_5700K;//0x4F;
		else 
			DDCBuffer[2]=CURRENT_GREEN_COLOR;//0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x17))//read B gain
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
		if(CURRENT_COLOR_TEMP==PCT_6500K)
			DDCBuffer[2]=USER_PREF_BLUE_COLOR_6500K;//0x4F;
		else if(CURRENT_COLOR_TEMP==PCT_9300K)
			DDCBuffer[2]=USER_PREF_BLUE_COLOR_9300K;//0x4F;
		else if(CURRENT_COLOR_TEMP==PCT_5700K)
			DDCBuffer[2]=USER_PREF_BLUE_COLOR_5700K;//0x4F;
		else 
			DDCBuffer[2]=CURRENT_BLUE_COLOR;//0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x15))//write R gain
{
			BYTE ucR_Gain;
			ucR_Gain=DDCBuffer[5];
			if((ucR_Gain >= 0) && (ucR_Gain <= 255))
			{
				if(CURRENT_COLOR_TEMP == PCT_6500K) 
				{
					USER_PREF_RED_COLOR_6500K=ucR_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_6500K,USER_PREF_GREEN_COLOR_6500K,USER_PREF_BLUE_COLOR_6500K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_9300K) 
				{
					USER_PREF_RED_COLOR_9300K=ucR_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_9300K,USER_PREF_GREEN_COLOR_9300K,USER_PREF_BLUE_COLOR_9300K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_5700K) 
				{
					USER_PREF_RED_COLOR_5700K=ucR_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_5700K,USER_PREF_GREEN_COLOR_5700K,USER_PREF_BLUE_COLOR_5700K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_USER) 
				{
					USER_PREF_ADC_RED_GAIN=ucR_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);   
				}
			       //mStar_AdjustRedColor(CURRENT_CONTRAST,ucR_Gain);
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x16))//write G gain
{
			BYTE ucG_Gain;
			ucG_Gain=DDCBuffer[5];
			if((ucG_Gain >= 0) && (ucG_Gain <= 255))
			{
				if(CURRENT_COLOR_TEMP == PCT_6500K) 
				{
					USER_PREF_GREEN_COLOR_6500K=ucG_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_6500K,USER_PREF_GREEN_COLOR_6500K,USER_PREF_BLUE_COLOR_6500K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_9300K) 
				{
					USER_PREF_GREEN_COLOR_9300K=ucG_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_9300K,USER_PREF_GREEN_COLOR_9300K,USER_PREF_BLUE_COLOR_9300K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_5700K) 
				{
					USER_PREF_GREEN_COLOR_5700K=ucG_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_5700K,USER_PREF_GREEN_COLOR_5700K,USER_PREF_BLUE_COLOR_5700K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_USER) 
				{
					CURRENT_GREEN_COLOR=ucG_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);   
				}
  			       //mStar_AdjustGreenColor(CURRENT_CONTRAST,ucG_Gain);
				
				
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x17))//write B gain
{
			BYTE ucB_Gain;
			ucB_Gain=DDCBuffer[5];
			if((ucB_Gain >= 0) && (ucB_Gain <= 255))
			{
				if(CURRENT_COLOR_TEMP == PCT_6500K) 
				{
					USER_PREF_BLUE_COLOR_6500K=ucB_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_6500K,USER_PREF_GREEN_COLOR_6500K,USER_PREF_BLUE_COLOR_6500K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_9300K) 
				{
					USER_PREF_BLUE_COLOR_9300K=ucB_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_9300K,USER_PREF_GREEN_COLOR_9300K,USER_PREF_BLUE_COLOR_9300K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_5700K) 
				{
					USER_PREF_BLUE_COLOR_5700K=ucB_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, USER_PREF_RED_COLOR_5700K,USER_PREF_GREEN_COLOR_5700K,USER_PREF_BLUE_COLOR_5700K);   
				}
				else if(CURRENT_COLOR_TEMP == PCT_USER) 
				{
					CURRENT_BLUE_COLOR=ucB_Gain;
				       msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);   
				}
			       //mStar_AdjustBlueColor(CURRENT_CONTRAST,ucB_Gain);
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x18)&&(DDCBuffer[5]==0))//save CT
{
			CURRENT_COLOR_TEMP=PCT_9300K;
                     SaveFactorySetting();
			SaveNVRAMBlock(EEP_COLOR_MEMBER_ADDR(u8COLOR_ColorTemp), sizeof(CURRENT_COLOR_TEMP));		 
			
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x18)&&(DDCBuffer[5]==1))//save CT
{
			CURRENT_COLOR_TEMP=PCT_6500K;
                     SaveFactorySetting();
			SaveNVRAMBlock(EEP_COLOR_MEMBER_ADDR(u8COLOR_ColorTemp), sizeof(CURRENT_COLOR_TEMP));		 

			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x18)&&(DDCBuffer[5]==2))//save CT
{
		#if (defined(RunFeng_Project))
			CURRENT_COLOR_TEMP=PCT_5700K;
		#else
			CURRENT_COLOR_TEMP=PCT_6500K;
		#endif
                     SaveFactorySetting();
			SaveNVRAMBlock(EEP_COLOR_MEMBER_ADDR(u8COLOR_ColorTemp), sizeof(CURRENT_COLOR_TEMP));		 


			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
 #ifdef Adj_DefBri_PWM
else if((DDCBuffer[3]==0x37)&&(DDCBuffer[4]==0x01))//Get  PWM adjust range
{

//       #define BRIGHTNESS_REAL_MIN                       0x1F0//496
//       #define BRIGHTNESS_REAL_MID                       0x750//1872
//       #define BRIGHTNESS_REAL_MAX                      0x9B0
                DefBriPwmFlag=1;
               hw_ClrGreenLed();
		   DDCBuffer[0]=0x8B;
		   DDCBuffer[1]=0x00;
		   DDCBuffer[2]=0x4F;
		   DDCBuffer[3]=(USER_PREF_DEF_SUBBRIGHTNESS>>8)&0xFF;
		   DDCBuffer[4]=USER_PREF_DEF_SUBBRIGHTNESS&0xFF;
		   DDCBuffer[5]=(BRIGHTNESS_REAL_MAX>>8)&0xFF;
		   DDCBuffer[6]=BRIGHTNESS_REAL_MAX&0xFF;
		   DDCBuffer[7]=((BRIGHTNESS_REAL_MID -(BRIGHTNESS_REAL_MID-BRIGHTNESS_REAL_MIN)/2)>>8)&0xFF;
		   DDCBuffer[8]=(BRIGHTNESS_REAL_MID -(BRIGHTNESS_REAL_MID-BRIGHTNESS_REAL_MIN)/2)&0xFF;
		   DDCBuffer[9]=DDCBuffer[3]+DDCBuffer[4]+DDCBuffer[5]+DDCBuffer[6]+DDCBuffer[7]+DDCBuffer[8];
		   return 9;
}

else if((DDCBuffer[3]==0x37)&&(DDCBuffer[4]==0x02))//set Default  PWM adjust range
{
                     DefBriPwmFlag=1;
                     USER_PREF_BRIGHTNESS=90;			
			USER_PREF_DEF_SUBBRIGHTNESS=(DDCBuffer[5]<<8)|DDCBuffer[6];
	        if((USER_PREF_DEF_SUBBRIGHTNESS<(BRIGHTNESS_REAL_MID -(BRIGHTNESS_REAL_MID-BRIGHTNESS_REAL_MIN)/2)) || (USER_PREF_DEF_SUBBRIGHTNESS>BRIGHTNESS_REAL_MID))
			USER_PREF_DEF_SUBBRIGHTNESS=BRIGHTNESS_REAL_MID -(BRIGHTNESS_REAL_MID-BRIGHTNESS_REAL_MIN)/2;
		   msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x00;
		   DDCBuffer[2]=0x4F;	
		   return 2;
}
//else if((DDCBuffer[3]==0x37)&&(DDCBuffer[4]==0x01)&&(DDCBuffer[5]==0))//Get Default  PWM adjust range
//{
             
//}
else if((DDCBuffer[3]==0x37)&&(DDCBuffer[4]==0x03))//Save Default Brightness PWM
{
       DefBriPwmFlag=1;

     #if USEFLASH
      SaveMonitorSetting();
     #else 
      SaveNVRAMBlock(EEP_FACTORY_MEMBER_ADDR(u8FAC_FacBrightness), sizeof(USER_PREF_DEF_SUBBRIGHTNESS));
      SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u8SYS_BackLight), sizeof(USER_PREF_BRIGHTNESS));
     #endif    
	DDCBuffer[0]=0x84;
	DDCBuffer[1]=0x00;
	DDCBuffer[2]=0x4F;	
	 return 2;	

}
#endif
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x19)&&(DDCBuffer[5]==0))//set brightness and contrast default
{
			USER_PREF_BRIGHTNESS=DEF_BRIGHTNESS;
			CURRENT_CONTRAST=DEF_CONTRAST;
			msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
			msAPI_AdjustContrast(USER_PREF_WIN_SEL, CURRENT_CONTRAST);
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x1A))//read adjust brightness 
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=USER_PREF_BRIGHTNESS;
			return 2;
}
else if((DDCBuffer[3]==0x32)&&(DDCBuffer[4]==0x1B))//read adjust contrast
{
                     //ContrastTempValue=GetScale100Value(CURRENT_CONTRAST, MIN_CONTRAST_VALUE, MAX_CONTRAST_VALUE);
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=CURRENT_CONTRAST;//ContrastTempValue;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1A))//write adjust brightness 
{
			BYTE uctempData;
			uctempData=DDCBuffer[5];
			if((uctempData >= 0) && (uctempData <= 100))
			{
				USER_PREF_BRIGHTNESS=uctempData;
				msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			DDCBuffer[0]=0x84;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1B))//write adjust contrast
{
			BYTE uctempData;
			uctempData=DDCBuffer[5];
			if((uctempData >= 0) && (uctempData <= 100))
			{
			       //ContrastTempValue=uctempData;
				CURRENT_CONTRAST=uctempData;
				//CURRENT_CONTRAST=(MIN_CONTRAST_VALUE+((MAX_CONTRAST_VALUE-MIN_CONTRAST_VALUE)/100)*ContrastTempValue);
				msAPI_AdjustContrast(USER_PREF_WIN_SEL, CURRENT_CONTRAST);
				
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
			}
			else
			{
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
			}
			DDCBuffer[0]=0x84;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1C))//set default language or select reset language
{
			USER_PREF_LANGUAGE=DEF_LANGUAGE;
			//LoadLanguageFont(USER_PREF_LANGUAGE);
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1D)&&(DDCBuffer[5]==0))//enter burn mode
{
			return 0;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x1D)&&(DDCBuffer[5]==1))//exit burn mode
{
			return 0;
}
else if((DDCBuffer[3]==0x35)&&(DDCBuffer[4]==0x1F)&&(DDCBuffer[5]==0))//enter align mode
{
			ubAutoAlign=1;
			Set_ATEModeFlag();
			SET_FACTORY_MODE_FLAG();

			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x35)&&(DDCBuffer[4]==0x1F)&&(DDCBuffer[5]==1))//exit align mode
{
			ubAutoAlign=0;
			//Clr_ATEModeFlag();
			CLR_FACTORY_MODE_FLAG();
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}

else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x21)&&(DDCBuffer[5]==0))//factory mode on
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x21)&&(DDCBuffer[5]==1))//factory mode off
{
ubAutoAlign=0;
		if(FACTORY_MODE_FLAG)
		{	
			//Clr_ATEModeFlag();
			CLR_FACTORY_MODE_FLAG();
			//Set_HideFactoryRootMenuFlag();//20090521
                      SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(MONITOR_FLAGS));

                     if(BURNIN_MODE_FLAG && (SyncLossState(USER_PREF_INPUT_TYPE)))
                        g_u8MenuPageIndex=BURNIN_MENU;
            	       else	 
			 g_u8MenuPageIndex=MENU_ROOT;
					   
			 g_u8MenuItemIndex=0; 
			 Menu_Hide();		
		        g_u8SaveUserData|=SAVE_FACTORY_SETTING|SAVE_MONITOR_SETTING|SAVE_MODE_SETTING;
			 SaveUserPrefSetting(MENU_ROOT);
		}
		else
		{
		        g_u8SaveUserData|=SAVE_FACTORY_SETTING|SAVE_MONITOR_SETTING|SAVE_MODE_SETTING;
			SaveUserPrefSetting(MENU_ROOT);//SaveUserPref();
		}
			ResetAllSetting();

			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x23)&&(DDCBuffer[5]==0))//change source vga
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x23)&&(DDCBuffer[5]==1))//change source dvi
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
}
#if HKC_DDCCI_WRITEEDID//write key ddc data
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x40))//Start DDC data update
{
         HKC_ReadDefaultDDC();

	  DDCBuffer[0]=0x84;
	  DDCBuffer[1]=0x00;
	  DDCBuffer[2]=0x4F;
	  return 2;	 

}
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x50))//Finish DDC data update
{
	    Set_EDID_Update_Flag();
           CheckUpdateEDID=1;
//	hw_ClrGreenLed();
         if(BURNIN_MODE_FLAG && (SyncLossState(USER_PREF_INPUT_TYPE)))
            g_u8MenuPageIndex=BURNIN_MENU;
	  //else if(FACTORY_MODE_FLAG||BURNIN_MODE_FLAG)	 
         //   g_u8MenuPageIndex=MENU_FACTORYROOT;
	  else
            g_u8MenuPageIndex=MENU_ROOT;
	  	
       SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_DDCFlag), sizeof(DDCCI_FUN_FLAGS));
	//NVRam_ReadTbl(NVRAM_ADDR_DDC_VGA, &DDCBuf, 128);
       DDCBuffer[0]=0x84;
	DDCBuffer[1]=0x00;
	DDCBuffer[2]=0x4F;
	return 2; 

}
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x51))//Verify DDC Updated 
{
    //   hw_ClrGreenLed();
            if(EDID_Update_Flag)
            	{
	        DDCBuffer[1]=0x03;
	        DDCBuffer[2]=0x58;	
	        return 2;   	
            	}
	     else
	     	{
	   //       hw_ClrGreenLed();
	
                DDCBuffer[0]=0x84;
	         DDCBuffer[1]=0x00;
	         DDCBuffer[2]=0x4F;		
		 return 2;
	     	}

}

else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x41))//update Vendor / Product ID
{  

     DdcDataChksum=0;
	 for(DdcDataLoop=0;DdcDataLoop<10;DdcDataLoop++)
	 { 
	   DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
	 }
	   if(DDCBuffer[0x05+10]==DdcDataChksum)
	 {
	   for(DdcDataLoop=0;DdcDataLoop<10;DdcDataLoop++)
	   { 
	      DDCBuf[0x08+DdcDataLoop]=DDCBuffer[0x05+DdcDataLoop];
	   }


	   	      DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
	 }
	  else
	  	{
	  		DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x03;
			DDCBuffer[2]=0x58;
			return 2;	
	  	}	 
}

else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x42))//Write Basic Display parameters/Features
{
   DdcDataChksum=0;
   for(DdcDataLoop=0;DdcDataLoop<5;DdcDataLoop++)
   { 
	 DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
   }
	 if(DDCBuffer[0x05+5]==DdcDataChksum)
   {
		 for(DdcDataLoop=0;DdcDataLoop<5;DdcDataLoop++)
		   DDCBuf[0x14+DdcDataLoop]=DDCBuffer[5+DdcDataLoop];

		  DDCBuffer[0]=0x84;
		  DDCBuffer[1]=0x00;
		  DDCBuffer[2]=0x4F;
		  return 2;
   }
	else
	  {
		  DDCBuffer[0]=0x84;
		  DDCBuffer[1]=0x03;
		  DDCBuffer[2]=0x58;
		  return 2;   
	  }

}
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x43))//Write color characteristics
{


DdcDataChksum=0;
for(DdcDataLoop=0;DdcDataLoop<10;DdcDataLoop++)
{ 
  DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
}
  if(DDCBuffer[0x05+10]==DdcDataChksum)
{
	  for(DdcDataLoop=0;DdcDataLoop<10;DdcDataLoop++)
		DDCBuf[0x19+DdcDataLoop]=DDCBuffer[5+DdcDataLoop];

	   DDCBuffer[0]=0x84;
	   DDCBuffer[1]=0x00;
	   DDCBuffer[2]=0x4F;
	   return 2;
}
 else
   {
	   DDCBuffer[0]=0x84;
	   DDCBuffer[1]=0x03;
	   DDCBuffer[2]=0x58;
	   return 2;   
   }

}
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x44))//Write Established Timings 
{

	 DdcDataChksum=0;
	 for(DdcDataLoop=0;DdcDataLoop<3;DdcDataLoop++)
	 { 
	   DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
	 }
	   if(DDCBuffer[0x05+3]==DdcDataChksum)
	 {
		   for(DdcDataLoop=0;DdcDataLoop<3;DdcDataLoop++)
			 DDCBuf[0x23+DdcDataLoop]=DDCBuffer[5+DdcDataLoop];
	 
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
	 }
	  else
		{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x03;
			DDCBuffer[2]=0x58;
			return 2;	
		}
}
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x45))//Write standard timing ID
{   
	DdcDataChksum=0;
	for(DdcDataLoop=0;DdcDataLoop<16;DdcDataLoop++)
	{ 
	  DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
	}
	  if(DDCBuffer[0x05+16]==DdcDataChksum)
	{
		  for(DdcDataLoop=0;DdcDataLoop<16;DdcDataLoop++)
			DDCBuf[0x26+DdcDataLoop]=DDCBuffer[5+DdcDataLoop];
	
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x00;
		   DDCBuffer[2]=0x4F;
		   return 2;
	}
	 else
	   {
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x03;
		   DDCBuffer[2]=0x58;
		   return 2;   
	   } 
}
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x46))//Write Block1
{
	DdcDataChksum=0;
	for(DdcDataLoop=0;DdcDataLoop<18;DdcDataLoop++)
	{ 
	  DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
	}
	  if(DDCBuffer[0x05+18]==DdcDataChksum)
	{
		  for(DdcDataLoop=0;DdcDataLoop<18;DdcDataLoop++)
			DDCBuf[0x36+DdcDataLoop]=DDCBuffer[5+DdcDataLoop];
	
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x00;
		   DDCBuffer[2]=0x4F;
		   return 2;
	}
	 else
	   {
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x03;
		   DDCBuffer[2]=0x58;
		   return 2;   
	   }

}

else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x47))//Write Block2
{
	DdcDataChksum=0;
	for(DdcDataLoop=0;DdcDataLoop<18;DdcDataLoop++)
	{ 
	  DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
	}
	  if(DDCBuffer[0x05+18]==DdcDataChksum)
	{
		  for(DdcDataLoop=0;DdcDataLoop<18;DdcDataLoop++)
			DDCBuf[0x48+DdcDataLoop]=DDCBuffer[5+DdcDataLoop];
	
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x00;
		   DDCBuffer[2]=0x4F;
		   return 2;
	}
	 else
	   {
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x03;
		   DDCBuffer[2]=0x58;
		   return 2;   
	   }

}
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x48))//Write Block3
{
	DdcDataChksum=0;
	for(DdcDataLoop=0;DdcDataLoop<18;DdcDataLoop++)
	{ 
	  DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
	}
	  if(DDCBuffer[0x05+18]==DdcDataChksum)
	{
		  for(DdcDataLoop=0;DdcDataLoop<18;DdcDataLoop++)
			DDCBuf[0x5A+DdcDataLoop]=DDCBuffer[5+DdcDataLoop];
	
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x00;
		   DDCBuffer[2]=0x4F;
		   return 2;
	}
	 else
	   {
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x03;
		   DDCBuffer[2]=0x58;
		   return 2;   
	   }

}
else if((DDCBuffer[3]==0x40)&&(DDCBuffer[4]==0x49))//Write Block4
{
	DdcDataChksum=0;
	for(DdcDataLoop=0;DdcDataLoop<18;DdcDataLoop++)
	{ 
	  DdcDataChksum+=DDCBuffer[0x05+DdcDataLoop];
	}
	  if(DDCBuffer[0x05+18]==DdcDataChksum)
	{
		  for(DdcDataLoop=0;DdcDataLoop<18;DdcDataLoop++)
			DDCBuf[0x6C+DdcDataLoop]=DDCBuffer[5+DdcDataLoop];
	
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x00;
		   DDCBuffer[2]=0x4F;
		   return 2;
	}
	 else
	   {
		   DDCBuffer[0]=0x84;
		   DDCBuffer[1]=0x03;
		   DDCBuffer[2]=0x58;
		   return 2;   
	   }

}
#endif
#if FDDCCI_JRY_WriteEDID
   else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x30))//DDC长度
{
        //    Set_ATEModeFlag();
  //      EdidProcess=1;
		hw_ClrDDC_WP();
              Set_WriteEDIDFlag();

	          edid_cont=((DDCBuffer[5])*128);
		   edid_pointer=0;
		    if((DDCBuffer[5])<3)
			{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x00;
			DDCBuffer[2]=0x4F;
			return 2;
			}
		    else
			{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x03;
			DDCBuffer[2]=0x58;
			return 2;				
			}
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x31))//DDC接收
{
			XDATA BYTE j,i=0;
		   	j=DDCBuffer[5];
			if (edid_pointer<edid_cont)
			 {
	                 for(i=0;i<j;i++)
			      {
				 DDCBuf[edid_pointer]=DDCBuffer[6+i];
				 edid_pointer++;
			      }
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
				return 2;
			 }
			else
			  {
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				return 2;
  		         }				
	
}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x32))   //2    ddC 校验
{
			XDATA BYTE checksun=0;
			XDATA WORD i;
			if ((edid_cont==128)||(edid_cont==256))
				{
				for(i=0;i<edid_cont;i++)
					{
					checksun+=DDCBuf[i];
					
					}
					if(checksun==0)
						{
						DDCBuffer[0]=0x84;
						DDCBuffer[1]=0x00;
						DDCBuffer[2]=0x4F;
						return 2;
						}

				}
				else
					{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				return 2;
					}

}
else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x33))//ddc保存
{
			XDATA BYTE checksun=0;
			BYTE  EDID_TYPE=0;
			//WORD u16Counter=0;

 			//Osd_Show();
			if(DDCBuffer[5]==S__VGA)
			{
				EDID_TYPE=S__VGA;				
				ReLoadcounter=0;
				Set_WriteEDIDFlag();
			}
			else if (DDCBuffer[5]==S__DVI)
			{
				EDID_TYPE=S__DVI;				
				ReLoadcounter=0;
				Set_WriteEDIDFlag();
			}
			else if (DDCBuffer[5]==S__HDMI)
			{
				EDID_TYPE=S__HDMI;				
				ReLoadcounter=0;
				Set_WriteEDIDFlag();
			}
			else if (DDCBuffer[5]==S__DP)
			{
				EDID_TYPE=S__DP;				
				ReLoadcounter=0;
				Set_WriteEDIDFlag();
			}


			//Write to 24C16
			 if(EDID_TYPE==S__VGA)
			 {
                      //NVRam_WriteTbl(NVRAM_ADDR_DDC_VGA, &DDCBuf, 128);
                      /*
                       for(u16Counter = 0; u16Counter< EDID_VGA_SIZE; u16Counter++)
                       {
                           u8NVRAM_MAP[ADDR_EDID_VGA+i]=DDCBuf[i];
                       }			
			 SaveNVRAMBlock(ADDR_EDID_VGA, EDID_VGA_SIZE);
			 */
			 NVRam_WriteTbl(ADDR_EDID_VGA, (BYTE *) &DDCBuf, EDID_VGA_SIZE);
			 }
			 else if (EDID_TYPE==S__DVI)
			 {
                      //NVRam_WriteTbl(NVRAM_ADDR_DDC_DVI, &DDCBuf, 128);
                      /*
                       for(u16Counter = 0; u16Counter< EDID_DIGITAL1_SIZE; u16Counter++)
                       {
                           u8NVRAM_MAP[ADDR_EDID_DIGITAL1+i]=DDCBuf[i];
                       }			
			 SaveNVRAMBlock(ADDR_EDID_DIGITAL1, EDID_DIGITAL1_SIZE);	
			 */
			 NVRam_WriteTbl(ADDR_EDID_DIGITAL1, (BYTE *) &DDCBuf, EDID_DIGITAL1_SIZE);
			 }
			 else if (EDID_TYPE==S__HDMI)
			 {
                      //NVRam_WriteTbl(NVRAM_ADDR_DDC_HDMI, &DDCBuf, 128);
                      /*
                       for(u16Counter = 0; u16Counter< EDID_DIGITAL2_SIZE; u16Counter++)
                       {
                           u8NVRAM_MAP[ADDR_EDID_DIGITAL2+i]=DDCBuf[i];
                       }			
			 SaveNVRAMBlock(ADDR_EDID_DIGITAL2, EDID_DIGITAL2_SIZE);	
			 */
			 NVRam_WriteTbl(ADDR_EDID_DIGITAL2, (BYTE *) &DDCBuf, EDID_DIGITAL2_SIZE);
			 }
			 else if (EDID_TYPE==S__DP)
			 {
                      //NVRam_WriteTbl(NVRAM_ADDR_DDC_HDMI, &DDCBuf, 128);
                      /*
                       for(u16Counter = 0; u16Counter< EDID_DP_SIZE; u16Counter++)
                       {
                           u8NVRAM_MAP[ADDR_EDID_DP+i]=DDCBuf[i];
                       }			
			 SaveNVRAMBlock(ADDR_EDID_DP, EDID_DP_SIZE);		
			 */
			 NVRam_WriteTbl(ADDR_EDID_DP, (BYTE *) &DDCBuf, EDID_DP_SIZE);
			 }

			//---Get temp checksun
			 checksun=DDCBuf[127];
			 DDCBuf[127]=0;



			 //----Read CheckSun
			if(EDID_TYPE==S__VGA)
			{
			#if 1
			 NVRam_ReadTbl(ADDR_EDID_VGA, (BYTE*)&DDCBuf, EDID_VGA_SIZE);	
		       #else	 
			 ReadNVRAMBlock(ADDR_EDID_VGA, EDID_VGA_SIZE);		  
                     for(u16Counter = 0; u16Counter< EDID_VGA_SIZE; u16Counter++)
                     {
                         DDCBuf[127] = u8NVRAM_MAP_BK[ADDR_EDID_VGA+127];
                     }			
			#endif		 
			}
			else if (EDID_TYPE==S__DVI)
			{
			#if 1
			 NVRam_ReadTbl(ADDR_EDID_DIGITAL1, (BYTE*)&DDCBuf, EDID_DIGITAL1_SIZE);		
			 #else
			 ReadNVRAMBlock(ADDR_EDID_DIGITAL1, EDID_DIGITAL1_SIZE);		  
                     for(u16Counter = 0; u16Counter< EDID_DIGITAL1_SIZE; u16Counter++)
                     {
                         DDCBuf[127] = u8NVRAM_MAP_BK[ADDR_EDID_DIGITAL1+127];
                     }	
			#endif		 
			}
			else if (EDID_TYPE==S__HDMI)
			{
			#if 1
			 NVRam_ReadTbl(ADDR_EDID_DIGITAL2, (BYTE*)&DDCBuf, EDID_DIGITAL2_SIZE);	
			#else
			 ReadNVRAMBlock(ADDR_EDID_DIGITAL2, EDID_DIGITAL2_SIZE);		  
                     for(u16Counter = 0; u16Counter< EDID_DIGITAL2_SIZE; u16Counter++)
                     {
                         DDCBuf[127] = u8NVRAM_MAP_BK[ADDR_EDID_DIGITAL2+127];
                     }	
			#endif		 
			}
			else if (EDID_TYPE==S__DP)
			{
			#if 1
			 NVRam_ReadTbl(ADDR_EDID_DP, (BYTE*)&DDCBuf, EDID_DP_SIZE);	
			#else
			 ReadNVRAMBlock(ADDR_EDID_DP, EDID_DP_SIZE);		  
                     for(u16Counter = 0; u16Counter< EDID_DP_SIZE; u16Counter++)
                     {
                         DDCBuf[127] = u8NVRAM_MAP_BK[ADDR_EDID_DP+127];
                     }			
			#endif		 
			}

					
			    if(DDCBuf[127]==checksun)
				{
				   
					 if( EDID_TYPE==S__VGA)
	                              {
	                                   Set_VGA_EDID_DDCCIReLoad_Flag();
					       ReLoadcounter=15;
					       Clr_WriteEDIDFlag();	 
	                              }        
					 if( EDID_TYPE==S__DVI)
	                              {
	                                 Set_DVI_EDID_DDCCIReLoad_Flag();
					     ReLoadcounter=15;
					     Clr_WriteEDIDFlag();	 
	                              }        
					 if( EDID_TYPE==S__HDMI)
	                              {
					      Set_HDMI_EDID_DDCCIReLoad_Flag();
					      ReLoadcounter=15;
					      Clr_WriteEDIDFlag();	 
	                              }  
					 if( EDID_TYPE==S__DP)
	                              {
					      Set_DP_EDID_DDCCIReLoad_Flag();
					      ReLoadcounter=15;
					      Clr_WriteEDIDFlag();	 
	                              }  
					 
				 SaveNVRAMBlock(EEP_SYSTEM_MEMBER_ADDR(u16SYS_SystemFlag), sizeof(DDCCI_FUN_FLAGS));
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
				//Set_Edid_WP();
				return 2;
				}
			      else
				{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				Clr_WriteEDIDFlag();	 
				return 2;
				}
			
}
#endif	 
#if ENABLE_HDCP
	else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x34))//HDCP 长度
		{
			if((DDCBuffer[5])==6)
				{
				//当前批次[6]
				//分三批，第一/二批128BYTE,
				//第三批33YBTE.
				//当为第一批次时清零校验和。
				if(DDCBuffer[6]==0)
                             {            
				    DDCCi_checksun=0;
                                Flash_HDCPcont=0;
                               }
				// 3个批次，第一次传0，第二次传1，第三次传2
				Hdcpsave_offset=DDCBuffer[6];
				//DDCCI_cont本批大小[7]；
				DDCCI_cont=DDCBuffer[7];
				//Hdcp_pointer本批次的OFFSET 起始位置[8][9]
				Hdcp_pointer=DDCBuffer[8];
				Hdcp_pointer=Hdcp_pointer<<8;
				Hdcp_pointer=Hdcp_pointer|DDCBuffer[9];
				//Tchecksun本批次的Checksun
				Tchecksun=DDCBuffer[10];
				Tchecksun=Tchecksun<<8;
				Tchecksun=Tchecksun|DDCBuffer[11];
				
				DDCCI_pointer=0;
				//DDCCI_pointer=0;
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x00;
				DDCBuffer[2]=0x4F;
				return 2;
				}
			else
				{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				return 2;				
				}
		}

	else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x35))//HDCP  接收
		{
			XDATA BYTE j,i=0;
			j=DDCBuffer[5];
			if(DDCCI_pointer<DDCCI_cont)
				{
				//传4批次，每批次32BYTE
		                for(i=0;i<j;i++)
					{
					
					  if(Flash_HDCPcont==0)
                                        {               
       					DDC1[DDCCI_pointer]=DDCBuffer[6+i];
       					DDCCi_checksun+=DDC1[DDCCI_pointer];
                                         }
                                     else if(Flash_HDCPcont==1)
                                        {
                                          DDC1[DDCCI_pointer+128]=DDCBuffer[6+i];
       					DDCCi_checksun+=DDC1[DDCCI_pointer+128];
                                         }
                                     else if(Flash_HDCPcont==2)
                                       {   
                                          DDC1[DDCCI_pointer+256]=DDCBuffer[6+i];
       					DDCCi_checksun+=DDC1[DDCCI_pointer+256];
                                        }
                                 
       					DDCCI_pointer++;
					}
				if(DDCCI_pointer<=(DDCCI_cont-1))//2<=  保证卡在最后一次发送校验码
					{

						DDCBuffer[0]=0x84;
						DDCBuffer[1]=0x00;
						DDCBuffer[2]=0x4F;
						return 2;
					}
				else if((DDCCI_pointer==DDCCI_cont)&&(Tchecksun==DDCCi_checksun))
					{
	//////////////////////////////至此->本批次接收数据OK，可以以Hdcp_pointer为OFFSET 进行保存/////////////////////////////////////
       //Flash_Write_Factory_KeySet(FLASH_KEY_HDCP, Hdcp_pointer,  DDCCI_cont, RX_EDID, DDCCI_cont);	
                                        if(Flash_HDCPcont==2)
                        			{
                        			  #if(!ENABLE_HDCP22)
							NVRam_WriteTbl( ADDR_HDCP_BLOCK , DDC1, 128 );
							NVRam_WriteTbl( ADDR_HDCP_BLOCK+128 , DDC1+128, 128 );
							NVRam_WriteTbl( ADDR_HDCP_BLOCK+256 , DDC1+256, 33 );	
						  #endif	
                        			}
                        		       Flash_HDCPcont++;
                                       
						DDCBuffer[0]=0x84;
						DDCBuffer[1]=DDCCi_checksun&0xff;
						DDCBuffer[2]=DDCCi_checksun>>8;
						return 2;
					}
				else
					{
					DDCBuffer[0]=0x84;
					DDCBuffer[1]=0x03;
					DDCBuffer[2]=0x58;
					return 2;				
					}

				}		
			else
				{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				return 2;				
				}
		}
	else if((DDCBuffer[3]==0x39)&&(DDCBuffer[4]==0x36))//HDCP 校验
		{
		 WORD wCount ; 
              XDATA BYTE i=0;
		//在此处对保存的数据进行读取，如果校验和与此前接收的校验和相等则OK,否则NG.
		 checksun_verify=0;

            for(i=0;i<20;i++)
                DDC1[i]=0;
                #if(!ENABLE_HDCP22)
			NVRam_ReadTbl( ADDR_HDCP_BLOCK , (BYTE*)&DDC1, 128 );
			NVRam_ReadTbl( ADDR_HDCP_BLOCK+128 , (BYTE*)&DDC1+128, 128 );
			NVRam_ReadTbl( ADDR_HDCP_BLOCK+256 , (BYTE*)&DDC1+256, 33 );
		 #endif	

		 	for(wCount=0; wCount<289; wCount++)
			{
			checksun_verify+=DDC1[wCount];
			}

			if(DDCCi_checksun==checksun_verify)
				{
				       Set_LoadHDCPKeyFlag();
					DDCBuffer[0]=0x84;
					DDCBuffer[1]=0x00;
					DDCBuffer[2]=0x4f;
					return 2;
				}
			else
				{
				DDCBuffer[0]=0x84;
				DDCBuffer[1]=0x03;
				DDCBuffer[2]=0x58;
				return 2;				
				}
		}
#endif

else
{
			DDCBuffer[0]=0x84;
			DDCBuffer[1]=0x03;
			DDCBuffer[2]=0x58;
			return 2;
}
}
//-----------------------------------tudar add for hkc autoalign end-----------------------


}
#endif




