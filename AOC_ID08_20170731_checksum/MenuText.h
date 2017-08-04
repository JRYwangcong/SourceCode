#include "Panel.h"

//=================================================================
BYTE code strVersionDate[] =
{
    //123456789012345678901234567890
   _V,F_FWVersion,'\x0'//"V"FWVersion  //Benz 2007.4.14   21:46:49
};
BYTE code strChangeDate[] =
{
    //123456789012345678901234567890
    ChangeDate,'\x0'  //Benz 2007.4.15   22:47:41
};

BYTE code strModelName[] =
{
    //123456789012345678901234567890
    ShowModelName,'\x0'  //Benz 2007.4.15   22:47:41
};

#if PANELSSCSTEPLFUN
BYTE code strSSCSTEPL[] = 
{
    //123456789012345678901234567890
    _S,_S,_C,_SPACE,_S,_t,_e,_p,_L,'\x0'
};
BYTE code strSSCSTEPH[] = 
{
    //123456789012345678901234567890
    _S,_S,_C,_SPACE,_S,_t,_e,_p,_H,'\x0'
};
BYTE code strSSCSPANL[] = 
{
    //123456789012345678901234567890
    _S,_S,_C,_SPACE,_S,_p,_a,_n,_L,'\x0'
};
BYTE code strSSCSPANH[] = 
{
    //123456789012345678901234567890
    _S,_S,_C,_SPACE,_S,_p,_a,_n,_H,'\x0'
};
#endif


#if EnablePanelServiceMenu
BYTE code strPanel[2][15] =  //
{
    //123456789012345678901234567890
    {PanelName,'\x0'},
    {_U,_s,_e,_r,_P,_a,_n,_e,_l,_SPACE,_SPACE,_SPACE,'\x0'}
};
#else
BYTE code strPanel[] =  //
{
    //123456789012345678901234567890
    PanelName,'\x0'
};
#endif

BYTE code strIcModelText[] =
{
    //123456789012345678901234567890
  #if ModelName==ENVISION_JRY1PTRL||ModelName==ENVISION_R2218WS||ModelName==ENVISION_R2018WS||ModelName==ENVISION_JRYL1PTR_EV1
     _T,_S,_U,_M,_1,_P,_T,_R,_L,_DASH,_L,_F,'\x0'  //Benz 2007.4.15   22:47:41
    //#elif ModelName==JRY_58WV6_58NWHL
 //   _T,_S,_U,_M,_U,_5,_8,_N,_W,_H,_L,_DASH,_L,_F,_1,'\x0'  //Benz 2007.4.15   22:47:41
   #endif
};

#if BlacklitTimeOnShow
BYTE code strPanelOn[] =  //
{
    //123456789012345678901234567890
    _P,_a,_n,_e,_l,_SPACE,_O,_n,'\x0'//"PANEL\X001ON"
};
#endif
BYTE code strAutoColor[] =
{
    //123456789012345678901234567890
    _A,_u,_t,_o,_SPACE,_C,_o,_l,_o,_r,'\x0'//"AUTO\x001COLOR"
};
BYTE code strPass[] =
{
    //123456789012345678901234567890
    _P,_a,_s,_s,'\x0'//"PASS"
};
BYTE code strFail[] =
{
    //123456789012345678901234567890
    _F,_a,_i,_l,'\x0'//"FAIL"
};
BYTE code strAdcGain[] =
{
    //123456789012345678901234567890
    _G,_a,_i,_n,'\x0'//"GAIN"
};
BYTE code strAdcOffset[] =
{
    //123456789012345678901234567890
    _O,_f,_f,_s,_e,_t,'\x0'//"OFFSET"
};
BYTE code strFactoryRed[] =
{
    //123456789012345678901234567890
    _R,'\x0'//"R"
};
BYTE code strFactoryGreen[] =
{
    //123456789012345678901234567890
   _G, '\x0'//"G"
};
BYTE code strFactoryBlue[] =
{
    //123456789012345678901234567890
    _B,'\x0'//"B"
};
BYTE code strFCool1[] =
{
    //123456789012345678901234567890
    // "C2"//"C2\x08393"//9300K"
    _C,_o,_o,_l,'\x0'//"COOL1"
};
BYTE code strFWarm1[] =
{
    //123456789012345678901234567890
    // "C2"//"C2\x08393"//9300K"
    _W,_a,_r,_m,'\x0'//"WARM1"
};
BYTE code strFsRGB[] =
{
    //123456789012345678901234567890
    _s,_R,_G,_B,'\x0'//"SRGB" //\x08365"//6500K"
};
BYTE code strFNORMAL[] =
{
    //123456789012345678901234567890
   _N,_o,_r,_m,_a,_l,'\x0'// "NORMAL" //\x08365"//6500K"
};
BYTE code strFactoryBrightness[] =
{
    //123456789012345678901234567890
    _B,_r,_i,'\x0'//"BRI" //\x08365"//6500K"
};
BYTE code strFactoryContrast[] =
{
    //123456789012345678901234567890
    _C,_o,_n,_t,'\x0'//"CONT" //\x08365"//6500K"
};
BYTE code strBurnin[] =
{
    //123456789012345678901234567890
    _B,_u,_r,_n,_I,_n,'\x0'////"BURIN"
};
#if Enable_ProductModeAdjust
BYTE code strDFM[] =
{
    //123456789012345678901234567890
    _D,_F,_M,'\x0'////"DFM"
};
#endif	

#if MS_OVD
BYTE code strOverDrive[] = 
{
    //123456789012345678901234567890
    _O,_V,_D,'\x0'
};
#endif

BYTE code strOnOff[2][4]=
{//123456789012345678901234567890
 //  O	 f	 f
 {_O,_f,_f,'\x0'},// {"OFF"},
 //  O	 n
  {_O,_n,_SPACE,'\x0'}//{"ON\x001"}
};
/*
BYTE code strBank[] =
{//123456789012345678901234567890
  _B,_a,_n,_k,'\x0'//"BANK"
};
BYTE code strBankName[3][9]=
{//123456789012345678901234567890
 {_0,_DASH,_S,_c,_a,_l,_e,_r,'\x0'},//{"0\x011SCALER"},
 {_1,_DASH,_A,_D,_C,_SPACE,_SPACE,_SPACE,'\x0'},//{"1\x011ADC\x01\x01\x01"},
 {_2,_DASH,_T,_C,_o,_n,_SPACE,_SPACE,'\x0'}//{"2\x011TCON\x01"}
};
BYTE code strAddress[]=
{
    //123456789012345678901234567890
    _A,_d,_d,_r,'\x0'//"ADDR"
};
BYTE code strValue[] =
{
    //123456789012345678901234567890
    _V,_a,_l,_u,_e,'\x0'//"VALUE"
};
//*/
#if 0//EnablePanelServiceMenu

BYTE code strPanelSelect[] =
{
    //123456789012345678901234567890
    _P,_a,_n,_e,_l,'\x0'//"VALUE" //20130402 petit
};

#endif
#if 0// EnablePanelServiceMenu
BYTE code strAdjustPanelPara[] = 
{
    //123456789012345678901234567890
    _P,_a,_n,_e,_l,_SPACE,_S,_e,_r,_v,_i,_c,_e,_SPACE,_M,_e,_n,_u,'\x0'
};

BYTE code strPanelDither[] = 
{
    //123456789012345678901234567890
    _C,_o,_l,_o,_r,_SPACE,_D,_e,_p,_t,_h,'\x0'
};

BYTE code str6BitColor[] = 
{
    //123456789012345678901234567890
    _6,'\x0'
};
BYTE code str8BitColor[] = 
{
    //123456789012345678901234567890
    _8,'\x0'
};

