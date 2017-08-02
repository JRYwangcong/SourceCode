
#ifndef _DRVMSOVD_H_
#define _DRVMSOVD_H_
#ifndef _MSOVD_D_H_
#define _MSOVD_D_H_

#ifdef _MSOVD_D_C_
  #define _MSOVDDEC_D_
#else
  #define _MSOVDDEC_D_ extern
#endif

//*************************************************************************************************
// Description: OD look-up table loader.
//
// ex: msLoadODT(tOverDrive)
//
// It provides two kinds of format (normal and compressed) for OD look-up table.
// The look-up table and OD lib should be located in the same bank!!
#if ENABLE_RTE && (!MST9U_ASIC_1P) && (OD_HSCAL_EN)
#define OD_GUARD_BAND   16
#else
#define OD_GUARD_BAND   0
#endif

#if CHIP_ID==MST9U|CHIP_ID==MST9U2|CHIP_ID==MST9U3
#define OD_BusLength   16
#else //MST9U4
#define OD_BusLength   32
#endif

_MSOVDDEC_D_ void msDrvOverDriverMultiInit(BYTE *pODTbl);
_MSOVDDEC_D_ void msDrvOverDriverInit(void);
_MSOVDDEC_D_ void msDrvOverDriveTable( BYTE *pODTbl);
_MSOVDDEC_D_ void msDrvOvreDriveTable2nd( BYTE *pODTbl);
_MSOVDDEC_D_ void msDrvOverDriveOnOff(Bool bEnable);

_MSOVDDEC_D_ void msDrvOverDriveMode(BYTE mode, BYTE h_scale, BYTE v_scale, BYTE OD_2nd_EN);
_MSOVDDEC_D_ void msDrvOverDriveWindow(BYTE enable, WORD u16HST,WORD u16HEND,  WORD u16VST, WORD u16VEND);
_MSOVDDEC_D_ void msDrvOverDriveAdjustWeighting(BYTE u8Weighting);
_MSOVDDEC_D_ void msDrvOverDriveVerify(void);
_MSOVDDEC_D_ void msDrvOverDriveVerify2nd(void);
_MSOVDDEC_D_ void msLoad_MultiODTable(BYTE index);

#undef _MSOVDDEC_D_
#endif


#endif

