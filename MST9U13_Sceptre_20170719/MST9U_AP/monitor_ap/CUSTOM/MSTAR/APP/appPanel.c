#include "datatype.h"
//#include "reg52.h"
#include "i2cdef.h"
#include "Board.h"
#include "MDebug.h"
#include "Common.h"
#include "Global.h"
#include "misc.h"
#include "GPIO_DEF.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "appPanel.h"


void Panel_i2c_Delay( void )
{
    DelayUs(4);
}
Bool i2c_Start_Panel( void )
{
    Set_Panel_I2C_SDA();
    Set_Panel_I2C_SCL();
    Panel_i2c_Delay();
    if( Panel_I2C_SDALo() || Panel_I2C_SCLLo() )
        return FALSE;
    Clr_Panel_I2C_SDA();
    Panel_i2c_Delay();
    Clr_Panel_I2C_SCL();
    Panel_i2c_Delay();
    return TRUE;
}
//=============================================
// Setup i2c Stop condition
void i2c_Stop_Panel( void )
{
    // SCL=L, SDA=L, Stop condition.
    Clr_Panel_I2C_SCL();
    Clr_Panel_I2C_SDA();
    Panel_i2c_Delay();
    Set_Panel_I2C_SCL();
    Panel_i2c_Delay();
    Set_Panel_I2C_SDA();
    Panel_i2c_Delay();
}
//============================================
Bool Wait_i2C_Ack_Panel( void )
{
    BYTE i;
    for( i = 0; i < 5; i++ )
    {
        if( Panel_I2C_SDALo() )
            return TRUE;
        Panel_i2c_Delay();
    }

    if( Panel_I2C_SDALo() )
        return TRUE;
    else
        return FALSE;

}
//============================================
Bool i2c_SendByte_Panel( BYTE value )
{
    BYTE i;
    Bool result;

    for( i = 0; i < 8; i++ ) // Send data via i2c pin
    {
        if( value & BIT7 )
         Set_Panel_I2C_SDA();
        else
         Clr_Panel_I2C_SDA();
        Panel_i2c_Delay();
        Set_Panel_I2C_SCL();
        Panel_i2c_Delay();
        value <<= 1;
        Clr_Panel_I2C_SCL();
            DelayUs(1);
    }
    Set_Panel_I2C_SDA();
    Panel_i2c_Delay();
    Set_Panel_I2C_SCL();
    result = Wait_i2C_Ack_Panel();
    Clr_Panel_I2C_SCL();
    Panel_i2c_Delay();
    Clr_Panel_I2C_SDA();

    return result;
}

//============================================
BYTE i2c_ReceiveByte_Panel( const Bool ack )
{
    BYTE i;
    BYTE value = 0;

    for( i = 0; i < 8; i++ )
    {
        value <<= 1;
        Set_Panel_I2C_SDA();
        Panel_i2c_Delay();
        Set_Panel_I2C_SCL();
        Panel_i2c_Delay();
        if( Panel_I2C_SDAHi() )
            value |= BIT0;
        Clr_Panel_I2C_SCL();
    }
    if( ack )
    {
        Clr_Panel_I2C_SDA();
    }
    else
    {
        Set_Panel_I2C_SDA();
    }
    Panel_i2c_Delay();
    Set_Panel_I2C_SCL();
    Panel_i2c_Delay();
    Clr_Panel_I2C_SCL();

    return value;
}

//////////////////////////////////////////////////////////////////////////
// I2C access start.
//
// Arguments: ucSlaveAdr - slave address
//            trans_t - PANEL_I2C_TRANS_WRITE/PANEL_I2C_TRANS_READ
//////////////////////////////////////////////////////////////////////////
Bool Panel_i2c_AccessStart(BYTE ucSlaveAdr, Panel_I2cIoTransType trans_t)
{
    BYTE ucDummy; // loop dummy

    if (trans_t == PANEL_I2C_TRANS_READ) // check i2c read or write
        ucSlaveAdr = PANEL_I2C_DEVICE_ADR_READ(ucSlaveAdr); // read
    else
        ucSlaveAdr = PANEL_I2C_DEVICE_ADR_WRITE(ucSlaveAdr); // write

    ucDummy = PANEL_I2C_ACCESS_DUMMY_TIME;
    while (ucDummy--)
    {
        if (i2c_Start_Panel() == FALSE)
        { 
            i2c_Stop_Panel();
            continue;
        }

        if (i2c_SendByte_Panel(ucSlaveAdr) == TRUE) // check acknowledge
            return TRUE;

        i2c_Stop_Panel();

        ForceDelay1ms(1);
    } // while

    return FALSE;
}



