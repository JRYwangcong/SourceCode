




//2006-06-20
#include "types.h"
#include "mode.h"
#include "Panel.h"


InputResolutionType code StandardModeResolution[]=
{ 	
	{640, 350},  // Res_640x350,
	{640, 400},  // Res_640x400,
	{640, 480},  // Res_640x480,
	
	{700, 570}, //Res_700x570  3
	{720, 400},  // Res_720x400,
	{720, 480},  // Res_720x240
	{720, 483},  //Res_720x483,
	{720, 576},  //Res_720x576,       7
	
	{800, 600},  // Res_800x600,
	{800, 1209}, // Res_800x1209
	{832, 624},  // Res_832x624,
	{848, 480}, // Res_848x480
	{884, 930},  // Res_884x930,      12
	
	{1024, 768},  // Res_1024x768,
	{1128, 730},  // Res_1128x730,
	{1152, 864},  // Res_1152x864,
	{1152, 870},  // Res_1152x870,        15

	{1280, 600},	// Res_1280x600,
	{1280, 720},  // Res_1280x720,
	{1280, 768}, // Res_1280x768
	{1280, 800}, // Res_1280x800
	{1280, 960},  // Res_1280x960,
	{1280, 1024},  // Res_1280x1024,         //21

	{1360, 768}, // Res_1360x768
	{1366, 768},//Res_1366x768

	{1400, 1050}, // Res_1400x1050
	{1440, 900}, // Res_1440x900        //25

	{1600, 900}, // 
	
	{1600, 1200},  // Res_1600x1200,
	{1680, 1050}, // Res_1680x1050
	
	{1792, 1344}, // Res_1792x1344
	
	{1920, 1080}, // Res_1920x1080
	{1920, 1200}, // Res_1920x1200
};
InputModeType code StandardMode[]=
{
//=========VESA==========
	// 0. 640x350 85Hz (VESA)
	{
	SyncHpVn, // Flags
	Res_640x350, // ResIndex
	379, 851, // HFreq, VFreq
	832, 445, // HTotal, VTotal
	160, 63, // HStart, VStart
	64,
	},

	// 1. 640x400 60Hz // JueryWu 101108; add;
	{
	SyncHpVp | SyncHnVp | SyncHnVn, // Flags
	Res_640x400, // ResIndex
	250, 600, // HFreq, VFreq
	800, 417, // HTotal, VTotal
	144, 14, // HStart, VStart
	64,
	},
	
	// 2. 640x400 85Hz (VESA)
	{
	SyncHpVp | SyncHnVp | SyncHnVn, // Flags
	Res_640x400, // ResIndex
	379, 851, // HFreq, VFreq
	832, 445, // HTotal, VTotal
	160, 44, // HStart, VStart
	64,
	},

	// 3. 640x480 72Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_640x480, // ResIndex
	379, 728, // HFreq, VFreq
	832, 520, // HTotal, VTotal
	130, 23, // HStart, VStart
	40,
	},
	
	// 4. 640x480 75Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_640x480, // ResIndex
	375, 750, // HFreq, VFreq
	840, 500, // HTotal, VTotal
	130, 19, // HStart, VStart
	64,
	},
	
	// 5. 640x480 85Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_640x480, // ResIndex
	433, 850, // HFreq, VFreq
	832, 509, // HTotal, VTotal
	136, 28, // HStart, VStart
	56,
	},

	// 6. 720x400 85Hz (VESA)
	{
	SyncHpVp | SyncHnVp | SyncHnVn, // Flags
	Res_720x400, // ResIndex
	379, 850, // HFreq, VFreq
	936, 446, // HTotal, VTotal
	180, 45, // HStart, VStart
	72,
	},

	// 7. 800x600 56Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_800x600, // ResIndex
	352, 562, // HFreq, VFreq
	1024, 625, // HTotal, VTotal
	140, 24, // HStart, VStart
	72,
	},
	
	// 8. 800x600 60Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_800x600, // ResIndex
	379, 603, // HFreq, VFreq
	1056, 628, // HTotal, VTotal
	92, 24, // HStart, VStart
	128,
	},

	// 9. 800x600 70Hz // JueryWu 101105;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_800x600, // ResIndex
	438, 697, // HFreq, VFreq
	1040, 628, // HTotal, VTotal
	96, 24, // HStart, VStart
	124,
	},
	
	// 10. 800x600 72Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_800x600, // ResIndex
	481, 722, // HFreq, VFreq
	1040, 666, // HTotal, VTotal
	64, 29, // HStart, VStart
	120,
	},
	
	// 11. 800x600 75Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_800x600, // ResIndex
	469, 750, // HFreq, VFreq
	1056, 625, // HTotal, VTotal
	160, 24, // HStart, VStart
	80,
	},
	
	// 12. 800x600 85Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_800x600, // ResIndex
	537, 851, // HFreq, VFreq
	1048, 631, // HTotal, VTotal
	216, 30, // HStart, VStart
	64,
	},

	// 13. 848X480 60HZ 
	{
	SyncHpVp, // Flags
	Res_848x480, // ResIndex
	310, 600, // HFreq, VFreq
	1088, 517, // HTotal, VTotal
	130, 31, // HStart, VStart
	112,
	},

	// 14. 1024x768 60Hz (VESA)
	{ //SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flagsc
	SyncHpVn | SyncHnVn, // Flags
	Res_1024x768, // ResIndex
	484, 600, // HFreq, VFreq
	1344, 806, // HTotal, VTotal
	162, 28, // HStart, VStart
	136,
	},

	// 15. 1024x768 70Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1024x768, // ResIndex
	565, 700, // HFreq, VFreq
	1328, 806, // HTotal, VTotal
	144, 28, // HStart, VStart
	136,
	},

	//16  1024x768 72Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,// 22
	Res_1024x768, // ResIndex
	577, 721, // HFreq, VFreq
	1328, 800, // HTotal, VTotal
	160, 35, // HStart, VStart
	136,
	},

	// 17. 1024x768 75Hz (VESA)
	{ //SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	SyncHpVp | SyncHpVn | SyncHnVn, // Flags
	Res_1024x768, // ResIndex
	600, 750, // HFreq, VFreq
	1312, 800, // HTotal, VTotal
	190, 31, // HStart, VStart
	96,
	},

	// 18. 1024x768 85Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVn, // Flags
	Res_1024x768, // ResIndex
	687, 850, // HFreq, VFreq
	1376, 808, // HTotal, VTotal
	304, 39, // HStart, VStart
	96,
	},

	// 19. 1128x730 60Hz
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1128x730, // ResIndex
	453, 600, // HFreq, VFreq
	1280, 755, // HTotal, VTotal
	41, 21, // HStart, VStart
	96,
	},
	
	// 20. 1152x864 60Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1152x864, // ResIndex
	538, 600, // HFreq, VFreq
	1520,895, // HTotal, VTotal    //2004-09-10
	123, 30, // HStart, VStart
	124,
	},
	
	// 21. 1152x864 70Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1152x864, // ResIndex
	640, 701, // HFreq, VFreq
	1480,912,//14, // HTotal, VTotal
	184, 30, // HStart, VStart
	124,
	},

	// 22. 1152x864 72Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1152x864, // ResIndex
	649, 719, // HFreq, VFreq
	1536, 903,//14, // HTotal, VTotal
	312, 36, // HStart, VStart
	130,
	},
	
	// 23. 1152x864 75Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1152x864, // ResIndex
	675, 750, // HFreq, VFreq
	1600,900, // HTotal, VTotal // 1560, 900, // JueryWu 091228;
	256, 35, // HStart, VStart
	128,
	},
	
	// 24. 1152x864 85Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1152x864, // ResIndex
	771, 850, // HFreq, VFreq
	1576, 907, // HTotal, VTotal
	360, 42, // HStart, VStart
	140,
	},

	// 25. 1280x720 60Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x720, // ResIndex
	450, 600, // HFreq, VFreq
	1650, 750, // HTotal, VTotal
	188, 19, // HStart, VStart
	40,
	},
	
	// 26. 1280x720 70Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x720, // ResIndex
	524, 700, // HFreq, VFreq
	1688, 750, // HTotal, VTotal
	180, 39, // HStart, VStart
	120,
	},

	// 27. 1280x720 72Hz (VESA) // JueryWu 101108; add;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x720, // ResIndex
	542, 719, // HFreq, VFreq
	1680, 753, // HTotal, VTotal
	328, 30, // HStart, VStart
	96,
	},
	
	// 28. 1280x720 75Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x720, // ResIndex
	563, 750, // HFreq, VFreq
	1688, 750, // HTotal, VTotal
	424, 39, // HStart, VStart
	120,
	},  
	

	// 29. 1280x960 60Hz (VESA)
	{ 
	//SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn |bWideMode, // Flags
	SyncHpVp, // | SyncHpVn | SyncHnVp | SyncHnVn,
	Res_1280x960, // ResIndex
	600, 600, // HFreq, VFreq
	1800, 1000, // HTotal, VTotal
	313, 35, // HStart, VStart
	112,
	},  
	
	// 30. 1280x960 70Hz (VESA)
	{
	SyncHpVp, // | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x960, // ResIndex
	700, 700, // HFreq, VFreq
	1727, 1000, // HTotal, VTotal
	353, 38, // HStart, VStart
	94,
	},  
	
	// 31. 1280x960 72Hz (VESA)
	{
	SyncHpVp, //  | SyncHpVn | SyncHnVp | SyncHnVn , // Flags
	Res_1280x960, // ResIndex
	723, 722, // HFreq, VFreq
	1696, 1002,//1728, 1001, // HTotal, VTotal
	353, 38, // HStart, VStart
	96,
	},

	// 32. 1280x960 75Hz (VESA)
	{ //SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	SyncHpVp, // | SyncHpVn | SyncHnVn|SyncHnVp, // Flags
	Res_1280x960, // ResIndex
	750, 750, // HFreq, VFreq
	1728, 1000,//1680, 1000, // HTotal, VTotal
	258, 39, // HStart, VStart
	102,
	},  
	
	// 33. 1280x960 85Hz (VESA)
	{ //SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	SyncHpVp, // Flags
	Res_1280x960, // ResIndex
	859, 850, // HFreq, VFreq
	1728, 1011, // HTotal, VTotal
	384, 50, // HStart, VStart
	102,
	},

	//34. 1280x1024 60Hz (VESA)
	{
	SyncHpVp | SyncHnVn|SyncHpVn|SyncHnVp, // Flags
	Res_1280x1024, // ResIndex
	640, 600, // HFreq, VFreq
	1688, 1066, // HTotal, VTotal
	250, 37, // HStart, VStart
	112,
	},

	//35. 1280x1024 75Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x1024, // ResIndex
	800, 750, // HFreq, VFreq
	1688, 1066, // HTotal, VTotal
	249, 37, // HStart, VStart
	144,
	},
	
	//36. 1280x1024 85Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x1024, // ResIndex
	911, 850, // HFreq, VFreq
	1728, 1072, // HTotal, VTotal
	384, 47, // HStart, VStart
	160,
	},

