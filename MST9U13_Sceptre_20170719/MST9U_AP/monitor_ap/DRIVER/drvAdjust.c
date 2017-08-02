
///////////////////////////////////////
// DRVADJUST_V VERSION: V02
////////////////////////////////////////
#include <string.h>
#include "Global.h"
#include "sysinfo2_MST9U.h"
#include "msPQ.h"

#define drvAdjust_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG && drvAdjust_DEBUG
#define drvAdjust_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define drvAdjust_PRINT(format, ...)
#endif


void msDrvSetBorderFormat(BYTE u8WinIndex, BYTE u8Left, BYTE u8Right, BYTE u8Up, BYTE u8Down, BYTE u8color)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIndex));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                //border size
                msWrite2ByteMask(u32BaseAddr+SC0F_04, ((WORD)u8Left<<8), 0xFF00);
                msWrite2ByteMask(u32BaseAddr+SC0F_06, ((WORD)u8Right<<8), 0xFF00);
                msWrite2ByteMask(u32BaseAddr+SC0F_08, ((WORD)u8Up<<8), 0xFF00);
                msWrite2ByteMask(u32BaseAddr+SC0F_0A, ((WORD)u8Down<<8), 0xFF00);

                msWrite2ByteMask(u32BaseAddr+SC10_48, ((WORD)u8color<<8), 0xFF00);
            }
            else
            {
                //border size
                msWrite2ByteMask(u32BaseAddr+SC0F_04, ((WORD)u8Left), 0x00FF);
                msWrite2ByteMask(u32BaseAddr+SC0F_06, ((WORD)u8Right), 0x00FF);
                msWrite2ByteMask(u32BaseAddr+SC0F_08, ((WORD)u8Up), 0x00FF);
                msWrite2ByteMask(u32BaseAddr+SC0F_0A, ((WORD)u8Down), 0x00FF);
/*
                //set sub win H/V start and end
                msWrite2Byte(u32BaseAddr+SC0F_0E, msRead2Byte(u32BaseAddr+SC10_10));    //DE
                msWrite2Byte(u32BaseAddr+SC0F_10, msRead2Byte(u32BaseAddr+SC10_12));
                msWrite2Byte(u32BaseAddr+SC0F_12, (msRead2Byte(u32BaseAddr+SC10_14)+1));
                msWrite2Byte(u32BaseAddr+SC0F_14, msRead2Byte(u32BaseAddr+SC10_16));
*/
                msWriteByte(u32BaseAddr+SC0F_2E, u8color);
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
	
}

void msDrvEnableBorder(BYTE u8WinIndex, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIndex));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWriteBit(u32BaseAddr+SC10_32, bEnable, BIT2);
            }
            else
            {
                msWriteBit(u32BaseAddr+SC0F_02, bEnable, BIT7); //sub win border enable
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}
#if (ENABLE_MWE)
static WORD g_u16MweScalerMaskOut;
void msDrvMWE_GetScalerMask(BYTE u8WinIndex)
{
    g_u16MweScalerMaskOut = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;
    g_DisplayWindow[u8WinIndex].u16DwScMaskOut = (g_u16MweScalerMaskOut >> 1);
}
void msDrvMWE_ResetScalerMask(BYTE u8WinIndex)
{
    g_DisplayWindow[u8WinIndex].u16DwScMaskOut = g_u16MweScalerMaskOut;
}
#endif
void msDrvAdjustHBR2HPosition(BYTE u8DispWin, WORD u16Position)
{
    WORD u16SCMask = g_DisplayWindow[u8DispWin].u16DwScMaskIn;
    BYTE temp = GetLsbBitPos(u16SCMask);
    ST_WINDOW_INFO u16AutoWinOri = g_DisplayWindow[u8DispWin].sDwAutoWin;
    ST_WINDOW_INFO u16CapWinOri = g_DisplayWindow[u8DispWin].sDwCaptureWin;
    WORD u16GuardBand, u16CapHStartL, u16CapHStartR, u16AutoWinOriMid, u16CapWinOriMid;
    BYTE u8ScalerIdxL=0, u8ScalerIdxR=0;

    u16Position = u16Position; // already in g_DisplayWindow[u8DispWin].sDwCaptureWin

    // get L/R scaler index
    if(g_ScalerInfo[temp].u8SiIPHBR2LR==0) // L
    {
        u8ScalerIdxL = temp;
        u16SCMask   -= (BIT0<<u8ScalerIdxL);
        u8ScalerIdxR = GetLsbBitPos(u16SCMask);
    }
    else
    {
        u8ScalerIdxR = temp;
        u16SCMask   -= (BIT0<<u8ScalerIdxR);
        u8ScalerIdxL = GetLsbBitPos(u16SCMask);
    }

    // Calculate GuardBand
    u16AutoWinOriMid = u16AutoWinOri.x+u16AutoWinOri.width/2;
    u16CapWinOriMid  = u16CapWinOri.x+u16CapWinOri.width/2;
#if 0 // for dynamicly adjust guard band case
    u16GuardBand = DIFF(u16AutoWinOriMid, u16CapWinOriMid);
    // Calculate New Capture H Start
    u16CapHStartL = u16CapWinOri.x;
    if(u16AutoWinOriMid>=u16CapWinOriMid) // same or move left
    {
        u16CapHStartR = u16AutoWinOri.x;
    }
    else // move right
    {
        u16CapHStartR = u16AutoWinOri.x + u16GuardBand*2;
    }

#else
    if( IsWindowMemBal(u8DispWin) )
        u16GuardBand = 0;
    else
        u16GuardBand = DFT_HBR2_GUARDBAND;
    // Calculate New Capture H Start
    u16CapHStartL = u16CapWinOri.x;
    u16CapHStartR = u16AutoWinOri.x + u16GuardBand + u16CapWinOriMid - u16AutoWinOriMid;
    #if Enable_Auto_Switch_DE_Mode
      if(FreeSyncFlag)
      {
        if( INPUT_IS_HDMI_2_0(MapWin2Port(u8DispWin)) )
            u16CapHStartR -= DFT_HBR2_GUARDBAND;
      }
    #else
      #if (CHIP_ID==MST9U3) && (!ENABLE_DEONLY_MODE)
        {
          if( INPUT_IS_HDMI_2_0(MapWin2Port(u8DispWin)) )
              u16CapHStartR -= DFT_HBR2_GUARDBAND;
        }
      #endif
    #endif
#endif

    msDrvIP1SetImageHStart(u16CapHStartL, u8ScalerIdxL);
    msDrvIP1SetImageHStart(u16CapHStartR, u8ScalerIdxR);
}

void msDrvAdjustSharpness(BYTE u8WinIndex, BYTE u8SharpLevel, BYTE u8Step, EN_WINDOW_COLOR ecolor)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIndex));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;


    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                if (u8SharpLevel==0x00) //No Change, Default
                {
                    if(ecolor == eWIN_COLOR_YUV)
                    {
                        msWriteByte(u32BaseAddr+SC18_A0, 0x03); //enable Y
                        msWriteByte(u32BaseAddr+SC18_A9, 0);
                    }
                    else
                    {
                        msWriteByte(u32BaseAddr+SC18_A0, 0x0B); //enable Y
                    }
                    msWriteByte(u32BaseAddr+SC18_A1, 0x03); //enable C
                    msWriteByte(u32BaseAddr+SC18_A2, 0x10); //Y gain tbl 1
                    msWriteByte(u32BaseAddr+SC18_A3, 0x32);
                    msWriteByte(u32BaseAddr+SC18_A4, 0x44);
                    msWriteByte(u32BaseAddr+SC18_A5, 0);
                }
                else
                {
                    if (u8Step) //Sharpest
                    {
                        if(ecolor == eWIN_COLOR_YUV)
                        {
                            msWriteByte(u32BaseAddr+SC18_A0, 0x83); //enable Y
                            msWriteByte(u32BaseAddr+SC18_A9, u8SharpLevel);
                        }
                        else
                        {
                            msWriteByte(u32BaseAddr+SC18_A0, 0x8B); //enable Y
                        }
                        msWriteByte(u32BaseAddr+SC18_A1, 0x83); //enable C
                        msWriteByte(u32BaseAddr+SC18_A2, 0x10); //Y gain tbl 1
                        msWriteByte(u32BaseAddr+SC18_A3, 0x32);
                        msWriteByte(u32BaseAddr+SC18_A4, 0x44);
                        msWriteByte(u32BaseAddr+SC18_A5, u8SharpLevel);
                    }
                    else //Blurriest
                    {
                        if(ecolor == eWIN_COLOR_YUV)
                        {
                            msWriteByte(u32BaseAddr+SC18_A0, 0x03); //enable Y
                            msWriteByte(u32BaseAddr+SC18_A9, u8SharpLevel);
                        }
                        else
                        {
                            msWriteByte(u32BaseAddr+SC18_A0, 0x0B); //enable Y
                        }
                        msWriteByte(u32BaseAddr+SC18_A0, 0x0B); //enable Y
                        msWriteByte(u32BaseAddr+SC18_A1, 0x03); //enable C
                        msWriteByte(u32BaseAddr+SC18_A2, 0xCF); //Y gain tbl 1
                        msWriteByte(u32BaseAddr+SC18_A3, 0x69);
                        msWriteByte(u32BaseAddr+SC18_A4, 0x24);
                        msWriteByte(u32BaseAddr+SC18_A5, u8SharpLevel);
                    }
                }
            }
            else
            {
                if (u8SharpLevel==0x00) //No Change, Default
                {
                    if(ecolor == eWIN_COLOR_YUV)
                    {
                        msWriteByte(u32BaseAddr+SC18_B0, 0x03); //enable Y
                        msWriteByte(u32BaseAddr+SC18_B9, 0);
                    }
                    else
                    {
                        msWriteByte(u32BaseAddr+SC18_B0, 0x0B); //enable Y
                    }
                    msWriteByte(u32BaseAddr+SC18_B1, 0x03); //enable C
                    msWriteByte(u32BaseAddr+SC18_B2, 0x10); //Y gain tbl 1
                    msWriteByte(u32BaseAddr+SC18_B3, 0x32);
                    msWriteByte(u32BaseAddr+SC18_B4, 0x44);
                    msWriteByte(u32BaseAddr+SC18_B5, 0);

                }
                else
                {
                    if (u8Step) //Sharpest
                    {
                        if(ecolor == eWIN_COLOR_YUV)
                        {
                            msWriteByte(u32BaseAddr+SC18_B0, 0x83); //enable Y
                            msWriteByte(u32BaseAddr+SC18_B9, u8SharpLevel);
                        }
                        else
                        {
                            msWriteByte(u32BaseAddr+SC18_B0, 0x8B); //enable Y
                        }
                        msWriteByte(u32BaseAddr+SC18_B1, 0x83); //enable C
                        msWriteByte(u32BaseAddr+SC18_B2, 0x10); //Y gain tbl 1
                        msWriteByte(u32BaseAddr+SC18_B3, 0x32);
                        msWriteByte(u32BaseAddr+SC18_B4, 0x44);
                        msWriteByte(u32BaseAddr+SC18_B5, u8SharpLevel);
                    }
                    else //Blurriest
                    {
                        if(ecolor == eWIN_COLOR_YUV)
                        {
                            msWriteByte(u32BaseAddr+SC18_B0, 0x03); //enable Y
                            msWriteByte(u32BaseAddr+SC18_B9, u8SharpLevel);
                        }
                        else
                        {
                            msWriteByte(u32BaseAddr+SC18_B0, 0x0B); //enable Y
                        }
                        msWriteByte(u32BaseAddr+SC18_B1, 0x03); //enable C
                        msWriteByte(u32BaseAddr+SC18_B2, 0xCF); //Y gain tbl 1
                        msWriteByte(u32BaseAddr+SC18_B3, 0x69);
                        msWriteByte(u32BaseAddr+SC18_B4, 0x24);
                        msWriteByte(u32BaseAddr+SC18_B5, u8SharpLevel);
                    }
                }
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

/******************************************************************************************************************************************
* Black Level
******************************************************************************************************************************************/
void msDrvBlackLevelEnable(BYTE u8WinIndex, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    DWORD  u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if(u8ScalerIdx % 2) //Main
            {
                msWriteByteMask(u32BaseAddr + SC10_2D, (bEnable ? BIT0 : 0), BIT0);
            }
            else                //Sub
            {
                msWriteByteMask(u32BaseAddr + SC0F_31, (bEnable ? BIT0 : 0), BIT0);
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvAdjustBlacklevel(BYTE u8WinIndex, BYTE u8Redblacklevel, BYTE u8Greenblacklevel, BYTE u8Blueblacklevel )
{
    BYTE u8ScalerIdx = 0;
    DWORD  u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;
    WORD u16Red_BLvl;
    WORD u16Green_BLvl;
    WORD u16Blue_BLvl;
    u16Red_BLvl = u8Redblacklevel<<3;
    u16Green_BLvl = u8Greenblacklevel<<3;
    u16Blue_BLvl = u8Blueblacklevel<<3;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
            u32BaseAddr =  u32BaseAddr + 0x00;
            else
            u32BaseAddr =  u32BaseAddr + 0x06;

        msWrite2Byte(u32BaseAddr+ SC0F_6C, u16Red_BLvl);
        msWrite2Byte(u32BaseAddr+ SC0F_6E, u16Green_BLvl);
        msWrite2Byte(u32BaseAddr+ SC0F_70, u16Blue_BLvl);
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvAdjustBlacklevel_10Bits(BYTE u8WinIndex, WORD u16Redblacklevel, WORD u16Greenblacklevel, WORD u16Blueblacklevel )
{
    BYTE u8ScalerIdx = 0;
    DWORD  u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;
    WORD u16Red_BLvl;
    WORD u16Green_BLvl;
    WORD u16Blue_BLvl;
    u16Red_BLvl = u16Redblacklevel<<1;
    u16Green_BLvl = u16Greenblacklevel<<1;
    u16Blue_BLvl = u16Blueblacklevel<<1;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
            u32BaseAddr =  u32BaseAddr + 0x00;
            else
            u32BaseAddr =  u32BaseAddr + 0x06;

        msWrite2Byte(u32BaseAddr+ SC0F_6C, u16Red_BLvl);
        msWrite2Byte(u32BaseAddr+ SC0F_6E, u16Green_BLvl);
        msWrite2Byte(u32BaseAddr+ SC0F_70, u16Blue_BLvl);
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

/******************************************************************************************************************************************
* Custom Hue R/G/B/C/M/Y
******************************************************************************************************************************************/
void msDrvCustomHueEnable(BYTE u8WinIdx, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
                msWriteBit(u32BaseAddr + SC14_02, bEnable,  BIT0);
            else
                msWriteBit(u32BaseAddr + SC14_03, bEnable,  BIT0);

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

}

void msDrvAdjustCustomHue(BYTE u8WinIdx, BYTE u8HueValue, BYTE u8RegOffset)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            msWriteByte(u32BaseAddr + SC14_0E + u8RegOffset, (u8HueValue & 0x7F));

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

#if (CHIP_ID == MST9U3)
/******************************************************************************************************************************************
* Custom Saturation R/G/B/C/M/Y for RGB
******************************************************************************************************************************************/
void msDrvCustomSatEnableRGBMode(BYTE u8WinIdx, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
                msWriteBit(u32BaseAddr + SC14_02, bEnable,  BIT4);
            else
                msWriteBit(u32BaseAddr + SC14_03, bEnable,  BIT4);

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

}

void msDrvAdjustCustomSatRGBMode(BYTE u8WinIdx, BYTE u8Value, BYTE u8RegOffset)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            msWriteByte(u32BaseAddr + SC14_14 + u8RegOffset, (u8Value & 0x7F));

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

/******************************************************************************************************************************************
* Custom Brightness R/G/B/C/M/Y for RGB
******************************************************************************************************************************************/
void msDrvCustomBriEnableRGBMode(BYTE u8WinIdx, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
                msWriteBit(u32BaseAddr + SC14_02, bEnable , BIT5);
            else
                msWriteBit(u32BaseAddr + SC14_03, bEnable , BIT5);

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

}

void msDrvAdjustCustomBriRGBMode(BYTE u8WinIdx, BYTE u8Value, BYTE u8RegOffset)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            msWriteByte(u32BaseAddr + SC14_1A + u8RegOffset, (u8Value & 0x7F));

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

#endif



/******************************************************************************************************************************************
* Write Gamma Table (1024 Entry - 14Bit)
******************************************************************************************************************************************/



//#define MIU_Gamma_DMA_READER_BUFFER_BASE_ADDR   0x8F0000

#if Enable_AutoDownloadGamma

void msDrvDeGammaLoadTbl_Compress_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx )
{
    BYTE u8ScalerIdx = 0;
    WORD u16Level, j; //Data 2

    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    BYTE EachLevel_Array[1024] = {};
    DWORD u32Gamma_write_ptr = AUTOLOAD_GMA_AVAILABLE;
    DWORD data = 0;
    //clear Mem data
    for( u16Level=0; u16Level<300; u16Level++)
        msMemWriteByte(u32Gamma_write_ptr + u16Level*16 + (112/8), 0x00);
    //===============

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            j = 0;

            for( u16Level=0; u16Level<300; u16Level++, j++)
            {
                if (u16Level < 128)
                {
                    data = (pu8GammaTblIdx[u8Color][j*3] << 16) + (pu8GammaTblIdx[u8Color][j*3+2] << 8) + pu8GammaTblIdx[u8Color][j*3+1];
                }
                if (u8Color==0) //Red
                {

                    //main R

                    EachLevel_Array[(110)/8]=1<<(110%8);
                    msMemWrite4Byte(u32Gamma_write_ptr+u16Level*16 + (72/8), data);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(110/8)  ,EachLevel_Array[(110)/8],1<<(110%8));

                }
                else if (u8Color==1) //Green
                {

                    EachLevel_Array[(109)/8]=1<<(109%8);
                    msMemWrite4Byte(u32Gamma_write_ptr+u16Level*16+(36)/8, data<<4);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(109/8)  ,EachLevel_Array[(109)/8],1<<(109%8));

                }
                else if (u8Color==2) //Blue
                {

                    EachLevel_Array[(108)/8]=1<<(108%8);
                    msMemWrite4Byte(u32Gamma_write_ptr+u16Level*16  ,data );
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(108/8)  ,EachLevel_Array[(108)/8],1<<(108%8));

                }
            }

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;

    }
}

