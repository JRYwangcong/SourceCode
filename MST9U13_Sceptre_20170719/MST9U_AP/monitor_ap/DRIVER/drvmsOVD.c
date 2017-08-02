#define _MSOVD_D_C_

#include "Global.h"
#include "sysinfo2_MST9U.h"
#include "drvmsOVD.h"

#if ENABLE_MSTV_UART_DEBUG
    #define OD_DPUTSTR(str)    printMsg(str)
    #define OD_DPRINTF(str, x) printData(str, x)
#else
    #define OD_DPUTSTR(str)
    #define OD_DPRINTF(str, x)
#endif

//#define OD_LIB_VER        0x03
//code ==================================================================

//*************************************************************************************************
//Function:  msODMemoryTest()
//Description: For Memory test
//
//           ex:    printData(" msODMemoryTest = %d"  , msODMemoryTest());
//           if the result is 0,
//           if the result is 1,
//
//*************************************************************************************************

//*************************************************************************************************
//Function:  msOD_Reset()
//Description: Reset MIU bank
//caller:        Power_TurnOnPanel of power.c
//
//           It need to reset MIU bank before turn on panel.
//*************************************************************************************************


//*************************************************************************************************
//Function:  msOD_OnOff()
//Description: Enable/Disable OverDrive and setup OD strength
//          ex:
//          msOD_OnOff( _OD_Disable, 0x50 ); //disable OD
//          msOD_OnOff( _OD_Enable, 0x5F ); //enable OD and set strength as 0x5F
//          OD strength: 0x50 (weak)  <------------>  0x5F (strong)
//
//*************************************************************************************************
//*************************************************************************************************
//Function:  msLoadODT()
//Description: OD look-up table loader.
//
//           ex: msLoadODT(tOverDrive)
//
//           It provides two kinds of format (normal and compressed) for OD look-up table.
//           The look-up table and OD lib should be located in the same bank!!
//
//*************************************************************************************************
#if ENABLE_OD_MTB
#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
void msLoad_MultiODTable(BYTE index)
{
     WORD wCount,wXORKeyIdx;
     BYTE ucXORKey,OD_data,ucMultiTblIdx,ucSramIdx,ucTblData;
     DWORD dwXdataAddr=0;
     BYTE ucReg;
     BYTE ucOffset;

    ucReg = msReadByte(SCL0_16_20);
    msWriteBit(SCL0_16_20, 0, _BIT0); // msWriteByte(REG_3120, 0x00); // Disable OD before load code
    while(msReadByte(SCL0_16_20)&_BIT0);
    msWriteByte(SCL0_16_02, 0x00);    // load code start
    wCount=0;

    ucOffset = index*8;

    for(ucMultiTblIdx =0;ucMultiTblIdx < 8; ucMultiTblIdx++)
    {
        while(wCount<289)
        {
            for(ucSramIdx = 0;ucSramIdx < 4;ucSramIdx++)
            {
                if(ucSramIdx<3)
                    wXORKeyIdx =(9 + 10*ucSramIdx) + 272*(WORD)(ucSramIdx);
                else
                    wXORKeyIdx =(9 + 10*ucSramIdx) + 256*(WORD)(ucSramIdx-2) + 272*2;

                if( wCount == 9 && ucSramIdx == 0)
                    ucXORKey = 0;
                else if( wCount == 19 && ucSramIdx == 1)
                    ucXORKey = 0;
                else if( wCount == 29 && ucSramIdx == 2)
                    ucXORKey = 0;
                else if( wCount == 39 && ucSramIdx == 3)
                    ucXORKey = 0;
                else
                    ucXORKey =*(tOverDrive_AllTbl[ucMultiTblIdx+ucOffset]+wXORKeyIdx);

                if(ucSramIdx < 2 && wCount < 272)
                {
                    ucTblData = *(tOverDrive_AllTbl[ucMultiTblIdx+ucOffset]+(272*(WORD)ucSramIdx) + wCount ) ;
                    OD_data = ucXORKey ^ ucTblData;
                    msMemWriteByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx  ,OD_data);
                    //printf("2222 ucSramIdx==[%d] ,wCount==[%d],OD_data==[%x] ,ReadDRAM==[%x] \n",ucSramIdx,wCount,OD_data,msMemReadByte(0x0033DC000UL+dwXdataAddr+ ucSramIdx ));
                }
                else if(ucSramIdx >=2 && wCount < 256)
                {
                    ucTblData = *(tOverDrive_AllTbl[ucMultiTblIdx+ucOffset]+(256*(WORD)(ucSramIdx-2) + 272*2) + wCount ) ;
                    OD_data = ucXORKey ^ ucTblData;
                    msMemWriteByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx  ,OD_data);
                    //printf("3333 ucSramIdx==[%d] ,wCount==[%d],OD_data==[%x] ,ReadDRAM==[%x] \n",ucSramIdx,wCount,OD_data,msMemReadByte(0x0033DC000UL+dwXdataAddr+ ucSramIdx ));
                }
                else if (((ucSramIdx ==0x00||ucSramIdx ==0x01) && (wCount >= 272 && wCount<289)) ||((ucSramIdx ==0x02||ucSramIdx ==0x03) && (wCount >= 256 && wCount<289)))
                {
                    msMemWriteByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx  ,0x00);
                    //printf("4444 ucSramIdx==[%d] ,wCount==[%d],OD_data==[%x] ,ReadDRAM==[%x] \n",ucSramIdx,wCount,OD_data,msMemReadByte(0x0033DC000UL+dwXdataAddr+ ucSramIdx ));
                }
                msMemWriteByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ 0x08  ,0x01);//enable 1st od table
            }
            wCount++;
            dwXdataAddr += 0x10; //16 bytes alligment
        }
        wCount = 0;
    }
    msDrvSetAutoDownload(ADL_OD_TBL, ENABLE, ADL_MODE_ENABLE, (AUTOLOAD_OD_AVAILABLE), 289);
    msWriteByteMask(SCL0_29_80, _BIT5|_BIT0 ,_BIT5 |_BIT4|_BIT0);//[0]:Enable,[5:4]:Interval.0:32,1:64,2:128,3:256
    msWriteByte(SCL0_29_84, 0x07);    // 8 multi-table(Index)
    msWriteByteMask(SCL0_16_03, _BIT2|_BIT0, _BIT2|_BIT0);//od table auto load enable
    msWriteByte(SCL0_29_82, 91);    //#line start
    //[NOTE]:if you want to read OD table by TV tool , you need to disable AutoDownload first
    //ForceDelay1ms(10);
    //msWriteByteMask(SCL0_16_03, 0x00, _BIT2|_BIT0);//od table auto load enable
    msWriteByte(SCL0_16_20, ucReg);

}
#else //MST9U4
void msLoad_MultiODTable(BYTE index)
{
    BYTE ucMultiGainIdx;
    index=index;
    msWriteByteMask(SCL0_29_02, _BIT0 ,_BIT0);//[0]:reg_gain_table_int_en
    msWriteByteMask(SCL0_29_80, _BIT5|_BIT4 ,_BIT6|_BIT5|_BIT4);//[2]:region gain value 0,[6:4]:Interval.0:32,1:64,2:128,3:256 others:512

    for(ucMultiGainIdx =0;ucMultiGainIdx < 11; ucMultiGainIdx++)
    {
        msWriteByteMask((SCL0_29_20+ucMultiGainIdx), tOverDrive_MultiGain[ucMultiGainIdx] ,0xFF);//region gain value 0~10
        OVERDRIVE_PRINT(" (SCL0_29_20+ucMultiGainIdx)==[%x] ,tOverDrive_MultiGain[ucMultiGainIdx]==[%x] \n",(SCL0_29_20+ucMultiGainIdx),tOverDrive_MultiGain[ucMultiGainIdx]);
    }
}
#endif
#else
#if ENABLE_OD_AutoDownTBL
void msDrvOverDriveTable(BYTE *pODTbl)
{
     WORD wCount=0,wXORKeyIdx,wi,wj;
     BYTE ucXORKey,OD_data=0xFF,ucSramIdx,ucTblData;
     DWORD dwXdataAddr=0;
     BYTE ucReg;

    ucReg = msReadByte(SCL0_16_20);
    msWriteBit(SCL0_16_20, 0, _BIT0);
    while(msReadByte(SCL0_16_20)&_BIT0);
    msWriteByte(SCL0_16_02, 0x00);

    //clear Mem data
    for(wj=0; wj<289; wj++)
    {
        for(wi=0; wi<OD_BusLength; wi++)
        {
            msMemWriteByte(AUTOLOAD_OD_AVAILABLE + wi+wj*OD_BusLength, 0x00);
        }
    }
    //De-compress OD LUT and write to DRAM
    while(wCount<289)
    {
        for(ucSramIdx = 0;ucSramIdx < 4;ucSramIdx++)
        {
            if(ucSramIdx<3)
                wXORKeyIdx =(9 + 10*ucSramIdx) + 272*(WORD)(ucSramIdx);
            else
                wXORKeyIdx =(9 + 10*ucSramIdx) + 256*(WORD)(ucSramIdx-2) + 272*2;

            if( wCount == 9 && ucSramIdx == 0)
                ucXORKey = 0;
            else if( wCount == 19 && ucSramIdx == 1)
                ucXORKey = 0;
            else if( wCount == 29 && ucSramIdx == 2)
                ucXORKey = 0;
            else if( wCount == 39 && ucSramIdx == 3)
                ucXORKey = 0;
            else
                ucXORKey =*(pODTbl+wXORKeyIdx);

            if(ucSramIdx < 2 && wCount < 272)
            {
                ucTblData = *(pODTbl+(272*(WORD)ucSramIdx) + wCount ) ;
                OD_data = ucXORKey ^ ucTblData;
                msMemWriteByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx  ,OD_data);
                if(wCount<10)
                OVERDRIVE_PRINT(" ucSramIdx==[%d] ,wCount==[%d],OD_data==[%d] ,ReadDRAM==[%d] \n",ucSramIdx,wCount,OD_data,msMemReadByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx ));
            }
            else if(ucSramIdx >=2 && wCount < 256)
            {
                ucTblData = *(pODTbl+(256*(WORD)(ucSramIdx-2) + 272*2) + wCount ) ;
                OD_data = ucXORKey ^ ucTblData;
                msMemWriteByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx  ,OD_data);
                if(wCount<10)
                OVERDRIVE_PRINT(" ucSramIdx==[%d] ,wCount==[%d],OD_data==[%d] ,ReadDRAM==[%d] \n",ucSramIdx,wCount,OD_data,msMemReadByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx ));
            }
            else if (((ucSramIdx ==0x00||ucSramIdx ==0x01) && (wCount >= 272 && wCount<289)) ||((ucSramIdx ==0x02||ucSramIdx ==0x03) && (wCount >= 256 && wCount<289)))
            {
                msMemWriteByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx  ,0x00);
                if(wCount<10)
                OVERDRIVE_PRINT(" ucSramIdx==[%d] ,wCount==[%d],OD_data==[%d] ,ReadDRAM==[%d] \n",ucSramIdx,wCount,OD_data,msMemReadByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ ucSramIdx ));
            }
            msMemWriteByte(AUTOLOAD_OD_AVAILABLE+dwXdataAddr+ 0x08  ,0x01);//enable 1st od table
        }
        wCount++;
        dwXdataAddr += OD_BusLength; //16 or 32 bytes alligment
    }
    msDrvSetAutoDownload(ADL_OD_TBL, ENABLE, ADL_MODE_ENABLE, (AUTOLOAD_OD_AVAILABLE), 289);
    msWriteByteMask(SCL0_16_03, _BIT2|_BIT0, _BIT2|_BIT0);//od table auto load enable
    ForceDelay1ms(50);
    msWriteByteMask(SCL0_16_03, 0x00, _BIT2|_BIT0);//od table auto load enable
    msWriteByte(SCL0_16_20, ucReg);
}
#else
void msDrvOverDriveTable(BYTE *pODTbl)
{
    WORD wCount;
    BYTE ucTARGET;

    if(0 == pODTbl) return;
    OD_DPUTSTR("Load_OD_Table");

    msWriteBit(SCL0_16_20, FALSE, _BIT0); // Disable OD before load code
    msWriteByte(SCL0_16_02, 0x0E); // load code start

     // Uncompressed mode
    ucTARGET=*(pODTbl+9);// 10th
    for(wCount=0; wCount<289; wCount++)
    {
        if(wCount<272)
        msWriteByte(SCL0_16_06, (wCount == 9)?ucTARGET:(ucTARGET ^ *(pODTbl+wCount)));//Od table sram1 write data
        else
             msWriteByte(SCL0_16_06, 0x00);//Od table sram1 write data
        msWrite2Byte(SCL0_16_04, wCount|0x8000);//Od table sram1 address
        while(_bit7_(msReadByte(SCL0_16_05)));//Od table sram1 write enable (toggle type). , 0x3105[0]+ 0x3104[7:0] ==address [8:0]
    }
    ucTARGET=*(pODTbl+272+19);// 20th
    for(wCount=0; wCount<272; wCount++)
    {
        msWriteByte(SCL0_16_0C, (wCount == 19)?ucTARGET:(ucTARGET ^ *(pODTbl+272+wCount)));
        msWrite2Byte(SCL0_16_0A, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_0B)));
    }
    ucTARGET=*(pODTbl+272*2+29);// 30th
    for(wCount=0; wCount<272; wCount++)
    {
        if(wCount<256)
        msWriteByte(SCL0_16_12, (wCount == 29)?ucTARGET:(ucTARGET ^ *(pODTbl+272*2+wCount)));
        else
            msWriteByte(SCL0_16_12, 0x00);
        msWrite2Byte(SCL0_16_10, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_11)));
    }
    ucTARGET=*(pODTbl+272*2+256+39);// 40th
    for(wCount=0; wCount<256; wCount++)
    {
        msWriteByte(SCL0_16_18, (wCount == 39)?ucTARGET:(ucTARGET ^ *(pODTbl+272*2+256+wCount)));
        msWrite2Byte(SCL0_16_16, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_17)));
    }
    msWriteByte(SCL0_16_02, 0x00);
}
#endif
#endif
#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
void msDrvOvreDriveTable2nd(BYTE *pODTbl)
{
    WORD wCount;
    BYTE ucTARGET;

    if(0 == pODTbl) return;
    OD_DPUTSTR("Load_OD_2nd_Table");

    msWriteBit(SCL0_16_20, FALSE, _BIT0); // Disable OD before load code
    msWriteByte(SCL0_16_1C, 0x0E); // load code start

     // Uncompressed mode
    ucTARGET=*(pODTbl+76);// 10th//need to confirm OD tbl
    for(wCount=0; wCount<81; wCount++)
    {
        if(wCount<72)
            msWriteByte(SCL0_16_06, /*(wCount == 9)?ucTARGET:*/(ucTARGET ^ *(pODTbl+wCount)));//Od table sram1 write data
        else
            msWriteByte(SCL0_16_06, 0x00);
        msWrite2Byte(SCL0_16_04, wCount|0x8000);//Od table sram1 address
        while(_bit7_(msReadByte(SCL0_16_05)));//Od table sram1 write enable (toggle type). , 0x3105[0]+ 0x3104[7:0] ==address [8:0]
    }
    //ucTARGET=*(pODTbl+72+19);// 20th//need to confirm OD tbl
    for(wCount=0; wCount<72; wCount++)
    {
        msWriteByte(SCL0_16_0C, (wCount == 4)?ucTARGET:(ucTARGET ^ *(pODTbl+72+wCount)));
        msWrite2Byte(SCL0_16_0A, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_0B)));
    }
    //ucTARGET=*(pODTbl+72*2+29);// 30th//need to confirm OD tbl
    for(wCount=0; wCount<72; wCount++)
    {
        if(wCount<64)
            msWriteByte(SCL0_16_12, /*(wCount == 29)?ucTARGET:*/(ucTARGET ^ *(pODTbl+72*2+wCount)));
        else
            msWriteByte(SCL0_16_12, 0x00);
        msWrite2Byte(SCL0_16_10, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_11)));
    }
    //ucTARGET=*(pODTbl+72*2+64+39);// 40th//need to confirm OD tbl
    for(wCount=0; wCount<64; wCount++)
    {
        msWriteByte(SCL0_16_18, /*(wCount == 39)?ucTARGET:*/(ucTARGET ^ *(pODTbl+72*2+64+wCount)));
        msWrite2Byte(SCL0_16_16, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_17)));
    }
    msWriteByte(SCL0_16_1C, 0x00);
}
#else //MST9U4
void msDrvOvreDriveTable2nd(BYTE *pODTbl)
{
    if(0 == pODTbl) return;
    OD_DPUTSTR("Load_OD_2nd_Table");
}
#endif

