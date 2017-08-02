///////////////////////////////////////////////////////////////////////////////
/// @file UserPrefAPI.h
/// @brief Head file of UserPrefApi.c
/// @author MStarSemi Inc.
///
/// Functions to Read/Write user preference data
///
/// Features
///  -Monitor user preference setting init/read/write.
///  -Display mode setting init/write/write.
///  -
///  -
///////////////////////////////////////////////////////////////////////////////
#ifndef _USERPREFAPI_H
#define _USERPREFAPI_H

#include "EepromMap.h"

extern void SaveDiffToEEPROM(WORD u16Addr, WORD u16Cnt);
extern void SaveNVRAMBlock(WORD u16Addr, WORD u16Num);
extern void ReadNVRAMBlock(WORD u16Addr, WORD u16Cnt);

extern ST_EEP_USER_DATA *pUserPref;


#define _WinSelIDBackup()\
        BYTE u8BackUpCurrentWin = USER_PREF_WIN_SEL;

#define _WinSelIDForceSet(u8Win)\
         USER_PREF_WIN_SEL = u8Win;

#define _WinSelIDRestore()\
        USER_PREF_WIN_SEL = u8BackUpCurrentWin;


#endif   /// _USERPREF_H

