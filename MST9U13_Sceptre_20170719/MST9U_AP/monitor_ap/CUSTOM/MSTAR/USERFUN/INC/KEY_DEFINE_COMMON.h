


//SAR_KEY_VALUE
#if (defined(KTC_Project))
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x56
#define ADCKEY_A2_VALUE     0xA3

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x56
#define ADCKEY_B2_VALUE     0xA3

#elif (defined(HKC_Project))
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x56
#define ADCKEY_A2_VALUE     0xB8

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x56
#define ADCKEY_B2_VALUE     0xB8

#elif (defined(LianHeChuangXin_Project))
#if (MS_BOARD_TYPE_SEL==BD_MST9UHL_V6)
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x56
#define ADCKEY_A2_VALUE     0xB8

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x56
#define ADCKEY_B2_VALUE     0xB8
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHD_BV1||MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1||MS_BOARD_TYPE_SEL==BD_MST9UHD_GV1)
  #define ADCKEY_A0_VALUE     0x00
  #define ADCKEY_A1_VALUE     0x54
  #define ADCKEY_A2_VALUE     0xB7
  
  #define ADCKEY_B0_VALUE     0x00
  #define ADCKEY_B1_VALUE     0x45
  #define ADCKEY_B2_VALUE     0xBF
#else
     #if defined(GK3B_Project)
     #define ADCKEY_A0_VALUE     0x00
     #define ADCKEY_A1_VALUE     0x55
     #define ADCKEY_A2_VALUE     0xC1
     
     #define ADCKEY_B0_VALUE     0x00
     #define ADCKEY_B1_VALUE     0x46
     #define ADCKEY_B2_VALUE     0xBF

     #elif 0//Dayu
     #define ADCKEY_A0_VALUE                                         0x8C
     #define ADCKEY_A1_VALUE                                         0xB8
     #define ADCKEY_A2_VALUE                                         0xE2
     
     #define ADCKEY_B0_VALUE                                         0x8C
     #define ADCKEY_B1_VALUE                                         0xB8
     #define ADCKEY_B2_VALUE                                         0xE2
     #else
     #define ADCKEY_A0_VALUE     0x00
     #define ADCKEY_A1_VALUE     0x55
     #define ADCKEY_A2_VALUE     0xB8
     
     #define ADCKEY_B0_VALUE     0x00
     #define ADCKEY_B1_VALUE     0x46
     #define ADCKEY_B2_VALUE     0xBF
     #endif
#endif
#elif (defined(DaYu_Project))
#define ADCKEY_A0_VALUE     0x8C
#define ADCKEY_A1_VALUE     0xB8
#define ADCKEY_A2_VALUE     0xEC

#define ADCKEY_B0_VALUE     0x8C
#define ADCKEY_B1_VALUE     0xB8
#define ADCKEY_B2_VALUE     0xEC
#elif ((defined(FengYuan_Project))&&(MS_BOARD_TYPE_SEL==BD_MST9UHL_V6))
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x56
#define ADCKEY_A2_VALUE     0xB8

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x56
#define ADCKEY_B2_VALUE     0xB8
#elif ((defined(HongHan_Project))&&(MS_BOARD_TYPE_SEL==BD_MST9UHL_V6))
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x56
#define ADCKEY_A2_VALUE     0xB8

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x56
#define ADCKEY_B2_VALUE     0xB8
#elif (defined(XuLi_Project))
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x46
#define ADCKEY_A2_VALUE     0x74

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x46
#define ADCKEY_B2_VALUE     0x74

#elif (defined(MingCai_Project))

#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x46
#define ADCKEY_A2_VALUE     0x74

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x46
#define ADCKEY_B2_VALUE     0x74

#elif (defined(KeOuShi_Project))
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x46
#define ADCKEY_A2_VALUE     0x74

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x46
#define ADCKEY_B2_VALUE     0x74

#elif (defined(ZhongXinKeJi_Project))
  #define ADCKEY_A0_VALUE     0x00
  #define ADCKEY_A1_VALUE     0x54
  #define ADCKEY_A2_VALUE     0xB7
  
  #define ADCKEY_B0_VALUE     0x00
  #define ADCKEY_B1_VALUE     0x45
  #define ADCKEY_B2_VALUE     0x74

#else
#define ADCKEY_A0_VALUE     0x00
#define ADCKEY_A1_VALUE     0x46
#define ADCKEY_A2_VALUE     0x78

#define ADCKEY_B0_VALUE     0x00
#define ADCKEY_B1_VALUE     0x46
#define ADCKEY_B2_VALUE     0x78
#endif
#define KEY_TOL                      8///4


