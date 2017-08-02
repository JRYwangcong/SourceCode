#define _DEBUG_C_

#if ENABLE_MSTV_UART_DEBUG
#include "datatype.h"
#include "Common.h"
//#include "debugDef.h"
#include "drvUartDebug.h"
#include "Global.h"
#include "Board.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "msOSD.h"
#include "NVRam.h"
#include "i2c.h"
#include "misc.h"
#include "Adjust.h"
//#include "msADC.h"
#include "drvADC.h"
//#include "userPref.h"
#include "AutoFunc.h"
#include "menudef.h"
#include "menufunc.h"
#include "msACE.h"
#include "MsDLC_Setting.h"
//#include "MsDLC.h"
#include "msflash.h"
#include "drvmsOVD.h"
#include "Keypaddef.h"
#include "appWindow.h"

#include "Power.h"
#include "MDebug.h"
#include "Common.h"
#include "msflash.h"
#include "GPIO_DEF.h"
#include "ColorFinetune_Default.h"
#include "drvOSDblend.h"
#if ENABLE_DPS
#include "msDPS_Setting.h"
#endif
#include "Keypad.h"
#if ENABLE_DAC
#include "drvAudio.h"
#endif

#if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif

#if ENABLE_MENULOAD
#include "apiXC_SC_MenuLoad.h"
#endif

#include "msPQ.h"
#include "appWindow.h"
#if (ZUI)
#include "MApp_ZUI_APIgdi.h"
#endif

#if (XMODEM_DWNLD_ENABLE)
#include "xmodem.h"
#endif

#if ENABLE_DEMURA_FUNCTION
#include "drvDemura.h"
#endif

#if ENABLE_MSTV_UART_DEBUG
#warning "Please remember to disable the debug function!"
#endif

#if ENABLE_DELTAE_FUNCTION
#include "drvDeltaE.h"
#endif

#include "drvMWE.h"

#if ENABLE_PIU_UART_ResetCheck
#include "drvUART.h"
#include "uartdebug.h"
#include "SysInit.h"
#endif
#include "drvUsbMain.h"

#if ENABLE_DAISY_CHAIN
#include "msDaisyChain.h"
#endif

#include "imginfo.h"


//BYTE xdata SIORxIndex = 0;
//BYTE xdata ReSyncCounter;
extern Bool xdata bFlashWriteFactory;
#define DEBUG_Massage    1
#if ENABLE_MSTV_UART_DEBUG&&DEBUG_Massage
    #define DB_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
    #define DB_PRINT(format, ...)
#endif




#if ENABLE_MSTV_UART_DEBUG
#if ENABLE_TOUCH_PANEL || ENABLE_TOUCH_PANEL_CTRL_OSD
extern Bool TPL_ReadSurface(void);
#endif
#if ENABLE_TOUCH_PANEL_DATA_FROM_USB
extern Bool USB_SPI_WriteRegister(BYTE addr, BYTE val);
extern Bool USB_SPI_ReadRegister(BYTE addr, BYTE *val, BYTE count);
#endif
#endif
extern void putcharb(MS_U8 byte);

void putSIOChar( unsigned char sendData )
{
#if 1
    putcharb(sendData);
#else
   if(!UART_READ_ES())
        return;

    UART_BUFFER() = sendData;
    while( 1 )
    {
        if( EA && UART_READ_ES() )
        {
            if( TI0_FLAG )
            {
                TI0_FLAG = 0;
                break;
            }
        }
        else
        {
            if(UART_TI_FLAG())
            {
                UART_CLR_TI_FLAG();
                break;
            }
        }
    }
#endif
}

#if ENABLE_MSTV_UART_DEBUG
/*
Please enter the position of data you want to check
Ex:
Resolution:800x600

 (0,0)           (799,0)
    _____________
    |           |
    |           |
    |           |
    -------------
 (0,599)         (799,599)
H[11:0] = {ucPara1[7:0], ucPara2[7:4]}
V[11:0] = {ucPara2[3:0], ucPara3[7:0]}
*/
void msLineBufLatchData(BYTE ucPara1, BYTE ucPara2, BYTE ucPara3)
{
    WORD HPosition = ((ucPara1<<4)|(ucPara2>>4))&0x0FFF, VPosition = ((ucPara2<<8)|(ucPara3))&0x0FFF;
    WORD VStart = _msRead2Byte(SC0_05);

    _msWrite2ByteMask(SC0_05,(VStart+VPosition),0x7FF);
    _msWrite2ByteMask(SC5_31,( HPosition / 3 ), 0x0FFF); // H pack address
    _msWriteByteMask(SC5_33, ( HPosition % 3 ), 0x03); // H position
    _msWriteByteMask(SC5_34, BIT7,BIT7);//1:read from start of frmae, 0:read from end of frame
    _msWriteByteMask(SC5_32, BIT6, BIT6); // LB latch data function enable
    Delay1ms(100);
    DB_PRINT("LB R0:%d",_msReadByte(SC5_35)); // LB Latch R0
    DB_PRINT("LB G0:%d",_msReadByte(SC5_36)); // LB Latch G0
    DB_PRINT("LB B0:%d\r\n",_msReadByte(SC5_37)); // LB Latch B0
    _msWriteByteMask(SC5_32, 0, BIT6); // IP latch data function disable
    _msWrite2ByteMask(SC0_05,VStart,0x7FF); //restore V start value
}

/*
Please enter the position of data you want to check
Ex:
Resolution:800x600

 (0,0)           (799,0)
    _____________
    |           |
    |           |
    |           |
    -------------
 (0,599)         (799,599)

   H[11:0] = {ucPara1[7:0], ucPara2[7:4]}
   V[11:0] = {ucPara2[3:0], ucPara3[7:0]} */
void msIPLatchData(BYTE ucPara1, BYTE ucPara2, BYTE ucPara3)
{
    WORD HPosition = ((ucPara1<<4)|(ucPara2>>4))&0x0FFF, VPosition = ((ucPara2<<8)|(ucPara3))&0x0FFF;
    _msWrite2ByteMask(SC5_31, (HPosition+(_msRead2Byte(SC0_07)&0x7FF)-1), 0x0FFF); // Image H position
    _msWrite2ByteMask(SC5_33, (VPosition+(_msRead2Byte(SC0_05)&0x7FF)), 0x07FF); // Image V position
    _msWriteByteMask(SC5_34, BIT5, BIT5); // IP latch data function enable
    Delay1ms(100);
    DB_PRINT("IP R:%d",_msReadByte(SC5_35)); // IP Latch R
    DB_PRINT("IP G:%d",_msReadByte(SC5_36)); // IP Latch G
    DB_PRINT("IP B:%d\r\n",_msReadByte(SC5_37)); // IP Latch B
    _msWriteByteMask(SC5_34, 0, BIT5); // IP latch data function disable
}
/*non-inear scaling curve
      W0   W1   W2   W2    W1    W0
         |    |____|____|_    |
         |   /|    |    | \   |
         |  / |    |    |  \  |
         | /  |    |    |   \ |
    _____|/   |    |    |    \|________
   Initial Offset = ucPara1[6:0]
   Delta1 = ucPara2[7:0]
*/
void msNonLinearScaling(BYTE ucPara1, BYTE ucPara2)
{
    WORD W0, W1, W2;

    if(!(_msReadByte(SC0_52)&BIT7))
        _msWrite3Byte(SC0_50, 0x900000);
    W1 = ((DWORD)ucPara1<<12)*2/(ucPara2<<3);
    if(W1>(PANEL_WIDTH/2))
    {
        DB_PRINT("Since the initial offset is too large or delta1 is too small\r\n");
        DB_PRINT("The non-linear scaling results may be wrong!\r\n");
        DB_PRINT("Please correct the settings and try again\r\n");
    }
    W0 = ((PANEL_WIDTH/2)-W1)/2;
    W2 = W0;
    _msWrite2ByteMask(SCD_20, W0,0x3FF);//width of zone 0 in non-linear
    _msWrite2ByteMask(SCD_22, W1,0x3FF);//width of zone 1 in non-linear
    _msWrite2ByteMask(SCD_24, W2,0x3FF);//width of zone 2 in non-linear
    _msWriteByteMask(SCD_26, ucPara1,0x7F);//value of initial offset
    _msWrite2ByteMask(SCD_26, BIT7,BIT7);//0: positive initial offset, 1: negative initial offset"
    _msWrite2ByteMask(SCD_27, BIT7, BIT7);//non-linear scaling enable
    _msWriteByte(SCD_28, 0);//delta value in zone 0
    _msWriteByte(SCD_29, ucPara2);//delta value in zone 1
}

/* test Internal EDID
CHIP_FAMILY_TSUMC: 256*2+128 K Sram

    _____________   0 K - Start
    |  Arranged | 128 K
    |     IN    | 256 K
    |  REG_3EEC | 384 K
    |  REG_3EED | 512 K
    ------------- 640 K - End

256*2: for DP (if DP not used, can be arranged for other ports)
128: for VGA or DVI or shared
*/
void TestInternalEDID_Write(BYTE SramIndex, BYTE StartIndex)
{
    BYTE i;

    _msWriteByteMask( REG_3E4B , 0,BIT7);// Set DDC To Write
    _msWriteByteMask( REG_3E49 , 0,BIT5);// Clr Write Protect
    _msWriteByteMask( REG_3EEB , SramIndex<<4, 0xF0);// Sram Index

    for( i = 0; i < 128; i++ )
    {
        _msWriteByte( REG_3E4B ,i );
        _msWriteByte( REG_3E4C ,i+StartIndex );

        _msWriteByteMask( REG_3E43 , BIT5,BIT5);
        _msWriteByteMask( REG_3E43 , 0,BIT5);
    }
    _msWriteByteMask( REG_3E4B , BIT7,BIT7);// Set DDC To Read
    _msWriteByteMask( REG_3E49,  BIT5 | BIT7, BIT5 | BIT7 ); // [7]ADC DDC enable / [5]Set Write Protect
}

void TestInternalEDID_Read(BYTE SramIndex)
{
    BYTE i;

    _msWriteByteMask( REG_3EEB , SramIndex<<4, 0xF0);// Sram Index

    for( i = 0; i < 128; i++ )
    {
        _msWriteByte( REG_3E4B ,i );
        _msWriteByteMask( REG_3E43 , BIT4,BIT4);
        _msWriteByteMask( REG_3E43 , 0,BIT4);
        DB_PRINT("%d",_msReadByte(REG_3E0E));
    }
}

/* ucPara1: 0:R, 1:G, 2:B */
void msCTMaxMin(BYTE ucPara1)
{
    _msWriteByteMask(SC9_41, ucPara1<<5, _BIT6|_BIT5);
    DB_PRINT("Max CT: %d\r\n", _msRead2Byte(SC9_4A)>>2);
    DB_PRINT("Min CT: %d\r\n", _msRead2Byte(SC9_4C)>>2);
}

/*
Please enter the position of data you want to check
Ex:
Resolution:800x600
 (0,0)           (799,0)
    _____________
    |           |
    |           |
    |           |
    -------------
 (0,599)         (799,599)
   H[11:0] = {ucPara1[7:0], ucPara2[7:4]}
   V[11:0] = {ucPara2[3:0], ucPara3[7:0]} */
void msCT_CoordinatesSetting(BYTE ucPara1  ,BYTE ucPara2 ,BYTE ucPara3)
{
    BYTE uLength=10;
    WORD wi,wj , HPosition = ((ucPara1<<4)|(ucPara2>>4))&0x0FFF, VPosition = ((ucPara2<<8)|(ucPara3))&0x0FFF;
    //BYTE uHOffset=_msRead2Byte(SC0_12)+PanelHSyncWidth;
    BYTE uHOffset=0x6D;//_msRead2Byte(SC0_12)+PanelHSyncWidth;

    DB_PRINT(" CT square size: %d x %d", uLength);

    for( wi = VPosition; wi < (VPosition+uLength); wi = wi + 1 )
    {
    DB_PRINT(" \n\r CT V_ST_Position: %d", wi);
    _msWrite2ByteMask(SC9_46, wi, 0x3FF);            //reg_ct_vst
    _msWrite2ByteMask(SC9_48, wi+uLength, 0x3FF);    //reg_ct_vend

    for( wj = HPosition; wj < (HPosition+uLength); wj = wj + 1 )
    {
        _msWrite2ByteMask(SC9_42, wj +uHOffset, 0x1FFF);             //reg_ct_hst
        _msWrite2ByteMask(SC9_44, wj+uLength+uHOffset, 0x1FFF);      //reg_ct_hend
        DB_PRINT(" \n\r CT H_ST_Position: %d", wj);

        DB_PRINT(" \n\r Max CT: %d", _msRead2Byte(SC9_4A)>>2);
        DB_PRINT(" \n\r Min CT: %d", _msRead2Byte(SC9_4C)>>2);
    }
    }
}


