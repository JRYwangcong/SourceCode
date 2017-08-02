#define _USERPREF_C_
#include <string.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "msOSD.h"
#include "MDebug.h"
#include "menudef.h"
#include "menufunc.h"
#include "DDC2Bi.H"
#include "NVRam.h"
#include "Adjust.h"
#include "ms_reg.h"
#include "Power.h"
#include "Ms_rwreg.h"
#include "misc.h"
#include "i2c.h"
#include "UserPref.h"
#include "Menu.h"
//#include "FactoryMenu.h"
//#include "FMenuFunc.h"
#include "FactoryAligment.h"
#include "drvDDC2Bi.h"
#include "UserPrefDef.h"
#include "appWindow.h"

#if defined (_WISTRON_ETO_)  //Poyen 20120229
extern void SetColorTempDDCCI(void);
#endif

//extern BYTE rxIndex;
//extern BYTE rxStatus;
//extern BYTE rxLength;
//extern BYTE rxChkSum;//=0;
//extern BYTE txStatus;
//extern BYTE txLength;

extern BYTE xdata u8CommandCode;
extern BYTE xdata u8CPCode;
extern BYTE xdata u8ValueH;
extern BYTE xdata u8ValueL;
extern BYTE xdata u8RetValueH;
extern BYTE xdata u8RetValueL;
extern BYTE xdata u8PageValue;
extern WORD xdata u16Value;
//extern BYTE idata rxInputPort;

//extern BYTE xdata DDCBuffer[]; //declared in DDC2Bi.h
extern  BYTE code tVersionString[] ;

extern void DDC2Bi_InitRx( void );
extern void DDC2Bi_Init( void );
extern Bool ExecuteKeyEvent( MenuItemActionType enMenuAction );
extern void Delay1ms( WORD msNums );
extern Bool AdjustMWEMode(BYTE u8View, BYTE  u8MWE_Mode );
extern Bool SetInputColorFormat( BYTE u8DispWin);//20070227-2
extern void DDC2Bi_MessageReady( void );
extern Bool AdjustFSubBrightness(MENU_ITEM_ACTION action);

//////////////////////////////////////////////////
#define T_Chinese   1
#define English     2
#define French      3
#define German      4
#define Italian     5
#define Japanese    6
#define Korean      7
#define Portuguese  8
#define Russian     9
#define Spanish     0x0A
#define S_Chinese   0x0D
#define Brazil      0x0E
#define Dutch       0x14

#ifndef FW_PIN_STRING
#define FW_PIN_STRING     "00.00000.000"
#endif
#ifndef MODEL_NAME_PIN_STRING
#define MODEL_NAME_PIN_STRING   "Dell xxxxxx"
#endif

BYTE code tFirmwarePinString[]=FW_PIN_STRING;
BYTE code tModelNamePinString[]=MODEL_NAME_PIN_STRING;
BYTE code tFWVersionString[]=VERSION_STRING;

BYTE code OSD_Language_TB[]=
{
#if 1
   T_Chinese,   LANG_INDEX_MAX,
   English,     LANG_English,
   French,      LANG_INDEX_MAX,
   German,      LANG_INDEX_MAX,
   Italian,     LANG_INDEX_MAX,
   Japanese,    LANG_INDEX_MAX,
   Korean,      LANG_INDEX_MAX,
   Portuguese,  LANG_INDEX_MAX,
   Russian,     LANG_INDEX_MAX,
   Spanish,     LANG_INDEX_MAX,
   S_Chinese,   LANG_INDEX_MAX,
   Brazil,      LANG_INDEX_MAX,
   Dutch,       LANG_INDEX_MAX
#else
   T_Chinese,   LANG_INDEX_MAX,
   English,     LANG_English,
   French,      LANG_FRANCE,
   German,      LANG_GERMAN,
   Italian,     LANG_INDEX_MAX,
   Japanese,    LANG_JAPAN,
   Korean,      LANG_INDEX_MAX,
   Portuguese,  LANG_INDEX_MAX,
 #if RUSSIAN
   Russian,     LANG_RUSSIAN,
 #else
   Russian,     LANG_INDEX_MAX,
 #endif
   Spanish,     LANG_SPANISH,
 #if SIMPLY_CHINESE
   S_Chinese,   LANG_SChinese,
 #else
   S_Chinese,   LANG_INDEX_MAX,
 #endif
 #if BRAZILIAN_PORTUGUESE
   Brazil,      LANG_BRAZILIAN_PORTUGUESE,
 #else
   Brazil,      LANG_INDEX_MAX,
 #endif
   Dutch,       LANG_INDEX_MAX
#endif
};

