#include "Board.h"
#define Multi_Number	1
#define Multi_Gauge 1
#define OnlyDrawColor	BIT7
#define OnlyDrawCode	BIT6

#define extra_offset 0

typedef enum
{
	OSD_HPosition=5,
	OSD_VPosition=5+2,
	OSD_Timeout=5+4,
	OSD_Trans=5+6,
    OSD_Language=5+8,
#ifdef EnableLightBar
    OSD_LightBar=5+10,
#endif
}OsdSetupItem;

#define SpaceFont			0x01
#define CommonColorAddr     GuageFontStartAddr // 0x15

#if DisplayLogo == EnvisionLOGO

	#if Enable_OsdEngine1
		#define Frame2ColorAddr     GuageFontStartAddr+20
		#define Frame4ColorAddr	   Start4ColorAddr+20
		#define Frame8ColorAddr     0xC0
	#else
		#define Frame2ColorAddr     0x09
		#define Frame2ColorAddr1     GuageFontStartAddr-1
	#endif

#else

	#if Enable_OsdEngine1
		#define Frame2ColorAddr     GuageFontStartAddr+20
		#define Frame4ColorAddr	   Start4ColorAddr+20
		#define Frame8ColorAddr     0xC0
	#else
		#define Frame2ColorAddr     0x14
		#define Frame2ColorAddr1     GuageFontStartAddr-1
	#endif

#endif








#define _2ndPageColorFontAddr	0xF0//0xFF-2

//========================================================================
//========================================================================

#if Enable_OsdEngine1
#define Start2ColorAddr     0x23 // 0x1D
#else
#define Start2ColorAddr     0x61 // 0x1D
#endif

#define Luma2FontAddr       Start2ColorAddr
#define Image2FontAddr      (Luma2FontAddr +  Luma2FontNum) // 2
#define Colortemp2FontAddr  (Image2FontAddr +    Image2FontNum) // 1
#define Picture2FontAddr    (Colortemp2FontAddr+  Colortemp2FontNum) // 4
#define OsdSetup2FontAddr   (Picture2FontAddr+  Picture2FontNum) // 1
#define Extra2FontAddr      (OsdSetup2FontAddr +   OsdSetup2FontNum) // 2
#define Reset2FontAddr      (Extra2FontAddr +   Extra2FontNum)   // 1
#define Exit2FontAddr       (Reset2FontAddr +   Reset2FontNum)   // 1
#define End2FontAddr        (Exit2FontAddr +   Exit2FontNum)  // 1
//========================================================================
//========================================================================

#if Enable_OsdEngine1
#define Start4ColorAddr     0x30 // 0x1D
#else
#define Start4ColorAddr     0x6E // 0x2A// ((Reset2FontAddr + Reset2FontNum)+((Reset2FontAddr + Reset2FontNum)%2))
#endif
#define Luma4FontAddr		Start4ColorAddr
#define Image4FontAddr		(Luma4FontAddr + Luma4FontNum*2)  // 9*2)//
#define Colortemp4FontAddr	(Image4FontAddr +  Image4FontNum *2) // 13*2) //
#define Color4FontAddr		(Colortemp4FontAddr + Colortemp4FontNum*2)// 8*2) //
#define Picture4FontAddr	(Color4FontAddr + Color4FontNum*2)  // 9*2) //
#define OsdSetup4FontAddr	(Picture4FontAddr +    Picture4FontNum*2) // 11*2) //
#define Extra4FontAddr		(OsdSetup4FontAddr +  OsdSetup4FontNum*2) // 10*2) //
#if Enable_OsdEngine1
#define Reset4FontAddr		(Extra4FontAddr + Extra4FontNum*2) // 13*2) //
#else
#if Enable_CascadeMode
//#define Reset4FontAddr		2
#define Reset4FontAddr		(Extra4FontAddr + Extra4FontNum*4)
#else
#define Reset4FontAddr		0//(Extra4FontAddr + Extra4FontNum*2)
#endif
#endif
#define Exit4FontAddr		(Reset4FontAddr + Reset4FontNum*2)  //9*2) //

#define End4FontAddr		(Exit4FontAddr +  Exit4FontNum*2) // 11*2) //

//========================================================================
//========================================================================

//Ver F: 0x29~0x5E

