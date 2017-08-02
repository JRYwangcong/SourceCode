
#define _APPWINDOW_C_
#include <string.h>
#include "Global.h"
#include "appWindow.h"
#include "appInput.h"
#include "msScalerAP.h"
#include "menufunc.h"
#include "Menu.h"
#include "UserPrefAPI.h"
#include "Detect.h"
#include "AutoFunc.h"
#include "msPQ.h"
#include "mapi_combo.h"
#include "MenuFont.h"
#include "BinInfo.h"
#include "InfoBlock.h"
#include "appPanel.h"
#include "MsDLC_Setting.h"

#define FOR_DEMO    0

#define APPWINDOW_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && APPWINDOW_DEBUG
#define APPWINDOW_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define APPWINDOW_PRINT(format, ...)
#endif

extern void mapi_tmds_ForceColorFormat(BYTE enInputPort, BOOL bFlag, EN_COLOR_FORMAT_TYPE enColor);

BYTE g_bAPPWindow_FlyWin = 0;

//
ST_WINDOW_INFO tbMultiWin_Type[eMULTIWIN_MAX][DISPLAY_MAX_NUMS] =
{
#if Enable_Panel_Rotate_180
    //eMULTIWIN_OFF
    {
    {0, 0, PANEL_WIDTH, PANEL_HEIGHT},      //DISPLAY_WIN1
    {0, 0, 0, 0},                           //DISPLAY_WIN2
    {0, 0, 0, 0},                           //DISPLAY_WIN3
    {0, 0, 0, 0},                           //DISPLAY_WIN4
    },
    //eMULTIWIN_2_PIP
    {
    {0, 0, PANEL_WIDTH, PANEL_HEIGHT},      //DISPLAY_WIN1
    {0, 0, PANEL_WIDTH, PANEL_HEIGHT},      //DISPLAY_WIN2
    {0, 0, 0, 0},                           //DISPLAY_WIN3
    {0, 0, 0, 0},                           //DISPLAY_WIN4
    },
    //eMULTIWIN_2_POP1
    {
    {PANEL_WIDTH/2, 0, PANEL_WIDTH/2, PANEL_HEIGHT},    //DISPLAY_WIN1
    {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT},                //DISPLAY_WIN2
    {0, 0, 0, 0},                                       //DISPLAY_WIN3
    {0, 0, 0, 0},                                       //DISPLAY_WIN4
    },
    /*
    //eMULTIWIN_2_POP2
    {
    {0, 0, PANEL_WIDTH, PANEL_HEIGHT/2},                    //DISPLAY_WIN1
    {0, PANEL_HEIGHT/2, PANEL_WIDTH, PANEL_HEIGHT/2},       //DISPLAY_WIN2
    {0, 0, 0, 0},                                           //DISPLAY_WIN3
    {0, 0, 0, 0},                                           //DISPLAY_WIN4
    },	
*/

    //eMULTIWIN_3
    {
    {PANEL_WIDTH/2, 0, PANEL_WIDTH/2, PANEL_HEIGHT},                            //DISPLAY_WIN1
    {0, PANEL_HEIGHT/2, PANEL_WIDTH/2, PANEL_HEIGHT/2}, //DISPLAY_WIN2
    {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2},              //DISPLAY_WIN3
    {0, 0, 0, 0},                           //DISPLAY_WIN4
    },
    //eMULTIWIN_4
    {
    {PANEL_WIDTH/2, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2},                                  //DISPLAY_WIN1
    {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2},                      //DISPLAY_WIN2
    {PANEL_WIDTH/2, PANEL_WIDTH/2, PANEL_WIDTH/2, PANEL_HEIGHT/2},                     //DISPLAY_WIN3
    {0, PANEL_HEIGHT/2, PANEL_HEIGHT/2, PANEL_HEIGHT/2},         //DISPLAY_WIN4
    },


#else
    //eMULTIWIN_OFF
    {
    {0, 0, PANEL_WIDTH, PANEL_HEIGHT},      //DISPLAY_WIN1
    {0, 0, 0, 0},                           //DISPLAY_WIN2
    {0, 0, 0, 0},                           //DISPLAY_WIN3
    {0, 0, 0, 0},                           //DISPLAY_WIN4
    },
    //eMULTIWIN_2_PIP
    {
    {0, 0, PANEL_WIDTH, PANEL_HEIGHT},      //DISPLAY_WIN1
    {0, 0, PANEL_WIDTH, PANEL_HEIGHT},      //DISPLAY_WIN2
    {0, 0, 0, 0},                           //DISPLAY_WIN3
    {0, 0, 0, 0},                           //DISPLAY_WIN4
    },
    //eMULTIWIN_2_POP1
    {
    {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT},                //DISPLAY_WIN1
    {PANEL_WIDTH/2, 0, PANEL_WIDTH/2, PANEL_HEIGHT},    //DISPLAY_WIN2
    {0, 0, 0, 0},                                       //DISPLAY_WIN3
    {0, 0, 0, 0},                                       //DISPLAY_WIN4
    },
    /*
    //eMULTIWIN_2_POP2
    {
    {0, 0, PANEL_WIDTH, PANEL_HEIGHT/2},                    //DISPLAY_WIN1
    {0, PANEL_HEIGHT/2, PANEL_WIDTH, PANEL_HEIGHT/2},       //DISPLAY_WIN2
    {0, 0, 0, 0},                                           //DISPLAY_WIN3
    {0, 0, 0, 0},                                           //DISPLAY_WIN4
    },	
*/

    //eMULTIWIN_3
    {
    {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT},                            //DISPLAY_WIN1
    {PANEL_WIDTH/2, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2},              //DISPLAY_WIN2
    {PANEL_WIDTH/2, PANEL_HEIGHT/2, PANEL_WIDTH/2, PANEL_HEIGHT/2}, //DISPLAY_WIN3
    {0, 0, 0, 0},                           //DISPLAY_WIN4
    },
    //eMULTIWIN_4
    {
    {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2},                                  //DISPLAY_WIN1
    {PANEL_WIDTH/2, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2},                      //DISPLAY_WIN2
    {0, PANEL_HEIGHT/2, PANEL_WIDTH/2, PANEL_HEIGHT/2},                     //DISPLAY_WIN3
    {PANEL_WIDTH/2, PANEL_HEIGHT/2, PANEL_WIDTH/2, PANEL_HEIGHT/2},         //DISPLAY_WIN4
    },
#endif
};

////////////////////////////////////////////////
//      Local Function
////////////////////////////////////////////////
WORD GetPipWindowWidth(BYTE u8PipSize)
{
    WORD u16PipWidth=0;

    switch(u8PipSize)
    {
        case ePIPSIZE_SMALL:
            u16PipWidth = (PANEL_WIDTH*35/100);
            break;

        case ePIPSIZE_MIDDLE:
            u16PipWidth = (PANEL_WIDTH*40/100);
            break;

        case ePIPSIZE_LARGE:
            u16PipWidth = (PANEL_WIDTH*45/100);
            break;

        case ePIPSIZE_HUGE:
            u16PipWidth = (PANEL_WIDTH*50/100);
            break;
        case ePIPSIZE_MAX:
            break;
    }
    return u16PipWidth;
}
WORD GetPipWindowHeight(BYTE u8PipSize)
{
    WORD u16PipHeight=0;

    switch(u8PipSize)
    {
        case ePIPSIZE_SMALL:
            u16PipHeight = (PANEL_HEIGHT*35/100);
            break;

        case ePIPSIZE_MIDDLE:
            u16PipHeight = (PANEL_HEIGHT*40/100);
            break;

        case ePIPSIZE_LARGE:
            u16PipHeight = (PANEL_HEIGHT*45/100);
            break;

        case ePIPSIZE_HUGE:
            u16PipHeight = (PANEL_HEIGHT*50/100);
            break;
        case ePIPSIZE_MAX:
            break;
    }
    return u16PipHeight;
}
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
static void SetPipSize(ST_WINDOW_INFO* pPipWin, ePIPSIZE_Type ePipType)
{
    switch(ePipType)
    {
        case ePIPSIZE_SMALL:
            pPipWin->x = USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2);
            pPipWin->y = USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2);
            pPipWin->width = GetPipWindowWidth(ePIPSIZE_SMALL);
            pPipWin->height = GetPipWindowHeight(ePIPSIZE_SMALL);
            break;

        case ePIPSIZE_MIDDLE:
            pPipWin->x = USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2);
            pPipWin->y = USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2);
            pPipWin->width = GetPipWindowWidth(ePIPSIZE_MIDDLE);
            pPipWin->height = GetPipWindowHeight(ePIPSIZE_MIDDLE);
            break;

        case ePIPSIZE_LARGE:
            pPipWin->x = USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2);
            pPipWin->y = USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2);
            pPipWin->width = GetPipWindowWidth(ePIPSIZE_LARGE);
            pPipWin->height = GetPipWindowHeight(ePIPSIZE_LARGE);
            break;

        case ePIPSIZE_HUGE:
            pPipWin->x = USER_PREF_MULTI_WIN_H_POS(DISPLAY_WIN2);
            pPipWin->y = USER_PREF_MULTI_WIN_V_POS(DISPLAY_WIN2);
            pPipWin->width = GetPipWindowWidth(ePIPSIZE_HUGE);
            pPipWin->height = GetPipWindowHeight(ePIPSIZE_HUGE);
            break;

        case ePIPSIZE_MAX:
            break;
    }

}
#endif

static void SetOverScan(ST_WINDOW_INFO* psCaptueWin, BYTE u8HOverScan, BYTE u8VOverScan)
{
    DWORD u326Width = psCaptueWin->width;
    DWORD u32Height = psCaptueWin->height;
    WORD u16HStart = 0;
    WORD u16VStart = 0;

    if(u8HOverScan > 100)
        u8HOverScan = 100;

    if(u8HOverScan != 100)
    {
        //caculate horizontal overscan
        u326Width = (DWORD)u326Width * u8HOverScan / 100;
        u326Width &= 0xFFFE;
        u16HStart = (psCaptueWin->width - u326Width)/2;
        psCaptueWin->x += u16HStart;
        psCaptueWin->width = u326Width;
    }

    if(u8VOverScan > 100)
        u8VOverScan = 100;

    if(u8HOverScan != 100)
    {
        //caculate vertical overscan
        u32Height = (DWORD)u32Height * u8VOverScan / 100;
        u32Height &= 0xFFFE;
        u16VStart = (psCaptueWin->height - u32Height)/2;
        psCaptueWin->y += u16VStart;
        psCaptueWin->height = u32Height;
    }
}

