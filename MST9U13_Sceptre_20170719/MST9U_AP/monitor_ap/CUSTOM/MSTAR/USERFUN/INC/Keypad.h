///////////////////////////////////////////////////////////////////////////////
/// @file Keypad.h
/// @brief Head file of Keypad.c
/// @author MStarSemi Inc.
///
/// Keypad handler function.
///
/// Features
///  -
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _KEYPAD_H
#define _KEYPAD_H

extern BYTE xdata g_u8KeyDebounceCounter;
extern BYTE xdata g_u8KeypadButton;

//extern xdata BYTE KeypadButton;
//extern BYTE xdata KeyDebug;

extern void Key_ScanKeypad( void );
extern void CheckACKeyStatus( void );

//extern void CheckFactoryKeyStatus( void );
#endif /// KEYPAD_C

