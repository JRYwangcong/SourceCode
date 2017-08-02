////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MAPP_GLOBALSETTINGST_H
#define MAPP_GLOBALSETTINGST_H

/********************************************************************************/
/*                              Include files                                   */
/********************************************************************************/
#include "Board.h"
#include "datatype.h"

/********************************************************************************/
/*                                 Macro                                        */
/********************************************************************************/


/********************************************************************************/
/*                                 Enum                                         */
/********************************************************************************/

typedef enum _EN_MENU_LANGUAGE
{
#if 1
    LANGUAGE_ENGLISH,
    LANGUAGE_CHINESE,
    LANGUAGE_MENU_MAX = LANGUAGE_CHINESE,        // OSD Menu Language Maximum
    LANGUAGE_CZECH,       //Minimum of OSD menu, Audio, and Subtitle Language
    LANGUAGE_DANISH,
    LANGUAGE_GERMAN,
    LANGUAGE_SPANISH,
    LANGUAGE_GREEK,
    LANGUAGE_FRENCH,
    LANGUAGE_CROATIAN,
    LANGUAGE_ITALIAN,
    LANGUAGE_HUNGARIAN,
    LANGUAGE_DUTCH,
    LANGUAGE_NORWEGIAN,
    LANGUAGE_POLISH,
    LANGUAGE_PORTUGUESE,
    LANGUAGE_RUSSIAN,
    LANGUAGE_ROMANIAN,
    LANGUAGE_SLOVENIAN,
    LANGUAGE_SERBIAN,
    LANGUAGE_FINNISH,
    LANGUAGE_SWEDISH,
    LANGUAGE_BULGARIAN,
    LANGUAGE_SLOVAK,
    //LANGUAGE_MENU_MAX = LANGUAGE_CHINESE,        // OSD Menu Language Maximum
    LANGUAGE_NONE = 0xFF,
#else
    LANGUAGE_CZECH,       //Minimum of OSD menu, Audio, and Subtitle Language
    LANGUAGE_DANISH,
    LANGUAGE_GERMAN,
    LANGUAGE_ENGLISH,
    LANGUAGE_SPANISH,
    LANGUAGE_GREEK,
    LANGUAGE_FRENCH,
    LANGUAGE_CROATIAN,
    LANGUAGE_ITALIAN,
    LANGUAGE_HUNGARIAN,
    LANGUAGE_DUTCH,
    LANGUAGE_NORWEGIAN,
    LANGUAGE_POLISH,
    LANGUAGE_PORTUGUESE,
    LANGUAGE_RUSSIAN,
    LANGUAGE_ROMANIAN,
    LANGUAGE_SLOVENIAN,
    LANGUAGE_SERBIAN,
    LANGUAGE_FINNISH,
    LANGUAGE_SWEDISH,
    LANGUAGE_BULGARIAN,
    LANGUAGE_SLOVAK,
    LANGUAGE_CHINESE,
    LANGUAGE_MENU_MAX = LANGUAGE_CHINESE,        // OSD Menu Language Maximum
    LANGUAGE_NONE = 0xFF,
#endif
} EN_LANGUAGE;

#define LANGUAGE_DEFAULT LANGUAGE_ENGLISH

#define GET_OSD_MENU_LANGUAGE()             (MApp_GetMenuLanguage())
#define SET_OSD_MENU_LANGUAGE(x)            (MApp_SetMenuLanguage(x))
#define SET_TIME_MENU_ZONE(x)               (MApp_SetTimeZone(x))

#undef INTERFACE
#endif

