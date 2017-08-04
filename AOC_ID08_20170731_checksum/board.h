#ifndef _BOARD_H


    #define _BOARD_H
    //****************************************************************
    #ifndef ModelName
    #define  ModelName      AOC_Q17//Debug Mode need define
    #endif
    //********************LogoType Define*******************************
    #define AOCLOGO                 			1
    #define EnvisionLOGO                		2
    #define StaplesLOGO				3
////
    #define PuntaLOGO					4//  yi qing ji tuan
    #define WearnesLOGO				5//  
    #define AcerLOGO				6//  
    
    #define NoBrand					255
    //*******************ModelName Define*******************************
    #define AOC_917Fwx				            1
    #define AOC_917Fwh				            2
    #define AOC_919P2_Asia			            3
    #define AOC_Q17W					        4
    #define AOC_Q156W				            5
    #define AOC_2564                          	6
    #define AOC_731FW				            7
    #define AOC_Q22W					        8
    #define AOC_919Pwz_Asia			            9
    #define AOC_919Vwa_Asia			           10
    #define AOC_Q17					           11
    #define AOC_2230FH                         12
    #define AOC_Q19					           13
    #define AOC_931Fwz				           14
    #define AOC_2218FH                         15
    #define AOC_2219P2				           16//Amy2007.12.26   18:36:02
    #define ENVISION_G2217                     17
    #define ENVISION_G2219                     18
    #define AOC_930FWz                   	   19
    #define ENVISION_H716Wx			           20
    #define AOC_2217V					       21
    #define AOC_2217PWC				           22
    #define AOC_2230F					       23
    #define AOC_919P2_Europe			       24
    #define AOC_919Pwz_Europe		           25
    #define AOC_919Vwa_Europe		           26
    #define AOC_2216VW				           27
    #define AOC_831S_Plus				       28
    #define AOC_917Sw				           29
    #define AOC_F19					           30
    #define AOC_919Va2				           31
    #define AOC_2219Vwg				           32
    #define AOC_F22					           33
    #define AOC_919V					       34
    #define ENVISION_H819W			           35
    #define ENVISION_P851				       36
    #define ENVISION_P2151			           37
    #define AOC_2219Vg				           38
    #define AOC_2230Fa				           39
    #define AOC_2330V					       40
    #define ENVISION_G2460			           41
    #define ENVISION_G2219_LM220WE1            42
    #define AOC_2430V					       43
    #define AOC_2230F_LTM220M1L01	           44
    #define AOC_2219P2_2883			           45
    #define ENVISION_G2218wg			       46
    #define AOC_2037F                          47
    #define AOC_2237Fw                         48
    #define AOC_2237Fwe                        49
    #define AOC_E2437Fh                        50 // e means WLED
    #define AOC_G2261w                         51
    #define AOC_2238Vh                         52
    #define ENVISION_P2271w                    53
    #define ENVISION_P2471w                    54
    #define AOC_2219Vwg5                       55
    #define AOC_e2237Fw                        56 // e means WLED
    #define AOC_e2437Fw                        57
	#define AOC_e2037F						   58
    #define ENVISION_P971wl                    59
	#define ENVISION_P2071l 				   60
	#define AOC_e937Fw						   61
    #define ENVISION_P2271wl                   62
    #define AOC_2330V_PLUS                     63
    #define AOC_E2237Fwh                       64
    #define AOC_1620Swa				           65
    #define AOC_2219P2_3329                    66
    #define AOC_2330V_14L                      67
    #define ENVISION_G2470                     68
    #define AOC_e1621Swb                       69
    #define AOC_e2437F                         70
    #define AOC_2437F                          71
    #define ENVISION_P971l                     72
    #define ENVISION_P2471                     73
    #define ENVISION_G2770                     74
    #define ENVISION_G2360g                    75
    #define ENVISION_P2371l                    76
    #define AOC_919Vz					       77
    #define AOC_919Sz					       78
    #define AOC_e2237Fw2                       79
    #define AOC_e2437F2                        80
    #define ENVISION_G2218			           81
    #define AOC_931Fwa                         82
    #define ENVISOIN_P2273wl                   83
	#define ENVISION_P2071l1	               84
	#define AOC_e2037F1                        85
	#define ENVISION_G2218WG1                  86
	#define AOC_919Vz1                         87
	#define AOC_919Sz1                         88
	#define ENVISION_P2271wl1                  89
	#define AOC_1620Sw                         90
	#define ENVISION_P2371l1                   91
	#define AOC_E2237Fwh1                      92
	#define ENVISION_P971wl1                   93
    #define ENVISION_P971wal1                  94       
    #define AOC_e2795v                  95
    #define ENVISOIN_P2473wl                       96//101227 add by win
    #define AOC_931Fwa_TMS190                   97
    #define AOC_e1620Swb                               98
    #define AOC_1620Sw_TPM156B1L01        99
    #define ENVISION_G2460W                        100
    #define AOC_e1621Swb_                             101
    #define ENVISION_G2770L                         102
    #define AOC_1620Sw_T                              103
    #define ENVISOIN_P2271WS                      104
    #define AOC_e1620Swb_1                          105
	#define ENVISION_P2071L_T 				   		106		//110617 Modify
	#define ENVISION_G2218_PLUS			            107
	#define ENVISION_P971wal                 108   //alen 111108
	#define AOC_2219P2_3834                  109
	#define ENVISION_P2371l2                 110   //Peter 111123
	#define ENVISION_P971waL2                111   //hong  add 20120201
	#define ENVISION_H2276DL                 112   //Peter 120115
	#define AOC_E2795V_AUOM270HW01V2         113   //Peter
    #define AOC_e1620Swb_2                   114    //Peter 120409 TSUMXXT
    #define ENVISION_G2770L_L20              115   //Peter 120419
    #define ENVISOIN_P2473wl_L02             116   //Peter 120425
    #define AOC_G2770L_AUOM270               117
    #define ENV_P971WALS_TPM185              118    //Peter 120601
    #define ENVISOIN_P2473wl_TPM236          119    //Peter 120620
    #define ENV_P971WLS_TPM185_NO_AUDIO      120    //Peter 120712
    #define ENVISOIN_P2273wlTPM              121
    #define AOC_E2795V_L20                   122    //bian 120810
    #define ENVISION_H2083LS			123//petit 20121030
    #define ENVISION_H2083WL			124//petit 20121030
    #define ENVISION_H2473DL			125//petit 20121204 update
    #define ENVISION_P971waL_CMI		126//petit 20121228
    #define ENVISION_P971WALS			127//20130322
    #define ENVISION_H2030WS			128//Gino 20140522
    #define ENVISION_H2083WM			129//Alpha 20150417
	#define ENVISION_JRY1PTRL                 130  // For eDP ------1366x768   lixw 20160323
    #define ENVISION_R2218WS                131//lixw 20151119
    #define ENVISION_R2018WS                 132//lixw 20151119
    #define ENVISION_JRYL1PTR_EV1         133//lixw 20160329  yiqing  for eDP

	 //*****************************************************************
    //*******************PanelType Define********************************
    //Wide Panel
    #define PanelCMO190W				1  //19wide
    #define PanelHSD170MGW1			    3  //17w
    #define PanelCMOL201L01			    4
    #define PanelCMOM220Z1_L03		    5
    #define PanelLTM190M2				6
    #define PanelCLAA201WA04			7
    #define PanelM190A1L02			    8
    #define PanelCLAA220WA01			9
    #define PanelLTM220M1L01			10
    #define PanelCLAA170WA02			12
    #define PanelM190Z1L01			    13
    #define PanelM156B1L01			    14
    #define PanelCMOM220Z1_PS3		    16//Amy2007.12.26   18:36:04
    #define PanelCMO220Z1M01			17
    #define PanelCLAA220WA02			18
    #define PanelCLAA185WA01			19
    #define PanelSVA156WX1			    20
    #define PanelM185B1L01			    21
    #define PanelM216H1L01			    23
    #define PanelHT190WG1			    24//BOE 19Wide
    #define PanelLM185WH1TLA1		    25
    #define PanelLTM230HT01			    26
    #define PanelCMOM236H1L01		    27
    #define PanelLM220WE1			    28
    #define PanelAUOM240HW01V0		    29
    #define PanelAUOM185XW01		    30
    #define PanelLM230WF1TLA3		    31
    #define PanelTPM215HW01			    32// RSDS 1920x1080
    #define PanelCMOM200O1L02           33 // 1600x900
    #define PanelAUOM215HW01L07         34 // 1920x1080
    #define PanelAUOM215HW01V7          35 // 1920x1080
    #define PanelAUOM240HW01V6          36 // 1920x1080 LED type
    #define PanelTPM215HW01H1PS1        37 // 1920x1080 mini-LVDS 6-pair 6-bit
    #define PanelCMOM215H1L01		    38 // 1929x1080 LVDS 
    #define PanelCMOM236H1L07		    39 // 1929x1080 LVDS 
    #define PanelAUOM215HW01V6          40 // 1920x1080 WLED
    #define PanelTPM236H3L02            41
    #define PanelTPM200O1L02			42
	#define PanelM185XW01				43
    #define PanelTPM215H1L01            44// 1920x1080 LED type	
    #define PanelLM230WF1TLD1           45
    #define PanelAUOM156XW01V0          46//1366x768
    #define PanelAUOM240HW01V7          47
	#define PanelAUOB156XW02V6          48//1366x768    NOTEBOOK PANEL
    #define PanelTPM185B1XW01           49//1366X768 led panel
    #define PanelLM200WD3TLA1           50//1600x900
    #define PanelAUOM270HW01V0          51//1920x1080 
    #define PanelTPM200O1L02C1D         52//1600x900
    #define PanelLM230WF5TLA1           53//1920x1080
    #define PanelHSD190MEN4A02          54// 1280x1024  4:3 panel
    #define PanelAUOM240HW01VB          55//1920x1080 same as PanelAUOM240HW01V6 except version
    #define PanelTPM190A1               56
    #define PanelAUOM215HW01VB          57 // 1920x1080 WLED
    #define PanelAUOM215HW01            58 // 1920x1080 WLED
	#define PanelLM200WD3TLC4           59// 1920x1080 
	#define PanelLM200WD3               60// 1920x1080
	#define PanelLM215WF1               61//1920x1080
	#define PanelLM215WF4               62 // 1920x1080
	#define PanelTPM156B1               63
	#define PanelLM230WF5TLD1           64//1920x1080
    #define  PanelM215HW01              65//it lost.
	#define PanelM185B3LA1              67
    #define PanelCMOM270HGEL10          68
    #define PanelM236H3LA3              69 // 1920x1080 WLED
    #define PanelTMS190                 70//  1440x900
    #define PanelCLAA156WB11A	        71//1366 768
    #define PanelTPM156B1L01            72//1366 768
    #define PanelCMOM236H1L01_1         73//1920x1080
    #define PanelM236MWF1               74//1920x1080
    #define PanelTPM156B1WX102TC        75//1366 768
    #define PanelCLAA215FA04            76// 1920x1080
    #define PanelAUOM215HW01VC          77 // 1920x1080 
    #define PanelCMON156B6L0B	        78//1366 768 LED
	#define PanelLTM200KT01 			79		//110617 Modify
   	#define PanelBOEHT156WXB 		    80		//1366 768 LED
   	#define PanelLTM220MT05             81
    #define PanelLTM185AT05             82
	#define PanelTPM220Z1               83
	#define PanelLTM220MT09             84
	#define PanelLM230WF5TLF1           85
	#define PanelCMIM185BGEL22          86 // 1920x1080
	#define PanelTPM220Z1_ZGEL01        87
    #define PanelAUOM270HW01V2          88
    #define PanelLTN156AT24_P           89
    #define PanelLM270HGEL20            90
    #define PanelHB156WX1_100           91
    #define PanelB156XTN02              92
    #define PanelM236HGEL20             93
    #define PanelTPM185B1_WX3400        94    //Peter.Yu 2012.06.01
    #define PanelTPM236WU110            95    //2012.06.20
    #define PanelTPM215HW01_HGEL02      96
   #define  PanelCMIM195FEG_L20			97//petit 20121030
   #define PanelLNTBM200WD3_T2BB		98//petit 20121031
     #define PanelAUOM240HW01VD		    99//petit 20121101
     #define PanelCMIM185BGEL22_new	100//petit 20121228
    #define PanelTPM240WF1_HTN01			101//20130327
    #define PanelTPM240WF1_HW01             102//20130625
    #define PanelTPM195WD1                        103//Gino 20140522
    #define PanelM200HJJL20					  104//Alpha 20150417
    #define PanelHSM215EU_HTN01				105//20151119 lixw
    #define PanelHSM195EU_RTN01				106//20151119 lixw
    #define PanelAUOM215HW03            		107 // 1920x1080 
	//
    #define PanelLGLP156WHB_DP             		108//1366x768 eDP lixw  20160407
    #define PanelINNN173FGE_E23_DP             	109//1600x900 eDP lixw  20160408
    #define PanelB156XTN07             	110//1366*768
    #define PanelNT156WHM_N12		111//1366*768
    #define PanelM185BGE_L23		112//1366*768
    #define PanelM185WHB_N10		113//1366*768
    #define PanelLM185TT3A			114
    
	//Normal Panel
    #define PanelCMO190				    2
    #define PanelHSD190MEN3_A00		    11
    #define PanelCLAA170EA07P 		    15
    #define PanelLM190E08 			    22
    
   
     //*****************************************************************
    //*******************MainBoardType Define****************************
    #define Other						0
    #define MainBoard_715G2575		1
    #define MainBoard_2578			2
    #define MainBoard_2573			3//HDCP Key In Flash
    #define MainBoard_2069			4//No used
    #define MainBoard_1558			5//No used
    #define MainBoard_2701			6
    #define MainBoard_2186_2A		7//HDCP Key In EEPRom
    #define MainBoard_2186_1			8//HDCP Key In EEPRom
    #define MainBoard_2698_2_X_X_2	9//TSUMU58BWHJ
    #define MainBoard_2698_2_X_X_8	10//TSUM56AWHL
    #define MainBoard_2810_B			11
    #define MainBoard_2883_B_X_X_2	12//TSUMO5PCWHL,TSUMO58CWHL
    #define MainBoard_715G2564_B		13
     #define MainBoard_715G2904_B    	14
    #define MainBoard_715G2808		15
    #define MainBoard_2883_B_X_X_1	16//TSUM5PFHL
    #define MainBoard_715G2988		17
    #define MainBoard_2670_C                18//TSUMU5RBWHQ-LF//Amy2007.12.26   18:37:10
    #define MainBoard_2670_1                19//TSUMU5RBWHQ-LF PQFP-128
    #define MainBoard_2670_B                20//Feature:Reduce DDC&LED,Scaler TSUMU5REHQ   Note:It was updated to RDG2670_C_4_X_1_080611
    #define RDG2670_C_4_X_1_080611	21//Feature:Reduce DDC&LED,Scaler TSUMU5REHQ
    #define MainBoard_2883_1B		22//TSUM58EHL
    #define MainBoard_3289_A2		23// TSUM5REHQ(RSDS) with dc-dc converter (VGH) 
    #define MainBoard_715G3329_A2     24 // 1A1D TSUMU58EHL/TSUMU58NHL/TSUMO58GHL TSUMU58PWHL
    #define MainBoard_715G3598_M0A   25 // 1A1D TSUMU58EHL-1 with touch key(Cypress) and light sensor
    #define MainBoard_715G3603_M0A   26 // 1A1D1H TSUMU88MDT3-1 / TSUMO88QWDT3-1 with touch key(Cypress) and light sensor
    #define MainBoard_715G3289_C2      27 // 1A1D TSUMU5BEHQ-1(mini-LVDS)/TSUMU5REHQ-1(RSDS) with dc-dc converter(VGH)
    #define MainBoard_715G3663_M0A   28 // 1A1H TSUMU58MDT3-1
    #define MainBoard_712G3244_2        29// 1A   TSUM1PLR-LF
	#define MainBoard_715G3244_TSUMU18NR	30//
	#define MainBoard_715G3598_1PLL			31//
	#define MainBoard_715G4175_M0A              32//TSUM1PNWR-LF LQPF-64
	#define MainBoard_715G3329_1_2           33// TSUMO58QWHL-LF
	#define MainBoard_715G3329_58N			34 //TUSMU58NWHL-LF
	#define MainBoard_715G3329_18N			35 //TUSMU18NWHL-LF
	#define MainBoard_4432                 	36 // N-type TSUMU58NWHL5-LF-1
	#define MainBoard_715G3329_1_2A         37// TSUMO58QWHL-LF//101023
	#define MainBoard_715G3529              38
	#define MainBoard_715G3529_T			39	//TSUMU18TR-LF //110311 Modify
    #define MainBoard_715G4734             	40    //Tsumu18pwr-LF
	#define MainBoard_715G3959_T			41	//TSUMU18TR-LF //110617 Modify
    #define MainBoard_715G3834              42
    #define MainBoard_715G5265_T		43//TSUMU18TR-LF   petit 20121029
    #define MainBoard_3834_18T		44//TSUMU18TR-LF   petit 20121228
    #define MainBoard_715G6952_M0B		45//TSUMU19BR9   Gino 20140522
    #define MainBoard_715G6912_M1A		46//TSUMU19BR9	 Alpha 20150417

	#define MainBoard_SLB1PTR               47//TSUM1PTR-LF
	#define MainBoard_JRY58NWHL               48
	
	#define MainBoard_JRYL1PTR_LF             49///1PTR-EV1

    #define INPUT_1A			0
    #define INPUT_1A1D		1
    #define INPUT_1A2D		2
    #define INPUT_2A2D		3
    #define INPUT_1A1H		4

    #define PANEL_17			0
    #define PANEL_17_WIDE	1
    #define PANEL_19			2
    #define PANEL_19_WIDE	3
    #define PANEL_20_WIDE	4
    #define PANEL_22_WIDE         		5

    //*****************************************************************
    #define Default_HDCP_Key	0
    #define Backupdata		0

    //*******************Language Define********************************
    #define ID2007Standard	0
    //English,Spanish,France,German,Italian,Portugues,Russia,SChina,
    #define ASIA				1
    //English,Spanish,France,Portugues,Russia,SChina,TChina,Korea,
    #define EUROPE			2
    //English,Spanish,France,German,Italian,Russia,Swedish,Polish,Czech,
    
	#define All13Language		3	//gy:090824:for 13 Language(Add ASIS and EUROPE Together)
	//English,German,France,Italian,Spanish,Swedish,Polish,Czech,Russia,Portugues,Korea,TChina,SChina
	
	#define All14Language_AddJap		4	
	//English,German,France,Italian,Spanish,Swedish,Polish,Czech,Russia,Portugues,Korea,TChina,SChina,Japan
    //*****************************************************************

	//*******************VCOM TYPE************************************
	
	#define VCOM_RICHTEK              		0
	#define VCOM_MAXIM              		1   //MAX9668
	

    //===============================================
	#if (ModelName==AOC_917Fwh)
		#include "AOC_917Fwh.h"
	#elif (ModelName==AOC_919P2_Asia)
		#include "AOC_919P2_Asia.h"
	#elif (ModelName==AOC_919P2_Europe)
		#include "AOC_919P2_Europe.h"
	#elif (ModelName==AOC_917Fwx)
		#include "AOC_917Fwx.h"
    #elif (ModelName ==AOC_2564)
              #include "AOC_2564.h"
	#elif (ModelName ==AOC_Q156W)
		#include "AOC_Q156W.h"
	#elif (ModelName ==AOC_Q17W)
              #include "AOC_Q17W.h"
	#elif (ModelName ==AOC_731FW)
              #include "AOC_731FW.h"
	#elif (ModelName == AOC_Q22W)
		#include "AOC_Q22W.h"
	#elif (ModelName == AOC_919Pwz_Asia)
		#include "AOC_919Pwz_Asia.h"
	#elif (ModelName == AOC_919Pwz_Europe)
		#include "AOC_919Pwz_Europe.h"
	#elif (ModelName == AOC_919Vwa_Asia)
		#include "AOC_919Vwa_Asia.h"
	#elif (ModelName == AOC_919Vwa_Europe)
		#include "AOC_919Vwa_Europe.h"
  	#elif ( ModelName  == AOC_Q17)
		#include "AOC_Q17.h"
  	#elif ( ModelName  == AOC_Q19)
		#include "AOC_Q19.h"
    #elif ( ModelName  == AOC_2230FH)
              #include "AOC_2230FH.h"
    #elif ( ModelName  == AOC_931Fwz)
              #include "AOC_931Fwz.h"			  
    #elif ( ModelName  == AOC_2218FH)
              #include "AOC_2218FH.h"
	#elif ( ModelName  == AOC_2219P2)//Amy2007.12.26   18:39:01
              #include "AOC_2219P2.h"
	#elif ( ModelName  == ENVISION_G2217)
              #include "ENVISION_G2217.h"
	#elif ( ModelName  == ENVISION_G2219 )
              #include "ENVISION_G2219.h"
	#elif ( ModelName  == AOC_930FWz )
              #include "AOC_930FWz.h"
	#elif ( ModelName  == ENVISION_H716Wx )
              #include "ENVISION_H716Wx.h"
	#elif ( ModelName  == AOC_2217V )
              #include "AOC_2217V.h"
	#elif ( ModelName  == AOC_2217PWC )
              #include "AOC_2217PWC.h"
	#elif ( ModelName  == AOC_2230F )
              #include "AOC_2230F.h"
	#elif ( ModelName  == AOC_2216VW )
              #include "AOC_2216VW+.h"
	#elif ( ModelName  == AOC_831S_Plus )
              #include "AOC_831S.h"
	#elif ( ModelName  == AOC_917Sw )
              #include "AOC_917Sw.h"
	#elif ( ModelName  == AOC_F19 )
              #include "AOC_F19.h"
	#elif ( ModelName  == AOC_919Va2 )
              #include "AOC_919Va2.h"
	#elif ( ModelName  == AOC_2219Vwg )
	          #include "AOC_2219Vwg.h"
	#elif ( ModelName  == AOC_F22 )
	          #include "AOC_F22.h"
	#elif ( ModelName  == AOC_919V )
	          #include "AOC_919V.h"
	#elif ( ModelName  == ENVISION_H819W )
	          #include "ENVISION_H819W.h"
	#elif ( ModelName  == ENVISION_P851 )
	          #include "ENVISION_P851.h"
	#elif ( ModelName  == ENVISION_P2151 )
	          #include "ENVISION_P2151.h"
	#elif ( ModelName  == AOC_2219Vg)
	          #include "AOC_2219Vg.h"
	#elif ( ModelName  == AOC_2230Fa )
              #include "AOC_2230Fa.h"
	#elif ( ModelName  == AOC_2330V )
              #include "AOC_2330V.h"
	#elif ( ModelName  == ENVISION_G2460)
              #include "ENVISION_G2460.h"
	#elif ( ModelName  == ENVISION_G2219_LM220WE1)
              #include "ENVISION_G2219_LM220WE1.h"	
	#elif ( ModelName  == AOC_2430V)
        	  #include "AOC_2430V.h"
	#elif ( ModelName  == AOC_2230F_LTM220M1L01)
              #include "AOC_2230F_LTM220M1L01.h"
	#elif ( ModelName  == AOC_2219P2_2883)
              #include "AOC_2219P2_2883.h"
	#elif ( ModelName  == ENVISION_G2218wg)
              #include "ENVISION_G2218wg.h"
	#elif ( ModelName  == AOC_2037F)
              #include "AOC_2037F.h"
	#elif ( ModelName  == AOC_2237Fw)
              #include "AOC_2237Fw.h"
	#elif ( ModelName  == AOC_2237Fwe)
              #include "AOC_2237Fwe.h"
	#elif ( ModelName  == AOC_E2437Fh)
              #include "AOC_E2437Fh.h"
    #elif ( ModelName == AOC_G2261w)
              #include "AOC_G2261w.h"
    #elif ( ModelName == AOC_2238Vh)
              #include "AOC_2238Vh.h"
    #elif ( ModelName == ENVISION_P2271w)
              #include "ENVISION_P2271w.h"                            
    #elif ( ModelName == ENVISION_P2471w)
              #include "ENVISION_P2471w.h"
    #elif ( ModelName == AOC_2219Vwg5)
              #include "AOC_2219Vwg5.h"
    #elif ( ModelName  == AOC_e2237Fw)
              #include "AOC_e2237Fw.h"
    #elif ( ModelName  == AOC_e2437Fw)
              #include "AOC_e2437Fw.h"
	#elif(ModelName==AOC_e2037F)
			  #include "AOC_e2037F.h"
	#elif(ModelName==ENVISION_P971wl)
	       #include"ENVISION_P971wl.h"
	#elif ( ModelName == ENVISION_P2071l)
           #include "ENVISION_P2071l.h"
	#elif ( ModelName == AOC_e937Fw)
           #include "AOC_e937Fw.h"
	#elif ( ModelName == ENVISION_P2271wl)
           #include "ENVISION_P2271wl.h"  
	#elif (ModelName==AOC_2330V_PLUS)
	       #include "AOC_2330V_PLUS.h"  
    #elif (ModelName==AOC_E2237Fwh)
	       #include "AOC_E2237Fwh.h"
	#elif(ModelName==AOC_1620Swa)
	       #include "AOC_1620Swa.h"
	#elif(ModelName==AOC_2219P2_3329)
	       #include"AOC_2219P2_3329.h"
	#elif(ModelName==AOC_2219P2_3834)
	        #include"AOC_2219P2_3834.h"
	#elif(ModelName==AOC_2330V_14L)
	      #include"AOC_2330V_14L.h"
	#elif(ModelName==ENVISION_G2470)
	      #include"ENVISION_G2470.h"
	#elif(ModelName==AOC_e1621Swb)
	 #include"AOC_e1621Swb.h"
	#elif(ModelName==AOC_e2437F)
	 #include"AOC_e2437F.h"
	#elif(ModelName==AOC_2437F)
	 #include"AOC_2437F.h"
	#elif(ModelName==ENVISION_P971l)
	 #include"ENVISION_P971l.h"
	#elif(ModelName==ENVISION_P2471)
	 #include"ENVISION_P2471.h"
	#elif(ModelName==ENVISION_G2770)
	 #include"ENVISION_G2770.h"
	 #elif(ModelName==AOC_e2795v)
	 #include"AOC_e2795v.h"
     #elif(ModelName == AOC_E2795V_AUOM270HW01V2)
     #include"AOC_E2795V_AUOM270HW01V2.h"
	  #elif(ModelName==ENVISION_G2770L )
	 #include"ENVISION_G2770L .h"
    #elif(ModelName == ENVISION_G2770L_L20)
        #include"ENVISION_G2770L_L20.h"
	#elif(ModelName==ENVISION_G2360g)
	 #include "ENVISION_G2360g .h"
	#elif(ModelName==ENVISION_P2371l)
	 #include "ENVISION_P2371l .h" 
	#elif(ModelName==AOC_919Vz)
	 #include "AOC_919Vz.h"
	#elif(ModelName==AOC_919Sz)
	 #include "AOC_919Sz.h"
    #elif (ModelName==AOC_e2237Fw2)
        #include "AOC_e2237Fw2.h"
    #elif (ModelName==AOC_e2437F2)
        #include "AOC_e2437F2.h"
    #elif (ModelName==ENVISION_G2218)
        #include "ENVISION_G2218.h"
	#elif (ModelName==ENVISION_G2218_PLUS)
        #include "ENVISION_G2218_PLUS.h"
    #elif (ModelName==AOC_931Fwa)
        #include "AOC_931Fwa.h"
   #elif (ModelName==AOC_931Fwa_TMS190)
        #include "AOC_931Fwa_TMS190.h"
    #elif (ModelName==ENVISOIN_P2273wl)
        #include "ENVISOIN_P2273wl.h"
    #elif (ModelName==ENVISOIN_P2473wl)
        #include "ENVISOIN_P2473wl.h"
	#elif ( ModelName == ENVISION_P2071l1)
        #include "ENVISION_P2071l1.h"
	#elif(ModelName==AOC_e2037F1)
	    #include "AOC_e2037F1.h"
	#elif ( ModelName  == ENVISION_G2218WG1)
        #include " ENVISION_G2218WG1.h"
	#elif ( ModelName  == ENVISION_G2460W)
        #include " ENVISION_G2460W.h"
	#elif(ModelName==AOC_919Vz1)
	    #include "AOC_919Vz1.h"
	#elif(ModelName==AOC_919Sz1)
	    #include "AOC_919Sz1.h"
	#elif (ModelName==ENVISION_P2271wl1)
        #include "ENVISION_P2271wl1.h"
	#elif (ModelName==ENVISOIN_P2271WS)
        #include "ENVISION_P2271WS.h"
	#elif (ModelName==AOC_1620Sw)
        #include "AOC_1620Sw.h"
	#elif (ModelName==AOC_1620Sw_TPM156B1L01)
        #include "AOC_1620Sw_TPM156B1L01.h"
	#elif (ModelName==AOC_e1620Swb)
        #include "AOC_e1620Swb.h"
	#elif (ModelName==AOC_e1620Swb_1)
        #include "AOC_e1620Swb_1.h"
	#elif (ModelName==AOC_e1621Swb_)
        #include "AOC_e1621Swb_.h"
	#elif(ModelName==ENVISION_P2371l1)
	 #include "ENVISION_P2371l1.h"
	#elif (ModelName==AOC_E2237Fwh1)
	  #include "AOC_E2237Fwh1.h"
	#elif(ModelName==ENVISION_P971wl1)//101023
	 #include"ENVISION_P971wl1.h"
	#elif(ModelName==ENVISION_P971wal1)//101023
	 #include"ENVISION_P971wal1.h"
	#elif(ModelName==ENVISION_P971wal) //alen 111108
	 #include"ENVISION_P971wal.h"
	#elif(ModelName==AOC_1620Sw_T)		//110311 Modify
 		#include"AOC_1620Sw_T.h"
	#elif(ModelName==ENVISION_P2071L_T)		//110617 Modify
		#include"ENVISION_P2071L_T.h"
    #elif(ModelName == ENVISION_P2371l2)  //111123 Modify
        #include"ENVISION_P2371l2.h"
    #elif(ModelName == ENVISION_P971waL2)  //hong add  20120201
        #include"ENVISION_P971waL2.h"
	#elif(ModelName == ENVISION_H2276DL)
        #include"ENVISION_H2276DL.h"
    #elif(ModelName == AOC_e1620Swb_2)
        #include"AOC_e1620Swb_2.h"
    #elif(ModelName == ENVISOIN_P2473wl_L02)
        #include"ENVISOIN_P2473wl_L02.h"
    #elif(ModelName == AOC_G2770L_AUOM270)
        #include"AOC_G2770L_AUOM270.h"
    #elif(ModelName == ENV_P971WALS_TPM185)
        #include"ENV_P971WALS_TPM185.h"
    #elif(ModelName == ENVISOIN_P2473wl_TPM236)
        #include"ENVISOIN_P2473wl_TPM236.h"
    #elif(ModelName == ENV_P971WLS_TPM185_NO_AUDIO)
        #include"ENV_P971WLS_TPM185_NO_AUDIO.h"
    #elif (ModelName==ENVISOIN_P2273wlTPM)
        #include "ENVISOIN_P2273wlTPM.h"
    #elif(ModelName == AOC_E2795V_L20)
        #include"AOC_E2795V_L20.h"
   #elif(ModelName == ENVISION_H2083LS)//petit 20121030
        #include"ENVISION_H2083LS.h"
   #elif(ModelName == ENVISION_H2083WL)//petit 20121030
        #include"ENVISION_H2083WL.h"
   #elif(ModelName == ENVISION_H2473DL)//petit 20121204 update
        #include"ENVISION_H2473DL.h"
   #elif(ModelName == ENVISION_P971waL_CMI)//petit 20121228
        #include"ENVISION_P971waL_CMI.h"
   #elif(ModelName == ENVISION_P971WALS)////20130322
        #include"ENVISION_P971WALS.h"
   #elif(ModelName == ENVISION_H2030WS)//Gino 20140523
        #include"ENVISION_H2030WS.h"
   #elif(ModelName == ENVISION_H2083WM)//Alpha 20150417
	 #include"ENVISION_H2083WM.h"
	#elif (ModelName==ENVISION_JRY1PTRL)
		#include "ENVISION_JRY1PTRL.h"   
	   #elif (ModelName==ENVISION_R2218WS)
   	#include "ENVISION_R2218WS.h"
	   #elif (ModelName==ENVISION_R2018WS)
   	#include "ENVISION_R2018WS.h"
	  #elif (ModelName==ENVISION_JRYL1PTR_EV1)
   	#include "ENVISION_JRYL1PTR_EV1.h"
	
	#endif

    #ifdef AOC37Series
        #define AOC_37_Series   1
    #else
        #define AOC_37_Series   0
    #endif
    
    #ifndef BURST_MODE
	#define BURST_MODE 0
    #endif
    

    #if (ModelName  == AOC_2218FH)
        #define ScanKey 1 
        #define OSD_PIVOT_EN    1
    #elif AOC_37_Series ||\
            ModelName==ENVISION_P2471w ||ModelName==ENVISION_P2271w|| ModelName==AOC_G2261w||ModelName==ENVISION_P971wl ||\
            ModelName==ENVISION_P2071l||ModelName==ENVISION_P2271wl||ModelName==ENVISION_P971l||ModelName==ENVISION_P2471||\
            ModelName==ENVISION_P2371l||ModelName==ENVISION_P2071l1||ModelName==ENVISION_P2271wl1||ModelName==ENVISION_P971wl1||\
            ModelName==ENVISION_P971wal1||ModelName==ENVISOIN_P2271WS||ModelName==ENVISION_P2071L_T||ModelName==ENVISION_P971wal||\
            ModelName==ENVISION_P2371l2||ModelName==ENVISION_P971waL2 || (ModelName == ENV_P971WALS_TPM185) || (ModelName == ENV_P971WLS_TPM185_NO_AUDIO)||\
            ModelName==ENVISION_P971waL_CMI||ModelName==ENVISION_P971WALS//petit 20121228  //20130322
        #define ScanKey 1 
        #define OSD_PIVOT_EN    0
    #else
        #define ScanKey 0
        #define OSD_PIVOT_EN    0
    #endif
	

        #if ScanKey
        #define Cypress		0
        #define Synaptics	1
            #if ModelName == AOC_2218FH
            #define Scan_Source	Synaptics
            #else
            #define Scan_Source	Cypress
            #endif
        #endif

        #if AOC_37_Series
        #define Enable_Lightsensor  1
        #define LightSensorUse2Channel  0 // if use TSL2561, don't care this define
        #else
        #define Enable_Lightsensor  0
        #define LightSensorUse2Channel  0
        #endif

        #if Enable_Lightsensor
        #define TAOS		0
        #define EverLight	1
        #define LightSensor_Source      EverLight//TAOS // 090610
        #if LightSensor_Source==TAOS
        #define TSL2561     1 // 0: 2550, 1: 2561
        #endif
        #endif

        #ifdef Panel_RSDS
        #define EnableVcomAdjustItem    1
        #elif defined Panel_miniLVDS
        #define EnableVcomAdjustItem    1
        #else
        #define EnableVcomAdjustItem    0
        #endif

	#if defined (TSUMXXT)		//110311 Modify
	#define ADCRB_SWAP          0
	#endif
	
	#ifndef ENABLE_ADC_RESET	//120502 Modify
	#define ENABLE_ADC_RESET		1
	#endif
	
       #ifndef F_ResetCheckEdidManueAndSN		
       #define F_ResetCheckEdidManueAndSN			0
       #endif

	#define  WaitingForDrawOSDSUB	1 // add for MainMenu icon switch gabbige	081125

	#if MainBoardType==MainBoard_715G3603_M0A || MainBoardType==MainBoard_715G3663_M0A
	#define MS_DAC	1
	#else
	#define MS_DAC	0
	#endif

    //*****************************************************************
    #if 0//LANGUAGE_TYPE==ID2007Standard
        #define CompressPropFont		0
    #else
        #define CompressPropFont		1
    #endif


