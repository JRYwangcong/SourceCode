
#define _DRVOSDBLEND_C

#include "Global.h"
#include "sysinfo2_MST9U.h"
#include "drvOSDblend.h"
#include "drvmsOVD.h"
#define OSDD_SCALER_BYPASS         0  //duplicae

#define OCTG_HDE_START      0x20
#define OCTG_VDE_START      0x10

#define OSDC_GB             16
#define OSDC_VFREQ          30

// OSD
sOSDBLEND_CONFIG sOsdblendConfig;
static DWORD u32OcBaseaddrL = ((DWORD)OSDC_L_ADR)>>5;
static DWORD u32OcBaseaddrR = ((DWORD)OSDC_R_ADR)>>5;
static DWORD u32OcClock = 0;
static WORD u16OcOsdWidth = PANEL_WIDTH;
static WORD u16OcOsdHeight = PANEL_HEIGHT;

// Small OSD Canvas
static WORD u16OcHPos = 0; //
static WORD u16OcVPos = 0;
static BYTE bOcGenReset = FALSE;

#define DRVOSDBLEND_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&DRVOSDBLEND_DEBUG
    #define DRVOSDBLEND_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
    #define DRVOSDBLEND_PRINT(format, ...)
#endif

static void msDrvOSDBlend_CalcHtt(WORD u16HSize, WORD u16VSize, BYTE u8VFreq, WORD *pHtt, WORD *pVtt)
{
    WORD u16Htt, u16HttMin;
    WORD u16VttMin;

    u16VttMin = OCTG_VDE_START*2 + u16VSize;
    u16HttMin = OCTG_HDE_START*2 + u16HSize;

    u16Htt = u32OcClock / ((DWORD)u16VttMin * u8VFreq);

    if( u16Htt > 0x1FFF )
    {
        *pHtt = 0x1FFF;
        *pVtt = u32OcClock / ((DWORD)0x1FFF * u8VFreq);
    }
    else if( u16Htt > u16HttMin )
    {
        *pHtt = u16Htt;
        *pVtt = u16VttMin;
    }
    else
    {
        *pHtt = u16HttMin;
        *pVtt = u16VttMin;
    }
}

////////////////////////////////////////////////////////////////////////
// Small Canvas, Fix OC OSD Size, Adjust OSD position by OC
// H, V position pixel, from 0
void msDrvOSDBlend_WindowPos(WORD u16HPos, WORD u16YPos)
{
    u16OcHPos = u16HPos & (~0x7);
    u16OcVPos = u16YPos;
}

// H, V position %
void msDrvOSDBlend_WindowPosRatio(BYTE u16HRatio, WORD u16VRatio)
{
    u16OcHPos = (((DWORD)(PANEL_WIDTH - u16OcOsdWidth) * u16HRatio) / 100) & (~0x7);
    u16OcVPos = ((DWORD)(PANEL_HEIGHT - u16OcOsdHeight) * u16VRatio) / 100;
}

// OSD H, V size
void msDrvOSDBlend_WindowSize(WORD u16Width, WORD u16Height)
{
    u16OcOsdWidth = ALIGN_16(u16Width);
    u16OcOsdHeight = u16Height;
    bOcGenReset = TRUE;
}