BYTE code strPanelOnTiming1[] = 
{
    //123456789012345678901234567890
    _O,_n,_SPACE,_T,_1,'\x0'
};
BYTE code strPanelOnTiming2[] = 
{
    //123456789012345678901234567890
    _O,_n,_SPACE,_T,_2,'\x0'
};
BYTE code strPanelOffTiming1[] = 
{
    //123456789012345678901234567890
    _O,_f,_f,_SPACE,_T,_1,'\x0'
};
BYTE code strPanelOffTiming2[] = 
{
    //123456789012345678901234567890
    _O,_f,_f,_SPACE,_T,_2,'\x0'
};
BYTE code strPanelHSyncWidth[] = 
{
    //123456789012345678901234567890
    _H,_S,_y,_n,_c,_SPACE,_W,_i,_d,_t,_h,'\x0'
};
BYTE code strPanelHSyncBackPorch[] = 
{
    //123456789012345678901234567890
    _H,_S,_y,_n,_c,_SPACE,_B,_P,_o,_r,_c,_h,'\x0'
};
BYTE code strPanelVSyncWidth[] = 
{
    //123456789012345678901234567890
    _V,_S,_y,_n,_c,_SPACE,_W,_i,_d,_t,_h,'\x0'
};
BYTE code strPanelVSyncBackPorch[] = 
{
    //123456789012345678901234567890
    _V,_S,_y,_n,_c,_SPACE,_B,_P,_o,_r,_c,_h,'\x0'
};
BYTE code strPanelWidth[] = 
{
    //123456789012345678901234567890
    _W,_i,_d,_t,_h,'\x0'
};
BYTE code strPanelHeight[] = 
{
    //123456789012345678901234567890
    _H,_e,_i,_g,_h,_t,'\x0'
};
BYTE code strPanelHTotal[] = 
{
    //123456789012345678901234567890
    _H,_T,_o,_t,_a,_l,'\x0'
};
BYTE code strPanelVTotal[] = 
{
    //123456789012345678901234567890
    _V,_T,_o,_t,_a,_l,'\x0'
};
BYTE code strPanelMaxHTotal[] = 
{
    //123456789012345678901234567890
    _M,_a,_x,_SPACE,_H,_T,_o,_t,_a,_l,'\x0'
};
BYTE code strPanelMinHTotal[] = 
{
    //123456789012345678901234567890
    _M,_i,_n,_SPACE,_H,_T,_o,_t,_a,_l,'\x0'
};
/*BYTE code strPanelMaxVTotal[] = 
{
    //123456789012345678901234567890
    _M,_a,_x,_SPACE,_V,_T,_o,_t,_a,_l,'\x0'
};
BYTE code strPanelMinVTotal[] = 
{
    //123456789012345678901234567890
    _M,_i,_n,_SPACE,_V,_T,_o,_t,_a,_l,'\x0'
};
*/

BYTE code strPanelDCLK[] = 
{
    //123456789012345678901234567890
    _D,_c,_l,_k,'\x0'
};

BYTE code strPanelMaxDCLK[] = 
{
    //123456789012345678901234567890
    _M,_a,_x,SpaceFont,_D,_c,_l,_k,'\x0'
};
BYTE code strPanelMinDCLK[] = 
{
    //123456789012345678901234567890
    _M,_i,_n,SpaceFont,_D,_c,_l,_k,'\x0'
};
BYTE code strSwingText[] = 
{
    _S,_w,_i,_n,_g,'\x0'//"Swing"
};

BYTE code strABPortSwap[] = 
{
    //123456789012345678901234567890
    _O,_d,_d,SpaceFont,_E,_v,_e,_n,SpaceFont,_S,_w,_a,_p,'\x0'
};
BYTE code strDualPixelOutput[] = 
{
    //123456789012345678901234567890
    _D,_u,_a,_l,SpaceFont,_P,_i,_x,_e,_l,'\x0'
};


BYTE code strPNSwap[] = 
{
    //123456789012345678901234567890
    _P,_N,SpaceFont,_S,_w,_a,_p,'\x0'
};
BYTE code strChannelSwap[] = 
{
    //123456789012345678901234567890
    _C,_h,_a,_n,_n,_e,_l,SpaceFont,_S,_w,_a,_p,'\x0'
};
BYTE code strMSBLSBExchange[] = 
{
    //123456789012345678901234567890
    _M,_L,_S,_B,SpaceFont,_E,_x,'\x0'
};
BYTE code strTIMode[] = 
{
    //123456789012345678901234567890
    _T,_I,SpaceFont,_M,_o,_d,_e,'\x0'
};
BYTE code strOddMSBLSBExchange[] = 
{
    //123456789012345678901234567890
    _O,_d,_d,_SPACE,_M,_L,_S,_B,SpaceFont,_E,_x,'\x0'
};
BYTE code strEvenMSBLSBExchange[] = 
{
    //123456789012345678901234567890
    _E,_v,_e,_n,_SPACE,_M,_L,_S,_B,SpaceFont,_E,_x,'\x0'
};
BYTE code strOddRBExchange[] = 
{
    //123456789012345678901234567890
    _O,_d,_d,SpaceFont,_R,_B,SpaceFont,_E,_x,'\x0'
};
BYTE code strEvenRBExchange[] = 
{
    //123456789012345678901234567890
    _E,_v,_e,_n,SpaceFont,_R,_B,SpaceFont,_E,_x,'\x0'
};
/*BYTE code strDECurrent[] = 
{
    //123456789012345678901234567890
    _D,_E,_SPACE,_C,_u,_r,_r,_e,_n,_t,'\x0'
};
BYTE code strClockCurrent[] = 
{
    //123456789012345678901234567890
    _C,_l,_o,_c,_k,_SPACE,_C,_u,_r,_r,_e,_n,_t,'\x0'
};
BYTE code strOddCurrent[] = 
{
    //123456789012345678901234567890
    _O,_d,_d,_SPACE,_C,_u,_r,_r,_e,_n,_t,'\x0'
};
BYTE code strEvenCurrent[] = 
{
    //123456789012345678901234567890
    _E,_v,_e,_n,_SPACE,_C,_u,_r,_r,_e,_n,_t,'\x0'
};

BYTE code str4mA[] = 
{
    //123456789012345678901234567890
    _SPACE,_4,_m,_A,'\x0'
};
BYTE code str6mA[] = 
{
    //123456789012345678901234567890
    _SPACE,_6,_m,_A,'\x0'
};
BYTE code str8mA[] = 
{
    //123456789012345678901234567890
    _SPACE,_8,_m,_A,'\x0'
};
BYTE code str12mA[] = 
{
    //123456789012345678901234567890
    _1,_2,_m,_A,'\x0'
};*/
BYTE code strBurstModeFreq[] = 
{
    //123456789012345678901234567890
    _B,_u,_r,_s,_t,_M,_o,_d,_e,_F,_r,_e,_q,'\x0'
};
BYTE code strDefDuty[] = 
{
    //123456789012345678901234567890
    _D,_e,_f,_SPACE,_D,_u,_t,_y,'\x0'
};
BYTE code strMinDuty[] = 
{
    //123456789012345678901234567890
    _M,_i,_n,_SPACE,_D,_u,_t,_y,'\x0'
};
BYTE code strMaxDuty[] = 
{
    //123456789012345678901234567890
    _M,_a,_x,_SPACE,_D,_u,_t,_y,'\x0'
};
BYTE code strChangeModePanelVdd[] = 
{
    //123456789012345678901234567890
    _P,_a,_n,_e,_l,_SPACE,_V,_d,_d,'\x0'
};
BYTE code strSSCSTEPL[] = 
{
    //123456789012345678901234567890
    _S,_S,_C,_SPACE,_S,_t,_e,_p,_L,'\x0'
};
BYTE code strSSCSTEPH[] = 
{
    //123456789012345678901234567890
    _S,_S,_C,_SPACE,_S,_t,_e,_p,_H,'\x0'
};
BYTE code strSSCSPANL[] = 
{
    //123456789012345678901234567890
    _S,_S,_C,_SPACE,_S,_p,_a,_n,_L,'\x0'
};
BYTE code strSSCSPANH[] = 
{
    //123456789012345678901234567890
    _S,_S,_C,_SPACE,_S,_p,_a,_n,_H,'\x0'
};
BYTE code strInitialSetting[] = 
{
    //123456789012345678901234567890
    _I,_n,_i,_t,_i,_a,_l,_SPACE,_S,_e,_t,_t,_i,_n,_g,'\x0'
};