void msDrvOverDriveOnOff(BOOL bEnable)
{
    //note:due to OD's double buffer mechanism, OD_MODE will update if output Vsync is coming.
    if(bEnable)
        msWriteByte(SCL0_16_20, (OD_MODE<<1)|_BIT0);
    else
        msWriteBit(SCL0_16_20, FALSE    ,_BIT0);
}

#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
void msDrvOverDriverInit(void)
{
    ////OD request base address
    msWrite4Byte(SCL0_16_2A, (DWORD)(OD_L_ADR)>>5);
    //Set OD frame buffer limit address for protecting R2 crash
    msWrite4Byte(SCL0_0D_A4, (DWORD)OD_L_ADR>>5);
    ////OD request base address
    msWrite4Byte(SCL0_58_2A, (DWORD)(OD_R_ADR)>>5);
    //Set OD frame buffer limit address for protecting R2 crash
    msWrite4Byte(SCL0_0D_D8, (DWORD)OD_R_ADR>>5);

    msWriteByte(SCL0_16_24, 0x10);  //od active threshold
    msWriteByte(SCL0_16_75, 0x2E);  //[5:0]od strength gradually slop

    msWriteByteMask(SCL0_16_6A, _BIT3, _BIT3);

    if(msDrvISSCDualPort())
        msWriteByteMask(SCL0_16_DF, BIT7, BIT7);    // h scaling middle line

    msDrvOverDriveMode(OD_MODE, OD_HSCAL_EN, OD_VSCAL_EN ,FALSE);
}
#else //MST9U4
void msDrvOverDriverInit(void)
{


    //OD request base address
    msWrite4ByteMask(SCL0_16_2A, (DWORD)(OD_L_ADR>>5)   ,0xFFFFFF);
    msWriteByteMask(SCL0_16_C0,  ((DWORD)(OD_L_ADR>>24)>>5)  ,0x03);
    //Set OD frame buffer limit address for protecting R2 crash
    msWrite4Byte(SCL0_0D_A4, (DWORD)OD_L_ADR>>5);

    //OD request base address
    msWrite4ByteMask(SCR0_16_2A, (DWORD)(OD_R_ADR>>5)   ,0xFFFFFF);
    msWriteByteMask(SCR0_16_C0,  ((DWORD)(OD_L_ADR>>24)>>5)  ,0x03);
    //Set OD frame buffer limit address for protecting R2 crash
    msWrite4Byte(SCL0_0D_D8, (DWORD)OD_R_ADR>>5);

    msWriteByte(SCL0_16_24, 0x10);  //od active threshold
    msWriteByte(SCL0_16_75, 0x2E);  //[5:0]od strength gradually slop
    //TBD SCL0_16_41[7]
    msWriteByteMask(SCL0_16_6A, _BIT3, _BIT3);

    msDrvOverDriveMode(OD_MODE, OD_HSCAL_EN, OD_VSCAL_EN,FALSE);
}
#endif
#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
void msDrvOverDriveMode(BYTE mode, BYTE h_scale, BYTE v_scale,BYTE OD_2nd_EN)
{
    DWORD u32OutputRes,u32ODWriteLimit,u32ODAddrLimit,u32PGBaseAddr;
    WORD u16PGAddrLimit, u16PnlHDE;
    BYTE u8ODBitPerPix = 24;
    v_scale=v_scale;
    msWriteByteMask(SCL0_00_80, BIT4, BIT4);
    if(msDrvISSCDualPort())
        u16PnlHDE = PanelWidth/2 + OD_GUARD_BAND;//Guard band for h scaling middle line
    else
        u16PnlHDE = PanelWidth;

    u32OutputRes = u16PnlHDE * PanelHeight;

    ////pg table address limit
    u16PGAddrLimit = (WORD)((float)u32OutputRes * u8ODBitPerPix / 128 / 64 / 256) + 2;
#if (CHIP_ID!=MST9U3)
    msWrite2ByteMask(SCL0_16_4E, (u16PGAddrLimit -1), 0x01FF);
#endif
    //OD_L
    ////pg table base address
    u32PGBaseAddr = (DWORD)OD_L_ADR + u16PGAddrLimit * 128 * 64 * 32;
    msWrite4Byte(SCL0_16_8E, u32PGBaseAddr>>5);//od request address limit
    ////OD limit address
#if (CHIP_ID==MST9U3)
    u32ODAddrLimit = ((DWORD)OD_L_ADR + (DWORD)OD_L_LEN) - 64;// 64 = 32 *2, keep 2 MIU bus unit margin.
#else
    u32ODAddrLimit = u32PGBaseAddr+((DWORD)u16PGAddrLimit<<5);
#endif
    msWrite4Byte(SCL0_16_2E, u32ODAddrLimit>>5);//od request address limit
    msWrite4Byte(SCL0_0D_A8, u32ODAddrLimit>>5);

    //OD_R
    ////pg table base address
    u32PGBaseAddr = (DWORD)OD_R_ADR + u16PGAddrLimit * 128 * 64 * 32;
    msWrite4Byte(SCL0_58_8E, u32PGBaseAddr>>5);//od request address limit
    ////OD limit address
#if (CHIP_ID==MST9U3)
    u32ODAddrLimit = ((DWORD)OD_R_ADR + (DWORD)OD_R_LEN) - 64;
#else
    u32ODAddrLimit = u32PGBaseAddr+((DWORD)u16PGAddrLimit<<5);
#endif
    msWrite4Byte(SCL0_58_2E, u32ODAddrLimit>>5);//od request address limit
    msWrite4Byte(SCL0_0D_DC, u32ODAddrLimit>>5);

    msWriteByteMask(SCL0_0D_A0, BIT0|BIT4, BIT0|BIT4);

    u32ODWriteLimit = ((DWORD)OD_L_LEN >> 2) ;

#if (CHIP_ID!=MST9U3)
    msWrite2ByteMask(SCL0_16_5E, (u16PnlHDE / (h_scale ? 2:1)), 0x0FFF);//tmp decode h size, it's important
#endif

#if (CHIP_ID==MST9U3)
    msWrite3Byte(SCL0_16_72, ((DWORD)OD_L_LEN)>>5); // OD write max limit, default set to 2MB
    msWrite3Byte(SCL0_16_76, ((DWORD)OD_L_LEN)>>5); // OD read max limit, default set to 2MB
#else
    msWrite3Byte(SCL0_16_72, (u32ODWriteLimit + 2)); // OD write max limit, default set to 2MB
    msWrite3Byte(SCL0_16_76, (u32OutputRes / (h_scale ? 2:1))+1); // OD read max limit, default set to 2MB
#endif

    if(PanelWidth % 2)//od compress mode encode preline force last data write to protect odd pix line
        msWriteByteMask(SCL0_16_4B, BIT3, BIT3);
    else
        msWriteByteMask(SCL0_16_4B, 0, BIT3);

    msWriteByteMask(SCL0_16_D8, h_scale, BIT0|BIT1|BIT3);

    if(OD_2nd_EN)
        msWriteByteMask(SCL0_16_1C, 0x00, BIT0);
    else
        msWriteByteMask(SCL0_16_1C, BIT0, BIT0);

    if(!msDrvISSCDualPort())
        msWriteByteMask(SCL0_00_80, 0x00, BIT4);

    msWriteByte(SCL0_16_20, (mode<<1)|_BIT0);

    //msWriteByteMask(SCL0_16_83, BIT7, BIT7);
    OD_DPRINTF("OD_mode = %x", mode);
    OD_DPRINTF("OD_h_scale = %x", h_scale);
    OD_DPRINTF("OD_OD_2nd_EN = %x", OD_2nd_EN);
}
#else //MST9U4
void msDrvOverDriveMode(BYTE mode, BYTE h_scale, BYTE v_scale ,BYTE OD_2nd_EN)
{
    DWORD u32OutputRes,u32ODWriteLimit,u32ODAddrLimit;
    WORD u16PnlHDE;
    OD_2nd_EN=OD_2nd_EN;

    msWriteByteMask(SCR0_00_88, BIT0, BIT0); //riu sub-bank common write enable

    if(msDrvISSCDualPort())
        u16PnlHDE = PanelWidth/2 + OD_GUARD_BAND;//Guard band for h scaling middle line
    else
        u16PnlHDE = PanelWidth;

    u32OutputRes = u16PnlHDE * PanelHeight;

      switch(mode)
      {
          case OD_MODE_444:
            u32ODAddrLimit = ((DWORD)u32OutputRes * 12 / 256 / (OD_HSCAL_EN ? 2:1)/ (OD_VSCAL_EN ? 2:1));
          break;
          case OD_MODE_565:
            u32ODAddrLimit = ((DWORD)u32OutputRes * 16 / 256 / (OD_HSCAL_EN ? 2:1)/ (OD_VSCAL_EN ? 2:1));
          break;
          case OD_MODE_666:
            u32ODAddrLimit = ((DWORD)u32OutputRes * 18 / 256 / (OD_HSCAL_EN ? 2:1)/ (OD_VSCAL_EN ? 2:1));
          break;
          case OD_MODE_555:
            u32ODAddrLimit = ((DWORD)u32OutputRes * 15 / 256 / (OD_HSCAL_EN ? 2:1)/ (OD_VSCAL_EN ? 2:1));
          break;
          case OD_MODE_888:
            u32ODAddrLimit = ((DWORD)u32OutputRes * 24 / 256 / (OD_HSCAL_EN ? 2:1)/ (OD_VSCAL_EN ? 2:1));
          break;
          default:
            u32ODAddrLimit = 0xFFFFFF;
          break;
      }
    //OD_L
    //OD limit address
    u32ODWriteLimit = (DWORD)(OD_L_ADR>>5) + u32ODAddrLimit;
    msWrite3Byte(SCL0_16_2E, u32ODWriteLimit);//od request address limit
    msWrite3Byte(SCL0_0D_A8, u32ODWriteLimit);

    //OD_R
    //OD limit address
    u32ODWriteLimit = (DWORD)(OD_R_ADR>>5) + u32ODAddrLimit;
    msWrite3Byte(SCR0_16_2E, u32ODWriteLimit);//od request address limit
    msWrite3Byte(SCL0_0D_DC, u32ODWriteLimit);
    //msWriteByteMask(SCL0_0D_A0, BIT0|BIT4, BIT0|BIT4);

    msWrite2ByteMask(SCL0_16_F4, u16PnlHDE/4, 0xFFFF);
    msWrite2ByteMask(SCL0_16_F6, PanelHeight, 0xFFFF);
    msWriteByteMask(SCL0_16_D8, h_scale, BIT0);
    msWriteByteMask(SCL0_16_F8, v_scale, BIT0);

    msWriteByteMask(SCL0_16_DA, 0x05, BIT2|BIT1|BIT0); //reg_sti_pro_en
    msWriteByteMask(SCL0_16_DB, 0x10, 0x3F); //reg_tdiff_gain
    msWriteByteMask(SCL0_16_DC, 0x00, 0xFF); //reg_tdiff_th

    if(!msDrvISSCDualPort())
        msWriteByteMask(SCR0_00_88, 0x00, BIT0);

    msWriteByte(SCL0_16_20, (mode<<1)|_BIT0);


    OD_DPRINTF("OD_mode = %x \r\n", mode);
    OD_DPRINTF("OD_h_scale = %x \r\n", h_scale);
}

