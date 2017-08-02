///////////////////////////////////////////////////////////////////////////////
/// @file UserPref.h
/// @brief Head file of UserPref.c
/// @author MStarSemi Inc.
///
/// Functions to Init/Read/Write user preference data
///
/// Features
///  -Monitor user preference setting init/read/write.
///  -Display mode setting init/write/write.
///  -
///  -
///////////////////////////////////////////////////////////////////////////////

#ifndef _USERPREF_H
#define _USERPREF_H
#include "Board.h"
#include "UserPrefAPI.h"

#ifdef _USERPREF_C_
#define _USERPREF_DEC_
#else
#define _USERPREF_DEC_ extern
#endif


_USERPREF_DEC_ void EDID_ReLoad_Func(BYTE Reload);

_USERPREF_DEC_ void FactoryBlockInit(void);
_USERPREF_DEC_ void FactoryBlockRead(void);
_USERPREF_DEC_ void FactoryBlockSave(void);
_USERPREF_DEC_ void SystemBlockInit(void);
_USERPREF_DEC_ void SystemBlockRead(void);
_USERPREF_DEC_ void SystemBlockSave(void);
_USERPREF_DEC_ void ColorParameterBlockInit(void);
_USERPREF_DEC_ void ColorParameterBlockSave(void);
#if ENABLE_VGA_INPUT
_USERPREF_DEC_ void ModeBlockInit(void);
_USERPREF_DEC_ void ModeBlockRead(void);
_USERPREF_DEC_ void ModeBlockSave(void);
#endif
_USERPREF_DEC_ void NvramMapDataInit(void);

_USERPREF_DEC_ void ReadMonitorSetting(void);
_USERPREF_DEC_ void SaveMonitorSetting(void);
_USERPREF_DEC_ void SaveFactorySetting(void);
#if ENABLE_VGA_INPUT
_USERPREF_DEC_ void SaveModeSetting(void);
_USERPREF_DEC_ void ReadModeSetting(void);
#endif
_USERPREF_DEC_ void HDCP2_Read(Bool ucCtl);

#undef _USERPREF_DEC_

#endif   /// _USERPREF_H
