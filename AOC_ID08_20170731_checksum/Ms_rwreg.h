extern void mStar_WriteByte(BYTE addr, BYTE value);
extern void mStar_WriteWord(BYTE addr, WORD value);
extern BYTE mStar_ReadByte(BYTE addr);
extern void mStar_WriteDDRByte(BYTE addr, BYTE value);
extern WORD mStar_ReadWord(BYTE addr);
extern void mStar_WriteTBL(BYTE addr, BYTE *tbl, WORD count, BYTE inc);
extern void msWriteByteMask(BYTE ucReg, BYTE ucValue, BYTE ucMask);
#if (defined TSUMXXT)||(defined TSUMXXNX)	//110311 Modify
extern void mStar_WriteByteMask(BYTE addr, BYTE value, BYTE mask);
#endif
