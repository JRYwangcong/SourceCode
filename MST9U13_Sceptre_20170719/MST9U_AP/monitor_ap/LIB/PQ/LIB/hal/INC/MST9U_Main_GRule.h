#if (PQ_GRULE_NR_ENABLE) || (PQ_GRULE_SR_ENABLE) || (PQ_GRULE_OSD_BW_ENABLE) || (PQ_GRULE_MPEG_NR_ENABLE) || (PQ_GRULE_FILM_MODE_ENABLE) || (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE) || (PQ_GRULE_ULTRAT_CLEAR_ENABLE) || (PQ_GRULE_BYPASS_NR_ENABLE) || (PQ_GRULE_BYPASS_DI_ENABLE) || (PQ_GRULE_BYPASS_PEAKING_ENABLE) || (PQ_GRULE_BYPASS_OTHERS_ENABLE)
typedef enum
{
#if PQ_GRULE_NR_ENABLE
    PQ_GRule_NR_Main,
#endif

#if PQ_GRULE_SR_ENABLE
    PQ_GRule_SR_Main,
#endif

#if PQ_GRULE_OSD_BW_ENABLE
    PQ_GRule_OSD_BW_Main,
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
    PQ_GRule_MPEG_NR_Main,
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
    PQ_GRule_FILM_MODE_Main,
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
    PQ_GRule_DYNAMIC_CONTRAST_Main,
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
    PQ_GRule_ULTRAT_CLEAR_Main,
#endif

#if PQ_GRULE_BYPASS_NR_ENABLE
    PQ_GRule_Bypass_NR_Main,
#endif

#if PQ_GRULE_BYPASS_DI_ENABLE
    PQ_GRule_Bypass_DI_Main,
#endif

#if PQ_GRULE_BYPASS_PEAKING_ENABLE
    PQ_GRule_Bypass_Peaking_Main,
#endif

#if PQ_GRULE_BYPASS_OTHERS_ENABLE
    PQ_GRule_Bypass_Others_Main,
#endif

}
MST_GRule_Index_Main;
#endif

#if PQ_GRULE_NR_ENABLE
typedef enum
{
    PQ_GRule_NR_Off_Main,
    PQ_GRule_NR_Low_Main,
    PQ_GRule_NR_Middle_Main,
    PQ_GRule_NR_High_Main,
    PQ_GRule_NR_Low_PIP_Main,
    PQ_GRule_NR_Middle_PIP_Main,
    PQ_GRule_NR_High_PIP_Main,
}
MST_GRule_NR_Index_Main;
#endif

#if PQ_GRULE_SR_ENABLE
typedef enum
{
    PQ_GRule_SR_Off_Main,
    PQ_GRule_SR_UHD_Main,
    PQ_GRule_SR_High_Plus_Main,
    PQ_GRule_SR_High_Main,
    PQ_GRule_SR_Middle_Plus_Main,
    PQ_GRule_SR_Middle_Main,
    PQ_GRule_SR_Low_Main,
    PQ_GRule_SR_DownScaling_Main,
    PQ_GRule_SR_Normal_High_Main,
    PQ_GRule_SR_Normal_Middle_Main,
    PQ_GRule_SR_Normal_Low_Main,
}
MST_GRule_SR_Index_Main;
#endif

#if PQ_GRULE_OSD_BW_ENABLE
typedef enum
{
    PQ_GRule_OSD_BW_On_3840_Main,
    PQ_GRule_OSD_BW_On_3840_PIP_Main,
    PQ_GRule_OSD_BW_On_3840_POP_Main,
    PQ_GRule_OSD_BW_On_2560_Main,
    PQ_GRule_OSD_BW_On_2560_PIP_Main,
    PQ_GRule_OSD_BW_On_2560_POP_Main,
}
MST_GRule_OSD_BW_Index_Main;
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
typedef enum
{
    PQ_GRule_MPEG_NR_Off_Main,
    PQ_GRule_MPEG_NR_Low_Main,
    PQ_GRule_MPEG_NR_Middle_Main,
    PQ_GRule_MPEG_NR_High_Main,
}
MST_GRule_MPEG_NR_Index_Main;
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
typedef enum
{
    PQ_GRule_FILM_MODE_Off_Main,
    PQ_GRule_FILM_MODE_On_Main,
}
MST_GRule_FILM_MODE_Index_Main;
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
typedef enum
{
    PQ_GRule_DYNAMIC_CONTRAST_Off_Main,
    PQ_GRule_DYNAMIC_CONTRAST_On_Main,
}
MST_GRule_DYNAMIC_CONTRAST_Index_Main;
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
typedef enum
{
    PQ_GRule_ULTRAT_CLEAR_Off_Main,
    PQ_GRule_ULTRAT_CLEAR_On_Main,
}
MST_GRule_ULTRAT_CLEAR_Index_Main;
#endif