static void SetAspect(ST_WINDOW_INFO* psCaptueWin, ST_WINDOW_INFO* psDispWin, eASPECT_TYPE eAspect)
{
    DWORD u32Width = psDispWin->width;
    DWORD u32Height = psDispWin->height;
    WORD u16HStart = 0;
    WORD u16VStart = 0;

    switch(eAspect)
    {
        case eASPECT_NATIVE:
       #if 0//(defined(LianHeChuangXin_Project))
            if((psCaptueWin->width*10/psCaptueWin->height)<(u32Width*10/u32Height))	
            	{
                u32Width = u32Height *4/3;
            	}
	#else			
            if ((u32Width * psCaptueWin->height ) > (u32Height * psCaptueWin->width ))
            {
                u32Width = u32Height * psCaptueWin->width/psCaptueWin->height ;
            }
            else
            {
                u32Height = u32Width *psCaptueWin->height/psCaptueWin->width;
            }
	#endif		
            u32Width &= 0xFFFE;
            u32Height &= 0xFFFE;
            u16HStart =  (psDispWin->width - u32Width)/2;
            u16VStart =  (psDispWin->height - u32Height)/2;
            break;

        case eASPECT_4_3:
            if ((u32Width*3) > (u32Height*4))
            {
                u32Width = u32Height *4/3;
            }
            else
            {
                u32Height = u32Width *3/4;
            }
            u32Width &= 0xFFFE;
            u32Height &= 0xFFFE;
            u16HStart =  (psDispWin->width - u32Width)/2;
            u16VStart =  (psDispWin->height - u32Height)/2;
            break;

        case eASPECT_5_4:
            if ((u32Width*4) > (u32Height*5))
            {
                u32Width = u32Height *5/4;
            }
            else
            {
                u32Height = u32Width *4/5;
            }
            u32Width &= 0xFFFE;
            u32Height &= 0xFFFE;
            u16HStart =  (psDispWin->width - u32Width)/2;
            u16VStart =  (psDispWin->height - u32Height)/2;
            break;

        case eASPECT_16_9:
            if ((u32Width*9) > (u32Height*16))
            {
                u32Width = u32Height *16/9;
            }
            else
            {
                u32Height = u32Width *9/16;
            }
            u32Width &= 0xFFFE;
            u32Height &= 0xFFFE;
            u16HStart =  (psDispWin->width - u32Width)/2;
            u16VStart =  (psDispWin->height - u32Height)/2;
            break;

        case eASPECT_1_1:
     #if EDID_3840x2160			
	   if((u32Width<psCaptueWin->width)||(u32Height<psCaptueWin->height))
     #else
	   if((u32Width<psCaptueWin->width)&&(u32Height<psCaptueWin->height))
     #endif	 
	   {
             ;
	   }
	   else
	   {
            u32Width = psCaptueWin->width;
            u32Height = psCaptueWin->height;
	   }
            u16HStart =  (psDispWin->width - u32Width)/2;
            u16VStart =  (psDispWin->height - u32Height)/2;
            break;
			
        case eASPECT_FULL:
        case eASPECT_MAX:
            break;
    }

    psDispWin->width = u32Width;
    psDispWin->height = u32Height;
    psDispWin->x += u16HStart;
    psDispWin->y += u16VStart;
}

static void SetWinColor(BYTE u8DispWin)
{
    EN_WINDOW_COLOR  eWinColor = eWIN_COLOR_RGB;
    EN_INPUT_COLOR   eInputColor = eINPUT_COLOR_RGB;
    EN_COLOR_RANGE  eInputRange = eCOLOR_RANGE_FULL;
    EN_COLOR_DEPTH_TYPE eColorDepth = COMBO_COLOR_DEPTH_8BIT;
    ST_COMBO_COLOR_FORMAT cf = msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[MapWin2Port(u8DispWin)].eIPMux));
    BYTE u8PortIdx = MapWin2Port(u8DispWin);
    BYTE u8MemFormat;

    DWORD   u32IP1BaseAddr = 0;
    WORD    u16AssignData = 0;
    //BYTE    u8ScalerIdx = 0;
    WORD    u16TempData=0;

    #if (CHIP_ID != MST9U4)
    EN_TMDS_PIXEL_REPETITION eRepetition = COMBO_TMDS_N0_PIX_REP;
    #endif


    //EN_TMDS_PIXEL_REPETITION u8Repetition = 0;

    eInputColor = msAPISrcGetPktColorSpace(u8DispWin);
    eInputRange = msAPISrcGetPktColorRange(u8DispWin);
    eColorDepth = msAPI_combo_IPGetColorDepth((EN_COMBO_IP_SELECT)(g_InputPort[u8PortIdx].eIPMux));

    msAPIWinGetMemFormat(u8DispWin, &u8MemFormat);

    // Set src input color space
    msAPISrcSetCurColorSpace(u8DispWin, eInputColor);

    APPWINDOW_PRINT("[appWindow] InputColor Original= %d\n", eInputColor);
    APPWINDOW_PRINT("[appWindow] InputColor eInputRange= %d\n", eInputRange);
//    APPWINDOW_PRINT("[appWindow] InputColor ddddd= %d\n", msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[u8PortIdx].eIPMux)).ucColorRange);

#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
    // Set src input color range
    msAPISrcSetCurColorRange(u8DispWin, eInputRange);
#endif

    switch (USER_PREF_INPUT_COLOR_FORMAT(u8DispWin))
    {
        case INPUT_COLOR_RGB:
            eInputColor = eINPUT_COLOR_RGB;
            u16AssignData = (BIT15|BIT2|BIT1|BIT0);
            APPWINDOW_PRINT("\n[appWindow] User set color to force RGB.\n");
            break;
        case INPUT_COLOR_YUV422:
            eInputColor = eINPUT_COLOR_422;
            u16AssignData = (BIT15|BIT2|BIT1);
            APPWINDOW_PRINT("\n[appWindow] User set color to force YUV 422.\n");
            break;
        case INPUT_COLOR_YUV444:
            eInputColor = eINPUT_COLOR_444;
            u16AssignData = (BIT15|BIT2|BIT1|BIT0);
            APPWINDOW_PRINT("\n[appWindow] User set color to force YUV 444.\n");
            break;
        case INPUT_COLOR_YUV420:
            eInputColor = eINPUT_COLOR_420;
            u16AssignData = (BIT15|BIT2|BIT1|BIT0);
            APPWINDOW_PRINT("\n[appWindow] User set color to force YUV 420.\n");
            break;

        case INPUT_COLOR_FORMAT_AUTO:
            APPWINDOW_PRINT("\n[appWindow] User set color to AUTO.\n");
            eInputColor = eInputColor;
            break;

        default:
            eInputColor = eInputColor;
            break;
    }

    //u32IP1BaseAddr = SCIP1RegBase(u8ScalerIdx);

    if(INPUT_IS_HDMI(MapWin2Port(u8DispWin)) || INPUT_IS_HDMI_2_0(MapWin2Port(u8DispWin)))
    {
        if(USER_PREF_INPUT_COLOR_FORMAT(u8DispWin) == INPUT_COLOR_YUV420)
        {
            mapi_tmds_ForceColorFormat(u8PortIdx, TRUE, COMBO_COLOR_FORMAT_YUV_420);
            APPWINDOW_PRINT("\n[appWindow] TMDS Force color format to YUV 420. (Port: %d)\n", u8PortIdx);
        }
        else
        {
            mapi_tmds_ForceColorFormat(u8PortIdx, FALSE, COMBO_COLOR_FORMAT_YUV_420);
            APPWINDOW_PRINT("\n[appWindow] Disable TMDS force color format settings. (Port: %d)\n", u8PortIdx);
        }

        if((USER_PREF_INPUT_COLOR_FORMAT(u8DispWin) == INPUT_COLOR_FORMAT_AUTO))// || (USER_PREF_INPUT_COLOR_FORMAT(u8DispWin) == INPUT_COLOR_YUV420))
        {
            msAPISrcForceInputFormat(u8DispWin, DISABLE, 0x00);
         /*
            u16TempData = msRead2Byte(u32IP1BaseAddr + SC01_04);
            u16TempData &= 0x7FFF;
            msWrite2Byte(u32IP1BaseAddr + SC01_04, u16TempData);
         */   
        }
        else
        {
            msAPISrcForceInputFormat(u8DispWin, ENABLE, u16AssignData);
            //msWrite2ByteMask(u32IP1BaseAddr + SC01_04, u16AssignData, (BIT15|BIT2|BIT1|BIT0));
        }
    }