void msDrvDeGammaWriteTbl_256E_12B_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx )
{
    BYTE u8ScalerIdx = 0;
    WORD u16Level, j; //Data 2

    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    BYTE EachLevel_Array[1024] = {};
    DWORD u32Gamma_write_ptr = AUTOLOAD_GMA_AVAILABLE;
    DWORD data = 0;
    //clear Mem data
    for( u16Level=0; u16Level<1024; u16Level++)
        msMemWriteByte(u32Gamma_write_ptr + u16Level*16 + (112/8), 0x00);
    //===============
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {

            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            j = 0;

            for( u16Level=0; u16Level<1024; u16Level++, j++)
            {
                if (u16Level < 256)
                {
                    if ((j % 3) == 0)   //0, 2, 4
                        data = pu8GammaTblIdx[u8Color][j] + (LOWNIBBLE(pu8GammaTblIdx[u8Color][j+2]) << 8);
                    else
                    {
                        data = pu8GammaTblIdx[u8Color][j] + (HIGHNIBBLE(pu8GammaTblIdx[u8Color][j+1]) << 8);
                        j++;
                    }
                }
                if (u8Color==0) //Red
                {

                    //main R

                    EachLevel_Array[(110)/8]=1<<(110%8);
                    msMemWrite4Byte(u32Gamma_write_ptr+u16Level*16 + (72/8), data);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(110/8)  ,EachLevel_Array[(110)/8],1<<(110%8));

                }
                else if (u8Color==1) //Green
                {

                    EachLevel_Array[(109)/8]=1<<(109%8);
                    msMemWrite4Byte(u32Gamma_write_ptr+u16Level*16+(36)/8, data<<4);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(109/8)  ,EachLevel_Array[(109)/8],1<<(109%8));

                }
                else if (u8Color==2) //Blue
                {

                    EachLevel_Array[(108)/8]=1<<(108%8);
                    msMemWrite4Byte(u32Gamma_write_ptr+u16Level*16  ,data );
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(108/8)  ,EachLevel_Array[(108)/8],1<<(108%8));

                }
            }

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}


