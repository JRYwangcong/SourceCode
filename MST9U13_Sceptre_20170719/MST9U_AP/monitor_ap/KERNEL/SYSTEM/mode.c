#define _MODE_C_
#include <math.h>
#include "datatype.h"
#include "Board.h"
#include "Global.h"
#include "Mode.h"

#include "ms_reg.h"
#include "Ms_rwreg.h"
#include "Detect.h"
#include "misc.h"
#include "drvADC.h"
#include "AutoFunc.h"
#include "Common.h"
#include "UserPrefAPI.h"
#include "UserPref.h"
//#include "halRwreg.h"
 #if ENABLE_DP_INPUT
#include "drvDPRxApp.h"
#endif

#define MODE_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG&&MODE_DEBUG
    #define MODE_printData(str, value)   printData(str, value)
    #define MODE_printMsg(str)           printMsg(str)
#else
    #define MODE_printData(str, value)
    #define MODE_printMsg(str)
#endif

extern BYTE mStar_GetInputStatus( BYTE InputPort );

#define SIMILAR_TIMING_BUFFER_MAX   10
BYTE xdata ucSimilarTimingNums;
BYTE xdata ucSimilarTimingBuffer[SIMILAR_TIMING_BUFFER_MAX]; // record similar timing buffer


//=====================================================================================
InputResolutionType code StandardModeResolution[] =
{
    { 640,    350},
    { 640,    400}, // RES_640x400,
    { 720,    400}, // RES_720x400,
    { 640,    480}, // RES_640x480,
    { 640,    500}, // RES_640x500,
    { 800,    600}, // RES_800x600,
    { 832,    624}, // RES_832x624,
    { 720,    480}, // RES_720x480I,
    { 720,    576}, // RES_720x576I,
    { 720,    480}, // RES_720x480P,
    { 640,    480}, // RES_640x480P,
    { 720,    576}, // RES_720x576P
    {1280,    720}, // RES_1280x720P_50
    {1280,    720}, // RES_1280x720P_60
    {1920,   1080}, // RES_1920x1080_24,
    {1920,   1080}, // RES_1920x1080_25,
    {1920,   1080}, // RES_1920x1080_30,
    {1920,   1080}, // Res_1920x1080_50
    {1920,   1080}, // Res_1920x1080_60
    {1024,    768}, // RES_1024x768,
    {1280,   1024}, // RES_1280x1024,
    {1600,   1200}, // RES_1600x1200,
    {1152,    864}, // RES_1152x864,
    {1152,    870}, // RES_1152x870,
    {1152,    900}, // RES_1152x900,
    {1176,    664}, // RES_1176x664,
    {1280,    960}, // RES_1280x960,
    {1280,    720}, // RES_1280x720,
    {1280,    768}, // RES_1280x768,
    {1280,    800}, // RES_1280x800,
    {1360,    768}, // RES_1360x768,
    {1400,   1050}, // RES_1440x1050,
    {1440,    900}, // RES_1440x900,
    {1680,   1050}, // RES_1680x1050,
    { 720,    576}, // RES_720x576,
    { 848,    480}, // RES_848x480,
    {1600,   1000}, // RES_1600x1000,
    {1920,   1200}, // RES_1920x1200,
    {1366,    768}, // RES_1366x768,
    {1600,    900}, // RES_1600x900,
    {1600,   1280}, // RES_1600x1280,
    {1792,   1344}, // RES_1792x1344,
    {1920,   1440}, // RES_1920x1440,
    {1856,   1392}, // RES_1856x1392,
    {2048,   1152},
    {2048,   1280}, // RES_2048x1280,
    {2560,   1440}, // RES_2560x1440,
    {1800,   1440}, // RES_1800x1440
    {2048,   1536}, // RES_2048x1536
    {2560,   1080},//RES_2560x1080
    
    {2560,   1600},//RES_2560X1600
    {1920,   2160},//RES_1920x2160
    {3840,   2160},//RES_3840x2160
    {4096,   2160},//RES_4096x2160
    
#if ENABLE_3D_FUNCTION
    {1280,   1440}, // RES_1280x720P_FP
    {1920,   1080}, // RES_1920x1080_3D
    #endif
};