/*	
    else if(INPUT_IS_DISPLAYPORT(MapWin2Port(u8DispWin)))	
    {
        //printMsg("vvvvvvvvvvvvvvvvvvvvv");
        u16TempData = msRead2Byte(u32IP1BaseAddr + SC01_04);
        //u16TempData &= 0x7FFF;//Mqsh for DP YUV422 Error 20170327
        msWrite2Byte(u32IP1BaseAddr + SC01_04, u16TempData);
    }
    else
    {
        u16TempData = msRead2Byte(u32IP1BaseAddr + SC01_04);
        u16TempData &= 0x7FFF;
        msWrite2Byte(u32IP1BaseAddr + SC01_04, u16TempData);
    }
*/
    //Pixel Repetition settings.
    if(INPUT_IS_HDMI(MapWin2Port(u8DispWin)) || INPUT_IS_HDMI_2_0(MapWin2Port(u8DispWin)))
    {
        eRepetition = msAPI_combo_IPGetPixelRepetitionInfo((EN_COMBO_IP_SELECT)g_InputPort[u8PortIdx].eIPMux);
        msAPISrcSetPixelRepetition(u8DispWin, (BYTE)eRepetition);

	/*
        u8Repetition = msAPI_combo_IPGetPixelRepetitionInfo(g_InputPort[u8PortIdx].eIPMux);
        msWriteByteMask(u32IP1BaseAddr + SC01_1A, u8Repetition, (BIT3|BIT2|BIT1|BIT0));
       */ 
    }
    else
    {
        msAPISrcSetPixelRepetition(u8DispWin, 0);
        //msWriteByteMask(u32IP1BaseAddr + SC01_1A, 0, (BIT3|BIT2|BIT1|BIT0));
    }

    if(eInputColor == eINPUT_COLOR_RGB)
    {
        //user select  picture mode
    #if ENABLE_SUPER_RESOLUTION
        if (UserPrefNoiseReduction != SUPER_RESOLUTION_OFF)
        {
            eWinColor = eWIN_COLOR_YUV;
        }
    #endif
    }
    else
    {
        eWinColor = eWIN_COLOR_YUV;
    }

    if( IsWindowDI(u8DispWin) || (u8MemFormat == MEM_FMT_422))
    {
        eWinColor = eWIN_COLOR_YUV;
    }

    msDrvHistogramYSetRGBMode(u8DispWin, ((eWinColor == eWIN_COLOR_RGB)? TRUE:FALSE));

    APPWINDOW_PRINT("[appWindow] InputColor = %d\n", eInputColor);
    APPWINDOW_PRINT("[appWindow] InputColorDepth = %d\n", eColorDepth);
    APPWINDOW_PRINT("[appWindow] (g_InputPort[u8PortIdx].eIPMux) = %d\n", (g_InputPort[u8PortIdx].eIPMux));

    APPWINDOW_PRINT("[appWindow] InputRange = %d\n", eInputRange);
    APPWINDOW_PRINT("[appWindow] WinColor = %d\n", eWinColor);
    if(INPUT_IS_VD(u8PortIdx))
    {
        msAPIWinSetColorSpaceInput(u8DispWin, eINPUT_COLOR_YPBPR, eCOLOR_RANGE_LIMIT);
    }
    else
    {
        switch(CURRENT_INPUT_COLOR_RANGE)
        {
            case OSD_INPUT_COLOR_RANGE_AUTO:
                eInputRange = eInputRange;
                break;
            case OSD_INPUT_COLOR_RANGE_FULL:
                eInputRange = eCOLOR_RANGE_FULL;
                break;
            case OSD_INPUT_COLOR_RANGE_LIMITED:
                eInputRange = eCOLOR_RANGE_LIMIT;
                break;
            default:
                eInputRange = eCOLOR_RANGE_FULL;
                break;
        }
  #if ((ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)&&(defined(LianHeChuangXin_Project)))	
	if(INPUT_IS_VGA(u8PortIdx)||INPUT_IS_DVI(u8PortIdx))
	 eInputRange = eCOLOR_RANGE_FULL;
  #endif	
	
        msAPIWinSetColorSpaceInput(u8DispWin, eInputColor, eInputRange);
    }

    msAPIWinSetColorSpaceWindow(u8DispWin, eWinColor);
}

static void CheckValidTiming(InputTimingType sInputTiming, ST_WINDOW_INFO sAutoWindow, BYTE *pbSupportTiming)
{
    WORD u16HFreq, u16VFreq, u16HSize, u16VSize, u8SyncPolarity;

    u16HFreq = HFREQ(sInputTiming.HPeiod);
    u16VFreq = VFREQ(u16HFreq, sInputTiming.VTotal);
    u16HSize = sAutoWindow.width;
    u16VSize = sAutoWindow.height;
    u8SyncPolarity = SyncPolarity(sInputTiming.fFlags);

    *pbSupportTiming = TRUE;
}

#if 0
static void SetPQMode(BYTE u8WinIndex, InputTimingType sInputTiming, ST_WINDOW_INFO *psAutoWin, ST_WIN_PQ_MODE *psPQMode)
{
    BYTE b480 = 0, b576 = 0, b720 = 0, b1080 = 0, bInterlace, b50Hz;
    WORD u16HFreq = HFREQ(sInputTiming.HPeiod);
    WORD u16VFreq = VFREQ(u16HFreq, sInputTiming.VTotal);
    EN_WINDOW_COLOR  eWinColor = eWIN_COLOR_RGB;

    msAPIWinGetColorSpaceWindow(u8WinIndex, &eWinColor);

    bInterlace = ((sInputTiming.fFlags & bInterlaceMode) ? 1 : 0);

    if( abs((int)u16VFreq - 50) < APP_INPUT_TIMING_TOL_VFREQ )
        b50Hz = 1;
    else
        b50Hz = 0;

    if( (abs((int)720 - psAutoWin->width) < APP_INPUT_TIMING_TOL_DE)
     || (abs((int)1440 - psAutoWin->width) < APP_INPUT_TIMING_TOL_DE) )
    {
        if( abs((int)480 - psAutoWin->height) < APP_INPUT_TIMING_TOL_DE )
            b480 = 1;
        else if( abs((int)576 - psAutoWin->height) < APP_INPUT_TIMING_TOL_DE )
            b576 = 1;
    }

    if( (abs((int)1280 - psAutoWin->width) < APP_INPUT_TIMING_TOL_DE)
     && (abs((int)720 - psAutoWin->height) < APP_INPUT_TIMING_TOL_DE) )
        b720 = 1;


    if( (abs((int)1920 - psAutoWin->width) < APP_INPUT_TIMING_TOL_DE)
     && (abs((int)1080 - psAutoWin->height) < APP_INPUT_TIMING_TOL_DE) )
        b1080 = 1;

    if( IsWindowRot(u8WinIndex) )
    {
        if( IsWindowRot90270(u8WinIndex) )
            psPQMode->u8MAMode = SC_MA_MODE_ROT;
        else
            psPQMode->u8MAMode = SC_MA_MODE_OFF;
        psPQMode->u8EOMode = SC_EO_MODE_OFF;
        psPQMode->u8MCMode = SC_MC_MODE_OFF;
        psPQMode->u8FilmMode = SC_FILM_MODE_OFF;
    }
    else if( b1080 )
    {
        if( bInterlace )
        {
#if 1
            psPQMode->u8MAMode = SC_MA_MODE_5;
#else
            psPQMode->u8MAMode = SC_MA_MODE_6;
#endif
            psPQMode->u8EOMode = SC_EO_MODE_0;
            psPQMode->u8MCMode = SC_MC_MODE_2;
            if( b50Hz )
                psPQMode->u8FilmMode = SC_FILM_MODE_4;
            else
                psPQMode->u8FilmMode = SC_FILM_MODE_5;
        }
        else
        {
            if(eWinColor == eWIN_COLOR_RGB)
            {
#if 1
                psPQMode->u8MAMode = SC_MA_MODE_OFF;
                psPQMode->u8MCMode = SC_MC_MODE_OFF;
#else
                psPQMode->u8MAMode = SC_MA_MODE_0;
                psPQMode->u8MCMode = SC_MC_MODE_OFF;
#endif
            }
            else
            {
#if 1
                psPQMode->u8MAMode = SC_MA_MODE_OFF;
                psPQMode->u8MCMode = SC_MC_MODE_OFF;
#else
                psPQMode->u8MAMode = SC_MA_MODE_2;
                psPQMode->u8MCMode = SC_MC_MODE_3;
#endif
            }
            psPQMode->u8EOMode = SC_EO_MODE_OFF;
            psPQMode->u8FilmMode = SC_FILM_MODE_OFF;
        }
    }
    else if( b720 )
    {
#if 1
        psPQMode->u8MAMode = SC_MA_MODE_OFF;
        psPQMode->u8MCMode = SC_MC_MODE_OFF;
#else
        psPQMode->u8MAMode = SC_MA_MODE_2;
        psPQMode->u8MCMode = SC_MC_MODE_0;
#endif
        psPQMode->u8EOMode = SC_EO_MODE_OFF;
        psPQMode->u8FilmMode = SC_FILM_MODE_OFF;
    }
    else if( b576 )
    {
        if( bInterlace )
        {
#if 1
            psPQMode->u8MAMode = SC_MA_MODE_5;
#else
            psPQMode->u8MAMode = SC_MA_MODE_8;
#endif
            psPQMode->u8EOMode = SC_EO_MODE_1;
            psPQMode->u8MCMode = SC_MC_MODE_1;
            psPQMode->u8FilmMode = SC_FILM_MODE_2;
        }
        else
        {
#if 1
            psPQMode->u8MAMode = SC_MA_MODE_OFF;
            psPQMode->u8MCMode = SC_MC_MODE_OFF;
#else
            psPQMode->u8MAMode = SC_MA_MODE_4;
            psPQMode->u8MCMode = SC_MC_MODE_0;
#endif
            psPQMode->u8EOMode = SC_EO_MODE_OFF;
            psPQMode->u8FilmMode = SC_FILM_MODE_OFF;
        }
    }
    else if( b480 )
    {
        if( bInterlace )
        {
#if 1
            psPQMode->u8MAMode = SC_MA_MODE_5;
#else
            psPQMode->u8MAMode = SC_MA_MODE_7;
#endif
            psPQMode->u8EOMode = SC_EO_MODE_1;
            psPQMode->u8MCMode = SC_MC_MODE_1;
            psPQMode->u8FilmMode = SC_FILM_MODE_3;
        }
        else
        {
#if 1
            psPQMode->u8MAMode = SC_MA_MODE_OFF;
            psPQMode->u8MCMode = SC_MC_MODE_OFF;
#else
            psPQMode->u8MAMode = SC_MA_MODE_3;
            psPQMode->u8MCMode = SC_MC_MODE_0;
#endif
            psPQMode->u8EOMode = SC_EO_MODE_OFF;
            psPQMode->u8FilmMode = SC_FILM_MODE_OFF;
       }
    }
    else
    {
        psPQMode->u8MAMode = SC_MA_MODE_OFF;
        psPQMode->u8EOMode = SC_EO_MODE_OFF;
        psPQMode->u8MCMode = SC_MC_MODE_OFF;
        psPQMode->u8FilmMode = SC_FILM_MODE_OFF;
    }

    if(bInterlace)
        msAPIWinSetColorSpaceWindow(u8WinIndex, eWIN_COLOR_YUV_422);

    APPWINDOW_PRINT("[appWin PQ] Win:%d MA:%d EO:%d MC:%d FILM:%d\r\n", u8WinIndex, psPQMode->u8MAMode, psPQMode->u8EOMode, psPQMode->u8MCMode, psPQMode->u8FilmMode);
}
#endif