void msDrvFixGammaWriteTbl_256E_12B_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx )
{
    BYTE u8ScalerIdx = 0;
    WORD u16Level,j; //Data 2

    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    BYTE EachLevel_Array[1024] = {};
    DWORD u32Gamma_write_ptr = AUTOLOAD_GMA_AVAILABLE;
    DWORD data;
    //clear Mem data
    for( u16Level=0; u16Level<1024; u16Level++)
        msMemWriteByte(u32Gamma_write_ptr + u16Level*16 + (108/8), 0x00);
    //===============
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            j = 0;
            for( u16Level=0; u16Level<256; u16Level++, j++)
            {
                if ((j % 3) == 0)   //0, 2, 4
                    data = pu8GammaTblIdx[u8Color][j] + (LOWNIBBLE(pu8GammaTblIdx[u8Color][j+2]) << 8);
                else
                {
                    data = pu8GammaTblIdx[u8Color][j] + (HIGHNIBBLE(pu8GammaTblIdx[u8Color][j+1]) << 8);
                    j++;
                }

                if (u8Color==0) //Red
                {

                    //main R

                    EachLevel_Array[(114)/8]=1<<(114%8);
                    msMemWrite2Byte(u32Gamma_write_ptr+u16Level*16 + (72/8), data);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(114/8)  ,EachLevel_Array[(114)/8],1<<(114%8));


                }
                else if (u8Color==1) //Green
                {

                    EachLevel_Array[(113)/8]=1<<(113%8);
                    msMemWrite2Byte(u32Gamma_write_ptr+u16Level*16+(36)/8, data<<4);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(113/8)  ,EachLevel_Array[(113)/8],1<<(113%8));

                }
                else if (u8Color==2) //Blue
                {

                    EachLevel_Array[(112)/8]=1<<(112%8);
                    msMemWrite2Byte(u32Gamma_write_ptr+u16Level*16  ,data );
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(112/8)  ,EachLevel_Array[(112)/8],1<<(112%8));

                }
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvGammaWriteTbl_1024E_14B_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx )
{
    BYTE u8ScalerIdx = 0;
    WORD u16Level; //Data 2
    WORD u16DataAddr;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    BYTE EachLevel_Array[1024] = {};
    DWORD u32Gamma_write_ptr = AUTOLOAD_GMA_AVAILABLE;
    BYTE dataH, dataL;
    WORD data;
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            for( u16Level=0; u16Level<1024; u16Level++)
            {
                u16DataAddr = u16Level;
                u16DataAddr = u16DataAddr <<1;

                dataH = *(*(pu8GammaTblIdx + u8Color) + u16DataAddr+1);
                dataL = *(*(pu8GammaTblIdx + u8Color) + u16DataAddr);
                data = dataH << 8 | dataL;
                #if (CHIP_ID == MST9U3)
                data >>= 2;
                dataH = data >> 8;
                dataL = data & (0xFF);
                #endif
            if (u8Color==0) //Red
            {

                if( (u8ScalerIdx % 2) == 1)// Main Win
                {
                    //main R
                    EachLevel_Array[85/8]=dataH>>(72%8)  ;
                    EachLevel_Array[(85-8)/8]=dataH |dataL>>(72%8) ;
                    EachLevel_Array[(72)/8]=dataL<<(72%8) ;
                    EachLevel_Array[(110)/8]=1<<(110%8);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(85/8)  ,EachLevel_Array[85/8] );
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(85-8)/8  ,EachLevel_Array[(85-8)/8]);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(72)/8  ,EachLevel_Array[(72)/8]);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(110/8)  ,EachLevel_Array[(110)/8],1<<(110%8));


                }
                else
                {
                    EachLevel_Array[101/8]=dataH>>(88%8)  ;
                    EachLevel_Array[(101-8)/8]=dataH<<(88%8) |dataL>>(88%8) ;
                    EachLevel_Array[(88)/8]=dataL<<(88%8) ;
                    EachLevel_Array[(114)/8]=1<<(114%8);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+101/8  ,EachLevel_Array[101/8]);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(101-8)/8  ,EachLevel_Array[(101-8)/8]);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(88)/8  ,EachLevel_Array[(88)/8]);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(114/8)  ,EachLevel_Array[(114)/8],1<<(114%8));
                }
            }
            else if (u8Color==1) //Green
            {
                if( (u8ScalerIdx % 2) == 1)// Main Win
                {
                    //main G

                    EachLevel_Array[49/8]=dataH>>(36%8)  ;
                    EachLevel_Array[(49-8)/8]=dataH<<(36%8) |dataL>>(36%8) ;
                    EachLevel_Array[(36)/8]=dataL<<(36%8) ;
                    EachLevel_Array[(109)/8]=1<<(109%8);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(49/8)  ,EachLevel_Array[49/8],0x0F);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(49-8)/8  ,EachLevel_Array[(49-8)/8]  );
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(36)/8  ,EachLevel_Array[(36)/8]  );
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(109/8)  ,EachLevel_Array[(109)/8],1<<(109%8));

                }
                else
                {
                    //sub G
                    EachLevel_Array[65/8]=dataH>>(52%8)  ;
                    EachLevel_Array[(65-8)/8]=dataH<<(52%8) |dataL>>(52%8) ;
                    EachLevel_Array[(52)/8]=EachLevel_Array[49/8]|dataL<<(52%8) ;
                    EachLevel_Array[(113)/8]=1<<(113%8);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+65/8  ,EachLevel_Array[65/8]);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(65-8)/8  ,EachLevel_Array[(65-8)/8]);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(52)/8  ,EachLevel_Array[(52)/8]  ,0xF0);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(113/8)  ,EachLevel_Array[(113)/8],1<<(113%8));
                }
            }
            else if (u8Color==2) //Blue
            {
                if( (u8ScalerIdx % 2) == 1)// Main Win
                {
                    //main B

                    EachLevel_Array[13/8]=dataH>>(0%8)  ;
                    EachLevel_Array[(13-8)/8]=dataH<<(0%8) |dataL>>(0%8) ;
                    EachLevel_Array[(0)/8]=dataL<<(0%8) ;
                    EachLevel_Array[(108)/8]=1<<(108%8);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(13/8)  ,EachLevel_Array[13/8] );
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(13-8)/8  ,EachLevel_Array[(13-8)/8]);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(0)/8  ,EachLevel_Array[(0)/8]);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(108/8)  ,EachLevel_Array[(108)/8],1<<(108%8));

                }
                else
                {
                    //sub B
                    EachLevel_Array[29/8]=dataH>>(16%8)  ;
                    EachLevel_Array[(29-8)/8]=dataH<<(16%8) |dataL>>(16%8) ;
                    EachLevel_Array[(16)/8]=dataL<<(16%8) ;
                    EachLevel_Array[(112)/8]=1<<(112%8);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+29/8  ,EachLevel_Array[29/8]);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(29-8)/8  ,EachLevel_Array[(29-8)/8]);
                    msMemWriteByte(u32Gamma_write_ptr+u16Level*16+(16)/8  ,EachLevel_Array[(16)/8]);
                    msMemWriteByteMask(u32Gamma_write_ptr+u16Level*16+(112/8)  ,EachLevel_Array[(112)/8],1<<(112%8));
                }
            }
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvGammaWriteTbl_256E_10B_ByDMA(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx )
{
    BYTE   u8LowValue=0,
         u8ScalerIdx=0,
         u8Temp, u8Temp2,
      dataH, dataL;

    WORD u16Level,
         u16Entry=0,
         u16Data=0,
         u16LatestData=0,
         u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut,
         u16TempGamma=0;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    DWORD u32Gamma_write_ptr = AUTOLOAD_GMA_AVAILABLE;
    BYTE EachLevel_Array[1024] = {};

    const int nSizeofGammaTable = 257;
    DWORD dwDecompressGamma[nSizeofGammaTable];
    memset(dwDecompressGamma, 0, nSizeofGammaTable*sizeof(DWORD));

    for( u16Level=0; u16Level<310; u16Level+=5)
    {
        // Write gamma data
        u8LowValue= *(*(pu8GammaTblIdx + u8Color) + u16Level);
        for(u8Temp=0; u8Temp<4; u8Temp++)
        {
            u16Data = *(*(pu8GammaTblIdx + u8Color) + u16Level+1+u8Temp);

            if (u16Level==0 && u8Temp==0)
            {
                dwDecompressGamma[u16Level/5*4+u8Temp] = ((u16Data << 2) + ((u8LowValue >> (u8Temp<<1)) & 0x03)) << 4;
            }
            else
            {
                dwDecompressGamma[u16Level/5*4+u8Temp] = (((dwDecompressGamma[u16Level/5*4+u8Temp-1]>>6) + (u16Data>>4)) << 6) + ((u16Data & 0x0F)<<2) +((u8LowValue >> (u8Temp<<1)) & 0x03);
            }
        }
    }

 // Write gamma data
    u16Level = 310;
    u8LowValue= *(*(pu8GammaTblIdx + u8Color) + u16Level);
    for(u8Temp=0; u8Temp<8; u8Temp++)
    {
        u16Data = *(*(pu8GammaTblIdx + u8Color) + u16Level+1+u8Temp);
        dwDecompressGamma[u16Level/5*4+u8Temp] = (((dwDecompressGamma[u16Level/5*4+u8Temp-1]>>6) + (u16Data>>4)) << 6) + ((u16Data & 0x0F)<<2) + (((u8LowValue >> (u8Temp)) & 0x01)<<1);
    }
    u16Data = *(*(pu8GammaTblIdx + u8Color) + u16Level+1+u8Temp);
    dwDecompressGamma[u16Level/5*4+u8Temp] = (((dwDecompressGamma[u16Level/5*4+u8Temp-1]>>6) + (u16Data>>4)) << 6) + ((u16Data & 0x0F)<<2);

    while(u16ScalerMask)
    {


        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            // From level 0 ~ 1023
            // set address first
            // Cal data
                for( u16Level=0; u16Level<257; u16Level++)
        {
        // Write gamma data
            if (u16Level > 0 && u16Level < 256)
                    u16TempGamma = (dwDecompressGamma[u16Level] - dwDecompressGamma[u16Level-1]) / 4;
            else if (u16Level == 256)
                    u16TempGamma = (dwDecompressGamma[u16Level] - dwDecompressGamma[u16Level-1]) / 3;
            for(u8Temp2=0; u8Temp2<4; u8Temp2++)
            {
                if (u16Level > 0)
                    {
                        if(u8Temp2==3)
                        {
                                if (u16Level == 256)
                                    continue;
                                u16LatestData = dwDecompressGamma[u16Level];
                        }
                        else
                        {
                                u16LatestData += u16TempGamma;
                        }
                    }
                    else
                    {
                        u16LatestData = dwDecompressGamma[u16Level];
                    }
                #if (CHIP_ID == MST9U3)
                dataH = (u16LatestData >> 2) >> 8;
                dataL = (u16LatestData >> 2)& (0xFF);
                #else
                dataH = u16LatestData >> 8;
                dataL = u16LatestData & (0xFF);
                    #endif
                if( u8Color == 0 ) // Red
                {
                    if( (u8ScalerIdx % 2) == 1)// Main Win
                    {
                        //main R
                        EachLevel_Array[85/8]=dataH>>(72%8)  ;
                        EachLevel_Array[(85-8)/8]=dataH |dataL>>(72%8) ;
                        EachLevel_Array[(72)/8]=dataL<<(72%8) ;
                        EachLevel_Array[(110)/8]=1<<(110%8);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(85/8)  ,EachLevel_Array[85/8] );
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(85-8)/8  ,EachLevel_Array[(85-8)/8]);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(72)/8  ,EachLevel_Array[(72)/8]);
                        msMemWriteByteMask(u32Gamma_write_ptr+u16Entry*16+(110/8)  ,EachLevel_Array[(110)/8],1<<(110%8));
                    }
                    else
                    {
                        EachLevel_Array[101/8]=dataH>>(88%8)  ;
                        EachLevel_Array[(101-8)/8]=dataH<<(88%8) |dataL>>(88%8) ;
                        EachLevel_Array[(88)/8]=dataL<<(88%8) ;
                        EachLevel_Array[(114)/8]=1<<(114%8);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+101/8  ,EachLevel_Array[101/8]);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(101-8)/8  ,EachLevel_Array[(101-8)/8]);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(88)/8  ,EachLevel_Array[(88)/8]);
                        msMemWriteByteMask(u32Gamma_write_ptr+u16Entry*16+(114/8)  ,EachLevel_Array[(114)/8],1<<(114%8));
                    }
                }
                else if( u8Color == 1 ) // Green
                {
                    if( (u8ScalerIdx % 2) == 1)// Main Win
                    {
                        //main G
                        EachLevel_Array[49/8]=dataH>>(36%8)  ;
                        EachLevel_Array[(49-8)/8]=dataH<<(36%8) |dataL>>(36%8) ;
                        EachLevel_Array[(36)/8]=dataL<<(36%8) ;
                        EachLevel_Array[(109)/8]=1<<(109%8);
                        msMemWriteByteMask(u32Gamma_write_ptr+u16Entry*16+(49/8)  ,EachLevel_Array[49/8],0x0F);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(49-8)/8  ,EachLevel_Array[(49-8)/8]  );
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(36)/8  ,EachLevel_Array[(36)/8]  );
                        msMemWriteByteMask(u32Gamma_write_ptr+u16Entry*16+(109/8)  ,EachLevel_Array[(109)/8],1<<(109%8));
                    }
                    else
                    {
                        //sub G
                        EachLevel_Array[65/8]=dataH>>(52%8)  ;
                        EachLevel_Array[(65-8)/8]=dataH<<(52%8) |dataL>>(52%8) ;
                        EachLevel_Array[(52)/8]=EachLevel_Array[49/8]|dataL<<(52%8) ;
                        EachLevel_Array[(113)/8]=1<<(113%8);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+65/8  ,EachLevel_Array[65/8]);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(65-8)/8  ,EachLevel_Array[(65-8)/8]);
                        msMemWriteByteMask(u32Gamma_write_ptr+u16Entry*16+(52)/8  ,EachLevel_Array[(52)/8]  ,0xF0);
                        msMemWriteByteMask(u32Gamma_write_ptr+u16Entry*16+(113/8)  ,EachLevel_Array[(113)/8],1<<(113%8));
                    }
                }
                else //if( ucColor == 2 ) // Blue
                {
                    if( (u8ScalerIdx % 2) == 1)// Main Win
                    {
                        //main B
                        EachLevel_Array[13/8]=dataH>>(0%8)  ;
                        EachLevel_Array[(13-8)/8]=dataH<<(0%8) |dataL>>(0%8) ;
                        EachLevel_Array[(0)/8]=dataL<<(0%8) ;
                        EachLevel_Array[(108)/8]=1<<(108%8);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(13/8)  ,EachLevel_Array[13/8] );
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(13-8)/8  ,EachLevel_Array[(13-8)/8]);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(0)/8  ,EachLevel_Array[(0)/8]);
                        msMemWriteByteMask(u32Gamma_write_ptr+u16Entry*16+(108/8)  ,EachLevel_Array[(108)/8],1<<(108%8));
                    }
                    else
                    {
                        //sub B
                        EachLevel_Array[29/8]=dataH>>(16%8)  ;
                        EachLevel_Array[(29-8)/8]=dataH<<(16%8) |dataL>>(16%8) ;
                        EachLevel_Array[(16)/8]=dataL<<(16%8) ;
                        EachLevel_Array[(112)/8]=1<<(112%8);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+29/8  ,EachLevel_Array[29/8]);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(29-8)/8  ,EachLevel_Array[(29-8)/8]);
                        msMemWriteByte(u32Gamma_write_ptr+u16Entry*16+(16)/8  ,EachLevel_Array[(16)/8]);
                        msMemWriteByteMask(u32Gamma_write_ptr+u16Entry*16+(112/8)  ,EachLevel_Array[(112)/8],1<<(112%8));
                    }
                }

                u16Entry++;
                if (u16Level == 0)
                    {
                        break;
                    }
            }
        }
        }


        u16LatestData = 0;
        u16Entry = 0;//Reset entry
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}
#else
void msDrvGammaWriteTbl_1024E_14B(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx )
{
    BYTE u8ScalerIdx = 0;
    WORD u16Level; //Data 2
    WORD u16DataAddr;
    WORD u16LatestData;
    BYTE u8TriggerBit;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    BYTE u8ErrorDetCnt=0;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            // From level 0 ~ 1023
            // set address first
            msWrite2Byte(u32BaseAddr + SC10_D8, 0x0000);

            //Main Window
            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWriteByteMask(u32BaseAddr + SC10_DA, _BIT0, _BIT0|_BIT1);//Main Win Burst write enable
            }
            else
            {
                msWriteByteMask(u32BaseAddr + SC10_DA, _BIT1, _BIT0|_BIT1);//Sub Win Burst write enable
            }

            if( u8Color == 0 ) // Red
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x00, 0x0C);

            }
            else if( u8Color == 1 ) // Green
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x04, 0x0C);
            }
            else //if( ucColor == 2 ) // Blue
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x08, 0x0C);
            }

            // Cal data
            for( u16Level=0; u16Level<1024; u16Level++)
            {
                u16DataAddr = u16Level;
                u16DataAddr = u16DataAddr <<1;

                // Write gamma data
                u16LatestData = *(*(pu8GammaTblIdx + u8Color) + u16DataAddr) + ((*(*(pu8GammaTblIdx + u8Color) + u16DataAddr + 1))<< 8);
#if (CHIP_ID==MST9U3)
                msWrite2Byte(u32BaseAddr+SC10_DC, u16LatestData>>2);
#else
                msWrite2Byte(u32BaseAddr+SC10_DC, u16LatestData);
#endif
                // Trigger write pulse
                msWriteByte(u32BaseAddr+SC10_DB, u8TriggerBit);

                // Check write finish
                while(msReadByte(u32BaseAddr+SC10_DB) & u8TriggerBit)
                {
                    Delay1ms(1);
                    u8ErrorDetCnt++;
                    if(u8ErrorDetCnt==100)
                    {
                        //Check Fail
                        break;
                    }
                }

            }

            if(u8ScalerIdx % 2) //Main
            {
                msWriteByteMask(u32BaseAddr+SC10_DA, 0, _BIT0|_BIT1);//Main Win Burst write disable
            }
            else                //Sub
            {
                msWriteByteMask(u32BaseAddr+SC10_DA, 0, _BIT1);//Sub Win Burst write disable
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }


}

