#include "Board.h"
#if !EnablePanelServiceMenu
	#if (PanelType==PanelCMOL201L01)
	    #include "PanelCMOL201_L01.h"  
	#elif (PanelType==PanelCMO190)
	    #include "PanelCMO190.h"
	#elif (PanelType==PanelCMO190W)
	    #include "PanelCMO190W.h"
	#elif (PanelType==PanelHSD170MGW1)
	    #include "PanelHSD170MGW1.h"
	#elif (PanelType==PanelCMOM220Z1_L03)
	    #include "PanelCMOM220Z1_L03.h"
	#elif (PanelType==PanelLTM190M2)
	    #include "PanelLTM190M2.h"
	#elif (PanelType==PanelCLAA201WA04)
	    #include "PanelCLAA201WA04.h"
	#elif (PanelType==PanelM190A1L02)
	    #include "PanelCMOM190A1L02.h"
	#elif (PanelType==PanelCLAA220WA01)
	    #include "PanelCLAA220WA01.h"
	#elif (PanelType==PanelCLAA220WA02)
	    #include "PanelCLAA220WA02.h"
	#elif (PanelType==PanelLTM220M1L01)
	    #include "PanelLTM220M1L01.h"
	#elif (PanelType==PanelHSD190MEN3_A00)
	    #include "PanelHSD190MEN3_A00.h"
	#elif (PanelType==PanelCLAA170WA02)
	    #include "PanelCLAA170WA02.h"
	#elif (PanelType==PanelM190Z1L01)
	    #include "PanelM190Z1L01.h"
	#elif ( PanelType  == PanelM156B1L01) 
	    #include "PanelM156B1L01.h"
	#elif ( PanelType == PanelHSD170MGW1 )
		#include "PanelHSD170MGW1.h"
	#elif ( PanelType 	== PanelCLAA170EA07P )
	      #include "PanelCLAA170EA07P.h"
	#elif ( PanelType 	== PanelCMOM220Z1_PS3 )//Amy2007.12.26   18:51:54
	      #include "PanelCMOM220Z1_PS3.h"
	#elif ( PanelType 	== PanelCMO220Z1M01 )//Amy2007.12.26   18:51:54
	      #include "PanelCMO220Z1M01.h"
	#elif ( PanelType 	== PanelCLAA185WA01 )
	      #include "PanelCLAA185WA01.h"
	#elif ( PanelType 	== PanelSVA156WX1 )
	      #include "PanelSVA156WX1.h"
	#elif ( PanelType 	== PanelM185B1L01 )
	      #include "PanelM185B1L01.h"
	#elif ( PanelType 	== PanelLM190E08 )
	      #include "PanelLM190E08.h"
	#elif ( PanelType 	== PanelM216H1L01 )
	      #include "PanelM216H1L01.h"
	#elif ( PanelType 	== PanelHT190WG1 )
	      #include "PanelHT190WG1.h"
	#elif ( PanelType 	== PanelLM185WH1TLA1 )
	      #include "PanelLM185WH1TLA1.h"
	#elif ( PanelType 	== PanelLTM230HT01 )
	      #include "PanelLTM230HT01.h"
	#elif ( PanelType 	== PanelLM230WF1TLA3 )
	      #include "PanelLM230WF1TLA3.h"
	#elif ( PanelType 	== PanelCMOM236H1L01 )
	      #include "PanelCMOM236H1L01.h"
	#elif ( PanelType 	== PanelLM220WE1 )
	      #include "PanelLM220WE1.h"
	#elif (PanelType == PanelAUOM240HW01V0)
    	  #include "PanelAUOM240HW01V0.h"    
	#elif (PanelType==PanelAUOM185XW01)
    	  #include "PanelAUOM185XW01.h"    
	#elif (PanelType==PanelTPM215HW01)
         #include "PanelTPM215HW01.h"
       #elif (PanelType == PanelCMOM200O1L02)
         #include "PanelCMOM200O1L02.h"
        #elif (PanelType == PanelAUOM215HW01L07)
         #include "PanelAUOM215HW01L07.h"         
       #elif (PanelType == PanelAUOM215HW01V7)
         #include "PanelAUOM215HW01V7.h"
       #elif (PanelType == PanelAUOM240HW01V6||PanelType==PanelAUOM240HW01VB)
         #include "PanelAUOM240HW01V6.h"
	#elif (PanelType==PanelTPM215HW01H1PS1)
         #include "PanelTPM215HW01H1PS1.h"
	#elif (PanelType==PanelCMOM215H1L01)
         #include "PanelCMOM215H1L01.h"
       #elif (PanelType==PanelCMOM236H1L07)
         #include "PanelCMOM236H1L07.h" 
       //#elif (PanelType==PanelCMOM200O1L02)
         //#include "PanelCMOM200O1L02.h" 
       #elif (PanelType == PanelAUOM215HW01V6||PanelType == PanelAUOM215HW01VB)
         #include "PanelAUOM215HW01V6.h"	
	#elif(PanelType==PanelAUOM215HW03)
	#include"PanelAUOM215HW03.h"
       #elif(PanelType==PanelTPM236H3L02 )
	   #include"PanelTPM236H3L02.h"
	   #elif(PanelType==PanelTPM200O1L02 )
	   #include"PanelTPM200O1L02.h"
	   #elif(PanelType==PanelM185XW01)
	   #include"PanelM185XW01.h"
	#elif(PanelType==PanelTPM215H1L01)
	#include "PanelTPM215H1L01.h" 
	#elif( PanelType  == PanelLM230WF1TLD1 )
	#include "PanelLM230WF1TLD1.h"	  
	#elif(PanelType==PanelAUOM156XW01V0)
	#include"PanelAUOM156XW01V0.h"
	#elif(PanelType==PanelAUOM240HW01V7)
	#include"PanelAUOM240HW01V7.h"
	#elif(PanelType==PanelAUOB156XW02V6 )
	#include"PanelAUOB156XW02V6.h"
	#elif(PanelType==PanelTPM185B1XW01)
	#include"PanelTPM185B1XW01.h"
	#elif(PanelType==PanelLM200WD3TLA1)
	#include "PanelLM200WD3TLA1.h"
	#elif(PanelType==PanelAUOM270HW01V0)
	#include"PanelAUOM270HW01V0.h"
	#elif(PanelType==PanelCMOM270HGEL10)
	#include"PanelCMOM270HGEL10.h"
	#elif(PanelType==PanelTPM200O1L02C1D)
	#include"PanelTPM200O1L02C1D.h"
	#elif(PanelType==PanelLM230WF5TLA1)
	#include"PanelLM230WF5TLA1.h"
	#elif(PanelType==PanelHSD190MEN4A02)
	#include"PanelHSD190MEN4A02.h"
	#elif(PanelType==PanelTPM190A1)
	#include"PanelTPM190A1.h"
	#elif(PanelType==PanelTMS190)
	#include"PanelTMS190.h"
	#elif(PanelType==PanelLM200WD3TLC4)
	#include"PanelLM200WD3TLC4.h"
	#elif(PanelType==PanelLM200WD3)
	#include"PanelLM200WD3.h"
	#elif(PanelType==PanelLM215WF1)
	#include"PanelLM215WF1.h"
	#elif(PanelType==PanelLM215WF4)
	#include"PanelLM215WF4TLE1.h"
	#elif(PanelType==PanelM215HW01)
	#include"PanelM215HW01.h"
	#elif(PanelType==PanelM185B3LA1)
	#include"PanelM185B3LA1.h"
	 #elif (PanelType == PanelB156XTN02)
    	#include "PanelB156XTN02.h"
	 #elif (PanelType == PanelB156XTN07)
    	#include "PanelB156XTN07.h"
	 #elif (PanelType == PanelNT156WHM_N12)
    	#include "PanelNT156WHM_N12.h"
	 #elif (PanelType == PanelM185BGE_L23)
    	#include "PanelM185BGE_L23.h"
	 #elif (PanelType == PanelM185WHB_N10)
    	#include "PanelM185WHB_N10.h"
	 #elif (PanelType == PanelLM185TT3A)
    	#include "PanelLM185TT3A.h"
	#elif(PanelType==PanelM236H3LA3)
	#include"PanelM236H3LA3.h"
	#elif (PanelType == PanelTPM215HW01_HGEL02)
    #include "PanelTPM215HW01_HGEL02.h"
	#elif (PanelType == PanelCMIM185BGEL22_new)//petit 20121228
    #include "PanelCMIM185BGEL22_new.h"
	#elif (PanelType == PanelTPM195WD1)//Gino 20140522
       #include "PanelTPM195WD1.h"
    #elif (PanelType == PanelM200HJJL20)//Gino 20140522
    #include "PanelM200HJJL20.h"
	#elif(PanelType==PanelHSM215EU_HTN01)
	#include"PanelHSM215EU_HTN01.h"
	#elif(PanelType==PanelHSM195EU_RTN01 )
	#include"PanelHSM195EU_RTN01.h"
	#elif (PanelType == PanelLGLP156WHB_DP)
    	#include "PanelLGLP156WHB.h"
	#elif (PanelType == PanelINNN173FGE_E23_DP)
    	#include "PanelINNN173FGE_E23_DP.h"
		
    #endif
