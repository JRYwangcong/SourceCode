
#ifndef MENUDEF_H
#define MENUDEF_H

#include "Board.h"
#include "Panel.h"
#include "UserPrefDef.h"

#ifndef HOT_MENU_LOCK_HOLD_TIME
#define HOT_MENU_LOCK_HOLD_TIME     10//15 //sec
#endif

#ifndef HOT_DOWN_HOLD_TIME
#define HOT_DOWN_HOLD_TIME     3//sec
#endif

#define POWER_HOLD_TIME        2//15 //sec

#define STFC_TIMEOUT                300//900 //FY12_MSG 5min

#define INIT_WORD_SPACE      2///1 Pixel
#define WORD_SPACE               2///1 Pixel
#define FONT_COMMAND      0xFF


#define MENU_TBL_CHECK              0 //Can be used during develop OSD, just for table checking.
#define ENABLE_WORD_FONT_INDEX      1 //for asia language
#define ENABLE_STFC_TIMEOUT_SLEEP   0
////////////////////////////////////////////
#define MULTI_NUMBER            0//1
#define MULTI_GAUGE             0//1
/// Enable USE_ADJUSTABLE_MENU make more flexibilty for change the Menu size and items position, but slower than that disabled.
#define USE_ADJUSTABLE_MENU     1
#define USE_PROP_NUMBER         1

typedef enum
{
    MENU_POWER_OFF,           // 0
    MENU_POWER_ON,            // 1
    MENU_STANDBY,             // 2
    MENU_CABLE_NOT_CONNECTED, // 3
    MENU_ENTERPOWERSAVING,     // 4
    MENU_UNSUPPORTED_MODE,    // 5
    MENU_ROOT,                // 6

#if (defined(LianHeChuangXin_Project))
    MENU_KEY_GUIDE,               
#endif

    // user definition from here
#if LOGO_ENABLE    
    MENU_LOGO,                //7
#endif    
#if ENABLE_VGA_INPUT
    MENU_AUTO,                //8
    MENU_HOT_AUTO,            //9
#endif
#if ENABLE_MULTI_INPUT
    MENU_INPUT,               //10
#endif
    MENU_HOT_MENU_LOCK,       //11
#if ENABLE_DEMURA_FUNCTION
    MENU_DEMURA_MEASURE_MODE, //14
#endif
    MENU_MAIN,                //15
    /// S1--------------------------------------------
    MENU_QUICKSTART,

    MENU_PICTURE,

    MENU_COLOR,
  
    MENU_SYSTEM,

#if LED_HOT_Menu_Func
    MENU_SYSTEMLLEDUSER,
#endif

    MENU_USER,

    MENU_ADVANCED,

#if LED_HOT_Menu_Func
    MENU_LEDMODE,
#endif





#if ENABLE_MULTI_INPUT
    MENU_INPUT_SOURCE,
#endif

#if ENABLE_AUDIO_SETTINGS_MENU
    MENU_HOTVOLUME,
#endif    
    MENU_HOTBRIGHTNESS,
    MENU_HOTCONTRAST,
#if Enable_HotAdjLowbluray
    MENU_HOTLOWBLURAY,                         //
#endif


    MENU_HOTPICTUREFORMAT,
    MENU_HOTPICTUREMODE,
    
#if ENABLE_AUDIO_SETTINGS_MENU    
    MENU_HOTMUTE,
#endif
#if HKC_RGB_Range_Func
    MENU_HOTRGBRANGE,
#endif
    MENU_HOTDCR,
    MENU_HOTINPUT,
    MENU_HOTCROSS,
#if SleepMode_Enable    
    MENU_POWEROFFMSG,
#endif   

#if SleepMode_Enable    
    MENU_SLEEPMODE,
#endif
#if 0//LED_HOT_Menu_Func    
    MENU_HOTLEDMODE,
#endif

#if Enable_Corss_LHCX_New

    HotKeyGameModeMenu,		// 22
    
    HotKeyGameSelectMenu,
    HotKeyGameSubSelectMenu,
    HotKeyGameClokMenu,
    HotKeyGameClokPositionMenu,
    HotKeyGameClokShowMenu,


#endif

    // user definition end
    MAX_MENU,

    //Extension Naming
    MENU_L1_START             = MENU_QUICKSTART,

#if LED_HOT_Menu_Func
    MENU_L1_END                 = MENU_LEDMODE,
#else
    MENU_L1_END                 = MENU_ADVANCED,
#endif
} MenuPageIndexType;
#define IS_MAIN_L0(MenuPage)        (MenuPage==MENU_MAIN)
#define IS_MAIN_L1(MenuPage)        ((MenuPage>=MENU_L1_START) && (MenuPage<=MENU_L1_END))
#define IS_WARNING_MENU(MenuPage)  ((MenuPage>=MENU_STANDBY) && (MenuPage<=MENU_UNSUPPORTED_MODE))
#if ENABLE_VGA_INPUT
#define IS_MSG_STATUS(MenuPage)     ((MenuPage>=MENU_AUTO) && (MenuPage<MENU_MAIN))
#else
#if LOGO_ENABLE
#define IS_MSG_STATUS(MenuPage)     ((MenuPage>MENU_LOGO) && (MenuPage<MENU_MAIN))
#else
#define IS_MSG_STATUS(MenuPage)     ((MenuPage>=MENU_INPUT) && (MenuPage<MENU_MAIN))
#endif
#endif

#if LED_HOT_Menu_Func
#define HOT_MENU_STATUS(MenuPage)     ((MenuPage>MENU_LEDMODE) && (MenuPage<MAX_MENU))
#else
#define HOT_MENU_STATUS(MenuPage)     ((MenuPage>MENU_ADVANCED) && (MenuPage<MAX_MENU))
#endif

#define STATUS_MENUPAGE			 (g_u8MenuPageIndex >= MENU_STANDBY && g_u8MenuPageIndex <= MENU_UNSUPPORTED_MODE)
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define InputStatusMultiWin        (g_u8MenuPageIndex == MENU_INPUT && USER_PREF_MULTI_WIN_MODE > eMULTIWIN_OFF)
#endif

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define HDMI_DP_AUDIO_ENABLE(InputPort)     (((USER_PREF_INPUT_TYPE==InputPort)&&(!SyncLossState(USER_PREF_INPUT_TYPE)))\
								||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_PIP||USER_PREF_MULTI_WIN_MODE==eMULTIWIN_2_POP1)&&(USER_PREF_INPUT_TYPE_SUBWIN1==InputPort)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1)))\
	                                              ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_3)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==InputPort)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==InputPort)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN2)))))\
	                                              ||((USER_PREF_MULTI_WIN_MODE==eMULTIWIN_4)&&(((USER_PREF_INPUT_TYPE_SUBWIN1==InputPort)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN1)))||((USER_PREF_INPUT_TYPE_SUBWIN2==InputPort)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN2)))||((USER_PREF_INPUT_TYPE_SUBWIN3==InputPort)&&(!SyncLossState(USER_PREF_INPUT_TYPE_SUBWIN3)))))\
	                                              )
#else
#define HDMI_DP_AUDIO_ENABLE(InputPort)     (USER_PREF_INPUT_TYPE==InputPort)
#endif



//Draw Number Flag
#define NUM_FUNC            0x01
#define NUM_HEX             0x02
#define NUM_DRAW_ARROW      0x04
#define NUM_LEADING_BLANK   0x08
#define NUM_SEC             0x10
#define NUM_4NUMS           0x20
#define NUM_PERCENT         0x40

#define NUM_DCR_DISABLE  0x80
#define NUM_END             0x8000

//Draw Text Flag    for RadioGroup
#define TXT_FUNC            0x01
#define TXT_OVER_256        0x02
#define TXT_CENTER          0x04    ///Use X,Y position as center
#define TXT_FULL_CENTER     0x08 ///Center of Menu and Clear other space at the same line