//////////////////////////////
/// Setup window //////////////
//////////////////////////////
#if 0
static void SetPQDisable(BYTE u8WinIndex, ST_WIN_PQ_MODE *psPQMode)
{
    g_u8PQEnable = 0;

    psPQMode->u8MAMode = SC_MA_MODE_OFF;
    psPQMode->u8EOMode = SC_EO_MODE_OFF;
    psPQMode->u8MCMode = SC_MC_MODE_OFF;
    psPQMode->u8FilmMode = SC_FILM_MODE_OFF;

    msPQ_Init(u8WinIndex);
    msPQ_SetMPQ(u8WinIndex);
}
#endif

void APPWindow_SetupMPQ(BYTE u8WinIndex, InputTimingType sInputTiming, ST_WINDOW_INFO sAutoWindow, ST_WIN_PQ_MODE *psPQMode)
{
#if 0
    extern BYTE g_u8PQEnable;
    BYTE u8Mem12;
    EN_WINDOW_COLOR eWinColor;
    EN_INPUT_COLOR eInputColor;
    EN_COLOR_RANGE eInputRange;
    BYTE SCDetWin = MapWin2ScIndex(u8WinIndex);
#endif
    BYTE u8PQMemFormat, u8PQMemFieldNum;

    WORD u16PQMemFieldMode;
    EN_PQ_DISPLAY_TYPE  eMultiWindowMode;
    BYTE u8Tmp, u8DispWinCnt;
    //Disable auto format with HDMI input for new FORCE COLOR FORMAT feature
    /*
    if (msAPISrcCheckHDMIMode(u8WinIndex))
    {
        USER_PREF_INPUT_COLOR_FORMAT(u8WinIndex) = INPUT_COLOR_FORMAT_AUTO; // overwrite for msAPI_SetInputColorFormat()
        APPWINDOW_PRINT("[appWindow] HDMI mode(win%d) - Auto Input Color\n", u8WinIndex);
    }
    */
#if 0
    // Set Window PQ Mode
    SetPQMode(u8WinIndex, sInputTiming, &sAutoWindow, psPQMode);

    msAPIWinGetColorSpaceInput(u8WinIndex, &eInputColor, &eInputRange);
    msAPIWinGetColorSpaceWindow(u8WinIndex, &eWinColor);

    msAPIWinGetMem12(u8WinIndex, &u8Mem12);

    g_u8PQEnable = 1;

    if( !g_ScalerInfo[SCDetWin].u8SiFbMode )
    {
        msAPIWinSetMemFormat(u8WinIndex, MEM_FMT_NO_USE);
    }
    else if( u8Mem12 )
    {
        SetPQDisable(u8WinIndex, psPQMode);
        msAPIWinSetMemFormat(u8WinIndex, MEM_FMT_444_12);
    }
    else if( eWinColor == eWIN_COLOR_YUV_422) // MA Mode MEM 422
    {
        msAPIWinSetMemFormat(u8WinIndex, MEM_FMT_422);
    }
#if FOR_DEMO
    else if (USER_PREF_MULTI_WIN_MODE > eMULTIWIN_OFF)
    {
        msAPIWinSetMemFormat(u8WinIndex, MEM_FMT_444_8);
    }
    else if (HBR2Flag(USER_PREF_WIN_SRC(u8WinIndex)))
    {
        msAPIWinSetMemFormat(u8WinIndex, MEM_FMT_444_8);
    }
#endif
    else if( psPQMode->u8MAMode <= SC_MA_MODE_0 )
    {
        msAPIWinSetMemFormat(u8WinIndex, MEM_FMT_444_10);
    }
    else // conflict setting between eWinColor & msPQ
    {
        SetPQDisable(u8WinIndex, psPQMode);
        msAPIWinSetMemFormat(u8WinIndex, MEM_FMT_444_10);
    }
#else

    sInputTiming = sInputTiming;
    sAutoWindow = sAutoWindow;
    psPQMode = psPQMode;

    for( u8Tmp = DISPLAY_WIN1, u8DispWinCnt = 0; u8Tmp < DISPLAY_MAX_NUMS ; u8Tmp++ )   //for debug cmd: in some conditions, the window number is bigger than 1, but multi window mode is none.
    {
        if (g_DisplayWindow[u8Tmp].u8DwEnable)
            u8DispWinCnt++;
    }
#if 0
    if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF && u8DispWinCnt == 1)
        eMultiWindowMode = ePQ_DISPLAY_ONE;
    else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP)
        eMultiWindowMode = ePQ_DISPLAY_PIP;
    else
        eMultiWindowMode = ePQ_DISPLAY_POP;
    msPQ_Init(u8WinIndex);
#else
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF && u8DispWinCnt == 1)
        eMultiWindowMode = ePQ_DISPLAY_ONE;
    else if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_2_PIP)
        eMultiWindowMode = ePQ_DISPLAY_PIP;
    else
        eMultiWindowMode = ePQ_DISPLAY_POP;
 #else
        eMultiWindowMode = ePQ_DISPLAY_ONE;
 #endif	
    msPQ_Init(eMultiWindowMode);

#endif
    msPQ_ForceYUV422(u8WinIndex, FALSE);    //for memory format is YUV422.
    msPQ_IsVideoMode(u8WinIndex, FALSE);
    msPQ_LoadTimingSetting(u8WinIndex, sAutoWindow, FALSE); // FALSE: to get mem format first
    msPQ_GetMemInfo(u8WinIndex, &u8PQMemFormat, &u8PQMemFieldNum, &u16PQMemFieldMode);

    msAPIWinSetMemFormat(u8WinIndex, u8PQMemFormat);
    msAPIWinSetMemFieldNum(u8WinIndex, u8PQMemFieldNum, u16PQMemFieldMode);

    SetWinColor(u8WinIndex);
    //msPQ_LoadTimingSetting(u8WinIndex, sAutoWindow, TRUE); // TRUE: DumpPQ

 
#endif

    // Set window color space
}