InputModeType code StandardMode[] =
{
  // 0: 640x350 70Hz (IMB VGA)
  { SYNC_HP_VN| SYNC_HN_VN,                             // Flags
    RES_640x350,                                        // ResIndex
    315, 700,                                           // HFreq, VFreq
    800, 449,                                           // HTotal, VTotal
    139, 58,                                            // HStart, VStart
  },
  // 1: 640x350 85Hz (VESA)
  { SYNC_HP_VN,                                         // Flags
    RES_640x350,                                        // ResIndex
    379, 851,                                           // HFreq, VFreq
    832, 445,                                           // HTotal, VTotal
    160, 63,                                            // HStart, VStart
  },
  //2: 640x400 70Hz (IMB VGA)
  { SYNC_HP_VP | SYNC_HN_VP | SYNC_HN_VN,               // Flags
    RES_640x400,                                        // ResIndex
    315, 700,                                           // HFreq, VFreq
    800, 449,                                           // HTotal, VTotal
    144, 37,                                            // HStart, VStart
  },
  // 2: 720x400 70Hz (VGA)
  { SYNC_HP_VP | SYNC_HN_VP ,                           // Flags
    RES_720x400,                                        // ResIndex
    315, 700,                                           // HFreq, VFreq
    900, 449,                                           // HTotal, VTotal
    55, 34,                                             // HStart, VStart 640x400@70=(55, 34),720x400@70=(55, 33)
  },
  // 3: 640x480 60Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x480P,//RES_640x480,                         // ResIndex
    315, 599,                                           // HFreq, VFreq
    800, 525,                                           // HTotal, VTotal
    49, 32,                                             // HStart, VStart
  },
  // 4: 640x480 66Hz (MAC)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x480,                                        // ResIndex
    350, 667,                                           // HFreq, VFreq
    864, 525,                                           // HTotal, VTotal
    97, 38,                                             // HStart, VStart
  },
  // 5: 640x480 72Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x480,                                        // ResIndex
    379, 728,                                           // HFreq, VFreq
    832, 520,                                           // HTotal, VTotal
    129, 27,                                            // HStart, VStart
  },
  // 6: 640x480 75Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x480,                                        // ResIndex
    375, 750,                                           // HFreq, VFreq
    840, 500,                                           // HTotal, VTotal
    121, 15,                                            // HStart, VStart
  },
  // 7: 800x600 56Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_800x600,                                        // ResIndex
    351, 562,                                           // HFreq, VFreq
    1024, 625,                                          // HTotal, VTotal
    127, 21,                                            // HStart, VStart
  },
  // 8: 800x600 60Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_800x600,                                        // ResIndex
    378, 603,                                           // HFreq, VFreq
    1056, 628,                                          // HTotal, VTotal
    87, 22,                                             // HStart, VStart
  },
  // 9: 800x600 72Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_800x600,                                        // ResIndex
    480, 721,                                           // HFreq, VFreq
    1040, 666,                                          // HTotal, VTotal
    63, 22,                                             // HStart, VStart
  },
  // 10: 800x600 75Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_800x600,                                        // ResIndex
    469, 750,                                           // HFreq, VFreq
    1056, 625,                                          // HTotal, VTotal
    159, 20,                                            // HStart, VStart
  },
  // 11: 832x624 75Hz (MAC)(H-V-)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_832x624,                                        // ResIndex
    497, 746,                                           // HFreq, VFreq
    1152, 667,                                          // HTotal, VTotal
    282, 38,                                            // HStart, VStart
  },
  // 12: 1024x768 60Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1024x768,                                       // enResIndex
    483, 600,                                           // HFreq, VFreq
    1344, 806,                                          // HTotal, VTotal
    161, 28,                                            // HStart, VStart
  },
  // 13: 1024x768 60Hz (MAC)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1024x768,                                       // enResIndex
    487, 600,                                           // HFreq, VFreq
    1312, 813,                                          // HTotal, VTotal
    296, 26,                                            // HStart, VStart
  },
  // 14: 1024x768 70Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1024x768,                                       // ResIndex
    564, 700,                                           // HFreq, VFreq
    1328, 806,                                          // HTotal, VTotal
    145, 28,                                            // HStart, VStart
  },
  // 15: 1024x768 72Hz (IBM)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1024x768,                                       // ResIndex
    576, 720,                                           // HFreq, VFreq
    1360, 801,                                          // HTotal, VTotal
    166, 28,                                            // HStart, VStart
  },
  // 16: 1024x768 75Hz (VESA)    //(GTF-vtotal=802, CVT-vtotal=805 H- V+)
  { SYNC_HN_VP | SYNC_HP_VN | SYNC_HN_VN ,              // Flags
    RES_1024x768,                                       // ResIndex
    601, 749,                                           // HFreq, VFreq
    1360, 802,                                          // HTotal, VTotal
    274, 29,                                            // HStart, VStart
  },
  // 17: 1024x768 75Hz (VESA)
  { SYNC_HP_VP ,                                        // Flags
    RES_1024x768,                                       // ResIndex
    600, 750,                                           // HFreq, VFreq
    1312, 800,                                          // HTotal, VTotal
    174, 27,                                            // HStart, VStart
  },
  // 18: 1152x864 60Hz (VESA GTF)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x864,                                       // ResIndex
    537, 600,                                           // HFreq, VFreq
    1520, 895,                                          // HTotal, VTotal
    296, 26,                                            // HStart, VStart
  },
  // 19: 1152x864 70Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x864,                                       // ResIndex
    630, 700,                                           // HFreq, VFreq
    1536, 900,                                          // HTotal, VTotal
    189, 31,                                            // HStart, VStart
  },
  // 20: QD_DMT1170
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x864,                                       // ResIndex
    639, 700,                                           // HFreq, VFreq
    1480, 912,                                          // HTotal, VTotal
    289, 43,                                            // HStart, VStart
  },
  // 21: 1152x864 75Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x864,                                       // ResIndex
    675, 750,                                           // HFreq, VFreq
    1600, 900,                                          // HTotal, VTotal
    252, 31,                                            // HStart, VStart
  },
  // 22: 1152x864 75Hz CVT
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x864,                                       // enResIndex
    677, 748,                                           // HFreq, VFreq
    1536, 905,                                          // HTotal, VTotal
    305, 33,                                            // HStart, VStart
  },
  // 23: 1152x870 75Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x870,                                       // ResIndex
    681, 750,                                           // HFreq, VFreq
    1552, 908,                                          // HTotal, VTotal
    201,  33,                                           // HStart, VStart
  },
  // 24: 1152x870 75Hz (MAC)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x870,                                       // ResIndex
    687, 751,                                           // HFreq, VFreq
    1456, 915,                                          // HTotal, VTotal
    262, 38,                                            // HStart, VStart
  },
  // 25: 1152 X 900 66 Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x900,                                       // ResIndex
    617, 660,                                           // HFreq, VFreq
    1536, 935,                                          // HTotal, VTotal
    193,  30,                                           // HStart, VStart
  },
  // 26: 1152 X 900 76 Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1152x900,                                       // ResIndex
    715, 760,                                           // HFreq, VFreq
    1552, 940,                                          // HTotal, VTotal
    201, 35,                                            // HStart, VStart
  },
  // 27: 1280x960 60Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x960,                                       // ResIndex
    600, 600,                                           // HFreq, VFreq
    1800, 1000,                                         // HTotal, VTotal
    417, 35,                                            // HStart, VStart
  },
  // 28: 1280x1024 60Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x1024,                                      // ResIndex
    640, 600,                                           // HFreq, VFreq
    1688, 1066,                                         // HTotal, VTotal
    244, 37,                                            // HStart, VStart
  },
  // 29: 1280x1024 70Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x1024,                                      // ResIndex
    749, 698,                                           // HFreq, VFreq
    1696, 1072,                                         // HTotal, VTotal
    376, 41,                                            // HStart, VStart
  },
  // 30: 1280x1024 70Hz (VESA) GTF
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x1024,                                      // ResIndex
    746, 700,                                           // HFreq, VFreq
    1728, 1066,                                         // HTotal, VTotal
    219, 37,                                            // HStart, VStart
  },
  // 31: 1280x1024 72Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x1024,                                      // ResIndex
    779, 720,                                           // HFreq, VFreq
    1728, 1082,                                         // HTotal, VTotal
    352, 51,                                            // HStart, VStart
  },
  // 32: 1280x1024 75Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x1024,                                      // ResIndex
    800, 750,                                           // HFreq, VFreq
    1688, 1066,                                         // HTotal, VTotal
    243, 37,                                            // HStart, VStart
  },
  // 33: 1280x1024 76Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x1024,                                      // ResIndex
    812, 762,                                           // HFreq, VFreq
    1664, 1066,                                         // HTotal, VTotal
    288, 31,                                            // HStart, VStart
  },
  // 34: 1600x1200 60Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1200,                                      // ResIndex
    750, 600,                                           // HFreq, VFreq
    2160, 1250,                                         // HTotal, VTotal
    298, 45,                                            // HStart, VStart
  },
  // 35: 1600x1200 60Hz_R (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1200,                                      // ResIndex
    740, 599,                                           // HFreq, VFreq
    1760, 1235,                                         // HTotal, VTotal
    75, 27,                                             // HStart, VStart
  },
  // 36: 1280x720 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720,                                       // ResIndex
    447, 599,                                           // HFreq, VFreq
    1664, 748,                                          // HTotal, VTotal
    193, 19,                                            // HStart, VStart
  },
  // 37: 1280x720 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720,                                       // ResIndex
    447, 599,                                           // HFreq, VFreq
    1664, 746,                                          // HTotal, VTotal
    193, 21,                                            // HStart, VStart
  },
  // 38: 1280x720 70Hz (GTF)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720,                                       // ResIndex
    525, 700,                                           // HFreq, VFreq
    1696, 750,                                          // HTotal, VTotal
    337, 25,                                            // HStart, VStart
  },
  // 39: 1280x720 72Hz (GTF)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720,                                       // ResIndex
    541, 720,                                           // HFreq, VFreq
    1696, 751,                                          // HTotal, VTotal
    337, 26,                                            // HStart, VStart
  },
  // 40: 1280x720 75Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720,                                       // ResIndex
    565, 748,                                           // HFreq, VFreq
    1696, 755,                                          // HTotal, VTotal
    209, 27,                                            // HStart, VStart
  },
  // 41: 1280x768_R 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x768,                                       // ResIndex
    474, 600,                                           // HFreq, VFreq
    1440, 790,                                          // HTotal, VTotal
    78, 11,                                             // HStart, VStart
  },
  // 42: 1280x768 60Hz
  { SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,               // Flags
    RES_1280x768,                                       // ResIndex
    478, 599,                                           // HFreq, VFreq
    1664, 798,                                          // HTotal, VTotal
    193, 19,                                            // HStart, VStart
  },
  // 43: 1280x768 60Hz GTF, vsync_width=3
  { SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,               // Flags
    RES_1280x768,                                       // ResIndex
    477, 600,                                           // HFreq, VFreq
    1680, 795,                                          // HTotal, VTotal
    328, 22,                                            // HStart, VStart
  },
  // 44: 1280x768 75Hz
  { SYNC_HN_VP ,                                        // Flags
    RES_1280x768,                                       // ResIndex
    603, 749,                                           // HFreq, VFreq
    1696, 805,                                          // HTotal, VTotal
    209, 26,                                            // HStart, VStart
  },
  // 45: 1280x800 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x800,                                       // ResIndex
    498, 601,                                           // HFreq, VFreq
    1680, 828,                                          // HTotal, VTotal
    326, 23,                                            // HStart, VStart
  },
  // 46: 1360x768 60Hz
  { SYNC_HP_VP,                                         // Flags
    RES_1360x768,                                       // ResIndex
    477, 600,                                           // HFreq, VFreq
    1792, 795,                                          // HTotal, VTotal
    253, 17,                                            // HStart, VStart
  },
  // 47: 1400x1050_R 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1400x1050,                                      // ResIndex
    647, 599,                                           // HFreq, VFreq
    1560, 1080,                                         // HTotal, VTotal
    76, 22,                                             // HStart, VStart
  },
  // 48: 1400x1050 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1400x1050,                                      // ResIndex
    653, 600,                                           // HFreq, VFreq
    1864, 1089,                                         // HTotal, VTotal
    232, 31,                                            // HStart, VStart
  },
  // 49: 1400x1050 75Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1400x1050,                                      // ResIndex
    823, 749,                                           // HFreq, VFreq
    1896, 1099,                                         // HTotal, VTotal
    378, 41,                                            // HStart, VStart
  },
  // 50: 1440 x 900 60Hz _R
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1440x900,                                       // ResIndex
    555, 599,                                           // HFreq, VFreq
    1600, 926,                                          // HTotal, VTotal
    77, 16,                                             // HStart, VStart
  },
  // 51: 1440x900 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1440x900,                                       // ResIndex
    559, 599,                                           // HFreq, VFreq
    1904, 934,                                          // HTotal, VTotal
    233, 24,                                            // HStart, VStart
  },
  // 52: 1680x1050 _R60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1680x1050,                                      // ResIndex
    647, 599,                                           // HFreq, VFreq
    1840, 1080,                                         // HTotal, VTotal
    0x4B, 0x14,                                         // HStart, VStart
  },
  // 53: 1680x1050 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1680x1050,                                      // ResIndex
    653,600,                                            // HFreq, VFreq
    2240, 1089,                                         // HTotal, VTotal
    281, 29,                                            // HStart, VStart
  },
  // 54: 720*576 _R60
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_720x576,                                        // ResIndex
    358, 597,                                           // HFreq, VFreq
    912, 599,                                           // HTotal, VTotal
    97, 12,                                             // HStart, VStart
  },
  // 55: 848x480 60Hz (GTF)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_848x480,                                        // ResIndex
    296, 597,                                           // HFreq, VFreq
    1056, 497,                                          // HTotal, VTotal
    105, 12,                                            // HStart, VStart
  },
  // 56: 848x480 70Hz
  { SYNC_HN_VP |SYNC_HP_VP ,                            // Flags
    RES_848x480,                                        // ResIndex
    350, 701,                                           // HFreq, VFreq
    1072, 500,                                          // HTotal, VTotal
    111, 15,                                            // HStart, VStart
  },
  // 57: 848x480 71.9Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_848x480,                                        // ResIndex
    361, 720,                                           // HFreq, VFreq
    1088, 501,                                          // HTotal, VTotal
    119, 16,                                            // HStart, VStart
  },
  // 58: 848x480 75Hz
  { SYNC_HN_VP,                                         // Flags
    RES_848x480,                                        // ResIndex
    377, 748,                                           // HFreq, VFreq
    1088, 504,                                          // HTotal, VTotal
    121, 15,                                            // HStart, VStart
  },
  // 59: 848x480 85Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_848x480,                                        // ResIndex
    430, 850,                                           // HFreq, VFreq
    1105, 506,                                          // HTotal, VTotal
    107, 24,                                            // HStart, VStart
  },
  // 60: 848x480 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_848x480,                                        // ResIndex
    310, 600,                                           // HFreq, VFreq
    1088, 517,                                          // HTotal, VTotal
    112, 22,                                            // HStart, VStart
  },
  // 61: 1280x800 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x800,                                       // ResIndex
    497, 598,                                           // HFreq, VFreq
    1680, 831,                                          // HTotal, VTotal
    201, 21,                                            // HStart, VStart
  },
  // 62: 800x600 67Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_800x600,                                        // ResIndex
    488, 669,                                           // HFreq, VFreq
    1024, 730,                                          // HTotal, VTotal
    176, 82,                                            // HStart, VStart
  },
  // 63: 720x480i 60Hz (480i)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN | bInterlaceMode, // Flags
    RES_720x480I,                                       // ResIndex
    157, 600,                                           // HFreq, VFreq
    858, 525/*/2*/,                                     // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    58, 24,                                             // HStart, VStart
