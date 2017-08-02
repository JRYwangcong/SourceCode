#ifndef _HWI2C_H_
#define _HWI2C_H_

#ifdef _HWI2C_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define MIIC0                   0
#define MIIC1                   1

#define MIIC0_1_ENABLE          _BIT5   //MIIC0 on GPIO00 GPIO01
#define MIIC0_2_ENABLE          _BIT6   //MIIC0 on GPIO04 GPIO05
#define MIIC1_0_ENABLE          _BIT7   //MIIC1 on GPIO53 GPIO52

#define MIIC0_ENABLE_MASK       _BIT6|_BIT5
#define MIIC1_ENABLE_MASK       _BIT7

#define HWI2C0_REG_BANK         0x111800
#define HWI2C1_REG_BANK         0x111900

#define MIICRegBase(X)          (((X)==0)?(HWI2C0_REG_BANK):(HWI2C1_REG_BANK))

#define HWI2C_CONFIG_REG(X)     MIICRegBase(X) + 0x00
#define HWI2C_WDATA_REG(X)      MIICRegBase(X) + 0x04
#define HWI2C_RDATA_REG(X)      MIICRegBase(X) + 0x06
#define HWI2C_START_CNT_REG(X)  MIICRegBase(X) + 0x18
#define HWI2C_STOP_CNT_REG(X)   MIICRegBase(X) + 0x10
#define HWI2C_HIGH_CNT_REG(X)   MIICRegBase(X) + 0x12
#define HWI2C_LOW_CNT_REG(X)    MIICRegBase(X) + 0x14
#define HWI2C_SDA_CNT_REG(X)    MIICRegBase(X) + 0x16
#define HWI2C_LATCH_CNT_REG(X)  MIICRegBase(X) + 0x1a

#define MIIC_STATE(X)           (msReadByte(MIICRegBase(X)+0x0A))
#define WAIT_READY(X)           (msReadByte(MIICRegBase(X)+0x08) & BIT0)
#define CHECK_NONACK(X)         (msReadByte(MIICRegBase(X)+0x05) & BIT0)
#define CLEARFLAG(X)            (msWriteByte(MIICRegBase(X)+0x08, 0x00))
#define STARTBIT(X)             (msWriteByte(MIICRegBase(X)+0x02, 0x01))
#define STOPBIT(X)              (msWriteByte(MIICRegBase(X)+0x03, 0x01))
#define ACK_REV(X)              (msWriteByte(MIICRegBase(X)+0x07, 0x01))
#define NACK_REV(X)             (msWriteByte(MIICRegBase(X)+0x07, 0x03))

/* The error numbers of master I2C */
#define EMIICSTART  1
#define EMIICSTOP   2
#define EMIICSTATE  3
#define EMIICSB     4
#define EMIICRB     5
#define EMIICACK    6

INTERFACE void hw_i2c_intial(BYTE);
INTERFACE BYTE hw_i2c_start(BYTE);
INTERFACE BYTE hw_i2c_stop(BYTE);
INTERFACE BYTE hw_i2c_send_byte(BYTE, BYTE);
INTERFACE BYTE hw_i2c_receive_byte(BYTE, BOOL, BYTE *);
INTERFACE BOOL hw_i2c_burst_write(BYTE, WORD, BYTE *);
INTERFACE BOOL hw_i2c_burst_read(BYTE, BYTE, BYTE *);

#if (USE_SW_I2C == 0)
INTERFACE void i2C_Intial(void);
INTERFACE BOOL i2c_Start(void);
INTERFACE void i2c_Stop(void);
INTERFACE BOOL i2c_SendByte(BYTE ucWriteData);
INTERFACE BYTE i2c_ReceiveByte(BOOL bAck);
#endif

#undef INTERFACE

#endif