#if (defined(LeXingEnMa_Project))
#define ADCKEY_A0       KEY_MINUS
#define ADCKEY_A1       KEY_MENU//KEY_INCVALUE
#define ADCKEY_A2       KEY_EXIT//KEY_DECVALUE

#define ADCKEY_B0       KEY_PLUS//KEY_EXIT
#define ADCKEY_B1       KEY_INCVALUE//KEY_PLUS
#define ADCKEY_B2       KEY_DECVALUE//KEY_MENU

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(SanSe_Project))
#define ADCKEY_A0       KEY_MINUS
#define ADCKEY_A1       KEY_INCVALUE
#define ADCKEY_A2       KEY_DECVALUE

#define ADCKEY_B0       KEY_EXIT
#define ADCKEY_B1       KEY_PLUS
#define ADCKEY_B2       KEY_MENU

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(HuiGe_Project))
#if New_Key_HuiGe
#define ADCKEY_A0       KEY_EXIT//KEY_MINUS
#define ADCKEY_A1       KEY_INCVALUE//KEY_MENU
#define ADCKEY_A2       KEY_MENU//KEY_EXIT

#define ADCKEY_B0       KEY_MINUS//KEY_PLUS
#define ADCKEY_B1       KEY_DECVALUE//KEY_INCVALUE
#define ADCKEY_B2       KEY_PLUS//KEY_DECVALUE

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#else
#define ADCKEY_A0       KEY_MINUS
#define ADCKEY_A1       KEY_MENU//KEY_INCVALUE
#define ADCKEY_A2       KEY_EXIT//KEY_DECVALUE

#define ADCKEY_B0       KEY_PLUS//KEY_EXIT
#define ADCKEY_B1       KEY_INCVALUE//KEY_PLUS
#define ADCKEY_B2       KEY_DECVALUE//

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#endif
#elif (defined(JieBo_Project))//
#define ADCKEY_A0       KEY_EXIT//KEY_MINUS
#define ADCKEY_A1       KEY_MENU//KEY_INCVALUE
#define ADCKEY_A2       KEY_MINUS//KEY_DECVALUE

#define ADCKEY_B0       KEY_INCVALUE//KEY_EXIT
#define ADCKEY_B1       KEY_PLUS
#define ADCKEY_B2       KEY_DECVALUE//KEY_MENU
#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(Kogan_Project))
#define ADCKEY_A0       KEY_MINUS//KEY_EXIT
#define ADCKEY_A1       KEY_MENU
#define ADCKEY_A2       KEY_PLUS//KEY_MINUS

#define ADCKEY_B0       KEY_INCVALUE
#define ADCKEY_B1       KEY_DECVALUE//KEY_PLUS
#define ADCKEY_B2       KEY_EXIT//KEY_DECVALUE
#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(JieBo_BangZi_Project))
#define ADCKEY_A0       KEY_MENU//KEY_EXIT
#define ADCKEY_A1       KEY_EXIT//KEY_MENU
#define ADCKEY_A2       KEY_INCVALUE//KEY_MINUS

#define ADCKEY_B0       KEY_PLUS//KEY_INCVALUE
#define ADCKEY_B1       KEY_DECVALUE//KEY_PLUS
#define ADCKEY_B2       KEY_MINUS//KEY_DECVALUE
#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(RX_Project))

#if 1
#define ADCKEY_A0       KEY_MINUS
#define ADCKEY_A1       KEY_DECVALUE
#define ADCKEY_A2       KEY_INCVALUE

#define ADCKEY_B0       KEY_PLUS
#define ADCKEY_B1       KEY_EXIT
#define ADCKEY_B2       KEY_MENU
#else
#define ADCKEY_A0       KEY_DECVALUE
#define ADCKEY_A1       KEY_INCVALUE
#define ADCKEY_A2       KEY_EXIT

#define ADCKEY_B0       KEY_MINUS
#define ADCKEY_B1       KEY_PLUS
#define ADCKEY_B2       KEY_MENU
#endif

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(KTC_Project))
#if (PanelType==PanelM315DJJ_K30)
#define ADCKEY_A0       KEY_EXIT
#define ADCKEY_A1       KEY_MENU
#define ADCKEY_A2       KEY_MINUS

#define ADCKEY_B0       KEY_NOTHING
#define ADCKEY_B1       KEY_NOTHING
#define ADCKEY_B2       KEY_PLUS

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))


#elif (PanelType==PanelLSM315HP01)
#define ADCKEY_A0        KEY_POWER
#define ADCKEY_A1       KEY_NOTHING
#define ADCKEY_A2       KEY_MENU//KEY_MINUS //KEY_EXIT