#define TXT_ARROW           0x10 // right align to x,y pos
#define TXT_ICON        0x20
#define TXT_OVER_512        0x40
#define TXT_RealText      0x80

#define TXT_PIP_PBP_DISABLE      0x100

#define TXT_END             0x8000

#define BAR_FUNC            0x01
#define BAR_END             0x80

//Define the flags for DrawIconType
#define ICO_FUNC            0x01
#define ICO_MONO            0x02 //MONO Font Icon if set,else Color Font Icon
//#define ICO_TABLE           0x04 //There are two type of icon data structure, one save the memory offset of start position the other use the table to save each fonts offset.
#define ICO_OVER512         0x04
#define ICO_OVER256         0x08
//#define ICO_TOP             0x10  ///Draw top raw only
#define ICO_BOTTOM          0x20 ///Draw Bottom only only
#define ICO_WAIT            0x40 //Jison 080521

#define ICO_END             0x8000

typedef BYTE* (code *fpDisplayText)(void);
typedef Bool  (code *fpExecFunc)(void);
typedef void  (code *fpLoadFont)(void);
//typedef BYTE (code *fpItemIndex)(void);

#if (ZUI)
typedef Bool  (code *fpAdjustValue)(Bool enAction);
#else
typedef Bool  (code *fpAdjustValue)(MenuItemActionType enAction);
#endif

typedef WORD  (code *fpGetValue)(void);
//typedef WORD (code *fpGetMaxValue)(void);
//typedef WORD (code *fpSetMaxValue)(void);

// Menu Page flag Bits
#define MPB_FUNC        0x01
#define MPB_STAY        0x02 // for stay in menu page
#define MPB_INVISIBLE   0x04 // menu page visible or not
#define MPB_CENTER      0x08
#define MPB_LOAD_GAUGE  0x20
#define MPB_OSD_COUNT3  0x40
#define MPB_OSD_COUNT5  0x80

// Menu Item Bits
//#define MIB_ENGINE1             0x0001  // Engine 1 item

#define MIB_DCR_ENABLE         0x0001  // for DCR
#define MIB_SELECTABLE          0x0002  // for menu item selectable
#define MIB_BRIGHTNESS_DISABLE  0x0004  
#define MIB_DVI_DISABLE         0x0008

#define MIB_FACTORY_ITEM        0x0010
#define MIB_ADJUST_ITEM         0x0020
#define MIB_FREESYNC_DISABLE         0x0040
#define MIB_ColorTemp_DISABLE         0x0080

#define MIB_USER_DISABLE     0x0100
#define MIB_IR_DELAY             0x0400
#define MIB_PIP_PBP_DISABLE             0x0800
#define MIB_IR_SELECT            0x1000



#if MSTAR_LOGO
#define LogoMenuHSize 	42
#define LogoMenuVSize	11
#define Logo4CFontStart   		0x100
#elif LOGO_RX
#define LogoMenuHSize 	61
#define LogoMenuVSize	22
#define Logo4CFontStart   		0x102
#elif LOGO_MONOPRICE
#define LogoMenuHSize 	86
#define LogoMenuVSize	6
#define Logo4CFontStart   		0x102
#elif LOGO_TERRA
#define LogoMenuHSize 	47
#define LogoMenuVSize	11
#define Logo4CFontStart   		0x102

#elif LOGO_UPSTAR
#define LogoMenuHSize 	41
#define LogoMenuVSize	12
#define Logo4CFontStart   		0x102

#elif LOGO_HKC
#define LogoMenuHSize 	35
#define LogoMenuVSize	9
#define Logo4CFontStart   		0x102

#elif LOGO_AXM
#define LogoMenuHSize 	86
#define LogoMenuVSize	16
#define Logo4CFontStart   		0x1ff
#elif LOGO_ZEOL
#define LogoMenuHSize 	55
#define LogoMenuVSize	8
#define Logo4CFontStart   		0x102
#elif LOGO_ZEOL_X2
#define LogoMenuHSize 	120
#define LogoMenuVSize	15
#define Logo4CFontStart   		0x102

#elif LOGO_Neview
#define LogoMenuHSize 	57
#define LogoMenuVSize	23
#define Logo4CFontStart   		0x1F0
#elif LOGO_HDC
#define LogoMenuHSize 	83
#define LogoMenuVSize	20
#define Logo4CFontStart   		0x1F0
#elif LOGO_JAPANNEXT
#define LogoMenuHSize 	127
#define LogoMenuVSize	11
#define Logo4CFontStart   		0x1F0
#elif LOGO_KOIS
#define LogoMenuHSize 	79
#define LogoMenuVSize	19
#define Logo4CFontStart   		0x1E0
#elif LOGO_KOGAN||LOGO_KOGAN_NEW
#define LogoMenuHSize 	79
#define LogoMenuVSize	18
#define Logo4CFontStart   		0x1E0
#elif LOGO_BENZ
#define LogoMenuHSize 	100
#define LogoMenuVSize	12
#define Logo4CFontStart   		0x102
#elif LOGO_TIEMU
#if 1
#define LogoMenuHSize 	42
#define LogoMenuVSize	7
#define Logo4CFontStart   		0x102
#else
#define LogoMenuHSize 	51
#define LogoMenuVSize	7
#define Logo4CFontStart   		0x102
#endif

#elif LOGO_X_Star
#define LogoMenuHSize 	109
#define LogoMenuVSize	25
#define Logo4CFontStart   		0x1FE
#elif LOGO_BUBALUS
#define LogoMenuHSize 	59
#define LogoMenuVSize	14
#define Logo4CFontStart   		0x1FE
#elif LOGO_Qnix
#define LogoMenuHSize 	80
#define LogoMenuVSize	19
#define Logo4CFontStart   		0x1FE
#elif LOGO_AV
#define LogoMenuHSize 	68
#define LogoMenuVSize	34
#define Logo4CFontStart   		0x1FE
#elif LOGO_NIXEUS
#define LogoMenuHSize 	85
#define LogoMenuVSize	11
#define Logo4CFontStart   		0x170
#elif LOGO_IMAGIC
#define LogoMenuHSize 	55
#define LogoMenuVSize	13
#define Logo4CFontStart   		0x1FE
#elif LOGO_HYUNDAI
#define LogoMenuHSize 	96
#define LogoMenuVSize	19
#define Logo4CFontStart   		0x1FE
#elif LOGO_DMM
#define LogoMenuHSize 	98
#define LogoMenuVSize	10
#define Logo4CFontStart   		0x1FE
#elif LOGO_VMazon
#define LogoMenuHSize 	93
#define LogoMenuVSize	12
#define Logo4CFontStart   		0x1FE
#elif LOGO_Titan
#define LogoMenuHSize 	69
#define LogoMenuVSize	26
#define Logo4CFontStart   		0x1FE
#elif LOGO_IIP
#define LogoMenuHSize 	68
#define LogoMenuVSize	26
#define Logo4CFontStart   		0x1FE
#elif LOGO_ForGame
#define LogoMenuHSize 	99
#define LogoMenuVSize	7
#define Logo4CFontStart   		0x1FE
#elif LOGO_GREATWALL
#define LogoMenuHSize 	82
#define LogoMenuVSize	8
#define Logo4CFontStart   		0x1FE
#elif LOGO_keoushi
#define LogoMenuHSize 	74
#define LogoMenuVSize	11
#define Logo4CFontStart   		0x1F0
#elif LOGO_ViewSync
#define LogoMenuHSize 	79
#define LogoMenuVSize	18
#define Logo4CFontStart   		0x1F0
#elif LOGO_GameMNT
#define LogoMenuHSize 	70
#define LogoMenuVSize	18
#define Logo4CFontStart   		0x1F0
#elif LOGO_SaioSone
#define LogoMenuHSize 	103
#define LogoMenuVSize	6
#define Logo4CFontStart   		0x1F0
#elif LOGO_Sceptre
#define LogoMenuHSize 	97
#define LogoMenuVSize	9
#define Logo4CFontStart   		0x1F0
#elif LOGO_TCL
#define LogoMenuHSize 	32
#define LogoMenuVSize	19
#define Logo4CFontStart   		0x1F0
#elif LOGO_Sceptre_New
#define LogoMenuHSize 	132//136
#define LogoMenuVSize	11//12
#define Logo4CFontStart   		0x1F0

