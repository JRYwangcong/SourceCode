#include "Types.h"
#include "i2cdef.h"
#include "i2c.h"
#include "Board.h"
#include "Global.h"
//#include "mStar.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "GPIO_DEF.h"
#if !USEFLASH
#define NVRAM_DEVICE    0xA0
void NVRam_WriteByte( WORD addr, BYTE value )
{
    Clr_EEPROM_WP(); //wumaozhong 20050331
    i2c_WriteTBL( NVRAM_DEVICE, addr, &value, 1 );
    Set_EEPROM_WP(); //wumaozhong 20050331
}

void NVRam_WriteWord(WORD addr, WORD value)
{
    Clr_EEPROM_WP();
    i2c_WriteTBL(NVRAM_DEVICE, addr, (BYTE*)&value, 2);
    Set_EEPROM_WP();
}

void NVRam_ReadByte( WORD addr, BYTE *value )
{
    i2c_ReadTBL( NVRAM_DEVICE, addr, value, 1 );
}

#if (EEPROM >= EEPROM_24C32)
void NVRam_WriteTbl( WORD addr, BYTE *buffer, WORD count )
{
    Clr_EEPROM_WP(); //wumaozhong 20050331
    i2c_WriteTBL( NVRAM_DEVICE, addr, buffer, count );
    Set_EEPROM_WP(); //wumaozhong 20050331
}

void NVRam_ReadTbl( WORD addr, BYTE *buffer, WORD count )
{
    i2c_ReadTBL( NVRAM_DEVICE, addr, buffer, count );
}
#else
void NVRam_WriteTbl( WORD addr, BYTE *buffer, BYTE count )
{
    Clr_EEPROM_WP(); //wumaozhong 20050331
    i2c_WriteTBL( NVRAM_DEVICE, addr, buffer, count );
    Set_EEPROM_WP(); //wumaozhong 20050331
}

void NVRam_ReadTbl( WORD addr, BYTE *buffer, BYTE count )
{
    i2c_ReadTBL( NVRAM_DEVICE, addr, buffer, count );
}
#endif

#if 0
void Device_WriteByte( BYTE deviceAddr, WORD addr, BYTE value )
{
    i2c_WriteTBL( NVRAM_DEVICE, addr, &value, 1 );
}
void Device_ReadByte( BYTE deviceAddr, WORD addr, BYTE *value )
{
    i2c_ReadTBL( deviceAddr, addr, value, 1 );
}
#endif
#endif