void msDrvIPPatternGen(BYTE u8ScalerIdx, BYTE u8ModeIdx)
{
    BYTE u8ResIdx = StandardMode[u8ModeIdx].ResIndex;


    // select ipclk = odclk
    msWriteByte(REG_120F18+u8ScalerIdx, 0x0c);


    //set timing gen.
    msWrite2Byte(REG_102E24, StandardModeResolution[u8ResIdx].DispWidth); //hde
    msWrite2Byte(REG_102E26, 0x16); // h front porch
    msWrite2Byte(REG_102E28, 0x10); // h sync width
    msWrite2Byte(REG_102E2A, StandardMode[u8ModeIdx].HTotal); //htotal

    msWrite2Byte(REG_102E2C, StandardModeResolution[u8ResIdx].DispHeight); //vde
    msWrite2Byte(REG_102E2E, 0x10); // v front porch
    msWrite2Byte(REG_102E30, 0x4); // v sync width
    msWrite2Byte(REG_102E32, StandardMode[u8ModeIdx].VTotal); //vtotal

    if(StandardMode[u8ModeIdx].Flags & bInterlaceMode)
        msWriteByte(REG_102E21, 0x00); //??
    else
        msWriteByte(REG_102E21, 0x00);

    //ramp ??
    msWriteByte(REG_102E34, 0x02);
    msWriteByte(REG_102E35, 0x04);
    msWriteByte(REG_102E36, 0x00);
    msWriteByte(REG_102E37, 0x00);
    msWriteByte(REG_102E38, 0x00);
    msWriteByte(REG_102E39, 0x02);
    msWriteByte(REG_102E3A, 0xff);
    msWriteByte(REG_102E3B, 0x03);
    msWriteByte(REG_102E3C, 0x00);
    msWriteByte(REG_102E3D, 0x02);
    msWriteByte(REG_102E3E, 0x00);
    msWriteByte(REG_102E3F, 0x00);

    //ipmux
    msWriteByte(REG_102E02, 0xf0);

    //enable pattern gen.
    msWriteByte(REG_102E20, 0x01);

}
#if ENABLE_JUMP
static void TestCommand_Jump( void )
{
    BYTE ucPara1 = (g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
    BYTE ucPara2 = (g_UartCommand.Buffer[_UART_CMD_INDEX4_]);

    switch( (g_UartCommand.Buffer[_UART_CMD_INDEX2_]) )
    {
        case 0x00:
            {
                msLVDSOpenShortTest();
            }
            break;

        case 0x01:
            {
                msInputOpenShortTest();
            }
            break;

        default:
            ucPara1 = ucPara2;
            break;
    }
}
#endif
static void TestCommand_Scaler( void )
{
    BYTE ucPara1 = (g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
    BYTE ucPara2 = (g_UartCommand.Buffer[_UART_CMD_INDEX4_]);

    switch( (g_UartCommand.Buffer[_UART_CMD_INDEX2_]) )
    {
        case 0x00:
            {
                BYTE    WinIndex, SCDetWin, InputPort;

                for( WinIndex=DISPLAY_WIN1; WinIndex < DISPLAY_MAX_NUMS; WinIndex++ )
                    DB_PRINT("\nWindowState = %d, %d",
                        WinIndex,
                        g_DisplayWindow[WinIndex].eDwStatus);
                for( SCDetWin=SCL0_SUB ; SCDetWin < SC_MAX_NUMS ; SCDetWin++ )
                    DB_PRINT("\nScalerState = %d, %d, %d",
                        SCDetWin, g_ScalerInfo[SCDetWin].eSiDispStatus,
                        g_ScalerInfo[SCDetWin].u8SiIPPortIndex);
                for( InputPort=First_Input_Port; InputPort < Input_Nums; InputPort++ )
                    DB_PRINT("\nInputState = %d, %d, %d, %x",
                        InputPort,
                        g_InputPort[InputPort].eIPStatus,
                        g_InputPort[InputPort].u8IPScIndex,
                        g_InputPort[InputPort].u16IPScMask);

                DB_PRINT("\n\n");
            }
            break;

        case 0x01:
            {
                APPWindow_Configure((eMULTIWIN_Type)ucPara1 ,(ePIPSIZE_Type)ucPara2);
                DB_PRINT("\nMultiWinType = %d, PIP_Size = %d\n",ucPara1, ucPara2);
            }
            break;

        case 0x02:
            {
                    DB_PRINT("\nMONITOR_FLAGS = %d", MONITOR_FLAGS);
            }
            break;

        case 0x03:
            {
                    MONITOR_FLAGS = ucPara1;
                    DB_PRINT("\n Set MONITOR_FLAGS ", 0);
            }
            break;

        case 0x04:
            {
                BYTE    WinIndex;

                for( WinIndex=DISPLAY_WIN1; WinIndex < DISPLAY_MAX_NUMS; WinIndex++ )
                    if( g_DisplayWindow[WinIndex].u8DwEnable )
                    {
                        DB_PRINT("\nWindowMem[%d][0]: %08x, %08x, %08x",WinIndex, g_DisplayWindow[WinIndex].u32DwFbSize[0], g_DisplayWindow[WinIndex].u32DwMemBase[0], g_DisplayWindow[WinIndex].u32DwMemSize[0]);
                        DB_PRINT("\nWindowMem[%d][1]: %08x, %08x, %08x",WinIndex, g_DisplayWindow[WinIndex].u32DwFbSize[1], g_DisplayWindow[WinIndex].u32DwMemBase[1], g_DisplayWindow[WinIndex].u32DwMemSize[1]);
                    }
            }
            break;


        case 0x05:
            {
                msAPIWinSetColorSpaceInput(DISPLAY_WIN1, (EN_INPUT_COLOR)ucPara1, eCOLOR_RANGE_FULL);
                msAPIWinSetColorSpaceWindow(DISPLAY_WIN1, (EN_WINDOW_COLOR)ucPara2);
                msAPI_AdjustContrast( DISPLAY_WIN1, 0x80);
                DB_PRINT("\n>>>InputColor = %d, WinColor = %d", ucPara1, ucPara2);
            }
            break;
#if ENABLE_VGA_INPUT
        case 0x06:
            {
                drvADC_SetADCSource((ADC_SOURCE_TYPE) ucPara1);
                DB_PRINT("\n>>>SetADCColor = %d", ucPara1);
            }
            break;

        case 0x07:
            {
                drvADC_SetPhaseCode(ucPara1);
                DB_PRINT("\n>>>SetADCPhaseCode = %d", ucPara1);
            }
            break;

        case 0x08:
            {
                mStar_AutoGeomtry(DISPLAY_WIN1);
                DB_PRINT("\n>>>AutoGeomtr");
            }
            break;
#endif
        case 0x09:
            {
                msDrvMcuR2To51();
            }
            break;

        case 0x0A:
            {
                DB_PRINT("\n>>>InputCableConnected %d = %d",ucPara1, msAPISrcIsCableConnected(ucPara1));
            }
            break;

        case 0x0B:
            {
                msAPI_combo_IPControlHPD((EN_COMBO_IP_SELECT)g_InputPort[ucPara1].eIPMux, ucPara2);
                DB_PRINT("\n>>>HPD port: %d, HL:%d", ucPara1, ucPara2);
            }
            break;

        case 0x11:
            g_u8PQEnable = ucPara1;
            DB_PRINT("g_u8PQEnable:%d\r\n", g_u8PQEnable);
            break;

        case 0x15:
            msDrv_SDHD_DETECT(ucPara1);
            break;

        case 0x20:
            msAPIWinSetMem12(ucPara1, ucPara2);
            DB_PRINT("Win:%d, Mem12:%d\r\n", ucPara1, ucPara2);
            break;

        default:
            ucPara1 = ucPara2;
            break;
    }
}

static void TestCommand_PxP( void )
{
    ST_WINDOW_INFO  DisplayWin = {0, 0, PANEL_WIDTH, PANEL_HEIGHT};

    switch( UART_EXT_CMD_MS_DAT2 )
    {
        case 0x00:
            {
                DB_PRINT("[DB_PxP] 0x00: List of Cmds\n");
                DB_PRINT("[DB_PxP] 0x01: Del all win\n");
                DB_PRINT("[DB_PxP] 0x02: Add 1 win (+param:W,P,R,xx,yy,ww,hh)\n");
                DB_PRINT("[DB_PxP] 0x03: Win apply all\n");
                DB_PRINT("[DB_PxP] 0x04: Win resetup (+param:W)\n");
                DB_PRINT("[DB_PxP] 0x05: Win re-position (+param:W,xx,yy)\n");
                DB_PRINT("[DB_PxP] 0x06: Win re-size (+param:W,ww,hh)\n");
                DB_PRINT("[DB_PxP] 0x07: Win re-shape (+param:W,xx,yy,ww,hh)\n");
                DB_PRINT("[DB_PxP] 0x08: Win SwitchPort (+param:W,P)\n");
                DB_PRINT("[DB_PxP] 0x09: Win SourceSwap (+param:W,W)\n");
                DB_PRINT("[DB_PxP] 0x0A: Win Rotate (+param:W,R)\n");
                DB_PRINT("[DB_PxP] 0x0B: Win Enable(+param:W,P,R)\n");
                DB_PRINT("[DB_PxP] 0x0C: Win Disable(+param:W)\n");
                DB_PRINT("[DB_PxP] 0x10: 2 Win (+param:P,R)\n");
                DB_PRINT("[DB_PxP] 0x11: 4 Win (+param:P,R)\n");
                DB_PRINT("[DB_PxP] 0x12: 8 Win (+param:P,R) - need extend win#\n");
                DB_PRINT("[DB_PxP] 0x13: 8 Win (+param:P,R) - need extend win#\n");
                DB_PRINT("[DB_PxP] 0x14: 1 Win (+param:P,R)\n");
                DB_PRINT("[DB_PxP] 0x15: 6 Win (+param:X) - need extend win#\n");
            }
            break;

        case 0x01: // Del all
            {
                BYTE u8IdxWin;

                for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
                {
                    msAPIWinDel(u8IdxWin);
                }
                DB_PRINT("[DB_PxP] Window Del All\n");
            }
            break;

        case 0x02: // Add 1 win
            {
                DisplayWin.x        = (UART_EXT_CMD_MS_DAT6<<8) | UART_EXT_CMD_MS_DAT7;
                DisplayWin.y        = (UART_EXT_CMD_MS_DAT8<<8) | UART_EXT_CMD_MS_DAT9;
                DisplayWin.width    = (UART_EXT_CMD_MS_DAT10<<8) | UART_EXT_CMD_MS_DAT11;
                DisplayWin.height   = (UART_EXT_CMD_MS_DAT12<<8) | UART_EXT_CMD_MS_DAT13;

                msAPIWinAdd(UART_EXT_CMD_MS_DAT3,  UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5, DisplayWin);
                //g_DisplayWindow[UART_EXT_CMD_MS_DAT3].u8Dw12b = UART_EXT_CMD_MS_DAT14;
                DB_PRINT("[DB_PxP] Win:%d, Port:%d, Rot:%d, x:%d, y:%d, W:%d, H:%d \n",UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5, DisplayWin.x, DisplayWin.y, DisplayWin.width, DisplayWin.height);
            }
            break;

        case 0x03: // Win apply
            {
                msAPIWinApply();
                DB_PRINT("[DB_PxP] Window Apply All\n");
            }
            break;

        case 0x04: // Win Re-setup
            {
                msAPIWinReSetup(UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, TRUE);
                DB_PRINT("[DB_PxP] Window Resetup:%d, %d \n", UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4);
            }
            break;

        case 0x05: // Win Re-position
            {
                APPWindow_SetPosition(UART_EXT_CMD_MS_DAT3,(UART_EXT_CMD_MS_DAT4<<8) | UART_EXT_CMD_MS_DAT5, (UART_EXT_CMD_MS_DAT6<<8) | UART_EXT_CMD_MS_DAT7);
                DB_PRINT("[DB_PxP] Window Re-position: Win:%d, x:%d, y:%d \n", UART_EXT_CMD_MS_DAT3,(UART_EXT_CMD_MS_DAT4<<8) | UART_EXT_CMD_MS_DAT5, (UART_EXT_CMD_MS_DAT6<<8) | UART_EXT_CMD_MS_DAT7);
            }
            break;

        case 0x06: // Win Re-size
            {
                ST_WINDOW_INFO  sDisplayWin;
                msAPIWinGetDisplayWin(UART_EXT_CMD_MS_DAT3, &sDisplayWin);
                APPWindow_SetSize(UART_EXT_CMD_MS_DAT3, sDisplayWin.x, sDisplayWin.y,(UART_EXT_CMD_MS_DAT4<<8) | UART_EXT_CMD_MS_DAT5, (UART_EXT_CMD_MS_DAT6<<8) | UART_EXT_CMD_MS_DAT7);
                DB_PRINT("[DB_PxP] Window Re-size: Win:%d, W:%d, H:%d \n", UART_EXT_CMD_MS_DAT3,(UART_EXT_CMD_MS_DAT4<<8) | UART_EXT_CMD_MS_DAT5, (UART_EXT_CMD_MS_DAT6<<8) | UART_EXT_CMD_MS_DAT7);
            }
            break;

        case 0x07: // Win Re-shape
            {
                ST_WINDOW_INFO  sDisplayWin;

                sDisplayWin.x      = (UART_EXT_CMD_MS_DAT4<<8) | UART_EXT_CMD_MS_DAT5;
                sDisplayWin.y      = (UART_EXT_CMD_MS_DAT6<<8) | UART_EXT_CMD_MS_DAT7;
                sDisplayWin.width  = (UART_EXT_CMD_MS_DAT8<<8) | UART_EXT_CMD_MS_DAT9;
                sDisplayWin.height = (UART_EXT_CMD_MS_DAT10<<8) | UART_EXT_CMD_MS_DAT11;
                msAPIWinSetDisplayWin(UART_EXT_CMD_MS_DAT3, &sDisplayWin);
                msAPIWinReSetup(UART_EXT_CMD_MS_DAT3, TRUE, TRUE);
                DB_PRINT("[DB_PxP] Window Re-size: Win:%d, x:%d, y:%d, W:%d, H:%d \n", UART_EXT_CMD_MS_DAT3, sDisplayWin.x, sDisplayWin.y, sDisplayWin.width, sDisplayWin.height);
            }
            break;

        case 0x08: // Win Switch port
            {
                msAPIWinFreerun(UART_EXT_CMD_MS_DAT3, TRUE);
                msAPIWinSwitchPort(UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
                DB_PRINT("[DB_PxP] Window SwitchPort: Win:%d, P:%d\n", UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4);
            }
            break;

        case 0x09: // Win source swap
            {
                BYTE Source1 = MapWin2Port(UART_EXT_CMD_MS_DAT3);
                BYTE Source2 = MapWin2Port(UART_EXT_CMD_MS_DAT4);
                msAPIWinFreerun(UART_EXT_CMD_MS_DAT3, TRUE);
                msAPIWinFreerun(UART_EXT_CMD_MS_DAT4, TRUE);
                msAPIWinSwitchPort(UART_EXT_CMD_MS_DAT3,Source2);
                msAPIWinSwitchPort(UART_EXT_CMD_MS_DAT4,Source1);
                DB_PRINT("[DB_PxP] Window SourceSwap: Win:%d,%d, P:%d,%d\n", UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, Source2, Source1);
            }
            break;

        case 0x0A: // Win Rotate
            {
                msAPIWinSetRotateMode(UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4);
                msAPIWinReSetup(UART_EXT_CMD_MS_DAT3, TRUE, TRUE);
                DB_PRINT("[DB_PxP] Window Rotate: Win:%d, R:%d\n", UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4);
            }
            break;

        case 0x0B: // Win Enable
            {
                msAPIWinEnable(UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5);
                DB_PRINT("[DB_PxP] Window Enable: Win:%d, P:%d, R:%d\n", UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4,UART_EXT_CMD_MS_DAT5);
            }
            break;

        case 0x0C: // Win Disable
            {
                msAPIWinDisable(UART_EXT_CMD_MS_DAT3);
                DB_PRINT("[DB_PxP] Window Disable: Win:%d\n", UART_EXT_CMD_MS_DAT3);
            }
            break;

        case 0x10: // 2 wins
            {
                ST_WINDOW_INFO  DisplayWin1 = {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT};
                ST_WINDOW_INFO  DisplayWin2 = {PANEL_WIDTH/2, 0, PANEL_WIDTH/2, PANEL_HEIGHT};
                BYTE u8IdxWin;

                WAIT_V_OUTPUT_BLANKING_START();
                for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
                {
                    msAPIWinDel(u8IdxWin);
                    msAPIWinFreerun(u8IdxWin, TRUE);
                }

                msAPIWinAdd(DISPLAY_WIN1,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin1);
                msAPIWinAdd(DISPLAY_WIN2,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin2);
                msAPIWinApply();

                DB_PRINT("[DB_PxP] 2 Windows\n");
            }
            break;

        case 0x11: // 4 wins
            {
                ST_WINDOW_INFO  DisplayWin1 = {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin2 = {PANEL_WIDTH/2, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin3 = {0, PANEL_HEIGHT/2, PANEL_WIDTH/2, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin4 = {PANEL_WIDTH/2, PANEL_HEIGHT/2, PANEL_WIDTH/2, PANEL_HEIGHT/2};
                BYTE u8IdxWin;

                WAIT_V_OUTPUT_BLANKING_START();
                for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
                {
                    msAPIWinDel(u8IdxWin);
                    msAPIWinFreerun(u8IdxWin, TRUE);
                }

                msAPIWinAdd(DISPLAY_WIN1,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin1);
                msAPIWinAdd(DISPLAY_WIN2,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin2);
                msAPIWinAdd(DISPLAY_WIN3,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin3);
                msAPIWinAdd(DISPLAY_WIN4,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin4);
                msAPIWinApply();

                DB_PRINT("[DB_PxP] 4 Windows\n");
            }
            break;

        case 0x12: // 8 wins
            {
                ST_WINDOW_INFO  DisplayWin1 = {0, 0, PANEL_WIDTH/4, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin2 = {PANEL_WIDTH/4, 0, PANEL_WIDTH/4, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin3 = {PANEL_WIDTH*2/4, 0, PANEL_WIDTH/4, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin4 = {PANEL_WIDTH*3/4, 0, PANEL_WIDTH/4, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin5 = {0, PANEL_HEIGHT/2, PANEL_WIDTH/4, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin6 = {PANEL_WIDTH/4, PANEL_HEIGHT/2, PANEL_WIDTH/4, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin7 = {PANEL_WIDTH*2/4, PANEL_HEIGHT/2, PANEL_WIDTH/4, PANEL_HEIGHT/2};
                ST_WINDOW_INFO  DisplayWin8 = {PANEL_WIDTH*3/4, PANEL_HEIGHT/2, PANEL_WIDTH/4, PANEL_HEIGHT/2};
                BYTE u8IdxWin;

                WAIT_V_OUTPUT_BLANKING_START();
                for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
                {
                    msAPIWinDel(u8IdxWin);
                    msAPIWinFreerun(u8IdxWin, TRUE);
                }

                msAPIWinAdd(0%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin1);
                msAPIWinAdd(1%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin2);
                msAPIWinAdd(2%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin3);
                msAPIWinAdd(3%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin4);
                msAPIWinAdd(4%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin5);
                msAPIWinAdd(5%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin6);
                msAPIWinAdd(6%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin7);
                msAPIWinAdd(7%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin8);
                msAPIWinApply();

                DB_PRINT("[DB_PxP] 8 Windows\n");
            }
            break;

        case 0x13: // 8 wins
            {
                ST_WINDOW_INFO  DisplayWin1 = {PANEL_WIDTH*0/16, PANEL_HEIGHT*0/8, PANEL_WIDTH*8/16, PANEL_HEIGHT*8/8};
                ST_WINDOW_INFO  DisplayWin2 = {PANEL_WIDTH*2/16, PANEL_HEIGHT*2/8, PANEL_WIDTH*4/16, PANEL_HEIGHT*4/8};
                ST_WINDOW_INFO  DisplayWin3 = {PANEL_WIDTH*1/16, PANEL_HEIGHT*1/8, PANEL_WIDTH*6/16, PANEL_HEIGHT*6/8};
                ST_WINDOW_INFO  DisplayWin4 = {PANEL_WIDTH*3/16, PANEL_HEIGHT*3/8, PANEL_WIDTH*2/16, PANEL_HEIGHT*2/8};
                ST_WINDOW_INFO  DisplayWin5 = {PANEL_WIDTH*8/16, PANEL_HEIGHT*0/8, PANEL_WIDTH*8/16, PANEL_HEIGHT*8/8};
                ST_WINDOW_INFO  DisplayWin6 = {PANEL_WIDTH*10/16, PANEL_HEIGHT*2/8, PANEL_WIDTH*4/16, PANEL_HEIGHT*4/8};
                ST_WINDOW_INFO  DisplayWin7 = {PANEL_WIDTH*9/16, PANEL_HEIGHT*1/8, PANEL_WIDTH*6/16, PANEL_HEIGHT*6/8};
                ST_WINDOW_INFO  DisplayWin8 = {PANEL_WIDTH*11/16, PANEL_HEIGHT*3/8, PANEL_WIDTH*2/16, PANEL_HEIGHT*2/8};
                BYTE u8IdxWin;

                WAIT_V_OUTPUT_BLANKING_START();
                for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
                {
                    msAPIWinDel(u8IdxWin);
                    msAPIWinFreerun(u8IdxWin, TRUE);
                }

                msAPIWinAdd(0%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin1);
                msAPIWinAdd(1%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin2);
                msAPIWinAdd(2%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin3);
                msAPIWinAdd(3%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin4);
                msAPIWinAdd(4%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin5);
                msAPIWinAdd(5%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin6);
                msAPIWinAdd(6%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin7);
                msAPIWinAdd(7%DISPLAY_MAX_NUMS,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin8);
                msAPIWinApply();

                DB_PRINT("[DB_PxP] 8 Windows\n");
            }
            break;

        case 0x14: // 1 wins
            {
                ST_WINDOW_INFO  DisplayWin1 = {0, 0, PANEL_WIDTH, PANEL_HEIGHT};
                BYTE u8IdxWin;

                WAIT_V_OUTPUT_BLANKING_START();
                for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
                {
                    msAPIWinDel(u8IdxWin);
                    msAPIWinFreerun(u8IdxWin, TRUE);
                }

                msAPIWinAdd(DISPLAY_WIN1,  UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, DisplayWin1);
                msAPIWinApply();

                DB_PRINT("[DB_PxP] 1 Window\n");
            }
            break;

        case 0x15:  // 6 wins
            {
                ST_WINDOW_INFO  DisplayWin1 = {                0, 0, PANEL_WIDTH/4, (PANEL_HEIGHT/8)*7};
                ST_WINDOW_INFO  DisplayWin2 = {    PANEL_WIDTH/4, 0, PANEL_WIDTH/4, PANEL_HEIGHT};
                ST_WINDOW_INFO  DisplayWin3 = {    PANEL_WIDTH/2, 0, PANEL_WIDTH/4, PANEL_HEIGHT};
                ST_WINDOW_INFO  DisplayWin4 = {(PANEL_WIDTH/4)*3, 0, PANEL_WIDTH/4, (PANEL_HEIGHT/8)*7};
                ST_WINDOW_INFO  DisplayWin5 = {                0, (PANEL_HEIGHT/8)*7, PANEL_WIDTH/4, PANEL_HEIGHT/8};
                ST_WINDOW_INFO  DisplayWin6 = {(PANEL_WIDTH/4)*3, (PANEL_HEIGHT/8)*7, PANEL_WIDTH/4, PANEL_HEIGHT/8};

                BYTE u8IdxWin;
                WAIT_V_OUTPUT_BLANKING_START();
                for( u8IdxWin = DISPLAY_WIN1 ; u8IdxWin < DISPLAY_MAX_NUMS ; u8IdxWin++ )
                {
                    msAPIWinDel(u8IdxWin);
                    msAPIWinFreerun(u8IdxWin, TRUE);
                }
                msAPIWinAdd(DISPLAY_WIN1, 1, 0, DisplayWin1);
                msAPIWinAdd(DISPLAY_WIN2, 3, 0, DisplayWin2);
                msAPIWinAdd(DISPLAY_WIN3, 2, 0, DisplayWin3);
                msAPIWinAdd(DISPLAY_WIN4, 4, 0, DisplayWin4);
                if( UART_EXT_CMD_MS_DAT3 )
                {
                    msAPIWinAdd(4%DISPLAY_MAX_NUMS, 5, 0, DisplayWin5);
                    msAPIWinAdd(5%DISPLAY_MAX_NUMS, 0, 0, DisplayWin6);
                }
                msAPIWinApply();
                DB_PRINT("[DB_PxP] 6 Windows\n");
            }
            break;

        case 0xF0:
            {
                DB_PRINT("Fading Window Start\n");
                APPWindow_FadingWindow();
                DB_PRINT("Fading Window End\n");
            }
            break;

        case 0xF1:
            {
                extern BYTE g_bAPPWindow_FlyWin;
                g_bAPPWindow_FlyWin = UART_EXT_CMD_MS_DAT3;
            }
            break;

        case 0xF2:
            {
                ST_WINDOW_INFO sCapWindow = { (PANEL_WIDTH/2-320), (PANEL_HEIGHT/2-240), 640, 480};
                ST_WINDOW_INFO sDispWindow = { 0, 0, PANEL_WIDTH, PANEL_HEIGHT };
                msAPIWinAdd(UART_EXT_CMD_MS_DAT3, Input_Nums, 0, sDispWindow);
                msAPIWinSetDirectDisplay(UART_EXT_CMD_MS_DAT3, TRUE, &sCapWindow);
                msAPIWinApply();
                break;
            }

        case 0xF3:
            APPWindow_DrawBMP(UART_EXT_CMD_MS_DAT3);
            break;
		
		case 0xF4:
			APPWindow_DrawJPG(UART_EXT_CMD_MS_DAT3);
			break;
			
        case 0xF5:
            msAPIWinSetDirectDisplay(UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, NULL);
            break;

        case 0xF6:
            msAPISrcFakeDualCfg(UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5);
            break;

        case 0xF7:
            msAPISrcSetType(UART_EXT_CMD_MS_DAT3, (EN_API_SRC_TYPE)UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5);
            break;

        case 0xF8:
            msAPIWinSetMonoDisplay(UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4);
            break;

        case 0xF9:
            msAPISrcOfflineDetectPort(UART_EXT_CMD_MS_DAT3);
            break;

        case 0xFA:
            DB_PRINT("Offline Status:%d\n", msAPISrcOfflineDetectStatus());
            break;

        case 0xFB:
            {
                ST_FAKE_CAP_WIN_INFO sUserFakeCapWin;
                BOOL bcapWinEnable;
                BYTE u8IdxWin;

                u8IdxWin = UART_EXT_CMD_MS_DAT3;
                bcapWinEnable = UART_EXT_CMD_MS_DAT4;
                sUserFakeCapWin.x = (UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6;
                sUserFakeCapWin.y = (UART_EXT_CMD_MS_DAT7<<8) | UART_EXT_CMD_MS_DAT8;
                sUserFakeCapWin.width = (UART_EXT_CMD_MS_DAT9<<8) | UART_EXT_CMD_MS_DAT10;
                sUserFakeCapWin.height= (UART_EXT_CMD_MS_DAT11<<8) | UART_EXT_CMD_MS_DAT12;

                if( sUserFakeCapWin.x & _BIT15 )
                    sUserFakeCapWin.x = 0 - (sUserFakeCapWin.x & 0x7FFF);
                if( sUserFakeCapWin.y & _BIT15 )
                    sUserFakeCapWin.y = 0 - (sUserFakeCapWin.y & 0x7FFF);

                msAPIWinSetMemFakeCapWin(u8IdxWin, sUserFakeCapWin, bcapWinEnable);
                msAPIWinReSetup(u8IdxWin, FALSE, FALSE);

                if(bcapWinEnable)
                {
                    DB_PRINT("Enable fake capture window\n");
                    DB_PRINT("Fake capture window: win:%d, x:%d, y:%d, w:%d, h:%d\n", u8IdxWin, sUserFakeCapWin.x, sUserFakeCapWin.y, sUserFakeCapWin.width, sUserFakeCapWin.height);
                }
                else
                {
                    DB_PRINT("Disable fake capture window\n");
                }
            }
            break;

#if (CHIP_ID == MST9U2)
        case 0xFF:
            msAPIWinDisplayOnTop(UART_EXT_CMD_MS_DAT3);
            break;
#endif

        default:
            break;
    }
}

static void TestCommand_ScalingFilter(void)
{
    switch (UART_EXT_CMD_MS_DAT2)
    {
        case 0x00:
            DB_PRINT("[DB_SF] 0x00: List of commands\n");
            DB_PRINT("    Format - 04 COMMAND [WINDOW] [H/V] [MODE] [SRAM]\n");
            DB_PRINT("  [WINDOW] - 00~07\n");
            DB_PRINT("     [H/V] - 00 for H, 01 for V\n");
            DB_PRINT("    [MODE] - 00~04 stand for BYPASS, BILINEAR\n");
            DB_PRINT("               BICUBIC, Y_6_TAP and Y_8_TAP\n");
            DB_PRINT("    [SRAM] - 00~06, 06 for non-used\n");
            DB_PRINT("*Note* Actually the [SRAM] parameter will be\n");
            DB_PRINT("       ignored when using BYPASS and BILINEAR mode\n");
            DB_PRINT("[DB_SF] 0x01: Set_Chroma_ScalingFilter\n");
            DB_PRINT("[DB_SF] 0x02: Set_Luma_ScalingFilter\n");
            break;
        case 0x01:
            DB_PRINT("[DB_SF] Set H or V chroma Scaling filter\n");
            msDrvSetChromaScalingFilter(UART_EXT_CMD_MS_DAT3, (EN_DIRECTION)UART_EXT_CMD_MS_DAT4,
                (EN_SCALING_FILTER_MODE)UART_EXT_CMD_MS_DAT5, (EN_SCALING_FILTER_SRAM)UART_EXT_CMD_MS_DAT6);
            break;
        case 0x02:
            DB_PRINT("[DB_SF] Set H or V luma Scaling filter\n");
            msDrvSetLumaScalingFilter(UART_EXT_CMD_MS_DAT3, (EN_DIRECTION)UART_EXT_CMD_MS_DAT4,
                (EN_SCALING_FILTER_MODE)UART_EXT_CMD_MS_DAT5, (EN_SCALING_FILTER_SRAM)UART_EXT_CMD_MS_DAT6);
            break;
        default:
            break;
    }
}

#if ENABLE_MENULOAD
static void TestCommand_ScalerMLoad(void)
{
    BYTE ucPara1 = UART_EXT_CMD_MS_DAT3;
    BYTE ucPara2 = UART_EXT_CMD_MS_DAT4;
    BYTE ucPara3 = UART_EXT_CMD_MS_DAT5;

#if ENABLE_BITMASK_MENULOAD
    BYTE ucPara4 = UART_EXT_CMD_MS_DAT6;
#endif

	switch( UART_EXT_CMD_MS_DAT2 )
    {
   		case 0x00:
            {
                DWORD addrTemp[35]=
                {
                    0x95102F30, 0x95102F32, 0x95102F34, 0x95102F36, 0x95102F38, 0x95102F3A, 0x95102F3C,
                    0x95102F3E, 0x95102F40, 0x95102F42, 0x95102F44, 0x9F102B30, 0x9F102B32, 0x95102F46,
                    0x95102F48, 0x95102F4A, 0x95102F4C, 0x95102F4E, 0x95102B30, 0x95102B32, 0x95102B34,
                    0x95102B36, 0x95102B38, 0x95102B3A, 0x95102B3C, 0x95102B3E, 0x95102B40, 0x95102B42,
                    0x95102F50, 0x95102B44, 0x95102B46, 0x95102B48, 0x95102B4A, 0x95102B4C, 0x95102B4E
                };
                WORD valueTemp[35]=
                {
                    0x3575, 0x3576, 0x3577, 0x3578, 0x3579, 0x3580, 0x3581, 0x3582, 0x3583, 0x3584,
                    0x3585, 0x8643, 0x7652, 0x3586, 0x3587, 0x3588, 0x3589, 0x3590, 0x5555, 0x6666,
                    0x8888, 0x9999, 0x0000, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6783, 0x6666,
                    0x7777, 0x8888, 0x9999, 0x7654, 0x5324,
                };
                WORD valueMask[35]=
                {
                    0xFFFF, 0x0FFF, 0xFFFF, 0xFFF0, 0xFFFF, 0x0FFF, 0xFF0F, 0xF0FF, 0xFF0F, 0xFFFF,
                    0xF0FF, 0xFFFF, 0xFFFF, 0xF0F0, 0xFFFF, 0xFFFF, 0xFFFF, 0x0FFF, 0xFFFF, 0xFFFF,
                    0xFFFF, 0x0FFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0FFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
                    0x0FFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0FFF,
                };
                MApi_XC_MLoad_WriteCmds_And_Fire(addrTemp,valueTemp, valueMask ,35);
            }
            DB_PRINT(" =====MApi_XC_MLoad_WriteCmds_And_Fire====\r\n");
            break;

      	case 0x01:
            MApi_XC_MLoad_WriteCmd_And_Fire(0x95102B30,0x7839, 0xFFFF );
            MApi_XC_MLoad_WriteCmd_And_Fire(0x95102F00+ucPara1,((ucPara2<<8)|ucPara2), 0xFFFF );
            MApi_XC_MLoad_WriteCmd_And_Fire(0x95102B00+ucPara1,((ucPara3<<8)|ucPara3), 0xFFFF );
            DB_PRINT(" =====MApi_XC_MLoad_WriteCmd_And_Fire_2====\r\n");
            break;

        case 0x02:
            _MLOAD_ENTRY();
            {
                BYTE retryc=0;
                for(retryc=0;retryc<ucPara3;retryc++)
                {
                    MApi_XC_MLoad_WriteCmd(0x95102F00+ucPara1+(retryc*4),((ucPara2<<8)|ucPara2), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95102B00+ucPara1+(retryc*4),(((ucPara2<<8)|ucPara2)+0x07), 0xFFFF );
                }
            }
            MApi_XC_MLoad_Fire(TRUE );
            _MLOAD_RETURN();
            DB_PRINT(" =====MenuLoad_Status====\r\n");
            break;

      	case 0x03:
            _MLOAD_ENTRY();
            {
                {
                    MApi_XC_MLoad_WriteCmd(0x95102F32,((ucPara1<<8)|ucPara1), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95102F30,(((ucPara1<<8)|ucPara1)+0x07), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95102B30,(((ucPara2<<8)|ucPara2)+0x07), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95102B32,(((ucPara2<<8)|ucPara2)+0x07), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95102B34,(((ucPara2<<8)|ucPara2)+0x07), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95102B35,(((ucPara2<<8)|ucPara2)+0x07), 0xFFFF );

                    MApi_XC_MLoad_WriteCmd(0x95102D30,(((ucPara3<<8)|ucPara3)+0x07), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95102D32,(((ucPara3<<8)|ucPara3)+0x07), 0xFFFF );

                    MApi_XC_MLoad_WriteCmd(0x95103134,((ucPara2<<8)|ucPara2), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95103135,((ucPara2<<8)|ucPara2), 0xFFFF );

                    MApi_XC_MLoad_WriteCmd(0x95102F34,(((ucPara1<<8)|ucPara1)+9), 0xFFFF );
                    MApi_XC_MLoad_WriteCmd(0x95102F38,(((ucPara1<<8)|ucPara1)+0x03), 0xFFFF );
                }
            }
            MApi_XC_MLoad_Fire(TRUE );
            _MLOAD_RETURN();
            DB_PRINT(" =====MenuLoad_Status====\r\n");
            break;

      	case 0x04:
            MApi_XC_MLoad_WriteComCmd(0x95102B30, 0x3575, 0xFFFF, SCA_FIRE_FIXL0|SCA_FIRE_FIXR1);
            MApi_XC_MLoad_WriteComCmd(0x95102B32, 0x3576, 0x0FFF, SCA_FIRE_FIXL1|SCA_FIRE_FIXR1);
            MApi_XC_MLoad_WriteComCmd(0x95102B34, 0x3577, 0xFFFF, SCA_FIRE_FIXL0|SCA_FIRE_FIXR0);
            MApi_XC_MLoad_WriteComCmd(0x95102B36, 0x3578, 0xFFF0, SCA_FIRE_FIXL0|SCA_FIRE_FIXL1|SCA_FIRE_FIXR1);
            MApi_XC_MLoad_Fire(TRUE );
            DB_PRINT(" =====MApi_XC_MLoad_WriteComCmd====\r\n");
            msWrite2Byte(0x100FD6, msRead2Byte(0x100FD6)|BIT15);
            break;

#if ENABLE_BITMASK_MENULOAD
		case 0x05:
            // BK_102F Sub_BK_10  addr: 0x10  data: (ucPara1<<8)|ucPara2 mask:(ucPara3<<8)|ucPara4)
            MApi_XC_MLoad_BitMask_WriteCmd(0x10102F10, ((ucPara1<<8)|ucPara2), ((ucPara3<<8)|ucPara4));
            MApi_XC_MLoad_BitMask_Fire(TRUE );
            //msWrite2Byte(0x100FD6, msRead2Byte(0x100FD6)|BIT15); //stop R2
            ForceDelay1ms(50);
            msWriteByte(0x102F00, 0x10);
            DB_PRINT("BK_102F Sub_BK_10, addr_h08  data: =%lx \n", msRead2Byte(0x102F10));
            break;

 		case 0x06:
        	// BK_102B Sub_BK_10  addr: 0x10  data: (ucPara1<<8)|ucPara2 mask:(ucPara3<<8)|ucPara4)
            MApi_XC_MLoad_BitMask_WriteCmd(0x10102B10, ((ucPara1<<8)|ucPara2), ((ucPara3<<8)|ucPara4));
            MApi_XC_MLoad_BitMask_Fire(TRUE );
            //msWrite2Byte(0x100FD6, msRead2Byte(0x100FD6)|BIT15); //stop R2
            ForceDelay1ms(50);
            msWriteByte(0x102B00, 0x10);
            DB_PRINT("BK_102B Sub_BK_10, addr_h08  data: =%lx \n", msRead2Byte(0x102B10));
            break;

        case 0x07:
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_06, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_26, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_46, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_66, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_86, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_A6, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_C6, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_E6, ((ucPara1<<8)|ucPara2), 0x0FFF);

            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_08, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_28, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_48, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_68, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_88, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_A8, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_C8, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_E8, ((ucPara1<<8)|ucPara2), 0x0FFF);

            MApi_XC_MLoad_BitMask_Fire(TRUE);

            DB_PRINT("OSD2_06 =%lx \n", msRead2Byte(OSD2_06));
            DB_PRINT("OSD2_26 =%lx \n", msRead2Byte(OSD2_26));
            DB_PRINT("OSD2_46 =%lx \n", msRead2Byte(OSD2_46));
            DB_PRINT("OSD2_66 =%lx \n", msRead2Byte(OSD2_66));
            DB_PRINT("OSD2_86 =%lx \n", msRead2Byte(OSD2_86));
            DB_PRINT("OSD2_A6 =%lx \n", msRead2Byte(OSD2_A6));
            DB_PRINT("OSD2_C6 =%lx \n", msRead2Byte(OSD2_C6));
            DB_PRINT("OSD2_E6 =%lx \n", msRead2Byte(OSD2_E6));
            DB_PRINT("OSD2_08 =%lx \n", msRead2Byte(OSD2_08));
            DB_PRINT("OSD2_28 =%lx \n", msRead2Byte(OSD2_28));
            DB_PRINT("OSD2_48 =%lx \n", msRead2Byte(OSD2_48));
            DB_PRINT("OSD2_68 =%lx \n", msRead2Byte(OSD2_68));
            DB_PRINT("OSD2_88 =%lx \n", msRead2Byte(OSD2_88));
            DB_PRINT("OSD2_A8 =%lx \n", msRead2Byte(OSD2_A8));
            DB_PRINT("OSD2_C8 =%lx \n", msRead2Byte(OSD2_C8));
            DB_PRINT("OSD2_E8 =%lx \n", msRead2Byte(OSD2_E8));
            break;

        case 0x08:
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_06, ((ucPara1<<8)|ucPara2), 0x0111);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_26, ((ucPara1<<8)|ucPara2), 0x0333);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_46, ((ucPara1<<8)|ucPara2), 0x0555);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_66, ((ucPara1<<8)|ucPara2), 0x0777);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_86, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_A6, ((ucPara1<<8)|ucPara2), 0x0222);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_C6, ((ucPara1<<8)|ucPara2), 0x0444);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_E6, ((ucPara1<<8)|ucPara2), 0x0666);

            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_08, ((ucPara1<<8)|ucPara2), 0x0111);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_28, ((ucPara1<<8)|ucPara2), 0x0333);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_48, ((ucPara1<<8)|ucPara2), 0x0555);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_68, ((ucPara1<<8)|ucPara2), 0x0777);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_88, ((ucPara1<<8)|ucPara2), 0x0FFF);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_A8, ((ucPara1<<8)|ucPara2), 0x0222);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_C8, ((ucPara1<<8)|ucPara2), 0x0444);
            MApi_XC_MLoad_BitMask_WriteSpreadCmd(OSD2_E8, ((ucPara1<<8)|ucPara2), 0x0666);

            MApi_XC_MLoad_BitMask_Fire(TRUE);

            DB_PRINT("OSD2_06 =%lx \n", msRead2Byte(OSD2_06));
            DB_PRINT("OSD2_26 =%lx \n", msRead2Byte(OSD2_26));
            DB_PRINT("OSD2_46 =%lx \n", msRead2Byte(OSD2_46));
            DB_PRINT("OSD2_66 =%lx \n", msRead2Byte(OSD2_66));
            DB_PRINT("OSD2_86 =%lx \n", msRead2Byte(OSD2_86));
            DB_PRINT("OSD2_A6 =%lx \n", msRead2Byte(OSD2_A6));
            DB_PRINT("OSD2_C6 =%lx \n", msRead2Byte(OSD2_C6));
            DB_PRINT("OSD2_E6 =%lx \n", msRead2Byte(OSD2_E6));
            DB_PRINT("OSD2_08 =%lx \n", msRead2Byte(OSD2_08));
            DB_PRINT("OSD2_28 =%lx \n", msRead2Byte(OSD2_28));
            DB_PRINT("OSD2_48 =%lx \n", msRead2Byte(OSD2_48));
            DB_PRINT("OSD2_68 =%lx \n", msRead2Byte(OSD2_68));
            DB_PRINT("OSD2_88 =%lx \n", msRead2Byte(OSD2_88));
            DB_PRINT("OSD2_A8 =%lx \n", msRead2Byte(OSD2_A8));
            DB_PRINT("OSD2_C8 =%lx \n", msRead2Byte(OSD2_C8));
            DB_PRINT("OSD2_E8 =%lx \n", msRead2Byte(OSD2_E8));
            break;