/******************************************************************************************************************************************
* Write Gamma Table (256 Entry - 10 Bit)
* Gamma Table include 320 Bytes (256Byte + 256/4 Bytes)
******************************************************************************************************************************************/
void msDrvGammaWriteTbl_256E_10B(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx )
{
    BYTE u8ErrorDetCnt=0,
         u8TriggerBit=0,
         u8LowValue=0,
         u8ScalerIdx=0,
         u8Temp, u8Temp2;
    WORD u16Level,
         u16Data=0,
         u16LatestData=0,
         u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut,
         u16TempGamma=0;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    const int nSizeofGammaTable = 257;
    DWORD dwDecompressGamma[nSizeofGammaTable];
    memset(dwDecompressGamma, 0, nSizeofGammaTable*sizeof(DWORD));

    for( u16Level=0; u16Level<310; u16Level+=5)
    {
        // Write gamma data
        u8LowValue= *(*(pu8GammaTblIdx + u8Color) + u16Level);
        for(u8Temp=0; u8Temp<4; u8Temp++)
        {
            u16Data = *(*(pu8GammaTblIdx + u8Color) + u16Level+1+u8Temp);

            if (u16Level==0 && u8Temp==0)
            {
                dwDecompressGamma[u16Level/5*4+u8Temp] = ((u16Data << 2) + ((u8LowValue >> (u8Temp<<1)) & 0x03)) << 4;
            }
            else
            {
                dwDecompressGamma[u16Level/5*4+u8Temp] = (((dwDecompressGamma[u16Level/5*4+u8Temp-1]>>6) + (u16Data>>4)) << 6) + ((u16Data & 0x0F)<<2) +((u8LowValue >> (u8Temp<<1)) & 0x03);
            }
        }
    }
    // Write gamma data
    u16Level = 310;
    u8LowValue= *(*(pu8GammaTblIdx + u8Color) + u16Level);
    for(u8Temp=0; u8Temp<8; u8Temp++)
    {
        u16Data = *(*(pu8GammaTblIdx + u8Color) + u16Level+1+u8Temp);
        dwDecompressGamma[u16Level/5*4+u8Temp] = (((dwDecompressGamma[u16Level/5*4+u8Temp-1]>>6) + (u16Data>>4)) << 6) + ((u16Data & 0x0F)<<2) + (((u8LowValue >> (u8Temp)) & 0x01)<<1);
    }
    u16Data = *(*(pu8GammaTblIdx + u8Color) + u16Level+1+u8Temp);
    dwDecompressGamma[u16Level/5*4+u8Temp] = (((dwDecompressGamma[u16Level/5*4+u8Temp-1]>>6) + (u16Data>>4)) << 6) + ((u16Data & 0x0F)<<2);


    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            // From level 0 ~ 1023
            // set address first
            msWrite2Byte(u32BaseAddr + SC10_D8, 0x0000);

            if(u8ScalerIdx % 2) //Main Window
            {
                msWriteByteMask(u32BaseAddr + SC10_DA, _BIT0, _BIT0);//Main Win Burst write enable
            }
            else                //Sub Window
            {
                msWriteByteMask(u32BaseAddr + SC10_DA, _BIT1, _BIT1);//Sub Win Burst write enable
            }

            if( u8Color == 0 ) // Red
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x00, 0x0C);

            }
            else if( u8Color == 1 ) // Green
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x04, 0x0C);
            }
            else //if( ucColor == 2 ) // Blue
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x08, 0x0C);
            }

            // Cal data
            for( u16Level=0; u16Level<257; u16Level++)
            {
                if (u16Level > 0 && u16Level < 256)
                    u16TempGamma = (dwDecompressGamma[u16Level] - dwDecompressGamma[u16Level-1]) / 4;
                else if (u16Level == 256)
                    u16TempGamma = (dwDecompressGamma[u16Level] - dwDecompressGamma[u16Level-1]) / 3;
                for(u8Temp2=0; u8Temp2<4; u8Temp2++)
                {
                    if (u16Level > 0)
                    {
                        if(u8Temp2==3)
                        {
                            if (u16Level == 256)
                                continue;
                            u16LatestData = dwDecompressGamma[u16Level];
                        }
                        else
                        {
                            u16LatestData += u16TempGamma;
                        }
                    }
                    else
                    {
                        u16LatestData = dwDecompressGamma[u16Level];
                    }
#if (CHIP_ID==MST9U3)
                    msWrite2Byte(u32BaseAddr+SC10_DC, u16LatestData>>2);
#else
                    msWrite2Byte(u32BaseAddr+SC10_DC, u16LatestData);
#endif

                    // Trigger write pulse
                    msWriteByte(u32BaseAddr+SC10_DB, u8TriggerBit);

                    // Check write finish
                    u8ErrorDetCnt = 0;
                    while(msReadByte(u32BaseAddr+SC10_DB) & u8TriggerBit)
                    {
                        Delay1ms(1);
                        u8ErrorDetCnt++;
                        if(u8ErrorDetCnt==100)
                        {
                            //Check Fail
                            break;
                        }
                    }
                    if (u16Level == 0)
                    {
                        break;
                    }
                }

            }

            if(u8ScalerIdx % 2) //Main
            {
                msWriteByteMask(u32BaseAddr+SC10_DA, 0, _BIT0);//Main Win Burst write disable
            }
            else                //Sub
            {
                msWriteByteMask(u32BaseAddr+SC10_DA, 0, _BIT1);//Sub Win Burst write disable
            }
        }

        u16LatestData = 0;//Reset Data
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}
#endif
/******************************************************************************************************************************************
* Write Gamma Table (256 Entry - 8 Bit)
******************************************************************************************************************************************/
void msDrvGammaWriteTbl_256E_8B(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8GammaTblIdx )
{
    BYTE u8ErrorDetCnt=0,
         u8TriggerBit=0,
         u8ScalerIdx=0,
         u8Temp;
    WORD u16Level,
         u16Data=0,
         u16LatestData=0,
         u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut,
         u16TempGamma=0;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            // From level 0 ~ 1023
            // set address first
            msWrite2Byte(u32BaseAddr + SC10_D8, 0x0000);

            if(u8ScalerIdx % 2)//Main Window
            {
                msWriteByteMask(u32BaseAddr + SC10_DA, _BIT0, _BIT0);//Main Win Burst write enable
            }
            else                //Sub Window
            {
                msWriteByteMask(u32BaseAddr + SC10_DA, _BIT1, _BIT1);//Sub Win Burst write enable
            }

            if( u8Color == 0 ) // Red
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x00, 0x0C);

            }
            else if( u8Color == 1 ) // Green
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x04, 0x0C);
            }
            else //if( ucColor == 2 ) // Blue
            {
                u8TriggerBit = _BIT0;
                msWriteByteMask(u32BaseAddr+SC10_DA, 0x08, 0x0C);
            }

            // Cal data
            for( u16Level=0; u16Level<256; u16Level++)//256
            {
                // Write gamma data
                u16Data = *(*(pu8GammaTblIdx + u8Color) + u16Level);
                u16Data <<= 6;

                u16TempGamma = (u16Data - u16LatestData) / 4;
                for(u8Temp=0; u8Temp<4; u8Temp++)
                {
                    if(u8Temp==3)
                    {
                        u16LatestData = u16Data;
                    }
                    else
                    {
                        u16LatestData += u16TempGamma;
                    }
#if (CHIP_ID==MST9U3)
                    msWrite2Byte(u32BaseAddr+SC10_DC, u16LatestData>>2);
#else
                    msWrite2Byte(u32BaseAddr+SC10_DC, u16LatestData);
#endif

                    // Trigger write pulse
                    msWriteByte(u32BaseAddr+SC10_DB, u8TriggerBit);

                    // Check write finish
                    while(msReadByte(u32BaseAddr+SC10_DB) & u8TriggerBit)
                    {
                        Delay1ms(1);
                        u8ErrorDetCnt++;
                        if(u8ErrorDetCnt==100)
                        {
                            //Check Fail
                            break;
                        }
                    }
                }
            }
            if(u8ScalerIdx % 2) //Main
            {
                msWriteByteMask(u32BaseAddr+SC10_DA, 0, _BIT0);//Main Win Burst write disable
            }
            else                //Sub
            {
                msWriteByteMask(u32BaseAddr+SC10_DA, 0, _BIT1);//Sub Win Burst write disable
            }
        }

        u16LatestData = 0;//Reset Data
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}
/******************************************************************************************************************************************
* Gamma Load Table
******************************************************************************************************************************************/
void msDrvGammaLoadTbl_256E_8B(BYTE u8WinIdx, BYTE **pu8GammaTblIdx)
{
    BYTE u8ScalerIdx = 0;
    BYTE u8TgtChannel;
    BYTE u8BackupMainSubEnableFlag = 0;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if(u8ScalerIdx % 2) //Main
            {
                // If gamma on, turn off it
                if( msReadByte(u32BaseAddr+SC10_A0) & _BIT0 ) // Check main
                {
                    msWriteBit(u32BaseAddr+SC10_A0, _DISABLE, _BIT0);
                    u8BackupMainSubEnableFlag |= _BIT0;
                }
            }
            else                //Sub
            {
                if( msReadByte(u32BaseAddr+SCF_30) & _BIT0 ) // Check sub
                {
                    msWriteBit(u32BaseAddr+SCF_30, _DISABLE, _BIT0);
                    u8BackupMainSubEnableFlag |= _BIT1;
                }
            }

            for(u8TgtChannel=0; u8TgtChannel<3; u8TgtChannel++ )
            {
                // Write data to gamma channel
                msDrvGammaWriteTbl_256E_8B(u8WinIdx, u8TgtChannel, pu8GammaTblIdx);
            }
        }

        u8ScalerIdx++;
        u16ScalerMask >>= 1;

    }
}

void msDrvGammaLoadTbl_256E_10B(BYTE u8WinIdx, BYTE **pu8GammaTblIdx)
{
    BYTE u8ScalerIdx = 0;
    BYTE u8TgtChannel;
    BYTE u8BackupMainSubEnableFlag = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {

            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if(u8ScalerIdx % 2) //Main
            {
                // If gamma on, turn off it
                if( msReadByte(u32BaseAddr+SC10_A0) & _BIT0 ) // Check main
                {
                    msWriteBit(u32BaseAddr+SC10_A0, _DISABLE, _BIT0);
                    u8BackupMainSubEnableFlag |= _BIT0;
                }
            }
            else                //Sub
            {
                if( msReadByte(u32BaseAddr+SCF_30) & _BIT0 ) // Check sub
                {
                    msWriteBit(u32BaseAddr+SCF_30, _DISABLE, _BIT0);
                    u8BackupMainSubEnableFlag |= _BIT1;
                }
            }

        }

         u8ScalerIdx++;
         u16ScalerMask >>= 1;
     }

     for(u8TgtChannel=0; u8TgtChannel<3; u8TgtChannel++ )
     {
              // Write data to gamma channel
              #if Enable_AutoDownloadGamma
              msDrvGammaWriteTbl_256E_10B_ByDMA(u8WinIdx, u8TgtChannel, pu8GammaTblIdx);
              #else
              msDrvGammaWriteTbl_256E_10B(u8WinIdx, u8TgtChannel, pu8GammaTblIdx);
           #endif
      }

#if Enable_AutoDownloadGamma
    msDrvGamma_DMA_Reader_Trigger(u8WinIdx);
#endif
}

void msDrvGammaLoadTbl_1024E_14B(BYTE u8WinIdx, BYTE **pu8GammaTblIdx)
{
    BYTE u8ScalerIdx = 0;
    BYTE u8TgtChannel;
    BYTE u8BackupMainSubEnableFlag = 0;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {

            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            // If gamma on, turn off it
            if(u8ScalerIdx % 2)     //Main
            {
                if( msReadByte(u32BaseAddr+SC10_A0) & _BIT0 ) // Check main
                {
                    msWriteBit(u32BaseAddr+SC10_A0, _DISABLE, _BIT0);
                    u8BackupMainSubEnableFlag |= _BIT0;
                }
            }
            else                    //Sub
            {
                if( msReadByte(u32BaseAddr+SC0F_30) & _BIT0 ) // Check sub
                {
                    msWriteBit(u32BaseAddr+SC0F_30, _DISABLE, _BIT0);
                    u8BackupMainSubEnableFlag |= _BIT1;
                }
            }

            for(u8TgtChannel=0; u8TgtChannel<3; u8TgtChannel++ )
            {
                // Write data to gamma channel
        #if Enable_AutoDownloadGamma
                msDrvGammaWriteTbl_1024E_14B_ByDMA(u8WinIdx, u8TgtChannel, pu8GammaTblIdx);
        #else
                msDrvGammaWriteTbl_1024E_14B(u8WinIdx, u8TgtChannel, pu8GammaTblIdx);
        #endif
            }
        }

        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

#if Enable_AutoDownloadGamma
    msDrvGamma_DMA_Reader_Trigger(u8WinIdx);
#endif
}
/******************************************************************************************************************************************
* Set Gamma LUT Enable/Disable
******************************************************************************************************************************************/
void msDrvGammaEnable(BYTE u8WinIdx, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if(bEnable)
            {
                if(u8ScalerIdx %2)  //Main
                {
                    msWriteBit(u32BaseAddr+SC10_A0, TRUE, _BIT0);
                }
                else                //Sub
                {
                    msWriteBit(u32BaseAddr+SC0F_30, TRUE, _BIT0);
                }
                msWriteBit(u32BaseAddr+SC0F_AF, TRUE, _BIT4); // repeat gamma table max value for interpolation
            }
            else
            {
                if(u8ScalerIdx %2)  //Main
                {
                    msWriteBit(u32BaseAddr+SC10_A0, FALSE, _BIT0);
                }
                else                //Sub
                {
                    msWriteBit(u32BaseAddr+SC0F_30, FALSE, _BIT0);
                }
                msWriteBit(u32BaseAddr+SC0F_AF, FALSE, _BIT4); // repeat gamma table max value for interpolation
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

/*de-Gamma*/
void msDrvDeGammaLoadTbl_Compress(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8DeGammaTblIdx, WORD** pu16SegPntTbl)
{

    WORD u16Count, i;
    DWORD u32BaseAddr;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    BYTE u8TriggerBits;
    u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));

    switch (u8Color){
        case 0: {u8TriggerBits = 0;}break;
        case 1: {u8TriggerBits = BIT5;}break;
        case 2: {u8TriggerBits = BIT6;}break;
        default: {u8TriggerBits = 0;}break;
    }

    msDrvSCCommonWrite(u16ScalerMask, TRUE);
    u16Count = 300;
    // address port initial as 0
    msWrite2Byte(u32BaseAddr + SC2D_C2, 0x00);
    // pre set value
    msWrite2Byte(u32BaseAddr + SC2D_C4, 0x00);

    // [3] = write, [7] = burst write
    msWrite2ByteMask(u32BaseAddr + SC2D_C0, BIT3 |BIT7 | u8TriggerBits, BIT3 | BIT7 | u8TriggerBits);

    for(i=0; i<u16Count; i++)
    {
        if (i < 128)
        {
            msWriteByte(u32BaseAddr + SC2D_C6, pu8DeGammaTblIdx[u8Color][i * 3]); //data
            msWriteByte(u32BaseAddr + SC2D_C7, 0); //data
            msWriteByte(u32BaseAddr + SC2D_C4, pu8DeGammaTblIdx[u8Color][i * 3 + 1]); //data
            msWriteByte(u32BaseAddr + SC2D_C5, pu8DeGammaTblIdx[u8Color][i * 3 + 2]); //data

        }
        else
        {
            msWriteByte(u32BaseAddr + SC2D_C6, pu8DeGammaTblIdx[u8Color][127 * 3]); //data
            msWriteByte(u32BaseAddr + SC2D_C7, 0); //data
            msWriteByte(u32BaseAddr + SC2D_C4, pu8DeGammaTblIdx[u8Color][127 * 3 + 1]); //data
            msWriteByte(u32BaseAddr + SC2D_C5, pu8DeGammaTblIdx[u8Color][127 * 3 + 2]); //data

        }
    }
    //close write & burst write cmd
    msWrite2Byte(u32BaseAddr + SC2D_C0, 0);

    //write offset setting
    msWriteByte(u32BaseAddr + SC2D_DE + (u8Color * 2) , pu8DeGammaTblIdx[3][u8Color]);

    //write segment point setting (8bits * 3(point) = 3bytes per channel)
    for (i = 0; i < 3; i++)
        msWrite2Byte(u32BaseAddr + SC2D_CC + (i * 2) + (u8Color * 6), pu16SegPntTbl[0][i + u8Color * 3]);


    msDrvSCCommonWrite(u16ScalerMask, FALSE);

}

void msDrvDeGammaLoadTbl_Compress_Offset(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8DeGammaTblIdx, WORD** pu16SegPntTbl)
{

    WORD i;
    DWORD u32BaseAddr;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    msDrvSCCommonWrite(u16ScalerMask, TRUE);
    //write offset setting
    msWriteByte(u32BaseAddr + SC2D_DE + (u8Color * 2) , pu8DeGammaTblIdx[3][u8Color]);
    //write segment point setting (8bits * 3(point) = 3bytes per channel)
    for (i = 0; i < 3; i++)
        msWrite2Byte(u32BaseAddr + SC2D_CC + (i * 2) + (u8Color * 6), pu16SegPntTbl[0][i + u8Color * 3]);

    msDrvSCCommonWrite(u16ScalerMask, FALSE);

}

