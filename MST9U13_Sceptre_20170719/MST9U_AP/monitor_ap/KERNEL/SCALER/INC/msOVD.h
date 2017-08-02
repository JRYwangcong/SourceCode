#ifndef _MSOVD_H_
#define _MSOVD_H_

#ifdef _MSOVD_C_
  #define _MSOVDDEC_
#else
  #define _MSOVDDEC_ extern
#endif


typedef enum
{
   _OD_Disable,
   _OD_Enable

}ODStatusType;

typedef enum
{
   _MIU_SSC_OFF,
   _MIU_SSC_40K_ZERO_POINT_FIVE_PERCENT,    //40k, 0.5%
   _MIU_SSC_40K_ONE_PERCENT,                //40K,1%
   _MIU_SSC_40K_TWO_PERCENT,                //40K,2%
   _MIU_SSC_40K_THREE_PERCENT               //40K,3%

}MIUSSCType;

//*************************************************************************************************
//Description: OD look-up table loader.
//
//           ex: msLoadODT(tOverDrive)
//
//           It provides two kinds of format (normal and compressed) for OD look-up table.
//           The look-up table and OD lib should be located in the same bank!!

//_MSOVDDEC_ code BYTE tOverDrive[];
#if ENABLE_RTE
#if ENABLE_OD_MTB
 extern code BYTE* tOverDrive_AllTbl[];
 //extern BYTE tMultiODTbl[] __attribute__((aligned(16)));  // keep this to verify MultiOD
#else
 extern BYTE tOverDrive[];
#endif
#endif
extern void msAPI_SwitchOverDriveMODE(BYTE mode);
extern void msAPI_AdjustOverDriveWeighting(BYTE u8Weighting);

#endif