#if Enable_OsdEngine1
#define Start8ColorAddr     0xEB // 0xDC
#else
#if Enable_CascadeMode
//#define Start8ColorAddr     0x29+2
#define Start8ColorAddr     0x29+(Extra4FontAddr + Extra4FontNum*4)
#else
#define Start8ColorAddr     0x29
#endif
#endif
#define Image8FontAddr		Start8ColorAddr
#define Color8FontAddr		(Image8FontAddr +  Image8FontNum*3)
#define OsdSetup8FontAddr	 (Color8FontAddr + Color8FontNum*3)
//unused #define Extra8FontAddr	    (OsdSetup8FontAddr + OsdSetup8FontNum*3)


#if Enable_OsdEngine1
#define Reset8FontAddr	    0x00
#else
#define Reset8FontAddr	    (OsdSetup8FontAddr + OsdSetup8FontNum*3)
#endif
#define Colortemp8FontAddr	(Reset8FontAddr +  Reset8FontNum*3) //  15
#define Picture8FontAddr	(Colortemp8FontAddr +  Colortemp8FontNum*3)//  18
#define Exit8FontAddr		(Picture8FontAddr + Picture8FontNum*3)  // 27
#define End8FontAddr	    (Exit8FontAddr + Exit8FontNum*3) // 36


#define HotKey2ColorAddr 0x30
#define HotKeyDCR2ColorAddr     0x3A
#define HotKeyEnergyStar2ColorAddr  0x60
#if Enable_OsdEngine1
#define HotKey4ColorAddr 0x40
#define HotKey8ColorAddr 0x80
#else
#define HotKey4ColorAddr 0x40+0x40
#define HotKey8ColorAddr 0x80+0x40
#endif


#define Color_2  0x01
#define Color_4  0x02
#define Color_8  0x03
#define Color_2T  0x04    //Transparent
#define Color_4T  0x05    //Transparent
#define Color_8T  0x06    //Transparent


typedef enum
{ CP_WhiteColor,
  CP_RedColor,
  CP_LightBlue,
  CP_BlueColor,
  CP_WhiteColor1,
  CP_GrayColor,
  CP_LightColor,
  CP_BlackColor,
  CP_MaxColor
} CP_ColorType;

typedef enum
{ PowerOffMenu, 		                                		// 0
   PowerOnMenu,                                                            // 1
   StandbyMenu,		                                                    // 2
//	SuspenMenu,
  SleepMenu,                                                                 // 3
  CableNotConnectedMenu,                                            // 4
  UnsupportedModeMenu,		                                       // 5
  RootMenu, 					                                       // 6

  // user definition from here
  MainMenu, 	//7
  LuminanceMenu,                                                          //8
#if !1//OSD_2008
  ContrastMenu, BrightnessMenu,
  ECOMenu,
  GAMAMenu,
  DCRMenu,    // 9,10,11,12, 13
#endif
  ImageSetupMenu,                                                        //14,
#if !1//OSD_2008
 ClockMenu, FocusMenu, HPositionMenu, VPositionMenu,   // 15,16,17,18
#endif
  ColorTempMenu,                                                          // 19
#if 0
  ColorTempSelectMenu,
#endif
#if !1//OSD_2008
  RedMenu, GreenMenu, BlueMenu,
#if Enable_YMC
  YMenu, CMenu, MMenu, // 20,21,22,23,24,25, 26
#endif
#endif
  ColorBoostMenu,                                                           //27
#if 0
FullMenu, SkinProtectMenu, GreenEnhanceMenu, BlueEnhanceMenu, AutoDetectMenu, DemoMenu,// 28,29,30,31,32,33
#endif
  BrightFrameMenu,  //34
#if 0
  BFFrameSizeMenu,
  BF_BrightnessMenu,
  BF_ContrastMenu,
  BF_HueMenu,
  BF_SaturationMenu,
  BFSwitchMenu,// 35,36,37,38,39,40

  BF_PositionMenu,// 41
  BFHPositionMenu,BFVPositionMenu,//42 ,43
 #endif

  OsdSetUpMenu,                                                               //44,
#if 0
  OsdHPositionMenu,OsdVPositionMenu,OsdTimeOutMenu,
#if 1//OSD_2008
  OsdTransparenceMenu,
#endif

  LanguageMenu,//45,46,47,48
 #endif
  ExtraMenu,                                                                    //14 //49
#if 0
  InputSelectMenu,

  AutoConfigMenu ,
#endif
#if 0//DDCCI_ENABLE
  DDCCIMenu,
#endif
#if 0//Enable_Expansion
  ExpansionMenu,
#endif
 ResetMenu,								//15
#if 0
 InformationMenu,//,50,51,52,53
#endif
  HotKeyECOMenu, // 54
 HotKeyDCRMenu, //17// 54
#if Enable_Lightsensor
HotKeyiCareMenu,
#endif
#if HotExpansionAdjust
 HotExpansionMenu,
#endif
#if AudioFunc
  HotKeyVolMenu,  // 55
  //HotKeyMuteMenu, // 55
#else
  HotKeyConMenu,     //19
#endif
#if EnableHotBrightness
HotKeyBriMenu,
#endif
#if HotInputSelect  
	HotInputSelectMenu,
#endif
  ResetWaitMenu,    //20//56

  AutoMenu,//57

  OsdLockMenu,//58

  AutoColorMenu,	//59

  BurninMenu,//24//60

  FactoryMenu,//	61

  LogoMenu, 	//62

  InputInfoMenu, // 63
#if DisplayEnergyStar
  EnergyStarMenu, // 64
#endif  
#if DDCCI_ENABLE
  DdcciInfoMenu, // 65
#endif
#if EnablePanelServiceMenu
  AdjustPanelMenu,
#endif
//  OsdLockMenu,
  // user definition end
#ifdef OffPower  
  OffPowerWarningMenu,
#endif
  MaxMenu
} MenuPageIndexType;