#endif

void msDrvOverDriveWindow(BYTE enable, WORD u16HST,WORD u16HEND,  WORD u16VST, WORD u16VEND)
{
    #if (CHIP_ID==MST9U||CHIP_ID==MST9U2)
    BYTE ucMode = (msReadByte(SCL0_16_20)>>1)&0x07;
    BYTE ucH_Scale = msReadByte(SCL0_16_D8)&0x01;
    #endif
    BYTE i;

    if(enable)
    {
        msWriteByteMask(SCL0_16_20,BIT6|BIT7,BIT6|BIT7);

        //HST
        #if !(CHIP_ID==MST9U||CHIP_ID==MST9U2)
        msWriteByteMask(SCL0_16_7A,(u16HST<<4)&0x70,0x70);          //[6:4] LSB
        #endif
        msWriteByteMask(SCL0_16_26,(u16HST>>3)&0xFF,0xFF);          //[7:0]
        msWriteByteMask(SCL0_16_7D,((u16HST>>11)<<4)&0x10,0x10);    //[4]   MSB
        //HEND
        #if !(CHIP_ID==MST9U||CHIP_ID==MST9U2)
        msWriteByteMask(SCL0_16_7B,(u16HEND)&0x07,0x07);             //[2:0] LSB
        #endif
        msWriteByteMask(SCL0_16_27,(u16HEND>>3)&0xFF,0xFF);          //[7:0]
        msWriteByteMask(SCL0_16_7D,((u16HEND>>11)<<5)&0x20,0x20);    //[5]   MSB

        //VST
        #if !(CHIP_ID==MST9U||CHIP_ID==MST9U2)
        msWriteByteMask(SCL0_16_7B,(u16VST<<2)&0x1C,0x1C);          //[4:2] LSB
        #endif
        msWriteByteMask(SCL0_16_28,(u16VST>>3)&0xFF,0xFF);          //[7:0]
        msWriteByteMask(SCL0_16_7D,((u16VST>>11)<<6)&0x40,0x40);    //[6]   MSB
        //VEND
        #if !(CHIP_ID!=MST9U3)
        msWriteByteMask(SCL0_16_7B,(u16VEND<<5)&0xE0,0xE0);          //[7:5] LSB
        #endif
        msWriteByteMask(SCL0_16_29,(u16VEND>>3)&0xFF,0xFF);          //[7:0]
        msWriteByteMask(SCL0_16_7D,((u16VEND>>11)<<7)&0x80,0x80);    //[7]   MSB
    }
    else
        msWriteByteMask(SCL0_16_20,0x00,BIT6|BIT7);

    #if (CHIP_ID==MST9U||CHIP_ID==MST9U2)
        if((ucMode == OD_MODE_COMPRESS) )
        {
            WORD uc_HSize = (((WORD)u16HEND-u16HST)+BIT3)/(ucH_Scale ? 2:1);
            msWrite2ByteMask(SCL0_16_5E,uc_HSize,0x0FFF);
        }
    #endif

    msWriteByteMask(SCL0_16_6C,BIT3, BIT3);
    Delay1ms(1000);
    msWriteByteMask(SCL0_16_6C,0x00, BIT3);
    for(i=0;i<5;i++)
        OD_DPRINTF("OD_Status = %x", msReadByte(SCL0_16_70));
}