#else
    131, 37,                                            // HStart, VStart
#endif
  },
  // 64: 720x576i 50Hz (576i)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN | bInterlaceMode, // Flags
    RES_720x576I,                                       // ResIndex
    156, 500,                                           // HFreq, VFreq
    864, 625/*/2*/,                                     // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    70, 33,                                             // HStart, VStart
#else
    144, 47,                                            // HStart, VStart
#endif
  },
  // 65: 720x480P 60Hz (YPbPr)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_720x480P,                                       // ResIndex
    315, 600,                                           // HFreq, VFreq
    858, 525,                                           // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    60, 27,                                             // HStart, VStart
#else
    132, 39,                                            // HStart, VStart
#endif
  },
  // 66: 640x480 60Hz (YPbPr)   //640x480P vsyncwidth=6
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x480P,                                       // ResIndex
    315, 599,                                           // HFreq, VFreq
    780, 525,                                           // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    65, 28,                                             // HStart, VStart
#else
    152, 41,                                            // HStart, VStart
#endif
  },
  // 67: 720x576P 50Hz (YPbPr)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_720x576P,                                       // ResIndex
    312, 500,                                           // HFreq, VFreq
    864, 625,                                           // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    69, 36,                                             // HStart, VStart
#else
    142, 50,                                            // HStart, VStart
#endif
  },
  // 68: 1280x720P 50Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720P_50,                                   // ResIndex
    375, 500,                                           // HFreq, VFreq
    1980, 750,                                          // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    260, 17,                                            // HStart, VStart
#else
    285, 35,                                            // HStart, VStart
#endif
  },
  // 69: 1280x720P 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720P_60,                                   // ResIndex
    450, 600,                                           // HFreq, VFreq
    1650, 750,                                          // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    260, 17,                                            // HStart, VStart
#else
    285, 35,                                            // HStart, VStart
#endif
  },
  // 70: 1920x1080i 50Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN | bInterlaceMode, // Flags
    RES_1920x1080_50,                                   // ResIndex
    281, 500,                                           // HFreq, VFreq
    2640, 1125,                                         // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    192, 24,                                            // HStart, VStart
#else
    233, 52,                                            // HStart, VStart
#endif
  },
  // 71: 1920x1080i 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN | bInterlaceMode, // Flags
    RES_1920x1080_60,                                   // ResIndex
    337, 600,                                           // HFreq, VFreq
    2200, 1124,                                         // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    192, 24,                                            // HStart, VStart
#else
    233, 52,                                            // HStart, VStart
#endif
  },
  // 72: 1920x1080P 50Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_50,                                   // ResIndex
    563, 500,                                           // HFreq, VFreq
    2640, 1125,                                         // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    192, 33,                                            // HStart, VStart
#else
    229, 60,                                            // HStart, VStart
#endif
  },
  // 73: 1920x1080P 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_60,                                   // ResIndex
    675, 600,                                           // HFreq, VFreq
    2200, 1125,                                         // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    192, 33,                                            // HStart, VStart
#else
    229, 60,                                            // HStart, VStart
