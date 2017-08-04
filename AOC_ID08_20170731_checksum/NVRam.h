#if !USEFLASH
extern void NVRam_WriteByte(WORD addr, BYTE value);
extern void NVRam_WriteWord(WORD addr, WORD value);
extern void NVRam_ReadByte(WORD addr, BYTE *value);
extern void NVRam_WriteTbl(WORD addr, BYTE *buffer, BYTE count);
extern void NVRam_ReadTbl(WORD addr, BYTE *buffer, BYTE count);
#endif

#if VGA_EDID_Write_To_24C02
extern void NVRam_WriteByte(WORD addr, BYTE value);
extern void NVRam_ReadByte(WORD addr, BYTE *value);
#endif