#if 1// ((PanelType==PNL_USUAL_S1366x768) || (PanelType==PNL_USUAL_D1366x768)) // JueryWu 091223; // PanelType==PanelM156B1L01||PanelType==PanelSVA156WX1 || PanelType == PanelCLAA185WA01|| PanelType == PanelM185B1L01 
	// 37. 1360X768 60Hz // Reduced; // JueryWu 101108; add;
	{
	SyncHpVp,
	Res_1360x768, // ResIndex
	474, 600, // HFreq, VFreq
	1520,790, // HTotal, VTotal
	112, 19, // HStart, VStart
	4,
	},  

	// 38. 1360X768 60Hz 
	{
	SyncHpVp,
	Res_1360x768, // ResIndex
	476, 601, // HFreq, VFreq
	1792,795, // HTotal, VTotal
	260,17, // HStart, VStart
	128,
	},

	// 39. 1360X768 70Hz // Reduced; // JueryWu 101108; add;
	{
	SyncHpVp,
	Res_1360x768, // ResIndex
	561, 699, // HFreq, VFreq
	1792,802, // HTotal, VTotal
	352, 31, // HStart, VStart
	120,
	},

	// 40. 1360X768 75Hz // Reduced; // JueryWu 101108; add;
	{
	SyncHpVp,
	Res_1360x768, // ResIndex
	603, 749, // HFreq, VFreq
	1808,805, // HTotal, VTotal
	368, 34, // HStart, VStart
	112,
	},

	//41 1366x768 60Hz // Reduced blanking;
	{
	SyncHpVp | SyncHnVn,
	Res_1366x768, // ResIndex
	480, 600, // HFreq, VFreq
	1500,800, // HTotal, VTotal
	120, 31, // HStart, VStart
	4,
	},

	//42 1366x768 60Hz // Normal;
	{
	SyncHpVp | SyncHnVn,
	Res_1366x768, // ResIndex
	477, 600, // HFreq, VFreq
	1792,798, // HTotal, VTotal
	230, 17, // HStart, VStart
	128,
	},

	//43 1366x768 60Hz_1  //20150831 lixw  add
	{  
	 SyncHpVp | SyncHpVn,        // Flags
	 Res_1366x768,                                    // ResIndex 
	477, 600,                                         // HFreq, VFreq
	1776,795,                                         // HTotal, VTotal
	230, 26,//362, 17,                                          // HStart, VStart		  
	128,
	},
