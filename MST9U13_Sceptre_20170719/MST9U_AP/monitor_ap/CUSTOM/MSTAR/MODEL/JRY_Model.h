


#define ENABLE_MMD_DCR                                           1




///Language Menu========================================
//Only Three Language


#define Enable_Engliish                  1
#define Enable_German                  0
#define Enable_Spanish                  1
#define Enable_Greek                     0
#define Enable_French                   1
#define Enable_Italian                    0
#define Enable_Hungarian              0
#define Enable_Dutch                      0
#define Enable_Portuguese             0
#define Enable_BrazilPortuguese   0
#define Enable_Polish                       0
#define Enable_Russian                    0
#define Enable_Swedish                  0
#define Enable_Finnish                    0
#define Enable_Turkish                    0
#define Enable_Czech                      0
#define Enable_Ukrainian                 0
#define Enable_SChinese                  0
#define Enable_TChinese                  1
#define Enable_Japanese                 0
#define Enable_Korean                     0



#define DEF_LANGUAGE                  (LANG_English)





#define MAX_MAINMENU_INDEX		9
#define MIN_SUBMENU_INDEX			0
#define MAX_SUBMENU_INDEX			10
#define MAX_HOTMENU_INDEX			10

///==================================================
///==================================================

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

#define MSTAR_LOGO                             0
#define LOGO_RX                                    0
#define LOGO_MONOPRICE                     0// (defined(KunDe_Project))
#define LOGO_TERRA                              0
#define LOGO_UPSTAR                            (0&&(defined(SenYo_Project)))
#define LOGO_HKC                                  0
#define LOGO_ZEOL                                (0&&(defined(XinZuoEr_Project)))
#define LOGO_Neview                             0
#define LOGO_HDC                                  (0&&(defined(HuiGe_Project)))
#define LOGO_JAPANNEXT                      (1&&(defined(MingCai_Project)||defined(HuiGe_Project)||defined(Japan_Project)||defined(XuLi_Project)))
#define LOGO_KOIS                                 (1&&(defined(FengYuan_Project))&&(defined(KOIS_Project)))
#define LOGO_KOGAN                              0//(defined(Kogan_Project))
#define LOGO_KOGAN_NEW                    (defined(Kogan_Project))
#define LOGO_BENZ                                (0&&(defined(RunFeng_Project)))//TIEMU 
#define LOGO_TIEMU                              (1&&(defined(RunFeng_Project)))
#define LOGO_ZEOL_X2                           (1&&(defined(XinZuoEr_Project)))
#define LOGO_X_Star                              (0&&(defined(FengYuan_Project)))
#define LOGO_BUBALUS                          (defined(XingYuan_Project))
#define LOGO_Qnix                                 (0&&(defined(FengYuan_Project)))
#define LOGO_AV                                    (0&&(defined(FengYuan_Project)))
#define LOGO_HYUNDAI                          (1&&(defined(FengYuan_Project))&&(defined(Korean_HYUNDAI_Project)))
#define LOGO_AXM                                   (0&&(defined(JieBo_Project)))
#define LOGO_NIXEUS                             (1&&(defined(JieBo_Project)))
#define LOGO_IMAGIC                              (1&&(defined(LianHeChuangXin_Project))&&(defined(IMA_Project)))
#define LOGO_DMM                                  (1&&(defined(Changjia_Project)))
#define LOGO_VMazon                              (1&&(defined(LianHeChuangXin_Project))&&(defined(Vmazon_Project)))
#define LOGO_Titan                                  (1&&(defined(LianHeChuangXin_Project))&&(defined(Titan_Project)))
#define LOGO_ForGame                            (0&&(defined(LianHeChuangXin_Project))&&(defined(K3D_Project)))
#define LOGO_IIP                                     (1&&(defined(RX_Project)))
#define LOGO_GREATWALL                       (1&&(defined(SenYo_Project)))
#define LOGO_keoushi                              0//   (1&&(defined(KeOuShi_Project)))
#define LOGO_ViewSync                           (0&&(defined(FengYuan_Project)))
#define LOGO_GameMNT                           (1&&(defined(LianHeChuangXin_Project))&&(defined(K3D_Project)))
#define LOGO_SaioSone				(1&&(defined(LianHeChuangXin_Project))&&(defined(GK3B_Project)))
#define LOGO_Sceptre				(0&&(defined(LianHeChuangXin_Project))&&(defined(K3C_Project)))
#define LOGO_TCL                                   (1&&(defined(LianHeChuangXin_Project))&&(defined(V3G_TCL_Project)))
#define LOGO_Sceptre_New                    (1&&(defined(LianHeChuangXin_Project)||(defined(ZhongXinKeJi_Project))))

#define LOGO_ENABLE                   (MSTAR_LOGO|LOGO_RX|LOGO_MONOPRICE|LOGO_TERRA|LOGO_UPSTAR|LOGO_HKC|LOGO_ZEOL||LOGO_ZEOL_X2|LOGO_Neview||LOGO_HDC||LOGO_JAPANNEXT||LOGO_KOIS||LOGO_KOGAN||LOGO_KOGAN_NEW||LOGO_BENZ||LOGO_TIEMU||LOGO_X_Star||LOGO_BUBALUS||LOGO_Qnix||LOGO_AV||LOGO_AXM||LOGO_NIXEUS||LOGO_IMAGIC||LOGO_HYUNDAI||LOGO_DMM||LOGO_VMazon||LOGO_Titan||LOGO_IIP||LOGO_ForGame||LOGO_GREATWALL||LOGO_keoushi||LOGO_ViewSync||LOGO_GameMNT||LOGO_SaioSone||LOGO_Sceptre||LOGO_TCL||LOGO_Sceptre_New)
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------







