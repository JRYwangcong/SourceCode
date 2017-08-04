extern void Osd_DynamicLoadFont(BYTE addr, BYTE *fontPtr, WORD num);
extern void DynamicLoadFont(MenuFontType *menuFonts);

#if LANGUAGE_TYPE != All14Language_AddJap  && LANGUAGE_TYPE != ASIA
extern MenuFontType code MenuPage2[];
#endif
extern MenuFontType code MenuPage5[];

extern MenuFontType code MenuPage7[];

extern MenuFontType code MenuPage13[];
extern MenuFontType code MenuPage18[];
extern MenuFontType code MenuPage26[];
extern MenuFontType code MenuPage33[];
extern MenuFontType code MenuPage44[];
extern MenuFontType code MenuPage49[];
extern MenuFontType code MenuPage50[];
extern MenuFontType code MenuPage54[];
//extern MenuFontType code MenuPage55[];
extern MenuFontType code MenuPage56[];
extern MenuFontType code MenuPage57[];
extern MenuFontType code MenuPage59[];
#if LANGUAGE_TYPE == All14Language_AddJap
extern MenuFontType code MenuPage51[];
#endif
#if INPUT_TYPE==INPUT_1A
extern MenuFontType code MenuPage4[];
#endif