#endif


	//44. 1600x1200 60Hz (VESA)
	{
	SyncHpVp | SyncHnVn, // Flags
	Res_1600x1200, // ResIndex
	750, 600, // HFreq, VFreq
	2160, 1250, // HTotal, VTotal
	303, 49, // HStart, VStart
	192,
	},
	
	//45. 1600x1200 65Hz (VESA)
	{
	SyncHpVp | SyncHnVn, // Flags
	Res_1600x1200, // ResIndex
	813, 650, // HFreq, VFreq
	2160, 1250, // HTotal, VTotal
	303, 49, // HStart, VStart
	192,
	},
	
	//46. 1600x1200 70Hz (VESA)
	{
	SyncHpVp | SyncHnVn, // Flags
	Res_1600x1200, // ResIndex
	875, 700, // HFreq, VFreq
	2160, 1250, // HTotal, VTotal
	303, 49, // HStart, VStart
	192,
	},
	
	//47. 1600x1200 75Hz (VESA)
	{
	SyncHpVp | SyncHnVn, // Flags
	Res_1600x1200, // ResIndex
	938, 750, // HFreq, VFreq
	2160, 1250, // HTotal, VTotal
	303, 49, // HStart, VStart
	192,
	},

	//48. 1792x1344 60Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1792x1344, // ResIndex
	836, 600, // HFreq, VFreq
	2448, 1394, // HTotal, VTotal
	528, 49, // HStart, VStart
	200,
	},

	//49. 1792x1344 70Hz // JueryWu 101108; add;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1792x1344, // ResIndex
	980, 700, // HFreq, VFreq
	2448, 1401, // HTotal, VTotal
	520, 54, // HStart, VStart
	216,
	},

	//50. 1792x1344 75Hz // JueryWu 101108; add;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1792x1344, // ResIndex
	1053, 749, // HFreq, VFreq
	2448, 1405, // HTotal, VTotal
	520, 58, // HStart, VStart
	216,
	},
	