typedef enum
{
  COLORBOOST_FULL,       //4 //1
  COLORBOOST_HALF,    // 2
  COLORBOOST_OFF,   // 3
  COLORBOOST_Nums

} ColorBoostType;


typedef enum
{
  ECO_Standard,       //4 //1
  ECO_Text,          // 2
  ECO_Internet,   // 3
  ECO_Game,        // 4
  ECO_Movie,        // 5
  ECO_Sports,       //4 // 6


  ECO_Nums

} ECOModeType;

#if Enable_Expansion
typedef enum
{
  Expansion_Full,
  Expansion_Aspect,
  Expansion_Nums
}ExpansionType;
#endif

typedef enum
{
#if 0
   CTEMP_Cool1,   //  8500K
 //  CTEMP_Warm2,  // 6500K
   CTEMP_Normal,  // 7800K
   CTEMP_Warm1,  // 5700K//   CTEMP_Cool2,   //  9300K
   CTEMP_SRGB,   //  SRGB
   CTEMP_USER,	//  USER

  CTEMP_Nums
  #endif

	CTEMP_Warm1,	// 5700K//	 CTEMP_Cool2,	//	9300K
	CTEMP_Normal,  // 7800K
	CTEMP_Cool1,   //  8500K	 
	CTEMP_SRGB,   //  SRGB
	CTEMP_USER,	//  USER
	CTEMP_Nums 
} ColorTempType;


#if LANGUAGE_TYPE==ASIA
typedef enum
{
  LANG_English,
  LANG_Spanish,
  LANG_France,
  LANG_Portugues,
  LANG_Russia,
  LANG_Korea,
  LANG_TChina,
  LANG_SChina,


  LANG_Nums
} LanguageType;
#elif LANGUAGE_TYPE==EUROPE
typedef enum
{
  LANG_English,
  LANG_France,
  LANG_Spanish,
  LANG_German,
  LANG_Russia,
  LANG_Italian,
  LANG_Swedish,
  LANG_Polish,
  LANG_Czech,
  LANG_Nums
} LanguageType;
#elif LANGUAGE_TYPE==All13Language
typedef enum
{
  LANG_English,
  LANG_German,
  LANG_France,
  LANG_Italian,
  LANG_Spanish,
  LANG_Swedish,
  LANG_Polish,
  LANG_Czech,
  LANG_Russia,
  LANG_Portugues,
  LANG_Korea,
  LANG_TChina,
  LANG_SChina,
  LANG_Nums
} LanguageType;
#elif LANGUAGE_TYPE==All14Language_AddJap
typedef enum
{
  LANG_English,
  LANG_German,
  LANG_France,
  LANG_Italian,
  LANG_Spanish,
  LANG_Swedish,
  LANG_Polish,
  LANG_Czech,
  LANG_Russia,
  LANG_Portugues,
  LANG_Korea,
  LANG_TChina,
  LANG_SChina,
  LANG_Japan,
  LANG_Nums
} LanguageType;

