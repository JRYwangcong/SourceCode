

#ifndef _APPPANEL_H_
#define _APPPANEL_H_

#ifdef _APPPPANEL_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif

typedef enum _Panel_I2cIoTransType
{
    PANEL_I2C_TRANS_READ,
    PANEL_I2C_TRANS_WRITE
}Panel_I2cIoTransType;

#define PANEL_I2C_DEVICE_ADR_WRITE(slave_adr)   (slave_adr & ~_BIT0)
#define PANEL_I2C_DEVICE_ADR_READ(slave_adr)    (slave_adr | _BIT0)

#define PANEL_DEVICE    0x38
#define PANEL_I2C_ACCESS_DUMMY_TIME   3


INTERFACE void appPanel_SetPanel_QFHD_2D_Graphic_Mode(void);
INTERFACE Bool SwitchTCONDisplay(void);
#undef INTERFACE
#endif