void msDrvOSDBlend_WindowReconfig(void)
{
    BYTE bLSide, bRSide;
    WORD u16PanelMid = PANEL_WIDTH / 2;
    WORD u16LSize, u16LMid, u16LLast, u16LHPos, u16LVPos;
    WORD u16RSize, u16RStart, u16RMid, u16RLast, u16RHPos, u16RVPos;
    WORD u16Htt, u16Vtt;

    // Right Side
    if( u16OcHPos >= u16PanelMid )
    {
        u16LSize = u16OcOsdWidth;
        u16LMid = u16LSize / 2 - 1;
        u16LLast = u16LSize - 1;
        u16LHPos = 0;
        u16LVPos = 0;

        u16RSize = u16LSize;
        u16RStart = 0;
        u16RMid = 0;
        u16RLast = 0;
        u16RHPos = u16OcHPos - u16PanelMid + OD_GUARD_BAND;
        u16RVPos = u16OcVPos;

        bLSide = FALSE;
        bRSide = TRUE;
    }
    // Middle
    else if( (u16OcHPos + u16OcOsdWidth) >= u16PanelMid )
    {
        u16LSize = u16PanelMid - u16OcHPos;
        u16LMid = u16LSize / 2 - 1;
        u16LLast = u16LSize - 1;
        u16LHPos = u16OcHPos;
        u16LVPos = u16OcVPos;

        u16RSize = u16OcOsdWidth - u16LSize;
        u16RStart = u16LSize;
        u16RMid = u16LSize + u16RSize / 2 - 1;
        u16RLast = u16OcOsdWidth - 1;
        u16RHPos = OD_GUARD_BAND;
        u16RVPos = u16OcVPos;

        bLSide = TRUE;
        bRSide = TRUE;
    }
    // Left Side
    else
    {
        u16LSize = u16OcOsdWidth;
        u16LMid = u16LSize / 2 - 1;
        u16LLast = u16LSize - 1;
        u16LHPos = u16OcHPos;
        u16LVPos = u16OcVPos;

        u16RSize = 0;
        u16RStart = 0;
        u16RMid = 0;
        u16RLast = 0;
        u16RHPos = 0;
        u16RVPos = 0;

        bLSide = TRUE;
        bRSide = FALSE;
    }

    msDrvOSDBlend_CalcHtt(u16OcOsdWidth, u16OcOsdHeight, OSDC_VFREQ, &u16Htt, &u16Vtt);

    // oc
    if( bOcGenReset )
    {
        msWriteByte(REG_133289, 0x01);
        msWriteByte(REG_133389, 0x01);

        // oc timing gen.
        msWrite2Byte(REG_133002, 0x0002);                               //hs start
        msWrite2Byte(REG_133004, 0x0004);                               //hs end
        msWrite2Byte(REG_133006, OCTG_HDE_START);                       //hfde start
        msWrite2Byte(REG_133008, OCTG_HDE_START + u16OcOsdWidth -1);    //hfde end
        msWrite2Byte(REG_13300A, u16Htt);                               //htotal
        msWrite2Byte(REG_13300E, u16Vtt);                               //vtotal
        msWrite2Byte(REG_133014, OCTG_VDE_START );                      //vfde start
        msWrite2Byte(REG_133016, OCTG_VDE_START + u16OcOsdHeight -1);   //vfde end
        msWrite2Byte(REG_13301C, 0x0002);                               //vs start
        msWrite2Byte(REG_13301E, 0x0004);                               //vs end
    }
    else
    {
        WAIT_V_OUTPUT_BLANKING_START();
        msWriteByteMask(REG_13314A, 0x01, 0x01);
        msWriteByteMask(REG_133146, 0x01, 0x07);
        msWriteByteMask(REG_13314A, 0x00, 0x01);
    }

    // osdc
    if( bRSide && bLSide )
        msWriteByteMask(REG_13315A, _BIT0, _BIT0);
    else
        msWriteByteMask(REG_13315A, 0, _BIT0);

    //osdc
    msWrite2Byte(REG_13314C, u16LLast);     //pos_last_l
    msWrite2Byte(REG_133150, u16LMid);      //pos_mid_l
    msWrite2Byte(REG_133154, u16RStart);    //pos_st_r
    msWrite2Byte(REG_133156, u16RMid);      //pos_mid_r
    msWrite2Byte(REG_133158, u16RLast);     //pos_last_r

    if( !bOcGenReset )
    {
        ForceDelay1ms((3000/OSDC_VFREQ));
        WAIT_V_OUTPUT_BLANKING_START();
        msWriteByteMask(REG_133146, 0x00, 0x07);
        msWriteByteMask(REG_133248, 0x01, 0x07);
        msWriteByteMask(REG_133348, 0x01, 0x07);
    }

    //osdd
    if( bRSide & !bLSide )
        msWrite4Byte(REG_133328, u32OcBaseaddrL);
    else
        msWrite4Byte(REG_133328, u32OcBaseaddrR);

    msWrite2Byte(REG_13324C, u16LSize);         // L hsiz
    msWrite2Byte(REG_133250, u16LSize/2);       // L mid h size
    msWrite2Byte(REG_13334C, u16RSize);         // R h size
    msWrite2Byte(REG_133350, u16RSize/2);       // R mid h size

    // blending size L
    if( bLSide )
    {
        msWrite2Byte(REG_133280, 0x0001 + u16LHPos);            //h start
        msWrite2Byte(REG_133282, u16LSize + u16LHPos);          //h end
        msWrite2Byte(REG_133284, 0x0001 + u16LVPos);            //v start
        msWrite2Byte(REG_133286, u16OcOsdHeight + u16LVPos);    //v end
    }
    else
    {
        msWrite2Byte(REG_133280, 0x0000);                       //h start
        msWrite2Byte(REG_133282, 0x0000);                       //h end
        msWrite2Byte(REG_133284, 0x0000);                       //v start
        msWrite2Byte(REG_133286, 0x0000);                       //v end
    }
    msWrite2Byte(REG_133290, 0x4002);

    // blending size R
    if( bRSide )
    {
        msWrite2Byte(REG_133380, 0x0001 + u16RHPos);            //h start
        msWrite2Byte(REG_133382, u16RSize + u16RHPos);          //h end
        msWrite2Byte(REG_133384, 0x0001 + u16RVPos);            //v start
        msWrite2Byte(REG_133386, u16OcOsdHeight + u16RVPos);    //v end
        msWrite2Byte(REG_133390, 0x4002);
    }
    else
    {
        msWrite2Byte(REG_133380, 0x0000);                       //h start
        msWrite2Byte(REG_133382, 0x0000);                       //h end
        msWrite2Byte(REG_133384, 0x0000);                       //v start
        msWrite2Byte(REG_133386, 0x0000);                       //v end
    }

    if( bOcGenReset )
    {
        bOcGenReset = FALSE;

        msWriteByteMask(REG_133042, 0x01, 0x01);
        msWriteByteMask(REG_13314A, 0x01, 0x01);
        msWriteByteMask(REG_13324A, 0x0C, 0x0C);
        msWriteByteMask(REG_13334A, 0x0C, 0x0C);
        msWriteByteMask(REG_133042, 0x00, 0x01);
        msWriteByteMask(REG_13314A, 0x00, 0x01);
        msWriteByteMask(REG_13324A, 0x00, 0x0C);
        msWriteByteMask(REG_13334A, 0x00, 0x0C);

        ForceDelay1ms((4000/OSDC_VFREQ));

        msWriteByte(REG_133289, 0x00);
        msWriteByte(REG_133389, 0x00);
    }
    else
    {
        ForceDelay1ms((1000/OSDC_VFREQ));
        WAIT_V_OUTPUT_BLANKING_START();
        msWriteByteMask(REG_133248, 0x00, 0x07);
        msWriteByteMask(REG_133348, 0x00, 0x07);
    }
}
// End Small Canvas
////////////////////////////////////////////////////////////////////////

