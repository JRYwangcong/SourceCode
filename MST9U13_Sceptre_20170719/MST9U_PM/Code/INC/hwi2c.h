#ifndef     HWI2C_H
#define     HWI2C_H

#define MIIC0_1_ENABLE        _BIT5      //MIIC0 on GPIO00 GPIO01
#define MIIC0_2_ENABLE        _BIT6      //MIC0 on GPIO04 GPIO05
#define MIIC1_0_ENABLE          _BIT7     //MIIC1 on GPIO53 GPIO52

#define MIIC0_ENABLE_MASK  _BIT6|_BIT5
#define MIIC1_ENABLE_MASK  _BIT7

//=================================================
//MIIC 0 MIIC1 enable
#define MIIC0_ENABLE   (MIIC0_1_ENABLE)
#define MIIC1_ENABLE   (0)
//==================================================
#if MIIC1_ENABLE
#define HWI2C_REG_BANK           0x111900
#else
#define HWI2C_REG_BANK           0x111800
#endif
#define HWI2C_CONFIG_REG    HWI2C_REG_BANK + 0x00 
#define HWI2C_WDATA_REG     HWI2C_REG_BANK + 0x04
#define HWI2C_RDATA_REG     HWI2C_REG_BANK + 0x06

#define HWI2C_START_CNT_REG     HWI2C_REG_BANK + 0x18
#define HWI2C_STOP_CNT_REG      HWI2C_REG_BANK + 0x10
#define HWI2C_HIGH_CNT_REG      HWI2C_REG_BANK + 0x12
#define HWI2C_LOW_CNT_REG       HWI2C_REG_BANK + 0x14
#define HWI2C_SDA_CNT_REG       HWI2C_REG_BANK + 0x16
#define HWI2C_LATCH_CNT_REG     HWI2C_REG_BANK + 0x1a

extern void i2C_Intial(void);
extern BOOL i2c_Start(void);
extern void i2c_Stop(void);
extern bit i2c_SendByte(BYTE ucWriteData);
extern BYTE i2c_ReceiveByte(BOOL bAck);

#endif