#endif
  },
  // 74: 800x600 70Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_800x600,                                        // ResIndex
    437, 700,                                           // HFreq, VFreq
    1040, 625,                                          // HTotal, VTotal
    193, 20,                                            // HStart, VStart
  },
  // 75: 1280x1024 72Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x1024,                                      // ResIndex
    768, 720,                                           // HFreq, VFreq
    1728, 1067,                                         // HTotal, VTotal
    219, 38,                                            // HStart, VStart
  },
  // 76: 1600x1000R 60Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1000,                                      // ResIndex
    616, 599,                                           // HFreq, VFreq
    1760, 1029,                                         // HTotal, VTotal
    76, 19,                                             // HStart, VStart
  },
  // 77: 640x500 58Hz (QD-THB0658)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x500,                                        // ResIndex
    312, 577,                                           // HFreq, VFreq
    808, 541,                                           // HTotal, VTotal
    81, 10,                                             // HStart, VStart
  },
  // 78: 1176x664 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1176x664,                                       // ResIndex
    417, 604,                                           // HFreq, VFreq
    1512, 690,                                          // HTotal, VTotal
    288, 23,                                            // HStart, VStart
  },
  // 79: 1920x1200R 50Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1200,                                      // ResIndex
    614, 500,                                           // HFreq, VFreq
    2080, 1229,                                         // HTotal, VTotal
    75, 19,                                             // HStart, VStart
  },
  // 80: 1920x1200R 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1200,                                      // ResIndex
    740, 600,                                           // HFreq, VFreq
    2080, 1235,                                         // HTotal, VTotal
    74, 25,                                             // HStart, VStart
  },
  // 81: 1440x900 75Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1440x900,                                       // ResIndex
    706, 750,                                           // HFreq, VFreq
    1936, 942,                                          // HTotal, VTotal
    248, 32,                                            // HStart, VStart
  },
  // 82: 1280x800R 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x800,                                       // ResIndex
    493, 599,                                           // HFreq, VFreq
    1440, 823,                                          // HTotal, VTotal
    78, 13,                                             // HStart, VStart
  },
  // 83: 1280x800 75Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x800,                                       // ResIndex
    628, 749,                                           // HFreq, VFreq
    1696, 838,                                          // HTotal, VTotal
    209, 28,                                            // HStart, VStart
  },
  // 84:  1600x1000R 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1000,                                      // ResIndex
    621, 599,                                           // HFreq, VFreq
    2128, 1038,                                         // HTotal, VTotal
    264, 28,                                            // HStart, VStart
  },
  // 85: 1280x800 70Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x800,                                       // ResIndex
    583, 700,                                           // HFreq, VFreq
    1696, 833,                                          // HTotal, VTotal
    209, 28,                                            // HStart, VStart
  },
  // 86: 1280x800 72Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x800,                                       // ResIndex
    600, 720,                                           // HFreq, VFreq
    1712, 834,                                          // HTotal, VTotal
    217, 29,                                            // HStart, VStart
  },
  // 87: 1366x768 59.8Hz
  { SYNC_HP_VP ,                                        // Flags
    RES_1366x768,                                       // ResIndex
    477, 598,                                           // HFreq, VFreq
    1792, 798,                                          // HTotal, VTotal
    210, 23,                                            // HStart, VStart
  },
  // 88: 1680x1050 75Hz Reduce   QD-CVR1675D
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1680x1050,                                      // ResIndex
    815, 749,                                           // HFreq, VFreq
    1840, 1088,                                         // HTotal, VTotal
    103, 28,                                            // HStart, VStart
  },
  // 89: 1920X1080 60Hz Reduce   CVR1960H
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_60,                                   // ResIndex
    666, 600,                                           // HFreq, VFreq
    2080,1111,                                          // HTotal, VTotal
    104, 22,                                            // HStart, VStart
  },
  // 90: 1920x1080 60Hz          DMT1960H
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_60,                                   // ResIndex
    675, 600,                                           // HFreq, VFreq
    2200, 1125,                                         // HTotal, VTotal
    142, 35,                                            // HStart, VStart
  },
  // 91: 1920X1080 50Hz          CVT1950H
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_50,                                   // ResIndex
    556, 500,                                           // HFreq, VFreq
    2544, 1114,                                         // HTotal, VTotal
    312, 25,                                            // HStart, VStart
  },
  // 92: 1600x900 60R Hz (CVT)    CVR1659H
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x900,                                       // ResIndex
    555, 599,                                           // HFreq, VFreq
    1760, 926,                                          // HTotal, VTotal
    77, 17,                                             // HStart, VStart
  },
  // 93: 1600x900 50Hz  (CVT)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x900,                                       // ResIndex
    464, 499,                                           // HFreq, VFreq
    2080, 929,                                          // HTotal, VTotal
    388, 20,                                            // HStart, VStart
  },
  // 94: 1600x900 60Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x900,                                       // ResIndex
    600, 600,                                           // HFreq, VFreq
    1800, 1000,                                         // HTotal, VTotal
    169, 95,                                            // HStart, VStart
  },

  //165 < output dclk < 205//(START)

  // 95: 1600x1000 75Hz (GTF)   DLCK=169.128MHz     CVT1660B
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1000,                                      // ResIndex
    783, 750,                                           // HFreq, VFreq
    2160, 1044,                                         // HTotal, VTotal
    280, 37,                                            // HStart, VStart
  },
  // 96: 1600x1200 65Hz         DLCK=175.5MHz       DMT1665
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1200,                                      // ResIndex
    813, 650,                                           // HFreq, VFreq
    2160, 1250,                                         // HTotal, VTotal
    297, 45,                                            // HStart, VStart
  },
  // 97, 1600x1200 70Hz         DLCK=189MHz         DMT1670
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1200,                                      // ResIndex
    875, 700,                                           // HFreq, VFreq
    2160, 1250,                                         // HTotal, VTotal
    297, 45,                                            // HStart, VStart
  },
  // 98: 1600x1200 75Hz         DLCK=202.5MHz       DMT1675
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1200,                                      // ResIndex
    938, 750,                                           // HFreq, VFreq
    2160, 1250,                                         // HTotal, VTotal
    296, 45,                                            // HStart, VStart
  },
  // 99: 1600x1200 75Hz         DLCK=204.75MHz      CVT1675
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1200,                                      // ResIndex
    941, 750,                                           // HFreq, VFreq
    2176, 1255,                                         // HTotal, VTotal
    445, 47,                                            // HStart, VStart
  },
  // 100: 1600x1280 60Hz        DLCK=171.75MHz      CVT1660A
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x1280,                                      // ResIndex
    795, 599,                                           // HFreq, VFreq
    2160, 1327,                                         // HTotal, VTotal
    256, 35,                                            // HStart, VStart
  },
  // 101: 1680x1050 75Hz        DCLK=187MHz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1680x1050,                                      // ResIndex
    823, 750,                                           // HFreq, VFreq
    2272, 1099,                                         // HTotal, VTotal
    297, 39,                                            // HStart, VStart
  },
  // 102: 1792x1344 60Hz        DLCK=204.75MHz      DMT1760
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1792x1344,                                      // ResIndex
    836, 600,                                           // HFreq, VFreq
    2448, 1394,                                         // HTotal, VTotal
    328, 45,                                            // HStart, VStart
  },
  // 103: 1920X1080 60Hz        DCLK=173MHz         QD-CVT1960H
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_60,                                   // ResIndex
    672, 600,                                           // HFreq, VFreq
    2576, 1120,                                         // HTotal, VTotal
    520, 31,                                            // HStart, VStart
  },
  // 104: 1920x1080 75Hz (CVT)  DCLK=220.75MHz      CVT1975H
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_60,                                   // ResIndex
    846, 749,                                           // HFreq, VFreq
    2608,1130,                                          // HTotal, VTotal
    344, 42,                                            // HStart, VStart
  },
  // 105: 1920x1200 60Hz        DCLK=193.25MHz      CVT1960D
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1200,                                      // ResIndex
    746, 600,                                           // HFreq, VFreq
    2592, 1245,                                         // HTotal, VTotal
    336, 35,                                            // HStart, VStart
  },
  // 106: 1920x1200 75Hz        DCLK=245.25MHz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1200,                                      // ResIndex
    940, 749,                                           // HFreq, VFreq
    2608, 1255,                                         // HTotal, VTotal
    527, 35,                                            // HStart, VStart
  },
  // 107: 1920x1440 60Hz reduce DCLK=184.75MHz      QD-CVR1960
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1440,                                      // ResIndex
    888, 600,                                           // HFreq, VFreq
    2080, 1481,                                         // HTotal, VTotal
    73, 33,                                             // HStart, VStart
  },
  // 108: 1920x1440 60Hz        DCLK=233.5MHz       QD-CVT1960
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1440,                                      // ResIndex
    895, 600,                                           // HFreq, VFreq
    2608, 1493,                                         // HTotal, VTotal
    527, 35,                                            // HStart, VStart
  },
  // 109: 1856x1392 60Hz        DCLK=217.25MHz      QD-CVT1860
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1856x1392,                                      // ResIndex
    865, 599,                                           // HFreq, VFreq
    2512, 1443,                                         // HTotal, VTotal
    527, 35,                                            // HStart, VStart
  },
  // 110: 1856x1392 60Hz        DCLK=218.25MHz      QD-DMT1860A
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1856x1392,                                      // ResIndex
    863, 599,                                           // HFreq, VFreq
    2528, 1439,                                         // HTotal, VTotal
    527, 35,                                            // HStart, VStart
  },

  //165 < output dclk < 205//(END)

  // 111: 1600x900 60Hz       CVT
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1600x900,                                       // ResIndex
    560, 599,                                           // HFreq, VFreq
    2112, 934,                                          // HTotal, VTotal
    410, 25,                                            // HStart, VStart
  },
  // 112: 832x624 75Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_832x624,                                        // ResIndex
    489, 748,                                           // HFreq, VFreq
    1088, 654,                                          // HTotal, VTotal
    129, 22,                                            // HStart, VStart
  },
  // 113: 640x480 73Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x480,                                        // ResIndex
    366, 731,                                           // HFreq, VFreq
    816, 501,                                           // HTotal, VTotal
    89, 16,                                             // HStart, VStart
  },
  // 114: 1920x1080i 50Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN | bInterlaceMode,   // Flags
    RES_1920x1080_50,                                   // ResIndex
    312, 500,                                           // HFreq, VFreq
    2304,1250,                                          // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    346, 154,                                           // HStart, VStart
#else
    192, 160,                                           // HStart, VStart