#define Youweipaike		1
#define EnableHotBrightness		0//(1&&Youweipaike)//HotBrightness


    //*****************************************************************
    #if DisplayLogo==NoBrand
        #define DisplayPowerOnLogo       	0
    #else
        #define DisplayPowerOnLogo       	1
    #endif
    //*****************************************************************
    //When Released Plesease Remember To Set DEBUG Flags To Be 0!!!!!!!!!
    //#define  DEBUG_EN 
    #ifdef DEBUG_EN
        #define DEBUG_ENABLE                     		1
        #message "Please remember to disable the debug function!"//Zhifeng.Wu Please don't canel it,just only a message to remind you to disable it ,thanks.
    #else
     #define DEBUG_ENABLE		 		0
	 #if DEBUG_ENABLE
	 #message "Please remember to disable the debug function!"
	 #endif
    #endif
		  #define DEBUG_ALLBANK			0
		  #define DEBUG_IOPORT			0
		  #define DEBUG_MSTMCU			0
		  #define DEBUG_FLASH			0
	         #define DEBUG_DETECT			0//DEBUG_ENABLE
                #define DEBUG_PRINTDATA		DEBUG_ENABLE
		  #define DEBUG_MSG				DEBUG_ENABLE

                #define DEBUG_INPUTPORT     	0//DEBUG_ENABLE
		  #define DEBUG_FINDMODE		DEBUG_ENABLE//
		  #define DEBUG_INPUTTIMING 	0//DEBUG_ENABLE
		  #define DEBUG_TIMEINGCHANGE	0
		  #define DEBUG_SAVEMODE		0
                #define DEBUG_CAPTUREWIN        0
		  #define DEBUG_DE				0
		  #define DEBUG_HDMI			0
		  #define DEBUG_OSD				0////DEBUG_ENABLE
		  #define DEBUG_FLASHADDR		0////DEBUG_ENABLE
    #define DDCCI_DEBUG_ENABLE			0
    #if ScanKey
                #define DEBUG_SCANKEY               0
                #define DEBUG_SLIDER                  0                    
    #endif
    #if Enable_Lightsensor
		  #define DEBUG_LightSensor		0
    #endif
    #ifdef MS_PM
                #define DEBUG_PM    0
    #endif

    //*****************************************************************
    #define Panel_N156BGE			1	//	群创
    #define Panel_B156xtno7		0	//	AUO
	
    #define Panel_N156BGE_NEW			1	//	群创
    
    #define Panel_NT156WHM_N12			0	//	BOE
    //HDMI