#if PQ_GRULE_BYPASS_NR_ENABLE
typedef enum
{
    PQ_GRule_Bypass_NR_Off_Main,
    PQ_GRule_Bypass_NR_On_Main,
}
MST_GRule_Bypass_NR_Index_Main;
#endif

#if PQ_GRULE_BYPASS_DI_ENABLE
typedef enum
{
    PQ_GRule_Bypass_DI_Off_Main,
    PQ_GRule_Bypass_DI_On_Main,
}
MST_GRule_Bypass_DI_Index_Main;
#endif

#if PQ_GRULE_BYPASS_PEAKING_ENABLE
typedef enum
{
    PQ_GRule_Bypass_Peaking_Off_Main,
    PQ_GRule_Bypass_Peaking_On_Main,
}
MST_GRule_Bypass_Peaking_Index_Main;
#endif

#if PQ_GRULE_BYPASS_OTHERS_ENABLE
typedef enum
{
    PQ_GRule_Bypass_Others_Off_Main,
    PQ_GRule_Bypass_Others_On_Main,
}
MST_GRule_Bypass_Others_Index_Main;
#endif

#if PQ_GRULE_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_NR_Off_Main,
    PQ_GRule_Lvl_NR_Low_Main,
    PQ_GRule_Lvl_NR_Middle_Main,
    PQ_GRule_Lvl_NR_High_Main,
}
MST_GRule_NR_LvL_Index_Main;
#endif

#if PQ_GRULE_SR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_SR_Off_Main,
    PQ_GRule_Lvl_SR_UHD_Main,
    PQ_GRule_Lvl_SR_High_Plus_Main,
    PQ_GRule_Lvl_SR_High_Main,
    PQ_GRule_Lvl_SR_Middle_Plus_Main,
    PQ_GRule_Lvl_SR_Middle_Main,
    PQ_GRule_Lvl_SR_Low_Main,
    PQ_GRule_Lvl_SR_DownScaling_Main,
    PQ_GRule_Lvl_SR_Normal_High_Main,
    PQ_GRule_Lvl_SR_Normal_Middle_Main,
    PQ_GRule_Lvl_SR_Normal_Low_Main,
}
MST_GRule_SR_LvL_Index_Main;
#endif

#if PQ_GRULE_OSD_BW_ENABLE
typedef enum
{
    PQ_GRule_Lvl_OSD_BW_On_Main,
}
MST_GRule_OSD_BW_LvL_Index_Main;
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_MPEG_NR_Off_Main,
    PQ_GRule_Lvl_MPEG_NR_Low_Main,
    PQ_GRule_Lvl_MPEG_NR_Middle_Main,
    PQ_GRule_Lvl_MPEG_NR_High_Main,
}
MST_GRule_MPEG_NR_LvL_Index_Main;
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
typedef enum
{
    PQ_GRule_Lvl_FILM_MODE_Off_Main,
    PQ_GRule_Lvl_FILM_MODE_On_Main,
}
MST_GRule_FILM_MODE_LvL_Index_Main;
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
typedef enum
{
    PQ_GRule_Lvl_DYNAMIC_CONTRAST_Off_Main,
    PQ_GRule_Lvl_DYNAMIC_CONTRAST_On_Main,
}
MST_GRule_DYNAMIC_CONTRAST_LvL_Index_Main;
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_ULTRAT_CLEAR_Off_Main,
    PQ_GRule_Lvl_ULTRAT_CLEAR_On_Main,
}
MST_GRule_ULTRAT_CLEAR_LvL_Index_Main;
#endif

#if PQ_GRULE_BYPASS_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_Bypass_NR_Off_Main,
    PQ_GRule_Lvl_Bypass_NR_On_Main,
}
MST_GRule_Bypass_NR_LvL_Index_Main;
#endif

#if PQ_GRULE_BYPASS_DI_ENABLE
typedef enum
{
    PQ_GRule_Lvl_Bypass_DI_Off_Main,
    PQ_GRule_Lvl_Bypass_DI_On_Main,
}
MST_GRule_Bypass_DI_LvL_Index_Main;
#endif

