#define _MS_2DTo3D_C_
#include "Global.h"

#if 0//ENABLE_R2_2DTO3D
#define MS2DTo3D_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && MS2DTo3D_DEBUG
#define MS2DTo3D_printData(str, value)   printData(str, value)
#define MS2DTo3D_printMsg(str)           printMsg(str)
#else
#define MS2DTo3D_printData(str, value)
#define MS2DTo3D_printMsg(str)
#endif


code BYTE t2Dto3DPIXGain[5] =
{
    0x10, 0x18, 0x30, 0x48, 0x60,
};

code BYTE t2Dto3DBLKGain[5] =
{
    0x04, 0x08, 0x10, 0x18, 0x20,
};

code BYTE t2Dto3DPIXOffset[5] =
{
    0x80, 0x80, 0x80, 0x80, 0x80,
};

code BYTE t2Dto3DBLKOffset[5] =
{
    0x20, 0x40, 0x80, 0xC0, 0xE0,
};


void Sc2DTo3DInit(void)
{
    xdata WORD ucRegVal;
    xdata DWORD dwBase;


    MS2DTo3D_printMsg("-------------Sc2DTo3DInit------------");
    mcuDMADownloadCode(DMA_TO_DRAM_SOURCE_ADDR, DMA_TO_DRAM_DESTIN_ADDR, DMA_TO_DRAM_BYTE_COUNT);
    //mcuArrangeCodeAddr(_DRAM, MCU_ON_DRAM_START_ADDR , MCU_ON_DRAM_END_ADDR);
    //mcuArrangeCodeAddr(_SPI, MCU_ON_SPI_START_ADDR, MCU_ON_SPI_END_ADDR);

    //DR settings
    _msWriteByte(SC12_C2, 0x01);                         //enable depth render
    _msWrite2ByteMask(SC12_A2, PANEL_WIDTH, 0x1FFF);     //frame width
    _msWrite2ByteMask(SC12_AA, PANEL_WIDTH, 0x1FFF);     //H pixel number
    _msWrite2ByteMask(SC12_A4, PANEL_HEIGHT, 0x1FFF);    //frame height

    //set the values of dr miu request control and dr miu last control
    //equal to ceiling( ceiling(PANEL_WIDTH / BLOCK_WIDTH)  * 1 bytes / number of bytes per dr request)
    //e.g. ceiling( ceiling(1920/32) * 1  / 4 )
    if(PANEL_WIDTH & 0x1F)
        ucRegVal = PANEL_WIDTH/32 + 1;
    else
        ucRegVal = PANEL_WIDTH/32;

    if(ucRegVal & 0x03)
        ucRegVal = ucRegVal/4 + 1;
    else
        ucRegVal = ucRegVal/4;
    _msWriteByte(SC12_A0, ucRegVal); //dr miu request control
    _msWriteByte(SC12_A1, ucRegVal); //dr miu last control

    //set the base address in DRAM for dr blk
    dwBase = (MIU_DRBLK_ADDR_START>>2);             //unit: 4 byte
    _msWrite2Byte(SC12_88, dwBase);                   //dr blk rd base low
    _msWrite2ByteMask(SC12_8A, dwBase>>16, 0x007F);  //dr blk rd base high
    //set the base address in DRAM for dr pix
    dwBase = (MIU_DRPIX_ADDR_START>>2);             //unit: 4 byte
    _msWrite2Byte(SC12_8C, dwBase);                   //dr pix rd base low
    _msWrite2ByteMask(SC12_8E, dwBase>>16, 0x007F);  //dr pix rd base high

    _msWriteByte(SC12_84,0x80);  //enable dr to read miu

    //DD settings
    _msWrite2ByteMask(SC14_02, PANEL_WIDTH, 0x0FFF);     //frame width
    _msWrite2ByteMask(SC14_04, PANEL_HEIGHT, 0x0FFF);    //frame height

    //set the values of dd miu request control and dd miu last control
    //equal to ceiling(PANEL_WIDTH / BLOCK_WIDTH)  * (32 bytes / number of bytes per dd request)
    //e.g. ceiling(1920/32) * (32/16)
    if(PANEL_WIDTH & 0x1F)
        ucRegVal = PANEL_WIDTH/32 + 1;
    else
        ucRegVal = PANEL_WIDTH/32;
    ucRegVal *= 2;
    _msWriteByte(SC14_0A, ucRegVal); //dd miu request control
    _msWriteByte(SC14_0B, ucRegVal); //dd miu last control

    //set the base address in DRAM for dd
    dwBase = (MIU_DD_ADDR_START>>2);                //unit: 4 byte
    _msWrite2Byte(SC14_0E, dwBase);                  //dd det wr base low  sw allocate
    _msWrite2ByteMask(SC14_0C, dwBase>>16, 0x01FF);  //dd det wr base high sw allocate

    _msWriteByte(SC14_0D, 0x80); //enable dd to write miu

    _msWrite2ByteMask(SC13_C2, PANEL_WIDTH, 0x3FFF);     //frame width
    _msWrite2ByteMask(SC13_C4, PANEL_HEIGHT, 0x1FFF);    //frame height

    _msWriteByte(SC13_C0, 0x07); //T3D line buffer delay 1 h-blanking for senting out the last line
    _msWriteByte(SC13_C6, 0x03); //[1] depthlb_en,[0] srclb_en
    //[0]reg_render_control (always enable in passive mode),
    //[1]reg_lr_control_l = 0, [2]reg_lr_control_r = 1 (passive mode, first line is L)
    //[1]reg_lr_control_l = 1, [2]reg_lr_control_r = 0 (passive mode, first line is R)
    //[1]reg_lr_control_l = 1, [2]reg_lr_control_r = 1 (active mode)
    _msWriteByte(SC13_D0, 0x05);
    //[4]MTV_BYPASS_EN enable effect of gain and offset effect (1), disable effect gain and offset effect (0)
    _msWriteByte(SC13_10, 0x30);

    //=======to be done======================
    //The number of levels, default vaules of gains, offsets for OSD
    //are going to disscuss with RD, FAE and customers.
    //The gain and offset values here are just for temporary use.
    #if ENABLE_OSD_3D_CONTROL
    Sc2DTo3DSetPIXGain(t2Dto3DPIXGain[UserPref2Dto3DGain]);
    Sc2DTo3DSetBLKGain(t2Dto3DBLKGain[UserPref2Dto3DGain]);
    Sc2DTo3DSetPIXOffset(t2Dto3DPIXOffset[UserPref2Dto3DOffset]);
    Sc2DTo3DSetBLKOffset(t2Dto3DBLKOffset[UserPref2Dto3DOffset]);
    #endif

    _msWriteByteMask(SC0_CE, BIT2, BIT2);    //enable vsync int


}