//========Industry STD======
	// 51. 640x350 70Hz (IMB VGA)
	{
	SyncHpVn, // Flags
	Res_640x350, // ResIndex
	315, 700, // HFreq, VFreq
	800, 449, // HTotal, VTotal
	48, 58, // HStart, VStart
	64,
	},

		// 52. 720x400 70Hz (VGA)
	{
	SyncHpVp | SyncHnVp | SyncHnVn, // Flags
	Res_720x400, // ResIndex
	315, 700, // HFreq, VFreq
	900, 449, // HTotal, VTotal
	60, 35,//155, 35, // HStart, VStart
	40,
	},

	// 53. 640x400 70Hz (IMB VGA)
	{
	SyncHpVp | SyncHnVp | SyncHnVn, // Flags
	Res_640x400, // ResIndex
	315, 700, // HFreq, VFreq
	800, 449, // HTotal, VTotal
	126, 33, // HStart, VStart	
	64,
	},

	// 54. 640x480 60Hz (VESA)
	{ //SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_640x480, // ResIndex
	315, 599, // HFreq, VFreq
	800, 525, // HTotal, VTotal
	50, 33, // HStart, VStart
	96,
	},
	
	// 55. 640x480 66Hz (MAC)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_640x480, // ResIndex
	350, 667, // HFreq, VFreq
	864, 525, // HTotal, VTotal
	100, 42, // HStart, VStart
	102,
	},

	// 56. 640x480 70Hz // JueryWu 101108; add;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_640x480, // ResIndex
	350, 696, // HFreq, VFreq
	800, 503, // HTotal, VTotal
	60, 30, // HStart, VStart
	40,
	},

	// 57. 720x400 60Hz (VGA)
	{
	SyncHpVp | SyncHnVp | SyncHnVn, // Flags
	Res_720x400, // ResIndex
	248, 593, // HFreq, VFreq
	896, 419, // HTotal, VTotal
	80, 40,//155, 35, // HStart, VStart
	64,
	},



	// 58. 832x624 75Hz (MAC)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_832x624, // ResIndex
	497, 746, // HFreq, VFreq
	1152, 665,//667, // HTotal, VTotal
	240, 42, // HStart, VStart
	120,
	},

	//59  1152x870 60Hz (MAC)   
	{
	SyncHnVp, // Flags
	Res_1152x870, // ResIndex
	540, 600, // HFreq, VFreq
	1480, 900, // HTotal, VTotal
	264, 24, // HStart, VStart
	120,
	},

	// 60. 1152x870 75Hz (MAC)
	{
	SyncHpVp | SyncHpVn  | SyncHnVn, // Flags
	Res_1152x870, // ResIndex
	689, 751, // HFreq, VFreq
	1456, 915, // HTotal, VTotal
	145, 39, // HStart, VStart
	124,
	},

	// 61.1280X768 60HZ (Reduce)
	{
	SyncHpVn, // Flags
	Res_1280x768, // ResIndex
	474, 600, // HFreq, VFreq
	1440,790, // HTotal, VTotal
	100, 19, // HStart, VStart
	32,
	},
	
	// 62.1280X768 60HZ (Normal)
	{
	SyncHnVp, // Flags
	Res_1280x768, // ResIndex
	478, 599, // HFreq, VFreq
	1664,798, // HTotal, VTotal
	220, 27, // HStart, VStart
	128,
	},

	// 63.1280X768 70HZ // JueryWu 101108; add;
	{
	SyncHnVp, // Flags
	Res_1280x768, // ResIndex
	560, 698, // HFreq, VFreq
	1696,802, // HTotal, VTotal
	336, 31, // HStart, VStart
	128,
	},
	
	// 64.1280X768 75HZ 
	{
	SyncHnVp, // Flags
	Res_1280x768, // ResIndex
	603, 748, // HFreq, VFreq
	1696,805, // HTotal, VTotal
	208, 19, // HStart, VStart
	128,
	},
	
	// 65 .1280X768 85HZ 
	{
	SyncHnVp, // Flags
	Res_1280x768, // ResIndex
	686, 848, // HFreq, VFreq
	1712,809, // HTotal, VTotal
	352, 38, // HStart, VStart
	130,
	},

	//66 1280x800 60Hz // Reduced blanking;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1280x800, // ResIndex 
	493, 598, // HFreq, VFreq
	1440, 823,// HTotal, VTotal
	112, 20,       // HStart, VStart
	32,
	},
	
	//65 1280x800 60Hz // Normal;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1280x800, // ResIndex 
	497, 598, // HFreq, VFreq
	1680, 831,// HTotal, VTotal
	210, 28,       // HStart, VStart
	128,
	},

	//66 1280x800 70Hz // JueryWu 101108; add;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1280x800, // ResIndex 
	584, 698, // HFreq, VFreq
	1696, 836,// HTotal, VTotal
	336, 33,       // HStart, VStart
	128,
	},
	
	//67 1280x800 75Hz
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1280x800, // ResIndex 
	628, 749, // HFreq, VFreq
	1696, 838,// HTotal, VTotal
	208, 28,       // HStart, VStart
	128,
	},
	
	//68 1280x800 76Hz
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1280x800, // ResIndex 
	647, 764, // HFreq, VFreq
	1568, 846,// HTotal, VTotal
	264, 43,       // HStart, VStart
	120,
	},

	//69 1280x800 85Hz
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1280x800, // ResIndex 
	716, 849, // HFreq, VFreq
	1712, 843,// HTotal, VTotal
	352, 40,       // HStart, VStart
	128,
	},

	// 70 1280 X 1024  72 Hz  
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x1024, // ResIndex 
	779, 720, // HFreq, VFreq
	1728, 1082, // HTotal, VTotal
	230, 41, // HStart, VStart
	144,
	},

	//71 1280 X 1024 70 Hz    
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x1024, // ResIndex
	748, 697, // HFreq, VFreq
	1722/*1696*/, 1064, // HTotal, VTotal  //20150831 lixw  
	226, 37, // HStart, VStart
	112,
	},