BYTE code strSaveSetting[] = 
{
    //123456789012345678901234567890
    _S,_a,_v,_e,_SPACE,_S,_e,_t,_t,_i,_n,_g,'\x0'
};


#endif

#if EnableVcomAdjustItem||PANEL_VCOM_ADJUST		//121031 Modify
BYTE code strVCOM[] = 
{
    //123456789012345678901234567890
    _V,_C,_O,_M,'\x0'
};
#endif

//===============================================================================================================================================================================
//===============================================================================================================================================================================

BYTE code strFactory[2] =
{
  MAIN_FACTORY_FONT_START,
  '\x0'
};
//===============================================================================================================================================================================
//===============================================================================================================================================================================
      BYTE code English_Luminace_Text[]=        {    English_Luminace                                  };
      BYTE code English_Imagesetup_Text[]=      {    English_Imagesetup                                };
      BYTE code English_Colortemp_Text[]=       {    English_Colortemp                                 };
      BYTE code English_Colorboost_Text[]=      {    English_Colorboost                                };
      BYTE code English_Pictureboost_Text[]=    {    English_Pictureboost                              };
      BYTE code English_Osdsetup_Text[]=        {    English_Osdsetup                                  };
      BYTE code English_Extra_Text[]=           {    English_Extra                                     };
      BYTE code English_Reset_Text[]=           {    English_Reset                                     };
      BYTE code English_Exit_Text[]=            {    English_Exit                                      };

      BYTE* code Language_English_Table[]=
      {
               &English_Luminace_Text,
               &English_Imagesetup_Text,
               &English_Colortemp_Text,
               &English_Colorboost_Text,
               &English_Pictureboost_Text,
               &English_Osdsetup_Text,
               &English_Extra_Text,
               &English_Reset_Text,
               &English_Exit_Text
      };

      BYTE code Spanish_Luminace_Text[]=        {    Spanish_Luminace                                  };
      BYTE code Spanish_Imagesetup_Text[]=      {    Spanish_Imagesetup                                };
      BYTE code Spanish_Colortemp_Text[]=       {    Spanish_Colortemp                                 };
      BYTE code Spanish_Colorboost_Text[]=      {    Spanish_Colorboost                                };
      BYTE code Spanish_Pictureboost_Text[]=    {    Spanish_Pictureboost                              };
      BYTE code Spanish_Osdsetup_Text[]=        {    Spanish_Osdsetup                                  };
      BYTE code Spanish_Extra_Text[]=           {    Spanish_Extra                                     };
      BYTE code Spanish_Reset_Text[]=           {    Spanish_Reset                                     };
      BYTE code Spanish_Exit_Text[]=            {    Spanish_Exit                                      };

      BYTE* code  Language_Spanish_Table[]=
      {
               &Spanish_Luminace_Text,
               &Spanish_Imagesetup_Text,
               &Spanish_Colortemp_Text,
               &Spanish_Colorboost_Text,
               &Spanish_Pictureboost_Text,
               &Spanish_Osdsetup_Text,
               &Spanish_Extra_Text,
               &Spanish_Reset_Text,
               &Spanish_Exit_Text
      };

      BYTE code France_Luminace_Text[]=         {    France_Luminace                                   };
      BYTE code France_Imagesetup_Text[]=       {    France_Imagesetup                                 };
      BYTE code France_Colortemp_Text[]=        {    France_Colortemp                                  };
      BYTE code France_Colorboost_Text[]=       {    France_Colorboost                                 };
      BYTE code France_Pictureboost_Text[]=     {    France_Pictureboost                               };
      BYTE code France_Osdsetup_Text[]=         {    France_Osdsetup                                   };
      BYTE code France_Extra_Text[]=            {    France_Extra                                      };
      BYTE code France_Reset_Text[]=            {    France_Reset                                      };
      BYTE code France_Exit_Text[]=             {    France_Exit                                       };

      BYTE* code  Language_France_Table[]=
      {
               &France_Luminace_Text,
               &France_Imagesetup_Text,
               &France_Colortemp_Text,
               &France_Colorboost_Text,
               &France_Pictureboost_Text,
               &France_Osdsetup_Text,
               &France_Extra_Text,
               &France_Reset_Text,
               &France_Exit_Text
      };

      BYTE code Russia_Luminace_Text[]=         {    Russia_Luminace                                   };
      BYTE code Russia_Imagesetup_Text[]=       {    Russia_Imagesetup                                 };
      BYTE code Russia_Colortemp_Text[]=        {    Russia_Colortemp                                  };
      BYTE code Russia_Colorboost_Text[]=       {    Russia_Colorboost                                 };
      BYTE code Russia_Pictureboost_Text[]=     {    Russia_Pictureboost                               };
      BYTE code Russia_Osdsetup_Text[]=         {    Russia_Osdsetup                                   };
      BYTE code Russia_Extra_Text[]=            {    Russia_Extra                                      };
      BYTE code Russia_Reset_Text[]=            {    Russia_Reset                                      };
      BYTE code Russia_Exit_Text[]=             {    Russia_Exit                                       };

      BYTE* code  Language_Russia_Table[]=
      {
               &Russia_Luminace_Text,
               &Russia_Imagesetup_Text,
               &Russia_Colortemp_Text,
               &Russia_Colorboost_Text,
               &Russia_Pictureboost_Text,
               &Russia_Osdsetup_Text,
               &Russia_Extra_Text,
               &Russia_Reset_Text,
               &Russia_Exit_Text
      };