#elif LANGUAGE_TYPE==ID2007Standard
typedef enum
{
  LANG_English,
  LANG_Spanish,
  LANG_France,
  LANG_German,
  LANG_Italian,
  LANG_Portugues,
  LANG_Russia,
  LANG_SChina,
  LANG_Nums
} LanguageType;
#endif



typedef enum
{
  GAMA1,          // 0
  GAMA2,   // 1
  GAMA3,        // 2

  GAMA_Nums
} GAMAModeType;

typedef enum
{ MIA_Nothing,
  MIA_GotoNext, // goto next
  MIA_GotoPrev, // goto prev
  MIA_PrevItem,
  MIA_NextItem,
  MIA_JumpNaviItem,
  MIA_JumpAdjustItem,
  MIA_Exit, // Exit
  MIA_IncValue, // increase value
  MIA_DecValue, // decrease value
  MIA_ExecFunc,
  MIA_Auto,
  MIA_Brite,
  MIA_Cont,
  MIA_InvIncValue,
  MIA_InvDecValue,
  MIA_RedrawMenu,
  MIA_GotoECO,
  MIA_ECO,
#if HotInputSelect
  MIA_ShowHotInput,
#endif
#if HotExpansionAdjust
  MIA_AdjustExpansion,
#endif
#if AudioFunc

  MIA_VOL,
  MIA_Mute
  ,
#endif
#if EnableVcomAdjustItem
	MIA_HotVCOM,
#endif

  MIA_DCR,
#if Enable_Lightsensor
  MIA_iCare,
#endif
  MIA_UpItem,
  MIA_DownItem,
  MIA_ExitGotoNext, // goto next
  MIA_ExitGotoPrev, // goto prev
  MIA_ExecGotoPrev,
  MIA_DDC,
  MIA_SourceSel, //2006-02-22
  MIA_Power // power control
  #if !AllInOneKey
  ,
  MIA_AdjustValue,// power control
  MIA_ExitAdjustValue
  #endif

} MenuItemActionType;


typedef enum
{ DWI_Nothing,
  DWI_Icon,
  DWI_Text,
  DWI_FullText,
  DWI_CenterText,
  DWI_FullCenterText,
#if 0
  DWI_TextWithSpace,
#endif
  DWI_None
} DrawMenuItemType;


typedef enum
{
SUB_WIN_00,
SUB_WIN_01,
SUB_WIN_02,
SUB_WIN_03

}SubWinType;

typedef BYTE *(*fpDisplayText)(void);
typedef Bool(*fpExecFunc)(void);
typedef BYTE(*fpItemIndex)(void);
typedef Bool(*fpAdjustValue)(MenuItemActionType action);
typedef WORD(*fpGetValue)(void);
typedef WORD(*fpGetMaxValue)(void);
typedef WORD(*fpSetMaxValue)(void);
#define mpbStay 	             0x01 // for stay in menu page
#define mpbInvisible	      0x02 // menu page visible or not
#define mpbBrowse		0x04
//#define mpbClrInner	      0x08
#define mpbRedraw		0x10
#define mpbMoving		0x20
#define mpbCenter		0x40
#define mpbLogoFrame	0x80
#define mibSelectable	0x01 // for menu item selectable
#define mibInvisible	0x02 // for menu item selectable
#define mibDVIDisable	0x04
#define mibFactoryItem	0x08
//1015	#define mibClrInner1	0x10
#define mibCenter		0x10
#define mibsRGBDisable 0x20
//#define mibClrInner	0x40
#if Enable_Lightsensor
#define mibiCareDisable 0x40
#endif
#define mibDrawValue	0x80
#define mibUserColor 		0x0100
#define mibBFEnable 		0x0200
#define mibStdEnable    	0x0400
#define mibDCRDisable   	0x0800
#define mibDemoDisable   	0x1000
#define mibHighlight		0x2000
#define mibSkipImageRatio   0x4000 // 071218 for skip wide timing
#if PICTUREBOOST_FUN_EN
#define mibPicBoostDisable    0x8000 
#endif
#define dwiNumber	    0x01
#define dwiGauge	          0x02
#define dwiRadioGroup   0x04
#define dwiEnd		    0x08
#define dwiHex		    0x10
#define dwiOptionBar 		0x20
#define dwiDirectStr		0x40
#ifdef OffPower
#define dwiZERONumber       0x80  
#endif
//#define dwiJustShowValue    0x40
//#define dwiOptionBar1 	0x80
typedef struct
{
    fpAdjustValue AdjustFunction;
    fpExecFunc ExecFunction;
} MenuItemFuncType;
#if Multi_Number //multi-number
    typedef struct
    {
        BYTE Flags;
        BYTE XPos, YPos;
        fpGetValue GetValue;
    } NumberType;
    typedef struct
    {
        // BYTE Flags;
        BYTE ForeColor, BackColor;
        NumberType *DisplayNumber;
    } DrawNumberType;