void msDrvOSDBlend_WindowConfig(WORD u16Blend_x, WORD u16Blend_y, sOSDC_SIZE sOSDCWin, sOSDC_SIZE sOSDDWin)
{
    WORD u16osdc_sizeL,u16osdc_sizeR, u16blend_HsizeL, u16blend_HsizeR;
    BYTE u8blend_guardband = 0;
    BOOL bHSP_EN, bVSP_EN;
    DWORD u32factor, u32fac_init;
    BYTE bLSide, bRSide;
    WORD u16Htt, u16Vtt;

    bHSP_EN = FALSE;
    bVSP_EN = FALSE;
    sOsdblendConfig.u8osdc_guardband = 0;
    sOsdblendConfig.u16osdc_width = sOSDCWin.u16width;
    sOsdblendConfig.u16osdc_hiegh = sOSDCWin.u16hiegh;
    sOsdblendConfig.u16blend_width = sOSDDWin.u16width;
    sOsdblendConfig.u16blend_hiegh = sOSDDWin.u16hiegh;

    if(sOsdblendConfig.u16osdc_width > PANEL_WIDTH )
        sOsdblendConfig.u16osdc_width = PANEL_WIDTH;

    if(sOsdblendConfig.u16osdc_hiegh> PANEL_HEIGHT )
        sOsdblendConfig.u16osdc_hiegh = PANEL_HEIGHT;


    if(sOsdblendConfig.u16blend_width > sOsdblendConfig.u16osdc_width)
        bHSP_EN = TRUE;

    if(sOsdblendConfig.u16blend_hiegh > sOsdblendConfig.u16osdc_hiegh)
        bVSP_EN = TRUE;


    if(sOSDCWin.u16width > 1920)
    {
        msWrite2Byte(REG_133222, 0x1030);  // need check??
        msWrite2Byte(REG_133322, 0x1030);  // need check??
    }
    else
    {
        if(sOSDDWin.u16width > 1920)
        {
            msWrite2Byte(REG_133222, 0x1020);  // need check??
            msWrite2Byte(REG_133322, 0x1020);  // need check??
        }
        else
        {
            msWrite2Byte(REG_133222, 0x1012);  // need check??
            msWrite2Byte(REG_133322, 0x1012);  // need check??
        }
    }

    bLSide = TRUE;
    bRSide = TRUE;
    if(u16Blend_x  > (PANEL_WIDTH/2))
    {
        u16blend_HsizeL = 0;
        u16blend_HsizeR = sOsdblendConfig.u16blend_width;
        bLSide = FALSE;
    }
    else if((u16Blend_x + sOsdblendConfig.u16osdc_width) < (PANEL_WIDTH/2))
    {
        u16blend_HsizeR = 0;
        u16blend_HsizeL = sOsdblendConfig.u16blend_width;
        bRSide = FALSE;
    }
    else
    {
        u16blend_HsizeR = u16Blend_x + sOsdblendConfig.u16blend_width - (PANEL_WIDTH/2);
        u16blend_HsizeL = sOsdblendConfig.u16blend_width - u16blend_HsizeR;
    }

    #if MST9U_ASIC_1P
    bLSide = TRUE;
    bRSide = FALSE;
    u16blend_HsizeR = 0;
    u16blend_HsizeL = PANEL_WIDTH;
    #endif

    DRVOSDBLEND_PRINT("\r\n u16blend_HsizeL =%d", u16blend_HsizeL);
    DRVOSDBLEND_PRINT("\r\n u16blend_HsizeR =%d\r\n", u16blend_HsizeR);
    if(bHSP_EN)
    {
        #if MST9U_ASIC_1P
        sOsdblendConfig.u8osdc_guardband = 0;
        #else
        sOsdblendConfig.u8osdc_guardband = OSDC_GB;
        #endif

        u16osdc_sizeL = ((DWORD)u16blend_HsizeL * sOsdblendConfig.u16osdc_width) / sOsdblendConfig.u16blend_width;
        u8blend_guardband = ((DWORD)sOsdblendConfig.u8osdc_guardband * sOsdblendConfig.u16blend_width) / sOsdblendConfig.u16osdc_width;

        u32factor = ((DWORD)sOsdblendConfig.u16osdc_width *1048576)/sOsdblendConfig.u16blend_width;
        u32fac_init = (((DWORD)u16blend_HsizeL - u8blend_guardband)* u32factor) & 0xFFFFF;

        u16osdc_sizeR = sOsdblendConfig.u16osdc_width - u16osdc_sizeL;

        msWrite2Byte(REG_1332CE, (WORD)(u32factor>>8));
        msWriteByte(REG_1332CD, (BYTE)u32factor);
        msWrite2Byte(REG_1333CE, (WORD)(u32factor>>8));
        msWriteByte(REG_1333CD, (BYTE)u32factor);

        msWrite3ByteMask(REG_1333CA, u32fac_init, 0xFFFFF);

        msWriteBit(REG_1332D5, FALSE, _BIT1);
        msWriteBit(REG_1333D5, FALSE, _BIT1);

        msWriteBit(REG_133292 , TRUE, _BIT0);
        msWriteBit(REG_133392 , TRUE, _BIT0);

        msWriteByteMask(REG_133236 , 0x80, 0xC0); //[7:6] reg_ocm_pre_read_last_
        msWriteByteMask(REG_133336 , 0x80, 0xC0); //[7:6] reg_ocm_pre_read_last_

    }
    else
    {
        sOsdblendConfig.u8osdc_guardband = 0;
        u16blend_HsizeL = ALIGN_4(u16blend_HsizeL);
        u16blend_HsizeR = ALIGN_4(u16blend_HsizeR);
        u16osdc_sizeL = u16blend_HsizeL;
        u16osdc_sizeR = u16blend_HsizeR;

        msWriteBit(REG_1332D5, TRUE, _BIT1);
        msWriteBit(REG_1333D5, TRUE, _BIT1);

        msWriteBit(REG_133292 , FALSE, _BIT0);
        msWriteBit(REG_133392 , FALSE, _BIT0);

        msWriteByteMask(REG_133392 , 0x00, 0x01);

        msWriteByteMask(REG_133236 , 0xC0, 0xC0); //[7:6] reg_ocm_pre_read_last_
        msWriteByteMask(REG_133336 , 0xC0, 0xC0); //[7:6] reg_ocm_pre_read_last_
    }

    msWrite2Byte(REG_1332D0, u16osdc_sizeL + sOsdblendConfig.u8osdc_guardband);
    msWrite2Byte(REG_1332D2, u16blend_HsizeL + u8blend_guardband);

    msWrite2Byte(REG_1333D0, u16osdc_sizeR + sOsdblendConfig.u8osdc_guardband);
    msWrite2Byte(REG_1333D2, u16blend_HsizeR + u8blend_guardband);

    DRVOSDBLEND_PRINT("\r\n u16osdc_sizeL =%d", u16osdc_sizeL);
    DRVOSDBLEND_PRINT("\r\n u16osdc_sizeR =%d\r\n", u16osdc_sizeR);
    //u32FrameSizeL = ALIGN_32((((DWORD)(u16osdc_sizeL+sOsdblendConfig.u8osdc_guardband)*sOSDCWin.u16hiegh*6)/10)* 4)>>5;
    //u32FrameSizeR = ALIGN_32((((DWORD)(u16osdc_sizeR+sOsdblendConfig.u8osdc_guardband)*sOSDCWin.u16hiegh*6)/10)* 4)>>5;

    if(bVSP_EN)
    {
        u32factor = ((DWORD)sOsdblendConfig.u16osdc_hiegh *1048576)/sOsdblendConfig.u16blend_hiegh;

        msWrite2Byte(REG_1332C4, (WORD)(u32factor>>8));
        msWriteByte(REG_1332C3, (BYTE)u32factor);

        msWrite2Byte(REG_1333C4, (WORD)(u32factor>>8));
        msWriteByte(REG_1333C3, (BYTE)u32factor);

    #if OSDD_SCALER_BYPASS
        msWriteBit(REG_1332D5, TRUE, _BIT2);
        msWriteBit(REG_1333D5, TRUE, _BIT2);
        msWriteByteMask(REG_133276 , 0x01, 0x0F);
        msWriteByteMask(REG_133376 , 0x01, 0x0F);
        msWriteBit(REG_133292 , FALSE, _BIT1);
        msWriteBit(REG_133392 , FALSE, _BIT1);
    #else
        msWriteBit(REG_1332D5, FALSE, _BIT2);
        msWriteBit(REG_1333D5, FALSE, _BIT2);
        msWriteByteMask(REG_133276 , 0x00, 0x0F);
        msWriteByteMask(REG_133376 , 0x00, 0x0F);
        msWriteBit(REG_133292 , TRUE, _BIT1);
        msWriteBit(REG_133392 , TRUE, _BIT1);
    #endif

    }
    else
    {
        msWriteBit(REG_1332D5, TRUE, _BIT2);
        msWriteBit(REG_1333D5, TRUE, _BIT2);

        msWriteBit(REG_133292 , FALSE, _BIT1);
        msWriteBit(REG_133392 , FALSE, _BIT1);
    }

    msWrite2Byte(REG_1332C6, sOsdblendConfig.u16osdc_hiegh);
    msWrite2Byte(REG_1332C8, sOsdblendConfig.u16blend_hiegh);
    msWrite2Byte(REG_1333C6, sOsdblendConfig.u16osdc_hiegh);
    msWrite2Byte(REG_1333C8, sOsdblendConfig.u16blend_hiegh);

    if(bHSP_EN)
    {
        msDrvOSDBlend_CalcHtt(sOsdblendConfig.u16osdc_width, sOsdblendConfig.u16osdc_hiegh, OSDC_VFREQ, &u16Htt, &u16Vtt);

        // oc timing gen.
        msWrite2Byte(REG_133002, 0x0002);     //hs start
        msWrite2Byte(REG_133004, 0x0004);     //hs end
        msWrite2Byte(REG_133006, OCTG_HDE_START);     //hfde start
        msWrite2Byte(REG_133008, OCTG_HDE_START + sOsdblendConfig.u16osdc_width -1); //hfde end
        msWrite2Byte(REG_13300A, u16Htt); //htotal
        msWrite2Byte(REG_13300E, u16Vtt); //vtotal
        msWrite2Byte(REG_133014, OCTG_VDE_START ); //vfde start
        msWrite2Byte(REG_133016, OCTG_VDE_START + sOsdblendConfig.u16osdc_hiegh -1); //vfde end
        msWrite2Byte(REG_13301C, 0x0002);     //vs start
        msWrite2Byte(REG_13301E, 0x0004);     //vs end
    }
    else
    {
        msDrvOSDBlend_CalcHtt(PANEL_WIDTH, PANEL_HEIGHT, OSDC_VFREQ, &u16Htt, &u16Vtt);

        // oc timing gen.
        msWrite2Byte(REG_133002, 0x0002);     //hs start
        msWrite2Byte(REG_133004, 0x0004);     //hs end
        msWrite2Byte(REG_133006, OCTG_HDE_START);     //hfde start
        msWrite2Byte(REG_133008, OCTG_HDE_START + sOsdblendConfig.u16osdc_width -1); //hfde end
        msWrite2Byte(REG_13300A, u16Htt); //htotal
        msWrite2Byte(REG_13300E, u16Vtt); //vtotal
        msWrite2Byte(REG_133014, OCTG_VDE_START ); //vfde start
        msWrite2Byte(REG_133016, OCTG_VDE_START + sOsdblendConfig.u16osdc_hiegh -1); //vfde end
        msWrite2Byte(REG_13301C, 0x0002);     //vs start
        msWrite2Byte(REG_13301E, 0x0004);     //vs end
    }

    if( bRSide & !bLSide )
        msWrite4Byte(REG_133328, u32OcBaseaddrL);
    else
        msWrite4Byte(REG_133328, u32OcBaseaddrR);

    if( bRSide && bLSide )
        msWriteByteMask(REG_13315A, _BIT0, _BIT0);
    else
        msWriteByteMask(REG_13315A, 0, _BIT0);

    //osdc
    if(!bLSide)
    {
        msWrite2Byte(REG_13314C, u16osdc_sizeR + sOsdblendConfig.u8osdc_guardband -1);          //pos_last_l
        msWrite2Byte(REG_133150, ALIGN_4(u16osdc_sizeR/2 + sOsdblendConfig.u8osdc_guardband)-1);          //pos_mid_l
        msWrite2Byte(REG_133154, u16osdc_sizeR - sOsdblendConfig.u8osdc_guardband);          //pos_st_r
        msWrite2Byte(REG_133156, ALIGN_4(u16osdc_sizeR + u16osdc_sizeL/2)-1 );          //pos_mid_r
        msWrite2Byte(REG_133158, sOsdblendConfig.u16osdc_width - 1);          //pos_last_r
    }
    else
    {
        msWrite2Byte(REG_13314C, u16osdc_sizeL + sOsdblendConfig.u8osdc_guardband -1);          //pos_last_l
        msWrite2Byte(REG_133150, ALIGN_4((u16osdc_sizeL + sOsdblendConfig.u8osdc_guardband)/2)-1);          //pos_mid_l
        msWrite2Byte(REG_133154, u16osdc_sizeL - sOsdblendConfig.u8osdc_guardband);          //pos_st_r
        msWrite2Byte(REG_133156, ALIGN_4(u16osdc_sizeL - sOsdblendConfig.u8osdc_guardband + ((u16osdc_sizeR + sOsdblendConfig.u8osdc_guardband)/2)) -1);          //pos_mid_r
        msWrite2Byte(REG_133158, sOsdblendConfig.u16osdc_width - 1);          //pos_last_r
    }

    //osdd
    msWrite2Byte(REG_13324C, u16osdc_sizeL + sOsdblendConfig.u8osdc_guardband); // hsiz
    msWrite2Byte(REG_133250, ALIGN_4((u16osdc_sizeL + sOsdblendConfig.u8osdc_guardband)/2)); //mid h size
    msWrite2Byte(REG_13334C, u16osdc_sizeR + sOsdblendConfig.u8osdc_guardband); //h size
    msWrite2Byte(REG_133350, ALIGN_4((u16osdc_sizeR + sOsdblendConfig.u8osdc_guardband)/2)); //mid h size


    // blending size L
    if(bLSide)
    {
        msWrite2Byte(REG_133280, 0x0001+u16Blend_x);           //h start
        msWrite2Byte(REG_133282, u16blend_HsizeL+u16Blend_x+u8blend_guardband);    // h end
        msWrite2Byte(REG_133284, 0x0001+ u16Blend_y);           //v start
        msWrite2Byte(REG_133286, sOsdblendConfig.u16blend_hiegh + u16Blend_y);     //v end
    }
    else
    {
        msWrite2Byte(REG_133280, 0);    //h start
        msWrite2Byte(REG_133282, 0);    // h end
        msWrite2Byte(REG_133284, 0);    //v start
        msWrite2Byte(REG_133286, 0);    //v end
    }
    msWrite2Byte(REG_133290, 0x4002);

    // blending size R
    if(bRSide)
    {
        u8blend_guardband += (!bHSP_EN)?OD_GUARD_BAND:0;
        msWrite2Byte(REG_133380, 0x0001+u8blend_guardband - ((bHSP_EN)?OD_GUARD_BAND:0));           //h start
        msWrite2Byte(REG_133382, u16blend_HsizeR+u8blend_guardband);    // h end
        msWrite2Byte(REG_133384, 0x0001 + u16Blend_y);           //v start
        msWrite2Byte(REG_133386, sOsdblendConfig.u16blend_hiegh + u16Blend_y);     //v end
    }
    else
    {
        msWrite2Byte(REG_133380, 0);    //h start
        msWrite2Byte(REG_133382, 0);    // h end
        msWrite2Byte(REG_133384, 0);    //v start
        msWrite2Byte(REG_133386, 0);    //v end
    }
    msWrite2Byte(REG_133390, 0x4002);

}

