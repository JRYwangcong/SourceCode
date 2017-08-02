////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
#ifndef _ZUI_TABLES_H
#define _ZUI_TABLES_H

#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIgdi.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"


//NOTE: when you add new control, you need to add here!

#include "MApp_ZUI_CTLkeyboard.h"
#include "MApp_ZUI_CTLanimation.h"
#include "MApp_ZUI_CTLautoclose.h"
#include "MApp_ZUI_CTLballprogbar.h"
#include "MApp_ZUI_CTLbgtransparent.h"
#include "MApp_ZUI_CTLbuttonclick.h"
#include "MApp_ZUI_CTLcheckbox.h"
#include "MApp_ZUI_CTLdynabmp.h"
#include "MApp_ZUI_CTLdynacolortext.h"
#include "MApp_ZUI_CTLdynaepgpunctext.h"
#include "MApp_ZUI_CTLdynalist.h"
#include "MApp_ZUI_CTLdynalist2.h"
#include "MApp_ZUI_CTLdynapunctext.h"
#include "MApp_ZUI_CTLdynatext.h"
#include "MApp_ZUI_CTLdynatexteffect.h"
#include "MApp_ZUI_CTLeffect.h"
#include "MApp_ZUI_CTLeffectcube.h"
#include "MApp_ZUI_CTLeffectfall.h"
#include "MApp_ZUI_CTLeffectflip.h"
#include "MApp_ZUI_CTLeffectfold.h"
#include "MApp_ZUI_CTLeffectpopup.h"
#include "MApp_ZUI_CTLeffectzoom.h"
#include "MApp_ZUI_CTLeffectroll.h"
#include "MApp_ZUI_CTLeffectslideitem.h"
#include "MApp_ZUI_CTLfake3d.h"
#include "MApp_ZUI_CTLgrid.h"
#include "MApp_ZUI_CTLlistbox.h"
#include "MApp_ZUI_CTLmainframe.h"
#include "MApp_ZUI_CTLmarquee.h"
#include "MApp_ZUI_CTLmotiontrans.h"
#include "MApp_ZUI_CTLpagescroll.h"
#include "MApp_ZUI_CTLpercentprogbar.h"
#include "MApp_ZUI_CTLradiobutton.h"
#include "MApp_ZUI_CTLrectprogbar.h"
#include "MApp_ZUI_CTLrotatestatic.h"
#include "MApp_ZUI_CTLscroller.h"
#include "MApp_ZUI_CTLslider.h"
#include "MApp_ZUI_CTLspin.h"
#include "MApp_ZUI_CTLtextviewer.h"
#include "MApp_ZUI_ACTflippage.h"


//NOTE: when you add new OSD page, you need to add here!


#if (UI_SKIN_SEL==UI_SKIN_BM_800X600X1555)

#include "../res800x600x1555_BM/Include/OSDTbl_h.inl"

#elif (UI_SKIN_SEL==UI_SKIN_NON_BM_800X600X1555)

#include "../res800x600x1555/Include/OSDTbl_h.inl"

#endif

#endif /* _ZUI_TABLES_H */