void msDrvOverDriveVerify(void)
{
    WORD wCount;

    OD_DPUTSTR("Load_OD_Table");

    msWriteBit(SCL0_16_20,FALSE, _BIT0); // Disable OD before load code
    msWriteByte(SCL0_16_02, 0x0E); // load code start

    for(wCount=0; wCount<289; wCount++)
    {
        if(wCount>255)
        {
             msWriteByte(SCL0_16_06, wCount-255);
             OD_DPRINTF("%x",wCount-255);
        }
        else
        {
            msWriteByte(SCL0_16_06, wCount);//Od table sram1 write data
            OD_DPRINTF("%x",wCount);
        }

        msWrite2Byte(SCL0_16_04, wCount|0x8000);//Od table sram1 address
        while(_bit7_(msReadByte(SCL0_16_05)));//Od table sram1 write enable (toggle type). , 0x3105[0]+ 0x3104[7:0] ==address [8:0]
    }
    for(wCount=0; wCount<272; wCount++)
    {
        if(wCount>255)
        {
            msWriteByte(SCL0_16_0C, wCount-255);
            OD_DPRINTF("%x",wCount-255);
        }
        else
        {
            msWriteByte(SCL0_16_0C, wCount);
            OD_DPRINTF("%x",wCount);
        }
        msWrite2Byte(SCL0_16_0A, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_0B)));

    }
    for(wCount=0; wCount<272; wCount++)
    {
        if(wCount>255)
        {
            msWriteByte(SCL0_16_12, wCount-255);
            OD_DPRINTF("%x",wCount-255);
        }
        else
        {
            msWriteByte(SCL0_16_12, wCount);
            OD_DPRINTF("%x",wCount);
        }
        msWrite2Byte(SCL0_16_10, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_11)));
    }
    for(wCount=0; wCount<256; wCount++)
    {
        msWriteByte(SCL0_16_18, wCount);
        OD_DPRINTF("%x",wCount);
        msWrite2Byte(SCL0_16_16, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_17)));
    }
    OD_DPUTSTR("Read_OD_Table");
    for(wCount=0; wCount<289; wCount++)
    {
        msWrite2Byte(SCL0_16_04, wCount|0x4000);//Od table sram1 address
        while(_bit6_(msReadByte(SCL0_16_05)));//Od table sram1 read enable (toggle type). , 0x3105[0]+ 0x3104[7:0] ==address [8:0]
        OD_DPRINTF("%x",msReadByte(SCL0_16_08));
    }
    for(wCount=0; wCount<272; wCount++)
    {
        msWrite2Byte(SCL0_16_0A, wCount|0x4000);
        while(_bit6_(msReadByte(SCL0_16_0B)));
        OD_DPRINTF("%x",msReadByte(SCL0_16_0E));
    }
    for(wCount=0; wCount<272; wCount++)
    {
        msWrite2Byte(SCL0_16_10, wCount|0x4000);
        while(_bit6_(msReadByte(SCL0_16_11)));
        OD_DPRINTF("%x",msReadByte(SCL0_16_14));
    }
    for(wCount=0; wCount<256; wCount++)
    {
        msWrite2Byte(SCL0_16_16, wCount|0x4000);
        while(_bit6_(msReadByte(SCL0_16_17)));
        OD_DPRINTF("%x",msReadByte(SCL0_16_1A));
    }
    msWriteByte(SCL0_16_02, 0x00);
}