#define ADCKEY_B0       KEY_MINUS //KEY_EXIT //KEY_MINUS
#define ADCKEY_B1      KEY_PLUS  //KEY_MENU //KEY_PLUS
#define ADCKEY_B2       KEY_EXIT// KEY_PLUS  //KEY_MENU

#define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))



#elif (PanelType==PanelLC430EGY_SHA1)

#define ADCKEY_A0       KEY_PLUS
#define ADCKEY_A1       KEY_MINUS 
#define ADCKEY_A2       KEY_MENU//KEY_MINUS //KEY_EXIT

#define ADCKEY_B0       KEY_INCVALUE //KEY_EXIT //KEY_MINUS
#define ADCKEY_B1      KEY_DECVALUE  //KEY_MENU //KEY_PLUS
#define ADCKEY_B2       KEY_EXIT// KEY_PLUS  //KEY_MENU

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#else
#define ADCKEY_A0       KEY_POWER
#define ADCKEY_A1       KEY_NOTHING
#define ADCKEY_A2       KEY_EXIT

#define ADCKEY_B0       KEY_MINUS
#define ADCKEY_B1       KEY_PLUS
#define ADCKEY_B2       KEY_MENU

#define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#endif

#elif (defined(WanMei_Project))
#define ADCKEY_A0       KEY_MINUS
#define ADCKEY_A1       KEY_MENU//KEY_INCVALUE
#define ADCKEY_A2       KEY_EXIT//KEY_DECVALUE

#define ADCKEY_B0       KEY_PLUS//KEY_EXIT
#define ADCKEY_B1       KEY_INCVALUE//KEY_PLUS
#define ADCKEY_B2       KEY_DECVALUE//KEY_MENU

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(ZhouYang_Project))

#if (PanelType==PanelLSC400FN04)
#define ADCKEY_A0       KEY_PLUS//KEY_INCVALUE
#define ADCKEY_A1       KEY_INCVALUE
#define ADCKEY_A2       KEY_DECVALUE//KEY_PLUS

#define ADCKEY_B0       KEY_MINUS //KEY_DECVALUE
#define ADCKEY_B1       KEY_MENU
#define ADCKEY_B2       KEY_EXIT

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#else
#define ADCKEY_A0       KEY_INCVALUE
#define ADCKEY_A1       KEY_MINUS
#define ADCKEY_A2       KEY_PLUS

#define ADCKEY_B0       KEY_DECVALUE
#define ADCKEY_B1       KEY_MENU
#define ADCKEY_B2       KEY_EXIT

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#endif
#elif (defined(SenYo_Project))

#define ADCKEY_A0       KEY_MINUS//KEY_MINUS
#define ADCKEY_A1       KEY_PLUS//KEY_INCVALUE
#define ADCKEY_A2       KEY_EXIT// KEY_DECVALUE

#define ADCKEY_B0       KEY_INCVALUE//KEY_EXIT
#define ADCKEY_B1       KEY_DECVALUE
#define ADCKEY_B2       KEY_MENU

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(HuaKe_Project))

#define ADCKEY_A0       KEY_MENU
#define ADCKEY_A1       KEY_DECVALUE
#define ADCKEY_A2       KEY_PLUS

#define ADCKEY_B0       KEY_MINUS
#define ADCKEY_B1       KEY_INCVALUE
#define ADCKEY_B2       KEY_EXIT

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(SenPaiTe_Project))

#define ADCKEY_A0       KEY_MENU
#define ADCKEY_A1       KEY_INCVALUE
#define ADCKEY_A2       KEY_DECVALUE

#define ADCKEY_B0       KEY_EXIT
#define ADCKEY_B1       KEY_PLUS
#define ADCKEY_B2       KEY_MINUS

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(HKC_Project))

#define ADCKEY_A0       KEY_MINUS
#define ADCKEY_A1       KEY_INCVALUE
#define ADCKEY_A2       KEY_POWER

#define ADCKEY_B0       KEY_MENU
#define ADCKEY_B1       KEY_PLUS
#define ADCKEY_B2       KEY_EXIT

#define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(FengYuan_Project))
#if (MS_BOARD_TYPE_SEL==BD_MST9UHL_V6)
   #define ADCKEY_A0       KEY_MENU
   #define ADCKEY_A1       KEY_PLUS
   #define ADCKEY_A2       KEY_EXIT
   
   #define ADCKEY_B0       KEY_MINUS
   #define ADCKEY_B1       KEY_INCVALUE
   #define ADCKEY_B2       KEY_POWER

   #define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))   