#if  ENABLE_HDMI
    #define G2_G3_HDMI_AUDIO_FADING		0///1
    #define FixDVIPort 						0
    #define ENABLE_CEC					0
    #define ENABLE_CEC_INT				0
    #define HDMIAUTODETECT                        	0
    #define HDMICOLORTEST				1
    #define ENABLE_CTSNSYNC				1
    #define CheckCTSStable                 0
    #define EN_HDMI_NEW_PIXEL_REPETITION    1	
    #define EN_HDMI_1_3                                 1
#else
    #define G2_G3_HDMI_AUDIO_FADING	0
    #define FixDVIPort						0
    #define ENABLE_CEC					0
    #define ENABLE_CEC_INT				0
    #define HDMIAUTODETECT                      	0
    #define HDMICOLORTEST				0
    #define ENABLE_CTSNSYNC				0	
    #define CheckCTSStable                  0
    #define EN_HDMI_NEW_PIXEL_REPETITION    0	
    #define EN_HDMI_1_3                                 0
#endif


	#if EnablePanelServiceMenu
	#define PANEL_WIDTH                 _PanelWidth
	#define PANEL_HEIGHT               	_PanelHeight
	#else
	#define PANEL_WIDTH                 PanelWidth
	#define PANEL_HEIGHT                PanelHeight
	#endif

    #define AOC_GAMMA	1
    #define ECO			1
    #define ChipTSU		1     //MST81*   #define ChipTSU 0
    #define ChipTSUM_G      1       // for Gaultier IC
    #define SRGB			1
    #define MySon		0       //  *** WARNING ***  Winbond define 0
    #define MST_MCU	1
    #define Encoder 	0
    #define Customer	0
    #define MST2Layer	0
    #define AOCKey		1
    #define Init_HDCPKey    1//0:internal HDCP key 090507 coding for test
    #if !Init_HDCPKey
        #message "Please remember to disable the default HDCP key!"//Zhifeng.Wu Please don't canel it,just only a message to remind you to disable it ,thanks.
    #endif
    #define Enable_YMC	0
    #define MWEFunction	1
    //#define Enable_Cache		     1
    //#define Enable_OsdEngine1      1// 1: two OSD engine,; 0: one osd engine
    #define DisplayInputInfo	1// 1: show input info ; 0:show energyStar logo
    #define DisplayEnergyStar   0
    #define EnableAutoDVI       0

     #define Enable_SubWin	!Enable_OsdEngine1// 1: yuhjinn test sub-win only
     #define Enable_GDColor      !Enable_OsdEngine1	 
     #define CenterTitleText	!Enable_OsdEngine1	 
     #define AutoProcess		1
     #define TestAutoSwitch  1	

    #if ModelName!=ENVISION_JRYL1PTR_EV1
    #define OffPower      //since 090304,add offtimer function
    #endif
   // #if ModelName==ENVISION_G2218WG1
    #define  DefaultIconExit     0// petit 20121228 for ID08 new spec request.
    #define  DefaultIconLumi     1//modify 20130102 for ID08 new spec request.
   // #else 
    //#define  DefaultIconExit     1 //since 091023,default Icon standy by exit,first model is 37 series
   // #endif
    
    #define EnableNewHotKey    //edit by lizzie.ding,100525  for ECO+DCR hotkey,if not define this,there is no DCR hotkey
     #ifdef EnableNewHotKey
        #define HotExpansionAdjust		Enable_Expansion
        #define UnifyHotDcrEco			HotExpansionAdjust
    #else
        #define HotExpansionAdjust  		0
        #define UnifyHotDcrEco		 	0
    #endif

    #if Dual
     #define HotInputSelect	 1
    #else
     #define HotInputSelect	 0
    #endif


    /*#if USE_EON_FLASH
    #define SR_ProtectValue     0x94
    #else
    #define SR_ProtectValue     0x8c
    #endif*/

    #define DATA_LENGTH_20_BYTE//For HDCP Tool Send 20Byte Data One Block	 
    #define Enable_ProductModeAdjust		0//Zhifeng.Wu Design For Manufacture
    #define NewFactoryCommand			1
    #if 1//Dual   for PE request,all mode have to support Singal port write EDID,100701
	//#define UseVGACableReadWriteAllPortsEDID  // edit by lizzie.ding  100525 , for dual model have support  Singal port write EDID
    #endif

