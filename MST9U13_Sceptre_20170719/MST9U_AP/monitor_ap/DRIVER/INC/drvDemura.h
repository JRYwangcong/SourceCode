

#ifndef _MS_DEMURA_H_
#define _MS_DEMURA_H_
#define ENABLE_SET_RAW_DEMURA_TABLE  0
#define MAX_LAYERS  5
#define ExtremeDataAlgorithm 1

#if (CHIP_ID==MST9U3)
#define MAX_NODES  3321 //for 4096x2560 resolution
#define MAX_NODES_Hor  81
#define MAX_NODES_Ver 41
#define MAX_COMPRESS_NODES  2091
#define MAX_COMPRESS_NODES_Hor  51 // // 81/8*5 + 1
#else
#define MAX_NODES  2665 //for 4096x2560 resolution
#define MAX_NODES_Hor  65
#define MAX_NODES_Ver 41
#define MAX_COMPRESS_NODES  1681
#define MAX_COMPRESS_NODES_Hor  41 // // 65/8*5 + 1

#endif

//==========MSTAR COMMAND====================================
#define MS_GET_DeMura_COMMAND       0xA0
#define MS_SET_DeMura_COMMAND       0xA1
#define MS_GetDemuraTableFromMem    0xA2
#define MS_SaveDemuraTable          0xA3
#define MS_SetDemuraDoubleLayerGain 0xA4
#define MS_EnableDemura             0xA5
#define MS_SetDemuraPanelSize       0xA6
#define MS_SET_DeMura_MEASURE_MODE  0xC0
#define MS_GetDemuraVersion         0xAA
#define MS_GetDemuraTableMemAddress       0xAB
//==========================================================

typedef struct
{
    BYTE RY[MAX_LAYERS];
    BYTE GY[MAX_LAYERS];
    BYTE BY[MAX_LAYERS];
} NodeOfDemura;

typedef struct
{
    BYTE G0R0;
    BYTE B0;
    BYTE RY2Y1;
    BYTE RY4Y3;
    BYTE GY2Y1;
    BYTE GY4Y3;
    BYTE BY2Y1;
    BYTE BY4Y3;
}LayerGainofDemura;

#if ExtremeDataAlgorithm

#define sizeOfDeMuraBlock (sizeof(CompressTableofDeMura)+sizeof(DemuraSetting))

typedef struct
{
WORD TableSize;
WORD HorSize;
WORD VerSize;
BYTE CompressDeMuraTab[MAX_NODES_Hor*MAX_NODES_Ver*3];  //(Hor * Ver / 3) * 4(1 unit 4 bytes) * 3(RGB)
BYTE CheckSum;
}CompressTableofDeMura;

typedef struct
{
	LayerGainofDemura LayerGains;
	BYTE CheckSumLayerGain;
}DemuraSetting;

#endif

typedef struct
{
    NodeOfDemura Nodes[MAX_NODES];
    WORD CheckSum;
    LayerGainofDemura LayerGains;
    BYTE CheckSumLayerGain;
} TableOfDemura;

void deDelta(  int * arr, int num );
int ReturnNegativePositiveInteger(int input,int bits_count,bool IsSign);
int readSS10( unsigned int* from, int* val);

WORD CalcCheckSum(void);
void drvSetDemuraNodes(int index, int len); //idx range : 0 ~ 2664,
void drvSaveDemuraNodes(int index, int len); //idx range : 0 ~ 2664,
void drvLoadDemuraNodes(int index, int len); //idx range : 0 ~ 2664,
void drvSetDemuraTable(void);
void drvGetDemuraNodes(int index, int len); //idx range : 0 ~ 2664,
WORD CalCheckSum(DWORD index, DWORD len);


extern void drvGetDemuraNodesFromMem(DWORD storedAddr);
//extern void drvGetDemuraNodesFromMem(void);
extern void drvClearDemuraTable(void);
extern void drvSaveDemuraTable(void);
extern void drvLoadDemuraTable(void);
extern void drvInitDemuraTable(void);
extern void drvSetDemuraPanelSize(int PanelSize_Z);
extern void drvEnabledDemura(BYTE enable); // 1 : enable, 0 :disable
extern void drvCompressDeMura(void);
extern void drvDeCompressDeMura(void);
extern xdata BYTE g_u8CompressDeMuraTab[3][MAX_NODES];
extern void LoadDeMuraTabAllLayer(void);
extern void drvInitialDemura(int hSizeOfPanel);
extern void drvLoadCompressDemuraTable(void);
extern void drvSaveComprssDemuraTable(void);
extern void drvSaveDoubleLayerGain(BYTE *LayerGain, BYTE chkSumDLG);
#if ENABLE_SET_RAW_DEMURA_TABLE
typedef enum
{
	SingleLayer,
	FullLayer
}RawDemuraTableFormat;
extern void drvSetRawDemuraTable(RawDemuraTableFormat tblFmt,  U8 (*pTbl)[MAX_NODES]);
extern void drvSetSingleLayerDemuraTable(void);
extern void drvSetFullLayerDemuraTable(void);
#endif

extern TableOfDemura _tableOfDemura;
extern DWORD DramDemuraTabOffset;

extern BYTE DemuraCalibrationHandler(WORD *_checkSum,WORD* _status);
extern BOOL IsDemuraCommand(BYTE CommandCode);

#endif

