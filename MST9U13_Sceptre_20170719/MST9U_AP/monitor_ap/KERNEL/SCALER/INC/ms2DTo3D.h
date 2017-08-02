#ifndef _MS_2DTo3D_H_
#define _MS_2DTo3D_H_

#ifdef _MS_2DTo3D_C_
#define MS_2DTo3DDec
#else
#define MS_2DTo3DDec   extern
#endif

#if 0//ENABLE_R2_2DTO3D
//typedef struct
//{
//    BYTE ucOffset;
//    BYTE ucDepth;
//}Sc2DTo3DInfo;

//MS_2DTo3DDec xdata Sc2DTo3DInfo g_Sc2DTo3DInfo;

MS_2DTo3DDec void Sc2DTo3DInit(void);
MS_2DTo3DDec void Sc2DTo3DOff(void);
MS_2DTo3DDec void Sc2DTo3DSetPIXGain(WORD pixGain);
MS_2DTo3DDec void Sc2DTo3DSetPIXOffset(BYTE pixOffset);
MS_2DTo3DDec void Sc2DTo3DSetBLKGain(BYTE blkGain);
MS_2DTo3DDec void Sc2DTo3DSetBLKOffset(BYTE blkOffset);

#endif

#undef MS_2DTo3DDec
#endif