#if LANGUAGE_TYPE==ASIA

      BYTE code Portugues_Luminace_Text[]=      {    Portugues_Luminace                                };
      BYTE code Portugues_Imagesetup_Text[]=    {    Portugues_Imagesetup                              };
      BYTE code Portugues_Colortemp_Text[]=     {    Portugues_Colortemp                               };
      BYTE code Portugues_Colorboost_Text[]=    {    Portugues_Colorboost                              };
      BYTE code Portugues_Pictureboost_Text[]=  {    Portugues_Pictureboost                            };
      BYTE code Portugues_Osdsetup_Text[]=      {    Portugues_Osdsetup                                };
      BYTE code Portugues_Extra_Text[]=         {    Portugues_Extra                                   };
      BYTE code Portugues_Reset_Text[]=         {    Portugues_Reset                                   };
      BYTE code Portugues_Exit_Text[]=          {    Portugues_Exit                                    };

      BYTE* code  Language_Portugues_Table[]=
      {
               &Portugues_Luminace_Text,
               &Portugues_Imagesetup_Text,
               &Portugues_Colortemp_Text,
               &Portugues_Colorboost_Text,
               &Portugues_Pictureboost_Text,
               &Portugues_Osdsetup_Text,
               &Portugues_Extra_Text,
               &Portugues_Reset_Text,
               &Portugues_Exit_Text
      };

      BYTE code SChina_Luminace_Text[]=         {    SChina_Luminace                                   };
      BYTE code SChina_Imagesetup_Text[]=       {    SChina_Imagesetup                                 };
      BYTE code SChina_Colortemp_Text[]=        {    SChina_Colortemp                                  };
      BYTE code SChina_Colorboost_Text[]=       {    SChina_Colorboost                                 };
      BYTE code SChina_Pictureboost_Text[]=     {    SChina_Pictureboost                               };
      BYTE code SChina_Osdsetup_Text[]=         {    SChina_Osdsetup                                   };
      BYTE code SChina_Extra_Text[]=            {    SChina_Extra                                      };
      BYTE code SChina_Reset_Text[]=            {    SChina_Reset                                      };
      BYTE code SChina_Exit_Text[]=             {    SChina_Exit                                       };

      BYTE* code  Language_SChina_Table[]=
      {
               &SChina_Luminace_Text,
               &SChina_Imagesetup_Text,
               &SChina_Colortemp_Text,
               &SChina_Colorboost_Text,
               &SChina_Pictureboost_Text,
               &SChina_Osdsetup_Text,
               &SChina_Extra_Text,
               &SChina_Reset_Text,
               &SChina_Exit_Text
      };

      BYTE code TChina_Luminace_Text[]=         {    TChina_Luminace                                   };
      BYTE code TChina_Imagesetup_Text[]=       {    TChina_Imagesetup                                 };
      BYTE code TChina_Colortemp_Text[]=        {    TChina_Colortemp                                  };
      BYTE code TChina_Colorboost_Text[]=       {    TChina_Colorboost                                 };
      BYTE code TChina_Pictureboost_Text[]=     {    TChina_Pictureboost                               };
      BYTE code TChina_Osdsetup_Text[]=         {    TChina_Osdsetup                                   };
      BYTE code TChina_Extra_Text[]=            {    TChina_Extra                                      };
      BYTE code TChina_Reset_Text[]=            {    TChina_Reset                                      };
      BYTE code TChina_Exit_Text[]=             {    TChina_Exit                                       };

      BYTE* code  Language_TChina_Table[]=
      {
               &TChina_Luminace_Text,
               &TChina_Imagesetup_Text,
               &TChina_Colortemp_Text,
               &TChina_Colorboost_Text,
               &TChina_Pictureboost_Text,
               &TChina_Osdsetup_Text,
               &TChina_Extra_Text,
               &TChina_Reset_Text,
               &TChina_Exit_Text
      };

      BYTE code Korea_Luminace_Text[]=          {    Korea_Luminace                                    };
      BYTE code Korea_Imagesetup_Text[]=        {    Korea_Imagesetup                                  };
      BYTE code Korea_Colortemp_Text[]=         {    Korea_Colortemp                                   };
      BYTE code Korea_Colorboost_Text[]=        {    Korea_Colorboost                                  };
      BYTE code Korea_Pictureboost_Text[]=      {    Korea_Pictureboost                                };
      BYTE code Korea_Osdsetup_Text[]=          {    Korea_Osdsetup                                    };
      BYTE code Korea_Extra_Text[]=             {    Korea_Extra                                       };
      BYTE code Korea_Reset_Text[]=             {    Korea_Reset                                       };
      BYTE code Korea_Exit_Text[]=              {    Korea_Exit                                        };

      BYTE* code  Language_Korea_Table[]=
      {
               &Korea_Luminace_Text,
               &Korea_Imagesetup_Text,
               &Korea_Colortemp_Text,
               &Korea_Colorboost_Text,
               &Korea_Pictureboost_Text,
               &Korea_Osdsetup_Text,
               &Korea_Extra_Text,
               &Korea_Reset_Text,
               &Korea_Exit_Text
      };



      BYTE* code  Language_Table[]=
      {
                &Language_English_Table,
                &Language_Spanish_Table,
                &Language_France_Table,
                &Language_Portugues_Table,
                &Language_Russia_Table,
                &Language_Korea_Table,
                &Language_TChina_Table,
                &Language_SChina_Table
                
                
      };

#elif LANGUAGE_TYPE==All13Language

BYTE code German_Luminace_Text[]=	   {	German_Luminace 							   };
	 BYTE code German_Imagesetup_Text[]=	{	 German_Imagesetup								};
	 BYTE code German_Colortemp_Text[]= 	{	 German_Colortemp								};
	 BYTE code German_Colorboost_Text[]=	{	 German_Colorboost								};
	 BYTE code German_Pictureboost_Text[]=	{	 German_Pictureboost							};
	 BYTE code German_Osdsetup_Text[]=		{	 German_Osdsetup								};
	 BYTE code German_Extra_Text[]= 		{	 German_Extra									};
	 BYTE code German_Reset_Text[]= 		{	 German_Reset									};
	 BYTE code German_Exit_Text[]=			{	 German_Exit									};

	 BYTE* code  Language_German_Table[]=
	 {
			  &German_Luminace_Text,
			  &German_Imagesetup_Text,
			  &German_Colortemp_Text,
			  &German_Colorboost_Text,
			  &German_Pictureboost_Text,
			  &German_Osdsetup_Text,
			  &German_Extra_Text,
			  &German_Reset_Text,
			  &German_Exit_Text
	 };

	 BYTE code Italian_Luminace_Text[]= 	 {	  Italian_Luminace								  };
	 BYTE code Italian_Imagesetup_Text[]=	 {	  Italian_Imagesetup							  };
	 BYTE code Italian_Colortemp_Text[]=	 {	  Italian_Colortemp 							  };
	 BYTE code Italian_Colorboost_Text[]=	 {	  Italian_Colorboost							  };
	 BYTE code Italian_Pictureboost_Text[]=  {	  Italian_Pictureboost							  };
	 BYTE code Italian_Osdsetup_Text[]= 	 {	  Italian_Osdsetup								  };
	 BYTE code Italian_Extra_Text[]=		 {	  Italian_Extra 								  };
	 BYTE code Italian_Reset_Text[]=		 {	  Italian_Reset 								  };
	 BYTE code Italian_Exit_Text[]= 		 {	  Italian_Exit									  };

	 BYTE* code  Language_Italian_Table[]=
	 {
			  &Italian_Luminace_Text,
			  &Italian_Imagesetup_Text,
			  &Italian_Colortemp_Text,
			  &Italian_Colorboost_Text,
			  &Italian_Pictureboost_Text,
			  &Italian_Osdsetup_Text,
			  &Italian_Extra_Text,
			  &Italian_Reset_Text,
			  &Italian_Exit_Text
	 };


