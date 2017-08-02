
#ifndef _AMP_I2S_TAS5707_H_
#define _AMP_I2S_TAS5707_H_





extern WORD A5707WriteReg(BYTE u1Addr, BYTE *pu1Buf, WORD u2ByteCnt);
extern WORD A5707ReadReg(BYTE u1Addr, BYTE *pu1Buf, WORD u2ByteCnt);
extern void AMPReset(BOOL bReset);
extern void AMPHWMute(BOOL bMute);
extern WORD AMP_Init(void);
extern WORD AMP_DeInit(VOID);
extern  WORD AMP_SetMute(BOOL bMute);
extern WORD AMP_GetMute(BOOL* pbMute);
extern WORD AMP_SetSubWooferVol(BYTE u32Volume);
extern WORD AMP_WriteReg(WORD u32RegAddr, WORD u32ByteSize, BYTE* pu8Value);
extern WORD AMP_ReadReg(WORD u32RegAddr, WORD u32ByteSize, BYTE* pu8Value);




#endif