//////////////////////////////
/// Setup window //////////////
//////////////////////////////
void APPWindow_SetupWindow(BYTE u8WinIndex, InputTimingType sInputTiming, ST_WINDOW_INFO sAutoWindow, BYTE *pbSupportTiming, ST_WINDOW_INFO *psCapWin, ST_WINDOW_INFO *psDispWin)
{
    BYTE u8InputPort = MapWin2Port(u8WinIndex);
#if 1//((defined(JieBo_BangZi_Project))&&(PanelType== PanelLM310UH1_SLA1))//for VB1 to LVDS 3840x1260 60 display error
    WORD u16Width=0,u16Height=0;
	
        u16Width = msDrvIP1ReadAutoWidth(MapPort2DetScIndex(MapWin2Port(u8WinIndex)));
        u16Height = msDrvIP1ReadAutoHeight(MapPort2DetScIndex(MapWin2Port(u8WinIndex)));
        if (HBR2Flag(MapWin2Port(u8WinIndex)))
		u16Width*=2;
#endif
	
#if 0
    ST_FAKE_CAP_WIN_INFO sUserFakeCapWin;
#endif

    APPWINDOW_PRINT("[appWindow] APPWindow_Setup(Win%d)\n",u8WinIndex);

    u8WinIndex = u8WinIndex;


   if(u8WinIndex==DISPLAY_WIN1)
   {
       printData("3333333333333 u16Width==%x",u16Width);
       printData("3333333333333 u16Height==%x",u16Height);
       InputWidth=u16Width;
       InputHeight=u16Height;
   }

	

    CheckValidTiming(sInputTiming, sAutoWindow, pbSupportTiming);

    if( *pbSupportTiming != TRUE )
    {
        APPWINDOW_PRINT("[appWindow] pbSupportTiming.x = %d\n", *pbSupportTiming);
    }
    else
    {
        psCapWin->x       = sAutoWindow.x;
        psCapWin->y       = sAutoWindow.y;
        psCapWin->width   = sAutoWindow.width;
        psCapWin->height  = sAutoWindow.height;

        // Calulate Over scan
    #if YPBPR_PERF_OVERSCAN
        if (IsOverScan(u8InputPort))
        {
            APPWINDOW_PRINT("[appWindow] OverScan\n");
            SetOverScan(psCapWin, 97, 97);
        }
        else
    #endif
        {
             SetOverScan(psCapWin, 100, 100);
        }


        // Calulate Acpect
        APPWINDOW_PRINT("[appWindow] USER_PREF_WIDE_MODE = %d\n", USER_PREF_WIDE_MODE(u8InputPort));

#if ((defined(JieBo_BangZi_Project))&&(PanelType== PanelLM310UH1_SLA1))//for VB1 to LVDS 3840x1260 60 display error
  if((u16Width==3840)&&(u16Height==2160)&&(abs(SrcVfreq(u8InputPort)-600)<10))
  {
            SetAspect(psCapWin, psDispWin, eASPECT_1_1);
  }
else
#endif
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU		
        if (USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
            SetAspect(psCapWin, psDispWin, (eASPECT_TYPE)USER_PREF_WIDE_MODE(u8InputPort));
	else if(USER_PREF_MULTI_WIN_MODE != eMULTIWIN_OFF)
	{	
	     if(USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP)
	     {  
	          if(u8WinIndex==DISPLAY_WIN1)
	          {   SetAspect(psCapWin, psDispWin, (eASPECT_TYPE)USER_PREF_WIDE_MODE(u8InputPort));
		   }
		   else if( SyncLossState(MapWin2Port(u8WinIndex)) )
	          {   
	              SetAspect(psCapWin, psDispWin, eASPECT_FULL);
		   }
		   else
	          { 
	             SetAspect(psCapWin, psDispWin, eASPECT_FULL);
		   }
	     }
	     else
	     	{
	     	#if 1//(defined(HKC_Project)/*||defined(LeXingEnMa_Project)*/)
		  SetAspect(psCapWin, psDispWin, eASPECT_FULL);
              #else
		  SetAspect(psCapWin, psDispWin, eASPECT_NATIVE);
		#endif	  
	     	}
	 }
#else
	 SetAspect(psCapWin, psDispWin, (eASPECT_TYPE)USER_PREF_WIDE_MODE(u8InputPort));
#endif	
	            //SetAspect(psCapWin, psDispWin, eASPECT_NATIVE);

        // Set fake cpature window, each window should be set respectively
    #if 0
        //temp...
        {
            sUserFakeCapWin.x = psCapWin->x;
            sUserFakeCapWin.y = psCapWin->y;
            sUserFakeCapWin.width = psCapWin->width;
            sUserFakeCapWin.height = psCapWin->height;
        }

        msAPIWinSetMemFakeCapWin(u8WinIndex, sUserFakeCapWin, bcapWinEnable);
    #endif

        APPWINDOW_PRINT("[appWindow] sAutoWindow.x = %d\n",  sAutoWindow.x);
        APPWINDOW_PRINT("[appWindow] sAutoWindow.y = %d\n",  sAutoWindow.y);
        APPWINDOW_PRINT("[appWindow] sAutoWindow.width = %d\n",  sAutoWindow.width);
        APPWINDOW_PRINT("[appWindow] sAutoWindow.height = %d\n",  sAutoWindow.height);
        APPWINDOW_PRINT("[appWindow] CapWin.x = %d\n",  psCapWin->x);
        APPWINDOW_PRINT("[appWindow] CapWin.y = %d\n",  psCapWin->y);
        APPWINDOW_PRINT("[appWindow] CapWin.width = %d\n",  psCapWin->width);
        APPWINDOW_PRINT("[appWindow] CapWin.height = %d\n",  psCapWin->height);
        APPWINDOW_PRINT("[appWindow] DispWin.x = %d\n",  psDispWin->x);
        APPWINDOW_PRINT("[appWindow] DispWin.y = %d\n",  psDispWin->y);
        APPWINDOW_PRINT("[appWindow] DispWin.width = %d\n",  psDispWin->width);
        APPWINDOW_PRINT("[appWindow] DispWin.height = %d\n",  psDispWin->height);
    }

g_stMenuFlag.bDisplayInputNoSignalMsg[u8WinIndex]=0;
	
}


//////////////////////////////
/// Setup UserPref /////////////
//////////////////////////////
void APPWindow_SetupUserPref( BYTE u8WinIndex )
{
    extern BYTE g_u8PQEnable;
    APPWINDOW_PRINT("[appWindow] SetupUserPref(%d)",u8WinIndex);
    _WinSelIDBackup();


    if( g_u8PQEnable )
    {
#if 0
        msPQ_Init(u8WinIndex);
        msPQ_SetMPQ(u8WinIndex);
        msPQ_LoadTimingSetting(u8WinIndex);
#endif
        ST_WINDOW_INFO sDwInfo;
        msAPIWinGetCaptureWin(u8WinIndex, &sDwInfo);
        msPQ_LoadTimingSetting(u8WinIndex, sDwInfo, TRUE); // TRUE: DumpPQ
        msPQ_LoadXRuleTable(u8WinIndex);

    }

    //Update User setting
    _WinSelIDForceSet(u8WinIndex);

    #if (ENABLE_MWE)
    if(!MWE_InitColorSetting())
    #endif
    {
        msAPI_AutoSetHistogramRange(u8WinIndex);
        Menu_AdjustColorSetting();
    }

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
    SetPIPPositionParameter();
    Set_PIP_Border();
#endif

    _WinSelIDRestore();

   if(BacklightFlag&&BlackScreenFlag&&(!OSDActionFlag))
   {
     mStar_BlackWhiteScreenCtrl(BW_SCREEN_OFF);
   }


}

/////////////////////////////////////////////////////////////////////////
///////////////////////////// Source switch  ///////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPWindow_SourceSwitch(BYTE u8InputPort, BYTE *pu8SwitchPortCnt, WORD *u16StableCnt, WORD u16SyncLossThd )
{
    BYTE u8WinMask = 0;
    BYTE u8WinIndex =0;

    if( (INPUT_IS_FAKE_DUAL_S(u8InputPort)) // only Fake_Dual_M (Fake_Dual_Mono) can switch port
     && (!INPUT_IS_FAKE_DUAL_MONO(u8InputPort)) )
        return;

    if( ((*u16StableCnt) % u16SyncLossThd) == 0 ) // check syncloss timeout
    {
    #if 0//Enable_AutoInput_Menu
        if (USER_PREF_INPUT_PRIORITY != INPUT_PRIORITY_AUTO)
        {
            *u16StableCnt = 0;
            if (SyncLossState(u8InputPort))
            {
                if (msAPISrcIsCableConnected(u8InputPort))
                    Clr_CableNotConnectedFlag(u8InputPort);
                else
                    Set_CableNotConnectedFlag(u8InputPort);

                if ((g_u8MenuPageIndex==MENU_CABLE_NOT_CONNECTED&&CableNotConnectedFlag(u8InputPort))
                    ||(g_u8MenuPageIndex==MENU_STANDBY&&!CableNotConnectedFlag(u8InputPort))
                    ||g_u8MenuPageIndex==MENU_SUSPEND
                    )
                    return;
       #if LOGO_ENABLE
            #if (!ZUI)
                if(!g_stMenuFlag.bDisplayLogo)
                {
                    if(!g_stMenuFlag.bPressSrcInputKey)
                    {
                        InitOsdAfterLogo(); //reload font
                        Menu_InitAction();
                    }
                }
            #endif
	#endif		
            }
            return;
        }
     #endif
        *u16StableCnt = 1;

#if AutoSwitchPort_Enable
      if((*pu8SwitchPortCnt) < GetMulWindowInputPortNums())
      {
        *pu8SwitchPortCnt += 1;
      }
#else	 
        *pu8SwitchPortCnt += 1;

        if((*pu8SwitchPortCnt) > Input_Nums*2)
        {
            // e.g. enter DPMS or something

#if 0            
            if (SyncLossState(u8InputPort))
            {
                if (msAPISrcIsCableConnected(u8InputPort))
                    Clr_CableNotConnectedFlag(u8InputPort);
                else
                    Set_CableNotConnectedFlag(u8InputPort);

                if ((g_u8MenuPageIndex==MENU_CABLE_NOT_CONNECTED&&CableNotConnectedFlag(u8InputPort))
                    ||(g_u8MenuPageIndex==MENU_STANDBY&&!CableNotConnectedFlag(u8InputPort))
                    ||g_u8MenuPageIndex==MENU_SUSPEND
                    )
                    return;
       #if LOGO_ENABLE
            #if (!ZUI)
                if(!g_stMenuFlag.bDisplayLogo)
                {
                    if(!g_stMenuFlag.bPressSrcInputKey)
                    {
                        InitOsdAfterLogo(); //reload font
                        Menu_InitAction();
                    }
                }
            #endif
	#endif		
                return;
            }
#endif
			
        }
#endif

        u8WinMask = msAPISrcGetWinMask(u8InputPort);

      #if 1
	 if(u8WinMask&(1<<DISPLAY_WIN1))
          u8InputPort =  Get_AutoSource_NextPort(u8InputPort);
	 else
          u8InputPort = u8InputPort;
      #else
          u8InputPort = (u8InputPort+1) % Input_Nums;
      #endif	  

           g_stMenuFlag.bDisplayInputMsg[DISPLAY_WIN1]=1;
	
        if( (INPUT_IS_FAKE_DUAL_S(u8InputPort)) // skip Fake_Dual_S (not Fake_Dual_Mono)
          &&(!INPUT_IS_FAKE_DUAL_MONO(u8InputPort)) )
            u8InputPort = (u8InputPort+1) % Input_Nums;

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
     if(USER_PREF_MULTI_WIN_MODE == eMULTIWIN_OFF)
#endif	 	
        Clr_HotCrossMenuFlag();

#if (!AIO_Board  )
#if HDMI_HPD_Enable
   if(MENU_HDMI_HPD_FLAG)
#endif
   {
#if ((defined(FengYuan_Project))&&(!HDMI_HPD_Enable))
        APPWINDOW_PRINT("[appInput] PortSwitched HDMI1HPDFlag:%x,HDMI2HPDFlag:%x, HDMI3HPDFlag:%x\n",HDMI1HPDFlag,HDMI2HPDFlag,HDMI3HPDFlag);
    if((u8InputPort==INPUT_PRIORITY_HDMI)&&(!HDMI1HPDFlag))
	;
    else if((u8InputPort==INPUT_PRIORITY_HDMI2ND)&&(!HDMI2HPDFlag))	
	;
    else if((u8InputPort==INPUT_PRIORITY_HDMI3ND)&&(!HDMI3HPDFlag))	
	;
    else
#endif
        if((!INPUT_IS_DISPLAYPORT(u8InputPort))&&(!INPUT_IS_VGA(u8InputPort)))
        {
            //msAPISrcSetHPD(u8InputPort, DFT_HPD_PERIOD); // set reasonable value, e.g. >= 300ms
         #if (defined(XuLi_Project))//mqsh for GTX 750 Ti HDMI 2.0 Switch port display error(input(3840x2160 60) OSD show 1920x2160 60)
           msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, FALSE);
            ForceDelay1ms(30);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, FALSE);	
            ForceDelay1ms(300);
            msAPI_combo_IPClockControl((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, TRUE);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, TRUE);
	 #else
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, FALSE);	
            ForceDelay1ms(300);
            msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[u8InputPort].eIPMux, TRUE);	 				

	 #endif
        }
   }
#endif
        APPWINDOW_PRINT("[appInput] PortSwitched WinMask:%x, Port:%d\n",u8WinMask,u8InputPort);

#if 1
	   if(u8WinMask&(1<<DISPLAY_WIN1))
	   {
		   msAPIWinSwitchPort(u8WinIndex, u8InputPort);
		   USER_PREF_WIN_SRC(u8WinIndex) = u8InputPort;		   
	   }