#elif ((PanelType==PanelLSC400FN04)||(defined(ForZhouYang315HP_Project)))
   //For ZhouYang_Project
   #define ADCKEY_A0       KEY_PLUS//KEY_INCVALUE
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE//KEY_PLUS
   
   #define ADCKEY_B0       KEY_MINUS //KEY_DECVALUE
   #define ADCKEY_B1       KEY_MENU
   #define ADCKEY_B2       KEY_EXIT
   
   #define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
//FengYuan_Project
/*
   #define ADCKEY_A0       KEY_MENU
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_PLUS
   
   #define ADCKEY_B0       KEY_EXIT
   #define ADCKEY_B1       KEY_DECVALUE
   #define ADCKEY_B2       KEY_MINUS

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
*/
#elif (defined(M280PU_Project))
   #define ADCKEY_A0       KEY_MINUS
   #define ADCKEY_A1       KEY_EXIT
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_PLUS
   #define ADCKEY_B1       KEY_MENU
   #define ADCKEY_B2       KEY_INCVALUE

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#else
#ifdef Japan_Project

   #define ADCKEY_A0       KEY_EXIT//KEY_MINUS
   #define ADCKEY_A1       KEY_INCVALUE//KEY_MENU
   #define ADCKEY_A2       KEY_MENU//KEY_EXIT
   
   #define ADCKEY_B0       KEY_MINUS//KEY_PLUS
   #define ADCKEY_B1       KEY_DECVALUE//KEY_INCVALUE
   #define ADCKEY_B2       KEY_PLUS//KEY_DECVALUE
   
   #define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#else
   #define ADCKEY_A0       KEY_MENU//KEY_MINUS
   #define ADCKEY_A1       KEY_DECVALUE//KEY_INCVALUE
   #define ADCKEY_A2       KEY_MINUS//KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_EXIT
   #define ADCKEY_B1       KEY_INCVALUE//KEY_PLUS
   #define ADCKEY_B2       KEY_PLUS//KEY_MENU

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#endif

#endif

#elif (defined(RunFeng_Project))
#if (PanelType==PanelM350DVR01_01||PanelType==PanelT430QVN01)
   #define ADCKEY_A0       KEY_PLUS
   #define ADCKEY_A1       KEY_DECVALUE
   #define ADCKEY_A2       KEY_INCVALUE
   
   #define ADCKEY_B0       KEY_MINUS
   #define ADCKEY_B1       KEY_MENU//KEY_PLUS
   #define ADCKEY_B2       KEY_POWER

   #define HW_POWER_KEY       1// (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
   #define HW_GPIO_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

/*
   #define ADCKEY_A0       KEY_MINUS
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_PLUS//KEY_MENU
   #define ADCKEY_B1       KEY_MENU//KEY_PLUS
   #define ADCKEY_B2       KEY_EXIT

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
*/
#elif (PanelType==PanelT500QVR05)
   #define ADCKEY_A0       KEY_MENU
   #define ADCKEY_A1       KEY_PLUS
   #define ADCKEY_A2       KEY_EXIT
   
   #define ADCKEY_B0       KEY_INCVALUE
   #define ADCKEY_B1       KEY_MINUS
   #define ADCKEY_B2       KEY_POWER

   #define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))   
#else
   #define ADCKEY_A0       KEY_EXIT
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_MENU
   #define ADCKEY_B1       KEY_PLUS
   #define ADCKEY_B2       KEY_MINUS

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
#endif

#elif (defined(KunDe_Project))
   #define ADCKEY_A0       KEY_MINUS
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_PLUS
   #define ADCKEY_B1       KEY_EXIT
   #define ADCKEY_B2       KEY_MENU

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(LianHeChuangXin_Project))
#if AIO_Board
  #if defined(GK3B_Project)
   #define ADCKEY_A0       KEY_EXIT
   #define ADCKEY_A1       KEY_PLUS
   #define ADCKEY_A2       KEY_MENU
   
   #define ADCKEY_B0       KEY_INCVALUE
   #define ADCKEY_B1       KEY_MINUS
   #define ADCKEY_B2       KEY_POWER

   #define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))   
  #else
   #define ADCKEY_A0       KEY_POWER
   #define ADCKEY_A1       KEY_PLUS
   #define ADCKEY_A2       KEY_EXIT
   
   #define ADCKEY_B0       KEY_INCVALUE
   #define ADCKEY_B1       KEY_MENU
   #define ADCKEY_B2       KEY_MINUS

   #define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))
   #endif