void msDrvDeGammaLoadTbl(BYTE u8WinIdx, BYTE u8Color, BYTE **pu8DeGammaTblIdx)
{
    DWORD u32BaseAddr;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    WORD u16Data, i, j;
    BYTE u8TriggerBits;
    u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));

    switch (u8Color){
        case 0: {u8TriggerBits = 0;}break;
        case 1: {u8TriggerBits = BIT5;}break;
        case 2: {u8TriggerBits = BIT6;}break;
        default: {u8TriggerBits = 0;}break;
    }

    msDrvSCCommonWrite(u16ScalerMask, TRUE);
    // address port initial as 0
    msWrite2Byte(u32BaseAddr + SC2D_C2, 0x00);
    // pre set value
    msWrite2Byte(u32BaseAddr + SC2D_C4, 0x00);

    // [3] = write, [7] = burst write
    msWrite2ByteMask(u32BaseAddr + SC2D_C0, BIT3 |BIT7 | u8TriggerBits, BIT3 | BIT7 | u8TriggerBits);
    j = 0;
    for(i = 0; i < 256; i++, j++)
    {
        if ((j % 3) == 0)   //0, 2, 4
            u16Data = pu8DeGammaTblIdx[u8Color][j] + (LOWNIBBLE(pu8DeGammaTblIdx[u8Color][j+2]) << 8);
        else
        {
            u16Data = pu8DeGammaTblIdx[u8Color][j] + (HIGHNIBBLE(pu8DeGammaTblIdx[u8Color][j+1]) << 8);
            j++;
        }
        msWrite2Byte(u32BaseAddr + SC2D_C4, u16Data);
    }
    for(i = 256; i < 1024; i++)
    {
        msWrite2Byte(u32BaseAddr + SC2D_C4, u16Data);
    }
    //close write & burst write cmd
    msWrite2Byte(u32BaseAddr + SC2D_C0, 0);

    msDrvSCCommonWrite(u16ScalerMask, FALSE);

}

void msDrvDeGammaLoadTbl_256E_12B(BYTE u8WinIdx, BYTE u8Color,WORD **pu8DeGammaTbl)  //sam 141203
{
    DWORD u32BaseAddr;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    WORD u16Data, i, j;
    BYTE u8TriggerBits;
    u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));

    switch (u8Color){
        case 0: {u8TriggerBits = 0;}break;
        case 1: {u8TriggerBits = BIT5;}break;
        case 2: {u8TriggerBits = BIT6;}break;
        default: {u8TriggerBits = 0;}break;
    }

    msDrvSCCommonWrite(u16ScalerMask, TRUE);
    // address port initial as 0
    msWrite2Byte(u32BaseAddr + SC2D_C2, 0x00);
    // pre set value
    msWrite2Byte(u32BaseAddr + SC2D_C4, 0x00);

    // [3] = write, [7] = burst write
    msWrite2ByteMask(u32BaseAddr + SC2D_C0, BIT3 |BIT7 | u8TriggerBits, BIT3 | BIT7 | u8TriggerBits);
    j = 0;
    for(i = 0; i < 256; i++, j++)
    {
        u16Data = pu8DeGammaTbl[u8Color][i];
        msWrite2Byte(u32BaseAddr + SC2D_C4, u16Data);
    }
    for(i = 256; i < 1024; i++)
    {
        msWrite2Byte(u32BaseAddr + SC2D_C4, u16Data);
    }
    //close write & burst write cmd
    msWrite2Byte(u32BaseAddr + SC2D_C0, 0);

    msDrvSCCommonWrite(u16ScalerMask, FALSE);
}

void msDrvDeGammaEnable(BYTE u8WinIdx, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if(bEnable)
            {
                if(u8ScalerIdx %2)  //Main
                {
                    msWriteBit(u32BaseAddr+SC25_02, TRUE, _BIT0);
                }
                else                //Sub
                {
                    msWriteBit(u32BaseAddr+SC25_22, TRUE, _BIT0);
                }
            }
            else
            {
                if(u8ScalerIdx %2)  //Main
                {
                    msWriteBit(u32BaseAddr+SC25_02, FALSE, _BIT0);
                }
                else                //Sub
                {
                    msWriteBit(u32BaseAddr+SC25_22, FALSE, _BIT0);
                }
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

/*fix gamma*/
void msDrvFixGammaLoadTbl_256E_10B(BYTE u8WinIdx, BYTE u8Color,  BYTE **pu8FixGammaTblIdx)
{
    DWORD u32BaseAddr;
    WORD u16Count,i, u16Data, j;

    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    BYTE u8TriggerBits;

    switch (u8Color){
        case 0: {u8TriggerBits = 0;}break;
        case 1: {u8TriggerBits = BIT5;}break;
        case 2: {u8TriggerBits = BIT6;}break;
        default: {u8TriggerBits = 0;}break;
    }

    u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));

    u16Count = 256;

    msDrvSCCommonWrite(u16ScalerMask, TRUE);
    // address port initial as 0
    msWrite2Byte(u32BaseAddr + SC25_F2, 0x0);
    // pre set value
    msWrite2Byte(u32BaseAddr + SC25_F4, 0x0);

    // RGB write together and write one time for clear write bit.
    msWrite2ByteMask(u32BaseAddr + SC25_F0, BIT(3) | u8TriggerBits, BIT(3) | u8TriggerBits);
    j = 0;
    for(i=0; i<u16Count; i++, j++)
    {

        if ((j % 3) == 0)   //0, 2, 4
            u16Data = pu8FixGammaTblIdx[u8Color][j] + (LOWNIBBLE(pu8FixGammaTblIdx[u8Color][j+2]) << 8);
        else
        {
            u16Data = pu8FixGammaTblIdx[u8Color][j] + (HIGHNIBBLE(pu8FixGammaTblIdx[u8Color][j+1]) << 8);
            j++;
        }

        // waiting ready.
        while (msRead2Byte(u32BaseAddr + SC25_F0) & BIT3);

        msWrite2Byte(u32BaseAddr + SC25_F2, i); //address
        msWrite2Byte(u32BaseAddr + SC25_F4, u16Data); //data
        msWrite2ByteMask(u32BaseAddr + SC25_F0, BIT3, BIT3); //io_w enable

    }

    msWrite2Byte(u32BaseAddr + SC25_F0, 0x0);
    msDrvSCCommonWrite(u16ScalerMask, FALSE);

}

void msDrvFixGammaLoadTbl_256E_16B(BYTE u8WinIdx, BYTE u8Color,  BYTE **pu8FixGammaTblIdx)
{
    DWORD u32BaseAddr;
    WORD u16Count,i, u16Data, j;

    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    BYTE u8TriggerBits;

    switch (u8Color){
        case 0: {u8TriggerBits = 0;}break;
        case 1: {u8TriggerBits = BIT5;}break;
        case 2: {u8TriggerBits = BIT6;}break;
        default: {u8TriggerBits = 0;}break;
    }

    u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));

    u16Count = 256;

    msDrvSCCommonWrite(u16ScalerMask, TRUE);
    // address port initial as 0
    msWrite2Byte(u32BaseAddr + SC25_F2, 0x0);
    // pre set value
    msWrite2Byte(u32BaseAddr + SC25_F4, 0x0);

    // RGB write together and write one time for clear write bit.
    msWrite2ByteMask(u32BaseAddr + SC25_F0, BIT(3) | u8TriggerBits, BIT(3) | u8TriggerBits);
    j = 0;
    for(i=0; i<u16Count; i++, j++)
    {
        u16Data = pu8FixGammaTblIdx[u8Color][j*2] + (pu8FixGammaTblIdx[u8Color][j*2+1] << 8);

        // waiting ready.
        while (msRead2Byte(u32BaseAddr + SC25_F0) & BIT3);

        msWrite2Byte(u32BaseAddr + SC25_F2, i); //address
        msWrite2Byte(u32BaseAddr + SC25_F4, u16Data); //data
        msWrite2ByteMask(u32BaseAddr + SC25_F0, BIT3, BIT3); //io_w enable

    }

    msWrite2Byte(u32BaseAddr + SC25_F0, 0x0);
    msDrvSCCommonWrite(u16ScalerMask, FALSE);
}

void msDrvFixGammaEnable(BYTE u8WinIdx, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if(bEnable)
            {
                if(u8ScalerIdx %2)  //Main
                {
                    msWriteBit(u32BaseAddr+SC25_02, TRUE, _BIT2);
                }
                else                //Sub
                {
                    msWriteBit(u32BaseAddr+SC25_22, TRUE, _BIT2);
                }
            }
            else
            {
                if(u8ScalerIdx %2)  //Main
                {
                    msWriteBit(u32BaseAddr+SC25_02, FALSE, _BIT2);
                }
                else                //Sub
                {
                    msWriteBit(u32BaseAddr+SC25_22, FALSE, _BIT2);
                }
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

}

/******************************************************************************************************************************************
* Histogram Y set RGB mode
******************************************************************************************************************************************/
void msDrvHistogramYSetRGBMode(BYTE u8WinIdx, Bool u8Enable)
{
    BYTE u8ScalerIdx = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                if(u8Enable)
                {
                    //tmp remove it. checking using rule
                    msWriteByteMask(u32BaseAddr+SC1A_09,_BIT2,_BIT2);//Histogram Y report as PC(RGB) mode enable
                    //msWriteByteMask(u32BaseAddr+SC1A_09,_BIT5,_BIT5);//Main window luma curve enable as PC(RGB) mode enable
                    msWriteByteMask(u32BaseAddr + SC2B_C0, 0x00,_BIT7);    //close main cbcr2uv
                }
                else
                {
                    msWriteByteMask(u32BaseAddr+SC1A_09,0x00,_BIT2);
                    //msWriteByteMask(u32BaseAddr+SC1A_09,0x00,_BIT5);//Main window luma curve enable as PC(RGB) mode enable
                    msWriteByteMask(u32BaseAddr + SC2B_C0, _BIT7,_BIT7);    //enable main cbcr2uv
                }
            }
            else
            {
                if(u8Enable)
                {
                    //tmp remove it. checking using rule
                    msWriteByteMask(u32BaseAddr+SC1A_09,_BIT2,_BIT2);//Histogram Y report as PC(RGB) mode enable
                    //msWriteByteMask(u32BaseAddr+SC1A_09,_BIT4,_BIT4);//sub window luma curve enable as PC(RGB) mode enable
                    msWriteByteMask(u32BaseAddr + SC2B_C0, 0,_BIT3);    //close sub cbcr2uv
                }
                else
                {
                    msWriteByteMask(u32BaseAddr+SC1A_09,0x00,_BIT2);
                    //msWriteByteMask(u32BaseAddr+SC1A_09,0x00,_BIT4);//sub window luma curve enable as PC(RGB) mode enable
                    msWriteByteMask(u32BaseAddr + SC2B_C0, _BIT3,_BIT3);    //enable main cbcr2uv
                }
            }
        }

        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

