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

#ifndef _MAPP_ZUI_APICOMMON_H
  #define _MAPP_ZUI_APICOMMON_H

#include "datatype.h"
#if ENABLE_DMP
#include "MApp_UiMediaPlayer_Define.h"
#endif



    /**
     * \defgroup handles ZUI data types
     * @{
     */


/**
 * \var typedef unsigned int HWND
 * \brief Handle to main window or control. (16 bits, maximum 65535)
 */
typedef U16 HWND;


/**
 * \var typedef unsigned int WPARAM
 * \brief A type definition for the first message paramter. (32 bits)
 */
typedef U32    WPARAM;

/**
 * \var typedef unsigned long WPARAM
 * \brief A type definition for the second message paramter.
 */
//typedef U32   LPARAM;


/**
 * \var LPCTSTR
 * \brief 8/16 bits Character indepentdent: constant string pointer
 *
*/
/**
 * \var LPTSTR
 * \brief 8/16 bits Character indepentdent: string pointer
 *
*/
/**
 * \var TCHAR
 * \brief 8/16 bits Character indepentdent: character type
 *
*/
typedef U16              TCHAR;
typedef const TCHAR *      LPCTSTR;
typedef TCHAR *            LPTSTR;
#define SIZEOF_TCHAR        2

/////////////////////////////////////////
// Event Prototypes....
// note: don't use function pointer anymore...
//typedef void (*CALLBACK_ACTION_EXEC)(HWND sender);
//typedef void (*CALLBACK_SEL_CHANGED)(HWND sender, U16 sel_index);


/**
 * Size structure of a 2-dimension object.
 */
typedef struct _SIZE
{
    /**
     * the extent in x coordinate of a 2D object.
     */
    S16 cx;
    /**
     * the extent in y coordinate of a 2D object.
     */
    S16 cy;
} tSIZE;

/**
 * \def VIRTUAL_KEY_CODE
 * \brief system independent key code (maximum 63)
 *
*/
typedef enum
{
    VK_NULL = 0,

    VK_TV_RADIO,
    VK_CHANNEL_LIST,
    VK_CHANNEL_FAV_LIST,
    VK_CHANNEL_RETURN,
    VK_CHANNEL_PLUS,
    VK_CHANNEL_MINUS,

    VK_AUDIO,
    VK_IMAGE,
    VK_VIDEO,
    VK_DVD,
    VK_SETUP,

    VK_VOLUME_PLUS,
    VK_VOLUME_MINUS,

    VK_UP,
    VK_POWER,
    VK_EXIT,
    VK_MENU,
    VK_DOWN,
    VK_LEFT,
    VK_SELECT,
    VK_RIGHT,

    VK_NUM_0,
    VK_NUM_1,
    VK_NUM_2,
    VK_NUM_3,
    VK_NUM_4,
    VK_NUM_5,
    VK_NUM_6,
    VK_NUM_7,
    VK_NUM_8,
    VK_NUM_9,

    VK_MUTE,
    VK_PAGE_UP,
    VK_PAGE_DOWN,
    VK_CLOCK,

    VK_INPUT_SOURCE,

    VK_EPG,
    VK_INFO,
    VK_BACK,
    VK_MTS,
    VK_TTX,

    VK_RED,
    VK_GREEN,
    VK_YELLOW,
    VK_BLUE,

    VK_PICTURE,
    VK_CC,
    VK_ZOOM,
    VK_FREEZE,
    VK_INDEX,

    VK_SLEEP,

    VK_DSC_KEY_ARC4X3,
    VK_DSC_KEY_ARC16X9,
    VK_DSC_KEY_ARCZOOM,

    VK_PLAY,
    VK_PAUSE,
    VK_STOP,
    VK_NEXT,
    VK_PREVIOUS,
    VK_FF,
    VK_REWIND,
    VK_RECORD,

    VK_SUBTITLE,
    VK_UPDATE,
    VK_EPGTIMER_COUNTDOWN,

    VK_DASH,

    VK_PIP,
    VK_HOME,
    VK_MAX = 69,
} VIRTUAL_KEY_CODE;

/**
 * \def TABLESIZE(table)
 * \brief A macro returns the number of elements in a \a table.
 */
#define TABLESIZE(table)    (sizeof(table)/sizeof(table[0]))

/* MAX/MIN/ABS macors */
/**
 * \def MAX(x, y)
 * \brief A macro returns the maximum of \a x and \a y.
 */
#ifndef MAX
#define MAX(x, y)           (((x) > (y))?(x):(y))
#endif
/**
 * \def MIN(x, y)
 * \brief A macro returns the minimum of \a x and \a y.
 */
#ifndef MIN
#define MIN(x, y)           (((x) < (y))?(x):(y))
#endif
/**
 * \def ABS(x)
 * \brief A macro returns the absolute value of \a x.

#ifndef ABS
#define ABS(x)              (((x)<0) ? -(x) : (x))
#endif*/


/*#ifndef PATH_MAX
    #define PATH_MAX    256
#endif

#ifndef NAME_MAX
    #define NAME_MAX    64
#endif*/


/**
 * \def MAX_PATH
 * \brief The possible maximal length of a path name.
 * \note This definition is an alias of PATH_MAX
 */
//#define MAX_PATH        PATH_MAX
/**
 * \def MAX_NAME
 * \brief The possible maximal length of a file name.
 * \note This definition is an alias of NAME_MAX
 */
//#define MAX_NAME        NAME_MAX

    /** @} end of misc_macros */

    /** @} end of macros_types */


#endif /* _MAPP_ZUI_APICOMMON_H */