#else
	#if (PanelType==PanelCMOL201L01)
	    #include "PanelCMOL201_L01_Init.h"  
	#elif (PanelType==PanelCMO190)
	    #include "PanelCMO190_Init.h"
	#elif (PanelType==PanelCMO190W)
	    #include "PanelCMO190W_Init.h"
	#elif(PanelType==PanelCMOM270HGEL10)
	#include"PanelCMOM270HGEL10_Init.h"
	#elif (PanelType==PanelHSD170MGW1)
	    #include "PanelHSD170MGW1_Init.h"
	#elif (PanelType==PanelCMOM220Z1_L03)
	    #include "PanelCMOM220Z1_L03_Init.h"
	#elif (PanelType==PanelLTM190M2)
	    #include "PanelLTM190M2_Init.h"
	#elif (PanelType==PanelCLAA201WA04)
	    #include "PanelCLAA201WA04_Init.h"
	#elif (PanelType==PanelM190A1L02)
	    #include "PanelCMOM190A1L02_Init.h"
	#elif (PanelType==PanelCLAA220WA01)
	    #include "PanelCLAA220WA01_Init.h"
	#elif (PanelType==PanelLTM220M1L01)
	    #include "PanelLTM220M1L01_Init.h"
	#elif (PanelType==PanelHSD190MEN3_A00)
	    #include "PanelHSD190MEN3_A00_Init.h"
	#elif (PanelType==PanelCLAA170WA02)
	    #include "PanelCLAA170WA02_Init.h"
	#elif (PanelType==PanelM190Z1L01)
	    #include "PanelM190Z1L01_Init.h"
	#elif ( PanelType  == PanelM156B1L01) 
	    #include "PanelM156B1L01_Init.h"
	#elif ( PanelType == PanelHSD170MGW1 )
		#include "PanelHSD170MGW1_Init.h"
	#elif ( PanelType 	== PanelCLAA170EA07P )
	      #include "PanelCLAA170EA07P_Init.h"
	#elif ( PanelType 	== PanelCMOM220Z1_PS3 )//Amy2007.12.26   18:51:54
	      #include "PanelCMOM220Z1_PS3_Init.h"
	#elif ( PanelType 	== PanelCMO220Z1M01 )//Amy2007.12.26   18:51:54
	      #include "PanelCMO220Z1M01_Init.h"
	#elif ( PanelType 	== PanelCLAA185WA01 )
	      #include "PanelCLAA185WA01_Init.h"
	#elif ( PanelType 	== PanelSVA156WX1 )
	      #include "PanelSVA156WX1_Init.h"
	#elif ( PanelType 	== PanelM185B1L01 )
	      #include "PanelM185B1L01_Init.h"
	#elif ( PanelType 	== PanelM216H1L01 )
	      #include "PanelM216H1L01_Init.h"
	#elif ( PanelType 	== PanelHT190WG1 )
	      #include "PanelHT190WG1_Init.h"		  
	#elif ( PanelType 	== PanelLM185WH1TLA1 )
	      #include "PanelLM185WH1TLA1_Init.h"
	#elif ( PanelType 	== PanelLTM230HT01 )
	      #include "PanelLTM230HT01_Init.h"
	#elif ( PanelType 	== PanelLM230WF1TLA3 )
	      #include "PanelLM230WF1TLA3_Init.h"
	#elif ( PanelType 	== PanelCMOM236H1L01 )
	      #include "PanelCMOM236H1L01_Init.h"		  
	#elif ( PanelType 	== PanelLM220WE1 )
	      #include "PanelLM220WE1_Init.h"
	#elif (PanelType == PanelAUOM240HW01V0)
    	  #include "PanelAUOM240HW01V0_Init.h"    
	#elif (PanelType==PanelAUOM185XW01)
    	  #include "PanelAUOM185XW01_Init.h"    
       #elif (PanelType==PanelTPM215HW01H1PS1)
         #include "PanelTPM215HW01H1PS1_Init.h"
	#elif (PanelType == PanelAUOM215HW01V6||PanelType == PanelAUOM215HW01VB||PanelType == PanelAUOM215HW01)
         #include "PanelAUOM215HW01V6_Init.h"
	#elif(PanelType==PanelTPM236H3L02 )
	   #include"PanelTPM236H3L02_Init.h"
	#elif(PanelType==PanelCLAA215FA04 )
	   #include"PanelCLAA215FA04_Init.h"
	#elif( PanelType  == PanelLM230WF1TLD1 )
	      #include "PanelLM230WF1TLD1_Init.h"	  
       #elif(PanelType==PanelAUOM156XW01V0)
	#include"PanelAUOM156XW01V0_Init.h"
	#elif(PanelType==PanelAUOM240HW01V7)
	#include"PanelAUOM240HW01V7_Init.h"
	#elif(PanelType==PanelAUOM240HW01V6||PanelType==PanelAUOM240HW01VB)
	#include"PanelAUOM240HW01V6_Init.h"
	#elif(PanelType==PanelTPM185B1XW01)
	#include"PanelTPM185B1XW01_Init.h"
	#elif(PanelType==PanelLM200WD3TLA1)
	#include"PanelLM200WD3TLA1_Init.h"
	#elif(PanelType==PanelAUOM270HW01V0)
	#include"PanelAUOM270HW01V0_Init.h"
	#elif(PanelType==PanelTPM200O1L02C1D)
	#include"PanelTPM200O1L02C1D_Init.h"
	#elif(PanelType==PanelLM230WF5TLA1)
	#include"PanelLM230WF5TLA1_Init.h"
	#elif(PanelType==PanelHSD190MEN4A02)
	#include"PanelHSD190MEN4A02_Init.h"
	#elif(PanelType==PanelTPM190A1)
	#include"PanelTPM190A1_Init.h"
	#elif(PanelType==PanelTMS190)
	#include"PanelTMS190_Init.h"
	#elif(PanelType==PanelLM200WD3TLC4)
	#include"PanelLM200WD3TLC4_Init.h"
	#elif(PanelType==PanelLM200WD3)
	#include"PanelLM200WD3_Init.h"
	#elif (PanelType==PanelCMOM236H1L01_1)
       #include "PanelCMOM236H1L01_1Init.h"
	#elif (PanelType==PanelM236MWF1 )
       #include "PanelM236MWF1 _Init.h"
	#elif (PanelType==PanelLM215WF1)
       #include "PanelLM215WF1_Init.h"
	#elif(PanelType==PanelLM190E08 )
	#include"PanelLM190E08_Init.h"
	#elif(PanelType==PanelLM215WF4)
	#include"PanelLM215WF4TLE1_Init.h"
	#elif(PanelType==PanelTPM156B1)
	#include"PanelTPM156B1_Init.h"
	#elif(PanelType==PanelTPM156B1L01)
	#include"PanelTPM156B1L01_Init.h"
	#elif(PanelType==PanelTPM156B1WX102TC)
	#include"PanelTPM156B1WX102TC_Init.h"
	#elif(PanelType==PanelCLAA156WB11A)
	#include"PanelCLAA156WB11A_Init.h"
	#elif(PanelType==PanelCMON156B6L0B)
	#include"PanelCMON156B6L0B_Init.h"
	#elif(PanelType==PanelBOEHT156WXB)
	#include"PanelBOEHT156WXB_Init.h"
	#elif(PanelType==PanelM215HW01)
	#include"PanelM215HW01_Init.h"
	#elif(PanelType==PanelM185B3LA1)
	#include"PanelM185B3LA1_Init.h"
	#elif(PanelType==PanelM236H3LA3)
	#include"PanelM236H3LA3_Init.h"
	#elif(PanelType==PanelAUOM215HW01VC)
	#include"PanelAUOM215HW01VC_Init.h"
	#elif(PanelType==PanelAUOM215HW03)
	#include"PanelAUOM215HW03.h"
	#elif(PanelType==PanelLTM200KT01)		//110617 Modify
	#include"PanelLTM200KT01_Init.h"
	#elif(PanelType==PanelLTM220MT05)		//110617 Modify
	#include"PanelLTM220MT05_Init.h"
    #elif(PanelType==PanelLTM185AT05)		//110617 Modify
	#include"PanelLTM185AT05_Init.h"
    #elif(PanelType==PanelTPM220Z1)		//110617 Modify
	#include"PanelTPM220Z1_Init.h"
	#elif(PanelType==PanelLTM220MT09)		//110617 Modify
	#include"PanelLTM220MT09_Init.h"
	#elif(PanelType == PanelLM230WF5TLF1)   //111123 Modify
    #include"PanelLM230WF5TLF1_Init.h"
	#elif(PanelType == PanelCMIM185BGEL22)   //111123 Modify
    #include"PanelCMIM185BGEL22_Init.h"
	#elif (PanelType == PanelTPM220Z1_ZGEL01)//120115
    #include "PanelTPM220Z1_ZGEL01_Init.h"
    #elif (PanelType == PanelAUOM270HW01V2)
    #include "PanelAUOM270HW01V2_Init.h"
    #elif (PanelType == PanelLTN156AT24_P)
    #include "PanelLTN156AT24_P_Init.h"
    #elif (PanelType == PanelLM270HGEL20)
    #include "PanelM270HGE_L20_Init.h"
    #elif (PanelType == PanelHB156WX1_100)
    #include "PanelHB156WX1_100_Init.h"
    #elif (PanelType == PanelB156XTN02)
    #include "PanelB156XTN02_Init.h"
    #elif (PanelType == PanelM236HGEL20)
    #include "PanelM236HGEL20_Init.h"
    #elif (PanelType == PanelTPM185B1_WX3400)
    #include "PanelTPM185B1_WX3400_Init.h"
    #elif (PanelType == PanelTPM236WU110)
    #include "PanelTPM236WU110_Init.h"
    #elif (PanelType == PanelTPM215HW01_HGEL02)
    #include "PanelTPM215HW01_HGEL02_Init.h"
    #elif (PanelType == PanelCMIM195FEG_L20)//petit 20121030
    #include "PanelCMIM195FEG-L20_Init.h"
   #elif (PanelType == PanelLNTBM200WD3_T2BB)//petit 20121031
    #include "PanelLNTBM200WD3_T2BB_Init.h"
   #elif (PanelType == PanelAUOM240HW01VD)//petit 20121101
    #include "PanelAUOM240HW01VD_Init.h"