/******************************************************************************************************************************************
* Histogram Set Range
******************************************************************************************************************************************/
#if 0
void msDrvHistogramSetRange(BYTE SCDetWin)
{
    DWORD u32SCRegBase = SCRegBase(SCDetWin);
    WORD u16ImgHDEStart = 0;//PanelHStart/(msDrvISSCDualPort() ? 2:1) + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.x;
    WORD u16ImgVDEStart =0; //g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.y;
    WORD u16ImgHDEEnd = u16ImgHDEStart + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.width - 1;
    WORD u16ImgVDEEnd = u16ImgVDEStart + g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.height - 1;
    //printData(" XXXX SCDetWin==%x",SCDetWin);

#if !ENABLE_SUBSCOUT
    if((SCDetWin&BIT0) == 0) // sub
        return;
#endif


    if ( (g_ScalerInfo[SCDetWin].eSiDispStatus != SC_STATUS_IDLE)
      &&(g_ScalerInfo[SCDetWin].sSiOP2DisplayWin.width != 0) ) // for 2 sc ==> 1 sc case
    {
        if( SCDetWin & BIT0 )//Main
        {
            msWriteByteMask(u32SCRegBase+SC1A_34,(u16ImgHDEStart/8),0xFF);
            msWriteByteMask(u32SCRegBase+SC1A_35,(u16ImgHDEEnd/8),0xFF);
            msWriteByteMask(u32SCRegBase+SC1A_02,(u16ImgVDEStart/8),0xFF);
            msWriteByteMask(u32SCRegBase+SC1A_03,(u16ImgVDEEnd/8),0xFF);
            msWriteByteMask(u32SCRegBase+SC1A_10, _BIT7, _BIT7);   // main histogram range enable
            //printData(" @@@@ Main  u16ImgHDEStart/8==%x",u16ImgHDEStart);
            //printData(" @@@@ Main  u16ImgVDEStart/8==%x",u16ImgVDEStart);
            //printData(" @@@@ Main  u16ImgHDEEnd/8==%x",u16ImgHDEEnd);
            //printData(" @@@@ Main  u16ImgVDEEnd/8==%x",u16ImgVDEEnd);
        }
        else//Sub
        {
            msWriteByteMask(u32SCRegBase+SC1A_36,(u16ImgHDEStart/8),0xFF);
            msWriteByteMask(u32SCRegBase+SC1A_37,(u16ImgHDEEnd/8),0xFF);
            msWriteByteMask(u32SCRegBase+SC1A_06,(u16ImgVDEStart/8),0xFF);
            msWriteByteMask(u32SCRegBase+SC1A_07,(u16ImgVDEEnd/8),0xFF);
            msWriteByteMask(u32SCRegBase+SC1A_10, _BIT3, _BIT3);   // sub histogram range enable
            //printData(" #### Main  u16ImgHDEStart/8==%x",u16ImgHDEStart);
            //printData(" #### Main  u16ImgVDEStart/8==%x",u16ImgVDEStart);
            //printData(" #### Main  u16ImgHDEEnd/8==%x",u16ImgHDEEnd);
            //printData(" #### Main  u16ImgVDEEnd/8==%x",u16ImgVDEEnd);
        }
    }
}
//#else
void msDrvHistogramSetRange(BYTE u8WinIndex, WORD u16HStart, WORD u16HEnd, WORD u16VStart, WORD u16VEnd)
{
    xdata BYTE bWindow = MAIN_WINDOW;

    BYTE ucScalerIdx=0,
         u8ScalerCounter=0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIndex));

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u8ScalerCounter++;
        }
        u16ScalerMask >>= 1;
    }

    if(u8ScalerCounter==1)// One scaler only
    {
        msWriteByteMask(u32BaseAddr+SC18_EA,_BIT2,_BIT2);//VIP 4k2k VIP histogram counters and range

        msWriteByteMask(u32BaseAddr+SC1A_08,_BIT4,_BIT4);                    //New Y' histogram option enable. 0: Histogram in Y. 1: Histogram in Y'.
        msWriteByteMask(u32BaseAddr+SC1A_10,_BIT0,_BIT0);                //Histogram accelerate mode ON
        msWriteByteMask(u32BaseAddr+SC1A_08, 0, (BIT3|BIT2) );               //clear hand shaking
        if( MAIN_WINDOW == bWindow )
            msWriteByteMask(u32BaseAddr+SC1A_08, BIT1, BIT1|BIT0);  // Main window statistic enable
        else
            msWriteByteMask(u32BaseAddr+SC1A_08, BIT0, BIT1|BIT0);  // Sub window statistic enable

        //>>--------------------Range-------------------------------------------------------
        msWriteByte(u32BaseAddr+SC1A_02, u16VStart/16 );                           // set statistic start range (unit: 8 vertical lines)
        msWriteByte(u32BaseAddr+SC1A_03, u16VEnd/16   );                            // set statistic end range (uint: 8 vertical lines)
        msWriteByteMask(u32BaseAddr+SC1A_08, BIT1, BIT1 );                      // set statistic vertical range is valid

        msWrite2Byte(u32BaseAddr+SC1A_34, u16HStart/8);             // set statistic start range (unit: 1 pixel)
        msWrite2Byte(u32BaseAddr+SC1A_35, u16HEnd/8 );                          // set statistic start range (unit: 1 pixel)
        msWriteByteMask(u32BaseAddr+SC1A_10, (MAIN_WINDOW==bWindow)?(BIT7):(BIT3), BIT7|BIT3 );     // Histogram range enable. Main[7], Sub[3]
    }
    else
    {
        u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;

        while(u16ScalerMask != 0)
        {
            if(u16ScalerMask & BIT0)
            {
                if((u16HStart > PANEL_WIDTH/2) && (ucScalerIdx==SCL0_MAIN || ucScalerIdx==SCL1_MAIN || ucScalerIdx==SCL0_SUB || ucScalerIdx==SCL1_SUB));
                else
                {
                    u32BaseAddr = g_ScalerInfo[ucScalerIdx].u32SiRegBase;
                    msWriteByteMask(u32BaseAddr+SC18_EA, _BIT2,_BIT2);//VIP 4k2k VIP histogram counters and range

                    msWriteByteMask(u32BaseAddr+SC1A_08, _BIT4,_BIT4);                    //New Y' histogram option enable. 0: Histogram in Y. 1: Histogram in Y'.
                    msWriteByteMask(u32BaseAddr+SC1A_10, _BIT0,_BIT0);                //Histogram accelerate mode ON
                    msWriteByteMask(u32BaseAddr+SC1A_08, 0, (BIT3|BIT2) );               //clear hand shaking
                    msWriteByteMask(u32BaseAddr+SC1A_08, BIT1, BIT1|BIT0);              //Main Histrogram window enable

                    msWriteByte(u32BaseAddr+SC1A_02, u16VStart/16 );                           // set statistic start range (unit: 8 vertical lines)
                    msWriteByte(u32BaseAddr+SC1A_03, u16VEnd/16   );                            // set statistic end range (uint: 8 vertical lines)
                    msWriteByteMask(u32BaseAddr+SC1A_08, BIT1, BIT1 );                      // set statistic vertical range is valid

                    if((u16HEnd) > (PANEL_WIDTH/2))
                    {
                        if((ucScalerIdx==SCL0_MAIN) || (ucScalerIdx==SCL1_MAIN))//L0 or L1
                        {
                            msWrite2Byte(u32BaseAddr+SC1A_34, u16HStart/8);             // Left Side HStart
                            msWrite2Byte(u32BaseAddr+SC1A_35, (PANEL_WIDTH/16) );    // Left Side HEnd (Panel Width/2/8)
                            msWriteByteMask(u32BaseAddr+SC1A_10, (MAIN_WINDOW==bWindow)?(BIT7):(BIT3), BIT7|BIT3 ); // Histogram range enable. Main[7], Sub[3]
                        }
                        else if((ucScalerIdx==SCR0_MAIN) || (ucScalerIdx==SCR1_MAIN))//R0 or R1
                        {
                            msWrite2Byte(u32BaseAddr+SC1A_34, 0);             // Right Side HStart at 0
                            msWrite2Byte(u32BaseAddr+SC1A_35, ((u16HEnd-u16HStart+1-PANEL_WIDTH/2)/8) );    // Right Side HEnd
                            msWriteByteMask(u32BaseAddr+SC1A_10, (MAIN_WINDOW==bWindow)?(BIT7):(BIT3), BIT7|BIT3 ); // Histogram range enable. Main[7], Sub[3]
                        }
                    }
                    else
                    {
                            msWrite2Byte(u32BaseAddr+SC1A_34, u16HStart/8);             // set statistic start range (unit: 1 pixel)
                            msWrite2Byte(u32BaseAddr+SC1A_35, u16HEnd/8 );                          // set statistic start range (unit: 1 pixel)
                            msWriteByteMask(u32BaseAddr+SC1A_10, (MAIN_WINDOW==bWindow)?(BIT7):(BIT3), BIT7|BIT3 ); // Histogram range enable. Main[7], Sub[3]
                    }
                }
            }
            ucScalerIdx ++;
            u16ScalerMask >>= 1;
        }
    }
}
#endif


void msDrvAutoSetHistogramRange(BYTE u8WinIndex)
{
    BYTE ucScalerIdx=0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[ucScalerIdx].u32SiRegBase;
    xdata WORD u16HStart = 0, u16HEnd = 0, u16VStart = 0, u16VEnd = 0;

    DWORD u32TotalCount = 0;
    volatile bit g_bEnable4K2KCountRange =0;

    while(u16ScalerMask != 0)
    {
        u32BaseAddr = g_ScalerInfo[ucScalerIdx].u32SiRegBase;
        u16HStart = 0;
        u16HEnd = g_ScalerInfo[ucScalerIdx].sSiOP2DisplayWin.width ;
        u16VStart = 0;
        u16VEnd = g_ScalerInfo[ucScalerIdx].sSiOP2DisplayWin.height ;
        u32TotalCount =(DWORD)u16HEnd * u16VEnd/32;

        if(u16ScalerMask & BIT0)
        {


            if (u32TotalCount > 0xFFFF)
                g_bEnable4K2KCountRange=TRUE;
            else
                g_bEnable4K2KCountRange=FALSE;

            msWriteByteMask(u32BaseAddr+SC18_EA, ((g_bEnable4K2KCountRange)?(_BIT2):(0x00)),_BIT2);        //VIP 4k2k VIP histogram counters and range

            if( (ucScalerIdx % 2) == 1)// Main Win
            {
                //V
                msWriteByte(u32BaseAddr+SC1A_02, u16VStart/((g_bEnable4K2KCountRange)?(16):(8)) );                       // set statistic start range (unit: 8 vertical lines)
                msWriteByte(u32BaseAddr+SC1A_03, u16VEnd/((g_bEnable4K2KCountRange)?(16):(8)));
                //H
                msWrite2Byte(u32BaseAddr+SC1A_34, u16HStart/((g_bEnable4K2KCountRange)?(16):(8)));
                msWrite2Byte(u32BaseAddr+SC1A_35, u16HEnd/((g_bEnable4K2KCountRange)?(16):(8)) );
                //msWriteByteMask(u32BaseAddr+SC1A_10, BIT7, BIT7 );  // sub histogram range enable
                #if 1
                drvAdjust_PRINT("#### Main Histrogram Scalar BaseAddr:%x ,u16HStart:%d, u16HEnd:%d \r\n", u32BaseAddr,u16HStart, u16HEnd);
                drvAdjust_PRINT("#### Main Histrogram Scalar BaseAddr:%x ,u16VStart:%d, u16VEnd:%d \r\n", u32BaseAddr,u16VStart, u16VEnd);
                #endif
            }
            else
            {
                //V
                msWriteByte(u32BaseAddr+SC1A_06, u16VStart/((g_bEnable4K2KCountRange)?(16):(8)));                      // set statistic start range (unit: 8 vertical lines)
                msWriteByte(u32BaseAddr+SC1A_07, u16VEnd/((g_bEnable4K2KCountRange)?(16):(8)));
                //H
                msWrite2Byte(u32BaseAddr+SC1A_36, u16HStart/((g_bEnable4K2KCountRange)?(16):(8)));
                msWrite2Byte(u32BaseAddr+SC1A_37, u16HEnd/((g_bEnable4K2KCountRange)?(16):(8)));
                //msWriteByteMask(u32BaseAddr+SC1A_10, BIT3, BIT3 );  // sub histogram range enable
                #if 1
                drvAdjust_PRINT("#### Sub Histrogram Scalar BaseAddr:%x ,u16HStart:%d, u16HEnd:%d \r\n", u32BaseAddr,u16HStart, u16HEnd);
                drvAdjust_PRINT("#### Sub Histrogram Scalar BaseAddr:%x ,u16VStart:%d, u16VEnd:%d \r\n", u32BaseAddr,u16VStart, u16VEnd);
                #endif
            }
        }
        ucScalerIdx ++;
        u16ScalerMask >>= 1;
    }
}

