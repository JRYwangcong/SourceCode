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

#define MAPP_ZUI_APISTRINGS_C
#define _ZUI_INTERNAL_INSIDE_ //NOTE: for ZUI internal


#include <stdlib.h>
#include <string.h>
#include "Board.h"
#include "datatype.h"
#include "MsCommon.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "MApp_GlobalSettingSt.h"
#include "MApp_ZUI_Main.h"
#include "MApp_ZUI_APIcommon.h"
#include "MApp_ZUI_APIstrings.h"
#include "MApp_ZUI_APIwindow.h"
#include "MApp_ZUI_APItables.h"
#include "MApp_ZUI_ACTglobal.h"
//#include "MApp_GlobalVar.h"

#include "OSDcp_readbin.h" //for OSDcp_readbin_string_ptr()
#include "MApp_GlobalFunction.h" //for MApp_GetMenuLanguage()
//#include "MApp_RestoreToDefault.h"
#include "OSDcp_String_EnumIndex.h" //for LANG_PAGE_MAX

/*/ ZUI: use MApp_ZUI_API_Strlen()
///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_U16StringCopy
///  get a string pointer from a U16 integer
///  NOTE: assume input string is located in XDATA!
///
///  @param [in]        str LPTSTR   source string buffer
///
///  @return U16  string length
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
U16 MApp_ZUI_API_U16StringLength (LPTSTR str)
{
    U16 len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}*/

/*/ ZUI: use MApp_ZUI_API_Strcpy()
///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_U16StringCopy
///  get a string pointer from a U16 integer
///  NOTE: assume input string is located in XDATA!
///
///  @param [out]       dest LPTSTR   dest string buffer
///  @param [in]        src LPTSTR   source string buffer
///
///  @return U16  string length
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
U16 MApp_ZUI_API_U16StringCopy (LPTSTR dest, LPTSTR src)
{
    U16 len = 0;
    while (src[len])
    {
        dest[len] = src[len];
        len++;
    }
    dest[len] = 0;
    return len;
}*/

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_StringBuffU8toU16
///  Transform U8 string to U16 string. Mainly for ZUI OSD display.
///    (always return CHAR_BUFFER[])
///  NOTE: assume input string is located in XDATA!
///
///  @param [in]       pStrDest U16*  => Destination of string
///  @param [in]       pStrSource U8*  => content of string
///  @param [in]       u16Length U16  => length of string
///
///  @return LPTSTR   pStrDest
///
///  @author MStarSemi @date 2008/3/6
///////////////////////////////////////////////////////////////////////////////
LPTSTR MApp_ZUI_API_StringBuffU8toU16(LPTSTR pStrDest, U8* pStrSource, U16 u16Length)
{
    {
        U16 i;
        for (i = 0; i<=u16Length; i++)
        {
            (pStrDest)[i] = pStrSource[i];
            if((pStrDest)[i]==0)
                break;
        }
    }
    return pStrDest;
}



///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetString
///  get a string pointer from string ID, used for localization data.
///    (always return CHAR_BUFFER[])
///
///  @param [in]       id U16     string ID
///
///  @return LPTSTR string content
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
LPTSTR MApp_ZUI_API_GetString(U16 id)
{
    U8 u8Language = GET_OSD_MENU_LANGUAGE();

    if (u8Language >= LANG_PAGE_MAX) //check numbers of language
        u8Language = 0;
    if (id >= E_ZUI_STR_MAX) //check numbers of strings
        id = 0;

    OSDcp_readbin_string_ptr(u8Language, id, (U16*)CHAR_BUFFER);

    return CHAR_BUFFER;
}

// add function for catact string
LPTSTR MApp_ZUI_API_PrintU16String(U8*pu8Fmt,...)
{
    U16 Dest[0x64]={0};
    U8 DestinationIndex = 0;
    U8 bufferIndex = 0;
    U16 id;
    U8 u8Language = GET_OSD_MENU_LANGUAGE();
    va_list va;
    va_start(va,pu8Fmt);
    memset(CHAR_BUFFER,0x00,STRING_BUFFER_LENGTH);
    while(*pu8Fmt)
    {
        if(*pu8Fmt=='%')
        {
            switch(*++pu8Fmt)
            {
                case 's':
                    pu8Fmt++;
                    id = (U16)va_arg(va,int);
                    OSDcp_readbin_string_ptr(u8Language, id, (U16*)Dest);
                    DestinationIndex = 0;
                    while(Dest[DestinationIndex]!=0)
                    {
                        if(CHAR_BUFFER[bufferIndex]==0)
                        {
                            CHAR_BUFFER[bufferIndex++] = Dest[DestinationIndex++];
                        }
                        else
                        {
                        
                            bufferIndex++;
                        }
                    }//*/
                    break;
                 case 'd':
                    pu8Fmt++;
                    id = (U16)va_arg(va,int);
                    MApp_UlongToU16String(id, Dest, MApp_GetNoOfDigit(id));
                    DestinationIndex = 0;
                    while(Dest[DestinationIndex]!=0)
                    {
                        if(CHAR_BUFFER[bufferIndex]==0)
                        {
                            CHAR_BUFFER[bufferIndex++] = Dest[DestinationIndex++];
                        }
                        else
                        {
                        
                            bufferIndex++;
                        }
                    }//*/
                    break;
            
            }
        }
        else
        {
            CHAR_BUFFER[bufferIndex++]=*pu8Fmt++;
            //continue;
        }


    }
    
    va_end(va);
    CHAR_BUFFER[bufferIndex] = 0;
    return CHAR_BUFFER;    


}


