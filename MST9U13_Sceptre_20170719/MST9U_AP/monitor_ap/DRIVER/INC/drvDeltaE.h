
#ifndef _MS_DELTAE_H_
#define _MS_DELTAE_H_


//==========MSTAR COMMAND====================================
#define MS_Set_PostGamma      	    0x32
#define MS_WR_PostGamma      	    0x33
#define MS_SetPostGammaFromMem      0x39
#define MS_LoadColorMode            0x42
#define MS_LoadPostGamma            0x44
#define MS_Set_3DLUT          	    0x50
#define MS_WR_3DLUT           	    0x51
#define MS_IsUse_3DLUT           	0x52
#define MS_Set_DICOM_Mode           0x60
#define MS_Set_GammaCurve           0x71
#define MS_Set_ColorTrackMode       0x80
#define MS_WR_ColorMatrix      	    0x84
#define MS_SAVE_ColorMatrix         0x86
#define MS_WR_ColorTemp             0x98
#define MS_Set_ColorTemp            0x99
#define MS_GetDeltaEVersion         0x9A
//=========================================================

#define NUM_OF_COLORMODE                5
#define NUM_OF_COLORMODE_GAMMA          3
#define NUM_OF_COLOR_TEMP               6
#define NUM_OF_COLORTRACK_GAMMA         3

typedef enum
{
    DELTAE_COLORMODE_SRGB,
    DELTAE_COLORMODE_ADOBERGB,
    DELTAE_COLORMODE_BT709,
    DELTAE_COLORMODE_BT2020,
    DELTAE_COLORMODE_DCIP,
    DELTAE_NUM_OF_COLORMODE
} EnumDeltaEColorModes;


typedef enum
{
    AutoColorFunction_ColorMode,
    AutoColorFunction_DICOMMode,
    AutoColorFunction_GammaCurve,
    AutoColorFunction_ColorTemp,
    AutoColorFunction_ColorTrack,
}AutoColorFunction;

#define GammaTableSize    320
typedef struct{
    BYTE GammaValue;
	BYTE Data[3][GammaTableSize];
	short CheckSum;
} __attribute__((packed))StoredFormatOfPostGamma;
//}StoredFormatOfPostGamma;
//StoredFormatOfPostGamma _postGamma;

#ifndef DEF_DELTAE_TABLE
#define DEF_DELTAE_TABLE    DELTAE_COLORMODE_SRGB
#endif

#if ENABLE_ColorMode_FUNCTION
    #if Enable3DLUTColorMode
        #define _3DLUTTableSize    125
        typedef struct{
            BYTE ColorMode;
        	BYTE Data[3][_3DLUTTableSize];
        	short CheckSum;
        } __attribute__((packed))StoredFormatOf3DLUT;

        #define sizeOfColorModeBlock (sizeof(StoredFormatOfPostGamma)*NUM_OF_COLORMODE_GAMMA + sizeof(StoredFormatOf3DLUT)*NUM_OF_COLORMODE)
    #else
        typedef struct{
            BYTE ColorMode;
        	short Data[9];
        	short CheckSum;
        }  __attribute__((packed))StoredFormatOfColorMatirx;
        //} StoredFormatOfColorMatirx;
        //    StoredFormatOfColorMatirx _colorMatrix;
        #define sizeOfColorModeBlock (sizeof(StoredFormatOfPostGamma)*NUM_OF_COLORMODE_GAMMA + sizeof(StoredFormatOfColorMatirx)*NUM_OF_COLORMODE)
    #endif
#else
#define sizeOfColorModeBlock 0
#endif


#if ENABLE_DICOM_FUNCTION
#define sizeOfDICOMBlock sizeof(StoredFormatOfPostGamma)
#else
#define sizeOfDICOMBlock 0
#endif


#if ENABLE_GAMMA_FUNCTION
#define sizeOfGammaBlock sizeof(StoredFormatOfPostGamma)
#else
#define sizeOfGammaBlock 0
#endif


#if ENABLE_COLORTEMP_FUNCTION
    typedef struct{
    	BYTE rgbCont[3];
    	short CheckSum;
    }  __attribute__((packed))StoredFormatOfColorTemp;
    //} StoredFormatOfColorMatirx;
    StoredFormatOfColorTemp _colorTemp;
#define sizeOfColorTempBlock (sizeof(StoredFormatOfPostGamma) + sizeof(StoredFormatOfColorTemp) * NUM_OF_COLOR_TEMP)
#else
#define sizeOfColorTempBlock 0
#endif

#if ENABLE_COLORTRACK_FUNCTION
#define sizeOfColorTrackBlock (sizeof(StoredFormatOfPostGamma) * NUM_OF_COLORTRACK_GAMMA)
#else
#define sizeOfColorTrackBlock 0
#endif

#define sizeOfDeltaEBlock (sizeOfColorModeBlock + sizeOfDICOMBlock + sizeOfGammaBlock + sizeOfColorTempBlock + sizeOfColorTrackBlock)


