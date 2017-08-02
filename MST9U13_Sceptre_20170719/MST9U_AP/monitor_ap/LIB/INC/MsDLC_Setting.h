#ifndef __MSDLC_SETTING_H
#define __MSDLC_SETTING_H


typedef enum
{
    DLC_STATIC = 0,
    DLC_LOW = 1,    
    DLC_MEDIUM = 2,
    DLC_HIGH = 3,

} DLC_MODE_T;

extern void msDlcHandler(void);

extern void msDlcInit(void);
extern void msDlcOnOff(BYTE u8WinIdx, Bool bSwitch, BYTE u8MDLCode);
extern void setWinDlcOnOff(BOOL En, BYTE u8Mode );
//share with DPS 
extern void msGetWinHistogramData(void);
extern void msSetWinHistogramRange(void);
#endif 
