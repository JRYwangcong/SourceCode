#include "i2c.h"

extern void NVRam_WriteByte( WORD addr, BYTE value );
extern void NVRam_WriteWord( WORD addr, WORD value );
extern void NVRam_ReadByte( WORD addr, BYTE *value );

#if (EEPROM >= EEPROM_24C32)
extern void NVRam_WriteTbl( WORD addr, BYTE *buffer, WORD count );
extern void NVRam_ReadTbl( WORD addr, BYTE *buffer, WORD count );
#else
extern void NVRam_WriteTbl( WORD addr, BYTE *buffer, BYTE count );
extern void NVRam_ReadTbl( WORD addr, BYTE *buffer, BYTE count );
#endif