#elif (PanelType == PanelCMIM185BGEL22_new)//petit 20121228
    #include "PanelCMIM185BGEL22_new_Init.h"
  #elif (PanelType == PanelTPM240WF1_HTN01)//20130327
    #include "PanelTPM240WF1_HTN01_Init.h"
 #elif (PanelType == PanelTPM240WF1_HW01)//20130327
    #include "PanelTPM240WF1_HW01_Init.h"
   #elif (PanelType == PanelTPM195WD1)//Gino 20140523
    #include "PanelTPM195WD1_Init.h"
   #elif (PanelType == PanelM200HJJL20)//Gino 20140523
    #include "PanelM200HJJL20_Init.h"
  #elif(PanelType==PanelHSM215EU_HTN01)
  #include"PanelHSM215EU_HTN01_Init.h"
   #elif (PanelType == PanelLGLP156WHB_DP)
    #include "PanelLGLP156WHB_Init.h"
    #elif (PanelType == PanelINNN173FGE_E23_DP)
    #include "PanelINNN173FGE_E23_DP_Init.h"

	#endif
#endif
	
#ifndef DISABLE_SHORT_FRAME_TUNE	//petit 20121031
#define DISABLE_SHORT_FRAME_TUNE    0
#endif

#ifndef PANEL_VCOM_ADJUST		//121031 Modify
#define PANEL_VCOM_ADJUST       0
#endif

