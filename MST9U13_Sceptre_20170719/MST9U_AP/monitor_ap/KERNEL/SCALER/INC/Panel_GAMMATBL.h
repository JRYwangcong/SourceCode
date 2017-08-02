
#ifndef _GAMMATBL_H
#define _GAMMATBL_H
#if (PanelType==PanelLM220WE1)
#include "Gama_PanelLM220WE1.h"
#elif(PanelType==PanelLTM230HT03)
#include "Gama_PanelLTM230HT03.h"
#elif (PanelType == PanelLM230WF5TLD1)
#include  "Gama_PanelLM230WF5TLD1.h"
#elif (PanelType==PanelTPM215HW01_HGE)// ||(PanelType==PanelTPM215HW01_HGJ)      //110828 Rick add
#include "Gama_PanelTPM215HW01_HGE.h"
#elif (PanelType == PanelM320QAN01||PanelType == PanelCMI28UltraHD||PanelType == PanelLSF490FJ02)
#include  "Gama_PanelM320QAN01.h"
#elif (PanelType == PanelLSC400FN04)
#include  "Gama_PanelLSC400FN04.h"
#elif (PanelType == PanelLTM340YP01)
#include  "Gama_PanelLTM340YP01.h"
#elif PanelType == PanelT430QVN01
#include  "Gama_PanelT430QVN01.h"
#elif PanelType == PanelM350DVR01_01
#include  "Gama_PanelM350DVR01_01.h"
#elif PanelType == PanelLSM315DP01
#include  "Gama_PanelLSM315DP01.h"
#elif PanelType == PanelLM270WQ1
#include  "Gama_PanelLM270WQ1.h"
#elif PanelType == PanelLSM270DP01
#include  "Gama_PanelLSM270DP01.h"
#elif PanelType == PanelLSM315HP01
#include  "Gama_PanelLSM315HP01.h"
#elif PanelType == PanelM238DCJ_E50
#include  "Gama_PanelM238DCJ.h"
#elif PanelType == PanelM315DVR011||PanelType == PanelLSM315DP02
#include  "Gama_PanelM315DVR011.h"
#elif PanelType==PanelM350DVA01_1
#include  "Gama_PanelM350DVA01_1.h"
#elif PanelType==PanelLSM270HP06
#include  "Gama_PanelLSM270HP06.h"

#else
#warning "Please Panel Gamma Table!"
#include "Gama_Default.h" // copy from Gama_PanelLM220WE1.h
#endif

/*
//256 Entry - 10 Bits
code BYTE *tAllGammaTab[][5] =
{
    //  18
    {
        tblGamma20_RN,
        tblGamma20_GN,
        tblGamma20_BN,
    },
    //  20
    {
        tblGamma20_RN,
        tblGamma20_GN,
        tblGamma20_BN,
    },
    // 22
    {
       tblGamma22_RN,
        tblGamma22_GN,
        tblGamma22_BN,
    },
    //  24
    {
        tblGamma24_RN,
        tblGamma24_GN,
        tblGamma24_BN,
    },
    //  26
    {
        tblGamma20_RN,
        tblGamma20_GN,
        tblGamma20_BN,
    },
};
*/
//post gamma : 1024 entry - 14 bits
code BYTE *tAllFullGammaTab[][5]=
{
    // 2.0
    {tblFullGamma22_R, tblFullGamma22_G, tblFullGamma22_B, },//Gamma off
    {tblFullGamma18_R, tblFullGamma18_G, tblFullGamma18_B, },
    {tblFullGamma20_R, tblFullGamma20_G, tblFullGamma20_B, },
    {tblFullGamma22_R, tblFullGamma22_G, tblFullGamma22_B, },
    {tblFullGamma24_R, tblFullGamma24_G, tblFullGamma24_B, },
#if 0//(!Enable_ECOGameMode)    
    {tblFullGamma26_R, tblFullGamma26_G, tblFullGamma26_B, },
#endif    
};

#if 1//((defined(FengYuan_Project))||Enable_ECOGameMode||defined(XuLi_Project)||defined(MingCai_Project))
code BYTE *tAllFullGammaTab_Game[][3]=
{
    // 2.0
    {tblFullGammaGame1_R, tblFullGammaGame1_G, tblFullGammaGame1_B, },
    {tblFullGammaGame2_R, tblFullGammaGame2_G, tblFullGammaGame2_B, },
    {tblFullGammaGame3_R, tblFullGammaGame3_G, tblFullGammaGame3_B, },
   #if 1//Enable_ECOGameMode 
    {tblFullGammaGame4_R, tblFullGammaGame4_G, tblFullGammaGame4_B, },
   #endif 
};
#endif


#endif
