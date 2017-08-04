#include "board.h"
extern void mStar_AdjustBrightness(BYTE brightness);
extern void mStar_AdjustDcrBrightness(void);
extern void mStar_AdjustBlackLevel(BYTE BlackLevel);
extern void mStar_AdjustUserPrefBlacklevel(BYTE Redblacklevel, BYTE Greenblacklevel, BYTE Blueblacklevel);
extern void mStar_AdjustContrast(BYTE contrast);
extern void mStar_AdjustHTotal(WORD htotal);
extern void mStar_AdjustPhase(BYTE phase);
extern void mStar_AdjustHPosition(WORD position);
extern void mStar_AdjustVPosition(WORD position);
#if UsesRGB
    void mStar_AdjustRedColor(BYTE rColor, BYTE gColor, BYTE bColor);
    void mStar_AdjustGreenColor(BYTE rColor, BYTE gColor, BYTE bColor);
    void mStar_AdjustBlueColor(BYTE rColor, BYTE gColor, BYTE bColor);
#else 
    void mStar_AdjustRedColor(BYTE color, BYTE contrast);
    void mStar_AdjustGreenColor(BYTE color, BYTE contrast);
    void mStar_AdjustBlueColor(BYTE color, BYTE contrast);
#endif 
extern void mStar_AdjustVolume(BYTE volume);
extern void mStar_AdjustAdcRedGain(BYTE gain);
extern void mStar_AdjustAdcGreenGain(BYTE gain);
extern void mStar_AdjustAdcBlueGain(BYTE gain);
extern void mStar_AdjustAdcGain(BYTE rGain, BYTE gGain, BYTE bGain);
extern void mStar_AdjustAdcOffset(BYTE rOffset, BYTE gOffset, BYTE bOffset);
extern void mStar_AdjustAdcRedOffset(BYTE offset);
extern void mStar_AdjustAdcGreenOffset(BYTE offset);
extern void mStar_AdjustAdcBlueOffset(BYTE offset);
extern void mStar_WaitForDataBlanking(void);
extern void mStar_AdjustBackgoundColor(BYTE pattern);
