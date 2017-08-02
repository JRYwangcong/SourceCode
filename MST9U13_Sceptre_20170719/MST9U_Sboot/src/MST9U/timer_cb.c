#include <stdio.h>
#include "assert.h"
//#include "mhal_arch.h"
#include "risc32.h"
#include "datatype.h"

typedef unsigned long               MS_U32;
typedef unsigned char               MS_U8;
typedef signed long               MS_S32;
typedef unsigned char               MS_BOOL;
typedef void (*TimerCb)(MS_U32 u32Vector);

volatile static TimerCb timer_isr[2] = {0, 0};
volatile static MS_U32 timer_period[2]={1, 1};
volatile static MS_BOOL gStartTimer[2]={0, 0};
volatile static MS_U32 firsttimer_interval[2]={0, 0};
volatile static MS_U32 firsttimer_interval_old[2]={0, 0};
volatile static MS_BOOL firsttimer_interval_flag[2]={0, 0};
volatile static MS_U32 gsystem_ip_time_ms[2]={0, 0};
extern volatile MS_U32 gsystem_time_ms;
//MS_U8 gtimer_cb_number;
void Timer_firsttime_interval(MS_U32 firsttime_interval, MS_S32 s32TimerId_set);
void Timer_firsttime_interval(MS_U32 firsttime_interval, MS_S32 s32TimerId_set)
{
    firsttimer_interval_flag[s32TimerId_set]=0;
    firsttimer_interval_old[s32TimerId_set]=0;
    gsystem_ip_time_ms[s32TimerId_set]=0;
    firsttimer_interval[s32TimerId_set]=firsttime_interval;

}
void Timer_Start_set(MS_BOOL bStartTimer_set, MS_S32 s32TimerId_set);
void Timer_Start_set(MS_BOOL bStartTimer_set, MS_S32 s32TimerId_set)
{
    gStartTimer[s32TimerId_set]=bStartTimer_set;
}
void Timer_Period_Register(MS_U32 periodCb, MS_S32 s32TimerId_set);
void Timer_Period_Register(MS_U32 periodCb, MS_S32 s32TimerId_set)
{

    //printf("Timer size of = %lx\n",sizeof(timer_period));
    if(s32TimerId_set>=2)
    {
        printf("Not enough buffer to set period!!!!!\n");
    }
    if(periodCb==0)
    {
        periodCb=1;
    }
    timer_period[s32TimerId_set]=periodCb;

}
MS_S32 Timer_IRQ_Register(void *ptCb);
MS_S32 Timer_IRQ_Register(void *ptCb)
{
    int i;
    //printf("Timer size of = %lx\n",sizeof(timer_isr));

    for(i=0;i<=(sizeof(timer_isr)>>2);i++)
    {
        if(i==((sizeof(timer_isr))>>2))
        {
            printf("Not enough buffer to attach timerCB!!!!!\n");
            ASSERT(0);
        }
        if(timer_isr[i]==0)
        {
            timer_isr[i]=(TimerCb)ptCb;
            break;
        }
    }
    return i;
}
void Timer_IRQ_Delete(MS_S32 timer_ld);
void Timer_IRQ_Delete(MS_S32 timer_ld)
{
    if(timer_isr[timer_ld]!=0)
    {
        timer_isr[timer_ld]=0;
        gsystem_ip_time_ms[timer_ld]=0;
    }
}

void  ProcessTimer0Isr( MHAL_SavedRegisters *pHalReg, U32 vector );
void  ProcessTimer0Isr( MHAL_SavedRegisters *pHalReg, U32 vector )
{
    int i;
    UNUSED(pHalReg);
    UNUSED(vector);

    gsystem_time_ms++;
    for(i=0;i<2;i++)
        {
        if (timer_isr[i])
            {
             if((firsttimer_interval_flag[i]==0)&&(gStartTimer[i]==1))
                {
                if(firsttimer_interval_old[i]==0)
                    {
                        firsttimer_interval_old[i]=gsystem_time_ms;
                    }
                if(((gsystem_time_ms<firsttimer_interval_old[i])&&((0xFFFFFFFF-firsttimer_interval_old[i]+gsystem_time_ms)>=firsttimer_interval[i]))||((gsystem_time_ms>firsttimer_interval_old[i])&&((gsystem_time_ms-firsttimer_interval_old[i])>=firsttimer_interval[i])))
                    {
                        firsttimer_interval_flag[i]=1;
                    }
                if(firsttimer_interval[i]==0)
                    {
                        firsttimer_interval_flag[i]=1;
                    }
                }
                if(((gsystem_ip_time_ms[i]%timer_period[i])==0)&&(gStartTimer[i]==1)&&(firsttimer_interval_flag[i]==1))
                    {
                        timer_isr[i](0);
                    }
                if((gStartTimer[i]==1)&&(firsttimer_interval_flag[i]==1))
                    {
                        gsystem_ip_time_ms[i]++;
                    }
            }
        }
}
