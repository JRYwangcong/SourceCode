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

#define MAPP_ZUI_APICHINESEIME_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


#include <stdlib.h>
#include <string.h>
#include "datatype.h"
#ifdef ENABLE_BT
#include "MApp_ZUI_APIChineseIME.h"
#include "MApp_ZUI_APIChineseIMELib.h"

/////////////////////////////////////////////////////////////////////////////////
//Example:  want to input "°¡"
//
//  step1.  input  'a'      display: < 0:a 1: ai  2: an  3: ang  4: ao >
//  step2.  choose  0       display: < 0: °¡   1:  °¢   2:  ß¹    3:  àÄ   4:  ëç   5:  ï¹>
//  step3.  choose  0       display:  °¡
//
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetPinYinInfoByFistChar
///  Mainly for ZUI OSD display.
///
///  NOTE:
///
///  @param [in]       c  U8  => input one char
///  @param [out]     pstart U8*  => the first position of these pinyin
///  @param [out]     pnumber U16*  => numbers of the group of pinyin
///
///  @return
///
///  @author MStarSemi @date 2008/11/27
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_GetPinYinInfoByFistChar(U8 c, U16 *pstart, U16 *pnumber)
{
    U8 i, ndx;

    *pstart = 0;
    if (c >= 'a' && c <= 'z')
    {
        ndx = c - 'a';
        *pnumber = PinYinNumber[ndx];
        for (i = 0; i < ndx; i ++)
        {
            *pstart += PinYinNumber[i];
        }
    }
    else
    {
        *pnumber = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetStringByFirstChar
///  Get one pinyin by index number.  Mainly for ZUI OSD display.
///
///  NOTE:
///
///  @param [in]       c  U8  => input one char
///  @param [in]     offset U16  => input pinyin index number
///  @param [out]     pstr U8*  => output one pinyin
///
///  @return
///
///  @author MStarSemi @date 2008/11/27
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_GetStringByFirstChar(U8 c, U16 offset, U8 *pstr)
{
    U16 start, number;
    U8 *ptr;

    MApp_ZUI_API_GetPinYinInfoByFistChar(c, &start, &number);
    if (offset < number)
    {
        ptr = PinYinArray[start +offset];
        while (*ptr != '\0')
        {
            *pstr = *ptr;
            pstr ++;
            ptr ++;
        }
        *pstr = 0;
    }
    else
    {
        *pstr = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetWordsInfoByFistChar
///  Mainly for ZUI OSD display.
///
///  NOTE:
///
///  @param [in]       c  U8  => input one char
///  @param [in]     offset U16  => input pinyin index number
///  @param [out]     pstart U16*  => the first position of the words. They have the same pinyin.
///  @param [out]     pnumber U16*  => numbers of the group of words.
///
///  @return
///
///  @author MStarSemi @date 2008/11/27
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_GetWordsInfoByFistChar(U8 c, U16 offset, U16 *pstart, U16 *pnumber)
{
    U16 i, start0, number0;

    MApp_ZUI_API_GetPinYinInfoByFistChar(c, &start0, &number0);
    *pstart = 0;
    if (offset < number0)
    {
        start0 += offset;
        *pnumber = WordsNumber[start0];
        for (i = 0; i < start0; i ++)
        {
            *pstart += WordsNumber[i];
        }
    }
    else
    {
        *pnumber = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
///  global  GetStringByFirstChar
///  Mainly for ZUI OSD display.
///
///  NOTE:
///
///  @param [in]       c  U8  => input one char
///  @param [in]     offset U16  => input pinyin index number
///  @param [in]     wofs U16  => input one word's index
///
///  @return one word's unicode
///
///  @author MStarSemi @date 2008/11/27
///////////////////////////////////////////////////////////////////////////////
U16 MApp_ZUI_API_GetWordsByPinYin(char c, U16 offset, U16 wofs)
{
    U16 start, number;

    MApp_ZUI_API_GetWordsInfoByFistChar(c, offset, &start, &number);
    if (number != 0 && wofs < number)
    {
        return WordsUnicode[start + wofs];
    }
    else
    {
        return 0;
    }
}

#endif

#undef MAPP_ZUI_APISTRINGS_C
