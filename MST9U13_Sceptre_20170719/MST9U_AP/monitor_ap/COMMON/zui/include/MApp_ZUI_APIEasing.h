////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPP_EASING_H
#define _MAPP_EASING_H


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifdef _MAPP_EASING_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#include "datatype.h"
#include "math.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Extern Global Variabls
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Extern Functions
//------------------------------------------------------------------------------
typedef enum _E_EASING_TYPE
{
    E_EASING_LINEAR,
    E_EASING_EASEINQUAD,
    E_EASING_EASEOUTQUAD,
    E_EASING_EASEINOUTQUAD,
    E_EASING_EASEOUTINQUAD,
    E_EASING_EASEINCUBIC,
    E_EASING_EASEOUTCUBIC,
    E_EASING_EASEINOUTCUBIC,
    E_EASING_EASEOUTINCUBIC,
    E_EASING_EASEINQUART,
    E_EASING_EASEOUTQUART,
    E_EASING_EASEINOUTQUART,
    E_EASING_EASEOUTINQUART,
    E_EASING_EASEINQUINT,
    E_EASING_EASEOUTQUINT,
    E_EASING_EASEINOUTQUINT,
    E_EASING_EASEOUTINQUINT,
    E_EASING_EASEINSINE,
    E_EASING_EASEOUTSINE,
    E_EASING_EASEINOUTSINE,
    E_EASING_EASEOUTINSINE,
    E_EASING_EASEINEXPO,
    E_EASING_EASEOUTEXPO,
    E_EASING_EASEINOUTEXPO,
    E_EASING_EASEOUTINEXPO,
    E_EASING_EASEINCIRC,
    E_EASING_EASEOUTCIRC,
    E_EASING_EASEINOUTCIRC,
    E_EASING_EASEOUTINCIRC,
    E_EASING_EASEINELASTIC,
    E_EASING_EASEOUTELASTIC,
    E_EASING_EASEINOUTELASTIC,
    E_EASING_EASEOUTINELASTIC,
    E_EASING_EASEINBACK,
    E_EASING_EASEOUTBACK,
    E_EASING_EASEINOUTBACK,
    E_EASING_EASEOUTINBACK,
    E_EASING_EASEINBOUNCE,
    E_EASING_EASEOUTBOUNCE,
    E_EASING_EASEINOUTBOUNCE,
    E_EASING_EASEOUTINBOUNCE
}E_EASING_TYPE;

INTERFACE S32 MApp_ZUI_API_Easing(E_EASING_TYPE easingType, S32 t, S32 b, S32 c, S32 d);

#undef INTERFACE

#endif /* _MAPP_EASING_H */

