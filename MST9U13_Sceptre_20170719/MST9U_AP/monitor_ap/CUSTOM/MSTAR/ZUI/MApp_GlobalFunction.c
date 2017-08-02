////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define MAPP_GLOBAL_FUNCTION_C

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <string.h>
#include <math.h>
#include "datatype.h"

#include "MApp_GlobalFunction.h"

static EN_LANGUAGE enLanguage = LANGUAGE_DEFAULT;
//*******************************************************************************
//             Functions
//******************************************************************************

//*************************************************************************
//Function name:    MApp_GetNoOfDigit
//Passing parameter:    U32 u32Data: Value need to be calculated
//Return parameter:     U8: number of digit
//Description:      Calculate the digit number of u32Data
//*************************************************************************
U8 MApp_GetNoOfDigit ( U32 u32Data )
{
    U8 u8NoOfDigit;

    u8NoOfDigit = 0;
    do
    {
        u32Data /= 10;
        u8NoOfDigit++;
    }
    while ( u32Data != 0 );

    return u8NoOfDigit;
}
//*************************************************************************
//Function name:    MApp_UlongToU16String
//Passing parameter:    U32 ulValue: Value need to be transfered into string
//              U16* bArrOutput: Transfered string pointer
//              S8 NoOfDigit: The number of digits need to transfer
// Return parameter:    none
//Description:      Transfer numer to u16string
//*************************************************************************
void MApp_UlongToU16String ( U32 ulValue, U16 *pArrOutput, S8 NoOfDigit )
{
    pArrOutput[NoOfDigit] = 0;
    NoOfDigit -= 1;

    while ( ulValue )
    {
        pArrOutput[NoOfDigit--] = ( ulValue % 10 ) + 0x30;
        ulValue /= 10;
    }
    while ( NoOfDigit >= 0 )
    {
        pArrOutput[NoOfDigit] = '0';
        NoOfDigit--;
    }
}

//*************************************************************************
//Function name:    MApp_UlongToU8String
//Passing parameter:    U32 ulValue: Value need to be transfered into string
//              U16* bArrOutput: Transfered string pointer
//              S8 NoOfDigit: The number of digits need to transfer
// Return parameter:    none
//Description:      Transfer numer to u8string
//*************************************************************************
void MApp_UlongToU8String ( U32 ulValue, U8 *pArrOutput, S8 NoOfDigit )
{
    pArrOutput[NoOfDigit] = 0;
    NoOfDigit -= 1;

    while ( ulValue )
    {
        pArrOutput[NoOfDigit--] = ( ulValue % 10 ) + 0x30;
        ulValue /= 10;
    }
    while ( NoOfDigit >= 0 )
    {
        pArrOutput[NoOfDigit] = '0';
        NoOfDigit--;
    }
}

//*************************************************************************
//Function name:    MApp_HexUlongToU16String
//Passing parameter:    U32 ulValue: Value need to be transfered into string
//              U16* bArrOutput: Transfered string pointer
//              S8 NoOfDigit: The number of digits need to transfer
// Return parameter:    none
//Description:      Transfer numer to u16string
//*************************************************************************
//#if (KEEP_UNUSED_FUNC == 1)
void MApp_HexUlongToU16String ( U32 ulValue, U16 *pArrOutput, S8 NoOfDigit )
{
    U8 u8NumValue = 0;

    pArrOutput[NoOfDigit] = 0;
    NoOfDigit -= 1;

    while ( ulValue )
    {
        u8NumValue = ulValue % 0x10;
        if(u8NumValue <= 9)
            pArrOutput[NoOfDigit] = u8NumValue + 0x30;
        else
            pArrOutput[NoOfDigit] = (u8NumValue-0x0A) + 'A';
        NoOfDigit--;
        ulValue /= 0x10;
    }
    while ( NoOfDigit >= 0 )
    {
        pArrOutput[NoOfDigit] = '0';
        NoOfDigit--;
    }
}
//#endif