/*
	//70. 1400x1050 60Hz // Reduced blanking;
	{
	SyncHpVn, // Flags
	Res_1400x1050, // ResIndex
	647, 599, // HFreq, VFreq
	1560, 1080, // HTotal, VTotal
	112, 27, // HStart, VStart
	32,
	},
	
	//71. 1400x1050 60Hz (VESA)
	{
	SyncHnVp, // Flags
	Res_1400x1050, // ResIndex
	653, 599, // HFreq, VFreq
	1864, 1089, // HTotal, VTotal
	376, 36, // HStart, VStart
	144,
	},

	//72. 1400x1050 70Hz (VESA)
	{
	SyncHnVp, // Flags
	Res_1400x1050, // ResIndex
	766, 699, // HFreq, VFreq
	1880, 1096, // HTotal, VTotal
	384, 43, // HStart, VStart
	144,
	},
	
	//73. 1400x1050 75Hz (VESA)
	{
	SyncHnVp,      // Flags
	Res_1400x1050, // ResIndex
	823, 749,      // HFreq, VFreq
	1896, 1099,    // HTotal, VTotal
	392, 46,     // HStart, VStart
	144,
	},
	
	//74. 1400x1050 85Hz (VESA)
	{
	SyncHnVp,      // Flags
	Res_1400x1050, // ResIndex
	939,  850,      // HFreq, VFreq
	1912, 1105,    // HTotal, VTotal
	408,  52,     // HStart, VStart
	152,
	},
//*/

	// 72 1600x900 60Hz // JueryWu 100524;
	{
	SyncHnVp | SyncHpVn,
	Res_1600x900, // ResIndex
	 555, 599,
       1760, 926,
       82, 17,
	80,
	},

	// 73. 1440X900 60HZ // Reduced blanking;
	{
	SyncHpVn | SyncHnVp, // Flags//SyncHpVn, // Flagsxml 08/11/4
	Res_1440x900, // ResIndex
	555, 599, // HFreq, VFreq
	1600,926, // HTotal, VTotal
	106, 23, // HStart, VStart
	32,
	},
	
	// 74. 1440X900 60HZ // Normal;
	{
	SyncHpVn | SyncHnVp, // Flags//SyncHpVn, // Flagsxml 08/11/4SyncHnVp, // Flags
	Res_1440x900, // ResIndex
	560, 599, // HFreq, VFreq
	1904,934, // HTotal, VTotal
	236, 24, // HStart, VStart
	150,
	},
	
	// 75. 1440X900 70HZ // JueryWu 101108; add;
	{
	SyncHpVn | SyncHnVp, // Flags//SyncHpVn, // Flagsxml 08/11/4SyncHnVp, // Flags
	Res_1440x900, // ResIndex
	656, 698, // HFreq, VFreq
	1920,940, // HTotal, VTotal
	392, 37, // HStart, VStart
	152,
	},
	
	// 76. 1440X900 75HZ 
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1440x900, // ResIndex
	706, 750, // HFreq, VFreq
	1936,942, // HTotal, VTotal
	248,  32, // HStart, VStart
	152,
	},
	
	// 77. 1440X900 85HZ 
	{
	SyncHpVn | SyncHnVp, // Flags
	Res_1440x900, // ResIndex
	804, 848, // HFreq, VFreq
	1952,948, // HTotal, VTotal
	408,  45, // HStart, VStart
	152,
	},



	// 78. 1600X900 60HZ // Reduced blanking;
	{
	SyncHpVp | SyncHnVn, // Flags//SyncHpVn, // Flagsxml 08/11/4
	Res_1600x900, // ResIndex
	600, 600, // HFreq, VFreq
	1800,1000, // HTotal, VTotal
	100, 99, // HStart, VStart
	80,
	},


	//79  1600x900 70Hz // JueryWu 101108;
	{
	SyncHnVp | SyncHpVn,
	Res_1600x900, // ResIndex
	656, 698, // HFreq, VFreq
	2144,940, // HTotal, VTotal
	440, 37, // HStart, VStart
	200,
	},

	//80 1600x900 75Hz // JueryWu 101108;
	{
	SyncHnVp | SyncHpVn,
	Res_1600x900, // ResIndex
	705, 751, // HFreq, VFreq
	2144,942, // HTotal, VTotal
	440, 39, // HStart, VStart
	200,
	},


	// 81. 1680X1050 60HZ // Reduced blanking;
	{
	SyncHpVp|SyncHnVn|SyncHnVp|SyncHpVn, // Flags
	Res_1680x1050, // ResIndex
	647,  599, // HFreq, VFreq
	1840,1080, // HTotal, VTotal
	106,   27, // HStart, VStart
	32,
	},
	
	// 82. 1680X1050 60HZ 
	{
	SyncHnVp|SyncHpVn, // Flags
	Res_1680x1050, // ResIndex
	653 , 600, // HFreq, VFreq
	2240,1089, // HTotal, VTotal
	282 ,  29, // HStart, VStart
	176,
	},

	// 83. 1680X1050 70HZ // JueryWu 101108; add;
	{
	SyncHnVp|SyncHpVn, // Flags
	Res_1680x1050, // ResIndex
	766 , 699, // HFreq, VFreq
	2272,1096, // HTotal, VTotal
	472 ,  43, // HStart, VStart
	176,
	},

	// 84. 1680X1050 75HZ 
	{
	SyncHnVp|SyncHpVn, // Flags
	Res_1680x1050, // ResIndex
	823 , 749, // HFreq, VFreq
	2272,1099, // HTotal, VTotal
	300 ,  46, // HStart, VStart
	176,
	},

	//85. 1920x1080 60Hz (VESA) // JueryWu 100514;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1920x1080, // ResIndex
	672, 600, // HFreq, VFreq
	2576, 1120, // HTotal, VTotal
	326, 37, // HStart, VStart
	204,
	},
	//86. 1920x1080 60Hz (VESA) // JueryWu 100514;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1920x1080, // ResIndex
	666, 600, // HFreq, VFreq
	2080, 1111, // HTotal, VTotal
	100, 37, // HStart, VStart
	204,
	},

	//87. 1920x1080 60Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1920x1080, // ResIndex
	675, 600, // HFreq, VFreq
	2200, 1125, // HTotal, VTotal
	149, 35, // HStart, VStart
	44,
	},

	//88. 1920x1080 70Hz // JueryWu 101108; add;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1920x1080, // ResIndex
	788, 699, // HFreq, VFreq
	2592, 1127, // HTotal, VTotal
	536, 44, // HStart, VStart
	220,
	},

	//89. 1920x1080 75Hz // JueryWu 101108; add;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1920x1080, // ResIndex
	846, 749, // HFreq, VFreq
	2608, 1130, // HTotal, VTotal
	340, 47, // HStart, VStart
	220,
	},
	
	//90. 1920x1200 60Hz // Reduced blanking;
	{
	SyncHnVp|SyncHpVn, // Flags
	Res_1920x1200, // ResIndex
	740, 600, // HFreq, VFreq
	2080, 1235, // HTotal, VTotal
	112, 32, // HStart, VStart
	32,
	},

	//91. 1920x1200 60Hz // Normal;
	{
	SyncHnVp|SyncHpVn, // Flags
	Res_1920x1200, // ResIndex
	746, 600, // HFreq, VFreq
	2592, 1245, // HTotal, VTotal
	536, 42, // HStart, VStart
	200,
	},

	//92. 1920x1200 70Hz // JueryWu 101108; add;
	{
	SyncHnVp|SyncHpVn, // Flags
	Res_1920x1200, // ResIndex
	875, 699, // HFreq, VFreq
	2608, 1252, // HTotal, VTotal
	552, 49, // HStart, VStart
	208,
	},

	//93. 1920x1200 75Hz // JueryWu 101108; add;
	{
	SyncHnVp|SyncHpVn, // Flags
	Res_1920x1200, // ResIndex
	940, 749, // HFreq, VFreq
	2608, 1255, // HTotal, VTotal
	552, 52, // HStart, VStart
	208,
	},