static const int _channelNum = 3;     //Number of channels
static const int _maxEntries = 1024;  //Number of entries in native de-gamma table
static const int _segPntNum = 3;    //Number of segements
static const int _maxDelta = 0x20;
static const int _comMaxEntries = 384;//Number of entries in compressed de-gamma talbe;


//-------------------------------------------------------------------------------
// Specify the Delta-E color mode. sRGB/AdobeRGB
//-------------------------------------------------------------------------------
extern void msLoadDeltaEColorMode(BYTE ucDeltaEColorMode);
//-------------------------------------------------------------------------------
// Apply DICOM mode
//-------------------------------------------------------------------------------
extern void msSetDICOMMode(void);
//-------------------------------------------------------------------------------
// Fix gamma + Post gamma
//-------------------------------------------------------------------------------
extern void msSetGammaCurve(BYTE u8DispWin, BYTE GammaIndex);
//-------------------------------------------------------------------------------
// Output Test Pattern For Measurement. sRGB/AdobeRGB
//-------------------------------------------------------------------------------
extern void msSetTestPattern(BOOL IsOn, BYTE R, BYTE G , BYTE B);
//-------------------------------------------------------------------------------
// Output Color Engine Test Pattern For Verify Color Function ( sRGB , AdobeRGB , BT709 , BT2020 ... )
//-------------------------------------------------------------------------------
extern void msSetColorEngineTestPattern(BYTE u8WinIdx, Bool bEnable, WORD u16Red, WORD u16Green, WORD u16Blue);
//-------------------------------------------------------------------------------
// Control Color Engine to On/Off, it includes de-gamma/fix-gamma/post-gamma & color matrix.
//-------------------------------------------------------------------------------
extern void msSetColorEngine(BOOL IsOn);
//-------------------------------------------------------------------------------
// Assign color matrix data.
//-------------------------------------------------------------------------------
extern void msSetColorMatrixData(int idx, short data);
//-------------------------------------------------------------------------------
// Save color matrix data to EEPROM.
//-------------------------------------------------------------------------------
extern void msSaveColorMatrixData(BYTE ucDeltaEColorMode, BYTE idx);
//-------------------------------------------------------------------------------
// Assign 3D LUT data.
//-------------------------------------------------------------------------------
extern void msSet3DLUTData(int ch, int idx, BYTE data);
//-------------------------------------------------------------------------------
// Save 3D LUT data to EEPROM.
//-------------------------------------------------------------------------------
extern void msSave3DLUTData(BYTE ucColorMode, BYTE idx);
//-------------------------------------------------------------------------------
// Assign post gamma data.
//-------------------------------------------------------------------------------
extern void msSetPostGammaData(int ch, int idx, BYTE data);
//-------------------------------------------------------------------------------
// Save post gamma data to EEPROM.
//-------------------------------------------------------------------------------
extern void  msSavePostGammaData(WORD addr, BYTE GammaValue);
extern void  msSavePostGammaTable(BYTE mode, BYTE idx, BYTE GammaValue);
//-------------------------------------------------------------------------------
// Assign color temp data.
//-------------------------------------------------------------------------------
extern void msSetColorTempData(int idx, WORD u16Red, WORD u16Green, WORD u16Blue);
//-------------------------------------------------------------------------------
// Save color temp data to EEPROM.
//-------------------------------------------------------------------------------
extern void msSaveColorTempData(int idx);

//-------------------------------------------------------------------------------
// Load color temp data from EEPROM.
//-------------------------------------------------------------------------------
extern void msLoadColorTempMode(BYTE idx);

//-------------------------------------------------------------------------------
// Set color track mode
//-------------------------------------------------------------------------------
extern void msSetColorTrackMode(BYTE u8DispWin, BYTE GammaIndex);
//-------------------------------------------------------------------------------
// degamma Algorithm
//-------------------------------------------------------------------------------

extern void msCompressGamma(float nativeGma[_channelNum][_maxEntries],
							 unsigned char compressedGma[_channelNum][_comMaxEntries],
							 unsigned char segOft[_segPntNum],
							 unsigned int segPnt[_channelNum][_segPntNum]);
extern void msCompressGammaWithSegValue(float nativeGma[_channelNum][_maxEntries],
							unsigned char compressedGma[_channelNum][_comMaxEntries],
							unsigned char segOftVal[_channelNum][_segPntNum + 1],
							unsigned int segPnt[_channelNum][_segPntNum]);
extern void msInitSegSettings(float nativeGma[_channelNum][_maxEntries],
								unsigned char segOft[_segPntNum],
								unsigned char segOftVal[_channelNum][_segPntNum + 1],
								unsigned int segPnt[_channelNum][_segPntNum]);

extern int msGetSegOffset(unsigned int segPnt[_channelNum][_segPntNum],
							unsigned char segOftVal[_channelNum][_segPntNum + 1],
							int ch,
							int entry);

extern void msFindMaxDelta(float nativeGma[_channelNum][_maxEntries], int maxDelta[_channelNum], unsigned char point[_channelNum]);

extern void msResetWholeColorCalibrationData(void);

extern BYTE AutoColorCalibrationHandler(WORD *_checkSum);

extern BOOL IsAutoColorCommand(BYTE CommandCode);

#endif