#if PQ_GRULE_BYPASS_PEAKING_ENABLE
typedef enum
{
    PQ_GRule_Lvl_Bypass_Peaking_Off_Main,
    PQ_GRule_Lvl_Bypass_Peaking_On_Main,
}
MST_GRule_Bypass_Peaking_LvL_Index_Main;
#endif

#if PQ_GRULE_BYPASS_OTHERS_ENABLE
typedef enum
{
    PQ_GRule_Lvl_Bypass_Others_Off_Main,
    PQ_GRule_Lvl_Bypass_Others_On_Main,
}
MST_GRule_Bypass_Others_LvL_Index_Main;
#endif


#define PQ_GRULE_RULE_NUM_Main 11

#if PQ_GRULE_NR_ENABLE
#define PQ_GRULE_NR_IP_NUM_Main 5
#define PQ_GRULE_NR_NUM_Main 7
#define PQ_GRULE_NR_LVL_NUM_Main 4
#endif

#if PQ_GRULE_SR_ENABLE
#define PQ_GRULE_SR_IP_NUM_Main 12
#define PQ_GRULE_SR_NUM_Main 11
#define PQ_GRULE_SR_LVL_NUM_Main 11
#endif

#if PQ_GRULE_OSD_BW_ENABLE
#define PQ_GRULE_OSD_BW_IP_NUM_Main 1
#define PQ_GRULE_OSD_BW_NUM_Main 6
#define PQ_GRULE_OSD_BW_LVL_NUM_Main 1
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
#define PQ_GRULE_MPEG_NR_IP_NUM_Main 7
#define PQ_GRULE_MPEG_NR_NUM_Main 4
#define PQ_GRULE_MPEG_NR_LVL_NUM_Main 4
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
#define PQ_GRULE_FILM_MODE_IP_NUM_Main 3
#define PQ_GRULE_FILM_MODE_NUM_Main 2
#define PQ_GRULE_FILM_MODE_LVL_NUM_Main 2
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
#define PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Main 1
#define PQ_GRULE_DYNAMIC_CONTRAST_NUM_Main 2
#define PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Main 2
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
#define PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main 5
#define PQ_GRULE_ULTRAT_CLEAR_NUM_Main 2
#define PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main 2
#endif

#if PQ_GRULE_BYPASS_NR_ENABLE
#define PQ_GRULE_Bypass_NR_IP_NUM_Main 12
#define PQ_GRULE_Bypass_NR_NUM_Main 2
#define PQ_GRULE_Bypass_NR_LVL_NUM_Main 2
#endif

#if PQ_GRULE_BYPASS_DI_ENABLE
#define PQ_GRULE_Bypass_DI_IP_NUM_Main 5
#define PQ_GRULE_Bypass_DI_NUM_Main 2
#define PQ_GRULE_Bypass_DI_LVL_NUM_Main 2
#endif

#if PQ_GRULE_BYPASS_PEAKING_ENABLE
#define PQ_GRULE_Bypass_Peaking_IP_NUM_Main 8
#define PQ_GRULE_Bypass_Peaking_NUM_Main 2
#define PQ_GRULE_Bypass_Peaking_LVL_NUM_Main 2
#endif

#if PQ_GRULE_BYPASS_OTHERS_ENABLE
#define PQ_GRULE_Bypass_Others_IP_NUM_Main 10
#define PQ_GRULE_Bypass_Others_NUM_Main 2
#define PQ_GRULE_Bypass_Others_LVL_NUM_Main 2
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_NR_IP_Index_Main[PQ_GRULE_NR_IP_NUM_Main];
extern const U8 MST_GRule_NR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_NR_NUM_Main][PQ_GRULE_NR_IP_NUM_Main];
#endif

#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_SR_IP_Index_Main[PQ_GRULE_SR_IP_NUM_Main];
extern const U8 MST_GRule_SR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_SR_NUM_Main][PQ_GRULE_SR_IP_NUM_Main];
#endif

#if PQ_GRULE_OSD_BW_ENABLE
extern const U8 MST_GRule_OSD_BW_IP_Index_Main[PQ_GRULE_OSD_BW_IP_NUM_Main];
extern const U8 MST_GRule_OSD_BW_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_OSD_BW_NUM_Main][PQ_GRULE_OSD_BW_IP_NUM_Main];
#endif