void Panel_i2cBurstWriteBytes(BYTE ucSlaveAdr, BYTE *pBuf, BYTE ucBufLen)
{
    BYTE ucDummy; // loop dummy

    ucDummy = PANEL_I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
		
        if (Panel_i2c_AccessStart(ucSlaveAdr, PANEL_I2C_TRANS_WRITE) == FALSE)
            continue;
        while(ucBufLen--) // loop of writting data
        {
			
            i2c_SendByte_Panel(*pBuf); // send byte
            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    i2c_Stop_Panel();
}

//==================================================================
//==================================================================

#if (PanelType==PanelV400DK1_KS1 ||PanelType==PanelV400DK1_QS1||PanelType==PanelV420DK1_KS1||PanelType==PanelV400DJ1_QS5)//1718

#if 1
//without SR Function&Color Engine Function
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table1[7] = 
{
    0x20, 0x01, 0x02, 0x00, 0x40, 0xFF, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table2[7] = 
{
    0x80, 0x02, 0x00, 0x40, 0x62, 0x51, 0x73,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table3[7] = 
{
    0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table4[7] = 
{
    0x23, 0x1B, 0x10, 0x27, 0xC2, 0xEE, 0xD0,
};

#if 0
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table5[7] = 
{
    0x33, 0x1B, 0x10, 0x27, 0x40, 0x71, 0xD0,
};
#else
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table5[7] = 
{
    0x23, 0x1B, 0x10, 0x27, 0x40, 0x40, 0xD0,
};
#endif

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table6[7] = 
{
    0x33, 0x1B, 0x10, 0x27, 0xC3, 0xEE, 0xD0,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table7[7] = 
{
    0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table8[7] = 
{
    0x13, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table9[7] = 
{
    0x33, 0x1C, 0x13, 0x00, 0x04, 0x40, 0x40,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table10[7] = 
{
    0x33, 0x1C, 0x13, 0x00, 0x04, 0x00, 0x40,
};
#elif 0//FHD 120

//without SR Function&Color Engine Function
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table1[7] = 
{
    0x80, 0x01, 0x02, 0x00, 0x00, 0x55, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table2[7] = 
{
    0x80, 0x02, 0x00, 0x00, 0x02, 0x01, 0x03,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table3[7] = 
{
    0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table4[7] = 
{
    0x23, 0x1B, 0x10, 0x27, 0xC2, 0xEE, 0xD0,
};

#if 1
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table5[7] = 
{
    0x33, 0x1B, 0x10, 0x27, 0x40, 0x71, 0xD0,
};
#else
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table5[7] = 
{
    0x23, 0x1B, 0x10, 0x27, 0x40, 0x40, 0xD0,
};
#endif

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table6[7] = 
{
    0x33, 0x1B, 0x10, 0x27, 0xC3, 0xEE, 0xD0,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table7[7] = 
{
    0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table8[7] = 
{
    0x13, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table9[7] = 
{
    0x33, 0x1C, 0x13, 0x00, 0x04, 0x40, 0x40,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table10[7] = 
{
    0x33, 0x1C, 0x13, 0x00, 0x04, 0x00, 0x40,
};

#else
//without SR Function&Color Engine Function
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table1[7] = 
{
    0x40, 0x01, 0x02, 0x00, 0x00, 0x11, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table2[7] = 
{
    0x30, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table3[7] = 
{
    0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table4[7] = 
{
    0x23, 0x1B, 0x10, 0x27, 0xC2, 0xEE, 0xD0,
};

#if 1
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table5[7] = 
{
    0x33, 0x1B, 0x10, 0x27, 0x40, 0x71, 0xD0,
};
#else
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table5[7] = 
{
    0x23, 0x1B, 0x10, 0x27, 0x40, 0x40, 0xD0,
};
#endif

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table6[7] = 
{
    0x33, 0x1B, 0x10, 0x27, 0xC3, 0xEE, 0xD0,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table7[7] = 
{
    0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table8[7] = 
{
    0x13, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table9[7] = 
{
    0x33, 0x1C, 0x13, 0x00, 0x04, 0x40, 0x40,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table10[7] = 
{
    0x33, 0x1C, 0x13, 0x00, 0x04, 0x00, 0x40,
};


#endif


#else
//without SR Function
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table1[7] = 
{
    0x20, 0x01, 0x02, 0x00, 0x40, 0xFF, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table2[7] = 
{
    0x80, 0x02, 0x00, 0x40, 0x62, 0x51, 0x73,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table3[7] = 
{
    0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table4[7] = 
{
    0x23, 0x1B, 0x10, 0x27, 0xC2, 0xEE, 0xD0,
};
#if 0
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table5[7] = 
{
    0x33, 0x1B, 0x10, 0x27, 0x40, 0x71, 0xD0,
};
#else
BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table5[7] = 
{
    0x23, 0x1B, 0x10, 0x27, 0x40, 0x40, 0xD0,
};
#endif

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table6[7] = 
{
    0x33, 0x1B, 0x10, 0x27, 0xC3, 0xEE, 0xD0,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table7[7] = 
{
    0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table8[7] = 
{
    0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table9[7] = 
{
    0x33, 0x1C, 0x13, 0x00, 0x04, 0x40, 0x40,
};

BYTE code QFHD_2D_50_60_Hz_Graphic_Mode_Table10[7] = 
{
    0x33, 0x1C, 0x13, 0x00, 0x04, 0x00, 0x40,
};
#endif

BYTE code UHD_Panel_PanelV650DK1_LS1_Table[9] = 
{
//    0x26, 0x38,0x65,0x16,0x00,0x03,0x02,0x01,0x00,		
    0x26, 0x38,0x50,0x6D,0x00,0x00,0x00,0x00,0x02,		
};

/*
BYTE code UHD_Panel_PanelV400DK1_LS1_Table[7] = 
{
    0x23, 0x02,0x00,0x00,0x00,0x00,0x00,		
};
*/


void appPanel_SetPanel_QFHD_2D_Graphic_Mode(void)
{
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table1, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table1));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table2, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table2));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table3, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table3));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table4, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table4));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table5, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table5));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table6, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table6));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table7, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table7));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table8, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table8));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table9, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table9));
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table10, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table10));

    ForceDelay1ms(100);
}


#if(PanelType==PanelV390DK1_LS1)
Bool SwitchTCONDisplay(void)
{
  //BYTE CommandTbl[8]={0x38,0x50,0x6D,0x00,0x00,0x00,0x00,0x02};

  //ExternalI2C_WriteTbl(0xE0, 0x26, ( BYTE *)&CommandTbl, 8);
  Panel_i2cBurstWriteBytes(0xE0, UHD_Panel_PanelV650DK1_LS1_Table, sizeof(UHD_Panel_PanelV650DK1_LS1_Table));
  
  return TRUE;
}
#endif


#if 0//(PanelType==PanelV400DK1_KS1)
Bool SwitchTCONDisplay(void)
{
  Panel_i2cBurstWriteBytes(PANEL_DEVICE, UHD_Panel_PanelV400DK1_LS1_Table, sizeof(UHD_Panel_PanelV400DK1_LS1_Table));
  return TRUE;
}
#endif


/*
void appPanel_SetPanel_QFHD_2D_Graphic_Mode_Index(BYTE CommandIndex)
{
printData("CommandIndex==%d",CommandIndex);
  if(CommandIndex==1)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table1, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table1));
 else if(CommandIndex==2)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table2, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table2));
 else if(CommandIndex==3)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table3, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table3));
 else if(CommandIndex==4)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table4, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table4));
 else if(CommandIndex==5)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table5, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table5));
 else if(CommandIndex==6)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table6, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table6));
  else if(CommandIndex==7)
   Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table7, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table7));
 else if(CommandIndex==8)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table8, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table8));
 else if(CommandIndex==9)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table9, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table9));
 else if(CommandIndex==10)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, QFHD_2D_50_60_Hz_Graphic_Mode_Table10, sizeof(QFHD_2D_50_60_Hz_Graphic_Mode_Table10));
 else if(CommandIndex==20)
    Panel_i2cBurstWriteBytes(PANEL_DEVICE, UHD_Panel_PanelV400DK1_LS1_Table, sizeof(UHD_Panel_PanelV400DK1_LS1_Table));


    printData("CommandIndex end==%d",CommandIndex);

    ForceDelay1ms(100);
}
*/