/******************************************************************************************************************************************
* Histogram Report Enable
******************************************************************************************************************************************/
void msDrvHistogramReportEnable(BYTE u8WinIdx)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;


    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            msWriteByteMask(u32BaseAddr+SC1A_10, _BIT0,_BIT0);      //Histogram accelerate mode ON
            msWriteByteMask(u32BaseAddr+SC1A_09,_BIT1,_BIT1);       //Histogram report sum accumulator add 1bit
            //msWriteByteMask(u32BaseAddr+SC1A_08, _BIT4,_BIT4);         //New Y' histogram option enable. 0: Histogram in Y. 1: Histogram in Y'.

            // 1 for set manual H range setting enable in Main window
            if(u8ScalerIdx%2)//Main
            {
                msWriteByteMask( u32BaseAddr+SC1A_08, (BIT1|BIT2), (BIT2|BIT1) );
            }
            else//Sub
            {
                msWriteByteMask( u32BaseAddr+SC1A_08, (BIT0|BIT2), (BIT2|BIT0) );
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

/******************************************************************************************************************************************
* Histogram Check ACK
******************************************************************************************************************************************/
Bool msDrvHistogramCheckACK(BYTE u8WinIdx)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            if(!(msReadByte(u32BaseAddr+SC1A_08) & _BIT3))
            {
                return FALSE;
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
    return TRUE;
}

/******************************************************************************************************************************************
* Histogram get Data
******************************************************************************************************************************************/
xdata DWORD u32Histogram[DISPLAY_MAX_NUMS][32];

void msDrvHistogramGetData(BYTE u8WinIndex ,BYTE u8Section ,DWORD *pu32Data)
{
    xdata BYTE  ucTmp =0;

            for (ucTmp = 0; ucTmp < u8Section; ucTmp++)
            {
                pu32Data[ucTmp] = u32Histogram[u8WinIndex][ucTmp];
                 //reg_total_32_x = Real_total_32_x /128
                //drvAdjust_PRINT("uwArray32[]==[%x]\n\r",pu32Data[ucTmp]);
            }

}

/******************************************************************************************************************************************
* Histogram get Data
******************************************************************************************************************************************/

void msDrvHistogramUpDateData(BYTE u8WinIndex)
{
    xdata BYTE  ucTmp =0;
                   /*use to Story Temp.Histrogram DLC data */
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;
    volatile bit g_bEnable4K2KCountRange =0;

    for (ucTmp = 0; ucTmp < 32; ucTmp++)
    {
        u32Histogram[u8WinIndex][ucTmp] = 0x00000000;
    }
    g_u16Total_Weight[u8WinIndex]=0x00000000;
    g_u16Total_Count[u8WinIndex]=0x00000000;
    g_u8Ymax[u8WinIndex]=0x00;
    g_u8Ymin[u8WinIndex]=0xFF;
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            if (msReadByte(u32BaseAddr+SC18_EA)&BIT2)
                g_bEnable4K2KCountRange=TRUE;
            else
                g_bEnable4K2KCountRange=FALSE;

            for (ucTmp = 0; ucTmp < 32; ucTmp++)
            {
                u32Histogram[u8WinIndex][ucTmp] += ((WORD)msRead2Byte(u32BaseAddr+SC1A_80+ucTmp*2)*((g_bEnable4K2KCountRange)?(4):(1)));
                //drvAdjust_PRINT(" !!!!!!!!!!uwArray32[%x]===>>%x \r\n", (u32BaseAddr+SC1A_80+ucTmp*2), u32Histogram[u8WinIndex][ucTmp]);
                drvAdjust_PRINT("#### Histrogram uwArray32[%x]===>>%x \r\n", (u32BaseAddr+SC1A_80+ucTmp*2), u32Histogram[u8WinIndex][ucTmp]);
            }
            //<<-----------------------------------------------------------------------------------
            //>>--------------------Total Weight and Count , Min. and Maximum report----------------
            g_u16Total_Weight[u8WinIndex] += msRead2Byte(u32BaseAddr+SC1A_0C)*((g_bEnable4K2KCountRange)?(4):(1));//Pixel Weight Total Y
            g_u16Total_Count[u8WinIndex] += msRead2Byte(u32BaseAddr+SC1A_0E)*((g_bEnable4K2KCountRange)?(4):(1));//Pixel Count Total
            drvAdjust_PRINT("#### Histrogram  total weight : %x, total Count : %x \r\n", g_u16Total_Weight[u8WinIndex], g_u16Total_Count[u8WinIndex]);
            //<<-----------------------------------------------------------------------------------
            if(g_u8Ymax[u8WinIndex] < msReadByte(u32BaseAddr+SC1A_16))
                g_u8Ymax[u8WinIndex] = msReadByte(u32BaseAddr+SC1A_16);

            if(g_u8Ymin[u8WinIndex] > msReadByte(u32BaseAddr+SC1A_17))
                g_u8Ymin[u8WinIndex]= msReadByte(u32BaseAddr+SC1A_17);
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvHistogramRangeEnable(BYTE u8WinIndex, BYTE bEnable)
{
    BYTE ucScalerIdx=0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIndex].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[ucScalerIdx].u32SiRegBase;


    while(u16ScalerMask != 0)
    {
        u32BaseAddr = g_ScalerInfo[ucScalerIdx].u32SiRegBase;

        if(u16ScalerMask & BIT0)
        {
            if( (ucScalerIdx % 2) == 1)// Main Win
            {
                msWriteByteMask(u32BaseAddr+SC1A_10, bEnable? BIT7: 0, BIT7 );  // main histogram range enable
            }
            else
            {
                msWriteByteMask(u32BaseAddr+SC1A_10, bEnable? BIT3: 0, BIT3 );  // sub histogram range enable
            }
        }
        ucScalerIdx ++;
        u16ScalerMask >>= 1;
    }

}

/******************************************************************************************************************************************
* DLC
******************************************************************************************************************************************/

void msDrvSetDLCCurve(BYTE u8WinIdx, BYTE* u8Table)
{

    BYTE u8ScalerIdx = 0, i = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            //printData("u32BaseAddr===%x",u32BaseAddr);

            if(u8ScalerIdx % 2) //Main
            {
                for(i = 0 ; i < 16 ; i++)
                    msWriteByte(u32BaseAddr + SC1A_60 + i,u8Table[i] );
            }
            else                //Sub
            {
                for(i = 0 ; i < 16 ; i++)
                    msWriteByte(u32BaseAddr + SC1A_70 + i,u8Table[i] );
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}


void msDrvSetDLCEnable(BYTE u8WinIdx, Bool bEnable, EN_WINDOW_COLOR ecolor)
{

    BYTE u8ScalerIdx = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if(u8ScalerIdx % 2) //Main
            {
                msWriteByteMask(u32BaseAddr + SC1A_08,(bEnable ? BIT7 : 0), BIT7 );
                if (ecolor == eWIN_COLOR_RGB && bEnable)
                {
                    msWriteByteMask(u32BaseAddr+SC1A_09,_BIT5,_BIT5);//Main window luma curve enable as PC(RGB) mode enable
                }
                else
                {
                    msWriteByteMask(u32BaseAddr+SC1A_09,0x00,_BIT5);//Main window luma curve enable as PC(RGB) mode enable
                }

            }
            else                //Sub
            {
                msWriteByteMask(u32BaseAddr + SC1A_08,(bEnable ? BIT6 : 0), BIT6 );
                if (ecolor == eWIN_COLOR_RGB && bEnable)
                {
                    msWriteByteMask(u32BaseAddr+SC1A_09,_BIT4,_BIT4);//sub window luma curve enable as PC(RGB) mode enable
                }
                else
                {
                    msWriteByteMask(u32BaseAddr+SC1A_09,0x00,_BIT4);//sub window luma curve enable as PC(RGB) mode enable
                }
            }
        }

        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

WORD const tMatrix_601_Limited[]=
{
    0x1C1,  0x1E88, 0x1FB8,
    0x107,  0x204,  0x64,
    0x1F69, 0x1ED7, 0x1C1,
};
WORD const tMatrix_601_FullRange[]=
{
    0x20B,  0x1E4A, 0x1FAC,
    0x132,  0x259,  0x74,
    0x1F50, 0x1EA5, 0x20B,
};

#if 1
WORD const tMatrix_709_Limited[]=
{
    0x1C1,  0x1E68, 0x1FD8,
    0xBB,   0x274,  0x3F,
    0x1F99, 0x1EA6, 0x1C1,
};
WORD const tMatrix_709_FullRange[]=
{
    0x20B,  0x1E25, 0x1FD0,
    0xDA,   0x2DC,  0x49,
    0x1F89, 0x1E6D, 0x20B,
};
#else
WORD const tMatrix_709_Limited[]=
{
    0x20B,  0x1E25, 0x1FD0,
    0xDA,   0x2DC,  0x49,
    0x1F89, 0x1E6D, 0x20B,
};
WORD const tMatrix_709_FullRange[]=
{
    0x1C1,  0x1E68, 0x1FD8,
    0xBB,   0x274,  0x3F,
    0x1F99, 0x1EA6, 0x1C1,
};
#endif
WORD msDrvSelectColorFormatCSCCofficient(BYTE u8Colorime,BYTE u8Range, BYTE u8index)
{
    if(u8Colorime == 0 && u8Range == 0)
        return tMatrix_601_Limited[u8index];
    else if(u8Colorime == 0 && u8Range == 1)
        return tMatrix_601_FullRange[u8index];
    else if(u8Colorime == 1 && u8Range == 0)
        return tMatrix_709_Limited[u8index];
    else //if(u8Colorime == 1 && u8Range == 1)
        return tMatrix_709_FullRange[u8index];
}

void msDrvColorFormat_CSCEnable(BYTE u8WinIdx, BYTE u8CSCSel, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
    BYTE u8MxCnt=0;
    BYTE u8PortIdx = MapWin2Port(u8WinIdx);
    EN_COLOR_RANGE_TYPE u8ColorRange = (EN_COLOR_RANGE_TYPE)msAPI_combo_IPGetColorFormat((EN_COMBO_IP_SELECT)(g_InputPort[u8PortIdx].eIPMux)).ucColorRange;
    EN_INPUT_YUV_COLORIMETRY_TYPE u8Colorimetry;

#if ENABLE_HDMI_INPUT||ENABLE_DP_INPUT
    if((INPUT_IS_TMDS(u8PortIdx)||INPUT_IS_DISPLAYPORT(u8PortIdx)) && (msAPISrcGetPktYuvColorimetry(u8WinIdx)!=eINPUT_YUV_COLORIMETRY_ITU601))
	{
        u8Colorimetry = eINPUT_YUV_COLORIMETRY_ITU709;
	}
    else
#endif
	{
        u8Colorimetry = eINPUT_YUV_COLORIMETRY_ITU601;
	}

    if(u8CSCSel == IP_CSC_MATRIX)
        u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskIn;

#if EABLE_Y_EXTENSION
    u8ColorRange = COMBO_COLOR_RANGE_LIMIT;
#endif

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            if(u8CSCSel == IP_CSC_MATRIX)
            {
                if(u8ScalerIdx % 2) //Main
                {
                    msWriteByteMask(u32BaseAddr + SC02_80, (bEnable ? BIT3 : 0), BIT3);//Disable Main CSC
                    msWriteByteMask(u32BaseAddr + SC02_02, (bEnable ? BIT2 : 0), BIT2);//Disable Main CSC Dither
                    msWriteByteMask(u32BaseAddr + SC02_80, (bEnable && (u8ColorRange == COMBO_COLOR_RANGE_LIMIT) ? 0: BIT0), BIT0);
                    msWriteByteMask(u32BaseAddr + SC02_80, (bEnable && (u8Colorimetry == eINPUT_YUV_COLORIMETRY_ITU709) ? BIT1:0), BIT1);
                }
                else                //Sub
                {
                    msWriteByteMask(u32BaseAddr + SC04_80, (bEnable ? BIT3 : 0), BIT3);//Disable Sub CSC
                    msWriteByteMask(u32BaseAddr + SC04_02, (bEnable ? BIT2 : 0), BIT2);//Disable Sub CSC Dither
                    msWriteByteMask(u32BaseAddr + SC04_80, (bEnable && (u8ColorRange == COMBO_COLOR_RANGE_LIMIT) ? 0: BIT0), BIT0);
                    msWriteByteMask(u32BaseAddr + SC04_80, (bEnable && (u8Colorimetry == eINPUT_YUV_COLORIMETRY_ITU709) ? BIT1:0), BIT1);
                }
            }
            else
            {
                if(u8ScalerIdx % 2) //Main
                {
                    msWriteByteMask(u32BaseAddr + SC18_0C, (bEnable ? BIT7|BIT6|BIT5|BIT0 : 0x00), BIT7|BIT6|BIT5|BIT0);
                    for(u8MxCnt=0; u8MxCnt<9; u8MxCnt++)
                    {
                            msWrite2Byte(u32BaseAddr + SC18_0E + u8MxCnt*2, msDrvSelectColorFormatCSCCofficient(u8Colorimetry, u8ColorRange, u8MxCnt));
                    }
                }
                else                //Sub
                {
                    msWriteByteMask(u32BaseAddr + SC18_EC, (bEnable ? BIT7|BIT6|BIT5|BIT0 : 0x00), BIT7|BIT6|BIT5|BIT0);
                    for(u8MxCnt=0; u8MxCnt<9; u8MxCnt++)
                    {
                            msWrite2Byte(u32BaseAddr + SC18_EE + u8MxCnt*2, msDrvSelectColorFormatCSCCofficient(u8Colorimetry, u8ColorRange, u8MxCnt));
                    }
                }
            }
        }

        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvColorFormat_YSub16Enable(BYTE u8WinIdx, BYTE u8YSUBSel, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if(u8ScalerIdx % 2) //Main
            {
                if(u8YSUBSel == ACE_Y_SUB)
                    msWriteByteMask(u32BaseAddr + SC0F_AE, (bEnable ? BIT6 : 0), BIT6);//Disable Main Y-16
                else
                    msWriteByteMask(u32BaseAddr + SC1A_1E, (bEnable ? 0xF0 : 0), 0xFF);//Disable Main Y-16
            }
            else                //Sub
            {
                if(u8YSUBSel == ACE_Y_SUB)
                    msWriteByteMask(u32BaseAddr + SC0F_AE, (bEnable ? BIT7 : 0), BIT7);//Disable Sub Y-16
                else
                    msWriteByteMask(u32BaseAddr + SC1A_1F, (bEnable ? 0xF0 : 0), 0xFF);//Disable Sub Y-16
            }
        }

        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvDeGamma_Compress_DMA_Reader_Trigger(BYTE u8WinIdx)
{
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = SCRegBase ( MapWin2ScIndex ( u8WinIdx ) );

    msWrite4Byte(SCL0_5A_2C, (AUTOLOAD_GMA_ADR>>4));   //   base address of allocated memory for client1
    msWrite2Byte(SCL0_5A_34, /*(AUTOLOAD_GMA_LEN>>4)*/300);   //   the amount of client1 auto download data
    msWrite2Byte(SCL0_5A_3A, /*(AUTOLOAD_GMA_LEN>>4)*/300);   //   length of client1 DMA's request  PS:16byte per entry

    msDrvSCCommonWrite(u16ScalerMask, ENABLE);
    msWriteByteMask(u32BaseAddr+SC25_F1, 1, BIT0);
    msDrvSCCommonWrite(u16ScalerMask, DISABLE);

    // trigger
    msWriteBit(SCL0_5A_22, TRUE, BIT2);

    TimeOutCounter =50;
    while((!(msReadByte(SCL0_5A_22) &_BIT2)) && TimeOutCounter){}

    TimeOutCounter = 50;
    while((msReadByte(SCL0_5A_22) &_BIT2) && TimeOutCounter){}

    msDrvSCCommonWrite(u16ScalerMask, ENABLE);
    msWriteByteMask(u32BaseAddr+SC25_F1, 0, BIT0);
    msDrvSCCommonWrite(u16ScalerMask, DISABLE);
}

void msDrvDeGamma_DMA_Reader_Trigger(BYTE u8WinIdx)
{
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = SCRegBase ( MapWin2ScIndex ( u8WinIdx ) );

    msWrite4Byte(SCL0_5A_2C, (AUTOLOAD_GMA_ADR>>4));   //   base address of allocated memory for client1
    msWrite2Byte(SCL0_5A_34, (AUTOLOAD_GMA_LEN>>4));   //   the amount of client1 auto download data
    msWrite2Byte(SCL0_5A_3A, (AUTOLOAD_GMA_LEN>>4));   //   length of client1 DMA's request  PS:16byte per entry

    msDrvSCCommonWrite(u16ScalerMask, ENABLE);
    msWriteByteMask(u32BaseAddr+SC25_F1, 1, BIT0);
    msDrvSCCommonWrite(u16ScalerMask, DISABLE);

    // trigger
    msWriteBit(SCL0_5A_22, TRUE, BIT2);

    TimeOutCounter =50;
    while((!(msReadByte(SCL0_5A_22) &_BIT2)) && TimeOutCounter){}

    TimeOutCounter = 50;
    while((msReadByte(SCL0_5A_22) &_BIT2) && TimeOutCounter){}

    msDrvSCCommonWrite(u16ScalerMask, ENABLE);
    msWriteByteMask(u32BaseAddr+SC25_F1, 0, BIT0);
    msDrvSCCommonWrite(u16ScalerMask, DISABLE);
}

void msDrvGamma_DMA_Reader_Trigger(BYTE u8WinIdx)
{
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = SCRegBase ( MapWin2ScIndex ( u8WinIdx ) );

    msWrite4Byte(SCL0_5A_04, (AUTOLOAD_GMA_ADR>>4));   //   base address of allocated memory for client1
    msWrite2Byte(SCL0_5A_10, (AUTOLOAD_GMA_LEN>>4));   //   the amount of client1 auto download data
    msWrite2Byte(SCL0_5A_16, (AUTOLOAD_GMA_LEN>>4));   //   length of client1 DMA's request  PS:16byte per entry

    msDrvSCCommonWrite(u16ScalerMask, ENABLE);
    msWriteByteMask(u32BaseAddr+SC10_DB, BIT3, BIT3);
    msDrvSCCommonWrite(u16ScalerMask, DISABLE);

    // trigger
    msWriteBit(SCL0_5A_02, TRUE, BIT0);

    TimeOutCounter=50;
    while((!(msReadByte(SCL0_5A_02) &_BIT0)) && TimeOutCounter){}

    TimeOutCounter = 50;
    while((msReadByte(SCL0_5A_02) &_BIT0)&& TimeOutCounter){}

    msDrvSCCommonWrite(u16ScalerMask, ENABLE);
    msWriteByteMask(u32BaseAddr+SC10_DB, 0, BIT3);
    msDrvSCCommonWrite(u16ScalerMask, DISABLE);

}

void msDrvFixGamma_DMA_Reader_Trigger(BYTE u8WinIdx)
{
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = SCRegBase ( MapWin2ScIndex ( u8WinIdx ) );

    msWrite4Byte(SCL0_5A_2C, (AUTOLOAD_GMA_ADR>>4));   //   base address of allocated memory for client1
    msWrite2Byte(SCL0_5A_34, (AUTOLOAD_GMA_LEN>>4) / 4);   //   the amount of client1 auto download data
    msWrite2Byte(SCL0_5A_3A, (AUTOLOAD_GMA_LEN>>4) / 4);   //   length of client1 DMA's request  PS:16byte per entry

    msDrvSCCommonWrite(u16ScalerMask, ENABLE);
    msWriteByteMask(u32BaseAddr+SC25_F1, 1, BIT0);
    msDrvSCCommonWrite(u16ScalerMask, DISABLE);

    // trigger
    msWriteBit(SCL0_5A_22, TRUE, BIT2);

    TimeOutCounter=50;
    while((!(msReadByte(SCL0_5A_22) &_BIT2)) && TimeOutCounter){}

    TimeOutCounter = 50;
    while((msReadByte(SCL0_5A_22) &_BIT2)&& TimeOutCounter){}

    msDrvSCCommonWrite(u16ScalerMask, ENABLE);
    msWriteByteMask(u32BaseAddr+SC25_F1, 0, BIT0);
    msDrvSCCommonWrite(u16ScalerMask, DISABLE);
}

//============= ICC =========================================

void msDrvLoadICCInitialTable(void)
{
/*
    WORD wAddress = 0 , wData;
    DWORD dwAddr;
    BYTE u8Loop;

    for(u8Loop =  0 ; u8Loop <4 ; u8Loop++)
    {
        dwAddr =  0x102B00;
        dwAddr += (u8Loop*0x200);
        //DB_printData("\ndwAddr = 0x%x",(dwAddr>>8));

        msWriteByte(dwAddr+SC1C_F0, 0x01);
        for(wAddress = 0 ; wAddress <=0x3FF ; wAddress++)
        {
            msWrite2Byte(dwAddr+SC1C_F2, wAddress);
            wData = tICCInitialTable[wAddress];

            msWrite2Byte(dwAddr+SC1C_F4,wData);
            msWriteByteMask(dwAddr+SC1C_F5, BIT7,BIT7);
            DelayUs(1);
        }
        msWriteByte(dwAddr+SC1C_F0, 0x00);

    }
    */
    msPQ_UserDumpTable(0xFF, 0xAB, 0x00);   //dump 6 wins ICC sram table

}

/******************************************************************************************************************************************
* Custom Saturation R/G/B/C/M/Y
******************************************************************************************************************************************/
void msDrvCustomSaturationEnable(BYTE u8WinIdx, Bool bEnable)
{

    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWriteByteMask(u32BaseAddr + SC2B_C0, (bEnable ? _BIT6: 0x00),_BIT6);
                msWrite2Byte(u32BaseAddr + SC2B_D8,0x0000);
                msWrite2Byte(u32BaseAddr + SC5E_D8,0x0000);
            }
            else
            {
                msWriteByteMask(u32BaseAddr + SC2B_C0, (bEnable ? _BIT2: 0x00),_BIT2);
                msWrite2Byte(u32BaseAddr + SC2B_DA,0x0000);
                msWrite2Byte(u32BaseAddr + SC5E_DA,0x0000);
            }
            msWriteByte(u32BaseAddr + SC2B_D6,0x09);//ICC decrease saturation common gain,
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }


}

