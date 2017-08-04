extern void Init_MCU(void);
#if MySon 
    extern void EnableRS232(void);
#else 
    extern void EnableISP(void);
#endif
#if Enable_Cache
extern void MCU_EnableCache(void);
extern void MCU_DisableCache(void);
#endif
//extern void MCU_EnableCache(void);
//extern void MCU_DisableCache(void);
