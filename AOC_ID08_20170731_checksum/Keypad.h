extern xdata BYTE KeypadButton;
#ifdef PowerKeyDeBounce
extern bit PushPowerKeyFlag;
extern BYTE xdata PushPowerKeyDeBounce;
#endif
extern void Key_ScanKeypad(void);
extern void CheckFactoryKeyStatus(void);