#endif
  },
  // 115: 640x500 58Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x500,                                        // ResIndex
    310, 578,                                           // HFreq, VFreq
    816, 537,                                           // HTotal, VTotal
    112, 16,                                            // HStart, VStart
  },
  // 116: 640x350_50Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x350,                                        // ResIndex
    315, 500,                                           // HFreq, VFreq
    800, 629,                                           // HTotal, VTotal
    128, 38,                                            // HStart, VStart
  },
  // 117: 2048x1152 60RHz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_2048x1152,                                      // ResIndex
    709, 599,                                           // HFreq, VFreq
    2208, 1185,                                         // HTotal, VTotal
    74, 24,                                             // HStart, VStart
  },
  // 118: 2048x1152R 60Hz      DCLK=162MHz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
     RES_2048x1152,                                     // ResIndex
     720, 600,                                          // HFreq, VFreq
     2250,1200,                                         // HTotal, VTotal
     167, 43,                                           // HStart, VStart
  },
  // 119: 2048x1152 60Hz       DCLK=197MHz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_2048x1152,                                      // ResIndex
    716, 600,                                           // HFreq, VFreq
    2752,1195,                                          // HTotal, VTotal
    352, 34,                                            // HStart, VStart
  },
  // 120: 2048x1280 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_2048x1280,                                      // ResIndex
    789, 599,                                           // HFreq, VFreq
    2208,1317,                                          // HTotal, VTotal
    73, 27,                                             // HStart, VStart
  },
   // 122: 2560x1080 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_2560X1080,                                      // ResIndex
    666, 599,                                           // HFreq, VFreq
    2720, 1111,                                         // HTotal, VTotal
    80,  18,                                           // HStart, VStart
  },
 // 121: 2560x1440 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_2560x1440,                                      // ResIndex
    887, 599,                                           // HFreq, VFreq
    2720, 1481,                                         // HTotal, VTotal
    752,  55,                                           // HStart, VStart
  },

  // 131: 2560x1600 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_2560X1600,                                      // ResIndex
    987, 600,                                           // HFreq, VFreq
    2720, 1646,                                         // HTotal, VTotal
    80, 20,                                            // HStart, VStart
  },

  // 122: 640x400 85Hz (VESA)
  { SYNC_HP_VP | SYNC_HN_VP | SYNC_HN_VN,               // Flags
    RES_640x400,                                        // ResIndex
    379, 851,                                           // HFreq, VFreq
    832, 445,                                           // HTotal, VTotal
    96, 40,                                             // HStart, VStart
  },
  // 123: 640x480 85Hz (VESA)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_640x480,                                        // ResIndex
    433, 850,                                           // HFreq, VFreq
    832, 509,                                           // HTotal, VTotal
    136, 28,                                            // HStart, VStart
  },
  // 124: 720x400 85Hz (VESA)
  { SYNC_HP_VP | SYNC_HN_VP | SYNC_HN_VN,               // Flags
    RES_720x400,                                        // ResIndex
    379, 850,                                           // HFreq, VFreq
    936, 446,                                           // HTotal, VTotal
    180, 45,                                            // HStart, VStart
  },
  // 125: 1280x768 85Hz
  { SYNC_HN_VP ,                                        // Flags
    RES_1280x768,                                       // ResIndex
    686, 848,                                           // HFreq, VFreq
    1712, 809,                                          // HTotal, VTotal
    352,38,                                             // HStart, VStart
  },
  // 126: 1920x1080P 24Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_24,                                   // ResIndex
    270, 240,                                           // HFreq, VFreq
    2750, 1125,                                         // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    194, 31,                                            // HStart, VStart
#else
    229, 60,                                            // HStart, VStart
#endif
  },
  // 127: 1920x1080P 25Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_25,                                   // ResIndex
    281, 250,                                           // HFreq, VFreq
    2640, 1125,                                         // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    194, 31,                                            // HStart, VStart
#else
    229, 60,                                            // HStart, VStart
#endif
  },
  // 128: 1920x1080P 30Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_30,                                   // ResIndex
    338, 300,                                           // HFreq, VFreq
    2200, 1125,                                         // HTotal, VTotal
#if 0//YPBPR_PERF_OVERSCAN
    194, 31,                                            // HStart, VStart
#else
    229, 60,                                            // HStart, VStart
#endif
  },
  // 129: 1800x1440 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1800x1400,                                      // ResIndex
    218, 599,                                           // HFreq, VFreq
    2440, 1493,                                         // HTotal, VTotal
    73, 27,                                             // HStart, VStart
  },
  // 130: 2048x1536 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_2048x1536,                                      // ResIndex
    109, 599,                                           // HFreq, VFreq
    2208, 1590,                                         // HTotal, VTotal
    73, 27,                                             // HStart, VStart
  },

  // 132: 1920x1080 60Hz FP (HDMI)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x2160,                                      // ResIndex
    1368, 600,                                           // HFreq, VFreq
    2080, 2280,                                         // HTotal, VTotal
    752,  55,                                           // HStart, VStart
  },
  // 133: 3840x2160 30Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_3840x2160,                                      // ResIndex
    675, 300,                                           // HFreq, VFreq
    4400, 2250,                                         // HTotal, VTotal
    752,  55,                                           // HStart, VStart
  },
  // 134: 3840x2160 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_3840x2160,                                      // ResIndex
    1333, 600,                                           // HFreq, VFreq
    4000, 2222,                                         // HTotal, VTotal
    752,  55,                                           // HStart, VStart
  },

  // 134: 3840x2160 60Hz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_4096x2160,                                      // ResIndex
    1348, 600,                                           // HFreq, VFreq
    4400, 2250,                                         // HTotal, VTotal
    752,  55,                                           // HStart, VStart
  },


#if ENABLE_3D_FUNCTION
  // 132: 1920x1080 24Hz FP (HDMI)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x2160,                                      // ResIndex
    541, 240,                                           // HFreq, VFreq
    2750, 2250,                                         // HTotal, VTotal
    192, 41,                                            // HStart, VStart
  },
  // 133: 1280x720P 60Hz FP <--> 1920x1440@60Hz 234MHz
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720P_FP,                                   // ResIndex
    900, 600,                                           // HFreq, VFreq
    1650, 1500,                                         // HTotal, VTotal
    220, 20,                                            // HStart, VStart
  },
  // 134: 1280x720P 50Hz FP
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1280x720P_FP,                                   // ResIndex
    750, 500,                                           // HFreq, VFreq
    1980, 1500,                                         // HTotal, VTotal
    220, 20,                                            // HStart, VStart
  },
  // 135: 1920x1080P 120Hz (2D-CVT-RB)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_3D,                                   // ResIndex
    1372, 1200,                                         // HFreq, VFreq
    2080, 1144,                                         // HTotal, VTotal
    80, 8,                                              // HStart, VStart
  },
  // 136: 1920x1080P 110Hz (2D-CVT-RB)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_3D,                                   // ResIndex
    1251, 1100,                                         // HFreq, VFreq
    2080, 1138,                                         // HTotal, VTotal
    80, 8,                                              // HStart, VStart
  },
  // 137: 1920x1080P 100Hz (2D-CVT-RB)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_3D,                                   // ResIndex
    1133, 1000,                                         // HFreq, VFreq
    2080, 1133,                                         // HTotal, VTotal
    80, 8,                                              // HStart, VStart
  },
  // 138: 1920x1080P 120Hz (3D-CVT-RB)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_3D,                                   // ResIndex
    1378, 1200,                                         // HFreq, VFreq
    2080, 1149,                                         // HTotal, VTotal
    80, 8,                                              // HStart, VStart
  },
  // 139: 1920x1080P 110Hz (3D-CVT-RB)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_3D,                                   // ResIndex
    1257, 1100,                                         // HFreq, VFreq
    2080, 1143,                                         // HTotal, VTotal
    80, 8,                                              // HStart, VStart
  },
  // 140: 1920x1080P 100Hz (3D-CVT-RB)
  { SYNC_HP_VP | SYNC_HP_VN | SYNC_HN_VP | SYNC_HN_VN,  // Flags
    RES_1920x1080_3D,                                   // ResIndex
    1138, 1000,                                         // HFreq, VFreq
    2080, 1138,                                         // HTotal, VTotal
    80, 8,                                              // HStart, VStart
  },
#endif  //ENABLE_3D_FUNCTION
  //=====IMPORTANT    DON'T DELETE IT, OR IT WILL CAUSE LOOP INIFINITE
  { 0,                  // Flags
    (ResolutionType)0,  // ResIndex
    0, 0,               // HFreq, VFreq
    0, 0,               // HTotal, VTotal
    0, 0,               // HStart, VStart
  }
};
WORD GetStandardModeWidth(BYTE InputPort )
{
    return StandardModeResolution[StandardModeGroup(InputPort)].DispWidth;
}
WORD GetStandardModeHeight(BYTE InputPort )
{
    return StandardModeResolution[StandardModeGroup(InputPort)].DispHeight;
}
WORD GetStandardModeGroup(BYTE InputPort )
{
    return StandardMode[SrcModeIndex(InputPort)].ResIndex;
}
WORD GetStandardModeHStart(BYTE InputPort )
{
    return StandardMode[SrcModeIndex(InputPort)].HStart;
}
WORD GetStandardModeVStart(BYTE InputPort )
{
    return StandardMode[SrcModeIndex(InputPort)].VStart;
}
WORD GetStandardModeVTotal(BYTE InputPort )
{
    return StandardMode[SrcModeIndex(InputPort)].VTotal;
}
WORD GetStandardModeHTotal(BYTE InputPort )
{
    return StandardMode[SrcModeIndex(InputPort)].HTotal;
}
WORD GetStandardModeHFreq(BYTE InputPort )
{
    return StandardMode[SrcModeIndex(InputPort)].HFreq;
}
WORD GetStandardModeVFreq(BYTE InputPort )
{
    return StandardMode[SrcModeIndex(InputPort)].VFreq;
}
//========================================================================================
#if 1
#if ENABLE_VGA_INPUT

