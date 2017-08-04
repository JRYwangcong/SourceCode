extern void mStar_WriteDDC1(void);
extern void mStar_WriteDDC2(void);
#ifdef ReduceDDC
extern void mStar_InitDDC(void);
extern void mStar_CheckRAM(void);
extern void mStar_ClrDDC_WP(void);
extern void mStar_SetDDC_WP(void);
#endif
#ifdef  UseVGACableReadWriteAllPortsEDID
extern void WriteDevice(BYTE DeviceType,WORD Address, BYTE Length);
extern void ReadDevice(BYTE DeviceType,WORD Address, BYTE Length);
#endif
#if F_ResetCheckEdidManueAndSN
extern Bool FactoryCheckEdidManufactureAndSN( void );
#endif