static void msDrvOSDBlend_SetClock(eOSDC_CLK clock)
{
    msWriteByteMask(REG_120F17 , (BYTE)(clock)<<2, 0x0F);

    switch(clock)
    {
        case eOSDC_CLK_320M:
            u32OcClock = 320000000;
            break;

        case eOSDC_CLK_123M:
            u32OcClock = 123000000;
            break;

        case eOSDC_CLK_172M:
            u32OcClock = 172000000;
            break;

        case eOSDC_CLK_86M:
            u32OcClock = 86000000;
            break;

        default:
            u32OcClock = 0;
            break;
    }
}

void msDrvOSDBlend_Init(BYTE source)
{
    DWORD u32FrameSize;
    sOSDC_SIZE sOSDCWin, sOSDDWin;

    sOsdblendConfig.bIsSourceGOP = source; // SOSD or GOP
    sOsdblendConfig.bScaling = FALSE;
    sOsdblendConfig.eOSDC_format = eOSDC_ARGB8888;
#if (!ZUI)
    sOsdblendConfig.eOSDC_clk = eOSDC_CLK_86M;
#else
    sOsdblendConfig.eOSDC_clk = eOSDC_CLK_172M;
#endif
    u32FrameSize = (u32OcBaseaddrR - u32OcBaseaddrL)/2;

#if ENABLE_OSDC_OSDD
#if _LIMIT_OSDSIZE_
    /*if(sOsdblendConfig.bIsSourceGOP)
    {
        sOSDCWin.u16width = LIMIT_OSDC_HSIZE;
        sOSDCWin.u16hiegh = LIMIT_OSDC_VSIZE;
        sOSDDWin.u16width = sOSDCWin.u16width;
        sOSDDWin.u16hiegh = sOSDCWin.u16hiegh;
    }
    else
    */
    {
        sOSDCWin.u16width = LIMIT_OSDC_HSIZE;
        sOSDCWin.u16hiegh = LIMIT_OSDC_VSIZE;
        sOSDDWin.u16width = PANEL_WIDTH;
        sOSDDWin.u16hiegh = PANEL_HEIGHT;
    }
#else
    sOSDCWin.u16width = PANEL_WIDTH;
    sOSDCWin.u16hiegh = PANEL_HEIGHT;
    sOSDDWin.u16width = PANEL_WIDTH;
    sOSDDWin.u16hiegh = PANEL_HEIGHT;
#endif
#endif //#if ENABLE_OSDC_OSDD

    sOsdblendConfig.u8osdc_guardband = 0;

    //osdc clock gen.
    msDrvOSDBlend_SetClock(sOsdblendConfig.eOSDC_clk);

    if(sOsdblendConfig.bIsSourceGOP)
    {
        msWrite2Byte(REG_133044, 0x0008);     //config mix
        msWriteByteMask(REG_120F23, 0x0C, 0x0F);
        msWriteByteMask(REG_120F24, 0x0C, 0x0F);
        msWriteByteMask(REG_120F25, 0x0C, 0x0F);
    }
    else
    {
        // sosd
        msWriteByteMask(REG_101C03 , _BIT4|_BIT0, _BIT4|_BIT0);
        msWriteByteMask(REG_120F11 , 0x0C, 0x0F);
        msWrite2Byte(REG_133044, 0x0108);     //config mix
     }

    //osdc L
    msWrite2Byte(REG_133124, 0x0020);          //
    msWrite2Byte(REG_133126, 0x1428);          //
    msWrite4Byte(REG_13312C, u32OcBaseaddrL);      // waddr
    msWrite2Byte(REG_133134, 0x03ff);          //line fetch
    msWrite2Byte(REG_133136, 0x0009);          //

    //osdc R
    msWrite2Byte(REG_1331A4, 0x0020);          //
    msWrite2Byte(REG_1331A6, 0x1428);          // need confirm
    msWrite4Byte(REG_1331AC, u32OcBaseaddrR);      // waddr
    msWrite2Byte(REG_1331B4, 0x03ff);          //line fetch
    msWrite2Byte(REG_1331B6, 0x0009);          //

    //osdc
    msWrite2Byte(REG_133160, 0x08f1);          //
    msWrite2Byte(REG_133146, 0x0000);          //
    msWrite2Byte(REG_133148, 0x0040);          //
    msWrite2Byte(REG_13315A, 0x0001);          //
    msWrite2Byte(REG_13316A, 0x8540);          //
    msWrite2Byte(REG_13316E, 0x0000);          //
    msWrite2Byte(REG_133172, 0x0000);          //
    msWriteByteMask(REG_1331E0, (BYTE)sOsdblendConfig.eOSDC_format, 0x0F);  // [3:0] mode:ARGB8888

    //OSDD_L
    //msWrite2Byte(REG_133220, 0x0001);
    msWrite4Byte(REG_133228, u32OcBaseaddrL);
    msWrite2Byte(REG_133234, 0x03ff);           //line fetch
    msWrite2Byte(REG_133248, 0x0048);           //[4:3] rw_diff

    msWrite2Byte(REG_133252, 0x0003);
    msWrite2Byte(REG_1332C0, 0x0000);
    msWrite2Byte(REG_1332C2, 0x0000);
    msWrite2Byte(REG_1332C4, 0x1000);
    msWrite2Byte(REG_1332C6, 0x00f0);
    msWrite2Byte(REG_1332C8, 0x00f0);
    msWrite2Byte(REG_1332CA, 0x0000);
    msWrite2Byte(REG_1332CC, 0x0000);
    msWrite2Byte(REG_1332CE, 0x1000);
    msWrite2Byte(REG_1332D0, 0x0182);
    msWrite2Byte(REG_1332D2, 0x0182);

    //OSDD_R
    //msWrite2Byte(REG_133320, 0x0001);
    msWrite4Byte(REG_133328, u32OcBaseaddrR);

    msWrite2Byte(REG_133334, 0x03ff); //line fetch
    msWrite2Byte(REG_133348, 0x0048); //[4:3] rw_diff
    msWrite2Byte(REG_133352, 0x0003);
    msWrite2Byte(REG_1333C0, 0x0000);
    msWrite2Byte(REG_1333C2, 0x0000);
    msWrite2Byte(REG_1333C4, 0x1000);
    msWrite2Byte(REG_1333C6, 0x00f0);
    msWrite2Byte(REG_1333C8, 0x00f0);
    msWrite2Byte(REG_1333CA, 0x0000);
    msWrite2Byte(REG_1333CC, 0x0000);
    msWrite2Byte(REG_1333CE, 0x1000);
    msWrite2Byte(REG_1333D0, 0x0182);
    msWrite2Byte(REG_1333D2, 0x0182);

    #if OSDD_SCALER_BYPASS
    msWrite2Byte(REG_1332D6, 0x0000);//h-coef
    msWrite2Byte(REG_1332D8, 0x0000);
    msWrite2Byte(REG_1332DA, 0x0000);
    msWrite2Byte(REG_1332DC, 0x0000);

    msWrite2Byte(REG_1333D6, 0x0000);//h-coef
    msWrite2Byte(REG_1333D8, 0x0000);
    msWrite2Byte(REG_1333DA, 0x0000);
    msWrite2Byte(REG_1333DC, 0x0000);
    #else
    msWrite2Byte(REG_1332D6, 0x620f);//h-coef
    msWrite2Byte(REG_1332D8, 0x5d00);
    msWrite2Byte(REG_1332DA, 0x7a29);
    msWrite2Byte(REG_1332DC, 0x467a);

    msWrite2Byte(REG_1333D6, 0x620f);//h-coef
    msWrite2Byte(REG_1333D8, 0x5d00);
    msWrite2Byte(REG_1333DA, 0x7a29);
    msWrite2Byte(REG_1333DC, 0x467a);
    #endif

    msWrite4Byte(REG_133130, u32FrameSize);      // frame fetch
    msWrite4Byte(REG_1331B0, u32FrameSize);      // frame fetch
    msWrite4Byte(REG_133230, u32FrameSize);      // frame fetch
    msWrite4Byte(REG_133330, u32FrameSize);      // frame fetch

    if (sOsdblendConfig.bIsSourceGOP == TRUE)
    {
        msWriteByteMask(REG_120F23 , 0x0C, 0x0F); // gopg0_clk sel osdc_p
        msWriteByteMask(REG_120F24 , 0x0C, 0x0F); // gopg1_clk sel osdc_p
        msWriteByteMask(REG_120F25 , 0x0C, 0x0F); // gopg2_clk sel osdc_p
        msWriteByteMask(REG_120F26 , 0x0C, 0x0F); // gopg3_clk sel osdc_p

        msWriteByteMask(REG_GOP_FF , 0x02, 0x02); // force write enable

        msWriteByteMask(REG_GOP_FE , 0x00, 0x0F); // GOP0
        msWriteByteMask(REG_GOP_01 , 0x80, 0x80); // alpha out inverse
        msWriteByteMask(REG_GOP_02 , 0x06, 0x07); // reg_gwin_dest to OC
        msWrite2Byte(REG_GOP_1E, 0x0000);         // reg_hs_pipe_q=0
        msWriteByteMask(REG_GOP_FE , 0x03, 0x0F); // GOP1
        msWriteByteMask(REG_GOP_01 , 0x80, 0x80); // alpha out inverse
        msWriteByteMask(REG_GOP_02 , 0x06, 0x07); // reg_gwin_dest to OC
        msWrite2Byte(REG_GOP_1E, 0x0000);         // reg_hs_pipe_q=0
        msWriteByteMask(REG_GOP_FE , 0x06, 0x0F); // GOP2
        msWriteByteMask(REG_GOP_01 , 0x80, 0x80); // alpha out inverse
        msWriteByteMask(REG_GOP_02 , 0x06, 0x07); // reg_gwin_dest to OC
        msWrite2Byte(REG_GOP_1E, 0x0000);         // reg_hs_pipe_q=0
        msWriteByteMask(REG_GOP_FE , 0x09, 0x0F); // GOP3
        msWriteByteMask(REG_GOP_01 , 0x80, 0x80); // alpha out inverse
        msWriteByteMask(REG_GOP_02 , 0x06, 0x07); // reg_gwin_dest to OC
        msWrite2Byte(REG_GOP_1E, 0x0000);         // reg_hs_pipe_q=0

        msWriteByteMask(REG_GOP_EF , 0xF0, 0xF0); // mux to OC enable
        msWriteByteMask(REG_GOP_FF , 0x00, 0x02); // force write disable
    }

    if (sOsdblendConfig.bIsSourceGOP)
        msDrvOSDBlend_WindowConfig(( PANEL_WIDTH- sOSDDWin.u16width)/2, (PANEL_HEIGHT - sOSDDWin.u16hiegh)/2 , sOSDCWin , sOSDDWin);
    else
        msDrvOSDBlend_WindowConfig(0, 0, sOSDCWin , sOSDDWin);
    msDrvOSDBlend_Enable(FALSE);
}

