#ifndef _FMENUFUNC_H
#define _FMENUFUNC_H
#include "FactoryMenu.h"//Chihlung 20120321
#include "menufunc.h"
#ifdef _FMENUFUNC_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif


INTERFACE WORD GetBColor6500KValue(void);
INTERFACE WORD GetGColor6500KValue(void);
INTERFACE WORD GetRColor6500KValue(void);
INTERFACE Bool AdjustRedColor6500K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustGreenColor6500K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustBlueColor6500K(MENU_ITEM_ACTION action);


INTERFACE WORD GetBColor9300KValue(void);
INTERFACE WORD GetGColor9300KValue(void);
INTERFACE WORD GetRColor9300KValue(void);
INTERFACE Bool AdjustRedColor9300K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustGreenColor9300K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustBlueColor9300K(MENU_ITEM_ACTION action);

INTERFACE WORD GetRColor5700KValue(void);
INTERFACE WORD GetGColor5700KValue(void);
INTERFACE WORD GetBColor5700KValue(void);
INTERFACE Bool AdjustRedColor5700K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustGreenColor5700K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustBlueColor5700K(MENU_ITEM_ACTION action);

#if 0 //ENABLE_COLOR_TEMP_OSD
INTERFACE WORD GetRColor10000KValue(void);
INTERFACE WORD GetGColor10000KValue(void);
INTERFACE WORD GetBColor10000KValue(void);
INTERFACE Bool AdjustRedColor10000K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustGreenColor10000K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustBlueColor10000K(MENU_ITEM_ACTION action);

INTERFACE WORD GetRColor7500KValue(void);
INTERFACE WORD GetGColor7500KValue(void);
INTERFACE WORD GetBColor7500KValue(void);
INTERFACE Bool AdjustRedColor7500K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustGreenColor7500K(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustBlueColor7500K(MENU_ITEM_ACTION action);

INTERFACE WORD GetRColor5000KValue(void);
INTERFACE WORD GetGColor5000KValue(void);
INTERFACE WORD GetBColor5000KValue(void);
INTERFACE Bool AdjustRedColor5000K(MenuItemActionType action);
INTERFACE Bool AdjustGreenColor5000K(MenuItemActionType action);
INTERFACE Bool AdjustBlueColor5000K(MenuItemActionType action);
#endif

#if ENABLE_SRGB_FACTORY_ITEM//ENABLE_SRGB_ITEM
INTERFACE Bool SetFactoryColorSRGB(void);
INTERFACE Bool AdjustRedColorsRGB(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustGreenColorsRGB(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustBlueColorsRGB(MENU_ITEM_ACTION action);
INTERFACE WORD GetBColorsRGBValue(void);
INTERFACE WORD GetGColorsRGBValue(void);
INTERFACE WORD GetRColorsRGBValue(void);
#endif


INTERFACE Bool EnableBurninMode(void);
//INTERFACE WORD GetBurninValue(void);
INTERFACE Bool AdjustBurnin(MENU_ITEM_ACTION action);
#if 1//ENABLE_ENERGY_STAR_LOGO //Sky101029 ES logo on/off
INTERFACE Bool AdjustLogoDisplay(MENU_ITEM_ACTION action);
#endif

INTERFACE Bool AutoColor(void);

INTERFACE Bool AdjustAdcBlueOffset(MENU_ITEM_ACTION action);
INTERFACE WORD GetAdcBlueOffsetValue(void);
INTERFACE Bool AdjustAdcGreenOffset(MENU_ITEM_ACTION action);
INTERFACE WORD GetAdcGreenOffsetValue(void);
INTERFACE Bool AdjustAdcRedOffset(MENU_ITEM_ACTION action);
INTERFACE WORD GetAdcRedOffsetValue(void);

INTERFACE Bool AdjustBlueGain(MENU_ITEM_ACTION action);
INTERFACE WORD GetBlueGainValue(void);
INTERFACE Bool AdjustGreenGain(MENU_ITEM_ACTION action);
INTERFACE WORD GetGreenGainValue(void);
INTERFACE Bool AdjustRedGain(MENU_ITEM_ACTION action);
INTERFACE WORD GetRedGainValue(void);

INTERFACE Bool AdjustFSubBrightness(MENU_ITEM_ACTION action);
INTERFACE WORD GetFSubBrightnessValue(void);
INTERFACE Bool AdjustFacEDIDWP(MENU_ITEM_ACTION action);

INTERFACE Bool AdjustBankNo(MENU_ITEM_ACTION action);
INTERFACE Bool AdjustAddr(MENU_ITEM_ACTION action);
INTERFACE WORD GetAddrValue(void);
INTERFACE Bool AdjustRegValue(MENU_ITEM_ACTION action);
INTERFACE WORD GetAddrRegValue(void);
INTERFACE WORD GetRegValue(void);
INTERFACE Bool WriteRegValue(void);

#if ENABLE_FACTORY_SSCADJ 
INTERFACE WORD GetSSCFrequencyValue(void);
INTERFACE WORD GetSSCPermillageValue(void);
INTERFACE Bool AdjustSSCFrequency(MENU_ITEM_ACTION enAction);
INTERFACE Bool AdjustSSCPermillage(MENU_ITEM_ACTION enAction);


INTERFACE WORD GetDDRSSCFrequencyValue(void);
INTERFACE WORD GetDDRSSCPermillageValue(void);
INTERFACE Bool AdjustDDRSSCFrequency(MENU_ITEM_ACTION enAction);
INTERFACE Bool AdjustDDRSSCPermillage(MENU_ITEM_ACTION enAction);

#endif


//INTERFACE Bool SetFactoryColorTempsRGB(void);
INTERFACE Bool SetFactoryColorTemp6500K(void);
INTERFACE Bool SetFactoryColorTemp5700K(void);
INTERFACE Bool SetFactoryColorTemp9300K(void);

#if 0 //ENABLE_COLOR_TEMP_OSD
INTERFACE Bool SetFactoryColorTemp10000K(void);
INTERFACE Bool SetFactoryColorTemp7500K(void);
INTERFACE Bool SetFactoryColorTemp5000K(void);
#endif
#if 0 //White120131,update BID_Counter_for_FY13
INTERFACE Bool ResetBIDCounter(void); //White111101,BID_Counter_for_FY13
#endif

#undef INTERFACE
#endif  //_FMENUFUNC_H

