#include "types.h"
#include "i2cdef.h"
#include "i2c.h"
#include "board.h"
#include "global.h"
#define NVRAM_DEVICE    0xA0

#if !USEFLASH
void NVRam_WriteByte(WORD addr, BYTE value)
{
    Clr_EEPROM_WP(); //wumaozhong 20050331
    i2c_WriteTBL(NVRAM_DEVICE, addr, &value, 1);
    #if Backupdata  
        if ((addr >= MonitorSettingAddr && addr < BackupdataAddress) && !DonotchangeBackFlag)
        {
            i2c_WriteTBL(NVRAM_DEVICE, (BackupdataAddress + (addr - MonitorSettingAddr)), &value, 1);
            if (DDCCIAadjustFlag)
                DDCCIAdjCounter = 4;
            else
                Set_GetChecksumFlag();
        }
    #endif 
    Set_EEPROM_WP(); //wumaozhong 20050331 
}


void NVRam_WriteWord(WORD addr, WORD value)
{
	Clr_EEPROM_WP(); 
	i2c_WriteTBL(NVRAM_DEVICE, addr, (BYTE*)&value, 2);
	Set_EEPROM_WP(); 
}
 
void NVRam_ReadByte(WORD addr, BYTE *value)
{
    i2c_ReadTBL(NVRAM_DEVICE, addr, value, 1);
}

void NVRam_WriteTbl(WORD addr, BYTE *buffer, BYTE count)
{
    Clr_EEPROM_WP(); //wumaozhong 20050331
    i2c_WriteTBL(NVRAM_DEVICE, addr, buffer, count);
    #if Backupdata
        if ((addr >= MonitorSettingAddr && addr < BackupdataAddress) && !DonotchangeBackFlag)
        {
            i2c_WriteTBL(NVRAM_DEVICE, BackupdataAddress + (addr - MonitorSettingAddr), buffer, count);
            if (DDCCIAadjustFlag)
                DDCCIAdjCounter = 4;
            else
                Set_GetChecksumFlag();
        }
    #endif 
    Set_EEPROM_WP(); //wumaozhong 20050331
}

void NVRam_ReadTbl(WORD addr, BYTE *buffer, BYTE count)
{
    i2c_ReadTBL(NVRAM_DEVICE, addr, buffer, count);
}


#endif

#if VGA_EDID_Write_To_24C02
void NVRam_WriteByte(WORD addr, BYTE value)
{
    //Clr_EEPROM_WP(); 
    i2c_WriteTBL(NVRAM_DEVICE, addr, &value, 1);
    //Set_EEPROM_WP(); 
}

void NVRam_ReadByte(WORD addr, BYTE *value)
{
    i2c_ReadTBL(NVRAM_DEVICE, addr, value, 1);
}
#endif