void msDrvOSDBlend_Enable(BOOL bEnable)
{
    DWORD u32SCRegBase = SCRegBase(SCL0_MAIN);
    BYTE u8Scaler;
    //printf("msDrvOSDBlend_Enable(%d)", bEnable);

    //enable osdc
    msWriteBit(REG_133124, bEnable, _BIT0);
    msWriteBit(REG_1331A4, bEnable, _BIT0);

    //enable osdd
    msWriteBit(REG_133220, bEnable, _BIT4 | _BIT0);
    msWriteBit(REG_133320, bEnable, _BIT4 | _BIT0);

    if(bEnable)
    {
        ForceDelay1ms((3000/OSDC_VFREQ));
        MENU_LOAD_START();
        for(u8Scaler= SCL0_MAIN; u8Scaler < SC_MAX_NUMS ; u8Scaler+=2)
        {
            //blend source
            u32SCRegBase = SCRegBase(u8Scaler);
            if(sOsdblendConfig.bIsSourceGOP)
                DB_WBMask(u32SCRegBase+SC00_0D, 0xF0, 0xF0);
            else
                DB_WBMask(u32SCRegBase+SC00_0D, 0x80, 0xF0);
            DB_WBMask(u32SCRegBase+SC00_81, 0x20, 0x30);
            DB_WBMask(u32SCRegBase+SC10_46, _BIT7, _BIT5|_BIT7);
        }
        MENU_LOAD_END();
    }
    else
    {
        for(u8Scaler= SCL0_MAIN; u8Scaler < SC_MAX_NUMS ; u8Scaler+=2)
        {
            //blend source
            u32SCRegBase = SCRegBase(u8Scaler);
            msWriteByteMask(u32SCRegBase+SC00_0D, 0x00, 0xF0);
            msWriteByteMask(u32SCRegBase+SC00_81, 0x00, 0x30);
            msWriteByteMask(u32SCRegBase+SC10_46, _BIT7|_BIT5, _BIT5|_BIT7);
        }
    }
}