void Sc2DTo3DOff(void)
{
    MS2DTo3D_printMsg("-------------Sc2DTo3DOff------------");
    _msWriteByte(SC12_C2, 0x00);         //disable depth render
    _msWriteBit(SC13_10, 0, BIT4);       //disable effect of gain and offset

    _msWriteByte(REG_2080,0x00);
    _msWriteByteMask(SC0_CE, 0,BIT2);    //disable vsync int
    msMiuProtectCtrl(MIU_PROTECT_0,_ENABLE,0, MCU_ON_DRAM_START_ADDR, MCU_CODE_SIZE );
}

void Sc2DTo3DSetPIXGain(WORD pixGain)
{
    //2' complement (9 bits)
    _msWrite2ByteMask(SC13_CA, pixGain, 0x01FF);         //pixel left gain (+)
    _msWrite2ByteMask(SC13_CC, ~pixGain+0x01, 0x01FF);   //pixel right gain (-)
}

void Sc2DTo3DSetPIXOffset(BYTE pixOffset)
{
    _msWriteByte(SC13_CE, pixOffset); //pixel left offset
    _msWriteByte(SC13_CF, pixOffset); //pixel right offset
}

void Sc2DTo3DSetBLKGain(BYTE blkGain)
{
    //use MSB as sign bit
    _msWriteByte(SC13_0D, blkGain);      //block left gain (+)
    _msWriteByte(SC13_1D, blkGain|0x80); //block right gain (-)
}

void Sc2DTo3DSetBLKOffset(BYTE blkOffset)
{
    _msWriteByte(SC13_0C, blkOffset); //block left offset
    _msWriteByte(SC13_1C, blkOffset); //block right offset

}

#endif

