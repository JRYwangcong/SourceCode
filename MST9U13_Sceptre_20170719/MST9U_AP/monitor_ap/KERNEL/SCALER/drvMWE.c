#include "datatype.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "Common.h"
#include "drvMWE.h"
#include "msScaler.h"

#define drvMWE_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && drvMWE_DEBUG
#define drvMWE_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define drvMWE_PRINT(format, ...)
#endif

void msDrvMWEWindowEnable(BYTE u8ScalerIdx, BYTE u8MEWSel, Bool bMWEEnable, Bool bBorderEnable)
{
    DWORD u32BaseAddr = SCRegBase(u8ScalerIdx);

    msWriteBit(u32BaseAddr + SC20_20, bMWEEnable, BIT2);
    msWriteBit(u32BaseAddr + SC20_20, bMWEEnable, BIT7);
    msWriteBit(u32BaseAddr + SC0F_02, bBorderEnable, BIT7);       //sub border enable
    msWriteBit(u32BaseAddr + SC0F_02, bBorderEnable, BIT7);       //sub border enable

    msWriteByteMask(u32BaseAddr + SC0F_0C, u8MEWSel, 0x03);
    msWriteByteMask(u32BaseAddr + SC0F_0C, (bMWEEnable)<<(u8MEWSel+3), 0x70);

    msWriteBit(u32BaseAddr + SC10_32, !bMWEEnable, _BIT5);           // disable frame color
}

void msDrvMWEWindowConfig(BYTE u8ScalerIdx, BYTE u8MEWSel, ST_WINDOW_INFO sWinInfo)
{
    DWORD u32BaseAddr = SCRegBase(u8ScalerIdx);
    BYTE MWEAddrOffset = 0;

    sWinInfo.x += g_ScalerInfo[u8ScalerIdx].sSiOP2DisplayWin.x;
    sWinInfo.y += g_ScalerInfo[u8ScalerIdx].sSiOP2DisplayWin.y+msRead2Byte(u32BaseAddr + SC10_0C);

    MWEAddrOffset = u8MEWSel*8;
    msWriteByteMask(u32BaseAddr + SC0F_0C, u8MEWSel, 0x03);

    sWinInfo.x += msRead2Byte(u32BaseAddr + SC10_08);
    msWrite2Byte(u32BaseAddr + SC0F_0E, sWinInfo.x);    //DE
    msWrite2Byte(u32BaseAddr + SC0F_10, sWinInfo.x + sWinInfo.width -1 );
    msWrite2Byte(u32BaseAddr + SC0F_12, sWinInfo.y);
    msWrite2Byte(u32BaseAddr + SC0F_14, sWinInfo.y + sWinInfo.height -1);

    //msWriteByte(u32BaseAddr + SC0F_2E, 0x03);      //color
}

void msDrvMWEBorderColor(BYTE u8ScalerIdx, BYTE u8Color)
{
    DWORD u32BaseAddr = SCRegBase(u8ScalerIdx);

    msWrite2ByteMask(u32BaseAddr + SC0F_2E, u8Color, 0xFF);
}

void msDrvMWEBorderWidth(BYTE u8ScalerIdx, BYTE u8Upper, BYTE u8Down, BYTE u8Left, BYTE u8Right)
{
    DWORD u32BaseAddr = SCRegBase(u8ScalerIdx);
    msWrite2ByteMask(u32BaseAddr + SC0F_08, u8Upper, 0xFF); //border
    msWrite2ByteMask(u32BaseAddr + SC0F_0A, u8Down, 0xFF);
    msWrite2ByteMask(u32BaseAddr + SC0F_04, u8Left, 0xFF);
    msWrite2ByteMask(u32BaseAddr + SC0F_06, u8Right, 0xFF);

}

void msDrvMWEInit(void)
{
    msDrvMWEWindowEnable(SCL0_MAIN, eMWE_WIN2, FALSE, FALSE);
    msDrvMWEBorderColor(SCL0_MAIN,  0x03);
    msDrvMWEBorderWidth(SCL0_MAIN, 0x0F, 0x0F, 0x0F, 0x0F);
}