BYTE code Swedish_Luminace_Text[]=      {    Swedish_Luminace                                };
      BYTE code Swedish_Imagesetup_Text[]=    {    Swedish_Imagesetup                              };
      BYTE code Swedish_Colortemp_Text[]=     {    Swedish_Colortemp                               };
      BYTE code Swedish_Colorboost_Text[]=    {    Swedish_Colorboost                              };
      BYTE code Swedish_Pictureboost_Text[]=  {    Swedish_Pictureboost                            };
      BYTE code Swedish_Osdsetup_Text[]=      {    Swedish_Osdsetup                                };
      BYTE code Swedish_Extra_Text[]=         {    Swedish_Extra                                   };
      BYTE code Swedish_Reset_Text[]=         {    Swedish_Reset                                   };
      BYTE code Swedish_Exit_Text[]=          {    Swedish_Exit                                    };

      BYTE* code  Language_Swedish_Table[]=
      {
               &Swedish_Luminace_Text,
               &Swedish_Imagesetup_Text,
               &Swedish_Colortemp_Text,
               &Swedish_Colorboost_Text,
               &Swedish_Pictureboost_Text,
               &Swedish_Osdsetup_Text,
               &Swedish_Extra_Text,
               &Swedish_Reset_Text,
               &Swedish_Exit_Text
      };

      BYTE code Polish_Luminace_Text[]=      {    Polish_Luminace                                };
      BYTE code Polish_Imagesetup_Text[]=    {    Polish_Imagesetup                              };
      BYTE code Polish_Colortemp_Text[]=     {    Polish_Colortemp                               };
      BYTE code Polish_Colorboost_Text[]=    {    Polish_Colorboost                              };
      BYTE code Polish_Pictureboost_Text[]=  {    Polish_Pictureboost                            };
      BYTE code Polish_Osdsetup_Text[]=      {    Polish_Osdsetup                                };
      BYTE code Polish_Extra_Text[]=         {    Polish_Extra                                   };
      BYTE code Polish_Reset_Text[]=         {    Polish_Reset                                   };
      BYTE code Polish_Exit_Text[]=          {    Polish_Exit                                    };


      BYTE* code  Language_Polish_Table[]=
      {
               &Polish_Luminace_Text,
               &Polish_Imagesetup_Text,
               &Polish_Colortemp_Text,
               &Polish_Colorboost_Text,
               &Polish_Pictureboost_Text,
               &Polish_Osdsetup_Text,
               &Polish_Extra_Text,
               &Polish_Reset_Text,
               &Polish_Exit_Text
      };


      BYTE code Czech_Luminace_Text[]=      {    Czech_Luminace                                };
      BYTE code Czech_Imagesetup_Text[]=    {    Czech_Imagesetup                              };
      BYTE code Czech_Colortemp_Text[]=     {    Czech_Colortemp                               };
      BYTE code Czech_Colorboost_Text[]=    {    Czech_Colorboost                              };
      BYTE code Czech_Pictureboost_Text[]=  {    Czech_Pictureboost                            };
      BYTE code Czech_Osdsetup_Text[]=      {    Czech_Osdsetup                                };
      BYTE code Czech_Extra_Text[]=         {    Czech_Extra                                   };
      BYTE code Czech_Reset_Text[]=         {    Czech_Reset                                   };
      BYTE code Czech_Exit_Text[]=          {    Czech_Exit                                    };



      BYTE* code  Language_Czech_Table[]=
      {
               &Czech_Luminace_Text,
               &Czech_Imagesetup_Text,
               &Czech_Colortemp_Text,
               &Czech_Colorboost_Text,
               &Czech_Pictureboost_Text,
               &Czech_Osdsetup_Text,
               &Czech_Extra_Text,
               &Czech_Reset_Text,
               &Czech_Exit_Text
      };


BYTE code Portugues_Luminace_Text[]=	  {    Portugues_Luminace								 };
BYTE code Portugues_Imagesetup_Text[]=	  {    Portugues_Imagesetup 							 };
BYTE code Portugues_Colortemp_Text[]=	  {    Portugues_Colortemp								 };
BYTE code Portugues_Colorboost_Text[]=	  {    Portugues_Colorboost 							 };
BYTE code Portugues_Pictureboost_Text[]=  {    Portugues_Pictureboost							 };
BYTE code Portugues_Osdsetup_Text[]=	  {    Portugues_Osdsetup								 };
BYTE code Portugues_Extra_Text[]=		  {    Portugues_Extra									 };
BYTE code Portugues_Reset_Text[]=		  {    Portugues_Reset									 };
BYTE code Portugues_Exit_Text[]=		  {    Portugues_Exit									 };

BYTE* code	Language_Portugues_Table[]=
{
		 &Portugues_Luminace_Text,
		 &Portugues_Imagesetup_Text,
		 &Portugues_Colortemp_Text,
		 &Portugues_Colorboost_Text,
		 &Portugues_Pictureboost_Text,
		 &Portugues_Osdsetup_Text,
		 &Portugues_Extra_Text,
		 &Portugues_Reset_Text,
		 &Portugues_Exit_Text
};

BYTE code SChina_Luminace_Text[]=		  {    SChina_Luminace									 };
BYTE code SChina_Imagesetup_Text[]= 	  {    SChina_Imagesetup								 };
BYTE code SChina_Colortemp_Text[]=		  {    SChina_Colortemp 								 };
BYTE code SChina_Colorboost_Text[]= 	  {    SChina_Colorboost								 };
BYTE code SChina_Pictureboost_Text[]=	  {    SChina_Pictureboost								 };
BYTE code SChina_Osdsetup_Text[]=		  {    SChina_Osdsetup									 };
BYTE code SChina_Extra_Text[]=			  {    SChina_Extra 									 };
BYTE code SChina_Reset_Text[]=			  {    SChina_Reset 									 };
BYTE code SChina_Exit_Text[]=			  {    SChina_Exit										 };

BYTE* code	Language_SChina_Table[]=
{
		 &SChina_Luminace_Text,
		 &SChina_Imagesetup_Text,
		 &SChina_Colortemp_Text,
		 &SChina_Colorboost_Text,
		 &SChina_Pictureboost_Text,
		 &SChina_Osdsetup_Text,
		 &SChina_Extra_Text,
		 &SChina_Reset_Text,
		 &SChina_Exit_Text
};

BYTE code TChina_Luminace_Text[]=		  {    TChina_Luminace									 };
BYTE code TChina_Imagesetup_Text[]= 	  {    TChina_Imagesetup								 };
BYTE code TChina_Colortemp_Text[]=		  {    TChina_Colortemp 								 };
BYTE code TChina_Colorboost_Text[]= 	  {    TChina_Colorboost								 };
BYTE code TChina_Pictureboost_Text[]=	  {    TChina_Pictureboost								 };
BYTE code TChina_Osdsetup_Text[]=		  {    TChina_Osdsetup									 };
BYTE code TChina_Extra_Text[]=			  {    TChina_Extra 									 };
BYTE code TChina_Reset_Text[]=			  {    TChina_Reset 									 };
BYTE code TChina_Exit_Text[]=			  {    TChina_Exit										 };

BYTE* code	Language_TChina_Table[]=
{
		 &TChina_Luminace_Text,
		 &TChina_Imagesetup_Text,
		 &TChina_Colortemp_Text,
		 &TChina_Colorboost_Text,
		 &TChina_Pictureboost_Text,
		 &TChina_Osdsetup_Text,
		 &TChina_Extra_Text,
		 &TChina_Reset_Text,
		 &TChina_Exit_Text
};

BYTE code Korea_Luminace_Text[]=		  {    Korea_Luminace									 };
BYTE code Korea_Imagesetup_Text[]=		  {    Korea_Imagesetup 								 };
BYTE code Korea_Colortemp_Text[]=		  {    Korea_Colortemp									 };
BYTE code Korea_Colorboost_Text[]=		  {    Korea_Colorboost 								 };
BYTE code Korea_Pictureboost_Text[]=	  {    Korea_Pictureboost								 };
BYTE code Korea_Osdsetup_Text[]=		  {    Korea_Osdsetup									 };
BYTE code Korea_Extra_Text[]=			  {    Korea_Extra										 };
BYTE code Korea_Reset_Text[]=			  {    Korea_Reset										 };
BYTE code Korea_Exit_Text[]=			  {    Korea_Exit										 };

BYTE* code	Language_Korea_Table[]=
{
		 &Korea_Luminace_Text,
		 &Korea_Imagesetup_Text,
		 &Korea_Colortemp_Text,
		 &Korea_Colorboost_Text,
		 &Korea_Pictureboost_Text,
		 &Korea_Osdsetup_Text,
		 &Korea_Extra_Text,
		 &Korea_Reset_Text,
		 &Korea_Exit_Text
};