#else
#define LogoMenuHSize 	32
#define LogoMenuVSize	4
#define Logo4CFontStart   		0x100
#endif




#if USE_ADJUSTABLE_MENU
typedef struct
{
#if (ENABLE_MSTV_UART_DEBUG && MENU_TBL_CHECK)
    BYTE u8Check; ///for function ptr 1st BYTE check
#endif
    BYTE u8XValue, u8YValue;
} ConstXYType;

#if (ENABLE_MSTV_UART_DEBUG && MENU_TBL_CHECK)
typedef BYTE  (*GetXYValue)(BYTE u8Flag);       //3 byts pointer, 1st byte=0xFF
#else
typedef BYTE  (code *GetXYValue)(BYTE u8Flag);  //2 byte pointer
#endif
typedef union
{
    GetXYValue fpFunc;  ///This must be 1st member of union, inifializer just using the 1st member's format.
    ConstXYType stCXY;
} PositionType;

#define GET_DN_XPOS(DrawNumber)             ((DrawNumber.u8Flags&NUM_FUNC) ? DrawNumber.unPos.fpFunc(0) : DrawNumber.unPos.stCXY.u8XValue)
#define GET_DN_YPOS(DrawNumber)             ((DrawNumber.u8Flags&NUM_FUNC) ? DrawNumber.unPos.fpFunc(1) : DrawNumber.unPos.stCXY.u8YValue)
#define PTR_GET_DN_XPOS(pstDrawNumber)      (((pstDrawNumber)->u8Flags&NUM_FUNC) ? (pstDrawNumber)->unPos.fpFunc(0) : (pstDrawNumber)->unPos.stCXY.u8XValue)
#define PTR_GET_DN_YPOS(pstDrawNumber)      (((pstDrawNumber)->u8Flags&NUM_FUNC) ? (pstDrawNumber)->unPos.fpFunc(1) : (pstDrawNumber)->unPos.stCXY.u8YValue)

#define GET_DG_XPOS(DrawGauge)              ((DrawGauge.u8Flags&BAR_FUNC) ? DrawGauge.unPos.fpFunc(0) : DrawGauge.unPos.stCXY.u8XValue)
#define GET_DG_YPOS(DrawGauge)              ((DrawGauge.u8Flags&BAR_FUNC) ? DrawGauge.unPos.fpFunc(1) : DrawGauge.unPos.stCXY.u8YValue)
#define PTR_GET_DG_XPOS(pstDrawGauge)       (((pstDrawGauge)->u8Flags&BAR_FUNC) ? (pstDrawGauge)->unPos.fpFunc(0) : (pstDrawGauge)->unPos.stCXY.u8XValue)
#define PTR_GET_DG_YPOS(pstDrawGauge)       (((pstDrawGauge)->u8Flags&BAR_FUNC) ? (pstDrawGauge)->unPos.fpFunc(1) : (pstDrawGauge)->unPos.stCXY.u8YValue)

#define GET_DT_XPOS(DrawRadioTxt)           ((DrawRadioTxt.u8Flags&TXT_FUNC) ? DrawRadioTxt.unPos.fpFunc(0) : DrawRadioTxt.unPos.stCXY.u8XValue)
#define GET_DT_YPOS(DrawRadioTxt)           ((DrawRadioTxt.u8Flags&TXT_FUNC) ? DrawRadioTxt.unPos.fpFunc(1) : DrawRadioTxt.unPos.stCXY.u8YValue)
#define PTR_GET_DT_XPOS(pstDrawRadioTxt)    (((pstDrawRadioTxt)->u8Flags&TXT_FUNC) ? (pstDrawRadioTxt)->unPos.fpFunc(0) : (pstDrawRadioTxt)->unPos.stCXY.u8XValue)
#define PTR_GET_DT_YPOS(pstDrawRadioTxt)    (((pstDrawRadioTxt)->u8Flags&TXT_FUNC) ? (pstDrawRadioTxt)->unPos.fpFunc(1) : (pstDrawRadioTxt)->unPos.stCXY.u8YValue)

#define GET_DI_XPOS(DrawIcon)               ((DrawIcon.u8Flags&ICO_FUNC) ? DrawIcon.unPos.fpFunc(0) : DrawIcon.unPos.stCXY.u8XValue)
#define GET_DI_YPOS(DrawIcon)               ((DrawIcon.u8Flags&ICO_FUNC) ? DrawIcon.unPos.fpFunc(1) : DrawIcon.unPos.stCXY.u8YValue)
#define PTR_GET_DI_XPOS(pstDrawIcon)        (((pstDrawIcon)->u8Flags&ICO_FUNC) ? (pstDrawIcon)->unPos.fpFunc(0) : (pstDrawIcon)->unPos.stCXY.u8XValue)
#define PTR_GET_DI_YPOS(pstDrawIcon)        (((pstDrawIcon)->u8Flags&ICO_FUNC) ? (pstDrawIcon)->unPos.fpFunc(1) : (pstDrawIcon)->unPos.stCXY.u8YValue)

#define GET_MI_XPOS(MenuItem)               ((MenuItem.u8TXTFlags&TXT_FUNC) ? MenuItem.unPos.fpFunc(0) : MenuItem.unPos.stCXY.u8XValue)
#define GET_MI_YPOS(MenuItem)               ((MenuItem.u8TXTFlags&TXT_FUNC) ? MenuItem.unPos.fpFunc(1) : MenuItem.unPos.stCXY.u8YValue)
#define PTR_GET_MI_XPOS(pstMenuItem)        (((pstMenuItem)->u8TXTFlags&TXT_FUNC) ? (pstMenuItem)->unPos.fpFunc(0) : (pstMenuItem)->unPos.stCXY.u8XValue)
#define PTR_GET_MI_YPOS(pstMenuItem)        (((pstMenuItem)->u8TXTFlags&TXT_FUNC) ? (pstMenuItem)->unPos.fpFunc(1) : (pstMenuItem)->unPos.stCXY.u8YValue)


#define GET_MENU_XSIZE(MenuPage)            ((MenuPage.u8Flags&MPB_FUNC) ? MenuPage.unPos.fpFunc(0) : MenuPage.unPos.stCXY.u8XValue)
#define GET_MENU_YSIZE(MenuPage)            ((MenuPage.u8Flags&MPB_FUNC) ? MenuPage.unPos.fpFunc(1) : MenuPage.unPos.stCXY.u8YValue)
#define PTR_GET_MENU_XSIZE(pstMenuPage)     (((pstMenuPage)->u8Flags&MPB_FUNC) ? (pstMenuPage)->unPos.fpFunc(0) : (pstMenuPage)->unPos.stCXY.u8XValue)
#define PTR_GET_MENU_YSIZE(pstMenuPage)     (((pstMenuPage)->u8Flags&MPB_FUNC) ? (pstMenuPage)->unPos.fpFunc(1) : (pstMenuPage)->unPos.stCXY.u8YValue)

#if 1
#define COORD(x,y)  {(GetXYValue)((WORD)(x)+((y)<<8))}
#define SIZE(x,y)   {(GetXYValue)((WORD)(x)+((y)<<8))}
#else
#define COORD(x,y)  {(GetXYValue)(((WORD)(x)<<8)+(y))}
#define SIZE(x,y)   {(GetXYValue)(((WORD)(x)<<8)+(y))}
#endif
#define FUNC(f)     {f}