//========Detail Timing======
	// 94. 1280x720 60Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x720, // ResIndex
	447, 600, // HFreq, VFreq
	1440, 741, // HTotal, VTotal
	424, 39, // HStart, VStart
	196,
	},

//========Special Timing=====
	// 95. 1280x600 60Hz // JueryWu 101105;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x600, // ResIndex
	374, 599, // HFreq, VFreq
	1632, 624, // HTotal, VTotal
	178, 21, // HStart, VStart
	112,
	},

	// 96. 1280x600 60Hz (Reduced) // JueryWu 101105;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x600, // ResIndex
	370, 597, // HFreq, VFreq
	1440, 619, // HTotal, VTotal
	112, 16, // HStart, VStart
	32,
	},

	// 97. 1280x600 70Hz // JueryWu 101108; add;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1280x600, // ResIndex
	439, 699, // HFreq, VFreq
	1664, 628, // HTotal, VTotal
	320, 25, // HStart, VStart
	128,
	},

	//98  700x570 50Hz (For India TV Box)   
	{
	SyncHpVp  , // Flags
	Res_700x570, // ResIndex
	313, 500, // HFreq, VFreq
	884, 625, // HTotal, VTotal
	179, 43, // HStart, VStart
	64,
	},

	//99. 720x480 60Hz (YCbCr)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn | bInterlaceMode, // Flags
	Res_720x480, // ResIndex
	156, 500, // HFreq, VFreq
	900, 525, // HTotal, VTotal
	144, 88, // HStart, VStart
	46,
	},

	//100 720x480I 60Hz (YPbPr)  --> 78  Start of YPbPr
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn | bInterlaceMode, // Flags
	Res_720x480, // resolution index
	156, 600,    // HFreq, VFreq
	858, 525,    // HTotal, VTotal
	54, 26,      // HStart, VStart
	24,
	},

	// 101. 480P 60Hz
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_720x483, // ResIndex
	315, 599, // HFreq, VFreq
	858, 525, // HTotal, VTotal
	122, 36,   // HStart, VStart    
	32,
	},

	//102. 576P(YCbCr)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_720x576, // ResIndex
	312, 500, // HFreq, VFreq
	864, 625, // HTotal, VTotal
	125, 40, // HStart, VStart
	46,
	},

	//103  576 // 75Hz;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_720x576, // ResIndex
	455, 750, // HFreq, VFreq
	944, 602, // HTotal, VTotal
	169, 20, // HStart, VStart
	76,
	},
	
	//104. 576i // 50Hz;
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn | bInterlaceMode, // Flags
	Res_720x576, // ResIndex
	156, 500, // HFreq, VFreq
	864, 625, // HTotal, VTotal
	132, 22, // HStart, VStart
	64,
	},

	// 105. 800x1209 //TPV
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_800x1209, // ResIndex
	756, 600, // HFreq, VFreq
	1072, 1259, // HTotal, VTotal
	152+96, 46-3, // HStart, VStart
	128,
	},

	// 106. 884x930
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_884x930, // ResIndex
	600, 612, // HFreq, VFreq
	1133, 980, // HTotal, VTotal
	235, 31, // HStart, VStart
	102,
	},
	
	// 107. 1152x864 70Hz (For ATI Card)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1152x864, // ResIndex
	662, 702, // HFreq, VFreq
	1536, 900,//14, // HTotal, VTotal
	288, 49, // HStart, VStart
	128,
	},

	//108. 720p 60Hz
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1280x720, // ResIndex 
	447, 600, // HFreq, VFreq
	1650, 750, // HTotal, VTotal
	296, 20, // HStart, VStart
	128,
	},  

	//109 1920x1080i 50Hz (YPbPr)  --> 84
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn | bInterlaceMode, // Flags    
	Res_1920x1080, // resolution index
	281, 500,      // HFreq, VFreq
	2640, 1125,    // HTotal, VTotal
	186, 24,       // HStart, VStart
	128,
	},		
	
	//110. NTSC 1080i 
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn | bInterlaceMode, // Flags    
	Res_1920x1080, // resolution index
	337, 600,      // HFreq, VFreq
	2200, 1125,    // HTotal, VTotal
	232, 27,       // HStart, VStart
	112,
	},	

	//111. PAL 1080p
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1920x1080, // ResIndex 
	562, 500, // HFreq, VFreq
	2640, 1125, // HTotal, VTotal
	112, 28, // HStart, VStart
	32,
	},
	
	//112. NTSC 1080p 
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,   // Flags
	Res_1920x1080, // ResIndex 
	666, 599, // HFreq, VFreq
	2200, 1125,// HTotal, VTotal
	232, 34,       // HStart, VStart
	102,
	},

