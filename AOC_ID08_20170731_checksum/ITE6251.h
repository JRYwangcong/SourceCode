
#if IT6251
extern void InitIT6251(void);
extern void CheckIT6251(void);
extern void IT6251_PowerOn();
extern void IT6251_PowerDown() ;
extern void IT6251_DeviceLoopProcedure(void);
extern BYTE  ReadI2C_Byte(BYTE addr, BYTE offset);
extern BYTE  WriteI2C_Byte(BYTE addr, BYTE offset, BYTE d);
#endif