///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetStringSpecificLanguage
///  get a string pointer from string ID with specific language, used for localization data.
///    (always return CHAR_BUFFER[])
///
///  @param [in]       id U16     string ID
///  @param [in]       u8Language U8     language ID
///
///  @return LPTSTR string content
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
LPTSTR MApp_ZUI_API_GetStringSpecificLanguage(U16 id, U8 u8Language)
{

    if (u8Language >= LANG_PAGE_MAX) //check numbers of language
        u8Language = 0;
    if (id >= E_ZUI_STR_MAX) //check numbers of strings
        id = 0;

    OSDcp_readbin_string_ptr(u8Language, id, (U16*)CHAR_BUFFER);

    return CHAR_BUFFER;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_LoadString
///  get a string pointer from string ID, used for localization data
///   (load to user private string location)
///
///  @param [in]       id U16     string ID
///  @param [out]      str LPTSTR   string buffer
///
///  @return no return value
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
void MApp_ZUI_API_LoadString(U16 id, LPTSTR str)
{
    U8 u8Language = GET_OSD_MENU_LANGUAGE();

    if (u8Language >= LANG_PAGE_MAX) //check numbers of language
        u8Language = 0;
    if (id >= E_ZUI_STR_MAX) //check numbers of strings
        id = 0;

    OSDcp_readbin_string_ptr(u8Language, id, (U16*)str);
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetU16String
///  get a string pointer from a U16 integer
///
///  @param [in]       value U16     integer
///
///  @return LPTSTR string content
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
LPTSTR MApp_ZUI_API_GetU16String (U16 value)
{
    MApp_UlongToU16String(value, (U16*)CHAR_BUFFER, MApp_GetNoOfDigit(value));
    return CHAR_BUFFER;
}
///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetU16String
///  get a hex string pointer from a U16 integer
///
///  @param [in]       value U16     integer
///
///  @return LPTSTR string content
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
LPTSTR MApp_ZUI_API_GetU16HexString(U16 value)
{
    MApp_HexUlongToU16String((U32)value, (U16*)CHAR_BUFFER, (S8)MApp_GetNoOfHexDigit((U32)value));
    return CHAR_BUFFER;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetS16SignString
///  get a sign string pointer from a S16 integer
///
///  @param [in]       value S16     integer
///
///  @return LPTSTR string content
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
LPTSTR MApp_ZUI_API_GetS16SignString (S16 value)
{
    LPTSTR str = CHAR_BUFFER;
    if (value < 0)
    {
        str[0] = '-';
        MApp_UlongToU16String(-value, str+1, MApp_GetNoOfDigit(-value));
    }
    else if (value > 0)
    {
        str[0] = '+';
        MApp_UlongToU16String(value, str+1, MApp_GetNoOfDigit(value));
    }
    else //if (value == 0)
    {
        str[0] = '0';
        str[1] = 0;
    }
    return CHAR_BUFFER;
}

/*
void MApp_ZUI_API_ConvertStringU8ToU16(U16 * pu16Str, U8 * pu8Str)
{
    while( *pu8Str )
    {
        *pu16Str++ = *pu8Str++;
    }
    *pu16Str = 0;

}
*/

#if 0
static U8 CHAR_BUFFER[256];
//static U16 _GUI_StringLangPage = LANG_English;

static U16* p_copy_string_buffer = (U16*) CHAR_BUFFER2;

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_GetString
///  get a string pointer from string ID, used for localization data
///
///  @param [in]       idx STRING_ID_ENUM     string ID
///
///  @return LPTSTR string content
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
LPTSTR MApp_ZUI_API_GetString(U16 id)
{
    /*if (idx < STR_MAX)
        return _GUI_StringTable[_GUI_StringLangPage][idx];

    PRINT("MApp_ZUI_API_GetString(): illegal idx (%d) !\n", idx);
    ABORT();
    return "";*/
    U8 *u8String;

    //PRINT("\n[GetString] langid = %d, string id = %d", _GUI_StringLangPage, id);
    CHAR_BUFFER[0] = 0; //TODO: OSDcp_readbin_string_ptr(MENU_LANGUAGE, id, (U16*)CHAR_BUFFER);
    //PRINT("\n[GetString] get string %s", (char *)CHAR_BUFFER);
    u8String = MApp_Menu_Strcpy(CHAR_BUFFER, CHAR_IDX_2BYTE);
    //PRINT("\n[GetString] %d", (U16)(*u8String));
    //PRINT("\n[GetString] %d", (U16)*(u8String+1));
    //PRINT("\n[GetString] %d", (U16)*(u8String+2));
    //PRINT("\n[GetString] %d", (U16)*(u8String+3));

    return u8String;

    //return (U8*)CHAR_BUFFER;
}

///////////////////////////////////////////////////////////////////////////////
///  global  MApp_ZUI_API_SetStringLangPage
///  change string code page to a specific language
///
///  @param [in]       idx LANG_PAGE_ENUM     language ID
///
///  @return BOOL TRUE for success, FALSE for fail
///
///  @author MStarSemi @date 2007/1/25
///////////////////////////////////////////////////////////////////////////////
BOOLEAN MApp_ZUI_API_SetStringLangPage(U16 idx)
{
    /*if (idx < LANG_MAX && idx != _GUI_StringLangPage)
    {
        _GUI_StringLangPage = idx;
        return TRUE; //ok!
    }*/

    //TODO: MENU_LANGUAGE = idx;
    return TRUE;

    /*PRINT("MApp_ZUI_API_SetStringLangPage(): illegal lang!\n");
    ABORT();
    return FALSE;*/
}

#endif

#undef MAPP_ZUI_APISTRINGS_C