//=======================================

	// 113. 1280x960 60Hz (VESA)
	{ 
	//SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn |bWideMode, // Flags
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn,
	Res_1280x960, // ResIndex
	600, 600, // HFreq, VFreq
	1696,996,//1800, 1000, // HTotal, VTotal
	300, 30, // HStart, VStart
	204,
	},  

#if 0//1
	// 17. 1024x768 43i (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn | bInterlaceMode, // Flags
	Res_1024x768, // ResIndex
	355, 868, // HFreq, VFreq
	1264, 817, // HTotal, VTotal
	232, 24, // HStart, VStart
	},
#endif

#if 0
	// 35. 1024x768 60Hz (VESA)
	{
	//SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flagsc
	SyncHpVn | SyncHnVn, // Flags
	Res_1024x768, // ResIndex
	484, 600, // HFreq, VFreq
	1312, 813, // HTotal, VTotal
	259, 35, // HStart, VStart
	},
#endif
	// 114. 1024x768 75Hz (MAC)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1024x768, // ResIndex
	602, 749, // HFreq, VFreq
	1326, 804, // HTotal, VTotal
	180, 33, // HStart, VStart
	102,
	},

#if 0
	//43. 1400x1050 60Hz (VESA)
	{
	SyncHpVp | SyncHpVn | SyncHnVp | SyncHnVn, // Flags
	Res_1400x1050, // ResIndex
	653, 600, // HFreq, VFreq
	1864, 1089, // HTotal, VTotal
	376, 36, // HStart, VStart
	},