#define PHASE_STEP         (0x07)

#define CHECK_MODE_WIDTH   1

void appCheckSimilarTiming( BYTE InputPort )
{
#if CHECK_MODE_WIDTH
    WORD xdata wWidth;
#endif
    BYTE vsyncTime, i, ucIndex, uccount;
    DWORD maxcksum, cksum, maxcksum1;
    BYTE adjPhase;
    BYTE u8DetSCIndex = MapPort2DetScIndex(InputPort);

    InputPort = InputPort;

    //temp always return
    if(ucSimilarTimingNums <= 1 || (SrcFlags(InputPort)&bUnsupportMode))
        return;

    vsyncTime = GetVSyncTime(InputPort);
    i = 0;
    maxcksum = maxcksum1 = 0;
    ucIndex = SrcModeIndex(InputPort);
    uccount = (ucSimilarTimingNums>=SIMILAR_TIMING_BUFFER_MAX)?(SIMILAR_TIMING_BUFFER_MAX):(ucSimilarTimingNums);

            //printData(" ucSimilarTimingNums:%d", ucSimilarTimingNums);
    msDrvIP1AutoPhaseSetNoiseMask(7, u8DetSCIndex);

    while(uccount--)
    {
        //111027 Rick add for reload ADC table while check similar timing - B39609
        wWidth = HFREQ( SrcHPeriod(InputPort) );//(( DWORD )MST_CLOCK_MHZ * 10 + SrcHPeriod / 2 ) / SrcHPeriod; //calculate hfreq: round 5
        wWidth = (( DWORD )wWidth * StandardModeHTotal(InputPort) + 5000 ) / 10000; //dclk= hfreq * htotal

        drvADC_SetModewithPLLProtection(g_bInputSOGFlag ? ADC_INPUTSOURCE_YPBPR : ADC_INPUTSOURCE_RGB, wWidth, StandardModeHTotal(InputPort));//tmp need update??
        drvADC_AdjustHTotal(StandardModeHTotal(InputPort));//tmp need update??

#if CHECK_MODE_WIDTH
        wWidth = SearchMaxWidth(vsyncTime, InputPort); //tmp need update??

        //printData(" width:%d", wWidth);

        if(abs(wWidth - StandardModeWidth(InputPort)) < 2)     //110930 Rick modified check similar timing condition
        {
            //printData(" similar width:%d", wWidth);

            return;
        }
#endif

        msDrvIP1SetImageWidth(StandardModeWidth(InputPort), u8DetSCIndex);//_msWrite2Byte( SC0_0B, StandardModeWidth);
        msDrvIP1SetImageHeight(StandardModeHeight(InputPort), u8DetSCIndex);//_msWrite2Byte( SC0_09, StandardModeHeight );

        for( adjPhase = 0; adjPhase < PHASE_MAX; adjPhase += PHASE_STEP)
        {
            drvADC_SetPhaseCode( adjPhase );
            Delay1ms( vsyncTime + 3 );
            msDrvIP1AutoPhaseResultReady(u8DetSCIndex);//WaitAutoStatusReady(SC0_8B, BIT1);
            //cksum = _msRead2Byte(SC0_8E);
            //cksum = ( cksum << 16 ) | _msRead2Byte(SC0_8C);
            cksum = msDrvIP1AutoPhaseReadValue(u8DetSCIndex);
            if( cksum > maxcksum )
            {
                maxcksum = cksum;
            }
        }

        if(maxcksum > maxcksum1)
        {
            maxcksum1 = maxcksum;
            ucIndex = SrcModeIndex(InputPort);
            //printData(" SrcModeIndex_MaxCksm:%d", SrcModeIndex);
        }

        SrcModeIndex(InputPort) = ucSimilarTimingBuffer[i++];

    }

    msDrvIP1AutoPhaseSetNoiseMask(0, u8DetSCIndex);

    SrcModeIndex(InputPort) = ucIndex;

            //printData(" SrcModeIndexFinal:%d", SrcModeIndex);

}
#endif //ENABLE_VGA_INPUT

 #define MAX_INPUT_H_FREQ   1900
 #define MIN_INPUT_H_FREQ    125
 #define MAX_INPUT_V_FREQ   760
 #define MIN_INPUT_V_FREQ    220

 #if (PanelType==PanelT430QVN01)
 #define MAX_FREESYNC_V_FREQ   630
 #define MIN_FREESYNC_V_FREQ    480
 #else
 #define MAX_FREESYNC_V_FREQ   630
 #define MIN_FREESYNC_V_FREQ    480
 #endif



