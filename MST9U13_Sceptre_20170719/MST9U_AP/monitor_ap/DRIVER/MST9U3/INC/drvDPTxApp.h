#ifndef _DPTxApp_H_
#define _DPTxApp_H_

////////////////////////////////////////////////////////////
// ####  Fix Parameter for eDPTX (Do Not Change this Parameter) ####
////////////////////////////////////////////////////////////
#define eDPTXHBR2    0x14
#define eDPTXHBR      0x0A
#define eDPTXRBR      0x06
#define eDPTX4Lanes  4
#define eDPTX2Lanes  2
#define eDPTX1Lanes  1
// ******************************************************
#define eDPMCCSADDR 0x000037

// ######  external function call ###########################
extern void DPTxInit(void);
extern void DPTxHandle(void);
extern void DPTxSetMSA(BYTE PortNum);
extern void DPTxSetSCFastLPLL(BOOL IsVGA, WORD VGAHtt, WORD VGAVtt, WORD InVFreq);
extern Bool DPTxAuxDPCDReadByte( BYTE PortNum, DWORD  DPCDADDR , BYTE *pRdData);
extern Bool DPTxAuxDPCDReadBytes( BYTE PortNum, DWORD  DPCDADDR , BYTE Length , BYTE *pRxBuf);
extern Bool DPTxAuxDPCDWriteByte( BYTE PortNum, DWORD  DPCDADDR , BYTE wData);
extern Bool DPTxAuxDPCDWriteBytes (BYTE PortNum, DWORD  DPCDADDR, BYTE Length ,BYTE *pTxBuf);
extern Bool DPTxAuxEDIDReadNLCMD( BYTE PortNum, DWORD  DPCDADDR );
extern Bool DPTxAuxEDIDWriteByte( BYTE PortNum, DWORD  DPCDADDR , BYTE wData);
extern Bool DPTxAuxEDIDReadBytes( BYTE PortNum, DWORD  DPCDADDR , BYTE Length , BYTE *pRxBuf);
extern Bool DPTxReadEDID(BYTE PortNum);
extern Bool DPTxAuxMCCSReadBytes( BYTE PortNum, BYTE Length, BYTE EOF, BYTE *pRxBuf);
extern Bool DPTxAuxMCCSWriteBytes(BYTE PortNum, BYTE Length, BYTE EOF ,BYTE *pTxBuf);
extern Bool DPTxCheckHPD(BYTE HPDPortNum);
extern Bool DPTxCheckHPDIRQ(BYTE HPDPortNum);
extern Bool DPTxTraining(BYTE PortNum, BYTE LinkRate, BYTE LaneCount);
extern void DPTxHBR2DRVMAINTune(BYTE LaneNum,BYTE SetValue);
extern void DPTxHBR2DRVPRETune(BYTE LaneNum,BYTE SetValue);
extern void DPTxHBR2PREDRVMAINTune(BYTE LaneNum,BYTE SetValue);
extern void DPTxHBR2PREDRVPRETune(BYTE LaneNum,BYTE SetValue);
extern void DPTxHBR2SetSwPre(BYTE LinkRate,BYTE LaneNum,BYTE SwLv, BYTE PreLv);
extern void DPTxSwingTune(BYTE PortNum, BYTE SwingValue);
extern void DPTxPreEmphasisTune(BYTE PortNum, BYTE PremphasisValue);
extern void DPTxSSCEnable(BYTE bEnable, BYTE LinkRate);
extern void DPTxPGSyncRst(void);
extern void DPTxSwingCal(void);
extern void DPTxPreemphasisCal(void);
extern void DPTxHPDIRQRst(void);
extern void DPTxATSTPrintErrCNT(BYTE PortNum);

// ######  internal function call ###########################
void DPTxConfigLinkRate(BYTE LinkRate);
void DPTxVDClkRst(void);
void DPTxMLSignalEn(BYTE PortNum, BYTE bEnable);
#endif
