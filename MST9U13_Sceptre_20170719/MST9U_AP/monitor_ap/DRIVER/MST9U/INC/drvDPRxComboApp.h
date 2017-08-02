
#ifndef _DPRxComboApp_H_
#define _DPRxComboApp_H_


#if ENABLE_DP_INPUT

#ifndef _drvDPRXCOMBOAPP_7_H_
#define _drvDPRXCOMBOAPP_7_H_

#ifdef _DPRXCOMBOAPP_C_
#define _DPRXCOMBOAPP_DEC_
#else
#define _DPRXCOMBOAPP_DEC_ extern
#endif

#define _DPDATATYPE_            xdata
#define _DPDATATYPEEDID_        xdata

#define DPPacketMute    1

#ifndef UNUSED //to avoid compile warnings...
    #define UNUSED(var) (void)((var) = (var))
#endif

enum
{
    eComSTATE_INITIAL,     // 0
    eComSTATE_CKECKLOCK,   // 1
    eComSTATE_MEASURE,     // 2
    eComSTATE_NORMAL,      // 3
    eComSTATE_POWERSAVE,  //4 //4
    eComSTATE_DPIDLE       // 5
};


typedef enum
{
      Combo_DP_COLOR_DEPTH_6_BIT  = 0,
      Combo_DP_COLOR_DEPTH_8_BIT  = 1,
      Combo_DP_COLOR_DEPTH_10_BIT = 2,
      Combo_DP_COLOR_DEPTH_12_BIT = 3,
      Combo_DP_COLOR_DEPTH_16_BIT = 4,
      Combo_DP_COLOR_DEPTH_UNKNOWN = 5,
} Combo_DP_COLOR_DEPTH_FORMAT;


/*
typedef enum
{
    InputRange_Default=0,
    InputRange_Limited,
    InputRange_Full,
    InputRange_Max
}DPRangeType;

*/



typedef struct
{
	
	WORD	uwDPHtotal;
	WORD	uwDPVtotal;
	WORD	uwDPHWidth;
	WORD	uwDPVWidth;
	WORD	uwDPHPWS;
	WORD	uwDPVPWS;
	WORD	uwDPHStart;
	WORD	uwDPVStart;
	Bool	        bDPHPol;
	Bool	        bDPVPol;
	BYTE	ucDPColorFormat;
	Bool	        bDPLock;
	Bool	        bDPInterlace;
	Bool	        bDPNoVideoStrm;
	Bool	        bDPNoAudioStrm;
	Bool	        bDPPowerSaving;
	Bool	        bAudioMute;
	Bool         bDPSave;
	Bool         bAupllRun;
       Bool        bSquelchWaitStable;      
       Bool        bSquelchStable;   
       WORD      uwDPRxStableTimeout;
	BYTE       ucState;
	BYTE       DPState;
	Bool        bDPLANELOCK;
	Bool		bDPCDRLOCK;
       BYTE        bDPTrainingP1T;
       BYTE        bDPTrainingP0T;
	BYTE	 bLockFailCnt;
	WORD      uwFastTrainingCntDown;
	BYTE	 ucDPRangeFormat;
	BYTE        ucDPColorDepth;
	bool		 bDPAUPLLBIGChange;
	bool	 	 bDPVPLLBIGChange;
	bool		 bDPHDCPIRQ;
	DWORD    DPAUM_OLD;
	DWORD     DPAUN_OLD;
       BYTE	    DPAudioKM_OLD;
       BYTE	    DPAudioKP_OLD;
       DWORD      DPAudioFBDIV_OLD;	
	WORD	   DPHDCPCnt; 
	bool           bDPLoseCDRLockIRQ; 
	bool   	   bDPInterlaneSkewIRQ;
	WORD    	   uwSkewHPDCnt;
	WORD        uwCDRHPDCnt;
	BYTE          CTSLevel;
	Bool           bColorType; 
#if LOOPOUT_DPNOSIGNAL_SOLUTION
	WORD       uwReChkTiming;	// 140528 DMD Loop Out에서 간헐적 화면 안나옴 개선을 위한 Spot 솔루션 적용
	Bool 	   bReChkEnable;    // 140528 DMD Loop Out에서 간헐적 화면 안나옴 개선을 위한 Spot 솔루션 적용
#endif	
	BYTE          bPacket;
	BYTE          bPacketTime;
 } DP_Combo_INFO;

extern DP_Combo_INFO xdata gDPComboInfo[6];


#define  COMBO_OFFSET       0x600


#ifndef UNUSED //to avoid compile warnings...
    #define UNUSED(var) (void)((var) = (var))
#endif
extern BYTE xdata DP_Combo_Connected[6];
extern HBR_IP_SELECT  xdata DP_HBR_0;



void DPRxComboSet( BYTE ucComboPort );
extern void DPRxComboHandle( void );
void DPRxSetCombo_0(BYTE ucComboPort);
void DPRxSetCombo_1(BYTE ucComboPort);
extern void DPRxComboInit( void );
void DPRxComboTiming( BYTE ucComboPort );
void DPComboState( BYTE state );
BOOL DPRxComboCheckLock( BYTE ucComboPort  );
void DPRxComboIRQEnable( Bool bEnable, BYTE ucComboPort );
extern WORD DPRxComboOffset(BYTE ComboPort);
void DPRxComFastTraining2Enable( Bool bEnable , BYTE ComboPort);
 extern void mapi_DPCombo_ISR(void);
BOOL DPRxComCheckSQTime(WORD Times, BYTE  ComboPort);
Bool DP_ComGetSquelch( DWORD Times, BYTE  ComboPort);
void DPRxComClrSQTime(BYTE  ComboPort);
void DPRxComboCheckInputFormat(BYTE ComboPort);
BYTE DPRxComPacketColor( BYTE ComboPort );
BYTE  DPRxComPacketRange(BYTE ComboPort);
BYTE  DPRxComColorDepth(BYTE ComboPort);
BYTE DPRxComPacketType(BYTE ComboPort);
extern void mapi_DPCom_TimerInterrupt(void);
BOOL DPComFastLink(BYTE ComboPort);
void DP_ComSetSyntheizer( E_DP_Rate bRate,  BYTE  ComboPort);
void DPRxComboAudioHandle(BYTE ComboPort);
void DPComboAudioMuteCheck(BYTE ComboPort);
void DPRxComboCDRLossHander( BYTE ComboPort );
void DPRxComboHdcpReAuthen( BYTE ComboPort );
void DPComRxHPDEnable(Bool bEnable,  BYTE ComboPort );
BYTE DPRxComPacketType(BYTE ComboPort );
BOOL DPRxComCheckTiming(BYTE ComboPort, BYTE Count );
void DPRxComboHVFSMCheck(BYTE InputPort);
BYTE mapi_DP_ComboGetRangeFormat(BYTE InputPort);
BYTE mapi_DP_ComGetColorSpace(BYTE InputPort);
#if LOOPOUT_DPNOSIGNAL_SOLUTION
void DPRxComReChkTiming(BYTE ComboPort);	// 140528 DMD Loop Out에서 간헐적 화면 안나옴 개선을 위한 Spot 솔루션 적용
#endif
#endif
#endif
#endif
