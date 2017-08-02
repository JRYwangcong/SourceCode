///////////////////////////////////////////////////////////////////////////////
/// @file FactoryMenu.h
/// @brief Head file of FactoryMenu.c
/// @author MStarSemi Inc.
///
///
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _FACTORYMENU_H
#define _FACTORYMENU_H

/********************************************************************************/
/*                                              Include Files                                                                         */
/********************************************************************************/
#include "menudef.h"

//#include "FactoryMenu.h"//Chihlung 20120321
/********************************************************************************/
/*                                              Define                                                                                 */
/********************************************************************************/
/// The index of integrator defined menu for factory production starts from 0x80.
#define  BURNIN_MENU            (0|BIT7)
#if ENABLE_VGA_INPUT
#define  AUTO_COLOR_MENU        (1|BIT7)
#define  FACTORY_MENU           (2|BIT7)
#else
#define  FACTORY_MENU           (1|BIT7)
#endif


typedef enum 
{
#if ENABLE_VGA_INPUT
    FMI_Item_AutoColor,
    
    FMI_Item_RGain,
    FMI_Item_RGain_Adj,
    FMI_Item_GGain,
    FMI_Item_GGain_Adj,
    FMI_Item_BGain,
    FMI_Item_BGain_Adj,
#endif

    FMI_Item_6500KR,
    FMI_Item_6500KR_Adj,
    FMI_Item_6500KG,
    FMI_Item_6500KG_Adj,
    FMI_Item_6500KB,
    FMI_Item_6500KB_Adj,

    FMI_Item_9300KR,
    FMI_Item_9300KR_Adj,
    FMI_Item_9300KG,
    FMI_Item_9300KG_Adj,
    FMI_Item_9300KB,
    FMI_Item_9300KB_Adj,

    FMI_Item_5700KR,
    FMI_Item_5700KR_Adj,
    FMI_Item_5700KG,
    FMI_Item_5700KG_Adj,
    FMI_Item_5700KB,
    FMI_Item_5700KB_Adj,

    FMI_Item_BurnIn,
    FMI_Item_BurnIn_Adj,

    FMI_Item_EDIDWP,
    FMI_Item_EDIDWP_Adj,

    FMI_Item_LOGO,
    FMI_Item_LOGO_Adj,

    FMI_Item_ResetBlackLightTime,

    FMI_Item_Exit,




    FMI_Item_Max,


} FMI_Item;


extern void FactoryMenuHandler(void);
extern Bool FactoryExecuteKeyEvent(MenuItemActionType enMenuAction);

#undef INTERFACE
#define ENABLE_SRGB_FACTORY_ITEM 0 //Chihlung 20120321
#endif///   _FACTORYMENU_H
