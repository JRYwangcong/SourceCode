extern BYTE code DVIPhaseTbl[];
extern void mStar_MonitorInputTiming(void);
extern void mStar_ModeHandler(void);
extern Bool mStar_ValidTimingDetect(void);
extern BYTE mStar_FineTuneDVIPhase(BYTE channelReg, BYTE channel);
extern Bool mStar_IsStatusChanged(void);
extern Bool IsCableNotConnected(void);