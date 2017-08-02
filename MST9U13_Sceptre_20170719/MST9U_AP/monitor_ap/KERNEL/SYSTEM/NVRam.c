#include "datatype.h"
#include "i2cdef.h"
#include "i2c.h"
#include "Board.h"
#include "Global.h"
#include "mStar.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "GPIO_DEF.h"
#include "NVRam.h"

#define NVRAM_MAP_SYNC_RANGE_VALID      (addr<NVRAM_SIZE)

#if !USEFLASH
void NVRam_WriteByte( WORD addr, BYTE value )
{
    Clr_EEPROM_WP(); //wumaozhong 20050331
#if ENABLE_NVRAM_MAP_SYNC
    if( NVRAM_MAP_SYNC_RANGE_VALID() )
    {
        u8NVRAM_MAP[addr] = value;
        u8NVRAM_MAP_BK[addr] = value;
    }
#endif
    i2c_WriteTBL( NVRAM_DEVICE, addr, &value, 1 );
    Set_EEPROM_WP(); //wumaozhong 20050331
}

void NVRam_WriteWord(WORD addr, WORD value)
{
    Clr_EEPROM_WP();
#if ENABLE_NVRAM_MAP_SYNC
    if( NVRAM_MAP_SYNC_RANGE_VALID() )
    {
        u8NVRAM_MAP[addr] = (BYTE)(value&0x00FF);
        u8NVRAM_MAP_BK[addr] = (BYTE)(value&0x00FF);
        u8NVRAM_MAP[addr+1] = (BYTE)(value>>8);
        u8NVRAM_MAP_BK[addr+1] = (BYTE)(value>>8);
    }
#endif
    i2c_WriteTBL(NVRAM_DEVICE, addr, (BYTE*)&value, 2);
    Set_EEPROM_WP();
}

void NVRam_ReadByte( WORD addr, BYTE *value )
{
    i2c_ReadTBL( NVRAM_DEVICE, addr, value, 1 );
#if ENABLE_NVRAM_MAP_SYNC
    if( NVRAM_MAP_SYNC_RANGE_VALID() )
    {
        u8NVRAM_MAP[addr] = *value;
        u8NVRAM_MAP_BK[addr] = *value;
    }
#endif
}

void NVRam_ReadWord( WORD addr, WORD *value )
{
    i2c_ReadTBL( NVRAM_DEVICE, addr, (BYTE*)value, 2 );
#if ENABLE_NVRAM_MAP_SYNC
    if( NVRAM_MAP_SYNC_RANGE_VALID() )
    {
        u8NVRAM_MAP[addr] = (BYTE)((*value)&0xFF);
        u8NVRAM_MAP_BK[addr] = (BYTE)((*value)&0xFF);
        u8NVRAM_MAP[addr+1] = (BYTE)((*value)>>8);
        u8NVRAM_MAP_BK[addr+1] = (BYTE)((*value)>>8);
    }
#endif
}

#if ENABLE_NVRAM_MAP_SYNC
static void NVRAM_TblMapSync(WORD addr, BYTE *buffer, WORD count)
{
    WORD i;

    if( NVRAM_MAP_SYNC_RANGE_VALID() )
    {
        for( i = 0 ; i < count ; i++ )
        {
            u8NVRAM_MAP[addr+i] = buffer[i];
            u8NVRAM_MAP_BK[addr+i] = buffer[i];
        }
    }
}
#endif

#if (EEPROM >= EEPROM_24C32)
void NVRam_WriteTbl( WORD addr, BYTE *buffer, WORD count )
{
    Clr_EEPROM_WP(); //wumaozhong 20050331
#if ENABLE_NVRAM_MAP_SYNC
    NVRAM_TblMapSync(addr, buffer, count);
#endif
    i2c_WriteTBL( NVRAM_DEVICE, addr, buffer, count );
    Set_EEPROM_WP(); //wumaozhong 20050331
}

void NVRam_ReadTbl( WORD addr, BYTE *buffer, WORD count )
{
    i2c_ReadTBL( NVRAM_DEVICE, addr, buffer, count );
#if ENABLE_NVRAM_MAP_SYNC
    NVRAM_TblMapSync(addr, buffer, count);
#endif
}
#else
void NVRam_WriteTbl( WORD addr, BYTE *buffer, BYTE count )
{
    Clr_EEPROM_WP(); //wumaozhong 20050331
#if ENABLE_NVRAM_MAP_SYNC
    NVRAM_TblMapSync(addr, buffer, (WORD)count);
#endif
    i2c_WriteTBL( NVRAM_DEVICE, addr, buffer, count );
    Set_EEPROM_WP(); //wumaozhong 20050331
}

void NVRam_ReadTbl( WORD addr, BYTE *buffer, BYTE count )
{
    i2c_ReadTBL( NVRAM_DEVICE, addr, buffer, count );
#if ENABLE_NVRAM_MAP_SYNC
    NVRAM_TblMapSync(addr, buffer, (WORD)count);
#endif
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