#else
#define GET_DN_XPOS(DrawNumber)             (DrawNumber.u8XPos)
#define GET_DN_YPOS(DrawNumber)             (DrawNumber.u8YPos)
#define PTR_GET_DN_XPOS(pstDrawNumber)      ((pstDrawNumber)->u8XPos)
#define PTR_GET_DN_YPOS(pstDrawNumber)      ((pstDrawNumber)->u8YPos)

#define GET_DG_XPOS(DrawGauge)              (DrawGauge.u8XPos)
#define GET_DG_YPOS(DrawGauge)              (DrawGauge.u8YPos)
#define PTR_GET_DG_XPOS(pstDrawGauge)       ((pstDrawGauge)->u8XPos)
#define PTR_GET_DG_YPOS(pstDrawGauge)       ((pstDrawGauge)->u8YPos)

#define GET_DT_XPOS(DrawRadioTxt)           (DrawRadioTxt.u8XPos)
#define GET_DT_YPOS(DrawRadioTxt)           (DrawRadioTxt.u8YPos)
#define PTR_GET_DT_XPOS(pstDrawRadioTxt)    ((pstDrawRadioTxt)->u8XPos)
#define PTR_GET_DT_YPOS(pstDrawRadioTxt)    ((pstDrawRadioTxt)->u8YPos)

#define GET_DI_XPOS(DrawIcon)               (DrawIcon.u8XPos)
#define GET_DI_YPOS(DrawIcon)               (DrawIcon.u8YPos)
#define PTR_GET_DI_XPOS((pstDrawIcon))      ((pstDrawIcon)->u8XPos)
#define PTR_GET_DI_YPOS((pstDrawIcon))      ((pstDrawIcon)->u8YPos)

#define GET_MI_XPOS(MenuItem)               (MenuItem.u8XPos)
#define GET_MI_YPOS(MenuItem)               (MenuItem.u8YPos)
#define PTR_GET_MI_XPOS(pstMenuItem)        ((pstMenuItem)->u8XPos)
#define PTR_GET_MI_YPOS(pstMenuItem)        ((pstMenuItem)->u8YPos)

#define GET_MENU_XSIZE(MenuPage)            (MenuPage.u8XSize)
#define GET_MENU_YSIZE(MenuPage)            (MenuPage.u8YSize)
#define PTR_GET_MENU_XSIZE(pstMenuPage)     ((pstMenuPage)->u8XSize)
#define PTR_GET_MENU_YSIZE(pstMenuPage)     ((pstMenuPage)->u8YSize)

#define COORD(x,y)  x,y
#define SIZE(x,y)   x,y
#endif

typedef struct
{ fpAdjustValue AdjustFunction;
  fpExecFunc ExecFunction;
} MenuItemFuncType;

#if MULTI_NUMBER //multi-number

typedef struct
{ WORD u8Flags;
  BYTE u8XPos, u8YPos,
  fpGetValue GetValue;
} NumberType;

typedef struct
{ BYTE u8Flags;
  BYTE u8Color;
  NumberType *pstDisplayNumber;
} DrawNumberType;

#else // single number

typedef struct
{
    WORD u8Flags;
    BYTE u8Color;
#if USE_ADJUSTABLE_MENU
    PositionType unPos;
#else
    BYTE u8XPos, u8YPos;
#endif
    fpGetValue GetValue;
#if USE_PROP_NUMBER
    BYTE u8PropNumAddr;//The Menory address to put the PropNumber Font.
#endif
} DrawNumberType;
#endif

#if MULTI_GAUGE // multi-gauge

typedef struct
{ WORD u8Flags;
  BYTE u8XPos, u8YPos;
  fpGetValue GetValue;
} GaugeType;
typedef struct
{ BYTE u8Flags;
  BYTE u8Color;
  BYTE u8Length;
  GaugeType *pstDisplayGauge;
} DrawGaugeType;

#else // single gauge

typedef struct
{
    WORD u8Flags;
    BYTE u8Color;
    BYTE u8Length;
    #if USE_ADJUSTABLE_MENU
    PositionType unPos;
    #else
    BYTE u8XPos, u8YPos;
    #endif
    fpGetValue GetValue;
} DrawGaugeType;
#endif

typedef struct
{ WORD u8Flags;
#if (USE_ADJUSTABLE_MENU)
    PositionType unPos;
#else
  BYTE u8XPos, u8YPos;
#endif
  fpDisplayText DisplayText;
} RadioTextType;

typedef struct
{// BYTE u8Flags;
  BYTE u8Color;
  BYTE u8SelColor;
  fpGetValue GetUserPrefOption;
  RadioTextType *pstRadioText;
} DrawRadioGroupType;

typedef BYTE* (*fpGetIcon)(void);

typedef struct
{
    WORD u8Flags;
#if USE_ADJUSTABLE_MENU
    PositionType unPos;
#else
    BYTE u8XPos,u8YPos;
#endif
    fpGetIcon GetIcon;
} DrawIconType;

typedef struct
{ DrawNumberType *pstDrawNumber;
  DrawGaugeType *pstDrawGauge;
  DrawRadioGroupType *pstDrawRadioGroup;
  DrawIconType *pstDrawIcons;
} DrawValueType;

typedef struct
{
#if USE_ADJUSTABLE_MENU
    PositionType unPos;
#else
    BYTE u8XPos, u8YPos;
#endif
    BYTE u8Color;
    BYTE u8SelColor;
    BYTE u8NextMenuPage;
    WORD u8TXTFlags;
    fpDisplayText   DisplayText;
    MenuItemActionType *penKeyEvents;
    MenuItemFuncType stKeyFunction;
    DrawValueType stDisplayValue;
    WORD u16Flags;
} MenuItemType;

typedef struct
{ BYTE *pu8Fonts;
  WORD u16FontCount;
} MenuFontType;

#if 1
typedef WORD PropFontType;
#else
typedef struct
{ BYTE u8SpaceWidth;
  WORD tLineData[18];
} PropFontType;
#endif

typedef struct
{
#if USE_ADJUSTABLE_MENU
    PositionType unPos;
#else
    BYTE    u8XSize, u8YSize;
#endif
  BYTE u8PrevMenuPage;
  MenuItemType* pstMenuItems;
  BYTE  u8MenuItemCount;
  fpExecFunc ExecFunction;  /// Run this function after showing menu.
  fpLoadFont LoadFont;  ///Dynamic load font before showing menu.
  WORD u8Flags;
} MenuPageType;


///////////////////////////////////////////////////////////////////////////////////////
//
//Memory Mapping Common Use - mono
//
///////////////////////////////////////////////////////////////////////////////////////
#define COMMON_FONT_START               0x1EA //9 fonts   //static
#define PROP_NUM_START                     0x36//0x0B    // at least 5 number position needed
#define PROP_NUM_LEN                          3   //09+(3*5)=18h


#define MAIN_Top_Left                      (COMMON_FONT_START+0x00)
#define MAIN_Top_Mid                       (COMMON_FONT_START+0x01)
#define MAIN_Top_Right                    (COMMON_FONT_START+0x02)

#define MAIN_Bottom_Left                 (COMMON_FONT_START+0x06)
#define MAIN_Bottom_Mid                  (COMMON_FONT_START+0x07)
#define MAIN_Bottom_Right               (COMMON_FONT_START+0x08)

#define MAIN_Top_Line                     (COMMON_FONT_START+0x03)

#define MAIN_Mid_Line                     (COMMON_FONT_START+0x09)

#define MAIN_Left_Line                     (COMMON_FONT_START+0x04)
#define MAIN_Right_Line                     (COMMON_FONT_START+0x05)


#define MM_Top_Left                         (COMMON_FONT_START+0x0F)
#define MM_Top_Mid                          (COMMON_FONT_START+0x10)
#define MM_Top_Right                        (COMMON_FONT_START+0x11)
#define MM_Bottom_Left                     (COMMON_FONT_START+0x12)
#define MM_Bottom_Mid                     (COMMON_FONT_START+0x13)
#define MM_Bottom_Right                   (COMMON_FONT_START+0x14)