#if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
void msDrvOverDriveVerify2nd(void)
{
    BYTE wCount;

    OD_DPUTSTR("Load_OD_2nd_Table");

    msWriteBit(SCL0_16_20,FALSE, _BIT0); // Disable OD before load code
    msWriteByte(SCL0_16_1C, 0x0E); // load code start

    for(wCount=0; wCount<81; wCount++)
    {
        msWriteByte(SCL0_16_06, wCount);//Od table sram1 write data
        OD_DPRINTF("%x",wCount);
        msWrite2Byte(SCL0_16_04, wCount|0x8000);//Od table sram1 address
        while(_bit7_(msReadByte(SCL0_16_05)));//Od table sram1 write enable (toggle type). , 0x3105[0]+ 0x3104[7:0] ==address [8:0]
    }
    for(wCount=0; wCount<72; wCount++)
    {
        msWriteByte(SCL0_16_0C, wCount);
        OD_DPRINTF("%x",wCount);
        msWrite2Byte(SCL0_16_0A, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_0B)));

    }
    for(wCount=0; wCount<72; wCount++)
    {
        msWriteByte(SCL0_16_12, wCount);
        OD_DPRINTF("%x",wCount);
        msWrite2Byte(SCL0_16_10, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_11)));
    }
    for(wCount=0; wCount<64; wCount++)
    {
        msWriteByte(SCL0_16_18, wCount);
        OD_DPRINTF("%x",wCount);
        msWrite2Byte(SCL0_16_16, wCount|0x8000);
        while(_bit7_(msReadByte(SCL0_16_17)));
    }
    OD_DPUTSTR("Read_OD_2nd_Table");
    for(wCount=0; wCount<81; wCount++)
    {
        msWrite2Byte(SCL0_16_04, wCount|0x4000);//Od table sram1 address
        while(_bit6_(msReadByte(SCL0_16_05)));//Od table sram1 read enable (toggle type). , 0x3105[0]+ 0x3104[7:0] ==address [8:0]
        OD_DPRINTF("%x",msReadByte(SCL0_16_08));
    }
    for(wCount=0; wCount<72; wCount++)
    {
        msWrite2Byte(SCL0_16_0A, wCount|0x4000);
        while(_bit6_(msReadByte(SCL0_16_0B)));
        OD_DPRINTF("%x",msReadByte(SCL0_16_0E));
    }
    for(wCount=0; wCount<72; wCount++)
    {
        msWrite2Byte(SCL0_16_10, wCount|0x4000);
        while(_bit6_(msReadByte(SCL0_16_11)));
        OD_DPRINTF("%x",msReadByte(SCL0_16_14));
    }
    for(wCount=0; wCount<64; wCount++)
    {
        msWrite2Byte(SCL0_16_16, wCount|0x4000);
        while(_bit6_(msReadByte(SCL0_16_17)));
        OD_DPRINTF("%x",msReadByte(SCL0_16_1A));
    }
    msWriteByte(SCL0_16_1C, 0x00);
}
#else //MST9U4
void msDrvOverDriveVerify2nd(void)
{

}
#endif