//////////////////////////////////////////////////
BYTE xdata u8DoAllResetBit=0;	//20080403
extern void msAPI_AdjustBackgoundColor( BYTE u8DispWin ,BGColorType u8Pattern);

void ATE_Set_Pattern(BYTE ColorPattern)
{
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    BYTE u8win;
#endif
    if(ColorPattern)
    {
    #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
        if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
        {
            for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                msAPIWinEnableFrameColor(u8win, TRUE);
        }
        else
    #endif
            msAPIWinEnableFrameColor(DISPLAY_WIN1, TRUE);
        //msWriteByte(SC0_02, msReadByte(SC0_02) | NIS_B);
    }
	switch(ColorPattern)
    {
        case 0:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPIWinEnableFrameColor(u8win, FALSE);
                }
                else
            #endif
                    msAPIWinEnableFrameColor(DISPLAY_WIN1, FALSE);
            }
            //msWriteByte(SC0_02, msReadByte(SC0_02) & (~NIS_B));
            break;
        case 1:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_RED);
                }
                else
            #endif
                    msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_RED);
            }
            break;
        case 2:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_GREEN);
                }
                else
            #endif
                    msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_GREEN);
            }
            break;
        case 3:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_BLUE);
                }
                else
            #endif
                    msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_BLUE);
            }
            break;
        case 4:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_WHITE);
                }
                else
            #endif
                    msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_WHITE);
            }
            break;
        case 5:
            {
            #if ENABLE_MULTI_WINDOW_SETTINGS_MENU
                if (USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
                {
                    for(u8win=0; u8win<DISPLAY_MAX_NUMS; u8win++)
                        msAPI_AdjustBackgoundColor(u8win, ePATTERN_COLOR_BLACK);
                }
                else
            #endif
                    msAPI_AdjustBackgoundColor(DISPLAY_WIN1, ePATTERN_COLOR_BLACK);
            }
            break;
    }
}
#if (ODM_NAME==ODM_WISTRON)

