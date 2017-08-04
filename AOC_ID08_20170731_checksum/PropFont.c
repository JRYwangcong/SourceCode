#include <stdlib.h>
#include "types.h"
#include "ms_reg.h"
#include "panel.h"
#include "Board.h"
#include "debug.h"
#include "ms_rwreg.h"
#include "mstar.h"
#include "misc.h"
#include "global.h"
#include "menudef.h"
//#include "font.h"
#include "extlib.h"

#if LANGUAGE_TYPE == ASIA
 #include "PropFontAsia.h"
#elif LANGUAGE_TYPE == EUROPE
#include "PropFontEurope.h"
#elif LANGUAGE_TYPE == All13Language
#include "PropFontAllLanguage.h"
#elif LANGUAGE_TYPE == All14Language_AddJap
#include "PropFontAllLanguage_AddJap.h"
#else
#include "PropFontID2007Standard.h"
#endif
#include "MenuFontTbl.h"

extern BYTE  MenuPageIndex;

void Osd_DynamicLoadFont(BYTE addr, BYTE *fontPtr, WORD num)
{
    mStar_WriteByte(RAMFA, addr);
    #if CompressPropFont
        LoadCompressedPropFonts(fontPtr, num);
    #else
        LoadPropFonts(fontPtr, num);
    #endif
}

void DynamicLoadFont(MenuFontType *menuFonts)
{
	//if(MenuPageIndex != HotKeyECOMenu)
    if (menuFonts->Fonts)
	    Osd_DynamicLoadFont(PROP_FONT_START, (BYTE*)((menuFonts + UserPrefLanguage)->Fonts), (menuFonts + UserPrefLanguage)->FontCount);

}
