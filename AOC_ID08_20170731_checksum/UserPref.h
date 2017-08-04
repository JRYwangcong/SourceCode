#include "board.h"
extern void Init_MonitorSetting(void);
extern void Init_ModeSetting(void);
extern void CheckMonitorSettingRange(void);
extern void SaveMonitorSetting(void);
extern void SaveMonitorSetting2(void);
extern void SaveModeSetting(void);
extern void ReadMonitorSetting(void);
extern void ReadModeSetting(void);
extern void SaveBlacklitTime(void);
extern void ReadBlacklitTime(void);
extern void CheckColorValueRange(void);
extern WORD CalculateSettingCheckSum(BYTE* Array, BYTE Len);
#if USEFLASH
extern void SaveFactorySetting(void);
#endif
extern void Set_Flash_Function_Addr(void);

#if EnablePanelServiceMenu
extern void SavePanelSetting(void);
#endif
#if MWEFunction
typedef enum
{
	NORMAL,
	BLUE,
	GREEN,
	FLESHTONE,
	Y_FLESHTONE,
	MAX_COLOR_ENHANCE_TYPE
} ColorEnhanceType;

typedef enum
{
	OFF_W,
	WINDOW_W,
	SPLIT_W,
	FULL_W,
	MAX_MWE
} MWEType;

typedef enum
{
	OFF_SKIN,
	NORMAL_SKIN,
	RED_SKIN,
	YELLOW_SKIN,
	MAX_SKIN
} SkinToneType;

typedef enum
{
	OFF_DLC,
	MODE1_DLC,
	MODE2_DLC,
	MODE3_DLC,
	MAX_DLC
} DLCMode;

typedef enum
{
	OFF_MWEMODE,
	MOVIE1_MWEMODE,
	MOVIE2_MWEMODE,
	MAX_MWEMODE
} MWEMode;
#endif

#if USEFLASH
extern BYTE xdata ucFlashSaveState;	// Save user Setting flag
//extern WORD xdata ucFlashSaveCount;	// Flash Save counter
extern void UserPref_EnableFlashSaveBit(BYTE ucFlag);
extern void UserPref_FlashSaveFlagCheck(void);
extern void UserPref_FlashDataSaving(void);

#define bFlashSaveMonitorBit    BIT0
#define bFlashSaveModeBit       BIT1
#define bFlashSaveFactoryBit    BIT2
#define bFlashSaveMonitor2Bit   BIT3

#define bFlashForceSaveMonitorBit    BIT4
#define bFlashForceSaveModeBit       BIT5
#define bFlashForceSaveFactoryBit    BIT6
#define bFlashForceSaveMonitor2Bit    BIT7

#define FlashSaveMonitorFlag        (ucFlashSaveState&bFlashSaveMonitorBit)
#define Set_FlashSaveMonitorFlag()  (ucFlashSaveState|=bFlashSaveMonitorBit)
#define Clr_FlashSaveMonitorFlag()  (ucFlashSaveState&=~bFlashSaveMonitorBit)

#define FlashSaveModeFlag           (ucFlashSaveState&bFlashSaveModeBit)
#define Set_FlashSaveModeFlag()     (ucFlashSaveState|=bFlashSaveModeBit)
#define Clr_FlashSaveModeFlag()     (ucFlashSaveState&=~bFlashSaveModeBit)

#define FlashSaveFactoryFlag        (ucFlashSaveState&bFlashSaveFactoryBit)
#define Set_FlashSaveFactoryFlag()  (ucFlashSaveState|=bFlashSaveFactoryBit)
#define Clr_FlashSaveFactoryFlag()  (ucFlashSaveState&=~bFlashSaveFactoryBit)

#define FlashForceSaveMonitorFlag        (ucFlashSaveState&bFlashForceSaveMonitorBit)
#define Set_FlashForceSaveMonitorFlag()  (ucFlashSaveState|=bFlashForceSaveMonitorBit)
#define Clr_FlashForceSaveMonitorFlag()  (ucFlashSaveState&=~bFlashForceSaveMonitorBit)

#define FlashForceSaveModeFlag           (ucFlashSaveState&bFlashForceSaveModeBit)
#define Set_FlashForceSaveModeFlag()     (ucFlashSaveState|=bFlashForceSaveModeBit)
#define Clr_FlashForceSaveModeFlag()     (ucFlashSaveState&=~bFlashForceSaveModeBit)

#define FlashForceSaveFactoryFlag        (ucFlashSaveState&bFlashForceSaveFactoryBit)
#define Set_FlashForceSaveFactoryFlag()  (ucFlashSaveState|=bFlashForceSaveFactoryBit)
#define Clr_FlashForceSaveFactoryFlag()  (ucFlashSaveState&=~bFlashForceSaveFactoryBit)

#define FlashSaveMonitor2Flag        (ucFlashSaveState&bFlashSaveMonitor2Bit)
#define Set_FlashSaveMonitor2Flag()  (ucFlashSaveState|=bFlashSaveMonitor2Bit)
#define Clr_FlashSaveMonitor2Flag()  (ucFlashSaveState&=~bFlashSaveMonitor2Bit)

#define FlashForceSaveMonitor2Flag        (ucFlashSaveState&bFlashForceSaveMonitor2Bit)
#define Set_FlashForceSaveMonitor2Flag()  (ucFlashSaveState|=bFlashForceSaveMonitor2Bit)
#define Clr_FlashForceSaveMonitor2Flag()  (ucFlashSaveState&=~bFlashForceSaveMonitor2Bit)
#endif