#else
        while(u8WinMask!=0)
        {
            if(u8WinMask & BIT0)
            {
                msAPIWinSwitchPort(u8WinIndex, u8InputPort);
                USER_PREF_WIN_SRC(u8WinIndex) = u8InputPort;
            }
            u8WinMask >>=1;
            u8WinIndex ++;
        }
#endif		
    }
}

/////////////////////////////////////////////////////////////////////////
////////////////////////// Check Power Status  /////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPWindow_CheckColorSpaceChange( void )
{
    BYTE u8IdxWin;
    BYTE u8WinIndex;
    BYTE u8InputPort;
    BYTE u8WinMask;
    EN_INPUT_COLOR eInputColorCur;
    EN_INPUT_COLOR eInputColorPkt;


    for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
    {
        msAPIWinGetInputPort(u8IdxWin, &u8InputPort);

        if( (!msAPIWinGetDisplayStatus(u8IdxWin))
          ||(msAPISrcGetType(u8InputPort)==eAPI_SRCTYPE_ANALOG)
          ||(UnsupportedModeFlag(u8InputPort))
		  ||(InputTimingChangeFlag(u8InputPort))
          /*||(DVI mode)*/ )
        {
            continue;
        }

        eInputColorCur = msAPISrcGetCurColorSpace(u8IdxWin);
        eInputColorPkt = msAPISrcGetPktColorSpace(u8IdxWin);

        if(eInputColorCur != eInputColorPkt) // color space changed
        {
            u8WinIndex = 0;
            u8WinMask = msAPISrcGetWinMask(u8InputPort);

            APPWINDOW_PRINT("[appWindow] ColorSpaceChanged! WinMask:%x, P:%d, Color:%d \n",u8WinMask ,u8InputPort ,eInputColorPkt);

   if(BacklightFlag)
   {
     mStar_BlackWhiteScreenCtrl(BW_SCREEN_BLACK);
   }

            while(u8WinMask!=0)
            {
                if(u8WinMask & BIT0)
                {
                    msAPISrcSetCurColorSpace(u8WinIndex, eInputColorPkt);
                    msAPIWinReSetup(u8WinIndex, FALSE, TRUE);
                    APPWindow_SetupUserPref(u8WinIndex);
                }
                u8WinMask >>=1;
                u8WinIndex ++;
            }
        }
    }
}

#if (ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)
void APPWindow_CheckRangeFormatChange(void)
{
    BYTE u8IdxWin;
    BYTE u8WinIndex = 0;
    BYTE u8InputPort;
    BYTE u8WinMask = 0;
    EN_COLOR_RANGE eInputRangeCur;
    EN_COLOR_RANGE eInputRangePkt;

    for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
    {
        msAPIWinGetInputPort(u8IdxWin, &u8InputPort);

        if( (!msAPIWinGetDisplayStatus(u8IdxWin))
          ||(msAPISrcGetType(u8InputPort)==eAPI_SRCTYPE_ANALOG)\
          ||(UnsupportedModeFlag(u8InputPort))
		  ||(InputTimingChangeFlag(u8InputPort))
          /*||(DVI mode)*/ )
        {
            continue;
        }

        eInputRangeCur = msAPISrcGetCurColorRange(u8IdxWin);
        eInputRangePkt = msAPISrcGetPktColorRange(u8IdxWin);

        if(eInputRangeCur != eInputRangePkt) // color space changed
        {
            u8WinIndex = 0;
            u8WinMask = msAPISrcGetWinMask(u8InputPort);
            APPWINDOW_PRINT("[appWindow] ColorRangeChanged! WinMask:%x, P:%d, Range:%d \n",u8WinMask ,u8InputPort ,eInputRangePkt);

            while(u8WinMask!=0)
            {
                if(u8WinMask & BIT0)
                {
                    msAPISrcSetCurColorRange(u8IdxWin, eInputRangePkt);
                    msAPIWinReSetup(u8IdxWin, FALSE, TRUE);
                    APPWindow_SetupUserPref(u8IdxWin);
                }
                u8WinMask >>=1;
                u8WinIndex ++;
            }
        }
    }
}

void APPWindow_CheckColorimetryChange(void)
{
    BYTE u8IdxWin;
    BYTE u8WinIndex = 0;
    BYTE u8InputPort;
    BYTE u8WinMask = 0;
    EN_INPUT_YUV_COLORIMETRY_TYPE eInputColorimeryCur;
    EN_INPUT_YUV_COLORIMETRY_TYPE eInputColorimetryPkt;
    Bool bChanged = FALSE;

    for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
    {
        msAPIWinGetInputPort(u8IdxWin, &u8InputPort);

        if( (!msAPIWinGetDisplayStatus(u8IdxWin))
          ||(msAPISrcGetType(u8InputPort)==eAPI_SRCTYPE_ANALOG)\
          ||(UnsupportedModeFlag(u8InputPort))
		  ||(InputTimingChangeFlag(u8InputPort))
          /*||(DVI mode)*/ )
        {
            continue;
        }

        eInputColorimeryCur = msAPISrcGetCurColorimetry(u8IdxWin);
        eInputColorimetryPkt = msAPISrcGetPktYuvColorimetry(u8IdxWin);

        if(eInputColorimeryCur != eInputColorimetryPkt) // color space changed
        {
            u8WinIndex = 0;
            u8WinMask = msAPISrcGetWinMask(u8InputPort);

            APPWINDOW_PRINT("[appWindow] ColorimetryChanged! WinMask:%x, P:%d, Colorimetry:%d \n",u8WinMask ,u8InputPort ,eInputColorimetryPkt);

            bChanged = TRUE;

            while(u8WinMask!=0)
            {
                if(u8WinMask & BIT0)
                {
                    msAPISrcSetCurColorimetry(u8IdxWin, eInputColorimetryPkt);
                    msAPIWinReSetup(u8IdxWin, FALSE, TRUE);
                    APPWindow_SetupUserPref(u8IdxWin);
                }
                u8WinMask >>=1;
                u8WinIndex ++;
            }
        }
    }
}
#endif

/////////////////////////////////////////////////////////////////////////
///////////////////////////////  AP init  ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPWindow_Init( void )
{
    BYTE WinIndex;
    APPWINDOW_PRINT("[appWindow] APPWindow_Init\r\n");

    // setting based on g_DisplayWindow, supposely should be initialized by UserData.
 #if ENABLE_MULTI_WINDOW_SETTINGS_MENU   
    APPWindow_Configure((eMULTIWIN_Type)USER_PREF_MULTI_WIN_MODE, (ePIPSIZE_Type)USER_PREF_MULTI_WIN_SIZE(DISPLAY_WIN2));
#else
    APPWindow_Configure(eMULTIWIN_OFF, DISPLAY_WIN1);
#endif

    msAPICbInit();
    msAPICbAttach(eCB_SetupWindow, (void *)APPWindow_SetupWindow);
    msAPICbAttach(eCB_SetupMPQ, (void *)APPWindow_SetupMPQ);
    msAPICbAttach(eCB_SetupUserPref,(void *)APPWindow_SetupUserPref);
    msAPICbAttach(eCB_SourceSwitch,(void *)APPWindow_SourceSwitch);
    msAPICbAttach(eCB_CustomerSetPanelI2CCommand,(void *)appPanel_SetPanel_QFHD_2D_Graphic_Mode);
#if !Enable_AutoInput_Menu	
    msAPICbAttach(eCB_CustomerSetPMModeFlag,(void *)Set_PMModeFlag);
#endif
    msAPICbAttach(eCB_CustomerSetIsrTimingChangeStatus,(void *)SetIsrTimingChangeStatus);
#if AudioFunc
    msAPICbAttach(eCB_CustomerSetModeChangeAudioMuteOnOff,(void *)SetModeChangeAudioMuteOnOff);
#endif
#if INPUT_SAME_OUT
    msAPICbAttach(eCB_CustomerSetCurFrameMode, (void *)APPWindow_SetCurFrameMode);
#endif

    g_bAPPWindow_Init=1;
    USER_PREF_WIN_SEL = DISPLAY_WIN1;  //tempsetting
    for( WinIndex = 0 ; WinIndex < DISPLAY_MAX_NUMS ; WinIndex++ )
    {
        msAPIWinSetReadCurFrameMode(WinIndex, DISABLE);
    }
}

/////////////////////////////////////////////////////////////////////////
///////////////////////////  AP window handler  ////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPWindow_Handler( void )
{
    APPWindow_CheckColorSpaceChange();
#if ((ENABLE_CHECK_RANGE_COLORIMETRY_CHANGE)&&(!(defined(LianHeChuangXin_Project))))
    APPWindow_CheckRangeFormatChange();//Julian, 2014.03.17
    APPWindow_CheckColorimetryChange();//Julian, detect colorimetry change.
#endif

    APPWindow_FlyWindow();
}

//////////////////////////////
#if 0 // replaced by APPWindow_CheckLOnly, APPWindow_CheckROnly
static BOOL APPWindow_Check2P(ST_WINDOW_INFO sDisplayWin)
{
#if (MST9U_ASIC_1P==1)
    sDisplayWin = sDisplayWin;
#else
    if( (sDisplayWin.x < (PANEL_WIDTH/2))
     && ((PANEL_WIDTH/2) < sDisplayWin.x+sDisplayWin.width) )
        return TRUE;
#endif

    return FALSE;
}
#endif

static BOOL APPWindow_CheckLOnly(ST_WINDOW_INFO sDisplayWin)
{
    if((sDisplayWin.x + sDisplayWin.width)<=(PANEL_WIDTH/2) )
        return TRUE;
    return FALSE;
}