BYTE* code	Language_Table[]=
{
		  &Language_English_Table,
		  &Language_German_Table,
		  &Language_France_Table,
		  &Language_Italian_Table,
		  &Language_Spanish_Table,
		  &Language_Swedish_Table, 
		  &Language_Polish_Table, 
		  &Language_Czech_Table, 
		  &Language_Russia_Table,
		  &Language_Portugues_Table,
		  &Language_Korea_Table,
		  &Language_TChina_Table,
		  &Language_SChina_Table
	  
};

#elif LANGUAGE_TYPE==All14Language_AddJap

BYTE code German_Luminace_Text[]=	   {	German_Luminace 							   };
	 BYTE code German_Imagesetup_Text[]=	{	 German_Imagesetup								};
	 BYTE code German_Colortemp_Text[]= 	{	 German_Colortemp								};
	 BYTE code German_Colorboost_Text[]=	{	 German_Colorboost								};
	 BYTE code German_Pictureboost_Text[]=	{	 German_Pictureboost							};
	 BYTE code German_Osdsetup_Text[]=		{	 German_Osdsetup								};
	 BYTE code German_Extra_Text[]= 		{	 German_Extra									};
	 BYTE code German_Reset_Text[]= 		{	 German_Reset									};
	 BYTE code German_Exit_Text[]=			{	 German_Exit									};

	 BYTE* code  Language_German_Table[]=
	 {
			  &German_Luminace_Text,
			  &German_Imagesetup_Text,
			  &German_Colortemp_Text,
			  &German_Colorboost_Text,
			  &German_Pictureboost_Text,
			  &German_Osdsetup_Text,
			  &German_Extra_Text,
			  &German_Reset_Text,
			  &German_Exit_Text
	 };

	 BYTE code Italian_Luminace_Text[]= 	 {	  Italian_Luminace								  };
	 BYTE code Italian_Imagesetup_Text[]=	 {	  Italian_Imagesetup							  };
	 BYTE code Italian_Colortemp_Text[]=	 {	  Italian_Colortemp 							  };
	 BYTE code Italian_Colorboost_Text[]=	 {	  Italian_Colorboost							  };
	 BYTE code Italian_Pictureboost_Text[]=  {	  Italian_Pictureboost							  };
	 BYTE code Italian_Osdsetup_Text[]= 	 {	  Italian_Osdsetup								  };
	 BYTE code Italian_Extra_Text[]=		 {	  Italian_Extra 								  };
	 BYTE code Italian_Reset_Text[]=		 {	  Italian_Reset 								  };
	 BYTE code Italian_Exit_Text[]= 		 {	  Italian_Exit									  };

	 BYTE* code  Language_Italian_Table[]=
	 {
			  &Italian_Luminace_Text,
			  &Italian_Imagesetup_Text,
			  &Italian_Colortemp_Text,
			  &Italian_Colorboost_Text,
			  &Italian_Pictureboost_Text,
			  &Italian_Osdsetup_Text,
			  &Italian_Extra_Text,
			  &Italian_Reset_Text,
			  &Italian_Exit_Text
	 };


BYTE code Swedish_Luminace_Text[]=      {    Swedish_Luminace                                };
      BYTE code Swedish_Imagesetup_Text[]=    {    Swedish_Imagesetup                              };
      BYTE code Swedish_Colortemp_Text[]=     {    Swedish_Colortemp                               };
      BYTE code Swedish_Colorboost_Text[]=    {    Swedish_Colorboost                              };
      BYTE code Swedish_Pictureboost_Text[]=  {    Swedish_Pictureboost                            };
      BYTE code Swedish_Osdsetup_Text[]=      {    Swedish_Osdsetup                                };
      BYTE code Swedish_Extra_Text[]=         {    Swedish_Extra                                   };
      BYTE code Swedish_Reset_Text[]=         {    Swedish_Reset                                   };
      BYTE code Swedish_Exit_Text[]=          {    Swedish_Exit                                    };

      BYTE* code  Language_Swedish_Table[]=
      {
               &Swedish_Luminace_Text,
               &Swedish_Imagesetup_Text,
               &Swedish_Colortemp_Text,
               &Swedish_Colorboost_Text,
               &Swedish_Pictureboost_Text,
               &Swedish_Osdsetup_Text,
               &Swedish_Extra_Text,
               &Swedish_Reset_Text,
               &Swedish_Exit_Text
      };

      BYTE code Polish_Luminace_Text[]=      {    Polish_Luminace                                };
      BYTE code Polish_Imagesetup_Text[]=    {    Polish_Imagesetup                              };
      BYTE code Polish_Colortemp_Text[]=     {    Polish_Colortemp                               };
      BYTE code Polish_Colorboost_Text[]=    {    Polish_Colorboost                              };
      BYTE code Polish_Pictureboost_Text[]=  {    Polish_Pictureboost                            };
      BYTE code Polish_Osdsetup_Text[]=      {    Polish_Osdsetup                                };
      BYTE code Polish_Extra_Text[]=         {    Polish_Extra                                   };
      BYTE code Polish_Reset_Text[]=         {    Polish_Reset                                   };
      BYTE code Polish_Exit_Text[]=          {    Polish_Exit                                    };


      BYTE* code  Language_Polish_Table[]=
      {
               &Polish_Luminace_Text,
               &Polish_Imagesetup_Text,
               &Polish_Colortemp_Text,
               &Polish_Colorboost_Text,
               &Polish_Pictureboost_Text,
               &Polish_Osdsetup_Text,
               &Polish_Extra_Text,
               &Polish_Reset_Text,
               &Polish_Exit_Text
      };


      BYTE code Czech_Luminace_Text[]=      {    Czech_Luminace                                };
      BYTE code Czech_Imagesetup_Text[]=    {    Czech_Imagesetup                              };
      BYTE code Czech_Colortemp_Text[]=     {    Czech_Colortemp                               };
      BYTE code Czech_Colorboost_Text[]=    {    Czech_Colorboost                              };
      BYTE code Czech_Pictureboost_Text[]=  {    Czech_Pictureboost                            };
      BYTE code Czech_Osdsetup_Text[]=      {    Czech_Osdsetup                                };
      BYTE code Czech_Extra_Text[]=         {    Czech_Extra                                   };
      BYTE code Czech_Reset_Text[]=         {    Czech_Reset                                   };
      BYTE code Czech_Exit_Text[]=          {    Czech_Exit                                    };



      BYTE* code  Language_Czech_Table[]=
      {
               &Czech_Luminace_Text,
               &Czech_Imagesetup_Text,
               &Czech_Colortemp_Text,
               &Czech_Colorboost_Text,
               &Czech_Pictureboost_Text,
               &Czech_Osdsetup_Text,
               &Czech_Extra_Text,
               &Czech_Reset_Text,
               &Czech_Exit_Text
      };


BYTE code Portugues_Luminace_Text[]=	  {    Portugues_Luminace								 };
BYTE code Portugues_Imagesetup_Text[]=	  {    Portugues_Imagesetup 							 };
BYTE code Portugues_Colortemp_Text[]=	  {    Portugues_Colortemp								 };
BYTE code Portugues_Colorboost_Text[]=	  {    Portugues_Colorboost 							 };
BYTE code Portugues_Pictureboost_Text[]=  {    Portugues_Pictureboost							 };
BYTE code Portugues_Osdsetup_Text[]=	  {    Portugues_Osdsetup								 };
BYTE code Portugues_Extra_Text[]=		  {    Portugues_Extra									 };
BYTE code Portugues_Reset_Text[]=		  {    Portugues_Reset									 };
BYTE code Portugues_Exit_Text[]=		  {    Portugues_Exit									 };