#if PQ_GRULE_MPEG_NR_ENABLE
extern const U8 MST_GRule_MPEG_NR_IP_Index_Main[PQ_GRULE_MPEG_NR_IP_NUM_Main];
extern const U8 MST_GRule_MPEG_NR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_MPEG_NR_NUM_Main][PQ_GRULE_MPEG_NR_IP_NUM_Main];
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
extern const U8 MST_GRule_FILM_MODE_IP_Index_Main[PQ_GRULE_FILM_MODE_IP_NUM_Main];
extern const U8 MST_GRule_FILM_MODE_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_FILM_MODE_NUM_Main][PQ_GRULE_FILM_MODE_IP_NUM_Main];
#endif

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern const U8 MST_GRule_DYNAMIC_CONTRAST_IP_Index_Main[PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Main];
extern const U8 MST_GRule_DYNAMIC_CONTRAST_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_DYNAMIC_CONTRAST_NUM_Main][PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Main];
#endif

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern const U8 MST_GRule_ULTRAT_CLEAR_IP_Index_Main[PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main];
extern const U8 MST_GRule_ULTRAT_CLEAR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_ULTRAT_CLEAR_NUM_Main][PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main];
#endif

#if PQ_GRULE_BYPASS_NR_ENABLE
extern const U8 MST_GRule_Bypass_NR_IP_Index_Main[PQ_GRULE_Bypass_NR_IP_NUM_Main];
extern const U8 MST_GRule_Bypass_NR_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_Bypass_NR_NUM_Main][PQ_GRULE_Bypass_NR_IP_NUM_Main];
#endif

#if PQ_GRULE_BYPASS_DI_ENABLE
extern const U8 MST_GRule_Bypass_DI_IP_Index_Main[PQ_GRULE_Bypass_DI_IP_NUM_Main];
extern const U8 MST_GRule_Bypass_DI_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_Bypass_DI_NUM_Main][PQ_GRULE_Bypass_DI_IP_NUM_Main];
#endif

#if PQ_GRULE_BYPASS_PEAKING_ENABLE
extern const U8 MST_GRule_Bypass_Peaking_IP_Index_Main[PQ_GRULE_Bypass_Peaking_IP_NUM_Main];
extern const U8 MST_GRule_Bypass_Peaking_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_Bypass_Peaking_NUM_Main][PQ_GRULE_Bypass_Peaking_IP_NUM_Main];
#endif