static BOOL APPWindow_CheckROnly(ST_WINDOW_INFO sDisplayWin)
{
    if(sDisplayWin.x>=(PANEL_WIDTH/2))
        return TRUE;
    return FALSE;
}
void APPWindow_SetSize(BYTE u8DispWin, WORD u16HP, WORD u16VP, WORD u16Width, WORD u16Heigh)
{
    ST_WINDOW_INFO  sDisplayWin;
    BYTE bLOldOnly, bROldOnly, bLNewOnly, bRNewOnly;
    BOOL bRedispatch;
    msAPIWinGetDisplayWin(u8DispWin, &sDisplayWin);

    bRedispatch = ((g_DisplayWindow[u8DispWin].eDwStatus!=WIN_STATUS_OUTPUT_DISPLAYOK)||(UnsupportedModeFlag(MapWin2Port(u8DispWin))))?TRUE:FALSE;
    bLOldOnly = APPWindow_CheckLOnly(sDisplayWin);
    bROldOnly = APPWindow_CheckROnly(sDisplayWin);

    sDisplayWin.x = u16HP;
    sDisplayWin.y = u16VP;
    sDisplayWin.width = u16Width;
    sDisplayWin.height = u16Heigh;

    bLNewOnly = APPWindow_CheckLOnly(sDisplayWin);
    bRNewOnly = APPWindow_CheckROnly(sDisplayWin);

    msAPIWinSetDisplayWin(u8DispWin, &sDisplayWin);
    //msAPIWinReSetup(u8DispWin, ((bLOldOnly!=bLNewOnly)||(bROldOnly!=bRNewOnly)||(bRedispatch)?TRUE:FALSE), TRUE);
    msAPIWinReSetup ( u8DispWin, TRUE, TRUE );
}

void APPWindow_SetPosition(BYTE u8DispWin, WORD u16HP, WORD u16VP)
{
    ST_WINDOW_INFO  sDisplayWin;
    BYTE bLOldOnly, bROldOnly, bLNewOnly, bRNewOnly;
    BOOL bRedispatch;
    msAPIWinGetDisplayWin(u8DispWin, &sDisplayWin);

    bRedispatch = ((g_DisplayWindow[u8DispWin].eDwStatus!=WIN_STATUS_OUTPUT_DISPLAYOK)||(UnsupportedModeFlag(MapWin2Port(u8DispWin))))?TRUE:FALSE;
    bLOldOnly = APPWindow_CheckLOnly(sDisplayWin);
    bROldOnly = APPWindow_CheckROnly(sDisplayWin);

    sDisplayWin.x = u16HP;
    sDisplayWin.y = u16VP;

	

    bLNewOnly = APPWindow_CheckLOnly(sDisplayWin);
    bRNewOnly = APPWindow_CheckROnly(sDisplayWin);

    msAPIWinSetDisplayWin(u8DispWin, &sDisplayWin);
    msAPIWinReSetup(u8DispWin, ((bLOldOnly!=bLNewOnly)||(bROldOnly!=bRNewOnly)||(bRedispatch)?TRUE:FALSE), TRUE);
}

//////////////////////////////
void APPWindow_SetColorSpace(BYTE u8DispWin)
{
   // msAPIWinReSetup(u8DispWin, FALSE);
    SetWinColor(u8DispWin);
    APPWindow_SetupUserPref(u8DispWin);
}

//////////////////////////////
void APPWindow_Configure(eMULTIWIN_Type eType, ePIPSIZE_Type ePipType)
{
    ST_WINDOW_INFO  DisplayWin = {0, 0, PANEL_WIDTH, PANEL_HEIGHT};
    BYTE WindowROTType=ROT_NONE;

#if Enable_Panel_Rotate_180	
    WindowROTType=ROT_HV_MIRROR;
#elif Enable_Menu_Panel_Rotate_180
   if(USER_PREF_ROTATE_INDEX==Rotate_90)
    WindowROTType=ROT_90;
   else if(USER_PREF_ROTATE_INDEX==Rotate_180)
    WindowROTType=ROT_HV_MIRROR;
   else if(USER_PREF_ROTATE_INDEX==Rotate_270)
    WindowROTType=ROT_270;

#endif


    MST_ASSERT(eType <eMULTIWIN_MAX);

    switch(eType)
    {
        case eMULTIWIN_OFF:
            APPWINDOW_PRINT("<< eMULTIWIN_OFF >>\n");
            msAPIWinDel(DISPLAY_WIN1);
            msAPIWinDel(DISPLAY_WIN2);
            msAPIWinDel(DISPLAY_WIN3);
            msAPIWinDel(DISPLAY_WIN4);
            msAPIWinAdd(DISPLAY_WIN1,  USER_PREF_WIN_SRC(DISPLAY_WIN1), WindowROTType, DisplayWin);
	     mStar_SetupFreeRunMode();
            break;

        case eMULTIWIN_2_PIP:
            APPWINDOW_PRINT("<< eMULTIWIN_2_PIP >>\n");
            msAPIWinDel(DISPLAY_WIN1);
            msAPIWinDel(DISPLAY_WIN2);
            msAPIWinDel(DISPLAY_WIN3);
            msAPIWinDel(DISPLAY_WIN4);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_2_PIP][DISPLAY_WIN1], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN1, USER_PREF_WIN_SRC(DISPLAY_WIN1), WindowROTType, DisplayWin);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_2_PIP][DISPLAY_WIN2], sizeof(ST_WINDOW_INFO));
     #if ENABLE_MULTI_WINDOW_SETTINGS_MENU			
            SetPipSize(&DisplayWin, ePipType);
     #endif	 
            msAPIWinAdd(DISPLAY_WIN2, USER_PREF_WIN_SRC(DISPLAY_WIN2), WindowROTType, DisplayWin);
 	     mStar_SetupFreeRunMode();
           break;

        case eMULTIWIN_2_POP1:
            APPWINDOW_PRINT("<< eMULTIWIN_2_POP1 >>\n");
            msAPIWinDel(DISPLAY_WIN1);
            msAPIWinDel(DISPLAY_WIN2);
            msAPIWinDel(DISPLAY_WIN3);
            msAPIWinDel(DISPLAY_WIN4);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_2_POP1][DISPLAY_WIN1], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN1, USER_PREF_WIN_SRC(DISPLAY_WIN1), WindowROTType, DisplayWin);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_2_POP1][DISPLAY_WIN2], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN2, USER_PREF_WIN_SRC(DISPLAY_WIN2), WindowROTType, DisplayWin);
  	     mStar_SetupFreeRunMode();
          break;

        case eMULTIWIN_2_POP2:
            APPWINDOW_PRINT("<< eMULTIWIN_2_POP2 >>\n");
            msAPIWinDel(DISPLAY_WIN1);
            msAPIWinDel(DISPLAY_WIN2);
            msAPIWinDel(DISPLAY_WIN3);
            msAPIWinDel(DISPLAY_WIN4);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_2_POP2][DISPLAY_WIN1], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN1, USER_PREF_WIN_SRC(DISPLAY_WIN1), WindowROTType, DisplayWin);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_2_POP2][DISPLAY_WIN2], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN2, USER_PREF_WIN_SRC(DISPLAY_WIN2), WindowROTType, DisplayWin);
  	     mStar_SetupFreeRunMode();
          break;

        case eMULTIWIN_3:
            APPWINDOW_PRINT("<< eMULTIWIN_3 >>\n");
            msAPIWinDel(DISPLAY_WIN1);
            msAPIWinDel(DISPLAY_WIN2);
            msAPIWinDel(DISPLAY_WIN3);
            msAPIWinDel(DISPLAY_WIN4);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_3][DISPLAY_WIN1], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN1, USER_PREF_WIN_SRC(DISPLAY_WIN1), WindowROTType, DisplayWin);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_3][DISPLAY_WIN2], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN2, USER_PREF_WIN_SRC(DISPLAY_WIN2), WindowROTType, DisplayWin);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_3][DISPLAY_WIN3], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN3, GET_WIN_INPUTSOURCE(DISPLAY_WIN3), WindowROTType, DisplayWin);
 	     mStar_SetupFreeRunMode();
           break;

        case eMULTIWIN_4:
            APPWINDOW_PRINT("<< eMULTIWIN_4 >>\n");
            msAPIWinDel(DISPLAY_WIN1);
            msAPIWinDel(DISPLAY_WIN2);
            msAPIWinDel(DISPLAY_WIN3);
            msAPIWinDel(DISPLAY_WIN4);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_4][DISPLAY_WIN1], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN1, USER_PREF_WIN_SRC(DISPLAY_WIN1), WindowROTType, DisplayWin);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_4][DISPLAY_WIN2], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN2, USER_PREF_WIN_SRC(DISPLAY_WIN2), WindowROTType, DisplayWin);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_4][DISPLAY_WIN3], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN3, USER_PREF_WIN_SRC(DISPLAY_WIN3), WindowROTType, DisplayWin);
            memcpy(&DisplayWin, &tbMultiWin_Type[eMULTIWIN_4][DISPLAY_WIN4], sizeof(ST_WINDOW_INFO));
            msAPIWinAdd(DISPLAY_WIN4, USER_PREF_WIN_SRC(DISPLAY_WIN4), WindowROTType, DisplayWin);
	     mStar_SetupFreeRunMode();
            break;

        case eMULTIWIN_MAX:
            break;
    }
    msAPIWinApply();

}


//////////////////////////////
#if YPBPR_PERF_OVERSCAN
Bool IsOverScan(BYTE InputPort)
{
/////////////////////////////////////////////////////////////////////////////
// Overscan Rule
// Condition A: Video Timing
// Condition B: Interlace mode
// Condition C: VGA SOG+YUV color space(By user setting)
// Condition D: HDMI/DP && Not digital 1080p(!1080P)
// Condition E: HDMI and ITC flag is set --> no overscan.
//=> (A &&(B || C || D))
//***************************************************************************************************************
//INPORTANT:DVI with non-interlace mode needn't to do overscan,g_TMDSInfo.u8InputColor must be clear to INPUT_RGB.
//***************************************************************************************************************
/////////////////////////////////////////////////////////////////////////////
    BYTE u8Temp=StandardModeGroup(InputPort);

  #if (INPUT_TYPE==INPUT_1A)

    return ((msDrvIP1GetInputSyncType(MapPort2DetScIndex(INPUT_VGA))&COMP_B && u8Temp>=RES_720x480I && u8Temp<=RES_1920x1080_60)\
        && (InterlaceModeFlag(InputPort) || (CURRENT_INPUT_COLOR_FORMAT==INPUT_COLOR_YPBPR)));

  #else
    return ((u8Temp>=RES_720x480I && u8Temp<=RES_1920x1080_60)
            && (InterlaceModeFlag(InputPort)
        #if ENABLE_VGA_INPUT
            || (INPUT_IS_VGA(InputPort)&& (msDrvIP1GetInputSyncType(MapPort2DetScIndex(INPUT_VGA))&COMP_B) && (CURRENT_INPUT_COLOR_FORMAT==INPUT_COLOR_YPBPR))
        #endif
        #if ENABLE_DVI_INPUT || ENABLE_HDMI_INPUT
            || (( (INPUT_IS_TMDS(InputPort)&& (InputColorFormat(InputPort)!=INPUTCOLOR_RGB) )
        #endif
        #if ENABLE_DP_INPUT
               || INPUT_IS_DISPLAYPORT(InputPort)&& ((BYTE)msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[InputPort].eIPMux)).ucColorType !=(BYTE)INPUTCOLOR_RGB) )
        #endif
               ) && ((u8Temp!=RES_1920x1080_50 && u8Temp!=RES_1920x1080_60)
        #if 1//FRAME_BFF_SEL==FRAME_BUFFER
                 && (u8Temp!=RES_1920x1080_24 && u8Temp!=RES_1920x1080_25 && u8Temp!=RES_1920x1080_30)
        #endif
                    ))
            || SrcModeIndex(InputPort)==MD_1920X1080I_50)
            );
  #endif

}
#endif