BYTE* code	Language_Portugues_Table[]=
{
		 &Portugues_Luminace_Text,
		 &Portugues_Imagesetup_Text,
		 &Portugues_Colortemp_Text,
		 &Portugues_Colorboost_Text,
		 &Portugues_Pictureboost_Text,
		 &Portugues_Osdsetup_Text,
		 &Portugues_Extra_Text,
		 &Portugues_Reset_Text,
		 &Portugues_Exit_Text
};

BYTE code SChina_Luminace_Text[]=		  {    SChina_Luminace									 };
BYTE code SChina_Imagesetup_Text[]= 	  {    SChina_Imagesetup								 };
BYTE code SChina_Colortemp_Text[]=		  {    SChina_Colortemp 								 };
BYTE code SChina_Colorboost_Text[]= 	  {    SChina_Colorboost								 };
BYTE code SChina_Pictureboost_Text[]=	  {    SChina_Pictureboost								 };
BYTE code SChina_Osdsetup_Text[]=		  {    SChina_Osdsetup									 };
BYTE code SChina_Extra_Text[]=			  {    SChina_Extra 									 };
BYTE code SChina_Reset_Text[]=			  {    SChina_Reset 									 };
BYTE code SChina_Exit_Text[]=			  {    SChina_Exit										 };

BYTE* code	Language_SChina_Table[]=
{
		 &SChina_Luminace_Text,
		 &SChina_Imagesetup_Text,
		 &SChina_Colortemp_Text,
		 &SChina_Colorboost_Text,
		 &SChina_Pictureboost_Text,
		 &SChina_Osdsetup_Text,
		 &SChina_Extra_Text,
		 &SChina_Reset_Text,
		 &SChina_Exit_Text
};

BYTE code TChina_Luminace_Text[]=		  {    TChina_Luminace									 };
BYTE code TChina_Imagesetup_Text[]= 	  {    TChina_Imagesetup								 };
BYTE code TChina_Colortemp_Text[]=		  {    TChina_Colortemp 								 };
BYTE code TChina_Colorboost_Text[]= 	  {    TChina_Colorboost								 };
BYTE code TChina_Pictureboost_Text[]=	  {    TChina_Pictureboost								 };
BYTE code TChina_Osdsetup_Text[]=		  {    TChina_Osdsetup									 };
BYTE code TChina_Extra_Text[]=			  {    TChina_Extra 									 };
BYTE code TChina_Reset_Text[]=			  {    TChina_Reset 									 };
BYTE code TChina_Exit_Text[]=			  {    TChina_Exit										 };

BYTE* code	Language_TChina_Table[]=
{
		 &TChina_Luminace_Text,
		 &TChina_Imagesetup_Text,
		 &TChina_Colortemp_Text,
		 &TChina_Colorboost_Text,
		 &TChina_Pictureboost_Text,
		 &TChina_Osdsetup_Text,
		 &TChina_Extra_Text,
		 &TChina_Reset_Text,
		 &TChina_Exit_Text
};

BYTE code Korea_Luminace_Text[]=		  {    Korea_Luminace									 };
BYTE code Korea_Imagesetup_Text[]=		  {    Korea_Imagesetup 								 };
BYTE code Korea_Colortemp_Text[]=		  {    Korea_Colortemp									 };
BYTE code Korea_Colorboost_Text[]=		  {    Korea_Colorboost 								 };
BYTE code Korea_Pictureboost_Text[]=	  {    Korea_Pictureboost								 };
BYTE code Korea_Osdsetup_Text[]=		  {    Korea_Osdsetup									 };
BYTE code Korea_Extra_Text[]=			  {    Korea_Extra										 };
BYTE code Korea_Reset_Text[]=			  {    Korea_Reset										 };
BYTE code Korea_Exit_Text[]=			  {    Korea_Exit										 };

BYTE* code	Language_Korea_Table[]=
{
		 &Korea_Luminace_Text,
		 &Korea_Imagesetup_Text,
		 &Korea_Colortemp_Text,
		 &Korea_Colorboost_Text,
		 &Korea_Pictureboost_Text,
		 &Korea_Osdsetup_Text,
		 &Korea_Extra_Text,
		 &Korea_Reset_Text,
		 &Korea_Exit_Text
};

BYTE code Japan_Luminace_Text[]=		  {    Japan_Luminace									 };
BYTE code Japan_Imagesetup_Text[]=		  {    Japan_Imagesetup 								 };
BYTE code Japan_Colortemp_Text[]=		  {    Japan_Colortemp									 };
BYTE code Japan_Colorboost_Text[]=		  {    Japan_Colorboost 								 };
BYTE code Japan_Pictureboost_Text[]=	  {    Japan_Pictureboost								 };
BYTE code Japan_Osdsetup_Text[]=		  {    Japan_Osdsetup									 };
BYTE code Japan_Extra_Text[]=			  {    Japan_Extra										 };
BYTE code Japan_Reset_Text[]=			  {    Japan_Reset										 };
BYTE code Japan_Exit_Text[]=			  {    Japan_Exit										 };

BYTE* code	Language_Japan_Table[]=
{
		 &Japan_Luminace_Text,
		 &Japan_Imagesetup_Text,
		 &Japan_Colortemp_Text,
		 &Japan_Colorboost_Text,
		 &Japan_Pictureboost_Text,
		 &Japan_Osdsetup_Text,
		 &Japan_Extra_Text,
		 &Japan_Reset_Text,
		 &Japan_Exit_Text
};


BYTE* code	Language_Table[]=
{
		  &Language_English_Table,
		  &Language_German_Table,
		  &Language_France_Table,
		  &Language_Italian_Table,
		  &Language_Spanish_Table,
		  &Language_Swedish_Table, 
		  &Language_Polish_Table, 
		  &Language_Czech_Table, 
		  &Language_Russia_Table,
		  &Language_Portugues_Table,
		  &Language_Korea_Table,
		  &Language_TChina_Table,
		  &Language_SChina_Table,
	  	  &Language_Japan_Table
};

#else

      BYTE code German_Luminace_Text[]=      {    German_Luminace                                };
      BYTE code German_Imagesetup_Text[]=    {    German_Imagesetup                              };
      BYTE code German_Colortemp_Text[]=     {    German_Colortemp                               };
      BYTE code German_Colorboost_Text[]=    {    German_Colorboost                              };
      BYTE code German_Pictureboost_Text[]=  {    German_Pictureboost                            };
      BYTE code German_Osdsetup_Text[]=      {    German_Osdsetup                                };
      BYTE code German_Extra_Text[]=         {    German_Extra                                   };
      BYTE code German_Reset_Text[]=         {    German_Reset                                   };
      BYTE code German_Exit_Text[]=          {    German_Exit                                    };

      BYTE* code  Language_German_Table[]=
      {
               &German_Luminace_Text,
               &German_Imagesetup_Text,
               &German_Colortemp_Text,
               &German_Colorboost_Text,
               &German_Pictureboost_Text,
               &German_Osdsetup_Text,
               &German_Extra_Text,
               &German_Reset_Text,
               &German_Exit_Text
      };

      BYTE code Italian_Luminace_Text[]=      {    Italian_Luminace                                };
      BYTE code Italian_Imagesetup_Text[]=    {    Italian_Imagesetup                              };
      BYTE code Italian_Colortemp_Text[]=     {    Italian_Colortemp                               };
      BYTE code Italian_Colorboost_Text[]=    {    Italian_Colorboost                              };
      BYTE code Italian_Pictureboost_Text[]=  {    Italian_Pictureboost                            };
      BYTE code Italian_Osdsetup_Text[]=      {    Italian_Osdsetup                                };
      BYTE code Italian_Extra_Text[]=         {    Italian_Extra                                   };
      BYTE code Italian_Reset_Text[]=         {    Italian_Reset                                   };
      BYTE code Italian_Exit_Text[]=          {    Italian_Exit                                    };

      BYTE* code  Language_Italian_Table[]=
      {
               &Italian_Luminace_Text,
               &Italian_Imagesetup_Text,
               &Italian_Colortemp_Text,
               &Italian_Colorboost_Text,
               &Italian_Pictureboost_Text,
               &Italian_Osdsetup_Text,
               &Italian_Extra_Text,
               &Italian_Reset_Text,
               &Italian_Exit_Text
      };