#elif (MS_BOARD_TYPE_SEL==BD_MST9UHL_V6)
   #define ADCKEY_A0       KEY_PLUS
   #define ADCKEY_A1       KEY_DECVALUE
   #define ADCKEY_A2       KEY_POWER
   
   #define ADCKEY_B0       KEY_MENU
   #define ADCKEY_B1       KEY_MINUS
   #define ADCKEY_B2       KEY_EXIT
   #define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))   
#else
#if 0//Dayu
   #define ADCKEY_A0       KEY_EXIT
   #define ADCKEY_A1       KEY_PLUS
   #define ADCKEY_A2       KEY_NOTHING
   
   #define ADCKEY_B0       KEY_POWER
   #define ADCKEY_B1       KEY_MENU
   #define ADCKEY_B2       KEY_MINUS
#else
   #define ADCKEY_A0       KEY_MENU
   #define ADCKEY_A1       KEY_MINUS
   #define ADCKEY_A2       KEY_EXIT
   
   #define ADCKEY_B0       KEY_INCVALUE
   #define ADCKEY_B1       KEY_PLUS
   #define ADCKEY_B2       KEY_POWER
#endif
   #define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))   
#endif

#elif (defined(XinZuoEr_Project))
   #define ADCKEY_A0       KEY_PLUS
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_MENU
   #define ADCKEY_B1       KEY_MINUS
   #define ADCKEY_B2       KEY_EXIT

   #define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(DaYu_Project))
   #define ADCKEY_A0       KEY_EXIT
   #define ADCKEY_A1       KEY_PLUS
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_POWER
   #define ADCKEY_B1       KEY_MENU
   #define ADCKEY_B2       KEY_MINUS

   #define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))   

#elif ((defined(HongHan_Project))&&(MS_BOARD_TYPE_SEL==BD_MST9UHL_V6))
   #define ADCKEY_A0       KEY_MENU
   #define ADCKEY_A1       KEY_PLUS
   #define ADCKEY_A2       KEY_EXIT
   
   #define ADCKEY_B0       KEY_MINUS
   #define ADCKEY_B1       KEY_INCVALUE
   #define ADCKEY_B2       KEY_POWER

   #define HW_POWER_KEY        1//(_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))   
#elif (defined(XuLi_Project))
   #define ADCKEY_A0       KEY_EXIT
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_MENU
   #define ADCKEY_B1       KEY_PLUS
   #define ADCKEY_B2       KEY_MINUS

   #define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(MingCai_Project))

   #define ADCKEY_A0       KEY_EXIT
   #define ADCKEY_A1       KEY_DECVALUE
   #define ADCKEY_A2       KEY_INCVALUE 
   
   #define ADCKEY_B0       KEY_MENU
   #define ADCKEY_B1       KEY_MINUS
   #define ADCKEY_B2       KEY_PLUS

   #define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif(defined(Changjia_Project))
   #define ADCKEY_A0       KEY_DECVALUE//KEY_MINUS
   #define ADCKEY_A1       KEY_PLUS//KEY_INCVALUE
   #define ADCKEY_A2       KEY_EXIT//KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_MENU//KEY_EXIT
   #define ADCKEY_B1       KEY_INCVALUE//KEY_PLUS
   #define ADCKEY_B2       KEY_MINUS//KEY_MENU


#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(HongTian_Project))
   #define ADCKEY_A0       KEY_EXIT
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_PLUS
   #define ADCKEY_B1       KEY_MINUS
   #define ADCKEY_B2       KEY_MENU

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#elif (defined(ZhongXinKeJi_Project))
   #define ADCKEY_A0       KEY_MENU//KEY_MINUS//KEY_MENU
   #define ADCKEY_A1       KEY_NOTHING//KEY_MINUS
   #define ADCKEY_A2       KEY_NOTHING//KEY_EXIT
   
   #define ADCKEY_B0       KEY_PLUS//KEY_INCVALUE
   #define ADCKEY_B1       KEY_EXIT//KEY_PLUS
   #define ADCKEY_B2       KEY_MINUS//KEY_MENU//KEY_POWER

   #define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))


#else
   #if 0    //  shao jin  custer
   #define ADCKEY_A0       KEY_PLUS
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_MINUS
   #define ADCKEY_B1       KEY_MENU
   #define ADCKEY_B2       KEY_EXIT
   #else
   #define ADCKEY_A0       KEY_MINUS
   #define ADCKEY_A1       KEY_INCVALUE
   #define ADCKEY_A2       KEY_DECVALUE
   
   #define ADCKEY_B0       KEY_EXIT
   #define ADCKEY_B1       KEY_PLUS
   #define ADCKEY_B2       KEY_MENU
   #endif

#define HW_POWER_KEY        (_bit4_(MEM_MSREAD_BYTE(_REG_GPIO0_IN)))

#endif