void msDrvAdjustCustomSaturation(BYTE u8WinIdx, BYTE u8HueValue, BYTE u8RegOffset)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    WORD u16ColorInedx = BIT0;
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
                {
                    if(u8RegOffset <= 0x0F)
            {

                if(u8HueValue&0x80)
                {
                    msWrite2ByteMask(u32BaseAddr + SC2B_D2,(u16ColorInedx<<u8RegOffset) ,(u16ColorInedx<<u8RegOffset));
                }
                else
                {
                    msWrite2ByteMask(u32BaseAddr + SC2B_D2,0 ,(u16ColorInedx<<u8RegOffset));
                }

            msWriteByteMask((u32BaseAddr + SC2B_C2+u8RegOffset),u8HueValue ,0x0F);
            }
            else  //  > 0x0F
            {

                if(u8HueValue&0x80)
                {
                    msWrite2ByteMask(u32BaseAddr + SC5E_D2,(u16ColorInedx<<(u8RegOffset&0x0F)) ,(u16ColorInedx<<(u8RegOffset&0x0F)));
                }
                else
                {
                    msWrite2ByteMask(u32BaseAddr + SC5E_D2,0 ,(u16ColorInedx<<(u8RegOffset&0x0F)));
                }

            msWriteByteMask((u32BaseAddr + SC5E_C2+(u8RegOffset&0x0F)),u8HueValue ,0x0F);
            }
                }
            else
                {
                if(u8RegOffset <= 0x0F)
                    {
                if(u8HueValue&0x80)
                {
                    msWrite2ByteMask(u32BaseAddr + SC2B_D4,(u16ColorInedx<<u8RegOffset) ,(u16ColorInedx<<u8RegOffset));
                }
                else
                {
                    msWrite2ByteMask(u32BaseAddr + SC2B_D4,0 ,(u16ColorInedx<<u8RegOffset));
                }

            msWriteByteMask(u32BaseAddr + SC2B_C2+u8RegOffset,(u8HueValue<<4) ,0xF0);
                    }
        else
            {
                if(u8HueValue&0x80)
                {
                    msWrite2ByteMask(u32BaseAddr + SC5E_D4,(u16ColorInedx<<(u8RegOffset&0x0F)) ,(u16ColorInedx<<(u8RegOffset&0x0F)));
                }
                else
                {
                    msWrite2ByteMask(u32BaseAddr + SC5E_D4,0 ,(u16ColorInedx<<(u8RegOffset&0x0F)));
                }

            msWriteByteMask(u32BaseAddr + SC5E_C2+(u8RegOffset&0x0F),(u8HueValue<<4) ,0xF0);
            }
                }

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}


void msDrvCustomBrightnessEnable(BYTE u8WinIdx, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWriteByteMask(u32BaseAddr + SC1C_20, (bEnable ? 0x80: 0x00),0x80);
                //msWriteByteMask(u32BaseAddr + SC2B_C0, 0x80,0x80); //must enable cbcr to uv
            }
            else
            {
                msWriteByteMask(u32BaseAddr + SC1C_20, (bEnable ? 0x40: 0x00),0x40);
                //msWriteByteMask(u32BaseAddr + SC2B_C0, 0x08,0x08); //must enable cbcr to uv
            }

            msWriteByte(u32BaseAddr + SC1C_43,0xFF);//IBC c weight min limit and IBC y weight min limit
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

}

void msDrvAdjustCustomBrightness(BYTE u8WinIdx, BYTE u8BriValue, BYTE u8RegOffset)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWriteByteMask(u32BaseAddr + SC1C_22 + u8RegOffset, u8BriValue, 0x3F);
            }
            else
            {
                msWriteByteMask(u32BaseAddr + SC1C_32 + u8RegOffset, u8BriValue, 0x3F);
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

}

/******************************************************************************************************************************************
* Set XVYCC Color Matrix Enable/Disable
******************************************************************************************************************************************/
void msDrvColorMatrixEnable(BYTE u8WinIdx, Bool bEnable, Bool bCarryEn)
{
    BYTE u8ScalerIdx = 0;
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

                if(u8ScalerIdx %2)  //Main
                {
                msWriteBit(u32BaseAddr+SC25_02, bEnable, _BIT1);
                msWriteBit(u32BaseAddr+SC25_02, bCarryEn, _BIT7);
                }
                else                //Sub
                {
                msWriteBit(u32BaseAddr+SC25_22, bEnable, _BIT1);
                msWriteBit(u32BaseAddr+SC25_22, bCarryEn, _BIT7);
            }

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

/******************************************************************************************************************************************
* Custom Hue R/G/B/C/M/Y/Skin
******************************************************************************************************************************************/
void msDrvVIPHueEnable(BYTE u8WinIdx, Bool bEnable)
{

    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;

    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                msWriteByteMask(u32BaseAddr + SC1C_48, (bEnable ? _BIT7: 0x00),_BIT7);
            }
            else
            {
                msWriteByteMask(u32BaseAddr + SC1C_48, (bEnable ? _BIT6: 0x00),_BIT6);
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }

}

//u8RegOffset : 0 : Grey, 1 : R, 2 : G, 3 : B, 4 : C, 5 : M, 6 : Y, 7 : SKIN
void msDrvAdjustVIPHue(BYTE u8WinIdx, BYTE u8HueValue, BYTE u8RegOffset)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
            if(u8HueValue&0x80)
            {
                u8HueValue |= _BIT6;
            }

            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                 if(u8RegOffset <= 0x0F)
                 {
                      msWriteByteMask((u32BaseAddr + SC1C_4A + u8RegOffset), u8HueValue , 0x7F);
                 }
                 else  //  > 0x0F
                 {
                      msWriteByteMask((u32BaseAddr + SC5D_4A + (u8RegOffset&0x0F)),u8HueValue ,0x7F);
                 }
            }
            else
            {
                if(u8RegOffset <= 0x0F)
                {
                      msWriteByteMask(u32BaseAddr + SC1C_5A + u8RegOffset, u8HueValue, 0x7F);
                }
                else
                {
                      msWriteByteMask(u32BaseAddr + SC5D_5A + (u8RegOffset&0x0F), u8HueValue, 0x7F);
                }
            }

        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvFCCEnable(BYTE u8WinIdx, BYTE FCCRegion, Bool bEnable)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            if( (u8ScalerIdx % 2) == 1)// Main Win
            {
                BYTE RegionIdx = (0x01 << ((FCCRegion-1)%8));
                msWriteBit((u32BaseAddr + SC18_20 + (FCCRegion/8)), bEnable, RegionIdx);   //Enable main win FCC Region
            }
            else
            {
                BYTE RegionIdx = (0x01 << ((FCCRegion-1)%8));
                msWriteBit((u32BaseAddr + SC18_22 + (FCCRegion/8)), bEnable, RegionIdx);   //Enable sub win FCC Region
            }
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvAdjustFCC(BYTE u8WinIdx, BYTE FCCRegion, BYTE Strength, BYTE CbTarget, BYTE CrTarget, BYTE Distance)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8WinIdx));
    WORD u16ScalerMask = g_DisplayWindow[u8WinIdx].u16DwScMaskOut;
    while(u16ScalerMask)
    {
        if(u16ScalerMask & BIT0)
        {
            u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;

            BYTE FCCStrength = (Strength & 0x0F) << (FCCRegion%2==0) ? 4 : 0;
            msWriteByteMask((u32BaseAddr + SC18_40 + ((FCCRegion-1)/2)), FCCStrength, (FCCRegion%2==0) ? 0xF0 : 0x0F);   //Set FCC Region x Strength

            msWriteByte((u32BaseAddr + SC18_30 + ((FCCRegion-1)*2)), CbTarget); //Set FCC Region x CbTarget

            msWriteByte((u32BaseAddr + SC18_31 + ((FCCRegion-1)*2)), CrTarget); //Set FCC Region x CrTarget

            msWriteByte((u32BaseAddr + SC18_48 + ((FCCRegion-1))), Distance); //Set FCC Region x Distance
        }
        u8ScalerIdx++;
        u16ScalerMask >>= 1;
    }
}

void msDrvGetWinColorReport(WORD* minR, WORD *maxR, WORD *minG, WORD *maxG, WORD *minB, WORD *maxB)
{

    BYTE ucScalerIdx=0;
    WORD u16ScalerMask = g_DisplayWindow[USER_PREF_WIN_SEL].u16DwScMaskOut;
    DWORD u32BaseAddr = g_ScalerInfo[ucScalerIdx].u32SiRegBase;
    WORD u16HStart = 0, u16HEnd = 0, u16VStart = 0, u16VEnd = 0;
    WORD midX;
    WORD tmp[6] = {0};

    *minR = *minG = *minB = 0xFFFF;
    *maxR = *maxG = *maxB = 0x0000;

    while(u16ScalerMask != 0)
    {
        u32BaseAddr = g_ScalerInfo[ucScalerIdx].u32SiRegBase;
        u16HStart = msRead2Byte(u32BaseAddr + SC10_10);
        u16HEnd = g_DisplayWindow[USER_PREF_WIN_SEL].sDwDisplayWin.width;
        u16VStart = msRead2Byte(u32BaseAddr + SC10_14);
        u16VEnd = g_DisplayWindow[USER_PREF_WIN_SEL].sDwDisplayWin.height;
        midX = (u16HEnd / 2);

        if(u16ScalerMask & BIT0)
        {
            if( (ucScalerIdx % 2) == 1)// Main Win
            {
                msWriteByteMask(u32BaseAddr + SC0F_AF, BIT0, BIT0);
                msWrite2Byte(u32BaseAddr + SC0F_84, u16HStart); //set window x start position
                msWrite2Byte(u32BaseAddr + SC0F_86, u16HStart+midX); //set window x end position
                msWrite2Byte(u32BaseAddr + SC0F_88, u16VStart); //set window y start position
                msWrite2Byte(u32BaseAddr + SC0F_8A, u16VStart+u16VEnd); //set window y end position

                tmp[0] = msRead2Byte(u32BaseAddr + SC0F_C8);
                tmp[1] = msRead2Byte(u32BaseAddr + SC0F_CA);
                tmp[2] = msRead2Byte(u32BaseAddr + SC0F_CC);
                tmp[3] = msRead2Byte(u32BaseAddr + SC0F_CE);
                tmp[4] = msRead2Byte(u32BaseAddr + SC0F_D0);
                tmp[5] = msRead2Byte(u32BaseAddr + SC0F_D2);

                if(*minR > tmp[0])
                 *minR = tmp[0];

                if(*minG > tmp[2])
                 *minG = tmp[2];

                if(*minB > tmp[4])
                 *minB = tmp[4];

                if(*maxR < tmp[1])
                 *maxR = tmp[1];

                if(*maxG < tmp[3])
                 *maxG = tmp[3];

                if(*maxB < tmp[5])
                 *maxB = tmp[5];

            }
        }
        ucScalerIdx ++;
        u16ScalerMask >>= 1;
    }

}


