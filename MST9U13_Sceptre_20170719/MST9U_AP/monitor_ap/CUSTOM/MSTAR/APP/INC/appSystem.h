#ifndef _APPSYSTEM_H_
#define _APPSYSTEM_H_

#ifdef _APPSYSTEM_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif
////////////////////////////////
///// Struct MUST Sync with 51 /////
typedef struct
{ //MUST CARE 32 bits align & endian (WORD only)
    //////////////////////
    //MSTAR DRIVER AREA//
    WORD    u16PMMode;
    //MSTAR DRIVER AREA//
    //////////////////////

    //////////////////////
    /// CUSTOMER AREA ///
    WORD    u16PMPortIndex;
    WORD    u16PMPortSkipIndex;
    WORD    u16PMFlag;      
    WORD    u16PMCounter;      
	
    /// CUSTOMER AREA ///
    //////////////////////
}__attribute__((packed))sMAILBOX_R2;

typedef struct
{ //MUST CARE 32 bits align & endian (WORD only)
    //////////////////////
    //MSTAR DRIVER AREA//
    WORD    u16PMWakeupStatus;
    //MSTAR DRIVER AREA//
    //////////////////////

    //////////////////////
    /// CUSTOMER AREA ///
    WORD    u8PMActivePort;
    /// CUSTOMER AREA ///
    //////////////////////
}__attribute__((packed))sMAILBOX_51;


 #if(CHIP_ID == MST9U4)	
#define    SET_DP_SYSYEM_RUN_ENABLE()   	(msWriteByteMask(REG_0011C5,BIT7,BIT7)) 	  	
  #endif
INTERFACE sMAILBOX_R2 g_sMailBoxR2;
INTERFACE sMAILBOX_51 g_sMailBox51;
///// Struct MUST Sync with 51 /////
////////////////////////////////
INTERFACE void APPSystem_Handler( void );
INTERFACE void APPSystem_Init( void );
INTERFACE void APPSystem_ActivePort_For_PowerOn( void );

#undef INTERFACE
#endif