#endif
	
	// 115. 1024x768 72Hz (VESA)  //2006-07-07 Andy
	{
	//SyncHpVp | SyncHpVn | SyncHnVp  // Flags
	SyncHpVp, // Flags
	Res_1024x768, // ResIndex
	571, 721, // HFreq, VFreq
	1312, 800, // HTotal, VTotal
	272, 31, // HStart, VStart
	102,
	},

	//=====IMPORTANT	DON'T DELETE IT, OR IT WILL CAUSE LOOP INIFINITE
	{
	0, // Flags
	0, // ResIndex
	0, 0, // HFreq, VFreq
	0, 0, // HTotal, VTotal
	0, 0, // HStart, VStart
	0,
	}
};

WORD GetStandardModeWidth(BYTE StandardModeGroup)
{
    return StandardModeResolution[StandardModeGroup].DispWidth;
}

WORD GetStandardModeHeight(BYTE StandardModeGroup)
{
    return StandardModeResolution[StandardModeGroup].DispHeight;
}

WORD GetStandardModeGroup(BYTE SrcModeIndex)
{
	return StandardMode[SrcModeIndex].ResIndex;
}

WORD GetStandardModeHTotal(BYTE SrcModeIndex)
{
	return StandardMode[SrcModeIndex].HTotal;
}

WORD GetStandardModeVFreq(BYTE SrcModeIndex)
{
	return StandardMode[SrcModeIndex].VFreq;
}

WORD GetStandardModeHStart(BYTE SrcModeIndex)
{
    return StandardMode[SrcModeIndex].HStart;
}

WORD GetStandardModeVStart(BYTE SrcModeIndex)
{
    return StandardMode[SrcModeIndex].VStart;
}
