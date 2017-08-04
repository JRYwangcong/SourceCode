extern void mStar_Init(void);
extern Bool mStar_SetupMode(void);
extern void mStar_SetupFreeRunMode(void);
extern void mStar_PrepareForTimingChange(void);
extern void mStar_SetupInputPort(void);
extern void mStar_SetAnalogInputPort(void);
extern void mStar_ResetDClkPLL(void);
extern void mStar_PowerDown(void);
extern void mStar_PowerUp(void);
extern void ScalerReset(void);
extern void CheckADC(void); // check DVI phase overflow
#if Enable_Expansion
void mStar_SetScalingFactor(BYTE ExpansionMode);
#else
extern void mStar_SetScalingFactor(void);
#endif
#if HotInputSelect
extern void mStar_PowerUp(void);
#endif
extern void SetPWMFreq(WORD freq);
extern void GPIOPWMSelect(void);
#ifdef ReduceDDC
extern void InitVccDetectPin(void);
extern void SystemVccOK(void); 
extern void ScalerResetThenPowerDown(void);
extern void ScalerIntoDeepPowerDown(void);
#endif

#if defined TSUMXXNX || defined TSUMXXQX || defined TSUMXXT		//110311 Modify
extern void I_Gen_Tuning(void);
#endif
