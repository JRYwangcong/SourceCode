extern  BYTE OsdWindowWidth;
extern  BYTE OsdWindowHeight;
extern void Osd_SetOsdEngine(BYTE ucValue);
extern BYTE mStar_ReadOSDByte(BYTE address);
extern void mStar_WriteOSDByte(BYTE address, BYTE value);
extern void Osd_DrawNum(BYTE xPos, BYTE yPos, WORD value);
extern void Osd_Draw4Num(BYTE xPos, BYTE yPos, WORD value);
extern void Osd_DrawStr(BYTE xPos, BYTE yPos, BYTE *str);
extern void Osd_DrawHex(BYTE xPos, BYTE yPos, WORD value);
extern void Osd_SetPosition(BYTE xPos, BYTE yPos);
extern void Osd_SetWindowSize(BYTE width, BYTE height);
extern void Osd_SetTextColor(BYTE foreColor, BYTE backColor);

#if DISPLAY_CHECK_VOL
extern void Osd_DrawCheckSUM(BYTE xPos, BYTE yPos, WORD value);
#endif

//extern void Osd_DrawChar(BYTE xPos, BYTE yPos, BYTE ch);
extern void Osd_DrawGuage(BYTE xPos, BYTE yPos, BYTE length, BYTE value);
extern void Osd_Show(void);
extern void Osd_Hide(void);
extern void Osd_InitOSD(void);
extern void Osd_DrawPropStr(BYTE xPos, BYTE yPos, BYTE *str);
extern void DisableOsdWindow(BYTE winIndex);
extern void MoveOsdWindow(BYTE winIndex, BYTE YPos, BYTE xPosStar, BYTE Xlength, BYTE Ylength, BYTE FgColor, BYTE BgColor);
extern void Osd_DrawRealStr(BYTE xPos, BYTE yPos, BYTE *str);
extern void Osd_DrawCharDirect(BYTE xPos, BYTE yPos, BYTE ch);
extern void Osd_DrawContinuesChar(BYTE xPos, BYTE yPos, BYTE ch, BYTE ucNum);
extern void Osd_LoadLogoFontCP(void);
extern void LoadCommonFont(void);
extern void Osd_LoadFacoryFontCP(void);
#if 0
extern void Osd_SetText4Color(color);
extern void Osd_SetText8Color(color);
#else
extern void Osd_Set256TextColor(BYTE foreColor, BYTE backColor);
#endif
extern void Osd_LoadDCROffPalette(BYTE addr, BYTE *colorPtr, WORD num);
extern void Osd_DrawPropStr(BYTE xPos, BYTE yPos, BYTE *str);
extern void MonoColorSetting(void);
extern void Osd_SetTextMonoColor(BYTE foreColor, BYTE backColor);
extern void Osd_LoadMainMenuIconFont(void);
extern void Osd_DrawMainIcon(BYTE itemIndex, BYTE xpos, BYTE ypos, BYTE selected);
extern void Osd_SetTransparence(BYTE value);
extern void Osd_LoadMainFactoryFont(void);
extern void LoadMainSmallLogoFont(void);
extern void LoadCommonFont1(void);
extern void Osd_LoadCompressColorFont(BYTE addr, BYTE *fontPtr, WORD *mapPtr, BYTE count);

extern void Osd_LoadFont(BYTE addr, BYTE *fontPtr, WORD num);
extern void OSD_LoadEnergyStarFont(void);
extern void OSD_LoadStandardEcoIconFont(void);
extern void OSD_LoadSportEcoIconFont(void);
extern void OSD_LoadTextEcoIconFont(void);
extern void OSD_LoadMovieEcoIconFont(void);
extern void OSD_LoadInternetEcoIconFont(void);
extern void OSD_LoadGameEcoIconFont(void);
#if AudioFunc
extern void OSD_LoadVolIconFont(void);
#else
extern void LoadHotKeyConFont(void);
#endif
#if EnableHotBrightness
extern void LoadHotKeyBriFont(void);
#endif
extern void LoadHotKeyDCRFont(void);
#if Enable_Lightsensor // 090416
extern void LoadHotKeyiCareFont(void);
#endif
#if HotExpansionAdjust
extern void LoadHotKeyExpansionFont(void);
#endif
extern void OSD_AutoIconFont(void);
extern void SetOsdSubWindow( BYTE Win_Num, BYTE HStar, BYTE HEnd , BYTE VStar, BYTE VEnd, BYTE Color );
extern void DisableOsdSubWindow( BYTE Win_Num );
extern void SetOsdGraduallyBackGround( BYTE Win_Num, BYTE GDValue, BYTE GDDelta , BYTE GDDERange, BYTE GDControl, BYTE GDInit );