void msDrvOSDBlend_FrcOsdb_Enable(BOOLEAN bEnable)
{
    if (bEnable == TRUE)
    {
        msWriteByteMask(REG_102F00, 0x40, 0xFF); // switch to bank 40
  	#if (CHIP_ID == MST9U2)
        if (E_GOP_OSD_RIGHT == 2)
        {
            msWriteByteMask(REG_102F42, 0x03, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop2 right)
        }
        else if (E_GOP_OSD_RIGHT == 1)
        {
            msWriteByteMask(REG_102F46, 0x03, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop1 right)
        }
        msWriteByteMask(REG_102F48, 0x03, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop0 left)
  	#elif (CHIP_ID == MST9U4)
		if (E_GOP_OSD_RIGHT == 1)
		{			
			msWriteByteMask(BANK_SC_S_R0, 0x40, 0xFF); 			// switch right scaler to bank 40
			msWriteByteMask(BANK_SC_S_R0+0x40, 0x01, 0xFF); 	// reg_osdb_en (gop1 right side blend layer0)
		}
		msWriteByteMask(REG_102F40, 0x01, 0xFF); // reg_osdb_en (gop0 left side blend layer0)
	#endif
        /* move setting to GOP driver GOP_InitHSPDByGOP
        msWriteByteMask(REG_101FFF , 0x02, 0x02); // force write enable
        msWriteByteMask(REG_101FFE , 0x00, 0x0F); // GOP0
        msWrite2Byte(REG_101F1E, 0x0104);         // reg_hs_pipe_q
        msWriteByteMask(REG_101FFE , 0x03, 0x0F); // GOP1
        msWrite2Byte(REG_101F1E, 0x0101);         // reg_hs_pipe_q
        msWriteByteMask(REG_101FFE , 0x06, 0x0F); // GOP2
        msWrite2Byte(REG_101F1E, 0x00FE);         // reg_hs_pipe_q
        msWriteByteMask(REG_101FFF , 0x00, 0x02); // force write disable
        */
    }
    else
    {
        msWriteByteMask(REG_102F00, 0x40, 0xFF); // switch to bank 40
        msWriteByteMask(REG_102F40, 0x00, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop2 left)
        msWriteByteMask(REG_102F42, 0x00, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop2 right)
        msWriteByteMask(REG_102F44, 0x00, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop1 left)
        msWriteByteMask(REG_102F46, 0x00, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop1 right)
        msWriteByteMask(REG_102F48, 0x00, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop0 left)
        msWriteByteMask(REG_102F4A, 0x00, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop0 right)
        msWriteByteMask(REG_102F4C, 0x00, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop3 left)
        msWriteByteMask(REG_102F4E, 0x00, 0xFF); // reg_osdb_en & reg_osdb_osd_img_swap (gop3 right)
	#if (CHIP_ID == MST9U4)
		msWriteByteMask(BANK_SC_S_R0, 0x40, 0xFF);			// switch right scaler to bank 40
		msWriteByteMask(BANK_SC_S_R0+0x40, 0x00, 0xFF); 	// reg_osdb_en (gop1 right side blend layer0)
	#endif
    }

}