BYTE WistronAlignControl( void )
{
    BYTE u8TPValue;
    BYTE u8TempValue1;
    BYTE FCommand;
    BYTE i;

    if(u8CommandCode == GETPagedVCP||u8CommandCode == GETVCPFeature)
    {
        u8ValueH = 0x00;
        u8ValueL = 0xFF;    // one
        u8RetValueH = 0x00;

        if((u8CPCode == ADJ_Brightness) && (FACTORY_MODE_FLAG||u8CommandCode == GETPagedVCP))
        {
            u8RetValueL = USER_PREF_DEF_SUBBRIGHTNESS;
            u8ValueL = FACTORY_DEF_BRIGHTNESS_MAX;
            u8TPValue = 0x00;// Jeff add it in 2006年4月12日上午 11:44:32
        }

    #if defined (_WISTRON_ETO_)  //Poyen 20120229
        else if((u8CPCode == 0x11) && (FACTORY_MODE_FLAG||u8CommandCode == GETPagedVCP))
        {
            u8RetValueL = USER_PREF_FACTORY_BRIGHTNESS_TXT;
            u8ValueL = FACTORY_DEF_BRIGHTNESS_MAX;
            u8TPValue = 0x00;
        }
    #endif
        else  if(u8CPCode == ALL_Reset&&(u8DoAllResetBit))
        {
            u8RetValueH = 0x01;
            u8RetValueL = 0x01;
            u8TPValue = 0x01;
        }
        else if((u8CPCode == ADJ_Red_Gain) && (FACTORY_MODE_FLAG||u8CommandCode == GETPagedVCP))
        {
            u8ValueH = 0x00;
            u8ValueL = MAX_COLOR_VALUE;
            if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_USER)
                u8RetValueL =CURRENT_RED_COLOR;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_6500K)
                u8RetValueL =USER_PREF_RED_COLOR_6500K;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_9300K)
                u8RetValueL =USER_PREF_RED_COLOR_9300K;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_5700K)
                u8RetValueL =USER_PREF_RED_COLOR_5700K;
            u8TPValue = 0x00;
        }
        else if((u8CPCode == ADJ_Green_Gain) && (FACTORY_MODE_FLAG||u8CommandCode == GETPagedVCP))
        {
            u8ValueH = 0x00;
            u8ValueL = MAX_COLOR_VALUE;
            if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_USER)
                u8RetValueL =CURRENT_GREEN_COLOR;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_6500K)
                u8RetValueL =USER_PREF_GREEN_COLOR_6500K;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_9300K)
                u8RetValueL =USER_PREF_GREEN_COLOR_9300K;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_5700K)
                u8RetValueL =USER_PREF_GREEN_COLOR_5700K;
            u8TPValue = 0x00;
        }
        else if((u8CPCode == ADJ_Blue_Gain) && (FACTORY_MODE_FLAG||u8CommandCode == GETPagedVCP))
        {
            u8ValueH = 0x00;
            u8ValueL = MAX_COLOR_VALUE;
            if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_USER)
                u8RetValueL =CURRENT_BLUE_COLOR;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_6500K)
                u8RetValueL =USER_PREF_BLUE_COLOR_6500K;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_9300K)
                u8RetValueL =USER_PREF_BLUE_COLOR_9300K;
            else if(USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)== PCT_5700K)
                u8RetValueL =USER_PREF_BLUE_COLOR_5700K;
            u8TPValue = 0x00;
        }
        ///////////////////////////////////////////
        else if(u8CPCode == ADJ_OSD_LANGUAGE)
        {
            u8RetValueL=0;
            for(i=1;i<sizeof(OSD_Language_TB);i+=2)
            {
                if (USER_PREF_LANGUAGE==OSD_Language_TB[i])
                {
                    u8RetValueL=OSD_Language_TB[i-1];
                    break;
                }
            }
            u8TPValue = 0x00;// Jeff add it in 2006年4月12日上午 11:44:32
        }
    //////////////////////////////////////////
        else
            return 0x55;

        if(u8CommandCode == GETVCPFeature)
        {
            DDCBuffer[0] = 0x88;
            DDCBuffer[1] = 0x02;
            DDCBuffer[2] = 0x00;
            DDCBuffer[3] = u8CPCode;

            DDCBuffer[4] = u8TPValue;// Jeff add it in 2006年4月12日上午 11:44:32;
            DDCBuffer[5] = u8ValueH;
            DDCBuffer[6] = u8ValueL;
            DDCBuffer[7] = u8RetValueH;
            DDCBuffer[8] = u8RetValueL;

            if(u8CPCode == ALL_Reset)					//20080403
            {
                if(DDCBuffer[8]==1 && DDCBuffer[7]==1)	//20080403
                {
                    u8DoAllResetBit=0x00;            		    	//20080403
                }
            }
            return 8;
        }
        else
        {
            DDCBuffer[0] = 0x89;
            DDCBuffer[1] = 0xC7;
            DDCBuffer[2] = 0x00;
            DDCBuffer[3] = u8PageValue;
            DDCBuffer[4] = u8CPCode;

            DDCBuffer[5] = u8TPValue;
            DDCBuffer[6] = u8ValueH;
            DDCBuffer[7] = u8ValueL;
            DDCBuffer[8] = u8RetValueH;
            DDCBuffer[9] = u8RetValueL;
            u8DoAllResetBit=0;
            return 9;
        }
    }

	else if(u8CommandCode == SETVCPFeature ||(u8CommandCode == SETPagedVCP && u8PageValue == 0))
	{
        FCommand=FALSE;
        ///////////////////////////////////////////////////////////////////////////////////
        if( u8CommandCode == SETVCPFeature)
        {
            if(u8CPCode == ADJ_EDID_WP && u8PageValue == 0)
            {
            #if 0 //XXXXX
                if(( u8ValueL==0)&&( u8ValueH==0))
                    hw_ClrDDC_WP();
                else
                    hw_SetDDC_WP();
            #endif
                return 0;
            }
            else if(u8CPCode == ADJ_VOLUME && u8PageValue == 0)
            {
                u8TPValue = 0x00;// Jeff add it in 2006年4月12日上午 11:44:32
                return 0;
            }
            else if(u8CPCode == ADJ_AGINGMODE && u8PageValue == 0)
            {
                if(u8ValueL==1)
                    SET_BURNIN_MODE_FLAG();
                else
                    CLR_BURNIN_MODE_FLAG();
                SaveMonitorSetting();
                return 0;
            }

            else if(u8CPCode == ADJ_OSD_LANGUAGE && u8PageValue == 0)
            {
                for(i=0;i<sizeof(OSD_Language_TB);i+=2)
                {
                    if (u8ValueL==OSD_Language_TB[i])
                    {
                        if(OSD_Language_TB[i+1]>=LANG_INDEX_MAX)
                            USER_PREF_LANGUAGE=LANG_English;
                        else
                            USER_PREF_LANGUAGE=OSD_Language_TB[i+1];
                        break;
                    }
                }
                //----------------------------------------//Sky120614
                g_u8MenuPageIndex = MENU_ROOT;
                g_u8MenuItemIndex = 0;
                ExecuteKeyEvent(MIA_REDRAW_MENU);
                //----------------------------------------
                return 0;
            }
            else if(u8CPCode == ADJ_Screen_Pattern)
            {
                if( u8ValueL<6)
                    ATE_Set_Pattern(u8ValueL);
                u8TPValue=0;
            }
        }
///////////////////////////////////////////////////////////////////////////////////
        if((FACTORY_MODE_FLAG && u8CommandCode == SETVCPFeature) ||(u8CommandCode == SETPagedVCP && u8PageValue == 0))
        {
            if(u8CPCode == ALL_Reset)
            {
                if(u16Value != 0)
                {
                    USER_PREF_BACKLIGHT_TIME=0; // clear Backligh timer
                #if 0 //XXXXX
                    USER_PREF_LIFE_TIME_HOUR=0;
                    USER_PREF_LIFE_TIME_MINUTE=0;
                    NVRam_WriteTbl(BACKLIGHT_TIME_ADDR, (BYTE *) &stTimerRecord, TimerRecordSize);
                #endif
                    ResetAllSetting();

                #if !defined (_WISTRON_ETO_)  //Poyen 20120229
                    if(!UnsupportedModeFlag(SrcInputPortM/*//tmp*/))
                        SET_DDCCI_AUTO_ADJUST_FLAG();
                #endif
                }
                u8ValueL = 1;
                u8DoAllResetBit=0x01;//20080415
                FCommand=TRUE;
            }

            else if(u8CPCode == ADJ_Brightness)
            {
            #if defined (_WISTRON_ETO_)  //Poyen 20120229
                if (USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)!=PCT_6500K)//Chihlung120326
                {
                    USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)=PCT_6500K;
                    SetColorTempDDCCI();
                }
            #endif
                USER_PREF_DEF_SUBBRIGHTNESS=u8ValueL;
                AdjustFSubBrightness(MIA_NOTHING);
                u8TPValue=0;
                u8RetValueL=FACTORY_DEF_BRIGHTNESS_MAX;
                FCommand=TRUE;
            }

            #if defined (_WISTRON_ETO_) //Poyen 20120229
            else if(u8CPCode == 0x11 )
            {
                if (USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)!=CT_TEXT)//Chihlung120326
                {
                USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL)=CT_TEXT;
                SetColorTempDDCCI();
                }
                USER_PREF_FACTORY_BRIGHTNESS_TXT=u8ValueL;
                msAPI_AdjustBrightness(USER_PREF_FACTORY_BRIGHTNESS_TXT);
                //msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS_TXT);

                u8TPValue=0;
                u8RetValueL=FACTORY_DEF_BRIGHTNESS_MAX;
                FCommand=TRUE;
            }
            #endif

            else if(u8CPCode == ADJ_Red_Gain)
            {
                switch ( USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL))
                {
                case PCT_6500K:
                    USER_PREF_RED_COLOR_6500K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_6500K,USER_PREF_GREEN_COLOR_6500K,USER_PREF_BLUE_COLOR_6500K);
                break;
                case PCT_5700K:
                    USER_PREF_RED_COLOR_5700K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_5700K,USER_PREF_GREEN_COLOR_5700K,USER_PREF_BLUE_COLOR_5700K);
                break;
                case PCT_9300K:
                    USER_PREF_RED_COLOR_9300K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_9300K,USER_PREF_GREEN_COLOR_9300K,USER_PREF_BLUE_COLOR_9300K);
                break;
                case PCT_USER:
                        CURRENT_RED_COLOR= u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
                break;
                }

                u8RetValueL=MAX_COLOR_VALUE;
                u8TPValue=0;
                FCommand=TRUE;
            }
            else if(u8CPCode == ADJ_Green_Gain)
            {
                switch ( USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL))
                {
                    case PCT_6500K:
                        USER_PREF_GREEN_COLOR_6500K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_6500K,USER_PREF_GREEN_COLOR_6500K,USER_PREF_BLUE_COLOR_6500K);
                        break;
                    case PCT_5700K:
                        USER_PREF_GREEN_COLOR_5700K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_5700K,USER_PREF_GREEN_COLOR_5700K,USER_PREF_BLUE_COLOR_5700K);
                        break;
                    case PCT_9300K:
                        USER_PREF_GREEN_COLOR_9300K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_9300K,USER_PREF_GREEN_COLOR_9300K,USER_PREF_BLUE_COLOR_9300K);
                        break;
                    case PCT_USER:
                        CURRENT_GREEN_COLOR= u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
                        break;
                }

                u8RetValueL=MAX_COLOR_VALUE;
                u8TPValue=0;
                FCommand=TRUE;
            }
            else if(u8CPCode == ADJ_Blue_Gain)
            {
                switch ( USER_PREF_COLOR_TEMP(USER_PREF_WIN_SEL))
                {
                    case PCT_6500K:
                        USER_PREF_BLUE_COLOR_6500K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_6500K,USER_PREF_GREEN_COLOR_6500K,USER_PREF_BLUE_COLOR_6500K);
                        break;
                    case PCT_5700K:
                        USER_PREF_BLUE_COLOR_5700K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_5700K,USER_PREF_GREEN_COLOR_5700K,USER_PREF_BLUE_COLOR_5700K);
                        break;
                    case PCT_9300K:
                        USER_PREF_BLUE_COLOR_9300K = u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,USER_PREF_RED_COLOR_9300K,USER_PREF_GREEN_COLOR_9300K,USER_PREF_BLUE_COLOR_9300K);
                        break;
                    case PCT_USER:
                        CURRENT_BLUE_COLOR= u8ValueL;
                        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL,CURRENT_RED_COLOR,CURRENT_GREEN_COLOR,CURRENT_BLUE_COLOR);
                        break;
                }
                u8RetValueL=MAX_COLOR_VALUE;
                u8TPValue=0;
                FCommand=TRUE;
            }
        }

        if(FCommand)
        {
            if( u8CommandCode == SETPagedVCP)
            {
    			DDCBuffer[0] = 0x89;
    			DDCBuffer[1] = 0xC7;
    			DDCBuffer[2] = 0x00; // Result Code
    			DDCBuffer[3] = u8PageValue;
    			DDCBuffer[4] = u8CPCode;
    			DDCBuffer[5] = u8TPValue;
    			DDCBuffer[6] = 0x00;
    			DDCBuffer[7] = u8RetValueL;
    			//DDCBuffer[7] = 100;//u8RetValueL;
    			DDCBuffer[8] = u8ValueH;
    			DDCBuffer[9] = u8ValueL;
    			return 9;
            }
    		else
    	        return 0;
        }
        else
            return 0x55;
	}

    else if(u8CommandCode == FactoryCommand)
    {
        //Set_DDC_NMVAddrFlag();
        if (DDCBuffer[2] == ModeChangeRequest)
        {
            if (DDCBuffer[3] == 0x00)
            {
                //HW_SET_DDCWP();	//20080509
                CLR_FACTORY_MODE_FLAG();
            #if defined (_WISTRON_ETO_) //Poyen 20120229
                CLR_BURNIN_MODE_FLAG();
                if (!SyncLossState(SrcInputPortM/*//tmp*/))
                {
                    hw_SetGreenLed();
                    hw_ClrAmberLed();
                }
            #endif
            }
            else if (DDCBuffer[3] == 0x03)
            {
                SET_FACTORY_MODE_FLAG();

                USER_PREF_BRIGHTNESS = DEF_BRIGHTNESS;
                msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);

                #if defined (_WISTRON_ETO_) //Poyen 20120229
                    CURRENT_CONTRAST = DEF_CONTRAST;
                    msAPI_AdjustContrast(USER_PREF_WIN_SEL, CURRENT_CONTRAST);
                #endif
            }

            //NVRam_WriteWord(NVR_MONITOR_ADDR(u16MonitorFlag), MONITOR_FLAGS);

            //  NVRam_WriteByte(nvrDigitalMonitorAddr(MonitorFlag), MonitorFlags);
            DDCBuffer[0]=0x83;
            DDCBuffer[1]=0xC1;
            DDCBuffer[2]=0x11;
            if (FACTORY_MODE_FLAG)
                DDCBuffer[3]=0x03;
            else
                DDCBuffer[3]=0x00;
            return 3;
        }

        else if (DDCBuffer[2] == EEPROMRead)
        {
            u8ValueL=DDCBuffer[3]; // Read Length
            u16Value=((WORD) DDCBuffer[4] << 8) | ((WORD) DDCBuffer[5]);
            if (u16Value > 0x7FF)
                u8ValueL=0;
            if ((u16Value + u8ValueL) > 0x7FF)
                u8ValueL=0x800 - u16Value;
            NVRam_ReadTbl(u16Value, (BYTE *)&DDCBuffer[3], u8ValueL);
            DDCBuffer[0]=0x80 + 2 + u8ValueL;
            DDCBuffer[1]=0xC1;
            DDCBuffer[2]=0x09;
            return DDCBuffer[0];
        }
        else if (DDCBuffer[2] == EEPROMWrite)
        {
            if (!FACTORY_MODE_FLAG)
                return 0;
            u8ValueL=DDCBuffer[3]; // Write Length
            u16Value=((WORD) DDCBuffer[4] << 8) | ((WORD) DDCBuffer[5]);
            if (u16Value > 0x7FF)
                u8ValueL=0;
            if ((u16Value + u8ValueL) > 0x7FF)
                u8ValueL=0x800 - u16Value;
            NVRam_WriteTbl(u16Value, (BYTE *)&DDCBuffer[6], u8ValueL);
            DDCBuffer[0]=0x83;
            DDCBuffer[1]=0xC1;
            DDCBuffer[2]=0x02;
            DDCBuffer[3]=0x01;
            return DDCBuffer[0];
        }
    //////////////////////////////////////////////////////////////////////
        else if(DDCBuffer[2] == ASSET_READ_Wistron)
        {
            u8TempValue1=DDCBuffer[4];
        #if 0 //XXXXX
            if(DDCBuffer[3]== ASSET_Serial_Number)
            {

                NVRam_ReadTbl(SERIAL_NUMBER_ADDR, (BYTE *)&DDCBuffer[3], u8TempValue1);
            }
            else if(DDCBuffer[3] == ASSET_Backlight_Hours)
            {
                NVRam_ReadTbl(BACKLIGHT_TIME_ADDR, (BYTE *)&DDCBuffer[3], u8TempValue1);
            }
            else
        #endif
            if(DDCBuffer[3] ==ASSET_Firmware_Version)
            {
                for(i=0;i<6;i++)
                    DDCBuffer[3+i] = tFWVersionString[i];
                u8TempValue1=6;
            }
            else if(DDCBuffer[3] ==ASSET_Firmware_PIN)
            {
                for(i=0;i<12;i++)
                    DDCBuffer[3+i] = tFirmwarePinString[i];
                u8TempValue1=12;
            }
            //NVRam_ReadTbl(u8TempValue+ASSETSTARTADDR, &tDDCBuffer[3], u8TempValue1);
            DDCBuffer[0] = 2 + u8TempValue1 + 0x80;
            DDCBuffer[1] = 0xC1;
            DDCBuffer[2] = ASSET_READ_Wistron;
            //Set_DDC_NMVAssetFlag();
            return DDCBuffer[0]&~0x80;
        }
        else if(DDCBuffer[2] == ASSET_WRITE_Wistron)
        {
        #if 0 //XXXXX
            u8TempValue1 = (DDCBuffer[0]&0x7F)-3;// Read Length
            if(DDCBuffer[3]== ASSET_Serial_Number)
            {
                NVRam_WriteTbl(SERIAL_NUMBER_ADDR,(BYTE *)&DDCBuffer[4],u8TempValue1);
            }
            else if(DDCBuffer[3] == ASSET_Backlight_Hours)
            {
                NVRam_WriteTbl(BACKLIGHT_TIME_ADDR,(BYTE *)&DDCBuffer[4],u8TempValue1);
            }
        #endif
            return 0;
        }
    /////////////////////////////////////////////////////////////////////
    }
    else if(u8CommandCode == GETVCPPage)
    {
        DDCBuffer[0]=0x83;
        DDCBuffer[1]=0xC5;
        DDCBuffer[2]=0;
        DDCBuffer[3]=3;
        return 3;
    }
    else if (u8CommandCode == SaveCurrentSetting)	//Ailly 20080409 22T103
    {
        if (FACTORY_MODE_FLAG)  //JL 20070617
            SaveFactorySetting();
        SaveUserPref();
        return 0;
    }
	return 0x55;  //Not factory command control
}
#endif