#if ((PanelType!=PanelLGLP156WHB_DP)&&(PanelType!=PanelB156XTN02)&&(PanelType!=PanelB156XTN07)&&(PanelType!=PanelNT156WHM_N12)&&(PanelType!=PanelM185BGE_L23)&&(PanelType!=PanelM185WHB_N10)&&(PanelType!=PanelLM185TT3A))
	#define PanelSSCFunction    //2011.9.7   13:13:27 it is effect EnablePanelServiceMenu 
#endif

	#if 1// 2011.8.12   11:40:26 AOC require
    #define ESaverPowerDownFunc          1
	#else
    #define ESaverPowerDownFunc          0
	#endif
	
#ifndef AdjustFocusOSDValueBy1Step
#define  AdjustFocusOSDValueBy1Step  1//20150831   lixw
#endif

#ifndef IT6251
#define IT6251      0//20160323  lixw
#endif
#ifndef AUTO_COLOR_ITEM_EN// Auto color item in menu enable define; 	
#define AUTO_COLOR_ITEM_EN	0//ColorTempMenuAddAutoColor 
#endif
#ifndef DISP_DRC_ITEM_EN
#define DISP_DRC_ITEM_EN     0//LuminanceMenuDeleteDCR  // Enable DCR item display in menu;
#endif
#ifndef AOC_MENU_FRAME_MODE// 菜单显示样式定义;
#define AOC_MENU_FRAME_MODE		0	// 0: 菜单上/ 下有两条边; // 1: 菜单下无边;
#endif
#ifndef ALL_RES_SCALE_FUNC_EN// Enable 4:3/16:9 scale function at all resolution;
#define	ALL_RES_SCALE_FUNC_EN	0	// 1: All resolution can be scaled to 4:3 at wide panel;
#endif
#ifndef OSD_LOCK_FUNCTION_EN// Enable osd lock function;
#define	OSD_LOCK_FUNCTION_EN		0	// 1:Disable  0:enable
#endif
#ifndef PICTUREBOOST_FUN_EN// Enable picture boost  function;
#define	PICTUREBOOST_FUN_EN		0	// 1:Disable  0:enable
#endif
#ifndef PANELSSCSTEPLFUN
#define PANELSSCSTEPLFUN                   0
#endif

    //*****************************************************************
    #define ExtraOsd_offset 	(2*(DDCCI_ENABLE+Enable_Expansion))
    #define OSD_MOVE_BY_PIXL    1 // 100604 coding addition

    #if MST_MCU
        #define DirectBus	1
        #define DDRBus		0
    #else
        #define DirectBus	0
        #define DDRBus		1
    #endif
	#include "reg52.h"
    #if (MainBoardType==MainBoard_2573)
        #include "bd_MSTMCU_2573.h"
        #define EEPRom_24C04
	    #define USEFLASH		0
    #elif (MainBoardType==MainBoard_715G2575)
        #include "bd_MSTMCU_2575.h"
        #define EEPRom_24C04
	    #define USEFLASH		0
    #elif (MainBoardType==MainBoard_2578)
        #include "bd_MSTMCU_2578.h"
        #define EEPRom_24C04
	    #define USEFLASH		0
    #elif (MainBoardType==MainBoard_2701)
        #include "bd_MSTMCU_2701.h"
        //Use 24C16 EEPRom
	    #define USEFLASH		0
    #elif (MainBoardType==MainBoard_2186_2A)
        #include "bd_MSTMCU_2186_2A.h"
        //Use 24C16 EEPRom
        #define AOC_TSUM58BGJ	1
	    #define USEFLASH		0
    #elif (MainBoardType==MainBoard_2186_1)
        #include "bd_MSTMCU_2186_1.h"
        //Use 24C16 EEPRom
	    #define USEFLASH		0
    #elif (MainBoardType==MainBoard_2698_2_X_X_2)
        #include "bd_MSTMCU_2698_2_X_X_2.h"
        #define EEPRom_24C04
	    #define USEFLASH		0
	    #define Flash_256K
    #elif (MainBoardType==MainBoard_2698_2_X_X_8)
      #include "bd_MSTMCU_2698_2_X_X_8.h"
      #define EEPRom_24C04
	  #define USEFLASH		0
   #elif (MainBoardType==MainBoard_2883_B_X_X_1)
	  #include "bd_MSTMCU_2883_B_X_X_1.h"
	  #define EEPRom_24C04
	  #define USEFLASH		0
	  #define Flash_256K
   #elif (MainBoardType==MainBoard_2883_B_X_X_2)
	   #include "MainBoard_2883_B_X_X_2.h"
	   #define EEPRom_24C04
	   #define USEFLASH		0
	   #define Flash_256K
    #elif (MainBoardType==MainBoard_2810_B)
        #ifdef BGJ_TO_5PWGJ
        #include "bd_MSTMCU_2810.h"
        #else
        #include "bd_MSTMCU_2810_B.h"
        #endif
        #define EEPRom_24C04
	    #define USEFLASH		0
	    #define Flash_256K
    #elif (MainBoardType==MainBoard_715G2564_B)
        #include "bd_MSTMCU_2564_B.h"
        //#define EEPRom_24C04
	    #define Flash_256K
	    #define USEFLASH		1
    #elif (MainBoardType==MainBoard_715G2904_B)
        #include "bd_MSTMCU_2904_B.h"
	//#define EEPRom_24C04
	    #define Flash_256K
	    #define USEFLASH		1
   #elif (MainBoardType==MainBoard_715G4734)
        #include "bd_MSTMCU_4734.h"
	//#define EEPRom_24C04
	    #define Flash_256K
	    #define USEFLASH		1
    #elif (MainBoardType==MainBoard_715G2808)
        #include "bd_MSTMCU_2808.h"
        #define EEPRom_24C04
	    #define USEFLASH		0
    #elif (MainBoardType==MainBoard_715G2988)
        #include "bd_MSTMCU_2988.h"
        #define EEPRom_24C04
	    #define USEFLASH		0
   #elif (MainBoardType==MainBoard_2670_C )//Amy2007.12.26   18:40:50
        #include "bd_MSTMCU_2670_C.h"
        //#define EEPRom_24C16
	    #define USEFLASH		0
	    #define Flash_256K
   #elif (MainBoardType==MainBoard_2670_1 )
        #include "bd_MSTMCU_2670_1.h"
        #define EEPRom_24C04
	    #define USEFLASH		0
	    #define Flash_256K
   #elif (MainBoardType==MainBoard_2670_B )
        #include "bd_MSTMCU_2670_B.h"
	    #define USEFLASH		1
	    #define Flash_256K
   #elif (MainBoardType==RDG2670_C_4_X_1_080611)
       #include "RDG2670-C-4-X-1-080611.h"
	   #define USEFLASH		1
	   #define Flash_256K
   #elif (MainBoardType==MainBoard_2883_1B)
	   #include "MainBoard_2883_1B.h"
	   #define EEPRom_24C04
	   #define USEFLASH		0
	   #define Flash_256K
   #elif (MainBoardType==MainBoard_3289_A2)
        #include "MainBoard_3289_A2.h"
        #define USEFLASH	1
	    #define Flash_256K
   #elif (MainBoardType==MainBoard_715G3329_A2)
        #include "bd_MSTMCU_3329_A2.h"
	    #define USEFLASH		1
	    #define Flash_256K
   #elif (MainBoardType==MainBoard_715G3598_M0A)
        #include "bd_MSTMCU_3598_M0A.h"
	    #define USEFLASH		1
	    #define Flash_256K
   #elif (MainBoardType==MainBoard_715G3603_M0A)
        #include "bd_MSTMCU_3603_M0A.h"
	    #define USEFLASH		1
	    #define Flash_256K
	    #define   TSUMW88MDT3_For_HDMIDDCCI
	 
    #elif (MainBoardType==MainBoard_715G3289_C2)   
        #include "bd_MSTMCU_3289_C2.h"
	    #define USEFLASH		1
	    #define Flash_256K
   #elif (MainBoardType==MainBoard_715G3663_M0A)
        #include "bd_MSTMCU_3663_M0A.h"
	    #define USEFLASH		1
	    #define Flash_256K
    #elif (MainBoardType==MainBoard_712G3244_2)
	    #include "bd_MSTMCU_3244_2.h"
	    #define USEFLASH		1
	    #define Flash_256K
	#elif (MainBoardType == MainBoard_715G3244_TSUMU18NR)
        #include "bd_MSTMCU_3244_TSUMU18NR.h"
		#define USEFLASH		1
	 	#define Flash_256K
	#elif (MainBoardType==MainBoard_715G3598_1PLL)
        #include "bd_MSTMCU_3598_1PLL.h"
	    #define USEFLASH		1
	    #define Flash_256K
	#elif(MainBoardType==MainBoard_715G4175_M0A)
        #include "bd_MSTMCU_4175_M0A.h"
	    #define USEFLASH		1
	    #define Flash_256K	 
	#elif(MainBoardType==MainBoard_715G3329_1_2)
        #include "bd_MSTMCU_3329_1_2.h"
	    #define USEFLASH		1
	    #define Flash_256K	 
	#elif(MainBoardType==MainBoard_715G3329_58N)
	    #include "bd_MSTMCU_3329_58N.h"
	    #define USEFLASH	   1
	    #define Flash_256K	
	#elif(MainBoardType==MainBoard_715G3329_18N)
   		#include "bd_MSTMCU_3329_18N.h"
	    #define USEFLASH	   1
	    #define Flash_256K
	#elif (MainBoardType==MainBoard_4432)//add by win 101204
       #include "bd_MSTMCU_4432.h"
       #define USEFLASH    1
    //#define EEPRom_24C04
	#elif(MainBoardType==MainBoard_715G3329_1_2A)
        #include "bd_MSTMCU_3329_1_2A.h"
	    #define USEFLASH		1
	    #define Flash_256K	
	#elif(MainBoardType==MainBoard_715G3529)
        #include "bd_MSTMCU_3529.h" 
		#define USEFLASH		1
	    #define Flash_256K	 
	#elif(MainBoardType==MainBoard_715G3529_T)	//110311 Modify
		#include "bd_MSTMCU_3529_T.h" 
		#define USEFLASH		1
		#define Flash_256K	 
	#elif(MainBoardType==MainBoard_715G3959_T)		//110617 Modify
		#include "bd_MSTMCU_3959_T.h" 
		#define USEFLASH		1
		#define Flash_256K	 
	#elif (MainBoardType==MainBoard_715G3834)
        #include "bd_MSTMCU_3834.h"
	//#define EEPRom_24C04
	    #define Flash_256K
	    #define USEFLASH		1
    #elif (MainBoardType==Other)
        #include "bd_MSTMCU.h"
        //Use 24C16 EEPRom
	    #define USEFLASH		0
        #elif (MainBoardType==MainBoard_715G5265_T)//petit 20121030
        #include "bd_MSTMCU_5265_T.h"
	    #define Flash_256K
	    #define USEFLASH		1
	#elif (MainBoardType == MainBoard_SLB1PTR)
	        #include "bd_TSUM1PTRL_T.h"
	    #define Flash_256K
	    #define USEFLASH		1
	#elif (MainBoardType == MainBoard_JRYL1PTR_LF)
	        #include "bd_TSUM1PTRL_LF.h"
	    #define Flash_256K
	    #define USEFLASH		1
    	#elif (MainBoardType==MainBoard_715G6952_M0B)//Gino 20140522
        #include "bd_MSTMCU_6952_MOB.h"
	    #define Flash_256K
	    #define USEFLASH		1
        #elif (MainBoardType==MainBoard_3834_18T)//petit 20121228
        #include "MainBoard_3834_18T.h"
	    #define Flash_256K
	    #define USEFLASH		1
    	#elif (MainBoardType==MainBoard_715G6912_M1A)//Gino 20140522
        #include "bd_MSTMCU_6912_M1A.h"
	    #define Flash_256K
	    #define USEFLASH		1
		#endif
		
   #if !PANELSSCSTEPLFUN
   #define LPLL_STEP_NUM		0x00
   #define LPLL_SPAN_NUM		0x00
   #endif
	
    #ifdef ReduceDDC
	#define DetectInterval               2 // unit: ms
	#define SystemShutDownCntr     2 // unit: DetectInterval*mS
    #endif	
    #define ChipNone	0	// None A/NoneB
    #define ChipAC		1	// Buonarro arA
    #define ChipAD		2	// A
    #define ChipBA		4	// Giotto
    #define ChipBB		5
    #define ChipBC		6
    #define ChipID		ChipBC
    #define INT_PERIOD	1// timer interrupt: unit = 1ms
    //#define BaudRatePeriod	(0x10000 - (((unsigned long)CPU_CLOCK_MHZ * 1000000)/((unsigned long) SERIAL_BAUD_RATE / 10 * 32) + 5) / 10)
    #define SystemTick		1000	// 1 second
    #define UseINT			1
    #define UsesRGB		0
    #define ClockPeriod	(0x10000-(((unsigned long)CPU_CLOCK_KHZ*INT_PERIOD)/12))	// Interrupt every 1ms for Time 0
    #define BaudRatePeriod	(0x10000-(((unsigned long)CPU_CLOCK_KHZ * 1000+(DWORD)SERIAL_BAUD_RATE*16)/((DWORD)SERIAL_BAUD_RATE*32)))
    #define T1BaudRatePeriod	(0x100-(((unsigned long)CPU_CLOCK_KHZ * 1000+(DWORD)SERIAL_BAUD_RATE*192)/((DWORD)SERIAL_BAUD_RATE*384)))
    #define HFreq(hPeriod)	((DWORD)MST_CLOCK_MHZ*10+hPeriod/2)/hPeriod
    #define VFreq(hFreq, vTotal)	((DWORD)hFreq*1000+SrcVTotal/2)/SrcVTotal
    #define MST_HPeriod_UINT    MST_CLOCK_MHZ*16
    
#endif