#define MM_Left_Mid                          (COMMON_FONT_START+0x0B)
#define MM_Rightt_Mid                      (COMMON_FONT_START+0x0A)

#define ARROW_LEFT                         (COMMON_FONT_START+0x0D)
#define ARROW_RIGHT                       (COMMON_FONT_START+0x0E)



#define MS_Font_Start                      0x100

#define Source_Select_Addr             MS_Font_Start
#define DVI_Addr                              (Source_Select_Addr+38)  
#define HDMI_Addr                            (DVI_Addr+11)  
#if ENABLE_HDMI2ND_INPUT
#define HDMI2_Addr                            (HDMI_Addr+20)//17  
#define DP_Addr                                  (HDMI2_Addr+20)  
#else
#define DP_Addr                                (HDMI_Addr+17)  
#endif
///////////////////////////////////////////////////////////////////////////////////////
//Memory Mapping under 0x100 - mono
///////////////////////////////////////////////////////////////////////////////////////
#define MainString2Color_Start           0x57
//Mono
#define PROP_FONT_START                    0x100
#define S0_PROP_FONT_START              PROP_FONT_START //for factory menu compatiable wit dual OSD engine

#define Description_PROP_FONT_START       0x01

#define HotKey_ICON_Addr                         0x100

#define HotKey_Led_ICON_Addr                  0x160

// for Other Settings Menu-current language string
// This menu use 2 number, so it is safe to use the rest of number font ram.
#define CURRENT_LANGUAGE_START          (PROP_FONT_START-10)
//Other Settings end=0xC6
#define OS_L2_PROP_FONT_START           0x8D //101122 for "Energy Smart"

#define Common_PROP_FONT_START          0x200

#define Menu_Scetpre_Logo                    0x232//x230

#define Menu_Percent_Start                    0x22D

#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define MM_MW1_INPUT_SOURCE_TEXT_START  0xD0//0xBC
#define MM_MW2_INPUT_SOURCE_TEXT_START  0xD7//0xC3
#define MM_MW3_INPUT_SOURCE_TEXT_START  0xDE//0xCA
#define MM_MW4_INPUT_SOURCE_TEXT_START  0xE5//0xD1
#else
#define MM_INPUT_SOURCE_TEXT_START      0xF9//0xCB//0xC7         //Steven110525
#endif
//#define MM_WEB_SITE_TEXT_START          0xD0         //Steven110525

//#define SM_OPT_RESOLUTION_START         0xCF//for "Optimun Resolution:" string
//#define SM_MODEL_NAME_START             0xE8
//#define MM_VERSION_START                0xF0
//#define MM_PANEL_TYPE_START             0xF5

//Static
#define SM_CHECK_2C_FONT_START          0xDB//0xFA//0xFE //2 fonts

///////////////////////////////////////////////////////////////////////////////////////
//Memory Mapping between 0x100 ~ 0x200 - mono
///////////////////////////////////////////////////////////////////////////////////////
#define MM_GUIDE_FONT_START              (0x20+0x100)    //L0 prop font end=8Dh


#if Enable_Corss_LHCX_New
#define HotGameFrameFONT_2COLOR_ICON_START  0x30//0x01
#define HotGameFONT_2COLOR_ICON_START 	(0x30+0x14)
#define HotGameFONT_4COLOR_ICON_START 	0x40  //
#define HotGameClokNumberShift		4
#endif

#define MM_PROP_RESOLUTION_INFO_START   (0x20)    //Current timing
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define MW1_PROP_RESOLUTION_INFO_START  0x30    //Multi-Win1 current timing
#define MW2_PROP_RESOLUTION_INFO_START  0x40    //Multi-Win2 current timing
#define MW3_PROP_RESOLUTION_INFO_START  0x50    //Multi-Win3 current timing
#define MW4_PROP_RESOLUTION_INFO_START  0x60    //Multi-Win4 current timing
#else
#define MW1_PROP_RESOLUTION_INFO_START  0x20    //Multi-Win1 current timing
#endif
///////////////////////////////////////////////////////////////////////////////////////
//Memory Mapping over 0x200 - color
///////////////////////////////////////////////////////////////////////////////////////
//4 color
    //Main Menu icons - static

#define MM_2COLOR_CURSOR_START            0x72//0x70

#define MM_2COLOR_Picture_Start               (MM_2COLOR_CURSOR_START+16) 
#define MM_2COLOR_Color_Start               (MM_2COLOR_Picture_Start+12) 
#define MM_2COLOR_System_Start               (MM_2COLOR_Color_Start+14) 

#define MM_4COLOR_CURSOR_START            0xA8    //200h
#define MM_4COLOR_Icon_START                 (MM_4COLOR_CURSOR_START+10*2)


// main menu icons (Dynamic)

//8 color
#define MM_8COLOR_ICON_START            0xF4
#define MM_8COLOR_FONT_CNT              18

#define _4COLOR_FONT_START              MM_4COLOR_CURSOR_START  //0x200
#define _8COLOR_FONT_START              MM_8COLOR_ICON_START    //0x280

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define MSG_MODEL_NAME_START            SM_MODEL_NAME_START

//#define MSG_WARNING_MSG_START           SM_OPT_RESOLUTION_START//=0xCF    //max. len=0x11 //Jison 080408
//#define MSG_OOR_TIMING_START            SM_OPT_TIMING_START


#if (ODM_NAME==ODM_WISTRON)
#define FM_TEXT0_START                  0xC2 //0xC0//Sky100208 //for Wistron Factory Serial number
#else
#define FM_TEXT0_START                  0xC0
#endif
#define FM_TEXT1_START                  0xD0



//-----Factory Font Address------------------------------

#define FONT_FACTORY_SPACE 					0x20
#define FONT_FACTORY_DOT_AND_DASH 			0x2D
#define FONT_FACTORY_NUMBER_START		 	0x30
#define FONT_FACTORY_ENGLISH_BIG_START 		0x41
#define FONT_FACTORY_ENGLISH_SMALL_START 	0x61

//-----Font Address------------------------------------


//#define MM_AUTO_ICON_8C_START       (MM_8COLOR_ICON_START)// 3 in total
//#define MM_COLOR_ICON_8C_START      (MM_AUTO_ICON_8C_START+3*3)// 1 in total //Color Settings
//#define MM_AUDIO_ICON_8C_START      (MM_COLOR_ICON_8C_START+3)// 1 in total //Audio Settings
//#define MM_OTHER_ICON_8C_START      (MM_AUDIO_ICON_8C_START+3)// 1 in total //Audio Settings

//---------------------------------------------------------------------
//Icons address mapping
//#define MSG_NO_ICON_TOP_START       MSG_4COLOR_ICON_START
//#define MSG_NO_ICON_DOWN_START      (MSG_NO_ICON_TOP_START+12)
//#define MSG_YES_ICON_TOP_START      (MSG_NO_ICON_DOWN_START+12)
//#define MSG_YES_ICON_DOWN_START     (MSG_YES_ICON_TOP_START+12)
//#define MSG_YN_ICON_EXTEND_START    (MSG_YES_ICON_DOWN_START+12) //Jison 101115
//#define MSG_ICON_XSIZE              10

//MM Cursor Address(4Colors)
#define MM_CURSOR_TL            (MM_4COLOR_CURSOR_START)
#define MM_CURSOR_TOP           (MM_CURSOR_TL+2)
#define MM_CURSOR_TR            (MM_CURSOR_TOP+2)
#define MM_CURSOR_BL            (MM_CURSOR_TR+2)
#define MM_CURSOR_BUTTOM        (MM_CURSOR_BL+2)
#define MM_CURSOR_BR            (MM_CURSOR_BUTTOM+2)
#define MM_CURSOR_L             (MM_CURSOR_BR+2)
#define MM_CURSOR_R             (MM_CURSOR_L+2)
//SM Cursor Address
#define SM_CURSOR_TL            MM_CURSOR_TL
#define SM_CURSOR_TOP           MM_CURSOR_TOP
#define SM_CURSOR_TR            MM_CURSOR_TR
#define SM_CURSOR_BL            MM_CURSOR_BL
#define SM_CURSOR_BUTTOM        MM_CURSOR_BUTTOM
#define SM_CURSOR_BR            MM_CURSOR_BR
#define SM_CURSOR_L             MM_CURSOR_L
#define SM_CURSOR_R             MM_CURSOR_R

