#define _USERPREFAPI_C
#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "NVRam.h"
#include "MDebug.h"
#include "Keypaddef.h"
#include "Common.h"
#include "menudef.h"
//#include "UserPref.h"
#include "UserPrefDef.h"
//#include "UserPrefAPI.h"
#include "msflash.h"
#include "misc.h"
#include "ms_reg.h"

#include "Mode.h"

#define USER_PREF_API_DBG   1
#if (ENABLE_MSTV_UART_DEBUG & USER_PREF_API_DBG)
    #define UPA_printData(str, value)   printData(str, value)
    #define UPA_printMsg(str)           printMsg(str)
#else
    #define UPA_printData(str, value)
    #define UPA_printMsg(str)
#endif

ST_EEP_USER_DATA *pUserPref = (ST_EEP_USER_DATA*)u8NVRAM_MAP;

/****************************************************************
*                                                               *
*   Compare the diff between u8NVRAM_MAP and u8NVRAM_MAP_BK.    *
*   Update the final data from u8NVRAM_MAP to u8NVRAM_MAP_BK.   *
*   Save the u8NVRAM_MAP_BK to EEPROM                           *
*                                                               *
****************************************************************/
#define CHECK_SAVE_DIFF_ERROR   1

void SaveDiffToEEPROM(WORD u16Addr, WORD u16Cnt)
{
    WORD u16Index = 0x0000;
    WORD u16StartAddr = 0x0000;
    WORD u16EndAddr = 0x0000;
    WORD u16Counter = 0x0000;

    for(u16Index=0; u16Index<u16Cnt; u16Index++)
    {
        if(u16Addr+u16Index >= NVRAM_SIZE)
        {
            break;
        }

        if((u8NVRAM_MAP[u16Addr+u16Index]) != (u8NVRAM_MAP_BK[u16Addr+u16Index]))
        {
            //u8NVRAM_MAP_BK[u16Index] = u8NVRAM_MAP[u16Index];   //Update to Backup Map.
            if(u16Counter==0)   //If is empty
            {
                u16StartAddr = u16Addr+u16Index;
                u16EndAddr = u16Addr+u16Index;
                u16Counter++;
            }
            else
            {
                if(u16EndAddr != (u16Addr+u16Index-1))  //Not continue
                {
                    UPA_printData("u16Addr: 0x%x", u16StartAddr);
                    UPA_printData("u16Counter: 0x%x", u16Counter);

                    NVRam_WriteTbl(u16StartAddr, (BYTE *)(&u8NVRAM_MAP[u16StartAddr]), u16Counter);

                    //Update start/end addr, counter set to 1.
                    u16StartAddr = u16Addr+u16Index;
                    u16EndAddr = u16Addr+u16Index;
                    u16Counter = 1;
                }
                else    //Is continue
                {
                    u16EndAddr = u16Addr+u16Index;  //Update end addr.
                    u16Counter++;
                }
            }
        }
    }
    //If still have dirty data, save to EEPROM.
    if(u16Counter)
    {
        UPA_printData("Last u16Addr: 0x%x", u16StartAddr);
        UPA_printData("Last u16Counter: 0x%x", u16Counter);
        NVRam_WriteTbl(u16StartAddr, (BYTE *)&u8NVRAM_MAP[u16StartAddr], u16Counter);
    }

    NVRam_ReadTbl(u16Addr, (BYTE *)&u8NVRAM_MAP_BK[u16Addr], u16Cnt);
}

void SaveNVRAMBlock(WORD u16Addr, WORD u16Num)
{
    if(SAVE_DIFF_ENABLE_FLAG)
    {
        #if (CHECK_SAVE_DIFF_ERROR)
        WORD u16i;
        BYTE u8Temp = 0;

        //UPA_printMsg("Save Diff Enable.");
        #endif

        SaveDiffToEEPROM(u16Addr, u16Num);

        #if (CHECK_SAVE_DIFF_ERROR)
        for(u16i=0; u16i<u16Num; u16i++)
        {
            if(u8NVRAM_MAP[u16Addr+u16i] != u8NVRAM_MAP_BK[u16Addr+u16i])
            {
                NVRam_ReadByte(u16Addr+u16i, &u8Temp);
                if(u8NVRAM_MAP[u16Addr+u16i] != u8Temp)
                {
                    UPA_printData("Data Fail. addr: 0x%x", u16Addr+u16i);
                    break;
                }
                else
                {
                    UPA_printMsg("BK NG");
                }
            }
        }
        #endif
    }
    else
    {
        #if (CHECK_SAVE_DIFF_ERROR)
        WORD u16Cnt;
        //UPA_printMsg("Save Diff Disable.");
        #endif

        NVRam_WriteTbl(u16Addr, (BYTE *) &u8NVRAM_MAP[u16Addr], u16Num);
        NVRam_ReadTbl(u16Addr, (BYTE *) &u8NVRAM_MAP_BK[u16Addr], u16Num);

        #if (CHECK_SAVE_DIFF_ERROR)
        for(u16Cnt=0; u16Cnt<u16Num; u16Cnt++)
        {
            if(u8NVRAM_MAP_BK[u16Addr+u16Cnt] != u8NVRAM_MAP[u16Addr+u16Cnt])
            {
                UPA_printData("Fail Addr: 0x%x", u16Addr+u16Cnt);
                break;
            }
        }
        #endif
    }
}

void ReadNVRAMBlock(WORD u16Addr, WORD u16Cnt)
{
    WORD u16Counter = 0;
    NVRam_ReadTbl(u16Addr, (BYTE *)&u8NVRAM_MAP_BK[u16Addr], u16Cnt);
    for(u16Counter=0; u16Counter<u16Cnt; u16Counter++)
    {
        u8NVRAM_MAP[u16Addr+u16Counter] = u8NVRAM_MAP_BK[u16Addr+u16Counter];
    }
}