void APPWindow_FlyWindow(void)
{
    ST_WINDOW_INFO sDisplayWin;
    WORD x, y, x_end, y_end, width, height, i;
    BYTE x_offset = 4, y_offset = 4;
    static BYTE x_plus[DISPLAY_MAX_NUMS], y_plus[DISPLAY_MAX_NUMS];

    for( i = 0 ; i < DISPLAY_MAX_NUMS ; i++ )
    {
        if( (!g_bAPPWindow_FlyWin) || (!msAPIWinGetDisplayStatus(i)) )
            return;

        msAPIWinGetDisplayWin( i, &sDisplayWin );

        x = sDisplayWin.x;
        y = sDisplayWin.y;
        width = sDisplayWin.width;
        height = sDisplayWin.height;
        x_end = x + width - 1;
        y_end = y + height - 1;

        if( x_plus[i] )
        {
            if( x_end + x_offset < PANEL_WIDTH )
                x_plus[i] = 1;
            else
                x_plus[i] = 0;
        }
        else
        {
            if( x < x_offset )
                x_plus[i] = 1;
            else
                x_plus[i] = 0;
        }

        if( x_plus[i] )
        {
            x += x_offset;
            x_end += x_offset;
        }
        else
        {
            x -= x_offset;
            x_end -= x_offset;
        }

        if( y_plus[i] )
        {
            if( y_end + y_offset < PANEL_HEIGHT )
                y_plus[i] = 1;
            else
                y_plus[i] = 0;
        }
        else
        {
            if( y < y_offset )
                y_plus[i] = 1;
            else
                y_plus[i] = 0;
        }

        if( y_plus[i] )
        {
            y += y_offset;
            y_end += y_offset;
        }
        else
        {
            y -= y_offset;
            y_end -= y_offset;
        }

        APPWindow_SetPosition( i, x, y );
    }
}

// Force fading whole screen from black to while, 1 RGB pixel step per frame.
void APPWindow_FadingWindow(void)
{
    WORD i = 0;

    for(i=0;i<256;i++)
    {
        // Polling Customer Event to Stop Fading
        // e.g. timing change
        // ...

        WAIT_V_OUTPUT_BLANKING_START();
        msAPI_ForceFrameColor(ENABLE, i, i, i);
    }

    msAPI_ForceFrameColor(DISABLE, 0, 0, 0);
}

void APPWindow_DrawBMP(BYTE u8WinIndex)
{
#if ENABLE_FB_BMP_DRAW
    U32 src_addr, bin_len;
    BININFO cpr2_binfo;
    _sBMPType sBMPType, *bp = &sBMPType;
    BYTE *pData;
    WORD i, width_offset;
    WORD u16Mask;

    cpr2_binfo.B_ID = BIN_ID_BMP_TEST_FILE;
    if (!Get_BinInfo(&cpr2_binfo))
    {
        APPWINDOW_PRINT("Get CP_R2 BINFO (B_ID=0x%x) fail..\n", cpr2_binfo.B_ID);
        return;
    }

    src_addr = cpr2_binfo.B_FAddr;
    bin_len  = cpr2_binfo.B_Len;

    memcpy(((BYTE *)bp)+2, (BYTE*)(src_addr|0xC0000000), sizeof(_sBMPType)-2);

    APPWINDOW_PRINT("WIDTH:%x\n", bp->width);
    APPWINDOW_PRINT("HIGHT:%x\n", bp->height);
    APPWINDOW_PRINT("OFFSET:%x\n", bp->offset);

    pData = (BYTE*)((src_addr + bp->offset)|0xC0000000);
    width_offset = (bp->width * 3 + 3) & 0xFFFC;
    for( i = bp->height ; i > 0 ; i-- )
    {
        msAPIWinDrawLine(u8WinIndex, (i-1), 0, bp->width, pData, NULL);
        pData += width_offset;
    }

    msAPIWinFreerun(u8WinIndex, FALSE);
    u16Mask = (g_DisplayWindow[u8WinIndex].u16DwScMaskOut|g_DisplayWindow[u8WinIndex].u16DwScMaskIn) & bSC_MAIN;
    msDrvScMuteScreen(u16Mask, FALSE);

#else

    u8WinIndex = u8WinIndex;

#endif
}

void APPWindow_DrawJPG(BYTE u8WinIndex)
{	
#if ENABLE_FB_JPG_DRAW
	#include "tjpgd.h"

    BININFO jpg_binfo;	
    WORD u16Mask;
	
	void *work;       /* Pointer to the decompressor work area */
    JDEC jdec;        /* Decompression object */
    JRESULT res;      /* Result code of TJpgDec API */
    IODEV devid;      /* User defined device identifier */
	
	jpg_binfo.B_ID = BIN_ID_JPG_TEST_FILE;
    if (!Get_BinInfo(&jpg_binfo))
    {
        APPWINDOW_PRINT("Get JPG_BINFO (B_ID=0x%x) fail..\n", jpg_binfo.B_ID);
        return;
    }

	devid.u8widx = u8WinIndex;
    devid.fp = (BYTE *)(jpg_binfo.B_FAddr | 0xC0000000);
	
    /* Allocate a work area for TJpgDec */
    work = malloc(3100);
	
    /* Prepare to decompress */
    res = jd_prepare(&jdec, in_func, work, 3100, &devid);

    if (res == JDR_OK) {
        /* Ready to dcompress. Image info is available here. */
        APPWINDOW_PRINT("Image dimensions: %u by %u. %u bytes used.\n", jdec.width, jdec.height, 3100 - jdec.sz_pool);	
		devid.wfbuf = jdec.width;
		
        res = jd_decomp(&jdec, out_func, 0);   /* Start to decompress with 1/1 scaling */		
		if (res == JDR_OK) {
            /* Decompression succeeded. You have the decompressed image in the frame buffer here. */
			APPWINDOW_PRINT("\rDecompress OK\n");	
        } else {
            printf("Failed to decompress: rc=%d\n", res);
        }
    }
	else {
        APPWINDOW_PRINT("Failed to prepare: rc=%d\n", res);
    }

	free(work); 			/* Discard work area */
	
    msAPIWinFreerun(u8WinIndex, FALSE);
    u16Mask = (g_DisplayWindow[u8WinIndex].u16DwScMaskOut|g_DisplayWindow[u8WinIndex].u16DwScMaskIn) & bSC_MAIN;
    msDrvScMuteScreen(u16Mask, FALSE);
	
#else	

		u8WinIndex = u8WinIndex;

#endif
}

#if INPUT_SAME_OUT
Bool CHECK_FRC_MODE(void)
{
    WORD u16InputVfreq = 0;
    WORD hFreq = 0;
    WORD vFreq = 0;
    BYTE u8DetSCIndex = MapPort2DetScIndex(USER_PREF_INPUT_TYPE);

    // vFreq = msDrvIP1ReadVtotal(u8DetSCIndex);//_msRead2Byte(SC0_E2) & 0xFFF;
    hFreq = HFREQ(msDrvIP1ReadHperiod(u8DetSCIndex));
    vFreq = VFREQ(hFreq, msDrvIP1ReadVtotal(u8DetSCIndex));
    u16InputVfreq = vFreq;

    // u16InputVfreq=VFREQ( HFREQ( SrcHPeriod(MapWin2Port(USER_PREF_WIN_SEL)) ), SrcVTotal(MapWin2Port(USER_PREF_WIN_SEL)) );
	if( u16InputVfreq < (PanelMinVfreq) || u16InputVfreq > (PanelMaxVfreq) )
	{
		printData("PanelVfreq =%d", PanelVfreq);
		return TRUE;    //V_FREQ_IN(SCDetWin);
	}

	return FALSE;
}
void APPWindow_SetCurFrameMode( BYTE ucWinIndex)        //20140219 Rick add for input lag issue, condition: no pip/pbp & input 60hz timing
{

    WORD u8Vfreq = 0;

    u8Vfreq = VFREQ( HFREQ( SrcHPeriod(MapWin2Port(ucWinIndex)) ), SrcVTotal(MapWin2Port(ucWinIndex)) );
    
    u8Vfreq=u8Vfreq/10;

    APPWINDOW_PRINT("\r\n ###############u8Vfreq=%d",u8Vfreq);

    if(SrcFlags(SrcInputPortM)&bInterlaceMode)
    {
        u8Vfreq=u8Vfreq*2;
    }
	

    if(CHECK_FRC_MODE()
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
      ||(USER_PREF_MULTI_WIN_MODE!=eMULTIWIN_OFF)
#endif
	)
    	{
        msAPIWinSetReadCurFrameMode(ucWinIndex, DISABLE);
    	}
    else
    	{
		if(( INPUT_IS_HDMI_DRR(USER_PREF_INPUT_TYPE) ||INPUT_IS_DP_DRR(USER_PREF_INPUT_TYPE) ))
		{
		       msAPIWinSetReadCurFrameMode(ucWinIndex, ENABLE);
		}
		else
		{
			msAPIWinSetReadCurFrameMode(ucWinIndex, ENABLE);
		}
    	}
}
#endif