void msDrvOp2ReportWindowSetRange(BYTE u8WinIdx, WORD u16HStart, WORD u16HEnd, WORD u16VStart, WORD u16VEnd)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    drvMWE_PRINT("\r\n #### msDrvOp2ReportWindowSetRange u8WinIdx:%x ,u16HStart:%d, u16HEnd:%d, u16VStart:%d, u16VEnd:%d \r\n", u8WinIdx,u16HStart, u16HEnd,u16VStart, u16VEnd);

    while(u16ScalerMask)
        {
            if(u16ScalerMask & BIT0)
            {
                u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
                    //if( (u8ScalerIdx % 2) == 1)// Main Win
                    {
                msWriteByteMask(u32BaseAddr+SC10_34, BIT0,BIT0);           //Debug mode to show window position
                ForceDelay1ms(1000); //must disable it else it cant report correct value
                msWrite2Byte(u32BaseAddr + SC0F_88, u16VStart );
                msWrite2Byte(u32BaseAddr + SC0F_8A, u16VEnd);
                msWrite2Byte(u32BaseAddr + SC0F_84, u16HStart);
                msWrite2Byte(u32BaseAddr + SC0F_86, u16HEnd );
                    }
                    /*
                    else
                    {
                msWriteByteMask(u32BaseAddr+SC10_34, BIT0,BIT0);
                msWrite2Byte(u32BaseAddr + SC0F_88, u16VStart );
                msWrite2Byte(u32BaseAddr + SC0F_8A, u16VEnd);
                msWrite2Byte(u32BaseAddr + SC0F_84, u16HStart);
                msWrite2Byte(u32BaseAddr + SC0F_86, u16HEnd );
                    }
                    */
            }
            u8ScalerIdx++;
            u16ScalerMask >>= 1;
        }
}
void msDrvOp2ReportWindowEnable(BYTE u8WinIdx ,BYTE bEnable )
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    drvMWE_PRINT("\r\n #### msDrvOp2ReportWindowEnable u8WinIdx:%x , bEnable:%d \r\n", u8WinIdx,bEnable);

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            //if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWriteByte(u32BaseAddr + SC0F_AF, (bEnable ? (BIT1|BIT0): 0));
            }
            /*
            else
            {
                msWriteByte(u32BaseAddr + SC0F_AF, (bEnable ? (BIT1|BIT0): 0));
            }
            */
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}
static WORD u16CursorpointRGBMaxMinValue[DISPLAY_MAX_NUMS][6];
void msDrvOp2ReportWindowGetData(BYTE u8WinIdx )
{
    xdata BYTE  ucTmp =0;
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    drvMWE_PRINT("\r\n #### msDrvOp2ReportWindowGetData u8WinIdx:%x\r\n", u8WinIdx);
    drvMWE_PRINT("\r\n #### u16ScalerMask u8WinIdx:%x\r\n", u16ScalerMask);

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            msWriteByteMask(u32BaseAddr+SC10_34, 0,BIT0);   //disable debug mode to get correct data
            msWriteByteMask(u32BaseAddr+SC0F_8C, 0,0x0F);   //Stage select
            //if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                for (ucTmp = 0; ucTmp < 6; ucTmp++)
                {
                    u16CursorpointRGBMaxMinValue[u8WinIdx][ucTmp]=msRead2Byte(u32BaseAddr + SC0F_C8 +ucTmp*2);
                    drvMWE_PRINT("\r\n WindowReport_Rmin/Rmax/Gmin/Gmax/Bmin/Bmax  ADD=[%x],Value=[%x]",(u32BaseAddr + SC0F_C8 +ucTmp*2),u16CursorpointRGBMaxMinValue[u8WinIdx][ucTmp]);
                }
            }
            /*
            else
            {
                for (ucTmp = 0; ucTmp < 6; ucTmp++)
                {
                    u16CursorpointRGBMaxMinValue[u8WinIdx][ucTmp]=msRead2Byte(u32BaseAddr + SC0F_C8 +ucTmp*2);
                    drvMWE_PRINT("\r\n  YYYYYY u16CursorpointRGBMaxMinValue[u8WinIdx][ucTmp]===%x",u16CursorpointRGBMaxMinValue[u8WinIdx][ucTmp]);
                }
            }
            */
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
        }
}
void msDrvOp2CursorEnable(BYTE u8WinIdx ,BYTE bEnable )
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    //drvMWE_PRINT(" u16ScalerMask==%x",u16ScalerMask);

    drvMWE_PRINT("\r\n #### msDrvOp2CursorEnable u8WinIdx:%x ,bEnable:%x\r\n", u8WinIdx,bEnable);

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            //if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWriteByte(u32BaseAddr + SC0F_AF, (bEnable ? (BIT2|BIT0): 0));
            }
            /*
            else
            {
                msWriteByte(u32BaseAddr + SC0F_AF, (bEnable ? (BIT2|BIT0): 0));
            }
            */
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}
void msDrvOp2CursorSetCoordination(BYTE u8WinIdx, WORD u16x, WORD u16y)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    drvMWE_PRINT("\r\n #### msDrvOp2CursorSetCoordination u8WinIdx:%x ,u16x:%x ,u16y:%x  \r\n", u8WinIdx,u16x,u16y);

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            //if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWrite2Byte(u32BaseAddr + SC0F_32, u16x );
                msWrite2Byte(u32BaseAddr + SC0F_34, u16y);
            }
            /*
            else
            {
                msWrite2Byte(u32BaseAddr + SC0F_32, u16x );
                msWrite2Byte(u32BaseAddr + SC0F_34, u16y);
            }
            */
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}
static WORD u16CursorpointRGBValue[DISPLAY_MAX_NUMS][3];
void msDrvOp2CursorGetRGBValue(BYTE u8WinIdx )
{
    xdata BYTE  ucTmp =0;
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    drvMWE_PRINT("\r\n #### msDrvOp2CursorGetRGBValue u8WinIdx:%x  \r\n", u8WinIdx);

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            msWriteByteMask(u32BaseAddr+SC10_30, 0,0x0F);   //Stage select
            msWriteByteMask(u32BaseAddr+SC10_31, BIT0,BIT0);   //SW read trigger
            msWriteByteMask(u32BaseAddr+SC10_31, 0,BIT0);   //SW read trigger
            ForceDelay1ms(1);
            //if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                for (ucTmp = 0; ucTmp < 3; ucTmp++)
                {
                    u16CursorpointRGBValue[u8WinIdx][ucTmp]=msRead2Byte(u32BaseAddr + SC0F_5A +ucTmp*2);
                    drvMWE_PRINT("\r\n  cursor point R/G/B value ADD=[%x],Value=[%x]",(u32BaseAddr + SC0F_5A +ucTmp*2),u16CursorpointRGBValue[u8WinIdx][ucTmp]);
                }
            }
            /*
            else
            {
                for (ucTmp = 0; ucTmp < 3; ucTmp++)
                {
                    u16CursorpointRGBMaxMinValue[u8WinIdx][ucTmp]=msRead2Byte(u32BaseAddr + SC0F_5A +ucTmp*2);
                    drvMWE_PRINT(" \r\n  u16CursorpointRGBMaxMinValue[u8WinIdx][ucTmp]===%x",u16CursorpointRGBMaxMinValue[u8WinIdx][ucTmp]);
                }
            }
            */
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
        }
}


