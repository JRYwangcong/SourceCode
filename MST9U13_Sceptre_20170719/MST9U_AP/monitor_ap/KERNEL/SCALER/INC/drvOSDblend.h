
#ifndef _DRVOSDBLEND_H
#define _DRVOSDBLEND_H

#ifdef _DRVOSDBLEND_C
#define INTERFACE
#else
#define INTERFACE   extern
#endif

#if ENABLE_OSDC_OSDD
#define _LIMIT_OSDSIZE_         ((PANEL_WIDTH >1920)?1:0)
#define LIMIT_OSDC_HSIZE        (PANEL_WIDTH/2)
#define LIMIT_OSDC_VSIZE        (PANEL_HEIGHT/2)
#endif

#define SOSD_SOURCE             0
#define GOP_SOURCE              1

//osdc input data format
/*
0: ARGB8888
1: ARGB4444_OLD
2: ARGB4444_NEW
3: ARGB4444_THD
4: ARGB1555_OLD
5: ARGB1555_NEW
6: ARGB1555_THD
7: RGB888_UP
8: RGB888_DN
*/
typedef enum{
    eOSDC_CLK_320M,
    eOSDC_CLK_123M,
    eOSDC_CLK_172M,
    eOSDC_CLK_86M,
}eOSDC_CLK;

typedef enum{
    eOSDC_ARGB8888,
    eOSDC_ARGB4444_1,
    eOSDC_ARGB4444_2,
    eOSDC_ARGB4444_3,
    eOSDC_ARGB1555_1,
    eOSDC_ARGB1555_2,
    eOSDC_ARGB1555_3,
    eOSDC_RGB888_UP,
    eOSDC_RGB888_DN,
}eOSDC_FRMT;

typedef struct{
    WORD u16width;
    WORD u16hiegh;
}sOSDC_SIZE;

typedef struct{
    WORD u16osdc_width;
    WORD u16osdc_hiegh;
    WORD u16blend_x;
    WORD u16blend_y;
    WORD u16blend_width;
    WORD u16blend_hiegh;
    BYTE u8osdc_guardband;
    eOSDC_FRMT eOSDC_format;
    eOSDC_CLK eOSDC_clk;

    BYTE bIsSourceGOP:1;
    BYTE bScaling: 1;

}sOSDBLEND_CONFIG;


extern sOSDBLEND_CONFIG sOsdblendConfig;

INTERFACE void msDrvOSDBlend_WindowConfig(WORD u16Blend_x, WORD u16Blend_y, sOSDC_SIZE sOSDCWin, sOSDC_SIZE sOSDDWin);
INTERFACE void msDrvOSDBlend_Init(BYTE source);
INTERFACE void msDrvOSDBlend_Enable(BOOL bEnable);
INTERFACE void msDrvOSDBlend_WindowPos(WORD u16HPos, WORD u16YPos);
INTERFACE void msDrvOSDBlend_WindowPosRatio(BYTE u16HRatio, WORD u16VRatio);
INTERFACE void msDrvOSDBlend_WindowSize(WORD u16Width, WORD u16Height);
INTERFACE void msDrvOSDBlend_WindowReconfig(void);
INTERFACE void msDrvOSDBlend_FrcOsdb_Enable(BOOLEAN bEnable);
INTERFACE void msDrvOSDBlendFBSwitch(BOOL bEnable);

#undef INTERFACE
#endif ///_DRVOSDBLEND_H