//#define HM_SAPERATION_TOP                   (HM_CUSTOM_RGB_4COLOR_FONT_START)
//#define HM_SAPERATION_CENTER                (HM_CUSTOM_RGB_4COLOR_FONT_START+2)
//#define HM_SAPERATION_DOWN                  (HM_CUSTOM_RGB_4COLOR_FONT_START+4)

#define BTN_SL_FONT_CNT                 0
//  Button OSD size
#define HBTN_OSD_XSIZE  0
#define HBTN_OSD_YSIZE  0
#define VBTN_OSD_XSIZE  0
#define VBTN_OSD_YSIZE  0
#define SPACE_NORMAL    0
#define SPACE_ROTATE    0


//WHOLE FRAME(EG0+EG1)
#define WF_XWIDTH_L0        5
#define WF_XWIDTH_L1        15


#define WF_XWIDTH_L2        21 //24  //Steven110601
#define WF_XWIDTH           (WF_XWIDTH_L0+WF_XWIDTH_L1+WF_XWIDTH_L2)

#if 0
#if (INPUT_TYPE > (INPUT_1A|INPUT_5C))
#define WF_YHEIGHT          25
#elif (!ENABLE_MULTI_WINDOW_SETTINGS_MENU && ENABLE_RTE && ENABLE_WIDE_MODE)
#define WF_YHEIGHT          23
#elif (ENABLE_MULTI_WINDOW_SETTINGS_MENU)
    #if ENABLE_AUDIO_SETTINGS_MENU
    #define WF_YHEIGHT      29
    #else
    #define WF_YHEIGHT          27
    #endif
#else
#define WF_YHEIGHT          21
#endif
#else
#define WF_YHEIGHT          20//29
#endif

//MAIN MENU(EG0)
#define MM_XWIDTH_L0        WF_XWIDTH_L0
//#define MM_XWIDTH_L1        WF_XWIDTH_L1
//#define MM_XWIDTH_L2        WF_XWIDTH_L2
#define MM_XSTART_L0        4
//SUB MENU(EG1)
#define MM_XSTART_L1        WF_XWIDTH_L0
#define MM_XSTART_L2        (MM_XSTART_L1+WF_XWIDTH_L1)
#define MM_XWIDTH_L1        WF_XWIDTH_L1
#define MM_XWIDTH_L2        WF_XWIDTH_L2-1
#define MM_OPT_RES_XSTART   (MM_XWIDTH_L1+MM_XWIDTH_L2-12-12-1)
#define MM_XSIZE            32
#define MM_YSIZE            60

//SUB MENU SIZE
#define SM_XSIZE            21
#define SM_YSIZE            6

#define MM_HEADER_YHEIGHT	4
#if ENABLE_MULTI_WINDOW_SETTINGS_MENU
#define MM_BOTTOM_YHEIGHT   9//10
#else
#define MM_BOTTOM_YHEIGHT   6
#endif

#define GAUGE_HEIGHT        (SM_YSIZE-4)//15  //Must be odd for showing number in the center
#define GAUGE_YSTART        5

//Hot Menu
#define HM_ITEM_WIDTH       12
#define HM_ITEM_HEIGHT      (GAUGE_HEIGHT+4)
#define HM_HEIGHT           (HM_ITEM_HEIGHT+2)


#define Gauge_XPos              13//12//10//11
#define Gauge_Height           9//10
#define Number_XPos           25
#define GroupText_XPos      Number_XPos



#define AdjustMenuHSize     24
#define AdjustMenuVSize     3

#define AdjustGaugeLenght           25

#define AdjustTextXPos      1
#define AdjustTextYPos      1

#define AdjustGaugeXPos     7
#define AdjustGaugeYPos     1

#define AdjustNumXPos       21
#define AdjustNumYPos       1

#define TitleTextXPos               0
#define TitleTextYPos               1

#define LedColorTextXPos               36
#define LedColorTextYPos               6//8

#define LedColorTextShiftXPos         4

//
#define Main_TopLine_YPos           6
#define Main_Description_YPos     47
#define Main_Buttom_YPos            57

#define Sub_MidLine_YPos            34

#define STATUS_XSIZE        28//30
#define STATUS_XSTART       0//10
#define MSG_XSIZE           48//58
#define MSG_XSTART          0//10
#define FLY_WND_XSIZE       47

#if 1  //Steven110525
#define LOGO_2C_ICON_DOWN_CNT   95
#define LOGO_2C_START           0x4D
//over256
#define LOGO_4C_ICON_TOP_CNT    64
#define LOGO_4C_START           0
//over512  //Steven110526
#define MENU_LOGO_4C_ICON_CNT        35
#define MENU_LOGO_4C_START           0x70
#endif

#if (SIMPLY_CHINESE)
#define IS_ASIA_LANGUAGE() (USER_PREF_LANGUAGE==LANG_SChinese)
#endif



#if(MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1) 
typedef enum
{
#if Enable_AutoInput_Menu
SOURCE_AUTO_SWITCH_IDX,
#endif

#if ENABLE_VGA_INPUT
SOURCE_VGA_IDX,
#endif

#if ENABLE_DP_INPUT
SOURCE_Displayport1_IDX,
#if ENABLE_DP2ND_INPUT
SOURCE_Displayport2_IDX,
#endif
#endif

#if ENABLE_HDMI_INPUT
SOURCE_HDMI1_IDX,
#if ENABLE_HDMI2ND_INPUT
SOURCE_HDMI2_IDX,
#endif
#if ENABLE_HDMI3ND_INPUT
SOURCE_HDMI3_IDX,
#endif
#endif

#if ENABLE_DVI_INPUT
SOURCE_DVI_IDX,
#if ENABLE_DVI2ND_INPUT
SOURCE_DVI2_IDX,
#endif
#if ENABLE_DVI3ND_INPUT
SOURCE_DVI3_IDX,
#endif
#endif

#if USB_Updata_FW_Enable
SOURCE_USB_UPDATA,
#endif

SOURCE_Source_MAX_IDX,
} SourceMenuItemIndexType;

#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
typedef enum
{
#if Enable_AutoInput_Menu
SOURCE_AUTO_SWITCH_IDX,
#endif
#if ENABLE_DP_INPUT
SOURCE_Displayport1_IDX,
#if ENABLE_DP2ND_INPUT
SOURCE_Displayport2_IDX,
#endif
#endif

#if ENABLE_VGA_INPUT
SOURCE_VGA_IDX,
#endif
#if ENABLE_DVI_INPUT
SOURCE_DVI_IDX,
#if ENABLE_DVI2ND_INPUT
SOURCE_DVI2_IDX,
#endif
#if ENABLE_DVI3ND_INPUT
SOURCE_DVI3_IDX,
#endif

#endif
#if ENABLE_HDMI_INPUT
SOURCE_HDMI1_IDX,
#if ENABLE_HDMI2ND_INPUT
SOURCE_HDMI2_IDX,
#endif
#if ENABLE_HDMI3ND_INPUT
SOURCE_HDMI3_IDX,
#endif
#endif

#if USB_Updata_FW_Enable
SOURCE_USB_UPDATA,
#endif

SOURCE_Source_MAX_IDX,
} SourceMenuItemIndexType;