#if PQ_GRULE_BYPASS_OTHERS_ENABLE
extern const U8 MST_GRule_Bypass_Others_IP_Index_Main[PQ_GRULE_Bypass_Others_IP_NUM_Main];
extern const U8 MST_GRule_Bypass_Others_Main[QM_INPUTTYPE_NUM_Main][PQ_GRULE_Bypass_Others_NUM_Main][PQ_GRULE_Bypass_Others_IP_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_3840_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_3840_SR_Main[PQ_GRULE_SR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern const U8 MST_GRule_3840_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern const U8 MST_GRule_3840_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern const U8 MST_GRule_3840_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern const U8 MST_GRule_3840_DYNAMIC_CONTRAST_Main[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern const U8 MST_GRule_3840_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_NR_ENABLE
extern const U8 MST_GRule_3840_Bypass_NR_Main[PQ_GRULE_Bypass_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_DI_ENABLE
extern const U8 MST_GRule_3840_Bypass_DI_Main[PQ_GRULE_Bypass_DI_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_PEAKING_ENABLE
extern const U8 MST_GRule_3840_Bypass_Peaking_Main[PQ_GRULE_Bypass_Peaking_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_OTHERS_ENABLE
extern const U8 MST_GRule_3840_Bypass_Others_Main[PQ_GRULE_Bypass_Others_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_3840_PIP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_3840_PIP_SR_Main[PQ_GRULE_SR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern const U8 MST_GRule_3840_PIP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern const U8 MST_GRule_3840_PIP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern const U8 MST_GRule_3840_PIP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern const U8 MST_GRule_3840_PIP_DYNAMIC_CONTRAST_Main[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern const U8 MST_GRule_3840_PIP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_NR_ENABLE
extern const U8 MST_GRule_3840_PIP_Bypass_NR_Main[PQ_GRULE_Bypass_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_DI_ENABLE
extern const U8 MST_GRule_3840_PIP_Bypass_DI_Main[PQ_GRULE_Bypass_DI_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_PEAKING_ENABLE
extern const U8 MST_GRule_3840_PIP_Bypass_Peaking_Main[PQ_GRULE_Bypass_Peaking_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_OTHERS_ENABLE
extern const U8 MST_GRule_3840_PIP_Bypass_Others_Main[PQ_GRULE_Bypass_Others_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_3840_POP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_3840_POP_SR_Main[PQ_GRULE_SR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern const U8 MST_GRule_3840_POP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern const U8 MST_GRule_3840_POP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern const U8 MST_GRule_3840_POP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern const U8 MST_GRule_3840_POP_DYNAMIC_CONTRAST_Main[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern const U8 MST_GRule_3840_POP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_NR_ENABLE
extern const U8 MST_GRule_3840_POP_Bypass_NR_Main[PQ_GRULE_Bypass_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_DI_ENABLE
extern const U8 MST_GRule_3840_POP_Bypass_DI_Main[PQ_GRULE_Bypass_DI_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_PEAKING_ENABLE
extern const U8 MST_GRule_3840_POP_Bypass_Peaking_Main[PQ_GRULE_Bypass_Peaking_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_OTHERS_ENABLE
extern const U8 MST_GRule_3840_POP_Bypass_Others_Main[PQ_GRULE_Bypass_Others_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_2560_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_2560_SR_Main[PQ_GRULE_SR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern const U8 MST_GRule_2560_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern const U8 MST_GRule_2560_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern const U8 MST_GRule_2560_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern const U8 MST_GRule_2560_DYNAMIC_CONTRAST_Main[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern const U8 MST_GRule_2560_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_NR_ENABLE
extern const U8 MST_GRule_2560_Bypass_NR_Main[PQ_GRULE_Bypass_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_DI_ENABLE
extern const U8 MST_GRule_2560_Bypass_DI_Main[PQ_GRULE_Bypass_DI_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_PEAKING_ENABLE
extern const U8 MST_GRule_2560_Bypass_Peaking_Main[PQ_GRULE_Bypass_Peaking_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_OTHERS_ENABLE
extern const U8 MST_GRule_2560_Bypass_Others_Main[PQ_GRULE_Bypass_Others_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_2560_PIP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_2560_PIP_SR_Main[PQ_GRULE_SR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern const U8 MST_GRule_2560_PIP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern const U8 MST_GRule_2560_PIP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern const U8 MST_GRule_2560_PIP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern const U8 MST_GRule_2560_PIP_DYNAMIC_CONTRAST_Main[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern const U8 MST_GRule_2560_PIP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_NR_ENABLE
extern const U8 MST_GRule_2560_PIP_Bypass_NR_Main[PQ_GRULE_Bypass_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_DI_ENABLE
extern const U8 MST_GRule_2560_PIP_Bypass_DI_Main[PQ_GRULE_Bypass_DI_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_PEAKING_ENABLE
extern const U8 MST_GRule_2560_PIP_Bypass_Peaking_Main[PQ_GRULE_Bypass_Peaking_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_OTHERS_ENABLE
extern const U8 MST_GRule_2560_PIP_Bypass_Others_Main[PQ_GRULE_Bypass_Others_LVL_NUM_Main];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_2560_POP_NR_Main[PQ_GRULE_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_2560_POP_SR_Main[PQ_GRULE_SR_LVL_NUM_Main];
#endif
#if PQ_GRULE_OSD_BW_ENABLE
extern const U8 MST_GRule_2560_POP_OSD_BW_Main[PQ_GRULE_OSD_BW_LVL_NUM_Main];
#endif
#if PQ_GRULE_MPEG_NR_ENABLE
extern const U8 MST_GRule_2560_POP_MPEG_NR_Main[PQ_GRULE_MPEG_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_FILM_MODE_ENABLE
extern const U8 MST_GRule_2560_POP_FILM_MODE_Main[PQ_GRULE_FILM_MODE_LVL_NUM_Main];
#endif
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
extern const U8 MST_GRule_2560_POP_DYNAMIC_CONTRAST_Main[PQ_GRULE_DYNAMIC_CONTRAST_LVL_NUM_Main];
#endif
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
extern const U8 MST_GRule_2560_POP_ULTRAT_CLEAR_Main[PQ_GRULE_ULTRAT_CLEAR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_NR_ENABLE
extern const U8 MST_GRule_2560_POP_Bypass_NR_Main[PQ_GRULE_Bypass_NR_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_DI_ENABLE
extern const U8 MST_GRule_2560_POP_Bypass_DI_Main[PQ_GRULE_Bypass_DI_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_PEAKING_ENABLE
extern const U8 MST_GRule_2560_POP_Bypass_Peaking_Main[PQ_GRULE_Bypass_Peaking_LVL_NUM_Main];
#endif
#if PQ_GRULE_BYPASS_OTHERS_ENABLE
extern const U8 MST_GRule_2560_POP_Bypass_Others_Main[PQ_GRULE_Bypass_Others_LVL_NUM_Main];
#endif

