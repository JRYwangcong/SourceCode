#ifndef _DDCCOLORHANDLER_H
#define _DDCCOLORHANDLER_H

#include "Global.h"
#include "Adjust.h"
#include "drvDeltaE.h"
#include "drvMCCSCal.h"
#include "drvDemura.h"

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#define MStar_COMMAND               0xCC
#define MS_EN_INTERNAL_PATTERN      0x30
#define MS_SET_INTERNAL_PATTERN     0x31
#define MS_EN_COLOR_ENGINE_PATTERN  0x34
#define MS_SET_COLOR_ENGINE_PATTERN 0x35
#define MS_GET_MODEL_NAME           0x36
#define MS_BRIGHTNESS               0x37
#define MS_ColorEngine_OnOff   	    0x40
#define MS_Read_Status              0x8F
#define MS_Read_ACK                 0x8E
#define MS_WR_BLOCK                 0x90
#define MS_DDCWriteReg      	    0xA7
#define MS_DDCReadReg               0xA8
#define MS_GetToolFlagStatus        0xA9
#define MS_GetCommonVersion         0x3A
#define MS_RunISP                                   0xFA
#define MS_GetMemAddr                          0xF0

extern BYTE ColorCalibrationHandler(BYTE CommandCode);

//-------------------------------------------------------------------------------
//read write register by ddc
//-------------------------------------------------------------------------------
extern void msDDCWriteReg(BYTE u8DispWin,DWORD addr,BYTE value);
extern BYTE msDDCReadReg(BYTE u8DispWin,DWORD addr);


//-------------------------------------------------------------------------------
// SendFileConfig is a protocol for tool send a file to FW via Memory
//-------------------------------------------------------------------------------
typedef struct
{
    //Mem Config
    DWORD memAddr;   //DRAM mapping address, defalut : 0x87E00000;
    DWORD txHeaderOft; //Write by tool, read by FW. size is 4K bytes for dram mapping addr align.
    DWORD rxHeaderOft; //Write by FW, read by Tool. size is 4K bytes for dram mapping addr align.
    DWORD headerSize; //Write by FW, read by Tool.

    //Config by tool
    DWORD fileSize;           //Size of File, addr : memAddr + 0x0000;
    DWORD fileSentLen;     //File sent length  , addr : memAddr + 0x0004;
    BOOL bTerminated;  //Terminate the send file procedure, addr : memAddr + 0x0008;

    //Updated by FW
    DWORD fileRecvLen;    //Recieved lenght of File   , addr : memAddr + 0x000C;
    DWORD checkSum;      //Flash Wrote length;   , addr : memAddr + 0x0010;

    //buffer
    BYTE* pBuffer;            //Allocated buffer;

} SendFileConfig; //a Protocol for tool send a file to FW

/*DWORD ReadFileSize(void);
DWORD ReadSentFileLen(void);
BOOL ReadTerminatedState(void);
void WriteRecvFileLen(DWORD val);
void WriteCheckSum(DWORD val);
*/
#endif

