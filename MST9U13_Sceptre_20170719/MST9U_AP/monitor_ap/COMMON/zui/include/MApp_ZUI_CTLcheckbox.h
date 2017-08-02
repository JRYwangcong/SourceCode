////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPP_ZUI_CTL_CHECKBOX_H
#define _MAPP_ZUI_CTL_CHECKBOX_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef MAPP_ZUI_CTL_CHECKBOX_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define CB_CHECKBOX_BASE    0
#define CB_RADIOBUTTON_BASE 3

//////Checkbox control////////////////////////////////////////////////////


INTERFACE S32 MApp_ZUI_CTL_CheckboxWinProc(HWND, PMSG);

#define CHECKBOX_WINPROC MApp_ZUI_CTL_CheckboxWinProc


///////////////////////////////////////////////////////////////////////////////////
//  MACRO
//

#undef INTERFACE


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MAPP_ZUI_CTL_CHECKBOX_H */

