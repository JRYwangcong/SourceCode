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


#ifdef TSUMXXQX
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
//			 The version format : Xxxx , X = 1, means the LIB is for TSUMOxxQxx-1; if X =0 , the LIB is for TSUMOxxQxx
//		
_MSOVDDEC_ WORD msODLibVersionCheck(void);
//*************************************************************************************************
// Description: Initialize MIU Bank for OverDrive setting when 1st power on.	
//			
_MSOVDDEC_ void mStar_Load_tblOVD(void);
//*************************************************************************************************
//Description: Setup OverDrive mode base on diffenet resolution. 
//caller: 		 mStar_SetupMode() of mstar.c
//
//			 Only TSUMOx8Cxx series need to call this function. Remove it in other series.  
_MSOVDDEC_ void msOD_Setup(void);
//*************************************************************************************************
//Description: Reset MIU bank 
//caller:		 Power_TurnOnPanel of power.c
//
//			 It need to reset MIU bank before turn on panel.
_MSOVDDEC_ void msOD_Reset(BYTE ucStatus);
//*************************************************************************************************
//Description: monitor OverDrive Max. error status.
//caller: 		 main() of main.c
//
//			 Only TSUMOx8Cxx series need to call this function. Remove it in other series. 
_MSOVDDEC_ void msOD_Handler(void);
//*************************************************************************************************
//Description: Enable/Disable OverDrive and setup OD strength
//			ex:  
//			msOD_OnOff( _OD_Disable, 0x50 ); //disable OD
//			msOD_OnOff( _OD_Enable, 0x5F ); //enable OD and set strength as 0x5F
//			OD strength: 0x50 (weak)  <------------>  0x5F (strong)
//
_MSOVDDEC_ void msOD_OnOff(BYTE on, BYTE ODstrength);
//*************************************************************************************************
//Description: Power up/down memory
//caller:         mstar.c "mStar_PowerUp", "mStar_PowerDown"
//
_MSOVDDEC_ void msOD_MIUPowerUp( BYTE on );
//*************************************************************************************************
//Description: memory ssc
//caller:         initial and power up/down.
//
_MSOVDDEC_ void msOD_SetMIUSSC(BYTE SSCMode);
//*************************************************************************************************

#else
//************************************************************************************************
_MSOVDDEC_ void mStar_Load_tblOVD(void);
// caller: mStar_Init(), 
// Description: Initialize MIU Bank for OverDrive setting when 1st power on.	
//*************************************************************************************************
_MSOVDDEC_ void msOD_Setup(void);
//caller: mStar_SetupMode()
//Description: Setup OverDrive mode base on diffenet resolution.  
//*************************************************************************************************
_MSOVDDEC_ void msOD_Reset(BYTE ucStatus);
//Description: Reset MIU bank 
//*************************************************************************************************
_MSOVDDEC_ void msOD_Handler(void);
//caller: main()
//Description: check OverDrive Max. error status.
//*************************************************************************************************
_MSOVDDEC_ void msOD_OnOff(BYTE on, BYTE value);
//Description: Enable/Disable OverDrive and setup OD strength
//OD strength: 0x50 (weak)   <------------>  0x5F (strong)
//		ex:  msOD_OnOff( _OD_Disable, 0x50 ); //disable OD
//			msOD_OnOff( _OD_Enable, 0x5F ); //enable OD and set strength as 0x5F
//*************************************************************************************************
_MSOVDDEC_ WORD msODLibVersionCheck(void);
//Description: For LIB. version check.
//		ex:  	  printData(" OD LIB version = %x"  , msODLibVersionCheck());
//		TSUMOxxGxx-LF 	 : 0x2001
//		TSUMOxxGxx-LF-1 : 0x4101
//*************************************************************************************************
#endif ///end of newodrule

#endif