#else // single number
    typedef struct
    {
        BYTE Flags;
        BYTE ForeColor, BackColor;
        BYTE XPos, YPos;
        fpGetValue GetValue;
    } DrawNumberType;
#endif
#if Multi_Gauge // multi-gauge
    typedef struct
    {
        BYTE Flags;
        BYTE XPos, YPos;
        fpGetValue GetValue;
    } GaugeType;
    typedef struct
    {
        //BYTE Flags;
        BYTE ForeColor, BackColor;
        BYTE Length;
        GaugeType *DisplayGauge;
    } DrawGuageType;
#else // single gauge
    typedef struct
    {
        BYTE Flags;
        BYTE ForeColor, BackColor;
        BYTE Length;
        BYTE XPos, YPos;
        fpGetValue GetValue;
    } DrawGuageType;
#endif
typedef struct
{
    BYTE Flags;
    BYTE XPos, YPos;
    fpDisplayText DisplayText;
} RadioTextType;
typedef struct
{
    // BYTE Flags;
    BYTE ForeColor, BackColor;
    fpGetValue GetValue;
    RadioTextType *RadioText;
} DrawRadioGroupType;
typedef struct
{
    DrawNumberType *DrawNumber;
    DrawGuageType *DrawGuage;
    DrawRadioGroupType *DrawRadioGroup;
} DrawValueType;
typedef struct
{
    BYTE *Fonts;
    WORD FontCount;
} MenuFontType;
#if 0
    typedef struct
    {
        BYTE XPos, YPos;
        BYTE ForeColor, BackColor;
        BYTE SelForeColor, SelBackColor;
        BYTE NextPage;
        DrawMenuItemType DrawItemMethod;
        fpDisplayText DisplayText;
        MenuItemActionType *KeyEvents;
        MenuItemFuncType KeyFunction;
        DrawValueType DisplayValue;
        BYTE Flags;
    } MenuItemType;
#endif
typedef struct
{
    BYTE XPos, YPos;
    BYTE ForeColor, BackColor;
    BYTE SelForeColor, SelBackColor;
    BYTE NextPage;
    DrawMenuItemType DrawItemMethod;
    fpDisplayText DisplayText;
    MenuItemActionType *KeyEvents;
    MenuItemFuncType KeyFunction;
    DrawValueType DisplayValue;
    MenuFontType *Fonts;
    WORD Flags; // 2006/9/6 5:41PM by Emily BYTE Flags;
} MenuItemType;
#if 0
    typedef struct
    {
        BYTE SpaceWidth;
        WORD LineData[18];
    } PropFontType;
#endif
#if 0
    typedef struct
    {
        BYTE XSize, YSize;
        BYTE PrevPage;
        MenuItemType *MenuItems;
        BYTE ItemCount;
        fpExecFunc ExecFunction;
        BYTE Flags;
    } MenuPageType;
#endif
typedef struct
{
    BYTE XSize, YSize;
    BYTE PrevPage;
    MenuItemType *MenuItems;
    BYTE ItemCount;
    fpExecFunc ExecFunction;
    MenuFontType *Fonts;
    BYTE Flags;
} MenuPageType;
