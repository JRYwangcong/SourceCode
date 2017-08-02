#if (PQ_GRULE_NR_ENABLE) || (PQ_GRULE_SR_ENABLE)
typedef enum
{
#if PQ_GRULE_NR_ENABLE
    PQ_GRule_NR_Sub,
#endif

#if PQ_GRULE_SR_ENABLE
    PQ_GRule_SR_Sub,
#endif

}
MST_GRule_Index_Sub;
#endif

#if PQ_GRULE_NR_ENABLE
typedef enum
{
    PQ_GRule_NR_Off_Sub,
    PQ_GRule_NR_Low_Sub,
    PQ_GRule_NR_Middle_Sub,
    PQ_GRule_NR_High_Sub,
}
MST_GRule_NR_Index_Sub;
#endif

#if PQ_GRULE_SR_ENABLE
typedef enum
{
    PQ_GRule_SR_Off_Sub,
    PQ_GRule_SR_UHD_Sub,
    PQ_GRule_SR_High_Plus_Sub,
    PQ_GRule_SR_High_Sub,
    PQ_GRule_SR_Middle_Plus_Sub,
    PQ_GRule_SR_Middle_Sub,
    PQ_GRule_SR_Low_Sub,
    PQ_GRule_SR_DownScaling_Sub,
    PQ_GRule_SR_Normal_High_Sub,
    PQ_GRule_SR_Normal_Middle_Sub,
    PQ_GRule_SR_Normal_Low_Sub,
}
MST_GRule_SR_Index_Sub;
#endif

#if PQ_GRULE_NR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_NR_Off_Sub,
    PQ_GRule_Lvl_NR_Low_Sub,
    PQ_GRule_Lvl_NR_Middle_Sub,
    PQ_GRule_Lvl_NR_High_Sub,
}
MST_GRule_NR_LvL_Index_Sub;
#endif

#if PQ_GRULE_SR_ENABLE
typedef enum
{
    PQ_GRule_Lvl_SR_Off_Sub,
    PQ_GRule_Lvl_SR_UHD_Sub,
    PQ_GRule_Lvl_SR_High_Plus_Sub,
    PQ_GRule_Lvl_SR_High_Sub,
    PQ_GRule_Lvl_SR_Middle_Plus_Sub,
    PQ_GRule_Lvl_SR_Middle_Sub,
    PQ_GRule_Lvl_SR_Low_Sub,
    PQ_GRule_Lvl_SR_DownScaling_Sub,
    PQ_GRule_Lvl_SR_Normal_High_Sub,
    PQ_GRule_Lvl_SR_Normal_Middle_Sub,
    PQ_GRule_Lvl_SR_Normal_Low_Sub,
}
MST_GRule_SR_LvL_Index_Sub;
#endif


#define PQ_GRULE_RULE_NUM_Sub 2

#if PQ_GRULE_NR_ENABLE
#define PQ_GRULE_NR_IP_NUM_Sub 3
#define PQ_GRULE_NR_NUM_Sub 4
#define PQ_GRULE_NR_LVL_NUM_Sub 4
#endif

#if PQ_GRULE_SR_ENABLE
#define PQ_GRULE_SR_IP_NUM_Sub 12
#define PQ_GRULE_SR_NUM_Sub 11
#define PQ_GRULE_SR_LVL_NUM_Sub 11
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_NR_IP_Index_Sub[PQ_GRULE_NR_IP_NUM_Sub];
extern const U8 MST_GRule_NR_Sub[QM_INPUTTYPE_NUM_Sub][PQ_GRULE_NR_NUM_Sub][PQ_GRULE_NR_IP_NUM_Sub];
#endif

#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_SR_IP_Index_Sub[PQ_GRULE_SR_IP_NUM_Sub];
extern const U8 MST_GRule_SR_Sub[QM_INPUTTYPE_NUM_Sub][PQ_GRULE_SR_NUM_Sub][PQ_GRULE_SR_IP_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_3840_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_3840_SR_Sub[PQ_GRULE_SR_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_3840_PIP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_3840_PIP_SR_Sub[PQ_GRULE_SR_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_3840_POP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_3840_POP_SR_Sub[PQ_GRULE_SR_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_2560_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_2560_SR_Sub[PQ_GRULE_SR_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_2560_PIP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_2560_PIP_SR_Sub[PQ_GRULE_SR_LVL_NUM_Sub];
#endif

#if PQ_GRULE_NR_ENABLE
extern const U8 MST_GRule_2560_POP_NR_Sub[PQ_GRULE_NR_LVL_NUM_Sub];
#endif
#if PQ_GRULE_SR_ENABLE
extern const U8 MST_GRule_2560_POP_SR_Sub[PQ_GRULE_SR_LVL_NUM_Sub];
#endif

