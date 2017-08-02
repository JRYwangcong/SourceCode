///////////////////////////////////////////////////////////////////////////////
/// @file FMenuPropStr.h
/// @brief Head file of FMenuPropStr.c
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _FMENUPROPSTR_H
#define _FMENUPROPSTR_H
#include "FactoryMenu.h"//Chihlung 20120321
/********************************************************************************/
/*                                              Include Files                                                                         */
/********************************************************************************/
#include "Board.h"
/********************************************************************************/
/*                                              Define                                                                                 */
/********************************************************************************/

#ifdef _FMENUPROPSTR_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif
INTERFACE BYTE* AutoColorText(void);
INTERFACE BYTE* AdcGainText(void);
INTERFACE BYTE* FactoryRedText(void);
INTERFACE BYTE* FactoryGreenText(void);
INTERFACE BYTE* FactoryBlueText(void);
INTERFACE BYTE* AdcOffsetText(void);

#if 0//ENABLE_COLOR_TEMP_OSD //Mike 110620
INTERFACE BYTE* F10000KText(void);
INTERFACE BYTE* F9300KText(void);
INTERFACE BYTE* F7500KText(void);
INTERFACE BYTE* F6500KText(void);
INTERFACE BYTE* F5700KText(void);
INTERFACE BYTE* F5000KText(void);
#else
INTERFACE BYTE* FCoolText(void);
INTERFACE BYTE* FNormalText(void);
INTERFACE BYTE* FWarmText(void);//JL
#endif

INTERFACE BYTE* EDIDWPText(void);//JL

INTERFACE BYTE* FEDIDWPOnOffText(void);//JL
INTERFACE BYTE* FClearUesrAreaText(void);  //JL

INTERFACE BYTE* DateText(void);
INTERFACE BYTE *F_ChipText(void);
INTERFACE BYTE *F_PanelText(void);
INTERFACE BYTE *F_PanelSizeText(void);
INTERFACE BYTE *F_PanelNameText(void);
INTERFACE BYTE *F_PanelSizeNumber(void);

#if 0//ENABLE_ENERGY_STAR_LOGO
//Sky101029 ES logo on/off
INTERFACE BYTE* FESLogoText(void);
INTERFACE BYTE* EnergyLogoOnOffText(void);
#endif

//INTERFACE BYTE* K7500KText(void);
#if ENABLE_SRGB_FACTORY_ITEM//ENABLE_SRGB_ITEM //Sky120318//Chihlung 20120321
INTERFACE BYTE* FsRGBText(void);
#endif
INTERFACE BYTE* BurninText(void);
INTERFACE BYTE* BurninOnOffText(void);
#if 0
INTERFACE BYTE* BankText(void);
INTERFACE BYTE* BankNameText(void);
INTERFACE BYTE* AddressText(void);
INTERFACE BYTE* ValueText(void);
#endif
INTERFACE BYTE* ExitText(void);
INTERFACE BYTE* FAutoColorText(void);
INTERFACE BYTE* ResetBacklightText(void);


INTERFACE BYTE* KeyTestText(void);


#if F_ResetItem
INTERFACE BYTE* FactoryResetText(void);
#endif

INTERFACE BYTE* FESLogoText(void);
INTERFACE BYTE* LogoOnOffText(void);

///------------------------------------------------------------
INTERFACE BYTE* BacklightTimeText(void);
INTERFACE BYTE* HourText(void);

INTERFACE BYTE* FWVerSionText(void);
//INTERFACE BYTE* FactoryTText(void);
//INTERFACE BYTE* VersionText(void);
INTERFACE BYTE* PassText(void);
INTERFACE BYTE* FailText(void);
//INTERFACE BYTE* PanelText(void);
//INTERFACE BYTE* FPanelAUOText(void);
//INTERFACE BYTE* FPanelCMOText(void);
//INTERFACE BYTE* FPanelSECText(void);
//INTERFACE BYTE* FactoryVersionText(void);
INTERFACE BYTE* MinText(void);
INTERFACE BYTE* ColorTempText(void);

INTERFACE BYTE* FPassText(void);
INTERFACE BYTE* FFailText(void);
INTERFACE BYTE* FactoryModelName(void);

#if 0 //White120131,update BID_Counter_for_FY13
INTERFACE BYTE* ResetBIDText(void);//White111101,BID_Counter_for_FY13
#endif
#if 0
INTERFACE BYTE* BIDCountText(void);
#endif
#ifndef _FMENUPROPSTR_C
extern MenuFontType code MenuPage19[];
extern MenuFontType code MenuPage20[];

#endif


INTERFACE BYTE *F_PanelSSCFreqText(void);
INTERFACE BYTE *F_PanelSSCPermillageText(void);
INTERFACE BYTE *F_DDRSSCFreqText(void);
INTERFACE BYTE *F_DDRSSCPermillageText(void);


#undef INTERFACE

#endif ///   _FMENUPROPSTR_H

