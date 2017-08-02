////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPP_ZUI_ACTEFFECT_H
#define _MAPP_ZUI_ACTEFFECT_H

#include "datatype.h"
#include "MApp_ZUI_ACTeffectsetting.h"

#ifdef MAPP_ZUI_ACTEFFECT_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

/**
 * \var ZUI_TRANSEFFMODE
 * \brief transition effect type
 *
*/
typedef enum
{
    EN_EFFMODE_NONE,

    EN_EFFMODE_PAGE_SHOWUP,
    EN_EFFMODE_PAGE_EXIT,
    EN_EFFMODE_MAIN_BG_ROTATE,
    EN_EFFMODE_LIST_SHOWUP,
    EN_EFFMODE_MSGBOX_SHOWUP,
    //EN_EFFMODE_MAIN_BG_ROTATE_RIGHT,
    EN_EFFMODE_PAGE_FADE_IN,
    EN_EFFMODE_PAGE_FADE_OUT,
    EN_EFFMODE_PAGE_ZOOM_IN,
    EN_EFFMODE_PAGE_ZOOM_OUT,
    EN_EFFMODE_PAGE_SPREAD_OUT,
    EN_EFFMODE_PAGE_ROLL_UP,
    EN_EFFMODE_PAGE_MOSAIC,
    EN_EFFMODE_PAGE_EXIT_UP,
    EN_EFFMODE_PAGE_EXIT_DOWN,      // would be limited by the setting of display range
    EN_EFFMODE_PAGE_EXIT_LEFT,
    EN_EFFMODE_PAGE_EXIT_RIGHT,     // would be limited by the setting of display range
    EN_EFFMODE_PAGE_ENTER_UP,
    EN_EFFMODE_PAGE_ENTER_DOWN,      // would be limited by the setting of display range
    EN_EFFMODE_PAGE_ENTER_LEFT,
    EN_EFFMODE_PAGE_ENTER_RIGHT,     // would be limited by the setting of display range

    EN_EFFMODE_OPEN,
    EN_EFFMODE_CLOSE,

} ZUI_TRANSEFFMODE;

INTERFACE void MApp_ZUI_ACT_TransitionEffectBegin(ZUI_TRANSEFFMODE mode, ZUI_STATE target);
INTERFACE void MApp_ZUI_ACT_TransitionEffectDoing(void);
INTERFACE void MApp_ZUI_ACT_TransitionEffectEnd(void);

#undef INTERFACE

#endif // _MAPP_ZUI_ACTEFFECT_H