#else
typedef enum
{
#if Enable_AutoInput_Menu
SOURCE_AUTO_SWITCH_IDX,
#endif

#if ENABLE_VGA_INPUT
SOURCE_VGA_IDX,
#endif
#if ENABLE_DVI_INPUT
SOURCE_DVI_IDX,
#if ENABLE_DVI2ND_INPUT
SOURCE_DVI2_IDX,
#endif
#if ENABLE_DVI3ND_INPUT
SOURCE_DVI3_IDX,
#endif

#endif
#if ENABLE_HDMI_INPUT
SOURCE_HDMI1_IDX,
#if ENABLE_HDMI2ND_INPUT
SOURCE_HDMI2_IDX,
#endif
#if ENABLE_HDMI3ND_INPUT
SOURCE_HDMI3_IDX,
#endif
#endif
#if ENABLE_DP_INPUT
SOURCE_Displayport1_IDX,
#if ENABLE_DP2ND_INPUT
SOURCE_Displayport2_IDX,
#endif
#endif

#if USB_Updata_FW_Enable
SOURCE_USB_UPDATA,
#endif

SOURCE_Source_MAX_IDX,
} SourceMenuItemIndexType;
#endif


typedef enum
{
  Main_Quick_Start_IDX,
  Main_Picture_IDX,
  Main_Color_IDX,
  Main_System_IDX,

  Main_Max_IDX,
} MainMenuItemIndexType;

typedef enum
{
  QuickStart_BackLight_IDX,
  QuickStart_Brightness_IDX,
  QuickStart_Contrast_IDX,
  QuickStart_Preset_IDX,
  QuickStart_Aspect_IDX,
  QuickStart_Source_IDX,
  QuickStart_BlueLight_IDX,
  #if Free_Sync_Enable
  QuickStart_FreeSync_IDX,
  #endif

  QuickStart_Max_IDX,
} QuickStartMenuItemIndexType;

typedef enum
{
  QS_BackLight_IDX,
  QS_BackLight_Adj_IDX,
  QS_Brightness_IDX,
  QS_Brightness_Adj_IDX,
  QS_Contrast_IDX,
  QS_Contrast_Adj_IDX,
  QS_Preset_IDX,
  QS_Preset_Adj_IDX,
  QS_Aspect_IDX,
  QS_Aspect_Adj_IDX,
  QS_Source_IDX,
  QS_Source_Adj_IDX,
  QS_BlueLight_IDX,
  QS_BlueLight_Adj_IDX,
  #if Free_Sync_Enable
  QS_FreeSync_IDX,
  QS_FreeSync_Adj_IDX,
  #endif
  QS_Max_IDX,
} QSMenuItemIndexType;

typedef enum
{
  Picture_BackLight_IDX,
  Picture_Brightness_IDX,
  Picture_Contrast_IDX,
  Picture_Preset_IDX,
  Picture_DCR_IDX,
  Picture_Aspect_IDX,
  Picture_Sharpness_IDX,
#if ENABLE_VGA_INPUT
  Picture_AutoAdjust_IDX,
//  Picture_AutoColor_IDX,
  Picture_HPosition_IDX,
  Picture_VPosition_IDX,
  Picture_Clock_IDX,
  Picture_Phase_IDX,
#endif
  Picture_Max_IDX,
} PictureMenuItemIndexType;

typedef enum
{
  Pic_BackLight_IDX,
  Pic_BackLight_Adj_IDX,
  Pic_Brightness_IDX,
  Pic_Brightness_Adj_IDX,
  Pic_Contrast_IDX,
  Pic_Contrast_Adj_IDX,
  Pic_Preset_IDX,
  Pic_Preset_Adj_IDX,
  Pic_DCR_IDX,
  Pic_DCR_Adj_IDX,
  Pic_Aspect_IDX,
  Pic_Aspect_Adj_IDX,
  Pic_Sharpness_IDX,
  Pic_Sharpness_Adj_IDX,

#if ENABLE_VGA_INPUT
  Pic_AutoAdjust_IDX,
  Pic_AutoAdjust_Adj_IDX,
  Pic_AutoColor_IDX,
  Pic_AutoColor_Adj_IDX,
  Pic_HPosition_IDX,
  Pic_HPosition_Adj_IDX,
  Pic_VPosition_IDX,
  Pic_VPosition_Adj_IDX,
  Pic_Clock_IDX,
  Pic_Clock_Adj_IDX,
  Pic_Phase_IDX,
  Pic_Phase_Adj_IDX,
#endif
  Pic_Max_IDX,
} PicMenuItemIndexType;


typedef enum
{
  Color_Temperature_IDX,
  Color_Gamma_IDX,
  Color_Tint_IDX,
  Color_Saturation_IDX,
  Color_BlueLight_IDX,

  Color_Max_IDX,
} ColorMenuItemIndexType;

typedef enum
{
  Col_Temperature_IDX,
  Col_Temperature_Adj_IDX,
  Col_Gamma_IDX,
  Col_Gamma_Adj_IDX,
  Col_Tint_IDX,
  Col_Tint_Adj_IDX,
  Col_Saturation_IDX,
  Col_Saturation_Adj_IDX,
  Col_BlueLight_IDX,
  Col_BlueLight_Adj_IDX,

  Col_Max_IDX,
} ColMenuItemIndexType;

typedef enum
{
  User_Red_ItemIDX,
  User_Green_ItemIDX,
  User_Blue_ItemIDX,
  User_RGBCMY_ItemIDX,

  User_Max_ItemIDX,
} UserMenuItemIDXType;


typedef enum
{
  User_Red_IDX,
  User_Red_Adj_IDX,
  User_Green_IDX,
  User_Green_Adj_IDX,
  User_Blue_IDX,
  User_Blue_Adj_IDX,
  User_RGBCMY_IDX,
  User_RGBCMY_Adj_IDX,

  User_Max_IDX,
} UserMenuItemIndexType;



typedef enum
{
  Advanced_Red_ItemIDX,
  Advanced_Green_ItemIDX,
  Advanced_Blue_ItemIDX,
  Advanced_Cyan_ItemIDX,
  Advanced_Magenta_ItemIDX,
  Advanced_Yellow_ItemIDX,

  Advanced_Max_ItemIDX,
} AdvancedMenuItemIDXType;


typedef enum
{
  Advanced_Red_IDX,
  Advanced_Red_Adj_IDX,
  Advanced_Green_IDX,
  Advanced_Green_Adj_IDX,
  Advanced_Blue_IDX,
  Advanced_Blue_Adj_IDX,
  Advanced_Cyan_IDX,
  Advanced_Cyan_Adj_IDX,
  Advanced_Magenta_IDX,
  Advanced_Magenta_Adj_IDX,
  Advanced_Yellow_IDX,
  Advanced_Yellow_Adj_IDX,

  Advanced_Max_IDX,
} AdvancedMenuItemIndexType;




typedef enum
{
  System_OverDrive_IDX,
 #if Free_Sync_Enable
  System_FreeSync_IDX,
 #endif
  System_SleepMode_IDX,
#if LED_HOT_Menu_Func  
  System_LedMode_IDX,
#endif  
  System_Language_IDX,
  System_OSDHPosition_IDX,
#if Enable_OSD_Vposition  
  System_OSDVPosition_IDX,
#endif  
  System_OSDTimer_IDX,
  System_OSDTransparencia_IDX,
#if ENABLE_AUDIO_SETTINGS_MENU  
  System_Volume_IDX,
 #endif 
  System_Reset_IDX,

  System_Max_IDX,
} SystemMenuItemIndexType;