void msDrvOSDBlendFBSwitch(BOOL bEnable)
{
    static BOOL bSwitch = 0;
    static BOOL bOSDForceEnable = 0;
    BYTE u8Delay1ms;

    u8Delay1ms = 1000/OSDC_VFREQ+1;

    if(bEnable)
    {
        Delay1ms(u8Delay1ms*2);  // wait for 2 frame
        if(bSwitch)
        {
            bSwitch = 0;
            msWriteByteMask(REG_133248,BIT0|BIT1,BIT0|BIT1|BIT2);
            msWriteByteMask(REG_133348,BIT0|BIT1,BIT0|BIT1|BIT2);
            WAIT_V_OUTPUT_BLANKING_START();
            msWriteByteMask(REG_133146,BIT0,BIT0|BIT1|BIT2);
        }
        else
        {
            bSwitch = 1;
            msWriteByteMask(REG_133248,BIT0,BIT0|BIT1|BIT2);
            msWriteByteMask(REG_133348,BIT0,BIT0|BIT1|BIT2);
            WAIT_V_OUTPUT_BLANKING_START();
            msWriteByteMask(REG_133146,BIT0|BIT1,BIT0|BIT1|BIT2);
        }
        //Delay1ms(u8Delay1ms);
    }
    else
    {
        if(bOSDForceEnable)
        {
            Delay1ms(u8Delay1ms);
            msWriteByteMask(REG_133146,0x00,BIT0);
            msWriteByteMask(REG_133248,0x00,BIT0);
            msWriteByteMask(REG_133348,0x00,BIT0);
        }
    }
    bOSDForceEnable = bEnable;
}
