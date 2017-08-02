
#ifndef _MS_3DLUT_H_
#define _MS_3DLUT_H_
#define ENABLE_3DLUT_Test          1


typedef enum
{
    e3dLutDelta1X = 0,
    e3dLutDelta2X,
    e3dLutDelta4X,
    e3dLutDelta8X
} _3dLutDeltaEnum;

//extern void ms3DLutEnable(BYTE window, BYTE enable);
extern void drv3DLutInit(BYTE u8WinIndex);

extern void mStar_Setup3DLUT( BYTE u8WinIndex, BYTE u8ColorMode, BOOL bEnable );
extern void ms3DLutEnable(BYTE u8WinIndex, BYTE enable);

extern void drv3DLut_WriteCustomTable(BYTE u8WinIndex);

extern void drv3DLut_WriteCompressedLUT(BYTE u8WinIndex, BYTE* u8CompressTbl);
extern void ms3DLutGrayGuard(BYTE u8WinIndex, BOOL bGrayGuard);
#endif