typedef enum
{
  Sys_OverDrive_IDX,
  Sys_OverDrive_Adj_IDX,
  #if Free_Sync_Enable
  Sys_FreeSync_IDX,
  Sys_FreeSync_Adj_IDX,
  #endif
  Sys_SleepMode_IDX,
  Sys_SleepMode_Adj_IDX,
#if LED_HOT_Menu_Func  
  Sys_LedMode_IDX,
  Sys_LedMode_Adj_IDX,
#endif  
  Sys_Language_IDX,
  Sys_Language_Adj_IDX,
  Sys_OSDHPosition_IDX,
  Sys_OSDHPosition_Adj_IDX,
#if Enable_OSD_Vposition  
  Sys_OSDVPosition_IDX,
  Sys_OSDVPosition_Adj_IDX,
#endif  
  Sys_OSDTimer_IDX,
  Sys_OSDTimer_Adj_IDX,
  Sys_OSDTransparencia_IDX,
  Sys_OSDTransparencia_Adj_IDX,
#if ENABLE_AUDIO_SETTINGS_MENU  
  Sys_Volume_IDX,
  Sys_Volume_Adj_IDX,
#endif  
  Sys_Reset_IDX,
  Sys_Reset_Adj_IDX,

  Sys_Max_IDX,
} SysMenuItemIndexType;



typedef enum
{
  Led_Mode_ItemIDX,
  Led_Color_ItemIDX,
  Led_Brightness_ItemIDX,

  Led_Max_ItemIDX,
} LedMenuItemIDXType;


typedef enum
{
  Led_Mode_IDX,
  Led_Mode_Adj_IDX,
  Led_Color_IDX,
  Led_Color_Adj_IDX,
  Led_Brigntness_IDX,
  Led_Brigntness_Adj_IDX,

  LED_Max_IDX,
} LedMenuItemIndexType;








typedef enum
{
  BriCon_Brightness_IDX,
  BriCon_Contrast_IDX,
  Color_Setting_Picture_IDX,
#if ENABLE_DCR
  BriCon_DCR_IDX,
#endif  

  BriCon_NR_IDX,

  BriCon_Max_IDX,
} BriConMenuItemIndexType;

typedef enum
{
  Color_Setting_ColorTemp_IDX,
  Color_Setting_User_R_IDX,
  Color_Setting_User_G_IDX,
  Color_Setting_User_B_IDX,
  Color_Setting_Max_IDX,
} ColorSettingMenuItemIndexType;

typedef enum
{
#if ENABLE_VGA_INPUT  	
  Display_HPosition_IDX,
  Display_VPosition_IDX,
  Display_Clock_IDX,
  Display_Phase_IDX,
  Display_Auto_IDX,
#endif  
  Display_Max_IDX,
} DisplaySettingMenuItemIndexType;


typedef enum
{
 AUDIOSOURCE_AUDIO_IN,
#if ENABLE_HDMI_INPUT 	
 AUDIOSOURCE_INDEX01,
#endif
#if ENABLE_HDMI2ND_INPUT
 AUDIOSOURCE_INDEX02,
#endif
#if ENABLE_HDMI3ND_INPUT
 AUDIOSOURCE_INDEX03,
#endif
#if ENABLE_DP_INPUT
 AUDIOSOURCE_INDEX04,
#endif 
#if ENABLE_DP2ND_INPUT 
 AUDIOSOURCE_INDEX05,
#endif 

 AUDIOSOURCE_INDEX_MAX,
#if !ENABLE_HDMI_INPUT 	
 AUDIOSOURCE_INDEX01,
#endif
#if !ENABLE_HDMI2ND_INPUT
 AUDIOSOURCE_INDEX02,
#endif
#if !ENABLE_HDMI3ND_INPUT
 AUDIOSOURCE_INDEX03,
#endif
#if !ENABLE_DP_INPUT
 AUDIOSOURCE_INDEX04,
#endif 
#if !ENABLE_DP2ND_INPUT 
 AUDIOSOURCE_INDEX05,
#endif 


} AUDIOSOURCE_INDEXType;



typedef enum
{
  Multi_Window_Mode_IDX,
  Multi_Window_Sub_Win2_IDX,
  Multi_Window_Sub_Win3_IDX,
#if Enable_Multi_Win4  
  Multi_Window_Sub_Win4_IDX,
#endif
  Multi_Window_PIP_Size_IDX,
  Multi_Window_PIP_Position_IDX,
  Multi_Window_Swap_IDX,
  
  Multi_Window_Max_IDX,
} Multi_WindowMenuItemIndexType;

typedef enum
{
  Setup_Input_IDX,
#if ENABLE_AUDIO_SETTINGS_MENU  	
  Setup_Mute_IDX,
  Setup_Volume_IDX,
#endif  
  Setup_AutoColor_IDX,
  Setup_AutoAdjustment_IDX,
  Setup_Reset_IDX,

  Setup_Max_IDX,
} SetupMenuItemIndexType;

typedef enum
{
  Others_DisplaySize_IDX,
#if (Low_Blue_Light_Enable||Enable_HotAdjLowbluray||Enable_Lowbluray_FY)  	
  Others_LowBlue_IDX,
#endif  
  Others_OD_IDX,
#if Free_Sync_Enable  
  Others_FreeSync_IDX,
#endif
  Others_Max_IDX,
} OthersMenuItemIndexType;



typedef enum
{
  Other_Language_IDX,
  Other_OSDHPosition_IDX,
  Other_OSDVPosition_IDX,

  
  Other_OSDTransparency_IDX,
  Other_OSDTimeOut_IDX,
  //Other_OSDRotation_IDX,
#if SleepMode_Enable
  Other_SleepMode_IDX,
#elif Enable_Corss_FY
  Other_CrossMode_IDX,
#endif
  Other_Reset_IDX,
 
  Other_Max_IDX,
} OtherMenuItemIndexType;



typedef enum
{
  HotLedColorIDX,
  HotLedStrengthIDX,
  HotLedColorModeIDX,
  HotLedColorModeCancelIDX,
  
  HotLedColorMaxIDX,
} HOTLedMenuItemIndexType;


#if Enable_Corss_LHCX_New
typedef enum _HotKeyGameMenuItems_
{
HotKeyGame_ModeItem,
HotKeyGame_ClockItem,
HotKeyGame_Nums,

}_HotKeyGameMenuItems_;

typedef enum _HotKeyGameClokMenuItems_
{
HotKeyGameClok_30MItem,
HotKeyGameClok_40MItem,
HotKeyGameClok_50MItem,
HotKeyGameClok_60MItem,
HotKeyGameClok_90MItem,

HotKeyGameClok_Nums,

}_HotKeyGameClokMenuItems_;

typedef enum _HotKeyGameClokPositionMenuItems_
{
HotKeyGameClokPosition_UpItem,
HotKeyGameClokPosition_CenterItem,
HotKeyGameClokPosition_DownItem,

HotKeyGameClokPosition_Nums,
HotKeyGameClokPosition_Close,
}_HotKeyGameClokPositionMenuItems_;
#endif



#if 1

#define strLanguageIndex1	strEnglishM6[0]
#define strLanguageIndex2	strKoreanM6[0]	
#define strLanguageIndex3	strChineseM6[0]	

#else
#define strLanguageIndex1	strEnglishM15[0]
#define strLanguageIndex2	strGermanM15[0]	
#define strLanguageIndex3	strSpanishM15[0]	
#define strLanguageIndex4	strGreekM15[0]	
#define strLanguageIndex5	strFrenchM15[0]	
#define strLanguageIndex6	strItalianM15[0]	
#define strLanguageIndex7	strHungarianM15[0]	
#define strLanguageIndex8	strDutchM15[0]	
#define strLanguageIndex9	strPortugueseM15[0]	
#define strLanguageIndex10	strBrazilPortugueseM15[0]	
#define strLanguageIndex11	strPolishM15	[0]
#define strLanguageIndex12	strRussianM15[0]	
#define strLanguageIndex13	strSwedishM15[0]	
#define strLanguageIndex14	strFinnishM15[0]	
#define strLanguageIndex15	strTurkishM15[0]	
#define strLanguageIndex16	strCzechM15[0]
#define strLanguageIndex17	strUkrainianM15[0]	
#define strLanguageIndex18	strSimplifiedChineseM15[0]	
#define strLanguageIndex19	strTChineseM15[0]	
#define strLanguageIndex20	strJapaneseM15[0]	
#define strLanguageIndex21	strKoreanM15[0]	
#endif


#endif
