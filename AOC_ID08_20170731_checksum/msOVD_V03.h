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
   _MIU_SSC_40K_ZERO_POINT_FIVE_PERCENT,	//40k, 0.5%
   _MIU_SSC_40K_ONE_PERCENT,				//40K,1%
   _MIU_SSC_40K_TWO_PERCENT,				//40K,2%
   _MIU_SSC_40K_THREE_PERCENT				//40K,3%

}MIUSSCType;

//*************************************************************************************************
//Description: OD look-up table loader.
//
//			 ex: msLoadODT(tOverDrive)
//
//			 It provides two kinds of format (normal and compressed) for OD look-up table.
//			 The look-up table and OD lib should be located in the same bank!!
_MSOVDDEC_ void msLoadODT(Bool bODTblComp, BYTE *pODTbl);
//*************************************************************************************************
//Description: For LIB. version check.
//
//			 ex:  	  printData(" OD LIB version = %x"  , msODLibVersionCheck());
//
//			 The version format : XYxx , X: Chip type, Y: _1 or not, xx: version
//
_MSOVDDEC_ WORD msODLibVersionCheck(void);
//*************************************************************************************************
// Description: Initialize MIU Bank for OverDrive setting when 1st power on.
//
_MSOVDDEC_ void mStar_Load_tblOVD(void);
//*************************************************************************************************
//Description: Reset MIU bank
//caller:		 Power_TurnOnPanel of power.c
//
//			 It need to reset MIU bank before turn on panel.
_MSOVDDEC_ void msOD_Reset(Bool bEnable);
//*************************************************************************************************
//Description: Enable/Disable OverDrive and setup OD strength
//			ex:
//			msOD_OnOff( _OD_Disable, 0x50 ); //disable OD
//			msOD_OnOff( _OD_Enable, 0x5F ); //enable OD and set strength as 0x5F
//			OD strength: 0x50 (weak)  <------------>  0x5F (strong)
//
_MSOVDDEC_ void msOD_OnOff(BYTE on, BYTE ODstrength);
//*************************************************************************************************
//Description: Power up/down memory , only for TSUMQ
//caller:         mstar.c "mStar_PowerUp", "mStar_PowerDown"
//
_MSOVDDEC_ void msOD_MIUPowerUp( BYTE on );
//*************************************************************************************************
//Description: memory ssc, only for TSUMQ
//caller:         initial and power up/down.
//
_MSOVDDEC_ void msOD_SetMIUSSC(BYTE SSCMode);
//*************************************************************************************************

_MSOVDDEC_ Bool msODMemoryTest(void);

#endif