void msDrvOverDriveAdjustWeighting(BYTE u8Weighting)
{
    BYTE u8BackupcODCommonWrite=0;

    #if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
        u8BackupcODCommonWrite=msReadByte(SCL0_00_80);
        msWriteByteMask(SCL0_00_80, BIT4, BIT4);
    #else //MST9U4
        u8BackupcODCommonWrite=msReadByte(SCR0_00_88);
        msWriteByteMask(SCR0_00_88, BIT0, BIT0);
    #endif

    #if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
        msWriteByteMask(SCL0_16_83, ((u8Weighting&0x80)>>7)<<7,BIT7);
        msWriteByteMask(SCL0_16_4B, ((u8Weighting&0x40)>>6)<<4,BIT4);
        msWriteByteMask(SCL0_16_23, ((u8Weighting&0x3C)>>2)<<0,0x0F);
        msWriteByteMask(SCL0_16_87, ((u8Weighting&0x03)>>0)<<6,0xC0);
    #else //MST9U4
        msWriteByteMask(SCL0_16_22, u8Weighting,0xFF);
    #endif

    #if CHIP_ID==MST9U||CHIP_ID==MST9U2||CHIP_ID==MST9U3
        msWriteByte(SCL0_00_80, u8BackupcODCommonWrite);
    #else //MST9U4
        msWriteByte(SCR0_00_88, u8BackupcODCommonWrite);
    #endif
}