#endif

        default:
            break;
    }
}
#endif

#if (XMODEM_DWNLD_ENABLE)
static void TestCommand_XMODEM(void)
{
    xmodem_mode = XMODEM;
    if (xmodem_file_download() == XMSUCCESS)
    {
        xmodem_write_flash();
    }
}
#endif

static void TestCommand_AdjustOutput(void)
{
    BYTE u8L0RegBackup, u8WinIdx;

    u8L0RegBackup = msReadByte(SC00_02+BANK_SC_M_L0);

    // disable double buffer
    msWriteByte(SC00_02+BANK_SC_M_L0, u8L0RegBackup&(~BIT0));

    WAIT_V_OUTPUT_BLANKING_START();
    for (u8WinIdx = 0; u8WinIdx < DISPLAY_MAX_NUMS; u8WinIdx++)
    {
        msAPI_AdjustOutputHPosition(u8WinIdx, UART_EXT_CMD_MS_DAT2, UART_EXT_CMD_MS_DAT3);
        msAPI_AdjustOutputVPosition(u8WinIdx, UART_EXT_CMD_MS_DAT2, UART_EXT_CMD_MS_DAT3);
    }

    msWriteByte(SC00_02+BANK_SC_M_L0, u8L0RegBackup);
}

#ifdef EXTERNAL_MCU
void TestCommand_Ext_RIU(void)
{
    DWORD u32Reg = ((DWORD)UART_EXT_CMD_MS_DAT3 << 24) | ((DWORD)UART_EXT_CMD_MS_DAT4<<16) | ((DWORD)UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6;
    BYTE u8Value= UART_EXT_CMD_MS_DAT7;

    switch (UART_EXT_CMD_MS_DAT2)
    {
        case 0x00:
                msWriteByte_Ext(u32Reg, u8Value);
               DB_PRINT("[EXT_RIU]Write REG[0x%x] = %x\n",u32Reg, u8Value);

            break;

        case 0x01:
               u8Value = msReadByte_Ext(u32Reg);
               DB_PRINT("[EXT_RIU]Read REG[0x%x] = %x\n",u32Reg, u8Value);
            break;

    }

}
#endif

#if (ENABLE_MWE)
static void TestCommand_MWE( void )
{
    ST_WINDOW_INFO  sMWEWin = {0, 0, PANEL_WIDTH/2, PANEL_HEIGHT/2};

    switch( UART_EXT_CMD_MS_DAT2 )
    {
        case 0x01:
            sMWEWin.x = ((DWORD)UART_EXT_CMD_MS_DAT4<<8 )+ UART_EXT_CMD_MS_DAT5;
            sMWEWin.y = ((DWORD)UART_EXT_CMD_MS_DAT6<<8 )+ UART_EXT_CMD_MS_DAT7;
            msAPIMWEWinConfig(UART_EXT_CMD_MS_DAT3, sMWEWin, UART_EXT_CMD_MS_DAT8);
            break;

    }
}
#endif
#if ENABLE_DAISY_CHAIN
static void TestCommand_DaisyChainRW( void )
{
    switch( UART_EXT_CMD_MS_DAT2 )
    {
        case 0x00:
            DB_PRINT("[DB_DCRW] 0x00: List of Debug Functions\n");
            DB_PRINT("[DB_DCRW] 0x01: msDCWriteBit(AA, B, BP)\n");
            DB_PRINT("[DB_DCRW] 0x02: msDCReadByte(AA)\n");
            DB_PRINT("[DB_DCRW] 0x03: msDCWriteByte(AA, D)\n");
            DB_PRINT("[DB_DCRW] 0x04: msDCWriteByteMask(AA, D, M)\n");
            DB_PRINT("[DB_DCRW] 0x05: msDCRead2Byte(AA)\n");
            DB_PRINT("[DB_DCRW] 0x06: msDCWrite2Byte(AA, DD)\n");
            DB_PRINT("[DB_DCRW] 0x07: msDCWrite2ByteMask(AA, DD, MM)\n");
            DB_PRINT("[DB_DCRW] 0x08: msDCRead3Byte(AA)\n");
            DB_PRINT("[DB_DCRW] 0x09: msDCWrite3Byte(AA, DDD)\n");
            DB_PRINT("[DB_DCRW] 0x0A: msDCWrite3ByteMask(AA, DDD, MMM)\n");
            DB_PRINT("[DB_DCRW] 0x0B: msDCRead4Byte(AA)\n");
            DB_PRINT("[DB_DCRW] 0x0C: msDCWrite4Byte(AA, DDDD)\n");
            DB_PRINT("[DB_DCRW] 0x0D: msDCWrite4ByteMask(AA, DDDD, MMMM)\n");
            break;

        case 0x01:
            {
                msDCWriteBit( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5, UART_EXT_CMD_MS_DAT6 );
                DB_PRINT("msDCWriteBit: A:%x, B:%x, BP:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5, UART_EXT_CMD_MS_DAT6);
            }
            break;

        case 0x02:
            {
                BYTE Reply;
                msDCReadByte( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, &Reply );
                DB_PRINT("msDCReadByte: A:%x, D:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, Reply);
            }
            break;

        case 0x03:
            {
                msDCWriteByte( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5 );
                DB_PRINT("msDCWriteByte: A:%x, D:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5);
            }
            break;

        case 0x04:
            {
                msDCWriteByteMask( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5, UART_EXT_CMD_MS_DAT6 );
                DB_PRINT("msDCWriteByteMask: A:%x, D:%x, M:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, UART_EXT_CMD_MS_DAT5, UART_EXT_CMD_MS_DAT6);
            }
            break;

        case 0x05:
            {
                WORD Reply;
                msDCRead2Byte( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, &Reply );
                DB_PRINT("msDCRead2Byte: A:%x, D:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, Reply);
            }
            break;

        case 0x06:
            {
                msDCWrite2Byte( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6 );
                DB_PRINT("msDCWrite2Byte: A:%x, D:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6);
            }
            break;

        case 0x07:
            {
                msDCWrite2ByteMask( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6 , ((DWORD)UART_EXT_CMD_MS_DAT7<<8) | UART_EXT_CMD_MS_DAT8 );
                DB_PRINT("msDCWrite2ByteMask: A:%x, D:%x, M:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6 , ((DWORD)UART_EXT_CMD_MS_DAT7<<8) | UART_EXT_CMD_MS_DAT8);
            }
            break;

        case 0x08:
            {
                DWORD Reply;
                msDCRead3Byte( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, &Reply );
                DB_PRINT("msDCRead3Byte: A:%x, D:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, Reply);
            }
            break;

        case 0x09:
            {
                msDCWrite3Byte( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<16) | (UART_EXT_CMD_MS_DAT6<<8)|UART_EXT_CMD_MS_DAT7 );
                DB_PRINT("msDCWrite3Byte: A:%x, D:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<16) | (UART_EXT_CMD_MS_DAT6<<8)|UART_EXT_CMD_MS_DAT7);
            }
            break;

        case 0x0A:
            {
                msDCWrite3ByteMask( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4,((DWORD)UART_EXT_CMD_MS_DAT5<<16) | (UART_EXT_CMD_MS_DAT6<<8)|UART_EXT_CMD_MS_DAT7 , ((DWORD)UART_EXT_CMD_MS_DAT8<<16) | (UART_EXT_CMD_MS_DAT9<<8)|UART_EXT_CMD_MS_DAT10 );
                DB_PRINT("msDCWrite3ByteMask: A:%x, D:%x, M:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<16) | (UART_EXT_CMD_MS_DAT6<<8)|UART_EXT_CMD_MS_DAT7 , ((DWORD)UART_EXT_CMD_MS_DAT8<<16) | (UART_EXT_CMD_MS_DAT9<<8)|UART_EXT_CMD_MS_DAT10);
            }
            break;

        case 0x0B:
            {
                DWORD Reply;
                msDCRead4Byte( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, &Reply );
                DB_PRINT("msDCRead4Byte: A:%x, D:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, Reply);
            }
            break;

        case 0x0C:
            {
                msDCWrite4Byte( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<24) | (UART_EXT_CMD_MS_DAT6<<16)|(UART_EXT_CMD_MS_DAT7<<8)| UART_EXT_CMD_MS_DAT8);
                DB_PRINT("msDCWrite4Byte: A:%x, D:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<24) | (UART_EXT_CMD_MS_DAT6<<16)|(UART_EXT_CMD_MS_DAT7<<8)| UART_EXT_CMD_MS_DAT8);
            }
            break;

        case 0x0D:
            {
                msDCWrite4ByteMask( ((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<24) | (UART_EXT_CMD_MS_DAT6<<16)|(UART_EXT_CMD_MS_DAT7<<8)| UART_EXT_CMD_MS_DAT8 , ((DWORD)UART_EXT_CMD_MS_DAT9<<24) | (UART_EXT_CMD_MS_DAT10<<16)|(UART_EXT_CMD_MS_DAT11<<8)| UART_EXT_CMD_MS_DAT12 );
                DB_PRINT("msDCWrite4ByteMask: A:%x, D:%x, M:%x\r\n",((DWORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((DWORD)UART_EXT_CMD_MS_DAT5<<24) | (UART_EXT_CMD_MS_DAT6<<16)|(UART_EXT_CMD_MS_DAT7<<8)| UART_EXT_CMD_MS_DAT8  , ((DWORD)UART_EXT_CMD_MS_DAT9<<24) | (UART_EXT_CMD_MS_DAT10<<16)|(UART_EXT_CMD_MS_DAT11<<8)| UART_EXT_CMD_MS_DAT12);
            }
            break;

        default:
            break;
    }
}
#endif

#endif //ENABLE_MSTV_UART_DEBUG

static void TestCommand_DrawOSD( void )
{
    switch( UART_EXT_CMD_MS_DAT2 )
    {
        case 0x00:
            {
                DB_PRINT("Test Draw OSD \n");
            }
            break;

        case 0x01:
            {
                extern BYTE xdata g_u8MenuPageIndex;
                extern BYTE xdata g_u8MenuItemIndex;
                extern BYTE xdata g_u8MenuTempValue;
                //extern Bool ExecuteKeyEvent(MenuItemActionType enMenuAction);
                extern void DrawOsdMenu(Bool ShowMenu);

                if (UART_EXT_CMD_MS_DAT3<MAX_MENU)
                {
                    g_u8MenuPageIndex=UART_EXT_CMD_MS_DAT3;
                    g_u8MenuItemIndex=0;
                    g_u8MenuTempValue=0;

                    DB_PRINT("Test Draw OSD Item << %d >> \n", g_u8MenuPageIndex);
                    //ExecuteKeyEvent(MIA_REDRAW_MENU);
                    DrawOsdMenu(TRUE);
                }
            }
            break;

        case 0x02:
            {
                extern void Menu_Hide(void);
                DB_PRINT("Test Draw OSD Item : Menu_Hide \n");
                Menu_Hide();
            }
            break;

        case 0x03:
            {
                extern void Menu_Show(void);
                DB_PRINT("Test Draw OSD Item : Menu_Show \n");
                Menu_Show();
            }
            break;

        case 0x04:
        #if (ZUI)
            MApp_ZUI_API_DoRotate((OSDPAGE_RotateAngle)UART_EXT_CMD_MS_DAT3, TRUE);
        #endif
            break;
    #if ENABLE_DEMURA_FUNCTION
        case 0x05:
            {
                DB_PRINT("DeMuraMeasureMode <<Standby>> \n");
                extern BYTE xdata g_u8MenuPageIndex;
                extern void DrawOsdMenu(Bool ShowMenu);
                USER_PREF_DEMURA_MEASURE_COMMAND=UART_EXT_CMD_MS_DAT3;
                SET_DEMURA_MEASURE_STANDBY_FLAG();
                g_u8MenuPageIndex=MENU_DEMURA_MEASURE_MODE;
                DrawOsdMenu(TRUE);
            }
            break;

        case 0x06:
            {
                DB_PRINT("DeMuraMeasureMode <<Enter>> \n");
                extern BYTE xdata g_u8MenuPageIndex;
                extern void DrawOsdMenu(Bool ShowMenu);
                USER_PREF_DEMURA_MEASURE_COMMAND=UART_EXT_CMD_MS_DAT3;
                CLR_DEMURA_MEASURE_STANDBY_FLAG();
                g_u8MenuPageIndex=MENU_DEMURA_MEASURE_MODE;
                DrawOsdMenu(TRUE);
            }
            break;

        case 0x07:
            {
                DB_PRINT("DeMuraMeasureMode <<Close>> \n");
                USER_PREF_DEMURA_MEASURE_COMMAND=0;
                CloseDeMuraMeasureMode();
            }
            break;

		case 0x08:
			{	
				BYTE u8Count;
				CurveClass HCurve, VCurve;
								
                DB_PRINT("Test Draw Gradient Windows and Curves \n");
				for (u8Count = OSD_WIN0; u8Count <= OSD_WIN7; u8Count++)
                	drvOSD_SetWndOn(u8Count, FALSE);
				msWriteByteMask(OSD1_00, 0x00, 0x03);	// subbank0
				drvOSD_ClrOSD_SRAM();
				
				msWriteByteMask(OSD1_03, 0xFF, 0x08 );	// enable test pattern mode

				drvOSD_SetWndHPosition(OSD_WIN0, PANEL_WIDTH/4, PANEL_WIDTH*3/4);
            	drvOSD_SetWndVPosition(OSD_WIN0, PANEL_HEIGHT/4, PANEL_HEIGHT*3/4);
				drvOSD_SetWndCtrl(OSD_WIN0, OSD_WND_CTRL0, OWC0_FP_MODE, OWC0MASK_ALL);
				drvOSD_SetWndOn(OSD_WIN0, TRUE);

				drvOSD_SetTGenHPosition(OSD_TG0, PANEL_WIDTH/4, PANEL_WIDTH*3/4);
				drvOSD_SetTGenVPosition(OSD_TG0, PANEL_HEIGHT/4, PANEL_HEIGHT*3/4);
				drvOSD_SetTGenFixColor(OSD_TG0, 0, 0, 0);

				switch (UART_EXT_CMD_MS_DAT3)
				{
					case 0x00:
					{
						HCurve.u8PixelInit      = 0;
	                    HCurve.u8DeltaInit      = 20;
	                    HCurve.u8Step           = 15;
	                    HCurve.bDataSign        = 0;
	                    HCurve.bDeltaSign       = 0;
	                    HCurve.u8Delta          = 0;
	                    HCurve.u16Length0       = 0;
	                    HCurve.u16Length1       = 0;
	                    HCurve.bOffsetSign      = 0;
	                    HCurve.bOverflowEnable  = 0;
	                    HCurve.bDotEnable       = 0;
	                    HCurve.u8CurveMode      = 3;

	                    VCurve.u8PixelInit      = 0;
	                    VCurve.u8DeltaInit      = 0;
	                    VCurve.u8Step           = 0;
	                    VCurve.bDataSign        = 0;
	                    VCurve.bDeltaSign       = 0;
	                    VCurve.u8Delta          = 0;
	                    VCurve.u16Length0       = 0;
	                    VCurve.u16Length1       = 0;
	                    VCurve.bOffsetSign      = 0;
	                    VCurve.bOverflowEnable  = 0;
	                    VCurve.bDotEnable       = 0;
	                    VCurve.u8CurveMode      = 3;

						drvOSD_SetTgenCurve(OSD_TG0, UART_EXT_CMD_MS_DAT4, OSD_GD_EG0, 0, &HCurve, &VCurve);
					}
						break;

					case 0x01:
					{
						HCurve.u8PixelInit      = 0;
	                    HCurve.u8DeltaInit      = 20;
	                    HCurve.u8Step           = 15;
	                    HCurve.bDataSign        = 0;
	                    HCurve.bDeltaSign       = 0;
	                    HCurve.u8Delta          = 0;
	                    HCurve.u16Length0       = 0;
	                    HCurve.u16Length1       = 0;
	                    HCurve.bOffsetSign      = 0;
	                    HCurve.bOverflowEnable  = 1;
	                    HCurve.bDotEnable       = 0;
	                    HCurve.u8CurveMode      = 3;

	                    VCurve.u8PixelInit      = 0;
	                    VCurve.u8DeltaInit      = 0;
	                    VCurve.u8Step           = 0;
	                    VCurve.bDataSign        = 0;
	                    VCurve.bDeltaSign       = 0;
	                    VCurve.u8Delta          = 0;
	                    VCurve.u16Length0       = 0;
	                    VCurve.u16Length1       = 0;
	                    VCurve.bOffsetSign      = 0;
	                    VCurve.bOverflowEnable  = 0;
	                    VCurve.bDotEnable       = 0;
	                    VCurve.u8CurveMode      = 3;
						
						drvOSD_SetTgenCurve(OSD_TG0, UART_EXT_CMD_MS_DAT4, OSD_GD_EG0, 0, &HCurve, &VCurve);
					}
						break;

					default:
						break;
				}
				
				drvOSD_SetTGenOn(OSD_TG0, TRUE);
			}
			break;
			
    #endif
        default:
            break;
    }
}

#if ENABLE_DP_INPUT
static void TestCommand_DisplayPort( void )
{
  #if ENABLE_DP_OUTPUT
    BYTE xdata RdData[16];
    BYTE xdata WdData[16]={0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F};
  #endif
    switch( UART_EXT_CMD_MS_DAT2 )
    {
        case 0x00:
            DB_PRINT("Test DisplayPort \n");
            break;
        case 0x04:      //Test Ausio   Extend Command ==> 06 04
            //DPRxSetAUPLL(0);
            break;
    #if ENABLE_DP_OUTPUT
        case 0x08:      //Test eDPTX   Extend Command ==> 06 08
            DPTxAuxDPCDReadByte(0,(UART_EXT_CMD_MS_DAT3<<8)+UART_EXT_CMD_MS_DAT4,RdData);
            break;
        case 0x18:      //Test eDPTX   Extend Command ==> 06 18
            DPTxAuxDPCDReadByte(1,(UART_EXT_CMD_MS_DAT3<<8)+UART_EXT_CMD_MS_DAT4,RdData);
            break;
        case 0x09:      //Test eDPTX   Extend Command ==> 06 09
            DPTxAuxDPCDReadBytes(0,(UART_EXT_CMD_MS_DAT3<<8)+UART_EXT_CMD_MS_DAT4,UART_EXT_CMD_MS_DAT5,RdData);
            break;
        case 0x19:      //Test eDPTX    Extend Command ==> 06 19
            DPTxAuxDPCDReadBytes(1,(UART_EXT_CMD_MS_DAT3<<8)+UART_EXT_CMD_MS_DAT4,UART_EXT_CMD_MS_DAT5,RdData);
            break;
        case 0x0A:      //Test eDPTX   Extend Command ==> 06 0A
            DPTxAuxDPCDWriteByte(0,(UART_EXT_CMD_MS_DAT3<<8)+UART_EXT_CMD_MS_DAT4,UART_EXT_CMD_MS_DAT5);
            break;
        case 0x1A:      //Test eDPTX   Extend Command ==> 06 1A
            DPTxAuxDPCDWriteByte(1,(UART_EXT_CMD_MS_DAT3<<8)+UART_EXT_CMD_MS_DAT4,UART_EXT_CMD_MS_DAT5);
            break;
        case 0x0B:      //Test eDPTX   Extend Command ==> 06 0B
            DPTxAuxDPCDWriteBytes(0,(UART_EXT_CMD_MS_DAT3<<8)+UART_EXT_CMD_MS_DAT4,UART_EXT_CMD_MS_DAT5,WdData);
            break;
        case 0x1B:      //Test eDPTX   Extend Command ==> 06 1B
            DPTxAuxDPCDWriteBytes(1,(UART_EXT_CMD_MS_DAT3<<8)+UART_EXT_CMD_MS_DAT4,UART_EXT_CMD_MS_DAT5,WdData);
            break;
        case 0x77:      //Test eDPTX   Extend Command ==> 06 77
            DPTxTraining(0,UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
            break;
        case 0x87:      //Test eDPTX Extend Command ==> 06 87
            DPTxTraining(1,UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
            break;
#if eDPTXHBR2PortEn
        case 0x86:      //Test eDPTX  Extend Command ==> 06 86
            DPTxHBR2SetSwPre(UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4,UART_EXT_CMD_MS_DAT5,UART_EXT_CMD_MS_DAT6);
            DB_PRINT("HBR2 LinkRate %x\r\n",UART_EXT_CMD_MS_DAT3);
            DB_PRINT("HBR2 Set SwLv %d\r\n",UART_EXT_CMD_MS_DAT4);
            DB_PRINT("HBR2 Set PreLv %d\r\n",UART_EXT_CMD_MS_DAT5);
            break;
        case 0x80:      //Test eDPTX  Extend Command ==> 06 80
            DPTxHBR2DRVMAINTune(UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
            break;
        case 0x81:      //Test eDPTX  Extend Command ==> 06 81
            DPTxHBR2DRVPRETune(UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
            break;
        case 0x82:      //Test eDPTX  Extend Command ==> 06 82
            DPTxHBR2PREDRVMAINTune(UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
            break;
        case 0x83:      //Test eDPTX  Extend Command ==> 06 83
            DPTxHBR2PREDRVPRETune(UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
            break;
#else
        case 0x75:      //Test eDPTX  Extend Command ==> 06 75
            DPTxSwingTune(0x80,UART_EXT_CMD_MS_DAT3);
            break;
        case 0x76:      //Test eDPTX  Extend Command ==> 06 76
            DPTxPreEmphasisTune(0x80,UART_EXT_CMD_MS_DAT3);
            break;
#endif
        case 0x50:      //Test eDPTX Extend Command ==> 06 50 portnum
            DPTxReadEDID(UART_EXT_CMD_MS_DAT3);
            break;
        case 0x55:      //Test eDPTX Extend Command ==> 06 55
            DPTxPGSyncRst();
            break;
        case 0x44:      //Test eDPTX Extend Command ==> 06 44
            DPTxSSCEnable(UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
            break;
        case 0x33:      //Test eDPTX Extend Command ==> 06 33
            DPTxATSTPrintErrCNT(UART_EXT_CMD_MS_DAT3&0x01);
            break;
    #endif
        default:
            break;
    }
}
#endif

#if ENABLE_OSDC_OSDD
void TestCommand_OSDC(void)
{
    //sOSDC_SIZE sOsdc_size, sOsdd_size;

    switch( UART_EXT_CMD_MS_DAT2 )
    {
        case 0x00:
            msDrvOSDBlend_Init(SOSD_SOURCE);
            break;

        case 0x01:
            msDrvOSDBlend_Enable((BOOL) UART_EXT_CMD_MS_DAT3);
            break;
    #if 0
        case 0x02:
            sOsdc_size.u16width = ((WORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4;
            sOsdc_size.u16hiegh = ((WORD)UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6;
            sOsdd_size.u16width = ((WORD)UART_EXT_CMD_MS_DAT7<<8) | UART_EXT_CMD_MS_DAT8;
            sOsdd_size.u16hiegh = ((WORD)UART_EXT_CMD_MS_DAT9<<8) | UART_EXT_CMD_MS_DAT10;
            DB_PRINT("[DB_OSDC] osdc_with=%d, osdc_hiegh=%d, osdd_width=%d, osdd_hiegh=%d\n",sOsdc_size.u16width, sOsdc_size.u16hiegh, sOsdd_size.u16width, sOsdd_size.u16hiegh);
            msDrvOSDBlend_WindowConfig(sOsdc_size, sOsdd_size);
            break;
    #endif
        case 0x03:
            msDrvOSDBlend_WindowPos( (((WORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4), (((WORD)UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6) );
            msDrvOSDBlend_WindowReconfig();
            break;

        case 0x04:
            msDrvOSDBlend_WindowPosRatio(UART_EXT_CMD_MS_DAT3, UART_EXT_CMD_MS_DAT4);
            msDrvOSDBlend_WindowReconfig();
            break;

        case 0x05:
            msDrvOSDBlend_WindowSize(((WORD)UART_EXT_CMD_MS_DAT3<<8) | UART_EXT_CMD_MS_DAT4, ((WORD)UART_EXT_CMD_MS_DAT5<<8) | UART_EXT_CMD_MS_DAT6);
            msDrvOSDBlend_WindowReconfig();
            break;

        default:
            break;
    }
}
#endif


void TestCommand_CursorWindow(void)
{
    switch( UART_EXT_CMD_MS_DAT2 )
    {
    case 0x00: //report RGB max/min data in the defined window range
            DB_PRINT(" Window setting : CMD example 15 00 winIdx:00~03 HST:0300 HEND:0380  VST:0123 VEND:0180\r\n");
            msDrvOp2ReportWindowSetRange(UART_EXT_CMD_MS_DAT3, (UART_EXT_CMD_MS_DAT4<<8|UART_EXT_CMD_MS_DAT5), (UART_EXT_CMD_MS_DAT6<<8|UART_EXT_CMD_MS_DAT7), (UART_EXT_CMD_MS_DAT8<<8|UART_EXT_CMD_MS_DAT9), (UART_EXT_CMD_MS_DAT10<<8|UART_EXT_CMD_MS_DAT11));
            msDrvOp2ReportWindowEnable(UART_EXT_CMD_MS_DAT3 ,ENABLE);
            msDrvOp2ReportWindowGetData(UART_EXT_CMD_MS_DAT3 );
            break;

        case 0x01://enable or disable window report
            DB_PRINT(" WindowEnable: CMD example 15 01 winIdx:00~03  00/01->disable/enable\r\n");
            msDrvOp2ReportWindowEnable(UART_EXT_CMD_MS_DAT3 ,UART_EXT_CMD_MS_DAT4);
            break;

        case 0x02://cursor point RGB value
            DB_PRINT(" Cursor setting : CMD example 15 02 winIdx:00~03 wX:0300 wY:0123\r\n");
            msDrvOp2CursorSetCoordination(UART_EXT_CMD_MS_DAT3, (UART_EXT_CMD_MS_DAT4<<8|UART_EXT_CMD_MS_DAT5), (UART_EXT_CMD_MS_DAT6<<8|UART_EXT_CMD_MS_DAT7));
            msDrvOp2CursorEnable(UART_EXT_CMD_MS_DAT3,  ENABLE);
            msDrvOp2CursorGetRGBValue(UART_EXT_CMD_MS_DAT3);
            break;

        case 0x03://cursor enable or disable
            DB_PRINT(" CursorEnable: CMD example 15 03 winIdx:00~03  00/01->disable/enable\r\n");
            msDrvOp2CursorEnable(UART_EXT_CMD_MS_DAT3,UART_EXT_CMD_MS_DAT4);
            break;

        default:
            break;
    }
}

void TestCommand_MSPI_Flash(void)
{
#if ((CHIP_ID == MST9U4) && (ENABLE_MSPI_FLASH_ACCESS))
switch( UART_EXT_CMD_MS_DAT2 )
{
	case 0x00:
		MDrv_MSPI_Flash_Init(				\
			MSPI0, 							\
			MSPI_ENABLE|MSPI_NOT_RESET, 	\
			MSPI_CLK_DIV_2, 				\
			MSPI_TR_STR_DELAY_1_CYCLE, 		\
			MSPI_TR_END_DELAY_2_CYCLE,		\
			MSPI_B_TO_B_NO_DELAY,			\
			MSPI_W_TO_R_NO_DELAY);
		break;

	case 0x01:
		break;

	default:
		break;
}
#endif
}

extern void msDlcOnOff(BYTE u8WinIdx, Bool bSwitch, BYTE u8MDLCode);
extern XDATA BYTE g_bDLC_On;
extern xdata BOOL  REG_DSC_RUN_Flag;
extern xdata BOOL  REG_DPS_RUN_Flag;
extern void FactoryBlockInit(void);

#if (ENABLE_MWE)
static BYTE u8TempWinSrc = 0;
#endif
    //extern static void console_init(void);

#if ENABLE_PIU_UART_ResetCheck
static void console_initial(void)
{
    //before querying CPU_clock, need to set base address for driver
    MDrv_UART_Init(DEFAULT_UART_DEV, 115200);
    mdrv_uart_connect(DEFAULT_UART_PORT,DEFAULT_UART_DEV);

    DB_PRINT("\nDefault port(0x%x) connecting to 0x%x\n", DEFAULT_UART_PORT, mdrv_uart_get_connection(DEFAULT_UART_PORT));
    DB_PRINT("\n=== console_init ok (%s %s) ===\n\n", __TIME__, __DATE__);
}

void TestUartReset( void )
{
            //2.// UART RESET
            DB_PRINT(".RESET==>%x\r\n",0);
            msWriteByteMask(REG_1009A8, 0, BIT0);
            Delay1ms(10);
            msWriteByteMask(REG_1009A8, BIT0, BIT0);

            mdrv_uart_close(mdrv_uart_open(E_UART_PIU_UART0));
            Delay1ms(5000);
            console_initial();

            #if ENABLE_MSTV_UART_DEBUG
            MDrv_UART_DebugInit(DEFAULT_UART_DEV);
            #endif
            DB_PRINT(".RESET Done==>%x\r\n",1);
}
#endif
BOOL ExecTestCommand( void )
{
    BYTE rtn = TRUE;
    BYTE ucPara1 = (g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
    BYTE ucPara2 = (g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
    BYTE ucPara3 = (g_UartCommand.Buffer[_UART_CMD_INDEX4_]);
    WORD u8count;

    switch( (g_UartCommand.Buffer[_UART_CMD_INDEX1_]) )
    {
        case 0x00:
            {
                extern void mhal_dcache_flush(MS_U32 u32Base, MS_U32 u32Size);
                extern DWORD OldRWFuncCnt;
                DB_PRINT("\n Old reg R/W functios been called(except _msRegs/_scRegs): %d times",OldRWFuncCnt);
                mhal_dcache_flush(0x00,2032);
            }
            break;

#if ENABLE_MSTV_UART_DEBUG
        case 0x01: // test cmd scaler
            TestCommand_Scaler();
            break;

#if Enable_AutoDownloadGamma
        case 0x03:
            TestLoadGamma(ucPara1 ,ucPara2);
            //DB_PRINT("<TestLoadGamma>==%x\r\n",g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
            break;
#endif
#if ENABLE_JUMP
        case 0x04:
            TestCommand_Jump(); // Junp test
            break;
#endif
        case 0x05://Frame buffer and frame bufferless switch
            {
                BYTE enScalerIndex;
                if((ucPara1==FLM_FBL)||(ucPara1==FLM_FBL_FRAMEPLL))
                {
                    g_ScalerInfo[SCL0_MAIN].u8SiFbMode = 0;
                }
                else if((ucPara1==FLM_FB_TUNE_CLK)||(ucPara1==FLM_FB_FAST_LOCK))
                {
                    g_ScalerInfo[SCL0_MAIN].u8SiFbMode = 1;

                }
                for( enScalerIndex=SCL0_SUB ; enScalerIndex < SC_MAX_NUMS ; enScalerIndex++ )
                    msDrvSetTimingGen(enScalerIndex);
                 msDrvSetFrameSyncMode((FSyncMode)ucPara1, SCL0_MAIN);
            }
            break;

        case 0x06:
            {
                SBOOT_Info SbootInfo;
                PM_Info PMInfo;
                MS_CMP_Info APInfo;

                MApp_Imginfo_GetSboot(&SbootInfo);
                MApp_Imginfo_GetPM(&PMInfo);
                MApp_Imginfo_GetAP(&APInfo);

                DB_PRINT("Get Sboot code information:\n", 0);
                DB_PRINT("Sboot Chip ID = %s\n", (BYTE*)(&SbootInfo.chipid));
                DB_PRINT("Sboot Packge = %s\n", (BYTE*)(&SbootInfo.packge));
                DB_PRINT("Sboot Speed = %s\n", (BYTE*)(&SbootInfo.speed));
                DB_PRINT("Sboot Bus Width = %s\n", (BYTE*)(&SbootInfo.bus));
                DB_PRINT("Sboot Compiling Date = %s\n", (BYTE*)(&SbootInfo.u8Date));
                DB_PRINT("Sboot Compiling Time = %s\n", (BYTE*)(&SbootInfo.u8Time));

                DB_PRINT("Get PM code information:\n", 0);
                DB_PRINT("PM Compiling Date = %s\n", (BYTE*)(&PMInfo.u8Date));
                DB_PRINT("PM Compiling Time = %s\n", (BYTE*)(&PMInfo.u8Time));

                DB_PRINT("Get AP code information:\n", 0);
                DB_PRINT("AP Compiling Date = %s\n", (BYTE*)(&APInfo.u8Date));
                DB_PRINT("AP Compiling Time = %s\n", (BYTE*)(&APInfo.u8Time));
            }
            break;

        case 0x20:
            AdjustVideoHue((MENU_ITEM_ACTION)ucPara1);
            DB_PRINT("\r\n CURRENT_HUE (%d)", CURRENT_HUE);
            break;

        case 0x21:
            AdjustVideoSaturation((MENU_ITEM_ACTION)ucPara1);
            DB_PRINT("\r\n CURRENT_SATURATION (%d)", CURRENT_SATURATION);
            DB_PRINT("\r\n USER_PREF_WIN_SEL (%d)", USER_PREF_WIN_SEL);
            break;

        case 0x22:
            while( (ucPara1--) > 0x01 )
            {
                AdjustVideoSaturation((MENU_ITEM_ACTION)1);
                Delay1ms(50);
            }
            break;
#if ((CHIP_ID == MST9U)||(CHIP_ID == MST9U3)) && ENABLE_DAC
        case 0x23:
            {
                  #if  DP_NoInput_Audio
                msDrvAudioSineGen(TRUE,0x3<<8|0xe8, 0); //enable R_FR L_FR GAIN
                DB_PRINT("...2 msDrvAudioSineGen==>%x\r\n",0);
                mapi_DPRxAudioFreeRun(0);
          #endif
            }
            break;
#endif

#if ENABLE_DAC
        case 0x30:
            {
                extern void msDrvAudioSourceSel( AUDIO_SOURCE_SEL src );
                msDrvAudioSourceSel( (AUDIO_SOURCE_SEL)ucPara1 );
                DB_PRINT("...1 msAudioLineout_SourceSel==>%x\r\n",0);

            }
            break;
        case 0x31:
            {
                extern void msAPI_AudioSetDpgaMute( AUDIO_DPGA_SEL eDPGA, MS_BOOL enable );
                msAPI_AudioSetDpgaMute( (AUDIO_DPGA_SEL)ucPara1, TRUE);
                DB_PRINT("...2 msAudioDPGA_Mute==>%x\r\n",0);
            }
            break;
        case 0x32:
            {   //0~0113dB input 0xFFFF~0xFF8F
                msDrvAudioDPGASetVolume( (AUDIO_DPGA_SEL)ucPara1, (AUDIO_CH_SEL)0x02, (ucPara2<<8|ucPara3), VOLUME_FRACTION_MINUS_dot0dB);
                DB_PRINT("...3 msAudioDPGA_Mute==>%x\r\n",0);
            }
            break;
        case 0x33:
            {
		
              //  msDrvAudioSineGen(TRUE,ucPara1<<8|ucPara2,ucPara3); //enable R_FR L_FR GAIN
                DB_PRINT("...2 msDrvAudioSineGen==>%x\r\n",0);
            }
            break;
#if ((CHIP_ID==MST9U) || (CHIP_ID==MST9U2))
        case 0x34:
            msAPI_AudioDdmaDlyTime(((WORD)ucPara1) * 8, (AUDIO_CHANNEL)ucPara2);
            break;
        case 0x35:
            msAPI_AudioDdmaDlyEnable(ucPara1);
            break;
#endif
#if (ENABLE_CP_R2)

        case 0x36:
            msAPI_AudioEQGain1(ucPara1, ucPara2, ucPara3);
            break;

        case 0x37:
            msAPI_AudioEQGain2(ucPara1, ucPara2);
            break;

        case 0x38:
            msAPI_AudioEQEnable(ucPara1);
            break;

        case 0x39:
            msAPI_AudioBassEnable(ucPara1, ucPara2);
            break;

        case 0x3A:
            msAPI_AudioTrbleEnable(ucPara1, ucPara2);
            break;

#endif

#endif

#if ENABLE_RTE
        case 0x3B:
            msDrvOverDriveVerify();
            break;
        case 0x3C:
            msDrvOverDriveVerify2nd();
            break;
        case 0x3D:
            msDrvOverDriveWindow(TRUE ,0x00 ,ucPara1, ucPara2, ucPara3);    //enable, h_size, v_size
            break;
        case 0x3E:
            msDrvOverDriveMode(ucPara1, ucPara2, ucPara3 ,0);      //mode, h_scale, 2nd
            break;
    #if ENABLE_OD_MTB
        case 0x3F:
        {
        msAPI_OverDriveEnable(FALSE);
        msAPI_OverDriveMultiTable(ucPara1);
        msAPI_OverDriveInit();
        msAPI_OverDriveEnable( TRUE );
        DB_PRINT("<<Enable multi-OD through Autodownload>>\r\n");
        }
        break;
    #endif
#endif

        case 0x4F:
            SetupUserColorMode(TRUE, tUserColorMode);
            DB_PRINT("...Test user color mode...\r\n");
            break;

        case 0x50: // Keep this cmd!
            // Debug Pause in MDrv_UART_ExecTestCommand.
            rtn = FALSE;
            break;

        case 0x51: // Keep this cmd!
            // Debug Pause in MDrv_UART_ExecTestCommand.
            g_u8Test = ucPara1;
            break;

        case 0x52: // Keep this cmd!
            if (ucPara1==0x01)
            {
                msWriteByteMask(SCL0_01_80, _BIT0|BIT1, _BIT0|BIT1);
            }
            else if (ucPara1==0x02)
            {
                msWriteByteMask(SCR0_01_80, _BIT0|BIT1, _BIT0|BIT1);
            }
            else if (ucPara1==0x03)
            {
                msWriteByteMask(SCL0_01_80, _BIT0|BIT1, _BIT0|BIT1);
                msWriteByteMask(SCR0_01_80, _BIT0|BIT1, _BIT0|BIT1);
            }
            else if (ucPara1==0x04)
            {
                msWriteByteMask(SCL0_01_80, 0x00, _BIT0|BIT1);
                msWriteByteMask(SCR0_01_80, 0x00, _BIT0|BIT1);
            }
            break;


#if 0//ENABLE_DPS
        case 0x51:
            msDPS_Off_Setting();//REG_DPS_RUN_Flag=FALSE;//
            DB_PRINT("...Disable DPS Function...\r\n");
            break;
        case 0x52:
            msDPS_On_Setting();
            DB_PRINT("...Enable DPS Function...\r\n");
            break;
#endif

        case 0x53:
            setWinDlcOnOff(ENABLE,ucPara1);
            DB_PRINT("...DLC ON...Level(%d)\r\n", ucPara1);
            break;
        case 0x54:
            setWinDlcOnOff(DISABLE,ucPara1);
            DB_PRINT("...DLC Off...\r\n");
            break;
        case 0x55:
            DB_PRINT("\r\n g_bDLC_On (%d)", g_bDLC_On);
            //DB_PRINT("\r\n REG_DSC_RUN_Flag (%d)", REG_DSC_RUN_Flag);
            //DB_PRINT("\r\n REG_DPS_RUN_Flag (%d)", REG_DPS_RUN_Flag);
            break;
#if ENABLE_DEMURA_FUNCTION
        case 0x56:
            drvEnabledDemura(1);
            DB_PRINT("\r\n Demura Enable");
            break;
        case 0x57:
            DB_PRINT("\r\n load table EEPROM");
            drvDeCompressDeMura();
            DB_PRINT("\r\n De-compress data");
            drvSetDemuraTable();
            drvInitialDemura(PanelWidth);
            drvEnabledDemura(1);
            DB_PRINT("\r\n Demura Enable");
            break;
        case 0x58:
            drvInitDemuraTable();
            drvSetDemuraTable();
            drvInitialDemura(PanelWidth);
            drvEnabledDemura(1);
            DB_PRINT("\r\n Initial table(%x)");
            break;
    #endif

#if ENABLE_DELTAE_FUNCTION
        case 0x59:
            #if ENABLE_ColorMode_FUNCTION
                switch(ucPara1)
                {
                    DB_PRINT("... DELTAE_FUNCTION ...\r\n");
                    case 0x00:
                    DB_PRINT("... sRGB ...\r\n");
                    msLoadDeltaEColorMode(0);
                    break;

                    case 0x01:
                    DB_PRINT("... AdobeRGB ...\r\n");
                    msLoadDeltaEColorMode(1);
                    break;

                    case 0x02:
                    DB_PRINT("... BT709 ...\r\n");
                    msLoadDeltaEColorMode(2);
                    break;

                    case 0x03:
                    DB_PRINT("... BT2020 ...\r\n");
                    msLoadDeltaEColorMode(3);
                    break;

                    case 0x04:
                    DB_PRINT("... DCIP ...\r\n");
                    msLoadDeltaEColorMode(4);
                    break;
                }
                msSetColorEngine(TRUE);
            #endif
            break;
        case 0x5A:
            #if ENABLE_DICOM_FUNCTION
                DB_PRINT("... DICOM mode...\r\n");
                msSetDICOMMode();
            #endif
            break;
        case 0x5B:
            #if ENABLE_GAMMA_FUNCTION
            DB_PRINT("... msSetGammaCurve Gammaidx = %d...\r\n", ucPara1);
            msSetGammaCurve(USER_PREF_WIN_SEL, ucPara1);
            #endif
            break;
        case 0x5C:
            #if ENABLE_COLORTEMP_FUNCTION
            DB_PRINT("... msLoadColorTempMode idx = %d...\r\n", ucPara1);
            msLoadColorTempMode(ucPara1);
            #endif
            break;
        case 0x5D:
            #if ENABLE_COLORTRACK_FUNCTION
            DB_PRINT("... msSetColorTrackMode Gammaidx = %d...\r\n", ucPara1);
            msSetColorTrackMode(USER_PREF_WIN_SEL, ucPara1);
            #endif
            break;
#endif

#if ENABLE_PIU_UART_ResetCheck
        case 0x5E:
            TestUartReset();//REG_DPS_RUN_Flag=FALSE;//
            DB_PRINT("...TestUartReset...\r\n");
            break;
#endif
#endif // ENABLE_MSTV_UART_DEBUG


#if 1   //for PQ table test
        case 0x69:
        {
            DB_PRINT("[PQ_DEBUG]PQ_Init...\r\n");
            msPQ_Init( (EN_PQ_DISPLAY_TYPE)ucPara1);
        }
        break;
        case 0x6A:
        {
            DB_PRINT("[PQ_DEBUG]PQ_LoadTimingSetting...\r\n");
            ST_WINDOW_INFO  DisplayWin;
            DisplayWin.x =0;
            DisplayWin.y = 0;
            DisplayWin.width = g_DisplayWindow[ucPara1].sDwCaptureWin.width;
            DisplayWin.height = g_DisplayWindow[ucPara1].sDwCaptureWin.height;
            msPQ_LoadTimingSetting(ucPara1, DisplayWin, TRUE);
        }
        break;
        case 0x6B:
        {
            DB_PRINT("[PQ_DEBUG]PQ_LoadXRule...\r\n");
            msPQ_LoadXRuleTable(ucPara1);
        }
        break;
        case 0x6C:
        {
            DB_PRINT("[PQ_DEBUG]Bypass Mode\n");
            msPQ_BypassPQ(ucPara1, ucPara2);

        }
        break;
        case 0x6D:
        {
            DB_PRINT("[PQ_DEBUG]Dump user prefer pq timing data...\r\n");
            msPQ_LoadUserTiming(ucPara1, ucPara2);
        }
        break;
        case 0x6E:
        {
            DB_PRINT("[PQ_DEBUG]Scaling filte testing\n");
            if (ucPara1 == 0)
            {
                msPQ_UserDumpTable(0, 87, 1);
                msPQ_UserDumpTable(0, 88, 1);
                msPQ_UserDumpTable(0, 96, 1);
                msPQ_UserDumpTable(0, 97, 1);
            }
            else if (ucPara1 < 5)
            {
                msPQ_UserDumpTable(0, 87, 2);
                msPQ_UserDumpTable(0, 88, 6);
                msPQ_UserDumpTable(0, 96, 2);
                msPQ_UserDumpTable(0, 97, 6);
                msPQ_UserDumpTable(0, 102, ucPara1 + 17);
            }
        }
        break;
#endif
        case 0x6F:
        if (ucPara1 == 1)
        {
            printMsg("... FONT BASE TEST PATTERN Enable...\r\n");
            msWrite2Byte(OSD1_04, PANEL_WIDTH);
            msWrite2Byte(OSD1_06, PANEL_HEIGHT);
            msDrvOSDBlend_Init(0);
        }
        else
        {
            printMsg("... FONT BASE TEST PATTERN Disable...\r\n");
            msWriteBit(SCL0_00_0D, FALSE, BIT7);
            msWriteBit(SCL0_00_81, FALSE, BIT5);
            msDrvOSDBlend_Init(1);
        }
        break;

#if 1
        case 0x63: // Keypad_UP
            VKeyPad=VK_UP;
            DB_PRINT("VK_UP\r\n");
            break;
         case 0x83: // Keypad_UP
            VKeyPad=VK_DOWN;
            DB_PRINT("VK_DOWN\r\n");
            break;
        case 0x71: // Keypad_PIP
            VKeyPad=VK_PIP;
            DB_PRINT("VK_PIP\r\n");
            break;
        case 0x72: // Keypad_LEFT
            VKeyPad=VK_LEFT;
            DB_PRINT("VK_LEFT\r\n");
            break;
        case 0x73: // Keypad_MENU
            VKeyPad=VK_MENU;
            DB_PRINT("VK_MENU\r\n");
            break;
        case 0x74: // Keypad_RIGHT
            VKeyPad=VK_RIGHT;
            DB_PRINT("VK_RIGHT\r\n");
            break;
        case 0x75: // Keypad_SOURCE
            VKeyPad=VK_SOURCE;
            DB_PRINT("VK_SOURCE\r\n");
            break;
        case 0x76: // Keypad_POWER
            VKeyPad=VK_POWER;
            DB_PRINT("VK_POWER\r\n");
            break;

#endif

#if ENABLE_MSTV_UART_DEBUG && (ENABLE_TOUCH_PANEL||ENABLE_TOUCH_PANEL_CTRL_OSD)
        case 0x7E:
            {
                if(TPL_ReadSurface() == FALSE)
                    DB_PRINT( "<TPL_ReadSurface FAIL>\r\n");
            }
            break;
        case 0x7F:
            {
            }
            break;
#endif
#if ENABLE_MSTV_UART_DEBUG && ENABLE_TOUCH_PANEL_DATA_FROM_USB
        case 0x8E:
            {
                USB_SPI_WriteRegister(g_UartCommand.Buffer[2], g_UartCommand.Buffer[3]);
                DB_PRINT("WriteAddr:%x\r\n", g_UartCommand.Buffer[2]);
                DB_PRINT("WriteData:%x\r\n", g_UartCommand.Buffer[3]);
            }
            break;
        case 0x8F:
            {
                BYTE xdata tempbuf[0x20], i;
                USB_SPI_ReadRegister(0xC0, tempbuf, 0x20);
                for(i=0; i<0x20; i++)
                    DB_PRINT("tempbuf[%x]\r\n", tempbuf[i]);
            }
            break;
#endif
#if ENABLE_MSTV_UART_DEBUG && ENABLE_TOUCH_KEY
        case 0x90:
            {
                DB_PRINT("TouchKey:%x\r\n", IT7230_GetKeyStatus());
            }
            break;
#endif

#if ENABLE_MSTV_UART_DEBUG && ENABLE_VGA_INPUT
        case 0x91:
            {
                extern void drvADC_Test_Funct(BYTE tb1, BYTE tb2);
                drvADC_Test_Funct(g_UartCommand.Buffer[2], g_UartCommand.Buffer[3]);
            }
            break;
#endif

#if GPIO_DEBUG
        case 0xA0:
            {
            //  u32PWMCh    : PWM0~PWM9 DISP_PWM0~DISP_PWM5 (BIT0~BIT30)
            //  u16Freq     : if CLK=12M,the range is 6MHz~60Hz (0x5D8D80h~0xB7h)
            //  u16Freq     : 0~100% (0~0x64h)
            //  u8Config        : _PWM_DB_EN_,_PWM_DB_VSYNC_MODE_,_PWM_VSYNC_ALIGN_EN_,_PWM_HSYNC_ALIGN_EN_,_PWM_POLARITY_EN_,_PWM_OD_EN_
            extern void msAPIPWMConfig(DWORD u32PWMCh, DWORD u32Freq, WORD u16Duty, BYTE u8Config, BYTE u8Counter);
            extern void msDrvPWMEnableSwitch(DWORD u32PWMCh, Bool bEnable);
            msAPIPWMConfig(1<<ucPara1, ((DWORD)(ucPara2)<<8|ucPara3), BACKLIGHT_PWM_DUTY, BACKLIGHT_PWM_CONFIG, BACKLIGHT_PWM_VS_ALG_NUM);
            msDrvPWMEnableSwitch((DWORD)(1<<ucPara1),0x01);
            DB_PRINT("\n PWM selection : %d ",1<<ucPara1);
            DB_PRINT("\n PWM frequency : %d Hz",(ucPara2<<8|ucPara3));
            }
            break;
        case 0xA1:
            {
            extern void msAPI_AdjustBrightness( BYTE u8Brightness ); //0~100
            msAPI_AdjustBrightness(ucPara1);
            DB_PRINT("\n ucPara1 : %d ",ucPara1);
            }
            break;
        case 0xA2:
            {
            extern void msDrvPWMDoubleBuffer(DWORD u32PWMCh, BYTE u8Config);
            msDrvPWMDoubleBuffer(1<<ucPara1,ucPara2);
            DB_PRINT("\n msDrvPWMDoubleBuffer : %d ",ucPara1);
            }
            break;
        case 0xA3:
            {
            extern void msAPIPWMDuty(DWORD u32PWMCh,WORD u16ByteDuty);
            msAPIPWMDuty(1<<ucPara1,ucPara2);
            DB_PRINT("\n msDrvPWMDoubleBuffer : %d ",ucPara2);
            }
            break;
        case 0xA4:
            {
            //  u8Config        : _PWM_DB_EN_,_PWM_DB_VSYNC_MODE_,_PWM_VSYNC_ALIGN_EN_,_PWM_HSYNC_ALIGN_EN_,_PWM_POLARITY_EN_,_PWM_OD_EN_
            extern void msDrvPWMAlignSync(DWORD u32PWMCh,BYTE u8Config, BYTE u8Counter);
            extern void msAPIPWMConfig(DWORD u32PWMCh, DWORD u32Freq, WORD u16Duty, BYTE u8Config, BYTE u8Counter);
            //use 200Hz to verify Vsync alignment and use 200k to verify Hsync alignment
            msAPIPWMConfig(1<<ucPara1,((ucPara2==0x02)?(200):(200000)), BACKLIGHT_PWM_DUTY, BACKLIGHT_PWM_CONFIG, BACKLIGHT_PWM_VS_ALG_NUM);
            msDrvPWMAlignSync(1<<ucPara1,1<<ucPara2,ucPara3);
            DB_PRINT("\n msDrvPWMAlignSync : %d ",ucPara2);
            }
            break;
        case 0xA5:
            {
            extern void msDrvPWMPolarity(DWORD u32PWMCh, BYTE u8Config);
            msDrvPWMPolarity(1<<ucPara1,ucPara2);
            DB_PRINT("\n msDrvPWMAlignSync : %d ",ucPara2);
            }
            break;

#endif

#if ENABLE_TIME_MEASUREMENT
        case 0xA6:
            {
                DWORD i;
                TIME_MEASURE_BEGIN();
                for( i = 0; i < 1000; i++ )
                    ForceDelay1ms( 1 );
                TIME_MEASURE_END();

                TIME_MEASURE_BEGIN();
                for( i = 0; i < 250000; i++ );
                TIME_MEASURE_END();

                TIME_MEASURE_BEGIN();
                for( i = 0; i < 250000; i++ )
                    Delay4us();
                TIME_MEASURE_END();
            }
            break;
#endif



        case 0xCC:
        {
            extern void FactoryBlockInit(void);
            extern void FactoryBlockSave(void);
            extern void SystemBlockInit(void);
            extern void SystemBlockSave(void);
            extern void ColorParameterBlockInit(void);
            extern void ColorParameterBlockSave(void);

            if(ucPara1 == 0x00)
            {
                FactoryBlockInit();
                FactoryBlockSave();

                SystemBlockInit();
                SystemBlockSave();

                ColorParameterBlockInit();
                ColorParameterBlockSave();
            }
            else if(ucPara1 == 0x01)
            {
                ColorParameterBlockInit();
                ColorParameterBlockSave();
            }
            else if(ucPara1 == 0x02)
            {
                NVRam_WriteTbl(0x87, (BYTE *)&u8NVRAM_MAP[0x87], 0x4FF);
                NVRam_ReadTbl(0x87, (BYTE *)&u8NVRAM_MAP_BK[0x87], 0x4FF);
            }
            else if(ucPara1 == 0xFF)
            {
                DB_PRINT("Init\r\n");
                //Temp clear BK map
                for(u8count=0; u8count<0x1000; u8count++)
                {

                    u8NVRAM_MAP[u8count] = u8NVRAM_MAP_BK[u8count] = 0xCC;
                    NVRam_WriteByte(u8count, 0xCC);
                }
                DB_PRINT("OK\r\n");
            }
            }
            break;

        case 0xE0:
            {
                extern void VerGE_Main(void);
#if VERIFY_GE
                VerGE_Main();
#endif
            }
            break;

    #if (ENABLE_DAISY_CHAIN == 0x1)
      case 0xE8:
        {
            BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
            DB_PRINT("DP Switch to DP1.1\n");
            u8SendData = 0x1;
            while(!(msDCSetCmdSetSysMode(u8SendData, &u8ReplyData)) && --u8Retry)
            {
                DB_PRINT("[PM] DC Setup Retry %d\n",u8Retry);
            }
        }
        break;
      case 0xE9:
        {
            BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
            DB_PRINT("DP Switch to DP1.2 SST\n");
            u8SendData = 0x2;
            while(!(msDCSetCmdSetSysMode(u8SendData, &u8ReplyData)) && --u8Retry)
            {
                DB_PRINT("[PM] DC Setup Retry %d\n",u8Retry);
            }
        }
        break;
      case 0xEA:
        {
            BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
            DB_PRINT("DP Switch to DP1.2 MST\n");
            u8SendData = 0x3;
            while(!(msDCSetCmdSetSysMode(u8SendData, &u8ReplyData)) && --u8Retry)
            {
                DB_PRINT("[PM] DC Setup Retry %d\n",u8Retry);
            }
        }
        break;
      case 0xEB:
        {
            BYTE u8Retry=10, u8SendData=0, u8ReplyData=0;
            DB_PRINT("Check DC is busy or not in MST mode\n");

            while(!(msDCSetCmdCheckMstBusy(u8SendData, &u8ReplyData)) && --u8Retry)
            {
                DB_PRINT("[PM] DC Setup Retry %d\n",u8Retry);
            }

            if(u8ReplyData & BIT1) {
                DB_PRINT("[EP] Busy!");
            }
            else if(u8ReplyData & BIT2) {
                DB_PRINT("[EP] No-Busy!");
            }
        }
        break;

     #endif //#if (ENABLE_DAISY_CHAIN == 0x1)
        case 0xFA:
        {
                extern U16 msAPI_CalCheckSum(U32 u32FlashSize);
                extern MS_BOOL MDrv_SERFLASH_DetectSize(MS_U32 *u32FlashSize);
                U16 checksum;
                U32 flashsize;
                MDrv_SERFLASH_DetectSize(&flashsize);
                DB_PRINT(">>>> flashsize==%x\r\n",flashsize);
                DB_PRINT(">>>>Start to calculate checksum\r\n",0);
                checksum=msAPI_CalCheckSum(flashsize);
                DB_PRINT("<<<<<checksum=[%x]\r\n",checksum);
        }
        break;

      case 0xFB:
         if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerDebug]!=NULL)
              ((fpCustomerDebug)CB_FuncList[eCB_CustomerDebug])(ucPara1,ucPara2,ucPara3 );
            break;

        case 0xFC:
            {
                int i=10;
                DB_PRINT("enter ePM_POWER_DPMS\n");

                while(i--)
                {
                    DB_PRINT("  count down %d seconds..\r\n", i);
                    Delay1ms(1000);
                }
                msAPI_PMSetMode(ePM_POWER_DPMS);
            }
            break;

#if ENABLE_USB_HOST && ENABLE_FILESYSTEM
        case 0xFD:
        {
            if (ucPara1==0x01)
            {
                extern int CheckUSB_existDLBinFile(void);
                extern BOOL MW_UsbDownload_GetVersion(U8 *u8modelName, U32 *u8Version);
                U8 modelname[20]={0,};
                U32 version=0;
                CheckUSB_existDLBinFile();
                MW_UsbDownload_GetVersion(modelname,&version);
                DB_PRINT("@@@@@@@@@@@@@>>modelname=[%s],@@@@@@@@@@@>>version=[%x]\r\n",modelname,version);
            }
            else
            {
                extern int usb_check(BOOL bFullDL);
                usb_check(ucPara2);
            }
        }
        break;
#endif

#if (ENABLE_CP_R2)
        case 0xFE:
        {
                extern U32 MDrv_Sys_CoproRun(MS_U32 u32BootAddr);

                U32 u32CP_BIN_Addr = 0x0;
                MDrv_Sys_CoproRun(u32CP_BIN_Addr);
        }
        break;
#endif

        case 0xFF:
            CURRENT_RED_COLOR = CURRENT_GREEN_COLOR = CURRENT_BLUE_COLOR = 0x80;
            CURRENT_RED_BLACK_LEVEL = CURRENT_GREEN_BLACK_LEVEL= CURRENT_BLUE_BLACK_LEVEL = 0x80;
            CURRENT_COLOR_TEMP = ucPara1;
            CURRENT_CONTRAST = ucPara2;
            Menu_AdjustColorSetting();
            break;

        default:
            rtn = FALSE;
            break;
    }

    return rtn;
}

//I2C_SCL_Pin               // GPIO00
//I2C_SDA_Pin               // GPIO01
#undef Init_hwI2C_SCL_Pin
#define Init_hwI2C_SCL_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT0))
#undef Init_hwI2C_SDA_Pin
#define Init_hwI2C_SDA_Pin()        (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT1))

#define DSet_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT1))
#define DClr_i2c_SDA()               (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT1), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT1))
#define Di2c_SDAHi()                 (_bit1_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define Di2c_SDALo()                 (!Di2c_SDAHi())
#define DSet_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, TRUE, BIT0))
#define DClr_i2c_SCL()               (MEM_MSWRITE_BIT(_REG_GPIO0_OUT, FALSE, BIT0), MEM_MSWRITE_BIT(_REG_GPIO0_OEZ, FALSE, BIT0))
#define Di2c_SCLHi()                 (_bit0_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#define Di2c_SCLLo()                 (!Di2c_SCLHi())

typedef enum _I2cIoTransType
{
    I2C_TRANS_READ,
    I2C_TRANS_WRITE
}I2cIoTransType;

#define I2C_DEVICE_ADR_WRITE(slave_adr)   (slave_adr & ~_BIT0)
#define I2C_DEVICE_ADR_READ(slave_adr)    (slave_adr | _BIT0)

#define I2C_ACCESS_DUMMY_TIME   3

#define FWI2C_ACKNOWLEDGE           TRUE
#define FWI2C_NON_ACKNOWLEDGE       FALSE
#if 1//(USE_SW_I2C == 1)
//====================================
// Setup i2c Start condition
Bool Di2c_Start( void )
{
    DSet_i2c_SDA();
    DSet_i2c_SCL();
    Delay2us();
    if( Di2c_SDALo() || Di2c_SCLLo() )
        return FALSE;
    DClr_i2c_SDA();
    Delay2us();
    DClr_i2c_SCL();
    Delay2us();
    return TRUE;
}
//=============================================
// Setup i2c Stop condition
void Di2c_Stop( void )
{
    // SCL=L, SDA=L, Stop condition.
    DClr_i2c_SCL();
    DClr_i2c_SDA();
    Delay2us();
    DSet_i2c_SCL();
    Delay2us();
    DSet_i2c_SDA();
    Delay2us();
}
//============================================
Bool DWait_i2C_Ack( void )
{
    BYTE i;
    for( i = 0; i < 5; i++ )
    {
        if( Di2c_SDALo() )
            return TRUE;
        Delay2us();
    }

    if( Di2c_SDALo() )
        return TRUE;
    else
        return FALSE;

}
//============================================
Bool Di2c_SendByte( BYTE value )
{
    BYTE i;
    Bool result;

    for( i = 0; i < 8; i++ ) // Send data via i2c pin
    {
        if( value & BIT7 )
            DSet_i2c_SDA();
        else
            DClr_i2c_SDA();
        Delay2us();
        DSet_i2c_SCL();
        Delay2us();
        value <<= 1;
        DClr_i2c_SCL();
    }
    DSet_i2c_SDA();
    result = DWait_i2C_Ack();
    DSet_i2c_SCL();
    Delay2us();
    DClr_i2c_SCL();
    Delay2us();
    DClr_i2c_SDA();

    return result;
}

//============================================
BYTE Di2c_ReceiveByte( const Bool ack )
{
    BYTE i;
    BYTE value = 0;

    for( i = 0; i < 8; i++ )
    {
        value <<= 1;
        DSet_i2c_SDA();
        Delay2us();
        DSet_i2c_SCL();
        Delay2us();
        if( Di2c_SDAHi() )
            value |= BIT0;
        DClr_i2c_SCL();
    }
    if( ack )
    {
        DClr_i2c_SDA();
    }
    else
    {
        DSet_i2c_SDA();
    }
    Delay2us();
    DSet_i2c_SCL();
    Delay2us();
    DClr_i2c_SCL();

    return value;
}

void Di2c_Start1(void)
{
    DSet_i2c_SDA();
    Delay2us();
    DSet_i2c_SCL();
    Delay2us();
    DClr_i2c_SDA();
    Delay2us();
    DClr_i2c_SCL();
}
void Di2C_Intial(void)
{
    BYTE i, j;

    for(i=0 ;i < 20; i++)   // Generate SCL signals to reset EEPROM.
    {
        DSet_i2c_SCL();
        Delay2us();
        DClr_i2c_SCL();
        Delay2us();
    }

    j = 20;

    while( j-- )    // Male EEPROM to Release I2C bus.
    {
        for(i=0 ;i < 9; i++)    // 9 STARTs
        {
            Di2c_Start1();
        }
        Di2c_Stop();
        ForceDelay1ms(10);

        if(Di2c_SDAHi())
            break;
    }

}

#endif

BOOL i2c_AccessStart(BYTE ucSlaveAdr, I2cIoTransType trans_t)
{
    BYTE ucDummy; // loop dummy

    if (trans_t == I2C_TRANS_READ) // check i2c read or write
        ucSlaveAdr = I2C_DEVICE_ADR_READ(ucSlaveAdr); // read
    else
        ucSlaveAdr = I2C_DEVICE_ADR_WRITE(ucSlaveAdr); // write
    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while (ucDummy--)
    {
        Di2c_Start();

        if (Di2c_SendByte(ucSlaveAdr) == FWI2C_ACKNOWLEDGE) // check acknowledge
            return TRUE;
        Di2c_Stop();

        Delay1ms(1);
    } // while

    return FALSE;
}

void i2cBurstReadBytes(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE *pBuf, BYTE ucBufLen)
{
    BYTE ucDummy; // loop dummy

    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
        if (i2c_AccessStart(ucSlaveAdr, I2C_TRANS_WRITE) == FALSE)
            continue;
        if (Di2c_SendByte(ucSubAdr) == FWI2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;
        if (i2c_AccessStart(ucSlaveAdr, I2C_TRANS_READ) == FALSE)
            continue;

        while(ucBufLen) // loop to burst read
        {
            ucBufLen--;
            *pBuf = Di2c_ReceiveByte(ucBufLen); // receive byte

            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    Di2c_Stop();

    while(ucBufLen--) // if read error, fill 0xff
    {
        *pBuf = 0xff; // clear
        pBuf++; // next byte pointer
    } // while
}

void i2cBurstWriteBytes(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE *pBuf, BYTE ucBufLen)
{
    BYTE ucDummy; // loop dummy

    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
        if (i2c_AccessStart(ucSlaveAdr, I2C_TRANS_WRITE) == FALSE)
            continue;

        if (Di2c_SendByte(ucSubAdr) == FWI2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;

        while(ucBufLen--) // loop of writting data
        {
            Di2c_SendByte(*pBuf); // send byte

            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    Di2c_Stop();
}


BYTE i2cReadByte(BYTE ucSlaveAdr, BYTE ucSubAdr)
{
    BYTE ucBuf; // byte buffer

    i2cBurstReadBytes(ucSlaveAdr, ucSubAdr, &ucBuf, 1);
    return ucBuf;
}

void i2cWriteByte(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE ucVal)
{
    i2cBurstWriteBytes(ucSlaveAdr, ucSubAdr, &ucVal, 1);        // wz@ fpga hang
}

BOOL ProcessCommand( void )
{
    BYTE rtn=TRUE;

    switch( UART_CMD )
    {
#if 0
        case uartWrite_MST_Bank_n:
        {
            {
                const volatile BYTE ucBank = _scReadByte( (BYTE)SC0_00 ); // store original bank
                _scWriteByte( (BYTE)SC0_00, UART_CMD_MS_BANK );
                _scWriteByte( UART_CMD_MS_REGINDEX, UART_CMD_MS_REGDATA );
                _scWriteByte( (BYTE)SC0_00, ucBank ); // restore original bank
            }
            putSIOChar( 0xF1 );
            break;
        }

        case uartRead_MST_Bank_n:
        {
            {
                const volatile BYTE ucBank = _scReadByte( (BYTE)SC0_00 ); // store original bank
                _scWriteByte( (BYTE)SC0_00, UART_CMD_MS_BANK );
                UART_CMD_MS_REGDATA = _scReadByte( UART_CMD_MS_REGINDEX );
                _scWriteByte( (BYTE)SC0_00, ucBank ); // restore original bank
            }
            putSIOChar( 0xF2 );
            putSIOChar( UART_CMD_MS_REGDATA );
            break;
        }

        case uartWrite_MCU_XDATA:
        {
            //_msWriteByte( MAKEWORD( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L ), UART_CMD_MCU_DATA );
            _msRegs[MAKEWORD( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L )] =  UART_CMD_MCU_DATA;
            putSIOChar( 0xF1 );
            break;
        }

        case uartRead_MCU_XDATA:
        {
            //UART_CMD_MCU_DATA = _msReadByte( MAKEWORD( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L ) );
            UART_CMD_MCU_DATA = _msRegs[( MAKEWORD( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L ) )];
            putSIOChar( 0xF2 );
            putSIOChar( UART_CMD_MCU_DATA );
            break;
        }
#endif
        case uartTest_Command:

        {
            rtn = ExecTestCommand();
            break;
        }
    #if !USEFLASH
        case uartEEPROM_Write:
        {
            WORD uwEEADDR;
            uwEEADDR = MAKEWORD(g_UartCommand.Buffer[1],g_UartCommand.Buffer[2]);
            NVRam_WriteByte(uwEEADDR, g_UartCommand.Buffer[3]);
            //if (!g_bDebugASCIICommandFlag)
                putSIOChar(0xF1);
                //DB_PRINT("EEPROM[0x%x]=", uwEEADDR);
                //DB_PRINT("0x%x \r\n", g_UartCommand.Buffer[_UART_CMD_INDEX3_]);

            break;
        }
        case uartEEPROM_Read:
        {
        WORD uwEEADDR;
            uwEEADDR = MAKEWORD(g_UartCommand.Buffer[1],g_UartCommand.Buffer[2]);
            //uwEEADDR = g_UartCommand.Buffer[_UART_CMD_INDEX1_] & 0x0E;
            //uwEEADDR = (uwEEADDR << 7) + g_UartCommand.Buffer[_UART_CMD_INDEX2_];
        NVRam_ReadByte(uwEEADDR , &g_UartCommand.Buffer[3]);
            putSIOChar(0xF2);
            putSIOChar(g_UartCommand.Buffer[3]);
            //if (g_bDebugASCIICommandFlag)
            //{
                //DB_PRINT("EEPROM[0x%x]=", uwEEADDR);
                //DB_PRINT("0x%x \r\n", g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            //}
            break;
        }
    #endif
    //void i2c_WriteTBL( BYTE deviceID, WORD addr, BYTE *buffer, BYTE count )

    case uartI2C_Device_Write:
        {
        WORD adr;
        adr = g_UartCommand.Buffer[_UART_CMD_INDEX1_] ;
            if (g_bDebugASCIICommandFlag)
            {
                DB_PRINT("I2C 0x%x ", g_UartCommand.Buffer[_UART_CMD_INDEX1_]);
                DB_PRINT("[0x%x]=", g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
                DB_PRINT("0x%x \r\n", g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            }

                i2cWriteByte( g_UartCommand.Buffer[_UART_CMD_INDEX1_],
                    g_UartCommand.Buffer[_UART_CMD_INDEX2_],
                    g_UartCommand.Buffer[_UART_CMD_INDEX3_]);


        //i2c_WriteTBL( g_UartCommand.Buffer[_UART_CMD_INDEX1_],
        //  g_UartCommand.Buffer[_UART_CMD_INDEX2_],
        //  &g_UartCommand.Buffer[_UART_CMD_INDEX3_],1);
        //if (!g_bDebugASCIICommandFlag)
            putSIOChar(0xF1);

        }
        break;

    case uartI2C_Device_Read:
    //i2c_ReadTBL( NVRAM_DEVICE, addr, value, 1 );


    {
    // ======== for touch sensor =========
            g_UartCommand.Buffer[_UART_CMD_INDEX3_] =
            i2cReadByte( g_UartCommand.Buffer[_UART_CMD_INDEX1_],  g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
            if (g_bDebugASCIICommandFlag)
            {
                DB_PRINT("I2C 0x%x ", g_UartCommand.Buffer[_UART_CMD_INDEX1_]);
                DB_PRINT("[0x%x]=", g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
                DB_PRINT("0x%x \r\n", g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            }

    //i2c_ReadTBL( g_UartCommand.Buffer[_UART_CMD_INDEX1_],
    //          g_UartCommand.Buffer[_UART_CMD_INDEX2_],
    //          (BYTE *)&g_UartCommand.Buffer[_UART_CMD_INDEX3_] ,1);
            putSIOChar(0xF2);
            putSIOChar(g_UartCommand.Buffer[3]);

    }
    break;

        default:
            rtn=FALSE;
            break;
    }

    return rtn;
}

BOOL ProcessCommandExt( void )
{
    BYTE rtn=TRUE;

    switch( UART_EXT_CMD_MS_DAT1 )
    {
        case 0x00:
            DB_PRINT("[DB_Ext] 0x00: List of Debug Functions\n");
            DB_PRINT("[DB_Ext] 0x01: TestCommand_PxP()\n");
            DB_PRINT("[DB_Ext] 0x03: TestCommand_OSDC()\n");
            DB_PRINT("[DB_Ext] 0x04: TestCommand_ScalingFilter()\n");
            DB_PRINT("[DB_Ext] 0x05: TestCommand_DrawOSD()\n");
            DB_PRINT("[DB_Ext] 0x06: TestCommand_DisplayPort()\n");
            DB_PRINT("[DB_Ext] 0x07: TestCommand_DaisyChainRW()\n");
            DB_PRINT("[DB_Ext] 0x11: TestCommand_MWE()\n");
            DB_PRINT("[DB_Ext] 0x12: TestCommand_XMODEM()\n");
            DB_PRINT("[DB_Ext] 0x13: TestCommand_AdjustOutput()\n");
            DB_PRINT("[DB_Ext] 0x14: TestCommand_ScalerMLoad()\n");
            DB_PRINT("[DB_Ext] 0x15: TestCommand_CursorWindow()\n");
            DB_PRINT("[DB_Ext] 0x71: TestCommand_Ext_RIU()\n");
            break;

        case 0x01:
            TestCommand_PxP();
            DB_PRINT("TestCommand_Pxp\n");
            break;
#if ENABLE_OSDC_OSDD
        case 0x03:
            TestCommand_OSDC();
            break;
#endif
        case 0x04:
            TestCommand_ScalingFilter();
            break;

        case 0x05:
            TestCommand_DrawOSD();
            break;
#if ENABLE_DP_INPUT
        case 0x06:
            TestCommand_DisplayPort();
            break;
#endif
#if ENABLE_DAISY_CHAIN
        case 0x07:
            TestCommand_DaisyChainRW();
            break;
#endif
#if (ENABLE_MWE)
        case 0x11:
            TestCommand_MWE();
            break;
#endif
#if (XMODEM_DWNLD_ENABLE)
        case 0x12:
            TestCommand_XMODEM();
            break;
#endif
        case 0x13:
            TestCommand_AdjustOutput();
            break;

#if ENABLE_MENULOAD
        case 0x14:
            TestCommand_ScalerMLoad();
            break;
#endif
        case 0x15:
            TestCommand_CursorWindow();
            break;

		case 0x16:
			
			break;
			
#ifdef EXTERNAL_MCU
        case 0x71:
            TestCommand_Ext_RIU();
            break;
#endif
        default:
            rtn=FALSE;
            break;
    }

    return rtn;
}
#endif



