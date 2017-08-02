
#ifndef MS_PM_H
#define MS_PM_H

#ifdef _MS_PM_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

//#define PM_SPI_START_ADDR       0x10000 //setting moved to sboot

////////////////////////////////
///// Enum MUST Sync with 51 /////
typedef enum
{
    ePM_POWER_ON,
    ePM_POWER_DPMS,
    ePM_POWER_DCOFF,

    ePM_INVAILD
}ePM_Mode;

typedef enum
{
    ePMSTS_NON,
    ePMSTS_VGA_ACT,
    ePMSTS_DVI_ACT,
    ePMSTS_SAR_ACT,
    ePMSTS_GPIO_ACT,
    ePMSTS_MCCS04_ACT,
    ePMSTS_MCCS05_ACT,
    ePMSTS_MCCS01_ACT,
    ePMSTS_CEC_ACT,
    ePMSTS_FORCE_ON,
    ePMSTS_DP_ACT,
    ePMSTS_MHL_ACT,
    ePMSTS_POWERGPIO_ACT,
    ePMSTS_CABLESAR_ACT,
    ePMSTS_FACTORY_ACT,
    ePMSTS_KEYWAKEUP_ACT,
    
    ePMSTS_INVAID
}ePM_WakeupStatus;
///// Enum MUST Sync with 51 /////
////////////////////////////////


///////////////////////////////////////////////////////
//////////////////////// System ////////////////////////
///////////////////////////////////////////////////////
//  RCOSC = XTAL * Counter / 513 => Counter = RCOSC *513/XTAL,
//  IF RCOSC=12M, Xtal=12M, Counter=513 =>0x201
#if (CHIP_ID == MST9U3)
#define RCOSC_TARGET    0x801 //12MHz
#else
#define RCOSC_TARGET    0x201 //12MHz
#endif
INTERFACE Bool msPM_StartRCOSCCal(void);
INTERFACE void msPM_Handler(void);
///////////////////////////////////////////////////////
/////////////////////////// API ////////////////////////
///////////////////////////////////////////////////////
INTERFACE void msAPI_PMSetMode( ePM_Mode ePMMode );
INTERFACE ePM_WakeupStatus msAPI_PMGetWakeupStatus( void );
INTERFACE WORD msAPI_PMGetWakeupActivePort( void );
INTERFACE ePM_Mode    g_ePMMode;

#undef INTERFACE
#endif