void MApp_HexUlongToU8String ( U32 ulValue, U8 *pArrOutput, S8 NoOfDigit )
{
    U8 u8NumValue = 0;

    pArrOutput[NoOfDigit] = 0;
    NoOfDigit -= 1;

    while ( ulValue )
    {
        u8NumValue = ulValue % 0x10;
        if(u8NumValue <= 9)
            pArrOutput[NoOfDigit] = u8NumValue + 0x30;
        else
            pArrOutput[NoOfDigit] = (u8NumValue-0x0A) + 'A';
        NoOfDigit--;
        ulValue /= 0x10;
    }
    while ( NoOfDigit >= 0 )
    {
        pArrOutput[NoOfDigit] = '0';
        NoOfDigit--;
    }
}


//*************************************************************************
//Function name:    MApp_U8StringToU16String
//Passing parameter:    U8 *pu8Str: string pointer to be transfered
//              U16 *pu16Str: transfered string pointer
//              U8 u8Strlen: the number of characters to be transfered
// Return parameter:    none
//Description:      Transfer u8string to u16string
//*************************************************************************
void MApp_U8StringToU16String ( U8 *pu8Str, U16 *pu16Str, U8 u8Strlen )
{
    U8 u8Index;

    for ( u8Index = 0; u8Index < u8Strlen; u8Index++ )
    {
        pu16Str[u8Index] = pu8Str[u8Index];
    }
    pu16Str[u8Index] = 0;
}

U32 MApp_U8StringToUlong(U8* InputStr, U8 len)
{
    U32 ulValue = 0;
    U8 strIdx = 0;

    if(len<=1)
        return ulValue;

    for(strIdx=0; strIdx<len; strIdx++)
    {
        ulValue *= 10;
        ulValue += InputStr[strIdx] - '0';
    }

    return ulValue;
}

EN_LANGUAGE MApp_GetMenuLanguage(void)
{
#if 0 ///TODO: fixme
    if (stGenSetting.g_SysSetting.Language > LANGUAGE_MENU_MAX)
    {
        MS_DEBUG_MSG(printf("Get Lang Overflow: %u\n", (U16)stGenSetting.g_SysSetting.Language));
        stGenSetting.g_SysSetting.Language = DEFAULT_MENU_LANG;
#if ( ENABLE_ARABIC_OSD || ENABLE_THAI_OSD )
		msAPI_OSD_SetOSDLanguage((EN_OSDAPI_LANGUAGE)DEFAULT_MENU_LANG);
#endif
    }

    return stGenSetting.g_SysSetting.Language;
#endif
    return enLanguage;
}


void MApp_SetMenuLanguage(EN_LANGUAGE eLanguage)
{
#if 0 ///TODO: fixme
    if (eLanguage > LANGUAGE_MENU_MAX)
    {
        MS_DEBUG_MSG(printf("Set Lang Overflow: %u\n", (U16)eLanguage));
        eLanguage = DEFAULT_MENU_LANG;
    }

    stGenSetting.g_SysSetting.Language = eLanguage;

#if ( ENABLE_ARABIC_OSD || ENABLE_THAI_OSD )
  #if (ENABLE_ARABIC_TEST_UI)
    if ( eLanguage == LANGUAGE_SPANISH )
    {
        msAPI_OSD_SetOSDLanguage(LANGUAGE_OSD_ARABIC);
    }
    else
  #endif
    {
	msAPI_OSD_SetOSDLanguage((EN_OSDAPI_LANGUAGE)eLanguage);
    }
#endif

  #if ENABLE_AUTOTEST
    if(g_bAutobuildDebug)
    {
        if(OSD_L != stGenSetting.g_SysSetting.Language)
        {
            printf("71_OSD_Change_Language\n");
        }
        OSD_L = stGenSetting.g_SysSetting.Language;
    }
  #endif

  #if ENABLE_DTV
    MApp_SetSILanguage(eLanguage);
    #if (ENABLE_CI && ENABLE_CI_PLUS)
    {
        EN_SI_LANGUAGE eLangCodeId;
        U8 u8aLangCode[MAX_ISO639CODE_LENGTH] = { 0 };

        eLangCodeId = MApp_GetSILanguage(eLanguage);
        if (TRUE == msAPI_SI_GetISOLangCodeFromIndex(eLangCodeId, u8aLangCode))
            msAPI_CI_HLC_SetLanguage(u8aLangCode);
    }
    #endif

    #if ENABLE_TTX
    MApp_TTX_NotifyPMTTTXInfoChanged();
    #endif
  #endif
#endif

    enLanguage = eLanguage;
}

#undef MAPP_GLOBAL_FUNCTION_C