#define DIGITAL_TOLERANCE   3
Bool mStar_FindMode( BYTE InputPort )
{
    WORD hFreq, vFreq;
    BYTE htol, vtol;
    WORD tempHeight=0, tempWidth=0;
    bit bDigitalInput = 0;
    BYTE u8DetSCIndex = MapPort2DetScIndex(InputPort);



#define fStatus  hFreq
    fStatus = mStar_GetInputStatus(InputPort);
    if( SyncPolarity( SrcFlags(InputPort) ) != SyncPolarity( fStatus ) )        // Sync polarity changed
    {
        return FALSE;
    }
    SrcFlags(InputPort) &= 0x1F;
#undef fStatus
    hFreq = msDrvIP1ReadHperiod(u8DetSCIndex);//_msRead2Byte(SC0_E4) & 0x1FFF;
    if( abs( hFreq - SrcHPeriod(InputPort) ) > ((INPUT_IS_DISPLAYPORT(InputPort))?HPeriod_Torlance*8:HPeriod_Torlance) )        // HPeriod changed
    {
        return FALSE;
    }
    vFreq = msDrvIP1ReadVtotal(u8DetSCIndex);//_msRead2Byte(SC0_E2) & 0xFFF;
    if ( !INPUT_IS_HDMI_DRR ( InputPort ) )
    {
	    if( abs( vFreq - SrcVTotal(InputPort) ) > VTotal_Torlance )        // vtotal changed
	    {
	        return FALSE;
	    }
	}
    /////////////////////////////////////////////////////////
#if 0 // 14 bits precison & 16->8 line mode, so removed this overflow protection.
    if( SrcHPeriod(InputPort) > 511 )        // prevent counter overflow when input frequency is very low
    {
        hFreq = SrcHPeriod(InputPort) * 16;
    }
    else
    {
        tempHeight = GetVSyncTime(InputPort) * 8;
        msDrvIP1SetHperiodDetectMode(TRUE, u8DetSCIndex);
        Delay1ms( tempHeight );
        hFreq = msDrvIP1ReadHperiod(u8DetSCIndex)*2; // 8ine * 2
        msDrvIP1SetHperiodDetectMode(FALSE, u8DetSCIndex);
        Delay1ms( tempHeight );
    }

    hFreq=(hFreq+8)/16;
#endif

    MODE_printData( " FindMode_Hperoid:%d", hFreq );
    /////////////////////////////////////////////////////////
    if(!INPUT_IS_HDMI_DRR(InputPort))
        SrcVTotal(InputPort) = vFreq;

    if(!INPUT_IS_DP_DRR(InputPort))
        SrcHPeriod(InputPort) = hFreq;

    if(!INPUT_IS_VGA(InputPort))
    {
        DWORD u32PixClk;
        WORD u16Htt, u16Vtt;

        if(INPUT_IS_DISPLAYPORT(InputPort))
            msAPI_combo_IPGetDPHVInformation((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux, &u16Htt, &u16Vtt);
        else
        {
            u16Htt = msDrvIP1ReadHtotal(u8DetSCIndex)*(HBR2Flag(InputPort)?2:1);
#if 0//CHIP_ID == MST9U4
            u16Htt *= InputPNum(InputPort);
#endif
        }
        u16Vtt = SrcVTotal(InputPort);
        u32PixClk = msAPI_combo_IPGetPixelClock_10KHz((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);
        if((u16Htt!=0)&&(u16Vtt!=0))
        {
            hFreq = ((DWORD)u32PixClk * 100 + u16Htt/2)/u16Htt;
            vFreq = ((DWORD)hFreq * 1000 + u16Vtt/2)/u16Vtt;
        }
    }
    else
    {
        hFreq = HFREQ( SrcHPeriod(InputPort) );
        vFreq = VFREQ( hFreq, SrcVTotal(InputPort) );
    }
    //======== for interlace mode
    if( InterlaceModeFlag(InputPort) )
    {
        vFreq *= 2;
    #ifdef TaiWan_Project		
	 SET_DISPLAY_UNSUPPORT_MODE_FLAG();	
    #endif	
    }
    else
	 CLR_DISPLAY_UNSUPPORT_MODE_FLAG();	
	

    SrcVfreq(InputPort)=vFreq;
    MODE_printData( " FindMode_hFreq:%d", hFreq );
    MODE_printData( " FindMode_vFreq:%d", vFreq );
    MODE_printData( " SrcVTotal(InputPort):%d", SrcVTotal(InputPort) );
#if 0 //ENABLE_DP_OUTPUT
    DPTxSetSCFastLPLL(FALSE,StandardModeHTotal(InputPort),StandardModeVTotal(InputPort),vFreq);
#endif

  #if 0//((defined(LianHeChuangXin_Project))&&(defined(W3K_Project))&&(PanelType==PanelT430QVN01))
        if(hFreq>MAX_INPUT_H_FREQ || hFreq<MIN_INPUT_H_FREQ || vFreq>MAX_INPUT_V_FREQ || vFreq<MIN_INPUT_V_FREQ)
        {   
             
             SrcFlags(InputPort) |= bUnsupportMode;
             return TRUE;
        }
  #endif


  if(hFreq==0||vFreq==0)
     return FALSE;

    if(!INPUT_IS_VGA(InputPort))
    {
        tempWidth = msDrvIP1ReadAutoWidth(u8DetSCIndex);
        tempHeight = msDrvIP1ReadAutoHeight(u8DetSCIndex);
    }

  #if ((defined(LianHeChuangXin_Project)))
     #if (EDID_3840x2160||EDID_2560x1440)
        if(hFreq>MAX_INPUT_H_FREQ || hFreq<MIN_INPUT_H_FREQ || vFreq>MAX_INPUT_V_FREQ || vFreq<MIN_INPUT_V_FREQ)
        {   
             
             SET_DISPLAY_UNSUPPORT_MODE_FLAG();	
             return TRUE;
        }
     #else

        if(((tempWidth>PanelWidth)&&(abs(tempWidth-PanelWidth)>100))||((tempHeight>PanelHeight)&&(abs(tempHeight-PanelHeight)>60)))
        {   
         #if 0//EDID_1920x1080_144
           if((abs(tempWidth-1600)<10)&&(abs(tempHeight-1200)<10))
		 ; 
	    else	   
	 #endif
	    {
             SET_DISPLAY_UNSUPPORT_MODE_FLAG();	
             return TRUE;
	    }
        }		
     #endif

     if(InterlaceModeFlag(InputPort))
     {
          if((abs(tempWidth-1920)<10)&&(abs(tempHeight-PanelHeight)<10))
           ;
	   else
	    {
             SET_DISPLAY_UNSUPPORT_MODE_FLAG();	
             return TRUE;
	    }
     }

    #if FreeSync_Not_Support_Info
        if(( vFreq<MIN_FREESYNC_V_FREQ || vFreq>MAX_FREESYNC_V_FREQ)&&(INPUT_IS_DISPLAYPORT(InputPort)||INPUT_IS_HDMI(InputPort)))
        {   
          if(FreeSyncFlag)
             Set_FreeSyncMsgFlag();	
        }
     #endif
	 
  #endif

     InputVfreq=vFreq;
  
    #if ENABLE_DIGITAL_BYPASS_FINDMODE
    if(!INPUT_IS_VGA(InputPort))
    {
        tempWidth = msDrvIP1ReadAutoWidth(u8DetSCIndex);
        tempHeight = msDrvIP1ReadAutoHeight(u8DetSCIndex);
        MODE_printData("  AutoWidth:%d", tempWidth);
        MODE_printData("  AutoHeight:%d", tempHeight);
        SrcFlags(InputPort) |= bUserMode;
        return TRUE;
    }

    #endif
    //====================================
    // check if input timing is out of range

    htol = HFreq_Torlance; //0527 add for getting more accurate mode
    vtol = VFreq_Torlance;

    ucSimilarTimingNums = 0;
    SrcModeIndex(InputPort) = 0xFF;

    if(!INPUT_IS_VGA(InputPort))//(MapScaler2Port >= Input_Digital)
    {
        bDigitalInput = 1;
#if ENABLE_DP_INPUT     //111027 Rick modified - B39435
        if(INPUT_IS_DISPLAYPORT(InputPort) && HBR2Flag(InputPort))//(MapScaler2Port == Input_Displayport)
        {
            tempWidth = (msDrvIP1ReadAutoWidth(u8DetSCIndex)*2);
            tempHeight = (msDrvIP1ReadAutoHeight(u8DetSCIndex));
        }
        else
#endif
        {
            tempWidth = msDrvIP1ReadAutoWidth(u8DetSCIndex);//_msRead2Byte(SC0_84)-_msRead2Byte(SC0_80)+1; // get DE width
            tempHeight = msDrvIP1ReadAutoHeight(u8DetSCIndex);//_msRead2Byte(SC0_82)-_msRead2Byte(SC0_7E)+1; // get DE Height
        }
    }

#if 0//tmp (CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUM2) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMU) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_TSUMC) || (CHIP_FAMILY_TYPE == CHIP_FAMILY_MST9U)
    if (g_SetupPathInfo.ucSCFmtIn == SC_FMT_IN_3D_FP)
        tempHeight=tempHeight*2-SrcVTotal;
#endif

    // search input mode index
    {
        Bool found = FALSE;
        BYTE ucModeIndex;
        BYTE ucHVtolerance_min = HFreq_Torlance+VFreq_Torlance;
        BYTE ucHVtolerance = HFreq_Torlance+VFreq_Torlance;
        BYTE ucVtotal_min = VTotal_Torlance;
        BYTE ucVtotal = VTotal_Torlance;
        InputModeType *modePtr = StandardMode;
        BYTE ucSimilarindex=0;

        // get standard mode index
        for(ucModeIndex = 0; modePtr->HFreq > 0; modePtr++, ucModeIndex++)
        {
            if(bDigitalInput)
            {
                if((abs(tempWidth - StandardModeResolution[modePtr->ResIndex].DispWidth) > DIGITAL_TOLERANCE) ||
                        (abs(tempHeight - StandardModeResolution[modePtr->ResIndex].DispHeight) > DIGITAL_TOLERANCE))
                    continue;
            }

            if((InterlaceModeFlag(InputPort)) != (modePtr->Flags & bInterlaceMode))
                continue;

            if(SrcVTotal(InputPort) < StandardModeResolution[modePtr->ResIndex].DispHeight)
                continue;

            if((abs(hFreq - modePtr->HFreq) < HFreq_Torlance) &&
                (abs(vFreq - modePtr->VFreq) < VFreq_Torlance) &&
                (abs(SrcVTotal(InputPort) - modePtr->VTotal) < VTotal_Torlance)&&
                (GetSyncPolarity( SrcFlags(InputPort) )&modePtr->Flags))
            {
                ucHVtolerance = abs(hFreq - modePtr->HFreq) + abs(vFreq - modePtr->VFreq);
                ucVtotal = abs(SrcVTotal(InputPort) - modePtr->VTotal);

                if((ucHVtolerance_min >= ucHVtolerance) && (ucVtotal_min >= ucVtotal))
                {
                    found = TRUE;
                    ucHVtolerance_min = ucHVtolerance;
                    ucVtotal_min = ucVtotal;
                    SrcModeIndex(InputPort) = ucModeIndex;
                    SrcAutoHstart(InputPort) = StandardMode[ucModeIndex].HStart;
                    SrcAutoVstart(InputPort) = StandardMode[ucModeIndex].VStart;

                    ucSimilarTimingBuffer[ucSimilarindex] = ucModeIndex;
                    ucSimilarTimingNums ++;
                    ucSimilarindex = (ucSimilarindex+1)%SIMILAR_TIMING_BUFFER_MAX;
                }
            }
        }

        if( !found )
        {
            modePtr = StandardMode;
            ucHVtolerance_min = HFREQ_TOLERANCE_ENLARGE + VFREQ_TOLERANCE_ENLARGE;
            ucVtotal_min = VTotal_Delta;

            for(ucModeIndex = 0; modePtr->HFreq > 0; modePtr++, ucModeIndex++)
            {
                if(bDigitalInput)
                {
                    if((abs(tempWidth - StandardModeResolution[modePtr->ResIndex].DispWidth) > DIGITAL_TOLERANCE) ||
                        (abs(tempHeight - StandardModeResolution[modePtr->ResIndex].DispHeight) > DIGITAL_TOLERANCE))
                        continue;
                }

                if((InterlaceModeFlag(InputPort)) != (modePtr->Flags & bInterlaceMode))
                    continue;

                if(SrcVTotal(InputPort) < StandardModeResolution[modePtr->ResIndex].DispHeight)
                    continue;

                if((abs(hFreq - modePtr->HFreq) < HFREQ_TOLERANCE_ENLARGE) &&
                    (abs(vFreq - modePtr->VFreq) < VFREQ_TOLERANCE_ENLARGE) &&
                    (abs(SrcVTotal(InputPort) - modePtr->VTotal) < VTotal_Delta))
                {
                    ucHVtolerance = abs(hFreq - modePtr->HFreq) + abs(vFreq - modePtr->VFreq);
                    ucVtotal = abs(SrcVTotal(InputPort) - modePtr->VTotal);

                    if((ucHVtolerance_min >= ucHVtolerance) && (ucVtotal_min >= ucVtotal))
                    {
                        found = TRUE;
                        ucHVtolerance_min = ucHVtolerance;
                        ucVtotal_min = ucVtotal;
                        SrcModeIndex(InputPort) = ucModeIndex;
                        ucSimilarTimingBuffer[ucSimilarindex] = ucModeIndex;
                        ucSimilarTimingNums ++;
                        ucSimilarindex = (ucSimilarindex+1)%SIMILAR_TIMING_BUFFER_MAX;
                    }
                }
            }
        }

        if( !found )            // out of standard input range
        {
            modePtr = StandardMode;
            ucVtotal_min = VTotal_Delta;

            for(ucModeIndex = 0; modePtr->HFreq > 0; modePtr++, ucModeIndex++)
            {
                if((InterlaceModeFlag(InputPort)) != (modePtr->Flags & bInterlaceMode))
                    continue;

                if(SrcVTotal(InputPort) < StandardModeResolution[modePtr->ResIndex].DispHeight)
                    continue;

                if(abs(SrcVTotal(InputPort) - modePtr->VTotal) < VTotal_Delta)
                {
                    ucVtotal = abs(SrcVTotal(InputPort) - modePtr->VTotal);

                    if(ucVtotal_min > ucVtotal)
                    {
                        found = TRUE;
                        SrcFlags(InputPort) |= bUserMode;
                        ucVtotal_min = ucVtotal;
                        SrcModeIndex(InputPort) = ucModeIndex;
                        ucSimilarTimingBuffer[ucSimilarindex] = ucModeIndex;
                        ucSimilarTimingNums ++;
                        ucSimilarindex = (ucSimilarindex+1)%SIMILAR_TIMING_BUFFER_MAX;
                    }
                }
            }
        } // out of standard input range
    #if ENABLE_VGA_INPUT
        if(INPUT_IS_VGA(InputPort))
            appCheckSimilarTiming( InputPort );

      if(((SrcModeIndex(InputPort)==MD_720X480P_60)||(SrcModeIndex(InputPort)==MD_640X480_66))&&(abs(SrcVTotal(InputPort)-525)<5))
      	{
           if(abs(vFreq - 625)<5)
		 SrcModeIndex(InputPort)=MD_640X480_66;  	
      	}

      if((SrcModeIndex(InputPort)==MD_640X350_70)||(SrcModeIndex(InputPort)==MD_640X400_70))
      	{
      	  WORD Temp,Height=msDrvIP1ReadAutoHeight(u8DetSCIndex);
	  BYTE i;	  

	      Delay1ms(20);
	for(i=0; i<5; i++)
	{
             Temp=msDrvIP1ReadAutoHeight(u8DetSCIndex);
	      Delay1ms(5);
	      if(abs(Temp-Height)<10)
		  	break;
	}
	
           if(abs(Height - 350)<20)
		 SrcModeIndex(InputPort)=MD_640X350_70;  	
	    else
		 SrcModeIndex(InputPort)=MD_640X400_70;  	
      	}


#if (PanelHeight>=1440)
      if((SrcModeIndex(InputPort)==MD_1920X1440_60R)||(SrcModeIndex(InputPort)==MD_1920X1440_60))
      	{
           SrcModeIndex(InputPort)=MD_2560x1440_60;
    	}	
#endif        
        //if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetSimilarMode]!=NULL)
         //   SrcModeIndex(InputPort) = (((fpCustomerSetSimilarMode)CB_FuncList[eCB_CustomerSetSimilarMode])( InputPort ));
		
    #endif

        if( !found )
        {

            MODE_printMsg( "Not Found--> not support" );

            SrcFlags(InputPort) |= bUnsupportMode;
        }
        else   // found
        {
                MODE_printData("  ucSimilarTimingNums:%d", ucSimilarTimingNums);
                MODE_printData("  SrcModeIndex:%d", SrcModeIndex(InputPort));
                MODE_printData("  MD_3840x2160_60:%d", MD_3840x2160_60);
                MODE_printData("  SrcVTotal(InputPort):%d", SrcVTotal(InputPort));
                MODE_printData("  StandardModeWidth:%d", StandardModeWidth(InputPort));
                MODE_printData("  StandardModeHeight:%d", StandardModeHeight(InputPort));
#if 0 //ENABLE_DP_OUTPUT
          MODE_printData("  StandardModeHtotal:%d", StandardModeHTotal(InputPort));
                MODE_printData("  StandardModeVtotal:%d", StandardModeVTotal(InputPort));
#endif
        }



#if 0 //ENABLE_DP_OUTPUT
if(INPUT_IS_VGA(InputPort))
{DPTxSetSCFastLPLL(TRUE,StandardModeHTotal(InputPort),StandardModeVTotal(InputPort),vFreq);}
#endif

    } // search mode index

    return TRUE;
}


/////////////////////////////////////////////////////////////////////////
///////////////////////////////    find mode  //////////////////////////////
/////////////////////////////////////////////////////////////////////////
void msFindMode( BYTE InputPort )
{
    if( !DoModeSettingFlag(InputPort) ) // while timing change & stable
        return;

    if( !ReadyForChangeFlag(InputPort) )
    {
#if PANEL_VBY1()
#ifdef PanelI2CCommandSetting
    if(!VBY1_Check_LockN_Flag)
    {
      printMsg("111111111111111111111111111");
      Set_VBY1_Check_LockN_Flag();
      msWriteByteMask(REG_140600, 0x00, 0x01); // force at 1st time, especially for HW mode that might show lock(but display error) before we doing the training.
      msDrvCheckVBY1LockN(0);
      
      ForceDelay1ms(30);
      
      if(g_bAPPWindow_Init&&CB_FuncList[eCB_CustomerSetPanelI2CCommand]!=NULL)
        ((fpCustomerSetPanelI2CCommand)CB_FuncList[eCB_CustomerSetPanelI2CCommand])();
    }
#endif
#endif
	
        if( !mStar_FindMode(InputPort) ) // search mode index
        {
            Set_InputTimingChangeFlag(InputPort); // search mode faile
            msAPISrcStatusUpdate(InputPort, IP_STATUS_WAITSYNC);
            return;
        }
		
#if DP_HPD_Test
	TimingUnstableCNT=0;
 #endif

        if( !UnsupportedModeFlag(InputPort) ) //supported mode
        {
            //printf("msFindMode___<Support>\n");
        #if ENABLE_VGA_INPUT
            if (INPUT_IS_VGA(InputPort))
                ReadModeSetting(); // restore user's settings from NVRAM for each mode
            // set status to FINDMODEOK if input mode is supported
            mStar_SetupADC(InputPort);
        #endif
            msAPISrcStatusUpdate(InputPort, IP_STATUS_FINDMODEOK);
        }
        else
        {
            //printf("msFindMode___<OOR>\n");
            msAPISrcStatusUpdate(InputPort, IP_STATUS_FINDMODEOK);
        }
    }
    Set_ReadyForChangeFlag(InputPort); // find mode once
}

Bool IsVideoTiming( BYTE InputPort )
{
    BYTE u8Temp=StandardModeGroup(InputPort);
    if (u8Temp>=RES_720x480I && u8Temp<=RES_1920x1080_60)
        return TRUE;
    return FALSE;
}
#endif

