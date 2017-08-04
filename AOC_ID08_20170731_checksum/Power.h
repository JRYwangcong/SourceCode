extern void Power_PowerHandler(void);
extern void Power_PowerOnSystem(void);
extern void Power_PowerOffSystem(void);
extern void Power_TurnOnPanel(void);
extern void Power_TurnOffPanel(void);
extern void Power_TurnOffLed(void);
extern void Power_TurnOnGreenLed(void);
extern void Power_TurnOffGreenLed(void);
extern void Power_TurnOnAmberLed(void);
extern void Power_TurnOnPanel(void);
extern void Power_TurnOffPanel(void);
extern void Set_GreenLedFlash(BYTE times);
extern void Set_AmberLedFlash(BYTE times);
#if PanelType==PanelCMO220Z1M01
extern void hw_ClrPanel(void);
extern void hw_SetPanel(void);
extern void Set_CLK(void);
#endif
extern void PowerSavingLEDBreathing(void);
extern void InitialP06ToPWM1(Bool sel);