#if LANGUAGE_TYPE == EUROPE

      BYTE code Swedish_Luminace_Text[]=      {    Swedish_Luminace                                };
      BYTE code Swedish_Imagesetup_Text[]=    {    Swedish_Imagesetup                              };
      BYTE code Swedish_Colortemp_Text[]=     {    Swedish_Colortemp                               };
      BYTE code Swedish_Colorboost_Text[]=    {    Swedish_Colorboost                              };
      BYTE code Swedish_Pictureboost_Text[]=  {    Swedish_Pictureboost                            };
      BYTE code Swedish_Osdsetup_Text[]=      {    Swedish_Osdsetup                                };
      BYTE code Swedish_Extra_Text[]=         {    Swedish_Extra                                   };
      BYTE code Swedish_Reset_Text[]=         {    Swedish_Reset                                   };
      BYTE code Swedish_Exit_Text[]=          {    Swedish_Exit                                    };

      BYTE* code  Language_Swedish_Table[]=
      {
               &Swedish_Luminace_Text,
               &Swedish_Imagesetup_Text,
               &Swedish_Colortemp_Text,
               &Swedish_Colorboost_Text,
               &Swedish_Pictureboost_Text,
               &Swedish_Osdsetup_Text,
               &Swedish_Extra_Text,
               &Swedish_Reset_Text,
               &Swedish_Exit_Text
      };

      BYTE code Polish_Luminace_Text[]=      {    Polish_Luminace                                };
      BYTE code Polish_Imagesetup_Text[]=    {    Polish_Imagesetup                              };
      BYTE code Polish_Colortemp_Text[]=     {    Polish_Colortemp                               };
      BYTE code Polish_Colorboost_Text[]=    {    Polish_Colorboost                              };
      BYTE code Polish_Pictureboost_Text[]=  {    Polish_Pictureboost                            };
      BYTE code Polish_Osdsetup_Text[]=      {    Polish_Osdsetup                                };
      BYTE code Polish_Extra_Text[]=         {    Polish_Extra                                   };
      BYTE code Polish_Reset_Text[]=         {    Polish_Reset                                   };
      BYTE code Polish_Exit_Text[]=          {    Polish_Exit                                    };


      BYTE* code  Language_Polish_Table[]=
      {
               &Polish_Luminace_Text,
               &Polish_Imagesetup_Text,
               &Polish_Colortemp_Text,
               &Polish_Colorboost_Text,
               &Polish_Pictureboost_Text,
               &Polish_Osdsetup_Text,
               &Polish_Extra_Text,
               &Polish_Reset_Text,
               &Polish_Exit_Text
      };


      BYTE code Czech_Luminace_Text[]=      {    Czech_Luminace                                };
      BYTE code Czech_Imagesetup_Text[]=    {    Czech_Imagesetup                              };
      BYTE code Czech_Colortemp_Text[]=     {    Czech_Colortemp                               };
      BYTE code Czech_Colorboost_Text[]=    {    Czech_Colorboost                              };
      BYTE code Czech_Pictureboost_Text[]=  {    Czech_Pictureboost                            };
      BYTE code Czech_Osdsetup_Text[]=      {    Czech_Osdsetup                                };
      BYTE code Czech_Extra_Text[]=         {    Czech_Extra                                   };
      BYTE code Czech_Reset_Text[]=         {    Czech_Reset                                   };
      BYTE code Czech_Exit_Text[]=          {    Czech_Exit                                    };



      BYTE* code  Language_Czech_Table[]=
      {
               &Czech_Luminace_Text,
               &Czech_Imagesetup_Text,
               &Czech_Colortemp_Text,
               &Czech_Colorboost_Text,
               &Czech_Pictureboost_Text,
               &Czech_Osdsetup_Text,
               &Czech_Extra_Text,
               &Czech_Reset_Text,
               &Czech_Exit_Text
      };


        BYTE* code  Language_Table[]=
      {
                &Language_English_Table,
                &Language_France_Table,
                &Language_Spanish_Table,
                &Language_German_Table,
                &Language_Russia_Table,
                &Language_Italian_Table,
                &Language_Swedish_Table,
                &Language_Polish_Table,
                &Language_Czech_Table
      };

#else

      BYTE code Portugues_Luminace_Text[]=      {    Portugues_Luminace                                };
      BYTE code Portugues_Imagesetup_Text[]=    {    Portugues_Imagesetup                              };
      BYTE code Portugues_Colortemp_Text[]=     {    Portugues_Colortemp                               };
      BYTE code Portugues_Colorboost_Text[]=    {    Portugues_Colorboost                              };
      BYTE code Portugues_Pictureboost_Text[]=  {    Portugues_Pictureboost                            };
      BYTE code Portugues_Osdsetup_Text[]=      {    Portugues_Osdsetup                                };
      BYTE code Portugues_Extra_Text[]=         {    Portugues_Extra                                   };
      BYTE code Portugues_Reset_Text[]=         {    Portugues_Reset                                   };
      BYTE code Portugues_Exit_Text[]=          {    Portugues_Exit                                    };

      BYTE* code  Language_Portugues_Table[]=
      {
               &Portugues_Luminace_Text,
               &Portugues_Imagesetup_Text,
               &Portugues_Colortemp_Text,
               &Portugues_Colorboost_Text,
               &Portugues_Pictureboost_Text,
               &Portugues_Osdsetup_Text,
               &Portugues_Extra_Text,
               &Portugues_Reset_Text,
               &Portugues_Exit_Text
      };

      BYTE code SChina_Luminace_Text[]=         {    SChina_Luminace                                   };
      BYTE code SChina_Imagesetup_Text[]=       {    SChina_Imagesetup                                 };
      BYTE code SChina_Colortemp_Text[]=        {    SChina_Colortemp                                  };
      BYTE code SChina_Colorboost_Text[]=       {    SChina_Colorboost                                 };
      BYTE code SChina_Pictureboost_Text[]=     {    SChina_Pictureboost                               };
      BYTE code SChina_Osdsetup_Text[]=         {    SChina_Osdsetup                                   };
      BYTE code SChina_Extra_Text[]=            {    SChina_Extra                                      };
      BYTE code SChina_Reset_Text[]=            {    SChina_Reset                                      };
      BYTE code SChina_Exit_Text[]=             {    SChina_Exit                                       };

      BYTE* code  Language_SChina_Table[]=
      {
               &SChina_Luminace_Text,
               &SChina_Imagesetup_Text,
               &SChina_Colortemp_Text,
               &SChina_Colorboost_Text,
               &SChina_Pictureboost_Text,
               &SChina_Osdsetup_Text,
               &SChina_Extra_Text,
               &SChina_Reset_Text,
               &SChina_Exit_Text
      };


        BYTE* code  Language_Table[]=
      {
                &Language_English_Table,
                &Language_Spanish_Table,
                &Language_France_Table,
                &Language_German_Table,
                &Language_Italian_Table,
                &Language_Portugues_Table,
                &Language_Russia_Table,
                &Language_SChina_Table
      };

#endif

#endif


//===============================================================================================================================================================================
//===============================================================================================================================================================================

